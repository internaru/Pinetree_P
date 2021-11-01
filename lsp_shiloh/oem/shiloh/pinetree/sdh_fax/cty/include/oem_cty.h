/*
 *  $RCSfile: oem_cty.h,v $
 * $Revision: 1.3 $
 *   $Author: miurat $
 *     $Date: 2009/11/11 22:20:28 $
 */
/*****************************************************************
***               Copyright (c) 2004 - 2009                    ***
***               Conexant Systems Inc.                        ***
***               Imaging and PC Media                         ***
***               All Rights Reserved                          ***
***                                                            ***
***            CONFIDENTIAL -- No Dissemination or             ***
***            use without prior written permission.           ***
***                                                            ***
******************************************************************
*                                                                *
*  2004 - 2006 Major modification for Embedded T.32 with V.34 tm *
*  2008 - 2009 Modification for SSF336, No AT command base    tm *
*                                                                *
******************************************************************/

#ifndef __OEM_CTY_H_
#define __OEM_CTY_H_

#include "dc_type.h"

#define CTY_SIGNATURE   "CTY1"

/* Structure Typedefs */

#pragma pack(1)  // must be pack
#define MAX_STR_LEN        40
#define NUM_LIMITED_SREGS  10

// SDAA parameters
typedef struct SDAAParams_Tag
{
	UINT8  LineImpedance;
	UINT8  DCCurve;
	UINT8  RMake;
	UINT8  PulseEdge;
	UINT8  InitialDACvalue;
	UINT8  RingMaxFrequency;
	UINT8  RingMinFrequency;
	UINT8  RingThreshold;
	UINT8  LowRingImpedance;
	UINT8  BrazilRingDetection;
	UINT8  LineInUseThreshold;
	UINT8  ExtensionThreshold;
	UINT8  DigitalThreshold;
	UINT8  DIBPower;
} CntrySDAAStructure;


// Transmit Level
typedef struct Txlevel_Tag
{
	UINT16 Default;
	UINT8  Compensation;
} CntryTxlevelStructure;


// Pulse Dial Parameters
typedef struct Pulse_Tag
{
	UINT8  AllowPulseDialing;
	UINT8  PulseDialMode;
	UINT8  PulseSpeed;
	UINT8  PulseMakeRatio;
	UINT8  PulseBreakRatio;
	char   PulseMakeOffset;    // can be +/-
	char   PulseBreakOffset;   // can be +/-
	UINT8  PulseSetupTime;
	UINT8  PulseClearTime;
	UINT16 PulseInterd;
	UINT16 LineMuteDelay;      // Line mute delay
	UINT8  DisablePulseAfterTone;
	UINT8  DisableHearingPulseDialing;
} CntryPulseStructure;


// Ring Detection Parameters
typedef struct Ring_Tag
{
	UINT16 RingStable;
	UINT16 MinTimeBeforeAnswering;
	UINT16 MinDelayBtwnRings;
	UINT16 MaxDelayBtwnRings;
	UINT16 MinRingOnTime;
	UINT16 MinImmediateRingOn;
	UINT16 MaxImmediateRingOn;
} CntryRingStructure;


// DTMF Dial Parameters
typedef struct DTMF_Tag
{
	UINT16 DTMFOnTime;
	UINT16 DTMFInterdigit;
	UINT16 LowDTMFLevel;
	UINT16 HighDTMFLevel;
	UINT8  DTMFCompensation;
	UINT8  AllowDTMFabcd;
	UINT8  DisableHearingDTMFDialing;
} CntryDTMFStructure;


// Filter Parameters
typedef struct FilterParamTAG
{
	UINT16 FilterType;
	UINT16 Biquad1[5];
	UINT16 Biquad2[5];
	UINT16 LpFBK;
	UINT16 LpGain;
	UINT16 ThreshU;
	UINT16 ThreshL;
	UINT16 Biquad1_PreF[5];
	UINT16 Biquad2_PreF[5];
	int SqDisState;
} FilterParam;


typedef struct Filter_Tag
{
	FilterParam Primary;
	FilterParam Alternate;
} CntryFilterStructure;


typedef struct FilterThreshold_Tag
{
	UINT16 ThreshU;
	UINT16 ThreshL;
} FilterThresholdStructure;


