/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	pagesupervisor.c					    				
 **	 Revision  :	ver 1.4								    			
 **	 Date	   : 	Dec 21, 2009										
 **	 Author    :	Youngjin Lee		
 +-----------------------------------------------------------------+*/

#include <stdio.h>
#include "pioname.h"
#include "farch.h"
#include "em.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <string.h>

#include "print_job_types.h"
#include "printvars_api.h"

#include "gpio_api.h"
#include "dcmotor_ol.h" // dcmotor_openloop.h
#include "pwm_api.h"
#include "dec_sensor.h"
#include "dec_fuser.h"
#include "dec_laser.h"
#include "dec_adc_api.h"
#include "option.h"
#include "ipc.h"
#include "agConnectMgr.h"
#include "print_job_api.h"


#define DBG_PRFX "engsup: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING


#define _Set_PS_SequenceID_Change(id) \
	PGSPVSR->SequenceID = id;

#define	_Check_T1_and_RET	\
	if(PGSPVSR->T1_Value < PGSPVSR->T1_Comp)	\
		return;	

#define	_Check_T2_and_RET	\
	if ((PGSPVSR->T2_Value) > CLEAR)	\
		return;	
		
#define	_Set_T1(time, id)	\
	PGSPVSR->T1_Value = 0;	\
	PGSPVSR->T1_Comp = time;	\
	PGSPVSR->T1_State = id;
	
#define	_Set_T2(time, id)	\
	PGSPVSR->T2_Value = time;	\
	PGSPVSR->T2_State = id;

#define	_AddTime_T1(time, id)	\
	PGSPVSR->T1_Comp = PGSPVSR->T1_Value + time;	\
	PGSPVSR->T1_State = id;

#define	_Set_PageAheadT1(time, id)	\
	PGSPVSR->PageAhead->T1_Value = 0;	\
	PGSPVSR->PageAhead->T1_Comp = time;	\
	PGSPVSR->PageAhead->T1_State = id;	

#define	_Set_PageAhead_PS_SequenceID_Change(id)	\
	PGSPVSR->PageAhead->SequenceID = id;

extern void LaminatingSendMsgToEng(AGMSG msgType);
extern void LaminatingSendMsgToPM(uint32_t parameter1);

extern void Sensor_Power_On(void);
extern void Save_PrintPageParameter(void);
extern void	StartMainMotor(void);
extern void	StartMirrorMotor(void);

extern void	StartEP_PrintFromIdle(void);
extern void	StartEP_PrintFromStandby(void);

extern Uint32	DetectStartPaperToPins(PageSupervisor *);

extern void	StartTransferCleaning(void);
extern void	StartChargeCleaning(void);
extern void	StartDeveloperCleaning(void);
extern void	StartDeveloperNoPrinting(void);
extern void	StartDeveloperDefault(void);
extern void	FinishLaminationProcess(void);
extern void	FinishIdleProcess(void);

extern Uint8    Get_MainMotorState(void);
extern Uint8	Get_MainMotorLockState(void);
extern Uint8	Get_MirrorMotorLockState(void);
extern Uint8	TurnOnMPFSolenoid(void);
extern Uint8	TurnOffMPFSolenoid(void);
extern Uint8	TurnOnTray1Solenoid(Uint8);
extern Uint8	TurnOffTray1Solenoid(void);
extern Uint8 	TurnOnDuplexSolenoid(void);
extern Uint8 	TurnOffDuplexSolenoid(void);
extern void	Set_TransferBiasInterpageGap_Laminating(void);

extern void	StartDeveloperLaminating(void);
extern void	StartDeveloperLaminatingReady(void);

extern void	 	Motor_BWD(void);
extern void	 	Motor_FWD(void);

extern uint32_t clear_engine_state(void);

extern void	StopMainMotor(void);
extern void	StopMirrorMotor(void);

extern void	StartEP_Laminating(void);
extern void	StartEP_POR(void);
extern void	StartServoSearch(void);
extern Uint8	ServoSearchUpProcedure(Uint8 step);
extern void	ServoSearchDownProcedure(Uint8 step);

/* Paper Control */
extern Uint8	IsPaperPresentOnPins(void);
extern Uint8	IsPaperPresentOnPexs(void);
extern Uint8	IsPaperPresentOnMPF(void);
extern Uint8	IsPaperPresentOnTray1(void);

extern void Check_Sensor_Status(void);
extern void Sensor_Test_Off(void);
extern void Solenoid_Test(void);
extern void All_Solenoid_On(void);
extern void All_Solenoid_Off(void);
extern void sp_mode_hardware_test_init(void);

extern sp_mode_hardware_test_t hardware_test;

extern void StartEP_JOG(void);
#if FUSER_PARAM_TEST
extern Uint8 f10pagesUSE;
extern Uint8 ftargetTempfor25ppm;

extern Uint8 within10pagesTemp;
extern Uint8 over10pagesTemp;
extern Uint8 targetTempfor25ppm;
#endif
extern Uint8 Tray2FeedSensorOn;
extern Uint8 Tray3FeedSensorOn;

extern Uint8 fDebugOptionCommFail;

extern Uint8 MotorStartForLaminating;
extern Uint8 RunEngineForLamination;

/* video interface */
extern int		Set_PrintReadyNotify(void);
extern Uint8	Set_PrintCompleteNotify(Uint32);			//24Apr09 yjlee
//extern Uint8	Set_MainMotorRunningNotify(void);				//24Apr09 yjlee
extern int 		Set_LSUOnNotify(void);
extern int 		Set_LSUOffNotify(void);
extern int 		Set_CoolingNotify(Uint8);
extern int 		Set_FullStackNotify(Uint8);
int 		full_stack_notify(Uint8);
extern int 		Set_SourceEmptyNotify(Uint8, Uint8);

extern Boolean	Get_Tray2SensorStatus(void);
extern int cTimerStart_Motor;

extern void	Boost_Charge(void);

extern Uint32	Get_WaitTimeForNextPickup(PageSupervisor *);
extern Uint32 	Get_WaitTimeForNarrowMedia(PageSupervisor *);
extern Uint32 	DetectEndPaperToPins(PageSupervisor *);

extern void		MainMotorSpeedChange(int, Uint8);
extern void 	MirrorMotorSpeedChange(int, Uint8);

extern uint32_t GetPreHeatingDoneTime(void);
extern uint32_t GetPreheatingTime_Laminate(uint8_t);
extern uint32_t mtrstop_count_laminating1;
extern uint32_t mtrstop_count_laminating2;
extern uint32_t SelectPreheatingCount;
extern uint32_t PreHeatingDoneTime;
extern uint8_t lami_condition;
extern Uint8 StopLamination;
extern Uint8 ResumeLamination;

extern Uint8 JamCode_Laminating;

int SolenoidOnTimeCount;
Uint8 fSolenoidOnOff;
int tmp_SolenoidOnTime;
extern Uint8 LaminatingMode;
extern Uint32 JogModeCountFor24v;
extern Uint8 JogModeTimeIdx;

extern Uint8 Machine_TLI;
extern int 	cProcessCounter;
extern int 	cTempProcessCnt;
int 	cTempPickupTime;
#if DUPLEX_SLOW_MODE
int cPrintOver300pagesTime[DUPLEXPRINTSLOWPAGES];
int TopPrintOver300pagesTime;
int cDuplexPrintSlowlyFor15min;
extern Uint8 fDebugDuplexPrintSlowlyFor15minClear;
Uint8 fDuplexPrintSlowlyFor15min;
#endif
#if DUPLEX_SLOW_MODE2
Uint8 fDuplexPrintSlowlyForServoHH;
#endif
/* EP Control */
extern void	EnableChargeBiasSignal(void);
extern void	DisableTransferBiasSignal(void);

extern void	Set_DeveloperBiasForPrinting(Uint8 Darkness, Uint8 TonerSaver);
extern void Set_DeveloperBiasForServo(Uint8 TransferServo_PWM);
extern void	Set_DeveloperBiasForNoPrint(void);
extern void	Set_Developer_PWM(Uint8 PWM);
extern void	Set_Charge_PWM(Uint8 PWM);

extern void Set_TransferBiasInterpageGap(Uint8 IPG);
extern void	Set_TransferPrinting(Uint8 MediaSpec, Uint8 Source, Uint8 Destination);

extern void 	DisableDevPWM(void);	
extern void	DisableChargeBiasSignal(void);
extern bool printer_get_page(uint32_t);
extern char fTrayOpen;
extern char fTray2MotorStop;
extern char fTray2MotorRun;
extern char fTray2PickUp;
extern char fTray3MotorStop;
extern char fTray3MotorRun;
extern char fTray3PickUp;
extern Uint8 SecondPage;
char fTray1Empty;
extern Uint8	Set_FuserTemp(Uint32 Source, Uint32 OutputDestination, Uint32 PaperData, Uint32 PageID, Uint8 PowersaveToPrint);

extern Uint8 MemoryWrite(Uint16, Uint32);
extern void Write_Fuser_Func(void);

extern int	Detect_OverSensorAtWarmup(void);
extern Uint8	IsPaperPresentOnStack(void);

extern Uint32   SavedPaperPath;
#if SMART_IC_LOCK_ENABLE
extern Uint8 ftoner_count_write_for_coveropen;
#endif

extern Uint8 EP_ChargerTable[Num_ChargerTable];
extern Uint8 EP_DeveloperTable[Num_DeveloperTable];
extern Uint8 EP_TransferTable[Num_DeveloperTable];

extern Uint8 FanCtrlEnvIdx;
extern Uint8 JogModeEnvIdx;
extern STRUCT_TRAY_STATUS Tray_Status;

/* Fuer */
extern unsigned char OptionTray1State[2];
extern unsigned char OptionTray2State[2];
/* [KSR032912] */
//char fMode_Job;
unsigned char fMode_Job; /* [KSR032914] */
//unsigned int Mode_Job[NUM_TONER_JOB];
int Mode_Job[NUM_TONER_JOB];
unsigned int Drum_Mode_Job[NUM_TONER_JOB];
Uint32 cFreeMotorOn;
Uint32 cDrumFreeMotorOn;
Uint16 SmartICWrite_TonerHisJobADRESS;
Uint32 SmartICWrite_TonerHisJob;
Uint32 SmartICWrite_DrumHisJob;
extern Uint32 cPrintData;
extern Uint32 cPageCompleteCnt;
extern Uint8 fSmartICWrite_FreeMotorOn;
extern Uint8 fSmartICWrite_DrumFreeMotorOn;
extern Uint8 fSmartICWrite_TonerHisJob;
extern Uint8 fSmartICWrite_DrumHisJob;

extern Uint8 fWakeup;

extern Uint8 cOptionCommFail;

extern unsigned char OptionTray1Pro;
extern unsigned char OptionTray2Pro;

#if EP_PARAM_TEST
extern Uint8	Param_Charger_PWM;
extern Uint8	Param_Developer_PWM;
extern Uint8	Param_Transfer_PWM;
extern Uint8	Param_Servo_VALUE;
extern Uint8	Param_Target_VALUE;
extern Uint8	Param_Pick_VALUE;
extern Uint8	Param_FirstPick_VALUE;
extern Uint8 	Param_TxfOnTime_VALUE;
extern Uint8 	Param_TxfOnTime_Low_VALUE;
extern Uint8 	Param_TxfOffTime_VALUE;
extern Uint8 	Param_TxfOffTime_Low_VALUE;

extern Uint8	fParamChargerPWM;
extern Uint8	fParamDeveloperPWM;
extern Uint8	fParamTransferPWM;
extern Uint8	fParamServoVALUE;
extern Uint8	fParamTargetVALUE;
extern Uint8	fParamPickVALUE;
extern Uint8	fParamFirstPickVALUE;
extern Uint8	fParamTxfOnTimeVALUE;
extern Uint8	fParamTxfOffTimeVALUE;
#endif

extern Uint32	SavedRealPaperLength;
extern Uint32	SavedRealMediaSizeInMPF;
extern Uint32	SavedRealMediaSizeInTray1;
extern Uint32	SavedRealMediaSizeInTray2;
extern Uint32	SavedRealMediaSizeInTray3;

extern Uint8	fMainMotorJogMode;
extern Uint8	fMotorRunWithoutPrintCmd;
extern Uint8	fMotorRun;

extern Uint8	TransferBiasPWMForServo;
extern Uint8	TransferBiasPWMForPrinting;
extern Uint8	SavedChargePWM;
extern Uint8	SavedDeveloperPWM;
extern Uint8	SavedTransferPWM;

extern Uint8 fNewTonerCheck; /* [KSR032501] for SMC */
extern Uint8 fSmartICWrite_PageCount;
extern Uint8 fSmartICWrite_DrumPageCount;/* [KSR032912] */
extern Uint8 fSmartICWrite_UsedCheck;

extern Uint8 cChargerCnt;
extern Uint8 cDeveloperCnt;
extern Uint8 cTransferCnt;

extern Uint8	fHSync_Detacted;
extern Uint8	fPOR_Motor_Run_Slow;

extern Uint8	Param_Charger_PWM;
extern Uint8	EnvTempStatus;

extern ST_FLAG  TrayFlags;
extern ST_FLAG	PaperJam;
extern ST_FLAG	TempPaperJam;
extern ST_FLAG 	PaperEmpty;
extern ST_FLAG	TempPaperEmpty;
extern ST_FLAG Flags;
extern ST_FLAG EngineFatalError1;

extern Struct_Fuser_Control Fuser_Control;
extern ST_FLAG CommandReceiveFlag;

extern Uint8	View_Fuser_Cus_String;
extern Uint8	fReadHsync;
extern Uint16	MeasuredPaperLength;
extern Uint16	MeasuredPaperLengthInTray1;
extern Uint16	MeasuredPaperLengthInTray2;
extern Uint16	MeasuredPaperLengthInTray3;
extern Uint8 fTray2IsNotifying;

extern Uint32 	cTotal_Print_Count; /* [KSR032501] for SMC */

extern Uint8	fChangeTargetTemp;
extern Uint32  HeatingStartTemperature;

extern Uint8	fPwsToPrint;
extern Uint8	fPwsToPrint_1stPick;
extern Uint8	fPwsToPrint_2ndPick;
extern Uint8	fPwsToPrint_1stPage;
extern Uint8	fPwsToPrint_2ndPage;
extern Uint8	fPressWakeupKey;
extern Uint8 	fSpeedUpForPWS;
extern int MotorSpeedparam;

extern Uint8	fJamDetacted1st;
extern Uint32	NarrowMediaCheck;
extern Uint32	PaperLengthForJamCheck;
int 		Engine_Parameter[NumberOfEngineParameter];
extern Uint8	PrintPageCmdTemp;
extern Boolean	bePrintFromPowerSave;
extern ST_FLAG IpcFlag1;

extern Uint8	fMotorRunFromPowerSave;
extern Uint8	fState_FullStack;
extern Uint8 	fRequested_Power_Saver;
extern Boolean	EarlyStartCmd;	
extern Boolean 	ColdWakeUpCmd;
extern Uint8	cTray2PaperCheckBeforePickUp;

extern Uint32 tray1_high;
extern Uint32 tray1_low;

void	InitializePageSupervisor(void);
void	ClearAllPageSupervisor(void);
void	ClearOtherPageSupervisor(PageSupervisor *PGSPVSR);

void	PageSupervisors(Uint8);
//void 	(* PageSupervisorFunctbl[])(PageSupervisor *);

void	PS_None(PageSupervisor *);
void	PS_EP_InitializeFor_PreHeating(PageSupervisor *);
void	PS_Dev_On_BeforServoSearch(PageSupervisor *);
void	PS_xfr_On_BeforServoSearch(PageSupervisor *);
void	PS_ServoSearchStart_PreHeating(PageSupervisor *);
void	PS_ServoSearchDone_PreHeating(PageSupervisor *);
void	PS_Dev_On_PreHeating(PageSupervisor *);
void	PS_Dev_Ready_PreHeating(PageSupervisor *);
void	PS_PreHeating_Done(PageSupervisor *);

void	PS_StartPaperPickupSequence_Laminating(PageSupervisor *);
void	PS_FinishPaperPickup_Laminating(PageSupervisor *);
void	PS_DetectStartToPins_Laminating(PageSupervisor *);
void	PS_CheckPinsToPins_Laminating(PageSupervisor *);
void	PS_Finish_Laminating(PageSupervisor *);
void	PS_StartEndProcess_Laminating(PageSupervisor *);
void	PS_End_Laminating(PageSupervisor *);

void	PS_EP_InitializeFor_POR(PageSupervisor *);

void	PS_ChargeToAPC(PageSupervisor *PGSPVSR);
void	PS_ContinueAPC(PageSupervisor *PGSPVSR);
void	PS_Dev_On(PageSupervisor *PGSPVSR);
void	PS_EraseOn(PageSupervisor *PGSPVSR);
void	PS_Xfr_On(PageSupervisor *PGSPVSR);

void 	PS_MotorRun_Start(PageSupervisor *PGSPVSR);
void 	PS_MotorRun_ChargeToAPC(PageSupervisor *PGSPVSR);
void 	PS_MotorRun_ContinueAPC(PageSupervisor *PGSPVSR);
void	PS_MotorRun_Dev_On(PageSupervisor *PGSPVSR);
void 	PS_MotorRun_Xfr_On(PageSupervisor *PGSPVSR);

void 	PS_StartJogMode(PageSupervisor *PGSPVSR);

void	PS_StartServoSequence(PageSupervisor *PGSPVSR);
void	PS_XfrServoStepUp(PageSupervisor *PGSPVSR);
void	PS_XfrServoStepDown(PageSupervisor *PGSPVSR);
void	PS_XfrServoStep1Up(PageSupervisor *PGSPVSR);
void 	PS_Transfer_On(PageSupervisor *PGSPVSR);
void	PS_XfrEndServo(PageSupervisor *PGSPVSR);

void 	PS_PaperCleaning(PageSupervisor *PGSPVSR);
void	PS_PaperCleaningOnPins(PageSupervisor *PGSPVSR);
void	PS_PaperCleaningOnXfr(PageSupervisor *PGSPVSR);

void	PS_MotorWarmup(PageSupervisor *PGSPVSR);
void	PS_HSync_Detact(PageSupervisor *PGSPVSR);
void 	PS_Developer_Off(PageSupervisor *PGSPVSR);
void 	PS_Developer_On(PageSupervisor *PGSPVSR);
void	PS_FinishInitPOR(PageSupervisor *PGSPVSR);
void	PS_FuserWarmupForPOR(PageSupervisor *PGSPVSR);

void	PSP_StartPrintSequence(PageSupervisor *PGSPVSR);
	/* Set Wait Time for Fuser Temperature */
void	PSP_StartPrinter(PageSupervisor *PGSPVSR);
void	PSP_ChargeToAPC(PageSupervisor *PGSPVSR);

void	PSP_StartEPCleaning (PageSupervisor * PGSPVSR);
void	PSP_ChargeCleaning(PageSupervisor *PGSPVSR);
void	PSP_Dev_Off(PageSupervisor *PGSPVSR);
void	PSP_PickAftEPCleaning (PageSupervisor *PGSPVSR);

void	PSP_ContinueAPC(PageSupervisor *PGSPVSR);
void	PSP_Dev_On(PageSupervisor *PGSPVSR);
void	PSP_EraseOn(PageSupervisor *PGSPVSR);
void	PSP_Xfr_On(PageSupervisor *PGSPVSR);

void	PSP_StartServoSequence(PageSupervisor *PGSPVSR);
void	PSP_XfrServoStepUp(PageSupervisor *PGSPVSR);
void	PSP_XfrServoStepDown(PageSupervisor *PGSPVSR);
void	PSP_XfrServoStep1Up(PageSupervisor *PGSPVSR);
void 	PSP_Transfer_On(PageSupervisor *PGSPVSR);
void	PSP_XfrEndServo(PageSupervisor *PGSPVSR);

void	PSP_PaperCleaning(PageSupervisor *PGSPVSR);
void	PSP_PaperCleaningOnPins(PageSupervisor *PGSPVSR);
void	PSP_PaperCleaningOnXfr(PageSupervisor *PGSPVSR);

void	PSP_MotorWarmup(PageSupervisor *PGSPVSR);
void	PSP_MotorWarmupForPwsToPrint(PageSupervisor *PGSPVSR);
void 	PSP_HSync_Detact(PageSupervisor *PGSPVSR);
void 	PSP_Developer_Off(PageSupervisor *PGSPVSR);
void 	PSP_Developer_On(PageSupervisor *PGSPVSR);
void	PSP_FinishInit(PageSupervisor *PGSPVSR);

void	PSP_PickupForContinue(PageSupervisor *PGSPVSR);
void	PSP_StartPaperPickupSequence(PageSupervisor *PGSPVSR);
void	PSP_OptionResync(PageSupervisor *PGSPVSR);
void	PSP_ContinueTray2Pickup(PageSupervisor *PGSPVSR);
void	PSP_FinishPaperPickup(PageSupervisor *PGSPVSR);

void	PSP_BoostCharge(PageSupervisor *PGSPVSR);

void	PSP_DetectStartToPins(PageSupervisor *PGSPVSR);
void	PSP_ToNextPrintReady(PageSupervisor *PGSPVSR);
void	PSP_WaitNextPickup(PageSupervisor *PGSPVSR);

void	PSP_DetectNarrowMedia(PageSupervisor *PGSPVSR);
void	PSP_Dev_OnPrinting(PageSupervisor *PGSPVSR);
void	PSP_EraseOnPrinting(PageSupervisor *PGSPVSR);
void	PSP_Xfr_OnPrinting(PageSupervisor *PGSPVSR);

void	PSP_CheckPinsToPexs(PageSupervisor *PGSPVSR);
void	PSP_CheckPinsToPins(PageSupervisor *PGSPVSR);
void    PSP_FeedJamOutFuser(PageSupervisor *PGSPVSR);  /* VerADS */
void	PSP_WaitPinsOff(PageSupervisor *PGSPVSR);

void	PSP_EndToErase(PageSupervisor *PGSPVSR);
void	PSP_NextVideoPage(PageSupervisor *PGSPVSR);
void	PSP_EndToXfr(PageSupervisor *PGSPVSR);
#if IPG_ENABLE
void 	PSP_SendOptionTrayNotifyBeforeVideoStart(PageSupervisor *PGSPVSR);
void	PSP_VideoOn(PageSupervisor *PGSPVSR);
void	PSP_VideoOff(PageSupervisor *PGSPVSR);
#endif
void	PSP_SelectExitMotorSpeed(PageSupervisor *PGSPVSR);

void	PSP_CheckPexsToPexs(PageSupervisor *PGSPVSR);
void	PSP_EndOfPrintPage(PageSupervisor *PGSPVSR);
void	PSP_StopDuplexBackSide(PageSupervisor *PGSPVSR);
void	PSP_FullStackCheckPoint(PageSupervisor *PGSPVSR);
void	PSP_FullStackOn(PageSupervisor *PGSPVSR);
void	PSP_FullStackOff(PageSupervisor *PGSPVSR);
void	PSP_DuplexCooling(PageSupervisor *PGSPVSR);
void	PSP_StartCooling(PageSupervisor *PGSPVSR);
void 	PSP_WaitCooling(PageSupervisor *PGSPVSR);
void 	PSP_WaitFuserTemp(PageSupervisor *PGSPVSR);
void	PSP_StartDuplexor(PageSupervisor *PGSPVSR);
void	PSP_CheckDis(PageSupervisor *PGSPVSR);
void	PSP_ToDuplexHoldPoint(PageSupervisor *PGSPVSR);
void	PSP_DuplexFrontPrintReady(PageSupervisor *PGSPVSR);

void	PSP_StartExtendedFeed(PageSupervisor *PGSPVSR);
void 	PSP_EmptyCheckWaiting(PageSupervisor *PGSPVSR);
void	PSP_ToHopper(PageSupervisor *PGSPVSR);

void	PS_StartEPCleaning(PageSupervisor *PGSPVSR);
void	PS_ChargeCleaning(PageSupervisor *PGSPVSR);
void	PS_EPCleaning(PageSupervisor *PGSPVSR);
void	PS_Dev_Off(PageSupervisor *PGSPVSR);
void	PS_StopMainMotor(PageSupervisor *PGSPVSR);
void	PS_RunMainMotorForJobFinish(PageSupervisor *PGSPVSR);
void	PS_FinishEPCleaning(PageSupervisor *PGSPVSR);
void	PS_Idle(PageSupervisor *PGSPVSR);
void	PS_StopJogMode(PageSupervisor *PGSPVSR);

void	PS_StopPrinterSequence(PageSupervisor *PGSPVSR);
void	PS_StopSequence(PageSupervisor *PGSPVSR);

void	PS_PrintSequenceError(PageSupervisor *PGSPVSR);
void	PS_ExitError(PageSupervisor *PGSPVSR);
void	PS(PageSupervisor *PGSPVSR);

extern void	Set_Transfer_PWM(Uint8 PWM);

void VideoOffBeforeJamNotify(void);

Uint8	HowManyActivePageSupervisor(void);
Uint8	Set_Laminate_PGSPVSR_Parameter(Uint8 id);
Uint8	Set_POR_PGSPVSR_Parameter(Uint8 id);
Uint32	Get_MeasuredMediaSize(Uint8 Source);
Uint32	Get_MeasuredPaperPath(Uint8 Source, Uint32 T1_Value);
Uint8	Get_PrePageMediaSize(Uint8 Source);

Uint8	Set_PrintPageParameter(Uint8 id);
Uint8	Set_MotorRunPageParameter(void);
Uint8	Get_TopPageSupervisorSequenceID(void);
Uint8 	Get_LastPageSupervisorDestination(void);
Uint32 	GetDarknessConversionRate(Uint32 Level);
Uint8 TrayEmptyCheck(Uint32 tray);
#if DUPLEX_SLOW_MODE
void Set_DuplexPrintTime(int time);
char Get_DuplexPrintTime(int time);
char Clear_DuplexPrintSlowlyFor15min(int time);
#endif

int GetJogModePageCount(Uint32, Uint32);
int GetFanCtrlPageCount(Uint32, Uint32);
Uint8 GetJogModeEnvIdx(Uint8, int);
Uint8 GetFanCtrlEnvIdx(Uint8, int);

Uint8 IsPaperSizeNarrowInTray(Uint8 TraySource);
unsigned char IsPaperLengthNarrow(unsigned int length);
int PickupTempSelect(PageSupervisor *PGSPVSR);
int MotorStartTempSelectForWakeup(void);

PageSupervisor 		PGSupervisor[MAX_PAGE_SUPERVISOR];
//PageSupervisor *	PageSupervisorAddress[MAX_PAGE_SUPERVISOR];
PageSupervisor *	Get_MatchToSequenceID(Uint8 id);
PageSupervisor *	Get_PageSupervisorAddress(Uint8 n);
PageSupervisor *	Get_TopPageSupervisor(void);
PageSupervisor *	Get_LastPageSupervisor(void);
PageSupervisor *    Get_NextPageSupervisor(void);
PageSupervisor *	Get_NextVideoPageSupervisor(void);

void	FinishPageSupervisor(PageSupervisor *PGSPVSR);

//Uint8	IsSourceGood(Uint8 Source);
Uint8	IsSourceGood(PageSupervisor *PGSPVSR);
Uint8	Save_EmptySourceID(void);
Uint8 	fNoCheckTraySourceForPreRun;
Uint8 	fRequest_New_PageInfo;

Uint32 PendingPrintPageData[RIPDATA];

int JogModePageCount;
Uint32 JogModePageTime[JOGMODE_PAGE_MAX];
int FanCtrlPageCount;
Uint32 FanCtrlPageTime[FANCTRL_PAGE_MAX];

int LDPWMAdjustforCopy_Value;
Uint8 LDPWMAdjustforCopy;
Uint8 fPaperLengthIsNarrow;
Uint32 NarrowBoundary;/* [KSR032912] */
Uint32 cPageCompleteCnt;/* [KSR032912] */
#if IPG_ENABLE
Uint8	fVideo_Print_Enable;
Uint8	fReVideoOn;
#endif
Uint8	TopPageSupervisor;
Uint8	EmptyWarningForMPF;
Uint8	EmptyWarningForTray1;
Uint8	EmptyWarningForTray2;
Uint8	EmptyWarningForTray3;
Uint16	CodeOfMachineCheck;
Uint8	fStaticMotorRunTime;/* [KSR032912] */
Uint8	fMotorSpeedMode;
extern Uint8 fMotorSpeedChange;
Uint8 	SelectMotorRunTempForPWStoPrint;

unsigned int Factor_Page[NUM_TONER_FACTOR];/* [KSR032912] */
unsigned int Drum_Factor_Page[NUM_TONER_FACTOR];/* [KSR032912] */

Uint8	PinsIsUsedByOtherPageSupervisor;
Uint8	PexsIsUsedByOtherPageSupervisor;
Uint8	DisIsUsedByOtherPageSupervisor;

Uint32 PageNumVideoDoneFail;
int 	cPinsReleaseTime;
int 	cLastDuplexTime;
Uint8 PreExitSensorValue;
Uint8 ExitSensorValue;

Uint32 cRunMainMotorForJobFinish;
Uint32 cTotalPrintedPageForMotorBWD;
/* Feed Time */
Uint32	PaperFeedTimeFromT1BeforePins;
Uint32	SavedPaperPathT1ToPins;
Uint32	PaperFeedTimeFromT2BeforePins;
Uint32	SavedPaperPathT2ToPins;
Uint32	PaperFeedTimeFromT3BeforePins;
Uint32	SavedPaperPathT3ToPins;
Uint32	PaperFeedTimeFromMPFBeforePins;
Uint32	SavedPaperPathMPFToPins;

Uint8 	fPaperIsEmptyBeforeMotorRun;
Uint8 f1stOptionResync;

Uint32	LeftTimeForPaperComplete;
Uint32	LeftPaperLengthForComplete;

Uint8	DuplexFrontSidePrint;
Uint8	HoldingDuplex;
Uint32	SavedRealMediaSizeInPrePage;
Uint8	SavedSourceInPrePage;

Uint8 fWakeupState;
Uint8 fcancelpage;

Uint8	ChatteringForPinsOn;
Uint8	ChatteringForPexsOn;
Uint8	ChatteringForDisOn;
Uint8	ChatteringForPinsOff;
Uint8	ChatteringForPexsOff;

Uint8 	fPreRunBeforePrintCmd;
Uint8 	fWaitPrintPageCmd;

Uint8   bNarrowMedia;
Uint8	PSTicks_Gap32mm;
Uint8	PSTicks_Gap53mm;	// 14Aug09 : CHG 1 (new)
Uint8	PSTicks_Gap22mm;
Uint8	PSTicks_Gap22mm_L;

Uint8	fPOR_PageSupervisor_EPCleaning;

Uint8	fStartExitMotor;
Uint8	fHoldExitMotor;
Uint8   fStopExitMotor;
Uint8	fClearEmpty;
Uint8	fPOR_PageSupervisor_Act;

Uint8 	f2ndor3rdpickEmpty;

Uint16 	MoveLength;
Uint8	CodeOfJamStatus;
Uint8	cMemoryWrite_Count; /* [KSR032501] for SMC */
Uint8	fMemoryWrite;

Uint8	fFullStack_Error;
Uint8	fFullStackMsgSend;

Uint8	temp_fPaperEmptyForT1;
Uint8	temp_fPaperEmptyForT2;
Uint8	temp_fPaperEmptyForT3;
Uint8	temp_fPaperEmptyForMPF;

Uint8	ChangePrintSequence(Uint8 id);
Uint8	fStartFromPowerSaveMotorRun;	

Uint32	CurrentDarknessLevel;
Uint32 cFreeMotorRunCheck_ForTonerCount;

int EPtimingBUFF_New;
int EPtimingBUFF_Old;
int VideoDoneTime;

Uint32	fPGSPVSRRemoved;
int DrumLifePageCount;
Uint8	fWaitPageCmd;
Uint8 	lastPaperOutTray;
int cPORMotorStartCounter; /* [KSR032912] */
void	Set_PaperMediaSizeInTray(Uint8 Size);

static pthread_t lsu_thread_id;
//static pthread_t page_ctrl_thread_id;
#define PRINT_ENGINE_STACK_SIZE POSIX_MIN_STACK_SIZE
static ALIGN8 UINT8 lsu_stack[PRINT_ENGINE_STACK_SIZE];
mqd_t lsu_ctrl_mq;
//static ALIGN8 UINT8 page_ctrl_stack[PRINT_ENGINE_STACK_SIZE];
//mqd_t page_ctrl_mq;

static void* lsu_ctrl_loop(void* unused);
//static void* page_ctrl_loop(void* unused);
#define ENGINE_FATAL_LASER_FAIL 12 | STATUS_ERROR
#define NUM_PRINT_ENGINE_MESSAGES 4 

int cPORMotorStartCounter;

extern	Boolean PrintPageCmd;	
// #if DARKNESS
// extern TE_ENGINE_CARTRIDGE_DATA CartEPData;		//23Apr09 yjlee
// #endif
extern Uint8	f2ndDrawerState;
extern Uint8	f3ndDrawerState;
extern int 		i2c_dev;
extern Uint32 	cPageCount; /* [KSR032501] for SMC */
extern Uint32 	cDrumPageCount; /* [KSR032912] */

extern int	 cPOR_MotorRunStartTime;
extern Uint8 fPowerOnReset;
extern int Fuser_Setting_Parameters[NumberOfFuserParameter];

extern Uint32	cFullStackErrCnt;

extern int	 cDuplexPageCount;
extern int	 cDuplexSlowPrintCount;
extern Uint8 fDuplexSlowPrint;
extern Uint8 fDuplexSet;
extern Uint8 fDuplexCooling;
extern int	 cKeepMotorRunForCooling;

extern Uint32 ChangeVoltageToTemp(void);
extern engine_state_t engine_state;
extern engine_hardware_t *eng_hardware;
extern int ChargerPWMAdjustSlopeTable[8];
extern int ChargerPWMAdjustOffsetTable[8];
extern int ChargerPWMAdjustSlopeTable_DrumPages[4];
extern int ChargerPWMAdjustOffsetTable_DrumPages[4];
extern int LSUBeamPowerDutyAdjustSlopeTableForServoValue[6];
extern int LSUBeamPowerDutyAdjustOffsetTableForServoValue[6];
extern int LSUBeamPowerDutyAdjustSlopeTableForDLPC[2];
extern int LSUBeamPowerDutyAdjustOffsetTableForDLPC[2];

extern Uint8 fSmartICWrite_DotCount; /* [KSR032501] for SMC */
extern Uint8 fSmartICWrite_DrumDotCount;/* [KSR032912] */
extern Uint8 fSmartICWrite_PageCountInRun;
extern Uint8 fSmartICWrite_DrumPageCountInRun;/* [KSR032912] */
extern Uint8 fSmartICWrite_PageCountFailInRun;
extern Uint8 fSmartICWrite_DrumPageCountFailInRun;/* [KSR032912] */
extern Uint8 fSmartICWrite_DotCountInRun;
extern Uint8 fSmartICWrite_DrumDotCountInRun;/* [KSR032912] */
extern Uint8 fSmartICWrite_DotCountFailInRun;
extern Uint8 fSmartICWrite_DrumDotCountFailInRun;/* [KSR032912] */
extern Uint8 fSmartICWrite_TonerHisFactorRate;/* [KSR032912] */
extern Uint8 fSmartICWrite_DrumHisFactorRate;/* [KSR032912] */

