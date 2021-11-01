/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
 *
*               Marvell Confidential
* ==========================================================
*/
/**
 * \file agRouter.c
 *
 * \brief This module is the message router for the system.
 *
 *  It operates by having routines
 *  that wish to receive a message, create a message mailbox and then registering the 
 *  mailbox with this module.  When a message is sent to the module id the message is 
 *  routed by this routine to the correct mailbox.
 *
 *****************************************************************************/
 
#include <stdint.h>
#include "posix_ostools.h"
#include "agRouter.h"
#include "logger.h"
#include "ATypes.h"
#include "debug.h"
#include "lassert.h"
#include "error_types.h"
#include "pthread.h"
#include <sys/prctl.h>	//for Thread Name

mqd_t mq_MailBoxes[NUMBERIDS];   // the storage of the mailboxes.
#ifndef __linux__
TX_QUEUE *MailBoxes[NUMBERIDS];   // the storage of the mailboxes.
#endif
error_type_t MsgSend(MODULE_ID , MESSAGE*, uint32_t);    // Send a message to a given module    

/*
    Initialize the system by clearing the mailbox locations.
    */
void SYInitRouter()
{
    uint32_t i;
    
    for (i = 0; i < NUMBERIDS; i++)
    {
#ifndef __linux__
        MailBoxes[i] = NULL;
#endif
        mq_MailBoxes[i] = 0;
    }
}

void router_register_queue( MODULE_ID id, mqd_t message_queue )
{
    XASSERT(id < NUMBERIDS, id);
    XASSERT(mq_MailBoxes[id] == 0, id);  // mailbox should be empty; no double registers
    if(id < NUMBERIDS)
    {
        mq_MailBoxes[id] = message_queue;
#ifndef __linux__
        MailBoxes[id] = 0;
#endif
    }
}
#ifndef __linux__
/*
    This routine is called to register the mailbox with the system.
    */
error_type_t SYRegister(MODULE_ID ModID, TX_QUEUE *MailBox)	// Register a message queue.
{
   // Check to see if we are in range
    if(ModID >= NUMBERIDS)
    {
       // ca: ASSERT(0);
       return FAIL;
    }
    //
    // Now check to make sure this was not registered before.
    //
    if(MailBoxes[ModID] != NULL)
    { 
       // ca: ASSERT(0);
       return FAIL;
    }
    MailBoxes[ModID] = MailBox;
    ASSERT(mq_MailBoxes[ModID] == NULL); 
    return OK;
}


/*
    Unregister a mailbox.
    */
error_type_t SYUnRegister(MODULE_ID ModID)      	// unregister to receive messages
{
    if(ModID >= NUMBERIDS)
    {
       // ca: ASSERT(0);
       return FAIL;
    }
    MailBoxes[ModID] = NULL;
    return OK;
}
#endif

error_type_t SYMsgSendNoWait(MODULE_ID ModID, MESSAGE *Message)
{
    error_type_t e_res;

    e_res = MsgSend(ModID, Message, 
#ifdef __linux__
                    0);
#else
                    TX_NO_WAIT);
#endif

    if( e_res != OK )
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("tx_queue_send() msg %d failed to %d, MSG LOST !!\n", Message->msgType, ModID));
    }

    return e_res;
}

error_type_t SYMsgSendWait(MODULE_ID ModID, MESSAGE *Message, uint32_t WaitValue)
{
    return(MsgSend(ModID, Message, WaitValue));
}

error_type_t SYMsgSend(MODULE_ID ModID, MESSAGE *Message)
{
    // ThreadX generates the TX_WAIT_ERROR (4) error if you don't
    // specify the TX_NO_WAIT option when calling tx_queue_send()
    // from an interrupt service routine (or from anywhere that
    // is not a thread).  Therefore this SYMsgSend() function
    // MUST NOT be called from a non-thread.
    return(MsgSend(ModID, Message, 
#ifdef __linux__
                   POSIX_WAIT_FOREVER));
#else
                   TX_WAIT_FOREVER));
#endif
}

/*
 * Route a message to a given module.
    */
