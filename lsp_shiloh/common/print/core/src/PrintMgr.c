/*
 *
 * ============================================================================
 * Copyright (c) 2004-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file PrintMgr.c
 *
 * \brief This thread manages the printing of a page.

    Note that the MSG_STATUS_UPDATE is sent ONLY by engine and video and ONLY
    to the print manager. Implicitly, if print manager gets this message, it
    is from engine or video. Print manager is a clearing house  for status from
    engine and video. This "clearing house" operation was over-ridden for the
    case of engine media size misprint.

    Here is the sequence of events:

    1. Engine posts misprint size error; e.g. print letter document and load
       A6 paper. Certain engines will enter a lengthy cleaning cycle after the
       misprint error. The misprint error should be shown immediately, along
       with the continue button illuminated. This cleaning cycle didn't exist
       in all engines and must be accounted for as it changes status reporting timing
       significantly.

    2. To clear the misprint, press the continue button. Control panel code checks
       to see if the Print Manager has status of "Misprint Stall". This is
       determined via an API function call. This API is used because the current
       status as saved in the datastore may have been overwritten by another
       module; Control panel sends MSG_USER_INPUT to the status
       manager.

    3. Status manager, after receiving the MSG_USER_INPUT does the following:
        a. Looks for a module (resource?) waiting for user input. Waiting for
           user input is determined by a bit set in the posted status word.
        b. If a module waiting for user input is found, the message is forwarded
           to it. This results in MSG_USER_INPUT being sent to print manager.

    4. Print manager receives the user input message. The global variable
       gPrintMgrInfo mode field is set to PM_STALLED. As a result of this
       message, PrintMgrPrintPages() => PrintMgrHandleForcedStall() call
       sequence is executed. This leads to MSG_USER_ERR_RECOVERY being sent
       to the engine.

     5. Engine receives the MSG_USER_ERR_RECOVERY, this leads to a call to
       EngineHandlePageCancel(), which sends a command to the engine to clear
       the misprint. This command action is filtered on the MSG_USER_ERR_RECOVERY,
       a change from previous code. This clearing of the misprint will lead
       to the generation of an online message being sent to print manager - see
       #6.

     6. Engine status change interrupt occurs when misprint is cleared. Engine
       status function will send "worser" status message to print manager. This
       can include paper out, paper jam, etc. Generally this will be an "online"
       message if all is well with the engine. The case of paperout and paperjam
       are handled as special cases with the misprint error to make sure they
       get reported correctly with the size misprint.

     7. Print manager receives MSG_STATUS_UPDATE with the latest engine status.
        MSG_STATUS_UPDATE messages, by convention, are sent to print manager only
        by video and engine. The problem is that print manager is running its
        stall state machines and although the engine thinks the stall is cleared,
        print manager isn't sure. The print manager will "drop status" in these
        cases and be forever stuck in its state machine. The new code filters
        some status messages and forwards them to the status manager, and clears
        the stall state variables.

 *
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "agRouter.h"
#include "posix_ostools.h"
#include "PrintMgr.h"
#include "agprint.h"
#include "print_job_types.h"

#include "lassert.h"
#include "delay_api.h"
#include "print_logger.h"
#include "debug.h"
#include "platform.h"
#include "agResourceMgr.h"

#include "engPaperHandlingAPI.h"

#include "sys_init_api.h"
#include "PrintMgr_Priv.h"
#include "TrayPaperResolve.h"

#include "printvars_api.h"
#include "utils.h"
#include "response_subject.h"
#include "cmd_proc_api.h"
#include "PrintJobInfo.h"
#include "SysUI_Interface.h"
#include "print_job_api.h"

#include "db_api.h"
#include "spec.h"

#define DBG_PRFX "PM: "
#define PMDEBUG      (DBG_LOUD | DBG_OUTPUT)
#define PMDEBUGERROR (DBG_ERROR | DBG_OUTPUT)
#define PMDEBUGLOUD  (DBG_LOUD  | DBG_OUTPUT)
#define PMDEBUGSOFT  (DBG_SOFT  | DBG_OUTPUT)
#define PMENTEREXIT  0 // Disable enter/exit messages.
//#define PMENTEREXIT (DBG_ENTRY | DBG_OUTPUT) // Enable enter/exit messages.


typedef enum
{
    ADD_PAGEPRESTART = 0,
    ADD_PAGESTART,
}ADDTYPE;

typedef enum
{
    PRMGR_PAGE_PENDING = 0,  ///< got pre page (have page header and maybe page data)
    PRMGR_WAIT_FOR_PAGESTART, ///< per page start has been sent, wait for page start
    PRMGR_PAGE_QUERIED,      ///< got eng response
    PRMGR_PAGE_ACTIVE,       ///< sent page start to eng
    PRMGR_PAGE_STALLING,     ///< need user input, waiting for pages in engine to finish
    PRMGR_PAGE_STALLED,      ///< waiting for user input
}PAGE_STATE;

typedef enum
{
    ACTION_START,               ///< start stall check on new page
    ACTION_CONTINUE_YES,        ///< timeout or go button pressed on load tray
    ACTION_CONTINUE_NO,         ///< timeout or cancel button pressed on load tray, this becomes cancel job
    ACTION_CONTINUE_PAPERADDED, ///< paper added in some tray on printer
    ACTION_CONTINUE_OUTPUT_BIN_CLEARED, ///< paper cleared from some output tray/bin
    ACTION_NONE,                ///< nothing of interest happened
}STALLACTION;


/**
 * \brief Return a string with the symbolic name of the given enum value.
 *
 * \param actionEnum (STALLACTION) enum value to decode.
 *
 * \return const char* String name for enum value.
 *
 * This method is intended for use in debug printf calls.
 */
static const char* ToString_STALLACTION( STALLACTION actionEnum );


/**
 * \brief Return a string with the symbolic name of the given enum value.
 *
 * \param reasonEnum (STALLREASON) enum value to decode.
 *
 * \return const char* String name for enum value.
 *
 * This method is intended for use in debug printf calls.
 */
static const char* ToString_STALLREASON( STALLREASON reasonEnum );

//forced stall states
typedef enum
{
    FORCED_NONE = 0,         ///< no forced stall
    FORCED_WAITCANCEL,       ///< do forced stall, wait for page to cancel first
    FORCED_WAITPAGE,         ///< wait for page start for pre errors
    FORCED_ACTIVE,           ///< stall is active, wait for correct user input to clear
    FORCED_FAX,              ///< stall is active, wait for fax to finish
}FORCEDSTATE;
static const char* ToString_FORCEDSTATE( FORCEDSTATE enumValue );
/**
 * \brief Return a string with the symbolic name of the given enum value.
 *
 * \param actionEnum (STALLACTION) enum value to decode.
 *
 * \return const char* String name for enum value.
 *
 * This method is intended for use in debug printf calls.
 */



//structure that holds information on the current stall
typedef struct
{
    mediatype_t type;
    mediasize_t size;
    trayenum_t tray;
    trayenum_t outTray;
    STALLREASON reason;
    uint32_t stallCount;
}STALLINFO;

typedef struct
{
	uint32_t old_state;		// Check engine motor status (1: run, 0: stop)
	uint32_t new_state;
}MOTOR_RUN_STATE;

typedef struct
{
	uint32_t simplex;
	uint32_t duplex;
}PRINTEDPAGES;

typedef struct
{
	trayenum_t inputTray;
	mediasize_t size;
}MISMATCHINFO;

typedef struct
{
	uint32_t currentTonerPageCount;
	uint32_t currentTonerRemain;
	uint32_t currentCartridgeSN;
	char* currentCartridgeTLI;
	int* TonerJobByModes;
	int* TonerPrintedByCoverage;
	int cFreeMotorOn;
	int currentCartridgeCapacity;
}TONERINFO;

typedef struct
{
	int currentJobDrumLifePageCount;
	int machineDrumLifePageCount;
	PRINTEDPAGES printedPages;
	MOTOR_RUN_STATE motorRunState;
	mediasize_t machineTraySize[NUM_INPUTS];		// Engine Tray Paper size (by Sensors)
	MISMATCHINFO mismatchInfo;
	TONERINFO tonerInfo;

	uint32_t tray2VersionInfo;
	uint32_t tray3VersionInfo;
	uint32_t optionTrayNum;
	//uint16_t prevIgnoredPage;
	//uint32_t prevIgnoredJob;
	bool fDrumLowMode_3;
	bool fFirstMotorStop;
	uint32_t lastjobnum;
	uint8_t machineLocation;
}ENGINEINFO;


/**
 * \brief Structure that holds information on tray override.
 *
 * A tray override stays in effect during a print job. An override occurs when
 * the job couldn't be printed from the selected tray and the user selected an
 * override.
 */
typedef struct
{
    bool haveOverride;  ///< TRUE when an override is in effect
    mediatype_t requestedType;
    mediasize_t requestedSize;
    trayenum_t requestedTray;
    trayenum_t requestedOutTray;

    mediatype_t overrideType;
    mediasize_t overrideSize;
    trayenum_t overrideTray;
    trayenum_t overrideOutTray;
}OVERRIDEINFO;



/**
 * \brief Structure to hold information about a forced stall.
 *
 * A forced stall occurs when the print manager gets a status update from the
 * engine or video with the STATUS_USERINPUT_MASK set. This means it is an error
 * that requires user input before printing can resume. Because of auto tray
 * selection logic in the paper path stalled, the requested settings may not be
 * the settings actually used. Because of this I keep track of both the
 * requested settings and the settings that were used when the error occurred.
 */
typedef struct
{
    FORCEDSTATE state;             ///< forced stall state
    uint32_t statusCode;             ///< status code from engine or video that caused the force
    bool isUserAck;                ///< true if user must ack before clear, false if clear on online status
    bool isPreError;               ///< true if delay status until next page and has same settings
    trayenum_t requestedTray;        ///< tray originally requested in page
    trayenum_t requestedOutTray;     ///< output tray originally requested in page
    mediasize_t requestedMediaSize;  ///< media size requested in page
    mediatype_t requestedMediaType;  ///< media type requested in page
    trayenum_t errorTray;            ///< tray error occurred on
    mediasize_t errorMediaSize;      ///< media size error occurred on
    mediatype_t errorMediaType;      ///< media type error occurred on
    bool waitForCancel;            ///< true if need to wait for page to cancel before stalling
    void* pagePtr;                 ///< pointer to current printing page when forced error occurred
}FORCEDSTALL;

#define NUM_OF_JOBMODE				13		//12
#define NIP_TONER_CONSUMPTION		35

#define DRUM_LOW_MODE_1			45000		//45000		// 45000 is right value (Temp)
#define DRUM_LOW_MODE_2			48000		//48000
#define DRUM_LOW_MODE_3			49000		//49000
#define DRUM_END				50000		//50000
#define DRUM_LOW_1_CYCLE		50			//50

#define PRINTABLE_MAX_WIDTH		21600
#define PRINTABLE_MAX_LENGTH	35600

#define COPY_CORRECTION_FACTOR		1.15

uint32_t TonerJobMode[NUM_OF_JOBMODE] = { 0, 384, 227, 206, 193, 180, 174, 168, 161, 155, 149, 111, 100 };
// uint32_t TonerJobMode[NUM_OF_JOBMODE] = { 0, 270, 195, 164, 152, 139, 135, 132, 128, 125, 122, 109, 100 }; // for Low Duty

bool bSepCartridge = TRUE;
static bool firstStatusGet = TRUE;
DBINFO gDB_Info = {0,};
char serialnumber[11];

int PRNDUMPENABLE =0; /*Global variable to have DB value to store print in "/tmp" directory for debug reason*/
static int fuser_alpha=165;
static int fuser_beta=297;
static int fuser_gamma=1050;
static uint32_t fFuserLifePageCount=0;	// fFuserLifePageCount/100 = fuserInfo.FuserLifePageCount

//------------------------------------------------------------------------------
// OBJECT: LastPreviousNonDuplexInputTRAYENUM
//------------------------------------------------------------------------------

#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

/**
 * \brief Holds the non-duplexer, or "real" input tray last previously seen.
 */
static trayenum_t gLastPrevNonDuplexInputTRAYENUM = TRAY_ANY;

trayenum_t PrintMgrGetLastPreviousNonDuplexInputTRAYENUM(LASTPAGEINFO *lastPage)
{
    return gLastPrevNonDuplexInputTRAYENUM;
} // end PrintMgrGetLastPreviousNonDuplexInputTRAYENUM()

void PrintMgrSetLastPreviousNonDuplexInputTRAYENUM(trayenum_t trayEnum)
{
    if (trayEnum != EngGetAutoDuplexerInputTRAYENUM())
    {
        gLastPrevNonDuplexInputTRAYENUM = trayEnum;
    }
} // end PrintMgrSetLastPreviousNonDuplexInputTRAYENUM()

#endif // !defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

typedef struct PrintMgr_myDeviceStatus_s
{
    Response_Subject_T_BaseClassMembers;

    uint32_t status;

} PrintMgr_myDeviceStatus_t;

static PrintMgr_myDeviceStatus_t PrintMgr_myDeviceStatus;

typedef struct PrintMgr_myMediaStatus_s
{
    Response_Subject_T_BaseClassMembers;

    uint32_t status;

} PrintMgr_myMediaStatus_t;

static PrintMgr_myMediaStatus_t PrintMgr_myMediaStatus;

#define MargineOffset	50
/*
 * How I use the LINK_HEADER struct
 * Param  = page state
 * Param1 = HaveData
 * Data   = pointer to page
*/
#define HaveHdr 0
#define HaveData 1
/**
 * \brief Structure used to wrap PAGE_DATA objects in the print manager to queue them
 * and to track their state.
 */
typedef struct printmgr_page_s
{
    ATLISTENTRY node;       ///< node for linking pages
    PAGE_DATA* page_obj;    ///< pointer to system page object (PAGE_DATA)
    PAGE_STATE state;       ///< current state of the page
    bool have_data;         ///< indicator that data has been received for page, not just pre-start.
                            ///  TODO: Consider incorporating this info into a re-worked PAGE_STATE.
}printmgr_page_t;

static ATLISTENTRY printmgr_pending_list; ///< pending pages
static ATLISTENTRY printmgr_active_list;  ///< active pages - sent on to engine

//os stuff
#define PRINTMGRSTACK_SIZE  POSIX_MIN_STACK_SIZE
static ALIGN8 uint8_t PrintMgrStack[PRINTMGRSTACK_SIZE];
static pthread_t printmgr_thd_id;
#define NUMPMEVENTS 1000 + 20
static mqd_t PrintMgrQueue;

void *PrintMgrThread(void *unused);
//static TX_THREAD tPrintMgrThread;

//TX_THREAD *ResourceThreads[]={&tPrintMgrThread, NULL};  ///< List of threads that need to allocate.


// MESSAGE PrintMgrEvents[NUMPMEVENTS];

/**
 * \brief Global struct holding state of print manager.
 */
PrintMgrInfo gPrintMgrInfo;

/**
 * \brief Global struct holding engine specific values initialized by the
 *        engine code at startup and used by the print mgr, engine and video
 *        during runtime.
 */
bool gNewJob = TRUE;

/**
 * \brief When we are stalled for user input because don't have correct paper,
 *        the details are stored here
 */
STALLINFO gStallInfo = {MEDIATYPE_ANY, MEDIASIZE_ANY, TRAY_NONE, TRAY_NONE, STALL_NONE};

/**
 * \brief Holds the info from the last page we sent to the printer.
 */
LASTPAGEINFO gLastPage = {MEDIATYPE_ANY, MEDIASIZE_ANY, TRAY_NONE, TRAY_NONE, FALSE, FALSE};

//ENGINEINFO gEngineInfo = {0,0,0,0,{0,0},{0,0,0,0},0,0};
ENGINEINFO gEngineInfo = {0,};
extern uint32_t gCurTonerPageCount;
//------------------------------------------------------------------------------


/**
 * \brief Saves needed information when tray override is in effect.
 */
OVERRIDEINFO gOverrideInfo = {FALSE,
    MEDIATYPE_ANY, MEDIASIZE_ANY, TRAY_NONE, TRAY_NONE,
    MEDIATYPE_ANY, MEDIASIZE_ANY, TRAY_NONE, TRAY_NONE};

/**
 * \brief Holds state info for forced stalls.
 *
 * These are caused when some transitory status event happens in the engine or
 * video that the user needs to respond to. Since the status is transitory the
 * engine and video won't keep the state so the PM has to hold it and force it
 * to be the current status until cleared.
 */
FORCEDSTALL gForcedStallInfo;

// static TX_TIMER StartTimer;
// static uint32_t TimeRdy=1;

void PrintMgrPrintPages(MESSAGE* msg);
void PrintMgrAckJobMgrCancelDone( void );
void PrintMgrAddToPendingList(ADDTYPE type, PAGE_DATA* pPageObject);
STALLREASON PrintMgrDoStallCheck(PAGE_DATA* pPageObject, STALLACTION action);
trayenum_t GetInputUpper(const printer_setup_t* pSetup );
void PrintMgrStartStall(void);
void PrintMgrClearStall(void);
STALLREASON PrintMgrHandleForcedStall(MESSAGE* msg);
bool PrintMgrCheckForForcedStall(uint32_t* statusCode);
void PrintMgrClearForcedStall();
void PrintMgrSendMsg(MODULE_ID id, AGMSG msgType, uint32_t param1, uint32_t param2, void* param3);


void PrintMgrEngineStatus(MESSAGE* msg);

void PrintMgrDetectManualTray(const printer_setup_t* pSetup, trayenum_t* mfTray, bool* isPriority,
                              bool* isMPTray);
void PrintMgrDetectManualDuplexTray( const printer_setup_t* pSetup, trayenum_t* duplexTray );
void PrintMgrStartTimedOverride(uint32_t timedId);
void PrintMgrProcessTimedOverride(uint32_t timedId);
void PrintMgrStopTimedOverride(uint32_t timedId);
bool PrintMgrPageActive(void);
static void TempInitEngineConfig(MESSAGE* msg);
static void PrintMgrFlushActiveList(void);
static void PrintMgrFlushPendingList(void);
static void PrintMgrHandlePageDeliveryDone(MESSAGE* msg);
void PrintMgrHandlePrinterOptions(void);
void printmgr_dbg_init();
void get_DB_values_init();
void ResetDrumCountToDB(void);
void UpdateJobByModesToDB(uint32_t PrintedPages);
void UpdatePrintedPageCountToDB(int DrumLifeRemainPercentage, int MotorRunTime, int dlpc);
static void UpdateFuserCountToDB(int FuserMotorRunNormal, int FuserMotorRunLow);
static void CheckFuserUpdate(void);
static void ResetFuserCountToDB(void);

static uint32_t pagesPrintedCounter = 0;
static uint32_t pagesPrintedDuplexCounter = 0;
static uint32_t pagesHostPrintCounter = 0;
static uint32_t pagesReportPrintCounter = 0;
static uint32_t pagesUSBPrintCounter = 0;
static uint32_t pagesFaxPrintCounter = 0;
static uint32_t pagesCopyPrintCounter = 0;
uint32_t pagesFaxAppPagePrinted = 0;
DOCTYPE gFaxPrintJobDocType = e_NONE;

#if defined(HAVE_DBG_PRINTF) || defined(HAVE_CMD)
bool gActivatePrintMgrPrintLists = FALSE;
static void PageList_Print(void);
#else  // !ifdef HAVE_DBG_PRINTF
#define PageList_Print(void)
#endif // !ifdef HAVE_DBG_PRINTF



/**
 * \brief Media Prompt Subject.  Used to notify user interface when media
 * needs attention.
 */

typedef struct printmgr_media_prompt_subject_s
{
    Response_Subject_T_BaseClassMembers;  ///< Base class members for response
    uint32_t status;                      ///< The status
    printmgr_media_prompt_info_t media_prompt_info;  ///< Information about the tray causing the stall
} printmgr_media_prompt_subject_t;

static printmgr_media_prompt_subject_t printmgr_media_prompt;


void UserAck(Subject_t *s)
{
    DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: UserInput received %s",__func__);


    printmgr_media_prompt_subject_t * media_info = (printmgr_media_prompt_subject_t *)s;

    //printmgr_media_prompt_get_media_info(s, media_info );
    //msg->param1 == PARAM_USER_OK
    if( media_info )
    {
         DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: media prompt status = %x\n", media_info->status);
    }
    PrintMgrSendMsg(PRINTMGRID, MSG_USER_INPUT, PARAM_USER_OK, 0, NULL);
    //nextMsg.msgType = MSG_USER_ERR_RECOVERY;
    //SYMsgSend(ENGINEID, &nextMsg);
    //PrintMgrSendMsg(ENGINEID, MSG_USER_ERR_RECOVERY, 0, 0, NULL);

}

void UserCancel(Subject_t *s)
{
    MESSAGE cancelMsg;

    DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: UserInput received %s\n",__func__);


    printmgr_media_prompt_subject_t * media_info = (printmgr_media_prompt_subject_t *)s;

    //printmgr_media_prompt_get_media_info(s, media_info );
    //msg->param1 == PARAM_USER_OK
    if( media_info )
    {
         DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: media prompt status = %x\n", media_info->status);
    }

    cancelMsg.msgType = MSG_CANCELJOB;
    cancelMsg.param1 = SYS_REQUEST;
    cancelMsg.param2 = SJMID;
    cancelMsg.param3 = (void*)e_Printer;
    SYMsgSendNoWait(SJMID, &cancelMsg);

/*    cancelMsg.msgType = MSG_CANCELJOB;
    cancelMsg.param1 = 0;
    cancelMsg.param2 = 0;
    cancelMsg.param3 = NULL;
    SYMsgSendNoWait(JOBMGRID, &cancelMsg);
*/
}

static void printmgr_media_prompt_constructor(void)
{
    memset(&printmgr_media_prompt, 0, sizeof(printmgr_media_prompt));
    printmgr_media_prompt.subject_observer_oid_key = PrintMgr_myMediaPromptID;
    printmgr_media_prompt.try_again = NULL;
    MInitList(&printmgr_media_prompt.observers);
}

static void printmgr_media_prompt_notify(uint32_t statusCode, FORCEDSTALL *forcedStallInfo, TRY_AGAIN CallBack)
{
    printmgr_media_prompt_subject_t *media_prompt;

    DBG_PRINTF(LOG_WARNING|DBG_PRINTMGR,"PM: %s: status 0x%08x\n\tin_tray = %d, out_tray = %d, mediasize = %d, mediatype = %d\n",
               __func__, statusCode, forcedStallInfo->errorTray, forcedStallInfo->requestedOutTray, forcedStallInfo->errorMediaSize, forcedStallInfo->errorMediaType);

    printmgr_media_prompt.status = statusCode;
    printmgr_media_prompt.media_prompt_info.prompt_input_tray = forcedStallInfo->errorTray;
    printmgr_media_prompt.media_prompt_info.prompt_output_tray = forcedStallInfo->requestedOutTray;
    printmgr_media_prompt.media_prompt_info.prompt_mediasize = forcedStallInfo->errorMediaSize;
    printmgr_media_prompt.media_prompt_info.prompt_mediatype = forcedStallInfo->errorMediaType;
    printmgr_media_prompt.try_again = CallBack;
    printmgr_media_prompt.cancel_button = UserCancel;
    media_prompt = &printmgr_media_prompt;
    NOTIFY_OBSERVERS(media_prompt, observers);
}

static void printmgr_paper_out_prompt_notify(uint32_t statusCode, STALLINFO *StallInfo, TRY_AGAIN CallBack)
{
    printmgr_media_prompt_subject_t *media_prompt;

    DBG_PRINTF(LOG_WARNING|DBG_PRINTMGR,"PM: %s: status 0x%08x\n\tin_tray = %d, out_tray = %d, mediasize = %d, mediatype = %d\n",
               __func__, statusCode, StallInfo->tray, StallInfo->outTray, StallInfo->size, StallInfo->type);

    printmgr_media_prompt.status = statusCode;
    printmgr_media_prompt.media_prompt_info.prompt_input_tray = StallInfo->tray;
    printmgr_media_prompt.media_prompt_info.prompt_output_tray = StallInfo->outTray;
    printmgr_media_prompt.media_prompt_info.prompt_mediasize = StallInfo->size;
    printmgr_media_prompt.media_prompt_info.prompt_mediatype = StallInfo->type;
    printmgr_media_prompt.try_again = CallBack;
    printmgr_media_prompt.cancel_button = UserCancel;
    media_prompt = &printmgr_media_prompt;
    NOTIFY_OBSERVERS(media_prompt, observers);
}

void printmgr_media_prompt_get_media_info(Subject_t *s, printmgr_media_prompt_info_t *media_info)
{
    ASSERT(media_info != NULL);
    memcpy(media_info, &printmgr_media_prompt.media_prompt_info, sizeof(printmgr_media_prompt_info_t));
}

uint32_t printmgr_media_prompt_get_status_event(Subject_t *s)
{
    return printmgr_media_prompt.status;
}

TRY_AGAIN printmgr_media_prompt_get_retry_callback(Subject_t *o)
{
    return printmgr_media_prompt.try_again;
}


USER_CANCEL printmgr_media_prompt_get_cancel_button_callback(Subject_t *o)
{
    return printmgr_media_prompt.cancel_button;
}

void printmgr_media_prompt_AttachObserver(Observer_t *o)
{
    ATTACH_OBSERVER(printmgr_media_prompt.observers, o);
}

void printmgr_media_prompt_DetachObserver(Observer_t *o)
{
    DETACH_OBSERVER(printmgr_media_prompt.observers, o);
}
//////////////////////////////////////////////////////////////////////
// END Media Prompt Subject
//////////////////////////////////////////////////////////////////////

/**
 * \brief PrintMgr Device Status Subject.  Global notifier of all
 * print manager events
 */
static void PrintMgr_DeviceStatus_constructor(void)
{
    PrintMgr_myDeviceStatus.status = 0;
    PrintMgr_myDeviceStatus.subject_observer_oid_key = PrintMgr_myDeviceStatus_ID;
    MInitList(&PrintMgr_myDeviceStatus.observers);
}

static void PrintMgr_Notify_devicestatus( uint32_t statusCode, TRY_AGAIN Callback )
{
    PrintMgr_myDeviceStatus_t * my_status;

    PrintMgr_myDeviceStatus.status = statusCode;
    PrintMgr_myDeviceStatus.try_again = Callback;
    PrintMgr_myDeviceStatus.cancel_button = UserCancel;
    my_status = &PrintMgr_myDeviceStatus;
    NOTIFY_OBSERVERS( my_status, observers );
}

uint32_t PrintMgr_Get_devicestatus( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintMgr_myDeviceStatus_t,
                                subject_observer_oid_key, PrintMgr_myDeviceStatus_ID) );

    // message causes notify causes get so no lock needed.
    return PrintMgr_myDeviceStatus.status ;
}

TRY_AGAIN PrintMgr_Get_retry_callback(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintMgr_myDeviceStatus_t,
                                subject_observer_oid_key, PrintMgr_myDeviceStatus_ID) );
    return PrintMgr_myDeviceStatus.try_again;
}

USER_CANCEL PrintMgr_Get_cancel_button_callback(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintMgr_myDeviceStatus_t,
                                subject_observer_oid_key, PrintMgr_myDeviceStatus_ID) );
    return PrintMgr_myDeviceStatus.cancel_button;
}
void PrintMgr_AttachObserver_devicestatus(Observer_t *o)
{
    ATTACH_OBSERVER( PrintMgr_myDeviceStatus.observers, o );
}

void PrintMgr_DetachObserver_devicestatus(Observer_t *o)
{
    DETACH_OBSERVER( PrintMgr_myDeviceStatus.observers, o );
}

//////////////////////////////////////////////////////////////////////
// END PrintMgr Device Status Subject
//////////////////////////////////////////////////////////////////////

/**
 * \brief PrintMgr  Media Status Subject.  Global notifier of media
 * status change events
 */
static void PrintMgr_MediaStatus_constructor(void)
{
    PrintMgr_myMediaStatus.status = 0;
    PrintMgr_myMediaStatus.subject_observer_oid_key = PrintMgr_myMediaStatus_ID;
    MInitList(&PrintMgr_myMediaStatus.observers);
}

static void PrintMgr_Notify_MediaStatus( uint32_t statusCode, TRY_AGAIN Callback )
{
    PrintMgr_myMediaStatus_t * my_status;

    PrintMgr_myMediaStatus.status = statusCode;
    PrintMgr_myMediaStatus.try_again = Callback;
    PrintMgr_myMediaStatus.cancel_button = UserCancel;
    my_status = &PrintMgr_myMediaStatus;
    NOTIFY_OBSERVERS( my_status, observers );
}

