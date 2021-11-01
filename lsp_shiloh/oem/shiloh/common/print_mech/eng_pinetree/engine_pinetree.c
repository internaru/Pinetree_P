/*
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file engine_stub.c
 *
 * \brief This file contains a Print Engine stub.
 *
 * This print engine stub always presents itself as being online and will
 * immediately acknowledge all requests as if the pages were being printed.
 *
 */

 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>				/* ADD @@2013.01.08. By CSPark */
#include "memAPI.h"
#include "delay_api.h"
#include "nvram_api.h"			/* ADD @@2013.01.08. By CSPark */
#include "data_access_api.h"	/* ADD @@2013.01.08. By CSPark */
#include "logger.h"
#include "debug.h"
#include "utils.h"
#include "print_status_codes.h"
#include "engine_config.h"
#include "laser_engine_config_api.h"
#include "engPaperHandlingAPI.h"
#include "printvars_api.h"
#include "error_types.h"
#include "db_api.h"
#if defined(VIDEO_APP)
#include "video_laser_api.h"
#endif
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#include "pthread.h"
#include "posix_ostools.h"

#include "gpio_api.h"
#include "dcmotor_ol.h" // dcmotor_openloop.h
#include "pwm_api.h"
#include "dec_sensor.h"
#include "dec_fuser.h"
#include "dec_laser.h"
#include "dec_adc_api.h"

#include "sys_init_api.h"
#include "lassert.h"
#include "agMessage.h"

#include "print_job_types.h"
#include "paper_size_api.h"
// set a separate debug flag here because em.h overloads the system
// global debug define.


#include "farch.h"
#include "em.h"

#include "ipc.h"
#include "uart.h"
#include "option.h"
#include "dbg_pnm.h"
#include "agConnectMgr.h"
#include "print_job_api.h"

#ifdef SMART_IC_TONER /* [KSR032101] for SMC */
#include <pthread.h> /* 20130215 add by Ryu (for pthread) */
#endif

#define DBG_PRFX "ENG: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING
#define SLOG_MSG  DBG_MEMLOG_INFO		/* ADD : @@2013.01.08. By CSPark */


// debug logging tool 
#if 1
#define slogit(e) e,  logStringImpl("%s\n", #e )
// #define slogit(e) e,  dbg_printf("%s\n", #e )
#else
#define slogit(e) e
#endif


#define NUM_CREDITS 3

// ThreadX thread object
static pthread_t print_engine_thread_id;
static pthread_t mech_thread_id;

// Stack
#define PRINT_ENGINE_STACK_SIZE POSIX_MIN_STACK_SIZE
static ALIGN8 UINT8 print_engine_stack[PRINT_ENGINE_STACK_SIZE];
static ALIGN8 UINT8 mech_stack[PRINT_ENGINE_STACK_SIZE];
extern Uint32 ChangeVoltageToTemp(void);

extern void SelectGapRingMode(void);
#if MPT_SENSOR_PIN_CHG
extern void SelectMachineTypeForScanIRQ(void);
extern Uint8 fScan_IRQ;
#endif
#if EP_PARAM_TEST
extern void Get_EP_Test_Parameter(void);
#endif
#if ZX_ENABLE
extern void Get_ZX_Test_Parameter(void);
#endif
#if FUSER_PARAM_TEST
void Get_FUSER_Test_Parameter(void);
Uint8 f10pagesUSE;
Uint8 ftargetTempfor25ppm;

Uint8 within10pagesTemp;
Uint8 over10pagesTemp;
Uint8 numberofbasisofspeed;
Uint8 targetTempfor25ppm;
#endif
#if DARKNESS_TEST
extern void Get_Darkness_Parameter(void);
#endif

extern void watchdog_init(int terminal_count);
extern void watchdog_exit(void);
extern void watchdog_disable(void);
extern void watchdog_service(void);
extern void watchdog_set_terminal_count(int ms);

extern void	MainMotorSpeedChange(int, Uint8);
extern void	MirrorMotorSpeedChange(int, Uint8);
extern unsigned char IsPaperLengthNarrow(unsigned int length);
extern void Set_Engine_Parameter(void);
extern void Display_Ver_Info(void);
extern Uint8 fWakeupState;
extern Uint8 SecondPage;
extern Uint8 fPwsToPrint_1stPage;
extern Uint8 fPwsToPrint_2ndPage;
extern Uint8	fPwsToPrint;
extern Uint8 f2ndDrawerState;
extern Uint8 f3ndDrawerState;
extern Uint8 f2ndDrawerState_1stdetect;
extern Uint8 f3rdDrawerState_1stdetect;
extern Uint8 fOption_1stdetect;
extern Uint32 cPrintData;
extern Uint8 fPaperLengthIsNarrow;
extern Uint8 fWaitToChangeMotorSpeed;
extern Uint8 fWaitPrintingForResync;
extern Uint8 cFatalOptionCommFail;
extern int DrumLifePageCount;

extern Uint8 StartLamination;
extern Uint8 StartLaminating1page;
extern Uint8 StopLamination;
extern Uint8 ResumeLamination;
extern Uint8 EndLamination;
extern Uint8 CheckExitGuide;
extern uint32_t current_lamination_status;

extern Struct_Fuser_Control Fuser_Control;
#if DUPLEX_SLOW_MODE
extern int  cDuplexPrintSlowlyFor15min;
extern Uint8  fDuplexPrintSlowlyFor15min;
extern Uint8	EnvTempStatus_ForDuplexPrint;
extern Uint8 fDebugDuplexPrintSlowlyFor15minClear;
extern Struct_Fuser_Control Fuser_Control;
extern char Get_DuplexPrintTime(int time);
extern char Clear_DuplexPrintSlowlyFor15min(int time);
extern Uint8 adjust_servo_value_for_duplex_slow_mode;
#endif
#if DUPLEX_SLOW_MODE2
extern Uint8	EnvTempStatus_ForDuplexPrint;
extern Uint8  fDuplexPrintSlowlyForServoHH;
extern Uint8 fEnvTempStatusChage;
extern Uint8 fSimplexMotorUp;
extern Uint8 fDuplex_Slow_mode;
extern Uint8 	fRequest_New_PageInfo;
extern Uint8 adjust_servo_value_for_duplex_slow_mode;
#endif

extern Uint8 LDPWMAdjustforCopy;
extern Uint32 PrePageSize;
extern Uint32 PreJobNum;
extern Uint32 PreJobInfo;
extern Uint8 fCheckJobNumChange;
extern Uint8 fCheckJobInfoChange;
extern Uint8 fJobIDChanged;
extern Uint8 fJobInfoChanged;
extern Uint8 fIgnorePrePick;
extern Uint8    Get_MainMotorState(void);
extern ST_FLAG IpcFlag1;
extern ST_FLAG IpcFlag2;
extern ST_FLAG IpcFlag3;
extern unsigned char fRetryDetectInStandby;
extern Uint8 fDebugTargetTempChange;
extern Uint8 DEBUG_TargetTemp;
extern Uint8 fDebugStandbyTempChange;
extern Uint8 DEBUG_StandbyTemp;
extern void Set_Debug_Standby_Temp(unsigned char);
extern Uint8 SC_State;
extern int VideoDoneTime;

extern Uint8 fQuickly_sensor_check_T1;
extern Uint8 fQuickly_sensor_check_T2;
extern Uint8 fQuickly_sensor_check_T3;
extern char fTray2Notify;
extern Uint8 f_MSG_USER_ERR_RECOVERY;

extern void	CheckTray1(void);
extern void	CheckMPF(void);
// Message queue
mqd_t print_engine_mq;
#define NUM_PRINT_ENGINE_MESSAGES 4 
bool bottom_done = true;
//#define MAX_NUM_PAGE_TOKENS 20
static mqd_t page_mq; 

//typedef struct page_handle_s page_handle_t;

const uint32_t engine_state_cookie_val = 0x14567fd1;

static bool sheet_is_duplex = false;
static int sourceTray = MS_Tray1;

extern mqd_t lsu_ctrl_mq;

extern int Get_MotorType();
extern int motorType;

// FAN DUTY CYCLES
#define MAIN_FAN_LOW  5
#define MAIN_FAN_HALF 50
#define MAIN_FAN_FULL 100

// engine timing
 // 618 + extra
#define PAPER_ENTRY_TOP_TIMEOUT 2000
#define PAPER_ENTRY_BOTTOM_TIMEOUT 500
#define PAPER_FEED_TOP_TIMEOUT 300
#define PAPER_FEED_BOTTOM_TIMEOUT 300
#define PAPER_EXIT_TOP_TIMEOUT 600
#define PAPER_EXIT_BOTTOM_TIMEOUT 600
#define PAPER_MOTOR_FINISH_TIMEOUT 108

/** High voltage defines; note that these are duplicated from pwm_config.c
  * This is BAD.  Need to develop common header file and improve API. */
    // The Transfer PWM has a Print duty cycle of 70.1% (180 of 255).
    // The Transfer PWM has a IP duty cycle of 24.1% (69 of 255).
    // The Transfer PWM has a clean duty cycle of 0% (0 of 255).
    // The Charge PWM has a duty cycle of 81.6% (208 of 255).
    // The Developer Print PWM has a duty cycle of 62.4% (159 of 255).
    // The Developer No-Print PWM has a duty cycle of 7.8% (20 of 255)

	// move to "em.h"
    //#define TRANSFER_PRINT_DUTY_CYCLE 70
    //#define TRANSFER_IP_DUTY_CYCLE 24
    //#define TRANSFER_CLEAN_DUTY_CYCLE 0

    //#define DEVELOPER_PRINT_DUTY_CYCLE 62
    //#define DEVELOPER_NO_PRINT_DUTY_CYCLE 8  


//typedef struct engine_state_s : move to "em.h"

Uint16 first_motor_cycle;
Uint16 second_motor_cycle;
int MotorSpeedparam;
extern Uint8 fMotorSpeedMode;
extern Uint8 fMotorSpeedChange;
extern int cProcessCounter;
extern Uint8	fState_FullStack;
#if DARKNESS_TEST
extern Uint8 Darknesslevel;
#endif
extern Uint8 machineType;
extern int machineType_check(char*);

engine_state_t engine_state;

/// todo: engine error codes need to be in print_status_codes.h
#define ENGINE_FATAL_LASER_FAIL 12 | STATUS_ERROR
#define ENGINE_FATAL_PAPER_MOTOR_FAIL 13 | STATUS_ERROR

//typedef struct engine_hardware_s : move to "mv.h"

engine_hardware_t *eng_hardware;

#ifdef MARGIN_ADJUST /* @@2013.01.08. By CSPark */
/** \brief The engine nvram manager block version number.
 * This number should be incremented every time the structure
 * "engine_nvram_t" changes */
#define	NUM_MONO_VIDEO_CHANNELS	2
#define ENGINE_NVRAM_VERSION 1
typedef struct engine_nvram_s
{
    // Adjustment to engine default Top margin for each color.
    // Indexed by the COLOR enum.
    // Typically set by CPR calibration, but may also be adjusted from the
    // command line interface for debugging purposes.
    int16_t top_margin_adjust[NUM_MARKING_COLOR_CHANNELS];  // note this is signed; may be negative or positive

    // Adjustment to engine default side margins for each video channel.  Units are sub-pixels.
    uint32_t side_margin_adjust[NUM_MONO_VIDEO_CHANNELS];
} engine_nvram_t;

#ifdef HAVE_NVRAM
/** \brief The NVM manager handle, used to store language variables in NVram */
static nvram_handle_t * engine_nvram_handle = NULL;
#endif
static engine_nvram_t engine_nvram;

#endif /* MARGIN_ADJUST : @@2013.01.08. By CSPark */

static page_handle_t *queue_page_for_print(page_handle_t* page);
void engine_cancel_page_msg( MESSAGE *msg );
void engine_dbg_init();
void dec_printer_feed_paper( bool feed );

void engine_fromDB_set_front_top_margin_adjust_in_lines(int32_t num_lines);
void engine_fromDB_set_back_top_margin_adjust_in_lines(int32_t num_lines);
void engine_fromDB_set_front_side_margin_adjust_in_subpixels(int32_t num_value);
void engine_fromDB_set_back_side_margin_adjust_in_subpixels(int32_t num_value);
void engine_fromDB_set_vertical_magnification_in_lines(int32_t num_lines);
void engine_fromDB_set_back_side_gap_margin_in_subpixels(int32_t num_values);
void engine_fromDB_set_copy_front_side_margin_in_subpixels(int32_t num_value);
void engine_fromDB_set_copy_back_side_margin_in_subpixels(int32_t num_value);
void engine_fromDB_set_copy_front_top_margin_in_lines(int32_t num_lines);
void engine_fromDB_set_copy_back_top_margin_in_lines(int32_t num_lines);
void engine_fromDB_set_servo_value(int32_t servo_value);

void Set_Fuser_Parameter(void); /* [KSR031801] for Fuser Param *//* [KSF050201] */
//void Get_Fuser_Test_Parameter(void); /* [KSF050201] */

void LaminatingSendMsgToEng(AGMSG msgType);
void LaminatingSendMsgToPM(uint32_t parameter1);

void engine_state_init(void)
{
    engine_state.cookie = engine_state_cookie_val;
    engine_state.in_job = false;
    engine_state.paper_out_tray1 = false;
    engine_state.paper_out_tray2 = false;
    engine_state.paper_out_tray3 = false;
    engine_state.paper_out_tray4 = false;
    engine_state.front_door_open = false;
    engine_state.pick_page = 0;
    engine_state.picked_page = 0;
    engine_state.active_page = 0;
    engine_state.page_exiting = 0;
    
    engine_state.misspick_tray1_jam = false;
    engine_state.misspick_tray2_jam = false;
    engine_state.misspick_tray3_jam = false;
    engine_state.misspick_tray4_jam = false;
    engine_state.exit_not_detacted_jam = false;
    engine_state.input_not_released_jam = false;
    engine_state.exit_not_released_jam = false;
    engine_state.duplex_jam = false;

	engine_state.cartridge_lock = false;
	engine_state.cartridge_rec_fail = false;

    engine_state.video_done = 0;
    engine_state.pm_statusrequest = STATUS_ONLINE;

    // pick solenoid disengage timer = 250 ms
    // 
    engine_state.top_to_bottom_delay_msecs = 950; // letter.
    // letter + input to drum - interpage gap (11 * 25.4 + 45.5) / 230.6
    // engine_state.page_bottom_timer = 0;
    // revisit: tune me to be time from bottom to just past exit sensor
    engine_state.bottom_to_safe_delay_msecs = 1389;
    engine_state.page_safe_timer = 0; 
    engine_state.fcancelrequest = false;	
}


// Local function prototypes
/// message api thread talks to printmgr 
static void* engine_thread(void* unused);

bool engine_print_prepare(engine_hardware_t* eng_hw);
void engine_start_high_voltage(engine_hardware_t* eng_hw);
void engine_stop_high_voltage(engine_hardware_t* eng_hw);
void engine_print_stop(engine_hardware_t* eng_hw);

/// mech control thread
static void* engine_ctrl_loop(void* unused);

extern void EngineApplication(engine_hardware_t* eng_hw);
extern void Engine_Init(void);
extern Uint32 	PendingPrintPageData[RIPDATA];
extern ST_FLAG CommandReceiveFlag;
extern Uint8	fWaitPageCmd;
extern ST_FLAG		CoverState;
extern Uint8	fFullStack_Error;
extern Uint8 	fPreRunBeforePrintCmd;

extern Uint8	IsPaperPresentOnPins(void);
extern Uint8	IsPaperPresentOnPexs(void);
#ifdef SMART_IC_TONER /* [KSR032101] for SMC */
extern void 	SecurityIC_Check(void);
extern Uint8	fSmartIC_Authentication;
extern void		SmartICDevDrvInit(void);
Uint32	cTotal_Print_Count; /* 20130212 by Ryu (i2c_thread: cTotal_Print_Count) */
extern void 	CheckIPC(void); /* 20130213 add by Ryu (for IPC) */
extern void *System_IPC(void *arg);
//extern void *Tray2_Read(void *arg);
extern void *SmartIC_Read(void *arg); /* [KSR032501] for SMC thread */
#endif
extern Uint8	HowManyActivePageSupervisor(void);
uint32_t fduplex_test = false;

