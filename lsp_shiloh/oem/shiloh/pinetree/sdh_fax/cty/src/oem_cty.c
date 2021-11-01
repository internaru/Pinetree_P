/*
 *  $RCSfile: oem_cty.c,v $
 * $Revision: 1.8 $
 *   $Author: miurat $
 *     $Date: 2011/11/19 02:45:50 $
 */
/*****************************************************************
***               Copyright (c) 1994 - 2011                    ***
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
*  2008 - 2011 Modification for SSF336, No AT command base    tm *
*                                                                *
******************************************************************/

#include "configmo.h"

#include "t35_cty.h"
#include "oem_cty.h"

#define SELECT_EXT_PHONE_RELAY_PORT     EX_RELAY_PORT + 1     // Set bit 0 for Port 0
#define MODEM_ONHOOK_PHONE_RELAY_ON     EX_RELAY_MASK + 0x80  // Connect external phone to line
#define MODEM_ONHOOK_PHONE_RELAY_OFF    EX_RELAY_MASK         // Disconnect external phone
#define MODEM_OFFHOOK_PHONE_RELAY_ON    EX_RELAY_MASK + 0x80  // Connect external phone to line
#define MODEM_OFFHOOK_PHONE_RELAY_OFF   EX_RELAY_MASK         // Disconnect external phone

//**************************************************
//* Country paramters for Japan                    *
//**************************************************

