/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	ep_control.c					    				
 **	 Revision  :	ver 1.4								    			
 **	 Date	   : 	Dec 21, 2009										
 **	 Author    :	Youngjin Lee		
 +-----------------------------------------------------------------+*/

#include "pioname.h"
#include "farch.h"
#include "em.h"
#include	<stdio.h>

#define DBG_PRFX "engep: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING
#define SLOG_MSG  DBG_MEMLOG_INFO		/* ADD : @@2013.01.08. By CSPark */


void	InitializeCoolingParameter(void);
void	InitializeEPEvaParameter(void);
void	StartEP_Laminating(void);
void	StartEP_POR(void);
void	StartEP_JOG(void);
void	StartEP_PrintFromIdle(void);
void	StartEP_PrintFromStandby(void);

void	StartTransferCleaning(void);
void	StartChargeCleaning(void);
void	StartDeveloperCleaning(void);
void	StartDeveloperNoPrinting(void);
void	StartDeveloperDefault(void);
void	StartDeveloperLaminating(void);
void	StartDeveloperLaminatingReady(void);
void	FinishLaminationProcess(void);
void	FinishIdleProcess(void);

void	EnableChargeBiasSignal(void);
void	EnableTransferBiasSignal(void);
void	DisableChargeBiasSignal(void);
void	DisableTransferBiasSignal(void);

Uint8	ServoSearchUpProcedure(Uint8 step);
void	ServoSearchDownProcedure(Uint8 step);
void	FinishServoSearchProcedure(Uint8 TransferServo_PWM);

void	Set_DeveloperBiasForNoPrint(void);
void	Set_DeveloperBiasForPrinting(Uint8 Darkness, Uint8 TonerSaver);
void 	Set_DeveloperBiasForServo(Uint8 TransferServo_PWM);
void	Set_TransferBiasInterpageGap(Uint8 IPG);
void	Set_TransferBiasInterpageGap_Laminating(void);

/* For PWM */
void	Set_Charge_PWM(Uint8 PWM);
void	Set_Developer_PWM(Uint8 PWM);
void	Set_Transfer_PWM(Uint8 PWM);
extern void Set_Chr_PWM(Uint8 data);
extern void	Set_Dev_PWM(Uint8 PWM);
extern void	Set_Xfr_PWM(Uint8 PWM);
extern void	Xfr_Enable(void);
extern void	Xfr_Disable(void);

extern void DisableDevPWM(void);	
extern void DisableChrPWM(void);

extern unsigned char IsPaperLengthNarrow(unsigned int length);

#if EP_PARAM_TEST
extern Uint8	Param_Charger_PWM;
extern Uint8	Param_Developer_PWM;
extern Uint8	Param_Transfer_PWM;
extern Uint8	Param_Servo_VALUE;

extern Uint8	fParamChargerPWM;
extern Uint8	fParamDeveloperPWM;
extern Uint8	fParamTransferPWM;
extern Uint8	fParamServoVALUE;

extern Uint8 fParamIPGBeforePrinting;
extern Uint8 fParamIPGDuringPrinting;
extern Uint8 Param_IPG_Before_Printing;
extern Uint8 Param_IPG_During_Printing;
#endif

extern Uint8	fPwsToPrint;
extern Uint8 fPaperLengthIsNarrow;
extern int Fuser_Setting_Parameters[NumberOfFuserParameter];
extern int Engine_Parameter[NumberOfEngineParameter];
extern Uint8	fMainMotorJogMode;
extern Uint8 fMotorSpeedMode;

extern int 	cLastDuplexTime;
extern int 	cProcessCounter;
extern Uint8 fDuplexSet;
extern Struct_Fuser_Control Fuser_Control;

void	Set_TransferPrinting(Uint8 MediaSpec, Uint8 Source, Uint8 Destination);

Uint8	Get_Transfer_PWM(void);
Uint8	Get_XfrServoOutState(void);

Uint8	cServoSearch;
Uint8	EnvTempStatus;
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
Uint8	EnvTempStatus_ForDuplexPrint;
Uint8 adjust_servo_value_for_duplex_slow_mode;
#endif

Uint32 	cPageCount5perTrans;

Uint8	DeveloperPWMForPrinting;
Uint8	TransferBiasPWMForServo;
int	TransferPWMServoMinusCore;
// Uint8	TransferBiasPWMForInterpageGap;
Uint8	TransferBiasPWMForPrinting;
int		DevCompensation;

Uint8	SavedChargePWM;
Uint8	SavedDeveloperPWM;
Uint8	SavedTransferPWM;

Uint8	Charge_PWM;
int		Duplex1stNorPageNum;
int		Duplex2ndNorPageNum;

Uint8 cChargerCnt;
Uint8 cDeveloperCnt;
Uint8 cTransferCnt;
int cTempProcessCnt;
Uint8 EP_ChargerTable[Num_ChargerTable] = {135, 160, 185, DefaultCharge_PWM};
Uint8 EP_DeveloperTable[Num_DeveloperTable] = {30, 60, 90, 120, DefaultDeveloper_PWM};
Uint8 EP_TransferTable[Num_TransferTable];

extern engine_hardware_t *eng_hardware;

int ChargerPWMAdjustSlopeTable[8] = { 0, 0, 0, 0, 200, 69, 0, 0};
int ChargerPWMAdjustOffsetTable[8]= { 0, 0, 0, -1000, -21210, -6000, 5000, 5000};
int ChargerPWMAdjustSlopeTable_DrumPages[4] = { -40, -10, -5, -20};
int ChargerPWMAdjustOffsetTable_DrumPages[4] = { 0, -300000, -400000, 200000};

int LSUBeamPowerDutyAdjustSlopeTableForServoValue[6] = {0, 0, 0, 0, 0, 0};
int LSUBeamPowerDutyAdjustOffsetTableForServoValue[6] = {10, 0, 0, 0, 0, 0};
int LSUBeamPowerDutyAdjustSlopeTableForDLPC[2] = {0, 48};
int LSUBeamPowerDutyAdjustOffsetTableForDLPC[2] = {0, -375000};

#if ABBA2
Uint8 	DeveloperDarknessTable[DarknessLevel] = {150, 105, 114, 123, 132, 141, 150, 160, 170, 180, 190};
Uint8 	DeveloperDarknessTableForQuiet[DarknessLevel] = {140, 95, 104, 113, 122, 131, 140, 150, 160, 170, 180};
#else
#if MAC_TYPE_M 	// 40ppm
Uint8 	DeveloperDarknessTable[DarknessLevel] = {140, 95, 104, 113, 122, 131, 140, 150, 160, 170, 180};
#else 			// 45ppm
Uint8 	DeveloperDarknessTable[DarknessLevel] = {153, 108, 117, 126, 135, 144, 153, 163, 173, 183, 193};
#endif
Uint8 	DeveloperDarknessTableForQuiet[DarknessLevel] = {140, 95, 104, 113, 122, 131, 140, 150, 160, 170, 180};
#endif

