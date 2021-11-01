/*****************************************************************
*                 PVCS Version Control Information               *
*                                                                *
* $Header: /home/cvsref/rd/linux/package/digicolor/reference_code/fax/src/fax_services/homolgat.h,v 1.5 2009/10/05 18:36:53 miurat Exp $
*                                                                *
******************************************************************/

/*****************************************************************
***               Copyright (c) 1994 - 2008                    ***
***               Conexant Systems Inc.                        ***
***               Imaging and PC Media                         ***
***               All Rights Reserved                          ***
***                                                            ***
***            CONFIDENTIAL -- No Dissemination or             ***
***            use without prior written permission.           ***
***                                                            ***
******************************************************************
*                                                                *
*                                                                *
*                                                                *
*                                                                *
******************************************************************/

#ifndef __HOMOLGAT_H__
  #define __HOMOLGAT_H__

  #include "dc_type.h"
#if defined(RTOS_LINUX)  
  #include <../../../cty/src/oem_cty.h>
#elif defined(RTOS_THREADX)
  #include <..\cty\oem_cty.h>
#endif

  typedef struct
  {
    UINT8  T35Code;                    // T.35 Country Identification (1 byte)
    char   cInter[40];                 // Country Name (40 bytes)
    UINT16 Txlevel;                    // Transmit Level (2 bytes)
    CntrySDAAStructure SDAA;           // SDAA Control Parameters (14 bytes)
    UINT8  Relays[10];                 // Relay Control Parameters (10 bytes)
	UINT16 DelayPhoneOffLine;          // Delay time for relay control (2 bytes)
	UINT16 DelayPhoneToLine;           // Delay time for relay control (2 bytes)
    CntryPulseStructure Pulse;         // Pulse Dial Parameters (15 bytes)
    CntryDTMFStructure DTMFParam;      // DTMF Dial Parameters (11 bytes)
    CntryRingStructure Ring;           // Ring Detection Parameters (14 bytes)
    CntryFilterStructure Filter;       // Filter Parameters (52 bytes)
    CntryThresholdStructure Threshold; // Thresholds Parameters
    CntryToneStructure Tone;           // Tone Detection Parameters
    CntryTimingStructure Timing;       // Timing Parameter
    CntryCadenceStructure Cadence;     // TONE CADENCE
    CntryCallerIDStructure CallerID;   // CallerId Control
    CntryOptionStructure Option;       // Option Control
    char Signature[4];                 // "OEM2"

  } WORKING_TBL_t;  // Size was 135 bytes for DP3!!!




  void get_command_line_value   (UINT8 *ptr);
  void store_copy_str           (const char* ptr);
  void store_wr_char            (char ch);
  void store_wr_data            (UINT16 val, UINT8 digits);
  void enable_nvram_homol_table (void);

  void display_homol_tbl_value (void);
  void store_new_overlay       (void);
  void dp_set_homol_options    (void);
  void update_table_value      (void);
  void disable_nvram_table     (void);

  void   erase_nvram_table     (void);
  UINT16 country_code_mismatch (void);
  UINT16 homol_nvram_enabled   (void);
  UINT16 get_nvram_table       (void);
  void   display_homol_tbl     (void);
  void   parse_overlay_cmd (UINT8 *ptr);



  #ifdef __HOMOLGAT_C__   // {

  #define HOMOL_TABLE_SIZE 120
  #define POINTER_TABLE_SIZE (HOMOL_TABLE_SIZE - 1)
  #define HOMOL_PARM_CNT (HOMOL_TABLE_SIZE - 2)

  #define ILIMIT_OFFSET 83


// ----------------------------- //
// Functions local to homlogat.c //
// ----------------------------- //