error_type_t MsgSend(MODULE_ID ModID, MESSAGE* Message, uint32_t WaitValue) // Send a message to a given module    
{

#ifdef DEBUG
  /****** Cut and paste this line to comment in/out the print methods.
    DPRINTF((DBG_SOFT | DBG_OUTPUT),
        ("MsgSend: FROM='%s' TO='%s' msgType='%s'=0x%X=%d;\nMsgSend: @ %ld ticks; param1=0x%X  param2=0x%X  param3=0x%08X\n",
        (tx_thread_identify() ? (tx_thread_identify()->tx_thread_name ? tx_thread_identify()->tx_thread_name : "<thread name is NULL pointer>") : "<ISR>"), // FROM
        ToString_MODULE_ID(ModID),              // TO
        DecodeSysMsgEnum(Message->msgType),     // msgType as string
        Message->msgType, Message->msgType,     // msgType as hexadecimal, decimal
        tx_time_get(),                          // current timer ticks
        Message->param1, Message->param2, Message->param3));
  ******/
  /****** Cut and paste this line to comment in/out the print methods.
    // This option tends to be overkill...
    DPRINTF((DBG_ERROR | DBG_OUTPUT),
        ("MsgSend: FROM='%s' TO='%s' Print_MESSAGE follows..\n",
        (tx_thread_identify() ? (tx_thread_identify()->tx_thread_name ? tx_thread_identify()->tx_thread_name : "<thread name is NULL pointer>") : "<ISR>"), // FROM
        ToString_MODULE_ID(ModID)));            // TO
    Print_MESSAGE(Message);
  ******/
#endif

    if(ModID >= NUMBERIDS)
        return FAIL;
#ifndef __linux__
    if(MailBoxes[ModID] != NULL)
	{
#ifdef DEBUG
  /****** Cut and paste this line to comment in/out the queue full debug code.
    QUEUE* pQ = MailBoxes[ModID];
    if ( pQ->tx_queue_capacity == pQ->tx_queue_enqueued )
    {
        DPRINTF((DBG_ERROR|DBG_OUTPUT),
                ("MsgSend: Queue at 0x%08X name='%s' full! capacity=%d enqueued=%d *** Is this OK for this Q? ***\n",
                 pQ, ( pQ->tx_queue_name ? pQ->tx_queue_name : "<NULL ptr>" ),
                 pQ->tx_queue_capacity, pQ->tx_queue_enqueued ));
    }
  ******/
#endif // ifdef DEBUG
		if(tx_queue_send(MailBoxes[ModID], Message, WaitValue) == TX_SUCCESS)  // send the message no wait  
		{
			return OK;
		}
	}
	else 
#endif
    if (0 != mq_MailBoxes[ModID] )
    {
#if 0	 //To Debug Send Messages : LALIT
	int mq_retcode;
	struct mq_attr attr;	
	mq_retcode = (int)mq_getattr( mq_MailBoxes[ModID], &attr );		
	
	if ((attr.mq_maxmsg-2)<(attr.mq_curmsgs) && (ModID != GEN_COMPRESS_ID))
	{
		char senderName[30]={0,};
		prctl(PR_GET_NAME, senderName, 0, 0);
		DPRINTF((DBG_ERROR | DBG_OUTPUT),(" MsgSend: FROM = %s  TO = %s MessageType = %s, Message Queue MaxMsg=%ld, CurMsgs = %ld\n",  
			senderName, ToString_MODULE_ID(ModID), DecodeSysMsgEnum(Message->msgType), attr.mq_maxmsg,  attr.mq_curmsgs ) );
	}
#endif
	int mqretcode = 0; UNUSED_VAR(mqretcode); // release ignore OS error code.
	mqretcode = posix_send_sys_message( mq_MailBoxes[ModID], (char *)Message, WaitValue);
	XASSERT( mqretcode==0, (uint32_t)mqretcode );
	return OK;
    }
    return FAIL;
} // end MsgSend()

/*
 * Route a Sys UI  message to a given module.
*/
error_type_t SysUI_MsgSend(MODULE_ID ModID, SYS_UI_MESSAGE* Message )    
{
    uint32_t WaitValue = POSIX_WAIT_FOREVER;

    if(ModID >= NUMBERIDS)
        return FAIL;

    if (0 != mq_MailBoxes[ModID] )
	{
		if(ModID == SYSTEM_SERVICE_SENDER_ID){
			DPRINTF((LOG_SDH_ERR | DBG_OUTPUT),("SU: %04x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", 
				Message->cmd, Message->data[0], Message->data[1], Message->data[2], Message->data[3], Message->data[4], Message->data[5], Message->data[6], 
				Message->data[7], Message->data[8], Message->data[9], Message->data[10], Message->data[11], Message->data[12], Message->data[13]));
		}
		int mqretcode;		
		mqretcode = posix_message_send(mq_MailBoxes[ModID],  (char *)Message, sizeof(SYS_UI_MESSAGE), MQ_DEFAULT_PRIORITY, \
			(WaitValue == POSIX_WAIT_FOREVER) ? POSIX_WAIT_FOREVER : ((WaitValue)*(MILLISEC_PER_TICK*USEC_PER_MILLISEC)));
		XASSERT( mqretcode==0, (uint32_t)mqretcode );
		return OK;
	}
       return FAIL;
} 
    