Uint8	DeveloperPrintPWMTable[DarknessNumber][TonerSaverNumber] = {
					/* Toner Saver Off */	/* Toner Saver On */
/* Normal */		{ DevPWM_Normal,	TonerSaverDevPWM_Normal		},	
/* Lightest */		{ DevPWM_Lightest,	TonerSaverDevPWM_Lightest	},
/* Light */		{ DevPWM_Light,		TonerSaverDevPWM_Light   	},
/* Normal */		{ DevPWM_Normal,	TonerSaverDevPWM_Normal  	},
/* Dark */		{ DevPWM_Dark,		TonerSaverDevPWM_Dark   	},
/* Darkness */		{ DevPWM_Darkness,	TonerSaverDevPWM_Darkness	}
};

Uint8	TransferSpecTable[TransferSpecNumber] = {
	/*	smooth,			normal,			rough,			RESERVED,	*/
/* paper */
  /* light */		MTYPE_SmoothPaper,	MTYPE_SmoothPaper,	MTYPE_Paper,		MTYPE_Paper,
  /* normal */		MTYPE_SmoothPaper,	MTYPE_Paper,		MTYPE_RoughPaper,	MTYPE_Paper,
  /* heavy */		MTYPE_Paper,		MTYPE_RoughPaper,	MTYPE_RoughPaper,	MTYPE_DuplexPaper,
  /* RESERVED */	MTYPE_DuplexPaper,	MTYPE_Paper,		MTYPE_DuplexPaper,	MTYPE_Paper,

/* label */
  /* light */		MTYPE_Paper,		MTYPE_Paper,		MTYPE_Paper,		MTYPE_Paper,
  /* normal */		MTYPE_Label,		MTYPE_Label,		MTYPE_Label,		MTYPE_Label,
  /* heavy */		MTYPE_DuplexPaper,	MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_DuplexPaper,
  /* RESERVED */	MTYPE_Label,		MTYPE_Label,		MTYPE_Label,		MTYPE_Label,

/* envelope */
  /* light */		MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_DuplexPaper,	MTYPE_Envelope,
  /* normal */		MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_Envelope,		MTYPE_Envelope,
  /* heavy */		MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_Envelope,		MTYPE_Envelope,
  /* RESERVED */	MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_Envelope,		MTYPE_Envelope,

/* card stock */
  /* light */		MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,
  /* normal */		MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,
  /* heavy */		MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,
  /* RESERVED */	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_DuplexPaper,	MTYPE_CardStock,

/* transparency */
  /* light */		MTYPE_Transparency, 	MTYPE_Transparency, 	MTYPE_Transparency,	MTYPE_Transparency,
  /* normal */		MTYPE_Transparency, 	MTYPE_Transparency, 	MTYPE_Transparency,	MTYPE_Transparency,
  /* heavy */		MTYPE_Transparency, 	MTYPE_Transparency, 	MTYPE_Transparency,	MTYPE_Transparency,
  /* RESERVED */	MTYPE_Transparency, 	MTYPE_Transparency, 	MTYPE_Transparency,	MTYPE_Transparency,

/* Vinyl label */
  /* light */		MTYPE_Paper,		MTYPE_Paper,		MTYPE_Paper,		MTYPE_Paper,
  /* normal */		MTYPE_Label,		MTYPE_Label,		MTYPE_Label,		MTYPE_Label,
  /* heavy */		MTYPE_DuplexPaper,	MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_DuplexPaper,
  /* RESERVED */	MTYPE_Label,		MTYPE_Label,		MTYPE_Label,		MTYPE_Label,
  
/* Post card */
  /* light */		MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,
  /* normal */		MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,
  /* heavy */		MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,
  /* RESERVED */	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_DuplexPaper,	MTYPE_CardStock,
 
/* Recycled */
  /* light */		MTYPE_SmoothPaper,	MTYPE_SmoothPaper,	MTYPE_Paper,		MTYPE_Paper,
  /* normal */		MTYPE_SmoothPaper,	MTYPE_Paper,		MTYPE_RoughPaper,	MTYPE_Paper,
  /* heavy */		MTYPE_Paper,		MTYPE_RoughPaper,	MTYPE_RoughPaper,	MTYPE_DuplexPaper,
  /* RESERVED */	MTYPE_DuplexPaper,	MTYPE_Paper,		MTYPE_DuplexPaper,	MTYPE_Paper,
  
/* duplex paper */
  /* light */		MTYPE_Label,		MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_DuplexPaper,
  /* normal */		MTYPE_Label,		MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_DuplexPaper,
  /* heavy */		MTYPE_Label,		MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_DuplexPaper,
  /* RESERVED */	MTYPE_Label,		MTYPE_DuplexPaper,	MTYPE_Envelope,		MTYPE_DuplexPaper,

/* duplex card stock */
  /* light */		MTYPE_DuplexPaper,	MTYPE_CardStock,	MTYPE_Label,		MTYPE_CardStock,
  /* normal */		MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,
  /* heavy */		MTYPE_DuplexPaper,	MTYPE_CardStock,	MTYPE_Transparency,	MTYPE_CardStock,
  /* RESERVED */	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock,	MTYPE_CardStock
  

} ;

Uint8	TransferSpecDuplexTable[] = {
		/*	smooth,		normal,			rough,			RESERVED,	*/
/* duplex paper */
  /* light */		MTYPE_DuplexSmoothPaper,	MTYPE_DuplexSmoothPaper,	MTYPE_Envelope,			MTYPE_DuplexPaper,
  /* normal */		MTYPE_DuplexSmoothPaper,	MTYPE_DuplexPaper,			MTYPE_DuplexRoughPaper,	MTYPE_DuplexPaper,
  /* heavy */		MTYPE_DuplexPaper,			MTYPE_DuplexPaper,			MTYPE_DuplexRoughPaper,	MTYPE_DuplexPaper,
  /* RESERVED */	MTYPE_Label,				MTYPE_DuplexPaper,			MTYPE_Envelope,			MTYPE_DuplexPaper
};

#if ABBA2
Uint8	TransferSlopeTable[MediaNumber][TransferPWMRange] = {		// 36ppm ABBA2
// PWM <= 2 , 	 2 < PWM <= 24, 24 < PWM <= 54,  PWM > 54 
{60, 60, 48, 12},			// Paper 
{70, 83, 45, 15},			// Label 
{85, 97, 30,  7},			// Envelope 
{65, 68, 53, 10},			// Card Stock 
{25, 20, 22, 18},			// Transparency
{29, 43, 35, 16},			// Vinyl Label 
{30, 65, 32, 20},			// Post Card
{60, 60, 48, 12},			// Recycled
{32, 64, 35, 16},			// RESERVED 
{60, 66, 20, 18},			// Duplex Paper 
{65, 70, 45, 25},			// Rough Paper 
{65, 70, 30, 25},			// Duplex Rough Paper 
{65, 70, 48, 13},			// Smooth Paper 
{63, 64, 23, 20},			// Duplex Smooth Paper
};