uint32_t engine_get_time_msec()
{
    return timer_get_time_usec() / 1000;
}

#ifdef HAVE_NVRAM
error_type_t engine_nvram_init( nvram_init_type_t init_type,
        uint16_t version,
        void * init_location,
        uint32_t init_location_size,
        void * unused )
{
    // copy current vars to our local structure
    memcpy(&engine_nvram, init_location, MIN(sizeof(engine_nvram), init_location_size));

    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */

            // settings no longer valid; reset
            // TODO longer term may need to handle individual cases above
            memset(&engine_nvram, 0, sizeof(engine_nvram));

        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
            // for these cases we don't need to reset anything on the engine at this point

            // TODO: if we start storing user settable engine parameters we may wish to do something
            // for the NVRAM_USER_DEFAULTS case

            ASSERT(sizeof(engine_nvram) <= init_location_size);
            memcpy( init_location, &engine_nvram, sizeof(engine_nvram));
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            ASSERT( sizeof(engine_nvram) == init_location_size);
            break;
    }
    return ( OK );
}
#endif

void engine_init( void )
{
#ifdef MARGIN_ADJUST /* @@2013.01.08. By CSPark */
    memset(&engine_nvram, 0, sizeof(engine_nvram));
#ifdef HAVE_NVRAM
    {
    error_type_t e_res;
    e_res = nvram_variable_register( &engine_nvram_handle,
            ENG_VARS_ID,
            ENGINE_NVRAM_VERSION,
            sizeof(engine_nvram_t),
            engine_nvram_init,
            NULL );
    ASSERT( OK == e_res );
    }
#endif
#endif /* MARGIN_ADJUST : @@2013.01.08. By CSPark */

    printer_setup_t printer_setup;
    printvar_get_printersetup( &printer_setup );
    ConstructPhysicalTrayNumberToTRAYENUMObject( &printer_setup );

    posix_create_message_queue(&print_engine_mq, "/PrintEngQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(MESSAGE));
    router_register_queue(ENGINEID, print_engine_mq);

    posix_create_message_queue(&page_mq, "/PrintEngPgQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(void*));

    posix_create_thread(&mech_thread_id, engine_ctrl_loop, 0, "eng_ctrl_task", mech_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);
    posix_create_thread(&print_engine_thread_id, engine_thread, 0, "eng_msg_task", print_engine_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);

    MSG("Initialized\n");
    fduplex_test = false;
}

void engine_shutdown( void )
{
	/* Nothing yet -- This is just a stub */
    DBG1("Engine: Shutting down\n");
}


bool engine_try_to_clear_errors( ) 
{
    MESSAGE message;
    bool clear;

    clear = engine_state.pm_statusrequest == STATUS_ONLINE ? true : false;
  
    message.msgType = MSG_STATUSUPDATE;
    engine_state.pm_statusrequest = message.param1 = STATUS_ONLINE;

    //if (engine_state.exit_jam) {
        //engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_OUTPUT;
        //SYMsgSend(PRINTMGRID, &message);
    //}

	if (engine_state.paper_out_tray1) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY1;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.paper_out_tray2) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY2;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.paper_out_tray3) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY3;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.paper_out_tray4) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY4;
        SYMsgSend(PRINTMGRID, &message);
    }
    
    if (engine_state.misspick_tray1_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY1;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.misspick_tray2_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY2;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.misspick_tray3_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY3;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.misspick_tray4_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY4;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.exit_not_detacted_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_EXTSEN_NOT_DET;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.input_not_released_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_INPSEN_NOT_REL;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.exit_not_released_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_EXTSEN_NOT_REL;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.duplex_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_DUPLEX;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.front_door_open) {
        engine_state.pm_statusrequest = message.param1 = STATUS_DOOROPEN_FRONT;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.cartridge_lock) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TONER_BLOCK;
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.cartridge_rec_fail) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_REINSERTCARTRIDGE;
        SYMsgSend(PRINTMGRID, &message);
    }

    if ( message.msgType == STATUS_ONLINE ) {
        if ( !clear ) {
            SYMsgSend(PRINTMGRID, &message);
	}
    } else { 
        clear = false;
    }
    return 1; // clear; // false on error
}

//void clear_engine_state(void)
//{
	//engine_state.misspick_tray1_jam = false;
	//engine_state.misspick_tray2_jam = false;
	//engine_state.misspick_tray3_jam = false;
	//engine_state.misspick_tray4_jam = false;
	//engine_state.exit_not_detacted_jam = false;
	//engine_state.input_not_released_jam = false;
	//engine_state.exit_not_released_jam = false;
	//engine_state.duplex_jam = false;
	//engine_state.paper_out_tray1 = false;
	//engine_state.paper_out_tray2 = false;
	//engine_state.paper_out_tray3 = false;
	//engine_state.paper_out_tray4 = false;
	//engine_state.front_door_open = false;
	
	//engine_state.cartridge_lock = false;
	//engine_state.cartridge_rec_fail = false;
	//engine_state.pm_statusrequest = STATUS_ONLINE;
//}

uint32_t clear_engine_state(void)
{
	MESSAGE message;
	
	engine_state.paper_out_tray1 = false;
	engine_state.paper_out_tray2 = false;
	engine_state.paper_out_tray3 = false;
	engine_state.paper_out_tray4 = false;
	
	message.msgType = MSG_STATUSUPDATE;
	engine_state.pm_statusrequest = message.param1 = STATUS_ONLINE;
	
	// Door Check
	if(fFrontCoverState == OPEN)
	{
		engine_state.front_door_open = true;
		engine_state.pm_statusrequest = message.param1 = STATUS_DOOROPEN_FRONT;
		SYMsgSend(PRINTMGRID, &message);
		return engine_state.pm_statusrequest;
	}
	else
	{
		engine_state.front_door_open = false;
	}
	
	// Jam Check
	if((IsPaperPresentOnPexs() == FALSE) && (IsPaperPresentOnPins() == FALSE))
	{
		engine_state.misspick_tray1_jam = false;
		engine_state.misspick_tray2_jam = false;
		engine_state.misspick_tray3_jam = false;
		engine_state.misspick_tray4_jam = false;
		engine_state.exit_not_detacted_jam = false;
		engine_state.input_not_released_jam = false;
		engine_state.exit_not_released_jam = false;
		engine_state.duplex_jam = false;
	}
	else
	{
		if(engine_state.misspick_tray1_jam)				engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY1;
		else if(engine_state.misspick_tray2_jam)		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY2;
		else if(engine_state.misspick_tray3_jam)		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY3;
		else if(engine_state.misspick_tray4_jam)		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY4;
		else if(engine_state.exit_not_detacted_jam)		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_EXTSEN_NOT_DET;
		else if(engine_state.input_not_released_jam)	engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_INPSEN_NOT_REL;
		else if(engine_state.exit_not_released_jam)		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_EXTSEN_NOT_REL;
		else if(engine_state.duplex_jam)				engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_DUPLEX;
		SYMsgSend(PRINTMGRID, &message);
		return engine_state.pm_statusrequest;
	}
	
	// FullStack Check
	if((engine_state.full_stack==TRUE) || (fFullStack_Error==TRUE))
	{
		engine_state.full_stack = true;
		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_FULLSTACK;
		SYMsgSend(PRINTMGRID, &message);
		return engine_state.pm_statusrequest;
	}
	
	// Cartridge
	if(engine_state.cartridge_lock == true)
	{
		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TONER_BLOCK;
		SYMsgSend(PRINTMGRID, &message);
		return engine_state.pm_statusrequest;
	}
	else if(engine_state.cartridge_rec_fail == true)
	{
		engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_REINSERTCARTRIDGE;
		SYMsgSend(PRINTMGRID, &message);
		return engine_state.pm_statusrequest;
	}
	SYMsgSend(PRINTMGRID, &message);
	return engine_state.pm_statusrequest;
}


void* vd_page = 0; 	
/// video calls this
void engine_pagedone_callback(PAGE_DATA *page, error_type_t status)
{
    SLOG_ERR("video done callback %p %d\n", page, status);
    MSG("video done callback %p %d %p\n", page, status, vd_page);
    MSG("(eng)VideoDoneTime = %d\n", cProcessCounter-VideoDoneTime);
    if (status == OK)
    {
        vd_page = (void *)page;
        engine_state.video_done = (page_handle_t *)page; // really didn't want to know about PAGE_DATA pointers...  revisit making this an abstract type
    }
    else
    {
        ERR("video done callback CANCELED %p %d\n", page, status);
    }
}




// Private function definitions 0=Tray1, 1=MPT, 2=Tray2
static void* engine_thread(void* unused)
{
    MESSAGE message;
    int retcode;
    //extern bool fStartMainMotor;
    // Wait for system initialization to complete
    SysWaitForInit();
    engine_state_init();   
    SLOG_ERR("engine thread entry\n");

    while(1)
    {
        
        retcode = mq_receive(print_engine_mq, (char*)&message, sizeof(MESSAGE), 0);
        XASSERT( retcode == sizeof(MESSAGE), thread_errno);

        switch(message.msgType)
        {
		case MSG_START_LAMINATION:
			StartLamination = SET;
			MSG("(Eng)Receive MSG_START_LAMINATION\n");
			break;
			
		case MSG_START_LAMINATING_1PAGE:
			StartLaminating1page = SET;
			MSG("(Eng)Receive MSG_START_LAMINATING_1PAGE\n");
			break;
			
		case MSG_END_LAMINATION:
			EndLamination = SET;
			MSG("(Eng)Receive MSG_END_LAMINATION\n");
			break;
			
		case MSG_CHECK_EXITGUIDE_ATTACH:
			CheckExitGuide = SET;
			MSG("(Eng)Receive MSG_CHECK_EXITGUIDE_ATTACH\n");
			break;
			
		case MSG_CHECK_EXITGUIDE_DETACH:
			CheckExitGuide = SET;
			MSG("(Eng)Receive MSG_CHECK_EXITGUIDE_DETACH\n");
			break;
			
		case MSG_STOP_LAMINATION:
			StopLamination = SET;
			MSG("(Eng)Receive MSG_STOP_LAMINATION\n");
			LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMINATION_STOPPED);
			break;
			
		case MSG_RESUME_LAMINATION:
			ResumeLamination = SET;
			StopLamination = CLEAR;
			MSG("(Eng)Receive MSG_RESUME_LAMINATION\n");
			LaminatingSendMsgToPM(current_lamination_status);
			break;
						
        case MSG_JOBSTART:
        {
            // No action required           

            //---JOB_DATA* jobPtr = (JOB_DATA*)msg->param3;

                //this means a page is on the way soon. Call product specific to
                //see if it needs to warm up or anything.
            //---EngStartJob(jobPtr->jobDataType);
			//ERR("YJ_chk : Eng Job_Data <%d, %d, %d, %d>, Dup=%d, Col=%d, Cop=%d\n",
			//((JOB_DATA*)message.param3)->DEFAULTSOURCE, ((JOB_DATA*)message.param3)->requestedOutTray, ((JOB_DATA*)message.param3)->request_media_detect, ((JOB_DATA*)message.param3)->mediaType,
			//((JOB_DATA*)message.param3)->DUPLEX, ((JOB_DATA*)message.param3)->COLLATE, ((JOB_DATA*)message.param3)->COPIES);

            slogit(   engine_state.in_job = true );
            MSG("(eng)MSG_JOBSTART : JobNum = %d, JobInfo = %d, DocType = %d\n", ((JOB_DATA*)message.param3)->JobNum, ((JOB_DATA*)message.param3)->JobInfo, ((JOB_DATA*)message.param3)->DocType);
#if KSF_DBG_TIMESTAMP
			printf("ENG: MSG_JOBSTART (%u)\n",posix_gettime_ms());
#endif            
			
			if((((JOB_DATA*)message.param3)->JobInfo == COPYJOB) && (((JOB_DATA*)message.param3)->DocType == e_COPYPAGE)) // Copy
			{
				MSG("(eng)This is Copy Job\n");
				LDPWMAdjustforCopy = SET;
			}
			else
			{
				LDPWMAdjustforCopy = CLEAR;
			}
			
			uint32_t msg;
			msg = 0;
			if(Fuser_Control.EM_State == EMS_PowerSave)
			{
				msg = Wake_Up;
				posix_message_send(lsu_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
				MSG("(eng)Send Wakeup MSG !!\n");
			}
			
			fEarly_Start_Print_Cmd = SET;
            break;
        }
		
        case MSG_WAKEUP:
        {
            message.msgType = MSG_STATUSUPDATE;
            message.param1 = STATUS_ONLINE;
            SYMsgSend(PRINTMGRID, &message);
            DBG1("Engine: MSG_WAKEUP\n");
            break;
        }

        case MSG_STATUSREQUEST:
        {
            message.msgType = MSG_STATUSUPDATE;
            // message.param1 = STATUS_ONLINE or an error condition
            // use print_status_codes.h to report engine errors up
	    //if ( fStartMainMotor && (engine_state.pm_statusrequest & STATCAT_PAPEROUT) == STATCAT_PAPEROUT) 
	    //	message.param1 = STATUS_ONLINE;
	    //else
		message.param1 = engine_state.pm_statusrequest;
            DBG1("Engine: MSG_STATUSREQUEST %d\n", message.param1);
            SYMsgSend(PRINTMGRID, &message);

            break;
        }

        case MSG_ERRCLEAR:
        {
            message.msgType = MSG_CREDITUPDATE;	  

	    // check sensors perhaps the user has:
	    // added paper or cleared a jam or closed the door
	    // if all the static checks pass we then 

	    if ( engine_try_to_clear_errors( ) )
	        message.param1 = NUM_CREDITS;
	    else 
	        message.param1 = 0;
            SYMsgSend(PRINTMGRID, &message);

            DBG1("MSG_ERRCLEAR credits %d \n", message.param1 );
            break;
        }
        
        case MSG_PAGESTART:
        {
                dbg_dump_page_stripdata_to_pbm_file( message.param3 );
                dbg_validate_page( message.param3 );
		uint32_t x, y;
		
		PAGE_DATA *page = message.param3 ;
		paper_get_media_size( page->engMediaSize, &x, &y );
		ERR( " page media s %d media t %d tray %d x %d y %d image x %d y %d\n",
		     page->engMediaSize, page->engMediaType, page->engTray, 
		     x, y,
		     page->image_output_info.image_width_in_pixels, page->image_output_info.image_height_in_lines );
                queue_page_for_print( message.param3 );

            // sk: This shouldn't be here.
            // revisit: set / clear interface
            if((engine_state.pm_statusrequest != STATUS_ONLINE) && (engine_state.pm_statusrequest != STATUS_TONER_NO_ISSUE))
            {
                message.msgType = MSG_STATUSUPDATE;
		//if ( fStartMainMotor && (engine_state.pm_statusrequest & STATCAT_PAPEROUT) == STATCAT_PAPEROUT) 
		//    message.param1 = STATUS_ONLINE; //engine_state.pm_statusrequest;
		//else {
			DBG1("Engine: Not Online <%x>\n",engine_state.pm_statusrequest);
		    message.param1 = engine_state.pm_statusrequest;
		    SYMsgSend(PRINTMGRID, &message);
		    //}
                break;  // done here.
            }
            DBG1("Engine: Page start\n");
            break;
        }
	
        case MSG_CANCELJOB:
            ERR("Engine: MSG_CANCELJOB fail this should never be sent here!\n");
	    // break;
        case MSG_CANCELPAGE:
        {
			ERR("Engine: MSG_CANCELPAGE\n");
            if(message.msgType == MSG_CANCELJOB)
			{
				if (HowManyActivePageSupervisor() == 0)
				{
					engine_cancel_page_msg(&message);
					engine_state.fcancelrequest = FALSE;
				}
				else
				{
					engine_state.fcancelrequest = TRUE;
				}
			}
			else
			{
				engine_cancel_page_msg(&message);
			}
            break;
        }
        
        case MSG_JOBEND:
        {
            slogit(   engine_state.in_job = false );
            // No response required
            DBG1("Engine: Job end\n");
            break;
        }
        
        case MSG_SUPPLIES_PAGE_DONE:
		{
			extern ST_FLAG		IpcFlag2;
			extern Uint32 		Toner_Count_Of_Page;
			Uint32 dot_count;
			PAGE_DATA *curPageData = message.param3 ;
			dot_count = message.param2 / 10;	// 5% dots �� 1370444 ����, �״�� ���� 12K dots�� 4byte�� �ʰ���. ���� /10���� ����  
            ERR("EM: dot_count %d <page=%p> \n", dot_count, curPageData);
            fToner_Count_Cmd = ACTIVE;
            Toner_Count_Of_Page = dot_count;
			break;
		}
		
        case MSG_CONFIG_CHANGED:
        {
			ERR("ENG : MSG_CONFIG_CHANGED <%d, %d>\n", message.param1, message.param2);
			switch (message.param1)
			{
				case DB_DUPLEX_UPDATED:
					sheet_is_duplex = message.param2;
					break;

				case DB_TOPMARGIN_UPDATED:
					engine_fromDB_set_front_top_margin_adjust_in_lines(message.param2);		// message.param2 -50~+50
					break;

				case DB_LEFTMARGIN_UPDATED:
					engine_fromDB_set_front_side_margin_adjust_in_subpixels(message.param2);	// message.param2 -50~+50
					break;

				case DB_TRAYNO_UPDATED:
					// tray no
					switch(message.param2)
					{
						case 0:
							sourceTray = MS_Tray1;
							break;
						case 1:
							sourceTray = MS_MultiPurposeTray;
							break;
						case 2:
							sourceTray = MS_Tray2;
							break;
						case 3:
							sourceTray = MS_Tray3;
							break;
					}
					break;

				case DB_BACKTOPMARGIN_UPDATED:
					engine_fromDB_set_back_top_margin_adjust_in_lines(message.param2);		// message.param2 -50~+50
					break;

				case DB_BACKLEFTMARGIN_UPDATED:
					engine_fromDB_set_back_side_margin_adjust_in_subpixels(message.param2);		// message.param2 -50~+50
					break;

				case DB_BEAMADJUST_UPDATED:
					video_set_pcode(PWM_DB_SET_MODE, message.param2);
					break;

				case DB_VERTICAL_MAGNIFICATION_UPDATED:
					engine_fromDB_set_vertical_magnification_in_lines(message.param2);
                break;

			    case DB_HORIZONTAL_MAGNIFICATION_UPDATED:
					video_set_pixel_freq(PIXEL_FREQ_HMAG_MODE, message.param2);
				break;

			    case DB_A4_LT_GAP_MARGIN:
				    engine_fromDB_set_back_side_gap_margin_in_subpixels(message.param2); 		// message.param2 -50~+50
				break;

			    case DB_COPY_LEFTMARGIN_UPDATED:
				    engine_fromDB_set_copy_front_side_margin_in_subpixels(message.param2); 		// message.param2 -50~+50
				break;

			    case DB_COPY_BACKLEFTMARGIN_UPDATED:
				    engine_fromDB_set_copy_back_side_margin_in_subpixels(message.param2); 		// message.param2 -50~+50
				break;

			    case DB_COPY_TOPMARGIN_UPDATED:
				    engine_fromDB_set_copy_front_top_margin_in_lines(message.param2); 		// message.param2 -50~+50
				break;

			    case DB_COPY_BACKTOPMARGIN_UPDATED:
				    engine_fromDB_set_copy_back_top_margin_in_lines(message.param2); 		// message.param2 -50~+50
				break;
				
			    case DB_SERVOADJUST_UPDATED:
				    engine_fromDB_set_servo_value(message.param2); 		// message.param2 0~255
				break;
			}
			break;
        }
        
        case MSG_DRUM_PAGE_COUNT:
        {
			ERR("ENG : MSG_DRUM_PAGE_COUNT <%d>\n", message.param1);
			DrumLifePageCount = (int)(message.param1);
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE
			DrumLifePageCount = 0;
#endif
			break;
		}
        
        case MSG_USER_ERR_RECOVERY:
        {
			f_MSG_USER_ERR_RECOVERY = SET;
/*			
			fWaitPageCmd = TRUE;
			clear_engine_state();
			fClearErrorsCmdRcv = SET;
			Fuser_Control.EM_State = EMS_RecoverError;
*/
			ERR("ENG : MSG_USER_ERR_RECOVERY : Send Online\n");
			break;
		}
		
        default:
        {
            DBG1("Engine: Unhandled Message %d received \n", message.msgType);
        }
        } // end switch
    } // end while
    return 0;
}

