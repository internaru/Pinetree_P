/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	enginemanager.c					    				
 **	 Revision  :	ver 1.4								    			
 **	 Date	   : 	Dec 21, 2009										
 **	 Author    :	Youngjin Lee		
 +-----------------------------------------------------------------+*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "memAPI.h"
#include "delay_api.h"
#include "logger.h"
#include "debug.h"
#include "utils.h"
#include "print_status_codes.h"
#include "engine_config.h"
#if defined(VIDEO_APP)
#include "video_laser_api.h"
#endif
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif
#include "db_api.h"

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
#include "printvars_api.h"
#include "agprint.h"  // mediasize type etc.
#include "uart.h"
#include "option.h"

#include "pioname.h"
#include "farch.h"
#include "em.h"
#include "SysUI_Interface.h"
#include "SysServiceMgr.h"
#include "ipc.h"
#include "fwVersion.h"

#define DBG_PRFX "engmgr: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING

void Display_Ver_Info(void);

char ENGINE_VERSION[] = {SW_ENGINE_VERSION};
char * get_version_engine(void)
{
	return &ENGINE_VERSION[0];
}

int 	Set_FatalCheckNotify(Uint8);
int 	Set_JamCheckNotify(Uint8);

extern Uint8    Get_MainMotorState(void);
extern int 	Set_SmartICInfoNotify(Uint8, Uint8);
extern int 	Set_MachineCheckNotify(Uint8);
extern int	Set_PrintReadyNotify();
extern int 	Set_CoverOpenNotify(void);
extern int	Set_CoverClosedNotify (void);
extern int 	Set_PaperJamNotify(Uint8, Uint8);			//24Apr09 yjlee
extern int 	Set_SourceEmptyNotify(Uint8, Uint8);				//24Apr09 yjlee
extern int 	Set_MotorStatusNotify(Uint8, Uint32, Uint8, Uint32);
extern int 	Set_IgnoreEarlyStartNotify(void);
extern int 	Set_WakeKeyNotify(void);
extern void dec_printer_enable_thv(bool enable);

extern	void	InitializePageSupervisor(void); 
		/* Clear the parameters to be used for the page supervisor */
		
extern	void	InitializeEPEvaParameter(void) ;
		/* Set default value of the EP parameters for evaluation */

#if FUSER_PARAM_TEST
extern Uint8 f10pagesUSE;
extern Uint8 ftargetTempfor25ppm;

extern Uint8 within10pagesTemp;
extern Uint8 over10pagesTemp;
extern Uint8 numberofbasisofspeed;
extern Uint8 targetTempfor25ppm;
#endif

extern unsigned char OptionTray1Pro;
extern unsigned char OptionTray2Pro;

sp_mode_hardware_test_t hardware_test;

extern Uint8	fWaitPageCmd;
extern uint32_t clear_engine_state(void);

extern Uint8	IsPaperPresentOnMPF(void);
extern Uint8	IsPaperPresentOnTray1(void);
extern Uint8 	IsPaperPresentOnPins(void);
extern Uint8  	IsPaperPresentOnPexs(void);
extern Uint8	IsPaperPresentOnStack(void);

extern void	 	Motor_BWD(void);
extern void	 	Motor_FWD(void);

extern void    	DisableAllOutputPorts(void);
extern void    	StopMainMotor(void);
extern void	FinishPageSupervisor(PageSupervisor *PGSPVSR);

extern void 	init_temperature(void);
extern void Set_Fuser_PreHeating(void);
extern void Set_Fuser_Laminating_Standby(void);
extern Uint8	Get_MainMotorLockState(void);
extern Uint8	Get_MainFanLockState(void);
extern Uint8	Get_SubFanLockState(void);
extern Uint8	Get_DevFanLockState(void);
extern Uint8	Get_XfrServoOutState(void);
extern void 			Read_Fuser_Func(void);
extern void 			Write_Fuser_Func(void);
extern void		MainMotorDelay(void);
extern void 	Fusermanager(void);
extern void		FuserHeaterManager(void); /* [KSF032501] */
extern void 	PageSupervisors(Uint8);
extern void 	Set_Laminate_PGSPVSR_Parameter(Uint8 id);
extern void 	Set_POR_PGSPVSR_Parameter(Uint8 id);
extern PageSupervisor *Get_MatchToSequenceID(Uint8 id);
extern PageSupervisor *Get_TopPageSupervisor(void);
extern PageSupervisor *Get_LastPageSupervisor(void);
extern PageSupervisor *Get_NextPageSupervisor(void);
extern Uint8	Get_LastPageSupervisorDestination(void);
extern Uint8	Get_TopPageSupervisorSequenceID(void);
extern Uint8	Set_PrintPageParameter(Uint8 id);
extern Uint8    Set_MotorRunPageParameter(void);
extern Uint8	HowManyActivePageSupervisor();
extern int 		Set_EngineVersionInfo(void);
extern Uint8 	SmartICInfoNotify(void); /* [KSR032101] for SMC */
extern int 		Set_FullStackNotify(Uint8);
extern int 		full_stack_notify(Uint8);
extern int 		Set_CoolingNotify(Uint8);
extern int 		Set_PowerSaveActiveNotify(void);
extern void 	SecurityIC_Check(void);
extern void		SmartICDevDrvInit(void);/* [KSR032101] for SMC */

extern void		StartMirrorMotor(void);
extern void	StopMirrorMotor(void);
extern void		StartMainMotor(void);
extern void		StopMainMotor(void);
extern Uint8	TurnOnTray1Solenoid(Uint8);
extern Uint8	TurnOffTray1Solenoid(void);

extern Uint8	IsDoorCoverState(void);
extern void		ClearAllPageSupervisor(void);
extern Uint8	ChangePrintPageInfo(void);

extern Uint8 	SecurityIC_Authentication(void);
extern Uint8 	IsWakeKeyState(void);
extern void		MainMotorSpeedChange(int, Uint8);
extern void 	MirrorMotorSpeedChange(int, Uint8);
extern int MotorStartTempSelectForWakeup(void);
extern Uint32	Set_MeasuredMediaSize(Uint8 MediaSource, Uint8 MediaSize);
#if DUPLEX_SLOW_MODE
extern char Clear_DuplexPrintSlowlyFor15min(int time);
#endif
extern void Tray2_Detection_Check(void);
extern void Pending_Tray2_Detection_Result(void);
extern void engine_cancel_page_msg( MESSAGE *msg );

extern void LaminatingSendMsgToEng(AGMSG msgType);
extern void LaminatingSendMsgToPM(uint32_t parameter1);

extern mqd_t lsu_ctrl_mq;

extern Uint8 fcancelpage;
extern Uint8	fPaperIsEmptyBeforeMotorRun;

extern Uint32	SavedRealMediaSizeInMPF;
extern Uint32	SavedRealMediaSizeInTray1;
extern Uint32	SavedRealMediaSizeInTray2;
extern Uint32	SavedRealMediaSizeInTray3;
extern Uint32	SavedRealMediaSizeInDuplex;

extern Uint8	SavedChargePWM;
extern Uint8	SavedDeveloperPWM;
extern Uint8	SavedTransferPWM;
extern char fTray2Notify;
extern char fUartReadEnable;
extern Uint8	fPOR_PageSupervisor_EPCleaning;
extern char fTray1Empty;
extern Uint8 	Tray2_Version_Major;
extern Uint8 	Tray2_Version_Minor;
extern Uint8 	Toner_Used_Percentage;
extern Uint32 	cPageCount5perTrans;

extern Uint8 	fStore_TonerBlockState;
extern Uint8 	fWaitPrintPageCmd;
extern Uint8 cJobEndDotCountWriteFailCnt;

extern int SolenoidOnTimeCount;
extern Uint8 fSolenoidOnOff;
extern int tmp_SolenoidOnTime;
#if DUPLEX_SLOW_MODE
extern int  cDuplexPrintSlowlyFor15min;
extern Uint8  fDuplexPrintSlowlyFor15min;
extern Uint8	EnvTempStatus_ForDuplexPrint;
#endif
#if DUPLEX_SLOW_MODE2
extern Uint8	EnvTempStatus_ForDuplexPrint;
Uint8 fDuplex_Slow_mode;
Uint8 fEnvTempStatusChage;
Uint8 fSimplexMotorUp;
extern Uint8 fDuplexPrintSlowlyForServoHH;
#endif
extern unsigned char  fTray2FeedSensorDetectState;
extern unsigned char  fTray3FeedSensorDetectState;
extern unsigned char OptionPrePickState;

extern Uint8	fMotorSpeedMode;
Uint8	fMotorSpeedChange;

Uint8 Save_emptyTrayStatus;

Uint8 	machineType;
char fTrayOpen;
char fOnlineNotiy;
char fTrayCloseNotify;
Uint8	fPOR_Motor_Run_Slow;
extern Uint8	fPOR_PageSupervisor_Act;

extern Uint32 	PendingPrintPageData[RIPDATA];

extern Uint8	ChangePrintSequence(Uint8 id);
Uint8	fMotorRunWithoutPrintCmd;
Uint8	fMotorRun;

Uint8	fCheckMachineStateBeforeStart;
Uint8	fChangeTargetTemp;

Uint8 f1stTimeCheckTray1;
Uint8 f1stTimeCheckTray2;
Uint8 f1stTimeCheckTray3;
Uint8 LaminatingMode;
#if MPT_SENSOR_PIN_CHG
Uint8 fScan_IRQ;
#endif
Uint8 f24vOnForJogMode;

Uint8 fJob_Toner_Correction_value;
Uint32 PrePageSize;
Uint32 PreJobNum;
Uint32 PreJobInfo;
Uint8 fCheckJobNumChange;
Uint8 fCheckJobInfoChange;
Uint8 fJobIDChanged;
Uint8 fJobInfoChanged;
Uint8 fIgnorePrePick;

Uint8 StartLamination;
Uint8 StartLaminating1page;
Uint8 StopLamination;
Uint8 ResumeLamination;
Uint8 EndLamination;
Uint8 CheckExitGuide;
Uint8 MotorStartForLaminating;
Uint8 RunEngineForLamination;
uint32_t current_lamination_status;
extern Uint8 SecurityIC_Authentication_Laminate(void);

Uint8 	fPowerSave;
Uint8	fPwsToPrint;
Uint8	fMotorRunForPwsToPrint;
Uint8	fPwsToPrint_1stPick;
Uint8	fPwsToPrint_2ndPick;
Uint8	fPwsToPrint_1stPage;
Uint8	fPwsToPrint_2ndPage;
Uint8	fSpeedUpForPWS;
extern Uint8	SelectMotorRunTempForPWStoPrint;

Uint8 f_MSG_USER_ERR_RECOVERY;

uint32_t GetPreHeatingDoneTime(void);
uint32_t GetPreheatingTime_Laminate(uint8_t);
uint32_t mtrstop_count_laminating1;
uint32_t mtrstop_count_laminating2;
uint32_t SelectPreheatingCount;
uint32_t PreHeatingDoneTime;
uint8_t lami_condition;

uint32_t PreHeatingtime_Laminate[3][2][9] =
{
	// condition A											// condition B
	{{ 400,  700, 1100, 1500, 1700, 2000, 2300, 2700, 3000}, { 500,  600,  700, 1000, 1200, 1400, 2100, 2300, 2700}}, // servo < 70
	{{ 400,  500,  800, 1200, 1600, 1900, 2100, 2400, 2800}, { 800, 1100, 1400, 1700, 1900, 2100, 2300, 2500, 2800}}, // 70 <= servo < 110
	{{1100, 1600, 2000, 2500, 3000, 3700, 4500, 5500, 6000}, {1200, 1400, 1600, 1800, 1900, 2100, 2400, 2800, 3500}}  // 110 <= servo
};

uint32_t preheatingTime[3] =
{
	 6000, // servo < 70
	 8000, // 70 <= servo < 110
	17500  // 110 <= servo
};

Uint8 JamCode_Laminating;

Uint8 machineLocation;
Uint8 fEngineTimingCheck;
Uint32	tray1detect_count;
Uint32	tray2detect_count;
Uint32	tray3detect_count;
Uint32	mptdetect_count;
Uint8 	Tray1Online;
Uint8 	Tray2Online;
Uint8   Tray3Online;

Uint8 	ctray1highlowsensorchattering;
Uint8 	ftray1highlowsensorchattering;
Uint8 	fignoresensorchange;
Uint8 	temp_papervol;
Uint8 	pre_papervol;
Uint8 	ctray1sizesensorchattering;
Uint8 	ftray1sizesensorchattering;
Uint8 	fignoresizesensorchange;
Uint8 	temp_papersize;
Uint8 	pre_papersize;
Uint8 fWaitToChangeMotorSpeed;
Uint8 fWaitPrintingForResync;

Uint8 fWakeup;
Uint8 	Tray1OnlineChattering;
Uint8 	Tray2OnlineChattering;
Uint8 	Tray3OnlineChattering;
Uint8 	MPTOnlineChattering;
Uint8 	fFirstTray1LevelDetect;
Uint8 	fFirstTray2LevelDetect;
Uint8 	fFirstTray3LevelDetect;
#if SMART_IC_LOCK_ENABLE
Uint8 ftoner_count_write_for_coveropen;
#endif
#if DUPLEX_SLOW_MODE
Uint8 fDebugDuplexPrintSlowlyFor15minClear;
#endif
Uint8	fJamDetacted1st;
Uint8	fSmartIC_Authentication;
Uint8 fPressWakeupKey;
Uint8	fCoverOpenDebugMsg;
Uint8	fJamDebugTrace;
Uint32	cJamRecoverTimer;
Uint32	MotorRunTimeAfterJam;

int		cPOR_MotorRunStartTime;
Uint8	fPowerOnReset;
Uint8	fClearWaitPrintPageCmd;
Uint8	WakeupTempFromPws;
Uint8	PrintPageCmdTemp;
Uint32  HeatingStartTemperature;
extern int cPORMotorStartCounter;

Uint8 fDebugTargetTempChange;
Uint8 DEBUG_TargetTemp;
Uint8 fDebugStandbyTempChange;
Uint8 DEBUG_StandbyTemp;

STRUCT_TRAY_STATUS Tray_Status;
STRUCT_TRAY_STATUS Temp_Tray_Status;

Uint8 fQuickly_sensor_check_T1;
Uint8 fQuickly_sensor_check_T2;
Uint8 fQuickly_sensor_check_T3;

#if IPG_ENABLE
extern Uint8	fVideo_Print_Enable;
extern Uint8	fReVideoOn;
#endif

#if EP_PARAM_TEST
Uint8	Param_Charger_PWM;
Uint8	Param_Developer_PWM;
Uint8	Param_Transfer_PWM;
Uint8	Param_Servo_VALUE;
Uint8	Param_Target_VALUE;
Uint8	Param_Pick_VALUE;
Uint8	Param_Motor_VALUE;
Uint8	Param_FirstPick_VALUE;
Uint8 	Param_TxfOnTime_VALUE;
Uint8 	Param_TxfOnTime_Low_VALUE;
Uint8 	Param_TxfOffTime_VALUE;
Uint8 	Param_TxfOffTime_Low_VALUE;

Uint8	fParamChargerPWM;
Uint8	fParamDeveloperPWM;
Uint8	fParamTransferPWM;
Uint8	fParamServoVALUE;
Uint8	fParamTargetVALUE;
Uint8	fParamPickVALUE;
Uint8	fParamMotorVALUE;
Uint8	fParamFirstPickVALUE;
Uint8	fParamTxfOnTimeVALUE;
Uint8	fParamTxfOffTimeVALUE;
Uint8 fParamIPGBeforePrinting;
Uint8 fParamIPGDuringPrinting;
Uint8 Param_IPG_Before_Printing;
Uint8 Param_IPG_During_Printing;
#endif
#if DARKNESS_TEST
Uint8 Darknesslevel;
#endif
int		cDuplexPageCount;
int 	cDuplexSlowPrintCount;
int		cErrorStateCount;
Uint8	fDuplexSlowPrint;
Uint8	fDuplexSet;
int		cKeepMotorRunForCooling;
Uint8	fDuplexCooling;
Uint8	ftempEngineRestartCmd;
Struct_Temp_PageMsg		Temp_PageMsg;

Uint8 FanCtrlEnvIdx;
Uint8 JogModeEnvIdx;
Uint8 JogModeTimeIdx;
Uint32 JogModeCountFor24v;
Uint32 JogModeTime[MAX_JOGMODEENV][MAX_JOGMODETIME] =
{
	{  1,   4,   5, 480},
	{ 10, 480, 480, 480},
	{480, 480, 480, 480}
};

extern int MotorSpeedparam;
extern Uint32	NarrowMediaCheck;
extern Uint8	fMotorRun_Timer;

extern Uint8	ThroughputRate;

extern Uint32	SavedPaperPathT3ToPins;
extern Uint32	SavedPaperPathT2ToPins;
extern Uint32	SavedPaperPathT1ToPins;
extern Uint32	SavedPaperPathMPFToPins;
extern Uint8 	fPreRunBeforePrintCmd;

extern Uint8 	fRequested_Power_Saver;
extern Uint8	cMemoryWrite_Count; /* [KSR032101] for SMC */
extern Uint8	fMemoryWrite; /* [KSR032101] for SMC */
extern Uint8 	fSmartICWrite_PageCount; /* [KSR032101] for SMC */
extern Uint8 	fSmartICWrite_DrumPageCount; /* [KSR040101] */
extern Uint8 	fTonerLowMsgPending;
extern Uint8	fFullStack_Error;
extern Uint32 	PageNumVideoDoneFail;

extern Uint8	TransferBiasPWMForServo;
extern Uint8	EnvTempStatus;
extern Uint8 	fNewTonerCheck;
extern Uint8 	fStartMainMotor;
extern unsigned char fRetryDetectInStandby;
Uint8 fDebugOptionCommFail;
Uint8 fDebugOptionCommFailForDisconnection;
Uint8 cOptionCommFail;
Uint8 cFatalOptionCommFail;
extern char SendDataToOption1;
extern char SendDataToOption2;
extern char fOption1Disconnection;
extern char fOption2Disconnection;

extern Uint8 fOld_SmartIC;
extern Uint8 fTray2IsNotifying;
//extern Uint16	cLowFanCounter;
//extern Uint8	fPowerSaveMode;

//extern FUSER_PARAM Fuser_Control;
extern Uint8 fPaperLengthIsNarrow;
extern int Engine_Parameter[NumberOfEngineParameter];
extern Uint16	CodeOfMachineCheck;

extern int 	i2c_dev;
extern Uint8 SC_State;
extern engine_state_t engine_state;

void	InitializeEngineManager(void);
void	InitializeEngineVariable(void);

int Get_MotorType();
int motorType;

#if PICK_THD
static pthread_t dcmtr_thread_id;
//static pthread_t page_ctrl_thread_id;
#define PRINT_ENGINE_STACK_SIZE POSIX_MIN_STACK_SIZE
static ALIGN8 UINT8 dcmtr_stack[PRINT_ENGINE_STACK_SIZE];
mqd_t dcmtr_ctrl_mq;
static void* dcmtr_ctrl_loop(void* unused);
#define NUM_PRINT_ENGINE_MESSAGES 4 
#endif
#if UART_TX_THD
static pthread_t uarttx_thread_id;
static ALIGN8 UINT8 uarttx_stack[PRINT_ENGINE_STACK_SIZE];
mqd_t uarttx_ctrl_mq;
static void* uarttx_ctrl_loop(void* unused);
#endif

void	EngineApplication(engine_hardware_t* eng_hw);
void	EngineManager(void);

void	ErrorManager(void);

void	EM_EngineRestart(void);			/* Wait an Engine Restart Command */

void	EM_StartWarmup(void);			/* Wait Start Warm up from RIP */
void	EM_StartPOR(void);				/* Wait parameters and Start POR Sq.  */
void	EM_PowerOnResetState(void);		/* During POR Sequence */
void	EM_POR_Standby(void);			/* During POR Warm up for Fuser */	

void	EM_PrintReady(void);			/* Come up to Printer Idle Mode and Wait Start Command */
void	EM_Standby(void);				/* Wait for Print Page Command */
void	EM_PowerSave(void);
void    EM_JogMode(void);
void	EM_Printing(void);				/* Set Print Temperature and Now Prining*/
void	EM_CheckExitSensorForLaminatingStart(void);
void	EM_CheckExitSensorForLaminatingEnd(void);
void	EM_Laminating_Servo(void);
void	EM_PreHeating(void);
void	EM_Laminating(void);
void	EM_Laminating_Standby(void);
void	EM_Laminating_End(void);
void	EM_Laminating_Error(void);
void	EM_Laminating_Recovery(void);
void	EM_Laminating_CloseCounter(void);
void	EM_Laminating_CloseDoorCover(void);
void 	EM_ExitLastPage(void);			/* Carry a last printed page to exit */

void 	EM_EPCleaning(void);			/* End of Page Sequence (After EP Cleaning) */
void 	EM_PrinterStop(void);
void	EM_OpenDoorCover(void);
void	EM_CloseCounter(void);
void	EM_CloseDoorCover(void);

void	EM_RecoverPaperJam(void);
void	EM_RecoverPaperEmpty(void);
void	EM_RecoverFullStack(void);
void	EM_RecoverNoStopError(void);
void	EM_RecoverError(void);

void	EM_FatalError(void);
void	EM_OptionResync(void);
void	EM_SCError(void);
void	EM_Diagonostic(void);
void	EM_NonRecoveryError(void);

void	EM(void);


Uint8	PowerSaveControl(void);
void	StopAllPortsInEngine(void);
Uint8	StartPrintSequence(Uint8 id);
Uint8	Check_MachineStateBeforeStart(void);

void	RecoverError(void);
Uint8	CheckDoorCover(void);
Uint8	CheckDoorCover_Laminating(void);
void	CheckTray1(void);
void	CheckMPF(void);
void	CheckTray2(void);
void 	Sensor_Power_On(void);
void 	Sensor_Power_Off(void);
void 	Pre_Heat_Off(void);
void 	StandbyModeSelect(void); /* [KFS051502] */
void 	PS_24vOn(char);
void	PS_24vOff(char);
int machineType_check(char*);
char TrayCloseRecoveryEnable(char);

void PowerKeyPressed(int keyType);
extern Uint32 hv_pwm_period_mtr;
void SelectGapRingMode(void);
#if MPT_SENSOR_PIN_CHG
void SelectMachineTypeForScanIRQ(void);
#endif
#if EP_PARAM_TEST
void Get_EP_Test_Parameter(void);
#endif
#if DARKNESS_TEST
void Get_Darkness_Parameter(void);
#endif
Uint8	Set_FuserTemp(Uint32 Source, Uint32 OutputDestination, Uint32 PaperData, Uint32 PageID, Uint8 PowersaveToPrint);

void Set_Engine_Parameter(void);
void WakeupProcess(void);

void sp_mode_hardware_test_init(void);
void Check_Sensor_Status(void);
void Sensor_Test_Off(void);
void Get_Init_Sensor_Status(void);
void Get_Present_Sensor_Status(void);
void Solenoid_Test(void);
void All_Solenoid_On(void);
void All_Solenoid_Off(void);
extern Uint8	TurnOnMPFSolenoid(void);
extern Uint8	TurnOffMPFSolenoid(void);
extern Uint8 	TurnOnDuplexSolenoid(void);
extern Uint8 	TurnOffDuplexSolenoid(void);

extern void watchdog_service(void);

#ifdef SMART_IC_TONER	/* @@2013.01.10. By CSPark */
void WatchDogClear(unsigned int);
void WatchDogInit(unsigned int);
void WatchDogStop(int);
void Sensor_Power_Ctrl(char On);

Uint8	fNewTonerCheck;
Uint8 	Machine_TLI;
Uint32	cDotCount;
Uint32 cPageCount;
int		SmartICSeed;
Struct_Fuser_Control FuserControlMsgSet;

Uint32 cDrumPageCount; /* [KSR040101] */
Uint32 cDrumDotCount; /* [KSR040101] */
Uint32 cDrumPageCount5perTrans; /* [KSR040101] */
Uint32 cDrumRest; /* [KSR040101] */
#endif	/* SMART_IC_TONER : @@2013.01.10. By CSPark */

int	Detect_OverSensorAtWarmup(void);

ST_FLAG 	Flags;
ST_FLAG 	TrayFlags;
ST_FLAG		CoverError;
ST_FLAG		CoverState;
ST_FLAG 	PaperEmpty;
ST_FLAG		TempPaperEmpty;
ST_FLAG 	EngineFatalError1;
ST_FLAG 	EngineFatalFuserError;
ST_FLAG		PaperJam;
ST_FLAG		TempPaperJam;
ST_FLAG 	AttachedDevice;
ST_FLAG 	CommandReceiveFlag;
ST_FLAG		IpcFlag1;
ST_FLAG		IpcFlag2;
ST_FLAG		IpcFlag3;
//Struct_Fuser_Control Fuser_Control;

Uint8	f2ndDrawerState;
Uint8	f3ndDrawerState;
Uint8	f2ndDrawerState_1stdetect;
Uint8	f3rdDrawerState_1stdetect;
Uint8	fOption_1stdetect;

Uint8 SP_Old_PinSenorStatus;
Uint8 SP_Old_PexSenorStatus;
Uint8 SP_Old_MPTSenorStatus;
Uint8 SP_Old_CoverStatus;

Uint8 SP_New_PinSenorStatus;
Uint8 SP_New_PexSenorStatus;
Uint8 SP_New_MPTSenorStatus;
Uint8 SP_New_CoverStatus;

Uint8 fSensorTest;
BITSTATUS fHardwareTest;

Uint8	fEngineStateFull;

Uint8   fMainMotorJogMode;
Uint32  MainMotorJogCounter;
Uint32   MainMotor_1MinJogCounter;
Uint8	fMotorRunFromPowerSave;

Uint8	Pre_EM_State;
Uint16	EM_StateCounter;

Uint32	cFullStackDetacted;
Uint32	cFullStackReleased;
Uint32	cFullStackErrCnt;
Uint8	fState_FullStack;
Uint8	cTray2PaperCheckBeforePickUp;

char f24vOn;
Uint8 fSensorPowerOffForPWS;
Uint32 cSensorPowerOffCounterForPWS;

Uint32	cPowerSaveWaitCnt;
Uint32	cWakeKey;
Uint32	cWakeKeyCheckPeriod;
Uint8	fWaitWakeKeyRelease;
Uint8	fWakeKey_Onetime;;
Uint8	fWakeKeyDetactForPowerSave;

Boolean	bePrintFromPowerSave;	
Boolean	EarlyStartCmd;			
Boolean PrintPageCmd;
Boolean ColdWakeUpCmd;
Boolean fColdWakeupState;

Uint32 	cPreRunTimeoutCounter;
Uint8	JogModeType;
Uint8	fJogModeOn;
Uint8	fCoolingState;
Uint8	fHSync_Detacted;

int		cPowerSaveStart;
long	oldTimeSec;
long	oldTimeuSec;

extern Uint16	MeasuredPaperLength;
extern Uint16	MeasuredPaperLengthInTray1;
extern Uint16	MeasuredPaperLengthInTray2;
extern Uint16	MeasuredPaperLengthInTray3;
extern Uint8 	fSC_Writefault;
extern Uint8	CodeOfJamStatus;
extern Uint8	fStaticMotorRunTime;

extern int		Duplex1stNorPageNum;
extern int		Duplex2ndNorPageNum;

extern Uint8 fSmartICWrite_DotCount; /* [KSR032101] for SMC */
extern Uint8 fSmartICWrite_PageCountInRun;
extern Uint8 fSmartICWrite_PageCountFailInRun;
extern Uint8 fSmartICWrite_DotCountInRun;
extern Uint8 fSmartICWrite_DotCountFailInRun; /* [KSR032101] for SMC */
extern Uint8 fSmartICWrite_PageCountInRun; /* [KSR040101] */
extern Uint8 fSmartICWrite_PageCountFailInRun; /* [KSR040101] */

extern Uint32 ChangeVoltageToTemp(void);
extern Struct_Fuser_Control Fuser_Control;
extern Struct_Fuser_Control Pre_Fuser_Control;
extern engine_hardware_t *eng_hardware;

extern Uint8 fSmartICWrite_DrumDotCount; /* [KSR040101]-start */
extern Uint8 fSmartICWrite_DrumDotCountInRun;
extern Uint8 fSmartICWrite_DrumDotCountFailInRun;
extern Uint8 fSmartICWrite_DrumPageCountInRun;
extern Uint8 fSmartICWrite_DrumPageCountFailInRun; /* [KSR040101]-end */

Uint32	CoolingTimeTable[TempStatusNumber][PageRange] = {
{     0,      0,     0},		//	(servo < 62)	H
{     0,      0,     0},		//	(servo < 70)	HM
{     0,      0,     0},		//	(servo < 95)	M
{     0,      0,     0},		//	(servo < 110)	ML
{     0,      0,     0},		//	(servo < 180)	L
{     0,      0,     0},		//	(servo >= 180)	DL
};

