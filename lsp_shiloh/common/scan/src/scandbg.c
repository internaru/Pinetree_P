/**
 * \file scandbg.c
 *
 * \brief Debug/Test functions for scan firmware. 
 *
 * Mostly the callback function and the debug message handler.
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "pichw.h"
#include "pic.h"
#include "piehw.h"
#include "pie.h"
#include "scanif.h"
#include "scanvars.h"
#include "scanhwerr.h"
#include "scan.h"
#include "scanlibtest.h"
#include "cal.h"
#include "scantest.h"
#include "scantasktest.h"
#include "scanmech.h"
#include "afe.h"
#include "scantask.h"
#include "scandstest.h"
#include "demo_scanapp.h"
#include "scanvarstest.h"
#include "icefile.h"
#include "cisxtest.h"
#include "icetesttest.h"
#include "pietest.h"
#include "pictest.h"
#include "picdma_descrip_test.h"
#include "safetylock.h"

/* davep 17-Mar-2006 ; adding ICEFILE, a way to feed data into scanpipe over
 * USB or TCP or whatever; need to save the size since scanvars is overridden
 * by copyapp or scanapp and we want to keep a constant size based on our input
 * image. 
 */
static struct icefile_image_info g_image_info;

/* scan config bitfields */
/* davep 06-Dec-2010 ; moved scan_config from scanlib.c to scandbg.c */
static uint32_t scan_config;
//static uint32_t scan_config=SCAN_CONFIG_NOCAL;
//static uint32_t scan_config=SCAN_CONFIG_NOCAL|SCAN_CONFIG_FAKESCAN;
//static uint32_t scan_config=SCAN_CONFIG_NOCAL|SCAN_CONFIG_ICEFILE|SCAN_CONFIG_NO_CISX;
//static uint32_t scan_config=SCAN_CONFIG_NOPIE|SCAN_CONFIG_NO_PSENSE|SCAN_CONFIG_NO_ESD;
//static uint32_t scan_config=SCAN_CONFIG_NOCAL|SCAN_CONFIG_NOPIE|SCAN_CONFIG_NO_PSENSE|SCAN_CONFIG_NO_ESD;
//static uint32_t scan_config=SCAN_CONFIG_NOCAL|SCAN_CONFIG_ICEFILE;

/* davep 05-Apr-2012 ; adding user defined run-time initialized scandbg
 * callbacks (initially created to support the subsystem that cannot be named)
 *
 * We want hardware to run on scantask thread context. The serial port runs on
 * its own thread. We want to send "do such and such" debug messages to
 * scantask. Scantask calls scandbg. scandbg runs the debug functionality. 
 *
 * However, we can't always compile-time add code in scandbg due to
 * intellectual property concerns (i.e., scary lawyers and their scary
 * threats). 
 *
 * Solution is creating, at run-time, new scandbg messages that can be passed
 * to user defined functions.
 *
 * In the following code, assume there is proprietary hardware needing a
 * scandbg debug message. The message id for the proprietary hardware cannot be
 * compiled into scandbg.c 
 *
 * Example:
 *    my_msgid = scandbg_get_next_user_msgid();
 *    scerr = scandbg_register_user_debug( my_msgid, my_debug_callback_function );
 *
 * Function to send a debug message:
 *    scan_msg_t msg;
 *    msg.msgType = SMSG_SCAN_DEBUG;
 *    msg.param1 = my_msgid; 
 *    msg.param2 = my_param2;
 *    msg.param3 = my_param3;
 *    scerr = scantask_debug_msg_send( &msg );
 *
 */

#define MAX_USER_CALLBACK 32

struct user_callback 
{
    uint32_t id;
    scandbg_callback_fn callback_fn;
};

static struct user_callback user_callback_list[MAX_USER_CALLBACK];

static struct safety_lock scandbg_global_lock;

/* LUT to link debug/test code to message */
struct message_to_test_map {
    uint32_t msg;
    scan_err_t (*test_fcn)( void );
};

