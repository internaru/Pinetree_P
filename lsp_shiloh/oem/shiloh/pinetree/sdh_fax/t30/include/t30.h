/*
 *  $RCSfile: t30.h,v $
 * $Revision: 1.25 $
 *   $Author: miurat $
 *     $Date: 2011/12/16 02:28:36 $
 */
/************************************************************************
 *
 *      t30.h
 *
 *  T.30-related Definitions.
 *
 *  Copyright 2004 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "SDHFaxCommonHeader.h"

/* T.30 Facsimile Control Field (FCF) Definitions */
#define CFR         0x84
#define CIG         0x41
#define CRP         0x1A
#define CSI         0x40
#define CTC         0x12
#define CTR         0xC4
#define DCN         0xFA
#define DCS         0x82
#define DIS         0x80
#define DTC         0x81
#define EOM         0x8E
#define EOP         0x2E
#define EOR         0xCE
#define ERR         0x1C
#define FCD         0x06
#define FTT         0x44
#define MCF         0x8C
#define MPS         0x4E
#define NSC         0x21
#define NSF         0x20
#define NSS         0x22
#define PIN         0x2C
#define PIP         0xAC
#define PPR         0xBC
#define PPS         0xBE
#define PRI_EOM     0x9E
#define PRI_EOP     0x3E
#define PRI_MPS     0x5E
#define PWD         0xC1
#define RCP         0x86
#define RNR         0xEC
#define RR          0x6E
#define RTN         0x4C
#define RTP         0xCC
#define SEP         0xA1
#define SID         0xA2
#define SUB         0xC2
#define TSI         0x42
#define PSA         0x61
#define SEP_ERR     0xB1
#define SID_ERR     0xB2
#define PWD_ERR     0xD1
#define SUB_ERR     0xD2
#define PSA_ERR     0x71
#define RTC         0xF6
#define NUL         0x00
#define T30_END     0xFF

#if V34_SUPPORT
#define T30_TX_START_SPEED  T30_SPEED_V34_33600    /* Starting Tx speed */
#define T30_RX_START_SPEED  T30_SPEED_V34_33600    /* Starting Rx speed */
#else
#define T30_TX_START_SPEED  T30_SPEED_V17_14400    /* Starting Tx speed */
#define T30_RX_START_SPEED  T30_SPEED_V17_14400    /* Starting Rx speed */
#endif

/* T30 Direction */
#define T30_RECEIVE  0
#define T30_TRANSMIT 1
#define MEM_TO_PRINT 2
#define SCAN_TO_MEM  3

/* Non ECM Mode */
#define RTN_PERCENT 14

/* ECM Mode */
#define ECM_NONE 0
#define ECM_256  1
#define ECM_64   2

#ifdef INTERNET_FAX
#define ECM_T37  3      /* Internet FAX (T.37) mode */
#endif

#define MAX_RCP_FRAMES   3

#define PPR_FOR_FALLBACK 4

/* K Factors */
#define KFACTOR_STANDARD 2
#define KFACTOR_FINE     4
#define KFACTOR_SFINE    8

#define DATA_FROM_SCAN   0
#define DATA_FROM_FILE   1

#define WIDTH_A4         0   /* A4 page width (1728 pels) */
#define WIDTH_B4         1   /* B4 page width (2048 pels) */
#define WIDTH_A3         2   /* A3 page width (2432 pels) */

/* Page Length */
#define LEN_A4              0   /* A4 page length (297 mm) */
#define LEN_B4              1   /* B4 page length (364 mm) */
#define LEN_UNLTD           2   /* Unlimited page length */
#define LEN_LETTER          3   /* North American letter page length */
#define LEN_LEGAL           4   /* North American legal page length */

/* Minimum Scan Times */
#define MIN_SCAN_TIME       0   /* 0,5,10,20,40 msec min scan line time  */

/* T.30 timers */
#define AUTO_T4_TIMEOUT     3000    /* T4 time-out, automatic mode = 3 sec */
#define MANUAL_T4_TIMEOUT   4500    /* T4 time-out, manual mode = 4.5 sec */


#define REPEAT_2400 1              /* 3 tries everywhere else - already tried twice */


/* FIF table order */
enum {
    NSFtbl = 1,
    NSCtbl,
    NSStbl,
    SIDtbl,
    SUBtbl,
    PWDtbl,
    SEPtbl,
    CSItbl,
    TSItbl,
    CIGtbl,
    DIStbl,
    DCStbl,
    DTCtbl,
    PPStbl,
    PPRtbl,
    CTCtbl,
    EORtbl,
    RCPtbl
};

/* Miscellaneous Table Sizes */

#define TxPhaseDTableSize   10

