/****************************************************************************** 
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 * davep 28-Feb-2008 ;  Greetings! And welcome to pictest.c!
 *
 * Most of the "validate" functions in pictest.c don't actually validate.
 * During the original incarnation of pictest.c, we didn't have enough
 * knowledge of how PIC worked to actually validate it. If the data ran through
 * an enabled block, that was a victory in and of itself.
 *
 * I want to change that. I want real validation to occur in here.  Someday.
 *
 * The original validate functions were responsible for putting the data
 * back onto the ready list. As we added more and more test functions, we
 * eventually would just copy/paste a function that would hexdump the data
 * and release the icebuf. We'd visually inspect the data and say, "Well, I
 * think that's correct". 
 *
 * See also comments in dump_and_release()
 *
 * So, as of this writing, I'm going to skip the validation functions and just
 * call dump_and_release().  Might as well be honest about not really validating
 * the data until we really *do* validate the results.
 *
 *****************************************************************************/

#include "scos.h"

#include "lassert.h"
#include "debug.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "list.h"
#include "hwconfig_api.h"
#include "regAddrs.h"
#include "posix_ostools.h"

/* I'm testing PIC DMA so I need to see some of the internal parts for sanity
 * checks, etc.
 */
#define PICDMA_FRIEND

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scantools.h"
#include "scanhw.h"
#include "scan.h"
#include "pichw.h"
#include "safetylock.h"
#include "icedma.h"
#include "pic.h"
#include "picreset.h"
#include "piccbi.h"
#include "cal.h"
#include "pictest.h"
#include "pictest_pc.h"
#include "scantask.h"
#include "ostools.h"
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
#include "picdma_descrip.h"
#include "picdma_descrip_test.h"
#endif
#include "cisx.h"
#include "icetest.h"
#include "icetesttest.h"
#include "scanalyzer.h"

#define PICTEST_TASK_PRI     THR_PRI_NORMAL
#define THREAD_STACK_SIZE       8192

#if !defined __KERNEL__
static uint8_t pictest_stack[THREAD_STACK_SIZE] __attribute__ ((aligned (8)));
#endif
static pthread_t pictest_task_id; 

#define RGB_FEEDER_FIRST_PIXEL 0x1234
#define RGB_FEEDER_LAST_PIXEL  0x5678
 
//#define PIC_LRMR_RIGHT_MARGIN_DISABLED 0 
#define PIC_LRMR_RIGHT_MARGIN_DISABLED 65535
#define PIC_LRMR_LEFT_MARGIN_DISABLED 0

#define PICTEST_FEEDER_SEM_NAME  "/pictest"
static struct scos_sem feeder_thread_sem;

static uint8_t icetest_channels[ ICETEST_IDMA_NUM_CHANNELS ];
static uint8_t pic_channels[ PIC_WDMA_NUM_CHANNELS ];
static int num_icetest_channels;
static int num_pic_channels;

void pictest_feeder_sem_init( void )
{
    scan_err_t scerr;

    scerr = scos_sem_is_initialized( &feeder_thread_sem ); 
    if( scerr != SCANERR_NONE ) {
        scerr = scos_sem_init( &feeder_thread_sem, "/pictest", 1 );
        XASSERT( scerr==SCANERR_NONE, scerr );
    }
}

void pictest_feeder_lock( void )
{
    scan_err_t scerr;

    /* semaphore must NOT already be locked */
    scerr = scos_sem_trywait( &feeder_thread_sem );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

void pictest_feeder_wait_for_release( void )
{
    scan_err_t scerr;

    scerr = scos_sem_wait( &feeder_thread_sem );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

void pictest_feeder_release( void )
{
    scan_err_t scerr;

    scerr = scos_sem_post( &feeder_thread_sem );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 06-Mar-2008
 *
 */

void pictest_make_thread( thread_return_t (*entry_function)(void *), void *msg )
{
#ifdef HAVE_SQUASHED_ANT
    /* davep 12-Feb-2013 ; don't need separate thread anymore; use icetest 
     * (ANT register obsoleted)
     */
    ASSERT(0);
#endif

    /* feeder threads must sleep on this semaphore until we're ready to let them
     * run (originally I was able to create ThreadX threads suspended but don't
     * think I can do that in POSIX threads)
     */
    pictest_feeder_lock();

#ifdef __KERNEL__
    pictest_task_id =  kthread_run( entry_function, msg, "pictest" );
#else
    uint32_t pxretcode;
    pxretcode = posix_create_thread( &pictest_task_id, entry_function, msg, "pictest",
                                    pictest_stack, THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL );
    XASSERT(pxretcode==0, pxretcode);
#endif
}

pthread_t pictest_get_task_id( void )
{
    return pictest_task_id;
}

void pictest_wait_for_feeder_complete( void )
{
#ifdef __KERNEL__
    /* TODO */
#else
    pthread_join( pictest_task_id, NULL );
#endif
}

//--------------------------------------------------------------------------
// Function     : get_icebuf_ptr
//   returns    : pointer to ice_dma_buffer from list 
//                NULL if list is empty
//   list       : ptr to linked list
// Created by   : David Poole
// Date created : 8-Apr-05
// Description  : 
//  
//  Pop a struct ice_dma_buffer pointer from a linked list. Verify the
//  structure to within an inch of its life. Return pointer to the ICE DMA
//  buffer.
//
//  Assumes the ice_dma_buffer has a valid data pointer attached (not NULL).
// 
// Notes        : 
//
//--------------------------------------------------------------------------

static struct ice_dma_buffer * 
get_icebuf_ptr( ATLISTENTRY *list )
{
    ATLISTENTRY *entry;
    struct ice_dma_buffer *icebuf;

    /* stupid human check */
    XASSERT( list!=NULL,(uint32_t)list );
    XASSERT( IS_PTR_VALID(list), (uint32_t)list );

    icebuf = NULL;

    /* get the first buffer and look for our starting signature */
    entry = ATRemoveHeadList( list );

    if( entry == NULL ) {
        /* empty list */
        return icebuf;
    }

    /* return a pointer to the data */
    XASSERT( IS_PTR_VALID(entry), (uint32_t)entry );
    icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );
    XASSERT( IS_PTR_VALID(icebuf), (uint32_t)icebuf );
    XASSERT( icebuf->cookie==ICE_DMA_COOKIE, icebuf->cookie );

    /* note this assumes data will never be NULL */
    XASSERT( IS_DMAPTR_VALID(icebuf->data), (uint32_t)icebuf->data );

    return icebuf;
}

/**
 * \brief hexdump part of the icebuf in a list then release it back to PIC 
 *
 * Rolled some copy/paste code into this function.  This is a simple function
 * that will hexdump a small part of the result of a test then send the buffer
 * back to the PIC ready list.
 *
 * The tests assume all buffers are back on the ready list before the test will
 * successfully finish; it's a check for memory leaks.
 *
 * \author David Poole
 * \date 28-Feb-2008
 *
 */

static void 
dump_and_release( int num_colors, int total_rows, 
                  int pixels_per_row, ATLISTENTRY *results_buffers )
{
    struct ice_dma_buffer *icebuf;
    int i;

    dbg2("%s start\n", __FUNCTION__ );

    /* Want to dump the first few bytes of the first buffers than come out of
     * each channel. 
     *
     * assuming colors are in the list as R,G,B,R,G,B,R,G,B (if scanning color) 
     */
    for( i=0 ; i<num_colors ; i++ ) {
        icebuf = get_icebuf_ptr( results_buffers );
        if( icebuf== NULL ) {
            /* end of data, leave while(1) loop */
            break;
        }
        /* dump the first buffer of each color so we can see a bit of the results */
        dbg2( "Color %d:\n", i );
        if( i<num_colors ) {
            scanlog_hex_dump( icebuf->data, 
                    MIN( 16, icebuf->bytes_per_row * icebuf->num_rows ) ); 
        }
        /* put it back on the ready list */
        pic_wdma_add_ready( icebuf->channel, &icebuf );
    }
    
    /* release anything else out there */
    while( 1 ) {
        icebuf = get_icebuf_ptr( results_buffers );
        if( icebuf== NULL ) {
            /* end of data, leave while(1) loop */
            break;
        }
        pic_wdma_add_ready( icebuf->channel, &icebuf );
    }

    dbg2("%s end\n", __FUNCTION__ );
}

//--------------------------------------------------------------------------
// Function     : pictest_rgb_feeder
//   returns    : none
//   input      : scan_msg_t pointer
// Created by   : David Poole
// Date created : 3-Apr-05
// Description  : 
//
//  Feeds 16-bits per pixel, color as three planes (0,1,2) or mono (one plane)
//  data into PIC ADC test register.
//
//  Message->param1 == number of rows to send
//  Message->param2 == pixels per row
//  Message->param3 == boolean: true if mono, false if color
//
//  Total bytes sent will be rows * pixels_per_row * 2 
//
// 
// Notes        : 
//
// davep 07-Nov-2007 ; added capability to send Mono
//
//--------------------------------------------------------------------------

thread_return_t pictest_rgb_feeder( void *input )
{
    scan_msg_t *msg;
    int pcnt, ccnt, rcnt;
    int color[3];
    int rows, pixels_per_row, wordcount;
    int total_bytes;
    int sanity;
    int num_colors;
    bool verbose;
    scan_cmode_t cmode;

    dbg1("%s start\n", __FUNCTION__ );

    /* davep 12-Feb-2013 ; this platform doesn't support ANT register */
#ifdef HAVE_SQUASHED_ANT
    ASSERT(0);
#endif

    /* wait until my parent thread is ready to let me run */
    pictest_feeder_wait_for_release();

    /* hardwire for now; eventually (somehow) make a parameter */
//    verbose = true;
    verbose = false;

    /* Input should be a pointer to a message structure containing the number
     * of rows to feed into the ADC test register and the number of pixels in
     * each row.
     *
     * ADC wants words so we need to count words.
     */ 
    msg = (scan_msg_t *)input;

    rows = msg->param1;
    pixels_per_row = msg->param2;
    cmode = (scan_cmode_t)msg->param3;
    XASSERT( cmode==SCAN_CMODE_MONO || cmode==SCAN_CMODE_COLOR, cmode );

    /* hardwire ourselves to 2-byte per pixels */
    wordcount = pixels_per_row;

    dbg1("sending %s %d rows * %d pixels_per_row = %d words\n", 
                cmode==SCAN_CMODE_MONO?"mono":"color", 
                rows, pixels_per_row, rows*wordcount );
    /* "*2" for 2 bytes per pixel, "*3" for 3 color planes */
    if( cmode==SCAN_CMODE_MONO ) {
        total_bytes = rows * wordcount * 2;
    }
    else {
        total_bytes = rows * wordcount * 2 * 3;
    }
    dbg1("total bytes=%d\n", total_bytes );

    /* want to send in 16-bit per pixel mono/RGB data */
    if( cmode==SCAN_CMODE_MONO ) {
        color[0] = PIC_CBI_MEVEN;
        num_colors = 1;
    }
    else {
        color[0] = PIC_CBI_CEVEN_0;
        color[1] = PIC_CBI_CEVEN_1;
        color[2] = PIC_CBI_CEVEN_2;
        num_colors = 3;
    }

    sanity = 0;

    /* rcnt = row count 
     * ccnt = color count
     * pcnt = pixel count
     */
    for( rcnt=0 ; rcnt<rows ; rcnt++ ) {

        /* send in our color planes a line at a time like CIS does */
        for( ccnt=0 ; ccnt<num_colors ; ccnt++ ) {

            /* send in a starting pixel; PIC needs to know when it's receiving
             * the last line of data thus the _FIRPLL vs _FIRP 
             */
            if( rcnt == rows-1) {
                /* davep 01-Apr-2010 ; don't use FIRPLL, causes problems with
                 * descriptor DMA. 
                 */
                /* first pixel in a normal line */
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRP, color[ccnt], 0x1234 );
                /* first pixel in last line */
//                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRPLL, color[ccnt], 0x1234 );
            }
            else {
                /* first pixel in non-last line */
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRP, color[ccnt],
                                    RGB_FEEDER_FIRST_PIXEL );
            }
            if( verbose ) {
                dbg2( "%s row=%d color=%d\n", __FUNCTION__, rcnt, ccnt );
            }
            sanity++;

            /* now send in a block of data;
             * -2 for the starting and ending pixels 
             */
            for( pcnt=0 ; pcnt<wordcount-2 ; pcnt++ ) {
                /* davep 05-Dec-2007 ; make bigger to test BDR */
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_NORMP, color[ccnt], pcnt<<8 );
//                pic_adcnorm_ant( PIC_ADCN_TEST_DT_NORMP, color[ccnt], pcnt );

                if( verbose ) {
                    dbg2( "%s row=%d color=%d pixel=%#x\n", __FUNCTION__, rcnt,
                                ccnt, pcnt );
                }
                sanity++;
            }

            /* send in a last pixel */
            pic_adcnorm_ant( PIC_ADCN_TEST_DT_LASTP, color[ccnt],
                             RGB_FEEDER_LAST_PIXEL );

            if( verbose ) {
                dbg2( "%s row=%d color=%d pixel=%#x\n", __FUNCTION__, rcnt,
                            ccnt, RGB_FEEDER_LAST_PIXEL );
            }

            sanity++;
        }
    }

    XASSERT( sanity*2 == total_bytes, sanity );

    dbg1("%s done sending %d words of data\n", __FUNCTION__, sanity );
    dbg1("%s end\n", __FUNCTION__ );

    /* release the lock I'm holding */
    pictest_feeder_release();

    return (thread_return_t)0;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void 
