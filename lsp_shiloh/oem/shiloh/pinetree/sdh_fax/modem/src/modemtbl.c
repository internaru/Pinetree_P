/*
 *  $RCSfile: modemtbl.c,v $
 * $Revision: 1.13 $
 *   $Author: miurat $
 *     $Date: 2012/01/06 22:42:28 $
 */
/************************************************************************
 *
 *      modemtbl.c
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "cxshell.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "ERROR.H"
#include "MODEMX.H"
#include "T30X.H"
#include "ECMX.H"
#include "ECM.H"
#include "HDLC.H"
#include "modem336.h"


/* Modem configuration bits */
/* 7-5: reserved */
/* 4: 1 = IA sleep control required */
/* 3: 1 = 12 dB gain control required */
/* 2: signal recognition type */
/* 1: 1 = voice compatible */
/* 0: 1 = V.17 compatible */

/*   Modem Parameters */

const UINT16 DeltaMaxGain = 0x1983;     /* Amount to subtract from gain word */
/*  for new max gain (default ~= 10 ) */
const UINT8 VoiceECM = 1;               /* Voice error correction (0=OFF, 1=ON) */
const UINT16 VOXOnThresh  = 0xFFF2;     /* Voice turn-on threshold */
const UINT16 VOXOffThresh = 0xFFEE;     /* Voice turn-off threshold */


const UINT16 ModemRxMaxGainTbl [ 8 ] =
{
	0x0000,     /* - 50 dB */
	0x028F,     /* - 49 dB */
	0x051E,     /* - 48 dB */
	0x07AE,     /* - 47 dB */
	0x0A3D,     /* - 46 dB */
	0x0CCC,     /* - 45 dB */
	0x0FC0,     /* ~ - 44 dB.  The dafult MAXG value is used instead of - 44dB */
	0x11EB,     /* - 43 dB */
};


/*-----------------------------------------------------------------------
*      Tone Frequency and Level Tables for 9600 Hz sampling rate
*
*  The following tables are in the order: need to match MODEM_TX_TONE_TBL_INDEX in modemx.h
*
* Tone 1 freq, Tone 2 freq, Tone 1 level, Tone 2 level
*-----------------------------------------------------------------------*/

const UINT16 TxToneTbl[] =
{
	0x1918, 0x23A0, 0x3380, 0x3F65,     /* DTMF 0 */
	0x1296, 0x203D, 0x3720, 0x4020,     /* DTMF 1 */
	0x1296, 0x23A0, 0x3720, 0x3F65,     /* DTMF 2 */
	0x1296, 0x2763, 0x3720, 0x3FB0,     /* DTMF 3 */
	0x1488, 0x203D, 0x3500, 0x4020,     /* DTMF 4 */
	0x1488, 0x23A0, 0x3500, 0x3F65,     /* DTMF 5 */
	0x1488, 0x2763, 0x3500, 0x3FB0,     /* DTMF 6 */
	0x16B8, 0x203D, 0x3400, 0x4020,     /* DTMF 7 */
	0x16B8, 0x23A0, 0x3400, 0x3F65,     /* DTMF 8 */
	0x16B8, 0x2763, 0x3400, 0x3FB0,     /* DTMF 9 */
	0x1918, 0x203D, 0x3380, 0x4020,     /* DTMF * */
	0x1918, 0x2763, 0x3380, 0x3FB0,     /* DTMF # */
	0x1296, 0x2B8C, 0x36A0, 0x4060,     /* DTMF A */
	0x1488, 0x2B8C, 0x3500, 0x4060,     /* DTMF B */
	0x16B8, 0x2B8C, 0x3440, 0x4060,     /* DTMF C */
	0x1918, 0x2B8C, 0x3380, 0x4060,     /* DTMF D */
	0x0C52, 0x0000, 0x220A, 0x0000,     /*  462 HZ */
	0x1D55, 0x0000, 0x220A, 0x0000,     /* 1100 HZ */
	0x3800, 0x0000, 0x220A, 0x0000,     /* 2100 HZ */
	0x2C00, 0x0000, 0x220A, 0x0000,     /* 1650 HZ */
	0x3155, 0x0000, 0x220A, 0x0000,     /* 1850 HZ */
	0x40AB, 0x0000, 0x220A, 0x0000,     /* 2425 HZ */
	/* New Tones for Phone Answer Mode 1/9/99 */
	0x0AAA, 0x0000, 0x220A, 0x0000,     /* #22:  400 Hz */
	0x1000, 0x0000, 0x220A, 0x0000,     /* #23:  600 Hz */
	0x1555, 0x0000, 0x220A, 0x0000,     /* #24:  800 Hz */
	0x2555, 0x0000, 0x220A, 0x0000,     /* #25: 1400 Hz */
	0x3000, 0x0000, 0x220A, 0x0000,     /* #26: 1800 Hz */
	0x0000, 0x0000, 0x0000, 0x0000      /* #27:    0 Hz */
};

