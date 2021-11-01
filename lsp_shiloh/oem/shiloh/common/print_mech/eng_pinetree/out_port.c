/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	out_port.c					    				
 **	 Revision  :	ver 1.4								    			
 **	 Date	   : 	Dec 21, 2009										
 **	 Author    :	Youngjin Lee		
 +-----------------------------------------------------------------+*/

#include	"farch.h"
#include	"pioname.h"
#include	"em.h"
#include	"option.h"
#include	<stdio.h>

#define DBG_PRFX "engop: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING
#define SLOG_MSG  DBG_MEMLOG_INFO		/* ADD : @@2013.01.08. By CSPark */


extern Uint8	fStartExitMotor;
extern Uint8	fHoldExitMotor;
extern Uint8	fStopExitMotor;

extern uint32_t page_count;

extern int MotorSpeedparam;
extern Uint8 fMotorSpeedMode;
extern Uint8 fPaperLengthIsNarrow;
extern Uint8	fPwsToPrint;
extern Uint8	fMotorRunForPwsToPrint;
extern Uint8	fPwsToPrint_1stPick;
extern Uint8	fPwsToPrint_2ndPick;
extern Uint8	fPwsToPrint_1stPage;
extern Uint8	fPwsToPrint_2ndPage;
extern Uint8	fSpeedUpForPWS;
extern Uint8 	fWakeupState;
extern Uint8 	fJob_Toner_Correction_value;
extern Uint8 MotorStartForLaminating;

extern Uint8 cJobEndDotCountWriteFailCnt;

extern uint32_t mtrstop_count_laminating1;
extern uint32_t mtrstop_count_laminating2;

extern Uint8 fSmartICWrite_DotCount;
extern Uint8 fMemoryWrite;
extern Uint8 fSmartICWrite_DotCountInRun;
extern Uint32 cRunMainMotorForJobFinish;
extern Uint8 Machine_TLI;

extern Uint8 f1stOptionResync;
extern Uint8	fReadHsync;
#if DUPLEX_SLOW_MODE
extern int  cDuplexPrintSlowlyFor15min;
extern Uint8  fDuplexPrintSlowlyFor15min;
extern Uint8	EnvTempStatus;
#endif
#if DUPLEX_SLOW_MODE2
extern Uint8 fDuplexPrintSlowlyForServoHH;
#endif

extern void  	StartMainMotor(void);
extern void  	StopMainMotor(void);
extern Uint32 ChangeVoltageToTemp(void);
#if DUPLEX_SLOW_MODE
extern char Get_DuplexPrintTime(int time);
#endif

Uint32 tray1_high;
Uint32 tray1_low;

typedef struct _mRunTime{
	int start;
	int end;
}mRunTime;
mRunTime NormalFuserRunTime = { 
	.start = 0,
	.end = 0
};
mRunTime LowFuserRunTime = {
	.start = 0,
	.end = 0
};
int normal_fuserTime = CLEAR;
int low_fuserTime = CLEAR;


/* Define Function Prototype */

/* FEED , MPF Solenoid Function */
Uint8 TurnOnTray1Solenoid(Uint8);
Uint8 TurnOffTray1Solenoid(void);
Uint8 TurnOnMPFSolenoid(void);
Uint8 TurnOffMPFSolenoid(void);
Uint8 TurnOnDuplexSolenoid(void);
Uint8 TurnOffDuplexSolenoid(void);
extern void Option_Tx_AccuSpeedSet(char, unsigned char);

void dec_laser_config_change(unsigned char);

/* PWM Functions */
void EnableChrPWM(void);
void EnableDevPWM(void);
void EnableXfrPWM(void);
void DisableChrPWM(void);
void DisableDevPWM(void);
void DisableXfrPWM(void);
void Set_Chr_PWM(Uint8);
void Set_Dev_PWM(Uint8);
void Set_Xfr_PWM(Uint8);


Uint8 fMotorRun_Timer;
Uint8 fStartMainMotor;
int cTimerStart_Motor;
int cMainMotorDelay;
Uint32 hv_pwm_period_mtr;
#if MPT_SENSOR_PIN_CHG
extern Uint8 fScan_IRQ;
#endif