static const struct message_to_test_map msgmap[] = {
#ifdef HAVE_SCAN_TEST_CODE
    { SCAN_DBGMSG_TEST_SENSOR,      scantest_run_all_tests },
    { SCAN_DBGMSG_TEST_FINGER_TEST, scantest_run_finger_test }, 
    { SCAN_DBGMSG_TEST_SCANLIB,     scanlib2_test },
    { SCAN_DBGMSG_TEST_SCANDS,      scands_test },
    { SCAN_DBGMSG_TEST_TASK,        start_scantask_test },
    { SCAN_DBGMSG_TEST_SCANVARS,    scanvars_test },
    { SCAN_DBGMSG_TEST_CISX,        cisx_test },
    { SCAN_DBGMSG_TEST_ICETEST,     icetest_test },
    { SCAN_DBGMSG_TEST_PIE,         pie_run_tests },
//    { SCAN_DBGMSG_TEST_PIC,         picdma_descrip_test },
    { SCAN_DBGMSG_TEST_PIC,         pic_run_tests },
    /* davep 28-Jan-2013 ; comment this function out since not all AFE code has
     * it (yet)
     */
//    { SCAN_DBGMSG_TEST_AFE,         afe_test },
#endif

    /* end of list marker */
    { 0, NULL },
};

static void scandbg_lock( void )
{
    safety_lock_lock( &scandbg_global_lock );
}

static void scandbg_unlock( void )
{
    safety_lock_unlock( &scandbg_global_lock );
}

uint32_t scan_get_config( void )
{
    /* note no thread protection! */
    return scan_config;
}

void scan_set_config( uint32_t flags )
{
    scan_config = flags;
}

void scan_set_config_pipenet(bool enable )
{
    if (enable)
        scan_config |= SCAN_CONFIG_PIPENET;
    else
        scan_config &= ~SCAN_CONFIG_PIPENET;
}

void scan_set_config_nocal(bool enable)
{
    /* setting nocal will disable running calibration or using calcache */
    if (enable)
        scan_config |= SCAN_CONFIG_NOCAL;
    else
        scan_config &= ~SCAN_CONFIG_NOCAL;
}

void scan_set_config_icefile(bool enable)
{
    if (enable)
        scan_config |= SCAN_CONFIG_ICEFILE;
    else
        scan_config &= ~SCAN_CONFIG_ICEFILE;
}

void scan_set_config_copy_to_host( bool enable )
{
    /* davep 06-Dec-2006 ; copy-to-host  */
    if (enable)
        scan_config |= SCAN_CONFIG_COPY_TO_HOST;
    else
        scan_config &= ~SCAN_CONFIG_COPY_TO_HOST;
}

void scan_set_config_nopie( bool enable )
{
    /* davep 12-Dec-2006 ; disable PIE (useful for cal IQ testing) */
    if( enable ) {
        scan_config |= SCAN_CONFIG_NOPIE;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NOPIE;
    }
}

void scan_set_config_noleds( bool enable )
{
    /* davep 15-Dec-2006 ; turn off LEDs during a scan (but not durnig a cal)
     * Useful for doing dark scans to look for hardware noise on the sensor.
     */
    if( enable ) {
        scan_config |= SCAN_CONFIG_NOLEDS;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NOLEDS;
    }
}

void scan_set_config_cal_only( bool enable )
{
    /* davep 29-Jan-2007 ; as of this writing, checked in scan_setup_and_run();
     * used to stop the scan after the cal (e.g., for a cal from the command
     * line; different from cal-to-host or cal-job so we can do cals strictly
     * within scantask.
     */
    if( enable ) {
        scan_config |= SCAN_CONFIG_CAL_ONLY;
    }
    else {
        scan_config &= ~SCAN_CONFIG_CAL_ONLY;
    }
}

void scan_set_config_nobpr( bool enable )
{
    /* davep 07-Feb-2007 ; enable/disable bad pixel replacement in PIC */
    if( enable ) {
        scan_config |= SCAN_CONFIG_NOBPR;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NOBPR;
    }
}

void scan_set_config_nojobnotch( bool enable )
{
    /* davep 10-Apr-2007 ; disable notchfind after a job */
    if( enable ) {
        scan_config |= SCAN_CONFIG_NOJOBNOTCH;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NOJOBNOTCH;
    }

}

void scan_set_config_scan_from_home( bool enable ) 
{
    /* davep 13-Apr-2007 ; ignore notch and start from home position when we
     * scan
     */
    if( enable ) {
        scan_config |= SCAN_CONFIG_SCAN_FROM_HOME;
    }
    else {
        scan_config &= ~SCAN_CONFIG_SCAN_FROM_HOME;
    }
}