pictest_run_it( pthread_t thread, ATLISTENTRY *results_buffers, 
        uint8_t wait_channels[], uint8_t num_channels, int expected_rows )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    scan_msg_t msg;
    struct scos_mq *msgq;
    int received_rows;
    uint32_t wait_done;
    uint32_t current_done;
    int i;
    uint32_t qcnt;

    dbg2("%s start\n", __FUNCTION__ );

    /* build a bitmask we can use to do a simple compare to watch for all the
     * channels finishing
     */
    wait_done = 0;
    current_done = 0;
    for( i=0 ; i<num_channels ; i++ ) {
        XASSERT( wait_channels[i] < PIC_WDMA_NUM_CHANNELS, wait_channels[i] );
        SET_BIT( wait_done, wait_channels[i] );
    }

    msgq = scantask_get_msgq();

    received_rows = 0;

    /* resume the feeder thread now that we're ready for it */
    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
    pictest_feeder_release();
    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );

    /* spin waiting for PIC DMA to complete */
    while( wait_done != current_done ) {
        /* infinite loop unless our PIC DMA completes */
        dbg2( "waiting up to 5 seconds for messages...\n" );
        
        scerr = scos_mq_receive( msgq, &msg, USEC_PER_SECOND*5 );

        if( scerr == SCANERR_TIMEDOUT ) {
            /* dump out some stats so we know where we are */
            dbg1("No messages; wait some more.\n" );
            pic_wdma_debug_log();
            for( i=0 ; i<num_channels ; i++ ) {
                pic_wdma_channel_dump( wait_channels[i] );
            }
            dbg1("expected_rows=%d received_rows=%d\n", 
                        expected_rows*num_channels, received_rows );

//            dbg1("interrupt count=%d\n", pic_interrupt_stats.count );
            continue;
        }

        switch( msg.msgType ) {

            case SMSG_PIC_WDMA_DATA :
                /* PIC DMA complete interrupt */

                icebuf = msg.param3;

                XASSERT( icebuf->channel < PIC_WDMA_NUM_CHANNELS, icebuf->channel );
                icebuf_sanity( icebuf );

                /* is this the last buffer in this channel? */
                if( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                    dbg2( "channel %d is done\n", icebuf->channel );
                    SET_BIT( current_done, icebuf->channel );
                }

                received_rows += icebuf->num_rows;

                dbg2("dma msg=%p channel=%d num_rows=%d max_rows=%d bpr=%d data=%p total=%d\n", 
                            icebuf, icebuf->channel, icebuf->num_rows,
                            icebuf->max_rows, icebuf->bytes_per_row, 
                            icebuf->data, received_rows );

                ATInsertTailList( results_buffers, &icebuf->listnode );
                break;

            default :
                XASSERT( 0, msg.msgType );
                break;

        } /* end switch(msg.param1) */

    } /* end while( wait_done != current_done) */

    /* our message queue should be empty (no pending buffers) */
    qcnt = scos_mq_num_pending( msgq );
    XASSERT( qcnt==0, qcnt );

    dbg2("%s end\n", __FUNCTION__ );
}

/**
 * \brief  Wait for data from pictest dma transaction
 *
 * \author David Poole
 * \date 12-Feb-2013
 *
 *  Refactor of pictest_run_it()
 */

void pictest_pipe_wait( ATLISTENTRY *results_buffers, int expected_rows )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    scan_msg_t msg;
    struct scos_mq *msgq;
    int pic_received_rows;
    int icetest_received_rows;
    uint32_t pic_wait, icetest_wait;
    uint32_t current_done;
    int i;
    uint32_t qcnt;

    dbg2("%s start\n", __FUNCTION__ );

    /* build a bitmask we can use to do a simple compare to watch for all the
     * channels finishing
     */
    pic_wait = 0;
    current_done = 0;
    icetest_wait = 0;
    for( i=0 ; i<num_pic_channels ; i++ ) {
        XASSERT( pic_channels[i] < PIC_WDMA_NUM_CHANNELS, pic_channels[i] );
        SET_BIT( pic_wait, pic_channels[i] );
    }
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        XASSERT( icetest_channels[i] < PIC_WDMA_NUM_CHANNELS, icetest_channels[i] );
        SET_BIT( icetest_wait, icetest_channels[i] );
    }

    msgq = scantask_get_msgq();

    pic_received_rows = 0;
    icetest_received_rows = 0;

    /* spin waiting for all DMA to complete */
    while( icetest_wait || pic_wait ) {
        /* infinite loop unless our PIC DMA completes */
//        dbg2( "waiting up to 5 seconds for messages...\n" );
        
        scerr = scos_mq_receive( msgq, &msg, USEC_PER_SECOND*5 );

        if( scerr == SCANERR_TIMEDOUT ) {
            /* dump out some stats so we know where we are */
            dbg1("%s no messages; wait some more.\n", __FUNCTION__ );
            dbg2("icetest_wait=0x%x pic_wait=0x%x\n", icetest_wait, pic_wait );
//            pic_wdma_debug_log();
//            for( i=0 ; i<num_pic_channels ; i++ ) {
//                pic_wdma_channel_dump( pic_channels[i] );
//            }
            icetest_dump();
//            icetest_interrupt_stats_dump();
//            SCANALYZER_DUMP();
            dbg1("expected_rows=%d icetest_received_rows=%d pic_received_rows=%d\n", 
                        expected_rows*num_pic_channels, icetest_received_rows, pic_received_rows );

//            dbg1("interrupt count=%d\n", pic_interrupt_stats.count );
            continue;
        }

        switch( msg.msgType ) {
            case SMSG_PIC_WDMA_DATA :
                /* PIC DMA complete interrupt */
                PTR_ASSIGN( icebuf, msg.param3 );
                XASSERT( icebuf->channel < PIC_WDMA_NUM_CHANNELS, icebuf->channel );
                icebuf_sanity( icebuf );

                pic_received_rows += icebuf->num_rows;

                dbg2("pic wdma msg=%p channel=%d num_rows=%d max_rows=%d bpr=%d data=%p total=%d\n", 
                            icebuf, icebuf->channel, icebuf->num_rows,
                            icebuf->max_rows, icebuf->bytes_per_row, 
                            icebuf->data, pic_received_rows );

                /* davep 14-Feb-2013 ; XXX tmp debug */
//                dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
//                scanlog_hex_dump( icebuf->data, 16 );

                /* is this the last buffer in this channel? */
                if( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                    dbg2( "picw dma channel %d is done\n", icebuf->channel );
                    CLR_BIT( pic_wait, icebuf->channel );
                }

                ATInsertTailList( results_buffers, &icebuf->listnode );
                break;

            case SMSG_ICETEST_IDMA_DATA :
                /* ICETest DMA complete interrupt */
                PTR_ASSIGN( icebuf, msg.param3 );
                XASSERT( icebuf->channel < ICETEST_IDMA_NUM_CHANNELS, icebuf->channel );
                icebuf_sanity( icebuf );

                icetest_received_rows += icebuf->num_rows;

                dbg2("icetest idma icebuf=%p channel=%d num_rows=%d max_rows=%d bpr=%d data=%p total=%d\n", 
                            icebuf, icebuf->channel, icebuf->num_rows,
                            icebuf->max_rows, icebuf->bytes_per_row, 
                            icebuf->data, icetest_received_rows );

                /* davep 14-Feb-2013 ; XXX tmp debug */
                dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
                scanlog_hex_dump( icebuf->data, 16 );

                /* is this the last buffer in this channel? */
                if( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                    dbg2( "icetest channel %d is done\n", icebuf->channel );
                    CLR_BIT( icetest_wait, icebuf->channel );
                }

                /* put it back on the ready list so we can free it later */
                icetest_interrupt_disable();
                icetest_idma_add_ready( icebuf->channel, &icebuf );
                icetest_interrupt_enable();
                break;

            default :
                XASSERT( 0, msg.msgType );
                break;

        } /* end switch(msg.param1) */

    } /* end while( icetest_wait || pic_wait ) */

    /* our message queue should be empty (no pending buffers) */
    qcnt = scos_mq_num_pending( msgq );
    XASSERT( qcnt==0, qcnt );

    dbg2("%s end\n", __FUNCTION__ );
}


/**
 * \brief rolled some copy/paste code into function 
 *
 *
 * \author David Poole
 * \date 01-Nov-2007
 *
 */

void
verify_pic_buffer_counts( struct pic_dma_test *pictest )
{
    int i, cnt;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;

    dbg2( "%s\n", __FUNCTION__ );

    mm = pic_wdma_get_mm();

    for( i=0 ; i<pictest->num_channels ; i++ ) {
        ch = &mm->channels[ pictest->channels[i] ];

        cnt = ch->num_readies;

        /* davep 08-Nov-2007 ; XXX temp debug */
        dbg2( "%s ch=%d readies=%d empties=%d alloc=%d\n", __FUNCTION__,
                    ch->channel, ch->num_readies, ch->num_empties,
                    pictest->num_allocated[i] );

        XASSERT( ch->num_readies == pictest->num_allocated[i], 
                ch->num_readies );

        /* make sure the number of struct ice_dma_buffers we allocated matches */
        cnt += count_list( &ch->empties );

        dbg2( "%s %d==%d?\n", __FUNCTION__, cnt, pictest->num_empties );
        XASSERT( cnt==pictest->num_empties, cnt );
    }

}

//--------------------------------------------------------------------------
// Function     : pictest_bulbshadow
//   returns    : none
// Created by   : Brad Smith
// Date created : May 6 2005
// Description  : 
//
//  
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

