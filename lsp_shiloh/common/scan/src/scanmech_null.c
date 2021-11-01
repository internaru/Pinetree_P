/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/*
 * "Null" mech driver. Interface to scan core code so we can scan
 * without either having a motor or moving a motor.
 *
 * davep 23-Sep-2011 ; spun scanmech_null into scanmech_null
 *
 */

/** \file scanmech_null.c
 *
 * \brief Simple implementation of mech driver.
 *
 * This file can be used as is, in the scan environment.  It emulates a simple
 * mech for the scan code, setting up internal line syncs, but not actually
 * moving any motor.  It can be used for debugging scan code until a real motor
 * driver is available.  Also, it can be used as a starting point in the
 * creation of a motor/mech driver to implement the scanmech.h API.
 *
 * The core scan code calls:
 *     prepare -> setup -> run -> stop -> done
 *
 * Stop is usually optional. 
 *
 *     prepare - set the stage for the scan; for example:
 *                  - move the sensor to an appropriate location
 *                  or
 *                  - adf will pull paper to TOF
 *                  or
 *                  - ???
 *              Whatever is necessary to put the mech into a state where we are
 *              ready to do an image capture.
 *     setup - set up for a scan: initialize hardware, allocate memory, etc. 
 *     run - run! start moving paper (adf) or start moving sensor (flatbed).
 *             Whatever the mech is, start doing it.
 *     stop - stop the motor. 
 *     done - done! clean up. Rewind to home (flatbed). Eject page (adf).
 *             Whatever.
 *
 *
 * Weird Stuff.
 *
 * This architecture isn't perfect. We have some *ahem* h/a/c/k/s. Top of the
 * weird list is scan_is_motor_motion_enabled().
 *
 * If scan_is_motor_motion_enabled() is false, we scan without moving.
 * (For example, Calibration needs to scan without moving.)
 *
 * The 'motor motion enabled' weirdness allows us to use all the exact same
 * calls to capture data for calibration as we use for a regular scan. 
 *
 * scan_is_motor_motion_enabled() is due for a redesign and subsequent
 * demolition.
 */

#include <string.h>

#include "scos.h"

#include "lassert.h" 

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanhwerr.h"
#include "scan.h"
#include "scanhw.h"
#include "scanmech.h"
#include "scanmech_null.h"
#include "scancmdq.h"
#include "scanalyzer.h"
#include "scanif.h"

#define MOT_CTRL_SYNCS MOT_DC

typedef enum {
    SNULL_STATE_IDLE,
    SNULL_STATE_SETUP,
    SNULL_STATE_RUNNING,
    SNULL_STATE_PAUSED
} snull_state;

/* Think C++ and private member fields. But we don't have C++ so fake it. */
struct scanmech_null_members {

    bool is_open;

    t_ScanMech *null_scan_mech;

    snull_state state;

    scan_target_type_t scan_target_type;
};

static struct scanmech_null_members scanmech_self;

static void current_init( void )
{
    /* "constructor */
    memset( &scanmech_self, 0, sizeof(struct scanmech_null_members) );
}

static void current_cleanup( void )
{
    /* "destructor" */
    memset( &scanmech_self, 0, sizeof(struct scanmech_null_members) );
}

/**
 * \brief The Scan subsystem is telling the motor to do any preparations needed - we'll
 * be starting a scan soon. Typically this means "move to the right position." Also, this is
 * the only place where we are told what kind of scan is coming (look at scan_target_type), so
 * we need to save that for subsequent calls (setup, run, done).
 **/
