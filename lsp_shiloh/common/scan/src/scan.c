/****************************************************************************** ;
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: 
 * Author: just about everyone
 *
 *****************************************************************************/

#include <string.h>

#include "scos.h"

#include "regAddrs.h" 
#include "interrupt_api.h" 
#include "intnums.h" 
#include "lassert.h" 

#include "scancore.h" 
#include "scantypes.h"
#include "scantools.h"
#include "scandbg.h" 
#include "scanif.h" 
#include "scanhwerr.h"
#include "scan.h"
#include "scanhw.h"
#include "scanmech.h"
#include "scancmdq.h"
#include "scanplat.h"
#include "scantask.h"
#include "scanalyzer.h"
#include "scansen.h"
#include "scanlib.h"

static struct scan_interrupt_stats scanInterruptStats;

static scan_cmdq_isr_t cmdq_interrupt_function;  // Global pointer to the cmdq interrupt

/* davep 12-Nov-2012 ; hook the scan block external interrupt function */
static struct scan_external_interrupt scan_ext_int_drvr;
static bool scan_ext_int_drvr_not_null;

/* default to "scan is uninitialized" error; we'll set to 'none' once everything
 * is up and running
 */
static ScannerHWError scannerErrorCode=ScannerHWError_ScannerStarting; 

#ifdef __KERNEL__
static irqreturn_t scan_interrupt( int irq, void *dev_id ) 
#else
static void scan_interrupt( uint32_t param )
#endif
{
    unsigned int status;
    scan_msg_t msg;
    scan_err_t scerr;
    int retcode;

    /* 
     * BIT FAT NOTE: THIS IS AN INTERRUPT HANDLER.
     *
     */

    scanInterruptStats.total++;

    /* determine what interrupt we have */
    status = scif_status();

    SCANALYZER_LOG( LOG_SCAN_INTERRUPT, status );

//    dbg2( "%s status=%#lx\n", __FUNCTION__, status );

    /* write the scan interrupt ack register */
    scif_interrupt_ack(status);

    /* command queue overrun interrupt */
    if (status & SCIF_SSTAT_QOVR ) {
        scanInterruptStats.cmdQueueOverrun++;
        /* davep 09-Nov-2005 ; make sure this doesn't happen */
#if 0
        ASSERT(0);
#else
		scanlib_send_sc(SMSG_SC_CMD_QUEUE_OVR_FAIL);
#endif
    }

    /* command complete overrun interrupt */
    if(status & SCIF_SSTAT_CIOVR ) {
        scanInterruptStats.cmdCompleteOverrun++;
        /* davep 09-Nov-2005 ; make sure this doesn't happen */
#if 0
        ASSERT(0);
#else
		scanlib_send_sc(SMSG_SC_CMD_COMPLETE_OVR_FAIL);
#endif
    }

#ifdef SCIF_SSTAT_AFEPCOMP 
    /* AFE program complete interrupt */
    if(status & SCIF_SSTAT_AFEPCOMP ) {
        scanInterruptStats.afeComplete++;

        /* davep 18-Feb-2008 ; we don't use this interrupt so make sure we don't
         * hit it
         */
        ASSERT(0);
    }
#endif

    /* scan command complete interrupt */
    if(status & SCIF_SSTAT_SCMDCOMP ) {

        scanInterruptStats.cmdComplete++;

        retcode = cmdq_interrupt_function();
        switch( retcode ) {
            case SCAN_CMDQ_SUCCESS :
                /* nothing */
                break;

            case SCAN_CMDQ_DONE :
                memset(&msg, 0, sizeof(scan_msg_t));
                msg.msgType = SMSG_SCAN_SCMDCOMP; 

                /* wake up the scan task */
                scerr = scantask_msg_send_nowait(&msg);
                /* FIXME - shouldn't assert here; could fail (queue full) */
                XASSERT(scerr == 0, scerr);
                break;

            case SCAN_CMDQ_EMPTY :
                scanInterruptStats.cmdq_empties++;
                if( !(scif_status() & SCIF_SSTAT_QEMPTY) ) {
                    break;
                }
                scanInterruptStats.motor_stops++;

                /* send message to scantask telling them we stopped the motor and
                 * they're now responsible for restarting
                 */
                memset(&msg, 0, sizeof(scan_msg_t));
                msg.msgType = SMSG_SCAN_CMDQSTOP; 

                /* wake up the scan task */
                scerr = scantask_msg_send_nowait(&msg);
                /* FIXME - shouldn't assert here; could fail (queue full) */
                XASSERT(scerr == 0, scerr);
                break;

            default :
                /* in case something new comes up in the code in the future */
                XASSERT( 0, retcode );
        }
    }

    /* davep 05-Jul-2011 ; catch external interrupt source (e.g., digital
     * sensor controller) 
     */
#ifdef SCIF_SSTAT_EXTINTSTAT 
    /* davep 12-Nov-2012 ; add registration of external interrupt handler
     * instead of hardcoding a call
     */
    if( status & SCIF_SSTAT_EXTINTSTAT && scan_ext_int_drvr.isr ) {
        /* Calling a function pointer... how exciting! */
        scan_ext_int_drvr.isr();
    }
#endif

    /* davep 03-Apr-2013 ; log on ISR exit */
    SCANALYZER_LOG( LOG_SCAN_INTERRUPT, 0xffff0000 );

#ifdef __KERNEL__
    return IRQ_HANDLED;
#endif
}