#define FCFSendTableSize    20
#define FCFReceiveTableSize 19
#define RxCmdTableSize      23

#define EchoTblSize         8

#define MAX_PHONE_DIGITS    64     /* Max. Phone Number Digits, <255 */

#define MAX_ID_SIZE         24
#define LOG_TEXT_SIZE       81     /* sizeof(LOG)+overhead+'\0' */

#define ALWAYS_OFF  0
#define ALWAYS_ON   255

/* T.30 Fax Options */
typedef struct
{
    BOOL IgnoreJBIGL0;                   /* Ignore JBIG(T.85) option L0 capabilities */
    BOOL IgnoreJBIG;                     /* Ignore JBIG(T.85) capabilities */
    BOOL IgnoreFullColor;                /* Ignore Full Color mode */
    BOOL IgnoreJPEG;                     /* Ignore JPEG capabilities */
    BOOL IgnoreMMR;                      /* Ignore MMR capabilities */
    BOOL IgnoreMR;                       /* Ignore MR capabilities */
    BOOL IgnoreFileSystem;               /* Ignore file system */
    BOOL IgnoreECM;                      /* Ignore ECM */
    BOOL IgnoreEcho;                     /* Ignore echo */
    BOOL V29EPT;                         /* EPT on in V.29 mode */
    BOOL International;                  /* International mode */
} T_T30_OPTIONS;

/* T.30 Attributes for public use */
typedef struct
{
    UINT8       Direction;          /* Transmit or receive */
    UINT8       Memory;             /* Memory or non-memory operation */
    UINT8       Polling;            /* Polling operation */
    UINT8       Manual;             /* Manual or Auto Dialing */
    BOOL        TxAllRcvdFaxes;     /* Transmit all received faxes in memory */
} T_T30_MSG;

