/*
 *  $RCSfile: PHONEX.H,v $
 * $Revision: 1.15 $
 *   $Author: miurat $
 *     $Date: 2011/11/11 23:12:00 $
 */
/************************************************************************
 *
 *      PHONEX.H
 *
 *  Definitions for Phone Module's external users.
 *
 *  Copyright 1998 - 2010 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef _PHONEX_H_
#define _PHONEX_H_

#include "../../CommonHeader/SDHFaxCommonHeader.h"

#if __OVERRUN_ERROR__
#define PHONE_TASK_PERIODIC_TIME        10  // mSec
#else
#define PHONE_TASK_PERIODIC_TIME        20  // mSec
#endif
#define PHONE_DIAL_BYTES                80
#define PHONE_DIAL_DTMF_CHAR            'T'
#define PHONE_DIAL_PLUSE_CHAR           'P'
#define PHONE_DIAL_PAUSE_CHAR           ','
#define PHONE_DIAL_PULSE_TO_DTMF_CHAR   0x0a  // 0x0a for '*', 0x0b for '#'

#define PHONE_DIAL_PAUSE_TIME           2000  // mSec

/* PHONE_MAX_DIAL_BUFFER_SIZE >
   PHONE_MAX_AUTODIAL_DIGITS  >= PHONE_MAX_REIDAL_DIGITS */
#define PHONE_MAX_DIAL_BUFFER_SIZE      68  /* Phone number buffer size */
#define PHONE_MAX_AUTODIAL_DIGITS       64  /* Autodial number buffer size */
#define PHONE_MAX_REIDAL_DIGITS         64  /* Last Number Redial buffer size */

typedef enum
{
	PHONE_STATE_IDLE,
	PHONE_STATE_INIT,
	PHONE_STATE_ONHOOK,
	PHONE_STATE_ONHOOK_FOR_RING,
	PHONE_STATE_OFFHOOK,
	PHONE_STATE_OFFHOOK_ANS,
	PHONE_STATE_OFFHOOK_ANS_MANUAL,
	PHONE_STATE_OFFHOOK_CALL,
	PHONE_STATE_OFFHOOK_CALL_MANUAL,
	PHONE_STATE_DIAL_BUSY_TONE_SETUP,
	PHONE_STATE_DIAL_BUSY_TONE_CHECK,
	PHONE_STATE_DIAL_TONE_DETECTED,
	PHONE_STATE_DIAL_DIGIT,
	PHONE_STATE_ANS_FSK_BUSY_TONE_SETUP,
	PHONE_STATE_ANS_FSK_BUSY_TONE_CHECK,
	PHONE_STATE_BUSY_TONE_DETECTED,
	PHONE_STATE_ANS_TONE_DETECTED,
	PHONE_STATE_FSK_TONE_DETECTED,
	PHONE_STATE_RING_CHECK,
	PHONE_STATE_CID_START,
	PHONE_STATE_LINEINUSE_CHECK,
	PHONE_STATE_NOLINE_CHECK,
	PHONE_STATE_ABORT
} PHONE_STATES;

typedef enum    /* Line Signal Tone Detection Types */
{
	TONE_DETECT_OFF,
	TONE_DETECT_DIAL,
	TONE_DETECT_BUSY,
	TONE_DETECT_ANS,
	TONE_DETECT_CI,
	TONE_DETECT_CNG,
	TONE_DETECT_FSK,
	TONE_DETECT_DIAL_BUSY,
	TONE_DETECT_ANS_FSK_BUSY,
	TONE_DETECT_RINGBACK,
	TONE_DETECT_ON,
	TONE_DETECT_ABORT
} TONE_TYPES;

typedef enum
{
	PHONE_SELECT_COUNTRY,
	PHONE_DIAL_TIMEOUT,
	PHONE_TRANSMIT_LEVEL,
	PHONE_DTMF_LOW_LEVEL,
	PHONE_DTMF_HIGH_LEVEL,
	PHONE_PULSE_MAKE_TIME,
	PHONE_PULSE_BREAK_TIME,
	PHONE_PULSE_DIAL_TYPE,
	PHONE_INTERDIGIT_DELAY,
	PHONE_DC_CHARACTERISTIC,
	PHONE_IMPEDANCE,
	PHONE_CALLER_ID_TYPE,
	PHONE_BUSY_TONE_CYCLE,
	PHONE_BUSY_TONE_MIN_ON_TIME,
	PHONE_BUSY_TONE_MAX_ON_TIME,
	PHONE_BUSY_TONE_MIN_OFF_TIME,
	PHONE_BUSY_TONE_MAX_OFF_TIME,
	PHONE_EXTENSION_SUPPORT,
	PHONE_PULSE_FALL_TIME,
	PHONE_EXTENSION_VOLTAGE_THRESHOLD,
	PHONE_BLIND_DIAL,
	PHONE_REF_ONHOOK_VOLTAGE,
    PHONE_REF_LINEINUSE_VOLTAGE,
    PHONE_LINE_IN_USE_THRESHOLD
} PHONE_PARAMS_TYPE;

typedef enum                /* Caler Id. Reason Types */
{
	CID_NO_DATA      =  0 ,  /* Data NOT available */
	CID_DATA_VALID   = 'D',  /* Data available */
	CID_DATA_ERROR   = 'E',  /* Data has checksum error (for debug) */
	CID_OUT_OF_AREA  = 'O',  /* Out-of-area/unavailable */
	CID_PRIVATE      = 'P'   /* Private/anonymous */
} CID_REASON_TYPES;

#define CID_ID_LENGTH   16
#define CID_DN_LENGTH   16
#define CID_NAME_LENGTH 16

typedef struct              /* Caler Id. Information structure */
{
	CID_REASON_TYPES Reason;        /* 0, 'D', 'P', or 'O' */
	UINT8 Type;                     /* 1: Type 1, 2: Type 2 */
	UINT8 Month, Day, Hour, Minute; /* Date and Time value */
	UINT8 LineId[CID_ID_LENGTH];    /* Null-terminated ASCII Calling Line Id */
	UINT8 Number[CID_DN_LENGTH];    /* Null-terminated ASCII Directory Number */
	UINT8 Name[CID_NAME_LENGTH];    /* Null-terminated ASCII Directory Name */
} CID_INFORMATION;


extern void job_phone_start( void );
extern void activate_phone_job( void );
extern void set_phone_number( UINT8 *dial_num, UINT8 mode );
extern void set_phone_state( UINT8 new_state );
extern void set_phone_status( UINT8 info, UINT8 value );
extern UINT8 get_tone_status( UINT8 info );

extern LINE_STATUS get_line_status( void );

extern BOOL CIDRxData( UINT8 data );

extern UINT8 CheckToneDetection(TONE_TYPES ToneType, UINT8 SamplingTimeInMS);
extern ERCODE SetUpToneDetection(TONE_TYPES ToneType, UINT16 DetTimeout);

extern void phone_send_test_signal( MODEM_TX_TEST_INDEX mode );
extern void phone_set_params( PHONE_PARAMS_TYPE type, UINT16 val );

/*********************************************************************/
#endif  /* _PHONEX_H_  */