/**
 * \brief function to handle Scan Command Complete interrupts.
 *
 * Disable the Scan interrupt.  Save copy of old handler.  Assign new handler to
 * global function pointer.  Re-enable Scan interrupt if it was previously
 * enabled. Return copy of old hanlder.
 * 
 *
 * \return old scan cmdq int handler
 *
 * \author David Poole
 * \date 17-Feb-2008
 *
 */

scan_cmdq_isr_t scan_set_cmdq_interrupt( scan_cmdq_isr_t isr_function ) 
{
    scan_cmdq_isr_t old_cmdq_isr;
    uint32_t was_enabled;

    was_enabled = scan_interrupt_disable();

    old_cmdq_isr = cmdq_interrupt_function;

    cmdq_interrupt_function = isr_function;

    if( was_enabled ) {
        scan_interrupt_enable();
    }

    return old_cmdq_isr;
}

/**
 * \brief  Register a handler for the external interrupt signal.
 *
 * The scan block can allow an external signal (e.g., from the digital sensor
 * control block) to raise a scan block interrupt. 
 *
 * see scan_interrupt() below.
 *
 * Not all ASICs support this feature.
 *
 * \author David Poole
 * \date 12-Nov-2012
 */

scan_err_t scan_set_external_interrupt( struct scan_external_interrupt *new_scan_ext_int )
{
    uint32_t was_enabled;

    /* Did someone else get here first? Only allowing one initializer for now */
    if( scan_ext_int_drvr_not_null ) {
        return SCANERR_ALREADY;
    }

    was_enabled = scan_interrupt_disable();

    scan_ext_int_drvr = *new_scan_ext_int;

    /* we are initialized */
    scan_ext_int_drvr_not_null = true;

    if( was_enabled ) {
        scan_interrupt_enable();
    }

    return SCANERR_NONE;
}

ScannerHWError scan_get_hwerr( void )
{
    /* note no thread/interrupt protection! */
    return scannerErrorCode;
}

void scan_set_hwerr( ScannerHWError scanner_hw_err )
{
    /* note no thread/interrupt protection! */
    scannerErrorCode = scanner_hw_err;
}

bool scanner_is_alive( void )
{
    return scannerErrorCode==ScannerHWError_None;
}

/**
 * \brief  Emergency shutdown of scan 
 *
 *   >>> BIG FAT NOTE! <<<
 *      This function is called from asserts and exceptions. Do
 *      >NOT< call anything that could possible assert itself or
 *      we'll have an infinite recursion.
 *
 *      The code here should finish as quickly as possible (i.e., NOT dprintf()
 *      calls!) so the rest of the atassert() registered functions can be called
 *      as quickly as possible.
 *   >>> BIG FAT NOTE! <<<
 *
 * \author David Poole
 * \date a long long time ago in a cubicle far far away
 *
 * davep 24-Jul-07; added function header 
 *
 */

static void scan_emergency_stop( void ) 
{
    scan_cmdq_emergency_halt();

//    /* davep 30-Sep-2010 ; XXX temp debug */
//    scan_cmdq_dump_cmd_log();

//    scif_dump();
//    cisx_dump();
//    pluto_dump();
//    scan_dump();
//    scan_wdma_channel_dump(0);
//    scan_wdma_debug_log();

//    while( 1 ) { 
//        pluto_dump();
//        posix_sleep_seconds(2);
//    }

//    scanalyzer_dump();

    /* more? */
//    scif_dump();
}

