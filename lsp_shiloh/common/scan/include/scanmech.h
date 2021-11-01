/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2006-2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Motor API
 *
 *****************************************************************************/

/**
 * \file scanmech.h
 *
 * \brief Scan Mech API.
 *
 *  See scanmech_null.c for an example mech driver and many more notes.
 *
 *  This file defines the Scanmech object and interface. The scan code requires
 *  that a Scanmech object is created, most likely in scanplat.c. The code
 *  supports only one Scanmech object active at one time. The Scanmech object
 *  should be initialized with values for the member variables and member
 *  functions assigned. The member functions that are not assigned (NULL) will
 *  not be called.
 *
 * Scan Code <-> Motor/Mech Interface
 *
 * The scan subsystem communicates to motor code through a defined interface
 * known as the mech (mechanism) interface.  The user of the scan BSDK must
 * decide how to divide up the code to implement the interface.  It could be a
 * single file containing everything, or multiple files.  
 * Multiple files could be used to divide up motor-specific code from
 * mechanism-specific code (e.g.  a file for stepper motors and a file for a
 * flatbed mechanism, or a file for DC motors and a file for an adf mechanism).
 * Because of this flexibility, the various parts of the mechanism interface
 * may refer to names such as mech, smech, scanmech, motor, smot, and other
 * similar names.  In the current context, the BSDK user code written to
 * implement the mech interface will be referred to as the "mech code".
 *
 * t_ScanMechStatus 
 * Current status of the scan mech code.  It is the responsibility of the mech
 * code to update its current status with this enumeration (using
 * smech_set_status()).  
 * 
 * Scanmech Structure
 * struct ScanMech - contains member variables the mech code needs to
 * set, and member functions that the mech code needs to implement.
 *
 *
 * Member Variables.
 *
 * ScanMech::pending_low_water: the number of pending rows at which the scan code
 * will request a pause.  This is normally set to the number of rows the
 * mech code needs to ramp the motor down.
 *
 * ScanMech::pending_high_water: the number of pending rows at which the scan code
 * will unpause.
 *
 * ScanMech::mech_status: the current status of the mech code.  This should be set
 * using the smech_set_status() function.
 *
 *
 * Member Functions.
 *
 * This is the mech code to be written by the user of the scan BSDK.  These
 * functions need to implement the necessary commands that are specific
 * to the user's hardware to move the motor for scanning.
 *
 * All member functions defined in mech code will be called by the scan
 * code as needed.  For a normal scan, the order of the
 * function calls is:
 * 
 * - ScanMech::scan_page_prepare_fcn()
 * - ScanMech::scan_setup_fcn()
 * - ScanMech::smot_scan_start_fcn()
 * - ScanMech::scan_page_done_fcn()
 * 
 *
 * Blocking vs NON-Blocking.
 * 
 * Some functions are noted as being Blocking or NON-Blocking. Non-blocking
 * functions must return as quickly as possible. Blocking functions must finish
 * their action before returning from the function. 
 *
 * For example, scan_page_prepare_fcn is blocking. When the function returns,
 * the core code assumes the mech is fully ready for the scan. For example, a
 * prepare of a cal strip scan would stage the sensor to the cal strip then
 * return. Another example, an adf prepare would pull the first page to the top
 * of the TOF switch then return.
 *
 * In contrast, a non-blocking function *must* quickly return. The core code
 * assumes the function will start the requested action but not finish until a
 * t_ScanMechStatus message is received.
 * 
 * For example, smot_scan_start_pause_fcn is non-blocking. The core code calls
 * the function and assumes the mech will quickly return. The core code
 * continues to handle RTOS messages while the motor gearing coasts to a stop.
 * When the mech driver detects the motor has completely stopped, it sends a
 * t_ScanMechStatus message. 
 *
 * Another example, smot_scan_start_fcn is also non-blocking. The core code
 * calls start when ready to start the page scan. Core code assumes the mech
 * driver simply starts the motor moving and returns. Motor control occurs in
 * interrupt context (so far).
 *
 *
 * Pause/Resume.
 *      When the scan subsystem runs out of resources, it will need to stop the
 * scan until it can recover.  It will then tell the mech to stop moving the
 * motor (which stops the sync pulses).  
 *
 * The sequence of events is:
 * - The scan subsystem calls the mech->smot_scan_start_pause_fcn() 
 *
 * - The mech driver starts the process of pausing the motor. This must be an
 *   asynchronous process! The mech->smot_scan_start_pause_fcn() method must
 *   return immediately. 
 *
 * - Once the mech driver has detected the motor has fully stopped, the mech
 *   driver sets its status to ready via smech_set_status(). 
 *
 * - At some point, the scan subsystem recovers resources and is ready to continue.
 *   It  checks to make sure the smech status is ready, and gets back to the
 *    normal scan case by calling scan_mech->smot_scan_start_fcn()
 *
 *  Note, for more complex motor code, a motor will need to have much more work
 *  done on it, e.g.  ramped down, stopped, backed up.  In these "real motor"
 *  cases, the mech code can call additional functions, receive function calls
 *  from the motor code to wait for motor interrupts, or whatever is needed.
 *  At the point that the mech code is truly ready to restart the scan, it
 *  calls smech_set_status(), setting its status to ready, to indicate to the
 *  scan subsystem it is ready to restart now.
 *
 **/