uint32_t PrintMgr_Get_MediaStatus( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintMgr_myDeviceStatus_t,
                                subject_observer_oid_key, PrintMgr_myMediaStatus_ID) );

    // message causes notify causes get so no lock needed.
    return PrintMgr_myMediaStatus.status ;
}

TRY_AGAIN PrintMgr_Get_MediaStatus_RetryCallback(Subject_t * s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintMgr_myDeviceStatus_t,
                                subject_observer_oid_key, PrintMgr_myMediaStatus_ID) );
    return PrintMgr_myMediaStatus.try_again;
}

USER_CANCEL PrintMgr_Get_MediaStatus_CancelButtonCallback(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintMgr_myDeviceStatus_t,
                                subject_observer_oid_key, PrintMgr_myMediaStatus_ID) );
    return PrintMgr_myMediaStatus.cancel_button;
}
void PrintMgr_AttachObserver_MediaStatus(Observer_t *o)
{
    ATTACH_OBSERVER( PrintMgr_myMediaStatus.observers, o );
}

void PrintMgr_DetachObserver_MediaStatus(Observer_t *o)
{
    DETACH_OBSERVER( PrintMgr_myMediaStatus.observers, o );
}

//////////////////////////////////////////////////////////////////////
// END PrintMgr Media Status Subject
//////////////////////////////////////////////////////////////////////

#if 0
#ifndef START_TIME
#define START_TIME 0
#endif

#ifndef INIT_TIME
#define INIT_TIME 0
#endif

void TimeUpdate(ULONG Temp)
{
    LINK_MEMBER *TempMember;
    MESSAGE msg;
    TempMember = agGetHead(&gPrintMgrActiveList);
    while(TempMember != NULL)
    {
        if(TempMember->Param1 & 0x80000000)
        {
                TempMember->Param1 &= 0x7fffffff;
                msg.msgType = MSG_PAGESTART;
                msg.param3 = TempMember->Data;
                // Must use the message send with zero wait since we are in an int routine.
                // The only way this could fail is if the queue is full.  That should
                // never happen.
                //
                SYMsgSendWait(ENGINEID, &msg, 0);

                TimeRdy = 0;    // set up the timer for page pacing.
                tx_timer_deactivate(&StartTimer);
                tx_timer_change(&StartTimer,START_TIME,0);  // one shot timer
                tx_timer_activate(&StartTimer);
                return;
        }
        TempMember = TempMember->NextLink;
    }
    TimeRdy = 1;    // reset the timer.
} // end TimeUpdate()
#endif

/// Creates print manager thread.
void PrintMgrInit(void)
{
	if (IS_PINETREE || IS_PINETREEL){ fuser_alpha=165; fuser_beta=255; fuser_gamma= 860; }
	else if (IS_ABBA2H || IS_OAK2H)	{ fuser_alpha=152; fuser_beta=227; fuser_gamma= 840; }
	else if (IS_OAK2M)				{ fuser_alpha=152; fuser_beta=199; fuser_gamma= 840; }
	else 							{ fuser_alpha=165; fuser_beta=292; fuser_gamma=1010; }	// KARAS, PINETREES
	
    PrintMgr_DeviceStatus_constructor();

    PrintMgr_MediaStatus_constructor();

    printmgr_media_prompt_constructor();

    ATInitList(&printmgr_pending_list);
    ATInitList(&printmgr_active_list);

    posix_create_MESSAGE_queue( &PrintMgrQueue, "/PrintMgrQueue", NUMPMEVENTS );

    gPrintMgrInfo.sysOnline =  FALSE;

    gPrintMgrInfo.engOnline =  FALSE;
    gPrintMgrInfo.numCredits = 0;
    gPrintMgrInfo.maxCredits = 0;
    gPrintMgrInfo.status =     0;
    gPrintMgrInfo.mode =       PM_NORMAL;
    gPrintMgrInfo.statReqCnt = 0;
    gPrintMgrInfo.receivedFirstManualDuplexSecondSide = 0;
    gPrintMgrInfo.pageCancelState = PMCANCEL_NONE;
    gPrintMgrInfo.cancelType = (AGMSG)0;
    gPrintMgrInfo.cancelPtr  = NULL;

    router_register_queue(PRINTMGRID, PrintMgrQueue);   // Register a message queue.

    PrintMgrClearForcedStall();

    gStallInfo.stallCount = 0;

    printmgr_dbg_init();

    get_DB_values_init();

    gEngineInfo.fFirstMotorStop = TRUE;
    gEngineInfo.motorRunState.old_state = 1;
    gEngineInfo.motorRunState.new_state = 0;
    //gDB_Info.DrumLifePageCount = db_get_var_int("INST_DRUMCOUNT","DrumLifePageCount",&db_retval);
	//DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : db_DLPC = %d ##\n",gDB_Info.DrumLifePageCount);
	//gEngineInfo.machineDrumLifePageCount = gDB_Info.DrumLifePageCount * 100;
	//gDB_Info.traySettingPriority = db_get_var_int("CONF_PRINTSET", "SettingPriority", &db_retval);		// 0 : Printer Setting, 1 : Driver Setting
	printvar_set_trayconfig_get_from_DB();

    posix_create_thread(&printmgr_thd_id, PrintMgrThread, 0, "print_page_mgr", PrintMgrStack, PRINTMGRSTACK_SIZE, POSIX_THR_PRI_NORMAL);
}

void *PrintMgrThread(void *unused)
{
    MESSAGE nextMsg;
    MESSAGE outMsg;
    PAGE_DATA* curPageData = NULL;
    int iRET = 0; UNUSED_VAR(iRET); // error handling return ignored on OS call.
    //uint32_t* dot_count = NULL;
    uint32_t dot_count = 0;
    int* stMotorLow = {0,};

    //wait for system init to finish
    SysWaitForInit();

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: starting\n");

    //first thing we do when we come up is send a status
    //req to the engine to force it to give us the
    //latest status
    PrintMgrSendMsg(ENGINEID, MSG_STATUSREQUEST, 0, 0, NULL);

    while(1)
    {
        iRET = mq_receive(PrintMgrQueue, (char*)&nextMsg, sizeof(MESSAGE), 0);
        XASSERT( iRET == sizeof(MESSAGE), thread_errno);
        DBG_PRINTF_INFO(" got message '%s'=0x%X=%d; param1=0x%X param2=0x%X param3=0x%08X\n",
			DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType, nextMsg.msgType,
			nextMsg.param1, nextMsg.param2, nextMsg.param3 );
      //DPRINTF(PMDEBUG, ("PM: PrintMgrQueue: capacity=%d enqueued=%d%s\n",
      //                  PrintMgrQueue.tx_queue_capacity, PrintMgrQueue.tx_queue_enqueued,
      //                  ( ( PrintMgrQueue.tx_queue_capacity == PrintMgrQueue.tx_queue_enqueued )
      //                    ? " *** QUEUE FULL! DEADLOCK LIKELY! ***" : "" ) ));

#if 0 //For Message Queue Debug
		int mq_retcode;
		struct mq_attr attr;
		mq_retcode = (int)mq_getattr( PrintMgrQueue, &attr );

 		 DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR," PrintMgrQueue: --->>> MsgType '%s' ATTR: MaxMsg=%ld, CurMSGS = %ld \n", DecodeSysMsgEnum(nextMsg.msgType), attr.mq_maxmsg,  attr.mq_curmsgs);
#endif

        switch(nextMsg.msgType)
        {
            case MSG_JOBSTART:
                // Notification from job manager that a new job has started.
                // Use this to manage "pre-warmup" commands for the engine.
                gNewJob = TRUE;
                gPrintMgrInfo.receivedFirstManualDuplexSecondSide = 0;

                DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"PM: JOBSTART Job %d\n", ((JOB_DATA*)nextMsg.param3)->JobNum);

                // PrintMgrSendMsg(ENGINEID, MSG_JOBSTART, 0, 0, NULL);
                SYMsgSend(ENGINEID, &nextMsg);
                PageList_Print();
                break;

            case MSG_JOBEND:
                // JobMgr finishes all pages of this job before sending MSG_JOBEND for this job.
                // Furthermore, the JOB_DATA has been deallocated by the time we get this message.
                gLastPage.type = MEDIATYPE_UNKNOWN;
                gLastPage.size = MEDIASIZE_UNKNOWN;
                gLastPage.tray = TRAY_NONE;
                gLastPage.outTray = TRAY_NONE;
                gLastPage.isManualFeed = FALSE;
                gLastPage.isSingleFeed = FALSE;
                gOverrideInfo.haveOverride = FALSE;
                PrintMgrSetLastPreviousNonDuplexInputTRAYENUM(TRAY_ANY);
                // param1 is the job id of the job that just finished, JobMgr sends this
                // with MSG_JOBEND
                // param2 is 0
                // param3 is NULL!
                DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"PM: EOJ Job %d\n", (nextMsg.param1));
                PageList_Print();
                // The engine needs to know this to clear up things.
                SYMsgSend(ENGINEID, &nextMsg);
                PrintMgr_Notify_devicestatus(STATUS_ONLINE, UserAck);
                //UpdatePrintedPageCountToDB();  //Update DB for Printed pages
                break;

            case MSG_PAGEPRESTART:
                curPageData = (PAGE_DATA *)nextMsg.param3;
                ASSERT(curPageData != NULL);
                DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: MSG_PAGEPRESTART received for pg 0x%08x\n", curPageData);
                PrintMgrSendMsg(ENGINEID, MSG_PAGEPRESTART, 0, 0, curPageData);
                break; // Ignore for now
#if 0
                // agprintf("PM: enter page PRE start 0x%x\n", nextMsg.param3);
                curPageData = (PAGE_DATA *)nextMsg.param3;
                ASSERT(curPageData != NULL);
                ASSERT(IS_PAGE_DATA(curPageData));

                if ( curPageData->DocType == e_CALPAGE)
                {
                    // agprintf("PM: ****** IGNORE prestart message for CAL job ****** \n");
                    break;
                }

                // If this isn't a new job, simply exit.
                if (gNewJob != TRUE)
                {
                    // agprintf("PM: **** exiting page PRE start\n");
                    break;
                }

                gNewJob = FALSE;

                DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: got pre page 0x%08X\n", curPageData);

                PrintMgrAddToPendingList(ADD_PAGEPRESTART, curPageData);

                //check on engine status
                PrintMgrEngineStatus(&nextMsg);

                //this function has the logic to see if the page should be printed
                //based on our state
                PrintMgrPrintPages(&nextMsg);
                // agprintf("PM: PRE-RESERVATON exit page PRE start\n");
                break;