#if 0
void
pictest_bulbshadow( scan_cmode_t cmode, int pixels_per_row )
{
    int i;
    uint32_t num32;
    uint8_t *pc_lut;
    uint32_t pc_lut_dma;
    scan_msg_t *msg;
    ATLISTENTRY results_buffers;
    struct pic_dma_test pictest;
    int cnt;
    int num_colors;

    pic_interrupt_disable();

    dbg1("%s start ppr=%d\n", __FUNCTION__, pixels_per_row );
    
    /* start PIC from a clean state */
    pic_setup_for_test();

    num32 = PIC_BM_LRMR_LEFT(0) | PIC_BM_LRMR_RIGHT( 64 );
    *PIC_BM_LRMR0 = num32;
    *PIC_BM_LRMR1 = num32;
    *PIC_BM_LRMR2 = num32;

    /* set up our DMA for a data run */
    memset( &pictest, 0, sizeof(struct pic_dma_test));
    pictest_choose_channels( cmode, &pictest );
    pictest.rows_per_buffer = 3;
    pictest.total_rows      = 9;
    /* XXX - hardwiring to 16bpp */
    pictest.bytes_per_row = pixels_per_row * 2;
    pictest.buffers_per_channel = pictest.total_rows / pictest.rows_per_buffer;
    pictest.num_empties = pictest.buffers_per_channel * pictest.num_channels; 

    pictest_setup_dma( &pictest );

    dbg1( "PIC WDMA %d channel bulb shadow test of rows=%d bytes_per_row=%d total_bytes=%d\n",
                pictest.num_channels, 
                pictest.total_rows, pictest.bytes_per_row,
                pictest.bytes_per_row * pictest.total_rows );


    // Set up PRNU/DSNU registers
    
    /*
     * 1 => 16bits (PRNU, DNSU LUT 8-bits each)
     */
    // Shift left 1 for PRNU, shift left 0 for DSNU
    num32 = PIC_PC_PDCR1_TOTAL_BITS(1)  |
            PIC_PC_PDCR1_PRNU_COLOR0(1) |
            PIC_PC_PDCR1_DSNU_COLOR0(0) |
            PIC_PC_PDCR1_PRNU_COLOR1(1) |
            PIC_PC_PDCR1_DSNU_COLOR1(0) |
            PIC_PC_PDCR1_PRNU_COLOR2(1) |
            PIC_PC_PDCR1_DSNU_COLOR2(0);
    *PIC_PC_PDCR1 = num32;

    // Disable exposure compensation, enable PRNU/DSNU/Bad pixel compensation
    num32 = PIC_PC_PDCR2_BYEXP
          | 0//PIC_PC_PDCR2_BYPRNU
          | 0// PIC_PC_PDCR2_BYDSNU
          | 0//PIC_PC_PDCR2_BYBAD
          ;

    *PIC_PC_PDCR2 = num32;

    dbg1("PDCR1=%#lx PDCR2=%#lx\n", *PIC_PC_PDCR1, *PIC_PC_PDCR2 );
    
    pic_prnudsnu_reset_lut_dma();
    pic_prnudsnu_enable_lut_dma(true);

    /* Make our pixel correction buffer. Need 2 bytes of correction data for
       each pixel for each channel. */
//    pc_lut = MEM_MALLOC( pixels_per_row*2*3 );
    if( cmode==SCAN_CMODE_MONO ) {
        num_colors = 1;
    }
    else {
        num_colors = 3;
    }
    pc_lut = MEM_MALLOC( pixels_per_row*2*num_colors );
    XASSERT( pc_lut!=NULL,(uint32_t)pc_lut );

    /* make sure we get back a proper DMA address */
    XASSERT( ((uint32_t)pc_lut&~ICE_DMA_BYTE_ALIGN_MASK)==0, (uint32_t)pc_lut );

    /* make it a real DMA address by pushing in the cache bypass */
    pc_lut_dma = (uint32_t)pc_lut | CACHE_BYPASS_OFFSET;

    // Load the PRNU/DSNU correction table
    for (cnt=0;cnt<pixels_per_row*2*num_colors;cnt+=2) {
        ((uint8_t *)pc_lut_dma)[cnt]=192; //PRNU
        ((uint8_t *)pc_lut_dma)[cnt+1]=0; //DSNU
    }

/*
    // test bad pixel replacement by setting the dsnu and prnu correction values to all 1's.
    // note: if you set the values for the first pixel to all 1's, you'll get all 1's out
    // (since PIC sets the pixel's value to white in this case).
    pc_lut[2]=0xff;
    pc_lut[3]=0xff;

*/
    /* dump my lookup table so I know what it looks like */
    dbg1("PRNU/DSNU lookup table\n" );
//    scanlog_hex_dump( pc_lut, pictest.bytes_per_row );
    //scanlog_hex_dump( ((uint8_t *)pc_lut_dma), 64 );

    dbg1("push %p as %#lx len %d into PRNU/DSNU read DMA\n", 
                pc_lut, pc_lut_dma, pixels_per_row*2*num_colors );
    pic_prnudsnu_set_lut_addr(pc_lut_dma);
    pic_prnudsnu_set_lut_xfer_count(pixels_per_row*2*num_colors);

    /* start it */
    pic_prnudsnu_start_lut_dma();


    // Set up Bulb Shadow registers

    *PIC_PC_BSLSVCR = 0x00a0a0ff; // left slope
    *PIC_PC_BSRSVCR = 0x00a0a0ff; // right slope

    *PIC_PC_BSRLSGCR =  0x6      | // right min gain for color0
                        0x60     | // left max gain for color0
                        0xc00    | // right min gain for color1
                        0xc000   | // left max gain value for color1
                        0xc0000  | // right min gain for color2
                        0xc00000;  // left max gain value for color2

    // Set up the left and right shadow configs.
    *PIC_PC_BSLMPCR0 = 40; // set left max pixel for color0
    *PIC_PC_BSLMPCR1 = 10; // set left max pixel for color1
    *PIC_PC_BSLMPCR2 = 10; // set left max pixel for color2

    *PIC_PC_BSRSPCR0 = 50; // set right start pixel for color0
    *PIC_PC_BSRSPCR1 = 40; // set right start pixel for color1
    *PIC_PC_BSRSPCR2 = 30; // set right start pixel for color2


    /* need to pass a few parameters to the thread that feeds data into ADC so
     * cheat a bit and use the AGMSG structure to do so.  I am responsible for
     * freeing this, not the thread. 
     */
    msg = (scan_msg_t *)MEM_MALLOC( sizeof(scan_msg_t) ); 
    XASSERT( msg != NULL,(uint32_t)msg );
    memset( msg, 0, sizeof(scan_msg_t) );

    /* send feeder task the number of rows and pixels per row */
    /* 14-Apr-05 ; added param3 as the 16-bit pixel value to send */
    msg->param1 = pictest.total_rows;
    msg->param2 = pixels_per_row;
    msg->param3 = (void *)0x10;
    
    /* create feeder thread suspended; pictest_run_it() will resume */
    pictest_make_thread( pictest_prnudsnu_feeder, (void *)msg );

    /* data results will land here */
    ATInitList( &results_buffers );

    /* fire in the hole! */
    pic_interrupt_enable();

    /* run the feeder thread */
    pictest_run_it( pictest_task_id, &results_buffers, 
            pictest.channels, pictest.num_channels, pictest.total_rows );

    /* done getting data, clean up thread */
    pictest_wait_for_feeder_complete();

    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_disable( pictest.channels[i] );
    }

    /* davep 27-Feb-2008 ; XXX XXX shouldn't be calling pic_prnudsnu_validate!
     * I'm fixing pic_prnudsu_validate() so it will actually validate PRNU/DSNU
     * test results.  Previous version would just hexdump and free the data so
     * I was just being lazy here.
     */
//    pic_prnudsnu_validate( num_colors, pictest.total_rows, pixels_per_row, &results_buffers );
    ASSERT(0);

    /* 
     * Put everything back on the ready list so we can count our buffers and find any memory leaks
     */
    pictest_dma_cleanup( &results_buffers );

   /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();

    /* count PIC write */
    verify_pic_buffer_counts( &pictest );

    pic_wdma_close();

    /* free the message we passed to our feeder thread */
    PTR_FREE( msg );
    PTR_FREE( pc_lut );

    /* Reset PIC to its power-on reset state */
    pic_cleanup_after_test();

    dbg1("%s end\n", __FUNCTION__ );
}
#endif

static void 
pic_newstyle_validate( uint32_t expected_bytes, ATLISTENTRY *results_buffers )
{
    struct ice_dma_buffer *icebuf;
    bool first;

    dbg1("%s start\n", __FUNCTION__ );

    first = true;

    while( 1 ) {
        icebuf = get_icebuf_ptr( results_buffers );
        if( icebuf== NULL ) {
            /* end of data, leave while(1) loop */
            break;
        }

        /* dump a bit of the buffer so I can see a little of what I got */
        if( first ) {
            scanlog_hex_dump( icebuf->data, 256 );
            first = false;
        }

        // TODO - make sure we have valid data 

        /* put it back on the ready list */
        pic_wdma_add_ready( icebuf->channel, &icebuf );
    }
    
    dbg1("%s end\n", __FUNCTION__ );
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 14-Feb-2013
 */

scan_err_t pictest_setup_icetest_dma( scan_cmode_t cmode, 
                                        int pixels_per_row,
                                        int rows_per_buffer,
                                        int pic_total_rows_per_channel )
{
    scan_err_t final_scerr;
    int i;
    int icebufs_allocated, num_rows, rows_remaining;
    int icetest_total_rows;
    int icetest_bytes_per_row;
    struct ice_dma_buffer *icebuf;

    icetest_total_rows = pic_total_rows_per_channel;
    if( cmode==SCAN_CMODE_COLOR ) {
        icetest_total_rows *= 3;
    }

    /* the ICETest block runs on 32-bit values */
    icetest_bytes_per_row = pixels_per_row * sizeof(uint32_t);

    dbg2( "%s icetest bytes_per_row=%d\n", __FUNCTION__,
            icetest_bytes_per_row );

    icebufs_allocated = icetest_idma_open( icetest_channels, num_icetest_channels, 20, 
                                            icetest_total_rows, icetest_bytes_per_row );
    XASSERT( icebufs_allocated==20, icebufs_allocated );

    /* allocate, initialize, and load buffers for ICETest */
    rows_remaining = icetest_total_rows;
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        while( rows_remaining > 0 ) {
            num_rows = MIN( rows_remaining, rows_per_buffer );

            rows_remaining -= num_rows;
            XASSERT( rows_remaining >= 0, rows_remaining );

            /* icetest needs 3x the number of rows because sending rows of
             * R,G,B through the same single DMA channel (the single buffer has
             * to hold three rows rather than one row in mono)
             */
            if( cmode==SCAN_CMODE_COLOR ) {
                num_rows *= 3;
            }

            dbg2( "%s total=%d remain=%d icetest_num_rows=%d rpb=%d\n", __FUNCTION__,
                    icetest_total_rows, rows_remaining, num_rows, rows_per_buffer );

            icebuf = icebuf_new( num_rows, icetest_bytes_per_row, ICEBUF_TAG_ICETEST_IDMA );
            if( icebuf==NULL ) {
                /* out of memory */
                final_scerr = SCANERR_OUT_OF_MEMORY;
                goto fail;
            }
            icebuf->num_rows = num_rows;

            icetest_test_buffer_init( cmode, icebuf->data, icebuf->datalen,
                                        pixels_per_row, icebuf->num_rows );

            icetest_idma_add_ready( icetest_channels[i], &icebuf );
        }
    }

    return SCANERR_NONE;
fail:
    return final_scerr;
}

/**
 * \brief Launch the icetest and PIC DMA's (start the test)
 *
 * Rolled a ball of code into a function.
 *
 * \author David Poole
 * \date 14-Feb-2013
 */

void pictest_launch_icetest_and_pic(void)
{
    int i;

    /* turn on the ICETest hardware */
    icetest_enable( true );

    /* prime the ICETest idma pump */
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        icetest_idma_channel_enable( icetest_channels[i] );
    }
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        icetest_idma_channel_launch( icetest_channels[i] );
    }

    /* need to explicitly turn on our channels */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_enable( pic_channels[i] );
    }
    
    /* feed initial buffers into the channels */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_launch( pic_channels[i] );
    }

    /* davep 14-Jan-2011 ; I *still* don't want to talk about it. >:-(  */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    pic_cwdma_late_launch();
#endif

    /* davep 12-Feb-2013 ; XXX temp debug */
//    icetest_dump();
//    pic_dump();

    /* fire in the hole! */
    pic_interrupt_enable();
    icetest_idma_late_launch();
}

/**
 * \brief  Turn off icetest and PIC hardware after test is complete
 *
 * Rolled a ball of code into a function.
 *
 * \author David Poole
 * \date 14-Feb-2013
 */

void pictest_shutdown_icetest_and_pic( void )
{
    int i;

    /* we're now done; prepare for validation and cleanup */
    pic_interrupt_disable();
    icetest_interrupt_disable();

    icetest_enable( false );
    icetest_idma_disable();

    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_disable( pic_channels[i] );
    }
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        icetest_idma_channel_disable( icetest_channels[i] );
    }
}

/**
 * \brief  Close down icetest and PIC dmas, releasing memory. Leave HW in clean
 * state for next test.
 *
 * Rolled a ball of code into a function.
 *
 * \author David Poole
 * \date 14-Feb-2013
 */