// ----------------------------- //
// Variables local to homolgat.c //
// ----------------------------- //

  typedef struct
  {
    char *wptr;
    UINT16 size;

  } HOMOLOGATION_TABLE_OVERLAY_t;


  typedef struct
  {
    UINT16 cnt;

    UINT16 cmd;
    UINT16 idx;
    UINT16 val;

    UINT16 error;

  } HOMOL_CMD_LINE_PARAMS_t;

  typedef union
  {
    HOMOL_CMD_LINE_PARAMS_t ps;
    UINT16 pa[4];

  } HOMOL_CMD_LINE_PARAMS_u;


  static HOMOLOGATION_TABLE_OVERLAY_t ovl;
  static HOMOL_CMD_LINE_PARAMS_u cmd_line;
  static WORKING_TBL_t wtbl;

const char *homol_pstr[] =
{
  "CountryId ",			      // Country identification code

  "CountryIdStr "         // Country name & version string
};


UINT8 *ht_ptr [] =
{
  &wtbl.T35Code,
  &wtbl.Relays[0]

};


UINT8 *wt_ptr [] =
{
  &wtbl.T35Code,
  &wtbl.Relays[0]

};


#endif  // }




//#ifndef __HOML_API_H__
  //#define __HOML_API_H__

  //#include "type.h"

// ---------------------------------------------------- //
// Definition of application program interface commands //
// ---------------------------------------------------- //

typedef enum
{
  HOML_API_CMD_GET_NVRAM_STATUS,
  HOML_API_CMD_SET_NVRAM_STATUS,
  HOML_API_CMD_GET_PARM_VALUE,
  HOML_API_CMD_UPDATE_PARM_VALUE,
  HOML_API_CMD_ERASE_NVRAM_TBL

} HOMOL_API_CMD_e;

/*
  HOML_API_CMD_GET_NVRAM_STATUS:

    Get status of whether or not the NVRAM homologation table is being used.

    status = TRUE: NVRAM table is used instead of ROM (default) values.
    status = FALSE: NVRAM table is disabled and ROM (default) values are used.

    Usage:
    {
      UINT16 status;

      if (homl_api_command (HOML_API_CMD_GET_NVRAM_STATUS, (void*)&status) == API_ERROR_NONE)

      // The variable "status" contains the status of the NVRAM table
      ...
    }


  HOML_API_CMD_SET_NVRAM_STATUS

    When set to enabled (TRUE), the NVRAM table is used instead of the ROM (default)
    values.  If the NVRAM table has not previously been initialized by this
    command, then the NVRAM table is initialized with the ROM (default) values.
    Otherwise, the values previously stored in the NVRAM table values are used again.

    When set to disabled (FALSE), the ROM (default) values are used instead of the NVRAM
    values.  Any modification stored in the NVRAM table are retained and will be
    saved until erased.

    Usage:
    {
      UINT16 status = TRUE;  // Enable use of NVRAM table
      ...
      if (homl_api_command (HOML_API_CMD_SET_NVRAM_STATUS, (void*)&status) == API_ERROR_NONE)
      ...
    }


  HOML_API_CMD_GET_PARM_VALUE:

    Retrieve the current value of the specified parameter.  If NVRAM is enabled,
    the value stored in NVRAM is retrieved.  Otherwise, the default value stored
    in ROM is retrieved.

    Usage:
    {
      UINT16 val = HOML_IDX_DtmfLevel;

      if (homl_api_command (HOML_API_CMD_GET_PARM_VALUE, (void*)&val) == API_ERROR_NONE)

      // The variable "val" now contains the value of homologation parameter DtmfLevel.
      ...
    }


  HOML_API_CMD_UPDATE_PARM_VALUE:

    If NVRAM is enabled, this command is used to modify the specified parameter with
    the specified value.  If NVRAM is disabled, this command does nothing.

    Usage:
    {
      UINT16 parm[2];

      parm[0] = HOML_IDX_DtmfLevel;  // Index to parameter to be modified: DtmfLevel
      parm[1] = 8;                   // Set DtmfLevel to -8 dBm

      if (homl_api_command (HOML_API_CMD_UPDATE_PARM_VALUE, (void*)&parm) == API_ERROR_NONE)
      ...
    }


  HOML_API_CMD_ERASE_NVRAM_TBL:

    Erase the NVRAM table values and set everything to zero.  The NVRAM table must be
    disabled before it can be erased.  If it is not disable, this command does nothing.

    Usage:
    {
      if (homl_api_command (HOML_API_CMD_ERASE_NVRAM_TBL, (void*)0) == API_ERROR_NONE)
      ...
    }
*/

