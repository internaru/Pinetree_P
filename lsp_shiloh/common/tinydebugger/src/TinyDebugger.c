/*
 ***********************************************************
 *(c) Copyright Sindoh Co. Ltd. 
 *
 * SINDOH Confidential
 * @Version: 0.1
 *
 ***********************************************************
 **/
 
/**
 *\file TinyDebugger.cpp
 *
 *\brief Receive message from tiny debug client.
 * Do requested work.
 *
 **/
 
#include <sys/prctl.h>
#include <string.h>
#include "TinyDebugger.h"
#include "agRouter.h"
#include "agMessage.h"
#include "posix_ostools.h"
#include "logger.h"
#include "lassert.h"
#include "sys_init_api.h"
#include "SysUI_Interface.h"

#define TINY_DBG_STACK_SIZE POSIX_MIN_STACK_SIZE
#define NUM_MESSAGES 20

#define DBG_PRFX "[TD]: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 13 )

#define TINY_DEBUG_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__) 
#define TINY_DEBUG_DBG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

static ALIGN8 UINT8 TinyDebuggerStack[TINY_DBG_STACK_SIZE];      
static pthread_t TinyDebugger_thd_id;
static mqd_t TinyDebugger_queue;

#define CMD_TD_USB_CONNECT 0x0A01
#define CMD_TD_SYSTEM_DEBUG 0x0A02
#define CMD_TD_CMD_DEBUG 0x0A03


#if 0
#define MAX_MSG_CMD_LEN     21	//21 Command
#define MAX_MSG_DATA_LEN     51	//51 Data

typedef struct{
	char cmd[MAX_MSG_CMD_LEN];
	char data[MAX_MSG_DATA_LEN];
}TINY_DBG_MESSAGE;
#endif

typedef struct stTinyMsgData_{	
	unsigned int option;
}stTinyMsgData;


/**
 * 
 *\brief Debug USB command
 *
 *\param data received to do for USB command
 *
 **/
void USBDebugger(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;	
	stIC_USBThumbDrive stUSBConnection = {0};
	bool msgSendFlag = true;

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_INT_USB_THUMB_DRIVE;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);	
	
	if (strcmp("remove", (char *)Message->data) == 0){
		TINY_DEBUG_ERR("USB remove\n");
		stUSBConnection.status = UDB_STATUS_REMOVE;
	}
	else if(strcmp("add", (char *)Message->data) == 0){
		TINY_DEBUG_ERR("USB add\n");
		stUSBConnection.status = UDB_STATUS_ADD;
	}
	else if(strcmp("mount", (char *)Message->data) == 0){
		TINY_DEBUG_ERR("USB mount\n");
		stUSBConnection.status = UDB_STATUS_MOUNT;
	}
	else{
		msgSendFlag = false;
	}

	if(msgSendFlag){
		memcpy(msgToSend.data, &stUSBConnection, sizeof(stIC_USBThumbDrive));
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

/**
 * 
 *\brief Debug SYS command
 *
 *\param data received to do for SYS command
 *
 **/
void SYSDebugger(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;	
	stIC_SysJobFakeAction stSysJobAct = {0};
	bool msgSendFlag;

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	
	if (strcmp("fakecopy", (char *)Message->data) == 0){
		TINY_DEBUG_ERR("action fakecopy\n");
#if 0	
		msgToSend.cmd = CMD_INT_TD_SYS_JOB_ACTION;
		stSysJobAct.jobType = 1;	//Fake Copy
		stSysJobAct.action = COPY_ACTION_START;
#else
		msgSendFlag = false;
#endif
	}
	else{
		msgSendFlag = false;
	}

	if(msgSendFlag){
		memcpy(msgToSend.data, &stSysJobAct, sizeof(stIC_SysJobFakeAction));
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}


/**
 * 
 *\brief Debug MVL (Marvell) command
 *
 *\param data received to do for MVL command
 *
 **/
void MVLDebugger(SYS_UI_MESSAGE *Message)
{
	TINY_DEBUG_ERR("%s: %s\n", __func__, Message->data);

extern void serial_console_cmd_run(char *cmd);
	serial_console_cmd_run((char *)Message->data);
}



/**
 * Function name: TinyDebuggerThread
 *
 * \brief Main thread of the tiny debugger
 *
 * Receive message from tiny debug client and do required things.
 *
 **/
void *TinyDebuggerThread(void *unused)
{
	SYS_UI_MESSAGE nextMsg;
	int retcode;

	SysWaitForInit ();
	prctl(PR_SET_NAME, "tinydebugger", 0, 0, 0);

	// Wait for a message
	while (1)
	{
		retcode = mq_receive(TinyDebugger_queue, (char*)&nextMsg, sizeof(SYS_UI_MESSAGE), 0); 

		//TINY_DEBUG_ERR("  Received Message:  %s \n", __FUNCTION__);
		
		XASSERT( retcode == sizeof(SYS_UI_MESSAGE), thread_errno);

		TINY_DEBUG_ERR("%s[%d]: got message  CMD:0x%X\n", 	__FUNCTION__, __LINE__, nextMsg.cmd);

		switch(nextMsg.cmd)
		{
			case CMD_TD_USB_CONNECT:  //USB connection
			{
				USBDebugger(&nextMsg);
				break;
			}
			case CMD_TD_SYSTEM_DEBUG: 	//System Debug
			{
				SYSDebugger(&nextMsg);
				break;
			}
			case CMD_TD_CMD_DEBUG: 	//Cmd Debug
			{
				MVLDebugger(&nextMsg);
				break;
			}
			default:
				TINY_DEBUG_ERR("%s[%d]:  CMD:%s, DATA:%s not found.\n", __FUNCTION__, __LINE__, nextMsg.cmd, nextMsg.data);
				break;
		} // end switch ( nextMsg.msgType )
	} // end while (1)
	return 0;
}

void TinyDebuggerInit(void)
{
	posix_create_message_queue( &TinyDebugger_queue, "/mq_tiny_debugger", NUM_MESSAGES, sizeof(SYS_UI_MESSAGE) );
	router_register_queue(TINY_DEBUGGER_ID, TinyDebugger_queue);  // Register a message queue.
	posix_create_thread(&TinyDebugger_thd_id, TinyDebuggerThread, 0, "tinydebugger", TinyDebuggerStack, TINY_DBG_STACK_SIZE, POSIX_THR_PRI_NORMAL);	
}

//END