extern int motorType;

extern void ChangePWM(int);
extern void BLDC_Speed_Change(int);

extern void ReAuthenticationForWrite(Uint8);

extern Uint8 SecondPage;
extern Uint32 MainMotorJogCounter;
extern Uint32 MainMotor_1MinJogCounter;

extern Uint32 cDotCount;
extern Uint32 cPageCount5perTrans;
extern Uint8 machineType;
Uint8 modeOfMainClk;

extern Uint8	fPOR_PageSupervisor_Act;
extern Uint8 	fPressWakeupKey;

extern Uint8	fStaticMotorRunTime;
extern int cProcessCounter;
extern Struct_Fuser_Control Fuser_Control;
extern engine_hardware_t *eng_hardware;

extern Uint16 first_motor_cycle;
extern Uint16 second_motor_cycle;

void	MainMotorDelay(void);
void	StartMainMotor(void);
void	StopMainMotor(void);
void	StartMirrorMotor(void);
void	StopMirrorMotor(void);
void	MainMotorSpeedChange(int, Uint8);
void	MirrorMotorSpeedChange(int, Uint8);

void Set_Timer_Motor_Run(void);
int Get_Elapse_Motor_Run(void);

#if PICK_THD
extern mqd_t dcmtr_ctrl_mq;
#endif
extern mqd_t lsu_ctrl_mq;
extern int Set_MotorStatusNotify(Uint8, Uint32, Uint8, Uint32);

Uint8	Get_MainMotorLockState(void);
Uint8	Get_MirrorMotorLockState(void);
Uint8	Get_MainMotorState(void);

Uint8	IsPaperPresentOnStack(void);
Uint8	IsPaperPresentOnPins(void);
Uint8	IsPaperPresentOnPexs(void);
Uint8 	IsWakeKeyState(void);
Uint8	Get_MainFanLockState(void);
Uint8	Get_SubFanLockState(void);
Uint8	Get_DevFanLockState(void);

extern void DisableTransferBiasSignal(void);
extern void DisableDeveloperBiasSignal(void);
extern void DisableChargeBiasSignal(void);
extern Uint8 TurnOffMPFSolenoid(void);
extern Uint8 TurnOffTray1Solenoid(void);
extern int 	Set_LSUOffNotify(void);
extern Uint8 	TurnOffDuplexSolenoid(void);
extern Struct_Fuser_Control Fuser_Control;

extern engine_hardware_t *eng_hardware;
extern STRUCT_TRAY_STATUS Tray_Status;

void StartFuserRuntime(int time, Uint8 motormode);
void EndFuserRuntime(int time, Uint8 motormode, int *normalRuntime, int *lowRuntime);

void Xfr_Enable(void)
{
	gpio_write( eng_hardware->thv_en, 0);
}

void Xfr_Disable(void)
{
	gpio_write( eng_hardware->thv_en, 1);
}

Uint8 TurnOnTray1Solenoid(Uint8 Recycle)
{
#if ABBA2
	gpio_write( eng_hardware->pick_pin, 1 );
#else
#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		gpio_write( eng_hardware->pick_pin, 1 );
	}
	else
	{
#if KARA_S && !KARA_S_OLD_ENGINE
#if PICK_THD
		uint32_t msg;
		engine_hardware_t* eng_hw = eng_hardware;
		if(Recycle == TRUE)
		{
			if(fMotorSpeedMode == MTR_MODE_QUIET)
			{
				set_pidcontrol_param(eng_hw->paper_motor, PID_Kp, PID_Ki, PID_Kd, PID_dt, PID_Target_Recycled, PID_Target_Quiet); // pid parm setting.
			}
			else
			{
				set_pidcontrol_param(eng_hw->paper_motor, PID_Kp, PID_Ki, PID_Kd, PID_dt, PID_Target_Recycled, PID_Target_Normal); // pid parm setting.
			}
		}
		else
		{
			if(fMotorSpeedMode == MTR_MODE_QUIET)
			{
				set_pidcontrol_param(eng_hw->paper_motor, PID_Kp, PID_Ki, PID_Kd, PID_dt, PID_Target_Quiet, PID_Target_Quiet); // pid parm setting.
			}
			else
			{
				set_pidcontrol_param(eng_hw->paper_motor, PID_Kp, PID_Ki, PID_Kd, PID_dt, PID_Target_Normal, PID_Target_Normal); // pid parm setting.
			}
		}
		
		msg = DC_MOTOR_RUN;
		posix_message_send(dcmtr_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
#else
		engine_hardware_t* eng_hw = eng_hardware;
		dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
#endif
#else
		gpio_write( eng_hardware->pick_pin, 1 );
#endif
	}
#endif
	return ON;
}