#endif

        case MSG_PAGEPRESTART_RSP:
                curPageData = (PAGE_DATA*) nextMsg.param3;
                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: Got response from engine, prestart completed. page %x\n", curPageData);
                if( curPageData->page_state == PAGE_STATE_ENG_PRESTART)
                {
                    gPrintMgrInfo.numCredits--;
                }
                break;

        case MSG_SUPPLIES_PAGE_DONE:
                //dot_count = (uint32_t*) nextMsg.param3;
                //DBG_PRINTF(LOG_ERR|DBG_PRINTMGR, "PM: dot_count %d\n", *dot_count);
                curPageData = (PAGE_DATA *)nextMsg.param3;
                if(curPageData->pMyJob->JobInfo == COPYJOB)
                {
					DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: Copy Job : Pre Dot Check %d \n", nextMsg.param2);
					nextMsg.param2 = nextMsg.param2 * COPY_CORRECTION_FACTOR;
				}
                dot_count = nextMsg.param2;
                DBG_PRINTF(LOG_ERR|DBG_PRINTMGR, "PM: dot_count %d <page=%p> (Job Info %d)\n", dot_count, curPageData, curPageData->pMyJob->JobInfo);

                SYMsgSend(ENGINEID, &nextMsg);
                break;

        case MSG_PAGESTART:
                curPageData = (PAGE_DATA*) nextMsg.param3;
                if ( gPrintMgrInfo.pageCancelState != PMCANCEL_NONE )
                {
                    // If we are doing some sort of cancel, we need to ignore
                    // any PAGESTART messages that are queued up. JobMgr will
                    // re-send them to us later, if and when appropriate.
                    //
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: gPrintMgrInfo.pageCancelState=%s; ignore MSG_PAGESTART. page %d of job %d @ 0x%08X\n",
                                      ToString_PAGECANCEL(gPrintMgrInfo.pageCancelState), curPageData->PageCount, curPageData->pMyJob->JobNum, curPageData );
                    //gEngineInfo.prevIgnoredPage = curPageData->PageCount;
                    //gEngineInfo.prevIgnoredJob = curPageData->pMyJob->JobNum;
                    PageList_Print();
                    break;
                }

                //if ( (gPrintMgrInfo.pageCancelState == PMCANCEL_NONE) && (gEngineInfo.prevIgnoredPage != 0) && (gEngineInfo.prevIgnoredJob != 0) )
                //{
					//DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: gPrintMgrInfo.pageCancelState=%s; page %d of job %d @ %lx (pre page %d of job %d)\n",
                                      //ToString_PAGECANCEL(gPrintMgrInfo.pageCancelState),
                                      //curPageData->PageCount, curPageData->pMyJob->JobNum, curPageData,
                                      //gEngineInfo.prevIgnoredPage, gEngineInfo.prevIgnoredJob);

					//if ( ( (curPageData->PageCount == gEngineInfo.prevIgnoredPage + 1) && (curPageData->pMyJob->JobNum == gEngineInfo.prevIgnoredJob) ) ||
					     //( (curPageData->PageCount == 1) && (curPageData->pMyJob->JobNum == gEngineInfo.prevIgnoredJob + 1) )
					   //)
					//{
						//DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"ignore MSG_PAGESTART\n");
						//break;
					//}
				//}

				//gEngineInfo.prevIgnoredPage = 0;
                //gEngineInfo.prevIgnoredJob = 0;

                //curPageData = (PAGE_DATA*) nextMsg.param3;
                ASSERT(curPageData != NULL);
                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: got page %d of job %d @ %lx (%d)\n", curPageData->PageCount, curPageData->pMyJob->JobNum, curPageData, curPageData->PageSent);

                // if the application did not provide a media size we need to look it up based on the x and y dimensions
                if(curPageData->PAPER == MEDIASIZE_ANY)
                {
                    mediasize_t media_size;
                    if( paper_get_media_size_enum( curPageData->image_output_info.image_width_in_pixels, curPageData->image_output_info.image_height_in_lines, PAPER_UNITS_600DPI, &media_size) == OK )
                    {
                        curPageData->PAPER = media_size;
                    }
                }

                PrintMgrAddToPendingList(ADD_PAGESTART, curPageData);

                //check on engine status
                PrintMgrEngineStatus(&nextMsg);

                //this function has the logic to see if the page should be printed
                //based on our state
                PrintMgrPrintPages(&nextMsg);
                PageList_Print();
                break;

            case MSG_CANCELJOB:
            case MSG_CANCELPAGE:
            case MSG_RESTARTPAGE:
            case MSG_RETRYPAGE:
                DPRINTF(PMDEBUG, ("PM: got %s param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                  DecodeSysMsgEnum(nextMsg.msgType),
                                  nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
                                  nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
                                  nextMsg.param3));

                if(nextMsg.param1 == LOCAL_START)
                {
                    //start the cancel state machine, cancel engine first
                    gPrintMgrInfo.pageCancelState = PMCANCEL_ENGINE;
                    if(gPrintMgrInfo.cancelType != MSG_CANCELJOB)
                    {
						gPrintMgrInfo.cancelType = nextMsg.msgType;
					}
                    gPrintMgrInfo.cancelPtr  = nextMsg.param3;

                    //On page cancel clear the number of credits if the engine
                    //is offline so we won't send any more pages until get an online
                    //and a credit update. If it is online then the page is being
                    //canceled for some other reason than an engine error (user cancel probably)
                    //so keep the credits
                    if(gPrintMgrInfo.engOnline == FALSE)
                        gPrintMgrInfo.numCredits = 0;

                    //send cancel onto engine first
                    //when it is done we send cancel to video
                    DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: cancel requested; cancel engine pages.\n");
                    outMsg.msgType = MSG_CANCELPAGE;
                    //
                    // Make sure we pass on the correct cancel.
                    //
                    if(nextMsg.msgType == MSG_CANCELJOB)
                    {
                        outMsg.msgType = MSG_CANCELJOB;
                    }
                    outMsg.param1 = LOCAL_START;
                    outMsg.param2 = PRINTMGRID;
                    SYMsgSend(ENGINEID, &outMsg);

                    /**
                     * Clear out stall variables, as appropriate.
                     *
                     * - MSG_CANCELJOB:
                     *   - Clear ANY stall, including FORCED. Just reset the
                     *     forced stall indicator, and clear the other stall
                     *     variables below.
                     */
                    if ( nextMsg.msgType == MSG_CANCELJOB )
                    {
                        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: Clear Stall: gForcedStallInfo.state=%s -> FORCED_NONE\n",
                                              ToString_FORCEDSTATE(gForcedStallInfo.state) );
                        PrintMgrClearForcedStall();
                    }

                    /**
                     * - MSG_CANCELPAGE, MSG_RESTARTPAGE, MSG_RETRYPAGE:
                     *   - Clear only un-forced stalls. If a forced stall is
                     *     active at a page cancel, that forced stall has to
                     *     get cleared by other means.
                     */
                    if ( gForcedStallInfo.state != FORCED_ACTIVE )
                    {

                        if( gStallInfo.reason == STALL_MANUALDUPLEX )
                        {
                            // if we get a cancel and we are waiting for manual duplex input we have to reset that
                            // flag so that next attempt at printing this page will wait accordingly.
                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: we got cancel message while stalling for manual duplex !!!!!!!\n");
                            gPrintMgrInfo.receivedFirstManualDuplexSecondSide = 0;
                        }


                        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: Clear Stall: gStallInfo.reason=%s -> STALL_NONE\n",
                                              ToString_STALLREASON(gStallInfo.reason) );
                        PrintMgrClearStall();
                        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: Clear Stall: gPrintMgrInfo.mode=%s -> PM_NORMAL\n",
                                              ToString_PRINTMODE(gPrintMgrInfo.mode) );
                        gPrintMgrInfo.mode = PM_NORMAL;
                    }
                }
                else if(nextMsg.param1 == LOCAL_ACK)
                {
                    if(gPrintMgrInfo.pageCancelState == PMCANCEL_ENGINE)
                    {
                        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: eng page cancel done; now cancel video.\n");



                        // The calls to the flush routines were relocated here from their
                        // original location above in the LOCAL_START processing.  They had to be
                        // relocated here to accommodate the timing of engines with intermediate transfer
                        // belts.  On these engines a valid page is still being transfered from the
                        // belt to the paper when the cancel comes in.  If we immediately clear the
                        // lists at the start of the cancel then we discard the page done on the
                        // page being transfered and when the cancel condition is cleared this causes
                        // us to reprint the last page.
                        DPRINTF(PMDEBUGSOFT, ("PM: flush active and pending lists\n"));
                        PrintMgrFlushActiveList();
                        PrintMgrFlushPendingList();


                        if ( gPrintMgrInfo.engOnline )
                        {
                            //gPrintMgrInfo.pageCancelState = PMCANCEL_NONE;
                            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrAckJobMgrCancelDone!!!\n");
                            PrintMgrAckJobMgrCancelDone();
                        }
                        else // Engine offline - don't have JobMgr resend pages quite yet...
                        {
                            gPrintMgrInfo.pageCancelState = PMCANCEL_WAIT_FOR_ENGINE_ONLINE;
                        }

                        // REMOVING THIS DUE TO THE FACT THE WITH NEW STATUS OBSERVERS WE WERE GETTING TOO MANY
                        // SYSTEM ONLINE EVENTS AND CAUSING ENGINE CREDITS TO BE UPDATED TO OFTEN.

                        /*
                        //if both engine and video are online
                        if(gPrintMgrInfo.sysOnline && gPrintMgrInfo.engOnline)
                        {
                            //tell engine to clear any error states
                            //this will cause engine to send us a
                            //credit update
                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR, "MSG_ERRCLEAR from LOCAL_ACK\n");

                            outMsg.msgType = MSG_ERRCLEAR;
                            SYMsgSend(ENGINEID, &outMsg);
                        }

                        */
                        PrintMgrSendMsg(ENGINEID, MSG_STATUSREQUEST, 0, 0, 0);
                    }
                    else
                    {
                        DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: Invalid cancel state! %s\n", ToString_PAGECANCEL(gPrintMgrInfo.pageCancelState) );
                        // Print Manager (PM) receiveing more than one MSG_STATUSUPDATE/ STATUS_DOOROPEN_FRONT
                        // status update message when front door opens and Tray open.(require more check)
                        // So Print JobManager (JM) getting more then one MSG_RESTARTPAGE message from
                        // print_status_updater(Observer_t *observer, Subject_t *s) callback fucntion.
                        // JM sends MSG_CANCELPAGE to PM and PM sends MSG_CANCELPAGE to Engine Manager(EM). EM send reply MSG_CANCELPAGE with LOCAL_ACK.
                        // Require proper handling for message MSG_RESTARTPAGE in jobMgr.c: print_status_updater function.

                        // WARNING: **** Tempary solution: commenting //ASSERT( false );

                        //ASSERT( false );
                    }

                    //for page processing force all 4 of these messages to a single
                    //message. This simplifies the logic and all 4 do the same thing here
                    DBG_PRINTF(LOG_ERR,"%s MSG_CANCELPAGE \n",__func__);
                    nextMsg.msgType = MSG_CANCELPAGE;
                    PrintMgrPrintPages(&nextMsg);
                }
                else
                {
                    DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: invalid cancel param1: %d, param2: %d\n",nextMsg.param1, nextMsg.param2);
                }
                PageList_Print();
                break;

            case MSG_PAGEDONE:
                ASSERT(nextMsg.param3 != 0);
                PrintMgrHandlePageDeliveryDone(&nextMsg);
                PrintMgrPrintPages(&nextMsg);
                PageList_Print();
                DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: Got Page done %x\n",nextMsg.param3);
                if (nextMsg.param2 == 2)	gEngineInfo.printedPages.duplex = gEngineInfo.printedPages.duplex + 2;
                else 						gEngineInfo.printedPages.simplex++;
                break;

            case MSG_CREDITUPDATE:
                {
                    //engine is telling us the max pages he can
                    //currently take. Credits are decremented
                    //when we send a page start and incremented
                    //on page done. on error we clear the
                    //credits and wait for a new update when
                    //the error is cleared
                    gPrintMgrInfo.numCredits = nextMsg.param1;
                    gPrintMgrInfo.maxCredits = nextMsg.param1;

                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: Credits %d\n", nextMsg.param1);

                    //print any pending pages
                    //this function has the logic to see if the page should be printed
                    //based on our state
                    PrintMgrPrintPages(&nextMsg);


                }
                PageList_Print();
                break;

            case MSG_PRINTONLINE:
                {
                    uint32_t status = nextMsg.param2;

                    //we get this message from the datastore when the online variable changes
                    if (status == OFFLINE)
                    {
                        //system went offline, this puts us in either
                        //PM_ENGREADY if we were in PM_READY or PM_OFFLINE
                        //if were in PM_SYSREADY
                        gPrintMgrInfo.sysOnline = FALSE;

                        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: sys offline\n");

                    }
                    else
                    {
                        //system went online.
                        //This puts us in PM_SYSREADY if were in PM_OFFLINE
                        //or PM_READY if in PM_ENGREADY
                        if(gPrintMgrInfo.sysOnline == FALSE ) // only do this if the state has changed
                        {

                            gPrintMgrInfo.sysOnline = TRUE;
                            DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: sys online\n");

                            //system is now online. if engine is online resume
                            //printing, else query the engine to see what is up
                            if (gPrintMgrInfo.engOnline == TRUE)
                            {
                                //tell engine to clear any error states
                                //this will cause engine to send us a
                                //credit update
                                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR, "MSG_ERRCLEAR from SYS ONLINE\n");
                                //outMsg.msgType = MSG_ERRCLEAR;
                                //SYMsgSend(ENGINEID, &outMsg);

                                PrintMgrPrintPages(&nextMsg);
                            }
                            else
                            {
                                //send query to engine, this will cause it to send
                                //a MSG_STATUSUPDATE message
                                nextMsg.msgType = MSG_STATUSREQUEST;
                                SYMsgSend(ENGINEID, &nextMsg);
                            }
                        }
                    }
                }
                PageList_Print();
                break;

            case MSG_TRAYCONFIG:
               //tray config notification, see if this will clear up any stalls
                //DPRINTF( (PMDEBUGSOFT), ("PM: got tray notify\n"));
                PrintMgrPrintPages(&nextMsg);
                PageList_Print();
                break;

            case MSG_STATUSUPDATE:
                {

                    /*
                     * The engine and video send this message. We then generate
                     * the status event message to the status manager if needed.
                     *
                     * Generally we just forward the message on but not always.
                     * If we are stalling for user input, the status mgr will
                     * take the system offline and have the PM as the source. The
                     * system will stay offline until the PM sends an online message.
                     *
                     * This means while stalled we don't want to forward any online
                     * messages from the engine or the system will go back online and
                     * we will still be sitting waiting for the user input message.
                    */

                    uint32_t status = nextMsg.param1;
                    bool sendStatus = TRUE;

                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: got status from engine 0x%08X param2 0x%x param3 0x%x\n", status, nextMsg.param2, nextMsg.param3);

                    if ( nextMsg.param2 == STATUS_TRAY) 		//((nextMsg.param2&STATUS_TRAY) == STATUS_TRAY)
                    {
                        PrintMgr_Notify_MediaStatus(status, NULL);  //new observer to notify Media status
                        switch(nextMsg.param1)
                        {
							// Tray1
							case STATUS_TRAY1_MEDIA_LETTER:
								gEngineInfo.machineTraySize[TRAY_2] = MEDIASIZE_LETTER;
								break;
							case STATUS_TRAY1_MEDIA_EXEC:
								gEngineInfo.machineTraySize[TRAY_2] = MEDIASIZE_EXECUTIVE;
								break;
							case STATUS_TRAY1_MEDIA_A4:
								gEngineInfo.machineTraySize[TRAY_2] = MEDIASIZE_A4;
								break;
							case STATUS_TRAY1_MEDIA_B5:
								gEngineInfo.machineTraySize[TRAY_2] = MEDIASIZE_B5;
								break;
							case STATUS_TRAY1_MEDIA_LEGAL:
								gEngineInfo.machineTraySize[TRAY_2] = MEDIASIZE_LEGAL;
								break;
							case STATUS_TRAY1_MEDIA_A5:
								gEngineInfo.machineTraySize[TRAY_2] = MEDIASIZE_A5;
								break;
							case STATUS_TRAY1_MEDIA_UNKNOWN:
								gEngineInfo.machineTraySize[TRAY_2] = MEDIASIZE_UNKNOWN;
								break;
							// Tray2
							case STATUS_TRAY2_MEDIA_LETTER:
								gEngineInfo.machineTraySize[TRAY_3] = MEDIASIZE_LETTER;
								break;
							case STATUS_TRAY2_MEDIA_EXEC:
								gEngineInfo.machineTraySize[TRAY_3] = MEDIASIZE_EXECUTIVE;
								break;
							case STATUS_TRAY2_MEDIA_A4:
								gEngineInfo.machineTraySize[TRAY_3] = MEDIASIZE_A4;
								break;
							case STATUS_TRAY2_MEDIA_B5:
								gEngineInfo.machineTraySize[TRAY_3] = MEDIASIZE_B5;
								break;
							case STATUS_TRAY2_MEDIA_LEGAL:
								gEngineInfo.machineTraySize[TRAY_3] = MEDIASIZE_LEGAL;
								break;
							case STATUS_TRAY2_MEDIA_A5:
								gEngineInfo.machineTraySize[TRAY_3] = MEDIASIZE_A5;
								break;
							case STATUS_TRAY2_MEDIA_UNKNOWN:
								gEngineInfo.machineTraySize[TRAY_3] = MEDIASIZE_UNKNOWN;
								break;
							// Tray3
							case STATUS_TRAY3_MEDIA_LETTER:
								gEngineInfo.machineTraySize[TRAY_4] = MEDIASIZE_LETTER;
								break;
							case STATUS_TRAY3_MEDIA_EXEC:
								gEngineInfo.machineTraySize[TRAY_4] = MEDIASIZE_EXECUTIVE;
								break;
							case STATUS_TRAY3_MEDIA_A4:
								gEngineInfo.machineTraySize[TRAY_4] = MEDIASIZE_A4;
								break;
							case STATUS_TRAY3_MEDIA_B5:
								gEngineInfo.machineTraySize[TRAY_4] = MEDIASIZE_B5;
								break;
							case STATUS_TRAY3_MEDIA_LEGAL:
								gEngineInfo.machineTraySize[TRAY_4] = MEDIASIZE_LEGAL;
								break;
							case STATUS_TRAY3_MEDIA_A5:
								gEngineInfo.machineTraySize[TRAY_4] = MEDIASIZE_A5;
								break;
							case STATUS_TRAY3_MEDIA_UNKNOWN:
								gEngineInfo.machineTraySize[TRAY_4] = MEDIASIZE_UNKNOWN;
								break;
						}
						DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: Engine Tray Paper Size : Tray1 <%d>, Tray2 <%d>, Tray3 <%d>\n"
						, gEngineInfo.machineTraySize[TRAY_1], gEngineInfo.machineTraySize[TRAY_2], gEngineInfo.machineTraySize[TRAY_3]);
                    }
                    else if ( nextMsg.param2 == STATUS_LAMINATEINFO)
                    {
						DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: sendStatus %x\n",status);
						PrintMgr_Notify_devicestatus(status, NULL);
					}
                    else
                    {
						//int tempValue = 0;
						//error_type_t db_retval = FAIL;

						//tempValue = db_get_var_int(PRINT_VARS_ID, "SizeTypeTray_size", &db_retval);
						//DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"DB_TEST : %d\n", tempValue);
						//tempValue++;
						//db_set_var(PRINT_VARS_ID, "SizeTypeTray_size", tempValue);

	                    /*
	                     * Any status coming from engine or video with the user input bit set
	                     * is an error that requires the user to perform some action before
	                     * we can go on. Check for that here. This function determines if this
	                     * is a stall status and also whether we should forward it to the
	                     * status manager.
	                    */
	                    sendStatus = PrintMgrCheckForForcedStall(&status);
	                    //  if(sendStatus == FALSE)
	                    //      DPRINTF( (PMDEBUGSOFT), ("PM: drop status from engine 0x%08X\n", status));

	                    if(sendStatus == TRUE)
	                    {
	                        PrintMgrEngineStatus(&nextMsg);

	                        //save the current printer status
	                        gPrintMgrInfo.status = status;

	                        //for any critical or fatal messages we take the engine
	                        //offline (we let the system decide what to do about warnings).
	                        //The job manager will cancel all pages on these errors
	                        //These errors take precedence over stalled for user
	                        //input so always send status to status manager here
	                        if(ISCRITICAL(status) || ISFATAL(status) || ISOFFLINE(status))
	                        {
	                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: eng offline, forward to stat mgr\n");
	                            gPrintMgrInfo.engOnline = FALSE;
	                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: flush active and pending lists\n" );
	                            PrintMgrFlushActiveList();
	                            PrintMgrFlushPendingList();
	                        }
	                        else
	                        {
	                            //else this is an online message
	                            if(gPrintMgrInfo.engOnline == FALSE)
	                            {
	                                //going from offline to online
	                                //tell engine to clear any error states
	                                //this will cause engine to send us a
	                                //credit update which will restart printing
	                                DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: eng online\n");

	                                // The error clear message send is commented out
	                                // because it was causing an extra print credit
	                                // update to occur. Note that the print online
	                                // message processing also causes a credit
	                                // update to occur.
	                                nextMsg.msgType = MSG_ERRCLEAR;
	                                SYMsgSend(ENGINEID, &nextMsg);
	                                gPrintMgrInfo.engOnline = TRUE;

	                                if ( gPrintMgrInfo.pageCancelState == PMCANCEL_WAIT_FOR_ENGINE_ONLINE)
	                                {
	                                    //gPrintMgrInfo.pageCancelState = PMCANCEL_NONE;
	                                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrAckJobMgrCancelDone!!!\n");
	                                    PrintMgrAckJobMgrCancelDone();
	                                }
	                            }

	                            //if we are not stalled then forward the online message to
	                            //the status mgr, if we are stalled we don't want to because
	                            //that would overwrite our stall message and leave us hung
	                            if (gPrintMgrInfo.mode == PM_STALLED)
	                            {
	                                sendStatus = FALSE;
	                                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stalled; do not forward online status\n");
	                            }
	                        }

	                        //if sendStatus is still true then forward the status event message
	                        //to the status manager
	                        if(sendStatus == TRUE)
	                        {
								DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: sendStatus %x\n",status);
	                            PrintMgr_Notify_devicestatus(status, UserAck);

	                            outMsg.msgType = MSG_PRINT_STATUS_EVENT;
	                            outMsg.param1 = status;
	                            outMsg.param2 = PRINTMGRID;
	                            PrintMgrPrintPages(&outMsg);
	                        }

	                        //Update DB for Printed pages
	                        if ( STATCATCMP(status, STATCAT_PAPEROUT) ||
	                          STATCATCMP(status, STATCAT_PAPERJAM_TYPE) ||
	                          STATCATCMP(status, STATCAT_PRINTHEAD) )
	                        {
	                            //UpdatePrintedPageCountToDB();
	                        }
	                    }
                    }
                }
                TempInitEngineConfig(&nextMsg);
                PageList_Print();
                break;

            case MSG_USER_INPUT:
                DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"PM: UserInput received \n");

                if(gPrintMgrInfo.mode == PM_NORMAL)
                {
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM_NORMAL\n ");
                    //got a user input when not stalled. This can occur for things
                    //like paper jam or paper out where the engine may not be able to detect
                    //the error is cleared, the user hit OK, send message to engine to try
                    //and detect the err clear. We expect to get a status update
                    //Status mgr will only forward this onto us for fatal or critical errors and
                    //for user ok button presses
                    nextMsg.msgType = MSG_USER_ERR_RECOVERY;
                    SYMsgSend(ENGINEID, &nextMsg);
                    // OBSOLETE - engine should handle this SYMsgSend(VIDEOMGRID, &nextMsg);
                }
                else
                {
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM_STALLED, etc.\n");

                    //user hit ok while we were stalled so now see if we can print
                    PrintMgrPrintPages(&nextMsg);
                }
                PageList_Print();
                break;

            case MSG_PAPER_ADDED:
            case MSG_OUTPUT_BIN_CLEARED:
                PrintMgrPrintPages(&nextMsg);
                PageList_Print();
                break;

            case MSG_DELAY_NOTIFY:
                //we get this message when the timed override timer expires. It
                //acts just like the user hit the continue or cancel button
                PrintMgrProcessTimedOverride(nextMsg.param1);
                break;

            case MSG_TIME_TO_END:
                SYMsgSend(ENGINEID, &nextMsg);
                break;

            case MSG_STATUS_TONER:
				gEngineInfo.tonerInfo.currentTonerPageCount = nextMsg.param1;
				gEngineInfo.tonerInfo.currentTonerRemain = nextMsg.param2;
				gCurTonerPageCount = gEngineInfo.tonerInfo.currentTonerPageCount;
				DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## TONER COUNT = %d, Remain(per) = %d ##\n", nextMsg.param1, nextMsg.param2);
				if(gEngineInfo.motorRunState.new_state == 0)
				{
					db_set_var_int("INST_TONERCOUNT","Count",gEngineInfo.tonerInfo.currentTonerPageCount);
					db_set_var_int("INST_TONERCOUNT","CountInPercent",gEngineInfo.tonerInfo.currentTonerRemain);
				}
				break;

			case MSG_STATUS_TONER_REMAIN_CHK_REQ:
				PrintMgr_Notify_devicestatus(STATUS_TONER_REMAIN_CHECK, UserAck);
				DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## MSG_STATUS_TONER_REMAIN_CHK_REQ : Remain(per) = %d ##\n", gEngineInfo.tonerInfo.currentTonerRemain);
				break;

			case MSG_MACHINE_LOCATION_INFO:
				gEngineInfo.machineLocation = nextMsg.param1;
				DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## MSG_MACHINE_LOCATION_INFO = %d ##\n", gEngineInfo.machineLocation);
				break;

			case MSG_CARTRIDGE_PRINT_JOB_MODE_INFO:
				gEngineInfo.tonerInfo.cFreeMotorOn = nextMsg.param1;
				gEngineInfo.tonerInfo.TonerJobByModes = (int*)nextMsg.param3;
				DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## Toner Job by mode %d %d %d %d %d %d %d %d %d %d , Free Motor On (%d)##\n",
				gEngineInfo.tonerInfo.TonerJobByModes[0],gEngineInfo.tonerInfo.TonerJobByModes[1],gEngineInfo.tonerInfo.TonerJobByModes[2],gEngineInfo.tonerInfo.TonerJobByModes[3],
				gEngineInfo.tonerInfo.TonerJobByModes[4],gEngineInfo.tonerInfo.TonerJobByModes[5],gEngineInfo.tonerInfo.TonerJobByModes[6],gEngineInfo.tonerInfo.TonerJobByModes[7],
				gEngineInfo.tonerInfo.TonerJobByModes[8],gEngineInfo.tonerInfo.TonerJobByModes[9],gEngineInfo.tonerInfo.cFreeMotorOn);
				break;

			case MSG_CARTRIDGE_PRINTED_BY_COVERAGE_INFO:
				gEngineInfo.tonerInfo.TonerPrintedByCoverage = (int*)nextMsg.param3;
				DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## Toner Printed by coverage %d %d %d %d %d %d ##\n",
				gEngineInfo.tonerInfo.TonerPrintedByCoverage[0],gEngineInfo.tonerInfo.TonerPrintedByCoverage[1],gEngineInfo.tonerInfo.TonerPrintedByCoverage[2],
				gEngineInfo.tonerInfo.TonerPrintedByCoverage[3],gEngineInfo.tonerInfo.TonerPrintedByCoverage[4],gEngineInfo.tonerInfo.TonerPrintedByCoverage[5]);
				break;

			case MSG_CARTRIDGE_INFO:
				gEngineInfo.tonerInfo.currentCartridgeSN = nextMsg.param1;
				snprintf(serialnumber, sizeof(serialnumber), "%d", gEngineInfo.tonerInfo.currentCartridgeSN);
				gEngineInfo.tonerInfo.currentCartridgeCapacity = nextMsg.param2;
				gEngineInfo.tonerInfo.currentCartridgeTLI  = (char*)nextMsg.param3;
				
				if ( strncmp(gEngineInfo.tonerInfo.currentCartridgeTLI, "A616DNST", 8) )
					bSepCartridge = TRUE;
				else
					bSepCartridge = FALSE;

				DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## Cartridge : TLI = %s, SN = %d Cap = %d %s bSepCartridge(%d) ##\n",
				gEngineInfo.tonerInfo.currentCartridgeTLI, gEngineInfo.tonerInfo.currentCartridgeSN, gEngineInfo.tonerInfo.currentCartridgeCapacity, serialnumber, bSepCartridge);
				db_set_recd_toner_history(gEngineInfo.tonerInfo.currentCartridgeTLI, serialnumber, gEngineInfo.tonerInfo.currentCartridgeCapacity);

				//gEngineInfo.tonerInfo.currentCartridgeSN = nextMsg.param1;
				//gEngineInfo.tonerInfo.currentCartridgeTLI  = (char*)nextMsg.param3;
				//DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## Cartridge : TLI = %s, SN = %d ##\n", gEngineInfo.tonerInfo.currentCartridgeTLI, gEngineInfo.tonerInfo.currentCartridgeSN);
				//db_set_var_int("INST_MACHINE","TonerSerialNo",gEngineInfo.tonerInfo.currentCartridgeSN);
				//db_set_var("INST_MACHINE","TonerTLI",gEngineInfo.tonerInfo.currentCartridgeTLI);
				break;

			case MSG_STATUS_DRUM_RESET:
				if(nextMsg.param1 == DRUM_CARTRIDGE_RESET)
				{
					DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : GOT MSG_STATUS_DRUM_RESET !!!##\n");
					ResetDrumCountToDB();
				}
				break;
				
			case MSG_STATUS_FUSER_RESET:
				if(nextMsg.param1 == FUSER_UNIT_RESET)
				{
					DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : GOT MSG_STATUS_FUSER_RESET !!!##\n");
					ResetFuserCountToDB();
				}
				break;

			case MSG_TRAY_VERSION:
				gEngineInfo.tray2VersionInfo = nextMsg.param1;
				gEngineInfo.tray3VersionInfo = nextMsg.param2;
				if(gEngineInfo.tray2VersionInfo == 0) {
					gEngineInfo.optionTrayNum = NO_OPTIONTRAY_DETACTED;
				}
				else {
					if(gEngineInfo.tray3VersionInfo == 0) {
						gEngineInfo.optionTrayNum = ONE_OPTIONTRAY_DETACTED;
					}
					else {
						gEngineInfo.optionTrayNum = TWO_OPTIONTRAY_DETACTED;
					}
				}
				db_set_var_int("INST_MACHINE","T2Version",gEngineInfo.tray2VersionInfo);
				db_set_var_int("INST_MACHINE","T3Version",gEngineInfo.tray3VersionInfo);
				DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## Tray2 Ver = %x, Tray3 Ver = %x <%d> ##\n", gEngineInfo.tray2VersionInfo, gEngineInfo.tray3VersionInfo, gEngineInfo.optionTrayNum);
				break;

            case MSG_STATUS_MOTOR_RUN:
				{
					int db_DLPC = 0;
					int job_mode_sx = 0;
					int job_mode_dx = 0;
					int tempDLPC_Remain;
					int tempPrintedPages;
					bool f_stop_after_print = false;
					static uint32_t job_cnt = DRUM_LOW_1_CYCLE;

					gEngineInfo.motorRunState.new_state = nextMsg.param1;
					if((gEngineInfo.motorRunState.old_state == 1) && (gEngineInfo.motorRunState.new_state == 0))		// Motor Runing >> Stop
					{
						if((gEngineInfo.printedPages.simplex !=0) || (gEngineInfo.printedPages.duplex !=0))
						{
							if (gEngineInfo.printedPages.simplex <= 10)
								job_mode_sx = gEngineInfo.printedPages.simplex;
							else if ((gEngineInfo.printedPages.simplex > 10) && (gEngineInfo.printedPages.simplex <= 99))
							    job_mode_sx = 11;
							else
								job_mode_sx = 12;

							if (gEngineInfo.printedPages.duplex <= 10)
								job_mode_dx = gEngineInfo.printedPages.duplex;
							else if ((gEngineInfo.printedPages.duplex > 10) && (gEngineInfo.printedPages.duplex <= 99))
							    job_mode_dx = 11;
							else
								job_mode_dx = 12;

							DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : TonerJobMode SX (%d) DX (%d) ##\n",gEngineInfo.printedPages.simplex, gEngineInfo.printedPages.duplex);

							gEngineInfo.currentJobDrumLifePageCount = (TonerJobMode[job_mode_sx] * gEngineInfo.printedPages.simplex) + (TonerJobMode[job_mode_dx] * gEngineInfo.printedPages.duplex * 3 / 2);
							f_stop_after_print = true;

							tempPrintedPages = gEngineInfo.printedPages.simplex + gEngineInfo.printedPages.duplex;
							UpdateJobByModesToDB(tempPrintedPages);
						}
						else
						{
							gEngineInfo.currentJobDrumLifePageCount = NIP_TONER_CONSUMPTION;
							f_stop_after_print = false;
							if (bSepCartridge)
								gDB_Info.drumInfo.FreeMotorRunCount++;
							db_set_var_int("INST_DRUMCOUNT", "FreeMotorOnDrumCount", gDB_Info.drumInfo.FreeMotorRunCount);
						}

						// Toner Count & Remain Percentage
						db_set_var_int("INST_TONERCOUNT","Count",gEngineInfo.tonerInfo.currentTonerPageCount);
						db_set_var_int("INST_TONERCOUNT","CountInPercent",gEngineInfo.tonerInfo.currentTonerRemain);

						// Drum Life Page Count (DLPC) & Remain percentage
						if (bSepCartridge)
							gEngineInfo.machineDrumLifePageCount += gEngineInfo.currentJobDrumLifePageCount;
						
						db_DLPC = gEngineInfo.machineDrumLifePageCount / 100;
						//db_set_var_int("INST_DRUMCOUNT", "DrumLifePageCount", db_DLPC);

						if(db_DLPC >= DRUM_LOW_MODE_1)
						{
							tempDLPC_Remain = 0;
						}
						else
						{
							//tempDLPC_Remain = (DRUM_END - db_DLPC) * 100 / DRUM_END;
							tempDLPC_Remain = 100 - (db_DLPC * 100 / DRUM_LOW_MODE_1);
						}
						//db_set_var_int("INST_DRUMCOUNT", "CountInPercent", tempDLPC_Remain);
						gDB_Info.drumInfo.DrumLifeRemainPercentage = tempDLPC_Remain;
						// Motor Run Time (*10msec)
						//gDB_Info.drumInfo.MotorRunTime += nextMsg.param2;		// 161028: changed to Normal speed only
						gDB_Info.fuserInfo.MotorRunNormal += nextMsg.param2;	// 161028: added for Normal speed
						stMotorLow = (int*)nextMsg.param3;		// 161028: added for Low speed
						gDB_Info.fuserInfo.MotorRunLow += stMotorLow[0];
						if (bSepCartridge)
							gDB_Info.drumInfo.MotorRunTime += (nextMsg.param2 + stMotorLow[0]);	//(gDB_Info.fuserInfo.MotorRunNormal + gDB_Info.fuserInfo.MotorRunLow);
						//db_set_var_int("INST_DRUMCOUNT", "Motor", gDB_Info.drumInfo.MotorRunTime);

						gEngineInfo.printedPages.simplex = 0;
						gEngineInfo.printedPages.duplex = 0;
						DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : MotorStop!!! DLPC Job = %d, DLPC Mac = %d CurrentMotorNormal = %d, CurrentMotorLow = %d ##\n",gEngineInfo.currentJobDrumLifePageCount, gEngineInfo.machineDrumLifePageCount, nextMsg.param2, stMotorLow[0]);

						// Machine Print Count & Drum Print Count
						UpdatePrintedPageCountToDB(gDB_Info.drumInfo.DrumLifeRemainPercentage, gDB_Info.drumInfo.MotorRunTime, db_DLPC);
						UpdateFuserCountToDB(nextMsg.param2, stMotorLow[0]);

						// Send Drum Page Count to Engine
						nextMsg.msgType = MSG_DRUM_PAGE_COUNT;
						nextMsg.param1 = db_DLPC;
						SYMsgSend(ENGINEID, &nextMsg);

						//Drum Low & End Message Handling
						if(db_DLPC >= DRUM_END)
						{
							PrintMgr_Notify_devicestatus(STATUS_DRUM_END, UserAck);
							gEngineInfo.fDrumLowMode_3 = true;
						}
						else if(db_DLPC >= DRUM_LOW_MODE_3)
						{
							gEngineInfo.fDrumLowMode_3 = true;
							if(gEngineInfo.fFirstMotorStop == TRUE)
							{
								PrintMgr_Notify_devicestatus(STATUS_DRUM_LOW_MODE_2, UserAck);
							}
						}
						else if(db_DLPC >= DRUM_LOW_MODE_2)
						{
							PrintMgr_Notify_devicestatus(STATUS_DRUM_LOW_MODE_2, UserAck);
						}
						else if(db_DLPC >= DRUM_LOW_MODE_1)
						{
							if(f_stop_after_print==true)
							{
								job_cnt++;
								if(job_cnt >= DRUM_LOW_1_CYCLE)
								{
									job_cnt = 0;
									PrintMgr_Notify_devicestatus(STATUS_DRUM_LOW_MODE_1, UserAck);
								}
							}
							else if(gEngineInfo.fFirstMotorStop == TRUE)
							{
								PrintMgr_Notify_devicestatus(STATUS_DRUM_LOW_MODE_1, UserAck);
							}
						}
						gEngineInfo.fFirstMotorStop = FALSE;
					}
					gEngineInfo.motorRunState.old_state = gEngineInfo.motorRunState.new_state;
				}
				break;

            case MSG_CONFIG_CHANGED:
				{
					DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## PM: Got MSG_CONFIG_CHANGED ##\n");
					int tempTopMargin = 0;
					int tempLeftMargin = 0;
					int tempBackTopMargin = 0;
					int tempBackLeftMargin = 0;
					int tempBeamAdjust = 0;
					int tempVerticalMagnification = 0;
					int tempHorizontalMagnification = 0;
					int tempA4LTGapMargin = 0;
					int tempCopyLeftMargin = 0;
					int tempCopyBackLeftMargin = 0;
					int tempCopyTopMargin = 0;
					int tempCopyBackTopMargin = 0;
					int tempServoAdjustValue = 0;
					mediacnfg_t pMediaCnfg;

					error_type_t db_retval = FAIL;

					tempTopMargin = db_get_var_int("CONF_Margin", "TOP", &db_retval);
					tempLeftMargin = db_get_var_int("CONF_Margin", "LEFT", &db_retval);
					tempBackTopMargin = db_get_var_int("CONF_DuplexMargin", "TOP", &db_retval);
					tempBackLeftMargin = db_get_var_int("CONF_DuplexMargin", "LEFT", &db_retval);
					tempBeamAdjust = db_get_var_int("CONF_LSUPWM", "B1_PWM1", &db_retval);
					tempVerticalMagnification = db_get_var_int("CONF_Margin", "VERTICALMAGNIFICATION", &db_retval);
					tempHorizontalMagnification = db_get_var_int("CONF_Margin", "HORIZONMAGNIFICATION", &db_retval);
					tempA4LTGapMargin = db_get_var_int("CONF_DuplexMargin", "A4_LT_GAP", &db_retval);
					tempCopyLeftMargin = db_get_var_int("CONF_Margin", "COPY_LEFT", &db_retval);
					tempCopyBackLeftMargin = db_get_var_int("CONF_DuplexMargin", "COPY_LEFT", &db_retval);
					tempCopyTopMargin = db_get_var_int("CONF_Margin", "COPY_TOP", &db_retval);
					tempCopyBackTopMargin = db_get_var_int("CONF_DuplexMargin", "COPY_TOP", &db_retval);
					tempServoAdjustValue = db_get_var_int("CONF_SYSTEMSP", "ServoAdjust", &db_retval);
					
					PRNDUMPENABLE = db_get_var_int("CONF_SYSTEMSP","PRNDumpEnable",&db_retval);

					printvar_set_trayconfig_get_from_DB();
					printvar_get_trayconfig(&pMediaCnfg);

					DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED \n Type = %d %d %d %d \n Texture = %d %d %d %d %d %d %d %d\n Weight = %d %d %d %d %d %d %d %d\n Darkness = %d Setting Priority = %d TonerSave = %d\n"
					,pMediaCnfg.mediaType[0],pMediaCnfg.mediaType[1],pMediaCnfg.mediaType[2],pMediaCnfg.mediaType[3]
					,pMediaCnfg.paperType.PlainPaper.Texture, pMediaCnfg.paperType.TransParency.Texture, pMediaCnfg.paperType.PaperLabel.Texture
					,pMediaCnfg.paperType.VinylLabel.Texture, pMediaCnfg.paperType.Envelope.Texture, pMediaCnfg.paperType.CardStock.Texture
					,pMediaCnfg.paperType.PostCard.Texture, pMediaCnfg.paperType.Recycled.Texture
					,pMediaCnfg.paperType.PlainPaper.Weight, pMediaCnfg.paperType.TransParency.Weight, pMediaCnfg.paperType.PaperLabel.Weight
					,pMediaCnfg.paperType.VinylLabel.Weight, pMediaCnfg.paperType.Envelope.Weight, pMediaCnfg.paperType.CardStock.Weight
					,pMediaCnfg.paperType.PostCard.Weight, pMediaCnfg.paperType.Recycled.Weight
					,pMediaCnfg.mediaPrintDarkness, pMediaCnfg.traySettingPriority, pMediaCnfg.tonerSaveMode);

					if(gDB_Info.topMargin != tempTopMargin)
					{
						gDB_Info.topMargin = tempTopMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x02;
						nextMsg.param2 = gDB_Info.topMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - topMargin=%d ##\n",gDB_Info.topMargin);
					}
					if(gDB_Info.leftMargin != tempLeftMargin)
					{
						gDB_Info.leftMargin = tempLeftMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x03;
						nextMsg.param2 = gDB_Info.leftMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - leftMargin=%d ##\n",gDB_Info.leftMargin);
					}
					if(gDB_Info.backTopMargin != tempBackTopMargin)
					{
						gDB_Info.backTopMargin = tempBackTopMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x05;
						nextMsg.param2 = gDB_Info.backTopMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - backTopMargin=%d ##\n",gDB_Info.backTopMargin);
					}
					if(gDB_Info.backLeftMargin != tempBackLeftMargin)
					{
						gDB_Info.backLeftMargin = tempBackLeftMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x06;
						nextMsg.param2 = gDB_Info.backLeftMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - backLeftMargin=%d ##\n",gDB_Info.backLeftMargin);
					}
					if(gDB_Info.beamAdjust != tempBeamAdjust)
					{
						gDB_Info.beamAdjust = tempBeamAdjust;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x07;
						nextMsg.param2 = gDB_Info.beamAdjust;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - leftMargin=%d ##\n",gDB_Info.beamAdjust);
					}
					if(gDB_Info.verticalMagnification != tempVerticalMagnification)
					{
						gDB_Info.verticalMagnification = tempVerticalMagnification;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x08;
						nextMsg.param2 = gDB_Info.verticalMagnification;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - leftMargin=%d ##\n",gDB_Info.verticalMagnification);
					}
					if(gDB_Info.horizontalMagnification != tempHorizontalMagnification)
					{
						gDB_Info.horizontalMagnification = tempHorizontalMagnification;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x09;
						nextMsg.param2 = gDB_Info.horizontalMagnification;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - leftMargin=%d ##\n",gDB_Info.horizontalMagnification);
					}
					if(gDB_Info.A4LTGapMargin != tempA4LTGapMargin)
					{
						gDB_Info.A4LTGapMargin = tempA4LTGapMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x0A;
						nextMsg.param2 = gDB_Info.A4LTGapMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - A4LTGapMargin=%d ##\n",gDB_Info.A4LTGapMargin);
					}
					if(gDB_Info.copyLeftMargin != tempCopyLeftMargin)
					{
						gDB_Info.copyLeftMargin = tempCopyLeftMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x0B;
						nextMsg.param2 = gDB_Info.copyLeftMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - CopyLeftMargin=%d ##\n",gDB_Info.copyLeftMargin);
					}
					if(gDB_Info.copyBackLeftMargin != tempCopyBackLeftMargin)
					{
						gDB_Info.copyBackLeftMargin = tempCopyBackLeftMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x0C;
						nextMsg.param2 = gDB_Info.copyBackLeftMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - CopyBackLeftMargin=%d ##\n",gDB_Info.copyBackLeftMargin);
					}
					if(gDB_Info.copyTopMargin != tempCopyTopMargin)
					{
						gDB_Info.copyTopMargin = tempCopyTopMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x0D;
						nextMsg.param2 = gDB_Info.copyTopMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - CopyTopMargin=%d ##\n",gDB_Info.copyTopMargin);
					}
					if(gDB_Info.copyBackTopMargin != tempCopyBackTopMargin)
					{
						gDB_Info.copyBackTopMargin = tempCopyBackTopMargin;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x0E;
						nextMsg.param2 = gDB_Info.copyBackTopMargin + MargineOffset;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - CopyBackTopMargin=%d ##\n",gDB_Info.copyBackTopMargin);
					}
					if(gDB_Info.servoadjustvalue != tempServoAdjustValue)
					{
						gDB_Info.servoadjustvalue = tempServoAdjustValue;
						nextMsg.msgType = MSG_CONFIG_CHANGED;
						nextMsg.param1 = 0x0F;
						nextMsg.param2 = gDB_Info.servoadjustvalue;
						SYMsgSend(ENGINEID, &nextMsg);
						DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## MSG_CONFIG_CHANGED - ServoAdjustValue=%d ##\n",gDB_Info.servoadjustvalue);
					}
					//DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"MSG_CONFIG_CHANGED : Duplex=%d, TopMargin=%d, LeftMargin=%d \n", tempDuplex,tempTopMargin,tempLeftMargin);
				}
				break;

            default:
       //         DPRINTF((PMDEBUG), ("PM: unhandled msgType='%s'=0x%X=%d\n", DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType, nextMsg.msgType));
                //ASSERT(0);
                break;
        } // end switch(nextMsg.msgType)
    } // end while(1)
    return 0;
} // end PrintMgrThread()

/******************************************************************************
 * Function Name: PrintMgrPrintPages
 */