/* T.30 Attributes for public use */
typedef struct
{
    UINT8  CountryCode;                              /* T.35 country code */
    UINT8  FaxDialMode;                              /* DTMF:0 PULSE:1 */
    UINT8  FaxAnswerMode;                            /* Manual, Auto, Poll */
    UINT8  FaxAutoAnswerRings;                       /* Rings for auto answer */
    UINT8  FaxAttenuation;                           /* Fax Tx attenuation (dB, from 40 for -4.0 dBm ) */
    UINT8  FaxTxStartSpeed;                          /* Tx starting speed of modem */
    UINT8  FaxRxStartSpeed;                          /* Rx starting speed of modem */
    UINT8  FaxTxResolution;                          /* Tx Resolution */
    UINT8  FaxRxResolution;                          /* Rx Resolution */
    UINT8  FaxTxImageWidth;                          /* Tx image width */
    UINT8  FaxRxImageWidth;                          /* Rx image width */
    UINT8  FaxUnlimitLength;                         /* Fax unlimit length */
    UINT8  FaxMinimumScanTime;                       /* Minimum scan time */
    UINT16 FaxDelayedTxDocID;                        /* Fax delayed tx doc ID */
    BOOL   FaxLineRequest;                           /* Local voice request */
    BOOL   FaxRxPolling;                             /* Next call is rx by polling */
    UINT8  FaxEqualizer;                             /* Cable equalizer disable/enable/enable with other values */
    UINT8  RxMaxGain;                                /* Receive level dB selection */
    UINT8  FaxTAMTimeout;                            /* Time to wait for fax tone on TAM */
    UINT8  FaxLocalID[ StandardFIFSize ];            /* Local ID */
    UINT8  FaxRemoteID[ StandardFIFSize ];           /* Remote ID */
    UINT8  FaxTextHeader[ StandardFIFSize ];         /* Text header */
    UINT8  FaxVoiceSilence;                          /* Fax/Voice silence time */
    UINT8  PollTxSource;                             /* Polling allowed from scanner, memory, or both */
    UINT8  FaxVoiceRequest;                          /* Remote voice request */
    UINT8  FaxLogging;                               /* Should faxes be logged ? */
    T_T30_OPTIONS FaxOptions;                        /* Fax options */
    UINT8  SpeakerCallMode;                          /* Speaker ( call ) - off, on, on till T.30 phase */
    UINT8  SpeakerAnswerMode;                        /* Speaker ( answer ) - off, on, on till T.30 phase */
    UINT8  SpeakerVolume;                            /* Speaker volume */
    BOOL   ForceRxToMem;                             /* Forced receive to memory */
    BOOL   BufferedPrinting;                         /* Buffered printing enabled */
    BOOL   Buffered_DelAfterTx;                      /* Delete after transmitting on buffered transmit */
    BOOL   DelAfterTxAll;                            /* Delate afterwards on transmit all received faxes job */
    BOOL   IgnoreVoiceRequest;                       /* Should voice requests be honoured ? */
    UINT32 LogTypeInitialValue;                      /* Initial value of the Type field of the log */
    UINT8  LogErrorResult;                           /* Log.Result value on errors */
    UINT8  PrintLastLog;                             /* Should log be printed */
    UINT8  CoverPage;                                /* Should coverpage be sent ? */
    UINT8  CoverPageID;                              /* ID of the coverpage to send */
    BOOL   FaxForward;                               /* Fax forwarding selection */
    UINT8  FaxForwardNo[ LEN_PHONE_NO + 1 ];         /* Number to forward faxes to */
    UINT32 T1Timeout;                                /* T1 timeout in msecs */
    UINT32 T5Timeout;                                /* T5 timeout in msecs */
    UINT32 CEDDetectionTimeout;                      /* Called station detection timeout in msecs */
    UINT8  ECMMode;                                  /* Default ECM mode (0:none/1:256/2:64 bytes) */
    UINT8  NoOfTriesOnFallback;                      /* # of times to try each speed while fallingback */
    BOOL   HandleNAPaperSizes;                       /* Should North American paper sizes be advertised/handled */
    BOOL   ForceRxToMem_Color;                       /* Forced receive to memory for color fax */
    UINT8  TxImageType;                              /* Type of image to be sent, color, grayscale, or bi-level */
    UINT8  TxImageSource;                            /* Scan or File */
    UINT8  TxRequirePassword;                        /* Sender require password match */
    UINT8  RxRequirePassword;                        /* Receiver require password match */
    UINT8  FaxLocalTxPassword[ StandardFIFSize ];    /* Local sender password */
    UINT8  FaxLocalRxPassword[ StandardFIFSize ];    /* Local receiver password */
    UINT8  FaxRemotePassword[ StandardFIFSize ];     /* Remote password */
    UINT8  TxRequireSubaddress;                      /* Sender require subaddress match */
    UINT8  RxRequireSubaddress;                      /* Receiver require subaddress match */
    UINT8  FaxLocalTxSubaddress[ StandardFIFSize ];  /* Local sender subaddress */
    UINT8  FaxLocalRxSubaddress[ StandardFIFSize ];  /* Local receiver subaddress */
    UINT8  FaxRemoteSubaddress[ StandardFIFSize ];   /* Remote subaddress */
    UINT8  PollTxRequirePassword;                    /* Poll sender require password match */
    UINT8  PollRxRequirePassword;                    /* Poll receiver require password match */
    UINT8  PollLocalTxPassword[ StandardFIFSize ];   /* Poll local sender password */
    UINT8  PollLocalRxPassword[ StandardFIFSize ];   /* Poll local receiver password */
    UINT8  PollRemotePassword[ StandardFIFSize ];    /* Poll remote password */
    UINT8  PollTxRequireSubaddress;                  /* Poll sender require subaddress match */
    UINT8  PollRxRequireSubaddress;                  /* Poll receiver require subaddress match */
    UINT8  PollLocalTxSubaddress[ StandardFIFSize ]; /* Poll local sender subaddress */
    UINT8  PollLocalRxSubaddress[ StandardFIFSize ]; /* Poll local receiver subaddress */
    UINT8  PollRemoteSubaddress[ StandardFIFSize ];  /* Poll remote subaddress */
    BOOL   FaxEnableNSF;                             /* Enable NFS */
    UINT8  FaxLocalNSF[ StandardFIFSize ];           /* Local NSF */
    UINT8  FaxLocalNSS[ StandardFIFSize ];           /* Local NSS */
    UINT8  FaxLocalNSC[ StandardFIFSize ];           /* Local NSC */
    UINT8  FaxRemoteNSF[ StandardFIFSize ];          /* Remote NSF */
    UINT8  FaxRemoteNSS[ StandardFIFSize ];          /* Remote NSS */
    UINT8  FaxRemoteNSC[ StandardFIFSize ];          /* Remote NSC */
    UINT8  fax_retry_number;                         /* number of retry time */
    UINT8  fax_retry_interval;                       /* retry interval */
    UINT8  fax_brightness;                           /* scan brightness */
    UINT8  fax_contrast;                             /* scan contrast */
    BOOL   fax_enable_tti;                           /* enable/dsable TTI */
    BOOL   SpeakerOffV8;                             /* Speaker off at V.8 in Phase A */
    UINT8  RxV34AbortBadEQM;                         /* Abort V.34 receiving if bad EQM over 6 second */
#if __SUPPORT_MANUAL_RECEIVE__
    UINT8  ManualReceive;                            /* 0: Auto Answer Mode, 1: Manual Answer Mode */
    UINT8  FaxReceiveSource;                         /* Received allowed from machine, external phone */
#else
#endif
} T_T30_ATTRIBUTE;

