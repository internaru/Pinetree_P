/**
 *
 * ============================================================================
 * Copyright (c) 2015 SINDOH, Inc. All Rights Reserved 
 *
 *                         SINDOH Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file imagelogMonitor.c 
 * \brief Implements a imagelog Monitor.
 */

/** include files **/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/prctl.h>	//for Thread Name
#include <dirent.h>

#include "agRouter.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "SysUI_Interface.h"
#include "db_api.h"

#include "imagelog.h"
#include "imagelogMonitor.h"
#include "solution.h"

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define ILM_STACK_SIZE POSIX_MIN_STACK_SIZE
#define ILM_QUEUE_SIZE 50

#define DBG_PRFX "ILM: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_IMAGELOG | LOGGER_SUBMODULE_BIT( 1 )
#define ILM_SDH_ERR DBG_PRINTF_SDH_ERR
#define ILM_SDH_DEBUG DBG_PRINTF_SDH_DEBUG

#define IMAGE_LOG_PATH_LENGTH 256
char g_ImageLogToSend[IMAGE_LOG_PATH_LENGTH];

#define MSG_IMAGE_LOG_SEND_FAIL 2003
#define MSG_IMAGE_LOG_SEND_SUCCESS 2004
//#define MSG_IMAGE_LOG_CONFIG_CHANGED 2005

#define IMAGE_LOG_TIMEOUT_MAX 1800	///<In Seconds 1800=30*60 30 Minutes

typedef enum {
    IL_DELIVERY_IMMEDIATE = 0,
    IL_DELIVERY_SET_TIME
} IMAGE_LOG_DELIVERY_MODE;

typedef enum {
    IL_TIMEOUT_DELIVERY = 1,
    IL_TIMEOUT_INTERVAL = 2,
} IMAGE_LOG_TIMEOUT_MODE;

typedef enum {
	IL_DELIVERY_STATE_NONE = 0,
	IL_DELIVERY_STATE_PROGRESS,
	IL_DELIVERY_STATE_PROGRESS_NEWRECV,
	IL_DELIVERY_STATE_RETRY,
} IMAGE_LOG_DELIVERY_STATE;

typedef enum {
	IMAGE_LOG_MEMORY_FULL = 101,
	IMAGE_LOG_SEND_RETRY_FAIL,
} IMAGE_LOG_ERROR;

/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */
static mqd_t ImageLogMon_queue;
static pthread_t ImageLogMonThreadId;
static ALIGN8 uint8_t ImageLogMonStack[ILM_STACK_SIZE];

extern uint32_t g_nSaveImageLog; ///<Save Image Log 0: Off 1: On
extern uint32_t g_nIsEnableImageLog; ///<Image Log 0: Off 1: On
extern uint32_t g_nILFileTransfer; ///<0: Off 1: On
static uint32_t g_nILDeliveryType = 0; ///<0: Immediate 1: Set Time
static uint32_t g_nILSendFailSCDisplay = 0; ///<0: Display notification only 1: Display SC occur
static uint32_t g_nILDeliveryState = 0;
static unsigned long g_nILSendingTime = 0; ///<Start Sending Time. Set this time with Seconds
static unsigned long g_nILSendingTimeTO = 0; ///<End Sending Time. Set this time with Seconds
static uint32_t g_nSendRetryCount = 0;
static uint32_t g_nSendRetryMax = 10;
static uint32_t g_nRetryIntervalMax = 120;
static uint32_t g_nSendLogAfterDeliveryDone = 0;
static uint32_t g_nConfigChangedPending = 0;
uint32_t g_nImageLogSendingFailed = 0;

//Timer
extern int g_nImageLogDeliveryTimeout;
extern int g_nImageLogIntervalTimeout;


/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */
extern void SendSCStatusToUI(UINT32 scCode, UINT32 senderModule);
extern void SetImageLogStatus(uint32_t status);

static void *ImageLogMonitorThread(void *unused);

int TakeDeliveryAction(unsigned int notifyType, unsigned int param2);
void CheckTimeoutCondition(unsigned int timeout, unsigned int category);
int ReadImageLogPath();
int SendImageLogData();
void RetrySendingImageLog();
int ImageLogDeliveryResult(unsigned int sendResultJobId, unsigned int errorCode);
int ImageLogDeleteFolder(char *logPath);
void SendNotificationToSSM(int type, int option);

