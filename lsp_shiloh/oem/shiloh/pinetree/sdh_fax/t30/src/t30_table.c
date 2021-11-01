/*
 *  $RCSfile: t30_table.c,v $
 * $Revision: 1.40 $
 *   $Author: miurat $
 *     $Date: 2012/01/11 01:51:16 $
 */
/************************************************************************
 *
 *      t30_table.c
 *
 *  T.30 Tables (Transmit and Receive)
 *
 *  Copyright 2004 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "SDHFaxCommonHeader.h"
#include "cxshell.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "ERROR.H"
#include "MODEMX.H"
#include "T30X.H"
#include "HDLC.H"
#include "ECMX.H"
#include "ECM.H"
#include "t30.h"

#include "t35_cty.h"
#include "oem_cty.h"

void t30_init_params( CtryPrmsStruct *Ctry );

T_T30_ATTRIBUTE t30_attribute;

/*----------------------------------------------------------------------------*/

/* T.30 Attribute structure */
static const T_T30_ATTRIBUTE t30_default_attribute =
{
	T35_UNITED_STATES,      /* T.35 country code */
	DIAL_DTMF,              /* Dial mode */
	AUTO_ANSWER,            /* Answer mode */
	2,                      /* Rings for auto answer */
	90,                     /* Tx attenuation (dB, from 10 for -1.0 dBm) */
	T30_TX_START_SPEED,     /* Tx start speed */
	T30_RX_START_SPEED,     /* Rx start speed */
	RES_STANDARD,           /* Tx Resolution */
	RES_300,                /* Rx Resolution */
	WIDTH_A4,               /* Tx data width */
	WIDTH_A4,               /* Rx data width */
	TRUE,                   /* Unlimit length */
	0,                      /* Minimum scan time */
	0,                      /* Fax delayed tx doc ID */
	0,                      /* Local voice request */
	0,                      /* Next call is rx by polling */
	0,                      /* Enable/disable cable equalizer */
	6,                      /* Receive level - default */
	0,                      /* Time to wait for fax tone on TAM */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Local ID */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Remote ID */
    "Conexant Systems", /* Text header */
	0,                      /* Fax/Voice silence time */
	TXPOLLFROM_NONE,        /* Polling allowed from scanner, memory, or both */
	0,                      /* Remote voice request */
	1,                      /* Fax logging is enabled */
	{                       /* Fax options */
		FALSE,              /* Ignore JBIG(T.85) option L0 capabilities */
		FALSE,              /* Ignore JBIG(T.85) capabilities */
		TRUE,               /* Ignore Full Color mode */
		TRUE,               /* Ignore JPEG capabilities */
		FALSE,              /* Ignore MMR capabilities */
		FALSE,              /* Ignore MR capabilities */
		FALSE,              /* Ignore file system */
		FALSE,              /* Ignore ECM */
		FALSE,              /* Ignore echo */
		FALSE,              /* EPT on in V.29 mode by T.4 2003 */
		FALSE               /* International mode */
	},
	ALWAYS_OFF,             /* Speaker ( call ) will be on till this state */
	ALWAYS_OFF,             /* Speaker ( answer ) will be on till this state */
	1,                      /* Speaker volume */
	FALSE,                  /* Force receive to memory */
	TRUE,                   /* Buffered printing enabled */
	TRUE,                   /* Delete page after successful buffered transmit */
	TRUE,                   /* Delete after successful transmission of received faxes */
	FALSE,                  /* Do not honour voice requests */
	0,                      /* Initial value of the Type field of the log */
	0, /*LOG1_FAIL,            Log.Result value if fax fails */
	0, /*CP_FAXLOG_ON_POLLED_AND_ERROR,  Print faxlog only when polled or transmit errors */
	0,                      /* Do not send coverpage */
	0,                      /* Send coverpage ID 0 if coverpage sent */
	FALSE,                  /* Fax forwarding turned off */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Forwarding No. */
	T1_TIMEOUT,             /* T1 is 35+-5 seconds according to T.30 standard */
	60000,                  /* T5 is 60 seconds according to T30 standard */
	38000,                  /* T1 timeout according to T.30 standard */
	ECM_256,                /* Default ECM mode is 256 byte mode */
	1,                      /* Try each speed once */
	TRUE,                   /* Handle North American paper sizes */
	TRUE,                   /* Force receive color fax to memory */
	IMG_BILEVEL,            /* Send bilevel images by default */
	DATA_FROM_SCAN,         /* Image source */
	0,                                           /* Sender require password */
	0,                                           /* Receiver require password */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Sender password */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Receiver password */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Remote password */
	0,                                           /* Sender require subaddress */
	0,                                           /* Receiver require subaddress */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Sender subaddress */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Receiver subaddress */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Remote subaddress */
	0,                                           /* Poll Sender require password */
	0,                                           /* Poll Receiver require password */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Poll Sender password */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Poll Receiver password */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Poll Remote password */
	0,                                           /* Poll Sender require subaddress */
	0,                                           /* Poll Receiver require subaddress */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Poll Sender subaddress */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Poll Receiver subaddress */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Poll Remote subaddress */
	FALSE,                                       /* NFS enable */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Local NSF */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Local NSC */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Local NSS */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Remote NSF */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Remote NSC */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, /* Remote NSS */
	3,                                           /* number of fax retry */
	3,                                           /* minute for retry interval */
	0,                                           /* Brightness */
	0,                                           /* Contrast */
	1,                                           /* tti on */
	TRUE,                                        /* Speaker off at V.8 in Phase A */
#if __SUPPORT_MANUAL_RECEIVE__
    0,                                           /* Abort V.34 receiving if bad EQM over 6 second, bit0:EQM, bit1:RLSD */
    0,                                           /* Manual receive */
    RECEIVE_SOURCE_MACHINE                       /* Fax receive source */
#else
    0                                            /* Abort V.34 receiving if bad EQM over 6 second, bit0:EQM, bit1:RLSD */
#endif
};