/**
 * \brief Main loop message handler for PrintMgr.
 *
 * \date 2/24/2004
 *
 * <pre>
 * State transitions:
 *
 *  CurState        Event               Action                  NewState
 * ===========================================================================
 *  NONE            MSG_PREPAGESTART    add to pending          PENDING
 *
 *  PENDING         any                 if stall
 *                                         if printing          STALLING
 *                                         else                 STALL
 *                                      else if can print
 *                                         send query           QUERY
 *                                      else                    PENDING
 *
 *  QUERY           MSG_PAGEQUERY_RSP                           QUERIED
 *
 *  QUERIED         any                 if HaveData
 *                                        pagestart to engine   ACTIVE
 *                                      else                    QUERIED
 *
 *  ACTIVE          MSG_PAGEDONE        page done to jobmgr     NONE
 *
 *  STALLING        MSG_PAGEDONE        if all pages done       STALLED
 *                                      else                    STALLING
 *
 *  STALLED for     MSG_PAPER_ADDED     gStallInfo.reason =     STALLED for
 *  STALL_SIZETYPE  && gStallInfo.size,   STALL_RECONFIGURE;    STALL_RECONFIGURE
 *   or             type, & tray not    PrintMgrStartStall();
 *  STALL_PAPEROUT  config'd suitably   TODO
 *
 *  STALLED for     MSG_USER_INPUT      DoStallCheck(ACTION_    PENDING
 *  STALL_RECONFIGURE                     CONTINUE_PAPERADDED);
 *                                      which retries tray selection.
 *
 *  STALLED         MSG_PAPER_ADDED                             PENDING
 *                  MSG_USER_INPUT                              PENDING
 *                  MSG_OUTPUT_BIN_CLEARED                      PENDING
 *
 *  any             MSG_PAGESTART       HaveData = TRUE         any
 * </pre>
 *
 * \note Any time we get a fatal/critical error from the printer or
 *       a page cancel from the job manager, we empty our page lists.
 *       The job manager will resend the pages after the error is cleared
 *       or the job canceled.
 *
 ******************************************************************************/
void PrintMgrPrintPages(MESSAGE* msg)
{
    ATLISTENTRY* node = NULL;
    printmgr_page_t* pmpage = NULL;

    bool done = FALSE;
    bool processReady = FALSE;
    bool printReady = FALSE;
    bool printing = FALSE;
    STALLREASON reason;
    // uint32_t TimeDelay;

    static uint32_t page_sent = 0;  // debug counter of pages sent


    // agprintf("PM: PrintMgrPrintPages() 0x%x\n", msg->param3);
    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: PrintMgrPrintPages: enter\n");

    //First set up some booleans that make the logic in the while loop
    //simpler. Alot of the cases have to do these same checks.

    //if both the engine and the system are on line we can process pages
    if(gPrintMgrInfo.engOnline && gPrintMgrInfo.sysOnline)
    {
        processReady = TRUE;
        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: online, can process\n");
    }

    //if we are process ready and the engine has given us print credits we
    //can handle print pages
    if(processReady && gPrintMgrInfo.numCredits > 0)
    {
        printReady = TRUE;
        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: have %d credits, can print\n", gPrintMgrInfo.numCredits);
    }

    //see if there are active pages printing (have been sent to engine)
    if(!ATIsListEmpty(&printmgr_active_list))
    {
        printing = TRUE;
        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: already printing\n");
    }

    // We handle forced stall separately from paper path stall.
    // Check here to see if we need to do a forced stall.
    // If doing a forced stall then skip the paper path stall.
    reason = PrintMgrHandleForcedStall(msg);

    //if doing some sort of forced stall process the stall
    if(reason != STALL_NONE)
    {
        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall reason=%d=%s, state=%d=%s\n", reason,
                     ToString_STALLREASON(reason), gForcedStallInfo.state, ToString_FORCEDSTATE(gForcedStallInfo.state));
        //pending means it is in the process of doing a stall
        //we don't want to run the print path stall check so set done to true
        if(reason == STALL_PENDING)
        {
            done = TRUE;
        }
        //else the stall started or finished
        else
        {
            if(reason == STALL_FINISHED)
            {
                gPrintMgrInfo.mode = PM_NORMAL;
                PrintMgrClearStall();
            }
            else
            {
                gPrintMgrInfo.mode = PM_STALLED;
                PrintMgrStartStall();
                done = TRUE;
            }
        }
    }

    while(!done)
    {
        //scan through the pages in the pending list and process any that we
        //can. We only process one page at a time. We don't move on until the
        //current page is moved to the active list
        node = ATListHead(&printmgr_pending_list);
        if(node != NULL)
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: process page\n");
            pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);

            switch(pmpage->state)
            {
                case PRMGR_PAGE_PENDING:
                    {
                        // agprintf("PM: page pending\n");
                        /*
                         * page is pending, see if we have everything we need to print it
                        */
                        bool sendQuery = FALSE;
                        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: page pending\n");

                        //if we are currently stalled then move this page to the stalled state
                        //and exit. We only hit this case if we hit a forced NOW stall. In that
                        //case we stall even if there are no pending pages. If a page arrives while
                        //that stall is active we need to put the page in the stalled state so
                        //we won't try and do a stall check on it until after the forced stall
                        //is cleared
                        if(gPrintMgrInfo.mode == PM_STALLED)
                        {
                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: page pending, but stalled\n");
                            pmpage->state = PRMGR_PAGE_STALLED;
                            done = TRUE;
                            break;
                        }


                        //We want to only send a single manual feed page to the
                        //printer at a time. If we have an active page at
                        //the printer (printing == TRUE) and it was a manual
                        //feed then break and try again when get the page done
                        //DPRINTF( (PMDEBUGSOFT), ("PM: printing: %ld, isManual: %ld\n", printing, gLastPage.isManual));
                        if(printing == TRUE && gLastPage.isSingleFeed == TRUE )
                        {
                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: printing manual wait for it to finish\n");
                            done = TRUE;
                            break;
                        }

                        /* Special case:
                         *    if all trays out of paper we want to do the stall check
                         *    anyway to find a specific tray to prompt the user to load
                         *    Don't do this if we are in the middle of canceling
                        */
                        if(STATCATCODECMP(gPrintMgrInfo.status, STATUS_PAPER_OUT_ALL) && gPrintMgrInfo.pageCancelState == PMCANCEL_NONE)
                        {
                            printReady = TRUE;
                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: paper out, force paper path stall check\n");
                        }

                        //only do stall check on real print pages, not cal pages
                        //and only if we have credits and are online, or if a forced
                        //stall was requested
                        // if(printReady && !calReady)
                        if ( printReady  && (pmpage->page_obj->DocType != e_CALPAGE) )
                        {
                            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: page pending do stall check\n");
                            reason = PrintMgrDoStallCheck(pmpage->page_obj, ACTION_START);

                            if(reason != STALL_NONE)
                            {
                                if(!ATIsListEmpty(&printmgr_active_list))
                                {
                                    //currently printing, we have to wait for current pages
                                    //to finish
                                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stalling, wait for page done\n");
                                    gPrintMgrInfo.mode = PM_STALLING;
                                }
                                else
                                {
                                    gPrintMgrInfo.mode = PM_STALLED;
                                    PrintMgrStartStall();
                                }

                                //page is stalled until something changes (paper added,
                                //size changed)
                                pmpage->state = PRMGR_PAGE_STALLED;
                                done = TRUE;
                            }
                            else
                            {
                                //page can print, send the query
                                gPrintMgrInfo.mode = PM_NORMAL;

                                sendQuery = TRUE;
                            }
                        }
                        else if ( pmpage->page_obj->DocType == e_CALPAGE ) //(calReady)
                        {
                            sendQuery = TRUE;
                        }
                        else
                        {
                            done = TRUE;
                        }

                        //were not stalling send the query to the engine
                        if(sendQuery)
                        {
                            // Test for prestart, don't set sendQuery if prestart! DAB
                            if (msg->msgType == MSG_PAGEPRESTART)
                            {
                                pmpage->state = PRMGR_WAIT_FOR_PAGESTART;
                                PrintMgrSendMsg(ENGINEID, MSG_PAGEPRESTART, 0, 0, pmpage->page_obj);
                            }
                            else
                            {
                                pmpage->state = PRMGR_PAGE_QUERIED;

                                // we must set done to false so that the loop will run again with the new parameter. This used to done
                                // by the MSG_PAGEQUERY_RESP sent by the engine would then call this function again.
                                done = false;
                            }
                        }
                    }
                    break;

                case PRMGR_WAIT_FOR_PAGESTART:
                    {
                        // Simply advance to the next state, nothing to do here.
                        pmpage->state = PRMGR_PAGE_QUERIED;
                        done = true;
                    }

                    break;

                case PRMGR_PAGE_QUERIED:
                    // Send up to numCredits page start messages to the engine.
                    // Move the pages from the pending list to the active list.
                    // Only send if all the page data is present.
                    if( (gPrintMgrInfo.pageCancelState == PMCANCEL_NONE) && (printReady /*|| calReady*/) && (pmpage->have_data))
                    {
#if 0
                        TX_INTERRUPT_SAVE_AREA

                        if(TimeRdy == 0)
                        {
                            // Turn off the interrupts since one is pending.
                            TX_DISABLE
                            ATRemoveEntryList(&pmpage->node);
                            ATInsertTailList(&printmgr_active_list, &pmpage->node);
                            printing = TRUE;
                            //DPRINTF( (PMDEBUGSOFT), ("PM: PAGE: query: Link: 0x%08X\n", pItem));

                            pmpage->state = PRMGR_PAGE_ACTIVE;

                            gPrintMgrInfo.numCredits--;
                            page_sent++;
                            if (gPrintMgrInfo.numCredits == 0)
                            {
                                done = TRUE;
                            }
                            TX_RESTORE
                            break;
                        }

                        if (START_TIME)
                        {
                            // We don't have to worry about turning off interrupts
                            // here since we do not have any pending.
                            TimeRdy = 0;    // set up the timer for page pacing.
                            tx_timer_deactivate(&StartTimer);
                            TimeDelay = START_TIME;
                            if(gPrintMgrActiveList.LinkHead == NULL)
                                TimeDelay += INIT_TIME;
                            tx_timer_change(&StartTimer,TimeDelay,0);  // one shot timer
                            tx_timer_activate(&StartTimer);
                        }
#endif
                        ATRemoveEntryList(&pmpage->node);
                        ATInsertTailList(&printmgr_active_list, &pmpage->node);
                        printing = TRUE;

                        pmpage->state = PRMGR_PAGE_ACTIVE;
                        //DPRINTF( (PMDEBUGSOFT), ("PM: sent page start to eng page %d of job %d @ %lx\n", pmpage->page_obj->PageCount, pmpage->page_obj->pMyJob->JobNum, pmpage->page_obj));

                        PrintMgr_Notify_devicestatus(STATUS_INFO_PRINTING, UserAck);
                        DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"PM: sent page start to eng %lx credits %d\n", pmpage->page_obj, gPrintMgrInfo.numCredits);
                        PrintMgrSendMsg(ENGINEID, MSG_PAGESTART, 0, 0, pmpage->page_obj);

                        if( pmpage->page_obj->page_state != PAGE_STATE_ENG_PRESTART )
                        {
                            gPrintMgrInfo.numCredits--;
                        }
                        page_sent++;
                        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR, "PM: %s Sent page - credits: %d page sent: %d \n\n", __func__, gPrintMgrInfo.numCredits, page_sent);
                        if(gPrintMgrInfo.numCredits == 0)
                        {
                            done = TRUE;
                        }
                    }
                    else
                    {
                        done = TRUE;
                    }
                    break;

                case PRMGR_PAGE_STALLED:
                    {
                        //Page has been stalled for user input. Check to see if the
                        //condition has been resolved.

                        //got page done, if we are stalling (waiting for current pages
                        //to finish so we can stall) look to see if all pages in
                        //engine are done, if so go to stalled
                        if ( (gPrintMgrInfo.mode == PM_STALLING) &&
                             ATIsListEmpty(&printmgr_active_list) &&
                             ( (msg->msgType == MSG_PAGEDONE)
#if PAGEDONE_PROTOCOL_MAY_USE_PAGEVIDEODONE_THEN_PAGEDELIVERYDONE
                               || (msg->msgType == MSG_PAGEDELIVERYDONE)
#endif
                             ) )
                        {
                            gPrintMgrInfo.mode = PM_STALLED;
                            PrintMgrStartStall();
                            done = TRUE;
                        }
                        //else we are stalled, see if we can clear it.
                        else
                        {
                            bool doStallCheck = FALSE;
                            STALLACTION action = ACTION_NONE;

                            //user did something (added paper or hit a button).
                            //See if we can continue now
                            if(msg->msgType == MSG_USER_INPUT)
                            {
                                PrintMgrStopTimedOverride(gStallInfo.stallCount);

                                if (gStallInfo.reason == STALL_RECONFIGURE)
                                {
                                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: user reconfigured tray (or cancelled popup)\n");
                                    action = ACTION_CONTINUE_PAPERADDED;
                                }
                                else if(msg->param1 == PARAM_USER_OK)
                                {
                                    // PrintMgrHandlePrinterOptions();
                                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: got user input ok\n");
                                    action = ACTION_CONTINUE_YES;
                                }
                                else
                                {
                                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: got user input no\n");
                                    action = ACTION_CONTINUE_NO;
                                }
                                // Call below was moved from above to fix
                                // defect 484, cancel and continue both need to be handled.
                                PrintMgrHandlePrinterOptions();
                                doStallCheck = TRUE;
                            }
                            //paper added
                            else if(msg->msgType == MSG_PAPER_ADDED)
                            {
                                /**
                                 * The MSG_PAPER_ADDED message "should" have the
                                 * one-relative number (of the physical tray to
                                 * which paper was added) in param1. If param1 is
                                 * zero, then no tray's paper sensor transitioned
                                 * from empty to full. Not sure how this can happen,
                                 * but if it does, we use the tray from gStallInfo.
                                 *
                                 */
#if defined(HAVE_POPUP_TRAY_SIZE_TYPE_CONFIGURATION_SUPPORT)
                                int         oneRelativePhysicalTrayNumber = msg->param1;
                                trayenum_t    popupTRAYENUM = TRAY_NONE;

                                if (oneRelativePhysicalTrayNumber)
                                {
                                    popupTRAYENUM = EngPhysicalTrayNumberToTRAYENUM(oneRelativePhysicalTrayNumber);
                                }

                                if (popupTRAYENUM == TRAY_NONE)
                                {
                                    popupTRAYENUM = gStallInfo.tray;
                                    oneRelativePhysicalTrayNumber = EngTRAYENUMToPhysicalTrayNumber(popupTRAYENUM);
                                }

                                /**
                                 * Once we decide which tray to look at, then we determine
                                 * whether that tray's configuration is compatible with the
                                 * media size and media type being looked for.
                                 */
                                const mediacnfg_t pMediaCnfg;
                                printvar_get_trayconfig(&pMediaCnfg);
                                /**
                                 * PopUp functionality:
                                 * -# IF we prompted the user to load a tray
                                 *    with a specific media size or media type
                                 *    (== STALL_SIZETYPE or STALL_PAPEROUT), AND
                                 * -# IF the user has loaded a tray (we assume with the
                                 *    prompted-for size or type) (== MSG_PAPER_ADDED), AND
                                 * -# IF that tray's configured size or type does not match the
                                 *    prompted-for size or type,
                                 * -# THEN we must "PopUp" the "Tray # Size" and "Tray # Type"
                                 *    menus for the user to configure the tray just loaded.
                                 */
                                if ( ( (gStallInfo.reason == STALL_PAPEROUT) ||
                                       (gStallInfo.reason == STALL_SIZETYPE) ) &&
                                     !MediaSizeAndTypeFitTrayConfiguration(gStallInfo.size, gStallInfo.type, popupTRAYENUM, &pMediaCnfg) )
                                {
                                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PopUp: reconfigure physical tray %d (raw trayenum_t=%d)\n", oneRelativePhysicalTrayNumber, popupTRAYENUM);
                                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PopUp: suggest media size=%d, type=%d\n", gStallInfo.size, gStallInfo.type);

                                    // Set proper state and trigger the popup.
                                    gStallInfo.reason = STALL_RECONFIGURE;
                                    // User might have loaded new media into a tray different
                                    // from the tray in the original "Load tray" prompt, so
                                    // always set the reconfigure stall tray to the popup tray.
                                    gStallInfo.tray = popupTRAYENUM;
                                    PrintMgrStartStall();
                                } // end if ( (STALL_PAPEROUT || STALL_SIZETYPE) && (tray config not compatible) )
                                else
#endif // defined(HAVE_POPUP_TRAY_SIZE_TYPE_CONFIGURATION_SUPPORT)

                                // this check handles the case of the manual feed job where the user simply added paper to the manual feed tray
                                // this is equivalent to pushing the GO button according to paper handling flow chart.
                                if ( (msg->param1 == 1) &&
                                     ( (gStallInfo.reason == STALL_MANUALDUPLEX) ||
                                       (gStallInfo.reason == STALL_MANUALFEEDJOB) ||
                                       (gStallInfo.reason == STALL_CLEANINGPAGE) ) )
                                {
                                    // Paper has been added to physical tray 1,
                                    // for a job stalled for one of the above reasons.
                                    action = ACTION_CONTINUE_YES;
                                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: got user input via PAPER added to Tray 1\n");
                                    doStallCheck = TRUE;
                                }
                                else
                                {
                                    action = ACTION_CONTINUE_PAPERADDED;
                                    doStallCheck = TRUE;
                                }
#if defined(HAVE_POPUP_TRAY_SIZE_TYPE_CONFIGURATION_SUPPORT)
                          //      DSUnlockVar(e_TrayConfig);
#endif
                            } // end else if (msg->msgType == MSG_PAPER_ADDED)
                            //
                            else if (msg->msgType == MSG_OUTPUT_BIN_CLEARED)
                            {
                                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: user cleared output tray/bin.\n");
                                action = ACTION_CONTINUE_OUTPUT_BIN_CLEARED;
                                doStallCheck = TRUE;
                            }

                            if(doStallCheck)
                            {
                                //user did something we are interested in, rerun the stall checks
                                reason = PrintMgrDoStallCheck(pmpage->page_obj, action);

                                //check the result of the stall check
                                if(reason == STALL_NONE)
                                {
                                    //stall has been cleared. we can resume.
                                    gPrintMgrInfo.mode = PM_NORMAL;

                                    pmpage->state = PRMGR_PAGE_QUERIED;
                                    done = false;

                                    //tell status mgr we are back online so it will clear the
                                    //stall status
                                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: Paper added tell stat manager\n");

                                    PrintMgr_Notify_devicestatus(STATUS_ONLINE, NULL);

                                    PrintMgrClearStall();
                                }
                                else
                                {
                                    //we restalled, see why
                                    if(reason == STALL_CANCELJOB)
                                    {
                                        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: user requested cancel job\n");
                                        //send cancel page to status manager
                                        PrintMgrSendMsg(SJMID, MSG_CANCELJOB,
                                                        SYS_REQUEST, PRINTMGRID, (void*)e_Printer);
                                        //send online to clear the offline stall we sent

                                        PrintMgr_Notify_devicestatus(STATUS_ONLINE, NULL);
                                        PrintMgrClearStall();
                                    }
                                    else
                                    {
                                        PrintMgrStartStall();
                                    }
                                    done = TRUE;
                                }
                            }
                            else
                            {
                                //not a message we are looking for in this state
                                done = TRUE;
                            }
                        } // end else trying to clear stall.
                    } // end case PRMGR_PAGE_STALLED:
                    break;

                default:
                    DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: PRINT LIST CORRUPTED HELP!!!\n");
                    done = TRUE;
                    break;
            } // end switch(pItem->Param)
        }
        else // !pItem
        {
            //nothing to do
            done = TRUE;
        }
    } // end while(!done)

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrPrintPages: exit\n");
} // end PrintMgrPrintPages()

static void TempInitEngineConfig(MESSAGE* msg)
{
	if(firstStatusGet == TRUE)
	{
		error_type_t db_retval = FAIL;

		firstStatusGet = FALSE;
		gDB_Info.topMargin = db_get_var_int("CONF_Margin", "TOP", &db_retval);
		gDB_Info.leftMargin = db_get_var_int("CONF_Margin", "LEFT", &db_retval);
		gDB_Info.backTopMargin = db_get_var_int("CONF_DuplexMargin", "TOP", &db_retval);
		gDB_Info.backLeftMargin = db_get_var_int("CONF_DuplexMargin", "LEFT", &db_retval);
		gDB_Info.beamAdjust = db_get_var_int("CONF_LSUPWM", "B1_PWM1", &db_retval);
		gDB_Info.verticalMagnification = db_get_var_int("CONF_Margin", "VERTICALMAGNIFICATION", &db_retval);
		gDB_Info.horizontalMagnification = db_get_var_int("CONF_Margin", "HORIZONMAGNIFICATION", &db_retval);
		gDB_Info.A4LTGapMargin = db_get_var_int("CONF_DuplexMargin", "A4_LT_GAP", &db_retval);
		gDB_Info.copyLeftMargin = db_get_var_int("CONF_Margin", "COPY_LEFT", &db_retval);
		gDB_Info.copyBackLeftMargin = db_get_var_int("CONF_DuplexMargin", "COPY_LEFT", &db_retval);
		gDB_Info.copyTopMargin = db_get_var_int("CONF_Margin", "COPY_TOP", &db_retval);
		gDB_Info.copyBackTopMargin = db_get_var_int("CONF_DuplexMargin", "COPY_TOP", &db_retval);
		gDB_Info.servoadjustvalue = db_get_var_int("CONF_SYSTEMSP", "ServoAdjust", &db_retval);

		PRNDUMPENABLE = db_get_var_int("CONF_SYSTEMSP","PRNDumpEnable",&db_retval);

		DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"## Check_DB - topMargin=%d, leftMargin=%d, beamAdjust=%d ##\n", gDB_Info.topMargin, gDB_Info.leftMargin, gDB_Info.beamAdjust);


		msg->param1 = 0x02;
		msg->param2 = gDB_Info.topMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x03;
		msg->param2 = gDB_Info.leftMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x05;
		msg->param2 = gDB_Info.backTopMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x06;
		msg->param2 = gDB_Info.backLeftMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x07;
		msg->param2 = gDB_Info.beamAdjust;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x08;
		msg->param2 = gDB_Info.verticalMagnification;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x09;
		msg->param2 = gDB_Info.horizontalMagnification;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x0A;
		msg->param2 = gDB_Info.A4LTGapMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x0B;
		msg->param2 = gDB_Info.copyLeftMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x0C;
		msg->param2 = gDB_Info.copyBackLeftMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x0D;
		msg->param2 = gDB_Info.copyTopMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x0E;
		msg->param2 = gDB_Info.copyBackTopMargin + MargineOffset;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);

		msg->param1 = 0x0F;
		msg->param2 = gDB_Info.servoadjustvalue;
		PrintMgrSendMsg(ENGINEID, MSG_CONFIG_CHANGED, msg->param1, msg->param2, msg->param3);
	}
}

/******************************************************************************/
static void PrintMgrFlushActiveList(void)
{
    ATLISTENTRY* node;
    printmgr_page_t* pmpage;

    while ((node = ATRemoveHeadList(&printmgr_active_list)) != NULL)
    {
        pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);
        MEM_FREE_AND_NULL(pmpage);
    }
}

/******************************************************************************/
static void PrintMgrFlushPendingList(void)
{
    ATLISTENTRY* node;
    printmgr_page_t* pmpage;

    while ((node = ATRemoveHeadList(&printmgr_pending_list)) != NULL)
    {
        pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);
        MEM_FREE_AND_NULL(pmpage);
    }
}

//uint32_t page_done = 0;

/******************************************************************************
 * Function Name: PrintMgrHandlePageDeliveryDone
 */
/**
 * \brief Handle page done, when we get the Delivery part of our two-part protocol.
 *
 ******************************************************************************/
static void PrintMgrHandlePageDeliveryDone(MESSAGE* msg)
{
    PAGE_DATA*  pMostRecentlyDeliveredPage = (PAGE_DATA*)msg->param3;
    AGMSG       msgType = MSG_PAGEDONE;
    ATLISTENTRY* node;
    printmgr_page_t *pmpage = NULL;

    if ( pMostRecentlyDeliveredPage == 0 || pMostRecentlyDeliveredPage->sheet == 0 )
	return;
    PAGE_DATA* front = pMostRecentlyDeliveredPage->sheet->FrontPage;
    PAGE_DATA* back = pMostRecentlyDeliveredPage->sheet->BackPage;


    int cnt = front && back ? 2 : 1;

    pagesPrintedCounter += cnt;
    if(cnt == 2){
        ++pagesPrintedDuplexCounter;
    }
    
    trayenum_t recentPageEngTray = pMostRecentlyDeliveredPage->engTray;
    mediasize_t recentPageMediaSize;
    if(recentPageEngTray == TRAY_1){ //MPT
    	mediacnfg_t stMediaCnfg;
    	printvar_get_trayconfig(&stMediaCnfg);
    	recentPageMediaSize = stMediaCnfg.mediaSize[TRAY_1];
    }
    else{
    	recentPageMediaSize = gEngineInfo.machineTraySize[recentPageEngTray];
    }
    
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: Job  PrintedMedia size =%d ,recentPageMediaSize =%d \n",
											pMostRecentlyDeliveredPage->pMyJob->PrintedPaperSize,recentPageMediaSize);

	if(pMostRecentlyDeliveredPage->pMyJob->PrintedPaperSize == MEDIASIZE_ANY)
	{
		pMostRecentlyDeliveredPage->pMyJob->PrintedPaperSize =recentPageMediaSize;
	}
	else if(pMostRecentlyDeliveredPage->pMyJob->PrintedPaperSize != recentPageMediaSize)
	{
		uint32_t width1=0;
		uint32_t length1=0;
		uint32_t width2=0;
		uint32_t length2=0;
		if(OK == paper_get_media_size( pMostRecentlyDeliveredPage->pMyJob->PrintedPaperSize, &width1, &length1) )
		{
			DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM : existing printed dimensions WxH = %d x %d\n",width1,length1);
		}
		if(OK == paper_get_media_size( recentPageMediaSize, &width2, &length2)) 
		{
			DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM : new printed dimensions WxH = %d x %d\n",width2,length2);
		}
		if( (width1+length1) < (width2+length2) )
		{
			pMostRecentlyDeliveredPage->pMyJob->PrintedPaperSize =recentPageMediaSize;
		}

		DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM:  Job New PrintedMedia size =%d ,recentPageMediaSize =%d \n",
										pMostRecentlyDeliveredPage->pMyJob->PrintedPaperSize,recentPageMediaSize);
	}

    //If DocType is Report print
    DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: DeliveryDoneDocType=%d\n", pMostRecentlyDeliveredPage->DocType);
    DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: DeliveryDone_PJI_DocType=%d\n", pMostRecentlyDeliveredPage->pMyJob->pjob_info->jobDocType);

    DOCTYPE curJobDocType = pMostRecentlyDeliveredPage->pMyJob->pjob_info->jobDocType;
    if((curJobDocType == e_NETIOPAGE) ||(curJobDocType == e_PIPEIOPAGE)){
        pagesHostPrintCounter += cnt;
    }
    else if((curJobDocType == e_SYSTEMREPORT) ||(curJobDocType == e_NETWORKREPORT) ||(curJobDocType == e_SPREPORT) ||
	(curJobDocType == e_FAXTXRXREPORT) || (curJobDocType == e_FAXTCR) || (curJobDocType == e_T30LOGREPORT) || (curJobDocType == e_MEMTXJOBLIST) ||
	(curJobDocType == e_TCRFORFAXSERVICE) || (curJobDocType == e_ERRLOGREPORT) || (curJobDocType == e_ADDRESSBOOK) || (curJobDocType == e_SPAMFAXLIST) ||
	(curJobDocType == e_BROADCASTRESULTREPORT)){
        pagesReportPrintCounter += cnt;
    }
    else if((curJobDocType == e_USBDRIVEJPGPAGE) || (curJobDocType == e_USBDRIVEPDFPAGE)){
        pagesUSBPrintCounter += cnt;
    }
    else if((curJobDocType == e_FAX) || (curJobDocType == e_FAX_LONGPAGE)){
        gFaxPrintJobDocType = curJobDocType;
        pagesFaxPrintCounter += cnt;
        pagesFaxAppPagePrinted += cnt;
    }
    else if(curJobDocType == e_COPYPAGE){
        pagesCopyPrintCounter += cnt;
    }
    else{
        DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: ERR PPC: PJI_DocType=%d, DocType =%d\n",
                pMostRecentlyDeliveredPage->pMyJob->pjob_info->jobDocType, pMostRecentlyDeliveredPage->DocType);
                //ASSERT(FALSE); //TODO:
    }

