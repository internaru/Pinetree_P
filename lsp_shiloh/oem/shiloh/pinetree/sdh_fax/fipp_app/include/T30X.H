/*
 *  $RCSfile: T30X.H,v $
 * $Revision: 1.28 $
 *   $Author: miurat $
 *     $Date: 2011/12/16 02:28:15 $
 */
/************************************************************************
 *
 *      T30X.H
 *
 *  T.30 Public Definitions.
 *
 *  Copyright 2004 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef _T30X_H_
#define	_T30X_H_

#include "SDHFaxCommonHeader.h"

/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/
/* Fax States */
typedef enum t30states
{
    STATE_IDLE,
    STATE_RX_PHASE_A,
    STATE_RX_PHASE_B,
    STATE_RX_PHASE_B_SIGNAL_REC,
    STATE_RX_PHASE_C,
    STATE_RX_PHASE_C_ECM,
    STATE_RX_PHASE_D_SIGNAL_REC,
    STATE_RX_PHASE_D,
    STATE_RX_PHASE_E,
    STATE_RX_DISCONN,
    STATE_RX_ABORT,
    STATE_TX_PHASE_A,
    STATE_TX_PHASE_B,
    STATE_TX_SET_MODE,
    STATE_TX_PHASE_C,
    STATE_TX_PHASE_C_ECM,
    STATE_TX_PHASE_D,
    STATE_TX_PHASE_D2,
    STATE_TX_PHASE_E,
    STATE_TX_DISCONN,
    STATE_TX_VOICE_REQ,
    STATE_TX_ABORT,
    STATE_HOST_ABORT
} T_T30_STATES;


typedef enum {
    FXA_INIT_ATTRIBUTE,
    FXA_COUNTRY_CODE,
    FXA_ATTENUATION,
    FXA_BUFFERED_DELAFTERTX,
    FXA_BUFFEREDPRINT,
    FXA_CEDDETECT_TIMEOUT,      // 5
    FXA_COVERPAGE,
    FXA_COVERPAGE_ID,
    FXA_DELAYEDTXDOCID,
    FXA_DOCID,
    FXA_ECMMODE,                // 10
    FXA_EQUALIZER,
    FXA_FALLBACK_NOOFTRIES,
    FXA_FAXFORWARD,
    FXA_FAXFORWARD_NO,
    FXA_FAXLOGGING,             // 15
    FXA_IGNOREECHO,
    FXA_IGNOREECM,
    FXA_IGNOREMMR,
    FXA_IGNOREMR,
    FXA_IGNOREJBIG,             // 20
    FXA_IGNOREJBIGL0,
    FXA_IGNOREJPEG,
    FXA_IGNOREFULLCOLOR,
    FXA_IGNOREPAGEMEMORY,
    FXA_IGNORE_VOICEREQ,        // 25
    FXA_IMGPROC,
    FXA_INTLMODE,
    FXA_LASTTXLOG,
    FXA_LINEREQ,
    FXA_LOCALID,                // 30
    FXA_LOGTYPE_INIT,
    FXA_LOG_ERRORRESULT,
    FXA_LOG_PRINT,
    FXA_LOG_REMOTENUMBER,
    FXA_PAGENO,                 // 35
    FXA_REMOTEID,
    FXA_TXRESOLUTION,
    FXA_RXRESOLUTION,
    FXA_FORCERXTOMEM,
    FXA_FORCERXTOMEM_COLOR,     // 40
    FXA_RXMAXGAIN,
    FXA_SPEAKER_CALLMODE,
    FXA_SPEAKER_ANSWERMODE,
    FXA_SPEAKERVOLUME,
    FXA_TXSTARTSPEED,           // 45
    FXA_RXSTARTSPEED,
    FXA_T1TIMEOUT,
    FXA_T5TIMEOUT,
    FXA_TCFERRORS,
    FXA_TEXTHEADER,             // 50
    FXA_V29EPT,
    FXA_VOICEREQ,
    FXA_TXREQUIREPASSWORD,
    FXA_RXREQUIREPASSWORD,
    FXA_TXPASSWORD,             // 55
    FXA_RXPASSWORD,
    FXA_REMOTEPASSWORD,
    FXA_TXREQUIRESUBADDRESS,
    FXA_RXREQUIRESUBADDRESS,
    FXA_TXSUBADDRESS,           // 60
    FXA_RXSUBADDRESS,
    FXA_REMOTESUBADDRESS,
    FXA_POLLTXMODE,
    FXA_POLLRXMODE,
    FXA_POLLTXREQUIREPASSWORD,  // 65
    FXA_POLLRXREQUIREPASSWORD,
    FXA_POLLTXPASSWORD,
    FXA_POLLRXPASSWORD,
    FXA_POLLREMOTEPASSWORD,
    FXA_POLLTXREQUIRESUBADDRESS,// 70
    FXA_POLLRXREQUIRESUBADDRESS,
    FXA_POLLTXSUBADDRESS,
    FXA_POLLRXSUBADDRESS,
    FXA_POLLREMOTESUBADDRESS,
    FXA_ENABLENSF,              // 75
    FXA_LOCALNSF,
    FXA_LOCALNSC,
    FXA_LOCALNSS,
    FXA_REMOTENSF,
    FXA_REMOTENSC,              // 80
    FXA_REMOTENSS,
    FXA_TXIMAGEWIDTH,
    FXA_RXIMAGEWIDTH,
    FXA_TXDATASOURCE,
    FXA_DIALMODE,               // 85
    FXA_ANSWERMODE,
    FXA_AUTOANSWERRINGS,
    FXA_RETRY_NUMBER,
    FXA_RETRY_INTERVAL,
    FXA_BRIGHTNESS,             // 90
    FXA_CONTRAST,
    FXA_TTI_ENABLE,
    FXA_SPEAKERV8OFF,
#if __SUPPORT_MANUAL_RECEIVE__
    FXA_RXV34ABORTBADEQM,
    FXA_MANUALRECEIVE,            // 95
    FXA_RECEIVESOURCE,
#else
    FXA_RXV34ABORTBADEQM
#endif
} T30_ATTRIB;