Uint8 TurnOffTray1Solenoid(void)
{
#if ABBA2
	gpio_write( eng_hardware->pick_pin, 0 );
#else
#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		gpio_write( eng_hardware->pick_pin, 0 );
	}
	else
	{
#if KARA_S && !KARA_S_OLD_ENGINE
#if PICK_THD
		uint32_t msg;
		msg = DC_MOTOR_STOP;
		posix_message_send(dcmtr_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
#else
	//	dcmotor_run(eng_hardware->paper_motor, eng_hardware->speed_off);
#endif
#else
		gpio_write( eng_hardware->pick_pin, 0 );
#endif
	}
#endif
	return OFF;
}

Uint8 TurnOnMPFSolenoid(void)
{
	gpio_write( eng_hardware->pick_mpt, 1 );
	return ON;
}

Uint8 TurnOffMPFSolenoid(void)
{
	gpio_write( eng_hardware->pick_mpt, 0 );
	return OFF;
}

Uint8 TurnOnDuplexSolenoid(void)
{
	gpio_write( eng_hardware->duplex_sol, 1 );
	return ON;
}

Uint8 TurnOffDuplexSolenoid(void)
{
	gpio_write( eng_hardware->duplex_sol, 0 );
	return OFF;
}

void  MainMotorDelay(void)
{
	cMainMotorDelay = cProcessCounter - cTimerStart_Motor;
	if((fStartMainMotor == SET) && (cMainMotorDelay >= 2))
	{
#if ENG_TRACE
		MSG("(Eng)## MainMotorDelay!!!(%d)##\n", cProcessCounter);
#endif		
#if KARA_S
		gpio_write( eng_hardware->mainmotor_start, 0 );
#endif
		fStartMainMotor = CLEAR;
	}
}

