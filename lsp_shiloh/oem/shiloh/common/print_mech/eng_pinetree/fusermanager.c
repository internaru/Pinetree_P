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

#include	"farch.h"
#include	"pioname.h"
#include 	"em.h"
#include	<stdlib.h>
#include	<stdio.h>

#include <stdbool.h>
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
#include "fusermanager.h" /* [KSF032501] for heater manager *//* [KSF050201] */
#include "Fuser_Interface.h" /* [KSF032501] for heater manager */

#if ABBA2
#define	DEFAULT_STANDBY		160
#define	DEFAULT_PRINT			180
#define	DEFAULT_PICKUP		15
#else
#define	DEFAULT_STANDBY		175 /* [KSF050201] - start */
#define	DEFAULT_PRINT			180
#define	DEFAULT_PICKUP		15
#endif

#define DBG_PRFX "fuser: " /* [KSF072301] */
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_SDH_ERR
#define WARN DBG_PRINTF_SDH_WARNING
#define MSG  DBG_PRINTF_SDH_NOTICE
#define DBG1 DBG_PRINTF_SDH_DEBUG

extern int cProcessCounter;

Struct_Fuser_Control Fuser_Control;
Struct_Fuser_Control Pre_Fuser_Control;
static unsigned char Adc_Print_Low;/* [KSF050201] - start */
static unsigned char Adc_Print;
//static unsigned char Adc_Print_Hi;/* [KSF050201] - end */
static unsigned char Param_PrintTemperature ; /* [KSF050201] - start */
static unsigned char Param_PickupTemperature ;
static unsigned char Waveform_buf[50];
#if ZX_ENABLE
static unsigned char Pre_Waveform_buf;
#endif

static int NdelayFan ; /* [KSF050303] */
static int Temp_Difference;
static unsigned int	cUnderTempPrintCount;
static unsigned char Param_StandbyTemperature;
static unsigned char ADCData; /* [KSF032604] for ADC Temp */

/* [KSF032605] for Check_error */
static unsigned char Adc_Standby;
//static unsigned char Adc_Standby_Hi;
//static unsigned char Adc_Standby_Low;
//static unsigned char Adc_Print_Low;
//static unsigned char Adc_Print;
//static unsigned char Adc_Print_Hi;
static unsigned char Param_StandbyTemperature;
//static unsigned char Param_PickupTemperature ;

static unsigned int	 FuserControlCounter;
static unsigned int	 cMainFanCounterForPOR;
static unsigned int	 cUnderTempStandbyCount;
static unsigned int	 cUnderTempPrintCount;
#if ZX_ENABLE
static	uint32_t fuserstate = 0;
//static	uint32_t Pre_fuserstate = 0;
#endif

//static unsigned int	 cStandbyTimeoutCount; /* [KSF051604] */
//static unsigned int	 cPowerlowTimeoutCount;	 /* [KSF051604] */

unsigned int fan_Counter ;
unsigned int sub_fan_Counter ;
unsigned int dev_fan_Counter ;
unsigned int fPrintToOff ; /* [KSF050302] */
unsigned int cFanStopInError;

Uint16 OldMainFanduty;
Uint16 OldSubFanduty;
Uint16 OldDevFanduty;
Uint8 mainfanoff_debug;
Uint8 subfanoff_debug;
Uint8 devfanoff_debug;
Uint8 fMainfanForPOR;

Uint8 fFanOn, fSubFanOn, fDevFanOn;

Uint8	f_mon_fan_lock ;
Uint8	f_mon_sub_fan_lock; /* [KSF050201] - end */
Uint8	f_mon_dev_fan_lock; /* [KSF050201] - end */
Uint8	fMainFanDutyUpForStandby;
Uint8	fMainFanLockFail;

extern engine_hardware_t *eng_hardware;
#if STRESS_TEST_TONER_IC
extern Uint8 MemoryWrite(Uint16, Uint32, Uint8);/* [KSR062501] */
extern Uint32 MemoryRead(Uint16, Uint8);/* [KSR062501] */
#endif
#if STRESS_TEST_TONER_IC_AUTH
extern void 	SecurityIC_Check(void); /* [KSR062601] */
#endif
int cKeepStandbyCnt; /* [KSF032605] for heater manager *//* [KSF050201] - start */
Uint8 StandbyCase;
Uint8 fUnderTempPrint;
Uint8 fEnterStandby;

#if EP_PARAM_TEST
extern Uint8	Param_Pick_VALUE;
extern Uint8	fParamPickVALUE;
extern Uint8	Param_Target_VALUE;
extern Uint8	fParamTargetVALUE;
extern Uint8	Param_Motor_VALUE;
extern Uint8	fParamMotorVALUE;
#endif

extern Uint8 f24vOnForJogMode;
struct new_fuser_cntl_set	fuser_cntl_set ;
unsigned int WaveformDuty;
unsigned int nFuser_Counter;
unsigned int cMotorRunStart;
int New_AvgTemp;

unsigned int OndoNo;

/* [KSF061301] : 305 */
unsigned char heatingFlag;
unsigned char TotalHeatingFlag;
unsigned int heatingCount;
unsigned int totalHeatingCount;
unsigned int heatingBeforeTemp;
unsigned int heatingAfterTemp; 
unsigned int heatingErrorCount;

extern Uint8 FanCtrlEnvIdx;
extern Uint8 DEBUG_StandbyTemp;

/* [KSF072301] : 302 */
unsigned char TotalWarmupFlag;
unsigned int totalWarmupCount;

extern Uint8 machineType;
extern Uint8 Machine_TLI;

extern Uint8 RunEngineForLamination;

Uint8	WarmupCase_POR; /* [KSF070801] */

Uint8	fUnderTempPrint;
Uint8	ADC_buf[10];
Uint32	ADC_Avg_buf[5];
Uint8  ADC_Max;
Uint8  ADC_Min;
Uint8	ptAvg;
int	New_AvgTemp;
Uint32	Old_AvgTemp; 
Uint32 ADC_Avg;
Uint32 sum_xy;
Uint32 sum_y;
Uint8	StandbyCase;
Uint8	fEnterStandby;
Uint8	cFanForCooling;
int cKeepStandbyCnt;
int cOnPreCnt;
char fFanCtrl24vOff;

#if FAN_TEST
Uint16 FanOff;
Uint16 MainFanMax;
Uint16 MainFanHigh;
Uint16 MainFanMid;
Uint16 MainFanLow;
Uint16 MainFanMin;
Uint16 SubFanMax;
Uint16 SubFanLow;
#else
Uint8 MainFanMax;
Uint8 MainFanHigh;
Uint8 MainFanMid;
Uint8 MainFanLow;
Uint8 MainFanMin;
Uint8 SubFanMax;
Uint8 SubFanLow;
#endif
Uint8	ColdWakeupCase;
Uint8 	fColdWakeupHoldingStart;
Uint32 cColdWakeupHoldingCnt;

Uint8	f_thermistoropen, f_overheat, f_understandby, f_underprinting, f_firstStandby; /* [KSF032605] for Check_error */
unsigned int n_thermistoropenCount, n_understandbyCount, n_underprintingCount, n_firstStandbyTime, n_thermistorOverTempCount ; /* [KSF032605] for Check_error */

Uint8	fFuserOn;
extern int Fuser_Setting_Parameters[NumberOfFuserParameter];
extern int Engine_Parameter[NumberOfEngineParameter]; /* [KSF050201] - end */
#if KSF_DBG_SET_DUTY
#define NUM_FUS_PARAM 42
int32_t fus_param[NUM_FUS_PARAM];
#if KSF_DBG_SET_DUTY_WH
int32_t fus_paramWh[NUM_FUS_PARAM];
#endif
#endif
Uint32 ChangeVoltageToTemp(void);
Uint32 ChangeVoltageToTempPrint(void);

extern void	PS_24vOff(char);
void Read_Fuser_Func(void);
void Write_Fuser_Func(void);

extern Uint8	Get_MainFanLockState(void);
extern Uint8	Get_SubFanLockState(void);
extern Uint8	Get_DevFanLockState(void);

unsigned char Get_ADCData(void) /* [KSF032604] for ADC Temp *//* [KSF050201] - start */
{
	Uint32 voltage;
	engine_hardware_t* eng_hw = eng_hardware;
	
	voltage = dec_adc_scaled_read_channel_mV(eng_hw->thermistor);
	voltage = voltage / 10;	
	
	return voltage;
} /* [KSF050201] - end */

Uint32 ChangeVoltageToTemp(void)/* ChangeADCtoTemp(ADCvalue) */
{
	engine_hardware_t* eng_hw = eng_hardware;
	Uint32 voltage;
	
	voltage = dec_adc_scaled_read_channel_mV(eng_hw->thermistor);
	voltage = voltage / 10;
#if KSF_DBG_THERM
	if( ( nFuser_Counter % 50 ) == 0 )
	{
		printf("[HW] thermistor => %d\n",voltage); /* Req HW */	
	}
#endif

#if 0 /* old */
	if(voltage >=199) 			return 20;
	
	switch(voltage)
	{
		case 198:			return 23;
		case 197:			return 42;
		case 196:			return 55;
		case 195:			return 64;
		case 194:			return 72;
		case 193:			return 79;
		case 192:			return 84;
		case 191:			return 90;
		case 190:			return 94;
		case 189:			return 99;
		case 188:			return 103;
		case 187:			return 106;
		case 186:			return 110;
		case 185:			return 113;
		case 184:			return 117;
		case 183:			return 121;
		case 182:			return 123;
		case 181:			return 125;
		case 180:			return 128;
		case 179:			return 131;
		case 178:			return 133;
		case 177:			return 136;
		case 176:			return 138;
		case 175:			return 140;
		case 174:			return 143;
		case 173:			return 145;
		case 172:			return 147;
		case 171:			return 149;
		case 170:			return 151;
		case 169:			return 153;
		case 168:			return 155;
		case 167:			return 157;
		case 166:			return 159;
		case 165:			return 161;
		case 164:			return 163;
		case 163:			return 165;
		case 162:			return 166;
		case 161:			return 168;
		case 160:			return 170;
		case 159:			return 172;
		case 158:			return 173;
		case 157:			return 175;
		case 156:			return 177;
		case 155:			return 178;
		case 154:			return 180;
		case 153:			return 182;
		case 152:			return 183;
		case 151:			return 185;
		case 150:			return 186;
		case 149:			return 188;
		case 148:			return 190;
		case 147:			return 191;
		case 146:			return 193;
		case 145:			return 194;
		case 144:			return 196;
		case 143:			return 198;
		case 142:			return 199;
		case 141:			return 200;
		case 140:			return 202;
		case 139:			return 203;
		case 138:			return 205;
		case 137:			return 206;
		case 136:			return 208;
		case 135:			return 209;
		case 134:			return 211;
		case 133:			return 212;
		case 132:			return 214;
		case 131:			return 215;
		case 130:			return 217;
		case 129:			return 218;
		case 128:			return 220;
		case 127:			return 221;
		case 126:			return 223;
		case 125:			return 224;
		case 124:			return 225;
		case 123:			return 227;
		case 122:			return 228;
		case 121:			return 230;
		case 120:			return 231;
		case 119:			return 233;
		case 118:			return 234;
		case 117:			return 236;
		case 116:			return 237;
		case 115:			return 239;
		case 114:			return 240;
		case 113:			return 242;
		case 112:			return 243;
		case 111:			return 245;
		case 110:			return 246;
		case 109:			return 248;
		case 108:			return 249;
		
		default:			return 255;
	}
#endif /* old */

#if 1 /* [KSF082101] */
	if(voltage >=199) 			return 8;
	
	switch(voltage)
	{
		case 198:			return 23;
		case 197:			return 42;
		case 196:			return 54;
		case 195:			return 64;
		case 194:			return 71;
		case 193:			return 78;		
		case 192:			return 83;
		case 191:			return 89;
		case 190:			return 93;
		case 189:			return 98;
		case 188:			return 101;
		case 187:			return 105;
		case 186:			return 109;
		case 185:			return 112;
		case 184:			return 115;
		case 183:			return 118;
		case 182:			return 121;
		case 181:			return 124;
		case 180:			return 127;
		case 179:			return 129;
		case 178:			return 132;
		case 177:			return 134;
		case 176:			return 137;
		case 175:			return 139;
		case 174:			return 141;
		case 173:			return 143;
		case 172:			return 145;
		case 171:			return 147;
		case 170:			return 149;
		case 169:			return 151;
		case 168:			return 153;
		case 167:			return 155;
		case 166:			return 157;
		case 165:			return 159;
		case 164:			return 161;
		case 163:			return 163;
		case 162:			return 165;
		case 161:			return 166;
		case 160:			return 168;
		case 159:			return 170;
		case 158:			return 171;
		case 157:			return 173;
		case 156:			return 175;
		case 155:			return 176;
		case 154:			return 178;
		case 153:			return 180;
		case 152:			return 181;
		case 151:			return 183;
		case 150:			return 184;
		case 149:			return 186;
		case 148:			return 187;
		case 147:			return 189;
		case 146:			return 191;
		case 145:			return 192;
		case 144:			return 194;
		case 143:			return 195;
		case 142:			return 197;
		case 141:			return 198;
		case 140:			return 200;
		case 139:			return 201;
		case 138:			return 202;
		case 137:			return 204;
		case 136:			return 205;
		case 135:			return 207;
		case 134:			return 208;
		case 133:			return 210;
		case 132:			return 211;
		case 131:			return 213;
		case 130:			return 214;
		case 129:			return 215;
		case 128:			return 217;
		case 127:			return 218;
		case 126:			return 220;
		case 125:			return 221;
		case 124:			return 223;
		case 123:			return 224;
		case 122:			return 225;
		case 121:			return 227;
		case 120:			return 228;
		case 119:			return 230;
		case 118:			return 231;
		case 117:			return 233;
		case 116:			return 234;
		case 115:			return 236;
		case 114:			return 237;
		case 113:			return 238;
		case 112:			return 240;
		case 111:			return 241;
		case 110:			return 243;
		case 109:			return 244;
		case 108:			return 246;
		case 107:			return 247;
		case 106:			return 249;
		case 105:			return 250;
		case 104:			return 251;
		case 103:			return 252;
		case 102:			return 253;				
		case 101:			return 254;		
		
		default:			return 255;
	}
#endif /* [KSF082101] new therm */
}

Uint32 ChangeVoltageToTempPrint(void)/* ChangeADCtoTemp(ADCvalue) */
{
	engine_hardware_t* eng_hw = eng_hardware;
	Uint32 voltage;
	
	voltage = dec_adc_scaled_read_channel_mV(eng_hw->thermistor);
	voltage = voltage / 10;
#if KSF_DBG_THERM
	if( ( nFuser_Counter % 50 ) == 0 )
	{
		printf("[HW-2] thermistor2 -> %d\n",voltage); /* Req HW */	
	}
#endif

#if 1 /* old */
	if(voltage >=199) 			return 20;
	
	switch(voltage)
	{
		case 198:			return 23;
		case 197:			return 42;
		case 196:			return 55;
		case 195:			return 64;
		case 194:			return 72;
		case 193:			return 79;
		case 192:			return 84;
		case 191:			return 90;
		case 190:			return 94;
		case 189:			return 99;
		case 188:			return 103;
		case 187:			return 106;
		case 186:			return 110;
		case 185:			return 113;
		case 184:			return 117;
		case 183:			return 121;
		case 182:			return 123;
		case 181:			return 125;
		case 180:			return 128;
		case 179:			return 131;
		case 178:			return 133;
		case 177:			return 136;
		case 176:			return 138;
		case 175:			return 140;
		case 174:			return 143;
		case 173:			return 145;
		case 172:			return 147;
		case 171:			return 149;
		case 170:			return 151;
		case 169:			return 153;
		case 168:			return 155;
		case 167:			return 157;
		case 166:			return 159;
		case 165:			return 161;
		case 164:			return 163;
		case 163:			return 165;
		case 162:			return 166;
		case 161:			return 168;
		case 160:			return 170;
		case 159:			return 172;
		case 158:			return 173;
		case 157:			return 175;
		case 156:			return 177;
		case 155:			return 178;
		case 154:			return 180;
		case 153:			return 182;
		case 152:			return 183;
		case 151:			return 185;
		case 150:			return 186;
		case 149:			return 188;
		case 148:			return 190;
		case 147:			return 191;
		case 146:			return 193;
		case 145:			return 194;
		case 144:			return 196;
		case 143:			return 198;
		case 142:			return 199;
		case 141:			return 200;
		case 140:			return 202;
		case 139:			return 203;
		case 138:			return 205;
		case 137:			return 206;
		case 136:			return 208;
		case 135:			return 209;
		case 134:			return 211;
		case 133:			return 212;
		case 132:			return 214;
		case 131:			return 215;
		case 130:			return 217;
		case 129:			return 218;
		case 128:			return 220;
		case 127:			return 221;
		case 126:			return 223;
		case 125:			return 224;
		case 124:			return 225;
		case 123:			return 227;
		case 122:			return 228;
		case 121:			return 230;
		case 120:			return 231;
		case 119:			return 233;
		case 118:			return 234;
		case 117:			return 236;
		case 116:			return 237;
		case 115:			return 239;
		case 114:			return 240;
		case 113:			return 242;
		case 112:			return 243;
		case 111:			return 245;
		case 110:			return 246;
		case 109:			return 248;
		case 108:			return 249;
		
		default:			return 255;
	}
#endif /* old */

#if 0 /* [KSF080902] new therm (adj:-8) */
	if(voltage >=203) 			return 26;
	
	switch(voltage)
	{
		case 202:			return 34;
		case 201:			return 46;
		case 200:			return 56;
		case 199:			return 63;
		case 198:			return 70;
		case 197:			return 75;		
		case 196:			return 81;
		case 195:			return 85;
		case 194:			return 90;
		case 193:			return 93;
		case 192:			return 97;
		case 191:			return 101;
		case 190:			return 104;
		case 189:			return 107;
		case 188:			return 110;
		case 187:			return 113;
		case 186:			return 116;
		case 185:			return 119;
		case 184:			return 121;
		case 183:			return 124;
		case 182:			return 126;
		case 181:			return 128;
		case 180:			return 131;
		case 179:			return 133;
		case 178:			return 135;
		case 177:			return 137;
		case 176:			return 139;
		case 175:			return 141;
		case 174:			return 143;
		case 173:			return 145;
		case 172:			return 147;
		case 171:			return 149;
		case 170:			return 151;
		case 169:			return 153;
		case 168:			return 155;
		case 167:			return 157;
		case 166:			return 158;
		case 165:			return 160;
		case 164:			return 162;
		case 163:			return 163;
		case 162:			return 165;
		case 161:			return 167;
		case 160:			return 168;
		case 159:			return 170;
		case 158:			return 172;
		case 157:			return 173;
		case 156:			return 175;
		case 155:			return 176;
		case 154:			return 178;
		case 153:			return 179;
		case 152:			return 181;
		case 151:			return 182;
		case 150:			return 184;
		case 149:			return 185;
		case 148:			return 187;
		case 147:			return 188;
		case 146:			return 190;
		case 145:			return 192;
		case 144:			return 193;
		case 143:			return 195;
		case 142:			return 196;
		case 141:			return 197;
		case 140:			return 199;
		case 139:			return 200;
		case 138:			return 202;
		case 137:			return 203;
		case 136:			return 205;
		case 135:			return 206;
		case 134:			return 207;
		case 133:			return 209;
		case 132:			return 210;
		case 131:			return 212;
		case 130:			return 213;
		case 129:			return 215;
		case 128:			return 216;
		case 127:			return 217;
		case 126:			return 219;
		case 125:			return 220;
		case 124:			return 222;
		case 123:			return 223;
		case 122:			return 225;
		case 121:			return 226;
		case 120:			return 227;
		case 119:			return 229;
		case 118:			return 230;
		case 117:			return 232;
		case 116:			return 233;
		case 115:			return 235;
		case 114:			return 236;
		case 113:			return 238;
		case 112:			return 239;
		case 111:			return 241;
		case 110:			return 242;
		case 109:			return 243;
		case 108:			return 244;
		
		default:			return 255;
	}
#endif /* [KSF080902] new therm */

#if 0 /* [KSF082001] */
	if(voltage >=199) 			return 5;
	
	switch(voltage)
	{
		case 198:			return 20;
		case 197:			return 39;
		case 196:			return 51;
		case 195:			return 61;
		case 194:			return 68;
		case 193:			return 75;		
		case 192:			return 80;
		case 191:			return 86;
		case 190:			return 90;
		case 189:			return 95;
		case 188:			return 98;
		case 187:			return 102;
		case 186:			return 106;
		case 185:			return 109;
		case 184:			return 112;
		case 183:			return 115;
		case 182:			return 118;
		case 181:			return 121;
		case 180:			return 124;
		case 179:			return 126;
		case 178:			return 129;
		case 177:			return 131;
		case 176:			return 133;
		case 175:			return 136;
		case 174:			return 138;
		case 173:			return 140;
		case 172:			return 142;
		case 171:			return 144;
		case 170:			return 146;
		case 169:			return 148;
		case 168:			return 150;
		case 167:			return 152;
		case 166:			return 154;
		case 165:			return 156;
		case 164:			return 158;
		case 163:			return 160;
		case 162:			return 162;
		case 161:			return 163;
		case 160:			return 165;
		case 159:			return 167;
		case 158:			return 168;
		case 157:			return 170;
		case 156:			return 172;
		case 155:			return 173;
		case 154:			return 175;
		case 153:			return 177;
		case 152:			return 178;
		case 151:			return 180;
		case 150:			return 181;
		case 149:			return 183;
		case 148:			return 184;
		case 147:			return 186;
		case 146:			return 188;
		case 145:			return 189;
		case 144:			return 191;
		case 143:			return 192;
		case 142:			return 194;
		case 141:			return 195;
		case 140:			return 197;
		case 139:			return 198;
		case 138:			return 199;
		case 137:			return 201;
		case 136:			return 202;
		case 135:			return 204;
		case 134:			return 205;
		case 133:			return 207;
		case 132:			return 208;
		case 131:			return 210;
		case 130:			return 211;
		case 129:			return 212;
		case 128:			return 214;
		case 127:			return 215;
		case 126:			return 217;
		case 125:			return 218;
		case 124:			return 220;
		case 123:			return 221;
		case 122:			return 222;
		case 121:			return 224;
		case 120:			return 225;
		case 119:			return 227;
		case 118:			return 228;
		case 117:			return 230;
		case 116:			return 231;
		case 115:			return 233;
		case 114:			return 234;
		case 113:			return 235;
		case 112:			return 237;
		case 111:			return 238;
		case 110:			return 240;
		case 109:			return 241;
		case 108:			return 243;
		case 107:			return 244;
		case 106:			return 246;
		case 105:			return 247;
		case 104:			return 248;
		case 103:			return 249;
		case 102:			return 250;				
		case 101:			return 251;
		case 100:			return 252;		
		case  99:			return 253;
		case  98:			return 254;			
		
		default:			return 255;
	}
#endif /* [KSF082001] new therm */

#if 0 /* [KSF082101] */
	if(voltage >=199) 			return 8;
	
	switch(voltage)
	{
		case 198:			return 23;
		case 197:			return 42;
		case 196:			return 54;
		case 195:			return 64;
		case 194:			return 71;
		case 193:			return 78;		
		case 192:			return 83;
		case 191:			return 89;
		case 190:			return 93;
		case 189:			return 98;
		case 188:			return 101;
		case 187:			return 105;
		case 186:			return 109;
		case 185:			return 112;
		case 184:			return 115;
		case 183:			return 118;
		case 182:			return 121;
		case 181:			return 124;
		case 180:			return 127;
		case 179:			return 129;
		case 178:			return 132;
		case 177:			return 134;
		case 176:			return 137;
		case 175:			return 139;
		case 174:			return 141;
		case 173:			return 143;
		case 172:			return 145;
		case 171:			return 147;
		case 170:			return 149;
		case 169:			return 151;
		case 168:			return 153;
		case 167:			return 155;
		case 166:			return 157;
		case 165:			return 159;
		case 164:			return 161;
		case 163:			return 163;
		case 162:			return 165;
		case 161:			return 166;
		case 160:			return 168;
		case 159:			return 170;
		case 158:			return 171;
		case 157:			return 173;
		case 156:			return 175;
		case 155:			return 176;
		case 154:			return 178;
		case 153:			return 180;
		case 152:			return 181;
		case 151:			return 183;
		case 150:			return 184;
		case 149:			return 186;
		case 148:			return 187;
		case 147:			return 189;
		case 146:			return 191;
		case 145:			return 192;
		case 144:			return 194;
		case 143:			return 195;
		case 142:			return 197;
		case 141:			return 198;
		case 140:			return 200;
		case 139:			return 201;
		case 138:			return 202;
		case 137:			return 204;
		case 136:			return 205;
		case 135:			return 207;
		case 134:			return 208;
		case 133:			return 210;
		case 132:			return 211;
		case 131:			return 213;
		case 130:			return 214;
		case 129:			return 215;
		case 128:			return 217;
		case 127:			return 218;
		case 126:			return 220;
		case 125:			return 221;
		case 124:			return 223;
		case 123:			return 224;
		case 122:			return 225;
		case 121:			return 227;
		case 120:			return 228;
		case 119:			return 230;
		case 118:			return 231;
		case 117:			return 233;
		case 116:			return 234;
		case 115:			return 236;
		case 114:			return 237;
		case 113:			return 238;
		case 112:			return 240;
		case 111:			return 241;
		case 110:			return 243;
		case 109:			return 244;
		case 108:			return 246;
		case 107:			return 247;
		case 106:			return 249;
		case 105:			return 250;
		case 104:			return 251;
		case 103:			return 252;
		case 102:			return 253;				
		case 101:			return 254;		
		
		default:			return 255;
	}
#endif /* [KSF082101] new therm */
}