/*-----------------------------------------------------------------------
*       DTMF Output Level Adjust Table
*-----------------------------------------------------------------------*/
const float TxDTMFTbl[ 7 ] =
{
	0.708,   /* -3dB */
	0.794,   /* -2dB */
	0.891,   /* -1dB */
	1.000,   /*  0dB */
	1.122,   /* +1dB */
	1.259,   /* +2dB */
	1.413    /* +3dB */
};


const DTMF_COMP_LEVEL DTMFCompLevel[ 16 ] =
{
	{ 1, 6 },    // 0
	{ 3, 7 },    // 1
	{ 3, 6 },    // 2
	{ 3, 4 },    // 3
	{ 2, 8 },    // 4
	{ 2, 7 },    // 5
	{ 2, 5 },    // 6
	{ 1, 8 },    // 7
	{ 1, 7 },    // 8
	{ 1, 4 },    // 9
	{ 1, 8 },    // *
	{ 1, 4 },    // #
	{ 2, 1 },    // A
	{ 2, 1 },    // B
	{ 2, 1 },    // C
	{ 2, 1 }     // D
};

const TX_COMP_LEVEL TxCompLevel[ 8 ] =
{
	{ 3, 15 },  // V8_MOD
	{ 3, 21 },  // V21_MOD
	{ 3, 18 },  // V27_MOD
	{ 4, 20 },  // V29_MOD
	{ 3, 17 },  // V17_MOD
	{ 6, 21 },  // V34_MOD
	{ 4, 27 },  // TONE1100
	{ 4, 15 }   // Tone2100
};

/*-----------------------------------------------------------------------
*      Tone Frequency and Level Tables for 8000 Hz sampling rate
*
*  The following tables are in the order: need to match MODEM_TX_TONE_TBL_INDEX in modemx.h
*
* Tone 1 freq, Tone 2 freq, Tone 1 level, Tone 2 level
*-----------------------------------------------------------------------*/
const UINT16 TxSpkrToneTbl[] =
{
	0x1E1D, 0x2AC1, 0x65AB, 0x7FFF,     /* DTMF 0 */
	0x164E, 0x26B0, 0x65AB, 0x7FFF,     /* DTMF 1 */
	0x164E, 0x2AC1, 0x65AB, 0x7FFF,     /* DTMF 2 */
	0x164E, 0x2F44, 0x65AB, 0x7FFF,     /* DTMF 3 */
	0x18A4, 0x26B0, 0x65AB, 0x7FFF,     /* DTMF 4 */
	0x18A4, 0x2AC1, 0x65AB, 0x7FFF,     /* DTMF 5 */
	0x18A4, 0x2F44, 0x65AB, 0x7FFF,     /* DTMF 6 */
	0x1B44, 0x26B0, 0x65AB, 0x7FFF,     /* DTMF 7 */
	0x1B44, 0x2AC1, 0x65AB, 0x7FFF,     /* DTMF 8 */
	0x1B44, 0x2F44, 0x65AB, 0x7FFF,     /* DTMF 9 */
	0x1E1D, 0x26B0, 0x65AB, 0x7FFF,     /* DTMF * */
	0x1E1D, 0x2F44, 0x65AB, 0x7FFF,     /* DTMF # */
	0x164E, 0x3442, 0x65AB, 0x7FFF,     /* DTMF A */
	0x18A4, 0x3442, 0x65AB, 0x7FFF,     /* DTMF B */
	0x1B44, 0x3442, 0x65AB, 0x7FFF,     /* DTMF C */
	0x1E1D, 0x3442, 0x65AB, 0x7FFF,     /* DTMF D */
	0x0EC9, 0x0000, 0x2D6A, 0x0000,     /*  462 HZ */
	0x2333, 0x0000, 0x2D6A, 0x0000,     /* 1100 HZ */
	0x4333, 0x0000, 0x2D6A, 0x0000,     /* 2100 HZ */
	0x34CC, 0x0000, 0x2D6A, 0x0000,     /* 1650 HZ */
	0x3B33, 0x0000, 0x2D6A, 0x0000,     /* 1850 HZ */
	0x4D99, 0x0000, 0x2D6A, 0x0000,     /* 2425 HZ */
	/* New Tones for Phone Answer Mode 1/9/99 */
	0x0CCC, 0x0000, 0x220A, 0x0000,     /* #22:  400 Hz */
	0x1333, 0x0000, 0x220A, 0x0000,     /* #23:  600 Hz */
	0x1999, 0x0000, 0x220A, 0x0000,     /* #24:  800 Hz */
	0x2CCC, 0x0000, 0x220A, 0x0000,     /* #25: 1400 Hz */
	0x3999, 0x0000, 0x220A, 0x0000,     /* #26: 1800 Hz */
	0x0000, 0x0000, 0x0000, 0x0000      /* #27:    0 Hz */
};


