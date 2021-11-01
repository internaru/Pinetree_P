/*
 ***********************************************************
 *(c) Copyright Sindoh Co. Ltd. 
 *
 *               SINDOH Confidential
 * 
 ***********************************************************
 **/
#include <stdio.h>
#include <fcntl.h>
#include <sys/prctl.h>	//for Thread Name
#include <string.h>
#include <sys/time.h>
#include "spec.h"
#include "SysServiceMgr.h"
#include "SysPosixTimer.h"
#include "SysUI_Interface.h"
#include "paper_size_config.h"
#include "agRouter.h"
#include "posix_ostools.h"
#include "logger.h"
#include "lassert.h"
#include "sys_init_api.h"
#include "SJMgr.h"
#include "agResourceMgr.h"
#include "utils.h"
#include "sjm_observers_api.h"
#include "print_events_common_api.h"
#include "../../scan/include/scanapp_observers_api.h"
#include "JobMgr.h"
#include "PrintMgr.h"
#include "cmd_proc_api.h"
#include "debug.h"
#include "ipc.h"
#include "db_api.h"
#include "file_to_pipe_api.h"
#include "NetworkIFModule.h"  		//ABBA/OAK Network Interface Module
#include "NetworkUI_Interface.h"	//ABBA/OAK Network_UI Interface
#include "solution.h"			//EMail Alert SMTP APIs
#include "pjl_extra_api.h"
#include "SysCardReaderMsg.h"
#define SYS_SHARED_MEMORY
#include "SysSharedMemory.h"
#undef SYS_SHARED_MEMORY

#ifdef HAVE_COPY_SUPPORT
#include "copyapp_api.h"
#endif
#ifdef HAVE_SCAN_SUPPORT
#include "../../../common/scan/include/asp.h"
#include "../../../common/scan/include/scantypes.h"
#include "../../../oem/shiloh/common/scan/apps/scanapp_uiscan.h"
#include "../../../common/scan/include/scanman.h"
#endif
#ifdef HAVE_IMAGELOG
#include "imagelog.h"
#endif
#include "t35_cty.h"

extern int g_nTimeoutValue;
extern bool g_bResetTimeout;
extern bool g_bPauseTimeout;
extern int g_nTimelyTimeout;
extern pthread_mutex_t gSysPosixTimerMutex;
extern bool g_bDebugESMTimer;
extern bool g_bDebugAdaptiveTimer;
extern bool g_bDebugHourlyTimer;
extern bool g_bDebugtimelyTimer;
#define TIMEOUT_PAUSE 0
#define TIMEOUT_RESET 1
#define SECONDS_IN_MINUTE_58 3480
#define SECONDS_IN_MINUTE_60 3600
int g_nEnergySaveType;	 ///<Energy Save Type 0: Normal, 1: Adaptive Energy Save
int g_nIES_WaitIdle = 1;	// Energy Save Wait Idle 1 Minute = 60 Seconds
int g_nIES_WaitBusy = 60;	// Energy Save Wait Idle 1 Hour = 360 Seconds

int g_nWrongImageTimedWait = 2;	// Wrong Image send cancel with timed wait for 2 second

extern int auth_result;
static bool g_bResetWakeupOneTime = false;
extern UINT8	fWakeKey_Onetime;
static bool g_nEnergySaveMode = false;	///< true: Power/Energy Save Mode, false: Wake up mode
static uint32_t g_nEnergySaveState = ESS_FULL_WAKE_UP;	///< 1:ESS_ENERGY_SAVE, 2:ESS_COLD_WAKE_UP, 3:ESS_FULL_WAKE_UP
static bool g_nTFTBackLightOn = false;	///< Pinetree-S TFT Backlight true: ON, false: OFF
static bool g_nPCJobWakeUp = false;	///< Pinetree-S WakeUp mode true: WakeupWith TFT backlight OFF, false: do nothing
static bool g_bOnInitSendUSBScrOnce = false;
uint32_t g_nCurrentPrintJobID = 0;			/// Current Print job ID: When System Job Manager start a new job.
uint32_t g_nCurrentScanJobID = 0;			/// Current Scan job ID: When System Job Manager start a new job.
uint32_t g_nCurrentCopyJobID = 0;			/// Current Copy job ID: When System Job Manager start a new job.
JOB_TYPES g_nCurrentJobType = 0;		/// Current job Type: When System Job Manager start a new job.
JOB_TYPES g_nCurScanJobType = 0;		/// Current Scan Job Type: When System Job Manager start a scan job.
static uint32_t g_nPrintJobCount = 0;
static uint32_t g_nLaminationJobCount = 0;
static uint32_t g_nScanJobCount = 0;
static uint32_t g_nCopyJobCount = 0;
static uint32_t g_nFaxJobCount = 0;
static uint32_t g_nFaxPrintJobCount = 0;
static uint32_t g_nFaxPrintCurJobID = 0;
static uint32_t g_nFaxScanJobCount = 0;
static uint32_t g_nFaxSendJobCount = 0;
static uint32_t g_nFaxReceiveJobCount = 0;
static bool g_bFaxDialingFlag = false;
static bool g_bFaxRingingFlag = false;

uint32_t	g_nCurrentUIScreenID = 0;		///< 0: Home, 1: Copy, 2: Scan, 3: Print, 4: FAX, 5: FileManager, 6: Print_Job_Log, 7: UP, 8: SP, 9: SDK/J
uint32_t UI_SCREEN_UP_ID  = 7;		///<UP Screen
uint32_t UI_SCREEN_SP_ID  = 8;		///<SP Screen

int g_nDefaultTray = 0;
static int gNtpEnable = 0;
static uint32_t gNtpSyncTimeHour = 0;

#define DBG_PRFX "SSM: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 1 )  // System Service Layer Mask

#define DBG_SSL_PRINTF_SDH_ERR DBG_PRINTF_SDH_ERR
#define DBG_SSL_PRINTF_SDH_DEBUG DBG_PRINTF_SDH_DEBUG


//#define SSL_MANAGE_JOBID_LIST
#ifdef SSL_MANAGE_JOBID_LIST
ATLISTENTRY gJobIDList;
typedef struct _SS_JobID
{
	uint32_t id;
	uint32_t type;
	ATLISTENTRY ListNode;
}SS_JobID;
#endif

#define SSLSTACK_SIZE POSIX_MIN_STACK_SIZE          ///< Size of the stack
#define NUM_MESSAGES 20             ///< Size of the message queue.

static ALIGN8 UINT8 SysServiceRecvStack[SSLSTACK_SIZE];      ///< Stack for the System service 
static pthread_t SysServiceRecv_thd_id;
static mqd_t SysServiceRecv_queue;
static mqd_t SysServiceSender_queue;
extern mqd_t lsu_ctrl_mq;	// To send Power saver and wake up message to ENGINE PageSupervisor.

//Shared Memory Variables
stShmAuthInfo *g_pSHM_AuthInfo = NULL;	///SHM: For Authentication Information
stShmSolutionAuth *g_pSHM_SolAuthInfo = NULL;	///SHM: For Solution Auth Info
stShmSolutionJobRes *g_pSHM_SolJobRes = NULL;	///SHM: For Solution Job Result
uint32_t *g_pSHM_PrinterStatus = NULL;	///SHM: For Printer status
uint32_t *g_pSHM_ScanStatus = NULL;	///SHM: For Scan status
uint32_t *g_pSHM_FaxStatus = NULL;	///SHM: For Fax status
uint32_t *g_pSHM_TrayStatus = NULL;	///SHM: For Available Tray
shm_FeedTrayStatus *g_pSHM_FeedTrayStatus = NULL;	///SHM: For current Tray status

extern uint32_t gDBFailCode;

//fax Variables
extern int isFaxCardReady;

static Observer_t *ObsSJMJobStatus;		///< Observer to get job status from System Job Manager. JobReceived, JobStart, JobDone
static Observer_t *ObsPrintJobMgrDeviceStatus;		///< Observer to get Print Device status from Print Job Manager like ONLINE, JOB_START, JOB_END.
//static Observer_t *ObsPrintJobMgrPageEnd;		///< Observer to get Print PAGE_DATA structure from Print Job Manager.
static Observer_t *ObsPrintMgrMediaStatus;		///< Observer to get Print medai status.
static Observer_t *ObsPrintMgrDeviceStatus;		///< Observer to get Print Manager Device Status
static Observer_t *ObsPrintMgrMediaPrompt;	///<Observer to get Print Media prompt.
static Observer_t *ObsPrintSubSystemStatus;	///<Observer to get Print Subsystem Status.

typedef struct stPrint_SystemStatus_
{
	uint32_t status; 
	TRY_AGAIN retryCallback;
	USER_CANCEL cancelCallback;
	Subject_t* retrySubject;
} stPrint_SystemStatus;
//TRY_AGAIN CbRetryPrintDeviceStatus = NULL;	///<Retry callback for Print mgr device status
stPrint_SystemStatus gStRetryPrintSystemStatus = {0};	///<Retry callback for Print mgr device status

typedef struct stPrintInform_ {
	uint32_t totalPage;
	uint32_t totalCopy;
	uint32_t totalRealPrint;
} stPrintInform;

static stPrintInform gPrintInform = {0};

#ifdef HAVE_COPY_SUPPORT
static Observer_t *ObsCopyAppEvent;	///<Observer to get Copy application's events like Copy Start, Copy Cancel, Num of Scan.
#endif

#ifdef HAVE_SCAN_SUPPORT
static Observer_t *ObsScanAppEvent;	///<Observer to get Scan application's events like Scan Start, Scan Cancel, Num of Scan, Scan End.
static Observer_t *ObsScanManEvent;	///<Observer to get Scan application's events like JAM, SC.
#endif

static uint32_t gPrevVolTray0 = 0; 	/// <MPT  Preveious volume 
static uint32_t gPrevVolTray1 = 0;	/// <Tray1  Preveious volume 
static uint32_t gPrevVolTray2 = 0;	/// <Tray2  Preveious volume 
static uint32_t gPrevVolTray3 = 0;	/// <Tray3  Preveious volume 

static uint32_t gTrayAttached = 1;	/// <Tray attached 
static uint32_t gTrayPJLCode = 0;	/// <PJL status code for Tray attached 
static bool gFrontDoorStatus = false;	/// <Front door open status false: close. true: open
static bool gSystemJamStatus = false;	/// <Jam Status true: Occur. false: clear
static bool gSystemPaperEmptyStatus = false;	/// <Paper empty status during Printing true: Occur. false: clear
static bool gSystemPaperMismatchStatus = false;	/// <Paper mismatch status during Printing true: Occur. false: clear
static bool gSystemFullStack = false;	/// <Out tray Full Stack true: Occur. false: clear
static bool gSystemSCStatus = false;	/// <Out tray Full Stack true: Occur. false: clear
static bool gSystemPrintJobStatus =false;	/// <Current Job Status false: completed. true: between job
bool gLaminationJobStatus = false;/// <Current Lamination Job Status false: completed. true: between job
static bool gLamination1PageStatus = false;/// <Current Laminating Page status false: completed. true: between lamination job
static bool gCopyWaitUserInput =false;	/// <Copy Job wait for user input false: no wait. true: Wait for user input
static bool gCopyJobFromADF =false;	/// <Copy Job from ADF false: Copy from FB. true: Copy from ADF
static bool gScanJobStatus =false;	/// <Scan Job Status false: completed. true: between job
static bool gScanJamStatus =false;	/// <Scan JAM Status false: Cleared. true: Occured
static bool gScanADFPaperPresent =false;	/// <Scan ADF Paper present false: No paper. true: Paper present
static bool gScanADFTopCoverOpen =false;	/// <Scan top Cover Open state false: Closed. true: Opened
static bool gCopyJobStatus =false;	/// <Copy Job Status false: completed. true: between job
static bool gFaxJobStatus =false;		/// <All Fax Job Status false: completed. true: between job
static bool gFaxPrintJobStatus =false;		/// <Fax Print Job Status false: completed. true: between job
static uint32_t gFaxOnHookBtnPressed = 0;		/// <Fax On Hook Button 1 = pressed, 0 = not pressed or finished
static bool gConfUpdatePending =false;	/// <Config changed by web and System busy then set true: config update pending, false:No pending
static bool gHourlyTimeUpdatePending = false;	/// <Hourly time update msg received and System busy then set true: Hourly time update pending, false:No pending
static bool gSendWaitUserInput =false;	/// <Send wait user input message to UI false: don't send. true: Send
bool gWaitForScanCalibrationDone = false;	/// < false: don't wait. true: Wait
//static bool gAutoEmailAlert =false;		/// <Auto EMail Alert in flag ON= true, OFF=false

static bool gFaxPrintJobCanceled = false;		///< true= If Fax job cancelled, false: Fax job not cancelled. Its valid only for one job.
uint32_t gFaxPrintAppPagePrinted = 0;	///< Local use
static uint32_t pagesFaxSendCounter = 0;
extern uint32_t pagesFaxAppPagePrinted;
extern DOCTYPE gFaxPrintJobDocType;

static uint32_t gPrintSystemState = 999;	/// <Print System State
static uint32_t gLaminationSystemState = 999;	/// <Print System State
static uint32_t gCopySystemState = 999;	/// <Copy System State
static uint32_t gFaxPrintSystemState = 999;	/// <Fax Print System State
static uint32_t gPlotterJamCode = 0;		/// <Plotter JAM State

bool gInitCompleted = false;	/// <Initialization of UI Manager false: Not Completed, true: Completed
bool gAuthConfSentOnInit = false;	/// <Send authentication configuration on system init
static uint32_t gTotalSysUpTimeInHour = 0;
uint32_t gSystemAuthType = AUTH_TYPE_NONE;	/// <System Authentication Type
char  gSystemAuthUserId[AUTH_BASIC_LENGTH];
bool g_bSystemAuthConfChanged = false;	/// <Whenever system's Auth config changed
uint32_t gPrintSystemStatus = 0;	/// <Current Print System Status
static uint32_t gPrintCancelReason = 0;	/// <Current Print Job Cancel reason
uint32_t gScanSystemStatus = 0;	/// <Current Scan System PJL Status code
uint32_t gFaxSystemStatus = 0;	/// <Current Fax System PJL Status code
uint32_t gTonerStatus = 99;	/// <Toner status 
uint32_t gPCUDrumStatus = DRUM_NORMAL;	/// <PCU Drum cartridge status
uint32_t gCurTonerPageCount = 0; ///Get current toner page count from PrintMgr
static uint32_t g_nAliveAlertLastExecTime = 0;
static uint32_t g_nAliveAlertSentHour = 0;
static bool g_bAliveAlertSentOnce = false;

//Image Log
#define PRINT_JOB_NAME_SIZE    81
char  g_sILFaxNumber[SDH_FAX_LEN_PHONE_NO + 1] = {0};
char  g_sILFaxTxFileName[SDH_FILENAME_MAX + 1] = {0};
char  g_sILPrintJobName[PRINT_JOB_NAME_SIZE] = {0};
char  g_sILScanFileName[SDH_FILENAME_MAX + 1] = {0};
static int g_nILFaxDestCount = 0;
static int g_nILFaxLastDestFilter = 100;
static uint32_t g_nILFaxUniqueJobID = 0;

typedef struct stIC_SysJobInfoList_{
	ATLISTENTRY infoNode;
	stIC_JobDoneInfo *pInfo;
	UINT8 destCount;
	UINT8 destFlag;
	UINT8 jobResult;
}stIC_SysJobInfoList;

static ATLISTENTRY g_ScanJobList; ///< Linked list of scan job done list
stIC_JobDoneInfo *g_pStScanJobDoneInfo = {0};
stIC_JobDoneInfo g_stFaxJobDoneInfo = {0};
extern stIC_JobDoneInfo g_faxScanInfo;
extern char gFaxLogInID[];

#define SSM_STATCAT_TONER_MASK      0x00001010
#define SSM_STATCAT_TONER_REMAIN_MASK      0x00001020
#define SSM_STATCAT_TRAY_INSTALLED_MASK      0x0000FF0F
#define SSM_STATUS_TRAY_ATTACHED_MASK      0x0000150E
#define SSM_STATUS_TRAY_NOT_ATTACHED_MASK      0x0000150F
#define MAX_ADMIN_EMAILALERT_NUM    10	///<Email Alert Max limit

static stInit_MachineConf gStMachConf = {0};	/// <Machine configuration
static stScnPltr_PlotterFeedTrayStatus gStFeedTray0Status = {0};	/// <MPT tray status
static stScnPltr_PlotterFeedTrayStatus gStFeedTray1Status = {0};	/// <Tray 1 status
static stScnPltr_PlotterFeedTrayStatus gStFeedTray2Status = {0};	/// <Tray 2 status
static stScnPltr_PlotterFeedTrayStatus gStFeedTray3Status = {0};	/// <Tray 3 status

#define PDF_PCL_FILE_PATH "/tmp/pdfpclpath"
#define JPG_PCL_FILE_PATH "/tmp/jpgpclpath"
#define TFT_FB_PATH "/proc/mv61fb"
#define TFT_TOUCH_PATH "/proc/nutouch"
#define TFT_DATA_SIZE 10

#define ISPRINTJOB(pjtype) ((pjtype==ejob_PrintIO)||(pjtype==ejob_InternalPagePrint)||(pjtype==ejob_Copy)||(pjtype==ejob_FaxPrint)||(pjtype==ejob_zjs_host_print)||(pjtype==ejob_PrintFile))
#define ISONLYPRINTJOB(pjtype) ((pjtype==ejob_PrintIO)||(pjtype==ejob_InternalPagePrint)||(pjtype==ejob_zjs_host_print)||(pjtype==ejob_PrintFile))

#define UIMGR_CREATED_MQ	///< IF UI Manager will create Message Queue

int gInitialMachineSetupProceeding = 0;
extern UINT8 isDKSolution; ///< Hana Bank solution. Send counter on Scan completed

//======================== START: FW VERSION ================
char g_sUIManagerVersion[12];
char g_sUICCVersion[12];
char g_sPS_GUIVersion[12];
char g_sPS_BrowserVersion[12];

extern char gJobName[81];

char * get_version_UI(void)
{
	return &g_sUIManagerVersion[0];
}

char * get_version_UICC(void)
{
	return &g_sUICCVersion[0];
}

char * get_version_PSGUI(void)
{
	return &g_sPS_GUIVersion[0];
}

char * get_version_PSBrowser(void)
{
	return &g_sPS_BrowserVersion[0];
}
//======================== END: FW VERSION ================

void *SysServiceReceiverThread(void *unused);
void SysServiceDebugInit();
void CheckPCUDrumStatus();
void SetTFTPanelMode(int TFTMode);
void SendTFTWakeUp();
void SendWakeUpMsgToSSM(uint32_t appID, int energyModeAction);
void SysDecideEnergySave(ENERGY_SAVE_STATE energyMode, int wakeupAction);
void SysEnergySaveMode(ENERGY_SAVE_STATE state, int wakeupAction);
bool GetEnergySaveMode();
char* GetCurerntAuthUser(void);
void AuthConfService(void);
void AuthUserActionRequest(SYS_UI_MESSAGE *Message);
void AuthSolUserActionResult(SYS_UI_MESSAGE *Message);
void AuthUserActionResult(SYS_UI_MESSAGE *Message);
void AuthParserAuthResult(SYS_UI_MESSAGE *Message);
static void AuthUserActionCountUP(stIC_JobDoneInfo *pJobDoneInfo, AUTH_COUNTUP_JOB_TYPE type, AUTH_SOL_JOB_RESULT jobResult);
static stIC_JobDoneInfo* SysManageScanJobList(int reqType, stIC_JobDoneInfo *pNewInfo, int param1);
static void SendJobDoneInfo(UINT32 sender, stIC_JobDoneInfo *pJobDoneInfo, UINT8 jobType, int jobResult);
void FaxPrintJobDoneNotify(UINT32 pagePrinted, UINT32 jobDocType, UINT32 senderID, UINT32 jobResult);
void NotifySystemJobDone(void);
void SysWakeUpForUserAttention(int wakeUpReason);
void FaxSJMResultService(SYS_UI_MESSAGE *Message);
void FaxSetPhoneNumber(int FaxType, char *FaxNumber);
void SendSpecialNotification(int type, int option);
void SendStatusForLaminationJob(uint32_t status);
void GoLaminationService(SYS_UI_MESSAGE *Message);
void GoLaminationResultService(SYS_UI_MESSAGE *Message);
extern void LaminatingSendMsgToEng(AGMSG msgType);
static void CheckInitialMachineSetup(void);
static int SetCountryCode(uint32_t);
int GetAliveEmailAlertFlag(void);
char *UrlEncode(char *str);
#ifdef HAVE_SDHFAX
extern void FaxPrintJobResult(int result_fax_print, int printed_pages);
extern int faxapp_check_on_hook_button_pressed (void);
#endif
/* Application CallBack */
static AppCallbackFun gAppCallbacksList[APPCB_CALLBACK_MAX]; ///< Application callback list
bool karasUprintSettingFlag = false;
void get_new_tz_info(struct timezone *ptz); 
uint32_t GetJobNumber(void);
static int rtc_time_check();

int GetTrayInform(TRAY_INFORMATION_REQUEST request, int trayNum) // trayNum = 0 -> MPT
{
	error_type_t err = -1;
	int ret = -1;
	switch(request) {
	case SETTING_TRAY_COUNT:
		ret = ((int)*g_pSHM_TrayStatus - 16000) / 100;
		break;
	case TRAY_STATUS:
		switch(trayNum) {
		case 0:
			ret = gStFeedTray0Status.trayStatus;
			break;
		case 1:
			ret = g_pSHM_FeedTrayStatus->tray[0].trayStatus;
			break;
		case 2:
			ret = g_pSHM_FeedTrayStatus->tray[1].trayStatus;
			break;
		case 3:
			ret = g_pSHM_FeedTrayStatus->tray[2].trayStatus;
			break;
		}
		break;
	case TRAY_PAPER_AMOUNT:
		switch(trayNum) {
		case 0:
			ret = gStFeedTray0Status.paperVolume;
			break;
		case 1:
			ret = g_pSHM_FeedTrayStatus->tray[0].paperVolume;
			break;
		case 2:
			ret = g_pSHM_FeedTrayStatus->tray[1].paperVolume;
			break;
		case 3:
			ret = g_pSHM_FeedTrayStatus->tray[2].paperVolume;
			break;
		}
		break;
	case TRAY_PAPER_SIZE:
	{
		char* trayTable[] = {
			"TrayConfig_mediaSize_0",
			"TrayConfig_mediaSize_1",
			"TrayConfig_mediaSize_2",
			"TrayConfig_mediaSize_3",
		};
		ret = db_get_var_int("CONF_PRNT", trayTable[trayNum], &err);
		if(ret == 554) { //Auto
			switch(trayNum) {
			case 0:
				ret = gStFeedTray0Status.sizeCode;
				break;
			case 1:
				ret = g_pSHM_FeedTrayStatus->tray[0].sizeCode;
				break;
			case 2:
				ret = g_pSHM_FeedTrayStatus->tray[1].sizeCode;
				break;
			case 3:
				ret = g_pSHM_FeedTrayStatus->tray[2].sizeCode;
				break;
			}
		}
	}
		break;
	}
	return ret;
}

void RegisterAppCallback(APP_CALLBACK_ACTION action, AppCallbackFun callbackfun)
{
	if (action < APPCB_CALLBACK_MAX){
		gAppCallbacksList[action] = callbackfun;
	}
}

uint32_t GetJobNumber(void)
{
	return g_nPrintJobCount;
}

void InitKarasUprintVariableSetting(void)
{
	error_type_t dbErr = FAIL;
	if(IS_KARAS && db_get_var_int("CONF_SOLUTIONSP", "PrintAuthVerify", &dbErr) == 1 && db_get_var_int("CONF_SOLUTIONSP", "CardReader", &dbErr) == 1)
		karasUprintSettingFlag = true;
}

char* ChangePaperSizeToString(UINT16 paper)
{
	char* returnString = "A4";
	switch(paper) {
	case 1:
		returnString = "Letter SEF";
		break;
	case 5:
		returnString = "Legal SEF";
		break;
	case 6:
		returnString = "Statement SEF";
		break;
	case 7:
		returnString = "Executive SEF";
		break;
	case 8:
		returnString = "A3 SEF";
		break;
	case 9:
		returnString = "A4 SEF";
		break;
	case 11:
		returnString = "A5 SEF";
		break;
	case 12:
		returnString = "B4 SEF";
		break;
	case 13:
		returnString = "B5 SEF";
		break;
	case 17:
		returnString = "11X17 SEF";
		break;
	case 19:
		returnString = "Com_9_Env";
		break;
	case 20:
		returnString = "Com10 Envelope";
		break;
	case 27:
		returnString = "DL Envelope";
		break;
	case 28:
		returnString = "C5 Envelope";
		break;
	case 30:
		returnString = "C4 Envelope";
		break;
	case 31:
		returnString = "C6 Envelope";
		break;
	case 34:
		returnString = "B5 Envelope";
		break;
	case 37:
		returnString = "Monarch Envelope";
		break;
	case 43:
		returnString = "Card_100X148";
		break;
	case 69:
		returnString = "Card_200X148";
		break;
	case 70:
		returnString = "A6 SEF";
		break;
	case 75:
		returnString = "LETTER LEF";
		break;
	case 76:
		returnString = "A3 SEF";
		break;
	case 77:
		returnString = "A4 LEF";
		break;
	case 78:
		returnString = "A5 LEF";
		break;
	case 79:
		returnString = "B4 LEF";
		break;
	case 80:
		returnString = "B5 LEF";
		break;
	case 82:
		returnString = "Card 148X200";
		break;
	case 88:
		returnString = "B6 SEF";
		break;
	case 89:
		returnString = "B6 LEF";
		break;
	case 256:
		returnString = "Custom_Size";
		break;
	case 258:
		returnString = "US_Folio";
		break;
	case 264:
		returnString = "16K SEF";
		break;
	case 268:
		returnString = "4\"X6\"";
		break;
	case 298:
		returnString = "8K SEF";
		break;
	case 303:
		returnString = "16K SEF";
		break;
	case 311:
		returnString = "EXECUTIVE LEF";
		break;
	case 317:
		returnString = "STATEMENT LEF";
		break;
	case 321:
		returnString = "8X13 SEF";
		break;
	case 328:
		returnString = "Y3 Envelope";
		break;
	case 329:
		returnString = "Y4 Envelope";
		break;
	case 330:
		returnString = "L3 Envelope";
		break;
	case 331:
		returnString = "L4 Envelope";
		break;
	case 332:
		returnString = "K1 Envelope";
		break;
	case 333:
		returnString = "K2 Envelope";
		break;
	case 334:
		returnString = "K3 Envelope";
		break;
	case 359:
		returnString = "Quatro";
		break;
	case 360:
		returnString = "G_Legal";
		break;
	case 361:
		returnString = "G_Letter";
		break;
	case 554:
		returnString = "Auto";
		break;
	default:
		returnString = "A4";
		break;
	}
	return returnString;
}

/**
 * \brief Get Fax Phone Number
 *
 * \param FaxJobType int 1: Fax Transfer 2: Fax Receive
 * \param FaxNumber char Pointer to Fax Number
 *
 */
void FaxSetPhoneNumber(int FaxJobType, char *FaxNumber)
{
	memset(g_sILFaxNumber, 0x00, SDH_FAX_LEN_PHONE_NO);
	snprintf(g_sILFaxNumber, strlen(FaxNumber)+1, "%s", FaxNumber);
	//DBG_SSL_PRINTF_SDH_ERR("[%s]: set FaxNo:%s strlen:%d\n", __FUNCTION__, g_sILFaxNumber, strlen(g_sILFaxNumber));
}

/**
 * \brief Set FaxTx Job log for Solution
 *
 * \param Fax FileName char Pointer to File Name
 * \param jobID Unique job ID
 * \param destCount 0: not last job val: Last job with destination count
 *
 */
void FaxSetJobLog(char *FileName, UINT32 jobID, int destCount)
{
	memset(g_sILFaxTxFileName, 0x00, SDH_FILENAME_MAX);
	snprintf(g_sILFaxTxFileName, strlen(FileName)+1, "%s", FileName);
	g_nILFaxDestCount = destCount;
	g_nILFaxUniqueJobID = jobID;
//	DBG_SSL_PRINTF_SDH_ERR("[%s]: set FileName:%s UniqueJobID:%d DestCount:%d\n", __FUNCTION__, g_sILFaxTxFileName, jobID, destCount);
}

void ScanSetJobFileName(char *FileName)
{
	memset(g_sILScanFileName, 0x00, SDH_FILENAME_MAX+1);
	snprintf(g_sILScanFileName, strlen(FileName)+1, "%s", FileName);
	DBG_SSL_PRINTF_SDH_ERR("[%s]: set FileName:%s\n", __FUNCTION__, g_sILScanFileName);
}

char toHex(char code) {
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

char *UrlEncode(char *str)
{
	char *encstr, buf[2+1];
	unsigned char c;
	int i, j;

	if(str == NULL) return NULL;
	if((encstr = (char *)malloc((strlen(str) * 3) + 1)) == NULL) 
		return NULL;

	for(i = j = 0; str[i]; i++) 
	{
		c = (unsigned char)str[i];
		if((c >= '0') && (c <= '9')) encstr[j++] = c;
		else if((c >= 'A') && (c <= 'Z')) encstr[j++] = c;
		else if((c >= 'a') && (c <= 'z')) encstr[j++] = c;
		else if((c == '@') || (c == '.') || (c == '/') || (c == '\\')
				|| (c == '-') || (c == '_') || (c == ':') ) 
				encstr[j++] = c;
		else 
		{
			sprintf(buf, "%02x", c);
			encstr[j++] = '%';
			encstr[j++] = buf[0];
			encstr[j++] = buf[1];
		}
	}
	encstr[j] = '\0';

	return encstr;
}

bool CheckUSBMounted()
{
	FILE *read_fp;
	char buffer[10];
	int readSize=0;
	bool usbMounted = false;

	memset(buffer, '\0', sizeof(buffer));	
	read_fp = popen("cat /proc/mounts | grep /mnt/sda1 | awk '{print $2}' | tr -d '\n'", "r");

	if(read_fp != NULL) {
		DBG_SSL_PRINTF_SDH_ERR("read_fp != NULL\n");
		readSize = fread(buffer, sizeof(char), 10-1, read_fp);
		if(readSize > 0) {
			usbMounted = true; 	//sprintf("%s",buffer);
			DBG_SSL_PRINTF_SDH_ERR("USB Mounted:%d \n", usbMounted);
		}
		pclose(read_fp);
	}
	return usbMounted;
}

/**
 * \brief Send Wake-up message to SSM. 
 * User should wait for 2 second after calling this function.
 *
 * \param appID Application IDs defines in agRouter.h as MODULE_ID.
 *
 */
void SendWakeUpMsgToSSM(uint32_t appID, int modeAction)
{
	SYS_UI_MESSAGE msg;
	error_type_t err;
	stWnd_EnergyMode stEnergyMode = {0}; 
	msg.msgSender = appID;
	msg.cmd = CMD_INT_ENERGY_MODE;
	memset(msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stEnergyMode.mode = ESM_WAKE_UP;
	stEnergyMode.action = modeAction;
	memcpy(msg.data, &stEnergyMode, sizeof(stWnd_EnergyMode));
	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
	XASSERT( err==OK, err ); 
}

/**
 * \brief Return current status of Energy/Power Save Mode .
 *
 * \return bool true: System is in Energy-save mode, false: System is in Wake-up Mode.
 *
 */
bool GetEnergySaveMode()
{
	if(g_nEnergySaveMode){
		return true;
	}
	else{
		return false;
	}
}

int get_current_day(void)
{
	time_t nowSec;
	nowSec=time(NULL);
	nowSec=nowSec/(60*60*24); //Sec->day

	//DBG_SSL_PRINTF_SDH_ERR("AliveAlert %s: day%d", __func__, nowSec);
	return nowSec;
}

void AliveEmailAlertControl(int type)
{
	int curr_day = 0;
	curr_day = get_current_day();
	DBG_SSL_PRINTF_SDH_ERR("AliveAlert last: %d cur:%d \n", g_nAliveAlertLastExecTime, curr_day );
	if ( g_nAliveAlertLastExecTime != curr_day || type ) {
		g_nAliveAlertLastExecTime = curr_day;
		db_set_var_int("CONF_MAILALERTTYPE", "AlertCounterSentTime", g_nAliveAlertLastExecTime);
		ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_ALIVE_SEND, gCurTonerPageCount);
	}
}

//IMPORANT for CallBacks Function: 
//Processing in the notification callbacks needs to be minimal since this is done on the subject's thread.

//NOTE: The observer's ID does not need to match the corresponding subject's ID

/**
 * \brief Callback get job status from System Job Manager. 
 * For a JobId with JobType status like JobReceived, JobStart, JobDone.
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
static void SS_SJMJobStatusCB(Observer_t *obs, Subject_t *sub)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Called.  \n", __FUNCTION__);

	sjm_jobstate_enum_t JobState;
	JOB_TYPES JobType;
	JOB_TYPES nextJobType = ejob_NULL;
	uint32_t JobId;
	error_type_t err;
	SYS_UI_MESSAGE msg;
	bool msgSendFlag = true;
	stPrint_GoPrintResult printResult = {0};
	stScan_GoScanResult scanResult = {0};
	stCopy_GoCopyResult copyResult = {0};
	stIC_FaxJobResult faxResult = {0};
	stWnd_EnergyMode stEnergyMode = {0}; 

	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, 
							   subject_observer_oid_key, SJM_JobStatus_ID));

	// Keep processing to a minimum, this is called on the Subject's thread
	JobState = SJM_Get_JobStatus( sub);
	JobType = SJM_Get_JobType(sub);
	JobId = SJM_Get_JobId(sub);
	
	DBG_SSL_PRINTF_SDH_ERR("SJMCB: Job ID=%d Type=%s State=%s\n", JobId, to_string_job_types(JobType), to_string_sjm_jobstate_enum(JobState));

	msg.msgSender = SJMID;
	memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	switch(JobState)
	{
		case esjm_JobReceived:
			//Check for Energy/Power Save mode 
			if( (JobType == ejob_PrintIO ) ||(JobType == ejob_zjs_host_print)  ||(JobType == ejob_InternalPagePrint) || (JobType == ejob_InternalPageHttp ) ||(JobType == ejob_PJL ) || (JobType == ejob_ACL)
				|| (JobType == ejob_Fax) ||(JobType == ejob_FaxPrint) ||(JobType == ejob_ScanToHost) || (JobType == ejob_CopyToHost) || (JobType == ejob_PrintFile)) {
				
				stEnergyMode.mode = ESM_WAKE_UP; 	// WAKE-UP msg
				if(JobType == ejob_ScanToHost){
					stEnergyMode.action = EMA_SCAN_TO_HOST;
				}
				else{
					stEnergyMode.action = EMA_RECEIVED_PRINT;
				}
				msg.cmd = CMD_INT_ENERGY_MODE;
				memcpy(msg.data, &stEnergyMode, sizeof(stWnd_EnergyMode));

				err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
				XASSERT( err==OK, err );
			}
			break;
		case esjm_JobStart:
			if( (JobType == ejob_PrintIO ) || (JobType == ejob_zjs_host_print ) || (JobType == ejob_InternalPagePrint ) || (JobType == ejob_PrintFile )){
#ifdef SSL_MANAGE_JOBID_LIST
				SS_JobID *pJobData = (SS_JobID *) MEM_MALLOC(sizeof(SS_JobID));
				pJobData->id = JobId;
				pJobData->type= JobType;				
				ATInsertTailList(&gJobIDList, &pJobData->ListNode);

				ATLISTENTRY* pNode;
				pNode = ATListHead(&gJobIDList);
				SS_JobID *pIDData;
				pIDData = CONTAINING_RECORD(pNode, SS_JobID, ListNode);
				if(pIDData){					
					g_nCurrentPrintJobID = pIDData->id;
					g_nCurrentJobType = pIDData->type;
				}
				int totalJob = ATNumListNodes(&gJobIDList);
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: JobState = esjm_JobStart, JobId= %d, Type= %s, TotalJobInQueue=%d  \n", __FUNCTION__, g_nCurrentPrintJobID, to_string_job_types(JobType), totalJob);
#else
				//g_nCurrentPrintJobID = JobId;
				g_nCurrentJobType = JobType;
#endif
			}
			else if( (JobType == ejob_Copy) ||(JobType == ejob_CopyToHost)){
				g_nCurrentCopyJobID = JobId;
				g_nCurrentJobType = JobType;
			}
			else if( (JobType == ejob_ScanToEmail ) || (JobType == ejob_ScanToHost ) || (JobType == ejob_ScanToMemCard )){
				g_nCurrentScanJobID = JobId;
				g_nCurrentJobType = JobType;
				g_nCurScanJobType = JobType;
			}
			break;
		case esjm_JobDone:
			if( (JobType == ejob_PrintIO ) || (JobType == ejob_zjs_host_print ) || (JobType == ejob_InternalPagePrint ) || (JobType == ejob_PrintFile )){
				nextJobType = SJMGetCurJobType();
#ifdef SSL_MANAGE_JOBID_LIST				
				ATLISTENTRY* pNode = ATRemoveHeadList(&gJobIDList);
				if(pNode){
					SS_JobID *pIDData = CONTAINING_RECORD(pNode, SS_JobID, ListNode);
					if(pIDData){
						g_nCurrentPrintJobID = pIDData->id;
						g_nCurrentJobType = pIDData->type;
					}
					else{
						g_nCurrentPrintJobID = 0;
						g_nCurrentJobType = ejob_NULL;
					}
					memFree(pIDData);
				}
				int totalJob = ATNumListNodes(&gJobIDList);
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: JobState = esjm_JobDone, JobId= %d, Type=%s, TotalJobInQueue=%d  \n", __FUNCTION__, g_nCurrentPrintJobID, to_string_job_types(JobType), totalJob);
#else
				//g_nCurrentPrintJobID = 0;
				g_nCurrentJobType = ejob_NULL;
#endif
			}
			else if( (JobType == ejob_Copy) ||(JobType == ejob_CopyToHost)){
				g_nCurrentCopyJobID = 0;
				g_nCurrentJobType = ejob_NULL;
			}
			else if( (JobType == ejob_ScanToEmail ) || (JobType == ejob_ScanToHost ) || (JobType == ejob_ScanToMemCard )){
				g_nCurrentScanJobID = 0;
				g_nCurrentJobType = ejob_NULL;
				g_nCurScanJobType = ejob_NULL;
			}			
			break;
		case esjm_JobCancel_at_device:
			break;
		case esjm_JobCancel_by_user:
			break;
		case esjm_JobCancelDone:
			break;
		default:			
			break;
	}

	//Reset msg data
	memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	switch(JobType)
	{	
		//Print Jobs
		case ejob_PrintIO:
		case ejob_zjs_host_print:
		case ejob_InternalPagePrint:
		case ejob_PrintFile:
			printResult.jobID = JobId;
			printResult.result = JobState;
			printResult.page = nextJobType;
			
			msg.cmd = CMD_INT_PRINT_GOPRINT_RES;
			memcpy(msg.data, &printResult, sizeof(stPrint_GoPrintResult));	
			break;
		//case ejob_ScanCal:
		//case ejob_HttpScan:
		case ejob_ScanToEmail:
		case ejob_ScanToHost:
		case ejob_ScanToMemCard:	
			scanResult.result = JobState;
			scanResult.page = JobType;
			
			msg.cmd = CMD_INT_SCAN_GOSCAN_RES;
			memcpy(msg.data, &scanResult, sizeof(stScan_GoScanResult));
			break;
		case ejob_Copy:
		case ejob_CopyToHost:
			copyResult.jobID = JobId;
			copyResult.result = JobState;
			
			msg.cmd = CMD_INT_COPY_GOCOPY_RES;
			memcpy(msg.data, &copyResult, sizeof(stCopy_GoCopyResult));
			break;
		case ejob_ScanFax:
		case ejob_Fax:
		case ejob_FaxPrint:
			faxResult.jobType = JobType;
			faxResult.jobID = JobId;
			faxResult.result = JobState;
			
			msg.cmd = CMD_INT_FAX_SJM_RES;
			memcpy(msg.data, &faxResult, sizeof(stIC_FaxJobResult));
			break;
		default:
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){		
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
		XASSERT( err==OK, err );	
	}
	
}

/**
 * \brief Callback get Print device status from Print Job Manager. 
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
static void SS_PJMDeviceStatusCB(Observer_t *obs, Subject_t *sub)
{
	uint32_t printDeviceStatus;
	SYS_UI_MESSAGE msg;
	error_type_t err;
	bool msgSendFlag = true;

	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, 
							   subject_observer_oid_key, JobMgr_myDeviceStatus_ID));

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);

	// Keep processing to a minimum, this is called on the Subject's thread
	printDeviceStatus = JobMgr_Get_devicestatus( sub);

	msg.msgSender = JOBMGRID;
	msg.cmd = CMD_INT_PRINT_GOPRINT_RES;
	memset(msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);			

	stPrint_GoPrintResult goPrintResult = {0};
	goPrintResult.jobID = JobMgr_Get_resourceJobID(sub);
	goPrintResult.result = printDeviceStatus;	
	memcpy(msg.data, &goPrintResult, sizeof(stPrint_GoPrintResult));				

	if(msgSendFlag)
	{
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);				
		XASSERT( err==OK, err );
	}
}

/**
 * \brief Callback: Get Print JOB_DATA and PAGE_DATA structure
 * from Print Job Manager after page end.
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
#if 0
static void SS_PJMPageEndCB(Observer_t *obs, Subject_t *sub)
{
	DBG_SSL_PRINTF_SDH_DEBUG("  [SSL] CallBack -> %s Called.  \n", __FUNCTION__);
	JOB_DATA *JobDataStruct;
	PAGE_DATA *PageDataStruct;

	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, 
							   subject_observer_oid_key, PRINT_JOBMGR_GLOBAL_PAGE_END_ID));

	// Keep processing to a minimum, this is called on the Subject's thread
	JobDataStruct = print_jobmgr_global_page_end_observer_get_job_data( sub );
	PageDataStruct = print_jobmgr_global_page_end_observer_get_page_data( sub );

//	DBG_SSL_PRINTF_SDH_DEBUG("  ################## :> %s :  Print Job Page Count = %d \n",
						__FUNCTION__, JobDataStruct->PAGECOUNT);
	
	///TODO: Prepare Message data and Call SendMessageToUI function

	///TODO: Temp Message Remove from here
	
	MESSAGE msg;
	error_type_t err;
	bool msgSendFlag = false;
		
	msg.msgType = MSG_STATE_PRINTJOB;
	msg.param1 = 0;		//Printer Ready after print Page end
	msg.param2 = 0; 	//no use
	msg.param3 = NULL; 	//no use
	msgSendFlag = true;
	
	if(msgSendFlag)
	{
		err = SYMsgSend(SYSTEM_SERVICE_SENDER_ID, &msg);         
		XASSERT( err==OK, err );
	}

}

#endif	
/**
 * \brief Callback: Get Print Media status.
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
static void SS_PMMediaStatusCB(Observer_t *obs, Subject_t *sub)
{
	uint32_t CurStatus; 
	error_type_t err;
	SYS_UI_MESSAGE msg;
	bool msgSendFlag = true;

	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, subject_observer_oid_key, PrintMgr_myMediaStatus_ID));
	// Keep processing to a minimum, this is called on the Subject's thread
	CurStatus = PrintMgr_Get_MediaStatus(sub);
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X  \n", __FUNCTION__, CurStatus);
	
	msg.msgSender = PRINTMGRID;	
	memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);	
	memcpy(msg.data, &CurStatus, sizeof(uint32_t));

	if( ( (CurStatus & SSM_STATCAT_TRAY_INSTALLED_MASK) == SSM_STATUS_TRAY_ATTACHED_MASK)
		|| ( (CurStatus & SSM_STATCAT_TRAY_INSTALLED_MASK) == SSM_STATUS_TRAY_NOT_ATTACHED_MASK) ){
			DBG_SSL_PRINTF_SDH_DEBUG("[%s] Media Attach Status 0x%X\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_MACHINE_CONF;
	}
	else{
		DBG_SSL_PRINTF_SDH_DEBUG("[%s] Media Status 0x%X\n", __FUNCTION__, CurStatus);
		msg.cmd = CMD_INT_PLOTTER_TRAY_STATUS;
	}

	if(msgSendFlag){
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
		XASSERT( err==OK, err );
	}	
}


/**
 * \brief Callback when a Print Manager device status is updated.
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
static void SS_PMDeviceStatusCB(Observer_t *obs, Subject_t *sub)
{
	uint32_t CurStatus;	
	error_type_t err;
	SYS_UI_MESSAGE msg;
	bool msgSendFlag = true;

	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, 
							   subject_observer_oid_key, PrintMgr_myDeviceStatus_ID));
	// Keep processing to a minimum, this is called on the Subject's thread
	CurStatus = PrintMgr_Get_devicestatus(sub);

	if( (CurStatus == STATUS_INPUT_TRAY_SIZE_MISMATCH) ||( CurStatus == STATUS_DRUM_LOW_MODE_3 ) ){
		gStRetryPrintSystemStatus.status = CurStatus;
		gStRetryPrintSystemStatus.retryCallback = PrintMgr_Get_retry_callback(sub);
		gStRetryPrintSystemStatus.cancelCallback = PrintMgr_Get_cancel_button_callback(sub);
		gStRetryPrintSystemStatus.retrySubject = sub;
	}
   	
	//For PINETREE-L
	if(IS_PINETREEL && PINETREE_L){
		if(((CurStatus & STATUS_PAPER_OUT) == STATUS_PAPER_OUT) && ISEMPTYFINAL(CurStatus)){
			gStRetryPrintSystemStatus.status = CurStatus;
			gStRetryPrintSystemStatus.retryCallback = PrintMgr_Get_retry_callback(sub);
			gStRetryPrintSystemStatus.cancelCallback = PrintMgr_Get_cancel_button_callback(sub);
			gStRetryPrintSystemStatus.retrySubject = sub;
		}
	}

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X \n", __FUNCTION__, CurStatus);
	
	msg.msgSender = PRINTMGRID;	
	msg.cmd = 0;
	memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	memcpy(msg.data, &CurStatus, sizeof(uint32_t));

	switch ( CurStatus & STATUS_CATEGORY_MASK) {
		
		case STATCAT_PAPERJAM_TYPE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X  STATCAT_PAPERJAM_TYPE\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PLOTTER_JAM_STATUS;	//Plotter jam status
			break;
		case STATCAT_PRINTHEAD:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_PRINTHEAD\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PLOTTER_SC_STATUS;	//SC status;	
			break;
		case STATCAT_PAPEROUT:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_PAPEROUT\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PLOTTER_TRAY_STATUS;	//Plotter feed tray status
			break;
		case STATCAT_TRAY:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_TRAY\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_MACHINE_CONF;	//Machine configuration
			break;
		case STATCAT_INFO:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_INFO\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PLOTTER_MODE_STATUS;	//Status info: Plotter mode status
			break;
		case STATCAT_ONLINE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_ONLINE\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PLOTTER_MODE_STATUS;	//Status info: Plotter mode status
			break;				
		case STATCAT_DOOROPEN:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_DOOROPEN\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PLOTTER_DOOR_STATUS;	//Door Open: Plotter Door open status
			break;
		case STATCAT_CARTRIDGE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_CARTRIDGE\n", __FUNCTION__, CurStatus);
			if(( ( CurStatus & SSM_STATCAT_TONER_MASK ) == SSM_STATCAT_TONER_MASK ) || 
				( ( CurStatus & SSM_STATCAT_TONER_REMAIN_MASK ) == SSM_STATCAT_TONER_REMAIN_MASK )){
				msg.cmd = CMD_INT_PLOTTER_TONER_STATUS;	//Toner status
			}else{
				msg.cmd = CMD_INT_PCU_STATUS;	//PCU Drum status
			}
			break;
		case STATCAT_INPUT:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_INPUT\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PRINT_PAPER_MISMATCH;
			break;
		case STATCAT_WRONGIMAGE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_WRONGIMAGE\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_WRONG_IMAGE_SIZE;
			break;
		case STATCAT_LAMINATEINFO:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X STATCAT_LAMINATEINFO\n", __FUNCTION__, CurStatus);
			msg.cmd = CMD_INT_PRINT_GOLAMINATION_RES;
			break;
		default:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Switch(0x%X )  Status = 0x%X NOT FOUND \n", __FUNCTION__, (CurStatus & STATUS_CATEGORY_MASK), CurStatus);
			msgSendFlag = false;
			break;
	}
	
	if(msgSendFlag)
	{		
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);        		
		XASSERT( err==OK, err );
	}
}


/**
 * \brief Callback for Media prompt like input tray, media type, media size, output tray
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
 void SS_PMMediaPromptCB(Observer_t *obs, Subject_t *sub)
{
//	DBG_SSL_PRINTF_SDH_DEBUG("   [SSL] CallBack ->  %s Called.  \n", __FUNCTION__);
#if 0
	uint32_t MediaStatus;
	printmgr_media_prompt_info_t *pMediaInfo;
	pMediaInfo = (printmgr_media_prompt_info_t *) MEM_MALLOC(sizeof(printmgr_media_prompt_info_t));

	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, 
							   subject_observer_oid_key, PrintMgr_myMediaPromptID));
	
	// Keep processing to a minimum, this is called on the Subject's thread
	MediaStatus = printmgr_media_prompt_get_status_event(sub);		
	printmgr_media_prompt_get_media_info(sub, pMediaInfo);
//	DBG_SSL_PRINTF_SDH_DEBUG("   [SSL] CallBack -:> %s :  MediaSTATUS = %d, InputTray = %d, MediaType = %d, MediaSize = %d, OutputTray = %d \n",
				__FUNCTION__, MediaStatus, pMediaInfo->prompt_input_tray, pMediaInfo->prompt_mediatype, pMediaInfo->prompt_mediasize, pMediaInfo->prompt_output_tray);
	
	//Prepare Message data and send message to UI

	memFree(pMediaInfo);

#endif	
}



/**
 * \brief Callback for Print Sub System Status related to Print module.
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
 void SS_PrintSubSystemCB(Observer_t *obs, Subject_t *sub)
{
	SYS_UI_MESSAGE msg;
	error_type_t err;	
	stIC_PrintSubSystem stPrinterStatus = {0};

	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, 
							   subject_observer_oid_key, PrintJobMgr_PrintStatus_ID));
	
	// Keep processing to a minimum, this is called on the Subject's thread
	//status = pec_get_current_printstatus();	
	stPrinterStatus.printStatus = pec_get_printstatus(sub);
	stPrinterStatus.pageStatus= pec_get_printpagestatus(sub);
	stPrinterStatus.jobStatus = pec_get_printjobstatus(sub);
	stPrinterStatus.resourceJobType= pec_get_resourceJobType(sub);
	stPrinterStatus.resourceJobID = pec_get_resourceJobID(sub);

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: size = %d, LEN = %d\n", __FUNCTION__, sizeof(SYS_UI_MESSAGE), SYS_UI_MESSAGE_DATA_LEN);

	memset(&msg, 0, sizeof(SYS_UI_MESSAGE));

	msg.msgSender = PRINTMGRID; 
	msg.cmd = CMD_INT_PRINT_SUB_SYSTEM;
	memcpy(msg.data, &stPrinterStatus, sizeof(stIC_PrintSubSystem));

	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
	XASSERT( err==OK, err );
}
	

/**
 * \brief Callback to get Event from Copy Application.
 * Like Application Idle, Start, Running, Complete, Cancel, Out of Memory.
 *
 * \param obs An event reporting API Observer_t pointer, allocated by the caller.
 * \param sub The Subject_t pointer passed use for event callback.
 *
 */
 void SS_CopyAppEventCB(Observer_t *obs, Subject_t *sub)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Called.  \n", __FUNCTION__);
#ifdef HAVE_COPY_SUPPORT
	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t, 
							   subject_observer_oid_key, COPYAPP_OBSERVER_ID));	

	copyapp_event_t CpAppEvent;
	SYS_UI_MESSAGE msg;
	error_type_t err;
	CpAppEvent = copyapp_observer_get_event(sub);		
	
	memset(&msg, 0, sizeof(SYS_UI_MESSAGE));
	msg.msgSender = COPYAPPID; 
	msg.cmd = CMD_INT_COPY_APP_EVENT;
	memcpy(msg.data, &CpAppEvent, sizeof(copyapp_event_t));

	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
	XASSERT( err==OK, err );	
#endif	
}

 /**
  * \brief Callback to get Event from Scan Application.
  * Like Application Idle, Start, Running, Complete, Cancel, Out of Memory.
  *
  * \param obs An event reporting API Observer_t pointer, allocated by the caller.
  * \param sub The Subject_t pointer passed use for event callback.
  *
  */
  void SS_ScanAppEventCB(Observer_t *obs, Subject_t *sub)
 {
	 DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Called.  \n", __FUNCTION__);
 #ifdef HAVE_SCAN_SUPPORT
	 ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t,
								subject_observer_oid_key, SCANAPP_OBSERVER_ID));

	 scanapp_event_t ScanAppEvent;
	 SYS_UI_MESSAGE msg;
	 error_type_t err;
	 ScanAppEvent = scanapp_observer_get_event(sub);

	 memset(&msg, 0, sizeof(SYS_UI_MESSAGE));
	 msg.msgSender = SCANPARSEID;
	 msg.cmd = CMD_INT_SCAN_GOSCAN_RES;
	 memcpy(msg.data, &ScanAppEvent, sizeof(scanapp_event_t));

	 err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
	 XASSERT( err==OK, err );
 #endif
 }

/**
  * \brief Callback to get Event from Scanner current Status like JAM or SC.
  *
  * \param obs An event reporting API Observer_t pointer, allocated by the caller.
  * \param sub The Subject_t pointer passed use for event callback.
  *
  */
  void SS_ScannerStatusEventCB(Observer_t *obs, Subject_t *sub)
{
	 DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Called.  \n", __FUNCTION__);
#ifdef HAVE_SCAN_SUPPORT
	ASSERT(VALIDATE_OBJECT_KEY(obs, Observer_t,
							   subject_observer_oid_key, SCANMAN_OBSERVER_ID));

	scanman_event_t scanmanEvent;
	uint32_t scanmanEventValue;
	stIC_ScanmanEventStatus stScanEventStatus = {0};	
	
	SYS_UI_MESSAGE msg;
	error_type_t err;
	scanman_observer_get_last_event( &scanmanEvent, &scanmanEventValue );

	stScanEventStatus.eventReason = scanmanEvent;
	stScanEventStatus.eventStatus = scanmanEventValue;

	memset(&msg, 0, sizeof(SYS_UI_MESSAGE));
	msg.msgSender = SCANMANID;
	msg.cmd = CMD_INT_SCANNER_JAM_STATUS;
	memcpy(msg.data, &stScanEventStatus, sizeof(stIC_ScanmanEventStatus));

	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
	XASSERT( err==OK, err );
 #endif
}


void SPChangedService(SYS_UI_MESSAGE* Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	error_type_t err;
	MESSAGE msg;
	SYS_UI_MESSAGE msgToSend;
	stInit_ConfChanged stConfChanged = {0};	
	bool updateDataNow = true;
	gConfUpdatePending = false; //Clear Update Pending
	
	DBG_SSL_PRINTF_SDH_ERR("[%s] MSG_CONFIG_CHANGED Sender:%d\n", __FUNCTION__, Message->msgSender);

	if(Message->msgSender == WEB_MONITOR_ID){
		if(gSystemPrintJobStatus || gScanJobStatus ||gCopyJobStatus || gFaxJobStatus){
			//Job in progress. Update later
			gConfUpdatePending = true;
			updateDataNow = false;
		}
		else{
			//Send Conf Changed message to UI Manager. So UI Manager can update with DB.
			msgToSend.msgSender = SYSTEM_SERVICE_ID;
			memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
			stConfChanged.mode = 0;	//Conf All
			msgToSend.cmd = CMD_SU_CONFIG_CHANGED;
			memcpy(msgToSend.data, &stConfChanged, sizeof(stInit_ConfChanged));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );
		}
	}
	
	if(updateDataNow){
		msg.msgType = MSG_CONFIG_CHANGED;
		msg.param1 = 0;
		msg.param2 = 0;
		msg.param3 = NULL;
		err = SYMsgSend(JOBMGRID, &msg);
		XASSERT( err==OK, err );
#ifdef HAVE_IMAGELOG
		msg.msgType = MSG_CONFIG_CHANGED;
		msg.param1 = 0;
		msg.param2 = 0;
		msg.param3 = NULL;
		err = SYMsgSend(IMAGE_LOG_ID, &msg);
		XASSERT( err==OK, err );
#endif
#ifdef HAVE_SCANBOX
		msg.msgType = MSG_CONFIG_CHANGED;
		msg.param1 = 0;
		msg.param2 = 0;
		msg.param3 = NULL;
		err = SYMsgSend(SCAN_BOX_ID, &msg);
		XASSERT( err==OK, err );
#endif
		//Config Changed: Update energy save Timeout value
		UpdateEnergySaveTimeout();

		//Check if Auth Config modified
		AuthConfService();

		//update default tray 
		g_nDefaultTray = db_get_var_int("CONF_COPYSET", "PrintSrcTray", &err);
		if(err == FAIL || g_nDefaultTray < TRAY_MPT || g_nDefaultTray >= TRAY_MAX ){
			g_nDefaultTray = TRAY1;
		}

		//NTP Time Synchronization
		int DbValue;
		DbValue=db_get_var_int("CONF_SYSTEM", "NtpEnable", &err);
		if ( err == FAIL ) {
			DbValue = 1;
		}
		if ( gNtpEnable==0 && DbValue!=0 ) {
			gNtpSyncTimeHour = gTotalSysUpTimeInHour;
			system("/app/ntpd.sh &");
		}
		gNtpEnable = DbValue;

		//USB Stick
		char str[64];
		static int gBlockUSBStick = -1;
		DbValue = db_get_var_int("CONF_SYSTEMSP", "BlockUSBStick", &err);
		if ( err == FAIL ) {
			DBG_SSL_PRINTF_SDH_ERR("%s DB Read fail\n", __func__);
		}
		else {
			if ( gBlockUSBStick == -1 ) {
				gBlockUSBStick = DbValue;
			}
			else {
				if ( gBlockUSBStick != DbValue ) {
					gBlockUSBStick = DbValue;
					memset(str, 0, sizeof(str));
					sprintf(str, "/app/usb_stick_power.sh %s", (gBlockUSBStick==1)?"block":"enable");
					system(str);
				}
			}
		}
		//USB Wifi 
		static int ghaveWLan = -1;
		DbValue = db_get_var_int("CONF_SYSTEMSP", "haveWLan", &err);
		if ( err == FAIL ) {
			DBG_SSL_PRINTF_SDH_ERR("%s DB Read fail\n", __func__);
		}
		else {
			if ( ghaveWLan == -1 ) {
				ghaveWLan = DbValue;
			}
			else {
				if ( ghaveWLan != DbValue ) {
					ghaveWLan = DbValue;
					memset(str, 0, sizeof(str));
					sprintf(str, "/app/wifi_usb_power.sh %s", (ghaveWLan==0)?"block":"enable");
					system(str);
				}
			}
		}

		system(". /app/karas_pinetree_util.sh --tz-change");
	extern void settimeofday_tz();
		settimeofday_tz();
	}
}



// ========================== LAMINATION SERVICES START ==========================

void SendStatusForLaminationJob(uint32_t status)
{
	//if( (gLaminationSystemState == LMN_RSLT_PREHEAT_DONE)
	//		 || (gLaminationSystemState == LMN_RSLT_1PAGE_LAMINATION_DONE)
	//		 || (gLaminationSystemState == LMN_RSLT_ENGINE_READY) ){

	if( (gLaminationSystemState == LMN_RSLT_LAMINATING_READY_MPT_SET) ||
			(gLaminationSystemState == LMN_RSLT_LAMINATING_READY_MPT_EMPTY) ||
			(gLaminationSystemState == LMN_RSLT_1PAGE_LAMINATION_DONE)){

		SYS_UI_MESSAGE msg;
		msg.msgSender = PRINTMGRID;
		msg.cmd = 0;
		memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);
		memcpy(msg.data, &status, sizeof(uint32_t));
		GoLaminationResultService(&msg);
	}
	else{
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Dont send to UI. Cur Status:%d \n", __FUNCTION__, gLaminationSystemState);
	}
}

void GoLaminationService(SYS_UI_MESSAGE *Message)
{
	stPrint_GoLamination stGoLamination = { 0 };
	stPrint_GoLaminationResult laminationResult = { 0 };
	SYS_UI_MESSAGE msgToSend;
	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = 0;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	memcpy(&stGoLamination, Message->data, sizeof(stPrint_GoLamination));

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Action:%d Type:%d\n", __FUNCTION__, stGoLamination.action, stGoLamination.type);
	if (stGoLamination.action == LMN_ACT_START_LAMINATION) {//Start Lamination: Request preheating

		//20150824: Send full wake_up to engine if last time cold started
		SysWakeUpForUserAttention(SW_LAMINATION_JOB_REQUESTED);

		//Check for print job: If any print job already running then do not allow Lamination job
		if (gSystemPrintJobStatus) {	// || gLaminationJobStatus)
			DBG_SSL_PRINTF_SDH_ERR("[%s]: Lamination job failed because Print job running\n", __FUNCTION__);
			laminationResult.result = LMN_RSLT_JOB_FAILED;
			msgToSend.cmd = CMD_SU_PRINT_GOLAMINATION_RES;
			memcpy(msgToSend.data, &laminationResult, sizeof(stPrint_GoLamination));
			GoLaminationResultService(&msgToSend);
		}
		else {
			laminationResult.result = LMN_RSLT_JOB_ACCEPTED;
			msgToSend.cmd = CMD_SU_PRINT_GOLAMINATION_RES;
			memcpy(msgToSend.data, &laminationResult, sizeof(stPrint_GoLamination));
			GoLaminationResultService(&msgToSend);

			//lamination preheat request
			LaminatingSendMsgToEng(MSG_START_LAMINATION);
		}
	}
	else if (stGoLamination.action == LMN_ACT_START_1PAGE) {
		//Start one page Lamination
		LaminatingSendMsgToEng(MSG_START_LAMINATING_1PAGE);
		gLamination1PageStatus = true;
	}
	else if (stGoLamination.action == LMN_ACT_STOP_LAMINATION) {
		//Stop Lamination
		LaminatingSendMsgToEng(MSG_STOP_LAMINATION);
		//gLamination1PageStatus = false;
	}
	else if (stGoLamination.action == LMN_ACT_RESUME_LAMINATION) {
		//Resume Lamination
		LaminatingSendMsgToEng(MSG_RESUME_LAMINATION);
	}
	else if (stGoLamination.action == LMN_ACT_END_LAMINATION) {
		//call end of lamination
		LaminatingSendMsgToEng(MSG_END_LAMINATION);
	}
	else if (stGoLamination.action == LMN_ACT_CHECK_EXIT_GUIDE_ATTACH) {
		//Check Exit guide attached or not
		LaminatingSendMsgToEng(MSG_CHECK_EXITGUIDE_ATTACH);
	}
	else if (stGoLamination.action == LMN_ACT_CHECK_EXIT_GUIDE_DETACH) {
		//Check Exit guide Detached or not
		LaminatingSendMsgToEng(MSG_CHECK_EXITGUIDE_DETACH);
	}
}

void GoLaminationResultService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stPrint_GoLaminationResult laminationResult = { 0 };
	bool msgSendFlag = true;

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_PRINT_GOLAMINATION_RES;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	//memcpy(&laminationResult, Message->data, sizeof(stPrint_GoLaminationResult));

#if 0
	if (Message->msgSender == SJMID) {
		memcpy(&laminationResult, Message->data, sizeof(stPrint_GoLaminationResult));
		switch(laminationResult.result)
		{
			case esjm_JobReceived:
				msgSendFlag = false;
				++g_nLaminationJobCount;
				if(g_nLaminationJobCount > 0) {
					gLaminationJobStatus = true;
					TimeoutControl(TIMEOUT_PAUSE);
				}
				DBG_SSL_PRINTF_SDH_ERR("JCLamination:%d\n", g_nLaminationJobCount);
				break;
			case esjm_JobStart:
				break;
			case esjm_JobDone:
				--g_nLaminationJobCount;
				if(g_nLaminationJobCount <= 0) {
					gLaminationJobStatus = false;
					TimeoutControl(TIMEOUT_RESET);
				}
				DBG_SSL_PRINTF_SDH_ERR("-JCLamination:%d\n", g_nLaminationJobCount);
				break;
			case esjm_JobCancel_at_device:
			case esjm_JobCancel_by_user:
				break;
			case esjm_JobCancelDone:
				break;
			default:
				msgSendFlag = false;
				break;
		}
	}
	else
#endif
	if (Message->msgSender == PRINTMGRID) {
		uint32_t laminationStatus;
		memcpy(&laminationStatus, Message->data, sizeof(uint32_t));
		switch (laminationStatus) {
		case STATUS_LAMINATEINFO_PREHEATING:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_PREHEATING \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_PREHEATING;
			//if(gLamination1PageStatus){  //Check if 1-Page Laminating
			//	laminationResult.result = LMN_RSLT_LAMINATING_PREHEATING;
			//}
			break;
		case STATUS_LAMINATEINFO_PREHEATDONE:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_PREHEATDONE \n", __FUNCTION__, laminationStatus);

			//Set MPT_EMPTY then check if paper present then set MPT_SET
			laminationResult.result = LMN_RSLT_LAMINATING_READY_MPT_EMPTY; //LMN_RSLT_PREHEAT_DONE;

			if ((gSystemFullStack == false) && (gFrontDoorStatus == false)
					&& (gSystemJamStatus == false) && (gSystemSCStatus == false)
					&& (gStFeedTray0Status.paperVolume != MEDIA_TRAY_PAPER_END)) {

				laminationResult.result = LMN_RSLT_LAMINATING_READY_MPT_SET;
			}
			break;
		case STATUS_LAMINATEINFO_LAMINATING:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_LAMINATING \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_LAMINATING;
			break;
		case STATUS_LAMINATEINFO_LAMINATINGDONE:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_LAMINATINGDONE \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_1PAGE_LAMINATION_DONE;
			gLamination1PageStatus = false;
			break;
		case STATUS_LAMINATEINFO_LAMIJOBDONE:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_LAMIJOBDONE \n", __FUNCTION__, laminationStatus);
			gLamination1PageStatus = false;
			laminationResult.result = LMN_RSLT_JOB_COMPLETED;
			break;
		case STATUS_LAMINATEINFO_EXITGUIDE_CLEAR:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_EXITGUIDE_CLEAR \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_EXIT_GUIDE_NOT_SET;
			break;
		case STATUS_LAMINATEINFO_EXITGUIDE_SET:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_EXITGUIDE_SET \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_EXIT_GUIDE_STILL_SET;
			break;
		case STATUS_LAMINATEINFO_HEATING:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_HEATING \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_LAMINATING_PREHEATING;
			break;
		case STATUS_LAMINATEINFO_LAMINATION_STOPPED:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_LAMINATION_STOPPED \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_STOPED_USER_REQ;
			break;
#if 0
		case STATUS_LAMINATEINFO_JAM_INPUT_DETECT:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_JAM_INPUT_DETECT \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_STOPED_JAM;
			break;
		case STATUS_LAMINATEINFO_JAM_INPUT_RELEASE:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_LAMINATEINFO_JAM_INPUT_RELEASE \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_STOPED_JAM;
			break;
#endif
			//Special case when user set paper then send Engine_Ready msg to UI
			//if last message was LMN_RSLT_PREHEAT_DONE or LMN_RSLT_1PAGE_LAMINATION_DONE
		case STATUS_TRAY4_PAPER_LOW:
		case STATUS_TRAY4_PAPER_NORMAL:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_TRAY4 MPT PAPER SET \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_LAMINATING_READY_MPT_SET;
			break;
			//Special case
		case STATUS_PAPER_OUT_TRAY4:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: %d STATUS_TRAY4 MPT EMPTY \n", __FUNCTION__, laminationStatus);
			laminationResult.result = LMN_RSLT_LAMINATING_READY_MPT_EMPTY;
			break;

		default:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X Error: Please check status value \n", __FUNCTION__, laminationStatus);
			msgSendFlag = false;
			break;
		}
	}
	else if (Message->msgSender == SYSTEM_SERVICE_ID) {
		memcpy(&laminationResult, Message->data, sizeof(stPrint_GoLaminationResult));
		//Do nothing send same status
	}
	else {
		msgSendFlag = false;
	}

	if (msgSendFlag) {

		//TODO: do not trust laminationResult.result for gLaminationJobStatus. Add proper counting
		if (laminationResult.result == LMN_RSLT_JOB_ACCEPTED) {
			++g_nLaminationJobCount;
			if (g_nLaminationJobCount > 0) {
				gLaminationJobStatus = true;
				TimeoutControl(TIMEOUT_PAUSE);
			}
			DBG_SSL_PRINTF_SDH_ERR("JCLamination:%d\n", g_nLaminationJobCount);
		}
		else if (laminationResult.result == LMN_RSLT_JOB_COMPLETED) {
			--g_nLaminationJobCount;
			if (g_nLaminationJobCount <= 0) {
				gLaminationJobStatus = false;
				TimeoutControl(TIMEOUT_RESET);
			}
			DBG_SSL_PRINTF_SDH_ERR("-JCLamination:%d\n", g_nLaminationJobCount);
		}

		//if(gLaminationSystemState != laminationResult.result){
		if (1) {
			gLaminationSystemState = laminationResult.result;
			memcpy(msgToSend.data, &laminationResult, sizeof(stPrint_GoLaminationResult));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT(err==OK, err);
		}

		//Send LAMINATING_READY_* msg if last msg was 1PAGE_LAMINATION_DONE and no problem at system side
		if (gLaminationSystemState == LMN_RSLT_1PAGE_LAMINATION_DONE){
			//Check Error condition, if no system error then send LAMINATING_READY msg
			if ((gSystemFullStack == false) && (gFrontDoorStatus == false)
					&& (gSystemJamStatus == false) && (gSystemSCStatus == false)){

				if(gStFeedTray0Status.paperVolume != MEDIA_TRAY_PAPER_END){
					laminationResult.result = LMN_RSLT_LAMINATING_READY_MPT_SET;
				}
				else{
					laminationResult.result = LMN_RSLT_LAMINATING_READY_MPT_EMPTY;
				}

				gLaminationSystemState = laminationResult.result;
				memcpy(msgToSend.data, &laminationResult, sizeof(stPrint_GoLaminationResult));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT(err==OK, err);
			}
			else{
				DBG_SSL_PRINTF_SDH_ERR("Error after 1PAGE_LAMINATION_DONE Door:%d Jam:%d SC:%d FS:%d\n",
						gFrontDoorStatus, gSystemJamStatus, gSystemSCStatus, gSystemFullStack);
			}
		}

	} //if (msgSendFlag)
}
//========================= LAMINATION SERVICES END ==========================


static void FilePrintDone( char *path, void *context )
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   File Print DONE. \n", __FUNCTION__);
}

static void PrnPrintDone( char *path, void *context )
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   Prn Print DONE. \n", __FUNCTION__);
}

void SendCancelJobMsgToSJM(UINT32 jobId, DOACTION actionType, RESOURCES resourceType)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s] JobID=%d, Action=%d, Resource=%d\n", __FUNCTION__, jobId, actionType, resourceType);
	error_type_t err;
	MESSAGE msg;

	if(jobId == 0){
		//Start Job Canceling. Send CANCELJOB msg to SJM.
		msg.msgType = MSG_CANCELJOB;
		msg.param1 = actionType;	//SYS_REQUEST or SYS_REQUSER;
		msg.param2 = 0;
		msg.param3 = (void*) resourceType;
		err = SYMsgSendNoWait( SJMID, &msg );
		XASSERT( err==OK, err );
	} else{
		///TODO:  Cancel JOB which has jobID.
		DBG_SSL_PRINTF_SDH_ERR("[%s] Cannot cancel jobId=%d\n", __FUNCTION__, jobId);
	}
}

#define FILEPRINT_KIND_PDF 0
#define FILEPRINT_KIND_JPG 1

void GoPrintService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	error_type_t e_res;
	MESSAGE msg;	
	stShmGoPrint *pPrintAddInfo;
	int ret;
	
	stPrint_GoPrint stGoPrint;
	FILE *filePrintFP;
	char* pFileExtension;

	int tray1PaperSize = 0;

	memcpy(&stGoPrint, Message->data, sizeof(stPrint_GoPrint));

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Action:%d Type:%d Kind:%d\n", __FUNCTION__, stGoPrint.action, stGoPrint.type, stGoPrint.kind);

	if(stGoPrint.action == PRINT_ACTION_START){	//Start Job
	
		switch(stGoPrint.type){
			case PRINT_TYPE_INTERNAL_REPORT:	//Internal report
				msg.msgType = MSG_INTERNAL_PAGE;
				switch(stGoPrint.kind){
				case 0:
					msg.param1 = e_SYSTEMREPORT;
					break;
				case 1:
					msg.param1 = e_NETWORKREPORT;
					break;
				case 2:
					msg.param1 = e_SPREPORT;
					break;
				case 4:
					msg.param1 = e_FAXTXRXREPORT;
					break;
				case 5:
					msg.param1 = e_FAXTCR;
					break;
				case 6:
					msg.param1 = e_MEMTXJOBLIST;
					break;	
				case 7:
					msg.param1 = e_ERRLOGREPORT;
					break;
				case 8:
					msg.param1 = e_T30LOGREPORT;
					break;
				case 9:
					msg.param1 = e_TCRFORFAXSERVICE;
					break;						
				case 10:
					msg.param1 = e_ADDRESSBOOK;
					break;
				case 11:
					msg.param1 = e_SPAMFAXLIST;
					break;
				case 12:
					msg.param1 = e_BROADCASTRESULTREPORT;
					break;
				default:
					msg.param1 = 0;
					break;
				}
				msg.param2 = 0;
				msg.param3 = 0; 	//NOT USE
				err = SYMsgSend(INTERNALMGRID, &msg);         // send the message.
				XASSERT( err==OK, err );
				break;
			case PRINT_TYPE_PATTERN_PRINT:	//Pattern print
				e_res = OK;
				tray1PaperSize = getTray1SizeCode(gStFeedTray1Status.sizeCode);
				switch(stGoPrint.kind)
				{
					case 0:
						if(tray1PaperSize == MEDIASIZE_LETTER)
						{
							file_to_pipe( "/app/pattern/skew_LT.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						else
						{
							file_to_pipe( "/app/pattern/skew.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						break;
					case 1:
						if(tray1PaperSize == MEDIASIZE_LETTER)
						{
							file_to_pipe( "/app/pattern/skew_duplex_LT.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						else
						{
							file_to_pipe( "/app/pattern/skew_duplex.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						break;
					case 2:
						if(tray1PaperSize == MEDIASIZE_LETTER)
						{
							file_to_pipe( "/app/pattern/quality_LT.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						else
						{
							file_to_pipe( "/app/pattern/quality.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						break;
					case 3:
						if(tray1PaperSize == MEDIASIZE_LETTER)
						{
							file_to_pipe( "/app/pattern/cleaning_LT.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						else
						{
							file_to_pipe( "/app/pattern/cleaning_A4.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						}
						break;
					case 4:
						file_to_pipe( "/app/pattern/PCLFontlist.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						break;
					case 5:
						file_to_pipe( "/app/pattern/PSFontlist.prn", PrnPrintDone, NULL, e_SYSTEMREPORT );
						break;
				}
				break;
			case PRINT_TYPE_FILE_PRINT:	//File print			
				e_res = OK;			
				pPrintAddInfo = shm_attach(stGoPrint.addInfo);
				pPrintAddInfo->image_height = -1;
				pPrintAddInfo->image_width = -1;

				DBG_SSL_PRINTF_SDH_ERR("stGoPrint.addInfo : %d\n", stGoPrint.addInfo);
				if ( pPrintAddInfo != (void *)-1 ) {
					DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->filePath : %s\n", pPrintAddInfo->filePath);
					DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->fileName : %s\n", pPrintAddInfo->fileName);
					DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo others : %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", pPrintAddInfo->makeApp, pPrintAddInfo->orginType, pPrintAddInfo->sort, pPrintAddInfo->inTray, pPrintAddInfo->staple, pPrintAddInfo->duplex, pPrintAddInfo->coverSlip, pPrintAddInfo->stamp, pPrintAddInfo->fileCount, pPrintAddInfo->repeat, pPrintAddInfo->darkness);
					pFileExtension = strrchr(pPrintAddInfo->fileName, '.') + 1;
					DBG_SSL_PRINTF_SDH_DEBUG("pFileExtension : %s\n", pFileExtension);

					// Convert TIFF & PNG to JPG
					if(!strncasecmp(pFileExtension, "tif", 3) || !strncasecmp(pFileExtension, "tiff", 4))
					{
#if 0  // Different coverted file
						char cmd_buffer[SDH_FILEPATH_MAX+SDH_FILENAME_MAX+80]; // Path Length + Name Length + Command Length
						char fileNameConverted[SDH_FILENAME_MAX];
						char* pFileExtensionConverted = NULL;
						memset(cmd_buffer, 0, sizeof(cmd_buffer));
						memset(fileNameConverted, 0, sizeof(fileNameConverted));
						snprintf(fileNameConverted, sizeof(fileNameConverted), pPrintAddInfo->fileName);
						pFileExtensionConverted = strrchr(fileNameConverted, '.') + 1;
						snprintf(pFileExtensionConverted, 4, "jpg");
						snprintf(cmd_buffer, sizeof(cmd_buffer), "convert \"%s/%s[0]\" \"/tmp/%s\"", pPrintAddInfo->filePath, pPrintAddInfo->fileName, fileNameConverted);
						DBG_SSL_PRINTF_SDH_ERR("\n------------------------------\nConverting... \n %s\n", cmd_buffer);
						system(cmd_buffer);
						DBG_SSL_PRINTF_SDH_ERR("Converting Complete\n------------------------------\n\n");
						snprintf(pPrintAddInfo->filePath, sizeof(pPrintAddInfo->filePath), "/tmp");
						snprintf(pPrintAddInfo->fileName, sizeof(pPrintAddInfo->fileName), "%s", fileNameConverted);
						DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->filePath(CV) : %s\n", pPrintAddInfo->filePath);
						DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->fileName(CV) : %s\n", pPrintAddInfo->fileName);
#else  // Just one converted file(overwrite)
						char cmd_buffer[SDH_FILEPATH_MAX+SDH_FILENAME_MAX+80]; // Path Length + Name Length + Command Length
						memset(cmd_buffer, 0, sizeof(cmd_buffer));
						snprintf(cmd_buffer, sizeof(cmd_buffer), "convert \"%s/%s[0]\" \"/tmp/tif.jpg\"", pPrintAddInfo->filePath, pPrintAddInfo->fileName);
						DBG_SSL_PRINTF_SDH_ERR("\n------------------------------\nConverting... \n %s\n", cmd_buffer);
						system(cmd_buffer);
						DBG_SSL_PRINTF_SDH_ERR("Converting Complete\n------------------------------\n\n");
						snprintf(pPrintAddInfo->filePath, sizeof(pPrintAddInfo->filePath), "/tmp");
						snprintf(pPrintAddInfo->fileName, sizeof(pPrintAddInfo->fileName), "tif.jpg");
						DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->filePath(CV) : %s\n", pPrintAddInfo->filePath);
						DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->fileName(CV) : %s\n", pPrintAddInfo->fileName);
						pFileExtension = strrchr(pPrintAddInfo->fileName, '.') + 1;
#endif
					}
					else if(!strncasecmp(pFileExtension, "png", 3))
					{
						char cmd_buffer[SDH_FILEPATH_MAX+SDH_FILENAME_MAX+80]; // Path Length + Name Length + Command Length
						memset(cmd_buffer, 0, sizeof(cmd_buffer));
						snprintf(cmd_buffer, sizeof(cmd_buffer), "convert -background \"rgb(255,255,255)\" -flatten \"%s/%s\" \"/tmp/png.jpg\"", pPrintAddInfo->filePath, pPrintAddInfo->fileName);
						DBG_SSL_PRINTF_SDH_ERR("\n------------------------------\nConverting... \n %s\n", cmd_buffer);
						system(cmd_buffer);
						DBG_SSL_PRINTF_SDH_ERR("Converting Complete\n------------------------------\n\n");
						snprintf(pPrintAddInfo->filePath, sizeof(pPrintAddInfo->filePath), "/tmp");
						snprintf(pPrintAddInfo->fileName, sizeof(pPrintAddInfo->fileName), "png.jpg");
						DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->filePath(CV) : %s\n", pPrintAddInfo->filePath);
						DBG_SSL_PRINTF_SDH_ERR("pPrintAddInfo->fileName(CV) : %s\n", pPrintAddInfo->fileName);
						pFileExtension = strrchr(pPrintAddInfo->fileName, '.') + 1;
					}

					if(!strncasecmp(pFileExtension, "pdf", 3)){
						filePrintFP = fopen(PDF_PCL_FILE_PATH, "w");
						if(filePrintFP){
							fprintf( filePrintFP, "@PJL ENTER LANGUAGE=PDF_FILE\n" );
							fprintf( filePrintFP, "%s/%s\n", pPrintAddInfo->filePath, pPrintAddInfo->fileName);

							ret = shm_remove(stGoPrint.addInfo);
							if ( ret < 0 ) {
								DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_remove error: \n", __FUNCTION__);
							}							
							fclose(filePrintFP);
						}
						else{
							DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   File Open Error \n", __FUNCTION__);
						}						
						file_to_pipe(PDF_PCL_FILE_PATH, FilePrintDone, pPrintAddInfo, e_USBDRIVEPDFPAGE);
					}
					else if(!strncasecmp(pFileExtension, "jpg", 3) || !strncasecmp(pFileExtension, "jpeg", 4)){
						filePrintFP = fopen(JPG_PCL_FILE_PATH, "w");
						if(filePrintFP){
							fprintf( filePrintFP, "@PJL ENTER LANGUAGE=IMAGE_FILE\n" );
							fprintf( filePrintFP, "%s/%s\n", pPrintAddInfo->filePath, pPrintAddInfo->fileName);

							ret = shm_remove(stGoPrint.addInfo);
							if ( ret < 0 ) {
								DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_remove error: \n", __FUNCTION__);
							}							
							fclose(filePrintFP);
						}
						else{
							DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   File Open Error \n", __FUNCTION__);
						}						
						file_to_pipe(JPG_PCL_FILE_PATH, FilePrintDone, pPrintAddInfo, e_USBDRIVEJPGPAGE);
					}
				}
				ret = shm_remove(stGoPrint.addInfo);
				if ( ret < 0 ) {
					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_remove error: \n", __FUNCTION__);
				}
				break;
			case PRINT_TYPE_TEST_PRINT: // Katusha test print
			{
				stShmGoPrint addinfo;
				memset(&addinfo, 0, sizeof(addinfo));

				addinfo.inTray = 8;   // Auto
				addinfo.darkness = 5;
				addinfo.sort = 1;
				addinfo.duplex = 0;
				addinfo.repeat = 1;
				addinfo.image_height = -1;
				addinfo.image_width = -1;

				FILE* fp = fopen(PDF_PCL_FILE_PATH, "w");
				if (fp) {
					fprintf(fp, "@PJL SET RESOLUTION=1200\n" );
					fprintf(fp, "@PJL ENTER LANGUAGE=PDF_FILE\n" );
					fprintf(fp, "/app/page/katusha_test.pdf\n");

					fclose(fp);
				}

				file_to_pipe(PDF_PCL_FILE_PATH, FilePrintDone, &addinfo, e_USBDRIVEPDFPAGE);
			}
				break;
			default:
				break;
		}
	}
	else if(stGoPrint.action == PRINT_ACTION_CANCEL){	//Cancel Job
		SendCancelJobMsgToSJM(stGoPrint.jobID, SYS_REQUEST, e_Printer);
	}
	else if(stGoPrint.action == PRINT_ACTION_RESUME){	//Resume Job
		if(stGoPrint.jobID == 0){
			if(gStRetryPrintSystemStatus.retryCallback != NULL){
				(gStRetryPrintSystemStatus.retryCallback)(gStRetryPrintSystemStatus.retrySubject);
			}
		}
	}
}


int getTray1SizeCode(int detectedPaperSize)
{
	if(gStFeedTray1Status.sizeCode == MEDIASIZE_BAD)
	{
		int UPPaperSize = 0;
		error_type_t db_retval = FAIL;
		UPPaperSize = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", &db_retval);
		if(db_retval == FAIL)
		{
			DBG_SSL_PRINTF_SDH_ERR("%s DB Read fail\n", __func__);
		}
		return UPPaperSize;
	}
	else
		return gStFeedTray1Status.sizeCode;
}

void CopyFailedError(UINT8 err_no)
{
	SYS_UI_MESSAGE msgToSend;
	DBG_SSL_PRINTF_SDH_ERR("Copy failed due to : %d\n", err_no);
	stCopy_GoCopyResult stCopyResult = {0};
	stCopyResult.result = RSLT_JOB_FAILED;
	stCopyResult.jobID = g_nCurrentCopyJobID;
	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_COPY_GOCOPY_RES;
	memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
	CopyGoCopyResultService(&msgToSend);
}


void CopyParamError(UINT8 err_no, UINT16 paper_size)
{
	SYS_UI_MESSAGE msgToSend;
	DBG_SSL_PRINTF_SDH_ERR("valid check result : %d\n", err_no);

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_COPY_GOCOPY_RES;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	msgToSend.data[0] = COPYAPP_EVENT_PARAM_FAULT;
	msgToSend.data[4] = err_no;
	msgToSend.data[8] = paper_size%256;
	msgToSend.data[9] = paper_size/256;
	
	CopyAppEventService(&msgToSend);
}

void CopyAppService(SYS_UI_MESSAGE *Message)
{
	ROTATE_DIR rotation = e_NoRotate;
	UINT8	ucAutoTray;
	
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Called.  \n", __FUNCTION__);

	if(g_nEnergySaveMode){
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Error: Do not allow UI Copy job in Power Save mode\n", __FUNCTION__);
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Error: Ignoring UI Copy job request\n", __FUNCTION__);
		//Send message to UI for Copy failed
		CopyFailedError(1);
		return;
	}

#ifdef HAVE_COPY_SUPPORT
	
	error_type_t err;
	STARTRECIPE *StartIt;
	MESSAGE msg;

	stCopy_GoCopy stGoCopy = {0};	
	memcpy(&stGoCopy, Message->data, sizeof(stCopy_GoCopy));

#if 1	
	DBG_SSL_PRINTF_SDH_ERR("action : %d\n", stGoCopy.action);
	DBG_SSL_PRINTF_SDH_ERR("scanResolution : %d\n", stGoCopy.scanResolution);
	DBG_SSL_PRINTF_SDH_ERR("collation : %d\n", stGoCopy.collation);
	DBG_SSL_PRINTF_SDH_ERR("numberOfCopies : %d\n", stGoCopy.numberOfCopies);
	DBG_SSL_PRINTF_SDH_ERR("copyQuality : %d\n", stGoCopy.copyQuality);
	DBG_SSL_PRINTF_SDH_ERR("scanDuplex : %d\n", stGoCopy.scanDuplex);
	DBG_SSL_PRINTF_SDH_ERR("printDuplex : %d\n", stGoCopy.printDuplex);
	DBG_SSL_PRINTF_SDH_ERR("brightness : %d\n", stGoCopy.brightness);
//	DBG_SSL_PRINTF_SDH_ERR("contrast : %d\n", stGoCopy.contrast);
//	DBG_SSL_PRINTF_SDH_ERR("density : %d\n", stGoCopy.density);
	DBG_SSL_PRINTF_SDH_ERR("copyBackground : %d\n", stGoCopy.copyBackground);
	DBG_SSL_PRINTF_SDH_ERR("copyResize : %d\n", stGoCopy.copyResize);
	DBG_SSL_PRINTF_SDH_ERR("scale : %d\n", stGoCopy.scale);
	DBG_SSL_PRINTF_SDH_ERR("scanSizeCode : %d\n", stGoCopy.scanPaperSize);
//	DBG_SSL_PRINTF_SDH_ERR("scanCustomSize : w(%d) h(%d)\n", stGoCopy.scanCustomSize[0], stGoCopy.scanCustomSize[1]);
	DBG_SSL_PRINTF_SDH_ERR("printPaperType : %d\n", stGoCopy.printPaperType);
//	DBG_SSL_PRINTF_SDH_ERR("printCustomSize : w(%d) h(%d)\n", stGoCopy.printCustomSize[0], stGoCopy.printCustomSize[1]);
	DBG_SSL_PRINTF_SDH_ERR("inputTray : %d\n", stGoCopy.inputTray);
	DBG_SSL_PRINTF_SDH_ERR("trayPaperInfo : M(%d) T1(%d) T2(%d) T3(%d)\n", stGoCopy.trayPaperInfo[0]
						, stGoCopy.trayPaperInfo[1], stGoCopy.trayPaperInfo[2], stGoCopy.trayPaperInfo[3]);
	DBG_SSL_PRINTF_SDH_ERR("printBordering : %d\n", stGoCopy.printBordering);
#endif

	if (stGoCopy.scale == 165)
	{
		DBG_SSL_PRINTF_SDH_ERR("change scale: 165 -> 164\n");
		stGoCopy.scale = 164;
	}
	else if (stGoCopy.scale == 131)
	{
		DBG_SSL_PRINTF_SDH_ERR("change scale: 165 -> 164\n");
		stGoCopy.scale = 130;
	}
	else if (stGoCopy.scale == 193)
	{
		DBG_SSL_PRINTF_SDH_ERR("change scale: 165 -> 164\n");
		stGoCopy.scale = 192;
	}
	///TODO: Call Copy APP APIs to set CopyConfig received from UI process
	
	if (stGoCopy.action == COPY_ACTION_START)
	{
		error_type_t db_retval = FAIL;
		UINT8	valid_check;
		
		valid_check = copyapp_check_param_valid(stGoCopy);
		if (valid_check != e_COPY_VALID_OK)
		{
			if ( valid_check == e_COPY_VALID_FAIL_FB_PAPER)
				CopyParamError(valid_check, stGoCopy.scanPaperSize);
			else
				CopyParamError(valid_check, 0);
			return;
		}
			
		if (stGoCopy.copyResize == e_ID_COPY)
		{
			DBG_SSL_PRINTF_SDH_ERR("ID_COPY : scale change %d -> 100\n", stGoCopy.scale);
			stGoCopy.scale = 100;
		}
		else if ((stGoCopy.scanPaperSize == MEDIASIZE_LEGAL) && (!copyapp_check_adf_paper()))
		{
			stGoCopy.scale = COPY_SCALE_AUTO;
			stGoCopy.scanResolution = e_SCANQUALITY_NORMAL;
			stGoCopy.copyResize = e_ONE_NORMAL;
		}
		
		ucAutoTray = stGoCopy.inputTray;
		if (ucAutoTray == e_COPY_INTRAY_AUTO)
		{
			UINT16 paper_size = 0;
			if ((stGoCopy.copyResize != e_ONE_NORMAL) || (stGoCopy.copyResize == e_ID_COPY))
			{
				ucAutoTray = copyapp_find_tray_A4_LT_LG(stGoCopy.trayPaperInfo, e_COPY_FIND_TRAY_SCOPE_A4_LT_LG);
				if (ucAutoTray == e_COPY_VALID_FAIL_FIND_TRAY)
				{
					CopyParamError(e_COPY_VALID_FAIL_ID_PAPER, 0);
					return;
				}
/*				if ((stGoCopy.copyResize == e_ID_COPY) &&
					((stGoCopy.trayPaperInfo[ucAutoTray] != MEDIASIZE_A4) && (stGoCopy.trayPaperInfo[ucAutoTray] != MEDIASIZE_LETTER)))
				{
					CopyParamError(e_COPY_VALID_FAIL_FIND_TRAY, 0);
					return;
				}
*/
			}
			else {
				ucAutoTray = copyapp_find_tray(stGoCopy.scanPaperSize, stGoCopy.trayPaperInfo, stGoCopy.scale, &rotation, &paper_size);
				if (ucAutoTray == e_COPY_VALID_FAIL_FIND_TRAY)
				{
					if ((paper_size == MEDIASIZE_A4) || (paper_size == MEDIASIZE_LETTER) || (paper_size == MEDIASIZE_LEGAL))
					{
						ucAutoTray = copyapp_find_tray(paper_size, stGoCopy.trayPaperInfo, 100, &rotation, &paper_size);
					}
					
					if (ucAutoTray == e_COPY_VALID_FAIL_FIND_TRAY)
					{
						CopyParamError(e_COPY_VALID_FAIL_FIND_TRAY, paper_size);
						return;
					}
				}
			}
				
			DBG_SSL_PRINTF_SDH_ERR("COPY_TRAY_AUTO : %d, rot(%d)\n", ucAutoTray, rotation);

		}
		else if ((stGoCopy.scale == COPY_SCALE_AUTO ) && (stGoCopy.copyResize == e_ONE_NORMAL))
		{
			stGoCopy.scale = copyapp_find_scale(stGoCopy.scanPaperSize, stGoCopy.trayPaperInfo[ucAutoTray], &rotation);
			DBG_SSL_PRINTF_SDH_ERR("COPY_SCALE_AUTO : %d, rot(%d)\n", stGoCopy.scale, rotation);
			if (stGoCopy.scale == COPY_SCALE_ERROR)
			{
				CopyParamError(e_COPY_VALID_FAIL_FIND_SCALE, 0);
				return;
			}
		}
		else if (stGoCopy.scale == COPY_SCALE_AUTO )
		{
			stGoCopy.scale = 100;
		}
// nUP check
		if ( (stGoCopy.copyResize == e_TWO_PAGES_PER_SHEET) || (stGoCopy.copyResize == e_FOUR_PAGES_PER_SHEET) )
		{
			valid_check = copyapp_check_nUP_valid(stGoCopy.scanPaperSize, stGoCopy.trayPaperInfo[ucAutoTray] );
			if (valid_check != e_COPY_VALID_OK)
			{
				CopyParamError(valid_check, 0);
				return;
			}	
		}
		
// Duplex print check : A4/LT/LG/Folio only
		if (stGoCopy.printDuplex)
		{
			if ((stGoCopy.trayPaperInfo[ucAutoTray] != MEDIASIZE_LETTER) && (stGoCopy.trayPaperInfo[ucAutoTray] != MEDIASIZE_A4)
				&& (stGoCopy.trayPaperInfo[ucAutoTray] != MEDIASIZE_LEGAL) && (stGoCopy.trayPaperInfo[ucAutoTray] != MEDIASIZE_CUSTOM_85x13))
			{
				DBG_SSL_PRINTF_SDH_ERR("Duplex print check : %d\n", stGoCopy.trayPaperInfo[ucAutoTray]);
				CopyParamError(e_COPY_VALID_FAIL_DUPLEX_PRINT, 0);
				return;
			}
		}

		stGoCopy.density = db_get_var_int("CONF_COPYSET", "Darkness", &db_retval);
		if(db_retval == FAIL)
			stGoCopy.density = 6;
		
		if (stGoCopy.copyBackground)
		{
			stGoCopy.copyBackground = db_get_var_int("CONF_COPYSET", "BgRemove", &db_retval);
		
			if(db_retval == FAIL)
				stGoCopy.copyBackground = 5;
		}
			
		copyapp_set_fp_rotation( rotation );
		copyapp_set_fp_colormode(COPY_CMODE_MONO);	// CopyConfig.ccmode );
		copyapp_set_fp_qualitymode( stGoCopy.copyQuality );
		
		if (stGoCopy.copyResize == e_ID_COPY)
		{
			copyapp_set_id_copy(TRUE);
			copyapp_set_fp_print_paper_size( stGoCopy.trayPaperInfo[ucAutoTray] );
		}
		else
		{
			copyapp_set_id_copy(FALSE);
			copyapp_set_fp_resize( stGoCopy.copyResize, stGoCopy.scale );	///TODO: set custom resize data
			copyapp_set_fp_print_paper_size( stGoCopy.trayPaperInfo[ucAutoTray] );
		}
		
		copyapp_set_fp_num_copies( stGoCopy.numberOfCopies );
		copyapp_set_fp_brightness( stGoCopy.brightness );
		copyapp_set_fp_background( stGoCopy.copyBackground );
//		copyapp_set_fp_contrast(stGoCopy.contrast);
		copyapp_set_fp_print_quality( PRINTQUALITY_NORMAL );	// always 600 dpi
		copyapp_set_fp_input_tray( stGoCopy.inputTray );
		copyapp_set_fp_print_paper_size_info(stGoCopy.trayPaperInfo);
		copyapp_set_fp_collation( stGoCopy.collation );
		copyapp_set_fp_print_duplex( stGoCopy.printDuplex );
		copyapp_set_fp_scan_duplex( stGoCopy.scanDuplex );
		copyapp_set_fp_scan_paper_size( stGoCopy.scanPaperSize );
		copyapp_set_fp_print_density( stGoCopy.density );
		
		if (stGoCopy.scanResolution == e_SCANQUALITY_HIGH)
			copyapp_set_fp_resolution( 600 );
		else
			copyapp_set_fp_resolution( 300 );
		
		//Prepare a MSG_STARTRECIPE message for Copy App
		StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
		ASSERT(StartIt != NULL);
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Called.  BEFORE Send MSG to CopyApp \n", __FUNCTION__);
		msg.msgType = MSG_STARTRECIPE;
		StartIt->Job = ejob_Copy;
		StartIt->AdditionalResource = 0;
		StartIt->Pipe = 0;
		StartIt->Wait = e_WaitForever;
		msg.param3 = StartIt;       // not freed here, freed by Message receiver

		/** Send a MSG_STARTRECIPE message to the Copy Application Thread telling it to kick off a copy **/
		err = SYMsgSend(COPYAPPID, &msg);         // send the message.
		XASSERT( err==OK, err );
	}
	else if ((stGoCopy.action == COPY_ACTION_A3MERGE) || (stGoCopy.action == COPY_ACTION_A3SEPARATE))
	{
		error_type_t db_retval = FAIL;
		
//		stGoCopy.action = COPY_ACTION_A3SEPARATE;
		
		ucAutoTray = stGoCopy.inputTray;

		if (ucAutoTray == e_COPY_INTRAY_AUTO)
		{
			if (stGoCopy.scanPaperSize == MEDIASIZE_A3)		// A3
			{
				if (stGoCopy.action == COPY_ACTION_A3SEPARATE)
				{
					ucAutoTray = copyapp_find_tray_A4_LT_LG(stGoCopy.trayPaperInfo, e_COPY_FIND_TRAY_SCOPE_A4);
				}
				else
				{
					ucAutoTray = copyapp_find_tray_A4_LT_LG(stGoCopy.trayPaperInfo, e_COPY_FIND_TRAY_SCOPE_A4_LT);
				}
			}
			else if (stGoCopy.scanPaperSize == MEDIASIZE_11X17)		// DLT
			{
				if (stGoCopy.action == COPY_ACTION_A3SEPARATE)
				{
					ucAutoTray = copyapp_find_tray_A4_LT_LG(stGoCopy.trayPaperInfo, e_COPY_FIND_TRAY_SCOPE_LT);
				}
				else
				{
					ucAutoTray = copyapp_find_tray_A4_LT_LG(stGoCopy.trayPaperInfo, e_COPY_FIND_TRAY_SCOPE_LT_A4);
				}
			}
			else		// LG
			{
				if (stGoCopy.action == COPY_ACTION_A3SEPARATE)
				{
					CopyParamError(e_COPY_VALID_FAIL_MERGE_PAPER, 0);
				}
				else
				{	
					ucAutoTray = copyapp_find_tray_A4_LT_LG(stGoCopy.trayPaperInfo, e_COPY_FIND_TRAY_SCOPE_LG_LT_A4);
				}
			}
			
			if ((ucAutoTray == e_COPY_VALID_FAIL_FIND_TRAY) || (ucAutoTray >= e_COPY_INTRAY_MAX ))
			{
// fail A4/LT -> select default tray.
//				ucAutoTray = db_get_var_int("CONF_COPYSET", "PrintSrcTray", &db_retval);
				CopyParamError(e_COPY_VALID_FAIL_MERGE_PAPER, 0);
				return;
			}
		}

		if (stGoCopy.action == COPY_ACTION_A3SEPARATE)
		{
			if (stGoCopy.scanPaperSize == MEDIASIZE_A3)
			{
				stGoCopy.scale = copyapp_find_scale(MEDIASIZE_A4, stGoCopy.trayPaperInfo[ucAutoTray], &rotation);
			}
			else
			{
				stGoCopy.scale = copyapp_find_scale(MEDIASIZE_LETTER, stGoCopy.trayPaperInfo[ucAutoTray], &rotation);
			}
		}
		else
		{
			stGoCopy.scale = copyapp_find_scale(stGoCopy.scanPaperSize, stGoCopy.trayPaperInfo[ucAutoTray], &rotation);
		}
		
		if (stGoCopy.scale == COPY_SCALE_ERROR)
		{
			CopyParamError(e_COPY_VALID_FAIL_FIND_SCALE, 0);
			return;
		}
// ADF paper set check
		if (copyapp_check_adf_paper())
		{
			DBG_SSL_PRINTF_SDH_ERR("A3 merge : Remove adf paper\n");
			CopyParamError(e_COPY_VALID_FAIL_MERGE_ADF, 0);
			return;
		}
		
		if (stGoCopy.action == COPY_ACTION_A3MERGE)
		{
			rotation = (rotation == e_NoRotate)?e_Rotate90:e_NoRotate;
		}
		
		stGoCopy.density = db_get_var_int("CONF_COPYSET", "Darkness", &db_retval);
		if(db_retval == FAIL)
			stGoCopy.density = 6;
			
		if (stGoCopy.copyBackground)
		{
			stGoCopy.copyBackground = db_get_var_int("CONF_COPYSET", "BgRemove", &db_retval);
			
			if(db_retval == FAIL)
				stGoCopy.copyBackground = 5;
		}
		
		copyapp_set_fp_rotation( rotation );
		copyapp_set_fp_colormode(COPY_CMODE_MONO);	// CopyConfig.ccmode );
		copyapp_set_fp_qualitymode( stGoCopy.copyQuality );
		copyapp_set_id_copy(FALSE);
		if (stGoCopy.scanPaperSize == MEDIASIZE_11X17)
		{
			
			if (stGoCopy.action == COPY_ACTION_A3SEPARATE)
			{
				copyapp_set_fp_resize( e_DLT_SEPARATE, stGoCopy.scale );	
			}
			else
			{
				copyapp_set_fp_resize( e_DLT_MERGE, stGoCopy.scale );	///TODO: set custom resize data	A3 => A4 : 70
			}
			copyapp_set_fp_scan_paper_size( MEDIASIZE_LETTER );
		}
		else
		{
			if (stGoCopy.action == COPY_ACTION_A3SEPARATE)
			{
				copyapp_set_fp_resize( e_A3_SEPARATE, stGoCopy.scale );	
			}
			else
			{
				copyapp_set_fp_resize( e_A3_MERGE, stGoCopy.scale );	///TODO: set custom resize data	A3 => A4 : 70
			}
			copyapp_set_fp_scan_paper_size( MEDIASIZE_A4 );
		}
		copyapp_set_fp_print_paper_size( stGoCopy.trayPaperInfo[ucAutoTray] );
		copyapp_set_fp_num_copies( stGoCopy.numberOfCopies );
		copyapp_set_fp_brightness( stGoCopy.brightness );
		copyapp_set_fp_background( stGoCopy.copyBackground );
//		copyapp_set_fp_contrast(stGoCopy.contrast);
		copyapp_set_fp_print_quality( PRINTQUALITY_NORMAL );	// always 600 dpi
		copyapp_set_fp_input_tray( stGoCopy.inputTray );
		copyapp_set_fp_print_paper_size_info(stGoCopy.trayPaperInfo);
		copyapp_set_fp_collation( stGoCopy.collation );
		copyapp_set_fp_print_duplex( stGoCopy.printDuplex );
		copyapp_set_fp_scan_duplex( 0 );
		copyapp_set_fp_print_density( stGoCopy.density );
		copyapp_set_fp_resolution( 300 );
		
		//Prepare a MSG_STARTRECIPE message for Copy App
		StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
		ASSERT(StartIt != NULL);
		msg.msgType = MSG_STARTRECIPE;
		StartIt->Job = ejob_Copy;
		StartIt->AdditionalResource = 0;
		StartIt->Pipe = 0;
		StartIt->Wait = e_WaitForever;
		msg.param3 = StartIt;       // not freed here, freed by Message receiver

		/** Send a MSG_STARTRECIPE message to the Copy Application Thread telling it to kick off a copy **/
		err = SYMsgSend(COPYAPPID, &msg);         // send the message.
		XASSERT( err==OK, err );
		
		
	}
	else if (stGoCopy.action == COPY_ACTION_RESUME_SCAN)
	{	
		if(gCopyWaitUserInput){
			gCopyWaitUserInput = false;
			copyapp_copy_input(e_COPY_NEXT);
		}
	}
	else if (stGoCopy.action == COPY_ACTION_RESUME_PRINT)
	{
		//Send Printer to resume. When Copy job resumed due to 1. Paper_Mismatch 2. Drum_Low_3. In both case only print job stopped.
		//if((gCopySystemState == RSLT_STOPED_MISMATCH)	 || (gCopySystemState == RSLT_STOPED_DRUM)	){
		if(gStRetryPrintSystemStatus.retryCallback != NULL){
			(gStRetryPrintSystemStatus.retryCallback)(gStRetryPrintSystemStatus.retrySubject);
		}
		else{
			DBG_SSL_PRINTF_SDH_ERR("[%s]: Print RetryCallback == NULL. Can not resume printing.\n", __FUNCTION__);
		}
	}
//	else if (stGoCopy.action == COPY_ACTION_STOP)
	else if (stGoCopy.action == COPY_ACTION_COMPLETE)
	{
		copyapp_copy_input(e_COPY_FINISH);
	}
	else if (stGoCopy.action == COPY_ACTION_CANCEL)
	{
		copyapp_copy_input(e_COPY_CANCEL);
	}
#endif
}

void CopyAppEventService(SYS_UI_MESSAGE *Message)
{	
	SYS_UI_MESSAGE msgToSend;
	bool msgSendFlag = true;
	stCopy_GoCopyResult stGoCopyResult = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_COPY_GOCOPY_RES;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	copyapp_event_t copyAppEvent;
	memcpy(&copyAppEvent, Message->data, sizeof(copyapp_event_t));

	switch(copyAppEvent){
		case COPYAPP_EVENT_NULL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_NULL  \n", __FUNCTION__);
			msgSendFlag = false;
			break;
		case COPYAPP_EVENT_IDLE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_IDLE  \n", __FUNCTION__);
			stGoCopyResult.result = 0;
			msgSendFlag = false;
			break;
		case COPYAPP_EVENT_COPY_START:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_COPY_START  \n", __FUNCTION__);			
			//stGoCopyResult.result = 12;
			msgSendFlag = false;
			break;
		case COPYAPP_EVENT_COPY_RUNNING:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_COPY_RUNNING  \n", __FUNCTION__);
			//gCopyJobFromADF = false;
			//stGoCopyResult.result = RSLT_JOB_RESUMED;
			msgSendFlag = false;	//Because esjm_JobStart msg receiving after this msg
			break;
		case COPYAPP_EVENT_COPY_COMPLETE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_COPY_COMPLETE  \n", __FUNCTION__);
			gCopyJobFromADF = false;
			gCopyWaitUserInput = false;	//Clear flag: Wait for User input 
			//stGoCopyResult.result = RSLT_JOB_COMPLETED;
			msgSendFlag = false;
			break;
		case COPYAPP_EVENT_CANCEL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_CANCEL  \n", __FUNCTION__);
			stGoCopyResult.result = RSLT_JOB_CANCELED;
			break;
		case COPYAPP_EVENT_OUT_OF_MEMORY:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_OUT_OF_MEMORY  \n", __FUNCTION__);			
			stGoCopyResult.result = RSLT_STOPED_SC;
			break;
		case COPYAPP_EVENT_WAITING_FOR_USER_INPUT:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_WAITING_FOR_USER_INPUT  \n", __FUNCTION__);
			stGoCopyResult.result = RSLT_WAIT_USERINPUT;
			gCopyWaitUserInput = true;
			break;
		case COPYAPP_EVENT_WAITING_FOR_USER_INPUT_MERGE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_WAITING_FOR_USER_INPUT_MERGE  \n", __FUNCTION__);
			stGoCopyResult.result = RSLT_WAIT_USERINPUT_MERGE;
			gCopyWaitUserInput = true;
			break;
		case COPYAPP_EVENT_COPY_OFFLINE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_COPY_OFFLINE  \n", __FUNCTION__);
			//stGoCopyResult.result = 7;
			msgSendFlag = false;
			break;
		case COPYAPP_EVENT_COPY_FROM_ADF:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_COPY_FROM_ADF  \n", __FUNCTION__);
			gCopyJobFromADF = true;
			//stGoCopyResult.result = RSLT_JOB_RESUMED; //RUNNING
			msgSendFlag = false; //No need to send here.
			break;
		case COPYAPP_EVENT_PARAM_FAULT:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: Status = COPYAPP_EVENT_PARAM_FAULT  \n", __FUNCTION__);
			stGoCopyResult.result = RSLT_JOB_PARAM_FAULT;
			stGoCopyResult.sizeCode = (Message->data[9]*256) + Message->data[8];
			stGoCopyResult.failCode =  Message->data[4];
			break;
		default:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Error Status = %d not found  \n", __FUNCTION__);
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		stGoCopyResult.jobID = g_nCurrentCopyJobID;
		stGoCopyResult.source = gCopyJobFromADF;
		memcpy(msgToSend.data, &stGoCopyResult, sizeof(stCopy_GoCopyResult));
		CopyGoCopyResultService(&msgToSend); //Send filtered message. No repeated message.		
	}

}


void CopyGoCopyResultService(SYS_UI_MESSAGE *Message)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stCopy_GoCopyResult stCopyResult;
	bool msgSendFlag = true;

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_COPY_GOCOPY_RES;	//Go Copy result
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	memcpy(&stCopyResult, Message->data, sizeof(stCopy_GoCopyResult));	

	if(Message->msgSender == SJMID){
		switch(stCopyResult.result)
		{
			case esjm_JobReceived:
				stCopyResult.result = RSLT_JOB_ACCEPTED;	//Copy Job Accepeted. Job will start soon
				++g_nCopyJobCount;
				if(g_nCopyJobCount > 0){
					gCopyJobStatus = true;
					TimeoutControl(TIMEOUT_PAUSE);
				}
				DBG_SSL_PRINTF_SDH_ERR("JCCopy:%d\n", g_nCopyJobCount);
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalCopyJob=%d, Status = esjm_JobReceived, CopyJobStatus=%d  \n", __FUNCTION__, g_nCopyJobCount, gCopyJobStatus);
				break;
			case esjm_JobStart:
				stCopyResult.result = RSLT_JOB_RESUMED;	//Copy Job Running
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalCopyJob=%d, JobID=%d, Status = esjm_JobStart\n", __FUNCTION__, g_nCopyJobCount, stCopyResult.jobID);
				break;
			case esjm_JobDone:
				stCopyResult.result = RSLT_JOB_COMPLETED;	//Copy Job done or completed
				--g_nCopyJobCount;
				if(g_nCopyJobCount <= 0){
					gCopyJobStatus = false;
					TimeoutControl(TIMEOUT_RESET);
				}
				DBG_SSL_PRINTF_SDH_ERR("-JCCopy:%d\n", g_nCopyJobCount);
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalCopyJob=%d, JobID=%d, Status = esjm_JobDone  \n", __FUNCTION__, g_nCopyJobCount,  stCopyResult.jobID);
				NotifySystemJobDone();
				break;
			case esjm_JobCancel_at_device:
				stCopyResult.result = RSLT_JOB_CANCELING;
				break;
			case esjm_JobCancel_by_user:
				stCopyResult.result = RSLT_JOB_CANCELING;
				break;
			case esjm_JobCancelDone:
				stCopyResult.result = RSLT_JOB_CANCELED;
				break;
			default:
				msgSendFlag = false;
				break;
		}//End switch
	}
#if 0	
	else if(Message->msgSender ==JOBMGRID)
	{

		switch(stCopyResult.result)
		{				
			case COPYAPP_EVENT_COPY_START:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_COPY_START  \n", __FUNCTION__);		
				msgSendFlag = false;
				break;
			case COPYAPP_EVENT_COPY_COMPLETE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = COPYAPP_EVENT_COPY_COMPLETE  \n", __FUNCTION__);		
				msgSendFlag = false;
				break;				
			default:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status =  0x%X Error: Please check status value  \n", __FUNCTION__, stCopyResult.result);
				msgSendFlag = false;
				break;
		}	
	}
#endif
	else if(Message->msgSender == SYSTEM_SERVICE_ID){
		// Paper JAM, Paper Empty, Full Stack
		//Do nothing send same structure
	}
	
	else	{
		msgSendFlag = false;
	}
		
	if(msgSendFlag){
		if((gCopySystemState == RSLT_JOB_CANCELING) && (stCopyResult.result == RSLT_JOB_RESUMED)){
			DBG_SSL_PRINTF_SDH_ERR("  [ %s] Msg Old:%d Ignore New:%d \n", __FUNCTION__, gCopySystemState, stCopyResult.result);
			//JOB_CANCELING is in progress so Ignore JOB_RESUMED message.
		}
		else{
			if( (gCopySystemState == RSLT_WAIT_USERINPUT) && (stCopyResult.result == RSLT_JOB_RECOVERED) ){
				gSendWaitUserInput = true;
			}
			else if( stCopyResult.result == RSLT_JOB_COMPLETED){
				gSendWaitUserInput = false;
			}
			
			if(gCopySystemState != stCopyResult.result){
				if ( (stCopyResult.result != RSLT_JOB_PARAM_FAULT) && (stCopyResult.result != RSLT_JOB_FAILED) ){
					gCopySystemState = stCopyResult.result;
				}
				stCopyResult.source = gCopyJobFromADF;
				memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );

				//If user didn't respond to user input message but job stoped and resumed by any reason then send USERINPUT msg again
				if((gCopyWaitUserInput) && (gSendWaitUserInput) && (gCopySystemState == RSLT_JOB_RESUMED)){
					gSendWaitUserInput = false;
					if(gCopySystemState != RSLT_WAIT_USERINPUT){
						stCopyResult.result = RSLT_WAIT_USERINPUT;
						gCopySystemState = stCopyResult.result;
						memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
						err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
						XASSERT( err==OK, err );
					}
				}

				if ( IS_TFTLCD_ATTACHED ){
					if((gCopySystemState == RSLT_WAIT_USERINPUT) || (gCopySystemState == RSLT_STOPED_USER)
						|| (gCopySystemState == RSLT_STOPED_JAM) || (gCopySystemState == RSLT_STOPED_PAPEREMPTY)
						|| (gCopySystemState == RSLT_STOPED_FULLSTACK) || (gCopySystemState == RSLT_STOPED_MISMATCH)
						|| (gCopySystemState == RSLT_STOPED_SC) || (gCopySystemState == RSLT_STOPED_FRONTDOOROPEN)
						|| (gCopySystemState == RSLT_STOPED_DRUM) || (gCopySystemState == RSLT_WAIT_USERINPUT_MERGE)){
						
						if(g_nTFTBackLightOn == false){
							SetTFTPanelMode(TFT_FB_WAKEUP);
						}
					}
				}
			
				if(gCopySystemState == RSLT_STOPED_PAPEREMPTY){
					ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_PAPEREMPTY, stCopyResult.inTray);
				}
			}
			else if((stCopyResult.result == RSLT_JOB_ACCEPTED) || (stCopyResult.result == RSLT_WAIT_USERINPUT)
				|| (stCopyResult.result == RSLT_JOB_COMPLETED)){
				gCopySystemState = stCopyResult.result;
				memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );
			}
		}
	}
}


void SysJobTakeActionService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stIC_SysJobFakeAction stFakeAction = {0};
	stCopy_FakeCopyGoCopy stFakeCopy = {0};
	
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);
	memcpy(&stFakeAction, Message->data, sizeof(stIC_SysJobFakeAction));
	
	msgToSend.msgSender = SYSTEM_SERVICE_ID;	
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	switch(stFakeAction.jobType){
		case 1: //Fake Copy
			stFakeCopy.action = stFakeAction.action;
			msgToSend.cmd = CMD_SU_FAKE_COPY_GOCOPY;
			memcpy(msgToSend.data, &stFakeCopy, sizeof(stCopy_FakeCopyGoCopy));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );
			break;
		default:
			break;
	}
}

void SendSpecialNotification(int type, int option)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stSpecial_Notification stSpecialNotify = {0};

	DBG_SSL_PRINTF_SDH_DEBUG("[%s] type:%d option:%d\n", __FUNCTION__, type, option);

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	msgToSend.cmd = CMD_SU_SPECIAL_NOTIFICATION;

	stSpecialNotify.type = type;
	stSpecialNotify.option = option;
	memcpy(msgToSend.data, &stSpecialNotify, sizeof(stSpecial_Notification));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void SpecialNotificationService(SYS_UI_MESSAGE *Message)
{
	stSpecial_Notification stSpecialNotify = {0};
	memcpy(&stSpecialNotify, Message->data, sizeof(stSpecial_Notification));

	//Just forward message to UI for now
	SendSpecialNotification(stSpecialNotify.type, stSpecialNotify.option);
}

void ScanCounterService(int scanType, int scanCount)
{
	uint32_t scannedCounter = 0;

	SCAN_COUNTER_STRUCT stScanCounter = {0};
	error_type_t db_retval;
	int FBScanCount = 0;
	int A3ScanCount = 0;
	int ADFSimplexScanCount = 0;
	int ADFDuplexScanCount = 0;
	
	//read scanned count from 'scanman'
	scannedCounter = scanman_scanner_counter_read();

	switch(scanType){
		case FB_SCAN_COUNT:
			FBScanCount = scannedCounter;
			DBG_SSL_PRINTF_SDH_ERR("%s: ScanCount Up: FB(%d)\n", __FUNCTION__, FBScanCount);
			break;
		case FB_A3_SCAN_COUNT:
			FBScanCount = scannedCounter*2;
			A3ScanCount = scannedCounter;
			DBG_SSL_PRINTF_SDH_ERR("%s: ScanCount Up: FB_A3(%d)\n", __FUNCTION__, FBScanCount);
			break;
		case FB_LEGAL_SCAN_COUNT:
			FBScanCount = scannedCounter*2;
			break;
		case ADF_SIMPLEX_SCAN_COUNT:
			ADFSimplexScanCount = scannedCounter;
			DBG_SSL_PRINTF_SDH_ERR("%s: ScanCount Up: ADF(F)(%d)\n", __FUNCTION__, ADFSimplexScanCount);
			break;
		case ADF_DUPLEX_SCAN_COUNT:
			ADFSimplexScanCount = scannedCounter;
			ADFDuplexScanCount = scannedCounter/2;
			DBG_SSL_PRINTF_SDH_ERR("%s: ScanCount Up: ADF(B)(%d)\n", __FUNCTION__, ADFDuplexScanCount);
			break;
		default:
			break;
	}

	if(FBScanCount || A3ScanCount || ADFSimplexScanCount || ADFDuplexScanCount){
		db_retval = db_get_scancount(&stScanCounter);
		if(db_retval == FAIL){
			DBG_SSL_PRINTF_SDH_ERR("%s: Fail to get DB data: ScanCounter\n", __FUNCTION__);
		}
		else{
			stScanCounter.scan += (FBScanCount + ADFSimplexScanCount);
			stScanCounter.fbscan += FBScanCount;
			stScanCounter.a3scan += A3ScanCount;
			stScanCounter.adfscan += ADFSimplexScanCount;
			stScanCounter.adfduplex += ADFDuplexScanCount;

			db_retval = db_set_scancount(stScanCounter);
			if(db_retval == FAIL){
				DBG_SSL_PRINTF_SDH_ERR("%s: Fail to set DB data: ScanCounter\n", __FUNCTION__);
			}
		}
	}

	DBG_SSL_PRINTF_SDH_ERR("Update Scan Counter[%d][%d]: FB(%d) FB_A3(%d) ADF_S(%d) ADF_D(%d)\n", scanType, scannedCounter, FBScanCount, A3ScanCount, ADFSimplexScanCount, ADFDuplexScanCount);
}

uint32_t GetScanPJLStatusCode(void)
{
	return gScanSystemStatus;
}

void SetScannerStatus(bool isSenderScanMan, uint32_t statusCode)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s] Sender=%d Code=%d\n", __FUNCTION__, isSenderScanMan, statusCode);
	uint32_t curScannerStatus = 0;
	bool shmWriteFlag = true;
	if(isSenderScanMan){
		switch(statusCode){
			case SCANMAN_EVENT_IDLE:
			case SCANMAN_EVENT_SCAN_COMPLETE:
				curScannerStatus = 60001;
				break;
			case SCANMAN_EVENT_SCAN_OFFLINE:
				curScannerStatus = 60002;
				break;
			//case SCANMAN_EVENT_SCAN_CALIBRATION_RUNNING:
			//case SCANMAN_EVENT_SCAN_CALIBRATION_COMPLETE:
				//break;
			case SCANMAN_EVENT_SCAN_CANCEL:
				curScannerStatus = 60007;
				break;
			case SCANMAN_EVENT_SCAN_START:
			case SCANMAN_EVENT_SCAN_RUNNING:
				curScannerStatus = 60014;
				break;

			case SCANMAN_EVENT_ADF_PAPER_JAM:
				if(gScanJamStatus){
					curScannerStatus = 62000;
				}
				else{
					curScannerStatus = 60001;
				}
				break;
			case SCANMAN_EVENT_ADF_TOP_COVER_OPEN:
				if(gScanADFTopCoverOpen){
					curScannerStatus = 62200;
				}
				else{
					curScannerStatus = 60001;
				}
				break;
			default:
				shmWriteFlag = false;
				break;
		}
	}
	else{
		if(!gScanJamStatus){
			switch(statusCode){
#if 0
				case SCANAPP_EVENT_IDLE:
				case SCANAPP_EVENT_SCAN_COMPLETE:
					curScannerStatus = 60001;
					break;
				case SCANAPP_EVENT_SCAN_OFFLINE:
					curScannerStatus = 60002;
					break;
				case SCANAPP_EVENT_SCAN_START:
				case SCANAPP_EVENT_SCAN_RUNNING:
					curScannerStatus = 60014;
					break;
#endif
				case SCANAPP_EVENT_HOST_CANCEL:
				case SCANAPP_EVENT_DEVICE_CANCEL:
					curScannerStatus = 60007;
					break;
				case SCANAPP_EVENT_WAITING_FOR_USER_INPUT:
				case SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_UISCAN:
				case SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_PCSCAN:
					curScannerStatus = 60029;
					break;
				case SCANAPP_EVENT_TIMEOUT:
				case SCANAPP_EVENT_SCAN_PAUSED:
				case SCANAPP_EVENT_SCAN_FAILED_FB_EMPTY_FOR_MERGE:
					shmWriteFlag = false;
					break;
				default:
					shmWriteFlag = false;
					break;
			}
		}
	}

	if(shmWriteFlag){
		if(curScannerStatus != gScanSystemStatus){
			gScanSystemStatus = curScannerStatus;
			DBG_SSL_PRINTF_SDH_DEBUG("[%s] CurStatus=%d=0x%X\n", __FUNCTION__, curScannerStatus, curScannerStatus);
			//SHM: Write Scanner status to Shared Memory so other process(like web server) can use it.
			if(g_pSHM_ScanStatus){
				memcpy(g_pSHM_ScanStatus, &curScannerStatus, sizeof(uint32_t));
			}
		}
	}
}

void ScanFailedError(UINT8 err_no)
{
	SYS_UI_MESSAGE msgToSend;
	error_type_t err;
	DBG_SSL_PRINTF_SDH_ERR("SCAN failed due to : %d\n", err_no);
	stScan_GoScanResult stScanResult = {0};
	stScanResult.result = SCN_RSLT_JOB_FAILED;
	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_SCAN_GOSCAN_RES;
	memcpy(msgToSend.data, &stScanResult, sizeof(stScan_GoScanResult));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void ScanAppNotify(int notifyType)
{
	MESSAGE send_msg;
	error_type_t err;
	memset(&send_msg, 0x00, sizeof(send_msg));
	
	if(notifyType == UDB_STATUS_REMOVE) {
		send_msg.msgType = MSG_NITIFY_USBTHUMB_DISCONNECT;
		err = SYMsgSend(SCANTOEMAILAPPID, &send_msg);
	}
}

void ScanAppService(SYS_UI_MESSAGE *Message)
{
#if HAVE_SCAN_SUPPORT

	if(g_nEnergySaveMode){
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Error: Do not allow UI Scan job in Power Save mode\n", __FUNCTION__);
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Error: Ignoring UI Scan job request\n", __FUNCTION__);
		//Send message to UI for Scan Job failed
		ScanFailedError(1);
		return;
	}

	error_type_t err;
	
	stScan_GoScan stGoScan;
	memset(&stGoScan, 0x00, sizeof(stScan_GoScan));
	memcpy(&stGoScan, Message->data, sizeof(stScan_GoScan));

#if 1
	DBG_SSL_PRINTF_SDH_ERR("act         : %d\n", stGoScan.act);
	DBG_SSL_PRINTF_SDH_ERR("dpi(hori)   : %d\n", stGoScan.dpi_hori);
	DBG_SSL_PRINTF_SDH_ERR("dpi(vert)   : %d\n", stGoScan.dpi_vert);
	DBG_SSL_PRINTF_SDH_ERR("border(L)   : %d\n", stGoScan.border_left);
	DBG_SSL_PRINTF_SDH_ERR("border(R)   : %d\n", stGoScan.border_right);
	DBG_SSL_PRINTF_SDH_ERR("border(T)   : %d\n", stGoScan.border_top);
	DBG_SSL_PRINTF_SDH_ERR("border(B)   : %d\n", stGoScan.border_bottom);
	DBG_SSL_PRINTF_SDH_ERR("sizecode    : %d\n", stGoScan.sizecode);
	DBG_SSL_PRINTF_SDH_ERR("brightness  : %d\n", stGoScan.brightness);
	DBG_SSL_PRINTF_SDH_ERR("color       : %d\n", stGoScan.color);
	DBG_SSL_PRINTF_SDH_ERR("fileformat  : %d\n", stGoScan.fileformat);
	DBG_SSL_PRINTF_SDH_ERR("pagetype    : %d\n", stGoScan.pagetype);
	DBG_SSL_PRINTF_SDH_ERR("source      : %d\n", stGoScan.source);
	DBG_SSL_PRINTF_SDH_ERR("contrast    : %d\n", stGoScan.contrast);
	DBG_SSL_PRINTF_SDH_ERR("quantiLevel : %d\n", stGoScan.quantlevel);
	DBG_SSL_PRINTF_SDH_ERR("destination : %d\n", stGoScan.destination);
#endif

	if(stGoScan.act == SCAN_ACTION_START ||
		stGoScan.act == SCAN_ACTION_A3MERGE ||
		stGoScan.act == SCAN_ACTION_SOL_SCAN_START ||
		stGoScan.act == SCAN_ACTION_CONNECTION_TEST) {
		/* when 'stGoScan.act' is 'SCAN_ACTION_SOL_SCAN_START', It means this scan job is for personal job
		 * and when sending FTP, send .end file and scanned file
		 *  for example : scanned file --> test.pdf, then test.end file is attached together
		 */
		//Data Structure for Transfering SCAN Setting from UIManager to ScanApp.
		UISCAN_JOB_SETTINGS   *scanJobSettings;
		uint32_t scan_width_mm = 0; 
		uint32_t scan_height_mm = 0;
		STARTRECIPE         *StartIt;
		MESSAGE setupMsg;
		MESSAGE cmdMsg;
		int shm_ret = 0;
#if SDH_USE_SET_FILE_INFO
		stShmSaveFile *saveFileIinfo = NULL;
#endif
		stShmSendEmail *sendEmailInfo = NULL;
		stShmSendServer *sendServerInfo = NULL;

#if 1
		/*
		 ***********************************************************************
		 * Check : FB scan didn't support 'US Folio' Size
		 ***********************************************************************
		 */
		if( /*stGoScan.sizecode == MEDIASIZE_LEGAL || */ stGoScan.sizecode == MEDIASIZE_CUSTOM_85x13) {
			// ADF paper set check
			if (!scanner_check_adf_paper()) {
				DBG_SSL_PRINTF_SDH_DEBUG("FB Scan didn't support Size(%d)\n", stGoScan.sizecode);
				int shm_ret;
				shm_ret = shm_remove(stGoScan.addInfo);
				if ( shm_ret < 0 ) {
					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_remove error: \n", __FUNCTION__);
				}
				scanapp_log_event(SCANAPP_EVENT_SCAN_FAILED_FB_NOT_SUPPORT_SIZE);
				return;
			}
		}
#endif

		/*
		 ***********************************************************************
		 * Check : When using memory stick, check mounted
		 ***********************************************************************
		 */
		if(stGoScan.destination == UI_FILE_DEST_USBTHUMB ) {
			char *firstSearchingDir;
			int notShowPosition;
			firstSearchingDir = findingUSBThumb( &notShowPosition );
			if (firstSearchingDir == NULL) {
				int shm_ret;
				bool isSDCard = FALSE;
				DBG_SSL_PRINTF_SDH_DEBUG("USB Thumbdrive NOT Connected\n");

// check SD card
				saveFileIinfo = shm_attach(stGoScan.addInfo);
				if ( saveFileIinfo != (void *)-1 ) {
					if(saveFileIinfo->mountedPath != NULL) {
						DBG_SSL_PRINTF_SDH_ERR("saveFileIinfo->mountedPath  : %s\n", saveFileIinfo->mountedPath);
						if ( strncmp( saveFileIinfo->mountedPath, "/mnt/sd/", 8 ) == 0 )
						{
							isSDCard = TRUE;
						}
					}
					shm_ret = shm_detach(saveFileIinfo);
					if ( shm_ret < 0 ) {
						DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm detach error: \n", __FUNCTION__);
					}
				}
				
				if (isSDCard == FALSE)
				{
					shm_ret = shm_remove(stGoScan.addInfo);
					if ( shm_ret < 0 ) {
						DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_remove error: \n", __FUNCTION__);
					}
					scanapp_log_event(SCANAPP_EVENT_SCAN_FAILED_USBTHUMB_NOT_CONNECT);
					return;
				}
			}
		}

		/*
		 ***************************************************************************************************
		 * Check : Fix the setting value of Merge Scan(A3, 11X17-Double Letter) 
		 ***************************************************************************************************
		 */
		if(stGoScan.act == SCAN_ACTION_A3MERGE) {
			// ADF paper set check
			if (scanner_check_adf_paper()) {
				DBG_SSL_PRINTF_SDH_DEBUG("A3 merge Scan: Remove adf paper\n");
				int shm_ret;
				shm_ret = shm_remove(stGoScan.addInfo);
				if ( shm_ret < 0 ) {
					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_remove error: \n", __FUNCTION__);
				}
				scanapp_log_event(SCANAPP_EVENT_SCAN_FAILED_ADF_PAPER_IN_MERGE);
				return;
			}

			stGoScan.dpi_hori = 300;
			stGoScan.dpi_vert = 300;
			stGoScan.source = UI_SCAN_SOURCE_FB;
			stGoScan.color = UI_SCANTYPE_GRAY;
		}

		/********************************************************************************************
		 * have to setup the scan parameter received from UIManager
		 * set ScanConfig received from UI process. (Temporary Setup : Default)
		 *********************************************************************************************/
		scanJobSettings = (UISCAN_JOB_SETTINGS *) MEM_MALLOC(sizeof(UISCAN_JOB_SETTINGS)); //Free at : "scanapp_uiscan.c" --> "case MSG_SETTING_CHANGED:"
		memset( scanJobSettings, 0x00, sizeof(UISCAN_JOB_SETTINGS) );
		ASSERT(scanJobSettings != NULL);


		/// Check UI Setting Value is avariable or not, if not avariable modify the setting value.
		if((stGoScan.dpi_hori == 600) && 
			(stGoScan.color == UI_SCANTYPE_COLOR) &&
			(stGoScan.source == UI_SCAN_SOURCE_ADF_DUPLEX_TOPTOP || stGoScan.source == UI_SCAN_SOURCE_ADF_DUPLEX_TOPBOTTOM)) {
			/*SINDOH Spec : Not Support (600dpi & Color & Duplex) Mode */
			stGoScan.dpi_hori = 300;
			stGoScan.dpi_vert = 300;
			DBG_SSL_PRINTF_SDH_ERR("=========================== Change The UI Setting Value dpi -> changed to : %d\n", stGoScan.dpi_hori);
			DBG_SSL_PRINTF_SDH_ERR("dpi -> changed to : %d\n", stGoScan.dpi_hori);
		}
		
		///< Border Erase >  (Default 2mm padding executed in scan app)
		scanJobSettings->eraseBorderParam.userInputBottom 	= MAX(0, stGoScan.border_bottom - DEFAULT_BORDER_MM);	//*1mm
		scanJobSettings->eraseBorderParam.userInputLeft 		= MAX(0, stGoScan.border_left - DEFAULT_BORDER_MM);		//*1mm
		scanJobSettings->eraseBorderParam.userInputTop 		= MAX(0, stGoScan.border_top - DEFAULT_BORDER_MM);		//*1mm
		scanJobSettings->eraseBorderParam.userInputRight		= MAX(0, stGoScan.border_right - DEFAULT_BORDER_MM);		//*1mm
		
		///< 1. Brightness Values > 
		///   Mono : (20~180,step:20), UI Setting : 1(Bright) ~ 9(Darker)
		///   Color: (20~180,step:20),  UI Setting : 1(Bright) ~ 9(Darker)		
		///< 2. Gamma Values from 0 - 99 (Color Mode:22, Gray Mode:19)
		if(stGoScan.color == UI_SCANTYPE_COLOR) {
			scanJobSettings->Brightness = (10 - stGoScan.brightness) * 20;
			if( scanJobSettings->Brightness < 20 || scanJobSettings->Brightness > 180 ) {
				scanJobSettings->Brightness = 100; //default value
			}
			scanJobSettings->Gamma = SDH_SCAN_COLOR_GAMMA_VAL;
		}
		else {
			scanJobSettings->Brightness = (10 - stGoScan.brightness) * 20;
			if( scanJobSettings->Brightness < 20 || scanJobSettings->Brightness > 180 ) {
				scanJobSettings->Brightness = 100; //default value
			}
			scanJobSettings->Gamma = SDH_SCAN_GRAY_GAMMA_VAL;
		}
		
		///< Contrast Values > (20~100,step:20)
		scanJobSettings->Contrast = stGoScan.contrast * 20;
		if( scanJobSettings->Contrast < 0 || scanJobSettings->Contrast > 100 ) {
			scanJobSettings->Contrast = 100; //default value
		}

		/// Until UIManger Menu Configured, Default Setup.
		scanJobSettings->Contrast = 100; //default value
		
		///< Resolution > < Resolution any number from 75 to 1200 > : Pinetree <75,100,150,200,300,600>
		if ( stGoScan.dpi_hori == stGoScan.dpi_vert ) {
			scanJobSettings->Resolution	= stGoScan.dpi_hori;
		}
		else {
			scanJobSettings->Resolution			= 300;
		}
		
		scanJobSettings->XYScale.x_numerator		= 1;
		scanJobSettings->XYScale.x_denominator		= 1;
		scanJobSettings->XYScale.y_numerator		= 1;
		scanJobSettings->XYScale.y_denominator		= 1;
		scanJobSettings->Sharp						= 100;	///< Sharp values range from [0,200] 0 max smooth, 100 no change, 200 big sharpen
		scanJobSettings->deprecated1				= 0;	///< deprecated1 Not currently used->
		scanJobSettings->BitsPerPixel				= 8;	///< BitsPerPixel Value 8
		scanJobSettings->reserved1					= 0;	///< reserved1
		scanJobSettings->reserved2					= 0;	///< reserved2
		scanJobSettings->reserved3					= 0;	///< reserved3
		
		///< SCAN_JOB_SETTINGS_DOC >
		switch( stGoScan.source ) {
			case UI_SCAN_SOURCE_AUTO:
				scanJobSettings->scanSource = SCAN_JOB_SETTINGS_DOC_SRC_AUTO;
				break;
			case UI_SCAN_SOURCE_FB:
				scanJobSettings->scanSource = SCAN_JOB_SETTINGS_DOC_SRC_FLATBED;
				break;
			case UI_SCAN_SOURCE_ADF:
				scanJobSettings->scanSource = SCAN_JOB_SETTINGS_DOC_SRC_ADF;
				break; 
			case UI_SCAN_SOURCE_ADF_DUPLEX_TOPTOP:
			case UI_SCAN_SOURCE_ADF_DUPLEX_TOPBOTTOM: 
				scanJobSettings->scanSource = SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX;
				break;
			default:
				scanJobSettings->scanSource = SCAN_JOB_SETTINGS_DOC_SRC_AUTO;
		}
	 
		///< Scan Original Size > : Transfered size information multiplexing 100 of inch
		//Paper Size : 1inch == 25.4mm, DPI : Dot Per Inch (Letter 8.5inch * 300 = 2550 pxl)
		/* 
		********************************************************
		* A4 		: 210*297(mm*mm), 8.3*11.7(inch)
		* Letter 	: 216*279(mm*mm), 8.5*11.0(inch)
		********************************************************* 
		*/
		///< sizecode Values >
		scanJobSettings->sizecode = stGoScan.sizecode;
		if( paper_get_media_size( stGoScan.sizecode, &scan_width_mm, &scan_height_mm ) == OK ) {
			scanJobSettings->scan_window.right = (scan_width_mm * 100) / MMx100_PER_INCH;
			scanJobSettings->scan_window.bottom = (scan_height_mm * 100) / MMx100_PER_INCH;
			scanJobSettings->ScannableArea.right = (scan_width_mm * 100) / MMx100_PER_INCH;
		}
		else {
			scanJobSettings->scan_window.right = 850;//Letter, 825;//A4
			scanJobSettings->scan_window.bottom = 1100;//Letter, 1170//A4
			scanJobSettings->ScannableArea.right = 850;//Letter, 825//A4			
		}
		
		/// When Scan With ADF, we have to consider ADF Offset. 'input_horizontal_offset' parm used only for ADF Scan
		/// This horizontal offset value is used in UIScan for <setup_scan_area> only in ADF Scan
		if( scanJobSettings->scan_window.right < MAX_SCAN_ADF_WIDTH_HINCHES ) {
			scanJobSettings->input_horizontal_adfOffset = (MAX_SCAN_ADF_WIDTH_HINCHES - scanJobSettings->scan_window.right)/2;
		}
		else {
			scanJobSettings->input_horizontal_adfOffset = 0;
		}
		
		scanJobSettings->scan_window.top            = 0; 		///< scan_window in hundredths of an inch (top,left,bottom,right)
		scanJobSettings->scan_window.left           = 0;
		scanJobSettings->ScannableArea.top          = 0; 		///< ScannableArea in hundredths of an inch
		scanJobSettings->ScannableArea.left         = 0;
		scanJobSettings->ScannableArea.bottom       = -1;

		switch(stGoScan.color)
		{
		case UI_SCANTYPE_COLOR:
			scanJobSettings->DataType = SCAN_DATA_TYPE_RGB;
			scanJobSettings->ScanType = e_ColorScan;
			break;
		case UI_SCANTYPE_GRAY:
			scanJobSettings->DataType = SCAN_DATA_TYPE_MONO;
			scanJobSettings->ScanType = e_MonochromeScan;
			break;
		case UI_SCANTYPE_BW_TEXT:
			scanJobSettings->DataType = SCAN_DATA_TYPE_BILEVEL;
			scanJobSettings->ScanType = e_MonochromeScan;
			break;
		case UI_SCANTYPE_BW_PHOTO:
			scanJobSettings->DataType = SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION;
			scanJobSettings->ScanType = e_MonochromeScan;
			break;
		default:
			scanJobSettings->DataType = SCAN_DATA_TYPE_MONO;
			scanJobSettings->ScanType = e_MonochromeScan;
			break;
		}
		
		///< file format Values >
		if(stGoScan.pagetype == UI_FILE_PAGE_SINGLE) {
			scanJobSettings->pageType = UI_FILE_PAGE_SINGLE;
			switch(stGoScan.fileformat) 
			{
			case UI_FILE_TIFF:
				scanJobSettings->fileFormat = UI_FILE_TIFF;
				break;
			case UI_FILE_PDF:
				scanJobSettings->fileFormat = UI_FILE_PDF;
				scanJobSettings->isSecurityDocument = false;
				break;
			case UI_FILE_JPEG:
				scanJobSettings->fileFormat = UI_FILE_JPEG;
				break;
			case UI_FILE_SECURITY_PDF:
				scanJobSettings->fileFormat = UI_FILE_PDF;				
				scanJobSettings->isSecurityDocument = true;
				break;
			default:
				break;
			}
		}
		else if(stGoScan.pagetype == UI_FILE_PAGE_MULTI) {
			scanJobSettings->pageType = UI_FILE_PAGE_MULTI;
			switch(stGoScan.fileformat) 
			{
			case UI_FILE_TIFF:
				scanJobSettings->fileFormat = UI_FILE_TIFF_MULTI_PAGE;
				break;
			case UI_FILE_PDF:
				scanJobSettings->fileFormat = UI_FILE_PDF_MULTI_PAGE;
				scanJobSettings->isSecurityDocument = false;
				break;
			case UI_FILE_JPEG:
				scanJobSettings->fileFormat = UI_FILE_JPEG;
				break;
			case UI_FILE_SECURITY_PDF:
				scanJobSettings->fileFormat = UI_FILE_PDF_MULTI_PAGE;
				scanJobSettings->isSecurityDocument = true;	
				break;
			default:
				break;
			}		
		}
		
		///< Personal Scan Job : affect only fo FTP send>
		if( stGoScan.act == SCAN_ACTION_SOL_SCAN_START ) {
			scanJobSettings->isPersonalScanJob = TRUE;
		}
		else {
			scanJobSettings->isPersonalScanJob = FALSE;
		}
		
		///< JPEG file quality Values >
		scanJobSettings->jpegQuality = stGoScan.quantlevel;
		
		///< destination Values >		
		scanJobSettings->scanToDestination = stGoScan.destination;

		///<Shared momory attach & remove, created in 'UIManager'>
		/*
		 ***********************************************************************************************************************
		 * 1.Attach shared memory, copy memory (shared memory is created in UIManager and Deleted in System)
		 ************************************************************************************************************************
		 */
		switch(stGoScan.destination)
		{
		case UI_FILE_DEST_USBTHUMB:
#if SDH_USE_SET_FILE_INFO
			saveFileIinfo = shm_attach(stGoScan.addInfo);
			if ( saveFileIinfo != (void *)-1 ) {
				if(saveFileIinfo->mountedPath != NULL) {
					DBG_SSL_PRINTF_SDH_ERR("saveFileIinfo->mountedPath  : %s\n", saveFileIinfo->mountedPath);
				}
				if(saveFileIinfo->fileName != NULL) {
					DBG_SSL_PRINTF_SDH_ERR("saveFileIinfo->fileName     : %s\n", saveFileIinfo->fileName);
				}
				memcpy(&scanJobSettings->saveToFileInfo, saveFileIinfo, sizeof(stShmSaveFile));
			}
			else {
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_attach error: \n", __FUNCTION__);				
			}
#endif
			break;
		case UI_FILE_DEST_EMAIL:
			sendEmailInfo = shm_attach(stGoScan.addInfo);
			if ( sendEmailInfo != (void *)-1 ) {
				DBG_SSL_PRINTF_SDH_ERR("sendEmailInfo->emailSenderAddress    : %s\n", sendEmailInfo->emailSenderAddress);
				DBG_SSL_PRINTF_SDH_ERR("sendEmailInfo->emailReceiverAddress  : %s\n", sendEmailInfo->emailReceiverAddress);
				DBG_SSL_PRINTF_SDH_ERR("sendEmailInfo->emailSubject          : %s\n", sendEmailInfo->emailSubject);
				DBG_SSL_PRINTF_SDH_ERR("sendEmailInfo->emailText             : %s\n", sendEmailInfo->emailText);
				if(sendEmailInfo->fileName != NULL) {
					DBG_SSL_PRINTF_SDH_ERR("saveFileIinfo->fileName     : %s\n", sendEmailInfo->fileName);
				}
				memcpy(&scanJobSettings->sendToEmailInfo, sendEmailInfo, sizeof(stShmSendEmail));
			}
			else {
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_attach error: \n", __FUNCTION__);				
			}
			break;
		case UI_FILE_DEST_FTP:
		case UI_FILE_DEST_SMB:
		case UI_FILE_DEST_WEBDAV:
			sendServerInfo = shm_attach(stGoScan.addInfo);
			if ( sendServerInfo != (void *)-1 ) {
				DBG_SSL_PRINTF_SDH_ERR("sendServerInfo->serverAddress  : %s\n", sendServerInfo->serverAddress);
				DBG_SSL_PRINTF_SDH_ERR("sendServerInfo->serverPath     : %s\n", sendServerInfo->serverPath);
				DBG_SSL_PRINTF_SDH_ERR("sendServerInfo->serverId       : %s\n", sendServerInfo->serverId);
				DBG_SSL_PRINTF_SDH_ERR("sendServerInfo->serverPassword : %s\n", sendServerInfo->serverPassword);
				DBG_SSL_PRINTF_SDH_ERR("sendServerInfo->serverPort     : %d\n", sendServerInfo->serverPort);
				if(sendServerInfo->fileName != NULL) {
					DBG_SSL_PRINTF_SDH_ERR("saveFileIinfo->fileName     : %s\n", sendServerInfo->fileName);
				}
				memcpy(&scanJobSettings->sendToServerInfo, sendServerInfo, sizeof(stShmSendServer));
			}
			else {
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_attach error: \n", __FUNCTION__);				
			}
			break;
		default:
			break;
		}
		
		/*
		 ***********************************************************************************************************************
		 * 2. Detach & Remove shared memory
		 ************************************************************************************************************************
		 */
		switch(stGoScan.destination) {
			case UI_FILE_DEST_USBTHUMB:
				shm_ret = shm_detach(saveFileIinfo);
				break;
			case UI_FILE_DEST_EMAIL:
				shm_ret = shm_detach(sendEmailInfo);
				break;
			case UI_FILE_DEST_FTP:
			case UI_FILE_DEST_SMB:
			case UI_FILE_DEST_WEBDAV:
				shm_ret = shm_detach(sendServerInfo);
				break;
			default:
				break;
		}
		if ( shm_ret < 0 ) {
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm detach error: \n", __FUNCTION__);
		}

		shm_ret = shm_remove(stGoScan.addInfo);
		if ( shm_ret < 0 ) {
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: shm_remove error: \n", __FUNCTION__);
		}
		
		if(stGoScan.act == SCAN_ACTION_CONNECTION_TEST) {
			//Prepare a MSG_CONNECT_TEST message for scan connection test.
			setupMsg.msgType = MSG_CONNECT_TEST;
			setupMsg.param3 = scanJobSettings;
			err = SYMsgSend(SCANTOEMAILAPPID, &setupMsg);
			XASSERT( err==OK, err );			
		}
		else {
			/************************************************************************************************************
			 * Send a MSG_SETTING_CHANGED message to the Scan Application Thread telling config scan job
			 ************************************************************************************************************/
			//Prepare a MSG_SETTING_CHANGED message for scan config.
			setupMsg.msgType = MSG_SETTING_CHANGED;
			setupMsg.param3 = scanJobSettings;
			err = SYMsgSend(SCANTOEMAILAPPID, &setupMsg);
			XASSERT( err==OK, err );

			/*************************************************************************************************************
			 * Send a MSG_STARTRECIPE message to the Scan Application Thread telling it to kick off a scan
			 *************************************************************************************************************/
			//Prepare a MSG_STARTRECIPE message for Start Scan Job.
			StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
			ASSERT(StartIt != NULL);
			cmdMsg.msgType = MSG_STARTRECIPE;
			StartIt->Job = ejob_ScanToEmail;
			StartIt->AdditionalResource = 0;
			StartIt->Pipe = 0;
			StartIt->Wait = e_WaitForever;
			cmdMsg.param3 = StartIt;    // not freed here, freed by Message receiver

			err = SYMsgSend(SCANTOEMAILAPPID, &cmdMsg);    // send the message.
			XASSERT( err==OK, err );
		}
	}
	else if(stGoScan.act == SCAN_ACTION_RESUME)
	{
		/* Scan Job Resume (Scan Next Page) */
		scanner_ui_scan_input(SCAN_ACTION_RESUME);
	}
	else if(stGoScan.act == SCAN_ACTION_STOP)
	{
		/* Scan Job Stop */
		scanner_ui_scan_input(SCAN_ACTION_STOP);
	}	
	else if( stGoScan.act == SCAN_ACTION_CANCEL ) 
	{	//UI Panel <Stop/Cance> Key Mapping
		/* Scan Job Cancel */
		scanner_ui_scan_input( SCAN_ACTION_CANCEL );
#if SDH_USE_PC_SCAN_CANCEL_BY_UI
		scanner_pc_scan_input( SCAN_ACTION_CANCEL );
#endif
	}
	else if( stGoScan.act == SCAN_ACTION_FINISH )
	{	//UI Panel <#> Key Mapping
		/* Scan Job Finish */
		scanner_ui_scan_input( SCAN_ACTION_FINISH );		
	}
#endif //HAVE_SCAN_SUPPORT
}

/**
 * \brief get scanapp_event_t information from  Observer_t (SCAN) event callback and
 * send correct messgae (SYS_UI_MESSAGE) to UIManager to display status of ScanApp.
 *
 * \param SYS_UI_MESSAGE : The message which event callback fuction sent including event data.
 * \param
 *
 */
void ScanAppEventService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	bool msgSendFlag = true;

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_SCAN_GOSCAN_RES;	//GoScan Result
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stScan_GoScanResult goGscanResult = {0};
	UINT32 scanJobType = 0;
	scanapp_event_t scanAppEvent;
	memcpy(&scanAppEvent, Message->data, sizeof(scanapp_event_t));

	if(Message->msgSender == SJMID){
		memcpy(&goGscanResult, Message->data, sizeof(stScan_GoScanResult));
		scanJobType = goGscanResult.page;
		switch(goGscanResult.result){
			case esjm_JobReceived:
				msgSendFlag = false;
				++g_nScanJobCount;
				if(g_nScanJobCount > 0){
					gScanJobStatus = true;
					TimeoutControl(TIMEOUT_PAUSE);
				}
				DBG_SSL_PRINTF_SDH_ERR("JCScan:%d %s\n", g_nScanJobCount, to_string_job_types(scanJobType));
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalScanJob=%d, Status = esjm_JobReceived, ScanJobStatus=%d	\n", __FUNCTION__, g_nScanJobCount, gScanJobStatus);
				break;
			case esjm_JobStart:
				msgSendFlag = false;
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalScanJob=%d, Status = esjm_JobStartn", __FUNCTION__, g_nScanJobCount);
				break;
			case esjm_JobDropped:
			case esjm_JobDone:
				msgSendFlag = false;
				--g_nScanJobCount;
				if(g_nScanJobCount <= 0){
					gScanJobStatus = false;
					TimeoutControl(TIMEOUT_RESET);
				}
				//SCAN job countUP
				if ((goGscanResult.result == esjm_JobDone) && g_pStScanJobDoneInfo) {
					if(scanJobType == ejob_ScanToEmail){
						SendJobDoneInfo(SYSTEM_SERVICE_ID, g_pStScanJobDoneInfo, AUTH_JOB_SCAN_TO_X, AUTH_SOL_JOB_OK);
					}
					else{
						SendJobDoneInfo(SYSTEM_SERVICE_ID, g_pStScanJobDoneInfo, AUTH_JOB_SCAN, AUTH_SOL_JOB_OK);
					}
				}
				DBG_SSL_PRINTF_SDH_ERR("-JCScan:%d %s\n", g_nScanJobCount, to_string_sjm_jobstate_enum(goGscanResult.result));
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalScanJob=%d, Status = %s\n", __FUNCTION__, g_nScanJobCount, to_string_sjm_jobstate_enum(goGscanResult.result));
				NotifySystemJobDone();
				break;
			case esjm_JobCancel_at_device:
				msgSendFlag = false;	
				break;
			case esjm_JobCancel_by_user:
				msgSendFlag = false;			
				break;
			case esjm_JobCancelDone:
				msgSendFlag = false;
				break;
			default:
				msgSendFlag = false;
				break;
		}//End switch
	}
	else{
		//Parse the evnet message from ScanApp. After that, send message to UIManager.
		switch(scanAppEvent){
			case SCANAPP_EVENT_NULL:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_NULL  \n", __FUNCTION__);
				break;
			case SCANAPP_EVENT_IDLE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_IDLE  \n", __FUNCTION__);
				break;
			case SCANAPP_EVENT_SCAN_START:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_START  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_ACCEPTED;
				goGscanResult.page      = 1; //scaned page
				goGscanResult.source    = 0; //flat bed
				msgSendFlag = true;				
				break;
			case SCANAPP_EVENT_SCAN_RUNNING:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_START  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_PAGE_DONE;
				goGscanResult.page  	   = 0; //scaned page
				goGscanResult.source    = 0; //flat bed
				msgSendFlag = true;
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_RUNNING  \n", __FUNCTION__);
				break;
			case SCANAPP_EVENT_SCAN_COMPLETE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_COMPLETE  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_COMPLETED;
				goGscanResult.page      = 1; //scaned total page
				goGscanResult.source    = 0; //flat bed
				msgSendFlag = true;
				break;
			case SCANAPP_EVENT_HOST_CANCEL:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_HOST_CANCEL  \n", __FUNCTION__);
				break;
			case SCANAPP_EVENT_DEVICE_CANCEL:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_DEVICE_CANCEL  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_COMPLETED;
				goGscanResult.page      = 1; //scaned total page
				goGscanResult.source    = 0; //flat bed
				msgSendFlag = true;
				break;
			case SCANAPP_EVENT_TIMEOUT:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_TIMEOUT  \n", __FUNCTION__);
				break;
			case SCANAPP_EVENT_SCAN_OFFLINE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_OFFLINE  \n", __FUNCTION__);
				break;
			case SCANAPP_EVENT_WAITING_FOR_USER_INPUT:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_WAITING_FOR_USER_INPUT  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_WAIT_USERINPUT;
				msgSendFlag = true;
				break;
			case SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_UISCAN:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_UISCAN  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_WAIT_NEXT_LEGAL_UISCAN;
				msgSendFlag = true;
				break;
			case SCANAPP_EVENT_SCAN_FAILED_FB_EMPTY_FOR_MERGE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_FAILED_FB_EMPTY_FOR_MERGE  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_FAILED_FB_EMPTY_FOR_MERGE;
				msgSendFlag = true;
				break;
			case SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_PCSCAN:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_PCSCAN  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_WAIT_NEXT_LEGAL_PCSCAN;
				msgSendFlag = true;
				break;
			case SCANAPP_EVENT_SCAN_FAILED_MEMORY_FULL:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_FAILED_MEMORY_FULL  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_FAILED_MEMORY_FULL;
				break;
			case SCANAPP_EVENT_SCAN_FAILED_FILE_WRITE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_FAILED_FILE_WRITE  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_FAILED_FILE_WRITE;
				break;
			case SCANAPP_EVENT_SCAN_FAILED_ADF_PAPER_IN_MERGE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_FAILED_ADF_PAPER_IN_MERGE  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_FAILED_ADF_PAPER_MERGE;
				break;
			case SCANAPP_EVENT_SCAN_FAILED_FB_NOT_SUPPORT_SIZE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_FAILED_FB_NOT_SUPPORT_SIZE  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_FAILED_FB_NOT_SUPPORT_SIZE;
				break;
			case SCANAPP_EVENT_SCAN_FAILED_USBTHUMB_NOT_CONNECT:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_FAILED_USBTHUMB_NOT_CONNECT  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_FAILED_USBTHUMB_NOT_CONNECT;
				break;
			case SCANAPP_EVENT_SCAN_STOP_MEMORY_FULL:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = SCANAPP_EVENT_SCAN_STOP_MEMORY_FULL  \n", __FUNCTION__);
				goGscanResult.result    = SCN_RSLT_JOB_STOP_MEMORY_FULL;
				break;
			default:
				msgSendFlag = false;
				break;
		}
		
		ScannerPlotterModeStatusService(scanAppEvent, 0);
		SetScannerStatus(false, scanAppEvent);
	}
	
	if(msgSendFlag){
		memcpy(&msgToSend.data, &goGscanResult, sizeof(stScan_GoScanResult));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );

		if ( IS_TFTLCD_ATTACHED ){
			if((goGscanResult.result == SCN_RSLT_WAIT_USERINPUT) || (goGscanResult.result == SCN_RSLT_WAIT_NEXT_LEGAL_UISCAN) || (goGscanResult.result == SCN_RSLT_WAIT_NEXT_LEGAL_PCSCAN)){
				if(g_nTFTBackLightOn == false){
					SetTFTPanelMode(TFT_FB_WAKEUP);
				}
			}
		}
	}
}

void ScanmanEventService(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG(" [%s] \n", __FUNCTION__);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend = {0};
	bool msgSendFlag = true;
	stScnPltr_ScannerJamStatus stScannerJamStatus = {0};
	stIC_ScanmanEventStatus stScanmanEventRecv = {0};
	stCopy_GoCopyResult stCopyResult = {0};
	stScnPltr_ADFStatus stADFStatus = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID;	
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	memcpy(&stScanmanEventRecv, Message->data, sizeof(stIC_ScanmanEventStatus));

	switch(stScanmanEventRecv.eventReason){
		case SCANMAN_EVENT_IDLE:
		case SCANMAN_EVENT_SCAN_START:
		case SCANMAN_EVENT_SCAN_RUNNING:
		case SCANMAN_EVENT_SCAN_COMPLETE:
		case SCANMAN_EVENT_SCAN_CANCEL:
		case SCANMAN_EVENT_SCAN_OFFLINE:
			msgSendFlag = false;
			SetScannerStatus(true, stScanmanEventRecv.eventReason);
			break;
		//case SCANMAN_EVENT_SCAN_CALIBRATION_RUNNING:
		case SCANMAN_EVENT_SCAN_CALIBRATION_COMPLETE:
			gWaitForScanCalibrationDone = false;	//Calibration Done so no wait
			msgSendFlag = false;
			break;
		case SCANMAN_EVENT_ADF_PAPER_PRESENT:
			DBG_SSL_PRINTF_SDH_ERR("SCANMAN_EVENT_ADF_PAPER_PRESENT=%d\n", stScanmanEventRecv.eventStatus);
			if(stScanmanEventRecv.eventStatus == 0){
				stADFStatus.Status &= ~(bitl(0));	//ADF NO paper present
				gScanADFPaperPresent = false;
			}
			else{
				stADFStatus.Status = bitl(0);	//ADF Paper present
				gScanADFPaperPresent = true;
			}
			//If Top cover already opened then set bit 2
			if(gScanADFTopCoverOpen){
				stADFStatus.Status |= bitl(2);
			}

			//If system is in Energy Save mode then Send Wake-up
			SysWakeUpForUserAttention(EMA_SET_ADF_DOC);

			//Send ADF Paper status: If Top cover open then don't send ADF Paper status
			if(gScanADFTopCoverOpen){
				msgSendFlag = false;
			}
			else{
				msgToSend.cmd = CMD_SU_ADF_MODE_STATUS;
				memcpy(&msgToSend.data, &stADFStatus, sizeof(stScnPltr_ADFStatus));
			}
			break;
		case SCANMAN_EVENT_ADF_PAPER_JAM:
			DBG_SSL_PRINTF_SDH_ERR("SCANMAN_EVENT_ADF_PAPER_JAM=%d\n", stScanmanEventRecv.eventStatus);
			if(stScanmanEventRecv.eventStatus == 0){
				stScannerJamStatus.jamReason = 0; //Clear JAM
				gScanJamStatus = false;
				TimeoutControl(TIMEOUT_RESET);
			}
			else{
				stScannerJamStatus.jamReason = 202;
				if(!gScanJamStatus){
					SaveErrorLogInDB(ERR_RECOVERABLE, 202, true); //Scanner J202 mod.kks 2014.06.16
				}
				gScanJamStatus = true;
				TimeoutControl(TIMEOUT_PAUSE);
			}
			SetScannerStatus(true, stScanmanEventRecv.eventReason);
			msgToSend.cmd = CMD_SU_SCANNER_JAM_STATUS;
			memcpy(&msgToSend.data, &stScannerJamStatus, sizeof(stScnPltr_ScannerJamStatus));
			break;
		case SCANMAN_EVENT_ADF_PAPER_NOPICK:
			DBG_SSL_PRINTF_SDH_ERR("SCANMAN_EVENT_ADF_PAPER_NOPICK=%d\n", stScanmanEventRecv.eventStatus);
# if 0 // Original Code.			
			msgSendFlag = false;
			if(stScanmanEventRecv.eventStatus == 0){
				stScannerJamStatus.jamReason = 0;	//Clear JAM
			}
			else{
				stScannerJamStatus.jamReason = 102;
			}
#else // The Paper JAM Modify mod.kks 
			if(stScanmanEventRecv.eventStatus == 0){
				stScannerJamStatus.jamReason = 0; //Clear JAM
				gScanJamStatus = false;
				TimeoutControl(TIMEOUT_RESET);
			}
			else{
				stScannerJamStatus.jamReason = 201;
				if(!gScanJamStatus){
					SaveErrorLogInDB(ERR_RECOVERABLE, 201, true); //Scanner J201 mod.kks 2014.06.16
				}
				gScanJamStatus = true;
				TimeoutControl(TIMEOUT_PAUSE);
			}
			SetScannerStatus(true, stScanmanEventRecv.eventReason);
			msgToSend.cmd = CMD_SU_SCANNER_JAM_STATUS;
			memcpy(&msgToSend.data, &stScannerJamStatus, sizeof(stScnPltr_ScannerJamStatus));
#endif
			break;
		case SCANMAN_EVENT_ADF_TOP_COVER_OPEN:
			DBG_SSL_PRINTF_SDH_ERR("SCANMAN_EVENT_ADF_TOP_COVER_OPEN=%d\n", stScanmanEventRecv.eventStatus);
			if(stScanmanEventRecv.eventStatus == 0){
				stADFStatus.Status &= ~(bitl(2));
				gScanADFTopCoverOpen = false;
				//If ADF paper present then set bit 0
				if(gScanADFPaperPresent){
					stADFStatus.Status |= bitl(0);
				}
			}
			else{
				stADFStatus.Status = bitl(2);
				gScanADFTopCoverOpen = true;
			}

			SysWakeUpForUserAttention(SW_ADF_TOP_COVER_STATUS_CHANGE);
			SetScannerStatus(true, stScanmanEventRecv.eventReason);
			msgToSend.cmd = CMD_SU_ADF_MODE_STATUS;
			memcpy(&msgToSend.data, &stADFStatus, sizeof(stScnPltr_ADFStatus));
			break;
		case SCANMAN_EVENT_FB_PAPER_EMPTY:
			DBG_SSL_PRINTF_SDH_ERR("SCANMAN_EVENT_FB_PAPER_EMPTY=%d\n", stScanmanEventRecv.eventStatus);
			msgSendFlag = false;
			//Scan job failed due to FB paper empty
			if(stScanmanEventRecv.eventStatus != 0){
				if(gCopyJobStatus){
					stCopyResult.result = RSLT_JOB_FAILED_MERGE;
					stCopyResult.jobID = g_nCurrentCopyJobID;
					msgToSend.cmd = CMD_SU_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if(gScanJobStatus){
					stScan_GoScanResult stScanResult = {0};
					stScanResult.result = SCN_RSLT_JOB_FAILED_FB_EMPTY_FOR_MERGE;
					msgToSend.cmd = CMD_SU_SCAN_GOSCAN_RES;
					memcpy(msgToSend.data, &stScanResult, sizeof(stScan_GoScanResult));
					msgSendFlag = true;
				}
				else if(g_nFaxScanJobCount){
					stPrint_SendFaxStatus stSendFaxStatus = {0};
					stSendFaxStatus.statusType = FAX_JOB_FAILED_MERGE;
					msgToSend.cmd = CMD_SU_FAX_SEND_FAX_STATUS;
					memcpy(msgToSend.data, &stSendFaxStatus, sizeof(stPrint_SendFaxStatus));
					msgSendFlag = true;
				}
			}
			break;
		case SCANMAN_EVENT_ADF_PAPER_SHORT:
			msgSendFlag = false;
			/* 
			 * Coding Here
			 */
			break;
		case SCANMAN_EVENT_PAGE_END_COUNT:
			DBG_SSL_PRINTF_SDH_ERR("SCANMAN_EVENT_PAGE_END_COUNT=%d\n", stScanmanEventRecv.eventStatus);
			msgSendFlag = false;
			ScanCounterService(stScanmanEventRecv.eventStatus, 0);
			break;
		case SCANMAN_EVENT_SC_AFE_OFFSET_FAIL:
		case SCANMAN_EVENT_SC_AFE_GAIN_FAIL:
		case SCANMAN_EVENT_SC_AFE_SETTING_FAIL:
		case SCANMAN_EVENT_SC_CIS_HOME_POSITION_FAIL:
		case SCANMAN_EVENT_SC_WHITE_STRIP_READ_FAIL:
		case SCANMAN_EVENT_SC_BLACK_STRIP_READ_FAIL:
		case SCANMAN_EVENT_SC_BAD_PIXEL_FAIL:
		case SCANMAN_EVENT_SC_CIS_HOME_OFF_FAIL:
		case SCANMAN_EVENT_SC_CIS_HOME_ON_FAIL:
		case SCANMAN_EVENT_SC_BAD_PIXEL_FAIL_B:
		case SCANMAN_EVENT_SC_PIC_OVF_FAIL:
		case SCANMAN_EVENT_SC_CMD_QUEUE_OVR_FAIL:
		case SCANMAN_EVENT_SC_CMD_COMPLETE_OVR_FAIL:
		case SCANMAN_EVENT_SC_MEM_MALLOC_FAIL:
		case SCANMAN_EVENT_SC_PIPE_WAIT_TIMEOUT:
		case SCANMAN_EVENT_SC_SCAN_ASSERT_FAIL:
			DBG_SSL_PRINTF_SDH_ERR("SCANMAN_EVENT_SC=%d\n", stScanmanEventRecv.eventReason);
			msgSendFlag = false;
			SendSCStatusToUI(stScanmanEventRecv.eventReason, SCAN_APP_SCID);
			break;
		default:
			DBG_SSL_PRINTF_SDH_DEBUG("Unknown Scaman Event Type=%d, Status=%d\n", stScanmanEventRecv.eventReason, stScanmanEventRecv.eventStatus);
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );

		//Notify to ADF event to ScanApp if current JobType is ScanToHost
		//if(g_nCurScanJobType == ejob_ScanToHost){
			scanapp_notify_adf_status(stScanmanEventRecv.eventReason, stScanmanEventRecv.eventStatus);
		//}

		if ( IS_TFTLCD_ATTACHED ){
			if(gScanJamStatus){
				if(g_nTFTBackLightOn == false){
					SetTFTPanelMode(TFT_FB_WAKEUP);
				}
			}
		}
	}
}

/**
 * Scan send service (UI -> system msg IF)
 */
void ScanSendService(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_ERR("[%s] error not supported\n", __FUNCTION__);
#if 0
	//error_type_t err;
	stScan_GoScanSend stGoScanSend = {0};
	memcpy(&stGoScanSend, Message->data, sizeof(stScan_GoScanSend));

	if(stGoScanSend.act == SCAN_ACTION_CANCEL){
		removeNetworkJob(stGoScanSend.jobID);
	}
#endif
}

void ScanSendResultService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	bool msgSendFlag = false; //TODO: 20170601: send 0683 in future for now disable
	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_SCAN_GOSCANSEND_RES;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	stIC_ScanSendResult stIntScanSendRes = {0};
	stScan_GoScanSendResult stGoScanSendRes = {0};

	memcpy(&stIntScanSendRes, Message->data, sizeof(stIC_ScanSendResult));
	stGoScanSendRes.jobID = stIntScanSendRes.jobID;
	stGoScanSendRes.scanJobID = stIntScanSendRes.scanJobID;
	stGoScanSendRes.result = stIntScanSendRes.result;
	stGoScanSendRes.jobType = stIntScanSendRes.jobType;
	stGoScanSendRes.errorCode = stIntScanSendRes.errorCode;
	stGoScanSendRes.addInfo = stIntScanSendRes.addInfo;

	DBG_SSL_PRINTF_SDH_ERR("[%s] JobID:%d, ScanJobID:%d DestCount:%d result:%d jobType:%d\n",
			__FUNCTION__,stGoScanSendRes.jobID, stGoScanSendRes.scanJobID,
			stIntScanSendRes.destCount,	stGoScanSendRes.result, stGoScanSendRes.jobType);

	switch(stGoScanSendRes.result){
		case SCN_SEND_ACCEPTED:
			break;
		case SCN_SEND_STARTED:
			break;
		case SCN_SEND_CANCEL_FAILED:
			break;
		case SCN_SEND_FAILED:
		case SCN_SEND_CANCELED:
		case SCN_SEND_FAILED_DUE_TO_NETWORK_ERROR:
		case SCN_SEND_FAILED_DUE_TO_UNKNOWN_ERROR:
			{
				stIC_JobDoneInfo stJobDone = {0};
				stJobDone.jobNum = stGoScanSendRes.scanJobID;
				stJobDone.copies = 0; //Use copies as ScanResult
				SysManageScanJobList(SCAN_INFO_COUNTUP, &stJobDone, stIntScanSendRes.destCount);
			}
			break;
		case SCN_SEND_COMPLETED:
			{
				stIC_JobDoneInfo stJobDone = {0};
				stJobDone.jobNum = stGoScanSendRes.scanJobID;
				stJobDone.copies = 1; //Use copies as ScanResult
				SysManageScanJobList(SCAN_INFO_COUNTUP, &stJobDone, stIntScanSendRes.destCount);
			}
			break;
		default:
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		memcpy(&msgToSend.data, &stGoScanSendRes, sizeof(stScan_GoScanSendResult));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}


//********************************************************************************
//**********************************************************************************
//************************************************************************************


/**
 *
 *\brief Used For Scan Application check USB Thumb is mounted or not.
 *
 **/
char * findingUSBThumb(int *notShowPosition)
{
	int i=0,j=0;
	int ifFindSpace=0;
	int ifFindVFAT=0;
	int SpacePosition[4]={0, 0, 0,0};
	FILE *fp;
	char procFileBuf[PROC_FILEBUFSIZE];
	char tmpMountPosition[PROC_FILEBUFSIZE];
	char USBMountPosition[PROC_FILEBUFSIZE];
   
	char *returnValue;
   
	fp=fopen(PROC_FILESYSTEM,"rt");
	if (fp == NULL)
	{
		//printf (">>> UM: Proc Filesystem Error\n");
		return 0;
	}

	ifFindVFAT=0;
	while (fgets(procFileBuf,PROC_FILEBUFSIZE,fp))
	{
		ifFindSpace=0;
		for (j=0; j<3; j++) SpacePosition[j]=0;
		j=0;
		for (i=0;i<(int)strlen(procFileBuf);i++)
		{
			if (procFileBuf[i]==' ')
			{
				ifFindSpace=1;
			} 
			else if (ifFindSpace==1)
			{
				SpacePosition[j]=i;
				j++;
				ifFindSpace=0;
			}
		}
		//printf("SpacePosition:%d %d %d %d (L:%d)\n",SpacePosition[0],SpacePosition[1],SpacePosition[2],SpacePosition[3],strlen(procFileBuf));
		strncpy(tmpMountPosition,&procFileBuf[SpacePosition[3]],strlen(procFileBuf)-SpacePosition[3]-1);    tmpMountPosition[strlen(procFileBuf)-SpacePosition[3]-1]='\0';
		//printf ("Parsed Str1:%s\n",tmpMountPosition);
		if ( (tmpMountPosition[0] == 's') && (tmpMountPosition[1] == 'd') && (tmpMountPosition[3] == '1') )
		{
			// int putPosition=0;
			ifFindVFAT++;
			strcpy(USBMountPosition,"/mnt/");
			strcat(USBMountPosition,tmpMountPosition);
			strcat(USBMountPosition,"/");
		}
	}
	fclose(fp);
	if (ifFindVFAT > 0)
	{
		returnValue=(char*)malloc(strlen(USBMountPosition)+1);
		strcpy(returnValue,USBMountPosition);
		*notShowPosition=strlen(USBMountPosition)-1;
		//printf ("Found USBMountPosition: %s, notShowPos:%d\n",USBMountPosition,*notShowPosition);
		return returnValue;
	}
	else
	{
		return 0;
	}
}

char * GetVersion(uint32_t  elementID)
{
	char *versionStr = NULL;
	
	switch(elementID){
		case ICT_UI_MANAGER:
			versionStr = &g_sUIManagerVersion[0];
			break;
		case ICT_UICC:
			versionStr = &g_sUICCVersion[0];
			break;
		case ICT_SDC:
			break;
		case ICT_EGB:
			break;
		case ICT_FINISHER:
			break;
		case ICT_GUI_PS:
			versionStr = &g_sPS_GUIVersion[0];
			break;
		case ICT_BROWSER_PS:
			versionStr = &g_sPS_BrowserVersion[0];
			break;			
		default:
			break;
	}
	return versionStr;
}

void InitCompletionService(SYS_UI_MESSAGE *Message)
{
	stInit_InitCompletion stInitCompletion;
	static int rtc_time_check_once = 0;

	memcpy(&stInitCompletion, Message->data, sizeof(stInit_InitCompletion));

	switch(stInitCompletion.type){
		case ICT_UI_MANAGER:
			memset(g_sUIManagerVersion, 0, sizeof(g_sUIManagerVersion));
			snprintf(g_sUIManagerVersion, sizeof(g_sUIManagerVersion), "%s", stInitCompletion.version);
			DBG_SSL_PRINTF_SDH_ERR("[%s] UIMANAGER Version: %s\n", __FUNCTION__, g_sUIManagerVersion);
			if(!IS_TFTLCD_ATTACHED){
				gInitCompleted = true;
			}
			if ( rtc_time_check_once == 0 ) {
				rtc_time_check_once = 1;
				error_type_t err;
				int tempNtpEnable = db_get_var_int("CONF_SYSTEM", "NtpEnable", &err);
				if ( err == FAIL ) {
					tempNtpEnable = 0;
				}
				if ( tempNtpEnable == 0 ) {
					if ( rtc_time_check() < 0 ) {
						SendSpecialNotification(SNT_INVALID_RTC_TIME, SNO_SHOW_POPUP_WITH_CANCEL);
					}
				}
			}
			break;
		case ICT_UICC:
			memset(g_sUICCVersion, 0, sizeof(g_sUICCVersion));
			snprintf(g_sUICCVersion, sizeof(g_sUICCVersion), "%s", stInitCompletion.version);
			DBG_SSL_PRINTF_SDH_DEBUG("[%s] UICC Version: %s\n", __FUNCTION__, g_sUICCVersion);
			break;
		case ICT_GUI_PS:
			memset(g_sPS_GUIVersion, 0, sizeof(g_sPS_GUIVersion));
			snprintf(g_sPS_GUIVersion, sizeof(g_sPS_GUIVersion), "%s", stInitCompletion.version);
			DBG_SSL_PRINTF_SDH_ERR("[%s] GUI Version: %s\n", __FUNCTION__, g_sPS_GUIVersion);
			break;
		case ICT_BROWSER_PS:
			memset(g_sPS_BrowserVersion, 0, sizeof(g_sPS_BrowserVersion));
			snprintf(g_sPS_BrowserVersion, sizeof(g_sPS_BrowserVersion), "%s", stInitCompletion.version);
			DBG_SSL_PRINTF_SDH_ERR("[%s] Browser Version: %s\n", __FUNCTION__, g_sPS_BrowserVersion);
			gInitCompleted = true;
			break;
		default:
			break;
	}

	if(gInitCompleted){
		if(g_bOnInitSendUSBScrOnce == false){
			g_bOnInitSendUSBScrOnce = true;
			if(CheckUSBMounted()){
				SYS_UI_MESSAGE msgToSend;
				stIC_USBThumbDrive stUSBConnection = {0};
				msgToSend.msgSender = SYSTEM_SERVICE_ID;
				msgToSend.cmd = CMD_INT_USB_THUMB_DRIVE;
				memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
				stUSBConnection.status = UDB_STATUS_MOUNT;
				memcpy(msgToSend.data, &stUSBConnection, sizeof(stIC_USBThumbDrive));
				USBThumbDriveService(&msgToSend);
			}
		}

		//Check Alive Alert
		if(g_bAliveAlertSentOnce == false){
			g_bAliveAlertSentOnce = true;
			if( GetAliveEmailAlertFlag() ){
				AliveEmailAlertControl(0);
			}
		}
	}
}

uint32_t GetTrayPJLStatusCode(void)
{
	return gTrayPJLCode;
}

void SetTrayAttachedInfo(int trayAttached)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s] Tray Attached=%d\n", __FUNCTION__, trayAttached);
	uint32_t trayStatusCode = 0;
	bool shmWriteFlag = true;
	switch(trayAttached){
		case 1:
			trayStatusCode = 16100;
			break;
		case 2:
			trayStatusCode = 16200;
			break;
		case 3:
			trayStatusCode = 16300;
			break;
		default:
			shmWriteFlag = false;
			break;
	}
	
	if(shmWriteFlag){
		if(trayStatusCode != gTrayPJLCode){
			gTrayPJLCode = trayStatusCode;
			//SHM: Write Tray attached information to Shared Memory so other process(like web server) can use it.		
			if(g_pSHM_TrayStatus){
				memcpy(g_pSHM_TrayStatus, &trayStatusCode, sizeof(uint32_t));
			}
		}
	}
}

void MachConfService(SYS_UI_MESSAGE *Message)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	uint32_t CurStatus; 
	bool msgSendFlag = true;

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);

	memcpy(&CurStatus, Message->data, sizeof(uint32_t));

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_MACHINE_CONF;	//Machine configuration
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	gStMachConf.wifiModuleType = gWifiModuleType;
	switch(CurStatus){		
		case STATUS_TRAY3_ATTACHED: 
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_ATTACHED \n", __FUNCTION__);
			gStMachConf.feedTrayOpt2 |= bitl(0);
			gTrayAttached = 3;			
			break;
		case STATUS_TRAY3_NOT_ATTACHED: 
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_NOT_ATTACHED \n", __FUNCTION__);
			gStMachConf.feedTrayOpt2 &= ~(bitl(0)); 	
			break;
			
		case STATUS_TRAY2_ATTACHED:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_ATTACHED \n", __FUNCTION__);
			gStMachConf.feedTrayOpt1 |= bitl(0);
			gTrayAttached = 2;
			msgSendFlag = false;
			break;
		case STATUS_TRAY2_NOT_ATTACHED:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_NOT_ATTACHED \n", __FUNCTION__);
			gStMachConf.feedTrayOpt1 &= ~(bitl(0));
			gTrayAttached = 1;
			msgSendFlag = false;
			break;
		default:	
			msgSendFlag = false;
			break;
	}
	
	if(msgSendFlag){
		//Send authentication config message to UIManager when system init done
		if(!gAuthConfSentOnInit){
			gAuthConfSentOnInit = true;
			AuthConfService();
		}
		//NOTE: For a situation that default tray has been detached.
		/*Sometime problem occurs under situation that default tray is set as tray 3 and tray 3 is not connected
		 * then change default tray 1 in DB. */
		//=================== Check and if problem then set default tray  ============
		int defaultTray = 0;
		bool resetDefaultTray = false;
		error_type_t db_retval = FAIL;

		defaultTray = db_get_var_int("CONF_PRNT", "Default_Src_Tray", &db_retval);
		if(db_retval == FAIL){
			DBG_SSL_PRINTF_SDH_ERR("[%s] DB read fail for Default_Src_Tray\n", __FUNCTION__);
			defaultTray = 1;
		}
		switch(defaultTray){
			case 2:
				if( (gStMachConf.feedTrayOpt1 & bitl(0)) != bitl(0) ){
					resetDefaultTray = true;
				}
				break;
			case 3:
				if( (gStMachConf.feedTrayOpt2 & bitl(0)) != bitl(0) ){
					resetDefaultTray = true;
				}
				break;
			default:
				break;
		}

		if(resetDefaultTray){
			defaultTray = 1;
			db_retval = db_set_var_int("CONF_PRNT", "Default_Src_Tray", defaultTray);
			if(db_retval == FAIL){
				DBG_SSL_PRINTF_SDH_ERR("[%s] DB set fail for Default_Src_Tray\n", __FUNCTION__);
			}
		}
		//=================== ===========

		SetTrayAttachedInfo(gTrayAttached);
		memcpy(msgToSend.data, &gStMachConf, sizeof(stInit_MachineConf));	
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void SetInfoService(SYS_UI_MESSAGE *Message)
{
	int ret;
	stInit_SetGetInfo stGetInfo;
	memcpy(&stGetInfo, Message->data, sizeof(stGetInfo));
	switch (stGetInfo.type) {
		case SYS_REQ_VERSION:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Error to get Version \n", __FUNCTION__);
			break;
		case SYS_SET_COUNTRY_CODE: // Country Code
			{
				error_type_t err;
				SYS_UI_MESSAGE msgToSend;
				if ( stGetInfo.data == 0 ) {
					// Initial Setup is done in UI side.
					ret = 0; //OK
				}
				else {
					ret = SetCountryCode((uint32_t)stGetInfo.data);
				}
				if ( ret == 0 ) {
					error_type_t db_retval;
					// Clear initial machine setup
					db_retval = db_set_var_int("CONF_SYSTEMSP", "INITIALMACHINESETUP", 0);
					if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
					// Save CountryCode
					db_retval = db_set_var_int("CONF_SYSTEMSP", "INITIALMACHINECOUNTRYCODE", stGetInfo.data);
					if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
				}
				msgToSend.msgSender = SYSTEM_SERVICE_ID; 
				msgToSend.cmd = CMD_SU_SETGET_INFO;
				stGetInfo.data = ret;
				memcpy(msgToSend.data, &stGetInfo, sizeof(stGetInfo));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );
				gInitialMachineSetupProceeding = 0;
			}
			break;
		case SYS_REQ_TEST_SEND:
			if(Message->msgSender == WEB_MONITOR_ID){
				DBG_SSL_PRINTF_SDH_ERR("[%s]: Test Email Send request received from WebMonitor\n", __FUNCTION__);
			}
			else{
				DBG_SSL_PRINTF_SDH_ERR("[%s]: Test Email Send request received from GUI\n", __FUNCTION__);
			}
			ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_TEST_SEND, gCurTonerPageCount);
			break;
		default:
			break;
	}
}

void ScreenChangeReqService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stWnd_ScreenChgReqResult stScrChgReqResult = {0}; 
	stWnd_ScreenChgReq stScrChgReq = {0};	

	if ( gInitialMachineSetupProceeding ) {
		return;
	}

	memcpy(&stScrChgReq, Message->data, sizeof(stWnd_ScreenChgReq));	

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Request Screen: %d  \n", __FUNCTION__, stScrChgReq.screen);

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_SCR_CHANGE_RES;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stScrChgReqResult.screen = stScrChgReq.screen;
	stScrChgReqResult.result = SCREEN_CHANGE_OK;	//OK

	//For requested screen UP and SP
	if((stScrChgReqResult.screen == UI_SCREEN_UP_ID) || (stScrChgReqResult.screen == UI_SCREEN_SP_ID)){
		if(gSystemPrintJobStatus || gScanJobStatus ||gCopyJobStatus || gFaxJobStatus || gLaminationJobStatus){
			stScrChgReqResult.result = SCREEN_CHANGE_FAIL_SYS_BUSY; //FAIL: in processing
			TimeoutControl(TIMEOUT_PAUSE);
		}
	}

	//Set current screen for System use
	if(stScrChgReqResult.result == SCREEN_CHANGE_OK){
		if((g_nCurrentUIScreenID == UI_SCREEN_SP_ID) && (stScrChgReqResult.screen == 0)){
			SendSPModeDone();
		}
		if(g_nCurrentUIScreenID != stScrChgReqResult.screen){
			g_nCurrentUIScreenID = stScrChgReqResult.screen;
			if((g_nCurrentUIScreenID == UI_SCREEN_UP_ID) || (g_nCurrentUIScreenID == UI_SCREEN_SP_ID)){
				TimeoutControl(TIMEOUT_PAUSE);
			}
			else{
				TimeoutControl(TIMEOUT_RESET);
				//If user select Copy screen then send Full WAKE-UP
				if(g_nCurrentUIScreenID == SCREEN_COPY){
					SysWakeUpForUserAttention(SW_COPY_SCREEN_REQUESTED);
				}
			}
		}
	}
	else{
		DBG_SSL_PRINTF_SDH_ERR(
			"SCR: Fail SCR:%d P:%d[%d] S:%d[%d] C:%d[%d] F:%d[%d] ESM:%d L:%d\n",
			stScrChgReqResult.screen, gSystemPrintJobStatus,
			g_nPrintJobCount, gScanJobStatus, g_nScanJobCount,
			gCopyJobStatus, g_nCopyJobCount, gFaxJobStatus, g_nFaxJobCount,
			g_nEnergySaveMode, gLaminationJobStatus);
	}

	memcpy(msgToSend.data, &stScrChgReqResult, sizeof(stWnd_ScreenChgReqResult));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
	
	if (IS_SCANNER_ATTACHED) {
		//App Acceptable msg with Scanner and Plotter Status
		ScannerPlotterModeStatusService(0,0);
	}
}


void USBThumbDriveService(SYS_UI_MESSAGE *Message)
{
	if(!gSystemSCStatus && !gInitialMachineSetupProceeding ){
		error_type_t err;
		SYS_UI_MESSAGE msgToSend;
		stIC_USBThumbDrive stUSBConnection = {0};
		stWnd_ScreenChgReqResult stScrChgUSB = {0};
		bool msgSendFlag = true;
		
		memcpy(&stUSBConnection, Message->data, sizeof(stIC_USBThumbDrive));
		
		msgToSend.msgSender = SYSTEM_SERVICE_ID;
		msgToSend.cmd = CMD_SU_SCR_CHANGE_RES;
		memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

		switch(stUSBConnection.status){
			case UDB_STATUS_REMOVE:	//Remove USB
				stScrChgUSB.param = UDB_STATUS_REMOVE;
				ScanAppNotify(UDB_STATUS_REMOVE);
				break;
			case UDB_STATUS_ADD:	//Add USB
				//If Energy save mode then Wake-up.
				stScrChgUSB.param = UDB_STATUS_ADD;
				SysWakeUpForUserAttention(EMA_USB_PLUGGING);
				break;
			case UDB_STATUS_MOUNT:	//Mount USB
				stScrChgUSB.param = UDB_STATUS_MOUNT;
				break;
			default:
				msgSendFlag = false;
				break;
		}

		if(msgSendFlag){
			stScrChgUSB.screen = SCREEN_USB;
			stScrChgUSB.result = SCREEN_CHANGE_OK;
			memcpy(msgToSend.data, &stScrChgUSB, sizeof(stWnd_ScreenChgReqResult));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );
		}
	}
}

void KeyTouchEventService(SYS_UI_MESSAGE *Message)
{
	//DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);

	stWnd_KeyPressed stKeyTouchPressed;

	memcpy(&stKeyTouchPressed, Message->data, sizeof(stWnd_KeyPressed));

	if(stKeyTouchPressed.mode == KEY_OR_TOUCH_PRESSED){	//Key or Touch Pressed
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Key or Touch Pressed  \n", __FUNCTION__);
		TimeoutControl(TIMEOUT_RESET);
		if ( IS_TFTLCD_ATTACHED ){
			if(Message->msgSender == ENGINEID){
				SysWakeUpForUserAttention(SW_DURING_PRINT_POWER_KEY_PRESSED);
			}
			else{
				if((g_nEnergySaveMode == false) && (g_nTFTBackLightOn == false)){
					SetTFTPanelMode(TFT_FB_WAKEUP);
				}
			}
		}
	}
	else if(stKeyTouchPressed.mode == POWER_KEY_PRESSED){	//Power Key Pressed	
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Power Key Pressed  \n", __FUNCTION__);
		if(Message->msgSender == ENGINEID){
			g_bResetWakeupOneTime = true;
		}
		
		if(g_nEnergySaveMode){	//go  to WAKE UP
			SysWakeUpForUserAttention(EMA_POWER_KEY_PRESSED);
		}
		else{	//go to ENERGY SAVE
			if ( IS_TFTLCD_ATTACHED ){
				//If System is in WAKE-UP mode and TFT is off then first "Wakeup FB: On Backlight of TFT"
				if((g_nEnergySaveMode == false) && (g_nTFTBackLightOn == false)){	
					SetTFTPanelMode(TFT_FB_WAKEUP);
					if(g_bResetWakeupOneTime){
						g_bResetWakeupOneTime = false;
						fWakeKey_Onetime = 0;
					}
				}
				else{
					SysDecideEnergySave(ESS_ENERGY_SAVE, 0);
				}
			}
			else{
				SysDecideEnergySave(ESS_ENERGY_SAVE, 0);
			}
		}
	}
}

void HourlyAlarmService(SYS_UI_MESSAGE *Message)
{
	stIC_HourlyAlarm stHourlyAlarm = {0}; 	
	memcpy(&stHourlyAlarm, Message->data, sizeof(stIC_HourlyAlarm));
	DBG_SSL_PRINTF_SDH_ERR("[%s]: Sender=%d UP Time=%d\n", __FUNCTION__, Message->msgSender, stHourlyAlarm.totalSysUpTime);
	gTotalSysUpTimeInHour = stHourlyAlarm.totalSysUpTime;
	//In Hourly Alarm timer event.
	if( Message->msgSender == SYSTEM_ENERGY_SAVE_TIMER ){

		gHourlyTimeUpdatePending = false; //Clear hourly timer update pending flag

		if(gSystemPrintJobStatus ||gScanJobStatus ||gCopyJobStatus ||gFaxJobStatus ){
			gHourlyTimeUpdatePending = true;
		}
		else{
			//Sync HW clock -> System clock. Options: hwclock -s or hwclock --hctosys
			system("hwclock -su;date;hwclock -r");
			extern void settimeofday_tz();
			settimeofday_tz();

			if ( gNtpEnable &&
					(stHourlyAlarm.totalSysUpTime - gNtpSyncTimeHour >= 24) ) {
				//Network time -> System clock
				gNtpSyncTimeHour = stHourlyAlarm.totalSysUpTime;
				system("/app/ntpd.sh &");
			}

			//Check Alive Alert
			g_nAliveAlertSentHour++;
			if(g_nAliveAlertSentHour >= 24){
				g_nAliveAlertSentHour = 0;
				if( GetAliveEmailAlertFlag() ){
					AliveEmailAlertControl(1);
				}
			}
		}
	}
}


void TimelyAlarmService(SYS_UI_MESSAGE *Message)
{
	stIC_TimelyAlarm stTimelyAlarm = {0};
	memcpy(&stTimelyAlarm, Message->data, sizeof(stIC_TimelyAlarm));
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Sender=%d, Timeout %d second\n", __FUNCTION__, Message->msgSender, stTimelyAlarm.totalTimeInSec);

	if( Message->msgSender == SYSTEM_ENERGY_SAVE_TIMER ){
		if(stTimelyAlarm.totalTimeInSec == g_nWrongImageTimedWait){
			if(gPrintCancelReason == STATUS_WRONG_IMAGE_SIZE_CANCEL_REQ){
				DBG_SSL_PRINTF_SDH_ERR("STATUS_WRONG_IMAGE_SIZE_CANCEL_REQ Send MSG_CANCELJOB to SJM\n");
				SendCancelJobMsgToSJM(0, SYS_REQUEST, e_Printer);
			}
		}
	}
}

void EnergySaveModeService(SYS_UI_MESSAGE *Message)
{	
	stWnd_EnergyMode stEnergyMode = {0}; 
	int adaptiveESTimeout;
	memcpy(&stEnergyMode, Message->data, sizeof(stWnd_EnergyMode));
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Sender=%d, Mode=%d, Type=%d\n", __FUNCTION__, Message->msgSender, stEnergyMode.mode, g_nEnergySaveType);

	// In Job Start event.
	//Adaptive Energy Saving: Set rate of CurrentWeekTime
	if( ( Message->msgSender == SJMID ) && ( stEnergyMode.mode == ESM_WAKE_UP ) ){
		g_nPCJobWakeUp = true;
		if (g_nEnergySaveType == EST_ADAPTIVE){
			error_type_t db_retval;
			char query[QUERY_LENGTH];
			char *table = "RECD_ADJUSTSLEEPTIMER";
			memset(query, 0, sizeof(query));

			int curHour = GetCurrentWeekTime();
			int curRate = GetCurrentRate(curHour);
			DBG_SSL_PRINTF_SDH_DEBUG(" %s: Adaptive Energy Saving Update DB Cur Hour=%d, Rate=%d\n", __FUNCTION__, curHour, curRate);
			
			if (curRate != 1){
				snprintf(query, QUERY_LENGTH, "UPDATE %s SET Rate=%d WHERE  DateHour=%d", table, 1, curHour);
				 db_retval = db_trySQLiteExec(query);
				if(db_retval == FAIL){
					DBG_SSL_PRINTF_SDH_DEBUG(" %s: Fail to Update DB table\n", __FUNCTION__);
				}
			}
		}
	}

	//In Every timeout Timer event.
	// Adaptive Power Saving. RealPowerSave = Power save time in minutes.
	if( ( Message->msgSender == SYSTEM_ENERGY_SAVE_TIMER ) && ( stEnergyMode.mode == ESM_ADAPTIVE_TIMEOUT ) && 
		( !(gSystemPrintJobStatus ||gScanJobStatus ||gCopyJobStatus ||gFaxJobStatus ) ) ){
		//Sync System clock with HW clock
		//system("hwclock -su;date;hwclock -r");
		
		if (g_nEnergySaveType == EST_ADAPTIVE){
			adaptiveESTimeout = MinuteAdaptiveEnergySaving();
			//20171011: Special request to set 60 minute timeout to 58 minute
			if(adaptiveESTimeout == SECONDS_IN_MINUTE_60)
				adaptiveESTimeout = SECONDS_IN_MINUTE_58;
			//Update Timeout value
			DBG_SSL_PRINTF_SDH_DEBUG(" %s: Adaptive Timer event ESTimeout=%d\n", __FUNCTION__, adaptiveESTimeout);
			if( adaptiveESTimeout != g_nTimeoutValue){
				pthread_mutex_lock( &gSysPosixTimerMutex );
				g_nTimeoutValue = adaptiveESTimeout;
				pthread_mutex_unlock( &gSysPosixTimerMutex );
			}
			ClearNextRate(); //Clear next period
		}
	}

	if(stEnergyMode.mode == ESM_WAKE_UP ) { 	// 0 : normal or WAKE-UP		
		SysWakeUpForUserAttention(stEnergyMode.action);
	}
	else	if(stEnergyMode.mode == ESM_ENERGY_SAVE )  { 	 // 1 : energy save mode
		SysDecideEnergySave(ESS_ENERGY_SAVE, 0);
	}
	g_nPCJobWakeUp = false;	//Reset
}


void FaxEnergySaveMode(bool state)
{
	SYS_UI_MESSAGE msgToSend;
	error_type_t err;

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_INT_FAX_POWER_SAVE_MODE;

	if (state == true)
	{
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Send Fax To Go To ENERGY_SAVE_MODE  \n", __FUNCTION__);
		msgToSend.data[0] = ESM_ENERGY_SAVE;
	}
	else
	{
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Send Fax To Go To WAKE_UP  \n", __FUNCTION__);
		msgToSend.data[0] = ESM_WAKE_UP;
	}

	err =SysUI_MsgSend( FAXAPPID, &msgToSend );
	XASSERT( err==OK, err );
}

void ScanEnergySaveMode(bool state)
{
	MESSAGE msg;
	error_type_t err;
		
	if(state == FALSE) {
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Send ScanMan To Go To WAKE_UP  \n", __FUNCTION__);
		msg.msgType = MSG_SCAN_POWERSAVE;
		msg.param1 = ESM_WAKE_UP;
	}
	else if(state == TRUE) {
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Send ScanMan To Go To Sleep  \n", __FUNCTION__);
		msg.msgType = MSG_SCAN_POWERSAVE;
		msg.param1 = ESM_ENERGY_SAVE;	
	}
	err = SYMsgSend(SCANMANID, &msg);
	XASSERT( err==OK, err );
}

void SysWakeUpForUserAttention(int wakeUpReason)
{
	if(g_nEnergySaveMode){	//If Energy save mode then send Wake-UP
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Wake-Up by %d\n", __FUNCTION__, wakeUpReason);
		switch(wakeUpReason){
			case EMA_RECEIVED_PRINT:
				SysDecideEnergySave(ESS_FULL_WAKE_UP, wakeUpReason);
				break;
			case EMA_FAX_RX:
			case EMA_FAX_TX:
			case EMA_POWER_KEY_PRESSED:
			case EMA_SET_ADF_DOC:
			case EMA_CARD_TAGGING:
			case EMA_USB_PLUGGING:
			case EMA_SC_OCCUR:
			case EMA_SCAN_TO_HOST:
				SysDecideEnergySave(ESS_COLD_WAKE_UP, wakeUpReason);
				break;
			default:
				break;
		}
	}
	else{
		//For User attention when system already in Wake-UP mode then just turn on Backlight of TFT
		switch(wakeUpReason){
			case EMA_SET_ADF_DOC:
			case EMA_CARD_TAGGING:
			case EMA_USB_PLUGGING:
			case EMA_SC_OCCUR:
			case SW_PLOTTER_DOOR_STATUS_CHANGE:
			case SW_PLOTTER_TRAY_STATUS_CHANGE:
			case SW_ADF_TOP_COVER_STATUS_CHANGE:
			case SW_DURING_PRINT_POWER_KEY_PRESSED:
				if( IS_TFTLCD_ATTACHED ){
					//If System is in WAKE-UP mode and TFT is off then first "Wakeup FB: ON Backlight of TFT"
					if(g_nTFTBackLightOn == false){
						DBG_SSL_PRINTF_SDH_ERR("[%s]: TFT Wake-Up by %d\n", __FUNCTION__, wakeUpReason);
						//Notify to UI for User Attention
						error_type_t err;
						SYS_UI_MESSAGE msgToSend;
						stWnd_EnergyMode stEnergySaveMode = {0};

						msgToSend.msgSender = SYSTEM_SERVICE_ID;
						msgToSend.cmd = CMD_SU_ENERGY_MODE;
						memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
						stEnergySaveMode.mode = ES_NORMAL_MODE;
						memcpy(msgToSend.data, &stEnergySaveMode, sizeof(stWnd_EnergyMode));
						err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
						XASSERT( err==OK, err );

						SetTFTPanelMode(TFT_FB_WAKEUP);
					}
				}
				break;
			case SW_COPY_SCREEN_REQUESTED:
			case SW_LAMINATION_JOB_REQUESTED:
				if(g_nEnergySaveState != ESS_FULL_WAKE_UP){
					g_nEnergySaveState = ESS_FULL_WAKE_UP;
					uint32_t msg;
					msg = Wake_Up;
					posix_message_send(lsu_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
				}
				break;
			default:
				break;
		}
	}
}

void SysDecideEnergySave(ENERGY_SAVE_STATE energyMode, int wakeupAction)
{
	DBG_SSL_PRINTF_SDH_ERR("EST: %d %d %d\n", energyMode, g_nEnergySaveType, g_nTimeoutValue);
	if((energyMode == ESS_FULL_WAKE_UP) || (energyMode == ESS_COLD_WAKE_UP)){
		if(g_nEnergySaveMode){	//If System is in Energy Save Mode then send WAKE-UP
			g_nEnergySaveMode = false;
			//Enable Power
			SysEnergySaveMode(energyMode, wakeupAction);

			//Wake Up: Wi-Fi Power save OFF
			NIF_PowerSave_off_wifi();
#ifdef HAVE_SDHFAX
			// Send Fax to wake up...
			if ( IS_FAX_ATTACHED ) {
				FaxEnergySaveMode(false);
			}
#endif
			//Wake up : ScanMan
			ScanEnergySaveMode(false);
			
			TimeoutControl(TIMEOUT_RESET);
			CheckPCUDrumStatus();	//Check PCU drum status and notify to UI
		}
	}
	else if(energyMode == ESS_ENERGY_SAVE){
		if(!g_nEnergySaveMode){	//Go to Energy Save mode
			DBG_SSL_PRINTF_SDH_ERR(
					"ESM: FD:%d PJam:%d PFS:%d P:%d SJ:%d SCb:%d S:%d C:%d F:%d FO:%d SC:%d ES:%d SCR:%d L:%d, I:%d\n",
					gFrontDoorStatus, gSystemJamStatus, gSystemFullStack, g_nPrintJobCount, gScanJamStatus,
					gWaitForScanCalibrationDone, g_nScanJobCount, g_nCopyJobCount, g_nFaxJobCount,
					gFaxOnHookBtnPressed, gSystemSCStatus, g_nEnergySaveMode, g_nCurrentUIScreenID,
					gLaminationJobStatus, gInitialMachineSetupProceeding);

			if(gFrontDoorStatus ||gSystemJamStatus || gSystemFullStack || gSystemPrintJobStatus
					||gScanJamStatus || gWaitForScanCalibrationDone || gScanJobStatus
					||gCopyJobStatus ||gFaxJobStatus ||gFaxOnHookBtnPressed
					||gSystemSCStatus || gLaminationJobStatus || gInitialMachineSetupProceeding){

				if(g_bResetWakeupOneTime){
					g_bResetWakeupOneTime = false;
					fWakeKey_Onetime = 0;
				}
			}
			else{
				g_nEnergySaveMode = true;
				g_nCurrentUIScreenID = 0; //Set Home Screen
				if ( IS_SCANNER_ATTACHED) {
					gWaitForScanCalibrationDone = true; //Wait: Don't allow Energy Save Mode till SCANNER Calibration is DONE
					//Sleep mode : ScanMan
					ScanEnergySaveMode(true);
					DBG_SSL_PRINTF_SDH_ERR("[%s]: PowerSave Scan Done\n", __FUNCTION__);
				}
#ifdef HAVE_SDHFAX
				// Send Fax to go to energy save mode...
				if ( IS_FAX_ATTACHED ) {
					FaxEnergySaveMode(true);
					DBG_SSL_PRINTF_SDH_ERR("[%s]: PowerSave Fax Done\n", __FUNCTION__);
				}
#endif

				SysEnergySaveMode(energyMode, wakeupAction);
				DBG_SSL_PRINTF_SDH_ERR("[%s]: PowerSave LSU Done\n", __FUNCTION__);
				//Sleep: Wi-Fi Power save ON
				NIF_PowerSave_on_wifi();
				DBG_SSL_PRINTF_SDH_ERR("[%s]: PowerSave WiFi Done\n", __FUNCTION__);
				TimeoutControl(TIMEOUT_PAUSE);
				DBG_SSL_PRINTF_SDH_ERR("[%s]: PowerSave Timeout Pause Done\n", __FUNCTION__);
			}
		}
	}
}

void TimeoutControl(uint32_t type)
{
	if(type == TIMEOUT_PAUSE){
		//DBG_SSL_PRINTF_SDH_ERR("TOT: %d\n", type);
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]:Timeout TIMEOUT_PAUSE  \n", __FUNCTION__);
		pthread_mutex_lock( &gSysPosixTimerMutex );
		g_bPauseTimeout = true;
		g_bResetTimeout = false;	//When PauseTimeout is true, Make ResetTimeout false.
		pthread_mutex_unlock( &gSysPosixTimerMutex );
	}
	else if(type == TIMEOUT_RESET){
		if (gFrontDoorStatus || gSystemJamStatus || gSystemFullStack
			|| gSystemPrintJobStatus || gScanJamStatus || gScanJobStatus
			|| gCopyJobStatus || gFaxJobStatus || gFaxOnHookBtnPressed
			|| gSystemSCStatus || g_nEnergySaveMode
			|| (g_nCurrentUIScreenID == UI_SCREEN_UP_ID)
			|| (g_nCurrentUIScreenID == UI_SCREEN_SP_ID)
			|| gLaminationJobStatus) {

		DBG_SSL_PRINTF_SDH_ERR(
				"TOC: FD:%d PJam:%d PFS:%d P:%d SJ:%d S:%d C:%d F:%d FO:%d SC:%d ES:%d SCR:%d L:%d\n",
				gFrontDoorStatus, gSystemJamStatus, gSystemFullStack,
				g_nPrintJobCount, gScanJamStatus, g_nScanJobCount,
				g_nCopyJobCount, g_nFaxJobCount, gFaxOnHookBtnPressed,
				gSystemSCStatus, g_nEnergySaveMode, g_nCurrentUIScreenID,
				gLaminationJobStatus);

			//DO NOTHING
		}else{
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Timeout TIMEOUT_RESET  \n", __FUNCTION__);
			pthread_mutex_lock( &gSysPosixTimerMutex );
			g_bResetTimeout = true;
			pthread_mutex_unlock( &gSysPosixTimerMutex );
		}
	}
}

void UpdateEnergySaveTimeout()
{
	int sleepMinVal = 0;
	error_type_t db_retval = FAIL;
	
	sleepMinVal = db_get_var_int("CONF_SYSTEM", "SleepMin", &db_retval);
	if(db_retval == FAIL){
		DBG_SSL_PRINTF_SDH_ERR("%s: Fail to get DB data:SleepMin\n", __FUNCTION__);
		sleepMinVal = 0;	//On fail: Disable energy save
	}

	if(sleepMinVal == -1){	//Adaptive Energy Save Mode	
		g_nEnergySaveType = EST_ADAPTIVE;
		sleepMinVal = MinuteAdaptiveEnergySaving(); //In seconds
	}else{
		g_nEnergySaveType = EST_NORMAL;
		sleepMinVal *= 60; //Minute to seconds
	}
	DBG_SSL_PRINTF_SDH_ERR("%s: Type=%d, TimeOut=%d\n", __FUNCTION__, g_nEnergySaveType, sleepMinVal);
	
	//20171011: Special request to set 60 minute timeout to 58 minute
	if(sleepMinVal == SECONDS_IN_MINUTE_60)
		sleepMinVal = SECONDS_IN_MINUTE_58;
	if(sleepMinVal != g_nTimeoutValue){
		pthread_mutex_lock( &gSysPosixTimerMutex );
		g_nTimeoutValue = sleepMinVal;
		pthread_mutex_unlock( &gSysPosixTimerMutex );
	}

	int sleepTimeInMinute = 0;
	sleepTimeInMinute = db_get_var_int("CONF_SYSTEM", "IdleSleeptime", &db_retval);
	if(db_retval == FAIL){
		DBG_SSL_PRINTF_SDH_ERR("%s: Fail to get DB data:IdleSleeptime\n", __FUNCTION__);
		g_nIES_WaitIdle = 1;
	}
	else{
		g_nIES_WaitIdle = sleepTimeInMinute;
	}

	sleepTimeInMinute = 0;
	db_retval = FAIL;
	sleepTimeInMinute = db_get_var_int("CONF_SYSTEM", "BusySleeptime", &db_retval);
	if(db_retval == FAIL){
		DBG_SSL_PRINTF_SDH_ERR("%s: Fail to get DB data:BusySleeptime\n", __FUNCTION__);
		g_nIES_WaitBusy = 60;
	}
	else{
		g_nIES_WaitBusy = sleepTimeInMinute;
	}
}

void SendTFTWakeUp()
{
	if((g_nEnergySaveMode == false) && (g_nTFTBackLightOn == false)){
		SetTFTPanelMode(TFT_FB_WAKEUP);
	}
}

void SetTFTPanelMode(int TFTMode)
{
	DBG_SSL_PRINTF_SDH_ERR("%s: TFTMode=%d\n", __FUNCTION__,TFTMode);

	bool touchFlag = false;
	bool fbFlag = false;
	char touchEcho[TFT_DATA_SIZE];
	char fbEcho[TFT_DATA_SIZE];	

	switch(TFTMode){
		case TFT_FB_SLEEP:
			g_nTFTBackLightOn = false;
			fbFlag = true;
			snprintf(fbEcho, TFT_DATA_SIZE, "suspend");
			break;
		case TFT_FB_WAKEUP:
			g_nTFTBackLightOn = true;
			fbFlag = true;
			snprintf(fbEcho, TFT_DATA_SIZE, "unblank");
			break;
		case TFT_TOUCH_SLEEP:
			touchFlag = true;
			snprintf(touchEcho, TFT_DATA_SIZE, "sleep");
			break;
		case TFT_TOUCH_WAKEUP:
			touchFlag = true;
			snprintf(touchEcho, TFT_DATA_SIZE, "wakeup");
			break;
		case TFT_DEEP_SLEEP:
			g_nTFTBackLightOn = false;
			fbFlag = true;
			touchFlag = true;
			snprintf(fbEcho, TFT_DATA_SIZE, "suspend");
			snprintf(touchEcho, TFT_DATA_SIZE, "sleep");
			break;
		case TFT_DEEP_WAKEUP:
			g_nTFTBackLightOn = true;
			fbFlag = true;
			touchFlag = true;
			snprintf(fbEcho, TFT_DATA_SIZE, "unblank");
			snprintf(touchEcho, TFT_DATA_SIZE, "wakeup");
			break;
		default:
			break;
	}

	if(fbFlag){
		DBG_SSL_PRINTF_SDH_ERR("%s: FB_Echo=%s, Size=%d\n", __FUNCTION__,fbEcho, sizeof(fbEcho));
		int fd = open(TFT_FB_PATH, O_RDWR);
		if( fd == -1){
			DBG_SSL_PRINTF_SDH_ERR("[%s] Open Failed FB\n", __FUNCTION__);
			return;
		}
		write(fd, fbEcho, sizeof(fbEcho));
		close(fd);
	}

	if(touchFlag){
		DBG_SSL_PRINTF_SDH_ERR("%s: Touch_Echo=%s, Size=%d\n", __FUNCTION__,touchEcho, sizeof(touchEcho));
		int fd = open(TFT_TOUCH_PATH, O_RDWR);
		if( fd == -1){
			DBG_SSL_PRINTF_SDH_ERR("[%s] Open Failed Touch\n", __FUNCTION__);
			return;
		}
		write(fd, touchEcho, sizeof(touchEcho));
		close(fd);
	}	
}

#define LCD_OFF_PATH "/proc/st7567fb/off"
void set_lcd_reset_into_low(void)
{
	if ( IS_TFTLCD_ATTACHED ){
		SetTFTPanelMode(TFT_DEEP_SLEEP);
	}
	else{
		char off[] = "1";
		int fd = open(LCD_OFF_PATH, O_RDWR);
		if( fd == -1){
			DBG_SSL_PRINTF_SDH_DEBUG("Open Failed %s\n", __FUNCTION__);
			return;
		} 
		write(fd, off, sizeof(off));
		close(fd);
	}
}

#define LCD_RESUME_PATH "/proc/st7567fb/reinit"
void lcd_resume()
{
	char reinit[] = "1";
	int fd = open(LCD_RESUME_PATH, O_RDWR);
	if( fd == -1){
		DBG_SSL_PRINTF_SDH_ERR("Open Failed %s\n", __FUNCTION__);
		return;
	} 
	write(fd, reinit, sizeof(reinit));
	close(fd);
}

void SysEnergySaveMode(ENERGY_SAVE_STATE state, int wakeupAction)
{
	uint32_t msg = 0;
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stWnd_EnergyMode stEnergySaveMode = {0};
	stEnergySaveMode.action = wakeupAction;

	g_nEnergySaveState = state;
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_ENERGY_MODE;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	if(state == ESS_ENERGY_SAVE){
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Send ENERGY_SAVE_MODE\n", __FUNCTION__);
		stEnergySaveMode.mode = ES_ENERGY_SAVE_MODE;
		msg = Power_Saver_mode;
		posix_message_send(lsu_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
#if 0
		system("/app/power_down.sh");
#endif
	}
	else{
#if 0
		system("/app/power_up.sh");
#endif
		if(IS_KARAS){
			DBG_SSL_PRINTF_SDH_ERR("[%s]: Send WAKE_UP\n", __FUNCTION__);
			msg = Wake_Up;
		}
		else{
			if(state == ESS_COLD_WAKE_UP){
				DBG_SSL_PRINTF_SDH_ERR("[%s]: Send COLD WAKE_UP\n", __FUNCTION__);
				msg = Cold_Wake_Up;
			}
			else if(state == ESS_FULL_WAKE_UP){
				DBG_SSL_PRINTF_SDH_ERR("[%s]: Send FULL WAKE_UP\n", __FUNCTION__);
				msg = Wake_Up;
			}
		}
		if(msg != 0){
			posix_message_send(lsu_ctrl_mq, (char*)&msg, sizeof(uint32_t), 0, 0);
		}

		usleep(100*1000);
		if ( IS_TFTLCD_ATTACHED ){
			//Pinetree-S: If PC Job just Touch Wake-up else power key DEEP Wake-up
			if(g_nPCJobWakeUp){	
				SetTFTPanelMode(TFT_TOUCH_WAKEUP);
				stEnergySaveMode.mode = ES_NORMAL_LCD_OFF_MODE;
			}
			else{
				SetTFTPanelMode(TFT_DEEP_WAKEUP);
				stEnergySaveMode.mode = ES_NORMAL_MODE;
			}
		}
		else{
			lcd_resume();	//KARA-S and PINETREE
			stEnergySaveMode.mode = ES_NORMAL_MODE;
		}
	}

	memcpy(msgToSend.data, &stEnergySaveMode, sizeof(stWnd_EnergyMode));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );	
	
}

void NotifySystemJobDone(void)
{
	if(gConfUpdatePending || gHourlyTimeUpdatePending){
		DBG_SSL_PRINTF_SDH_ERR("Pending Conf=%d, TimeUpdate=%d\n", gConfUpdatePending, gHourlyTimeUpdatePending);
	}
	SYS_UI_MESSAGE msgToSend;
	msgToSend.msgSender = 0;
	msgToSend.cmd = 0;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	if((gSystemPrintJobStatus == false) && (gCopyJobStatus == false) && (gFaxJobStatus == false) && (gScanJobStatus == false)){
		if(gConfUpdatePending){
			msgToSend.msgSender = WEB_MONITOR_ID;
			msgToSend.cmd = CMD_US_CONFIG_CHANGED;
			SPChangedService(&msgToSend);
		}
		if(gHourlyTimeUpdatePending){
			stIC_HourlyAlarm stHourlyAlarm;
			stHourlyAlarm.totalSysUpTime = gTotalSysUpTimeInHour;
			msgToSend.msgSender = SYSTEM_ENERGY_SAVE_TIMER;
			msgToSend.cmd = CMD_INT_HOUR_ALARM;
			memcpy(msgToSend.data, &stHourlyAlarm, sizeof(stIC_HourlyAlarm));
			HourlyAlarmService(&msgToSend);
		}
	}
}
//============================== Start: Adaptive Energy Save mode routines ==================
int GetCurrentHourRemainMinute(void)
{
	time_t nowTime;
	int nowRemainMinute;
	//struct tm *localTime;
	
	nowTime=time(NULL);
	//localTime = gmtime(&nowTime );
	//nowRemainMinute = 60 -localTime->tm_min;
	nowRemainMinute = 61 - ((nowTime / 60) % 60);	//Adding one extra minute (61) to adjust remaining seconds.
	DBG_SSL_PRINTF_SDH_DEBUG(" %s: NowRemainMinute=%d \n", __FUNCTION__, nowRemainMinute);	
	return nowRemainMinute;
}

int GetCurrentWeekTime(void)
{
	//Return current slice of hour
	time_t nowSec;
	nowSec=time(NULL);
	nowSec=nowSec/60; //Sec->Min
	nowSec=nowSec/60; //Min->Hour
	//now Hour
	nowSec=nowSec%336;//336=24*7*2 2Week Hour
	return nowSec;
}

int GetCurrentRate(int currH)
{
	//Read Row with WeekTime=currH	
	error_type_t db_retval = FAIL;
	int nowRate = 0;
	char query[QUERY_LENGTH];
	char* retValue = NULL;
	char *table = "RECD_ADJUSTSLEEPTIMER";
	
	memset(query, 0, sizeof(query));
	snprintf(query, QUERY_LENGTH, "SELECT Rate FROM %s WHERE  DateHour=%d", table, currH);
	retValue = db_get_var_query(query, &db_retval);
	if(db_retval == FAIL){
		DBG_SSL_PRINTF_SDH_DEBUG(" %s: Fail to get the DB data\n", __FUNCTION__);
		return -1;
	}
	nowRate = atoi(retValue);
	MEM_FREE_AND_NULL(retValue);
	return nowRate;
}

int ClearNextRate(void)
{
	error_type_t db_retval = FAIL;
	int curH=GetCurrentWeekTime();
	int clrH=(curH+1)%336;
	int rateToClear = 0;
	char query[QUERY_LENGTH];
	char* retValue = NULL;
	char *table = "RECD_ADJUSTSLEEPTIMER";

	memset(query, 0, sizeof(query));
	snprintf(query, QUERY_LENGTH, "SELECT Rate FROM %s WHERE  DateHour=%d", table, clrH);
	retValue = db_get_var_query(query, &db_retval);
	if(db_retval == FAIL){
		DBG_SSL_PRINTF_SDH_DEBUG(" %s: Fail to get DB data\n", __FUNCTION__);
		return -1;
	}
	
	rateToClear = atoi(retValue);
	MEM_FREE_AND_NULL(retValue);
	
	if(rateToClear != 0){
		snprintf(query, QUERY_LENGTH, "UPDATE %s SET Rate=%d WHERE  DateHour=%d", table, 0, clrH);
		db_retval = db_trySQLiteExec(query);
		if(db_retval == FAIL){
			DBG_SSL_PRINTF_SDH_DEBUG(" %s: Fail to Update DB table\n", __FUNCTION__);
		}
	}
	return 0;	
}

int MinuteAdaptiveEnergySaving(void)
{
	int curH = GetCurrentWeekTime();
	int seeH = (curH+168)%336; //168=24*7
	int rate = GetCurrentRate(seeH);
	int retMinute;
	if (rate == -1){
		retMinute = g_nIES_WaitIdle;
	}
	else{
		retMinute = ((rate>0)?g_nIES_WaitBusy:g_nIES_WaitIdle);
	}
	//DBG_SSL_PRINTF_SDH_DEBUG(" %s: WaitIdle=%d, WaitBusy=%d\n", __FUNCTION__, g_nIES_WaitIdle, g_nIES_WaitBusy);
	
	//Must use whole of WaitBusy time. Not Remain.
	//if(retMinute == g_nIES_WaitBusy){
		//retMinute = GetCurrentHourRemainMinute();
	//}
	retMinute = retMinute*60;	//In seconds
	DBG_SSL_PRINTF_SDH_DEBUG(" %s: WeekHour=%d, Rate=%d, TimeOut=%d\n", __FUNCTION__, seeH, rate, retMinute);
	return retMinute; 
}

//============================== End: Adaptive Energy Save mode routines ==================

char* GetCurerntAuthUser(void)
{
	DBG_SSL_PRINTF_SDH_ERR("[%s] AuthType=%d\n", __FUNCTION__, gSystemAuthType);
	if(gSystemAuthType){
		strcpy( gSystemAuthUserId, AuthInfo.authID);
	}
	else{
		memset( gSystemAuthUserId, 0x00, AUTH_BASIC_LENGTH );
		//20160426: Added for Special case to save Auth without AuthEnable
		strcpy( gSystemAuthUserId, AuthInfo.authID);
	}
	return gSystemAuthUserId;
}


void AuthConfService(void)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);

	error_type_t err;
	SYS_UI_MESSAGE msgToSend = {0};
	stAuth_AuthConf stAuthconf = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_AUTH_CONFIG;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	int authEnable = 0;
	int authType = 0;
	error_type_t db_retval = FAIL;

	authEnable = db_get_var_int("CONF_SOLUTIONUP", "AuthEnable", &db_retval);
	if(db_retval == FAIL){
		DBG_SSL_PRINTF_SDH_ERR("[%s] DB Fail to get AuthEnable\n", __FUNCTION__);
		authEnable = 0;
	}

	if(authEnable){
		authType = db_get_var_int("CONF_SOLUTIONSP", "AuthType", &db_retval);
		if(db_retval == FAIL){
			DBG_SSL_PRINTF_SDH_ERR("[%s] DB Fail to get AuthType\n", __FUNCTION__);
			authType = 0;
		}
	}

	gSystemAuthType = authType;
	g_bSystemAuthConfChanged = true;

	switch(authType){
		case AUTH_TYPE_NONE:
			stAuthconf.authType = AUTH_TYPE_NONE;
			stAuthconf.shmID = 0;
			//20160426: Added for Special case to save Auth without AuthEnable
			if(g_pSHM_SolAuthInfo){
				stAuthconf.shmID = shm_get_keyid(SHM_KEY_SOLUTION_AUTH);
			}
			break;
		case AUTH_TYPE_BASIC:
			if(g_pSHM_SolAuthInfo){
				stAuthconf.authType = AUTH_TYPE_BASIC;
				stAuthconf.shmID = shm_get_keyid(SHM_KEY_SOLUTION_AUTH);
			}
			break;
		case AUTH_TYPE_UPRINT_SOL:
			if(g_pSHM_AuthInfo){
				stAuthconf.authType = AUTH_TYPE_UPRINT_SOL;
				stAuthconf.shmID = shm_get_keyid(SHM_KEY_AUTH_INFO);
			}
			break;
		case AUTH_TYPE_WINDOWS:
		case AUTH_TYPE_LDAP:
			///TODO: FUTURE
			break;
		default:
			break;
	}

	DBG_SSL_PRINTF_SDH_ERR("[%s] AuthEnable=%d AuthType=%d\n", __FUNCTION__, authEnable, authType);
	memcpy(&msgToSend.data, &stAuthconf, sizeof(stAuth_AuthConf));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );

	//Send msg to AuthMgr for config changed
	msgToSend.cmd = CMD_INT_SC_AUTH_CONF_CHANGED;
	memcpy(&msgToSend.data, &stAuthconf.authType, sizeof(UINT8));
	err = SysUI_MsgSend(SCARD_RECEIVER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void AuthUserActionRequest(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stAuth_UserActionReq stUserActionReq = {0};
	stAuth_Req stAuthMgrReq = {0};
	bool msgSendFlag = true;

	memcpy(&stUserActionReq, Message->data, sizeof(stAuth_UserActionReq));
	DBG_SSL_PRINTF_SDH_ERR("[%s] User Req=%d\n", __FUNCTION__, stUserActionReq.action);

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_INT_SC_AUTH_REQ;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	switch(stUserActionReq.action){
		case AUTH_ACTION_LOG_IN:
			stAuthMgrReq.action = AUTH_LOGIN;
			if(g_pSHM_AuthInfo){
				//memcpy(AuthInfo.userID, g_pSHM_AuthInfo->userID, MAX_CODE_LENGTH);
				//memcpy(AuthInfo.userPwd, g_pSHM_AuthInfo->userPassword, AUTH_BASIC_LENGTH);
				strcpy(AuthInfo.userID, g_pSHM_AuthInfo->userID);
				strcpy(AuthInfo.userPwd, g_pSHM_AuthInfo->userPassword);
				DBG_SSL_PRINTF_SDH_ERR("[%s] UserID=%s=%s, Pswd=%s=%s\n", __FUNCTION__, g_pSHM_AuthInfo->userID, AuthInfo.userID,
									g_pSHM_AuthInfo->userPassword, AuthInfo.userPwd);
			}
			else{
				DBG_SSL_PRINTF_SDH_ERR("[%s] SHM=NULL\n", __FUNCTION__);
			}
			break;
		case AUTH_ACTION_LOG_OUT:
			stAuthMgrReq.action = AUTH_LOGOUT;
			if(g_pSHM_AuthInfo){
				strcpy(AuthInfo.userID, g_pSHM_AuthInfo->userID);
			}
			else{
				DBG_SSL_PRINTF_SDH_ERR("[%s] SHM=NULL\n", __FUNCTION__);
			}
			break;
		default:
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		memcpy(&msgToSend.data, &stAuthMgrReq, sizeof(stAuth_Req));
		err = SysUI_MsgSend(SCARD_RECEIVER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void AuthSolUserActionResult(SYS_UI_MESSAGE *Message)
{
	//error_type_t err;
	//MESSAGE msg;
	stAuth_SolutionLoginRes stAuthSolLoginRes = {0};

	memcpy(&stAuthSolLoginRes, Message->data, sizeof(stAuth_SolutionLoginRes));
	DBG_SSL_PRINTF_SDH_ERR("[%s] User Req=%d\n", __FUNCTION__, stAuthSolLoginRes.action);

	switch(stAuthSolLoginRes.action){
		case AUTH_ACTION_LOG_IN:	//Login
			if(stAuthSolLoginRes.result == 0){
				if(g_pSHM_SolAuthInfo){
					strcpy(AuthInfo.userID, g_pSHM_SolAuthInfo->userID);
					strcpy(AuthInfo.userPwd, g_pSHM_SolAuthInfo->userPassword);
					strcpy(AuthInfo.authID, g_pSHM_SolAuthInfo->curLoginID);
					DBG_SSL_PRINTF_SDH_ERR("[%s] UserID:%s=%s, Pswd:%s=%s, CurLoginID:%s=%s\n", __FUNCTION__, g_pSHM_SolAuthInfo->userID, AuthInfo.userID,
							g_pSHM_SolAuthInfo->userPassword, AuthInfo.userPwd, g_pSHM_SolAuthInfo->curLoginID, AuthInfo.authID);
				}
			}
			else{
				DBG_SSL_PRINTF_SDH_ERR("[%s] Login Fail\n", __FUNCTION__);
				memset(AuthInfo.authID, 0x00, AUTH_BASIC_LENGTH);
			}
			break;
		case AUTH_ACTION_LOG_OUT: //Logout
			memset(AuthInfo.userID, 0x00, MAX_CODE_LENGTH);
			memset(AuthInfo.userPwd, 0x00, AUTH_BASIC_LENGTH);
			memset(AuthInfo.authID, 0x00, AUTH_BASIC_LENGTH);

			memset(g_pSHM_SolAuthInfo->userID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
			memset(g_pSHM_SolAuthInfo->userPassword, 0x00, SDH_AUTH_CONF_BASIC_MAX);
			memset(g_pSHM_SolAuthInfo->curLoginID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
			break;
		case 3: //Print Result
			//Notify to Parser
			DBG_SSL_PRINTF_SDH_ERR("[%s] Print Auth Result=%d\n", __FUNCTION__, stAuthSolLoginRes.result);
			auth_result = stAuthSolLoginRes.result;
//			msg.msgType = MSG_AUTH_RESULT;
//			msg.param1 = stAuthSolLoginRes.result;
//			msg.param2 = 0;
//			msg.param3 = NULL;
//			err = SYMsgSend( PJLPARSER, &msg );
//			XASSERT( err==OK, err );
			break;
		default:
			break;
	}
}

void AuthUserActionResult(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stAuth_Result stAuthMgrRes = {0};
	stAuth_UserActionRes stUserActionRes = {0};
	stAuth_SolutionAuthReq stSolAuthReq = {0};
	bool msgSendFlag = true;
	int userRegNo = 0;
	error_type_t db_retval = FAIL;
	char query[QUERY_LENGTH];

	memcpy(&stAuthMgrRes, Message->data, sizeof(stAuth_Result));

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_AUTH_RESULT;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	DBG_SSL_PRINTF_SDH_ERR("[%s] AuthType:%d Action:%d Result:%d UserID:%s, UserPW:%s, AuthID:%s, printAuthID:%s, printAuthPW:%s\n", __FUNCTION__,
			gSystemAuthType, stAuthMgrRes.act, stAuthMgrRes.result, AuthInfo.userID, AuthInfo.userPwd, AuthInfo.authID, AuthInfo.printAuthID, AuthInfo.printAuthPW);

	if(stAuthMgrRes.act == AUTH_SOL_AUTH){
		//error_type_t err;
		//MESSAGE msg;
		//msg.msgType = MSG_AUTH_RESULT;
		if(stAuthMgrRes.result == 0){
			auth_result = 0;
		//	msg.param1 = 0;
		}
		else{
			auth_result = 1;
		//	msg.param1 = 1;
		}
//		msg.param2 = 0;
//		msg.param3 = NULL;
//		err = SYMsgSend( PJLPARSER, &msg );
//		XASSERT( err==OK, err );
	}

	if(gSystemAuthType == AUTH_TYPE_BASIC){
		switch(stAuthMgrRes.act){
			case AUTH_SOL_AUTH:	//Sol Auth Test
				stSolAuthReq.authType = 1;
				if(g_pSHM_SolAuthInfo){
					strcpy(g_pSHM_SolAuthInfo->cardNumber, AuthInfo.userID);
					g_pSHM_SolAuthInfo->readFlag = 1;
					//strcpy(g_pSHM_SolAuthInfo->curLoginID, AuthInfo.userID);
					//strcpy(g_pSHM_SolAuthInfo->userPassword, AuthInfo.userPwd);
					//Reset timeout. User login ACT as user action
					TimeoutControl(TIMEOUT_RESET);
				}
				else{
					DBG_SSL_PRINTF_SDH_ERR("[%s] SHM=NULL\n", __FUNCTION__);
				}
				break;
			default:
				msgSendFlag = false;
				break;
		}

		if(msgSendFlag){
			SysWakeUpForUserAttention(EMA_CARD_TAGGING);
			DBG_SSL_PRINTF_SDH_ERR("[%s] SndingToUI cardNumber:%s\n", __FUNCTION__, g_pSHM_SolAuthInfo->cardNumber);
			msgToSend.cmd = CMD_SU_SOL_AUTH_REQ;
			memcpy(&msgToSend.data, &stSolAuthReq, sizeof(stAuth_SolutionAuthReq));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );
		}
	}
	else if(gSystemAuthType == AUTH_TYPE_UPRINT_SOL){
		switch(stAuthMgrRes.act){
			case AUTH_LOGIN:
				if(g_pSHM_AuthInfo){
					//memcpy(g_pSHM_AuthInfo->curLoginID, AuthInfo.authID, SDH_AUTH_CONF_BASIC_MAX);
					strcpy(g_pSHM_AuthInfo->curLoginID, AuthInfo.authID);
					//Get user RegNo from DB
					char *table = "RECD_GROUPADDR";
					char *tmpRegNo = NULL;
					memset(query, 0, sizeof(query));
					snprintf(query, QUERY_LENGTH, "SELECT RegNo FROM %s WHERE Name='%s'", table, AuthInfo.authID);
					tmpRegNo = db_get_var_query(query, &db_retval);
					if(db_retval == OK) {
						if(tmpRegNo != NULL){
							userRegNo = atoi(tmpRegNo);
							MEM_FREE_AND_NULL(tmpRegNo);
						}
					}
					g_pSHM_AuthInfo->curLoginRegNo = userRegNo;
					strcpy(g_pSHM_AuthInfo->emailAddress, AuthInfo.email);
					strcpy(g_pSHM_AuthInfo->ftpAddress, AuthInfo.ftpServer);
					g_pSHM_AuthInfo->ftpPort = AuthInfo.ftpPort;
					strcpy(g_pSHM_AuthInfo->ftpID, AuthInfo.ftpID);
					strcpy(g_pSHM_AuthInfo->ftpPassword, AuthInfo.ftpPwd);
					g_pSHM_AuthInfo->prefer = AuthInfo.prefer;

					//Reset timeout. User login ACT as user action
					TimeoutControl(TIMEOUT_RESET);
				}
				else{
					DBG_SSL_PRINTF_SDH_ERR("[%s] SHM=NULL\n", __FUNCTION__);
				}
				stUserActionRes.action = AUTH_ACTION_LOG_IN;
				stUserActionRes.permission = stAuthMgrRes.permission;
				break;
			case AUTH_LOGOUT:
				stUserActionRes.action = AUTH_ACTION_LOG_OUT;
				break;
			case AUTH_CARD_TAG:
				stUserActionRes.action = AUTH_ACTION_CARD_TAGGED;
				break;
			//case AUTH_SOL_AUTH:
			//	break;
			case AUTH_NET_PRINT:
				if((stAuthMgrRes.result == 0) && (stAuthMgrRes.permission == 1)){
					auth_result = 0; //OK
				}
				else{
					auth_result = 1; //NG
				}
				msgSendFlag = false;
				break;
			default:
				msgSendFlag = false;
				break;
		}

		if(msgSendFlag){
			SysWakeUpForUserAttention(EMA_CARD_TAGGING);
			stUserActionRes.result = stAuthMgrRes.result;
			memcpy(&msgToSend.data, &stUserActionRes, sizeof(stAuth_UserActionRes));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );
		}
	}
}

void AuthParserAuthResult(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stIC_ParserAuthResult stParserAuthResult = {0};
	stAuth_UserActionRes stUserActionRes = {0};
	bool msgSendFlag = true;

	memcpy(&stParserAuthResult, Message->data, sizeof(stIC_ParserAuthResult));

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_AUTH_RESULT;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	DBG_SSL_PRINTF_SDH_ERR("[%s] Parser Auth Result=%d AuthID=%s strlen=%d\n", __FUNCTION__, stParserAuthResult.result, stParserAuthResult.authID, strlen(stParserAuthResult.authID));

	if(stParserAuthResult.result == 0){	//Auth Fail
		stUserActionRes.action = AUTH_ACTION_PRINT_AUTH_FAIL;
		if(g_pSHM_AuthInfo){
			strcpy(g_pSHM_AuthInfo->printAuthFailID, stParserAuthResult.authID);
		}
		else{
			DBG_SSL_PRINTF_SDH_ERR("[%s] SHM=NULL\n", __FUNCTION__);
		}
	}
	else{//Auth Success
		msgSendFlag = false;
	}

	if(msgSendFlag){
		memcpy(&msgToSend.data, &stUserActionRes, sizeof(stAuth_UserActionRes));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void AuthParserAuthRequest(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	UINT8 authMode;
	stAuth_SolutionAuthReq stSolAuthReq = {0};
	stAuth_Req stPrintAuthReq = {0};

	authMode = (UINT8)Message->data[0];
	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_SOL_AUTH_REQ;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	DBG_SSL_PRINTF_SDH_ERR("[%s] AuthType:%d Mode:%d\n", __FUNCTION__, gSystemAuthType, authMode);
	DBG_SSL_PRINTF_SDH_ERR("[%s] Print UserID:%s Pwd:%s\n", __FUNCTION__, AuthInfo.printAuthID, AuthInfo.printAuthPW);
	/* Find!!!!!!!!!!!!!!!! */
	// while(1) {
	// }

	if(gSystemAuthType == AUTH_TYPE_BASIC){
		//Send msg to UI
		stSolAuthReq.authType = 2;
		stSolAuthReq.mode = authMode;
		if(g_pSHM_SolAuthInfo){
			strcpy(g_pSHM_SolAuthInfo->userID, AuthInfo.printAuthID);
			strcpy(g_pSHM_SolAuthInfo->userPassword, AuthInfo.printAuthPW);
			g_pSHM_SolAuthInfo->readFlag = 1;
		}

		//Save user ID for SJM use
		strcpy(AuthInfo.authID, AuthInfo.printAuthID);

		memcpy(&msgToSend.data, &stSolAuthReq, sizeof(stAuth_SolutionAuthReq));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
	else if(gSystemAuthType == AUTH_TYPE_UPRINT_SOL){
		//Send msg to AuthMgr
		msgToSend.cmd = CMD_INT_SC_AUTH_REQ;
		memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

		stPrintAuthReq.action = AUTH_NET_PRINT;
		stPrintAuthReq.jobType = authMode;
		memcpy(&msgToSend.data, &stPrintAuthReq, sizeof(stAuth_Req));
		err = SysUI_MsgSend(SCARD_RECEIVER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void AuthUserActionCountUP(stIC_JobDoneInfo *pJobDoneInfo, AUTH_COUNTUP_JOB_TYPE countUPJobType, AUTH_SOL_JOB_RESULT jobResult)
{
	DBG_SSL_PRINTF_SDH_ERR("[%s] JobID:%d Job Type:%d Page:%d Result:%d\n",
			__FUNCTION__, pJobDoneInfo->jobNum, countUPJobType, pJobDoneInfo->totalPages, jobResult);

	if(gSystemAuthType == AUTH_TYPE_BASIC){
		error_type_t err;
		SYS_UI_MESSAGE msgToSend;
		stAuth_SolutionJobRes stSolJobRes = {0};

		msgToSend.msgSender = SYSTEM_SERVICE_ID;
		msgToSend.cmd = CMD_SU_SOL_AUTH_JOB_RES;
		memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

		stSolJobRes.jobID = pJobDoneInfo->jobNum;
		stSolJobRes.jobType = countUPJobType;
		stSolJobRes.jobResult = jobResult;
		stSolJobRes.shmID = shm_get_keyid(SHM_KEY_SOLUTION_JOB_RES);
		stSolJobRes.jobCount = pJobDoneInfo->totalPages;
		g_pSHM_SolJobRes->duplex = pJobDoneInfo->duplex;
		g_pSHM_SolJobRes->paperSize = pJobDoneInfo->paperSize;
		memset(g_pSHM_SolJobRes->userID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		strcpy(g_pSHM_SolJobRes->userID, pJobDoneInfo->userID);

		switch(countUPJobType){
			case AUTH_JOB_PRINT:
				strcpy(g_pSHM_SolJobRes->fileName, g_sILPrintJobName);
				break;
			case AUTH_JOB_SCAN:
				strcpy(g_pSHM_SolJobRes->fileName, g_sILScanFileName);
				break;
			case AUTH_JOB_COPY:
				strcpy(g_pSHM_SolJobRes->fileName, "");
				break;
			case AUTH_JOB_FAX_SEND:
				strcpy(g_pSHM_SolJobRes->phoneNumber, g_sILFaxNumber);
				strcpy(g_pSHM_SolJobRes->fileName, g_sILFaxTxFileName);
				break;
			default:
				break;
		}

		//DBG_SSL_PRINTF_SDH_ERR("[%s] SHM_KEY:%d solJobRes:%s, %s, %s\n", __FUNCTION__, stSolJobRes.shmID,
		//		g_pSHM_SolJobRes->userID, g_pSHM_SolJobRes->phoneNumber, g_pSHM_SolJobRes->fileName);

		memcpy(&msgToSend.data, &stSolJobRes, sizeof(stAuth_SolutionJobRes));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
	else if(gSystemAuthType == AUTH_TYPE_UPRINT_SOL){
		error_type_t err;
		SYS_UI_MESSAGE msgToSend;
		stAuth_Req stAuthMgrReq = {0};

		msgToSend.msgSender = SYSTEM_SERVICE_ID;
		msgToSend.cmd = CMD_INT_SC_AUTH_REQ;
		memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

		stAuthMgrReq.action = AUTH_COUNTUP;
		stAuthMgrReq.jobType = countUPJobType;
		stAuthMgrReq.jobCount = pJobDoneInfo->totalPages;

		switch(countUPJobType){
			case AUTH_JOB_PRINT:
				strcpy(AuthInfo.printJobID,  pJobDoneInfo->userID);
				//memcpy(AuthInfo.printJobID, userID, AUTH_BASIC_LENGTH);
				//memcpy(AuthInfo.printJobID, AuthInfo.authID, AUTH_BASIC_LENGTH);
				break;
			case AUTH_JOB_SCAN:
				strcpy(AuthInfo.scanJobID,  pJobDoneInfo->userID);
				break;
			case AUTH_JOB_FAX_SEND:
				strcpy(AuthInfo.faxJobID,  pJobDoneInfo->userID);
				break;
			case AUTH_JOB_COPY:
				strcpy(AuthInfo.copyJobID,  pJobDoneInfo->userID);
				break;
			default:
				break;
		}

		memcpy(&msgToSend.data, &stAuthMgrReq, sizeof(stAuth_Req));
		err = SysUI_MsgSend(SCARD_RECEIVER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void ScannerPlotterModeStatusService(uint32_t scnrStatus, uint32_t pltrStatus)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stScnPltr_ScanPltrModeStatus stStatusInfo;
	bool msgSendFlag = true;

	stStatusInfo.scannerStatus = SCN_PLT_MODE_READY;
	stStatusInfo.plotterStatus = SCN_PLT_MODE_READY;
	stStatusInfo.appAcceptable = 0xF0;	//1111 All App acceptable
	/* App Acceptable 
		bit(0) : Print App
		bit(1) : Scan App
		bit(2) : Copy App
		bit(3) : Fax Line
	*/

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_SCANNER_PLOTTER_MODE_STATUS;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	if(pltrStatus == STATUS_ONLINE){
		stStatusInfo.plotterStatus = SCN_PLT_MODE_READY;
	}
	
	if(scnrStatus == SCANAPP_EVENT_IDLE){
		stStatusInfo.scannerStatus = SCN_PLT_MODE_READY;
	}	
	
	if(gSystemPrintJobStatus){	//Printing
		stStatusInfo.scannerStatus = SCN_PLT_MODE_READY;
		stStatusInfo.plotterStatus = SCN_PLT_MODE_BUSY;
		stStatusInfo.appAcceptable = (stStatusInfo.appAcceptable & 0x50);	//0101		
	}
	if(gScanJobStatus){	//Scanning
		stStatusInfo.scannerStatus = SCN_PLT_MODE_BUSY;
		stStatusInfo.plotterStatus = SCN_PLT_MODE_READY;
		stStatusInfo.appAcceptable = (stStatusInfo.appAcceptable & 0x80);	//1000
	}
	if(gCopyJobStatus){	//Copying
		stStatusInfo.scannerStatus = SCN_PLT_MODE_BUSY;
		stStatusInfo.plotterStatus = SCN_PLT_MODE_BUSY;
		stStatusInfo.appAcceptable = (stStatusInfo.appAcceptable & 0x10);	//0001	
	}
	if(gFaxJobStatus){	//Send Fax or Receive Fax
		msgSendFlag = false;
		///TODO:
	}
	
	if(gFrontDoorStatus){	//Front Door Open
		stStatusInfo.plotterStatus = SCN_PLT_MODE_DOOR_OPEN;
		stStatusInfo.appAcceptable = (stStatusInfo.appAcceptable & 0x50);	//0101
	}
	if(gSystemJamStatus){		//Plotter Jam
		stStatusInfo.plotterStatus = SCN_PLT_MODE_JAM;
		stStatusInfo.appAcceptable = (stStatusInfo.appAcceptable & 0x50);	//0101
	}
	if(gSystemSCStatus){		//SC
		stStatusInfo.plotterStatus = SCN_PLT_MODE_SC;
		stStatusInfo.appAcceptable = (stStatusInfo.appAcceptable & 0x00);	//0000
	}
	if(gScanJamStatus){		//Scanner Jam
		stStatusInfo.scannerStatus = SCN_PLT_MODE_JAM;
		stStatusInfo.appAcceptable = (stStatusInfo.appAcceptable & 0x80);	//1000
	}
	
	if(msgSendFlag){
		memcpy(msgToSend.data, &stStatusInfo, sizeof(stScnPltr_ScanPltrModeStatus));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void PlotterModeStatusService(SYS_UI_MESSAGE *Message)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;	
	uint32_t CurStatus; 	
	bool msgSendFlag = true;
	bool isPlotterBusy = false;

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);
	memcpy(&CurStatus, Message->data, sizeof(uint32_t));
	
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PLOTTER_MODE_STATUS;	//Status info: Plotter mode status
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);		

	//Send authentication config message to UIManager when system once on system Online
	if(!gAuthConfSentOnInit){
		gAuthConfSentOnInit = true;
		AuthConfService();
	}

	if(gSystemPrintJobStatus ||gCopyJobStatus || gFaxPrintJobStatus){
		isPlotterBusy = true;
	}

	if( ( (isPlotterBusy == true) && (gSystemJamStatus == true) ) //Printing & JAM
		|| ( (isPlotterBusy == true) && (gSystemSCStatus == true) ) //Printing & SC
		|| (isPlotterBusy == false)){	//No Print JOB

		stScnPltr_PlotterModeStatus stStatusInfo;
		stStatusInfo.status = 0;

		switch(CurStatus)	{
			case STATUS_INFO_CANCELING: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_INFO_CANCELING \n", __FUNCTION__);
				msgSendFlag = false;
				break;
			case STATUS_INFO_PRINTING:		
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_INFO_PRINTING \n", __FUNCTION__);
				stStatusInfo.status = SCN_PLT_MODE_BUSY; //printing: busy
				break;
			case STATUS_ONLINE:		
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_ONLINE \n", __FUNCTION__);
				stStatusInfo.status = SCN_PLT_MODE_READY; //printer: Ready
				break;
			default:	
				msgSendFlag = false;
				break;
		}
		
		if(msgSendFlag){
			memcpy(msgToSend.data, &stStatusInfo, sizeof(stScnPltr_PlotterModeStatus));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );
		}
	}
}

void PlotterDoorOpenService(SYS_UI_MESSAGE *Message)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;	
	uint32_t CurStatus; 	
	bool msgSendFlag = true;
	
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);
	memcpy(&CurStatus, Message->data, sizeof(uint32_t));
	
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PLOTTER_DOOR_STATUS;	//Door Open: Plotter Door open status
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);		

	stScnPltr_PlotterDoorOpenStatus stDoorStatus;
	stDoorStatus.door = 0;
	
	switch(CurStatus)
	{	
		case STATUS_DOOROPEN_FRONT: 
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_DOOROPEN_FRONT \n", __FUNCTION__);
			if(gFrontDoorStatus == false){
				stDoorStatus.door =  bitl(0);		//Front Door Open => bit(0):1
				gFrontDoorStatus = true;

				//Clear Toner last status
				gTonerStatus = gTonerStatus+100;
			}
			else{
				msgSendFlag = false;
			}
			TimeoutControl(TIMEOUT_PAUSE);
			break;
		case STATUS_ONLINE: 
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_ONLINE so Door Front: Closed\n", __FUNCTION__);
			if(gFrontDoorStatus){
				stDoorStatus.door &= ~(bitl(0));	//Front Door Closed => bit(0):0
				gFrontDoorStatus = false;
			}
			else{
				msgSendFlag = false;
			}
			TimeoutControl(TIMEOUT_RESET);
			break;
		case STATUS_DOORCLOSE_FRONT: 
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_DOORCLOSE_FRONT: Closed\n", __FUNCTION__);
			if(gFrontDoorStatus){
				stDoorStatus.door &= ~(bitl(0));	//Front Door Closed => bit(0):0
				gFrontDoorStatus = false;
			}
			else{
				msgSendFlag = false;
			}
			TimeoutControl(TIMEOUT_RESET);
			break;
		default:
			msgSendFlag = false;
			break;
	}	

	if(msgSendFlag){
		SysWakeUpForUserAttention(SW_PLOTTER_DOOR_STATUS_CHANGE);
		memcpy(msgToSend.data, &stDoorStatus, sizeof(stScnPltr_PlotterDoorOpenStatus));	
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void PlotterFeedTrayStatusService(SYS_UI_MESSAGE *Message)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;	
	uint32_t CurStatus; 	
	bool msgSendFlag = true;	
	uint32_t statusTrayNo = 999;
	stScnPltr_PlotterFeedTrayStatus outputTrayStatus = {0};

	memcpy(&CurStatus, Message->data, sizeof(uint32_t));

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = 0;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);	

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SYS_UI_MESSAGE data.size = %d , MAX_SIZE \n", __FUNCTION__,  sizeof(msgToSend.data), SYS_UI_MESSAGE_DATA_LEN);

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = 0x%X  \n", __FUNCTION__, CurStatus);

	if((CurStatus & STATUS_PAPER_OUT) == STATUS_PAPER_OUT) {			
		msgToSend.cmd = CMD_SU_PLOTTER_TRAY_STATUS;	//Plotter feed tray status		
		
		switch(CurStatus)	{		
			case STATUS_PAPER_OUT_TRAY1:	// Tray1
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_PAPER_OUT_TRAY1 \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.paperVolume = MEDIA_TRAY_PAPER_END;
				gPrevVolTray1 = MEDIA_TRAY_PAPER_END;
				break;
			case STATUS_PAPER_OUT_TRAY2:	// Tray 2 Option Tray
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_OUT_TRAY2 \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_2;
				gStFeedTray2Status.paperVolume = MEDIA_TRAY_PAPER_END;
				gPrevVolTray2 = MEDIA_TRAY_PAPER_END;
				break;
			case STATUS_PAPER_OUT_TRAY3:	// Tray 3 Option Tray
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_OUT_TRAY3 \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_3;
				gStFeedTray3Status.paperVolume = MEDIA_TRAY_PAPER_END;
				gPrevVolTray3 = MEDIA_TRAY_PAPER_END;
				break;
			case STATUS_PAPER_OUT_TRAY4:	// MPT
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_OUT_TRAY4 \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_MPT;
				gStFeedTray0Status.paperVolume = MEDIA_TRAY_PAPER_END;
				gPrevVolTray0 = MEDIA_TRAY_PAPER_END;
				break;
			default:	
				msgSendFlag = false;
				break;
		}				
	}
	else	if((CurStatus & STATUS_TRAY) == STATUS_TRAY){
		
		msgToSend.cmd = CMD_SU_PLOTTER_TRAY_STATUS;	//Plotter Feed tray status;
		
		switch(CurStatus) {
		
			case STATUS_OUTTRAY_FULL_CLEAR:	// Stack Full Clear
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_OUTTRAY_FULL_CLEAR \n", __FUNCTION__);
				msgSendFlag = false;
				//gSystemFullStack = false;	//Don't set false here. PrintSubSystem will set false
				//TimeoutControl(TIMEOUT_RESET);
				outputTrayStatus.tray = PLT_TRAY_OUTPUT;	//OUTPUT tray
				outputTrayStatus.paperVolume = MEDIA_TRAY_PAPER_END;	//clear
				msgToSend.cmd = CMD_SU_PLOTTER_TRAY_STATUS;	//Plotter Tray status;
				memcpy(msgToSend.data, &outputTrayStatus, sizeof(stScnPltr_PlotterFeedTrayStatus));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );
				break;
				
				///========== START: Tray1	info Messages =============
			case STATUS_TRAY1_OPEN: /// Tray 1 open
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_OPEN \n", __FUNCTION__); 		
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_OPEN;
				break;
			case STATUS_TRAY1_CLOSE:	/// Tray 1 close
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_CLOSE \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;
			case STATUS_TRAY1_MEDIA_LETTER: /// Tray 1 Media Letter
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_MEDIA_LETTER \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.sizeCode = MEDIASIZE_LETTER;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;
			case STATUS_TRAY1_MEDIA_EXEC:	/// Tray 1 Media Exec
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_MEDIA_EXEC \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.sizeCode = MEDIASIZE_EXECUTIVE;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;
			case STATUS_TRAY1_MEDIA_A4: /// Tray 1 Media A4
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_MEDIA_A4 \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.sizeCode = MEDIASIZE_A4;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;
			case STATUS_TRAY1_MEDIA_B5: /// Tray 1 Media B5
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_MEDIA_B5 \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.sizeCode = MEDIASIZE_B5;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;
			case STATUS_TRAY1_MEDIA_LEGAL:	/// Tray 1 Media Legal
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_MEDIA_LEGAL \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.sizeCode = MEDIASIZE_LEGAL;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;			
			case STATUS_TRAY1_MEDIA_A5: /// Tray 1 Media A5
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_MEDIA_A5 \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.sizeCode = MEDIASIZE_A5;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;
			case STATUS_TRAY1_MEDIA_UNKNOWN:	/// Tray 1 Media Unknown
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_MEDIA_UNKNOWN \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.sizeCode = MEDIASIZE_BAD;
				gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				break;	
				
			case STATUS_TRAY1_PAPER_LOW:	/// Tray 1 Paper Low
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_PAPER_LOW \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.paperVolume = MEDIA_TRAY_PAPER_LOW;
				break;
			case STATUS_TRAY1_PAPER_NORMAL: /// Tray 1 Paper Normal
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY1_PAPER_NORMAL \n", __FUNCTION__);
				statusTrayNo = PLT_TRAY_1;
				gStFeedTray1Status.paperVolume = MEDIA_TRAY_PAPER_FULL;
				
				break;
				///========== END: Tray1  info Messages =============

				///========== START: Tray 2  info Messages =============
			case STATUS_TRAY2_OPEN: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_OPEN \n", __FUNCTION__);
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_OPEN;
				statusTrayNo = PLT_TRAY_2;
				break;	
			case STATUS_TRAY2_CLOSE:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_CLOSE \n", __FUNCTION__);
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;
			case STATUS_TRAY2_MEDIA_LETTER: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_MEDIA_LETTER \n", __FUNCTION__);
				gStFeedTray2Status.sizeCode = MEDIASIZE_LETTER;
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;
			case STATUS_TRAY2_MEDIA_EXEC:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_MEDIA_EXEC \n", __FUNCTION__);
				gStFeedTray2Status.sizeCode= MEDIASIZE_EXECUTIVE;
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;
			case STATUS_TRAY2_MEDIA_A4: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_MEDIA_A4 \n", __FUNCTION__);
				gStFeedTray2Status.sizeCode = MEDIASIZE_A4;
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;
			case STATUS_TRAY2_MEDIA_B5: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_TRAY2_MEDIA_B5 \n", __FUNCTION__);
				gStFeedTray2Status.sizeCode = MEDIASIZE_B5;
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;
			case STATUS_TRAY2_MEDIA_LEGAL:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_MEDIA_LEGAL \n", __FUNCTION__);
				gStFeedTray2Status.sizeCode = MEDIASIZE_LEGAL;
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;			
			case STATUS_TRAY2_MEDIA_A5: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_MEDIA_A5 \n", __FUNCTION__);
				gStFeedTray2Status.sizeCode= MEDIASIZE_A5;
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;
			case STATUS_TRAY2_MEDIA_UNKNOWN:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_MEDIA_UNKNOWN \n", __FUNCTION__);
				gStFeedTray2Status.sizeCode = MEDIASIZE_BAD;
				gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_2;
				break;	
			case STATUS_TRAY2_PAPER_LOW:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_PAPER_LOW \n", __FUNCTION__);
				gStFeedTray2Status.paperVolume = MEDIA_TRAY_PAPER_LOW;
				statusTrayNo = PLT_TRAY_2;
				break;
			case STATUS_TRAY2_PAPER_NORMAL: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY2_PAPER_NORMAL \n", __FUNCTION__);
				gStFeedTray2Status.paperVolume = MEDIA_TRAY_PAPER_FULL;
				statusTrayNo = PLT_TRAY_2;
				break;
				///========== END: Tray2  info Messages =============

				///========== START: Tray 3  info Messages =============
			case STATUS_TRAY3_OPEN: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_OPEN \n", __FUNCTION__);
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_OPEN;
				statusTrayNo = PLT_TRAY_3;
				break;	
			case STATUS_TRAY3_CLOSE:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_CLOSE \n", __FUNCTION__);
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;
			case STATUS_TRAY3_MEDIA_LETTER: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_MEDIA_LETTER \n", __FUNCTION__);
				gStFeedTray3Status.sizeCode = MEDIASIZE_LETTER;
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;
			case STATUS_TRAY3_MEDIA_EXEC:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_MEDIA_EXEC \n", __FUNCTION__);
				gStFeedTray3Status.sizeCode = MEDIASIZE_EXECUTIVE;
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;
			case STATUS_TRAY3_MEDIA_A4: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_MEDIA_A4 \n", __FUNCTION__);
				gStFeedTray3Status.sizeCode = MEDIASIZE_A4;
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;
			case STATUS_TRAY3_MEDIA_B5: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_MEDIA_B5 \n", __FUNCTION__);
				gStFeedTray3Status.sizeCode = MEDIASIZE_B5;
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;
			case STATUS_TRAY3_MEDIA_LEGAL:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_MEDIA_LEGAL \n", __FUNCTION__);
				gStFeedTray3Status.sizeCode = MEDIASIZE_LEGAL;
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;			
			case STATUS_TRAY3_MEDIA_A5: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_MEDIA_A5 \n", __FUNCTION__);
				gStFeedTray3Status.sizeCode = MEDIASIZE_A5;
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;
			case STATUS_TRAY3_MEDIA_UNKNOWN:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_MEDIA_UNKNOWN \n", __FUNCTION__);
				gStFeedTray3Status.sizeCode = MEDIASIZE_BAD;
				gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_3;
				break;	
			case STATUS_TRAY3_PAPER_LOW:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_PAPER_LOW \n", __FUNCTION__);\
				gStFeedTray3Status.paperVolume = MEDIA_TRAY_PAPER_LOW;
				statusTrayNo = PLT_TRAY_3;
				break;
			case STATUS_TRAY3_PAPER_NORMAL: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY3_PAPER_NORMAL \n", __FUNCTION__);
				gStFeedTray3Status.paperVolume = MEDIA_TRAY_PAPER_FULL;
				statusTrayNo = PLT_TRAY_3;
				break;
				///========== END: Tray3  info Messages =============
				///========== START: MPT(Tray 4)  info Messages =============
			case STATUS_TRAY4_OPEN: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_OPEN \n", __FUNCTION__);
				gStFeedTray0Status.trayStatus = MEDIA_TRAY_STATUS_OPEN;
				statusTrayNo = PLT_TRAY_MPT;
				break;	
			case STATUS_TRAY4_CLOSE:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_CLOSE \n", __FUNCTION__);
				gStFeedTray0Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
				statusTrayNo = PLT_TRAY_MPT;
				break;
			case STATUS_TRAY4_MEDIA_LETTER: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_MEDIA_LETTER \n", __FUNCTION__);
				gStFeedTray0Status.sizeCode = MEDIASIZE_LETTER;
				statusTrayNo = PLT_TRAY_MPT;
				break;
			case STATUS_TRAY4_MEDIA_EXEC:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_MEDIA_EXEC \n", __FUNCTION__);
				gStFeedTray0Status.sizeCode = MEDIASIZE_EXECUTIVE;
				statusTrayNo = PLT_TRAY_MPT;
				break;
			case STATUS_TRAY4_MEDIA_A4: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_MEDIA_A4 \n", __FUNCTION__);
				gStFeedTray0Status.sizeCode = MEDIASIZE_A4;
				statusTrayNo = PLT_TRAY_MPT;
				break;
			case STATUS_TRAY4_MEDIA_B5: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_MEDIA_B5 \n", __FUNCTION__);
				gStFeedTray0Status.sizeCode = MEDIASIZE_B5;
				statusTrayNo = PLT_TRAY_MPT;
				break;
			case STATUS_TRAY4_MEDIA_LEGAL:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_MEDIA_LEGAL \n", __FUNCTION__);
				gStFeedTray0Status.sizeCode = MEDIASIZE_LEGAL;
				statusTrayNo = PLT_TRAY_MPT;
				break;			
			case STATUS_TRAY4_MEDIA_A5: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_MEDIA_A5 \n", __FUNCTION__);
				gStFeedTray0Status.sizeCode = MEDIASIZE_A5;
				statusTrayNo = PLT_TRAY_MPT;
				break;
			case STATUS_TRAY4_MEDIA_UNKNOWN:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_MEDIA_UNKNOWN \n", __FUNCTION__);
				gStFeedTray0Status.sizeCode = MEDIASIZE_BAD;
				statusTrayNo = PLT_TRAY_MPT;
				break;	
			case STATUS_TRAY4_PAPER_LOW:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_PAPER_LOW \n", __FUNCTION__);\
				gStFeedTray0Status.paperVolume = MEDIA_TRAY_PAPER_LOW;
				statusTrayNo = PLT_TRAY_MPT;
				break;
			case STATUS_TRAY4_PAPER_NORMAL: 
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_TRAY4_PAPER_NORMAL \n", __FUNCTION__);
				gStFeedTray0Status.paperVolume = MEDIA_TRAY_PAPER_FULL;
				statusTrayNo = PLT_TRAY_MPT;
				break;
				///========== END: MPT (Tray4)  info Messages =============
			default:	
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status =  0x%X Error: Please check Media status value  \n", __FUNCTION__, CurStatus);
				msgSendFlag = false;
				break;
		}	
	}
	else {
		msgSendFlag = false;
	}

	if(msgSendFlag){
		if(statusTrayNo == PLT_TRAY_MPT){
			memcpy(msgToSend.data, &gStFeedTray0Status, sizeof(stScnPltr_PlotterFeedTrayStatus));
			DBG_SSL_PRINTF_SDH_DEBUG(" %s, Tray=%d, Vol=%d, SizeCode=%d, Status=%d\n", __FUNCTION__, gStFeedTray0Status.tray, gStFeedTray0Status.paperVolume, gStFeedTray0Status.sizeCode, gStFeedTray0Status.trayStatus);			
		}
		else if(statusTrayNo == PLT_TRAY_1)	{
			memcpy(msgToSend.data, &gStFeedTray1Status, sizeof(stScnPltr_PlotterFeedTrayStatus));
			DBG_SSL_PRINTF_SDH_DEBUG("  %s, Tray=%d, Vol=%d, SizeCode=%d, Status=%d\n", __FUNCTION__, gStFeedTray1Status.tray, gStFeedTray1Status.paperVolume, gStFeedTray1Status.sizeCode, gStFeedTray1Status.trayStatus);
			if(g_pSHM_FeedTrayStatus){
				//memcpy(g_pSHM_FeedTrayStatus->tray[0], &g_pSHM_FeedTrayStatus, sizeof(stScnPltr_PlotterFeedTrayStatus));
				g_pSHM_FeedTrayStatus->tray[0].paperVolume = gStFeedTray1Status.paperVolume;
				g_pSHM_FeedTrayStatus->tray[0].sizeCode = gStFeedTray1Status.sizeCode;
				g_pSHM_FeedTrayStatus->tray[0].trayStatus = gStFeedTray1Status.trayStatus;
			}
		}
		else if(statusTrayNo == PLT_TRAY_2)	{
			memcpy(msgToSend.data, &gStFeedTray2Status, sizeof(stScnPltr_PlotterFeedTrayStatus));
			DBG_SSL_PRINTF_SDH_DEBUG("  %s, Tray=%d, Vol=%d, SizeCode=%d, Status=%d\n", __FUNCTION__, gStFeedTray2Status.tray, gStFeedTray2Status.paperVolume, gStFeedTray2Status.sizeCode, gStFeedTray2Status.trayStatus);
			if(g_pSHM_FeedTrayStatus){
				g_pSHM_FeedTrayStatus->tray[1].paperVolume = gStFeedTray2Status.paperVolume;
				g_pSHM_FeedTrayStatus->tray[1].sizeCode = gStFeedTray2Status.sizeCode;
				g_pSHM_FeedTrayStatus->tray[1].trayStatus = gStFeedTray2Status.trayStatus;
			}
		}
		else if(statusTrayNo == PLT_TRAY_3)	{
			memcpy(msgToSend.data, &gStFeedTray3Status, sizeof(stScnPltr_PlotterFeedTrayStatus));
			DBG_SSL_PRINTF_SDH_DEBUG("  %s, Tray=%d, Vol=%d, SizeCode=%d, Status=%d\n", __FUNCTION__, gStFeedTray3Status.tray, gStFeedTray3Status.paperVolume, gStFeedTray3Status.sizeCode, gStFeedTray3Status.trayStatus);
			if(g_pSHM_FeedTrayStatus){
				g_pSHM_FeedTrayStatus->tray[2].paperVolume = gStFeedTray3Status.paperVolume;
				g_pSHM_FeedTrayStatus->tray[2].sizeCode = gStFeedTray3Status.sizeCode;
				g_pSHM_FeedTrayStatus->tray[2].trayStatus = gStFeedTray3Status.trayStatus;
			}
		}

		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);				
		XASSERT( err==OK, err );

		if((statusTrayNo == PLT_TRAY_MPT) &&
				((gStFeedTray0Status.paperVolume == MEDIA_TRAY_PAPER_LOW) ||
				 (gStFeedTray0Status.paperVolume == MEDIA_TRAY_PAPER_FULL) ||
				 (gStFeedTray0Status.paperVolume == MEDIA_TRAY_PAPER_END) )){
			if(gLaminationJobStatus){
				SendStatusForLaminationJob(CurStatus);
			}
		}
	}
}

void PlotterJamStatusService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	uint32_t CurStatus; 
	bool msgSendFlag = true;
	stScnPltr_PlotterFeedTrayStatus outputTrayStatus = {0};
	
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);

	memcpy(&CurStatus, Message->data, sizeof(uint32_t));

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PLOTTER_JAM_STATUS;	//Plotter jam status;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	
	stScnPltr_PlotterJamStatus stJamStatus;
	stJamStatus.jamReason = 0;
	stJamStatus.jamStatus = 0;
	
	switch(CurStatus)
	{
		case STATUS_PAPER_JAM_REINSERTCARTRIDGE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_PAPER_JAM_REINSERTCARTRIDGE \n", __FUNCTION__);
			TonerDrumStatusService(Message);
			msgSendFlag = false;
			break;
		case STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE \n", __FUNCTION__);
			TonerDrumStatusService(Message);
			msgSendFlag = false;
			break;
		case STATUS_PAPER_JAM_TONER_BLOCK:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_PAPER_JAM_TONER_BLOCK \n", __FUNCTION__);
			TonerDrumStatusService(Message);
			msgSendFlag = false;
			break;
		case STATUS_PAPER_JAM_TRAY1:	// Tray1
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_TRAY1 \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_TRAY1;
			//stJamStatus.jamStatus |= bitl(1);
			break;	
		case STATUS_PAPER_JAM_TRAY2:	// Tray 2
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_TRAY2 \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_TRAY2;
			break;	
		case STATUS_PAPER_JAM_TRAY3:	// Tray 3
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_TRAY3 \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_TRAY3; 
			break;	
		case STATUS_PAPER_JAM_TRAY4:	// MPT
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_TRAY4 -> MPT  \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_MPT;
			break;				
		case STATUS_PAPER_JAM_EXTSEN_NOT_DET:	// 104 JAM
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_EXTSEN_NOT_DET \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_NOTREACHED_EX_SENSOR;
			break;	
		case STATUS_PAPER_JAM_INPSEN_NOT_REL:	// 105 JAM
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_INPSEN_NOT_REL \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_NOTRELEASED_IN_SENSOR;
			break;	
		case STATUS_PAPER_JAM_EXTSEN_NOT_REL:	// 106 JAM
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_EXTSEN_NOT_REL \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_NOTRELEASED_EX_SENSOR;
			break;	
		case STATUS_PAPER_JAM_DUPLEX:	// 107 JAM
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_DUPLEX \n", __FUNCTION__);
			stJamStatus.jamReason = PLT_JAM_DUPLEX;
			break;	
		case STATUS_PAPER_JAM_FULLSTACK:		
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_PAPER_JAM_FULLSTACK \n", __FUNCTION__);
			msgSendFlag = false;
			if(gSystemFullStack == false){
				msgToSend.cmd = CMD_SU_PLOTTER_TRAY_STATUS;		//Plotter Tray status;
				outputTrayStatus.tray = PLT_TRAY_OUTPUT;	//OUTPUT tray
				outputTrayStatus.paperVolume = MEDIA_TRAY_PAPER_FULL;

				memcpy(msgToSend.data, &outputTrayStatus, sizeof(stScnPltr_PlotterFeedTrayStatus));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );

				ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_OUTPUTFULL, PLT_TRAY_OUTPUT);
			}
			break;
		default:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status =  0x%X Error: Please check status value  \n", __FUNCTION__, CurStatus);
			msgSendFlag = false;
			break;		
	}		
	

	if(msgSendFlag){
		if(gPlotterJamCode != stJamStatus.jamReason){
			gPlotterJamCode = stJamStatus.jamReason;			
			memcpy(msgToSend.data, &stJamStatus, sizeof(stScnPltr_PlotterJamStatus));
			
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );

			ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_PAPERJAM, stJamStatus.jamReason);
			SaveErrorLogInDB(ERR_RECOVERABLE, stJamStatus.jamReason, true);
		}
	}
}

void TonerDrumStatusService(SYS_UI_MESSAGE *Message)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	uint32_t CurStatus;
	int tonerCount = 0;
	error_type_t db_retval = FAIL;
	bool msgSendFlag = true;
	stScnPltr_TonerStatus stTonerStatus = {0};
	
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);

	memcpy(&CurStatus, Message->data, sizeof(uint32_t));

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PLOTTER_TONER_STATUS;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	stTonerStatus.tonerStatus = 0;
	
	switch(CurStatus)
	{
		case STATUS_TONER_LOW:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_TONER_LOW \n", __FUNCTION__);
			stTonerStatus.toner = TONNER_STATUS_LOW;
			break;
		case STATUS_TONER_END:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_TONER_END \n", __FUNCTION__);
			stTonerStatus.toner = TONNER_STATUS_END;
			break;
		case STATUS_TONER_NO_ISSUE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_TONER_NO_ISSUE \n", __FUNCTION__);
			//stTonerStatus.toner = TONNER_STATUS_FULL;
			msgSendFlag = false;
			break;
		case STATUS_PAPER_JAM_TONER_BLOCK:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_PAPER_JAM_TONER_BLOCK \n", __FUNCTION__);
			stTonerStatus.toner = TONNER_STATUS_END;
			if(gTonerStatus == TONNER_STATUS_END){
				msgSendFlag = false;
			}
			break;
		case STATUS_PAPER_JAM_REINSERTCARTRIDGE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_PAPER_JAM_REINSERTCARTRIDGE \n", __FUNCTION__);
			stTonerStatus.toner = TONNER_STATUS_INVALID_CARTRIDGE;
			if(gTonerStatus == TONNER_STATUS_INVALID_CARTRIDGE){
				msgSendFlag = false;
			}
			break;
		case STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE \n", __FUNCTION__);
			stTonerStatus.toner = TONNER_STATUS_ILLEGAL_CARTRIDGE;
			if(gTonerStatus == TONNER_STATUS_ILLEGAL_CARTRIDGE){
				msgSendFlag = false;
			}
			break;
		case STATUS_TONER_REMAIN_CHECK:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]:  Status = STATUS_TONER_REMAIN_CHECK \n", __FUNCTION__);
			stTonerStatus.toner = STATUS_TONER_REMAIN_CHECK;
			break;
		default:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status =  0x%X Error: Please check status value  \n", __FUNCTION__, CurStatus);
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		if(stTonerStatus.toner == STATUS_TONER_REMAIN_CHECK){
			tonerCount = db_get_var_int("INST_TONERCOUNT", "CountInPercent", &db_retval);
			if(db_retval == FAIL){
				tonerCount = 0;
			}
			//CHECK Percentage
			if(tonerCount <= 10){
				stTonerStatus.toner = 1;
			}
			else{
				stTonerStatus.toner = ((tonerCount+9)/20)*2;
			}
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: TonerCountInPercent = %d, Send Toner=%d\n", __FUNCTION__, tonerCount, stTonerStatus.toner);
		}
		memcpy(msgToSend.data, &stTonerStatus, sizeof(stScnPltr_TonerStatus));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
		
		DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Last Toner Status=%d, Cur Toner Status=%d\n", __FUNCTION__, gTonerStatus, stTonerStatus.toner);
		if(gTonerStatus >= 100){
			gTonerStatus = gTonerStatus - 100;
		}

		if(gTonerStatus != stTonerStatus.toner){
			gTonerStatus = stTonerStatus.toner;
			if(stTonerStatus.toner == TONNER_STATUS_LOW){

				ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_TONERLOW, gCurTonerPageCount);
			}
			else if(stTonerStatus.toner == TONNER_STATUS_END){
				ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_TONEREND, TONNER_STATUS_END);
			}
			//Save Toner Error in DB for Error Report
			switch(stTonerStatus.toner){
				case TONNER_STATUS_END:
					SaveErrorLogInDB(ERR_RECOVERABLE, 121, false); //121: Toner Cartridge Change
					break;
				case TONNER_STATUS_INVALID_CARTRIDGE:
					SaveErrorLogInDB(ERR_RECOVERABLE, 120, false);	//120: Re-insert Cartridge
					break;
				case TONNER_STATUS_ILLEGAL_CARTRIDGE:
					SaveErrorLogInDB(ERR_RECOVERABLE, 122, false);	//122: Replace Cartridge Error
					break;
				default:
					break;
			}
		}
	}
}


void SCStatusService(SYS_UI_MESSAGE *Message)
{	
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	uint32_t CurStatus; 
	UINT32 scCode = 0;
	bool msgSendFlag = true;
	memcpy(&CurStatus, Message->data, sizeof(uint32_t));

	switch(CurStatus){			
		case STATUS_FATAL_ENGINE_FEEDMOTORFAIL: 
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_FEEDMOTORFAIL \n", __FUNCTION__);
			scCode = SC_MAIN_MOTOR_LOCK_FAIL;		// Main Motor fail			
			break;
		case STATUS_FATAL_ENGINE_MIRRORMOTORFAIL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_MIRRORMOTORFAIL \n", __FUNCTION__);
			scCode = SC_MIRROR_MOTOR_LOCK_FAIL;		// Mirror Motor fail
			break;
		case STATUS_FATAL_ENGINE_HSYNCFAIL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_HSYNCFAIL \n", __FUNCTION__);
			scCode = SC_PRINTHEAD_LOST_HSYNC;		// HSYNC fail
			break;
		case STATUS_FATAL_ENGINE_VIDEOLONG:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_VIDEOLONG \n", __FUNCTION__);
			scCode = SC_VIDEO_LONG_FAIL;		// Engine Video Long: Video Done Fail
			break;
		case STATUS_FATAL_ENGINE_FUSER300:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_FUSER300 \n", __FUNCTION__);
			scCode = SC_FUSER_UNDER_TEMP_PRINTING;		// Fuser under temperature while printing
			break;
		case STATUS_FATAL_ENGINE_FUSER301:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_FUSER301 \n", __FUNCTION__);
			scCode = SC_FUSER_UNDER_TEMP_STANDBY;		// Fuser under temperature while at standby
			break;
		case STATUS_FATAL_ENGINE_FUSER302:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_FUSER302 \n", __FUNCTION__);
			scCode = SC_FUSER_FAILED_TO_REACH_STANDBY_TEMP;		// Fuser failed to reach standby temperature
			break;
		case STATUS_FATAL_ENGINE_FUSER303:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_FUSER303 \n", __FUNCTION__);
			scCode = SC_FUSER_OVER_TEMP;		// Fuser over temperature
			break;
		case STATUS_FATAL_ENGINE_FUSER304:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_FUSER304 \n", __FUNCTION__);
			scCode = SC_FUSER_THERMISTOR_FAIL;		// Fuser thermistor failure
			break;
		case STATUS_FATAL_ENGINE_FUSER305:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_FUSER305 \n", __FUNCTION__);
			scCode = SC_FUSER_UNDER_TEMP_HEATING;		// Fuser under temperature while heating
			break;
		case STATUS_FATAL_ENGINE_MAINFANMOTORFAIL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_MAINFANMOTORFAIL \n", __FUNCTION__);
			scCode = SC_FAN_MOTOR_LOCK_FAIL;		// Fan motor lock failure
			break;
		case STATUS_FATAL_ENGINE_SUBFANMOTORFAIL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_SUBFANMOTORFAIL \n", __FUNCTION__);
			scCode =SC_SUBFAN_MOTOR_LOCK_FAIL ;		// Sub Fan motor lock failure
			break;
		case STATUS_FATAL_ENGINE_OPTIONFAIL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_OPTIONFAIL \n", __FUNCTION__);
			scCode =SC_OPTION_TRAY_LOST_LINK_ERR;	
			break;			
		case STATUS_FATAL_ENGINE_DEVFANMOTORFAIL:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_FATAL_ENGINE_DEVFANMOTORFAIL \n", __FUNCTION__);
			scCode = SC_DEVFAN_MOTOR_LOCK_FAIL;		// Dev Fan motor lock failure
			break;
		default:
			DBG_SSL_PRINTF_SDH_ERR("[%s]: Status =  0x%X Error: Please check SC Code\n", __FUNCTION__, CurStatus);
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		SendSCStatusToUI(scCode, PRINT_APP_SCID);
	}
}

void SendSCStatusToUI(UINT32 scCode, UINT32 senderModule)
{
	//DBG_SSL_PRINTF_SDH_ERR("[%s] SC Code=%d, Sender=%d\n", __FUNCTION__, scCode, senderModule);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stScnPltr_SCStatus stScStatus = {0};

	//If system is in Energy Save mode then Send Wake-up
	SysWakeUpForUserAttention(EMA_SC_OCCUR);
	
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PLOTTER_SC_STATUS;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	
	stScStatus.code = scCode;

#if AUTO_REBOOT_ON_SWSC
#define NAME_LAST_SC "/app/last.sc.dat"
	FILE *fp = NULL;
	UINT32 last_sc = 0;
	char str[32] = {0,};

	fp = fopen(NAME_LAST_SC, "r");
	if ( fp != NULL ) {
		fscanf(fp, "%d", (int *)&last_sc);
		fclose(fp);
	}

	switch ( scCode ) {
	case SC_SYSTEM_ASSERT_FAIL:
	case SC_SYSTEM_SEG_FAULT:
	case SC_VIDEO_LONG_FAIL:
		if ( last_sc != scCode ) {
			stScStatus.autoReboot = 1;
		}
		else {
			stScStatus.autoReboot = 0;
		}
		break;
	case SC_SYSTEM_ODMA_WAIT_FAIL:
		stScStatus.autoReboot = 1;
		break;
	default:
		stScStatus.autoReboot = 0;
		break;
	}

	fp = fopen(NAME_LAST_SC, "w");
	sprintf( str, "%d", (int)scCode );
	if ( fp != NULL ) {
		fwrite( str, strlen(str), 1, fp);
		fclose(fp);
	}

	if ( stScStatus.autoReboot == 1 ) {
		system ("/app/run_auto_reboot.sh &");
	}
#endif //AUTO_REBOOT_ON_SWSC

	memcpy(msgToSend.data, &stScStatus, sizeof(stScnPltr_SCStatus));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	DBG_SSL_PRINTF_SDH_ERR("[%s] SC Code=%d, Sender=%d, Result=%d\n", __FUNCTION__, scCode, senderModule, err);
	//XASSERT( err==OK, err );

	if((scCode == SC_SYSTEM_BACKUPDB_NONE) 
		||(scCode == SC_SYSTEM_BACKUPDB_INTEGRITY_CHECK_FAIL)
		 ||(scCode == SC_SYSTEM_BACKUPDB_COPY_FAIL)){
		//Don't save SC error in DB
	}
	else{
		SaveErrorLogInDB(ERR_NONRECOVERABLE, stScStatus.code, true);
		ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_INTERVENTION, stScStatus.code);
	}
}

void InputTrayPaperService(SYS_UI_MESSAGE *Message)
{
#if 1
	SYS_UI_MESSAGE msgToSend;
	uint32_t InTrayStatus;
	stPrint_GoPrintResult stPrintResult = {0};
	stCopy_GoCopyResult stCopyResult = {0};
	stPrint_SendFaxStatus stFaxPrintResult = {0};
	
	memcpy(&InTrayStatus, Message->data, sizeof(uint32_t));
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	switch(InTrayStatus)	{
		case STATUS_INPUT_TRAY_SIZE_MISMATCH:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_INPUT_TRAY_SIZE_MISMATCH, InTray, sizeCode \n", __FUNCTION__);
			if(gCopyJobStatus){
				stCopyResult.inTray = get_mismatch_tray();
				stCopyResult.sizeCode = get_mismatch_size();
				stCopyResult.result = RSLT_STOPED_MISMATCH;
				stCopyResult.jobID = g_nCurrentCopyJobID;
				msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
				memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
				CopyGoCopyResultService(&msgToSend);
			}
			else if(gSystemPrintJobStatus){
				//For Pinetree-S Print Job: Send printing/resumed message first then send paper mismatch message
				//Job resumed msg
				stPrintResult.result = RSLT_JOB_RESUMED;
				stPrintResult.jobID = g_nCurrentPrintJobID;
				msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
				memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
				PrintGoPrintResultService(&msgToSend);

				//Job Stopped due to Paper mismatch msg
				stPrintResult.inTray = get_mismatch_tray();
				stPrintResult.sizeCode = get_mismatch_size();
				stPrintResult.result = RSLT_STOPED_MISMATCH;
				stPrintResult.jobID = g_nCurrentPrintJobID;
				msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
				memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
				PrintGoPrintResultService(&msgToSend);
			}
			else if(gFaxPrintJobStatus){
				stFaxPrintResult.inTray = get_mismatch_tray();
				stFaxPrintResult.sizeCode = get_mismatch_size();
				stFaxPrintResult.statusType = FAX_PRINT_STOPED_PAPER_MISMATCH;
				msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
				memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
				FaxSJMResultService(&msgToSend);
			}
			break;
		default:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status =  0x%X Error: Please check status value  \n", __FUNCTION__, InTrayStatus);
			break;
	}
#endif
}

void CheckPCUDrumStatus()
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stScnPltr_PCUStatus stPcuStatus = {0};
		
	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_PLOTTER_PCU_STATUS;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	//If PCU drum current status is LED_FOREVER then notify to UI.
	if(gPCUDrumStatus == DRUM_CHANGE_LED_FOREVER){
		DBG_SSL_PRINTF_SDH_DEBUG("[%s] DrumStatus = DRUM_CHANGE_LED_FOREVER\n", __FUNCTION__);
		stPcuStatus.drumStatus = gPCUDrumStatus;
		memcpy(msgToSend.data, &stPcuStatus, sizeof(stScnPltr_PCUStatus));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void PCUStatusService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	uint32_t pcuStatus; 
	bool msgSendFlag = true;
	stScnPltr_PCUStatus stPcuStatus = {0};
	
	memcpy(&pcuStatus, Message->data, sizeof(uint32_t));
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PLOTTER_PCU_STATUS;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	switch(pcuStatus)	{
		case STATUS_DRUM_LOW_MODE_1:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_DRUM_LOW_MODE_1\n", __FUNCTION__);
			stPcuStatus.drumStatus = DRUM_CHANGE;
			break;
		case STATUS_DRUM_LOW_MODE_2:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_DRUM_LOW_MODE_2\n", __FUNCTION__);
			stPcuStatus.drumStatus = DRUM_CHANGE_LED_ONE_MIN;
			break;
		case STATUS_DRUM_LOW_MODE_3:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_DRUM_LOW_MODE_3\n", __FUNCTION__);
			stPcuStatus.drumStatus = DRUM_CHANGE_LED_FOREVER;
			break;
		case STATUS_DRUM_END:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_DRUM_END\n", __FUNCTION__);
			stPcuStatus.drumStatus = DRUM_END_LOCK;
			break;
		default:
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X Error: Please check status value  \n", __FUNCTION__, pcuStatus);
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		gPCUDrumStatus = stPcuStatus.drumStatus;
		memcpy(msgToSend.data, &stPcuStatus, sizeof(stScnPltr_PCUStatus));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
#if 1	//In Future Change it to PrintSubSystem
		stPrint_GoPrintResult stPrintResult = {0};
		stCopy_GoCopyResult stCopyResult = {0};
		stPrint_SendFaxStatus stFaxPrintResult = {0};
		//Send GoPrintResult or GoCopyResult for DRUM Empty
		if(gPCUDrumStatus == DRUM_CHANGE_LED_FOREVER){
			if(gCopyJobStatus){
				stCopyResult.result = RSLT_STOPED_DRUM;
				stCopyResult.jobID = g_nCurrentCopyJobID;
				msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
				memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
				CopyGoCopyResultService(&msgToSend);
			}
			else if(gFaxPrintJobStatus){
				stFaxPrintResult.statusType = FAX_PRINT_STOPED_DRUM_END;
				msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
				memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
				FaxSJMResultService(&msgToSend);
			}
			else if(gSystemPrintJobStatus){
				//For Pinetree-S Print Job: Send printing/resumed message first then send DRUM_LOW message
				//Job resumed msg
				stPrintResult.result = RSLT_JOB_RESUMED;
				stPrintResult.jobID = g_nCurrentPrintJobID;
				msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
				memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
				PrintGoPrintResultService(&msgToSend);

				stPrintResult.result = RSLT_STOPED_DRUM;
				stPrintResult.jobID = g_nCurrentPrintJobID;
				msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
				memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
				PrintGoPrintResultService(&msgToSend);
			}
		}
#endif
		if(gPCUDrumStatus == DRUM_END_LOCK){
			ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_DRUMCHANGE, gPCUDrumStatus);
		}
	}
}

void PCUResetService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	MESSAGE msg;
	stScnPltr_PCUReset stPcuReset = {0};
	stScnPltr_PCUStatus stPcuStatus = {0};
	
	memcpy(&stPcuReset, Message->data, sizeof(stScnPltr_PCUReset));
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PLOTTER_PCU_STATUS;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);		

	if(stPcuReset.pcuReset == DRUM_CARTRIDGE_RESET){
		//Reset drum status
		gPCUDrumStatus = DRUM_NORMAL;
		//Send RESET DRUM msg to PM
		msg.msgType = MSG_STATUS_DRUM_RESET;
		msg.param1 = DRUM_CARTRIDGE_RESET;
		msg.param2 = 0;
		msg.param3 = NULL;
		err = SYMsgSend( PRINTMGRID, &msg );
		XASSERT( err==OK, err ); 
		
		//Send DRUM_NORMAL msg to UI	
		stPcuStatus.drumStatus = DRUM_NORMAL;
		memcpy(msgToSend.data, &stPcuStatus, sizeof(stScnPltr_PCUStatus));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
	else if(stPcuReset.pcuReset == FUSER_UNIT_RESET){
		//Reset Fuser unit
		gPCUDrumStatus = DRUM_NORMAL;
		//Send RESET DRUM msg to PM
		msg.msgType = MSG_STATUS_FUSER_RESET;
		msg.param1 = FUSER_UNIT_RESET;
		msg.param2 = 0;
		msg.param3 = NULL;
		err = SYMsgSend( PRINTMGRID, &msg );
		XASSERT( err==OK, err );
	}
	else{
		DBG_SSL_PRINTF_SDH_ERR("[%s] unknown action=%d\n", __FUNCTION__, stPcuReset.pcuReset);
	}
}

void SaveErrorLogInDB( int ErrorType, int ErrorCode, bool AllowDuplicate)
{
	int pageCount = 0;
	error_type_t db_retval = FAIL;
	char query[QUERY_LENGTH];
	char *retValuePtr = NULL;
	int lastErrorCode = 0;

	//To stop duplicate entry in ErrorLog Table
	if(AllowDuplicate == false){
		memset(query, 0, sizeof(query));
		snprintf(query, QUERY_LENGTH, "SELECT ErrorDesc FROM RECD_ERRLOG WHERE rowid IN (SELECT rowid FROM RECD_ERRLOG ORDER BY rowid DESC LIMIT 1);");
		retValuePtr = db_get_var_query(query, &db_retval);
		if(db_retval == OK){
			if(retValuePtr != NULL){
				lastErrorCode = atoi(retValuePtr);
				MEM_FREE_AND_NULL(retValuePtr);
				DBG_SSL_PRINTF_SDH_ERR("[%s] ErrorCode Last=%d Now=%d\n", __FUNCTION__, lastErrorCode, ErrorCode);
				if(lastErrorCode == ErrorCode){
					return;
				}
			}
		}
	}

	db_retval = FAIL;
	pageCount = db_get_var_int("INST_MACHINECOUNT", "TotalPrint", &db_retval);
	if(db_retval == FAIL){
		pageCount = 0;
	}

	db_retval = FAIL;
	time_t timeNow = time(NULL);
	
	db_set_recd_err_log(ErrorType, ErrorCode, g_nCurrentJobType, g_nCurrentPrintJobID, timeNow, 0, pageCount, 0, "127001", "1");
	
#if 0
	int ErrType = db_get_var_int("RECD_ERRLOG", "ErrorType", &db_retval);
	int ErrDesc = db_get_var_int("RECD_ERRLOG", "ErrorDesc", &db_retval);
	int JobType = db_get_var_int("RECD_ERRLOG", "SubJobType", &db_retval);
	int JobID = db_get_var_int("RECD_ERRLOG", "JobID", &db_retval);
	int ErrDate = db_get_var_int("RECD_ERRLOG", "Date", &db_retval);
	int pageCnt = db_get_var_int("RECD_ERRLOG", "PageCount", &db_retval);

	time_t timeOld = ErrDate;
	struct tm *stTm = localtime(&timeOld);
	
	DBG_SSL_PRINTF_SDH_ERR( " \n%d %d %d %d %d %d\n", stTm->tm_mon+1, stTm->tm_mday, stTm->tm_year+1900, stTm->tm_hour, stTm->tm_min, stTm->tm_sec );
	DBG_SSL_PRINTF_SDH_ERR(" : %s : ErrorType %d=  0x%X, ErrorDesc=%d, JobType=%d, JobID=%d, Date=%d, PageCount=%d \n", 
		__FUNCTION__, ErrType, ErrType, ErrDesc, JobType, JobID, ErrDate,  pageCnt);
#endif
}

void PrintWrongImageService(SYS_UI_MESSAGE *Message)
{
	uint32_t printStatus = 0;
	memcpy(&printStatus, Message->data, sizeof(uint32_t));

	DBG_SSL_PRINTF_SDH_ERR("[%s] Status=0x%X\n", __FUNCTION__, printStatus);
	if(printStatus == STATUS_WRONG_IMAGE_SIZE_CANCEL_REQ){
		gPrintCancelReason = printStatus;

		pthread_mutex_lock( &gSysPosixTimerMutex );
		g_nTimelyTimeout = g_nWrongImageTimedWait;
		pthread_mutex_unlock( &gSysPosixTimerMutex );
	}
}

void PrintPageDoneInfoService(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	stIC_PrintPageDoneInfo stPrintPageDoneInfo;
	memcpy(&stPrintPageDoneInfo, Message->data, sizeof(stIC_PrintPageDoneInfo));

	if(Message->msgSender == JOBMGRID){
		DBG_SSL_PRINTF_SDH_ERR("PageDone: ID:%d DocType:%d SheetPages=%d, PagesPrinted:%d PageCount=%d\n",
			stPrintPageDoneInfo.jobNum, stPrintPageDoneInfo.jobDocType, stPrintPageDoneInfo.sheetPageCount, stPrintPageDoneInfo.pagesPrinted, stPrintPageDoneInfo.pageCount);

		gPrintInform.totalPage = stPrintPageDoneInfo.pageCount;
	
		switch(stPrintPageDoneInfo.jobDocType){
			case e_FAX:
			case e_FAX_LONGPAGE:
				FaxPrintJobDoneNotify(stPrintPageDoneInfo.sheetPageCount, stPrintPageDoneInfo.jobDocType, JOBMGRID, 2);
				break;
			default:
				break;
		}
	}
}

stIC_JobDoneInfo* SysManageScanJobList(int reqType, stIC_JobDoneInfo *pNewInfo, int param1)
{
	ASSERT(pNewInfo);
	DBG_SSL_PRINTF_SDH_ERR("[%s]: req:%d for job ID:%d\n", __FUNCTION__, reqType, pNewInfo->jobNum);

	//Step 1. Search if scan job node exist
	stIC_JobDoneInfo* pResultInfo = NULL;
	ATLISTENTRY* pScanJobNode = NULL;
	stIC_SysJobInfoList* pScanJobInfo = NULL;
	UINT8 listCount = 0;
	pScanJobNode = ATListHead(&g_ScanJobList);
	while(pScanJobNode != NULL){
		pScanJobInfo = CONTAINING_RECORD(pScanJobNode, stIC_SysJobInfoList, infoNode);
		if (pScanJobInfo && (pScanJobInfo->pInfo->jobNum == pNewInfo->jobNum)){
			DBG_SSL_PRINTF_SDH_ERR("[%s]: Scan job found at:%d\n", __FUNCTION__, listCount);
			pResultInfo = pScanJobInfo->pInfo;
			break;
		}
		pScanJobInfo = NULL;
		++listCount;
		pScanJobNode = ATListNext(&g_ScanJobList, pScanJobNode);
	}

	if(reqType == SCAN_INFO_ADD){
		//1. Search if scan job already exist
		//2. If yes then just append data
		//3. If not then create new node and set data

		if(pResultInfo){
			//Step 2. If found then just append data
			//In the case of FB multipage scan, ScanMan sends jobdone msg but ScanApp waiting for user response.
			//20141114: Now it seems that ScanMan's FB scanning is same as ADF, so this code for fail safe case.
			pResultInfo->totalPages += pNewInfo->totalPages;
			DBG_SSL_PRINTF_SDH_ERR("[%s]: appended Scan JobInfoList pages:%d\n",
					__FUNCTION__, pResultInfo->totalPages);
		}
		else{
			//Step 3: If Scan job not exist then create new
			//g_ScanJobList
			stIC_SysJobInfoList *pScanJobInfo = (stIC_SysJobInfoList*)MEM_CALLOC(1, sizeof(stIC_SysJobInfoList));
			ASSERT(pScanJobInfo);
			stIC_JobDoneInfo *pScanJobDoneInfo = (stIC_JobDoneInfo*)MEM_CALLOC(1, sizeof(stIC_JobDoneInfo));
			ASSERT(pScanJobDoneInfo);
			pScanJobInfo->pInfo = pScanJobDoneInfo;
			pScanJobDoneInfo->jobNum = pNewInfo->jobNum;
			memset(pScanJobDoneInfo->userID, 0x00, JOB_INFO_USER_ID_SIZE);
			strcpy(pScanJobDoneInfo->userID, pNewInfo->userID);
			pScanJobDoneInfo->jobDocType = pNewInfo->jobDocType;
			pScanJobDoneInfo->paperSize = pNewInfo->paperSize;
			pScanJobDoneInfo->duplex = pNewInfo->duplex;
			pScanJobDoneInfo->totalPages = pNewInfo->totalPages;
			ATInsertTailList(&g_ScanJobList, &pScanJobInfo->infoNode);
			pResultInfo = pScanJobInfo->pInfo;
			DBG_SSL_PRINTF_SDH_ERR("[%s]: Created new Scan JobInfoList pages:%d\n",
					__FUNCTION__, pResultInfo->totalPages);
		}
	}
	else if(reqType == SCAN_INFO_DELETE){
		if(pResultInfo){
			ATRemoveEntryList(&pScanJobInfo->infoNode);
			MEM_FREE_AND_NULL(pScanJobInfo->pInfo);
			MEM_FREE_AND_NULL(pScanJobInfo);
			pResultInfo = NULL;
			DBG_SSL_PRINTF_SDH_ERR("[%s]: deleted Scan JobInfoList\n", __FUNCTION__, listCount);
		}
		else{
			DBG_SSL_PRINTF_SDH_ERR("[%s] error NULL pointer cannot delete scan JobInfoList\n", __FUNCTION__);
		}
	}
	else if(reqType == SCAN_INFO_COUNTUP){
		if(pScanJobInfo){
			if(pScanJobInfo->destFlag){
				pScanJobInfo->destCount--;
				pScanJobInfo->jobResult += pNewInfo->copies;
			}
			else{
				pScanJobInfo->destFlag = 1;
				pScanJobInfo->destCount = param1 - 1;//One job is already done so set -1
				pScanJobInfo->jobResult = pNewInfo->copies;
			}
			DBG_SSL_PRINTF_SDH_ERR("[%s] CountUP flag:%d remain destCount:%d result:%d\n",
					__FUNCTION__, pScanJobInfo->destFlag,
					pScanJobInfo->destCount, pScanJobInfo->jobResult);
			//If destination count is 0 then send CountUP information
			if(pScanJobInfo->destCount == 0){
				if(pScanJobInfo->jobResult){
					AuthUserActionCountUP(pScanJobInfo->pInfo, AUTH_JOB_SCAN, AUTH_SOL_JOB_OK);
				}
				else{
					AuthUserActionCountUP(pScanJobInfo->pInfo, AUTH_JOB_SCAN, AUTH_SOL_JOB_NG);
				}

				//ScanToX job is finished, remove node and mem_free data
				ATRemoveEntryList(&pScanJobInfo->infoNode);
				MEM_FREE_AND_NULL(pScanJobInfo->pInfo);
				MEM_FREE_AND_NULL(pScanJobInfo);
				pResultInfo = NULL;
			}
		}
		else{
			DBG_SSL_PRINTF_SDH_ERR("[%s] error NULL pointer cannot count ScanToX destination\n", __FUNCTION__);
		}
	}

	return pResultInfo;
}

void JobDoneInfoService(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	stIC_JobDoneInfo stJobDoneInfo;
	int deleteScanInfoNode = 0;
	memcpy(&stJobDoneInfo, Message->data, sizeof(stIC_JobDoneInfo));
	error_type_t dbErr = FAIL;

	if(Message->msgSender == JOBMGRID){
		//if(stJobDoneInfo.userID[0] != 0x00){
		if(strlen(stJobDoneInfo.userID)){
			DBG_SSL_PRINTF_SDH_ERR("JobID:%d UserID=%s DocType:%d PagesPrinted:%d\n",
				stJobDoneInfo.jobNum, stJobDoneInfo.userID, stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages);
			if(karasUprintSettingFlag == true) {
				gPrintInform.totalCopy = stJobDoneInfo.copies;
				gPrintInform.totalRealPrint = stJobDoneInfo.totalPages;
				DBG_SSL_PRINTF_SDH_ERR("KHDBG : totalPage -> %u, totalCopy -> %u, realPrinting -> %d\n", gPrintInform.totalPage, gPrintInform.totalCopy, gPrintInform.totalRealPrint);
				DBG_SSL_PRINTF_SDH_ERR("KHDBG : authID = %s , authPW = %s\n", AuthInfo.printAuthID, AuthInfo.printAuthPW);
				DBG_SSL_PRINTF_SDH_ERR("KHDBG : File name = %s\n", gJobName);
				DBG_SSL_PRINTF_SDH_ERR("KHDBG : Encode name = %s\n", UrlEncode(gJobName));
				

				stPrintInformationJobLog printInform = {0};
				// if(gPrintInform.totalCopy > 1)
				// 	if(gPrintInform.totalRealPrint >= gPrintInform.totalPage / gPrintInform.totalCopy)
				// 		gPrintInform.totalRealPrint = gPrintInform.totalPage / gPrintInform.totalCopy;

				printInform.count = gPrintInform.totalRealPrint;
				strncpy(printInform.serial, db_get_var("INST_MACHINE", "machineserialno", &dbErr), sizeof(printInform.serial));
				printInform.jobId = stJobDoneInfo.jobNum;
				strncpy(printInform.fileName, UrlEncode(gJobName), sizeof(printInform.fileName));
				strncpy(printInform.paperSize, ChangePaperSizeToString((UINT16)stJobDoneInfo.paperSize), sizeof(printInform.paperSize));
				printInform.duplex = stJobDoneInfo.duplex;
				GoJobLog(&printInform);

				memset(&gPrintInform, 0, sizeof(gPrintInform));
			}
			switch(stJobDoneInfo.jobDocType){
				case e_COPYPAGE:
					AuthUserActionCountUP(&stJobDoneInfo, AUTH_JOB_COPY, AUTH_SOL_JOB_OK);
					break;
				case e_NETIOPAGE:
				case e_PIPEIOPAGE:
//				case e_SYSTEMREPORT:
//				case e_NETWORKREPORT:
//				case e_SPREPORT:
//				case e_FAXTXRXREPORT:
//				case e_FAXTCR:
//				case e_T30LOGREPORT:
//				case e_MEMTXJOBLIST:
//				case e_TCRFORFAXSERVICE:
//				case e_ERRLOGREPORT:
//				case e_ADDRESSBOOK:
				case e_USBDRIVEJPGPAGE:
				case e_USBDRIVEPDFPAGE:
					AuthUserActionCountUP(&stJobDoneInfo, AUTH_JOB_PRINT, AUTH_SOL_JOB_OK);
					break;
				default:
					break;
			}
		}
		else{
			DBG_SSL_PRINTF_SDH_ERR("JobID:%d UserID=NULL strlen=%d DocType:%d Copy:%u PagesPrinted:%d\n",
				stJobDoneInfo.jobNum, strlen(stJobDoneInfo.userID), stJobDoneInfo.jobDocType, stJobDoneInfo.copies ,stJobDoneInfo.totalPages);
		}
	}
	else if(Message->msgSender == SCANMANID){
		//For Scanner save info if JobID is available or UserID is available
		if(strlen(stJobDoneInfo.userID) || stJobDoneInfo.jobNum){
			DBG_SSL_PRINTF_SDH_ERR("JobID:%d UserID=%s JobType:%d PagesScanned:%d\n",
				stJobDoneInfo.jobNum, stJobDoneInfo.userID, stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages);

			switch(stJobDoneInfo.jobDocType){
				//case ejob_Copy:
				case ejob_ScanToHost:
				case ejob_HttpScan:
				case ejob_ScanToEmail:
				case ejob_ScanToMemCard:
				{
					g_pStScanJobDoneInfo = SysManageScanJobList(SCAN_INFO_ADD, &stJobDoneInfo, 0);
					ASSERT(g_pStScanJobDoneInfo);
				}
					break;
#if 0 //20150528 LALIT: Do not send FaxTx count-up here. Only scanning is finished, so when actual sending done then send count-up.
				//case ejob_Fax:
				case ejob_ScanFax:
					AuthUserActionCountUP(&stJobDoneInfo, AUTH_JOB_FAX_SEND, AUTH_SOL_JOB_OK);
#endif
					break;
				default:
					break;
			}
		}
		else{
			DBG_SSL_PRINTF_SDH_ERR("JobID:%d UserID=NULL strlen=%d JobType:%d PagesScanned:%d\n",
				stJobDoneInfo.jobNum, strlen(stJobDoneInfo.userID), stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages);
		}
	}
	else if(Message->msgSender == SYSTEM_SERVICE_ID){
		//Delete ScanJobList for ScanToHost and ScanToUSB
		if(stJobDoneInfo.jobDocType == AUTH_JOB_SCAN){
			deleteScanInfoNode = 1;
		}

		if(strlen(stJobDoneInfo.userID)){
			DBG_SSL_PRINTF_SDH_ERR("JobID:%d UserID=%s JobType:%d notifyPages:%d\n",
				stJobDoneInfo.jobNum, stJobDoneInfo.userID, stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages);

			switch(stJobDoneInfo.jobDocType){
				case AUTH_JOB_SCAN:
					AuthUserActionCountUP(&stJobDoneInfo, AUTH_JOB_SCAN, AUTH_SOL_JOB_OK);
					break;
				case AUTH_JOB_FAX_SEND:
				{
					//Received FaxJobResult as stJobDoneInfo.copies
					int faxTxJobResult = stJobDoneInfo.copies;
					if(faxTxJobResult >= g_nILFaxLastDestFilter){
						faxTxJobResult -= g_nILFaxLastDestFilter;
					}
					AuthUserActionCountUP(&stJobDoneInfo, AUTH_JOB_FAX_SEND, faxTxJobResult);
					break;
				}
				default:
					break;
			}
		}
		else{
			DBG_SSL_PRINTF_SDH_ERR("JobID:%d UserID=NULL strlen=%d JobType:%d notifyPages:%d result:%d\n",
				stJobDoneInfo.jobNum, strlen(stJobDoneInfo.userID), stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages, stJobDoneInfo.copies);

			//Received FaxJobResult as stJobDoneInfo.copies
			int faxTxJobResult = stJobDoneInfo.copies;
			if(faxTxJobResult >= g_nILFaxLastDestFilter){
				faxTxJobResult -= g_nILFaxLastDestFilter;
			}
			//If userID is NULL still notify for special case like below
			if((stJobDoneInfo.jobDocType == AUTH_JOB_FAX_SEND) &&
					((faxTxJobResult == AUTH_SOL_JOB_DEL_OK) ||
					 (faxTxJobResult == AUTH_SOL_JOB_DEL_NG) ||
					 (faxTxJobResult == AUTH_SOL_JOB_CANCELLED_BY_USER) ||
					 (faxTxJobResult == AUTH_SOL_JOB_FILENAME_MISMATCH) ) ){
				AuthUserActionCountUP(&stJobDoneInfo, AUTH_JOB_FAX_SEND, faxTxJobResult);
			}
		}
	}

#ifdef HAVE_IMAGELOG
	char jobTypeChar = '0';
	int subJobType = 0;
	if(Message->msgSender == JOBMGRID){
		DBG_SSL_PRINTF_SDH_ERR("IMAGE_LOG: JobID:%d UserID=%s DocType:%d PagesPrinted:%d\n",
			stJobDoneInfo.jobNum, stJobDoneInfo.userID, stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages);

		switch(stJobDoneInfo.jobDocType){
			case e_COPYPAGE:
				jobTypeChar = 'C';
				break;
			case e_NETIOPAGE:
			case e_PIPEIOPAGE:
//				case e_SYSTEMREPORT:
//				case e_NETWORKREPORT:
//				case e_SPREPORT:
//				case e_FAXTXRXREPORT:
//				case e_FAXTCR:
//				case e_T30LOGREPORT:
//				case e_MEMTXJOBLIST:
//				case e_TCRFORFAXSERVICE:
//				case e_ERRLOGREPORT:
//				case e_ADDRESSBOOK:
			case e_USBDRIVEJPGPAGE:
			case e_USBDRIVEPDFPAGE:
				jobTypeChar = 'P';
				break;
			//FAX Receive
			case e_FAX:
			case e_FAX_LONGPAGE:
				jobTypeChar = 'F';
				subJobType = 2;
				break;
			default:
				break;
		}
	}
#if 0 //20150528 LALIT: If FB scan then scanman send jobend but ScanApp waiting for user input. To solve FB scan count problem.
	// Do not sent FAX send job done(ImageLog) on Scanning finished. But send on Actual job send.
	else if(Message->msgSender == SCANMANID){
		DBG_SSL_PRINTF_SDH_ERR("IMAGE_LOG: JobID:%d UserID=%s JobType:%d PagesScanned:%d\n",
			stJobDoneInfo.jobNum, stJobDoneInfo.userID, stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages);

		switch(stJobDoneInfo.jobDocType){

			//case ejob_Copy:
			case ejob_ScanToHost:
			case ejob_HttpScan:
			case ejob_ScanToEmail:
			case ejob_ScanToMemCard:
				jobTypeChar = 'S';
				break;
			//FAX Send
			//case ejob_Fax:
			case ejob_ScanFax:
				jobTypeChar = 'F';
				subJobType = 1;
				break;
			default:
				break;
		}
	}
#endif
	else if(Message->msgSender == SYSTEM_SERVICE_ID){
		DBG_SSL_PRINTF_SDH_ERR("IMAGE_LOG: JobID:%d UserID=%s JobType:%d notifyPages:%d\n",
			stJobDoneInfo.jobNum, stJobDoneInfo.userID, stJobDoneInfo.jobDocType, stJobDoneInfo.totalPages);

		switch(stJobDoneInfo.jobDocType){
			case AUTH_JOB_SCAN:
			case AUTH_JOB_SCAN_TO_X:
				jobTypeChar = 'S';
				break;
			case AUTH_JOB_FAX_SEND:
			{
				if(stJobDoneInfo.copies >= g_nILFaxLastDestFilter){
					jobTypeChar = 'F';
					subJobType = 1;
				}
				break;
			}
			default:
				break;
		}
	}

	//set_metafile_end_info(jobTypeChar, char* _destination, int _copy, char* _filename, int _originalpages);
	if(jobTypeChar == 'C')
	{
		set_metafile_end_info(jobTypeChar, subJobType, NULL, stJobDoneInfo.copies, /*filename*/NULL, stJobDoneInfo.totalPages, stJobDoneInfo.jobNum);
	}
	else if(jobTypeChar == 'P')
	{
		set_metafile_end_info(jobTypeChar, subJobType, NULL, stJobDoneInfo.copies, g_sILPrintJobName, stJobDoneInfo.totalPages, stJobDoneInfo.jobNum);
		//memset(g_sILPrintJobName, 0x00, PRINT_JOB_NAME_SIZE);
	}
	else if(jobTypeChar == 'S')
	{
		///TODO: Get Scan destination
		set_metafile_end_info(jobTypeChar, subJobType, /*destination*/ NULL, stJobDoneInfo.copies, NULL, stJobDoneInfo.totalPages, stJobDoneInfo.jobNum);
	}
	else if(jobTypeChar == 'F')
	{
		set_metafile_end_info(jobTypeChar, subJobType, g_sILFaxNumber, 1, NULL, stJobDoneInfo.totalPages, stJobDoneInfo.jobNum);
		//memset(g_sILFaxNumber, 0x00, SDH_FAX_LEN_PHONE_NO);
	}
#endif
	if(deleteScanInfoNode){
		//Delete ScanJobInfo node from list, no need to check return value
		SysManageScanJobList(SCAN_INFO_DELETE, &stJobDoneInfo, 0);
	}
}

void SendJobDoneInfo(UINT32 sender, stIC_JobDoneInfo *pJobDoneInfo, UINT8 jobType, int jobResult)
{
	SYS_UI_MESSAGE msg;
	msg.msgSender = sender;
	msg.cmd = CMD_INT_JOB_DONE_INFO;
	memset(msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stIC_JobDoneInfo stJobDoneInfo = {0};
	memcpy(&stJobDoneInfo, pJobDoneInfo, sizeof(stIC_JobDoneInfo));
	stJobDoneInfo.copies = jobResult;
	stJobDoneInfo.jobDocType = jobType;
	memcpy(msg.data, &stJobDoneInfo, sizeof(stIC_JobDoneInfo));
	JobDoneInfoService(&msg);
}

void PrintGoPrintResultService(SYS_UI_MESSAGE *Message)
{	
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stPrint_GoPrintResult printResult;
	bool msgSendFlag = true;
	JOB_TYPES nextJobType = ejob_NULL;

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_PRINT_GOPRINT_RES;	//Go Print result
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	memcpy(&printResult, Message->data, sizeof(stPrint_GoPrintResult));	

	if(Message->msgSender == SJMID)
	{
		switch(printResult.result)
		{
			case esjm_JobReceived:
				msgSendFlag = false;
				++g_nPrintJobCount;
				if(g_nPrintJobCount > 0){
					gSystemPrintJobStatus = true;
					TimeoutControl(TIMEOUT_PAUSE);
				}
				if(karasUprintSettingFlag == true)
					GoJobLog(NULL);
				DBG_SSL_PRINTF_SDH_ERR("JCPrint:%d\n", g_nPrintJobCount);
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalPrintJob=%d, Status = esjm_JobReceived, PrintJobStatus=%d  \n", __FUNCTION__, g_nPrintJobCount, gSystemPrintJobStatus);
				break;
			case esjm_JobStart:
				printResult.result = RSLT_JOB_ACCEPTED;
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalPrintJob=%d, JobID=%d, Status = esjm_JobStartn", __FUNCTION__, g_nPrintJobCount, printResult.jobID);
				break;
			case esjm_JobDone:
				printResult.result = RSLT_JOB_COMPLETED;	//Job done or completed
				nextJobType = printResult.page;
				printResult.page = 0;
				--g_nPrintJobCount;
				if(g_nPrintJobCount <= 0){
					gSystemPrintJobStatus = false;
					TimeoutControl(TIMEOUT_RESET);
				}
				DBG_SSL_PRINTF_SDH_ERR("-JCPrint:%d\n", g_nPrintJobCount);
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SJM TotalPrintJob=%d, JobID=%d, Status = esjm_JobDone  \n", __FUNCTION__, g_nPrintJobCount,  printResult.jobID);
				NotifySystemJobDone();
				break;
			case esjm_JobCancel_at_device:
			case esjm_JobCancel_by_user:
				printResult.result = RSLT_JOB_CANCELING;
				if(gPrintCancelReason == STATUS_WRONG_IMAGE_SIZE_CANCEL_REQ){
					printResult.sizeCode = MEDIASIZE_A3;
				}
				break;
			case esjm_JobCancelDone:
				printResult.result = RSLT_JOB_CANCELED;
				if(gPrintCancelReason == STATUS_WRONG_IMAGE_SIZE_CANCEL_REQ){
					printResult.sizeCode = MEDIASIZE_A3;
					gPrintCancelReason = 0; //Reset Cancel reason
				}
				break;
			default:
				msgSendFlag = false;
				break;
		}
	}
	else if(Message->msgSender ==JOBMGRID)
	{		
		switch(printResult.result)
		{
			case STATUS_JOBINFO_START_JOB:
				DBG_SSL_PRINTF_SDH_ERR("[%s]: ID=%d STATUS_JOBINFO_START_JOB  \n", __FUNCTION__, printResult.jobID);
				//gSystemPrintJobStatus = true;
				//printResult.result = 0;
				//g_nPritingJobStatus = STATUS_JOBINFO_START_JOB;
				//TimeoutControl(TIMEOUT_PAUSE);
				msgSendFlag = false;
				break;
				
			case STATUS_PDL_MEMORYOUT:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: ID=%d STATUS_PDL_MEMORYOUT\n", __FUNCTION__, printResult.jobID);
				msgSendFlag = false;
				break;	
				
			case STATUS_INFO_CANCELING:				
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: ID=%d STATUS_INFO_CANCELING\n", __FUNCTION__, printResult.jobID);
				//printResult.result = RSLT_JOB_CANCELING;
				msgSendFlag = false;
				break;
				
			case STATUS_ONLINE:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: ID=%d STATUS_ONLINE\n", __FUNCTION__, printResult.jobID);
				msgSendFlag = false;
				break;
				
			case STATUS_JOBINFO_CANCEL:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: ID=%d STATUS_JOBINFO_CANCEL\n", __FUNCTION__, printResult.jobID);
				//printResult.result = RSLT_JOB_CANCELED;
				msgSendFlag = false;
				break;
				
			case STATUS_JOBINFO_END_JOB:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: ID=%d STATUS_JOBINFO_END_JOB\n", __FUNCTION__, printResult.jobID);
				//gSystemPrintJobStatus = false;
				//printResult.result = 5;	//Job completed
				//g_nPritingJobStatus = STATUS_JOBINFO_END_JOB;
				//TimeoutControl(TIMEOUT_RESET);
				msgSendFlag = false;
				break;
				
			default:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X Error: Please check status value\n", __FUNCTION__, printResult.result);
				msgSendFlag = false;
				break;
		}	

		//SHM: Write Device status to Shared Memory so other process(like web server) can use it.
		if(g_pSHM_PrinterStatus){
			memcpy(g_pSHM_PrinterStatus, &printResult.result, sizeof(uint32_t));
		}

	}
	else if(Message->msgSender == SYSTEM_SERVICE_ID)
	{
		// Printing, Print Resumed, Paper JAM, Paper Empty, Full Stack, Paper Mismatch
		//Do nothing send same structure
	}
	else {
		msgSendFlag = false;
	}
	
	if(msgSendFlag) {		
		if((gPrintSystemState == RSLT_JOB_CANCELING) && (printResult.result == RSLT_JOB_RESUMED)){
			DBG_SSL_PRINTF_SDH_ERR("  [ %s] Msg Old:%d Ignore New:%d \n", __FUNCTION__, gPrintSystemState, printResult.result);
			//JOB_CANCELING is in progress so Ignore JOB_RESUMED message.
		}
		else{
			if(gPrintSystemState != printResult.result){
				gPrintSystemState = printResult.result;
				memcpy(msgToSend.data, &printResult, sizeof(stPrint_GoPrintResult));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );

				if ( IS_TFTLCD_ATTACHED ){
					if((gPrintSystemState == RSLT_WAIT_USERINPUT) || (gPrintSystemState == RSLT_STOPED_USER)
						|| (gPrintSystemState == RSLT_STOPED_JAM) || (gPrintSystemState == RSLT_STOPED_PAPEREMPTY)
						|| (gPrintSystemState == RSLT_STOPED_FULLSTACK) || (gPrintSystemState == RSLT_STOPED_MISMATCH)
						|| (gPrintSystemState == RSLT_STOPED_SC) || (gPrintSystemState == RSLT_STOPED_FRONTDOOROPEN)
						|| (gPrintSystemState == RSLT_STOPED_DRUM) || (gPrintSystemState == RSLT_WAIT_USERINPUT_MERGE)){
						
						if(g_nTFTBackLightOn == false){
							SetTFTPanelMode(TFT_FB_WAKEUP);
						}
					}
				}

				if(gPrintSystemState == RSLT_STOPED_PAPEREMPTY){
					ErrorNotifyToEmailAlertRequest(EMAIL_ALERT_PAPEREMPTY, printResult.inTray);
				}
			}
			else if((printResult.result == RSLT_JOB_ACCEPTED) //|| (printResult.result == RSLT_JOB_CANCELED)
				|| (printResult.result == RSLT_JOB_COMPLETED)){
				gPrintSystemState = printResult.result;
				memcpy(msgToSend.data, &printResult, sizeof(stPrint_GoPrintResult));
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );
			}
			//One Print job has completed and SJM has Print job pending then Send JOB RESUMED msg.
			if((gPrintSystemState == RSLT_JOB_COMPLETED) && (gSystemPrintJobStatus)){
				if(nextJobType > 1000){
					nextJobType -= 1000;
					DBG_SSL_PRINTF_SDH_ERR("[%s] To send RESUME message check NextJobType=%d=%s\n", __FUNCTION__, nextJobType, to_string_job_types(nextJobType));
					if(ISONLYPRINTJOB(nextJobType)){	//If next job is Print job then send RESUME message
						printResult.result = RSLT_JOB_RESUMED;
						printResult.jobID = g_nCurrentPrintJobID;
						gPrintSystemState = printResult.result;
						memcpy(msgToSend.data, &printResult, sizeof(stPrint_GoPrintResult));
						err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
						XASSERT( err==OK, err );
					}
				}
				else{
					DBG_SSL_PRINTF_SDH_ERR("[%s] Resource not obtained for NextJobType=%d=%s\n", __FUNCTION__, nextJobType, to_string_job_types(nextJobType));
					//Resource is not obtained for this job so no need to send RESUMED message.
				}
			}
		}
	}
}

void NetworkResponseService(SYS_UI_MESSAGE *Message){	

	enum{ENUM_NIF_GET, ENUM_NIF_SET, ENUM_NIF_EXECUTE};
	enum{ENUM_NIF_WIRED, ENUM_NIF_WIFI, ENUM_NIF_UAP};
	enum{ENUM_NIF_IPV4, ENUM_NIF_IPV6, ENUM_NIF_MAC, ENUM_NIF_SSID, ENUM_NIF_SLIST,ENUM_NIF_CLIST};
	enum{ENUM_NIF_DHCP, ENUM_NIF_STATIC};
	enum{ENUM_NIF_NONE, ENUM_NIF_NOSEC, ENUM_NIF_WEP, ENUM_NIF_WPA};

	error_type_t err;
	stNetwork_Request stNetwork;
	SYS_UI_MESSAGE msgToSend;
	stNetwork_Response networkResult;
	bool msgSendFlag = true;

	memcpy(&stNetwork, Message->data, sizeof(stNetwork_Request));		//copy data to stNetwork

	//Init msgSender
	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_NETWORK_RESPONSE;	//Network Response
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
	networkResult.setMode = stNetwork.setMode;
	networkResult.netConnet = stNetwork.netConnet;
	networkResult.netType = stNetwork.netType;
	networkResult.setDhcp = stNetwork.setDhcp;
	networkResult.passwdType = stNetwork.passwdType;
	//DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);
	//DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   Type: %d   Conn: %d\n", __FUNCTION__, stNetwork.type, stNetwork.netConnet);

	if(stNetwork.setMode == ENUM_NIF_GET){	//get request
		if(stNetwork.netConnet == ENUM_NIF_WIRED){	//netConnet
			if(stNetwork.netType == ENUM_NIF_IPV4){	//IPv4
				networkResult.netResult = NUI_getIPAddrV4_wired();
			}
			else if(stNetwork.netType == ENUM_NIF_IPV6){	//IPv6
				networkResult.netResult = NUI_getIPAddrV6_wired();
			}
			else if(stNetwork.netType == ENUM_NIF_MAC){	//MAC
				networkResult.netResult = NUI_getMacAddr_wired();
			}
		}
		else if(stNetwork.netConnet == ENUM_NIF_WIFI){
			if(stNetwork.netType == ENUM_NIF_IPV4){	//IPv4
				networkResult.netResult = NUI_getIPAddrV4_wifi();
			}
			else if(stNetwork.netType == ENUM_NIF_IPV6){	//IPv6
				networkResult.netResult = NUI_getIPAddrV6_wifi();
			}
			else if(stNetwork.netType == ENUM_NIF_MAC){	//MAC
				networkResult.netResult = NUI_getMacAddr_wifi();
			}
			else if(stNetwork.netType == ENUM_NIF_SLIST){
				networkResult.netResult = NUI_getSSIDList_wifi();
			}
		}
		else if(stNetwork.netConnet == ENUM_NIF_UAP){
		}
	}
	else if(stNetwork.setMode == ENUM_NIF_SET){			//set request
		if(stNetwork.netConnet == ENUM_NIF_WIRED){			//wired
			if(stNetwork.netType == ENUM_NIF_IPV4){		//IPv4
				if(stNetwork.setDhcp == ENUM_NIF_DHCP){	//setDhcp
					DBG_SSL_PRINTF_SDH_ERR("Set Wired Network IP - DHCP \n");
					networkResult.netResult = NUI_setDHCPDynamic_wired();
				}
				if(stNetwork.setDhcp == ENUM_NIF_STATIC){	//static
					DBG_SSL_PRINTF_SDH_ERR("Set Wired Network IP - Static \n");
					networkResult.netResult = NUI_setDHCPStatic_wired();
				}
			}
		}
		if(stNetwork.netConnet == ENUM_NIF_WIFI){			//wired
			if(stNetwork.netType == ENUM_NIF_IPV4){		//IPv4
				if(stNetwork.setDhcp == ENUM_NIF_DHCP){	//setDhcp
					DBG_SSL_PRINTF_SDH_ERR("Set WiFi Network IP - DHCP \n");
					networkResult.netResult = NUI_setDHCPDynamic_wifi();
				}
				if(stNetwork.setDhcp == ENUM_NIF_STATIC){	//static
					DBG_SSL_PRINTF_SDH_ERR("Set WiFi Network - Static IP \n");
					networkResult.netResult = NUI_setDHCPStatic_wifi();
				}
			}
			else if(stNetwork.netType == ENUM_NIF_SSID){
				networkResult.netResult = NUI_ConnectSSID_wifi();
			}

		}
	}
	else if(stNetwork.setMode == ENUM_NIF_EXECUTE){	//execute request
		if(stNetwork.netConnet == ENUM_NIF_WIFI){			//WIFI
			if(stNetwork.netType == ENUM_NIF_CLIST){		//IPv4
				networkResult.netResult = NUI_ClearSSID_wifi();
			}
		
			if(stNetwork.setDhcp == ENUM_NIF_DHCP){		//OFF
				networkResult.netResult = NUI_ConnectOff_wifi();
			}
			else if(stNetwork.setDhcp == ENUM_NIF_STATIC){	//ON
				networkResult.netResult = NUI_ConnectOn_wifi();
			}
		}
		if(stNetwork.netConnet == ENUM_NIF_UAP){			//UAP
			if(stNetwork.setDhcp == ENUM_NIF_DHCP){		//OFF
			}
			else if(stNetwork.setDhcp == ENUM_NIF_STATIC){	//ON
			}
		}
	}
	if(msgSendFlag)
	{
		memcpy(msgToSend.data, &networkResult, sizeof(stNetwork_Response));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

void SPRequestService (SYS_UI_MESSAGE *Message)
{
	error_type_t err;	
	stSp_Request stSPRequest = {0};
	uint32_t msgData = 0;
	bool msgSendFlag_to_lsu = false;
	bool msgSendFlag_to_scanman = false;
		
	memcpy(&stSPRequest, Message->data, sizeof(stSp_Request));
	
	switch (stSPRequest.mode){
		case SP_MODE_TYPE_READ:		//read			
			DBG_SSL_PRINTF_SDH_ERR("SP Request  READ Error: Please check NO value = 0x%X  \n", stSPRequest.no);
			break;
		case SP_MODE_TYPE_WRITE:		//write
			DBG_SSL_PRINTF_SDH_ERR("SP Request  WRITE Error: Please check NO value = 0x%X  \n", stSPRequest.no);
			// TO DO sometinhg
			break;
		case SP_MODE_TYPE_EXECUTE:		//execute
			switch (stSPRequest.no){
				case SP_MODE_INPUT_SENSOR:
					DBG_SSL_PRINTF_SDH_ERR("SP_MODE_PLOTTER_SENSOR_Test\n");
					if(stSPRequest.size == 0){	//Test Start
						msgData = Sensor_Test_Start;
					}else if(stSPRequest.size == 1){	//Test End
						msgData = hardware_test_off;
					}
					msgSendFlag_to_lsu = true;
					break;
				case SP_MODE_SYS_RECOVERY:	//System Recovery
					DBG_SSL_PRINTF_SDH_ERR("System Recovery Start...\n");
					system("/app/karas_pinetree_util.sh --system-recovery");
					break;
				case SP_MODE_FIRMWARE_UPDATE:	//Firmware Update
					DBG_SSL_PRINTF_SDH_ERR("Network Firmware Update Start...\n");
					system("/app/karas_pinetree_util.sh --net-update");
					break;
				case SP_MODE_SOLENOID:
					DBG_SSL_PRINTF_SDH_ERR("SP_MODE_SOLENOID\n");
					msgData = Solenoid_Test_Start;
					msgSendFlag_to_lsu = true;
					break;
				case SP_MODE_TRAY1_MOTOR:
					DBG_SSL_PRINTF_SDH_ERR("SP_MODE_TRAY1_MOTOR\n");
					msgData = Motor_Test_Start_T1;
					msgSendFlag_to_lsu = true;
					break;
				case SP_MODE_TRAY2_MOTOR:
					DBG_SSL_PRINTF_SDH_ERR("SP_MODE_TRAY2_MOTOR\n");
					msgData = Motor_Test_Start_T2;
					msgSendFlag_to_lsu = true;					
					break;
				case SP_MODE_TRAY3_MOTOR:
					DBG_SSL_PRINTF_SDH_ERR("SP_MODE_TRAY3_MOTOR\n");
					msgData = Motor_Test_Start_T3;
					msgSendFlag_to_lsu = true;					
					break;
				case SP_MODE_SCANNER_FB_FREE_RUN:
				case SP_MODE_SCANNER_ADF_FREE_RUN:
				case SP_MODE_SCANNER_FEED_MOTOR:
				case SP_MODE_SCANNER_TRANSPORT_MOTOR:
				case SP_MODE_SCANNER_CHECK_PRESENT_SENSOR:
				case SP_MODE_SCANNER_CHECK_INTERVAL_SENSOR:
				case SP_MODE_SCANNER_CHECK_FIRSTSCAN_SENSOR:
				case SP_MODE_SCANNER_CHECK_TOPCOVER_OPEN:
				case SP_MODE_SCANNER_CHECK_HOME_SENSOR:
					DBG_SSL_PRINTF_SDH_ERR("SP_MODE SP_MODE_SCANNER = %d\n", stSPRequest.no);
					msgSendFlag_to_scanman = true;					
					break;
				case SP_MODE_SD_CARD_FORMAT:
				{
					DBG_SSL_PRINTF_SDH_ERR("Receive SP_MODE_SD_CARD_FORMAT\n");

					int retcode = system ( "/app/sd_format.sh" );
					UINT32 result;
					/* retcode 0: end, 1: ng(not inserted) */
					if ( WEXITSTATUS ( retcode ) == 0 ) {
						result = SP_MODE_RESULT_END; 
					}
					else {
						result = SP_MODE_RESULT_NG;
					}

					SYS_UI_MESSAGE msgToSend;
					stSp_Reponse stSPResponse = {0};
					msgToSend.msgSender = SYSTEM_SERVICE_ID;
					msgToSend.cmd = CMD_SU_SP_RESPONSE;
					memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

					stSPResponse.mode = SP_MODE_TYPE_EXECUTE;
					stSPResponse.no = SP_MODE_SD_CARD_FORMAT;
					stSPResponse.result = result;
					stSPResponse.size = 1;
					//FUTURE USE
					/*memset(&stSPResponse.buffer, 0, 12);
					memcpy(&stSPResponse.buffer, buffer, stSPResponse.size);*/

					memcpy(msgToSend.data, &stSPResponse, sizeof(stSp_Reponse));
					err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
					XASSERT( err==OK, err );
					break;
				}
				default:
					DBG_SSL_PRINTF_SDH_ERR("SP Request  EXECUTE Error: Please check NO value = 0x%X  \n", stSPRequest.no);
					msgSendFlag_to_lsu = false;
					msgSendFlag_to_scanman = false;
					break;
			}
			break;
		default:
			DBG_SSL_PRINTF_SDH_ERR("SP Request  Error: Please check MODE value = 0x%X  \n", stSPRequest.mode);
			msgSendFlag_to_lsu = false;
			msgSendFlag_to_scanman = false;
			break;
	}

	if(msgSendFlag_to_lsu){
		DBG_SSL_PRINTF_SDH_ERR("Send Msg to lsu_ctrl_mq\n");
		posix_message_send(lsu_ctrl_mq, (char*)&msgData, sizeof(uint32_t), 0, 0);
	}
	
	if(msgSendFlag_to_scanman){
		DBG_SSL_PRINTF_SDH_ERR("Send Msg to SCANMANID\n");
		MESSAGE sendMsg;
		memset(&sendMsg, 0x00, sizeof(MESSAGE));

		if(stSPRequest.size == 0) {
			/* Scanner Test Start */
			sendMsg.msgType = MSG_SCANNER_SP_MODE_START_MSG;
		}
		else {
			/* Scanner Test Stop */
			sendMsg.msgType = MSG_SCANNER_SP_MODE_STOP_MSG;
		}
		sendMsg.param1 = stSPRequest.no;
		if(stSPRequest.no == SP_MODE_SCANNER_FEED_MOTOR || stSPRequest.no == SP_MODE_SCANNER_TRANSPORT_MOTOR) {
			sendMsg.param2 = atoi((const char*)stSPRequest.buffer);
		}
		
		err = SYMsgSend(SCANMANID, &sendMsg);
	}
}

void SendSPModeDone()
{
	DBG_SSL_PRINTF_SDH_ERR("[%s]\n", __FUNCTION__);
	//Call All harware off
	uint32_t msgData = 0;
	msgData = hardware_test_off;
	posix_message_send(lsu_ctrl_mq, (char*)&msgData, sizeof(uint32_t), 0, 0);
}

void SPResponseSensorTest(unsigned char* buffer)
{
	DBG_SSL_PRINTF_SDH_ERR("[%s]: %d%d%d\n",__FUNCTION__, buffer[0], buffer[1], buffer[2]);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stSp_Reponse stSPResponse = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_SP_RESPONSE;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stSPResponse.mode = SP_MODE_TYPE_EXECUTE;
	stSPResponse.no = SP_MODE_INPUT_SENSOR;
	stSPResponse.result = 2;
	stSPResponse.size = 3;
	memset(&stSPResponse.buffer, 0, 12);
	memcpy(&stSPResponse.buffer, buffer, stSPResponse.size);
	
	memcpy(msgToSend.data, &stSPResponse, sizeof(stSp_Reponse));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void SPResponseScannerSensorTest(unsigned int test_mode, unsigned char sensor_result)
{
	DBG_SSL_PRINTF_SDH_ERR("[%s]: mode:%d, result:%d\n",__FUNCTION__, test_mode, sensor_result);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stSp_Reponse stSPResponse = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_SP_RESPONSE;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stSPResponse.mode = SP_MODE_TYPE_EXECUTE;
	stSPResponse.no = test_mode;
	stSPResponse.result = 2;
	stSPResponse.size = 1;
	memset(&stSPResponse.buffer, 0, 12);
	stSPResponse.buffer[0] = sensor_result;
	
	memcpy(msgToSend.data, &stSPResponse, sizeof(stSp_Reponse));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void SPResponseSolenoidTest()
{
	DBG_SSL_PRINTF_SDH_ERR("[%s]\n", __FUNCTION__);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stSp_Reponse stSPResponse = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_SP_RESPONSE;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stSPResponse.mode = SP_MODE_TYPE_EXECUTE;
	stSPResponse.no = SP_MODE_SOLENOID;
	stSPResponse.result = 2;
	stSPResponse.size = 0;

	memcpy(msgToSend.data, &stSPResponse, sizeof(stSp_Reponse));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
}


void SPResponseMotorTest(int trayNo)
{
	DBG_SSL_PRINTF_SDH_ERR("[%s]: %d\n", __FUNCTION__, trayNo);
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	stSp_Reponse stSPResponse = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = CMD_SU_SP_RESPONSE;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	stSPResponse.mode = SP_MODE_TYPE_EXECUTE;
	switch(trayNo){
		case 1:
			stSPResponse.no = SP_MODE_TRAY1_MOTOR;
			break;
		case 2:
			stSPResponse.no = SP_MODE_TRAY2_MOTOR;
			break;
		case 3:
			stSPResponse.no = SP_MODE_TRAY3_MOTOR;
			break;
		default:
			break;
	}
	
	stSPResponse.result = 2;
	stSPResponse.size = 0;
	memcpy(msgToSend.data, &stSPResponse, sizeof(stSp_Reponse));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void PrintSubSystemService(SYS_UI_MESSAGE *Message)
{	
	uint32_t status;		
	uint32_t deviceStatusPM;
	uint32_t deviceStatusJM;
	JOB_TYPES printJobType = 0;
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	bool msgSendFlag = true;
	stPrint_GoPrintResult stPrintResult = {0};
	stPrint_GoLaminationResult stLaminationResult = { 0 };
	stCopy_GoCopyResult stCopyResult = {0};
	stPrint_SendFaxStatus stFaxPrintResult = {0};
	stIC_PrintSubSystem stPrinterStatus = {0};

	msgToSend.msgSender = SYSTEM_SERVICE_ID; 
	msgToSend.cmd = 0;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	memcpy(&stPrinterStatus, Message->data, sizeof(stIC_PrintSubSystem));	
	status = stPrinterStatus.printStatus;
	deviceStatusPM = stPrinterStatus.pageStatus;
	deviceStatusJM = stPrinterStatus.jobStatus;
	printJobType = stPrinterStatus.resourceJobType;
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status=0x%X, PM=0x%X, JM=0x%X, JobType=%s, JobID=%d\n", __FUNCTION__,
				status, deviceStatusPM, deviceStatusJM, to_string_job_types(printJobType), stPrinterStatus.resourceJobID);

	switch(printJobType){
		case ejob_Copy:
			//g_nCurrentCopyJobID = stPrinterStatus.resourceJobID;
			stCopyResult.jobID = stPrinterStatus.resourceJobID;
			break;
		case ejob_FaxPrint:
			///TODO: Fax has different job id.
			break;
		case ejob_PrintIO:
		case ejob_InternalPagePrint:
		case ejob_InternalPageHttp:
		case ejob_PJL:
		case ejob_ACL:
		case ejob_zjs_host_print:
		case ejob_PrintFile:
			g_nCurrentPrintJobID = stPrinterStatus.resourceJobID;
			stPrintResult.jobID = g_nCurrentPrintJobID;
			break;
		default:
			break;
	}

	if(Message->msgSender == PRINTMGRID){
		//if(status == STATUS_ONLINE){
		if(deviceStatusPM == STATUS_ONLINE){

			if(gSystemPaperEmptyStatus || gSystemPaperMismatchStatus || gFrontDoorStatus || gSystemFullStack ||gSystemJamStatus || gSystemSCStatus){
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					stCopyResult.result = RSLT_JOB_RECOVERED;
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					stFaxPrintResult.statusType = FAX_PRINT_RECOVERED;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if(gSystemPrintJobStatus){
					stPrintResult.result = RSLT_JOB_RECOVERED;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
			}
			
			if(gFrontDoorStatus){				
				if(deviceStatusPM != STATUS_DOOROPEN_FRONT){
					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: STATUS_DOOROPEN_FRONT - Closed \n", __FUNCTION__);
					msgToSend.cmd = CMD_INT_PLOTTER_DOOR_STATUS;
					memcpy(msgToSend.data, &deviceStatusPM, sizeof(uint32_t));
					PlotterDoorOpenService(&msgToSend);
				}
			}

			if(gSystemFullStack){
				if(deviceStatusPM != STATUS_PAPER_JAM_FULLSTACK){
					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: STATUS_PAPER_JAM_FULLSTACK - Cleared \n", __FUNCTION__);
					gSystemFullStack = false;
					TimeoutControl(TIMEOUT_RESET);
				}
			}

			if(gSystemJamStatus){
				if((deviceStatusPM & STATUS_PAPERJAM) != STATUS_PAPERJAM){
					
					msgToSend.cmd = CMD_SU_PLOTTER_JAM_STATUS;
					stScnPltr_PlotterJamStatus stJamStatus;
					stJamStatus.jamReason = PLT_JAM_CLEAR;
					stJamStatus.jamStatus = 0;

					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: PAPER JAM - Cleared\n", __FUNCTION__);
					memcpy(msgToSend.data, &stJamStatus, sizeof(stScnPltr_PlotterJamStatus));
					err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
					XASSERT( err==OK, err );
					
					gSystemJamStatus = false;
					gPlotterJamCode = 0;	//Reset JAM Code
					TimeoutControl(TIMEOUT_RESET);
				}
			}
			
			if(gSystemSCStatus){
				if((deviceStatusPM & (COMMON_FATAL|STATCAT_PRINTHEAD)) != (COMMON_FATAL|STATCAT_PRINTHEAD)){
					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: SC - Cleared \n", __FUNCTION__);
					gSystemSCStatus = false;
					TimeoutControl(TIMEOUT_RESET);
				}
			}
			
			if(gSystemPaperMismatchStatus){
				gSystemPaperMismatchStatus = false;
			}

			if(gSystemPaperEmptyStatus){
				gSystemPaperEmptyStatus = false;
			}
#if 0
			if((gSystemPrintJobStatus == false) && (gCopyJobStatus == false) && (gFaxJobStatus == false) && (gConfUpdatePending == true)){
				msgToSend.msgSender = WEB_MONITOR_ID; 
				msgToSend.cmd = CMD_US_CONFIG_CHANGED;
				SPChangedService(&msgToSend);
			}
#else
			if((gSystemPrintJobStatus == false) && (gCopyJobStatus == false) && (gFaxPrintJobStatus == false)){
				NotifySystemJobDone();
			}
#endif
		
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_ONLINE \n", __FUNCTION__);			
		}
		else if(ISOFFLINE(status)){
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_STOPPED \n", __FUNCTION__);
			if(deviceStatusPM == STATUS_DOOROPEN_FRONT){
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_STOPPED : STATUS_DOOROPEN_FRONT \n", __FUNCTION__);
				TimeoutControl(TIMEOUT_PAUSE);				
				msgSendFlag = false;
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					stCopyResult.result = RSLT_STOPED_FRONTDOOROPEN;
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					stFaxPrintResult.statusType = FAX_PRINT_STOPED_FRONTDOOR_OPEN;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if(gSystemPrintJobStatus){
					stPrintResult.result = RSLT_STOPED_FRONTDOOROPEN;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
				 else if (gLaminationJobStatus) {
					stLaminationResult.result = LMN_RSLT_STOPED_FRONTDOOROPEN;
					msgToSend.cmd = CMD_SU_PRINT_GOLAMINATION_RES;
					memcpy(msgToSend.data, &stLaminationResult, sizeof(stPrint_GoLaminationResult));
					GoLaminationResultService(&msgToSend);
				}
			}
			else if(deviceStatusPM == STATUS_DOORCLOSE_FRONT){
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_STOPPED : STATUS_DOORCLOSE_FRONT \n", __FUNCTION__);
				msgSendFlag = false;
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					stCopyResult.result = RSLT_JOB_RECOVERED;
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					stFaxPrintResult.statusType = FAX_PRINT_RECOVERED;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if(gSystemPrintJobStatus){
					stPrintResult.result = RSLT_JOB_RECOVERED;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
			}
			else if(deviceStatusPM == STATUS_PAPER_JAM_FULLSTACK){
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_STOPPED : STATUS_PAPER_JAM_FULLSTACK \n", __FUNCTION__);
				msgSendFlag = false;
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					stCopyResult.result = RSLT_STOPED_FULLSTACK;
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					stFaxPrintResult.statusType = FAX_PRINT_STOPED_FULL_STACK;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if(gSystemPrintJobStatus){
					stPrintResult.result = RSLT_STOPED_FULLSTACK;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
				else if (gLaminationJobStatus) {
					stLaminationResult.result = LMN_RSLT_STOPED_FULLSTACK;
					msgToSend.cmd = CMD_SU_PRINT_GOLAMINATION_RES;
					memcpy(msgToSend.data, &stLaminationResult, sizeof(stPrint_GoLaminationResult));
					GoLaminationResultService(&msgToSend);
				}
				gSystemFullStack = true;
				TimeoutControl(TIMEOUT_PAUSE);
			}
			else if(deviceStatusPM == STATUS_PAPER_JAM_REINSERTCARTRIDGE){
				msgSendFlag = false;
				TimeoutControl(TIMEOUT_PAUSE);
			}
			else if( deviceStatusPM ==STATUS_PAPER_JAM_TONER_BLOCK ){
				msgSendFlag = false;
				//Do nothing
			}
			else if((deviceStatusPM & STATUS_PAPERJAM) == STATUS_PAPERJAM)	{		
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_STOPPED : STATUS_PAPERJAM \n", __FUNCTION__);
				msgSendFlag = false;
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					stCopyResult.result = RSLT_STOPED_JAM;	//Paper Jam
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					stFaxPrintResult.statusType = FAX_PRINT_STOPED_PAPER_JAM;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if(gSystemPrintJobStatus){
					stPrintResult.result = RSLT_STOPED_JAM;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
				else if (gLaminationJobStatus) {
					stLaminationResult.result = LMN_RSLT_STOPED_JAM;
					msgToSend.cmd = CMD_SU_PRINT_GOLAMINATION_RES;
					memcpy(msgToSend.data, &stLaminationResult, sizeof(stPrint_GoLaminationResult));
					GoLaminationResultService(&msgToSend);
				}
				gSystemJamStatus = true;
				TimeoutControl(TIMEOUT_PAUSE);
			}
			else if( (deviceStatusPM & STATUS_PAPER_OUT) == STATUS_PAPER_OUT){
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_STOPPED : STATUS_PAPER_OUT \n", __FUNCTION__);
				msgSendFlag = false;
				if(ISEMPTYFINAL(deviceStatusPM) && (deviceStatusJM != STATUS_INFO_CANCELING)){
					
					gSystemPaperEmptyStatus = true;
						
					uint32_t trayNo = 999;
					uint32_t sizeCode = MEDIASIZE_BAD;

					switch(deviceStatusPM)	{		
						case STATUS_PAPER_OUT_TRAY1_FINAL:
							trayNo = PLT_TRAY_1;
							sizeCode = gStFeedTray1Status.sizeCode;
							break;
						case STATUS_PAPER_OUT_TRAY2_FINAL:
							trayNo = PLT_TRAY_2;
							sizeCode = gStFeedTray2Status.sizeCode;
							break;
						case STATUS_PAPER_OUT_TRAY3_FINAL:
							trayNo = PLT_TRAY_3;
							sizeCode = gStFeedTray3Status.sizeCode;
							break;		
						case STATUS_PAPER_OUT_TRAY4_FINAL:	// MPT
							trayNo = PLT_TRAY_MPT;
							sizeCode = gStFeedTray0Status.sizeCode;
							break;
						default:
							break;
					}

					DBG_SSL_PRINTF_SDH_DEBUG(" %s : STATUS_PAPER_OUT InTray=%d, sizeCode=%d \n", __FUNCTION__, trayNo, sizeCode);
					if((printJobType == ejob_Copy) && gCopyJobStatus){
						stCopyResult.result = RSLT_STOPED_PAPEREMPTY;	//Paper Empty/out
						stCopyResult.inTray = trayNo;
						stCopyResult.sizeCode = sizeCode;
						msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
						memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
						CopyGoCopyResultService(&msgToSend);
					}
					else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
						stFaxPrintResult.statusType = FAX_PRINT_STOPED_PAPER_EMPTY;
						stFaxPrintResult.inTray = trayNo;
						stFaxPrintResult.sizeCode = sizeCode;
						msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
						memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
						FaxSJMResultService(&msgToSend);
					}
					else if(gSystemPrintJobStatus){
						stPrintResult.result = RSLT_STOPED_PAPEREMPTY;
						stPrintResult.inTray = trayNo;
						stPrintResult.sizeCode = sizeCode;
						msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
						memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
						PrintGoPrintResultService(&msgToSend);
					}
				}					
			}
#if 0
			else if( (deviceStatusPM & STATUS_INPUT_TRAY_SIZE_MISMATCH) == STATUS_INPUT_TRAY_SIZE_MISMATCH){
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = STATUS_INPUT_TRAY_SIZE_MISMATCH\n", __FUNCTION__);
				msgSendFlag = false;
				gSystemPaperMismatchStatus = true;
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					stCopyResult.inTray = get_mismatch_tray();
					stCopyResult.sizeCode = get_mismatch_size();
					stCopyResult.result = RSLT_STOPED_MISMATCH;
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					stFaxPrintResult.inTray = get_mismatch_tray();
					stFaxPrintResult.sizeCode = get_mismatch_size();
					stFaxPrintResult.statusType = FAX_PRINT_STOPED_PAPER_MISMATCH;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if(gSystemPrintJobStatus){
					//For Pinetree-S Print Job: Send printing/resumed message first then send paper mismatch message
					//Job resumed msg
					stPrintResult.result = RSLT_JOB_RESUMED;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);

					//Job Stopped due to Paper mismatch msg
					stPrintResult.inTray = get_mismatch_tray();
					stPrintResult.sizeCode = get_mismatch_size();
					stPrintResult.result = RSLT_STOPED_MISMATCH;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
			}
#endif
			else if((deviceStatusPM & (COMMON_FATAL|STATCAT_PRINTHEAD)) == (COMMON_FATAL|STATCAT_PRINTHEAD)){
				DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_STOPPED : Error SC  \n", __FUNCTION__);
				gSystemSCStatus = true;
				TimeoutControl(TIMEOUT_PAUSE);
			}
#if 0
			else if(deviceStatusPM == STATUS_DRUM_LOW_MODE_3) {
				//DRUM Empty
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					stCopyResult.result = RSLT_STOPED_DRUM;
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					stFaxPrintResult.statusType = FAX_PRINT_STOPED_DRUM_END;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if(gSystemPrintJobStatus){
					stPrintResult.result = RSLT_STOPED_DRUM;
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
				msgSendFlag = false;
			}
#endif
			else{
				//-1: No Use.  Abnormal: Find out proper reason.
				msgSendFlag = false;
			}

			if(msgSendFlag){
				err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
				XASSERT( err==OK, err );
			}
			
		}
		else if( (status & (STATUS_SEVERITY_MASK | STATUS_CATEGORY_MASK)) == STATUS_INFO ){
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = 0x%X => PRINTER_STATE_PROCESSING, PrintJobStatus=%d \n", __FUNCTION__, status, gSystemPrintJobStatus);
			TimeoutControl(TIMEOUT_PAUSE);
			if(status == STATUS_INFO_PRINTING){
				DBG_SSL_PRINTF_SDH_ERR("Printing:%s ID:%d\n", to_string_job_types(printJobType), stPrinterStatus.resourceJobID);
				if((printJobType == ejob_Copy) && gCopyJobStatus){
					//If paper mismatched and job is recovered
					if(gSystemPaperMismatchStatus){
						gSystemPaperMismatchStatus = false;
						stCopyResult.result = RSLT_JOB_RECOVERED;
						msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
						memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
						CopyGoCopyResultService(&msgToSend);
					}
					DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = Copy print Job Running or Resumed (automatically recovered) \n", __FUNCTION__);
					stCopyResult.result = RSLT_JOB_RESUMED;	//Copy Job Resumed (automatically recovered)
					msgToSend.cmd = CMD_INT_COPY_GOCOPY_RES;
					memcpy(msgToSend.data, &stCopyResult, sizeof(stCopy_GoCopyResult));
					CopyGoCopyResultService(&msgToSend);
				}
				else if((printJobType == ejob_FaxPrint) && gFaxPrintJobStatus){
					//If paper mismatched and job is recovered
					if(gSystemPaperMismatchStatus){
						gSystemPaperMismatchStatus = false;
						stFaxPrintResult.statusType = FAX_PRINT_RECOVERED;
						msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
						memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
						FaxSJMResultService(&msgToSend);
					}
					stFaxPrintResult.statusType = FAX_PRINT_RESUMED;
					msgToSend.cmd = CMD_INT_FAX_PRINT_JOB_RES;
					memcpy(msgToSend.data, &stFaxPrintResult, sizeof(stPrint_SendFaxStatus));
					FaxSJMResultService(&msgToSend);
				}
				else if((gSystemPrintJobStatus) && (!gCopyJobStatus)){
					//If paper mismatched and job is recovered
					if(gSystemPaperMismatchStatus){
						gSystemPaperMismatchStatus = false;
						stPrintResult.result = RSLT_JOB_RECOVERED;
						msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
						memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
						PrintGoPrintResultService(&msgToSend);
					}
					DBG_SSL_PRINTF_SDH_DEBUG("[%s] %s Print Job Resumed(Printing) \n", __FUNCTION__, to_string_job_types(printJobType));
					stPrintResult.result = RSLT_JOB_RESUMED; //Print Job Resumed (automatically recovered)
					msgToSend.cmd = CMD_INT_PRINT_GOPRINT_RES;
					memcpy(msgToSend.data, &stPrintResult, sizeof(stPrint_GoPrintResult));
					PrintGoPrintResultService(&msgToSend);
				}
			}
		}
		else	{
			DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Status = PRINTER_STATE_IDLE \n", __FUNCTION__);
			
			if(deviceStatusJM == STATUS_JOBINFO_START_JOB){
				//Print job already started.
			}
			msgSendFlag = false;
		}		
	}
	else	{
		msgSendFlag = false;

	}
	
	UNUSED_VAR(msgSendFlag);

	if ( IS_SCANNER_ATTACHED) {		
		ScannerPlotterModeStatusService(0, status);
	}	
	SetPrintSystemStatus(status, deviceStatusPM);
}	

void SetPrintSystemStatus(uint32_t filteredStatus, uint32_t pmStatus)
{
	if( (pmStatus == STATUS_DRUM_END) ||(gPCUDrumStatus == DRUM_END_LOCK) ){
		gPrintSystemStatus = STATUS_DRUM_END;
	}
	else if( (pmStatus == STATUS_DRUM_LOW_MODE_3) ||(pmStatus == STATUS_DRUM_LOW_MODE_2) ||(pmStatus == STATUS_DRUM_LOW_MODE_1) ||
		(gPCUDrumStatus == DRUM_CHANGE_LED_FOREVER) || (gPCUDrumStatus == DRUM_CHANGE_LED_ONE_MIN) ||(gPCUDrumStatus == DRUM_CHANGE)){
		if( (gPrintSystemState == RSLT_STOPED_DRUM) && (gPCUDrumStatus == DRUM_CHANGE_LED_FOREVER)){
			gPrintSystemStatus = STATUS_DRUM_END;
		}
		else	if( map_print_status_to_pjl(filteredStatus) == 10001){
			gPrintSystemStatus = STATUS_DRUM_LOW_MODE_3;
		}
		else{
			gPrintSystemStatus = filteredStatus;
		}
	}
	else if( (pmStatus == STATUS_TONER_END) || (pmStatus == STATUS_PAPER_JAM_TONER_BLOCK) ||(gTonerStatus == TONNER_STATUS_END) ){
		if( map_print_status_to_pjl(filteredStatus) == 10001){
			gPrintSystemStatus = STATUS_TONER_END;
		}
		else{
			gPrintSystemStatus = filteredStatus;
		}
	}
	else if( (pmStatus == STATUS_TONER_LOW) ||(gTonerStatus == TONNER_STATUS_LOW) ){
		if( map_print_status_to_pjl(filteredStatus) == 10001){
			gPrintSystemStatus = STATUS_TONER_LOW;
		}
		else{
			gPrintSystemStatus = filteredStatus;
		}
	}
	else{
		gPrintSystemStatus = filteredStatus;
	}
	DBG_SSL_PRINTF_SDH_ERR("PSS: 0x%X\n", gPrintSystemStatus);
}

uint32_t GetPrintSystemStatus()
{
	return gPrintSystemStatus;
}

uint32_t GetFaxPJLStatusCode(void)
{
	return gFaxSystemStatus;
}

void SetFaxSystemStatus(uint32_t faxAppStatus )
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s] FaxAppStatus %d\n", __FUNCTION__, faxAppStatus);
	uint32_t curFaxStatus = 0;
	bool shmWriteFlag = true;
	switch(faxAppStatus){
		case FAX_SENDING_COMPLETE:
		//case FAX_RECEIVING_COMPLETE:
		case FAX_PRINTING_COMPLETE:
		case FAX_LINE_IS_ON:
		case FAX_SENDING_FAIL:
		case FAX_RECEIVING_FAIL:
		case FAX_NONCOMPLETE_FAIL:
		case FAX_JOB_FAILED_MERGE:
		case FAX_SCAN_FAIL:	
		case FAX_CANCEL_COMPLETE:
			curFaxStatus = 80001;	//ONLINE
			break;		
		case FAX_NO_LINE:
			curFaxStatus = 80002;	//OFFLINE
			break;
		case FAX_SCANNING_FB:
		case FAX_SCANNING_ADF:
		case FAX_SCANNING_COMPLETE:
		case FAX_DIALING:
		//case FAX_DIAL_NO:
		//case FAX_CONNECTING:
		//case FAX_CONNECTED_SPEED:
		case FAX_SENDING:
			curFaxStatus = 80014;	//Sending
			break;
		case FAX_RINGING:
		case FAX_RECEIVING:
		case FAX_PRINTING:
			curFaxStatus = 80015;	//Receiving
			break;
		case FAX_WAIT_USERINPUT_MERGE:
			curFaxStatus = 80029;	//Wait for user input
			break;
		default:
			shmWriteFlag = false;
			break;
	}

	if(shmWriteFlag){
		if(curFaxStatus != gFaxSystemStatus){
			gFaxSystemStatus = curFaxStatus;
			//SHM: Write Fax job status to Shared Memory so other process(like web server) can use it.
			if(g_pSHM_FaxStatus){		
				memcpy(g_pSHM_FaxStatus, &curFaxStatus, sizeof(uint32_t));
			}
		}
	}	
}

void FaxTxNotifyJobDone(UINT32 jobID, UINT8 statusType, UINT8 pages)
{
	int jobResult = -1;

	switch(statusType){
		case FAX_SCANNING_COMPLETE:
			if(isDKSolution){
				jobResult = AUTH_SOL_JOB_OK;
			}
			break;
		case FAX_SENDING_COMPLETE:
			jobResult = AUTH_SOL_JOB_OK;
			break;
		case FAX_SENDING_FAIL:
			jobResult = AUTH_SOL_JOB_NG;
			break;
		case FAX_NONCOMPLETE_FAIL:
			jobResult = AUTH_SOL_JOB_PENDING;
			break;
		case FAX_FAX_TX_DELETE_OK:
			jobResult = AUTH_SOL_JOB_DEL_OK;
			break;
		case FAX_FAX_TX_DELETE_FAIL:
			jobResult = AUTH_SOL_JOB_DEL_NG;
			break;
		case FAX_FAX_TX_FILENAME_MISMATCH:
			jobResult = AUTH_SOL_JOB_FILENAME_MISMATCH;
			break;
		case FAX_FAX_TX_CANCELED_BY_USER:
			jobResult = AUTH_SOL_JOB_CANCELLED_BY_USER;
			break;
		default:
			break;
	}

	if(g_nILFaxDestCount > 0){
		jobResult+=g_nILFaxLastDestFilter;
	}

	if(jobResult >= 0){
		memset(&g_stFaxJobDoneInfo, 0x00, sizeof(stIC_JobDoneInfo));
		memcpy(&g_stFaxJobDoneInfo, &g_faxScanInfo, sizeof(g_stFaxJobDoneInfo));
		g_stFaxJobDoneInfo.jobNum = g_nILFaxUniqueJobID;
		g_stFaxJobDoneInfo.totalPages = pages;
		SendJobDoneInfo(SYSTEM_SERVICE_ID, &g_stFaxJobDoneInfo, AUTH_JOB_FAX_SEND, jobResult);
	}
	//Reset destination count and uniqueJobID
	//g_nILFaxDestCount = 0;
	//g_nILFaxUniqueJobID = 0;
}

void FaxOnHookService(void)
{
	if(faxapp_check_on_hook_button_pressed()){	//If Already Pressed then now On-Hook will finish
		gFaxOnHookBtnPressed = 0;
	}
	else{	//On Hook will start
		gFaxOnHookBtnPressed = 1;
	}
	DBG_SSL_PRINTF_SDH_ERR("OnHookButtonPressed=%d\n", gFaxOnHookBtnPressed);
	if(gFaxOnHookBtnPressed){
		TimeoutControl(TIMEOUT_PAUSE);
	}
	else{
		TimeoutControl(TIMEOUT_RESET);
	}
}

void FaxSendCounterService(uint32_t faxSendCount)
{
	FAXSEND_COUNTER_STRUCT stFaxSendCounter = {0};
	error_type_t db_retval;

	if(faxSendCount){
		db_retval = db_get_faxsendcount(&stFaxSendCounter);
		if(db_retval == FAIL){
			DBG_SSL_PRINTF_SDH_ERR("%s: Fail to get DB data: FaxSendCounter\n", __FUNCTION__);
		}
		else{
			stFaxSendCounter.faxsend += faxSendCount;
			db_retval = db_set_faxsendcount(stFaxSendCounter);
			if(db_retval == FAIL){
				DBG_SSL_PRINTF_SDH_ERR("%s: Fail to set DB data: FaxSendCounter\n", __FUNCTION__);
			}
		}
	}
	DBG_SSL_PRINTF_SDH_ERR("FaxSend Counter %d:%d\n", faxSendCount, stFaxSendCounter.faxsend);
}

void FaxPrintJobDoneNotify(UINT32 pagePrinted, UINT32 jobDocType, UINT32 senderID, UINT32 jobResult)
{
	DBG_SSL_PRINTF_SDH_ERR("[%s] Pages:%d, JobType=%d, S:%d, R:%d\n", __FUNCTION__, pagePrinted, jobDocType, senderID, jobResult);
	if(gAppCallbacksList[APPCB_FAX_PRINT_FINISHED]){
		//Call FAX callback funtion
		(*gAppCallbacksList[APPCB_FAX_PRINT_FINISHED])(jobResult, pagePrinted);
	}
}

void FaxFailedError(UINT8 err_no)
{
	SYS_UI_MESSAGE msgToSend;
	error_type_t err;
	DBG_SSL_PRINTF_SDH_ERR("FAX failed due to : %d\n", err_no);

	stPrint_SendFaxStatus stSendFaxStatus = {0};
	stSendFaxStatus.statusType = FAX_SCAN_FAIL;
	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	msgToSend.cmd = CMD_SU_FAX_SEND_FAX_STATUS;
	memcpy(msgToSend.data, &stSendFaxStatus, sizeof(stPrint_SendFaxStatus));
	err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
	XASSERT( err==OK, err );
}

void FaxIncrementJobCount(void)
{
	++g_nFaxJobCount;
	if(g_nFaxJobCount > 0){
		gFaxJobStatus = true;
		TimeoutControl(TIMEOUT_PAUSE);
	}
	DBG_SSL_PRINTF_SDH_ERR("JCFax:%d\n", g_nFaxJobCount);
}

void FaxDecrementJobCount(void)
{
	if ( g_nFaxJobCount > 0 ){
		--g_nFaxJobCount;
	}

	if(g_nFaxJobCount <= 0){
		gFaxJobStatus = false;
		TimeoutControl(TIMEOUT_RESET);
	}
	DBG_SSL_PRINTF_SDH_ERR("-JCFax:%d\n", g_nFaxJobCount);
}

void FaxSJMResultService(SYS_UI_MESSAGE *Message)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]:   \n", __FUNCTION__);
	
	stIC_FaxJobResult stFaxJobResult = {0};
	stPrint_SendFaxStatus stSendFaxStatus = {0};

	if(Message->msgSender == SJMID){
		memcpy(&stFaxJobResult, Message->data, sizeof(stIC_FaxJobResult));	
		
		switch(stFaxJobResult.result){
			case esjm_JobReceived:
				DBG_SSL_PRINTF_SDH_DEBUG("[%s] QueuedFaxJob=%d, Status = esjm_JobReceived, FaxJobStatus=%d  \n", __FUNCTION__, g_nFaxJobCount, gFaxJobStatus);
				break;
			case esjm_JobStart:
				FaxIncrementJobCount();
				if(stFaxJobResult.jobType == ejob_FaxPrint){
					++g_nFaxPrintJobCount;
					if(g_nFaxPrintJobCount > 0){
						gFaxPrintJobStatus = true;
					}
					gFaxPrintSystemState = FAX_PRINT_RESUMED;	//To stop sending FAX print resume message
				}
				DBG_SSL_PRINTF_SDH_DEBUG("[%s] TotalFaxJob=%d JobID=%d %s esjm_JobStart\n", __FUNCTION__, g_nFaxJobCount, stFaxJobResult.jobID, to_string_job_types(stFaxJobResult.jobType));
				break;
			case esjm_JobDone:
				FaxDecrementJobCount();
				if(stFaxJobResult.jobType == ejob_FaxPrint){
					--g_nFaxPrintJobCount;
					if(g_nFaxPrintJobCount <= 0){
						gFaxPrintJobStatus = false;
					}
#if 1
					if(gFaxPrintJobCanceled){
						FaxPrintJobDoneNotify(0, gFaxPrintJobDocType, SJMID, 0);
					}
					else{
						FaxPrintJobDoneNotify(0, gFaxPrintJobDocType, SJMID, 1);
					}
#else
					gFaxPrintAppPagePrinted = pagesFaxAppPagePrinted;
					DBG_SSL_PRINTF_SDH_ERR("ejob_FaxPrint Pages:%d, JobType=%d\n", gFaxPrintAppPagePrinted, gFaxPrintJobDocType);
					if(gAppCallbacksList[APPCB_FAX_PRINT_FINISHED]){
						if(gFaxPrintJobDocType == e_FAX_LONGPAGE){
							gFaxPrintAppPagePrinted /= 2;
						}
						
						if(gFaxPrintJobCanceled){
							(*gAppCallbacksList[APPCB_FAX_PRINT_FINISHED])(0, gFaxPrintAppPagePrinted);
						}
						else{
							(*gAppCallbacksList[APPCB_FAX_PRINT_FINISHED])(1, gFaxPrintAppPagePrinted);
						}
					}
#endif
					//Reset Total page printed by Fax Print app
					pagesFaxAppPagePrinted = 0;
					gFaxPrintJobCanceled = false;
					gFaxPrintSystemState = FAX_PRINTING_COMPLETE;
				}
				DBG_SSL_PRINTF_SDH_DEBUG("[%s] TotalFaxJob=%d JobID=%d %s esjm_JobDone\n", __FUNCTION__, g_nFaxJobCount, stFaxJobResult.jobID, to_string_job_types(stFaxJobResult.jobType));
				NotifySystemJobDone();
				break;
			case esjm_JobCancel_at_device:
				break;
			case esjm_JobCancel_by_user:
				break;
			case esjm_JobCancelDone:
				DBG_SSL_PRINTF_SDH_ERR("[%s] TotalFaxJob=%d JobID=%d %s esjm_JobCancelDone\n", __FUNCTION__, g_nFaxJobCount, stFaxJobResult.jobID, to_string_job_types(stFaxJobResult.jobType));
				if(stFaxJobResult.jobType == ejob_FaxPrint){
					gFaxPrintJobCanceled = true;
				}
				break;
			default:
				break;
		}
	}
	else if(Message->msgSender == FAXAPPID){
		memcpy(&stSendFaxStatus, Message->data, sizeof(stPrint_SendFaxStatus));
		//Send for Adaptive energy save mode and Wake-up if sleeping
		if(stSendFaxStatus.statusType == FAX_RINGING){
			SendWakeUpMsgToSSM(SJMID, EMA_FAX_RX);
		}
		if(stSendFaxStatus.statusType == FAX_DIALING){
			SendWakeUpMsgToSSM(SJMID, EMA_FAX_TX);
		}
		
		switch(stSendFaxStatus.statusType){
			case FAX_SCANNING_FB:
			case FAX_SCANNING_ADF:
				if(g_nFaxScanJobCount == 0){
					FaxIncrementJobCount();
					++g_nFaxScanJobCount;
				}
				break;
			case FAX_SCANNING_COMPLETE:
			case FAX_SCAN_FAIL:
				if(isDKSolution){
					//Notify Solution Fax Tx job done for count-up
					FaxTxNotifyJobDone(stSendFaxStatus.jobID, stSendFaxStatus.statusType, stSendFaxStatus.totalPageNum);
				}
				if(g_nFaxScanJobCount > 0){
					FaxDecrementJobCount();
					--g_nFaxScanJobCount;
				}
				break;
			case FAX_DIALING:
				if(g_bFaxDialingFlag == false){
					g_bFaxDialingFlag = true;
					FaxIncrementJobCount();
				}
				break;
			case FAX_SENDING:
				if(g_nFaxSendJobCount == 0){
					++g_nFaxSendJobCount;
				}
				break;
			case FAX_SENDING_COMPLETE:
			case FAX_SENDING_FAIL:
			case FAX_NONCOMPLETE_FAIL:
				g_bFaxDialingFlag = false;
				g_bFaxRingingFlag = false;
				FaxDecrementJobCount();
				//Notify Solution Fax Tx job done for count-up
				FaxTxNotifyJobDone(stSendFaxStatus.jobID, stSendFaxStatus.statusType, stSendFaxStatus.totalPageNum);
				if(g_nFaxSendJobCount > 0){
					--g_nFaxSendJobCount;
					pagesFaxSendCounter = stSendFaxStatus.totalPageNum;
					if(pagesFaxSendCounter > 0){
						FaxSendCounterService(pagesFaxSendCounter);
						pagesFaxSendCounter = 0; //Reset
					}
				}
				break;
			case FAX_RINGING:
				if(g_bFaxRingingFlag == false){
					g_bFaxRingingFlag = true;
					FaxIncrementJobCount();
				}
				break;
			case FAX_RECEIVING:
				if(g_nFaxReceiveJobCount == 0){
					++g_nFaxReceiveJobCount;
				}
				break;
			case FAX_RECEIVING_COMPLETE:
			case FAX_RECEIVING_FAIL:
				g_bFaxDialingFlag = false;
				g_bFaxRingingFlag = false;
				FaxDecrementJobCount();
				if(g_nFaxReceiveJobCount > 0){
					--g_nFaxReceiveJobCount;
				}
				break;
			case FAX_CANCEL_COMPLETE: //Cancellation can be for FAX Send and FAX Receive. In Both case FAX_CANCEL_COMPLETE message receiving
				//CANCEL_COMPLETE msg also received when user cancelled SCAN job
				//If FAX scanning, sending or receiving job CANCELLED then FAX will send CANCEL_COMPLETE msg
				if(g_bFaxDialingFlag || g_bFaxRingingFlag || g_nFaxScanJobCount){
					if(g_nFaxScanJobCount > 0){
						--g_nFaxScanJobCount;
					}
					g_bFaxDialingFlag = false;
					g_bFaxRingingFlag = false;
					FaxDecrementJobCount();
				}

				if(g_nFaxReceiveJobCount > 0){
					--g_nFaxReceiveJobCount;
				}
				if(g_nFaxSendJobCount > 0){
					--g_nFaxSendJobCount;
					pagesFaxSendCounter = stSendFaxStatus.totalPageNum;
					if(pagesFaxSendCounter > 0){
						FaxSendCounterService(pagesFaxSendCounter);
						pagesFaxSendCounter = 0; //Reset
					}
				}
				break;
			case FAX_PRINTING:
				g_nFaxPrintCurJobID = stSendFaxStatus.jobID;
				break;
			case FAX_PRINTING_COMPLETE:
				g_nFaxPrintCurJobID = 0;	//Reset
				break;
			case FAX_FAX_TX_DELETE_OK:
			case FAX_FAX_TX_DELETE_FAIL:
			case FAX_FAX_TX_FILENAME_MISMATCH:
			case FAX_FAX_TX_CANCELED_BY_USER:
				//Notify Solution Fax Tx job done for count-up
				FaxTxNotifyJobDone(stSendFaxStatus.jobID, stSendFaxStatus.statusType, 0);
				break;
			default:
				break;
		}
		DBG_SSL_PRINTF_SDH_ERR("QueuedFaxJob=%d Scan=%d Send=%d Receive=%d Print=%d Status=%d JobID=%d FaxJobStatus=%d\n",
			g_nFaxJobCount, g_nFaxScanJobCount, g_nFaxSendJobCount, g_nFaxReceiveJobCount, g_nFaxPrintJobCount, stSendFaxStatus.statusType, stSendFaxStatus.jobID, gFaxJobStatus);

		SetFaxSystemStatus(stSendFaxStatus.statusType);
	}
	else if(Message->msgSender == SYSTEM_SERVICE_ID){
		error_type_t err;
		SYS_UI_MESSAGE msgToSend;
		msgToSend.msgSender = SYSTEM_SERVICE_ID;
		msgToSend.cmd = CMD_SU_FAX_SEND_FAX_STATUS;
		memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
		memcpy(&stSendFaxStatus, Message->data, sizeof(stPrint_SendFaxStatus));
		stSendFaxStatus.jobID = g_nFaxPrintCurJobID;

		if(gFaxPrintSystemState != stSendFaxStatus.statusType){
			gFaxPrintSystemState = stSendFaxStatus.statusType;
			memcpy(msgToSend.data, &stSendFaxStatus, sizeof(stPrint_SendFaxStatus));
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
			XASSERT( err==OK, err );
		}
	}
}


void FaxGoService(SYS_UI_MESSAGE *nextMsg)
{
	int ret = 0;
	error_type_t err;

	switch (nextMsg->cmd)
	{
		case CMD_INT_FAX_GO_FAX_SCAN: //MSG_GO_FAX_SCAN
		{
			if(g_nEnergySaveMode){
				DBG_SSL_PRINTF_SDH_ERR("[%s]: Error: Do not allow UI GoFAX job in Power Save mode\n", __FUNCTION__);
				DBG_SSL_PRINTF_SDH_ERR("[%s]: Error: Ignoring UI GoFAX job request\n", __FUNCTION__);
				//Send message to UI for FAX Job failed
				FaxFailedError(1);
				return;
			}

			stFax_GoFaxScan *ptrGoFaxScan;
			stShmPhoneNumber *ptrPhoneNumber;
			int i = 0;
			ptrGoFaxScan = (stFax_GoFaxScan *)nextMsg->data;

			DBG_SSL_PRINTF_SDH_DEBUG("bal: txType: %d\n", ptrGoFaxScan->TxJobType);
			DBG_SSL_PRINTF_SDH_DEBUG("bal: ptrPhoneNumber: %d\n", ptrGoFaxScan->NoOfRx);
			if ( ptrGoFaxScan->addInfo ) {

				ptrPhoneNumber = shm_attach(ptrGoFaxScan->addInfo);
				if ( ptrPhoneNumber != (void *)-1 ) {
					for ( i = 0 ; i < ptrGoFaxScan->NoOfRx ; i ++ ) 
					{
						DBG_SSL_PRINTF_SDH_DEBUG("no[%d]: %s\n", i, ptrPhoneNumber->no[i]);
					}

					ret = shm_detach((void *)ptrPhoneNumber);
					if ( ret < 0 ) {
						DBG_SSL_PRINTF_SDH_DEBUG("shm_detach error: %s line%d\n", __func__, __LINE__);
					}
//					shm_remove(ptrGoFaxScan->addInfo);
				}
				err = SysUI_MsgSend( FAXAPPID, nextMsg );
				XASSERT( err==OK, err );
			}
			else if(ptrGoFaxScan->TxJobType == 3)
			{
				err = SysUI_MsgSend( FAXAPPID, nextMsg );
				XASSERT( err==OK, err );
			}
			break;
		}

		case CMD_INT_FAX_GO_ON_HOOK: // MSG_GO_ON_HOOK
		case CMD_INT_FAX_GO_UPDATE_PARAM: // MSG_GO_UPDATE_PARAM
		case CMD_INT_FAX_GO_FAXSCAN_MODE: // MSG_GO_FAX_SCAN_MORE
		case CMD_INT_FAX_GO_FAX_CANCEL:
		case CMD_INT_FAX_GO_FAX_DELETE_PENDING_JOB:
		case CMD_INT_FAX_GO_FAX_PRINT_REPORT:
		case CMD_INT_FAX_GO_FAX_RESET_DB:
		case CMD_INT_FAX_GO_FAX_SP_MODE:
		case CMD_INT_FAX_GO_FAX_ACK_READ_DB:
		case CMD_INT_FAX_GO_FAX_DELETE_DOCMAN:
		case CMD_US_FAX_GO_SOLUTION_FAX:
		{
			//Make sure that the fax card is ready.
			if(isFaxCardReady)
			{
				if(nextMsg->cmd == CMD_INT_FAX_GO_ON_HOOK){
					FaxOnHookService();
				}

				err = SysUI_MsgSend( FAXAPPID, nextMsg );
				XASSERT( err==OK, err );
			}
			break;
		}

		case CMD_SU_FAX_SEND_FAX_STATUS: //MSG_FAX_STATUS
		case CMD_SU_FAX_SEND_FAX_MEMORY_STATUS:
		case CMD_SU_FAX_SEND_FAX_REQUEST_READ_DB:
		case CMD_SU_FAX_SEND_FAX_BROADCAST_JOBID:
		case CMD_SU_FAX_SEND_PCFAX_JOBID:
		{
			if(nextMsg->cmd == CMD_SU_FAX_SEND_FAX_STATUS){
				FaxSJMResultService(nextMsg);
			}
			err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, nextMsg);
			XASSERT( err==OK, err );
			break;
		}
		default:
		break;
	} // end switch(nextMsg.cmd)
}

//************* Tx Fucntion Send Result ********* */
void SendTxResultService(SYS_UI_MESSAGE *Message)
{
	error_type_t err;
	SYS_UI_MESSAGE msgToSend;
	bool msgSendFlag = true;
	stSend_SendResult stTxSendResult = {0};

	memcpy(&stTxSendResult, Message->data, sizeof(stSend_SendResult));
	DBG_SSL_PRINTF_SDH_ERR("[%s]: Result=%d App=%d SendType=%d\n", __FUNCTION__, stTxSendResult.result, stTxSendResult.reqAppType, stTxSendResult.sendJobType);

	msgToSend.msgSender = SYSTEM_SERVICE_ID;
	memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	switch(stTxSendResult.reqAppType){
		case SCANTOEMAILAPPID:
			stTxSendResult.reqAppType = SEND_REQ_APP_SCAN;
			break;
		case FAXAPPSOLUTIONID:
			stTxSendResult.reqAppType = SEND_REQ_APP_FAX;
			break;
		case SEND_REQ_APP_SCAN:
		case SEND_REQ_APP_FAX:
		case SEND_REQ_APP_CONNTEST_UI:
			//Send same msg
			break;
		case SEND_REQ_APP_LAST:
		case SEND_REQ_APP_NONE:
			msgSendFlag = false;
			break;
		default:
			msgSendFlag = false;
			break;
	}

	if(msgSendFlag){
		msgToSend.cmd = CMD_SU_SEND_RES;
		memcpy(msgToSend.data, &stTxSendResult, sizeof(stSend_SendResult));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}

//************* START: EMAIL -ALERT FUNCTIONS ********* */
int ErrorNotifyToEmailAlertRequest(int alertType, uint32_t errorCode)
{
	if(alertType == EMAIL_ALERT_ALIVE_SEND){
		if( GetAliveEmailAlertFlag() != 1){
			DBG_SSL_PRINTF_SDH_ERR("[%s]: AlertCounterEnable=OFF\n", __FUNCTION__);
			return -1;
		}
	}
	else{
		if( GetAutoEmailAlertFlag() != 1){
			DBG_SSL_PRINTF_SDH_ERR("[%s]: AutoEmailAlert=OFF\n", __FUNCTION__);
			return -1;
		}
	}

	bool isAlertPaperEmpty = false;
	bool isAlertTonerEnd = false;
	bool isAlertOutputFull = false;
	bool isAlertPaperJam = false;
	bool isAlertIntervention = false;
	bool isAlertDrumChange = false;
	bool isAlertTestMail = false;
	bool isAlertAliveSend = false;
	int sendResult = 0;
	error_type_t db_retval = FAIL;

	switch(alertType) {
		case EMAIL_ALERT_PAPEREMPTY:
			isAlertPaperEmpty = db_get_var_int("CONF_MAILALERTTYPE", "AlertPaperEmpty", &db_retval);
			if(db_retval == FAIL){
				isAlertPaperEmpty = 0;
			}
			break;
		case EMAIL_ALERT_TONERLOW:
		case EMAIL_ALERT_TONEREND:
			isAlertTonerEnd = db_get_var_int("CONF_MAILALERTTYPE", "AlertTonerEnd", &db_retval);
			if(db_retval == FAIL){
				isAlertTonerEnd = 0;
			}
			break;
		case EMAIL_ALERT_OUTPUTFULL:
			isAlertOutputFull = db_get_var_int("CONF_MAILALERTTYPE", "AlertOutputFull", &db_retval);
			if(db_retval == FAIL){
				isAlertOutputFull = 0;
			}
			break;
		case EMAIL_ALERT_PAPERJAM:
			isAlertPaperJam = db_get_var_int("CONF_MAILALERTTYPE", "AlertPaperJam", &db_retval);
			if(db_retval == FAIL){
				isAlertPaperJam = 0;
			}
			break;
		case EMAIL_ALERT_INTERVENTION:
			isAlertIntervention = db_get_var_int("CONF_MAILALERTTYPE", "AlertIntervention", &db_retval);
			if(db_retval == FAIL){
				isAlertIntervention = 0;
			}
			break;
		case EMAIL_ALERT_DRUMCHANGE:
			isAlertDrumChange = db_get_var_int("CONF_MAILALERTTYPE", "AlertDrumChange", &db_retval);
			if(db_retval == FAIL){
				isAlertDrumChange = 0;
			}
			break;
		case EMAIL_ALERT_TEST_SEND:
			isAlertTestMail = true;
			break;
		case EMAIL_ALERT_ALIVE_SEND:
			isAlertAliveSend = true;
			break;
		default:
			break;
	}

	if (isAlertPaperEmpty || isAlertTonerEnd || isAlertOutputFull
			|| isAlertPaperJam || isAlertIntervention || isAlertDrumChange
			|| isAlertTestMail || isAlertAliveSend) {
		sendResult = ErrorNotifyToEmailAlertSend(alertType, errorCode);
	}
	
	return sendResult;
}

int NotiMsgNotifyToEmailAlertRequest(int nofiMsgType)
{
	int isAlertDrumChange = 0;
	int sendResult = 0;
	error_type_t db_retval = FAIL;

	isAlertDrumChange = db_get_var_int("CONF_MAILALERTTYPE", "AlertDrumChange", &db_retval);
	if(db_retval == FAIL){
		isAlertDrumChange = 0;
	}
	
	switch(nofiMsgType) {
		case 0:	//NOTIFY_Engine_DrumChange:
			if( isAlertDrumChange == 1 ) {
				sendResult = ErrorNotifyToEmailAlertSend(EMAIL_ALERT_DRUMCHANGE, 0);
			}
			break;
		default:
			break;
	}
	return sendResult;
}

int GetAutoEmailAlertFlag(void)
{
	//Check Error Email Alert Function ON/OFF from DB.
	int isAlertEnable = 0;
	error_type_t db_retval = FAIL;

	isAlertEnable = db_get_var_int("CONF_MAILALERTTYPE", "AlertEnable", &db_retval);
	if(db_retval == FAIL){
		isAlertEnable = 0;
	}
	return isAlertEnable;
}

int GetAliveEmailAlertFlag(void)
{
	//Check Error Alive Email Alert Function ON/OFF from DB.
	int isAlertEnable = 0;
	error_type_t db_retval = FAIL;

	isAlertEnable = db_get_var_int("CONF_MAILALERTTYPE", "AlertCounterEnable", &db_retval);
	if(db_retval == FAIL){
		isAlertEnable = 0;
	}
	return isAlertEnable;
}

int ErrorNotifyToEmailAlertSend(int alertType, uint32_t errorCode)
{
	/***************************************************************
	* Get Setting Value of SMTP
	***************************************************************/
	char serverAddr[200]={0,};
	char senderID[200]={0,};
	char senderPW[200]={0,};
	char senderEmail[200]={0,};
	int serverPort = 0;
	int secureType = 1;
	int authType = 1;
	error_type_t db_retval = FAIL;
	char* tempValue;
	
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailServer", &db_retval);
	if(db_retval == OK) {
		if(tempValue != NULL){
			snprintf (serverAddr,  sizeof(serverAddr), "%s",tempValue);
			MEM_FREE_AND_NULL(tempValue);
		}
	}
	
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailID", &db_retval);
	if(db_retval == OK) {
		if(tempValue != NULL){
			snprintf (senderID,  sizeof(senderID), "%s",tempValue);
			MEM_FREE_AND_NULL(tempValue);
		}
	}

	tempValue= db_get_var("CONF_SYSTEM", "DefaultMailAddress", &db_retval);
	if(db_retval == OK) {
		if(tempValue != NULL){
			snprintf (senderEmail,  sizeof(senderID), "%s",tempValue);
			MEM_FREE_AND_NULL(tempValue);
		}
	}
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailPW", &db_retval);
	if(db_retval == OK) {
		if(tempValue != NULL){
			snprintf (senderPW,  sizeof(senderPW), "%s",tempValue);
			MEM_FREE_AND_NULL(tempValue);
		}
	}

	serverPort = db_get_var_int("CONF_SYSTEM", "DefaultMailPort", &db_retval);
	if(db_retval == FAIL){
		serverPort = 0;
	}
	db_retval = FAIL;
#if 0	
	secureType = db_get_var_int("CONF_SYSTEM", "DefaultMailSecureType", &db_retval);
	if(db_retval == FAIL){
		secureType = 0;
	}
	db_retval = FAIL;
	authType = db_get_var_int("CONF_SYSTEM", "DefaultMailAuthType", &db_retval);
	if(db_retval == FAIL){
		authType = 0;
	}
	db_retval = FAIL;
#endif

	/***************************************************************
	* Get Setting Value of Email Elert Receiver
	***************************************************************/
	char emailAddrList[MAX_ADMIN_EMAILALERT_NUM*200] = {0,};
	char *tmpEmailAddr = NULL;
	int adminIdx = 0;
	char *table = "RECD_ADMINMAILADDR";
	char query[QUERY_LENGTH];

	for(adminIdx=0; adminIdx<MAX_ADMIN_EMAILALERT_NUM; adminIdx++){
		memset(query, 0, sizeof(query));
		snprintf(query, QUERY_LENGTH, "SELECT EmailAddr FROM %s WHERE NO=%d", table, adminIdx+1);
		tmpEmailAddr = db_get_var_query(query, &db_retval);
		if(db_retval == OK) {
			if(tmpEmailAddr != NULL){
				if(adminIdx != 0){
					strcat(emailAddrList, ";");
				}
				strcat(emailAddrList, tmpEmailAddr);
				MEM_FREE_AND_NULL(tmpEmailAddr);
			}
		}
	}
	
	//DBG_SSL_PRINTF_SDH_ERR("[%s]:EmaiList:%s\n", __FUNCTION__, emailAddrList);

	/***************************************************************
	* Request Email Alert Send
	***************************************************************/
	int sendResult = 0;
	char emailTitleStr[200] = {0,};
	char emailBodyStr[1100] = {0,};
	char emailIPStr[200] = {0,};
	char emailSerialStr[200] = {0,};
	char emailTLIStr[200] = {0,};
	char emailLocationStr[200] = {0,};
	char* header = "SINDOH";

	//Set IP Address String.
	char *IpAddr = 0;
	NIF_getIPAddrV4_str_wired(&IpAddr);
	sprintf(emailIPStr, "Machine IP : ");
	strcat(emailIPStr, IpAddr);
	free(IpAddr);

	//Set SerialNum String
	db_retval = FAIL;	
	sprintf(emailSerialStr, "Machine Serial : ");
	tempValue = db_get_var(INSTANCE_MACHINE,"MachineSerialNo",&db_retval);
	if((tempValue == NULL) || (db_retval == FAIL)){
		strcat(emailSerialStr, "Unknown");
	}
	else{
		strcat(emailSerialStr, tempValue);
		MEM_FREE_AND_NULL(tempValue);
	}

	//Set TLI String
	db_retval = FAIL;
	sprintf(emailTLIStr, "Machine Name: ");
	tempValue = db_get_var(INSTANCE_MACHINE,"MachineTLI",&db_retval);
	if((tempValue == NULL) || (db_retval == FAIL)){
		strcat(emailTLIStr, "Unknown TLI");
	}
	else{
		if (strcmp(tempValue, "P247B01") == 0 ||
			strcmp(tempValue, "M247B01") == 0) {
			// If katusah,
			header = "KATUSHA";
		}
		strcat(emailTLIStr, tempValue);
		MEM_FREE_AND_NULL(tempValue);
	}
	
	//Set Location
	db_retval = FAIL;
	sprintf(emailLocationStr, "Location: ");
	tempValue = db_get_var(INSTANCE_MACHINE,"Location",&db_retval);
	if((tempValue == NULL) || (db_retval == FAIL)){
		strcat(emailLocationStr, "Unknown Location");
	}
	else{
		strcat(emailLocationStr, tempValue);
		MEM_FREE_AND_NULL(tempValue);
	}

	DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Error Code is : %d\n", __FUNCTION__, errorCode);
	switch(alertType){
		case EMAIL_ALERT_PAPEREMPTY:
			sprintf(emailTitleStr, "%s Machine Paper is empty", header);
			break;
		case EMAIL_ALERT_TONERLOW:
			sprintf(emailTitleStr, "%s Machine Toner is low", header);
			break;
		case EMAIL_ALERT_TONEREND:
			sprintf(emailTitleStr, "%s Machine Toner is empty. Replace Toner Cartridge", header);
			break;
		case EMAIL_ALERT_OUTPUTFULL:
			sprintf(emailTitleStr, "%s Machine Output Tray Full", header);
			break;
		case EMAIL_ALERT_PAPERJAM:
			sprintf(emailTitleStr, "%s Machine Jam Occurred", header);
			break;
		case EMAIL_ALERT_INTERVENTION:
			sprintf(emailTitleStr, "%s Machine Intervention Required", header);
			break;
		case EMAIL_ALERT_DRUMCHANGE:		 
			sprintf(emailTitleStr, "%s Machine Drum Cartridge level is empty. Replace Drum Cartridge", header);
			break;
		case EMAIL_ALERT_TEST_SEND:
			sprintf(emailTitleStr, "%s Machine Test Mail", header);
			break;
		case EMAIL_ALERT_ALIVE_SEND:
			sprintf(emailTitleStr, "%s Machine Counter Mail", header);
			break;
		default:
			break;
	}
	strcat(emailBodyStr, "\n");
	strcat(emailBodyStr, emailTitleStr);

	if(alertType == EMAIL_ALERT_INTERVENTION){
		char errorCodeStr[32] = {0,};
		//Set Title
		sprintf(errorCodeStr, "Error Code %d", errorCode);
		strcat(emailTitleStr, " - ");
		strcat(emailTitleStr, errorCodeStr);
		//Set Body
		memset(errorCodeStr, 0x00, 32);
		sprintf(errorCodeStr, "Error Code: %d", errorCode);
		strcat(emailBodyStr, "\n");
		strcat(emailBodyStr, errorCodeStr);
	}
	else if(alertType == EMAIL_ALERT_PAPERJAM){
		char errorCodeStr[32] = {0,};
		//Set Title
		sprintf(errorCodeStr, "Jam Code %d", errorCode);
		strcat(emailTitleStr, " - ");
		strcat(emailTitleStr, errorCodeStr);
		//Set Body
		memset(errorCodeStr, 0x00, 32);
		sprintf(errorCodeStr, "Jam Code: %d", errorCode);
		strcat(emailBodyStr, "\n");
		strcat(emailBodyStr, errorCodeStr);
	}

	char totalPrintPageStr[64] = {0,};
	int totalPrintPageCount = 0;
	//Set Location
	db_retval = FAIL;
	totalPrintPageCount = db_get_var_int("INST_MACHINECOUNT", "TotalPrint", &db_retval);
	if(db_retval == FAIL){
		totalPrintPageCount = 0;
	}
	snprintf(totalPrintPageStr, sizeof(totalPrintPageStr), "Total Print Page Count: %d", totalPrintPageCount);
	strcat(emailBodyStr, "\n");
	strcat(emailBodyStr, totalPrintPageStr);

	char tonerPageCountStr[64] = {0,};
	snprintf(tonerPageCountStr, sizeof(tonerPageCountStr), "Toner Page Count(5%% based): %d", gCurTonerPageCount);
	strcat(emailBodyStr, "\n");
	strcat(emailBodyStr, tonerPageCountStr);

	strcat(emailBodyStr, "\n\n");
	strcat(emailBodyStr, emailTLIStr);
	strcat(emailBodyStr, "\n");
	strcat(emailBodyStr, emailSerialStr);
	strcat(emailBodyStr, "\n");
	strcat(emailBodyStr, emailLocationStr);
	strcat(emailBodyStr, "\n");
	strcat(emailBodyStr, emailIPStr);

#if 0	//No Network ping test here
	//Ping TEST Before Alert To Email Server.
	if (NIF_getDHCPStatus_wired() == NIF_Status_Static){
		if( networkPingTestForEmailAlert(serverAddr) != NIF_Status_IfUp ){
			DBG_SSL_PRINTF_SDH_DEBUG("(UI)Wired Net Not Connected\n");
			return -1;
		}		
	}
#endif

	//DBG_SSL_PRINTF_SDH_ERR(" [%s]: \n emailBodyStr=\n%s\n", __FUNCTION__, emailBodyStr);
	email_info_message_t  mEmail;
	mEmail.username=calloc(1,strlen(senderID)+1);
	strcpy(mEmail.username,senderID);
	mEmail.password=calloc(1,strlen(senderPW)+1);
	strcpy(mEmail.password,senderPW);
	mEmail.senderName=calloc(1,strlen(header)+1);
	strcpy(mEmail.senderName,header);
	mEmail.senderEmail=calloc(1,strlen(senderEmail)+1);
	strcpy(mEmail.senderEmail,senderEmail);
	mEmail.mailhost=calloc(1,strlen(serverAddr)+1);
	strcpy(mEmail.mailhost,serverAddr);
	mEmail.port=serverPort;
	mEmail.attachmentPath="";
	mEmail.toAddress=calloc(1,strlen(emailAddrList)+1);
	strcpy(mEmail.toAddress,emailAddrList);
	mEmail.subject=calloc(1,strlen(emailTitleStr)+1);
	strcpy(mEmail.subject,emailTitleStr);
	mEmail.content=calloc(1,strlen(emailBodyStr)+1);
	strcpy(mEmail.content,emailBodyStr);
	mEmail.secureType=secureType;
	mEmail.authType=authType;
	mEmail.isDebugMode=0;
	mEmail.email_msg=NULL;
	
	sendResult= sendMail(&mEmail);

	if(sendResult == -1){
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Fail to send Email Alert!\n", __FUNCTION__);
	}
	else{
		DBG_SSL_PRINTF_SDH_ERR("[%s]: Email Alert Sent Successfully!\n",__FUNCTION__);
	}
	return sendResult;
}

//************* END: EMAIL -ALERT FUNCTIONS ********* */


void SysServiceObserverInit()
{
	// Job Status from System Job Manager 
	ObsSJMJobStatus = Observer_Constructor(SJM_JobStatus_ID, SS_SJMJobStatusCB); 
	SJM_AttachObserver_JobStatus(ObsSJMJobStatus);

	//Device status, PAGE_DATA and JOB_DATA from Print Job Manager.
	ObsPrintJobMgrDeviceStatus = Observer_Constructor(JobMgr_myDeviceStatus_ID, SS_PJMDeviceStatusCB); 
	JobMgr_AttachObserver_devicestatus(ObsPrintJobMgrDeviceStatus);
//	ObsPrintJobMgrPageEnd = Observer_Constructor(PRINT_JOBMGR_GLOBAL_PAGE_END_ID, SS_PJMPageEndCB);	
	//print_jobmgr_global_page_end_observer_attach( ObsPrintJobMgrPageEnd );

	ObsPrintMgrMediaStatus = Observer_Constructor(PrintMgr_myMediaStatus_ID, SS_PMMediaStatusCB);		
	PrintMgr_AttachObserver_MediaStatus( ObsPrintMgrMediaStatus );
	
	//Print Manager Device Status, Media Prompt and Print Status
	ObsPrintMgrDeviceStatus = Observer_Constructor(PrintMgr_myDeviceStatus_ID, SS_PMDeviceStatusCB);
	PrintMgr_AttachObserver_devicestatus(ObsPrintMgrDeviceStatus);
	ObsPrintMgrMediaPrompt = Observer_Constructor(PrintMgr_myMediaPromptID, SS_PMMediaPromptCB);
	printmgr_media_prompt_AttachObserver(ObsPrintMgrMediaPrompt);

	ObsPrintSubSystemStatus = Observer_Constructor(PrintJobMgr_PrintStatus_ID, SS_PrintSubSystemCB);
	pec_attachobserver_printstatus(ObsPrintSubSystemStatus);

#ifdef HAVE_COPY_SUPPORT	
	//CopyApp Event 
	ObsCopyAppEvent = Observer_Constructor(COPYAPP_OBSERVER_ID, SS_CopyAppEventCB);
	copyapp_observer_attach(ObsCopyAppEvent);
#endif

#ifdef HAVE_SCAN_SUPPORT
	//ScanApp Event
	ObsScanAppEvent = Observer_Constructor(SCANAPP_OBSERVER_ID, SS_ScanAppEventCB);
	scanapp_observer_attach(ObsScanAppEvent);
	//ScanMan Status
	ObsScanManEvent = Observer_Constructor(SCANMAN_OBSERVER_ID, SS_ScannerStatusEventCB);
	scanman_observer_attach(ObsScanManEvent);		
#endif

}

void SysServiceCheckSystemStatus(void)
{
	DBG_SSL_PRINTF_SDH_DEBUG(" [%s] : DBFailCode=%d\n", __FUNCTION__, gDBFailCode);
	//Check DB status
	if(gDBFailCode){
		SendSCStatusToUI(gDBFailCode, SYSTEM_MODULE_SCID);
	}
}

void SysServiceVariableInit(void)
{
	error_type_t err;
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);
	InitKarasUprintVariableSetting();
	
#ifdef SSL_MANAGE_JOBID_LIST
	ATInitList(&gJobIDList);
#endif

	int i = 0;
	for(i = 0; i < APPCB_CALLBACK_MAX; i++){
		gAppCallbacksList[i] = NULL;
	}
	
	//Remove RegisterAppCallback from here. RegisterAppCallback should be called by Application like FAX, etc.
	RegisterAppCallback(APPCB_FAX_PRINT_FINISHED, &FaxPrintJobResult);
	
	gStFeedTray0Status.tray = 0;
	gStFeedTray0Status.paperVolume = MEDIA_TRAY_PAPER_END;
	gStFeedTray0Status.sizeCode = MEDIASIZE_BAD;  
	gStFeedTray0Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;
	
	gStFeedTray1Status.tray = 1;
	gStFeedTray1Status.paperVolume = MEDIA_TRAY_PAPER_LOW;
	gStFeedTray1Status.sizeCode = MEDIASIZE_BAD;
	gStFeedTray1Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;

	gStFeedTray2Status.tray = 2;
	gStFeedTray2Status.paperVolume = MEDIA_TRAY_PAPER_LOW;
	gStFeedTray2Status.sizeCode = MEDIASIZE_BAD;
	gStFeedTray2Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;

	gStFeedTray3Status.tray = 3;
	gStFeedTray3Status.paperVolume = MEDIA_TRAY_PAPER_LOW;
	gStFeedTray3Status.sizeCode = MEDIASIZE_BAD;
	gStFeedTray3Status.trayStatus = MEDIA_TRAY_STATUS_CLOSE;

	//gAutoEmailAlert = GetAutoEmailAlertFlag();
	if(IS_TFTLCD_ATTACHED){
		g_nTFTBackLightOn = true;
	}
	//Set Energy Save Timeout value.
	UpdateEnergySaveTimeout();

	//update default tray 
	g_nDefaultTray = db_get_var_int("CONF_COPYSET", "PrintSrcTray", &err);
	if(err == FAIL || g_nDefaultTray < TRAY_MPT || g_nDefaultTray >= TRAY_MAX ){
		g_nDefaultTray = TRAY1;
	}

	g_nAliveAlertLastExecTime = db_get_var_int("CONF_MAILALERTTYPE", "AlertCounterSentTime", &err);
	if ( err == FAIL ) {
		g_nAliveAlertLastExecTime = 0;
	}

	ATInitList(&g_ScanJobList); //Init Scan job list
}

void SystemServiceInit(void)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s]\n", __FUNCTION__);

	SysServiceVariableInit();

	SysServiceReceiverInit();	 //Message receiver 
	//SysServiceSenderInit();		//Message sender ///On 11 July 2014 Moved to main.c (Sysinit.h) INITIALIZE_SYS_UI_SENDER_INIT

  	SysServiceObserverInit();	//Attach Observers 

//	SysServiceCheckSystemStatus();  // move to nvram_db_shim.c

	SysServiceDebugInit();

	//Timer Code for Test.
	SPTInitializeTimer();


	//Create Shared memory	
	g_pSHM_AuthInfo = (stShmAuthInfo *)shm_create(SHM_KEY_AUTH_INFO, sizeof(stShmAuthInfo));
	if ( g_pSHM_AuthInfo == (void *)-1 ) {
		g_pSHM_AuthInfo = NULL;
		DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_AUTH_INFO\n", SHM_KEY_AUTH_INFO);
	}
	else{
		//Init Auth SHM data
		memset(g_pSHM_AuthInfo->userID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		memset(g_pSHM_AuthInfo->userPassword, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		g_pSHM_AuthInfo->curLoginRegNo = 0;
		memset(g_pSHM_AuthInfo->emailAddress, 0x00, SDH_AUTH_CONF_EMAIL_ADDR);
		memset(g_pSHM_AuthInfo->ftpAddress, 0x00, SDH_AUTH_CONF_FTP_ADDR);
		g_pSHM_AuthInfo->ftpPort = 0;
		memset(g_pSHM_AuthInfo->ftpID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		memset(g_pSHM_AuthInfo->ftpPassword, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		g_pSHM_AuthInfo->prefer = 0;
		memset(g_pSHM_AuthInfo->printAuthFailID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
	}

	g_pSHM_SolAuthInfo = (stShmSolutionAuth *)shm_create(SHM_KEY_SOLUTION_AUTH, sizeof(stShmSolutionAuth));
	if ( g_pSHM_SolAuthInfo == (void *)-1 ) {
		g_pSHM_SolAuthInfo = NULL;
		DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_SOLUTION_AUTH\n", SHM_KEY_SOLUTION_AUTH);
	}
	else{
		//Init Solution Auth SHM data
		memset(g_pSHM_SolAuthInfo->userID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		memset(g_pSHM_SolAuthInfo->userPassword, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		memset(g_pSHM_SolAuthInfo->curLoginID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		memset(g_pSHM_SolAuthInfo->cardNumber, 0x00, SDH_SOLUTION_CARD_NO);
	}

	g_pSHM_SolJobRes = (stShmSolutionJobRes *)shm_create(SHM_KEY_SOLUTION_JOB_RES, sizeof(stShmSolutionJobRes));
	if ( g_pSHM_SolJobRes == (void *)-1 ) {
		g_pSHM_SolJobRes = NULL;
		DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_SOLUTION_JOB_RES\n", SHM_KEY_SOLUTION_JOB_RES);
	}
	else{
		//Init Solution Job result SHM data
		memset(g_pSHM_SolJobRes->userID, 0x00, SDH_AUTH_CONF_BASIC_MAX);
		memset(g_pSHM_SolJobRes->phoneNumber, 0x00, SDH_FAX_LEN_PHONE_NO+1);
		memset(g_pSHM_SolJobRes->fileName, 0x00, SDH_FILENAME_MAX+1);
	}

	g_pSHM_PrinterStatus = (uint32_t *)shm_create(SHM_KEY_PRINTER_STATUS, sizeof(uint32_t));
	if ( g_pSHM_PrinterStatus == (void *)-1 ) {
		g_pSHM_PrinterStatus = NULL;
		DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_PRINTER_STATUS\n", SHM_KEY_PRINTER_STATUS);
	}
	g_pSHM_TrayStatus = (uint32_t *)shm_create(SHM_KEY_TRAY_STATUS, sizeof(uint32_t));
	if ( g_pSHM_TrayStatus == (void *)-1 ) {
		g_pSHM_TrayStatus = NULL;
		DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_TRAY_STATUS\n", SHM_KEY_TRAY_STATUS);
	}	
	if(IS_SCANNER_ATTACHED){
		g_pSHM_ScanStatus = (uint32_t *)shm_create(SHM_KEY_SCANNER_STATUS, sizeof(uint32_t));
		if ( g_pSHM_ScanStatus == (void *)-1 ) {
			g_pSHM_ScanStatus = NULL;
			DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_SCANNER_STATUS\n", SHM_KEY_SCANNER_STATUS);
		}
	}
 	if(IS_FAX_ATTACHED){
		g_pSHM_FaxStatus = (uint32_t *)shm_create(SHM_KEY_FAX_STATUS, sizeof(uint32_t));
		if ( g_pSHM_FaxStatus == (void *)-1 ) {
			g_pSHM_FaxStatus = NULL;
			DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_FAX_STATUS\n", SHM_KEY_FAX_STATUS);
		}
	}

 	g_pSHM_FeedTrayStatus = (shm_FeedTrayStatus *)shm_create(SHM_KEY_FEED_TRAY_STATUS, sizeof(shm_FeedTrayStatus));
	if ( g_pSHM_FeedTrayStatus == (void *)-1 ) {
		g_pSHM_FeedTrayStatus = NULL;
		DBG_SSL_PRINTF_SDH_ERR("Error in shm_create SHMID=%d SHM_KEY_FEED_TRAY_STATUS\n", SHM_KEY_TRAY_STATUS);
	}
	else{
		int i = 0;
		for(i = 0; i < SDH_FEED_TRAY_MAX; i++){
			g_pSHM_FeedTrayStatus->tray[i].trayNo = i+1;
			g_pSHM_FeedTrayStatus->tray[i].paperVolume = MEDIA_TRAY_PAPER_LOW;
			g_pSHM_FeedTrayStatus->tray[i].sizeCode = MEDIASIZE_BAD;
			g_pSHM_FeedTrayStatus->tray[i].trayStatus = MEDIA_TRAY_STATUS_CLOSE;
		}
	}
}

void SysServiceReceiverInit(void)
{
	DBG_SSL_PRINTF_SDH_DEBUG(" [%s] called \n", __FUNCTION__);

#ifdef UIMGR_CREATED_MQ	 
	SysServiceRecv_queue = mq_open("/mq_ui_to_sys",O_RDWR );
	XASSERT( SysServiceRecv_queue != (mqd_t)-1, SysServiceRecv_queue );
#else
	//posix_create_MESSAGE_queue( &SysServiceRecv_queue, "/mq_ui_to_sys", NUM_MESSAGES );
	posix_create_message_queue( &SysServiceRecv_queue, "/mq_ui_to_sys", NUM_MESSAGES, sizeof(SYS_UI_MESSAGE) );
#endif
	router_register_queue(SYSTEM_SERVICE_RECEIVER_ID, SysServiceRecv_queue);  // Register a message queue.
	posix_create_thread(&SysServiceRecv_thd_id, SysServiceReceiverThread, 0, "sysservicereceiver", SysServiceRecvStack, SSLSTACK_SIZE, POSIX_THR_PRI_NORMAL);
}

void SysServiceSenderInit(void)
{
	DBG_SSL_PRINTF_SDH_DEBUG("[%s] called \n", __FUNCTION__);

#ifdef UIMGR_CREATED_MQ	 
	SysServiceSender_queue = mq_open( "/mq_sys_to_ui", O_RDWR );
	XASSERT( SysServiceSender_queue != (mqd_t)-1, SysServiceSender_queue );
#else
	//posix_create_MESSAGE_queue( &SysServiceSender_queue, "/mq_sys_to_ui", NUM_MESSAGES );
	posix_create_message_queue( &SysServiceSender_queue, "/mq_sys_to_ui", NUM_MESSAGES, sizeof(SYS_UI_MESSAGE) );
#endif
	router_register_queue(SYSTEM_SERVICE_SENDER_ID, SysServiceSender_queue);  // Register a message queue.	
}

/**
 * Function name: SysServiceReceiverThread
 *
 * \brief Main thread of the system service receiver
 *
 * Receive message from UI process and do required things.
 *
 **/
void *SysServiceReceiverThread(void *unused)
{
	SYS_UI_MESSAGE             nextMsg;
	int                 retcode;

	SysWaitForInit ();
	prctl(PR_SET_NAME, "sysServiceReceiver", 0, 0, 0);

	DBG_SSL_PRINTF_SDH_DEBUG("   Called :  %s  Starting \n", __FUNCTION__);

	CheckInitialMachineSetup();
	// Wait for a message
	while (1)
	{
		retcode = mq_receive(SysServiceRecv_queue, (char*)&nextMsg, sizeof(SYS_UI_MESSAGE), 0); 
#if 0 //For Message Queue Debug
		int mq_retcode;
		struct mq_attr attr;
		mq_retcode = (int)mq_getattr( SysServiceRecv_queue, &attr );	
	
 		 DBG_PRINTF_ERR(" SYS_SERVICE_LAYER:  SSL: Receiver: --->>> ATTR: MaxMsg=%ld, CurMSGS = %ld \n", attr.mq_maxmsg,  attr.mq_curmsgs);			 
#endif

		DBG_SSL_PRINTF_SDH_DEBUG("[%s]: Received Message\n", __FUNCTION__);
		
		XASSERT( retcode == sizeof(SYS_UI_MESSAGE), thread_errno);

		if(nextMsg.msgSender == UI_MANAGER_ID){
			DPRINTF((LOG_SDH_ERR | DBG_OUTPUT),("US: %04x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", 
				nextMsg.cmd, nextMsg.data[0], nextMsg.data[1], nextMsg.data[2], nextMsg.data[3], nextMsg.data[4], nextMsg.data[5],  nextMsg.data[6], 
				nextMsg.data[7], nextMsg.data[8], nextMsg.data[9], nextMsg.data[10], nextMsg.data[11], nextMsg.data[12], nextMsg.data[13]));
		}

//		DBG_SSL_PRINTF_SDH_DEBUG("%s[%d]: got message '%s'=0x%X=%d; param1=0x%08X param2=0x%08X\n", 
//						__FILE__, __LINE__, DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType, nextMsg.msgType, nextMsg.param1, nextMsg.param2 );

		switch(nextMsg.cmd)
		{		
			case CMD_US_INIT_COMPLETION:	//GROUP01_CMD01
			{
				InitCompletionService(&nextMsg);
				break;
			}
			case CMD_INT_MACHINE_CONF:	//GROUP01_CMD81
			{
				MachConfService(&nextMsg);
				break;
			}
			case CMD_US_SETGET_INFO:
			{
				SetInfoService(&nextMsg);
				break;
			}
			case CMD_US_CONFIG_CHANGED:		//GROUP01_CMD03 
			{
				SPChangedService(&nextMsg);
				break;
			}
			case CMD_US_SCR_CHANGE_REQ: 	//GROUP02_CMD01
			{
				ScreenChangeReqService(&nextMsg);
				break;
			}			
			case CMD_US_KEY_PRESSED:	//GROUP02_CMD02 
			{
				KeyTouchEventService(&nextMsg);
				break;
			}	
			case CMD_INT_ENERGY_MODE:	//GROUP02_CMD82
			{
				EnergySaveModeService(&nextMsg);
				break;
			}
			case CMD_INT_HOUR_ALARM:
			{
				HourlyAlarmService(&nextMsg);
				break;
			}
			case CMD_INT_TIMELY_ALARM:
			{
				TimelyAlarmService(&nextMsg);
				break;
			}
			case CMD_US_AUTH_REQ:			//AUTH GROUP03_CMD01
			{
				AuthUserActionRequest(&nextMsg);
				break;
			}
			case CMD_US_SOL_AUTH_RES:			//AUTH GROUP03_CMD03
			{
				AuthSolUserActionResult(&nextMsg);
				break;
			}
			case CMD_INT_CS_AUTH_RESULT:			//AUTHMGR->SSM GROUP03_CMD20
			{
				AuthUserActionResult(&nextMsg);
				break;
			}
			case CMD_INT_PARSER_AUTH_RESULT:			//PJLPARSER->SSM Auth Result
			{
				AuthParserAuthResult(&nextMsg);
				break;
			}
			case CMD_INT_PARSER_AUTH_REQ:			//PJLPARSER->SSM Auth Request
			{
				AuthParserAuthRequest(&nextMsg);
				break;
			}
			case CMD_INT_SCANNER_JAM_STATUS:
			{
				ScanmanEventService(&nextMsg);
				break;
			}
			case CMD_INT_PLOTTER_MODE_STATUS:	//GROUP04_CMD84
			{
				PlotterModeStatusService(&nextMsg);
				break;
			}		
			case CMD_INT_PLOTTER_DOOR_STATUS:	//GROUP04_CMD85
			{
				PlotterDoorOpenService(&nextMsg);
				break;
			}
			case CMD_INT_PLOTTER_TRAY_STATUS:	//GROUP04_CMD86
			{
				PlotterFeedTrayStatusService(&nextMsg);
				break;
			}			
			case CMD_INT_PLOTTER_JAM_STATUS:		//GROUP04_CMD87
			{
				PlotterJamStatusService(&nextMsg);
				break;
			}
			case CMD_INT_PLOTTER_TONER_STATUS:		//GROUP04_CMD88
			{
				TonerDrumStatusService(&nextMsg);
				break;
			}			
			case CMD_INT_PLOTTER_SC_STATUS:		//GROUP04_CMD89
			{
				SCStatusService(&nextMsg);
				break;
			}
			case CMD_US_PLOTTER_PCU_RESET:		//GROUP04_CMD0A
			{
				PCUResetService(&nextMsg);
				break;
			}
			case CMD_US_PRINT_GOPRINT:	//GROUP05_CMD01
			{
				GoPrintService(&nextMsg);
				break;
			}
			case CMD_INT_PRINT_GOPRINT_RES:	//GROUP05_CMD81
			{
				PrintGoPrintResultService(&nextMsg);
				break;
			}
			case CMD_US_PRINT_GOLAMINATION: {
				GoLaminationService(&nextMsg);
				break;
			}
			case CMD_INT_PRINT_GOLAMINATION_RES: {
				GoLaminationResultService(&nextMsg);
				break;
			}
			case CMD_US_SCAN_GOSCAN:		//GROUP06_CMD01
			{
				ScanAppService(&nextMsg);
				break;
			}
			case CMD_INT_SCAN_GOSCAN_RES:	//GROUP06_CMD81
			{
				ScanAppEventService(&nextMsg);
				break;
			}
			case CMD_US_SCAN_GOSCANSEND:		//GROUP06_CMD03
			{
				ScanSendService(&nextMsg);
				break;
			}
			case CMD_INT_SCAN_GOSCANSEND_RES:	//GROUP06_CMD83
			{
				ScanSendResultService(&nextMsg);
				break;
			}
			case CMD_US_COPY_GOCOPY:	//GROUP07_CMD01
			{
				CopyAppService(&nextMsg);
				break;
			}
			case CMD_INT_COPY_GOCOPY_RES:	//GROUP07_CMD81:
			{
				CopyGoCopyResultService(&nextMsg);
				break;
			}
			case CMD_US_FAX_GO_FAX_SCAN:	//MSG_GO_FAX_SCAN
			case CMD_INT_FAX_GO_ON_HOOK:	//MSG_GO_ON_HOOK
			case CMD_INT_FAX_GO_UPDATE_PARAM: //MSG_GO_UPDATE_PARAM
			case CMD_INT_FAX_SEND_FAX_STATUS:	//MSG_FAX_STATUS
			case CMD_INT_FAX_SEND_FAX_MEMORY_STATUS:
			case CMD_INT_FAX_SEND_FAX_REQUEST_READ_DB:
			case CMD_INT_FAX_SEND_FAX_BROADCAST_JOBID:
			case CMD_INT_FAX_SEND_PCFAX_JOBID:
			case CMD_INT_FAX_GO_FAXSCAN_MODE:
			case CMD_INT_FAX_GO_FAX_CANCEL:
			case CMD_INT_FAX_GO_FAX_DELETE_PENDING_JOB:
			case CMD_INT_FAX_GO_FAX_PRINT_REPORT:
			case CMD_INT_FAX_GO_FAX_RESET_DB:
			case CMD_INT_FAX_GO_FAX_SP_MODE:
			case CMD_INT_FAX_GO_FAX_ACK_READ_DB:
			case CMD_INT_FAX_GO_FAX_DELETE_DOCMAN:
			case CMD_US_FAX_GO_SOLUTION_FAX:
			{
				FaxGoService(&nextMsg);
				break;
			}
			case CMD_INT_TX_SEND_RESULT:
			{
				SendTxResultService(&nextMsg);
				break;
			}
			case CMD_INT_FAX_SJM_RES:
			{
				FaxSJMResultService(&nextMsg);
				break;
			}
			case CMD_US_NETWORK_REQUEST:
			{
				NetworkResponseService(&nextMsg);
				break;
			}
			case CMD_US_SP_REQUEST:
			{
				SPRequestService(&nextMsg);
				break;
			}
			case CMD_INT_PRINT_SUB_SYSTEM:
			{
				PrintSubSystemService(&nextMsg);
				break;
			}
			case CMD_INT_COPY_APP_EVENT:
			{
				CopyAppEventService(&nextMsg);
				break;
			}
			case CMD_INT_PRINT_PAPER_MISMATCH:
			{
				InputTrayPaperService(&nextMsg);
				break;
			}
			case CMD_INT_WRONG_IMAGE_SIZE:
			{
				PrintWrongImageService(&nextMsg);
				break;
			}
			case CMD_INT_PAGE_DONE_INFO:	//Page done from JM
			{
				PrintPageDoneInfoService(&nextMsg);
				break;
			}
			case CMD_INT_JOB_DONE_INFO:	//Page done from JM
			{
				JobDoneInfoService(&nextMsg);
				break;
			}
			case CMD_INT_USB_THUMB_DRIVE:
			{
				USBThumbDriveService(&nextMsg);
				break;
			}
			case CMD_INT_PCU_STATUS:
			{
				PCUStatusService(&nextMsg);
				break;
			}
			case CMD_INT_TD_SYS_JOB_ACTION:
			{
				SysJobTakeActionService(&nextMsg);
				break;
			}
			case CMD_INT_SPECIAL_NOTIFICATION:
			{
				SpecialNotificationService(&nextMsg);
				break;
			}
			default:
				break;
		} // end switch ( nextMsg.msgType )
	} // end while (1)
	return 0;
} // end SysServiceLayerThread()

/*
*
* \brief System Service Command to send dummy message to UI process.
* 
* 10 - Front Door Open 11 - Upper Door Open 12 - Finisher Door Open 13 - Bridge Door Open 
* 20 - Tray Manual Empty 21 - Tray 1 Empty 22 - Tray 2 Empty 23 - Tray 3 Empty 24 - Tray 4 Empty
* 30 - Pickup Jam Occur  31 - Feed Jam Occur  32 - LE Position Jam Occur  33 - TE Position Jam Occur  34 - Reverce Jam Occur 
* 40 - Toner End  41 - Toner 10%  43 - Toner 40%  47 - Toner 70%   49 - Toner 90% 
* 50 - Printer Ready  51 - Printer Busy  52 - Printer Warmup  53 - Printer Energy Save  54 - Printer state Door Open
* 
*/
int SysServiceCmd(int argc, char* argv[])
{
	uint32_t msgNo;
	MESSAGE msg;
	//error_type_t err;
	bool msgSendFlag = false;

	if( argc != 2) 
	{
		return CMD_USAGE_ERROR;
	}

	if( str_mkint( argv[1], &msgNo ) != 0 ) 
	{
		cmd_printf("Bad integer \"%s\" for Message No \n", argv[1] );
		return CMD_USAGE_ERROR;
	}

	cmd_printf( "Message No: *%u\n", msgNo );

	switch(msgNo)
	{
		case 0: /* TimeZoneNo of CONF_SYSTEM is set properly depending on countrycode */
			{ 
				/* Caution!!
				   1. do not remove or modify because it is used in /app/karas_pinetree_util.sh
				   2. this is called on doing factory reset and setting TLI in only abroad TLI
				   3. input : MachineTLI in INST_MACHINE
								  INITIALMACHINECOUNTRYCODE in CONF_SYSTEMSP
				   				  CountryTable[] Table
				   4. output : TimeZoneNo in CONF_SYSTEM
				 */
				int zone_no;
				error_type_t db_retval = FAIL;
				char* pBuffer = NULL;
				char machineTLI[30] = {0, };
				memset(machineTLI, 0, sizeof(machineTLI));
				/* read TLI */
				pBuffer = db_get_var("INST_MACHINE", "MachineTLI", &db_retval);
				if(db_retval == OK)
				{
					if(pBuffer)
					{
						snprintf(machineTLI, sizeof(machineTLI), pBuffer);
						MEM_FREE_AND_NULL(pBuffer);
					}
					DBG_SSL_PRINTF_SDH_ERR("Machine TLI : %s\n", machineTLI);
				}
				else
				{
					DBG_SSL_PRINTF_SDH_ERR("TLI DB Read Fail\n");
				}

#define CONTURY_CODE_1 4
#define CONTURY_CODE_2 5
				if( (machineTLI[CONTURY_CODE_1] == 'L' && machineTLI[CONTURY_CODE_2] == 'V') ||
						(machineTLI[CONTURY_CODE_1] == 'H' && machineTLI[CONTURY_CODE_2] == 'V')) { /* check if TLI is for ABROAD */
					if ( IS_KARAS || IS_ABBA2H ) { // for Printer
						zone_no = 9;
					}
					else { // for Copier
						uint32_t CountryCode;
						zone_no = 9;
						CountryCode = db_get_var_int("CONF_SYSTEMSP", "InitialMachineCountryCode", &db_retval);
						if(db_retval == OK)
						{
							if ( CountryCode < MAX_COUNTRY_CODE ) {
								zone_no = CountryTable[CountryCode].TimeZoneNo;
							}
						}
						else
						{
							DBG_SSL_PRINTF_SDH_ERR("InitialMachineCountryCode DB Read Fail\n");
						}
						/*set Timezone*/
						char query[128];
						error_type_t db_retval = FAIL;
						char timeZone[50] = {0, };
						memset( query, 0, sizeof(query));
						sprintf(query, "select TimeZone from RECD_TIMEZONE where NO = %d",  zone_no);
						DBG_SSL_PRINTF_SDH_ERR("query: %s\n", query);
						pBuffer = db_get_var_query(query, &db_retval);
						if(db_retval == OK && pBuffer) {
							snprintf(timeZone, sizeof(timeZone), pBuffer);
							MEM_FREE_AND_NULL(pBuffer);
							db_set_var("CONF_SYSTEM", "TimeZone", timeZone);
						}
						else
						{
							DBG_SSL_PRINTF_SDH_ERR("%s %d DB Read fail\n", __func__, __LINE__);
						}
						db_retval = db_set_var_int("CONF_SYSTEM", "TimeZoneNo", zone_no);
						if(db_retval == FAIL){
							DBG_SSL_PRINTF_SDH_ERR("[%s] DB set fail for TimeZoneNo\n", __FUNCTION__);
						}
						DBG_SSL_PRINTF_SDH_ERR("TimeZone:%s, TimeZoneNo:%d\n", timeZone, zone_no);
					}
				}
				break;
			}
		case 10:
		case 11:
		case 12:
		case 13:
			msg.msgType = MSG_STATUS_DOOR_OPEN;
			msg.param1 = (1<<(msgNo-10));		
			msg.param2 = 0; 		

			msgSendFlag = true;			
			break;
		case 20: 
		case 21:
		case 22:
		case 23:
		case 24:
			msg.msgType = MSG_STATUS_TRAY_EMPTY;
			msg.param1 = (msgNo -20);	
			msg.param2 = 0; 		

			msgSendFlag = true;
			break;
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
			msg.msgType = MSG_STATUS_JAM;
			msg.param1 = (msgNo - 30);		
			msg.param2 = 1; 		

			msgSendFlag = true;			
			break;
		case 40: 
		case 41:
		case 43:
		case 44:
		case 49:			
			msg.msgType = MSG_STATUS_TONER;
			msg.param1 = (0x00 | (msgNo-40));	
			msg.param2 = 0;  		
			msgSendFlag = true;
			break;
		case 50: 
		case 51:
		case 52:
		case 53:
		case 54:
			msg.msgType = MSG_STATE_PRINTJOB;
			msg.param1 = (msgNo - 50);	
			msg.param2 = 0; 		

			msgSendFlag = true;
			break;
		case 101:
			g_bDebugESMTimer = true; //ON: Debug Energy Save Mode timer
			break;
		case 102:
			g_bDebugESMTimer = false; //OFF: Debug Energy Save Mode timer
			break;
		case 103:
			g_bDebugAdaptiveTimer = true; //ON: Debug Adaptive Energy Save Mode timer
			break;
		case 104:
			g_bDebugAdaptiveTimer = false; //OFF: Debug Adaptive Energy Save Mode timer
			break;
		case 105:
			g_bDebugHourlyTimer = true; //ON: Debug Hourly timer
			break;
		case 106:
			g_bDebugHourlyTimer = false; //OFF: Debug Hourly timer
			break;
		case 107:
			g_bDebugtimelyTimer = true; //ON: Debug Timely timer
			break;
		case 108:
			g_bDebugtimelyTimer = false; //OFF: Debug Hourly timer
			break;
		case 110:
			//Debug SJM all jobs
			SJMGetAllJobInfo();
			break;
		case 111:
		{
			int mq_retcode;
			struct mq_attr attr;
			mq_retcode = (int)mq_getattr( SysServiceRecv_queue, &attr );
			cmd_printf("SSM: Receiver --->>> ATTR: MaxMsg=%ld, CurMSGS = %ld \n", attr.mq_maxmsg,  attr.mq_curmsgs);
		}
			break;
		case 115:
		{
			ATLISTENTRY* pScanJobNode = NULL;
			stIC_SysJobInfoList* pScanJobInfo = NULL;
			UINT8 listCount = 0;
			pScanJobNode = ATListHead(&g_ScanJobList);
			while(pScanJobNode){
				pScanJobInfo = CONTAINING_RECORD(pScanJobNode, stIC_SysJobInfoList, infoNode);
				if (pScanJobInfo){
					DBG_SSL_PRINTF_SDH_ERR("[%d]: JobNO:%d Flag:%d DestCount:%d  TotalPage:%d JobType:%s\n",
							listCount, pScanJobInfo->pInfo->jobNum, pScanJobInfo->destFlag,
							pScanJobInfo->destCount, pScanJobInfo->pInfo->totalPages,
							to_string_job_types(pScanJobInfo->pInfo->jobDocType));
				}
				pScanJobInfo = NULL;
				++listCount;
				pScanJobNode = ATListNext(&g_ScanJobList, pScanJobNode);
			}
		}
		break;
		case 320:
			GoJobLog(NULL);
			break;
		case 500:
			printf("Tray Count = %d\n", GetTrayInform(SETTING_TRAY_COUNT, 0));
			printf("Tray status 0 = %d\n", GetTrayInform(TRAY_STATUS, 0));
			printf("Tray status 1 = %d\n", GetTrayInform(TRAY_STATUS, 1));
			printf("Tray status 2 = %d\n", GetTrayInform(TRAY_STATUS, 2));
			printf("Tray status 3 = %d\n", GetTrayInform(TRAY_STATUS, 3));
			printf("Tray amount 0 = %d\n", GetTrayInform(TRAY_PAPER_AMOUNT, 0));
			printf("Tray amount 1 = %d\n", GetTrayInform(TRAY_PAPER_AMOUNT, 1));
			printf("Tray amount 2 = %d\n", GetTrayInform(TRAY_PAPER_AMOUNT, 2));
			printf("Tray amount 3 = %d\n", GetTrayInform(TRAY_PAPER_AMOUNT, 3));
			printf("Tray size 0 = %d\n", GetTrayInform(TRAY_PAPER_SIZE, 0));
			printf("Tray size 1 = %d\n", GetTrayInform(TRAY_PAPER_SIZE, 1));
			printf("Tray size 2 = %d\n", GetTrayInform(TRAY_PAPER_SIZE, 2));
			printf("Tray size 3 = %d\n", GetTrayInform(TRAY_PAPER_SIZE, 3));
			break;
		case 9910:	//Assert
			{
				int failError = 1;
				XASSERT( failError==0, failError );
			}
			break;
		case 9912:	//Segmentation Fault
			{
				char *s="hello world";
				*s='H';
				//int i = *(int*)0;
			}
			break;
		default:
			cmd_printf( "Error: Command not found for Message No: *%u \n", msgNo );
			break;
	}
	
	if(msgSendFlag)
	{
		msg.param3 = NULL;	//no use
		//err = SYMsgSend(SYSTEM_SERVICE_SENDER_ID, &msg);		 
		//XASSERT( err==OK, err );
	}

	return CMD_OK;
} // end SysServiceCmd()


void SysServiceDebugInit()
{
#ifdef HAVE_CMD
	cmd_register("ssm", "System Service Manager Commands", NULL, NULL, NULL);
	cmd_register("ssm send", "\nSend command to SSM:\n\t Commands Details:\
			\n\t Energy Save Timer debug mode: 101 - ON \t 102 - OFF\t\
			\n\t Adaptive Energy save timer debug mode: 103 - ON\t 104 - OFF\
			\n\t Hourly timer debug mode: 105 - ON\t 106 - OFF \
			\n\t Timely timer debug mode: 107 - ON\t 108 - OFF \
			\n\t Show SJM jobs info: 110\
			\n\t Show SystemService Queue Attr: 111\
			\n\t Make SC Error: 9910 - Software ASSERT\t9912 - Segmentation Fault",
			"Example:\n\tFor Software Assert: cmd>ssm send 9910\n", NULL, SysServiceCmd);
#endif
}

void get_new_tz_info(struct timezone *ptz) {
	time_t t;
	struct tm tm;
	time(&t);
	localtime_r(&t, &tm);
	ptz->tz_dsttime = (tm.tm_isdst > 0)? 1:0;
	ptz->tz_minuteswest = -1 * tm.tm_gmtoff / 60;

//    printf("[%s %d]tz_minuteswest: %d, tz_dsttime: %d\n", __func__, __LINE__, ptz->tz_minuteswest, ptz->tz_dsttime); 
	return;
}

int rtc_time_check() {
	struct timeval tnow;
	struct tm check;
	time_t tcheck;

	check.tm_year = 2013 - 1900;
	check.tm_mon = 1 - 1;
	check.tm_mday = 2;
	check.tm_hour = 0;
	check.tm_min = 0;
	check.tm_sec = 0;
	check.tm_isdst = 0;

	tcheck = mktime(&check);
	gettimeofday(&tnow, NULL);
	DBG_SSL_PRINTF_SDH_ERR("tnow.tv_sec :%d, tcheck :%d\n", (int)tnow.tv_sec, (int)tcheck);
	if ( (int)tnow.tv_sec < (int)tcheck ) {
		DBG_SSL_PRINTF_SDH_ERR("invalid rtc time\n");
		return -1;
	}
	return 0;
}

#define MAX_COUNTRY_CODE	72+1
const stCountryTable CountryTable[] = {
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_KOREA,					9	},	// 00:for Factory Default   (GMT+09:00)
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_UNITED_ARAB_EMIRATES,	103	},	// 01:Arab Emirates(UAE)	(GMT+04:00) Abu Dhabi, Muscat, -
	{ LANGUAGE_SPANISH,		A4_BASE,	T35_ARGENTINA,				45	},	// 02:Argentina				(GMT-03:00) Buenos Aires, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_AUSTRALIA,				125	},	// 03:Australia				(GMT+10:00) Canberra, Melbourne, Sydney, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_AZERBAIJAN,				101	},	// 04:Azerbaijan			(GMT+04:00) Baku, ON
	{ LANGUAGE_ENGLISH,		LT_BASE,	T35_BAHAMAS,				32	},	// 05:Bahamas				(GMT-05:00) Indiana (East), ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_BAHRAIN,				97	},	// 06:Bahrain				(GMT+03:00) Baghdad, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_BELGIUM,				68	},	// 07:Belgium				(GMT+01:00) Brussels, Copenhagen, Madrid, Paris, ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_BOLIVIA,				40	},	// 08:Bolivia				(GMT-04:00) Georgetown, La Paz, Manaus, San Juan, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_BOSNIA_HERZEGOVINA,		70	},	// 09:Bosnia and Herzegovin	(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb, ON
	{ LANGUAGE_PORTUGUESE,	A4_BASE,	T35_BRAZIL,					43	},	// 10:Brazil				(GMT-03:00) Brasilia, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_BULGARIA,				94	},	// 11:Bulgaria				(GMT+02:00) Sofia, Tallinn, Vilnius, ON
	{ LANGUAGE_ENGLISH,		LT_BASE,	T35_CANADA,					32	},	// 12:Canada				(GMT-05:00) Indiana (East), ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_CHILE,					41	},	// 13:Chile					(GMT-04:00) Santiago, ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_COLOMBIA,				31	},	// 14:Colombia				(GMT-05:00) Bogota, Lima, Quito, -
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_COSTA_RICA,				27	},	// 15:Costa Rica			(GMT-06:00) Central America, -
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_CUBA,					32	},	// 16:Cuba					(GMT-05:00) Indiana (East), ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_CYPRUS,					80	},	// 17:Cyprus				(GMT+02:00) E. Europe, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_CZECH_REPUBLIC,			66	},	// 18:Czech					(GMT+01:00) Budapest, Ljubljana, Prague, ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_DOMINICAN_REPUBLIC,		37	},	// 19:Dominican Republic	(GMT-04:00) Atlantic Time (Canada), OFF
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_ECUADOR,				31	},	// 20:Ecuador				(GMT-05:00) Bogota, Lima, Quito, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_EGYPT,					91	},	// 21:Egypt					(GMT+02:00) Cairo, OFF
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_EL_SALVADOR,			27	},	// 22:El Salvador			(GMT-06:00) Central America, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_ESTONIA,				94	},	// 23:Estonia				(GMT+02:00) Sofia, Tallinn, Vilnius, ON
	{ LANGUAGE_FRANCH,		A4_BASE,	T35_FRANCE,					68	},	// 24:France				(GMT+01:00) Brussels, Copenhagen, Madrid, Paris, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_GERMANY,				74	},	// 25:Germany				(GMT+01:00) Amsterdam, Berlin, Bern, ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_GUATEMALA,				27	},	// 26:Guatemala				(GMT-06:00) Central America, -
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_HONDURAS,				27	},	// 27:Honduras				(GMT-06:00) Central America, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_HONGKONG,				8	},	// 28:Hong Kong				(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_INDIA,					110	},	// 29:India					(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_INDIA,					110	},	// 30:India-OEM				(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_IRAN,					100	},	// 31:Iran					(GMT+03:30) Tehran, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_IRAQ,					97	},	// 32:Iraq					(GMT+03:00) Baghdad, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_ITALY,					76	},	// 33:Italy					(GMT+01:00) Rome, Stockholm, Vienna, ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_JAMAICA,				33	},	// 34:Jamaica				(GMT-05:00) Indiana (East), OFF
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_JORDAN,					86	},	// 35:Jordan				(GMT+02:00) Amman, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_KAZAKHSTAN,				111	},	// 36:Kazakhstan			(GMT+06:00) Astana, Dhaka, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_KUWAIT,					99	},	// 37:Kuwait				(GMT+03:00) Kuwait, Riyadh, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_LATVIA,					92	},	// 38:Latvia				(GMT+02:00) Helsinki, Kyiv, Riga, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_LEBANON,				82	},	// 39:Lebanon				(GMT+02:00) Beirut, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_LIBYAN_ARAB_JAMAHIRIYA,	81	},	// 40:Libya					(GMT+02:00) E. Europe, OFF
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_LITHUANIA,				80	},	// 41:Lithuania				(GMT+02:00) E. Europe, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_LUXEMBOURG,				68	},	// 42:Luxembourg			(GMT+01:00) Brussels, Copenhagen, Madrid, Paris, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_MALAYSIA,				7	},	// 43:Malaysia				(GMT+08:00) Kuala Lumpur, Singapore, -
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_MEXICO,					25	},	// 44:Mexico				(GMT-06:00) Guadalajara, Mexico City, Monterrey, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_NETHERLANDS,			74	},	// 45:Netherlands			(GMT+01:00) Amsterdam, Berlin, Bern, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_NEW_ZEALAND,			134	},	// 46:New Zealand			(GMT+12:00) Auckland, Wellington, ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_NICARAGUA,				27	},	// 47:Nicaragua				(GMT-06:00) Central America, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_OMAN,					103	},	// 48:Oman					(GMT+04:00) Abu Dhabi, Muscat, -
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_PANAMA,					32	},	// 49:Panama				(GMT-05:00) Indiana (East), ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_PARAGUAY,				34	},	// 50:Paraguay				(GMT-04:00) Asuncion, ON
	{ LANGUAGE_SPANISH,		A4_BASE,	T35_PERU,					31	},	// 51:Peru					(GMT-05:00) Bogota, Lima, Quito, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_POLAND,					70	},	// 52:Poland				(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb, ON
	{ LANGUAGE_PORTUGUESE,	A4_BASE,	T35_PORTUGAL,				58	},	// 53:Portugal				(GMT) Dublin, Edinburgh, Lisbon, London, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_QATAR,					99	},	// 54:Qatar					(GMT+03:00) Kuwait, Riyadh, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_ROMANIA,				80	},	// 55:Romania				(GMT+02:00) E. Europe, ON
	{ LANGUAGE_RUSSIAN,		A4_BASE,	T35_RUSSIAN_FEDERATION,		10	},	// 56:Russia				(GMT+04:00) Moscow, St. Petersburg, volgograd, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_SAUDI_ARABIA,			99	},	// 57:Saudi Arabia			(GMT+03:00) Kuwait, Riyadh, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_SINGAPORE,				7	},	// 58:Singapore				(GMT+08:00) Kuala Lumpur, Singapore, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_SOUTH_AFRICA,			5	},	// 59:South Africa			(GMT+02:00) Harare, Pretoria, -
	{ LANGUAGE_SPANISH,		A4_BASE,	T35_SPAIN,					68	},	// 60:Spain					(GMT+01:00) Brussels, Copenhagen, Madrid, Paris, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_SWEDEN,					76	},	// 61:Sweden				(GMT+01:00) Rome, Stockholm, Vienna, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_SWITZERLAND,			74	},	// 62:Switzerland			(GMT+01:00) Amsterdam, Berlin, Bern, ON
	{ LANGUAGE_ARABIC,		A4_BASE,	T35_TUNISIA,				77	},	// 63:Tunisia				(GMT+01:00) Rome, Stockholm, Vienna, OFF
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_TURKEY,					80	},	// 64:Turkey				(GMT+02:00) E. Europe, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_UKRAINE,				92	},	// 65:Ukraine				(GMT+02:00) Helsinki, Kyiv, Riga, ON
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_UNITED_KINGDOM,			58	},	// 66:United Kingdom		(GMT) Dublin, Edinburgh, Lisbon, London, ON
	{ LANGUAGE_ENGLISH,		LT_BASE,	T35_UNITED_STATES,			1	},	// 67:United States			(GMT-05:00) Eastern time (US & Canada), ON
	{ LANGUAGE_SPANISH,		A4_BASE,	T35_URUGUAY,				49	},	// 68:Uruguay				(GMT-03:00) Montevideo, ON
	{ LANGUAGE_SPANISH,		LT_BASE,	T35_VENEZUELA,				40	},	// 69:Venezuela				(GMT-04:00) Georgetown, La Paz, Manaus, San Juan, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_NEPAL,					146	},	// 70:Nepal					(GMT+05:45) Kathmandu, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_INDONESIA,				147	},	// 71:Indonesia				(GMT+09:00) Jayapura, -
	{ LANGUAGE_ENGLISH,		A4_BASE,	T35_THAILAND,				114	},	// 72:Thailand				(GMT+07:00) Bangkok, Hanoi, Jakarta 
};

int SetCountryCode(uint32_t code) {
	int ret = 0;
	int data;
	error_type_t db_retval;

	DBG_SSL_PRINTF_SDH_ERR ("%s code: %d\n", __func__, code);
	if ( code < MAX_COUNTRY_CODE ) {
		/* Language	*/
		data = CountryTable[code].language;
		DBG_SSL_PRINTF_SDH_ERR ("language: %d\n", data);
		db_retval = db_set_var_int("CONF_SYSTEM", "Linguistic", data);
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		db_retval = db_set_var_int("CONF_SYSTEMSP", "initialMachineLinguistic", data);
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		system("rm /app/.browser/cache/file* -rf");

		/* Paper Size: A4, LT */
		data = CountryTable[code].paperType;
		DBG_SSL_PRINTF_SDH_ERR ("paperType: %d\n", data);
		db_retval = db_set_var_int("CONF_COPYSET", "OriginalSize", (data==A4_BASE)?MEDIASIZE_A4:MEDIASIZE_LETTER );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		db_retval = db_set_var_int("CONF_SCANSET", "OriginalSize", (data==A4_BASE)?MEDIASIZE_A4:MEDIASIZE_LETTER );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		db_retval = db_set_var_int("CONF_FAXUP", "TxOriginalSize", (data==A4_BASE)?MEDIASIZE_A4:MEDIASIZE_LETTER );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
#define SP_DISABLE 0
#define SP_ENABLE 1
		db_retval = db_set_var_int("CONF_SYSTEMSP", "LegalMerge", (data==A4_BASE)?SP_DISABLE:SP_ENABLE );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
#define SELECT_A3_DEFAULT     0
#define SELECT_1117_DEFAULT   1
		db_retval = db_set_var_int("CONF_PRNT", "SELECT_A31117", (data==A4_BASE)?SELECT_A3_DEFAULT:SELECT_1117_DEFAULT );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", (data==A4_BASE)?MEDIASIZE_A4:MEDIASIZE_LETTER );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", (data==A4_BASE)?MEDIASIZE_A4:MEDIASIZE_LETTER );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", (data==A4_BASE)?MEDIASIZE_A4:MEDIASIZE_LETTER );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
		db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", (data==A4_BASE)?MEDIASIZE_A4:MEDIASIZE_LETTER );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);

		/*FaxCode*/
		data = CountryTable[code].faxCode;
		DBG_SSL_PRINTF_SDH_ERR ("faxCode: %d\n", data);
		db_retval = db_set_var_int("CONF_FAXSP", "SelectCountry", data );
		if(db_retval == FAIL) DBG_SSL_PRINTF_SDH_ERR("%s %d DB Write fail\n", __func__, __LINE__);
	
		/*Timezone*/
		char query[128];
		error_type_t db_retval = FAIL;
		char* pBuffer = NULL;
		char timeZone[50] = {0, };
		memset( query, 0, sizeof(query));
		sprintf(query, "select TimeZone from RECD_TIMEZONE where NO = %d",  CountryTable[code].TimeZoneNo);
		DBG_SSL_PRINTF_SDH_ERR("query: %s\n", query);
		pBuffer = db_get_var_query(query, &db_retval);
		if(db_retval == OK && pBuffer) {
			snprintf(timeZone, sizeof(timeZone), pBuffer);
			MEM_FREE_AND_NULL(pBuffer);
			db_set_var("CONF_SYSTEM", "TimeZone", timeZone);
		}
		else
		{
			DBG_SSL_PRINTF_SDH_ERR("%s %d DB Read fail\n", __func__, __LINE__);
		}
		db_retval = db_set_var_int("CONF_SYSTEM", "TimeZoneNo", CountryTable[code].TimeZoneNo);
		if(db_retval == FAIL){
			DBG_SSL_PRINTF_SDH_ERR("[%s] DB set fail for TimeZoneNo\n", __FUNCTION__);
		}
		DBG_SSL_PRINTF_SDH_ERR("TimeZone:%s, TimeZoneNo:%d\n", timeZone, CountryTable[code].TimeZoneNo);
	}
	else {
		ret = 1; //NG
	}
	return ret;
}

void CheckInitialMachineSetup(void) {
	error_type_t db_retval = FAIL;
	int initialMachineSetup;
	initialMachineSetup = db_get_var_int("CONF_SYSTEMSP", "INITIALMACHINESETUP", &db_retval);
	if(db_retval == FAIL) {
		initialMachineSetup = 0;
		DBG_SSL_PRINTF_SDH_ERR("%s DB Read fail\n", __func__);
	}

	if ( initialMachineSetup ) {
		gInitialMachineSetupProceeding = 1;
		error_type_t err;
		SYS_UI_MESSAGE msgToSend;
		stWnd_ScreenChgReqResult stScrChgReqResult = {0}; 

		msgToSend.msgSender = SYSTEM_SERVICE_ID; 
		msgToSend.cmd = CMD_SU_SCR_CHANGE_RES;
		memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);

		stScrChgReqResult.screen = SCREEN_INIT_MACHINE_SETUP;
		stScrChgReqResult.result = SCREEN_CHANGE_OK;	//OK
		memcpy(msgToSend.data, &stScrChgReqResult, sizeof(stWnd_ScreenChgReqResult));
		err = SysUI_MsgSend(SYSTEM_SERVICE_SENDER_ID, &msgToSend);
		XASSERT( err==OK, err );
	}
}
//END 