#if ABBA2
Uint8	FuserTemperatuerDuplextbl[] = {	
			/*	smooth,		normal,		rough,		dummy,	*/
/* duplex paper */
  /* light */	Temp_155C,	Temp_165C,	Temp_175C,	Temp_165C,
  /* normal */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_170C,
  /* heavy */	Temp_180C,	Temp_190C,	Temp_205C,	Temp_190C, 
  /* dummy */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_170C,
};
#else
Uint8	FuserTemperatuerDuplextbl[] = {	
			/*	smooth,		normal,		rough,		dummy,	*/
/* duplex paper */
  /* light */	Temp_155C,	Temp_165C,	Temp_175C,	Temp_165C,
  /* normal */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_170C,
  /* heavy */	Temp_180C,	Temp_190C,	Temp_205C,	Temp_190C, 
  /* dummy */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_170C,
};
#endif
Uint8	FuserTemperatuerDuplextblForQuiet[] = {	
			/*	smooth,		normal,		rough,		dummy,	*/
/* duplex paper */
  /* light */	Temp_160C,	Temp_160C,	Temp_180C,	Temp_165C,
  /* normal */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_170C,
  /* heavy */	Temp_160C,	Temp_180C,	Temp_180C,	Temp_190C, 
  /* dummy */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_170C,
};
#if ABBA2
Uint8	FuserTemperaturetbl[] = {	
	/*	smooth,		normal,		rough,		dummy,	*/
/* paper *//* [KSF040101] */
  /* light */	Temp_165C,	Temp_175C,	Temp_185C,	Temp_175C,
  /* normal */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C, /* 7 */  
  /* heavy */	Temp_190C,	Temp_200C,	Temp_215C,	Temp_200C,
  /* dummy */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C,

/* label */
  /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 19 */
  /* normal */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, 
  /* heavy */	Temp_210C,	Temp_210C,	Temp_210C,	Temp_210C,
  /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */

/* envelope */
  /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 35*/
  /* normal */	Temp_185C,	Temp_185C,	Temp_185C,	Temp_185C,
  /* heavy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C,
  /* dummy */	Temp_170C,	Temp_170C,	Temp_170C,	Temp_170C, /* 47*/

/* card stock */
  /* light */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_205C,
  /* normal */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, /* 55 */
  /* heavy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C,
  /* dummy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, 

/* transparency */
  /* light */	Temp_160C,	Temp_160C,	Temp_160C,	Temp_160C, /* 67 */
  /* normal */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
  /* heavy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
  /* dummy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,	/* 79 */

/* Vinyl label */
  /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 19 */
  /* normal */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, 
  /* heavy */	Temp_210C,	Temp_210C,	Temp_210C,	Temp_210C,
  /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */
  
/* Post Card */
  /* light */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_205C,
  /* normal */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, /* 55 */
  /* heavy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C,
  /* dummy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, 

/* Recycled */
  /* light */	Temp_165C,	Temp_175C,	Temp_185C,	Temp_175C, /* 67 */  
  /* normal */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C,
  /* heavy */	Temp_190C,	Temp_200C,	Temp_215C,	Temp_200C,
  /* dummy */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C,
} ;
#else
Uint8	FuserTemperaturetbl[] = {	
	/*	smooth,		normal,		rough,		dummy,	*/
/* paper *//* [KSF040101] */
  /* light */	Temp_165C,	Temp_175C,	Temp_190C,	Temp_175C,
  /* normal */	Temp_170C,	Temp_180C,	Temp_205C,	Temp_180C, /* 7 */  
  /* heavy */	Temp_195C,	Temp_200C,	Temp_215C,	Temp_200C,
  /* dummy */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C,

/* label */
  /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 19 */
  /* normal */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, 
  /* heavy */	Temp_210C,	Temp_210C,	Temp_210C,	Temp_210C,
  /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */

/* envelope */
  /* light */	Temp_180C,	Temp_180C,	Temp_190C,	Temp_180C, /* 35*/
  /* normal */	Temp_185C,	Temp_195C,	Temp_195C,	Temp_185C,
  /* heavy */	Temp_195C,	Temp_205C,	Temp_205C,	Temp_195C,
  /* dummy */	Temp_170C,	Temp_170C,	Temp_170C,	Temp_170C, /* 47*/

/* card stock */
  /* light */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_205C,
  /* normal */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, /* 55 */
  /* heavy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C,
  /* dummy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, 

/* transparency */
  /* light */	Temp_160C,	Temp_160C,	Temp_160C,	Temp_160C, /* 67 */
  /* normal */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
  /* heavy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
  /* dummy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,	/* 79 */

/* Vinyl label */
  /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 19 */
  /* normal */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, 
  /* heavy */	Temp_210C,	Temp_210C,	Temp_210C,	Temp_210C,
  /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */
  
/* Post Card */
  /* light */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_205C,
  /* normal */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, /* 55 */
  /* heavy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C,
  /* dummy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, 

/* Recycled */
  /* light */	Temp_165C,	Temp_175C,	Temp_190C,	Temp_175C, /* 67 */  
  /* normal */	Temp_170C,	Temp_180C,	Temp_205C,	Temp_180C,
  /* heavy */	Temp_195C,	Temp_200C,	Temp_215C,	Temp_200C,
  /* dummy */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C,
} ;
#endif
Uint8	FuserTemperaturetblForQuiet[] = {	
	/*	smooth,		normal,		rough,		dummy,	*/
/* paper */
  /* light */	Temp_160C,	Temp_160C,	Temp_180C,	Temp_175C,
  /* normal */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_180C, /* 7 */  
  /* heavy */	Temp_160C,	Temp_180C,	Temp_180C,	Temp_200C,
  /* dummy */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C,

/* label */
  /* light */	Temp_165C,	Temp_165C,	Temp_185C,	Temp_180C, /* 19 */
  /* normal */	Temp_165C,	Temp_175C,	Temp_185C,	Temp_195C, 
  /* heavy */	Temp_165C,	Temp_185C,	Temp_185C,	Temp_210C,
  /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */

/* envelope */
  /* light */	Temp_180C,	Temp_180C,	Temp_190C,	Temp_180C, /* 35*/
  /* normal */	Temp_185C,	Temp_195C,	Temp_195C,	Temp_185C,
  /* heavy */	Temp_195C,	Temp_205C,	Temp_205C,	Temp_195C,
  /* dummy */	Temp_170C,	Temp_170C,	Temp_170C,	Temp_170C, /* 47*/

/* card stock */
  /* light */	Temp_160C,	Temp_160C,	Temp_190C,	Temp_205C,
  /* normal */	Temp_160C,	Temp_180C,	Temp_190C,	Temp_220C, /* 55 */
  /* heavy */	Temp_160C,	Temp_190C,	Temp_190C,	Temp_220C,
  /* dummy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, 

/* transparency */
  /* light */	Temp_140C,	Temp_140C,	Temp_160C,	Temp_160C, /* 67 */
  /* normal */	Temp_140C,	Temp_150C,	Temp_160C,	Temp_175C,
  /* heavy */	Temp_140C,	Temp_160C,	Temp_160C,	Temp_175C,
  /* dummy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,	/* 79 */

/* Vinyl label */
  /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 19 */
  /* normal */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, 
  /* heavy */	Temp_210C,	Temp_210C,	Temp_210C,	Temp_210C,
  /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */
  
/* Post Card */
  /* light */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_205C,
  /* normal */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, /* 55 */
  /* heavy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C,
  /* dummy */	Temp_220C,	Temp_220C,	Temp_220C,	Temp_220C, 

/* Recycled */
  /* light */	Temp_160C,	Temp_160C,	Temp_180C,	Temp_175C, /* 67 */ 
  /* normal */	Temp_160C,	Temp_170C,	Temp_180C,	Temp_180C, 
  /* heavy */	Temp_160C,	Temp_180C,	Temp_180C,	Temp_200C,
  /* dummy */	Temp_170C,	Temp_180C,	Temp_190C,	Temp_180C,
} ;
//~ #else
//~ 
//~ Uint8	FuserTemperatuerDuplextbl[] = {	
			//~ /*	smooth,		normal,		rough,		dummy,	*/
//~ /* duplex paper */
  //~ /* light */	Temp_165C,	Temp_165C,	Temp_165C,	Temp_165C,
  //~ /* normal */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
  //~ /* heavy */	Temp_195C,	Temp_195C,	Temp_205C,	Temp_195C, 
  //~ /* dummy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
//~ };
//~ 
//~ Uint8	FuserTemperaturetbl[] = {	
	//~ /*	smooth,		normal,		rough,		dummy,	*/
//~ /* paper */
  //~ /* light */	Temp_165C,	Temp_165C,	Temp_165C,	Temp_165C,
  //~ /* normal */	Temp_185C,	Temp_185C,	Temp_185C,	Temp_185C, /* 7 */
  //~ /* heavy */	Temp_195C,	Temp_195C,	Temp_215C,	Temp_215C,
  //~ /* dummy */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C,
//~ 
//~ /* label */
  //~ /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 19 */
  //~ /* normal */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, 
  //~ /* heavy */	Temp_210C,	Temp_210C,	Temp_210C,	Temp_210C,
  //~ /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */
//~ 
//~ /* envelope */
  //~ /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 35*/
  //~ /* normal */	Temp_185C,	Temp_185C,	Temp_185C,	Temp_185C,
  //~ /* heavy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C,
  //~ /* dummy */	Temp_170C,	Temp_170C,	Temp_170C,	Temp_170C, /* 47*/
//~ 
//~ /* card stock */
  //~ /* light */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_190C,
  //~ /* normal */	Temp_215C,	Temp_220C,	Temp_220C,	Temp_205C, /* 55 */
  //~ /* heavy */	Temp_220C,	Temp_220C,	Temp_225C,	Temp_215C,
  //~ /* dummy */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_205C, 
//~ 
//~ /* transparency */
  //~ /* light */	Temp_160C,	Temp_160C,	Temp_160C,	Temp_160C, /* 67 */
  //~ /* normal */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
  //~ /* heavy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,
  //~ /* dummy */	Temp_175C,	Temp_175C,	Temp_175C,	Temp_175C,	/* 79 */
//~ 
//~ /* Vinyl label */
  //~ /* light */	Temp_180C,	Temp_180C,	Temp_180C,	Temp_180C, /* 19 */
  //~ /* normal */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, 
  //~ /* heavy */	Temp_210C,	Temp_210C,	Temp_210C,	Temp_210C,
  //~ /* dummy */	Temp_195C,	Temp_195C,	Temp_195C,	Temp_195C, /* 31 */
  //~ 
//~ /* Post Card */
  //~ /* light */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_190C,
  //~ /* normal */	Temp_215C,	Temp_220C,	Temp_220C,	Temp_205C, /* 55 */
  //~ /* heavy */	Temp_220C,	Temp_220C,	Temp_225C,	Temp_215C,
  //~ /* dummy */	Temp_205C,	Temp_205C,	Temp_205C,	Temp_205C,   
//~ } ;
//~ #endif

int Fuser_Setting_Parameters[NumberOfFuserParameter];
extern Uint8	fStartFromPowerSaveMotorRun;	
extern Uint8	SavedSourceInPrePage;

int 	cProcessCounter;

#ifdef SMART_IC_TONER	/* @@2013.01.10. By CSPark */
void WatchDogClear(unsigned int time)
{
#if 0
	ioctl(DEV_Engine_Timer, CMD_WATCHDOG_CLEAR | IOCTL_MAGIC, &time);
#endif
}

void WatchDogInit(unsigned int time)
{
#if 0
#if	ENG_TRACE
	printf("## WatchDogInit ##\n");
#endif
	ioctl(DEV_Engine_Timer, CMD_WATCHDOG_INIT | IOCTL_MAGIC, &time);
#endif
}

void WatchDogStop(int sig)
{
#if 0
	ioctl(DEV_Engine_Timer, CMD_WATCHDOG_STOP | IOCTL_MAGIC);

	StopAllPortsInEngine();
	// Sensor_Power_Off();
	PWM_Close();
	Engine_Timer_Close();
	close(fuser_dev);
	close(i2c_dev);
	signal(SIGINT,SIG_DFL);
#endif
}
#endif	/* SMART_IC_TONER : @@2013.01.10. By CSPark */

#if PICK_THD
void* dcmtr_ctrl_loop(void* unused)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t err;
	uint32_t msg;
	
	while(1)
	{
		err = posix_wait_for_message(dcmtr_ctrl_mq, (char*)&msg, sizeof(msg), POSIX_WAIT_FOREVER);
		XASSERT(err == 0, err);
	
		switch(msg)
		{
			case DC_MOTOR_RUN:
				MSG("dcmotor_start msg : %d <%d>\n",msg, cProcessCounter);
				dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
				break;
			case DC_MOTOR_STOP:
				MSG("dcmotor_stop msg : %d <%d>\n",msg, cProcessCounter);
				dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off );
				break;
		}
	}
	return 0;
}
#endif
#if UART_TX_THD
void* uarttx_ctrl_loop(void* unused)
{
//	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t err;
	uint32_t msg;
	
	while(1)
	{
		err = posix_wait_for_message(uarttx_ctrl_mq, (char*)&msg, sizeof(msg), POSIX_WAIT_FOREVER);
		XASSERT(err == 0, err);
	
		switch(msg)
		{

		}
	}
	return 0;
}
#endif

void InitializeVariable(void)
{
//	MESSAGE message = {0};
	
	cProcessCounter = 0;
	fTray1Empty = FALSE;
	Tray1Online = SET;
	Tray2Online = SET;
	Tray3Online = SET;
	Tray1OnlineChattering = CLEAR;
	Tray2OnlineChattering = CLEAR;
	Tray3OnlineChattering = CLEAR;
	MPTOnlineChattering = CLEAR;
	fFirstTray1LevelDetect = CLEAR;
	fFirstTray2LevelDetect = CLEAR;
	fFirstTray3LevelDetect = CLEAR;
	tray1detect_count = CLEAR;
	tray2detect_count = CLEAR;
	tray3detect_count = CLEAR;
	mptdetect_count = CLEAR;
	
	ctray1highlowsensorchattering = CLEAR;
	ftray1highlowsensorchattering = CLEAR;
	fignoresensorchange = CLEAR;
	temp_papervol = CLEAR;
#if ABBA2
	pre_papervol = IsPaperPresentOnTray1();
#else
#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		pre_papervol = IsPaperPresentOnTray1();
	}
	else
	{
		pre_papervol = CLEAR;
	}
#endif
	ctray1sizesensorchattering = CLEAR;
	ftray1sizesensorchattering = CLEAR;
	fignoresizesensorchange = CLEAR;
	temp_papersize = CLEAR;
	pre_papersize = CLEAR;
	
	fTray2Notify = FALSE;
	fTrayOpen = FALSE;
	fOnlineNotiy = FALSE;
	fTrayCloseNotify = FALSE;
	fEngineRestartCmd = RECEIVED;
	NarrowMediaCheck = 0;
	cFullStackDetacted = 0;
	cFullStackReleased = 0;
	fState_FullStack = CLEAR;
	f2ndDrawerState = CLEAR;
	f3ndDrawerState = CLEAR;
	f2ndDrawerState_1stdetect = CLEAR;
	f3rdDrawerState_1stdetect = CLEAR;
	fOption_1stdetect = CLEAR;
	fJamDebugTrace = TRUE;
	fMotorRun_Timer = CLEAR;
	fSmartIC_Authentication = TRUE;
	fOld_SmartIC = CLEAR;
	fStore_TonerBlockState = CLEAR;
	
	fWaitPrintPageCmd = CLEAR;
	fChangeTargetTemp = CLEAR;
	fPreRunBeforePrintCmd = CLEAR;
	fPOR_Motor_Run_Slow = FALSE;
	
	IpcFlag1.DR.BYTE = (Uint8)0 ;
	IpcFlag2.DR.BYTE = (Uint8)0 ;
	IpcFlag3.DR.BYTE = (Uint8)0 ;
	
	fStartMainMotor = CLEAR;
	fEngine_Restart_Cmd = NOT_ACTIVE;
	ftempEngineRestartCmd = CLEAR;

	Temp_PageMsg.temp_PageID = CLEAR;
	Temp_PageMsg.temp_Source = CLEAR;
	Temp_PageMsg.temp_Narrow = CLEAR;
	Temp_PageMsg.temp_MediaDescription = CLEAR;
	Temp_PageMsg.temp_Length = CLEAR;
	Temp_PageMsg.temp_Darkness = CLEAR;
	Temp_PageMsg.temp_TonerSaver = CLEAR;
	Temp_PageMsg.temp_OutputDestination = CLEAR;
	Temp_PageMsg.temp_cheat_gap = CLEAR;
	
//	machineType = ABBA_OAK_H;
	
	Save_emptyTrayStatus = TRAY_MEDIA_UNKNOWN;
	
	Tray_Status.Tray1.newTrayStatus = 0x00;
	Tray_Status.Tray1.oldTrayStatus = 0x00;
	Tray_Status.Tray1.emptyTrayStatus = 0x00;
	Tray_Status.Tray2.newTrayStatus = 0x00;
	Tray_Status.Tray2.oldTrayStatus = 0x00;
	Tray_Status.Tray2.emptyTrayStatus = 0x00;
	Tray_Status.Tray3.newTrayStatus = 0x00;
	Tray_Status.Tray3.oldTrayStatus = 0x00;
	Tray_Status.Tray3.emptyTrayStatus = 0x00;
	Tray_Status.Tray4.newTrayStatus = 0x00;
	Tray_Status.Tray4.oldTrayStatus = 0x00;
	Tray_Status.Tray4.emptyTrayStatus = 0x00;
	
	f1stTimeCheckTray1 = SET;
	f1stTimeCheckTray2 = SET;
	f1stTimeCheckTray3 = SET;
	
	f_MSG_USER_ERR_RECOVERY = CLEAR;
}

void InitializeFuserControlMsg(void)
{
	Fuser_Control.RequestState = ST_STANDBY;
	Fuser_Control.State = ST_OFF;	/* ADD .state Initialize Value : @@2013.01.10. By CSPark */
	Fuser_Control.Target_Temperature = 180;
	Fuser_Control.PaperJam = CLEAR;
	Fuser_Control.TempPaperJam = CLEAR;
	Fuser_Control.fPowerSaveMode = CLEAR;
	Fuser_Control.fMakeMotorRun = CLEAR;
	Fuser_Control.fFatalError = CLEAR;
	Fuser_Control.fPOR_PageSupervisor_Active = SET;
	Fuser_Control.fMainMotorState = CLEAR;
	Fuser_Control.fFuserFault = CLEAR;
	Fuser_Control.FuserFaultNumber = CLEAR;
	Fuser_Control.NarrowMediaCount = CLEAR;
	Fuser_Control.fDuplexJob = CLEAR;
	Fuser_Control.StandbyModeType = CLEAR;
	Fuser_Control.fLowTemperature = CLEAR;
	Fuser_Control.fDevFanOff_ForLowServo = CLEAR;
	Fuser_Control.FanStopLongDelayInStandby = CLEAR;
#if BLI_TEST
	Fuser_Control.LowSpeedFan_Wakeup = CLEAR;
#endif
	fStartFromPowerSaveMotorRun = CLEAR;
	bePrintFromPowerSave = CLEAR;
	Pre_Fuser_Control = Fuser_Control;
	
	Write_Fuser_Func();
}

void InitializeSystem(void)
{
	InitializeEngineManager();
	InitializePageSupervisor(); 
	InitializeEPEvaParameter();
}

void Engine_Init(void)
{
	dec_printer_enable_thv(true);
    pwm_run(eng_hardware->main_fan, 1);    // turn off main fan
	pwm_run(eng_hardware->sub_fan, 1);
	pwm_run(eng_hardware->dev_fan, 1);
    pwm_run(eng_hardware->transfer_voltage, 1);
    pwm_run(eng_hardware->developer_voltage, 1);
    pwm_run(eng_hardware->charging_voltage, 1);
    
    pwm_change_duty_cycle(eng_hardware->main_fan, 0, 526);
    pwm_change_duty_cycle(eng_hardware->sub_fan, 0, 526);
    pwm_change_duty_cycle(eng_hardware->dev_fan, 0, 526);
    
    Pre_Heat_Off();
	Sensor_Power_On();
	Motor_FWD();
	InitializeVariable();
	InitializeSystem();
	InitializeFuserControlMsg();
}

void _Set_EM_State(Uint8 st)
{
		Fuser_Control.EM_State = (Uint8)st;
		EM_StateCounter = (Uint8)0;
}

void	InitializeEngineManager(void)
	/* Initialize a variable used the engine manager */
{
	InitializeEngineVariable();
	Fuser_Control.EM_State = EMS_RIP_SYNC;
	EM_StateCounter = NONE;
	fEngineApplication = ACTIVE;
	fCoverOpenDebugMsg = TRUE;	
}