Uint8	TransferOffsetTable[MediaNumber][TransferPWMRange] = {		// 36ppm ABBA2
// PWM <= 2 , 	 2 < PWM <= 24, 24 < PWM <= 54,  PWM > 54 
{ 95,  94, 102, 163},			// Paper 
{ 83,  80, 108, 159},			// Label 
{ 92,  89, 139, 178},			// Envelope 
{ 82,  81,  92, 165},			// Card Stock 
{134, 135, 134, 141},			// Transparency
{ 90,  87, 115, 166},			// Vinyl Label 
{ 89,  88,  89, 172},			// Post Card 
{ 95,  94, 102, 163},			// Recycled
{ 69,  61, 107, 112},			// RESERVED 
{105, 104, 138, 142},			// Duplex Paper 
{ 82,  80,  98, 132},			// Rough Paper 
{ 97,  95, 125, 134},			// Duplex Rough Paper 
{ 82,  83,  99, 159},			// Smooth Paper 
{103, 102, 133, 139},			// Duplex Smooth Paper
};

Uint8	TransferSlopeTableForQUIET[MediaNumber][TransferPWMRange] = {		// 25ppm ABBA2
// PWM <= 2 , 	 2 < PWM <= 24, 24 < PWM <= 54,  PWM > 54 
{51, 53, 20, 24},			// Paper 
{29, 43, 35, 16},			// Label 
{55, 61, 46,  3},			// Envelope 
{30, 65, 32, 20},			// Card Stock 
{32, 45, 21,  6},			// Transparency
{29, 43, 35, 16},			// Vinyl Label 
{30, 65, 32, 20},			// Post Card
{51, 53, 20, 24},			// Recycled
{32, 64, 35, 16},			// RESERVED 
{51, 45, 29, 13},			// Duplex Paper 
{45, 67, 29, 16},			// Rough Paper 
{51, 45, 29, 13},			// Duplex Rough Paper 
{51, 53, 32, 19},			// Smooth Paper 
{51, 45, 29, 13},			// Duplex Smooth Paper
};

Uint8	TransferOffsetTableForQUIET[MediaNumber][TransferPWMRange] = {		// 25ppm ABBA2
// PWM <= 2 , 	 2 < PWM <= 24, 24 < PWM <= 54,  PWM > 54 
{ 73,  72, 113, 107},			// Paper 
{ 90,  90, 120, 125},			// Label 
{ 92,  92, 127, 171},			// Envelope 
{ 67,  65, 104, 125},			// Card Stock 
{103, 102, 146, 143},			// Transparency
{ 90,  87, 115, 166},			// Vinyl Label 
{ 89,  88,  89, 172},			// Post Card 
{ 73,  72, 113, 107},			// Recycled
{ 69,  61, 107, 112},			// RESERVED 
{ 95,  95, 134, 131},			// Duplex Paper 
{ 76,  74, 129, 122},			// Rough Paper 
{100, 101, 139, 136},			// Duplex Rough Paper 
{ 67,  67, 107, 101},			// Smooth Paper 
{ 89,  90, 128, 125},			// Duplex Smooth Paper
};
#else
#if MAC_TYPE_M
Uint8	TransferSlopeTable[MediaNumber][TransferPWMRange] = {		// 40ppm
// PWM <= 10 , 	 10 < PWM <= 24, 24 < PWM <= 54,  PWM > 54 
{39, 79, 42, 17},			// Paper 
{70, 83, 45, 15},			// Label 
{85, 97, 30,  7},			// Envelope 
{65, 68, 53, 10},			// Card Stock 
{25, 20, 22, 18},			// Transparency
{29, 43, 35, 16},			// Vinyl Label 
{30, 65, 32, 20},			// Post Card
{39, 79, 42, 17},			// Recycled
{32, 64, 35, 16},			// RESERVED 
{71, 67, 27, 17},			// Duplex Paper 
{39, 79, 42, 17},			// Rough Paper 
{71, 67, 27, 17},			// Duplex Rough Paper 
{39, 79, 42, 17},			// Smooth Paper 
{71, 67, 27, 17},			// Duplex Smooth Paper
};

Uint8	TransferOffsetTable[MediaNumber][TransferPWMRange] = {		// 40ppm
// PWM <= 7 , 	 7 < PWM <= 17, 17 < PWM <= 29,  PWM > 29 
{ 84,  70,  97, 139},			// Paper 
{ 90,  87, 115, 166},			// Label 
{ 99,  96, 146, 185},			// Envelope 
{ 89,  88,  89, 172},			// Card Stock 
{141, 142, 141, 148},			// Transparency
{ 90,  87, 115, 166},			// Vinyl Label 
{ 89,  88,  89, 172},			// Post Card 
{ 84,  70,  97, 139},			// Recycled
{ 73,  68, 114, 119},			// RESERVED 
{ 92,  92, 122, 139},			// Duplex Paper 
{ 89,  78, 107, 151},			// Rough Paper 
{ 98,  98, 128, 145},			// Duplex Rough Paper 
{ 79,  62,  87, 127},			// Smooth Paper 
{ 87,  87, 117, 134},			// Duplex Smooth Paper
};
#else
Uint8	TransferSlopeTable[MediaNumber][TransferPWMRange] = {		// 45ppm
// PWM <= 7 , 	 7 < PWM <= 17, 17 < PWM <= 29,  PWM > 29 
{19, 78, 61, 27},			// Paper 
{70, 83, 45, 15},			// Label 
{85, 97, 30,  7},			// Envelope 
{65, 68, 53, 10},			// Card Stock 
{25, 20, 22, 18},			// Transparency
{29, 43, 35, 16},			// Vinyl Label 
{30, 65, 32, 20},			// Post Card
{19, 78, 61, 27},			// Recycled
{32, 64, 35, 16},			// RESERVED 
{19,114, 29, 25},			// Duplex Paper 
{19, 78, 61, 27},			// Rough Paper 
{19,114, 29, 25},			// Duplex Rough Paper 
{19, 78, 61, 27},			// Smooth Paper 
{19,114, 29, 25},			// Duplex Smooth Paper
};

Uint8	TransferOffsetTable[MediaNumber][TransferPWMRange] = {		// 45ppm
// PWM <= 7 , 	 7 < PWM <= 17, 17 < PWM <= 29,  PWM > 29 
{ 88,  73,  81, 112},			// Paper 
{ 90,  87, 115, 166},			// Label 
{ 99,  96, 146, 185},			// Envelope 
{ 89,  88,  89, 172},			// Card Stock 
{141, 142, 141, 148},			// Transparency
{ 90,  87, 115, 166},			// Vinyl Label 
{ 89,  88,  89, 172},			// Post Card 
{ 88,  73,  81, 112},			// Recycled
{ 73,  68, 114, 119},			// RESERVED 
{ 89,  66, 111, 116},			// Duplex Paper 
{ 96,  81,  91, 124},			// Rough Paper 
{ 95,  72, 117, 122},			// Duplex Rough Paper 
{ 80,  65,  71, 100},			// Smooth Paper 
{ 84,  61, 106, 111},			// Duplex Smooth Paper
};
#endif

