/*
 *  $RCSfile: MODEMX.H,v $
 * $Revision: 1.32 $
 *   $Author: miurat $
 *     $Date: 2012/08/21 22:15:08 $
 */
/************************************************************************
 *
 *      MODEMX.H
 *
 *  Modem Public Definitions
 *
 *  Copyright 1998 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef __MODEMX_H__
#define __MODEMX_H__


#define MODEM_TASK_PERIODIC_TIME        10  // mSec

#define USE_SDAA	1
#define V34_SUPPORT 1

//#define MEMORY_POOL_DEFAULT (OSC_MEMORY_POOL *)NULL;

/* Modem Configuration Bit Definitions  */
#define MODEM_IASleepCtrl       0x10  /* 4: 1 = IA sleep control required */
#define MODEM_12dBCtrl          0x08  /* 3: 1 = 12 dB gain control required */
#define MODEM_SigRecType        0x04  /* 2: signal recognition type */
#define MODEM_VceComp           0x02  /* 1: 1 = voice compatible */
#define MODEM_V17Comp           0x01  /* 0: 1 = V.17 compatible */

/* Modem State Definitions */
#define MODEM_IDLE      0
#define HDLC_RX_MODE    1
#define HDLC_TX_MODE    2
#define T4_RX_MODE      3
#define T4_TX_MODE      4
#define FLAGS_RX_MODE   5
#define CID_RX_MODE     6   /* Caller Id. (V.23 Receive only) */
#define MODEM_V8_MODE   7   /* V34 handshake (negotiation) states */
#define MODEM_TX_TCF    8   /* Tx TCF */
#define MODEM_RX_TCF    9   /* Rx TCF */
#define VOICE_RX_MODE   10
#define VOICE_TX_MODE   11

/* Modem Carrier Status Definitions */
#define CARRIER_ON      0
#define CARRIER_OFF     1
#define EQM_BAD         2
#define RLSD_OFF        3

/* Error Message Definitions */
#define NO_ERROR        0
#define NO_COMMAND      1
#define NO_RESPONSE     2
#define FRAME_ERROR     3
#define DISCONNECT      4

/* High Speed Signal Recognition Responses */
#define FSK             5
#define HIGH_SPEED      6
#define NO_CARRIER      7
#define CI_TONE         8
#define TIME_OUT        9

#define MODEM_ABORT     0x80

/* Timeouts */
#define ECM_FLAG_VERIFY_TIME    10
#define T30_FLAG_VERIFY_TIME    100
#define CED_VERIFY_TIME         1000    /* CED verify time = 1 sec */
#define T30_DELAY_TIME          20      /* Time to allow other tasks to run */


/* Modem Speakerphone Operations */
#define SPKRPHONE_MIC_LEVEL 0
#define SPKRPHONE_MUTE_MIC  1
#define SPKRPHONE_VOL_UP    2
#define SPKRPHONE_VOL_DN    3
#define SPKRPHONE_ENABLE    4

#define CONSECUTIVE_EOL  2

#define MAX_MODEM_MCB_MSGS      0x00000100

#define OSI_YSY_ERROR() ASSERT(0)

#define SENDER_DELAY_ADJUSTMENT        40*1000 /* 40 seconds */
#define ECM_BLOCK_LIMIT_TIMEOUT_SEC    30*1000 /* 30 seconds at 33600 */
#define ECM_BLOCK_LIMIT_TIMEOUT        SENDER_DELAY_ADJUSTMENT+(ECM_BLOCK_LIMIT_TIMEOUT_SEC*33600)/ModemControl->ConnectedSpeed

#define TX_PHASE_C_TIMEOUT     10*1000 /* 10 seconds */

