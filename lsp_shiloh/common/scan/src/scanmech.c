/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2006-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan Mech API
 *
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "regAddrs.h" 
#include "intnums.h"
#include "lassert.h" 
#include "debug.h"
//#include "interrupt_api.h" 
//#include "cpu_api.h"

#include "scancore.h"
#include "scantypes.h"
#include "scantask.h"
#include "scan.h"
#include "scandbg.h" 
#include "scanif.h"
#include "scanmech.h"
#include "scancmdq.h"
#include "scanalyzer.h"
#include "scanman.h"
#include "scanvars.h"
#include "scanlib.h"
#include "scancap.h"
#include "adfpath.h"
#include "adfgapper.h"


static t_ScanMech* scan_mech;

static bool MotorMotionEnabled = true;

#define STATE_TRANSITION(new_state)  (scanmech_state_transition( (new_state), __LINE__ ))

/* davep 03-Oct-2011 ; adding a state machine.
 *
 * Originally created to trap some race conditions where scan cmdq would finish
 * before smech_run() was even called.
 */
typedef enum {  SCANMECH_STATE_NULL=0, /* uninitialized/not used */
                SCANMECH_STATE_IDLE=1,
                SCANMECH_STATE_PREPARE=2,
                SCANMECH_STATE_SETUP=3,
                SCANMECH_STATE_RUN=4,
                SCANMECH_STATE_DONE=5,
             } scanmech_state_t; 

static scanmech_state_t scanmech_state;

/* davep 24-Oct-2012 ; adding global to capture capability of the attached mech
 * driver to handle the new cal strip overscan feature (we scan 1" over the cal
 * strip then find cal strip in the image
 */
#define CALSTRIP_FULLSCAN_NULL 0
#define CALSTRIP_FULLSCAN_SUPPORTED 1
#define CALSTRIP_FULLSCAN_NOT_SUPPORTED 2

static inline void scanmech_state_transition( scanmech_state_t new_state, int line_number )
{
    /*
     * BIG FAT NOTE!
     *
     * Can be called from interrupt context!
     *
     */

#ifdef SCANMECH_DEBUG
    if( !scos_in_interrupt() ) {
        dbg2( "%s curr=%d next=%d\n", __FUNCTION__, scanmech_state, new_state );
    }
#endif

    scanmech_state = new_state;
}

t_ScanMech* scan_get_scan_mech(void)
{
    ASSERT( scan_mech != NULL );

    return scan_mech;
}

void scan_set_scan_mech(t_ScanMech* sm)
{
    scan_mech = sm;
}

void scan_enable_motor_motion(bool enable)
{
    MotorMotionEnabled = enable;
}

void smech_set_status(t_ScanMechStatus new_status)
{
    scan_msg_t msg;
    scan_err_t scerr;

    /* BIG FAT NOTE!
     *
     * This can be called from interrupt context.  Beware!
     */
    if (scan_mech->mech_status != new_status) {
        
        /* Save our internal status */
        scan_mech->mech_status = new_status;

        /* Let scantask know what happened.  Pass the new status in the message
         * so they can see all the state transitions (they might care).
         */
        msg.msgType = SMSG_SCAN_MECH_STATUS;
        msg.param1 = (uint32_t)new_status;
        msg.param2 = 0;
        msg.param3 = 0;
        scerr = scantask_msg_send_nowait(&msg);
        XASSERT(scerr == 0, scerr);
    }
}

/**
 * \brief  An ADF paper sensor has changed.
 *
 * \param new_value the value of the sensor
 *
 * Originally created to hook to a GPIO interrupt on the tof switch.
 *
 * This function is called from the smech driver to inform the upper level scan
 * code that the ADF paper has done something interesting. 
 *
 * \author David Poole
 * \date 30-Aug-2009
 *
 */