void	InitializeEngineVariable(void)
{	
	motorType = MOTOR_OLD;
	Fuser_Control.fMakeMotorRun = CLEAR;
	fMainMotorJogMode = CLEAR;
	/* Engine Motor Offset */
		
	fRequested_Power_Saver = NOT_ACTIVE;
	
	/* Paper Source Size Initialize */
	SavedRealMediaSizeInMPF = MS_Unknown;
	SavedRealMediaSizeInTray1 = MS_Unknown; 
	SavedRealMediaSizeInTray2 = MS_Unknown;
	SavedRealMediaSizeInTray3 = MS_Unknown;
	SavedRealMediaSizeInDuplex = MS_Unknown;

	MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
	
	SavedChargePWM = CLEAR;
	SavedDeveloperPWM = CLEAR;
	SavedTransferPWM = CLEAR;
	
	
	SavedPaperPathMPFToPins = PSTicks_MPFToPins;
	SavedPaperPathT1ToPins = PSTicks_T1ToPins;
	SavedPaperPathT2ToPins = PSTicks_T2ToPins;
	SavedPaperPathT3ToPins = PSTicks_T3ToPins;
	
	CoverState.DR.BYTE = CLEAR;
	
	/* Initialize Engine Error Variable */
		/*EngineFatalError1.DR.BYTE = CLEAR;*/
	fPaperFeedMotorFailure = CLEAR;
	fMirrorMotorFailure = CLEAR;
	fLaserDiodeFailure = CLEAR;
	fNoFirstHsyncSignal = CLEAR;
	fPrintheadLostHsync = CLEAR;
	fVideoDoneFail = CLEAR;
	fSoftwareFailure = CLEAR;
	
	/* PaperJam.DR.BYTE = CLEAR; */
	fPaperJamOnPins = CLEAR;
	fPaperJamBetweenPinsAndPexs = CLEAR;
	fPaperJamOnPexs = CLEAR;
	fPaperJamOnDis = CLEAR;
	fPaperJamAtMPF = CLEAR;

	/* TempPaperJam.DR.BYTE = CLEAR; */
	PaperJamOnPins = CLEAR;
	PaperJamBetweenPinsAndPexs = CLEAR;
	PaperJamOnPexs = CLEAR;
	PaperJamInDuplex = CLEAR;
	PaperJamBetweenPickupAndPins = CLEAR;
	
	/* PaperEmpty.DR.BYTE = CLEAR; */
	fPaperEmptyForMPF = NO_EMPTY;
	fPaperEmptyForT1 = NO_EMPTY;
	fPaperEmptyForT2 = NO_EMPTY;
	fPaperEmptyForT3 = NO_EMPTY;
	//if(IsPaperPresentOnMPF() == PRESENT) 
	//{
		//EmptyForMPF = NO_EMPTY;
	//}
	//else 
	//{
		//EmptyForMPF = EMPTY;
	//}
	//if(IsPaperPresentOnTray1() == PRESENT)		// added	//21Apr09 yjlee 	
	//{
		//EmptyForT1 = NO_EMPTY;
	//}
	//else
	//{
		//EmptyForT1 = EMPTY;
	//}
	EmptyForT2 = NO_EMPTY;
	EmptyForT3 = NO_EMPTY;
	fRetryPickUp = CLEAR;
	fLastPickUp = CLEAR;
	fStatusOfTray1Remove = CLEAR;
	fStatusOfTray1Insert = CLEAR;
	fCheckMachineStateBeforeStart = FALSE;

	fEngineStateFull = CLEAR;
	ThroughputRate = FASTEST;

	fMotorRunWithoutPrintCmd = CLEAR;
	fMotorRun = CLEAR;

	SavedSourceInPrePage = CLEAR;
	fMotorRunFromPowerSave = CLEAR;

	AttachedDevice.DR.BYTE = 0xC3;
	cJamRecoverTimer = 0;
	cPowerSaveWaitCnt = 0;

	cPOR_MotorRunStartTime = 0;
	fPowerOnReset = CLEAR;
	fClearWaitPrintPageCmd = CLEAR;
	cWakeKey = 0;
	cWakeKeyCheckPeriod = 0;
	fWaitWakeKeyRelease = CLEAR;
	fWakeKey_Onetime = CLEAR;
	
	cDuplexPageCount = 0;
	cDuplexSlowPrintCount = 0;
	fDuplexSlowPrint = CLEAR;
	fDuplexSet = CLEAR;
	cKeepMotorRunForCooling = 0;
	fJogModeOn = CLEAR;
	MainMotorJogCounter = 0;
	JogModeType = JogType7;
	fDuplexCooling = CLEAR;
	fCoolingState = CLEAR;
	fHSync_Detacted = CLEAR;
	EarlyStartCmd = CLEAR;
	ColdWakeUpCmd = CLEAR;
	fColdWakeupState = CLEAR;
	fPowerSave = CLEAR;
#if IPG_ENABLE
	fVideo_Print_Enable = SET;
	fReVideoOn = CLEAR;
#endif
#if PICK_THD
	posix_create_message_queue(&dcmtr_ctrl_mq, "/DCmotorEngQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(uint32_t));
	posix_create_thread(&dcmtr_thread_id, dcmtr_ctrl_loop, 0, "dcmtr_ctrl_task", dcmtr_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);
#endif
#if UART_TX_THD
	posix_create_message_queue(&uarttx_ctrl_mq, "/UartTxEngQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(uint32_t));
	posix_create_thread(&uarttx_thread_id, uarttx_ctrl_loop, 0, "uarttx_ctrl_task", uarttx_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);
#endif	
	PS_24vOn(SET);
	fSensorPowerOffForPWS = CLEAR;
	fMotorSpeedChange = CLEAR;
	
	fPwsToPrint = CLEAR;
	fMotorRunForPwsToPrint = CLEAR;
	fPwsToPrint_1stPick = CLEAR;
	fPwsToPrint_2ndPick = CLEAR;
	fPwsToPrint_1stPage = CLEAR;
	fPwsToPrint_2ndPage = CLEAR;
	fPressWakeupKey = CLEAR;
	fSpeedUpForPWS = CLEAR;
	
	fEngineTimingCheck = CLEAR;
	cTray2PaperCheckBeforePickUp = 0;
	
	fJob_Toner_Correction_value = CLEAR;
	PrePageSize = CLEAR;
	PreJobNum = CLEAR;
	PreJobInfo = CLEAR;
	fCheckJobNumChange = CLEAR;
	fCheckJobInfoChange = CLEAR;
	fJobIDChanged = CLEAR;
	fJobInfoChanged = CLEAR;
	fIgnorePrePick = CLEAR;
	fNewTonerCheck = FALSE;
#if SMART_IC_LOCK_ENABLE
	ftoner_count_write_for_coveropen = CLEAR;
#endif
	fWaitToChangeMotorSpeed = CLEAR;
	fWaitPrintingForResync = CLEAR;
	fDebugOptionCommFail = TRUE;
	fDebugOptionCommFailForDisconnection = TRUE;
	cOptionCommFail = CLEAR;
	cFatalOptionCommFail = CLEAR;
	fDebugTargetTempChange = CLEAR;
	DEBUG_TargetTemp = CLEAR;
	fDebugStandbyTempChange = CLEAR;
	DEBUG_StandbyTemp = CLEAR;
	
	fQuickly_sensor_check_T1 = CLEAR;
	fQuickly_sensor_check_T2 = CLEAR;
	fQuickly_sensor_check_T3 = CLEAR;
	
	sp_mode_hardware_test_init();
	fHardwareTest.byte = CLEAR;	
#if DUPLEX_SLOW_MODE
	fDebugDuplexPrintSlowlyFor15minClear = FALSE;
	EnvTempStatus_ForDuplexPrint = EnvTemp_DL;
#endif
#if DUPLEX_SLOW_MODE2
	fDuplex_Slow_mode = CLEAR;
	fEnvTempStatusChage = CLEAR;
	fSimplexMotorUp = CLEAR;
	EnvTempStatus_ForDuplexPrint = EnvTemp_DL;
#endif
	fWakeup = CLEAR;
	
	StartLamination = CLEAR;
	StartLaminating1page = CLEAR;
	StopLamination = CLEAR;
	ResumeLamination = CLEAR;
	EndLamination = CLEAR;
	CheckExitGuide = CLEAR;
	MotorStartForLaminating = CLEAR;
	LaminatingMode = CLEAR;
	RunEngineForLamination = CLEAR;
	current_lamination_status = STATUS_LAMINATEINFO_LAMINATINGDONE;
	
	FanCtrlEnvIdx = FAN_CTRL_L;
	JogModeEnvIdx = JOG_MODE_L;
	JogModeTimeIdx = CLEAR;
	JogModeCountFor24v = CLEAR;
	f24vOnForJogMode = CLEAR;
	
	mtrstop_count_laminating1 = CLEAR;
	mtrstop_count_laminating2 = CLEAR;
	SelectPreheatingCount = CLEAR;
	PreHeatingDoneTime = PSTicks_Minimum;
	lami_condition = CON_MTR_STOP_LAMI1;
	
	JamCode_Laminating = JAM_CLEAR_LAMINATING;
}

extern void	Set_Charge_PWM(Uint8 PWM);
extern void	Set_Developer_PWM(Uint8 PWM);
extern void	Set_Transfer_PWM(Uint8 PWM);

long getTime(void)
{
    /* get monotonic clock time */
    static struct timespec oldtime = {0,0};
    struct timespec monotime;
    long timeresult;
    clock_gettime(CLOCK_MONOTONIC, &monotime);

    long interval;	//unit : ms

    interval = (monotime.tv_sec - oldtime.tv_sec) * 1000 + (monotime.tv_nsec - oldtime.tv_nsec)/1000000;
    if ( interval < 16 ) timeresult = 1;
    else				 timeresult = (interval + 5) / 10;

    if ( timeresult > 2 ) {
        MSG("\n@@ tr  = %ld <%d>@\n\n",timeresult, cProcessCounter);
    }
    oldtime.tv_sec = monotime.tv_sec;
    oldtime.tv_nsec = monotime.tv_nsec;
    return timeresult;
}

void	EngineApplication(engine_hardware_t* eng_hw)
	/* When interrupt of Timer for TIME BASE TIMMER,
	execute and call PageSupervisor and Engine Manager */
{
	//REL_ASSERT( OK == gpio_write( eng_hw->pick_pin, ON ) );
	//REL_ASSERT( OK == gpio_write( eng_hw->pick_mpt, ON ) );
	//REL_ASSERT( OK == gpio_write( eng_hw->duplex_sol, ON ) );
	//MSG("LOCK <%d>\n",Get_MainMotorLockState());
	//MSG("LOCK main <%d> sub <%d>  dev <%d> \n",Get_MainFanLockState(), Get_SubFanLockState(), Get_DevFanLockState());
	//MSG("ADC <v=%d> <t=%d>\n",dec_adc_scaled_read_channel_mV(eng_hw->thermistor), ChangeVoltageToTemp());
	//REL_ASSERT( OK == gpio_write( eng_hw->pick_pin, OFF ) );
	//REL_ASSERT( OK == gpio_write( eng_hw->pick_mpt, OFF ) );
	//REL_ASSERT( OK == gpio_write( eng_hw->duplex_sol, OFF ) );
	//if(Get_MainMotorLockState()==LOCK)
	//{
		//dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off );
		//MSG("LOCK <%d> OFF\n",Get_MainMotorLockState());
	//}
	//Set_Charge_PWM(25);
	//Set_Developer_PWM(50);
	//Set_Transfer_PWM(100);
	
	//MSG("Input Sensor State = %d\n",PAPER_IN);
	//MSG("Output Sensor State = %d\n",PAPER_OUT);pick_pin
	//MSG("T1 Sensor State = %d\n",TRAY1_SEN);
	//MSG("Door State = %d\n",DOOR_OPEN);
	//MSG("Full Stack State = %d\n",FULL_STACK_SEN);
	
	//dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
	
	//if(XPORT_LOCK == LOCK)
	//{
		//MSG("Main Motor LOCK OK\n");
		//posix_sleep_ms(2000);
		//dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off );
		//MSG("Motor Stop\n");
	//}
	
	//if()
	//{
		//slogit( dec_stop_laser_mirror(eng_hw->laser) );
	//}

	//if(fStartMainMotor==SET)
	//{
		//GP_Write(BLDC_ST,0);		// Motor Driver Start Enable // Motor enable after clk (10msec)
		//fStartMainMotor = CLEAR;
	//}

	//if(fEngineApplication == ACTIVE) 
	//{
		//MSG("<T=%d>\n",ChangeVoltageToTemp());
		
		Uint8 tickModify;
#if 0		
		if(fEngineTimingCheck == CLEAR)
		{
			gpio_write(eng_hardware->GPIO_E17, 1);
			fEngineTimingCheck = SET;
		}
		else
		{
			gpio_write(eng_hardware->GPIO_E17, 0);
			fEngineTimingCheck = CLEAR;
		}
#endif		
		tickModify = (Uint8)(getTime());
		cProcessCounter = cProcessCounter + tickModify;
		
		Read_Fuser_Func();
		MainMotorDelay();
		FuserHeaterManager(); /* [KSF050201] */
		PageSupervisors(tickModify);
		EngineManager();
		Write_Fuser_Func();
	//}
	//if(Fuser_Control.State == ST_OFF)
	//{
		//MSG("## OFF <%d> ##\n",ChangeVoltageToTemp());
		//if(ChangeVoltageToTemp() <= 130)
		//{
			//Fuser_Control.RequestState=CMD_FUSER_STANDBY;
		//}
	//}
	//if(Fuser_Control.State == ST_STANDBY)
	//{
		//MSG("## STANDBY <%d> ##\n",ChangeVoltageToTemp());
		//if(ChangeVoltageToTemp() <= 160)
		//{
			//Fuser_Control.RequestState=CMD_FUSER_PRINT;
		//}
	//}
	//if(Fuser_Control.State == ST_PRINT)
	//{
		//MSG("## PRINT <%d> ##\n",ChangeVoltageToTemp());
		//Fuser_Control.RequestState=CMD_FUSER_OFF;
	//}
}

void	(* EngineManagerFunctbl[])(void) =
{
/* For POR and Warmup */
	/* EM_SyncronizeToRip, */
	EM_EngineRestart,						/* EMS_RIP_SYNC */
	EM_StartWarmup,							/* EMS_StartWarmup */
	EM_StartPOR,							/* EMS_StartPOR */
	EM_PowerOnResetState,
	EM_POR_Standby,
	
	/* For Standby */
	EM_PrintReady,		/* 5sequence */
	EM_Standby,
	EM_PowerSave,
	EM_JogMode,
	
	/* For Print Sequence */
	EM_Printing,
	
	/* For Laminating Sequence */
	EM_CheckExitSensorForLaminatingStart,
	EM_CheckExitSensorForLaminatingEnd,
	EM_Laminating_Servo,
	EM_PreHeating,
	EM_Laminating,
	EM_Laminating_Standby,
	EM_Laminating_End,
	EM_Laminating_Error,
	EM_Laminating_Recovery,
	EM_Laminating_CloseCounter,
	EM_Laminating_CloseDoorCover,
	
	/* For EP Cleaning Process */
	EM_ExitLastPage, 	/* 10sequence */
	EM_EPCleaning,		
	
	/* Stop Printer & return Standby mode */
	EM_PrinterStop,
	
	/* For Open Front Cover Condition */
	EM_OpenDoorCover,
	EM_CloseCounter,
	EM_CloseDoorCover, 	/* 15sequence */
	
	EM_RecoverPaperJam,	
	EM_RecoverPaperEmpty,
	EM_RecoverFullStack,
	EM_RecoverNoStopError,
	EM_RecoverError,
	EM_OptionResync,
	EM_FatalError, /* For Unrecoverable Error Condition */
	/* For Maximum State Sign */
	EM_SCError,
	EM_Diagonostic,
	EM_NonRecoveryError,
	EM
};

void	EngineManager(void)
	/* manager engine use function pointer to call function 
	and call error manager */
{
	if(fCheckMachineStateBeforeStart == TRUE)
	{
		Check_MachineStateBeforeStart();
	}

	if(fPowerSave == FALSE)
	{
		CheckMPF();
		if(RunEngineForLamination == SET)
		{
			CheckDoorCover_Laminating();
		}
		else
		{
			CheckDoorCover();
		}
		CheckTray1();
	}
	
	if((f2ndDrawerState == SET) && (fDebugOptionCommFail == TRUE))
	{
		CheckTray2();
	}	
	
	if(Fuser_Control.fMakeMotorRun == SET) 
	{
		/*CheckTonerSensor();*/
	}
	else 
	{
		MainMotorJogCounter++;
		if(MainMotorJogCounter > 6000) 
		{
			MainMotorJogCounter = CLEAR;
			MainMotor_1MinJogCounter++;
		}
	}
	ErrorManager();
	(* EngineManagerFunctbl[Fuser_Control.EM_State])();	
}

void	ErrorManager(void)
	/* To stop printer when Error Manager detecte error 
	from engine that be related work */
{
	PageSupervisor *PGSPVSR;
	/* Door Cover Open */
	cErrorStateCount++;

	if(fFrontCoverState == OPEN) 
	{
		/*StopAllPortsInEngine();*/
		return;
	}
	
	if ((PGSPVSR = Get_MatchToSequenceID(PSS_PrinterError))
		 != (PageSupervisor *)NULL)
	{
		PGSPVSR->Flags = PSS_PrinterError; /*PrinterError Debugging*/
	}
	
	if(Fuser_Control.EM_State == EMS_RecoverError) 
	{
		return;
	}
	
	if(Fuser_Control.EM_State == EMS_RecoverPaperJam) 
	{
		return;
	}
	
	if(Fuser_Control.EM_State == EMS_RecoverFullStack) 
	{
		return;
	}

	if (Fuser_Control.EM_State >= (Uint8)EMS_FatalError)
		return ;
/* [KSR032101] for SMC */		
//   if( PaperJamBetweenPinsAndPexs == JAM ) return; 
	if( ((fPaperEmptyForMPF == EMPTY) || (fPaperEmptyForT1 == EMPTY) || (fPaperEmptyForT2 == EMPTY) || (fPaperEmptyForT3 == EMPTY)) ||
		((PaperJamBetweenPickupAndPins == JAM) || (PaperJamOnPins == JAM) || (PaperJamBetweenPinsAndPexs == JAM) || (PaperJamOnPexs == JAM) || (PaperJamInDuplex == JAM)) ||
		(fPaperFeedMotorFailure || fMirrorMotorFailure || fNoFirstHsyncSignal || fPrintheadLostHsync || fSoftwareFailure || fVideoDoneFail) ||
		(Fuser_Control.fFuserFault != 0) || (fFullStack_Error==SET) )
	{
		// if(cMemoryWrite_Count != 0)
		// {
			// cMemoryWrite_Count = 0;
			// fMemoryWrite = TRUE;
			// fSmartICWrite_PageCount = TRUE;
		// }
//		printf("(eng)cMemoryWrite_Count = %d\n", cMemoryWrite_Count);//121204
#if SMART_IC_LOCK_ENABLE
		ftoner_count_write_for_coveropen = CLEAR;
#endif
		if(cMemoryWrite_Count != 0)
		{
			cMemoryWrite_Count = 0;
			fSmartICWrite_PageCountFailInRun = FALSE;
			fSmartICWrite_DrumPageCountFailInRun = FALSE; /* [KSR040101] */
			fSmartICWrite_DotCountFailInRun = FALSE;
			fSmartICWrite_DrumDotCountFailInRun = FALSE; /* [KSR040101] */
			fSmartICWrite_DotCount = TRUE;
			fSmartICWrite_DrumDotCount = TRUE; /* [KSR040101] */
			fMemoryWrite = TRUE;
			fSmartICWrite_PageCount = TRUE;
			fSmartICWrite_DrumPageCount = TRUE; /* [KSR040101] */
		}
		else 
		{
			if (fSmartICWrite_PageCountFailInRun == TRUE)	
			{
#if	ENG_TRACE
				printf("(Eng) Memory Write Retry PageCount !!!\n");
#endif			
				fSmartICWrite_PageCountFailInRun = FALSE;
				fSmartICWrite_PageCount = TRUE;		
			}
			
			if (fSmartICWrite_DotCountFailInRun == TRUE)
			{
#if	ENG_TRACE
				printf("(Eng) Memory Write Retry DotCount !!!\n");
#endif			
				fSmartICWrite_DotCountFailInRun = FALSE;
				fSmartICWrite_DotCount = TRUE;
				fMemoryWrite = TRUE; /* [KSR040101]-start */
			}
				
			if (fSmartICWrite_DrumPageCountFailInRun == TRUE)	
			{
#if	ENG_TRACE
				printf("(Eng) Memory Write Retry DrumPageCount !!!\n");
#endif			
				fSmartICWrite_DrumPageCountFailInRun = FALSE;
				fSmartICWrite_DrumPageCount = TRUE;		
			}
			
			if (fSmartICWrite_DrumDotCountFailInRun == TRUE)
			{
#if	ENG_TRACE
				printf("(Eng) Memory Write Retry DrumDotCount !!!\n");
#endif			
				fSmartICWrite_DrumDotCountFailInRun = FALSE;
				fSmartICWrite_DrumDotCount = TRUE; /* [KSR040101]-end */
				fMemoryWrite = TRUE;
			}	
		}
	}

	if(fPaperEmptyForMPF == EMPTY || fPaperEmptyForT1 == EMPTY || fPaperEmptyForT2 == EMPTY || fPaperEmptyForT3 == EMPTY) 
	{
#if	ENG_TRACE
		MSG("(Eng) ErrorManager : Empty!!!\n");
#endif	
		if(engine_state.paper_out_tray1 == true) 			Save_emptyTrayStatus = GetTrayMediaInfo(Tray_Status.Tray1.emptyTrayStatus);
		else if(engine_state.paper_out_tray2 == true) 		Save_emptyTrayStatus = GetTrayMediaInfo(Tray_Status.Tray2.emptyTrayStatus);
		else if(engine_state.paper_out_tray3 == true)		Save_emptyTrayStatus = GetTrayMediaInfo(Tray_Status.Tray3.emptyTrayStatus);
		else if(engine_state.paper_out_tray4 == true)		Save_emptyTrayStatus = TRAY_MEDIA_UNKNOWN;
		else {}
	
		cErrorStateCount = 0;
		Fuser_Control.EM_State = EMS_RecoverPaperEmpty;
		return;
	}

	else if ((SC_State == ST_CERT_FAILULE || SC_State == ST_TONER_BLOCK || SC_State == ST_TONER_WRONG) && (fSC_Writefault==CLEAR))
	{
		MSG("(Eng) ErrorManager : SC_State = %d, !!!\n", SC_State);
		cErrorStateCount = 0;
		StopAllPortsInEngine() ;
		Fuser_Control.EM_State = EMS_SCError;
		return;
	}
	
	else if ((PaperJamBetweenPickupAndPins == JAM)
		|| (PaperJamOnPins == JAM) || (PaperJamBetweenPinsAndPexs == JAM)
		|| (PaperJamOnPexs == JAM) || (PaperJamInDuplex == JAM)) 
	{
		cErrorStateCount = 0;
		Fuser_Control.EM_State = EMS_RecoverPaperJam ;
		MSG("(eng)Paper Jam : EMS_RecoverPaperJam (%d)\n", cProcessCounter);
		MSG("(eng)PaperJamBetweenPickupAndPins = %d, PaperJamOnPins = %d, PaperJamBetweenPinsAndPexs = %d, PaperJamOnPexs = %d, PaperJamInDuplex = %d\n", 
						PaperJamBetweenPickupAndPins, PaperJamOnPins, PaperJamBetweenPinsAndPexs, PaperJamOnPexs, PaperJamInDuplex);
		return ;
	}

	/*else if ((EngineFatalError.DR.BYTE & 0x83) != 0)*/
	else if(fPaperFeedMotorFailure || fMirrorMotorFailure
	|| fNoFirstHsyncSignal || fPrintheadLostHsync || fSoftwareFailure || fVideoDoneFail) 
	{
		StopAllPortsInEngine() ;
#if	ENG_TRACE		
		MSG("(Eng)FatalError Condition 1 : fPaperFeedMotorFailure=%d, fMirrorMotorFailure=%d, fSoftwareFailure=%d, fNoFirstHsyncSignal = %d, fVideoDoneFail = %d\n", 
		fPaperFeedMotorFailure, fMirrorMotorFailure, fSoftwareFailure, fNoFirstHsyncSignal, fVideoDoneFail );
#endif		
		Fuser_Control.EM_State = (Uint8)EMS_FatalError ;
		return;
	}
	else if(Fuser_Control.fFuserFault != 0) 
	{				
		StopAllPortsInEngine() ;
		Fuser_Control.RequestState = (Uint8)ST_OFF;
#if	ENG_TRACE		
		MSG("(Eng)FatalError Condition 2 : FuserFault is occured\n");
#endif	
		CodeOfMachineCheck = (Fuser_Control.FuserFaultNumber-100);		// Fuser > Engine : 3xx, Engine > HostPrint : 2xx, HostPrint > SysMgr : 3xx 
		Fuser_Control.EM_State = (Uint8)EMS_FatalError ;
		return;
	}
	else if(fFullStack_Error==SET)
	{
#if	ENG_TRACE
		MSG("(Eng)## FullStack_Error = %d ##\n",cErrorStateCount);
#endif		
		cErrorStateCount = 0;
		Fuser_Control.RequestState = ST_OFF; 
		Fuser_Control.EM_State = EMS_RecoverFullStack ;
		return ;
	}
	else
	{
	}
}
	
void	EM_EngineRestart(void)		/* EMS_RIP_SYNC */
	/* Wait an Engine Restart Command */
{
	if(fEngineRestartCmd == RECEIVED) 
	{
#if	ENG_TRACE		
		MSG("(Eng)EngineRestartCmd is received!\n");
#endif		
		Set_EngineVersionInfo();
		if(SmartICInfoNotify() == SC_OK)
		{
			Set_MotorStatusNotify(MOTOR_STOP, 0x00, Toner_Used_Percentage, cPageCount5perTrans);
		}
		
		fEngineRestartCmd = NOT_RECEIVED;
		Fuser_Control.EM_State = EMS_StartWarmup;

		fPaperFeedMotorFailure = CLEAR;
		fMirrorMotorFailure = CLEAR;
		fLaserDiodeFailure = CLEAR;
		fNoFirstHsyncSignal = CLEAR;
		fPrintheadLostHsync = CLEAR;
		fVideoDoneFail = CLEAR;
		fSoftwareFailure = CLEAR;
			
		Fuser_Control.fFuserFault = CLEAR;
		
		Fuser_Control.fFatalError = CLEAR;
		CodeOfMachineCheck = CLEAR;
		
		Write_Fuser_Func();
		RecoverError();
	}
	Check_MachineStateBeforeStart();
}
	
void	EM_StartWarmup(void)		/* EMS_StartWarmup */
	/* Wait Start Warm up from RIP */
{
	if(Check_MachineStateBeforeStart() == FALSE) 
	{
#if	ENG_TRACE		
		MSG("(Eng)Check_MachineStateBeforeStart : FAIL !\n");
#endif		
		return;
	}

#if	ENG_TRACE		
	MSG("(Eng)EM_StartWarmup : to EMS_StartPOR !\n");
#endif		
	fStartWarmupCmd = NOT_RECEIVED;
	Fuser_Control.EM_State = EMS_StartPOR;
}

void	EM_StartPOR(void)			/* EMS_StartPOR */
	/* Wait parameters and Start POR Sq.  */
{
	StandbyModeSelect(); /* [KSF051502] */
	Fuser_Control.EM_State = EMS_PowerOnResetState;
}

void	EM_PowerOnResetState(void)			/* EMS_PowerOnResetState */
	/* During POR Sequence */
{
	if( ((Fuser_Control.State == ST_WARMUP) && (ChangeVoltageToTemp() >= Fuser_Setting_Parameters[3])) /* [KSF051501] */
	|| Fuser_Control.State == ST_STANDBY 
	|| Fuser_Control.State == ST_PRINT)
	{
		if(fClearWaitPrintPageCmd != SET)
		{
			Fuser_Control.fMainMotorState = TRUE;
			cPOR_MotorRunStartTime = cProcessCounter;
			fPowerOnReset = SET;
#if	ENG_TRACE
			MSG("(Eng)## cPOR_MotorRunStartTime = %d ##\n",cPOR_MotorRunStartTime);
#endif
		}
		else
		{
			fClearWaitPrintPageCmd = CLEAR;
		}
		/* POR Page Supervisor Set */
#if	ENG_TRACE
		HeatingStartTemperature = ChangeVoltageToTemp();
		MSG("(Eng) ## %s : HeatingStartTemperature = %d ##\n",__func__,HeatingStartTemperature);
#endif
		fPOR_PageSupervisor_Act = SET;	
		Set_POR_PGSPVSR_Parameter(PSS_EP_InitializeFor_POR);
		Fuser_Control.EM_State = EMS_POR_Standby;
	}
}

void	EM_POR_Standby(void)			/* EMS_POR_Standby */
	/* During POR Warm up for Fuser */	
{
//	if((Fuser_Control.State == ST_STANDBY) ||	(Fuser_Control.State >= ST_PRINT)) 
	if(1) 
	{
		/* Printer Idle Mode */
		if(fPOR_PageSupervisor_Act) 
		{
			if(fPOR_PageSupervisor_EPCleaning) 
			{
#if	ENG_TRACE				
				MSG("(Eng)EM_POR_Standby : Start EP_Cleaning\n");
#endif				
				Fuser_Control.EM_State = EMS_EPCleaning;
				return;
			}
		}
	}
}

void	EM_PrintReady(void)				/* EMS_PrintReady */
	/* Come up to Printer Idle Mode and Wait Start Command */
{
	/* Printer Idle Mode */
	if(fPendingPrintPageCmd == RECEIVED) 
	{
		_Set_EM_State(EMS_Standby);
		return;
	}
	if(bePrintFromPowerSave) 
	{
		Fuser_Control.EM_State = EMS_Standby;
		return;
	}

	if(fClearErrorsCmdRcv == RECEIVED) 
	{
		if(Set_PrintReadyNotify() == SEND) 
		{
			fClearErrorsCmdRcv = CLEAR;
		}
	}

}

void	EM_Standby(void)			/* EMS_Standby */
	/* Wait for Print Page Command */
{
	MESSAGE message;
		
	fDuplexCooling = CLEAR;
	fDuplexSet = CLEAR;
	fCoolingState = CLEAR;
	fColdWakeupState = CLEAR;
	
	cDuplexPageCount = 0;
	cDuplexSlowPrintCount = 0;
	fDuplexSlowPrint = CLEAR;
	
	fWakeup = CLEAR;
	fcancelpage = CLEAR;
	
#if DUPLEX_SLOW_MODE
	Clear_DuplexPrintSlowlyFor15min(cProcessCounter);
#endif

	if((cOptionCommFail > 0) || (fRetryDetectInStandby == SET))
	{
		MSG("(eng)Retry Option Detection In standby (%d)\n", cOptionCommFail);
		cOptionCommFail++;

		f2ndDrawerState = CLEAR;
		f3ndDrawerState = CLEAR;
		fTray2Notify = CLEAR;
		
		if(cOptionCommFail <= 5)
		{
			Option_Tx_Sync(OPTION_T1);
			Option_Tx_Sync(OPTION_T2);
			return;
		}
		if(cOptionCommFail <= 7)
		{
			return;
		}

		watchdog_service();
		Tray2_Detection_Check();
		if((fOption_1stdetect == SET) && (fRetryDetectInStandby == SET))
		{
			fOption_1stdetect = CLEAR;
			usleep(100*1000);
			if(f2ndDrawerState == SET) 	{ f2ndDrawerState_1stdetect = TRUE;		fQuickly_sensor_check_T2 = SET;		fTray2Notify = TRUE;		Option_Tx_Notify(OPTION_T1); }
			usleep(10*1000);
			if(f3ndDrawerState == SET)	{ f3rdDrawerState_1stdetect = TRUE;		fQuickly_sensor_check_T3 = SET;		fTray2Notify = TRUE;		Option_Tx_Notify(OPTION_T2); }
			usleep(10*1000);
		}
		Pending_Tray2_Detection_Result();
		if(f2ndDrawerState == SET) 		Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
		if(f3ndDrawerState == SET) 		Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
		cOptionCommFail = CLEAR;
		fDebugOptionCommFail = TRUE;		
	}
	fRetryDetectInStandby = CLEAR;
	
	if(engine_state.fcancelrequest == TRUE)
	{
		message.msgType = MSG_CANCELJOB;
		engine_cancel_page_msg(&message);
		engine_state.fcancelrequest = FALSE;
	}

	if(fSC_Writefault==TRUE)
	{
		fSC_Writefault = CLEAR;
#if	ENG_TRACE				
		printf("(Eng)## fSC_Writefault flag Clear ##\n");
#endif			
	}
	
	if(fStore_TonerBlockState==SET)
	{
		SC_State = ST_TONER_BLOCK;
		fStore_TonerBlockState = CLEAR;
	}
	
	if(fTonerLowMsgPending == TRUE)
	{
		Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_LOW);
		fTonerLowMsgPending = CLEAR;
	}
	ThroughputRate = FASTEST;
	Fuser_Control.NarrowMediaCount = 0; 
	
	if(bePrintFromPowerSave == TRUE)
	{
		if(((ChangeVoltageToTemp() >= Fuser_Setting_Parameters[3]) && (fPendingPrintPageCmd != RECEIVED)) /* [KSF040102] */	
		|| ((ChangeVoltageToTemp() >= MotorStartTempSelectForWakeup()) && (fPendingPrintPageCmd == RECEIVED)))
		{
#if	ENG_TRACE				
			MSG("(Eng)## <Motor Start From PWS> Cur_Temp = %d ##\n",ChangeVoltageToTemp());
#endif	
			SelectMotorRunTempForPWStoPrint = CLEAR;
			Write_Fuser_Func();
			bePrintFromPowerSave = CLEAR;
			fStaticMotorRunTime = SET;
			cPORMotorStartCounter = cProcessCounter;
//			StartMainMotor();
			fMotorRunFromPowerSave = SET;
			Fuser_Control.fPOR_PageSupervisor_Active = CLEAR;

			fStartFromPowerSaveMotorRun = SET;
			//if(!HowManyActivePageSupervisor()) 
			if(PrintPageCmd == FALSE) 
			{
#if	ENG_TRACE				
				MSG("(Eng)## HowManyActivePageSupervisor() = %d ##\n",HowManyActivePageSupervisor());
#endif				
				fMotorRunWithoutPrintCmd = SET;
				fMotorRun = SET;
			}
		}
		else
		{
			return;
		}
	}
	
	if(IsPaperPresentOnStack() == PRESENT)
	{
		cFullStackDetacted++;
		if(cFullStackDetacted >= 250)
		{
			fState_FullStack = TRUE;
			fFullStack_Error = TRUE;
//			Set_FullStackNotify(FullStackSet);
			full_stack_notify(FullStackSet);
			
			// PaperJamOnPexs = JAM;
			// CodeOfJamStatus = JAM_TE_EXIT_RELEASE;
#if	ENG_TRACE				
			MSG("(Eng)## FullStack State Detacted ##\n");
#endif	
		}
	}
	else
	{
		cFullStackDetacted = 0;
	}	
#if !ABBA2
	cWakeKeyCheckPeriod++;
	if(fWakeKey_Onetime == CLEAR)
	{
		if(cWakeKeyCheckPeriod % 3 == 0)
		{
			if((IsWakeKeyState() == TRUE) && ((fPendingPrintPageCmd == CLEAR) || (fPreRunBeforePrintCmd == CLEAR)))
			{
				cWakeKey++;
				if((cWakeKey >= 2) && (fWaitWakeKeyRelease == CLEAR))
				{
					cWakeKey = 0;
		#if	ENG_TRACE		
					MSG("(Eng)## Pws Key is Detacted !!! ##\n");
		#endif					
					usleep(100000);
		//			Set_PowerSaveActiveNotify();
					PowerKeyPressed(POWER_KEY_PRESSED);
					fWaitWakeKeyRelease = SET;
					fWakeKey_Onetime = SET;
		//			fPressPwsKey = SET;
				}
			}
			else
			{
				fWaitWakeKeyRelease = CLEAR;
				cWakeKey = 0;
			}
		}
	}
#endif
	/* For Power Save Mode */
	if(PowerSaveControl() == TRUE) 
	{
#if	ENG_TRACE		
		MSG("(Eng)+++++++++++++++++++++\n");
		MSG("(Eng)+  POWER SAVE MODE (%d) +\n", cProcessCounter);
#endif
		fWaitWakeKeyRelease = CLEAR;
		fWakeKey_Onetime = CLEAR;
		cWakeKey = 0;
				
		fPowerSave = TRUE;
		Tray1OnlineChattering = CLEAR;
		Tray2OnlineChattering = CLEAR;
		Tray3OnlineChattering = CLEAR;
		cPowerSaveStart = cProcessCounter;
		Fuser_Control.EM_State = EMS_PowerSave;
#if BLI_TEST
		Fuser_Control.LowSpeedFan_Wakeup = SET;
#endif
		
		fPwsToPrint = CLEAR;
		fPwsToPrint_1stPick = CLEAR;
		fPwsToPrint_2ndPick = CLEAR;
		fPwsToPrint_1stPage = CLEAR;
		fPwsToPrint_2ndPage = CLEAR;
		fPressWakeupKey = CLEAR;
		fPaperLengthIsNarrow = FALSE;
		fSpeedUpForPWS = CLEAR;
		
		JogModeCountFor24v = CLEAR;
		return;
	}

	if ((fPendingPrintPageCmd == RECEIVED) || (fPreRunBeforePrintCmd == RECEIVED))
	{
#if	ENG_TRACE		
		MSG("(Eng)EM_Standby : fPendingPrintPageCmd is receiverd\n");
#endif
		if ((fPendingPrintPageCmd == RECEIVED) && (fPreRunBeforePrintCmd == RECEIVED))
		{
			fPreRunBeforePrintCmd = CLEAR;
		}
		if (Check_MachineStateBeforeStart() == FALSE)
			return ;
	}

	/* Add '98/November/09 For ReProgramming Engine Code */
	/*else
		Check_ReProgrammingRequest() ;*/

	if((fPendingPrintPageCmd == RECEIVED) || (fPreRunBeforePrintCmd == RECEIVED))
	{
		/* Start Print */
		PrintPageCmdTemp = ChangeVoltageToTemp();
		
#if DEBUG_EARLY
		fMotorRunWithoutPrintCmd = CLEAR;
		fMotorRun = CLEAR;
		PrintPageCmd = TRUE;
		if(fPreRunBeforePrintCmd == RECEIVED)
		{		
			cPreRunTimeoutCounter = cProcessCounter;
#if	ENG_TRACE			
			MSG("(Eng)### fPreRunBeforePrintCmd == RECEIVED : cPreRunTimeoutCounter = %d###\n",cPreRunTimeoutCounter);
#endif				
		}
		
		if(StartPrintSequence(PSPS_StartPrintSequence) == TRUE) 
		{
#if	ENG_TRACE			
			MSG("(Eng)EM_Standby : fPendingPrintPageCmd is receiverd to EMS_Printing1\n");
#endif
			cWakeKey = 0;
			
			Fuser_Control.EM_State = EMS_Printing;
			return;	
		}
#else
		if(StartPrintSequence(PSPS_StartPrintSequence) == TRUE) 
		{
			fMotorRunWithoutPrintCmd = CLEAR;
			fMotorRun = CLEAR;
			PrintPageCmd = TRUE;
#if	ENG_TRACE			
			MSG("(Eng)EM_Standby : fPendingPrintPageCmd is receiverd to EMS_Printing2\n");
#endif
			cWakeKey = 0;
			Fuser_Control.EM_State = EMS_Printing;
			return;	
		}
#endif
	}

	// PowerSave ���� Wakeup Cmd�� Wake ���� ��  
	if(fMotorRunWithoutPrintCmd == SET) 
	{
#if	ENG_TRACE		
		MSG("(Eng)EM_Standby : fMotorRunWithoutPrintCmd is receiverd to EMS_Printing\n");
#endif
		cWakeKey = 0;
		StartPrintSequence(PSPS_StartPrintSequence);
		Fuser_Control.EM_State = EMS_Printing;
		return;
	}
	
	if(StartLamination == SET)
	{
		StartLamination = CLEAR;
		StartLaminating1page = CLEAR;
		EndLamination = CLEAR;
		StopLamination = CLEAR;
		ResumeLamination = CLEAR;
		LaminatingMode = CLEAR;
		RunEngineForLamination = SET;
		lami_condition = CON_MTR_STOP_LAMI1;
		CheckExitGuide = SET;
		Fuser_Control.EM_State = EMS_CheckExitSensorForLaminatingStart;
		MSG("(eng)EMS_Standby : to EMS_CheckExitSensorForLaminatingStart\n");
		
		return;
	}
	
#if MAC_TYPE_MH
	if(MainMotor_1MinJogCounter >= JogModeTime[JogModeEnvIdx][JogModeTimeIdx])
#else
	if(MainMotor_1MinJogCounter > 120)	
#endif
	{
		if(JogModeCountFor24v == 1000)
		{
			if(cFatalOptionCommFail == CLEAR)
			{
				JogModeTimeIdx++;
				if(JogModeTimeIdx == MAX_JOGMODETIME)
				{
					JogModeTimeIdx = MAX_JOGMODETIME - 1;
				}
				
				fMainMotorJogMode = SET;
				Set_MotorRunPageParameter();
				Pre_EM_State = EMS_Standby;
				cWakeKey = 0;
				Fuser_Control.EM_State = EMS_JogMode;
	#if	ENG_TRACE			
				MSG("(Eng.App)##### JOG MODE #####\n");
	#endif				
			}
			return;
		}
		JogModeCountFor24v++;
	}
	
	if(fClearErrorsCmdRcv == RECEIVED) 
	{
		if(Set_PrintReadyNotify() == SEND) 			//24Apr09 yjlee
		{
			fClearErrorsCmdRcv = CLEAR;
		}
	}
}

void	EM_PowerSave(void)
{
#if DUPLEX_SLOW_MODE
	Clear_DuplexPrintSlowlyFor15min(cProcessCounter);
#endif
	
	if(fColdWakeupState == CLEAR)
	{
		if (cProcessCounter - cPowerSaveStart >= 20)
		{
			if(f24vOn == TRUE)
			{
				if(FanCtrlEnvIdx == FAN_CTRL_L)
				{
					PS_24vOff(SET);
				}
				else
				{
					f24vOn = FALSE;
					f24vOnForJogMode = TRUE;
				}
				fSensorPowerOffForPWS = SET;
				cSensorPowerOffCounterForPWS = cProcessCounter;
			}
		}
		
		if((cProcessCounter - cSensorPowerOffCounterForPWS >= 10) &&  (fSensorPowerOffForPWS == SET))
		{
	
extern void set_lcd_reset_into_low();
#if 1
			set_lcd_reset_into_low();
			Sensor_Power_Off();
			fSensorPowerOffForPWS = CLEAR;
#else
//			DBG_PRINTF_ERR("kbl %d\n", __LINE__);
//			set_lcd_reset_into_low();
//			posix_sleep_ms(1);
//			DBG_PRINTF_ERR("kbl %d\n", __LINE__);
//			Sensor_Power_Off();
//			posix_sleep_ms(1);
//			fSensorPowerOffForPWS = CLEAR;
//			DBG_PRINTF_ERR("kbl %d\n", __LINE__);
#endif
			gpio_write( eng_hardware->mainmotor_start, 0 ); // BLDC_ST for PWS (High -> Low)
			gpio_write( eng_hardware->mainmotor_dir, 0 );	// BLDC_DIR for PWS (High -> Low)
															// nLSU_MOTOR_CLK for PWS (low -> High)
															
			Tray_Status.Tray1.newTrayStatus = 0x00;
			Tray_Status.Tray1.oldTrayStatus = 0x00;
			Tray_Status.Tray1.emptyTrayStatus = 0x00;
			Tray_Status.Tray2.newTrayStatus = 0x00;
			Tray_Status.Tray2.oldTrayStatus = 0x00;
			Tray_Status.Tray2.emptyTrayStatus = 0x00;
			Tray_Status.Tray3.newTrayStatus = 0x00;
			Tray_Status.Tray3.oldTrayStatus = 0x00;
			Tray_Status.Tray3.emptyTrayStatus = 0x00;
			Tray_Status.Tray4.newTrayStatus = 0x00;
			Tray_Status.Tray4.oldTrayStatus = 0x00;
			Tray_Status.Tray4.emptyTrayStatus = 0x00;
															
			f2ndDrawerState = CLEAR;
			f3ndDrawerState = CLEAR;
			fTray2Notify = CLEAR;
			fUartReadEnable = FALSE;
			
			SendDataToOption1 = CLEAR;
			SendDataToOption2 = CLEAR;
			
			f1stTimeCheckTray1 = SET;
			f1stTimeCheckTray2 = SET;
			f1stTimeCheckTray3 = SET;
			
//			uart_exit();
//			posix_sleep_ms(1);
//			uart_init(VTIME_OUT, VMIN_READ_CHAR);
//			uart_recv_handler_register(UCH_OPTION, (UART_RECV_HANDLER *)&rx_option_handler);
			
			MESSAGE message;
			message.msgType = MSG_STATUSUPDATE;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAYS_UNSTABLE );
			SYMsgSend(PRINTMGRID, &message);
		}
	}
	
	if(!((Fuser_Control.State == ST_POWER_SAVE) || (Fuser_Control.State == ST_COLD_WAKEUP)))
	{
		return;
	}
	
	cWakeKeyCheckPeriod++;
	if(fWakeKey_Onetime == CLEAR)
	{
		if(cWakeKeyCheckPeriod % 3 == 0)
		{
			if(IsWakeKeyState()==TRUE)
			{
				cWakeKey++;
				if((cWakeKey >= 2) && (fWaitWakeKeyRelease == CLEAR))
				{
					cWakeKey = 0;
	//				Sensor_Power_On();
		#if	ENG_TRACE		
					MSG("(Eng)## Wake Key is Detacted : Sensor Power On!!! ##\n");
		#endif					
					usleep(100000);
		//			Set_WakeKeyNotify();
					PowerKeyPressed(POWER_KEY_PRESSED);
					fWaitWakeKeyRelease = SET;
					fWakeKey_Onetime = SET;
					fPressWakeupKey = SET;
				}
			}
			else
			{
				fWaitWakeKeyRelease = CLEAR;
				cWakeKey = 0;
			}
		}
	}
	
//	if (fPendingPrintPageCmd == TRUE)
	if (0)
	{
        if ( EarlyStartCmd ) 	EarlyStartCmd = FALSE;
		bePrintFromPowerSave = TRUE;
		PrintPageCmd = TRUE;
		
		if(fColdWakeupState == FALSE) WakeupProcess();
		
		fPowerSave = FALSE;
#if !ABBA2
#if PWS_TO_PRINT		
		fPwsToPrint = TRUE;
		fMotorRunForPwsToPrint = SET;
#endif
		fPwsToPrint_1stPick = TRUE;
		fSpeedUpForPWS = SET;
		SelectMotorRunTempForPWStoPrint = SET;
#endif
		if (Check_MachineStateBeforeStart() == FALSE)
			return;
		if ( Fuser_Control.RequestState == ST_PRINT ) 
		{
			WakeupTempFromPws = ChangeVoltageToTemp();
			Fuser_Control.EM_State = EMS_Printing;				
		}
		else
		{
			Fuser_Control.RequestState = ST_PRINT;
			WakeupTempFromPws = ChangeVoltageToTemp();
#if	ENG_TRACE
			MSG("(Eng) ## WakeupTempFromPws = %d ##\n",WakeupTempFromPws);
#endif
			//Fuser_Control.EM_State = EMS_Printing; // del by namja
			Fuser_Control.EM_State = EMS_PrintReady;
		}
		HeatingStartTemperature = ChangeVoltageToTemp();
		MSG("(Eng) ## %s : HeatingStartTemperature = %d ##\n",__func__,HeatingStartTemperature);
		fColdWakeupState = FALSE;
		Fuser_Control.fPowerSaveMode = CLEAR;
		fWaitWakeKeyRelease = CLEAR;
	}
	else if ( EarlyStartCmd )
	{
// #if MAC_TYPE_MH	
		// SubFan_On();
// #endif
		bePrintFromPowerSave = TRUE;
		PrintPageCmd = FALSE;
		fColdWakeupState = FALSE;
		Fuser_Control.fPowerSaveMode = CLEAR;
		fWaitWakeKeyRelease = CLEAR;
		
		if((Fuser_Control.RequestState == ST_OFF) || (Fuser_Control.RequestState == ST_POWER_SAVE) || (Fuser_Control.RequestState == ST_COLD_WAKEUP)) 
		{
			fWakeKey_Onetime = CLEAR;
			if(fColdWakeupState == FALSE) WakeupProcess();
			
			fPowerSave = FALSE;
#if !ABBA2
#if PWS_TO_PRINT		
			fPwsToPrint = TRUE;
			fMotorRunForPwsToPrint = SET;
#endif
			fPwsToPrint_1stPick = TRUE;
			fSpeedUpForPWS = SET;
			SelectMotorRunTempForPWStoPrint = SET;
#endif
			JogModeCountFor24v = CLEAR;
			EarlyStartCmd = CLEAR;
			Fuser_Control.fMainMotorState = TRUE;
			HeatingStartTemperature = ChangeVoltageToTemp();
			MSG("(Eng) ## %s : HeatingStartTemperature = %d ##\n",__func__,HeatingStartTemperature);
			Fuser_Control.RequestState = ST_STANDBY;
			StandbyModeSelect(); /* [KSF051502] */
			Fuser_Control.EM_State = EMS_PrintReady;
			return;
		}
	}
	
	else if(ColdWakeUpCmd == ACTIVE)
	{
#if	ENG_TRACE
		MSG("(Eng) ## COLD WAKE UP ##\n");
#endif		
		fWakeKey_Onetime = CLEAR;
		fWaitWakeKeyRelease = CLEAR;
		cWakeKey = 0;
		
		ColdWakeUpCmd = CLEAR;
		fColdWakeupState = TRUE;
		WakeupProcess();
		fPowerSave = FALSE;
		
		Fuser_Control.RequestState = ST_COLD_WAKEUP;
	}
	else if(fColdWakeupState == TRUE)
	{
		if(PowerSaveControl() == TRUE)
		{
#if	ENG_TRACE
			MSG("(Eng) ## COLD WAKE UP => PowerSave ##\n");
#endif
			fWakeKey_Onetime = CLEAR;
			fWaitWakeKeyRelease = CLEAR;
			cWakeKey = 0;
		
			fColdWakeupState = FALSE;
			fPowerSave = TRUE;
			Tray1OnlineChattering = CLEAR;
			Tray2OnlineChattering = CLEAR;
			Tray3OnlineChattering = CLEAR;
			cPowerSaveStart = cProcessCounter;
		}
	}
	
	else if(fClearErrorsCmdRcv == RECEIVED) 			//27Apr09 yjlee
	{
		if(Set_PrintReadyNotify() == SEND) 				//24Apr09 yjlee
		{
			PS_24vOn(SET);
			fPowerSave = FALSE;
			
			SecurityIC_Check();
			SmartICInfoNotify();

			Sensor_Power_On();
			fColdWakeupState = FALSE;
			fClearErrorsCmdRcv = CLEAR;
			fWaitWakeKeyRelease = CLEAR;
		}
	}

#if MAC_TYPE_MH
	if(MainMotor_1MinJogCounter >= JogModeTime[JogModeEnvIdx][JogModeTimeIdx])
#else
	if(MainMotor_1MinJogCounter > 120)	
#endif
	{
		if(JogModeCountFor24v == 1000)
		{
			Sensor_Power_Ctrl(SET);						
			PS_24vOn(CLEAR);
		}
		if(JogModeCountFor24v >= 1010)
		{
			JogModeTimeIdx++;
			if(JogModeTimeIdx == MAX_JOGMODETIME)
			{
				JogModeTimeIdx = MAX_JOGMODETIME - 1;
			}
			gpio_write( eng_hardware->mainmotor_start, 1 );
			gpio_write( eng_hardware->mainmotor_dir, 1 );
			
			fMainMotorJogMode = SET;
			Set_MotorRunPageParameter();
			Pre_EM_State = EMS_PowerSave;
			Fuser_Control.EM_State = EMS_JogMode;
			return;
		}
		JogModeCountFor24v++;
	}
	EM_EngineRestart() ;		/* To EM_RIP_SYNC */
}

