/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Agilent Technologies, Inc. All Rights Reserved
 *
 *                         Agilent Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "regAddrs.h"
#include "cpu_api.h"
#include "debug.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scan.h"
#include "scanhw.h"
#include "scanif.h"
#include "scanmech.h"
#include "scancmdq.h"
#include "scanalyzer.h"
#include "scanplat.h"
#include "scantask.h"
#include "scanpower.h"
#include "ostools.h"
#include "scanimg.h"
#include "scanalyzer.h"
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
#include "picdma_descrip.h"
#endif
#include "scands.h"
#include "safetylock.h"
#include "scanlib.h"

//#define CMDQ_STATE_DEBUG
//#define CMDQ_STATE_ISR_DEBUG
#define __DEL_XASSERT__  (1)

/* We have a command queue for scan commands for channels. The scan block
 * itself has no concept of channels but we need to match up PIC DMA buffers to
 * scan line captures.
 */
#define MAX_CMDQ_CHANNELS 6

/** SCANCMD register is actually a 2-deep queue */
#define SCAN_CMD_DEPTH 2

#define MAX_CMDQ_RUNNING SCAN_CMD_DEPTH

/** number of commands we can have queued up waiting to be pushed into the scan
 * hardware SCANCMD register
 */
#define MAX_CMDQ_PENDING  64

//States for the statemachine
typedef enum {  CMDQ_STATE_IDLE=0,
                CMDQ_STATE_READY_FOR_SCAN=1,
                CMDQ_STATE_SCANNING=2,
                CMDQ_STATE_START_PAUSE=3,
                CMDQ_STATE_PAUSED=4,
                CMDQ_STATE_SCAN_STOPPED=5,
             } cmdq_state_t; // trajectory state

static cmdq_state_t cmdq_state;

/* when curr==next, ringbuf is empty
 * when (next+1)%max==curr, ringbuf is full
 *
 * Always need one extra slot in the array to differentiate between full and
 * empty.
 */
struct uint32_ringbuf {
    uint32_t data[ MAX_CMDQ_PENDING + 1 ];

    /* indices into the data[] array */
    int curr, next;

    /* number of entries in use */
    int count; 

    /* maximum number of elements in the list */
    int max;
};

struct scan_cmdq_channel {
    uint8_t channel;

    struct uint32_ringbuf pending_rows;

    bool is_open;
};

struct scan_cmdq_t {

    /* one bit is set for every open channel; when a command has been queued
     * for each channel, we pop it from the channels' pending and push into the
     * command queue which waits for hardware queue space
     */
    uint32_t all_queued_mask;
    uint32_t curr_queued_mask;

    struct uint32_ringbuf pending_rows_queue;
    struct uint32_ringbuf running_cmds;

    int num_running; ///< number of commands in SCAN_SCMD

    int expected_rows;
    int pending_rows;
    int running_rows;
    int completed_rows;
    int queued_rows;  //Number of rows in the queue no matter if data is enabled or not
                      // this will be the same as running_rows if data is enabled.

    int total_data_rows; /* Number of rows pushed into the queue with dataen set */

    /* number of times I pushed something into SCIF_SCMD */
    int commands_queued;

    /* total number of rows added through scan_cmdq_channel_add_rows() (for debugging) */
    int total_rows_added;

    int num_channels;
    struct scan_cmdq_channel channels[MAX_CMDQ_CHANNELS];

    uint32_t sclkper;  //Save the sclkper so we don't have to calculate it for every command added.
};

/* only need one instance of a scan command queue */
static struct scan_cmdq_t scan_cmdq;

static scan_cmdq_row_status_t scan_cmdq_row_status;

/* constants from the mech driver; used for pause/resume */
uint32_t scan_mech_pending_low_water;
uint32_t scan_mech_pending_high_water;

static t_ScanMechStatus cached_mech_status;

/* davep 06-Jul-2009 ; adding on-demand pause/resume */
static uint32_t g_pause_cause_flags;

/* davep 15-Jun-2012 ; add mutual exclusion sanity lock (verifies functions
 * callable from thread or ISR do not collide). 
 */
static struct safety_lock scan_cmdq_protection_lock;

#define STATE_TRANSITION(new_state)  (state_transition( (new_state), __LINE__ ))

static void scan_cmdq_lock( void )
{
    scan_err_t scerr;

    /* davep 15-Jun-2012 ; this function can be called from thread or interrupt
     * context. The lock is a sanity check, not a mutual exclusion mechanism.
     * The caller must disable the scan interrupt. Linux kernel cannot disable
     * interrupt from interrupt's own handler (deadlocks).
     */
    scerr = safety_lock_try_lock( &scan_cmdq_protection_lock );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void scan_cmdq_unlock( void )
{
    safety_lock_unlock( &scan_cmdq_protection_lock );
}

static void state_transition( cmdq_state_t new_state, int line_number )
{
    /*
     * called from interrupt context!
     */
    SCANALYZER_LOG( LOG_CMDQ_STATE_TRANS, line_number );
    SCANALYZER_LOG( LOG_CMDQ_OLD_STATE, cmdq_state );
    SCANALYZER_LOG( LOG_CMDQ_NEW_STATE, new_state );
    cmdq_state = new_state;
}

static void set_cached_mech_status(t_ScanMechStatus mech_status)
{
//    dbg2( "%s curr=%d new=%d\n", __FUNCTION__, cached_mech_status, mech_status);
    cached_mech_status = mech_status;
}

static t_ScanMechStatus get_cached_mech_status(void)
{
    return cached_mech_status;
}

/**
 * \brief
 *
 *
 * \author David Poole
 * \date 16-Nov-2005
 *
 */

static void
ringbuf_init( struct uint32_ringbuf *crb )
{
    memset( crb, 0, sizeof(struct uint32_ringbuf) );
    crb->curr = crb->next = 0;

    /* maximum number of elements is one less that our available space (need
     * the one extra slot to indicate full vs empty)
     */
    crb->max = MAX_CMDQ_PENDING;
}

static int
ringbuf_push( struct uint32_ringbuf *crb, uint32_t uint32 )
{
    int nextnext;

#ifdef CMDQ_STATE_DEBUG
//    dbg2( "%s 0x%x\n", __FUNCTION__, cmd );
#endif

    nextnext = (crb->next+1) % crb->max;
    if( nextnext == crb->curr ) {
        /* ringbuf is full */

        /* davep 07-Jul-2009 ; screw it; fail fast */
        ASSERT(0);

        return -1;
    }

    crb->data[ crb->next ] = uint32;
    crb->next = nextnext;
    crb->count++;

    return 0;
}

static int
ringbuf_pop( struct uint32_ringbuf *crb, uint32_t *cmd )
{
    *cmd = 0;

    if( crb->next==crb->curr ) {
        /* ringbuf is empty */

        /* davep 07-Jul-2009 ; screw it; fail fast */
//        ASSERT(0);

        return -1;
    }

    *cmd = crb->data[crb->curr];
    crb->curr = (crb->curr+1) % crb->max;
    crb->count--;

#ifdef CMDQ_STATE_DEBUG
//    dbg2( "%s 0x%x\n", __FUNCTION__, *cmd );
#endif

    return 0;
}

static int ringbuf_count( struct uint32_ringbuf *crb )
{
    int count;
    int curr;

    /* crb->curr is top of stack
     * crb->next is next free slot
     */
    count = 0;
    curr = crb->curr;
    while( curr != crb->next ) {
        count += 1;
        curr = (curr+1) % crb->max;
    }

    return count;
}

static void ringbuf_dump( struct uint32_ringbuf *crb )
{
    int curr;

    /* crb->curr is top of stack
     * crb->next is next free slot
     */
    curr = crb->curr;
    dbg1( "ringbuf: " );
    while( curr != crb->next ) {
        curr = (curr+1) % crb->max;
        dbg1( "0x%x ", crb->data[curr] );
    }
    dbg1( "\n" );
}

scan_err_t scan_cmdq_onetime_init( void )
{
    scan_err_t scerr;
    t_ScanMech* scan_mech;

    dbg2("%s\n", __FUNCTION__);

    /* Make sure the scan irq is turned off */
    scan_interrupt_disable();

    safety_lock_init( &scan_cmdq_protection_lock );

    /* Turn on SA logs we want to see by default */
#if 0
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_OPEN);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_CLOSE);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_LAUNCH);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_ISR);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_CMD);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_STOP);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_STATE_TRANS);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_OLD_STATE);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_NEW_STATE);
    SCANALYZER_ENABLE_LOG(LOG_CMDQ_ADD_ROWS);
