/*
 * ============================================================================
 * Copyright (c) 2007-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanalyzer.h
 *
 * \brief Header file for Scanalyzer test/debug tool.
 *
 * A debugging tool to log events and data to memory. Created for logging scan
 * events that happen in interrupts were we cannot write to the console because
 * it would take to much time.
 *
 * It is designed to take up the least amount of memory. Raw binary data is stored
 * and can be dumped the console by command. You can use a script on a host computer
 * to decipher the logs.
 *
 * Events can be enabled and disabled. Only enabled events are added to the log.
 * This way we can save space for only the events we are interested in. They can
 * be enabled and disabled by console commands and the log can be dumped to the 
 * console.
 *
 **/

#ifndef INC_SCANALYZER_H
#define INC_SCANALYZER_H


/**
 *  \brief API Access Macros
 *
 *  All scanalyzer clients should use these macros to access the scanalyzer: if
 *  scanalyzer is bound out (not compiled and linked) then all references will
 *  also be removed.
 *
 **/
#ifdef HAVE_SCANALYZER_SUPPORT
  #define SCANALYZER_INIT() scanalyzer_init()
  #define SCANALYZER_ENABLE_LOG(a) scanalyzer_enable_log(a)
  #define SCANALYZER_DISABLE_LOG(a) scanalyzer_disable_log(a)
  #define SCANALYZER_DISABLE_ALL() scanalyzer_disable_all()
  #define SCANALYZER_LOG(a, b) scanalyzer_log(a,b)
  #define SCANALYZER_CLEAR_LOG() scanalyzer_clear_log()
  #define SCANALYZER_DUMP() scanalyzer_dump()
  #define SCANALYZER_PAUSE(a) scanalyzer_pause_log(a)
#else
  #define SCANALYZER_INIT()
  #define SCANALYZER_ENABLE_LOG(a)
  #define SCANALYZER_DISABLE_LOG(a)
  #define SCANALYZER_DISABLE_ALL()
  #define SCANALYZER_LOG(a, b)
  #define SCANALYZER_CLEAR_LOG()
  #define SCANALYZER_DUMP()
  #define SCANALYZER_PAUSE(a)
#endif


/**
 *  \brief Log pause types
 *
 *  Data types that are used to control the log pause feature.
 *
 **/
typedef enum  {
    LOG_PAUSE_ALL,              /**< Pause all data logging */
    LOG_RUN                     /**< Run all enabled logs   */
}t_PauseType;


/**
 *  \brief Log event ID type
 *
 *  A list of events that can be logged: ID values are written to the data log
 *  and serve as the 'key' for the associated data value.
 *
 **/