void	EM_JogMode(void)
{
	PageSupervisor *now_PGSPVSR;

	MainMotorJogCounter = CLEAR;
	MainMotor_1MinJogCounter = CLEAR;

	if((now_PGSPVSR = Get_MatchToSequenceID(PSS_StopSequence)) != NULL) 
	{
	/*if(now_PGSPVSR->SequenceID == PSS_StopSequence) {*/
		now_PGSPVSR->Flags = PSS_StopSequence;
		Check_MachineStateBeforeStart();
		Fuser_Control.EM_State = Pre_EM_State;
		
		if((Pre_EM_State == EMS_PowerSave) && (fColdWakeupState == CLEAR))
		{
			if(f24vOnForJogMode == CLEAR) PS_24vOff(CLEAR);
			usleep(100*1000);
			Sensor_Power_Ctrl(CLEAR);
			gpio_write( eng_hardware->mainmotor_start, 0 ); // BLDC_ST for PWS (High -> Low)
			gpio_write( eng_hardware->mainmotor_dir, 0 );	// BLDC_DIR for PWS (High -> Low)
		}
		return;
	}
}

void	EM_Printing(void)			/* EMS_Printing */
	/* Set Print Temperature and Now Prining*/
{
	PageSupervisor *now_PGSPVSR;
	Uint8 num;
//	Uint8 pagenum;
//	Uint32 CoolingTime;
	// SubFan_On();
	int PickupOnTime;
	
	if(fSolenoidOnOff > 0)
	{
		if(fSolenoidOnOff == SOLENOID_TRAY1)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
#if PINETREE_L
				//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
				if(1)
#else
				if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
				{
					PickupOnTime = PSTicks_SolenoidOnTime;
				}
				else
				{
					PickupOnTime = PSTicks_SolenoidOnTime_PINTREEL;
				}
			}
			else
			{
#if PINETREE_L
				//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
				if(1)
#else
				if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
				{
					PickupOnTime = PSTicks_SolenoidOnTime_Quiet;
				}
				else
				{
					PickupOnTime = PSTicks_SolenoidOnTime_PINTREEL_Quiet;
				}
			}
		}
		else if(fSolenoidOnOff == SOLENOID_MPT)
		{
			PickupOnTime = PSTicks_SolenoidOnTime_MPT;
		}
		else {}
		
		SolenoidOnTimeCount++;
		
		if(SolenoidOnTimeCount > PickupOnTime)
		{
			if(fSolenoidOnOff == SOLENOID_TRAY1)		TurnOffTray1Solenoid();
			else if(fSolenoidOnOff == SOLENOID_MPT) 	TurnOffMPFSolenoid();
			else {}
			MSG("(eng)Solenoid OFF ID : %d (%d), SolenoidOnTime = %d\n", fSolenoidOnOff, cProcessCounter, cProcessCounter - tmp_SolenoidOnTime);
			SolenoidOnTimeCount = CLEAR;
			fSolenoidOnOff = CLEAR;
		}
	}
	
	if(fPreRunBeforePrintCmd == SET)
	{
#if	ENG_TRACE		
		MSG("(Eng)## Early Start Cmd (0xb1) is received during EM_Priting :IGNORE ##\n");
#endif			
		fPreRunBeforePrintCmd = CLEAR;
	}
	
	num = HowManyActivePageSupervisor();
	
	if(num < 1) 
	{
#if	ENG_TRACE		
		MSG("(Eng)$$RequestState : ST_STANDBY1$$\n");
#endif	
		cKeepMotorRunForCooling = 0;
		fDuplexSet = CLEAR;
//~ #if MAC_TYPE_MH
		StandbyModeSelect(); /* [KSF051502] */
//~ #endif
		Fuser_Control.RequestState = ST_STANDBY;
		Check_MachineStateBeforeStart();
		_Set_EM_State(EMS_Standby);
		return;
	}
	else if(num < 2)
	{
		if(Get_TopPageSupervisorSequenceID() > PSPS_EndOfPrintPage) 
		{
//~ #if MAC_TYPE_MH	
// #if 0		
			if(fDuplexSet == TRUE)
			{
				if(cDuplexPageCount < 300)
				{
					Fuser_Control.FanStopLongDelayInStandby = CLEAR;
				}
				else
				{
					Fuser_Control.FanStopLongDelayInStandby = SET;
				}
#if 0 // delete cooling time			
				CoolingTime = CoolingTimeTable[EnvTempStatus][pagenum];
				
				if(fCoolingState == CLEAR)
				{
					Set_CoolingNotify(SET);
				}
				
				cKeepMotorRunForCooling++;
				if((cKeepMotorRunForCooling < CoolingTime) && (fPendingPrintPageCmd != RECEIVED))
				{
					// MSG("(eng)$$ CoolingTime = %d [%d][%d] cKeepMotorRunForCooling = %d, CurTemp = %d $$\n",CoolingTime, EnvTempStatus, pagenum, cKeepMotorRunForCooling, Fuser_Control.Current_Temperature);
					Fuser_Control.fDuplexJob = 2;
					Fuser_Control.Target_Temperature = Temp_033C;
					return;
				}

				Fuser_Control.Target_Temperature = Temp_165C;
				if(ChangeVoltageToTemp() < 160)
				{
					// MSG("(eng)$$ CurTemp = %d $$\n",Fuser_Control.Current_Temperature);
					return;
				}
#endif
			}
			else
			{
				Fuser_Control.FanStopLongDelayInStandby = CLEAR;
			}
			
			if(fCoolingState == SET)
			{
				Set_CoolingNotify(CLEAR);
			}
//~ #endif
			cKeepMotorRunForCooling = 0;
			fDuplexSet = CLEAR;
//~ #if MAC_TYPE_MH
			StandbyModeSelect(); /* [KSF051502] */
//~ #endif
			Fuser_Control.RequestState = ST_STANDBY;
			Fuser_Control.EM_State = EMS_ExitLastPage;
			return ;
		}
	}
	else 
	{
	    if((now_PGSPVSR = Get_MatchToSequenceID(PSS_StopSequence)) != NULL) 
		{
			now_PGSPVSR->Flags = PSS_StopSequence;
		}
	}
#if !ABBA2
	cWakeKeyCheckPeriod++;
	if(cWakeKeyCheckPeriod % 3 == 0)
	{
		if(IsWakeKeyState() == TRUE)
		{
			cWakeKey++;
			if((cWakeKey >= 2) && (fWaitWakeKeyRelease == CLEAR))
			{
				cWakeKey = 0;
	#if	ENG_TRACE		
				MSG("(Eng)## Wake Key is Detected In Printing!!! ##\n");
	#endif					
				PowerKeyPressed(KEY_OR_TOUCH_PRESSED);
				fWaitWakeKeyRelease = SET;
			}
		}
		else
		{
			fWaitWakeKeyRelease = CLEAR;
			cWakeKey = 0;
		}
	}
#endif
	if(fChangeTargetTemp == SET)
	{
#if	ENG_TRACE			
		// MSG("(Eng)## Fuser_Control.State = %d ##\n",Fuser_Control.State);
#endif
		if((Fuser_Control.State == ST_STANDBY) || (Fuser_Control.State == ST_COOL_STANDBY))
		{
// #if MAC_TYPE_MH	
			// SubFan_On();
// #endif
			Fuser_Control.RequestState = ST_PRINT;
			fChangeTargetTemp = CLEAR;
		}
	}
	
	if(fWaitPrintPageCmd == SET)
	{
		if((fPendingPrintPageCmd == RECEIVED) && (fWaitToChangeMotorSpeed == CLEAR))
		{
#if	ENG_TRACE			
			MSG("(Eng)## fPendingPrintPageCmd is RECEIVED during Pre Run!!! ##\n");
#endif				
			PrintPageCmd = TRUE;
			ChangePrintPageInfo();
		}
		else if(fWaitToChangeMotorSpeed == SET)
		{
			MSG("(eng)## fPendingPrintPageCmd is RECEIVED  BUT Go to standby!!!\n");
			fMotorRunWithoutPrintCmd = SET;
			fWaitPrintPageCmd = CLEAR;
		}
		else {}
	}
	// ChangePrintPageInfo(); ���� fWaitPrintPageCmd�� clear �Ǵ� ���� �����ϸ� �ȵȴ� 
	// - 300 timing�� fPendingPrintPageCmd�� �ް� 301�� ChangePrintPageInfo(); ó���� �Ʒ� �Լ��� ����� Time out���� Printing Stop�� �߻��� ���� ���� 
	if(fWaitPrintPageCmd == SET)	
	{
		if(((cProcessCounter-cPreRunTimeoutCounter) > 500) && (fPendingPrintPageCmd != RECEIVED))
		{
#if	ENG_TRACE			
			MSG("(Eng)## TIMEOUT : fPendingPrintPageCmd is not RECEIVED during Pre Run!!! cProcessCounter =%d,cPreRunTimeoutCounter=%d##\n",cProcessCounter,cPreRunTimeoutCounter);
#endif
			Set_IgnoreEarlyStartNotify();
			fWaitPrintPageCmd = CLEAR;
			fClearWaitPrintPageCmd = TRUE;
			StandbyModeSelect(); /* [KSF051502] */
			StopAllPortsInEngine();
			Fuser_Control.RequestState = ST_STANDBY;
			Fuser_Control.EM_State = EMS_PowerOnResetState;
		}
	}
	
	if((fMotorRunWithoutPrintCmd == SET) && (fWaitToChangeMotorSpeed == CLEAR))
	{
		if(fPendingPrintPageCmd == RECEIVED) 
		{
#if	ENG_TRACE			
			MSG("(Eng)## fPendingPrintPageCmd is RECEIVED during fMotorRunWithoutPrintCmd!!! ##\n");
#endif				
#if DEBUG_EARLY
			fMotorRunWithoutPrintCmd = CLEAR;	
			fMotorRun = CLEAR;
			PrintPageCmd = TRUE;
			fWaitPrintPageCmd = SET;
			// ChangePrintSequence(PSPS_StartPaperPickupSequence);
			ChangePrintPageInfo();
#else
			// ChangePrintSequence(PSPS_StartPaperPickupSequence);
			ChangePrintPageInfo();
			fMotorRunWithoutPrintCmd = CLEAR;	
			fMotorRun = CLEAR;
			PrintPageCmd = TRUE;
#endif
			return;
		}
	}
	
	if(fcancelpage == SET)
	{
		MSG("(eng)Cancel page : go to standby\n");
		fcancelpage = CLEAR;
		StandbyModeSelect(); /* [KSF051502] */
		StopAllPortsInEngine();
		Fuser_Control.RequestState = ST_STANDBY;
		Fuser_Control.EM_State = EMS_PowerOnResetState;
	}
	EM_StateCounter++;
	
	if(EM_StateCounter < /*1500*/EMTicks_5min)
	{
//~ #if MAC_TYPE_MH
		// if(((cDuplexPageCount >= Duplex1stNorPageNum) && (((cDuplexPageCount - Duplex1stNorPageNum) % Duplex2ndNorPageNum) == 0))
		// && (Get_LastPageSupervisorDestination() != OutputDuplexor))
		if(0)
//~ #else
		//~ if(0)
//~ #endif
		{
// #if DUPLEX_COOLING
#if MAC_TYPE_MH
			StartPrintSequence(PSPS_DuplexCooling);
#endif
		}
		else
		{
			StartPrintSequence(PSPS_PickupForContinue);
		}
	}
	else 
	{
	       	/* After 5 min continuous print page, Start EP Cleaning Mode */
#if LAST_PAGESUPERVISOR
		if(Get_LastPageSupervisorDestination() != OutputDuplexor) 
		{ 
			/* Print Simplex or Duplex */
			if(StartPrintSequence(PSPS_StartEPCleaning) == TRUE) 
			{
				EM_StateCounter = 0;
			}
		}
		else 
		{
			/* Print Duplex Front(2nd) Pape */
			StartPrintSequence(PSPS_PickupForContinue);
	  	}
#else
		now_PGSPVSR = Get_TopPageSupervisor(); 
		if(now_PGSPVSR->OutputDestination != OutputDuplexor) 
		{ 
			if(now_PGSPVSR->Source == MS_Duplex) 
			{ 
				if((Get_NextPageSupervisor())->Active == CLEAR) 
				{
					if(StartPrintSequence(PSPS_StartEPCleaning) == TRUE) 
					{
						EM_StateCounter = 0;
					}
				}
			}
			else 
			{
				if(StartPrintSequence(PSPS_StartEPCleaning) == TRUE) 
				{
					EM_StateCounter = 0;
				}
			}
		}
		else 
		{
			StartPrintSequence(PSPS_PickupForContinue);
	  	}
#endif
	}
}

void	EM_CheckExitSensorForLaminatingStart(void)			/* EMS_CheckExitSensorForLaminatingStart */
{
	if(CheckExitGuide == SET)
	{
		CheckExitGuide = CLEAR;
		if(IsPaperPresentOnPexs() == TRUE)
		{
			Set_Laminate_PGSPVSR_Parameter(PSL_ServoSearchStart_PreHeating);
			Fuser_Control.EM_State = EMS_Laminating_Servo;
			MSG("(eng)EMS_CheckExitSensorForLaminatingStart : to EMS_Laminating_Servo\n");
		}
		else
		{
			/* send exit sensor msg */
			LaminatingSendMsgToPM(STATUS_LAMINATEINFO_EXITGUIDE_CLEAR);
		}
	}
	
	if(EndLamination == SET)
	{
		Fuser_Control.EM_State = EMS_Laminating_Standby;
		MSG("(eng)EMS_CheckExitSensorForLaminatingStart : to EMS_Laminating_Standby\n");
	}
		
	return;
}

void	EM_CheckExitSensorForLaminatingEnd(void)			/* EMS_CheckExitSensorForLaminatingEnd */
{
	if(CheckExitGuide == SET)
	{
		CheckExitGuide = CLEAR;
		if(IsPaperPresentOnPexs() == TRUE)
		{
			/* send exit sensor msg */
			LaminatingSendMsgToPM(STATUS_LAMINATEINFO_EXITGUIDE_SET);
		}
		else
		{
			LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMIJOBDONE);
			Fuser_Control.EM_State = EMS_Standby;
			
			SecurityIC_Check();
			SmartICInfoNotify();
			
			RunEngineForLamination = CLEAR;
			MSG("(Eng)## tempEngState = %x ##\n",clear_engine_state());
			MSG("(eng)EMS_CheckExitSensorForLaminatingEnd : to EMS_Standby\n");
		}
	}
	
	if(ResumeLamination == SET)
	{
		Set_Fuser_Laminating_Standby();
		Fuser_Control.EM_State = EMS_Laminating_Standby;
		MSG("(eng)EMS_CheckExitSensorForLaminatingEnd : to EMS_Laminating_Standby\n");
	}
	
	return;
}

void	EM_Laminating_Servo(void)			/* EMS_Laminating_Servo */
{
	if(Get_TopPageSupervisor()->Flags == PSL_ServoSearchDone_PreHeating)
	{
#if	ENG_TRACE		
		MSG("(Eng)EMS_Laminating_Servo : to EMS_PreHeating\n");
#endif		
		FinishPageSupervisor(Get_TopPageSupervisor());
		PreHeatingDoneTime = GetPreHeatingDoneTime();
		LaminatingSendMsgToPM(STATUS_LAMINATEINFO_PREHEATING);
		Set_Laminate_PGSPVSR_Parameter(PSL_EP_InitializeFor_PreHeating);
		Fuser_Control.EM_State = EMS_PreHeating;
		MSG("(eng)EMS_Laminating_Servo : to EMS_PreHeating\n");
		Set_Fuser_PreHeating();
	}
	
	return;
}

void	EM_PreHeating(void)			/* EMS_PreHeating */
{
	if((Get_TopPageSupervisor()->Flags == PSL_End_Laminating) || (EndLamination == SET))
	{
#if	ENG_TRACE		
		MSG("(Eng)EMS_PreHeating : to EMS_Laminating_Standby\n");
#endif		
		EM_StateCounter = CLEAR;
		LaminatingSendMsgToPM(STATUS_LAMINATEINFO_PREHEATDONE);
		FinishPageSupervisor(Get_TopPageSupervisor());
		Set_Fuser_Laminating_Standby();
		Fuser_Control.EM_State = EMS_Laminating_Standby;
	}
	
	return;
}

void	EM_Laminating(void)			/* EMS_Laminating */
{
	if(JamCode_Laminating != JAM_CLEAR_LAMINATING)
	{
		MSG("(Eng)EMS_Laminating : to EMS_Laminating_Error\n");
		Fuser_Control.RequestState = ST_OFF;
		Fuser_Control.EM_State = EMS_Laminating_Error;
	}
	else
	{
		if((Get_TopPageSupervisor()->Flags == PSL_End_Laminating) 
//		|| ((ResumeLamination == SET) && (Get_TopPageSupervisor()->Flags < PSL_StartPaperPickupSequence_Laminating))
		|| (EndLamination == SET))
		{
//			MSG("(eng)Flags = %d, ResumeLamination = %d, EndLamination = %d\n", Get_TopPageSupervisor()->Flags, ResumeLamination, EndLamination);
//			if((ResumeLamination == SET) && (Get_TopPageSupervisor()->Flags < PSL_StartPaperPickupSequence_Laminating))
//			{
//				StopMainMotor();
//			}
			ResumeLamination = CLEAR;
			EM_StateCounter = CLEAR;
			FinishPageSupervisor(Get_TopPageSupervisor());
			LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMINATINGDONE);
			MSG("(Eng)EMS_Laminating : to EMS_Laminating_Standby\n");
			Set_Fuser_Laminating_Standby();
			Fuser_Control.EM_State = EMS_Laminating_Standby;
			Fuser_Control.RequestState = ST_STANDBY;
		}
	}
}