void smech_adf_paper_event_callback( scan_adf_paper_event_t adfevt, 
                                    uint32_t value, uint32_t value2 )
{
    scan_err_t scerr;
    scan_msg_t msg;

    /* 
     * BIG FAT NOTE! 
     *
     * Can be called from interrupt context!
     *
     */

    memset( &msg, 0, sizeof(scan_msg_t) );
    msg.msgType = SMSG_SCAN_ADF_PAPER_EVENT;
    msg.param1 = (uint32_t)adfevt;
    msg.param2 = (uint32_t)value;
    msg.param3 = (void *)value2;

//    extern int you_die_now;
//    if( you_die_now ) {
//        ASSERT(0);
//    }
//    dbg2( "adf evt %d %d\n", value, value2);

    /* davep 03-Feb-2011 ; cheap hack alert! we need to update the scands
     * adf_paper_present flag but cannot do so from interrupt context. So send
     * off to ScanMan to do the dirty work for us.
     */
    if(( adfevt==SCAN_ADF_PAPER_PRESENT ) 
       || ( adfevt==SCAN_ADF_PAPER_JAM ) 
       || ( adfevt==SCAN_ADF_PAPER_NOPICK )
       || ( adfevt==SCAN_ADF_TOP_COVER_OPEN)
       || ( adfevt==SCAN_ADF_PAPER_SHORT)
	   /*SP Input Check Mode mod.kks 2014.06.12 */
	   || ( adfevt==SCAN_SP_PAPER_PRESENT )
       || ( adfevt==SCAN_SP_PAPER_INTERVAL)
       || ( adfevt==SCAN_SP_PAPER_1STSCAN)
	   || ( adfevt==SCAN_SP_PAPER_TOPCOVER )
       || ( adfevt==SCAN_SP_PAPER_HOME)
       /*SP Input Check Mode end*/
       || ( adfevt==SCAN_ADF_WAKEUP_DONE)
       || ( adfevt==SCAN_ADF_POWERSAVE_DONE)){ // Mech Power save ready done mod.kks 2014.09.15
       	   
        scerr = scantask_msg_callback(&msg);
    }
    else { 
        scerr = scantask_msg_send_nowait(&msg);
    }
    XASSERT(scerr == 0, scerr);
}

scan_err_t scanmech_selftest( void )
{
    scan_err_t retval = SCANERR_NONE;

    if( scan_mech && scan_mech->smech_selftest_fcn) {
        retval = scan_mech->smech_selftest_fcn(scan_mech);
    }
    return retval;
}

scan_err_t scanmech_prepare( scan_target_type_t scan_target_type )
{
    scan_err_t scerr;
    scan_err_t return_value = SCANERR_NONE;
    scan_err_t prepare_return_value = SCANERR_NONE;
    struct scanmech_capabilities mech_cap;
    static int supports_calstrip_fullscan=0;

    dbg2( "%s target_type=%d\n", __FUNCTION__, scan_target_type );

    /* davep 24-Oct-2012 ; add backwards compatible support for mech drivers
     * that don't support the two new cal target types (moving and stationary)
     */
    if( scan_target_type==SCAN_TARGET_TYPE_STATIONARY_CAL || 
        scan_target_type==SCAN_TARGET_TYPE_MOVING_CAL ) {

        /* add a three-state value so only have to query the ioctl once */
        if( supports_calstrip_fullscan==CALSTRIP_FULLSCAN_NULL ) {
            /* can this mech driver handle the moving/stationary target types? */
            scerr = scanmech_get_capabilities( &mech_cap );
            if( scerr==SCANERR_NOT_IMPLEMENTED) {
                /* mech driver doesn't have an ioctl */
                supports_calstrip_fullscan = CALSTRIP_FULLSCAN_NOT_SUPPORTED;
            }
            else if( !scanmech_capability_has_target( &mech_cap, scan_target_type )) {
                /* mech driver knows not these mysterious new cal types */
                supports_calstrip_fullscan = CALSTRIP_FULLSCAN_NOT_SUPPORTED;
            }
            else { 
                XASSERT( scerr==SCANERR_NONE, scerr );
                supports_calstrip_fullscan = CALSTRIP_FULLSCAN_SUPPORTED;
            }
        }

        if( supports_calstrip_fullscan==CALSTRIP_FULLSCAN_NOT_SUPPORTED ) {
            /* if caller wants a non-moving scan, we will fall back on the
             * motor motion enable/disable hack 
             */
            if( scan_target_type==SCAN_TARGET_TYPE_STATIONARY_CAL ) {
                scan_enable_motor_motion( false );
            }
            scan_target_type = SCAN_TARGET_TYPE_CAL;
            dbg2( "%s falling back to old scan_target_type=%d\n", __FUNCTION__, scan_target_type );
        }
    }

    /* davep 28-Nov-2012 ; there is a race condition between enabling the scan block
     * interrupts and starting the mech. scan_cmdq_launch() enables the scan
     * block interrupts then calls smech_run(). smech_run() calls the mech
     * driver start function. If the entire scan finishes before the mech can
     * start, scanmech_asynch_stop() will assert because our scanmech_state
     * hasn't be set to run. Classic race condition. 
     *
     * The entire scan can finish during the stationary moves in calibration.
     * We scan small area (30 rows) that is finishing before the mech driver
     * start function finishes.
     *
     * So we now fall back to the MotorMotionEnable kludge. smech_run() will
     * return immediately without calling into the (possibly slow) mech driver.
     */
    if( scan_target_type==SCAN_TARGET_TYPE_STATIONARY_CAL ) {
        scan_enable_motor_motion( false );
    }

    STATE_TRANSITION( SCANMECH_STATE_PREPARE );

    //Call the motor driver function to stage the page
    if(scan_mech && scan_mech->scan_page_prepare_fcn) {
        prepare_return_value = scan_mech->scan_page_prepare_fcn(scan_mech, scan_target_type );
    }

    //Wait for the scan mech to be ready
    return_value = scan_cmdq_wait_for_ready();

    dbg2( "%s prepret=%d ret=%d\n", __FUNCTION__, prepare_return_value, return_value );

    /* prioritize the prepare's return value above the wait for ready */
    if( prepare_return_value != 0 ) {
        return prepare_return_value;
    }

    return return_value;
}