enum {
    T30_SPEED_V21_300,
    T30_SPEED_V27_2400,
    T30_SPEED_V27_4800,
    T30_SPEED_V29_7200,
    T30_SPEED_V29_9600,
    T30_SPEED_V17_7200,
    T30_SPEED_V17_9600,
    T30_SPEED_V17_12000,
    T30_SPEED_V17_14400,
    T30_SPEED_V34_2400,
    T30_SPEED_V34_4800,
    T30_SPEED_V34_7200,
    T30_SPEED_V34_9600,
    T30_SPEED_V34_12000,
    T30_SPEED_V34_14400,
    T30_SPEED_V34_16800,
    T30_SPEED_V34_19200,
    T30_SPEED_V34_21600,
    T30_SPEED_V34_24000,
    T30_SPEED_V34_26400,
    T30_SPEED_V34_28800,
    T30_SPEED_V34_31200,
    T30_SPEED_V34_33600
};


/******************************************************************************
*  2. DEFINES
******************************************************************************/

#ifndef __ATTRIB_H__
#define GET             0
#define SET             1
#endif  // __ATTRIB_H__

#define IMG_BILEVEL     0
#define IMG_GRAYSCALE   1
#define IMG_COLOR       2
#define IMG_PHOTO       3

/* T30 Rx Mode */
#define RX_NOT_YET      0
#define RX_MEMORY       1
#define RX_PRINTER      2
#define RX_POLL_TX      3
#define RX_STOP         4

/* Flow direction */
#define TO_PRINTER      0
#define TO_MEMORY       1
#define FROM_SCANNER    2

/* Transmit Poll Doc from options */
#define TXPOLLFROM_NONE   0
#define TXPOLLFROM_MEMORY 1
#define TXPOLLFROM_SCAN   2
#define TXPOLLFROM_BOTH   3

#define RX_POLL 1

/* T.30 message definitions */
#define FXM_CALL_AUTO         0
#define FXM_CALL_MANUAL       1
#define FXM_POLL_AUTO         2
#define FXM_POLL_MANUAL       3
#define FXM_ABORT             4
#define FXM_MEMTX             5
#define FXM_ANSWER            6
#define FXM_TX_ALL_RCVD_FAXES 7

/* Answer mode defines */
#define AUTO_ANSWER      0
#define MANUAL_ANSWER    1

#if __SUPPORT_MANUAL_RECEIVE__
/* Receive source defines */
#define RECEIVE_SOURCE_MACHINE     0
#define RECEIVE_SOURCE_PHONE       1
#else
#endif

#define DIAL_DTMF        0
#define DIAL_PULSE       1

#define T30_SPEKER_OFF   0
#define T30_SPEKER_CALL  1
#define T30_SPEKER_ANS   2
#define T30_SPEKER_MANUAL_ON  3
#define T30_SPEKER_MANUAL_OFF 4
#define T30_SPEKER_V8_OFF 5

/* Defined here for use in modem code */
#define T1_TIMEOUT  38000   /* T1 time-out = 38 sec */
#define T2_TIMEOUT   6700   /* T2 time-out = 6.7 sec */