void init_temperature(void) /* call Fuser_open *//* [KSF050201] - start */
{
	Param_StandbyTemperature 	=  DEFAULT_STANDBY ; /* 175 */
	Param_PrintTemperature 		=  DEFAULT_PRINT ; /* 180 */
	Param_PickupTemperature 	=  DEFAULT_PICKUP ; /* 15 */

#if EP_PARAM_TEST
	if(fParamPickVALUE == SET)
		Param_PickupTemperature = Fuser_Setting_Parameters[2]  = Param_Pick_VALUE;
	else
		Param_PickupTemperature = Fuser_Setting_Parameters[2]  = FUSER_PARAM_PICKUP_TMP;
#else
	Param_PickupTemperature = Fuser_Setting_Parameters[2]  = FUSER_PARAM_PICKUP_TMP;
#endif
	Adc_Standby = DEFAULT_STANDBY ;
#if EP_PARAM_TEST
	if(fParamTargetVALUE == SET)
		Adc_Print_Low = Param_Target_VALUE-Param_PickupTemperature; /* bufParam[0] - bufParam[2] */
	else
		Adc_Print_Low = Param_PrintTemperature-Param_PickupTemperature; /* bufParam[0] - bufParam[2] */
#else
	Adc_Print_Low = Param_PrintTemperature-Param_PickupTemperature; /* bufParam[0] - bufParam[2] */
#endif
	Adc_Print = DEFAULT_PRINT;
}

void init_fuser_state(void)
{
	Fuser_Control.State = ST_OFF ;	
	Fuser_Control.RequestState	= CMD_FUSER_STANDBY ;
	Fuser_Control.fPOR_PageSupervisor_Active = SET;
	nFuser_Counter = 0 ;
	cMotorRunStart = 0 ;
	FuserControlCounter = 0 ;
	n_firstStandbyTime = nFuser_Counter ;
	NdelayFan = 0;
	cUnderTempStandbyCount = 0;
//	cStandbyTimeoutCount = 0; /* [KSF051604] */
//	cPowerlowTimeoutCount = 0;	/* [KSF051604] */
	cUnderTempPrintCount = 0;
	
	ptAvg = 0;
	Old_AvgTemp = 0;
	New_AvgTemp = 0;
	WaveformDuty = 4;
	cKeepStandbyCnt = 0;
	StandbyCase = 1;
	SetWaveform_0();
	fEnterStandby = 0;
	cFanForCooling = 0;
	fPrintToOff = 0; /* [KSF050302] */
	cFanStopInError = 0;
	
#if FAN_TEST
	FanOff = FAN_0;
#endif
	MainFanMax = FAN_99;
	MainFanHigh = FAN_50;
	MainFanMid = FAN_13;
	MainFanLow = FAN_1;
#if FAN_TEST
	MainFanMin = FAN_0_5;	
#endif
	SubFanMax = FAN_99;
	SubFanLow = FAN_5;	
	
	OldMainFanduty = 0;
	OldSubFanduty = 0;
	OldDevFanduty = 0;
	
	mainfanoff_debug = TRUE;
	subfanoff_debug = TRUE;
	devfanoff_debug = TRUE;
	
	fMainfanForPOR = TRUE;
	fMainFanDutyUpForStandby = FALSE;
	fMainFanLockFail = FALSE;
	
	cOnPreCnt = 0;
	fFanCtrl24vOff = CLEAR;
}
#if ( KSF_DBG_SET_DUTY | KSF_DBG_SET_DUTY_WH )
void selectPrintDutyTable( int duty )
{
	if( duty == 100 )
	{
		SetWaveform_100();
	}
	else if( duty == 90 )
	{
		SetWaveform_90();		
	}
	else if( duty == 80 )
	{
		SetWaveform_80();		
	}		
	else if( duty == 75 )
	{
		SetWaveform_75();		
	}
	else if( duty == 70 )
	{
		SetWaveform_70();		
	}
	else if( duty == 60 )
	{
		SetWaveform_60();		
	}
	else if( duty == 50 )
	{
		SetWaveform_50();		
	}
	else if( duty == 40 )
	{
		SetWaveform_40();		
	}	
	else if( duty == 33 )
	{
		SetWaveform_33();		
	}
	else if( duty == 30 )
	{
		SetWaveform_30();		
	}	
	else if( duty == 25 )
	{
		SetWaveform_25();		
	}
	else if( duty == 20 )
	{
		SetWaveform_20();		
	}
	else if( duty == 16 )
	{
		SetWaveform_16();		
	}	
	else if( duty == 10 )
	{
		SetWaveform_10();		
	}
	else if( duty == 0 )
	{
		SetWaveform_0();		
	}
	else
	{
		printf("[ERR] undefined duty:%d => Set 0 [pc]\n",duty);
		SetWaveform_0();		
	}
}
#endif

#if KSF_DBG_SET_DUTY
void getTestFuserPrintDutyTableValue(){
	FILE	*paramFP;
    char white1[200];
    int i;
    
    paramFP = fopen("./FUSER_PARAMETER.txt", "r");
    if(paramFP == NULL)
    {
		printf("[FUS] FUSER_PARAMETER.txt File Open Fail !!");
		for( i = 0; i < NUM_FUS_PARAM; i++ )
		{
			fus_param[i] = 80;
		}
		printf(" => Change All duty : %d\n",fus_param[0]);		
	}
	else
	{
		for( i = 0; i < NUM_FUS_PARAM; i++ )
		{
			fscanf( paramFP, "%s%d",white1, &fus_param[i]); 	
		}		
		fclose(paramFP);			
	}
	
	DBG1("##################################\n");
	DBG1("##	A1	=	%d	##\n", fus_param[0]);
	DBG1("##	A2	=	%d	##\n", fus_param[1]);
	DBG1("##	A3	=	%d	##\n", fus_param[2]);
	DBG1("##	A4	=	%d	##\n", fus_param[3]);
	DBG1("##	A5	=	%d	##\n", fus_param[4]);
	DBG1("##	A6	=	%d	##\n", fus_param[5]);
	DBG1("##	A7	=	%d	##\n", fus_param[6]);

	DBG1("##	B1	=	%d	##\n", fus_param[7]);
	DBG1("##	B2	=	%d	##\n", fus_param[8]);
	DBG1("##	B3	=	%d	##\n", fus_param[9]);
	DBG1("##	B4	=	%d	##\n", fus_param[10]);
	DBG1("##	B5	=	%d	##\n", fus_param[11]);
	DBG1("##	B6	=	%d	##\n", fus_param[12]);
	DBG1("##	B7	=	%d	##\n", fus_param[13]);

	DBG1("##	C1	=	%d	##\n", fus_param[14]);
	DBG1("##	C2	=	%d	##\n", fus_param[15]);
	DBG1("##	C3	=	%d	##\n", fus_param[16]);
	DBG1("##	C4	=	%d	##\n", fus_param[17]);
	DBG1("##	C5	=	%d	##\n", fus_param[18]);
	DBG1("##	C6	=	%d	##\n", fus_param[19]);
	DBG1("##	C7	=	%d	##\n", fus_param[20]);

	DBG1("##	D1	=	%d	##\n", fus_param[21]);
	DBG1("##	D2	=	%d	##\n", fus_param[22]);
	DBG1("##	D3	=	%d	##\n", fus_param[23]);
	DBG1("##	D4	=	%d	##\n", fus_param[24]);
	DBG1("##	D5	=	%d	##\n", fus_param[25]);
	DBG1("##	D6	=	%d	##\n", fus_param[26]);
	DBG1("##	D7	=	%d	##\n", fus_param[27]);

	DBG1("##	E1	=	%d	##\n", fus_param[28]);
	DBG1("##	E2	=	%d	##\n", fus_param[29]);
	DBG1("##	E3	=	%d	##\n", fus_param[30]);
	DBG1("##	E4	=	%d	##\n", fus_param[31]);
	DBG1("##	E5	=	%d	##\n", fus_param[32]);
	DBG1("##	E6	=	%d	##\n", fus_param[33]);
	DBG1("##	E7	=	%d	##\n", fus_param[34]);

	DBG1("##	F1	=	%d	##\n", fus_param[35]);
	DBG1("##	F2	=	%d	##\n", fus_param[36]);
	DBG1("##	F3	=	%d	##\n", fus_param[37]);
	DBG1("##	F4	=	%d	##\n", fus_param[38]);
	DBG1("##	F5	=	%d	##\n", fus_param[39]);
	DBG1("##	F6	=	%d	##\n", fus_param[40]);
	DBG1("##	F7	=	%d	##\n", fus_param[41]);	
	DBG1("##################################\n");	
}
#endif

#if KSF_DBG_SET_DUTY_WH
void getTestFuserWarmupHeatingDutyTableValue(){ /* [KSF090601] */
	FILE	*paramFP;
    char white1[200];
    int i;
    
    paramFP = fopen("./FUSER_PARAMETER_WH.txt", "r");
    if(paramFP == NULL)
    {
		printf("[FUS] FUSER_PARAMETER_WH.txt File Open Fail !!");
		for( i = 0; i < NUM_FUS_PARAM; i++ )
		{
			fus_paramWh[i] = 90;
		}
		printf(" => Change All duty : %d\n",fus_paramWh[0]);		
	}
	else
	{
		for( i = 0; i < NUM_FUS_PARAM; i++ )
		{
			fscanf( paramFP, "%s%d",white1, &fus_paramWh[i]); 	
		}		
		fclose(paramFP);			
	}
	
	DBG1("##################################\n");
	DBG1("##	G1	=	%d	##\n", fus_paramWh[0]);
	DBG1("##	G2	=	%d	##\n", fus_paramWh[1]);
	DBG1("##	G3	=	%d	##\n", fus_paramWh[2]);
	DBG1("##	G4	=	%d	##\n", fus_paramWh[3]);
	DBG1("##	G5	=	%d	##\n", fus_paramWh[4]);
	DBG1("##	G6	=	%d	##\n", fus_paramWh[5]);
	DBG1("##	G7	=	%d	##\n", fus_paramWh[6]);

	DBG1("##	H1	=	%d	##\n", fus_paramWh[7]);
	DBG1("##	H2	=	%d	##\n", fus_paramWh[8]);
	DBG1("##	H3	=	%d	##\n", fus_paramWh[9]);
	DBG1("##	H4	=	%d	##\n", fus_paramWh[10]);
	DBG1("##	H5	=	%d	##\n", fus_paramWh[11]);
	DBG1("##	H6	=	%d	##\n", fus_paramWh[12]);
	DBG1("##	H7	=	%d	##\n", fus_paramWh[13]);

	DBG1("##	I1	=	%d	##\n", fus_paramWh[14]);
	DBG1("##	I2	=	%d	##\n", fus_paramWh[15]);
	DBG1("##	I3	=	%d	##\n", fus_paramWh[16]);
	DBG1("##	I4	=	%d	##\n", fus_paramWh[17]);
	DBG1("##	I5	=	%d	##\n", fus_paramWh[18]);
	DBG1("##	I6	=	%d	##\n", fus_paramWh[19]);
	DBG1("##	I7	=	%d	##\n", fus_paramWh[20]);

	DBG1("##	J1	=	%d	##\n", fus_paramWh[21]);
	DBG1("##	J2	=	%d	##\n", fus_paramWh[22]);
	DBG1("##	J3	=	%d	##\n", fus_paramWh[23]);
	DBG1("##	J4	=	%d	##\n", fus_paramWh[24]);
	DBG1("##	J5	=	%d	##\n", fus_paramWh[25]);
	DBG1("##	J6	=	%d	##\n", fus_paramWh[26]);
	DBG1("##	J7	=	%d	##\n", fus_paramWh[27]);

	DBG1("##	K1	=	%d	##\n", fus_paramWh[28]);
	DBG1("##	K2	=	%d	##\n", fus_paramWh[29]);
	DBG1("##	K3	=	%d	##\n", fus_paramWh[30]);
	DBG1("##	K4	=	%d	##\n", fus_paramWh[31]);
	DBG1("##	K5	=	%d	##\n", fus_paramWh[32]);
	DBG1("##	K6	=	%d	##\n", fus_paramWh[33]);
	DBG1("##	K7	=	%d	##\n", fus_paramWh[34]);

	DBG1("##	L1	=	%d	##\n", fus_paramWh[35]);
	DBG1("##	L2	=	%d	##\n", fus_paramWh[36]);
	DBG1("##	L3	=	%d	##\n", fus_paramWh[37]);
	DBG1("##	L4	=	%d	##\n", fus_paramWh[38]);
	DBG1("##	L5	=	%d	##\n", fus_paramWh[39]);
	DBG1("##	L6	=	%d	##\n", fus_paramWh[40]);
	DBG1("##	L7	=	%d	##\n", fus_paramWh[41]);	
	DBG1("##################################\n");	
}
#endif
void init_error_flag(void) /* ST_ERROR -> error init */
{
	Fuser_Control.fFuserFault = FALSE ;	
	Fuser_Control.FuserFaultNumber = 0 ;
	////////////////////// Error Flag initialize
	f_overheat = FALSE ;
	f_thermistoropen = FALSE ;
	f_understandby = FALSE ;
	f_underprinting = FALSE ;
	fUnderTempPrint = CLEAR;
	f_firstStandby = SET ;
	n_firstStandbyTime = nFuser_Counter ;
	f_mon_fan_lock = CLEAR ;
	f_mon_sub_fan_lock = CLEAR;
	f_mon_dev_fan_lock = CLEAR;
	fan_Counter = 0;
	sub_fan_Counter = 0;
	dev_fan_Counter = 0;
} /* [KSF050201] - end */

#if FAN_TEST
void MainFan_DutyChange(Uint16 fan_duty) /* ( duty: 0 ~ 999 ) */
{
#if ZX_ENABLE
	MainFan_Off();
#else
	engine_hardware_t* eng_hw = eng_hardware;
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	Uint16 main_fan_duty;
#if BLI_TEST	
	if((fan_duty != 0) && (Fuser_Control.LowSpeedFan_Wakeup == SET))
	{
		main_fan_duty = MainFanLow;
	}
	else
	{
		main_fan_duty = fan_duty;
	}
#else
	main_fan_duty = fan_duty;
#endif
	
	first_cycle = (Uint16)(MAIN_FAN_PWM_PERIOD * main_fan_duty / 1000);
	sencond_cycle = (Uint16)(MAIN_FAN_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->main_fan, first_cycle, sencond_cycle);
	
	if(OldMainFanduty != main_fan_duty)
	{
#if FAN_LOG
		DBG1("(eng_dev)MainFan_DutyChange : %d > %d cycle_1 : %d, cycle_2 : %d, fan_Counter = %d <%d>\n", OldMainFanduty, main_fan_duty, first_cycle, sencond_cycle, fan_Counter, cProcessCounter);
#endif
	}
	OldMainFanduty = main_fan_duty;
	mainfanoff_debug = TRUE;
	
	if(main_fan_duty == 0)
		f_mon_fan_lock = CLEAR ;
	else
		f_mon_fan_lock = SET ;
#endif
}

void SubFan_DutyChange(Uint16 fan_duty) /* ( duty: 0 ~ 999 ) */
{
#if ZX_ENABLE
	SubFan_Off();
#else
	engine_hardware_t* eng_hw = eng_hardware;
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(SUB_FAN_PWM_PERIOD * fan_duty / 1000);
	sencond_cycle = (Uint16)(SUB_FAN_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->sub_fan, first_cycle, sencond_cycle);
	
	if(OldSubFanduty != fan_duty)
	{
#if 1//FAN_LOG
		DBG1("(eng_dev)SubFan_DutyChange : %d > %d cycle_1 : %d, cycle_2 : %d, sub_fan_Counter = %d <%d>\n", OldSubFanduty, fan_duty, first_cycle, sencond_cycle, sub_fan_Counter, cProcessCounter);
#endif
	}
	OldSubFanduty = fan_duty;
	subfanoff_debug = TRUE;
	
	if(fan_duty == 0)
		f_mon_sub_fan_lock = CLEAR ;
	else
		f_mon_sub_fan_lock = SET ;
#endif
}

void DevFan_DutyChange(Uint16 fan_duty) /* ( duty: 0 ~ 999 ) */
{
#if ZX_ENABLE
	DevFan_Off();
#else
	engine_hardware_t* eng_hw = eng_hardware;
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(DEV_FAN_PWM_PERIOD * fan_duty / 1000);
	sencond_cycle = (Uint16)(DEV_FAN_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->dev_fan, first_cycle, sencond_cycle);
	
	if(OldDevFanduty != fan_duty)
	{
#if 1//FAN_LOG
		DBG1("(eng_dev)DevFan_DutyChange : %d > %d cycle_1 : %d, cycle_2 : %d, dev_fan_Counter = %d <%d>\n", OldDevFanduty, fan_duty, first_cycle, sencond_cycle, dev_fan_Counter, cProcessCounter);
#endif
	}
	OldDevFanduty = fan_duty;
	devfanoff_debug = TRUE;
	
	if(fan_duty == 0)
		f_mon_dev_fan_lock = CLEAR ;
	else
		f_mon_dev_fan_lock = SET ;
#endif
}

void MainFan_Off()
{
	engine_hardware_t* eng_hw = eng_hardware;
	pwm_off(eng_hw->main_fan);
	
	if(mainfanoff_debug == TRUE)
	{
#if FAN_LOG
		DBG1("(eng_dev)MainFan_Off <%d>\n", cProcessCounter);
#endif
	}
	mainfanoff_debug = FALSE;
	OldMainFanduty = 0;
	f_mon_fan_lock = CLEAR ;
}