void pictest_cleanup_icetest_and_pic( void )
{
    scan_err_t scerr;

    /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();
    icetest_idma_sanity();

    /* verify everything is correct then free up allocated memory */
    pic_wdma_close();
    icetest_idma_close();

    pic_cleanup_after_test();

    scerr = icetest_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
// Notes        : 
//
//   davep 12-Feb-2013 ; this function is some of the oldest code we have. Now
//   updating to use ICETest rather than the now obsolete ANT register.
//
//--------------------------------------------------------------------------

scan_err_t pic_newstyle_test( scan_cmode_t cmode )
{
    scan_err_t scerr, final_scerr;
    int pic_total_rows_per_channel;
    int pic_bytes_per_row;
    int rows_per_buffer;
    int pixels_per_row;
    ATLISTENTRY results_buffers;
    int retcode, buffers_per_channel; 
    int i;

    dbg2( "%s cmode=%d start\n", __FUNCTION__, cmode );

    pic_interrupt_disable();

    pic_setup_for_test();

    /* 200 rows, 1712 pixels at 2-bytes per pixel.
     *
     * Should get 600 rows out (200 total rows for each of our three color
     * channels we're testing.
     */
    pic_total_rows_per_channel = 200;
    pixels_per_row = 1712;
    pic_bytes_per_row = pixels_per_row * 2;
    rows_per_buffer = 10;
    if( cmode==SCAN_CMODE_MONO ) {
        num_pic_channels = 1;
    }
    else {
        num_pic_channels = 3;
    }
    /* be conservative and allocate a huge amount of buffers */
    buffers_per_channel = pic_total_rows_per_channel;

    dbg1( "PIC WDMA %d channel test of rows=%d pic_bytes_per_row=%d total_bytes=%d\n",
                num_pic_channels,
                pic_total_rows_per_channel, pic_bytes_per_row, 
                pic_bytes_per_row*pic_total_rows_per_channel );

    /* data results will land here */
    ATInitList( &results_buffers );

    /* now start settup up the PIC write DMA data structures */

    /* 3-channel color test */
    if( cmode==SCAN_CMODE_MONO ) {
        pic_channels[0] = PIC_DMA_CHANNEL_MEVEN;
    }
    else {
        pic_channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
        pic_channels[1] = PIC_DMA_CHANNEL_CEVEN_1;
        pic_channels[2] = PIC_DMA_CHANNEL_CEVEN_2;
    }

    pic_wdma_open( pic_channels, num_pic_channels, pic_total_rows_per_channel,
                    pic_total_rows_per_channel, pic_bytes_per_row );

    /* Feed in some buffers, exactly enough to hold all the data we expect back;
     * the setup wdma buffers function returns the number of buffers that were
     * allocated; want to make sure we get at least SOME memory for all channels
     * or we won't even get off the ground
     *
     * The pictest_pipe_wait() function simple waits for PIC_WDMA_MESSAGES and
     * attaches the returning icebufs onto a list. The buffers are never
     * recycled.  Consequently we have to send in enough buffers for exactly
     * the data we're sending.
     */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        retcode = pic_wdma_setup_buffers( pic_channels[i], buffers_per_channel, 
                                          rows_per_buffer, pic_bytes_per_row );
        XASSERT( retcode==buffers_per_channel, retcode );
    }

    /* davep 12-Feb-2013 ; upgrade to ICETest */
    scerr = icetest_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    icetest_set_test_mode( ICETEST_CONFIG_DATA_TO_CISX );
#ifdef HAVE_CISX
    cisx_set_bypass( true );
#endif

    memset( icetest_channels, 0, sizeof(icetest_channels) );
    icetest_channels[0] = 0;
    num_icetest_channels = 1;

#if 1
    scerr = pictest_setup_icetest_dma( cmode, pixels_per_row, 
                            rows_per_buffer, pic_total_rows_per_channel);
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto fail;
    }
#else
    icetest_total_rows = pic_total_rows_per_channel;
    if( cmode==SCAN_CMODE_COLOR ) {
        icetest_total_rows *= 3;
    }

    /* the ICETest block runs on 32-bit values */
    icetest_bytes_per_row = pixels_per_row * sizeof(uint32_t);

    dbg2( "%s icetest bytes_per_row=%d\n", __FUNCTION__,
            icetest_bytes_per_row );

    icebufs_allocated = icetest_idma_open( icetest_channels, num_icetest_channels, 20, 
                                            icetest_total_rows, icetest_bytes_per_row );
    XASSERT( icebufs_allocated==20, icebufs_allocated );

    /* allocate, initialize, and load buffers for ICETest */
    rows_remaining = icetest_total_rows;
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        while( rows_remaining > 0 ) {
            num_rows = MIN( rows_remaining, rows_per_buffer );

            rows_remaining -= num_rows;
            XASSERT( rows_remaining >= 0, rows_remaining );

            /* icetest needs 3x the number of rows because sending rows of
             * R,G,B through the same single DMA channel (the single buffer has
             * to hold three rows rather than one row in mono)
             */
            if( cmode==SCAN_CMODE_COLOR ) {
                num_rows *= 3;
            }

            dbg2( "%s total=%d remain=%d icetest_num_rows=%d rpb=%d\n", __FUNCTION__,
                    icetest_total_rows, rows_remaining, num_rows, rows_per_buffer );

            icebuf = icebuf_new( num_rows, icetest_bytes_per_row, ICEBUF_TAG_ICETEST_IDMA );
            if( icebuf==NULL ) {
                /* out of memory */
                final_scerr = SCANERR_OUT_OF_MEMORY;
                goto fail;
            }
            icebuf->num_rows = num_rows;

            icetest_test_buffer_init( cmode, icebuf->data, icebuf->datalen,
                                        pixels_per_row, icebuf->num_rows );

            icetest_idma_add_ready( icetest_channels[i], &icebuf );
        }
    }
#endif

    pictest_launch_icetest_and_pic();
#if 0
    /* turn on the ICETest hardware */
    icetest_enable( true );

    /* prime the ICETest idma pump */
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        icetest_idma_channel_enable( icetest_channels[i] );
    }
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        icetest_idma_channel_launch( icetest_channels[i] );
    }

    /* need to explicitly turn on our channels */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_enable( pic_channels[i] );
    }
    
    /* feed initial buffers into the channels */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_launch( pic_channels[i] );
    }

    /* davep 14-Jan-2011 ; I *still* don't want to talk about it. >:-(  */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    pic_cwdma_late_launch();
#endif

    /* davep 12-Feb-2013 ; XXX temp debug */
//    icetest_dump();
//    pic_dump();

    /* fire in the hole! */
    pic_interrupt_enable();
    icetest_idma_late_launch();
#endif

    /* run the message loop */
    pictest_pipe_wait( &results_buffers, pic_total_rows_per_channel );

    pictest_shutdown_icetest_and_pic();
#if 0
    /* we're now done; prepare for validation and cleanup */
    pic_interrupt_disable();
    icetest_interrupt_disable();

    icetest_enable( false );
    icetest_idma_disable();

    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_disable( pic_channels[i] );
    }
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        icetest_idma_channel_disable( icetest_channels[i] );
    }
#endif
    pic_newstyle_validate( pic_total_rows_per_channel * pic_bytes_per_row, &results_buffers );

    pictest_cleanup_icetest_and_pic();
#if 0
    /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();
    icetest_idma_sanity();

    /* verify everything is correct then free up allocated memory */
    pic_wdma_close();
    icetest_idma_close();

    pic_cleanup_after_test();

    scerr = icetest_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );
#endif

    dbg1( "%s cmode=%d done\n", __FUNCTION__, cmode );

    return SCANERR_NONE;

fail : 
    icetest_idma_close();
    pic_wdma_close();
    pic_cleanup_after_test();
    return final_scerr;
}

//--------------------------------------------------------------------------
// Function     : pictest_margin_feeder
//   returns    : none
//   input      : from ThreadX, pointer to a scan_msg_t structure containing
//                information about data to send to ADC Norm Test register
// Created by   : David Poole
// Date created : 16-Apr-05
// Description  : 
//
//  Feeds the number of rows and pixels per row into the ADC test register for
//  the Left/Right Margin tests.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

thread_return_t pictest_margin_feeder( void *input )
{
    scan_msg_t *msg;
//    uint32_t num32;
    int pcnt, ccnt, rcnt;
    int color[3];
    int rows, pixels_per_row, wordcount;
    int total_bytes;
    int sanity;
    uint16_t pixel;
//    uint32_t test_exposure;
    int num_colors;
    scan_cmode_t cmode;

    dbg1("%s start\n", __FUNCTION__ );

    /* wait until my parent thread is ready to let me run */
    pictest_feeder_wait_for_release();

    /* Input should be a pointer to a message structure containing the number
     * of rows to feed into the ADC test register and the number of pixels in
     * each row.
     *
     * ADC wants words so we need to count words.
     */ 
    msg = (scan_msg_t *)input;

    rows = msg->param1;
    pixels_per_row = msg->param2;
    /* davep 12-Nov-2007 ; change param3 to mono vs color */
    cmode = (scan_cmode_t)msg->param3;
    XASSERT( cmode==SCAN_CMODE_MONO || cmode==SCAN_CMODE_COLOR, cmode );
//    test_exposure = (uint32_t)msg->param3;
//    test_exposure = false;

    /* hardwire ourselves to 2-byte per pixels */
    wordcount = pixels_per_row;

    /* "*2" for 2 bytes per pixel, "*3" for 3 color planes */
    if( cmode==SCAN_CMODE_MONO ) {
        total_bytes = rows * wordcount * 2;
    }
    else {
        total_bytes = rows * wordcount * 2 * 3;
    }

    dbg1("%s sending %d rows * %d pixels_per_row=%d words bytes=%d\n", 
                __FUNCTION__, rows, pixels_per_row, rows*wordcount, total_bytes );

    /* want to send in 16-bit per pixel RGB data */
    if( cmode==SCAN_CMODE_MONO ) {
        color[0] = PIC_CBI_MEVEN;
        num_colors = 1;
    }
    else {
        color[0] = PIC_CBI_CEVEN_0;
        color[1] = PIC_CBI_CEVEN_1;
        color[2] = PIC_CBI_CEVEN_2;
        num_colors = 3;
    }

    sanity = 0;
    /* rcnt = row count 
     * ccnt = color count
     * pcnt = pixel count
     */
    for( rcnt=0 ; rcnt<rows ; rcnt++ ) {

//        // If requested, send in exposure data at the start of each row.
//        if (test_exposure)
//            num32 = PIC_ADCN_ANT_DATATYPE(0) 
//            | PIC_ADCN_ANT_COLOR(0) 
//            | PIC_ADCN_ANT_EXP
//            | PIC_ADCN_ANT_DATA(1712)
//            ;

        dbg2( "%s sending row %d of %d\n", __FUNCTION__, rcnt+1, rows );

        /* send in planes a line at a time like CIS does */
        for( ccnt=0 ; ccnt<num_colors ; ccnt++ ) {

            dbg2( "%s sending color %d of %d\n", __FUNCTION__, ccnt, num_colors );

            /* want an entire line of data to be pixels values from
             * 0..pixels_per_row so we can tell exactly where the margins are
             * chopping us
             */
            pixel = 0;

            /* send in a starting pixel; PIC needs to know when it's receiving
             * the last line of data thus the _FIRPLL vs _FIRP 
             */
            if( rcnt == rows-1) {
                /* davep 02-Apr-2010 ; don't use FIRPLL (breaks descriptor
                 * based PIC WDMA)
                 */
//                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
//                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
//                        | PIC_ADCN_ANT_DATA(pixel)
//                        ;
//                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRPLL) 
//                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
//                        | PIC_ADCN_ANT_DATA(pixel)
//                        ;
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRP, color[ccnt], pixel );
            }
            else {
                /* first pixel in non-last line */
//                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
//                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
//                        | PIC_ADCN_ANT_DATA(pixel)
//                        ;
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRP, color[ccnt], pixel );
            }
//            *PIC_ADCN_ANT = num32;
            sanity++;
            pixel++;

            /* now send in a block of data;
             * -2 for the starting and ending pixels 
             */
            for( pcnt=0 ; pcnt<wordcount-2 ; pcnt++ ) {
//                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_NORMP) 
//                      | PIC_ADCN_ANT_COLOR( color[ccnt] ) 
//                      | PIC_ADCN_ANT_DATA( pixel )
//                      ;
//                *PIC_ADCN_ANT = num32;
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_NORMP, color[ccnt], pixel );
                sanity++;
                pixel++;
            }

            /* send in a last pixel */
//            num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_LASTP) 
//                  | PIC_ADCN_ANT_COLOR( color[ccnt] ) 
//                  | PIC_ADCN_ANT_DATA( pixel )
//                  ;
//            *PIC_ADCN_ANT = num32;
            pic_adcnorm_ant( PIC_ADCN_TEST_DT_LASTP, color[ccnt], pixel );
            sanity++;
            pixel++;
        }

//        num32 = PIC_ADCN_ANT_DATATYPE(0) 
//                | PIC_ADCN_ANT_COLOR(0) 
//                | PIC_ADCN_ANT_EXP
//                | PIC_ADCN_ANT_DATA(1712)
//                ;

        /* davep 14-Feb-2008 ; shotgun debug for pic overflow; delay between
         * each line (line -> mono==1 row, color==3 rows)
         */
//        scos_thread_sleep(1*SECONDS);
                
    }

    XASSERT( sanity*2 == total_bytes, sanity );

    dbg1("Done sending %d words of data\n", sanity );
    dbg1("%s end\n", __FUNCTION__ );

    /* release the lock I'm holding */
    pictest_feeder_release();

    return (thread_return_t)0;
}