/************************/
/* Miscellaneous Tables */
/************************/

/* Vertical resolution in DPI per mode */
const UINT16 t30_vres_table[ 5 ] = {
	 98,                  /* RES_STANDARD */
	196,                  /* RES_FINE */
	391,                  /* RES_SUPERFINE */
	304,                  /* RES_300 */
	391                   /* RES_ULTRAFINE */
};

/* Horizontal resolution in DPI per mode */
const UINT16 t30_hres_table[ 5 ] = {
	203,                  /* RES_STANDARD */
	203,                  /* RES_FINE */
	203,                  /* RES_SUPERFINE */
	304,                  /* RES_300 */
	405                   /* RES_ULTRAFINE */
};

/* Number of lines per resolution based on length negotiated */
const UINT16 t30_len_table[ 5 ][ 5 ] = {
	{ 1143, 2286, 4573, 3430, 4573 },     /* A4 */
	{ 1401, 2802, 5600, 4204, 5605 },     /* B4 */
	{ (UINT16)-1, (UINT16)-1, (UINT16)-1, (UINT16)-1, (UINT16)-1 }, /* Unlimited */
	{ 1078, 2156, 4301, 3300, 4301 },     /* Letter */
	{ 1372, 2744, 5474, 4200, 5474 }      /* Legal */
};

/* Number of pixels per line based on resolution negotiated */
const UINT16 t30_width_table[ 3 ][ 5 ] = {
	{ 1728, 1728, 1728, 2592, 3456 },    /* A4 */
	{ 2048, 2048, 2048, 3072, 4096 },    /* B4 */
	{ 2432, 2432, 2432, 3648, 4864 }     /* A3 */
};

const UINT8 FCFSendTable[ FCFSendTableSize ] =  {
	0,
	NSF, NSC, NSS,
	SID, SUB, PWD, SEP,
	CSI, TSI, CIG,
	DIS, DCS, DTC,
	PPS, PPR, CTC,
	EOR, RCP, CRP
};