scan_err_t smech_null_page_prepare(t_ScanMech* scan_motor, scan_target_type_t scan_target_type )
{
    dbg2( "%s\n", __FUNCTION__ );

    ASSERT( !scanmech_self.is_open );

    scanmech_self.scan_target_type = scan_target_type;
    scanmech_self.is_open = true;

    switch( scan_target_type ) {
        case SCAN_TARGET_TYPE_CAL : 
            // Here would be a command to the motor to move to the calibration
            // strip
            break;

        case SCAN_TARGET_TYPE_DOC :
            // Here would be a proper location to compute how far to move to
            // location y_hinch, and command the motor to move to that location
            // (leaving potential ramp up distance for the motor to reach speed
            // first)  
            break;

        default : 
            return SCANERR_NOT_IMPLEMENTED;
    }

    // depending on how you talk to you motor, perhaps here you would put code
    // to check to make sure the motor has stopped moving before changing your
    // mech status to ready.
    smech_set_status(SCAN_MECH_READY);

    return SCANERR_NONE;
}

/*
 * \brief Setup system for the scan
 *
 * Grab motor motion parameters, set everything up except the actual "run" bit.
 */
void smech_null_scan_setup( t_ScanMech *scan_mech )
{
    dbg2( "%s\n", __FUNCTION__ );

    // a null mech driver really has no need to know the length of a scan, etc,
    // but this is for the edification of Mech API users.  

//    if (scan_is_motor_motion_enabled())
//    {
//        // In a real Mech module (one that really drives a motor), checking for motor
//        // motion enabled is the correct thing to do, and setting the sync control based
//        // on that setting, as seen here, is to be done at this point in the code
//        // (using the appropriate scif_motor_setup() function call below).
//
//        // but since this is just an example, and we really are a null mech driver, we
//        // always use internally generated line syncs, because there is no motor
//        scif_motor_setup(MOTOR1, SYNC_DISABLE, MOT_CTRL_SYNCS, MOT_CTRL_SYNCS);
//    }
//    else
//    {
        // The scan code doesn't want motor motion at this time, so only
        // use internally generated line syncs
        scif_motor_setup(MOTOR1, SYNC_DISABLE, MOT_CTRL_SYNCS, MOT_CTRL_SYNCS);
//    }

    scanmech_self.state = SNULL_STATE_SETUP;
}


/**
 * \brief start the scan
 *
 * Usually this part is fairly simple - we just make the motor start moving. Make
 * sure that start pulses are a result of that move!
 **/
void smech_null_run(t_ScanMech* scan_motor) 
{
    dbg2( "%s\n", __FUNCTION__ );

    ASSERT( scanmech_self.is_open );
    scanmech_self.state = SNULL_STATE_RUNNING;

//    if( !scan_is_motor_motion_enabled()) {
//        smech_set_status( SCAN_MECH_RUNNING );
//        return;
//    }

    // functions will be put here to calculate how far and how fast to command
    // the motor to move based on the dpi, cmode, scan_distance

    switch( scanmech_self.scan_target_type ) {
        case SCAN_TARGET_TYPE_CAL : 
            break;

        case SCAN_TARGET_TYPE_DOC :
            break;

        /* TODO add other document types */

        default : 
            XASSERT( 0, scanmech_self.scan_target_type );
            break;
    }

    smech_set_status( SCAN_MECH_RUNNING );
}

/**
 * \brief The scan subsystem says the scan is done - do any cleanup needed
 *
 *
 **/
scan_err_t smech_null_page_done(t_ScanMech* scan_motor, smech_done_flags_t flags )
{
    dbg2( "%s flags=%d\n", __FUNCTION__, flags );

    ASSERT( scanmech_self.is_open );

    if( flags==SMECH_DONE_FLAG_CANCEL )  {
        /* Canceling. Do whatever is necessary to cancel the scan.
         *
         * For example, eject page from ADF if scan_target_type is
         * SCAN_TARGET_TYPE_DOC. 
         */
    }

//    if (scan_is_motor_motion_enabled())
//    {
        switch( scanmech_self.scan_target_type ) {
            case SCAN_TARGET_TYPE_CAL:
                /* We did a moving scan during calibration. Return to start of cal strip.
                 *
                 * Cal does several scans. The first few are stationary. The
                 * last few are with movement. When a moving scan is done, we
                 * need to move back to the start of the cal strip. Or the
                 * sensor will "walk" down the bed until we are no longer under
                 * the cal strip.
                 */
                break;

            case SCAN_TARGET_TYPE_DOC :
                /* For example, flatbed will return to home position. ADF will
                 * eject the sheet.
                 */
                break;

            default :
                return SCANERR_NOT_IMPLEMENTED;
        }
//    }

    /* call the "destructor" */
    current_cleanup();

    scanmech_self.state = SNULL_STATE_IDLE;
    smech_set_status(SCAN_MECH_READY);

    ASSERT( !scanmech_self.is_open );
    
    return SCANERR_NONE;
}

