/***********************************/
/* Fuser Device Driver Header File */
/* YoungJae Jang, Sindoh.          */
/***********************************/
#ifndef Fuser
#define Fuser

#include "spec.h"

//#include "Fuser_DC1250.h"
#define FAN_TEST 1
#define FAN_LOG 0
/* General Definition */
#define FUSER_DEV_NAME		"/dev/fuserdev"
#define FUSER_DESCRIPTION	"Official_Release"
#define FUSER_MAJOR_NUM		240
#define FIXHZ	1000
#define FUSER_100MS			(1*FIXHZ/10)
#define FUSER_10MS			(1*FIXHZ/100)
//#define MICROSEC				1000
#define COLD_WAKEUP_TEMP	130
	
#define	OPENTHERMISTER_TIMEOUT			600	//10					// (Unit : 10msec)
#define	THERMISTER_OVER_TEMP_TIMEOUT	100	//10					// (Unit : 10msec)
#define OPENTHERMISTER_TIMEOUT_EX		20	//1000	//100			// (Unit : 500msec)
#if ABBA2 
#define	UNDERTEMPERATURE_TIMEOUT		20 	// (Unit : 500msec)
#define REACH_STANDBY_TEMP_TIMEOUT		110	// (Unit : 500msec)
#else
#define	UNDERTEMPERATURE_TIMEOUT		120 //60  //20	//10	//1000	//100	// (Unit : 500msec)	/* [KSF070302] *//* [KSF120401] for adc(h/w) err 30s->1m */
#define REACH_STANDBY_TEMP_TIMEOUT		180 //110	//120	//6000	//600	// (Unit : 500msec) /* [KSF070301] *//* [KSF071702] *//* [KSF071801] */
#endif
#define FAN_LOCKDOWN_TIMEOUT			500		//50
#define STANDBY_TIMEOUT					120 /* [KSR032701] standby state timeout (1 min) */
#define POWER_LOW_TIMEOUT				( 120 * 60 ) /* [KSR032701] power low state timeout (60 min) */

/* [KSR031801] siyo add  */
#define SUB_FAN_LOCKDOWN_TIMEOUT		600