#ifndef SCANMECH_H
#define SCANMECH_H

/** Scanmech status codes. Negative codes indicate error */
typedef enum {
    SCAN_MECH_ERROR              = -1,
    SCAN_MECH_READY              = 0,
    SCAN_MECH_RUNNING            = 1
} t_ScanMechStatus;

typedef enum {
    SMECH_DONE_FLAG_SUCCESS = 0,    ///< scan completed successfully
    SMECH_DONE_FLAG_CANCEL  = 1,    ///< cancel behavior requested
} smech_done_flags_t;

typedef enum {
    SCAN_ADF_PAPER_EVENT_NONE = 0,  ///< uninitialized, null, zip, zero, nada
    SCAN_ADF_PAPER_PRESENT = 1,     ///< paper present in ADF
    SCAN_ADF_PAPER_TOF = 2,         ///< top of form (paper in path sensor)
    SCAN_ADF_PAPER_JAM = 3,         ///< paper jammed
    SCAN_ADF_PAPER_NOPICK = 4,         ///< paper miss pick
    SCAN_ADF_TOP_COVER_OPEN = 5,         ///< ADF Top Cover Open
    SCAN_FB_PAPER_EMPTY = 6,
    SCAN_ADF_PAPER_SHORT = 7,
        
    /*SP INPUT Sensor Check mod.kks 2014.06.12 */
    SCAN_SP_PAPER_PRESENT = 8,
    SCAN_SP_PAPER_INTERVAL = 9,
    SCAN_SP_PAPER_1STSCAN = 10,
    SCAN_SP_PAPER_TOPCOVER = 11,
    SCAN_SP_PAPER_HOME = 12,
    /* SP END */
    
    SCAN_ADF_POWERSAVE_DONE = 13, // Mech Power save ready done mod.kks 2014.09.15
    SCAN_ADF_WAKEUP_DONE = 14, // Mech Power save ready done mod.kks 2014.09.24
} scan_adf_paper_event_t;

typedef enum {
    SMECH_FAILURE_NONE=0,           ///< uninitialized, null, zip, zero, nada
    SMECH_FAILURE_ADF_PAPER_JAM=1,  ///< upper level code detected paper jam   
    SMECH_FAILURE_SHORT_PAPER=2,  ///< upper level code detected paper jam   
    SMECH_FAILURE_ADF_TOPCOVER_OPEN_JAM=3, /// upper level code detected Top Cover Open  //Top Cover Open Error mod.kks 2014.06.13
    SMECH_FAILURE_ADF_NO_PICKUP_JAM=4,  ///< upper level code detected paper jam   
    /* nothing else yet */
} smech_failure_t;

/** Typedef for the ScanMech structure */
typedef struct ScanMech t_ScanMech;

/* davep 24-Oct-2012 ; adding scanmech_capabilities for core code to query
 * capabilities of driver. Created to allow scanmech core driver to gracefully
 * handle older drivers in a backwards compatible way. 
 *
 * Losely based on Win32 DrvDeviceCapabilities()
 */
#define MAX_SUPPORTED_TARGET_TYPES 16