#define LocalDISSize  10
#define RemoteCapSize 12
#define NSTDHeaderSize  4
#define StandardFIFSize 20

#define T30_TIMER_START(Tim,msec)    t30_timer_start((void *)&T30Control->Tim,msec)
#define T30_TIMER_EXPIRE(Tim)        t30_timer_expire((void *)&T30Control->Tim)


/******************************************************************************
*  3. STRUCTURES
******************************************************************************/

typedef struct
{
    UINT32 start_time;
    UINT32 interval;
} T_T30_TIMER;


/* T4Tx/T4Rx pipeline parameters    */
// temp for compile THY 6/4/09
typedef unsigned char IMG_PARAMS;

typedef enum
{
   PAPSIZE_LETTER,
   PAPSIZE_LEGAL,
   PAPSIZE_A4,
   PAPSIZE_6INCH,
   PAPSIZE_B4,
   PAPSIZE_A3,
   NO_OF_PAPER_SIZES
} PAPSIZE;

typedef struct rxparams
{
    UINT8  Res;
    UINT8  Coding;
    UINT8  Width;
    UINT8  ECMMode;
    UINT16 DocID;
    UINT8  DocOwnerID;
    PAPSIZE PaperType;
    UINT8  PageLen;
    UINT8  T4DxJobID;
    UINT8  FileCxDxJobID;
    BOOL   PrtInitialized;
    UINT8  PrtStatus;           /* Miscellaneous printer status information */
    BOOL BufferedRx;
    BOOL FaxStorage;
    BOOL DelayedInitJob;
} T_T4_RX_PARAMS;

typedef struct txparams
{
    UINT8  Res;
    UINT8  Coding;
    UINT8  Width;
    UINT8  ECMMode;
    UINT16 DocID;
    UINT8  DocOwnerID;
    PAPSIZE PaperType;
    UINT8  PageLen;
    UINT8  T4CxJobID;
    UINT8  FileCxDxJobID;
    BOOL   ScnInitialized;
    UINT8  ScnStatus;           /* Miscellaneous scanner status information */
    BOOL   BufferedTx;
    UINT8  PagesInDoc;
    UINT8  MinBytes;
    UINT8  CoverPage;           /* Coverpage sending selection */
    BOOL   SendingCoverPage;
    UINT16 CoverPageDocID;      /* ID of the coverpage document */
    UINT8  CoverPageDocOwnerID;
    UINT8  ImageType;           /* Type of image to be sent, color, grayscale, or bi-level */
    UINT16 ScnMode;             /* Scanner (resolution) stepping mode selection */
    IMG_PARAMS OrigImgProc;     /* Copy of the original image processing values */
} T_T4_TX_PARAMS;

typedef union txrxparams
{
    T_T4_TX_PARAMS  Tx;
    T_T4_RX_PARAMS  Rx;
} T_T4_TX_RX_PARAMS;