/**
 * \brief do any power on self tests
 *
 *
 **/

scan_err_t smech_null_selftest(t_ScanMech* scan_motor)
{
    dbg2( "%s\n", __FUNCTION__ );

#if 0
    /* fake a hardware error to test scantask/scanman failure interaction */
    scan_set_hwerr( ScannerHWError_MotorProblem );
    return SCANERR_HARDWARE_FAIL;
#endif

#if 0
    /* fake a slow startup device so can keep scan offline for a while */
    posix_sleep_seconds( 20 );
#endif

    return SCANERR_NONE;
}

/**
     * \brief Blocking Stop
 *
 * Usuallly called when a scan is cancelled.
 *
 **/

void smech_null_blocking_stop(t_ScanMech* scan_motor)
{
    dbg2( "%s\n", __FUNCTION__ );

    /* Called to cancel a scan. We have nothing really to cancel so simply
     * report ready.
     */

    ASSERT( scanmech_self.is_open );

#if 0
    /* fake a slow cancel so can test the core code */
    posix_sleep_seconds( 20 );
#endif

    scanmech_self.state = SNULL_STATE_IDLE;
    smech_set_status(SCAN_MECH_READY);
}

/**
 * \brief Stop motors, the scan is complete.
 *  
 *  Note that this function is called before the scan_page_done_fcn()
 *
 **/

void smech_null_async_stop(t_ScanMech* scan_motor)
{
    /* 
     * BIG FAT NOTE! THIS FUNCTION CAN BE CALLED IN INTERRUPT CONTEXT! 
     */

    ASSERT( scanmech_self.is_open );

    /* Called to let us know that the scan completed, so we can stop the motor now. */
    
    // If this were really a full Mech module that ran a motor, here is where
    // you would command the motor to begin the stopping process.  Then you
    // would NOT do smech_set_status(SCAN_MECH_READY), because you would not be
    // ready - the motor is still running.  Once the motor driver indicated to
    // this Mech driver that it was done, then would be the time to set the
    // status to ready.

    // But this really is a null mech driver, so we have nothing really to do
    // to stop the motor so simply report ready.

    scanmech_self.state = SNULL_STATE_IDLE;
    smech_set_status(SCAN_MECH_READY);
}

/**
 * \brief Start the pause.
 *
 * Usually simple enough to remove PWM from the motor (servo). 
 *
 * This function is asychronous. Should do whatever necessary to begin the
 * motor pause then return. When the pause is complete (interrupt context?),
 * call smech_set_status(SCAN_MECH_READY).
 *
 **/

void smech_null_start_pause(t_ScanMech* scan_motor) 
{
    ASSERT( scanmech_self.is_open );

//    dbg2( "%s\n", __FUNCTION__ );

    // If needed, this is the location at which you would store your
    // scanmech_self motor location, perhaps into something like the
    // "scanmech_self" structure, in preparation for moving back to that
    // location after the motor has stopped.
    
    scanmech_self.state = SNULL_STATE_PAUSED;

    // For a real mech/motor driver, here is where you would command the motor
    // to begin stopping, and then you'd immediately return from this API call
    // *without* changing the mech status to ready.  You don't want to change
    // the mech status to ready until the motor is in the appropriate location
    // to restart the scan after the pause (resume), and completely stopped.

    // But we are just the null mech/motor driver, and we don't have a motor
    // to tell us when we are stopped, so we just set our state to ready
    smech_set_status(SCAN_MECH_READY);
}