void ReadConfigData()
{
	int getIntVal = 0;
	error_type_t db_retval = FAIL;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "Delivery", &db_retval);
	if(db_retval == FAIL){
		ILM_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:Delivery\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nILDeliveryType = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "SendingTime", &db_retval);
	if(db_retval == FAIL){
		ILM_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:SendingTime\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nILSendingTime = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "SendingTimeTO", &db_retval);
	if(db_retval == FAIL){
		ILM_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:SendingTimeTO\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nILSendingTimeTO = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "NumberOfRetry", &db_retval);
	if(db_retval == FAIL){
		ILM_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:NumberOfRetry\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nSendRetryMax = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "RetryInterval", &db_retval);
	if(db_retval == FAIL){
		ILM_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:RetryInterval\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nRetryIntervalMax = getIntVal*60;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "SendFailSCDisplay", &db_retval);
	if(db_retval == FAIL){
		ILM_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:SendFailSCDisplay\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nILSendFailSCDisplay = getIntVal;

	ILM_SDH_DEBUG("%s DeliveryType:%d SendingTime:%d SendingTimeTO:%d SendRetryMax:%d RetryIntervalMax:%d SendFailScDisplay:%d\n", __FUNCTION__,
			g_nILDeliveryType, g_nILSendingTime, g_nILSendingTimeTO, g_nSendRetryMax, g_nRetryIntervalMax, g_nILSendFailSCDisplay);
}

void SetInitialData()
{
	ReadConfigData();
	PopulateImageLogVars(); //Network DB
	g_nConfigChangedPending = 0;
	g_nImageLogSendingFailed = 0;
	g_nSendRetryCount = 0;

	ILM_SDH_ERR("%s IsEnableImageLog:%d ILFileTransfer:%d ILDeliveryType:%d\n", __FUNCTION__, g_nIsEnableImageLog, g_nILFileTransfer, g_nILDeliveryType);
	if(g_nIsEnableImageLog && g_nILFileTransfer && (g_nILDeliveryType == IL_DELIVERY_SET_TIME)){
		g_nImageLogDeliveryTimeout = IMAGE_LOG_TIMEOUT_MAX;
	}
}

int ImageLogMonitorInit()
{
	SetInitialData();

	// Create a Queue(mail box) and register it.
	posix_create_MESSAGE_queue( &ImageLogMon_queue, "/imglogmon_mq", ILM_QUEUE_SIZE );
	// Register the queue with the router.
	router_register_queue(IMAGE_LOG_MONITOR_ID, ImageLogMon_queue);
	// start the thread 
	posix_create_thread(&ImageLogMonThreadId,
						ImageLogMonitorThread,
						0,
						"ImageLogMonitor",
						ImageLogMonStack,
						ILM_STACK_SIZE,
						POSIX_THR_PRI_NORMAL);

	return 0;
}

void* ImageLogMonitorThread( void *unused )
{
	MESSAGE msg;
	int iret;

	SysWaitForInit();
	prctl(PR_SET_NAME,"ImageLogMonitor", 0, 0, 0);

	// wait for message
	while(1)
	{
		iret = posix_wait_for_message(ImageLogMon_queue, (char*)&msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);
		if (iret != 0)
			continue;  // signal interrupt

		switch(msg.msgType)
		{
			case MSG_IMAGE_LOG_NOTIFY:
			{
				if(g_nIsEnableImageLog && g_nILFileTransfer){
					if(g_nILDeliveryState == IL_DELIVERY_STATE_PROGRESS){
						g_nILDeliveryState = IL_DELIVERY_STATE_PROGRESS_NEWRECV;
					}
					else{
						//g_nILDeliveryState = IL_DELIVERY_STATE_NONE; ///TODO: Remove from here. Its Temporary for TESTING
					}
					TakeDeliveryAction(msg.param1, msg.param2);
				}
				break;
			}
			case MSG_IMAGE_LOG_TIMEOUT:
			{
				CheckTimeoutCondition(msg.param1, msg.param2);
				break;
			}
			case MSG_CONFIG_CHANGED:
			{
				ILM_SDH_ERR("MSG_CONFIG_CHANGED Received, DeleveryState:%d\n", g_nILDeliveryState);
				if((g_nILDeliveryState == IL_DELIVERY_STATE_PROGRESS) ||
						(g_nILDeliveryState == IL_DELIVERY_STATE_PROGRESS_NEWRECV)){
					g_nConfigChangedPending = 1;
				}
				else{
					SetInitialData();
					TakeDeliveryAction(IL_NOTIFY_CONFIG_CHANGED, msg.param2);
				}
				break;
			}
			case MSG_SENDJOB_RESULT:
			{
				ImageLogDeliveryResult(msg.param1, msg.param2);
				break;
			}
			default:
				break;
		} // switch

	} // while
	return 0;
}