void engine_cancel_page_msg( MESSAGE *message )
{
    // revisit
    void *cleanup = 0;

    while(0 == posix_wait_for_message(page_mq, (char*)&cleanup, sizeof(void*), 0))
    {
        SLOG_ERR("canceling page %p\n", cleanup);
        ERR("canceling page %p\n", cleanup);
    }
    engine_state.fcancelrequest = FALSE;
    extern void Sleep_till_nothing_in_flight();

    if (message->msgType == MSG_CANCELJOB) {
	// atXStempt to clear the engine
	extern void ClearAllPageSupervisor();

	Sleep_till_nothing_in_flight();
	ClearAllPageSupervisor(); 

    }

    // engine is done canceling, now cancel video
    // this is an inflight page! it is up to the video to do its thing
    // we will continue moving the paper.
    SLOG_ERR( "canceling video pages \n" );
    ERR( "canceling video pages \n" );
    video_cancel_pages();
    fWaitPageCmd = TRUE;

    // if there was a page actually in video when the cancel occurred we need to wait
    // for video cancel to complete before acknowledging the cancel
    //Sleep_till_nothing_in_flight();

	extern Struct_Fuser_Control Fuser_Control;
	ERR("Engine: engine_cancel_page_msg : EM_State %d %d (%x)\n",Fuser_Control.EM_State, Fuser_Control.fMakeMotorRun, engine_state.pm_statusrequest);
    if((Fuser_Control.EM_State == EMS_RecoverError) && (SC_State != ST_TONER_BLOCK) && (Fuser_Control.fMakeMotorRun==CLEAR) && ((STATUS_CATEGORY_MASK & engine_state.pm_statusrequest) == STATCAT_PAPEROUT))
    {
		fClearErrorsCmdRcv = true;
		clear_engine_state();
	}
	
	ERR("Current ENG Status = %x\n",engine_state.pm_statusrequest);
    // message->msgType = MSG_CANCELPAGE;
    message->param1 = LOCAL_ACK;
    SYMsgSend(PRINTMGRID, message);
    ERR("Engine: MSG_CANCELPAGE\n");
    if (message->msgType == MSG_CANCELJOB) {		// TODO : error condition logic required
	//if(0) {
	message->msgType = MSG_STATUSUPDATE;
            // message.param1 = STATUS_ONLINE or an error condition
            // use print_status_codes.h to report engine errors up
	    //if ( fStartMainMotor && (engine_state.pm_statusrequest & STATCAT_PAPEROUT) == STATCAT_PAPEROUT) 
	    //	message.param1 = STATUS_ONLINE;
	    //else
	//message->param1 = STATUS_ONLINE; // engine_state.pm_statusrequest;
	//ERR("Engine: MSG_STATUSREQUEST %d\n", message->param1);
	//SYMsgSend(PRINTMGRID, message);
	
	message->msgType = MSG_CREDITUPDATE;
    message->param1 = NUM_CREDITS;
    SYMsgSend(PRINTMGRID, message);
    }

//!!!!! // this is new, after a cancel we need to update the credits.

    message->msgType = MSG_CREDITUPDATE;
    message->param1 = NUM_CREDITS;
    SYMsgSend(PRINTMGRID, message);
}

/// who calls when and why?
/// *** FIRST PAGE OUT TIMING POINT ***
/// from :  MSG_TIME_TO_END,   
/// Param1 is the number of 10msec till the end of a page.estimate. /// Used by copy
/// This message really means "time to end of page data". When the time
/// ticks down to within the length of time required for engine pre-warmup,
/// issue the prewarmup command.
/// 
void EngPlatPrewarmupCmd(
    uint32_t ten_msecs_till_end_of_page
    )
{ 
    DBG1(" EngPlatPrewarmupCmd(%ld milliseconds)"
                " @ %ld usecs\n", 
                ten_msecs_till_end_of_page, 
                timer_get_time_usec());

    // convert to engine_warmup_signal if needed in usecs.
}


/// Cancel any pending engine print command.
void EngCancelJob(void)
{
    DBG1(" %s ", __FUNCTION__ );

}