void scanmech_setup( void )
{
    dbg2( "%s\n", __FUNCTION__ );

    STATE_TRANSITION( SCANMECH_STATE_SETUP );

    if( !MotorMotionEnabled ) {
        /* No motor motion, disable motor row sync and bail out */
        scif_motor_setup( MOTOR1, SYNC_DISABLE, MOT_DC, MOT_DC);
        return;
    }

    if(scan_mech && scan_mech->scan_setup_fcn) {
        scan_mech->scan_setup_fcn(scan_mech);
    }
}

void scanmech_run( void )
{
    /* davep 28-Nov-2012 ; There is a tough race condition between
     * scan_cmdq_launch() starting the scan block and the mech driver starting
     * the motor. There is a possibility the scan will finish before this
     * function finishes. (Think small stationary scans for calibration.) This
     * function needs to get to the smech run state quickly. 
     */

    if( !MotorMotionEnabled ) {
        STATE_TRANSITION( SCANMECH_STATE_RUN );

        dbg2( "%s\n", __FUNCTION__ );
        return;
    }

    dbg2( "%s\n", __FUNCTION__ );

    if (scan_mech && scan_mech->smot_scan_start_fcn) {
        scan_mech->smot_scan_start_fcn(scan_mech);
    }

    /* Note I'm transitioning state after calling start. I want to trap on
     * a stop function being called before start has finished.
     */
    STATE_TRANSITION( SCANMECH_STATE_RUN );
}

scan_err_t scanmech_done( smech_done_flags_t flags )
{
    scan_err_t return_value = SCANERR_NONE;
    scan_err_t done_return_value = SCANERR_NONE;

    dbg2( "%s\n", __FUNCTION__ );

    STATE_TRANSITION( SCANMECH_STATE_DONE );

    /* davep 24-Oct-2012 ; as a kludge, always enable motor motion when we're
     * done. We only turn it off when we're doing certain cal moves to be
     * backwards compatible with older mech drivers.  
     */
    scan_enable_motor_motion(true);

    if( scan_mech && scan_mech->scan_page_done_fcn) {
        done_return_value = scan_mech->scan_page_done_fcn(scan_mech, flags );
    }

    // Wait for the scan mech to be ready
    return_value = scan_cmdq_wait_for_ready();

    STATE_TRANSITION( SCANMECH_STATE_IDLE );

    dbg2( "%s done_ret=%d ret=%d\n", __FUNCTION__, done_return_value, return_value );

    /* prioritize done's return value above the wait for ready */
    if( done_return_value != SCANERR_NONE ) {
        return done_return_value;
    }

    return return_value;
}