void SubFan_Off()
{
	engine_hardware_t* eng_hw = eng_hardware;
	pwm_off(eng_hw->sub_fan);
	
	if(subfanoff_debug == TRUE)
	{
#if 1//FAN_LOG
		DBG1("(eng_dev)SubFan_Off <%d>\n", cProcessCounter);
#endif
	}
	subfanoff_debug = FALSE;
	OldSubFanduty = 0;
	f_mon_sub_fan_lock = CLEAR ;
}

void DevFan_Off()
{
	engine_hardware_t* eng_hw = eng_hardware;
	pwm_off(eng_hw->dev_fan);
	
	if(devfanoff_debug == TRUE)
	{
#if 1//FAN_LOG
		DBG1("(eng_dev)DevFan_Off <%d>\n", cProcessCounter);
#endif
	}
	devfanoff_debug = FALSE;
	OldDevFanduty = 0;
	f_mon_dev_fan_lock = CLEAR ;
}
#else
void MainFan_DutyChange(Uint8 fan_duty) /* ( duty: 0 ~ 99 ) */
{
	engine_hardware_t* eng_hw = eng_hardware;
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(MAIN_FAN_PWM_PERIOD * fan_duty / 100);
	sencond_cycle = (Uint16)(MAIN_FAN_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->main_fan, first_cycle, sencond_cycle);
	
	if(fan_duty == 0)
		f_mon_fan_lock = CLEAR ;
	else
		f_mon_fan_lock = SET ;
}

void SubFan_DutyChange(Uint8 fan_duty) /* ( duty: 0 ~ 99 ) */
{
	engine_hardware_t* eng_hw = eng_hardware;
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(SUB_FAN_PWM_PERIOD * fan_duty / 100);
	sencond_cycle = (Uint16)(SUB_FAN_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->sub_fan, first_cycle, sencond_cycle);
	
	if(fan_duty == 0)
		f_mon_sub_fan_lock = CLEAR ;
	else
		f_mon_sub_fan_lock = SET ;
}

void DevFan_DutyChange(Uint8 fan_duty) /* ( duty: 0 ~ 99 ) */
{
	engine_hardware_t* eng_hw = eng_hardware;
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(DEV_FAN_PWM_PERIOD * fan_duty / 100);
	sencond_cycle = (Uint16)(DEV_FAN_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->dev_fan, first_cycle, sencond_cycle);
	
	if(fan_duty == 0)
		f_mon_dev_fan_lock = CLEAR ;
	else
		f_mon_dev_fan_lock = SET ;
}
#endif
#if ZX_ENABLE
void FuserOff(void) /* TurnOffHeater *//* [KSF050201] */
{
	engine_hardware_t* eng_hw = eng_hardware;
	dec_fuser_set_temp( eng_hw->fuser, 0 ); // off
	fFuserOn = 0; /* [KSF050201] - start */
}
#else
void FuserOff(void) /* TurnOffHeater *//* [KSF050201] */
{
	engine_hardware_t* eng_hw = eng_hardware;
	dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
	fFuserOn = 0; /* [KSF050201] - start */
}
#endif

void DownToOff(void)
{
	SetWaveform_0();
	FuserOff();
	// TurnOffFan();
}

void DownToPowerSave(void)
{
	SetWaveform_0();	
	FuserOff();
#if KSF_DBG_TWO	
	printf("[FUS] DownToPowerSave\n");
#endif	 /* [KSF050201] - end */
#if FAN_TEST
//	MainFan_DutyChange(FanOff);
//	SubFan_DutyChange(FanOff); /* [KSF051603] */
//	DevFan_DutyChange(FanOff); /* [KSF051603] */
	MainFan_Off();
	SubFan_Off();
	DevFan_Off();
#else
	MainFan_DutyChange(2);
	SubFan_DutyChange(4); /* [KSF051603] */
	DevFan_DutyChange(4); /* [KSF051603] */
#endif
}

Uint32 getTempFromNo(Uint32 t_no) /* [KSF050201] - start */
{
	switch(t_no)
	{
		case 1:			return 23;
		case 2:			return 42;
		case 3:			return 55;
		case 4:			return 64;
		case 5:			return 72;
		case 6:			return 79;
		case 7:			return 84;
		case 8:			return 90;
		case 9:			return 94;
		case 10:			return 99;
		case 11:			return 103;
		case 12:			return 106;
		case 13:			return 110;
		case 14:			return 113;
		case 15:			return 117;
		case 16:			return 121;
		case 17:			return 123;
		case 18:			return 125;
		case 19:			return 128;
		case 20:			return 131;
		case 21:			return 133;
		case 22:			return 136;
		case 23:			return 138;
		case 24:			return 140;
		case 25:			return 143;
		case 26:			return 145;
		case 27:			return 147;
		case 28:			return 149;
		case 29:			return 151;
		case 30:			return 153;
		case 31:			return 155;
		case 32:			return 157;
		case 33:			return 159;
		case 34:			return 161;
		case 35:			return 163;
		case 36:			return 165;
		case 37:			return 166;
		case 38:			return 168;
		case 39:			return 170;
		case 40:			return 172;
		case 41:			return 173;
		case 42:			return 175;
		case 43:			return 177;
		case 44:			return 178;
		case 45:			return 180;
		case 46:			return 182;
		case 47:			return 183;
		case 48:			return 185;
		case 49:			return 186;
		case 50:			return 188;
		case 51:			return 190;
		case 52:			return 191;
		case 53:			return 193;
		case 54:			return 194;
		case 55:			return 196;
		case 56:			return 198;
		case 57:			return 199;
		case 58:			return 200;
		case 59:			return 202;
		case 60:			return 203;
		case 61:			return 205;
		case 62:			return 206;
		case 63:			return 208;
		case 64:			return 209;
		case 65:			return 211;
		case 66:			return 212;
		case 67:			return 214;
		case 68:			return 215;
		case 69:			return 217;
		case 70:			return 218;
		case 71:			return 220;
		case 72:			return 221;
		case 73:			return 223;
		case 74:			return 224;
		case 75:			return 225;
		case 76:			return 227;
		case 77:			return 228;
		case 78:			return 230;
		case 79:			return 231;
		case 80:			return 233;
		case 81:			return 234;
		case 82:			return 236;
		case 83:			return 237;
		case 84:			return 239;
		case 85:			return 240;
		case 86:			return 242;
		case 87:			return 243;
		case 88:			return 245;
		case 89:			return 246;
		case 90:			return 248;
		case 91:			return 249;
		
		default:			return 255;
	}	
} /* [KSF050201] - end */

#if ZX_ENABLE
void FuserOn( uint32_t fuser_state ) /* TurnOnHeater *//* [KSF050201] - start */
{
	engine_hardware_t* eng_hw = eng_hardware;
	dec_fuser_set_temp( eng_hw->fuser, fuser_state );	
	fFuserOn = 1;
}
#else
void FuserOn( Uint32 t_no ) /* TurnOnHeater *//* [KSF050201] - start */
{
	engine_hardware_t* eng_hw = eng_hardware;
	dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(t_no) );	
	fFuserOn = 1;
}
#endif

void	FanManager(void)
{		
	if(	f_mon_fan_lock == SET )
	{
		if(Get_MainFanLockState() == UNLOCK)
		{
			if(fan_Counter % 50 == 0)		DBG1("(eng_dev)main_fan : fan_Counter = %d\n", fan_Counter);
			if(fan_Counter++ > FAN_LOCKDOWN_TIMEOUT)
			{
				if(fMainFanLockFail == FALSE)
				{
					fMainFanLockFail = TRUE;
					fMainFanDutyUpForStandby = TRUE;
					fan_Counter = 0;
					DBG1("(eng_dev)MainFanLock_FirstFail\n");
				}
				else
				{
#if ZX_ENABLE
/* No Error Check */
#else
					Fuser_Control.FuserFaultNumber = ERROR_LOCK_TIMEOUT ;
					Fuser_Control.fFuserFault = TRUE ;
					Fuser_Control.State = ST_ERROR;
					fMainFanLockFail = FALSE;
					error_display_forFan();
#endif
				}
			}
		}
		else
		{
			fan_Counter = 0 ;
			fMainFanLockFail = FALSE;
		}
	}
	else
	{
		fan_Counter = 0 ;
		fMainFanLockFail = FALSE;
	}
	
	if(	f_mon_sub_fan_lock == SET )
	{
		if(Get_SubFanLockState() == UNLOCK)
		{
			if(sub_fan_Counter % 50 == 0)		DBG1("(eng_dev)sub_fan : sub_fan_Counter = %d\n", sub_fan_Counter);
			if(sub_fan_Counter++ > FAN_LOCKDOWN_TIMEOUT)
			{
#if ZX_ENABLE
/* No Error Check */
#else
				sub_fan_Counter = 0 ;
				Fuser_Control.FuserFaultNumber = ERROR_SUB_FAN_LOCK_TIMEOUT ;
				Fuser_Control.fFuserFault = TRUE ;
				Fuser_Control.State = ST_ERROR;
				error_display_forFan();			
#endif
			}
		}
		else
			sub_fan_Counter = 0 ;
	}
	else
		sub_fan_Counter = 0 ;
#if (!MAC_TYPE_M && !ABBA2)
	if((machineType == ABBA_OAK_H) && (Machine_TLI != TLI_DEFAULT))
	{
		if(	f_mon_dev_fan_lock == SET )
		{
			if(Get_DevFanLockState() == UNLOCK)
			{
				if(dev_fan_Counter % 50 == 0)		DBG1("(eng_dev)dev_fan : dev_fan_Counter = %d\n", dev_fan_Counter);
				if(dev_fan_Counter++ > FAN_LOCKDOWN_TIMEOUT)
				{
#if ZX_ENABLE
/* No Error Check */
#else
					dev_fan_Counter = 0 ;
					Fuser_Control.FuserFaultNumber = ERROR_DEV_FAN_LOCK_TIMEOUT ;
					Fuser_Control.fFuserFault = TRUE ;
					Fuser_Control.State = ST_ERROR;
					error_display_forFan();			
#endif
				}
			}
			else
				dev_fan_Counter = 0 ;
		}
		else
			dev_fan_Counter = 0 ;
	}
#endif
}

Uint8 FuserControlForColdWakeUp(void)
{
	int Temp_CurToWakeUpTemp;

	switch(ColdWakeupCase)
	{
		case 1:
			Temp_CurToWakeUpTemp = New_AvgTemp - (fuser_cntl_set.coldWakeHoldTemp*100);
			if((Temp_CurToWakeUpTemp >= 0) && (fColdWakeupHoldingStart == FALSE))
			{
				fColdWakeupHoldingStart = TRUE;
				cColdWakeupHoldingCnt = 0;
			}
			else if (fColdWakeupHoldingStart == TRUE)
			{
				cColdWakeupHoldingCnt++;
			}
			
			if(cColdWakeupHoldingCnt >= fuser_cntl_set.coldWakeHoldTime)
			{
				fColdWakeupHoldingStart = FALSE;
				cColdWakeupHoldingCnt = 0;
				ColdWakeupCase = 2;
			}
#ifdef KSF_DBG_ONE
			printf("### New_AvgTemp = %d <%d> ###\n",New_AvgTemp,cColdWakeupHoldingCnt);
#endif			
			break;
		
		case 2:
			Temp_CurToWakeUpTemp = New_AvgTemp - (fuser_cntl_set.coldWakeTargTemp*100);
			break;
			
		default:
			Temp_CurToWakeUpTemp = New_AvgTemp - (fuser_cntl_set.coldWakeTargTemp*100);
			break;
	}
	
	if(Temp_CurToWakeUpTemp < 0)
	{
#if ABBA2 // Abba2
		SetWaveform_100();
#else // Kara/-S, Pinetree/-S
		FuserControlForPrint(); /* [KSF080501] */
#endif
#ifdef KSF_DBG_ONE
//		printf("### ON ###\n");
#endif		
	}
	else
	{
		SetWaveform_0();
#ifdef KSF_DBG_ONE		
//		printf("### OFF ###\n");
#endif
	}
	return 0;
} /* [KSF050201] - end */

#if ABBA2 // Abba2
void FuserControlForStandby(void)
{
	int Temp_CurToStandby;

	Temp_CurToStandby = New_AvgTemp - (Param_StandbyTemperature*100);
	StandbyCase = 1;
#if KSF_DBG_TWO
	printf("[SB]Temp_CurToStandby:%d Fuser_Control.fMainMotorState:%d Fuser_Control.StandbyModeType:%d\n",Temp_CurToStandby,Fuser_Control.fMainMotorState,Fuser_Control.StandbyModeType);
#endif
	if((Temp_CurToStandby < 0) && (Fuser_Control.fMainMotorState == 0))
	{
		SetWaveform_100();
#if KSF_DBG_THREE
		printf("### Standby100p ###\n");
#endif
		cMotorRunStart = nFuser_Counter;
		return;
	}
	else if(Fuser_Control.fMainMotorState == 1) /* Eng : Check Motor State (check KY) -> fMainMotorState ? */
	{
		switch(Fuser_Control.StandbyModeType)
		{
			case 1:
				SetWaveform_100();						
				if(Temp_CurToStandby >= 0)
				{
#if KSF_DBG_THREE
					printf("### Standby1 ###\n");
#endif
					fEnterStandby = TRUE;
				}
				break;
			
			case 2:
				SetWaveform_100();
				if(Temp_CurToStandby >= 0)
				{						
#if KSF_DBG_THREE
					printf("### Standby2 ###\n");
#endif
					fEnterStandby = TRUE;
				}
				break;
			
			case 3:
				fEnterStandby = TRUE;				
				SetWaveform_100(); /* [KSF050802] */
#if KSF_DBG_THREE
				printf("### Standby3 ###\n");
#endif
				break;
		}
	}
	else
	{
		SetWaveform_0();
#if KSF_DBG_THREE
		printf("### Standby10 : 0p ###\n");
#endif
	}
}
#else // Kara/-S, Pinetree/-S
void FuserControlForStandby(void)
{
	int Temp_CurToStandby;

	//Temp_CurToStandby = New_AvgTemp - (Param_StandbyTemperature*100);
	Temp_CurToStandby = New_AvgTemp - ( ( Param_StandbyTemperature - 10 ) * 100 ); /* [KSF052001] */
	
	StandbyCase = 1;
//	printf("### Temp_CurToStandby = %d, Temp_Difference = %d ###\n",Temp_CurToStandby,Temp_Difference);
#if KSF_DBG_TWO
	printf("[SB]Temp_CurToStandby:%d Fuser_Control.fMainMotorState:%d Fuser_Control.StandbyModeType:%d\n",Temp_CurToStandby,Fuser_Control.fMainMotorState,Fuser_Control.StandbyModeType);
#endif	
	if((Temp_CurToStandby < 0) && (Fuser_Control.fMainMotorState == 0))
	{
//		SetWaveform_100(); /* [KSF050802] */
		FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
		printf("### Standby0 ###\n");
#endif
		cMotorRunStart = nFuser_Counter;
		//return 1;
	}
	else if(Fuser_Control.fMainMotorState == 1) /* Eng : Check Motor State (check KY) -> fMainMotorState ? */
	{
		switch(Fuser_Control.StandbyModeType)
		{
			case 1:
				if((nFuser_Counter - cMotorRunStart) <= fuser_cntl_set.mtruntime1)
				{
					//SetWaveform_100(); /* [KSF050802] */
					FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
					printf("### Standby1 ###\n");
#endif
				}
				else if(((nFuser_Counter - cMotorRunStart) > fuser_cntl_set.mtruntime1) 
				&& ((nFuser_Counter - cMotorRunStart) <= (fuser_cntl_set.mtruntime1 + fuser_cntl_set.mtruntime2)))
				{
					//SetWaveform_100(); /* [KSF050802] */
					FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
					printf("### Standby2 ###\n");
#endif
				}
				else if ((nFuser_Counter - cMotorRunStart) > (fuser_cntl_set.mtruntime1 + fuser_cntl_set.mtruntime2))
				{
					if(Temp_CurToStandby < 0)
					{					
						//SetWaveform_100(); /* [KSF050802] */
						FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
						printf("### Standby3 ###\n");
#endif
					}
					else
					{						
						//SetWaveform_100(); /* [KSF050802] */
						FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
						printf("### Standby4 ###\n");
#endif
						fEnterStandby = TRUE;
					}
				}
				break;
			
			case 2:
				if((nFuser_Counter - cMotorRunStart) <= fuser_cntl_set.mtruntime3)
				{					
					//SetWaveform_100(); /* [KSF050802] */
					FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
					printf("### Standby5 ###\n");
#endif
				}
				else if(((nFuser_Counter - cMotorRunStart) > fuser_cntl_set.mtruntime3) 
				&& ((nFuser_Counter - cMotorRunStart) <= (fuser_cntl_set.mtruntime3 + fuser_cntl_set.mtruntime4)))
				{					
					//SetWaveform_100(); /* [KSF050802] */
					FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
					printf("### Standby6 ###\n");
#endif
				}
				else if ((nFuser_Counter - cMotorRunStart) > (fuser_cntl_set.mtruntime3 + fuser_cntl_set.mtruntime4))
				{
					if(Temp_CurToStandby < 0)
					{						
						//SetWaveform_100(); /* [KSF050802] */
						FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
						printf("### Standby7 ###\n");
#endif
					}
					else
					{						
						//SetWaveform_100(); /* [KSF050802] */
						FuserControlForPrint(); /* [KSF050802] */						
#if KSF_DBG_THREE
						printf("### Standby8 ###\n");
#endif
						fEnterStandby = TRUE;
					}
				}
				break;
			
			case 3:
				fEnterStandby = TRUE;				
				//SetWaveform_100(); /* [KSF050802] */
				FuserControlForPrint(); /* [KSF050802] */
#if KSF_DBG_THREE
				printf("### Standby9 ###\n");
#endif
				break;
		}
	}
	else
	{
		SetWaveform_0();
#if KSF_DBG_THREE
		printf("### Standby10 : 0p ###\n");
#endif
	}
}/* [KSF050201] - end */
#endif

#if ABBA2 // Abba2
Uint8 FuserControlForPrint(void)
{
	int Temp_CurToPrint;
	
	Temp_CurToPrint = New_AvgTemp - (Fuser_Control.Target_Temperature*100);
	cKeepStandbyCnt = 0;
	StandbyCase = 1;

	if((Temp_CurToPrint > fuser_cntl_set.Tt_NB) && (Fuser_Control.State == ST_PRINT) && (fUnderTempPrint == CLEAR))
	{
#if KSF_DBG_THREE
		DBG1("[ControlForPrint] Temp_CurToPrint = %d, Temp_Difference = %d, cUnderTempPrintCount = %d, fUnderTempPrint = %d\n",Temp_CurToPrint,Temp_Difference,cUnderTempPrintCount,fUnderTempPrint);		
#endif
		fUnderTempPrint = SET;
	}
	
	if(Temp_CurToPrint < fuser_cntl_set.Tt_NB)
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB)/* [KF042501] */		
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[0] );
#else
			SetWaveform_100();
#endif				
#if KSF_DBG_PST_PRT
			DBG1("### A1 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))/* [KF042501] */		
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[1] );
#else
			SetWaveform_100();
#endif				
#if KSF_DBG_PST_PRT
			DBG1("### A2 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))/* [KF042501] */		
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[2] );
#else
			SetWaveform_100();
#endif				
#if KSF_DBG_PST_PRT
			DBG1("### A3 ###\n");
#endif				
		}				
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))/* [KF042501] */		
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[3] );
#else
			SetWaveform_100();
#endif				
#if KSF_DBG_PST_PRT
			DBG1("### A4 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))/* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[4] );				
#else
			SetWaveform_100();
#endif				
#if KSF_DBG_PST_PRT
			DBG1("### A5 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))/* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[5] );
#else
			SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT
			DBG1("### A6 ###\n");
#endif
		}
		else /* PB < [] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[6] );
#else
			SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT
			DBG1("### A7 ###\n");
#endif
		}			
	}/* NB - END */
	//Print Temp - Current Temp : NM
	else if((Temp_CurToPrint >= fuser_cntl_set.Tt_NB) && (Temp_CurToPrint < fuser_cntl_set.Tt_NM))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB)/* [KF042501] */		
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[7] );
#else
			SetWaveform_100();
#endif				
							
#if KSF_DBG_PST_PRT
			DBG1("### B1 ###\n");
#endif				
		}		
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))/* [KF042501] */		
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[8] );
#else
			SetWaveform_100();
#endif

#if KSF_DBG_PST_PRT				
			DBG1("### B2 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))/* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[9] );
#else
			SetWaveform_75();
#endif		
#if KSF_DBG_PST_PRT				
			DBG1("### B3 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))/* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[10] );
#else
			SetWaveform_75();
#endif		
#if KSF_DBG_PST_PRT				
			DBG1("### B4 ###\n");
#endif				
		}				
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM)) /* [KF041201] *//* [KF042501] *//* [KF042901] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[11] );
#else
			SetWaveform_50();
#endif
			
#if KSF_DBG_PST_PRT				
			DBG1("### B5 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB)) /* [KF041201] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[12] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### B6 ###\n");
#endif				
		}
		else /* PB < [] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[13] );
#else
			SetWaveform_33();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### B7 ###\n");
#endif				
		}
	}
	//Print Temp - Current Temp : NS
	else if((Temp_CurToPrint >= fuser_cntl_set.Tt_NM) && (Temp_CurToPrint < fuser_cntl_set.Tt_NS))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB) /* [KF042501] */		
		{			
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[14] );
#else
			SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### C1 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM)) /* [KF041201] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[15] );
#else
			SetWaveform_75();
#endif				
#if KSF_DBG_PST_PRT				
			DBG1("### C2 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS)) /* [KF041201] *//* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[16] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### C3 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS)) /* [KF041201] *//* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[17] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### C4 ###\n");
#endif				
		}		
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[18] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### C5 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[19] );
#else
			SetWaveform_33();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### C6 ###\n");