UINT8 *FIFSendTable[ FCFSendTableSize ] =  {
	0,
	0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0
};

const UINT8 ByteCntTbl[ FCFSendTableSize ] =  {
	0,
	20, 20, 20,
	20, 20, 20, 20,
	20, 20, 20,
	LocalDISSize, LocalDISSize, LocalDISSize,
	4, 32, 2,
	1, 0, 0
};

const UINT8 CtrlTbl[ FCFSendTableSize ] =  {
	0x13,
	0x03, 0x13, 0x03,
	0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03,
	0x13, 0x13, 0x13,
	0x13, 0x13, 0x13,
	0x13, 0x03, 0x13
};

const UINT8 FCFReceiveTable[ FCFReceiveTableSize ] =  {
	0,
	NSF, NSC, NSS,
	SID, SUB, PWD, SEP,
	CSI, TSI, CIG,
	DIS, DCS, DTC,
	PPS, PPR, CTC,
	EOR, RCP
};

const UINT8 MaxSizeTbl[ FCFReceiveTableSize ] =  {
	0,
	20, 20, 20,
	20, 20, 20, 20,
	20, 20, 20,
	RemoteCapSize, RemoteCapSize, RemoteCapSize,
	4, 32, 2,
	1, 0
};

const UINT8 MinSizeTbl[ FCFReceiveTableSize ] =  {
	0,
	1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1,
	3, 3, 3,
	4, 32, 2,
	1, 0
};

UINT8 *FIFReceiveTable[ FCFReceiveTableSize ] =  {
	0,
	0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0
};


/*******************/
/* Transmit Tables */
/*******************/

/* NSTD Header and Local Capabilities Tables */
const UINT8 NSTDHeader[] = { 0xBA, 0x20, 0x00, 0x00 };

const UINT8 LocalDIS[ LocalDISSize ] = {
	0x00,       /* No G1 or G2 */
	0x02,       /* MH, Standard, RX */
	0x80,       /* 20 msec, A4(297mm), 215mm */
	0x80,       /* Extend field */
	0x80,       /* Extend field */
	0x98,       /* mm, inch */
	0x80,       /* Extend field */
	0x80,       /* Extend field */
	0x80,       /* Extend field */
	0x00        /* Extend field */
};


const UINT8 FallbackTbl[ 11 ] = {
	0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};


const UINT8 ScanTimeTbl[ 16 ] = {
#if (MIN_SCAN_TIME==0)
	0x00, 0x00, 0x10, 0x10,
	0x20, 0x20, 0x00, 0x20,
	0x40, 0x40, 0x40, 0x00,
	0x20, 0x10, 0x70, 0x70
#elif (MIN_SCAN_TIME==5)
	0x00, 0x00, 0x10, 0x10,
	0x20, 0x20, 0x00, 0x20,
	0x40, 0x40, 0x40, 0x00,
	0x20, 0x10, 0x10, 0x10
#elif (MIN_SCAN_TIME==10)
	0x00, 0x00, 0x20, 0x10,
	0x20, 0x20, 0x00, 0x20,
	0x40, 0x40, 0x40, 0x00,
	0x20, 0x10, 0x20, 0x10
#elif (MIN_SCAN_TIME==20)
	/* 10ms min scan time selected when possible
	   even though advertising capibilities (DIS) as 20/10 ms */
	0x00, 0x00, 0x20, 0x20,
	0x20, 0x20, 0x00, 0x20,
	0x40, 0x40, 0x40, 0x00,
	0x20, 0x20, 0x20, 0x20
#elif (MIN_SCAN_TIME==40)
	0x40, 0x00, 0x40, 0x00,
	0x40, 0x00, 0x40, 0x00,
	0x40, 0x40, 0x40, 0x00,
	0x40, 0x00, 0x40, 0x00
#endif
};


/* The minimum number of bytes that must be transmitted per line. */
/* The bytes are in order of: */
/* reserved, V.27 2400, V.27 4800, V.29 7200, V.29 9600 */
/* V.17 7200, V.17 9600, V.17 12000, V.17 14400 */
/* reserved, reserved, reserved, reserved, reserved, reserved, reserved */