typedef enum
{
    DTMF_0, // 0
	DTMF_1,
	DTMF_2,
	DTMF_3,
	DTMF_4,
    DTMF_5, // 5
	DTMF_6,
	DTMF_7,
	DTMF_8,
	DTMF_9,
    DTMF_STAR, // 10
	DTMF_POUND,
	DTMF_A,
	DTMF_B,
	DTMF_C,
    DTMF_D, // 15
	OFF_HOOK,
	CNG_1100HZ,
	CED_2100HZ,
	ANSAM_2100HZ,
    V8_CI, // 20
	V21_CH2,
	V27TER_2400,
	V27TER_4800,
	V29_7200,
    V29_9600, // 25
	V17_7200_LONG,
	V17_7200_SHORT,
	V17_9600_LONG,
	V17_9600_SHORT,
    V17_12000_LONG, // 30
	V17_12000_SHORT,
	V17_14400_LONG,
	V17_14400_SHORT,
	V34_2400,
    V34_4800, // 35
	V34_7200,
	V34_9600,
	V34_12000,
	V34_14400,
    V34_16800, // 40
	V34_19200,
	V34_21600,
	V34_24000,
	V34_26400,
    V34_28800, // 45
	V34_31200,
	V34_33600,
	V34_2400_BAUD,
	V34_2800_BAUD,
    V34_3000_BAUD, // 50
	V34_3200_BAUD,
    V34_3429_BAUD,

    MODEM_TX_TEST_STOP
} MODEM_TX_TEST_INDEX;


typedef enum    /* Index to TxToneTbl[] in modemtbl.c */
{       /* The following constants are in the order to match TxToneTbl[] */
	MODEM_TX_462HZ = 16,    /*  462 HZ */
	MODEM_TX_1100HZ,        /* 1100 HZ */
	MODEM_TX_2100HZ,        /* 2100 HZ */
	MODEM_TX_1650HZ,        /* 1650 HZ */
	MODEM_TX_1850HZ,        /* 1850 HZ */
	MODEM_TX_2425HZ,        /* 2425 HZ */
	/* New Tones for Phone Answer Mode 1/9/99 */
	MODEM_TX_400HZ = 22,    /* #22:  400 Hz */
	MODEM_TX_600HZ,         /* #23:  600 Hz */
	MODEM_TX_800HZ,         /* #24:  800 Hz */
	MODEM_TX_1400HZ,        /* #25: 1400 Hz */
	MODEM_TX_1800HZ,        /* #26: 1800 Hz */
	MODEM_TX_0HZ            /* #27: 0 Hz, Sound Off */
} MODEM_TX_TONE_TBL_INDEX;

/* This section help to move all modem code from Phone Modules to modem.c 6/3/99 */



/* MODEM_TONE_SELECT specifies Id numbers for specific tone detection.
   It may be possible to use multiple filters for certain tone detections.
 */
typedef enum    /* Used by ModemToneDetected() */
{
	MODEM_DIAL_TONE,        /* TONEA for DIAL tone */
	MODEM_CNG_TONE,         /* TONEB for CNG tone */
	MODEM_BUSY_TONE,        /* TONEC for BUSY tone */
	MODEM_FSK_TONE,         /* TONEA for FSK tone */
	MODEM_HISPEED_FSK_TONE, /* FSK tone while receiving high speed data */
	MODEM_CI_TONE,          /* CI tone while in V.21 RX mode */
	MODEM_ANS_TONE,         /* ANSam or CED tone */
	MODEM_NO_TONE           /* no tone */
} MODEM_TONE_SELECT;


typedef enum
{
	Tone9600_LowBCD,
	Tone9600_1100,
	Tone9600_HighBCD,
	Tone_1650Hz,
	Tone8000_LowBCD,
	Tone8000_1100,
	Tone8000_HighBCD,
	Tone_2100Hz,
	Tone_1100Hz
} MODEM_RX_TONE;


typedef enum
{
	V8_MOD,
	V21_MOD,
	V27_MOD,
	V29_MOD,
	V17_MOD,
	V34_MOD,
	TONE1100,
	TONE2100
} MODEM_MODURATION;


typedef enum
{
	LINE_ONHOOK,
	LINE_INUSE,
	LINE_NOLINE,
	LINE_OFFHOOK,
	LINE_UNKNOWN
} LINE_STATUS;