#endif				
		}
		else /* PB < [] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[20] );
#else
			SetWaveform_25();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### C7 ###\n");
#endif				
		}
	}
	//Print Temp - Current Temp : ZR
	else if((Temp_CurToPrint >= fuser_cntl_set.Tt_NS) && (Temp_CurToPrint <= fuser_cntl_set.Tt_PS))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB)
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[21] );
#else
			SetWaveform_75();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D1 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[22] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D2 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[23] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D3 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[24] );
#else
			SetWaveform_33();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D4 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))/* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[25] );
#else
			SetWaveform_25();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D5 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))/* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[26] );
#else
			SetWaveform_25();
#endif				
#if KSF_DBG_PST_PRT				
		DBG1("### D6 ###\n");
#endif				
		}
		else /* PB < [] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[27] );
#else
			SetWaveform_0();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D7 ###\n");
#endif				
		}
	}
	//Print Temp - Current Temp : PS
	else if((Temp_CurToPrint > fuser_cntl_set.Tt_PS) && (Temp_CurToPrint <= fuser_cntl_set.Tt_PM))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB) /* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[28] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### E1 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM)) /* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[29] );
#else
			SetWaveform_50();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### E2 ###:6\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))/* KF041201 *//* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[30] );
#else
			SetWaveform_33();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### E3 ###\n");
#endif				
		}		
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS)) /* [KF042501] *//* [KF042901] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[31] );
#else
			SetWaveform_25();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### E4 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM)) /* [KF042501] *//* [KF042901] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[32] );
#else
			SetWaveform_25();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### E5 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[33] );
#else
			SetWaveform_0();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### E6 ###\n");
#endif				
		}
		else /* PB < [] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[34] );
#else
			SetWaveform_0();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### E7 ###\n");
#endif				
		}
	}
	//Print Temp - Current Temp : PM
	else
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB) /* [KF042501] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[35] );
#else
			SetWaveform_33();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### F1 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[36] );
#else
			SetWaveform_33();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### F2 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[37] );
#else
			SetWaveform_25();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### F3 ###\n");
#endif				
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[38] );
#else
			SetWaveform_0();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### F4 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[39] );
#else
			SetWaveform_0();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### F5 ###\n");
#endif				
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[40] );
#else
			SetWaveform_0();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### F6 ###\n");
#endif				
		}								
		else /* PB < [] */
		{
#if KSF_DBG_SET_DUTY
			selectPrintDutyTable( fus_param[41] );
#else
			SetWaveform_0();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### F7 ###\n");
#endif				
		}
		
		if(Temp_CurToPrint > 2000)
		{
			fUnderTempPrint = CLEAR;
			SetWaveform_0();
		}
	}
		
	if((Temp_CurToPrint < -3000) && (fUnderTempPrint == SET))
	{
#if KSF_DBG_PST_PRT		
		DBG1("### Print temp too low!!! <New_AvgTemp=%d, Temp_CurToPrint=%d>###\n",New_AvgTemp,Temp_CurToPrint);
#endif
		return 1;
	}
	return 0;
}
#else // Kara/-S, Pinetree/-S
Uint8 FuserControlForPrint(void) /* [KSF050201] - start *//* [KSF050702] */
{
	int Temp_CurToPrint;
	
	Temp_CurToPrint = New_AvgTemp - (Fuser_Control.Target_Temperature*100);
	cKeepStandbyCnt = 0;
	StandbyCase = 1;

	if((Temp_CurToPrint > fuser_cntl_set.Tt_NB) && (Fuser_Control.State == ST_PRINT) && (fUnderTempPrint == CLEAR))
	{
#if KSF_DBG_THREE
		DBG1("[ControlForPrint] Temp_CurToPrint = %d, Temp_Difference = %d, cUnderTempPrintCount = %d, fUnderTempPrint = %d\n",Temp_CurToPrint,Temp_Difference,cUnderTempPrintCount,fUnderTempPrint);		
#endif
		fUnderTempPrint = SET;
	}
	
	if(Temp_CurToPrint < fuser_cntl_set.Tt_NB)
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB)/* [KF042501] */		
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[0] );
#else						
						SetWaveform_50(); /* new heater */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### G1 ###:1\n");				
#endif												
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### G1 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */						
#if KSF_DBG_PST					
						DBG1("### G1 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120) /* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */
#if KSF_DBG_PST					
					DBG1("### G1 ###:4\n");
#endif										
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[0] );
#else					
					SetWaveform_50();
#endif
					
#if KSF_DBG_PST					
					DBG1("### G1 ###:5\n");
#endif					
				}
			}
			else
			{

#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[0] );
#else
				SetWaveform_100();
#endif				
								
#if KSF_DBG_PST_PRT
				DBG1("### A1 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))/* [KF042501] */		
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[1] );
#else						
						SetWaveform_50();
#endif

#if KSF_DBG_PST					
						DBG1("### G2 ###:1\n");				
#endif												
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### G2 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */						
#if KSF_DBG_PST					
						DBG1("### G2 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120) /* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */
#if KSF_DBG_PST					
					DBG1("### G2 ###:4\n");
#endif										
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[1] );
#else				
					SetWaveform_50();
#endif

#if KSF_DBG_PST					
					DBG1("### G2 ###:5\n");
#endif					
				}
			}
			else
			{

#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[1] );
#else
				SetWaveform_100();
#endif				
								
#if KSF_DBG_PST_PRT
				DBG1("### A2 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))/* [KF042501] */		
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[2] );
#else						
						SetWaveform_50();
#endif			
		
#if KSF_DBG_PST					
						DBG1("### G3 ###:1\n");				
#endif												
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### G3 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */						
#if KSF_DBG_PST					
						DBG1("### G3 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120) /* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */
#if KSF_DBG_PST					
					DBG1("### G3 ###:4\n");
#endif										
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[2] );
#else					
					SetWaveform_50();
#endif

#if KSF_DBG_PST					
					DBG1("### G3 ###:5\n");
#endif					
				}
			}
			else
			{

#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[2] );
#else
				SetWaveform_100();
#endif				
								
#if KSF_DBG_PST_PRT
				DBG1("### A3 ###\n");
#endif				
			}
		}				
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))/* [KF042501] */		
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[3] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### G4 ###:1\n");				
#endif												
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### G4 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */						
#if KSF_DBG_PST					
						DBG1("### G4 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120) /* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */
#if KSF_DBG_PST					
					DBG1("### G4 ###:4\n");
#endif										
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[3] );
#else					
					SetWaveform_50();
#endif

#if KSF_DBG_PST					
					DBG1("### G4 ###:5\n");
#endif					
				}
			}
			else
			{

#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[3] );
#else
				SetWaveform_100();
#endif				
								
#if KSF_DBG_PST_PRT
				DBG1("### A4 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))/* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[4] );
#else						
						SetWaveform_50();
#endif

#if KSF_DBG_PST					
						DBG1("### G5 ###:1\n");				
#endif	
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### G5 ###:2\n");
#endif						
					}					
					else
					{						
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### G5 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### G5 ###:4\n");
#endif						
				}				
				else
				{				
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[4] );
#else
					SetWaveform_50();
#endif

#if KSF_DBG_PST
					DBG1("### G5 ###:5\n");
#endif					
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[4] );				
#else
				SetWaveform_100();
#endif				
				
#if KSF_DBG_PST_PRT
				DBG1("### A5 ###\n");
#endif				
			}			
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))/* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[5] );
#else						
						SetWaveform_50();
#endif

#if KSF_DBG_PST					
						DBG1("### G6 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### G6 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### G6 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### G6 ###:4\n");
#endif						
				}		
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[5] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### G6 ###:5\n");
#endif
				}
			}			
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[5] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT
				DBG1("### A6 ###\n");
#endif
			}			
		}
		else /* PB < [] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[6] );
#else						
						SetWaveform_50();
#endif			
			
#if KSF_DBG_PST					
						DBG1("### G7 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### G7 ###:2\n");
#endif						
					}					
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### G7 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### G7 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[6] );
#else					
					SetWaveform_50();
#endif

#if KSF_DBG_PST
					DBG1("### G7 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[6] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT
				DBG1("### A7 ###\n");
#endif
			}			
		}			
	}/* NB - END */
	//Print Temp - Current Temp : NM
	else if((Temp_CurToPrint >= fuser_cntl_set.Tt_NB) && (Temp_CurToPrint < fuser_cntl_set.Tt_NM))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB)/* [KF042501] */		
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[7] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### H1 ###:1\n");				
#endif												
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### H1 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */						
#if KSF_DBG_PST					
						DBG1("### H1 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120) /* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */
#if KSF_DBG_PST					
					DBG1("### H1 ###:4\n");
#endif										
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[7] );
#else					
					SetWaveform_50();
#endif
					
#if KSF_DBG_PST					
					DBG1("### H1 ###:5\n");
#endif					
				}
			}
			else
			{

#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[7] );
#else
				SetWaveform_100();
#endif				
								
#if KSF_DBG_PST_PRT
				DBG1("### B1 ###\n");
#endif				
			}
		}		
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))/* [KF042501] */		
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[8] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### H2 ###:1\n");				
#endif							
					}	
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### H2 ###:2\n");
#endif						
					}
					else
					{						
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### H2 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### H2 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[8] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### H2 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[8] );
#else
				SetWaveform_100();
#endif

#if KSF_DBG_PST_PRT				
				DBG1("### B2 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))/* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[9] );
#else						
						SetWaveform_50();					
#endif

#if KSF_DBG_PST					
						DBG1("### H3 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### H3 ###:2\n");
#endif						
					}
					else
					{										
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### H3 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### H3 ###:4\n");
#endif						
				}		
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[9] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### H3 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[9] );
#else
				SetWaveform_100();
#endif		
#if KSF_DBG_PST_PRT				
				DBG1("### B3 ###\n");
#endif				
			}			
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))/* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[10] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### H4 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### H4 ###:2\n");
#endif						
					}
					else
					{										
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### H4 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### H4 ###:4\n");
#endif						
				}		
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[10] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### H4 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[10] );
#else
				SetWaveform_100();
#endif		
#if KSF_DBG_PST_PRT				
				DBG1("### B4 ###\n");
#endif				
			}			
		}				
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM)) /* [KF041201] *//* [KF042501] *//* [KF042901] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[11] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### H5 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### H5 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### H5 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### H5 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[11] );
#else					
					SetWaveform_50();
#endif

#if KSF_DBG_PST
					DBG1("### H5 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[11] );
#else
				SetWaveform_100();
#endif
				
#if KSF_DBG_PST_PRT				
				DBG1("### B5 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB)) /* [KF041201] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[12] );
#else
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### H6 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### H6 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### H6 ###:3\n");					
#endif
					}
				}	
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### H6 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[12] );
#else
					SetWaveform_50();
#endif		
		
#if KSF_DBG_PST
					DBG1("### H6 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[12] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### B6 ###\n");
#endif				
			}
		}
		else /* PB < [] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[13] );
#else						
						SetWaveform_50();
#endif			
						
#if KSF_DBG_PST					
						DBG1("### H7 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### H7 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### H7 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### H7 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[13] );
#else
					SetWaveform_50();
#endif

#if KSF_DBG_PST
					DBG1("### H7 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[13] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### B7 ###\n");
#endif				
			}
		}
	}
	//Print Temp - Current Temp : NS
	else if((Temp_CurToPrint >= fuser_cntl_set.Tt_NM) && (Temp_CurToPrint < fuser_cntl_set.Tt_NS))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB) /* [KF042501] */		
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[14] );
#else						
						SetWaveform_50();							
#endif				
				
#if KSF_DBG_PST					
						DBG1("### I1 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### I1 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### I1 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### I1 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[14] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### I1 ###:5\n");
#endif
				}
			}			
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[14] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### C1 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM)) /* [KF041201] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[15] );
#else
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### I2 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### I2 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### I2 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### I2 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[15] );
#else									
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### I2 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[15] );
#else
				SetWaveform_100();
#endif				
#if KSF_DBG_PST_PRT				
				DBG1("### C2 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS)) /* [KF041201] *//* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[16] );
#else
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### I3 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### I3 ###:2\n");
#endif						
					}
					else
					{	
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### I3 ###:3\n");					
#endif
					}
				}				
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### I3 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[16] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### I3 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[16] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### C3 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS)) /* [KF041201] *//* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[17] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### I4 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### I4 ###:2\n");
#endif						
					}
					else
					{	
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### I4 ###:3\n");					
#endif
					}
				}				
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### I4 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[17] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### I4 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[17] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### C4 ###\n");
#endif				
			}
		}		
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[18] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### I5 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### I5 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### I5 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### I5 ###:4\n");
#endif						
				}			
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[18] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### I5 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[18] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### C5 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[19] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### I6 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### I6 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### I6 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### I6 ###:4\n");
#endif						
				}							
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[19] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### I6 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[19] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### C6 ###\n");
#endif				
			}
		}
		else /* PB < [] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[20] );
#else						
						SetWaveform_50();
#endif						
						
#if KSF_DBG_PST					
						DBG1("### I7 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### I7 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### I7 ###:3\n");					
#endif
					}
				}			
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### I7 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[20] );
#else					
					SetWaveform_50();
#endif					
					
#if KSF_DBG_PST
					DBG1("### I7 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[20] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### C7 ###\n");
#endif				
			}
		}
	}
	//Print Temp - Current Temp : ZR
	else if((Temp_CurToPrint >= fuser_cntl_set.Tt_NS) && (Temp_CurToPrint <= fuser_cntl_set.Tt_PS))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB)
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[21] );
#else						
						SetWaveform_50(); /* [KF042501] */
#endif						
											
#if KSF_DBG_PST					
						DBG1("### J1 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### J1 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### J1 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### J1 ###:4\n");
#endif						
				}								
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[21] );
#else					
					SetWaveform_50(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### J1 ###:5\n");
#endif
				}
			}			
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[21] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### D1 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[22] );
#else						
						SetWaveform_50(); /* [KF042501] */
#endif						
										
#if KSF_DBG_PST					
						DBG1("### J2 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### J2 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### J2 ###:3\n");					
#endif
					}
				}		
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### J2 ###:4\n");
#endif						
				}							
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[22] );
#else					
					SetWaveform_50(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### J2 ###:5\n");
#endif
				}
			}		
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[22] );
#else
				SetWaveform_100();
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D2 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[23] );
#else						
						SetWaveform_33(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### J3 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### J3 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### J3 ###:3\n");					
#endif
					}
				}		
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### J3 ###:4\n");
#endif						
				}							
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[23] );
#else					
					SetWaveform_33(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### J3 ###:5\n");
#endif
				}
			}		
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[23] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_80();
#endif
#endif
#if KSF_DBG_PST_PRT				
			DBG1("### D3 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[24] );
#else						
						SetWaveform_33(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### J4 ###:1\n");				
#endif							
					}	
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### J4 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### J4 ###:3\n");					
#endif
					}
				}	
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### J4 ###:4\n");
#endif						
				}								
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[24] );
#else					
					SetWaveform_33(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### J4 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[24] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_80();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### D4 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))/* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[25] );
#else						
						SetWaveform_33(); /* [KF042501] */
#endif						
										
#if KSF_DBG_PST					
						DBG1("### J5 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### J5 ###:2\n");
#endif						
					}	
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### J5 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### J5 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[25] );
#else					
					SetWaveform_33(); /* [KF042501] */
#endif

#if KSF_DBG_PST
					DBG1("### J5 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[25] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_80();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### D5 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))/* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[26] );
#else						
						SetWaveform_10(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### J6 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### J6 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### J6 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### J6 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[26] );
#else
					SetWaveform_10(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### J6 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[26] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_33();
#endif
#endif				
#if KSF_DBG_PST_PRT				
			DBG1("### D6 ###\n");
#endif				
			}
		}
		else /* PB < [] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[27] );
#else						
						SetWaveform_0(); /* [KF042501] */					
#endif						
						
#if KSF_DBG_PST					
						DBG1("### J7 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### J7 ###:2\n");
#endif						
					}	
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### J7 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### J7 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[27] );
#else					
					SetWaveform_0(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### J7 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[27] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_25();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### D7 ###\n");
#endif				
			}
		}
	}
	//Print Temp - Current Temp : PS
	else if((Temp_CurToPrint > fuser_cntl_set.Tt_PS) && (Temp_CurToPrint <= fuser_cntl_set.Tt_PM))
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB) /* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[28] );
#else						
						SetWaveform_33(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### K1 ###:1\n");				
#endif							
					}	
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### K1 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### K1 ###:3\n");					
#endif
					}
				}	
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### K1 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[28] );
#else								
					SetWaveform_33(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### K1 ###:5\n");
#endif	
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[28] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_80();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### E1 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM)) /* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[29] );
#else						
						SetWaveform_33(); /* [KF042501] */
#endif						
										
#if KSF_DBG_PST					
						DBG1("### K2 ###:1\n");				
#endif							
					}	
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### K2 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### K2 ###:3\n");					
#endif
					}
				}	
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### K2 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[29] );
#else					
					SetWaveform_33(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### K2 ###:5\n");
#endif	
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[29] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_80();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### E2 ###:6\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))/* KF041201 *//* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[30] );
#else						
						SetWaveform_25(); /* [KF042501] */
#endif

#if KSF_DBG_PST					
						DBG1("### K3 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### K3 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### K3 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### K3 ###:4\n");
#endif						
				}			
				else
				{				
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[30] );
#else					
					SetWaveform_25(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### K3 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[30] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_50();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### E3 ###\n");
#endif				
			}
		}		
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS)) /* [KF042501] *//* [KF042901] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[31] );
#else
						SetWaveform_10(); /* [KF042501] */					
#endif

#if KSF_DBG_PST					
						DBG1("### K4 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### K4 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### K4 ###:3\n");					
#endif
					}
				}			
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### K4 ###:4\n");
#endif						
				}					
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[31] );
#else					
					SetWaveform_10(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### K4 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[31] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_33();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### E4 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM)) /* [KF042501] *//* [KF042901] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[32] );
#else						
						SetWaveform_10(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### K5 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### K5 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### K5 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### K5 ###:4\n");
#endif						
				}								
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[32] );
#else					
					SetWaveform_10(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### K5 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[32] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_33();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### E5 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[33] );
#else						
						SetWaveform_0(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### K6 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### K6 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### K6 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### K6 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[33] );
#else					
					SetWaveform_0(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### K6 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[33] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_25();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### E6 ###\n");
#endif				
			}
		}
		else /* PB < [] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[34] );
#else						
						SetWaveform_0(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### K7 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### K7 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### K7 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### K7 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[34] );
#else					
					SetWaveform_0(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### K7 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[34] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_10();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### E7 ###\n");
#endif				
			}
		}
	}
	//Print Temp - Current Temp : PM
	else
	{
		if(Temp_Difference < fuser_cntl_set.dTt_NB) /* [KF042501] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[35] );
#else						
						SetWaveform_10(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### L1 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### L1 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### L1 ###:3\n");					
#endif
					}
				}				
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### L1 ###:4\n");
#endif						
				}				
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[35] );
#else					
					SetWaveform_10(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### L1 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[35] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_33();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### F1 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NB) && (Temp_Difference < fuser_cntl_set.dTt_NM))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{			
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[36] );
#else
						SetWaveform_10(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### L2 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### L2 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST	
						DBG1("### L2 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### L2 ###:4\n");
#endif						
				}						
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[36] );
#else					
					SetWaveform_10(); /* [KF042501] */
#endif			
					
#if KSF_DBG_PST
					DBG1("### L2 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[36] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_33();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### F2 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NM) && (Temp_Difference < fuser_cntl_set.dTt_NS))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[37] );
#else
						SetWaveform_10(); /* [KF042501] */					
#endif						
						
#if KSF_DBG_PST					
						DBG1("### L3 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### L3 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### L3 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### L3 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[37] );
#else					
					SetWaveform_10(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### L3 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[37] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_33();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### F3 ###\n");
#endif				
			}
		}
		else if((Temp_Difference >= fuser_cntl_set.dTt_NS) && (Temp_Difference <= fuser_cntl_set.dTt_PS))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[38] );
#else						
						SetWaveform_0(); /* [KF042501] */
#endif						
										
#if KSF_DBG_PST					
						DBG1("### L4 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### L4 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### L4 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### L4 ###:4\n");
#endif						
				}
				else
				{	
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[38] );
#else					
					SetWaveform_0(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### L4 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[38] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_25();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### F4 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PS) && (Temp_Difference <= fuser_cntl_set.dTt_PM))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[39] );
#else						
						SetWaveform_0(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### L5 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### L5 ###:2\n");
#endif						
					}
					else
					{
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### L5 ###:3\n");					
#endif
					}
				}
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### L5 ###:4\n");
#endif						
				}
				else
				{	
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[39] );
#else
					SetWaveform_0(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### L5 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[39] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_25();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### F5 ###\n");
#endif				
			}
		}
		else if((Temp_Difference > fuser_cntl_set.dTt_PM) && (Temp_Difference <= fuser_cntl_set.dTt_PB))
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[40] );
#else						
						SetWaveform_0(); /* [KF042501] */					
#endif						
						
#if KSF_DBG_PST					
						DBG1("### L6 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### L6 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### L6 ###:3\n");					