void scanmech_blocking_stop( void )
{
    /* this function called only from threadspace */
    ASSERT( !scos_in_interrupt() );

    /* called from several places via scan_cmdq_cancel() so don't make many
     * assumptions about what state we're in
     */
    XASSERT( scanmech_state!=SCANMECH_STATE_IDLE, scanmech_state );

    if( !MotorMotionEnabled ) {
        return;
    }

    if (scan_mech && scan_mech->smot_scan_blocking_stop_fcn) {
        scan_mech->smot_scan_blocking_stop_fcn(scan_mech);
    }
}

void scanmech_async_stop( void )
{
    /* BIG FAT NOTE!
     *
     * Can be called in interrupt context! 
     *
     */

    /* This is a NON-blocking function.
     *
     * Mech is probably not stopped when the call completes.
     *
     * Might be. Might not be. Who knows? Wait for the mech status changed
     * event.
     */

    if( !MotorMotionEnabled ) {
        /* we're not moving so don't bother trying to stop */
        return;
    }

    /* verify we're running */
    XASSERT( scanmech_state==SCANMECH_STATE_RUN, scanmech_state );

    if(scan_mech && scan_mech->smot_scan_async_stop_fcn) {
        scan_mech->smot_scan_async_stop_fcn(scan_mech);
    }
}

void scanmech_emergency_halt( void )
{
    /* BIG FAT NOTE!
     *
     * Can be called in interrupt context! 
     *
     */
    if (scan_mech && scan_mech->smot_scan_halt_fcn) {
        scan_mech->smot_scan_halt_fcn(scan_mech);
    }
}

void scanmech_start_pause( void )
{
    /* BIG FAT NOTE!
     *
     * Can be called in interrupt context! 
     *
     */

    /* This is a NON-blocking function.
     *
     * Mech is probably not paused when the call completes.
     *
     * Might be. Might not be. Who knows? Wait for the mech status changed
     * event.
     */

    ASSERT( MotorMotionEnabled );

    /* verify we're running */
    XASSERT( scanmech_state==SCANMECH_STATE_RUN, scanmech_state );

    if (scan_mech && scan_mech->smot_scan_start_pause_fcn) {
        scan_mech->smot_scan_start_pause_fcn(scan_mech);
    }
}

scan_err_t scanmech_notchfind( void )
{
    scan_err_t retval = SCANERR_NONE;

    if( scan_mech && scan_mech->smech_notchfind_fcn) {
        retval = scan_mech->smech_notchfind_fcn(scan_mech);
    }
    return retval;
}


scan_err_t smech_poll( void )
{
    scan_err_t retval = SCANERR_NONE;

    if( scan_mech && scan_mech->smech_poll_fcn ) {
        retval = scan_mech->smech_poll_fcn(scan_mech);
    }

    return retval;
}

scan_err_t smech_fault_msg( scan_msg_t *msg )
{
    scan_err_t return_value = SCANERR_NONE;

    if (scan_mech && scan_mech->smech_fault_msg_fcn) {
        return_value = scan_mech->smech_fault_msg_fcn( scan_mech, msg );
    }

    return return_value;
}

scan_err_t smech_force_failure( smech_failure_t fail )
{
    scan_err_t return_value = SCANERR_NONE;

    if (scan_mech && scan_mech->smech_force_failure_fcn) {
        return_value = scan_mech->smech_force_failure_fcn( scan_mech, fail );
    }

    return return_value;
}

scan_err_t smech_sleep(bool sleep_mode)
{
    scan_err_t return_value = SCANERR_NONE;

    if (scan_mech && scan_mech->smech_sleep_fcn) {
        return_value = scan_mech->smech_sleep_fcn( scan_mech, sleep_mode );
    }

    return return_value;
}

void  smech_get_min_line_time( uint32_t scan_line_time_usec,
                               uint32_t *mech_line_time_usec )
{
    /* zero means the mech driver doesn't have a preference on scan line time */
    *mech_line_time_usec = 0;

    if (scan_mech && scan_mech->smech_get_min_line_time_fcn) {
        scan_mech->smech_get_min_line_time_fcn( scan_line_time_usec,
                                                 mech_line_time_usec );
    }
}

bool smech_is_motor_overheating( void )
{
    bool return_value = false;

    if( scan_mech && scan_mech->smech_is_motor_overheating_fcn) {
        return_value = scan_mech->smech_is_motor_overheating_fcn( scan_mech );
    }

    return return_value;
}

