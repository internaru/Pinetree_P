/****************************************************/
/* Fuser Device Driver Common Interface Header File */
/* Defines Commands and their argument structure    */
/* YoungJae Jang, Sindoh                            */
/****************************************************/

#ifndef _FuserInterface_H_
#define _FuserInterface_H_

#define IOCTL_MAGIC		0xE000

//#include <asm/types.h>
struct new_fuser_cntl_set {
	int		targettemp;
	int		standbytemp;
	int		pickuptemp;
	int		mtruntemp1;
	int		mtruntemp2;
	int		mtruntemp3;
	int		mtruntemp4;
	int		mtruntemp5;
	int		mtruntime1;
	int		mtruntime2;
	int		mtruntime3;
	int		mtruntime4;
	int		Ts_NM;
	int		Ts_NS;
	int		Ts_PS;
	int		Ts_PM;
	int		Ts_Off;
	int		T_On_Pre;
	int		T_On_Cur;
	int		Temp_On;
	int		Temp_Off;
	int		Ts_On_NM;
	int		Ts_On_NS;
	int		Ts_On_ZR;
	int		Ts_On_PS;
	int		Tt_NB;
	int		Tt_NM;
	int		Tt_NS;
	int		Tt_PS;
	int		Tt_PM;
	int		dTt_NB;
	int		dTt_NM;
	int		dTt_NS;
	int		dTt_PS;
	int		dTt_PM;
	int		dTt_PB;
	int		FanHighDuty;
	int		FanLowDuty;
	int		dup_target_temp;
	int		StandbyFanDelay;
	int		PwsFanRun;
	int		mainFanLow;
	int		mainFanMin;
	int		subFanLow;
	int		coldWakeHoldTemp;
	int		coldWakeHoldTime;
	int		coldWakeTargTemp;
} ; 

#if 0
typedef struct STRUCT_FUSER_CONTROL{
	unsigned char	RequestState;
	unsigned char	State;
	unsigned char	Target_Temperature;
	unsigned char	Current_Temperature;
	unsigned char	PaperJam;
	unsigned char	TempPaperJam;
	unsigned char	EM_State;
	unsigned char	fPowerSaveMode;
	unsigned char	fMakeMotorRun;
	unsigned char	fFatalError;
	unsigned char	fPOR_PageSupervisor_Active;
	unsigned char	fMainMotorState;
	unsigned char	StandbyModeType;
	unsigned char 	fFuserFault;	
	unsigned int	FuserFaultNumber;
	int				NarrowMediaCount;
	unsigned char	fDuplexJob ;		// 1: duplex , 0 : simplex
	unsigned char	fLowTemperature;
} __attribute__((packed)) Struct_Fuser_Control ; 

/* RequestState Variable*/
enum 
{
	CMD_FUSER_OFF = 0,
	CMD_FUSER_COLD_WAKE = 3,
	CMD_FUSER_STANDBY = 5,
	CMD_FUSER_PRINT = 7,
	CMD_FUSER_POWERSAVE = 8,
	CMD_FUSER_POWERLOW = 10 /* [KSF032703] add power low */		
};

/* State Variable*/
enum
{
	ST_OFF = 0,
	ST_ERROR,
	ST_WAIT_RUN,
	ST_COLD_WAKEUP,
	ST_WARMUP,
	ST_STANDBY,
	ST_HEATING,
	ST_PRINT,
	ST_POWER_SAVE,
	ST_COOL_STANDBY,
	ST_POWER_LOW	
};

/* ioctl() variable */
enum
{
	CMD_GP_TH_OUT_ON 				= IOCTL_MAGIC | 0,
	CMD_GP_TH_OUT_OFF				= IOCTL_MAGIC | 1,
	CMD_CC_GO_STANDBY_STATE			= IOCTL_MAGIC | 2,
	CMD_CC_GO_PRINT_STATE			= IOCTL_MAGIC | 3,
	CMD_CC_GO_POWERSAVE_STATE		= IOCTL_MAGIC | 4,
	CMD_CC_GO_OFF_STATE				= IOCTL_MAGIC | 5,
	CMD_RUN_MANAGER					= IOCTL_MAGIC | 6,
	CMD_CC_FAN_ON					= IOCTL_MAGIC | 7,
	CMD_CC_FAN_OFF					= IOCTL_MAGIC | 8,
	CMD_CC_FAN_80_ON				= IOCTL_MAGIC | 9,
	CMD_CC_FAN_30_ON				= IOCTL_MAGIC | 10,
	CMD_SET_PARAM					= IOCTL_MAGIC | 11,
	CMD_GET_THERMAL					= IOCTL_MAGIC | 12,
	CMD_SET_MINUS_PICKUP			= IOCTL_MAGIC | 13,
	CMD_SET_CTRL_DATA				= IOCTL_MAGIC | 14,
};
#endif
/*********************************/
/*   	Fuser ERROR CODE 	     */
/*********************************/
enum
{
	ERROR_UNDER_TEMPERATUREP_PRINTING = 300,
	ERROR_UNDER_TEMPERATUREP_STANDBY=301,
	ERROR_POR_NOT_REACH_STANDBY=302,
	ERROR_OVER_TERMERATURE=303,
	ERROR_THERMISTOR_OPEN=304 ,
	ERROR_UNDER_TEMPERATUREP_HEATING = 305,
	ERROR_LOCK_TIMEOUT = 130 ,
	ERROR_SUB_FAN_LOCK_TIMEOUT = 131,
	ERROR_DEV_FAN_LOCK_TIMEOUT = 132
};





#endif //FuserInterface
