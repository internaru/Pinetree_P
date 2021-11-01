/**
 *
 * ============================================================================
 * Copyright (c) 2016 SINDOH, Inc. All Rights Reserved
 *
 *                         SINDOH Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file scanbox.c 
 * \brief Implemention of scan box functionality.
 */

/** include files **/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <dirent.h>

#include "agRouter.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "scanbox.h"
#include "SysUI_Interface.h"
#include "db_api.h"

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define SCAN_BOX_STACK_SIZE 2*POSIX_MIN_STACK_SIZE ///< The size of the stack
#define SCAN_BOX_QUEUE_SIZE 20

#define DBG_PRFX "SB: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SCANBOX | LOGGER_SUBMODULE_BIT( 1 )
#define SB_SDH_ERR DBG_PRINTF_SDH_ERR
#define SB_SDH_DEBUG DBG_PRINTF_SDH_DEBUG

#define SB_SECONDS_IN_A_DAY 86400
#define SB_SECNODS_IN_HOUR 3600
#define SB_TIME_MARGIN_SECONDS 61
#define SCAN_BOX_MIN_MEMSIZE (100)	//In MB
#define SCAN_BOX_DEL_OLD_JOB_MEMSIZE (SCAN_BOX_MIN_MEMSIZE*5)	//In MB
#define FILE_SYSTEM_EXT4      0xEF53
#define SEARCH_PATH_MAX 1024 //2048  //4096
#define DELETE_LEVEL_MAX 5

//#define SCAN_BOX_TEST_FOLDER
#ifdef SCAN_BOX_TEST_FOLDER
#define SCAN_BOX_DIR_PATH "/mnt/sda1/scanbox" //"/tmp/scanbox"
#define SCAN_BOX_SD_MOUNT_PATH "/mnt/sda1" //"/tmp"
#else
#define SCAN_BOX_DIR_PATH "/mnt/sd/scanbox"
#define SCAN_BOX_SD_MOUNT_PATH "/mnt/sd"
#endif

typedef enum {
	SCAN_BOX_TIMEOUT_INTERVAL = 1,
    SCAN_BOX_TIMEOUT_POR = 2,
} SCAB_BOX_TIMEOUT_MODE;

/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */
static mqd_t scanBoxQueueID;
static pthread_t scanBoxThreadID;
static ALIGN8 uint8_t scanBoxStack[SCAN_BOX_STACK_SIZE];
static pthread_mutex_t gScanBoxDeleteLock;
static uint32_t g_nIsEnableScanBox = 0; ///<Scan Box Disable: Off 1: Enable
static uint32_t g_nSBDeleteIntervalDay = 0; ///<Scan box delete interval day 0, 1, 2, 7, 30 days
static uint32_t g_nScanBoxDeletedLastTime = 0; ///<Scan Box deleted last time in unix time
static uint32_t g_nScanBoxDeleteHour = 0; ///<Scan Box delete hour
static uint32_t g_nScanBoxDeleteFromTime = 0; ///<Set time_t before deleting starts
static uint32_t g_nDisplayFullSDCardSC = 0; ///<0: Do not display 1: Display SC occur
static uint32_t g_nScanBoxCanSave = 0; ///<Save Scan box 0: Off 1: On
uint32_t g_nScanBoxMemFull = 0; ///0: Not Full 1: Memory Full (Do not allow job in SJM)
uint32_t g_nScanBoxSDCardError = 0; ///0: Valid 1: Invalid format (Do not allow job in SJM)

extern int g_nScanBoxDeleteIntervalTimeout; ///< Set by ScanBox in Seconds


/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */

static void* ScanBoxThread( void *unused );
static void ScanBoxInitializeData();
static void ScanBoxReadDB();
static int SbGetSDCardStatus();
//static int SbGetFileSystemMemUsage(char *path);
static int SbGetFileSystemType(char *path);
static void ScanBoxDeleteFiles(const char* path, int level);
static void ScanBoxTimeoutAction(unsigned int timeout, unsigned int category);
static void ScanBoxNextDeleteTimeout();
static void ScanBoxSetLastDeletedTime();
static void ScanBoxNotificationToSSM(int type, int option);