#endif

    /* The mech will phone in a real status during scanplat_init_motors: assume things
     *  are broken until that point.
     */
    set_cached_mech_status(SCAN_MECH_ERROR);

    scanplat_init_mech();

    /* get a local copy of the high/low water marks so we don't have to keep
     * polling them for pause/resume 
     */
    scan_mech = scan_get_scan_mech();
    scan_mech_pending_low_water = scan_mech->pending_low_water;
    scan_mech_pending_high_water = scan_mech->pending_high_water;

    /* davep 25-Apr-2013 ; XXX temp shotgun debug; make bigger because our PIC
     * buffers are bigger
     */
//    scan_mech_pending_low_water = scan_mech->pending_low_water*4;
//    scan_mech_pending_high_water = scan_mech->pending_high_water*4;

    STATE_TRANSITION( CMDQ_STATE_IDLE );

    scan_set_cmdq_interrupt(scan_cmdq_isr);

    /* davep 11-Aug-2009 ; scancmdq and scanmech are a bit intertwined. This
     * call essentially waits for the mech to go to a ready state. Also contains
     * a message loop!
     */
    scerr = scan_cmdq_wait_for_ready();
    XASSERT( scerr==0, scerr );

    return SCANERR_NONE;
}


/**
 * \brief push a scan command into the scan block
 *
 * \param cmd register ready command
 * \param num_rows actual number of data rows that will result from this scan
 *        command does not include rows without SDATA bit set!
 *
 * We're only interested in tracking the number of >DATA< rows that will result
 * from a scan. We need to ignore any rows we scan but don't capture, e.g.,
 * rows we scan to desaturate the sensor.
 *
 * \author David Poole
 * \date 16-Nov-2005
 *
 */

static void
set_scan_cmd( uint32_t cmd )
{
    int retcode;
    uint32_t num32;
    int num_rows;

    num_rows = GET_SCANY(cmd);

    /* davep 30-Sep-2010 ; XXX temp debug */
//    dbg2( "%s cmd=%#x num_rows=%d\n", __FUNCTION__, cmd, num_rows );

    if (num_rows == 0) {
        return;
    }

    SCANALYZER_LOG(LOG_CMDQ_CMD, cmd);

    /* make sure we actually have queue space; this assert will only fail
     * if our data structures have a bug. We should always exactly match
     * the state of the hardware queue
     */
    num32 = scif_status();
    XASSERT( !(num32 & SCIF_SSTAT_QFULL), num32 );

    retcode = ringbuf_push( &scan_cmdq.running_cmds, cmd );
    XASSERT( retcode==0, retcode );

    if( cmd & SCIF_SCMD_SDATA(1) ) {

        scan_cmdq.running_rows += num_rows;
        XASSERT( num_rows==GET_SCANY(cmd), num_rows );

        scan_cmdq.pending_rows -= num_rows;
        scan_cmdq.total_data_rows += GET_SCANY(cmd);
    }

    scan_cmdq.num_running++;

    /* sanity checks */
#if __DEL_XASSERT__
    if( scan_cmdq.pending_rows>=0 ) {
        // no action
    }
    else {
        dbg1("XASSERT(%d,%d)\n", __LINE__, scan_cmdq.pending_rows);
		scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
    }
#else /* __DEL_XASSERT__ */
    XASSERT( scan_cmdq.pending_rows>=0, scan_cmdq.pending_rows );
#endif /* __DEL_XASSERT__ */

    //Keep track of number of rows queued even if data is not captured.
    scan_cmdq.queued_rows += GET_SCANY( cmd );

    SCIF_SCMD_WRITE(cmd);

    scan_cmdq.commands_queued++;
}

static void scan_cmdq_pause_complete(void)
{
    /* WARNING! May be called from interrupt context.  If not the caller should
     * make sure scan irqs are disabled before calling to protect the state change.
     */

    /* This function takes us from the CMDQ_STATE_START_PAUSE state to the
     * CMDQ_STATE_PAUSED state.  Punish the caller if the state isn't right
     * when calling.
     */
    XASSERT(cmdq_state==CMDQ_STATE_START_PAUSE, cmdq_state);

    /* Our asserts go away in release builds, so bail out now if our state isn't
     * right.
     */
    if (cmdq_state != CMDQ_STATE_START_PAUSE) {
        dbg1("%s: Bad state transition, state=%d\n", __FUNCTION__, cmdq_state);
        return;
    }

    /* Note: disabling the scif run bit will clear the command queue, so make
     *   sure we are really idle here.
     */
    XASSERT(scan_cmdq.num_running==0, scan_cmdq.num_running);
    XASSERT(scif_command_executing()==0, scif_command_executing());

    /* Stop the scan block */
    scif_control(false, false);    /* stop scan, no interrupt  */
    scif_clock(false);             /* disable scan clocks      */

    /* Finally ... set the paused state */
    STATE_TRANSITION( CMDQ_STATE_PAUSED );
    
    if ( scan_get_config() & SCAN_CONFIG_PR_TEST ) {
        scan_set_config_pipe_drain(false);
    }
}

static bool scan_cmdq_needs_to_pause(void)
{
    uint32_t row_cnt;

//    ASSERT( scan_mech );

    /* if we're doing no-stop-copy, expected_rows==0 so we'll have different
     * critera for pause/resume.
     */
    if( scan_cmdq.expected_rows ) {
        /* If pending_rows less than our low water, we should pause UNLESS we're at
         * the bottom of a scan. (At the bottom of the scan area, we would expect the
         * pending_rows to drop below the low water because there's not much left to
         * scan.)
         */

        /* sum the number of completed rows with the number of rows in flight */
        row_cnt = scan_cmdq.pending_rows + scan_cmdq.running_rows + scan_cmdq.completed_rows;

        if( (scan_cmdq.pending_rows <= scan_mech_pending_low_water) &&
            (row_cnt != scan_cmdq.expected_rows)) {

//            dbg1( "%s pending=%d low=%d running=%d completed=%d expected=%d\n",
//                        __FUNCTION__,
//                        scan_cmdq.pending_rows,
//                        scan_mech->pending_low_water,
//                        scan_cmdq.running_rows,
//                        scan_cmdq.completed_rows,
//                        scan_cmdq.expected_rows );

            return true;
        }
    }
    else {
        /* we're doing no-stopy-copy so ignore expected_rows */
        if( scan_cmdq.pending_rows <= scan_mech_pending_low_water ) {
            return true;
        }
    }

    return false;
}