void	EM_Laminating_Standby(void)			/* EMS_Laminating_Standby */
{
	ResumeLamination = CLEAR;
	if(EndLamination == SET)
	{
		EndLamination = CLEAR;
		EM_StateCounter = CLEAR;
		fRetryPickUp = CLEAR;
		fLastPickUp = CLEAR;
		FinishPageSupervisor(Get_TopPageSupervisor());
		
		Set_Laminate_PGSPVSR_Parameter(PSL_StartEndProcess_Laminating);
		CheckExitGuide = SET;
		MSG("(Eng)EMS_Laminating_Standby : to EMS_Laminating_End\n");
		Fuser_Control.EM_State = EMS_Laminating_End;
		init_temperature();
/*		
		MSG("(Eng)EMS_Laminating_Standby : to EMS_Laminating_End\n");
		Fuser_Control.EM_State = EMS_Laminating_End;
*/
	}
	else if(StartLaminating1page == SET)
	{
		StartLaminating1page = CLEAR;
		MSG("(Eng)EMS_Laminating_Standby : to EMS_Laminating\n");
		SelectPreheatingCount = cProcessCounter;
		FinishPageSupervisor(Get_TopPageSupervisor());
		LaminatingMode = SET;
		Set_Laminate_PGSPVSR_Parameter(PSL_EP_InitializeFor_PreHeating);
		LaminatingSendMsgToPM(STATUS_LAMINATEINFO_HEATING);
		Set_Fuser_PreHeating();
		Fuser_Control.EM_State = EMS_Laminating;
		//Fuser_Control.RequestState = ST_PRINT;
	}
	else
	{
/*
		if(EM_StateCounter > 10*60*100)
		{
			EM_StateCounter = CLEAR;
			MSG("(Eng)Laminate TimeOut\n");
			LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMIJOBDONE);
		}
*/
	}
	EM_StateCounter++;
	
	return;
}

void	EM_Laminating_End(void)			/* EMS_Laminating_End */
{
	if(Get_TopPageSupervisor()->Flags == PSL_End_Laminating) 
	{
		MSG("(Eng)EMS_Laminating_End : to EMS_CheckExitSensorForLaminatingEnd\n");
		FinishPageSupervisor(Get_TopPageSupervisor());
		Fuser_Control.EM_State = EMS_CheckExitSensorForLaminatingEnd;
		init_temperature();
	}
	
	return;
}

void	EM_Laminating_Error(void)			/* EMS_Laminating_Error */
{
	MESSAGE message;
	
	message.msgType = MSG_STATUSUPDATE;
	switch(JamCode_Laminating)
	{
		case JAM_INPUT_DETECT_LAMINATING:
			MSG("(eng)JAM_INPUT_DETECT_LAMINATING\n");
			message.param1 = STATUS_PAPER_JAM_TRAY4;
			//LaminatingSendMsgToPM(STATUS_LAMINATEINFO_JAM_INPUT_DETECT);
			break;
		case JAM_INPUT_RELEASE_LAMINATING:
			MSG("(eng)JAM_INPUT_RELEASE_LAMINATING\n");
			message.param1 = STATUS_PAPER_JAM_INPSEN_NOT_REL;
			//LaminatingSendMsgToPM(STATUS_LAMINATEINFO_JAM_INPUT_RELEASE);
			break;
		default:
			MSG("(eng)JAM_INPUT_DETECT_LAMINATING(default)\n");
			message.param1 = STATUS_PAPER_JAM_TRAY4;
			//LaminatingSendMsgToPM(STATUS_LAMINATEINFO_JAM_INPUT_DETECT);
			break;
	}
	SYMsgSend(PRINTMGRID, &message);
	
	Fuser_Control.EM_State = EMS_Laminating_Recovery;
	MSG("(Eng)EMS_Laminating_Error : to EMS_Laminating_Recovery\n");

	return;
}

void	EM_Laminating_Recovery(void)			/* EMS_Laminating_Recovery */
{
	MESSAGE message;
	
	if(fClearErrorsCmdRcv == SET)
	{
		fClearErrorsCmdRcv = CLEAR;
		
		LaminatingMode = CLEAR;
		PreHeatingDoneTime = CLEAR;
		JamCode_Laminating = JAM_CLEAR_LAMINATING;
		Fuser_Control.RequestState = ST_STANDBY;
		
		message.msgType = MSG_STATUSUPDATE;
		message.param1 = STATUS_ONLINE;
		SYMsgSend(PRINTMGRID, &message);
		
		LaminatingSendMsgToPM(STATUS_LAMINATEINFO_PREHEATING);
		
		Set_Laminate_PGSPVSR_Parameter(PSL_EP_InitializeFor_PreHeating);
		Fuser_Control.EM_State = EMS_PreHeating;
		Set_Fuser_PreHeating();
		MSG("(Eng)EMS_Laminating_Recovery : to EMS_PreHeating\n");
	}
	
	return;
}

void	EM_Laminating_CloseCounter(void)			/* EMS_Laminating_CloseCounter */
{
	if(IsDoorCoverState() == CLOSE) 
	{
		EM_StateCounter++;
		if(EM_StateCounter > PSTicks_500ms) 
		{
			fCoverOpenDebugMsg = TRUE;
			Fuser_Control.EM_State = EMS_Laminating_CloseDoorCover;
		}
	}
	else 
	{
		EM_StateCounter = CLEAR;
	}
	
	return;
}

void	EM_Laminating_CloseDoorCover(void)			/* EMS_Laminating_CloseDoorCover */
{
	MESSAGE message;
//	uint32_t tempEngState;
	Sensor_Power_On();
	Motor_FWD();
	
	fDoorCoverError = CLEAR;
	fClearErrorsCmdRcv = CLEAR;
		
	MSG("(Eng)## EM_CloseDoorCover(Laminate) ##\n");
	//LaminatingSendMsgToPM(STATUS_LAMINATEINFO_FRONTCOVER_CLOSE);
	message.msgType = MSG_STATUSUPDATE;
	message.param1 = STATUS_DOORCLOSE_FRONT;
	SYMsgSend(PRINTMGRID, &message);
	
	if(SecurityIC_Authentication_Laminate() == TRUE)
	{
		//LaminatingSendMsgToPM(STATUS_LAMINATEINFO_CARTRIDGE_SET);
		message.param1 = STATUS_TONER_NO_ISSUE;
		if(IsPaperPresentOnPins() == FALSE)
		{
			fClearErrorsCmdRcv = SET;
			Fuser_Control.EM_State = EMS_Laminating_Recovery;
		}
		else
		{
			JamCode_Laminating = JAM_INPUT_RELEASE_LAMINATING;
			Fuser_Control.EM_State = EMS_Laminating_Error;
		}
	}
	else
	{
		//LaminatingSendMsgToPM(STATUS_LAMINATEINFO_CARTRIDGE_CLEAR);
		message.param1 = STATUS_PAPER_JAM_REINSERTCARTRIDGE;
		Fuser_Control.EM_State = EMS_Laminating_Recovery;
	}
	SYMsgSend(PRINTMGRID, &message);
		
	return;
}

void 	EM_ExitLastPage(void)				/* EMS_ExitLastPage */
	/* Carry a last printed page to exit */
{
	Uint8 id;
		
	id = Get_TopPageSupervisorSequenceID();
	
	if(id >= PSS_StartEPCleaning) 
	{
		Fuser_Control.EM_State = EMS_EPCleaning;
		return;
	}
	else if(id == NONE) 
	{
		Check_MachineStateBeforeStart();
		_Set_EM_State(EMS_Standby);
		return;
	}
	
	EM_StateCounter++;
	if(EM_StateCounter < /*1500*/EMTicks_5min/* EMTicks_10min*/) 
	{
		if(StartPrintSequence(PSPS_PickupForContinue) == TRUE) 
		{
			Fuser_Control.EM_State = EMS_Printing;
		}
	}
	else {
#if LAST_PAGESUPERVISOR
		if(Get_LastPageSupervisorDestination() != OutputDuplexor) 
		{ 
			/* Print Simplex or Duplex */
			if(StartPrintSequence(PSPS_StartEPCleaning) == TRUE) 
			{
				EM_StateCounter = 0;
				Fuser_Control.EM_State = EMS_Printing;
			}
		}
		else 
		{
			/* Print Duplex Front(2nd) Pape */
			StartPrintSequence(PSPS_PickupForContinue);
	  	}
#else
		if(StartPrintSequence(PSPS_StartEPCleaning) == TRUE) 
		{
			Fuser_Control.EM_State = EMS_Printing;
			EM_StateCounter = 0;
		}
#endif
	}		
}

void 	EM_EPCleaning(void)				/* EMS_EPCleaning */
	/* End of Page Sequence (After EP Cleaning) */
{
	Uint8 id;
	
	if((id = Get_TopPageSupervisorSequenceID()) == PSS_StopSequence) 
	{
#if	ENG_TRACE		
		MSG("(Eng)EM_POR_Standby : to EMS_PrinterStop\n");
#endif		
		Fuser_Control.EM_State = EMS_PrinterStop;
		return;
	}
	else if(id == NONE) 
	{
#if	ENG_TRACE		
		MSG("(Eng)EM_POR_Standby : to EMS_Standby\n");
#endif		
		Check_MachineStateBeforeStart();
		_Set_EM_State(EMS_Standby);
		return;
	}
	
	if(fPOR_PageSupervisor_Act == CLEAR) 
	{
		if((Get_TopPageSupervisor()->Flags) < PSS_StopMainMotor)
		{
			if(StartPrintSequence(PSPS_PickupForContinue) == TRUE) 
			{
	#if	ENG_TRACE			
				MSG("(Eng)EM_POR_Standby : to EMS_Printing\n");
	#endif			
				Fuser_Control.EM_State = EMS_Printing;
				EM_StateCounter = CLEAR;
			}
		}
	}
	
	if(fSmartIC_Authentication == TRUE)
	{
#ifdef SMART_IC_CON		
//#if SECURITY_IC_ATTACHED /* [KSR040801] */
//#if	ENG_TRACE		
//		 MSG("(Eng)### SecurityIC_Check cProcessCounter = %d \n",cProcessCounter);
//#endif			
//		 SecurityIC_Check();
//		 fSmartIC_Authentication = CLEAR;
//#endif	
#endif 	// #ifdef SMART_IC_CON	
	}
}

void 	EM_PrinterStop(void)			/* EMS_PrinterStop */
{
	PageSupervisor *now_PGSPVSR;
	
	if((now_PGSPVSR = Get_MatchToSequenceID(PSS_StopSequence)) != NULL) 
	{
		now_PGSPVSR->Flags = PSS_StopSequence;
#if	ENG_TRACE		
		MSG("(Eng)EM_PrinterStop : to PSS_StopSequence\n");
#endif		
	}
	else
	{
		/* Check Machine State */
		Check_MachineStateBeforeStart();
		_Set_EM_State(EMS_Standby);
#if	ENG_TRACE		
		MSG("(Eng)EM_PrinterStop : to EMS_Standby\n");
#endif	
		fCheckJobNumChange = CLEAR;
		fCheckJobInfoChange = CLEAR;
		fJobIDChanged = CLEAR;
		fJobInfoChanged = CLEAR;
		fWaitToChangeMotorSpeed = CLEAR;
#if IPG_ENABLE
		fVideo_Print_Enable = SET;
		fReVideoOn = CLEAR;
#endif	
#if BLI_TEST
		Fuser_Control.LowSpeedFan_Wakeup = CLEAR;
#endif
	}
}

void	EM_OpenDoorCover(void)
{
	if(Set_CoverOpenNotify() == TRUE)
	{
		_Set_EM_State(EMS_CloseCounter);
	}
}

void	EM_CloseCounter(void)
{
	if(IsDoorCoverState() == CLOSE) 
	{
		EM_StateCounter++;
		if(EM_StateCounter > PSTicks_500ms) 
		{
			fCoverOpenDebugMsg = TRUE;
			Fuser_Control.EM_State = EMS_CloseDoorCover;
		}
	}
	else 
	{
		EM_StateCounter = CLEAR;
	}
	EM_EngineRestart();		/* EMS_RIP_SYNC */
}

void	EM_CloseDoorCover(void)
{
#if FULLSTACK_CHECK
	int i;
#endif
	MESSAGE message;
	uint32_t tempEngState;
	Sensor_Power_On();
	Motor_FWD();
	fDoorCoverError = CLEAR;
	fFrontCoverState = CLOSE;
	fClearErrorsCmdRcv = CLEAR;
	cFullStackDetacted = CLEAR;
	
	engine_state.cartridge_lock = false;
	engine_state.cartridge_rec_fail = false;
		
	message.msgType = MSG_STATUSUPDATE;
	engine_state.front_door_open = false;
	slogit( engine_state.pm_statusrequest = message.param1 = STATUS_DOORCLOSE_FRONT );
	SYMsgSend(PRINTMGRID, &message);
		
	if(Set_CoverClosedNotify() == SEND) 
	{
#if	ENG_TRACE		
		MSG("(Eng)## EM_CloseDoorCover ##\n");
#endif
#if 0
		tempEngState = clear_engine_state();
		if( (tempEngState == STATUS_ONLINE) || ( (tempEngState == STATUS_PAPER_JAM_TONER_BLOCK) || (tempEngState == STATUS_PAPER_JAM_REINSERTCARTRIDGE) ))
		{
#if	ENG_TRACE		
			MSG("(Eng)## tempEngState = %x ##\n",tempEngState);
#endif			
			SecurityIC_Check(); /* [KSR032101] for SMC */
			SmartICInfoNotify(); /* [KSR0401] */
		}
#endif		
		SecurityIC_Check(); /* [KSR032101] for SMC */
		SmartICInfoNotify(); /* [KSR0401] */
		
#if FULLSTACK_CHECK				
		watchdog_service();
		for(i = 0; i < 100; i++)
		{
			if(IsPaperPresentOnStack() == PRESENT)
			{
				cFullStackDetacted++;
			}
			usleep(10*1000);
		}
		if(cFullStackDetacted == 100)
		{
			fState_FullStack = TRUE;
			fFullStack_Error = TRUE;
			full_stack_notify(FullStackSet);
#if	ENG_TRACE				
			MSG("(Eng)## FullStack State Detacted ##\n");
#endif	
		}
		else
		{
			if(fState_FullStack == SET)
			{
				fState_FullStack = CLEAR;
				fFullStack_Error = CLEAR;
				full_stack_notify(FullStackClear);
				cFullStackReleased = 0;
			}
		}
		cFullStackDetacted = CLEAR;
#endif		

		tempEngState = clear_engine_state();
		MSG("(Eng)## tempEngState = %x ##\n",tempEngState);
		
		fWaitPageCmd = TRUE;
		//message.msgType = MSG_STATUSUPDATE;
		//slogit( engine_state.pm_statusrequest = message.param1 = STATUS_ONLINE ); 
		//clear_engine_state();
		//SYMsgSend(PRINTMGRID, &message);
		
		if(Fuser_Control.fFatalError == CLEAR) fClearErrorsCmdRcv = SET;
		
		Fuser_Control.EM_State = EMS_RecoverError;
	}
	else 
	{
		fDoorCoverError = ERROR;
	}
}

void	EM_RecoverPaperJam(void)
	/* For Operational Error and Recovering Process */
	/* Recoverable Error by Operator */
{
	if(cJamRecoverTimer==0)
	{
//		Set_PaperJamNotify(TempPaperJam.DR.BYTE, CodeOfJamStatus);
		Set_JamCheckNotify(CodeOfJamStatus);
		ClearAllPageSupervisor();	// ver 2.2.19 : Jam ��Ȳ �߻� ���� 850msec �� �����ֱ⿡�� ���� �߻�. 850msec �߰��� ����ִ� pagesupervisor �� ���� ���� �� �ֽ�. ���� �߰�
		DisableAllOutputPorts(); 	// ver 3.2.33 : Jam ��Ȳ �߻� ���� 850msec �� �����ֱ⿡�� ���� �߻�. 850msec �߰��� ����ִ� pagesupervisor �� ���� ���� �� �ֽ�. ���� �߰� 
/*		if(f2ndDrawerState == SET) 
		{
//			fTray2MotorRun = TRUE;
			Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS);
		}
		if(f3ndDrawerState == SET) 
		{
//			fTray3MotorRun = TRUE;
			Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORRUNSPEED_KARAS);
		}*/
		switch(machineType)
		{
			case ABBA_OAK_L:
				MotorRunTimeAfterJam = RUNNING_TIME_AFTER_JAM_L;
				break;
			case ABBA_OAK_M:
				MotorRunTimeAfterJam = RUNNING_TIME_AFTER_JAM_M;
				break;
			case ABBA_OAK_H:
				MotorRunTimeAfterJam = RUNNING_TIME_AFTER_JAM_H;
				break;
			default:
				MotorRunTimeAfterJam = RUNNING_TIME_AFTER_JAM_L;
				break;
		}
		
	}
	
	cJamRecoverTimer++;
	if(cJamRecoverTimer <= MotorRunTimeAfterJam)
	{
		return;
	}

	/* To avoid to stop printer during turning on the solenoid. */
	if(cJamRecoverTimer == MotorRunTimeAfterJam + 1)
	{
		if ((Get_MatchToSequenceID(PSPS_DetectStartToPins)
				== (PageSupervisor *)NULL)
			&& (Get_MatchToSequenceID(PSPS_FinishPaperPickup)
				== (PageSupervisor *)NULL)
			&& (Get_MatchToSequenceID(PSPS_FeedJamOutFuser)  /* VerADS continue feeding label past fuser */
                == (PageSupervisor *)NULL) )
		{
			StopAllPortsInEngine() ;
		}
	}
	
	if(cJamRecoverTimer <= MotorRunTimeAfterJam + 12)		// waiting 120msec 
	{
		return;
	}
	
	if(cJamRecoverTimer == MotorRunTimeAfterJam + 13)
	{
		Motor_BWD();						// Motor run : BWD
		StartMainMotor();
	}
	
	if(cJamRecoverTimer <= MotorRunTimeAfterJam + 17)		// Run 50msec BWD and Stop
	{
		return;
	}
	
	Motor_FWD();
	StopMainMotor();
#if	ENG_TRACE
	MSG("StopMainMotor 1\n");
#endif
	
	if(f2ndDrawerState == SET) 
	{
//		fTray2MotorStop = TRUE;
		Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
	}
	if(f3ndDrawerState == SET)
	{
//		fTray3MotorStop = TRUE;
		Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
	}
	
	cJamRecoverTimer=0;
	
	if (HowManyActivePageSupervisor() == 0)
	{
		Fuser_Control.RequestState = ST_OFF; 
		CodeOfJamStatus = JAM_IDLE_STATE;
		fClearErrorsCmdRcv = 0 ;
		Fuser_Control.EM_State = EMS_RecoverError;
		return ;
	}
}

void	EM_RecoverPaperEmpty(void)
{
	PageSupervisor	*PGSPVSR ;
	Uint8	n ;
	// Uint8 temp_DeviceID, temp_EmptyState;

	if ((PGSPVSR = Get_MatchToSequenceID(PSS_StopSequence))
		!= (PageSupervisor *)NULL)
	{
		PGSPVSR->Flags = PSS_StopSequence;
//~ #if MAC_TYPE_MH
		StandbyModeSelect(); /* [KSF051502] */
//~ #endif
		Fuser_Control.RequestState = ST_STANDBY;
		ClearAllPageSupervisor() ;
#if POR_CHECK_FULL
		StopAllPortsInEngine() ;
#else
	}
		/* '98/May/07 Add For Source Empty Sequence */
	else if ((PGSPVSR = Get_MatchToSequenceID(PSS_FinishEPCleaning))
		!= (PageSupervisor *)NULL)
	{
		StopAllPortsInEngine() ;
//~ #if MAC_TYPE_MH
		StandbyModeSelect(); /* [KSF051502] */
//~ #endif
		Fuser_Control.RequestState = ST_STANDBY;
#endif
	}

	/* To report Source Empty earlier */
	if (PaperEmpty.DR.BYTE != 0)
	{
		if(1)
		{
			fClearErrorsCmdRcv = CLEAR;
			PaperEmpty.DR.BYTE = CLEAR;
			TempPaperEmpty.DR.BYTE &= 0x02;
		}
	}
	else
	{
		n = HowManyActivePageSupervisor() ;
		if (n == 0) {
			Check_MachineStateBeforeStart() ;	
			Fuser_Control.RequestState = ST_OFF; 
			_Set_EM_State(EMS_RecoverError);
		}
		else if (n == 1)	
		{
			if ((fPendingPrintPageCmd == TRUE) /*|| (Set_PrintReadyNotify() == SEND)*/)			//24Apr09 yjlee
			{
				PaperEmpty.DR.BYTE = CLEAR;
				TempPaperEmpty.DR.BYTE &= 0x02;

				fClearErrorsCmdRcv = 0 ;
				fEngineStateFull = CLEAR;
				Fuser_Control.EM_State = EMS_ExitLastPage ;
			}
		}
		else
			EM_StateCounter++ ;
	}
}

void	EM_RecoverFullStack(void)
{
	if(fState_FullStack == TRUE)
	{
		if(IsPaperPresentOnStack() != PRESENT)
		{
			cFullStackReleased++;
			if(cFullStackReleased >= 150)
			{
				ClearAllPageSupervisor();
				
				fState_FullStack = CLEAR;
				fFullStack_Error = CLEAR;
//				Set_FullStackNotify(FullStackClear);
				full_stack_notify(FullStackClear);
				
				// Set_CoverOpenNotify();
				// usleep(10000);
				// Set_CoverClosedNotify();
				cFullStackReleased = 0;
				Fuser_Control.RequestState = ST_OFF; 
//				fClearErrorsCmdRcv = 0 ;
				fClearErrorsCmdRcv = RECEIVED;
				fWaitPageCmd = TRUE;
				Fuser_Control.EM_State = EMS_RecoverError;
#if	ENG_TRACE				
				MSG("(Eng)## FullStack State Released ##\n");
#endif				
			}
		}
	}
//~ #endif
}

void	EM_RecoverNoStopError(void)
{
	PageSupervisor *PGSPVSR;
#if	ENG_TRACE	
	MSG("(Eng)EM_RecoverNoStopError\n");
#endif	
	/* When other page sups have stopped, we can then go to the RecoverError state */
	if(((PGSPVSR = Get_MatchToSequenceID(PSS_StopSequence)) != (PageSupervisor *)NULL)
	|| ((PGSPVSR = Get_MatchToSequenceID(PSS_FinishEPCleaning)) != (PageSupervisor *)NULL)
	||  (HowManyActivePageSupervisor() == 0 ) )
	{
//		MSG("(Eng)$$$RequestState : ST_OFF$$$");
		Fuser_Control.RequestState = ST_OFF;
		ClearAllPageSupervisor();
		StopAllPortsInEngine();
		_Set_EM_State(EMS_RecoverError);
	}
}

void	EM_RecoverError(void)
{
	MESSAGE message;
	Fuser_Control.fDuplexJob = CLEAR;
	
	if (HowManyActivePageSupervisor() == 0)
	{
		if(engine_state.fcancelrequest == TRUE)
		{
			message.msgType = MSG_CANCELJOB;
			engine_cancel_page_msg(&message);
			engine_state.fcancelrequest = FALSE;
		}
	}
	
#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		if((f_MSG_USER_ERR_RECOVERY == SET) && (fClearErrorsCmdRcv == CLEAR))
		{
			f_MSG_USER_ERR_RECOVERY = CLEAR;
			fWaitPageCmd = TRUE;
			clear_engine_state();
			fClearErrorsCmdRcv = SET;
			MSG("(eng)f_MSG_USER_ERR_RECOVERY\n");
		}
		
	}
	
	if (fClearErrorsCmdRcv == RECEIVED)	
	{
		Motor_FWD();
		if (Fuser_Control.RequestState == ST_OFF)
		{
#if	ENG_TRACE				
			MSG("(Eng)## RecoverError 1 ##\n");
#endif				
			RecoverError();
			StandbyModeSelect(); /* [KSF051502] */
			Fuser_Control.RequestState = ST_STANDBY;
			Fuser_Control.EM_State = EMS_PowerOnResetState;
		}
		else
		{
#if	ENG_TRACE			
			MSG("(Eng)## RecoverError!!! 2 ##\n");
#endif			
			RecoverError();
#ifdef PRINTREADY_NOTIFY_2				
			if(Set_PrintReadyNotify() == SEND) 
			{
#else							
			if(/*Set_PrintReadyNotify() == SEND*/1) 
			{
#endif	
				Fuser_Control.EM_State = EMS_PrintReady;
			}
		}
		fJamDebugTrace = TRUE;
	}
	EM_EngineRestart() ;		/* To EM_RIP_SYNC */
}

void	EM_FatalError(void)
{
	PageSupervisor	*PGSPVSR ;
#if	ENG_TRACE	
	MSG("(Eng)EM_FatalError\n");
#endif
	if(HowManyActivePageSupervisor() == NONE) 
	{
		StopAllPortsInEngine();
//		if(Set_MachineCheckNotify(CodeOfMachineCheck) == TRUE) 
		if(Set_FatalCheckNotify(CodeOfMachineCheck) == TRUE) 
		{			//24Apr09 yjlee
			fPaperFeedMotorFailure = CLEAR;
			fMirrorMotorFailure = CLEAR;
			fLaserDiodeFailure = CLEAR;
			fNoFirstHsyncSignal = CLEAR;
			fPrintheadLostHsync = CLEAR;
			fVideoDoneFail = CLEAR;
			fSoftwareFailure = CLEAR;
			
			Fuser_Control.fFuserFault = CLEAR;
			
			Fuser_Control.fFatalError = SET;
			Fuser_Control.EM_State = EMS_NonRecoveryError;
		}
	}
	else if((PGSPVSR = Get_MatchToSequenceID(PSS_StopSequence))
	!= (PageSupervisor*)NULL) {
		PGSPVSR->Flags = PSS_StopSequence;
	}
}

void	EM_OptionResync(void)
{
	if(f2ndDrawerState == SET)
	{
		if(cFatalOptionCommFail > 0)
		{
			if(cFatalOptionCommFail >= 3)
			{
				MSG("(eng)Option FATAL ERROR!!(%d)\n", cFatalOptionCommFail);
				fSoftwareFailure = ERROR;
				CodeOfMachineCheck = SOFTWARE_ERROR_OPTION;
				
				fTray2Notify = CLEAR;		
				f2ndDrawerState = CLEAR;
				f3ndDrawerState = CLEAR;
			}
			else
			{
				fDebugOptionCommFailForDisconnection = TRUE;
				cOptionCommFail = CLEAR;
				fDebugOptionCommFail = TRUE;
				Option_Tx_Sync(OPTION_T1);
				Option_Tx_Sync(OPTION_T2);
			}
		}
		else
		{
			// Resync Success
			fWaitPrintingForResync = CLEAR;
			Fuser_Control.EM_State = Pre_EM_State;
			MSG("(eng)ReSync Success!!\n");
		}
	}
}

void 	EM_SCError(void)
{
	ClearAllPageSupervisor();
#if	ENG_TRACE	
	MSG("(Eng)EM_SCError\n");
#endif	

	if (HowManyActivePageSupervisor() == 0)
	{
		if (SC_State == ST_CERT_FAILULE)/* [KSR040901] - start */
		{
			if(fSoftwareFailure == ERROR)
			{
				Set_MachineCheckNotify(CodeOfMachineCheck);
			}
			Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE, 0x00);
		}
		else if (SC_State == ST_TONER_BLOCK)
		{
			Set_MotorStatusNotify(MOTOR_STOP, 0x00, Toner_Used_Percentage, cPageCount5perTrans);
			Set_SmartICInfoNotify(CARTRIDGE_CMD_CLOSE_FAILULE, 0x00);
		}/* [KSR040901] - end */
		
		Fuser_Control.RequestState = ST_OFF; 
		Fuser_Control.EM_State = EMS_RecoverError;
		return ;
	}
}

void	EM_Diagonostic(void)
{
	static unsigned int chardware_test_count = 0;
	Fuser_Control.RequestState = ST_OFF;
	
	if((hardware_test.test_off == ON) && (fHardwareTest.byte == 0))
	{
		chardware_test_count = 0;
		fClearErrorsCmdRcv = TRUE ;
		Fuser_Control.EM_State = EMS_RecoverError;
		hardware_test.test_off = OFF;
	}
	
	if(hardware_test.sensor_check == ON)
	{
		Check_Sensor_Status();
		if(hardware_test.sensor_check == OFF)
		{
			Sensor_Test_Off();
		}
	}
	
	if(hardware_test.solenoid_check == ON)
	{
		if(chardware_test_count == 0)
		{
			All_Solenoid_On();
		}
		chardware_test_count++;
		if(chardware_test_count >= 200)
		{
			All_Solenoid_Off();
			hardware_test.solenoid_check = OFF;	
			chardware_test_count = 0;
			SPResponseSolenoidTest();
		}
	}
	
	if(hardware_test.dcmotor_t1_check == ON)
	{
		fHardwareTest.bit.b2 = 1;
		if(chardware_test_count == 0)
		{
			TurnOnTray1Solenoid(0);
		}
		chardware_test_count++;
		if(chardware_test_count >= 200)
		{
			hardware_test.dcmotor_t1_check = OFF;
			chardware_test_count = 0;
			fHardwareTest.bit.b2 = 0;
			SPResponseMotorTest(1);
		}		
	}
	
	if(hardware_test.dcmotor_t2_check == ON)
	{
		fHardwareTest.bit.b3 = 1;
		if(chardware_test_count == 0)
		{
			if(OptionTray1Pro == OPTION_TRAY_250)			Option_Tx_Pickup(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS_250, 0);
			else 											Option_Tx_Pickup(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS, 0);
		}
		chardware_test_count++;
		if(chardware_test_count >= 200)
		{
			Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
			hardware_test.dcmotor_t2_check = OFF;
			chardware_test_count = 0;
			fHardwareTest.bit.b3 = 0;
			SPResponseMotorTest(2);
		}
	}
	
	if(hardware_test.dcmotor_t3_check == ON)
	{
		fHardwareTest.bit.b4 = 1;
		if(chardware_test_count == 0)
		{
			if(OptionTray2Pro == OPTION_TRAY_250)			Option_Tx_Pickup(OPTION_T2, TRAY2_MOTORRUNSPEED_KARAS_250, 0);
			else 											Option_Tx_Pickup(OPTION_T2, TRAY2_MOTORRUNSPEED_KARAS, 0);
		}
		chardware_test_count++;
		if(chardware_test_count >= 200)
		{
			Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
			hardware_test.dcmotor_t3_check = OFF;
			chardware_test_count = 0;
			fHardwareTest.bit.b4 = 0;
			SPResponseMotorTest(3);
		}
	}
}