bool IsScanBoxEnabled();
extern void SendSCStatusToUI(UINT32 scCode, UINT32 senderModule);


//-----------------------------------------------------------//
void ScanBoxInit()
{
	int px_status = 0;

	px_status = posix_mutex_init(&gScanBoxDeleteLock);
	XASSERT(px_status == 0, px_status);

	// Create a mail box and register it.
	posix_create_MESSAGE_queue(&scanBoxQueueID, "/scanBox", SCAN_BOX_QUEUE_SIZE);
	// Register the queue with the router.
	router_register_queue(SCAN_BOX_ID, scanBoxQueueID);
	// start the generator task
	posix_create_thread(&scanBoxThreadID, ScanBoxThread, 0, "ScanBox",
			scanBoxStack, SCAN_BOX_STACK_SIZE, POSIX_THR_PRI_NORMAL);

	//Check DB for ImageLog Enable and then check SD Card
	//ScanBoxInitializeData(); //ON config changed also doing same things
}

void* ScanBoxThread( void *unused )
{
	MESSAGE msg;
	//error_type_t err;
	int iret;

	SysWaitForInit();

	// main loop
	while (1)
	{
		iret = posix_wait_for_message(scanBoxQueueID, (char*) &msg,
				sizeof(MESSAGE), POSIX_WAIT_FOREVER);
		if (iret != 0)
			continue;  // signal interrupt

		switch (msg.msgType)
		{
			case MSG_SCAN_BOX_TIMEOUT:
			{
				ScanBoxTimeoutAction(msg.param1, msg.param2);
				break;
			}
			case MSG_CONFIG_CHANGED:
			{
				SB_SDH_ERR("ScanBox MSG_CONFIG_CHANGED received\n");
				//On Config Change msg:
				//Check DB for ScanBox Enabled and then check SD Card
				ScanBoxInitializeData();
				break;
			}
			default:
				break;
		} // switch
	} // while
	return 0;
}

void ScanBoxTimeoutAction(unsigned int timeout, unsigned int category)
{
	SB_SDH_ERR("%s Timeout:%d Category:%d\n", __FUNCTION__, timeout, category);

	if(IsScanBoxEnabled() == FALSE){
		SB_SDH_ERR("%s Error ScanBox is disabled\n", __FUNCTION__);
		g_nScanBoxDeleteIntervalTimeout = 0;
		return;
	}

	//Steps
	//1. Send Msg to GUI: ScanBox deleting in progress
	//2. Scan and delete old files from all the directories
	//3. set last deleted time
	//4. calculate and set next timeout
	//5. Send Msg to GUI: ScanBox deleting is done
#if 0
	if(category == SCAN_BOX_TIMEOUT_POR){
		//For advance
	}
	else if(category == SCAN_BOX_TIMEOUT_INTERVAL){
		//For advance
	}
#endif

	ScanBoxNotificationToSSM(SNT_SCANBOX_DELETING_IN_PROGRESS, SNO_SHOW_POPUP_FOREVER);

	//Added sleep before starting deleting operation.
	//To ensure GUI receive deleting message before Deleting starts.
	usleep(500*1000); //Wait for message to send to GUI

	time_t nowSec;
	nowSec=time(NULL);
	g_nScanBoxDeleteFromTime = nowSec - (nowSec%SB_SECONDS_IN_A_DAY) - (g_nSBDeleteIntervalDay*SB_SECONDS_IN_A_DAY);
	SB_SDH_ERR("File to delete: Time now:%d delete from:%d IntervalDay:%d\n",
			nowSec, g_nScanBoxDeleteFromTime, g_nSBDeleteIntervalDay);

	ScanBoxDeleteFiles(SCAN_BOX_DIR_PATH, 1);
	//
	//Set next delete timeout
	ScanBoxSetLastDeletedTime();
	ScanBoxNextDeleteTimeout();
	//Reset delete now now
	g_nScanBoxDeleteFromTime = 0;

	ScanBoxNotificationToSSM(SNT_SCANBOX_DELETING_IN_PROGRESS, SNO_CLEAR_NOTIFICATION);
}