/**
 * \brief  check to see if the command queue has enough rows in
 * the pending queue to restart the scan.
 *
 *
 * \author Theron Luttmann
 * \date 28-Mar-2007
 *
 *
 * \date 18-Jul-2009 davep ; add check on mech
 *      We need to avoid trying to restart the scan while the motor is still
 *      moving. We were hitting a problem were scancmdq would get enough memory
 *      to restart and attempt to restart. However, the mech was still rewinding
 *      and would subsequently assert.
 */
static bool scan_cmdq_ready_for_resume(void)
{
    SCANALYZER_LOG(LOG_CMDQ_READY_FOR_RESTART, scan_cmdq.pending_rows);
    SCANALYZER_LOG(LOG_CMDQ_READY_FOR_RESTART, scan_mech_pending_high_water);

    dbg2( "%s pend=%d hw=%d\n", __FUNCTION__, scan_cmdq.pending_rows,
                scan_mech_pending_high_water );

//    ASSERT( scan_mech );

    //Need to wait until we have some rows pending to restart.
    // Or, we have received all rows expected.
    if ( (scan_cmdq.pending_rows >= scan_mech_pending_high_water) ||
         (scan_cmdq.pending_rows + scan_cmdq.running_rows + scan_cmdq.completed_rows == scan_cmdq.expected_rows) ) {

    }
    else {
        dbg1( "[S_C] %s cmdq false\n", __FUNCTION__ );
        SCANALYZER_LOG(LOG_CMDQ_READY_FOR_RESTART, false );
        return false;
    }

    if( get_cached_mech_status() != SCAN_MECH_READY ) {
        dbg1( "[S_C] %s mech false\n", __FUNCTION__ );
        SCANALYZER_LOG(LOG_CMDQ_READY_FOR_RESTART, false );
        return false;
    }

    dbg2( "%s true\n", __FUNCTION__ );
    SCANALYZER_LOG(LOG_CMDQ_READY_FOR_RESTART, true );
    return true;
}

/**
 * \brief  open the scan command queue
 *
 * Initializes the internals of the software layer scan command queue.
 *
 * \param channels  Array of channels to open based on DMA channel numbers.
 * Will usually be PIC Write DMA channels.
 *
 * \param num_channels Number of elements in the channels[] array
 *
 * \param expected_rows  Number of rows we expect to flow through a scan run.
 * When the number of rows queued matches the expected rows, we stop pushing
 * commands into the scan block.
 *
 * \author David Poole
 * \date 16-Nov-2005
 *
 */

scan_err_t scan_cmdq_open( uint8_t channels[], int num_channels, int expected_rows )
{
    int i;
    uint8_t channel;
    uint32_t num32;
    uint32_t sclkper;

    dbg2( "%s num_channels=%d expected_rows=%d\n", __FUNCTION__, num_channels, expected_rows );

    SCANALYZER_LOG(LOG_CMDQ_OPEN, cmdq_state);

    XASSERT( cmdq_state==CMDQ_STATE_IDLE, cmdq_state );
    XASSERT( num_channels <= MAX_CMDQ_CHANNELS, num_channels );

    //need to save sclkper.
    sclkper = scan_cmdq.sclkper;

    //clearing out the struct for a new scan.
    memset( &scan_cmdq, 0, sizeof(scan_cmdq) );

    scan_cmdq.num_channels = MAX_CMDQ_CHANNELS;
    scan_cmdq.expected_rows = expected_rows;
    scan_cmdq.pending_rows = 0;
    scan_cmdq.running_rows = 0;
    scan_cmdq.queued_rows = 0;
    scan_cmdq.total_data_rows = 0;
    scan_cmdq.completed_rows = 0;
    scan_cmdq.total_rows_added = 0;
    scan_cmdq.sclkper = sclkper;  //restore sclkper

    scan_cmdq_save_row_status();

    /* Calculate how much data space we need then malloc it;
     *
     * The +1 on MAX_CMDQ_RUNNING and MAX_CMDQ_PENDING is for the ring buffer to
     * hold an extra slot for empty/not empty.
     */

    /* init pending and running queues */
    ringbuf_init( &scan_cmdq.running_cmds );
    ringbuf_init( &scan_cmdq.pending_rows_queue );

    /* allocate the "channel" buffers; commands coming into the scan cmdq will
     * first have to pass through the channel so we can sync up scanning
     * commands with actual pending buffer space (e.g., if we're scanning 3
     * colors, we'll need 3 pending commands before we actually push a command
     * into SCANCMD)
     */
    for( i=0 ; i<num_channels ; i++ ) {

        channel = channels[i];
        XASSERT( channel<MAX_CMDQ_CHANNELS, channel );

        scan_cmdq.channels[channel].channel = channel;
        scan_cmdq.channels[channel].is_open = true;

        ringbuf_init( &scan_cmdq.channels[channel].pending_rows );

        /* make sure we haven't seen this bit before */
        XASSERT( GET_BIT(scan_cmdq.all_queued_mask,channel)==0, channel );

        SET_BIT( scan_cmdq.all_queued_mask, channel );
    }

    /* make sure hardware is in a clean state */
    num32 = scif_status();
    XASSERT( num32 & SCIF_SSTAT_QEMPTY, num32 );

    /* Reset pause cause flags - nothing pending yet */
    g_pause_cause_flags = 0;

    /* Go to the ready state */
    STATE_TRANSITION( CMDQ_STATE_READY_FOR_SCAN );

    return SCANERR_NONE;
}

/**
 * \brief  Add a scan command to the software scan command queue.
 *
 * \param channel  The channel to add the command. When a matching command is
 * added to all the channels, the command is finally queued up to be pushed
 * into the scan command register itself. We wait until there is a command for
 * each channel to minimize the possibility of PIC overflow.
 *
 * \param cmd Actual register ready command
 *
 *  Note! Assumes caller disabled the scan interrupt.
 *
 * \author David Poole
 * \date 16-Nov-2005
 *
 */