struct scanmech_capabilities {
    /* sizeof this stucture; used with binary compatibility. Set the sizeof
     * before calling scanmech_get_capabilities(). In the driver, check the
     * size before typecasting to a local structure. In the driver, also set to
     * the size of current stuct. Both sizes must be careful to preserve binary
     * compatibility. 
     */
    uint32_t version; 

    /* list of target types (see scantypes.h) this mech driver supports.
     * Driver must zero terminate the list with SCAN_TARGET_TYPE_NONE
     */
    scan_target_type_t supported_target_types[MAX_SUPPORTED_TARGET_TYPES]; 
};

/* davep 21-Jun-2013 ; adding ioctl to force paper present/not present. Use
 * these for SCANMECH_IOCTL_FORCE_ADF_PAPER_PRESENT's arg 
 * Pass a uint32_t * for ptr and the ioctl will return the previous value.
 */
#define SCANMECH_ADF_NO_FORCE_PAPER_PRESENT   1 /* allow system to decide paper state */
#define SCANMECH_ADF_FORCE_PAPER_IS_PRESENT   2 /* force paper present */
#define SCANMECH_ADF_FORCE_PAPER_NOT_PRESENT  3 /* force paper not presenet */

/* Core code dev requests for scanman_dev_request(). Anything <65536 is reserved
 * for the core scan code. Anything >=65536 is platform/oem specific. Go nuts.
 */
/* leave 0 reserved */
#define SCANMECH_IOCTL_NULL          0
#define SCANMECH_IOCTL_SYSTEM_BASE   1
#define SCANMECH_IOCTL_MOVE_TO_HOME     			(SCANMECH_IOCTL_SYSTEM_BASE+0)
#define SCANMECH_IOCTL_MOVE_TO_CAL					(SCANMECH_IOCTL_SYSTEM_BASE+1)
#define SCANMECH_IOCTL_MOVE_TO_A3POSITION			(SCANMECH_IOCTL_SYSTEM_BASE+2)
#define SCANMECH_IOCTL_GET_FLATBED_MARGINS		(SCANMECH_IOCTL_SYSTEM_BASE+3)
#define SCANMECH_IOCTL_DEBUG_SET_FLATBED_MARGINS	(SCANMECH_IOCTL_SYSTEM_BASE+4)
#define SCANMECH_IOCTL_GET_ADF_MARGINS				(SCANMECH_IOCTL_SYSTEM_BASE+5)
#define SCANMECH_IOCTL_DEBUG_SET_ADF_MARGINS		(SCANMECH_IOCTL_SYSTEM_BASE+6)
#define SCANMECH_IOCTL_GET_ADF_DUPLEX_MARGINS		(SCANMECH_IOCTL_SYSTEM_BASE+7)
#define SCANMECH_IOCTL_DEBUG_SET_ADF_DUPLEX_MARGINS (SCANMECH_IOCTL_SYSTEM_BASE+8)
#define SCANMECH_IOCTL_ADF_PICK_AND_KICK         (SCANMECH_IOCTL_SYSTEM_BASE+9)
#define SCANMECH_IOCTL_FB_GET_NOTCH_POS          (SCANMECH_IOCTL_SYSTEM_BASE+10)
#define SCANMECH_IOCTL_ADF_EMPTY_INPUT_TRAY      (SCANMECH_IOCTL_SYSTEM_BASE+11)
#define SCANMECH_IOCTL_HARDSTOP                  (SCANMECH_IOCTL_SYSTEM_BASE+12)
#define SCANMECH_IOCTL_MOVEHOME                  (SCANMECH_IOCTL_SYSTEM_BASE+13)
#define SCANMECH_IOCTL_NOTCHFIND                 (SCANMECH_IOCTL_SYSTEM_BASE+14)
#define SCANMECH_IOCTL_GET_MECH_CAPABILITIES     (SCANMECH_IOCTL_SYSTEM_BASE+15)
#define SCANMECH_IOCTL_ADF_CLIENT_PAUSE          (SCANMECH_IOCTL_SYSTEM_BASE+16)
#define SCANMECH_IOCTL_FORCE_ADF_PAPER_PRESENT	(SCANMECH_IOCTL_SYSTEM_BASE+17)
#define SCANMECH_IOCTL_WAKE_UP_MODE				(SCANMECH_IOCTL_SYSTEM_BASE+18)
#define SCANMECH_IOCTL_MOVE_TO_HS_ON				(SCANMECH_IOCTL_SYSTEM_BASE+19)