void  StartMainMotor(void)
{
	MESSAGE message;
#if !KARA_S	
	engine_hardware_t* eng_hw = eng_hardware;
#endif
	normal_fuserTime = CLEAR;
	low_fuserTime = CLEAR;
	
	if(fPOR_PageSupervisor_Act == TRUE)
	{
		MSG("(eng)##StartMainMotor : POR (%d)## \n", cProcessCounter);
		video_set_pixel_freq(PIXEL_FREQ_LOW_SPEED_MODE, 85);
		video_set_pcode(PWM_LOW_SPEED_MODE, 0);
		fMotorSpeedMode = MTR_MODE_QUIET;
	}
	else if (MotorStartForLaminating == SET)
	{
		MSG("(eng)##StartMainMotor : Laminate (%d)## \n", cProcessCounter);
		MotorStartForLaminating = CLEAR;
		fMotorSpeedMode = MTR_MODE_LAMINATE;
	}
	else
	{
#if DUPLEX_SLOW_MODE
		if((fPaperLengthIsNarrow == TRUE) || (fMotorRunForPwsToPrint == TRUE) || (fDuplexPrintSlowlyFor15min != CLEAR))
#elif DUPLEX_SLOW_MODE2
		if((fPaperLengthIsNarrow == TRUE) || (fMotorRunForPwsToPrint == TRUE) || (fDuplexPrintSlowlyForServoHH != CLEAR))
#else
		if((fPaperLengthIsNarrow == TRUE) || (fMotorRunForPwsToPrint == TRUE))
#endif
		{
			MSG("(eng)##StartMainMotor : Print(Quiet) (%d)##\n", cProcessCounter);
			video_set_pixel_freq(PIXEL_FREQ_LOW_SPEED_MODE, 85);
			video_set_pcode(PWM_LOW_SPEED_MODE, 0);
			fMotorSpeedMode = MTR_MODE_QUIET;
			fMotorRunForPwsToPrint = CLEAR;
		}
		else
		{
			MSG("(eng)##StartMainMotor : Print(Normal) (%d)##\n", cProcessCounter);
			video_set_pixel_freq(PIXEL_FREQ_ORG_SPEED_MODE, 0);
			video_set_pcode(PWM_ORG_SPEED_MODE, 0);
			fMotorSpeedMode = MTR_MODE_NORMAL;
		}
	}
	Option_Tx_AccuSpeedSet(OPTION_T1, fMotorSpeedMode);
	Option_Tx_AccuSpeedSet(OPTION_T2, fMotorSpeedMode);
	//BLDC_Motor_PWM_EN(1);		// Motor Driver Clock Enable
	//GP_Write(BLDC_ST,0);		// Motor Driver Start Enable
	
// 	dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
#if KARA_S	
//	Uint16 first_cycle;
//	Uint16 sencond_cycle;
//	first_cycle = (Uint16)(HV_PWM_PER_CONV_MTR * 50);
//	sencond_cycle = (Uint16)(HV_PWM_PERIOD_MTR - first_cycle);

//	pwm_change_duty_cycle(eng_hw->mainmotor_voltage, first_motor_cycle, second_motor_cycle); // del by namja
	MainMotorSpeedChange(MotorSpeedparam, fMotorSpeedMode); // namja85, 130809
#else
	dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
#endif
	fStartMainMotor = SET;
	
	//Set_MotorStatusNotify(MOTOR_RUN,0x00,0x00,0x00);
	if(cRunMainMotorForJobFinish == CLEAR)
	{
		slogit( message.msgType = MSG_STATUS_MOTOR_RUN );
		message.param1 = MOTOR_RUN;
		message.param2 = normal_fuserTime;
		message.param3 = (int*)&low_fuserTime;
		SYMsgSend(PRINTMGRID, &message);
	}
	
#if 1 //#ifdef SMART_IC_CON		
	if(fMotorRun_Timer==CLEAR)
	{
		fMotorRun_Timer = SET;
		Set_Timer_Motor_Run();
	}
#endif	// #ifdef SMART_IC_CON	
	Fuser_Control.fMakeMotorRun = SET;
	MainMotorJogCounter = CLEAR;
	MainMotor_1MinJogCounter = CLEAR;
	SecondPage = SET;
	f1stOptionResync = SET;
	fJob_Toner_Correction_value = SET;
		
	if(cJobEndDotCountWriteFailCnt != 0)
	{
		fSmartICWrite_DotCount = TRUE; 
		fMemoryWrite = TRUE;
		fSmartICWrite_DotCountInRun = TRUE;
	}
}

void  StopMainMotor(void)
{
	MESSAGE message;
	
	engine_hardware_t* eng_hw = eng_hardware;
	
	normal_fuserTime = CLEAR;
	low_fuserTime = CLEAR;
	
	fStartMainMotor = CLEAR;
	fStaticMotorRunTime = CLEAR;
#if ENG_TRACE	
	MSG("(Eng)## StopMainMotor!!! : curr_temp = %d <%d> ##\n", ChangeVoltageToTemp(), cProcessCounter);
#endif	
	//GP_Write(BLDC_ST,1);		// Motor Driver Start Enable	
	//BLDC_Motor_PWM_EN(0);		// Motor Driver Clock Enable
// 	dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);

	EndFuserRuntime(cProcessCounter, fMotorSpeedMode, &normal_fuserTime, &low_fuserTime);
	MSG("(Eng)normal_fuserTime = %d, low_fuserTime = %d\n", normal_fuserTime, low_fuserTime);
	
#if KARA_S
	gpio_write( eng_hardware->mainmotor_start, 1 );
	pwm_off(eng_hw->mainmotor_voltage);
#else
	dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);
#endif
	