static page_handle_t *queue_page_for_print(page_handle_t* page)
{
    MSG("queue page  %p \n", page);

    //ERR("YJ_chk : Eng <%d, %d, %d, %d>, <%d, %d, %d, %d> Dup=%d DocType=%d\n",
	//((PAGE_DATA*)page)->DEFAULTSOURCE, ((PAGE_DATA*)page)->requestedOutTray, ((PAGE_DATA*)page)->PAPER, ((PAGE_DATA*)page)->mediaType,
	//((PAGE_DATA*)page)->engTray, ((PAGE_DATA*)page)->engOutTray, ((PAGE_DATA*)page)->engMediaSize, ((PAGE_DATA*)page)->engMediaType,
	//((PAGE_DATA*)page)->DUPLEX, ((PAGE_DATA*)page)->DocType);

    engine_state.send_msg_page = page;

    if(0 != posix_message_send(page_mq, (void*)&engine_state.send_msg_page, sizeof(void*), MQ_DEFAULT_PRIORITY, 0))
    {
        ERR(" failed to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
        SLOG_ERR(" failed to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
        return 0;
    }
    ERR(" success to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
    SLOG_ERR(" success to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
    return page;
}


/// wait for page from above to be ready to print.
///
uint32_t page_count=0;

bool printer_get_page(uint32_t ticks)
{
    uint32_t tmp_type;
    uint8_t option_pre_pick;
    uint32_t tmpDarknessLevel;
    
	if(0 == posix_wait_for_message(page_mq, (void*)&engine_state.pick_page, sizeof(void*), 0 ))
    {  
	page_count++;
	PAGE_DATA* page = (PAGE_DATA*)engine_state.pick_page;
        MSG("(eng)rec page  %p (%d, JobID %d, PageID %d) \n", page, page_count, page->pMyJob->JobNum, page->PageCount);
        SLOG_ERR("rec page  %p (%d)\n", page, page_count);
    
//    cPrintData++;
    
    uint32_t width, length;
		
	paper_get_media_size( page->engMediaSize, &width, &length);
	ERR( "(eng) page media = %d tray %d (%d) width = %d length = %d lastpage = %d darknesslevel = %d tonersave = %d resolution = %d\n", 
	     page->engMediaSize, page->engTray, page->engEmptyTray, width, length, page->fLastPage, page->ImageOption.darknessLevel, page->ImageOption.tonerSave, page->ImageOption.imageResolution);
	if(length > PL_Legal*100)
	{
		length = PL_Legal*100;
		MSG("(Eng)Length is very long. Setting Legal\n");
	}
	
	// Type 4bits, Weight 2bits, Texture 2bits
	//switch(page->engMediaType)
	//{
		//case MEDIATYPE_PLAIN_PAPER:			tmp_type=0x05;		break;			// Plain 		/ Normal 	/ Normal
		//case MEDIATYPE_TRANSPARENCY:		tmp_type=0x45;		break;			// Transparency / Normal 	/ Normal
		//case MEDIATYPE_CARDSTOCK:			tmp_type=0x35;		break;			// Cardstock 	/ Normal 	/ Normal
		//case MEDIATYPE_ENVELOPE:			tmp_type=0x25;		break;			// Envelope 	/ Normal	/ Normal
		//case MEDIATYPE_HEAVY1:				tmp_type=0x09;		break;			// Plain 		/ Heavy 	/ Normal
		//case MEDIATYPE_LABEL:				tmp_type=0x15;		break;			// Label 		/ Normal 	/ Normal
		//case MEDIATYPE_LIGHTPAPER:			tmp_type=0x00;		break;			// Plain 		/ Light 	/ Smooth
		//case MEDIATYPE_CARD_GLOSSY:			tmp_type=0x65;		break;			// Post Card 	/ Normal	/ Normal
		//case MEDIATYPE_EXTRA_HEAVY:			tmp_type=0x0a;		break;			// Plain 		/ Heavy 	/ Rough
		//default:							tmp_type=0x05;		break;
	//}
	tmp_type = page->MediaDescription;
	MSG("(Eng)### MediaDescription = 0x%x ###\n",tmp_type);
	
	if((fPwsToPrint_2ndPage == SET) && !((GetAutoDuplex(page->DUPLEX) == SET) && (page != (void*)page->sheet->BackPage)))
	{
		fPwsToPrint_2ndPage = CLEAR;
	}	
	if((fPwsToPrint_1stPage == SET) && !((GetAutoDuplex(page->DUPLEX) == SET) && (page != (void*)page->sheet->BackPage)))
	{
		fPwsToPrint_2ndPage = SET;
		fPwsToPrint_1stPage = CLEAR;
		MSG("(eng)fPwsToPrint_2ndPage\n");
	}	
	if(fWakeupState == SET)
	{
		fWakeupState = FALSE;
		fPwsToPrint_1stPage = SET;
		MSG("(eng)fPwsToPrint_1stPage\n");
	}
	
#if DUPLEX_SLOW_MODE2	
	fEnvTempStatusChage = EnvTemp_Clear;
	fDuplexPrintSlowlyForServoHH = CLEAR;
#endif

	// Print Darkness Setting (Dakness Level Range 1~10)
	if ((page->ImageOption.darknessLevel >= 1) && (page->ImageOption.darknessLevel <= 10))
		tmpDarknessLevel = page->ImageOption.darknessLevel;
	else
		tmpDarknessLevel = 6;			// Default Level = 6
		
	if (page->ImageOption.tonerSave != 0)
	{
		if(tmpDarknessLevel >= 4)
			tmpDarknessLevel -= 3;
		else
			tmpDarknessLevel = 1;
	}
#if 0 // del.lsh to apply org darkness level (1200dpi)
	//if (page->ImageOption.imageResolution == 1200)
	//{
		//if(tmpDarknessLevel <= 8)
			//tmpDarknessLevel += 2;
		//else
			//tmpDarknessLevel = 10;
	//}
#endif
	ERR("(eng) Darkness Level = %d\n",tmpDarknessLevel);
	
	if(GetAutoDuplex(page->DUPLEX))
	{
		if ( page == (void*)page->sheet->BackPage )
		{
			MSG("(eng)Duplex back %p %p %p \n", 
			    page, page->sheet->FrontPage, page->sheet->BackPage );
			//PendingPrintPageData[1] = page->temp_InputTray + 0x10;		//sourceTray;	//Tray1
			PendingPrintPageData[1] = page->engTray + 0x10;		//sourceTray;	//Tray1
			PendingPrintPageData[7] = 1;
			PendingPrintPageData[10] = (uint32_t)page;
			//PendingPrintPageData[12] = (uint32_t)page;
			PendingPrintPageData[11] = (uint32_t)page->sheet->BackPage;
			fduplex_test = true;
		}
		else
		{
			MSG("(eng)Duplex front %p %p %p \n", 
			    page, page->sheet->FrontPage, page->sheet->BackPage );
			PendingPrintPageData[1] = MS_Duplex;
			PendingPrintPageData[7] = 0;
			PendingPrintPageData[10] = (uint32_t)page;
			//PendingPrintPageData[11] = (uint32_t)page;
			PendingPrintPageData[11] = (uint32_t)page->sheet->BackPage;
			fduplex_test = false;
		}
	} 
	else 
	{
		MSG("(eng)Simplex \n");
		//PendingPrintPageData[1] = page->temp_InputTray + 0x10;		//sourceTray;	//Tray1
		PendingPrintPageData[1] = page->engTray + 0x10;		//sourceTray;	//Tray1
		//PendingPrintPageData[1] = 0x10;		//MPT
		PendingPrintPageData[7] = 0;
		PendingPrintPageData[10] = (uint32_t)page;
		PendingPrintPageData[11] = (uint32_t)page;
		//PendingPrintPageData[12] = 0;
	}
	PendingPrintPageData[8] = 0;
	
	if(((cFatalOptionCommFail == 1) && ((PendingPrintPageData[1] == MS_Tray2) || (PendingPrintPageData[1] == MS_Tray3)))
	&& (Fuser_Control.EM_State != EMS_OptionResync))
	{
		MSG("(eng)Wait Printing For Resync!!\n");
		fWaitPrintingForResync = SET;
	}
	
	if(page->pMyJob->JobInfo != PreJobInfo)
	{
		if(fCheckJobInfoChange == SET)
		{
			MSG("(eng)@JobInfo changed : %s -> %s @\n", (PreJobInfo==COPYJOB)?"Copy":"Print", (page->pMyJob->JobInfo==COPYJOB)?"Copy":"Print");
			fJobInfoChanged = SET;
		}
	}
	PreJobInfo = (page->pMyJob->JobInfo == COPYJOB)?COPYJOB:PRINTJOB;
	fCheckJobInfoChange = SET;
	
	if((page->pMyJob->JobNum != PreJobNum) && (length != PrePageSize))
	{
		if(fCheckJobNumChange == SET)
		{
			MSG("(eng)@JobID and PaperSize changed@\n");
			fJobIDChanged = SET;
		}
	}
	if((page->pMyJob->JobNum == PreJobNum) && (page->fSourceTrayChg == SET))
	{
		MSG("(eng)@Ignore Prepick@\n");
		fIgnorePrePick = SET;
	}
	
	MSG("(eng)page->fSourceTrayChg = %d\n", page->fSourceTrayChg);
	fCheckJobNumChange = SET;
	PrePageSize = length;
	
	IsPaperLengthNarrow(length);
	
	if((length < NarrowLengthBoundary) || (fWakeupState == TRUE))
		length = length / FEEDMOTORSPEED_L;
	else
		length = length / FEEDMOTORSPEED;
		
#if DUPLEX_SLOW_MODE
	if(cDuplexPrintSlowlyFor15min == CLEAR)
	{
		if((EnvTempStatus_ForDuplexPrint == EnvTemp_HH) && Get_DuplexPrintTime(cProcessCounter))
		{
			cDuplexPrintSlowlyFor15min = cProcessCounter;
			MSG("(eng)cDuplexPrintSlowlyFor15min is SET!! (%d)\n", cDuplexPrintSlowlyFor15min);
			fDebugDuplexPrintSlowlyFor15minClear = TRUE;
		}
	}
#endif
#if DUPLEX_SLOW_MODE2

	if((GetAutoDuplex(page->DUPLEX)) && (page == (void*)page->sheet->BackPage))
	{
		if(fPwsToPrint == CLEAR)
		{
			if(EnvTempStatus_ForDuplexPrint == EnvTemp_HH)
			{
				fDuplex_Slow_mode = SET;
				if(fMotorSpeedMode == MTR_MODE_NORMAL) 	fEnvTempStatusChage = EnvTemp_Slow;
				else 									fEnvTempStatusChage = EnvTemp_Clear;
			}
			else
			{
				fDuplex_Slow_mode = CLEAR;
				if(fMotorSpeedMode == MTR_MODE_QUIET) 	fEnvTempStatusChage = EnvTemp_Fast;
				else 									fEnvTempStatusChage = EnvTemp_Clear;
			}
		}
		else
		{
			fDuplex_Slow_mode = CLEAR;
		}
		MSG("(eng)EnvTempStatus_ForDuplexPrint %d (%d, %d)\n", EnvTempStatus_ForDuplexPrint, fEnvTempStatusChage, fDuplex_Slow_mode);
	}
	
	if(!GetAutoDuplex(page->DUPLEX) && (page->PageCount == 1) && (fPwsToPrint == CLEAR))
	{
		fDuplex_Slow_mode = CLEAR;
		if((fMotorSpeedMode == MTR_MODE_QUIET) && (fPaperLengthIsNarrow == CLEAR))
		{
			MSG("(eng)Simplex MotorspeedUP\n");
			fEnvTempStatusChage = EnvTemp_Fast;
			fSimplexMotorUp = SET;
		}
	}
#endif

#if DUPLEX_SLOW_MODE
	if((Get_MainMotorState() == RUNNING) && (fMotorSpeedMode == MTR_MODE_NORMAL) && (fJobIDChanged == CLEAR)
	 && ((fPaperLengthIsNarrow == TRUE) || ((cDuplexPrintSlowlyFor15min != CLEAR) && (GetAutoDuplex(page->DUPLEX)) && (page == (void*)page->sheet->BackPage))))
#elif DUPLEX_SLOW_MODE2
	if(((Get_MainMotorState() == RUNNING) && (fJobIDChanged == CLEAR) && (((fMotorSpeedMode == MTR_MODE_NORMAL) && (fPaperLengthIsNarrow == TRUE)) || ((fEnvTempStatusChage != EnvTemp_Clear) && (GetAutoDuplex(page->DUPLEX)) && (page == (void*)page->sheet->BackPage)) || (fSimplexMotorUp == SET)))
	|| ((Get_MainMotorState() == RUNNING) && (fJobInfoChanged == SET)))
#else	
	if((Get_MainMotorState() == RUNNING) && (fMotorSpeedMode == MTR_MODE_NORMAL) && (fPaperLengthIsNarrow == TRUE) && (fJobIDChanged == CLEAR))
#endif
	{
		MSG("(Eng)MotorSpeedChagne High->Low or Low->High\n");
		fWaitToChangeMotorSpeed = SET;
		fRequest_New_PageInfo = SET;
	}
	fJobInfoChanged = CLEAR;
#if DUPLEX_SLOW_MODE2
	fSimplexMotorUp = CLEAR;
#endif
#if DUPLEX_SLOW_MODE
	if((cDuplexPrintSlowlyFor15min != CLEAR) && (GetAutoDuplex(page->DUPLEX)) && (page == (void*)page->sheet->BackPage))
	{
		fDuplexPrintSlowlyFor15min = SET;
	}
	else
	{
		fDuplexPrintSlowlyFor15min = CLEAR;
	}	
	MSG("(eng)fDuplexPrintSlowlyFor15min = %d\n", fDuplexPrintSlowlyFor15min);
#endif
#if DUPLEX_SLOW_MODE2
	if((fEnvTempStatusChage == EnvTemp_Slow) && (GetAutoDuplex(page->DUPLEX)) && (page == (void*)page->sheet->BackPage))
	{
		fDuplexPrintSlowlyForServoHH = SET;
	}
	else
	{
		fDuplexPrintSlowlyForServoHH = CLEAR;
	}	
	MSG("(eng)fDuplexPrintSlowlyForServoHH = %d\n", fDuplexPrintSlowlyForServoHH);
#endif
	// 2�� Option Tray�� �̸� Pick �س��� ���� (ppm ���߱� ���ؼ�) ù��� ������ ������ page���� pre pick �� �� �ֵ��� ��.
	// 1�� Job�� Print Mgr���� Last Check �Ǵ� ���� ���� ������ Engine���� page ������ �Ѿ�� (ù���� pre pick ����) 
	//if( (page->PageCount == 1) || (page->PageCount == 2 && GetAutoDuplex(page->DUPLEX)) || (page->fLastPage == 1) )		// Duplex �� ������ ��� (JobMgr�� MSG_JOBEND �κ� �߰� ���� �ʿ�)
	if( (page->PageCount == 1 && !GetAutoDuplex(page->DUPLEX)) || (page->fLastPage == 1) || GetAutoDuplex(page->DUPLEX) || (fPaperLengthIsNarrow == TRUE) || (fIgnorePrePick == SET))		// Simplex �� ������ ��� 
		option_pre_pick = 0;
	else
		option_pre_pick = 0;
		
	if(page->pMyJob->JobNum != PreJobNum)
	{
		fIgnorePrePick = CLEAR;
		MSG("(eng)fIgnorePrePick CLEAR\n");
	}
	PreJobNum = page->pMyJob->JobNum;
	
    PendingPrintPageData[0] = page_count;
	PendingPrintPageData[2] = 0;
	PendingPrintPageData[3] = tmp_type;
	PendingPrintPageData[4] = length;
#if DARKNESS_TEST
	PendingPrintPageData[5] = Darknesslevel;
#else
	PendingPrintPageData[5] = tmpDarknessLevel;
#endif
	PendingPrintPageData[6] = 0;
	PendingPrintPageData[12] = option_pre_pick;
	PendingPrintPageData[13] = page->engEmptyTray + 0x10;
		
	ERR("(eng) !!@@ option_pre_pick <%d> @@!!\n",option_pre_pick);
	
	//ERR("(eng)temp_InputTray = %d\n",page->temp_InputTray);
	//if(page->temp_InputTray >= 4)
	//{
		//page->temp_InputTray = 0x01;
		//ERR("(eng)Force Tray1 !!!\n",page->temp_InputTray);
	//}
	//ERR("MediaType(Eng) = %x\n",tmp_type);
	
	//MSG("(Eng)=========================================(%d)\n", cProcessCounter);
	MSG("== PageCount = %d, JobNum = %d, PageData = %x, Copies = %d ==\n", page->PageCount, page->pMyJob->JobNum, page, page->COPIES);
	MSG("(Eng)Source  = 0x%x\n",PendingPrintPageData[1]);
	//MSG("(Eng)Narrow  = 0x%x\n",PendingPrintPageData[2]);
	//MSG("(Eng)Media  = 0x%x\n",PendingPrintPageData[3]);
	//MSG("(Eng)Length  = %d\n",PendingPrintPageData[4]);
	//MSG("(Eng)Darkness  = 0x%x\n",PendingPrintPageData[5]);
	// MSG("(Eng)TonerSaver  = 0x%x\n",PendingPrintPageData[6]);
	//MSG("(Eng)OutputDest  = 0x%x\n",PendingPrintPageData[7]);
	// MSG("(Eng)cheat_gap  = 0x%x\n",PendingPrintPageData[8]);
	//MSG("(Eng)=========================================\n");
/*	
	if((f2ndDrawerState == CLEAR) && (PendingPrintPageData[1] == MS_Tray2))
	{
		PendingPrintPageData[1] = MS_Tray1;
		MSG("(eng)Source Change = 0x%x (Tray2 is not detected)\n", PendingPrintPageData[1]);
		MSG("(eng)=========================================\n");
	}
	else if((f3ndDrawerState == CLEAR) && (PendingPrintPageData[1] == MS_Tray3))
	{
		PendingPrintPageData[1] = MS_Tray1;
		MSG("(eng)Source Change = 0x%x (Tray3 is not detected)\n", PendingPrintPageData[1]);
		MSG("(eng)=========================================\n");
	}
	else{}
*/	
	fWaitPageCmd = CLEAR;
	
//	if ((engine_state.fcancelrequest == TRUE) && (PendingPrintPageData[1] != MS_Duplex))
	if (engine_state.fcancelrequest == TRUE)
	{
		MSG("(eng)===== ENG: CANCEL THIS PAGE !!! =====\n");
		return false;
	}
//	fPendingPrintPageCmd = SET;
	MSG("(eng) fState_FullStack = %d, PendingPrintPageData[7] = %d\n", fState_FullStack, PendingPrintPageData[7]);
	//if((fState_FullStack == CLEAR) || ((fState_FullStack == SET) && (PendingPrintPageData[7] == OutputDuplexor)))
	if((fState_FullStack == CLEAR) || ((fState_FullStack == SET) && (PendingPrintPageData[1] == MS_Duplex)))
	{
		
		return true;
	}
	else
	{
		return false;
	}
    }
    //MSG("rec page  null \n");
    //SLOG_ERR("rec page  null \n");
    return false;
}

/// wait for job from above to give us a hint for when to start 
///
uint32_t block_page_avail( uint32_t timeout_msec )
{
    if (engine_state.in_job)
        return 100;
    else
    {
        delay_msec_timer(timeout_msec);

        if (engine_state.in_job)
            return 100;
    }
    return 0;
}


/// indicate the page is safe in the output bin, this is forwarded
/// on to the printmgr.
/// ONE_SHOT timer callback.
///
/// pinetree engine logic description:
/// Input sensor is used for page bottom this causes a timer to be fired after the 
/// paper should have passed the Exit sensor, we check the sensor here to determine if 
/// the paper has jammed or if it is sucessfull.
void engine_state_set_page_safe_cb(void *page_asvoid)
{
    MESSAGE message;
    engine_hardware_t *eng_hw = eng_hardware;
    PAGE_DATA *page = page_asvoid;

    // timer must fire after paper would have passed exit sensor.
    if (dec_sensor_poll( eng_hw->paper_exit ))  
    {
      ERR("paper exit jam @ %d\n", engine_get_time_msec());   
	goto fakeit;
        dec_printer_feed_paper(false);
	message.msgType = MSG_STATUSUPDATE;
	slogit( message.param1 = STATUS_PAPER_JAM_OUTPUT ); 
        message.param3 = page_asvoid; //engine_state.page_exiting; 
	engine_state.exit_jam = true;
        SLOG_ERR("EXIT jam \n");
	SYMsgSend(PRINTMGRID, &message);
    } 
    else 
    {
    fakeit:
        // tell print manager the page is safe in the output bin.

	    if (page) {
		    slogit( message.msgType = MSG_PAGEDONE );
		    message.param3 = page; // engine_state.page_exiting;
		    SYMsgSend(PRINTMGRID, &message);
	    }

        // engine_state.page_exiting = 0;
    }
    // engine_state.page_safe_timer = 0;  // only allow 1 page between BOTTOM and EXIT ie 1 between laser and output tray.

}

/// bottom is a sensor that needs a time delay in front of it so that the video actually completes 
/// before the video done message arrives in print/job mgr above us.
/// a timer is started that will send a page is safe in output bin message if no jams occur.
/// tbd: have video tell engine when it is video done then engine can forward this on.
///
void engine_state_set_page_bottom_cb(void *page_asvoid)
{
    engine_hardware_t *eng_hw = eng_hardware;
//    MESSAGE message;

    SLOG_ERR("_cb BOTTOM for page %p \n", page_asvoid);
    if (1 || engine_state.picked_page == 0 ) {
        slogit( dec_laser_set_top_sync( eng_hw->laser, false /* disable hvsync */));
	slogit( dec_enable_vdo_top_sensor( eng_hw->laser, eng_hw->paper_entry, false )); // false for off
	slogit( dec_enable_vdo( eng_hw->laser, false ));
        slogit( pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_IP_DUTY_CYCLE) );
    }

    if (engine_state.video_done == 0 )
    {
        // video is still running we have to cancel fault the page.
      SLOG_ERR("_cb video page is longer than physical paper!\n");   
      ERR("video page is longer than physical paper! %p %p\n", 
	  engine_state.active_page, engine_state.video_done );   
    }

    // tell print manager the page is done.
    //message.msgType = MSG_PAGEVIDEODONE;
    //message.param3 = page_asvoid; // engine_state.active_page;

    engine_state.page_exiting = page_asvoid; // engine_state.active_page; 
    engine_state.active_page = 0;
    engine_state.video_done = 0;
    
    delay_msec_timer_non_blocking(engine_state.bottom_to_safe_delay_msecs,
				  engine_state_set_page_safe_cb,
				  page_asvoid,
				  false /* non-repeating */
				  );
    // SLOG_ERR("page is video done pre msg\n");
    //SYMsgSend(PRINTMGRID, &message);  // try no wait or thread priority to fix 1.7sec delay here.         
    // SLOG_ERR("page is video done post msg\n");
}

void engine_state_set_page_at_bottom_cb( void *page_as_void )
{
    engine_hardware_t *eng_hw = eng_hardware; 

    if (false == dec_sensor_poll( eng_hw->paper_entry )) 
    {
        SLOG_ERR("_cb paper feed jam %s %p \n", __FUNCTION__, page_as_void );   ERR("paper feed jam \n");   
        //dec_printer_feed_paper(false); 
    }
    // else
    {
	/// msecs between sensor and laser
        delay_msec_timer_non_blocking( 154, engine_state_set_page_bottom_cb, page_as_void, false );
    }
}


void printer_set_paper_exit_jam()
{
    ERR("paper exit jam \n"); 
    // send message with engine_state.active_page is jammed
    // send message 

    //gpio_led_on(gpio_led_by_index(1));
    //gpio_led_on(gpio_led_by_index(2));
    ERR("paper jam\n");    
}

//void engine_state_set_paperout( bool out_of_paper )
//{
    //if (engine_state.paper_out != out_of_paper) // changed
    //{
        //engine_state.paper_out = out_of_paper;
	//engine_try_to_clear_errors();
    //}
//}


/// disable pick solenoid
void pick_off_cb( void *cb_data )
{
    gpio_handle_t *pick_pin = (gpio_handle_t*)cb_data;
    
    DBG_ASSERT(pick_pin);
    SLOG_ERR( "_cb disengage the pick solenoid.\n" );
    REL_ASSERT( OK == gpio_write( pick_pin, 0 ) );
}

/// shiloh: main tray pick only.
/// pick paper solenoid starts picking, and auto disables.
void dec_printer_pick_paper( )
{
    REL_ASSERT( OK == gpio_write( eng_hardware->pick_pin, 1 ) );
    delay_msec_timer_non_blocking(
        250, /// msecs to leave solenoid engaged then timer will disengage the solenoid.
        pick_off_cb, eng_hardware->pick_pin, false );
}

/// activate/deactivate paper feed clutch (REGI_CLUTCH)
void dec_printer_feed_paper(bool feed)
{
#ifdef TODO
    REL_ASSERT( OK == gpio_write( eng_hardware->regi_clutch_pin, feed ? 1 : 0) );
#endif
}

/// activate/deactivate THV_EN (transfer voltage)
void dec_printer_enable_thv(bool enable)
{
    REL_ASSERT( OK == gpio_write( eng_hardware->thv_en, enable ? 1 : 0) );
}

void engine_atassert( void )
{
    // fuser and stepper have atassert
    // pwm can't have an atassert "off" varies
    pwm_off(eng_hardware->transfer_voltage);  
    dec_printer_enable_thv(false);
    pwm_off(eng_hardware->charging_voltage);
    pwm_off(eng_hardware->developer_voltage);
    dec_stop_laser_mirror(eng_hardware->laser);
    pwm_off(eng_hardware->main_fan);
    pwm_off(eng_hardware->sub_fan);
    pwm_off(eng_hardware->dev_fan);
    gpio_write( eng_hardware->mainmotor_start, 1 ); // turn off main motor
    gpio_write( eng_hardware->mainmotor_dir, 1 );
	dcmotor_run(eng_hardware->paper_motor, eng_hardware->speed_off);
}

void engine_init_hw(engine_hardware_t* eng_hw)
{
	eng_hw->paper_avail = dec_sensor_by_name("paper_main_tray_sensor");
	
#if MPT_SENSOR_PIN_CHG
	if(fScan_IRQ == TRUE)
	{
		REL_ASSERT(
	        OK ==
	        gpio_open( &eng_hw->paper_avail_mpt_forScanIRQ, GPIO_BANK_F, 7,
	                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
			);
	}
	else
	{
		eng_hw->paper_avail_mpt = dec_sensor_by_name("paper_mpt_tray_sensor");
	}
#else
	eng_hw->paper_avail_mpt = dec_sensor_by_name("paper_mpt_tray_sensor");
#endif
	
    eng_hw->paper_entry = dec_sensor_by_name("paper_entry_sensor"); 
    eng_hw->paper_exit = dec_sensor_by_name("paper_exit_sensor");

    //eng_hw->paper_outbin_full = dec_sensor_by_name("paper_outbin_full_sensor");
    //eng_hw->door_open = dec_sensor_by_name("door_open_sensor");
#if KARA_S
	REL_ASSERT(
		OK ==
		gpio_open( &eng_hw->paper_low, GPIO_BANK_E, 23,
				   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
		);
	REL_ASSERT(
		OK ==
		gpio_open( &eng_hw->paper_high, GPIO_BANK_E, 24,
				   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
		);
	REL_ASSERT(
		OK ==
		gpio_open( &eng_hw->papersize_sw3, GPIO_BANK_E, 25,
				   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
		);
	REL_ASSERT(
		OK ==
		gpio_open( &eng_hw->papersize_sw2, GPIO_BANK_E, 26,
				   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
		);
	REL_ASSERT(
		OK ==
		gpio_open( &eng_hw->papersize_sw1, GPIO_BANK_E, 27,
				   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
		);
	REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->mainmotor_start, GPIO_BANK_D, 14,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 1 )
        );
	REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->mainmotor_dir, GPIO_BANK_D, 15,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 1 )
        );
#endif

    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->paper_outbin_full, GPIO_BANK_F, 21,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->door_open, GPIO_BANK_F, 25,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
	REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->mainfan_lock, GPIO_BANK_F, GPIO_PIN_MAINFAN_LOCK,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->subfan_lock, GPIO_BANK_F, GPIO_PIN_SUBFAN_LOCK,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->devfan_lock, GPIO_BANK_F, GPIO_PIN_DEVFAN_LOCK,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
	REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->tx_sensing, GPIO_BANK_G, GPIO_PIN_TX_SEN,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
	REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->paper_motor_lock, GPIO_BANK_D, GPIO_PIN_MOTOR_LOCK,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->wake_key, GPIO_BANK_F, GPIO_WAKE_KEY,
                   GPIO_DIRECTION_INPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->ui_nreset, GPIO_BANK_F, GPIO_UI_NRESET,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 1 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->sensor_pwr, GPIO_BANK_F, GPIO_SENSOR_PWR,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->pick_pin, GPIO_BANK_G, GPIO_PIN_PICK,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->pick_mpt, GPIO_BANK_G, GPIO_PIN_MPT_PICK,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->duplex_sol, GPIO_BANK_G, GPIO_PIN_DUP_SOL,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->pre_heat_ctrl, GPIO_BANK_F, 15,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
#if TEST_PIN //Test GPIO_E17  R1505
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->GPIO_E17, GPIO_BANK_E, 17,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
#endif
#if TEST_PIN2 //Test GPIO_E18  R1506
    REL_ASSERT(
        OK ==
        gpio_open( &eng_hw->GPIO_E18, GPIO_BANK_E, 18,
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 )
        );
#endif
    
    eng_hw->fuser = dec_fuser_by_name("main_fuser");
    eng_hw->main_fan = pwm_by_name("main_fan");
    eng_hw->sub_fan = pwm_by_name("sub_fan");
    eng_hw->dev_fan = pwm_by_name("dev_fan");
#if !KARA_S_OLD_ENGINE
    eng_hw->paper_motor = dcmotor_by_name("dcmotor_paper_motor");
#endif
    eng_hw->speed_off = dcmotor_speed_by_name("speed_off");
    eng_hw->speed_full = dcmotor_speed_by_name("speed_full");

    eng_hw->laser = dec_laser_by_index(0);


    eng_hw->transfer_voltage = pwm_by_name("pwm_transfer");
    eng_hw->charging_voltage = pwm_by_name("pwm_charge");
    eng_hw->developer_voltage = pwm_by_name("pwm_developer");
#if KARA_S
    eng_hw->mainmotor_voltage = pwm_by_name("main_motor");
#endif
    eng_hw->thermistor = dec_adc_by_name("fuser_temperature");

    REL_ASSERT( 
        OK == 
        gpio_open( &eng_hw->thv_en, GPIO_BANK_THV_EN, GPIO_PIN_THV_EN, 
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 1 ) 
        ); 

/*
    LSPTODO: Shiloh hardware will have this soon.
    REL_ASSERT( 
        OK == 
        gpio_open( &eng_hw->pin_24v_power, GPIO_BANK_G, 1, 
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_LOW, 0 ) 
        );  
    
*/ 
}


/**
 * \brief Get the engine ready to print.  Ramp motors, fans, 
 *        fuser, etc.
 *
 * \param eng_hw - engine hardware instance
 *
 * \return bool - true if ramp successful; false else
 */
bool engine_print_prepare(engine_hardware_t* eng_hw)
{
    uint32_t err;
    uint32_t motor_ramp_delay;

    MSG("==> %s\n", __func__);

    slogit(    pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_HALF) );
#if ZX_ENABLE
    slogit(    dec_fuser_set_temp( eng_hw->fuser, 1 )); // hot
#else
    slogit(    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(2) )); // hot
#endif

    // TODO: Need timeout and error return to handle fuser that won't heat.
    slogit(    dec_fuser_block_till_at_temp( eng_hw->fuser )); 

    motor_ramp_delay = dcmotor_speed_msecs_to_ramp(eng_hw->paper_motor, eng_hw->speed_full);
    motor_ramp_delay = MAX(motor_ramp_delay, 1750); // 1750 is the measured delay from the real unit.
                                                    // not sure if this is needed, but for the time being
                                                    // we will wait the max of the measured vs estimated ramp time



    slogit(    pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_FULL) );
#if ZX_ENABLE
    slogit(    dec_fuser_set_temp( eng_hw->fuser, 1 ) ); // full heat
#else
    slogit(    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ) ); // full heat
#endif
    slogit(    dec_fuser_block_till_at_temp( eng_hw->fuser )); // fuser fail timeout.

    slogit(    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full ));  // motor should be running during laser ramp and high voltage startup
    slogit(    dec_laser_start( eng_hw->laser ));
    slogit(    dec_laser_set_top_sync( eng_hw->laser, false ));
    engine_start_high_voltage(eng_hw);

    slogit(     err = dec_laser_wait( eng_hw->laser ) );
    if ( err )
    {
        engine_state.engine_fatal_error = ENGINE_FATAL_LASER_FAIL;
        ERR(" laser motor has failed\n");
        goto EXIT;
    }
    slogit(     err = dcmotor_wait( eng_hw->paper_motor, motor_ramp_delay )); 
    if ( err ) 
    {
        engine_state.engine_fatal_error = ENGINE_FATAL_PAPER_MOTOR_FAIL;
        ERR("paper motor watchdog, ramp fail\n");
        goto EXIT;
    }

    // after motor at speed run IP voltage for 260ms
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_IP_DUTY_CYCLE);
    delay_msec_timer(260); // or assume the fuser is slower than the motor 


    return true;