// ------------------------------------------------------ //
// Definition of indexes to homologation table parameters //
// ------------------------------------------------------ //

typedef enum
{
  HOML_IDX_CountryId,			        // 0) Country identification code (Read only)
  HOML_IDX_PulseTimeMake,		      // 1) pulse dial make time in msec
  HOML_IDX_PulseTimeBreak,		    // 2) pulse dial break time in msec
  HOML_IDX_PulseDigitPattern,     // 3) 0=normal, 2=Sweden, 3=Norway
  HOML_IDX_DtmfLevel, 			      // 4) DTMF high tone level in db
  HOML_IDX_MinDtmfDialSpeed,	    // 5) min s11 in msec
  HOML_IDX_MaxDtmfDialSpeed,	    // 6) max s11 in msec
  HOML_IDX_CallProgressThreshold, // 7) Call progress tones detection threshold in db
  HOML_IDX_DialToneDelay,		      // 8) Waiting time before start detecting dialtone
  HOML_IDX_DialToneHole,			    // 9) Max Dialtone hole time or off cadance in msec
  HOML_IDX_RingCountNullDelay,  	// 10) in multiples of 100 msec
  HOML_IDX_MinAutoAnsRingCount,	  // 11) Auto Answer ring count lower limit
  HOML_IDX_MaxAutoAnsRingCount,	  // 12) Auto Answer ring count upper limit
  HOML_IDX_MinRingPeriod,		      // 13) Ring detction lower limit for the ring period
  HOML_IDX_MaxRingPeriod,		      // 14) Ring detction upper limit for the ring period
  HOML_IDX_RingCadenceOn,		      // 15) Ring cadence on period in multiples of 10 ms
  HOML_IDX_RingCadenceOff,		    // 16) Ring cadence off period in multiples of 100 ms
  HOML_IDX_MinBusyCadenceOn,		  // 17) Min Busy cadence on period in multiples of 10 ms
  HOML_IDX_MaxBusyCadenceOn,		  // 18) Max Busy cadence on period in multiples of 10 ms
  HOML_IDX_BusyPassCount, 		    // 19) # of busy cycle to conclude busy state
  HOML_IDX_MinBusyCadenceOff,	    // 20) Min Busy cadence off period in multiples of 10 ms
  HOML_IDX_MaxBusyCadenceOff,	    // 21) Max Busy cadence off period in multiples of 10 ms
  HOML_IDX_ContinuousBusyDetect,  // 22)  0=disabled, 1=enabled
  HOML_IDX_AnsToneValidTime, 	    // 23) Answer tone validation time in multiples of 10 ms
  HOML_IDX_BlackListFlag, 		    // 24) Blacklisting/delayed, 0=disabled, 1=enabled
  HOML_IDX_MaxCallAttempts, 		  // 25) # of failed attempts for blacklisting a #
  HOML_IDX_TroubleCallDelay, 	    // 26) Delay for troubled call attempt in min
  HOML_IDX_IneffectiveCallDelay,	// 27) Delay for ineffective call attempt in min
  HOML_IDX_ErroneousCallDelay,	  // 28) Delay for erroneous call attempt in min
  HOML_IDX_BlackListPeriod, 		  // 29) Blacklisting duration in multiples of 10 min
  HOML_IDX_LoopSenseWaitTime,	    // 30) line current sense waiting time in multiples
                                  //     of 100 msec, 0 = loop sensing not required
  HOML_IDX_CallingToneFlag,   	  // 31) 0 = not required, 1 = required & disable permited
                                  //     2 = required & disable not perimited
  HOML_IDX_BlindDialPermited,     // 32) 0=not permited, 1=permited, 2=permited with spk on
  HOML_IDX_BusyDetectDisable,     // 33) Disabling with ATX, 0 = not permited, 1 = permited
  HOML_IDX_GuardToneType, 		    // 34) 0=not present & adjustable, 1=550 Hz, 2=1800 Hz
  HOML_IDX_BellModePermited, 	    // 35) 0=not permited, 1=permited
  HOML_IDX_OffHookRestrict, 		  // 36) 0=no restrictions, 1=permited with time limit,
                                  //     2=not permited
  HOML_IDX_OffhookShuntTime,		  // 37) Duration in multiples of 10 ms for which the
                                  //     shunt relay turned on when modem goes offhook.
  HOML_IDX_HookFlashTime,		      // 38) in multiples of 10 ms, 0 = hookflash disabled
  HOML_IDX_MinBlindDialPause,	    // 39) in sec
  HOML_IDX_MaxBlindDialPause,	    // 40) in sec
  HOML_IDX_MinNoAnsTimeOut,		    // 41) in sec
  HOML_IDX_MaxNoAnsTimeOut,		    // 42) in sec
  HOML_IDX_MinDialPauseTime,		  // 43) in sec
  HOML_IDX_MaxDialPauseTime,		  // 44) in sec
  HOML_IDX_MinNoCarrierDisc,		  // 45) in sec
  HOML_IDX_MaxNoCarrierDisc,		  // 46) in sec
  HOML_IDX_TxLevel,				        // 47) transmit level in dB
  HOML_IDX_RestrictDial,		      // 48) 0=If invalid dial modifiers are found dial the
                                  //     string ignoring the invalid modifiers.
                                  //     1=If invalid modifier found do dial any digits
                                  //     treat the entire string as NULL
  HOML_IDX_TadRxGain,			        // 49) tad RX gain
  HOML_IDX_RingDetectType,		    // 50) 1 = Full wave, 0 = Half wave
  HOML_IDX_DelayAttempts,		      // 51) # of attempts after which delay is effective
  HOML_IDX_MaxAttempts, 			    // 52) total # of all failed attempts before blocking
                                  //     any further call attempts,
                                  //     if = 255 means this parameter not applicable
  HOML_IDX_ErrorCallWeight,		    // 53) # attempts incrementing factor for erroneous call
  HOML_IDX_DialToneValidTime,	    // 54) Dial tone validation time in multiples of 100 ms
  HOML_IDX_DialBusyDetect,		    // 55) Busy detection during dial tone detection,
                                  //     0=disabled
  HOML_IDX_DtmfToneDiff,			    // 56) DTMF high & low tone level difference
  HOML_IDX_LocalPhoneDetect,		  // 57) local phone detection in Speakerpone, 0=on, 1=off
  HOML_IDX_PulseDialPermited,	    // 58) Flag indicating pulse dialing allowed or not ,
                                  //     0=allowed, 1=not allowed
  HOML_IDX_FilterIndex,			      // 59) 0=340-560, 1=310-485, 2=363-502, 3=276-504,
                                  //     4=415-460.
  HOML_IDX_DialToneThreshold,	    // 60) dial tone detection threshold in db
  HOML_IDX_DissableABCD,			    // 61) Flag to dissable A,B,C, & D digit dialing
                                  //     0=ABCD dialing allowed, 1=ABCD dialing dissabled
  HOML_IDX_MaxCommaPauseTime,	    // 62) in sec, restricts total comma pause duration
                                  //     if there are too many commas
  HOML_IDX_TadTxLevel,		        // 63) TAD transmit level in dB
  HOML_IDX_RxLevel,			          // 64) receive threshold in dB (there is 6 db offsetin DSP)
  HOML_IDX_WaitDialTimeOut,	      // 65) W dial modifier no dialtone timeout in sec
  HOML_IDX_ToneThreshold,	        // 66) Answer tone detection threshold
  HOML_IDX_DialToneLevel,	        // 67) Dialtone min energy level in multiples of 10
  HOML_IDX_FaxRxLevel,		        // 68) Fax receive threshold in dB.
  HOML_IDX_PulseTonePermited,     // 69) Flag indicating mixing of pulse and tone dialing
                                  //     in same dial string allowed or not, 0=allowed
  HOML_IDX_RingImpedanceRly,	    // 70) Ringer impedance relay flag, 0=Off, 1=On
  HOML_IDX_LoopViRly,		          // 71) DC loop V/I characteristics relay flag, 0=Off, 1=On
  HOML_IDX_LoopLimitRly,		      // 72) DC loop limiting relay flag , 0=Off, 1=On
  HOML_IDX_ComplexImpedance,      // 73) 0 = Mercury internal components
                                  //     1 = real impedance (external components)
                                  //     2 = complex impedance (external components)
  HOML_IDX_LineGainAMSB,		      // 74) Mercury Gain - CIOCA MSB
  HOML_IDX_LineGainALSB,		      // 75) Mercury Gain - CIOCA LSB
  HOML_IDX_LineGainBMSB,		      // 76) Mercury Gain - CIOCB MSB
  HOML_IDX_LineGainBLSB,		      // 77) Mercury Gain - CIOCB LSB
  HOML_IDX_TadLineGainBMSB,	      // 78) Mercury CIOCB MSB - Tx gain in Tad mode
  HOML_IDX_FdspLineGainBMSB,	    // 79) Mercury CIOCB MSB - Tx gain in Fdsp mode
  HOML_IDX_CTR21Enable,		        // 80) True = enable CTR21, FALSE = disable CTR21
  HOML_IDX_DCOffset1,			        // 81) Perseus homologation dc offset
  HOML_IDX_DCOffset2,			        // 82) Perseus homologation dc offset
  HOML_IDX_ILimitOffset,	        // 83 & 84) Perseus homologation current limit offset

  HOML_IDX_TxLevelHalfdB = 85,    // 85: Tx level 1/2db adjustments

  HOML_IDX_FaxV21EchoDelta,	      // 86: V.21 delta for echo countermeasures.
  HOML_IDX_FaxV21EchoIgnore,	    // 87: Ignore v21 in phase-C (10ms increment)
  HOML_IDX_FaxEcmFrameTimeout,    // 88: ECM phase-C frame timeout in seconds.
  HOML_IDX_FaxTcfCheckTime,	      // 89: Reqired zero duration for TCF in 10ms ticks.
  HOML_IDX_FaxEolTimeout,		      // 90: EOL Timer in seconds.
  HOML_IDX_FaxTxPreemphasis,	    // 91: Modem Tx Preemphasis used for Fax.
  HOML_IDX_FaxForceEPT,		        // 92: Force EPT for v29 (normally, v29 does not use EPT)
  HOML_IDX_FaxT1Timeout,		      // 93: Fax T1 Timeout in seconds.
  HOML_IDX_Fax64OctetECM,		      // 94: Force/Request 64 octet frames for ECM Tx/Rx.
  HOML_IDX_FaxV21Ignore1stDIS,    // 95: Ignore first DIS frame.
  HOML_IDX_IgnoreRing,		        // 96: Ignore Ring interrupt: used during config of S/Hregs
  HOML_IDX_rpasPeriod,		        // 97: First Ring period in multiples of 10 ms (if non-zero)
  HOML_IDX_TadDialToneThreshold,  // 98: Dial tone/busy threshold for TAD (IIR1/IIR2)
  HOML_IDX_OcfFeaturesOn,         // 99: Select API features for OCF
  HOML_IDX_localHndsetFeature,    // 100: Set if has OCF local handset
  HOML_IDX_DTScompliant,          // 101: DTS Compliant FAX behavior,
  HOML_IDX_DropoutTimer,          // 102: Dropout timer for reporting to App
  HOML_IDX_CallerIDToneThreshold1,   // 103: Caller ID tone detection threshold in db
  HOML_IDX_CallerIDToneThreshold2,   // 104: Caller ID tone detection threshold in db
  HOML_IDX_V34FaxPreambleTime,		   // 105: V.34 fax primary channel preamble (10ms units)
  HOML_IDX_AllowUncompressedFaxMode, // 106: Control of uncompressed fax encoding option (default is off)
  HOML_IDX_Class2FaxLegacyBitOrder,	 // 107: Enable for legacy bit ordering on Class 2.x fax (MRI 55587)
  HOML_IDX_PolarisReg3,				    // 108: Polaris reg 3
  HOML_IDX_PolarisReg4,				    // 109: Polaris reg 4
  HOML_IDX_PolarisRegTxRxLevel,	  // 110: Polaris reg 5
  HOML_IDX_PolarisReg6,				    // 111: Polaris reg 6
  HOML_IDX_PolarisReg7,				    // 112: Polaris reg 7
//  HOML_IDX_V90TxLevel             // 113: V90 Tx Level (0x1FF3)
  HOML_IDX_V90TxLevel,             // 113: V90 Tx Level (0x1FF3)

// +sjb-9/13/07: Begin addition...
  HOML_IDX_FaxV34Phase3Timeout,		// 114: Timeout for V.34 Phase 3 to low-speed fallback (MRI 59006)
  HOML_IDX_FaxClass1AutoDropout,	// 115: Auto dropout time for Class 1 in 100ms (0=disable)
  HOML_IDX_Class2RemoteLimits,		// 116: Limit Class 2.0 remote report options (+FIS) for Windows Apps (MRI 60859)
  HOML_IDX_FaxFlowControl,			  // 117: Fax flow control on only during data mode
  HOML_IDX_V34FaxCCfallback,      // 118: V.34 fax CC fallback to low-speed (MRI 61327)

  HOML_IDX_LAST
// +sjb-9/13/07: End addition.

} HOML_IDX_e;

