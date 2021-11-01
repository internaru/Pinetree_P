/************************************************************************
 *
 *  fsu_api.h
 *
 *  header file for fsu_api.c
 *
 *  Copyright 2004 - 2010 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef __FSU_API_H__
#define __FSU_API_H__

#include "dc_fwcxdxx_lnx_api.h"
#include "ATTRIB.H"
#include "MODEMX.H"
#include "PHONEX.H"
#include "T30X.H"

/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/


/******************************************************************************
*  2. DEFINES
******************************************************************************/
#define SIGNED_LEVEL

#define CNG      0x11
#define CED      0x10
#define TCF      0x16
#define IMG      0x26
#define ANSam    0x36
#define V8CM     0x46
#define V8JM     0x56
#define V8CJ     0x66
#define V8CI     0x76
#define V34P2    0x96
#define V34P3    0xA6
#define V34C1    0xB6
#define V34CC    0xC6
#define V34P1    0xD6
#define V34PC    0xE6
#define RTC      0xF6
#define CFR      0x84
#define CIG      0x41
#define CRP      0x1A
#define CSI      0x40
#define CTC      0x12
#define CTR      0xC4
#define DCN      0xFA
#define DCS      0x82
#define DIS      0x80
#define DTC      0x81
#define EOM      0x8E
#define EOP      0x2E
#define EOR      0xCE
#define ERR      0x1C
#define FCD      0x06
#define FTT      0x44
#define MCF      0x8C
#define MPS      0x4E
#define NSC      0x21
#define NSF      0x20
#define NSS      0x22
#define PIN      0x2C
#define PIP      0xAC
#define PPR      0xBC
#define PPS      0xBE
#define PRI_EOM  0x9E
#define PRI_EOP  0x3E
#define PRI_MPS  0x5E
#define RCP      0x86
#define RNR      0xEC
#define RR       0x6E
#define RTN      0x4C
#define RTP      0xCC
#define TSI      0x42
#define SEP_ERR  0xB1
#define SID_ERR  0xB2
#define PWD_ERR  0xD1
#define SUB_ERR  0xD2
#define PSA_ERR  0x71
#define NUL      0x00
#define T30_END  0xFF


typedef enum
{
	PHONE_ONHOOK,      // 0
	PHONE_OFFHOOK,
	PHONE_LINEINUSE,
	PHONE_LINENOTUSE,
	PHONE_NOLINE,
	PHONE_PRESENTLINE, // 5
	PHONE_NODIAL,
	PHONE_DIAL,
	PHONE_BUSY,
	PHONE_NOANSWER,
	PHONE_RINGON,      // 10
	PHONE_RINGOFF,
	PHONE_CALLERID,
	PHONE_ABORT,
	PHONE_DETECTEDFAX,
	RX_PHASE_A,        // 15
	RX_PHASE_B,
	RX_PHASE_C,
	RX_PHASE_D,
	RX_PHASE_E,
	RX_DISCONN,        // 20
	RX_ABORT,
	TX_PHASE_A,
	TX_PHASE_B,
	TX_PHASE_C,
	TX_PHASE_D,        // 25
	TX_PHASE_E,
	TX_DISCONN,
	TX_VOICE_REQ,
	TX_ABORT,
	HOST_ABORT,			// TBD		//30
	FAX_END,
	
	PHONE_CNG = 254
} FAX_EVENT_STATE; 


typedef enum
{
	FAX_RX_DATA = 0,
	FAX_TX_DATA
} FAX_DIRECTION;


#ifndef DATA_FROM_SCAN
typedef enum
{
	DATA_FROM_SCAN,
	DATA_FROM_FILE
} FAX_DATA_SOURCE;
#endif


#ifndef WIDTH_A4
typedef enum
{
	WIDTH_A4,
	WIDTH_B4,
	WIDTH_A3
} FAX_PAGE_WIDTH;
#endif

typedef enum
{
	FSU_OK,
    FSU_MEMORY_FULL_NAND,
    FSU_MEMORY_FULL_RAM,
    FSU_JOBQ_FULL,
    FSU_SPAM_FAX,

    FSU_RETURN_CODE_LAST
} FSU_RETURN_CODE;


/******************************************************************************
*  3. STRUCTURES
******************************************************************************/
typedef struct FaxEvent_t
{
	FAX_EVENT_STATE type;
	FAX_DIRECTION   dir;
	UINT16          fcf;
	UINT16          size;
    UINT16          *data;
#ifdef SIGNED_LEVEL
	INT8            lvl;
#else
	UINT8           lvl;
#endif
	UINT16          eqm;
} FAX_EVENT;


/******************************************************************************
*  4. ALLOCS
******************************************************************************/


/******************************************************************************
*  5. FUNCTION Declares
******************************************************************************/
extern void Fax_API_CancelFax(BOOLEAN flag_abort_key, UINT16 jnum);
extern void Fax_API_T30Attribute(UINT8 set_get, T30_ATTRIB attrib_id, UINT8 *msg);
extern void Fax_API_Speaker_Volume(UINT8 volume);
extern void fax_events(FAX_EVENT *pEvent);
extern void set_fsu_state( UINT8 state );
extern void fsu_tx_fax_cancel(void);
extern void StopHomologationSignal(void);
extern void SetPhoneParams(UINT8 type, UINT16 val);
extern int Get_LineStatus(void);
extern void StartHomologationSignal(MODEM_TX_TEST_INDEX HomologationSignal);
#endif		// __FSU_API_H__