/*SP Mode mod.kks 2014.06.13*/
#define SCANMECH_IOCTL_SP_FB_FREE_RUN				(SCANMECH_IOCTL_SYSTEM_BASE+20)
#define SCANMECH_IOCTL_SP_ADF_FREE_RUN				(SCANMECH_IOCTL_SYSTEM_BASE+21)
#define SCANMECH_IOCTL_SP_FEED_MOTOR				(SCANMECH_IOCTL_SYSTEM_BASE+22)
#define SCANMECH_IOCTL_SP_TRANS_MOTOR				(SCANMECH_IOCTL_SYSTEM_BASE+23)
#define SCANMECH_IOCTL_SP_PRESENT_SENSOR				(SCANMECH_IOCTL_SYSTEM_BASE+24)
#define SCANMECH_IOCTL_SP_INTERVAL_SENSOR				(SCANMECH_IOCTL_SYSTEM_BASE+25)
#define SCANMECH_IOCTL_SP_1STSCAN_SENSOR				(SCANMECH_IOCTL_SYSTEM_BASE+26)
#define SCANMECH_IOCTL_SP_TOPCOVER_SENSOR		(SCANMECH_IOCTL_SYSTEM_BASE+27)
#define SCANMECH_IOCTL_SP_HOMESENSOR				(SCANMECH_IOCTL_SYSTEM_BASE+28)
/* SP Mode END*/

/* Sleep Mode mod.kks 2014.08.07 */
#define SCANMECH_IOCTL_SLEEP_MODE				(SCANMECH_IOCTL_SYSTEM_BASE+29)

#define SCANMECH_IOCTL_POWER_CONTROL    (SCANMECH_IOCTL_SYSTEM_BASE+50)

/* designed for use with control panel */
#define SCANMECH_IOCTL_PAPER_JAM_RECOVER (SCANMECH_IOCTL_SYSTEM_BASE+100)//
#define SCANMECH_IOCTL_ADF_CLEAR         (SCANMECH_IOCTL_SYSTEM_BASE+101)

/* Any platform/product/oem specific ioctl should start here. Think WM_USER in
 * the Windows APIs. You have 24-bits to play with. Go nuts.
 */
#define SCANMECH_IOCTL_USER_BASE 65536


/**
 * \brief The Scanmech structure.
 *
 * This structure defines the Scanmech object. The member
 * variables and functions will be used by the scan code.
 * Member functions that are defined will be called by the scan
 * code at specific events in the scan process.
 */
struct ScanMech {

    // Member variables:

    uint16_t pending_low_water;   ///< when to pause, usually set to number of rows needed for motor ramp down.
    uint16_t pending_high_water;  ///< When to unpause.
    t_ScanMechStatus mech_status; ///< The status of the Scanmech


    // Member functions:

    /**
    * \brief <b>API FUNCTION:</b> scan setup function
    * 
    * This function will be called before we start the scan, after
    * scan_page_prepare_fcn().
    * This code allows the mech code to initialize variables or perform
    * tasks that need to be done before we start a scan.  At the
    * exit of this function, the scanner must be ready for a scan.
    *
    * Note that while general motor movement is allowed during this function
    * call, no internal scans are allowed.
    * 
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    void (*scan_setup_fcn)(t_ScanMech* scan_motor );

    /**
    * \brief <b>API FUNCTION:</b> Prepare the Scan.
    * 
    * This function will get called before the scan, and is called
    * before the scan_setup_fcn().
    *
    * The purpose is to prepare the page/mechanism for scanning in any way
    * needed, including staging the scanbar, moving the scan bar
    * over the cal strip, or moving the page (adf) quickly
    * (usually faster than scan speed) to a position to start the scan.
    * 
    * This function is also allowed to execute an internal scan
    * (e.g. notchfind, searching for tof, etc)
    * 
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    scan_err_t (*scan_page_prepare_fcn)(t_ScanMech* scan_motor, scan_target_type_t scan_target_type );

    /**
    * \brief <b>API FUNCTION:</b> Start Scan
    *
    * This function is called to start the scan. The Scanmech code can
    * then start the motors and start scanning. The scan code will
    * then wait for line start pulses from the motor block.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    void (*smot_scan_start_fcn)(t_ScanMech* scan_motor);

    /**
    * \brief <b>API FUNCTION:</b> Page Done
    *
    * After the scan, this function is called. The Scanmech can do
    * some clean up after the scan. The mech may want to move the
    * scanbar to the home position after the scan, or perform a
    * notchfind. In the case of and ADF device it may want to eject
    * the page after the scan.
    *
    * Scanmech is complete and shut down after this call.
    *
    * Note that this function is allowed to execute an internal scan such as
    * for a notchfind.
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    * \param flags - see smech_done_flags_t
    *
    * \date 14-July-2009 davep ; added smech_done_flags. Originally created for
    * ADF-based systems to be able to handle paper eject.
    */
    scan_err_t (*scan_page_done_fcn)(t_ScanMech* scan_motor, smech_done_flags_t flags );