#if 1 //#ifdef SMART_IC_CON	
	Uint32 MotorRunTime;
	
	if(fMotorRun_Timer==SET)
	{
		fMotorRun_Timer = CLEAR;
		MotorRunTime = Get_Elapse_Motor_Run();
#if ENG_TRACE	
		MSG("(Eng)## MotorRunTime = %d ##\n",MotorRunTime);
#endif
		//Toner_Used_Percentage = (cDotCount / ((TonerCartridgeCapacity * TRANS_COEFFICIENT_5PERCENT)/100));
		//Set_MotorStatusNotify(MOTOR_STOP, MotorRunTime, Toner_Used_Percentage, cPageCount5perTrans);
	}
	else
	{
		MotorRunTime = 0;
#if ENG_TRACE	
		MSG("(Eng)## Off -> Off MotorRunTime = %d ##\n",MotorRunTime);
#endif		
	}
#endif	// #ifdef SMART_IC_CON	
	if(cRunMainMotorForJobFinish == CLEAR)
	{
		slogit( message.msgType = MSG_STATUS_MOTOR_RUN );
		message.param1 = MOTOR_STOP;
		message.param2 = normal_fuserTime;
		message.param3 = (int*)&low_fuserTime;
		SYMsgSend(PRINTMGRID, &message);
	}
	
	Fuser_Control.fMainMotorState = CLEAR;
	Fuser_Control.fMakeMotorRun = CLEAR;
	MainMotorJogCounter = CLEAR;
	MainMotor_1MinJogCounter = CLEAR;
	SecondPage = CLEAR;
	
	fPwsToPrint = CLEAR;
	fMotorRunForPwsToPrint = CLEAR;
	fPwsToPrint_1stPick = CLEAR;
	fPwsToPrint_2ndPick = CLEAR;
	fPwsToPrint_1stPage = CLEAR;
	fPwsToPrint_2ndPage = CLEAR;
	fPressWakeupKey = CLEAR;
	fSpeedUpForPWS = CLEAR;
	
	f1stOptionResync = CLEAR;
#if FUSER_PARAM_TEST
	page_count = CLEAR;
#endif
	fWakeupState = FALSE;
	fJob_Toner_Correction_value = CLEAR;
	
	if(cJobEndDotCountWriteFailCnt != 0)
	{
		ReAuthenticationForWrite(cJobEndDotCountWriteFailCnt);
	}
	
	mtrstop_count_laminating1 = cProcessCounter;
	mtrstop_count_laminating2 = cProcessCounter;
}

extern engine_state_t engine_state;

void StartMirrorMotor (void)
{
	uint32_t msg;
	StopMirrorMotor();
	if(engine_state.mirror_motor_state != LSU_MIRROR_RUN_AND_SYNC)
	{
		engine_state.mirror_motor_state = msg = LSU_MIRROR_RUN_AND_SYNC;
		posix_message_send(lsu_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
	}
}

void StopMirrorMotor (void)
{
	uint32_t msg;
	if(engine_state.mirror_motor_state != LSU_MIRROR_STOP)
	{
		engine_state.mirror_motor_state = msg = LSU_MIRROR_STOP;
		posix_message_send(lsu_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
	}
}

void Set_Timer_Motor_Run(void)
{
	cTimerStart_Motor = cProcessCounter;
#if	ENG_TRACE	
	MSG("(Eng)## Set_Timer_Motor_Run = %d ##\n",cTimerStart_Motor);
#endif	
}

int Get_Elapse_Motor_Run(void)
{
	return cProcessCounter - cTimerStart_Motor;
}

void Motor_FWD(void)
{
#if	ENG_TRACE	
	MSG("(Eng)## Motor_FWD ##\n");
#endif
	gpio_write( eng_hardware->mainmotor_dir, 1 );
	//GP_Write(BLDC_DIR,1);
}

void Motor_BWD(void)
{
#if	ENG_TRACE	
	MSG("(Eng)## Motor_BWD ##\n");
#endif	
	gpio_write( eng_hardware->mainmotor_dir, 0 );
	//GP_Write(BLDC_DIR,0);
}

Uint8	Get_MainFanLockState(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->mainfan_lock, &val);
	
	if(val == LOW)
		return LOCK;
	else
		return UNLOCK;
}

Uint8	Get_SubFanLockState(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->subfan_lock, &val);
	
	if(val == LOW)
		return LOCK;
	else
		return UNLOCK;
}