EXIT:
    MSG("<== %s = %d\n", __func__, engine_state.engine_fatal_error);
    // fatal engine error 
    engine_print_stop(eng_hw);
    return false;  // error
}

/**
 * \brief Stop engine.  Turn off motors, fans, fuser, high voltage, etc.
 */
void engine_print_stop(engine_hardware_t* eng_hw)
{
    MSG("==> %s\n", __func__);

    engine_stop_high_voltage(eng_hw);
#if ZX_ENABLE
    dec_fuser_set_temp( eng_hw->fuser, 0 ); // hot
#else
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
#endif
    dec_stop_laser_mirror(eng_hw->laser);
#if KARA_S
	pwm_off(eng_hardware->mainmotor_voltage);
#else
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);
#endif
    pwm_off(eng_hw->main_fan);
    MSG("<== %s\n", __func__);
}

void engine_power_enable(engine_hardware_t* eng_hw, bool enable)
{
  /// lsptodo: 
  ///  REL_ASSERT( OK == gpio_write( eng_hardware->pin_24v_power, enable?1:0 ) );
}

/**
 * \brief High voltage startup sequence.
 */
void engine_start_high_voltage(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);


    dec_printer_enable_thv(true);
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_CLEAN_DUTY_CYCLE);
    pwm_run_dutycycle_percent(eng_hw->developer_voltage, DEVELOPER_NO_PRINT_DUTY_CYCLE);

    // switch from no-print to print duty cycle after 170 ms and start charge
    posix_sleep_ms(170);
    pwm_run_dutycycle_percent(eng_hw->developer_voltage, DEVELOPER_PRINT_DUTY_CYCLE);
    pwm_on(eng_hw->charging_voltage);

    SLOG_ERR("<== %s\n", __func__);
}

void engine_stop_high_voltage(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);
    pwm_off(eng_hw->developer_voltage);
    pwm_off(eng_hw->transfer_voltage);
    pwm_off(eng_hw->charging_voltage);
    dec_printer_enable_thv(false);
    SLOG_ERR("<== %s\n", __func__);
}

void engine_ramp_down(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);

    // cleaning voltage for 790ms
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_CLEAN_DUTY_CYCLE);
    posix_sleep_ms(790);

    // ip voltage for 200ms
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_IP_DUTY_CYCLE);
    posix_sleep_ms(200);

    // disable paper motor and wait 400ms with ip voltage on
#if KARA_S
	pwm_off(eng_hardware->mainmotor_voltage);
#else
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);
#endif
    posix_sleep_ms(400);

    engine_stop_high_voltage(eng_hw);

    SLOG_ERR("<== %s\n", __func__);
}

// 
bool engine_print_page(engine_hardware_t* eng_hw)
{
    uint32_t tstart;

    MSG("engine_print_page\n");

    if ( !engine_try_to_clear_errors( ) ) {
	SLOG_ERR( "engine error\n" );
        return false;
    }

    // fuser is hot, motors are turning, laser is ready
    // high voltage is on, so move some paper and tell
    // video to do it's thing
    engine_state.picked_page = engine_state.pick_page;
    tstart = engine_get_time_msec();
    SLOG_ERR("PICK T = 0 @ %d \n", tstart, engine_state.picked_page);

    dec_printer_pick_paper( );
    delay_msec_timer(420); // pick to register roller: 428 = (( 62.5 + 63.5 )) / 294


    video_print_page((PAGE_DATA*)engine_state.picked_page);
	//slogit(  dec_enable_vdo( eng_hw->laser, true ));
    //slogit(  dec_enable_vdo_top_sensor( eng_hw->laser, eng_hw->paper_entry, true )); // false for off
    SLOG_ERR("pre TOP @ %d  top on entry %d\n", engine_get_time_msec()-tstart, dec_sensor_poll( eng_hw->paper_entry ));

    // if (dec_sensor_block_state( eng_hw->paper_entry, false, PAPER_ENTRY_TOP_TIMEOUT ))  // wait for paper TOP'
    if (dec_sensor_block( eng_hw->paper_entry,  1000 ))  // wait for paper TOP
    {
        ERR("paper pick failure \n");       
        SLOG_ERR("paper pick failure\n");       
        return false;
    }

    engine_state.active_page = engine_state.picked_page;
    SLOG_ERR("==> %s active page %#x\n", __func__, engine_state.active_page);
    delay_msec_timer_non_blocking(engine_state.top_to_bottom_delay_msecs,
				  engine_state_set_page_at_bottom_cb,
				  engine_state.active_page,
				  false /* non-repeating */
				  );
    // 294 mm / sec engine speed
    // 80 mm interpage gap 

    //slogit(  dec_enable_vdo( eng_hw->laser, true ));
    
    //slogit(  dec_laser_set_top_sync( eng_hw->laser, true ));  // fake top
    SLOG_ERR("TOP @ %d\n", engine_get_time_msec()-tstart);

    delay_msec_timer(154); // 45.5mm input to transfer / 294 mm/sec  

    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_PRINT_DUTY_CYCLE);
    
    /// todo: input of page size currently LETTER
    /// 
    delay_msec_timer( 584 ) ; // function of page size  (( 11 * 25.4 )  - ( 62.5 + 63.5 + 16.5) + 80)/ 294 - .154
    SLOG_ERR("get next page @ %d\n", engine_get_time_msec()-tstart);
    return true; // ok to pick again
    
}
 