/* For RIP Video Interface */
// Uint32	PaperLength[] = {	/* 112mm/sec, 600dpi */ /* Real paper Length * 10 */
				/*	 8.5" x 14", NotDefine */
	// PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 11", 		279mm	Letter */
	// PL_Letter * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 14", 		356mm	Legal */
	// PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 7.2" x 10.1", 	257mm	B5 */
	// PL_B5 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.3" x 11.7", 	297mm	A4 */
	// PL_A4 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 7.25" x 10.5", 	267mm	Exec */
	// PL_Exec * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 5.83" x 8.27", 	210mm	A5 */
	// PL_A5 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 14", Custom */
	// PL_Custom * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 14", Custom */
	// PL_Unknown * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 7.5", 	190.5mm	envelope */
	// PL_Envelope_75 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 8.9", 	225.4mm	#9 envelope (8.9") */
	// PL_Envelope_9 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 9.5", 	241.3mm	#10 envelope (9.5") */
	// PL_Envelope_10 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 8.66", 	220mm	DL envelope (8.66") */
	// PL_Envelope_DL * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 9.02", 	229mm	C5 envelope (9.02") */
	// PL_Envelope_C5 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 9.84", 	250mm	B5 envelope (9.84") */
	// PL_Envelope_B5 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 14", other envelope */
	// PL_Envelope_Other * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 14", 		Reserved */
	// PL_Reserved * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 14", 		Reserved */
	// PL_Reserved * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 14", 		Reserved */
	// PL_Reserved * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 5.5" x 8.5", 	216mm	 Statement */
	// PL_Statement * Modifier_HeartBeat * MMTimerPerPaperSpeed,
				/*	 8.5" x 13", 		330mm	 Folio */
	// PL_Folio * Modifier_HeartBeat * MMTimerPerPaperSpeed
// } ;


// Uint32	HeartBeatPaperLength[NumberOfBoundary] = 
// { /* Real paper Length * 10 */
	 /*Shorter than Paper Path Capacity */
	// PSTicks_PaperLengthMinimum,
	
	/* 8.5" x 14", Custom, Shorter than 7.5"Env */
	// PL_Envelope_75 / 10 * Modifier_HeartBeat  * MMTimerPerPaperSpeed * 9,
	
	/* 8.5" x 7.5", 	190.5mm	envelope */
	// (PL_A5 + PL_Envelope_75) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 5.83" x 8.27", 	210mm	A5 */
	// (PL_Statement + PL_A5) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 5.5" x 8.5", 	216mm	 Statement */
	// (PL_Envelope_DL + PL_Statement) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,

	/* 8.5" x 8.66", 	220mm	DL envelope (8.66") */
	// (PL_Envelope_9 + PL_Envelope_DL) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 8.5" x 8.9", 	225.4mm	#9 envelope (8.9") */
	// (PL_Envelope_C5 + PL_Envelope_9) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 8.5" x 9.02", 	229mm	C5 envelope (9.02") */
	// (PL_Envelope_10 + PL_Envelope_C5) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 8.5" x 9.5", 	241.3mm	#10 envelope (9.5") */
	// (PL_Envelope_B5 + PL_Envelope_10) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 8.5" x 9.84", 	250mm	B5 envelope (9.84") */
	// (PL_B5 + PL_Envelope_B5) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 7.2" x 10.1", 	257mm	B5 */
	// (PL_Exec + PL_B5) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 7.25" x 10.5", 	267mm	Exec */
	// (PL_Letter + PL_Exec) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 8.5" x 11", 		279mm	Letter */
	// (PL_A4 + PL_Letter) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 8.3" x 11.7", 	297mm	A4 */
	// (PL_Folio + PL_A4) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	
	/* 8.5" x 13", 		330mm	 Folio */
	// (PL_Legal + PL_Folio) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
	/* 8.5" x 14", 		356mm	Legal */
	// PL_Legal / 10 * Modifier_HeartBeat * 11 * MMTimerPerPaperSpeed
// };

// Uint8	MediaSize[NumberOfBoundary + 2] = 
// {					
	// MS_Custom,			/* Shorter than Paper Path Capacity */
	// MS_Custom,			/* 8.5" x 14", Custom, Shorter than 7.5"Env */
	// MS_Envelope_75,		/* 8.5" x 7.5", 	190.5mm	envelope */
	// MS_A5,				/* 5.83" x 8.27", 	210mm	A5 */
	// MS_Statement, 			/* 5.5" x 8.5", 	216mm	 Statement */
	// MS_Envelope_DL,		/* 8.5" x 8.66", 	220mm	DL envelope (8.66") */
	// MS_Envelope_9,		/* 8.5" x 8.9", 	225.4mm	#9 envelope (8.9") */
	// MS_Envelope_C5,		/* 8.5" x 9.02", 	229mm	C5 envelope (9.02") */
	// MS_Envelope_10,		/* 8.5" x 9.5", 	241.3mm	#10 envelope (9.5") */
	// MS_Envelope_B5,		/* 8.5" x 9.84", 	250mm	B5 envelope (9.84") */
	// MS_B5,				/* 7.2" x 10.1", 	257mm	B5 */
	// MS_Exec,			/* 7.25" x 10.5", 	267mm	Exec */
	// MS_Letter,			/* 8.5" x 11", 		279mm	Letter */
	// MS_A4,				/* 8.3" x 11.7", 	297mm	A4 */
	// MS_Folio,			/* 8.5" x 13", 		330mm	 Folio */
	// MS_Legal,			/* 8.5" x 14", 		356mm	Legal */
	// MS_Custom,			/* Longer than Legal */
	// MS_Envelope_Other	/* Longer than Legal */
// } ;

Uint32	PaperLength[2][26] = {	/* 112mm/sec, 600dpi */ /* Real paper Length * 10 */
	{
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_A4 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_A5 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_A5_LEF * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_A6 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_B5 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_B6 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Letter * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Exec * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Statement * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Folio * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Quatro * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_G_Letter * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_G_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Card_100by148 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Card_148by200 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Card_200by148 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Envelope_Monarch * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Envelope_9 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Envelope_10 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Envelope_DL * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Envelope_C5 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Envelope_C6 * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed,
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed
	},
	{
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_A4 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_A5 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_A5_LEF * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_A6 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_B5 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_B6 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Letter * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Exec * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Statement * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Folio * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Quatro * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_G_Letter * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_G_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Card_100by148 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Card_148by200 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Card_200by148 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Envelope_Monarch * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Envelope_9 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Envelope_10 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Envelope_DL * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Envelope_C5 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Envelope_C6 * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed_L,
		PL_Legal * Modifier_HeartBeat * MMTimerPerPaperSpeed_L
	}
} ;


Uint32	HeartBeatPaperLength[2][NumberOfBoundary] = 
{ /* Real paper Length * 10 */
	/* Shorter than Paper Path Capacity */
	{
		PSTicks_PaperLengthMinimum,
		PL_A6 / 10 * Modifier_HeartBeat  * MMTimerPerPaperSpeed * 9,
		(PL_Envelope_C6 + PL_A6) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_B6 + PL_Envelope_C6) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Envelope_Monarch + PL_B6) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Card_148by200 + PL_Envelope_Monarch) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_A5 + PL_Card_148by200) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Statement + PL_A5) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Envelope_DL + PL_Statement) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Envelope_9 + PL_Envelope_DL) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Envelope_C5 + PL_Envelope_9) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Envelope_10 + PL_Envelope_C5) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Quatro + PL_Envelope_10) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_B5 + PL_Quatro) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Exec + PL_B5) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Letter + PL_Exec) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_A4 + PL_Letter) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Folio + PL_A4) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		(PL_Legal + PL_Folio) * Modifier_HeartBeat * MMTimerPerPaperSpeed / 2,
		PL_Legal / 10 * Modifier_HeartBeat * 11 * MMTimerPerPaperSpeed
	},
	{
		PSTicks_PaperLengthMinimum,
		PL_A6 / 10 * Modifier_HeartBeat  * MMTimerPerPaperSpeed_L * 9,
		(PL_Envelope_C6 + PL_A6) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_B6 + PL_Envelope_C6) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Envelope_Monarch + PL_B6) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Card_148by200 + PL_Envelope_Monarch) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_A5 + PL_Card_148by200) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Statement + PL_A5) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Envelope_DL + PL_Statement) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Envelope_9 + PL_Envelope_DL) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Envelope_C5 + PL_Envelope_9) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Envelope_10 + PL_Envelope_C5) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Quatro + PL_Envelope_10) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_B5 + PL_Quatro) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Exec + PL_B5) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Letter + PL_Exec) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_A4 + PL_Letter) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Folio + PL_A4) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		(PL_Legal + PL_Folio) * Modifier_HeartBeat * MMTimerPerPaperSpeed_L / 2,
		PL_Legal / 10 * Modifier_HeartBeat * 11 * MMTimerPerPaperSpeed_L
	}
};
Uint8	MediaSize[NumberOfBoundary + 2] = 
{					
	MS_Custom,			/* Shorter than Paper Path Capacity */
	MS_Custom,			/* 8.5" x 14", Custom, Shorter than 7.5"Env */
	MS_A6,
	MS_Envelope_C6,
	MS_B6,
	MS_Envelope_Monarch,		/* 8.5" x 7.5", 	190.5mm	envelope */
	MS_Card_148by200,
	MS_A5,				/* 5.83" x 8.27", 	210mm	A5 */
	MS_Statement, 			/* 5.5" x 8.5", 	216mm	 Statement */
	MS_Envelope_DL,		/* 8.5" x 8.66", 	220mm	DL envelope (8.66") */
	MS_Envelope_9,		/* 8.5" x 8.9", 	225.4mm	#9 envelope (8.9") */
	MS_Envelope_C5,		/* 8.5" x 9.02", 	229mm	C5 envelope (9.02") */
	MS_Envelope_10,		/* 8.5" x 9.5", 	241.3mm	#10 envelope (9.5") */
	MS_Quatro,
	MS_B5,				/* 7.2" x 10.1", 	257mm	B5 */
	MS_Exec,			/* 7.25" x 10.5", 	267mm	Exec */
	MS_Letter,			/* 8.5" x 11", 		279mm	Letter */
	MS_A4,				/* 8.3" x 11.7", 	297mm	A4 */
	MS_Folio,			/* 8.5" x 13", 		330mm	 Folio */
	MS_Legal,			/* 8.5" x 14", 		356mm	Legal */
	MS_Custom,			/* Longer than Legal */
	MS_Unknown
} ;

/*
void* page_ctrl_loop(void* unused)
{
	while(1)
	{
		if(fWaitPageCmd == TRUE)	
			printer_get_page(50);
		else 	
			posix_sleep_ms(15);
	}
	return 0;
}
*/

void* lsu_ctrl_loop(void* unused)
{
	uint32_t err;
	uint32_t msg;
	
	while(1)
	{
		err = posix_wait_for_message(lsu_ctrl_mq, (char*)&msg, sizeof(msg), POSIX_WAIT_FOREVER);
		XASSERT(err == 0, err);
		
		if(msg == LSU_MIRROR_RUN_AND_SYNC)
		{
			MirrorMotorSpeedChange(0, fMotorSpeedMode); // namja85, 130809
		}
		
		switch(msg)
		{
			MSG("<%d>\n",msg);
			case LSU_MIRROR_RUN:
				slogit( dec_laser_start(eng_hardware->laser));
				break;
			case LSU_MIRROR_STOP:
				fHSync_Detacted = CLEAR;
				MSG("MIRROR STOP\n");
				slogit( dec_stop_laser_mirror(eng_hardware->laser));
				slogit( dec_laser_set_top_sync( eng_hardware->laser, false /* disable hvsync */));
				slogit( dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, false )); // false for off
				slogit( dec_enable_vdo( eng_hardware->laser, false ));	
				break;
			case LSU_MIRROR_RUN_AND_SYNC:
				slogit(     dec_laser_start( eng_hardware->laser ));
				slogit(     dec_laser_set_top_sync( eng_hardware->laser, false ));
				slogit(     err = dec_laser_wait( eng_hardware->laser ) );
				
				if ( err )
				{
					engine_state.engine_fatal_error = ENGINE_FATAL_LASER_FAIL;
					ERR(" laser motor has failed\n");
				}
				else
				{
					fHSync_Detacted = SET;
					fNoFirstHsyncSignal = CLEAR;
					MSG("LASER OK\n",err);
				}
				break;
			// DC Power Key	
			case Power_Saver_mode:
				if((Fuser_Control.EM_State == EMS_Standby) || (Fuser_Control.EM_State == EMS_PowerSave))
				{
					fRequested_Power_Saver = ACTIVE;
				}
				MSG("(eng)Power_Saver_mode\n");
				break;
			
			case Wake_Up:
				if(fWakeup == CLEAR)
				{
					fWakeup = SET;
					EarlyStartCmd = ACTIVE;
#if !ABBA2
					fWakeupState = TRUE;
#endif
					MSG("(eng)Wake_Up\n");
				}
				break;
			
			case Cold_Wake_Up:
				ColdWakeUpCmd = ACTIVE;
				MSG("(eng)Cold_Wake_Up\n");
				break;
			
			/// SP Mode : Hardware Test	
#if HARDWARETEST
			case hardware_test_off:
				sp_mode_hardware_test_init();
				hardware_test.test_off = ON;
				break;
			case Sensor_Test_Start:
				hardware_test.sensor_check = ON;
				Fuser_Control.EM_State = EMS_Diagonostic;
				break;
			case Solenoid_Test_Start:
				hardware_test.solenoid_check = ON;
				Fuser_Control.EM_State = EMS_Diagonostic;
				break;
			case Motor_Test_Start_T1:
				hardware_test.dcmotor_t1_check = ON;
				Fuser_Control.EM_State = EMS_Diagonostic;
				break;
			case Motor_Test_Start_T2:
				hardware_test.dcmotor_t2_check = ON;
				Fuser_Control.EM_State = EMS_Diagonostic;
				break;
			case Motor_Test_Start_T3:
				hardware_test.dcmotor_t3_check = ON;
				Fuser_Control.EM_State = EMS_Diagonostic;
				break;
#endif
		}
	}
	return 0;
}

static PageSupervisor *Picking_PGSPVSR = NULL;

void	InitializePageSupervisor(void)
	/* Clear the parameters to be used for the page supervisor */
{
	Uint32 i;
	for(i=0; i<9; i++) 
	{
		PendingPrintPageData[i] = 0;
	}
	ClearAllPageSupervisor();
	
	TopPageSupervisor = CLEAR;
	/*Warning.byte = NONE */
	
	PSTicks_Gap32mm = Default_PSTicks_Gap32mm;
	PSTicks_Gap53mm = Default_PSTicks_Gap53mm;		// 14Aug09 : CHG 1 (new)
	PSTicks_Gap22mm = Default_PSTicks_Gap22mm;
	PSTicks_Gap22mm_L = Default_PSTicks_Gap22mm_L;
	
	MoveLength = CLEAR;
	CodeOfJamStatus = JAM_IDLE_STATE;

   /* Setup ISR to respond to falling edge on "Pins" ( leading edge of sheet ) */
	Picking_PGSPVSR = NULL;
//   TE_Sensor_SetInterruptHandler( TE_SENSOR_PAPER_INPUT, 0, ISR_PaperInSensor );	// Should be changed (Input Sensor IRS setting)	//23Apr09yjlee
	ChatteringForPinsOn = CLEAR;
	ChatteringForPexsOn = CLEAR;
	ChatteringForPinsOff = CLEAR;
	ChatteringForPexsOff = CLEAR;

	cFullStackErrCnt = 0;
	fFullStack_Error = CLEAR;
	fFullStackMsgSend = CLEAR;
	
	fPOR_PageSupervisor_Act = CLEAR;
	fWaitPageCmd = TRUE;
	
	fWakeupState = FALSE;
	
	posix_create_message_queue(&lsu_ctrl_mq, "/LaserEngQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(uint32_t));
	posix_create_thread(&lsu_thread_id, lsu_ctrl_loop, 0, "lsu_ctrl_task", lsu_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);
	
//	posix_create_message_queue(&page_ctrl_mq, "/PageCtrlQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(uint32_t));
//	posix_create_thread(&page_ctrl_thread_id, page_ctrl_loop, 0, "page_ctrl_task", page_ctrl_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);

	cFreeMotorRunCheck_ForTonerCount = CLEAR;
	cPrintData = CLEAR;
	SelectMotorRunTempForPWStoPrint = CLEAR;
	
	fPaperIsEmptyBeforeMotorRun = CLEAR;
	fNoCheckTraySourceForPreRun = CLEAR;
	fRequest_New_PageInfo = CLEAR;
	cTimerStart_Motor = 0;
	
	fSolenoidOnOff = CLEAR;
	SolenoidOnTimeCount = CLEAR;
	tmp_SolenoidOnTime = CLEAR;
	
	f2ndor3rdpickEmpty = CLEAR;
	f1stOptionResync = CLEAR;
	DrumLifePageCount = CLEAR;
#if DUPLEX_SLOW_MODE
	TopPrintOver300pagesTime = CLEAR;
	cDuplexPrintSlowlyFor15min = CLEAR;
	fDuplexPrintSlowlyFor15min = CLEAR;
	
	for(i = 0; i < DUPLEXPRINTSLOWPAGES; i++)
	{
		cPrintOver300pagesTime[i] = -1;
	}
#endif
	PageNumVideoDoneFail = CLEAR;
#if DUPLEX_SLOW_MODE2
	fDuplexPrintSlowlyForServoHH = CLEAR;
#endif
	EPtimingBUFF_New = 0;
	EPtimingBUFF_Old = 0;
	VideoDoneTime = 0;
	LDPWMAdjustforCopy = CLEAR;
	LDPWMAdjustforCopy_Value = CLEAR;
	cRunMainMotorForJobFinish = CLEAR;
	cTotalPrintedPageForMotorBWD = CLEAR;
	fcancelpage = CLEAR;
	PreExitSensorValue = CLEAR;
	ExitSensorValue = CLEAR;
}

void	(* PageSupervisorFunctbl[])(PageSupervisor *) =
{
        PS_None,                                 /* 0 sequence */
	
	PS_EP_InitializeFor_PreHeating,
	PS_Dev_On_BeforServoSearch,
	PS_xfr_On_BeforServoSearch,
	PS_ServoSearchStart_PreHeating,
	PS_ServoSearchDone_PreHeating,
	PS_Dev_On_PreHeating,
	PS_Dev_Ready_PreHeating,
	PS_PreHeating_Done,
	
	PS_StartPaperPickupSequence_Laminating,
	PS_FinishPaperPickup_Laminating,
	PS_DetectStartToPins_Laminating,
	PS_CheckPinsToPins_Laminating,
	PS_Finish_Laminating,
	PS_StartEndProcess_Laminating,
	PS_End_Laminating,
	
	/* POR EP Initialize Sequence */
	PS_EP_InitializeFor_POR,
	
	/* Core Search for Transfer Bias */
	/* Start EP Timing */
	PS_ChargeToAPC,
	PS_ContinueAPC,
	PS_Dev_On,
	PS_EraseOn,		/* 5sequence */
	PS_Xfr_On,
	
	PS_MotorRun_Start,
	PS_MotorRun_ChargeToAPC,
	PS_MotorRun_ContinueAPC,
	PS_MotorRun_Dev_On,/*10sequence*/
	PS_MotorRun_Xfr_On,	

	PS_StartJogMode,

	/* Servo Search for Transfer Bias */
	PS_StartServoSequence,
	PS_XfrServoStepUp,
	PS_XfrServoStepDown,			/* 15sequence */	
	PS_XfrServoStep1Up,			
	PS_XfrEndServo,	
	
	/* Clear Paper Path For EP Initialize */
	PS_PaperCleaning,
	PS_PaperCleaningOnPins,
	PS_PaperCleaningOnXfr,			/* 20sequence */
	
	/* Syncronization for EP and Motor */
	PS_MotorWarmup,	
	PS_HSync_Detact,
	PS_Developer_Off,
	PS_Developer_On,
	PS_Transfer_On,	
	/* Cleaning Sequence of POR Sequence */
	/* End of Initiaize Sequence */	
	PS_FinishInitPOR,			/* 25sequence */	
	PS_FuserWarmupForPOR,
	
	/* EP Initialize for Print Sequence */
	
/* 1st Print Sequence */
	/* Print Start Sequence */
	PSP_StartPrintSequence,				/* PSPS_StartPrintSequence */ 
	PSP_StartPrinter,					/* PSPS_StartPrinter */


/* 05.07 */
/* Cleaning after 10min continuous printing */
	/* Add '99/June/17 For Interpage Cleaning for charge roll */
	PSP_StartEPCleaning,			
		/* Wait for Starting EP cleaning process */
	PSP_ChargeCleaning,			/* 30sequence*/
		/* Wait for Turning off a Charge bias */
	PSP_Dev_Off,			
		/* Wait for Turning off a Developer bias */
	PSP_PickAftEPCleaning,	
		/* Continue 2 cycles of Chaging roll */

	
	/* EP Initialize for Print Sequence */
	PSP_ChargeToAPC,
	PSP_ContinueAPC,
	PSP_Dev_On,				/* 35sequence */
	PSP_EraseOn,
	PSP_Xfr_On,	
	
	/* Servo Search for Transfer Bias */
	PSP_StartServoSequence,
	PSP_XfrServoStepUp,
	PSP_XfrServoStepDown,			/* 40sequence */
	PSP_XfrServoStep1Up,
	PSP_XfrEndServo,	
	
	/* Clear Paper Path For EP Initialize */
	PSP_PaperCleaning,
	PSP_PaperCleaningOnPins,
	PSP_PaperCleaningOnXfr,			/* 45 sequence */
	
	/* Syncronization for EP and Motor */
	PSP_MotorWarmup,
	PSP_MotorWarmupForPwsToPrint,
	PSP_HSync_Detact,
	PSP_Developer_Off,
	PSP_Developer_On,
	PSP_Transfer_On,	
	PSP_FinishInit,				/* 50sequence */
	
	PSP_PickupForContinue,
	
	/* Paper Pick Up */
	PSP_StartPaperPickupSequence,
	PSP_OptionResync,
	PSP_ContinueTray2Pickup,	
	PSP_FinishPaperPickup,
	
	PSP_BoostCharge,			 /* 55sequence */

	/* Paper Transfer befor Paper Input */
	PSP_DetectStartToPins,			
	PSP_ToNextPrintReady,
	PSP_WaitNextPickup,
	
	/* Timing for Print Process */
	PSP_DetectNarrowMedia, 
	PSP_Dev_OnPrinting,			/* 60sequence */
	PSP_EraseOnPrinting,		
	PSP_Xfr_OnPrinting,
	
	PSP_CheckPinsToPexs,
	
	/* Paper Jam Sensing for Paper Length at Paper Input Sensor */	
	PSP_CheckPinsToPins,
        PSP_FeedJamOutFuser,                    /* VerADS */
	PSP_WaitPinsOff, 			/* 65sequence */
	
#if IPG_ENABLE
	PSP_SendOptionTrayNotifyBeforeVideoStart,
	PSP_VideoOff,
	PSP_VideoOn,
	PSP_NextVideoPage,
	PSP_EndToXfr,	
#else
	PSP_EndToXfr,	
#endif
	PSP_EndToErase,			
	PSP_SelectExitMotorSpeed,
	/* Paper Jam Sensing for Paper Length at Paper Exit Sensor */
	PSP_CheckPexsToPexs,
	
	
/* Duplex Paper Sequence */
	PSP_StopDuplexBackSide,
	PSP_StartDuplexor,			/* 70sequence */
	PSP_CheckDis,	
	PSP_ToDuplexHoldPoint,		
	PSP_DuplexFrontPrintReady,
	
// #if DUPLEX_COOLING	
//#if MAC_TYPE_MH
	PSP_DuplexCooling,
	PSP_StartCooling,
	PSP_WaitCooling,
	PSP_WaitFuserTemp,
//#endif	
	PSP_EndOfPrintPage,
	PSP_FullStackCheckPoint,
	PSP_FullStackOn,
	PSP_FullStackOff,
	
/* Paper Exit Sequence */
	PSP_StartExtendedFeed, 			/* 75sequence */
	PSP_EmptyCheckWaiting,
	PSP_ToHopper,
	
	
/* End of Process Sequence */
	/* EP Cleaning Sequence */
	PS_StartEPCleaning,	
	PS_ChargeCleaning,
	PS_EPCleaning,
	PS_Dev_Off, 				/* 80sequence */
	PS_StopMainMotor,
	PS_RunMainMotorForJobFinish,
	PS_FinishEPCleaning,	
	
	/* Engine Idle State */
	/* Main Motor Off, Mirror Motor On, Fuser On */
	PS_Idle,			
	PS_StopJogMode,				/* 85sequence */
	
	/* Stop Printer Sequence */
	PS_StopPrinterSequence,
	PS_StopSequence,	
	
/* Print Sequence Error */
	PS_PrintSequenceError,
	PS_ExitError,
	PS				
} ;

Uint8 Number;
void	PageSupervisors(Uint8 time)
	/* To manage the printing sequence */
{
	PageSupervisor *now_PGSPVSR;

	for(Number=0; Number < MAX_PAGE_SUPERVISOR; Number++) 
	{
		now_PGSPVSR = Get_PageSupervisorAddress(Number);
		if(now_PGSPVSR->Active) 
		{
			now_PGSPVSR->T1_Value = now_PGSPVSR->T1_Value + time;
			if(now_PGSPVSR->T1_Value > now_PGSPVSR->T1_Comp)	now_PGSPVSR->T1Delay = now_PGSPVSR->T1_Value - now_PGSPVSR->T1_Comp;
			else 												now_PGSPVSR->T1Delay = 0;	
			if(now_PGSPVSR->T2_Value >= time){
				now_PGSPVSR->T2_Value = now_PGSPVSR->T2_Value - time;
				now_PGSPVSR->T2Delay = 0;
			}else{
				now_PGSPVSR->T2Delay = time - now_PGSPVSR->T2_Value;
				now_PGSPVSR->T2_Value = 0;				
			}
			(* PageSupervisorFunctbl[now_PGSPVSR->T2_State])(now_PGSPVSR);
			(* PageSupervisorFunctbl[now_PGSPVSR->SequenceID])(now_PGSPVSR);
		}
		else 
		{
			if(Number == NONE) 
			{
				break;
			}
			else if(Number == ONE) 
			{
				now_PGSPVSR = &PGSupervisor[TopPageSupervisor];
				if(now_PGSPVSR->Flags == PSS_StopSequence) 
				{			
					FinishPageSupervisor(now_PGSPVSR);
				}
				break;
			}
			else 
			{
				now_PGSPVSR = &PGSupervisor[TopPageSupervisor];
				switch(now_PGSPVSR->Flags) 
				{
					case PSPS_ToHopper:
					case PSS_StopMainMotor:
					case PSS_StopSequence:
						FinishPageSupervisor(now_PGSPVSR);
						break;
					default:
						break;
				}
			}		
		}
	}
}

void	PS_None(PageSupervisor *PGSPVSR)
{
	/*PGSPVSR->Flags = PSS_NONE;*/
}

void	PS_EP_InitializeFor_PreHeating(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSL_EP_InitializeFor_PreHeating;

#if	ENG_TRACE		
    MSG("(eng)PS_EP_InitializeFor_PreHeating <%d>\n", cProcessCounter);
#endif	
	
	MotorStartForLaminating = SET;
	StartMainMotor();
	StartEP_Laminating();
	
	_Set_T1(PSTicks_Dev_NoPrint_PreHeating, PSL_Dev_On_PreHeating)
	_Set_PS_SequenceID_Change(PSL_Dev_On_PreHeating)
}

void	PS_Dev_On_BeforServoSearch(PageSupervisor *PGSPVSR)
{
	if((cChargerCnt < Num_ChargerTable) && ((cProcessCounter - cTempProcessCnt) >= 2))
	{
		//MSG("(eng)Set_Charger_PWM () = %d (%d)\n", EP_ChargerTable[cChargerCnt], cProcessCounter);
		Set_Charge_PWM(EP_ChargerTable[cChargerCnt]);
		cChargerCnt++;
		cTempProcessCnt = cProcessCounter;
	}
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_Dev_On_BeforServoSearch;

#if	ENG_TRACE		
    MSG("(eng)PS_Dev_On_BeforServoSearch <%d>\n", cProcessCounter);
#endif

	StartDeveloperDefault();
	
	_Set_T1(PSTicks_DevOnToXfrServo, PSL_xfr_On_BeforServoSearch)
	_Set_PS_SequenceID_Change(PSL_xfr_On_BeforServoSearch)
}

void	PS_xfr_On_BeforServoSearch(PageSupervisor *PGSPVSR)
{
	if(cDeveloperCnt < Num_DeveloperTable)
	{
//		MSG("(eng)Set_Developer_PWM = %d (%d)\n", EP_DeveloperTable[cDeveloperCnt], cProcessCounter);
		Set_Developer_PWM(EP_DeveloperTable[cDeveloperCnt]);
		cDeveloperCnt++;
	}
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_xfr_On_BeforServoSearch;

#if	ENG_TRACE		
    MSG("(eng)PS_xfr_On_BeforServoSearch <%d>\n", cProcessCounter);
#endif
	
	_Set_T1(PSTicks_Minimum, PSL_ServoSearchDone_PreHeating)
	_Set_PS_SequenceID_Change(PSL_ServoSearchDone_PreHeating)
	_Set_T2(PSTicks_Minimum, PSS_StartServoSequence)
}

void	PS_ServoSearchStart_PreHeating(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSL_ServoSearchStart_PreHeating;

#if	ENG_TRACE		
    MSG("(eng)PS_ServoSearchStart_PreHeating <%d>\n", cProcessCounter);
#endif
	StartMainMotor();
	StartEP_PrintFromStandby();
	
	_Set_T1(PSTicks_ChargeToDevOnForPrint, PSL_Dev_On_BeforServoSearch)
	_Set_PS_SequenceID_Change(PSL_Dev_On_BeforServoSearch)
}

void	PS_ServoSearchDone_PreHeating(PageSupervisor *PGSPVSR)
{
	if(PGSPVSR->T2_State == PSS_NONE) 
	{
		PGSPVSR->Flags = PSL_ServoSearchDone_PreHeating;

#if	ENG_TRACE		
	    MSG("(eng)PS_ServoSearchDone_PreHeating <%d>\n", cProcessCounter);
#endif	
		_Set_T1(0, PSS_NONE)
		_Set_PS_SequenceID_Change(PSS_NONE)
	}
}

void	PS_Dev_On_PreHeating(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_Dev_On_PreHeating;

#if	ENG_TRACE		
    MSG("(eng)PS_Dev_On_PreHeating <%d>\n", cProcessCounter);
#endif	

	StartDeveloperLaminating();
	StartTransferCleaning();
	
	_Set_T1(PSTicks_Txf_Clean_PreHeating, PSL_Dev_Ready_PreHeating)
	_Set_PS_SequenceID_Change(PSL_Dev_Ready_PreHeating)
}

void	PS_Dev_Ready_PreHeating(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_Dev_Ready_PreHeating;

#if	ENG_TRACE		
    MSG("(eng)PS_Dev_Ready_PreHeating <%d>\n", cProcessCounter);
#endif	
	
	if(LaminatingMode == SET)
	{
		fRetryPickUp = CLEAR;
		fLastPickUp = CLEAR;
		StartDeveloperLaminating();
		Set_TransferBiasInterpageGap_Laminating();
		_Set_T1(GetPreheatingTime_Laminate(lami_condition) - 200, PSL_StartPaperPickupSequence_Laminating)
		_Set_PS_SequenceID_Change(PSL_StartPaperPickupSequence_Laminating)
	}
	else
	{
		StartDeveloperLaminating();
		Set_TransferBiasInterpageGap_Laminating();
		
		_Set_T1(PreHeatingDoneTime, PSL_PreHeating_Done)
		_Set_PS_SequenceID_Change(PSL_PreHeating_Done)
	}
}

void	PS_PreHeating_Done(PageSupervisor *PGSPVSR)
{
	if(cDeveloperCnt < Num_DeveloperTable)
	{
//		MSG("(eng)Set_Developer_PWM = %d (%d)\n", EP_DeveloperTable[cDeveloperCnt], cProcessCounter);
		Set_Developer_PWM(EP_DeveloperTable[cDeveloperCnt]);
		cDeveloperCnt++;
	}
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_PreHeating_Done;

#if	ENG_TRACE		
    MSG("(eng)PS_PreHeating_Done <%d>\n", cProcessCounter);
#endif	
	
	_Set_T1(PSTicks_Minimum, PSL_StartEndProcess_Laminating)
	_Set_PS_SequenceID_Change(PSL_StartEndProcess_Laminating)
}

void	PS_StartPaperPickupSequence_Laminating(PageSupervisor *PGSPVSR)
{
	if(ResumeLamination == SET)
	{
		_Set_T1(PSTicks_Minimum, PSL_Finish_Laminating)
		_Set_PS_SequenceID_Change(PSL_Finish_Laminating)
		
		return;
	}
	
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_StartPaperPickupSequence_Laminating;

	Fuser_Control.RequestState = ST_PRINT;
	
	if(((Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP_LAMINATE) <= Fuser_Control.Current_Temperature)
	&& ((Fuser_Control.Target_Temperature + FUSER_PARAM_PICKUP_TMP_LAMINATE) >= Fuser_Control.Current_Temperature)
	&& (PGSPVSR->Source == MS_MultiPurposeTray))
	{
		LaminatingSendMsgToPM(STATUS_LAMINATEINFO_LAMINATING);
		MSG("(Eng)$$ Paper Pickup (Laminate) : %d Source = %x, Cur_Temp = %d, Target_Temp = %d $$\n", cProcessCounter, PGSPVSR->Source, Fuser_Control.Current_Temperature,  Fuser_Control.Target_Temperature);
		
		if(f2ndor3rdpickEmpty == CLEAR)
		{
			TurnOnMPFSolenoid();
		}
		f2ndor3rdpickEmpty = CLEAR;
		
		_Set_T1(PSTicks_SolenoidOnTime, PSL_FinishPaperPickup_Laminating)
		_Set_PS_SequenceID_Change(PSL_FinishPaperPickup_Laminating)
	}
}

void	PS_FinishPaperPickup_Laminating(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_FinishPaperPickup_Laminating;

#if	ENG_TRACE		
    MSG("(eng)PS_FinishPaperPickup_Laminating <%d>\n", cProcessCounter);
#endif	
	
	TurnOffMPFSolenoid();
	_AddTime_T1(PSLimit_TrayToPins_LAMINATE, PSL_DetectStartToPins_Laminating)
	_Set_PS_SequenceID_Change(PSL_DetectStartToPins_Laminating)
}

void	PS_DetectStartToPins_Laminating(PageSupervisor *PGSPVSR)
{
	if(IsPaperPresentOnPins() == TRUE) 
	{
		if(ChatteringForPinsOn < CHATTERING_TIME_PINS) 
		{
			ChatteringForPinsOn++;
			return;
		}
		ChatteringForPinsOn = CLEAR;

#if	ENG_TRACE			
		MSG("(Eng)++Input Sensor_Detacted (Laminate) = %d ++\n", cProcessCounter);
#endif
		PGSPVSR->InputDetectTime = cProcessCounter;

		PGSPVSR->Flags = PSL_DetectStartToPins_Laminating;

		fRetryPickUp = CLEAR;
		fLastPickUp = CLEAR;			
		
		_Set_T1(PSTicks_20s, PSL_CheckPinsToPins_Laminating)	
		_Set_PS_SequenceID_Change(PSL_CheckPinsToPins_Laminating)
		return;
	}
	ChatteringForPinsOn = CLEAR;
	_Check_T1_and_RET
	/* Paper Jam Error */
#if	ENG_TRACE	
	MSG("(Eng)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  Paper is not detacted on Input Sensor (Laminate) - Paper Jam case  +\n");
	MSG("(Eng)+  cProcessCounter = %d (PageID = %d)\n",cProcessCounter, PGSPVSR->PageID);
#endif
		
	if(fRetryPickUp == CLEAR)
	{
		fRetryPickUp = SET;
		_Set_PS_SequenceID_Change(PSL_StartPaperPickupSequence_Laminating)
		return;
	}
	else
	{
		if(fLastPickUp == CLEAR)
		{
			fLastPickUp = SET;
			_Set_PS_SequenceID_Change(PSL_StartPaperPickupSequence_Laminating)
			return;
		}
	}
	
	JamCode_Laminating = JAM_INPUT_DETECT_LAMINATING;
	
	_Set_T1(0, PSL_Finish_Laminating)
	_Set_PS_SequenceID_Change(PSL_Finish_Laminating)
}

void	PS_CheckPinsToPins_Laminating(PageSupervisor *PGSPVSR)
{
	ExitSensorValue = IsPaperPresentOnPins();
	
	if(PreExitSensorValue != ExitSensorValue)
	{
		ChatteringForPinsOff = CLEAR;
	}
	PreExitSensorValue = ExitSensorValue;
	
	if(ChatteringForPinsOff < CHATTERING_TIME) 
	{
		ChatteringForPinsOff++;
		return;
	}
	
	if(ExitSensorValue == FALSE)
	{
		PGSPVSR->Flags = PSL_CheckPinsToPins_Laminating;
#if	ENG_TRACE			
		MSG("(Eng)++Complete Lamination = %d ++\n", cProcessCounter);
#endif
		PGSPVSR->InputReleaseTime = cProcessCounter;
		
		lami_condition = CON_MTR_STOP_LAMI2;
		_Set_T1(PSTicks_6Hs - PSTicks_CleanTxf_Laminating, PSL_Finish_Laminating)
		_Set_PS_SequenceID_Change(PSL_Finish_Laminating)
	}
	
	_Check_T1_and_RET
	
#if	ENG_TRACE		
	MSG("(Eng)+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  Paper is not  released on Input Sensor (Laminate) - Paper Jam case  +\n");
	MSG("(Eng)+  cProcessCounter = %d (PageID = %d)\n",cProcessCounter, PGSPVSR->PageID);
#endif	

	JamCode_Laminating = JAM_INPUT_RELEASE_LAMINATING;
	
	_Set_T1(0, PSL_Finish_Laminating)
	_Set_PS_SequenceID_Change(PSL_Finish_Laminating)

	LaminatingMode = CLEAR;
	
	return;
}

void	PS_Finish_Laminating(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_Finish_Laminating;
	
	StartTransferCleaning();
//	StopMainMotor();
	
	MSG("(eng)PS_Finish_Laminating <%d>\n", cProcessCounter);
	
	_Set_T1(PSTicks_CleanTxf_Laminating, PSL_StartEndProcess_Laminating)
	_Set_PS_SequenceID_Change(PSL_StartEndProcess_Laminating)
		
	return;
}

void	PS_StartEndProcess_Laminating(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_StartEndProcess_Laminating;
	
#if	ENG_TRACE		
    MSG("(eng)PS_StartEndProcess_Laminating <%d>\n", cProcessCounter);
#endif	
	StartDeveloperNoPrinting();
	Set_TransferBiasInterpageGap_Laminating();
	
	_Set_T1(PSTicks_500ms, PSL_End_Laminating)	
	_Set_PS_SequenceID_Change(PSL_End_Laminating)
	return;
}

void	PS_End_Laminating(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSL_End_Laminating;
	
#if	ENG_TRACE		
    MSG("(eng)PS_End_Laminating <%d>\n", cProcessCounter);
#endif	
	FinishLaminationProcess();	
	StopMainMotor();
	
	_Set_T1(0, PSS_NONE)
	_Set_PS_SequenceID_Change(PSS_NONE)
	return;
}

/* ==========  POR EP Initialize Sequence  ========== */
/* Start Engine Manager POR and EP Warmup Sequence */
void	PS_EP_InitializeFor_POR(PageSupervisor *PGSPVSR)
	/* Motors and Charge Bias turn on and start EP initializer */
{
	PGSPVSR->Flags = PSS_EP_InitializeFor_POR;

	//16Apr09 yjlee 
	//if(Set_MainMotorRunningNotify() == TRUE) 
	if(1/*Set_MainMotorRunningNotify() == TRUE*/) 
	{
#if	ENG_TRACE		
	    MSG("(%d)PS_EP_InitializeFor_POR <%d>\n",PGSPVSR->PageID, cProcessCounter);
#endif	
		EPtimingBUFF_Old = cProcessCounter;
		EPtimingBUFF_New = cProcessCounter;
		cPORMotorStartCounter = cProcessCounter;
		StartMainMotor();
		StartMirrorMotor();
		fStaticMotorRunTime = SET;
		Fuser_Control.fPOR_PageSupervisor_Active = CLEAR;
		StartEP_POR();
/*		if(f2ndDrawerState == SET) 
		{
//			fTray2MotorRun = TRUE;
			Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS); // add by namja
		}
		if(f3ndDrawerState == SET) 
		{
//			fTray3MotorRun = TRUE;
			Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORRUNSPEED_KARAS); // add by namja
		}*/
//08May09 yjlee
/*
		_Set_T1(PSTicks_ChargeToLD, PSS_ChargeToAPC)
		_Set_PS_SequenceID_Change(PSS_ChargeToAPC)
*/
		if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T1(PSTicks_ChargeToDevOn_L, PSS_Dev_On)}
		else 										{_Set_T1(PSTicks_ChargeToDevOn, PSS_Dev_On)}
		_Set_PS_SequenceID_Change(PSS_Dev_On)
	}	
}