// Thresholds Parameters
typedef struct Threshold_Tag
{
	FilterThresholdStructure DialThresh;
	FilterThresholdStructure AltDialThresh;
	FilterThresholdStructure WDialThresh;
	FilterThresholdStructure AltWDialThresh;
	FilterThresholdStructure ProgThresh;
	FilterThresholdStructure AltProgThresh;
	char DTMFRxThresh;
	char RLSDXRTHOffset;    // RLSD Threshold Adjustment
} CntryThresholdStructure;


// Tone Detection Parameters
typedef struct Tone_Tag
{
	UINT16 DialStable;
	UINT16 WDialStable;
	UINT16 AnswerStable;
	UINT16 ProgressStable;
	UINT16 DialtoneWaitTime;
	UINT16 PolarityReversalIgnoreTime;
} CntryToneStructure;


// Timing Parameter
typedef struct Timing_Tag
{
	UINT16 InterCallDelay1;   //Delay when same number dialed after outgoing call
	UINT16 InterCallDelay2;   //Delay when different number dialed after outgoing call
	UINT16 InterCallDelay3;   //Delay after incoming call
	UINT16 BillingDelay;
	UINT16 wMaximumPauseBetweenDigits;//Maximum allowed induced total pause between digits when consecutive ","'s are issued.
	                                  //Unit is in seconds.
	UINT16 wDelayAfterFlashWhenDTMF;  //Induced delay after flash "!" when a DTMF digit has been dialed
	                                  //Unit is in milliseconds.
} CntryTimingStructure;


// TONE CADENCE
typedef struct CadPrm_Tag
{
	struct
	{
		UINT16   lMin;
		UINT16   lMax;
	} Interval[4];

	UINT16 lNumIntervalsNeeded;

} CadPrm;


typedef struct Cadence_Tag
{
	UINT8  ToneCadence_NumberOfPhases;
	CadPrm DialtoneParams;
	CadPrm WDialtoneParams;
	CadPrm BusyParams;
	CadPrm RingbackParams;
	CadPrm CongestionParams;
} CntryCadenceStructure;


// CallerID Parameter
typedef struct CallerID_Tag
{
	UINT8  Cid_TYPE;
	UINT8  Cid_Prefix;
	UINT8  Cid_Suffix;
	UINT8  DTMF_MinOnTime;
	UINT8  DTMF_MinOffTime;
	UINT16 Cid_WaitTime;
	UINT8  PreRing_Cid_Indicator;
	UINT8  PolarityReversal;
} CntryCallerIDStructure;


// Option Control
typedef struct Option_Tag
{
	UINT8 AltDialToneFilter;
	UINT8 AltCallProgressFilter;
	UINT8 AltWToneFilter;
	UINT8 CNGFilterSelection;
	UINT8 EnforceDialTone;
	UINT8 EnforceBusyTone;
	UINT8 LongToneIsBusy;
	UINT8 EPTforV29;
	UINT8 Spare0;
} CntryOptionStructure;


typedef struct CtryPrmsStruct_Tag
{
	// T.35 Country Identification
	UINT8 T35Code;

	// Country Name
	char cInter[MAX_STR_LEN];

	// Transmit Level
	CntryTxlevelStructure Txlevel;

	// SDAA Control Parameters
	CntrySDAAStructure SDAA;

	// External Phone Relay Control Parameters
	UINT8  Relays[10];
	UINT16 DelayPhoneOffLine;
	UINT16 DelayPhoneToLine;

	// Pulse Dial Parameters
	CntryPulseStructure Pulse;

	// DTMF Dial Parameters
	CntryDTMFStructure DTMFParam;

	// Ring Detection Parameters
	CntryRingStructure Ring;

	// Filter Parameters
	CntryFilterStructure Filter;

	// Thresholds Parameters
	CntryThresholdStructure Threshold;

	// Tone Detection Parameters
	CntryToneStructure Tone;

	// Timing Parameter
	CntryTimingStructure Timing;

	// TONE CADENCE
	CntryCadenceStructure Cadence;

	// CallerId Control
	CntryCallerIDStructure CallerID;

	// Option Control
	CntryOptionStructure Option;

	char Signature[4];

} CtryPrmsStruct;

#pragma pack()


#endif //__OEM_CTY_H_