void scan_set_config_fakescan( bool enable )
{
    /* davep 21-Aug-2007 ; scan using fakescan_run which allocates buffers and
     * sends them to scanman as if it was scan data
     */
    if( enable ) {
        scan_config |= SCAN_CONFIG_FAKESCAN;
    }
    else {
        scan_config &= ~SCAN_CONFIG_FAKESCAN;
    }
}

void scan_set_config_no_calcache_timeout(bool enable)
{
    /* Bug 7498 ; when set, this flag prevent the calibration cache entries from
     * timing out. If cache timeouts are disabled, if we have a cached entry,
     * we'll always use it. Created to prevent calibration between pages of a
     * multipage job. (We could get parts of pages over the sensor causing
     * calibration problems.)
     */
    if (enable)
        scan_config |= SCAN_CONFIG_NO_CALCACHE_TIMEOUT;
    else
        scan_config &= ~SCAN_CONFIG_NO_CALCACHE_TIMEOUT;
}

void scan_set_config_no_pc(bool enable)
{
    /* davep 25-Apr-2008 ; disable PC (Pixel Correction); prnudsnu and bad pixel
     * replacement will be put into "bypass all"
     */
    if (enable)
        scan_config |= SCAN_CONFIG_NOPC;
    else
        scan_config &= ~SCAN_CONFIG_NOPC;
}

void scan_set_config_no_prnu(bool enable)
{
    /* davep 10-Feb-2011 ; disable PRNU only */
    if (enable) {
        scan_config |= SCAN_CONFIG_NOPRNU;
    } 
    else {
        scan_config &= ~SCAN_CONFIG_NOPRNU;
    }
}

void scan_set_config_no_dsnu(bool enable)
{
    /* davep 10-Feb-2011 ; disable DSNU only */
    if (enable) {
        scan_config |= SCAN_CONFIG_NODSNU;
    } 
    else {
        scan_config &= ~SCAN_CONFIG_NODSNU;
    }
}

void scan_set_config_no_quad(bool enable)
{
    /* ericyh 23-May-2011 ; disable QUAD only */
    if (enable) {
        scan_config |= SCAN_CONFIG_NOQUAD;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NOQUAD;
    }
}

void scan_set_config_force_pp( bool enable )
{
    /* davep 18-Dec-2008 ; when true, force PP will obey the
     * SCAN_CONFIG_FORCED_PP_POS position value (on or off)
     */
    if( enable ) {
        scan_config |= SCAN_CONFIG_FORCE_PP;
    }
    else {
        scan_config &= ~SCAN_CONFIG_FORCE_PP;
    }
}

void scan_set_config_pp_position( bool pp_enabled )
{
    if( pp_enabled ) {
        scan_config |= SCAN_CONFIG_PP_POSITION;
    }
    else {
        scan_config &= ~SCAN_CONFIG_PP_POSITION;
    }
}

void scan_set_config_pipe_drain( bool enabled )
{
    /* davep 21-Jan-2009 ;Added 'pipe_drain' for testing pause/resume.  When
     * enabled, scanpipe will not allocate new memory after sending data to
     * scanman. Eventually, we'll
     * pause. 
     */
    if( enabled ) {
        scan_config |= SCAN_CONFIG_PIPE_DRAIN;
    }
    else {
        scan_config &= ~SCAN_CONFIG_PIPE_DRAIN;
    }
}

void scan_set_config_pr_test( bool enable )
{
    /* burtp 13-Aug-2009; Special mode to cause a pause/resume rodeo */
    if( enable ) {
        scan_config |= SCAN_CONFIG_PR_TEST;
    }
    else {
        scan_config &= ~SCAN_CONFIG_PR_TEST;
    }
}

void scan_set_config_no_xmargins( bool enable ) 
{
    /* davep 27-Apr-2011 ; PIC margins will be set to max width */
    if( enable ) {
        scan_config |= SCAN_CONFIG_NO_XMARGINS;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NO_XMARGINS;
    }

}

void scan_set_config_no_esd( bool enable )
{
    if( enable ) {
        scan_config |= SCAN_CONFIG_NO_ESD;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NO_ESD;
    }
}

void scan_set_config_no_psense( bool enable )
{
    if( enable ) {
        scan_config |= SCAN_CONFIG_NO_PSENSE;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NO_PSENSE;
    }
}