/**
 * \brief Immediately stop all motion. 
 *
 * Called in an error condition. Must immediately stop the motors.
 *
 * Usually called from the assert fail handler.
 *
 **/

void smech_null_emergency_halt(t_ScanMech* scan_motor)
{
    /* 
     * BIG FAT NOTE! THIS FUNCTION CAN BE CALLED IN INTERRUPT CONTEXT! 
     */

    /* Emergency halt!!! Something has gone very wrong. Stop everything
     * immediately.
     */
}

/**
 * \brief Run a notchfind. 
 *
 * ADF only systems won't need this function.
 *
 **/

scan_err_t smech_null_notchfind(t_ScanMech* scan_motor)
{
    dbg2( "%s\n", __FUNCTION__ );

    return SCANERR_NONE;
}

/**
 * \brief General pupose 'poll' function. 
 *
 * When enabled, this method will be called every 'N' seconds. 
 *
 * Originally created for a product that used a mechanical paper present (aka
 * "doc loaded") sensor. The image sensor had to scan for the mechanical switch
 * position.
 *  
 **/

scan_err_t smech_null_poll(t_ScanMech* scan_motor)
{
    dbg2( "%s\n", __FUNCTION__ );

    return SCANERR_NONE;    
}


/**
 * \brief Called by the core code when a SMSG_SCAN_MECH_FAULT message is
 * received.
 *
 * This function and SMSG_SCAN_MECH_FAULT is only used to communicate from
 * the mech driver's interrupt context and thread context.
 *
 * mech driver                      scanpipe
 * -----------                      --------
 *  isr: detect fault!
 *  send SMSG_SCAN_MECH_FAULT --->
 *
 *                                  thread: message loop gets SMSG_SCAN_MECH_FAULT  
 *              <----------------   call smech_fault_msg()
 *  thread: handle fault
 *
 *
 * Core code has no idea what's in the message or what it means. It's a
 * message from the mech driver to the mech driver. Passed directly down to the
 * mech driver.
 *  
 **/

scan_err_t smech_null_fault_msg(t_ScanMech* scan_motor, scan_msg_t *msg)
{
    dbg2( "%s\n", __FUNCTION__ );

    return SCANERR_NONE;    
}


/**
 * \brief Called by the core code into the mech driver to force a fault.
 *
 * For example, this function will be called if the core code decides there is
 * a paper jam.
 *  
 **/

scan_err_t smech_null_force_failure(t_ScanMech* scan_motor, smech_failure_t fail_code)
{
    dbg2( "%s\n", __FUNCTION__ );

    return SCANERR_NONE;    
}


/**
 * \brief  Optional adjust the scan line time.
 *  
 * Do nothing and the core code will use scan_line_time_usec. 
 *
 * Modify '*mech_line_time_usec' to something larger than scan_line_time_usec
 * and the core code will use that number as the scan line time.
 *
 * A complete explanation of this function is extremely complicated. You should
 * be safe ignoring this method. 
 */

void smech_null_get_min_line_time(uint32_t scan_line_time_usec,
                                  uint32_t *mech_line_time_usec)
{
    dbg2( "%s\n", __FUNCTION__ );
}

/**
 * \brief Return true/false if the motor is overheating.
 *
 * Only used during ADF scans. When we scan ADF, we start pulling paper and
 * don't pause between pages (see the pipecutter). Consequently, the motor will
 * run for a large duty cycle. 
 *
 * If the motor is overheating, the core code will pause between pages to
 * reduce the duty cycle.
 *
 * Usual method is to track duty cycle, return true when duty cycle exceeds a
 * certain level.
 *  
 **/