scan_err_t smech_adf_duplex_flip_page(void)
{
    if( !scan_mech || !scan_mech->smech_adf_duplex_flip_page_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    return scan_mech->smech_adf_duplex_flip_page_fcn( scan_mech );
}

scan_err_t smech_adf_duplex_eject(void)
{
    if( !scan_mech || !scan_mech->smech_adf_duplex_eject_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    return scan_mech->smech_adf_duplex_eject_fcn( scan_mech );
}

scan_err_t smech_adf_duplex_stage_input_sheet(void)
{
    if( !scan_mech || !scan_mech->smech_adf_duplex_stage_input_sheet_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    return scan_mech->smech_adf_duplex_stage_input_sheet_fcn( scan_mech );
}

scan_err_t smech_ioctl( uint32_t request, int arg, void *ptr )
{

#ifndef __KERNEL__
	return handle_send_ioctl(request, arg, ptr);
#else
    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    return scan_mech->smech_ioctl_fcn( scan_mech, request, arg, ptr );
#endif
}

scan_err_t scanmech_get_fb_margins( int dpi, 
                                    scan_cmode_t cmode,
                                    struct scan_flatbed_margins *fb_margins )
{

    scan_err_t scerr;

    memset( fb_margins, 0, sizeof(struct scan_flatbed_margins) );

    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    fb_margins->dpi = dpi;
    fb_margins->cmode = cmode;

    scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_GET_FLATBED_MARGINS, 
                                        sizeof(struct scan_flatbed_margins), (void *)fb_margins );
    
    return scerr;
}

scan_err_t scanmech_debug_set_fb_margins( struct scan_flatbed_margins *new_fb_margins )
{
    scan_err_t scerr;

    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_DEBUG_SET_FLATBED_MARGINS,
                                        sizeof(*new_fb_margins), (void *)new_fb_margins );

    return scerr;
}

scan_err_t scanmech_get_adf_margins( int dpi, 
                                     scan_cmode_t cmode,
                                     struct scan_adf_margins *adf_margins )
{
    scan_err_t scerr;

    memset( adf_margins, 0, sizeof(struct scan_adf_margins) );

    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    adf_margins->dpi = dpi;
    adf_margins->cmode = cmode;

    scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_GET_ADF_MARGINS, 
                                        sizeof(struct scan_adf_margins), (void *)adf_margins );
    
    return scerr;
}

scan_err_t scanmech_debug_set_adf_margins( struct scan_adf_margins *new_adf_margins )
{
    scan_err_t scerr;

    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_DEBUG_SET_ADF_MARGINS,
                                        sizeof(*new_adf_margins), (void *)new_adf_margins );

    return scerr;
}

scan_err_t scanmech_get_adf_duplex_margins( int dpi, 
                                     scan_cmode_t cmode,
                                     struct scan_adf_margins *adf_margins )
{
    scan_err_t scerr;

    memset( adf_margins, 0, sizeof(struct scan_adf_margins) );

    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    adf_margins->dpi = dpi;
    adf_margins->cmode = cmode;

    scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_GET_ADF_DUPLEX_MARGINS, 
                                        sizeof(struct scan_adf_margins), (void *)adf_margins );
    
    return scerr;
}

scan_err_t scanmech_debug_set_adf_duplex_margins( struct scan_adf_margins *new_adf_margins )
{
    scan_err_t scerr;

    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_DEBUG_SET_ADF_DUPLEX_MARGINS,
                                        sizeof(*new_adf_margins), (void *)new_adf_margins );

    return scerr;
}


scan_err_t scanmech_remove_adf_jam( void)
{
    scan_err_t scerr;

    if( !scan_mech || !scan_mech->smech_ioctl_fcn ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_PAPER_JAM_RECOVER,0,0);

    return scerr;
}
//Mechanical thing shtat should NOT block code, scanman_dev_request() is used
scan_err_t scanmech_debug_adf_pick_and_kick(void)
{
    uint32_t req_id;
    scan_err_t scerr;

    scerr = scanman_dev_request(SCANMECH_IOCTL_ADF_PICK_AND_KICK,0,0,&req_id);

    return scerr;
}