void	PS_ChargeToAPC(PageSupervisor *PGSPVSR)					/* PSS_ChargeToAPC */
	/* Starting Laser APC */
{
	_Check_T1_and_RET

	_Set_T1(PSTicks_ContinueAPC, PSS_ContinueAPC)
	_Set_PS_SequenceID_Change(PSS_ContinueAPC)
	
}

void	PS_ContinueAPC(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET

	_Set_T1(PSTicks_APC_EndToDev+2 + PSTicks_XMotorLockUp, PSS_Dev_On)
	_Set_PS_SequenceID_Change(PSS_Dev_On)		
}

void	PS_Dev_On(PageSupervisor *PGSPVSR)
{
	if((cChargerCnt < Num_ChargerTable) && ((cProcessCounter - cTempProcessCnt) >= 2))
	{
		//MSG("(eng)Set_Charger_PWM () = %d (%d)\n", EP_ChargerTable[cChargerCnt], cProcessCounter);
		Set_Charge_PWM(EP_ChargerTable[cChargerCnt]);
		cChargerCnt++;
		cTempProcessCnt = cProcessCounter;
	}
	_Check_T1_and_RET
	
	StartDeveloperDefault();
#if	ENG_TRACE		
	MSG("(Eng)PS_Dev_On <%d>\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T10 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T1(PSTicks_DevOnToServoPOR_L, PSS_Xfr_On)}
	else 										{_Set_T1(PSTicks_DevOnToServoPOR, PSS_Xfr_On)}
	_Set_PS_SequenceID_Change(PSS_Xfr_On)	
}

void	PS_Xfr_On(PageSupervisor *PGSPVSR)
{
	if(cDeveloperCnt < Num_DeveloperTable)
	{
		//MSG("(eng)Set_Developer_PWM = %d (%d)\n", EP_DeveloperTable[cDeveloperCnt], cProcessCounter);
		Set_Developer_PWM(EP_DeveloperTable[cDeveloperCnt]);
		cDeveloperCnt++;
	}
	_Check_T1_and_RET
		
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}

#if	ENG_TRACE		
	MSG("(Eng)PS_Xfr_On <%d>\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T11 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	_Set_T2(PSTicks_Minimum, PSS_StartServoSequence)
	_Set_T1(PSTicks_XfrToErase /* - overtime */, PSS_EraseOn)
		/* Limit time for which PLL signal of motor comes Lock */
	_Set_PS_SequenceID_Change(PSS_EraseOn)
}

void	PS_EraseOn(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET	
	
	_Set_T1(PSTicks_MotorLock /* - overtime */, PSS_MotorWarmup)
	_Set_PS_SequenceID_Change(PSS_MotorWarmup)
}

void 	PS_MotorRun_Start(PageSupervisor *PGSPVSR)
{
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
	
	PGSPVSR->Flags = PSS_MotorRun_Start;
	
	if(1/*Set_MainMotorRunningNotify() == TRUE*/) 
	{
		StartMainMotor();
		if ( PrintPageCmd )
		{
			StartMirrorMotor();
		}
		StartEP_POR();
		_Set_T1(PSTicks_ChargeToLD, PSS_MotorRun_ChargeToAPC)
		_Set_PS_SequenceID_Change(PSS_MotorRun_ChargeToAPC)
	}
}

void 	PS_MotorRun_ChargeToAPC(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	/* Auto Power Control */
	_Set_T1(PSTicks_ContinueAPC, PSS_MotorRun_ContinueAPC)
	_Set_PS_SequenceID_Change(PSS_MotorRun_ContinueAPC)
	
}

void 	PS_MotorRun_ContinueAPC(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET

	_Set_T1(PSTicks_APC_EndToDev + 1, PSS_MotorRun_Dev_On)
	_Set_PS_SequenceID_Change(PSS_MotorRun_Dev_On)		
}

void	PS_MotorRun_Dev_On(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET

	 Set_DeveloperBiasForNoPrint();

	_Set_T1(PSTicks_DevToXfr, PSS_MotorRun_Xfr_On)
	_Set_PS_SequenceID_Change(PSS_MotorRun_Xfr_On)
}

void 	PS_MotorRun_Xfr_On(PageSupervisor *PGSPVSR)
{
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
	Set_TransferBiasInterpageGap(FALSE);		
	FinishPageSupervisor( PGSPVSR );
}

void 	PS_StartJogMode(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSS_StartJogMode;
	
	if(/*Set_MainMotorRunningNotify() == TRUE*/1) 			//24Apr09 yjlee
	{
		StartMainMotor();
		StartEP_JOG();
#if MAC_TYPE_MH
		_Set_T1(PSTicks_JogMode_RunTime_H, PSS_StopMainMotor)
#else
		_Set_T1(PSTicks_JogMode_RunTime_L, PSS_StopMainMotor)
#endif
		_Set_PS_SequenceID_Change(PSS_StopMainMotor)
	}
	
}

void	PS_StartServoSequence(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET

	StartServoSearch();
	_Set_T2(PSTicks_ServoStep200ms, PSS_XfrServoStepUp)	
}

void	PS_XfrServoStepUp(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET

	if(ServoSearchUpProcedure(SERVO_UP_STEP) == TRUE) 
	{
		_Set_T2(PSTicks_Minimum, PSS_XfrServoStepDown)
	}
	else 
	{
		_Set_T2(PSTicks_ServoStep20ms, PSS_XfrServoStepUp)
	}
}

void	PS_XfrServoStepDown(PageSupervisor *PGSPVSR)
{
	ServoSearchDownProcedure(SERVO_DOWN_STEP);
	_Set_T2(PSTicks_ServoStep20ms, PSS_XfrServoStep1Up)
	
}

void	PS_XfrServoStep1Up(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET

	if(ServoSearchUpProcedure(SERVO_ONE_UP_STEP) == TRUE) 
	{		
		_Set_T2(PSTicks_Minimum, PSS_XfrEndServo)		
	}
	else 
	{
		_Set_T2(PSTicks_ServoStep20ms, PSS_XfrServoStep1Up)
	}
}

void	PS_XfrEndServo(PageSupervisor *PGSPVSR)
{
	if(RunEngineForLamination == CLEAR)
	{
		if(Detect_OverSensorAtWarmup()) 
		{
			if(fJamDetacted1st == TRUE)
			{
				return;
			}
			_Set_PS_SequenceID_Change(PSS_PrinterError)
		}
	}
	_Set_T2(NONE, PSS_NONE)
}

void 	PS_PaperCleaning(PageSupervisor *PGSPVSR)
{
	if(IsPaperPresentOnPexs() == PRESENT)
	{
		PGSPVSR->T2_State = PSS_PaperCleaning;
		_Set_T2(PSLimit_Legal - PSLimit_PinsToPexs, PSS_PaperCleaningOnPins)
		return;
	}

	if(fPOR_PageSupervisor_Act) 
	{
		_Check_T1_and_RET
	}
	else 
	{
		_Check_T2_and_RET
	}
	PaperJamOnPins = JAM;
	_Set_PS_SequenceID_Change(PSS_PrinterError)
}

void	PS_PaperCleaningOnPins(PageSupervisor *PGSPVSR)
{
	if(IsPaperPresentOnPins() == NOT_PRESENT) 
	{
		PGSPVSR->T2_State = PSS_PaperCleaningOnPins;
		_Set_T2(PSTicks_PinsToXfr + PSTicks_1s, PSS_PaperCleaningOnXfr)
		return;
	}
	if(fPOR_PageSupervisor_Act)
	{
		_Check_T1_and_RET
	}
	else 
	{
	_Check_T2_and_RET
	}
	/* Paper Jam between pins and pexs */
	PaperJamBetweenPinsAndPexs = JAM;
	_Set_PS_SequenceID_Change(PSS_PrinterError)
}

void	PS_PaperCleaningOnXfr(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET

	PGSPVSR->T2_State = PSS_PaperCleaningOnXfr;
	_Set_T2(PSTicks_Minimum, PSS_StartServoSequence)
}

void	PS_MotorWarmup(PageSupervisor *PGSPVSR)
{
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}

	if(PGSPVSR->T1_Value < PGSPVSR->T1_Comp) 
	{
		if(Get_MainMotorLockState() == UNLOCK)
			return;

		if(Get_MirrorMotorLockState() == UNLOCK)
			return;		
	}
	else
	{
		if(Get_MainMotorLockState() == UNLOCK) 
		{
			fPaperFeedMotorFailure = ERROR;
			CodeOfMachineCheck = NO_MAIN_MOTOR_LOCK;
		}		
		if(Get_MirrorMotorLockState() == UNLOCK) 
		{
			fMirrorMotorFailure = ERROR;
			CodeOfMachineCheck = NO_MIRROR_MOTOR_LOCK;
		}
	}
	if ( PGSPVSR->T2_State != PSS_NONE )
		return;

#if	ENG_TRACE	
	MSG("(Eng)PS_MotorWarmup : LD Enable <%d>\n",cProcessCounter);
#endif		
	Set_LSUOnNotify();

	_Set_T1(PSTicks_5s, PSS_HSync_Detact)
	_Set_PS_SequenceID_Change(PSS_HSync_Detact)
}

void PS_HSync_Detact(PageSupervisor *PGSPVSR)
{
	if(PGSPVSR->T1_Value < PGSPVSR->T1_Comp)
	{
		if(fHSync_Detacted == CLEAR)
		{				
			return;
		}
	}
	else
	{
		fNoFirstHsyncSignal = SET;
		CodeOfMachineCheck = PRINTHEAD_LOST_HSYNC;
	}
#if	ENG_TRACE	
	MSG("(Eng)PS_HSync_Detact <%d>\n",cProcessCounter);
#endif
//	fHSync_Detacted = CLEAR;
	StopMirrorMotor();
	
#if	ENG_TRACE	
	MSG("(Eng)PS_Developer_Off <%d>\n",cProcessCounter);
#endif	
	EPtimingBUFF_Old = cProcessCounter;
	EPtimingBUFF_New = cProcessCounter;
	
	StartDeveloperNoPrinting();
	StartTransferCleaning();
	
	if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T1(PSTicks_DeveloperNoPrintingLength_L, PSS_Developer_On)}
	else 										{_Set_T1(PSTicks_DeveloperNoPrintingLength, PSS_Developer_On)}
	_Set_PS_SequenceID_Change(PSS_Developer_On)	
	
	// _Set_T1(PSTicks_LDEnToDev - PSTicks_HSync, PSS_Developer_Off)
//	_Set_T1(PSTicks_Minimum, PSS_Developer_Off)
//	_Set_PS_SequenceID_Change(PSS_Developer_Off)
}

void PS_Developer_Off(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
#if	ENG_TRACE	
	MSG("(Eng)PS_Developer_Off <%d>\n",cProcessCounter);
#endif	
	StartDeveloperNoPrinting();
	StartTransferCleaning();
	
	if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T1(PSTicks_DeveloperNoPrintingLength_L, PSS_Developer_On)}
	else 										{_Set_T1(PSTicks_DeveloperNoPrintingLength, PSS_Developer_On)}
		
	_Set_PS_SequenceID_Change(PSS_Developer_On)	
}

void PS_Developer_On(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
#if	ENG_TRACE	
	MSG("(Eng)PS_Developer_On <%d>\n",cProcessCounter);
#endif	
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T12 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	Set_DeveloperBiasForPrinting(PGSPVSR->Darkness, PGSPVSR->TonerSaver);
	PGSPVSR->T1_State = PSS_FinishInitPOR;
	_Set_PS_SequenceID_Change(PSS_FinishInitPOR)

	if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T2(PSTicks_DevOnToXfrCleaning_L, PSS_Transfer_On)}
	else 										{_Set_T2(PSTicks_DevOnToXfrCleaning, PSS_Transfer_On)}
}

void	PS_Transfer_On(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
#if	ENG_TRACE	
	MSG("(Eng)PS_Transfer_On <%d>\n",cProcessCounter);
#endif	
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T13 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	Set_TransferBiasInterpageGap(FALSE);
	_Set_T2(NONE, PSS_NONE)
}

void	PS_FinishInitPOR(PageSupervisor *PGSPVSR)
	/* Syncronize for EP state and Motor Warm Up */
{	
	if(PGSPVSR->SequenceID == PSS_FinishInitPOR
	&& PGSPVSR->T2_State == PSS_NONE) 
	{
#if	ENG_TRACE		
		MSG("(Eng)PS_FinishInitPOR\n");
#endif		
		_Set_PS_SequenceID_Change(PSS_FuserWarmupForPOR)	
	}	
}

/* Wait For Starting Cleaning Sequence of POR Sequence */
void	PS_FuserWarmupForPOR(PageSupervisor *PGSPVSR)
{
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
	
	PGSPVSR->Flags = PSS_FuserWarmupForPOR;
//	if((Fuser_Control.State == ST_STANDBY) ||
//		(Fuser_Control.State >= ST_PRINT))
	if(1)
	{
#if	ENG_TRACE	
		MSG("(Eng)PS_FuserWarmupForPOR - Set_PrintReadyNotify(); (%d)\n", cProcessCounter);
#endif
		if(fPowerOnReset != SET)
		{
			Set_PrintReadyNotify();
		}
		
		if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T1(PSTicks_WaitCleaning_L, PSPS_ToHopper)}
		else 										{_Set_T1(PSTicks_WaitCleaning, PSPS_ToHopper)}

		_Set_PS_SequenceID_Change(PSPS_ToHopper)
	}
	/* For Fuser Error detection & No Action during Print Process */
	/* don't reach fuser standby state during 120sec */
	else if (Fuser_Control.State == ST_OFF) 
	{	
		/* If error has occured, the sequence goes to cleaning mode */
#if	ENG_TRACE
		MSG("(Eng)PS_FuserWarmupForPOR - to PSPS_ToHopper\n");
#endif		
		_Set_T1(PSTicks_Minimum, PSPS_ToHopper)
		_Set_PS_SequenceID_Change(PSPS_ToHopper)
	} 
}

// #if DUPLEX_COOLING
//#if MAC_TYPE_MH
void	PSP_DuplexCooling(PageSupervisor *PGSPVSR)
{
	MSG("$$$ WaitCooling Start : cProcessCounter = %d $$$\n",cProcessCounter);
	
	_Set_T1(PSTicks_WaitCooling, PSPS_StartCooling)
	_Set_PS_SequenceID_Change(PSPS_StartCooling)
}

void	PSP_StartCooling(PageSupervisor *PGSPVSR)
{
	Uint32 coolingTime = Engine_Parameter[13]; //-jrs set default to avoid compiler error in release build
	_Check_T1_and_RET
	
	Set_CoolingNotify(SET);
	StopMainMotor();
	MSG("StopMainMotor 3\n");
	Fuser_Control.Target_Temperature = Temp_033C;
	
	switch(EnvTempStatus)
	{
		case EnvTemp_H:
			coolingTime = Engine_Parameter[13];
			break;
		case EnvTemp_HM:
			coolingTime = Engine_Parameter[14];
			break;
		case EnvTemp_M:
			coolingTime = Engine_Parameter[15];
			break;
		case EnvTemp_L:
			coolingTime = Engine_Parameter[16];
			break;
	}
#if	ENG_TRACE
	MSG("(Eng)==EnvTempStatus = %d, coolingTime = %d==\n",EnvTempStatus, coolingTime);
#endif
// #if NEW_FUSER_TEST_PARAM
	_Set_T1(coolingTime, PSPS_WaitCooling)
// #else
	// _Set_T1(ENG_PARAM_SLOW_COOL_TIME, PSPS_WaitCooling)
// #endif
	_Set_PS_SequenceID_Change(PSPS_WaitCooling)
}

void 	PSP_WaitCooling(PageSupervisor *PGSPVSR)
{
	// MSG("$$$ 1.Current_Temperature = %d, cProcessCounter = %d $$$\n",Fuser_Control.Current_Temperature,cProcessCounter);
	_Check_T1_and_RET

	Set_CoolingNotify(CLEAR);
	StartMainMotor();
	Fuser_Control.Target_Temperature = PGSPVSR->FuserTemp;
	_Set_T1(PSTicks_Minimum, PSPS_WaitFuserTemp)
	_Set_PS_SequenceID_Change(PSPS_WaitFuserTemp)
}

void 	PSP_WaitFuserTemp(PageSupervisor *PGSPVSR)
{	
	_Check_T1_and_RET

	if(ChangeVoltageToTemp() >= (Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP))
	{
		_Set_T1(PSTicks_Minimum, PSPS_PickupForContinue)
		_Set_PS_SequenceID_Change(PSPS_PickupForContinue)
	}
}

void	PSP_StartPrintSequence(PageSupervisor *PGSPVSR)			/* PSPS_StartPrintSequence */
	/* Set Wait Time for Fuser Temperature */
{
	PGSPVSR->Flags = PSPS_StartPrintSequence;
	_Set_PS_SequenceID_Change(PSPS_StartPrinter);
}

void	PSP_StartPrinter(PageSupervisor *PGSPVSR)				/* PSS_StartPrinter */
	/* Start 1st Print Sequence and Now Start Motors */
{
	Uint8 temp_DeviceID, temp_EmptyState;
	PGSPVSR->Flags = PSPS_StartPrinter;
	// MSG("(Eng) ## PrintPageCmdTemp = %d, STDB temp = %d,  Target_Temperature = %d, Current_Temperature = %d, Fuser_Control.State = %d <%d>##\n", PrintPageCmdTemp, Fuser_Setting_Parameters[1], Fuser_Control.Target_Temperature, Fuser_Control.Current_Temperature, Fuser_Control.State, cProcessCounter);
	
	if(fStartFromPowerSaveMotorRun) 
	{
		if(/*Set_MainMotorRunningNotify() == TRUE*/1)			//24Apr09 yjlee 
		{
			MSG("(Eng)PSP_StartPrintSequence : fStartFromPowerSaveMotorRun = %d <%d>(%d)\n", fStartFromPowerSaveMotorRun, cProcessCounter, fMotorSpeedMode);
			fStartFromPowerSaveMotorRun = CLEAR;
			fNoCheckTraySourceForPreRun = CLEAR;

			StartMainMotor();
			StartMirrorMotor();
			StartEP_PrintFromStandby();
			EPtimingBUFF_Old = cProcessCounter;
			EPtimingBUFF_New = cProcessCounter;
// 07May09 yjlee
/*
			_Set_T1(PSTicks_ChargeToLD, PSPS_ChargeToAPC)
			_Set_PS_SequenceID_Change(PSPS_ChargeToAPC)
*/
			if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_Set_T1(PSTicks_ChargeToDevOn_L, PSPS_Dev_On)}
			else 										{_Set_T1(PSTicks_ChargeToDevOn, PSPS_Dev_On)}
			_Set_PS_SequenceID_Change(PSPS_Dev_On)
		}
	}
	else if(Fuser_Control.State >= ST_PRINT)
	// if(Fuser_Control.State >= ST_STANDBY)
	{
		if(fNoCheckTraySourceForPreRun == CLEAR)
		{
			if(IsSourceGood(PGSPVSR) == FALSE)
			{
				fRequest_New_PageInfo = CLEAR;
#if	ENG_TRACE		
				MSG("(Eng)PSP_StartPrinter - Source Check fail\n");
#endif
				if (fPGSPVSRRemoved == TRUE)
				{
					fPGSPVSRRemoved = FALSE;
					return;
				}
				if (temp_fPaperEmptyForT1==EMPTY)
				{
					temp_DeviceID = MS_Tray1;
					temp_EmptyState = EMPTY;
				}
				else if (temp_fPaperEmptyForMPF==EMPTY)
				{
					temp_DeviceID = MS_MultiPurposeTray;
					temp_EmptyState = EMPTY;
				}
				else if (temp_fPaperEmptyForT2==EMPTY)
				{
					temp_DeviceID = MS_Tray2;
					temp_EmptyState = EMPTY;
				}
				else if (temp_fPaperEmptyForT3==EMPTY)
				{
					temp_DeviceID = MS_Tray3;
					temp_EmptyState = EMPTY;
				}
		        else
		        {
		            MSG("(Eng)PSP_StartPaperPickupSequence - Unable to locate empty tray using default\n"); //-jrs
		            temp_DeviceID = MS_Tray1;   //-jrs set default values to avoid compiler error in release build
		            temp_EmptyState = EMPTY;    //-jrs set default values to avoid compiler error in release build
		        }
				
				if (Set_SourceEmptyNotify(temp_DeviceID,temp_EmptyState) == (Boolean)TRUE)
				{
					usleep(50*1000);
					if(fClearEmpty == TRUE)
					{
						fClearEmpty = CLEAR;
						fClearErrorsCmdRcv = CLEAR;
						temp_fPaperEmptyForT1 = CLEAR;
						temp_fPaperEmptyForMPF = CLEAR;
						temp_fPaperEmptyForT2 = CLEAR;
						temp_fPaperEmptyForT3 = CLEAR;
						FinishPageSupervisor( PGSPVSR );
						Set_PrintReadyNotify();
#if	ENG_TRACE				
						MSG("(Eng)Empty Noti > ClearIRE\n");
#endif
					}
					else
					{
						fClearEmpty = CLEAR;
						if (temp_fPaperEmptyForT1==EMPTY)
						{
							temp_fPaperEmptyForT1 = CLEAR;
							fPaperEmptyForT1=EMPTY;
						}
						// else if (temp_fPaperEmptyForMPF==EMPTY)
						if (temp_fPaperEmptyForMPF==EMPTY)
						{
							temp_fPaperEmptyForMPF = CLEAR;
							fPaperEmptyForMPF=EMPTY;
						}
						// else if (temp_fPaperEmptyForT2==EMPTY)
						if (temp_fPaperEmptyForT2==EMPTY)
						{
							temp_fPaperEmptyForT2 = CLEAR;
							fPaperEmptyForT2=EMPTY;
						}
						if (temp_fPaperEmptyForT3==EMPTY)
						{
							temp_fPaperEmptyForT3 = CLEAR;
							fPaperEmptyForT3=EMPTY;
						}
#if	ENG_TRACE				
						MSG("(Eng)Empty Noti > NO ClearIRE\n");
#endif
						fPaperIsEmptyBeforeMotorRun = TRUE;
						_Set_PS_SequenceID_Change(PSS_PrinterError)
					}
				}
				// _Set_PS_SequenceID_Change(PSS_PrinterError)
				return;
			}
			else
			{
				fRequest_New_PageInfo = CLEAR;
			}
		}
		
		fNoCheckTraySourceForPreRun = CLEAR;
		
		if ( 0 ) 
		{
			if(/*Set_MainMotorRunningNotify() == TRUE*/1) 	//21Apr09 yjlee 
			{
#if	ENG_TRACE				
				MSG("(Eng)PSP_StartPrintSequence to PSPS_EraseOn <%d>\n", cProcessCounter);
#endif
				StartMirrorMotor();
				_Set_T2(PSTicks_Minimum, PSPS_StartServoSequence)
				_Set_T1(PSTicks_XfrToErase /* - overtime */, PSPS_EraseOn)
				/* Limit time for which PLL signal of motor comes Lock */
				_Set_PS_SequenceID_Change(PSPS_EraseOn)			
			}
		}
		else if(/*Set_MainMotorRunningNotify() == TRUE*/1) 	//21Apr09 yjlee 
		{
#if	ENG_TRACE			
			MSG("(Eng)PSP_StartPrintSequence to PSPS_ChargeToAPC <%d>(%d)\n",cProcessCounter, fMotorSpeedMode);
#endif
			StartMainMotor();
			StartMirrorMotor();
			StartEP_PrintFromStandby();
			fStartFromPowerSaveMotorRun = CLEAR;
			EPtimingBUFF_New = cProcessCounter;
			EPtimingBUFF_Old = cProcessCounter;
// 07May09 yjlee
/*
			_Set_T1(PSTicks_ChargeToLD, PSPS_ChargeToAPC)
			_Set_PS_SequenceID_Change(PSPS_ChargeToAPC)
*/
			if(fMotorSpeedMode == MTR_MODE_QUIET)						{_Set_T1(PSTicks_ChargeToDevOn_L, PSPS_Dev_On)}
			else 														{_Set_T1(PSTicks_ChargeToDevOnForPrint, PSPS_Dev_On)}
			_Set_PS_SequenceID_Change(PSPS_Dev_On)
		}
	}
}

void	PSP_StartEPCleaning (PageSupervisor * PGSPVSR)
						/* PSPS_StartEPCleaning */
		/* Wait for Starting EP cleaning process */
{
	/* To countermeasure for 936 Service */
	if ((PGSPVSR->PageAhead != NULL) && (PGSPVSR->PageAhead->Active == TRUE)) 
	{
		if (PGSPVSR->PageAhead->SequenceID < PSPS_StartExtendedFeed)
			return ;
		else if (PGSPVSR->PageAhead->SequenceID >= PSS_StartEPCleaning)
		{
			_Set_PS_SequenceID_Change(PSPS_PickupForContinue)
			return ;
		}
	}
	PGSPVSR->Flags = PSPS_StartEPCleaning;
	StartTransferCleaning();
		/* Set Xfr Bias parameter for Xfr cleaning */
	_Set_T1(PSTicks_XfrToCharge + PSTicks_NipAndRising, PSPS_ChargeCleaning)
	_Set_PS_SequenceID_Change(PSPS_ChargeCleaning)
}

void	PSP_ChargeCleaning(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET

	StartChargeCleaning();
	_Set_T1(PSTicks_Charge2Roll, PSPS_PickAftEPCleaning)
	_Set_PS_SequenceID_Change(PSPS_PickAftEPCleaning)	
}

void	PSP_Dev_Off(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	StartDeveloperCleaning();
	_Set_T1(PSTicks_CleaningTime, PSPS_PickAftEPCleaning)
	_Set_PS_SequenceID_Change(PSPS_PickAftEPCleaning)
}

void	PSP_PickAftEPCleaning (PageSupervisor *PGSPVSR)
						/* PS_PRContinueCleaning */
		/* Continue 2 cycles of Chaging roll */
{
	_Check_T1_and_RET

	PGSPVSR->Flags = PSPS_StartPrintSequence ;
	StartEP_PrintFromIdle();
	/* Set the parameters of EP to start the sequence of Print from standby. */
	_Set_T2(0, PSS_NONE)
	_Set_T1(PSTicks_ChargeToDev, PSPS_Dev_On)
		/* The time from Charge roll to Developer roll with nip of Developer roll */
	_Set_PS_SequenceID_Change(PSPS_Dev_On)
}

void	PSP_ChargeToAPC(PageSupervisor *PGSPVSR)					/* PSS_ChargeToAPC */
{
	_Check_T1_and_RET
	/* Auto Power Control */
	_Set_T1(PSTicks_ContinueAPC, PSPS_ContinueAPC)
	_Set_PS_SequenceID_Change(PSPS_ContinueAPC)
}

void	PSP_ContinueAPC(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	_Set_T1(PSTicks_APC_EndToDev + 2 + PSTicks_XMotorLockUp , PSPS_Dev_On)
	_Set_PS_SequenceID_Change(PSPS_Dev_On)
}

void	PSP_Dev_On(PageSupervisor *PGSPVSR)
{
	if((cChargerCnt < Num_ChargerTable) && ((cProcessCounter - cTempProcessCnt) >= 2))
	{
		//MSG("(eng)Set_Charger_PWM () = %d (%d)\n", EP_ChargerTable[cChargerCnt], cProcessCounter);
		Set_Charge_PWM(EP_ChargerTable[cChargerCnt]);
		cChargerCnt++;
		cTempProcessCnt = cProcessCounter;
	}
	_Check_T1_and_RET
#if	ENG_TRACE		
	MSG("(Eng)PSP_Dev_On <%d>\n", cProcessCounter);
#endif	
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T1 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
//	MSG("(Eng)##########EP_Check Point_2 = %d##########\n",cProcessCounter);
	StartDeveloperDefault();
//07May09 yjlee
/*	
	_Set_T1(2*PSTicks_DevToXfr + PSTicks_XfrToCharge + PSTicks_ChargeToDev +1, PSPS_Xfr_On)
*/
	if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_Set_T1(PSTicks_DevOnToXfrServo_L, PSPS_Xfr_On)}
	else 								 		{_Set_T1(PSTicks_DevOnToXfrServo, PSPS_Xfr_On)}
	_Set_PS_SequenceID_Change(PSPS_Xfr_On)
}

void	PSP_Xfr_On(PageSupervisor *PGSPVSR)
{	
	if(cDeveloperCnt < Num_DeveloperTable)
	{
//		MSG("(eng)Set_Developer_PWM = %d (%d)\n", EP_DeveloperTable[cDeveloperCnt], cProcessCounter);
		Set_Developer_PWM(EP_DeveloperTable[cDeveloperCnt]);
		cDeveloperCnt++;
	}
	_Check_T1_and_RET
	
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
#if	ENG_TRACE	
	MSG("(Eng)PSP_Xfr_On <%d>\n", cProcessCounter);
#endif	
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T2 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	
	_Set_T2(PSTicks_Minimum, PSPS_StartServoSequence)
	_Set_T1(PSTicks_XfrToErase, PSPS_EraseOn)
		/* Limit time for which PLL signal of motor comes Lock */
	_Set_PS_SequenceID_Change(PSPS_EraseOn)
}

void	PSP_EraseOn(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET	
	
	_Set_T1(PSTicks_MotorLock /* - overtime */, PSPS_MotorWarmup)
	_Set_PS_SequenceID_Change(PSPS_MotorWarmup)
}

void	PSP_StartServoSequence(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
//	MSG("(Eng)##########EP_Check Point_3 = %d##########\n",cProcessCounter);
	StartServoSearch();
	_Set_T2(PSTicks_ServoStep200ms, PSPS_XfrServoStepUp)
}

void	PSP_XfrServoStepUp(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET

	if(ServoSearchUpProcedure(SERVO_UP_STEP) == TRUE) 
	{
		_Set_T2(PSTicks_Minimum, PSPS_XfrServoStepDown)
	}
	else 
	{
		_Set_T2(PSTicks_ServoStep20ms, PSPS_XfrServoStepUp)
	}
}

void	PSP_XfrServoStepDown(PageSupervisor *PGSPVSR)
{
	ServoSearchDownProcedure(SERVO_DOWN_STEP);
	_Set_T2(PSTicks_ServoStep20ms, PSPS_XfrServoStep1Up)
}

void	PSP_XfrServoStep1Up(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET

	if(ServoSearchUpProcedure(SERVO_ONE_UP_STEP) == TRUE) 
	{
		_Set_T2(PSTicks_Minimum, PSPS_XfrEndServo)		
	}
	else 
	{
		_Set_T2(PSTicks_ServoStep20ms, PSPS_XfrServoStep1Up)
	}
}