void CheckTimeoutCondition(unsigned int timeout, unsigned int category)
{
	ILM_SDH_DEBUG("%s DeliveryType:%d Timeout:%d Category:%d\n", __FUNCTION__, g_nILDeliveryType, timeout, category);
	if(category == IL_TIMEOUT_DELIVERY){
		if(g_nILDeliveryType == IL_DELIVERY_SET_TIME){
			unsigned long todayTimeInSecond;
			time_t rawTime;
			rawTime=time(NULL);
			todayTimeInSecond = (rawTime%(86400)); //86400 = 24*60*60
			ILM_SDH_ERR("%s: Now:%ld, SendingTime:%ld\n", __FUNCTION__, todayTimeInSecond, g_nILSendingTime);
			if((todayTimeInSecond <= g_nILSendingTime) && (todayTimeInSecond >= g_nILSendingTimeTO)){
				//Start Delivery
				SendImageLogData();
				g_nImageLogDeliveryTimeout = IMAGE_LOG_TIMEOUT_MAX; ///TODO: Set long timeout PosixTimer
			}
			else{
				int timeDiff = (g_nILSendingTime - todayTimeInSecond);
				ILM_SDH_ERR("%s: Timeout:%d, TimeDiff:%d\n", __FUNCTION__, g_nImageLogDeliveryTimeout, timeDiff);
				if((timeDiff > 0 ) && (timeDiff <= IMAGE_LOG_TIMEOUT_MAX )){
					g_nImageLogDeliveryTimeout = timeDiff;
				}
			}
//			struct tm *curTimeInfo;
//			curTimeInfo = localtime(&rawTime);
		}
	}
	else if(category == IL_TIMEOUT_INTERVAL){
		g_nImageLogIntervalTimeout = 0;
		//It is a RETRY timeout message so start retry again
		if(g_nSendRetryCount > 0){
			//g_nSendRetryCount--;
			SendImageLogData();
		}
	}
}

/**
 * \brief Take Delivery Action
 * Check delivery type and notifyType then take action
 * This routine can be called on MSG_IMAGE_LOG_NOTIFY,
 * MSG_CONFIG_CHANGED and previous Image Log sent Success.
 *
 * \param notifyType unsigned int see IMAGE_LOG_NOTIFY_TYPE
 * \param param2 unsigned int For extra information
 *
 */
int TakeDeliveryAction(unsigned int notifyType, unsigned int param2)
{
	ILM_SDH_DEBUG("%s DeliveryType:%d notifyType:%d Param2:%d\n", __FUNCTION__, g_nILDeliveryType, notifyType, param2);

	if((notifyType == IL_NOTIFY_SYS_INIT) || (notifyType == IL_NOTIFY_CONFIG_CHANGED)){
		sleep(5); //Wait for system init
	}

	switch(g_nILDeliveryType){
		case IL_DELIVERY_IMMEDIATE:
			if(notifyType == IL_NOTIFY_JOB_END){
				sleep(2); //Wait to save ImageLog(pdf files)
			}
			//case 1. Last log sent successfully then wait for 2 seconds and start sending process
			//case 2. Retry in progress then don't start sending ImageLog, because retry will do
			//case 3. Retry reached MAX then check
			//			if SendFailSCdisaply == 0, then set DeliveryState=IL_DELIVERY_STATE_NONE
			//				so start sending again &  SendImageLogData will reset g_nSendRetryCount=0
			//			else
			//				already SC occurred so do nothing here
			if(g_nSendRetryCount <= 0){ //case 1: No retry in progress
				SendImageLogData();
			}
			else if(g_nSendRetryCount >= g_nSendRetryMax){ //case 3: Reached MAX retry
				if(g_nILSendFailSCDisplay == 0){ //If SendFailSCdisaply==0 then try again to send again
					g_nILDeliveryState = IL_DELIVERY_STATE_NONE;
					SendImageLogData();
				}
			}
			break;
		case IL_DELIVERY_SET_TIME:
			if((notifyType == IL_NOTIFY_SYS_INIT) || (notifyType == IL_NOTIFY_SEND_NEXT)){
				CheckTimeoutCondition(0, IL_TIMEOUT_DELIVERY);
			}
			break;
		default:
			break;
	}
	return 0;
}