void scan_cmdq_channel_add_rows( uint8_t channel, uint16_t scan_lines )
{
    int i, retcode;
    uint32_t newcmd;
    struct scan_cmdq_channel *ch;
    int total_rows;
    int new_rows;

#ifdef CMDQ_STATE_DEBUG
    dbg2( "%s ch=%d lines=%d state=%d\n", __FUNCTION__, channel, scan_lines, cmdq_state );
#endif

    scan_cmdq.total_rows_added += scan_lines;

    /* davep 21-Jul-2008 ; add for my pic overflow problems */
    SCANALYZER_LOG(LOG_CMDQ_ADD_ROWS, scan_lines);

    //Get the total number of rows for this scan so far
    total_rows = (scan_cmdq.pending_rows +
                  scan_cmdq.running_rows +
                  scan_cmdq.completed_rows);
    new_rows = scan_lines;   //rows we are trying to add

//    dbg2( "%s pending=%d running=%d completed=%d expected=%d\n",
//                __FUNCTION__, scan_cmdq.pending_rows,
//                scan_cmdq.running_rows, scan_cmdq.completed_rows,
//                scan_cmdq.expected_rows );
//    dbg2( "%s total=%d new=%d\n", __FUNCTION__, total_rows, new_rows );

    // Ok, apparently scanpipe keeps adding channels to fill the buffers
    // during the scan. It does not keep track of how many we have complete
    // already, just keeps sending them as buffers are empty. So we need to
    // make sure we do not exceed expected_rows otherwise we will overflow the
    // buffers.
    // Seems strange to do that here, but ok.  --Theron
    /* davep 15-Jun-2009 ; adding run forever */
    if( scan_cmdq.expected_rows ) {
        if( total_rows == scan_cmdq.expected_rows) {
            return;
        }
        else if ((total_rows + new_rows) > scan_cmdq.expected_rows) {

            /* clean the command, adjusting the number of rows in the cmd to match
             * what we have left to scan.
             */
            new_rows = scan_cmdq.expected_rows - total_rows;
        }
    }

    XASSERT( channel<MAX_CMDQ_CHANNELS, channel );

    XASSERT( scan_cmdq.channels[channel].is_open, channel );

//    dbg2( "%d %d %d %d %d %d 0x%x 0x%x\n",
//                scan_cmdq.channels[0].pending_rows,
//                scan_cmdq.channels[1].pending_rows,
//                scan_cmdq.channels[2].pending_rows,
//                scan_cmdq.channels[3].pending_rows,
//                scan_cmdq.channels[4].pending_rows,
//                scan_cmdq.channels[5].pending_rows,
//                scan_cmdq.curr_queued_mask,
//                scan_cmdq.all_queued_mask );
//    dbg2( "0x%x 0x%x\n", scan_cmdq.curr_queued_mask, scan_cmdq.all_queued_mask );

    SET_BIT( scan_cmdq.curr_queued_mask, channel );

    if( scan_cmdq.curr_queued_mask != scan_cmdq.all_queued_mask ) {
        /* wait for more channels to have matching commands before we push into
         * the scan block
         */

        retcode = ringbuf_push( &scan_cmdq.channels[channel].pending_rows, new_rows );

        /* push could fail if we overflow our command queue */
        XASSERT( retcode==0, retcode );

        return;
    }

    scan_cmdq.curr_queued_mask = 0;

    /* pull a command from each of the other channels */
    for( i=0 ; i<scan_cmdq.num_channels ; i++ ) {

        ch = &scan_cmdq.channels[i];

        /* only bother with opened channels */
        if( !ch->is_open ) {
            continue;
        }

        /* only pop from the other channels; i.e., don't try to pop from the
         * channel we were originally queuing a command for
         */
        if( ch->channel != channel ) {
            retcode = ringbuf_pop( &ch->pending_rows, &newcmd );

            /* since this channel's pending bit was set, we MUST have had a
             * command queued
             */
            XASSERT( retcode==0, retcode );

            /* davep 11-Mar-2008 ; XXX temp debug */
//            dbg2( "%s ch=%d cmd=%#x newcmd=%#x\n", __FUNCTION__, ch->channel, cmd, newcmd );
        }

        /* reset the pending bit for channels with more data */
        if( ch->pending_rows.count > 0 ) {
            SET_BIT( scan_cmdq.curr_queued_mask, ch->channel );
        }
    }

    /* add rows to the hardware pending queue */
    scan_cmdq.pending_rows += new_rows;
    
    retcode = ringbuf_push( &scan_cmdq.pending_rows_queue, new_rows );
    XASSERT( retcode==0, retcode );

#ifdef CMDQ_STATE_DEBUG
    dbg2( "%s new_rows=%d total=%d\n", __FUNCTION__, new_rows,
                scan_cmdq.pending_rows );
#endif

    //If we are paused, check to see if we can restart.
    if( cmdq_state==CMDQ_STATE_PAUSED && scan_cmdq_ready_for_resume()) {
		dbg1( "[S_C] %s: ready for resume (MEM)\n", __FUNCTION__ );
        scan_cmdq_resume(SCAN_CMDQ_PAUSE_CAUSE_MEMORY );
    }
}

/**
 * \brief  Trigger the software scan command queue to fill the hardware scan
 * command queue
 *
 * Checks to see if there is room in the hardware scan command queue then
 * pushes as many commands as will fit into the hardware.
 *
 * \author David Poole
 * \date 16-Nov-2005
 *
 */

void scan_cmdq_launch( void )
{
    uint32_t cmd;
    int num_rows;
    scan_err_t scerr;
    t_ScanMechStatus mech_status;
    int retcode;
    uint32_t num32;
    uint32_t unsaturate_lines;

    ASSERT( !IN_INTERRUPT() );

    dbg2( "%s cmdq_state=%d\n", __FUNCTION__, cmdq_state );

    SCANALYZER_LOG(LOG_CMDQ_LAUNCH, cmdq_state);
#if __DEL_XASSERT__
    if( (cmdq_state == CMDQ_STATE_READY_FOR_SCAN) || (cmdq_state == CMDQ_STATE_PAUSED) ) {
        // no action
    }
    else {
        dbg1("XASSERT(%d,%d)\n", __LINE__, cmdq_state);
		scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
    }
#else /* __DEL_XASSERT__ */
    XASSERT((cmdq_state == CMDQ_STATE_READY_FOR_SCAN) || (cmdq_state == CMDQ_STATE_PAUSED), cmdq_state);
#endif /* __DEL_XASSERT__ */

    /* The mech should be ready now, otherwise we left things in a bad state */
    mech_status = get_cached_mech_status();
    XASSERT( mech_status==SCAN_MECH_READY, mech_status );

    /* The scan irq should already be off here, but make sure before we start poking
     * around with the hardware.
     */
    scan_interrupt_disable();

    /* We are checking to see if we are ready to restart. This will check to
     * see if we have enough rows to start. If not, then we just go into the
     * paused state and wait for more rows added.
     */
    if (!scan_cmdq_ready_for_resume()) {
        /* burtp 23-Jul-2009; Special case for not using scan_cmdq_pause_complete here.  We
         *  know that we aren't already actively scanning.  Leave the scan irq disabled when
         *  we exit as we are still idle.
         */
        STATE_TRANSITION( CMDQ_STATE_PAUSED );
        dbg1("[S_C] scan_cmdq_launch Exit1\n");
        return;
    }

    if( cmdq_state==CMDQ_STATE_PAUSED ) {
        /* we're about to resume; tell scanimg */
        scerr = scanimg_scanning_resume();
        /* TODO if it fails, what then? */
        XASSERT( scerr==0, scerr );
    }

    STATE_TRANSITION( CMDQ_STATE_SCANNING );

//    scerr = scands_get_integer_with_default( "sensor_unsaturate_lines", &unsaturate_lines, 1 );
//    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 08-Jul-2011 ; XXX temp ; don't desat */
//    unsaturate_lines = 0;

    unsaturate_lines = 1;

    /* If we are starting up (which should always be true here), we need to clock
     * some data through (without capturing it) to desaturate the sensor.
     */
    if ((unsaturate_lines > 0) && (scan_cmdq.queued_rows == 0))
    {
        /* Format the scan command and set it in hardware: don't capture the data! */
        cmd = scif_command( false, scan_cmdq.sclkper, unsaturate_lines );
        set_scan_cmd( cmd );
        dbg2("%s desaturate lines queued=%d cmd=0x%08x\n", __FUNCTION__, unsaturate_lines, cmd);
    }

    /* davep 13-Oct-2010 ; I don't want to talk about it. >:-(  */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    pic_cwdma_late_launch();
#endif

    /* fill up the scan block's command queue until it is full */
    while( scan_cmdq.num_running < SCAN_CMD_DEPTH ) {

        retcode = ringbuf_pop( &scan_cmdq.pending_rows_queue, &num32 );
        /* davep 05-Jun-2012 ; if ringbuf is empty, we have nothing else to
         * scan (happens when we're scanning small areas that use one command)
         */
        if( retcode != 0 ) {
            break;
        }

        num_rows = (int)num32;

        if( scan_cmdq.pending_rows < num_rows ) {
            num_rows = scan_cmdq.pending_rows;
        }

        /* Break out if no more rows to scan */
        if (num_rows == 0) {
            break;
        }

        /* Format the scan command and set it in hardware: note that set_scan_cmd will
         * reduce scan_cmdq.pending_rows by num_rows (if capturing data)
         */
        cmd = scif_command(true, scan_cmdq.sclkper, num_rows );
        set_scan_cmd( cmd );
    }

#ifdef CMDQ_STATE_DEBUG
    dbg2( "%s full \n", __FUNCTION__);
#endif

    if ( scan_get_config() & SCAN_CONFIG_PR_TEST ) {
        dbg2( "%s: rodeo mode allowing the pipe to drain\n", __FUNCTION__ );
        scan_set_config_pipe_drain(true);
    }

//    /* Moving out of idle, clear any left over interrupt bits and fire up the scan irq */
//    scif_interrupt_ack(scif_status());
//    scan_set_cmdq_interrupt(scan_cmdq_isr);
    // scan_interrupt_enable();

    /* smot_scan_start_fcn() has the right to fiddle with the scan block motor
     * config register. Now we can turn on the scan block. See also Bug 10964.
     */
     /* scapri 10-apr-2009 and 17-dec-2009 To avoid race conditions,
       we have gone back to setting up the scan block first, then
       starting the motor block.  As a result, smot_scan_start_fcn()
       is no longer allowed to fiddle with the scan block motor config
       reg (ie. function scif_motor_setup()).  So, mech code is now
       only allowed to call scif_motor_setup() in the scan_setup_fcn(),
       which has happened before reaching this point in the code.  */
    scif_clock(true);
    scif_control(true, true); // run, interrupt enable
    scan_interrupt_enable();

    // Start up the motor hw
    dbg1("[S_C] scanmech_run Call\n");
    scanmech_run();

    SCANALYZER_LOG(LOG_CMDQ_LAUNCH, cmdq_state);
}