static const CtryPrmsStruct Country_00 =
{
	// T.35 Country Identification
	T35_JAPAN,                      // T35Countrycode:

	// Country Name
	"Japan",                        // International string

	// Transmit Level
	{150,                            // Default: 150 for -15.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	58,                             // Ring Max frequency: Hz (if 0 uses default)
	14,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	20,                             // PulseSpeed
	33,                             // PulseMakeRatio
	67,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	0,                              // PulseSetupTime: ms Disable Dialshunt if 0 and Enable if != 0
	0,                              // PulseClearTime: ms Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	90,                             // LowDTMFLevel:  -9.0 dBm
	70,                             // HighDTMFLevel: -7.0 dBm
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ms ASAP
	700,                            // MinDelayBtwnRings: ms
	5000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x08C2,                         // ToneCoeffs
	0xEE7C,
	0x08C2,
	0xC774,
	0x74FE},
	{0x08C2,
	0x1184,
	0x08C2,
	0xCD4F,
	0x6495},
	0x7B30,                         // LPBFK
	0x04CF,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0000,                         // DialThreshU
	0x0000},                         // DialThreshL
	{0x0000,                         // AltDialThreshU
	0x0000},                         // AltDialThreshL
	{0x0000,                         // WDialThreshU
	0x0000},                         // WDialThreshL
	{0x0000,                         // AltWDialThreshU
	0x0000},                         // AltWDialThreshL
	{0x0000,                         // ProgThreshU
	0x0000},                         // ProgThreshL
	{0x0000,                         // AltProgThreshU
	0x0000},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable: ms
	750,                            // WDialStable: ms
	300,                            // AnswerStable: ms
	5,                              // ProgressStable: ms
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime: ms (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Flash delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                              // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{100,300},{100,300},{0,0},{0,0}},4},      // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},              // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{420,580},{420,580},{0,0},{0,0}},8},      // BusyParams
	{{{800,1200},{1600,2400},{0,0},{0,0}},2},   // RingbackParams
	{{{0,0},{0,0},{0,0},{0,0}},8}},              // CongestionParams

	// CallerId Control
	{3,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	5000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Japan



//**************************************************
//* Country paramters for Algeria                 *
//**************************************************

static const CtryPrmsStruct Country_02 =
{
    // T.35 Country Identification
    T35_ALGERIA,                   // T35Countrycode:

    // Country Name
    "Algeria",                     // International string

    // Transmit Level
    {100,                            // Default: 100 for -10.0dBm
    0},                              // Compensation: 1 for TBR21

    // SDAA Control Parameters
    {2,                              // Line impedance
    1,                              // DC curve
    1,                              // Pulse make resistance
    0,                              // Fast pulse edge
    0,                              // Initial DAC value      (if 0 uses default)
    68,                             // Ring Max frequency: Hz (if 0 uses default)
    15,                             // Ring Min frequency: Hz (if 0 uses default)
    0,                              // Ring threshold:     V  (if 0 uses default)
    0,                              // Low ring impedance     (if 0 uses default)
    0,                              // Ring detect at offhook (if 0 uses default)
    0,                              // Line in use threshold  (if 0 uses default)
    0,                              // Extension threshold    (if 0 uses default)
    0,                              // Digital line threshold (if 0 uses default)
    0},                              // DIB power              (if 0 uses default)

    // External Phone Relay Control Parameters
    {SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
    MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
    MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
    MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
    MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
    MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

    300,                            // DelayPhoneOffLine: 300 ms
    300,                            // DelayPhoneToLine:  300 ms

    // Pulse Dial Parameters
    {1,                              // AllowPulseDialing
    0,                              // PulseDialMode: Standard
    10,                             // PulseSpeed
    40,                             // PulseMakeRatio
    60,                             // PulseBreakRatio
    0,                              // PulseMakeOffset: No adjustment
    0,                              // PulseBreakOffset: No adjustment
    90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
    90,                             // PulseClearTime: Disable Dialshunt if 0
    900,                            // PulseInterd: ms
    0,                              // LineMuteDelay: ms
    0,                              // DisablePulseAfterTone:
    0},                              // DisableHearingPulseDialing:

    // DTMF Dial Parameters
    {85,                             // DTMFOnTime:     ms (if 0 uses S11)
    85,                             // DTMFInterdigit: ms (if 0 uses S11)
    80,                             // LowDTMFLevel
    60,                             // HighDTMFLevel
    0,                              // DTMFCompensation
    1,                              // AllowDTMFabcd
    0},                              // DisableHearingDTMFDialing:

    // Ring Detection Parameters
    {150,                            // RingStable: ms
    0,                              // MinTimeBeforeAnswering: ASAP
    300,                            // MinDelayBtwnRings:
    8000,                           // MaxDelayBtwnRings: ms
    0,                              // MinRingOnTime
    0,                              // MinImmediateRingOn
    400},                            // MaxImmediateRingOn

    // Filter Parameters
    //Primary ToneA
    {{0,                              // Use ToneA
    {0x1923,                         // ToneCoeffs
    0xCD3C,
    0x1A2C,
    0xC467,
    0x774F},
    {0x1923,
    0xD7DB,
    0x1A2C,
    0xC774,
    0x69BC},
    0x7B30,                         // LPBFK
    0x06CF,                         // LPGAIN
    0x0180,                         // THRESHU
    0x0180,                         // THRESHL
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
    0},                              // SqDisState
    //Alternate ToneA
    {0,                              // Use ToneA
    {0x0000,                         // AltToneCoeffs
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    {0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    0x0000,                         // LPBFK
    0x0000,                         // LPGAIN
    0,                              // THRESHU
    0,                              // THRESHL
    {0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
    0}},                              // SqDisState

    // Thresholds Parameters
    {{0x0180,                         // DialThreshU
    0x0180},                         // DialThreshL
    {0x0200,                         // AltDialThreshU
    0x0200},                         // AltDialThreshL
    {0x0200,                         // WDialThreshU
    0x0200},                         // WDialThreshL
    {0x0200,                         // AltWDialThreshU
    0x0200},                         // AltWDialThreshL
    {0x0200,                         // ProgThreshU
    0x0200},                         // ProgThreshL
    {0x0200,                         // AltProgThreshU
    0x0200},                         // AltProgThreshL
    0,                              // DTMFRxThresh
    0},                              // RLSDXRTHOffset

    // Tone Detection Parameters
    {750,                            // DialStable
    750,                            // WDialStable
    300,                            // AnswerStable
    70,                             // ProgressStable
    70,                             // DialtoneWaitTime: ms
    0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

    // Redial Parameter
    {2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing after INCOMING: ms

    // Billing Delay
    2000,                           // BillingDelay: ms

    // Maximum allowable pause time for (,)
    0,                              // Default is 0 which implies that there is NO limit

    // Falsh delay before dialing a DTMF digit
    0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

    // TONE CADENCE
    {2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
    {{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
    {{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
    {{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

    // CallerId Control
    {4,                              // Cid_TYPE: 1=French
    0,                              // Cid_Prefix: initial digit in DTMF type CID
    0,                              // Cid_Suffix: final digit in DTMF type CID
    0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
    0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
    3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
    0,                              // PreRing_Cid_Indicator:
    0},                              // PolarityReversal

    // Option Control
    {0,                              // AltDialToneFilter
    0,                              // AltCallProgressFilter
    0,                              // AltWToneFilter
    0,                              // CNGFilterSelection
    0,                              // EnforceDialTone
    0,                              // EnforceBusyTone
    0,                              // LongToneIsBusy
    0,                              // EPTforV29
    0},                              // Spare0

    CTY_SIGNATURE                   // Signature
}; //End of Algeria



//**************************************************
//* Country paramters for Germany                  *
//**************************************************

static const CtryPrmsStruct Country_04 =
{
	// T.35 Country Identification
	T35_GERMANY,                    // T35Countrycode:

	// Country Name
	"Germany",                      // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Germany



//**************************************************
//* Country paramters for Argentina                *
//**************************************************

static const CtryPrmsStruct Country_07 =
{
	// T.35 Country Identification
	T35_ARGENTINA,                  // T35Countrycode:

	// Country Name
	"Argentina",                    // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{150,450},{100,350},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,450},{250,550},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Argentina



//**************************************************
//* Country paramters for Australia                *
//**************************************************

static const CtryPrmsStruct Country_09 =
{
	// T.35 Country Identification
	T35_AUSTRALIA,                  // T35Countrycode:

	// Country Name
	"Australia",                    // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	4,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	70,                             // Ring Max frequency: Hz (if 0 uses default)
	14,                             // Ring Min frequency: Hz (if 0 uses default)
	19,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	33,                             // PulseMakeRatio
	67,                             // PulseBreakRatio
	0,                              // PulseMakeOffset:  No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: ms Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: ms Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay:
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	70,                             // LowDTMFLevel:  -7.0 dBm
	50,                             // HighDTMFLevel: -5.0 dBm
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{240,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x180,                          // THRESHU
	0x180,                          // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0D67,                         // AltToneCoeffs
	0xE463,
	0x0E3E,
	0xC5A2,
	0x7931},
	{0x7FFF,
	0xFC2F,
	0x0E3E,
	0xC5A2,
	0x70EE},
	0x7B30,                         // LPBFK
	0x04CF,                         // LPGAIN
	0x0000,                         // THRESHU
	0x0000,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable: ms
	750,                            // WDialStable: ms
	1000,                           // AnswerStable: ms
	5,                              // ProgressStable: ms
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime: ms (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Flash delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                              // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},              // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},              // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,400},{300,400},{0,0},{0,0}},8},      // BusyParams
	{{{350,450},{150,250},{350,450},{1800,2200}},2},// RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},      // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=No Caller ID
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{1,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	1,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Australia



//**************************************************
//* Country paramters for Austria                  *
//**************************************************

static const CtryPrmsStruct Country_0A =
{
	// T.35 Country Identification
	T35_AUSTRIA,                    // T35Countrycode:

	// Country Name
	"Austria",                      // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Austria



//**************************************************
//* Country paramters for Bahamas                  *
//**************************************************

static const CtryPrmsStruct Country_0B =
{
	// T.35 Country Identification
	T35_BAHAMAS,                   // T35Countrycode:

	// Country Name
	"Bahamas",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Bahamas



//**************************************************
//* Country paramters for Bahrain                  *
//**************************************************

static const CtryPrmsStruct Country_0C =
{
	// T.35 Country Identification
	T35_BAHRAIN,                   // T35Countrycode:

	// Country Name
	"Bahrain",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Bahrain



//**************************************************
//* Country paramters for Belgium                  *
//**************************************************

static const CtryPrmsStruct Country_0F =
{
	// T.35 Country Identification
	T35_BELGIUM,                    // T35Countrycode:

	// Country Name
	"Belgium",                      // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Belgium



//**************************************************
//* Country paramters for Bolivia                  *
//**************************************************

static const CtryPrmsStruct Country_14 =
{
    // T.35 Country Identification
    T35_BOLIVIA,                   // T35Countrycode:

    // Country Name
    "Bolivia",                     // International string

    // Transmit Level
    {100,                            // Default: 100 for -10.0dBm
    0},                              // Compensation: 1 for TBR21

    // SDAA Control Parameters
    {2,                              // Line impedance
    1,                              // DC curve
    1,                              // Pulse make resistance
    0,                              // Fast pulse edge
    0,                              // Initial DAC value      (if 0 uses default)
    68,                             // Ring Max frequency: Hz (if 0 uses default)
    15,                             // Ring Min frequency: Hz (if 0 uses default)
    0,                              // Ring threshold:     V  (if 0 uses default)
    0,                              // Low ring impedance     (if 0 uses default)
    0,                              // Ring detect at offhook (if 0 uses default)
    0,                              // Line in use threshold  (if 0 uses default)
    0,                              // Extension threshold    (if 0 uses default)
    0,                              // Digital line threshold (if 0 uses default)
    0},                              // DIB power              (if 0 uses default)

    // External Phone Relay Control Parameters
    {SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
    MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
    MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
    MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
    MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
    MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

    300,                            // DelayPhoneOffLine: 300 ms
    300,                            // DelayPhoneToLine:  300 ms

    // Pulse Dial Parameters
    {1,                              // AllowPulseDialing
    0,                              // PulseDialMode: Standard
    10,                             // PulseSpeed
    40,                             // PulseMakeRatio
    60,                             // PulseBreakRatio
    0,                              // PulseMakeOffset: No adjustment
    0,                              // PulseBreakOffset: No adjustment
    90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
    90,                             // PulseClearTime: Disable Dialshunt if 0
    900,                            // PulseInterd: ms
    0,                              // LineMuteDelay: ms
    0,                              // DisablePulseAfterTone:
    0},                              // DisableHearingPulseDialing:

    // DTMF Dial Parameters
    {85,                             // DTMFOnTime:     ms (if 0 uses S11)
    85,                             // DTMFInterdigit: ms (if 0 uses S11)
    80,                             // LowDTMFLevel
    60,                             // HighDTMFLevel
    0,                              // DTMFCompensation
    1,                              // AllowDTMFabcd
    0},                              // DisableHearingDTMFDialing:

    // Ring Detection Parameters
    {150,                            // RingStable: ms
    0,                              // MinTimeBeforeAnswering: ASAP
    300,                            // MinDelayBtwnRings:
    8000,                           // MaxDelayBtwnRings: ms
    0,                              // MinRingOnTime
    0,                              // MinImmediateRingOn
    400},                            // MaxImmediateRingOn

    // Filter Parameters
    //Primary ToneA
    {{0,                              // Use ToneA
    {0x1923,                         // ToneCoeffs
    0xCD3C,
    0x1A2C,
    0xC467,
    0x774F},
    {0x1923,
    0xD7DB,
    0x1A2C,
    0xC774,
    0x69BC},
    0x7B30,                         // LPBFK
    0x06CF,                         // LPGAIN
    0x0180,                         // THRESHU
    0x0180,                         // THRESHL
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
    0},                              // SqDisState
    //Alternate ToneA
    {0,                              // Use ToneA
    {0x0000,                         // AltToneCoeffs
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    {0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    0x0000,                         // LPBFK
    0x0000,                         // LPGAIN
    0,                              // THRESHU
    0,                              // THRESHL
    {0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
    0}},                              // SqDisState

    // Thresholds Parameters
    {{0x0180,                         // DialThreshU
    0x0180},                         // DialThreshL
    {0x0200,                         // AltDialThreshU
    0x0200},                         // AltDialThreshL
    {0x0200,                         // WDialThreshU
    0x0200},                         // WDialThreshL
    {0x0200,                         // AltWDialThreshU
    0x0200},                         // AltWDialThreshL
    {0x0200,                         // ProgThreshU
    0x0200},                         // ProgThreshL
    {0x0200,                         // AltProgThreshU
    0x0200},                         // AltProgThreshL
    0,                              // DTMFRxThresh
    0},                              // RLSDXRTHOffset

    // Tone Detection Parameters
    {750,                            // DialStable
    750,                            // WDialStable
    300,                            // AnswerStable
    70,                             // ProgressStable
    70,                             // DialtoneWaitTime: ms
    0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

    // Redial Parameter
    {2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing after INCOMING: ms

    // Billing Delay
    2000,                           // BillingDelay: ms

    // Maximum allowable pause time for (,)
    0,                              // Default is 0 which implies that there is NO limit

    // Falsh delay before dialing a DTMF digit
    0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

    // TONE CADENCE
    {2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
    {{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
    {{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
    {{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

    // CallerId Control
    {4,                              // Cid_TYPE: 1=French
    0,                              // Cid_Prefix: initial digit in DTMF type CID
    0,                              // Cid_Suffix: final digit in DTMF type CID
    0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
    0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
    3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
    0,                              // PreRing_Cid_Indicator:
    0},                              // PolarityReversal

    // Option Control
    {0,                              // AltDialToneFilter
    0,                              // AltCallProgressFilter
    0,                              // AltWToneFilter
    0,                              // CNGFilterSelection
    0,                              // EnforceDialTone
    0,                              // EnforceBusyTone
    0,                              // LongToneIsBusy
    0,                              // EPTforV29
    0},                              // Spare0

    CTY_SIGNATURE                   // Signature
}; //End of Bolivia



//**************************************************
//* Country paramters for Brazil                   *
//**************************************************

static const CtryPrmsStruct Country_16 =
{
	// T.35 Country Identification
	T35_BRAZIL,                     // T35Countrycode:

	// Country Name
	"Brazil",                       // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	1,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	100,                            // LowDTMFLevel
	80,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{150,600},{150,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,350},{150,350},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Brazil



//**************************************************
//* Country paramters for Bulgaria                 *
//**************************************************

static const CtryPrmsStruct Country_1B =
{
	// T.35 Country Identification
	T35_BULGARIA,                 // T35Countrycode:

	// Country Name
	"Bulgaria",                   // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,350},{150,850},{500,850},{650,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{120,600},{120,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Bulgaria



//**************************************************
//* Country paramters for Canada                   *
//**************************************************

static const CtryPrmsStruct Country_20 =
{
	// T.35 Country Identification
	T35_CANADA,                     // T35Countrycode:

	// Country Name
	"Canada",                       // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	0,                              // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	0,                              // PulseClearTime: Disable Dialshunt if 0
	750,                            // PulseInterd: ms
	0,                              // LineMuteDelay:
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{92,                             // DTMFOnTime:     ms (if 0 uses S11)
	72,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel:  -8.0 dBm
	60,                             // HighDTMFLevel: -6.0 dBm
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ms ASAP
	300,                            // MinDelayBtwnRings: ms
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x08C2,                         // ToneCoeffs
	0xEE7C,
	0x08C2,
	0xC774,
	0x74FE},
	{0x08C2,
	0x1184,
	0x08C2,
	0xCD4F,
	0x6495},
	0x7B30,                         // LPBFK
	0x04CF,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0000,                         // DialThreshU
	0x0000},                         // DialThreshL
	{0x0000,                         // AltDialThreshU
	0x0000},                         // AltDialThreshL
	{0x0000,                         // WDialThreshU
	0x0000},                         // WDialThreshL
	{0x0000,                         // AltWDialThreshU
	0x0000},                         // AltWDialThreshL
	{0x0000,                         // ProgThreshU
	0x0000},                         // ProgThreshL
	{0x0000,                         // AltProgThreshU
	0x0000},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable: ms
	750,                            // WDialStable: ms
	300,                            // AnswerStable: ms
	5,                              // ProgressStable: ms
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime: ms (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Flash delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                              // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},              // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},              // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{420,580},{420,580},{0,0},{0,0}},8},      // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},   // RingbackParams
	{{{220,280},{220,280},{0,0},{0,0}},8}},      // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE: 1=US type
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Canada



//**************************************************
//* Country paramters for Chile                    *
//**************************************************

static const CtryPrmsStruct Country_25 =
{
	// T.35 Country Identification
	T35_CHILE,                      // T35Countrycode:

	// Country Name
	"Chile",                        // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{0,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	20,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	100,                            // LowDTMFLevel
	80,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Chile



//**************************************************
//* Country paramters for China                    *
//**************************************************

static const CtryPrmsStruct Country_26 =
{
	// T.35 Country Identification
	T35_CHINA,                      // T35Countrycode:

	// Country Name
	"China",                        // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{150,450},{150,450},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{400,650},{400,640},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of China



//**************************************************
//* Country paramters for Colombia                 *
//**************************************************

static const CtryPrmsStruct Country_27 =
{
	// T.35 Country Identification
	T35_COLOMBIA,                   // T35Countrycode:

	// Country Name
	"Colombia",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Colombia



//**************************************************
//* Country paramters for Costa Rica               *
//**************************************************

static const CtryPrmsStruct Country_2B =
{
	// T.35 Country Identification
	T35_COSTA_RICA,                   // T35Countrycode:

	// Country Name
	"Costa Rica",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Costa Rica



//**************************************************
//* Country paramters for Cuba                     *
//**************************************************

static const CtryPrmsStruct Country_2C =
{
	// T.35 Country Identification
	T35_CUBA,                   // T35Countrycode:

	// Country Name
	"Cuba",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Colombia



//**************************************************
//* Country paramters for Cyprus                   *
//**************************************************

static const CtryPrmsStruct Country_2D =
{
	// T.35 Country Identification
	T35_CYPRUS,                     // T35Countrycode:

	// Country Name
	"Cyprus",                       // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,325},{150,325},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Cyprus



//**************************************************
//* Country paramters for Czech Republic           *
//**************************************************

static const CtryPrmsStruct Country_2E =
{
	// T.35 Country Identification
	T35_CZECH_REPUBLIC,             // T35Countrycode:

	// Country Name
	"Czech Republic",               // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{95,                             // DTMFOnTime:     ms (if 0 uses S11)
	95,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	500,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{600,750},{600,750},{0,0},{0,0}},2},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,600},{100,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},4},    // RingbackParams
	{{{100,600},{100,600},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Czech Republic



//**************************************************
//* Country paramters for Denmark                  *
//**************************************************

static const CtryPrmsStruct Country_31 =
{
	// T.35 Country Identification
	T35_DENMARK,                    // T35Countrycode:

	// Country Name
	"Denmark",                      // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Denmark



//**************************************************
//* Country paramters for Dominican Republic       *
//**************************************************

static const CtryPrmsStruct Country_33 =
{
	// T.35 Country Identification
	T35_DOMINICAN_REPUBLIC,         // T35Countrycode:

	// Country Name
	"Dominican Republic",           // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0}, 				// DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{500,900},{500,900},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{250,500},{250,500},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Dominican Republic



//**************************************************
//* Country paramters for Ecuador                  *
//**************************************************

static const CtryPrmsStruct Country_35 =
{
	// T.35 Country Identification
	T35_ECUADOR,                   // T35Countrycode:

	// Country Name
	"Ecuador",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0}, 				// DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{150,600},{150,600},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{150,450},{150,450},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Ecuador



//**************************************************
//* Country paramters for Egypt                    *
//**************************************************

static const CtryPrmsStruct Country_36 =
{
	// T.35 Country Identification
	T35_EGYPT,                   // T35Countrycode:

	// Country Name
	"Egypt",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Egypt


//**************************************************
//* Country paramters for El Salvador              *
//**************************************************

static const CtryPrmsStruct Country_37 =
{
	// T.35 Country Identification
	T35_EL_SALVADOR,                 // T35Countrycode:

	// Country Name
	"El Salvador",                   // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	75,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0}, 				// DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of El Salvador



//**************************************************
//* Country paramters for Finland                  *
//**************************************************

static const CtryPrmsStruct Country_3C =
{
	// T.35 Country Identification
	T35_FINLAND,                    // T35Countrycode:

	// Country Name
	"Finland",                      // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Finland



//**************************************************
//* Country paramters for France                   *
//**************************************************

static const CtryPrmsStruct Country_3D =
{
	// T.35 Country Identification
	T35_FRANCE,                     // T35Countrycode:

	// Country Name
	"France",                       // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	25,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	200,                            // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	7500,                           // MaxDelayBtwnRings: 7.5 seconds
	500,                            // MinRingOnTime
	200,                            // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of France



//**************************************************
//* Country paramters for Greece                   *
//**************************************************

static const CtryPrmsStruct Country_46 =
{
	// T.35 Country Identification
	T35_GREECE,                   // T35Countrycode:

	// Country Name
	"Greece",                     // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{100,300},{100,400},{500,800},{650,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{200,400},{200,400},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},    // RingbackParams
	{{{100,200},{100,200},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Greece



//**************************************************
//* Country paramters for Guatemala                *
//**************************************************

static const CtryPrmsStruct Country_49 =
{
	// T.35 Country Identification
	T35_GUATEMALA,                   // T35Countrycode:

	// Country Name
	"Guatemala",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Guatemala



//**************************************************
//* Country paramters for Honduras                 *
//**************************************************

static const CtryPrmsStruct Country_4F =
{
	// T.35 Country Identification
	T35_HONDURAS,                   // T35Countrycode:

	// Country Name
	"Honduras",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{100,200},{100,200},{0,0},{0,0}},4}, 		// DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{200,300},{200,300},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Honduras



//**************************************************
//* Country paramters for Hong Kong                *
//**************************************************

static const CtryPrmsStruct Country_50 =
{
	// T.35 Country Identification
	T35_HONGKONG,                   // T35Countrycode:

	// Country Name
	"Hong Kong",                    // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{100,300},{300,600},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Hong Kong



//**************************************************
//* Country paramters for Hungary                  *
//**************************************************

static const CtryPrmsStruct Country_51 =
{
	// T.35 Country Identification
	T35_HUNGARY,                    // T35Countrycode:

	// Country Name
	"Hungary",                      // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable 300 ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{150,600},{150,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Hungary



//**************************************************
//* Country paramters for Iceland                  *
//**************************************************

static const CtryPrmsStruct Country_52 =
{
	// T.35 Country Identification
	T35_ICELAND,                    // T35Countrycode:

	// Country Name
	"Iceland",                      // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{350,650},{350,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{130,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Iceland



//**************************************************
//* Country paramters for India                    *
//**************************************************

static const CtryPrmsStruct Country_53 =
{
    // T.35 Country Identification
    T35_INDIA,                    // T35Countrycode:

    // Country Name
    "India",                      // International string

    // Transmit Level
    {100,                            // Default: 100 for -10.0dBm
    0},                              // Compensation: 1 for TBR21

    // SDAA Control Parameters
    {1,                              // Line impedance
    3,                              // DC curve
    1,                              // Pulse make resistance
    0,                              // Fast pulse edge
    0,                              // Initial DAC value      (if 0 uses default)
    68,                             // Ring Max frequency: Hz (if 0 uses default)
    15,                             // Ring Min frequency: Hz (if 0 uses default)
    25,                             // Ring threshold:     V  (if 0 uses default)
    0,                              // Low ring impedance     (if 0 uses default)
    0,                              // Ring detect at offhook (if 0 uses default)
    0,                              // Line in use threshold  (if 0 uses default)
    0,                              // Extension threshold    (if 0 uses default)
    0,                              // Digital line threshold (if 0 uses default)
    0},                              // DIB power              (if 0 uses default)

    // External Phone Relay Control Parameters
    {SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
    MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
    MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
    MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
    MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
    MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

    300,                            // DelayPhoneOffLine: 300 ms
    300,                            // DelayPhoneToLine:  300 ms

    // Pulse Dial Parameters
    {1,                              // AllowPulseDialing
    0,                              // PulseDialMode: Standard
    10,                             // PulseSpeed
    40,                             // PulseMakeRatio
    60,                             // PulseBreakRatio
    0,                              // PulseMakeOffset: No adjustment
    0,                              // PulseBreakOffset: No adjustment
    90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
    90,                             // PulseClearTime: Disable Dialshunt if 0
    900,                            // PulseInterd: ms
    0,                              // LineMuteDelay: ms
    0,                              // DisablePulseAfterTone:
    0},                              // DisableHearingPulseDialing:

    // DTMF Dial Parameters
    {85,                             // DTMFOnTime:     ms (if 0 uses S11)
    85,                             // DTMFInterdigit: ms (if 0 uses S11)
    110,                            // LowDTMFLevel -11.0 dBV
    90,                             // HighDTMFLevel -9.0 dBV
    1,                              // DTMFCompensation: 1 for TBR21 (dBV)
    1,                              // AllowDTMFabcd
    0},                              // DisableHearingDTMFDialing:

    // Ring Detection Parameters
    {300,                            // RingStable: ms
    0,                              // MinTimeBeforeAnswering: ASAP
    300,                            // MinDelayBtwnRings:
    8000,                           // MaxDelayBtwnRings: ms
    0,                              // MinRingOnTime
    0,                              // MinImmediateRingOn
    0},                              // MaxImmediateRingOn

    // Filter Parameters
    //Primary ToneA
    {{0,                              // Use ToneA
    {0x1923,                         // ToneCoeffs
    0xCD3C,
    0x1A2C,
    0xC467,
    0x774F},
    {0x1923,
    0xD7DB,
    0x1A2C,
    0xC774,
    0x69BC},
    0x7B30,                         // LPBFK
    0x06CF,                         // LPGAIN
    0x0180,                         // THRESHU
    0x0180,                         // THRESHL
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
    0},                              // SqDisState
    //Alternate ToneA
    {0,                              // Use ToneA
    {0x0000,                         // AltToneCoeffs
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    {0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    0x0000,                         // LPBFK
    0x0000,                         // LPGAIN
    0,                              // THRESHU
    0,                              // THRESHL
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
    0}},                              // SqDisState

    // Thresholds Parameters
    {{0x0180,                         // DialThreshU
    0x0180},                         // DialThreshL
    {0x0200,                         // AltDialThreshU
    0x0200},                         // AltDialThreshL
    {0x0200,                         // WDialThreshU
    0x0200},                         // WDialThreshL
    {0x0200,                         // AltWDialThreshU
    0x0200},                         // AltWDialThreshL
    {0x0200,                         // ProgThreshU
    0x0200},                         // ProgThreshL
    {0x0200,                         // AltProgThreshU
    0x0200},                         // AltProgThreshL
    0,                              // DTMFRxThresh
    0},                              // RLSDXRTHOffset

    // Tone Detection Parameters
    {750,                            // DialStable
    750,                            // WDialStable
    300,                            // AnswerStable
    70,                             // ProgressStable
    70,                             // DialtoneWaitTime: ms
    0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

    // Redial Parameter
    {2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing after INCOMING: ms

    // Billing Delay
    2000,                           // BillingDelay: ms

    // Maximum allowable pause time for (,)
    0,                              // Default is 0 which implies that there is NO limit

    // Falsh delay before dialing a DTMF digit
    0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

    // TONE CADENCE
    {1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
    {{{300,500},{1800,2200},{0,0},{0,0}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{200,950},{200,950},{0,0},{0,0}},8},       // BusyParams
    {{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
    {{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

    // CallerId Control
    {0,                              // Cid_TYPE: 0=none
    0,                              // Cid_Prefix: initial digit in DTMF type CID
    0,                              // Cid_Suffix: final digit in DTMF type CID
    0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
    0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
    0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
    0,                              // PreRing_Cid_Indicator:
    0},                              // PolarityReversal

    // Option Control
    {0,                              // AltDialToneFilter
    0,                              // AltCallProgressFilter
    0,                              // AltWToneFilter
    0,                              // CNGFilterSelection
    0,                              // EnforceDialTone
    0,                              // EnforceBusyTone
    0,                              // LongToneIsBusy
    0,                              // EPTforV29
    0},                              // Spare0

    CTY_SIGNATURE                   // Signature
}; //End of India



//**************************************************
//* Country paramters for Indonesia                *
//**************************************************

static const CtryPrmsStruct Country_54 =
{
	// T.35 Country Identification
	T35_INDONESIA,                   // T35Countrycode:

	// Country Name
	"Indonesia",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	100,                            // LowDTMFLevel
	80,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{200,650},{200,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{220,280},{220,280},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Indonesia


//**************************************************
//* Country paramters for Iran                     *
//**************************************************

static const CtryPrmsStruct Country_55 =
{
	// T.35 Country Identification
	T35_IRAN,                   // T35Countrycode:

	// Country Name
	"Iran",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Iran



//**************************************************
//* Country paramters for Iraq                     *
//**************************************************

static const CtryPrmsStruct Country_56 =
{
	// T.35 Country Identification
	T35_IRAQ,                       // T35Countrycode:

	// Country Name
	"Iraq",                         // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Iraq



//**************************************************
//* Country paramters for Ireland                  *
//**************************************************

static const CtryPrmsStruct Country_57 =
{
	// T.35 Country Identification
	T35_IRELAND,                    // T35Countrycode:

	// Country Name
	"Ireland",                      // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Ireland



//**************************************************
//* Country paramters for Italy                    *
//**************************************************

static const CtryPrmsStruct Country_59 =
{
	// T.35 Country Identification
	T35_ITALY,                      // T35Countrycode:

	// Country Name
	"Italy",                        // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{160,240},{160,240},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Italy



//**************************************************
//* Country paramters for Jamaica                  *
//**************************************************

static const CtryPrmsStruct Country_5B =
{
	// T.35 Country Identification
	T35_JAMAICA,                   // T35Countrycode:

	// Country Name
	"Jamaica",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Jamaica



//**************************************************
//* Country paramters for Jordan                   *
//**************************************************

static const CtryPrmsStruct Country_5E =
{
	// T.35 Country Identification
	T35_JORDAN,                      // T35Countrycode:

	// Country Name
	"Jordan",                        // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0}, 				// DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,875},{300,875},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},    // RingbackParams
	{{{150,325},{150,325},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Jordan



//**************************************************
//* Country paramters for Korea                    *
//**************************************************

static const CtryPrmsStruct Country_61 =
{
	// T.35 Country Identification
	T35_KOREA,                      // T35Countrycode:

	// Country Name
	"Korea",                        // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	1,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{100,300},{100,290},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Korea



//**************************************************
//* Country paramters for Kuwait                   *
//**************************************************

static const CtryPrmsStruct Country_62 =
{
	// T.35 Country Identification
	T35_KUWAIT,                     // T35Countrycode:

	// Country Name
	"Kuwait",                       // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Kuwait



//**************************************************
//* Country paramters for Lebanon                  *
//**************************************************

static const CtryPrmsStruct Country_64 =
{
	// T.35 Country Identification
	T35_LEBANON,                    // T35Countrycode:

	// Country Name
	"Lebanon",                      // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0}, 				// DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{150,300},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Lebanon



//**************************************************
//* Country paramters for Libya                    *
//**************************************************

static const CtryPrmsStruct Country_67 =
{
	// T.35 Country Identification
	T35_LIBYAN_ARAB_JAMAHIRIYA,  // T35Countrycode:

	// Country Name
	"Libya",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Libya



//**************************************************
//* Country paramters for Liechtenstein            *
//**************************************************

static const CtryPrmsStruct Country_68 =
{
	// T.35 Country Identification
	T35_LIECHTENSTEIN,              // T35Countrycode:

	// Country Name
	"Liechtenstein",                // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{350,650},{350,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{130,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Liechtenstein



//**************************************************
//* Country paramters for Luxembourg               *
//**************************************************

static const CtryPrmsStruct Country_69 =
{
	// T.35 Country Identification
	T35_LUXEMBOURG,                 // T35Countrycode:

	// Country Name
	"Luxembourg",                   // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{350,650},{350,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{130,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Luxembourg



//**************************************************
//* Country paramters for Malaysia                 *
//**************************************************

static const CtryPrmsStruct Country_6C =
{
	// T.35 Country Identification
	T35_MALAYSIA,                   // T35Countrycode:

	// Country Name
	"Malaysia",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	14,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	100,                            // LowDTMFLevel
	80,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Malaysia



//**************************************************
//* Country paramters for Malta	                   *
//**************************************************

static const CtryPrmsStruct Country_70 =
{
	// T.35 Country Identification
	T35_MALTA,                      // T35Countrycode:

	// Country Name
	"Malta",                        // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,325},{150,325},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Malta



//**************************************************
//* Country paramters for Mexico                   *
//**************************************************

static const CtryPrmsStruct Country_73 =
{
	// T.35 Country Identification
	T35_MEXICO,                     // T35Countrycode:

	// Country Name
	"Mexico",                       // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	4,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{150,300},{150,300},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Mexico



//**************************************************
//* Country paramters for Nepal                     *
//**************************************************

static const CtryPrmsStruct Country_7A =
{
	// T.35 Country Identification
	T35_NEPAL,                   // T35Countrycode:

	// Country Name
	"Nepal",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Nepal



//**************************************************
//* Country paramters for Netherlands              *
//**************************************************

static const CtryPrmsStruct Country_7B =
{
	// T.35 Country Identification
	T35_NETHERLANDS,                // T35Countrycode:

	// Country Name
	"Netherlands",                  // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Netherlands



//**************************************************
//* Country paramters for New Zealand              *
//**************************************************

static const CtryPrmsStruct Country_7E =
{
	// T.35 Country Identification
	T35_NEW_ZEALAND,                // T35Countrycode:

	// Country Name
	"New Zealand",                  // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	1,                              // PulseDialMode:
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{95,                             // DTMFOnTime:     ms (if 0 uses S11)
	95,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{350,450},{150,250},{350,450},{1800,2200}},2},    // RingbackParams
	{{{60,300},{60,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of New Zealand



//**************************************************
//* Country paramters for Nicaragua                *
//**************************************************

static const CtryPrmsStruct Country_7F =
{
	// T.35 Country Identification
	T35_NICARAGUA,                   // T35Countrycode:

	// Country Name
	"Nicaragua",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Nicaragua



//**************************************************
//* Country paramters for Norway                   *
//**************************************************

static const CtryPrmsStruct Country_82 =
{
	// T.35 Country Identification
	T35_NORWAY,                     // T35Countrycode:

	// Country Name
	"Norway",                       // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Norway



//**************************************************
//* Country paramters for Oman                     *
//**************************************************

static const CtryPrmsStruct Country_83 =
{
	// T.35 Country Identification
	T35_OMAN,                       // T35Countrycode:

	// Country Name
	"Oman",                         // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Oman



//**************************************************
//* Country paramters for Panama                   *
//**************************************************

static const CtryPrmsStruct Country_85 =
{
	// T.35 Country Identification
	T35_PANAMA,                     // T35Countrycode:

	// Country Name
	"Panama",                       // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Panama



//**************************************************
//* Country parameters for Paraguay                *
//**************************************************

static const CtryPrmsStruct Country_87 =
{
    // T.35 Country Identification
    T35_PARAGUAY,                   // T35Countrycode:

    // Country Name
    "Paraguay",                     // International string

    // Transmit Level
    {100,                            // Default: 100 for -10.0dBm
    0},                              // Compensation: 1 for TBR21

    // SDAA Control Parameters
    {2,                              // Line impedance
    1,                              // DC curve
    1,                              // Pulse make resistance
    0,                              // Fast pulse edge
    0,                              // Initial DAC value      (if 0 uses default)
    68,                             // Ring Max frequency: Hz (if 0 uses default)
    15,                             // Ring Min frequency: Hz (if 0 uses default)
    0,                              // Ring threshold:     V  (if 0 uses default)
    0,                              // Low ring impedance     (if 0 uses default)
    0,                              // Ring detect at offhook (if 0 uses default)
    0,                              // Line in use threshold  (if 0 uses default)
    0,                              // Extension threshold    (if 0 uses default)
    0,                              // Digital line threshold (if 0 uses default)
    0},                              // DIB power              (if 0 uses default)

    // External Phone Relay Control Parameters
    {SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
    MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
    MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
    MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
    MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
    MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

    300,                            // DelayPhoneOffLine: 300 ms
    300,                            // DelayPhoneToLine:  300 ms

    // Pulse Dial Parameters
    {1,                              // AllowPulseDialing
    0,                              // PulseDialMode: Standard
    10,                             // PulseSpeed
    40,                             // PulseMakeRatio
    60,                             // PulseBreakRatio
    0,                              // PulseMakeOffset: No adjustment
    0,                              // PulseBreakOffset: No adjustment
    90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
    90,                             // PulseClearTime: Disable Dialshunt if 0
    900,                            // PulseInterd: ms
    0,                              // LineMuteDelay: ms
    0,                              // DisablePulseAfterTone:
    0},                              // DisableHearingPulseDialing:

    // DTMF Dial Parameters
    {85,                             // DTMFOnTime:     ms (if 0 uses S11)
    85,                             // DTMFInterdigit: ms (if 0 uses S11)
    80,                             // LowDTMFLevel
    60,                             // HighDTMFLevel
    0,                              // DTMFCompensation
    1,                              // AllowDTMFabcd
    0},                              // DisableHearingDTMFDialing:

    // Ring Detection Parameters
    {300,                            // RingStable: ms
    0,                              // MinTimeBeforeAnswering: ASAP
    300,                            // MinDelayBtwnRings:
    8000,                           // MaxDelayBtwnRings: ms
    0,                              // MinRingOnTime
    0,                              // MinImmediateRingOn
    400},                            // MaxImmediateRingOn

    // Filter Parameters
    //Primary ToneA
    {{0,                              // Use ToneA
    {0x1923,                         // ToneCoeffs
    0xCD3C,
    0x1A2C,
    0xC467,
    0x774F},
    {0x1923,
    0xD7DB,
    0x1A2C,
    0xC774,
    0x69BC},
    0x7B30,                         // LPBFK
    0x06CF,                         // LPGAIN
    0x0180,                         // THRESHU
    0x0180,                         // THRESHL
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
    0},                              // SqDisState
    //Alternate ToneA
    {0,                              // Use ToneA
    {0x0000,                         // AltToneCoeffs
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    {0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    0x0000,                         // LPBFK
    0x0000,                         // LPGAIN
    0,                              // THRESHU
    0,                              // THRESHL
    {0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
    0}},                              // SqDisState

    // Thresholds Parameters
    {{0x0180,                         // DialThreshU
    0x0180},                         // DialThreshL
    {0x0200,                         // AltDialThreshU
    0x0200},                         // AltDialThreshL
    {0x0200,                         // WDialThreshU
    0x0200},                         // WDialThreshL
    {0x0200,                         // AltWDialThreshU
    0x0200},                         // AltWDialThreshL
    {0x0200,                         // ProgThreshU
    0x0200},                         // ProgThreshL
    {0x0200,                         // AltProgThreshU
    0x0200},                         // AltProgThreshL
    0,                              // DTMFRxThresh
    0},                              // RLSDXRTHOffset

    // Tone Detection Parameters
    {750,                            // DialStable
    750,                            // WDialStable
    300,                            // AnswerStable
    70,                             // ProgressStable
    70,                             // DialtoneWaitTime: ms
    0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

    // Redial Parameter
    {2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing after INCOMING: ms

    // Billing Delay
    2000,                           // BillingDelay: ms

    // Maximum allowable pause time for (,)
    0,                              // Default is 0 which implies that there is NO limit

    // Falsh delay before dialing a DTMF digit
    0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

    // TONE CADENCE
    {2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
    {{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
    {{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
    {{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

    // CallerId Control
    {4,                              // Cid_TYPE: 1=French
    0,                              // Cid_Prefix: initial digit in DTMF type CID
    0,                              // Cid_Suffix: final digit in DTMF type CID
    0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
    0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
    3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
    0,                              // PreRing_Cid_Indicator:
    0},                              // PolarityReversal

    // Option Control
    {0,                              // AltDialToneFilter
    0,                              // AltCallProgressFilter
    0,                              // AltWToneFilter
    0,                              // CNGFilterSelection
    0,                              // EnforceDialTone
    0,                              // EnforceBusyTone
    0,                              // LongToneIsBusy
    0,                              // EPTforV29
    0},                              // Spare0

    CTY_SIGNATURE                   // Signature
}; //End of Paraguay



//**************************************************
//* Country paramters for Peru                     *
//**************************************************

static const CtryPrmsStruct Country_88 =
{
	// T.35 Country Identification
	T35_PERU,                       // T35Countrycode:

	// Country Name
	"Peru",                         // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Peru



//**************************************************
//* Country paramters for Poland                   *
//**************************************************

static const CtryPrmsStruct Country_8A =
{
	// T.35 Country Identification
	T35_POLAND,                     // T35Countrycode:

	// Country Name
	"Poland",                       // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	69,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	1,                              // PulseMakeOffset: No adjustment
	1,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,700},{300,700},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Poland



//**************************************************
//* Country paramters for Portugal                 *
//**************************************************

static const CtryPrmsStruct Country_8B =
{
	// T.35 Country Identification
	T35_PORTUGAL,                   // T35Countrycode:

	// Country Name
	"Portugal",                     // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{350,650},{350,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{130,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Portugal



//**************************************************
//* Country paramters for Qatar                    *
//**************************************************

static const CtryPrmsStruct Country_8D =
{
	// T.35 Country Identification
	T35_QATAR,                   	// T35Countrycode:

	// Country Name
	"Qatar",                     	// International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Qatar



//**************************************************
//* Country paramters for Romania                  *
//**************************************************

static const CtryPrmsStruct Country_8E =
{
	// T.35 Country Identification
	T35_ROMANIA,                 // T35Countrycode:

	// Country Name
	"Romania",                   // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,300},{100,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{170,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{100,280},{120,280},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Romania



//**************************************************
//* Country paramters for Saudi Arabia             *
//**************************************************

static const CtryPrmsStruct Country_98 =
{
    // T.35 Country Identification
    T35_SAUDI_ARABIA,                      // T35Countrycode:

    // Country Name
    "Saudi Arabia",                        // International string

    // Transmit Level
    {100,                            // Default: 100 for -10.0dBm
    0},                              // Compensation: 1 for TBR21

    // SDAA Control Parameters
    {2,                              // Line impedance
    1,                              // DC curve
    1,                              // Pulse make resistance
    0,                              // Fast pulse edge
    0,                              // Initial DAC value      (if 0 uses default)
    68,                             // Ring Max frequency: Hz (if 0 uses default)
    15,                             // Ring Min frequency: Hz (if 0 uses default)
    0,                              // Ring threshold:     V  (if 0 uses default)
    0,                              // Low ring impedance     (if 0 uses default)
    0,                              // Ring detect at offhook (if 0 uses default)
    0,                              // Line in use threshold  (if 0 uses default)
    0,                              // Extension threshold    (if 0 uses default)
    0,                              // Digital line threshold (if 0 uses default)
    0},                              // DIB power              (if 0 uses default)

    // External Phone Relay Control Parameters
    {SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
    MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
    MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
    MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
    MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
    MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

    300,                            // DelayPhoneOffLine: 300 ms
    300,                            // DelayPhoneToLine:  300 ms

    // Pulse Dial Parameters
    {1,                              // AllowPulseDialing
    0,                              // PulseDialMode: Standard
    10,                             // PulseSpeed
    40,                             // PulseMakeRatio
    60,                             // PulseBreakRatio
    0,                              // PulseMakeOffset: No adjustment
    0,                              // PulseBreakOffset: No adjustment
    90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
    90,                             // PulseClearTime: Disable Dialshunt if 0
    900,                            // PulseInterd: ms
    0,                              // LineMuteDelay: ms
    0,                              // DisablePulseAfterTone:
    0},                              // DisableHearingPulseDialing:

    // DTMF Dial Parameters
    {85,                             // DTMFOnTime:     ms (if 0 uses S11)
    85,                             // DTMFInterdigit: ms (if 0 uses S11)
    80,                             // LowDTMFLevel
    60,                             // HighDTMFLevel
    0,                              // DTMFCompensation
    1,                              // AllowDTMFabcd
    0},                              // DisableHearingDTMFDialing:

    // Ring Detection Parameters
    {150,                            // RingStable: ms
    0,                              // MinTimeBeforeAnswering: ASAP
    300,                            // MinDelayBtwnRings:
    8000,                           // MaxDelayBtwnRings: ms
    0,                              // MinRingOnTime
    0,                              // MinImmediateRingOn
    400},                            // MaxImmediateRingOn

    // Filter Parameters
    //Primary ToneA
    {{0,                              // Use ToneA
    {0x1923,                         // ToneCoeffs
    0xCD3C,
    0x1A2C,
    0xC467,
    0x774F},
    {0x1923,
    0xD7DB,
    0x1A2C,
    0xC774,
    0x69BC},
    0x7B30,                         // LPBFK
    0x06CF,                         // LPGAIN
    0x0180,                         // THRESHU
    0x0180,                         // THRESHL
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
    0},                              // SqDisState
    //Alternate ToneA
    {0,                              // Use ToneAREFERENCE,1,01
    {0x0000,                         // AltToneCoeffs
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    {0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    0x0000,                         // LPBFK
    0x0000,                         // LPGAIN
    0,                              // THRESHU
    0,                              // THRESHL
    {0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
    0}},                              // SqDisState

    // Thresholds Parameters
    {{0x0180,                         // DialThreshU
    0x0180},                         // DialThreshL
    {0x0200,                         // AltDialThreshU
    0x0200},                         // AltDialThreshL
    {0x0200,                         // WDialThreshU
    0x0200},                         // WDialThreshL
    {0x0200,                         // AltWDialThreshU
    0x0200},                         // AltWDialThreshL
    {0x0200,                         // ProgThreshU
    0x0200},                         // ProgThreshL
    {0x0200,                         // AltProgThreshU
    0x0200},                         // AltProgThreshL
    0,                              // DTMFRxThresh
    0},                              // RLSDXRTHOffset

    // Tone Detection Parameters
    {750,                            // DialStable
    750,                            // WDialStable
    300,                            // AnswerStable
    70,                             // ProgressStable
    70,                             // DialtoneWaitTime: ms
    0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

    // Redial Parameter
    {2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing after INCOMING: ms

    // Billing Delay
    2000,                           // BillingDelay: ms

    // Maximum allowable pause time for (,)
    0,                              // Default is 0 which implies that there is NO limit

    // Falsh delay before dialing a DTMF digit
    0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

    // TONE CADENCE
    {2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
    {{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
    {{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
    {{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

    // CallerId Control
    {4,                              // Cid_TYPE: 1=French
    0,                              // Cid_Prefix: initial digit in DTMF type CID
    0,                              // Cid_Suffix: final digit in DTMF type CID
    0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
    0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
    3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
    0,                              // PreRing_Cid_Indicator:
    0},                              // PolarityReversal

    // Option Control
    {0,                              // AltDialToneFilter
    0,                              // AltCallProgressFilter
    0,                              // AltWToneFilter
    0,                              // CNGFilterSelection
    0,                              // EnforceDialTone
    0,                              // EnforceBusyTone
    0,                              // LongToneIsBusy
    0,                              // EPTforV29
    0},                              // Spare0

    CTY_SIGNATURE                   // Signature
}; //End of Saudi Arabia



//**************************************************
//* Country paramters for Singapore                *
//**************************************************

static const CtryPrmsStruct Country_9C =
{
	// T.35 Country Identification
	T35_SINGAPORE,                  // T35Countrycode:

	// Country Name
	"Singapore",                    // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{900,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,850},{300,850},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Singapore



//**************************************************
//* Country paramters for South Africa             *
//**************************************************

static const CtryPrmsStruct Country_9F =
{
	// T.35 Country Identification
	T35_SOUTH_AFRICA,               // T35Countrycode:

	// Country Name
	"South Africa",                 // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{100,                            // DTMFOnTime:     ms (if 0 uses S11)
	100,                            // DTMFInterdigit: ms (if 0 uses S11)
	100,                            // LowDTMFLevel
	70,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{100,600},{100,600},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of South Africa



//**************************************************
//* Country paramters for Spain                    *
//**************************************************

static const CtryPrmsStruct Country_A0 =
{
	// T.35 Country Identification
	T35_SPAIN,                      // T35Countrycode:

	// Country Name
	"Spain",                        // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,650},{300,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{130,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Spain



//**************************************************
//* Country paramters for Sweden                   *
//**************************************************

static const CtryPrmsStruct Country_A5 =
{
	// T.35 Country Identification
	T35_SWEDEN,                     // T35Countrycode:

	// Country Name
	"Sweden",                       // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	2,                              // PulseDialMode:
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Sweden



//**************************************************
//* Country paramters for Switzerland              *
//**************************************************

static const CtryPrmsStruct Country_A6 =
{
	// T.35 Country Identification
	T35_SWITZERLAND,                // T35Countrycode:

	// Country Name
	"Switzerland",                  // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Switzerland



//**************************************************
//* Country paramters for Thailand                 *
//**************************************************

static const CtryPrmsStruct Country_A9 =
{
	// T.35 Country Identification
	T35_THAILAND,                   // T35Countrycode:

	// Country Name
	"Thailand",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	1,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{550,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{100,300},{100,290},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Thailand



//**************************************************
//* Country paramters for Tunisia                 *
//**************************************************

static const CtryPrmsStruct Country_AD =
{
    // T.35 Country Identification
    T35_TUNISIA,                   // T35Countrycode:

    // Country Name
    "Tunisia",                     // International string

    // Transmit Level
    {100,                            // Default: 100 for -10.0dBm
    0},                              // Compensation: 1 for TBR21

    // SDAA Control Parameters
    {2,                              // Line impedance
    1,                              // DC curve
    1,                              // Pulse make resistance
    0,                              // Fast pulse edge
    0,                              // Initial DAC value      (if 0 uses default)
    68,                             // Ring Max frequency: Hz (if 0 uses default)
    15,                             // Ring Min frequency: Hz (if 0 uses default)
    0,                              // Ring threshold:     V  (if 0 uses default)
    0,                              // Low ring impedance     (if 0 uses default)
    0,                              // Ring detect at offhook (if 0 uses default)
    0,                              // Line in use threshold  (if 0 uses default)
    0,                              // Extension threshold    (if 0 uses default)
    0,                              // Digital line threshold (if 0 uses default)
    0},                              // DIB power              (if 0 uses default)

    // External Phone Relay Control Parameters
    {SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
    MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
    MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
    MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
    MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
    MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
    MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

    300,                            // DelayPhoneOffLine: 300 ms
    300,                            // DelayPhoneToLine:  300 ms

    // Pulse Dial Parameters
    {1,                              // AllowPulseDialing
    0,                              // PulseDialMode: Standard
    10,                             // PulseSpeed
    40,                             // PulseMakeRatio
    60,                             // PulseBreakRatio
    0,                              // PulseMakeOffset: No adjustment
    0,                              // PulseBreakOffset: No adjustment
    90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
    90,                             // PulseClearTime: Disable Dialshunt if 0
    900,                            // PulseInterd: ms
    0,                              // LineMuteDelay: ms
    0,                              // DisablePulseAfterTone:
    0},                              // DisableHearingPulseDialing:

    // DTMF Dial Parameters
    {85,                             // DTMFOnTime:     ms (if 0 uses S11)
    85,                             // DTMFInterdigit: ms (if 0 uses S11)
    80,                             // LowDTMFLevel
    60,                             // HighDTMFLevel
    0,                              // DTMFCompensation
    1,                              // AllowDTMFabcd
    0},                              // DisableHearingDTMFDialing:

    // Ring Detection Parameters
    {150,                            // RingStable: ms
    0,                              // MinTimeBeforeAnswering: ASAP
    300,                            // MinDelayBtwnRings:
    8000,                           // MaxDelayBtwnRings: ms
    0,                              // MinRingOnTime
    0,                              // MinImmediateRingOn
    400},                            // MaxImmediateRingOn

    // Filter Parameters
    //Primary ToneA
    {{0,                              // Use ToneA
    {0x1923,                         // ToneCoeffs
    0xCD3C,
    0x1A2C,
    0xC467,
    0x774F},
    {0x1923,
    0xD7DB,
    0x1A2C,
    0xC774,
    0x69BC},
    0x7B30,                         // LPBFK
    0x06CF,                         // LPGAIN
    0x0180,                         // THRESHU
    0x0180,                         // THRESHL
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
    0},                              // SqDisState
    //Alternate ToneA
    {0,                              // Use ToneA
    {0x0000,                         // AltToneCoeffs
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    {0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000},
    0x0000,                         // LPBFK
    0x0000,                         // LPGAIN
    0,                              // THRESHU
    0,                              // THRESHL
    {0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
    0}},                              // SqDisState

    // Thresholds Parameters
    {{0x0180,                         // DialThreshU
    0x0180},                         // DialThreshL
    {0x0200,                         // AltDialThreshU
    0x0200},                         // AltDialThreshL
    {0x0200,                         // WDialThreshU
    0x0200},                         // WDialThreshL
    {0x0200,                         // AltWDialThreshU
    0x0200},                         // AltWDialThreshL
    {0x0200,                         // ProgThreshU
    0x0200},                         // ProgThreshL
    {0x0200,                         // AltProgThreshU
    0x0200},                         // AltProgThreshL
    0,                              // DTMFRxThresh
    0},                              // RLSDXRTHOffset

    // Tone Detection Parameters
    {750,                            // DialStable
    750,                            // WDialStable
    300,                            // AnswerStable
    70,                             // ProgressStable
    70,                             // DialtoneWaitTime: ms
    0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

    // Redial Parameter
    {2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
    2000,                           // InterCallDelay when redialing after INCOMING: ms

    // Billing Delay
    2000,                           // BillingDelay: ms

    // Maximum allowable pause time for (,)
    0,                              // Default is 0 which implies that there is NO limit

    // Falsh delay before dialing a DTMF digit
    0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

    // TONE CADENCE
    {2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
    {{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
    {{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
    {{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

    // CallerId Control
    {4,                              // Cid_TYPE: 1=French
    0,                              // Cid_Prefix: initial digit in DTMF type CID
    0,                              // Cid_Suffix: final digit in DTMF type CID
    0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
    0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
    3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
    0,                              // PreRing_Cid_Indicator:
    0},                              // PolarityReversal

    // Option Control
    {0,                              // AltDialToneFilter
    0,                              // AltCallProgressFilter
    0,                              // AltWToneFilter
    0,                              // CNGFilterSelection
    0,                              // EnforceDialTone
    0,                              // EnforceBusyTone
    0,                              // LongToneIsBusy
    0,                              // EPTforV29
    0},                              // Spare0

    CTY_SIGNATURE                   // Signature
}; //End of Tunisia


//**************************************************
//* Country paramters for Turkey                   *
//**************************************************

static const CtryPrmsStruct Country_AE =
{
	// T.35 Country Identification
	T35_TURKEY,                        // T35Countrycode:

	// Country Name
	"Turkey",                          // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	33,                             // PulseMakeRatio
	67,                             // PulseBreakRatio
	0,                              // PulseMakeOffset:  No adjustment
	0,                              // PulseBreakOffset: No adjustment
	0,                              // PulseSetupTime: ms Disable Dialshunt if 0 and Enable if != 0
	0,                              // PulseClearTime: ms Disable Dialshunt if 0
	750,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{100,                             // DTMFOnTime:     ms (if 0 uses S11)
	100,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel:  -8.0 dBm
	60,                             // HighDTMFLevel: -6.0 dBm
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable ms
	0,                              // MinTimeBeforeAnswering: ms ASAP
	300,                            // MinDelayBtwnRings: ms
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x08C2,                         // ToneCoeffs
	0xEE7C,
	0x08C2,
	0xC774,
	0x74FE},
	{0x08C2,
	0x1184,
	0x08C2,
	0xCD4F,
	0x6495},
	0x7B30,                         // LPBFK
	0x04CF,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0000,                         // DialThreshU
	0x0000},                         // DialThreshL
	{0x0000,                         // AltDialThreshU
	0x0000},                         // AltDialThreshL
	{0x0000,                         // WDialThreshU
	0x0000},                         // WDialThreshL
	{0x0000,                         // AltWDialThreshU
	0x0000},                         // AltWDialThreshL
	{0x0380,                         // ProgThreshU
	0x0380},                         // ProgThreshL
	{0x0000,                         // AltProgThreshU
	0x0000},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable: ms
	750,                            // WDialStable: ms
	300,                            // AnswerStable: ms
	5,                              // ProgressStable: ms
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime: ms (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Flash delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                              // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},              // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},              // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{420,580},{420,580},{0,0},{0,0}},8},      // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},   // RingbackParams
	{{{220,280},{220,280},{0,0},{0,0}},8}},      // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=US type
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: ms inter-digit timeout DTMF type CID
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
};



//**************************************************
//* Country paramters for Ukraine                  *
//**************************************************

static const CtryPrmsStruct Country_B2 =
{
	// T.35 Country Identification
	T35_UKRAINE,                   	// T35Countrycode:

	// Country Name
	"Ukraine",                     	// International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Ukraine



//**************************************************
//* Country paramters for United Arab Emirates     *
//**************************************************

static const CtryPrmsStruct Country_B3 =
{
	// T.35 Country Identification
	T35_UNITED_ARAB_EMIRATES,       // T35Countrycode:

	// Country Name
	"United Arab Emirates",         // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{200,550},{200,550},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{130,500},{130,500},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of United Arab Emirates



//**************************************************
//* Country paramters for United Kingdom           *
//**************************************************

static const CtryPrmsStruct Country_B4 =
{
	// T.35 Country Identification
	T35_UNITED_KINGDOM,             // T35Countrycode:

	// Country Name
	"United Kingdom",               // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	14,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{500,                            // DialStable
	500,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{200,550},{200,550},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,500},{250,750},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{5,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of United Kingdom



//**************************************************
//* Country paramters for United States            *
//**************************************************

static const CtryPrmsStruct Country_B5 =
{
	// T.35 Country Identification
	T35_UNITED_STATES,              // T35Countrycode:

	// Country Name
	"United States",                // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset:  No adjustment
	0,                              // PulseBreakOffset: No adjustment
	0,                              // PulseSetupTime: ms Disable Dialshunt if 0 and Enable if != 0
	0,                              // PulseClearTime: ms Disable Dialshunt if 0
	750,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{92,                             // DTMFOnTime:     ms (if 0 uses S11)
	72,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel:  -8.0 dBm
	60,                             // HighDTMFLevel: -6.0 dBm
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable ms
	0,                              // MinTimeBeforeAnswering: ms ASAP
	300,                            // MinDelayBtwnRings: ms
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x08C2,                         // ToneCoeffs
	0xEE7C,
	0x08C2,
	0xC774,
	0x74FE},
	{0x08C2,
	0x1184,
	0x08C2,
	0xCD4F,
	0x6495},
	0x7B30,                         // LPBFK
	0x04CF,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0000,                         // DialThreshU
	0x0000},                         // DialThreshL
	{0x0000,                         // AltDialThreshU
	0x0000},                         // AltDialThreshL
	{0x0000,                         // WDialThreshU
	0x0000},                         // WDialThreshL
	{0x0000,                         // AltWDialThreshU
	0x0000},                         // AltWDialThreshL
	{0x0380,                         // ProgThreshU
	0x0380},                         // ProgThreshL
	{0x0000,                         // AltProgThreshU
	0x0000},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable: ms
	750,                            // WDialStable: ms
	300,                            // AnswerStable: ms
	5,                              // ProgressStable: ms
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime: ms (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Flash delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                              // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},              // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},              // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{420,580},{420,580},{0,0},{0,0}},8},      // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},   // RingbackParams
	{{{100,400},{100,400},{0,0},{0,0}},8}},      // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE: 1=US type
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: ms inter-digit timeout DTMF type CID
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of United States



//**************************************************
//* Country paramters for Uruguay                  *
//**************************************************

static const CtryPrmsStruct Country_B7 =
{
	// T.35 Country Identification
	T35_URUGUAY,                   	// T35Countrycode:

	// Country Name
	"Uruguay",                     	// International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{100,350},{150,450},{550,850},{650,950}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Uruguay



//**************************************************
//* Country paramters for Russian Federation       *
//**************************************************

static const CtryPrmsStruct Country_B8 =
{
	// T.35 Country Identification
	T35_RUSSIAN_FEDERATION,         // T35Countrycode:

	// Country Name
	"Russian Federation",           // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	60,                             // LowDTMFLevel
	30,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Russian Federation



//**************************************************
//* Country paramters for Venezuela                *
//**************************************************

static const CtryPrmsStruct Country_BB =
{
	// T.35 Country Identification
	T35_VENEZUELA,                  // T35Countrycode:

	// Country Name
	"Venezuela",                    // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=US type
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Venezuela



//**************************************************
//* Country paramters for Slovakia                 *
//**************************************************

static const CtryPrmsStruct Country_C5 =
{
	// T.35 Country Identification
	T35_SLOVAKIA,                   // T35Countrycode:

	// Country Name
	"Slovakia",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{600,750},{600,750},{0,0},{0,0}},2},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,600},{100,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},4},    // RingbackParams
	{{{100,600},{100,600},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Slovakia



//**************************************************
//* Country paramters for Slovenia                 *
//**************************************************

static const CtryPrmsStruct Country_C6 =
{
	// T.35 Country Identification
	T35_SLOVENIA,                   // T35Countrycode:

	// Country Name
	"Slovenia",                     // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{300,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{500,                            // DialStable
	500,                            // WDialStable
	300,                            // AnswerStable
	40,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},1},    // RingbackParams
	{{{150,300},{150,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Slovenia



//**************************************************
//* Country paramters for Lithuania                *
//**************************************************

static const CtryPrmsStruct Country_C7 =
{
	// T.35 Country Identification
	T35_LITHUANIA,                  // T35Countrycode:

	// Country Name
	"Lithuania",                    // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,325},{150,325},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Lituania



//**************************************************
//* Country paramters for Azerbaijan               *
//**************************************************

static const CtryPrmsStruct Country_C8 =
{
	// T.35 Country Identification
	T35_AZERBAIJAN,                 // T35Countrycode:

	// Country Name
	"Azerbaijan",                     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Azerbaijan



//**************************************************
//* Country paramters for Kazakhstan               *
//**************************************************

static const CtryPrmsStruct Country_D2 =
{
	// T.35 Country Identification
	T35_KAZAKHSTAN,                 // T35Countrycode:

	// Country Name
	"Kazakhstan",                   // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	400},                            // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{100,832},{100,900},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{80,300},{150,650},{0,0},{0,0}},8}},        // CongestionParams

	// CallerId Control
	{4,                              // Cid_TYPE: 1=French
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Kazakhstan



//**************************************************
//* Country paramters for Bosnia and Herzegovina   *
//**************************************************

static const CtryPrmsStruct Country_E5 =
{
	// T.35 Country Identification
	T35_BOSNIA_HERZEGOVINA,         // T35Countrycode:

	// Country Name
	"Bosnia and Herzegovina",       // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{150,325},{150,325},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Bosnia and Herzegovina



//**************************************************
//* Country paramters for Latvia                   *
//**************************************************

static const CtryPrmsStruct Country_F8 =
{
	// T.35 Country Identification
	T35_LATVIA,                     // T35Countrycode:

	// Country Name
	"Latvia",                       // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{150,250},{150,250},{500,750},{800,1200}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
    {{{300,600},{300,600},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
    {{{150,325},{150,325},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Latvia



//**************************************************
//* Country paramters for Estonia                  *
//**************************************************

static const CtryPrmsStruct Country_F9 =
{
	// T.35 Country Identification
	T35_ESTONIA,                    // T35Countrycode:

	// Country Name
	"Estonia",                      // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{350,650},{350,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{130,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Estonia



//**************************************************
//* Country paramters for Europe                   *
//**************************************************

static const CtryPrmsStruct Country_FD =
{
	// T.35 Country Identification
	0xFD,                           // T35Countrycode:

	// Country Name
	"Europe",                       // International string

	// Transmit Level
	{110,                            // Default: 110 for -11.0dBm
	1},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{1,                              // Line impedance
	3,                              // DC curve
	1,                              // Pulse make resistance
	0,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	25,                             // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	110,                            // LowDTMFLevel -11.0 dBV
	90,                             // HighDTMFLevel -9.0 dBV
	1,                              // DTMFCompensation: 1 for TBR21 (dBV)
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime
	0,                              // MinImmediateRingOn
	0},                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	70,                             // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{2,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{160,240},{160,240},{500,700},{900,1100}},2}, // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{350,650},{350,650},{0,0},{0,0}},8},       // BusyParams
	{{{650,1600},{2900,6000},{0,0},{0,0}},2},    // RingbackParams
	{{{130,300},{130,300},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{0,                              // Cid_TYPE: 0=none
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	0,                              // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Europe



//**************************************************
//* Country paramters for Taiwan Republic of China *
//**************************************************

static const CtryPrmsStruct Country_FE =
{
	// T.35 Country Identification
	T35_TAIWAN_REPUBLIC_OF_CHINA,   // T35Countrycode:

	// Country Name
	"Taiwan Republic of China",     // International string

	// Transmit Level
	{100,                            // Default: 100 for -10.0dBm
	0},                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	{2,                              // Line impedance
	1,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0},                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	{SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF},   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	{1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset: No adjustment
	0,                              // PulseBreakOffset: No adjustment
	90,                             // PulseSetupTime: Disable Dialshunt if 0 and Enable if != 0
	90,                             // PulseClearTime: Disable Dialshunt if 0
	900,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0},                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	{85,                             // DTMFOnTime:     ms (if 0 uses S11)
	85,                             // DTMFInterdigit: ms (if 0 uses S11)
	80,                             // LowDTMFLevel
	60,                             // HighDTMFLevel
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0},                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	{150,                            // RingStable: ms
	0,                              // MinTimeBeforeAnswering: ASAP
	300,                            // MinDelayBtwnRings:
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0},                              // MinImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	{{0,                              // Use ToneA
	{0x1923,                         // ToneCoeffs
	0xCD3C,
	0x1A2C,
	0xC467,
	0x774F},
	{0x1923,
	0xD7DB,
	0x1A2C,
	0xC774,
	0x69BC},
	0x7B30,                         // LPBFK
	0x06CF,                         // LPGAIN
	0x0180,                         // THRESHU
	0x0180,                         // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0},                              // SqDisState
	//Alternate ToneA
	{0,                              // Use ToneA
	{0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	{0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000},
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},   // Prefilter
	0}},                              // SqDisState

	// Thresholds Parameters
	{{0x0180,                         // DialThreshU
	0x0180},                         // DialThreshL
	{0x0200,                         // AltDialThreshU
	0x0200},                         // AltDialThreshL
	{0x0200,                         // WDialThreshU
	0x0200},                         // WDialThreshL
	{0x0200,                         // AltWDialThreshU
	0x0200},                         // AltWDialThreshL
	{0x0200,                         // ProgThreshU
	0x0200},                         // ProgThreshL
	{0x0200,                         // AltProgThreshU
	0x0200},                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0},                              // RLSDXRTHOffset

	// Tone Detection Parameters
	{750,                            // DialStable
	750,                            // WDialStable
	300,                            // AnswerStable
	5,                              // ProgressStable
	70,                             // DialtoneWaitTime: ms
	0},                              // PolarityReversalIgnoreTime (0 implies no ignore time)

	// Redial Parameter
	{2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Falsh delay before dialing a DTMF digit
	0},                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	{1,                               // ToneCadence_NumberOfPhases: Only simple cadence in US
	{{{0,0},{0,0},{0,0},{0,0}},0},  // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{0,0},{0,0},{0,0},{0,0}},0},               // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	{{{420,580},{420,580},{0,0},{0,0}},8},       // BusyParams
	{{{900,3000},{2900,4600},{0,0},{0,0}},2},    // RingbackParams
	{{{220,280},{220,280},{0,0},{0,0}},8}},       // CongestionParams

	// CallerId Control
	{1,                              // Cid_TYPE:
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: inter-digit timeout DTMF type CID, ms
	0,                              // PreRing_Cid_Indicator:
	0},                              // PolarityReversal

	// Option Control
	{0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0},                              // Spare0

	CTY_SIGNATURE                   // Signature
}; //End of Taiwan Republic of China



#if 0
//**************************************************

// Add more country parameters here.
//
//**************************************************
//* Country paramters for XXX                      *
//**************************************************

static const CtryPrmsStruct Country_xx =
{
	// T.35 Country Identification
	T35_XYZ,                        // T35Countrycode:

	// Country Name
	"XYZ",                          // International string

	// Transmit Level
	100,                            // Default: 100 for -10.0dBm
	0,                              // Compensation: 1 for TBR21

	// SDAA Control Parameters
	2,                              // Line impedance
	2,                              // DC curve
	1,                              // Pulse make resistance
	1,                              // Fast pulse edge
	0,                              // Initial DAC value      (if 0 uses default)
	68,                             // Ring Max frequency: Hz (if 0 uses default)
	15,                             // Ring Min frequency: Hz (if 0 uses default)
	0,                              // Ring threshold:     V  (if 0 uses default)
	0,                              // Low ring impedance     (if 0 uses default)
	0,                              // Ring detect at offhook (if 0 uses default)
	0,                              // Line in use threshold  (if 0 uses default)
	0,                              // Extension threshold    (if 0 uses default)
	0,                              // Digital line threshold (if 0 uses default)
	0,                              // DIB power              (if 0 uses default)

	// External Phone Relay Control Parameters
	SELECT_EXT_PHONE_RELAY_PORT,    // GPIO_OUTPUT_SELECTION
	MODEM_ONHOOK_PHONE_RELAY_ON,    // GPIO_DEFAULT_STATE
	MODEM_OFFHOOK_PHONE_RELAY_ON,   // OFFHOOK_PHONETOLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PHONEOFFLINE
	MODEM_ONHOOK_PHONE_RELAY_ON,    // ONHOOK_PHONETOLINE
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_PHONEOFFLINE
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSESETUP
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_PULSECLEAR
	MODEM_OFFHOOK_PHONE_RELAY_OFF,  // OFFHOOK_FLASH
	MODEM_ONHOOK_PHONE_RELAY_OFF,   // ONHOOK_FLASH

	300,                            // DelayPhoneOffLine: 300 ms
	300,                            // DelayPhoneToLine:  300 ms

	// Pulse Dial Parameters
	1,                              // AllowPulseDialing
	0,                              // PulseDialMode: Standard
	10,                             // PulseSpeed
	40,                             // PulseMakeRatio
	60,                             // PulseBreakRatio
	0,                              // PulseMakeOffset:  No adjustment
	0,                              // PulseBreakOffset: No adjustment
	0,                              // PulseSetupTime: ms Disable Dialshunt if 0 and Enable if != 0
	0,                              // PulseClearTime: ms Disable Dialshunt if 0
	750,                            // PulseInterd: ms
	0,                              // LineMuteDelay: ms
	0,                              // DisablePulseAfterTone:
	0,                              // DisableHearingPulseDialing:

	// DTMF Dial Parameters
	92,                             // DTMFOnTime:     ms (if 0 uses S11)
	72,                             // DTMFInterdigit: ms (if 0 uses S11)
	57,                             // LowDTMFLevel:  -5.7 dBm
	36,                             // HighDTMFLevel: -3.6 dBm
	0,                              // DTMFCompensation
	1,                              // AllowDTMFabcd
	0,                              // DisableHearingDTMFDialing:

	// Ring Detection Parameters
	150,                            // RingStable ms
	0,                              // MinTimeBeforeAnswering: ms ASAP
	300,                            // MinDelayBtwnRings: ms
	8000,                           // MaxDelayBtwnRings: ms
	0,                              // MinRingOnTime: ms
	0,                              // MinImmediateRingOn
	0,                              // MaxImmediateRingOn

	// Filter Parameters
	//Primary ToneA
	0,                              // Use ToneA
	0x08C2,                         // ToneCoeffs
	0xEE7C,
	0x08C2,
	0xC774,
	0x74FE,
	0x08C2,
	0x1184,
	0x08C2,
	0xCD4F,
	0x6495,
	0x7B30,                         // LPBFK
	0x04CF,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // Prefilter
	0,                              // SqDisState
	//Alternate ToneA
	0,                              // Use ToneA
	0x0000,                         // AltToneCoeffs
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,                         // LPBFK
	0x0000,                         // LPGAIN
	0,                              // THRESHU
	0,                              // THRESHL
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // Prefilter
	0,                              // SqDisState

	// Thresholds Parameters
	0x0000,                         // DialThreshU
	0x0000,                         // DialThreshL
	0x0000,                         // AltDialThreshU
	0x0000,                         // AltDialThreshL
	0x0000,                         // WDialThreshU
	0x0000,                         // WDialThreshL
	0x0000,                         // AltWDialThreshU
	0x0000,                         // AltWDialThreshL
	0x0380,                         // ProgThreshU
	0x0380,                         // ProgThreshL
	0x0000,                         // AltProgThreshU
	0x0000,                         // AltProgThreshL
	0,                              // DTMFRxThresh
	0,                              // RLSDXRTHOffset

	// Tone Detection Parameters
	750,                            // DialStable: ms
	750,                            // WDialStable: ms
	300,                            // AnswerStable: ms
	5,                              // ProgressStable: ms
	70,                             // DialtoneWaitTime: ms
	0,                              // PolarityReversalIgnoreTime: ms (0 implies no ignore time)

	// Redial Parameter
	2000,                           // InterCallDelay when redialing same number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing diff number after OUTGOING: ms
	2000,                           // InterCallDelay when redialing after INCOMING: ms

	// Billing Delay
	2000,                           // BillingDelay: ms

	// Maximum allowable pause time for (,)
	0,                              // Default is 0 which implies that there is NO limit

	// Flash delay before dialing a DTMF digit
	0,                              // Default is 0 which implies that the default value is used (DEFAULT_DELAY_AFTER_FLASH)

	// TONE CADENCE
	1,                              // ToneCadence_NumberOfPhases: Only simple cadence in US
	0,0,0,0,0,0,0,0,0,              // DialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	0,0,0,0,0,0,0,0,0,              // WDialtoneParams (Min ON, Max ON, Min OFF, Max OFF, Min ON, Max ON, Min OFF, Max OFF, # of cycles)
	420,580,420,580,0,0,0,0,8,      // BusyParams
	900,3000,2900,4600,0,0,0,0,2,   // RingbackParams
	220,280,220,280,0,0,0,0,8,      // CongestionParams

	// CallerId Control
	1,                              // Cid_TYPE: 1=US type
	0,                              // Cid_Prefix: initial digit in DTMF type CID
	0,                              // Cid_Suffix: final digit in DTMF type CID
	0,                              // DTMF_MinOnTime: minimum on in DTMF type CID, ms
	0,                              // DTMF_MinOffTime: minimum off in DTMF type CID, ms
	3000,                           // Cid_WaitTime: ms inter-digit timeout DTMF type CID
	0,                              // PreRing_Cid_Indicator:
	0,                              // PolarityReversal

	// Option Control
	0,                              // AltDialToneFilter
	0,                              // AltCallProgressFilter
	0,                              // AltWToneFilter
	0,                              // CNGFilterSelection
	0,                              // EnforceDialTone
	0,                              // EnforceBusyTone
	0,                              // LongToneIsBusy
	0,                              // EPTforV29
	0,                              // Spare0

	CTY_SIGNATURE                   // Signature
}
#endif


//***********************************************
const CtryPrmsStruct* Country_Table[] =
{
	&Country_00,        // Japan
//	&Country_01,        // Albania
	&Country_02,        // Algeria
//	&Country_03,        // American Samoa
	&Country_04,        // Germany
//	&Country_05,        // Anguilla
//	&Country_06,        // Antigua and Barbuda
	&Country_07,        // Argentina
//	&Country_08,        // Ascension
	&Country_09,        // Australia
	&Country_0A,        // Austria
	&Country_0B,        // Bahamas
	&Country_0C,        // Bahrain
//	&Country_0D,        // Bangladesh
//	&Country_0E,        // Barbados
	&Country_0F,        // Belgium

//	&Country_10,        // Belize
//	&Country_11,        // Benin
//	&Country_12,        // Bermuda
//	&Country_13,        // Bhutan
	&Country_14,        // Bolivia
//	&Country_15,        // Botswana
	&Country_16,        // Brazil
//	&Country_17,        // British Antarctic Territory
//	&Country_18,        // British Indian Ocean Territory
//	&Country_19,        // British Virgin Islands
//	&Country_1A,        // Brunei Darussalam
	&Country_1B,        // Bulgaria
//	&Country_1C,        // Myanmar
//	&Country_1D,        // Burundi
//	&Country_1E,        // Belarus
//	&Country_1F,        // Cameroon

	&Country_20,        // Canada
//	&Country_21,        // Cape Verde
//	&Country_22,        // Cayman Islands
//	&Country_23,        // Central African Republic
//	&Country_24,        // Chad
	&Country_25,        // Chile
	&Country_26,        // China
	&Country_27,        // Colombia
//	&Country_28,        // Comoros
//	&Country_29,        // Congo
//	&Country_2A,        // Cook Islands
	&Country_2B,        // Costa Rica
	&Country_2C,        // Cuba
	&Country_2D,        // Cyprus
	&Country_2E,        // Czech Republic
//	&Country_2F,        // Cambodia

//	&Country_30,        // Democratic Peoples Republic of Korea
	&Country_31,        // Denmark
//	&Country_32,        // Djibouti
	&Country_33,        // Dominican Republic
//	&Country_34,        // Dominica
	&Country_35,        // Ecuador
	&Country_36,        // Egypt
	&Country_37,        // El Salvador
//	&Country_38,        // Equatorial Guinea
//	&Country_39,        // Ethiopia
//	&Country_3A,        // Falkland Islands
//	&Country_3B,        // Fiji
	&Country_3C,        // Finland
	&Country_3D,        // France
//	&Country_3E,        // French Polynesia
//	&Country_3F,        //

//	&Country_40,        // Gabon
//	&Country_41,        // Gambia
//	&Country_42,        // Germany (42)
//	&Country_43,        // Angola
//	&Country_44,        // Ghana
//	&Country_45,        // Gibraltar
	&Country_46,        // Greece
//	&Country_47,        // Grenada
//	&Country_48,        // Guam
	&Country_49,        // Guatemala
//	&Country_4A,        // Guernsey
//	&Country_4B,        // Guinea
//	&Country_4C,        // Guinea Bissau
//	&Country_4D,        // Guayana
//	&Country_4E,        // Haiti
	&Country_4F,        // Honduras

	&Country_50,        // Hong Kong
	&Country_51,        // Hungary
	&Country_52,        // Iceland
	&Country_53,        // India
	&Country_54,        // Indonesia
	&Country_55,        // Iran
	&Country_56,        // Iraq
	&Country_57,        // Ireland
//	&Country_58,        // Israel
	&Country_59,        // Italy
//	&Country_5A,        // Cote Divoire
	&Country_5B,        // Jamaica
//	&Country_5C,        // Afghanistan
//	&Country_5D,        // Jersey
	&Country_5E,        // Jordan
//	&Country_5F,        // Kenya

//	&Country_60,        // Kiribati
	&Country_61,        // Korea
	&Country_62,        // Kuwait
//	&Country_63,        // Lao Peoples Democratic Republic
	&Country_64,        // Lebanon
//	&Country_65,        // Lesotho
//	&Country_66,        // Liberia
	&Country_67,        // Libyan Arab Jamahiriya
	&Country_68,        // Liechtenstein
	&Country_69,        // Luxembourg
//	&Country_6A,        // Macao
//	&Country_6B,        // Madagascar
	&Country_6C,        // Malaysia
//	&Country_6D,        // Malawi
//	&Country_6E,        // Maldives
//	&Country_6F,        // Mali

	&Country_70,        // Malta
//	&Country_71,        // Mauritania
//	&Country_72,        // Mauritius
	&Country_73,        // Mexico
//	&Country_74,        // Monaco
//	&Country_75,        // Mongolia
//	&Country_76,        // Montserrat
//	&Country_77,        // Morocco
//	&Country_78,        // Mozambique
//	&Country_79,        // Nauru
	&Country_7A,        // Nepal
	&Country_7B,        // Netherlands
//	&Country_7C,        // Netherlands Antilles
//	&Country_7D,        // New Caledonia
	&Country_7E,        // New Zealand
	&Country_7F,        // Nicaragua

//	&Country_80,        // Niger
//	&Country_81,        // Nigeria
	&Country_82,        // Norway
	&Country_83,        // Oman
//	&Country_84,        // Pakistan
	&Country_85,        // Panama
//	&Country_86,        // Papua New Guinea
	&Country_87,        // Paraguay
	&Country_88,        // Peru
//	&Country_89,        // Philippines
	&Country_8A,        // Poland
	&Country_8B,        // Portugal
//	&Country_8C,        // Puerto Rico
	&Country_8D,        // Qatar
	&Country_8E,        // Romania
//	&Country_8F,        // Rwanda

//	&Country_90,        // Saint Kitts and Nevis
//	&Country_91,        // Saint Croix
//	&Country_92,        // Saint Helena and Ascension
//	&Country_93,        // Saint Lucia
//	&Country_94,        // San Marino
//	&Country_95,        // Saint Thomas
//	&Country_96,        // Sao Tome and Principe
//	&Country_97,        // Saint Vincent and The Grenadines
	&Country_98,        // Saudi Arabia
//	&Country_99,        // Senegal
//	&Country_9A,        // Seychelles
//	&Country_9B,        // Sierra Leone
	&Country_9C,        // Singapore
//	&Country_9D,        // Solomon Islands
//	&Country_9E,        // Somalia
	&Country_9F,        // South Africa

	&Country_A0,        // Spain
//	&Country_A1,        // Sri Lanka
//	&Country_A2,        // Sudan
//	&Country_A3,        // Suriname
//	&Country_A4,        // Swaziland
	&Country_A5,        // Sweden
	&Country_A6,        // Switzerland
//	&Country_A7,        // Syrian Arab Republic
//	&Country_A8,        // Tanzania
	&Country_A9,        // Thailand
//	&Country_AA,        // Togo
//	&Country_AB,        // Tonga
//	&Country_AC,        // Trinidad and Tobago
	&Country_AD,        // Tunisia
	&Country_AE,        // Turkey
//	&Country_AF,        // Turks and Caicos Islands

//	&Country_B0,        // Tuvalu
//	&Country_B1,        // Uganda
	&Country_B2,        // Ukraine
	&Country_B3,        // United Arab Emirates
	&Country_B4,        // United Kingdom
	&Country_B5,        // United States
//	&Country_B6,        // Burkina Faso
	&Country_B7,        // Uruguay
	&Country_B8,        // Russian Federation
//	&Country_B9,        // Vanuatu
//	&Country_BA,        // Vatican
	&Country_BB,        // Venezuela
//	&Country_BC,        // Viet Nam
//	&Country_BD,        // Wallis and Futuna
//	&Country_BE,        // Samoa
//	&Country_BF,        // Yemen (Bf)

//	&Country_C0,        // Yemen (C0)
//	&Country_C1,        // Serbia
//	&Country_C2,        // Democratic Republic of The Congo
//	&Country_C3,        // Zambia
//	&Country_C4,        // Zimbabwe
	&Country_C5,        // Slovakia
	&Country_C6,        // Slovenia
	&Country_C7,        // Lithuania
	&Country_C8,        // Azerbaijan
//	&Country_C9,        //
//	&Country_CA,        //
//	&Country_CB,        //
//	&Country_CC,        //
//	&Country_CD,        //
//	&Country_CE,        //
//	&Country_CF,        //

//	&Country_D0,        //
//	&Country_D1,        //
	&Country_D2,        // Kazakhstan
//	&Country_D3,        //
//	&Country_D4,        //
//	&Country_D5,        //
//	&Country_D6,        //
//	&Country_D7,        //
//	&Country_D8,        //
//	&Country_D9,        //
//	&Country_DA,        //
//	&Country_DB,        //
//	&Country_DC,        //
//	&Country_DD,        //
//	&Country_DE,        //
//	&Country_DF,        //

//	&Country_E0,        //
//	&Country_E1,        //
//	&Country_E2,        //
//	&Country_E3,        //
//	&Country_E4,        //
	&Country_E5,        // Bosnia and Herzegovina
//	&Country_E6,        //
//	&Country_E7,        //
//	&Country_E8,        //
//	&Country_E9,        //
//	&Country_EA,        //
//	&Country_EB,        //
//	&Country_EC,        //
//	&Country_ED,        //
//	&Country_EE,        //
//	&Country_EF,        //

//	&Country_F0,        //
//	&Country_F1,        //
//	&Country_F2,        //
//	&Country_F3,        //
//	&Country_F4,        //
//	&Country_F5,        //
//	&Country_F6,        //
//	&Country_F7,        //
	&Country_F8,        // Latvia
	&Country_F9,        // Estonia
//	&Country_FA,        // US Virgin Islands
//	&Country_FB,        //
//	&Country_FC,        //
	&Country_FD,        // Europe (Actually copy of Luxembourg)
	&Country_FE,        // Taiwan Republic of China
//	&Country_FF,        //

	0              // End marker
};