int IsImageLogSysWantToSend(const char *parentDir, char *entryName)
{
	struct stat stFileAttr;
	if (!strcmp(".", entryName) || !strcmp("..", entryName)){
		return 0;
	}

	//Automatically free the space allocated with alloca
	//when they exit through the function that called alloca
	size_t logPathLength = strlen(entryName) + strlen(parentDir) + 6;
	char *ImageLogDir = alloca(logPathLength);
	sprintf(ImageLogDir, "%s/%s", parentDir, entryName);
	ILM_SDH_ERR("Path:%s\n", ImageLogDir);

	if(stat(ImageLogDir, &stFileAttr) == 0){
		if(S_ISDIR(stFileAttr.st_mode)){
			//sprintf(path, "%s/.end", path);
//			char sysCmd[IMAGE_LOG_PATH_LENGTH+10] = {0x00,};
//			sprintf(sysCmd, "ls -al %s", ImageLogDir);
//			system(sysCmd);

			strcat (ImageLogDir, "/.end");
			if(stat(ImageLogDir, &stFileAttr) == 0){
				ILM_SDH_ERR(".end exist at Path:%s\n", ImageLogDir);
				//ENOENT
				if(S_ISREG(stFileAttr.st_mode)){
					memset(g_ImageLogToSend, 0x00, IMAGE_LOG_PATH_LENGTH);
					memcpy(g_ImageLogToSend, ImageLogDir, logPathLength-5);
					ILM_SDH_DEBUG("ToSend:%s\n", &g_ImageLogToSend);
					return 1;
				}
			}
			else{
				ILM_SDH_ERR(".end not available at Path:%s\n", ImageLogDir);
			}
		}
	}
	return 0;
}

/**
 * \brief Read Image log directory and call network API to deliver Image Log
 *
 * \param logPath Path to Image Log folder
 *
 */
int ReadImageLogPath(const char *logPath)
{
	struct dirent *ent;
	int uploadResult = 0;
	DIR *dir = opendir(logPath);

	if(dir == NULL){
		ILM_SDH_ERR("[%s] Error cannot open directory:%s\n", __FUNCTION__, logPath);
		return 1;
	}

	while ((ent = readdir(dir)) != NULL){
		char *entryName = ent->d_name;
		ILM_SDH_ERR("ImageLog:%s\n", entryName);
		if (IsImageLogSysWantToSend(logPath, entryName)) {
///TODO: send all Dir once or send one by one
			g_nILDeliveryState = IL_DELIVERY_STATE_PROGRESS;
			ILM_SDH_ERR("Start Uploading:%s\n", &g_ImageLogToSend);

			uploadResult = Image_log_Upload(g_ImageLogToSend);
			if(uploadResult != 0){
				g_nILDeliveryState = IL_DELIVERY_STATE_NONE;
				ILM_SDH_ERR("Image_log_Upload Failed, ErrorCode:%d\n", uploadResult);
			}
			break;

			// You can consider using alloca instead.
			//char *next = malloc(strlen(name) + strlen(entry_name) + 2);
			//sprintf(next, "%s/%s", name, entry_name);
			//list(next);
			//free(next);
		}
	}
	closedir(dir);
	return 0;
}

/**
 * \brief Check current delivery state and read Image Log path
 * If delivery already in progress then try later.
 */
int SendImageLogData()
{
	ILM_SDH_ERR("[%s] DeliveryState:%d RetryCount:%d\n", __FUNCTION__, g_nILDeliveryState, g_nSendRetryCount);
	if(g_nSaveImageLog == 0){
		ILM_SDH_ERR("[%s] Don't send now because SaveImageLog=0\n", __FUNCTION__);
		return -1;
	}

	if(g_nIsEnableImageLog && g_nILFileTransfer){
		if((g_nILDeliveryState == IL_DELIVERY_STATE_NONE) ||
				(g_nILDeliveryState == IL_DELIVERY_STATE_RETRY)){
			ReadImageLogPath(IMAGE_LOG_PATH);

			///TODO: If Retry counter MAX then decide to proceed from here or not
			///TODO: Remove below condition to reset retry counter
			if(g_nILDeliveryState == IL_DELIVERY_STATE_NONE){
				g_nSendRetryCount = 0;
			}
		}
		else{
			ILM_SDH_ERR("Already in IL_DELIVERY_STATE_PROGRESS\n");
			g_nSendLogAfterDeliveryDone = 1; //g_nSendLogAfterDeliveryDone++;
		}
	}
	else{
		ILM_SDH_ERR("[%s] Failed to Start ImageLogSending IsImageLogEnabled:%d FileTransfer:%d\n", __FUNCTION__, g_nIsEnableImageLog, g_nILFileTransfer);
	}
	return 0;
}