void	PSP_XfrEndServo(PageSupervisor *PGSPVSR)
{
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
#if	ENG_TRACE	
	MSG("(Eng)PSP_XfrEndServo <%d>\n",cProcessCounter);
#endif	
	
	PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
	if((page != NULL))
	{
		MSG("(eng)PSP_XfrEndServo : JobNum = %d, JobInfo = %d, DocType = %d\n", page->pMyJob->JobNum, page->pMyJob->JobInfo, page->pMyJob->DocType);
	}
	else
	{
		MSG("(eng)PSP_XfrEndServo : page is NULL\n");
	}

	unsigned char level = 0;
	LDPWMAdjustforCopy_Value = 0;
	if((LDPWMAdjustforCopy == SET) || ((page != NULL) && (page->pMyJob->JobInfo == COPYJOB) && (page->pMyJob->DocType == e_COPYPAGE))) // copy
	{
		if(TransferBiasPWMForServo >= 146) 			level = 5;
		else if(TransferBiasPWMForServo >= 116) 	level = 4;
		else if(TransferBiasPWMForServo >= 101) 	level = 3;
		else if(TransferBiasPWMForServo >= 91) 		level = 2;
		else if(TransferBiasPWMForServo >= 63) 		level = 1;
		else 										level = 0;
		LDPWMAdjustforCopy_Value += ((int)TransferBiasPWMForServo * LSUBeamPowerDutyAdjustSlopeTableForServoValue[level]) + LSUBeamPowerDutyAdjustOffsetTableForServoValue[level];
				
		if(DrumLifePageCount >= 10001) 				level = 1;
		else 										level = 0;
		LDPWMAdjustforCopy_Value += ((DrumLifePageCount * LSUBeamPowerDutyAdjustSlopeTableForDLPC[level]) + LSUBeamPowerDutyAdjustOffsetTableForDLPC[level])/100000;
		
		if(fMotorSpeedMode == MTR_MODE_QUIET) {
			video_set_pcode(PWM_LOW_SPEED_MODE, LDPWMAdjustforCopy_Value);
		} else {
			video_set_pcode(PWM_ORG_SPEED_MODE, LDPWMAdjustforCopy_Value);
		}
			
		LDPWMAdjustforCopy = CLEAR;
	}
	MSG("(eng)PSP_XfrEndServo : LDPWMAdjustforCopy_Value = %d (Servo = %d, DLPC = %d) Speedmode = %d\n", LDPWMAdjustforCopy_Value, TransferBiasPWMForServo, DrumLifePageCount, fMotorSpeedMode);
	_Set_T2(NONE, PSS_NONE)
}

void	PSP_PaperCleaning(PageSupervisor *PGSPVSR)
{
	if(IsPaperPresentOnPexs() == PRESENT) 
	{
		PGSPVSR->T2_State = PSPS_PaperCleaning;
		_Set_T2(PSLimit_Legal - PSLimit_PinsToPexs, PSPS_PaperCleaningOnPins)
		return;
	}
	_Check_T2_and_RET
	
	PaperJamOnPins = JAM;
	_Set_PS_SequenceID_Change(PSS_PrinterError)
}

void 	PSP_PaperCleaningOnPins(PageSupervisor *PGSPVSR)
{
	if(IsPaperPresentOnPins() == NOT_PRESENT) 
	{
		PGSPVSR->T2_State = PSPS_PaperCleaningOnPins;
		_Set_T2(PSTicks_PinsToXfr + PSTicks_1s, PSPS_PaperCleaningOnXfr)
		return;
	}
	_Check_T2_and_RET
	/* Paper Jam between pins and pexs */
	PaperJamBetweenPinsAndPexs = JAM;
	_Set_PS_SequenceID_Change(PSS_PrinterError)
}

void	PSP_PaperCleaningOnXfr(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
	PGSPVSR->T2_State = PSPS_PaperCleaningOnXfr;
	_Set_T2(PSTicks_Minimum, PSPS_StartServoSequence)
}

void	PSP_MotorWarmup(PageSupervisor *PGSPVSR)
{
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
	
	if(PGSPVSR->T1_Value < PGSPVSR->T1_Comp)
	{
		if(Get_MainMotorLockState() == UNLOCK)
			return ;
		
		if(Get_MirrorMotorLockState() == UNLOCK)
			return;
	}
	else 
	{
		if(Get_MainMotorLockState() == UNLOCK) 
		{
			fPaperFeedMotorFailure = ERROR;
			CodeOfMachineCheck = NO_MAIN_MOTOR_LOCK;
		}
		if(Get_MirrorMotorLockState() == UNLOCK) 
		{
			fMirrorMotorFailure = ERROR;
			CodeOfMachineCheck = NO_MIRROR_MOTOR_LOCK;
		}
	}
	
	if ( PGSPVSR->T2_State != PSS_NONE )
		return;
	

#if	ENG_TRACE	
	MSG("(Eng)##EP_Check Point_LD_On = %d##\n",cProcessCounter);
#endif		

	Set_LSUOnNotify();

	_Set_T1(PSTicks_5s, PSPS_HSync_Detact)
	_Set_PS_SequenceID_Change(PSPS_HSync_Detact)
}

void	PSP_MotorWarmupForPwsToPrint(PageSupervisor *PGSPVSR)
{
	if(PGSPVSR->T2_Value > CLEAR) 
	{
		if(Get_MainMotorLockState() == UNLOCK)
			return ;
		
		if(Get_MirrorMotorLockState() == UNLOCK)
			return;
	}
	else 
	{
		if(Get_MainMotorLockState() == UNLOCK) 
		{
			fPaperFeedMotorFailure = ERROR;
			CodeOfMachineCheck = NO_MAIN_MOTOR_LOCK;
		}
		if(Get_MirrorMotorLockState() == UNLOCK) 
		{
			fMirrorMotorFailure = ERROR;
			CodeOfMachineCheck = NO_MIRROR_MOTOR_LOCK;
		}
	}
#if	ENG_TRACE	
	MSG("(Eng)##EP_Check Point_LD_On_For_PwsToPrint = %d##\n",cProcessCounter);
	Set_DeveloperBiasForServo(TransferBiasPWMForServo);
	Set_DeveloperBiasForPrinting(PGSPVSR->Darkness, PGSPVSR->TonerSaver);
#endif		
//	EnableLD();					
	fSpeedUpForPWS = CLEAR;
	_Set_T2(NONE, PSS_NONE)
}

void PSP_HSync_Detact(PageSupervisor *PGSPVSR)
{
	if(PGSPVSR->T1_Value < PGSPVSR->T1_Comp)
	{
		if(fHSync_Detacted == CLEAR)
		{		
			return;
		}
	}
	else
	{
		//fNoFirstHsyncSignal = SET;
		//CodeOfMachineCheck = PRINTHEAD_LOST_HSYNC;
	}
	
//	fHSync_Detacted = CLEAR;

#if	ENG_TRACE	
	MSG("(Eng)PSP_HSync_Detact <%d>\n", cProcessCounter);
#endif		

#if ABBA2
	if(fMotorSpeedMode == MTR_MODE_QUIET) {
#if	ENG_TRACE
		MSG("(Eng)PSP_Developer_Off <%d>\n",cProcessCounter);
#endif
		EPtimingBUFF_Old = cProcessCounter;
		EPtimingBUFF_New = cProcessCounter;
		
		StartDeveloperNoPrinting();
		StartTransferCleaning();
		
		_Set_T1(PSTicks_DeveloperNoPrintingLength_L, PSPS_Developer_On)
		_Set_PS_SequenceID_Change(PSPS_Developer_On)
	}
	else {
		_Set_T1(PSTicks_Minimum, PSPS_Developer_Off)
		_Set_PS_SequenceID_Change(PSPS_Developer_Off)
	}
#else
#if	ENG_TRACE
	MSG("(Eng)PSP_Developer_Off <%d>\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = cProcessCounter;
	EPtimingBUFF_New = cProcessCounter;
	
	StartDeveloperNoPrinting();
	StartTransferCleaning();
	
	if(fMotorSpeedMode == MTR_MODE_QUIET)			{_Set_T1(PSTicks_DeveloperNoPrintingLength_L, PSPS_Developer_On)}
	else 											{_Set_T1(PSTicks_DeveloperNoPrintingLengthForPrint, PSPS_Developer_On)}
	_Set_PS_SequenceID_Change(PSPS_Developer_On)	
#endif	
	// _Set_T1(PSTicks_LDEnToDev - PSTicks_HSync, PSPS_Developer_Off)
//	if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_Set_T1(PSTicks_Minimum, PSPS_Developer_Off)}
//	else 								 		{_Set_T1(PSTicks_Minimum, PSPS_Developer_Off)}
//	_Set_PS_SequenceID_Change(PSPS_Developer_Off)
}

void PSP_Developer_Off(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
#if	ENG_TRACE
	MSG("(Eng)PSP_Developer_Off <%d>\n",cProcessCounter);
#endif
	StartDeveloperNoPrinting();
	StartTransferCleaning();
	
	EPtimingBUFF_Old = cProcessCounter;
	EPtimingBUFF_New = cProcessCounter;

//08May09 yjlee	
	if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_Set_T1(PSTicks_DeveloperNoPrintingLength_L, PSPS_Developer_On)}
	else 										{_Set_T1(PSTicks_DeveloperNoPrintingLengthForPrint, PSPS_Developer_On)}
	_Set_PS_SequenceID_Change(PSPS_Developer_On)	
}

void PSP_Developer_On(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
#if	ENG_TRACE
	MSG("(Eng)PSP_Developer_On <%d>\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T3 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	
	CurrentDarknessLevel = GetDarknessConversionRate(PGSPVSR->Darkness);
	Set_DeveloperBiasForPrinting(PGSPVSR->Darkness, PGSPVSR->TonerSaver);
	
	PGSPVSR->T1_State = PSPS_FinishInit;
	_Set_PS_SequenceID_Change(PSPS_FinishInit)	
	if(fMotorSpeedMode == MTR_MODE_QUIET) 			{_Set_T2(PSTicks_XfrCleaningTime_L - PSTicks_DeveloperNoPrintingLength_L, PSPS_Transfer_On)}
	else 											{_Set_T2(PSTicks_XfrCleaningTime - PSTicks_DeveloperNoPrintingLengthForPrint + PSTicks_Minimum, PSPS_Transfer_On)}
}

void	PSP_Transfer_On(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
#if	ENG_TRACE
	MSG("(Eng)PSP_Transfer_On <%d>\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T4 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	
	Set_TransferBiasInterpageGap(FALSE);
	_Set_T2(NONE, PSS_NONE)	
}

void	PSP_FinishInit(PageSupervisor *PGSPVSR)
{
	if(Detect_OverSensorAtWarmup()) 
	{
		if(fJamDetacted1st == TRUE)
		{
			return;
		}
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}

	if(PGSPVSR->T2_State != PSS_NONE) 
	{
		return ;
	}
	if(PGSPVSR->PageAhead != NULL) 
	{
		if(PGSPVSR->PageAhead->SequenceID == PSPS_DuplexFrontPrintReady) 
		{
			_Set_PS_SequenceID_Change(PSPS_PickupForContinue)
			return;
		}
	}
	// MSG("$$$ Fuser_Control.State = %d,  fMotorRunWithoutPrintCmd = %d $$$\n",Fuser_Control.State,fMotorRunWithoutPrintCmd);
	if(fMotorRunWithoutPrintCmd == SET) 
	{
//		if((Fuser_Control.State == ST_STANDBY) || (Fuser_Control.State == ST_PRINT))
		if(1)
		{
			fMotorRunWithoutPrintCmd = CLEAR;
			fMotorRun = CLEAR;
			_Set_PS_SequenceID_Change(PSS_StopMainMotor)
		}
		return;
	}
	else 
	{
		if(PGSPVSR->SequenceID == PSPS_FinishInit)
		{
			if((Fuser_Control.State >= ST_PRINT) && (fWaitPrintPageCmd == CLEAR) && (fChangeTargetTemp == CLEAR))
			{
				_Set_PS_SequenceID_Change(PSPS_StartPaperPickupSequence)
			}
		}
	}
}

void	PSP_PickupForContinue(PageSupervisor *PGSPVSR)
{
	Uint32 NextPaperPickupTime;
	
	if(PGSPVSR->PageAhead == NULL || PGSPVSR->PageAhead->Active == NOT_ACTIVE || PGSPVSR->PageAhead->SequenceID == PSPS_EmptyCheckWaiting ) 
	{
		if(PGSPVSR->PageAhead != NULL)
		{
			if(PGSPVSR->PageAhead->SequenceID == PSPS_EmptyCheckWaiting)
			{
				MSG("(Eng)$$ Link Page GOGO!!! $$\n");
				FinishPageSupervisor(PGSPVSR->PageAhead);
			}
		}
		_Set_PS_SequenceID_Change(PSPS_StartPrintSequence)
	}
	else if(PGSPVSR->PageAhead->SequenceID > PSS_FinishEPCleaning /*PSS_Idle*/) 
	{
		return ;
	}
	/* Print From Idle */
	else if(PGSPVSR->PageAhead->SequenceID == PSS_FinishEPCleaning/*PSS_Idle*/) 
	{
		PGSPVSR->Flags = PSPS_StartPrintSequence ;

		if (/*Set_MainMotorRunningNotify() == RUNNING*/1) 	//21Apr09 yjlee 
		{
			FinishPageSupervisor(PGSPVSR->PageAhead);
			StartMainMotor();
			StartMirrorMotor();	
			/* Set the parameters of EP to start the sequence of Print from idle */
			StartEP_PrintFromIdle();

			_Set_T2(NONE, PSS_NONE)
			/* Time from charge roll to developer roll with nip */
			_Set_T1(PSTicks_ChargeToDev, PSPS_Dev_On)
			_Set_PS_SequenceID_Change(PSPS_Dev_On)
		}
	}

	/* for continous printing after cleaning mode (Main Motor On) */
	else if(PGSPVSR->PageAhead->SequenceID == (Uint8)PSS_StopMainMotor) 
	{
		PGSPVSR->PageAhead->Flags = PSS_StopMainMotor ;
		PGSPVSR->Flags = PSPS_StartPrintSequence ;
		StartEP_PrintFromIdle() ;
		/* Set the parameters of EP to start the sequence of Print from standby. */
		_Set_T2(NONE, PSS_NONE)
		/* Time from charge roll to developer roll with nip */
		_Set_T1(PSTicks_ChargeToDev, PSPS_Dev_On)
		_Set_PS_SequenceID_Change(PSPS_Dev_On) /* LD Auto Power Control ??? */
	}
	/* Wait to end EP Cleaning */
	else if(PGSPVSR->PageAhead->SequenceID >= PSS_StartEPCleaning) 
	{
		return ;
	}
	else if(PGSPVSR->PageAhead->OutputDestination == OutputDuplexor) 
	{
	        /*if(PGSPVSR->PageAhead->Flags == PSPS_DuplexFrontPrintReady) {*/
		if(PGSPVSR->PageAhead->Flags == PSPS_ToDuplexHoldPoint
		|| PGSPVSR->PageAhead->Flags == PSPS_DuplexFrontPrintReady) 
		{
			DuplexFrontSidePrint = SET;
			SavedRealMediaSizeInPrePage = Get_PrePageMediaSize(PGSPVSR->PageAhead->Source);
			/* Update Pre PageSupervisor Data with Now PageSupervisor(Duplex Front Data) */
			SavedSourceInPrePage = PGSPVSR->PageAhead->Source;
			PGSPVSR->PageAhead->PageID = PGSPVSR->PageID;
			PGSPVSR->PageAhead->Source = PGSPVSR->Source;
			/*PGSPVSR->PageAhead->OutputDestination = PGSPVSR->OutputDestination;*/
			 /* Test YJSONG */
			PGSPVSR->PageAhead->OutputDestination = PGSPVSR->OutputDestination;
			PGSPVSR->PageAhead->page_info = PGSPVSR->page_info;
			PGSPVSR->PageAhead->page_done_info = PGSPVSR->page_done_info;
			PGSPVSR->PageAhead->fvideodone = 0;
			
			PGSPVSR->Active = NOT_ACTIVE;
			
			_Set_PageAheadT1(PSTicks_Minimum, PSPS_StartPaperPickupSequence)
			_Set_PageAhead_PS_SequenceID_Change(PSPS_StartPaperPickupSequence)
		}
		return;
	}
	/* For Waiting RIP Long Page Process */
	else if(PGSPVSR->PageAhead->SequenceID >= PSPS_StartExtendedFeed)
	{
	}
	/* Page Trailing edge passed paper input sensor */
	else if(PGSPVSR->PageAhead->SequenceID > PSPS_CheckPinsToPins)
	{
		NextPaperPickupTime = Get_WaitTimeForNextPickup(PGSPVSR) ; 
#if	ENG_TRACE		
		MSG("(Eng)$$case 1: NextPaperPickupTime = %d $$\n", NextPaperPickupTime);
#endif		
		_Set_T1(NextPaperPickupTime, PSPS_StartPaperPickupSequence)
		_Set_PS_SequenceID_Change(PSPS_StartPaperPickupSequence)
	}
	/* Paper is passing Paper input sensor */
	else if(PGSPVSR->PageAhead->SequenceID == PSPS_CheckPinsToPins)
	{
		NextPaperPickupTime = Get_WaitTimeForNarrowMedia(PGSPVSR);
#if	ENG_TRACE		
		MSG("(Eng)$$case 2: NextPaperPickupTime = %d $$\n", NextPaperPickupTime);
#endif
		_Set_T1(NextPaperPickupTime, PSPS_StartPaperPickupSequence)		// 14Aug09 : CHG 1 (new)
		//_Set_T1(NextPaperPickupTime + 30, PSPS_StartPaperPickupSequence)			// 14Aug09 : CHG 1 (del)
		_Set_PS_SequenceID_Change(PSPS_StartPaperPickupSequence)
	}
}

void	PSP_StartPaperPickupSequence(PageSupervisor *PGSPVSR)
{
	Uint8 temp_DeviceID, temp_EmptyState;

	if((PGSPVSR->fvideoOn == CLEAR) && (engine_state.fcancelrequest == TRUE))
	{
		MSG("(eng)Cancel this page : No Pickup\n");
		if(HowManyActivePageSupervisor() == 1)
		{
			fcancelpage = SET;
		}
		else
		{
			FinishPageSupervisor( PGSPVSR );
		}
		return;
	}
	
	if(((PGSPVSR->Source == MS_Tray2) || (PGSPVSR->Source == MS_Tray3))
	&& (Get_Elapse_UART() < 2)
	&& (cTray2PaperCheckBeforePickUp == 0))
	{
		MSG("(eng)Option Notify delay (%d) <%d>\n", fTray2IsNotifying, cProcessCounter);
		return;
	}

	if(((((PGSPVSR->PageAhead != NULL) && (PGSPVSR->PageAhead->Active == TRUE)) && (PGSPVSR->PageAhead->SequenceID == PSPS_SendOptionTrayNotifyBeforeVideoStart))
	 || (((int)(PGSPVSR->T1_Comp - PGSPVSR->T1_Value) <= 3) && (cTray2PaperCheckBeforePickUp <= 1)))
	 && ((PGSPVSR->Source == MS_Tray2) || (PGSPVSR->Source == MS_Tray3))
	 && (fDebugOptionCommFail == TRUE))
	{
		if(((PGSPVSR->PageAhead != NULL) && (PGSPVSR->PageAhead->Active == TRUE)) && (PGSPVSR->PageAhead->SequenceID == PSPS_SendOptionTrayNotifyBeforeVideoStart))
		{
			MSG("(eng)Option Tray Check before video start (%d)\n", cProcessCounter);
		}
		
#if	ENG_TRACE		
		MSG("(Eng)Wait TrayStatus : cnt=%d <%d>\n",cTray2PaperCheckBeforePickUp,cProcessCounter);
#endif		
		if(cTray2PaperCheckBeforePickUp == 0)
		{
			if(f2ndDrawerState == SET) Option_Tx_Notify(OPTION_T1);
		}
		else
		{
			if(f3ndDrawerState == SET) Option_Tx_Notify(OPTION_T2);
		}
		Set_Timer_UART();
		cTray2PaperCheckBeforePickUp++;
		return;
	}
	
	//if(IsSourceGood(PGSPVSR->Source) == FALSE) 
	if(IsSourceGood(PGSPVSR) == FALSE) 
	{
		fRequest_New_PageInfo = CLEAR;
#if	ENG_TRACE		
		PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
		
		MSG("(Eng)++ IsSourceGood() false (c %d): (PageNum = %d) (JobID %d, PageID %d) <%p> ++\n"
		,cProcessCounter, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount, PGSPVSR->page_info);
#endif
		fSpeedUpForPWS = CLEAR;
		
		if (fPGSPVSRRemoved == TRUE)
		{
			fPGSPVSRRemoved = FALSE;
			return;
		}
		if (temp_fPaperEmptyForT1==EMPTY)
		{
			temp_DeviceID = MS_Tray1;
			temp_EmptyState = EMPTY;
			cTray2PaperCheckBeforePickUp = 0;
		}
		else if (temp_fPaperEmptyForMPF==EMPTY)
		{
			temp_DeviceID = MS_MultiPurposeTray;
			temp_EmptyState = EMPTY;
			cTray2PaperCheckBeforePickUp = 0;
		}
		else if (temp_fPaperEmptyForT2==EMPTY)
		{
			temp_DeviceID = MS_Tray2;
			temp_EmptyState = EMPTY;
			cTray2PaperCheckBeforePickUp = 0;
		}
		else if (temp_fPaperEmptyForT3==EMPTY)
		{
			temp_DeviceID = MS_Tray3;
			temp_EmptyState = EMPTY;
			cTray2PaperCheckBeforePickUp = 0;
		}
        else
        {
            MSG("(Eng)PSP_StartPaperPickupSequence - Unable to locate empty tray using default\n"); //-jrs
            temp_DeviceID = MS_Tray1;   //-jrs set default values to avoid compiler error in release build
            temp_EmptyState = EMPTY;    //-jrs set default values to avoid compiler error in release build
            cTray2PaperCheckBeforePickUp = 0;
        }
		
		if (Set_SourceEmptyNotify(temp_DeviceID,temp_EmptyState) == (Boolean)TRUE)
		{
			if(fClearEmpty == TRUE)
			{
				fClearEmpty = CLEAR;
				fClearErrorsCmdRcv = CLEAR;
				temp_fPaperEmptyForT1 = CLEAR;
				temp_fPaperEmptyForMPF = CLEAR;
				temp_fPaperEmptyForT2 = CLEAR;
				temp_fPaperEmptyForT3 = CLEAR;
				FinishPageSupervisor( PGSPVSR );
				Set_PrintReadyNotify();
#if	ENG_TRACE				
				MSG("(Eng)Empty Noti > ClearIRE\n");
#endif
			}
			else
			{
				fClearEmpty = CLEAR;
				if (temp_fPaperEmptyForT1==EMPTY)
				{
					temp_fPaperEmptyForT1 = CLEAR;
					fPaperEmptyForT1=EMPTY;
				}
				// else if (temp_fPaperEmptyForMPF==EMPTY)
				if (temp_fPaperEmptyForMPF==EMPTY)
				{
					temp_fPaperEmptyForMPF = CLEAR;
					fPaperEmptyForMPF=EMPTY;
				}
				// else if (temp_fPaperEmptyForT2==EMPTY)
				if (temp_fPaperEmptyForT2==EMPTY)
				{
					temp_fPaperEmptyForT2 = CLEAR;
					fPaperEmptyForT2=EMPTY;
				}
				if (temp_fPaperEmptyForT3==EMPTY)
				{
					temp_fPaperEmptyForT3 = CLEAR;
					fPaperEmptyForT3=EMPTY;
				}
#if	ENG_TRACE				
				MSG("(Eng)Empty Noti > NO ClearIRE\n");
#endif
				_Set_PS_SequenceID_Change(PSS_PrinterError)
			}
		}
		// _Set_PS_SequenceID_Change(PSS_PrinterError)
		return;
	}
	else
	{
		fRequest_New_PageInfo = CLEAR;
	}
	/* End of Transfer Servo Search */
	if(PGSPVSR->T2_State != PSS_NONE) 
	{
#if	ENG_TRACE		
		MSG("(Eng)Servo Search fail : T2_State = %d\n", PGSPVSR->T2_State);
#endif		
		return ;
	}
	
	if(((Fuser_Control.State == ST_PRINT) && (PickupTempSelect(PGSPVSR) <= ChangeVoltageToTemp())) || (PGSPVSR->Source == MS_Duplex))
	{
		_Check_T1_and_RET
	
		if((fPwsToPrint == SET) && (fPaperLengthIsNarrow == CLEAR))
		{
			if(PGSPVSR->Source != MS_Duplex)
			{
				if(fPwsToPrint_2ndPick == TRUE)
				{
					if(fSpeedUpForPWS == SET)
					{
						return;
					}
					MSG("(eng)fPwsToPrint_2ndPick\n");
					fPwsToPrint = CLEAR;
					fPwsToPrint_1stPick = CLEAR;
					fPwsToPrint_2ndPick = CLEAR;
				}
			}
			if(fPwsToPrint_1stPick == TRUE)
			{
				MSG("(eng)fPwsToPrint_1stPick\n");
				fPwsToPrint_2ndPick = TRUE;
			}
		}
		/* Clearing Paper Path */

		/* EXPERIMENT - Record for ISR to address SHORT PAGE Problems */
		Picking_PGSPVSR = PGSPVSR;
		if(fRetryPickUp == CLEAR)
		{
			// lsptodo why change the page we are on ?
			// engine_state.picked_page = engine_state.pick_page;
			// PGSPVSR->page_info = engine_state.picked_page;
		}
		PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
		
#if	ENG_TRACE	
		MSG("(Eng)$$ Paper Pickup : %d (PageID = %d) (JobID %d, PageID %d) Source = %x, Cur_Temp = %d, Pickup_Temp = %d, Target_Temp = %d <page = %p> $$\n"
		, cProcessCounter, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount, PGSPVSR->Source, ChangeVoltageToTemp()
		, PickupTempSelect(PGSPVSR), Fuser_Control.Target_Temperature, PGSPVSR->page_info);
		
		cTray2PaperCheckBeforePickUp = 0;
		cTempPickupTime = cProcessCounter;
#endif
		MSG("(eng)Tray level state : (tray1)high = %d low = %d fTrayOpen = %d, (tray2)State[0] = %x, State[1] = %x, (tray3)State[0] = %x, State[1] = %x\n", 
					tray1_high, tray1_low, fTrayOpen, OptionTray1State[0], OptionTray1State[1], OptionTray2State[0], OptionTray2State[1]);
#if IPG_ENABLE
		
		if((fRetryPickUp == CLEAR) && (fVideo_Print_Enable == SET))
		{
			PGSPVSR->fvideoOn = SET;
			video_print_page((PAGE_DATA*)PGSPVSR->page_info);
			MSG("## video_print_page 1st page of the job : %p (%d) <%d> ## \n", PGSPVSR->page_info, PGSPVSR->PageID, cProcessCounter);
			
			slogit(  dec_enable_vdo( eng_hardware->laser, true ));
		    slogit(  dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, true )); // false for off
		    fVideo_Print_Enable = CLEAR;
		}
#endif
		switch(PGSPVSR->Source) 
		{
			case MS_MultiPurposeTray:
				MSG("(eng)MPT PICKUP\n");
				if(f2ndDrawerState == SET) Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
				if(f3ndDrawerState == SET) Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
				if(f2ndor3rdpickEmpty == CLEAR) TurnOnMPFSolenoid();
				SolenoidOnTimeCount = CLEAR;
				fSolenoidOnOff = SOLENOID_MPT;
				tmp_SolenoidOnTime = cProcessCounter;
				_Set_T1(PSTicks_ChargeBeforePins, PSPS_FinishPaperPickup)
				_Set_PS_SequenceID_Change(PSPS_FinishPaperPickup)
				break;

			case MS_Tray1:
				MSG("(eng)TRAY1 PICKUP\n");
				if(f2ndDrawerState == SET) Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
				if(f3ndDrawerState == SET) Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
				if(f2ndor3rdpickEmpty == CLEAR) TurnOnTray1Solenoid(PGSPVSR->RecycledPaperEnable);
#if ABBA2
				SolenoidOnTimeCount = CLEAR;
				fSolenoidOnOff = SOLENOID_TRAY1;
				tmp_SolenoidOnTime = cProcessCounter;
#endif
#if PINETREE_L
				//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
				if(1)
#else
				if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
				{
					SolenoidOnTimeCount = CLEAR;
					fSolenoidOnOff = SOLENOID_TRAY1;
					tmp_SolenoidOnTime = cProcessCounter;
				}
				_Set_T1(PSTicks_ChargeBeforePins, PSPS_FinishPaperPickup)
				_Set_PS_SequenceID_Change(PSPS_FinishPaperPickup)
				break;

			case MS_Tray2:
				if(f2ndDrawerState == SET) 
				{
					MSG("(eng)TRAY2 PICKUP\n");
//					fTray2PickUp = TRUE;
					if(fTrayOpen == CLEAR)
					{
						if(f2ndor3rdpickEmpty == CLEAR)
						{
							MSG("(eng)fRetryPickUp = %d, fLastPickUp = %d, Tray2FeedSensorOn = %d\n", fRetryPickUp, fLastPickUp, Tray2FeedSensorOn);
							if(((fRetryPickUp == CLEAR) && (fLastPickUp == CLEAR))
							|| (((fRetryPickUp == SET) || (fLastPickUp == SET)) && (Tray2FeedSensorOn == false)))
							{
								if(f3ndDrawerState == SET) Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
								if(fMotorSpeedMode == MTR_MODE_NORMAL)
								{
									if(fDebugOptionCommFail == TRUE)
									{
										if(OptionTray1Pro == OPTION_TRAY_250) 	Option_Tx_Pickup(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS_250, 0);
										else 									Option_Tx_Pickup(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS, 0);
									}
								}
								else
								{
									if(fDebugOptionCommFail == TRUE) 		Option_Tx_Pickup(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS_QUIET, 0);
								}
							}
						}
					}
					else
					{
						MSG("(eng)T1 Open During Printing\n");
					}
				}
				_Set_T1(PSTicks_ChargeBeforePins, PSPS_FinishPaperPickup)
				_Set_PS_SequenceID_Change(PSPS_FinishPaperPickup)	
				break;
				
			case MS_Tray3:
				if(f3ndDrawerState == SET) 
				{
					MSG("(eng)TRAY3 PICKUP\n");
//					fTray3PickUp = TRUE;
					if((fTrayOpen == CLEAR) && ((OptionTray1State[1]&0x70) != OPTION_OPEN))
					{
						if(f2ndor3rdpickEmpty == CLEAR)
						{
							MSG("(eng)fRetryPickUp = %d, fLastPickUp = %d, Tray2FeedSensorOn = %d, Tray3FeedSensorOn = %d\n", fRetryPickUp, fLastPickUp, Tray2FeedSensorOn, Tray3FeedSensorOn);
							if(((fRetryPickUp == CLEAR) && (fLastPickUp == CLEAR))
							|| (((fRetryPickUp == SET) || (fLastPickUp == SET)) && (Tray2FeedSensorOn == false) && (Tray3FeedSensorOn == false)))
							{
								if(fMotorSpeedMode == MTR_MODE_NORMAL)
								{
									if(fDebugOptionCommFail == TRUE)
									{
										if(OptionTray2Pro == OPTION_TRAY_250) 
										{
											Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS_250);
											Option_Tx_Pickup(OPTION_T2, TRAY2_MOTORRUNSPEED_KARAS_250, PGSPVSR->OptionPrePick);
										}
										else 
										{
											Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS);
											Option_Tx_Pickup(OPTION_T2, TRAY2_MOTORRUNSPEED_KARAS, PGSPVSR->OptionPrePick);
										}
									}
								}
								else
								{
									Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORRUNSPEED_KARAS_QUIET);
									if(fDebugOptionCommFail == TRUE) 		Option_Tx_Pickup(OPTION_T2, TRAY2_MOTORRUNSPEED_KARAS_QUIET, PGSPVSR->OptionPrePick);
								}
							}
						}
					}
					else
					{
						MSG("(eng)T1 or T2 Open During Printing\n");
					}
				}
				_Set_T1(PSTicks_ChargeBeforePins, PSPS_FinishPaperPickup)
				_Set_PS_SequenceID_Change(PSPS_FinishPaperPickup)	
				break;
				
			case MS_Duplex:
				if(HoldingDuplex) 
				{
					/* Keep 1*speed time */
					_AddTime_T1(PSTicks_Minimum, PSPS_FinishPaperPickup)
					_Set_PS_SequenceID_Change(PSPS_FinishPaperPickup)
				}
				else 
				{
					_AddTime_T1(PSTicks_Minimum, PSPS_FinishPaperPickup)
					_Set_PS_SequenceID_Change(PSPS_FinishPaperPickup)
				}
				break;

			default:
		        fSoftwareFailure = ERROR;
				CodeOfMachineCheck = SOFTWARE_ERROR;
				_Set_PS_SequenceID_Change(PSS_PrinterError)
				break;
		}
		f2ndor3rdpickEmpty = CLEAR;
	}
	else 
	{
		return;
	}
}

void	PSP_ContinueTray2Pickup(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSPS_ContinueTray2Pickup;
	
	_Check_T1_and_RET

//	TurnOnTray2Solenoid();					//24Apr09 yjlee
	_AddTime_T1(PSTicks_SolenoidOnTime, PSPS_FinishPaperPickup)
	_Set_PS_SequenceID_Change(PSPS_FinishPaperPickup)	
}				

void	PSP_FinishPaperPickup(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSPS_FinishPaperPickup;
	
	_Check_T1_and_RET
	int level;
	Uint8 ChargerAdjustPWM = 0;
	int ChargerAdjustPWM_Servo = 0;
	int ChargerAdjustPWM_Drum = 0;
#if EP_PARAM_TEST
	if(fParamChargerPWM == TRUE)
#else
	if(0)
#endif
	{
		Set_Charge_PWM(Param_Charger_PWM);
	}
	else
	{
#if MAC_TYPE_MH
		if(TransferBiasPWMForServo > 254)			level = 7;
		else if(TransferBiasPWMForServo >= 146)		level = 6;
		else if(TransferBiasPWMForServo >= 116)		level = 5;
		else if(TransferBiasPWMForServo >= 101)		level = 4;
		else if(TransferBiasPWMForServo >= 91)		level = 3;
		else if(TransferBiasPWMForServo >= 60)		level = 2;
		else if(TransferBiasPWMForServo >= 57)		level = 1;
		else 										level = 0;
		ChargerAdjustPWM_Servo = (((int)TransferBiasPWMForServo*ChargerPWMAdjustSlopeTable[level])+ChargerPWMAdjustOffsetTable[level])/1000;
		MSG("(eng)ChargerAdjustPWM_Servo = %d\n", ChargerAdjustPWM_Servo);
		if(DrumLifePageCount >= 40001) 					level = 3;
		else if(DrumLifePageCount >= 20001) 			level = 2;
		else if(DrumLifePageCount >= 10001) 			level = 1;
		else 									level = 0;
		ChargerAdjustPWM_Drum = ((DrumLifePageCount*ChargerPWMAdjustSlopeTable_DrumPages[level])+ChargerPWMAdjustOffsetTable_DrumPages[level])/100000;
		MSG("(eng)ChargerAdjustPWM_Drum = %d\n", ChargerAdjustPWM_Drum);
		
		ChargerAdjustPWM = (Uint8)(DefaultCharge_PWM+ChargerAdjustPWM_Servo+ChargerAdjustPWM_Drum);
		Set_Charge_PWM(ChargerAdjustPWM);
#else
		if(TransferBiasPWMForServo <= 65)
		{
			Set_Charge_PWM(DefaultCharge_PWM-18);
		}
#endif
	}
	
	//video_print_page((PAGE_DATA*)engine_state.picked_page);
	//MSG("## video_print_page : %p ## \n", engine_state.picked_page);
/*
	if(fRetryPickUp == CLEAR)
	{
#if IPG_ENABLE
		if(fVideo_Print_Enable == SET)
		{
			video_print_page((PAGE_DATA*)PGSPVSR->page_info);
			MSG("## 1. video_print_page : %p (%d) <%d> ## \n", PGSPVSR->page_info, PGSPVSR->PageID, cProcessCounter);
			
			slogit(  dec_enable_vdo( eng_hardware->laser, true ));
		    slogit(  dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, true )); // false for off
		    fVideo_Print_Enable = CLEAR;
		}
#else
		video_print_page((PAGE_DATA*)PGSPVSR->page_info);
		MSG("## video_print_page : %p (%d) <%d> ## \n", PGSPVSR->page_info, PGSPVSR->PageID, cProcessCounter);
		
		slogit(  dec_enable_vdo( eng_hardware->laser, true ));
	    slogit(  dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, true )); // false for off
#endif
	}
*/	
	switch (PGSPVSR->Source)
	{
		case MS_MultiPurposeTray:
			//TurnOffMPFSolenoid();
			_AddTime_T1(PSLimit_TrayToPins, PSPS_DetectStartToPins)
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;
		 
		case MS_Tray1:
//			TurnOffTray1Solenoid();
			if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_AddTime_T1(PSLimit_TrayToPins_QUIET, PSPS_DetectStartToPins)}
			else 										{_AddTime_T1(PSLimit_TrayToPins, PSPS_DetectStartToPins)}
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;
		
		case MS_Tray2:
			if(fMotorSpeedMode == MTR_MODE_QUIET)		{_AddTime_T1(PSLimit_Tray2ToPins_QUIET, PSPS_DetectStartToPins)}
			else 										{_AddTime_T1(PSLimit_Tray2ToPins, PSPS_DetectStartToPins)}
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;
		
		case MS_Tray3:
			if(fMotorSpeedMode == MTR_MODE_QUIET)		{_AddTime_T1(PSLimit_Tray3ToPins_QUIET, PSPS_DetectStartToPins)}
			else 										{_AddTime_T1(PSLimit_Tray3ToPins, PSPS_DetectStartToPins)}
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;
		
		case MS_Duplex:
			DuplexFrontSidePrint = CLEAR;
			if(fMotorSpeedMode == MTR_MODE_QUIET)		{_AddTime_T1(PSLimit_TrayToPins_QUIET, PSPS_DetectStartToPins)}
			else 										{_AddTime_T1(PSLimit_TrayToPins, PSPS_DetectStartToPins)}
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;
		
		default:
			TurnOffTray1Solenoid();
			fSoftwareFailure = ERROR;
			CodeOfMachineCheck = SOFTWARE_ERROR;
			_Set_PS_SequenceID_Change(PSS_PrinterError)
			break;
	}			
}