Uint8	TransferSlopeTableForQUIET[MediaNumber][TransferPWMRange] = {		// 25ppm
// PWM <= 10 , 	 10 < PWM <= 34, 34 < PWM <= 54,  PWM > 54 
{52, 58, 22, 24},			// Paper 
{70, 83, 45, 15},			// Label 
{52, 97, 47,  7},			// Envelope 
{65, 68, 53, 10},			// Card Stock 
{25, 20, 22, 18},			// Transparency
{29, 43, 35, 16},			// Vinyl Label 
{30, 65, 32, 20},			// Post Card
{52, 58, 22, 24},			// Recycled
{32, 64, 35, 16},			// RESERVED 
{100, 22, 14, 13},			// Duplex Paper 
{52, 58, 22, 24},			// Rough Paper 
{100, 22, 14, 13},			// Duplex Rough Paper 
{52, 58, 22, 24},			// Smooth Paper 
{100, 22, 14, 13},			// Duplex Smooth Paper
};

Uint8	TransferOffsetTableForQUIET[MediaNumber][TransferPWMRange] = {		// 25ppm
// PWM <= 10 , 	 10 < PWM <= 34, 34 < PWM <= 54,  PWM > 54 
{ 75,  72, 111, 107},			// Paper 
{ 83,  80, 108, 159},			// Label 
{ 75,  58, 110, 178},			// Envelope 
{ 82,  81,  92, 165},			// Card Stock 
{134, 135, 134, 141},			// Transparency
{ 90,  87, 115, 166},			// Vinyl Label 
{ 89,  88,  89, 172},			// Post Card 
{ 75,  72, 111, 107},			// Recycled
{ 69,  61, 107, 112},			// RESERVED 
{ 97, 122, 130, 131},			// Duplex Paper 
{ 83,  80, 119, 115},			// Rough Paper 
{103, 128, 136, 137},			// Duplex Rough Paper 
{ 67,  64, 103,  99},			// Smooth Paper 
{ 92, 117, 125, 126},			// Duplex Smooth Paper
};
#endif

#if ABBA2
int	DeveloperSlopeTableForServo[7] 	= {	0,0,6000,2143,3794,0,0};	
int	DeveloperOffsetTableForServo[7] = {	0,0,-560000,-166000,-360000,200000,200000};
int	DeveloperSlopeTableForServoForQuiet[7] 	= {	0,0,6000,7860,2420,0,0};	
int	DeveloperOffsetTableForServoForQuiet[7] = {	0,0,-560000,-743000,-100000,280000,0};
#else
#if MAC_TYPE_M 	// 40ppm
int	DeveloperSlopeTableForServo[7] 	= {	0,0,600,786,587,0,0};
int	DeveloperOffsetTableForServo[7] = {	5000,0,-56000,-74300,-51000,35000,0};
#else 			// 45ppm
int	DeveloperSlopeTableForServo[7] 	= {	0,0,600,786,242,0,0};
int	DeveloperOffsetTableForServo[7] = {	0,0,-56000,-74300,-10000,28000,28000};
#endif
int	DeveloperSlopeTableForServoForQuiet[7] 	= {	0,0,600,786,587,0,0};
int	DeveloperOffsetTableForServoForQuiet[7] = {	5000,0,-56000,-74300,-51000,35000,0};
#endif

#if ABBA2
int	DeveloperSlopeTableForPageCnt[5] = {-200,-16,-15,0,0};
int	DeveloperOffsetTableForPageCnt[5] = {100000,8016,5015,-100000,-100000};
int	DeveloperSlopeTableForPageCntForQuiet[4] = {-200,-16,-15,0};
int	DeveloperOffsetTableForPageCntForQuiet[4] = {100000,8016,5015,-100000};
#else
#if MAC_TYPE_M 	// 40ppm
int	DeveloperSlopeTableForPageCnt[4] = {0,-16,-15,0};
int	DeveloperOffsetTableForPageCnt[4] = {0,8016,5015,-100000};
#else 			// 45ppm
int	DeveloperSlopeTableForPageCnt[5] = {-100,-16,-15,-17,0};
int	DeveloperOffsetTableForPageCnt[5] = {50100,8016,5015,11000,-150000};
#endif
int	DeveloperSlopeTableForPageCntForQuiet[4] = {0,-16,-15,0};
int	DeveloperOffsetTableForPageCntForQuiet[4] = {0,8016,5015,-100000};
#endif

int Duplex1stNorPageNumTable[NumberOfTempState];
int Duplex2ndNorPageNumTable[NumberOfTempState];

void	InitializeCoolingParameter(void)
{
	Duplex1stNorPageNumTable[0] = Engine_Parameter[3];
	Duplex1stNorPageNumTable[1] = Engine_Parameter[4];
	Duplex1stNorPageNumTable[2] = Engine_Parameter[5];
	Duplex1stNorPageNumTable[3] = Engine_Parameter[6];
	
	Duplex2ndNorPageNumTable[0] = Engine_Parameter[9];
	Duplex2ndNorPageNumTable[1] = Engine_Parameter[10];
	Duplex2ndNorPageNumTable[2] = Engine_Parameter[11];
	Duplex2ndNorPageNumTable[3] = Engine_Parameter[12];
}
//#endif

void	InitializeEPEvaParameter(void)
{
	Charge_PWM = DefaultCharge_PWM;
	DeveloperPWMForPrinting = DefaultDeveloper_PWM;
}

void	StartEP_Laminating(void)
{
	Set_Charge_PWM(DefaultCharge_PWM_Laminating);
	Set_Transfer_PWM(TransferBiasPWMForCleaning);
	Set_Developer_PWM(DeveloperBiasPWMForNoPrint);
	EnableTransferBiasSignal();
}

void	StartEP_POR(void)
	/* Set the each bias at start of POR sequence */
{
	/* Turn On Charge Bias */
	EnableChargeBiasSignal();
	Set_Transfer_PWM(TransferBiasPWMForPOR);
	Set_Developer_PWM(DeveloperBiasPWMForNoPrint);
	/* Turn On Transfer Bias */
	EnableTransferBiasSignal();
}

void	StartEP_JOG(void)
{
	/* Turn On Charge Bias */
	Set_Charge_PWM(DefaultCharge_PWM);
	Set_Transfer_PWM(TransferBiasPWMForPOR);
	Set_Developer_PWM(DeveloperBiasPWMForNoPrint);
	/* Turn On Transfer Bias */
	EnableTransferBiasSignal();
}