void	EM_NonRecoveryError(void)
{
}

void	RecoverError(void)
	/* Clear All Recoverable Error */
{
	fPaperJamOnPins = CLEAR;
 	fPaperJamOnPexs = CLEAR;
	fPaperJamOnDis = CLEAR;
	fPaperJamAtMPF = CLEAR;
	
	PaperJamOnPins = CLEAR;

	fPaperJamBetweenPinsAndPexs = CLEAR;
	PaperJamBetweenPinsAndPexs = CLEAR;


	PaperJamOnPexs = CLEAR;
	PaperJamInDuplex = CLEAR;
	PaperJamBetweenPickupAndPins = CLEAR;
	
	fPaperEmptyForMPF = CLEAR;
	fPaperEmptyForT1 = CLEAR;
	fPaperEmptyForT2 = CLEAR;
	fPaperEmptyForT3 = CLEAR;
	
	EmptyForT1 = CLEAR;
	EmptyForT2 = CLEAR;
	EmptyForT3 = CLEAR;
	
	fDoorCoverError = CLEAR;
	fClearErrorsCmdRcv = CLEAR;
	
	fRetryPickUp = CLEAR;
	fLastPickUp = CLEAR;

	fEngineStateFull = CLEAR;

	fMotorRunWithoutPrintCmd = CLEAR;
	fMotorRun = CLEAR;

	bePrintFromPowerSave = CLEAR;
	PrintPageCmd = FALSE;
	fMotorRunFromPowerSave = CLEAR;

	cFullStackErrCnt = 0;
	cFullStackDetacted = 0;

	fDuplexCooling = CLEAR;
	fCoolingState = CLEAR;
	fHSync_Detacted = CLEAR;
	cKeepMotorRunForCooling = 0;
	fVideo_Print_Enable = SET;
	cTray2PaperCheckBeforePickUp = 0;
	PageNumVideoDoneFail = CLEAR;
#if DUPLEX_SLOW_MODE2
	fEnvTempStatusChage = EnvTemp_Clear;
	fDuplexPrintSlowlyForServoHH = CLEAR;
#endif
#if BLI_TEST
	Fuser_Control.LowSpeedFan_Wakeup = CLEAR;
#endif
#if !(FULLSTACK_CHECK)
	if(fState_FullStack == TRUE)
	{
		fState_FullStack = CLEAR;
		fFullStack_Error = CLEAR;
		full_stack_notify(FullStackClear);
		cFullStackReleased = 0;
	}
#endif
}

void	EM(void)
{
	
}

Uint8	StartPrintSequence(Uint8 id)
	/* Confirm to receive the Print_Page command and 
		Set a parameter to Page_Supervisor variable,
		Start the print sequence of this page
		Start sequence number receives on argument */
{
	if(((fPendingPrintPageCmd == TRUE) || (fPreRunBeforePrintCmd == RECEIVED)) && (fJobIDChanged == CLEAR) && (fWaitToChangeMotorSpeed == CLEAR) && (fWaitPrintingForResync == CLEAR))
	{
		if(Set_PrintPageParameter(id) == TRUE) 
		{
			MSG("(Eng)$$$ StartPrintSequence - RequestState : ST_PRINT$$$\n");
// #if MAC_TYPE_MH	
			// SubFan_On();
// #endif
			Fuser_Control.RequestState = ST_PRINT;
			return TRUE;
		}
	}
	
	if(fMotorRunWithoutPrintCmd == SET) 
	{
		if(fMotorRun == SET) 
		{
			if(Set_PrintPageParameter(id) == TRUE) 
			{
//~ #if MAC_TYPE_MH
				StandbyModeSelect(); /* [KSF051502] */
//~ #endif
				Fuser_Control.RequestState = ST_STANDBY;
				return TRUE;
			}
		}
	}
	return FALSE;
}

Uint8	PowerSaveControl(void)
{
	if((Fuser_Control.State != ST_STANDBY) && (Fuser_Control.State != ST_COLD_WAKEUP))
		return FALSE;

	if((fRequested_Power_Saver == ACTIVE) && (EarlyStartCmd != ACTIVE))
	{
		fRequested_Power_Saver = NOT_ACTIVE;
		EarlyStartCmd = CLEAR;
//		Sensor_Power_Off();
		Fuser_Control.fPowerSaveMode = SET;
		Fuser_Control.RequestState = ST_POWER_SAVE;
		return TRUE;
	}
	else if(((fRequested_Power_Saver == ACTIVE) && (EarlyStartCmd == ACTIVE)) || ((fRequested_Power_Saver != ACTIVE) && (EarlyStartCmd == ACTIVE)))
	{
		fRequested_Power_Saver = NOT_ACTIVE;
		EarlyStartCmd = CLEAR;
	}
	return FALSE;
}

Uint8	Check_MachineStateBeforeStart(void)
{
	if(HowManyActivePageSupervisor() > 0) 
	{
		return TRUE;
	}
	
	if(((IsPaperPresentOnPins() == PRESENT) || (IsPaperPresentOnPexs() == PRESENT)
	|| ((fTray2FeedSensorDetectState == TRUE) && (f2ndDrawerState == TRUE)) || ((fTray3FeedSensorDetectState == TRUE) && (f3ndDrawerState == TRUE) && (OptionPrePickState == CLEAR)))
	&& (fCheckMachineStateBeforeStart == FALSE))
	{
		// MSG("(Eng)### JAM Detacted : Paper input = %d, Paper Exit = %d (%d)###\n",IsPaperPresentOnPins(),IsPaperPresentOnPexs(),cProcessCounter);
		fCheckMachineStateBeforeStart = TRUE;		// ���� �������� ��츦 Ŀ�� �ϱ� ���Ͽ� 10msec ���� �ѹ� �� ���� ���� Ȯ���ϰ� Jam �Ǵ�.
		return FALSE;
	}
	
	fCheckMachineStateBeforeStart = FALSE;
	
	if(IsPaperPresentOnPins() == PRESENT) 
	{
		if(IsPaperPresentOnPexs() == PRESENT) 
		{
			if(fJamDebugTrace == TRUE)
			{
#if	ENG_TRACE				
				MSG("(Eng)##JAM : Paper input & Output Sensor##\n");
#endif				
				fJamDebugTrace = FALSE;
			}
			PaperJamBetweenPinsAndPexs = JAM;
		}
		else 
		{
			if(fJamDebugTrace == TRUE)
			{
#if	ENG_TRACE				
				MSG("(Eng)##JAM : Paper input Sensor(%d)##\n",cProcessCounter);
#endif				
				fJamDebugTrace = FALSE;
			}
			PaperJamOnPins	= JAM;
		}
		CodeOfJamStatus = JAM_IDLE_STATE;
		return	FALSE;
	}
	else if(IsPaperPresentOnPexs() == PRESENT) 
	{
		if(fJamDebugTrace == TRUE)
		{
#if	ENG_TRACE			
			MSG("(Eng)##JAM : Paper Output Sensor##\n");
#endif			
			fJamDebugTrace = FALSE;
		}
		PaperJamOnPexs = JAM;
		CodeOfJamStatus = JAM_IDLE_STATE;
		return FALSE;
	}
	else if((fTray2FeedSensorDetectState == TRUE) && (f2ndDrawerState == TRUE))
	{
		if(fJamDebugTrace == TRUE)
		{
#if	ENG_TRACE			
			MSG("(Eng)##JAM : Paper Tray2 Feed Sensor##\n");
#endif			
			fJamDebugTrace = FALSE;
		}
		PaperJamBetweenPickupAndPins = JAM;
		CodeOfJamStatus = JAM_LE_INPUT_DETACT_OPTION;
		return FALSE;
	}
	else if((fTray3FeedSensorDetectState == TRUE) && (f3ndDrawerState == TRUE) && (OptionPrePickState == CLEAR))
	{
		if(fJamDebugTrace == TRUE)
		{
#if	ENG_TRACE			
			MSG("(Eng)##JAM : Paper Tray3 Feed Sensor##\n");
#endif			
			fJamDebugTrace = FALSE;
		}
		PaperJamBetweenPickupAndPins = JAM;
		CodeOfJamStatus = JAM_LE_INPUT_DETACT_OPTION2;
		return FALSE;
	}
	fJamDebugTrace = TRUE;

	return TRUE;
}

Uint8	CheckDoorCover(void)
{
	PageSupervisor *now_PGSPVSR;
	//MESSAGE message;
	
	if( ( IsDoorCoverState() == OPEN ) ) 
	{
		if(fCoverOpenDebugMsg == TRUE)
		{
#if	ENG_TRACE				
			MSG("(Eng)COVER is Opened!\n");
#endif
			fCoverOpenDebugMsg = FALSE;
			fNewTonerCheck = FALSE;
			fStore_TonerBlockState = CLEAR;
			cJobEndDotCountWriteFailCnt = 0;
#if SMART_IC_LOCK_ENABLE			
			if(ftoner_count_write_for_coveropen == SET)
			{
				MSG("(Eng)toner_count_write_for_coveropen\n");
				fSmartICWrite_PageCountFailInRun = FALSE;
				fSmartICWrite_DotCountFailInRun = FALSE;
				fSmartICWrite_DotCount = TRUE;
				fMemoryWrite = TRUE;
				fSmartICWrite_PageCount = TRUE;
				
				ftoner_count_write_for_coveropen = CLEAR;
			}
#endif
		}
			
		if(fDoorCoverError == CLEAR) 
		{
			cJamRecoverTimer = 0;
			fFrontCoverState = OPEN;
			fDoorCoverError = ERROR;
			
			clear_engine_state();
			
			//message.msgType = MSG_STATUSUPDATE;
			//engine_state.front_door_open = true;
			//slogit( message.param1 = STATUS_DOOROPEN_FRONT );
			//SYMsgSend(PRINTMGRID, &message);
			
			now_PGSPVSR = Get_LastPageSupervisor();
#if	ENG_TRACE			
			MSG("(Eng)## HowManyActivePageSupervisor = %d, Source = %d, PageID = %d, fPreRunBeforePrintCmd = %d ##\n",HowManyActivePageSupervisor(), now_PGSPVSR->Source, now_PGSPVSR->PageID, fPreRunBeforePrintCmd);
			MSG("(Eng)## fPendingPrintPageCmd = %d, PendingPrintPageData[1] = %d, PendingPrintPageData[0] = %d ##\n",fPendingPrintPageCmd, PendingPrintPageData[1], PendingPrintPageData[0]);
#endif
			if((((HowManyActivePageSupervisor() != 0) && (now_PGSPVSR->Source != 0) && (now_PGSPVSR->PageID == 1)) 
			|| ((fPendingPrintPageCmd == SET) && (PendingPrintPageData[1] != 0) && (PendingPrintPageData[0] == 1))) && (fPreRunBeforePrintCmd == CLEAR))
			{
				if(((HowManyActivePageSupervisor() != 0) && (now_PGSPVSR->Source == MS_Duplex))
				|| ((fPendingPrintPageCmd == SET) && (PendingPrintPageData[1] == MS_Duplex)))
				{
					PaperJamInDuplex = JAM;
					CodeOfJamStatus = JAM_DUPLEX_PATH;
				}
				else 
				{
					PaperJamBetweenPickupAndPins = JAM;
					
					if(((HowManyActivePageSupervisor() != 0) && (now_PGSPVSR->Source == MS_MultiPurposeTray))
					|| ((fPendingPrintPageCmd == SET) && (PendingPrintPageData[1] == MS_MultiPurposeTray)))
					{
						CodeOfJamStatus = JAM_LE_INPUT_DETACT_MPT;
					}
					else if(((HowManyActivePageSupervisor() != 0) && (now_PGSPVSR->Source == MS_Tray2))
					|| ((fPendingPrintPageCmd == SET) && (PendingPrintPageData[1] == MS_Tray2)))
					{
						CodeOfJamStatus = JAM_LE_INPUT_DETACT_OPTION;
					}
					else if(((HowManyActivePageSupervisor() != 0) && (now_PGSPVSR->Source == MS_Tray3))
					|| ((fPendingPrintPageCmd == SET) && (PendingPrintPageData[1] == MS_Tray3)))
					{
						CodeOfJamStatus = JAM_LE_INPUT_DETACT_OPTION2;
					}
					else
					{
						CodeOfJamStatus = JAM_LE_INPUT_DETACT_TRAY1;
					}
				}
				Set_PaperJamNotify(TempPaperJam.DR.BYTE, CodeOfJamStatus);
			}
			fWaitPrintPageCmd = CLEAR;
			fPreRunBeforePrintCmd = CLEAR;
			StopAllPortsInEngine();
			Fuser_Control.EM_State = EMS_OpenDoorCover;
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(fDoorCoverError == ERROR)
		return FALSE;
	return TRUE;
}

Uint8	CheckDoorCover_Laminating(void)
{
	MESSAGE message;
	
	if( ( IsDoorCoverState() == OPEN ) ) 
	{
		if(fDoorCoverError == CLEAR)
		{
			fDoorCoverError = ERROR;
			MSG("(Eng)COVER is Opened!(Laminate)\n");
	
			fNewTonerCheck = FALSE;
			fStore_TonerBlockState = CLEAR;
			
			//LaminatingSendMsgToPM(STATUS_LAMINATEINFO_FRONTCOVER_OPEN);
			message.msgType = MSG_STATUSUPDATE;
			message.param1 = STATUS_DOOROPEN_FRONT;
			SYMsgSend(PRINTMGRID, &message);
			
			StopAllPortsInEngine();
			Fuser_Control.EM_State = EMS_Laminating_CloseCounter;
		}
	}
	
	return TRUE;
}

void	CheckTray1(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t papersize, val1, val2, val3, high, low;
	MESSAGE message;
	Uint8 papervol;
	
#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		if(f1stTimeCheckTray1 == SET)
		{
			f1stTimeCheckTray1 = CLEAR;
			SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_UNKNOWN);
			SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
			message.param1 = STATUS_TRAY1_MEDIA_UNKNOWN;
			MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_Unknown);
					
			message.msgType = MSG_STATUSUPDATE;
			message.param2 = STATUS_TRAY;
			SYMsgSend(PRINTMGRID, &message);
			MSG("(eng)Send Tray STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray1.oldTrayStatus,Tray_Status.Tray1.newTrayStatus);
			message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
		}
		
		Tray_Status.Tray1.oldTrayStatus = Tray_Status.Tray1.newTrayStatus;
		
		papervol = IsPaperPresentOnTray1();
		if(papervol != pre_papervol)
		{
			if(fignoresensorchange == CLEAR)
			{
				ctray1highlowsensorchattering = 0;
				ftray1highlowsensorchattering = SET;
			}
		}
		pre_papervol = papervol;
		
		if(ftray1highlowsensorchattering == SET)
		{
			if(ctray1highlowsensorchattering < TRAY1HIGHLOWCHATTERING)
			{
				MSG("(eng)Tray1 Chattering = %d\n", ctray1highlowsensorchattering);
				ctray1highlowsensorchattering++;
				fignoresensorchange = SET;
				return;
			}
			else
			{
				ctray1highlowsensorchattering = 0;
				ftray1highlowsensorchattering = CLEAR;
				fignoresensorchange = CLEAR;
			}
		}
		
		if(papervol == NOT_PRESENT)
		{
			SetTrayMediaEmpty(Tray_Status.Tray1.newTrayStatus);
		}
		else
		{
			SetTrayMediaNormal(Tray_Status.Tray1.newTrayStatus);
		}
		
		if(GetTrayMediaLevel(Tray_Status.Tray1.oldTrayStatus) != GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus))
		{
			printer_setup_t printer_setup;
			printvar_get_printersetup( &printer_setup );
			switch(GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus))
			{
				case TRAY_MEDIA_EMPTY:
					message.param1 = STATUS_PAPER_OUT_TRAY1;
					if ( printer_setup.inputTrays[1].paperPresent ) {
					    printer_setup.inputTrays[1].paperPresent = false;
					    printvar_set_printersetup( &printer_setup );
					}
					fTray1Empty = SET;
					break;
				case TRAY_MEDIA_NORMAL:
					message.param1 = STATUS_TRAY1_PAPER_NORMAL;
					if ( !printer_setup.inputTrays[1].paperPresent ) {
					    printer_setup.inputTrays[1].paperPresent = true;
					    printvar_set_printersetup( &printer_setup );
					}
					fTray1Empty = CLEAR;
					break;	
				default:
					break;
			}
			message.msgType = MSG_STATUSUPDATE;
			message.param2 = STATUS_TRAY;
			SYMsgSend(PRINTMGRID, &message);
			MSG("(eng)Send Tray Media Level STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray1.oldTrayStatus,Tray_Status.Tray1.newTrayStatus);
			message.param2 = 0; //Reset param2 value. Because more messages are using same message structure.
		}
	}
	else
	{

		gpio_read(eng_hw->paper_low, &low);
		gpio_read(eng_hw->paper_high, &high);
		papervol = (high << 1) + low;
		
		if(fQuickly_sensor_check_T1 == CLEAR)
		{
			if(pre_papervol != papervol)
			{
				ctray1highlowsensorchattering = CLEAR;
				ftray1highlowsensorchattering = TRUE;
				if(fignoresensorchange == CLEAR)
				{
					temp_papervol = pre_papervol;
					MSG("(eng)tray1 highlow sensor : %d, %d\n", temp_papervol, papervol);
				}
				else
				{
					MSG("(eng)tray1 highlow sensor ignore\n");
				}
			}
			
			pre_papervol = papervol;
			
			if(ftray1highlowsensorchattering == TRUE)
			{
				ctray1highlowsensorchattering++;
				if(ctray1highlowsensorchattering >= TRAY1HIGHLOWCHATTERING)
				{
					fignoresensorchange = CLEAR;
					ftray1highlowsensorchattering = FALSE;
					ctray1highlowsensorchattering = CLEAR;
				}
				else
				{
					fignoresensorchange = SET;
					papervol = temp_papervol;
				}
				MSG("(eng)tray1 highlow sensor result = %d\n", papervol);
			}
		}
		else
		{
			pre_papervol = papervol;
		}	
	
		gpio_read(eng_hw->papersize_sw1, &val1);
		gpio_read(eng_hw->papersize_sw2, &val2);
		gpio_read(eng_hw->papersize_sw3, &val3);
		papersize = (val1 << 2) + (val2 << 1) + val3;
		
		if(fQuickly_sensor_check_T1 == SET)
		{
			MSG("(eng)Quickly_sensor_check_T1\n");
			Tray1Online = TRUE;
			fQuickly_sensor_check_T1 = CLEAR;
			fFirstTray1LevelDetect = TRUE;
			pre_papersize = papersize;
		}
		else
		{
			if(pre_papersize != papersize)
			{
				ctray1sizesensorchattering = CLEAR;
				ftray1sizesensorchattering = TRUE;
				if(fignoresizesensorchange == CLEAR)
				{
					temp_papersize = pre_papersize;
					MSG("(eng)tray1 size sensor : %d, %d\n", temp_papersize, papersize);
				}
				else
				{
					MSG("(eng)tray1 size sensor ignore\n");
				}
			}
			
			pre_papersize = papersize;
			
			if(ftray1sizesensorchattering == TRUE)
			{
				ctray1sizesensorchattering++;
				if(ctray1sizesensorchattering >= TRAY1SIZECHATTERING)
				{
					fignoresizesensorchange = CLEAR;
					ftray1sizesensorchattering = FALSE;
					ctray1sizesensorchattering = CLEAR;
				}
				else
				{
					fignoresizesensorchange = SET;
					papersize = temp_papersize;
				}
				MSG("(eng)tray1 size sensor result = %d\n", papersize);
			}
			
			if(Tray1OnlineChattering == TRUE)
			{
				if(papersize == 7)
				{
					tray1detect_count++;
					Tray1Online = CLEAR;
					if(tray1detect_count == TRAY1CHATTERING)
					{
						Tray1OnlineChattering = FALSE;
						tray1detect_count = CLEAR;
						Tray1Online = TRUE;
						fFirstTray1LevelDetect = TRUE;
					}
				}
				else
				{
					tray1detect_count = CLEAR;
					Tray1Online = TRUE;
				}
			}
			else
			{
				if(papersize != 7)
				{
					tray1detect_count++;
					Tray1Online = CLEAR;
					if(tray1detect_count == TRAY1CHATTERING)
					{
						Tray1OnlineChattering = TRUE;
						tray1detect_count = CLEAR;
						Tray1Online = TRUE;
						fFirstTray1LevelDetect = TRUE;
					}
				}
				else
				{
					tray1detect_count = CLEAR;
					Tray1Online = TRUE;
				}
			}
		}
		
		if(Tray1Online == TRUE)
		{
			Tray_Status.Tray1.oldTrayStatus = Tray_Status.Tray1.newTrayStatus;
					
			switch(papersize)
			{
				case 0:
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_UNKNOWN);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_UNKNOWN;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_Unknown);
					break;
				case 1:
					//Letter
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_LT);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_LT);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_LETTER;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_Letter);
					break;
				
				case 2:
					//EXEC
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_EXEC);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_EXEC);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_EXEC;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_Exec);
					break;
					
				case 3:
					//A4
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_A4);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_A4);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_A4;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_A4);
					break;
				
				case 4:
					// B5
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_B5);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_B5);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_B5;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_B5);
					break;
				
				case 5:
					//Legal
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_LG);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_LG);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_LEGAL;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_Legal);
					break;
					
				case 6:
					//A5
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_A5);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_A5);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_A5;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_A5);
					break;
					
				case 7:
					fTrayOpen = TRUE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_UNKNOWN);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
					SetTrayMediaEmpty(Tray_Status.Tray1.newTrayStatus);
					SetTrayAttachOff(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_OPEN;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_Unknown);
					break;
				default:
					MSG("(eng)Unknown tray1 paper size : %d \n", papersize);
					fTrayOpen = FALSE;
					SetTrayMediaSize(Tray_Status.Tray1.newTrayStatus, TRAY_MEDIA_UNKNOWN);
					SetTrayMediaSize(Tray_Status.Tray1.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
					SetTrayAttachOn(Tray_Status.Tray1.newTrayStatus);
					message.param1 = STATUS_TRAY1_MEDIA_UNKNOWN;
					MeasuredPaperLengthInTray1 = Set_MeasuredMediaSize(MS_Tray1, MS_Unknown);
					break;
			}
			
			if(Tray_Status.Tray1.oldTrayStatus != Tray_Status.Tray1.newTrayStatus)
			{
				set_encoders_of_paper(eng_hw->paper_motor, papersize); 
				
				message.msgType = MSG_STATUSUPDATE;
				message.param2 = STATUS_TRAY;
				SYMsgSend(PRINTMGRID, &message);
				MSG("(eng)Send Tray STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray1.oldTrayStatus,Tray_Status.Tray1.newTrayStatus);
				message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
			}
			
			//TODO : ITEX�ӽ� ��å��. ���� �Ⱦ��� empty �����ϴ��� �����Ͽ� (auto link����) ���� �ʿ� 
			if(GetTrayAttachInfo(Tray_Status.Tray1.newTrayStatus) == TRUE)
			{
	//			IsPaperPresentOnTray1();
				switch(papervol)
				{
					case 1: // empty
						SetTrayMediaEmpty(Tray_Status.Tray1.newTrayStatus);
						break;
					case 2: // full
						SetTrayMediaNormal(Tray_Status.Tray1.newTrayStatus);
						break;
					case 3: // low
						SetTrayMediaLow(Tray_Status.Tray1.newTrayStatus);
						break;
				}
				
				if(fFirstTray1LevelDetect == FALSE)
				{
					if(GetTrayMediaLevel(Tray_Status.Tray1.oldTrayStatus) < GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus))
					{
						SetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus, Tray_Status.Tray1.oldTrayStatus);
					}
				}
				fFirstTray1LevelDetect = FALSE;
	
			}
			
			if(GetTrayMediaLevel(Tray_Status.Tray1.oldTrayStatus) != GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus))
			{
				printer_setup_t printer_setup;
				printvar_get_printersetup( &printer_setup );
				switch(GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus))
				{
					case TRAY_MEDIA_EMPTY:
						message.param1 = STATUS_PAPER_OUT_TRAY1;
						if ( printer_setup.inputTrays[1].paperPresent ) {
						    printer_setup.inputTrays[1].paperPresent = false;
						    printvar_set_printersetup( &printer_setup );
						}
						fTray1Empty = SET;
						break;
					case TRAY_MEDIA_LOW:
						message.param1 = STATUS_TRAY1_PAPER_LOW;
						if ( !printer_setup.inputTrays[1].paperPresent ) {
						    printer_setup.inputTrays[1].paperPresent = true;
						    printvar_set_printersetup( &printer_setup );
						}
						fTray1Empty = CLEAR;
						break;
					case TRAY_MEDIA_NORMAL:
						message.param1 = STATUS_TRAY1_PAPER_NORMAL;
						if ( !printer_setup.inputTrays[1].paperPresent ) {
						    printer_setup.inputTrays[1].paperPresent = true;
						    printvar_set_printersetup( &printer_setup );
						}
						fTray1Empty = CLEAR;
						break;	
					default:
						break;
				}
				message.msgType = MSG_STATUSUPDATE;
				message.param2 = STATUS_TRAY;
				SYMsgSend(PRINTMGRID, &message);
				MSG("(eng)Send Tray Media Level STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray1.oldTrayStatus,Tray_Status.Tray1.newTrayStatus);
				message.param2 = 0; //Reset param2 value. Because more messages are using same message structure.
			}
	
			if(((GetTrayAttachInfo(Tray_Status.Tray1.oldTrayStatus) == CLEAR) && (GetTrayAttachInfo(Tray_Status.Tray1.newTrayStatus) == SET))
			&& (engine_state.front_door_open == FALSE) && (engine_state.pm_statusrequest != STATUS_ONLINE)
			&& (fPOR_PageSupervisor_Act == CLEAR)
			&& (Get_MainMotorState() == STOP)
			&& (fPaperIsEmptyBeforeMotorRun == CLEAR)
			&& (Fuser_Control.EM_State == EMS_RecoverError)
			&& TrayCloseRecoveryEnable(1))
			{
				Sensor_Power_On();
				Motor_FWD();
				fClearErrorsCmdRcv = CLEAR;
				fWaitPageCmd = TRUE;
				clear_engine_state();
				MSG("(eng) ## Send Tray online STATUS_UPDATE <%x> ##\n", message.param1);
				fClearErrorsCmdRcv = SET;
				Fuser_Control.EM_State = EMS_RecoverError;
			}
		}
	}
}

void	CheckTray2(void)
{
	if(f3ndDrawerState == CLEAR)
	{
		if(Get_Elapse_UART() >= 100)
		{
			fTray2Notify = TRUE;
			Option_Tx_Notify(OPTION_T1);
			//MSG("(eng)Option_Tx_Notify(2) <%d>\n", cProcessCounter);
			Set_Timer_UART();
		}
	}
	else
	{
		if(Get_Elapse_UART() >= 50)
		{
			fTray2Notify = TRUE;
			if(fTray2IsNotifying == CLEAR)
			{
				//MSG("(eng)Option_Tx_Notify(2) <%d>\n", cProcessCounter);
				Option_Tx_Notify(OPTION_T1);
			}
			else
			{
				//MSG("(eng)Option_Tx_Notify(3) <%d>\n", cProcessCounter);
				Option_Tx_Notify(OPTION_T2);
			}
			Set_Timer_UART();
		}
	}

	if(((f2ndDrawerState == TRUE) && (SendDataToOption1 >= 5))
	|| ((f3ndDrawerState == TRUE) && (SendDataToOption2 >= 5)))
	{
		if(fDebugOptionCommFailForDisconnection == TRUE)
		{
			cFatalOptionCommFail++;
			MSG("(eng)Option Disconnected T2(%d):%d, T3(%d):%d <%d>\n", f2ndDrawerState, SendDataToOption1, f3ndDrawerState, SendDataToOption2, cFatalOptionCommFail);
			if(SendDataToOption1 >= 2) fOption1Disconnection = TRUE;
			if(SendDataToOption2 >= 2) fOption2Disconnection = TRUE;
			SendDataToOption1 = CLEAR;
			SendDataToOption2 = CLEAR;
		}
		fDebugOptionCommFailForDisconnection = FALSE;
	}
	if((Fuser_Control.EM_State == EMS_Standby) && (cFatalOptionCommFail == 1))
	{
		SendDataToOption1 = CLEAR;
		SendDataToOption2 = CLEAR;
		Pre_EM_State = Fuser_Control.EM_State;
		Fuser_Control.EM_State = EMS_OptionResync;
	}
}