    /**
    * \brief <b>API FUNCTION:</b> Stop Scan
    *
    * When this function is called, stop scanning and return to a
    * ready state.
    *
    * This function should block until the mech has completely finished. The
    * scan firmware assumes motor 
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    void (*smot_scan_blocking_stop_fcn)(t_ScanMech* scan_motor);

    /**
    * \brief <b>API FUNCTION:</b> Stop Scan
    *
    * Request the mech driver begin the process of stopping the scan.
    *
    * This is an asynchronous call and is called from interrupt context. Compare
    * to smot_scan_blocking_stop_fcn().
    *
    * NON-Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    void (*smot_scan_async_stop_fcn)(t_ScanMech* scan_motor);

    /**
    * \brief <b>API FUNCTION:</b> Halt Scan
    *
    * Stop the scan immediately. Do not ramp down the motor. Error
    * condition.
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    void (*smot_scan_halt_fcn)(t_ScanMech* scan_motor);       //void smot_halt(void)

    /**
    * \brief <b>API FUNCTION:</b> Start Pause
    *
    * Scanmech needs to pause the scan because of a lack of memory. Core code
    * continually checks the number of pending rows to scan with the low water
    * value. If pending rows is less than or equal to low water then Scancmdq
    * will call this function to start pausing the Scanmech.
    *
    * NON-Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    void (*smot_scan_start_pause_fcn)(t_ScanMech* scan_motor);     //void smot_scan_start_pause(void)

    /**
    * \brief <b>API FUNCTION:</b> Scanmech selftest
    *
    * Whatever is needed for selftest, like a power on self test.  Very mech specific.
    * This is where functionality such as notchfind, led and motor tests, setting home would go.
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    scan_err_t (*smech_selftest_fcn)(t_ScanMech* scan_motor);     //void smech_selftest(void)

    /**
    * \brief <b>NOT AN api function:</b> Scanmech notch find
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    scan_err_t (*smech_notchfind_fcn)(t_ScanMech* scan_motor);     //void smech_notchfind(void)

    /**
    * \brief <b>API FUNCTION:</b> Scanmech sensor polling
    *
    * Some scan mechanism use the scan sensor itself for detecting the Paper
    * Present or other scan/ADF switches. The firmware must wake up periodically
    * and check for the sensors.
    *
    * Originally created to support scan mechs with mechanical Paper Present
    * switches that needed to be polled using the image sensor.
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    scan_err_t (*smech_poll_fcn)(t_ScanMech* scan_motor);

    /**
    * \brief <b>API FUNCTION:</b> Scanmech fault function
    *
    * Allows the mech code to pass fault messages from interrupt context back
    * into thread space where error handling can be done safely.
    *
    * Example: motor stalls, ADF door/lid open events.
    *
    * Originally created so the mech driver(s) can communicate from their
    * interrupt space up to the Scantask calls in thread space.
    *
    * The contents of the message are completely opaque to the common scan code.
    *
    * Faults/failures detected in common code are sent downwards using
    * smech_force_failure_fcn().
    *
    * This is the mechanism through which "Bottom up" failures are propagated,
    * e.g., from interrupt context back up to thread context.
    *
    * \param scan_motor - pointer to the scan mech structure.
    * \param msg        - pointer the actual fault message.
    */
    scan_err_t (*smech_fault_msg_fcn)(t_ScanMech *scan_motor, scan_msg_t *msg);