const UINT8 T4MinBytesTbl[ 128 ] =  {
	0,  6, 12, 18, 24, 18, 24, 30, 36, 0,0,0,0,0,0,0,    /* 20 msec */
	0,  2,  3,  5,  6,  5,  6,  8,  9, 0,0,0,0,0,0,0,    /* 5 msec */
	0,  3,  6,  9, 12,  9, 12, 15, 18, 0,0,0,0,0,0,0,    /* 10 msec */
	0,  0,  0,  0,  0,  0,  0,  0,  0, 0,0,0,0,0,0,0,    /* illegal */
	0, 12, 24, 36, 48, 36, 48, 60, 72, 0,0,0,0,0,0,0,    /* 40 msec */
	0,  0,  0,  0,  0,  0,  0,  0,  0, 0,0,0,0,0,0,0,    /* illegal */
	0,  0,  0,  0,  0,  0,  0,  0,  0, 0,0,0,0,0,0,0,    /* illegal */
	0,  0,  0,  0,  0,  0,  0,  0,  0, 0,0,0,0,0,0,0     /* illegal */
};


const UINT8 SpeedIndexTbl[ 16 ] =  {
	1, 4, 2, 4, 0, 0, 0, 4, 0, 0, 0, 8, 0, 0, 0, 0
};


const UINT8 SpeedConvertTbl[ 16 ] =
{
	1, 4, 2, 3, 4, 0, 4, 0, 8, 6, 7, 5, 0, 0, 0, 0
};


const UINT8 DCS_SpeedBits[ 9 ] = {
	0x00, 0x00, 0x08, 0x0C, 0x04, 0x2C, 0x24, 0x28, 0x20
};


const UINT8 TxPhaseDTable[ TxPhaseDTableSize ] =  {
	CRP, CTR, ERR, MCF, PIP, PIN, PPR, RNR, RTP, RTN
};

/*          EOP                   MPS                   EOM              NULL
      =============         ===============       ==============        ======
*/
const T_T30_STATES TxPhaseDNextState[ TxPhaseDTableSize ][ 4 ] =  {
	{ STATE_TX_PHASE_D2,    STATE_TX_PHASE_D2,    STATE_TX_PHASE_D2,    STATE_TX_PHASE_D2 },    /* CRP */
	{ STATE_TX_PHASE_C_ECM, STATE_TX_PHASE_C_ECM, STATE_TX_PHASE_C_ECM, STATE_TX_PHASE_C_ECM }, /* CTR */
	{ STATE_TX_PHASE_E,     STATE_TX_PHASE_C,     STATE_TX_PHASE_C,     STATE_TX_PHASE_C },     /* ERR */
	{ STATE_TX_PHASE_E,     STATE_TX_PHASE_C,     STATE_TX_PHASE_B,     STATE_TX_PHASE_C },     /* MCF */
	{ STATE_TX_VOICE_REQ,   STATE_TX_VOICE_REQ,   STATE_TX_VOICE_REQ,   STATE_TX_PHASE_E },     /* PIP */
	{ STATE_TX_VOICE_REQ,   STATE_TX_VOICE_REQ,   STATE_TX_VOICE_REQ,   STATE_TX_PHASE_E },     /* PIN */
	{ STATE_TX_PHASE_C_ECM, STATE_TX_PHASE_C_ECM, STATE_TX_PHASE_C_ECM, STATE_TX_PHASE_C_ECM }, /* PPR */
	{ STATE_TX_PHASE_D2,    STATE_TX_PHASE_D2,    STATE_TX_PHASE_D2,    STATE_TX_PHASE_D2 },    /* RNR */
	{ STATE_TX_PHASE_E,     STATE_TX_SET_MODE,    STATE_TX_PHASE_B,     STATE_TX_PHASE_E },     /* RTP */
    { STATE_TX_SET_MODE,     STATE_TX_SET_MODE,    STATE_TX_PHASE_B,     STATE_TX_PHASE_E }      /* RTN */
};

