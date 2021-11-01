/*
 *  $RCSfile: phonetbl.c,v $
 * $Revision: 1.13 $
 *   $Author: miurat $
 *     $Date: 2012/03/21 00:27:03 $
 */
/************************************************************************
 *
 *      phonetbl.c
 *
 *  Phone parameter tables.
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"

#include "ATTRIB.H"
#include "MODEMX.H"
#include "PHONEX.H"
#include "oem_cty.h"
#include "phone.h"


CntrySDAAStructure PSD;
PDialParams PDP;
PToneParams PTP;
PRingParams PRP;

extern void SSFSetOnHookVoltage( UINT8 vol );
extern void SSFSetLineInUseVoltage( UINT8 vol );
extern void SSFSetLineInUseThreshold( UINT8 vol );

/* SDAA parameters */

static const CntrySDAAStructure PSDAA_Default =
{
	2,    /* Line Impedance */
	2,    /* DC Curve */
	1,    /* Make Resistance */
	1,    /* Pulse edge */
	0x40, /* Initial DAC value */
	0,    /* Ring Max Frequency */
	0,    /* Ring Min Frequency */
	16,   /* Ring Threshold */
	0,    /* South Korea Ring Impedance */
	0,    /* Brazil Ring Detection */
	0,    /* Line In Use Threshold */
	0,    /* Extension Threshold */
	0,    /* Digital Threshold */
	0     /* DIB power adjust */
};


/* Dial parameters */

static const PDialParams PDial_Default =
{
	  0, /* Blind dial : */
	100, /* DTMFOnTime : */
	100, /* DTMFOffTime : */
	 60, /* DTMFLowLevel : */
	 40, /* DTMFHighLevel : */
	  0, /* DTMFCompensation : */
	  1, /* DTMFAbcdAllow : */

	  0, /* PulseMode : */
	 10, /* PulseSpeed : */
	 40, /* PulseMakeTime : */
	 60, /* PulseBreakTime : */
	800, /* PulseInterDelay : */
	  0, /* PulseMakeOffset : */
	  0, /* PulseBreakOffset : */
	  0, /* PulseSetupTime : */
	  0, /* PulseClearTime : */
	  1  /* PulseDialAllow : */
};


/* Tone parameters */

static const PToneParams PTone_Default =
{
	4000,  /* MaxSilence : wait for 4 sec before abort if no dial tone */
	3000,  /* BlindDialDelay : delay 3 sec */
	3000,  /* DialToneLen : detect min. 3 sec dial tone */
	5000,  /* DialToneTimeOut : dial tone detection 5 sec timeout */

	350,   /* BusyToneOnMin : */
	800,   /* BusyToneOnMax : */
	350,   /* BusyToneOffMin : */
	800,   /* BusyToneOffMax : */
	3000,  /* BusyToneLen : detect min. 3 sec dial tone */
	60000, /* BusyToneTimeOut : busy tone 60 sec timeout */

	150,   /* CongToneOnMin : */
	350,   /* CongToneOnMax : */
	150,   /* CongToneOffMin : */
	350,   /* CongToneOffMax : */

	300,   /* CNGOnMin : 300 msec. In general, CNG on 0.5 sec, off 3 sec */
	700,   /* CNGOnMax : 700 msec */
	2500,  /* CNGOffMin : 2.5 sec */
	3500,  /* CNGOffMax : 3.5 sec */

	40,    /* DTMFOnTime : 40 msec */
	100,   /* DTMFOffTime : off time 100 msec */
	50,    /* DTMFVerifyTime : verify 50 msec on time */
	500,   /* FlagVerifyTime : HDLC flag 500 msec on */
	500    /* ANS tone debounce */
};


/* Ring parameters */

