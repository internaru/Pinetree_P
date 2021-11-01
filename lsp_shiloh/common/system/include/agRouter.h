/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file agRouter.h 
 *
 * \brief
 *
 */


#ifndef AGROUTER_H
#define AGROUTER_H

#include "agMessage.h"
#include "error_types.h"
#include "posix_ostools.h" 


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Module ID's.  These ID's are used by each module to identify
 * themselves to the router.
 *
 * \warning Once an ID is assigned to a module it needs to remain unique (ie.
 * when adding new modules) add them to the end and don't use any \#ifdefs to
 * compile IDs in or out.  The uniqueness is reqired to support precompiled
 * libraries that send and receive messages.
 */
typedef enum
{
    ENGINEID               = 0,
    PRINTMGRID             = 1,
    PARSERID               = 2,
    JOBMGRID               = 3,
    INTERNALMGRID          = 4,
    // VIDEOMGRID             = 5,
    OBSOLETE_STATMGRID     = 6,
    TESTPRINTMGRID         = 7,
    NETPORTID              = 8,
    SJMID                  = 9,
    ZJPARSER               = 10,
    ACLPARSER              = 11,
    CMPARSER               = 12,
    PJLPARSER              = 13,
    I_AM_NOT_USED_ID       = 14,
    COPYAPPID              = 15,
    SCANPARSEID            = 16,
    USBHOSTID              = 17,
    SCANMANID              = 18,
    COMPMGRID              = 19,
    HBPARSER               = 20,
    FAXAPPID               = 21,
    FAXPRINTID             = 22,
    COMPRESSIONID          = 23,
    RCVFAXID               = 24,
    SCANTOFAXID            = 25,
    SENDFAXID              = 26,
    HANDSETID              = 27,
    FAXPARSERID            = 28,
    AUDIOID                = 29,
    RENDERERID             = 30,
    KEYPADMGRID            = 31,
    UIMGRID                = 32,
    CONFIGPAGEDEBUGSTRINGS = 33,
    CONFIGPAGE_BMP         = 34,
    CONFIGPAGE_RNDR        = 35,
    SCAN_CALGRAPP_ID       = 36,
    CMGRID                 = 37,
    PWRMGR_ID              = 38,
    PHOTOJOBMGRID          = 39,
    DEMO_SCANAPP_ID        = 40,
    FAXER_FAX_JOB_AGENT_ID   = 41, //FAX
    FAXER_PRINT_JOB_AGENT_ID = 42, //FAX
    FAXER_SCAN_JOB_AGENT_ID  = 43, //FAX    
    WIRELESSCONFIGID       = 44,
    WPSID                  = 45,
    SCANTOMEMCARDAPPID     = 46,
    JBIGID                 = 47,
    JBIG_CODEC_ID          = 48,
    DISPMGRID              = 49,
    CMDPARSER              = 50,
    PBMGRID                = 51,
    PBPARSERID             = 53,
    DDPARSERID             = 54,
    FAXREPORTSID           = 55,
    WSD_SCAN_APP_ID        = 56,
    ICEFILEID              = 57,
    COPY_TO_HOST_ID        = 58,
    UNIRAST_MODULE_ID      = 59,
    MLAN_EVENT_ID          = 60,
    USBHOSTPARID           = 61,
    PASSTHRUID             = 62,
    GENSKEL_ID             = 63,    ///< Skeleton example generator ID
    GENPIP_ID              = 64,    ///< Print Image Pipeline generator ID
    MASSSTORAGEID          = 65,
    HTTP_SCAN_ID           = 66,
    GGS_RIP_ID             = 67,
    GEN_PIPP_PIPR_ID       = 68,
    SCANTOEMAILAPPID       = 69,
    MPDF_MODULE_ID         = 70,
    CONSUMABLES_ID         = 71,
    GEN_COMPRESS_ID        = 72,
    GSOAP_SCANAPP_ID       = 73,
    GSOAP_PARSER_ID        = 74,
    GEN_DECOMPRESS_ID      = 75,
    GEN_MIP_ID             = 76,
    FW_UPDATE_ID           = 77,
    GEN_WIFI_PRINT_ID      = 78,
    JPEG_PARSER_ID         = 79,
    GENPIP_CP_ID           = 80,    ///< Print Image Pipeline generator ID for copy
    GEN_NUP_ID             = 81,
    SMJOB_SCAN_APP_ID      = 82,
    PHOTOCOPY_ID           = 83,
    SYSTEM_JOB_MGR_TEST_ID = 84,
    PRINT_APP_TEST_ID      = 85,
    FWUPGRADEID            = 86,
    VPI_MODULE_ID          = 87,
    VPI_SYSTEM_ID          = 88,
    HOST1284_4_HANDLER_ID  = 89,
    PRINTER1284_4_WRAPPER_ID  = 90,
    NFC_ID                 = 91,

    PFPARSER   			    = 99,		// PF Parser		2013.08.28	by Juny
    SYSTEM_SERVICE_RECEIVER_ID         = 100,
    SYSTEM_SERVICE_SENDER_ID         = 101,
    TINY_DEBUGGER_ID         = 102,
    SYSTEM_ENERGY_SAVE_TIMER         = 103,

	A3_MERGE_ID				= 104,
	PCFAXPARSER   			    = 105,		// PCFAX Parser		2014.02.20	by inhwan5
	ERASE_ROLLER_ID			= 106,
    SCARD_SENDER_ID         = 107,
    SCARD_RECEIVER_ID       = 108,
    FAXAPPSOLUTIONID = 109,
	IMAGE_LOG_ID = 110,
	IMAGE_LOG_MONITOR_ID = 111,
	SCAN_BOX_ID = 112, //Scan box
    // Always add new values above this line, and be sure to << ATTENTION! <<
    // update the switch statement in ToString_MODULE_ID().  << ATTENTION! <<
    NUMBERIDS,     ///< This one needs to always be last in the list!
    INVALID_MODULE_ID = 0x7FFFFFFF
} MODULE_ID;


void SYInitRouter(void);
/**
 * \brief Register a message queue.
 */
void router_register_queue( MODULE_ID id, mqd_t message_queue );
#ifndef __linux__
error_type_t SYRegister(MODULE_ID, TX_QUEUE *);
error_type_t SYUnRegister(MODULE_ID);
#endif
error_type_t SYMsgSendWait(MODULE_ID , MESSAGE *, uint32_t );
error_type_t SYMsgSendNoWait(MODULE_ID , MESSAGE * );
/**
 * \brief Send a message to a given module
 */
error_type_t SYMsgSend(MODULE_ID, MESSAGE *);
error_type_t SysUI_MsgSend(MODULE_ID, SYS_UI_MESSAGE *);

#ifdef HAVE_DBG_PRINTF
    const char* ToString_MODULE_ID(MODULE_ID module_id_val);
#else
    static inline const char* ToString_MODULE_ID(MODULE_ID module_id_val) {return 0;}
#endif

#ifdef __cplusplus
}
#endif

#endif // ifndef AGROUTER_H