/*===========================================================================
*
*   Modem Transmit Level Table
=============================================================================
*
* Transmit level look-up table.  Full scale output for the SSF336
* is -1 +/- 1 dBM.  Formual for attenuation:
*
*     Attenuation (dB) = 20 log (V1/V2)
*
* Full scale is $7FFF
*
=============================================================================*/
const UINT16 TxLevelTbl[ 32 ] =
{           /* dBm */
	0x390A,         /* -1 */
	0x32D6,         /* -2 */
	0x2D4E,         /* -3 */
	0x2861,         /* -4 */
	0x23FD,         /* -5 */
	0x2013,         /* -6 */
	0x1C96,         /* -7 */
	0x197A,         /* -8 */
	0x16B5,         /* -9 */
	0x143D,         /* -10 */
	0x1209,         /* -11 */
	0x1013,         /* -12 */
	0x0E53,         /* -13 */
	0x0CC5,         /* -14 */
	0x0B61,         /* -15 */
	0x0A24,         /* -16 */
	0x090A,         /* -17 */
	0x080E,         /* -18 */
	0x072E,         /* -19 */
	0x0666,         /* -20 */
	0x05B4,         /* -21 */
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000
};

/* Modem Configuration Table */
const UINT8 ModemConfigTbl[ 23 ] =
{
	MODEM_V21_CH2_CONF,
	MODEM_V27_2400_CONF,
	MODEM_V27_4800_CONF,
	MODEM_V29_7200_CONF,
	MODEM_V29_9600_CONF,
	MODEM_V17_7200_CONF,
	MODEM_V17_9600_CONF,
	MODEM_V17_12000_CONF,
	MODEM_V17_14400_CONF,
	MODEM_V34_2400_CONF,
	MODEM_V34_4800_CONF,
	MODEM_V34_7200_CONF,
	MODEM_V34_9600_CONF,
	MODEM_V34_12000_CONF,
	MODEM_V34_14400_CONF,
	MODEM_V34_16800_CONF,
	MODEM_V34_19200_CONF,
	MODEM_V34_21600_CONF,
	MODEM_V34_24000_CONF,
	MODEM_V34_26400_CONF,
	MODEM_V34_28800_CONF,
	MODEM_V34_31200_CONF,
	MODEM_V34_33600_CONF
};

const UINT16 ModemSpeedTbl[ 14 ] =
{
	2400,  4800,  9600,  12000, 14400, 7200,  16800,
	19200, 21600, 24000, 26400, 28800, 31200, 33600
};

const UINT16 V34SpeedTbl[ 14 ] =
{
	2400,  4800,  7200,  9600,  12000, 14400, 16800,
	19200, 21600, 24000, 26400, 28800, 31200, 33600
};

const UINT16 V17SpeedTbl[ 9 ] =
{
	300, 2400, 4800, 7200, 9600, 7200, 9600, 12000, 14400
};