typedef struct
{
	UINT8 lBand;
	UINT8 hBand;
} DTMF_COMP_LEVEL;


typedef struct
{
	UINT8 Normal;
	UINT8 Complex;
} TX_COMP_LEVEL;


typedef struct
{
	UINT16 FaxTxLevel;       /* Fax Tx attenuation (dB, from 10 for -1.0 dBm) */
	UINT8  FaxEqzr;          /* Modem cable equalizer (0 = OFF, 1 = ON) */
	UINT8  RxMaxGain;        /* Modem RX max gain */
	UINT8  ForceEPTOn;       /* 1 = EPT on in all modes (except V.21) */
	UINT8  ForceEPTOff;      /* 1 = EPT off in all modes */
	UINT8  SpeakerVolume;    /* 0 to 7 (max) */
	UINT8  DTMFCompensation;
	UINT8  TxCompensation;
} T_MODEM_OPTIONS;

/* Modem  Timers */
typedef struct
{
	UINT32 start_time;
	UINT32 interval;
} T_MODEM_TIMER;

typedef struct modemcontrol
{
	UINT8  State;           /* Modem current state */
	INT8   Status;          /* Modem status */

	BOOL   V8Answer;        /* V8 negotiations is being done as an answerer */
	BOOL   V8Polling;       /* Initial polling is being done using V8 */
	BOOL   V34Mode;         /* V.34 active*/
	BOOL   HDLCMode;        /* HDLC */
	BOOL   HighSpeed;       /* Primary channel or High speed active */

	UINT16 ConnectedSpeed;

	UINT16 BufferQueueProcess; /* buffer counter to process */
	UINT16 TxFrameCnt;      /* frame counter for non-ECM Tx */
	UINT8  TxBlockCnt;

	UINT16 CDBByteNum;      /* The current byte being processed in the CDB */

	UINT16 TCFThreshold;
	UINT16 TCFOKBytes;
	UINT16 TCFErrorBytes;

	UINT16 ZeroCount;
	UINT8  EOLCount;
	UINT8  EOLType;

	BOOL   FoundRTC;
	BOOL   FoundFlag;
	BOOL   SendCRP;
	INT8   Level;
	UINT16 EQM;

	UINT16 IsrDataCounter;
	UINT16 BytesPerFrame;

	T_MODEM_OPTIONS Options;

	void ( *HSFrameEndNotify ) ( void );
	void ( *HSBadFrameNotify ) ( void );

	T_MODEM_TIMER TimerEvent; /* Timer for timing out modem events */
	T_MODEM_TIMER TimerA;
	T_MODEM_TIMER TimerB;
	T_MODEM_TIMER TimerC;
	T_MODEM_TIMER TimerD;
    T_MODEM_TIMER TimerE; // Used especially for PINETREE/-S system..

#ifdef V34_SUPPORT
	OSC_SEMAPHORE   m_SemaphoreV8;          // semaphore to wait V8 negotiation
	OSC_SEMAPHORE   m_SemaphoreEndOfTxData; // semaphore to wait transmit done
#endif

} T_MODEM_CONTROL;


#define MODEM_TIMER_START(Tim,msec)    modem_timer_start((void *)&ModemControl->Tim,msec)
#define MODEM_TIMER_EXPIRE(Tim)        modem_timer_expire((void *)&ModemControl->Tim)


extern T_MODEM_CONTROL *ModemControl;

extern void ModemIsr( void * );
extern void ModemClearIRQ( void );
extern void ModemRxConfigHDLC( UINT8 speed );
extern void ModemRxInitPage( UINT8 speed, UINT8 training );
extern void ModemTxInitPage( UINT8 speed, UINT8 training );

#ifdef SOFTWARE_FAX
extern void ModemInitToneDet( UINT32 type );
void ModemInitToneGenerator( UINT8 tone );
#else // !SOFTWARE_FAX
extern void ModemInitToneDet( UINT8, const UINT16 * );
#endif // SOFTWARE_FAX