#if 0		//To save printed page counter value to DB on every 10 pages printed.
    if(pagesPrintedCounter > 9){
        UpdatePrintedPageCountToDB();
    }
#endif
    while (cnt--) {

	    //    page_done++;

	    //page is done, increment number of page credits available if not canceling
	    //if in the middle of a cancel we want the credits to stay at 0
	    if(gPrintMgrInfo.pageCancelState == PMCANCEL_NONE)
		    gPrintMgrInfo.numCredits++;
	    //remove page from active list and free memory
        node = ATRemoveHeadList(&printmgr_active_list);
	    if (!node) {
		    DBG_PRINTF(LOG_WARNING|DBG_PRINTMGR,"PM: DeliveryDone(x%x) with no remaining Active pages, ignored!\n", msg->param3);
		    return;
	    }
	    DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: page done page %d of job %d @ %lx, credits: %lx\n",
		       ((PAGE_DATA*)(msg->param3))->PageCount, ((PAGE_DATA*)(msg->param3))->pMyJob->JobNum,  msg->param3, gPrintMgrInfo.numCredits);
        pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);
        pmpage->page_obj = NULL;
	    MEM_FREE_AND_NULL(pmpage);
    }

    PrintMgrSendMsg(JOBMGRID, msgType, 0, 0, pMostRecentlyDeliveredPage);  // SHEET done
} // end PrintMgrHandlePageDeliveryDone()


/******************************************************************************
 * Function Name: PrintMgrDoStallCheck
 */
/**
 * \brief Logic to check to see if we can print right away, or whether we
 *        need to "stall" and prompt the user to load paper, etc.
 *
 * \date 4/2/2004
 *
 * <pre>
 *  Actions:
    ACTION_START:
         Page just got to page manager. See if we can print it without stalling
         Possible stall reasons:
            STALL_PAPEROUT - no paper in any trays or in the selected tray
            STALL_SIZETYPE - size or type does not match in the selected tray
            STALL_MANUALDUPLEX - first page of duplex side of job, wait for user input
                                 (from PC, ctrl panel or engine status)
            STALL_MANUALFEEDJOB - manual feed, wait for user input (from PC, ctrl panel
                                  or engine status)
            STALL_CLEANINGPAGE - cleaning page, wait for user input
                                 (from PC, ctrl panel or engine status)
            STALL_OUTPUT_BIN_FULL - output tray/bin full; wait for it to be cleared.
         We store the stall reason, tray, size and type and send a user input event
         We can get any of these actions as a result of either stall:
            ACTION_CONTINUE_YES
            ACTION_CONTINUE_NO
            ACTION_CONTINUE_PAPERADDED
            ACTION_CONTINUE_OUTPUT_BIN_CLEARED

    ACTION_CONTINUE_YES:
         1. User selected continue with available media from the stall above -
            a. If STALL_PAPEROUT or STALL_SIZETYPE we set the input tray to any and
               run auto select again to see if we can find another tray to print from.
               If no good tray found we search again for any tray with paper in it and print.
               If none of this works we stall again.
            b. If STALL_MANUALDUPLEX - clear stall continue printing
            c. If STALL_MANUALFEEDJOB - clear stall continue printing
            d. If STALL_CLEANINGPAGE - clear stall continue printing

    ACTION_CONTINUE_NO:
         1. User selected cancel job for stall from ACTION_START above.  We stall with
            reason: STALL_CANCELJOB. The print mgr thread waits for the cancel to complete
            then clears the stall and puts the system back online

         2. User selected NO to the reconfigure stall. We clear the stall and print

    ACTION_CONTINUE_PAPERADDED:
         User added paper in response to stall from ACTION_START. We redo auto select
         to see if we can print. If we can print we clear the stall, else we set either
         STALL_PAPEROUT or STALL_SIZETYPE

    ACTION_CONTINUE_OUTPUT_BIN_CLEARED
         User cleared paper in response to STALL_OUTPUT_BIN_FULL.
         We redo stall check to see if we can print.
 * </pre>
 *
 ******************************************************************************/
STALLREASON PrintMgrDoStallCheck(PAGE_DATA* pPageObject, STALLACTION action)
{
    bool stall = FALSE;
    bool done = FALSE;
    bool startOverride = FALSE;     //set to true if override settings selected
    printer_setup_t pSetup;
    mediacnfg_t pMediaCnfg;
    STALLCHECKDATA stallData;
	int i;

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrDoStallCheck: enter\n");
    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: PrintMgrDoStallCheck(pPageObject=0x%08X, STALLACTION=%s)\n",
                 pPageObject, ToString_STALLACTION(action));

    stallData.manualDuplexTray = TRAY_ANY;
    stallData.curPageReason = STALL_NONE;
    stallData.havePriority = FALSE;
    stallData.isMPTray = FALSE;
    stallData.useAnyAvailable = FALSE;
    stallData.pageState = 0;
    stallData.manualTray = TRAY_NONE;

    printvar_get_printersetup(&pSetup);
    printvar_get_trayconfig(&pMediaCnfg);

	for(i=0;i<NUM_INPUTS;i++)
	{
		if(pPageObject->pMyJob->JobInfo == COPYJOB)
		{
			pMediaCnfg.mediaSize[i] = pPageObject->pMyJob->CopyTraySettingSize[i];
			DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: TraySize for CopyJob [%d] %d\n",i, pMediaCnfg.mediaSize[i]);
		}
		if(pMediaCnfg.mediaSize[i] == 554)		// 554 : Auto Paper Size Det
		{
			//DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: mediaSize[%d] : %d > %d \n",i ,pMediaCnfg.mediaSize[i], gEngineInfo.machineTraySize[i]);
			pMediaCnfg.mediaSize[i] = gEngineInfo.machineTraySize[i];
		}
		//pMediaCnfg.mediaType[i] = MEDIATYPE_ANY;		// TODO :
	}
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: Check pMediaCnfg Size MPT : %d, Tray1 : %d, Tray2 : %d, Tray3 : %d (def tray : %d)\n"
	, pMediaCnfg.mediaSize[TRAY_1], pMediaCnfg.mediaSize[TRAY_2], pMediaCnfg.mediaSize[TRAY_3], pMediaCnfg.mediaSize[TRAY_4], pMediaCnfg.defaultSourceTray);

    /**
     * Before we can send a page to the engine we have to figure out the size and
     * type of paper and what tray to pull it from. The driver can give us any
     * combination of this information. Trays can be configured to ANY for size and
     * type in which case they are considered a wildcard when doing matching.
     *
     * <pre>
     *    TRAY(TR)  SIZE(S)  TYPE(T)
     * ===========================
     * 1. ANY       value    ANY
     * 2. ANY       value    value
     * 3. ANY       ANY      ANY
     * 4. ANY       ANY      value
     * 5. value     value    ANY
     * 6. value     value    value
     * 7. value     ANY      ANY
     * 8. value     ANY      value
     * </pre>
     *
     * If no tray is given (1-4) and S or T is ANY, we set them to the default
     * configured S and T and then try to find a tray that best matches.
     *
     * If a tray is given (5-8) and S or T is ANY, we set them to the values
     * configured in the tray. If the value in the tray is ANY then we use the
     * default configured S and T.
     *
     * If we can't find a matching tray we set the tray to ANY and redo the check.
     */
     if(pPageObject->force_default_media_size == 1)		// ignore pPageObject->PAPER and 'use default media size' (A4 : Korea/China, LT : NA)
     {
		 DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: Force Default Media Size : Paper = %d Location = %d !!!\n", pPageObject->PAPER, gEngineInfo.machineLocation);
		 if(gEngineInfo.machineLocation == LOCAT_NA_LT)
		 {
			 pPageObject->PAPER = MEDIASIZE_LETTER;
		 }
		 else
		 {
			 pPageObject->PAPER = MEDIASIZE_A4;
		 }
	 }

	 // in some case, the requested paper size set with MEDIASIZE_A4_ROTATED (A4_LEF), but actual size is the MEDIASIZE_A4 (A4_SEF).
	 if(pPageObject->PAPER == MEDIASIZE_A4_ROTATED)
	 {
		 pPageObject->PAPER = MEDIASIZE_A4;
	 }

    uint32_t width, length;
    paper_get_media_size( pPageObject->PAPER, &width, &length);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: Check Image Size - Paper : %d, width : %d, length : %d\n", pPageObject->PAPER, width, length);

    if ( pPageObject->fRotate_oneup_image != 1 )
    {
		if((pPageObject->PAPER == MEDIASIZE_A3) || (width > 21600) || (length > 35600))
		{
			if((width > PRINTABLE_MAX_WIDTH) || (length > PRINTABLE_MAX_LENGTH))
			{
				stall = TRUE;
				done = TRUE;
				stallData.curPageReason = STALL_CANCELJOB;
			}
		}
	}

    //Use the driver settings when starting a stall check on a page. Use the settings
    //we stalled on for all others
    if(action == ACTION_START)
    {
		stallData.curPageSize = pPageObject->PAPER;
        stallData.curPageType = pPageObject->mediaType;
        stallData.curPageTray = pPageObject->DEFAULTSOURCE;
        stallData.curPageOutTray = pPageObject->requestedOutTray;

        DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check: JobID = %d, PageID = %d\n", pPageObject->pMyJob->JobNum, pPageObject->PageCount);

        if( (gEngineInfo.fDrumLowMode_3 == true) && (gEngineInfo.lastjobnum != pPageObject->pMyJob->JobNum)
        && (((pPageObject->PageCount == 1) && ((GetAutoDuplex(pPageObject->DUPLEX)) == FALSE)) || ((pPageObject->PageCount == 2) && ((GetAutoDuplex(pPageObject->DUPLEX)) == TRUE))) )
        {
			DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall : STALL_DRUM_NEAR_END\n");

			stallData.curPageReason = STALL_DRUM_NEAR_END;

			gStallInfo.reason = stallData.curPageReason;
			gStallInfo.tray = stallData.curPageTray;
			gStallInfo.outTray = stallData.curPageOutTray;
			gStallInfo.size = stallData.curPageSize;
			gStallInfo.type = stallData.curPageType;

			gEngineInfo.lastjobnum = pPageObject->pMyJob->JobNum;

			return stallData.curPageReason;
		}

		gEngineInfo.lastjobnum = pPageObject->pMyJob->JobNum;
        //if have override then at some point in this job we stalled for a tray
        //and the user selected the override. We need to continue to use the
        //override settings as long as they are the same as the orginal request that
        //resulted in the override.
        if(gOverrideInfo.haveOverride == TRUE)
        {
            if(stallData.curPageSize == gOverrideInfo.requestedSize &&
               stallData.curPageType == gOverrideInfo.requestedType &&
               stallData.curPageTray == gOverrideInfo.requestedTray &&
               stallData.curPageOutTray == gOverrideInfo.requestedOutTray)
            {
                DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check: use overrides: raw overrideTRAYENUM=%d; overrideOutTRAYENUM=%d\n",
                             gOverrideInfo.overrideTray, gOverrideInfo.overrideOutTray);
                stallData.curPageSize = gOverrideInfo.overrideSize;
                stallData.curPageType = gOverrideInfo.overrideType;
                stallData.curPageTray = gOverrideInfo.overrideTray;
                stallData.curPageOutTray = gOverrideInfo.overrideOutTray;
            }
            else
            {
                DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check: haveOverride but changed: raw curPageTRAYENUM=%d; requestedTRAYENUM=%d\n",
                              stallData.curPageTray, gOverrideInfo.requestedTray);
                DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check: haveOverride but changed: raw curPageOutTRAYENUM=%d; requestedOutTRAYENUM=%d\n",
                             stallData.curPageOutTray, gOverrideInfo.requestedOutTray);
            }
        }

        //special case, make sure the requested tray is present, if not, set to any
        if(stallData.curPageTray < NUM_INPUTS && !(pSetup.inputTrays[stallData.curPageTray].inputPresent))
        {
            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check: requested tray not present: %ld\n",
                         stallData.curPageTray+1 );
            pPageObject->DEFAULTSOURCE = TRAY_ANY;
            stallData.curPageTray = TRAY_ANY;
        }

        // Special case: make sure the requested output tray is present.
        // If not, set to default.
        // REVISIT TODO: OutTray logic needs work!
        if ( !(stallData.curPageOutTray < NUM_OUTPUTS) ||
             !(pSetup.outputTrays[stallData.curPageOutTray].outputPresent) )
        {
            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check: requested output tray not present! Raw trayenum_t=%ld. Force to TRAY_1.\n",
                         stallData.curPageOutTray );
            pPageObject->requestedOutTray = TRAY_1;
            stallData.curPageOutTray = TRAY_1;
        }
    }
    else // (action != ACTION_START)
    {
        stallData.curPageSize =   gStallInfo.size;
        stallData.curPageType =   gStallInfo.type;
        stallData.curPageTray =   gStallInfo.tray;
        stallData.curPageOutTray = gStallInfo.outTray;
        DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check: not start: raw curPageTRAYENUM=%d; curPageOutTRAYENUM=%d\n",
                     stallData.curPageTray, stallData.curPageOutTray);
    }

    //see if we have a manual feed tray and whether it is a priority tray
    //manual feed and priority are treated differently than standard trays
    //(we only say we have priority if it has paper in it)
    PrintMgrDetectManualTray(&pSetup, &stallData.manualTray, (bool *)&stallData.havePriority, (bool *)&stallData.isMPTray);


    switch(action)
    {
        case ACTION_START:
            //start stall check for this page

            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check start: tray: %ld, size: %ld, type: %ld\n",
                         stallData.curPageTray+1, stallData.curPageSize, stallData.curPageType);

            /*
             * Look for the manualduplex flag. it will be set in the first page
             * of duplex part of the manual duplex job. The ManDup2ndPg flag is set
             * on all the pages that are in the back side of the job.
             *
             * Let 0 mean neither flag is set, 0x01 be the manual duplex flag and
             * 0x02 be the ManDup2ndpage flag.
             * 1. A normal 4 page manual duplex job looks like this:   0, 0, 3, 2
             * 2. A 2 page manual duplex job with 2 copies looks like: 0, 0, 3, 3
             *    (page 1 is sent twice and then page 2 twice)
             * In both cases we only want to stall on the first 3. To do this we keep
             * track of gPrintMgrInfo.receivedFirstManualDuplexSecondSide.
             */

            //DPRINTF(PMDEBUG, ("PM: PrintMgrDoStallCheck ACTION_START: ManualDuplex=%d, ManDup2ndPg=%d\n", pPageObject->ManualDuplex, pPageObject->ManDup2ndPg));

            if (pPageObject->ManDup2ndPg)
            {
#if !defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)
                // Force all ManualDuplex second sides to come from designated ManualDuplex tray.
                PrintMgrDetectManualDuplexTray( &pSetup, &stallData.manualDuplexTray );
                stallData.curPageTray = gStallInfo.tray = stallData.manualDuplexTray;
#endif // !defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)

                stallData.curPageSize = pPageObject->PAPER; // Bugzilla # 6947.

                if ( pPageObject->ManualDuplex && (gPrintMgrInfo.receivedFirstManualDuplexSecondSide == 0))
                {
                    // this is the page we need to stall on and wait for user to press go button

                    // record the job number of the job we stalled on
                    gPrintMgrInfo.receivedFirstManualDuplexSecondSide = pPageObject->pMyJob->JobNum;

#if defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)
                    // If the tray is not given in the page
                    // use the same tray as the last page sent.
                    // Otherwise we don't know what tray to prompt for.
                    //
                    // By definition of the ManualDuplex protocol, this page
                    // should be asking for the ManualFeed tray, but we will
                    // go with whatever tray it is asking for.
                    if (stallData.curPageTray == TRAY_ANY)
                        stallData.curPageTray = gLastPage.tray;
#endif // defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)

                    DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: start manual duplex using raw trayenum_t %d\n",
                                 stallData.curPageTray);
                    stallData.curPageReason = STALL_MANUALDUPLEX;
                    stall = TRUE;
                    done = TRUE;
                }
                else // if ( pPageObject->ManualDuplex == 0 )
                {
                    // This is the subsequent pages being printed on the second side

#if defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)
                    // For subsequent second sides, always use the same tray as
                    // the last page sent.
                    // This makes Tray Override effective with ManualDuplex.
                    stallData.curPageTray = gLastPage.tray;
#endif // defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)

                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: continue manual duplex using raw trayenum_t %d\n",
                                 stallData.curPageTray);
                    stall = FALSE;
// TODO: setting done here is not necessarily correct as it causes us to miss
// paper-out and other error conditions encountered on the second side of a
// manual duplex job.  Additionally, setting done in the first case above may
// not be correct either because if the user hits GO when there is no paper in
// the tray and we're waiting for the manual duplex second side, it may cause
// us to drop the job.
//                  done = TRUE;
                } // end else // if ( pPageObject->ManualDuplex == 0 )
            } // end if (pPageObject->ManDup2ndPg)

            break;

        case ACTION_CONTINUE_YES:
            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall check continue\n");

            //user hit the continue button. see what to do
            if ( (gStallInfo.reason == STALL_MANUALDUPLEX) ||
                 (gStallInfo.reason == STALL_MANUALFEEDJOB) ||
                 (gStallInfo.reason == STALL_CLEANINGPAGE) )
            {

                //on manual feed or manual duplex first see if there is
                //a priority tray and the user put paper in it if so use it
                if(stallData.havePriority)
                {
                    stallData.curPageTray = stallData.manualTray;
                    stall = FALSE;
                    done = TRUE;
                }
                //Engines with a single multipurpose tray are used both as
                //the input tray and the manual feed tray. Manual feed is
                //indicated by the tray being set to Any. These trays don't
                //have a paper sensor so we can't tell if paper is present, so
                //continue.
                else if (TRAY_ANY == stallData.curPageTray)
                {
                    stall = FALSE;
                    done = TRUE;
                }
                // For all others only continue if paper is present.
                else if (pSetup.inputTrays[stallData.curPageTray].paperPresent)
                {
                    stall = FALSE;
                    // Do *NOT* set: done = TRUE; force through PrintMgrResolve() to inherit type, etc.
                }
                //no priority tray with paper in it and no paper in stalled tray
                else
                {
                    // For manual duplex we try to get the user to print the second side
                    // from the same tray as the first side. So survey the situation.
                    if(gStallInfo.reason == STALL_MANUALDUPLEX)
                    {
                        //stallData.curPageReason = STALL_PAPEROUT; leave as STALL_MANUALDUPLEX
                        stallData.curPageSize = pPageObject->PAPER; // Bugzilla # 6947.
#if defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)
                        // The user has pressed the [OK] button to tell us to
                        // start printing the second sides when we are stalled
                        // for ManualDuplex, yet the normal ManualDuplex tray
                        // is empty. If there is paper in the tray we used for
                        // the first side, then assume the user has placed the
                        // output stack in the same tray, so use that tray.
                        // Otherwise, start override mode.
                        if (pSetup.inputTrays[gLastPage.tray].paperPresent)
                        {
                            stallData.curPageTray = gLastPage.tray;
                            stall = FALSE;
                            // Do *NOT* set: done = TRUE; force through PrintMgrResolve() to inherit type, etc.
                            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: user pressed [OK]; manual duplex using raw trayenum_t %d\n",
                                         stallData.curPageTray);
                        }
                        else // No paper in gLastPage.tray, either.
                        {
                            // Start override mode.
                            //stallData.curPageTray = TRAY_ANY;
                            //stallData.useAnyAvailable = TRUE;
                            startOverride = TRUE;
                            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: user pressed [OK]; manual duplex using override\n");
                        } // end else // No paper in gLastPage.tray, either.
#else  // !defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)
                        // Force all ManualDuplex second sides to come from designated ManualDuplex tray.
                        PrintMgrDetectManualDuplexTray( &pSetup, &stallData.manualDuplexTray );
                        stallData.curPageTray = stallData.manualDuplexTray;
                        stall = TRUE;
                        // Do *NOT* set: done = TRUE; force through PrintMgrResolve() to inherit type, etc.
#endif // !defined(MANUAL_DUPLEX_CAN_USE_ANY_TRAY)
                    }
                    // For manual feed or cleaning page,
                    //   if user hit continue and no paper present
                    //     then set tray to any and redo stall check.
                    else
                    {
                        //stallData.curPageTray = TRAY_ANY;
                        //stallData.useAnyAvailable = TRUE;

                        //set this flag so if are able to find an override tray we
                        //will save the settings and continue to use them for all subsequent
                        //pages in the job, if we stall then we didn't find a good override
                        //so don't save them
                        startOverride = TRUE;
                    }

                }
            }
            else if(gStallInfo.reason == STALL_SIZETYPE ||
                    gStallInfo.reason == STALL_PAPEROUT)
            {
				pPageObject->pMyJob->mismatch_continue = 1;
                // 20060530: Bug 1601 in bugzilla
                //For engines with no paper sensor and a single multi purpose tray, continue means paper
                //was added not override. We want to stall here while the engine marks paper added and
                //resets the engine. The reset will cause an initializing status which will restart the page.
                //Overriding in this case causes us to select the manual feed slot and stall for manual feed.
                //This results in the user having to press the button twice on paper out. First push for paper
                //out and second push for manual feed.
                if(gStallInfo.reason == STALL_PAPEROUT &&
                   pSetup.options & PRINTER_NO_PAPER_SENSOR &&
                   pSetup.inputTrays[TRAY_1].properties & INPUTPROPERTY_MULTIPURPOSE)
                {
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: paper added manually\n");

                }
                else
                {
                    //paperout or sizetype
                    //on continue we are supposed to override the paper size and do
                    //auto select again, I set tray to any so we will try all trays
                    //if we find an override match we are supposed to continue using
                    //the override for all pages in the job until:
                    // 1. end of job
                    // 2. different size requested
                    // 3. different type requested
                    // 4. override tray runs out of paper
                    // 5. the tray size or type is reconfigured
                    //If we can't find a match using the auto select logic we are
                    //supposed to print from any tray that has paper in it
                    //stallData.curPageTray = TRAY_ANY;
                    //stallData.useAnyAvailable = TRUE;

                    //set this flag so if are able to find an override tray we
                    //will save the settings and continue to use them for all subsequent
                    //pages in the job, if we stall then we didn't find a good override
                    //so don't save them
                    startOverride = TRUE;
                    DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"PM: sizetype : startOverride = TRUE\n");
                }
            }
            //else reconfigure, try again with stall info from last time
            break;

        case ACTION_CONTINUE_NO:
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall check cancel\n");
            if(gStallInfo.reason == STALL_RECONFIGURE)
            {
                stallData.curPageSize = pPageObject->PAPER;
                stallData.curPageType = pPageObject->mediaType;
                stallData.curPageTray = pPageObject->DEFAULTSOURCE;
                stallData.curPageOutTray = pPageObject->requestedOutTray;
            }
            else
            {
                //send cancel job to status manager
                stall = TRUE;
                done = TRUE;
                stallData.curPageReason = STALL_CANCELJOB;
            }
            break;

        case ACTION_CONTINUE_PAPERADDED:
            //if we were waiting for paper and paper was added in some tray
            //then redo the autoselect and see if it passes
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall check paper added\n");

            /**
             * Only respond to paperadded if we were stalled for paper out, or
             * for reconfigure. On all others we wait for user input.
             */
            if ( (gStallInfo.reason == STALL_PAPEROUT) ||
                 (gStallInfo.reason == STALL_RECONFIGURE) )
            {
                //if original request was to any tray, set back to any and redo select instead
                //of using the tray we guessed on
                if(gStallInfo.tray != pPageObject->DEFAULTSOURCE)
                {
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: paper added switching to any tray\n");
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: because raw trayenum_t (gStallInfo.tray=%d != pPageObject->DEFAULTSOURCE=%d)\n",
                                 gStallInfo.tray, pPageObject->DEFAULTSOURCE);
                    stallData.curPageSize = pPageObject->PAPER;
                    stallData.curPageType = pPageObject->mediaType;
                    stallData.curPageTray = pPageObject->DEFAULTSOURCE;
                    stallData.curPageOutTray = pPageObject->requestedOutTray;
                }
            }
            else
            {
                stall = TRUE;
                done = TRUE;
                stallData.curPageReason = gStallInfo.reason;
            }
            break;

        case ACTION_CONTINUE_OUTPUT_BIN_CLEARED:
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: do stall check: output tray/bin cleared.\n");

            /**
             * Only respond to output tray/bin cleared if we were stalled for
             * bin full. On all others we continue the current stall reason.
             */
            if (gStallInfo.reason != STALL_OUTPUT_BIN_FULL)
            {
                stall = TRUE;
                done = TRUE;
                stallData.curPageReason = gStallInfo.reason;
            }
            break;

        default:
            DBG_PRINTF(LOG_ERR|DBG_PRINTMGR,"PM: stall error, bad state; out of range. STALLACTION=%s\n",
                         ToString_STALLACTION(action));
            break;

    } // end switch (action)

    //special case for single multipurpose tray devices. Always set tray to what was
    //sent by the driver.  For products with multipurpose trays the driver
    //will set the tray to auto for normal tray and to tray1 for manual duplex.
    //Our stall logic always picks a tray before stalling. On continue from the stall if we
    //don't reset to driver settings we will have switched a normal page to a manual feed page
#ifdef HAVE_PRINT_MECH
    if (   ( eng_get_have_only_one_physical_input_tray() ) // was ( NUM_INPUTS == 1 )
        && ( pSetup.inputTrays[TRAY_1].properties & INPUTPROPERTY_MULTIPURPOSE ) )
    {
        DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: Stall reset tray to: %d\n", pPageObject->DEFAULTSOURCE);
        stallData.isMPTray = TRUE;
        stallData.curPageTray = pPageObject->DEFAULTSOURCE;
        stallData.pageState &= ~HaveTray;
    }
#endif

    //figure out which settings we have given
    if ( (stallData.curPageSize != MEDIASIZE_ANY) && (stallData.curPageSize != MEDIASIZE_UNKNOWN) )
        stallData.pageState |= HaveSize;
    if ( (stallData.curPageType != MEDIATYPE_ANY) && (stallData.curPageType != MEDIATYPE_UNKNOWN) )
        stallData.pageState |= HaveType;
    if(stallData.curPageTray != TRAY_ANY && stallData.curPageTray != TRAY_NONE)
        stallData.pageState |= HaveTray;
    if (stallData.curPageOutTray < NUM_OUTPUTS)
        stallData.pageState |= HaveOutTray;

    if(!done)
    {
        //run the magic resolver to figure out all the size, type and tray stuff and
        //figure out the stall if can't resolve it
        stall = PrintMgrResolve(&pSetup, &pMediaCnfg, &stallData, pPageObject, &gLastPage);
        if(stall != FALSE)
			stallData.curPageReason = STALL_SIZETYPE;
    }

    if(stall == FALSE)
    {
        //save the settings from the last page sent to the engine
        //we need this information for future page stall checks
        gLastPage.size = stallData.curPageSize;
        gLastPage.tray = stallData.curPageTray;
        PrintMgrSetLastPreviousNonDuplexInputTRAYENUM(gLastPage.tray);
        gLastPage.type = stallData.curPageType;
        gLastPage.outTray = stallData.curPageOutTray;

        // Added check for single page feed. If the printer has a single page manual feed then we need to delay until paper
        // has completed it's feed. The isSingleFeed flag is check later in PrintMgrPrintPages to determine if it should
        // wait before it starts the next manual feed page.
        if(stallData.curPageTray == stallData.manualTray && pSetup.inputTrays[stallData.curPageTray].properties & INPUTPROPERTY_SINGLE_FEED)
        {
            gLastPage.isSingleFeed = TRUE;
            DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: sent manual job to engine\n");
        }
        else
        {
            gLastPage.isSingleFeed = FALSE;
        }

        //user hit override on load paper stall and we found a suitable tray
        //to use. store the original requested settings and the override settings
        //we will continue to use the overrides for all subsequent pages of the job so
        //user won't have to keep hitting override
        if(startOverride == TRUE)
        {
            gOverrideInfo.haveOverride = TRUE;
            gOverrideInfo.requestedSize = pPageObject->PAPER;
            gOverrideInfo.requestedType = pPageObject->mediaType;
            gOverrideInfo.requestedTray = pPageObject->DEFAULTSOURCE;
            gOverrideInfo.requestedOutTray = pPageObject->requestedOutTray;
            gOverrideInfo.overrideSize = stallData.curPageSize;
            gOverrideInfo.overrideType = stallData.curPageType;
            gOverrideInfo.overrideTray = stallData.curPageTray;
            gOverrideInfo.overrideOutTray = stallData.curPageOutTray;

            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: start using overrides: raw requestedTRAYENUM=%d; overrideTRAYENUM=%d\n", \
                         gOverrideInfo.requestedTray, gOverrideInfo.overrideTray);
        }

        pPageObject->engTray = stallData.curPageTray;
        pPageObject->engEmptyTray = stallData.curEmptyTray;
        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: engTray : %d %d\n", pPageObject->engTray, pPageObject->engEmptyTray);
        pPageObject->engOutTray = stallData.curPageOutTray;
        pPageObject->engMediaType = stallData.curPageType;
        pPageObject->engMediaSize = stallData.curPageSize;
        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: stall check no stall\n");

        PrintMgrClearStall();
    }
    else // stall
    {
        //increment the stall count everytime we stall, this is used in
        //timed override. We set up the timed notify with the current stallcount
        //then compare that with the current count when notified to make sure
        //we don't override the current stall with a notify from a previous stall
        gStallInfo.stallCount++;

        //anytime we stall while overrides are enabled means our
        //overrides didn't work so disable them
        gOverrideInfo.haveOverride = FALSE;

        gStallInfo.reason = stallData.curPageReason;
        gStallInfo.tray = stallData.curPageTray;
        gStallInfo.outTray = stallData.curPageOutTray;
        gStallInfo.size = stallData.curPageSize;
        gStallInfo.type = stallData.curPageType;
        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: stall check, stalled\n");
        DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: reason: %s, raw trayenum_t: %ld\n",
                     ToString_STALLREASON(stallData.curPageReason), stallData.curPageTray);
        DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: size: %ld, type: %ld\n",
                     stallData.curPageSize, stallData.curPageType);

    }

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrDoStallCheck: exit\n");
    return stallData.curPageReason;
} // end PrintMgrDoStallCheck()