Uint8	Get_DevFanLockState(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->devfan_lock, &val);
	
	if(val == LOW)
		return LOCK;
	else
		return UNLOCK;
}

Uint8	Get_MainMotorLockState(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->paper_motor_lock, &val);
	
	if(val == LOW)
		return LOCK;
	else
		return UNLOCK;
}

Uint8	Get_MirrorMotorLockState(void)
{
//	if(MM_LOCK == LOW)
	if(spinning_mirror_lock(eng_hardware->laser) == TRUE)
		return LOCK;
	else
		return UNLOCK;
}

Uint8	IsPaperPresentOnStack(void)
{
	//engine_hardware_t* eng_hw = eng_hardware;
	
	//if(FULL_STACK_SEN == LOW) {
		//return PRESENT;
	//}
	//return NOT_PRESENT;
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->paper_outbin_full, &val);

#if KARA_S_OLD_ENGINE	
	if(val == LOW)
#else
	if(val == HIGH)
#endif
		return PRESENT;
	else
		return NOT_PRESENT;
}
//#endif

Uint8	IsPaperPresentOnPins(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	
	if(PAPER_IN == LOW) {
		return PRESENT;
	}
	return NOT_PRESENT;
}

Uint8	IsPaperPresentOnPexs(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	
	if(PAPER_OUT == LOW) 
	{
		return NOT_PRESENT;
	}
	return PRESENT;
}

Uint8	Get_MainMotorState(void)
{
	if(Fuser_Control.fMakeMotorRun == LOW)
	{
	  return STOP;
	}
	else
	{
	  return RUNNING;
	}
}

Uint8	IsDoorCoverState(void)
{
	//engine_hardware_t* eng_hw = eng_hardware;
	
	//if(DOOR_OPEN == HIGH) {
		//return CLOSE;
	//}
	//else {
		//return OPEN;
	//}
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->door_open, &val);
	
	if(val == LOW)
		return CLOSE;
	else
		return OPEN;
}

Uint8	IsPaperPresentOnMPF(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
#if ABBA2
	uint32_t mpt_sensor;
	gpio_read(eng_hw->paper_high, &mpt_sensor);
	if(mpt_sensor == HIGH) {
		return NOT_PRESENT;
	}
	else {
		return PRESENT;
	}
#else
#if MPT_SENSOR_PIN_CHG
	uint32_t mpt_sensor;
#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		gpio_read(eng_hw->paper_high, &mpt_sensor);
		if(mpt_sensor == HIGH) {
			return NOT_PRESENT;
		}
		else {
			return PRESENT;
		}
	}
	else
	{
		if(fScan_IRQ == TRUE)
		{
			gpio_read(eng_hw->paper_avail_mpt_forScanIRQ, &mpt_sensor);
			if(mpt_sensor == HIGH) {
				return NOT_PRESENT;
			}
			else {
				return PRESENT;
			}
		}
		else
		{
			if(MPF_SEN == HIGH) {
				return NOT_PRESENT;
			}
			else {
				return PRESENT;
			}
		}
	}
#else	
	if(MPF_SEN == HIGH) {
		return NOT_PRESENT;
	}
	else {
		return PRESENT;
	}
#endif
#endif
}

/***********************************************************
 * TRAY Sensor State
 * 
 * Paper EMPTY  	: 	val1 == 1 && val2 == 0
 * Paper LOW 		: 	val1 == 1 && val2 == 1
 * Paper Normal 	: 	val1 == 0 && val2 == 1
 * */