#define LOG_NULL                      0               /* This must be first   */
#define LOG_TIMER_ROLLOVER            1  
#define LOG_CMDQ_CMD                  2  
#define LOG_CMDQ_LAUNCH               3  
#define LOG_SMEC_RUN                  4  
#define LOG_SMEC_TOF                  5  
#define LOG_SMEC_PP                   6  
#define LOG_SMEC_NEW_PAGE             7  
#define LOG_SMECH_STAGE_PAGE          8  
#define LOG_SMEC_PROFILE              9  
#define LOG_SMEC_PAGE_PREPARE         10 
#define LOG_SMECH_FIRST_LINE          11 
#define LOG_SMEC_MOVE_TABLE           12 
#define LOG_PLAT_PIX_PER              13 
#define LOG_SENSOR_MINLINETIME        14 
#define LOG_CMDQ_READY_FOR_RESTART    15 
#define LOG_SMECH_RAMP_UP             16 
#define LOG_SMEC_PAUSE                17 
#define LOG_SMEC_SCAN_STOP            18 
#define LOG_SMEC_READY_FOR_SCAN       19 
#define LOG_SMEC_START_SCAN           20 
#define LOG_SMEC_ERROR                21 
#define LOG_CMDQ_ISR                  22 
#define LOG_CMDQ_CLOSE                23 
#define LOG_CMDQ_OPEN                 24 
#define LOG_SMEC_IDLE                 25 
#define LOG_SMEC_EJECT_PAGE           26 
#define LOG_SMOT_ST_ISR               27 
#define LOG_SMOT_ST_CMD               28 
#define LOG_SMOT_ISR_IOR              29 
#define LOG_CMDQ_ADD_ROWS             30 
#define LOG_SMOT_ISR_VELOCITY         31 
#define LOG_CMDQ_STOP                 32 
#define LOG_CMDQ_STATE_TRANS          33 
#define LOG_CMDQ_OLD_STATE            34 
#define LOG_CMDQ_NEW_STATE            35 
#define LOG_SMOT_DC_VEL               36 
#define LOG_SMOT_DC_VELERROR          37 
#define LOG_SMOT_DC_CTRLPWM           38 
#define LOG_SMOT_DC_ENC_CHANGE        39 
#define LOG_SMOT_DC_POSERROR          40 
#define LOG_SMOT_DC_MTR_RPM           41 
#define LOG_SMOT_DC_ENC_ROLLOVER      42 
#define LOG_SMOT_ISR_ENC              43 
#define LOG_SMOT_ISR_FREQ             44 
#define LOG_SMOT_ISR_DUR              45 
#define LOG_SMOT_DC_TGTAVGPWM         46 
#define LOG_SMOT_DC_AVGPWM            47 
#define LOG_SMOT_CAL_AVGLD            48 
#define LOG_SMOT_CAL_TGTLD            49 
#define LOG_SMOT_DC_MTR_I             50 
#define LOG_CMDQ_PAUSE                51 
#define LOG_CMDQ_RESUME               52 
#define LOG_SMOT_DC_RUN               53 
#define LOG_SMOT_DC_STOP              54 
#define LOG_SMOT_DC_ESTOP             55 
#define LOG_SMOT_DC_OLD_STATE         56 
#define LOG_SMOT_DC_NEW_STATE         57 
#define LOG_SMOT_DC_OCC_CALL          58 
#define LOG_SMECH_TOFBOF_INT_ENABLE   59 
#define LOG_SMECH_TOFBOF_INT_FIRED    60 
#define LOG_SMEC_FORCE_FAIL           61 
#define LOG_SMECH_RAW_TOFBOF          62 
#define LOG_SMOT_DC_START_POS         63 
#define LOG_SMOT_DC_RS_POS            64 
#define LOG_SMOT_DC_END_POS           65 
#define LOG_SMOT_DC_HALT              66 
#define LOG_PIC_WDMA_IPEND            67 
#define LOG_SMOT_DC_ENC_RST           68 
#define LOG_SMOT_DC_PWM_WDOG          69 
#define LOG_PIE_INTERRUPT             70 
#define LOG_PIE_DDMA_READ_ISR         71 
#define LOG_PIE_DDMA_WRITE_ISR        72 
#define LOG_PIE_DDMA_CHANNEL_ISR      73 
#define LOG_ADF_DUPLEX_FLIPPING       74
#define LOG_ADF_DUPLEX_EJECTING       75
#define LOG_ADF_DUPLEX_STAGING        76
#define LOG_PIC_INTERRUPT             77 
#define LOG_PIC_DDMA_WRITE_ISR        78 
#define LOG_SCANPIPE_BUFFER_TO_DCC    79
#define LOG_PIC_PSESD                 80
#define LOG_PIC_PSENSE                81
#define LOG_ICETEST_INTERRUPT         82
#define LOG_ICETEST_STATUS            83
#define LOG_SCAN_INTERRUPT            84

#define NUM_LOG 128

/**
 *  \brief (API) Initialize scanalyzer code
 *
 *  API function to initialize the scanalyzer code. Must be called once at
 *  startup before any other scanalyzer routines can be used.
 *
 *  Note that all events are disabled by default; desired events must be 
 *  explicitly enabled.
 *
 **/
void scanalyzer_init(void);

/**
 *  \brief (API) Enable a scanalyzer event
 *
 *  Enable logging of the specified event.
 *
 *  \param[in] log_id  ID of event
 *
 **/
void scanalyzer_enable_log( uint32_t log_id);

/**
 *  \brief (API) Disable a scanalyzer event
 *
 *  Disable logging of the specified event.  While an event is disabled, any 
 *  logging calls are simply ignored.
 *
 *  \param[in] log_id  ID of event
 *
 **/
void scanalyzer_disable_log( uint32_t log_id);

/**
 *  \brief (API) Disable all scanalyzer events
 *
 *  Disable all of the scanalyzer events
 **/
void scanalyzer_disable_all(void);

/**
 *  \brief (API) Log a scanalyzer event
 *
 *  Log the specified event.
 *
 *  \warning
 *  This can be called in interrupt context!
 *
 *  \param[in] log_id  ID of event
 *  \param[in] data    Event data
 **/
void scanalyzer_log( uint32_t log_id, uint32_t data);

/**
 *  \brief (API) Clear the scanalyzer logs
 *
 *  Discard any stored event logs.  Note that the contents of the log
 *  will not be discarded if the logs are in a paused state.
 *
 **/
void scanalyzer_clear_log(void);

/**
 *  \brief (API) Dump the scanalyzer logs
 *
 *  Dump the log contents to the serial port.
 *
 **/
void scanalyzer_dump(void);

/**
 *  \brief (API) Set the logs pause state
 *
 *  Set the global log state to either running or paused.  When in the paused
 *  state:
 *    - no new data will be logged
 *    - the logs cannot be cleared
 *
 *  \param[in] pause  Desired log pause state
 **/
void scanalyzer_pause_log(t_PauseType pause);


#endif /* INC_SCANALYZER_H */