bool smech_null_is_motor_overheating(t_ScanMech* scan_motor)
{

    return false;
}

/**
 * \brief called by scanplat - do any power up variable initialization
 *
 *
 **/

void smech_null_init( t_ScanMech *scan_mech )
{
    /* Notes on what values I need to be able to successfully fool scancmdq into
     * thinking I have a valid motor driver.
     *
     * Based on the minimum scancmdq.c needs to run.
     */

    dbg2( "%s\n", __FUNCTION__ );

    /* start our member variables in a clean state */
    current_init();

    /* pending_high_water needed by scan_cmdq_ready_for_restart() (wiped high
     * and low water from previous product; no idea if they're correct)
     */
    scan_mech->pending_high_water = 80;

    /* pending_low_water needed by scan_cmdq_isr() */
    scan_mech->pending_low_water = 50;

    /* We must setup a mech status here, otherwise the smech_set_status() call may not
     * work properly later (it only sends a message if the state changes, that won't 
     * work if the variable is uninitialized).  Set the mech state to an error state 
     * until we are really READY.
     */
    scan_mech->mech_status = SCAN_MECH_ERROR;

    /* the following are all that are mostly required by the core code to do a
     * working scan */
    scan_mech->scan_setup_fcn = smech_null_scan_setup;
    scan_mech->scan_page_prepare_fcn = smech_null_page_prepare;
    scan_mech->smot_scan_start_fcn = smech_null_run;
    scan_mech->scan_page_done_fcn = smech_null_page_done;


    /* Our null mech driver needs to implement this function. We don't know how
     * far to scan (since we don't really have a mech). So we'll scan until the
     * core code tells us to stop.
     *
     * We need this method so we can call smech_set_status(SCAN_MECH_READY)
     * when the core code tells us to stop. The core code wants to see a 
     * transition SCAN_MECH_RUNNING -> SCAN_MECH_READY. 
     *
     * If we're doing a flatbed, we'll know before we start how far to scan. So
     * we'll stop when we reach that point, call smech_set_status(SCAN_MECH_READY).
     *
     * If we're doing an ADF, we'll know when we hit the bottom of the page
     * (via the paper sensors and adf paper events). So we'll stop when we
     * reach that point, call smech_set_status(SCAN_MECH_READY).
     */
    scan_mech->smot_scan_async_stop_fcn = smech_null_async_stop;

    /* The following are optional in our little null mech driver. Turn them
     * on/off for testing.
     */
    scan_mech->smot_scan_start_pause_fcn = smech_null_start_pause;
#if 0
    scan_mech->smech_selftest_fcn = smech_null_selftest;
    scan_mech->smot_scan_halt_fcn = smech_null_emergency_halt;
    scan_mech->smech_notchfind_fcn = smech_null_notchfind;
    scan_mech->smech_poll_fcn = smech_null_poll;
    scan_mech->smech_fault_msg_fcn = smech_null_fault_msg;
    scan_mech->smech_force_failure_fcn = smech_null_force_failure;
    scan_mech->smech_get_min_line_time_fcn = smech_null_get_min_line_time;
    scan_mech->smech_is_motor_overheating_fcn = smech_null_is_motor_overheating;    
#endif
    
    /* used with cancel */
    scan_mech->smot_scan_blocking_stop_fcn = smech_null_blocking_stop;

    /* set up for line syncs */
    scif_motor_setup(MOTOR1, SYNC_DISABLE, MOT_DC, MOT_DC);

    /* save a pointer for later use */
    scanmech_self.null_scan_mech = scan_mech;

    scanmech_self.state = SNULL_STATE_IDLE;
    scanmech_self.is_open = false;

    /* tell the scan/mot layer our driver */
    scan_set_scan_mech(scan_mech);
    
    /* Tell everyone the mech is now ready */
    dbg2( "%s: setting mech status to ready\n", __FUNCTION__ );
    smech_set_status(SCAN_MECH_READY);
}