/**
 * \brief Clear a forced stall.  This routine makes sure that all critical
 * stall settings are cleaned up when we clear a forced stall.
 */
void PrintMgrClearForcedStall()
{
    DPRINTF( (PMDEBUG), ("PM: PrintMgrClearForcedStall\n"));

    gForcedStallInfo.state = FORCED_NONE;
    gForcedStallInfo.statusCode = 0;
    gForcedStallInfo.isPreError = FALSE;
    gForcedStallInfo.isUserAck = FALSE;
    gForcedStallInfo.pagePtr = 0;
}

/******************************************************************************
 * Function Name: PrintMgrHandleForcedStall
 */
/**
 * \brief Run the forced stall state machine.
 *
 * \date 8/16/2004
 *
 * <pre>
 * Description: Run the forced stall state machine.
 *    These are the types of forced stalls due to errors that
 *    we handle:
 *    1. UserInput(no ack)- this means we will stall and display a prompt to let the user
 *       know an error occurred. If the error is cleared, the prompt will go away without
 *       requiring user input.
 *
 *    2. UserInput(ack) -  this is the same as #1 except the prompt will be displayed
 *       until the user hits continue to acknowledge it.
 *
 *    3. UserInput preerror - this is a wierd one that was added specifically for
 *       unexpected size errors but may be used for other errors at some point.
 *       Unexpected size occurs at the end of the print page. The page is done
 *       and will exit the printer. We don't show this error immediatly. We wait
 *       and look at the next page. If it has the exact settings as the page
 *       that caused the error we will do the forced stall.  We clear the stall
 *       if the user hits continue, cancel, or modifies the tray config
 * </pre>
 *
 *  Bug Fix: scenario - we detect the forced stall status. We send the
 *  hidden cancel status to force the page to cancel. When we detect the
 *  cancel we report the real error status to the user.
 *  - Problem 1 - If the page done message is sent and gets to the job mgr
 *  before the status manager sees the cancel status and schedules a cancel,
 *  the page will be deleted before the cancel. The job mgr
 *  will not send the cancel message (nothing to cancel).
 *  If this happens, we get stuck in the hidden error state.
 *  - Solution1 - We modified the code to treat a page done just like the cancel
 *  to move us into the forced stall state.
 *  - Problem2 - Now, however, if the job mgr sends the cancel
 *  but we get a page done from the engine just before, we will start the
 *  stall and move to forced active. Then we see the cancel which clears
 *  the stall.
 *  - Solution2 - do not clear stall on cancel if forced stall is active
 *  state
 *
 ******************************************************************************/
STALLREASON PrintMgrHandleForcedStall(MESSAGE* msg)
{
    STALLREASON result = STALL_NONE;   //default is no stall
    bool startStall = FALSE;
    PAGE_DATA* pPageObject;

    //DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrHandleForcedStall: enter\n");
    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: forced stall info %x\n",gForcedStallInfo.state);
    switch(gForcedStallInfo.state)
    {
        case FORCED_NONE:
            //no forced stall pending, we are done
            break;

        case FORCED_WAITCANCEL:
            //detected a forced stall. Don't start it until after
            //the page finishes canceling. We don't know if the page will
            //restart or not but we do want to be sure it is cancelled
            // Note: we won't get a cancel for the page if the engine sends a
            //  page done in between the time the status is reported and before
            //  the job manager processes the cancel so look for pagedone here and
            //  treat just like the cancel
            if ( (msg->msgType == MSG_CANCELPAGE) ||
#if PAGEDONE_PROTOCOL_MAY_USE_PAGEVIDEODONE_THEN_PAGEDELIVERYDONE
                 (msg->msgType == MSG_PAGEVIDEODONE) ||
                 (msg->msgType == MSG_PAGEDELIVERYDONE) ||
#endif // PAGEDONE_PROTOCOL_MAY_USE_PAGEVIDEODONE_THEN_PAGEDELIVERYDONE
                 (msg->msgType == MSG_PAGEDONE) )
            {
                //if not a preerror then force the stall (preerrors don't happen until the
                //next page start
                if(gForcedStallInfo.isPreError == FALSE)
                {
                    //We use the hidden status codes to cancel the page when we detect
                    //a user input error. While waiting for the page cancel we don't send
                    //any status updates to the status manager (except for other non
                    //user input errors). Here if the status is still the hidden category
                    //then no other error more important has occurred. So start the
                    //forced stall and send the user input status
                    if(STATCATCMP(STATCAT_HIDDEN, gPrintMgrInfo.status))
                    {
                        //status hasn't changed so start the stall
                        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: page cancel done, start forced stall\n");
                        gForcedStallInfo.state = FORCED_ACTIVE;

                        DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 1 set stall info state to ACTIVE\n");
                        startStall = TRUE;
                        result = STALL_FORCED;
                    }
                    //else some other error occurred so clear the force.
                    else
                    {
                        DBG_PRINTF(LOG_NOTICE|DBG_PRINTMGR,"PM: cancel done, forced stall cleared\n");

                        PrintMgrClearForcedStall();
                    }
                }
                else
                {
                    //this is a preerror goto the wait for page start state.
                    //return pending
                    result = STALL_PENDING;
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: cancel done, wait for next page for force\n");
                    gForcedStallInfo.state = FORCED_WAITPAGE;
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 1 set stall info state to FORCED_WAITPAGE\n");
                }
            }
            else
            {
                //waiting for page cancelled message, return pending
                result = STALL_PENDING;
            }
            break;

        case FORCED_WAITPAGE:
            //We only come into this state for preerror's.  Wait for page start message
            if(msg->msgType == MSG_PAGESTART)
            {
                pPageObject = (PAGE_DATA*)msg->param3;
                ASSERT(pPageObject != NULL);
                ASSERT(IS_PAGE_DATA(pPageObject));

                //If this page has the same requested settings as the one that caused the
                //error then assume it will end up with the same final settings and cause
                //the same error
                if(gForcedStallInfo.requestedMediaSize == pPageObject->PAPER &&
                   gForcedStallInfo.requestedMediaType == pPageObject->mediaType &&
                   gForcedStallInfo.requestedTray == pPageObject->DEFAULTSOURCE &&
                   gForcedStallInfo.requestedOutTray == pPageObject->requestedOutTray)
                {
                    startStall = TRUE;
                    result = STALL_FORCED;
                    gForcedStallInfo.state = FORCED_ACTIVE;
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 2 set stall info state to ACTIVE\n");
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: start size preerror forced stall\n");
                }
                else
                {
                    //we can clear the forced error by setting the state to none
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: clear preerror forced stall\n");
                    PrintMgrClearForcedStall();
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 3 set stall info state to NONE\n");
                }
            }
            //data store notify means user changed the tray configuration. Give user benefit
            //of doubt and assume they corrected the problem
            else if(msg->msgType == MSG_TRAYCONFIG)
            {
                if(gForcedStallInfo.isUserAck == FALSE)
                {
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: forced stall cleared, config changed\n");
                    PrintMgrClearForcedStall();
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 4 set stall info state to NONE\n");
                    result = STALL_FINISHED;
                }
                else
                {
                    result = STALL_PENDING;
                }
            }
            else
            {
                result = STALL_PENDING;
            }
            break;

        case FORCED_ACTIVE:
        case FORCED_FAX:
            //stall is active see if incoming message clears it
            //Messages we are looking for:
            // user input
            // status update
            if(msg->msgType == MSG_USER_INPUT)
            {
                if(msg->param1 == PARAM_USER_OK)
                {
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall, got user input ok\n");
                    //user hit ok, send error recover to engine to clear any
                    //errors and clear the stall
                    PrintMgrClearForcedStall();
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 4 set stall info state to NONE\n");
                    result = STALL_FINISHED;
                    // dab PrintMgrSendMsg(ENGINEID, MSG_USER_ERR_RECOVERY, 0, 0, 0);
                    PrintMgrSendMsg(ENGINEID, MSG_USER_ERR_RECOVERY, msg->param1, msg->param2, msg->param3);
                }
                else
                {
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall, got user input cancel\n");
                    //user said cancel page
                    //send cancel to jobmgr
                    //return pending so won't try to print
                    //clear stall
                    result = STALL_PENDING;
                    PrintMgrClearForcedStall();
                    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 5 set stall info state to NONE\n");
                    PrintMgrSendMsg(SJMID, MSG_CANCELPAGE, SYS_REQUEST, PRINTMGRID,(void*)e_Printer);
                }
            }
            else if (msg->msgType == MSG_PAPER_ADDED) // paper out cleared during job-cancel
            {
              DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: paper added, forced stall cleared\n");
              PrintMgrClearForcedStall();
              DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: set stall info state to FINISHED\n");
              result = STALL_FINISHED;
            }
            else if(msg->msgType == MSG_PRINT_STATUS_EVENT)
            {
                //the test for forced stall function blocks all status events from the
                //engine and video except the ones we are interested in while stalling.
                //So if we get an update here we should clear the stall
                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall cleared, status changed\n");
                PrintMgrClearForcedStall();
                DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: 6 set stall info state to NONE\n");
                result = STALL_FINISHED;

            }
            else
            {
                result = STALL_PENDING;
            }

            break;
    }

    if(startStall == TRUE)
    {
        gStallInfo.reason = result;
        gStallInfo.size = gForcedStallInfo.errorMediaSize;
        gStallInfo.type = gForcedStallInfo.errorMediaType;
        gStallInfo.tray = gForcedStallInfo.errorTray;
    }

    //DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrHandleForcedStall: exit\n");
    return result;
}


/******************************************************************************
 * Function Name: PrintMgrCheckForForcedStall
 */
/**
 * \brief Check the status code for a forced stall.
 *
 * \date 8/16/2004
 *
 * Also determine
 * if we should forward the status onto the status manager
 * based on the stall state. We want to ignore some status
 * messages from video or engine if we are stalling.
 ******************************************************************************/
bool PrintMgrCheckForForcedStall(uint32_t* statusCode)
{
    ATLISTENTRY* node;
    printmgr_page_t* pmpage;
    bool forwardStatus = TRUE; //by default forward the status
    bool isOnline = FALSE;   //true if online or non offline warning
    bool isUserInput = FALSE; //true for user input messages

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrCheckForForcedStall: enter status %x\n", *statusCode);

    if(ISONLINE(*statusCode) || ISWARNING(*statusCode))
        isOnline = TRUE;

    if(ISUSERINPUT(*statusCode))
        isUserInput = TRUE;



    //if we are already doing a forced stall we need to be selective about what
    //status we forward on
    if(gForcedStallInfo.state != FORCED_NONE)
    {
        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall info state:  %d \n", gForcedStallInfo.state);
        if(gForcedStallInfo.state == FORCED_WAITCANCEL)
        {
            //in this state we don't forward online messages or user input messages.
            //we want to stay offline until the cancel is done and pages restarted so
            //the forced stall state machine can decide what to do. And we don't want
            //to resend the user input message because we are already handling it
            forwardStatus = FALSE;
            DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: forced stall wait cancel, drop status\n");

            // This is here to ensure proper misprint control panel and engine behavior.
            // Don't drop the status if the engine is going online because
            // of the user input via control panel or host GUI.
            // If the standard "online" message is received from the engine,
            // this means that the misprint has been cleared. Forward the
            // status and clear the stall.
            if (*statusCode == STATUS_ONLINE)
            {
                forwardStatus = TRUE;

                DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: OVERRIDE INFO FORCED STALL SEND ONLINE STATUS\n");

                PrintMgrClearForcedStall();
                DPRINTF( (PMDEBUG), ("PM: Set stall info state to NONE\n"));
            }

            // Forward on important messages, this list may grow.
            // DAB Defect #3473
            if ( STATCATCMP(*statusCode, STATCAT_PAPEROUT) ||
                 STATCATCMP(*statusCode, STATCAT_PAPERJAM_AREA) ||
                 STATCATCMP(*statusCode, STATCAT_PAPERJAM_TYPE) ||
                 STATCATCMP(*statusCode, STATCAT_MISPRINT) ||
                 STATCATCMP(*statusCode, STATCAT_DATA_PATH) )
            {
                forwardStatus = TRUE;
            }
        }
        else if(gForcedStallInfo.state == FORCED_ACTIVE)
        {
            //in active state we need to do different things depending on the type of forced
            //stall. For user ack the user needs to acknowledge the error before we move on
            //we define acknowledge as user input. We also will clear if we get another
            //more severe error, usually the user will open and shut the front door which
            //will result in a door open and then a initializing error (we figure thats close
            //enough for a user acknowledge)
            if(gForcedStallInfo.isUserAck == TRUE)
            {
                //drop online messages and user input error messages from engine
                //The way this is set up the only user input error message we should see
                //is the same one we are handling. Since we are already handling it we
                //ignore it
                if(isOnline || isUserInput)
                {
                    forwardStatus = FALSE;
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: *2* forced stall active, drop status %d %d 0x%x\n",
                                 isOnline, isUserInput, *statusCode);
                }
                //if this error is greater than the user input error we are handling forward
                //it on
                else if(!STATSEVGTECMP(*statusCode, gPrintMgrInfo.status))
                {
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall ack active, drop lesser error\n");
                    forwardStatus = FALSE;
                }
            }
            else
            {
                //not a user ack error so we can clear the error when we see an online or
                //if we get a more severe error
                if(isUserInput)
                {
                    forwardStatus = FALSE;
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: *1* forced stall active, drop status\n");
                }
                else if(!STATSEVGTECMP(*statusCode, gPrintMgrInfo.status))
                {
                    forwardStatus = FALSE;
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall active, drop lesser error\n");
                }
            }
        }
        //else if(gForcedStallInfo.state == FORCED_WAITPAGE)
        //In FORCED_WAITPAGE state we are waiting for the next page to start to know if
        //we should stall so forward all status
    }
    //else we are not doing a stall so look for user input errors
    else if(isUserInput)
    {
        //Currently we assume user input errors occurr during printing. We also assume the
        //error is going to cause a cancel. We wait for the cancel to be complete before
        //doing the stall.
        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall request detected\n");

        gForcedStallInfo.statusCode = *statusCode;
        gForcedStallInfo.state = FORCED_WAITCANCEL;
        DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: 2 set stall info state to FORCED_WAITCANCEL\n");

        //see if user has to acknowledge the error. On some messages the user either has
        //to acknowledge it with a key press or just fix the error
        if(ISUSERACK(*statusCode))
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall isUserAck SET: 0x%x\n", *statusCode);
            gForcedStallInfo.isUserAck = TRUE;
        }
        else
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall isUserAck cleared: 0x%x\n", *statusCode);
            gForcedStallInfo.isUserAck = FALSE;
        }

        //for preerrors we don't send the status immediatly
        //we wait for the next page to start and see what its settings are
        if(ISPREERROR(*statusCode))
            gForcedStallInfo.isPreError = TRUE;
        else
            gForcedStallInfo.isPreError = FALSE;

        //we replace the real message with the hidden restart or cancel to
        //cancel the page. This happens real quick. Once the page has cancelled
        //we stall and send the original status message.
        if(ISRESTART(*statusCode))
            *statusCode = STATUS_HIDDENRESTART;
        else
            *statusCode = STATUS_HIDDENCANCEL;

        //save the settings for the currently printing page, some of the forced stalls
        //need this information for the user
        node = ATListHead(&printmgr_active_list);
        if(node)
        {
            pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);
            PAGE_DATA* pPageObject = pmpage->page_obj;
            ASSERT(pPageObject != NULL);
            ASSERT(IS_PAGE_DATA(pPageObject));

            gForcedStallInfo.pagePtr = pPageObject;
            gForcedStallInfo.errorTray = pPageObject->engTray;
            gForcedStallInfo.errorMediaType = pPageObject->engMediaType;
            gForcedStallInfo.errorMediaSize = pPageObject->engMediaSize;
            gForcedStallInfo.requestedMediaSize = pPageObject->PAPER;
            gForcedStallInfo.requestedMediaType = pPageObject->mediaType;
            gForcedStallInfo.requestedTray = pPageObject->DEFAULTSOURCE;
            gForcedStallInfo.requestedOutTray = pPageObject->requestedOutTray;

        }
        else
        {
            gForcedStallInfo.pagePtr = NULL;
        }
    }

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrCheckForForcedStall: exit\n");
    return forwardStatus;
}



/******************************************************************************
 * Function Name: PrintMgrGetSizeAndType
 */
/**
 * \brief Determine media size and media type, possibly based on tray or defaults.
 *
 * \date 8/10/2004
 *
 * If the size or type aren't present (indicated by pageState) then
 * figure out a size or type based on the configured values in the
 * tray or the default values.
 ******************************************************************************/
uint32_t PrintMgrGetSizeAndType(const mediacnfg_t* pCnfg, trayenum_t tray, uint8_t pageState,
                              mediasize_t* sizeOut, mediatype_t* typeOut)
{
    mediasize_t cnfgSize;
    mediatype_t cnfgType;
    uint32_t stateAdded = 0;

    //if no tray passed in then set to any so we will set to the
    //default value
    if ( (tray == TRAY_NONE) || (tray == TRAY_ANY) )
    {
        cnfgSize = MEDIASIZE_ANY;
        cnfgType = MEDIATYPE_ANY;
    }
    else
    {
        ASSERT( tray < NUM_INPUTS );
        cnfgSize = pCnfg->mediaSize[tray];
        //cnfgType = pCnfg->mediaType[tray];		// The mediaType value on DB is not matched with mediaType defines!!!
    }

    if(!(pageState & HaveSize))
    {
        //no size given, if there is a size configured for the tray
        //use it, else use the default size
        if(cnfgSize != MEDIASIZE_ANY)
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall check no size given use tray cnfg: %ld\n", cnfgSize);
            *sizeOut = cnfgSize;
        }
        //else use the default size for the current job
        else
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall check use default size: %ld\n", pCnfg->DefmediaSize);
            *sizeOut = pCnfg->DefmediaSize;
        }
        //we have picked a size
        stateAdded |= HaveSize;
    }

    if(!(pageState & HaveType))
    {
        //no type given
        //if tray has configured type use it
        //else use the default type
        if(cnfgType != MEDIATYPE_ANY)
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall check no type given use tray cnfg: %ld\n", cnfgType);
            //*typeOut = cnfgType;
        }
        else
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall check use default type: %ld\n", pCnfg->DefmediaType);
            //*typeOut = pCnfg->DefmediaType;
        }
        stateAdded |= HaveType;
    }

    return stateAdded;
}




//------------------------------------------------------------------------------
// { Begin InputTrayIterator methods.

/**
 * \brief Return the first value in our defined order for iterating through
 *        all general user input trays.
 *
 * \return trayenum_t Caller must check for TRAY_NONE to detect when all valid
 *                  values have been iterated over.
 *
 * Loop iteration order:
 * - First is the preferred trayenum_t value corresponding to one-relative
 *   physical tray number 1.
 * - Next come the trayenum_t values corresponding to one-relative physical tray
 *   numbers N (the highest numbered general user input tray in the printer),
 *   then trays N-1, ..., and finally Tray 2. Subsequent calls return TRAY_NONE.
 */
trayenum_t InputTrayIteratorGetFirst(void)
{
#if 0
#ifdef HAVE_PRINT_MECH
    trayenum_t returnTrayEnum = EngPhysicalTrayNumberToTRAYENUM(1);
#else
    trayenum_t returnTrayEnum = 0;
#endif
#endif
	mediacnfg_t pMediaCnfg;
	printvar_get_trayconfig(&pMediaCnfg);

	trayenum_t returnTrayEnum = pMediaCnfg.defaultSourceTray;
    //DPRINTF(PMDEBUGSOFT, ("PM: InputTrayIteratorGetFirst: raw trayenum_t=%d\n", returnTrayEnum));
    return returnTrayEnum;
} // end InputTrayIteratorGetFirst()

/**
 * \brief Return the next trayenum_t value in our defined order for iterating
 *        through all general user input trays.
 *
 * \param inputTrayEnum (trayenum_t) Current value in user's iteration. If the
 *        input value is TRAY_NONE, TRAY_NONE is returned.
 *
 * \return trayenum_t Caller must check for TRAY_NONE to detect when all valid
 *                  values have been iterated over.
 *
 * Loop iteration order:
 * - First is the preferred trayenum_t value corresponding to one-relative
 *   physical tray number 1.
 * - Next come the trayenum_t values corresponding to one-relative physical tray
 *   numbers N (the highest numbered general user input tray in the printer),
 *   then trays N-1, ..., and finally Tray 2. Subsequent calls return TRAY_NONE.
 *
 * Usage:
 * <pre>
 *  trayenum_t curTray = InputTrayIteratorGetFirst();
 *
 *  while (curTray != TRAY_NONE)
 *  {
 *      // ... logic involving curTray ...
 *
 *      curTray = InputTrayIteratorGetNext(curTray);
 *  }
 * </pre>
 */
trayenum_t InputTrayIteratorGetNext(trayenum_t inputTrayEnum, trayenum_t inputStartTrayEnum)
{


#if 1
    //inputTrayEnum++;
    //if(inputTrayEnum == (NUM_INPUTS - gEngineInfo.optionTrayNum))
		//inputTrayEnum = TRAY_1;
    //if(inputTrayEnum == inputStartTrayEnum)
		//return TRAY_NONE;
	//return inputTrayEnum;

	switch(inputStartTrayEnum)
	{
		case TRAY_1:	// MPT					// MPT(TRAY_1)-TRAY1(TRAY_2)-TRAY2(TRAY_3)-TRAY3(TRAY_4)
			inputTrayEnum++;
			if(inputTrayEnum == (NUM_INPUTS - gEngineInfo.optionTrayNum))		return TRAY_NONE;
			//return inputTrayEnum;
			break;

		case TRAY_2:	// Main Tray			// TRAY1-TRAY2-TRAY3-MPT
			inputTrayEnum++;
			if(inputTrayEnum == (NUM_INPUTS - gEngineInfo.optionTrayNum))		inputTrayEnum = TRAY_1;
			if(inputTrayEnum == inputStartTrayEnum)		return TRAY_NONE;
			//return inputTrayEnum;
			break;

		case TRAY_3:	// 1st Option Tray		// TRAY2-TRAY1-TRAY3-MPT
			if(inputTrayEnum == TRAY_3)			inputTrayEnum = TRAY_2;
			else if(inputTrayEnum == TRAY_2)
			{
				if(gEngineInfo.optionTrayNum == TWO_OPTIONTRAY_DETACTED)	inputTrayEnum = TRAY_4;
				else
				{
					inputTrayEnum = TRAY_1;
				}
			}
			else if(inputTrayEnum == TRAY_4)	inputTrayEnum = TRAY_1;
			else
			{
				return TRAY_NONE;
			}
			//return inputTrayEnum;
			break;

		case TRAY_4:	// 2nd Option Tray		// TRAY3-TRAY1-TRAY2-MPT
			if(inputTrayEnum == TRAY_4)			inputTrayEnum = TRAY_2;
			else if(inputTrayEnum == TRAY_2)	inputTrayEnum = TRAY_3;
			else if(inputTrayEnum == TRAY_3)	inputTrayEnum = TRAY_1;
			else 								return TRAY_NONE;
			//return inputTrayEnum;
			break;
	}
	return inputTrayEnum;

#else // WTef algorithm not understood!
#ifdef HAVE_PRINT_MECH
    trayenum_t returnTrayEnum = TRAY_NONE;
    if (inputTrayEnum != TRAY_NONE)
    {
        int oneRelativePhysicalTrayNumber = EngTRAYENUMToPhysicalTrayNumber(inputTrayEnum);
        // Note: Signed quantities are required for range checking to detect
        //       when a decrement operation has obtained a negative value, etc.
        int signedTrayEnumMax = NUM_INPUTS-1;
        int signedTrayEnumMin = (int)TRAY_1;
        int signedWorkingTrayEnum = (int)inputTrayEnum;

        // Assure inputTrayEnum is a valid value.
        if ( (signedWorkingTrayEnum >= signedTrayEnumMin) &&
             (signedWorkingTrayEnum <= signedTrayEnumMax) &&
             (oneRelativePhysicalTrayNumber != -1) )
        {
            if (oneRelativePhysicalTrayNumber == 2)
            {
                // We are done!
                returnTrayEnum = TRAY_NONE;
            }
            else // Need to "advance" the value, either 1 -> N or n -> n-1.
            {
                if (oneRelativePhysicalTrayNumber == 1) // "Advance" 1 -> N.
                {
                    /**
                     * There may be some trayenum_t values that are not general
                     * user input trays, e.g., the duplexer. Assume these are
                     * all collected together at the top end of the range. Skip
                     * over these in the while loop below. Such non-user-input
                     * trays return -1 from EngTRAYENUMToPhysicalTrayNumber().
                     */
                    signedWorkingTrayEnum = signedTrayEnumMax;
                    oneRelativePhysicalTrayNumber = EngTRAYENUMToPhysicalTrayNumber((trayenum_t)signedWorkingTrayEnum);
                    while ( (signedWorkingTrayEnum > signedTrayEnumMin) &&
                            (oneRelativePhysicalTrayNumber == -1) )
                    {
                        signedWorkingTrayEnum--;
                        oneRelativePhysicalTrayNumber = EngTRAYENUMToPhysicalTrayNumber((trayenum_t)signedWorkingTrayEnum);
                    }
                }
                else // "Advance" n -> n-1.
                {
                    signedWorkingTrayEnum--;
                    oneRelativePhysicalTrayNumber = EngTRAYENUMToPhysicalTrayNumber((trayenum_t)signedWorkingTrayEnum);
                }

                if (oneRelativePhysicalTrayNumber >= 2)
                    returnTrayEnum = (trayenum_t)signedWorkingTrayEnum;
                else
                    returnTrayEnum = TRAY_NONE;
            } // end else // Need to "advance" the value, either 1 -> N or n -> n-1.
        }
        else // Input trayenum_t is out of range. Make the caller fix their bug!
            ASSERT(FALSE);
    } // end if (inputTrayEnum != TRAY_NONE)
#endif
    //DPRINTF(PMDEBUGSOFT, ("PM: InputTrayIteratorGetNext: raw trayenum_t=%d\n", returnTrayEnum));
    return returnTrayEnum;
#endif // WTef
} // end InputTrayIteratorGetNext()

// } End   InputTrayIterator methods.
//------------------------------------------------------------------------------


// Return TRAY_NONE if no tray is marked as INPUT_UPPER
trayenum_t GetInputUpper(const printer_setup_t* pSetup )
{
    trayenum_t tray = TRAY_NONE;
    trayenum_t trayFound = TRAY_NONE;
    for( tray = TRAY_1; tray < NUM_INPUTS; tray++ )
    {
        if( pSetup->inputTrays[tray].inputType == INPUT_UPPER )
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: upper tray = %d\n", tray+1);
            trayFound = tray;
            break;
        }
    }
    // tray should be tray 1 if no matches were found for INPUT_UPPER. The input type should be set in the engine configuration code.
    // If it is not we will default to TRAY_1
    return trayFound;
}