//--------------------------------------------------------------------------
// Function     : pictest_hscale_feeder
//   returns    : none
//   input      : from ThreadX, pointer to a scan_msg_t structure containing
//                information about data to send to ADC Norm Test register
// Created by   : David Poole
// Date created : 16-Apr-05
// Description  : 
//
//  Feeds the number of rows and pixels per row into the ADC test register for
//  the Left/Right Margin tests.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void 
pictest_hscale_feeder( unsigned long int input )
{
    scan_msg_t *msg;
    uint32_t num32;
    int pcnt, ccnt, rcnt;
    int color[3];
    int rows, pixels_per_row, wordcount;
    int total_bytes;
    int sanity;
    uint16_t pixel;
    bool test_exposure;
    int num_colors;
    scan_cmode_t cmode;

    dbg1("%s start\n", __FUNCTION__ );

    /* davep 08-Nov-2007 ; hardwire to mono for now */
    cmode = SCAN_CMODE_MONO;
    
    /* Input should be a pointer to a message structure containing the number
     * of rows to feed into the ADC test register and the number of pixels in
     * each row.
     *
     * ADC wants words so we need to count words.
     */ 
    msg = (scan_msg_t *)input;

    rows = msg->param1;
    pixels_per_row = msg->param2;
    test_exposure = (uint32_t)msg->param3;

    /* hardwire ourselves to 2-byte per pixels */
    wordcount = pixels_per_row;

    dbg1("sending %d rows * %d pixels_per_row = %d words\n", 
                rows, pixels_per_row, rows*wordcount );
    /* "*2" for 2 bytes per pixel, "*3" for 3 color planes */
    if( cmode==SCAN_CMODE_MONO ) {
        total_bytes = rows * wordcount * 2;
    }
    else {
        total_bytes = rows * wordcount * 2 * 3;
    }

    dbg1("total bytes=%d\n", total_bytes );

    /* want to send in 16-bit per pixel RGB data */
    if( cmode==SCAN_CMODE_MONO ) {
        color[0] = PIC_CBI_MEVEN;
        num_colors = 1;
    }
    else {
        color[0] = PIC_CBI_CEVEN_0;
        color[1] = PIC_CBI_CEVEN_1;
        color[2] = PIC_CBI_CEVEN_2;
        num_colors = 3;
    }

    sanity = 0;
    /* rcnt = row count 
     * ccnt = color count
     * pcnt = pixel count
     */
    for( rcnt=0 ; rcnt<rows ; rcnt++ ) {

        // If requested, send in exposure data at the start of each row.
        if (test_exposure) {
            num32 = PIC_ADCN_ANT_DATATYPE(0) 
                | PIC_ADCN_ANT_COLOR(0) 
                | PIC_ADCN_ANT_EXP
                | PIC_ADCN_ANT_DATA(1712)
                ;
        }

        /* send in color planes a line at a time like CIS does */
        for( ccnt=0 ; ccnt<num_colors ; ccnt++ ) {

            /* want an entire line of data to be pixels values from
             * 0..pixels_per_row so we can tell exactly where the margins are
             * chopping us
             */
            pixel = 0;

            /* send in a starting pixel; PIC needs to know when it's receiving
             * the last line of data thus the _FIRPLL vs _FIRP 
             */
            if( rcnt == rows-1) {
                /* davep 02-Apr-2010 ; don't use FIRPLL (breaks descriptor
                 * based PIC WDMA)
                 */
                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
                        | PIC_ADCN_ANT_DATA(pixel)
                        ;
//                /* first pixel in last line */
//                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRPLL) 
//                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
//                        | PIC_ADCN_ANT_DATA(pixel)
//                        ;
            }
            else {
                /* first pixel in non-last line */
                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
                        | PIC_ADCN_ANT_DATA(pixel)
                        ;
            }
            *PIC_ADCN_ANT = num32;
            sanity++;
            pixel++;

            /* now send in a block of data;
             * -2 for the starting and ending pixels 
             */
            for( pcnt=0 ; pcnt<wordcount-2 ; pcnt++ ) {
                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_NORMP) 
                      | PIC_ADCN_ANT_COLOR( color[ccnt] ) 
                      | PIC_ADCN_ANT_DATA( pixel )
                      ;
                *PIC_ADCN_ANT = num32;
                sanity++;
                pixel++;
            }

            /* send in a last pixel */
            num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_LASTP) 
                  | PIC_ADCN_ANT_COLOR( color[ccnt] ) 
                  | PIC_ADCN_ANT_DATA( pixel )
                  ;
            *PIC_ADCN_ANT = num32;
            sanity++;
            pixel++;
        }

        num32 = PIC_ADCN_ANT_DATATYPE(0) 
                | PIC_ADCN_ANT_COLOR(0) 
                | PIC_ADCN_ANT_EXP
                | PIC_ADCN_ANT_DATA(1712)
                ;
    }

    XASSERT( sanity*2 == total_bytes, sanity );

    dbg1("Done sending %d words of data\n", sanity );
    dbg1("%s end\n", __FUNCTION__ );
}

//--------------------------------------------------------------------------
// Function     : pictest_addgain_feeder
//   returns    : none
//   input      : from ThreadX, pointer to a scan_msg_t structure containing
//                information about data to send to ADC Norm Test register
// Created by   : David Poole
// Date created : 16-Apr-05
// Description  : 
//
//  Feeds the number of rows and pixels per row into the ADC test register for
//  the Left/Right Margin tests.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

#if 0
static void 
pictest_addgain_feeder( unsigned long int input )
{
    /* davep 01-Jul-2008 ; dig through the Perforce archives to find the body of
     * this function.  Sorry.
     */
}
#endif

//--------------------------------------------------------------------------
// Function     : pictest_bdr_feeder
//   returns    : none
//   input      : from ThreadX, pointer to a scan_msg_t structure containing
//                information about data to send to ADC Norm Test register
// Created by   : Brad Smith
// Date created : 29-Apr-05
// Description  : 
//
//  Feeds data into the ANR test register for the purpose of testing
//  the bit depth reduction hardware.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

static void 
pictest_bdr_feeder( unsigned long int input )
{
    scan_msg_t *msg;
    uint32_t num32;
    int pixel, pcnt, ccnt, rcnt;
    int color[3];
    int rows, pixels_per_row, wordcount;
    int total_bytes;
    int sanity;

    dbg1("%s start\n", __FUNCTION__ );

    /* Input should be a pointer to a message structure containing the number
     * of rows to feed into the ADC test register and the number of pixels in
     * each row.
     *
     * ADC wants words so we need to count words.
     */ 
    msg = (scan_msg_t *)input;

    rows = msg->param1;
    pixels_per_row = msg->param2;

    wordcount = pixels_per_row;

    dbg1("sending %d rows * %d pixels_per_row = %d words\n", 
                rows, pixels_per_row, rows*wordcount );
    /* "*3" for 3 color planes */
    total_bytes = rows * wordcount * 3;
    dbg1("total bytes=%d\n", total_bytes );

    /* want to send in 16-bit per pixel RGB data */
    color[0] = PIC_CBI_CEVEN_0;
    color[1] = PIC_CBI_CEVEN_1;
    color[2] = PIC_CBI_CEVEN_2;

    sanity = 0;
    /* rcnt = row count 
     * ccnt = color count
     * pcnt = pixel count
     */
    for( rcnt=0 ; rcnt<rows ; rcnt++ ) {

        /* send in three color planes a line at a time like CIS does */
        for( ccnt=0 ; ccnt<3 ; ccnt++ ) {

            /* want an entire line of data to be pixel values from
             * 0..pixels_per_row.
             */
            pixel = 0;

            /* send in a starting pixel; PIC needs to know when it's receiving
             * the last line of data thus the _FIRPLL vs _FIRP 
             */
            if( rcnt == rows-1) {
                /* davep 02-Apr-2010 ; don't use FIRPLL (breaks descriptor
                 * based PIC WDMA)
                 */
                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
                        | PIC_ADCN_ANT_DATA(pixel)
                        ;
//                /* first pixel in last line */
//                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRPLL) 
//                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
//                        | PIC_ADCN_ANT_DATA(pixel)
//                        ;
            }
            else {
                /* first pixel in non-last line */
                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
                        | PIC_ADCN_ANT_COLOR(color[ccnt]) 
                        | PIC_ADCN_ANT_DATA(pixel)
                        ;
            }
            *PIC_ADCN_ANT = num32;
            sanity++;
            pixel++;

            /* now send in a block of data;
             * -2 for the starting and ending pixels 
             */
            for( pcnt=0 ; pcnt<wordcount-2 ; pcnt++ ) {
                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_NORMP) 
                      | PIC_ADCN_ANT_COLOR( color[ccnt] ) 
                      | PIC_ADCN_ANT_DATA( pixel )
                      ;
                *PIC_ADCN_ANT = num32;
                sanity++;
                pixel++;
            }

            /* send in a last pixel */
            num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_LASTP) 
                  | PIC_ADCN_ANT_COLOR( color[ccnt] ) 
                  | PIC_ADCN_ANT_DATA( pixel )
                  ;
            *PIC_ADCN_ANT = num32;
            sanity++;
            pixel++;
        }
    }

    // hard-coded for 2 byte-per-pixel
    dbg1( "sanity = %d, total_bytes = %d",sanity*2,total_bytes );
    XASSERT( sanity == total_bytes, sanity );

    dbg1("Done sending %d words of data\n", sanity );
    dbg1("%s end\n", __FUNCTION__ );
}
//--------------------------------------------------------------------------
// Function     : pictest_margin_validate
//   returns    : none
//   results_buffers : linked list of struct ice_dma_buffer containing PIC WDMA
//                     data
//   left_margin     : left margin setting from Left/Right Margin block
//                     register
//   right_margin    : right margin setting from Left/Right Margin block 
//                     register
// Created by   : David Poole
// Date created : 16-Apr-05
// Description  : 
//
//  Input data to the margin test is a 16bit number tracking the pixel.
//  0,1,2,3,4, ... number of pixels-1
//
//  The PIC Left/Rigth Margin block should have cut off the left and right side
//  of the pixel values. The new data values should start at the left margin
//  value and end at the right margin value.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

static void
pictest_margin_validate( ATLISTENTRY *results_buffers,
                         int left_margin,
                         int right_margin )
{
    struct ice_dma_buffer *icebuf;
    uint16_t *ptr16, *endptr16;
    uint16_t first_pixel, last_pixel, curr_pixel;
    int rcnt;
    bool first;

    dbg1("%s start\n", __FUNCTION__ );

    /* data is expected to be pixels 0,1,2,...max where max is the number of
     * pixels in the row
     */
    first_pixel = left_margin;
    last_pixel = right_margin;
    curr_pixel = first_pixel;

    first = true;

    while( 1 ) {
        icebuf = icebuf_pop( results_buffers );
        if( icebuf== NULL ) {
            /* end of data, leave while(1) loop */
            break;
        }

        cpu_dcache_invalidate_region( icebuf->data, icebuf->bytes_per_row*icebuf->num_rows );

        if( first ) {
            scanlog_hex_dump( icebuf->data, 256 );
            first = false;
        }

        /* make sure we have valid data in each row */
        for( rcnt=0 ; rcnt<icebuf->num_rows ; rcnt++ ) {

            curr_pixel = first_pixel;
            ptr16 = (uint16_t *)icebuf->data;
            endptr16 = (uint16_t *)(icebuf->data + icebuf->bytes_per_row);

            /* check first pixel (a specific value)  */

            while( ptr16 < endptr16 ) {
                if( *ptr16 != curr_pixel ) {

                    dbg1("%s %d: %#x!=%#x at %p in %p\n",
                            __FUNCTION__, __LINE__, 
                            *ptr16, curr_pixel, 
                             ptr16, icebuf->data );

                    XASSERT( 0, (uint32_t)ptr16 );
                }
                
                curr_pixel++;
                ptr16++;
            }
            /* make sure we ended where we were supposed to; last_pixel==0
             * means the right margin was disabled so we've hit the end of the
             * row when we've counted bytes_per_row
             */
            if( last_pixel != PIC_LRMR_RIGHT_MARGIN_DISABLED ) {
                XASSERT( curr_pixel-1==last_pixel, curr_pixel-1 );
            }
        }

        /* put it back on the ready list */
        pic_wdma_add_ready( icebuf->channel, &icebuf );
    }
    
    /* 21-Apr-05 ; dump out the bulb monitor registers */
//    dbg2( "PIC_BM_BMR0=%d\n", *PIC_BM_BMR0 );
//    dbg2( "PIC_BM_BMR1=%d\n", *PIC_BM_BMR1 );
//    dbg2( "PIC_BM_BMR2=%d\n", *PIC_BM_BMR2 );
//    dbg2( "PIC_BM_BMR3=%#lx\n", *PIC_BM_BMR3 );
    
    dbg1("%s end\n", __FUNCTION__ );
}
//--------------------------------------------------------------------------
// Function     : pictest_dma_cleanup
//   returns    : none
//   results_buffers : linked list of struct ice_dma_buffer containing PIC WDMA
//                     data
//   
// Created by   : Brad Smith
// Date created : 27-Apr-05
// Description  : 
//
// Returns all results_buffers to the ready list. 
// Notes        : 
//
//--------------------------------------------------------------------------

void
pictest_dma_cleanup( ATLISTENTRY *results_buffers )
{
    struct ice_dma_buffer *icebuf;

    dbg2("%s start\n", __FUNCTION__ );

    while( 1 ) {
        icebuf = icebuf_pop( results_buffers );
        if( icebuf== NULL ) {
            /* end of data, leave while(1) loop */
            break;
        }

        /* put it back on the ready list */
        pic_wdma_add_ready( icebuf->channel, &icebuf );
    }
    
    dbg2("%s end\n", __FUNCTION__ );
}