void	PSP_BoostCharge(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSPS_BoostCharge;
	
	_Check_T1_and_RET
#if	ENG_TRACE		
	MSG("(Eng)PSP_BoostCharge\n");
#endif	
	if((PGSPVSR->Darkness >= DARKNormal )  /* Was == DARKDarkest */
	&& (PGSPVSR->TonerSaver == TONERSaverOFF)) 
	{
		Boost_Charge();
	}
	else 
	{
		EnableChargeBiasSignal();
	}
	switch(PGSPVSR->Source) 
	{
		case MS_MultiPurposeTray:
			_AddTime_T1(PSLimit_MPFToPins - PSTicks_PickupToBoostChargeMPF, PSPS_DetectStartToPins)
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;
		
		case MS_Tray1:
			_AddTime_T1(PSLimit_T1ToPins - PSTicks_PickupToBoostChargeT1, PSPS_DetectStartToPins)
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;

		case MS_Tray2:
			_AddTime_T1(PSLimit_T2ToPins - PSTicks_PickupToBoostChargeT2, PSPS_DetectStartToPins)
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;
		
		case MS_Tray3:
			_AddTime_T1(PSLimit_T2ToPins - PSTicks_PickupToBoostChargeT3, PSPS_DetectStartToPins)
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)
			break;

		case MS_Duplex:
			/* Time between Holding Point and Paper Input Sensor */
			if(HoldingDuplex) 
			{
				/* For 1*speed */
				_AddTime_T1(PSLimit_DuplexHoldPointToPins - PSTicks_PickupToBoostChargeDuplex, PSPS_DetectStartToPins)/**/
			}
			else 
			{
				/* For 1.5*speed */
				_AddTime_T1(PSLimit_DuplexHoldPointToPins - PSTicks_PickupToBoostChargeDuplex, PSPS_DetectStartToPins)/**/
			}
			_Set_PS_SequenceID_Change(PSPS_DetectStartToPins)			
			break;

		default:
			fSoftwareFailure = ERROR;
			CodeOfMachineCheck = SOFTWARE_ERROR;
			_Set_PS_SequenceID_Change(PSS_PrinterError)
			break;
	}			
}

void	PSP_DetectStartToPins(PageSupervisor *PGSPVSR)
{
	MESSAGE message;
	
	Uint32 LeftPaperLength;
	
	if(PinsIsUsedByOtherPageSupervisor == NOT_USED) 
	{
		if( ( IsPaperPresentOnPins() == TRUE ) 
		/* || (bFakeFeeding && (PGSPVSR->T1_Value == PSLimit_T1ToPins -2)) */ ) 	// 22Apr09 yjlee
		{
			TurnOffDuplexSolenoid();
			if(ChatteringForPinsOn < CHATTERING_TIME_PINS) 
			{
				ChatteringForPinsOn++;
				return;
			}
			engine_state.active_page = engine_state.picked_page;
			
			ChatteringForPinsOn = CLEAR;
	
			//slogit(  dec_enable_vdo( eng_hardware->laser, true ));
			//slogit(  dec_laser_set_top_sync( eng_hardware->laser, true ));  // fake top
			PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
#if	ENG_TRACE			
			MSG("(Eng)++Input Sensor_Detacted = %d (PageID = %d) (JobID %d, PageID %d) <%p>++\n"
			,cProcessCounter, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount, PGSPVSR->page_info);
			MSG("(Eng)++pickup to input sensor = %d\n",cProcessCounter-cTempPickupTime);
#endif
			EPtimingBUFF_Old = cProcessCounter;
			EPtimingBUFF_New = cProcessCounter;
			PGSPVSR->InputDetectTime = cProcessCounter;
			VideoDoneTime = cProcessCounter;
			if(PGSPVSR->Source == MS_Tray1)
			{
				TurnOffTray1Solenoid();
			}
//			gpio_write(eng_hardware->GPIO_E17, 1); // for test
			PGSPVSR->Flags = PSPS_DetectStartToPins;
			PinsIsUsedByOtherPageSupervisor = USED;
			fWaitPageCmd = TRUE;

			/* EXPERIMENT - Tell ISR NOT to Assert PSYNC */
			Picking_PGSPVSR = NULL;

			LeftPaperLength = (Uint32)DetectStartPaperToPins(PGSPVSR);
			fRetryPickUp = CLEAR;
			fLastPickUp = CLEAR;			
//08May09 yjlee
			_Set_T2(PSTicks_50ms, PSPS_DetectNarrowMedia)			
			// _Set_T2(PSTicks_PinsToDevPrint, PSPS_Dev_OnPrinting)
			_Set_T1(PSTicks_50ms, PSPS_ToNextPrintReady)	
			// _Set_T1(LeftPaperLength, PSPS_ToNextPrintReady)	
			_Set_PS_SequenceID_Change(PSPS_ToNextPrintReady)
			return;
		}
		ChatteringForPinsOn = CLEAR;
	}
	_Check_T1_and_RET
	/* Paper Jam Error */
#if	ENG_TRACE	
	MSG("(Eng)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  Paper is not detacted on Input Sensor - Paper Jam case  +\n");
	// MSG("(Eng)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  cProcessCounter = %d (PageID = %d)\n",cProcessCounter, PGSPVSR->PageID);
#endif	
#ifndef KARA_S_OLD_ENGINE
	dcmotor_run(eng_hardware->paper_motor, eng_hardware->speed_off); //pickup dcmotor stop
#endif
	MSG("(eng)SensorTime(ID %d) : Input (%d,%d) Exit (%d,%d) InDe-InRe (%d) InDe-ExDe (%d) ExDe-ExRe (%d) InRe-ExRe (%d)\n"
		, PGSPVSR->PageID, PGSPVSR->InputDetectTime, PGSPVSR->InputReleaseTime, PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime
		, PGSPVSR->InputReleaseTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitDetectTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->InputReleaseTime);
	if(fRetryPickUp == CLEAR)
	{
		// if(PGSPVSR->Source == MS_MultiPurposeTray)
		if((PGSPVSR->Source == MS_MultiPurposeTray) || (PGSPVSR->Source == MS_Tray1) || (PGSPVSR->Source == MS_Tray2) || (PGSPVSR->Source == MS_Tray3))	// ver2.0.10 : Tray1도 Pick up 실패시 한번 더 시도	// ver2.0.14 : Tray2도 Pick up 실패시 한번 더!!!
		{
			if(fPwsToPrint == SET) fPwsToPrint_2ndPick = FALSE;
			fRetryPickUp = SET;

			if(((PGSPVSR->Source == MS_Tray2) || (PGSPVSR->Source == MS_Tray3))
			&& (cOptionCommFail > 0) && (f1stOptionResync == SET))
			{
				_Set_PS_SequenceID_Change(PSPS_OptionResync)
			}
			else
			{
				_Set_PS_SequenceID_Change(PSPS_StartPaperPickupSequence)
			}
			return;
		}
	}
	else
	{
		if(((PGSPVSR->Source == MS_MultiPurposeTray) || (PGSPVSR->Source == MS_Tray1) || (PGSPVSR->Source == MS_Tray2) || (PGSPVSR->Source == MS_Tray3)) && (fLastPickUp == CLEAR))
		{
			if(fPwsToPrint == SET) fPwsToPrint_2ndPick = FALSE;
			fLastPickUp = SET;
			if(((PGSPVSR->Source == MS_Tray2) || (PGSPVSR->Source == MS_Tray3))
			&& (cOptionCommFail > 0) && (f1stOptionResync == SET))
			{
				_Set_PS_SequenceID_Change(PSPS_OptionResync)
			}
			else
			{
				_Set_PS_SequenceID_Change(PSPS_StartPaperPickupSequence)
			}
			return;
		}
	}
	
	if((PGSPVSR->Source == MS_Tray2) && (f2ndDrawerState == SET))
	{
//		fTray2MotorStop = TRUE;
		Option_Tx_MotorRun(OPTION_T1, TRAY2_MOTORSTOPSPEED);
	}
	
	if((PGSPVSR->Source == MS_Tray3) && (f3ndDrawerState == SET))
	{
//		fTray3MotorStop = TRUE;
		Option_Tx_MotorRun(OPTION_T2, TRAY2_MOTORSTOPSPEED);
	}
	
	VideoOffBeforeJamNotify();
	
	message.msgType = MSG_STATUSUPDATE;
	switch (PGSPVSR->Source)
	{
		case MS_Tray1:
			engine_state.misspick_tray1_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY1 ); 
			break;
		case MS_Tray2:
			engine_state.misspick_tray2_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY2 ); 
			break;
		case MS_Tray3:
			engine_state.misspick_tray3_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY3 ); 
			break;
		case MS_MultiPurposeTray:
			engine_state.misspick_tray4_jam = true;
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TRAY4 ); 
			break;
		case MS_Duplex:
			if(PageNumVideoDoneFail != CLEAR)
			{
				fVideoDoneFail = SET;
				CodeOfMachineCheck = VIDEO_DONE_FAIL;
			}
			else
			{
				engine_state.duplex_jam = true;
				slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_DUPLEX ); 
			}
			break;
	}
	if(((PageNumVideoDoneFail == CLEAR) && (PGSPVSR->Source == MS_Duplex)) || (PGSPVSR->Source != MS_Duplex))
	{
		message.param3 = engine_state.picked_page;
//		dbg_validate_page( message.param3 );
		SYMsgSend(PRINTMGRID, &message);
		MSG("(Eng)++MissPick = %d (Page = %p)++\n",cProcessCounter, engine_state.picked_page);
	        
		if(PGSPVSR->Source == MS_Duplex) 
		{
			PaperJamInDuplex = JAM;
			CodeOfJamStatus = JAM_DUPLEX_PATH;
		}
		else 
		{
			PaperJamBetweenPickupAndPins = JAM;
			
			if(PGSPVSR->Source == MS_MultiPurposeTray)
			{
				CodeOfJamStatus = JAM_LE_INPUT_DETACT_MPT;
			}
			else if(PGSPVSR->Source == MS_Tray2)
			{
				CodeOfJamStatus = JAM_LE_INPUT_DETACT_OPTION;
			}
			else if(PGSPVSR->Source == MS_Tray3)
			{
				CodeOfJamStatus = JAM_LE_INPUT_DETACT_OPTION2;
			}
			else
			{
				CodeOfJamStatus = JAM_LE_INPUT_DETACT_TRAY1;
			}
		}
	}
	_Set_PS_SequenceID_Change(PSS_PrinterError)
}

void	PSP_ToNextPrintReady(PageSupervisor *PGSPVSR)
{	
	PGSPVSR->Flags = PSPS_ToNextPrintReady;
	Set_PrintReadyNotify();		//24Apr09 yjlee
#if	ENG_TRACE
	MSG("(Eng)PSP_ToNextPrintReady: T1_Value = %d, T1_Comp = %d\n",PGSPVSR->T1_Value, PGSPVSR->T1_Comp);
#endif	
		
	/* The trailing edge of media is not out completely from the source */
	if(PGSPVSR->T1_Value < PGSPVSR->T1_Comp) 
	{
		PGSPVSR->T1_State = PSPS_WaitNextPickup;
		_Set_PS_SequenceID_Change(PSPS_WaitNextPickup)
	}
	else 
	{
		if(fMotorSpeedMode == MTR_MODE_QUIET) 	{PGSPVSR->T1_Comp = PSLimit_Legal_L;}
		else 									{PGSPVSR->T1_Comp = PSLimit_Legal;}
		PGSPVSR->T1_State = PSPS_CheckPinsToPins;
		_Set_PS_SequenceID_Change(PSPS_CheckPinsToPins)
	}
}

void	PSP_WaitNextPickup(PageSupervisor *PGSPVSR)
	/* Wait for picking up a next page */
{
	_Check_T1_and_RET
#if	ENG_TRACE			
	//MSG("(Eng)PSP_WaitNextPickup\n");
#endif	
	PGSPVSR->Flags = PSPS_ToNextPrintReady;
	
	if(PGSPVSR->Source == MS_Duplex) 
	{
		/* For loading Long size paper after short paper */
		if(fMotorSpeedMode == MTR_MODE_QUIET) 	{PGSPVSR->T1_Comp = PSLimit_Legal_L;}
		else 									{PGSPVSR->T1_Comp = PSLimit_Legal;}
		PGSPVSR->T1_State = PSPS_CheckPinsToPins;
		_Set_PS_SequenceID_Change(PSPS_CheckPinsToPins)
	}
	else 
	{
		/* For loading Long size paper after short paper */
		// PGSPVSR->T1_Comp = PSLimit_Legal ;
		PGSPVSR->T1_Comp = PaperLengthForJamCheck;
		PGSPVSR->T1_State = PSPS_CheckPinsToPins;
		_Set_PS_SequenceID_Change(PSPS_CheckPinsToPins)
	}
}

// #define SLOWDOWN_THRESHOLD	10
// #define	SPEEDUP_THRESHOLD	0
extern Uint8	ThroughputRate;

void	PSP_DetectNarrowMedia(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
	int Temp_PSTicks_ToTxf;
	
	if(fMotorSpeedMode == MTR_MODE_NORMAL) 		NarrowBoundary = NarrowMediaBoundary;
	else if(fMotorSpeedMode == MTR_MODE_QUIET) 	NarrowBoundary = NarrowMediaBoundary_QUIET;
	else 										NarrowBoundary = NarrowMediaBoundary_R1200;
	
	if( ((NarrowMediaCheck > NarrowBoundary) || (NarrowMediaCheck == 0)) && (PGSPVSR->Narrow ==0) )
	{
		bNarrowMedia = NormalWidthMedia;
#if	ENG_TRACE		
		MSG("(Eng))1( Normal Size Media : NarrowMediaCheck = %d, NarrowMediaBoundary = %d, fMotorSpeedMode = %d\n",NarrowMediaCheck,NarrowBoundary, fMotorSpeedMode);
#endif		
	}
	else 
	{		
		bNarrowMedia = NarrowWidthMedia	;
#if	ENG_TRACE		
		MSG("(Eng))2( Narrow Size Media : NarrowMediaCheck = %d, NarrowMediaBoundary = %d, fMotorSpeedMode = %d\n",NarrowMediaCheck,NarrowBoundary, fMotorSpeedMode);
#endif		
	}
		
	if(ThroughputRate == FAST || ThroughputRate == FASTEST) 
	{
		if(bNarrowMedia == NarrowWidthMedia) 
		{
			ThroughputRate = FAST;
			if(++Fuser_Control.NarrowMediaCount >= SLOWDOWN_THRESHOLD) 
			{
				ThroughputRate = SLOW;
			}
		}
		else 
		{
			ThroughputRate = FASTEST;
		}
	}
	else 
	{
		if(bNarrowMedia == NormalWidthMedia)
		{
			if(--Fuser_Control.NarrowMediaCount <= SPEEDUP_THRESHOLD) 
			{ 
				ThroughputRate = FASTEST;
				Fuser_Control.NarrowMediaCount = 0; 
			}
		}
		else 
		{
			Fuser_Control.NarrowMediaCount = SLOWDOWN_THRESHOLD;
		}
	}
	Write_Fuser_Func();
#if	ENG_TRACE	
	MSG("(Eng)ThroughputRate = %d, NarrowMediaCount = %d, T2Delay = %d <%d>\n",ThroughputRate, Fuser_Control.NarrowMediaCount, PGSPVSR->T2Delay, cProcessCounter);
#endif

#if EP_PARAM_TEST
	if(fParamTxfOnTimeVALUE == TRUE)
	{
		if(fMotorSpeedMode == MTR_MODE_QUIET) 		{Temp_PSTicks_ToTxf = Param_TxfOnTime_Low_VALUE - PSTicks_50ms - PGSPVSR->T2Delay;}
		else 										{Temp_PSTicks_ToTxf = Param_TxfOnTime_VALUE - PSTicks_50ms - PGSPVSR->T2Delay;}
	}
	else
	{
		if(fMotorSpeedMode == MTR_MODE_QUIET) 		{Temp_PSTicks_ToTxf = PSTicks_PinsOnToXfr_L - PSTicks_50ms - PGSPVSR->T2Delay;}
		else 										{Temp_PSTicks_ToTxf = PSTicks_PinsOnToXfr - PSTicks_50ms - PGSPVSR->T2Delay;}
	}
#else
	if(fMotorSpeedMode == MTR_MODE_QUIET) 		{Temp_PSTicks_ToTxf = PSTicks_PinsOnToXfr_L - PSTicks_50ms - PGSPVSR->T2Delay;}
	else 										{Temp_PSTicks_ToTxf = PSTicks_PinsOnToXfr - PSTicks_50ms - PGSPVSR->T2Delay;}
#endif
	if(Temp_PSTicks_ToTxf < 1) 	
	{
		Temp_PSTicks_ToTxf = 1; 
		MSG("(eng)Inputdet delay is long\n");
	}
	_Set_T2(Temp_PSTicks_ToTxf, PSPS_Xfr_OnPrinting)
}

/* Timing for Print Process */
void	PSP_Dev_OnPrinting(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
#if	ENG_TRACE	
	MSG("(Eng)**PSP_Dev_OnPrinting = %d**\n",cProcessCounter);
#endif	
//	Set_DeveloperBiasForPrinting(PGSPVSR->Darkness, PGSPVSR->TonerSaver);		// This will be used next build!!! At this moment Dev Voltage = -610 
//08May09 yjlee
//	_Set_T2(PSTicks_PinsToXfr - PSTicks_FeelerOn - PSTicks_XfrMargine - 6, PSPS_Xfr_OnPrinting)
	_Set_T2(PSTicks_DevOnToXfrPrint, PSPS_Xfr_OnPrinting)
}

void	PSP_Xfr_OnPrinting(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
//	StopMainMotor();
//	MSG("(Eng)##########EP_Check Point_8 = %d##########\n",cProcessCounter);

	Set_TransferPrinting(PGSPVSR->MediaDescription.BYTE, PGSPVSR->Source, PGSPVSR->OutputDestination);
#if	ENG_TRACE	
	MSG("(Eng)***PSP_Xfr_OnPrinting = %d***\n",cProcessCounter);

	//MSG("(Eng)*******EP VALUES*******\n");
	//MSG("(Eng)> Txf Servo = %d \n",TransferBiasPWMForServo);
	//MSG("(Eng)> Transfer = %d \n",TransferBiasPWMForPrinting);
	//MSG("(Eng)> Developer = %d \n",SavedDeveloperPWM);
	//MSG("(Eng)> Charger = %d \n",SavedChargePWM);
//	gpio_write(eng_hardware->GPIO_E17, 0); // for test
	MSG("(Eng)*** EP VALUES : Servo=%d, Txf=%d, Dev=%d, Chg=%d ***\n",TransferBiasPWMForServo,TransferBiasPWMForPrinting,SavedDeveloperPWM,SavedChargePWM);
#endif	
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T5 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_Set_T2(PSTicks_XfrOnToExitDet_L, PSPS_CheckPinsToPexs)}
	else 								 		{_Set_T2(PSTicks_XfrOnToExitDet, PSPS_CheckPinsToPexs)}
}

void	PSP_EraseOnPrinting(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
	_Set_T2(PSTicks_EraseToPexs, PSPS_CheckPinsToPexs)
}

void	PSP_CheckPinsToPexs(PageSupervisor *PGSPVSR)
{
	if(cTransferCnt < Num_TransferTable)
	{
		//MSG("(eng)Set_Transfer_PWM = %d (%d)\n", EP_TransferTable[cTransferCnt], cProcessCounter);
		Set_Transfer_PWM(EP_TransferTable[cTransferCnt]);
		cTransferCnt++;
	}
	
	if((PGSPVSR->T2_Value < 100) && (PGSPVSR->T2_Value > 98)) 
	{ 
		if(PGSPVSR->Source == MS_Duplex) 
		{
			if(fStopExitMotor != SET) 
			{
			}
		}
	}
	if((PGSPVSR->T2_Value < 90)) 
	{
		if(fStartExitMotor != SET) 
		{
		}
	}
	
	if(PexsIsUsedByOtherPageSupervisor == FALSE) 
	{
		if(IsPaperPresentOnPexs() == TRUE) 
		{
			// if(ChatteringForPexsOn < CHATTERING_TIME) 
			// {
				// ChatteringForPexsOn++;
				// return;
			// }
			PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
#if	ENG_TRACE			
			MSG("(Eng)++Exit Sensor_Detacted = %d (PageID = %d) (JobID %d, PageID %d) <%p> ++\n"
			,cProcessCounter, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount, PGSPVSR->page_info);
#endif
			PGSPVSR->ExitDetectTime = cProcessCounter;
			ChatteringForPexsOn = CLEAR;
			PexsIsUsedByOtherPageSupervisor = TRUE;
			_Set_T2(NONE, PSS_NONE)
			return;
		}
		ChatteringForPexsOn = CLEAR;
	}
	
	_Check_T2_and_RET
	
#if	ENG_TRACE		
	MSG("(Eng)+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  Paper is not detacted on Exit Sensor - Paper Jam case  +\n");
	// MSG("(Eng)+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  cProcessCounter = %d (PageID = %d)\n",cProcessCounter, PGSPVSR->PageID);
#endif
	MSG("(eng)SensorTime(ID %d) : Input (%d,%d) Exit (%d,%d) InDe-InRe (%d) InDe-ExDe (%d) ExDe-ExRe (%d) InRe-ExRe (%d)\n"
		, PGSPVSR->PageID, PGSPVSR->InputDetectTime, PGSPVSR->InputReleaseTime, PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime
		, PGSPVSR->InputReleaseTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitDetectTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->InputReleaseTime);
	VideoOffBeforeJamNotify();
	
	if(PageNumVideoDoneFail != CLEAR)
	{
		fVideoDoneFail = SET;
		CodeOfMachineCheck = VIDEO_DONE_FAIL;
	}
	else
	{
		
		MESSAGE message;
		
		message.msgType = MSG_STATUSUPDATE;
		engine_state.exit_not_detacted_jam = true;
		slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_EXTSEN_NOT_DET ); 
	
		message.param3 = PGSPVSR->page_info; // lsptodo was :engine_state.active_page;
		dbg_validate_page( message.param3 );
		SYMsgSend(PRINTMGRID, &message);
		
	   /* Paper Jam between pins and pexs */
		PaperJamBetweenPinsAndPexs = JAM;
		CodeOfJamStatus = JAM_LE_EXIT_DETACT;
	}
	_Set_PS_SequenceID_Change(PSS_PrinterError)
}

void	PSP_CheckPinsToPins(PageSupervisor *PGSPVSR)
{
	//MESSAGE message;
	
	PGSPVSR->Flags = PSPS_CheckPinsToPins;
	if(PinsIsUsedByOtherPageSupervisor == USED) 
	{
		if(IsPaperPresentOnPins() == FALSE) 
		{
			if(ChatteringForPinsOff < CHATTERING_TIME) 
			{
				ChatteringForPinsOff++;
				return;
			}
			//slogit( dec_laser_set_top_sync( eng_hardware->laser, false /* disable hvsync */));
			//slogit( dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, false )); // false for off
			//slogit( dec_enable_vdo( eng_hardware->laser, false ));
			PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
#if	ENG_TRACE			
			MSG("(Eng)++Input Sensor Released = %d (PageID = %d) (JobID %d, PageID %d) <%p> ++\n"
			,cProcessCounter, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount, PGSPVSR->page_info);
#endif
			EPtimingBUFF_Old = cProcessCounter;
			EPtimingBUFF_New = cProcessCounter;
			
			PGSPVSR->InputReleaseTime = cProcessCounter;
			cPinsReleaseTime = cProcessCounter;			
			ChatteringForPinsOff = CLEAR;
			PinsIsUsedByOtherPageSupervisor = NOT_USED;
			
			DetectEndPaperToPins(PGSPVSR);
#if IPG_ENABLE
			//_Set_T1(PSTicks_PinsOffToVideoOff, PSPS_VideoOff)
			//_Set_PS_SequenceID_Change(PSPS_VideoOff)
			//_Set_T1(PSTicks_PinsOffToXfr, PSPS_EndToXfr)
			//_Set_PS_SequenceID_Change(PSPS_EndToXfr)
			_Set_T1(1, PSPS_SendOptionTrayNotifyBeforeVideoStart)
			_Set_PS_SequenceID_Change(PSPS_SendOptionTrayNotifyBeforeVideoStart)
#else	
			_Set_T1(PSTicks_PinsOffToXfr, PSPS_EndToXfr)
			_Set_PS_SequenceID_Change(PSPS_EndToXfr)
#endif
			return;
		}
		ChatteringForPinsOff = CLEAR;	
	}
	
	_Check_T1_and_RET
	
#if	ENG_TRACE		
	MSG("(Eng)+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  Paper is not  released on Input Sensor - Paper Jam case  +\n");
	// MSG("(Eng)+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  cProcessCounter = %d (PageID = %d)\n",cProcessCounter, PGSPVSR->PageID);
#endif	
	MSG("(eng)SensorTime(ID %d) : Input (%d,%d) Exit (%d,%d) InDe-InRe (%d) InDe-ExDe (%d) ExDe-ExRe (%d) InRe-ExRe (%d)\n"
		, PGSPVSR->PageID, PGSPVSR->InputDetectTime, PGSPVSR->InputReleaseTime, PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime
		, PGSPVSR->InputReleaseTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitDetectTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->InputReleaseTime);
	VideoOffBeforeJamNotify();
	
	if(PageNumVideoDoneFail != CLEAR)
	{
		fVideoDoneFail = SET;
		CodeOfMachineCheck = VIDEO_DONE_FAIL;
	}
	else
	{
		MESSAGE message;
		
		message.msgType = MSG_STATUSUPDATE;
		engine_state.input_not_released_jam = true;
		slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_INPSEN_NOT_REL ); 
	
		message.param3 = PGSPVSR->page_info; // lsptodo: was : engine_state.active_page;
		SYMsgSend(PRINTMGRID, &message);
	
		/* Paper Jam Error */
		if(IsPaperPresentOnPexs() == TRUE) 
		{
			PaperJamBetweenPinsAndPexs = JAM;
		}
		else 
		{
			PaperJamOnPins = JAM;
		}
		CodeOfJamStatus = JAM_TE_INPUT_RELEASE;
	}
	if ((PGSPVSR->MediaDescription).BIT.Type == MTYPE_Label)
	{
           /* If we are feeding labels and there is a gap closure jam, continue
              feeding until trailing edge of next page is past the fuser.  This
              is to prevent labels getting stuck under the fuser after a jam and
              sticking to the fuser.
           */
		_Set_T1(PSTicks_FuserLabelJamFeedDist, PSPS_FeedJamOutFuser)
		_Set_PS_SequenceID_Change(PSPS_FeedJamOutFuser)
	}
	else
	{
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
}

void    PSP_FeedJamOutFuser(PageSupervisor *PGSPVSR)
{
        /* We just wait here for the transport to feed the labels past the fuser nip */
        _Check_T1_and_RET
        _Set_PS_SequenceID_Change(PSS_PrinterError)
}

void	PSP_WaitPinsOff(PageSupervisor *PGSPVSR)
{

}

extern void* vd_page;
	
void    PSP_NextVideoPage(PageSupervisor *PGSPVSR)
{
	static int longer_video_count = 0;
	_Check_T1_and_RET
	
	if ( (void*)vd_page != (void*)PGSPVSR->page_info && longer_video_count < 100 ) {
		MSG("## (%s) video page is long : %p not eq %p (%d) %p (%d)##\n", __FUNCTION__ , 
		    PGSPVSR->page_info, vd_page, PGSPVSR->PageID, engine_state.video_done, longer_video_count);
		++longer_video_count;
		PageNumVideoDoneFail = PGSPVSR->PageID;
		return;
	} else {
		MSG("## (%s) video done : %p equals %p (%d) %p ##\n", __FUNCTION__, 
		    PGSPVSR->page_info, vd_page, PGSPVSR->PageID, engine_state.video_done);
	
		longer_video_count = 0;
	}
	PGSPVSR->fvideodone = SET;
	
	vd_page = 0;		// 1 to N 의 경우 동일 page pointer이기 때문에 Clear 하지 않을 경우 이전 Page done을 지금 받은 것으로 착각할 수 있음 
//	if((Get_NextVideoPageSupervisor()->Active == ACTIVE) && (PGSPVSR->fNoPaperOnTray == FALSE))
	if((Get_NextVideoPageSupervisor()->Active == ACTIVE) && (PGSPVSR->fNoPaperOnTray == FALSE)
	 && ((fPwsToPrint_1stPick == CLEAR) || ((fPwsToPrint_1stPick == SET) && (fPaperLengthIsNarrow == TRUE))))
	{
		if((TrayEmptyCheck(Get_NextVideoPageSupervisor()->Source) == NO_EMPTY) || (Get_NextVideoPageSupervisor()->SequenceID >= PSPS_FinishPaperPickup))
		{
			Get_NextVideoPageSupervisor()->fvideoOn = SET;
			MSG("++ %s :Source paper %d SeqID = %d ++ \n",__FUNCTION__, TrayEmptyCheck(Get_NextVideoPageSupervisor()->Source), Get_NextVideoPageSupervisor()->SequenceID);
			dbg_validate_page( (PAGE_DATA*)Get_NextVideoPageSupervisor()->page_info );
			video_print_page((PAGE_DATA*)Get_NextVideoPageSupervisor()->page_info);
			MSG("## %s : %p (%d) <%d> %p## \n", __FUNCTION__ , Get_NextVideoPageSupervisor()->page_info, Get_NextVideoPageSupervisor()->PageID, cProcessCounter, Get_TopPageSupervisor()->page_info);
		}
	}
	else
	{
		MSG("++ %s :Source paper %d ++ \n",__FUNCTION__, TrayEmptyCheck(Get_NextVideoPageSupervisor()->Source));
		MSG("## %s : NO NEXT PAGE - Video OFF (%d)## \n",__FUNCTION__, cProcessCounter);
		fVideo_Print_Enable = SET;
		slogit( dec_laser_set_top_sync( eng_hardware->laser, false /* disable hvsync */));
		slogit( dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, false )); // false for off
		slogit( dec_enable_vdo( eng_hardware->laser, false ));
	}
#if EP_PARAM_TEST
	if(fParamTxfOffTimeVALUE == TRUE)
	{
		if(fMotorSpeedMode == MTR_MODE_QUIET) 	{_Set_T1(Param_TxfOffTime_Low_VALUE-3, PSPS_EndToXfr)}
		else 									{_Set_T1(Param_TxfOffTime_VALUE-3, PSPS_EndToXfr)}
	}
	else
	{
		if(fMotorSpeedMode == MTR_MODE_QUIET) 	{_Set_T1(PSTicks_PinsOffToXfr_L-3, PSPS_EndToXfr)}
		else 									{_Set_T1(PSTicks_PinsOffToXfr-3, PSPS_EndToXfr)}
	}
#else
	if(fMotorSpeedMode == MTR_MODE_QUIET) 	{_Set_T1(PSTicks_PinsOffToXfr_L-3, PSPS_EndToXfr)}
	else 									{_Set_T1(PSTicks_PinsOffToXfr-3, PSPS_EndToXfr)}
#endif
	_Set_PS_SequenceID_Change(PSPS_EndToXfr)

}

void	PSP_EndToXfr(PageSupervisor *PGSPVSR)
	/* Turn Off Transfer Bias For Interpage Gap */
{
	//static int longer_video_count = 0;
//	MESSAGE message;
	_Check_T1_and_RET
		
#if ENG_TRACE
	MSG("(Eng)**PSP_Xfr_IPG = %d**\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T6 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	
	Set_TransferBiasInterpageGap(TRUE);

	//if ( (void*)vd_page != (void*)PGSPVSR->page_info && longer_video_count < 100 ) {
		//MSG("## video page is long : %p not eq %p (%d) %p ##\n", 
		    //PGSPVSR->page_info, vd_page, PGSPVSR->PageID, engine_state.video_done);
		//++longer_video_count;
		//return;
	//} else {
		//MSG("## video done : %p equals %p (%d) %p ##\n", 
		    //PGSPVSR->page_info, vd_page, PGSPVSR->PageID, engine_state.video_done);
		//longer_video_count = 0;
	//}

#if 1	//!IPG_ENABLE	
	//slogit( dec_laser_set_top_sync( eng_hardware->laser, false /* disable hvsync */));
	//slogit( dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, false )); // false for off
	//slogit( dec_enable_vdo( eng_hardware->laser, false ));
	
	MSG("## %s : %p (%d) ##\n", __FUNCTION__, PGSPVSR->page_info, PGSPVSR->PageID);

	engine_state.page_exiting = engine_state.active_page;
	engine_state.active_page = 0;
	engine_state.video_done = 0;
			
#endif			

	//_AddTime_T1(PSTicks_XfrOffToExitRel/*- PSTicks_PinsActuate*/, PSPS_CheckPexsToPexs)
	//_Set_PS_SequenceID_Change(PSPS_CheckPexsToPexs)
	
	if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_AddTime_T1(PSTicks_VideoOffToVideoOn_L, PSPS_VideoOn)}
	else 										{_AddTime_T1(PSTicks_VideoOffToVideoOn, PSPS_VideoOn)}
	_Set_PS_SequenceID_Change(PSPS_VideoOn)
}

#if IPG_ENABLE
void 	PSP_SendOptionTrayNotifyBeforeVideoStart(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	MSG("(eng)PSP_SendOptionTrayNotifyBeforeVideoStart (%d)\n", cProcessCounter);
	_Set_T1(2, PSPS_NextVideoPage)
	_Set_PS_SequenceID_Change(PSPS_NextVideoPage)
}