const UINT16 EQM_limit_tbl[ 9 ] =
{
	0, 0x2800, 0x0C00, 0x0A00, 0x0400, 0x1F00, 0x1E00, 0x1D00, 0x1B00
};


/**   U.S.A   Call Progress Tone Detector Filter Coefficient Tables   **/

#if 0
/* Dial Tone: 440 Hz */
/* 440 Hz Tone Coefficient Table @ 9600 Hz Sample Rate: 430 - 450 Hz */
const UINT16 Tone9600CoeffTbl440[ 10 ] =
{
	0x0042,     /* Alpha3 */
	0x0000,     /* Alpha2 */
	0x0000,     /* Alpha1 */
	0x79FE,     /* Beta2 */
	0xC0C4,     /* Beta1 */
	0x0042,     /* Alpha3' */
	0x0000,     /* Alpha2' */
	0x0000,     /* Alpha1' */
	0x79FE,     /* Beta2' */
	0xC0C4      /* Beta1' */
};

/* 440 Hz Tone Coefficient Table @ 8000 Hz Sample Rate: 430 - 450 Hz */
const UINT16 Tone8000CoeffTbl440[ 10 ] =
{
	0x0042,     /* Alpha3 */
	0x0000,     /* Alpha2 */
	0x0000,     /* Alpha1 */
	0x780C,     /* Beta2 */
	0xC0C4,     /* Beta1 */
	0x0042,     /* Alpha3' */
	0x0000,     /* Alpha2' */
	0x0000,     /* Alpha1' */
	0x775E,     /* Beta2' */
	0xC0C4      /* Beta1' */
};


/* Dial Tone: 600x120//350+440 Hz, Busy Tone/Congestion Tone: 600x120//480+620 Hz */
/* 505 Hz Tone Coefficient Table @ 9600 Hz Sample Rate: 315 - 695 Hz */
const UINT16 Tone9600CoeffTbl505[ 10 ] =
{
	0x02E0,     /* Alpha3 */
	0x0000,     /* Alpha2 */
	0x0000,     /* Alpha1 */
	0x7AD8,     /* Beta2 */
	0xC0C4,     /* Beta1 */
	0x02E0,     /* Alpha3' */
	0x0000,     /* Alpha2' */
	0x0000,     /* Alpha1' */
	0x73E8,     /* Beta2' */
	0xC0C4      /* Beta1' */
};

/* 505 Hz Tone Coefficient Table @ 8000 Hz Sample Rate: 315 - 695 Hz */
const UINT16 Tone8000CoeffTbl505[ 10 ] =
{
	0x02E0,     /* Alpha3 */
	0x0000,     /* Alpha2 */
	0x0000,     /* Alpha1 */
	0x7B5C,     /* Beta2 */
	0xC0C4,     /* Beta1 */
	0x02E0,     /* Alpha3' */
	0x0000,     /* Alpha2' */
	0x0000,     /* Alpha1' */
	0x6CBE,     /* Beta2' */
	0xC0C4      /* Beta1' */
};
#endif

/* Dial Tone: 600x120//350+440//425 Hz, Busy Tone/Congestion Tone: 600x120//480+620//425 Hz */

/* Low Frequency Busy/Congestion/Dial Tone Coefficient Table @ 9600 Hz Sample Rate: */
/* Dial Tone: 350+440//425 Hz */
const UINT16 Tone9600CoeffTblLowBCD[ 14 ] =
{
	0x23FE,     /* Alpha3 */
	0xBC81,     /* Alpha2 */
	0x212B,     /* Alpha1 */
	0x7B68,     /* Beta2 */
	0xC147,     /* Beta1 */
	0x23FE,     /* Alpha3' */
	0xBE01,     /* Alpha2' */
	0x212B,     /* Alpha1' */
	0x797F,     /* Beta2' */
	0xC147,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};