// ------------------------------------------- //
// Definition of API function call error codes //
// ------------------------------------------- //

typedef enum
{
  API_ERROR_NONE,                    // 0: No error occured executing API command
  API_ERROR_SET_SAME_STATUS,         // 1: Setting NVRAM status to the same state it's in
  API_ERROR_INVALID_PARM_INDEX,      // 2: The parameter index is not defined in HOMOL_IDX_e
  API_ERROR_NVRAM_NOT_ENABLED,       // 3: Command not executed because NVRAM was not enabled
  API_ERROR_INVALID_NVRAM_STATUS,    // 4: The NVRAM is not initialized properly (internal error)
  API_ERROR_COUNTRY_CODE_MISMATCH,   // 5: The FXCB country code does not match the NVRAM country code
  API_ERROR_COUNTRY_CODE_READ_ONLY,  // 6: Can't change the country code in NVRAM
  API_ERROR_ERASE_NOT_ALLOWED,       // 7: Can't erase NVRAM when it's enabled
  API_ERROR_INVALID_COMAMND          // 8: The specified command in not defined in HOMOL_API_CMD_e

} API_ERROR_CODES_e;

// -------------------------------------------------------- //
// Definition of function for application program interface //
// -------------------------------------------------------- //
/*
    Returns one of the codes defined in API_ERROR_CODES_e indicating
    the results of executing a command.

    The debug log will also contains a message of the reason for a failure.
*/

API_ERROR_CODES_e homl_api_command (HOMOL_API_CMD_e cmd, void* ptr);

#endif
