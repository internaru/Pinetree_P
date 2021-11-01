/*
 *  $RCSfile: phone.h,v $
 * $Revision: 1.9 $
 *   $Author: yooh1 $
 *     $Date: 2011/06/23 00:17:05 $
 */
/************************************************************************
 *
 *      phone.h
 *
 *  Definitions for Phone Module's internal users.
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

typedef enum    /* Answer Mode Line Signal State */
{
    ANS_NONE,       /* Initial State */
    ANS_TIMEOUT,    /* Detection Timeout */
    ANS_SILENCE,    /* Silence Timeout */
    ANS_HDLC,       /* HDLC detected */
    ANS_CNG,        /* CNG TONE detected */
    ANS_BUSY_TONE,  /* DIAL TONE detected */
    ANS_DIAL_TONE   /* DIAL TONE detected */
} ANS_STATE;


typedef struct tagPDialParams
{
	UINT8  BlindDial;

	UINT8  DTMFOnTime;
	UINT8  DTMFOffTime;
	UINT8  DTMFLowLevel;
	UINT8  DTMFHighLevel;
	UINT8  DTMFCompensation;
	UINT8  DTMFAbcdAllow;

	UINT8  PulseMode;
	UINT8  PulseSpeed;
	UINT8  PulseMakeTime;
	UINT8  PulseBreakTime;
	UINT16 PulseInterDelay;
	UINT8  PulseMakeOffset;
	UINT8  PulseBreakOffset;
	UINT8  PulseSetupTime;
	UINT8  PulseClearTime;
	UINT8  PulseDialAllow;
} PDialParams;


typedef struct tagPToneParams
{
	UINT16 MaxSilence;
	UINT16 BlindDialDelay;
	UINT16 DialToneLen;
	UINT16 DialToneTimeOut;
	UINT16 BusyToneOnMin;
	UINT16 BusyToneOnMax;
	UINT16 BusyToneOffMin;
	UINT16 BusyToneOffMax;
	UINT16 BusyToneLen;
	UINT16 BusyToneTimeOut;
	UINT16 CongToneOnMin;
	UINT16 CongToneOnMax;
	UINT16 CongToneOffMin;
	UINT16 CongToneOffMax;
	UINT16 CNGOnMin;
	UINT16 CNGOnMax;
	UINT16 CNGOffMin;
	UINT16 CNGOffMax;
	UINT16 DTMFOnTime;
	UINT16 DTMFOffTime;
	UINT16 DTMFVerifyTime;
	UINT16 FlagVerifyTime;
	UINT16 AnswerStable;
} PToneParams;


typedef struct tagPRingParams
{
	UINT16 RingOnDebounce;      /* Min. time to confirm RI ON  state */
	UINT16 RingOffDebounce;     /* Min. time to confirm RI OFF state */
	UINT16 RingMaxGlitch;       /* Max. time for RI ON/OFF glitchs */

	UINT16 RingOffMax;          /* Max Short Silence: 700 ms */
	UINT16 RingOnMax;           /* PATTERN_1: 1750 - 2250 ms */
	UINT16 RingOffTimeout;      /* PATTERN_1: 3550 - 4450 ms */

	/* Report time out after 10 Sec. ring off */
	UINT16 RingTimeoutNotify;   /* >= RingOffTimeout */

	/* First ringing burst time mapping */
	UINT16 RingOn1Ptn0;         /* Error */
	UINT16 RingOn1Ptn1;         /* PATTERN_1: 1750-2250 ms */
	UINT16 RingOn1Ptn2x;        /* PATTERN_X:  800-1100 ms?*/
	UINT16 RingOn1Ptn2xy;       /* PATTERN_2:  670- 930 ms, 580-1075 ms */
	/* PATTERN_Y:  600- 900 ms?*/
	UINT16 RingOn1Ptn2345y;     /* PATTERN_5:  400- 600 ms */
	UINT16 RingOn1Ptn345;       /* PATTERN_3:  310- 490 ms, 265- 575 ms */
	UINT16 RingOn1Ptn34;        /* PATTERN_4:  220- 380 ms, 150- 575 ms */

	/* First silence interval mapping */
	UINT16 RingOff1Ptn0;        /* Error */
	/* PATTERN_5:  N.A.*/
	UINT16 RingOff1Ptn15;       /* PATTERN_1:  3550-4450 ms */
	UINT16 RingOff1Ptnx;        /* PATTERN_X:  2550-3450 ms?*/
	UINT16 RingOff1Ptn15x;      /* for RingOff1Go */
	UINT16 RingOff1Ptn00;       /* Error */
	/* PATTERN_Y:  300-  500 ms?*/
	UINT16 RingOff1Ptn234y;     /* PATTERN_2:   310- 490 ms, 265- 575 ms */
	UINT16 RingOff1Ptn34;       /* PATTERN_3,4: 125- 275 ms,  95- 575 ms */

	/* CID turn on time mapping */
	UINT16 RingOffCidOn;        /* Turn on <= 500 (300) ms */
	UINT16 RingOffCidOff125;    /* around 3425 - 200 (475) ms, 125 */
	UINT16 RingOffCidOff34;     /* around 2925 - 200 (475) ms, 34 */
	UINT16 RingOffCidOffx;      /* around 2200 - 200 (475) ms, X */
	UINT16 RingOffCidOffy;      /* around 1600 - 200 (475) ms, Y */

	/* Second ringing burst time mapping */
	UINT16 RingOn2Ptn0;         /* Error */
	UINT16 RingOn2Ptn24y;       /* PATTERN_4:  850-1150 ms, 700-1150 ms */
	/* PATTERN_Y:  600- 900 ms?*/
	UINT16 RingOn2Ptn23y;       /* PATTERN_2:  670- 930 ms, 580-1075 ms */
	UINT16 RingOn2Ptn3;         /* PATTERN_3:  310- 490 ms, 265- 575 ms */

	/* Second silence interval mapping */
	UINT16 RingOff2Ptn0;        /* Error */
	UINT16 RingOff2Ptn2;        /* PATTERN_2:  3550-4450 ms, 3425-4450 ms */
	UINT16 RingOff2Ptny;        /* PATTERN_Y:  1600-2200 ms?*/
	UINT16 RingOff2Ptn2y;       /* for Off2Go */
	UINT16 RingOff2Ptn34;       /* PATTERN_3,4: 125- 275 ms,   95- 575 ms */

	/* Third ringing burst time mapping */
	UINT16 RingOn3Ptn0;         /* Error */
	UINT16 RingOn3Ptn3;         /* PATTERN_3:  670- 930 ms, 580-1075 ms */
	UINT16 RingOn3Ptn34;
	UINT16 RingOn3Ptn4;         /* PATTERN_4:  220- 380 ms, 150- 575 ms */
} PRingParams;