#if 0
static void
pictest_bulbmon_exposure_validate( void )
{
    dbg1("%s start\n", __FUNCTION__ );

    dbg1(  "Bulb Monitor exposure = %d\n",(int)*PIC_BM_BMR3);

    dbg1("%s end\n", __FUNCTION__ );
}
#endif

#if 0
static void
pictest_addgain_validate( ATLISTENTRY *results_buffers, uint16_t *predicted_values)
{
    /* davep 01-Jul-2008 ; dig through the Perforce archives to find the body of
     * this function.  Sorry.
     */
}
#endif

/**
 * \brief Validate the data from a PIC BDR test run. 
 *
 * TODO 
 *
 *
 * \author David Poole
 * \date 28-Feb-2008
 *
 */

static void
pictest_bdr_validate( ATLISTENTRY *results_buffers )
{
    /* davep 28-Feb-2008 ; TODO  write a C function that will validate the
     * output data based on the LUT and the input data.
     *
     * Most of the "validate" functions in pictest.c don't actually validate.
     * During the original incarnation of pictest.c, we didn't have enough
     * knowledge of how PIC worked to actually validate it. If the data ran
     * through an enabled block, that was a victory in and of itself.
     *
     * I want to change that. I want real validation to occur in here.  Someday.
     *
     * The original validate functions were responsible for putting the data
     * back onto the ready list. As we added more and more test functions, we
     * eventually would just copy/paste a function that would hexdump the data
     * and release the icebuf. We'd visually inspect the data and say, "Well, I
     * think that's correct". 
     *
     * See also dump_and_release()
     *
     */
}

static void
pictest_hscale_validate( ATLISTENTRY *results_buffers )
{
    uint16_t* dptr0;
    int row;
    struct ice_dma_buffer *icebuf;

    dbg1("%s start\n", __FUNCTION__ );

    while( 1 ) {
        icebuf = icebuf_pop( results_buffers );
        if( icebuf== NULL ) {
            break;
        }

        cpu_dcache_invalidate_region( icebuf->data, icebuf->bytes_per_row*icebuf->num_rows );

        /* davep 12-Nov-2007 ; XXX temp debug */
        dbg2( "%s icebuf=%p ch=%d\n", __FUNCTION__, icebuf, icebuf->channel );

        dptr0 = (uint16_t *)icebuf->data;

        for(row=0;row<icebuf->num_rows;row++,dptr0+=icebuf->bytes_per_row/2) {
            scanlog_hex_dump( (unsigned char *)dptr0, 16 );
            /* TODO validate pixels */
        }
        pic_wdma_add_ready( icebuf->channel, &icebuf );
    }

    dbg1("%s end\n", __FUNCTION__ );
}

//--------------------------------------------------------------------------
// Function     : pic_adjust_margin_size
//   returns    : none
//   pixels_per_row : number of pixels being sent into ADC
//   left_pixels    : number of pixels to remove from the left side
//   right_pixels   : number of pixels to remove from the right side
//   out_pixels     : after adjustment, number of pixels to expect out the
//                    bottom of PIC WDMA
//   left_margin    : the left margin setting for the Left/Right Margins
//                    register
//   right_margin   : the right margin setting for the Left/Right Margins
//                    register
// Created by   : David Poole
// Date created : 16-Apr-05
// Description  : 
//
//  Given a number of pixels per row and the desired pixels to remove from each
//  edge, shrink the left and right margins equally to make the number of bytes
//  per row land exactly on a 16-byte DMA burst boundry.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

static void
pic_adjust_margin_size( int pixels_per_row, int left_pixels, int right_pixels,
                        int *out_pixels, int *left_margin, int *right_margin )
{
    int extra_pixels, extra_left, extra_right;
    int check;

    /* left_margin is the first pixel we'll see.
     * right_margin-1 is the last pixel we'll see.
     * Both are exactly what should be fed into the L/R Margins registers
     * (LRMR0, LRMR1, LRMR2).
     *
     * right_pixels == 0  -- margin disabled, no pixels chopped 
     * left_pixels == 0   / 
     */

    /* stupid human check */
    XASSERT( left_pixels + right_pixels < pixels_per_row, pixels_per_row );

    if( right_pixels==0 ) {

        /* right margin disabled */
        *right_margin = PIC_LRMR_RIGHT_MARGIN_DISABLED;

        *left_margin = left_pixels;
        *out_pixels = pixels_per_row - left_pixels;

    }
    else if( left_pixels == 0  ) {

        /* left margin disabled */
        *left_margin = PIC_LRMR_LEFT_MARGIN_DISABLED;

        *right_margin = pixels_per_row - right_pixels -1;
        *out_pixels = *right_margin+1;

    }
    else  {
        *left_margin = left_pixels;
        *right_margin = pixels_per_row - right_pixels-1;
        *out_pixels = *right_margin - *left_margin + 1;
    }

//    *left_margin = left_pixels;
//    *right_margin = pixels_per_row - right_pixels;

//    *out_pixels = *right_margin - *left_margin + 1;

    /* Now balance to DMA burst size by increasing the left margin and
     * decreasing the right margin to chop off extra pixels. A pixel value of
     * zero means we don't want any chopping on that side so we'll put the
     * whole thing on the other side.
     */
    extra_pixels = *out_pixels % ICE_DMA_BYTE_ALIGN;
    extra_left = extra_pixels / 2;
    extra_right = extra_pixels - extra_left;

    /* recalculate */
    if( right_pixels == 0 ) {
        /* right margin disabled, extra must come from left */
        *left_margin += extra_pixels;
    }
    else if( left_pixels==0 ) {
        /* left margin disabled, extra must come from right */
        *right_margin -= extra_pixels;
    }
    else  {
        /* split the difference */
        *left_margin += extra_left;
        *right_margin -= extra_right;
    }

    *out_pixels -= extra_pixels;

//    *left_margin += extra_left;
//    *right_margin -= extra_right;

//    *out_pixels = *right_margin - *left_margin + 1;
    /* must have at least one complete burst */
    XASSERT( *out_pixels >= ICE_DMA_BYTE_ALIGN, *out_pixels );

    if( right_pixels == 0 ) {
        check = *out_pixels + *left_margin;
        XASSERT( check == pixels_per_row, check );
    }
    else if( left_pixels == 0 ) {
        check = *out_pixels + (pixels_per_row-*right_margin-1);
        XASSERT( check == pixels_per_row, check );
    }
    else {
        check = *out_pixels + *left_margin + (pixels_per_row - *right_margin-1);
        XASSERT( check == pixels_per_row, check ); 
    }

    if( *right_margin != PIC_LRMR_RIGHT_MARGIN_DISABLED ) {
        XASSERT( *right_margin < pixels_per_row, *right_margin );
        if( *left_margin != PIC_LRMR_LEFT_MARGIN_DISABLED ) {
            XASSERT( *right_margin > *left_margin, *right_margin );
        }
    }

    XASSERT( *left_margin >= 0, *left_margin );

    dbg2( "%s ppr=%d left_pixels=%d right_pixels=%d out_pixels=%d left_margin=%d right_margin=%d\n", 
                __FUNCTION__, pixels_per_row, left_pixels, right_pixels,
                *out_pixels, *left_margin, *right_margin );
}

//--------------------------------------------------------------------------
// Function     : pic_margin_test
//   returns    : none
//   pixels_per_row_in : number of pixels sent in the top of PIC
//   left_pixels  : how many pixels to cut from the left side
//   right_pixels : how many pixels to cut from the right side
// Created by   : David Poole
// Date created : 16-Apr-05
// Description  : 
//
//  Tests PIC left/right margins. 
//
//  As of this writing (16-Apr-05) this function is just a sandbox to learn how
//  the left/right margins work.
//
//  17-Apr-05; L/R Margins work pretty well. Have a pretty good test function
//  going.
// 
// Notes : 
//
// !!!
// !!! Assumes 16 bpp!
// !!!
//
//--------------------------------------------------------------------------

void run_margin_test( scan_cmode_t cmode, int pixels_per_row_in, int left_pixels, int right_pixels )
{
    scan_msg_t *msg;
    ATLISTENTRY results_buffers;
    int i;
//    int i, cnt;
    struct pic_dma_test pictest;
//    uint32_t num32;
    int pixels_per_row_out;
    int left_margin, right_margin;
    bool margins_disabled;

    dbg2( "%s mode=%d ppr=%d left=%d right=%d\n", __FUNCTION__, cmode,
                pixels_per_row_in, left_pixels, right_pixels );

    pic_interrupt_disable();

    pic_setup_for_test();

    if( left_pixels==0 && right_pixels==0 ) {
        margins_disabled = true;

        left_margin = PIC_LRMR_LEFT_MARGIN_DISABLED;
        right_margin = PIC_LRMR_RIGHT_MARGIN_DISABLED;
        pixels_per_row_out = pixels_per_row_in;
    }
    else {
        margins_disabled = false;

        /* fiddle the edges to find the margin register settings and move them
         * around such that we have exactly 16-byte DMA burst size amount of
         * data
         */
        pic_adjust_margin_size( pixels_per_row_in, left_pixels, right_pixels, 
                &pixels_per_row_out, &left_margin, &right_margin );
    }

    dbg2( "%s left_margin=%d right_margin=%d pixels_out=%d\n", __FUNCTION__,
                left_margin, right_margin, pixels_per_row_out );

    /* 16-Apr-05 ; the pixel margins are a little odd but I think I understand
     * them. The LR Margins are matches to counters within the block. 
     *
     * In testing, given 1712 pixels numbered 0,1,2,...1711, a Left margin of
     * 12, and a Right margin of 1700, the first pixel I see out is 12 (the
     * thirteenth pixel) and the last pixel I see is 1700.
     *
     * I think the Margin Block starts sending out pixels when some counter is
     * equal to the Left Margin and stops sending out pixels when the
     * counter is equal to the Right Margin.
     */

    /* set up PIC margins (they're in the Bulb Monitor and L/R Margin block) */
    /* davep 14-Jan-2011 ; update to new margins */
    pic_marg_set_left( left_margin );
    pic_marg_set_right( right_margin );
//    num32 = PIC_BM_LRMR_LEFT(left_margin) | PIC_BM_LRMR_RIGHT( right_margin );
//    *PIC_BM_LRMR0 = num32;
//    *PIC_BM_LRMR1 = num32;
//    *PIC_BM_LRMR2 = num32;

    /* 21-Apr-05 ; turn on bulb monitor 
     * Note that WIN (window size) is a -1 field.
     */
    /* davep 14-Jan-2011 ; update to new margins */
    pic_bm_set_color_counter(1);
    pic_bm_set_lines_to_avg(2);
    pic_bm_set_window_size(16);
    pic_bm_set_startpix(left_margin);
//    num32 = PIC_BM_BMCR_CC 
//          | PIC_BM_BMCR_NUML(1)  /* 2 line averaged */
////          | PIC_BM_BMCR_NUML(0)  /* 1 line averaged */
//          | PIC_BM_BMCR_WIN(16-1)  /* "historical value" according to BradS */
////          | PIC_BM_BMCR_WIN(1)  
////          | PIC_BM_BMCR_P(0)
//          | PIC_BM_BMCR_P( left_margin )
//          ;
//    *PIC_BM_BMCR = num32;

    /* enable BMB (Bulb Monitor Block) by clearing bypass bit */
//    REG_CLEAR_BITS( PIC_BM_BMCR, PIC_BM_BMCR_BYPASS );
    pic_bm_set_bypass( false );
    pic_marg_set_bypass( false );

//    dbg2( ("PIC_BM_BMCR=%#lx PIC_BM_LRMR0=%#lx 1=%#lx 2=%#lx\n", 
//                *PIC_BM_BMCR, *PIC_BM_LRMR0, *PIC_BM_LRMR1, *PIC_BM_LRMR2 );

    /* set up our DMA for a data run */
    memset( &pictest, 0, sizeof(struct pic_dma_test));
    pictest_choose_channels( cmode, &pictest );
    
    /* davep 14-Feb-2008 ; I'm passing pictest.total_rows to the feeder thread.
     * The feeder thread will send that many rows to each channel. So, in mono,
     * we'll get total_rows of data.  In color, we'll get total_rows*3 rows of
     * data.
     */
//    pictest.total_rows = 600;
    pictest.total_rows = 30; 
//    pictest.total_rows = 5; 
    /* XXX - hardwiring to 16bpp */
    pictest.bytes_per_row = pixels_per_row_out * 2;
    pictest.rows_per_buffer = 5;

#ifdef HAVE_ICE_LITE
    if( cmode==SCAN_CMODE_COLOR ) {
        /* davep 14-Feb-2008 ; color on ICE Lite sends all the rows to the same
         * channel so we'll need to make sure we have enough buffer space for
         * all our expected data. Remember: these tests don't recycle buffers.
         */
        pictest.total_rows *= 3;
    }
#endif

    /* +1 so we'll get one extra buffer to hold any remainder when
     * total_rows%rows_per_buffer != 0 
     */
    pictest.buffers_per_channel = pictest.total_rows /
                                  pictest.rows_per_buffer + 1;

    pictest.num_empties = pictest.buffers_per_channel;

    pictest_setup_dma( &pictest );

    dbg1( "PIC WDMA %d channel margin test of rows=%d bytes_per_row=%d total_bytes=%d\n",
                pictest.num_channels,
                pictest.total_rows, pictest.bytes_per_row,
                pictest.bytes_per_row * pictest.total_rows );

    /* Need to pass a few parameters to the thread that feeds data into ADC.
     * I am responsible for freeing this, not the thread. 
     */
    msg = (scan_msg_t *)MEM_MALLOC( sizeof(scan_msg_t) ); 
    XASSERT( msg != NULL,(uint32_t)msg );
    memset( msg, 0, sizeof(scan_msg_t) );

    /* tell our feeder task the amount of data to send */
    msg->param1 = pictest.total_rows;
#ifdef HAVE_ICE_LITE
    /* davep 14-Feb-2008 ; ICE Lite maps three color channels back onto one PIC
     * DMA channel so need to compensate for the stupid total_rows*=3 above
     */
    if( cmode==SCAN_CMODE_COLOR ) {
        msg->param1 = pictest.total_rows/3;
    }
#endif
    msg->param2 = pixels_per_row_in;
    msg->param3 = (void *)cmode; 
    
    /* create feeder thread suspended; run() will resume */
    /* davep 15-Nov-2007 ; have to use pictest_magin_feeder() instead of
     * my more generic pictest_rgb_feeder() because pictest_margin_validate()
     * expects a very specific data pattern 
     */
    pictest_make_thread( pictest_margin_feeder, msg );

    /* data results will land here */
    ATInitList( &results_buffers );

    /* davep 12-Nov-2007 ; XXX temp debug */
//    pic_wdma_channel_dump( pictest.channels[0]  );
//    pic_wdma_channel_dump( pictest.channels[1]  );
//    pic_wdma_channel_dump( pictest.channels[2]  );

    /* davep 14-Feb-2008 ; XXX temp debug */
//    pic_dump();
//    pic_wdma_debug_log();

    /* fire in the hole! */
    pic_interrupt_enable();

//    /* temporary ; dump the current channels' status info */
//    for( i=0 ; i<pictest.num_channels ; i++ ) {
//        pic_wdma_channel_dump( pictest.channels[i] );
//    }
    
    /* run the feeder thread */
    pictest_run_it( pictest_task_id, &results_buffers, 
            pictest.channels, pictest.num_channels, pictest.total_rows );

//    /* temporary ; dump the current channels' status info */
//    for( i=0 ; i<pictest.num_channels ; i++ ) {
//        pic_wdma_channel_dump( pictest.channels[i] );
//    }

    pictest_wait_for_feeder_complete();

    /* we're now done; prepare for validation and cleanup */
    pic_interrupt_disable();
    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_disable( pictest.channels[i] );
    }

    /* make sure our data is what we expect; also puts everything back on the
     * ready list so we can count our buffers and find any memory leaks 
     */
    pictest_margin_validate( &results_buffers, left_margin, right_margin );

   /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();

    /* count PIC write */
    verify_pic_buffer_counts( &pictest );

    /* verify everything is correct then free up allocated memory */
    pic_wdma_close( );