void	StartEP_PrintFromIdle(void)
	/* Set the each bias at start of Print from Standby sequence */
{
	/* Charge Set */
	EnableChargeBiasSignal();
	/* Developer Set */
	Set_Developer_PWM(DeveloperBiasPWMForNoPrint);
	/* Transfer Set */
	Set_Transfer_PWM(TransferBiasPWMForCleaning);
	/* Turn On Transfer Bias 
	EnableTransferBiasSignal();*/
}

void	StartEP_PrintFromStandby(void)
	/* Set the each bias at start of Print from Standby sequence */
{
	/* Charge Set */
	EnableChargeBiasSignal();
	/* Developer Set */
	Set_Developer_PWM(DeveloperBiasPWMForNoPrint);
	/* Transfer Set */
	Set_Transfer_PWM(TransferBiasPWMForCleaning);
	/* Turn On Transfer Bias */
	EnableTransferBiasSignal();
}

void	StartTransferCleaning(void)		//Done 	//21Apr09 yjlee 
{
	Set_Transfer_PWM(TransferBiasPWMForCleaning);
}


void 	StartChargeCleaning(void)	//Done 	//21Apr09 yjlee 
{
	DisableChargeBiasSignal();
	Set_Developer_PWM(DeveloperBiasPWMForCleaning);	
}

void	StartDeveloperCleaning(void)	//Done 	//21Apr09 yjlee 
{
	Set_Developer_PWM(DeveloperBiasPWMForCleaning);
}

void	StartDeveloperNoPrinting(void)	//Done 	//21Apr09 yjlee 
{
	Set_Developer_PWM(DeveloperBiasPWMForNoPrint);
}

void	StartDeveloperDefault(void)		//Done 	//21Apr09 yjlee 
{
	cDeveloperCnt = 0;
	//MSG("(eng)Set_Developer_PWM = %d (%d)\n", EP_DeveloperTable[cDeveloperCnt], cProcessCounter);
//	Set_Developer_PWM(DefaultDeveloper_PWM);
	Set_Developer_PWM(EP_DeveloperTable[cDeveloperCnt++]);
}

void	StartDeveloperLaminating(void)		//Done 	//21Apr09 yjlee 
{
	Set_Developer_PWM(DeveloperBiasPWMForLaminating);
}

void	StartDeveloperLaminatingReady(void)		//Done 	//21Apr09 yjlee 
{
	Set_Developer_PWM(DeveloperBiasPWMForLaminatingReady);
}

void 	FinishLaminationProcess(void)
{
	Set_Developer_PWM(DeveloperBiasPWMForStop);
	DisableChargeBiasSignal();
	Set_Transfer_PWM(1);
	DisableTransferBiasSignal();
}

void	FinishIdleProcess(void)			//Done 	//21Apr09 yjlee 
{
	Set_Developer_PWM(DeveloperBiasPWMForStop);
	//DisableDevPWM();
	DisableTransferBiasSignal();
}

void	EnableChargeBiasSignal(void)	//Done 	//21Apr09 yjlee 
{
//	Charge_PWM = DefaultCharge_PWM;	
	cChargerCnt = 0;
	Charge_PWM = EP_ChargerTable[cChargerCnt++];
	
	cTempProcessCnt = cProcessCounter;
	
#if EP_PARAM_TEST
	if (fParamChargerPWM==TRUE)
	{
		Charge_PWM = Param_Charger_PWM;
	}
#endif
	if(fMainMotorJogMode == SET)
	{
		Set_Charge_PWM(DefaultCharge_PWM);
	}
	else
	{
		Set_Charge_PWM(Charge_PWM);
	}
//	MSG("(eng)Set_Charger_PWM () = %d (%d)\n", Charge_PWM, cProcessCounter);
}

void	EnableTransferBiasSignal(void)	//Done 	//21Apr09 yjlee 
{
	Xfr_Enable();
}
void	DisableChargeBiasSignal(void)	//Done 	//21Apr09 yjlee 
{
	Set_Charge_PWM(CLEAR);
	//DisableChrPWM();
}

void	DisableTransferBiasSignal(void)	//Done 	//21Apr09 yjlee 
{
	Xfr_Disable();
}

void	DisableDeveloperBiasSignal(void)	//Done 	//21Apr09 yjlee 
{
	Set_Developer_PWM(DeveloperBiasPWMForStop);
	//DisableDevPWM();
}

void	Set_DeveloperBiasForPrinting(Uint8 Darkness, Uint8 TonerSaver)
{
	unsigned int Dev_PWM, PageCount_Range;	
	
	if(fMotorSpeedMode == MTR_MODE_NORMAL)
	{
		Dev_PWM = DeveloperDarknessTable[Darkness];
	}
	else
	{
		Dev_PWM = DeveloperDarknessTableForQuiet[Darkness];
	}
	// MSG("(Eng)## Dev_PWM = %d##\n", Dev_PWM);

#if MAC_TYPE_MH
	Dev_PWM += DevCompensation;
	// MSG("(Eng)## Dev_PWM2 = %d##\n", Dev_PWM);
	
	if(fMotorSpeedMode == MTR_MODE_NORMAL)
	{
#if MAC_TYPE_M
		if(cPageCount5perTrans >= 7002)	PageCount_Range = 3;
		else if(cPageCount5perTrans >= 5001)	PageCount_Range = 2;
		else if(cPageCount5perTrans >= 501)		PageCount_Range = 1;
		else 									PageCount_Range = 0;
#else
		if(cPageCount5perTrans >= 10001)		PageCount_Range = 4;
		else if(cPageCount5perTrans >= 7002)	PageCount_Range = 3;
		else if(cPageCount5perTrans >= 5001)	PageCount_Range = 2;
		else if(cPageCount5perTrans >= 501)		PageCount_Range = 1;
		else 									PageCount_Range = 0;
#endif	
		Dev_PWM += ((DeveloperSlopeTableForPageCnt[PageCount_Range] * (int)(cPageCount5perTrans)) + DeveloperOffsetTableForPageCnt[PageCount_Range])/10000;
		MSG("(Eng)## Developer_PWM_Adjust(forNormal) : cPageCount5perTrans = %d, PageCount_Range = %d , Dev_PWM = %d##\n", cPageCount5perTrans, PageCount_Range, Dev_PWM);
	}
	else
	{
		if(cPageCount5perTrans >= 7002)	PageCount_Range = 3;
		else if(cPageCount5perTrans >= 5001)	PageCount_Range = 2;
		else if(cPageCount5perTrans >= 501)		PageCount_Range = 1;
		else 									PageCount_Range = 0;
	
		Dev_PWM += ((DeveloperSlopeTableForPageCntForQuiet[PageCount_Range] * (int)(cPageCount5perTrans)) + DeveloperOffsetTableForPageCntForQuiet[PageCount_Range])/10000;
		MSG("(Eng)## Developer_PWM_Adjust(forQuiet) : cPageCount5perTrans = %d, PageCount_Range = %d , Dev_PWM = %d##\n", cPageCount5perTrans, PageCount_Range, Dev_PWM);
	}
	
#else
	if(TransferBiasPWMForServo >=131)
	{
		Dev_PWM += 15;
	}
	else if(TransferBiasPWMForServo >=116)
	{
		Dev_PWM += 10;
	}
	else if(TransferBiasPWMForServo >=106)
	{
		Dev_PWM += 5;
	}
	else if(TransferBiasPWMForServo <=65)
	{
		Dev_PWM -= 8;
#if	ENG_TRACE		
		MSG("(Eng)## Developer_PWM_Adjust : TransferBiasPWMForServo = %d, Dev_PWM = %d ##\n",TransferBiasPWMForServo,Dev_PWM);
#endif
	}
	
	if((cPageCount5perTrans >= 0) && (cPageCount5perTrans <= 500))
	{
		Dev_PWM += 3;
	}
	else if((cPageCount5perTrans > 4000) && (cPageCount5perTrans <= 5000))
	{
		Dev_PWM -= 3;
	}
	else if((cPageCount5perTrans > 5000) && (cPageCount5perTrans <= 6000))
	{
		Dev_PWM -= 5;
	}
	else if((cPageCount5perTrans > 6000) && (cPageCount5perTrans <= 7000))
	{
		Dev_PWM -= 6;
	}
	
#endif
	
#if EP_PARAM_TEST
	if(fParamDeveloperPWM == TRUE)
	{
		Dev_PWM = Param_Developer_PWM;
	}
#endif
	
	if(Dev_PWM > 255) 
	{
		Dev_PWM = 255;
	}

	DeveloperPWMForPrinting = Dev_PWM;
	Set_Developer_PWM(DeveloperPWMForPrinting);
}