Uint8	IsPaperPresentOnTray1(void)		// added //21Apr09 yjlee 	
{
	engine_hardware_t* eng_hw = eng_hardware;
#if ABBA2
	uint32_t tray1_sensor;
	gpio_read(eng_hw->paper_low, &tray1_sensor);
	if(tray1_sensor == HIGH) {
		return NOT_PRESENT;
	}
	else {
		return PRESENT;
	}
#else
	uint32_t tray1_sensor;
#if PINETREE_L
	//if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540) || (Machine_TLI == TLI_DEFAULT))
	if(1)
#else
	if((Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540))
#endif
	{
		gpio_read(eng_hw->paper_low, &tray1_sensor);
		if(tray1_sensor == HIGH) {
			return NOT_PRESENT;
		}
		else {
			return PRESENT;
		}
	}
	else
	{
#if KARA_S && !KARA_S_OLD_ENGINE
		uint32_t val1, val2;
		gpio_read(eng_hw->paper_low, &val1);
		gpio_read(eng_hw->paper_high, &val2);
		
		tray1_low = val1;
		tray1_high = val2;
		
		if((val1 == HIGH) && (val2 == LOW))
		{
			// Paper Empty
	//		SetTrayMediaEmpty(Tray_Status.Tray1.newTrayStatus);
			return NOT_PRESENT;
		}
		else
		{
			if((val1 == HIGH) && (val2 == HIGH))
			{
				// Paper Low
	//			SetTrayMediaLow(Tray_Status.Tray1.newTrayStatus);
			}
			else
			{
				// Paper Normal
	//			SetTrayMediaNormal(Tray_Status.Tray1.newTrayStatus);
			}
			return PRESENT;
		}
#else
		if(TRAY1_SEN == LOW) {
			return NOT_PRESENT;
		}
		else {
			return PRESENT;
		}
#endif
	}
#endif
}

Uint8 IsWakeKeyState(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->wake_key, &val);
#if ABBA2	
	if(val == HIGH)
		return TRUE;
	else
		return FALSE;
#else
	if(val == HIGH)
		return TRUE;
	else
		return FALSE;
#endif
}

void	DisableAllOutputPorts(void)
{
	Set_LSUOffNotify();
	StopMirrorMotor();
	DisableTransferBiasSignal();
	DisableDeveloperBiasSignal();
	DisableChargeBiasSignal();
	TurnOffMPFSolenoid();
	TurnOffTray1Solenoid();
	TurnOffDuplexSolenoid();
	MSG("DisableAllOutputPorts\n");
}
#define MTR_UNIT_PERIOD 211//100
#define MTR_UNIT_PERIOD_M 220//100
#define MTR_UNIT_PERIOD_L 300//50
#define MTR_UNIT_PERIOD_NEW 236
#define MTR_UNIT_PERIOD_M_NEW 243
#define MTR_UNIT_PERIOD_L_NEW 2
void MainMotorSpeedChange(int MotorSpeed, Uint8 fPOR_MotorRun)
{
	engine_hardware_t* eng_hw = eng_hardware;
	int32_t CHANGED_PWM_PEROID_MTR = 0;
	int32_t CHANGED_PWM_PER_CONV_MTR = 0;
	
	switch (fPOR_MotorRun)
	{
		case MTR_MODE_NORMAL:
#if MAC_TYPE_M
			if (motorType == MOTOR_NEW) {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD_M_NEW);
			} else {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD_M);
			}
#else
			if (motorType == MOTOR_NEW) {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD_NEW);
			} else {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD);
			}
#endif
			pwm_change_timebase(eng_hw->mainmotor_voltage, 0);
			break;
		case MTR_MODE_QUIET:
			if (motorType == MOTOR_NEW) {
				CHANGED_PWM_PEROID_MTR = HV_PWM_PERIOD_MTR_L_NEW - (MotorSpeed * MTR_UNIT_PERIOD_L_NEW);
				pwm_change_timebase(eng_hw->mainmotor_voltage, 1);
			} else {
				CHANGED_PWM_PEROID_MTR = HV_PWM_PERIOD_MTR_L - (MotorSpeed * MTR_UNIT_PERIOD_L);
				pwm_change_timebase(eng_hw->mainmotor_voltage, 0);
			}
			break;
		case MTR_MODE_LAMINATE:
			if (motorType == MOTOR_NEW) {
				CHANGED_PWM_PEROID_MTR = HV_PWM_PERIOD_MTR_LAMINATE_NEW;
			} else {
				CHANGED_PWM_PEROID_MTR = HV_PWM_PERIOD_MTR_LAMINATE;
			}
			pwm_change_timebase(eng_hw->mainmotor_voltage, 1);
			break;
		default:
			MSG("(eng)Unknown Motor speed mode (%d) : Setting Default Normal Motor speed\n", fPOR_MotorRun);