//    /* davep 24-Jan-2008 ; XXX temp debug */
//    pic_dump();

    pic_cleanup_after_test();

    dbg1( "%s done\n", __FUNCTION__ );
}

static void
pictest_margin( void )
{
    int i;
    scan_cmode_t cmode;

    dbg2( "%s\n", __FUNCTION__ );

    cmode = SCAN_CMODE_MONO;
    for( i=0 ; i<2 ; i++ ) {
        run_margin_test( cmode, 1712, 0, 0 );
        run_margin_test( cmode, 1712, 12, 12 );
//        run_margin_test( cmode, 5376, 0, 240 );
//        run_margin_test( cmode, 5376, 240, 0 );
        run_margin_test( cmode, 10272, 137, 99 );
        cmode = SCAN_CMODE_COLOR;
    }

    dbg2( "%s done\n", __FUNCTION__ );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

static void
run_hscale_test( scan_cmode_t cmode, uint32_t pixels_per_row_in, uint32_t pixels_per_row_out )
{
    scan_msg_t *msg;
    ATLISTENTRY results_buffers;
    int i, cnt;
    struct pic_dma_test pictest;
    uint32_t addr_status, cnt_status;
    uint32_t pic_hscale_factor;

    pic_interrupt_disable();

    pic_setup_for_test();

    /* davep 13-Apr-2008 ; adding support for 16/32-byte bursts. Need to
     * truncate down the requested pixels_per_row_out to the DMA align. I should
     * truncate the bytes_per_row but let's be lazy and just hit pixels_per_row.
     * Makes the later code easier.
     */
    if( (pixels_per_row_out & ~ICE_DMA_BYTE_ALIGN_MASK) != 0 ){
        dbg2( "%s truncating pixels_per_row_out from %d to %d for DMA align\n", 
                    __FUNCTION__, pixels_per_row_out, 
                    pixels_per_row_out & ICE_DMA_BYTE_ALIGN_MASK );
        pixels_per_row_out &= ICE_DMA_BYTE_ALIGN_MASK;
    }


    /* adjust this counter depending on how the hscale is set up; DMA will
     * expect to receive pixels_per_row_out
     */
    pic_hscale_factor = pic_hs_calc_factor( pixels_per_row_in,
                                pixels_per_row_out );

    dbg2( "%s start ppr_in=%d ppr_out=%d hscale=%d\n", __FUNCTION__, 
                pixels_per_row_in, pixels_per_row_out, pic_hscale_factor );

    /* Setup HScale;
     *
     * First, set bypass to disable block 
     */
    pic_hs_set_bypass( true );

    pic_hs_set_hscale_factor( pic_hscale_factor );
    
    /* clear bypass to enable hscale block */
    pic_hs_set_bypass( false );

    /* 600 rows, 1712 pixels at 2-bytes per pixel.
     *
     * Should get 1800 rows out (600 total rows for each of our three color
     * channels we're testing.
     */

    /* set up our DMA for a data run */
    memset( &pictest, 0, sizeof(struct pic_dma_test));
    pictest_choose_channels( cmode, &pictest );

//    pictest.total_rows = 600;
    pictest.total_rows = 30; 
//    pictest.total_rows = 5; 
    /* XXX - hardwired to 16bpp */
    pictest.bytes_per_row = pixels_per_row_out * 2;
    pictest.num_empties = pictest.total_rows; 
    if( cmode==SCAN_CMODE_MONO ) {
        pictest.buffers_per_channel = pictest.total_rows;
    }
    else {
        pictest.buffers_per_channel = pictest.total_rows / 3;
    }
    pictest.rows_per_buffer = 5;

    pictest_setup_dma( &pictest );

    dbg1( "PIC WDMA %d channel hscale test of rows=%d bytes_per_row=%d total_bytes=%d\n",
                pictest.num_channels,
                pictest.total_rows, pictest.bytes_per_row,
                pictest.bytes_per_row * pictest.total_rows );

    /* Need to pass a few parameters to the thread that feeds data into ADC.
     * I am responsible for freeing this, not the thread. 
     */
    msg = (scan_msg_t *)MEM_MALLOC( sizeof(scan_msg_t) ); 
    XASSERT( msg != NULL,(uint32_t)msg );
    memset( msg, 0, sizeof(scan_msg_t) );

    /* tell our feeder task the amount of data to send */
    msg->param1 = pictest.total_rows;
    msg->param2 = pixels_per_row_in;
    msg->param3 = (void *)cmode; /* tell pictest_rgb_feeder() to send mono or color */
//    msg->param3 = false; /* tell pictest_hscale_feeder() to not send exposure data */
    
    /* Create feeder thread suspended; run() will resume.
     * Using the margin test feeder which sends in pixels of 0,1,2,...N-1
     * where N is the pixels_per_row 
     */
    pictest_make_thread( pictest_rgb_feeder, msg );

    /* data results will land here */
    ATInitList( &results_buffers );

    /* fire in the hole! */
    pic_interrupt_enable();

    /* temporary ; dump the current channels' status info */
    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_dump( pictest.channels[i] );
    }
    
    /* run the feeder thread */
    pictest_run_it( pictest_task_id, &results_buffers, 
            pictest.channels, pictest.num_channels, pictest.total_rows );

    /* temporary ; dump the current channels' status info */
    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_dump( pictest.channels[i] );
    }

    pictest_wait_for_feeder_complete();

    /* free the message structure we passed to the thread */
    PTR_FREE( msg );

    /* we're now done; prepare for validation and cleanup */
    pic_interrupt_disable();
    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_disable( pictest.channels[i] );
    }

    /* make sure everything has finished, no remaining data anywhere which
     * would indicate an underflow
     */
    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_status( i, &cnt_status, &addr_status );
        if( cnt_status != 0 ) {
            dbg1( "DMA underflow! Channel %d transfer count status=0x%x should be 0\n", 
                        i, cnt_status );

            /* treat an underflow as a fatal condition */
            XASSERT( cnt_status==0, cnt_status );
        }
    }

    /* make sure our data is what we expect; also puts everything back on the
     * ready list so we can count our buffers and find any memory leaks 
     */
    pictest_hscale_validate( &results_buffers );


    /* everything used should be back on the appropriate lists */
    cnt = count_list( &results_buffers );
    XASSERT( cnt==0, cnt );
    pic_wdma_sanity();

    /* count PIC write */
    verify_pic_buffer_counts( &pictest );

    /* verify everything is correct then free up allocated memory */
    pic_wdma_close( );

    pic_cleanup_after_test();

    dbg1( "%s done\n", __FUNCTION__ );
}