bool scanmech_capability_has_target( struct scanmech_capabilities *mech_cap,
                                     scan_target_type_t scan_target_type )
{
    int i;

    for( i=0 ; i<MAX_SUPPORTED_TARGET_TYPES ; i++ ) {
        if( mech_cap->supported_target_types[i]==SCAN_TARGET_TYPE_NONE ) {
            break;
        }
        if( mech_cap->supported_target_types[i]==scan_target_type ) {
            return true;
        }        
    }
    return false;
}

scan_err_t scanmech_get_capabilities( struct scanmech_capabilities *mech_cap )
{
    scan_err_t scerr;

    memset( mech_cap, 0, sizeof(struct scanmech_capabilities) );
    mech_cap->version = sizeof(struct scanmech_capabilities);

    scerr = smech_ioctl( SCANMECH_IOCTL_GET_MECH_CAPABILITIES, 0, mech_cap );

    return scerr;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 10-Jan-2013
 */

static scan_err_t scanmech_adf_client_pause( int flag )
{
    scan_err_t scerr;
    struct scan_platform_capabilities scan_capabilities;

    /* if we don't have an ADF, why are you here? */
    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
    	dbg1("[S_CANCEL] Error : Dont have ADF!!\n");
        return SCANERR_INVALID_PARAM;
    }

    /* if we're not running a scan, why are you here? */
    if( !scanlib_is_locked() ) {
    	dbg1("[S_CANCEL] Warning : Skip Pause!!(not scanning)!!\n");
        return SCANERR_NOT_PERMITTED;
    }

    XASSERT( flag==0 || flag==1, flag );

    /* davep 15-Jan-2013 ; cannot use SMIRB because we want this action to
     * happen immediately; SMIRB goes into scantask task queue
     */
    scerr = smech_ioctl(SCANMECH_IOCTL_ADF_CLIENT_PAUSE,flag,NULL);

    return scerr;
}

scan_err_t scanmech_adf_client_request_pause( void )
{
    return scanmech_adf_client_pause(1);
}

scan_err_t scanmech_adf_client_clear_pause( void )
{
    return scanmech_adf_client_pause(0);
}

scan_err_t scanmech_move_to_A3scanpoint( int direction )
{
    scan_err_t scerr;
    
    dbg1("scanmech_move_to_A3scanpoint called : fb_a3_move->direction : %d\n", direction);
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_MOVE_TO_A3POSITION, direction, NULL);
        
	return scerr;
}

scan_err_t scanmech_spmode_request_fb_free_run( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_FB_FREE_RUN, argument, NULL);
	return scerr;
}

scan_err_t scanmech_spmode_request_adf_free_run( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_ADF_FREE_RUN, argument, NULL);
	return scerr;
}

scan_err_t scanmech_spmode_request_feed_motor( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_FEED_MOTOR, argument, NULL);	
	return scerr;
}

scan_err_t scanmech_spmode_request_trans_motor( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_TRANS_MOTOR, argument, NULL);
	return scerr;
}

scan_err_t scanmech_spmode_request_present_sensor( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_PRESENT_SENSOR, argument, NULL);
	return scerr;
}

scan_err_t scanmech_spmode_request_interval_sensor( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_INTERVAL_SENSOR, argument, NULL);
	return scerr;
}

scan_err_t scanmech_spmode_request_firstscan_sensor( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_1STSCAN_SENSOR, argument, NULL);
	return scerr;
}

scan_err_t scanmech_spmode_request_topcover_sensor( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_TOPCOVER_SENSOR, argument, NULL);
	return scerr;
}

scan_err_t scanmech_spmode_request_home_sensor( int argument )
{
	scan_err_t scerr;	
	scerr = scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SP_HOMESENSOR, argument, NULL);
	return scerr;
}

/*sleep mode mod.kks*/
void scanmech_sleep_request(void)
{
	scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_SLEEP_MODE, 0, NULL);
}

/*wakeup mode mod.kks*/
void scanmech_wakeup_request(void)
{
	scan_mech->smech_ioctl_fcn( scan_mech, SCANMECH_IOCTL_WAKE_UP_MODE, 0, NULL);
}

void scanmech_SC_halt(void)
{
	scan_mech->smot_scan_SC_halt_fcn();  // To stop the motor and clutch in Soc SC Error   mod.kks.  2014. 06.02
}