/* Low Frequency Busy/Congestion/Dial Tone Coefficient Table @ 8000 Hz Sample Rate: */
#if 1
/* Dial Tone: 350+440//425 Hz */
const UINT16 Tone8000CoeffTblLowBCD[ 14 ] =
{
	0x244D,     /* Alpha3 */
	0xBCA1,     /* Alpha2 */
	0x2175,     /* Alpha1 */
	0x79F5,     /* Beta2 */
	0xC147,     /* Beta1 */
	0x244D,     /* Alpha3' */
	0xBECD,     /* Alpha2' */
	0x2175,     /* Alpha1' */
	0x773A,     /* Beta2' */
	0xC147,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};
#else
/* 331+434 Hz: This Coefficient Table is provided by BIL */
const UINT16 Tone8000CoeffTblLowBCD[ 14 ] =
{
    0x0141,     /* Alpha3 */
    0x0000,     /* Alpha2 */
    0x0000,     /* Alpha1 */
    0x7A79,     /* Beta2 */
    0xC146,     /* Beta1 */
    0x0141,     /* Alpha3' */
    0x0000,     /* Alpha2' */
    0x0000,     /* Alpha1' */
    0x776B,     /* Beta2' */
    0xC146,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};
#endif


/* High Frequency Busy/Congestion/Dial Tone Coefficient Table @ 9600 Hz Sample Rate: */
/* Busy Tone/Congestion Tone: 600x120//480+620//425 Hz */
const UINT16 Tone9600CoeffTblHighBCD[ 14 ] =
{
	0x08C2,     /* Alpha3 */
	0xEE7C,     /* Alpha2 */
	0x08C2,     /* Alpha1 */
	0xC774,     /* Beta2 */
	0x74FE,     /* Beta1 */
	0x08C2,     /* Alpha3' */
	0x1184,     /* Alpha2' */
	0x08C2,     /* Alpha1' */
	0xCD4F,     /* Beta2' */
	0x6495,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};

/* High Frequency Busy/Congestion/Dial Tone Coefficient Table @ 9600 Hz Sample Rate: */
/* This is for V.8 and V.34 mode */
#if 1
/* Busy Tone/Congestion Tone: 600x120//480+620//425 Hz */
const UINT16 Tone8000CoeffTblHighBCD[ 14 ] =
{
	0x1155,     /* Alpha3 */
	0xF2D0,     /* Alpha2 */
	0x02C6,     /* Alpha1 */
	0x7424,     /* Beta2 */
	0xC3C9,     /* Beta1 */
	0x1155,     /* Alpha3' */
	0xE2D6,     /* Alpha2' */
	0x0FF9,     /* Alpha1' */
	0x6FFD,     /* Beta2' */
	0xC147,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};
#else
/* 475+579 Hz: This Coefficient Table is provided by BIL */
const UINT16 Tone8000CoeffTblHighBCD[ 14 ] =
{
	0x01BA,     /* Alpha3 */
	0x0000,     /* Alpha2 */
	0x0000,     /* Alpha1 */
	0x75FD,     /* Beta2 */
	0xC146,     /* Beta1 */
	0x01BA,     /* Alpha3' */
	0x0000,     /* Alpha2' */
	0x0000,     /* Alpha1' */
	0x71D4,     /* Beta2' */
	0xC146,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};
#endif


/* High Frequency Busy/Congestion/Dial Tone Coefficient Table @ 7200 Hz Sample Rate: */
/* Busy Tone/Congestion Tone: 600x120//480+620//425 Hz */
const UINT16 CoeffTblHighBCD[ 14 ] =
{
	0x08C2,     /* Alpha3 */
	0xEE7C,     /* Alpha2 */
	0x08C2,     /* Alpha1 */
	0xC774,     /* Beta2 */
	0x74FE,     /* Beta1 */
	0x08C2,     /* Alpha3' */
	0x1184,     /* Alpha2' */
	0x08C2,     /* Alpha1' */
	0xCD4F,     /* Beta2' */
	0x6495,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};

/* CI Tone: 980 Hz */
/* 980 Hz Tone Coefficient Table @ 7200 Hz Sample Rate, +/- 20 Hz */
const UINT16 CoeffTbl980[ 14 ] =
{
	0x0618,     /* Alpha3 */
	0xF715,     /* Alpha2 */
	0x061B,     /* Alpha1 */
	0xC147,     /* Beta2 */
	0x5322,     /* Beta1 */
	0x0618,     /* Alpha3' */
	0xF901,     /* Alpha2' */
	0x061B,     /* Alpha1' */
	0xC147,     /* Beta2' */
	0x5322,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};