const UINT8 VReqErrorCode[ 8 ] =  {
    FAX_ERROR_BAD_RX_ERROR_5, 0,     /* EOP-PIN, EOP-PIP */
	0, 0,                /* EOM-PIN, EOM-PIP */
	0, 0,                /* MPS-PIN, MPS-PIP */
    FAX_ERROR_BAD_RX_ERROR_6, 0      /* PRI_EOP-PIN, PRI_EOP-PIP */
};

const T_T30_STATES VReqNextState[ 8 ] = {
	STATE_TX_PHASE_E,  STATE_TX_PHASE_E,     /* EOP-PIN, EOP-PIP */
	STATE_TX_SET_MODE, STATE_TX_SET_MODE,    /* EOM-PIN, EOM-PIP */
	STATE_TX_SET_MODE, STATE_TX_SET_MODE,    /* MPS-PIN, MPS-PIP */
	STATE_TX_PHASE_E,  STATE_TX_PHASE_E      /* PRI_EOP-PIN, PRI_EOP-PIP */
};


/*******************/
/* Receive Tables */
/*******************/


/* 13 seconds of data per line allowed in non-ECM, 39 seconds in ECM */
const UINT16 T30_T4MaxBytesTbl[ 32 ] = {
	0,       /* V.21 300 bps - not used */
	0,       /* V.21 300 bps - not used */
	0x0F3C,  /* V.27 2400 bps */
	0x2DB4,  /* V.27 2400 bps */
	0x1E78,  /* V.27 4800 bps */
	0x5B68,  /* V.27 4800 bps */
	0x2DB4,  /* V.29 7200 bps */
	0x891C,  /* V.29 7200 bps */
	0x3CF0,  /* V.29 9600 bps */
	0xB6D0,  /* V.29 9600 bps */
	0x2DB4,  /* V.17 7200 bps */
	0x891C,  /* V.17 7200 bps */
	0x3CF0,  /* V.17 9600 bps */
	0xB6D0,  /* V.17 9600 bps */
	0x4C2C,  /* V.17 12000 bps */
	0xE484,  /* V.17 12000 bps */
	0x5B68,  /* V.17 14400 bps */
	0xFEFE,  /* V.17 14400 bps - only 36.25 seconds */

	0x2DB4,  /* V.34 2400 bps */
	0x5B68,  /* V.34 4800 bps */
	0x891C,  /* V.34 7200 bps */
	0xB6D0,  /* V.34 9600 bps */
	0xE484,  /* V.34 12000 bps */
	0xFEFE,  /* V.34 14400 bps - only 36.25 seconds */
	0xFEFE,  /* V.34 16800 bps - only 31.08 seconds */
	0xFEFE,  /* V.34 19200 bps - only 27.20 seconds */
	0xFEFE,  /* V.34 21600 bps - only 24.18 seconds */
	0xFEFE,  /* V.34 24000 bps - only 21.76 seconds */
	0xFEFE,  /* V.34 26400 bps - only 19.78 seconds */
	0xFEFE,  /* V.34 28800 bps - only 18.13 seconds */
	0xFEFE,  /* V.34 31200 bps - only 16.74 seconds */
	0xFEFE   /* V.34 33600 bps - only 15.54 seconds */
};


const UINT8 DIS_SpeedBits[ 23 ] =  {
	0x00, /* V.21 - not used */
	0x00, /* V.27ter fallback 2400 */
	0x08, /* V.27ter 4800 */
	0x0C, /* V.27ter and V.29 7200 */
	0x04, /* V.29 9600 */
	0x2C, /* V.27ter, V.29 and V.17  7200 */
	0x2C, /* V.27ter, V.29 and V.17  9600 */
	0x2C, /* V.27ter, V.29 and V.17 12000 */
	0x2C, /* V.27ter, V.29 and V.17 14400 */
	0x2C, /* V.27ter, V.29, V.17 and V.34  2400 */
	0x2C, /* V.27ter, V.29, V.17 and V.34  4800 */
	0x2C, /* V.27ter, V.29, V.17 and V.34  7200 */
	0x2C, /* V.27ter, V.29, V.17 and V.34  9600 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 12000 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 14400 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 16800 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 19200 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 21600 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 24000 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 26400 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 28800 */
	0x2C, /* V.27ter, V.29, V.17 and V.34 31200 */
	0x2C  /* V.27ter, V.29, V.17 and V.34 33600 */
};