void scan_set_config_no_cisx( bool enable )
{
    if( enable ) {
        scan_config |= SCAN_CONFIG_NO_CISX;
    }
    else {
        scan_config &= ~SCAN_CONFIG_NO_CISX;
    }
}

/**
 * \brief Tell icefile how to set up its icebufs
 *
 * The memory setup requirements of ICE (PIE in particular) are very picky.
 * Currently scanpipe is responsible for jumping through all the hoops to
 * calculate the strip sizes, total rows (with additional for strip size
 * padding), etc.  
 *
 * \author David Poole
 * \date 12-Nov-2010
 *
 */

scan_err_t icefile_set_image_info( struct icefile_image_info *image_info )
{
    scandbg_lock();

    memcpy( &g_image_info, image_info, sizeof(struct icefile_image_info) );

    dbg2( "%s total_rows=%d ppr=%d rpb=%d\n", __FUNCTION__, image_info->total_rows, 
            image_info->pixels_per_row, image_info->rows_per_buffer );

    scandbg_unlock();

    return SCANERR_NONE;
}

scan_err_t icefile_get_image_info( struct icefile_image_info *image_info )
{
    scandbg_lock();

    memcpy( image_info, &g_image_info, sizeof(struct icefile_image_info) );

    scandbg_unlock();

    return SCANERR_NONE;
}

/**************************************************
 * Function name   : scan_debug_dumpregs
 *    returns      : void
 *    arg1         : regs[] - array of name/address pairs of registers to dump
 *    arg2         : num_regs - number of elements in regs[]
 * Created by      : David Poole
 * Date created    : 13-Feb-05
 * Description     : 
 *
 *  Debug function to dump a block of registers along with a human friendly
 *  name.
 *
 *  Example: 
 *    SEXR=0x0 SFLQC=0x0 SLLQC=0x0 SQTGT=0x0 SFTGT=0x0 SQCNT=0x37F
 *
 * Notes           : 
 *
 *  I'm a little leery of this function because it does a blanket read of a big
 *  block of registers. I think there are a few registers (statistics counters)
 *  in the PIC RTL that will clear themselves on a read. 
 *
 **************************************************/

void scan_debug_dumpregs( struct scan_reg regs[], int num_regs )
{
    int i;

    for( i=0 ; i<num_regs ; i++ ) {
        dbg2("%8s=0x%08x", regs[i].name, *regs[i].addr); 
        if( i%4 == 3 ) {
            dbg2("\n");
        }
    }
    if( i%4 != 0 ) {
        dbg2("\n");
    }
}

/**
 * \brief  Call test code for a specific scandbg test subsystem message. 
 *
 * Moved from inside the switch() in scan_do_debug_msg() 
 *
 * \author David Poole
 * \date 24-Sep-2010
 *
 */

static scan_err_t scan_specific_subsystem_debug_msg( uint32_t param2, void *param3 )
{
#if !defined HAVE_SCAN_TEST_CODE
    /* we don't have test code :-( so sorry */
    dbg1( "%s test code not compiled into this build\n", __FUNCTION__ );
    return SCANERR_NOT_IMPLEMENTED;
#else
    scan_err_t scerr;
    const struct message_to_test_map *m;

    dbg2( "%s starting tests\n", __FUNCTION__ );

    m = msgmap;
    while( m->msg != 0 ) {
        if( m->msg==param2 ) {
            dbg2( "%s msg=%d test=%p\n", __FUNCTION__, m->msg, m->test_fcn );
            
            scerr = m->test_fcn();
            XASSERT( scerr==SCANERR_NONE, scerr );
            break;
        }
        m++;
    }
    if( m->msg==0 ) {
        /* catch unknown messages */
        dbg1("%s unknown debug message param2=%#lx param3=%p\n", 
                    __FUNCTION__, param2, param3 );
        XASSERT( 0, param2 );
    }

    dbg2( "%s complete\n", __FUNCTION__ );
    return SCANERR_NONE;
#endif
}

/**
 * \brief  Call the user debug message.
 *
 * This function scares me. We cannot control what crazy things might be done
 * in the user callback function. I am quite certain things will go badly here
 * someday. 
 *
 * \author David Poole
 * \date 05-Apr-2012
 */