typedef struct              /* Ring information structure */
{
	UINT16 RingOnTime;      /* milli-seconds */
	UINT16 RingOffTime;     /* milli-seconds */
	INT8 RingCount;         /* < 0, if long silence timeout. */
} RING_INFORMATION;


/*********************************************************************/

/* External variables */

extern PDialParams PDP;
extern PToneParams PTP;
extern PRingParams PRP;

#ifdef FAXVOICE
extern UINT8  ExtFaxVoiceTimeout;
extern UINT8  IntFaxVoiceTimeout;
extern UINT16 FaxVoiceSilenceTime;
#endif  /* FAXVOICE */

#ifdef MUSIC_ON_HOLD
extern const UINT8  SongNote[];
extern const UINT8  NoteSize;
extern const UINT16 NoteFreqTbl[];
extern const UINT16 TimeTbl[];        /* 3/24/98 changed from UINT8 */
extern UINT16 SpareTone[];
#endif  /* MUSIC_ON_HOLD */

#ifdef RING_DET_ISR
extern const UINT8  MaxRingPeriod;    /* 68 msec ,14.7 Hz, (Note: 2 msec base) */
extern const UINT8  MinRingPeriod;    /* 12 msec, 83.3 Hz, (Note: 2 msec base) */
extern const UINT8  MinRingCycles;    /* need to detect min. 7 cycles for "good ring" */
extern const UINT8  MinRingSilence;   /* Min. silence 250 msec after ring for "good ring" (2 msec base) */
#endif  /* RING_DET_ISR */


/*********************************************************************/

/** External Function declaration **/

extern void CxPhoneReport( UINT8 type, UINT8 *buffer );

/* dialer.c */
extern void phone_auto_dial( UINT8 *digits_buffer, UINT8 mode );
extern void phone_dial_digit( UINT8 digit, UINT8 type );


/* tonedet.c */
extern void SetMaxBusyToneTime(UINT16 TimeInMs);
extern void SetMaxCNGHalfCycle(UINT8 HalfCycleCount);
extern void  SetUpDTMFDetection(void);
extern UINT8 CheckDTMFDetection(void);

/* This one is defined in PHONEX.H, because TONE_TYPES is defined there */
//extern ERCODE SetUpToneDetection(TONE_TYPES ToneType, UINT16 DetTimeout);

/* ringdet.c */
extern void Init_RingState(BOOL bConf);
extern INT8 Detect_Ring_CID(UINT8 SamplingTimeInMS);
//extern ANSI_RING_PATTERNS GetDetectedRingPattern(void);// only used by phone.c
#ifdef _DEBUG
extern void PseudoRingDVTTEST(void);
#endif


/* callerid.c */
extern void CIDType1Start(void);
extern void CIDType2Start(void);
extern void CIDShutdown(void);
extern BOOL CIDMonitor(UINT8 SamplingTimeInMS);
extern BOOL CIDRxData(UINT8 data);
//extern CID_INFORMATION *CIDGetInforAddr(void);    // only used by phone.c

/* phone_main.c */
extern void phone_task( void );
extern void set_phone_number( UINT8 *dial_num, UINT8 mode );
extern void set_phone_state( UINT8 new_state );
extern void set_phone_status( UINT8 info, UINT8 value );
extern UINT8 get_tone_status( UINT8 info );

/*********************************************************************/