bool IsScanBoxEnabled()
{
	bool retVal = FALSE;
	if(g_nIsEnableScanBox && g_nScanBoxCanSave){
		retVal = TRUE;
	}
	else{
		retVal = FALSE;
	}
	return retVal;
}

void ScanBoxSetLastDeletedTime()
{
	time_t nowSec;
	nowSec=time(NULL);
	error_type_t db_retval;
	UINT32 timeNowInSec = nowSec;
	db_retval = db_set_var_int("CONF_SOLUTIONUP", "ScanBoxDeletedLastTime", timeNowInSec);
	if(db_retval == FAIL){
		SB_SDH_ERR("[%s] error DB update ScanBoxDeletedLastTime\n", __FUNCTION__);
	}
	g_nScanBoxDeletedLastTime = timeNowInSec;
}

void ScanBoxReadDB()
{
	int getIntVal = 0;
	error_type_t db_retval = FAIL;

	getIntVal = db_get_var_int("CONF_SOLUTIONUP", "ScanBoxEnable", &db_retval);
	if(db_retval == FAIL){
		SB_SDH_ERR("%s: Fail to get DB data ScanBoxEnable\n", __FUNCTION__);
		getIntVal = 0; //Disable
	}
	g_nIsEnableScanBox = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_SOLUTIONUP", "ScanBoxDeleteIntervalDay", &db_retval);
	if(db_retval == FAIL){
		SB_SDH_ERR("%s: Fail to get DB data ScanBoxDeleteIntervalDay\n", __FUNCTION__);
		getIntVal = 0; //FileTransfer:OFF
	}
	g_nSBDeleteIntervalDay = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_SOLUTIONUP", "ScanBoxDeletedLastTime", &db_retval);
	if(db_retval == FAIL){
		SB_SDH_ERR("%s: Fail to get DB data ScanBoxDeletedLastTime\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nScanBoxDeletedLastTime = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_SOLUTIONUP", "ScanBoxDeleteHour", &db_retval);
	if(db_retval == FAIL){
		SB_SDH_ERR("%s: Fail to get DB data ScanBoxDeleteHour\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nScanBoxDeleteHour = getIntVal;

	g_nDisplayFullSDCardSC = 1;

	SB_SDH_ERR("ScanBox Enable:%d IntervalDay:%d LastTime:%d DeleteHour:%d SCDisplay:%d\n",
			g_nIsEnableScanBox, g_nSBDeleteIntervalDay, g_nScanBoxDeletedLastTime,
			g_nScanBoxDeleteHour, g_nDisplayFullSDCardSC);
}

void ScanBoxInitializeData()
{
	ScanBoxReadDB();

	g_nScanBoxCanSave = 0; //First set to 0. After checking SD card status set to 1
	g_nScanBoxDeleteIntervalTimeout = 0; //set to 0. After checking SD card set new timeout
	if(g_nIsEnableScanBox){
		if(SbGetSDCardStatus() == 0){
			g_nScanBoxCanSave = 1;
#if 0
			if(SbGetFileSystemMemUsage(SCAN_BOX_DIR_PATH) < 0){
				SB_SDH_ERR("[%s] Error: SD Card is full \n", __FUNCTION__);
				if(g_nDisplayFullSDCardSC){
					SendSCStatusToUI(SC_IMAGELOG_SDCARD_FULL, SYSTEM_MODULE_SCID);
					g_nScanBoxCanSave = 0;
				}
				else{
					if(ScanBoxDeleteFiles(0) != 0){
						g_nScanBoxCanSave = 0;
					}
				}
			}
			else{
				//Set timeout to execute next delete operation
				//TODO: Set timeout to exe
			}
#endif
			ScanBoxNextDeleteTimeout();
		}
		else{
			g_nScanBoxCanSave = 0;
		}
	}
}

void ScanBoxNotificationToSSM(int type, int option)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stSpecial_Notification stSpecialNotify = {0};

	SB_SDH_ERR("[%s] type:%d option:%d\n", __FUNCTION__, type, option);

	msgToSend.msgSender = SCAN_BOX_ID;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	msgToSend.cmd = CMD_INT_SPECIAL_NOTIFICATION;

	stSpecialNotify.type = type;
	stSpecialNotify.option = option;
	memcpy(msgToSend.data, &stSpecialNotify, sizeof(stSpecial_Notification));
	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void ScanBoxNextDeleteTimeout()
{
	//Set timeout to execute next delete operation
	UINT32 todayRemainTimeInSec;
	struct tm *timeNow;
	time_t rawTime;
	rawTime=time(NULL);
	timeNow = localtime( &rawTime );
	if(timeNow){
#if 0
		int diffTimeInSec = 0;
		UINT32 nextDeleteHourInSec = g_nScanBoxDeleteHour*SB_SECNODS_IN_HOUR;
		UINT32 lastDeletedDayHrInSec = (g_nScanBoxDeletedLastTime%SB_SECONDS_IN_A_DAY);
		UINT32 curTimeInSec = (timeNow->tm_hour*SB_SECNODS_IN_HOUR)+(timeNow->tm_min*60)+timeNow->tm_sec;
		diffTimeInSec = curTimeInSec -
				(g_nScanBoxDeletedLastTime - lastDeletedDayHrInSec + nextDeleteHourInSec);
		SB_SDH_ERR(" Cur Time:%d LastDeletedTime:%d lastDeletedDayHrInSec:%d, nextDeleteHourInSec:%d\n",
					curTimeInSec, g_nScanBoxDeletedLastTime, lastDeletedDayHrInSec, nextDeleteHourInSec);
		if(diffTimeInSec <= 0){
#endif
		int diffTimeInDays = 0;
		todayRemainTimeInSec = (timeNow->tm_hour * SB_SECNODS_IN_HOUR)
				+ (timeNow->tm_min * 60) + timeNow->tm_sec; //Today's time passed in seconds
		 //Today's time remaining in seconds
		todayRemainTimeInSec = SB_SECONDS_IN_A_DAY - todayRemainTimeInSec;
		UINT32 nextDeleteTimeInSec = todayRemainTimeInSec
				+ (g_nScanBoxDeleteHour * SB_SECNODS_IN_HOUR)
				+ SB_TIME_MARGIN_SECONDS; //add time margin
		UINT32 lastDeletedDays = (g_nScanBoxDeletedLastTime/ SB_SECONDS_IN_A_DAY);
		UINT32 curDays = rawTime / SB_SECONDS_IN_A_DAY;
		diffTimeInDays = curDays - lastDeletedDays;
		SB_SDH_ERR("Time LastDeleted:%d Time Now:%d %s",
				g_nScanBoxDeletedLastTime, rawTime, ctime(&rawTime));
		SB_SDH_ERR("Diff:%d Cur Day:%d lastDeletedDays:%d nextDeleteTimeInSec:%d\n",
				diffTimeInDays, curDays, lastDeletedDays, nextDeleteTimeInSec);
		if(diffTimeInDays > 0){
			//If today's time is passed from DeleteHour then Delete files
			if(timeNow->tm_hour >= g_nScanBoxDeleteHour){
				//Send timeout msg to ScanBox to start delete old files
				SB_SDH_ERR("Sending msg to ScanBox to delete old files now\n");
				error_type_t err;
				MESSAGE msg;
				msg.msgType = MSG_SCAN_BOX_TIMEOUT;
				msg.param1 = 0;
				msg.param2 = SCAN_BOX_TIMEOUT_POR;
				msg.param3 = NULL;
				err = SYMsgSend(SCAN_BOX_ID, &msg);
				XASSERT( err==OK, err );
			}
			else{
				g_nScanBoxDeleteIntervalTimeout =
						(g_nScanBoxDeleteHour - timeNow->tm_hour) * SB_SECNODS_IN_HOUR
						- (timeNow->tm_min * 60) + SB_TIME_MARGIN_SECONDS;
				SB_SDH_ERR("Delete files today after %d seconds\n", g_nScanBoxDeleteIntervalTimeout);
			}
		}
		else{
			g_nScanBoxDeleteIntervalTimeout = nextDeleteTimeInSec;
		}
	}
}

int SbGetSDCardStatus()
{
	int retVal = 0;
#ifdef SCAN_BOX_TEST_FOLDER
	if(access(SCAN_BOX_SD_MOUNT_PATH, F_OK)){
#else
	if(access(SCAN_BOX_SD_MOUNT_PATH, F_OK)){
#endif
		SB_SDH_ERR("[ERROR] ScanBox can't find SD Card\n");
		SendSCStatusToUI(SC_IMAGELOG_SDCARD_NOT_DETECTED, SYSTEM_MODULE_SCID);
		retVal = -1;
	}
	else{
		int ret = SbGetFileSystemType(SCAN_BOX_SD_MOUNT_PATH);
#ifdef SCAN_BOX_TEST_FOLDER
		ret = 0;
#endif
		if(ret == 0){
			if(access(SCAN_BOX_DIR_PATH, F_OK)){
				SB_SDH_ERR("[ERROR] scanbox folder NG\n");
				if(mkdir(SCAN_BOX_DIR_PATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0){
					SendSCStatusToUI(SC_IMAGELOG_SDCARD_WRITE_ERR, SYSTEM_MODULE_SCID);
					retVal  = -2;
				}
			}
			else{
				SB_SDH_ERR("scanbox folder OK\n");
			}
		}
		else{
			SendSCStatusToUI(SC_IMAGELOG_SDCARD_INVALID_FILE_SYSTEM, SYSTEM_MODULE_SCID);
			retVal  = -3;
		}
	}

	if(retVal == 0){
		g_nScanBoxSDCardError = 0;
	}
	else{
		g_nScanBoxSDCardError = 1;
	}
	return retVal;
}

/**
 * \brief Delete Scan box data
 *
 * \param option 0: Delete old interval data 1: Delete all
 *
 */
void ScanBoxDeleteFiles(const char* path, int level)
{
	SB_SDH_ERR("[%s] Path:%s Level:%d\n", __FUNCTION__, path, level);
	if(level > DELETE_LEVEL_MAX){
		SB_SDH_ERR("[%s] Can not delete files delete level reached max %d > %d \n",
				__FUNCTION__, level, DELETE_LEVEL_MAX);
		return;
	}
	int n;
	int ret;
	int pathLength;
	struct dirent **namelist;
	struct stat stFileStat;
	char *pScanBoxFilePath;

	pScanBoxFilePath = (char*)MEM_MALLOC(SEARCH_PATH_MAX);
	ASSERT(pScanBoxFilePath != NULL);

	n = scandir(path, &namelist, NULL, alphasort);
	if (n < 0){
		SB_SDH_ERR("[%s] scandir failed\n", __FUNCTION__);
		return;
	}
	else {
		SB_SDH_ERR("Total entries: %d\n", n);

		while (n--) {
			//local scope
			{
				pathLength = strlen(namelist[n]->d_name) + strlen(path) + 2;
				if (pathLength < SEARCH_PATH_MAX) {
					memset(pScanBoxFilePath, 0x00, SEARCH_PATH_MAX);
					snprintf(pScanBoxFilePath, SEARCH_PATH_MAX, "%s/%s", path, namelist[n]->d_name);
					stat(pScanBoxFilePath, &stFileStat);
					SB_SDH_ERR("%-35s  %10ld  %s\n", namelist[n]->d_name, stFileStat.st_mtime, ctime(&stFileStat.st_mtime));
					//
					//Check if is directory then recursively search
					if(S_ISDIR(stFileStat.st_mode)){
						if (!strcmp(namelist[n]->d_name, ".") || !strcmp(namelist[n]->d_name, "..")){
							//Nothing to do
						}
						else{
							//
							//Recursively call "ScanBoxDeleteFiles" with the newPath
							ScanBoxDeleteFiles(pScanBoxFilePath, level+1);
						}
					}
					else{
						//Files: Delete old scanbox files
						if(stFileStat.st_mtime < g_nScanBoxDeleteFromTime){
							ret = remove(pScanBoxFilePath);
							if(ret != 0){
								SB_SDH_ERR("[%s]: Error unable to delete file:%s\n", __FUNCTION__, pScanBoxFilePath);
							}
							else{
								SB_SDH_ERR("Deleted: %s\n", pScanBoxFilePath);
							}
						}
					}
				}
				else{
					SB_SDH_ERR("[%s] Path length has got too long\n", __FUNCTION__);
				}
			}
			free(namelist[n]);
		} //while
		free(namelist);
	} //if (n < 0){else
	//Free pScanBoxFilePath memory
	MEM_FREE_AND_NULL(pScanBoxFilePath);
	return;
}

int SbGetFileSystemType(char *path)
{
	int retVal = 0;
	struct statfs stStatFS;

	if(statfs(path, &stStatFS) != 0){
		SB_SDH_ERR("[%s] statfs failed on path:%s\n", __FUNCTION__, path);
		retVal = -2;
	}
	else{
		//f_type: type of filesystem
		if(stStatFS.f_type == FILE_SYSTEM_EXT4){
			retVal = 0;
		}
		else{
			SB_SDH_ERR("[%s] ********** Error: Invalid filesystem **********\n", __FUNCTION__);
			SB_SDH_ERR("[%s] Path:%s Detected FileSystemType:0x%X\n", __FUNCTION__, path, stStatFS.f_type);
			retVal = -1;
		}
	}
	return retVal;
}

#if 0
/**
 * \brief Scan box file system memory usage
 *
 * \param option 0: Delete SCAN_BOX_MIN_MEMSIZE*5 MB old data 1: Delete all
 *
 * \return Return memory size in MB. On error return < 0
 */
int SbGetFileSystemMemUsage(char *path)
{
	int retVal = 0;
	int curMemSizeInMB = 0;
	struct statfs stStatFS;

	if(statfs(path, &stStatFS) != 0){
		SB_SDH_ERR("[%s] statfs failed on path:%s\n", __FUNCTION__, path);
		retVal = -2;
	}
	else{
		//f_type: type of filesystem
		//f_bsize: optimal transfer block size
		//bavail: free blocks available to unprivileged user
		//f_blocks: total data blocks in filesystem
		//
		curMemSizeInMB = (stStatFS.f_bavail * (stStatFS.f_bsize >> 10))>>10;
		SB_SDH_ERR("FileSystem Path:%s MemorySize:%d\n", path, curMemSizeInMB);

		if(curMemSizeInMB >= SCAN_BOX_MIN_MEMSIZE){
			g_nScanBoxMemFull = 0;
			retVal = curMemSizeInMB;
		}
		else{
			g_nScanBoxMemFull = 1;
			SB_SDH_ERR("[%s] ********** Error: ScanBox SD Card is full **********\n", __FUNCTION__);
			SB_SDH_ERR("[%s] SD Card is full Minimun_req:%dMB Available:%dMB \n",
					__FUNCTION__, SCAN_BOX_MIN_MEMSIZE, curMemSizeInMB);
			retVal = -1;
		}
	}
	return retVal;
}
#endif


#if 0
static void ScanBoxQuickShortDateModified(struct dirent **nameList, int left, int right)
{
	register int i, j;

	struct dirent *temp;
	struct stat ibuf;
	struct stat jbuf;
	struct stat xbuf;

	i = left; j = right;

	//Get date modified info
	stat(nameList[i]->d_name, &ibuf);
	stat(nameList[j]->d_name, &jbuf);
	stat(nameList[(left+right)/2]->d_name, &xbuf);

	do {
		while((ibuf.st_mtime > xbuf.st_mtime) && (i < right)) {
			i++;
			stat(nameList[i]->d_name, &ibuf);
		}
		while((jbuf.st_mtime < xbuf.st_mtime) && (j > left))  {
			j--;
			stat(nameList[j]->d_name, &jbuf);
		}
		if(i <= j) {
			temp = nameList[i];
			nameList[i] = nameList[j];
			nameList[j] = temp;

			i++;
			j--;
			//Set name
			stat(nameList[i]->d_name, &ibuf);
			stat(nameList[j]->d_name, &jbuf);
		}

	} while(i <= j);

	if(left < j){
		ScanBoxQuickShortDateModified(nameList, left, j);
	}
	if(i < right){
		ScanBoxQuickShortDateModified(nameList, i, right);
	}
}

static void ScanBoxQuickShortMe(struct dirent **nameList, int listCount)
{
	ScanBoxQuickShortDateModified(nameList, 0, listCount-1);
}
#endif
//EOF