void	PSP_VideoOff(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET

	slogit( dec_laser_set_top_sync( eng_hardware->laser, false /* disable hvsync */));
	slogit( dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, false )); // false for off
	slogit( dec_enable_vdo( eng_hardware->laser, false ));

	MSG("## %s : %p (%d) (%d)##\n", __FUNCTION__, PGSPVSR->page_info, PGSPVSR->PageID, cProcessCounter);

	engine_state.page_exiting = engine_state.active_page;
	engine_state.active_page = 0;
	engine_state.video_done = 0;

//	_AddTime_T1(PSTicks_PinsOffToXfr, PSPS_EndToXfr)
//	_Set_PS_SequenceID_Change(PSPS_EndToXfr)
	
	_AddTime_T1(PSTicks_VideoOffToVideoOn, PSPS_VideoOn)
	_Set_PS_SequenceID_Change(PSPS_VideoOn)
}

void	PSP_VideoOn(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	if((Get_NextVideoPageSupervisor()->Active == ACTIVE) && (PGSPVSR->fNoPaperOnTray == FALSE))
	{
		//dbg_validate_page( (PAGE_DATA*)Get_NextPageSupervisor()->page_info );
		//video_print_page((PAGE_DATA*)Get_NextPageSupervisor()->page_info);
		MSG("## PSP_VideoOn : %p (%d) <%d> ## \n", Get_NextVideoPageSupervisor()->page_info, Get_NextVideoPageSupervisor()->PageID, cProcessCounter);
		//slogit(  dec_enable_vdo( eng_hardware->laser, true ));
		//slogit(  dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, true )); // false for off
	}
	else
	{
		MSG("## PSP_VideoOn - Next Page Not Active <%d>\n", cProcessCounter);
		fVideo_Print_Enable = SET;
		
		slogit( dec_laser_set_top_sync( eng_hardware->laser, false /* disable hvsync */));
		slogit( dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, false )); // false for off
		slogit( dec_enable_vdo( eng_hardware->laser, false ));
	}
	//_AddTime_T1(PSTicks_PinsOffToXfr, PSPS_EndToXfr)
	//_Set_PS_SequenceID_Change(PSPS_EndToXfr)
	if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_AddTime_T1(PSTicks_XfrOffToExitRel_L/*- PSTicks_PinsActuate*/, PSPS_CheckPexsToPexs)}
	else 								 		{_AddTime_T1(PSTicks_XfrOffToExitRel/*- PSTicks_PinsActuate*/, PSPS_CheckPexsToPexs)}
	_Set_PS_SequenceID_Change(PSPS_CheckPexsToPexs)
}

#endif


void	PSP_EndToErase(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	_AddTime_T1(PSTicks_EraseToPexs, PSPS_CheckPexsToPexs)
	_Set_PS_SequenceID_Change(PSPS_CheckPexsToPexs)
}

void	PSP_SelectExitMotorSpeed(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
// Not Used	// 22Apr09 yjlee		
/*
	{
		int i,j;

		j=0;
		for(i=0; i<MAX_PAGE_SUPERVISOR; i++) 
			if( Get_PageSupervisorAddress(i)->Active ) j++;
	
		if((j==1) && (PGSPVSR->OutputDestination != OutputDuplexor)) 
		{
			StartDoubleForwardExitMotor();
		}
	}
*/	
	_AddTime_T1(PSLimit_PinsToPexs - PSTicks_PinsToXfr - PSTicks_XfrToFuser/*- PSTicks_PinsActuate*/, PSPS_CheckPexsToPexs)
	_Set_PS_SequenceID_Change(PSPS_CheckPexsToPexs)
}

void	PSP_CheckPexsToPexs(PageSupervisor *PGSPVSR)
	/* Paper Jam Sensing for Paper length at Paper Exit Sensor */
{
	MESSAGE message;
	PageSupervisor *NextPGSPVSR = NULL;
	
	if(IsPaperPresentOnPexs() == FALSE) 
	{
		// if(ChatteringForPexsOff < CHATTERING_TIME) 
		// {
			// ChatteringForPexsOff++;
			// return;
		// }
		PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
#if	ENG_TRACE		
		MSG("(Eng)++Exit Sensor_Released = %d (PageID = %d) (JobID %d, PageID %d) <%p>++\n"
		,cProcessCounter, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount, PGSPVSR->page_info);
#endif	
		ChatteringForPexsOff = CLEAR;
		PexsIsUsedByOtherPageSupervisor = CLEAR;
		PGSPVSR->ExitReleaseTime = cProcessCounter;
		MSG("(eng)SensorTime(ID %d) : Input (%d,%d) Exit (%d,%d) InDe-InRe (%d) InDe-ExDe (%d) ExDe-ExRe (%d) InRe-ExRe (%d)\n"
			, PGSPVSR->PageID, PGSPVSR->InputDetectTime, PGSPVSR->InputReleaseTime, PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime
			, PGSPVSR->InputReleaseTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitDetectTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->InputReleaseTime);
		//MSG("## PAGEDELIVERYDONE : %p ##\n", engine_state.page_exiting);
		//MSG("## PAGEDELIVERYDONE : %p (%d) ##\n", PGSPVSR->page_info, PGSPVSR->PageID);
		//slogit( message.msgType = MSG_PAGEDONE );
		//message.param3 = engine_state.page_exiting;
		
		cTotalPrintedPageForMotorBWD++;
		
#if DUPLEX_SLOW_MODE
		MSG("(Eng)PGSPVSR->PowersaveToPrint = %d, PGSPVSR->OutputDestination = %d, fPwsToPrint = %d, fPaperLengthIsNarrow = %d, fDuplexPrintSlowlyFor15min = %d\n", PGSPVSR->PowersaveToPrint, PGSPVSR->OutputDestination, fPwsToPrint, fPaperLengthIsNarrow, fDuplexPrintSlowlyFor15min);
		if((((PGSPVSR->PowersaveToPrint == 1)||(PGSPVSR->PowersaveToPrint == 2)) && (PGSPVSR->OutputDestination == OutputStandardOrRear) && (fDuplexPrintSlowlyFor15min == CLEAR))
		&& (fPwsToPrint == TRUE) && (fPaperLengthIsNarrow == CLEAR))
#elif DUPLEX_SLOW_MODE2
		MSG("(Eng)PGSPVSR->PowersaveToPrint = %d, PGSPVSR->OutputDestination = %d, fPwsToPrint = %d, fPaperLengthIsNarrow = %d, fDuplexPrintSlowlyForServoHH = %d\n", PGSPVSR->PowersaveToPrint, PGSPVSR->OutputDestination, fPwsToPrint, fPaperLengthIsNarrow, fDuplexPrintSlowlyForServoHH);
		if((((PGSPVSR->PowersaveToPrint == 1)||(PGSPVSR->PowersaveToPrint == 2)) && (PGSPVSR->OutputDestination == OutputStandardOrRear) && (fDuplexPrintSlowlyForServoHH == CLEAR))
		&& (fPwsToPrint == TRUE) && (fPaperLengthIsNarrow == CLEAR))
#else
		MSG("(Eng)PGSPVSR->PowersaveToPrint = %d, PGSPVSR->OutputDestination = %d, fPwsToPrint = %d, fPaperLengthIsNarrow = %d\n", PGSPVSR->PowersaveToPrint, PGSPVSR->OutputDestination, fPwsToPrint, fPaperLengthIsNarrow);
		if((((PGSPVSR->PowersaveToPrint == 1)||(PGSPVSR->PowersaveToPrint == 2)) && (PGSPVSR->OutputDestination == OutputStandardOrRear))
		&& (fPwsToPrint == TRUE) && (fPaperLengthIsNarrow == CLEAR))
#endif
		{
			MSG("(eng)Motor Speed Up : LDpwmAdjustValue = %d\n", LDPWMAdjustforCopy_Value);
			fMotorSpeedMode = MTR_MODE_NORMAL;
			MainMotorSpeedChange(MotorSpeedparam, fMotorSpeedMode);
//			MirrorMotorSpeedChange(0, fMotorSpeedMode);
//			engine_state.mirror_motor_state = LSU_MIRROR_STOP;
			mirror_lock_clear(eng_hardware->laser);
			video_set_pixel_freq(PIXEL_FREQ_ORG_SPEED_MODE, 0);
			video_set_pcode(PWM_ORG_SPEED_MODE, LDPWMAdjustforCopy_Value);
//			video_set_pcode(PWM_ORG_SPEED_MODE, 0);
			StopMirrorMotor();
			StartMirrorMotor();
			Option_Tx_AccuSpeedSet(OPTION_T1, fMotorSpeedMode);
			Option_Tx_AccuSpeedSet(OPTION_T2, fMotorSpeedMode);
		}
		NextPGSPVSR = Get_NextPageSupervisor();
		if(NextPGSPVSR != NULL)
		{
			//MSG("(eng)NextPGSPVSR->Active = %d, Source = %x, Outputdestination = %d, PageID = %d\n", NextPGSPVSR->Active, NextPGSPVSR->Source, NextPGSPVSR->OutputDestination, NextPGSPVSR->PageID);
			if((PGSPVSR->OutputDestination == OutputDuplexor) && (NextPGSPVSR->Active == CLEAR))
			{
				PGSPVSR->OutputDestination = OutputStandardOrRear;
				MSG("(eng)Set OutputDestination : OutputDuplexor > OutputStandardOrRear\n");
			}
		}
		else
		{
			MSG("(eng)NextPGSPVSR is NULL\n");
		}
		
		if(PGSPVSR->OutputDestination == OutputDuplexor) 
		{
// #if DUPLEX_COOLING
#if MAC_TYPE_MH
			fDuplexSet = TRUE;
			cDuplexPageCount++;
#if DUPLEX_SLOW_MODE
			if(cDuplexPrintSlowlyFor15min == CLEAR)
			{
				Set_DuplexPrintTime(cProcessCounter);
			}
#endif
#endif	
#if	ENG_TRACE			
			MSG("(Eng)PSP_CheckPexsToPexs : OutputDuplexor <cDuplexPageCount=%d>\n",cDuplexPageCount);
#endif
			if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_Set_T1(PSTicks_PexsToExitSolOn_L , PSPS_StopDuplexBackSide)}
			else 								 		{_Set_T1(PSTicks_PexsToExitSolOn , PSPS_StopDuplexBackSide)}
			_Set_PS_SequenceID_Change(PSPS_StopDuplexBackSide)			
		}
		else 
		{
#if	ENG_TRACE				
			MSG("(Eng)PSP_CheckPexsToPexs - Output Dest = Standard\n");
#endif
#if MAC_TYPE_MH
			if(PGSPVSR->Source == MS_Duplex)
			{
				cLastDuplexTime = cProcessCounter;
#if	ENG_TRACE			
				MSG("(Eng)# cLastDuplexTime <%d> #\n",cLastDuplexTime);
#endif
			}
#endif
			
#if MAC_TYPE_MH
			_Set_T2(PSTicks_Minimum, PSPS_EndOfPrintPage)
			_AddTime_T1(PSTicks_100ms, PSPS_FullStackCheckPoint)
			_Set_PS_SequenceID_Change(PSPS_FullStackCheckPoint)
/*			
			if(PGSPVSR->fFullStackCheckSize == TRUE)
			{
#if	ENG_TRACE				
				MSG("(Eng)## fFullStackCheckSize = TRUE ##\n");
#endif				
				_Set_T2(PSTicks_Minimum, PSPS_EndOfPrintPage)
				_AddTime_T1(PSTicks_Minimum, PSPS_FullStackOn)
				_Set_PS_SequenceID_Change(PSPS_FullStackOn)
			}
			else
			{
				_Set_T2(PSTicks_Minimum, PSPS_EndOfPrintPage)
				_AddTime_T1(PSTicks_Minimum, PSPS_StartExtendedFeed)
				_Set_PS_SequenceID_Change(PSPS_StartExtendedFeed)
			}
*/
#else
			_Set_T2(PSTicks_Minimum, PSPS_EndOfPrintPage)
			_AddTime_T1(PSTicks_Minimum, PSPS_StartExtendedFeed)
			_Set_PS_SequenceID_Change(PSPS_StartExtendedFeed)
#endif
			PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
			MSG("## MSG_PAGEDONE : %p (%p, %d) (JobID %d, PageID %d) ##\n"
			, PGSPVSR->page_done_info, PGSPVSR->page_info, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount);
			
			slogit( message.msgType = MSG_PAGEDONE );
			//message.param3 = PGSPVSR->page_info; //lsptodo: was: engine_state.page_exiting;
			message.param3 = PGSPVSR->page_done_info; //lsptodo: was: engine_state.page_exiting;
			if(PGSPVSR->Source == MS_Duplex)	message.param2 = 2;
			else 								message.param2 = 1;
			
			JogModePageCount = GetJogModePageCount(cProcessCounter, message.param2);
			printf("(Eng)JogModePageCount = %d\n", JogModePageCount);
			
			FanCtrlPageCount = GetFanCtrlPageCount(cProcessCounter, message.param2);
			printf("(Eng)FanCtrlPageCount = %d\n", FanCtrlPageCount);
			
			JogModeEnvIdx = GetJogModeEnvIdx(TransferBiasPWMForServo, JogModePageCount);
			FanCtrlEnvIdx = GetFanCtrlEnvIdx(TransferBiasPWMForServo, FanCtrlPageCount);
			
			SYMsgSend(PRINTMGRID, &message);
		}
		//SYMsgSend(PRINTMGRID, &message);
		if(PageNumVideoDoneFail == PGSPVSR->PageID){
			MSG("(eng)PageNumVideoDoneFail CLEAR\n");
			PageNumVideoDoneFail = CLEAR;
		}
	}
	ChatteringForPexsOff = CLEAR;
	_Check_T1_and_RET
	
#if	ENG_TRACE	
	MSG("(Eng)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  Paper is not  released on Exit Sensor - Paper Jam case  +\n");	
	// MSG("(Eng)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	MSG("(Eng)+  cProcessCounter = %d (PageID = %d)\n",cProcessCounter, PGSPVSR->PageID);
#endif
	MSG("(eng)SensorTime(ID %d) : Input (%d,%d) Exit (%d,%d) InDe-InRe (%d) InDe-ExDe (%d) ExDe-ExRe (%d) InRe-ExRe (%d)\n"
		, PGSPVSR->PageID, PGSPVSR->InputDetectTime, PGSPVSR->InputReleaseTime, PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime
		, PGSPVSR->InputReleaseTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitDetectTime-PGSPVSR->InputDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->ExitDetectTime, PGSPVSR->ExitReleaseTime-PGSPVSR->InputReleaseTime);
	VideoOffBeforeJamNotify();
	
	if(PageNumVideoDoneFail != CLEAR)
	{
		fVideoDoneFail = SET;
		CodeOfMachineCheck = VIDEO_DONE_FAIL;
	}
	else
	{
		message.msgType = MSG_STATUSUPDATE;
		engine_state.exit_not_released_jam = true;
		slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_EXTSEN_NOT_REL ); 
	
		message.param3 = PGSPVSR->page_info; //lsptodo: was: engine_state.page_exiting;
		SYMsgSend(PRINTMGRID, &message);
	
		if(IsPaperPresentOnPins() == PRESENT) 
		{
			PaperJamBetweenPinsAndPexs = JAM;
		}
		else 
		{
			PaperJamOnPexs = JAM;
		}
		CodeOfJamStatus = JAM_TE_EXIT_RELEASE;
	}
	_Set_PS_SequenceID_Change(PSS_PrinterError)
}
void 	PSP_FullStackCheckPoint(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
#if	ENG_TRACE		
	MSG("(Eng)PSP_FullStackCheckPoint <%d>\n",cProcessCounter);
#endif
	if(PGSPVSR->fFullStackCheckSize == TRUE)
	{
#if	ENG_TRACE				
		MSG("(Eng)## fFullStackCheckSize = TRUE ##\n");
#endif				
		_AddTime_T1(PSTicks_Minimum, PSPS_FullStackOn)
		_Set_PS_SequenceID_Change(PSPS_FullStackOn)
	}
	else
	{
		_AddTime_T1(PSTicks_Minimum, PSPS_StartExtendedFeed)
		_Set_PS_SequenceID_Change(PSPS_StartExtendedFeed)
	}
}

void	PSP_FullStackOn(PageSupervisor *PGSPVSR)
{
	if(IsPaperPresentOnStack() == PRESENT)
	{
#if	ENG_TRACE		
		MSG("(Eng)PSP_FullStackOn <FullStack On : %d>\n",cProcessCounter);
#endif
		if(fMotorSpeedMode == MTR_MODE_QUIET) 			{_AddTime_T1(PSTicks_FullStackOnToOff_L, PSPS_FullStackOff)}
		else 								 			{_AddTime_T1(PSTicks_FullStackOnToOff, PSPS_FullStackOff)}
		_Set_PS_SequenceID_Change(PSPS_FullStackOff)
	}
	_Check_T1_and_RET
	
	// cFullStackErrCnt++;		
#if	ENG_TRACE		
	MSG("(Eng)PSP_FullStackOn - Full Stack Sensor not detacted : Error cnt : %d <%d>\n",cFullStackErrCnt, cProcessCounter);
#endif
	// if(cFullStackErrCnt >= 3)
	// {
		// fState_FullStack = TRUE;
		// usleep(3000000);
		// PaperJamOnPexs = JAM;
		// CodeOfJamStatus = JAM_TE_EXIT_RELEASE;
		// _Set_PS_SequenceID_Change(PSS_PrinterError)
	// }
	// else
	// {
		_AddTime_T1(PSTicks_Minimum, PSPS_StartExtendedFeed)
		_Set_PS_SequenceID_Change(PSPS_StartExtendedFeed)
	// }
}

void	PSP_FullStackOff(PageSupervisor *PGSPVSR)
{
	if(IsPaperPresentOnStack() != PRESENT)
	{
#if	ENG_TRACE		
		MSG("(Eng)PSP_FullStackOff <FullStack Off : %d>\n",cProcessCounter);
#endif
		cFullStackErrCnt = 0;
		_AddTime_T1(PSTicks_Minimum, PSPS_StartExtendedFeed)
		_Set_PS_SequenceID_Change(PSPS_StartExtendedFeed)
	}
	_Check_T1_and_RET
		
	cFullStackErrCnt++;
#if	ENG_TRACE		
	MSG("(Eng)PSP_FullStackOff - Full Stack Sensor not released : Error cnt : %d <%d>\n",cFullStackErrCnt, cProcessCounter);
#endif
	if(cFullStackErrCnt >= 3)
	{
		fState_FullStack = TRUE;
		//_AddTime_T1(PSTicks_Minimum + 2, PSPS_ToHopper)
		//_Set_PS_SequenceID_Change(PSPS_ToHopper)
		_AddTime_T1(PSTicks_Minimum, PSPS_StartExtendedFeed)
		_Set_PS_SequenceID_Change(PSPS_StartExtendedFeed)
	}
	else
	{
		_AddTime_T1(PSTicks_Minimum, PSPS_StartExtendedFeed)
		_Set_PS_SequenceID_Change(PSPS_StartExtendedFeed)
	}
}
//#endif

void	PSP_StopDuplexBackSide(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSPS_StopDuplexBackSide;
	
	_Check_T1_and_RET
#if	ENG_TRACE		
	MSG("(Eng)PSP_StopDuplexBackSide <Duplex Solenoid On : %d>\n",cProcessCounter);
#endif	
	if(/*Set_PrintCompleteNotify(PGSPVSR->PageID) == TRUE*/1) 			//24Apr09 yjlee
	{
		TurnOnDuplexSolenoid();
//		_Set_T1( PSTicks_Minimum /*PSTicks_50ms*/, PSPS_StartDuplexor)
		if(fMotorSpeedMode == MTR_MODE_QUIET) 		{_Set_T1(PSTicks_DupSolOnToOff_L, PSPS_StartDuplexor)}
		else 								 		{_Set_T1(PSTicks_DupSolOnToOff, PSPS_StartDuplexor)}
		_Set_PS_SequenceID_Change(PSPS_StartDuplexor)
	}
}

void	PSP_StartDuplexor(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSPS_StartDuplexor;
	_Check_T1_and_RET
		
	TurnOffDuplexSolenoid();

//	_Set_T1(PSTicks_Exit1RollToDis_2 /*+ 100*/, PSPS_CheckDis)
	_Set_PS_SequenceID_Change(PSPS_CheckDis)
		
//	_Set_T1(PSTicks_Exit1RollToRegRol, PSPS_ToDuplexHoldPoint)	//05Aug09
	_Set_T1(PSTicks_Minimum, PSPS_ToDuplexHoldPoint)
	_Set_PS_SequenceID_Change(PSPS_ToDuplexHoldPoint)
}

void	PSP_CheckDis(PageSupervisor *PGSPVSR)
{
	_Set_T1(PSTicks_DisToHoldPoint_2 - PSTicks_7mm, PSPS_ToDuplexHoldPoint)
	_Set_PS_SequenceID_Change(PSPS_ToDuplexHoldPoint)
	// }
	// ChatteringForDisOn = CLEAR;
	// _Check_T1_and_RET
	
	// PaperJamInDuplex = JAM;
// abba will not use this func (AQU debug msg)		//17Apr09 yjlee		
//	TE_Debug_SetDebugData(InternalDuplex_Paper_Jam, PGSPVSR->PageID, 0x00, 0x00, 0x00, 0x00);
	// _Set_PS_SequenceID_Change(PSS_PrinterError)
}

void	PSP_ToDuplexHoldPoint(PageSupervisor *PGSPVSR)
{
	
	_Check_T1_and_RET

	PGSPVSR->Flags = PSPS_ToDuplexHoldPoint;

	_Set_T1(PSTicks_Minimum, PSPS_DuplexFrontPrintReady)
	_Set_PS_SequenceID_Change(PSPS_DuplexFrontPrintReady)
}

void	PSP_DuplexFrontPrintReady(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSPS_DuplexFrontPrintReady;
	
	_Check_T1_and_RET

	if(DuplexFrontSidePrint) 
	{
		DuplexFrontSidePrint = CLEAR;
	}
	else 
	{
		if(fStartExitMotor == SET) 
		{
			HoldingDuplex = SET;
		}
	}
	if(IsPaperPresentOnPins() == PRESENT) 
	{
		PaperJamOnPins = JAM;
		CodeOfJamStatus = JAM_DUPLEX_PATH;
		_Set_PS_SequenceID_Change(PSS_PrinterError)
	}
}

void	PSP_EndOfPrintPage(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
#if	ENG_TRACE		
	MSG("(Eng)Step_Check : PSP_EndOfPrintPage\n");
#endif	
	
	if(PGSPVSR->OutputDestination == OutputDuplexor) 
	{
		_Set_T2(NONE, PSS_NONE)
		return;
	}
	else 
	{
		if(Set_PrintCompleteNotify(PGSPVSR->PageID) == TRUE)
		{
#if KSR_DBG_ONE			
			printf("[PSV] cPageCount = %d cTotal_Print_Count = %d cMemoryWrite_Count = %d\n",cPageCount,cTotal_Print_Count, cMemoryWrite_Count);  /* [KSF050806] */
#endif			
			cPageCount++; /* [KSR032501] for SMC */
			cTotal_Print_Count++;
			cMemoryWrite_Count++;
			cPageCompleteCnt++; /* [KSR032912] */
			cPrintData++;
#if BLI_TEST
			if((cDuplexPageCount >= 3) || (cPageCompleteCnt >= 5))
			{
				Fuser_Control.LowSpeedFan_Wakeup = CLEAR;
			}
#endif
#if 0 /* [KSF061001] */ /* [KSR070405] */
			if(cMemoryWrite_Count >= 10)
			{
#if KSR_DBG_ONE				
				printf("[PSV] cMemoryWrite_Count = %d\n",cMemoryWrite_Count);  /* [KSF050806] */
#endif				
				cMemoryWrite_Count = 0;
				fSmartICWrite_DotCountInRun = TRUE;
				fMemoryWrite = TRUE;
			}
#endif			
			if(fMemoryWrite==TRUE)
			{
				fSmartICWrite_PageCountInRun = TRUE;
				fSmartICWrite_PageCount = TRUE;
				fSmartICWrite_DrumPageCountInRun = TRUE; /* [KSR032912] */
				fSmartICWrite_DrumPageCount = TRUE; /* [KSR032912] */
			}
			
			if((fPwsToPrint == SET) && (fPaperLengthIsNarrow == CLEAR) && (fPwsToPrint_1stPick == SET))
			{
				fPwsToPrint_1stPick = CLEAR;
				_Set_T2(PSTicks_MotorLock, PSPS_MotorWarmupForPwsToPrint)
				return;
			}
			else
			{
				fSpeedUpForPWS = CLEAR;
				_Set_T2(NONE, PSS_NONE)
				return;
			}
		}
	}
	_Set_T2(PSTicks_Minimum, PSPS_EndOfPrintPage)
}

/* ==========  Paper Exit Sequence  ========== */

/* Paper Transfer To Hopper or Rear Cover */
void	PSP_StartExtendedFeed(PageSupervisor *PGSPVSR)
{
	MESSAGE message;
	printer_setup_t printer_setup;
#if	ENG_TRACE	
	MSG("(Eng)Step_Check : PSP_StartExtendedFeed\n");
#endif
	MSG("(Eng)fNoPaperOnTray = 0x%x \n",PGSPVSR->fNoPaperOnTray);
	
	if((PGSPVSR->fNoPaperOnTray != FALSE) && (PGSPVSR->CleaningTime_NoVideo == CLEAR))
	{
		printvar_get_printersetup( &printer_setup );
		lastPaperOutTray = PGSPVSR->fNoPaperOnTray;
		MSG("(Eng)lastPaperOutTray = 0x%x \n",lastPaperOutTray);
		
		switch(PGSPVSR->fNoPaperOnTray)
		{
			case MS_Tray1:
				//engine_state.paper_out_tray1 = true;
				printer_setup.inputTrays[1].paperPresent = false;
				slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY1 );
				break;
			case MS_Tray2:
				//engine_state.paper_out_tray2 = true;
				printer_setup.inputTrays[2].paperPresent = false;
				slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY2 );
				break;
			case MS_Tray3:
				//engine_state.paper_out_tray3 = true;
				printer_setup.inputTrays[3].paperPresent = false;
				slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY3 );
				break;
			case MS_MultiPurposeTray:
				//engine_state.paper_out_tray4 = true;
				printer_setup.inputTrays[0].paperPresent = false;
				slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY4 );
				break;
		}
		message.msgType = MSG_STATUSUPDATE;
		SYMsgSend(PRINTMGRID, &message);
		MSG("(Eng)Send MSG_STATUSUPDATE <%x>\n",message.param1);
		printvar_set_printersetup( &printer_setup );
		 
		PGSPVSR->fNoPaperOnTray = FALSE;
		_AddTime_T1(PSTicks_5s, PSPS_EmptyCheckWaiting)
		_Set_PS_SequenceID_Change(PSPS_EmptyCheckWaiting)
	}
	else
	{
		if(PGSPVSR->CleaningTime_NoVideo == SET)
		{
			PGSPVSR->CleaningTime_NoVideo = CLEAR;
			MSG("(eng)CleaningTime_NoVideo is CLEAR\n");
		}
		_AddTime_T1(PSTicks_Minimum + 2, PSPS_ToHopper)
		_Set_PS_SequenceID_Change(PSPS_ToHopper)
	}
}

void 	PSP_EmptyCheckWaiting(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	_AddTime_T1(PSTicks_Minimum + 2, PSPS_ToHopper)
	_Set_PS_SequenceID_Change(PSPS_ToHopper)	
}

void	PSP_ToHopper(PageSupervisor *PGSPVSR)
{
	/* Wait for starting EP Cleaning process */
	if(PGSPVSR->T2_State != PSS_NONE) 
	{
		_Check_T1_and_RET
	}
	_Check_T1_and_RET
	
	if(PGSPVSR->T2_State != PSS_NONE) return;
#if	ENG_TRACE		
	MSG("(Eng)Step_Check : PSP_ToHopper\n");
#endif		
	if (PGSPVSR->Flags != (Uint8)PSPS_ToHopper)
	{
		PGSPVSR->Flags = PSPS_ToHopper ;
		return ;
	}
	_AddTime_T1(PSTicks_Minimum, PSS_StartEPCleaning)
	_Set_PS_SequenceID_Change(PSS_StartEPCleaning)
}

/* ==========  End Of Process Sequence  ========== */
/* EP Cleaning Sequence */
void	PS_StartEPCleaning(PageSupervisor *PGSPVSR)
{
	//MESSAGE message;
	
	if(fPOR_PageSupervisor_Act) 
	{
		if(Detect_OverSensorAtWarmup()) 
		{
			if(fJamDetacted1st == TRUE)
			{
				return;
			}
			_Set_PS_SequenceID_Change(PSS_PrinterError)
		}
	}
	_Check_T1_and_RET
	
	PGSPVSR->Flags = PSS_StartEPCleaning;
	if(fPOR_PageSupervisor_Act) 
	{
		fPOR_PageSupervisor_EPCleaning = SET;
	}
#if	ENG_TRACE	
	MSG("(Eng)PS_StartEPCleaning <%d>\n",cProcessCounter);
#endif	
	EPtimingBUFF_Old = cProcessCounter;
	EPtimingBUFF_New = cProcessCounter;
	
	StartTransferCleaning();

	if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T1(PSTicks_XfrToMotorOff_L, PSS_EPCleaning)}
	else 										{_Set_T1(PSTicks_XfrToMotorOff, PSS_EPCleaning)}
	
	_Set_PS_SequenceID_Change(PSS_EPCleaning)
}

void	PS_ChargeCleaning(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET

	/* Stat the cleaning process for charge roll */
	StartChargeCleaning();
#if	ENG_TRACE
	MSG("(Eng)Step_Check : PS_ChargeCleaning\n");
#endif	
//08May09 yjlee
/*
	_Set_T1(PSTicks_CleaningTime, PSS_EPCleaning)
	_Set_PS_SequenceID_Change(PSS_EPCleaning)
*/	
	_Set_T1(PSTicks_ChargeToMotorOff, PSS_StopMainMotor)
	_Set_PS_SequenceID_Change(PSS_StopMainMotor)	
}

void	PS_EPCleaning(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	Set_TransferBiasInterpageGap(FALSE);
#if	ENG_TRACE	
	MSG("(Eng)PS_EPCleaning <%d>\n",cProcessCounter);
#endif	
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T7 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	// _Set_T1(PSTicks_CleaningTime, PSS_StopMainMotor)
	if(fMotorSpeedMode == MTR_MODE_QUIET)		{_Set_T1(PSTicks_TxfIPTime_L, PSS_StopMainMotor)}
	else 										{_Set_T1(PSTicks_TxfIPTime, PSS_StopMainMotor)}
	
	_Set_PS_SequenceID_Change(PSS_StopMainMotor)
}

void	PS_Dev_Off(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	/* Set Developer Bias to cleaning level */
	StartDeveloperCleaning();

	_Set_T1(PSTicks_CleaningTime, PSS_StopMainMotor)
	_Set_PS_SequenceID_Change(PSS_StopMainMotor)
}