void Set_DeveloperBiasForServo(Uint8 TransferServo_PWM)
{
	int Servo_Range;
#if MAC_TYPE_MH
	if(fMotorSpeedMode == MTR_MODE_NORMAL)
	{
#if MAC_TYPE_M
		if(TransferServo_PWM > 254)				Servo_Range = 6;
		else if(TransferServo_PWM >=146) 		Servo_Range = 5;
		else if(TransferServo_PWM >=116)		Servo_Range = 4;
		else if(TransferServo_PWM >=101)		Servo_Range = 3;
		else if(TransferServo_PWM >=95)			Servo_Range = 2;
		else if(TransferServo_PWM >=60)			Servo_Range = 1;
		else 									Servo_Range = 0;
#else
		if(TransferServo_PWM > 254)				Servo_Range = 6;
		else if(TransferServo_PWM >=146) 		Servo_Range = 5;
		else if(TransferServo_PWM >=116)		Servo_Range = 4;
		else if(TransferServo_PWM >=101)		Servo_Range = 3;
		else if(TransferServo_PWM >=95)			Servo_Range = 2;
		else if(TransferServo_PWM >=60)			Servo_Range = 1;
		else 									Servo_Range = 0;
#endif		
#if ABBA2
		DevCompensation = ((DeveloperSlopeTableForServo[Servo_Range] * (int)(TransferServo_PWM)) + DeveloperOffsetTableForServo[Servo_Range])/10000;
#else
		DevCompensation = ((DeveloperSlopeTableForServo[Servo_Range] * (int)(TransferServo_PWM)) + DeveloperOffsetTableForServo[Servo_Range])/1000;
#endif
		MSG("(Eng)## Developer_PWM_Adjust(forNormal) : Servo_Range = %d , DevCompensation = %d##\n",Servo_Range, DevCompensation);
	}
	else
	{
		if(TransferServo_PWM > 254)				Servo_Range = 6;
		else if(TransferServo_PWM >=146) 		Servo_Range = 5;
		else if(TransferServo_PWM >=116)		Servo_Range = 4;
		else if(TransferServo_PWM >=101)		Servo_Range = 3;
		else if(TransferServo_PWM >=95)			Servo_Range = 2;
		else if(TransferServo_PWM >=60)			Servo_Range = 1;
		else 									Servo_Range = 0;
#if ABBA2
		DevCompensation = ((DeveloperSlopeTableForServoForQuiet[Servo_Range] * (int)(TransferServo_PWM)) + DeveloperOffsetTableForServoForQuiet[Servo_Range])/10000;
#else
		DevCompensation = ((DeveloperSlopeTableForServoForQuiet[Servo_Range] * (int)(TransferServo_PWM)) + DeveloperOffsetTableForServoForQuiet[Servo_Range])/1000;
#endif
		MSG("(Eng)## Developer_PWM_Adjust(forQuiet) : Servo_Range = %d , DevCompensation = %d##\n",Servo_Range, DevCompensation);
	}
#endif
}


void	Set_DeveloperBiasForNoPrint(void)
{
	Set_Developer_PWM(DeveloperBiasPWMForNoPrint);
}

void	StartServoSearch(void)
	/* Set voltage of starting of transfer bias before Servo Search */
{
	cServoSearch = NONE;
	TransferBiasPWMForServo = NONE;
	TransferPWMServoMinusCore = NONE;
	// TransferBiasPWMForInterpageGap = NONE;
	TransferBiasPWMForPrinting = NONE;

	Set_Transfer_PWM(45);		// Servo Start PWM = 55(TransferCORE_PWM) - 10
}

Uint8	ServoSearchUpProcedure(Uint8 step)
	/* Set the value of PEWM for Servo Search */
{
	Uint8 Xfr_PWM;
	
	Xfr_PWM = Get_Transfer_PWM();
//	MSG("(eng)ServoSearchUpProcedure : step = %d, Xfr_PWM = %d, tx_sensing = %d, cServoSearch = %d (%d)\n", step, Xfr_PWM, Get_XfrServoOutState(), cServoSearch, cProcessCounter);
	if(Get_XfrServoOutState() == HIGH) 
	{
		if(step > ONE) 
			return TRUE;
		
		cServoSearch++;
		if(cServoSearch > ONE) 
		{
//			StartDeveloperDefault();
			FinishServoSearchProcedure(Xfr_PWM);
			return TRUE;
		}
	}
	else 
	{
		if(Xfr_PWM >= LIMTE_FOR_TRANSFER_SERVO_SEARCH_PWM) 
		{
			if(step > ONE) 
				return TRUE;
			else 
			{
//				StartDeveloperDefault();
				FinishServoSearchProcedure(Xfr_PWM);
				return TRUE;
			}
		}
		if((Xfr_PWM + step) > LIMTE_FOR_TRANSFER_SERVO_SEARCH_PWM) 
		{
			Set_Transfer_PWM(LIMTE_FOR_TRANSFER_SERVO_SEARCH_PWM);
		}
		else 
		{
			Set_Transfer_PWM(Xfr_PWM + step); 
		}
		cServoSearch = NONE;
	}
	return FALSE;
}

