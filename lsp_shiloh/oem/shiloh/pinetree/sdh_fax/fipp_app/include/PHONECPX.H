/*
 *  $RCSfile: PHONECPX.H,v $
 * $Revision: 1.3 $
 *   $Author: miurat $
 *     $Date: 2009/09/25 18:45:38 $
 */
/************************************************************************
 *
 *      PHONECPX.H
 *
 *  Definitions for Phone Control Program Module's external interfaces.
 *
 *  Copyright 1998 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef _PhoneCPX_H_
#define _PhoneCPX_H_

/** PhoneCP Attributes **/

typedef enum                /* PhoneCP Attribute Types */
{
    PCA_STATUS = PCA,
    PCA_DISTINCTIVE_RING,
    PCA_RING_DELAY
} PHONECP_ATTRIB;


/** PhoneCP Messages **/

typedef enum                /* PhoneCP Message Types */
{
    /* For External API Use */
    PCM_OP_REQUEST,         /* OP*/
    PCM_OP_TERMINATE,       /* PROC_... */
    PCM_RINGING,            /* Ring Count */
    PCM_SENSOR_INPUT,       /* PHONECP_SENSOR_INPUT_TYPES */
    PCM_TAM_STATUS,         /* PHONECP_TAMSTATUS_TYPES */
    PCM_VKEY_SPEAKERPHONE,  /* NULL */
    PCM_VKEY_FLASH,         /* NULL */
    PCM_VKEY_HOLD,          /* NULL */
    PCM_VKEY_MUTE,          /* NULL */
    PCM_FAX_TO_VOICE,       /* NULL */
    PCM_VOICE_TO_FAX,       /* FAX_MANUAL or FAX_ANSWER ? */

    /* For Internal Use Only */
    PCM_PROCINIT,           /* UINT8,    PhoneCP procedure initialize */
    PCM_PROCEXIT,           /* NULL,     PhoneCP procedure exit */
    PCM_TIMEOUT             /* Timer Id, PhoneCP timer time out */
} PHONECP_MESSAGE_TYPES;

/*-------------------------------------------------------------------*/

/* PhoneCP Operations */
typedef enum                /* PhoneCP Operational Types */
{                           /* for message PCM_OP_REQUEST, PCM_OP_TERMINATE */
    PCOP_HOSTFAX_ACTIVE,    /* Request to RUN in Host Fax (Class1) Mode */
    PCOP_HOSTFAX_TERMINATE, /* Host Fax (Class1) Mode Terminate */
    PCOP_T30_ACTIVE,        /* Request to RUN in T30/FAX Mode */
    PCOP_T30_TERMINATE,     /* T30/FAX Mode Terminate */
    PCOP_TAM_ACTIVE,        /* Request to RUN in TAM Mode */
    PCOP_TAM_TERMINATE,     /* TAM Mode Terminate */
    PCOP_PHONE_ACTIVE,      /* Request to RUN in Phone Mode */
    PCOP_PHONE_TERMINATE    /* Phone Mode Terminate */
} PHONECP_OP_TYPES;

/* PhoneCP sensor input types */
typedef enum                    /* PhoneCP Sensors Status Change Types */
{                               /* for message PCM_SENSOR_INPUT */
    PCSC_HANDSET_UP,            /* Bulit-in Handset OFF_HOOK */
    PCSC_HANDSET_DOWN,          /* Bulit-in Handset  ON_HOOK */
    PCSC_EXT_DEV_ACTIVE,        /* Plug-in EXT. Device OFF_HOOK */
    PCSC_EXT_DEV_INACTIVE,      /* Plug-in EXT. Device  ON_HOOK */
    PCSC_RING_TIMEOUT,          /* Could be Parallel EXT. Device OFF_HOOK */
    PCSC_SILENCE_DETECTED,      /* Specified silence time detected */
    PCSC_CNG_DETECTED,          /* Specified CNG type detected */
    PCSC_HDLC_DETECTED,         /* HDLC detected */
    PCSC_DIAL_TONE_DETECTED,    /* Specified Dial Tone Time detected */
    PCSC_BUSY_TONE_DETECTED,    /* Specified Busy Tone Time detected */
    PCSC_DETECTION_TIMEOUT,     /* Noisy line with no CNG/HDLC/
                                    DIAL TONE/ BUSY TONE/SILENCE detected  */
    PCSC_CNG_DETECTSTART,       /* Portion of CNG detected */
    PCSC_CNG_DETECTGIVEUP       /* Gives up CNG detection */
} PHONECP_SENSOR_INPUT_TYPES;

/* PhoneCP statuses */
typedef enum
{
    PCPSTATUS_IDLE,             /* idle status */
    PCPSTATUS_FAXANSWER,        /* FAX answer status */
    PCPSTATUS_TADANSWER,        /* TAD answer status */
    PCPSTATUS_TELEPHONE,        /* telephone status */
    PCPSTATUS_EXTTELEPHONE      /* external telephone status */
} PHONECP_STATUSES;

/* PhoneCP TAM status types */
typedef enum                /* PhoneCP TAM Status Change Types */
{                           /* for message PCM_TAM_STATUS */
    STOP_TAM,               /* TAM stop */
    PLAY_OGM,               /* play OGM to line */
    PLAY_OGM_LOCAL,         /* play OGM locally */
    PLAY_ICM,               /* play ICM to line */
    PLAY_ICM_LOCAL,         /* play ICM locally */
    RECORD_ICM,             /* record ICM from line */
    RECORD_OGM,             /* record OGM from handset */
    RECORD_OGM_REMOTE       /* record OGM from line */
} PHONECP_TAMSTATUS_TYPES;


/*** Public Interface Function Prototypes ***/

extern ERCODE PhoneCPMessage(UINT8 MessageId, const UINT8 Param);
extern ERCODE PhoneCPAttribute(UINT8 Operation, ATTRIB Attrib_Id, void *pParam);
extern BOOL LocalPhoneMode(void);
extern BOOL CheckFaxRcvAvailable(void);
extern BOOL MsgCtrFaxForward (BOOL auto_receive, BOOL polling);

/*********************************************************************/
#endif  /* _PhoneCPX_H_  */