void scan_cmdq_cancel( void )
{
    ASSERT( !IN_INTERRUPT() );

    /* Shut everything down: we are finished */
    scan_interrupt_disable();
    scif_control(false, false); // stop scan, no interrupt
    scif_clock(false);          // disable scan clocks

    SCANALYZER_LOG(LOG_CMDQ_STOP, cmdq_state);

    /* Tell mech to stop -- this is a blocking call! */
    scanmech_blocking_stop();

    /*
     * The cancel routine can be called from any state.  If we are already in idle
     * then we must stay in idle, otherwise transition to stopped.
     */
    if (cmdq_state != CMDQ_STATE_IDLE) {
        STATE_TRANSITION( CMDQ_STATE_SCAN_STOPPED );
    }
}


//This is called when in error state or assert.
// NOTE this can be called by interrupt, so we just want
// to disable the hardware to avoid damage.   --Theron
void scan_cmdq_emergency_halt( void )
{
    if (scanpower_sleeping())
    {
        return;
    }
    /* Shut everything down: we are finished */
    scif_control(false, false); // stop scan, no interrupt
    scif_clock(false);          // disable scan clocks

    /* davep 15-Jun-2012 ; cannot call intDisable here. This function can be
     * called from interrupt context. In Linux kernel, disabling interrupt from
     * its own handler will deadlock.
     */

    /* Tell mech to halt (this does not block) */
    scanmech_emergency_halt();

    /*
     * WARNING: this state transistion assumes that we won't ever be coming back (called
     * as we assert for example).  Don't go abusing the emergency halt ...
     */
    STATE_TRANSITION( CMDQ_STATE_SCAN_STOPPED );
}

/**
 * \brief  Close the scan command queue.
 *
 * Should be called after every scan. Cleans up the channels, releases any
 * memory.
 *
 * \author David Poole
 * \date 16-Nov-2005
 *
 */

void scan_cmdq_close( void )
{
    int  i;

    dbg2( "%s\n", __FUNCTION__ );

    /* davep 24-Apr-2013 ; gently ignore a close if we haven't been opened
     * (handles case in scanlib2_run() where pipe_start() fails before
     * scan_cmdq_open() can be called)
     */
    if( cmdq_state==CMDQ_STATE_IDLE ) {
        return;
    }

    SCANALYZER_LOG(LOG_CMDQ_CLOSE, cmdq_state);

    /* we could be in the 'ready' state still if someone called open but never
     * called launch
     */
#if __DEL_XASSERT__
    if( cmdq_state==CMDQ_STATE_READY_FOR_SCAN || 
        cmdq_state==CMDQ_STATE_SCAN_STOPPED ) {
        // no action
    }
    else {
        dbg1("XASSERT(%d,%d)\n", __LINE__, cmdq_state);
		scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
    }
#else /* __DEL_XASSERT__ */
    XASSERT( cmdq_state==CMDQ_STATE_READY_FOR_SCAN || 
             cmdq_state==CMDQ_STATE_SCAN_STOPPED, 
             cmdq_state );
#endif /* __DEL_XASSERT__ */

    /* Shut everything down: we are finished */
    scan_interrupt_disable();
    scif_control(false, false); // stop scan, no interrupt
    scif_clock(false);          // disable scan clocks

    for( i=0 ; i<scan_cmdq.num_channels ; i++ ) {
        scan_cmdq.channels[i].is_open = false;
    }

    /* davep 30-Jul-2007 ; XXX shotgun debug ; obliterate the ringbufs (their
     * data pointers all point into scan_cmdq.qdata)
     */
    memset( &scan_cmdq.running_cmds, 0, sizeof(struct uint32_ringbuf) );
    memset( scan_cmdq.channels, 0, sizeof(scan_cmdq.channels) );

    /* Go to the idle state */
    STATE_TRANSITION( CMDQ_STATE_IDLE );
}