typedef struct rxcmd
{
    UINT8 command;
    T_T30_STATES next_state;
    void (*fxn)(void);
} T_RX_CMD_TABLE;

extern T_T30_MSG t30_msg;
extern T_T30_ATTRIBUTE t30_attribute;

/* T.30 Table Externals */
extern const UINT8 FCFSendTable[];
extern UINT8 *FIFSendTable[];
extern const UINT8 ByteCntTbl[];
extern const UINT8 CtrlTbl[];
extern const UINT8 FCFReceiveTable[];
extern const UINT8 MaxSizeTbl[];
extern const UINT8 MinSizeTbl[];
extern UINT8 *FIFReceiveTable[];
extern const UINT8 LocalNSTD[];
extern const UINT8 NSTDHeader[];
extern const UINT8 LocalNSTDCap[];
extern const UINT8 LocalDIS[];
extern const UINT8 FallbackTbl[];
extern const UINT8 ScanTimeTbl[];
extern const UINT8 ScanTimeTbl2[];
extern const UINT8 T4MinBytesTbl[];
extern const UINT8 SpeedIndexTbl[];
extern const UINT8 SpeedConvertTbl[];
extern const UINT8 DCS_SpeedBits[];
extern const UINT8 TxPhaseDTable[];
extern const T_T30_STATES TxPhaseDNextState[][4];
extern const UINT16 T30_T4MaxBytesTbl[];
extern const UINT8 DIS_SpeedBits[];
extern const UINT8 EchoTbl[];
extern const T_RX_CMD_TABLE RxCmdTable[];
extern const UINT8 VReqErrorCode[];
extern const T_T30_STATES VReqNextState[];

extern const UINT16 t30_len_table[5][5];
extern const UINT16 t30_width_table[3][5];
extern const UINT16 t30_vres_table[5];
extern const UINT16 t30_hres_table[5];

/* T.30 Miscellaneous Externals */
extern UINT8 T30AlertLocalOp(void);
extern void T30ShutdownJob(void);
extern UINT8 T30GetCommand(void);
extern UINT8 T30CheckCommand(void);
extern UINT8 T30GetResponse(void);
extern ERCODE T30InitJob(void);
extern void T30InitVars(void);
extern void T30SendFCF(UINT8);
extern void T30StopFax(void);
extern UINT8 T30StoreFrame(void);
extern void T30AllocateTimers(void);
extern void T30Task(UINT32);
extern BOOL	T30RxCheckMultiPage(UINT8 *post_msg);

/* T.30 Receive Externals */
extern UINT8 T30RxCheckErrors(void);
extern void T30RxTrainingCheck(UINT8 send_fcf);
extern void T30RxReadDCS(void);
extern void T30RxPhaseA(void);
extern void T30RxPhaseB(void);
extern void T30RxSignalRec(void);
extern void T30RxPhaseC(void);
extern void T30RxPhaseBDCommand(void);
extern void T30RxPhaseBDProcessCommand(void);
extern void T30RxPhaseE(void);
extern void T30RxProcessBadCommand(void);
extern void T30RxProcessDCN(void);
extern void T30RxProcessDCS(void);
extern void T30RxProcessDIS(void);
extern void T30RxProcessDTC(void);
extern void T30RxProcessEOM(void);
extern void T30RxProcessMPS(void);
extern void T30RxProcessEOP(void);
extern void T30RxProcessPRIQ(void);
extern void T30RxProcessPWD(void);
extern void T30RxProcessSID(void);
extern void T30RxProcessSUB(void);
extern void T30RxProcessSEP(void);
extern void T30RxProcessNSC(void);
extern void T30RxProcessNSS(void);
extern void T30RxInitPage(void);

/* T.30 Transmit Externals */
extern UINT8 T30TxFallback(void);
extern void T30TxMakeDCS(void);
extern T_T30_STATES T30TxProcessRTN(UINT8 table_index);
extern void T30TxPhaseA(void);
extern void T30TxPhaseB(void);
extern void T30TxSetMode(void);
extern void T30TxPhaseC(void);
extern void T30TxPhaseD(void);
extern void T30TxPhaseD2(void);
extern void T30TxPhaseE(void);
extern void T30TxVoiceReq(void);
extern UINT8 T30TxCheckModeChange(void);
extern T_T30_STATES T30TxPhaseDProcessResponse( UINT8 response, UINT8 table_index );
extern void T30TxInitPage(void);
extern UINT8 T30TxCheckErrors(void);