/* CI Tone: 980 Hz */
/* 980 Hz Tone Coefficient Table @ 9600 Hz Sample Rate, +/- 20 Hz */
const UINT16 Tone9600CoeffTbl980[ 14 ] =
{
	0x0205,     /* Alpha3 */
	0xFBF9,     /* Alpha2 */
	0x0206,     /* Alpha1 */
	0xC147,     /* Beta2 */
	0x5337,     /* Beta1 */
	0x0205,     /* Alpha3' */
	0x00B4,     /* Alpha2' */
	0x0206,     /* Alpha1' */
	0xC147,     /* Beta2' */
	0x4000,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};

/* CI Tone: 1180 Hz */
/* 1180 Hz Tone Coefficient Table @ 7200 Hz Sample Rate, +/- 20 Hz */
const UINT16 CoeffTbl1180[ 14 ] =
{
	0x0665,     /* Alpha3 */
	0xF8A8,     /* Alpha2 */
	0x0668,     /* Alpha1 */
	0xC0E5,     /* Beta2 */
	0x4176,     /* Beta1 */
	0x0665,     /* Alpha3' */
	0xFA30,     /* Alpha2' */
	0x0668,     /* Alpha1' */
	0xC0E5,     /* Beta2' */
	0x4176,     /* Beta1' */
	0x7E67,     /* LPFBK */
	0x02DF,     /* LPGAIN */
	0x2A00,     /* THRESHU */
	0x1C00      /* THRESHL */
};

/* CNG Tone: 1100 Hz */
/* 1100 Hz Tone Coefficient Table @ 9600 Hz Sample Rate, +/- 40 Hz */
const UINT16 Tone9600CoeffTbl1100[ 14 ] =
{
	0x01B3,     /* Alpha3 */
	0xFC9C,     /* Alpha2 */
	0x01B4,     /* Alpha1 */
	0xC147,     /* Beta2 */
	0x48C6,     /* Beta1 */
	0x01B3,     /* Alpha3' */
	0x0097,     /* Alpha2' */
	0x01B4,     /* Alpha1' */
	0xC147,     /* Beta2' */
	0x4897,     /* Beta1' */
	0x7E67,     /* LPFBK */
	0x02DF,     /* LPGAIN */
	0x2A00,     /* THRESHU */
	0x1C00      /* THRESHL */
};

/* 1100 Hz Tone Coefficient Table @ 8000 Hz Sample Rate, +/- 40 Hz */
const UINT16 Tone8000CoeffTbl1100[ 14 ] =
{
	0x1E1D,     /* Alpha3 */
	0xD917,     /* Alpha2 */
	0x1BC0,     /* Alpha1 */
	0x53CD,     /* Beta2 */
	0xC147,     /* Beta1 */
	0x1E1D,     /* Alpha3' */
	0xDBDA,     /* Alpha2' */
	0x1BC0,     /* Alpha1' */
	0x50C6,     /* Beta2' */
	0xC147,     /* Beta1' */
	0x7E67,     /* LPFBK */
	0x02DF,     /* LPGAIN */
	0x2A00,     /* THRESHU */
	0x1C00      /* THRESHL */
};


/* 950 Hz Tone Coefficient Table @ 7200 Hz Sample Rate, +/- 40 Hz, -40dBm */
const UINT16 CoeffTbl950[ 14 ] =
{
	0x0506,     /* Alpha3 */
	0xF71E,     /* Alpha2 */
	0x07DA,     /* Alpha1 */
	0xC187,     /* Beta2 */
	0x5773,     /* Beta1 */
	0x0506,     /* Alpha3' */
	0xF7ED,     /* Alpha2' */
	0x07DA,     /* Alpha1' */
	0xC187,     /* Beta2' */
	0x5362,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x0880,     /* THRESHU */
	0x0580      /* THRESHL */
};