#endif
					}
				}		
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### L6 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[40] );
#else					
					SetWaveform_0(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### L6 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[40] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_10();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### F6 ###\n");
#endif				
			}
		}								
		else /* PB < [] */
		{
#if ZX_ENABLE
			if(0)
#else
			if((Fuser_Control.State == ST_WARMUP)||(Fuser_Control.State == ST_HEATING))
#endif
			{
				if( Fuser_Control.fMakeMotorRun == 1 )/* [KF053101] */
				{
					if((New_AvgTemp/100) >= (Adc_Print_Low - 5)) /* [KF060506] */
					{
#if KSF_DBG_SET_DUTY_WH
						selectPrintDutyTable( fus_paramWh[41] );
#else						
						SetWaveform_0(); /* [KF042501] */
#endif						
						
#if KSF_DBG_PST					
						DBG1("### L7 ###:1\n");				
#endif							
					}
					else if((New_AvgTemp/100) < 120)/* [KF060501] */
					{
						SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
						DBG1("### L7 ###:2\n");
#endif						
					}
					else
					{					
						SetWaveform_100(); /* [KF060302] *//* [KF060506] */
#if KSF_DBG_PST
						DBG1("### L7 ###:3\n");					
#endif
					}
				}		
				else if((New_AvgTemp/100) < 120)/* [KF060501] */
				{
					SetWaveform_100(); /* < 100[do] */							
#if KSF_DBG_PST					
					DBG1("### L7 ###:4\n");
#endif						
				}
				else
				{
#if KSF_DBG_SET_DUTY_WH
					selectPrintDutyTable( fus_paramWh[41] );
#else					
					SetWaveform_0(); /* [KF042501] */
#endif					
					
#if KSF_DBG_PST
					DBG1("### L7 ###:5\n");
#endif
				}
			}
			else
			{
#if KSF_DBG_SET_DUTY
				selectPrintDutyTable( fus_param[41] );
#else
#if ZX_ENABLE
				SetWaveform_0();
#else
				SetWaveform_10();
#endif
#endif
#if KSF_DBG_PST_PRT				
				DBG1("### F7 ###\n");
#endif				
			}
		}
		
		if(Temp_CurToPrint > 2000)
		{
			fUnderTempPrint = CLEAR;
			SetWaveform_0();
		}
	}
		
	if(((Temp_CurToPrint < -3000) && (fUnderTempPrint == SET)) && ((Fuser_Control.State != ST_WARMUP) || (Fuser_Control.State != ST_HEATING)))
	{
#if KSF_DBG_PST_PRT		
		DBG1("### Print temp too low!!! <New_AvgTemp=%d, Temp_CurToPrint=%d>###\n",New_AvgTemp,Temp_CurToPrint);
#endif
		return 1;
	}
	return 0;
}/* [KSF050301] - end */
#endif