static scan_err_t internal_scan_cmdq_start_pause( scan_cmdq_pause_cause_t pause_cause )
{
    /*
     *  Warning!
     *
     *  Can be called from interrupt context!
     */

    /* davep 15-Jun-2012 ; this function can be called from thread or interrupt
     * context. The lock is a sanity check, not a mutual exclusion mechanism.
     * The caller must disable the scan interrupt. Linux kernel cannot disable
     * interrupt from interrupt's own handler (deadlocks).
     */
    scan_cmdq_lock();

    /* There is a timing window where cmdq can be done (we are in stopped state) but
     * the pipe hasn't processed all of its messages yet.  Asking us to pause here
     * is just rude ... talk to the hand.
     */
    if (cmdq_state == CMDQ_STATE_SCAN_STOPPED) {
        scan_cmdq_unlock();
        return SCANERR_NOT_PERMITTED;
    }
    
    g_pause_cause_flags |= (uint32_t)pause_cause;

    /* If we were already paused or starting to pause, don't try to pause again.
     * Note I still set the appropriate flag in the pause_cause set. The calling
     * code expects to need to call resume and we want to hold off the resume
     * until all pause causes are cleared.
     */
    if( cmdq_state==CMDQ_STATE_START_PAUSE || cmdq_state==CMDQ_STATE_PAUSED ) {
        scan_cmdq_unlock();
        return SCANERR_NONE;
    }

    /* If we get here we should be in the scanning state */
    XASSERT(cmdq_state==CMDQ_STATE_SCANNING, cmdq_state);

    /* The coast is clear, make the state transition */
    STATE_TRANSITION( CMDQ_STATE_START_PAUSE );

    /* If for some reason we don't have any commands running we need to stop
     * things now ... we may not go back to the isr in the CMDQ_STATE_START_PAUSE
     * state.
     */
    if( scan_cmdq.running_rows==0 ) {
        scan_cmdq_save_row_status();
        scanmech_start_pause();
        scan_cmdq_pause_complete();
    }
    
    scan_cmdq_unlock();

    return SCANERR_NONE;
}

/**
 * \brief  Wrapper around the start pause function.
 *
 * The start pause function is shared between thread and ISR. Use this wrapper
 * function from thread context. The scan cmdq isr (below) will call the
 * internal version directly.
 *
 * \author David Poole
 * \date 15-Jun-2012
 */

scan_err_t scan_cmdq_start_pause( scan_cmdq_pause_cause_t pause_cause )
{
    scan_err_t scerr;
    bool was_enabled;

    was_enabled = scan_interrupt_disable();

    scerr = internal_scan_cmdq_start_pause( pause_cause );
    
    if( was_enabled) {
        scan_interrupt_enable();
    }

    return scerr;
}

void scan_cmdq_resume( scan_cmdq_pause_cause_t pause_cause )
{
    bool was_enabled;

    SCANALYZER_LOG( LOG_CMDQ_RESUME, (uint32_t)pause_cause );

    /* should never be called from interrupt context */
    ASSERT( !IN_INTERRUPT() );

    dbg2( "%s cause=%#x flags=%#x\n",
                __FUNCTION__, pause_cause, g_pause_cause_flags );

    //We should be paused
    XASSERT( cmdq_state == CMDQ_STATE_PAUSED, cmdq_state );

    was_enabled = scan_interrupt_disable();

    /* clear this specific pause cause flag */
    g_pause_cause_flags &= ~((uint32_t)pause_cause);

    if( g_pause_cause_flags!=0 ) {
        /* can't resume until all our pause causes are cleared */
        if( was_enabled ) {
            scan_interrupt_enable();
        }
#if 1	// add.lsh to debug copy stop
        if(pause_cause == SCAN_CMDQ_PAUSE_CAUSE_MEMORY)
        	dbg1( "[S_C] resume reject due to PIPE(Mem) : %d, (mem:BIT0,pipe:BIT1) !!\n", g_pause_cause_flags);
        else
        	dbg1( "[S_C] resume reject due to MEM(CQ) : %d, (mem:BIT0,pipe:BIT1) !!\n", g_pause_cause_flags);
#endif
        return;
    }
#if 1	// add.lsh to debug copy stop
    else{
    	if(pause_cause == SCAN_CMDQ_PAUSE_CAUSE_MEMORY)
        	dbg1( "[S_C] resume accept by MEM(CQ) : %d, (mem:BIT0,pipe:BIT1) !!\n", g_pause_cause_flags);
        else
        	dbg1( "[S_C] resume accept by PIPE(Mem) : %d, (mem:BIT0,pipe:BIT1) !!\n", g_pause_cause_flags);
    }
#endif

    if( was_enabled ) {
        scan_interrupt_enable();
    }

    //restart the scan
    scan_cmdq_launch();
}

/**
 * \brief Pop last command from queue, push another command into queue,
 * adjust counters.
 *
 * Called from scan_interrupt(). This is an ISR. Beware!
 *
 *
 * \author David Poole
 * \date 16-Nov-2005
 *
 */

int scan_cmdq_isr( void )
{
    int retcode;
    int num_rows;
    uint32_t cmd;
    int return_value = SCAN_CMDQ_SUCCESS;
    uint32_t num32;
    
    /* BIG FAT NOTE: THIS IS AN INTERRUPT HANDLER!
     *
     * Currently it is called from scan_interrupt()
     */

#ifdef CMDQ_STATE_ISR_DEBUG
    dbg2( "%s\n", __FUNCTION__ );
#endif
	
    SCANALYZER_LOG(LOG_CMDQ_ISR, ringbuf_count(&scan_cmdq.running_cmds) );

    /* Enforce our state machine: isr should only fire while actively scanning */
#if __DEL_XASSERT__
    if( (cmdq_state == CMDQ_STATE_START_PAUSE) || (cmdq_state == CMDQ_STATE_SCANNING) ) {
        // no action
    }
    else {
        dbg1("XASSERT(%d,%d)\n", __LINE__, cmdq_state);
		scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
        return return_value;
    }
#else /* __DEL_XASSERT__ */
    XASSERT((cmdq_state == CMDQ_STATE_START_PAUSE) || (cmdq_state == CMDQ_STATE_SCANNING), cmdq_state);
#endif /* __DEL_XASSERT__ */

    /* pop the current command of which there better be one if we were called */
    XASSERT( scan_cmdq.num_running > 0, scan_cmdq.num_running );

    retcode = ringbuf_pop( &scan_cmdq.running_cmds, &cmd );
    XASSERT( retcode==0, retcode );
    scan_cmdq.num_running--;
    num_rows = GET_SCANY(cmd);
    scan_cmdq.queued_rows -= num_rows;

    /* change our counters but only if this last command actually captured data */
    if( cmd & SCIF_SCMD_SDATA(1) ) {
        scan_cmdq.running_rows -= num_rows;
        scan_cmdq.completed_rows += num_rows;
        
        /* stupid human checks */
#if __DEL_XASSERT__
        if( scan_cmdq.running_rows >= 0 ) {
            // no action
        }
        else {
            dbg1("XASSERT(%d,%d)\n", __LINE__, scan_cmdq.running_rows);
			scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
        }
#else /* __DEL_XASSERT__ */
        XASSERT( scan_cmdq.running_rows >= 0, scan_cmdq.running_rows );
#endif /* __DEL_XASSERT__ */

        /* davep 15-Jun-2009 ; we run forever if expected_rows==0 */
        if( scan_cmdq.expected_rows ) {
#if __DEL_XASSERT__
            if( scan_cmdq.completed_rows <= scan_cmdq.expected_rows ) {
                // no action
            }
            else {
                dbg1("XASSERT(%d,%d)\n", __LINE__, scan_cmdq.completed_rows);
				scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
                return return_value;
            }
#else /* __DEL_XASSERT__ */
            XASSERT( scan_cmdq.completed_rows <= scan_cmdq.expected_rows,
                     scan_cmdq.completed_rows );
#endif /* __DEL_XASSERT__ */
        }
    }

#ifdef CMDQ_STATE_ISR_DEBUG
    scan_cmdq_dump();
#endif

    /* if we've received as many rows as we were told to expect, we're done so
     * don't try to push in another command
     *
     * (expected_rows==0 if we're doing no-stopy-copy)
     */
    
    if( scan_cmdq.expected_rows && scan_cmdq.expected_rows<=scan_cmdq.completed_rows ) {
        STATE_TRANSITION( CMDQ_STATE_SCAN_STOPPED );

        if ( scan_get_config() & SCAN_CONFIG_PR_TEST ) {
            scan_set_config_pipe_drain(false);
        }

        /* Ask the motor/mech driver to stop the motor. This is a non-blocking
         * call so the motor could still be moving when we return. Can't call
         * the blocking motor stop function from interrupt context.  We'll call
         * scan_cmdq_wait_for_ready() to poll on the motor motion completely
         * stopping.
         */
        scanmech_async_stop();

        /* we're done! whee! */
        return SCAN_CMDQ_DONE;
    }

    switch (cmdq_state) {
        case CMDQ_STATE_START_PAUSE:
            /* davep 16-Jul-2009 ; XXX experimental ; if we're trying to start a
             * pause, don't feed any more commands into the command queue
             *
             * pause the mech when we have no more commands running (don't want
             * to smear which can happen when we scan while moving)
             */
            if( scan_cmdq.running_rows==0 ) {
                scan_cmdq_save_row_status();
                scanmech_start_pause();
                scan_cmdq_pause_complete();
            }
            return SCAN_CMDQ_SUCCESS;
            break;

        case CMDQ_STATE_SCANNING:
            /* If we're below low water and we've got no more pending rows, it's time
             * to start the pause and let the command queue drain.
             */
            if( scan_cmdq_needs_to_pause() && (scan_cmdq.pending_rows==0)) {

                /* davep 15-Jun-2012 ; calling from ISR so call the
                 * non-interrupt protected version 
                 */
                internal_scan_cmdq_start_pause( SCAN_CMDQ_PAUSE_CAUSE_MEMORY );
                /* burtp 23-Jul-2009; Don't feed any more commands into the command queue */
                return SCAN_CMDQ_SUCCESS;
            }
            break;

        default:
            /* We should not be here in the other states */
#if __DEL_XASSERT__
            dbg1("XASSERT(%d,%d)\n", __LINE__, cmdq_state);
			scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
#else /* __DEL_XASSERT__ */
            XASSERT(0, cmdq_state);
#endif /* __DEL_XASSERT__ */
            break;
    }

    /* burtp 04-Aug-2009; Note that we can get here with pending rows equal to
     *  zero at the end of a scan (no more pending, but rows still running).
     */
    retcode = ringbuf_pop( &scan_cmdq.pending_rows_queue, &num32 );
    if( retcode==0 ) {
        num_rows = (int)num32;
    }
    else {
        /* pending was empty; no further rows to scan */
        num_rows = 0;
    }
//    dbg2( "%s num_rows=%d\n", __FUNCTION__, num_rows );

    if( scan_cmdq.pending_rows < num_rows ) {
        num_rows = scan_cmdq.pending_rows;
    }

    /* Format the scan command and set it in hardware: note that set_scan_cmd will
     * reduce scan_cmdq.pending_rows by num_rows (if capturing data).  Only create
     * and set the command if we have some rows to work on.
     */
    if (num_rows > 0) { 
        cmd = scif_command( true, scan_cmdq.sclkper, num_rows);
        set_scan_cmd( cmd );
    }
    
//    SCANALYZER_LOG(LOG_CMDQ_ISR, 0xffff0000 | scan_cmdq.pending_rows );

    return return_value;
}