static void
pictest_hscale( void )
{
    int i;
    scan_cmode_t cmode;

    dbg2( "%s\n", __FUNCTION__ );

    cmode = SCAN_CMODE_MONO;
    for( i=0 ; i<2 ; i++ ) {
        run_hscale_test( cmode, 5136, 5136 ); 
        run_hscale_test( cmode, 5136, 5136 ); 
        run_hscale_test( cmode, 5136, 5136 ); 
        run_hscale_test( cmode, 5136, 5136 ); 
        run_hscale_test( cmode, 5136, 5136 ); 
        run_hscale_test( cmode, 3712, 112 ); 
        run_hscale_test( cmode, 5136, 5136 ); 
        run_hscale_test( cmode, 5136, 4136 );
        run_hscale_test( cmode, 3712, 4136 ); 
        cmode = SCAN_CMODE_COLOR;
    }

    dbg2( "%s done\n", __FUNCTION__ );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

#if 0
static void
pictest_bulbmon_exposure(int pixels_per_row_in)
{
    int ponN, ponNorm;
    scan_msg_t *msg;
    ATLISTENTRY results_buffers;
    int i;
//    int i, cnt;
    struct pic_dma_test pictest;
    uint32_t num32;
    int pixels_per_row_out = pixels_per_row_in;
    scan_cmode_t cmode;

    pic_interrupt_disable();

    pic_setup_for_test();

    cmode = SCAN_CMODE_MONO;

    num32 = PIC_BM_LRMR_LEFT(0) | PIC_BM_LRMR_RIGHT( 0 );
    *PIC_BM_LRMR0 = num32;
    *PIC_BM_LRMR1 = num32;
    *PIC_BM_LRMR2 = num32;

    // Setup bulb monitor 
    num32 = PIC_BM_BMCR_CC 
          | PIC_BM_BMCR_NUML(0)  /* 1 line averaged */
          | PIC_BM_BMCR_WIN(16-1)  /* "historical value" according to BradS */
          | PIC_BM_BMCR_P(0)
          ;
    *PIC_BM_BMCR = num32;

    //  First, set bypass to disable the bulb monitor/margins block 
    //REG_SET_BITS( PIC_BM_BMCR, PIC_BM_BMCR_BYPASS );

    /* Calculate values for the PONCR register for the specified number of pixels.
     *(These formulas came from the Pixel Correction Block MA doc.)
     */
    ponN = log2(pixels_per_row_in);
    ponNorm = ceil(pow(2,13+ponN)/pixels_per_row_in);
    dbg1( "ponN = %d, ponNorm = %d\n",ponN,ponNorm );
    
    /* set up the PONCR reg */
    *PIC_PC_PONCR = (uint32_t)(PIC_PC_PONCR_PON_NORM(ponNorm) | PIC_PC_PONCR_PON_N(ponN));

    // Clear the bypass bit to enable the bulb monitor/margins block.
    *PIC_BM_BMCR &= ~PIC_BM_BMCR_BYPASS;

    /* set up our DMA for a data run */
    memset( &pictest, 0, sizeof(struct pic_dma_test));
    pictest.channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
    pictest.channels[1] = PIC_DMA_CHANNEL_CEVEN_1;
    pictest.channels[2] = PIC_DMA_CHANNEL_CEVEN_2;
    pictest.num_channels = 3;
    pictest.total_rows = 75;
    /* XXX - hardwiring to 16bpp */
    pictest.bytes_per_row = pixels_per_row_out * 2;
    pictest.num_empties = pictest.total_rows; 
    pictest.buffers_per_channel = pictest.total_rows / 3;
    pictest.rows_per_buffer = 5;
    //pictest.rows_per_buffer = 2;

    pictest_setup_dma( &pictest );

    dbg1( "PIC WDMA %d channel margin test of rows=%d bytes_per_row=%d total_bytes=%d\n",
                pictest.num_channels, 
                pictest.total_rows, pictest.bytes_per_row,
                pictest.bytes_per_row * pictest.total_rows );

    /* Need to pass a few parameters to the thread that feeds data into ADC.
     * I am responsible for freeing this, not the thread. 
     */
    msg = (scan_msg_t *)MEM_MALLOC( sizeof(scan_msg_t) ); 
    XASSERT( msg != NULL,(uint32_t)msg );
    memset( msg, 0, sizeof(scan_msg_t) );

    /* tell our feeder task the amount of data to send */
    msg->param1 = pictest.total_rows;
    msg->param2 = pixels_per_row_in;
    msg->param3 = (uint32_t *)cmode;
    
    /* create feeder thread suspended; run() will resume */
    pictest_make_thread( pictest_margin_feeder, msg );

    /* data results will land here */
    ATInitList( &results_buffers );

    /* fire in the hole! */
    pic_interrupt_enable();

    /* run the feeder thread */
    pictest_run_it( pictest_task_id, &results_buffers, 
            pictest.channels, pictest.num_channels, pictest.total_rows );

    pictest_wait_for_feeder_complete();

    /* we're now done; prepare for validation and cleanup */
    pic_interrupt_disable();

    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_disable( pictest.channels[i] );
    }

    /* 
     * Put everything back on the ready list so we can count our buffers and find any memory leaks 
     */
    pictest_dma_cleanup( &results_buffers );

    // Check the bulb monitor exposure value
    pictest_bulbmon_exposure_validate();

   /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();

    /* count PIC write */
    verify_pic_buffer_counts( &pictest );

    /* verify everything is correct then free up allocated memory */
    pic_wdma_close( );

    pic_cleanup_after_test();

    dbg1( "%s done\n", __FUNCTION__ );
}
#endif

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

#if 0
static void
pictest_add_gain(int pixels_per_row_in)
{
    /* davep 01-Jul-2008 ; dig through the Perforce archives to find the body of
     * this function.  Sorry.
     */
}

#endif

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

static void 
run_bdr_test( scan_cmode_t cmode, int pixels_per_row )
{
    scan_msg_t *msg;
    ATLISTENTRY results_buffers;
    int i, cnt;
    struct pic_dma_test pictest;
//    volatile uint32_t num32;
//    int pixels_per_row_out = pixels_per_row_in;
    volatile uint32_t *bdr_lut;

    dbg2( "%s cmode=%d ppr=%d\n", __FUNCTION__, cmode, pixels_per_row );

    pic_interrupt_disable();

    pic_setup_for_test();

    /* davep 28-Feb-2008 ; cheap hack; round up the pixels to proper DMA aligned */
    pixels_per_row = ICE_DMA_ALIGN_ME(pixels_per_row);

//    pic_marg_set_left(0);
//    pic_marg_set_right( pixels_per_row-1 );
//    pic_bm_set_bypass( false );

//    pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
    pic_wdma_set_bitpack_mode( PIC_BITPACK_8BIT );

    /* set up our DMA for a data run */
    memset( &pictest, 0, sizeof(struct pic_dma_test));
    pictest_choose_channels( cmode, &pictest );
    pictest.rows_per_buffer = 3;
    pictest.total_rows = 15;
    if( pic_wdma_get_bitpack_mode() == PIC_BITPACK_8BIT ) {
        pictest.bytes_per_row = pixels_per_row;
    }
    else {
        pictest.bytes_per_row = pixels_per_row*2;
    }
    /* setting num_empties to total_rows is overkill but will guarantee we'll
     * have all the icebufs we need for the future data buffers
     */
    pictest.num_empties = pictest.total_rows; 
    if( cmode==SCAN_CMODE_MONO ) {
        pictest.buffers_per_channel = pictest.total_rows;
    }
    else {
        pictest.buffers_per_channel = pictest.total_rows / 3;
    }

    pictest_setup_dma( &pictest );

    dbg1( "PIC WDMA channels=%d BDR test of rows=%d bytes_per_row=%d total_bytes=%d\n",
                pictest.num_channels, 
                pictest.total_rows, pictest.bytes_per_row,
                pictest.bytes_per_row * pictest.total_rows );


    /*
     * Set up the BDR and the BDR LUT
     */

    /*First enable CPU access. */
    pic_bdr_enable_fw_access( true );

    bdr_lut = PIC_BDR_LUT;
    for( cnt=0 ; cnt<PIC_BDR_LUT_SIZE-1 ; cnt++ ) {
        // linear LUT
        *bdr_lut++ = cnt/4;
    }
    /* set the last entry to max (1025 entries, remember?) */
    *bdr_lut = 0xff;

//    /* davep 23-Apr-2010 ; XXX temp debug */
//    scanlog_hex_dump( (unsigned char *)PIC_BDR_LUT, PIC_BDR_LUT_SIZE*sizeof(uint32_t) );
    
    /* verify the values we just plugged in there are really there */
    bdr_lut = PIC_BDR_LUT;
    for( cnt=0 ; cnt<PIC_BDR_LUT_SIZE-1 ; cnt++ ) {
        // linear LUT
        XASSERT( *bdr_lut==cnt/4, *bdr_lut );
        bdr_lut += 1;
    }
    /* test the last entry (1025 entries, remember?) */
    XASSERT( *bdr_lut == 0xff, *bdr_lut );

//    scanlog_hex_dump( (unsigned char *)PIC_BDR_LUT, PIC_BDR_LUT_SIZE );

    pic_bdr_enable_fw_access( false );

    /* take bdr out of bypass */
    pic_bdr_set_bypass( false );
//    pic_bdr_set_bypass( true );
    dbg2( "%s BDRCR=0x%x\n", __FUNCTION__, *PIC_BDR_BDRCR );

    /*
     * At this point, we're all set up and ready to run.
     */

    /* Need to pass a few parameters to the thread that feeds data into ADC.
     * I am responsible for freeing this, not the thread. 
     */
    msg = (scan_msg_t *)MEM_MALLOC( sizeof(scan_msg_t) ); 
    XASSERT( msg != NULL,(uint32_t)msg );
    memset( msg, 0, sizeof(scan_msg_t) );

    /* tell our feeder task the amount of data to send */
    msg->param1 = pictest.total_rows;
    msg->param2 = pixels_per_row;
    msg->param3 = (void *)cmode; /* tell pictest_rgb_feeder() to send mono or color */

    /* create feeder thread suspended; run() will resume */
    pictest_make_thread( pictest_rgb_feeder, msg );

    /* data results will land here */
    ATInitList( &results_buffers );

    /* fire in the hole! */
    pic_interrupt_enable();

    /* run the feeder thread */
    pictest_run_it( pictest_task_id, &results_buffers, 
            pictest.channels, pictest.num_channels, pictest.total_rows );

    pictest_wait_for_feeder_complete();

    /* we're now done; prepare for validation and cleanup */
    pic_interrupt_disable();

    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_disable( pictest.channels[i] );
    }

    /* we don't have a function to validate anything; just hexdump the data and
     * release back to the PIC ready list
     */
    dump_and_release( pictest.num_channels, pictest.total_rows, 
                     pixels_per_row, &results_buffers );

    /* 
     * Put everything back on the ready list so we can count our buffers and
     * find any memory leaks
     */
    pictest_dma_cleanup( &results_buffers );

   /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();

    /* count PIC write */
    verify_pic_buffer_counts( &pictest );

    /* verify everything is correct then free up allocated memory */
    pic_wdma_close( );

    pic_cleanup_after_test();

    /* free any allocated memory */
    PTR_FREE( msg );

    dbg1( "%s done\n", __FUNCTION__ );
}

/**
 * \brief run PIC BDR tests 
 *
 *
 * \author David Poole
 * \date 28-Feb-2008
 *
 */

static void
pictest_bdr( void )
{
    int i;
    scan_cmode_t cmode;

    dbg2( "%s\n", __FUNCTION__ );

    cmode = SCAN_CMODE_MONO;
    for( i=0 ; i<2 ; i++ ) {
        run_bdr_test( cmode, 1712 );
        run_bdr_test( cmode, 1314 );
        run_bdr_test( cmode, 2713 );
        run_bdr_test( cmode,  300 );
        run_bdr_test( cmode,  666 );
        run_bdr_test( cmode, 5000 );
        cmode = SCAN_CMODE_COLOR;
    }

#ifdef HAVE_THREE_BDR_LUT 
    bdr_3lut_test();
#endif

    dbg2( "%s done\n", __FUNCTION__ );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

scan_err_t pic_run_tests( void )
{
    struct timespec start_time, end_time, elapsed_time;
    scan_err_t scerr;

    dbg2( "%s\n", __FUNCTION__ );

    pictest_feeder_sem_init();

    /* XXX - should only be calling this function ONCE in the entire run of the
     * system 
     */
    pic_wdma_init();

    clock_gettime( CLOCK_REALTIME, &start_time );

    /* davep 06-Oct-2010 ; add #ifdef kludge to run descriptor dma specific
     * tests 
     */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    /* davep 25-Jul-2011 ; works as of this writing */ 
//    picdma_descrip_test();
#endif

    /* davep 12-Nov-2007 ; pic_newstyle_test() verified working as of this
     * writing 
     */
    /* davep 25-Jul-2011 ; works as of this writing */ 
    /* davep 14-Feb-2013 ; works as of this writing */
    pic_newstyle_test( SCAN_CMODE_MONO );
    pic_newstyle_test( SCAN_CMODE_COLOR );

    /* davep 23-Nov-2007 ; pictest_prnudsnu() verified working as of this
     * writing 
     */
    /* davep 25-Jul-2011 ; fails in certain asics with quad and the higher
     * pmult/pscale/poffset dmult/dscale/doffset
     */
//    pictest_prnudsnu();

    /* davep 15-Nov-2007 ; pictest_margin() verified working as of this writing
     * (Note: pictest_margin() doesn't do anything with bulb monitor/exposure
     * testing even though the margins live in the Bulb Monitor block)
     * 
     */
    /* davep 25-Jul-2011 ; works as of this writing */
//    pictest_margin();

    /* davep 12-Nov-2007 ; pictest_hscale() verified working as of this writing */
    /* davep 25-Jul-2011 ; works as of this writing */ 
    /* davep 14-May-2012 ; this test disable to eliminate floating point (and
     * we don't use hscale as of this writing)
     */
//    pictest_hscale();

    /* davep 28-Feb-2008 ; pictest_bdr() verified working as of this writing */
    /* davep 25-Jul-2011 ; works as of this writing */
//    pictest_bdr();

    clock_gettime( CLOCK_REALTIME, &end_time );
    timespec_subtract( &end_time, &start_time, &elapsed_time );
    dbg1( "test time sec=%ld nsec=%ld\n", elapsed_time.tv_sec,
                  elapsed_time.tv_nsec );

    /* davep 28-Feb-2008 ; be polite and put PIC and Cal back the way we found
     * it (sort of) 
     *
     * I'm being lazy and asserting on the return code (shame!)
     */
    scerr = pic_soft_setup();
    XASSERT( scerr==0, scerr );

    scerr = cal_onetime_init();
    XASSERT( scerr==0, scerr );

    dbg1( "%s done!\n", __FUNCTION__ );
    return SCANERR_NONE;
}

void pictest_unused( void )
{
    /* don't call this function */
    XASSERT(0,0);
    while(1) {}

    /* call unused functions here to shut up the *&@#$@#%@!!! warnings */
    pictest_hscale(); 
//    pictest_bulbmon_exposure(0);
    pictest_bdr();
    pictest_margin();
//    pictest_add_gain(0);
    pictest_bdr_feeder(0);
    pictest_bdr_validate(NULL);
}