extern void ModemTxDTMFNumbers( UINT8 *digits_buffer, UINT8 on_time, UINT8 off_time, UINT8 lband_level, UINT8 hband_level );
extern UINT8 ModemTxPulseNumbers( UINT8 *digits_buffer, UINT8 pulse_mode, UINT8 make_time, UINT8 break_time, UINT16 inter_delay );
extern void ModemTxTone( UINT16 freq, UINT8 level );
extern void ModemTxDualTone( UINT16 freq1, UINT8 level1, UINT16 freq2, UINT8 level2 );
extern void ModemTxSetDualToneFreq( UINT16 freq1, UINT16 freq2 );
extern void ModemTxSetDualToneLevel( UINT8 level1, UINT8 level2 );
extern void ModemTxDTMFDigit( UINT8 digit, UINT16 on_time, UINT16 off_time, UINT8 lband_level, UINT8 hband_level );
extern void ModemTxPulseDigit( UINT8 digit, UINT8 pulse_mode, UINT8 make_time, UINT8 break_time );
extern UINT8 ModemFlagSequence( void );
extern UINT8 ModemFSKFlags( void );
extern UINT8 ModemDetectTone( UINT8 on_mask, UINT8 off_mask );
extern UINT8 ModemRxTraining( void );
extern UINT8 ModemRxCarrierError( UINT8 ctrl );
extern void ModemRTSConfig( UINT8 mode );
extern void ModemTxStopTestSignal( void );
extern INT8 ModemReadRxLevel( void );
extern INT8 ModemReadTxLevel( void );
extern UINT16 ModemReadEQM( void );
extern void ModemWaitRX( void );
extern void ModemRxConfigRing( void );
extern void ModemRxConfigDTMF( void );
extern void ModemRxConfigDTMFDigit ( BOOL setup_min_time, UINT8 min_time );
extern BOOL ModemRxDTMFDigit( UINT8 *dtmf_digit );
extern UINT8 ModemHighSampleRate( void );

extern UINT8 ModemInitJob( void );
extern void ModemReinitJob( void );
extern void ModemInitVars( void );
extern UINT8 ModemShutdownJob( void );

extern UINT8 ModemRxTCF( UINT8 speed );
extern UINT8 ModemRxHsSignalRec( UINT8 speed, UINT8 training, UINT32 timeout, UINT8 ecm );
extern void ModemTxTCF( UINT8 speed );
extern void ModemSetTraining( UINT8 mode );
extern void ModemTxCED ( void );
extern void ModemTxCNG( void );
extern void ModemEnableRxInt( void );
extern void ModemEnableTxInt( void );
extern void ModemDisableRxInt( void );
extern void ModemDisableTxInt( void );
extern void ModemCDBWriteErrorData( void );
extern void ModemCDBMarkEndOfPage( void );
extern void ModemCDBDone( UINT8 last_buffer );
extern UINT16 ModemV34FallSpeed( void );
extern UINT8 ModemV34CheckCC( void );
extern void ModemReset( UINT8 force );
extern void ModemConfigGPIO( UINT8 port, UINT8 out );
extern void ModemSetGPIO( UINT8 port, UINT8 out );
extern UINT8 ModemGetGPIO( UINT8 port );
extern BOOL ModemBitOrBust( UINT8 reg, UINT8 bit, BOOL change );
extern void ModemConnectedSpeed( UINT8 IndexToSpeed );

extern void ModemEnableToneInterrupt( MODEM_TONE_SELECT tone, BOOL enable );