/*  Table of RX responses that we could have received an echo of that would */
/*  require us to get another command, instead of doing signal recognition. */
const UINT8 EchoTbl[ EchoTblSize ] =  {
	CIG, CSI, CTR, DIS, DTC, FTT, RNR, NSF
};

const T_RX_CMD_TABLE RxCmdTable[ RxCmdTableSize ] =  {
	{ 0,        STATE_RX_PHASE_E,               T30RxProcessBadCommand },
	{ DCN,      STATE_RX_DISCONN,               T30RxProcessDCN },
	{ DCS,      STATE_RX_PHASE_B_SIGNAL_REC,    T30RxProcessDCS },
	{ TSI,      STATE_RX_PHASE_B_SIGNAL_REC,    T30RxProcessDCS },  /* Process same as DCS */
	{ DIS,      STATE_TX_SET_MODE,              T30RxProcessDIS },
	{ CSI,      STATE_TX_SET_MODE,              T30RxProcessDIS },  /* Process same as DIS */
	{ DTC,      STATE_TX_SET_MODE,              T30RxProcessDTC },
	{ PWD,      STATE_TX_SET_MODE,              T30RxProcessPWD },
	{ SID,      STATE_TX_SET_MODE,              T30RxProcessSID },
	{ SUB,      STATE_TX_SET_MODE,              T30RxProcessSUB },
	{ SEP,      STATE_TX_SET_MODE,              T30RxProcessSEP },
	{ EOM,      STATE_RX_PHASE_D_SIGNAL_REC,    T30RxProcessEOM },
	{ MPS,      STATE_RX_PHASE_D_SIGNAL_REC,    T30RxProcessMPS },
	{ EOP,      STATE_RX_PHASE_D,               T30RxProcessEOP },
	{ PRI_EOM,  STATE_RX_PHASE_D,               T30RxProcessPRIQ },
	{ PRI_MPS,  STATE_RX_PHASE_D,               T30RxProcessPRIQ },
	{ PRI_EOP,  STATE_RX_PHASE_D,               T30RxProcessPRIQ },
	{ PPS,      STATE_RX_PHASE_D_SIGNAL_REC,    ECMRxProcessPPS },
	{ RR,       STATE_RX_PHASE_D_SIGNAL_REC,    ECMRxProcessRR },
	{ CTC,      STATE_RX_PHASE_D_SIGNAL_REC,    ECMRxProcessCTC },
	{ EOR,      STATE_RX_PHASE_D_SIGNAL_REC,    ECMRxProcessEOR },
	{ NSC,      STATE_TX_SET_MODE,              T30RxProcessNSC },
	{ NSS,      STATE_RX_PHASE_B_SIGNAL_REC,    T30RxProcessNSS }
};


void t30_init_params( CtryPrmsStruct *Ctry )
{
	t30_attribute = t30_default_attribute;

	ModemControl->TCFThreshold = 10;  // ratio of ok to error

	if ( Ctry != 0 )
	{
		/* update t30 parameters */
		t30_attribute.FaxAttenuation     = Ctry->Txlevel.Default;
		ModemControl->Options.FaxTxLevel = Ctry->Txlevel.Default;
		ModemControl->Options.TxCompensation = Ctry->Txlevel.Compensation;

		t30_attribute.FaxOptions.V29EPT  = Ctry->Option.EPTforV29;
		ModemControl->Options.ForceEPTOn = Ctry->Option.EPTforV29;
	}
}