void* engine_ctrl_loop(void* unused)
{  
    engine_hardware_t* eng_hw;
    Uint32 cWatchDogCounter = 0;
    char buffer[128];
	char* pBuffer;
	error_type_t db_retval = FAIL;
	
    //bool page = false;
    //int timeout_10msecs = 10;
    //uint32_t page_avail_in_x_msec = 0;

    SysWaitForInit();

    //uart (option) init
	UartInitialize();
//	MSG("Uart initialize\n");
	uart_recv_handler_register(UCH_OPTION, (UART_RECV_HANDLER *)&rx_option_handler);
//	MSG("UART RECEIVE HANDLER\n");

	watchdog_init(5000); // ms

#if 1	/* check if watchdog disable is set or not in SP */
	int sp_watchdog_disable = 0;
	sp_watchdog_disable = db_get_var_int("CONF_SYSTEM", "WatchdogDisable", &db_retval);
	if(db_retval == FAIL){
		sp_watchdog_disable = 0;    //On fail: watchdog enable
	}
	if(sp_watchdog_disable){
		watchdog_disable();
	}
#endif

    eng_hardware = (engine_hardware_t*)MEM_MALLOC(sizeof(engine_hardware_t));
    REL_ASSERT(eng_hardware != NULL);

    eng_hw = eng_hardware;
    
#if MPT_SENSOR_PIN_CHG
	SelectMachineTypeForScanIRQ();
#endif

    engine_init_hw(eng_hw);

    engine_dbg_init();
    
    atassert( engine_atassert );
    MSG("mech thread init\n");

	gpio_write( eng_hardware->mainmotor_start, 1 ); // turn off main motor
	gpio_write( eng_hardware->mainmotor_dir, 1 );

    video_register_page_done_callback(engine_pagedone_callback);

    pwm_off(eng_hw->main_fan);    // turn off main fan

    dec_fuser_enable_power(true);   // enable 24v power supply

    // TODO: remove this; force state for the moment until the sensor works
    //engine_state_set_paperout( false ); // have paper 

    //slogit(     pwm_run(thv_en, 1));  // cal thv_en
    
    pBuffer = NULL;
	memset(buffer, 0, sizeof(buffer));
    pBuffer = db_get_var("INST_MACHINE", "MachineTLI", &db_retval);
	if(pBuffer)
	{
		snprintf(buffer, sizeof(buffer), pBuffer);
		MEM_FREE_AND_NULL(pBuffer);
		machineType = machineType_check(buffer);
	}
    	
    Engine_Init();

    motorType = Get_MotorType();
    
	SelectGapRingMode();

#if 0
    FILE	*paramFP;
    char white1[80];
    float mag_param[5];
    
    paramFP = fopen("/app/test_pidcontrol.txt", "r");
    if(paramFP < 0)
    {
		MSG("(eng)test_pidcontrol.txt open fail\n");
	}
	else
	{
		fscanf( paramFP, "%s%f%s%f%s%f%s%f%s%f", 
			white1, &mag_param[0], 
			white1, &mag_param[1], 
			white1, &mag_param[2], 
			white1, &mag_param[3], 
			white1, &mag_param[4]);
		fclose(paramFP);
		set_pidcontrol_param(eng_hw->paper_motor, mag_param[0], mag_param[1], mag_param[2], mag_param[3], (int)(mag_param[4])); // pid parm setting.
	}
#else
	set_pidcontrol_param(eng_hw->paper_motor, PID_Kp, PID_Ki, PID_Kd, PID_dt, PID_Target_Quiet, PID_Target_Quiet); // pid parm setting.
#endif

#if EP_PARAM_TEST
	Get_EP_Test_Parameter();
#endif

#if FUSER_PARAM_TEST
	Get_FUSER_Test_Parameter();
#endif

#if DARKNESS_TEST
	Get_Darkness_Parameter();
#endif
/****************************************************************
* Name          	 : IPC Thread
* Author             : SW TEAM2 (Y.J.LEE & I.H.Oh)
* Date First Issued  : 01/Jun/2009 
* Description        : IPC Thread (between Engine and System)
*****************************************************************/	
	int res;
	char msg[] = "KARA S SYS IPC";
	pthread_t 		ipc_thread;
	pthread_attr_t	thread_attr;
	res = pthread_attr_init(&thread_attr);
	if (res!=0)
	{
//		perror("Attribut creation failed");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if (res!=0)
	{
//		perror("Setting detached attribute failed");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_create(&ipc_thread, &thread_attr, System_IPC, (void *)msg);
	if (res!=0)
	{
//		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}
/****************************************************************
* Name          	 : SmartIC Thread
* Author             : SW TEAM2 (Y.J.LEE & Y.J.JANG)
* Date First Issued  : 05/Nov/2009 
* Description        : Tray2 Read Thread 
*****************************************************************/	
#if SECURITY_IC_ATTACHED
	pthread_t 		smartic_thread; /* [KSR032501] for SMC */

	pthread_attr_t	thread_attr_smartic;  /* [KSR032501] for SMC */

	res = pthread_attr_init(&thread_attr_smartic); /* [KSR032501] for SMC */
	if (res!=0)
	{
		printf("[SMC] Attribut creation failed");
		exit(EXIT_FAILURE);
	}
	res = pthread_attr_setdetachstate(&thread_attr_smartic, PTHREAD_CREATE_DETACHED); /* [KSR032501] for SMC */
	if (res!=0)
	{
		printf("[SMC] Setting detached attribute failed");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_create(&smartic_thread, &thread_attr_smartic, SmartIC_Read, (void *)msg); /* [KSR032501] for SMC */
	if (res!=0)
	{
		printf("[SMC] Thread creation failed");
		exit(EXIT_FAILURE);
	}
#endif /* SECURITY_IC_ATTACHED */	

	Set_Fuser_Parameter();
#if ZX_ENABLE
	Get_ZX_Test_Parameter();
#endif

	Set_Engine_Parameter();
	
//	engine_fromDB_set_vertical_magnification_in_lines(mag_param[0]);
	Display_Ver_Info();

    Tray2_Detection_Check();
    usleep(100*1000);
    if(f2ndDrawerState == SET) 	{ f2ndDrawerState_1stdetect = TRUE;		fQuickly_sensor_check_T2 = SET;		fTray2Notify = TRUE;		Option_Tx_Notify(OPTION_T1); }
    usleep(10*1000);
	if(f3ndDrawerState == SET)	{ f3rdDrawerState_1stdetect = TRUE;		fQuickly_sensor_check_T3 = SET;		fTray2Notify = TRUE;		Option_Tx_Notify(OPTION_T2); }
	usleep(10*1000);
    if(f2ndDrawerState == CLEAR)
	{
		fRetryDetectInStandby = SET;
		fOption_1stdetect = SET;
	}
	
	CheckMPF();
	fQuickly_sensor_check_T1 = SET;
	CheckTray1();
	usleep(10*1000);
	Pending_Tray2_Detection_Result();
		
#if SECURITY_IC_ATTACHED
	SmartICDevDrvInit();
	SecurityIC_Check(); /* 20130214 chg by Ryu ( => enginemanager) *//* [KSF051503] */
	fSmartIC_Authentication = CLEAR; /* 20130214 chg by Ryu ( => enginemanager) *//* [KSF051503] */
#endif /* SECURITY_IC_ATTACHED */
	watchdog_service();
    while (1)
	{	
		if (fWaitPageCmd == TRUE) // willing to start a new page
		{
			fPendingPrintPageCmd = printer_get_page( 0 ); // poll for next page 
            if ( !fPendingPrintPageCmd ) {
                posix_sleep_ms(9);
            }
		}
		else
		{
			posix_sleep_ms(9);
		}
		
		CheckIPC();
		EngineApplication(eng_hw);
		
		cWatchDogCounter++;
		if(cWatchDogCounter == 10)
		{
//			MSG("(eng)Current_Temp = %d\n", ChangeVoltageToTemp());
			cWatchDogCounter = 0;
			watchdog_service();
		}
	}
    return 0;

}

void engine_clean(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);
#ifdef TODO
    dec_printer_enable_thv(true);

#if KARA_S
//	Uint16 first_cycle;
//	Uint16 sencond_cycle;
//	first_cycle = (Uint16)(HV_PWM_PER_CONV_MTR * 50);
//	sencond_cycle = (Uint16)(HV_PWM_PERIOD_MTR - first_cycle);

	pwm_change_duty_cycle(eng_hw->mainmotor_voltage, first_motor_cycle, second_motor_cycle);
#else
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
#endif

    delay_msec_timer(10);
    pwm_run(eng_hw->charging_voltage, 1);

    delay_msec_timer(232);

    pwm_run(eng_hw->developer_voltage, 1);

    delay_msec_timer(750);

    dec_printer_enable_thv(false);
    pwm_run(eng_hw->transfer_voltage, 1);

    delay_msec_timer(2008);

    engine_stop_high_voltage(eng_hw);

    delay_msec_timer(250);

#if KARA_S
	pwm_off(eng_hardware->mainmotor_voltage);
#else
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off );
#endif
#endif
    SLOG_ERR("<== %s\n", __func__);
    
    return;
}

#define UNIT_PERIOD 100
void engine_fromDB_set_vertical_magnification_in_lines(int32_t num_lines)
{
#if MOTOR_SPEED_MODE
	MotorSpeedparam = num_lines;	
#else	
	int32_t CHANGED_PWM_PEROID_MTR;
	int32_t CHANGED_PWM_PER_CONV_MTR;
		
	CHANGED_PWM_PEROID_MTR = HV_PWM_PERIOD_MTR_L - (num_lines * UNIT_PERIOD);
	CHANGED_PWM_PER_CONV_MTR = CHANGED_PWM_PEROID_MTR/100;
	
	first_motor_cycle = (Uint16)(CHANGED_PWM_PER_CONV_MTR * 50);
	second_motor_cycle = (Uint16)(CHANGED_PWM_PEROID_MTR - first_motor_cycle);
#endif	
}

#ifdef MARGIN_ADJUST /* @@2013.01.08. By CSPark */
/// Specify the top margin adjustment in lines for a given color plane.  This will
/// be applied as an adjustment to the default top margin.  May be postive or negative.
/// Will be persisted in the nvram.
///
/// \param process_color color plane to adjust
/// \param num_lines Signed absolute margin adjustment in lines.
///
void engine_set_top_margin_adjust_in_lines(COLOR process_color, int16_t num_lines)
{
    engine_nvram.top_margin_adjust[process_color] += num_lines;

#ifdef HAVE_NVRAM
    // persist the value
    nvram_set_var( engine_nvram_handle, &engine_nvram );
#endif
}

/// Read the top margin adjustment in lines for a given color plane.
///
/// \param process_color color plane
///
inline int16_t engine_get_top_margin_adjust_in_lines(COLOR process_color)
{
    return engine_nvram.top_margin_adjust[process_color];
}

#define UINT_TO_SINT_OFFSET	-50
#define MACHINE_ADJUST_FRONT_TOP_VALUE 3
#define MACHINE_ADJUST_BACK_TOP_VALUE 4
int32_t	adjust_front_top_margin = 0;
int32_t adjust_back_top_margin = 0;
int32_t	adjust_copy_front_top_margin = 0;
int32_t adjust_copy_back_top_margin = 0;

void engine_fromDB_set_front_top_margin_adjust_in_lines(int32_t num_lines)
{
	num_lines += MACHINE_ADJUST_FRONT_TOP_VALUE;
	adjust_front_top_margin = (num_lines + UINT_TO_SINT_OFFSET)*2; // UNIT : 0.2mm
}

void engine_fromDB_set_back_top_margin_adjust_in_lines(int32_t num_lines)
{
	num_lines += MACHINE_ADJUST_BACK_TOP_VALUE;
	adjust_back_top_margin = (num_lines + UINT_TO_SINT_OFFSET)*2; // UNIT : 0.2mm
}

void engine_fromDB_set_copy_front_top_margin_in_lines(int32_t num_lines)
{
	adjust_copy_front_top_margin = (num_lines + UINT_TO_SINT_OFFSET)*2; // UNIT : 0.2mm
    MSG("num_lines : %d\t copy adjust : %d\n", num_lines, adjust_copy_front_top_margin);
}

void engine_fromDB_set_copy_back_top_margin_in_lines(int32_t num_lines)
{
	adjust_copy_back_top_margin = (num_lines + UINT_TO_SINT_OFFSET)*2; // UNIT : 0.2mm
    MSG("num_lines : %d\t copy adjust : %d\n", num_lines, adjust_copy_back_top_margin);
}

void engine_fromDB_set_servo_value(int32_t servo_value)
{
	adjust_servo_value_for_duplex_slow_mode = (Uint8)servo_value;
    MSG("servo_value : %d\t servo adjust : %d\n", servo_value, adjust_servo_value_for_duplex_slow_mode);
}

#define GDI_TOP_MARGIN_ADJUST 48 // 4mm : measured
uint32_t engine_get_top_margin_in_lines( PAGE_DATA *page, COLOR process_color )
{
    int32_t top_margin = (int32_t) engine_get_default_top_margin_in_lines(page, process_color);

#if 0	/* CHANGE READ DB : @@2013.01.25. By CSPark */
    //MSG("%s: Color %d default top margin = %d, adjust = %d, adjusted margin = %d", process_color, top_margin, engine_get_top_margin_adjust_in_lines(process_color), top_margin + engine_get_top_margin_adjust_in_lines(process_color));
    top_margin += engine_get_top_margin_adjust_in_lines(process_color);
#else	/* CHANGE READ DB : @@2013.01.25. By CSPark */
    //MSG("%s: Color %d default top margin = %d, adjust = %d, adjusted margin = %d", process_color, top_margin, adjust_top_margin, top_margin + adjust_top_margin);
    ERR("%d %d\n", adjust_front_top_margin, adjust_back_top_margin);

	ERR("PAGE -> Parser Info. : %d\n", page->parser_info);
	if( page->parser_info == 1 )
		top_margin += GDI_TOP_MARGIN_ADJUST;

	if( GetAutoDuplex(page->DUPLEX) )
    {
        if( page == (void*)page->sheet->BackPage )
        {
            top_margin += adjust_front_top_margin;
			if (page->image_output_info.sourceAPP == APPTYPE_COPY) // copy case
			{
				top_margin += adjust_copy_back_top_margin;  // copy back page
			}
        }
        else
        {
            top_margin += adjust_back_top_margin;
			if (page->image_output_info.sourceAPP == APPTYPE_COPY) // copy case
			{
				top_margin += adjust_copy_front_top_margin; // copy front page
			}
        }
    }
    else
    {
        top_margin += adjust_front_top_margin;
		if (page->image_output_info.sourceAPP == APPTYPE_COPY) // copy case
		{
			top_margin += adjust_copy_front_top_margin;
		}
    }
#endif	/* CHANGE READ DB : @@2013.01.25. By CSPark */
    XASSERT(top_margin >= 0, top_margin);

    return (uint32_t) top_margin;
}

/// Specify the side margin adjustment in subpixels for a given video channel.  This will
/// be applied as an adjustment to the default side margin.  May be postive or negative.
/// Will be persisted in the nvram.
///
/// \param process_color color plane to adjust
/// \param num_subpixels Signed absolute margin adjustment in subpixels
///
void engine_set_side_margin_adjust_in_subpixels(uint32_t video_chan, int32_t num_subpixels)
{
    ASSERT(video_chan < NUM_VIDEO_CHAN);
    engine_nvram.side_margin_adjust[video_chan] += num_subpixels;

#ifdef HAVE_NVRAM
    // persist the value
    nvram_set_var( engine_nvram_handle, &engine_nvram );
#endif
}

/// Read the top margin adjustment in lines for a given color plane.
///
/// \param process_color color plane
///
inline int32_t engine_get_side_margin_adjust_in_subpixels(uint32_t channel_number)
{
    return engine_nvram.side_margin_adjust[channel_number];
}

int16_t	adjust_front_side_margin = 0;
int16_t	adjust_back_side_margin = 0;
int16_t adjust_back_side_gap_margin = 0;
int16_t	adjust_copy_front_side_margin = 0;
int16_t	adjust_copy_back_side_margin = 0;

#define MACHINE_ADJUST_FRONT_LEFT_VALUE -6
#define MACHINE_ADJUST_BACK_LEFT_VALUE -6
#define MACHINE_ADJUST_A4_LT_GAP_VALUE -7
#define MACHINE_ADJUST_FRONT_LEFT_140402_SDK 6
#define MACHINE_ADJUST_BACK_LEFT_140402_SDK 3

#define SUB_PIXEL_DENOM 64
#define PIXEL_PER_MM_600DPI 23.62205  // 1mm * 0.03937...inches/mm * 600 DPI
#define SUBPIXEL_ADJUST_VALUE PIXEL_PER_MM_600DPI / 10 * SUB_PIXEL_DENOM
#define	SIDE_OFFSET	30
void engine_fromDB_set_front_side_margin_adjust_in_subpixels(int32_t num_value)
{
	num_value += MACHINE_ADJUST_FRONT_LEFT_VALUE;
	num_value += MACHINE_ADJUST_FRONT_LEFT_140402_SDK;
	adjust_front_side_margin = (num_value + UINT_TO_SINT_OFFSET)*2 + SIDE_OFFSET; // UNIT : 0.2mm
	adjust_front_side_margin *= SUBPIXEL_ADJUST_VALUE;
	MSG("num_value : %d\t adjust : %d\n", num_value, adjust_front_side_margin);
}

void engine_fromDB_set_back_side_margin_adjust_in_subpixels(int32_t num_value)
{
	num_value += MACHINE_ADJUST_BACK_LEFT_VALUE;
	num_value += MACHINE_ADJUST_BACK_LEFT_140402_SDK;
    adjust_back_side_margin = (num_value + UINT_TO_SINT_OFFSET)*2 + SIDE_OFFSET; // UNIT : 0.2mm
    adjust_back_side_margin *= SUBPIXEL_ADJUST_VALUE;
    MSG("num_value : %d\t adjust : %d\n", num_value, adjust_back_side_margin);
}

#define BACKPAGE_MARGIN_OFFSET_A4 -20  // 2mm
#define BACKPAGE_MARGIN_OFFSET_LT -10  // 1mm

void engine_fromDB_set_back_side_gap_margin_in_subpixels(int32_t num_value)
{
	num_value += MACHINE_ADJUST_A4_LT_GAP_VALUE;
    adjust_back_side_gap_margin = (num_value + UINT_TO_SINT_OFFSET)*2; // UNIT : 0.2mm
    adjust_back_side_gap_margin *= SUBPIXEL_ADJUST_VALUE;
    MSG("num_value : %d\t adjust : %d\n", num_value, adjust_back_side_gap_margin);
}

void engine_fromDB_set_copy_front_side_margin_in_subpixels(int32_t num_value)
{
	adjust_copy_front_side_margin = (num_value + UINT_TO_SINT_OFFSET)*2; // UNIT : 0.2mm
    adjust_copy_front_side_margin *= SUBPIXEL_ADJUST_VALUE;
    MSG("num_value : %d\t copy adjust : %d\n", num_value, adjust_copy_front_side_margin);
}

void engine_fromDB_set_copy_back_side_margin_in_subpixels(int32_t num_value)
{
	adjust_copy_back_side_margin = (num_value + UINT_TO_SINT_OFFSET)*2; // UNIT : 0.2mm
    adjust_copy_back_side_margin *= SUBPIXEL_ADJUST_VALUE;
    MSG("num_value : %d\t copy adjust : %d\n", num_value, adjust_copy_back_side_margin);
}

/// left margin adjust for each formal media size (1 = 0.1mm)
#define ADJ_LEFT_MEDIASIZE_A4		-1
#if 1
#define ADJ_LEFT_MEDIASIZE_B5		8
#define ADJ_LEFT_MEDIASIZE_A5		10
#define ADJ_LEFT_MEDIASIZE_B6_JIS	0
#define ADJ_LEFT_MEDIASIZE_A6		11
#define ADJ_LEFT_MEDIASIZE_LEGAL		-5
#define ADJ_LEFT_MEDIASIZE_LETTER		-3
#define ADJ_LEFT_MEDIASIZE_STATEMENT	-6
#define ADJ_LEFT_MEDIASIZE_EXECUTIVE	-7
#define ADJ_LEFT_MEDIASIZE_CUSTOM_85x13	-7	// US Folio
#define ADJ_LEFT_MEDIASIZE_CUSTOM_8x10		2	// Eng Quatro
#define ADJ_LEFT_MEDIASIZE_CUSTOM_825x14	-2	// G. Legal
#define ADJ_LEFT_MEDIASIZE_CUSTOM_8x105		16	// G. Letter
#define ADJ_LEFT_MEDIASIZE_JAPANESE_POSTCARD		9	// 100x148
#define ADJ_LEFT_MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED	11	// 148x200
#define ADJ_LEFT_MEDIASIZE_ENV_9		15
#define ADJ_LEFT_MEDIASIZE_ENV_10		8
#define ADJ_LEFT_MEDIASIZE_ENV_MONARCH	17
#define ADJ_LEFT_MEDIASIZE_ENV_C5		-3
#define ADJ_LEFT_MEDIASIZE_ENV_C6		20
#define ADJ_LEFT_MEDIASIZE_ENV_DL		11

#define ADJ_LEFT_MARGIN_PIXEL_FOR_COPY	-10	// pixels
#endif

uint32_t engine_get_side_margin_in_subpixels( PAGE_DATA *page,
                                              uint32_t channel_number,
                                              uint32_t image_width_in_pixels,
                                              resolution_t page_x_resolution )
{
	int margin_engshift = 0;
	float op_pixel = SUBPIXEL_ADJUST_VALUE;
    uint32_t side_margin = engine_get_default_side_margin_in_subpixels(page, channel_number, image_width_in_pixels, page_x_resolution);
#if 0	/* CHANGE READ DB : @@2013.01.25. By CSPark */
    //MSG("%s: Channel %d default side margin = %d, adjust = %d, adjusted margin = %d", channel_number, side_margin, engine_get_side_margin_adjust_in_subpixels(channel_number), side_margin + engine_get_side_margin_adjust_in_subpixels(channel_number));
    side_margin += engine_get_side_margin_adjust_in_subpixels(channel_number);
#else	/* CHANGE READ DB : @@2013.01.25. By CSPark */
	//MSG("%s: Channel %d default side margin = %d, adjust = %d, adjusted margin = %d", channel_number, side_margin, adjust_side_margin, side_margin + adjust_side_margin);
	MSG("MRG_S : %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
	if( GetAutoDuplex(page->DUPLEX) )
    {
        if( page == (void*)page->sheet->BackPage )
        {
//            MSG("Simplex Page\n");
            side_margin += adjust_front_side_margin;
			MSG("MRG_S : DS %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);

			if(page->PAPER == MEDIASIZE_A4)
				side_margin += (ADJ_LEFT_MEDIASIZE_A4 * SUBPIXEL_ADJUST_VALUE);
			MSG("MRG_S : DS PAPER %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
			
			if (page->image_output_info.sourceAPP == APPTYPE_COPY)
			{
				margin_engshift = ((ADJ_LEFT_MARGIN_PIXEL_FOR_COPY / PIXEL_PER_MM_600DPI * 10) * SUBPIXEL_ADJUST_VALUE);
				side_margin += margin_engshift;
				MSG("MRG_S : DS COPY ADJ %d %d\n", side_margin, page->image_output_info.image_engshift);

				side_margin += adjust_copy_back_side_margin;
				ERR("MRG_S : DS COPY SP %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
			}
        }
        else
        {
            if(page->PAPER == MEDIASIZE_LETTER)
            {
                side_margin += (BACKPAGE_MARGIN_OFFSET_LT * SUBPIXEL_ADJUST_VALUE);
            }
            else if(page->PAPER == MEDIASIZE_A4)
            {
                side_margin += (BACKPAGE_MARGIN_OFFSET_A4 * SUBPIXEL_ADJUST_VALUE);
				MSG("MRG_S : GAP BF %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
				side_margin += adjust_back_side_gap_margin;
				MSG("MRG_S : GAP AF %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
            }
//            MSG("Duplex Page\n");
            side_margin += adjust_back_side_margin;
			MSG("MRG_S : DD %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
			
			if (page->image_output_info.sourceAPP == APPTYPE_COPY)
			{
				margin_engshift = ((ADJ_LEFT_MARGIN_PIXEL_FOR_COPY / PIXEL_PER_MM_600DPI * 10) * SUBPIXEL_ADJUST_VALUE);
				side_margin += margin_engshift;
				MSG("MRG_S : DD COPY ADJ %d %d\n", side_margin, page->image_output_info.image_engshift);

				side_margin += adjust_copy_front_side_margin;
				ERR("MRG_S : DD COPY SP %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
			}

        }
    }
    else
    {
        MSG("Simplex Page\n");
        side_margin += adjust_front_side_margin;
		MSG("MRG_S : SS %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);

		if(page->PAPER == MEDIASIZE_A4)
		{
			side_margin += (ADJ_LEFT_MEDIASIZE_A4 * SUBPIXEL_ADJUST_VALUE);
			MSG("MRG_S : SS PAPER %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
		}
			
		if (page->image_output_info.sourceAPP == APPTYPE_COPY)
		{
			margin_engshift = ((ADJ_LEFT_MARGIN_PIXEL_FOR_COPY / PIXEL_PER_MM_600DPI * 10) * SUBPIXEL_ADJUST_VALUE);
			side_margin += (margin_engshift + (ADJ_LEFT_MEDIASIZE_LETTER * SUBPIXEL_ADJUST_VALUE));
			MSG("MRG_S : SS COPY ADJ %d(%d) %d\n", side_margin, margin_engshift, page->image_output_info.image_engshift);

			side_margin += adjust_copy_front_side_margin;
			ERR("MRG_S : SS COPY SP %d %d\n", side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
		}
		else
		{
			switch ( page->PAPER )
			{
#if 1
				case MEDIASIZE_B5:			side_margin += (ADJ_LEFT_MEDIASIZE_B5 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_A5:			side_margin += (ADJ_LEFT_MEDIASIZE_A5 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_B6_JIS:		side_margin += (ADJ_LEFT_MEDIASIZE_B6_JIS * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_A6:			side_margin += (ADJ_LEFT_MEDIASIZE_A6 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_LEGAL:			side_margin += (ADJ_LEFT_MEDIASIZE_LEGAL * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_LETTER:			side_margin += (ADJ_LEFT_MEDIASIZE_LETTER * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_STATEMENT:	side_margin += (ADJ_LEFT_MEDIASIZE_STATEMENT * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_EXECUTIVE:	side_margin += (ADJ_LEFT_MEDIASIZE_EXECUTIVE * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_CUSTOM_85x13:side_margin += (ADJ_LEFT_MEDIASIZE_CUSTOM_85x13 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_CUSTOM_8x10:	side_margin += (ADJ_LEFT_MEDIASIZE_CUSTOM_8x10 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_CUSTOM_825x14:	side_margin += (ADJ_LEFT_MEDIASIZE_CUSTOM_825x14 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_CUSTOM_8x105:	side_margin += (ADJ_LEFT_MEDIASIZE_CUSTOM_8x105 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_JAPANESE_POSTCARD:	side_margin += (ADJ_LEFT_MEDIASIZE_JAPANESE_POSTCARD * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED:	side_margin += (ADJ_LEFT_MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_ENV_9:		side_margin += (ADJ_LEFT_MEDIASIZE_ENV_9 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_ENV_10:		side_margin += (ADJ_LEFT_MEDIASIZE_ENV_10 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_ENV_MONARCH:	side_margin += (ADJ_LEFT_MEDIASIZE_ENV_MONARCH * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_ENV_C5:		side_margin += (ADJ_LEFT_MEDIASIZE_ENV_C5 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_ENV_C6:		side_margin += (ADJ_LEFT_MEDIASIZE_ENV_C6 * SUBPIXEL_ADJUST_VALUE);	break;
				case MEDIASIZE_ENV_DL:		side_margin += (ADJ_LEFT_MEDIASIZE_ENV_DL * SUBPIXEL_ADJUST_VALUE);	break;
#endif
				default:	break;
			}
			MSG("MRG_S : SS PAPER(%d) %d %d\n", page->PAPER, side_margin, side_margin / SUBPIXEL_ADJUST_VALUE);
		}
    }
    MSG("adjust_side_margin : %d %d\n", adjust_front_side_margin, adjust_back_side_margin);
	MSG("MRG_FINAL : %f\n", side_margin / op_pixel);
//	MSG("MRG_S : %d(%d+%d)\n", side_margin, adjust_front_side_margin, adjust_back_side_margin);
#endif	/* CHANGE READ DB : @@2013.01.25. By CSPark */
    return (uint32_t) side_margin;
}
#else /* MARGIN_ADJUST : @@2013.01.08. By CSPark */

uint32_t engine_get_top_margin_in_lines( PAGE_DATA *page, COLOR process_color )
{
    // mono engine so no modification of margin is needed.
    return engine_get_default_top_margin_in_lines(page, process_color);
}

uint32_t engine_get_side_margin_in_subpixels( PAGE_DATA *page,
                                              uint32_t channel_number,
                                              uint32_t image_width_in_pixels,
                                              resolution_t page_x_resolution )
{
    // mono engine so no modification of margin is needed.
    return engine_get_default_side_margin_in_subpixels(page, channel_number, image_width_in_pixels, page_x_resolution);
}
#endif /* MARGIN_ADJUST : @@2013.01.08. By CSPark */

/**************************************************************************/
/**************************************************************************/
/* DEBUG & TEST                                                           */
/**************************************************************************/
/**************************************************************************/

#ifdef HAVE_CMD
void eng_test_paper_path(engine_hardware_t* eng_hw, bool test_fuser)
{
    uint32_t err;
    uint32_t tstart;
    uint32_t motor_ramp_delay;

    ASSERT(eng_hw != NULL);

    dbg_printf("Let's %s some paper!\n", test_fuser ? "fuse" : "move");

    slogit(    pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_FULL) );

    if(test_fuser)
    {
#if ZX_ENABLE
	    dec_fuser_set_temp( eng_hw->fuser, 1 ); // hot
#else
        dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(1) ); // warm
#endif
        dbg_printf("Wait fuser for fuser warm.\n");
        dec_fuser_block_till_at_temp( eng_hw->fuser );
	// dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(2) );
#if ZX_ENABLE
	    dec_fuser_set_temp( eng_hw->fuser, 1 ); // hot
#else	
        dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ); 
#endif
    }

#if KARA_S
//	Uint16 first_cycle;
//	Uint16 sencond_cycle;
//	first_cycle = (Uint16)(HV_PWM_PER_CONV_MTR * 50);
//	sencond_cycle = (Uint16)(HV_PWM_PERIOD_MTR - first_cycle);

	pwm_change_duty_cycle(eng_hw->mainmotor_voltage, first_motor_cycle, second_motor_cycle);
#else
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
#endif

    delay_msec_timer(1750);

    motor_ramp_delay = dcmotor_speed_msecs_to_ramp(eng_hw->paper_motor, eng_hw->speed_full)*2;
    dbg_printf("Motor ramp delay = %d\n", motor_ramp_delay);
    err = dcmotor_wait( eng_hw->paper_motor, motor_ramp_delay ); /// should be at speed within this time
    if ( err ) 
    {
#if ZX_ENABLE
	    dec_fuser_set_temp( eng_hw->fuser, 1 ); // hot
#else
        dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
#endif
        dbg_printf("paper motor watchdog, jammed = %#x\n", err);
        goto EXIT;
    }

    // dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ); 
    tstart = engine_get_time_msec();
    dbg_printf("PICK T = 0 @ %d\n", tstart);
    dec_printer_pick_paper( );

    if(test_fuser)
    {
        dbg_printf("Wait fuser for fuser hot.\n");
        dec_fuser_block_till_at_temp( eng_hw->fuser );
    }

    //    if (dec_sensor_block_state( eng_hw->paper_entry, true, PAPER_ENTRY_TOP_TIMEOUT ))  // wait for paper TOP
    if (dec_sensor_block_state( eng_hw->paper_entry, true, PAPER_ENTRY_TOP_TIMEOUT ))  // wait for paper TOP
    {
        dbg_printf("paper pick failure \n");       
        goto EXIT;
    }

    dbg_printf("PICK done. FEED @ %d\n", engine_get_time_msec()-tstart);


    // wait for paper entry to go inactive
    if (dec_sensor_block_state( eng_hw->paper_entry, false, PAPER_ENTRY_BOTTOM_TIMEOUT ))  // bottom
    {
        dbg_printf("paper feed jam \n");   
        dec_printer_feed_paper(false);
        goto EXIT;
    }    

    dbg_printf("FEED done. deactivate FEED clutch @ %d\n", engine_get_time_msec()-tstart);


    if( dec_sensor_block( eng_hw->paper_exit, PAPER_EXIT_TOP_TIMEOUT ) )
    {            
        dbg_printf("Exit sensor indicates paper jam at laser \n");
        goto EXIT;
    }

    dbg_printf("F sensor inactive @ %d\n", engine_get_time_msec()-tstart);

    // wait for paper exit sensor to go inactive
    if( dec_sensor_block( eng_hw->paper_exit, PAPER_EXIT_BOTTOM_TIMEOUT ) )
    {            
        dbg_printf("EXIT sensor indicates paper exit jam \n");
    }

    dbg_printf("EXIT done @ %d\n", engine_get_time_msec()-tstart);

    posix_sleep_ms(PAPER_MOTOR_FINISH_TIMEOUT*MILLISEC_PER_TICK);
EXIT:
    /// turn it all off.
#if ZX_ENABLE
    dec_fuser_set_temp( eng_hw->fuser, 1 ); // hot
#else
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
#endif
#if KARA_S
	pwm_off(eng_hardware->mainmotor_voltage);
#else
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);
#endif
    pwm_off(eng_hw->main_fan);

    dbg_printf("Paper Path test done.\n");
}

void eng_test_fuser(engine_hardware_t* eng_hw)
{
    dbg_printf("Start Fuser Test.\n");
#if ZX_ENABLE
    dec_fuser_set_temp( eng_hw->fuser, 1 ); // hot
#else
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(1) ); // pre-heat
#endif

    dbg_printf("Wait fuser for fuser pre-heat.\n");
    dec_fuser_block_till_at_temp( eng_hw->fuser );
    dbg_printf("Pre-heat done\n");
#if ZX_ENABLE
    dec_fuser_set_temp( eng_hw->fuser, 1 ); // hot
#else
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(2) ); // warm
#endif
    delay_msec_timer(1750);
#if ZX_ENABLE
    dec_fuser_set_temp( eng_hw->fuser, 1 ); // hot
#else
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ); // hot
#endif

    dbg_printf("Wait fuser for fuser hot.\n");
    dec_fuser_block_till_at_temp( eng_hw->fuser );
    dbg_printf("Hot done\n");

    delay_msec_timer(1000);

#if ZX_ENABLE
    dec_fuser_set_temp( eng_hw->fuser, 0 ); // off
#else
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
#endif

    dbg_printf("Fuser test done.\n");
}

void dec_printer_pick_test(bool pick)
{
//TODO    REL_ASSERT( OK == gpio_write( eng_hardware->pick_pin, pick?1:0 ) );
}

void dec_printer_feed_test(bool feed)
{
    dec_laser_t *laser;
    dec_sensor_t *paper_entry;

    laser = dec_laser_by_index(0);
    paper_entry = dec_sensor_by_name("paper_entry_sensor"); 

//TODO    REL_ASSERT( OK == gpio_write( eng_hardware->regi_clutch_pin, feed?1:0 ) );

    SLOG_ERR(" vdo dma TOP test \n");
    slogit(  dec_enable_vdo_top_sensor( laser, paper_entry, true )); // false for off
    slogit(  dec_enable_vdo( laser, true ));
}

void engine_debug_usage()
{
    dbg_printf("Engine command options:\n");
    dbg_printf("    test ppath - attempt to run a sheet through the paper path\n");
    dbg_printf("    test ppathf - attempt to run a sheet through the paper path with fuser active\n");
    dbg_printf("    test fuser - ramp fuser to hot, leave hot for 1s, then disable\n");
    dbg_printf("    fan [on|off] - turn main engine fan on or off\n");
    dbg_printf("    power [on|off] - turn 24v power supply on or off\n");
    dbg_printf("    pick [on|off] - engage/disengage paper pick clutch\n");
    dbg_printf("    feed [on|off] - engage/disengage paper feed clutch\n");
    dbg_printf("    clean - run engine cleaning cycle\n");
#ifdef MARGIN_ADJUST /* @@2013.01.08. By CSPark */
    dbg_printf("    topmargin [value] - set top margin for color to value;\n\tcolor may be one of Y|M|C|K\n");
    dbg_printf("    sidemargin [value] - set side margin for chan to value\n");
#endif /* MARGIN_ADJUST : @@2013.01.08. By CSPark */
}

void engine_debug_cmd_err()
{
    dbg_printf("Engine command error or invalid command.\n");
    engine_debug_usage();
}

int engine_debug_callback( int argc, char *argv[] )
{
    engine_hardware_t* eng_hw = eng_hardware;
    int pos = 1;

    if( argc < 2 || str_match( argv[pos], "help", 4 )) 
    {
        engine_debug_usage();
        /* not enough parameters */
        return 0;
    }

    ASSERT(eng_hw != NULL);
/*
    int i;
    dbg_printf("command args %d : ", argc);
    for (i = 0; i < argc; i++)
    {
        dbg_printf("%s ", argv[i]);
    }
    dbg_printf("\n\n");
*/
    if( str_match(argv[pos], "test", 4 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "ppath", 5 ) )
        {
            eng_test_paper_path(eng_hw, false);
        }
        else if(str_match(argv[pos], "ppathf", 6 ) )
        {
            eng_test_paper_path(eng_hw, true);
        }
        else if(str_match(argv[pos], "fuser", 5 ) )
        {
            //eng_test_fuser(eng_hw);
            pos++;
            if(str_match(argv[pos], "target", 6 ) )
            {
				pos++;
				if(str_match(argv[pos], "off", 3 ))
				{
					fDebugTargetTempChange = CLEAR;
					MSG("(eng)change target temperature for debug OFF\n");
				}
				else
				{
					char buf[25];
					strncpy(buf, argv[pos], sizeof(buf));
					DEBUG_TargetTemp = atoi(buf);
					fDebugTargetTempChange = SET;
					MSG("(eng)change target temperature for debug = %d\n", DEBUG_TargetTemp);
				}
			}
			else if(str_match(argv[pos], "standby", 7 ))
			{
				pos++;
				if(str_match(argv[pos], "off", 3 ))
				{
					fDebugStandbyTempChange = CLEAR;
					Set_Debug_Standby_Temp(fDebugStandbyTempChange);
					MSG("(eng)change Standby temperature for debug OFF\n");
				}
				else
				{
					char buf[25];
					strncpy(buf, argv[pos], sizeof(buf));
					DEBUG_StandbyTemp = atoi(buf);
					fDebugStandbyTempChange = SET;
					Set_Debug_Standby_Temp(fDebugStandbyTempChange);
					MSG("(eng)change Standby temperature for debug = %d\n", DEBUG_StandbyTemp);
				}
			}
        }
        else if(str_match(argv[pos], "lami", 4 ) )
        {
            //eng_test_fuser(eng_hw);
            pos++;
            if(str_match(argv[pos], "utoe", 4 ) )
            {
				pos++;
				if(str_match(argv[pos], "zero", 4 ))
				{
					LaminatingSendMsgToEng(MSG_START_LAMINATION);
					MSG("(eng)laminate utoe zero\n");
				}
				else if(str_match(argv[pos], "one", 3 ))
				{
					LaminatingSendMsgToEng(MSG_START_LAMINATING_1PAGE);
					MSG("(eng)laminate utoe one\n");
				}
				else if(str_match(argv[pos], "two", 3 ))
				{
					LaminatingSendMsgToEng(MSG_END_LAMINATION);
					MSG("(eng)laminate utoe two\n");
				}
				else
				{
					MSG("(eng)laminate utoe unknown\n");
				}
			}
			else if(str_match(argv[pos], "etou", 4 ))
			{
				pos++;
				if(str_match(argv[pos], "two", 3 ))
				{
					LaminatingSendMsgToPM(STATUS_LAMINATEINFO_PREHEATING);
					MSG("(eng)laminate etou two\n");
				}
				else if(str_match(argv[pos], "three", 5 ))
				{
					LaminatingSendMsgToPM(STATUS_LAMINATEINFO_PREHEATDONE);
					MSG("(eng)laminate etou three\n");
				}
				else if(str_match(argv[pos], "five", 4 ))
				{
					LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMINATING);
					MSG("(eng)laminate etou five\n");
				}
				else if(str_match(argv[pos], "six", 3 ))
				{
					LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMINATINGDONE);
					MSG("(eng)laminate etou six\n");
				}
				else if(str_match(argv[pos], "seven", 5 ))
				{
					LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMIJOBDONE);
					MSG("(eng)laminate etou seven\n");
				}
				else
				{
					MSG("(eng)laminate etou unknown\n");
				}
			}
        }
        else
        {
            engine_debug_cmd_err();
        }
    }
    else if( str_match(argv[pos], "fan", 3 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_FULL);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            pwm_off(eng_hw->main_fan);
        }
        else
        {
            engine_debug_cmd_err();
        }
    }
    else if( str_match(argv[pos], "power", 5 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            engine_power_enable(eng_hw, true);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            engine_power_enable(eng_hw, false);
        }
        else
        {
            engine_debug_cmd_err();
        }
    }

    else if( str_match(argv[pos], "pick", 4 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            dec_printer_pick_test(true);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            dec_printer_pick_test(false);
        }
        else
        {
            engine_debug_cmd_err();
        }

    }
    else if( str_match(argv[pos], "feed", 4 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            dec_printer_feed_test(true);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            dec_printer_feed_test(false);
        }
        else
        {
            engine_debug_cmd_err();
        }

    }
    else if( str_match(argv[pos], "clean", 5 ) ) 
    {
        engine_clean(eng_hw);
    }
#ifdef MARGIN_ADJUST /* @@2013.01.08. By CSPark */
    else if ( str_match(argv[pos], "topmargin", 9 ))
    {
        COLOR color;
        int16_t lines;
        if(argc < 3)
        {
            cmd_printf("topmargin adjust K = %d\n",
                       engine_get_top_margin_adjust_in_lines(COLOR_BLACK));
            goto DONE;
        }

        color = COLOR_BLACK;
        lines = atoi(argv[2]);
        cmd_printf("Set adjust for color %d to %d\n", color, lines);
        engine_set_top_margin_adjust_in_lines(color, lines);
    }
    else if ( str_match(argv[pos], "sidemargin", 10 ))
    {
        int32_t subpixels;
        int num_chan;
        if(argc < 3)
        {
            cmd_printf("sidemargin adjust:");
            for (num_chan = 0; num_chan < NUM_MONO_VIDEO_CHANNELS; num_chan++)
            {
                cmd_printf(" %d = %d", num_chan, engine_get_side_margin_adjust_in_subpixels(num_chan));
                if(num_chan % 4 == 0 && num_chan > 0)
                {
                    cmd_printf("\n\t");
                }
            }
            cmd_printf("\n");
            goto DONE;
        }

#if 0	/* @@2013.01.24. By CSPark */
        video_chan = atoi(argv[2]);
        subpixels = atoi(argv[3]);
        if (video_chan < NUM_MONO_VIDEO_CHANNELS)
        {
            cmd_printf("Set adjust for chan %d to %d\n", video_chan, subpixels);
            engine_set_side_margin_adjust_in_subpixels(video_chan, subpixels);
        }
        else
        {
            cmd_printf("ERROR: video channel out of range\n");
        }
#else	/* @@2013.01.24. By CSPark */
		subpixels = atoi(argv[2]);
		subpixels *= 128;	/* 0.1mm Unit */
		for (num_chan = 0; num_chan < NUM_MONO_VIDEO_CHANNELS; num_chan++)
        {
			cmd_printf("Set adjust for chan %d to %d\n", num_chan, subpixels);
			engine_set_side_margin_adjust_in_subpixels(num_chan, subpixels);
        }
#endif	/* @@2013.01.24. By CSPark */
    }
#endif /* MARGIN_ADJUST : @@2013.01.08. By CSPark */
	else if( str_match(argv[pos], "version", 7 ))
	{
		MSG("(Eng)SW_ENGINE_VERSION = %s\n", SW_ENGINE_VERSION);
	}
    else
    {
        dbg_printf("Failed to match command %s", argv[pos]);
        engine_debug_cmd_err();
    }
    
DONE:
    return 0;
}
#endif

static int dbg_engine_cancel_job_cb( int argc, char *argv[] )
{
    MESSAGE sys_msg;
    error_type_t err;

    sys_msg.msgType = MSG_CANCELJOB;
    sys_msg.param1 = SYS_REQUSER;
    sys_msg.param2 = 0;
    sys_msg.param3 = 0;

    err = SYMsgSend( SJMID, &sys_msg );
    XASSERT( err==OK, err );
    return 0;
}

void engine_dbg_init()
{
#ifdef HAVE_CMD
    int i_res;
    i_res = cmd_register( "eng", NULL, NULL, NULL, NULL );
    i_res = cmd_register_subcmd( "eng",
				 "main",
				 "pinetree engine debug",
				 NULL,
				 NULL,
				 engine_debug_callback );
    i_res = cmd_register_subcmd( "eng",
				 "dumpmono",
				 "dump mono strip data to supplied path/file name",
				 NULL,
				 NULL,
				 dbg_dump_mono_filename_cb );
    i_res = cmd_register_subcmd( "eng",
				 "dumpgray",
				 "dump gray strip data to supplied path/file name",
				 NULL,
				 NULL,
				 dbg_dump_gray_filename_cb );
    i_res = cmd_register_subcmd( "eng",
				 "cancel",
				 "press the cancel button",
				 NULL,
				 NULL,
				 dbg_engine_cancel_job_cb );

#endif
}


void LaminatingSendMsgToEng(AGMSG msgType)
{
	MESSAGE msg;
	
	msg.msgType = msgType;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;
	SYMsgSend(ENGINEID, &msg);
	
	return;
}

void LaminatingSendMsgToPM(uint32_t parameter1)
{
	MESSAGE msg;
	
	msg.msgType = MSG_STATUSUPDATE;
	msg.param1 = parameter1;
	msg.param2 = STATUS_LAMINATEINFO;
	msg.param3 = 0;
	SYMsgSend(PRINTMGRID, &msg);
	
	if((parameter1 == STATUS_LAMINATEINFO_PREHEATING)
	|| (parameter1 == STATUS_LAMINATEINFO_PREHEATDONE)
	|| (parameter1 == STATUS_LAMINATEINFO_LAMINATING)
	|| (parameter1 == STATUS_LAMINATEINFO_LAMINATINGDONE)
	|| (parameter1 == STATUS_LAMINATEINFO_HEATING)
	|| (parameter1 == STATUS_LAMINATEINFO_JAM_INPUT_DETECT)
	|| (parameter1 == STATUS_LAMINATEINFO_JAM_INPUT_RELEASE))
	{
		current_lamination_status = parameter1;
	}
	
	return;
}