void	ServoSearchDownProcedure(Uint8 step)
{
	Uint8 Xfr_PWM;
	
	Xfr_PWM = Get_Transfer_PWM();
	
	if(Xfr_PWM < step) 
	{
		Set_Transfer_PWM(PWM_Minimum);
	}
	else 
	{
		Set_Transfer_PWM(Xfr_PWM - step);
	}
	cServoSearch = CLEAR;
//	MSG("(eng) servo down Servo %d > %d, cServoSearch = %d(%d)\n", Xfr_PWM, Xfr_PWM-step, cServoSearch, cProcessCounter);
}

void	FinishServoSearchProcedure(Uint8 TransferServo_PWM)
{
	TransferBiasPWMForServo = TransferServo_PWM;
#if EP_PARAM_TEST
	if(fParamServoVALUE == TRUE)
	{
		TransferBiasPWMForServo = Param_Servo_VALUE;
	}
#endif
	
#if	ENG_TRACE
	MSG("(Eng)==TransferServo_PWM = %d ==\n",TransferBiasPWMForServo);
#endif

//	if(TransferBiasPWMForServo < TransferCORE_PWM)
//	{
//		TransferPWMServoMinusCore = NONE;
//	}
//	else
//	{
		TransferPWMServoMinusCore = (int)(TransferBiasPWMForServo - TransferCORE_PWM);
#if !(ABBA2)
		if(TransferPWMServoMinusCore > 100)
		{
			TransferPWMServoMinusCore = 100;
		}
#endif
//	}
	
	Set_DeveloperBiasForServo(TransferBiasPWMForServo);

	if(TransferBiasPWMForServo < Engine_Parameter[0])
	{
		if(fDuplexSet == CLEAR)
		{
			EnvTempStatus = EnvTemp_H;
			Fuser_Control.fLowTemperature = CLEAR;
			Fuser_Control.fDevFanOff_ForLowServo = CLEAR;
			// tempOffset = (cProcessCounter - cLastDuplexTime ) * 8 / 60000;
		}
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
		EnvTempStatus_ForDuplexPrint = EnvTemp_H;
#endif
	}
	else if(TransferBiasPWMForServo < Engine_Parameter[1])
	{
		if(fDuplexSet == CLEAR)
		{
			EnvTempStatus = EnvTemp_HM;
			Fuser_Control.fLowTemperature = CLEAR;
			Fuser_Control.fDevFanOff_ForLowServo = CLEAR;
			// tempOffset = (cProcessCounter - cLastDuplexTime ) * 8 / 60000;
		}
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
		EnvTempStatus_ForDuplexPrint = EnvTemp_HM;
#endif
	}
	else if(TransferBiasPWMForServo < Engine_Parameter[2])
	{
		if(fDuplexSet == CLEAR)
		{
			EnvTempStatus = EnvTemp_M;
			Fuser_Control.fLowTemperature = CLEAR;
			Fuser_Control.fDevFanOff_ForLowServo = CLEAR;
			// tempOffset = (cProcessCounter - cLastDuplexTime ) * 12 / 60000;
		}
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
		EnvTempStatus_ForDuplexPrint = EnvTemp_M;
#endif
	}
#if ABBA2
	else
	{
		if(fDuplexSet == CLEAR)
		{
			EnvTempStatus = EnvTemp_L;
			Fuser_Control.fLowTemperature = SET;
			Fuser_Control.fDevFanOff_ForLowServo = SET;
		}
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
		EnvTempStatus_ForDuplexPrint = EnvTemp_L;
#endif
	}
#else
	else if(TransferBiasPWMForServo < Engine_Parameter[18])
	{
		if(fDuplexSet == CLEAR)
		{
			EnvTempStatus = EnvTemp_ML;
			Fuser_Control.fLowTemperature = SET;
			Fuser_Control.fDevFanOff_ForLowServo = SET;
		}
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
		EnvTempStatus_ForDuplexPrint = EnvTemp_ML;
#endif
	}
	else if(TransferBiasPWMForServo < Engine_Parameter[19])
	{
		if(fDuplexSet == CLEAR)
		{
			EnvTempStatus = EnvTemp_L;
			Fuser_Control.fLowTemperature = SET;
			Fuser_Control.fDevFanOff_ForLowServo = SET;
		}
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
		EnvTempStatus_ForDuplexPrint = EnvTemp_L;
#endif
	}
	else
	{
		if(fDuplexSet == CLEAR)
		{
			EnvTempStatus = EnvTemp_DL;
			Fuser_Control.fLowTemperature = SET;
			Fuser_Control.fDevFanOff_ForLowServo = SET;
			// tempOffset = 0;
		}
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
		EnvTempStatus_ForDuplexPrint = EnvTemp_DL;
#endif
	}
#endif
#if (DUPLEX_SLOW_MODE||DUPLEX_SLOW_MODE2)
	if(TransferBiasPWMForServo <= adjust_servo_value_for_duplex_slow_mode) EnvTempStatus_ForDuplexPrint = EnvTemp_HH;
	MSG("(eng)EnvTempStatus = %d, EnvTempStatus_ForDuplexPrint = %d, fLowTemperature = %d, fDevFanOff_ForLowServo = %d\n", EnvTempStatus, EnvTempStatus_ForDuplexPrint, Fuser_Control.fLowTemperature, Fuser_Control.fDevFanOff_ForLowServo);	
#else
	if(fDuplexSet == CLEAR)
	{
		MSG("(eng)EnvTempStatus = %d, fLowTemperature = %d, fDevFanOff_ForLowServo = %d\n", EnvTempStatus, Fuser_Control.fLowTemperature, Fuser_Control.fDevFanOff_ForLowServo);	
	}
#endif
	
	// TransferBiasPWMForInterpageGap = 69;	// ABBA New!!!
	Set_TransferBiasInterpageGap(FALSE);
}

void	Set_TransferBiasInterpageGap_Laminating(void)
{
	Set_Transfer_PWM(TransferBiasPWMForInterpageGap_Laminating);
}

void	Set_TransferBiasInterpageGap(Uint8 IPG)			//Done 	//21Apr09 yjlee 
	/* Set transfer bias to IP level */
{
#if EP_PARAM_TEST
	if(IPG == TRUE) {											// between paper and paper
		if(fParamIPGDuringPrinting == TRUE)
			Set_Transfer_PWM(Param_IPG_During_Printing);
		else
			Set_Transfer_PWM(TransferBiasPWMForInterpageGap);
	} else {													// before printing first paper
		if(fParamIPGBeforePrinting == TRUE)
			Set_Transfer_PWM(Param_IPG_Before_Printing); 
		else
			Set_Transfer_PWM(TransferBiasPWMForInterpageGap);
	}
#else
	if(IPG == TRUE) {
		Set_Transfer_PWM(TransferBiasPWMForInterpageGap); // between paper and paper
	} else {
		Set_Transfer_PWM(TransferBiasPWMForInterpageGap); // before printing first paper
	}
#endif	
}