void	PS_StopMainMotor(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	
	if(fStaticMotorRunTime) 
	{
		if(((HeatingStartTemperature >= 160) && (fMotorRunFromPowerSave != TRUE)) || ((HeatingStartTemperature >= 125) && (fMotorRunFromPowerSave == TRUE)))
		{
//				printf("Over deg Case : ProcessCounter <%d> MotorStartCounter <%d>, StartTemp <%d>\n"
//				,cProcessCounter, cPORMotorStartCounter, HeatingStartTemperature);
		}
		else
		{
			if((cProcessCounter - cPORMotorStartCounter) <= STATIC_MOTOR_RUN_TIME_UNDER_TEMP)
			{
//				printf("Under deg Case : ProcessCounter <%d> MotorStartCounter <%d>, StartTemp <%d>\n"
//				,cProcessCounter, cPORMotorStartCounter, HeatingStartTemperature);
				return;
			}
		}
	}
	MSG("(eng)ProcessCounter <%d>, MotorStartCounter <%d>, StartTemp <%d>, fMotorRunFromPowerSave = %d\n"
		, cProcessCounter, cPORMotorStartCounter, HeatingStartTemperature, fMotorRunFromPowerSave);

	if(fPowerOnReset == SET)
	{
		fPowerOnReset = CLEAR;
		if(bePrintFromPowerSave == CLEAR)
		{
			Set_PrintReadyNotify();
		}
	}
	bePrintFromPowerSave = CLEAR;

//#endif
	PGSPVSR->Flags = PSS_StopMainMotor;
	/* [KSR032912] */
	if((cPrintData == CLEAR) && (Get_MainMotorState() == RUNNING))/* [KSR032912] */
	{
#ifdef KSR_DBG_TWO
//	printf("<<%s>>\n",__func__); /* [KSR040201] */
#endif
		cFreeMotorRunCheck_ForTonerCount++;
		cFreeMotorOn++;
		cDrumFreeMotorOn++;
		fSmartICWrite_FreeMotorOn = SET;
		fSmartICWrite_DrumFreeMotorOn = SET;
		
		if(Fuser_Control.EM_State != EMS_JogMode)
		{
			JogModeEnvIdx = JOG_MODE_L;
			FanCtrlEnvIdx = FAN_CTRL_L;
		}
	}
	else
	{
#if KSR_DBG_THREE
	printf("<<%s>> cPageCompleteCnt:%d fMode_Job:%d\n",__func__ ,cPageCompleteCnt ,fMode_Job); /* [KSR040201] */ /* [KSF050806] */
#endif
		if(cPageCompleteCnt == 1) fMode_Job = 0;
		else if(cPageCompleteCnt == 2) fMode_Job = 1;
		else if(cPageCompleteCnt == 3) fMode_Job = 2;
		else if(cPageCompleteCnt == 4) fMode_Job = 3;
		else if(cPageCompleteCnt == 5) fMode_Job = 4;
		else if((cPageCompleteCnt >= 6) && (cPageCompleteCnt <= 10)) fMode_Job = 5;
		else if((cPageCompleteCnt >= 11) && (cPageCompleteCnt <= 20)) fMode_Job = 6;
		else if((cPageCompleteCnt >= 21) && (cPageCompleteCnt <= 50)) fMode_Job = 7;
		else if(cPageCompleteCnt >= 50) fMode_Job = 8;
		else {}
		(Mode_Job[fMode_Job])++;
		Mode_Job[NUM_TONER_JOB-1]++; //total job count
		//printf("(eng)cPageCompleteCnt = %d, Mode_Job[%d] = %d, Mode_Job[12] = %d\n", cPageCompleteCnt, fMode_Job, Mode_Job[fMode_Job], Mode_Job[12]);
		(Drum_Mode_Job[fMode_Job])++;
		Drum_Mode_Job[NUM_TONER_JOB-1]++; //Drum total job count
		//printf("(eng)cPageCompleteCnt = %d, Drum_Mode_Job[%d] = %d, Drum_Mode_Job[12] = %d\n", cPageCompleteCnt, fMode_Job, Drum_Mode_Job[fMode_Job], Drum_Mode_Job[12]);
		SmartICWrite_TonerHisJobADRESS = fMode_Job + SC_TONER_HIS_MODE_JOB_1_ADDR;
		SmartICWrite_TonerHisJob = Mode_Job[fMode_Job];
		SmartICWrite_DrumHisJob = Drum_Mode_Job[fMode_Job];
		fSmartICWrite_TonerHisJob = SET;
		fSmartICWrite_DrumHisJob = SET;
		
		//printf("## Toner Job by mode (Eng) %d %d %d %d %d %d %d %d %d %d ##\n",Mode_Job[0], Mode_Job[1], Mode_Job[2], Mode_Job[3], Mode_Job[4], Mode_Job[5], Mode_Job[6], Mode_Job[7], 
		//Mode_Job[8], Mode_Job[9]);
	}
	MESSAGE message;
	
	message.msgType = MSG_CARTRIDGE_PRINT_JOB_MODE_INFO;
	message.param1 = cFreeMotorOn;
	message.param3 = (int*)Mode_Job;
		
	SYMsgSend(PRINTMGRID, &message);
		
	cPageCompleteCnt = 0;
	cPrintData = 0;
	
	StopMainMotor();
	Set_TransferBiasInterpageGap(FALSE);
#if	ENG_TRACE	
	MSG("(Eng)PS_StopMainMotor <%d>\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T8 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	Set_PaperMediaSizeInTray(MS_Unknown);
	
	fPaperIsEmptyBeforeMotorRun = CLEAR;
	
	JogModeCountFor24v = CLEAR;
	if(fMainMotorJogMode) 
	{
		_Set_T1(PSTicks_StopJogMode, PSS_StopJogMode)
		_Set_PS_SequenceID_Change(PSS_StopJogMode)
	}
	else 
	{
		JogModeTimeIdx = CLEAR;
		if((fPOR_PageSupervisor_Act == CLEAR) && (cTotalPrintedPageForMotorBWD >= VALUEOFPRINTEDPAGECOUNTER))
		{
			cTotalPrintedPageForMotorBWD = CLEAR;
			_Set_T2(PSTicks_180ms, PSS_RunMainMotorForJobFinish)
		}
		_Set_T1(PSTicks_StopMotorTime, PSS_FinishEPCleaning)
		_Set_PS_SequenceID_Change(PSS_FinishEPCleaning)
	}

}

void	PS_RunMainMotorForJobFinish(PageSupervisor *PGSPVSR)
{
	_Check_T2_and_RET
	
	cRunMainMotorForJobFinish++;
//	MSG("(Eng)cRunMainMotorForJobFinish = %d(%d)\n", cRunMainMotorForJobFinish, cProcessCounter);
	
	if(cRunMainMotorForJobFinish == 1)									// BWD 10ms
	{
		Motor_BWD();
		StartMainMotor();
	}
	if(cRunMainMotorForJobFinish == 4)									// STOP 200ms
	{
		StopMainMotor();
	}
	if(cRunMainMotorForJobFinish == 22)									// STOP 200ms
	{
		StartMainMotor();
	}
	if(cRunMainMotorForJobFinish == 25)									// STOP
	{
		StopMainMotor();
		Motor_FWD();
		
		cRunMainMotorForJobFinish = CLEAR;
		MSG("(eng)Finish : PS_RunMainMotorForJobFinish (%d)\n", cProcessCounter);
		_Set_T2(NONE, PSS_NONE)
	}
	
	return;
}

void	PS_FinishEPCleaning(PageSupervisor *PGSPVSR)
{
	PGSPVSR->Flags = PSS_FinishEPCleaning;
	_Check_T1_and_RET
#if	ENG_TRACE	
	MSG("(Eng)PS_FinishEPCleaning <%d>\n",cProcessCounter);
#endif
	EPtimingBUFF_Old = EPtimingBUFF_New;
	EPtimingBUFF_New = cProcessCounter;
	MSG("(eng)EPtiming T9 : %d\n", EPtimingBUFF_New-EPtimingBUFF_Old);
	/* Turn off transfer and develper bias */
	FinishIdleProcess();
	DisableChargeBiasSignal();
	/*_Set_T1(PSTicks_4s, PSS_Idle)
	_Set_PS_SequenceID_Change(PSS_Idle)*/

	_Set_T1(PSTicks_Minimum, PSS_StopPrinterSequence)
	_Set_PS_SequenceID_Change(PSS_StopPrinterSequence)
}

void	PS_Idle(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	_Set_PS_SequenceID_Change(PSS_StopPrinterSequence)
}


void	PS_StopJogMode(PageSupervisor *PGSPVSR)
{
	_Check_T1_and_RET
	/* Developer PWM Stop */
	Set_Developer_PWM(DeveloperBiasPWMForStop);
	//DisableDevPWM();
	DisableChargeBiasSignal();
	Set_Transfer_PWM(1); //add by namja
	DisableTransferBiasSignal();

	_Set_T1(PSTicks_Minimum, PSS_StopPrinterSequence)
	_Set_PS_SequenceID_Change(PSS_StopPrinterSequence)
}

void	PS_StopPrinterSequence(PageSupervisor *PGSPVSR)
	/* Stop Printer Sequence */
{
	_Check_T1_and_RET
	
	if(PGSPVSR->T2_State != PSS_NONE) 
	{
		return ;
	}
	
	if(fMainMotorJogMode == SET) 
	{
		fMainMotorJogMode = CLEAR;
	}
	else 
	{
		Set_LSUOffNotify();
		StopMirrorMotor();	// should be changed 	//20Apr09 yjlee
		MSG("(Eng)StopMirrorMotor\n");
	}
	
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
	
	/*PGSPVSR->Flags = PSS_StopPrinterSequence;*/
	if(fPOR_PageSupervisor_Act) 
	{
		fPOR_PageSupervisor_Act = CLEAR;
		fPOR_PageSupervisor_EPCleaning = CLEAR;
	}
	_Set_T1(PSTicks_Minimum, PSS_StopSequence)
	_Set_PS_SequenceID_Change(PSS_StopSequence)
}

void	PS_StopSequence(PageSupervisor *PGSPVSR)
{
#if	ENG_TRACE /* [KSR032501] for SMC */
//	printf("(Eng)Step_Check : PS_StopSequence\n");
#endif
	if(fNewTonerCheck == TRUE)
	{
		fNewTonerCheck = CLEAR;
		fSmartICWrite_UsedCheck = TRUE;
	}
		
	// if(cMemoryWrite_Count != 0)
	// {
		// cMemoryWrite_Count = 0;
		// fMemoryWrite = TRUE;
		// fSmartICWrite_PageCount = TRUE;
	// }
#if SMART_IC_LOCK_ENABLE
	ftoner_count_write_for_coveropen = CLEAR;
#endif
	if(cMemoryWrite_Count != 0)
	{
		cMemoryWrite_Count = 0;
		fSmartICWrite_PageCountFailInRun = FALSE;
		fSmartICWrite_DotCountFailInRun = FALSE;
		fSmartICWrite_DotCount = TRUE;
		fSmartICWrite_DrumDotCount = TRUE; /* [KSR032912] */
		fMemoryWrite = TRUE;
		fSmartICWrite_PageCount = TRUE;
		fSmartICWrite_DrumPageCount = TRUE; /* [KSR032912] */
//		fSmartICWrite_TonerHisFactorRate = TRUE; /* [KSR032912] */
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
			fMemoryWrite = TRUE;
		}	
		
		if (fSmartICWrite_DrumPageCountFailInRun == TRUE) /* [KSR032912] */
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
			fSmartICWrite_DrumDotCount = TRUE;
			fMemoryWrite = TRUE;
		}	
	}
	
#if MAC_TYPE_MH
	if(fState_FullStack == SET)	
	{
		fFullStack_Error = SET;
		if(fFullStackMsgSend != SET)
		{
			full_stack_notify(FullStackSet);
		}
	}
#endif
}

void	PS_PrintSequenceError(PageSupervisor *PGSPVSR)
	/* Wait for End of Other Print Sequence */
{
	if (PGSPVSR->Flags != PSS_PrinterError)
		return ;
	
	Picking_PGSPVSR = NULL;
	_Set_T2(0, PSS_NONE)
	
	if (PaperJam.DR.BYTE != CLEAR)
		return ;
#if	ENG_TRACE
	MSG("(Eng)Step Check : PS_PrintSequenceError\n");
#endif	
	ClearOtherPageSupervisor(PGSPVSR) ;
	
	// Fuser_Control.fDuplexJob = CLEAR;
	fDuplexCooling = CLEAR;
	cKeepMotorRunForCooling = 0;
	
	_Set_T1(PSTicks_Minimum, PSS_ExitError)
	_Set_PS_SequenceID_Change(PSS_ExitError)
}

void	PS_ExitError(PageSupervisor *PGSPVSR)
{
	if((PGSPVSR->PageAhead == NULL) 
	|| (PGSPVSR->PageAhead->Active == NOT_ACTIVE)) 
	{
#if	ENG_TRACE		
		MSG("(Eng)Step Check : PS_ExitError\n");
#endif		
		_Set_T1(PSTicks_Minimum, PSS_StartEPCleaning)
		_Set_PS_SequenceID_Change(PSS_StartEPCleaning)
	}
}

void	PSP_OptionResync(PageSupervisor *PGSPVSR)
{
	MSG("(eng)Retry Option Detection In PS_OptionError (%d)\n", cOptionCommFail);
	cOptionCommFail++;
	if(cOptionCommFail <= 5)
	{
		Option_Tx_Sync(OPTION_T1);
		Option_Tx_Sync(OPTION_T2);
		return;
	}
	if(cOptionCommFail <= 7)
		return;

	cOptionCommFail = CLEAR;
	fDebugOptionCommFail = TRUE;
	f1stOptionResync = CLEAR;
	_Set_PS_SequenceID_Change(PSPS_StartPaperPickupSequence)
}

void	PS(PageSupervisor *PGSPVSR)
{
}

Uint8	Set_Laminate_PGSPVSR_Parameter(Uint8 id)
{
	PageSupervisor *now_PGSPVSR;
	Uint8 i, TargetTemp;
	
	for(i=0; i < MAX_PAGE_SUPERVISOR; i++) 
	{
		now_PGSPVSR = Get_PageSupervisorAddress(i);
	
		if(!(now_PGSPVSR->Active)) 
		{
			now_PGSPVSR->PageID = CLEAR;
			now_PGSPVSR->Source = MS_MultiPurposeTray;
			now_PGSPVSR->Narrow = CLEAR;
			now_PGSPVSR->MediaDescription.BYTE = DEFAULT_PAGE; /* Paper Normal Normal */
			now_PGSPVSR->Darkness = DARKNormal;
			now_PGSPVSR->TonerSaver = TONERSaverOFF;
			now_PGSPVSR->OutputDestination = OutputStandardOrRear;
			now_PGSPVSR->Length = CLEAR;
			now_PGSPVSR->page_info = NULL;
			
			Fuser_Control.Target_Temperature = FUSER_PARAM_TMP_LAMINATE;
			TargetTemp = Fuser_Control.Target_Temperature;
			
			now_PGSPVSR->FuserTemp = TargetTemp;
#if	ENG_TRACE			
			MSG("(Eng)##Fuser Target Temp Laminating : %d##\n",TargetTemp);
#endif			
			now_PGSPVSR->Flags = CLEAR;
			now_PGSPVSR->T1_State = CLEAR;
			now_PGSPVSR->T1_Value = CLEAR;
			now_PGSPVSR->T1_Comp = CLEAR;
			now_PGSPVSR->T2_State = CLEAR;
			now_PGSPVSR->T2_Value = CLEAR;
			now_PGSPVSR->SequenceID = id;
			now_PGSPVSR->PageAhead = (PageSupervisor *) NULL;
			now_PGSPVSR->Active = TRUE;
		
			return i+TopPageSupervisor;
		}
	}
	return 255;
}

Uint8	Set_POR_PGSPVSR_Parameter(Uint8 id)
	/* Initialize the member of structure for PageSupervisor parameter as sequence ID
	Return the factor number that the arrangement uses as return value 
	If there is not the domain that be set parameter ,return "255"
	*/
{
	PageSupervisor *now_PGSPVSR;
	Uint8 i, TargetTemp;
	
	for(i=0; i < MAX_PAGE_SUPERVISOR; i++) 
	{
		now_PGSPVSR = Get_PageSupervisorAddress(i);
	
		if(!(now_PGSPVSR->Active)) 
		{
			now_PGSPVSR->PageID = CLEAR;
			now_PGSPVSR->Source = CLEAR;
			now_PGSPVSR->Narrow = CLEAR;
			now_PGSPVSR->MediaDescription.BYTE = DEFAULT_PAGE; /* Paper Normal Normal */
			now_PGSPVSR->Darkness = DARKNormal;
			now_PGSPVSR->TonerSaver = TONERSaverOFF;
			now_PGSPVSR->OutputDestination = OutputStandardOrRear;
			now_PGSPVSR->Length = CLEAR;
			now_PGSPVSR->page_info = NULL;
		
			TargetTemp = Set_FuserTemp(MS_Tray1, OutputStandardOrRear, DEFAULT_PAGE, 0, 0);

			now_PGSPVSR->FuserTemp = TargetTemp;
#if	ENG_TRACE			
			MSG("(Eng)##Fuser Target Temp 1 : %d##\n",TargetTemp);
#endif			
			now_PGSPVSR->Flags = CLEAR;
			now_PGSPVSR->T1_State = CLEAR;
			now_PGSPVSR->T1_Value = CLEAR;
			now_PGSPVSR->T1_Comp = CLEAR;
			now_PGSPVSR->T2_State = CLEAR;
			now_PGSPVSR->T2_Value = CLEAR;
			now_PGSPVSR->SequenceID = id;
			now_PGSPVSR->PageAhead = (PageSupervisor *) NULL;
			now_PGSPVSR->Active = TRUE;
		
			return i+TopPageSupervisor;
		}
	}
	return 255;
}

Uint8	Set_PrintPageParameter(Uint8 id)
	/* Setting Print Page Parameter to Page Supervisor structure */
	/* If setting OK:		TRUE
		  nothing free space:	FALSE */
{
	PageSupervisor *pre_PGSPVSR = NULL;
	PageSupervisor *now_PGSPVSR;
	
//	MEDIA_DISCRPT FuserPaperData;
	Uint8 i, TargetTemp;
	PAGE_DATA* page;
	
	fClearErrorsCmdRcv = CLEAR;
	
	for(i=0; i< MAX_PAGE_SUPERVISOR; i++) 
	{
		now_PGSPVSR = Get_PageSupervisorAddress(i);
		
		if(now_PGSPVSR->Active) 
		{
			pre_PGSPVSR = now_PGSPVSR;
			continue;
		}
		/* PageSupervisor Parameter Setting */
		
		// if((pre_PGSPVSR->Active == TRUE) && (pre_PGSPVSR->PageID == now_PGSPVSR->PageID) && (pre_PGSPVSR->Source != now_PGSPVSR->Source))
		// {
			// MSG("@@@  pre_PGSPVSR->PageID = %d, now_PGSPVSR->PageID=%d, pre_PGSPVSR->Source = %d, now_PGSPVSR->Source = %d@@@\n",pre_PGSPVSR->PageID,now_PGSPVSR->PageID,pre_PGSPVSR->Source,now_PGSPVSR->Source);
			// TempSource = now_PGSPVSR->Source;
			// now_PGSPVSR = pre_PGSPVSR;
			// now_PGSPVSR->Source = TempSource;
		// }
		
		now_PGSPVSR->Active = TRUE; 
		if(fPendingPrintPageCmd == TRUE)
		{
			now_PGSPVSR->PageID = PendingPrintPageData[0];
// #if MAC_TYPE_MH
			// if((now_PGSPVSR->PageID == 1) && (fStandbyState == SET))
			// {
				// fStandbyState = CLEAR;
				// fFirstTenPages = SET;
			// }
// #endif
		}
		else
		{
			now_PGSPVSR->PageID = 0;
		}
#if 0
		if(PendingPrintPageData[1] == MS_Duplex)
		{
			if(IsPaperSizeNarrowInTray(pre_PGSPVSR->Source) == TRUE)
			{
				PendingPrintPageData[1] = pre_PGSPVSR->Source;
				MSG("(Eng)Papersize In Tray = 0x%x : Duplex Source Change 0x%x => 0x%x\n", PendingPrintPageData[1], MS_Duplex, PendingPrintPageData[1]);
			}
		}
		else
		{
			if(IsPaperSizeNarrowInTray(PendingPrintPageData[1]) == TRUE)
			{	
				if(PendingPrintPageData[7] == OutputDuplexor)
				{
					PendingPrintPageData[7] = OutputStandardOrRear;
					MSG("(eng)Papersize In Tray = 0x%x : OutputDestination Change 0x%x => 0x%x\n", PendingPrintPageData[1], OutputDuplexor, PendingPrintPageData[7]);
				}
			}
		}
#endif
		now_PGSPVSR->Source = PendingPrintPageData[1];
		now_PGSPVSR->Narrow = PendingPrintPageData[2];
		now_PGSPVSR->MediaDescription.BYTE = PendingPrintPageData[3];
		now_PGSPVSR->Length = PendingPrintPageData[4];	//(Uint32)PaperLength[PendingPrintPageData[4]] ;
		now_PGSPVSR->Darkness =  PendingPrintPageData[5];
		now_PGSPVSR->TonerSaver = PendingPrintPageData[6];
		now_PGSPVSR->OutputDestination = PendingPrintPageData[7];
		now_PGSPVSR->cheat_gap = PendingPrintPageData[8];
//		now_PGSPVSR->page_info = engine_state.pick_page;

		now_PGSPVSR->page_info = NULL;
		if((fPreRunBeforePrintCmd != SET) && (fMotorRunWithoutPrintCmd != SET))
		{
			now_PGSPVSR->page_info = (void*)PendingPrintPageData[10] ;
		}
		now_PGSPVSR->page_done_info = (void*)PendingPrintPageData[11] ;
		now_PGSPVSR->OptionPrePick = PendingPrintPageData[12];
		now_PGSPVSR->EmptySourceTray = PendingPrintPageData[13];
		now_PGSPVSR->RecycledPaperEnable = (now_PGSPVSR->MediaDescription.BIT.Type==MTYPE_Recycled)?SET:CLEAR;
		if(fPwsToPrint_1stPage == SET)		now_PGSPVSR->PowersaveToPrint = 1;
		else if(fPwsToPrint_2ndPage == SET)	now_PGSPVSR->PowersaveToPrint = 2;
		else 								now_PGSPVSR->PowersaveToPrint = 0;
		now_PGSPVSR->fvideodone = 0;
		now_PGSPVSR->fvideoOn = 0;
		now_PGSPVSR->CleaningTime_NoVideo = CLEAR;
		now_PGSPVSR->InputDetectTime = 0;
		now_PGSPVSR->InputReleaseTime = 0;
		now_PGSPVSR->ExitDetectTime = 0;
		now_PGSPVSR->ExitReleaseTime = 0;
		
		if((fPreRunBeforePrintCmd != SET) && (fMotorRunWithoutPrintCmd != SET))
		{
			page = (PAGE_DATA*)now_PGSPVSR->page_info;
		}
		
		
#if MAC_TYPE_MH
		if((fPendingPrintPageCmd == TRUE) || (fPreRunBeforePrintCmd == RECEIVED))
		{
			if((now_PGSPVSR->OutputDestination == OutputDuplexor) || (now_PGSPVSR->Source == MS_Duplex))
			{
				// if(fDuplexCooling == CLEAR)
				// {
					// fDuplexCooling = TRUE;
					Fuser_Control.fDuplexJob = 1;
				// }
			}
			else
			{
				Fuser_Control.fDuplexJob = CLEAR;
			}
		}
		else
		{
			Fuser_Control.fDuplexJob = CLEAR;
		}
#if	ENG_TRACE			
		MSG("(Eng)## Set_PrintPageParameter : Fuser_Control.fDuplexJob = %d <%p> ##\n",Fuser_Control.fDuplexJob, now_PGSPVSR->page_info);
#endif
#endif		
		if((fMotorRunWithoutPrintCmd == SET) && (PrintPageCmd == CLEAR)) 
		{
			fMotorRun = CLEAR;
			TargetTemp = Temp_173C;
		}
		else if((fPreRunBeforePrintCmd == RECEIVED) && (fPendingPrintPageCmd != RECEIVED))
		{
			fWaitPrintPageCmd = SET;
#if MAC_TYPE_MH			
			TargetTemp = Temp_190C;
			Fuser_Control.Target_Temperature = Temp_190C;
#else			
			TargetTemp = Temp_185C;
			Fuser_Control.Target_Temperature = Temp_185C;
#endif
			
#if	ENG_TRACE		
			MSG("(Eng)## fPreRunBeforePrintCmd is received : Target Temp = %d##\n",TargetTemp);
#endif				
		}
		else 
		{
			TargetTemp = Set_FuserTemp(PendingPrintPageData[1], PendingPrintPageData[7], PendingPrintPageData[3], page->PageCount/*now_PGSPVSR->PageID*/,now_PGSPVSR->PowersaveToPrint);
		}
		
		now_PGSPVSR->FuserTemp = TargetTemp;
#if	ENG_TRACE		
		MSG("(Eng)##Fuser Target Temp 2 : %d##\n",TargetTemp);
#endif		
		MSG("(eng)Set_PrintPageParameter : PageID = %d (SqID=%d) (%d)\n", now_PGSPVSR->PageID, id, cProcessCounter);
		now_PGSPVSR->Flags = PSS_NONE;
		
		now_PGSPVSR->T1_State = 0;
		now_PGSPVSR->T1_Value = 0;
		now_PGSPVSR->T1_Comp = 0;
		
		now_PGSPVSR->T2_State = 0;
		now_PGSPVSR->T2_Value = 0;
		
		now_PGSPVSR->SequenceID = id;	
		now_PGSPVSR->PageAhead = pre_PGSPVSR;
		
		if(fPreRunBeforePrintCmd == SET)
		{
			fNoCheckTraySourceForPreRun = SET;
			fPaperLengthIsNarrow = CLEAR;
#if DUPLEX_SLOW_MODE
			fDuplexPrintSlowlyFor15min = CLEAR;
#endif
#if DUPLEX_SLOW_MODE2
			fDuplexPrintSlowlyForServoHH = CLEAR;
#endif
		}
		
		if(id == PSPS_StartEPCleaning)
		{
			if(now_PGSPVSR->PageAhead != NULL)
			{
				MSG("(eng)Start Cleaning Time\n");
				now_PGSPVSR->PageAhead->fNoPaperOnTray = MS_Tray1;
				now_PGSPVSR->PageAhead->CleaningTime_NoVideo = SET;
				fRequest_New_PageInfo = SET;
			}
		}
		
		fPendingPrintPageCmd = CLEAR;
		fPreRunBeforePrintCmd = CLEAR;
		return TRUE;
	}
	return FALSE;
}

Uint8	ChangePrintSequence(Uint8 id)
{
	PageSupervisor *now_PGSPVSR;
	Uint8 TargetTemp;
	PAGE_DATA* page;
	
	now_PGSPVSR = Get_TopPageSupervisor();
	if(fPendingPrintPageCmd == TRUE) 
	{
		if(now_PGSPVSR->SequenceID == PSPS_FinishInit) 
		{
			/* PageSupervisor Parameter Setting */
			now_PGSPVSR->PageID = PendingPrintPageData[0];
			now_PGSPVSR->Source = PendingPrintPageData[1];
			now_PGSPVSR->Narrow = PendingPrintPageData[2];
			now_PGSPVSR->MediaDescription.BYTE = PendingPrintPageData[3];
			now_PGSPVSR->Length = PendingPrintPageData[4];	//(Uint32)PaperLength[PendingPrintPageData[4]] ;
			now_PGSPVSR->Darkness =  PendingPrintPageData[5];
			now_PGSPVSR->TonerSaver = PendingPrintPageData[6];
			now_PGSPVSR->OutputDestination = PendingPrintPageData[7];
			now_PGSPVSR->cheat_gap = PendingPrintPageData[8];
			now_PGSPVSR->OptionPrePick = PendingPrintPageData[12];
			now_PGSPVSR->EmptySourceTray = PendingPrintPageData[13];
			now_PGSPVSR->RecycledPaperEnable = (now_PGSPVSR->MediaDescription.BIT.Type==MTYPE_Recycled)?SET:CLEAR;
			if(fPwsToPrint_1stPage == SET)		now_PGSPVSR->PowersaveToPrint = 1;
			else if(fPwsToPrint_2ndPage == SET)	now_PGSPVSR->PowersaveToPrint = 2;
			else 								now_PGSPVSR->PowersaveToPrint = 0;
			now_PGSPVSR->fvideodone = 0;
			now_PGSPVSR->fvideoOn = 0;
			now_PGSPVSR->CleaningTime_NoVideo = CLEAR;
			now_PGSPVSR->InputDetectTime = 0;
			now_PGSPVSR->InputReleaseTime = 0;
			now_PGSPVSR->ExitDetectTime = 0;
			now_PGSPVSR->ExitReleaseTime = 0;
			
			page = (PAGE_DATA*)now_PGSPVSR->page_info;
			
			TargetTemp = Set_FuserTemp(PendingPrintPageData[1], PendingPrintPageData[7], PendingPrintPageData[3], page->PageCount/*PendingPrintPageData[0]*/, now_PGSPVSR->PowersaveToPrint);

			now_PGSPVSR->FuserTemp = TargetTemp;
#if	ENG_TRACE			
			MSG("(Eng)##Fuser Target Temp 3 : %d##\n",TargetTemp);
#endif			
			now_PGSPVSR->Flags = PSS_NONE;

			now_PGSPVSR->T1_State = 0;
			now_PGSPVSR->T1_Value = 0;
			now_PGSPVSR->T1_Comp = 0;

			now_PGSPVSR->T2_State = 0;
			now_PGSPVSR->T2_Value = 0;

			now_PGSPVSR->SequenceID = id;	
			now_PGSPVSR->PageAhead = NULL;
		
			fPendingPrintPageCmd = CLEAR;
//			MSG("(Eng)$$$RequestState : ST_PRINT$$$");
// #if MAC_TYPE_MH	
			// SubFan_On();
// #endif
			Fuser_Control.RequestState = ST_PRINT;
			return TRUE;
		}
	}
	return FALSE;
}

Uint8	ChangePrintPageInfo(void)
{
	PageSupervisor *now_PGSPVSR;
	Uint8 TargetTemp;
	PAGE_DATA* page;
	
	now_PGSPVSR = Get_TopPageSupervisor();
	if(fWaitPrintPageCmd == TRUE) 
	{
		/* PageSupervisor Parameter Setting */
		now_PGSPVSR->PageID = PendingPrintPageData[0];
		now_PGSPVSR->Source = PendingPrintPageData[1];
		now_PGSPVSR->Narrow = PendingPrintPageData[2];
		now_PGSPVSR->MediaDescription.BYTE = PendingPrintPageData[3];
		now_PGSPVSR->Length = PendingPrintPageData[4];//(Uint32)PaperLength[PendingPrintPageData[4]] ;
		now_PGSPVSR->Darkness =  PendingPrintPageData[5];
		now_PGSPVSR->TonerSaver = PendingPrintPageData[6];
		now_PGSPVSR->OutputDestination = PendingPrintPageData[7];
		now_PGSPVSR->cheat_gap = PendingPrintPageData[8];
		now_PGSPVSR->page_info = NULL;
		now_PGSPVSR->page_info = (void*)PendingPrintPageData[10] ;
		now_PGSPVSR->page_done_info = (void*)PendingPrintPageData[11] ;
		now_PGSPVSR->OptionPrePick = PendingPrintPageData[12];
		now_PGSPVSR->EmptySourceTray = PendingPrintPageData[13];
		now_PGSPVSR->RecycledPaperEnable = (now_PGSPVSR->MediaDescription.BIT.Type==MTYPE_Recycled)?SET:CLEAR;
		if(fPwsToPrint_1stPage == SET)		now_PGSPVSR->PowersaveToPrint = 1;
		else if(fPwsToPrint_2ndPage == SET)	now_PGSPVSR->PowersaveToPrint = 2;
		else 								now_PGSPVSR->PowersaveToPrint = 0;
		now_PGSPVSR->fvideodone = 0;
		now_PGSPVSR->fvideoOn = 0;
		now_PGSPVSR->CleaningTime_NoVideo = CLEAR;
		now_PGSPVSR->InputDetectTime = 0;
		now_PGSPVSR->InputReleaseTime = 0;
		now_PGSPVSR->ExitDetectTime = 0;
		now_PGSPVSR->ExitReleaseTime = 0;
		
		page = (PAGE_DATA*)now_PGSPVSR->page_info;
		
		TargetTemp = Set_FuserTemp(PendingPrintPageData[1], PendingPrintPageData[7], PendingPrintPageData[3], page->PageCount/*PendingPrintPageData[0]*/, now_PGSPVSR->PowersaveToPrint);
		if(now_PGSPVSR->T1_State >= PSPS_FinishInit)
		{
#if	ENG_TRACE	
			MSG("(Eng) ## ChangePrintPageInfo : DEVELOPER PWM MODIFY <%d, %d> ##  \n",PSPS_FinishInit,now_PGSPVSR->T1_State);
#endif				
			Set_DeveloperBiasForPrinting(now_PGSPVSR->Darkness, now_PGSPVSR->TonerSaver);
		}
		else
		{
#if	ENG_TRACE	
			MSG("(Eng) ## ChangePrintPageInfo : DO NOTHING <%d, %d> ##  \n",PSPS_FinishInit,now_PGSPVSR->T1_State);
#endif			
		}
#if	ENG_TRACE			
		MSG("(Eng)## ChangePrintPageInfo : now_PGSPVSR->FuserTemp = %d, TargetTemp = %d ##\n",now_PGSPVSR->FuserTemp,TargetTemp);
#endif
		
#if MAC_TYPE_MH
		if((now_PGSPVSR->OutputDestination == OutputDuplexor) || (now_PGSPVSR->Source == MS_Duplex))
		{
			// if(fDuplexCooling == CLEAR)
			// {
				// fDuplexCooling = TRUE;
				Fuser_Control.fDuplexJob = 1;
			// }
		}
		else
		{
			Fuser_Control.fDuplexJob = CLEAR;
		}
#endif	
#if	ENG_TRACE			
		MSG("(Eng)## ChangePrintPageInfo : Fuser_Control.fDuplexJob = %d <%d>##\n",Fuser_Control.fDuplexJob, cProcessCounter);
#endif
		
// #if MAC_TYPE_MH	
		// SubFan_On();
// #endif
		if(now_PGSPVSR->FuserTemp == TargetTemp)
		{
			Fuser_Control.RequestState = ST_PRINT;
		}
		else
		{
//#if MAC_TYPE_MH			
			Fuser_Control.RequestState = ST_PRINT;
//#else 			
//			Fuser_Control.RequestState = ST_STANDBY;
//			fChangeTargetTemp = SET;
//#endif
		}
		
		now_PGSPVSR->FuserTemp = TargetTemp;
		
		now_PGSPVSR->Flags = PSS_NONE;
		now_PGSPVSR->PageAhead = NULL;
		
		fWaitPrintPageCmd = CLEAR;
		fPendingPrintPageCmd = CLEAR;
		return TRUE;
	}
	return FALSE;
}

Uint8	Set_MotorRunPageParameter(void)
	/* Setting Print Page Parameter to Page Supervisor structure */
	/* If setting OK:		TRUE
		  nothing free space:	FALSE */
{
	PageSupervisor *pre_PGSPVSR = NULL;
	PageSupervisor *now_PGSPVSR;
	
	Uint8 i;
	
	for(i=0; i< MAX_PAGE_SUPERVISOR; i++) 
	{
		now_PGSPVSR = Get_PageSupervisorAddress(i);
		
		if(now_PGSPVSR->Active) 
		{
			pre_PGSPVSR = now_PGSPVSR;
			continue;
		}
		/* PageSupervisor Parameter Setting */
		now_PGSPVSR->Active = TRUE;
		now_PGSPVSR->Flags = PSS_NONE;
		
		now_PGSPVSR->T1_State = 0;
		now_PGSPVSR->T1_Value = 0;
		now_PGSPVSR->T1_Comp = 0;
		
		now_PGSPVSR->T2_State = 0;
		now_PGSPVSR->T2_Value = 0;
		
		if(fMainMotorJogMode == SET) 
		{
			now_PGSPVSR->SequenceID = PSS_StartJogMode;	
		}
		else 
		{
		        now_PGSPVSR->SequenceID = PSS_MotorRun_Start;	
		}
		now_PGSPVSR->PageAhead = pre_PGSPVSR;	
		return TRUE;
	}
	return FALSE;
}

Uint8	Save_EmptySourceID(void)
	/* Return ID of tray detects Media Empty */
{
	if (fPaperEmptyForMPF == 1)
	{
		SavedRealMediaSizeInMPF = MS_Unknown;
		return (MS_MultiPurposeTray) ;
	}
	
	if (fPaperEmptyForT1 == 1)
	{
		SavedRealMediaSizeInTray1 = MS_Unknown;
		return (MS_Tray1) ;
	}
	
	if (fPaperEmptyForT2 == 1)
	{
		SavedRealMediaSizeInTray2 = MS_Unknown;
		return (MS_Tray2) ;
	}
	
	if (fPaperEmptyForT3 == 1)
	{
		SavedRealMediaSizeInTray3 = MS_Unknown;
		return (MS_Tray3) ;
	}
	return ((Uint8)0) ;
}

PageSupervisor *Get_MatchToSequenceID(Uint8 id)
	/* Return the address of PageSupervisor structure parameter to match the SequenceID in the active it. */
	/* NULL address is returned not to match SequenceID */
{
	PageSupervisor *now_PGSPVSR;
	Uint8 i;
	
	for(i=0; i<MAX_PAGE_SUPERVISOR; i++) 
	{
		now_PGSPVSR = Get_PageSupervisorAddress(i);
		if(now_PGSPVSR->Active == ACTIVE) 
		{
			if(now_PGSPVSR->SequenceID == id)
				return now_PGSPVSR;
		}
		else
			break;
	}
	return (PageSupervisor *)NULL;
}

Uint8	Get_TopPageSupervisorSequenceID(void)
	/* Get the SequenceID of top page on PageSupervisor */
	/* FALSE is returned for No Active Pagesupervisor. */
{

	if(PGSupervisor[TopPageSupervisor].Active == FALSE)
		return FALSE;
	return(PGSupervisor[TopPageSupervisor].SequenceID);
}

PageSupervisor *Get_TopPageSupervisor(void) 
{
	PageSupervisor *ptr;
	ptr = &PGSupervisor[TopPageSupervisor];
	return(ptr);
}

PageSupervisor *Get_LastPageSupervisor(void) 
{
	PageSupervisor *ptr;
	Uint8 i;

	ptr = 0;

	for(i = 0; i < MAX_PAGE_SUPERVISOR; i++) 
	{ 
		if((&PGSupervisor[(TopPageSupervisor + i)%MAX_PAGE_SUPERVISOR])->Active == FALSE) 
		{
			ptr = &PGSupervisor[(TopPageSupervisor + i - 1)%MAX_PAGE_SUPERVISOR];
			break;
		}
	}
	return ptr;
}

Uint8	Get_LastPageSupervisorDestination(void) 
{
	Uint8 i;

	for(i = 0; i < MAX_PAGE_SUPERVISOR; i++) 
	{ 
		if(PGSupervisor[(TopPageSupervisor + i)%MAX_PAGE_SUPERVISOR].Active == FALSE) 
		{
			return	PGSupervisor[(TopPageSupervisor + i - 1)%MAX_PAGE_SUPERVISOR].OutputDestination;
		}
	}
	return	0;
}

PageSupervisor *Get_NextVideoPageSupervisor(void)
{	
	Uint8 i, NextVideoPageSupervisor;
	PageSupervisor *ptr;
	
	for(i = 0; i < MAX_PAGE_SUPERVISOR; i++) 
	{
		if((PGSupervisor[i].Active == TRUE) && ((PGSupervisor[i].SequenceID == PSPS_NextVideoPage) || (PGSupervisor[i].SequenceID == PSPS_VideoOn)))
		{
			break;
		} 
	}
	if(i == 5)
	{
		NextVideoPageSupervisor = 0;
	}
	else
	{
		NextVideoPageSupervisor = i + 1;
	}
	ptr = &PGSupervisor[NextVideoPageSupervisor];
	return (ptr);
}

PageSupervisor *Get_NextPageSupervisor(void) 
{
	PageSupervisor *ptr;
	Uint8 NextPageSupervisor;
	if(TopPageSupervisor == 5) 
	{
		NextPageSupervisor = 0;
	}
	else 
	{
		NextPageSupervisor = TopPageSupervisor + 1; 
	}
	ptr = &PGSupervisor[NextPageSupervisor];
	return(ptr);
}