/* Main T.30 control structure */
typedef struct t30control
{
    T_T30_STATES State;        /* T.30 state variable */
    UINT16 Status;              /* T.30 status variable */
    UINT8 PhoneStatus;         /* Phone status */
    UINT8 ManualMode;          /* 0 = auto-dial/-answer, 1 = manual dial/answer */
    UINT8 *FIF_PTR;            /* Pointer to the FIF to send */
    UINT8 BusyToneDetected;    /* 1 = Busy tone detected during Phase A */
    UINT8 CalledStationDetected; /* Called station detected during Phase A */
    UINT8 OrigRes;             /* Copy of the original fax resolution */
    UINT8 OrigWidth;           /* Copy of the original page width */
    UINT8 ModemSpeed;          /* Modem speed table index (see above) */
    UINT8 ModemMaxSpeed;       /* Modem maximum speed */
    UINT8 ModemSpeedTryCnt;    /* Number of times current speed has been already tried */
    UINT8 DISFlg;              /* 0 = DIS not received, 1 = DIS received */
    UINT8 TxPollFlg;           /* 0 = not TX by polling, 1 = TX by polling */
    UINT8 VoiceReqFlg;         /* 1 = 'voice request' active, 0 = not */
    UINT8 NSTDFlg;             /* 1 = Fax Engine non-standard mode, 0 = not */
    UINT8 LocalNSTD[NSTDHeaderSize];    /* FIF for the NSF/NSS/NSC frame - country & maker code */
    UINT8 LocalNSTDCap[LocalDISSize];   /* Rest of FIF is same as DIS/DCS */
    UINT8 FirstCmdFlg;         /* First command for a multi-command sequence */
    UINT8 FrameTxCnt;          /* Counts number of times a T.30 command is sent */
    UINT8 FrameRepeat;         /* Number of times a T.30 command should be repeated */
    UINT8 PRIQCnt;             /* Counts number of PRI-Q's received */
    UINT8 Try2400;             /* Counts number of times we've tried 2400 bps */
    UINT8 LostCarrFlg;         /* 1 = carrier lost during page reception */
    UINT8 SignalRecFlg;        /* 1 = FSK detected during signal recognition */
    UINT8 PageNum;             /* Document page number */
    UINT8 PagesDone;           /* Pages printed */
    UINT8 LastFCFRcvd;         /* Copy of last FCF received */
    UINT8 LastDIS;             /* Copy of 2nd byte of last DIS received */
    UINT8 LastFCFSent;         /* Copy of last FCF sent */
    UINT8 FCFToSend;           /* FCF to send during post-message procedure */
    UINT8 PostMsgCmd;          /* Post-message command (0 = EOP, 1 = MPS, 2 = EOM) */
    UINT8 RemoteCap[RemoteCapSize]; /* Remote DIS, DCS, or DTC FIF */
    UINT8 LocalCap[LocalDISSize];   /* Local DIS, DCS, or DTC FIF */
    UINT8 MaxFIFLen;           /* Max. FIF length */
    UINT8 AlertCnt;            /* Counts beeps */
    UINT8 RxToMemFlg;          /* Non-zero = RX to memory */
    UINT8 TxFromMemFlg;        /* Non-zero = TX from memory */
    UINT8 PollFromMemFlg;      /* Non-zero = poll from memory */
    BOOL  TxAllRcvdFaxes;      /* Transmit all received faxes in memory */
    UINT8 MemPageNum;          /* Number of RX pages stored in memory
                                  or page number of current document to transmit */
    UINT8 ECMEnabled;          /* 1 = ECM mode enabled */
    UINT8 ECMLongTrain;        /* 1 = force a long train after ECM CTC/CTR */
    UINT8 Direction;           /* 0 = tx, 1 = rx */
    UINT8 ScnDocPresent;       /* 0 = no document in feeder, 1 = document in feeder */
    T_T30_TIMER TimerA;        /* T.30 Timer A */
    T_T30_TIMER TimerB;        /* T.30 Timer B */
    T_T30_TIMER TimerC;        /* T.30 Timer C */
    T_T30_TIMER TimerD;        /* T.30 Timer D */
    T_T30_TIMER TimerE;        /* T.30 Timer E */
    UINT32 SuspendTime;        /* Time (msec) to suspend T.30 task */
    UINT32 DelayTime;          /* Time (msec) to delay T.30 task to allow others to run */
    T_T4_TX_RX_PARAMS T4Params;

       /* Other call log related stuff */
    UINT32 CallDuration;        /* Amount of time taken by the call in ticks.
                                  Start time in ticks till before hanging up phone. */
    UINT32 errorpage[8];        /* Pages that received an RTN on transmission */
    UINT32 LogTypeInitialValue; /* Initial value of the Type field of the log */

    BOOL  V34Mode;              /* Non-zero = In V34 mode */
    UINT8 TriedV8;              /* Non-zero = In non-V34 mode after V8 negotiations */
    UINT8 DIS_size;             /* Size of the DIS received from the remote */
    IMG_PARAMS ImageProcessing; /* Scan or print Image processing parameters */
    UINT8 PPRSendCnt;           /* number of sending PPR before sending MFC THY */
    UINT8 end_of_fax;
    UINT8 rcvd_post_msg;        // received post message to check mult-page case
    BOOL  release_semaphore;    // TRUE if release semaphore for SrvFax after completing of Phase B nego.
    UINT32 total_jbig_rcvd_line;	// used by jd

    UINT8 got_PWD;
    UINT8 got_SID;
    UINT8 got_SUB;
    UINT8 got_SEP;
    UINT8 got_PSA;

    UINT8 DocCreateFlag;		// 1 = Document is created.
} T_T30_CONTROL;

/* T.30 Control Externals */
extern T_T30_CONTROL *T30Control;


/******************************************************************************
*  4. ALLOCS
******************************************************************************/


/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/
/* T.30 public functions */
extern ERCODE T30Message(UINT8 message);
extern ERCODE T30Attribute( UINT8 set_get, T30_ATTRIB attrib_id, UINT8 *msg );
extern int T30Query(UINT8 request, UINT8 *pBuff, UINT32 size);
extern void T30SpeakerControl(UINT8 control);
extern void set_fsu_state(UINT8 state);
UINT8 get_fsu_state(void);
extern void t30_timer_start( void *tmr, UINT32 msec );
extern BOOL t30_timer_expire( void *tmr );

#endif // _T30X_H_