void	Set_TransferPrinting(Uint8 MediaSpec, Uint8 Source, Uint8 Destination)
{
	Uint8 TransferSpec, PWM_Range;
	Uint8 Xfr_PWM;
	
	if(Source == MS_Duplex) {
		/* For Duplex Front Print */
		TransferSpec = TransferSpecDuplexTable[MediaSpec & 0x0f];
	}
	else {
		/* For Simplex or Duplex Back Side Print */
		TransferSpec = TransferSpecTable[MediaSpec];
#if	ENG_TRACE	
		MSG("(Eng)## TransferSpec = %d, MediaSpec = %d ##\n",TransferSpec,MediaSpec);
#endif
	}

	if(fMotorSpeedMode == MTR_MODE_QUIET)
	{
		//MSG("(eng)TransferPWM for Quiet\n");
#if ABBA2
		if(TransferPWMServoMinusCore <= 2)			PWM_Range = 0;
		else if(TransferPWMServoMinusCore <= 24)	PWM_Range = 1;
		else if(TransferPWMServoMinusCore <= 54)	PWM_Range = 2;
		else 										PWM_Range = 3;
#else
		if(TransferPWMServoMinusCore <= 10)			PWM_Range = 0;
		else if(TransferPWMServoMinusCore <= 34)	PWM_Range = 1;
		else if(TransferPWMServoMinusCore <= 54)	PWM_Range = 2;
		else 										PWM_Range = 3;
#endif
		Xfr_PWM = (TransferSlopeTableForQUIET[TransferSpec][PWM_Range] * TransferPWMServoMinusCore) / 32
			+ TransferOffsetTableForQUIET[TransferSpec][PWM_Range];
#if	ENG_TRACE	
		MSG("(Eng)TransferPWMServoMinusCore = %d, TransferSpec = %d, PWM_Range = %d (Quiet)\n",TransferPWMServoMinusCore,TransferSpec,PWM_Range);
#endif
	}
	else
	{
		//MSG("(eng)TransferPWM for Normal\n");
#if ABBA2
		if(TransferPWMServoMinusCore <= 2)			PWM_Range = 0;
		else if(TransferPWMServoMinusCore <= 24)	PWM_Range = 1;
		else if(TransferPWMServoMinusCore <= 54)	PWM_Range = 2;
		else 										PWM_Range = 3;
#else
#if MAC_TYPE_M										// 40ppm
		if(TransferPWMServoMinusCore <= 10)			PWM_Range = 0;
		else if(TransferPWMServoMinusCore <= 24)	PWM_Range = 1;
		else if(TransferPWMServoMinusCore <= 54)	PWM_Range = 2;
		else 										PWM_Range = 3;
#else 												// 45ppm
		if(TransferPWMServoMinusCore <= 7)			PWM_Range = 0;
		else if(TransferPWMServoMinusCore <= 17)	PWM_Range = 1;
		else if(TransferPWMServoMinusCore <= 29)	PWM_Range = 2;
		else 										PWM_Range = 3;
#endif
#endif
		Xfr_PWM = (TransferSlopeTable[TransferSpec][PWM_Range] * TransferPWMServoMinusCore) / 32
			+ TransferOffsetTable[TransferSpec][PWM_Range];
#if	ENG_TRACE	
		MSG("(Eng)TransferPWMServoMinusCore = %d, TransferSpec = %d, PWM_Range = %d (Normal)\n",TransferPWMServoMinusCore,TransferSpec,PWM_Range);
#endif
	}

#if EP_PARAM_TEST
	if(fParamTransferPWM == TRUE)
	{
		Xfr_PWM = Param_Transfer_PWM;
	}
#endif
	
	TransferBiasPWMForPrinting = Xfr_PWM;
	EP_TransferTable[0] = Xfr_PWM * 0.6;
	EP_TransferTable[1] = Xfr_PWM * 0.8;
	EP_TransferTable[2] = Xfr_PWM;
	cTransferCnt = 0;
//	MSG("(eng)Set_Transfer_PWM = %d (%d)\n", EP_TransferTable[cTransferCnt], cProcessCounter);
	Set_Transfer_PWM(EP_TransferTable[cTransferCnt++]);
//	Set_Transfer_PWM(Xfr_PWM);
}

Uint8	Get_Transfer_PWM(void)
{
	return SavedTransferPWM;
}

Uint8	Get_XfrServoOutState(void)
{
	engine_hardware_t* eng_hw = eng_hardware;
	uint32_t val;
	
	gpio_read(eng_hw->tx_sensing, &val);
	return val;
}

void	Set_Charge_PWM(Uint8 PWM)
{
	engine_hardware_t* eng_hw = eng_hardware;
	
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(HV_PWM_PER_CONV * PWM);
	sencond_cycle = (Uint16)(HV_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->charging_voltage, first_cycle, sencond_cycle);
	
	SavedChargePWM = PWM;
	//Set_Chr_PWM(PWM);
}

void	Set_Developer_PWM(Uint8 PWM)
{
	engine_hardware_t* eng_hw = eng_hardware;
	
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(HV_PWM_PER_CONV * PWM);
	sencond_cycle = (Uint16)(HV_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->developer_voltage, first_cycle, sencond_cycle);
	
	SavedDeveloperPWM = PWM;
	//Set_Dev_PWM(PWM);
}

void	Set_Transfer_PWM(Uint8 PWM)
{
	engine_hardware_t* eng_hw = eng_hardware;
	
	Uint16 first_cycle;
	Uint16 sencond_cycle;
	
	first_cycle = (Uint16)(HV_PWM_PER_CONV * PWM);
	sencond_cycle = (Uint16)(HV_PWM_PERIOD - first_cycle);

	pwm_change_duty_cycle(eng_hw->transfer_voltage, first_cycle, sencond_cycle);
	
	SavedTransferPWM = PWM;
	//Set_Xfr_PWM(PWM);
}

void	Boost_Charge(void)
{
	Uint8	Old_TransferBiasPWMForServo;

	Old_TransferBiasPWMForServo = TransferBiasPWMForServo;
	TransferBiasPWMForServo = 116;

	if(TransferBiasPWMForServo < 100) { /* was 104 */
		Charge_PWM = DefaultCharge_PWM;
	}
	else if(TransferBiasPWMForServo < 115) { /* Was 124 */ 
//		Charge_PWM = DefaultCharge_PWM + 10;
		Charge_PWM = DefaultCharge_PWM;
	}
	else {
//		Charge_PWM = DefaultCharge_PWM + 20;
		Charge_PWM = DefaultCharge_PWM;
	}
	TransferBiasPWMForServo = Old_TransferBiasPWMForServo;

#if EP_PARAM_TEST
	if(fParamChargerPWM == TRUE)
	{
		Charge_PWM = Param_Charger_PWM;
	}
#endif

	Set_Charge_PWM(Charge_PWM);	
}