/**
 * \brief
 *
 * \author Theron Luttmann
 * \date 04-Oct-2007
 *
 */

scan_err_t scan_cmdq_setup( void )
{
    uint32_t scanx;
    uint32_t scan_line_time, smech_sclkper, smech_line_time;
    scan_cmode_t cmode;
    uint32_t dpi;

    //Calc sclkper for the commands. This value is used only for internal
    // start pulse generation.
    scanimg_get_scanx(&scanx);
    scanimg_get_cmode(&cmode);
    scanimg_get_dpi(&dpi);

    scan_cmdq.sclkper = scif_calc_sclkper(cmode, dpi, scanx);

    /* make slightly bigger to give our lines some breathing room */
    scan_cmdq.sclkper += scan_cmdq.sclkper/10;

    dbg2( "%s scanx=%d sclkper=%d\n", __FUNCTION__, scanx, scan_cmdq.sclkper );

    scanmech_setup();

    scan_line_time = scanimg_calc_line_time_from_sclkper( scan_cmdq.sclkper );
    dbg2( "%s scan_line_time=%d\n", __FUNCTION__, scan_line_time );

    /* davep 22-Jun-2009 ; get the line time from the mech; calculate another
     * sclkper; take the max of the two
     *
     * Zero means the mech driver doesn't have a preference on scan line time
     */
    smech_get_min_line_time( scan_line_time, &smech_line_time );
    if( smech_line_time > 0 ) {
        smech_sclkper = scanimg_calc_sclkper_from_line_time( smech_line_time );

        dbg1( "%s scan_sclkper=%d mech_sclkper=%d\n", __FUNCTION__,
                    scan_cmdq.sclkper, smech_sclkper );
        dbg1( "%s scan_line=%d mech_line=%d in usec\n", __FUNCTION__,
                    scan_line_time, smech_line_time );

        scan_cmdq.sclkper = MAX( smech_sclkper, scan_cmdq.sclkper );
    }

    /* davep 22-Jun-2009 ; XXX do I have to kludge in the sclkper scale when I
     * change the sclkper based on the scanmech min line time? Probably.
     */

    return SCANERR_NONE;
}