/* 1100 Hz Tone Coefficient Table @ 7200 Hz Sample Rate, +/- 30 Hz */
const UINT16 CoeffTbl1100[ 14 ] =
{
	0x01B3,     /* Alpha3 */
	0xFC9C,     /* Alpha2 */
	0x01B4,     /* Alpha1 */
	0xC147,     /* Beta2 */
	0x48C6,     /* Beta1 */
	0x01B3,     /* Alpha3' */
	0x0097,     /* Alpha2' */
	0x01B4,     /* Alpha1' */
	0xC147,     /* Beta2' */
	0x4897,     /* Beta1' */
	0x7E67,     /* LPFBK */
	0x02DF,     /* LPGAIN */
	0x2A00,     /* THRESHU */
	0x1C00      /* THRESHL */
};

/* 1250 Hz Tone Coefficient Table @ 7200 Hz Sample Rate, +/- 40 Hz, -40dBm */
const UINT16 CoeffTbl1250[ 14 ] =
{
	0x050B,     /* Alpha3 */
	0xF9B3,     /* Alpha2 */
	0x07E1,     /* Alpha1 */
	0xC187,     /* Beta2 */
	0x3CD3,     /* Beta1 */
	0x050B,     /* Alpha3' */
	0xFAAC,     /* Alpha2' */
	0x07E1,     /* Alpha1' */
	0xC187,     /* Beta2' */
	0x37EF,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x1600,     /* THRESHU */
	0x0A00      /* THRESHL */
};

/* FSK flag Tone: 1650 Hz */
/*   1650 Hz Tone Coefficent Table copied from SSF336 Designer's Guide */
const UINT16 CoeffTbl1650[ 14 ] =
{
	0x0306,     /* Alpha3 */
	0xF9F9,     /* Alpha2 */
	0x0307,     /* Alpha1 */
	0xC147,     /* Beta2 */
	0x10A6,     /* Beta1 */
	0x0306,     /* Alpha3' */
	0x010D,     /* Alpha2' */
	0x0307,     /* Alpha1' */
	0xC147,     /* Beta2' */
	0x106E,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x1600,     /* THRESHU */
	0x0A00      /* THRESHL */
};

const UINT16 Tone9600CoeffTbl1650[ 14 ] =
{
	0x028B,     /* Alpha3 */
	0xFAEA,     /* Alpha2 */
	0x028C,     /* Alpha1 */
	0xC289,     /* Beta2 */
	0x3BDA,     /* Beta1 */
	0x028B,     /* Alpha3' */
	0x01F3,     /* Alpha2' */
	0x028C,     /* Alpha1' */
	0xC289,     /* Beta2' */
	0x3A68,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x1600,     /* THRESHU */
	0x0A00      /* THRESHL */
};

/* CED Tone: 2100 Hz */
/*  2100 Hz Tone Coefficient Table copied from SSF336 Designer's Guide */
const UINT16 CoeffTbl2100[ 14 ] =
{
	0x01E8,     /* Alpha3 */
	0xFC32,     /* Alpha2 */
	0x01E9,     /* Alpha1 */
	0xC147,     /* Beta2 */
	0xDF4F,     /* Beta1 */
	0x01E8,     /* Alpha3' */
	0x034E,     /* Alpha2' */
	0x01E9,     /* Alpha1' */
	0xC147,     /* Beta2' */
	0xDF19,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x1600,     /* THRESHU */
	0x0A00      /* THRESHL */
};

const UINT16 Tone9600CoeffTbl2100[ 14 ] =
{
	0x0397,     /* Alpha3 */
	0xF8D3,     /* Alpha2 */
	0x0399,     /* Alpha1 */
	0xC3C9,     /* Beta2 */
	0x1905,     /* Beta1 */
	0x0397,     /* Alpha3' */
	0x02C0,     /* Alpha2' */
	0x0399,     /* Alpha1' */
	0xC3C9,     /* Beta2' */
	0x176D,     /* Beta1' */
	0x7F30,     /* LPFBK */
	0x00CF,     /* LPGAIN */
	0x1600,     /* THRESHU */
	0x0A00      /* THRESHL */
};

/* Tone detector TONEA access codes */
const UINT16 FreqAccTbl1[ 14 ] =
{
	0xAA1,      /* Alpha3 */
	0xAA2,      /* Alpha2 */
	0xAA3,      /* Alpha1 */
	0xAA4,      /* Beta2 */
	0xAA5,      /* Beta1 */
	0xBA1,      /* Alpha3' */
	0xBA2,      /* Alpha2' */
	0xBA3,      /* Alpha1' */
	0xBA4,      /* Beta2' */
	0xBA5,      /* Beta1' */
	0xBA0,      /* LPFBK */
	0xAA0,      /* LPGAIN */
	0xAB8,      /* THRESHU */
	0xBB8       /* THRESHL */
};