void Fusermanager(void) /* FuserState_Manager */
{
	Uint16 nFuser_Counter;
	nFuser_Counter = 0;
#if EP_PARAM_TEST
	Uint8 temp_var;
	if(fParamMotorVALUE == SET)
		temp_var = Param_Motor_VALUE;
	else
		temp_var = 3;
#endif
	switch (Fuser_Control.State) 
	{
		case ST_ERROR:
			DownToOff(); /* [KSF050201] - start */
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
			printf("ST_ERROR:%d (%u)\n",nFuser_Counter, posix_gettime_ms());
#endif /* [KSF050201] - end */
			if(Fuser_Control.RequestState == CMD_FUSER_OFF)
			{				
				init_error_flag() ; /* [KSF050201] */
			}
			if(cFanStopInError++ > COUNT_3SEC)
			{
				MainFan_Off();
				SubFan_Off();
				DevFan_Off();
			}
			break ;
		case ST_WAIT_RUN:
		case ST_OFF :
			if(Fuser_Control.RequestState == CMD_FUSER_STANDBY) /* [KSF050201] */
			{
				Fuser_Control.State = ST_WARMUP;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )				
				printf("ST_OFF : changed ST_WARMUP:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif
				f_firstStandby = SET; /* [KSF071701] */
				FuserControlCounter = 0;
				FuserControlForStandby();

				fPrintToOff = 0; /* [KSF050302] */			
				fMainFanDutyUpForStandby = FALSE;
				
				if(fMainfanForPOR == TRUE)
				{
					cMainFanCounterForPOR = 0;
					NdelayFan = COUNT_3SEC;
				}	
			}
			else if (Fuser_Control.RequestState == CMD_FUSER_COLD_WAKE)
			{
				Fuser_Control.State = ST_COLD_WAKEUP;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_OFF : changed ST_COLD_WAKEUP:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] - start */
#endif
				if(New_AvgTemp/100 <= fuser_cntl_set.coldWakeHoldTemp)
				{
					ColdWakeupCase = 1;
				}
				else
				{
					ColdWakeupCase = 2;
				} /* [KSF050201] - end */
				FuserControlCounter = 0;
				cColdWakeupHoldingCnt = 0; /* [KSF050201] - start */
				fColdWakeupHoldingStart = FALSE;
				FuserControlForColdWakeUp(); /* [KSF050201] - end */
			
#if FAN_TEST
				MainFan_DutyChange(MainFanMid);/* Fan_High */
				SubFan_DutyChange(SubFanMax);
				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(13);/* Fan_High */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
				
				fPrintToOff = 0; /* [KSF050302] */
			}
			else if (Fuser_Control.RequestState == CMD_FUSER_PRINT) /* [KSF050201] - start */
			{ /* [KSF050201] - end */
				Fuser_Control.State = ST_HEATING;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )					
				printf("ST_OFF : changed ST_HEATING1:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif				
				FuserControlCounter = 0;
#if FAN_TEST
				MainFan_DutyChange(MainFanMid);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(SubFanMax);
				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
				
				fPrintToOff = 0; /* [KSF050302] */
			}
			else /* [KSF050201] - start */
			{ // keep current state
#if KSF_DBG_THREE
				printf("ST_OFF : keep current state:%d\n",nFuser_Counter);			
#endif
				cKeepStandbyCnt = 0;
				StandbyCase = 1;
				WarmupCase_POR = 1;
				DownToOff(); /* [KSF050201] - end */
				
#if FAN_TEST
				if(FanCtrlEnvIdx == FAN_CTRL_H)
				{
					if( FuserControlCounter > COUNT_60SEC)
					{
						if(FuserControlCounter > COUNT_600SEC)
						{
							MainFan_Off();
							SubFan_Off();
							DevFan_Off();
						}
						else
						{
							if(fMainFanDutyUpForStandby == TRUE)
							{
								MainFan_DutyChange(MainFanLow);
							}
							else
							{
								MainFan_DutyChange(MainFanMin);
							}
							SubFan_DutyChange(SubFanLow);
							DevFan_DutyChange(SubFanLow);
						}
					}
				}
				else
				{
					MainFan_Off();
					SubFan_Off();
					DevFan_Off();
				}
#else						
				MainFan_DutyChange(5);
				SubFan_DutyChange(99); /* Sub_Fan_Low */				
				DevFan_DutyChange(99); /* Sub_Fan_Low */				
				if(fPrintToOff == 1) /* [KSF050302] - start */
				{
#if KSF_DBG_THREE
				printf("(1)=> FuserControlCounter:%d\n",FuserControlCounter);			
#endif					
					if(FuserControlCounter > COUNT_3SEC)/* 3[sec] */
					{
						MainFan_DutyChange(0);
						SubFan_DutyChange(0);
						DevFan_DutyChange(0);
					}
				}
				else
				{
#if KSF_DBG_THREE
				printf("(2)=> FuserControlCounter:%d\n",FuserControlCounter);			
#endif					
					if((FuserControlCounter > COUNT_180SEC)/* 180[sec] */ || (Fuser_Control.fCoverOpen == TRUE))
					{
						MainFan_DutyChange(0);
						SubFan_DutyChange(0);
						DevFan_DutyChange(0);
					}
				}/* [KSF050302] - end */
#endif
			}			
			break;
			
		case ST_COLD_WAKEUP:
			OndoNo = 84; /* [KSF051301] */		
			if(Fuser_Control.RequestState == CMD_FUSER_OFF) 
			{ /* [KSF050201] - end */
				Fuser_Control.State = ST_OFF ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_COLD_WAKEUP : changed ST_OFF:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0 ;
				DownToOff(); /* [KSF050201] */
			}
			else if (Fuser_Control.RequestState == CMD_FUSER_POWERSAVE) /* [KSF050201] */
			{
				Fuser_Control.State = ST_POWER_SAVE ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_COLD_WAKEUP : changed ST_POWER_SAVE:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0 ;
				DownToPowerSave();
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_STANDBY) /* [KSF050201] */
			{
				Fuser_Control.State = ST_WARMUP;
#if ( KSF_DBG_THREE	| KSF_DBG_TIMESTAMP )
				printf("ST_COLD_WAKEUP : changed ST_WARMUP:%d (%u)\n",nFuser_Counter, posix_gettime_ms());/* [KSF050201] */
#endif
				f_firstStandby = SET; /* [KSF070801] */
				FuserControlCounter = 0;
				FuserControlForStandby();
				
				fMainFanDutyUpForStandby = FALSE;
#if FAN_TEST				
				//MainFan_DutyChange(50); /* Fan_High */
				//SubFan_DutyChange(99);	
				//DevFan_DutyChange(99);	
#else
				MainFan_DutyChange(50); /* Fan_High */
				SubFan_DutyChange(99);	
				DevFan_DutyChange(99);	
#endif
			}
			else if (Fuser_Control.RequestState == CMD_FUSER_PRINT) /* [KSF050201] */
			{
				Fuser_Control.State = ST_HEATING;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_COLD_WAKEUP : changed ST_HEATING1:%d (%u)\n",nFuser_Counter, posix_gettime_ms());/* [KSF050201] */
#endif				
				FuserControlCounter = 0;
			
#if FAN_TEST
				MainFan_DutyChange(MainFanMid);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(SubFanMax);
				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
			}
			else /* [KSF050201] - start */
			{ // keep current state
				FuserControlForColdWakeUp(); /* [KSF050201] - end */
/*				
				if( FuserControlCounter > NdelayFan)
				{
					MainFan_DutyChange(2);
					if(Fuser_Control.fLowTemperature == 1)
					{
						SubFan_DutyChange(0);						
						DevFan_DutyChange(0);						
					}
					else
					{
						SubFan_DutyChange(4);
						DevFan_DutyChange(4);
					}
				}				
*/
			}
			break;
			
		case ST_WARMUP :
			OndoNo = 84; /* [KSF051301] */	
			if(Fuser_Control.RequestState == CMD_FUSER_OFF) 
			{ /* WARMUP -> OFF *//* [KSF050201] - end */
				Fuser_Control.State = ST_OFF ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_WARMUP : changed ST_OFF:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0 ;
				//FuserOff();
				DownToOff(); /* [KSF050201] */
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_PRINT) /* [KSF050201] */
			{ /* WARMUP -> HEATING (only FAN MAX) */
				Fuser_Control.State = ST_HEATING ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_WARMUP : changed ST_HEATING2:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif									
				FuserControlCounter = 0 ;				
#if FAN_TEST
				MainFan_DutyChange(MainFanMid);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(SubFanMax);
				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_POWERSAVE) /* [KSF050201] */
			{ /* WARMUP -> POWER SAVE */
				Fuser_Control.State = ST_POWER_SAVE ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_WARMUP : changed ST_POWER_SAVE:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0 ;
				// Must add ....
				DownToPowerSave();	
			}
#if ABBA2
			//else if(((New_AvgTemp/100) >= Adc_Standby ) && (fEnterStandby == TRUE))
			else if(((New_AvgTemp/100) >= Adc_Standby ))
#else
			else if(((New_AvgTemp/100) >= ( Adc_Standby - 10 ) ) && (fEnterStandby == TRUE) ) /* [fEnterStandby check need] *//* [KSF052001] */	
#endif
			{/* [KSF050302] */
				fEnterStandby = CLEAR; /* [KSF050201] - end */
				Fuser_Control.State = ST_STANDBY ;
				if((fMainfanForPOR == TRUE) && (Fuser_Control.fPOR_PageSupervisor_Active == FALSE))
				{
					FuserControlCounter = cMainFanCounterForPOR;
				}
				else
				{
					MainFan_DutyChange(MainFanMid);
					FuserControlCounter = 0 ;
				}
				FuserControlForKeepStandby(); /* [KSF050201] */
#if ( KSF_DBG_THREE	| KSF_DBG_TIMESTAMP )
				printf("ST_WARMUP : changed ST_STANDBY:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif								
				NdelayFan = COUNT_3SEC;
				f_firstStandby= CLEAR ; /* [KSF050201] */
				//Fuser_Control.NarrowMedeaCount = 0;
			}
			else 
			{
				if((fMainfanForPOR == TRUE) && (Fuser_Control.fPOR_PageSupervisor_Active == FALSE))
				{
					++cMainFanCounterForPOR;
					MainFan_DutyChange(MainFanMid);
					if(cMainFanCounterForPOR > NdelayFan)
					{
						MainFan_DutyChange(MainFanMin);
					}
				}
#if OVER_HEAT_DBG						
				printf("ST_WARMUP : else:%d (%d) [%d] (%d) fs:%d cc:%d\n",nFuser_Counter, Fuser_Control.fMakeMotorRun, (New_AvgTemp/100),(Fuser_Control.fPOR_PageSupervisor_Active),f_firstStandby, FuserControlCounter);				
#endif
#if ABBA2 // Abba2
				FuserControlForStandby();
#else // Kara/-S, Pinetree/-S
				if((((New_AvgTemp/100) > 125)&&((New_AvgTemp/100) < 145))&& (Fuser_Control.fPOR_PageSupervisor_Active == 1) && (Fuser_Control.fMakeMotorRun == 0)) /* POR only & Motor stop & 175 > temp */
				{
					FuserControlForPorBeforeMotorRun();
				}
				else
				{
					FuserControlForStandby();
					if( (New_AvgTemp/100) >= ( Adc_Standby - 10 ) ) /* [KF052001] */
					{
						fEnterStandby = CLEAR;
						Fuser_Control.State = ST_STANDBY ;
						if((fMainfanForPOR == TRUE) && (Fuser_Control.fPOR_PageSupervisor_Active == CLEAR))
						{
							FuserControlCounter = cMainFanCounterForPOR;
						}
						else
						{
							MainFan_DutyChange(MainFanMid);
							FuserControlCounter = 0 ;
						}
						FuserControlForKeepStandby();
#if OVER_HEAT_DBG
						printf("ST_WARMUP : changed ST_STANDBY:%d New_AvgTemp:%d Adc_Standby:%d\n",nFuser_Counter,New_AvgTemp, Adc_Standby);
#endif								
						NdelayFan = COUNT_3SEC;
						f_firstStandby= CLEAR ;
						Fuser_Control.NarrowMediaCount = 0 ;
					}					
				}
#endif
			}
			break;
			
		case ST_STANDBY :			
			if(Fuser_Control.RequestState == CMD_FUSER_PRINT) 
			{ /* STANDBY -> HEATING */ /* [KSF050201] - end */		
				Fuser_Control.State = ST_HEATING ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_STANDBY : changed ST_HEATING3:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif				
				FuserControlCounter = 0;
				cUnderTempStandbyCount = 0; /* [KSF051001] */				
#if FAN_TEST
				MainFan_DutyChange(MainFanMid);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(SubFanMax);
				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
				
				//cStandbyTimeoutCount = 0; /* standby timeout count init *//* [KSF050201] *//* [KSF050302] */
			} 
			else if (Fuser_Control.RequestState == CMD_FUSER_OFF) /* [KSF050201] */
			{
				Fuser_Control.State = ST_OFF ;
				FuserControlCounter = 0 ;
				cUnderTempStandbyCount=0; /* [KSF051001] */
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_STANDBY : changed ST_OFF:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				//FuserOff(); /* [KSF050201] */
				DownToOff();
			}
			else if (Fuser_Control.RequestState == CMD_FUSER_POWERSAVE) /* [KSF050201] */
			{
				Fuser_Control.State = ST_POWER_SAVE ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_STANDBY : changed ST_POWER_SAVE:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				if(FanCtrlEnvIdx == FAN_CTRL_H)
				{
					fFanCtrl24vOff = SET;
					cUnderTempStandbyCount = 0;
					DownToOff();
				}
				else
				{
					FuserControlCounter = 0;
					cUnderTempStandbyCount = 0; /* [KSF051001] */
					DownToPowerSave();		//	MainFan_DutyChange(50);
				}
			}
			else 
			{ // keep current state
				fMainfanForPOR = FALSE;
				fFanCtrl24vOff = CLEAR;
#if KSF_DBG_TWO			
				printf("ST_STANDBY : keep current state:%d (%u)\n",nFuser_Counter, posix_gettime_ms());
#endif			
				if(FuserControlForKeepStandby()==1)
				{
					cUnderTempStandbyCount++;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
					printf("[EC 301] cUnderTempStandbyCount:%d (%u)\n",cUnderTempStandbyCount, posix_gettime_ms());
#endif
					if(cUnderTempStandbyCount>=UNDERTEMPERATURE_TIMEOUT)
					{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR
						// ERROR 301	: UNDER TEMPERATURE WHILE STANDBY
						Fuser_Control.FuserFaultNumber = ERROR_UNDER_TEMPERATUREP_STANDBY ;
						Fuser_Control.fFuserFault = TRUE;
						Fuser_Control.State = ST_ERROR;
						error_display();
#endif	// IGNORE_HEATER_ERROR
#endif
					}
				}
				else
				{
					cUnderTempStandbyCount = 0;
				} /* [KSF050201] - end */
				
				/* [KSF050302] - start */
				
				if(FanCtrlEnvIdx == FAN_CTRL_H)
				{
					if( FuserControlCounter > COUNT_60SEC)
					{
						if(FuserControlCounter > COUNT_600SEC)
						{
							SubFan_Off();
							DevFan_Off();
						}
						else
						{
							if(fMainFanDutyUpForStandby == TRUE)
							{
								MainFan_DutyChange(MainFanLow);
							}
							else
							{
								MainFan_DutyChange(MainFanMin);
							}
							SubFan_DutyChange(SubFanLow);
							DevFan_DutyChange(SubFanLow);
						}
					}
				}
				else
				{
#if FANDELAY_IN_STANDBY
					if( FuserControlCounter > (Fuser_Control.FanStopLongDelayInStandby? COUNT_60SEC:NdelayFan))
#else
					if( FuserControlCounter > NdelayFan)
#endif
					{
						Fuser_Control.FanStopLongDelayInStandby = CLEAR;
#if FAN_TEST
						if(fMainFanDutyUpForStandby == TRUE)
							MainFan_DutyChange(MainFanLow);
						else
							MainFan_DutyChange(MainFanMin);
						//SubFan_DutyChange(FanOff);
						//DevFan_DutyChange(FanOff);
						SubFan_Off();
						DevFan_Off();
#else
						MainFan_DutyChange(2);
						SubFan_DutyChange(5);
						DevFan_DutyChange(5);
						if(Fuser_Control.fLowTemperature == 1)
						{
							SubFan_DutyChange(0); /* Turn off sub fan */
							DevFan_DutyChange(0); /* Turn off sub fan */
						}
						else
						{
							SubFan_DutyChange(4);
							DevFan_DutyChange(4);
						}
#endif
					}
				}
				/* [KSF050302] - end */				
			}			
			break;
			
		case ST_HEATING:		
			OndoNo = 84;
			f_firstStandby= CLEAR; /* [KSF070801] */
			if (Fuser_Control.RequestState == CMD_FUSER_OFF) /* [KSF050201] - end */
			{
				Fuser_Control.State = ST_OFF ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_HEATING : changed ST_OFF:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif								
				FuserControlCounter = 0 ;
				DownToOff(); /* [KSF050201] */
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_STANDBY) /* [KSF050201] */
			{
				Fuser_Control.State = ST_STANDBY ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_HEATING : changed ST_COOL_STANDBY:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0;
				FuserControlForStandby();
				
				fMainFanDutyUpForStandby = FALSE;
				
				NdelayFan = fuser_cntl_set.StandbyFanDelay ;
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_POWERSAVE) /* [KSF050201] - start */
			{
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_HEATING : CMD_FUSER_POWERSAVE:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] - end */
#endif
				Fuser_Control.State = ST_POWER_SAVE ;
				FuserControlCounter = 0 ;
				DownToPowerSave();
			}
#if EP_PARAM_TEST
			else if((New_AvgTemp/100) >= (Adc_Print_Low - temp_var)) /* [KSF050804] motor fast run */
#else
#if ABBA2 // Abba2
			else if((New_AvgTemp/100) >= Adc_Print_Low)
#else // Kara/-S, Pinetree/-S
			else if((New_AvgTemp/100) >= (Adc_Print_Low - 3)) /* [KSF050804] motor fast run */
#endif
#endif
			//else if(ChangeVoltageToTemp() >= 180) /* [KSF050201] */
			{
#if EP_PARAM_TEST				
				DBG1("(param test) TEMP_VAL:%d\n",temp_var);
#endif				
				
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_HEATING : changed ST_PRINT:%d[%d] (%u)\n",New_AvgTemp,Adc_Print_Low, posix_gettime_ms());
#endif				
				Fuser_Control.State = ST_PRINT ;
				f_underprinting = FALSE ;
#if FAN_TEST
//				MainFan_DutyChange(MainFanMid);		// FAN MAX	
//				SubFan_DutyChange(SubFanMax);
//				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50);		// FAN MAX	
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
				FuserControlForPrint();		
				FuserControlCounter = 0 ;
			}
			else
			{
#if KSF_DBG_THREE
				printf("ST_HEATING : else:%d\n",nFuser_Counter); /* [KSF050201] */
#endif
				FuserControlForPrint();
			}
			break;
			
		case ST_PRINT:
			OndoNo = 84; /* [KSF051301] */
			f_firstStandby= CLEAR; /* [KSF070801] */ 
			if (Fuser_Control.RequestState == CMD_FUSER_OFF) 
			{ /* [KSF050201] - end */
				Fuser_Control.State = ST_OFF ;
				fUnderTempPrint = CLEAR;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_PRINT : changed ST_OFF:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif						
				FuserControlCounter = 0;
				cUnderTempPrintCount = 0;  /* [KSF051002] */
				DownToOff(); /* [KSF050201] */
				fPrintToOff = 1; /* [KSF050302] */
			}		
			else if(Fuser_Control.RequestState == CMD_FUSER_STANDBY) /* [KSF050201] */
			{
				Fuser_Control.State = ST_STANDBY ;
				fUnderTempPrint = CLEAR;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_PRINT : changed ST_STANDBY:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif						
				FuserControlCounter = 0;
				cUnderTempPrintCount = 0;  /* [KSF051002] */
				FuserControlForStandby();				
				
				fMainFanDutyUpForStandby = FALSE;
				
				NdelayFan = fuser_cntl_set.StandbyFanDelay ;
				Fuser_Control.NarrowMediaCount = 0;
			}
			else if (Fuser_Control.RequestState == CMD_FUSER_POWERSAVE) /* [KSF050201] */
			{
				Fuser_Control.State = ST_POWER_SAVE ;
				fUnderTempPrint = CLEAR;
#if ( KSF_DBG_THREE	| KSF_DBG_TIMESTAMP )
				printf("ST_PRINT : changed ST_POWER_SAVE:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0;
				cUnderTempPrintCount = 0; /* [KSF051002] */
				DownToPowerSave();				
			}
			else  /* [KSF050201] - start */
			{ // keep current state
#if KSF_DBG_THREE
				printf("ST_PRINT : keep current state:%d\n",nFuser_Counter);
#endif
				if(RunEngineForLamination == SET)
				{
					FuserControlForLaminate();
				}
				else
				{
					if((FuserControlForPrint()==1) && (fUnderTempPrint == SET))
					{
	#if KSF_DBG_THREE
						printf("ST_PRINT : keep current state:%d (cUnderTempPrintCount:%d)\n",nFuser_Counter,cUnderTempPrintCount);
	#endif					
						cUnderTempPrintCount++;
						if(cUnderTempPrintCount>=UNDERTEMPERATURE_TIMEOUT)
						{
	#if ZX_ENABLE
	/* No Error Check */
	#else
	#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
							printf("ERROR 300 !!! (UNDER TEMPERATURE WHILE PRINTING) (%u)\n",posix_gettime_ms());
	#endif
#ifndef IGNORE_HEATER_ERROR	
							// ERROR 300	: UNDER TEMPERATURE WHILE PRINTING
							Fuser_Control.FuserFaultNumber = ERROR_UNDER_TEMPERATUREP_PRINTING ;
							Fuser_Control.fFuserFault = TRUE ;
							Fuser_Control.State = ST_ERROR;
							error_display();
#endif // IGNORE_HEATER_ERROR					
	#endif
						}
					}
					else
					{
						cUnderTempPrintCount = 0;
					} /* [KSF050201] - end */
				}
				
				if(Fuser_Control.fDuplexJob != FALSE)
				{
					MainFan_DutyChange(MainFanHigh);
				}
#if BLI_TEST
				else
				{
					MainFan_DutyChange(MainFanMid);
				}
#endif
			}			
			break;
			
		case ST_POWER_SAVE:
			if(Fuser_Control.RequestState == CMD_FUSER_OFF) 
			{
				Fuser_Control.State = ST_OFF ;
#if ( KSF_DBG_THREE	| KSF_DBG_TIMESTAMP )
				printf("ST_POWER_SAVE : changed ST_OFF:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif						
				FuserControlCounter = 0 ;
				DownToOff(); /* [KSF050201] */
			}
			else if (Fuser_Control.RequestState == CMD_FUSER_COLD_WAKE)
			{
				Fuser_Control.State = ST_COLD_WAKEUP;
				if(New_AvgTemp/100 <= fuser_cntl_set.coldWakeHoldTemp) /* [KSF050201] - start */
				{
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_POWER_SAVE : changed ST_COLD_WAKEUP->Case1:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
					ColdWakeupCase = 1;
				}
				else
				{
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_POWER_SAVE : changed ST_COLD_WAKEUP->Case2:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
					ColdWakeupCase = 2;
				} /* [KSF050201] - end */
				FuserControlCounter = 0;
				cColdWakeupHoldingCnt = 0; /* [KSF050201] - start */
				fColdWakeupHoldingStart = FALSE;
				FuserControlForColdWakeUp(); /* [KSF050201] - end */
#if FAN_TEST
//				MainFan_DutyChange(MainFanMid);/* Fan_High */
//				SubFan_DutyChange(SubFanMax);
//				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50); /* Fan_High */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_STANDBY) /* [KSF050201] - start */
			{
				//if(ChangeVoltageToTemp() >= 149) /* [KSF050201] */
				if(Adc_Standby < (New_AvgTemp/100)) /* [KSF050201] */
				{
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
					printf("ST_POWER_SAVE : changed ST_COOL_STANDBY:%d (%u)\n",New_AvgTemp, posix_gettime_ms());
#endif					
					Fuser_Control.State = ST_STANDBY ;
#if FAN_TEST
					NdelayFan = COUNT_3SEC;
					FuserControlCounter = 0;
					MainFan_DutyChange(MainFanMid);
#else
					MainFan_DutyChange(2); /* Fan_Min */
					SubFan_DutyChange(4); /* Sub_Fan_Low */		
					DevFan_DutyChange(4); /* Sub_Fan_Low */		
#endif
				}
				else if((New_AvgTemp/100) < Adc_Standby) /* [KSF050201] *//* [KSF051601] */
				//else if(ChangeVoltageToTemp() < 149)  
				{
					Fuser_Control.State = ST_WARMUP ;						
#if ( KSF_DBG_THREE	| KSF_DBG_TIMESTAMP )
					printf("ST_POWER_SAVE : changed ST_WARMUP:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif
					f_firstStandby = SET; /* [KSF070801] */
					FuserControlCounter = 0;
					FuserControlForStandby();
#if FAN_TEST
#else
					MainFan_DutyChange(13);
					SubFan_DutyChange(99);
					DevFan_DutyChange(99);
#endif
				}
				else
				{
					Fuser_Control.State = ST_STANDBY ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
					printf("ST_POWER_SAVE : changed ST_STANDBY:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif							
					NdelayFan = COUNT_3SEC;		// Delay Fan Low
#if FAN_TEST	
					MainFan_DutyChange(MainFanMid);
//					SubFan_DutyChange(SubFanLow);
//					DevFan_DutyChange(SubFanLow);
#else
					MainFan_DutyChange(2);
					SubFan_DutyChange(4);
					DevFan_DutyChange(4);
#endif
					Fuser_Control.NarrowMediaCount = 0;
				}
				FuserControlCounter = 0;
				FuserControlForStandby();
				fMainFanDutyUpForStandby = FALSE;
			}		
			else if(Fuser_Control.RequestState == CMD_FUSER_PRINT) 
			{ /* [KSF050201] - end */
				Fuser_Control.State = ST_HEATING ;	
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_POWER_SAVE : changed ST_HEATING4:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif								
				FuserControlCounter = 0 ;
#if FAN_TEST
				MainFan_DutyChange(MainFanMid);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(SubFanMax);
				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
			}
			else
			{
#if KSF_DBG_THREE
				printf("ST_POWER_SAVE : else:%d\n",nFuser_Counter); /* [KSF050201] */
#endif

#if FAN_TEST
				if(FanCtrlEnvIdx == FAN_CTRL_H)
				{
					if( FuserControlCounter > COUNT_60SEC)
					{
						if(FuserControlCounter > COUNT_600SEC)
						{
							SubFan_Off();
							DevFan_Off();
							MainFan_Off();
							
							if(fFanCtrl24vOff == SET)
							{
								FanCtrlEnvIdx = FAN_CTRL_L;
								fFanCtrl24vOff = CLEAR;
								PS_24vOff(SET);
								f24vOnForJogMode = FALSE;
								usleep(100*1000);
								gpio_write( eng_hardware->mainmotor_start, 0 ); // BLDC_ST for PWS (High -> Low)
								gpio_write( eng_hardware->mainmotor_dir, 0 );	// BLDC_DIR for PWS (High -> Low)
							}
						}
						else
						{
							if(fMainFanDutyUpForStandby == TRUE)
							{
								MainFan_DutyChange(MainFanLow);
							}
							else
							{
								MainFan_DutyChange(MainFanMin);
							}
							SubFan_DutyChange(SubFanLow);
							DevFan_DutyChange(SubFanLow);
						}
					}
				}
				else
				{
					MainFan_Off();
					SubFan_Off();
					DevFan_Off();
				}
#else
				if((FuserControlCounter >= fuser_cntl_set.PwsFanRun) || (Fuser_Control.fLowTemperature == 1))
				{
					MainFan_DutyChange(0);
					SubFan_DutyChange(0);
					DevFan_DutyChange(0);
				}
#endif
			}
			break;
			
		case ST_COOL_STANDBY:
			if (Fuser_Control.RequestState == CMD_FUSER_OFF) 
			{
				Fuser_Control.State = ST_OFF ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_COOL_STANDBY : changed ST_OFF:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0 ;
				DownToOff(); /* [KSF050201] */
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_PRINT) /* [KSF050201] */
			{
				Fuser_Control.State = ST_HEATING ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_COOL_STANDBY : changed ST_HEATING5:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif					
				FuserControlCounter = 0 ;
#if FAN_TEST
				MainFan_DutyChange(MainFanMid);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(SubFanMax);
				DevFan_DutyChange(SubFanMax);
#else
				MainFan_DutyChange(50);		/* [KSF050302] (Fan_High) */
				SubFan_DutyChange(99);
				DevFan_DutyChange(99);
#endif
			}
			else if(Fuser_Control.RequestState == CMD_FUSER_POWERSAVE) /* [KSF050201] */
			{
				Fuser_Control.State = ST_POWER_SAVE ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_COOL_STANDBY : changed ST_POWER_SAVE:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif						
				FuserControlCounter = 0 ;
				DownToPowerSave();
			}				
			//else if(ChangeVoltageToTemp() >= 149 )
			else if((New_AvgTemp/100) <= Adc_Standby )  
			{
				Fuser_Control.State = ST_STANDBY ;
#if ( KSF_DBG_THREE | KSF_DBG_TIMESTAMP )
				printf("ST_COOL_STANDBY : changed ST_STANDBY:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif							
				FuserControlForStandby();
				Fuser_Control.NarrowMediaCount = 0 ; /* [KSF050201] */
			}
			else /* [KSF050201] */
			{
#if KSF_DBG_THREE
				printf("ST_COOL_STANDBY : else:%d (%u)\n",nFuser_Counter, posix_gettime_ms()); /* [KSF050201] */
#endif
				FuserControlForStandby();
			}
			break;
	}
	++FuserControlCounter ;
}

Uint32 GetADCAvg(Uint8* arr) /* [KSF050201] - start */
{
	Uint8 maxVal,minVal,i;
	Uint32 sumVal;
	
	maxVal=0;
	minVal=0;
	sumVal=arr[0];
	
	for(i=1; i<10;i++)
	{
		if(arr[maxVal] < arr[i])
		{
			maxVal = i;
		}
		if(arr[minVal] > arr[i])
		{
			minVal = i;
		}
		sumVal+=arr[i];
	}
//	printf("FUSER : sumVal = %d, ADC_Max = %d, ADC_Min = %d, Result = %d, %d\n", sumVal, arr[maxVal], arr[minVal], (sumVal-arr[maxVal]-arr[minVal])/8, ((sumVal-arr[maxVal]-arr[minVal])*1000)/8);
	// return ((sumVal-arr[maxVal]-arr[minVal])*1000)/8;
	return ((sumVal-arr[minVal])*100)/9;
}

Uint8 FuserControlForPorBeforeMotorRun(void) /* [KSF070801] */
{
	int Temp_CurToWarmup;
	int PorTemp_Off;
	int Param_WarmupTemperature;
	
	Param_WarmupTemperature = 130;
	PorTemp_Off = 200; /* [KSF071003] */

	Temp_CurToWarmup = New_AvgTemp - (Param_WarmupTemperature*100);
#if OVER_HEAT_DBG
	DBG1("[FuserControlForPorBeforeMotorRun] Temp_CurToWarmup:%d Temp_On:%d Temp_Off:%d WarmupCase:%d\n",Temp_CurToWarmup,fuser_cntl_set.Temp_On,PorTemp_Off,WarmupCase_POR);
#endif	
	if((Temp_CurToWarmup >= PorTemp_Off) && (WarmupCase_POR == 3))
	{
		SetWaveform_0();
		WarmupCase_POR = 1;
#if OVER_HEAT_DBG		
		DBG1("### 0 ### : %d (%d)\n",Temp_CurToWarmup, New_AvgTemp);
#endif
	}
	else if(((Temp_CurToWarmup <= fuser_cntl_set.Temp_On) && (WarmupCase_POR == 1)) 
	|| ((WarmupCase_POR == 2) && (cOnPreCnt != 0)))
	{
		if((New_AvgTemp/100) < ((Param_WarmupTemperature) - 10))
		{
#if ZX_ENABLE
			SetWaveform_100();
#else			
			SetWaveform_50(); 
#endif
#if OVER_HEAT_DBG
			DBG1("### 50 ### : %d (%d)\n",Temp_CurToWarmup, New_AvgTemp);
#endif			
		}
		else
		{
#if ZX_ENABLE
			SetWaveform_100();
#else
			SetWaveform_10();		
#endif
#if OVER_HEAT_DBG
			DBG1("### 10 ### : %d (%d)\n",Temp_CurToWarmup, New_AvgTemp);
#endif			
		}
		
		if(WarmupCase_POR == 1)
		{
			cOnPreCnt = fuser_cntl_set.T_On_Pre;
		}
		cOnPreCnt--;
		//WarmupCase_NotPOR = 2;
	}
	else if(((WarmupCase_POR == 2) && (cOnPreCnt == 0)) 
	|| ((WarmupCase_POR == 3) && (Temp_CurToWarmup < PorTemp_Off))) 
	{
		if((New_AvgTemp/100) < ((Param_WarmupTemperature) - 10))
		{
#if ZX_ENABLE
			SetWaveform_100();
#else
			SetWaveform_50();
#endif
#if OVER_HEAT_DBG
			DBG1("### 50 ### : %d (%d)\n",Temp_CurToWarmup, New_AvgTemp);
#endif					
		}
		else
		{
#if ZX_ENABLE
			SetWaveform_100();
#else
			SetWaveform_10();		
#endif
#if OVER_HEAT_DBG
			DBG1("### 10 ### : %d (%d)\n",Temp_CurToWarmup, New_AvgTemp);
#endif			
		}		
		
		WarmupCase_POR = 3;
	}
	else
	{
		SetWaveform_0();
		WarmupCase_POR = 1;
#if OVER_HEAT_DBG
		DBG1("### 0 ### : %d (%d)\n",Temp_CurToWarmup, New_AvgTemp);
#endif		
	}
	
	return 0;
}

void	error_display(void)
{
	ERR("#######################################################\n") ;
	ERR("#            ERROR FUSER : CODE = %d                 #\n", Fuser_Control.FuserFaultNumber);
	ERR("#######################################################\n") ;
}

void	error_display_forFan(void)
{
	ERR("#######################################################\n") ;
	ERR("#            ERROR FAN : CODE = %d                 #\n", Fuser_Control.FuserFaultNumber);
	ERR("#######################################################\n") ;
}

int Check_error(unsigned char ADCData)
{
//	if(CheckJam()!=0)
//		Fuser_Control.State = ST_ERROR ;
//	else if(Fuser_Control.State == ST_ERROR)
//		Fuser_Control.State = ST_OFF ;		
	// if(ADCData > ADC_1)
#if !ABBA2
	/* new 302 *//* [KSF072301] */
	if( Fuser_Control.State == ST_WARMUP )
	{
		if(TotalWarmupFlag != 1)
		{
#if NEW_SC_LOG2			
			DBG1("(1)warmup Start : %d\n",(New_AvgTemp/100));		
#endif			
			totalWarmupCount = 0;
			TotalWarmupFlag = 1;			
		}
		
		if( TotalWarmupFlag == 1 )
		{
#if NEW_SC_LOG2	
			if((nFuser_Counter%10) ==0)
			{
				DBG1("(8) totalWarmupCount:%d (M:%d)\n",totalWarmupCount,Fuser_Control.fMakeMotorRun);
			}
#endif			
#if ABBA2 // Abba2
			if(totalWarmupCount >= 5500/* 55[sec] */)			
#else
			if(totalWarmupCount >= 12000/* 120[sec] */)			
#endif
			{
#if ZX_ENABLE
/* No Error Check */
#else
#if NEW_SC_LOG2			
				DBG1("(9) totalWarmupCount:%d (M:%d)\n",totalWarmupCount,Fuser_Control.fMakeMotorRun);
#endif	
#ifndef IGNORE_HEATER_ERROR
				//  ERROR 302	: Fail to reach STANDBY
				Fuser_Control.FuserFaultNumber = ERROR_POR_NOT_REACH_STANDBY ;
				Fuser_Control.fFuserFault = TRUE ;
				Fuser_Control.State = ST_ERROR;		
				error_display();	
#endif	// IGNORE_HEATER_ERROR										
#endif
			}
			totalWarmupCount++;
		}
	}
	else
	{
		if((nFuser_Counter%100) ==0)
		{
#if NEW_SC_LOG2			
			DBG1("(0) Not Warmup Mode : [%d]\n",Fuser_Control.State);
#endif			
		}

		totalWarmupCount = 0;
		TotalWarmupFlag = 0;
	}
	/* new 302 - end */
#endif
	
#if !ABBA2
	/* 305 *//* [KSF061301] */
	if( Fuser_Control.State == ST_HEATING )
	{
		if(TotalHeatingFlag != 1)
		{
#if NEW_SC_LOG			
			printf("(1)Heating Start : %d\n",(New_AvgTemp/100));		
#endif			
			totalHeatingCount = 0;
			TotalHeatingFlag = 1;			
		}

		if(heatingFlag != 1)
		{
			heatingFlag = 1;
			heatingCount = 0;
			heatingBeforeTemp = (New_AvgTemp/100);
#if NEW_SC_LOG		
			printf("(2)heatingBeforeTemp:%d\n",heatingBeforeTemp);
#endif			
		}
		
		if( TotalHeatingFlag == 1 )/* [KSF061301] */
		{
#if NEW_SC_LOG			
			printf("(8) totalHeatingCount:%d\n",totalHeatingCount);
#endif			
			if(totalHeatingCount >= 3000) /* [KSF062101] */
			{
#if ZX_ENABLE
/* No Error Check */
#else
#if NEW_SC_LOG			
				printf("(9) totalHeatingCount:%d\n",totalHeatingCount);
#endif				
#ifndef IGNORE_HEATER_ERROR
				Fuser_Control.FuserFaultNumber = ERROR_UNDER_TEMPERATUREP_HEATING ;
				Fuser_Control.fFuserFault = TRUE ;
				Fuser_Control.State = ST_ERROR;
				error_display();				
#endif	// IGNORE_HEATER_ERROR				
#endif
			}
			totalHeatingCount++;
		}
		
		if( heatingFlag == 1 )
		{
			if( heatingCount >= 300 )
			{
				heatingAfterTemp = (New_AvgTemp/100);
#if NEW_SC_LOG				
				printf("(3) heatingBeforeTemp:[%d] heatingAfterTemp:[%d]\n",heatingBeforeTemp,heatingAfterTemp);				
#endif
				if((heatingBeforeTemp > heatingAfterTemp)&&(Fuser_Control.fMakeMotorRun == 0))/* [KSF062101] */
				{
#if NEW_SC_LOG					
					printf("(4) heatingBeforeTemp:[%d] > heatingAfterTemp:[%d]\n",heatingBeforeTemp,heatingAfterTemp);
#endif
					heatingErrorCount++;
#if NEW_SC_LOG
					printf("(5) heatingErrorCount:%d\n",heatingErrorCount);
#endif
					if( heatingErrorCount >= 2 ) /* SC 305 */
					{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR
						Fuser_Control.FuserFaultNumber = ERROR_UNDER_TEMPERATUREP_HEATING ;
						Fuser_Control.fFuserFault = TRUE ;
						Fuser_Control.State = ST_ERROR;
						error_display();
#endif	// IGNORE_HEATER_ERROR				
#endif
					}					
				}
				else if((heatingBeforeTemp > (heatingAfterTemp + 5))&&(Fuser_Control.fMakeMotorRun == 1))/* [KSF062101] *//* [KSF080603] */
				{
#if NEW_SC_LOG					
					printf("(4-1) heatingBeforeTemp:[%d] > (heatingAfterTemp + 5):[%d]\n",heatingBeforeTemp,(heatingAfterTemp + 5));
#endif
					heatingErrorCount++;
#if NEW_SC_LOG
					printf("(5-1) heatingErrorCount:%d\n",heatingErrorCount);
#endif
					if( heatingErrorCount >= 2 ) /* SC 305 */
					{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR
						Fuser_Control.FuserFaultNumber = ERROR_UNDER_TEMPERATUREP_HEATING ;
						Fuser_Control.fFuserFault = TRUE ;
						Fuser_Control.State = ST_ERROR;
						error_display();
#endif	// IGNORE_HEATER_ERROR						
#endif
					}					
				}
				else
				{
#if NEW_SC_LOG					
					printf("(6) heatingErrorCount init\n");
#endif
					heatingErrorCount = 0;
				}
				heatingAfterTemp = 0;
				heatingCount = 0;
				heatingFlag = 0;
			}
#if NEW_SC_LOG			
			printf("(7) heatingCount:%d\n",heatingCount);
#endif
			heatingCount++;

		}
		
	}
	else
	{
		if((nFuser_Counter%100) ==0)
		{
#if NEW_SC_LOG			
//			printf("(0) Not Heating Mode : [%d]\n",Fuser_Control.State);
#endif			
		}
		heatingFlag = 0;
		heatingCount = 0;
		totalHeatingCount = 0;
		heatingBeforeTemp = 0;
		heatingAfterTemp = 0;
		heatingErrorCount = 0; 		
	}
	/* 305 - end */
#endif	
	
	if((New_AvgTemp/100) < 1)
	{
		if(f_thermistoropen != TRUE)
		{
			f_thermistoropen = TRUE ;
			n_thermistoropenCount = nFuser_Counter ;					
		}
		else
		{
			if( (nFuser_Counter - n_thermistoropenCount) > OPENTHERMISTER_TIMEOUT )
			{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR
				//  ERROR 304	: THERMISTOR OPEN
				Fuser_Control.FuserFaultNumber = ERROR_THERMISTOR_OPEN ;
				Fuser_Control.fFuserFault = TRUE ;
				Fuser_Control.State = ST_ERROR;
				error_display();
#endif	// IGNORE_HEATER_ERROR				
#endif
			}
		}
	}
	else
	{
		f_thermistoropen = FALSE ;
	}
	
	if((New_AvgTemp/100) > 235)
	{
		if(f_overheat != TRUE)
		{
			f_overheat = TRUE;
			n_thermistorOverTempCount = nFuser_Counter;
		}
		else
		{
			if( (nFuser_Counter - n_thermistorOverTempCount) > THERMISTER_OVER_TEMP_TIMEOUT )
			{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR	
				Fuser_Control.FuserFaultNumber = ERROR_OVER_TERMERATURE;			//  ERROR 303	: Over Temperature
				Fuser_Control.fFuserFault = TRUE;
				Fuser_Control.State = ST_ERROR;		
				error_display();
#endif	// IGNORE_HEATER_ERROR				
#endif
			}
		}
	}
	else
	{
		f_overheat = FALSE;
	}
	
	// if((New_AvgTemp/100) > 245)
	// {
		// if(f_overheat == true)		// error detection on 2 times 
		// {
			// Fuser_Control.FuserFaultNumber = ERROR_OVER_TERMERATURE ;			//  ERROR 303	: Over Temperature
			// Fuser_Control.fFuserFault = TRUE ;
			// Fuser_Control.State = ST_ERROR;		
			// error_display();
		// }
		// f_overheat = true;
	// }
	
	
	if(f_firstStandby== SET)
	{
#if ABBA2 // Abba2
		if((New_AvgTemp/100) < Adc_Standby)
#else
		if((New_AvgTemp/100) < ( Adc_Standby - 10 )) /* [KSF070801] */  
#endif
		{
			if((New_AvgTemp/100) < 35 )	
			{
				if(FuserControlCounter > OPENTHERMISTER_TIMEOUT_EX)
				{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR	
					//  ERROR 304	: THERMISTOR OPEN
					Fuser_Control.FuserFaultNumber = ERROR_THERMISTOR_OPEN ;
					Fuser_Control.fFuserFault = TRUE ;
					Fuser_Control.State = ST_ERROR;
//#ifdef KSF_DBG_ONE
					printf("\n############# under 35' 10sec timeout#################\n");
//#endif
					error_display();
#endif	// IGNORE_HEATER_ERROR					
#endif
				}
			}							
#if ABBA2
			if(FuserControlCounter > REACH_STANDBY_TEMP_TIMEOUT )
			{
#ifndef IGNORE_HEATER_ERROR				
				//  ERROR 302	: Fail to reach STANDBY
				Fuser_Control.FuserFaultNumber = ERROR_POR_NOT_REACH_STANDBY ;
				Fuser_Control.fFuserFault = TRUE ;
				Fuser_Control.State = ST_ERROR;		
				error_display();
#endif	// IGNORE_HEATER_ERROR				
			}
#endif
		}
		else
			f_firstStandby= CLEAR ;
	}

	return 0;
}

#if ABBA2 // Abba2
Uint8 FuserControlForKeepStandby(void)
{
	int Temp_CurToStandby;
	Temp_CurToStandby = New_AvgTemp - (Param_StandbyTemperature*100);

	if((Temp_CurToStandby >= fuser_cntl_set.Temp_Off) && (StandbyCase == 3))
	{
		SetWaveform_0();
		StandbyCase = 1;
	}
	else if(((Temp_CurToStandby <= fuser_cntl_set.Temp_On) && (StandbyCase == 1)) 
	|| ((StandbyCase == 2) && (cOnPreCnt != 0)))
	{
		SetWaveform_25();
		if(StandbyCase == 1)
		{
			cOnPreCnt = fuser_cntl_set.T_On_Pre;
		}
		cOnPreCnt--;
		StandbyCase = 2;
	}
	else if(((StandbyCase == 2) && (cOnPreCnt == 0)) 
	|| ((StandbyCase == 3) && (Temp_CurToStandby < fuser_cntl_set.Temp_Off))) 
	{
		SetWaveform_50(); 				
		StandbyCase = 3;
	}
	else
	{
		SetWaveform_0();
		StandbyCase = 1;		
	}

	if(New_AvgTemp < ((Param_StandbyTemperature - 10)*100))
	{
		return 1;
	}
	return 0;
}
#else // Kara/-S, Pinetree/-S
Uint8 FuserControlForKeepStandby(void) /* [KSF071004] */
{
	int Temp_CurToStandby;
	Temp_CurToStandby = New_AvgTemp - (Param_StandbyTemperature*100);

	fuser_cntl_set.Temp_Off = 200; 
	
	if((Temp_CurToStandby >= fuser_cntl_set.Temp_Off) && (StandbyCase == 3))
	{
		SetWaveform_0();
		StandbyCase = 1;
	}
	else if(((Temp_CurToStandby <= fuser_cntl_set.Temp_On) && (StandbyCase == 1)) 
	|| ((StandbyCase == 2) && (cOnPreCnt != 0)))
	{
		if((New_AvgTemp/100) < ((Param_StandbyTemperature) - 5))
		{
#if ZX_ENABLE
			SetWaveform_100();
#else
			SetWaveform_50(); 	
#endif
		}
		else
		{
#if ZX_ENABLE
			SetWaveform_100();
#else
			//SetWaveform_10(); /* [KF052201] *//* [KSF081902] */				
			SetWaveform_16(); /* [KSF082002] */
			//SetWaveform_12(); /* [KSF090401] */
#endif
		}
		
		if(StandbyCase == 1)
		{
			cOnPreCnt = fuser_cntl_set.T_On_Pre;
		}
		cOnPreCnt--;
		StandbyCase = 2;
	}
	else if(((StandbyCase == 2) && (cOnPreCnt == 0)) 
	|| ((StandbyCase == 3) && (Temp_CurToStandby < fuser_cntl_set.Temp_Off))) 
	{
		if((New_AvgTemp/100) < ((Param_StandbyTemperature) - 5))
		{
#if ZX_ENABLE
			SetWaveform_100();
#else
			SetWaveform_50(); 				
#endif
		}
		else
		{
#if ZX_ENABLE
			SetWaveform_100();
#else
			//SetWaveform_10(); /* [KSF081902] */			
			SetWaveform_16(); /* [KSF082002] */			
			//SetWaveform_12(); /* [KSF090401] */			
#endif
		}		
		
		StandbyCase = 3;
	}
	else
	{
		SetWaveform_0();
		StandbyCase = 1;		
	}

	//if(New_AvgTemp < ((Param_StandbyTemperature - 30)*100)) 
	if(New_AvgTemp < ((Param_StandbyTemperature - 50)*100)) 	/* [KSF120401] for adc(h/w) err */
	{
		return 1;
	}
	return 0;
}
#endif

Uint8 FuserControlForLaminate(void)
{
	int lamanating_duty = 0;
	int Temp_CurToPrint;
	Temp_CurToPrint = New_AvgTemp - (Fuser_Control.Target_Temperature*100);

	if(Temp_CurToPrint >= 0)
	{
		SetWaveform_0();
		lamanating_duty = 0;
	}
	else if(Temp_CurToPrint >= -500)
	{
		SetWaveform_16();
		lamanating_duty = 16;
	}
	else
	{
		SetWaveform_50();
		lamanating_duty = 50;
	}
	
	DBG1("(Eng)Current = %d, Target = %d, duty = %d (laminate)\n", New_AvgTemp, Fuser_Control.Target_Temperature*100, lamanating_duty);
	
	if(New_AvgTemp < ((Fuser_Control.Target_Temperature - 50)*100)) 	/* [KSF120401] for adc(h/w) err */
	{
		return 1;
	}
	return 0;
}

/* [KSF032602] set fuser param */
void Set_Fuser_Parameter(void)
{
	init_fuser_state(); /* [KSF0508] */
	init_temperature();
	init_error_flag();
	NdelayFan = 0;
	
	Fuser_Setting_Parameters[0]  = FUSER_PARAM_TARGET_TMP;
	Param_StandbyTemperature = Fuser_Setting_Parameters[1]  = FUSER_PARAM_STANDBY_TMP;

	Fuser_Setting_Parameters[3]  = FUSER_PARAM_MT_RUN_TMP1;
	Fuser_Setting_Parameters[4]  = FUSER_PARAM_MT_RUN_TMP2;
	Fuser_Setting_Parameters[5]  = FUSER_PARAM_MT_RUN_TMP3;
	Fuser_Setting_Parameters[6]  = FUSER_PARAM_MT_RUN_TMP4;
	Fuser_Setting_Parameters[7]  = FUSER_PARAM_MT_RUN_TMP5;
	Fuser_Setting_Parameters[8]  = FUSER_PARAM_MT_RUN_TIME1;
	Fuser_Setting_Parameters[9]  = FUSER_PARAM_MT_RUN_TIME2;
	Fuser_Setting_Parameters[10] = FUSER_PARAM_MT_RUN_TIME3;
	Fuser_Setting_Parameters[11] = FUSER_PARAM_MT_RUN_TIME4;
	fuser_cntl_set.Ts_NM = Fuser_Setting_Parameters[12] = FUSER_PARAM_Ts_NM;
	fuser_cntl_set.Ts_NS = Fuser_Setting_Parameters[13] = FUSER_PARAM_Ts_NS;
	fuser_cntl_set.Ts_PS = Fuser_Setting_Parameters[14] = FUSER_PARAM_Ts_PS;
	fuser_cntl_set.Ts_PM = Fuser_Setting_Parameters[15] = FUSER_PARAM_Ts_PM;
	Fuser_Setting_Parameters[16] = FUSER_PARAM_Ts_Off;
	fuser_cntl_set.T_On_Pre = Fuser_Setting_Parameters[17] = FUSER_PARAM_T_On_Pre; /* 50 */
	Fuser_Setting_Parameters[18] = FUSER_PARAM_T_On_Cur;
	fuser_cntl_set.Temp_On = Fuser_Setting_Parameters[19] = FUSER_PARAM_Temp_On; /* 0 */
	fuser_cntl_set.Temp_Off = Fuser_Setting_Parameters[20] = FUSER_PARAM_Temp_Off; /* 500 */
	Fuser_Setting_Parameters[21] = FUSER_PARAM_Ts_On_NM;
	Fuser_Setting_Parameters[22] = FUSER_PARAM_Ts_On_NS;
	Fuser_Setting_Parameters[23] = FUSER_PARAM_Ts_On_ZR;
	Fuser_Setting_Parameters[24] = FUSER_PARAM_Ts_On_PS;
	fuser_cntl_set.Tt_NB = Fuser_Setting_Parameters[25] = FUSER_PARAM_Tt_NB;
	fuser_cntl_set.Tt_NM = Fuser_Setting_Parameters[26] = FUSER_PARAM_Tt_NM;
	fuser_cntl_set.Tt_NS = Fuser_Setting_Parameters[27] = FUSER_PARAM_Tt_NS;
	fuser_cntl_set.Tt_PS = Fuser_Setting_Parameters[28] = FUSER_PARAM_Tt_PS;
	fuser_cntl_set.Tt_PM = Fuser_Setting_Parameters[29] = FUSER_PARAM_Tt_PM;
	fuser_cntl_set.dTt_NB = Fuser_Setting_Parameters[30] = FUSER_PARAM_dTt_NB;
	fuser_cntl_set.dTt_NM = Fuser_Setting_Parameters[31] = FUSER_PARAM_dTt_NM;
	fuser_cntl_set.dTt_NS = Fuser_Setting_Parameters[32] = FUSER_PARAM_dTt_NS; 
	fuser_cntl_set.dTt_PS = Fuser_Setting_Parameters[33] = FUSER_PARAM_dTt_PS; 
	fuser_cntl_set.dTt_PM = Fuser_Setting_Parameters[34] = FUSER_PARAM_dTt_PM; 
	fuser_cntl_set.dTt_PB = Fuser_Setting_Parameters[35] = FUSER_PARAM_dTt_PB;
	fuser_cntl_set.FanHighDuty = Fuser_Setting_Parameters[36] = FUSER_PARAM_FAN_HIGH_SPEED_DUTY;
	fuser_cntl_set.FanLowDuty = Fuser_Setting_Parameters[37] = FUSER_PARAM_FAN_LOW_SPEED_DUTY;
	fuser_cntl_set.dup_target_temp = Fuser_Setting_Parameters[38] = FUSER_PARAM_DUP_TARGET_TMP;
	fuser_cntl_set.StandbyFanDelay = Fuser_Setting_Parameters[39] = FUSER_PARAM_FAN_TIME_BEFORE_STANDBY;
	fuser_cntl_set.PwsFanRun = Fuser_Setting_Parameters[40] = FUSER_PARAM_FAN_TIME_PWR_SAVE;
	fuser_cntl_set.mainFanLow = Fuser_Setting_Parameters[41] = FUSER_PARAM_MAIN_FAN_LOW;
	fuser_cntl_set.mainFanMin = Fuser_Setting_Parameters[42] = FUSER_PARAM_MAIN_FAN_MIN;
	fuser_cntl_set.subFanLow = Fuser_Setting_Parameters[43] = FUSER_PARAM_SUB_FAN_LOW;
	fuser_cntl_set.coldWakeHoldTemp = Fuser_Setting_Parameters[44] = FUSER_PARAM_COLD_WAKEUP_HOLDING_TEMP;
	fuser_cntl_set.coldWakeHoldTime = Fuser_Setting_Parameters[45] = FUSER_PARAM_COLD_WAKEUP_HOLDING_TIME;
	fuser_cntl_set.coldWakeTargTemp = Fuser_Setting_Parameters[46] = FUSER_PARAM_COLD_WAKEUP_TAR_TEMP;
//	DisplayFuserParams();
#if KSF_DBG_SET_DUTY
	getTestFuserPrintDutyTableValue();
#endif	
#if KSF_DBG_SET_DUTY_WH
	getTestFuserWarmupHeatingDutyTableValue(); /* [KSF090601] */
#endif
}   

void DisplayFuserParams(void)
{
	printf("+--------------------------------+\n");
	printf("|   KARA-S FUSER PARAMETER VALUE  |\n");
	printf("+--------------------------------+\n");
	printf("|   TargetTemp :	%d	 |\n",Fuser_Setting_Parameters[0]);
	printf("|   StandbyTemp :	%d	 |\n",Fuser_Setting_Parameters[1]);
	printf("|   PickupTemp :	%d	 |\n",Fuser_Setting_Parameters[2]);
	printf("|   MT_RunTemp1 :	%d	 |\n",Fuser_Setting_Parameters[3]);
	printf("|   MT_RunTemp2 :	%d	 |\n",Fuser_Setting_Parameters[4]);
	printf("|   MT_RunTemp3 :	%d	 |\n",Fuser_Setting_Parameters[5]);
	printf("|   MT_RunTemp4 :	%d	 |\n",Fuser_Setting_Parameters[6]);
	printf("|   MT_RunTemp5 :	%d	 |\n",Fuser_Setting_Parameters[7]);
	printf("|   MT_RunTime1 :	%d	 |\n",Fuser_Setting_Parameters[8]);
	printf("|   MT_RunTime2 :	%d	 |\n",Fuser_Setting_Parameters[9]);
	printf("|   MT_RunTime3 :	%d	 |\n",Fuser_Setting_Parameters[10]);
	printf("|   MT_RunTime4 :	%d	 |\n",Fuser_Setting_Parameters[11]);
	printf("|   Ts_NM :		%d	 |\n",Fuser_Setting_Parameters[12]);
	printf("|   Ts_NS :		%d	 |\n",Fuser_Setting_Parameters[13]);
	printf("|   Ts_PS :		%d	 |\n",Fuser_Setting_Parameters[14]);
	printf("|   Ts_PM :		%d	 |\n",Fuser_Setting_Parameters[15]);
	printf("|   Ts_Off :		%d	 |\n",Fuser_Setting_Parameters[16]);
	printf("|   T_On_Pre :		%d	 |\n",Fuser_Setting_Parameters[17]);
	printf("|   T_On_Cur :		%d	 |\n",Fuser_Setting_Parameters[18]);
	printf("|   Temp_On :		%d	 |\n",Fuser_Setting_Parameters[19]);
	printf("|   Temp_Off :		%d	 |\n",Fuser_Setting_Parameters[20]);
	printf("|   Ts_On_NM :		%d	 |\n",Fuser_Setting_Parameters[21]);
	printf("|   Ts_On_NS :		%d	 |\n",Fuser_Setting_Parameters[22]);
	printf("|   Ts_On_ZR :		%d	 |\n",Fuser_Setting_Parameters[23]);
	printf("|   Ts_On_PS :		%d	 |\n",Fuser_Setting_Parameters[24]);
	printf("|   Tt_NB :		%d	 |\n",Fuser_Setting_Parameters[25]);
	printf("|   Tt_NM :		%d	 |\n",Fuser_Setting_Parameters[26]);
	printf("|   Tt_NS :		%d	 |\n",Fuser_Setting_Parameters[27]);
	printf("|   Tt_PS :		%d	 |\n",Fuser_Setting_Parameters[28]);
	printf("|   Tt_PM :		%d	 |\n",Fuser_Setting_Parameters[29]);
	printf("|   dTt_NB :		%d	 |\n",Fuser_Setting_Parameters[30]);
	printf("|   dTt_NM :		%d	 |\n",Fuser_Setting_Parameters[31]);
	printf("|   dTt_NS : 		%d	 |\n",Fuser_Setting_Parameters[32]);
	printf("|   dTt_PS :		%d	 |\n",Fuser_Setting_Parameters[33]);
	printf("|   dTt_PM :		%d	 |\n",Fuser_Setting_Parameters[34]);
	printf("|   dTt_PS :		%d	 |\n",Fuser_Setting_Parameters[35]);
	printf("|  FAN_TIME_BEFORE_STANDBY :	%d	 \n",Fuser_Setting_Parameters[39]);
	printf("|  FAN_TIME_PWR_SAVE :	%d	 \n",Fuser_Setting_Parameters[40]);
	printf("|  MAIN_FAN_LOW :	%d	 \n",Fuser_Setting_Parameters[41]);
	printf("|  MAIN_FAN_MIN :	%d	 \n",Fuser_Setting_Parameters[42]);
	printf("|  SUB_FAN_LOW :	%d	 \n",Fuser_Setting_Parameters[43]);
	printf("|  ColdWake_HoldTemp :	%d	 \n",Fuser_Setting_Parameters[44]);
	printf("|  ColdWake_HoldTime :	%d	 \n",Fuser_Setting_Parameters[45]);
	printf("|  ColdWake_TargTemp :	%d	 \n",Fuser_Setting_Parameters[46]);
	printf("+--------------------------------+\n");
	// printf("|   DX 1st Normal num of page :	%d	 \n",Fuser_Setting_Parameters[36]);
	// printf("|   DX 2nd Normal num of page :	%d	 \n",Fuser_Setting_Parameters[37]);
	// printf("|   DX slow cooling time  :	%d	 \n",Fuser_Setting_Parameters[38]);
	// printf("|   DX idle fan run time  :	%d	 \n",Fuser_Setting_Parameters[39]);
	// printf("|   Fan High Speed (per)  :	%d	 \n",Fuser_Setting_Parameters[40]);
	// printf("|   Fan Low Speed (per)  :	%d	 \n",Fuser_Setting_Parameters[41]);
	// printf("|   Target Temp (Duplex)  :	%d	 \n",Fuser_Setting_Parameters[42]);
	// printf("+--------------------------------+\n");
}

void DisplayFuserControlParam( void )
{
	printf("[1].Fuser_Control.Current_Temperature:%d\n",Fuser_Control.Current_Temperature);
	printf("[2].Fuser_Control.PaperJam:%d\n",Fuser_Control.PaperJam);
	printf("[3].Fuser_Control.TempPaperJam:%d\n",Fuser_Control.TempPaperJam);
	printf("[4].Fuser_Control.EM_State:%d\n",Fuser_Control.EM_State);
	printf("[5].Fuser_Control.fPowerSaveMode:%d\n",Fuser_Control.fPowerSaveMode);
	printf("[6].Fuser_Control.fMakeMotorRun:%d\n",Fuser_Control.fMakeMotorRun);
	printf("[7].Fuser_Control.fFatalError:%d\n",Fuser_Control.fFatalError);
	printf("[8].Fuser_Control.fPOR_PageSupervisor_Active:%d\n",Fuser_Control.fPOR_PageSupervisor_Active);
	printf("[9].Fuser_Control.fMainMotorState:%d\n",Fuser_Control.fMainMotorState);
	printf("[10].Fuser_Control.StandbyModeType:%d\n",Fuser_Control.StandbyModeType);
	printf("[11].Fuser_Control.NarrowMediaCount:%d\n",Fuser_Control.NarrowMediaCount);
	printf("[12].Fuser_Control.fLowTemperature:%d\n",Fuser_Control.fLowTemperature);
	printf("[13].Fuser_Control.fCoverOpen:%d\n",Fuser_Control.fCoverOpen);
}

void FuserHeaterManager( void )  /* [KSF032501] for Heater Manager Control */
{
	Uint8 ADC_Count;
	Uint8 Waveform_Count;

	ADC_Count = nFuser_Counter%10;
	Waveform_Count = nFuser_Counter % WaveformDuty;
	
	ADCData = Get_ADCData();

#if KSF_TEMP_DISPLAY
	if( ( nFuser_Counter % 50 ) == 0 ) /* [KSF032802] for DBG */
	{
		printf("[%d]:%d\n",nFuser_Counter,ChangeVoltageToTemp());
	}
#endif	

	/* standby, warmup, heating, print */
	if( Fuser_Control.State == ST_PRINT )
	{
		//ADC_buf[ADC_Count] = ChangeVoltageToTempPrint();
		ADC_buf[ADC_Count] = ChangeVoltageToTemp();				
	}
	else
	{
		ADC_buf[ADC_Count] = ChangeVoltageToTemp();
		if((ADC_buf[ADC_Count] == 255)&&((Fuser_Control.State == ST_POWER_SAVE) || (Fuser_Control.State == ST_STANDBY))) /* [KSF121101] for therm err */
		{
			ADC_buf[ADC_Count] = 0;
		}
	}
	
//	Fuser_Control.Current_Temperature = ADC_buf[ADC_Count];
	
#if ZX_ENABLE
	switch (Fuser_Control.State)
	{
		case ST_WARMUP:
			fuserstate = ZX_WARMUP;
			break;
		case ST_STANDBY:
			fuserstate = ZX_READY;
			break;
		case ST_HEATING:
		case ST_PRINT:
			fuserstate = ZX_PRINT;
			break;
		default:
			fuserstate = ZX_WARMUP;
			break;
	}
	
	if((Waveform_buf[Waveform_Count] != Pre_Waveform_buf) || (Fuser_Control.State != Pre_Fuser_Control.State))
	{
		if(Waveform_buf[Waveform_Count] != 0)
		{
			DBG1("(+) H-ON:%d(%d[%d]%d)\n",Waveform_Count,nFuser_Counter,WaveformDuty,Waveform_buf[Waveform_Count]);			
			FuserOn(fuserstate);
		}
		else
		{
			DBG1("<-> H-OFF:%d(%d[%d]%d)\n",Waveform_Count,nFuser_Counter,WaveformDuty,Waveform_buf[Waveform_Count]);			
			FuserOff();
		}
	}
	Pre_Waveform_buf = Waveform_buf[Waveform_Count];
#else
	if(Waveform_buf[Waveform_Count] != 0)
	{
//			printf("(+) H-ON:%d(%d[%d]%d)\n",Waveform_Count,nFuser_Counter,WaveformDuty,Waveform_buf[Waveform_Count]);			
			FuserOn(OndoNo);
	}
	else
	{
//			printf("<-> H-OFF:%d(%d[%d]%d)\n",Waveform_Count,nFuser_Counter,WaveformDuty,Waveform_buf[Waveform_Count]);			
			FuserOff();
	}
#endif
	Pre_Fuser_Control = Fuser_Control;
	
	if(ADC_Count == 9)
	{
		/*Temp Average during 100ms*/
		ADC_Avg_buf[ptAvg] = GetADCAvg((Uint8*)ADC_buf);
//		printf("==> ADC_Avg_buf[ptAvg]:%d\n",ADC_Avg_buf[ptAvg]);
		ptAvg++;
		if(ptAvg==5)
		{
			if(nFuser_Counter==49)
			{
				Old_AvgTemp = ADC_Avg_buf[0];
			}
			/*Temp result during 500ms*/
			ptAvg=0;
			sum_xy = ADC_Avg_buf[0] + ADC_Avg_buf[1]*2 + ADC_Avg_buf[2]*3 + ADC_Avg_buf[3]*4 + ADC_Avg_buf[4]*5;
			sum_y = Old_AvgTemp + ADC_Avg_buf[0] + ADC_Avg_buf[1] + ADC_Avg_buf[2] + ADC_Avg_buf[3] + ADC_Avg_buf[4];
			New_AvgTemp = (((2*sum_xy/7)-(5*sum_y/7)) + ((11*sum_y/21)-(3*sum_xy/21)));
			if(New_AvgTemp < 0)	New_AvgTemp = 0;
			// Temp_Difference = New_AvgTemp/1000 - Old_AvgTemp/1000;
			Temp_Difference = New_AvgTemp - Old_AvgTemp;
#if KSF_DBG_ONE			
			printf("[FUS] FUSER(%dms) : Old_AvgTemp=%d, New_AvgTemp=%d, Temp_Difference = %d\n",nFuser_Counter, Old_AvgTemp, New_AvgTemp, Temp_Difference);
#endif
			Old_AvgTemp = New_AvgTemp;
			
			Fuser_Control.Current_Temperature = New_AvgTemp/100;
			/* Fuser State & TargetTemp Control */
			Fusermanager();
		}
	}
	
	nFuser_Counter++;
	
	// Fuser error check
	if(!((Fuser_Control.State == ST_ERROR)||(Fuser_Control.State==ST_WAIT_RUN)||(Fuser_Control.State == ST_OFF)||(Fuser_Control.State == ST_POWER_SAVE)))
	{
		Check_error(ADCData);
	}
	else
	{
		f_thermistoropen = FALSE;
		f_overheat = FALSE;
	}	
#if FAN_MGR	
	FanManager();
#endif
#if STRESS_TEST_TONER_IC_AUTH
	if( (( nFuser_Counter % 100 ) == 0 )&&(Fuser_Control.State == ST_STANDBY) ) /* [KSR062601] for Stress Test Toner IC AUTH */
	{
		SecurityIC_Check();
	}
#endif
	
#if STRESS_TEST_TONER_IC
	if( (( nFuser_Counter % 10 ) == 0 )&&(Fuser_Control.State == ST_STANDBY) ) /* [KSR062501] for Stress Test Toner IC */
	{
		printf("\n[Stress Test] Write(%d)\n",(nFuser_Counter/10));
		if(MemoryWrite(40,(nFuser_Counter/10), TONER) != 0)
		{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR
			printf("Write ERROR!!!\n");
			Fuser_Control.FuserFaultNumber = ERROR_UNDER_TEMPERATUREP_HEATING ;
			Fuser_Control.fFuserFault = TRUE ;
			Fuser_Control.State = ST_ERROR;
			error_display();			
#endif	// IGNORE_HEATER_ERROR
#endif
		}
	}

	posix_sleep_ms(10);	

	if( (( nFuser_Counter % 10 ) == 0)&&(Fuser_Control.State == ST_STANDBY) ) /* [KSR062501] for Stress Test Toner IC */
	{
		if((nFuser_Counter/10) != (MemoryRead(40, TONER)))
		{
#if ZX_ENABLE
/* No Error Check */
#else
#ifndef IGNORE_HEATER_ERROR
			printf("[Stress Test Error] (%d)(%d)\n",(nFuser_Counter/10),(MemoryRead(40, TONER)));
			Fuser_Control.FuserFaultNumber = ERROR_UNDER_TEMPERATUREP_HEATING ;
			Fuser_Control.fFuserFault = TRUE ;
			Fuser_Control.State = ST_ERROR;
			error_display();			
#endif	// IGNORE_HEATER_ERROR
#endif
		}
		else
		{
			printf("==> OK\n");
		}
	}
#endif	
	
}

 /* [KSF032601] for Heater Manager Control (add waveform) */
void SetWaveform_100(void)
{
	WaveformDuty = 50;
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;	Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;	Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;	Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;	Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;	Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;	Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;	Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;	Waveform_buf[26] = 100;Waveform_buf[27] = 100;Waveform_buf[28] = 100;	Waveform_buf[29] = 100;
	Waveform_buf[30] = 100;Waveform_buf[31] = 100;Waveform_buf[32] = 100;	Waveform_buf[33] = 100;Waveform_buf[34] = 100;Waveform_buf[35] = 100;	Waveform_buf[36] = 100;Waveform_buf[37] = 100;Waveform_buf[38] = 100;	Waveform_buf[39] = 100;
	Waveform_buf[40] = 100;Waveform_buf[41] = 100;Waveform_buf[42] = 100;	Waveform_buf[43] = 100;Waveform_buf[44] = 100;Waveform_buf[45] = 100;	Waveform_buf[46] = 100;Waveform_buf[47] = 100;Waveform_buf[48] = 100;	Waveform_buf[49] = 100;
#if KSF_DBG_DUTY
	DBG1("100[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
}

void SetWaveform_90(void) /* [KSF081901] */
{
	WaveformDuty = 50;
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;	Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;	Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;	Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;	Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;	Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;	Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;	Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;	Waveform_buf[26] = 100;Waveform_buf[27] = 100;Waveform_buf[28] = 100;	Waveform_buf[29] = 100;
	Waveform_buf[30] = 100;Waveform_buf[31] = 100;Waveform_buf[32] = 100;	Waveform_buf[33] = 100;Waveform_buf[34] = 100;Waveform_buf[35] = 100;	Waveform_buf[36] = 100;Waveform_buf[37] = 100;Waveform_buf[38] = 100;	Waveform_buf[39] = 100;
	Waveform_buf[40] = 100;Waveform_buf[41] = 100;Waveform_buf[42] = 100;	Waveform_buf[43] = 100;Waveform_buf[44] = 100; Waveform_buf[45] = 0;		Waveform_buf[46] = 0;  Waveform_buf[47] = 0;  Waveform_buf[48] = 0;		Waveform_buf[49] = 0;
#if KSF_DBG_DUTY
	DBG1("90[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
}

void SetWaveform_80(void) /* [KSF081901] */
{
	WaveformDuty = 50;
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;	Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;	Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;	Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;	Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;	Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;	Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;	Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;	Waveform_buf[26] = 100;Waveform_buf[27] = 100;Waveform_buf[28] = 100;	Waveform_buf[29] = 100;
	Waveform_buf[30] = 100;Waveform_buf[31] = 100;Waveform_buf[32] = 100;	Waveform_buf[33] = 100;Waveform_buf[34] = 100;Waveform_buf[35] = 100;	Waveform_buf[36] = 100;Waveform_buf[37] = 100;Waveform_buf[38] = 100;	Waveform_buf[39] = 100;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0; Waveform_buf[45] = 0;		Waveform_buf[46] = 0;  Waveform_buf[47] = 0;  Waveform_buf[48] = 0;		Waveform_buf[49] = 0;
#if KSF_DBG_DUTY
	DBG1("80[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
}

void SetWaveform_75(void)
{
	WaveformDuty = 50;
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
#if ZX_ENABLE
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;Waveform_buf[26] = 100;Waveform_buf[27] = 100;Waveform_buf[28] = 100;Waveform_buf[29] = 100;
	Waveform_buf[30] = 100;Waveform_buf[31] = 100;Waveform_buf[32] = 100;Waveform_buf[33] = 100;Waveform_buf[34] = 100;Waveform_buf[35] = 100;Waveform_buf[36] = 100;Waveform_buf[37] = 100;Waveform_buf[38] = 0;Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("75[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;Waveform_buf[26] = 100;Waveform_buf[27] = 100;Waveform_buf[28] = 100;Waveform_buf[29] = 100;
	Waveform_buf[30] = 100;Waveform_buf[31] = 100;Waveform_buf[32] = 100;Waveform_buf[33] = 100;Waveform_buf[34] = 100;Waveform_buf[35] = 100;Waveform_buf[36] = 100;Waveform_buf[37] = 50;Waveform_buf[38] = 0;Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("75[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_70(void) /* [KSF081901] */
{
	WaveformDuty = 50;
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;	Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;	Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;	Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;	Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;	Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;	Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;	Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;	Waveform_buf[26] = 100;Waveform_buf[27] = 100;Waveform_buf[28] = 100;	Waveform_buf[29] = 100;
	Waveform_buf[30] = 100;Waveform_buf[31] = 100;Waveform_buf[32] = 100;	Waveform_buf[33] = 100;Waveform_buf[34] = 100;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0; Waveform_buf[45] = 0;		Waveform_buf[46] = 0;  Waveform_buf[47] = 0;  Waveform_buf[48] = 0;		Waveform_buf[49] = 0;
#if KSF_DBG_DUTY
	DBG1("70[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
}

void SetWaveform_60(void) /* [KSF081901] */
{
	WaveformDuty = 50;
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;	Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;	Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;	Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;	Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;	Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;	Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;	Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;	Waveform_buf[26] = 100;Waveform_buf[27] = 100;Waveform_buf[28] = 100;	Waveform_buf[29] = 100;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0; Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0; Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;
#if KSF_DBG_DUTY
	DBG1("60[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
}

void SetWaveform_50(void)
{
	WaveformDuty = 50;
#if ZX_ENABLE
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 100;Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY	
	DBG1("50[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;Waveform_buf[19] = 100;
	Waveform_buf[20] = 100;Waveform_buf[21] = 100;Waveform_buf[22] = 100;Waveform_buf[23] = 100;Waveform_buf[24] = 100;Waveform_buf[25] = 50;Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY	
	DBG1("50[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_40(void) /* [KSF081901] */
{
	WaveformDuty = 50;
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;	Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;	Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;	Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;	Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;	Waveform_buf[16] = 100;Waveform_buf[17] = 100;Waveform_buf[18] = 100;	Waveform_buf[19] = 100;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0; Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0; Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0; Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;
#if KSF_DBG_DUTY
	DBG1("40[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
}

void SetWaveform_33(void)
{
	WaveformDuty = 50;
#if ZX_ENABLE
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;Waveform_buf[16] = 100;Waveform_buf[17] = 0;Waveform_buf[18] = 0;Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("33[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 100;Waveform_buf[16] = 50;Waveform_buf[17] = 0;Waveform_buf[18] = 0;Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("33[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_30(void) /* [KSF081901] */
{
	WaveformDuty = 50;
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;	Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;	Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;	Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;	Waveform_buf[13] = 100;Waveform_buf[14] = 100;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0; Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0; Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;
#if KSF_DBG_DUTY
	DBG1("30[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
}

void SetWaveform_25(void)
{
	WaveformDuty = 50;	
#if ZX_ENABLE
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 100;Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("25[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 100;Waveform_buf[12] = 50;Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("25[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_20(void)
{
	WaveformDuty = 50;
#if ZX_ENABLE
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 100;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("20[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 100;
	Waveform_buf[10] = 50;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("20[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_16(void)
{
	WaveformDuty = 50;
#if ZX_ENABLE
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 100;Waveform_buf[ 9] = 0;
	Waveform_buf[10] = 0;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("16[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 100;Waveform_buf[ 8] = 50;Waveform_buf[ 9] = 0;
	Waveform_buf[10] = 0;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("16[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_12(void) /* [KSF090401] */
{
	WaveformDuty = 50;
#if ZX_ENABLE
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 100;Waveform_buf[ 7] = 0;Waveform_buf[ 8] = 0;Waveform_buf[ 9] = 0;
	Waveform_buf[10] = 0;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("12[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 50;Waveform_buf[ 7] = 0;Waveform_buf[ 8] = 0;Waveform_buf[ 9] = 0;
	Waveform_buf[10] = 0;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("12[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_10(void)
{
	WaveformDuty = 50;
#if ZX_ENABLE
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 100;Waveform_buf[ 6] = 0;Waveform_buf[ 7] = 0;Waveform_buf[ 8] = 0;	Waveform_buf[ 9] = 0;
	Waveform_buf[10] = 0;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("10[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#else
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 100;Waveform_buf[ 1] = 100;Waveform_buf[ 2] = 100;Waveform_buf[ 3] = 100;Waveform_buf[ 4] = 100;Waveform_buf[ 5] = 50;Waveform_buf[ 6] = 0;Waveform_buf[ 7] = 0;Waveform_buf[ 8] = 0;	Waveform_buf[ 9] = 0;
	Waveform_buf[10] = 0;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;		
#if KSF_DBG_DUTY
	DBG1("10[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
#endif
}

void SetWaveform_0(void)
{
	WaveformDuty = 50;
	 /* [KSF032601] for Heater Manager Control (add waveform) *//* [KSF050803] */
	Waveform_buf[ 0] = 0;Waveform_buf[ 1] = 0;Waveform_buf[ 2] = 0;	Waveform_buf[ 3] = 0;Waveform_buf[ 4] = 0;Waveform_buf[ 5] = 0;	Waveform_buf[ 6] = 0;Waveform_buf[ 7] = 0;Waveform_buf[ 8] = 0;	Waveform_buf[ 9] = 0;
	Waveform_buf[10] = 0;Waveform_buf[11] = 0;Waveform_buf[12] = 0;	Waveform_buf[13] = 0;Waveform_buf[14] = 0;Waveform_buf[15] = 0;	Waveform_buf[16] = 0;Waveform_buf[17] = 0;Waveform_buf[18] = 0;	Waveform_buf[19] = 0;
	Waveform_buf[20] = 0;Waveform_buf[21] = 0;Waveform_buf[22] = 0;	Waveform_buf[23] = 0;Waveform_buf[24] = 0;Waveform_buf[25] = 0;	Waveform_buf[26] = 0;Waveform_buf[27] = 0;Waveform_buf[28] = 0;	Waveform_buf[29] = 0;
	Waveform_buf[30] = 0;Waveform_buf[31] = 0;Waveform_buf[32] = 0;	Waveform_buf[33] = 0;Waveform_buf[34] = 0;Waveform_buf[35] = 0;	Waveform_buf[36] = 0;Waveform_buf[37] = 0;Waveform_buf[38] = 0;	Waveform_buf[39] = 0;
	Waveform_buf[40] = 0;Waveform_buf[41] = 0;Waveform_buf[42] = 0;	Waveform_buf[43] = 0;Waveform_buf[44] = 0;Waveform_buf[45] = 0;	Waveform_buf[46] = 0;Waveform_buf[47] = 0;Waveform_buf[48] = 0;	Waveform_buf[49] = 0;	

#ifdef KSF_DBG_DUTY
//	DBG1("0[pc] Tn:%d Tt:%d Td:%d\n",(New_AvgTemp/100), Fuser_Control.Target_Temperature, Temp_Difference);
#endif
} /* [KSF050201] - end */

void Set_Debug_Standby_Temp(unsigned char debug_on)
{
	if(debug_on == SET)
	{
		Param_StandbyTemperature = DEBUG_StandbyTemp;
		Adc_Standby = DEBUG_StandbyTemp;
	}
	else
	{
		Param_StandbyTemperature = FUSER_PARAM_STANDBY_TMP; // default standby temparature
		Adc_Standby = FUSER_PARAM_STANDBY_TMP;
	}
	return;
}

void Read_Fuser_Func(void)
{
	if(Fuser_Control.State != Pre_Fuser_Control.State)
			DBG1("(eng)fuser_state : %d -> %x <%d>\n", Pre_Fuser_Control.State, Fuser_Control.State, cProcessCounter);
}

void Write_Fuser_Func(void)
{
#if ABBA2
	Adc_Print = Fuser_Control.Target_Temperature;	
	Adc_Print_Low = Fuser_Control.Target_Temperature-Param_PickupTemperature;
#endif
	return;
}

#if ZX_ENABLE
void Get_ZX_Test_Parameter(void)
{
	FILE	*paramFP;
    char white1[80];
    int32_t ep_param[2];
    
    paramFP = fopen("/app/ZX_PARAMETER.txt", "r");
    if(paramFP == NULL)
    {
		DBG1("(eng)ZX_PARAMETER.txt open fail\n");
	}
	else
	{
		fscanf( paramFP, "%s%d%s%d", 
			white1, &ep_param[0], 
			white1, &ep_param[1]
		);
		
		DBG1("##################################\n");
		DBG1("##	TargetTemp		=	%d	##\n", ep_param[0]);
		DBG1("##	StandbyTemp		=	%d	##\n", ep_param[1]);
		DBG1("##################################\n");
		
		Param_PrintTemperature 		=  ep_param[0];
		Param_StandbyTemperature 	=  ep_param[1];
		Adc_Standby = ep_param[1];
		Adc_Print_Low = Param_PrintTemperature-Param_PickupTemperature;
		Fuser_Setting_Parameters[0] = ep_param[0];
		Fuser_Setting_Parameters[1] = ep_param[1];
		fclose(paramFP);
	}

	return;
}
#endif

void Set_Fuser_PreHeating(void)
{
	Fuser_Control.Target_Temperature = FUSER_PARAM_TMP_LAMINATE;
	Param_StandbyTemperature 	=  FUSER_PARAM_TMP_LAMINATE_PREHEAT ;
	Param_PrintTemperature 		=  FUSER_PARAM_TMP_LAMINATE ;
	Param_PickupTemperature 	=  FUSER_PARAM_PICKUP_TMP_LAMINATE ;

	Param_PickupTemperature = Fuser_Setting_Parameters[2]  = FUSER_PARAM_PICKUP_TMP_LAMINATE;
	Adc_Standby = FUSER_PARAM_TMP_LAMINATE_PREHEAT ;

	Adc_Print_Low = Param_PrintTemperature-Param_PickupTemperature;
	Adc_Print = FUSER_PARAM_TMP_LAMINATE;
	DBG1("(eng)Set_Fuser_PreHeating\n");
}

void Set_Fuser_Laminating_Standby(void)
{
	Fuser_Control.Target_Temperature = FUSER_PARAM_TMP_LAMINATE;
	Param_StandbyTemperature 	=  FUSER_PARAM_STANDBY_TMP ;
	Param_PrintTemperature 		=  FUSER_PARAM_TMP_LAMINATE ;
	Param_PickupTemperature 	=  FUSER_PARAM_PICKUP_TMP_LAMINATE ;

	Param_PickupTemperature = Fuser_Setting_Parameters[2]  = FUSER_PARAM_PICKUP_TMP_LAMINATE;
	Adc_Standby = FUSER_PARAM_STANDBY_TMP ;

	Adc_Print_Low = Param_PrintTemperature-Param_PickupTemperature;
	Adc_Print = FUSER_PARAM_TMP_LAMINATE;
	DBG1("(eng)Set_Fuser_Laminating_Standby\n");
}