    /**
    * \brief <b>API FUNCTION:</b> Top-down Scanmech failure.
    *
    * Allows the common code to indicate a mech failure down to the mech driver.
    *
    * Originally created to communicate a pipe cutter paper jam (too much input)
    * from scanpipe down to the mech driver.  (The mech driver handles paper
    * jams.)
    *
    * Can NOT be called in interrupt context!  Use SMSG_SCAN_MECH_FAULT
    *
    * Example: "soft" paper jam (ADF paper beyond a certain length).
    *
    * This is the mechanism through which "Top down" failures are propagated.
    *
    * \param scan_motor - pointer to the scan mech structure.
    * \param fail - the failure
    */
    scan_err_t (*smech_force_failure_fcn)(t_ScanMech *scan_motor,
                                          smech_failure_t fail_code );

    /**
    * \brief <b>API FUNCTION:</b> Scanmech set sleep mode
    *
    * Called to set the mech sleep mode.  Power to the scan subsystem can
    * be turned off in sleep mode, so the mech needs to suspend any periodic
    * activities (example: paper present polling).
    *
    * \param scan_motor - pointer to the scan mech structure.
    * \param sleep_mode - TRUE: enter sleep mode, FALSE: exit sleep mode
    */
    scan_err_t (*smech_sleep_fcn)(t_ScanMech *scan_motor, bool sleep_mode);

    /**
     * \brief Get the minimum scan line time desired by the scan mech.
     *
     * The mech driver may need a longer scan line time than that calculated using
     * scanx. The scan cmdq will get the line time from the mech then use the max of
     * the pixel line time and the mech line time.
     *
     * If the mech driver doesn't care about the line time, set mech_line_time_usec
     * to zero.
     *
     * The scan cmdq will call this function after the scan_setup_fcn() is called.
     *
     * Originally created to add line time on a stepper motor platform with weird
     * harmonics at certain step frequencies. We extended the line time a bit to
     * avoid those harmonics.
     *
     * \param scan_line_time_usec - the line time based on the number of pixels in
     *                              the line (SCANX)
     *
     * \param mech_line_time_usec - the line time the scan mech driver would like
     *                              (set to zero to use the scan_line_time_usec)
     *
     * \author David Poole
     * \date 22-Jun-2009
     */

    void (*smech_get_min_line_time_fcn)( uint32_t scan_line_time_usec,
                                         uint32_t *mech_line_time_usec );

	void (*smot_scan_SC_halt_fcn)(void);  // To stop the motor and clutch in Soc SC Error   mod.kks.  2014. 06.02
    /**
    * \brief <b>API FUNCTION:</b> Scanmech get motor overheat status
    *
    * Returns TRUE if the scan motor is overheating (running with too
    * high a duty cycle), FALSE otherwise.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    bool (*smech_is_motor_overheating_fcn)(t_ScanMech *scan_motor);

    /**
    * \brief <b>API FUNCTION:</b> 
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    scan_err_t (*smech_adf_duplex_flip_page_fcn)(t_ScanMech *scan_mech);

    /**
    * \brief <b>API FUNCTION:</b> 
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    scan_err_t (*smech_adf_duplex_eject_fcn)(t_ScanMech *scan_mech);

    /**
    * \brief <b>API FUNCTION:</b> 
    *
    * Blocking function.
    *
    * \param scan_motor - pointer to the scan mech structure.
    */
    scan_err_t (*smech_adf_duplex_stage_input_sheet_fcn)(t_ScanMech *scan_mech);

    /**
    * \brief <b>API FUNCTION:</b> 
    *
    * http://en.wikipedia.org/wiki/Ioctl
    *
    * \param 
    */
    scan_err_t (*smech_ioctl_fcn)( t_ScanMech *scan_mech, uint32_t request, int arg, void *ptr );
};

/**
* \brief Set the active scan mech.
*
*
* \param sm - pointer to the scan mech structure.
*/
void scan_set_scan_mech(t_ScanMech* sm);

/**
* \brief Get the active scan mech.
*
*
* \return pointer to the scan mech structure.
*/
t_ScanMech* scan_get_scan_mech(void);

/**
* \brief Enable motor motion
*
*
* \param enable - Boolean to enable or disable the motor
* movement during a scan.
*/
void scan_enable_motor_motion(bool enable);