/* Tone detector TONEB access codes */
const UINT16 FreqAccTbl2[ 14 ] =
{
	0xAA7,      /* Alpha3 */
	0xAA8,      /* Alpha2 */
	0xAA9,      /* Alpha1 */
	0xAAA,      /* Beta2 */
	0xAAB,      /* Beta1 */
	0xBA7,      /* Alpha3' */
	0xBA8,      /* Alpha2' */
	0xBA9,      /* Alpha1' */
	0xBAA,      /* Beta2' */
	0xBAB,      /* Beta1' */
	0xBA6,      /* LPFBK */
	0xAA6,      /* LPGAIN */
	0xAB9,      /* THRESHU */
	0xBB9       /* THRESHL */
};

/* Tone detector TONEC access codes */
const UINT16 FreqAccTbl3[ 14 ] =
{
	0xAAD,      /* Alpha3 */
	0xAAE,      /* Alpha2 */
	0xAAF,      /* Alpha1 */
	0xAB0,      /* Beta2 */
	0xAB1,      /* Beta1 */
	0xBAD,      /* Alpha3' */
	0xBAE,      /* Alpha2' */
	0xBAF,      /* Alpha1' */
	0xBB0,      /* Beta2' */
	0xBB1,      /* Beta1' */
	0xBAC,      /* LPFBK */
	0xAAC,      /* LPGAIN */
	0xABA,      /* THRESHU */
	0xBBA       /* THRESHL */
};

const UINT16 FreqPrefilterTbl[ 10 ] =
{
	0xAB2,      /* Alpha3 */
	0xAB3,      /* Alpha2 */
	0xAB4,      /* Alpha1 */
	0xAB5,      /* Beta2 */
	0xAB6,      /* Beta1 */
	0xBB2,      /* Alpha3' */
	0xBB3,      /* Alpha2' */
	0xBB4,      /* Alpha1' */
	0xBB5,      /* Beta2' */
	0xBB6       /* Beta1' */
};

const UINT16 CoeffTblPrefilter[ 10 ] =
{
	0x0000,
	0x0000,
	0x4000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x4000,
	0x0000,
	0x0000
};

const UINT16 *FreqAccTable[ 4 ] =
{
	FreqAccTbl3, FreqAccTbl2, FreqAccTbl1, FreqPrefilterTbl
};

#ifdef LOSS_READY
const UINT8 LossTbl[ 4 ] =
{
	0,                              /* 0-3 dB */
	TXLOSS2_CTRL,                   /* 4-7 dB */
	TXLOSS3_CTRL,                   /* 8-11 dB */
	TXLOSS3_CTRL | TXLOSS2_CTRL     /* 12-15 dB */
};
#endif

const UINT8 SpeedMaskTbl[ 4 ] =
{
	0x02,       /* 50% slow */
	0x01,       /* Normal */
	0x04,       /* 33% fast */
	0x08        /* 50% fast */
};

const UINT8 ToneCntrlTbl[ 4 ] =
{
	0x82, 0x82, 0x83, 0x83
};

#ifdef SPKRPN_READY
const UINT8 ToneAccTbl[ 4 ] =
{
	MODEM_SCALEC2_ACC,
	MODEM_SCALEC1_ACC,
	MODEM_DELPHI2_ACC,
	MODEM_DELPHI1_ACC
};

const UINT8 SpkrToneCntrlTbl[ 8 ] =
{
	0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03
};

const UINT8 SpkrToneAccTbl[ 8 ] =
{
	MODEM_P_SCALEC2_ACC,
	MODEM_S_SCALEC2_ACC,
	MODEM_P_SCALEC1_ACC,
	MODEM_S_SCALEC1_ACC,
	MODEM_P_DELPHI2_ACC,
	MODEM_S_DELPHI2_ACC,
	MODEM_P_DELPHI1_ACC,
	MODEM_S_DELPHI1_ACC
};
#endif