bool CheckTray(trayenum_t tray, const printer_setup_t* pSetup, const mediacnfg_t* pCnfg, mediasize_t size, mediatype_t type,
               BESTMATCH* bestMatch,mediasize_t findSize, mediatype_t findType, uint8_t matchType )
{
    bool done = FALSE;
    const INPUTTRAY* pTray;

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: CheckTray: enter (tray : %d)\n",tray);

    pTray = &pSetup->inputTrays[tray];

    //skip any inputs not currently present
    if(!pTray->inputPresent)
    {
        //DPRINTF( (PMDEBUGSOFT), ("PM: tray not present\n"));
        //DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: CheckTray: exit1\n");
        return done;
    }

    //if tray doesn't support selected size skip it
    if(!PrintMgrCheckForSupportedSize(pTray, size))
    {
        DBG_PRINTF(LOG_WARNING|DBG_PRINTMGR,"PM: size not supported in tray\n");
        //DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: CheckTray: exit2\n");
        return done;
    }

    //if tray doesn't support selected type skip it
    if(!PrintMgrCheckForSupportedType(pTray, type))
    {
        DBG_PRINTF(LOG_WARNING|DBG_PRINTMGR,"PM: type not supported in tray\n");
        //DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: CheckTray: exit3\n");
        return done;
    }

    //We are trying to find the best non empty or empty tray. We print with the
    //best nonempty tray, but if all trays are empty we want to prompt the user
    //to load the best empty tray.
    //if(pCnfg->mediaSize[tray] == findSize && pCnfg->mediaType[tray] == findType)
    if(pCnfg->mediaSize[tray] == findSize)
    {
        //if it matches and has paper in it we go with it
        if(pTray->paperPresent)
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: good tray match %d %d %d\n",
                         tray+1,pCnfg->mediaSize[tray],findSize);

            bestMatch->bestNonEmpty = tray;
            bestMatch->matchTypeNonEmpty = matchType;
            done = TRUE;
        }
        //else only save as a good empty candidate if we don't already have one.
        //we iterate from best choice to worst choice so we save the first
        //one we find.
        else if(bestMatch->bestEmpty == TRAY_NONE)
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: found good empty tray %d %d %d\n",tray+1,pCnfg->mediaSize[tray],findSize);

            bestMatch->bestEmpty = tray;
            bestMatch->matchTypeEmpty = matchType;
        }
    }

    //DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: CheckTray: exit4\n");
    return done;
}

void PrintMgrDetectManualTray(const printer_setup_t* pSetup, trayenum_t* mfTray, bool* isPriority,
                              bool* isMPTray)
{
    trayenum_t tray;

    //see if we have a manual feed tray and whether it is a priority tray
    //manual feed and priority are treated differently than standard trays
    //(we only say we have priority if it has paper in it)
    for(tray = TRAY_1; tray < NUM_INPUTS; tray++)
    {
        if(pSetup->inputTrays[tray].inputPresent)
        {
            if(pSetup->inputTrays[tray].inputType == INPUT_MANUAL)
            {
                //we have a manual tray, save its index
                *mfTray = tray;

                if((pSetup->inputTrays[tray].properties & INPUTPROPERTY_PRIORITY) &&
                   pSetup->inputTrays[tray].paperPresent)
                {
                    *isPriority = TRUE;  //manual tray is also a priority tray
                }

                //there are some special cases we need to do for multipurpose trays
                //when doing autoselection so keep a flag for it
                if(pSetup->inputTrays[tray].properties & INPUTPROPERTY_MULTIPURPOSE)
                {
                    *isMPTray = TRUE;
                }
                break;
            }
        }
    }
}

/******************************************************************************
 * Function Name: PrintMgrDetectManualDuplexTray
 */
/**
 * \brief Identify which tray should be used for manual duplexing.
 *
 * \date 10/26/06
 *
 * We need to identify which tray should be used for manual duplexing.
 * This function will look for the input property and return which tray
 * is to be used for manual duplexing. It will return ANY if there is
 * not a tray identified.
 ******************************************************************************/
void PrintMgrDetectManualDuplexTray( const printer_setup_t* pSetup, trayenum_t* duplexTray )
{
    trayenum_t tray;

    *duplexTray = TRAY_ANY;

    for(tray = TRAY_1; tray < NUM_INPUTS; tray++)
    {
        if(pSetup->inputTrays[tray].inputPresent)
        {
            if(pSetup->inputTrays[tray].properties & INPUTPROPERTY_MANUAL_DUPLEX)
            {
                //we have a manual tray, save its index
                *duplexTray = tray;
                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: found manual duplex tray %d\n", tray+1);

                break;
            }
        }
    }
    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: manual default duplex tray %d\n", *duplexTray+1);
}


/******************************************************************************
 * Function Name: PrintMgrHandlePrinterOptions
 */
/**
 * \brief Handle engines that don't have paper sensors.
 *
 * \date 8/24/2004
 *
 * This function takes care of engines that don't have
 * paper sensors. These types of engines can detect when
 * they are out of paper because of the errors they get
 * when printing, but they can't tell when paper is added.
 * When we get a user input ok message we assume the user
 * has added paper.  We send the user error recover message
 * to the engine so it can do what it needs to do to clear
 * the error out message.
 *
 * When we come out of this function we will redo the stall
 * check and continue to stall because paper is still out.
 *
 * The engines can take a couple of different approaches.
 * -# Set the paperPresent flag in the printersetup structure and
 *   Reset the engine. This will cause an initializing status
 *   to be sent which will cancel the current job. When the engine
 *   goes back online, the job will restart, the stall check will
 *   pass and we will print (if paper wasn't really added we will
 *   repeat the paper out stall)
 * -# Set the paperPresent flag in printersetup and send the paperadded
 *   message. The print manager will clear the stall and try to print
 *
 ******************************************************************************/
void PrintMgrHandlePrinterOptions(void)
{
    printer_setup_t  pSetup;
    uint32_t i;
    bool sendRecover = FALSE;

    //pSetup = (const printer_setup_t*)DSLockVar(e_PrinterSetup);
    printvar_get_printersetup(&pSetup);

    //if the printer doesn't have paper sensors and
    //any of the trays show tray empty then we have
    //to send the recover message
    if(pSetup.options & PRINTER_NO_PAPER_SENSOR)
    {
        for(i = 0; i < NUM_INPUTS; i++)
        {
            if(pSetup.inputTrays[i].inputPresent &&
               !pSetup.inputTrays[i].paperPresent)
            {
                sendRecover = TRUE;
                break;
            }
        }
    }
  //  DSUnlockVar(e_PrinterSetup);

    if(sendRecover)
    {
        PrintMgrSendMsg(ENGINEID, MSG_USER_ERR_RECOVERY, 0, 0, 0);
    }
}

bool PrintMgrCheckForSupportedSize(const INPUTTRAY* pTray, mediasize_t size)
{
    UINT16 sizeIndex = 0;
    bool result = FALSE;

    for(sizeIndex = 0; sizeIndex < pTray->numSizes; sizeIndex++)
    {
        //DPRINTF( (PMDEBUGSOFT), ("    ...supportedSizes[%ld]=%ld%s\n", sizeIndex, pTray->supportedSizes[sizeIndex], ((pTray->supportedSizes[sizeIndex] == MEDIASIZE_ANY) ? "=ANY; matches any input size" : "")));
        if(pTray->supportedSizes[sizeIndex] == size ||
           pTray->supportedSizes[sizeIndex] == MEDIASIZE_ANY)
        {
            result = TRUE;
            break;
        }
    }

    //DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: PrintMgrCheckForSupportedSize(tray INPUTTYPE=%ld, size=%ld): %s\n",
                 //pTray->inputType, size, toStringCond(result));
    return result;
}


bool PrintMgrCheckForSupportedType(const INPUTTRAY* pTray, mediatype_t type)
{
    UINT16 typeIndex = 0;
    bool result = FALSE;

    for(typeIndex = 0; typeIndex < pTray->numTypes; typeIndex++)
    {
        //DPRINTF( (PMDEBUGSOFT), ("    ...supportedTypes[%ld]=%ld%s\n", typeIndex, pTray->supportedTypes[typeIndex], ((pTray->supportedTypes[typeIndex] == MEDIATYPE_ANY) ? "=ANY; matches any input type" : "")));
        //look for matching type or for the tray to be set to accept any type
        if(pTray->supportedTypes[typeIndex] == type ||
           pTray->supportedTypes[typeIndex] == MEDIATYPE_ANY)
        {
            result = TRUE;
            break;
        }
    }
    //DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: PrintMgrCheckForSupportedType(tray INPUTTYPE=%ld, type=%ld): %s\n",
                 //pTray->inputType, type, toStringCond(result));
    return result;
}


void PrintMgrClearStall(void)
{
    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrClearStall: enter\n");

    //clear the stall
    if(gStallInfo.reason != STALL_NONE)
    {
        gStallInfo.reason = STALL_NONE;
        gStallInfo.tray = TRAY_NONE;
        gStallInfo.size = MEDIASIZE_ANY;
        gStallInfo.type = MEDIATYPE_ANY;
    }

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrClearStall: exit\n");
}


void PrintMgrStartStall(void)
{
    size_type_tray_t size_type_tray;
    uint32_t flags=0;
    uint32_t status = STATUS_INPUT_ERROR;
    TRY_AGAIN retry_callback = NULL;

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrStartStall: enter\n");

    printvar_get_sizetypetray(&size_type_tray);

    size_type_tray.size = gStallInfo.size;
    size_type_tray.type = gStallInfo.type;
    size_type_tray.tray = gStallInfo.tray;

    //set value in data store
    //!! if timeout configured start timer here
    //send STATUS_INPUT_RECONFIGURE_TRAY or STATUS_INPUT_LOAD_TRAY
    switch(gStallInfo.reason)
    {
        case STALL_PAPEROUT:
        case STALL_SIZETYPE:
            if (gStallInfo.reason == STALL_PAPEROUT)
            {
                flags = STT_PAPEROUT;
                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall for paper\n");
            }
            else // (gStallInfo.reason == STALL_SIZETYPE)
            {
                flags = STT_WRONGSIZE | STT_WRONGTYPE;
                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall for size/type\n");
            }
            retry_callback = UserAck;

            /*
             * According to ERS any "Load Tray"
             * prompt must include a tray number. If there is no clear reason
             * to prompt for any particular tray, prompt for tray 1.
             */
  #if 0
  #ifdef HAVE_PRINT_MECH
            if (EngPaperPresentInSomeInputTray())
            {
                switch (EngTRAYENUMToPhysicalTrayNumber(gStallInfo.tray))
                {
                case 1: status = STATUS_INPUT_LOAD_TRAY_1; break;
                case 2: status = STATUS_INPUT_LOAD_TRAY_2; break;
                case 3: status = STATUS_INPUT_LOAD_TRAY_3; break;
                default:
                    gStallInfo.tray = EngPhysicalTrayNumberToTRAYENUM(1);
                    status = STATUS_INPUT_LOAD_TRAY_1;
                    break;
                }
            }
            else // No tray has paper, so don't offer user "Press [OK] for available media".
            {
                switch (EngTRAYENUMToPhysicalTrayNumber(gStallInfo.tray))
                {
                case 1: status = STATUS_INPUT_LOAD_TRAY_1_NO_MEDIA_AVAILABLE; break;
                case 2: status = STATUS_INPUT_LOAD_TRAY_2_NO_MEDIA_AVAILABLE; break;
                case 3: status = STATUS_INPUT_LOAD_TRAY_3_NO_MEDIA_AVAILABLE; break;
                default:
                    gStallInfo.tray = EngPhysicalTrayNumberToTRAYENUM(1);
                    status = STATUS_INPUT_LOAD_TRAY_1_NO_MEDIA_AVAILABLE;
                    break;
                }
            }
   #endif
   #endif
			status = STATUS_INPUT_TRAY_SIZE_MISMATCH;
			DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: stall for size/type  <%x>\n",status);
            //start timed override on platforms that support it for
            //paper out and size type
            PrintMgrStartTimedOverride(gStallInfo.stallCount);
            break;

        case STALL_MANUALFEEDJOB:
            flags = STT_WRONGSIZE | STT_WRONGTYPE;
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall for manual feed job\n");
     #ifdef HAVE_PRINT_MECH
            if (EngPaperPresentInSomeInputTray())
            {
                status = STATUS_INPUT_MANUAL_FEED;
            }
            else // No tray has paper, so don't offer user "Press [OK] for available media".
     #endif
            {
                status = STATUS_INPUT_MANUAL_FEED_NO_MEDIA_AVAILABLE;
            }
            retry_callback = UserAck;
            break;

        case STALL_RECONFIGURE:
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall for reconfigure\n");
            retry_callback = UserAck;
            status = STATUS_INPUT_RECONFIGURE_TRAY;
            break;

        case STALL_MANUALDUPLEX:
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall for manual duplex; raw gStallInfo.tray=%d\n");
            retry_callback = UserAck;

            /*
             * According to ERS, any "Load Tray"
             * prompt must include a tray number. If there is no clear reason
             * to prompt for any particular tray, prompt for tray 1.
             */
   #ifdef HAVE_PRINT_MECH
            switch (EngTRAYENUMToPhysicalTrayNumber(gStallInfo.tray))
            {
            case 1: status = STATUS_INPUT_MANUAL_DUPLEX_TRAY_1; break;
            case 2: status = STATUS_INPUT_MANUAL_DUPLEX_TRAY_2; break;
            case 3: status = STATUS_INPUT_MANUAL_DUPLEX_TRAY_3; break;
            default:
              {
                // For some reason, we got bad information about which tray to
                // use. Look up designated ManualDuplex tray, and hope that it
                // is among the trayenum_t values associated with user Tray 1.
                //
                // Need to debug the circumstances if we ever get here!
                printer_setup_t pSetup;
                trayenum_t            manDupTrayEnum;
                printvar_get_printersetup(&pSetup);
                PrintMgrDetectManualDuplexTray( &pSetup, &manDupTrayEnum );
           //     DSUnlockVar(e_PrinterSetup);
                if (manDupTrayEnum == TRAY_ANY)
                {
                    manDupTrayEnum = EngPhysicalTrayNumberToTRAYENUM(1);
                }
                DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: given raw gStallInfo.tray %d not valid; change to raw %d\n",
                             gStallInfo.tray, manDupTrayEnum);
                gStallInfo.tray = manDupTrayEnum;
                status = STATUS_INPUT_MANUAL_DUPLEX_TRAY_1;
                break;
              }
            }
     #endif
            break;

        case STALL_CLEANINGPAGE:
            status = STATUS_INPUT_CLEANING_PAGE;
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall for cleaning page\n");
            break;

        case STALL_OUTPUT_BIN_FULL:
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: stall for output tray/bin full\n");
            status = STATUS_OUTPUT_STALL_CLEAR_FACEDOWN_BIN;
            break;

        case STALL_FORCED:
            status = gForcedStallInfo.statusCode;
            /* Clear the cancel and restart flags
             * in the error we save off. After the page restarts from the original
             * error code we don't want it to keep restarting every time we stall
             * on it (sequence: error causes page to be cancelled by status manager
             * and restarted by job manager. when we see the restarted page we
             * stall and resend the status to the status manager, this time we
             * don't want a cancel we want a stall for user input)
            */
            CLEARCANCEL(status);
            CLEARRESTART(status);
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: forced stall requested by engine\n");
            break;

        case STALL_DRUM_NEAR_END:
			status = STATUS_DRUM_LOW_MODE_3;
			retry_callback = UserAck;
			break;

		case STALL_CANCELJOB:
			//posix_sleep_ms(100);
			status = STATUS_WRONG_IMAGE_SIZE_CANCEL_REQ;
			retry_callback = UserAck;
			break;

        default:
            ASSERT(0);
            break;
    }
    DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"PM: stall tray: %d, stall size: %d, stall type: %d, reason: %s\n",
                      gStallInfo.tray+1, gStallInfo.size, gStallInfo.type, ToString_STALLREASON(gStallInfo.reason));

    size_type_tray.flags = flags;
    printvar_set_sizetypetray(&size_type_tray);


    switch ( gStallInfo.reason )
    {
        case STALL_FORCED:
            printmgr_media_prompt_notify(status, &gForcedStallInfo, UserAck);
            break;

        case STALL_MANUALFEEDJOB:
        case STALL_PAPEROUT:
            printmgr_paper_out_prompt_notify(status, &gStallInfo, UserAck);
            break;

        default:
            break;
    }

    PrintMgr_Notify_devicestatus(status, retry_callback);

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrStartStall: exit\n");
} // end PrintMgrStartStall()



void PrintMgrSendMsg(MODULE_ID id, AGMSG msgType, uint32_t param1, uint32_t param2, void* param3)
{
    MESSAGE msg;

    msg.msgType = msgType;
    msg.param1 = param1;
    msg.param2 = param2;
    msg.param3 = param3;
    SYMsgSend(id, &msg);
}

/******************************************************************************
 * Function Name: PrintMgrEngineStatus
 */
/**
 * \brief Determine if we need to send a status request to the engine.
 *
 * \date 4/15/2004
 ******************************************************************************/
void PrintMgrEngineStatus(MESSAGE* msg)
{
    if(msg->msgType == MSG_STATUSUPDATE)
    {
        if(gPrintMgrInfo.statReqCnt)
        {
            gPrintMgrInfo.statReqCnt--;
        }
    }
    else
    {
        if(gPrintMgrInfo.statReqCnt == 0)
        {
            //see if we need to query the engine for status
            //if in normal mode (don't care while stalled, calibrating or doing out of band)
            //  if engine is offline send
            //  else if engine is online and num credits is 0
            //
            if(gPrintMgrInfo.mode == PM_NORMAL)
            {
                if(gPrintMgrInfo.engOnline == FALSE ||
                   gPrintMgrInfo.numCredits == 0)
                {
                    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: send status query\n");
                    PrintMgrSendMsg(ENGINEID, MSG_STATUSREQUEST, 0, 0, 0);
                    gPrintMgrInfo.statReqCnt++;

                }
            }
        }
    }
}


void PrintMgrAddToPendingList(ADDTYPE type, PAGE_DATA* pPageObject)
{
    bool have_data = false;
    bool addLink = false;
    ATLISTENTRY* node;
    printmgr_page_t* pmpage;

    ASSERT(IS_PAGE_DATA(pPageObject));

    //for prestart and a new link with HaveHdr set
    if(type == ADD_PAGEPRESTART)
    {
        // ca: DPRINTF( (PMDEBUGSOFT), ("PM: got page hdr: 0x%08X\n", pPageObject));
        addLink = TRUE;
    }
    else
    {
        //This is a page start.
        // We need to handle all these cases:
        //  1. job mgr sent a prestart for this page, now this is the data
        //  2. job mgr didn't send prestart:
        //     a. this is entire single copy page
        //     b. this is entire multi copy page (we will be getting multiple
        //        page starts with the same pointer)

        //     DPRINTF( (PMDEBUGSOFT), ("PM: got page data: 0x%08X\n", pPageObject));

        // start at tail of list and work to head;  page we are looking for is most likely
        // at the tail if it exists at all
        node = ATListTail(&printmgr_pending_list);
        while(node != NULL)
        {
            pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);
            if (pmpage->page_obj == pPageObject)
            {
                //found page pointer, see if case 1 or case 2
                if (!pmpage->have_data)
                {
                    pmpage->have_data = true;
                }
                else
                {
                    //This is case 2, set node to NULL so we will add a new link
                    //with this pointer
                    node = NULL;
                }
                break;
            }
            node = ATListPrev(&printmgr_pending_list, node);
        }

        //didn't find page in list, so add a new link
        if(node == NULL)
        {
            addLink = true;
            have_data = true;
        }
    }

    if(addLink)
    {
        pmpage = (printmgr_page_t*)MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), sizeof(printmgr_page_t));
        ASSERT(pmpage != NULL);

        pmpage->page_obj = pPageObject;
        pmpage->state = PRMGR_PAGE_PENDING;
        pmpage->have_data = have_data;
        ATInsertTailList(&printmgr_pending_list, &pmpage->node);
    }
}

void ResetDrumCountToDB(void)
{
	gEngineInfo.fDrumLowMode_3 = false;
	// Reset DLPC
	gEngineInfo.machineDrumLifePageCount = 0;
	// Reset Drum Motor Run Time
	gDB_Info.drumInfo.MotorRunTime = 0;
	// Reset Drum Remain (set to 100%)
	gDB_Info.drumInfo.DrumLifeRemainPercentage = 100;
	// Reset Drum Page Count
	gDB_Info.drumInfo.DrumPrintedPageCount = 0;

	db_set_drumcount(gDB_Info.machineCount, gDB_Info.drumInfo.DrumPrintedPageCount, gDB_Info.drumInfo.MotorRunTime, gDB_Info.drumInfo.DrumLifeRemainPercentage, gEngineInfo.machineDrumLifePageCount);

	// Reset Drum Jobs by modes
	gDB_Info.DRUM.job_mode_1 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode1p", gDB_Info.DRUM.job_mode_1);
	gDB_Info.DRUM.job_mode_2 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode2p", gDB_Info.DRUM.job_mode_2);
	gDB_Info.DRUM.job_mode_3 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode3p", gDB_Info.DRUM.job_mode_3);
	gDB_Info.DRUM.job_mode_4 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode4p", gDB_Info.DRUM.job_mode_4);
	gDB_Info.DRUM.job_mode_5 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode5p", gDB_Info.DRUM.job_mode_5);
	gDB_Info.DRUM.job_mode_6 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode6p", gDB_Info.DRUM.job_mode_6);
	gDB_Info.DRUM.job_mode_7 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode11p", gDB_Info.DRUM.job_mode_7);
	gDB_Info.DRUM.job_mode_8 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode21p", gDB_Info.DRUM.job_mode_8);
	gDB_Info.DRUM.job_mode_9 = 0;
	db_set_var_int("INST_DRUMCOUNT", "CountMode50p", gDB_Info.DRUM.job_mode_9);
	gDB_Info.drumInfo.FreeMotorRunCount = 0;
	db_set_var_int("INST_DRUMCOUNT", "FreeMotorOnDrumCount", gDB_Info.drumInfo.FreeMotorRunCount);
}

void UpdateJobByModesToDB(uint32_t PrintedPages)
{
	if(PrintedPages == 1)
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_1++;
		gDB_Info.MACHINE.job_mode_1++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode1p", gDB_Info.DRUM.job_mode_1);
		db_set_var_int("INST_MACHINECOUNT", "CountMode1p", gDB_Info.MACHINE.job_mode_1);
	}
	else if(PrintedPages == 2)
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_2++;
		gDB_Info.MACHINE.job_mode_2++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode2p", gDB_Info.DRUM.job_mode_2);
		db_set_var_int("INST_MACHINECOUNT", "CountMode2p", gDB_Info.MACHINE.job_mode_2);
	}
	else if(PrintedPages == 3)
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_3++;
		gDB_Info.MACHINE.job_mode_3++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode3p", gDB_Info.DRUM.job_mode_3);
		db_set_var_int("INST_MACHINECOUNT", "CountMode3p", gDB_Info.MACHINE.job_mode_3);
	}
	else if(PrintedPages == 4)
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_4++;
		gDB_Info.MACHINE.job_mode_4++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode4p", gDB_Info.DRUM.job_mode_4);
		db_set_var_int("INST_MACHINECOUNT", "CountMode4p", gDB_Info.MACHINE.job_mode_4);
	}
	else if(PrintedPages == 5)
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_5++;
		gDB_Info.MACHINE.job_mode_5++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode5p", gDB_Info.DRUM.job_mode_5);
		db_set_var_int("INST_MACHINECOUNT", "CountMode5p", gDB_Info.MACHINE.job_mode_5);
	}
	else if((PrintedPages > 5) && (PrintedPages <= 10))
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_6++;
		gDB_Info.MACHINE.job_mode_6++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode6p", gDB_Info.DRUM.job_mode_6);
		db_set_var_int("INST_MACHINECOUNT", "CountMode6p", gDB_Info.MACHINE.job_mode_6);
	}
	else if((PrintedPages > 10) && (PrintedPages <= 20))
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_7++;
		gDB_Info.MACHINE.job_mode_7++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode11p", gDB_Info.DRUM.job_mode_7);
		db_set_var_int("INST_MACHINECOUNT", "CountMode11p", gDB_Info.MACHINE.job_mode_7);
	}
	else if((PrintedPages > 20) && (PrintedPages <= 50))
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_8++;
		gDB_Info.MACHINE.job_mode_8++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode21p", gDB_Info.DRUM.job_mode_8);
		db_set_var_int("INST_MACHINECOUNT", "CountMode21p", gDB_Info.MACHINE.job_mode_8);
	}
	else if(PrintedPages > 50)
	{
		if ( bSepCartridge )
			gDB_Info.DRUM.job_mode_9++;
		gDB_Info.MACHINE.job_mode_9++;
		db_set_var_int("INST_DRUMCOUNT", "CountMode50p", gDB_Info.DRUM.job_mode_9);
		db_set_var_int("INST_MACHINECOUNT", "CountMode50p", gDB_Info.MACHINE.job_mode_9);
	}
}

void UpdatePrintedPageCountToDB(int DrumLifeRemainPercentage, int MotorRunTime, int dlpc)
{
	//Fuser Print count
	gDB_Info.fuserInfo.FuserPrintedPageCount +=  pagesPrintedCounter;
	//Drum Print count
	if (bSepCartridge)
		gDB_Info.drumInfo.DrumPrintedPageCount +=  pagesPrintedCounter;
	//Total Print count
	gDB_Info.machineCount.total +=  pagesPrintedCounter;
	//Duplex Print count
	if(pagesPrintedDuplexCounter){
		pagesPrintedDuplexCounter = 0;
		gDB_Info.machineCount.duplex +=  pagesPrintedCounter;
	}
	//Host Print Counter
	if(pagesHostPrintCounter){
	    gDB_Info.machineCount.host +=  pagesHostPrintCounter;
	    pagesHostPrintCounter = 0;
	}
	//Report Print Counter
	if(pagesReportPrintCounter){
		gDB_Info.machineCount.report +=	pagesReportPrintCounter;
		pagesReportPrintCounter = 0;
	}
	//USB Print Counter
	if(pagesUSBPrintCounter){
		gDB_Info.machineCount.usbdrive += pagesUSBPrintCounter;
		pagesUSBPrintCounter = 0;
	}
	if(pagesFaxPrintCounter){
		gDB_Info.machineCount.faxprint += pagesFaxPrintCounter;
		pagesFaxPrintCounter = 0;
	}
	if(pagesCopyPrintCounter){
		gDB_Info.machineCount.copyprint += pagesCopyPrintCounter;
		pagesCopyPrintCounter = 0;
	}

	//Reset counter
	pagesPrintedCounter = 0;

	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : UpdatePrintedPageCountToDB : %d %d %d %d %d %d %d %d %d ##\n", gDB_Info.machineCount.total, gDB_Info.machineCount.host, gDB_Info.machineCount.report
	, gDB_Info.machineCount.usbdrive, gDB_Info.machineCount.duplex, gDB_Info.drumInfo.DrumPrintedPageCount, MotorRunTime, DrumLifeRemainPercentage, dlpc);
	
	db_set_drumcount(gDB_Info.machineCount, gDB_Info.drumInfo.DrumPrintedPageCount, MotorRunTime, DrumLifeRemainPercentage, dlpc);
}

void UpdateFuserCountToDB(int FuserMotorRunNormal, int FuserMotorRunLow)
{
	fFuserLifePageCount += ( (fuser_alpha*FuserMotorRunLow + fuser_beta*FuserMotorRunNormal)/fuser_gamma );
	gDB_Info.fuserInfo.FuserLifePageCount = fFuserLifePageCount/100;
	
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : FuserPrintedPageCount = %d, ResetActionCount = %d, MotorRunNormal = %d, MotorRunLow = %d, FuserLifePageCount = %d/100 ##\n"
	, gDB_Info.fuserInfo.FuserPrintedPageCount, gDB_Info.fuserInfo.ResetActionCount, gDB_Info.fuserInfo.MotorRunNormal, gDB_Info.fuserInfo.MotorRunLow, fFuserLifePageCount);
	db_set_fusercount(gDB_Info.fuserInfo.FuserPrintedPageCount, gDB_Info.fuserInfo.ResetActionCount, gDB_Info.fuserInfo.MotorRunNormal, gDB_Info.fuserInfo.MotorRunLow, gDB_Info.fuserInfo.FuserLifePageCount);
}