/**
 * \brief Retry Sending Image Log.
 * Keep track of Retry count and wait for interval.
 *
 */
void RetrySendingImageLog()
{
	ILM_SDH_ERR("%s RetryCount:%d Max:%d, Timeout:%d\n", __FUNCTION__, g_nSendRetryCount, g_nSendRetryMax, g_nRetryIntervalMax);
	if(g_nSendRetryCount < g_nSendRetryMax){
		g_nSendRetryCount++;
		g_nILDeliveryState = IL_DELIVERY_STATE_RETRY;

		g_nImageLogIntervalTimeout = g_nRetryIntervalMax;

		//Wait for Timeout message for retry
	}
	else{
		ILM_SDH_ERR("%s Retried MAX, Stopped ImageLog sending\n", __FUNCTION__);
		g_nImageLogIntervalTimeout = 0;

		SetImageLogStatus(ILS_TRANSFER_ERR);

		if(g_nILSendFailSCDisplay){
			g_nImageLogSendingFailed = 1; //Do not allow new job in SJM
			SendSCStatusToUI(SC_IMAGELOG_TRANSFER_ERR, SYSTEM_MODULE_SCID);
		}
		else{
			//Send Special Notification to UI
			SendNotificationToSSM(SNT_IMAGE_LOG_FAILED_TO_DELIVER, SNO_SHOW_ON_TOP_LINE);
		}
	}
}

/**
 * \brief Check Image Log Delivery Result and take action accordingly
 *
 * \param sendResultJobId Job ID
 * \param errorCode Error code success or fail
 *
 */
int ImageLogDeliveryResult(unsigned int sendResultJobId, unsigned int errorCode)
{
	ILM_SDH_DEBUG("%s: JobID:%d, ErrorCode:%d\n", __FUNCTION__, sendResultJobId, errorCode);
	g_nILDeliveryState = IL_DELIVERY_STATE_NONE;

	if(g_nConfigChangedPending){
		SetInitialData();
		TakeDeliveryAction(IL_NOTIFY_CONFIG_CHANGED, 0);
	}
	else{
		if(errorCode == 0){ //Success
			///Get ID or ImageLog Path that sent successfully
			ILM_SDH_ERR("SendSuccess:%s\n", &g_ImageLogToSend);
			ImageLogDeleteFolder(g_ImageLogToSend);

			//If Retry was MAX and now sent successfully so send CLEAR Special Notification to UI
			if(g_nILSendFailSCDisplay == 0){
				if(g_nSendRetryCount >= g_nSendRetryMax){
					SendNotificationToSSM(SNT_IMAGE_LOG_FAILED_TO_DELIVER, SNO_CLEAR_NOTIFICATION);
					SetImageLogStatus(ILS_NO_PROBLEM);
				}
			}

			g_nSendRetryCount = 0;
			g_nImageLogSendingFailed = 0;

			//Take action after checking current Delivery type
			TakeDeliveryAction(IL_NOTIFY_SEND_NEXT, 0);
		}
		else{ //On Fail Retry
			RetrySendingImageLog();
		}
	}

	return 0;
}

/**
 * \brief Delete Image Log file for which delivery done
 *
 * \param folderPath char path to Image Log folder
 *
 */
int ImageLogDeleteFolder(char *logPath)
{
	ILM_SDH_ERR("[%s] Start Deleting:%s<<\n", __FUNCTION__, logPath);
	char sysCmd[IMAGE_LOG_PATH_LENGTH+10] = {0x00,};
	sprintf(sysCmd, "rm -rf %s", logPath);
	system(sysCmd);
	system("sync");
	ILM_SDH_DEBUG("[%s] Deleted !!!\n", __FUNCTION__);
	return 0;
}

void SendNotificationToSSM(int type, int option)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stSpecial_Notification stSpecialNotify = {0};

	ILM_SDH_DEBUG("[%s] type:%d option:%d\n", __FUNCTION__, type, option);

	msgToSend.msgSender = IMAGE_LOG_MONITOR_ID;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	msgToSend.cmd = CMD_INT_SPECIAL_NOTIFICATION;

	stSpecialNotify.type = type;
	stSpecialNotify.option = option;
	memcpy(msgToSend.data, &stSpecialNotify, sizeof(stSpecial_Notification));
	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msgToSend);
	XASSERT( err==OK, err );
}
//EOF