static const PRingParams PRing_Default =
{
	  100, /* RingOnDebounce  : Min. time to confirm RI ON  state */
	  100, /* RingOffDebounce : Min. time to confirm RI OFF state */
	  250, /* RingMaxGlitch   : Max. time for RI ON/OFF glitchs */

	  750, /* RingOffMax      : Max Short Silence: 700 ms */
	 2500, /* RingOnMax       : PATTERN_1: 1750 - 2250 ms */
	 5000, /* RingOffTimeout  : PATTERN_1: 3550 - 4450 ms */

	       /* Report time out after 10 Sec. ring off */
	10000, /* RingTimeoutNotify : >= RingOffTimeout */

	       /* First ringing burst time mapping */
	 2500, /* RingOn1Ptn0     : Error */
	 1400, /* RingOn1Ptn1     : PATTERN_1: 1750-2250 ms */
	  800, /* RingOn1Ptn2x    : PATTERN_X:  800-1100 ms?*/
	  600, /* RingOn1Ptn2xy   : PATTERN_2:  670- 930 ms, 580-1075 ms */
	       /* PATTERN_Y:  600- 900 ms?*/
	  550, /* RingOn1Ptn2345y : PATTERN_5:  400- 600 ms */
	  300, /* RingOn1Ptn345   : PATTERN_3:  310- 490 ms, 265- 575 ms */
	  100, /* RingOn1Ptn34    : PATTERN_4:  220- 380 ms, 150- 575 ms */

	       /* First silence interval mapping */
	 4600, /* RingOff1Ptn0    : Error */
	       /* PATTERN_5:  N.A.*/
	 3500, /* RingOff1Ptn15   : PATTERN_1:  3550-4450 ms */
	 2000, /* RingOff1Ptnx    : PATTERN_X:  2550-3450 ms?*/
	 1000, /* RingOff1Ptn15x  : for RingOff1Go */
	  800, /* RingOff1Ptn00   : Error */
	       /* PATTERN_Y:  300-  500 ms?*/
	  200, /* RingOff1Ptn234y : PATTERN_2:   310- 490 ms, 265- 575 ms */
	   95, /* RingOff1Ptn34   : PATTERN_3,4: 125- 275 ms,  95- 575 ms */

	       /* CID turn on time mapping */
	  300, /* RingOffCidOn     : Turn on <= 500 (300) ms */
	 3000, /* RingOffCidOff125 : around 3425 - 200 (475) ms, 125 */
	 2500, /* RingOffCidOff34  : around 2925 - 200 (475) ms, 34 */
	 1800, /* RingOffCidOffx   : around 2200 - 200 (475) ms, X */
	 1200, /* RingOffCidOffy   : around 1600 - 200 (475) ms, Y */

	       /* Second ringing burst time mapping */
	 1500, /* RingOn2Ptn0   : Error */
	  650, /* RingOn2Ptn24y : PATTERN_4:  850-1150 ms, 700-1150 ms */
	       /* PATTERN_Y:  600- 900 ms?*/
	  500, /* RingOn2Ptn23y : PATTERN_2:  670- 930 ms, 580-1075 ms */
	  100, /* RingOn2Ptn3   : PATTERN_3:  310- 490 ms, 265- 575 ms */

	       /* Second silence interval mapping */
	 4700, /* RingOff2Ptn0  : Error */
	 2800, /* RingOff2Ptn2  : PATTERN_2:  3550-4450 ms, 3425-4450 ms */
	 1000, /* RingOff2Ptny  : PATTERN_Y:  1600-2200 ms?*/
	 1000, /* RingOff2Ptn2y : for Off2Go */
	   95, /* RingOff2Ptn34 : PATTERN_3,4: 125- 275 ms,   95- 575 ms */

	       /* Third ringing burst time mapping */
	 1500, /* RingOn3Ptn0 : Error */
	  800, /* RingOn3Ptn3 : PATTERN_3:  670- 930 ms, 580-1075 ms */
	  400, /* RingOn3Ptn34: */
	   60, /* RingOn3Ptn4 : PATTERN_4:  220- 380 ms, 150- 575 ms */
};


void phone_init_params( CtryPrmsStruct *Ctry )
{
	/* set default parameters */
	PSD = PSDAA_Default;
	PDP = PDial_Default;
	PTP = PTone_Default;
	PRP = PRing_Default;

	if ( Ctry != 0 )
	{
		/* update SDAA parameters */

		PSD = Ctry->SDAA;

		/* update dial parameters */

		if ( Ctry->DTMFParam.DTMFOnTime != 0 )
		{
			PDP.DTMFOnTime      = Ctry->DTMFParam.DTMFOnTime;
		}

		if ( Ctry->DTMFParam.DTMFInterdigit != 0 )
		{
			PDP.DTMFOffTime     = Ctry->DTMFParam.DTMFInterdigit;
		}

		PDP.DTMFLowLevel    = Ctry->DTMFParam.LowDTMFLevel;
		PDP.DTMFHighLevel   = Ctry->DTMFParam.HighDTMFLevel;
		PDP.DTMFCompensation= Ctry->DTMFParam.DTMFCompensation;
		PDP.DTMFAbcdAllow   = Ctry->DTMFParam.AllowDTMFabcd;

		PDP.PulseMode       = Ctry->Pulse.PulseDialMode;
		PDP.PulseSpeed      = Ctry->Pulse.PulseSpeed;
		PDP.PulseMakeTime   = Ctry->Pulse.PulseMakeRatio;
		PDP.PulseBreakTime  = Ctry->Pulse.PulseBreakRatio;
		PDP.PulseInterDelay = Ctry->Pulse.PulseInterd;
		PDP.PulseMakeOffset = Ctry->Pulse.PulseMakeOffset;
		PDP.PulseBreakOffset= Ctry->Pulse.PulseBreakOffset;
		PDP.PulseSetupTime  = Ctry->Pulse.PulseSetupTime;
		PDP.PulseClearTime  = Ctry->Pulse.PulseClearTime;
		PDP.PulseDialAllow  = Ctry->Pulse.AllowPulseDialing;


		/* update tone parameters */

		PTP.DialToneLen     = Ctry->Tone.DialStable;
		PTP.AnswerStable    = Ctry->Tone.AnswerStable;
		PTP.BusyToneOnMin   = Ctry->Cadence.BusyParams.Interval[ 0 ].lMin;
		PTP.BusyToneOnMax   = Ctry->Cadence.BusyParams.Interval[ 0 ].lMax;
		PTP.BusyToneOffMin  = Ctry->Cadence.BusyParams.Interval[ 1 ].lMin;
		PTP.BusyToneOffMax  = Ctry->Cadence.BusyParams.Interval[ 1 ].lMax;
		PTP.BusyToneLen     = Ctry->Cadence.BusyParams.Interval[ 0 ].lMin * Ctry->Cadence.BusyParams.lNumIntervalsNeeded;
		PTP.BusyToneTimeOut = Ctry->Cadence.BusyParams.Interval[ 0 ].lMax * Ctry->Cadence.BusyParams.lNumIntervalsNeeded;
		if ( PTP.BusyToneTimeOut > 0x1998 )
		{
           PTP.BusyToneTimeOut = 0xfff0;
		}
		else
		{
           PTP.BusyToneTimeOut *= 10;
		}
		PTP.CongToneOnMin   = Ctry->Cadence.CongestionParams.Interval[ 0 ].lMin;
		PTP.CongToneOnMax   = Ctry->Cadence.CongestionParams.Interval[ 0 ].lMax;
		PTP.CongToneOffMin  = Ctry->Cadence.CongestionParams.Interval[ 1 ].lMin;
		PTP.CongToneOffMax  = Ctry->Cadence.CongestionParams.Interval[ 1 ].lMax;


		/* update ring parameters */

		PRP.RingOnDebounce  = Ctry->Ring.RingStable;
		PRP.RingOffDebounce = Ctry->Ring.RingStable;
		PRP.RingOffMax      = Ctry->Ring.MinDelayBtwnRings;
		PRP.RingOffTimeout  = Ctry->Ring.MaxDelayBtwnRings;
	}
}