struct scan_interrupt_stats * scan_get_interrupt_stats( void )
{
    return &scanInterruptStats;
}

void scan_interrupt_stats_dump( void )
{
    dbg1( "total=%d qover=%d cover=%d afec=%d cmdc=%d cmdqe=%d mstop=%d\n",
            scanInterruptStats.total,
            scanInterruptStats.cmdQueueOverrun,
            scanInterruptStats.cmdCompleteOverrun,
            scanInterruptStats.afeComplete,
            scanInterruptStats.cmdComplete,
            scanInterruptStats.cmdq_empties,
            scanInterruptStats.motor_stops );
}

void scan_interrupt_stats_clear( void )
{
    memset( &scanInterruptStats, 0, sizeof(scanInterruptStats) );
}

uint32_t scan_interrupt_disable( void )
{
    uint32_t was_enabled;

    /* disable the scan block's interrupts */
    was_enabled = scif_interrupt_disable();

    /* if we have something driving the scan block's external AFE interrupt
     * source, disable it
     */
    if( scan_ext_int_drvr.disable ) {
        uint32_t ext_int_was_enabled;

        ext_int_was_enabled = scan_ext_int_drvr.disable();

        /* Factor the external interface into this mess. Note this will
         * eventually cause the enable/disable state to be synchronized which
         * is a good thing. Trust me.
         */
        was_enabled = was_enabled || ext_int_was_enabled;
    }

    return was_enabled;
}

void scan_interrupt_enable( void )
{
    /* enable the scan block interrupts */
    scif_interrupt_enable();

    /* if we have something driving the scan block's external AFE interrupt
     * source, enable it
     */
    if( scan_ext_int_drvr.enable ) {
        scan_ext_int_drvr.enable();
    }
}

#ifdef __KERNEL__
scan_err_t scan_capture_interrupt( void )
{
    int retcode;

    /* wait until explicitly enabled */
    scan_interrupt_disable();

    retcode = request_irq( INTNUM_SCAN, scan_interrupt, 0, "icescan", NULL );
    dbg2( "%s request_irq retcode=%d\n", __FUNCTION__, retcode );
    XASSERT(retcode==0, retcode );

    return SCANERR_NONE;
}

void scan_release_interrupt( void )
{
    scan_interrupt_disable();
    /* disable at the OS level as well */
    disable_irq( INTNUM_SCAN );
    free_irq( INTNUM_SCAN, NULL );
}
#else
scan_err_t scan_capture_interrupt( void )
{
    intAttach( INTNUM_SCAN, 1, scan_interrupt, INTNUM_SCAN );
    intEnable( INTNUM_SCAN );
    return SCANERR_NONE;
}

void scan_release_interrupt( void )
{
    scan_interrupt_disable();
    /* disable at the OS level as well */
    intDisable( INTNUM_SCAN );
    intDetach( INTNUM_SCAN );
}
#endif

scan_err_t scan_onetime_init(void) // one time cold init
{
    scan_err_t scerr;
    int retcode;

    dbg2( "%s\n", __FUNCTION__ );

    /* Lots of scan modules use SA for logging, make sure it's ready to go */
    SCANALYZER_INIT();

//    scif_reset();

    // setup SCAN interrupt but don't enable until all the data is set up
    scan_interrupt_disable();
    scerr = scan_capture_interrupt();
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* scapri 16-Jan-2008 ; gpio init must happen before init */
    scanplat_init_scan_gpio();

    scan_cmdq_onetime_init();

    // when something goes wrong, call our stop function
    retcode = atassert(scan_emergency_stop);
    XASSERT(retcode==0, retcode);

//    SCANALYZER_ENABLE_LOG(LOG_SCAN_INTERRUPT);

    return 0;
}

/**
 * \brief  default method for returning number of sensors
 *
 * Can be overridden by scansen drivers.
 *
 * (Put in scan.c because I didn't have anywhere else to put it. #sheepish)
 *
 * \author David Poole
 * \date 12-Jul-2013
 */

uint32_t __attribute__ ((weak)) scansen_get_num_sensors(void)
{
    return 1;
}