void CheckFuserUpdate(void)
{
	int FuserFactor = 0;
	int RatioMotorPage = 0;
	int T1 = 0;
	int T2 = 0;
	
	if (IS_PINETREE || IS_PINETREEL){ T1=1033; T2=484; }
	else if (IS_ABBA2H || IS_OAK2H)	{ T1= 799; T2=430; }
	else if (IS_OAK2M)				{ T1= 809; T2=469; }
	else 							{ T1= 978; T2=430; }	// KARAS, PINETREES
	
	RatioMotorPage = gDB_Info.drumInfo.MotorRunTime / gDB_Info.drumInfo.DrumPrintedPageCount;
	
	if ( RatioMotorPage >= T1 )
	{
		if (IS_PINETREE || IS_PINETREEL){ FuserFactor = 213; }
		else if (IS_ABBA2H || IS_OAK2H)	{ FuserFactor = 186; }
		else if (IS_OAK2M)				{ FuserFactor = 172; }
		else 							{ FuserFactor = 218; }	// KARAS, PINETREES
	}
	else if ( (RatioMotorPage < T1) && (RatioMotorPage >= T2) )
	{
		if (IS_PINETREE || IS_PINETREEL){ FuserFactor = 131; }
		else if (IS_ABBA2H || IS_OAK2H)	{ FuserFactor = 132; }
		else if (IS_OAK2M)				{ FuserFactor = 126; }
		else 							{ FuserFactor = 131; }	// KARAS, PINETREES
	}
	else
		FuserFactor = 100;
		
	fFuserLifePageCount = (FuserFactor * gDB_Info.machineCount.total);
	gDB_Info.fuserInfo.FuserLifePageCount = fFuserLifePageCount/100;
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : Update Generated FuserLifePageCount = %d/100 ##\n", fFuserLifePageCount);
	
	db_set_fusercount(gDB_Info.fuserInfo.FuserPrintedPageCount, gDB_Info.fuserInfo.ResetActionCount, gDB_Info.fuserInfo.MotorRunNormal, gDB_Info.fuserInfo.MotorRunLow, gDB_Info.fuserInfo.FuserLifePageCount);
}

void ResetFuserCountToDB(void)
{
	gDB_Info.fuserInfo.ResetActionCount++;
	gDB_Info.fuserInfo.FuserLifePageCount = 0;
	gDB_Info.fuserInfo.FuserPrintedPageCount = 0;
	gDB_Info.fuserInfo.MotorRunNormal = 0;
	gDB_Info.fuserInfo.MotorRunLow = 0;
	fFuserLifePageCount = 0;
	
	db_set_fusercount(gDB_Info.fuserInfo.FuserPrintedPageCount, gDB_Info.fuserInfo.ResetActionCount, gDB_Info.fuserInfo.MotorRunNormal, gDB_Info.fuserInfo.MotorRunLow, gDB_Info.fuserInfo.FuserLifePageCount);
	db_set_fuserhistory(gDB_Info.machineCount.total);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : ResetFuserCountToDB.. ResetAction = %d UsedCount = %d ##\n", gDB_Info.fuserInfo.ResetActionCount, gDB_Info.machineCount.total);
}

trayenum_t get_mismatch_tray(void)
{
	return gEngineInfo.mismatchInfo.inputTray;
}

void set_mismatch_tray(trayenum_t tray)
{
	gEngineInfo.mismatchInfo.inputTray = tray;
	return;
}

mediasize_t get_mismatch_size(void)
{
	return gEngineInfo.mismatchInfo.size;
}

void set_mismatch_size(mediasize_t size)
{
	 gEngineInfo.mismatchInfo.size = size;
}

int* GetTonerJobByModes(void)
{
	return gEngineInfo.tonerInfo.TonerJobByModes;
}

int* GetTonerPrintedByCoverage(void)
{
	return gEngineInfo.tonerInfo.TonerPrintedByCoverage;
}

int GetFreeMotorOnCount(void)
{
	return gEngineInfo.tonerInfo.cFreeMotorOn;
}

/******************************************************************************
 * Function Name: PrintMgrStartTimedOverride
 */
/**
 * \brief Start a timed notification if the platform has
 *        timed override for paper stalls.
 *
 * \date 8/10/2004
 ******************************************************************************/
void PrintMgrStartTimedOverride(uint32_t timedId)
{
#ifdef PM_TIMED_OVERRIDE_SUPPORTED
    bool regRes;
    PAPEROUTACTION TmpAction;

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrStartTimedOverride: enter\n");

    //TmpAction = (PAPEROUTACTION *)DSLockVar(e_PaperOutAction);
    papervar_get_paperoutaction(&TmpAction);

    if(TmpAction.Action == PA_OVERRIDE || TmpAction.Action == PA_CANCEL)
    {
        regRes = DMNotifyRegister(PRINTMGRID, TmpAction.Timeout*100, FALSE, timedId );
    }

  //  DSUnlockVar(e_PaperOutAction);

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrStartTimedOverride: exit\n");
#endif // PM_TIMED_OVERRIDE_SUPPORTED
}


void PrintMgrProcessTimedOverride(uint32_t timedId)
{
#ifdef PM_TIMED_OVERRIDE_SUPPORTED
    bool send = FALSE;
    MESSAGE outMsg;
    PAPEROUTACTION TmpAction;

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrProcessTimedOverride: enter\n");

    /*
     * When we register for the timed notify we send the current stall count.
     * If the stall count is still the same, process the message. Otherwise
     * assume the user continued on that stall before the timeout and skip the
     * message here.
    */
    if(gPrintMgrInfo.mode == PM_STALLED && timedId == gStallInfo.stallCount)
    {
      //  TmpAction = (PAPEROUTACTION *)DSLockVar(e_PaperOutAction);

        paperGetVarPaperOutAction(&TmpAction);
        if(TmpAction.Action == PA_OVERRIDE)
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: override timer expired, send ok\n");

            //send user input message to ourselves
            outMsg.param1 = PARAM_USER_OK;
            send = TRUE;
        }
        else if(TmpAction.Action == PA_CANCEL)
        {
            DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: override timer expired, send cancel\n");
            outMsg.param1 = PARAM_USER_CANCEL;
            send = TRUE;
        }

        if(send == TRUE)
        {
            outMsg.msgType = MSG_USER_INPUT;
            SYMsgSend(PRINTMGRID, &outMsg);
        }

     //   DSUnlockVar(e_PaperOutAction);
    }

    DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"PM: PrintMgrProcessTimedOverride: exit\n");
#endif // PM_TIMED_OVERRIDE_SUPPORTED
}


void PrintMgrStopTimedOverride(uint32_t timedId)
{
#ifdef PM_TIMED_OVERRIDE_SUPPORTED
    DBG_PRINTF(LOG_INFO|DBG_PRINTMGR,"PM: disable override timer\n");
    DMNotifyUnregister( PRINTMGRID, timedId );
#endif
}


/*******************************************************************************
 * Function Name: PrintMgrPageActive
 */
/**
 * \brief Return TRUE if there are active pages at the engine.
 *
 * \date 8/17/2004
 ******************************************************************************/
bool PrintMgrPageActive(void)
{
    return !ATIsListEmpty(&printmgr_active_list);
}

uint32_t PMGetStallStatusWord( void )
{
    return gForcedStallInfo.statusCode;
}

bool MediaSizeAndTypeFitTrayConfiguration(mediasize_t inMediaSize, mediatype_t inMediaType, trayenum_t inTrayEnum, const mediacnfg_t* pMediaCnfg)
{
    mediasize_t   traysConfiguredMediaSize = pMediaCnfg->mediaSize[inTrayEnum];
    mediatype_t   traysConfiguredMediaType = pMediaCnfg->mediaType[inTrayEnum];
    bool        sizeAndTypeFit = FALSE;

    if ( ( (traysConfiguredMediaSize == MEDIASIZE_ANY) ||
           (traysConfiguredMediaSize == inMediaSize) )
         &&
         ( (traysConfiguredMediaType == MEDIATYPE_ANY) ||
           (traysConfiguredMediaType == inMediaType) ) )
    {
        sizeAndTypeFit = TRUE;
    }

    return sizeAndTypeFit;
} // end MediaSizeAndTypeFitTrayConfiguration()

void get_DB_values_init(void)
{
	error_type_t db_retval = FAIL;
	int tempDLPC_Remain;

	// Get Drum Life Page Count (DLPC)
	//gDB_Info.drumInfo.DrumLifePageCount = db_get_var_int("INST_DRUMCOUNT","DrumLifePageCount",&db_retval);
	//DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : db_DLPC = %d ##\n",gDB_Info.drumInfo.DrumLifePageCount);
	//gEngineInfo.machineDrumLifePageCount = gDB_Info.drumInfo.DrumLifePageCount * 100;

	if(gDB_Info.drumInfo.DrumLifePageCount >= DRUM_LOW_MODE_1)
	{
		tempDLPC_Remain = 0;
	}
	else
	{
		//tempDLPC_Remain = (DRUM_END - gDB_Info.drumInfo.DrumLifePageCount) * 100 / DRUM_END;
		tempDLPC_Remain = 100 - (gDB_Info.drumInfo.DrumLifePageCount * 100 / DRUM_LOW_MODE_1);
	}
	/*
	// Get Drum Remain Percentage
	gDB_Info.drumInfo.DrumLifeRemainPercentage = db_get_var_int("INST_DRUMCOUNT","CountInPercent",&db_retval);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : DLPC Remain = %d (DB) %d (Cal) ##\n",gDB_Info.drumInfo.DrumLifeRemainPercentage, tempDLPC_Remain);
	// Get Drum Printed Page Count
	gDB_Info.drumInfo.DrumPrintedPageCount = db_get_var_int("INST_DRUMCOUNT","Count",&db_retval);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : DrumPrintedPageCount = %d ##\n",gDB_Info.drumInfo.DrumPrintedPageCount);
	// Get Drum Motor Run Time (*10msec)
	gDB_Info.drumInfo.MotorRunTime = db_get_var_int("INST_DRUMCOUNT","Motor",&db_retval);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : Motor Run Time = %d ##\n",gDB_Info.drumInfo.MotorRunTime);
	*/
	db_get_drumcount(&gDB_Info.machineCount, &gDB_Info.drumInfo.DrumPrintedPageCount, &gDB_Info.drumInfo.DrumLifeRemainPercentage, &gDB_Info.drumInfo.MotorRunTime, &gDB_Info.drumInfo.DrumLifePageCount);
	gEngineInfo.machineDrumLifePageCount = gDB_Info.drumInfo.DrumLifePageCount * 100;

	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : DLPC = %d, DrumPrintedPageCount = %d, DrumLifeRemainPercentage = %d, MotorRunTime = %d##\n"
	,gDB_Info.drumInfo.DrumLifePageCount, gDB_Info.drumInfo.DrumPrintedPageCount, gDB_Info.drumInfo.DrumLifeRemainPercentage, gDB_Info.drumInfo.MotorRunTime);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : machineCount = %d %d %d %d %d ##\n"
	,gDB_Info.machineCount.total, gDB_Info.machineCount.host, gDB_Info.machineCount.report, gDB_Info.machineCount.usbdrive, gDB_Info.machineCount.duplex);

	// Get Drum Free Motor Run Count
	gDB_Info.drumInfo.FreeMotorRunCount = db_get_var_int("INST_DRUMCOUNT","FreeMotorOnDrumCount",&db_retval);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : Drum Free Motor Run Count = %d ##\n",gDB_Info.drumInfo.FreeMotorRunCount);

	// Get Job by Mode (Drum)
	gDB_Info.DRUM.job_mode_1 = db_get_var_int("INST_DRUMCOUNT","CountMode1p",&db_retval);
	gDB_Info.DRUM.job_mode_2 = db_get_var_int("INST_DRUMCOUNT","CountMode2p",&db_retval);
	gDB_Info.DRUM.job_mode_3 = db_get_var_int("INST_DRUMCOUNT","CountMode3p",&db_retval);
	gDB_Info.DRUM.job_mode_4 = db_get_var_int("INST_DRUMCOUNT","CountMode4p",&db_retval);
	gDB_Info.DRUM.job_mode_5 = db_get_var_int("INST_DRUMCOUNT","CountMode5p",&db_retval);
	gDB_Info.DRUM.job_mode_6 = db_get_var_int("INST_DRUMCOUNT","CountMode6p",&db_retval);
	gDB_Info.DRUM.job_mode_7 = db_get_var_int("INST_DRUMCOUNT","CountMode11p",&db_retval);
	gDB_Info.DRUM.job_mode_8 = db_get_var_int("INST_DRUMCOUNT","CountMode21p",&db_retval);
	gDB_Info.DRUM.job_mode_9 = db_get_var_int("INST_DRUMCOUNT","CountMode50p",&db_retval);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : DRUM Job by mode = %d %d %d %d %d %d %d %d %d##\n",gDB_Info.DRUM.job_mode_1,gDB_Info.DRUM.job_mode_2,gDB_Info.DRUM.job_mode_3,
	gDB_Info.DRUM.job_mode_4,gDB_Info.DRUM.job_mode_5,gDB_Info.DRUM.job_mode_6,gDB_Info.DRUM.job_mode_7,gDB_Info.DRUM.job_mode_8,gDB_Info.DRUM.job_mode_9);

	// Get Job by Mode (Machine)
	gDB_Info.MACHINE.job_mode_1 = db_get_var_int("INST_MACHINECOUNT","CountMode1p",&db_retval);
	gDB_Info.MACHINE.job_mode_2 = db_get_var_int("INST_MACHINECOUNT","CountMode2p",&db_retval);
	gDB_Info.MACHINE.job_mode_3 = db_get_var_int("INST_MACHINECOUNT","CountMode3p",&db_retval);
	gDB_Info.MACHINE.job_mode_4 = db_get_var_int("INST_MACHINECOUNT","CountMode4p",&db_retval);
	gDB_Info.MACHINE.job_mode_5 = db_get_var_int("INST_MACHINECOUNT","CountMode5p",&db_retval);
	gDB_Info.MACHINE.job_mode_6 = db_get_var_int("INST_MACHINECOUNT","CountMode6p",&db_retval);
	gDB_Info.MACHINE.job_mode_7 = db_get_var_int("INST_MACHINECOUNT","CountMode11p",&db_retval);
	gDB_Info.MACHINE.job_mode_8 = db_get_var_int("INST_MACHINECOUNT","CountMode21p",&db_retval);
	gDB_Info.MACHINE.job_mode_9 = db_get_var_int("INST_MACHINECOUNT","CountMode50p",&db_retval);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : MACHINE Job by mode = %d %d %d %d %d %d %d %d %d##\n",gDB_Info.MACHINE.job_mode_1,gDB_Info.MACHINE.job_mode_2,gDB_Info.MACHINE.job_mode_3,
	gDB_Info.MACHINE.job_mode_4,gDB_Info.MACHINE.job_mode_5,gDB_Info.MACHINE.job_mode_6,gDB_Info.MACHINE.job_mode_7,gDB_Info.MACHINE.job_mode_8,gDB_Info.MACHINE.job_mode_9);
	
	// Get Fuser Count
	db_get_fusercount(&gDB_Info.fuserInfo.FuserPrintedPageCount, &gDB_Info.fuserInfo.ResetActionCount, &gDB_Info.fuserInfo.MotorRunNormal, &gDB_Info.fuserInfo.MotorRunLow, &gDB_Info.fuserInfo.FuserLifePageCount);
	DBG_PRINTF(LOG_DEBUG|DBG_PRINTMGR,"## PM : Fuser Count = %d, ResetAction = %d, MotorNormal = %d, MotorLow = %d, FuserLifePageCount = %d ##\n"
	,gDB_Info.fuserInfo.FuserPrintedPageCount, gDB_Info.fuserInfo.ResetActionCount, gDB_Info.fuserInfo.MotorRunNormal, gDB_Info.fuserInfo.MotorRunLow, gDB_Info.fuserInfo.FuserLifePageCount);
	fFuserLifePageCount = gDB_Info.fuserInfo.FuserLifePageCount*100;
	
	if ( (gDB_Info.machineCount.total!=0) && (gDB_Info.fuserInfo.ResetActionCount==0) && (gDB_Info.fuserInfo.FuserLifePageCount==0) ) // need to add extra condition? (FuserPrintedPageCount, MotorRunNormal, MotorRunLow)
		CheckFuserUpdate();
}

//------------------------------------------------------------------------------
// FUNCTION NAME: PrintMgrAckJobMgrCancelDone
/**
 * \brief Send ack to JobMgr to start resending pages.
 *
 * \return void
 */
void PrintMgrAckJobMgrCancelDone( void )
{
    PrintMgrSendMsg(JOBMGRID, gPrintMgrInfo.cancelType, LOCAL_ACK, PRINTMGRID, gPrintMgrInfo.cancelPtr);
    gPrintMgrInfo.cancelType = (AGMSG)0;
}



static const char* ToString_FORCEDSTATE( FORCEDSTATE enumValue )
{
    switch (enumValue)
    {
        CASE_ENUM_RETURN_STRING(FORCED_NONE);
        CASE_ENUM_RETURN_STRING(FORCED_WAITCANCEL);
        CASE_ENUM_RETURN_STRING(FORCED_WAITPAGE);
        CASE_ENUM_RETURN_STRING(FORCED_ACTIVE);
        DEFAULT_CASE_ENUM_RETURN_STRING(FORCEDSTATE);
    } // end switch (enumValue)
    return "bug";
} // end ToString_FORCEDSTATE()

const char* ToString_PAGECANCEL( PAGECANCEL enumValue )
{
    switch (enumValue)
    {
        CASE_ENUM_RETURN_STRING(PMCANCEL_NONE);
        CASE_ENUM_RETURN_STRING(PMCANCEL_ENGINE);
        // CASE_ENUM_RETURN_STRING(PMCANCEL_VIDEO);
        CASE_ENUM_RETURN_STRING(PMCANCEL_WAIT_FOR_ENGINE_ONLINE);
        DEFAULT_CASE_ENUM_RETURN_STRING(PAGECANCEL);
    }
    return "bug";
}

#ifdef HAVE_DBG_PRINTF
const char* ToString_PRINTMODE( PRINTMODE enumValue )
{
    switch (enumValue)
    {
        CASE_ENUM_RETURN_STRING(PM_NORMAL);
        CASE_ENUM_RETURN_STRING(PM_STALLING);
        CASE_ENUM_RETURN_STRING(PM_STALLED);
        DEFAULT_CASE_ENUM_RETURN_STRING(PRINTMODE);
    }
    return "bug";
}
#endif


static const char* ToString_STALLACTION( STALLACTION actionEnum )
{
    switch (actionEnum)
    {
        CASE_ENUM_RETURN_STRING(ACTION_START);
        CASE_ENUM_RETURN_STRING(ACTION_CONTINUE_YES);
        CASE_ENUM_RETURN_STRING(ACTION_CONTINUE_NO);
        CASE_ENUM_RETURN_STRING(ACTION_CONTINUE_PAPERADDED);
        CASE_ENUM_RETURN_STRING(ACTION_CONTINUE_OUTPUT_BIN_CLEARED);
        CASE_ENUM_RETURN_STRING(ACTION_NONE);
        DEFAULT_CASE_ENUM_RETURN_STRING(STALLACTION);
    }
    return "bug";
}


const char* ToString_STALLREASON( STALLREASON reasonEnum )
{
    switch (reasonEnum)
    {
        CASE_ENUM_RETURN_STRING(STALL_NONE);
        CASE_ENUM_RETURN_STRING(STALL_PAPEROUT);
        CASE_ENUM_RETURN_STRING(STALL_SIZETYPE);
        CASE_ENUM_RETURN_STRING(STALL_RECONFIGURE);
        CASE_ENUM_RETURN_STRING(STALL_MANUALDUPLEX);
        CASE_ENUM_RETURN_STRING(STALL_MANUALFEEDJOB);
        CASE_ENUM_RETURN_STRING(STALL_CLEANINGPAGE);
        CASE_ENUM_RETURN_STRING(STALL_OUTPUT_BIN_FULL);
        CASE_ENUM_RETURN_STRING(STALL_CANCELJOB);
        CASE_ENUM_RETURN_STRING(STALL_FORCED);
        CASE_ENUM_RETURN_STRING(STALL_PENDING);
        CASE_ENUM_RETURN_STRING(STALL_FINISHED);
        CASE_ENUM_RETURN_STRING(STALL_DRUM_NEAR_END);
        DEFAULT_CASE_ENUM_RETURN_STRING(STALLREASON);
    } // end switch (reasonEnum)
    return "bug";
} // end ToString_STALLREASON()

#if defined(HAVE_DBG_PRINTF) || defined(HAVE_CMD)
static void PageList_Print(void)
{
    ATLISTENTRY* node;
    printmgr_page_t* pmpage;

    if (!gActivatePrintMgrPrintLists)
    {
        return;
    }

    cmd_printf("PM: PageList_Print Active/Pending lists: begin\n");

    node = ATListHead(&printmgr_active_list);
    while (node)
    {
        pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);
        cmd_printf("PM:   Active list pmpage=0x%08X; page_data=0x%08X, state=%d, haveData=%d\n",
                     (unsigned int)pmpage, (unsigned int)pmpage->page_obj, pmpage->state, pmpage->have_data);

        node = ATListNext(&printmgr_active_list, node);
    }

    node = ATListHead(&printmgr_pending_list);
    while (node)
    {
        pmpage = CONTAINING_RECORD(node, printmgr_page_t, node);
        cmd_printf("PM:   PENDING list pmpage=0x%08X; page_data=0x%08X, state=%d, haveData=%d\n",
                     (unsigned int)pmpage, (unsigned int)pmpage->page_obj, pmpage->state, pmpage->have_data);

        node = ATListNext(&printmgr_pending_list, node);
    }

    cmd_printf("PM: PageList_Print Active/Pending lists: end\n");

} // end PageList_Print()


//------------------------------------------------------------------------------

int PrintMgrPrintState(int argc, char* argv[])
{
    cmd_printf("PM STATE: ---------- enter ----------\n");

    cmd_printf("PM STATE: gPrintMgrInfo.engOnline=%d\n", gPrintMgrInfo.engOnline);
    cmd_printf("PM STATE: gPrintMgrInfo.sysOnline=%d\n", gPrintMgrInfo.sysOnline);
    cmd_printf("PM STATE: gPrintMgrInfo.numCredits=%d\n", gPrintMgrInfo.numCredits);
    cmd_printf("PM STATE: gPrintMgrInfo.maxCredits=%d\n", gPrintMgrInfo.maxCredits);
    cmd_printf("PM STATE: gPrintMgrInfo.pageCancelState=%s\n",
                 ToString_PAGECANCEL(gPrintMgrInfo.pageCancelState));
    cmd_printf("PM STATE: gPrintMgrInfo.cancelPtr=0x%08X\n", (unsigned int)gPrintMgrInfo.cancelPtr );
    cmd_printf("PM STATE: gPrintMgrInfo.statReqCnt=%d\n", gPrintMgrInfo.statReqCnt);
    cmd_printf("PM STATE: gPrintMgrInfo.receivedFirstManualDuplexSecondSide=%d\n",
                 (unsigned int)gPrintMgrInfo.receivedFirstManualDuplexSecondSide);
    cmd_printf("PM STATE: gPrintMgrInfo.mode=%s\n", ToString_PRINTMODE(gPrintMgrInfo.mode));
    cmd_printf("PM STATE: gPrintMgrInfo.status=0x%08X\n", (unsigned int)gPrintMgrInfo.status);
//-jrs    cmd_printf("PM STATE: PrintMgrQueue: capacity=%d enqueued=%d%s\n",
//-jrs                      (int)PrintMgrQueue.tx_queue_capacity, (int)PrintMgrQueue.tx_queue_enqueued,
//-jrs                      ( ( PrintMgrQueue.tx_queue_capacity == PrintMgrQueue.tx_queue_enqueued )
//-jrs                        ? " *** QUEUE FULL! DEADLOCK LIKELY! ***" : "" ) );

    cmd_printf("PM STATE: gStallInfo.type=%d\n", gStallInfo.type);
    cmd_printf("PM STATE: gStallInfo.size=%d\n", gStallInfo.size);
    cmd_printf("PM STATE: gStallInfo.tray=%d\n", gStallInfo.tray);
    cmd_printf("PM STATE: gStallInfo.outTray=%d\n", gStallInfo.outTray);
    cmd_printf("PM STATE: gStallInfo.reason=%s\n", ToString_STALLREASON(gStallInfo.reason));
    cmd_printf("PM STATE: gStallInfo.stallCount=%d\n", (int)gStallInfo.stallCount);

    cmd_printf("PM STATE: gLastPage.type=%d\n", gLastPage.type);
    cmd_printf("PM STATE: gLastPage.size=%d\n", gLastPage.size);
    cmd_printf("PM STATE: gLastPage.tray=%d\n", gLastPage.tray);
    cmd_printf("PM STATE: gLastPage.outTray=%d\n", gLastPage.outTray);
    cmd_printf("PM STATE: gLastPage.isManualFeed=%d\n", gLastPage.isManualFeed);
    cmd_printf("PM STATE: gLastPage.isSingleFeed=%d\n", gLastPage.isSingleFeed);

#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
    cmd_printf("PM STATE: gLastPrevNonDuplexInputTRAYENUM=%d\n", gLastPrevNonDuplexInputTRAYENUM);
#endif // defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

    cmd_printf("PM STATE: gOverrideInfo.haveOverride=%d\n", gOverrideInfo.haveOverride);
    cmd_printf("PM STATE: gOverrideInfo.requestedType=%d\n", gOverrideInfo.requestedType);
    cmd_printf("PM STATE: gOverrideInfo.requestedSize=%d\n", gOverrideInfo.requestedSize);
    cmd_printf("PM STATE: gOverrideInfo.requestedTray=%d\n", gOverrideInfo.requestedTray);
    cmd_printf("PM STATE: gOverrideInfo.requestedOutTray=%d\n", gOverrideInfo.requestedOutTray);
    cmd_printf("PM STATE: gOverrideInfo.overrideType=%d\n", gOverrideInfo.overrideType);
    cmd_printf("PM STATE: gOverrideInfo.overrideSize=%d\n", gOverrideInfo.overrideSize);
    cmd_printf("PM STATE: gOverrideInfo.overrideTray=%d\n", gOverrideInfo.overrideTray);
    cmd_printf("PM STATE: gOverrideInfo.overrideOutTray=%d\n", gOverrideInfo.overrideOutTray);

    cmd_printf("PM STATE: gForcedStallInfo.state=%s\n", ToString_FORCEDSTATE(gForcedStallInfo.state));
    cmd_printf("PM STATE: gForcedStallInfo.statusCode=0x%08X\n", (unsigned int)gForcedStallInfo.statusCode);
    cmd_printf("PM STATE: gForcedStallInfo.isUserAck=%d\n", gForcedStallInfo.isUserAck);
    cmd_printf("PM STATE: gForcedStallInfo.isPreError=%d\n", gForcedStallInfo.isPreError);
    cmd_printf("PM STATE: gForcedStallInfo.requestedTray=%d\n", gForcedStallInfo.requestedTray);
    cmd_printf("PM STATE: gForcedStallInfo.requestedOutTray=%d\n", gForcedStallInfo.requestedOutTray);
    cmd_printf("PM STATE: gForcedStallInfo.requestedMediaSize=%d\n", gForcedStallInfo.requestedMediaSize);
    cmd_printf("PM STATE: gForcedStallInfo.requestedMediaType=%d\n", gForcedStallInfo.requestedMediaType);
    cmd_printf("PM STATE: gForcedStallInfo.errorTray=%d\n", gForcedStallInfo.errorTray);
    cmd_printf("PM STATE: gForcedStallInfo.errorMediaSize=%d\n", gForcedStallInfo.errorMediaSize);
    cmd_printf("PM STATE: gForcedStallInfo.errorMediaType=%d\n", gForcedStallInfo.errorMediaType);
    cmd_printf("PM STATE: gForcedStallInfo.waitForCancel=%d\n", gForcedStallInfo.waitForCancel);
    cmd_printf("PM STATE: gForcedStallInfo.pagePtr=0x%08X\n", (unsigned int)gForcedStallInfo.pagePtr);

    bool saveActive = gActivatePrintMgrPrintLists;
    gActivatePrintMgrPrintLists = TRUE;
    PageList_Print();
    gActivatePrintMgrPrintLists = saveActive;

    cmd_printf("PM STATE: ---------- exit ----------\n");
    return 0;
} // end PrintMgrPrintState()

void printmgr_dbg_init()
{
#ifdef HAVE_CMD
    cmd_register("printmgr", "PrintMgr Commands", NULL, NULL, NULL);
    cmd_register("printmgr state",  "Dump all print manager state info", NULL, NULL, PrintMgrPrintState);
#endif
}

#endif // ifdef DEBUG

//------------------------------------------------------------------------------