void phone_set_params( PHONE_PARAMS_TYPE type, UINT16 val )
{
	TRACE_PHONE( "phone set params %d %d\n", type, val );

	switch ( type )
	{
		case PHONE_BLIND_DIAL:

			PDP.BlindDial = val;
			break;

		case PHONE_DIAL_TIMEOUT:

			PTP.DialToneTimeOut = val;
			break;

		case PHONE_DTMF_LOW_LEVEL:

			PDP.DTMFLowLevel = ( UINT8 ) val;
			break;

		case PHONE_DTMF_HIGH_LEVEL:

			PDP.DTMFHighLevel = ( UINT8 ) val;
			break;

		case PHONE_PULSE_MAKE_TIME:

			PDP.PulseMakeTime = ( UINT8 ) val;
			break;

		case PHONE_PULSE_BREAK_TIME:

			PDP.PulseBreakTime = ( UINT8 ) val;
			break;

		case PHONE_PULSE_DIAL_TYPE:

			PDP.PulseMode = ( UINT8 ) val;
			break;

		case PHONE_INTERDIGIT_DELAY:

			PDP.PulseInterDelay = val;
			break;

		case PHONE_DC_CHARACTERISTIC:

			PSD.DCCurve = ( UINT8 ) val;
			break;

		case PHONE_IMPEDANCE:

			PSD.LineImpedance = ( UINT8 ) val;
			break;

		case PHONE_CALLER_ID_TYPE:

			break;

		case PHONE_BUSY_TONE_CYCLE:

			PTP.BusyToneLen = PTP.BusyToneOnMin * val;
			break;

		case PHONE_BUSY_TONE_MIN_ON_TIME:

			PTP.BusyToneOnMin = val;
			break;

		case PHONE_BUSY_TONE_MAX_ON_TIME:

			PTP.BusyToneOnMax = val;
			break;

		case PHONE_BUSY_TONE_MIN_OFF_TIME:

			PTP.BusyToneOffMin = val;
			break;

		case PHONE_BUSY_TONE_MAX_OFF_TIME:

			PTP.BusyToneOffMax = val;
			break;

		case PHONE_EXTENSION_SUPPORT:

			break;

		case PHONE_PULSE_FALL_TIME:

			PSD.PulseEdge = ( UINT8 ) val;
			break;

		case PHONE_EXTENSION_VOLTAGE_THRESHOLD:

			PSD.ExtensionThreshold = ( UINT8 ) val;
			break;

		case PHONE_REF_ONHOOK_VOLTAGE:

			SSFSetOnHookVoltage( ( UINT8 ) val );
			break;

		case PHONE_REF_LINEINUSE_VOLTAGE:

			SSFSetLineInUseVoltage( ( UINT8 ) val );
			break;

        case PHONE_LINE_IN_USE_THRESHOLD:

            SSFSetLineInUseThreshold( ( UINT8 ) val );
            break;

		default:

			break;
	}
}