enum {
	ZX_WARMUP = 1,
	ZX_READY = 2,
	ZX_PRINT = 3,
};
/****************************************/
/* Fuser Device Driver control function */
/****************************************/
#if 0
static int Fuser_init (void);
static void Fuser_exit(void);
static int Fuser_open (struct inode *inode, struct file *filp);
static int Fuser_release (struct inode *inode, struct file *filp);
static ssize_t Fuser_read (struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t Fuser_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int Fuser_ioctl (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

void save_thermal(void) ;

module_init(Fuser_init);	//Fuser module entry
module_exit(Fuser_exit);	//Fuser module remove

MODULE_AUTHOR ("Youngjin Lee @ Sindoh");
MODULE_DESCRIPTION ("Abba/Oak Fuser Device Driver");
MODULE_LICENSE ("Proprietary");

struct file_operations Fuser_fops =
{
	.owner   = THIS_MODULE,
	.read    = Fuser_read,
	.write	 = Fuser_write,
	.ioctl   = Fuser_ioctl,
	.open    = Fuser_open,
	.release = Fuser_release,
};
#endif

/****************************************/
/* Fuser Control Parameter define	*/
/****************************************/


void Set_Debug_Standby_Temp(unsigned char debug_on);
int	Check_error(unsigned char ADCData);
void 	FuserTimer_RegisterTimer(unsigned long timeover);
void 	FuserTimer_Handler(unsigned long arg);
void	FuserManager(void);
unsigned char 	Get_ADCData(void);
unsigned char	Get_SubFanLock(void);


void	FuserState_Manager(void);
void 	TurnOnHeater(void);
void 	TurnOffHeater(void);
int		HeaterLineControl(Uint8 targetTemp, Uint8 currentTemp);

void 	KeepTemperature(Uint8 keepTemp);
void 	DownToStandby(void);
void	DownToPowerSave(void);

int 	RiseToStandby(Uint8 currADC);
int		DecisionStairStepStandby(Uint8 currADC);
int 	DoStairStepStandby(void);

int 	RiseToPrint(Uint8 currADC);
int 	DecisionStairStepPrint(Uint8 currADC);
int		DoStairStepPrint(void);

void 	TestPin_On(void);
void 	TestPin_Off(void);
void 	DownToOff(void) ;
void  	PrintFuser(void);
void	TurnOnMainFan(void);
void	TurnOffMainFan(void);
void	TurnOnSubFan(void);
void	TurnOffSubFan(void);

void 	Heater_DutyChange(Uint8 heater_duty);
#if FAN_TEST
void	MainFan_DutyChange(Uint16 Fan_Duty);
void	SubFan_DutyChange(Uint16 Fan_Duty);
void	DevFan_DutyChange(Uint16 Fan_Duty);
void	MainFan_Off(void);
void	SubFan_Off(void);
void	DevFan_Off(void);
#else
void	MainFan_DutyChange(Uint8 Fan_Duty);
void	SubFan_DutyChange(Uint8 Fan_Duty);
void	DevFan_DutyChange(Uint8 Fan_Duty);
#endif
void 	init_error_flag(void);
void 	init_temperature(void);
void 	init_fuser_state(void);
#if KSF_DBG_SET_DUTY
void	getTestFuserPrintDutyTableValue(void); /* [KSF081402] for dbg only */
void	getTestFuserWarmupHeatingDutyTableValue(void); /* [KSF090601] for dbg only */
void	selectPrintDutyTable(int duty);
#endif
void	error_display(void) ;
void	error_display_forFan(void) ;
Uint8		ChangeADCtoTemp(Uint8 ADCvalue);
Uint8 		GetADCMax(Uint8* arr);
Uint8 		GetADCMin(Uint8* arr);
Uint32 	GetADCAvg(Uint8* arr);
Uint8 FuserControlForColdWakeUp(void);
void FuserControlForStandby(void);
Uint8 FuserControlForPrint(void);
Uint8 FuserControlForLaminate(void);
Uint8 FuserControlForKeepStandby(void);
Uint8 FuserControlForKeepPowerLow(void); /* [KSR032001] add new siyo powerlow */
Uint8 FuserControlForPorBeforeMotorRun(void); /* [KSR070801] */
void SetWaveform_100(void);
void SetWaveform_90(void);
void SetWaveform_80(void);
void SetWaveform_75(void);
void SetWaveform_70(void);
void SetWaveform_60(void);
void SetWaveform_50(void);
void SetWaveform_40(void);
void SetWaveform_33(void);
void SetWaveform_30(void);
void SetWaveform_25(void);
void SetWaveform_20(void);
void SetWaveform_16(void);
void SetWaveform_10(void);
void SetWaveform_0(void);

void Set_Fuser_Parameter(void); /* [KSR031801] set fuser param */
//void Get_Fuser_Test_Parameter(void); /* [KSR031801] set fuser test param */
void DisplayFuserParams(void); /* [KSR031801] display fuser param */
//void Fuser_write(void); /* [KSF050804] */
void DisplayFuserControlParam( void ); /* [KSF050804] */


/*********************************/
/*	FuserTemp_Control Parameter	 */
/*********************************/
#define FUSER_CHECK_TIME	150
#define DUPLEX			1
// #define COUNT_12SEC			1200		//120
// #define COUNT_10_5SEC		1050		//105
// #define	COUNT_10SEC			1000		//100
// #define	COUNT_7SEC			700		//70
// #define	COUNT_5SEC			500		//50
// #define	COUNT_2SEC			200		//20
#define	COUNT_0SEC			0		//0
#define COUNT_3SEC			6
#define COUNT_6SEC			12		//60
#define COUNT_20SEC			40
#define COUNT_30SEC			60
#define COUNT_60SEC			120
#define COUNT_180SEC		360
#define COUNT_300SEC		600
#define COUNT_600SEC		1200
#define TRUE				1
#define FALSE				0
#define SET					1
#define CLEAR				0

// #define FAN_HIGH		50
#if FAN_TEST
#define FAN_MAX			1000
#define FAN_HIGH		500
#define FAN_MID			130
#define	FAN_LOW			10

#define FAN_100			1000
#define FAN_99			990
#define FAN_50			500
#define FAN_13			130
#define	FAN_10			100
#define	FAN_5			50
#define FAN_4			40
#define FAN_2			20
#define	FAN_1			10
#define FAN_0_5			7 // actual value 7, calculated value 5
#define FAN_0			0
#else
#define FAN_MAX			100
#define FAN_HIGH		50
#define FAN_MID			13
#define	FAN_LOW			1

#define FAN_100			100
#define FAN_99			99
#define FAN_50			50
#define FAN_13			13
#define	FAN_10			10
#define	FAN_5			5
#define FAN_4			4
#define FAN_2			2
#define	FAN_1			1
#endif
#define POWER_SAVE_FAN_RUN	240		//	2min

#define DUTY100P_HI		0xff
#define DUTY100P_LO		0xff
#define DUTY80P_HI		0xcc
#define DUTY80P_LO		0xcc
#define	DUTY50P_HI		0x80
#define	DUTY50P_LO		0x00
#define DUTY30P_HI		0x4c
#define DUTY30P_LO		0xcc
#define DUTY15P_HI		0x26
#define DUTY15P_LO		0x66
#define DUTY5P_HI			0x0c
#define DUTY5P_LO			0xcc
#define DUTY1P_HI			0x02
#define DUTY1P_LO			0x8f


#define	ADC_1				0xFE
#define	ADC_2				0xFE
#define	ADC_3				0xFE
#define	ADC_4				0xFE
#define	ADC_5				0xFE
#define	ADC_6				0xFE
#define	ADC_7				0xFE
#define	ADC_8				0xFE
#define	ADC_9				0xFD
#define	ADC_10			0xFD
#define	ADC_11			0xFD
#define	ADC_12			0xFD
#define	ADC_13			0xFD
#define	ADC_14			0xFD
#define	ADC_15			0xFD
#define	ADC_16			0xFD
#define	ADC_17			0xFD
#define	ADC_18			0xFD
#define	ADC_19			0xFD
#define	ADC_20			0xFC
#define	ADC_21			0xFC
#define	ADC_22			0xFC
#define	ADC_23			0xFC
#define	ADC_24			0xFC
#define	ADC_25			0xFC
#define	ADC_26			0xFC
#define	ADC_27			0xFC
#define	ADC_28			0xFB
#define	ADC_29			0xFB
#define	ADC_30			0xFB
#define	ADC_31			0xFB
#define	ADC_32			0xFB
#define	ADC_33			0xFB
#define	ADC_34			0xFB
#define	ADC_35			0xFA
#define	ADC_36			0xFA
#define	ADC_37			0xFA
#define	ADC_38			0xFA
#define	ADC_39			0xFA
#define	ADC_40			0xF9
#define	ADC_41			0xF9
#define	ADC_42			0xF9
#define	ADC_43			0xF9
#define	ADC_44			0xF9
#define	ADC_45			0xF8
#define	ADC_46			0xF8
#define	ADC_47			0xF8
#define	ADC_48			0xF8
#define	ADC_49			0xF7
#define	ADC_50			0xF7
#define	ADC_51			0xF7
#define	ADC_52			0xF7
#define	ADC_53			0xF6
#define	ADC_54			0xF6
#define	ADC_55			0xF6
#define	ADC_56			0xF5
#define	ADC_57			0xF5
#define	ADC_58			0xF5
#define	ADC_59			0xF5
#define	ADC_60			0xF4
#define	ADC_61			0xF4
#define	ADC_62			0xF4
#define	ADC_63			0xF3
#define	ADC_64			0xF3
#define	ADC_65			0xF3
#define	ADC_66			0xF2
#define	ADC_67			0xF2
#define	ADC_68			0xF1
#define	ADC_69			0xF1
#define	ADC_70			0xF1
#define	ADC_71			0xF0
#define	ADC_72			0xF0
#define	ADC_73			0xEF
#define	ADC_74			0xEF
#define	ADC_75			0xEE
#define	ADC_76			0xEE
#define	ADC_77			0xEE
#define	ADC_78			0xED
#define	ADC_79			0xED
#define	ADC_80			0xEC
#define	ADC_81			0xEC
#define	ADC_82			0xEB
#define	ADC_83			0xEB
#define	ADC_84			0xEA
#define	ADC_85			0xEA
#define	ADC_86			0xE9
#define	ADC_87			0xE9
#define	ADC_88			0xE8
#define	ADC_89			0xE7
#define	ADC_90			0xE7
#define	ADC_91			0xE6
#define	ADC_92			0xE6
#define	ADC_93			0xE5
#define	ADC_94			0xE4
#define	ADC_95			0xE4
#define	ADC_96			0xE3
#define	ADC_97			0xE2
#define	ADC_98			0xE2
#define	ADC_99			0xE1
#define	ADC_100			0xE1
#define	ADC_101			0xE0
#define	ADC_102			0xDF
#define	ADC_103			0xDE
#define	ADC_104			0xDE
#define	ADC_105			0xDD
#define	ADC_106			0xDC
#define	ADC_107			0xDC
#define	ADC_108			0xDB
#define	ADC_109			0xDA
#define	ADC_110			0xD9
#define	ADC_111			0xD8
#define	ADC_112			0xD8
#define	ADC_113			0xD7
#define	ADC_114			0xD6
#define	ADC_115			0xD5
#define	ADC_116			0xD4
#define	ADC_117			0xD4
#define	ADC_118			0xD3
#define	ADC_119			0xD2
#define	ADC_120			0xD1
#define	ADC_121			0xD0
#define	ADC_122			0xCF
#define	ADC_123			0xCE
#define	ADC_124			0xCE
#define	ADC_125			0xCD
#define	ADC_126			0xCC
#define	ADC_127			0xCB
#define	ADC_128			0xCA
#define	ADC_129			0xC9
#define	ADC_130			0xC8
#define	ADC_131			0xC7
#define	ADC_132			0xC6
#define	ADC_133			0xC5
#define	ADC_134			0xC4
#define	ADC_135			0xC3
#define	ADC_136			0xC2
#define	ADC_137			0xC1
#define	ADC_138			0xC0
#define	ADC_139			0xBF
#define	ADC_140			0xBE
#define	ADC_141			0xBD
#define	ADC_142			0xBC
#define	ADC_143			0xBB
#define	ADC_144			0xBA
#define	ADC_145			0xB9
#define	ADC_146			0xB8
#define	ADC_147			0xB7
#define	ADC_148			0xB6
#define	ADC_149			0xB5
#define	ADC_150			0xB4
#define	ADC_151			0xB3
#define	ADC_152			0xB2
#define	ADC_153			0xB1
#define	ADC_154			0xB0
#define	ADC_155			0xAF
#define	ADC_156			0xAE
#define	ADC_157			0xAD
#define	ADC_158			0xAC
#define	ADC_159			0xAA
#define	ADC_160			0xA9
#define	ADC_161			0xA8
#define	ADC_162			0xA7
#define	ADC_163			0xA6
#define	ADC_164			0xA5
#define	ADC_165			0xA4
#define	ADC_166			0xA3
#define	ADC_167			0xA2
#define	ADC_168			0xA1
#define	ADC_169			0xA0
#define	ADC_170			0x9F
#define	ADC_171			0x9D
#define	ADC_172			0x9C
#define	ADC_173			0x9B
#define	ADC_174			0x9A
#define	ADC_175			0x99
#define	ADC_176			0x98
#define	ADC_177			0x97
#define	ADC_178			0x96
#define	ADC_179			0x95
#define	ADC_180			0x94
#define	ADC_181			0x92
#define	ADC_182			0x91
#define	ADC_183			0x90
#define	ADC_184			0x8F
#define	ADC_185			0x8E
#define	ADC_186			0x8D
#define	ADC_187			0x8C
#define	ADC_188			0x8B
#define	ADC_189			0x8A
#define	ADC_190			0x89
#define	ADC_191			0x88
#define	ADC_192			0x87
#define	ADC_193			0x86
#define	ADC_194			0x85
#define	ADC_195			0x84
#define	ADC_196			0x82
#define	ADC_197			0x81
#define	ADC_198			0x80
#define	ADC_199			0x7F
#define	ADC_200			0x7E
#define	ADC_201			0x7D
#define	ADC_202			0x7C
#define	ADC_203			0x7B
#define	ADC_204			0x7A
#define	ADC_205			0x79
#define	ADC_206			0x78
#define	ADC_207			0x77
#define	ADC_208			0x76
#define	ADC_209			0x75
#define	ADC_210			0x74
#define	ADC_211			0x73
#define	ADC_212			0x72
#define	ADC_213			0x71
#define	ADC_214			0x70
#define	ADC_215			0x6F
#define	ADC_216			0x6E
#define	ADC_217			0x6E
#define	ADC_218			0x6D
#define	ADC_219			0x6C
#define	ADC_220			0x6B
#define	ADC_221			0x6A
#define	ADC_222			0x69
#define	ADC_223			0x68
#define	ADC_224			0x67
#define	ADC_225			0x66
#define	ADC_226			0x65
#define	ADC_227			0x64
#define	ADC_228			0x64
#define	ADC_229			0x63
#define	ADC_230			0x62
#define	ADC_231			0x61
#define	ADC_232			0x60
#define	ADC_233			0x5F
#define	ADC_234			0x5E
#define	ADC_235			0x5E
#define	ADC_236			0x5D
#define	ADC_237			0x5C
#define	ADC_238			0x5B
#define	ADC_239			0x5A
#define	ADC_240			0x59
#define	ADC_241			0x59
#define	ADC_242			0x58
#define	ADC_243			0x57
#define	ADC_244			0x56
#define	ADC_245			0x55
#define	ADC_246			0x55
#define	ADC_247			0x54
#define	ADC_248			0x53
#define	ADC_249			0x52
#define	ADC_250			0x52

Uint8 CELSIUS2ADC[251] =
{
0xFF,
ADC_1,
ADC_2,
ADC_3,
ADC_4,
ADC_5,
ADC_6,
ADC_7,
ADC_8,
ADC_9,
ADC_10,
ADC_11,
ADC_12,
ADC_13,
ADC_14,
ADC_15,
ADC_16,
ADC_17,
ADC_18,
ADC_19,
ADC_20,
ADC_21,
ADC_22,
ADC_23,
ADC_24,
ADC_25,
ADC_26,
ADC_27,
ADC_28,
ADC_29,
ADC_30,
ADC_31,
ADC_32,
ADC_33,
ADC_34,
ADC_35,
ADC_36,
ADC_37,
ADC_38,
ADC_39,
ADC_40,
ADC_41,
ADC_42,
ADC_43,
ADC_44,
ADC_45,
ADC_46,
ADC_47,
ADC_48,
ADC_49,
ADC_50,
ADC_51,
ADC_52,
ADC_53,
ADC_54,
ADC_55,
ADC_56,
ADC_57,
ADC_58,
ADC_59,
ADC_60,
ADC_61,
ADC_62,
ADC_63,
ADC_64,
ADC_65,
ADC_66,
ADC_67,
ADC_68,
ADC_69,
ADC_70,
ADC_71,
ADC_72,
ADC_73,
ADC_74,
ADC_75,
ADC_76,
ADC_77,
ADC_78,
ADC_79,
ADC_80,
ADC_81,
ADC_82,
ADC_83,
ADC_84,
ADC_85,
ADC_86,
ADC_87,
ADC_88,
ADC_89,
ADC_90,
ADC_91,
ADC_92,
ADC_93,
ADC_94,
ADC_95,
ADC_96,
ADC_97,
ADC_98,
ADC_99,
ADC_100,
ADC_101,
ADC_102,
ADC_103,
ADC_104,
ADC_105,
ADC_106,
ADC_107,
ADC_108,
ADC_109,
ADC_110,
ADC_111,
ADC_112,
ADC_113,
ADC_114,
ADC_115,
ADC_116,
ADC_117,
ADC_118,
ADC_119,
ADC_120,
ADC_121,
ADC_122,
ADC_123,
ADC_124,
ADC_125,
ADC_126,
ADC_127,
ADC_128,
ADC_129,
ADC_130,
ADC_131,
ADC_132,
ADC_133,
ADC_134,
ADC_135,
ADC_136,
ADC_137,
ADC_138,
ADC_139,
ADC_140,
ADC_141,
ADC_142,
ADC_143,
ADC_144,
ADC_145,
ADC_146,
ADC_147,
ADC_148,
ADC_149,
ADC_150,
ADC_151,
ADC_152,
ADC_153,
ADC_154,
ADC_155,
ADC_156,
ADC_157,
ADC_158,
ADC_159,
ADC_160,
ADC_161,
ADC_162,
ADC_163,
ADC_164,
ADC_165,
ADC_166,
ADC_167,
ADC_168,
ADC_169,
ADC_170,
ADC_171,
ADC_172,
ADC_173,
ADC_174,
ADC_175,
ADC_176,
ADC_177,
ADC_178,
ADC_179,
ADC_180,
ADC_181,
ADC_182,
ADC_183,
ADC_184,
ADC_185,
ADC_186,
ADC_187,
ADC_188,
ADC_189,
ADC_190,
ADC_191,
ADC_192,
ADC_193,
ADC_194,
ADC_195,
ADC_196,
ADC_197,
ADC_198,
ADC_199,
ADC_200,
ADC_201,
ADC_202,
ADC_203,
ADC_204,
ADC_205,
ADC_206,
ADC_207,
ADC_208,
ADC_209,
ADC_210,
ADC_211,
ADC_212,
ADC_213,
ADC_214,
ADC_215,
ADC_216,
ADC_217,
ADC_218,
ADC_219,
ADC_220,
ADC_221,
ADC_222,
ADC_223,
ADC_224,
ADC_225,
ADC_226,
ADC_227,
ADC_228,
ADC_229,
ADC_230,
ADC_231,
ADC_232,
ADC_233,
ADC_234,
ADC_235,
ADC_236,
ADC_237,
ADC_238,
ADC_239,
ADC_240,
ADC_241,
ADC_242,
ADC_243,
ADC_244,
ADC_245,
ADC_246,
ADC_247,
ADC_248,
ADC_249,
ADC_250
};


#endif	//Fuser