#if MAC_TYPE_M
			if (motorType == MOTOR_NEW) {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD_M_NEW);
			} else {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD_M);
			}
#else
			if (motorType == MOTOR_NEW) {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD_NEW);
			} else {
				CHANGED_PWM_PEROID_MTR = hv_pwm_period_mtr - (MotorSpeed * MTR_UNIT_PERIOD);
			}
#endif
			pwm_change_timebase(eng_hw->mainmotor_voltage, 0);
			break;
	}
	CHANGED_PWM_PER_CONV_MTR = CHANGED_PWM_PEROID_MTR/100;
	
	first_motor_cycle = (Uint16)(CHANGED_PWM_PER_CONV_MTR * 50);
	second_motor_cycle = (Uint16)(CHANGED_PWM_PEROID_MTR - first_motor_cycle);
	pwm_change_duty_cycle(eng_hw->mainmotor_voltage, first_motor_cycle, second_motor_cycle);
	MSG("(eng)## MainMotorSpeedChange : MotorSpeed = %d, fPOR_MotorRun = %d, first_motor_cycle = %d, second_motor_cycle =%d ##\n", MotorSpeed, fPOR_MotorRun, first_motor_cycle, second_motor_cycle);
	
	StartFuserRuntime(cProcessCounter, fPOR_MotorRun);
}

void MirrorMotorSpeedChange(int MirrorMotorSpeed, Uint8 fPOR_MirrorMotorRun)
{	
	switch (fPOR_MirrorMotorRun)
	{
		case MTR_MODE_NORMAL:
#if MAC_TYPE_L
			dec_laser_config_change(SPEED_45PPM); // 45ppm
#elif MAC_TYPE_M
			dec_laser_config_change(SPEED_40PPM); // 40ppm
#else
			dec_laser_config_change(SPEED_45PPM); // 45ppm
#endif
			break;
		case MTR_MODE_QUIET:
			dec_laser_config_change(SPEED_25PPM); // 25ppm
			break;
		default:
			break;
	}
#if ENG_TRACE	
	MSG("(Eng)## MirrorMotorSpeedChange :  MirrorMotorSpeed = %d, fPOR_MirrorMotorRun = %d ##\n",MirrorMotorSpeed, fPOR_MirrorMotorRun);
#endif
}

void StartFuserRuntime(int time, Uint8 motormode)
{
	switch(motormode)
	{
		case MTR_MODE_NORMAL:
			NormalFuserRunTime.start = time;
			break;
		
		case MTR_MODE_QUIET:
			LowFuserRunTime.start = time;
			break;
		
		default:
			break;
	}
//	MSG("(eng)FuserRunTime:start [Normal:%d, Low:%d]\n", NormalFuserRunTime.start, LowFuserRunTime.start);
	
	return;
}

void EndFuserRuntime(int time, Uint8 motormode, int *normalRuntime, int *lowRuntime)
{
	switch(motormode)
	{
		case MTR_MODE_NORMAL:
			if(NormalFuserRunTime.start != CLEAR)
			{
				NormalFuserRunTime.end = time;
				*normalRuntime = NormalFuserRunTime.end - NormalFuserRunTime.start;
				if(LowFuserRunTime.start != CLEAR) // speedchange while running
				{
					*lowRuntime = NormalFuserRunTime.start - LowFuserRunTime.start;
				}
			}
			break;
		
		case MTR_MODE_QUIET:
			if(LowFuserRunTime.start != CLEAR)
			{
				LowFuserRunTime.end = time;
				*lowRuntime = LowFuserRunTime.end - LowFuserRunTime.start;
				if(NormalFuserRunTime.start != CLEAR) // speedchange while running
				{
					*normalRuntime = LowFuserRunTime.start - NormalFuserRunTime.start;
				}
			}
			break;
		
		default:
			break;
	}
//	MSG("(eng)FuserRunTime [Normal:%d, Low:%d]\n", *normalRuntime, *lowRuntime);
	
	NormalFuserRunTime.start = CLEAR;
	NormalFuserRunTime.end = CLEAR;
	LowFuserRunTime.start = CLEAR;
	LowFuserRunTime.end = CLEAR;
	
	return;
}