scan_err_t scan_cmdq_wait_for_ready(void)
{
    scan_err_t scerr, final_scerr;
    scan_msg_t msg;
    struct scos_mq * msgq;
    bool continue_to_wait;
    t_ScanMechStatus cached_mech_status;

    ASSERT( !IN_INTERRUPT() );

    dbg2( "%s status=%d\n", __FUNCTION__, get_cached_mech_status() );

    msgq = scantask_get_msgq();

    continue_to_wait = true;

    final_scerr = SCANERR_NONE;

    /* davep 19-Aug-2009 ; Bug 13825 ; always pump the message loop so we can
     * catch status transitions rather than leaving them in the message loop.
     */

    while( continue_to_wait ) {

        scerr = scos_mq_receive( msgq, &msg, USEC_PER_SECOND/10 );

        if( scerr==SCANERR_TIMEDOUT ) {
            cached_mech_status = get_cached_mech_status();

            if( cached_mech_status==SCAN_MECH_READY ) {
                continue_to_wait = false;
            }
            else {
                dbg2( "%s no messages; status=%d wait some more\n",
                            __FUNCTION__, cached_mech_status );
            }
            continue;
        }
        else { 
            XASSERT( scerr==0, scerr );
        }

        switch ( msg.msgType ) {
            case SMSG_SCAN_MECH_STATUS:
                /* We got a status update from the mech, cache the new value */
                set_cached_mech_status((t_ScanMechStatus)msg.param1);

                if (get_cached_mech_status() == SCAN_MECH_READY) {
                    continue_to_wait = false;
                }
                break;

            case SMSG_SCAN_SCMDEND:
                //If we cancel we can see this message here. Just ignore it for now
                // --Theron
                break;

            case SMSG_SCAN_CANCEL:
                //User canceled the scan set return value.
                // we will return a cancel code but need to wait
                // for the motors to stop.
                final_scerr = SCANERR_SCAN_CANCELLED;

                //cancel the scan -- this is a blocking call!
                scan_cmdq_cancel();
                break;

            case SMSG_SMECH_POLL :
                /* We should shut off the polling before we run a scan but gently
                 * ignore this message anyway. No sense risking a race condition.
                 */
                break;

            case SMSG_SCAN_MECH_FAULT :
                /* davep 29-Apr-2009 ; adding mech fault handling */
                scerr = smech_fault_msg( &msg );
                if( scerr != 0 ) {
                    dbg1( "%s mech fault scerr=%d so breaking out of wait for ready message loop\n",
                                __FUNCTION__, scerr );
                    continue_to_wait = false;
                    final_scerr = scerr;
                }
                break;

            case SMSG_SCAN_MECH_IOCTL :
                /* davep 24-May-2012 ; adding ioctl handler so mech driver can
                 * communicate with itself.  We have no idea what's in this
                 * message or what it mean. (It's a message from the mech
                 * driver to the mech driver.) Pass directly down to the mech
                 * driver.
                 */
                scerr = smech_ioctl( msg.param1, msg.param2, msg.param3 );
                break;

            case SMSG_SCAN_ADF_PAPER_EVENT :
                /* scancmdq doesn't care what happens with adf paper */
                dbg2( "%s ignore adf paper event=%d value=%d\n", 
                            __FUNCTION__, (uint32_t)msg.param1, (uint32_t)msg.param2 );
                break;

            case SMSG_AFE_HW_FAILURE : 
                /* davep 28-Jun-2012 ; catch AFE hardware failure; can happen in
                 * interrupt context so ISR will send the thread this message
                 */
                dbg1( "%s afe failure param1=0x%x\n", __FUNCTION__, msg.param1 );

                /* cancel the scan -- this is a blocking call! We will stay in
                 * this message loop waiting for mech ready
                 */
                scan_cmdq_cancel();

                final_scerr = SCANERR_HARDWARE_FAIL;
                break;

            default :
                //Assert on unexpected messages
                dbg1("%s unknown message type=%#x param1=0x%x param2=0x%x param3=%p\n",
                            __FUNCTION__, msg.msgType, msg.param1, msg.param2, msg.param3 );
                XASSERT(0,msg.msgType );
        }

    }

    return final_scerr;
}

// Called when we get a status change message from the mech.
void scan_cmdq_mech_status_change(t_ScanMechStatus mech_status)
{
    scan_err_t scerr;

    ASSERT( !IN_INTERRUPT() );

    dbg2( "%s cmdq_state=%d mech_status=%d\n", __FUNCTION__, cmdq_state, mech_status );

    /* Update our cached mech state: the command queue will always use the cached value
     * to force mech updates to come through a message queue.
     */
    set_cached_mech_status(mech_status);

    /* davep 19-Aug-2009 ; <insert big long description of edge triggers vs state
     * polling>  See bug 13825.
     */
    if( cmdq_state==CMDQ_STATE_IDLE ) {
        /* ignore ; this is a late status change, probably a message from a
         * blocking move
         */
        //dbg2( "%s late status change mech_status=%d\n", 
        dbg1( "[C_S] late status change mech_status=%d\n", // mod.lsh to debug copy stop
                    __FUNCTION__, mech_status );

        /* do NOT do any further processing on this state change! We update our
         * local status but nothing else.
         */
        return;
    }

    /* burtp 23-Jul-2009; This message can be delayed by so much it is almost
     * worthless for getting into the paused state.  But it might help us resume.
     */
    if( cmdq_state==CMDQ_STATE_PAUSED && mech_status==SCAN_MECH_READY) {

        /* davep 11-Oct-2010 ; now that we are 100% paused, inform scanimg of
         * our pause 
         */
        scerr = scanimg_scanning_pause();
        /* TODO if it fails, what then? */
        XASSERT( scerr==0, scerr );

        if (scan_cmdq_ready_for_resume()) {
            dbg1( "[S_C] %s: ready for resume (MEM)\n", __FUNCTION__ );
            scan_cmdq_resume( SCAN_CMDQ_PAUSE_CAUSE_MEMORY );
        }
    }
}

void scan_cmdq_dump(void)
{
    int i;
    struct scan_cmdq_channel *ch;

    dbg1( "scancmdq num_running=%d expected_rows=%d pending_rows=%d running_rows=%d completed_rows=%d queued_rows=%d total_data_rows=%d cmdqueued=%d totalrows=%d state=%d\n",
         scan_cmdq.num_running,
         scan_cmdq.expected_rows,
         scan_cmdq.pending_rows,
         scan_cmdq.running_rows,
         scan_cmdq.completed_rows,
         scan_cmdq.queued_rows,
         scan_cmdq.total_data_rows,
         scan_cmdq.commands_queued,
         scan_cmdq.total_rows_added,
         cmdq_state);
    dbg1( "scancmdq curr_queued=0x%x all_queued=0x%x\n",
            scan_cmdq.curr_queued_mask, 
            scan_cmdq.all_queued_mask );

    dbg1( "scancmdq pending max=%d count=%d\n",
            scan_cmdq.pending_rows_queue.max,
            scan_cmdq.pending_rows_queue.count
        );
    ringbuf_dump( &scan_cmdq.pending_rows_queue );

    for( i= 0 ; i<MAX_CMDQ_CHANNELS ; i++ ) {
        ch = &scan_cmdq.channels[i];
        if( !ch->is_open ) { 
            continue;
        }
        dbg1( "scancmdq ch=%d ringbuf max=%d count=%d\n", ch->channel,
                ch->pending_rows.max, ch->pending_rows.count );
        ringbuf_dump( &ch->pending_rows );
    }


    /* davep 12-May-2010 ; quiet a warning; currently ringbuf_count() only used
     * in a SCANALYZER_LOG() call and scanalyzer is compile-time optional
     */
    if( 0 ) { ringbuf_count(&scan_cmdq.running_cmds); }
}

void scan_cmdq_run_forever( void )
{
    /* davep 15-Jun-2009 ; adding run forever feature; if expected_rows==0, we
     * run forever
     */
    dbg2( "%s\n", __FUNCTION__ );

    scan_cmdq.expected_rows = 0;
}

/*
 * ballen 12/2011 We expose this so that the mech code
 * can do fancy math on the correct line time, after adding
 * the 10% buffer, mech line time, etc.
 */
uint32_t scan_cmdq_get_sclkper(void)
{
    return scan_cmdq.sclkper;
}

/**
 * \brief Save cmdq row status.
 *
 * Originally created to assist mech drivers in pause/resume.
 *
 * \author David Poole, Harris Huang
 * \date 16-Nov-2012
 */
void scan_cmdq_save_row_status(void)
{
    scan_cmdq_row_status.rs_expected_rows = scan_cmdq.expected_rows;
    scan_cmdq_row_status.rs_pending_rows = scan_cmdq.pending_rows;
    scan_cmdq_row_status.rs_running_rows= scan_cmdq.running_rows;
    scan_cmdq_row_status.rs_completed_rows = scan_cmdq.completed_rows;
    scan_cmdq_row_status.rs_queued_rows= scan_cmdq.queued_rows;
}


/**
 * \brief Get cmdq row status.
 *
 * Originally created to assist mech drivers in pause/resume.
 *
 * \author David Poole, Harris Huang
 * \date 16-Nov-2012
 */
scan_cmdq_row_status_t scan_cmdq_get_row_status()
{
    return scan_cmdq_row_status;
}