/* davep 31-Aug-2009 ; adding adf paper event interface.
 *
 * BIG FAT NOTE!
 * This can be called from interrupt context.  Beware!
 *
 * Not part of the scanmech structure because this function is called from the
 * mech driver up to the core scantask code.
 */
void smech_adf_paper_event_callback( scan_adf_paper_event_t adfevt, 
                            uint32_t value, uint32_t value2 );

/**
 * \brief Set the mech status
 *
 * BIG FAT NOTE!
 * This can be called from interrupt context.  Beware!
 *
 * \param new_status - The new status for the mech.
 */
void smech_set_status(t_ScanMechStatus new_status);

scan_err_t scanmech_selftest(void);

void scanmech_setup( void );

void scanmech_run( void );

scan_err_t scanmech_prepare( scan_target_type_t scan_target_type );

scan_err_t scanmech_done( smech_done_flags_t flags );

void scanmech_blocking_stop( void );

void scanmech_emergency_halt( void );

void scanmech_start_pause( void );

void scanmech_async_stop( void );

scan_err_t smech_poll( void );

scan_err_t scanmech_notchfind(void);

scan_err_t smech_fault_msg( scan_msg_t *msg );

scan_err_t smech_force_failure( smech_failure_t fail_code );

scan_err_t smech_sleep(bool sleep_mode);

void smech_get_min_line_time( uint32_t scan_line_time_usec,
                              uint32_t *mech_line_time_usec );

bool smech_is_motor_overheating(void);

/* davep 08-Sep-2011 ; adding duplex */
scan_err_t smech_adf_duplex_flip_page(void);

scan_err_t smech_adf_duplex_eject(void);

scan_err_t smech_adf_duplex_stage_input_sheet(void);

/* davep 26-Sep-2011 ; adding ioctl() */
scan_err_t smech_ioctl( uint32_t request, int arg, void *ptr );

/* davep 13-Oct-2011 ; moving margins from scanplat to scanmech */
scan_err_t scanmech_get_fb_margins( int dpi, 
                                    scan_cmode_t cmode,
                                    struct scan_flatbed_margins *fb_margins );

scan_err_t scanmech_debug_set_fb_margins( struct scan_flatbed_margins *new_fb_margins );

scan_err_t scanmech_get_adf_margins( int dpi, 
                                     scan_cmode_t cmode,
                                     struct scan_adf_margins *adf_margins );

scan_err_t scanmech_debug_set_adf_margins( struct scan_adf_margins *new_adf_margins );

scan_err_t scanmech_get_adf_duplex_margins( int dpi, 
                                     scan_cmode_t cmode,
                                     struct scan_adf_margins *adf_margins );

scan_err_t scanmech_debug_set_adf_duplex_margins( struct scan_adf_margins *new_adf_margins );

scan_err_t scanmech_debug_adf_pick_and_kick(void);

scan_err_t scanmech_get_capabilities( struct scanmech_capabilities *mech_cap );

bool scanmech_capability_has_target( struct scanmech_capabilities *mech_cap,
                                     scan_target_type_t scan_target_type );

scan_err_t scanmech_adf_client_request_pause( void );
scan_err_t scanmech_adf_client_clear_pause( void );

//remove the paper jam status from UI code
scan_err_t scanmech_remove_adf_jam( void);

scan_err_t scanmech_move_to_A3scanpoint( int direction );

scan_err_t scanmech_spmode_request_fb_free_run( int argument );

scan_err_t scanmech_spmode_request_adf_free_run( int argument );

scan_err_t scanmech_spmode_request_feed_motor( int argument );

scan_err_t scanmech_spmode_request_trans_motor( int argument );

scan_err_t scanmech_spmode_request_present_sensor( int argument );

scan_err_t scanmech_spmode_request_interval_sensor( int argument );

scan_err_t scanmech_spmode_request_firstscan_sensor( int argument );

scan_err_t scanmech_spmode_request_topcover_sensor( int argument );

scan_err_t scanmech_spmode_request_home_sensor( int argument );

/* Sleep Mode mod.kks 2014.08.07*/
void scanmech_sleep_request(void);

void scanmech_wakeup_request(void);

void scanmech_SC_halt( void);

#endif /* SCANMECH_H */