static scan_err_t scandbg_call_user_debug( uint32_t param1, uint32_t param2, void *param3 )
{
    scan_err_t scerr;
    int i;
    scandbg_callback_fn fn;

    for( i=0 ; i<MAX_USER_CALLBACK ; i++ ) {
        if( user_callback_list[i].id==param1 ) {
            /* may heaven help us if this code is bad */
            fn = user_callback_list[i].callback_fn; 

            dbg2( "%s call %p 0x%x 0x%x %p\n", __FUNCTION__, 
                        fn, param1, param2, param3 );

            scerr = fn( param1, param2, param3 ); 
            
            /* found it! */
            if( scerr != SCANERR_NONE ) {
                dbg1( "%s scandbg user callback %p return scerr=%d\n", 
                            __FUNCTION__, fn, scerr );
            }

            return SCANERR_NONE;
        }
    }
    return SCANERR_NO_ENTRY;
}

/**
 * \brief Get a run-time unique scandbg message id.
 *
 * \author David Poole
 * \date 05-Apr-2012
 */

uint32_t scandbg_get_next_user_msgid( void )
{
    static uint32_t next_msgid=SCAN_DBGMSG_USER;
    uint32_t id;

    scandbg_lock();

    id = next_msgid++;

    scandbg_unlock();

    /* check for potential abuse */
    XASSERT( next_msgid < SCAN_DBGMSG_USER+100, next_msgid );

    return id;
}

/**
 * \brief register a scandbg message callback
 *
 * \author David Poole
 * \date 05-Apr-2012
 */

scan_err_t scandbg_register_user_debug( uint32_t id, scandbg_callback_fn fn )
{
    int i;

    /* id==0 reserved for an empty slot */
    ASSERT( id!=0 );

    scandbg_lock();

    /* find/claim an empty slot */
    for( i=0 ; i<MAX_USER_CALLBACK ; i++ ) {
        if( user_callback_list[i].id==0 ) {
            user_callback_list[i].id = id;
            user_callback_list[i].callback_fn = fn;
        }
    }

    scandbg_unlock();

    return SCANERR_NONE;
}

scan_err_t scandbg_onetime_init( void )
{
    safety_lock_init( &scandbg_global_lock );

    return SCANERR_NONE;
}

/**************************************************
 * Function name   : scan_do_debug_msg
 *    returns      : none
 *    arg1         : msg - pointer to an incoming message
 * Created by      : David Poole
 * Date created    : 13-Feb-05
 * Description     : 
 
 *  Parse and dispatch a scan system debug message.
 *
 * Notes           : 
 *
 *  This function doesn't use MESSAGE directly because I didn't want to create
 *  a header file dependency between MESSAGE and the rest of the scanning code
 *  that might include scandbg.h.
 *
 **************************************************/

void scan_do_debug_msg( uint32_t param1, uint32_t param2, void *param3 )
{
    scan_err_t scerr;

    scerr = 0;

    switch( param1 ) {
        case SCAN_DBGMSG_DUMP_AFE :
            afe_dump();
            break;

        case SCAN_DBGMSG_DUMP_SCAN :
            /* 7-Jun-05 davep ; updated to ScottL's new unified scan driver */
            scif_dump();
            break;

        case SCAN_DBGMSG_PIC :
            pic_debug_msg( param2, param3 );
            break;

        case SCAN_DBGMSG_IP :
            dbg1("setting debug IP address to 0x%x\n", param2 );
            scan_debug_set_ip( param2 );
            break;

        case SCAN_DBGMSG_SCAN :
            scan_specific_subsystem_debug_msg( param2, param3 );
            break;

        case SCAN_DBGMSG_PLAT_TEST :
#ifdef HAVE_SCAN_TEST_CODE            
            /* run a platform specific test - located in scanplat.c*/
            scanplat_run_test(param2, param3);
#endif            
            break;

        case SCAN_DBGMSG_NOTCH :
            /* tell the mech code to do a notchfind */
            scanmech_notchfind();
            break;

        case SCAN_DBGMSG_REPOST :
            /* tell the mech code to do redo the power on selftest */
            scanmech_selftest();
            break;

        default :
            /* catch unknown messages */
            scerr = scandbg_call_user_debug( param1, param2, param3 );
            if( scerr != SCANERR_NONE ) {
                dbg1("%s unknown debug message param1=0x%x param2=0x%x param3=%p\n", 
                       __FUNCTION__, param1, param2, param3 );
                XASSERT( 0, param2 );
            }
            break;
    }
}