void	CheckMPF(void)
{
	MESSAGE message;
    static bool present = false;
	printer_setup_t printer_setup;

	Tray_Status.Tray4.oldTrayStatus = Tray_Status.Tray4.newTrayStatus;
	
	printvar_get_printersetup( &printer_setup );
	if(IsPaperPresentOnMPF() == PRESENT) 
	{
		EmptyForMPF = NO_EMPTY;
		SetTrayMediaNormal(Tray_Status.Tray4.newTrayStatus);
		if (!present) {
			present = true;
			printvar_get_printersetup( &printer_setup );
			printer_setup.inputTrays[0].paperPresent = true;
			printvar_set_printersetup( &printer_setup );
			dbg_printf("MFP present \n");
		}
	}
	else
	{
		/* Need to make notify func for Abba */
		EmptyForMPF = EMPTY;
		SetTrayMediaEmpty(Tray_Status.Tray4.newTrayStatus);
		if (present) {
			present = false;
			printvar_get_printersetup( &printer_setup );
			printer_setup.inputTrays[0].paperPresent = false;
			printvar_set_printersetup( &printer_setup );
			dbg_printf("MFP empty \n");
			SetTrayMediaEmpty(Tray_Status.Tray4.newTrayStatus);
		}
	}	
	if(GetTrayMediaLevel(Tray_Status.Tray4.oldTrayStatus) != GetTrayMediaLevel(Tray_Status.Tray4.newTrayStatus))
	{
		printvar_get_printersetup( &printer_setup );
		switch(GetTrayMediaLevel(Tray_Status.Tray4.newTrayStatus))
		{
			case TRAY_MEDIA_EMPTY:
				message.param1 = STATUS_PAPER_OUT_TRAY4;
				if ( printer_setup.inputTrays[0].paperPresent ) {
				    printer_setup.inputTrays[0].paperPresent = false;
				    printvar_set_printersetup( &printer_setup );
				}
				break;
			case TRAY_MEDIA_LOW:
				message.param1 = STATUS_TRAY4_PAPER_LOW;
				if ( !printer_setup.inputTrays[0].paperPresent ) {
				    printer_setup.inputTrays[0].paperPresent = true;
				    printvar_set_printersetup( &printer_setup );
				}
				break;
			case TRAY_MEDIA_NORMAL:
				message.param1 = STATUS_TRAY4_PAPER_NORMAL;
				if ( !printer_setup.inputTrays[0].paperPresent ) {
				    printer_setup.inputTrays[0].paperPresent = true;
				    printvar_set_printersetup( &printer_setup );
				}
				break;	
			default:
				break;
		}
		
		message.msgType = MSG_STATUSUPDATE;
		message.param2 = STATUS_TRAY;
		SYMsgSend(PRINTMGRID, &message);
		MSG("(eng)Send MPT Media Level STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray4.oldTrayStatus,Tray_Status.Tray4.newTrayStatus);
		message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
	}

	if((GetTrayMediaLevel(Tray_Status.Tray4.oldTrayStatus) == TRAY_MEDIA_EMPTY)
	&& (GetTrayMediaLevel(Tray_Status.Tray4.newTrayStatus) == TRAY_MEDIA_NORMAL))
	{
		MPTOnlineChattering = TRUE;
	}
	
	if(MPTOnlineChattering == TRUE)
	{
		if(GetTrayMediaLevel(Tray_Status.Tray4.newTrayStatus) == TRAY_MEDIA_NORMAL)
		{
			mptdetect_count++;
			if(mptdetect_count == MPTCHATTERING)
			{
				MPTOnlineChattering = FALSE;
				mptdetect_count = CLEAR;

				if(((engine_state.front_door_open == FALSE) && (engine_state.pm_statusrequest != STATUS_ONLINE))
				&& (fPOR_PageSupervisor_Act == CLEAR) && (Get_MainMotorState() == STOP)
				&& (fPaperIsEmptyBeforeMotorRun == CLEAR)
				&& (Fuser_Control.EM_State == EMS_RecoverError)
				&& (fClearErrorsCmdRcv == CLEAR)
				&& TrayCloseRecoveryEnable(4))
				{
					Sensor_Power_On();
					Motor_FWD();
					fClearErrorsCmdRcv = CLEAR;
					fWaitPageCmd = TRUE;
					clear_engine_state();
					MSG("(eng) ## Send MPT online STATUS_UPDATE <%x> ##\n", message.param1);
					fClearErrorsCmdRcv = SET;
					Fuser_Control.EM_State = EMS_RecoverError;
				}
				
			}
		}
		else
		{
			MPTOnlineChattering = FALSE;
			mptdetect_count = CLEAR;
		}
	}
}

void	StopAllPortsInEngine(void)
{
	ClearAllPageSupervisor();
	Fuser_Control.RequestState = ST_OFF;
	
	/* All output port is off */
	DisableAllOutputPorts();
	StopMainMotor();
	
	if(f2ndDrawerState == SET) 
	{
//		fTray2MotorStop = TRUE;
		Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
	}
	if(f3ndDrawerState == SET) 
	{
//		fTray3MotorStop = TRUE;
		Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
	}
}

Uint8	Set_FuserTemp(Uint32 Source, Uint32 OutputDestination, Uint32 PaperData, Uint32 PageID, Uint8 PowersaveToPrint)
{
	MEDIA_DISCRPT FuserPaperData;
	Uint8 cmd, fNoAdjustTargetTemp;
	
	FuserPaperData.BYTE = PaperData;
	
	// if(((Source == MS_Duplex) || (OutputDestination == OutputDuplexor)) &&
		// (FuserPaperData.BIT.Type == MTYPE_Paper)) {
	if((Source == MS_Duplex) && ((FuserPaperData.BIT.Type == MTYPE_Paper) || (FuserPaperData.BIT.Type == MTYPE_Recycled))) 
	{
#if DUPLEX_SLOW_MODE2
		if((fPaperLengthIsNarrow == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)) || ((fDuplex_Slow_mode == SET)))
#elif DUPLEX_SLOW_MODE
		if((fPaperLengthIsNarrow == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)) || (fDuplexPrintSlowlyFor15min != CLEAR))
#else
		if((fPaperLengthIsNarrow == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)))
#endif
		{
			Fuser_Control.Target_Temperature = FuserTemperatuerDuplextblForQuiet[FuserPaperData.BYTE & 0x0f];
			MSG("(eng)Target_Temp : Duplex_Quiet\n");
			fNoAdjustTargetTemp = SET;
		}
		else
		{
			Fuser_Control.Target_Temperature = FuserTemperatuerDuplextbl[FuserPaperData.BYTE & 0x0f];
			MSG("(eng)Target_Temp : Duplex_Normal\n");
			fNoAdjustTargetTemp = CLEAR;
		}	
#if NEW_FUSER_TEST_PARAM
		Fuser_Control.Target_Temperature = Fuser_Setting_Parameters[38];
#endif	
	}
	else {
		if(FuserPaperData.BIT.Type < MTYPE_RESERVED_MIN) {
#if DUPLEX_SLOW_MODE2
			if((fPaperLengthIsNarrow == TRUE) || (fPOR_PageSupervisor_Act == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)) || ((fDuplex_Slow_mode == SET)))
#elif DUPLEX_SLOW_MODE
			if((fPaperLengthIsNarrow == TRUE) || (fPOR_PageSupervisor_Act == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)) || (fDuplexPrintSlowlyFor15min != CLEAR))
#else
			if((fPaperLengthIsNarrow == TRUE) || (fPOR_PageSupervisor_Act == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)))
#endif
			{
				Fuser_Control.Target_Temperature = FuserTemperaturetblForQuiet[FuserPaperData.BYTE];
				MSG("(eng)Target_Temp 1 : Simplex_Quiet\n");
				fNoAdjustTargetTemp = SET;
			}
			else
			{
				Fuser_Control.Target_Temperature = FuserTemperaturetbl[FuserPaperData.BYTE];
				MSG("(eng)Target_Temp 1 : Simplex_Normal\n");
				fNoAdjustTargetTemp = CLEAR;
			}
		}
		else {
#if DUPLEX_SLOW_MODE2
			if((fPaperLengthIsNarrow == TRUE) || (fPOR_PageSupervisor_Act == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)) || ((fDuplex_Slow_mode == SET)))
#elif DUPLEX_SLOW_MODE
			if((fPaperLengthIsNarrow == TRUE) || (fPOR_PageSupervisor_Act == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)) || (fDuplexPrintSlowlyFor15min != CLEAR))
#else
			if((fPaperLengthIsNarrow == TRUE) || (fPOR_PageSupervisor_Act == TRUE) || ((fPwsToPrint == SET) && (PowersaveToPrint == 1)))
#endif
			{
				Fuser_Control.Target_Temperature = FuserTemperaturetblForQuiet[NORMAL_PAPER];
				MSG("(eng)Target_Temp 2 : Simplex_Quiet\n");
				fNoAdjustTargetTemp = SET;
			}
			else
			{
				Fuser_Control.Target_Temperature = FuserTemperaturetbl[NORMAL_PAPER];
				MSG("(eng)Target_Temp 2 : Simplex_Normal\n");
				fNoAdjustTargetTemp = CLEAR;
			}
		}
#if NEW_FUSER_TEST_PARAM
		Fuser_Control.Target_Temperature = Fuser_Setting_Parameters[0];
#endif		
	}

#if	ENG_TRACE		
	MSG("(Eng)== Target_Temperature = %d ==\n", Fuser_Control.Target_Temperature);
#endif

//~ #if MAC_TYPE_MH
	if((PageID <= 10) /*&& (fFirstTenPages == SET)*/) // 0 ~ 10 pages
	{
		if(Source == MS_Duplex) {
			cmd = Within10PagesDX;
		}
		else {
			cmd = Within10PagesSX;
		}
	}
	else // 10 ~ pages
	{
		if(Source == MS_Duplex) {
			cmd = Over10PagesDX;
		}
		else {
			cmd = Over10PagesSX;
		}
	}
	
	if((FuserPaperData.BIT.Weight == MW_Normal) && (FuserPaperData.BIT.Texture == MTX_Normal) && (fNoAdjustTargetTemp == CLEAR))
	{
		switch(EnvTempStatus)
		{
			case EnvTemp_H:
				if((FuserPaperData.BIT.Type == MTYPE_Paper) || (FuserPaperData.BIT.Type == MTYPE_Recycled))
				{
					switch(cmd)
					{
#if ABBA2
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 0;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 0;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature -= 10;
							break;
#else
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 15;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
#endif
					}
				}
				break;
			case EnvTemp_HM:
				if((FuserPaperData.BIT.Type == MTYPE_Paper) || (FuserPaperData.BIT.Type == MTYPE_Recycled))
				{
					switch(cmd)
					{
#if ABBA2
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 5;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 0;
							break;
#else
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 15;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
#endif
					}
				}
				break;
			case EnvTemp_M:
				if((FuserPaperData.BIT.Type == MTYPE_Paper) || (FuserPaperData.BIT.Type == MTYPE_Recycled))
				{
					switch(cmd)
					{
#if ABBA2
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 15;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
#else
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 15;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
#endif
					}
				}
				break;
			case EnvTemp_ML:
				if((FuserPaperData.BIT.Type == MTYPE_Paper) || (FuserPaperData.BIT.Type == MTYPE_Recycled))
				{
					switch(cmd)
					{
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 25;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 15;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
					}
				}
				else
				{
					switch(cmd)
					{
						case Within10PagesDX:
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
					}
				}
				break;
			case EnvTemp_L:
				if((FuserPaperData.BIT.Type == MTYPE_Paper) || (FuserPaperData.BIT.Type == MTYPE_Recycled))
				{
					switch(cmd)
					{
#if ABBA2
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 25;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
#else
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 25;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 20;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 25;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 15;
							break;
#endif
					}
				}
				else
				{
					switch(cmd)
					{
#if ABBA2
						case Within10PagesDX:
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
#else
						case Within10PagesDX:
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
#endif
					}
				}
				break;
			case EnvTemp_DL:
				if((FuserPaperData.BIT.Type == MTYPE_Paper) || (FuserPaperData.BIT.Type == MTYPE_Recycled))
				{
					switch(cmd)
					{
						case Within10PagesDX:
							Fuser_Control.Target_Temperature += 30;
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 25;
							break;
						case Over10PagesDX:
							Fuser_Control.Target_Temperature += 30;
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 20;
							break;
					}
				}
				else
				{
					switch(cmd)
					{
						case Within10PagesDX:
							break;
						case Within10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
						case Over10PagesDX:
							break;
						case Over10PagesSX:
							Fuser_Control.Target_Temperature += 10;
							break;
					}
				}
				break;
				
			default:
				break;
		}
	}
#if NEW_FUSER_TEST_PARAM
	Fuser_Control.Target_Temperature = Fuser_Setting_Parameters[0];
#endif	

#if EP_PARAM_TEST
	if(fParamTargetVALUE == SET)
		Fuser_Control.Target_Temperature = Param_Target_VALUE;
#endif

#if FUSER_PARAM_TEST
	if((ftargetTempfor25ppm == SET) && (PageID != 0))
	{
		Fuser_Control.Target_Temperature = targetTempfor25ppm;
	}
	if(f10pagesUSE == SET)
	{
		if((PageID >= 1) && (PageID <= numberofbasisofspeed))
			Fuser_Control.Target_Temperature = within10pagesTemp;
		else if(PageID > numberofbasisofspeed)
			Fuser_Control.Target_Temperature = over10pagesTemp;
		else{}
	}
#endif
	if(fDebugTargetTempChange == SET)
	{
		Fuser_Control.Target_Temperature = DEBUG_TargetTemp;
	}
#if ZX_ENABLE
	Fuser_Control.Target_Temperature = Fuser_Setting_Parameters[0];
#endif
#if	ENG_TRACE
	MSG("(Eng)## PageID =  %d,  Type = %d, Weight = %d, Texture = %d, Env = %d, cmd = %d, TargetTemp = %d <%d>##\n",PageID, FuserPaperData.BIT.Type, FuserPaperData.BIT.Weight, FuserPaperData.BIT.Texture, EnvTempStatus, cmd, Fuser_Control.Target_Temperature,cProcessCounter);
#endif
//~ #endif	
	
	return Fuser_Control.Target_Temperature;
}

int	Detect_OverSensorAtWarmup(void)
{
	/* Paper Jam Error */
	if(((IsPaperPresentOnPins() == TRUE) || (IsPaperPresentOnPexs() == TRUE)) && (fJamDetacted1st == FALSE))
	{
#if	ENG_TRACE		
		MSG("(Eng)## Detect_OverSensorAtWarmup : Input Sen = %d, Output Sen = %d (%d)##\n",IsPaperPresentOnPins(),IsPaperPresentOnPexs(),cProcessCounter);
#endif
		fJamDetacted1st = TRUE;
		return TRUE;
	}
	
	fJamDetacted1st = FALSE;
	// MSG("(Eng)$$$$$ fJamDetacted1st = FALSE (%d) $$$$$\n",cProcessCounter);
	if(IsPaperPresentOnPins() == TRUE) 
	{
		if(IsPaperPresentOnPexs() == TRUE) 
		{
			PaperJamBetweenPinsAndPexs = JAM;
		}
		else 
		{
			PaperJamOnPins = JAM;
		}
		CodeOfJamStatus = JAM_IDLE_STATE;
	}
	else if(IsPaperPresentOnPexs() == TRUE) 
	{
		PaperJamOnPexs = JAM;
		CodeOfJamStatus = JAM_IDLE_STATE;
	}
		
	if(TempPaperJam.DR.BYTE != CLEAR) 
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

void Sensor_Power_On(void)
{
#if	ENG_TRACE	
	MSG("(Eng)## SENSOR_POWER_ON ## <%d>\n", cProcessCounter);
#endif
	REL_ASSERT( OK == gpio_write( eng_hardware->sensor_pwr, 0 ) );
	REL_ASSERT( OK == gpio_write( eng_hardware->ui_nreset, 1 ) );
}

void Sensor_Power_Off(void)
{
#if	ENG_TRACE	
	MSG("(Eng)## SENSOR_POWER_OFF ## <%d>\n", cProcessCounter);
#endif	
	REL_ASSERT( OK == gpio_write( eng_hardware->ui_nreset, 0 ) );
	REL_ASSERT( OK == gpio_write( eng_hardware->sensor_pwr, 1 ) );
}

void Sensor_Power_Ctrl(char On)
{
#if	ENG_TRACE	
	MSG("(Eng)## SENSOR_POWER_CTRL = %d ## <%d>\n", On, cProcessCounter);
#endif	
	if(On == SET)
	{
		REL_ASSERT( OK == gpio_write( eng_hardware->sensor_pwr, 0 ) );
	}
	else
	{
		REL_ASSERT( OK == gpio_write( eng_hardware->sensor_pwr, 1 ) );
	}
}

void Pre_Heat_Off(void)
{
#if	ENG_TRACE	
	MSG("(Eng)## Pre_Heat_Off ## <%d>\n", cProcessCounter);
#endif	
	REL_ASSERT( OK == gpio_write( eng_hardware->pre_heat_ctrl, 0 ) );
}

void StandbyModeSelect(void)
{
	if(Fuser_Control.Current_Temperature < Fuser_Setting_Parameters[3])
	{
		Fuser_Control.StandbyModeType = 1;
	}
	else if ((Fuser_Control.Current_Temperature >= Fuser_Setting_Parameters[3]) && (Fuser_Control.Current_Temperature < Fuser_Setting_Parameters[1]))
	{
		Fuser_Control.StandbyModeType = 2;
	}
	else
	{
		Fuser_Control.StandbyModeType = 3;
	}
#if	ENG_TRACE	
	MSG("(Eng)## StandbyModeType = %d, Current_Temperature = %d ##\n",Fuser_Control.StandbyModeType, Fuser_Control.Current_Temperature);
#endif		
	Write_Fuser_Func();
}

int Set_FatalCheckNotify(Uint8 ErrorCode)
{
	MESSAGE message;
	
	message.msgType = MSG_STATUSUPDATE;
	
	switch (ErrorCode)
	{
		case NO_MAIN_MOTOR_LOCK:
			message.param1 = STATUS_FATAL_ENGINE_FEEDMOTORFAIL;
			MSG("(eng)NO_MAIN_MOTOR_LOCK\n");
			break;
		case NO_MIRROR_MOTOR_LOCK:
			message.param1 = STATUS_FATAL_ENGINE_MIRRORMOTORFAIL;
			MSG("(eng)NO_MIRROR_MOTOR_LOCK\n");
			break;
		case PRINTHEAD_LOST_HSYNC: // hsync fail
			message.param1 = STATUS_FATAL_ENGINE_HSYNCFAIL;
			MSG("(eng)PRINTHEAD_LOST_HSYNC\n");
			break;
		case FUSERFATAL300: // fuser fatal 300 - under temperature printing
			message.param1 = STATUS_FATAL_ENGINE_FUSER300;
			MSG("(eng)FUSERFATAL300\n");
			break;
		case FUSERFATAL301: // fuser fatal 301 - under temperature standby
			message.param1 = STATUS_FATAL_ENGINE_FUSER301;
			MSG("(eng)FUSERFATAL301\n");
			break;
		case FUSERFATAL302: // fuser fatal 302 - por not reach standby
			message.param1 = STATUS_FATAL_ENGINE_FUSER302;
			MSG("(eng)FUSERFATAL302\n");
			break;
		case FUSERFATAL303: // fuser fatal 303 - over temperature
			message.param1 = STATUS_FATAL_ENGINE_FUSER303;
			MSG("(eng)FUSERFATAL303\n");
			break;
		case FUSERFATAL304: // fuser fatal 304 - thermister open
			message.param1 = STATUS_FATAL_ENGINE_FUSER304;
			MSG("(eng)FUSERFATAL304\n");
			break;
		case FUSERFATAL305: // fuser fatal 305
			message.param1 = STATUS_FATAL_ENGINE_FUSER305;
			MSG("(eng)FUSERFATAL305\n");
			break;
		case MAINFANLOCKTIMEOUT: // main fan lock timeout
			message.param1 = STATUS_FATAL_ENGINE_MAINFANMOTORFAIL;
			MSG("(eng)MAINFANLOCKTIMEOUT\n");
			break;
		case SUBFANLOCKTIMEOUT: // sub fan lock timeout
			message.param1 = STATUS_FATAL_ENGINE_SUBFANMOTORFAIL;
			MSG("(eng)SUBFANLOCKTIMEOUT\n");
			break;
		case DEVFANLOCKTIMEOUT: // dev fan lock timeout
			message.param1 = STATUS_FATAL_ENGINE_DEVFANMOTORFAIL;
			MSG("(eng)DEVFANLOCKTIMEOUT\n");
			break;
		case SOFTWARE_ERROR_OPTION:
			message.param1 = STATUS_FATAL_ENGINE_OPTIONFAIL;
			MSG("(eng)SOFTWARE_ERROR_OPTION\n");
			break;
		case VIDEO_DONE_FAIL:
			message.param1 = STATUS_FATAL_ENGINE_VIDEOLONG;
			MSG("(eng)VIDEO_DONE_FAIL\n");
			break;
		default:
			MSG("(Eng)Unknown Fatal Error\n");
			break;
	}
	SYMsgSend(PRINTMGRID, &message);
	
	return TRUE;
}

int Set_JamCheckNotify(Uint8 ErrorCode)
{

	MESSAGE message;
	
	message.msgType = MSG_STATUSUPDATE;
	
	switch (ErrorCode)
	{
		case JAM_IDLE_STATE:
			engine_state.input_not_released_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_INPSEN_NOT_REL ); 
			MSG("(eng)JAM_IDLE_STATE\n");
			break;
		case JAM_LE_INPUT_DETACT_OPTION:
			engine_state.misspick_tray2_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY2 ); 
			MSG("(eng)JAM_LE_INPUT_DETACT_OPTION\n");
			break;
		case JAM_LE_INPUT_DETACT_OPTION2:
			engine_state.misspick_tray3_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY3 ); 
			MSG("(eng)JAM_LE_INPUT_DETACT_OPTION2\n");
			break;
		case JAM_DUPLEX_PATH:
			engine_state.duplex_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_DUPLEX ); 
			MSG("(eng)JAM_DUPLEX_PATH\n");
			break;
	}
	SYMsgSend(PRINTMGRID, &message);

	return TRUE;
}

void PowerKeyPressed(int keyType)
{
//	printf("(eng)PowerKeyPressed()\n");
	
	SYS_UI_MESSAGE msg;
	error_type_t err;

	msg.msgSender = ENGINEID;
	msg.cmd = CMD_INT_KEY_PRESSED;
	memset(msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stWnd_KeyPressed powerKeyPressed = {0};
	powerKeyPressed.mode = keyType;
	memcpy(msg.data, &powerKeyPressed, sizeof(stWnd_KeyPressed));				
	
	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);				
	XASSERT( err==OK, err );
	
	return;
}

void PS_24vOn(char flag) // namja, EVT2
{
	MSG("(eng)==PS_24vOn ( %d )\n", cProcessCounter);
	dec_fuser_enable_power(TRUE);
	if(flag == SET) f24vOn = TRUE;
}

void PS_24vOff(char flag) // namja, EVT2
{
	MSG("(eng)==PS_24vOff (%d)\n", cProcessCounter);
	dec_fuser_enable_power(FALSE);
	if(flag == SET) f24vOn = FALSE;
}

#if EP_PARAM_TEST
void Get_EP_Test_Parameter(void)
{
#define NUM_EP_PARAM 26
	FILE	*paramFP;
    char white1[80];
    int32_t ep_param[NUM_EP_PARAM];
    
    paramFP = fopen("/app/EP_PARAMETER.txt", "r");
    if(paramFP == NULL)
    {
		printf("(eng)EP_PARAMETER.txt open fail\n");
	}
	else
	{
		fscanf( paramFP, "%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d", 
			white1, &ep_param[0], 
			white1, &ep_param[1], 
			white1, &ep_param[2], 
			white1, &ep_param[3], 
			white1, &ep_param[4], 
			white1, &ep_param[5], 
			white1, &ep_param[6], 
			white1, &ep_param[7], 
			white1, &ep_param[8], 
			white1, &ep_param[9], 
			white1, &ep_param[10], 
			white1, &ep_param[11], 
			white1, &ep_param[12], 
			white1, &ep_param[13], 
			white1, &ep_param[14], 
			white1, &ep_param[15], 
			white1, &ep_param[16], 
			white1, &ep_param[17], 
			white1, &ep_param[18], 
			white1, &ep_param[19], 
			white1, &ep_param[20],
			white1, &ep_param[21], 
			white1, &ep_param[22],
			white1, &ep_param[23],
			white1, &ep_param[24],
			white1, &ep_param[25]
		);
		fclose(paramFP);
	}
	
	MSG("##################################\n");
	MSG("##	Chg PWM		=	%d	##\n", ep_param[0]);
	MSG("##	Chg use		=	%d	##\n", ep_param[1]);
	MSG("##	Dev PWM		=	%d	##\n", ep_param[2]);
	MSG("##	Dev use		=	%d	##\n", ep_param[3]);
	MSG("##	Txf PWM		=	%d	##\n", ep_param[4]);
	MSG("##	Txf use		=	%d	##\n", ep_param[5]);
	MSG("##	Servo		=	%d	##\n", ep_param[6]);
	MSG("##	Servo use	=	%d	##\n", ep_param[7]);
	MSG("##	IPG(Before Priting)		=	%d	##\n", ep_param[8]);
	MSG("##	IPG(Before Priting) USE		=	%d	##\n", ep_param[9]);
	MSG("##	IPG(During Printing)		=	%d	##\n", ep_param[10]);
	MSG("##	IPG(During Printing) USE		=	%d	##\n", ep_param[11]);
	MSG("##	Target Tmp	=	%d	##\n", ep_param[12]);
	MSG("##	Target use	=	%d	##\n", ep_param[13]);
	MSG("##	Pick Tmp	=	%d	##\n", ep_param[14]);
	MSG("##	Pick use	=	%d	##\n", ep_param[15]);
	MSG("##	Mtr VAL		=	%d	##\n", ep_param[16]);
	MSG("##	Mtr use		=	%d	##\n", ep_param[17]);
	MSG("##	1stPick VAL	=	%d	##\n", ep_param[18])
	MSG("##	1stPick use	=	%d	##\n", ep_param[19]);
	MSG("##	txfon time	=	%d	##\n", ep_param[20]);
	MSG("##	txfon_Low time	=	%d	##\n", ep_param[21]);
	MSG("##	txfon use	=	%d	##\n", ep_param[22]);
	MSG("##	txfoff time	=	%d	##\n", ep_param[23]);
	MSG("##	txfoff_Low time	=	%d	##\n", ep_param[24]);
	MSG("##	txfoff use	=	%d	##\n", ep_param[25]);
	MSG("##################################\n");
	
	Param_Charger_PWM = ep_param[0];
	fParamChargerPWM = ep_param[1];
	Param_Developer_PWM = ep_param[2];
	fParamDeveloperPWM = ep_param[3];
	Param_Transfer_PWM = ep_param[4];
	fParamTransferPWM = ep_param[5];
	Param_Servo_VALUE = ep_param[6];
	fParamServoVALUE = ep_param[7];
	
	Param_IPG_Before_Printing = ep_param[8];
	fParamIPGBeforePrinting = ep_param[9];
	Param_IPG_During_Printing = ep_param[10];
	fParamIPGDuringPrinting = ep_param[11];
	
	Param_Target_VALUE = ep_param[12];
	fParamTargetVALUE = ep_param[13];
	Param_Pick_VALUE = ep_param[14];
	fParamPickVALUE = ep_param[15];
	
	Param_Motor_VALUE = ep_param[16];
	fParamMotorVALUE = ep_param[17];
	
	Param_FirstPick_VALUE = ep_param[18];
	fParamFirstPickVALUE = ep_param[19];
	
	Param_TxfOnTime_VALUE = ep_param[20];
	Param_TxfOnTime_Low_VALUE = ep_param[21];
	fParamTxfOnTimeVALUE = ep_param[22];
	Param_TxfOffTime_VALUE = ep_param[23];
	Param_TxfOffTime_Low_VALUE = ep_param[24];
	fParamTxfOffTimeVALUE = ep_param[25];

	return;
}
#endif

#if FUSER_PARAM_TEST
void Get_FUSER_Test_Parameter(void)
{
	FILE	*paramFP ;
	Uint32 EPParam[6];
	char white1[80];
	
	paramFP = fopen("FUSER_Parameter.txt", "r");
	
	if(paramFP == NULL)
	{
#if	ENG_TRACE		
		printf("File open error\n");
#endif		
	}
	else
	{
		fscanf( paramFP, "%s%d%s%d%s%d%s%d%s%d%s%d", 
		white1, &EPParam[0], 
		white1, &EPParam[1], 
		white1, &EPParam[2], 
		white1, &EPParam[3],
		white1, &EPParam[4],
		white1, &EPParam[5]);
		fclose(paramFP);

#if	ENG_TRACE
		printf("+--------------------------------+\n");
		printf("|  FUSER STEP PARAMETER SETTING  |\n");
		printf("+--------------------------------+\n");
		printf("|   within 10pages temp :	%d	 |\n",EPParam[0]);
		printf("|   over 10pages temp :	%d	 |\n",EPParam[1]);
		printf("|   basis of speed(page num) :	%d	 |\n",EPParam[2]);
		printf("|   10pages USE :	%d	 |\n",EPParam[3]);
		printf("|   25ppm target temp :	%d	 |\n",EPParam[4]);
		printf("|   25ppm target temp USE:	%d	 |\n",EPParam[5]);
		printf("+--------------------------------+\n");
#endif
		if(EPParam[3]==1)
		{
			f10pagesUSE = TRUE;
			within10pagesTemp = EPParam[0];
			over10pagesTemp = EPParam[1];
			numberofbasisofspeed = EPParam[2];
#if	ENG_TRACE		
			printf("within10pagesTemp = %d, over10pagesTemp = %d, numberofbasisofspeed = %d\n", within10pagesTemp, over10pagesTemp, numberofbasisofspeed);
#endif		
		}
		else
		{
			f10pagesUSE = FALSE;
			within10pagesTemp = EPParam[0];
			over10pagesTemp = EPParam[1];
			numberofbasisofspeed = EPParam[2];
		}
		
		if(EPParam[5]==1)
		{
			ftargetTempfor25ppm = TRUE;
			targetTempfor25ppm= EPParam[4];
#if	ENG_TRACE		
			printf("targetTempfor25ppm = %d\n",targetTempfor25ppm);
#endif
		}
		else
		{
			ftargetTempfor25ppm = FALSE;
			targetTempfor25ppm= EPParam[4];
		}
	}
}
#endif