Uint8	HowManyActivePageSupervisor(void)
	/* Get the number of running page supervisor */
{
	PageSupervisor *now_PGSPVSR;
	Uint8 i, iNumber;
	
	iNumber = CLEAR;
	
	for(i=0; i < MAX_PAGE_SUPERVISOR; i++) 
	{
		now_PGSPVSR = Get_PageSupervisorAddress(i);
		if(now_PGSPVSR->Active) 
			iNumber++;
		else 
			break;
	}
	return iNumber;

}

PageSupervisor *	PageSupervisorAddress[MAX_PAGE_SUPERVISOR] = 
{
	&PGSupervisor[0],
	&PGSupervisor[1],
	&PGSupervisor[2],
	&PGSupervisor[3],
	&PGSupervisor[4],
	&PGSupervisor[5]
};

PageSupervisor *	Get_PageSupervisorAddress(Uint8 n)
	/* Get the address of the pagesupervisor structure No. n */
{
	return PageSupervisorAddress[(n + TopPageSupervisor)%MAX_PAGE_SUPERVISOR];
}

Uint32	Get_MeasuredPaperPath(Uint8 Source, Uint32 T1_Value)
	/* It returns the time of paper feeding from picked source to PINS. */
	/* Its length is reduced page gap and added solenoid response time */
	/* You should call "IsLastPage()" for storing the path length
	 before you use this function. */
{
	Uint32 PaperPathLength;
	
	switch(Source) 
	{
		case MS_MultiPurposeTray:
			PaperFeedTimeFromMPFBeforePins = T1_Value;
			if(fRetryPickUp == SET)
			{
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					PaperFeedTimeFromMPFBeforePins = FROM_MPF_TO_PIN_TIME;
				else
					PaperFeedTimeFromMPFBeforePins = FROM_MPF_TO_PIN_TIME_L;
			}
			SavedPaperPathMPFToPins	= PaperFeedTimeFromMPFBeforePins;
			
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
				if(T1_Value > PSEmpty_MPFToPins)
					EmptyWarningForMPF = ERROR;
			}
			else
			{
				if(T1_Value > PSEmpty_MPFToPins_L)
					EmptyWarningForMPF = ERROR;
			}
				
			PaperPathLength = PaperFeedTimeFromMPFBeforePins;
			/* In the case Label */
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				PaperPathLength -= /*PSTicks_SolenoidOnDelay*/  PSTicks_Gap22mm;
			else
				PaperPathLength -= /*PSTicks_SolenoidOnDelay*/  PSTicks_Gap22mm_L;
			// MSG("(Eng)@@@@@PaperFeedTimeFromMPFBeforePins = %d@@@@@\n", PaperFeedTimeFromMPFBeforePins);
			return PaperPathLength;

		case MS_Tray1:
			PaperFeedTimeFromT1BeforePins = T1_Value;
			// if(fRetryPickUp == SET)
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
				MSG("(eng)fRetryPickUp = %d, T1_Value = %d, LIMIT_TRAY1_TO_PIN_TIME = %d\n", fRetryPickUp, T1_Value, LIMIT_TRAY1_TO_PIN_TIME);
				if((fRetryPickUp == SET) || (T1_Value >= LIMIT_TRAY1_TO_PIN_TIME))
				{
					PaperFeedTimeFromT1BeforePins = FROM_TRAY1_TO_PIN_TIME;
				}
			}
			else
			{
				MSG("(eng)fRetryPickUp = %d, T1_Value = %d, LIMIT_TRAY1_TO_PIN_TIME_L = %d\n", fRetryPickUp, T1_Value, LIMIT_TRAY1_TO_PIN_TIME_L);
				if((fRetryPickUp == SET) || (T1_Value >= LIMIT_TRAY1_TO_PIN_TIME_L))
				{
					PaperFeedTimeFromT1BeforePins = FROM_TRAY1_TO_PIN_TIME_L;
				}
			}
			SavedPaperPathT1ToPins = PaperFeedTimeFromT1BeforePins;

			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
				if(T1_Value > PSEmpty_T1ToPins)
					EmptyWarningForTray1 = ERROR;
			}
			else
			{
				if(T1_Value > PSEmpty_T1ToPins_L)
					EmptyWarningForTray1 = ERROR;
			}
				
			PaperPathLength = PaperFeedTimeFromT1BeforePins;

			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				PaperPathLength -= /* + PSTicks_SolenoidOnDelay*/ PSTicks_Gap22mm;
			else
				PaperPathLength -= /* + PSTicks_SolenoidOnDelay*/ PSTicks_Gap22mm_L;
			// MSG("(Eng)@@@@@PaperFeedTimeFromT1BeforePins = %d@@@@@\n", PaperFeedTimeFromT1BeforePins);
			return PaperPathLength;
			
		case MS_Tray2:
			PaperFeedTimeFromT2BeforePins = T1_Value;
			// if(fRetryPickUp == SET)
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
				if((fRetryPickUp == SET) || (T1_Value >= LIMIT_TRAY2_TO_PIN_TIME))
				{
					MSG("(eng)fRetryPickUp = %d, T1_Value = %d, LIMIT_TRAY2_TO_PIN_TIME = %d\n", fRetryPickUp, T1_Value, LIMIT_TRAY2_TO_PIN_TIME);
					PaperFeedTimeFromT2BeforePins = FROM_TRAY2_TO_PIN_TIME;
				}
			}
			else
			{
				if((fRetryPickUp == SET) || (T1_Value >= LIMIT_TRAY2_TO_PIN_TIME_L))
				{
					MSG("(eng)fRetryPickUp = %d, T1_Value = %d, LIMIT_TRAY2_TO_PIN_TIME_L = %d\n", fRetryPickUp, T1_Value, LIMIT_TRAY2_TO_PIN_TIME_L);
					PaperFeedTimeFromT2BeforePins = FROM_TRAY2_TO_PIN_TIME_L;
				}
			}
			SavedPaperPathT2ToPins = PaperFeedTimeFromT2BeforePins;
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
				if(T1_Value > PSEmpty_T2ToPins)
					EmptyWarningForTray2 = ERROR;
			}
			else
			{
				if(T1_Value > PSEmpty_T2ToPins_L)
					EmptyWarningForTray2 = ERROR;
			}
				
			PaperPathLength = PaperFeedTimeFromT2BeforePins;
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				PaperPathLength -= /* + PSTicks_SolenoidOnDelay*/  PSTicks_Gap22mm;
			else
				PaperPathLength -= /* + PSTicks_SolenoidOnDelay*/  PSTicks_Gap22mm_L;
			return PaperPathLength;
					
		case MS_Tray3:
			PaperFeedTimeFromT3BeforePins = T1_Value;
			// if(fRetryPickUp == SET)
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
				if((fRetryPickUp == SET) || (T1_Value >= LIMIT_TRAY3_TO_PIN_TIME))
				{
					MSG("(eng)fRetryPickUp = %d, T1_Value = %d, LIMIT_TRAY3_TO_PIN_TIME = %d\n", fRetryPickUp, T1_Value, LIMIT_TRAY3_TO_PIN_TIME);
					PaperFeedTimeFromT3BeforePins = FROM_TRAY3_TO_PIN_TIME;
				}
			}
			else
			{
				if((fRetryPickUp == SET) || (T1_Value >= LIMIT_TRAY3_TO_PIN_TIME_L))
				{
					MSG("(eng)fRetryPickUp = %d, T1_Value = %d, LIMIT_TRAY3_TO_PIN_TIME_L = %d\n", fRetryPickUp, T1_Value, LIMIT_TRAY3_TO_PIN_TIME_L);
					PaperFeedTimeFromT3BeforePins = FROM_TRAY3_TO_PIN_TIME_L;
				}
			}
			SavedPaperPathT3ToPins = PaperFeedTimeFromT3BeforePins;
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
			{
				if(T1_Value > PSEmpty_T3ToPins)
					EmptyWarningForTray3 = ERROR;
			}
			else
			{
				if(T1_Value > PSEmpty_T3ToPins_L)
					EmptyWarningForTray3 = ERROR;
			}
				
			PaperPathLength = PaperFeedTimeFromT3BeforePins;
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				PaperPathLength -= /* + PSTicks_SolenoidOnDelay*/  PSTicks_Gap22mm;
			else
				PaperPathLength -= /* + PSTicks_SolenoidOnDelay*/  PSTicks_Gap22mm_L;
			return PaperPathLength;
				
		case MS_Duplex:
			PaperPathLength = T1_Value;
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				PaperPathLength = (PaperPathLength /* + PSTicks_SolenoidOnDelay*/ - PSTicks_Gap22mm);
			else
				PaperPathLength = (PaperPathLength /* + PSTicks_SolenoidOnDelay*/ - PSTicks_Gap22mm_L);
			PaperPathLength = SavedPaperPath;
			return PaperPathLength;
		
		default:
			return (Uint32)ERROR;
	}
}

Uint32	Get_MeasuredMediaSize(Uint8 Source)
	/* It returns the media length in the source to be picked */
{
	Uint8 SavedMediaSize = 0;
	
	switch(Source) 
	{
		case MS_MultiPurposeTray:
			SavedMediaSize = SavedRealMediaSizeInMPF;
			break;
		case MS_Tray1:
			SavedMediaSize = SavedRealMediaSizeInTray1;
			break;
		case MS_Tray2:
			SavedMediaSize = SavedRealMediaSizeInTray2;
			break;
		case MS_Tray3:
			SavedMediaSize = SavedRealMediaSizeInTray3;
			break;
		case MS_Duplex:
			SavedMediaSize = SavedRealMediaSizeInPrePage;
			break;
			
		default:
			break;
	}
#if	ENG_TRACE	
	MSG("(Eng)PaperLength[%d][%d] = %d  \n",fMotorSpeedMode, SavedMediaSize, PaperLength[fMotorSpeedMode][SavedMediaSize]);
#endif	
	return PaperLength[fMotorSpeedMode][SavedMediaSize];
}

Uint8	Get_PrePageMediaSize(Uint8 Source)
{
	switch(Source) 
	{
		case MS_MultiPurposeTray:
			return SavedRealMediaSizeInMPF;
			
		case MS_Tray1:
			return SavedRealMediaSizeInTray1;
		case MS_Tray2:
			return SavedRealMediaSizeInTray2;
		case MS_Tray3:
			return SavedRealMediaSizeInTray3;
		default:
			break;
	}
	return MS_Custom;
}

void	FinishPageSupervisor(PageSupervisor *PGSPVSR)
	/* Finish the page control sequence */
{
	PGSPVSR->Active = NOT_ACTIVE;
	/* Page Supervisor is Top Page Supervisor */
	if(PGSPVSR == Get_PageSupervisorAddress(0)) 
	{
		/* Increment the ID of Top Page Supervisor
	 	It indecates the ID of the Top of page supervisor structure */
		TopPageSupervisor++;
		if(TopPageSupervisor > (MAX_PAGE_SUPERVISOR - 1)) 
		{
			TopPageSupervisor = 0;
		}		
	}
}

void	ClearAllPageSupervisor(void)
	/* Clear the control of all page supervisor */
{
	Uint8	i;

	for(i=0; i<MAX_PAGE_SUPERVISOR; i++) 
		PGSupervisor[i].Active = NOT_ACTIVE;
	/* Initialize the flag to indicate that the sensor is used */
	PinsIsUsedByOtherPageSupervisor = NOT_USED;
	PexsIsUsedByOtherPageSupervisor = NOT_USED;
	DisIsUsedByOtherPageSupervisor = NOT_USED;
	
	DuplexFrontSidePrint = CLEAR;
	HoldingDuplex = CLEAR;
	MSG("(Eng)## All PageSupervisor Cleared ##\n");
	fPendingPrintPageCmd = CLEAR;
}

void	ClearOtherPageSupervisor(PageSupervisor *PGSPVSR)
	/* Cancel the control of the page supervisors except oneself */
{
	PageSupervisor	*now_ptr ;
	Uint8		n ;

	for (n = MAX_PAGE_SUPERVISOR; n > 0; ) 
	{
		now_ptr = Get_PageSupervisorAddress(--n) ;
		if (now_ptr == PGSPVSR)
			break ;
		else
		{
			now_ptr -> Active = (Boolean)FALSE ;
		}
	}
//	MSG("(Eng)$$$RequestState : ST_STANDBY$$$");
	Fuser_Control.RequestState = (Uint8)ST_STANDBY ;

/* Cancelation of the print command that is pending */
	fPendingPrintPageCmd = FALSE ;	/* Move From Paramset.c */
}
	
Uint8	IsSourceGood(PageSupervisor *PGSPVSR)
{
	MESSAGE message;
	Uint8 NumOfPageSupervisor;
	
	NumOfPageSupervisor = HowManyActivePageSupervisor();
	PGSPVSR->fNoPaperOnTray = FALSE;
	fPGSPVSRRemoved = FALSE;
	
	//PAGE_DATA* page = (PAGE_DATA*)PGSPVSR->page_info;
	
	//MSG("(Eng)++ IsSourceGood() Start (c %d): (PageNum = %d) (JobID %d, PageID %d) <%p> : ActivePSV %d ++\n"
	//,cProcessCounter, PGSPVSR->PageID, page->pMyJob->JobNum, page->PageCount, PGSPVSR->page_info, NumOfPageSupervisor);
	
	switch(PGSPVSR->Source) 
	{
		case MS_MultiPurposeTray:
			if(IsPaperPresentOnMPF() != PRESENT) 
			{
#if	ENG_TRACE				
				MSG("(Eng)###################\n");
				MSG("(Eng)#PAPER EMPTY : MPT#\n");
#endif
				if((fRetryPickUp == SET) || (fLastPickUp == SET))
				{
					f2ndor3rdpickEmpty = SET;
					MSG("(eng)2nd/3rd pickup EMPTY\n");
					return TRUE;
				}
				
				if(NumOfPageSupervisor >= 2)		// PageSupervisor가 1개 일 때 (첫 pick up) 바로 Empty Msg를 올리도록 (또한 이전 page 포인터를 가져오면 널 포인터 에러임)
				{
					PGSPVSR->PageAhead->fNoPaperOnTray = MS_MultiPurposeTray;
					FinishPageSupervisor( PGSPVSR );
					fPGSPVSRRemoved = TRUE;
					return FALSE;
				}
				else
				{
					if(fRequest_New_PageInfo == SET)
					{
						MSG("(eng)Request_New_PageInfo : MPT\n");
						_AddTime_T1(PSTicks_5s, PSPS_EmptyCheckWaiting)
						_Set_PS_SequenceID_Change(PSPS_EmptyCheckWaiting)
						message.msgType = MSG_STATUSUPDATE;
						slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY4 );
						SYMsgSend(PRINTMGRID, &message);
						fPGSPVSRRemoved = TRUE;
						return FALSE;
					}
					message.msgType = MSG_STATUSUPDATE;
					engine_state.paper_out_tray4 = true;
					slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY4_FINAL );
					SYMsgSend(PRINTMGRID, &message);
					
					temp_fPaperEmptyForMPF = EMPTY;
					return FALSE;
				}
				
				//temp_fPaperEmptyForMPF = EMPTY;
				//return FALSE;
			}
			break;

		case MS_Tray1:
#if KARA_S_OLD_ENGINE
			if((IsPaperPresentOnTray1() != PRESENT) )
#else
			if((fTray1Empty == SET/*IsPaperPresentOnTray1() != PRESENT*/) || (fTrayOpen == TRUE)) 
#endif
			{
#if	ENG_TRACE				
				MSG("(Eng)#####################\n");
				MSG("(Eng)#PAPER EMPTY : TRAY1#\n");
#endif
				if((fRetryPickUp == SET) || (fLastPickUp == SET))
				{
					f2ndor3rdpickEmpty = SET;
					MSG("(eng)2nd/3rd pickup EMPTY\n");
					return TRUE;
				}
				
				TurnOffTray1Solenoid();
				if(NumOfPageSupervisor >= 2)
				{
					PGSPVSR->PageAhead->fNoPaperOnTray = MS_Tray1;
					FinishPageSupervisor( PGSPVSR );
					fPGSPVSRRemoved = TRUE;
					return FALSE;
				}
				else
				{
					if(fRequest_New_PageInfo == SET)
					{
						MSG("(eng)Request_New_PageInfo : TRAY1\n");
						_AddTime_T1(PSTicks_5s, PSPS_EmptyCheckWaiting)
						_Set_PS_SequenceID_Change(PSPS_EmptyCheckWaiting)
						message.msgType = MSG_STATUSUPDATE;
						slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY1 );
						SYMsgSend(PRINTMGRID, &message);
						fPGSPVSRRemoved = TRUE;
						return FALSE;
					}
					message.msgType = MSG_STATUSUPDATE;
					engine_state.paper_out_tray1 = true;
					slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY1_FINAL );
					SYMsgSend(PRINTMGRID, &message);
					
					temp_fPaperEmptyForT1 = EMPTY;
					return FALSE;
				}
				//temp_fPaperEmptyForT1 = EMPTY;
				//return FALSE;
				return FALSE;
			}
			break;
		
		case MS_Tray2:
#if ABBA2
			if(((OptionTray1State[0]&0x30) == TRAY2_EMPTY_250)||((OptionTray1State[1]&0x70) == OPTION_OPEN) || (f2ndDrawerState == CLEAR))
#else
			if(((OptionTray1State[0]&0x30) == TRAY2_EMPTY)||((OptionTray1State[1]&0x70) == OPTION_OPEN) || (f2ndDrawerState == CLEAR))
#endif
			{
				MSG("(eng)OptionTray1State[0] = 0x%x, OptionTray1State[1] = 0x%x\n", OptionTray1State[0], OptionTray1State[1]);
#if	ENG_TRACE				
				MSG("(Eng)#####################\n");
				MSG("(Eng)#PAPER EMPTY : TRAY2#\n");
#endif
				if((fRetryPickUp == SET) || (fLastPickUp == SET))
				{
					f2ndor3rdpickEmpty = SET;
					MSG("(eng)2nd/3rd pickup EMPTY\n");
					return TRUE;
				}
				
				if(NumOfPageSupervisor >= 2)
				{
					PGSPVSR->PageAhead->fNoPaperOnTray = MS_Tray2;
					FinishPageSupervisor( PGSPVSR );
					fPGSPVSRRemoved = TRUE;
					return FALSE;
				}
				else
				{
					if(fRequest_New_PageInfo == SET)
					{
						MSG("(eng)Request_New_PageInfo : TRAY2\n");
						_AddTime_T1(PSTicks_5s, PSPS_EmptyCheckWaiting)
						_Set_PS_SequenceID_Change(PSPS_EmptyCheckWaiting)
						message.msgType = MSG_STATUSUPDATE;
						slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY2 );
						SYMsgSend(PRINTMGRID, &message);
						fPGSPVSRRemoved = TRUE;
						return FALSE;
					}
					message.msgType = MSG_STATUSUPDATE;
					engine_state.paper_out_tray2 = true;
					slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY2_FINAL );
					SYMsgSend(PRINTMGRID, &message);
					
					temp_fPaperEmptyForT2 = EMPTY;
					return FALSE;
				}
				//temp_fPaperEmptyForT2 = EMPTY;
				//return FALSE;
			}
			break;
		
		case MS_Tray3:
			if(((OptionTray2State[0]&0x30) == TRAY2_EMPTY)||((OptionTray2State[1]&0x70) == OPTION_OPEN) || (f3ndDrawerState == CLEAR))
			{
				MSG("(eng)OptionTray2State[0] = 0x%x, OptionTray2State[1] = 0x%x\n", OptionTray2State[0], OptionTray2State[1]);
#if	ENG_TRACE				
				MSG("(Eng)#####################\n");
				MSG("(Eng)#PAPER EMPTY : TRAY3#\n");
#endif
				if((fRetryPickUp == SET) || (fLastPickUp == SET))
				{
					f2ndor3rdpickEmpty = SET;
					MSG("(eng)2nd/3rd pickup EMPTY\n");
					return TRUE;
				}
				
				if(NumOfPageSupervisor >= 2)
				{
					PGSPVSR->PageAhead->fNoPaperOnTray = MS_Tray3;
					FinishPageSupervisor( PGSPVSR );
					fPGSPVSRRemoved = TRUE;
					return FALSE;
				}
				else
				{
					if(fRequest_New_PageInfo == SET)
					{
						MSG("(eng)Request_New_PageInfo : TRAY3\n");
						_AddTime_T1(PSTicks_5s, PSPS_EmptyCheckWaiting)
						_Set_PS_SequenceID_Change(PSPS_EmptyCheckWaiting)
						message.msgType = MSG_STATUSUPDATE;
						slogit( /*engine_state.pm_statusrequest = */ message.param1 = STATUS_PAPER_OUT_TRAY3 );
						SYMsgSend(PRINTMGRID, &message);
						fPGSPVSRRemoved = TRUE;
						return FALSE;
					}
					message.msgType = MSG_STATUSUPDATE;
					engine_state.paper_out_tray3 = true;
					slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY3_FINAL );
					SYMsgSend(PRINTMGRID, &message);
					
					temp_fPaperEmptyForT3 = EMPTY;
					return FALSE;
				}
				//temp_fPaperEmptyForT3 = EMPTY;
				//return FALSE;
			}
			break;
		
		case MS_Duplex:
			break;

		default:
			//MSG("(Eng)# NumOfPageSupervisor %d, fvideodone %d <Ahead PageID = %d> #\n",NumOfPageSupervisor,PGSPVSR->PageAhead->fvideodone,PGSPVSR->PageAhead->PageID);
			if((NumOfPageSupervisor >= 2) && (PGSPVSR->PageAhead->fvideodone == 0))
			{
				PGSPVSR->PageAhead->fNoPaperOnTray = PGSPVSR->EmptySourceTray;
				FinishPageSupervisor( PGSPVSR );
				MSG("(Eng)# EMPTY : Case but PASS !!! #\n");
				fPGSPVSRRemoved = TRUE;
				return FALSE;
			}
			else
			{
				message.msgType = MSG_STATUSUPDATE;
				//switch(lastPaperOutTray)
				switch(PGSPVSR->EmptySourceTray)
				{
					case MS_MultiPurposeTray:
						engine_state.paper_out_tray4 = true;
						slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY4_FINAL );
						temp_fPaperEmptyForMPF = EMPTY;
						break;
					case MS_Tray1:
						engine_state.paper_out_tray1 = true;
						slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY1_FINAL );
						temp_fPaperEmptyForT1 = EMPTY;
						break;
					case MS_Tray2:
						engine_state.paper_out_tray2 = true;
						slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY2_FINAL );
						temp_fPaperEmptyForT2 = EMPTY;
						break;
					case MS_Tray3:
						engine_state.paper_out_tray3 = true;
						slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY3_FINAL );
						temp_fPaperEmptyForT3 = EMPTY;
						break;
					default:
						MSG("(eng)Unknown EmptySourceTray : default setting\n");
						engine_state.paper_out_tray1 = true;
						slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY1_FINAL );
						temp_fPaperEmptyForT1 = EMPTY;
						break;
				}
				MSG("(Eng)#MSG_STATUSUPDATE : ReqSourceTray = 0x%x EmptySourceTray = 0x%x (status = %x)#\n", PGSPVSR->Source, PGSPVSR->EmptySourceTray, message.param1);
				SYMsgSend(PRINTMGRID, &message);
				return FALSE;
			}
			break;
	}
	return TRUE;
}

void	Set_PaperMediaSizeInTray(Uint8 Size)
{
	SavedRealMediaSizeInMPF = Size;
	SavedRealMediaSizeInTray1 = Size;
	SavedRealMediaSizeInTray2 = Size;
	SavedRealMediaSizeInTray3 = Size;

	MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
}

void Sleep_till_nothing_in_flight()
{
    int i;
    int sleep_cnt;
    bool still_active = true;

    for (sleep_cnt = 0; (sleep_cnt < 100) && still_active; sleep_cnt++)
    {
        still_active = false;
        for (i = 0; i < MAX_PAGE_SUPERVISOR; i++)
        {
            if (PGSupervisor[i].Active != NOT_ACTIVE)
            {
                still_active = true;
            }
        }
        if (still_active)
        {
	 	    /// wait for either a jam to clear out the list of pgsupervisors 
            /// or wait for in fight pages to complete.
            /// todo: add a limit to get out of here on crash.
            MSG("sleeping waiting for engine clear inflight\n");
            posix_sleep_ms(100);
        }
    }
}

int full_stack_notify(Uint8 status)
{
	MESSAGE message;
	
	message.msgType = MSG_STATUSUPDATE;
	if(status == SET)
	{
		engine_state.full_stack = true;
		fFullStackMsgSend = SET; 
		//slogit( engine_state.pm_statusrequest = message.param1 = STATUS_FACEDOWN_OUTPUT_BIN_FULL );
		slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_FULLSTACK );
		MSG("(eng) full_stack_noti. true\n");
		SYMsgSend(PRINTMGRID, &message);
	}
	else
	{
		fFullStackMsgSend = CLEAR;
		message.param1 = STATUS_OUTTRAY_FULL_CLEAR;
		message.param2 = STATUS_TRAY;
		SYMsgSend(PRINTMGRID, &message);
		MSG("(eng)Send Full Stack Clear \n");
		message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
		
		engine_state.full_stack = false;
		
		//slogit( engine_state.pm_statusrequest = message.param1 = STATUS_ONLINE ); 
		clear_engine_state();
		MSG("(eng) full_stack_noti. false\n");
		//SYMsgSend(PRINTMGRID, &message);
	}				

//	message.param3 = PGSPVSR->page_info; // lsptodo: was : engine_state.active_page;
	//SYMsgSend(PRINTMGRID, &message);
	return 1;
}
	
unsigned char IsPaperLengthNarrow(unsigned int length)
{
	if(length < NarrowLengthBoundary)
	{
		MSG("(eng)Narrow Media\n");
#if ABBA2
		fPaperLengthIsNarrow = FALSE;	// delete slowly printing narrow media for abba2
#else
		fPaperLengthIsNarrow = TRUE;
#endif
	}
	else
	{
		MSG("(eng)Normal Media\n");
		fPaperLengthIsNarrow = FALSE;
	}
	
#if FUSER_PARAM_TEST
	if(ftargetTempfor25ppm == SET)
		fPaperLengthIsNarrow = TRUE;
	else
		fPaperLengthIsNarrow = FALSE;
#endif

	return fPaperLengthIsNarrow;
}

int PickupTempSelect(PageSupervisor *PGSPVSR)
{
#if EP_PARAM_TEST
	if(fParamPickVALUE == SET)
	{
		if(fParamFirstPickVALUE == TRUE)
		{
			if(SecondPage == TRUE)
				return Fuser_Control.Target_Temperature - Param_FirstPick_VALUE;
			else
				return Fuser_Control.Target_Temperature - Param_Pick_VALUE;
		}
		else
			return Fuser_Control.Target_Temperature - Param_Pick_VALUE;
	}
	else
	{
		if(fParamFirstPickVALUE == TRUE)
		{
			if(SecondPage == TRUE)
				return Fuser_Control.Target_Temperature - Param_FirstPick_VALUE;
			else
				return Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP;
		}
		else
			return Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP;
	}
#else
	if(PGSPVSR->PowersaveToPrint == 1)
	{
		return Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP_FOR_PWS;
	}
#if BLI_TEST
	else if(PGSPVSR->PowersaveToPrint == 2)
	{
		return Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP_FOR_PWS_2nd;
	}
#endif
	else
	{
		return Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP;
	}
#endif
}

int MotorStartTempSelectForWakeup(void)
{
	int MotorStartTemp;
	
	if(SelectMotorRunTempForPWStoPrint == SET)
	{
		Set_FuserTemp(PendingPrintPageData[1], PendingPrintPageData[7], PendingPrintPageData[3], 0, 1); // select motor running temperature for powersavetoprint
		SelectMotorRunTempForPWStoPrint = CLEAR;
	}
	
	if((Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP) >= 150)
	{
		MotorStartTemp = 150;
	}
	else
	{
		MotorStartTemp = Fuser_Control.Target_Temperature - FUSER_PARAM_PICKUP_TMP;
	}
	
	return MotorStartTemp;
}

Uint8 IsPaperSizeNarrowInTray(Uint8 TraySource)
{
	Uint8 i;
	Uint8 Narrow[3] = {TRAY_MEDIA_EXEC, TRAY_MEDIA_B5, TRAY_MEDIA_A5};
	TRAY_DATA temp_Tray;
	switch (TraySource)
	{
		case MS_Tray1:
			temp_Tray = Tray_Status.Tray1;
			break;
		case MS_Tray2:
			temp_Tray = Tray_Status.Tray2;
			break;
		case MS_Tray3:
			temp_Tray = Tray_Status.Tray3;
			break;
		default:
			MSG("(eng)Unknown PaperSource (%d) : Setting tray1\n");
			temp_Tray = Tray_Status.Tray1;
			break;
	}
	for(i = 0; i < 3; i++)
	{
		if(GetTrayMediaInfo(temp_Tray.newTrayStatus) == Narrow[i]) return TRUE;
	}	
	return FALSE;
}
Uint32 GetDarknessConversionRate(Uint32 Level)
{
	Uint32 tempConversionRate;
	tempConversionRate = 100;
	
	switch (Level)
	{
		case 1:
			tempConversionRate = 40;
			break;
		case 2:
			tempConversionRate = 52;
			break;
		case 3:
			tempConversionRate = 64;
			break;
		case 4:
			tempConversionRate = 76;
			break;
		case 5:
			tempConversionRate = 88;
			break;
		case 6:
			tempConversionRate = 100;
			break;
		case 7:
			tempConversionRate = 115;
			break;
		case 8:
			tempConversionRate = 130;
			break;
		case 9:
			tempConversionRate = 140;
			break;
		case 10:
			tempConversionRate = 150;
			break;
		default:
			tempConversionRate = 100;
			break;
	}
	return tempConversionRate;
}

Uint8 TrayEmptyCheck(Uint32 tray)
{
	Uint8 result;
	result = NO_EMPTY;

	switch(tray) 
	{
		case MS_MultiPurposeTray:
			if(IsPaperPresentOnMPF() != PRESENT)
			{
				result = EMPTY;
			}
			break;
		case MS_Tray1:
			if(IsPaperPresentOnTray1() != PRESENT)
			{
				result = EMPTY;
			}
			break;
		case MS_Tray2:
			if(((OptionTray1State[0]&0x30) == TRAY2_EMPTY)||((OptionTray1State[1]&0x70) == OPTION_OPEN))
			{
				result = EMPTY;
			}
			break;
		case MS_Tray3:
			if(((OptionTray2State[0]&0x30) == TRAY2_EMPTY)||((OptionTray2State[1]&0x70) == OPTION_OPEN))
			{
				result = EMPTY;
			}
			break;
		case MS_Duplex:
			result = NO_EMPTY;
			break;
		default:
			result = EMPTY;
			break;
	}
	MSG("(eng)+++ TrayEmptyCheck before video on : source tray = %x result = %d +++\n",tray,result);
	return result;
}

#if DUPLEX_SLOW_MODE
void Set_DuplexPrintTime(int time)
{
	cPrintOver300pagesTime[TopPrintOver300pagesTime] = time;
	MSG("(Eng)cPrintOver300pagesTime[%d] = %d\n", TopPrintOver300pagesTime, cPrintOver300pagesTime[TopPrintOver300pagesTime]);
	TopPrintOver300pagesTime++;
	if(TopPrintOver300pagesTime == DUPLEXPRINTSLOWPAGES) TopPrintOver300pagesTime = CLEAR;
}

char Get_DuplexPrintTime(int time)
{
	int res = 0;
	int i;
	
	for(i = 0; i < DUPLEXPRINTSLOWPAGES; i++)
	{
		if(((time - cPrintOver300pagesTime[i]) <= DUPLEXPRINT300TIME) && (cPrintOver300pagesTime[i] != -1))
		{
			res++;
		}
	}
	MSG("(eng)Get_DuplexPrintTime = %d\n", res);
	return (res>=DUPLEXPRINTSLOWPAGES)?TRUE:FALSE;
}

char Clear_DuplexPrintSlowlyFor15min(int time)
{
	if((time - cDuplexPrintSlowlyFor15min) >= DUPLEXPRINTSLOW)
	{
		if(fDebugDuplexPrintSlowlyFor15minClear == TRUE)
		{
			MSG("(eng)cDuplexPrintSlowlyFor15min : %d->CLEAR\n", cProcessCounter - cDuplexPrintSlowlyFor15min);
			fDebugDuplexPrintSlowlyFor15minClear = FALSE;
		}
		cDuplexPrintSlowlyFor15min = CLEAR;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif

void VideoOffBeforeJamNotify(void)
{
	slogit( dec_laser_set_top_sync( eng_hardware->laser, false /* disable hvsync */));
	slogit( dec_enable_vdo_top_sensor( eng_hardware->laser, eng_hardware->paper_entry, false )); // false for off
	slogit( dec_enable_vdo( eng_hardware->laser, false ));	
	MSG("(eng)VideoOffBeforeJamNotify (%d)\n", cProcessCounter);

	return;
}

int GetJogModePageCount(Uint32 c_time, Uint32 mode)
{
	int result = 0;
	int idx = 0;
	int pre_time = 0;
	int i;
	
	pre_time = JogModePageTime[0];
	for(i = 0; i < JOGMODE_PAGE_MAX; i++)
	{
		if(JogModePageTime[i] == 0)
		{
			idx = i;
			break;
		}
		else
		{
			if(pre_time > JogModePageTime[i])
			{
				pre_time = JogModePageTime[i];
				idx = i;
			}
		}
	}
	
	JogModePageTime[idx] = c_time;
	if(mode == 2)
	{
		idx++;
		if(idx == JOGMODE_PAGE_MAX)
		{
			idx = 0;
		}
		JogModePageTime[idx] = c_time;
	}
	
	for(i = 0; i < JOGMODE_PAGE_MAX; i++)
	{
		if((JogModePageTime[i] != 0) && ((c_time - JogModePageTime[i]) < JOGMODE_PRINT_TIME))
		{
			result++;
		}
	}
	return result;
}

int GetFanCtrlPageCount(Uint32 c_time, Uint32 mode)
{
	int result = 0;
	int idx = 0;
	int pre_time = 0;
	int i;
	
	pre_time = FanCtrlPageTime[0];
	for(i = 0; i < FANCTRL_PAGE_MAX; i++)
	{
		if(FanCtrlPageTime[i] == 0)
		{
			idx = i;
			break;
		}
		else
		{
			if(pre_time > FanCtrlPageTime[i])
			{
				pre_time = FanCtrlPageTime[i];
				idx = i;
			}
		}
	}
	
	FanCtrlPageTime[idx] = c_time;
	if(mode == 2)
	{
		idx++;
		if(idx == FANCTRL_PAGE_MAX)
		{
			idx = 0;
		}
		FanCtrlPageTime[idx] = c_time;
	}
	
	for(i = 0; i < FANCTRL_PAGE_MAX; i++)
	{
		if((FanCtrlPageTime[i] != 0) && ((c_time - FanCtrlPageTime[i]) < JOGMODE_PRINT_TIME))
		{
			result++;
		}
	}
	return result;
}

Uint8 GetJogModeEnvIdx(Uint8 env_val, int page_cnt)
{
	Uint8 idx;
	
	if(page_cnt < JOGMODE_PAGE_MAX)
	{
		idx = JOG_MODE_L;
	}
	else
	{
		if(env_val <= 64)
		{
			idx = JOG_MODE_H;
		}
		else if(env_val < 85)
		{
			idx = JOG_MODE_M;
		}
		else
		{
			idx = JOG_MODE_L;			
		}
	}
	return idx;
}

Uint8 GetFanCtrlEnvIdx(Uint8 env_val, int page_cnt)
{
	Uint8 idx;
	
	if(page_cnt < FANCTRL_PAGE_MAX)
	{
		idx = FAN_CTRL_L;
	}
	else
	{
		if(env_val <= 64)
		{
			idx = FAN_CTRL_H;
		}
		else
		{
			idx = FAN_CTRL_L;			
		}
	}
	return idx;
}