#ifdef HAVE_DBG_PRINTF

/* FUNCTION NAME: ToString_MODULE_ID */
/** 
 * \brief Return a string with the symbolic name of the given enum value.
 * 
 * \param moduleIdEnum (MODULE_ID)
 * 
 * \return const char*
 * 
 * \author Paul Rolig
 * 
 * \date 2006/12/20
 **/
const char* ToString_MODULE_ID( MODULE_ID moduleIdEnum )
{
    switch (moduleIdEnum)
    {
        CASE_ENUM_RETURN_STRING(ENGINEID);
        CASE_ENUM_RETURN_STRING(PRINTMGRID);
        CASE_ENUM_RETURN_STRING(PARSERID);
        CASE_ENUM_RETURN_STRING(JOBMGRID);
        CASE_ENUM_RETURN_STRING(INTERNALMGRID);
        //CASE_ENUM_RETURN_STRING(VIDEOMGRID);
        CASE_ENUM_RETURN_STRING(OBSOLETE_STATMGRID);
        CASE_ENUM_RETURN_STRING(TESTPRINTMGRID);
        CASE_ENUM_RETURN_STRING(NETPORTID);
        CASE_ENUM_RETURN_STRING(SJMID);
        CASE_ENUM_RETURN_STRING(ZJPARSER);
        CASE_ENUM_RETURN_STRING(ACLPARSER);
        CASE_ENUM_RETURN_STRING(CMPARSER);
        CASE_ENUM_RETURN_STRING(PJLPARSER);
        CASE_ENUM_RETURN_STRING(I_AM_NOT_USED_ID);
        CASE_ENUM_RETURN_STRING(COPYAPPID);
        CASE_ENUM_RETURN_STRING(SCANPARSEID);
        CASE_ENUM_RETURN_STRING(USBHOSTID);
        CASE_ENUM_RETURN_STRING(SCANMANID);
        CASE_ENUM_RETURN_STRING(COMPMGRID);
        CASE_ENUM_RETURN_STRING(HBPARSER);
        CASE_ENUM_RETURN_STRING(FAXAPPID);
        CASE_ENUM_RETURN_STRING(FAXPRINTID);
        CASE_ENUM_RETURN_STRING(COMPRESSIONID);
        CASE_ENUM_RETURN_STRING(RCVFAXID);
        CASE_ENUM_RETURN_STRING(SCANTOFAXID);
        CASE_ENUM_RETURN_STRING(SENDFAXID);
        CASE_ENUM_RETURN_STRING(HANDSETID);
        CASE_ENUM_RETURN_STRING(FAXPARSERID);
        CASE_ENUM_RETURN_STRING(AUDIOID);
        CASE_ENUM_RETURN_STRING(RENDERERID);
        CASE_ENUM_RETURN_STRING(KEYPADMGRID);
        CASE_ENUM_RETURN_STRING(UIMGRID);
        CASE_ENUM_RETURN_STRING(CONFIGPAGEDEBUGSTRINGS);
        CASE_ENUM_RETURN_STRING(CONFIGPAGE_BMP);
        CASE_ENUM_RETURN_STRING(CONFIGPAGE_RNDR);
        CASE_ENUM_RETURN_STRING(SCAN_CALGRAPP_ID);
        CASE_ENUM_RETURN_STRING(CMGRID);
        CASE_ENUM_RETURN_STRING(PWRMGR_ID);
        CASE_ENUM_RETURN_STRING(PHOTOJOBMGRID);
        CASE_ENUM_RETURN_STRING(DEMO_SCANAPP_ID);
        CASE_ENUM_RETURN_STRING(FAXER_FAX_JOB_AGENT_ID);
        CASE_ENUM_RETURN_STRING(FAXER_PRINT_JOB_AGENT_ID);
        CASE_ENUM_RETURN_STRING(FAXER_SCAN_JOB_AGENT_ID);
        CASE_ENUM_RETURN_STRING(WIRELESSCONFIGID);
        CASE_ENUM_RETURN_STRING(WPSID);
        CASE_ENUM_RETURN_STRING(SCANTOMEMCARDAPPID);
        CASE_ENUM_RETURN_STRING(JBIGID);
        CASE_ENUM_RETURN_STRING(JBIG_CODEC_ID);
        CASE_ENUM_RETURN_STRING(DISPMGRID);
        CASE_ENUM_RETURN_STRING(CMDPARSER);
        CASE_ENUM_RETURN_STRING(PBMGRID);
        CASE_ENUM_RETURN_STRING(PBPARSERID);
        CASE_ENUM_RETURN_STRING(DDPARSERID);
        CASE_ENUM_RETURN_STRING(FAXREPORTSID);
        CASE_ENUM_RETURN_STRING(WSD_SCAN_APP_ID);
        CASE_ENUM_RETURN_STRING(ICEFILEID);
        CASE_ENUM_RETURN_STRING(COPY_TO_HOST_ID);
        CASE_ENUM_RETURN_STRING(UNIRAST_MODULE_ID);
        CASE_ENUM_RETURN_STRING(MLAN_EVENT_ID);
        CASE_ENUM_RETURN_STRING(USBHOSTPARID);
        CASE_ENUM_RETURN_STRING(PASSTHRUID);
        CASE_ENUM_RETURN_STRING(GENSKEL_ID);
        CASE_ENUM_RETURN_STRING(GENPIP_ID);
        CASE_ENUM_RETURN_STRING(MASSSTORAGEID);
        CASE_ENUM_RETURN_STRING(HTTP_SCAN_ID);
        CASE_ENUM_RETURN_STRING(GGS_RIP_ID);
        CASE_ENUM_RETURN_STRING(GEN_PIPP_PIPR_ID);
        CASE_ENUM_RETURN_STRING(SCANTOEMAILAPPID);
        CASE_ENUM_RETURN_STRING(MPDF_MODULE_ID);
        CASE_ENUM_RETURN_STRING(CONSUMABLES_ID);
        CASE_ENUM_RETURN_STRING(GEN_COMPRESS_ID);
        CASE_ENUM_RETURN_STRING(GSOAP_SCANAPP_ID);
        CASE_ENUM_RETURN_STRING(GSOAP_PARSER_ID);
        CASE_ENUM_RETURN_STRING(GEN_DECOMPRESS_ID);
        CASE_ENUM_RETURN_STRING(GEN_MIP_ID);
        CASE_ENUM_RETURN_STRING(FW_UPDATE_ID);
        CASE_ENUM_RETURN_STRING(GEN_WIFI_PRINT_ID);
        CASE_ENUM_RETURN_STRING(JPEG_PARSER_ID);
        CASE_ENUM_RETURN_STRING(GENPIP_CP_ID);
        CASE_ENUM_RETURN_STRING(GEN_NUP_ID);
        CASE_ENUM_RETURN_STRING(SMJOB_SCAN_APP_ID);
        CASE_ENUM_RETURN_STRING(PHOTOCOPY_ID);
        CASE_ENUM_RETURN_STRING(SYSTEM_JOB_MGR_TEST_ID);
        CASE_ENUM_RETURN_STRING(PRINT_APP_TEST_ID);
        CASE_ENUM_RETURN_STRING(FWUPGRADEID);
        CASE_ENUM_RETURN_STRING(VPI_MODULE_ID);
        CASE_ENUM_RETURN_STRING(VPI_SYSTEM_ID);
        CASE_ENUM_RETURN_STRING(HOST1284_4_HANDLER_ID);
        CASE_ENUM_RETURN_STRING(PRINTER1284_4_WRAPPER_ID);
        CASE_ENUM_RETURN_STRING(NFC_ID);
        CASE_ENUM_RETURN_STRING(PFPARSER);			// PF Parser		2013.08.28	by Juny
        CASE_ENUM_RETURN_STRING(PCFAXPARSER);		// PCFAX Parser	2014.02.20	by inhwan5
        DEFAULT_CASE_ENUM_RETURN_STRING(MODULE_ID);
    } // end switch (moduleIdEnum)
    return "bug";
} // end ToString_MODULE_ID()
#endif // ifdef DEBUG