void SelectGapRingMode(void)
{
	FILE	*paramFP;
    char white1[80];
    int retcode;
    strcpy(white1, "gapring");
    
    paramFP = fopen("/app/SelectGapringMode.txt", "r");
    if(paramFP == NULL)
    {
		MSG("(eng)SelectGapringMode.txt open fail\n");
	}
	else
	{
		retcode = fscanf(paramFP, "%s", white1);
		if(retcode == -1)
		{
			MSG("(eng)SelectGapringMode.txt Read fail\n");
		}
		fclose(paramFP);
	}
	MSG("(eng)This is %s\n", white1);
	if(strcmp("gapring", white1) == 0)
	{
		if (motorType == MOTOR_NEW) {
			hv_pwm_period_mtr = HV_PWM_PERIOD_MTR_NEW;
		} else {
			hv_pwm_period_mtr = HV_PWM_PERIOD_MTR; // gap ring 73286
		}
	}
	else
	{
		hv_pwm_period_mtr = 74046; // no gap ring
	}
}

#if DARKNESS_TEST
void Get_Darkness_Parameter(void)
{
	FILE	*paramFP;
    char white1[80];
    int32_t ep_param[1];
    
    paramFP = fopen("/app/EP_PARAMETER.txt", "r");
    if(paramFP == NULL)
    {
		printf("(eng)EP_PARAMETER.txt open fail\n");
	}
	else
	{
		fscanf( paramFP, "%s%d", 
			white1, &ep_param[0]);
		fclose(paramFP);
	}
	
	MSG("##################################\n");
	MSG("##	Darkness		=	%d	##\n", ep_param[0]);
	MSG("##################################\n");
	Darknesslevel = ep_param[0];
	return;
}
#endif
void Set_Engine_Parameter(void)
{
	Engine_Parameter[0] = ENG_PARAM_TEMP_SERVO_H;
	Engine_Parameter[1] = ENG_PARAM_TEMP_SERVO_HM;
	Engine_Parameter[2] = ENG_PARAM_TEMP_SERVO_M;
	Engine_Parameter[3] = ENG_PARAM_1ST_NOR_PAGE_H;
	Engine_Parameter[4] = ENG_PARAM_1ST_NOR_PAGE_HM;
	Engine_Parameter[5] = ENG_PARAM_1ST_NOR_PAGE_M;
	Engine_Parameter[6] = ENG_PARAM_1ST_NOR_PAGE_L;
	Engine_Parameter[7] = ENG_PARAM_1ST_NOR_PAGE_H_MAX;
	Engine_Parameter[8] = ENG_PARAM_1ST_NOR_PAGE_M_MAX;
	Engine_Parameter[9] = ENG_PARAM_2ND_NOR_PAGE_H;
	Engine_Parameter[10] = ENG_PARAM_2ND_NOR_PAGE_HM;
	Engine_Parameter[11] = ENG_PARAM_2ND_NOR_PAGE_M;
	Engine_Parameter[12] = ENG_PARAM_2ND_NOR_PAGE_L;
	Engine_Parameter[13] = ENG_PARAM_SLOW_COOL_TIME_H;
	Engine_Parameter[14] = ENG_PARAM_SLOW_COOL_TIME_HM;
	Engine_Parameter[15] = ENG_PARAM_SLOW_COOL_TIME_M;
	Engine_Parameter[16] = ENG_PARAM_SLOW_COOL_TIME_L;
	Engine_Parameter[17] = ENG_PARAM_COOLING_TIME_OFFSET;
	Engine_Parameter[18] = ENG_PARAM_TEMP_SERVO_ML;
	Engine_Parameter[19] = ENG_PARAM_TEMP_SERVO_L;
}   

void Display_Ver_Info(void)
{
	char ModelName[256];

#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		sprintf(ModelName, "%s", "PINETREE-L");
	}
	else
	{
		sprintf(ModelName, "%s", "Kara-S");
	}
	
	MSG("\n(Eng)******************************************************* \n");
	MSG("(Eng)* %s Engine Application\n", ModelName);
	MSG("(Eng)* %s ENGINE SW VERSION : Ver %s (%s)\n\n",
						ModelName,
						SW_ENGINE_VERSION,
						NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE ? SW_DESC_ICJIG : SW_DESCRIPTION);
	return;
}

int machineType_check(char* tli)
{
	char *machine_tli = tli;
	int temp_machineType;
	MESSAGE message;
	
	machineLocation = LOCAT_UNKNOWN;
	
	if(strncmp(machine_tli,"A610DN40",12)==0)			{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610DN40;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A615DN40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A615DN40;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A611DN45",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611DN45;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A616DN45",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A616DN45;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A616DNS45",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A616DNS45;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61040",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61040;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61540",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61540;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61140",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61140;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61640",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61640;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61245",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M61245;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61745",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M61745;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61345",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M61345;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61845",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M61845;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A610DN40-C",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610DN40_C;		machineLocation = LOCAT_CHINA;}
	else if(strncmp(machine_tli,"A611DN45-C",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611DN45_C;		machineLocation = LOCAT_CHINA;}
	else if(strncmp(machine_tli,"M611DN40-C",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611DN40_C;		machineLocation = LOCAT_CHINA;}
	else if(strncmp(machine_tli,"M612DN45-C",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612DN45_C;		machineLocation = LOCAT_CHINA;}
	else if(strncmp(machine_tli,"A610NA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610NA40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A615NA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A615NA40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A610MX40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610MX40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A610BR40-L",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610BR40_L;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610BR40-H",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610BR40_H;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610TR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610TR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610ZA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610ZA40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610TN40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610TN40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610AE40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610AE40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610IR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610IR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610RU40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610RU40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610IN40-HD",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610IN40_HD;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610IN40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610IN40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610MY40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610MY40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610SA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610SA40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610AU40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610AU40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610AR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610AR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610ES40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610ES40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610PY40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610PY40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A611NA47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611NA47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A616NA47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A616NA47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A611MX47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611MX47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A611BR47-L",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611BR47_L;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611BR47-H",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611BR47_H;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611TR47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611TR47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611ZA47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611ZA47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611TN47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611TN47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611AE47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611AE47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611IR47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611IR47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611RU47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611RU47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611IN47-HD",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611IN47_HD;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611IN47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611IN47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611MY47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611MY47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611SA47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611SA47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611AU47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611AU47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611AR47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611AR47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611ES47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611ES47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611PY47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611PY47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M611NA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611NA40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M611CO40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611CO40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M611MX40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611MX40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M611BR40-L",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611BR40_L;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611BR40-H",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611BR40_H;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611TR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611TR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611ZA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611ZA40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611TN40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611TN40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611AE40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611AE40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611IR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611IR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611RU40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611RU40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611IN40-HD",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611IN40_HD;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611IN40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611IN40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611MY40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611MY40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611SA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611SA40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611AU40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611AU40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611AR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611AR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611ES40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611ES40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611PY40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611PY40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M612NA47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612NA47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M612CO47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612CO47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M612MX47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612MX47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M612BR47-L",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612BR47_L;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612BR47-H",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612BR47_H;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612TR47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612TR47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612ZA47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612ZA47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612TN47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612TN47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612AE47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612AE47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612IR47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612IR47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612RU47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612RU47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612IN47-HD",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612IN47_HD;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612IN47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612IN47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612MY47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612MY47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612SA47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612SA47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612AU47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612AU47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612AR47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612AR47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612ES47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612ES47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612PY47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612PY47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A413DN36",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A413DN36;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M610NA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610NA40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M610BR40-L",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610BR40_L;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610IR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610IR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610RU40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610RU40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610IN40-HD",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610IN40_HD;		machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610IN40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610IN40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610MY40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610MY40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610SA40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610SA40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610AU40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610AU40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610AR40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610AR40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610CO40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610CO40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M610ES40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610ES40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M610PY40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M610PY40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A610PE40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610PE40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A611PE47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611PE47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M611PE40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611PE40;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"M612PE47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612PE47;			machineLocation = LOCAT_NA_A4;}
	else if(strncmp(machine_tli,"A610CL40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610CL40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A611CL47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611CL47;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M611CL40",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611CL40;			machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M612CL47",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612CL47;			machineLocation = LOCAT_NA_LT;}
	
	// kara
	else if(strncmp(machine_tli,"A610DN40H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610DN40H;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A615DN40H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A615DN40H;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A610LV40H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610LV40H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A610HV40H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610HV40H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A610DN40-CH",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_A610DN40_CH;		machineLocation = LOCAT_CHINA;}
	
	// kara-s
	else if(strncmp(machine_tli,"A611DN45H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611DN45H;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A616DN45H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A616DN45H;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A616DNS45H",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A616DNS45H;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A611LV47H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611LV47H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A611HV47H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611HV47H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"A611DN45HHU",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611DN45HHU;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A611DN45HH",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611DN45HH;		machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"A611DN45-CH",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_A611DN45_CH;		machineLocation = LOCAT_CHINA;}
	else if(strncmp(machine_tli,"P247B01",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_P247B01;			machineLocation = LOCAT_NA_A4;}
	
	// pinetree
	else if(strncmp(machine_tli,"M61140H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61140H;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61640H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61640H;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M611LV40H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611LV40H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M611HV40H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611HV40H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M611DN40-CH",12)==0)	{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M611DN40_CH;		machineLocation = LOCAT_CHINA;}
	
	// pinetree-s
	else if(strncmp(machine_tli,"M61245H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M61245H;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61745H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M61745H;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M612LV47H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612LV47H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M612HV47H",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612HV47H;		machineLocation = LOCAT_NA_LT;}
	else if(strncmp(machine_tli,"M61245HH",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M61245HH;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M612DN45-CH",12)==0)	{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M612DN45_CH;		machineLocation = LOCAT_CHINA;}
	else if(strncmp(machine_tli,"M247B01",12)==0)		{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_M247B01;			machineLocation = LOCAT_NA_A4;}
	
	// pinetree-l
	else if(strncmp(machine_tli,"M61040H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61040H;			machineLocation = LOCAT_KOREA;}
	else if(strncmp(machine_tli,"M61540H",12)==0)		{temp_machineType = ABBA_OAK_M;			Machine_TLI = TLI_M61540H;			machineLocation = LOCAT_KOREA;}
		
	else if(strncmp(machine_tli,"NOTLI",12)==0)			{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_DEFAULT;			machineLocation = LOCAT_UNKNOWN;}
	else												{temp_machineType = ABBA_OAK_H;			Machine_TLI = TLI_ERROR;			machineLocation = LOCAT_UNKNOWN;}
	
	MSG("(eng)## machineTLI = %s (%d), location = %d ##\n", machine_tli, Machine_TLI, machineLocation);
	MSG("(eng)## machine type = %d ##\n", temp_machineType);
	
	slogit( message.msgType = MSG_MACHINE_LOCATION_INFO );
	message.param1 = machineLocation; 
	SYMsgSend(PRINTMGRID, &message);
	
	return temp_machineType;
}

Uint8 Get_TrayInfo(Uint8 Tray_num)
{
	Uint8 tray_info = 0;
	switch (Tray_num)
	{
		case 1: // tray1
			tray_info = Tray_Status.Tray1.newTrayStatus;
			break;
		case 2: // tray2
			tray_info = Tray_Status.Tray2.newTrayStatus;
			break;
		case 3: // tray3
			tray_info = Tray_Status.Tray3.newTrayStatus;
			break;
		case 4: // mpt
			tray_info = Tray_Status.Tray4.newTrayStatus;
			break;
	}
/*	
	TRAY_MEDIA_UNKNOWN		0x10
	TRAY_MEDIA_LT			0x20
	TRAY_MEDIA_EXEC			0x30
	TRAY_MEDIA_A4			0x40
	TRAY_MEDIA_B5			0x50
	TRAY_MEDIA_LG			0x60
	TRAY_MEDIA_A5			0x70
	
	TRAY_MEDIA_NORMAL		0x03
	TRAY_MEDIA_LOW 			0x02
	TRAY_MEDIA_EMPTY		0x01
*/
	return tray_info;
}

void Check_Sensor_Status(void)
{
	int watchdog_count = 0;
	int sensor_changed = 0;
	watchdog_service();
	
	Fuser_Control.RequestState = ST_OFF;
	ClearAllPageSupervisor();
	DisableAllOutputPorts(); 
	StopMainMotor();

	if(f2ndDrawerState == SET) Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
	if(f3ndDrawerState == SET) Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
	
	Get_Init_Sensor_Status();
	fSensorTest =1;
	fHardwareTest.bit.b0 = 1;
	while(hardware_test.sensor_check)
	{
		Get_Present_Sensor_Status();
		
		if(SP_Old_PinSenorStatus != SP_New_PinSenorStatus)
		{
			SP_Old_PinSenorStatus = SP_New_PinSenorStatus;
			sensor_changed = ON;
#if 0
			if(SP_New_PinSenorStatus == PRESENT)
			{
#if ENG_TRACE				
				MSG("##### SP Mode : Input Sensor Detacted!!! #####\n");
#endif				
				//Set_SensorStatusInfoNotify(SP_InputSensor,PRESENT);
			}
			else
			{
#if ENG_TRACE				
				MSG("##### SP Mode : Input Sensor Released!!! #####\n");
#endif				
				//Set_SensorStatusInfoNotify(SP_InputSensor,NOT_PRESENT);
			}
#endif
		}
		if(SP_Old_PexSenorStatus != SP_New_PexSenorStatus)
		{
			SP_Old_PexSenorStatus = SP_New_PexSenorStatus;
			sensor_changed = ON;
#if 0
			if(SP_New_PexSenorStatus == PRESENT)
			{
#if ENG_TRACE				
				MSG("##### SP Mode : Exit Sensor Detacted!!! #####\n");
#endif				
				//Set_SensorStatusInfoNotify(SP_ExitSensor,PRESENT);
			}
			else
			{
#if ENG_TRACE				
				MSG("##### SP Mode : Exit Sensor Released!!! #####\n");
#endif				
				//Set_SensorStatusInfoNotify(SP_ExitSensor,NOT_PRESENT);
			}
#endif
		}
		if(SP_Old_MPTSenorStatus != SP_New_MPTSenorStatus)
		{
			SP_Old_MPTSenorStatus = SP_New_MPTSenorStatus;
			sensor_changed = ON;
#if 0
			if(SP_New_MPTSenorStatus == PRESENT)
			{
#if ENG_TRACE				
				MSG("##### SP Mode : MPT Sensor Detacted!!! #####\n");
#endif				
				//Set_SensorStatusInfoNotify(SP_MPTSensor,PRESENT);
			}
			else
			{
#if ENG_TRACE				
				MSG("##### SP Mode : MPT Sensor Released!!! #####\n");
#endif				
				//Set_SensorStatusInfoNotify(SP_MPTSensor,NOT_PRESENT);
			}
#endif
		}
		if(SP_Old_CoverStatus != SP_New_CoverStatus)
		{
			SP_Old_CoverStatus = SP_New_CoverStatus;
			if(SP_New_CoverStatus == CLOSE)
			{
#if ENG_TRACE				
				MSG("##### SP Mode : Door Cover CLOSE!!! #####\n");
#endif				
				//Set_SensorStatusInfoNotify(SP_CoverSwitch,CLOSE);
			}
			else
			{
#if ENG_TRACE				
				MSG("##### SP Mode : Door Cover OPEN!!! #####\n");
#endif					
				//Set_SensorStatusInfoNotify(SP_CoverSwitch,OPEN);
			}
		}
		
		if ( sensor_changed == ON ) {
			sensor_changed = OFF;
			Uint8 buffer[3];
			buffer[0] = SP_Old_MPTSenorStatus;
			buffer[1] = SP_Old_PinSenorStatus;
			buffer[2] = SP_Old_PexSenorStatus;
			SPResponseSensorTest(buffer);
		}
		usleep(10000);
		watchdog_count++;
		if(watchdog_count >= 10)
		{
			watchdog_service();
		}
	}
}

void Get_Init_Sensor_Status(void)
{
	SP_Old_PinSenorStatus = -1;//IsPaperPresentOnPins();
	SP_Old_PexSenorStatus = -1;//IsPaperPresentOnPexs();
	SP_Old_MPTSenorStatus = -1;//IsPaperPresentOnMPF();
	SP_Old_CoverStatus = -1;//IsDoorCoverState();
}

void Get_Present_Sensor_Status(void)
{
	SP_New_PinSenorStatus = IsPaperPresentOnPins();
	SP_New_PexSenorStatus = IsPaperPresentOnPexs();
	SP_New_MPTSenorStatus = IsPaperPresentOnMPF();
	SP_New_CoverStatus = IsDoorCoverState();
}

void Sensor_Test_Off(void)
{
#if ENG_TRACE				
	MSG("##### SP Mode : Sensor Test Off !!! #####\n");
#endif				
	fSensorTest = 0;
	fHardwareTest.bit.b0 = 0;
	//fClearErrorsCmdRcv = TRUE ;
	//fEngineRestartCmd = RECEIVED;
	//Fuser_Control.EM_State = EMS_RecoverError;
}

void All_Solenoid_On(void)
{
	fHardwareTest.bit.b1 = 1;
	TurnOnMPFSolenoid();
	TurnOnDuplexSolenoid();
}

void All_Solenoid_Off(void)
{
	TurnOffMPFSolenoid();
	TurnOffDuplexSolenoid();
	fHardwareTest.bit.b1 = 0;
}

void Solenoid_Test(void)
{
	MSG("##### SP Mode : Solenoid_Test!!! #####\n");
	All_Solenoid_On();
	usleep(2000000);
	All_Solenoid_Off();
}

void sp_mode_hardware_test_init(void)
{
	hardware_test.test_off = OFF;
	hardware_test.dcmotor_t1_check = OFF;
	hardware_test.dcmotor_t2_check = OFF;
	hardware_test.dcmotor_t3_check = OFF;
	hardware_test.sensor_check = OFF;
	hardware_test.solenoid_check = OFF;
	hardware_test.tray_status_t1_check = OFF;
	hardware_test.tray_status_t2_check = OFF;
	hardware_test.tray_status_t3_check = OFF;
}

#if MPT_SENSOR_PIN_CHG
void SelectMachineTypeForScanIRQ(void)
{
	FILE	*paramFP;
    paramFP = fopen("/app/scan.irq", "r");
    
    if(paramFP == NULL)
    {
		MSG("(eng)/app/scan.irq NOT exist : KARA-S\n");
		fScan_IRQ = FALSE;
	}
	else
	{
		MSG("(eng)/app/scan.irq exist : PINETREE(S)\n");
		fScan_IRQ = TRUE;
		fclose(paramFP);
	}
	return;
}
#endif

char TrayCloseRecoveryEnable(char tray_num)
{
#if 0
	char res = true;
	char Jam_Num = false;
		
	MSG("(eng)%s : paper_out_tray = (%d,%d,%d,%d), GetTrayAttachInfo = (%d,%d,%d), GetTrayMediaLevel = (%d,%d,%d,%d)\n"
		, __func__, engine_state.paper_out_tray1, engine_state.paper_out_tray2, engine_state.paper_out_tray3, engine_state.paper_out_tray4
		, GetTrayAttachInfo(Tray_Status.Tray1.newTrayStatus), GetTrayAttachInfo(Tray_Status.Tray2.newTrayStatus), GetTrayAttachInfo(Tray_Status.Tray3.newTrayStatus)
		, GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus), GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus), GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus), GetTrayMediaLevel(Tray_Status.Tray4.newTrayStatus));

	if(engine_state.paper_out_tray1 == true) Jam_Num = 1;
	else if(engine_state.paper_out_tray2 == true) Jam_Num = 2;
	else if(engine_state.paper_out_tray3 == true) Jam_Num = 3;
	else if(engine_state.paper_out_tray4 == true) Jam_Num = 4;
	else {}
	
	switch (Jam_Num)
	{
		case 4:
			if((engine_state.paper_out_tray4 == true) && (GetTrayMediaLevel(Tray_Status.Tray4.newTrayStatus) == TRAY_MEDIA_EMPTY))
			{
				return false;
			}
			break;
		case 3:
			if(GetTrayAttachInfo(Tray_Status.Tray3.newTrayStatus) == CLEAR)
			{
				return false;
			}
			else
			{
				if((engine_state.paper_out_tray3 == true) && (GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus) == TRAY_MEDIA_EMPTY))
				{
					return false;
				}
			}
		case 2:
			if(GetTrayAttachInfo(Tray_Status.Tray2.newTrayStatus) == CLEAR)
			{
				return false;
			}
			else
			{
				if((engine_state.paper_out_tray2 == true) && (GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus) == TRAY_MEDIA_EMPTY))
				{
					return false;
				}
			}
		case 1:
			if(GetTrayAttachInfo(Tray_Status.Tray1.newTrayStatus) == CLEAR)
			{
				return false;
			}
			else
			{
				if((engine_state.paper_out_tray1 == true) && (GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus) == TRAY_MEDIA_EMPTY))
				{
					return false;
				}
			}
			break;
		default:
			break;
	}
	
	return res;
#endif
	char res = false;
	char temp_tray_num = 0;	
	temp_tray_num = tray_num;
	
	if((engine_state.misspick_tray1_jam == true)
	&& (engine_state.misspick_tray2_jam == true)
	&& (engine_state.misspick_tray3_jam == true)
	&& (engine_state.misspick_tray4_jam == true)
	&& (engine_state.exit_not_detacted_jam == true)
	&& (engine_state.input_not_released_jam == true)
	&& (engine_state.exit_not_released_jam == true)
	&& (engine_state.duplex_jam == true))
	{
		temp_tray_num = 0;
	}
	
	MSG("(eng)%s : paper_out_tray = (%d,%d,%d,%d), paper_jam = (%d,%d,%d,%d,%d,%d,%d,%d),  GetTrayAttachInfo = (%d,%d,%d), GetTrayMediaInfo = (%x,%x,%x:%x), GetTrayMediaLevel = (%d,%d,%d,%d)\n"
		, __func__, engine_state.paper_out_tray1, engine_state.paper_out_tray2, engine_state.paper_out_tray3, engine_state.paper_out_tray4
		, engine_state.misspick_tray1_jam, engine_state.misspick_tray2_jam, engine_state.misspick_tray3_jam, engine_state.misspick_tray4_jam
		, engine_state.exit_not_detacted_jam, engine_state.input_not_released_jam, engine_state.exit_not_released_jam, engine_state.duplex_jam
		, GetTrayAttachInfo(Tray_Status.Tray1.newTrayStatus), GetTrayAttachInfo(Tray_Status.Tray2.newTrayStatus), GetTrayAttachInfo(Tray_Status.Tray3.newTrayStatus)
		, GetTrayMediaInfo(Tray_Status.Tray1.newTrayStatus), GetTrayMediaInfo(Tray_Status.Tray2.newTrayStatus), GetTrayMediaInfo(Tray_Status.Tray3.newTrayStatus), Save_emptyTrayStatus
		, GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus), GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus), GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus), GetTrayMediaLevel(Tray_Status.Tray4.newTrayStatus));
	
	if(temp_tray_num == 4) // mpt
	{
		res = true;
	}
	else
	{
		if(temp_tray_num != 0)
		{
			if((GetTrayAttachInfo(Tray_Status.Tray1.newTrayStatus) == true)
			&& (f2ndDrawerState?(GetTrayAttachInfo(Tray_Status.Tray2.newTrayStatus) == true):true)
			&& (f3ndDrawerState?(GetTrayAttachInfo(Tray_Status.Tray3.newTrayStatus) == true):true))
			{
				if(Save_emptyTrayStatus == TRAY_MEDIA_UNKNOWN)
				{
					res = true;
				}
				else
				{
					if(((Save_emptyTrayStatus == GetTrayMediaInfo(Tray_Status.Tray1.newTrayStatus)) && (GetTrayMediaLevel(Tray_Status.Tray1.newTrayStatus) != TRAY_MEDIA_EMPTY))	// tray1
					|| ((Save_emptyTrayStatus == GetTrayMediaInfo(Tray_Status.Tray2.newTrayStatus)) && (GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus) != TRAY_MEDIA_EMPTY)) 	// tray2
					|| ((Save_emptyTrayStatus == GetTrayMediaInfo(Tray_Status.Tray3.newTrayStatus)) && (GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus) != TRAY_MEDIA_EMPTY))) 	// tray3
					{
						res = true;
					}
				}
			}
		}
	}
	
	return res;
}

void WakeupProcess(void)
{
	gpio_write( eng_hardware->mainmotor_start, 1 ); // BLDC_ST for PWS (Low -> High)
	gpio_write( eng_hardware->mainmotor_dir, 1 );	// BLDC_DIR for PWS (Low -> High)
												// nLSU_Motor_CLK for PWS (High -> Low)
	Sensor_Power_On();						
	PS_24vOn(SET);
	watchdog_service();
	
	if(f2ndDrawerState_1stdetect == TRUE)
	{
		usleep(2500*1000);
		watchdog_service();
		Tray2_Detection_Check();
		watchdog_service();
		usleep(100*1000);
		if(f2ndDrawerState == SET) 	{ fQuickly_sensor_check_T2 = SET;		fTray2Notify = TRUE;		Option_Tx_Notify(OPTION_T1); }
		usleep(10*1000);
		if(f3ndDrawerState == SET) 	{ fQuickly_sensor_check_T3 = SET;		fTray2Notify = TRUE;		Option_Tx_Notify(OPTION_T2); }
		usleep(10*1000);
		if(((f2ndDrawerState == CLEAR) && (f2ndDrawerState_1stdetect == TRUE)) || ((f3ndDrawerState == CLEAR) && (f3rdDrawerState_1stdetect == TRUE)))
		{
			fRetryDetectInStandby = SET;
		}
	}
	else
	{
		// waiting for sensor level stabilization
		usleep(500*1000);
	}
	
	CheckMPF();
	fQuickly_sensor_check_T1 = SET;
	CheckTray1();
	Pending_Tray2_Detection_Result();
	
	SecurityIC_Check();
	SmartICInfoNotify();
}

uint32_t GetPreheatingTime_Laminate(uint8_t condition)
{
	uint8_t servoIdx = 0;
	uint8_t timeIdx = 0;
	uint32_t timeInterval = 0;
	
	if(TransferBiasPWMForServo < 70)				servoIdx = 0;
	else if(TransferBiasPWMForServo < 110)			servoIdx = 1;
	else 											servoIdx = 2;
	MSG("(eng)servoIdx = %d\n", servoIdx);
	
	if(condition == CON_MTR_STOP_LAMI1)
	{
		timeInterval = SelectPreheatingCount - mtrstop_count_laminating1;
		MSG("(eng)condition = %d, %d-%d = %d\n", condition, SelectPreheatingCount, mtrstop_count_laminating1, SelectPreheatingCount - mtrstop_count_laminating1);
		
		if(timeInterval <= 500) 			timeIdx = 0;
		else if(timeInterval <= 1000) 		timeIdx = 1; 
		else if(timeInterval <= 2000) 		timeIdx = 2; 
		else if(timeInterval <= 4000) 		timeIdx = 3; 
		else if(timeInterval <= 6000) 		timeIdx = 4; 
		else if(timeInterval <= 12000) 		timeIdx = 5; 
		else if(timeInterval <= 18000) 		timeIdx = 6; 
		else if(timeInterval <= 30000) 		timeIdx = 7; 
		else 								timeIdx = 8;
	}
	else
	{
		timeInterval = SelectPreheatingCount - mtrstop_count_laminating2;
		MSG("(eng)condition = %d, %d-%d = %d\n", condition, SelectPreheatingCount, mtrstop_count_laminating2, SelectPreheatingCount - mtrstop_count_laminating2);
		
		if(timeInterval <= 500) 			timeIdx = 0;
		else if(timeInterval <= 1000) 		timeIdx = 1; 
		else if(timeInterval <= 2000) 		timeIdx = 2; 
		else if(timeInterval <= 3000) 		timeIdx = 3; 
		else if(timeInterval <= 4000) 		timeIdx = 4; 
		else if(timeInterval <= 6000) 		timeIdx = 5; 
		else if(timeInterval <= 9000) 		timeIdx = 6; 
		else if(timeInterval <= 18000) 		timeIdx = 7; 
		else 								timeIdx = 8;
	}
	MSG("(eng)GetPreheatingTime_Laminate = %d\n", PreHeatingtime_Laminate[servoIdx][condition][timeIdx]);
	
	return PreHeatingtime_Laminate[servoIdx][condition][timeIdx];
}

uint32_t GetPreHeatingDoneTime(void)
{
	uint8_t servoIdx = 0;
	
	if(TransferBiasPWMForServo < 70)				servoIdx = 0;
	else if(TransferBiasPWMForServo < 110)			servoIdx = 1;
	else 											servoIdx = 2;
	MSG("(eng)preheatingTime = %d\n", preheatingTime[servoIdx]);
		
	return preheatingTime[servoIdx];
}

int Get_MotorType()
{
	error_type_t db_retval = FAIL;
	int motorType = MOTOR_OLD;
	int temp_motorType;

	temp_motorType = db_get_var_int("CONF_SYSTEMSP", "MotorType", &db_retval);

	if (db_retval != FAIL) {
		motorType = temp_motorType == 0 ? MOTOR_OLD : MOTOR_NEW;
	}

	return motorType;
}