/* Called Station Detection Constants */
#define MODEM_FAX_PHASE_A_TIMEOUT   1
#define MODEM_FAX_PHASE_A_BUSY      2
#define MODEM_FAX_PHASE_A_CED       3
#define MODEM_FAX_PHASE_A_CED_POLL  4
#define MODEM_FAX_PHASE_A_V34       5
#define MODEM_FAX_PHASE_A_V34_POLL  6
#define MODEM_FAX_PHASE_A_NON_V34   7
#define MODEM_FAX_PHASE_A_ABORT     0x80
extern UINT8 ModemTxDetCalledStation( UINT16 timeout, UINT8 ( *abort_fn ) ( void ), UINT8 *speed, BOOL poll );
extern UINT8 ModemRxBillingDelay( UINT8 *speed, BOOL poll, UINT16 timeout );
extern UINT8 V8Originate( UINT8 *speed, BOOL poll, BOOL sendCI, UINT16 timeout, UINT8 ( *abort_fn ) ( void ) );
extern UINT8 V8Answer( UINT8 *speed, BOOL poll, BOOL postDIS, UINT16 timeout );
extern void V8Tx_ANSam( void ) ;
extern void V8Tx_CI( void );

extern void ModemRxCIDSetup( void );
extern void ModemRxCIDShutdown( void );
extern void ModemTimerStart( UINT32 *timer );
extern BOOL ModemTimerExpired( UINT32 start_time, UINT32 interval );
extern UINT32 ModemElapsedTime( UINT32 start_time );
extern void modem_timer_start( void* tmr, UINT32 interval );
extern BOOL modem_timer_expire( void* tmr );

extern const UINT16 Tone9600CoeffTbl440[];      /* Dial Tone */
extern const UINT16 Tone8000CoeffTbl440[];
extern const UINT16 Tone9600CoeffTbl505[];      /* Busy/Congestion/Dial Tone */
extern const UINT16 Tone8000CoeffTbl505[];
extern const UINT16 Tone9600CoeffTblLowBCD[];   /* Busy/Congestion/Dial Tone */
extern const UINT16 Tone8000CoeffTblLowBCD[];
extern const UINT16 Tone9600CoeffTblHighBCD[];  /* Busy/Congestion/Dial Tone */
extern const UINT16 Tone8000CoeffTblHighBCD[];
extern const UINT16 Tone9600CoeffTbl1100[];     /* CNG Tone */
extern const UINT16 Tone8000CoeffTbl1100[];
extern const UINT16 Tone9600CoeffTbl1650[];
extern const UINT16 Tone9600CoeffTbl980[];

extern const UINT16 CoeffTblHighBCD[];          /* Busy/Congestion/Dial Tone */
extern const UINT16 CoeffTbl950[];
extern const UINT16 CoeffTbl980[];
extern const UINT16 CoeffTbl1180[];
extern const UINT16 CoeffTbl1100[];
extern const UINT16 CoeffTbl1250[];
extern const UINT16 CoeffTbl1650[];
extern const UINT16 CoeffTbl2100[];

extern ERCODE ModemSetupToneDet( MODEM_TONE_SELECT Filter );
extern BOOL ModemToneDetected( MODEM_TONE_SELECT Filter );
extern void ModemDisableToneDet ( MODEM_TONE_SELECT tone );
extern BOOL ModemDetectRing( void );
extern void ModemSetSpeakerVolume( UINT8 Volume );
extern void ModemTxConfigHDLC(UINT8);
extern void ModemTxSetV34SymbolRate( UINT16 rate );
extern void ModemTxSetPermanentData( UINT8 configure, UINT8 data, UINT8 speed, UINT8 training );

extern UINT8 ModemFEDStatus( void );
extern UINT8 ModemVOXStatus( void );

extern BOOL ModemGPI( UINT8 Bit );
extern void ModemGPO( UINT8 Bit, BOOL Set );

extern UINT8 ModemCompareAGCGain( void );

extern void ModemRxTrainingDisable( BOOL mode );
extern void ModemShowRAMVersion( void );

extern void SSFOffHook( void );
extern void SSFOnHook( void );
extern void SSFStopMonitorAPI( void );
extern void SSFStartMonitorAPI( void );
extern void SSFStopOffHookMonitor( void );
extern void SSFStartOffHookMonitor( void );
extern UINT16 SSFLineVoltage( void );

extern LINE_STATUS SSFLineInUse( void );
extern LINE_STATUS SSFLineStatus( void );

#endif // __MODEMX_H__

