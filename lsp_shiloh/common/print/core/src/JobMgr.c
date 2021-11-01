/****************************************************************************** 
 * Copyright (c) 2006-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \file JobMgr.c
 *
 * \brief This file handles everything related to jobs during a print operation.
 *
 * <b>RESPONSIBILITIES of JobMgr</b>
 *
 * JobMgr receives messages from language parsers regarding the start and end of:
 * - Job, which consists of a series of one or more Sheets;
 * - Sheet, which has a FrontPage, and for automatic duplexing, a BackPage.
 * - Page - which has a series of Planes of data, if it is not a blank page.
 * - Plane - data to be printed on the page.
 *
 * JobMgr maintains a list of jobs. The current active job is the list tail.
 * - A new Sheet is appended to the end of the current active Job.
 * - A new Page is appended to the current active Sheet.
 * - A new Plane is appended to the current active Page.
 *
 * For products that HAVE_AUTOMATIC_DUPLEXER_SUPPORT, JobMgr must:
 * - Copy the PAGE_DATA::DUPLEX member to each contained PLANED::DUPLEX member,
 *   for both simplex and duplex Pages. The Video subsystem relies on this.
 * - For AutoDuplex pages, JobMgr must:
 *   - Accumulate from the language parser:
 *     - First the FrontPage of a Sheet, then the BackPage of the same Sheet; or
 *     - First the BackPage of a Sheet, then the FrontPage of the same Sheet.
 *   - Send to PageMgr:
 *     - First the BackPage of a Sheet, as soon as it is ready, and
 *     - Then the FrontPage of the same Sheet, when it is ready, and its
 *       associated BackPage has already been sent.
 *
 * \todo REVISIT TODO: What about duplex pages which the parser has sent in
 *       2, 1, 4, 3, ... order, and we have received page 2, and have even had
 *       time to print it, but we have not yet received its matching page 1?
 *       In such a case, we have the following issue:
 *       The sheet will be waiting in the duplexer with just one side printed so
 *       far. But the engine will kick that page out after just a second or so,
 *       and cause an error condition.
 *       Maybe normal JamRecovery will clear this in due course?
 *       An error condition (paper jam) will occur, and the user will be alerted
 *       to clear the jam, and to do something like open and close the door.
 *       By the time the print operation on the BackPage can be retried, the
 *       associated FrontPage should have arrived in the printer, no?
 *
 *****************************************************************************/


/*** include files ***/

#include <string.h>
#include "pthread.h"
#include "agRouter.h"
#include "ATypes.h"
#include "print_job_types.h"
#include "agMessage.h"
#include "memAPI.h"
#include "debug.h"
#include "lassert.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "PrintMgr.h"
#include "PrintTools.h"
#include "platform.h"
#include "sys_init_api.h"
#include "printvars_api.h"
#include "PrintJobInfo.h"
#include "JobMgr.h"
#ifdef HAVE_POWER_MGR
#include "print_pwr_mgr.h"
#endif
#include "error_types.h"
#include "print_mode_api.h"

#include "print_events_common_api.h"
#include "posix_ostools.h"
#include "engPaperHandlingAPI.h"
#include "cmd_proc_api.h"

#include "db_api.h"
#include "PrintMgr_Priv.h"
#include "SysUI_Interface.h"
#include "print_job_api.h"

// revisit: this goes away with new status manager
BOOL specialMemoryOutGlobalIsSetAndItHasNoHome = FALSE;

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

#define DBG_PRFX "JM: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 12 )


/** Define event flags for communication */
#define CFLAG           0x00000001
#define YFLAG           0x00000002
#define MFLAG           0x00000004
#define KFLAG           0x00000008
#define DATA_AVAIL_FLAG 0x80000000


#define QUEUE_SIZE  10      ///< Number of messages in our queue
#define STACK_SIZE  POSIX_MIN_STACK_SIZE * 2   ///< Size of the stack
#define QUEUE_WAIT  2       ///< number of ticks to wait for a message, 5 = 50msec.

#ifndef PM_QUEUE_SIZE ///< should be in platform.h
#ifdef HAVE_GENERATOR_COMPRESS
#define PM_QUEUE_SIZE 27 // 100 + 2  ///< Max number of pages in a collated copy job + 2
#else
#define PM_QUEUE_SIZE 4 + 2  ///< Max number of pages in a collated copy job.
#endif
#define PM_QUEUE_MIN_SIZE  4 ///< minimum number of page sides to work ahead.
#warning fix all the platform.h variants.
#endif


sem_t *page_token_semaphore = 0;  

void JobAbortProcessing(void);
void process_job_start(MESSAGE *Mesag);
void process_page_start(MESSAGE *Mesag);
#if 0
static void print_jobmgr_global_page_end_observer_constructor( void );
static void print_jobmgr_global_page_end_observer_notify_event( JOB_DATA * job_data, PAGE_DATA * page_data );

static void print_jobmgr_global_job_end_observer_constructor( void );
static void print_jobmgr_global_job_end_observer_notify_event( JOB_DATA * job_data );
#endif
typedef struct JobMgr_myDeviceStatus_s
{
    Subject_T_BaseClassMembers;
    MLISTENTRY observers;  

    uint32_t status;
    JOB_TYPES resourceJobType;
    uint32_t resourceJobID;
} JobMgr_myDeviceStatus_t;

static JobMgr_myDeviceStatus_t JobMgr_myDeviceStatus;
#if 0
typedef struct JobMgr_myPlaneData_s
{
    Subject_T_BaseClassMembers;
    MLISTENTRY observers;

    JOB_EVENT_TYPE event_type;
    PAGE_DATA * pPage;
    PLANED * pPlane;

} JobMgr_myPlaneDataEvent_t;

static JobMgr_myPlaneDataEvent_t JobMgr_myPlaneDataEvent;

typedef struct print_jobmgr_global_page_end_observer_data_s
{
    Response_Subject_T_BaseClassMembers;
    JOB_DATA * job_data;
    PAGE_DATA * page_data;
} print_jobmgr_global_page_end_observer_data_t;

typedef struct print_jobmgr_global_job_end_observer_data_s
{
    Response_Subject_T_BaseClassMembers;
    JOB_DATA * job_data;
} print_jobmgr_global_job_end_observer_data_t;

static print_jobmgr_global_page_end_observer_data_t print_jobmgr_global_page_end_observer_data;
static print_jobmgr_global_job_end_observer_data_t print_jobmgr_global_job_end_observer_data;
#endif 

static void JobMgr_DeviceStatus_constructor(void)
{
    JobMgr_myDeviceStatus.status = 0;
    JobMgr_myDeviceStatus.subject_observer_oid_key = JobMgr_myDeviceStatus_ID;
    MInitList(&JobMgr_myDeviceStatus.observers);

}

static inline JOB_DATA* GET_JOB_LIST_HEAD_JOB_PTR();

static void JobMgr_Notify_devicestatus( uint32_t statusCode )
{
    JobMgr_myDeviceStatus_t * my_status;
    JobMgr_myDeviceStatus.status = statusCode;
    JobMgr_myDeviceStatus.resourceJobType = 0;
    JobMgr_myDeviceStatus.resourceJobID = 0;

    //LaLIT: Add JobType and JobID
    JOB_DATA *active_job = GET_JOB_LIST_HEAD_JOB_PTR();
	if(active_job)
	{
		JOB_RESOURCES *curJobResourcePtr = ((CURRENT_RESOURCE *)active_job->Resources)->JobResources;
		JobMgr_myDeviceStatus.resourceJobType = curJobResourcePtr->JobType;
		JobMgr_myDeviceStatus.resourceJobID = curJobResourcePtr->JobId;
	}

    my_status = &JobMgr_myDeviceStatus;
    NOTIFY_OBSERVERS( my_status, observers );
}


uint32_t JobMgr_Get_devicestatus( Subject_t *s)
{
    //   ASSERT(VALIDATE_OBJECT_KEY( s, JobMgr_myDeviceStatus_t, 
    //                            subject_observer_oid_key, JobMgr_myDeviceStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return JobMgr_myDeviceStatus.status ;
}

JOB_TYPES JobMgr_Get_resourceJobType( Subject_t *s)
{
    //   ASSERT(VALIDATE_OBJECT_KEY( s, JobMgr_myDeviceStatus_t,
    //                            subject_observer_oid_key, JobMgr_myDeviceStatus_ID) );

    // message causes notify causes get so no lock needed.
    return JobMgr_myDeviceStatus.resourceJobType ;
}

uint32_t JobMgr_Get_resourceJobID( Subject_t *s)
{
    //   ASSERT(VALIDATE_OBJECT_KEY( s, JobMgr_myDeviceStatus_t,
    //                            subject_observer_oid_key, JobMgr_myDeviceStatus_ID) );

    // message causes notify causes get so no lock needed.
    return JobMgr_myDeviceStatus.resourceJobID;
}


void JobMgr_AttachObserver_devicestatus(Observer_t *o)
{
    ATTACH_OBSERVER( JobMgr_myDeviceStatus.observers, o );
}

void JobMgr_DetachObserver_devicestatus(Observer_t *o)
{
    DETACH_OBSERVER( JobMgr_myDeviceStatus.observers, o );
}


#if 0

// observer functions for plane data event subject
static void JobMgr_PlaneDataEvent_constructor(void)
{
    JobMgr_myPlaneDataEvent.pPage = NULL;
    JobMgr_myPlaneDataEvent.pPlane = NULL;
    JobMgr_myPlaneDataEvent.event_type = e_JOB_IDLE;
    JobMgr_myPlaneDataEvent.subject_observer_oid_key = JobMgr_myPlaneDataEvent_ID;
    MInitList(&JobMgr_myPlaneDataEvent.observers);

}

static void JobMgr_Notify_PlaneDataEvent( JOB_EVENT_TYPE event_type, PAGE_DATA * pPage, PLANED * pPlane )
{
    JobMgr_myPlaneDataEvent_t * my_plane_data_event;
    JobMgr_myPlaneDataEvent.event_type = event_type;
    JobMgr_myPlaneDataEvent.pPage = pPage;
    JobMgr_myPlaneDataEvent.pPlane = pPlane;
    my_plane_data_event = &JobMgr_myPlaneDataEvent;
    NOTIFY_OBSERVERS( my_plane_data_event, observers );
}

JOB_EVENT_TYPE JobMgr_Get_plane_data_event_type( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, JobMgr_myPlaneDataEvent_t, 
                                subject_observer_oid_key, JobMgr_myPlaneDataEvent_ID) );

    return JobMgr_myPlaneDataEvent.event_type ;
}
PAGE_DATA * JobMgr_Get_plane_data_event_page( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, JobMgr_myPlaneDataEvent_t, 
                                subject_observer_oid_key, JobMgr_myPlaneDataEvent_ID) );

    // message causes notify causes get so no lock needed. 
    return JobMgr_myPlaneDataEvent.pPage ;
}

PLANED * JobMgr_Get_plane_data_event_plane_data( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, JobMgr_myPlaneDataEvent_t, 
                                subject_observer_oid_key, JobMgr_myPlaneDataEvent_ID) );

    // message causes notify causes get so no lock needed. 
    return JobMgr_myPlaneDataEvent.pPlane ;
}

void JobMgr_AttachObserver_plane_data_event(Observer_t *o)
{
    ATTACH_OBSERVER( JobMgr_myPlaneDataEvent.observers, o );
}

void JobMgr_DetachObserver_plane_data_event(Observer_t *o)
{
    DETACH_OBSERVER( JobMgr_myPlaneDataEvent.observers, o );
}

#endif 
///
/// This typedef is used to tell us the state of the cancel when we are aborting a job.
///
typedef enum
{
    RECOVERY_IDLE,
    RECOVERY_ACTIVE
} RECOVERYSTATE;
#if defined(HAVE_DBG_PRINTF) || defined (HAVE_CMD)// *** If you add an enum value above, also add it to the ToString method below! ***
/**
 * \brief Return a string with the symbolic name of the given enum value.
 * 
 * \param enumValue (PRINTMODE) enum value to decode.
 * 
 * \return const char* String name for enum value.
 * 
 * This method is intended for use in debug printf calls.
 */
const char* ToString_RECOVERYSTATE( RECOVERYSTATE enumValue );
#endif // ifdef DEBUG

/*** external functions ***/

void JobMgrInit(void);
void *JobMgr (void *unused);


/*** external data ***/

/*** internal functions ***/

void *SendJobDone(JOB_DATA *);
void SendTotalPagesPrinted(JOB_DATA *, void *);
void NotifyPageDone(JOB_DATA *, SHEET_DATA *);
void ScanBuffers(ATLISTENTRY *, COLOR);  // scans the first page and frees data.
void ClearPage(PAGE_DATA *);
static void DeletePage(PAGE_DATA *page);

void UpdateDataStore(JOB_DATA *);
void SendPages(void );
void FixSendCnts(void);
void FixMajicBit(PAGE_DATA *);
void SendJobComplete (JOB_DATA *);


/// Delete the JOB, caller must remove link from lists.
static void delete_job(JOB_DATA* job);
/// Delete a single sheet from a page header.
static void DeleteSheet(SHEET_DATA *sheet);
/// Delete all the attached sheets.
static void DeleteSheets(JOB_DATA *job);
static void collate_memory_out_revert_to_onecopy(JOB_DATA* job_ptr);
static PAGE_DATA* IncrementPagesPrintedAndPossiblyDisposeOfPage( PAGE_DATA* pPageData );
void JobMgrTranslateSettings(PAGE_DATA* pPage);
uint32_t FixPage(PAGE_DATA *);
static void pagedone_freesheet( SHEET_DATA *TempSheet );
void JobAbortProcessing(void);

#if defined(HAVE_DBG_PRINTF) || defined (HAVE_CMD)

bool gActivateJobMgrPrintLists = FALSE;

void JobList_Print(ATLISTENTRY* pJobHeader);
void PageList_Print(SHEET_DATA* pSheetData);
void PlaneDList_Print(MLISTENTRY* plane_list);
void SheetList_Print(ATLISTENTRY* sheet_list);
#else  // !ifdef DEBUG
# define JobList_Print(pJobHeader)
# define PageList_Print(pSheetData)
# define PlaneDList_Print(pPlaneDHeader)
# define SheetList_Print(pSheetHeader)
#endif // !ifdef DEBUG

#if defined(HAVE_JOBINFO_ARRAY_SUPPORT)
static void  ManageJobInfo(MESSAGE *msg);
static uint8_t ManageJobInfoIndex(uint32_t id);
#endif // defined(HAVE_JOBINFO_ARRAY_SUPPORT)

/*** public data ***/
static pthread_t jobmgr_thd_id;
static mqd_t jobmgr_msgq;

static ATLISTENTRY job_list;    ///< List of the jobs currently active.
static inline JOB_DATA* GET_JOB_LIST_HEAD_JOB_PTR()
{
    ATLISTENTRY* node;
    JOB_DATA* job = NULL;
    node = ATListHead(&job_list);
    if (node != NULL)
    {
        job = CONTAINING_RECORD(node, JOB_DATA, node);
    }
    return job;
}
static inline JOB_DATA* GET_JOB_LIST_TAIL_JOB_PTR()
{
    ATLISTENTRY* node;
    JOB_DATA* job = NULL;
    node = ATListTail(&job_list);
    if (node != NULL)
    {
        job = CONTAINING_RECORD(node, JOB_DATA, node);
    }
    return job;
}

// assert that job exists; used to pacify klocwork with assertion on job ptr immediately dereferenced
// We don't replace all calls to GET_JOB_LIST_TAIL_JOB_PTR() because some do their own null checking. -jrs
static inline JOB_DATA* GET_VALID_JOB_LIST_TAIL_JOB_PTR()
{
    JOB_DATA* job = GET_JOB_LIST_TAIL_JOB_PTR();
    ASSERT(job != NULL);
    return job;
}

static RECOVERYSTATE RecoveryState = RECOVERY_IDLE; ///< State variable for cancel.
static AGMSG RecoveryType;
static void*  RecoveryPtr = NULL;
static Observer_t *PrintStatus;

static signed int currentPMQueueOpenSlots; ///< number of pages left to send to the page manager
static uint32_t ReceivedPageCount;       ///< the count of the page we are working on.
static uint8_t  PmgrMsgSent = 0;         ///< Number of pages sent to print mgr. 
static uint8_t  MsgReceived;             ///< number of cancelled received, used by PCL, need cancel from it also.
static uint8_t  JamRecovery;             ///< status of jam recovery on a cancelled page.
static uint8_t  CurrentJamState;         ///< the jam state that was sent to PCL
// static uint32_t g_TotalJobCount = 1;     ///< global job count; used to number the jobs with unique IDs.
static MESSAGE JMResourceMsg;            ///< the message recieved when we got the resource. Used when freeing the job.
static bool fJMHaveResources = FALSE;    ///< flag used to indicate if the JMResourceMsg is valid.  This is used to fix a case
                                // where we would get cancel and done at the same time and source 2 free resource messages.
                                // This really isn't a good way to fix this.  We really need to associate the resource with
                                // the job to ensure things are tracked correctly and cleaned up.  This definitely needs
                                // to be done when we support multiple jobs in print at the same time, but this will work for now. 
//

extern PrintMgrInfo gPrintMgrInfo;
extern char g_sILPrintJobName;
extern bool karasUprintSettingFlag;

void jobmgr_free_all_page_tokens()
{
    DBG_PRINTF_INFO( "entry page tokens %d, \n", currentPMQueueOpenSlots );
    while ( currentPMQueueOpenSlots < PM_QUEUE_SIZE ) {
	DBG_PRINTF_INFO( "page tokens %d, \n", currentPMQueueOpenSlots );
	    
	++currentPMQueueOpenSlots;
	sem_post(page_token_semaphore);
    }
    sem_wait(page_token_semaphore); // save one token for cancel.
    --currentPMQueueOpenSlots;
    DBG_PRINTF_INFO( "exit page tokens %d, \n", currentPMQueueOpenSlots );
}

/// only called by SJM for print jobs.
void jobmgr_free_page_token_on_cancel()
{
    sem_post(page_token_semaphore);
}

void jobmgr_limit_pages_inflight( bool collated_copies )
{
    static int holdback = 0;
    int tmp;
    
    sem_getvalue(page_token_semaphore, &tmp );
    while( tmp > 0 && (tmp + holdback) > PM_QUEUE_SIZE ) {
	DBG_PRINTF_ERR("cancel repair enter limit_pages_inflight %d %d\n ", holdback, tmp );
	sem_wait( page_token_semaphore ); // re-reserve the cancel token
	tmp--;
    }
    DBG_PRINTF_NOTICE("enter limit_pages_inflight %d %d\n ", holdback, tmp );
    if ( collated_copies ) {
	while ( holdback ) { // release any saved tokens
	    sem_post(page_token_semaphore); 
	    holdback--;
	}
    } else {
	// holdback = 0;  // recompute 
	while( tmp > (1 + PM_QUEUE_MIN_SIZE) ) {
	    sem_wait( page_token_semaphore );
	    holdback++;
	    tmp--;
	    DBG_PRINTF_NOTICE("++ limit_pages_inflight %d %d\n ", holdback, tmp );
	}
	while ( holdback  > (PM_QUEUE_SIZE - (1 + PM_QUEUE_MIN_SIZE)) ) {
	    sem_post(page_token_semaphore); 
	    holdback--;
	    DBG_PRINTF_NOTICE("-- limit_pages_inflight %d %d\n ", holdback, tmp );
	}
    }
    sem_getvalue(page_token_semaphore, &tmp );

    if ( currentPMQueueOpenSlots >= PM_QUEUE_SIZE) {
	DBG_PRINTF_ERR("Q count err %d tmp %d Q %d\n ", holdback, tmp, currentPMQueueOpenSlots );
	currentPMQueueOpenSlots = PM_QUEUE_SIZE - 1;
    } else if ( currentPMQueueOpenSlots < PM_QUEUE_MIN_SIZE) {
	DBG_PRINTF_ERR("Q count low %d tmp %d Q %d\n ", holdback, tmp, currentPMQueueOpenSlots );
	currentPMQueueOpenSlots = PM_QUEUE_MIN_SIZE;
    }
    DBG_PRINTF_CRIT("exit limit_pages_inflight holdback %d tmp %d Q %d\n ", holdback, tmp, currentPMQueueOpenSlots );
}

/*** private data ***/

static unsigned char JobMgrStack[STACK_SIZE] ALIGN8;

void jobmgr_dbg_init()
{
#ifdef HAVE_CMD
    cmd_register("jobmgr", "Job Manager Commands", NULL, NULL, NULL);
    cmd_register("jobmgr state",  "Dump all job manager state info", NULL, NULL, JobMgrPrintState);
#endif
}


/**
 * \brief Callback when a system status is updated
 * When this happens, get the status, make a message and
 * forward it to the correct task
 */
static void print_status_updater(Observer_t *observer, Subject_t *s)
{
    uint32_t CurStatus;
    MESSAGE Message;
    JOB_DATA *TempJob;

    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, PrintMgr_myDeviceStatus_ID));
    CurStatus = PrintMgr_Get_devicestatus(s);

    DBG_PRINTF_INFO("Print Curstatus %x\n", CurStatus);

    // Only forward messages if there is a pending job. There are cases where
    // there are no pending jobs, but the current status asks for restart. 
    if(ISRESTART(CurStatus) && !ATIsListEmpty(&job_list))
    {
        Message.msgType = MSG_RESTARTPAGE;
        Message.param1 = SYS_START;

        TempJob = GET_JOB_LIST_HEAD_JOB_PTR();
        if(NULL != TempJob)  // if no jobs, get out
        {
            if(NULL != (CURRENT_RESOURCE *)(TempJob->Resources))  // if no resources get out
            {
                Message.param3 = ((CURRENT_RESOURCE *)TempJob->Resources)->JobResources;  // kill current job
                SYMsgSend(JOBMGRID, &Message);
            }
        }
    }
}

static sem_t *ink_data_available_semaphore = 0;  

static void job_mgr_notify_data_avail( void )
{
    int retcode = 0; UNUSED_VAR(retcode);

    if ( ink_data_available_semaphore == 0 ) 
    {   
        ink_data_available_semaphore = posix_sem_open("/ink_data_sem", 1);
        XASSERT( ink_data_available_semaphore != NULL, thread_errno );
    }
    retcode = sem_post( ink_data_available_semaphore );
    XASSERT( retcode==0, thread_errno );
}

/*** public functions ***/

void job_mgr_block_for_data_avail( uint32_t timeout )
{
    int retcode;
    struct timespec abs_timeout;

    if ( ink_data_available_semaphore == 0 ) 
    {  
        ink_data_available_semaphore = posix_sem_open("/ink_data_sem", 1);
        XASSERT( ink_data_available_semaphore != NULL, thread_errno );
    }   

    // convert from relative OS tick timeout to absolute time.
    clock_gettime( CLOCK_REALTIME, &abs_timeout );
    posix_calc_future_usec( &abs_timeout, timeout * 10 );

    retcode = sem_timedwait( ink_data_available_semaphore, &abs_timeout );
    if (retcode == -1)
    {
        retcode = thread_errno;
        // XASSERT( ETIMEOUT == retcode, retcode ); // 3 different values for etimeout !
    }
}

void job_mgr_free_memory( COLOR color )
{
    MESSAGE Message;

    // hack reuse message...
    Message.msgType = MSG_JOB_FREE_DATA;
    Message.param1 = color;
    Message.param2 = 0;
    Message.param3 = 0;

    SYMsgSend(JOBMGRID, &Message);
    // DBG_PRINTF_WARN("%s event flags: 0x%x\n", __func__, EventFlags.tx_event_flags_current);
}

void DbgPrintTrace(bool detailed);

#define NUM_STATUS_GENERATORS 10  ///< Number of modules in print subsystem that generate status max

/* FUNCTION NAME: JobMgrInit */
/**
 * \brief Initialize and get started the JobMgr function.
 */
void JobMgrInit(void)
{
    int retcode;
#ifdef HAVE_POWER_MGR
    print_register_pwr_event();
#endif

    JobMgr_DeviceStatus_constructor();
#if 0
    JobMgr_PlaneDataEvent_constructor();

    print_jobmgr_global_page_end_observer_constructor();
    print_jobmgr_global_job_end_observer_constructor();
#endif
    posix_create_MESSAGE_queue(&jobmgr_msgq, "/JobMgrQueue", QUEUE_SIZE);
    // Register the queue with the router.
    //
    //SYRegister(JOBMGRID, jobmgr_msgq);
    router_register_queue(JOBMGRID, jobmgr_msgq);

    // Initialize the job list
    ATInitList(&job_list);

    //
    // Initialize the Page manager queue.
    //
    currentPMQueueOpenSlots = PM_QUEUE_SIZE;
    page_token_semaphore = posix_sem_open( "/page_token_sem", PM_QUEUE_SIZE );
    sem_wait( page_token_semaphore ); // save one for cancel.
    --currentPMQueueOpenSlots;
    //
    // Register the resource
    //
    rm_register(e_Printer, JOBMGRID);
    //
    // initialize the nvram variables.
    // 
    printvar_init();

    init_print_events_common();
				
    retcode = posix_create_thread(&jobmgr_thd_id, JobMgr, 0, "tJobMgr", 
                  JobMgrStack, STACK_SIZE, POSIX_THR_PRI_NORMAL);
    REL_XASSERT( retcode==0, retcode );
}  


/*** private functions ***/

/* FUNCTION NAME: GetActivePageSide */
/**
 * \brief Function or Macro to generalize and encapsulate selection of the page
 *        side that is active for a given sheet.
 * 
 * \param sheetPtr (SHEET_DATA*) Pointer to the relevant sheet data structure.
 * 
 * \return PAGE_DATA* Pointer to this sheet's current active page. A cautious
 *         caller may want to check the result for NULL, to guard against the
 *         unexpected case in which the sheet has neither front nor back page.
 *
 * When doing AutoDuplex, JobMgr receives all the data for one side of a sheet
 * first, then all the data for the other side. However, page order may be
 * either 1, 2, 3, 4, ..., or 2, 1, 4, 3, ... ; an AutoDuplex page is marked so
 * we can tell which page order to use. This allows us to decide whether the
 * "active" page side is the front or the back.
 *
 * When not duplexing, all we ever use is FrontPage.
 *
 * <pre>
 *      TRUTH TABLE for calculating GetActivePageSide()
 *      ===============================================
 * 
 *      if (BackPage == NULL) then return FrontPage.
 * 
 *      if (BackPage != NULL) then compute from table:
 * 
 *                  | BackSideFirst |      NOT      |
 *                  |    == TRUE    | BackSideFirst |
 *      ------------+---------------+---------------+
 *       FrontPage  |    return     | *** ERROR *** |
 *        == NULL   |   BackPage    |  (but return  |
 *                  |               |   BackPage)   |
 *      ------------+---------------+---------------+
 *       FrontPage  |    return     |    return     |
 *        != NULL   |   FrontPage   |   BackPage    |
 *      ------------+---------------+---------------+
 * </pre>
 */
static PAGE_DATA* GetActivePageSide(SHEET_DATA* sheetPtr)
{
    PAGE_DATA* pagePtr = NULL;

    if (sheetPtr)
    {
        pagePtr = sheetPtr->FrontPage;

        if (sheetPtr->BackPage != NULL) // only set on autoduplexer engines
        {
            if ( (sheetPtr->FrontPage == NULL) || !GetParserSendsBackSideFirst(sheetPtr->BackPage->DUPLEX) )
            {
                pagePtr = sheetPtr->BackPage;
            }
        }
        ASSERT(pagePtr == NULL || IS_PAGE_DATA(pagePtr));

    }

    return pagePtr;
}  

// 
// This implements the PJL JOB START > 1 feature,
// setting start > 1 will skip printing the pages up till the given page number.
//
//
#if 0 /* compile out feature binding */
   #define filter_messages_removing_dont_print_pages(x) false
#else
static bool  ///< return true if no further processing is desired, false if unhandleed.
filter_messages_removing_dont_print_pages(
                                         MESSAGE *Mesag ///< message to be filtered.
                                         )
{
    JOB_DATA *active_job = GET_JOB_LIST_HEAD_JOB_PTR();

    if(active_job && 
       active_job->pjob_info )
    {

        ASSERT(IS_JOB_DATA(active_job));

        DBG_PRINTF_DEBUG(
                "------- pjob-info start %d done %d\n",
                 active_job->pjob_info->pages_start, 
                 active_job->pjob_info->pages_skipped_and_printed);

        if( active_job->pjob_info->pages_start > 1 && 
            active_job->pjob_info->pages_skipped_and_printed + 1 < active_job->pjob_info->pages_start
          )
        {
            // We are discarding pages sent to us!
            switch(Mesag->msgType)
            {

                case MSG_JOBPLANEDATA:     
                    {
                        PLANED *pTempPlane = (PLANED *)Mesag->param3;
                        ASSERT(IS_PLANED(pTempPlane));
                        /* JBIG planes are NEVER empty */
                        ASSERT( NULL != pTempPlane->big_buffer );
                        BigBuffer_Free(pTempPlane->big_buffer);   
                        pTempPlane->big_buffer = NULL;
                        DBG_PRINTF_DEBUG(
                                "deleting message '%s'=0x%X=%d; param1=0x%X param2=0x%X param3=0x%08X\n",
                                 DecodeSysMsgEnum(Mesag->msgType), Mesag->msgType, Mesag->msgType,
                                 Mesag->param1, Mesag->param2, Mesag->param3);
                        MEM_FREE_AND_NULL(Mesag->param3);
                        return true;
                    }
                case MSG_JOBPAGEEND:       
			active_job->pjob_info->pages_skipped_and_printed += 1; // active_job->COPIES;

                case MSG_JOBPAGESTART:            
                case MSG_JOBSHEETSTART:   
                case MSG_JOBPLANESTART:
                case MSG_JOBPLANEEND:     

                    DBG_PRINTF_DEBUG(
                            "deleting message '%s'=0x%X=%d; param1=0x%X param2=0x%X param3=0x%08X\n",
                             DecodeSysMsgEnum(Mesag->msgType), Mesag->msgType, Mesag->msgType,
                             Mesag->param1, Mesag->param2, Mesag->param3);
                    MEM_FREE_AND_NULL(Mesag->param3);
                    return true;

                case MSG_JOBSTART: 
                case MSG_JOBEND:  
	        case MSG_JOB_FREE_DATA:
                case MSG_PAGEDONE:              
                case MSG_OUT_OF_MEMORY:    
                case MSG_CANCELJOB: 
                case MSG_CANCELPAGE:
                case MSG_RESTARTPAGE:
                case MSG_RETRYPAGE:     
                case MSG_JOBABORT:        
                case MSG_RESOURCES:     
                case MSG_TIME_TO_END:
                default:
                    break;
            }
        }
        else if(Mesag->msgType == MSG_JOBPAGEEND )
        {
            active_job->pjob_info->pages_skipped_and_printed++;
        }
        DBG_PRINTF_DEBUG("post filter message '%s'=0x%X=%d; param1=0x%X param2=0x%X param3=0x%08X\n",
                             DecodeSysMsgEnum(Mesag->msgType), Mesag->msgType, Mesag->msgType,
                             Mesag->param1, Mesag->param2, Mesag->param3);
    }
    return false; // needs more processing
}
#endif

static inline JOB_DATA* jm_get_last_job(void)
{
    return GET_JOB_LIST_TAIL_JOB_PTR();
}

static SHEET_DATA * jm_get_last_job_sheet_ptr(void)
{
    JOB_DATA *p_job;
    SHEET_DATA *p_sheet;
    ATLISTENTRY* node;

    p_job = jm_get_last_job();

    if (p_job)
    {
        ASSERT(IS_JOB_DATA(p_job));
        if ((node = ATListTail(&p_job->sheet_list)) != NULL)
        {
            p_sheet = CONTAINING_RECORD(node, SHEET_DATA, node);
            return p_sheet;
        }
    }

    return NULL;
}

/* FUNCTION NAME: JobMgr */
/**
 * \brief This is the thread that processes JobMgr messages.
 *
 */
void *JobMgr (void *unused)
{
    MESSAGE Mesag, TempMesag;
    int iRET;

    //wait for system init to finish
    SysWaitForInit();

    PrintStatus = Observer_Constructor(PrintMgr_myDeviceStatus_ID, print_status_updater);
    PrintMgr_AttachObserver_devicestatus(PrintStatus);

    print_status_attach();

    jobmgr_dbg_init();

    while(1)
    {
        //
        // Get the message from the queue.
        //

        iRET = mq_receive(jobmgr_msgq, (char*)&Mesag, sizeof(MESSAGE), 0);

	if ( iRET < 0 ) {
	    DBG_PRINTF_ERR( "bad message %d %d\n", iRET, thread_errno );
            XASSERT( iRET == sizeof(MESSAGE), thread_errno);
	}
            if (filter_messages_removing_dont_print_pages(&Mesag) ) {
                // message handled get next.
                continue;
            } 
            //DBG_PRINTF_DEBUG("Inbox: capacity=%d enqueued=%d%s\n",
            //                  Inbox.tx_queue_capacity, Inbox.tx_queue_enqueued,
            //                  ( ( Inbox.tx_queue_capacity == Inbox.tx_queue_enqueued )
            //                    ? " *** QUEUE FULL! DEADLOCK LIKELY! ***" : "" ) );

#if 0   // this stuff checks our lists for problems
# ifdef DEBUG
            {
                BOOL save_activate_job_mgr_print_lists = gActivateJobMgrPrintLists;

                gActivateJobMgrPrintLists = TRUE;
                JobList_Print( &JobList );
                gActivateJobMgrPrintLists = save_activate_job_mgr_print_lists;
            }
# endif // ifdef DEBUG
#endif  // this stuff checks our lists for problems

            //
            // Handle the messages we received.
            //

#if defined(HAVE_JOBINFO_ARRAY_SUPPORT)
            ManageJobInfo(&Mesag);   // update status info in data store
#endif // defined(HAVE_JOBINFO_ARRAY_SUPPORT)

            switch(Mesag.msgType)
            {
                case   MSG_JOB_FREE_DATA:  // video hint to free memory on plane/page boundary 
                      if(RecoveryState == RECOVERY_IDLE)   // make sure we aren't doing something else.  
                      {
                          ScanBuffers(&job_list, (COLOR)Mesag.param1);  // scans the first page and frees data that needs freeing.
                      }
                      break;

                case    MSG_JOBSTART:
                {
                    process_job_start(&Mesag);
                    break;
                }

                case   MSG_JOBSHEETSTART:
                {
                    JOB_DATA*   job_sheet_job_ptr = GET_JOB_LIST_TAIL_JOB_PTR();
                    SHEET_DATA* sheet;

                    // Add the sheet to the last job on the list.
                    //
                    ASSERT( job_sheet_job_ptr );
                    sheet = (SHEET_DATA*)Mesag.param3;
                    ASSERT(sheet != NULL);
                    ATInsertTailList(&job_sheet_job_ptr->sheet_list, &sheet->node);
                    DBG_PRINTF_CRIT("MSG_JOBSHEETSTART: Just added to tail of job_sheet_job_ptr=0x%08X's sheet=0x%08X\n", job_sheet_job_ptr, sheet );
                    JobList_Print(&job_list);
                    break;
                }
                case MSG_JOBPAGESTART:

                    process_page_start(&Mesag);
                    
                    break;

            case MSG_JOBPLANESTART:
                {
                    // Plane starts, update the job structures
                    //
                    SHEET_DATA   *TempSheet;
                    PAGE_DATA  *TempPage;
                    PLANED*     pPlane = NULL;

                    TempSheet = jm_get_last_job_sheet_ptr(); //GET_LAST_JOB_LAST_SHEET_PTR();
                    ASSERT(TempSheet);
                    TempPage = GetActivePageSide(TempSheet);
                    ASSERT(IS_PAGE_DATA(TempPage));

                    if (Mesag.param3)
                    {
                        pPlane = (PLANED*)Mesag.param3;
                        ASSERT(IS_PLANED(pPlane));
                        pPlane->DUPLEX = TempPage->DUPLEX; // only used by autoduplex printers
                        AddPlaneToPage(TempPage, pPlane);
                    }
                    
                    break;
                }

            case MSG_JOBPLANEDATA:
                {
                    SHEET_DATA *TempSheet;
                    PAGE_DATA  *TempPage;
                    PLANED*    pPlane;
                    uint16_t   Copies;

                    if (Mesag.param3 == NULL)
                    {
                        DBG_PRINTF_ERR("ERROR: Bad message: plane data param3 == NULL\n");
                        break;
                    }

                    pPlane = (PLANED*)Mesag.param3;
                    ASSERT(pPlane != NULL);
                    ASSERT(IS_PLANED(pPlane));

                    // Check to see if we have valid job, sheet, and page pointers.
                    // If not throw out the data.
                    if ( ATIsListEmpty(&job_list) ||
                         ((TempSheet = jm_get_last_job_sheet_ptr()) == NULL)
                        || ((TempPage = GetActivePageSide(TempSheet)) == NULL) )
                    {
                        DBG_PRINTF_ERR("ERROR: Bad data order, Chuck Print Data\n");
                        if (pPlane->big_buffer)
                        {
                            pPlane->big_buffer = (BigBuffer_t*)BigBuffer_Free(pPlane->big_buffer);
                        }
                        // Must not try: MEM_FREE_AND_NULL(pPlane); because pPlane is not a
                        // normal memory allocation. It resides within param3.
                        // The page does not exist for this data, free the blocks.
                        MEM_FREE_AND_NULL(Mesag.param3);       // free the link header    
                        break;          // done, get out of here.
                    }

                    ASSERT(IS_PAGE_DATA(TempPage));

                    pPlane->DUPLEX = TempPage->DUPLEX;

                    //PrintPlaneInfo( pPlane ); // Comment in/out.
                    //
                    // If copies has an illegal value, make it legal.
                    //
                    if(TempPage->COPIES == 0)
                        TempPage->COPIES = 1;
                    Copies = TempPage->COPIES;   // set up the use cnt.
                    if ( ( TempPage->COLLATE || TempPage->JamRecovery == (UINT8)eOn ) &&
                         ( GET_VALID_JOB_LIST_TAIL_JOB_PTR()->JobMode != e_KillAll ) ) // if jam recovery is else where do not do this.
                    {
                        /**
                         * We *DOUBLE* Copies here because we pass Copies as the
                         * third argument to AddRasterToPlane(PAGE_DATA
                         * *TempPage, MESSAGE *Mesag, uint16_t UseCount), and that
                         * function assigns the Copies/UseCount value to
                         * (PLANED).UseCount. When the strip has finally
                         * printed, under more or less the same condition as
                         * above, the function ClearPage() calls
                         * FreeRasterFromPage() twice as many times as one might
                         * expect, and FreeRasterFromPage() is where UseCount
                         * gets decremented. So it all seems to work itself out
                         * in the end. Why the code is designed this way is not
                         * so evident.
                         */
                        Copies *= 2;   // take care of  jam recovery
//                        DBG_PRINTF_DEBUG("[%d] *** doubled *** Copies=%d\n", 
//                                    ((JOB_DATA *) GET_JOB_LIST_TAIL_JOB_PTR())->JobNum, Copies);
                    }
                    //
                    // Now for the usecount special cases. When we are doing
                    // a cleaning page, we need to set the use count to 0
                    // since we never get a video interrupt so the use count
                    // will never decrement.
                    //
                    if ( GET_VALID_JOB_LIST_TAIL_JOB_PTR()->DocType == e_CLEANINGPAGE )
                        Copies = 0;

                    // We have the page pointer.  Put this data on the end of the correct plane
                    //
                    AddRasterToPlane(TempPage, &Mesag, Copies);
                    job_mgr_notify_data_avail();

                    // Set up the current use count so we can recover from a cancel.
                    //
                    TempPage->CurrentUseCount = pPlane->UseCount;

                    // Update the current byte count for the job
                    JOB_DATA *job = GET_VALID_JOB_LIST_TAIL_JOB_PTR();
                    job->pjob_info->num_bytes += pPlane->DataLength;
                    NOTIFY_OBSERVERS( job->pjob_info, planedata_observers );
                    break;
                }

            case MSG_JOBPLANEEND:
                {
                    SHEET_DATA  *TempSheet;
                    PAGE_DATA   *TempPage;

                    ASSERT( !ATIsListEmpty(&job_list) );
                    TempSheet = jm_get_last_job_sheet_ptr(); //GET_LAST_JOB_LAST_SHEET_PTR();
                    TempPage = GetActivePageSide(TempSheet);
                    ASSERT(TempPage && IS_PAGE_DATA(TempPage));
                    SendPlaneEnd(TempPage, &Mesag);
                }

                break;

            case MSG_JOBPAGEEND:
                {
                    //
                    // The page is finished, send this page to the page manager.
                    // param3 may carry a pixel count.
                    //
                    SHEET_DATA  *TempSheet;
                    PAGE_DATA   *TempPage;

                    // See if we have a job associated with this page.  If so continue
                    if(ATIsListEmpty(&job_list))
                    {
                        DBG_PRINTF_ERR("ERROR: No Job: ignore Job Page End\n");
                        break;     // no job get out of here.
                    }

                    TempSheet = jm_get_last_job_sheet_ptr(); //GET_LAST_JOB_LAST_SHEET_PTR();
                    if(TempSheet == NULL)
                    {
                        DBG_PRINTF_ERR("ERROR: No Sheet: ignore Job Page End\n");
                        break;            // if no sheet, get out of here.
                    }
                    TempPage = GetActivePageSide(TempSheet);
                    if(TempPage == NULL)
                    {
                        DBG_PRINTF_ERR("ERROR: No Page: ignore Job Page End\n");
                        break;             // no page, get out of here.
                    }
                    ASSERT(IS_PAGE_DATA(TempPage));
                    DBG_PRINTF_CRIT("[%d] Job Page End Received: Page %d\n", TempPage->pMyJob->JobNum, TempPage->PageCount);
                    SendPageEnd(TempPage, &Mesag);
                    //
                    // If there are 0 copies, make it at least 1 so something will come out.
                    if (TempPage->COPIES == 0)
                        TempPage->COPIES = 1;

                    TempPage->PageReadyToSend = 1;          // this page is ready to go.
                    SendPages();
                    //
                    JobList_Print(&job_list);
                    break;
                }
                //
                //
            case MSG_JOBEND:
                {
                    JOB_DATA*   job_end_job_ptr = GET_JOB_LIST_TAIL_JOB_PTR();

                    if ( job_end_job_ptr == NULL )
                        break;

                    
                    ASSERT(IS_JOB_DATA(job_end_job_ptr));

                    job_end_job_ptr->JobReceiveActive = 0;  // we are finished getting the job
                    job_end_job_ptr->JobReceived = 1;       // flag that this job is finished.

                        // The parser has tracked the number of pages received, and sent the
                        // total in param1 of the message. Multiply by the number of copies
                        // to get the total number of pages in the job. This information is used
                        // to allow the the print manager to post toner out at the correct time
                        // and stall printing. DAB 20080118
                    job_end_job_ptr->PAGECOUNT = ReceivedPageCount * job_end_job_ptr->COPIES;

                    DBG_PRINTF_CRIT("Jobnum[%d] pages %d  JOBEND\n", 
				    job_end_job_ptr->JobNum, job_end_job_ptr->PAGECOUNT);

                        //
                        // Check the job to see if it has any sheets.  If not get rid of the job.
                        //
                    if(ATIsListEmpty(&job_end_job_ptr->sheet_list ))
                    {
                        Mesag.msgType = MSG_FREERECIPE;
                        Mesag.param3 = SendJobDone( job_end_job_ptr ); // parser finished after last sheet printed.
                        job_end_job_ptr = 0;
                        SYMsgSend(SJMID, &Mesag);
                        break;          // done here.
                    }

		    //
		    // SMART DUPLEX -- todo: feature on/off, asymetric media detect, blank page generation.
		    //
                    SHEET_DATA* tempSheet = jm_get_last_job_sheet_ptr(); //GET_LAST_JOB_LAST_SHEET_PTR();

                    if (tempSheet)
                    {
                        PAGE_DATA* tempPage = NULL;

                        if (tempSheet->FrontPage && !tempSheet->BackPage)
                            tempPage = tempSheet->FrontPage;
                        else if (tempSheet->BackPage && !tempSheet->FrontPage)
                            tempPage = tempSheet->BackPage;
                        
                        if (tempPage)
							tempPage->fLastPage = 1;			// 2�� Option Tray �������� pre-pick�� ���� �ʱ� ���ؼ�, ������ �� check!!!
						else
							tempSheet->BackPage->fLastPage = tempSheet->FrontPage->fLastPage = 1;		// Duplex�� ����� ����� 

                        if (tempPage && GetAutoDuplex(tempPage->DUPLEX))
                        {
                            ASSERT(IS_PAGE_DATA(tempPage));
                            // The last sheet in the job has only one page, and it's
                            // AutoDuplex. We need to do one of two things:
                            //  A. Turn the duplex page into a simplex page; or else
                            //  B. Generate a blank companion page.
                            //REVISIT TODO: For the moment, just turn it into a simplex page.
                            DBG_PRINTF_INFO("[%d] JOBEND Change page 0x%08X AutoDuplex -> Simplex\n", 
                                            tempPage->pMyJob->JobNum, tempPage);
                            tempPage->DUPLEX = 0;
			    //SetRotation(tempPage->DUPLEX, e_Rotate180); 
			    // lsptodo: test "smart" duplex, implement check for punched/preprinted media that cant be smart duplexed.
			    // NOT the job managers responsibility to play with trays
                            tempSheet->FrontPage = tempPage;
                            tempSheet->BackPage = NULL;
			    tempSheet->sheetReady = true;
                                /**
                                 * \warning In the case of an AutoDuplex job
                                 * with an odd number of pages, sent in order
                                 * 2-1-4-3(-5) we have a *** RACE CONDITION ***
                                 * In this case, SendPages() sends page 5 as
                                 * soon as page 5 is ready, since the parser has
                                 * marked it as a BackPage, not realizing that,
                                 * as the last page of the job, it really is a
                                 * FrontPage. So JobMgr::SendPages() sends the
                                 * MSG_PAGESTART to PrintMgr while the page is
                                 * still marked as an AutoDuplex BackSide. But
                                 * the parser very soon sends us MSG_JOBEND, so
                                 * we just "hurry up" and mark the page, to
                                 * which we still have a pointer, as a Simplex
                                 * page. This seems to happen in just a few
                                 * clock ticks, whereas the messaging protocol
                                 * between PrintMgr, Video, and Engine seems to
                                 * take several tens of clock ticks before
                                 * low-level Engine firmware assigns the actual
                                 * engine codes for input tray and output bin.
                                 * So for now, we appear to work.
                                 */
                        } // end if (tempPage && GetAutoDuplex(tempPage->DUPLEX))
                    } // end if (tempSheet)


                    SendPages(); // we might have more pages to send.
                // we do not clear have resources here since we will send it again when 
                // everything is complete.  By sending a freeio we allow the next job
                // to come in and begin processing.
                //
                    DBG_PRINTF_NOTICE("Send freeio\n");
                    Mesag.param3 = (void*)job_end_job_ptr->Resources;
                    DBG_PRINTF_NOTICE("JobMgr %#x\n",((CURRENT_RESOURCE *)Mesag.param3)->Me);
                    Mesag.msgType = MSG_FREEIO;
                    SYMsgSend(SJMID, &Mesag);
                    
                    JobList_Print(&job_list);
                    break;
                } // end case MSG_JOBEND


                // This message is received from the PrintManager when a sheet is done.
                // a sheet has a front and back.
            case MSG_PAGEDONE:       // received when the page is completed.
                {
                    SHEET_DATA  *TempSheet = NULL;
                    JOB_DATA    *page_done_job_ptr = GET_JOB_LIST_HEAD_JOB_PTR();
                    ATLISTENTRY* sheet_node;
                    PAGE_DATA * tempPage = (PAGE_DATA*)Mesag.param3;
                    
                    if(RecoveryState != RECOVERY_IDLE)
                    {
                        DBG_PRINTF_NOTICE("Page Done received during cancel this is normal \n");
                    }
                    //
                    //
                    //  get rid of the page in this job. and clean up anything else that is empty
                    //
                    ASSERT( page_done_job_ptr );
                    ASSERT(IS_JOB_DATA(page_done_job_ptr));
                    
                    if ( page_done_job_ptr->pjob_info )
                    {
                        page_done_job_ptr->pjob_info->pages_printed++; // update the pages printed count.
                        if (tempPage->sheet->BackPage)
                        {
                            page_done_job_ptr->pjob_info->pages_printed++;
                        }
                    }
                    DBG_PRINTF_CRIT("Page Done job %d, page %d tokens %d\n", 
				    page_done_job_ptr->JobNum,
				    page_done_job_ptr->pjob_info->pages_printed,
				    currentPMQueueOpenSlots
			);

                    if ( page_done_job_ptr->JobMode == e_KillAll )  // alternative is e_Normal which signals
                    {
                        //   a Zeno driver (host-based) job
                        // The JobMgr forwards the MSG_PAGEDONE msg on to the
                        // job originator (typically PCL) who then knows it is
                        // safe to release this page's description.  Until the
                        // MSG_PAGEDONE message is received the job originator
                        // has to hold on to the page description since a jam
                        // or paper out could force a reprint of the page.

                        SYMsgSend( page_done_job_ptr->SendingModule, &Mesag );
                    }

                    //
                    // Now find the page that is finished.  The page pointer is sent by the page manager.
                    // This page should be on the first sheet we find in the job.  We do this check to insure
                    // that that is correct.  If it is not correct we have a problem.
                    //             
                    sheet_node = ATListHead(&page_done_job_ptr->sheet_list);
                    while ( sheet_node != NULL )
                    {
                        TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);
                        ASSERT( TempSheet );
                        if ((TempSheet->FrontPage == tempPage) || (TempSheet->BackPage == tempPage))
                        {
                            DBG_PRINTF_INFO( "[%d] found TempSheet=0x%08X; FrontPage=0x%08X, BackPage=0x%08X\n", 
                                             TempSheet->FrontPage->pMyJob->JobNum,TempSheet, TempSheet->FrontPage, TempSheet->BackPage);
                            break;
                        }
                        TempSheet = NULL;
                        sheet_node = ATListNext(&page_done_job_ptr->sheet_list, sheet_node);
                    }

                    if (TempSheet)
                    {
                        NotifyPageDone(page_done_job_ptr, TempSheet);
                        PrintJobInfo_Notify_pageend( page_done_job_ptr->pjob_info );
                        pagedone_freesheet( TempSheet );
                    }
                    else
                    {
                        // this is unexpected.  We have a page done on a page that does not exist                        
                        // what if this page was previously cancelled?
                        DBG_PRINTF_ERR("-------Job Cancel and Page Done at the same time!!\n");
                    }

                    // Now take care of Job processing
                    // If we have received all the pages and sent all the page send the
                    // job done message
                    if (( page_done_job_ptr->JobReceived) && ATIsListEmpty(&page_done_job_ptr->sheet_list))
                    {
                        Mesag.param3 = SendJobDone( page_done_job_ptr ); 
                        page_done_job_ptr = NULL;
                        DBG_PRINTF_NOTICE("Send freerecipe\n");
                        Mesag.msgType = MSG_FREERECIPE;
                        SYMsgSend(SJMID, &Mesag); // param3 is untouched and now sent back.
                    }
                    SendPages();

                    JobList_Print(&job_list);
                    break;
                }
                //
                // We received a message from somewhere saying that we are out of memory.  If there is more than 1 
                // job or if the job is printing wait for memory to be freed.  If we are building the only page 
                // that exists, start racing the engine.  Copies is reduced to 1, jam recovery is off and 
                // start printing.
                //
            case MSG_OUT_OF_MEMORY:
                // q: what about mem-out while rendering the very 1st page before job knows anything?
                //   ASSERT(JobList.LinkHead);   // if we get this message with no job, we are screwed
                if (ATNumListNodes(&job_list) == 1)
                {
                    JOB_DATA *last_job = jm_get_last_job();
                    SHEET_DATA  *TempSheet = jm_get_last_job_sheet_ptr();
                    PAGE_DATA   *TempPage;

                    ASSERT(last_job != NULL);

                    // We only have a single job.  See how many sheets we have.
                    if (TempSheet == NULL)
                    {
                        DBG_PRINTF_ERR("*** ERROR - NO PAGES AND OUT OF MEMORY ***\n");

                        // tell the status manager we are out of memory
                        // TODO: The memory out status is incorrectly being sent on behalf of the PRINTMGRID.
                        // When the memory out recovery is in place - param2 needs to be changed to the
                        // module ID that is expecting the control panel user acknowledgement of this error.

                        // special hack is caught it Status manager who sends it back here as a job cancel
                        // after the button is pressed.
                        specialMemoryOutGlobalIsSetAndItHasNoHome = TRUE;

                        JobMgr_Notify_devicestatus(STATUS_PDL_MEMORYOUT);
                    }
                    else if (ATNumListNodes(&last_job->sheet_list) == 1)
                    {
                        // We only have a single sheet.  Check the pages on this sheet.
                        // We also know we have only a single job.
                        if ((TempSheet->FrontPage && !TempSheet->BackPage) ||
                            (TempSheet->BackPage  && !TempSheet->FrontPage))
                        {
                            // we only have a single page we are working on.  Start racing the engine.
                            // Send the current page to the page manager.
                            //
                            TempPage = GetActivePageSide(TempSheet);
                            ASSERT(IS_PAGE_DATA(TempPage));
                            if (ReturnRacingMode(TempPage))
                            {
                                break;  // already done, get out of here.
                            }

                            // scan through the page data setting the usecounts appropriately
                            // so data will be freed from the page
                            FreeRasterFromPage(TempPage, TO_1_IF_UNUSED_CLEAR_IF_USED);

                            if (RecoveryState != RECOVERY_IDLE)
                            {
                                break;  // do not send pages if we are aborting a job.
                            }

                            DBG_PRINTF_NOTICE("[%d] Racing\n", TempPage->pMyJob->JobNum);

                            TempPage->COPIES = 1;           // change to only 1 copy.
                            SetRacingMode(TempPage, 1);
                            TempPage->PageReadyToSend = 1;
                            SendPages();
                        } // end if ( have exactly one page )
                    } // end else if ( have exactly one sheet )
                    else
                    {
                        TempPage = GetActivePageSide(TempSheet);
                        if (TempPage->COLLATE)
                        {
                            collate_memory_out_revert_to_onecopy(GET_JOB_LIST_TAIL_JOB_PTR());
                            SendPages(); // start printing job with copies == 1
                        }
                    } // end if collate disable collate
                } // end if ( have exactly one job )
                JobList_Print(&job_list);
                break;
                //
                // Handle the cancel
                //
            case MSG_CANCELJOB: 
            case MSG_CANCELPAGE:
            case MSG_RESTARTPAGE:
            case MSG_RETRYPAGE: 
            {
                JOB_DATA*   cancel_restart_job_ptr = GET_JOB_LIST_HEAD_JOB_PTR();
                ATLISTENTRY* sheet_node;
                uint32_t fsheet_delete_check;

                DBG_PRINTF_CRIT("got %s param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                 DecodeSysMsgEnum(Mesag.msgType),
                                 Mesag.param1, ToString_DOACTION((DOACTION)Mesag.param1),
                                 Mesag.param2, ToString_MODULE_ID((MODULE_ID)Mesag.param2),
                                 Mesag.param3);
                                 
                //if(((Mesag.msgType == MSG_RESTARTPAGE) || (Mesag.msgType == MSG_CANCELJOB)) && (Mesag.param1 == LOCAL_ACK) && (Mesag.param2 == PRINTMGRID))
                if((Mesag.param1 == LOCAL_ACK) && (Mesag.param2 == PRINTMGRID))
                {
					gPrintMgrInfo.pageCancelState = PMCANCEL_NONE;
				}

                TempMesag.msgType = Mesag.msgType;
                TempMesag.param3 =  Mesag.param3;

                //
                // If this is a cal then we ignore all of these except MSG_CANCELPAGE.
                // 
                if ( ( cancel_restart_job_ptr ) && 
                     ( cancel_restart_job_ptr->DocType == e_CALPAGE ) && 
                     ( TempMesag.msgType != MSG_CANCELPAGE ) )
                    {
                        //
                        // If we are doing a cal job then we acknowledge the cancel and 
                    // get out of here on all requests other than CANCELPAGE.
                    // In the recipe we can specify that a job cannot be canceled.  
                    // The cal runs as a print job so it can be canceled.  This prevents a cancel
                        // of a cal from occurring.  We can't make a new job for 
                        // cal since cal will share the memory buffer from the print
                        // jobs so it has to run as a print job.
                        //
                    // The reason we allow CANCELPAGE to be processed is that we really only want to block
                    // processing of user requested cancels.  This is done by blocking the CANCELJOBs and RETRIES.
                    // The CANCELPAGE must be processed to correctly allow a color laser engine to recover from
                    // error events such as top door opened during a cal.
                    //
                        TempMesag.param1 = SYS_ACK;
                        TempMesag.param2 = JOBMGRID;
                        ((JOB_RESOURCES*)Mesag.param3)->RecoveryAction = 0;  // stop the SJM from infinitely retrying the cancel
                        ((JOB_RESOURCES*)Mesag.param3)->RecoveryResource = NULL;
                        SYMsgSend(SJMID, &TempMesag);
                        DBG_PRINTF_CRIT("(%s) e_CALPAGE case - SEND ACK \n", DecodeSysMsgEnum(RecoveryType));
                        break;
                    }

                //system is requesting we do some type of job or page recovery
                if(Mesag.param1 == SYS_START)
                {
                    SHEET_DATA   *TempSheet = NULL;
                    PAGE_DATA    *TempPage = NULL;
                    //
                    // If already cleaning up, acknowledge the cancel.
                    //
                    if(RecoveryState != RECOVERY_IDLE)
                    {
						DBG_PRINTF_CRIT("(%s) Already cleaning up - SEND ACK \n", DecodeSysMsgEnum(RecoveryType));
                        goto JOB_OR_PAGE_CANCEL_DONE;
                    }
                    //
                    // See if we have the correct job to cancel
                    // 
                    
                    if(fJMHaveResources && 
                       (((CURRENT_RESOURCE *)JMResourceMsg.param3)->JobResources == TempMesag.param3))
                    {
                        // we got a cancel before we actually got a job start, simple clean up.
                        
                        TempMesag.param1 = SYS_ACK;
                        TempMesag.param2 = JOBMGRID;
                        SYMsgSend(SJMID, &TempMesag);

                        JMResourceMsg.msgType = MSG_FREERECIPE;
                        SYMsgSend(SJMID, &JMResourceMsg); 
                        DBG_PRINTF_CRIT("(%s) we got a cancel before we actually got a job start - SEND ACK & Free Recipe \n", DecodeSysMsgEnum(RecoveryType));

                        // We had resources allocated (but had not yet received
                        // the corresponding MSG_JOBSTART), and we have just now
                        // sent the message to free those resources.
                        // Clear our flag, to indicate that we need to get new
                        // resources before we accept another MSG_JOBSTART.
                        // Since we had not received MSG_JOBSTART, we had not
                        // made an entry in JobList for the now-cancelled job.
                        fJMHaveResources = FALSE;
                        break;
                    }
                        //
                        // Make sure the job at the start is the one we are cancelling.
                    if (   ( cancel_restart_job_ptr == NULL )
                        || ( ((CURRENT_RESOURCE*)cancel_restart_job_ptr->Resources)->JobResources != Mesag.param3 ) )
                    {
                        // This happens when the job finishes just as the cancel comes in.
                        // The job is deleted and when the cancel appears, we don't have 
                        // the job anymore so ack the cancel and go on.
                        Mesag.param1 = SYS_ACK;
                        Mesag.param2 = JOBMGRID;
                        SYMsgSend(SJMID, &Mesag);
                        DBG_PRINTF_CRIT("(%s) the job at the start is the one we are cancelling - SEND ACK \n", DecodeSysMsgEnum(RecoveryType));
                        break;
                    }

                    // NOTE: For the rest of the SYS_START case, we now know that
                    //
                    //       ( cancel_restart_job_ptr != NULL )
                    //
                    //       because we just did a 'break' if ( it was == NULL ).
                    //

                    ASSERT(IS_JOB_DATA(cancel_restart_job_ptr));
                    // set the status to cancelling
                    if (MSG_CANCELJOB == Mesag.msgType)
                    {
                        //
                        // We only want to set our status to cancelling if we are 
                        // doing a job cancel.  All others leave the same.
                        //
                        JobMgr_Notify_devicestatus(STATUS_INFO_CANCELING);
                        cancel_restart_job_ptr->job_cancelling = true; 
                    }

                    //save off the info we will need for response to system job manager
                    RecoveryState = RECOVERY_ACTIVE;
                    if(RecoveryType != MSG_CANCELJOB)
                    {
						RecoveryType = Mesag.msgType;
					}
                    RecoveryPtr = Mesag.param3;

                    //count how many responses we are expecting from our submodules
                    PmgrMsgSent = 0;

                    DBG_PRINTF_CRIT("[%d]: cancelling page\n", cancel_restart_job_ptr->JobNum);

                        //
                        // Get the current jamrecovery configuration for this page
                        //
                    JamRecovery = (UINT8)eOff;
                    sheet_node = ATListHead(&cancel_restart_job_ptr->sheet_list);

                        /**
                         * Even with two duplex pages per sheet, it is
                         * sufficient to send just one cancel or restart message
                         * to PrintMgr. This is because when it passes such a
                         * message along to Engine and Video, those modules
                         * clear out all active pages.
                         */
                    if (sheet_node != NULL)
                        {
                            TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);
                            TempPage = GetActivePageSide(TempSheet);
                            if (TempPage != NULL)
                            {
                                ASSERT(IS_PAGE_DATA(TempPage));
                                JamRecovery = TempPage->JamRecovery;
                            }
                        }

                        //
                        // If we have pages at the print manager, send on the recovery message.
                        //
                        /*    
                        if ((TempSheet != NULL) &&
                            (TempPage != NULL) &&
                            (TempPage->PageSent != 0))
                        */
                        if(1)		// send recovery message to PM, in every case (have pages or not)
                        {
                            //DBG_PRINTF_CRIT( "JM[%d]: Send cancel/restart/retry Msg to PrintMgr.Page %d (0x%08X)\n", cancel_restart_job_ptr->JobNum, TempPage->PageCount, TempPage);
                            DBG_PRINTF_CRIT( "Send cancel/restart/retry Msg to PrintMgr.Page \n");
                            TempMesag.msgType = RecoveryType;
                            TempMesag.param1 = LOCAL_START;
                            TempMesag.param2 = JOBMGRID;
                            SYMsgSend (PRINTMGRID, &TempMesag);
                            PmgrMsgSent++;
                        }

                    MsgReceived = 0;                // clear the number of cancelled messages received.

                    // aborting the current pages.
                    /*
                    if(PmgrMsgSent == 0)    // if we have not sent anything, send an ack message to ourself.
                    {
                        Mesag.param1 = LOCAL_ACK;
                        DBG_PRINTF_INFO( "Sending Cancel msg to ourself\n");
                        SYMsgSend(JOBMGRID, &Mesag);
                    }
                    */
                    break;
                } // end if ( SYS_START )
                //
                // This message is received after a page is cancelled.  It scans the active job  
                // and cleans up the data to allow a restarted job to finish correctly.
                // After cleaning up any remaining pages are restarted.
                //
                else if(Mesag.param1 == LOCAL_ACK)
                {
                    ATLISTENTRY* local_ack_job_node;
                    SHEET_DATA  *TempSheet;
                    UINT8      PageCnt, JobDeleted, PageDeleted;
                    JOB_MODE     OurMode = e_Normal;
                    
                    currentPMQueueOpenSlots = PM_QUEUE_SIZE - 1;

                    DBG_PRINTF_NOTICE("canceled page RecoveryState=%s\n", 
                                 ToString_RECOVERYSTATE(RecoveryState));

                    MsgReceived++;

                    //
                    // make sure we have all responses 
                    //
                    if(MsgReceived < PmgrMsgSent)
                        break;

                    //if no jobs by the time we got the last ack, then a page done came in
                    //causing the job to finish between the time we sent the cancel and got the
                    //ack. So we are done except for sending the ack
                    // Or we got a cancel before we received the job start message.
                    //
                    if ( ( cancel_restart_job_ptr == NULL ) ||
                         ( ((CURRENT_RESOURCE *)cancel_restart_job_ptr->Resources)->JobResources != RecoveryPtr ) )
                    {
                        JobMgr_Notify_devicestatus(STATUS_ONLINE);
						DBG_PRINTF_CRIT("(%s) NO JOB - SEND ACK \n", DecodeSysMsgEnum(RecoveryType));
						if(RecoveryType == MSG_CANCELJOB)
						{
							RecoveryPtr = NULL;
						}
                        goto JOB_OR_PAGE_CANCEL_DONE;
                    }

                    //reset for next cancel
                    PmgrMsgSent = 0;

                    DBG_PRINTF_NOTICE("cancel_restart_job_ptr %#x ->Resources %#x\n",
                                     cancel_restart_job_ptr, cancel_restart_job_ptr->Resources );

                    if(RecoveryType == MSG_CANCELJOB)
                    {

                        // ack so cancel job done!
                        PrintJobInfo_Notify_canceljobend( cancel_restart_job_ptr->pjob_info );
                        JobMgr_Notify_devicestatus(STATUS_JOBINFO_CANCEL);

                        //now throw out the job
                        //Scenarios:
                        //  1. We have the entire job, we just got done with the cancel
                        //     (meaning all pages have been flushed from the print path)
                        //     so toss the job.
                        //  2. We are still receiving the job over an IO channel. In this
                        //     case we need to toss the job and toss any incoming data
                        DeleteSheets(cancel_restart_job_ptr);
                        //deleted job, send the job done message and update data store
                        Mesag.param3 = SendJobDone(cancel_restart_job_ptr);

                        //release the resource here for both cases. If we are chucking the job
                        //we won't release it when done 
                        RecoveryState = RECOVERY_IDLE;
                        TempMesag.msgType = RecoveryType;
                        TempMesag.param1 = SYS_ACK;
                        TempMesag.param2 = JOBMGRID;
                        TempMesag.param3 = RecoveryPtr;
                        SYMsgSend(SJMID, &TempMesag);
                        Mesag.msgType = MSG_FREERECIPE;
                        SYMsgSend(SJMID, &Mesag);
                        DBG_PRINTF_CRIT("cancel/restart/retry done. Resume printing1. (%s) \n", DecodeSysMsgEnum(RecoveryType));
			// jobmgr_free_all_page_tokens();
                        FixSendCnts(); 
                        SendPages();    
                        JobList_Print(&job_list);
                        RecoveryType = (AGMSG)0;
                        //
                        // set the status to on-line
                        JobMgr_Notify_devicestatus(STATUS_ONLINE);
                        break;
                    } // end if ( RecoveryType == MSG_CANCELJOB )

                    // Now check the jobs on the list and clean them up if needed.
                    //
                    // Make sure we have jobs to process.
                    // Scan through all the jobs and sheets looking for pages to clean up.
                    //

                    local_ack_job_node = ATListHead(&job_list);
                    PageCnt = 0;      //number of pages cleaned up.
                    JobDeleted = 0;   //number of jobs deleted doing cleanup
                    PageDeleted = 0;  //number of pages deleted doing cleanup

                    // Go through and see if we have data left in the first PM_QUEUE_SIZE.
                    // This is based on the assumption that video can be consuming data and releasing
                    // it from more than one page at a time(mono engines don't do this but color engines do)
                    // The pages can be all single sided jobs or part of duplex jobs (front and back page) and
                    // could potentially span jobs
                    while ( ( local_ack_job_node != NULL ) && ( PageCnt < PM_QUEUE_SIZE ) )
                    {
                        //get pointer to container holding the first sheet in the list
                        cancel_restart_job_ptr = CONTAINING_RECORD(local_ack_job_node, JOB_DATA, node);
                        ASSERT(IS_JOB_DATA(cancel_restart_job_ptr));
                        sheet_node = ATListHead(&cancel_restart_job_ptr->sheet_list);

						fsheet_delete_check = FALSE;
                        //scan all the sheets in this job until have cleaned up the max
                        while ( ( sheet_node != NULL ) && ( PageCnt < PM_QUEUE_SIZE ) )
                        {
                            //get pointer to the sheet data
                            TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);

                            // See if there is a back page.  Clean up if there is.
                            if (TempSheet->BackPage != NULL)
                            {
                                if(FixPage(TempSheet->BackPage) == 0)
                                {
                                    // 
                                    // This page has been removed.  Just continue.
                                    //
                                    PageDeleted++;
                                    TempSheet->BackPage = NULL;
                                    fsheet_delete_check = TRUE;
                                }
                                else
                                {
                                    SetRacingMode(TempSheet->BackPage, 0);
                                }
                                PageCnt++;          
                            } // end if ( BackPage )

                            if(TempSheet->FrontPage != NULL)
                            {
#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
                                /**
                                 * When we restart pages, we must deal with the
                                 * case of partially completed duplex sheets.
                                 * These are duplex sheets for which the
                                 * BackPage has been printed and its memory
                                 * released, but for which the FrontPage has not
                                 * yet been printed. We must simply dispose of
                                 * such a FrontPage.
                                 * 
                                 * This case only arises when JamRecovery = OFF.
                                 * 
                                 * \todo REVISIT TODO: Do we need to look at
                                 * page order, and whether the front's send
                                 * count has been incremented? This conceivably
                                 * might occur when pages come in 1-2-3-4 order
                                 * and we have received the front but not the
                                 * back, just when some sort of jam occurs on
                                 * pages ahead of them in the print queue.
                                 */
                                //if (   (GetAutoDuplex(TempSheet->FrontPage->DUPLEX))
                                    //&& (TempSheet->BackPage == NULL) )
                                //{
                                    //DBG_PRINTF_NOTICE("JM: Delete duplex FrontPage=0x%08X that has no BackPage\n", TempSheet->FrontPage);
                                    //DeletePage(TempSheet->FrontPage);
                                    //TempSheet->FrontPage = NULL;
                                //}
                                //else
#endif // defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
                                // change the use counts on the page to the same as the last block.
                                //
                                if(FixPage(TempSheet->FrontPage) == 0)
                                {
                                    // 
                                    // This page has been removed.  Just continue.
                                    //
                                    PageDeleted++;
                                    TempSheet->FrontPage = NULL;
                                    fsheet_delete_check = TRUE;

                                    // This is for page replay on a paper out
                                    // job.
                                    if (cancel_restart_job_ptr->paper_out_all)
                                    {
                                        MESSAGE statusMsg;

                                        cancel_restart_job_ptr->paper_out_all = false;

                                        DBG_PRINTF_NOTICE("- - - - - -  paperout_all and cancel\n\n");

                                        PrintJobInfo_Set_jobstatus(cancel_restart_job_ptr->pjob_info, PAPER_OUT_CANCEL_RESEND);
                                        statusMsg.msgType = MSG_CANCELJOB;
                                        statusMsg.param1 = SYS_REQUEST;
                                        statusMsg.param2 = SJMID;
                                        statusMsg.param3 = (void*)e_Printer;
                                        
                                        SYMsgSend(SJMID, &statusMsg);    
                                    }


                                }
                                else
                                {
                                    // turn off racing.
                                    DBG_PRINTF_NOTICE("JM - - paper out cancel\n");
                                    SetRacingMode(TempSheet->FrontPage, 0);
                                }
                                PageCnt++; 
                            } // end if ( FrontPage )

                            sheet_node = ATListNext(&cancel_restart_job_ptr->sheet_list, sheet_node);

                            //
                            // if both pages are gone from the sheet, remove the sheet.
                            //
                            if((TempSheet->FrontPage == NULL) && (TempSheet->BackPage == NULL) && (fsheet_delete_check == TRUE))
                            {
                                ATRemoveEntryList(&TempSheet->node);
                                PageDeleted++; //R: REVISIT TODO: Think this is a bug: Why increment PageDeleted when we delete a Sheet?
                                MEM_FREE_AND_NULL(TempSheet);
                                DBG_PRINTF_CRIT("CAUTION!!!!! Both pages are gone!!! REMEVE the SHEET!!!!!!! (%s) \n", DecodeSysMsgEnum(RecoveryType));
                            }
                        } // end while ( ( SheetLink != NULL ) && ( PageCnt < PM_QUEUE_SIZE ) )
                        local_ack_job_node = ATListNext(&job_list, local_ack_job_node);

                        //
                        // See if this job is empty, if so delete it.
                        // parsers with OurMode set to e_KillAll can regenerate the page
                        // so if jamstate is 1 we want them to regenerate
                        // Also do not kill the job if we have not received 
                        // all the job.
                        //
                        if ( ATIsListEmpty(&cancel_restart_job_ptr->sheet_list)
                            && !( ( OurMode == e_KillAll ) && ( CurrentJamState == 1 ) )
                            &&  ( cancel_restart_job_ptr->JobReceived ) )
                        {

                            JobDeleted++;
                            PageDeleted++; //R: REVISIT TODO: Think this is a bug: Why increment PageDeleted under this condition?
                            //
                            // Send a pjl end of job message.
                            //
                            DBG_PRINTF_NOTICE("JM - - paper out cancel failed\n");
                            Mesag.param3 = SendJobDone( cancel_restart_job_ptr ); // get resources ptr, clean up job
                            cancel_restart_job_ptr = NULL;

                            Mesag.msgType = MSG_FREERECIPE;
                            SYMsgSend(SJMID, &Mesag); // param3 is untouched and now sent back.

                            JobMgr_Notify_devicestatus(STATUS_ONLINE);
                        }
                        cancel_restart_job_ptr = NULL;
                    } // end while ( ( local_ack_job_link_ptr != NULL ) && ( PageCnt < PM_QUEUE_SIZE ) )

                    //
                    // When we get here we have cleaned up all the canceled pages. Now we need to decide
                    // whether to restart them or toss them based on the recovery option
                    // If PageDeleted is still 0 here, the page still has all of its data. Throw it out
                    // for page cancels
                    //
                    if ( ( PageDeleted == 0 ) && ( RecoveryType == MSG_CANCELPAGE ) )
                    {
                        cancel_restart_job_ptr = GET_JOB_LIST_HEAD_JOB_PTR();
                        if ( cancel_restart_job_ptr )
                        {
                            sheet_node = ATRemoveHeadList(&cancel_restart_job_ptr->sheet_list);
                            if(sheet_node != NULL)
                            {
                                SHEET_DATA* sheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);

                                DeleteSheet(sheet);         // get rid of the sheet.
                                MEM_FREE_AND_NULL(sheet);

                                DBG_PRINTF_NOTICE("cancel -> online %d\n",
                                                 cancel_restart_job_ptr->JobReceiveActive );

                                JobMgr_Notify_devicestatus(STATUS_ONLINE);
                            }
                        }
                    } // end if ( ( PageDeleted == 0 ) && ( RecoveryType == MSG_CANCELPAGE ) )

                    //
                    // See if all pages in job were deleted, if so send a job done. We assume that if there
                    // are no sheets left and the job is all in, we are finished and can complete.
                    //
                    if ( cancel_restart_job_ptr != NULL )
                    {
                        DBG_PRINTF_NOTICE("Send Job Finished %d\n", cancel_restart_job_ptr->JobReceiveActive );

                        if ( ATIsListEmpty(&cancel_restart_job_ptr->sheet_list) && ( cancel_restart_job_ptr->JobReceiveActive == 0 ) )
                        {
                            // We have finished getting the data for this page.  Send job complete.
                            //
                            PrintJobInfo_Set_jobstatus( cancel_restart_job_ptr->pjob_info, CANCELLED );

                            DeleteSheets(cancel_restart_job_ptr);        // get rid of the job contents
                            Mesag.param3 = SendJobDone(cancel_restart_job_ptr);

                            cancel_restart_job_ptr = NULL;
                            JobDeleted++;

                            Mesag.msgType = MSG_FREERECIPE;
                            SYMsgSend(SJMID, &Mesag);

                            //JobMgr_Notify_devicestatus(STATUS_ONLINE);

                        }
                    }
                } // end else if ( LOCAL_ACK )
                //
                // This message is received after JobMgr has sent SYS_ACK back
                // to SysJobMgr after handling LOCAL_ACK.
                // However, JobMgr automatically resumes printing pages for jobs
                // as soon as it completes any given cancel/restart/retry op.
                // Therefore, we simply ignore the msg.SYS_RESUME variation.
                //
                else if (Mesag.param1 == SYS_RESUME)
                {
                    break;
                }
                else
                {
                    DBG_PRINTF_ERR("BAD message - ignored!\n");
                    break;
                }
                //
                // We have finished the cleanup. Resume printing  
                //
            JOB_OR_PAGE_CANCEL_DONE:
                RecoveryState = RECOVERY_IDLE;
                TempMesag.msgType = RecoveryType;
                TempMesag.param1 = SYS_ACK;
                TempMesag.param2 = JOBMGRID;
                TempMesag.param3 = RecoveryPtr;
                SYMsgSend(SJMID, &TempMesag);
                DBG_PRINTF_CRIT("cancel/restart/retry done. Resume printing2. (%s) (%d)\n", DecodeSysMsgEnum(RecoveryType), Mesag.param1);
                if(Mesag.param1 == LOCAL_ACK)
                {
					RecoveryType = (AGMSG)0;
				}

                FixSendCnts();
                SendPages();    
                JobList_Print(&job_list);
                break;
            } // end cases: MSG_CANCELJOB, MSG_CANCELPAGE, MSG_RESTARTPAGE, MSG_RETRYPAGE.

            case MSG_JOBABORT:
                // on error parser wants to abort job construction.
                // used by parsers that do partial page construction ( host based ).
                    JobAbortProcessing();
                    break;

            case MSG_ACKRECIPE:
                // no action required
                break;

            case MSG_RESOURCES:
                // save the message for later.
                ASSERT(fJMHaveResources == FALSE);    // shouldn't get resources before we've freed the current resources
                JMResourceMsg = Mesag;
                fJMHaveResources = TRUE;
                break;

            case MSG_TIME_TO_END:
                SYMsgSend(PRINTMGRID,&Mesag);   // forward on.
                break;
                
            case MSG_CONFIG_CHANGED:
				SYMsgSend(PRINTMGRID, &Mesag);
				break;

            default:
DBG_PRINTF_ERR("ERROR unknown message %#x\n", Mesag.msgType);
                break;
            }
    } // end while(1)
    return 0;
} // end JobMgr()


/* FUNCTION NAME: PageIsReadyToSend */
static bool PageIsReadyToSend(PAGE_DATA* pagePtr)
{
    // Coded this way for clarity when stepping through in a debugger.
    // Nesting the 'if's this way is equivalent to '&&' operations between the
    // if-condition operands.
    if (pagePtr != NULL)
    {
        ASSERT(IS_PAGE_DATA(pagePtr));
        if (pagePtr->PageSent < pagePtr->COPIES)
            if (pagePtr->PageReadyToSend != 0)
                if (currentPMQueueOpenSlots > 0)
                    return TRUE;
    }
    return FALSE;
} // end PageIsReadyToSend()

/* FUNCTION NAME: SendOneCopyOfPage */
static void SendOneCopyOfPage(PAGE_DATA* pagePtr)
{
    MESSAGE tmpMsg = {0};

    ASSERT(IS_PAGE_DATA(pagePtr));
    ASSERT(pagePtr->sheet);
    // Check to see fo the print mode has been configured by someone higher up the pipe.
    // If hasn't been configured then we need the set it up here. This can occur if a print
    // job comes directly to the job manager without going through a generator. This should 
    // eventually be replaced with a generic generator that handles print mode configuation for
    // all print paths.
    if( print_mode_get_input_bpp(pagePtr->print_mode) == 0 )
    {
        DBG_PRINTF_WARNING("print mode not configured!!\n");
        print_mode_get(pagePtr);
    }

    pagePtr->PageSent++;
    currentPMQueueOpenSlots--;

    tmpMsg.msgType = MSG_PAGESTART;
    tmpMsg.param3 = pagePtr;
    DBG_PRINTF_CRIT("[%d]: SendOneCopyOfPage(0x%08X) %d to PM\n", pagePtr->pMyJob->JobNum, pagePtr, pagePtr->PageSent);
    SYMsgSend(PRINTMGRID, &tmpMsg);

} // end SendOneCopyOfPage()

/* FUNCTION NAME: SendAllCopiesOfPage */
static void SendAllCopiesOfPage(PAGE_DATA* pagePtr)
{
    ASSERT(IS_PAGE_DATA(pagePtr));
    while ((pagePtr->PageSent < pagePtr->COPIES) && (currentPMQueueOpenSlots > 0))
    {
        SendOneCopyOfPage(pagePtr); // Side effects affect loop condition, ie PageSent.
    }
} // end SendAllCopiesOfPage()

/* FUNCTION NAME: SendPages */
/**
 * \brief This is called to send available pages to the print manager.  
 *
 * This will send up to PM_QUEUE_SIZE pages before quitting.
 */
void SendPages(void)
{
    ATLISTENTRY* sheet_node;
    SHEET_DATA *TempSheet=0;
    ATLISTENTRY* job_node;
    JOB_DATA *TempJob;
    
	if(gPrintMgrInfo.pageCancelState != PMCANCEL_NONE)
	{
		DBG_PRINTF_CRIT("SCH: NOT PMCANCEL_NONE!!!!!!!!!!!!\n");
		return;
	}

    if (RecoveryState != RECOVERY_IDLE)
    {
        DBG_PRINTF_CRIT( "SendPages: Ignored! RecoveryState=%s\n", 
                     ToString_RECOVERYSTATE(RecoveryState));
        return;
    }

    DBG_PRINTF_CRIT("SendPages: look for pages to send to PM. (PM Queue Open slots %d)\n", currentPMQueueOpenSlots);
    job_node = ATListHead(&job_list);
    while((job_node != NULL) && (currentPMQueueOpenSlots > 0))     // no jobs we are done.
    {
        int i = 0;
        //DBG_PRINTF_NOTICE("SendPages: JobLink=0x%08X\n", JobLink);
        TempJob = CONTAINING_RECORD(job_node, JOB_DATA, node);
        //DBG_PRINTF_CRIT("SendPages: TempJob=0x%08X\n", TempJob);
        ASSERT(IS_JOB_DATA(TempJob));

        int next_page = 1;  // 1 - N where N is number of collated copies 
                            // 1 if no colation or not all received yet.
        sheet_node = ATListTail(&TempJob->sheet_list); // peek at tail ie last page of job.
        if( sheet_node != NULL ) 
        {
            // we wait for all the pages of a collated job to be received
            // before we start sending from the front of the list of pages again.
            // peeking at tail lets us know if its time to move to the front of list. 
            TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);
            //DBG_PRINTF_CRIT("SendPages: TempSheet(Tail)=0x%08X\n", TempSheet);
            if (TempSheet->FrontPage != NULL &&
                TempSheet->FrontPage->COLLATE &&
                TempJob->JobReceived == 1 )
            {
                next_page = TempSheet->FrontPage->PageSent + 1;  
                //DBG_PRINTF_CRIT("JM: SendPages: next_page=%d\n", next_page);
            }   
        }

        /**
         * If this is a calibration job and we are not first on the list, return.
         * Not the best place to do this, should be in the print mgr or engine.
         * We cannot start a cal job until the previous print job has completed.
         */
        if((TempJob->DocType == e_CALPAGE) &&
           (job_node != ATListHead(&job_list)))
        {
			DBG_PRINTF_CRIT("SendPages: e_CALPAGE return !!!!\n");
            return;             // don't send it yet.
		}

        sheet_node = ATListHead(&TempJob->sheet_list);
        // 
        // Scan through this job's sheets, sending the pages.
        //    
        while((sheet_node != NULL) && (currentPMQueueOpenSlots > 0))
        {
            ++i;

            //DBG_PRINTF_NOTICE("SendPages: SheetLink=0x%08X\n", SheetLink);
            TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);
            //DBG_PRINTF_CRIT("SendPages: TempSheet(Head)=0x%08X\n", TempSheet);

            PAGE_DATA *pPage = TempSheet->FrontPage ? TempSheet->FrontPage : TempSheet->BackPage;
            PAGE_DATA *first_side_to_send = NULL;
            PAGE_DATA *second_side_to_send = NULL;
            bool parser_and_engine_orders_match = false;

            if (!pPage) // Sheet has no pages yet -- search no further.
                return;

            ASSERT(IS_PAGE_DATA(pPage));

            if (GetAutoDuplex(pPage->DUPLEX))
            {
                if (GetSendFrontSideFirstToEngine(pPage->DUPLEX))
                {
                    first_side_to_send = TempSheet->FrontPage;
                    second_side_to_send = TempSheet->BackPage;
                    parser_and_engine_orders_match = !GetParserSendsBackSideFirst(pPage->DUPLEX);
                }
                else
                {
                    first_side_to_send = TempSheet->BackPage;
                    second_side_to_send = TempSheet->FrontPage;
                    parser_and_engine_orders_match = GetParserSendsBackSideFirst(pPage->DUPLEX);
                }
                
                /**
                 * Here, JobMgr manages the order of pages in each duplex pair.
                 * 
                 * To have a pair of AutoDuplex pages print correctly, we must
                 * print the back side of the sheet first, run the sheet through
                 * the duplexer, then print the front side of the sheet, and
                 * finally, send the sheet to the desired output tray/bin. When
                 * such sheets go to the FaceDown output tray/bin, then the
                 * sheets stack in correct order, with the odd-numbered pages
                 * facing down, and the even-numbered pages facing up, as the
                 * user would expect.
                 * 
                 * PrintMgr knows nothing of this. We simply send PrintMgr one
                 * page at a time in order 2, 1, 4, 3, ..., i.e.,
                 * back-then-front, back-then-front, etc.
                 */
                if (true == parser_and_engine_orders_match) // Duplex page order 2, 1, 4, 3, ... .
                {
                    /**
                     * Host based parsers typically send pages in order 2, 1, 4,
                     * 3, ..., i.e., back-then-front, back-then-front, etc.
                     * 
                     * When we receive pages in this order, we can send PrintMgr
                     * the BackPage as soon as it is ready, even if the
                     * FrontPage is not yet ready.
                     * 
                     * Unfortunately, this complicates things somewhat. We must
                     * handle the first copy of each page specially, and figure
                     * out what state we are in each time we pass through here.
                     * 
                     * Fortunately, PAGE_DATA::PageSent starts at 0 and counts
                     * up to PAGE_DATA::COPIES. PageSent gets incremented in
                     * SendOneCopyOfPage(), so it's easy to tell when we've sent
                     * one, and only one, copy of a given page to PrintMgr.
                     */

                    /**
                     * Try to send first copy of first_side_to_send.
                     */
                    if (   (PageIsReadyToSend(first_side_to_send))
                        && (first_side_to_send->PageSent == 0) )
                    {
                        first_side_to_send->requestedOutTray = EngGetAutoDuplexerOutputTRAYENUM();

                        DBG_PRINTF_NOTICE("[%d] SendPages: 2-1-4-3 first_side_to_send=0x%08X\n", 
                                         first_side_to_send->pMyJob->JobNum, first_side_to_send);

                        SendOneCopyOfPage(first_side_to_send);
                    } // end if (conditions appropriate to send first_side_to_send)

                    /**
                     * Try to send first copy of second_side_to_send.
                     */
                    if (   (first_side_to_send) // In case parser said back first, but actually sent front first.
                        && (first_side_to_send->PageSent == 1)
                        && (PageIsReadyToSend(second_side_to_send))
                        && (second_side_to_send->PageSent == 0) )
                    {
                        /**
                         * In case of conflict between second_side_to_send->COPIES and
                         * BackPage->COPIES, BackPage->COPIES wins out.
                         */
                        //lsptodo:MakeFrontPageCopiesMatchBackPageCopies( second_side_to_send, first_side_to_send );

                        second_side_to_send->DEFAULTSOURCE = EngGetAutoDuplexerInputTRAYENUM();

                        DBG_PRINTF_NOTICE("[%d] SendPages: 2-1-4-3 second_side_to_send=0x%08X\n", 
                                         second_side_to_send->pMyJob->JobNum, second_side_to_send);
                        TempSheet->sheetReady = true;
                        SendOneCopyOfPage(second_side_to_send);
                    } // end if (conditions appropriate to send second_side_to_send)

                    /**
                     * Try to send remaining copies, if any, of both pages.
                     */
                    if (PageIsReadyToSend(second_side_to_send) && PageIsReadyToSend(first_side_to_send))
                    {
                        while (   (first_side_to_send->PageSent < first_side_to_send->COPIES)
                               && (currentPMQueueOpenSlots > 0) )
                        {
                            SendOneCopyOfPage(first_side_to_send);
                            TempSheet->sheetReady = true;
                            SendOneCopyOfPage(second_side_to_send);
                            // Side effects of SendOneCopyOfPage() affect loop condition.
                        }
                    } // end if (both Front and Back are ReadyToSend)
                }
                else // Duplex page order 1, 2, 3, 4, ... .
                {
                    /**
                     * Embedded language parsers typically send pages in order
                     * 1, 2, 3, 4, ..., i.e., front-then-back, front-then-back,
                     * etc.
                     * 
                     * When we receive pages in this order, we must wait for
                     * both the second_side_to_send and its associated first_side_to_send to be
                     * ready to send.
                     */
                    if (PageIsReadyToSend(second_side_to_send) && PageIsReadyToSend(first_side_to_send))
                    {
                        /**
                         * Send all copies of both first_side_to_send and second_side_to_send, in
                         * pairs. In case of conflict between second_side_to_send->COPIES
                         * and first_side_to_send->COPIES, first_side_to_send->COPIES wins out.
                         */
                        //lsptodo: MakeFrontPageCopiesMatchBackPageCopies( second_side_to_send, first_side_to_send );

                        /**
                         * Set the input tray and output tray/bin members as
                         * we need them for duplexing.
                         */
                        first_side_to_send->requestedOutTray = EngGetAutoDuplexerOutputTRAYENUM();
                        //second_side_to_send->DEFAULTSOURCE = EngGetAutoDuplexerInputTRAYENUM();

                        DBG_PRINTF_CRIT("[%d] SendPages: 1-2-3-4 first_side_to_send=0x%08X\n", 
                                         first_side_to_send->pMyJob->JobNum, first_side_to_send);
                        DBG_PRINTF_CRIT("[%d] SendPages: 1-2-3-4 second_side_to_send=0x%08X\n", 
                                         second_side_to_send->pMyJob->JobNum, second_side_to_send);
						
						if((first_side_to_send->COPIES == 1) && (second_side_to_send->COPIES != 1) && (second_side_to_send->parser_info != 0))
                        {
							DBG_PRINTF_CRIT("GDI 1 page duplex job !!!\n");
							second_side_to_send->COPIES = 1;
						}
						
						if(first_side_to_send->COLLATE == 1)
						{
							//DBG_PRINTF_CRIT("CHK : JM: first_side_to_send Collate=%d\n", first_side_to_send->COLLATE);
							if (second_side_to_send->PageSent < next_page)
							{
								//DBG_PRINTF_CRIT("CHK : JM: PageSent = PageSent : %d, %d, fLastPage = %d, %d, PageCount = %d, %d\n"
								//, first_side_to_send->PageSent, second_side_to_send->PageSent, TempSheet->FrontPage->fLastPage, TempSheet->BackPage->fLastPage
								//, TempSheet->FrontPage->PageCount, TempSheet->BackPage->PageCount);
								
								SendOneCopyOfPage(first_side_to_send);
								TempSheet->sheetReady = true;
								SendOneCopyOfPage(second_side_to_send);
								
								// after error recovery (include wake from pws) Send 1 sheet > PageDone > Send 1 sheet ...... (If it is 1 sheet job)
								// Send additional sheet to PM   
								if ((TempSheet->FrontPage->PageCount == 1) && (TempSheet->FrontPage->fLastPage == 1) 
								&& (TempSheet->BackPage->PageCount == 2) && (TempSheet->BackPage->fLastPage == 1)
								&& (TempSheet->FrontPage->PageSent < TempSheet->FrontPage->COPIES))
								{
									SendOneCopyOfPage(first_side_to_send);
									SendOneCopyOfPage(second_side_to_send);
								}
							}
						}
						else
						{
							while (   (first_side_to_send->PageSent < first_side_to_send->COPIES)
								   && (currentPMQueueOpenSlots > 0) )
							{
								SendOneCopyOfPage(first_side_to_send);
								TempSheet->sheetReady = true;
								SendOneCopyOfPage(second_side_to_send);
								// Side effects of SendOneCopyOfPage() affect loop condition.
							} // end while (PageSent < COPIES)
						}
                    } // end if (both Front and Back are ReadyToSend)
                } // end else // Page order 1, 2, 3, 4, ... .

                /**
                 * Must not send any other pages to PrintMgr until all copies of
                 * both sides of this AutoDuplex pair have been sent completely.
                 *
                 * Check only whether second_side_to_send has been sent completely.
                 * second_side_to_send cannot have been sent completely before first_side_to_send
                 * has been sent completely. However, first_side_to_send might have been
                 * sent completely, gotten done printing, and already be
                 * deallocated! In that case, we do not want to wait for
                 * first_side_to_send to be non-NULL to continue searching through the
                 * list of sheets, because it may never again be non-NULL!
                 */
                //if (   (second_side_to_send == NULL)
                    //|| (second_side_to_send->PageSent < second_side_to_send->COPIES) )
                if (second_side_to_send == NULL)    
                {
                    DBG_PRINTF_NOTICE("[%d] SendPages: return; wait on duplex second side: second_side_to_send=0x%08X first_side_to_send=0x%08X\n", 
                                     TempSheet->FrontPage->pMyJob->JobNum, TempSheet->FrontPage, first_side_to_send);
                    return;
                }
            }
            else // Simplex
            {
                if (PageIsReadyToSend(TempSheet->FrontPage))
                {
                    DBG_PRINTF_NOTICE(
                            "SendPages: Simplex job %d page %d FrontPage=0x%08X Collate %d Copies %d sent %d printed %d i %d\n",
                             TempSheet->FrontPage->pMyJob->JobNum, 
                             TempSheet->FrontPage->PageCount,
                             TempSheet->FrontPage,
                             TempSheet->FrontPage->COLLATE,
                             TempSheet->FrontPage->COPIES,
                             TempSheet->FrontPage->PageSent,
                             TempSheet->FrontPage->PagesPrinted,
                             i );
                    TempSheet->sheetReady = true;
                    if (TempSheet->FrontPage->COLLATE) 
                    {
                        if (TempSheet->FrontPage->PageSent < next_page)
                        {
                            SendOneCopyOfPage(TempSheet->FrontPage);
                            if ((TempSheet->FrontPage->PageCount == 1) && (TempSheet->FrontPage->fLastPage == 1) && (TempSheet->FrontPage->PageSent < TempSheet->FrontPage->COPIES))
                            {
								//DBG_PRINTF_CRIT("SCH: SendOneCopyOfPage 2= 0x%08X %d %d\n",TempSheet->FrontPage,TempSheet->FrontPage->PageSent,TempSheet->FrontPage->COPIES);
								SendOneCopyOfPage(TempSheet->FrontPage);
							}
                        }
                    }
                    else
                    {
                        SendAllCopiesOfPage(TempSheet->FrontPage);
                    }
                }
            } // end else // Simplex

            // go to the next sheet.
            sheet_node = ATListNext(&TempJob->sheet_list, sheet_node);
        } // end while sheets

        //if( TempSheet && TempSheet->FrontPage && TempSheet->FrontPage->COLLATE && 
            //TempJob->JobReceived != 1 && next_page != TempSheet->FrontPage->COPIES ) 
        if( TempSheet && TempSheet->FrontPage && TempSheet->FrontPage->COLLATE && next_page != TempSheet->FrontPage->COPIES ) 
        {   // in collate case only one last copy of last page of job do we allow move to next job.
            break; 
        }
        else
        {   
            if (job_node == 0)
            {
                break; // Exit the loop if no sheets and no jobs!
            }
            
            // move to next job.
            job_node = ATListNext(&job_list, job_node);
        }
    } // end while jobs
} // end SendPages()


static void job_mgr_reset_page_for_retry(PAGE_DATA *page)
{
    if ( page != NULL )
    {
        ASSERT(IS_PAGE_DATA(page));

        // Reset the "can delete data" flag. This page has been canceled and is 
        // being restarted. In the case of a color laser engine, the video may 
        // have printed to the belt and decremented the use count, and the 
        // engine will have set the "can delete data" flag true for this page. 
        page->CanDeleteData = 0;  // lsptodo: orphaned flag check ink engine usage.

        DBG_PRINTF_NOTICE("%s Job %d, FPage %d setting PageSent from %d to %d CanDeleteData = %d\n",
                    __func__, page->pMyJob->JobNum, page->PageCount, page->PageSent, page->PagesPrinted, page->CanDeleteData);
	currentPMQueueOpenSlots += page->PageSent - page->PagesPrinted;
        page->PageSent = page->PagesPrinted;
    }
}


/* FUNCTION NAME: FixSendCnts */
/**
 * /brief Fix use counts on a job.
 *
 * This is called after a cancel to reset the send pointers
 * in a job.  Allows pages to be resent.
 */
void FixSendCnts(void)
{
    ATLISTENTRY* sheet_node;
    SHEET_DATA *TempSheet;
    ATLISTENTRY* job_node;
    JOB_DATA *TempJob;

    job_node = ATListHead(&job_list);
    while(job_node != NULL)     // no jobs we are done.
    {
        TempJob = CONTAINING_RECORD(job_node, JOB_DATA, node);

        ASSERT(IS_JOB_DATA(TempJob));

        sheet_node = ATListHead(&TempJob->sheet_list);

        // Scan through the list fixing up the PageSent fields.
        while(sheet_node != NULL)
        {
            TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);

            job_mgr_reset_page_for_retry(TempSheet->FrontPage);
            job_mgr_reset_page_for_retry(TempSheet->BackPage);

            sheet_node = ATListNext(&TempJob->sheet_list, sheet_node);
        }
        job_node = ATListNext(&job_list, job_node); // go to the next job.
    }
} 


/* FUNCTION NAME: SendJobDone */
/**
 * \brief Remove job if nothing there. This checks to see if the job is empty, 
 *      if so it does the end of job processing.
 * 
 * \param TempJob (JOB_DATA*) Pointer to the job to check.
 * 
 * \return void* Pointer to resource to give back.
 * 
 * \date 12/6/2006
 * 
 **/
void *SendJobDone(JOB_DATA *TempJob)
{
	//   uint32_t       tempVal;
 //   JOBEND       *JobEndData;
    MESSAGE      TempMesag;
    UINT32       save_job_num = TempJob->JobNum;   
    void        *save_resource_ptr = TempJob->Resources;

    ASSERT(TempJob);
    ASSERT(IS_JOB_DATA(TempJob));

    PrintJobInfo_Notify_jobend(TempJob->pjob_info);
    JobMgr_Notify_devicestatus(STATUS_JOBINFO_END_JOB);
    SendTotalPagesPrinted(TempJob, save_resource_ptr);

    DBG_PRINTF_INFO( "[%d] SendJobDone: \n", TempJob->JobNum );
    
    if (GET_JOB_LIST_HEAD_JOB_PTR() == TempJob)
	ATRemoveHeadList(&job_list);    // remove job from the list
    else {
	DBG_PRINTF_INFO( "[%d] SendJobDone: MIDDLE OF LIST \n", TempJob->JobNum );
	ATRemoveEntryList(&TempJob->node);    // remove job from middle of list
    }
    delete_job(TempJob);  // free 

    if(!ATIsListEmpty(&job_list))
    {
        JOB_DATA * new_job;
        new_job = GET_JOB_LIST_HEAD_JOB_PTR();
        ASSERT( NULL != new_job );
        ASSERT( NULL != new_job->pjob_info );
        
        // Update the doc type in datastore if new job available.
        UpdateDataStore (new_job);
        JobMgr_Notify_devicestatus(STATUS_JOBINFO_START_JOB);
    }
    else
    {
        // There are no more jobs on the list, send a job end to the engine.
        //
        TempMesag.msgType = MSG_JOBEND;
        TempMesag.param1 = save_job_num;
        TempMesag.param2 = 0;
        TempMesag.param3 = NULL;
        SYMsgSend(PRINTMGRID, &TempMesag);

        JobMgr_Notify_devicestatus(STATUS_ONLINE);
    }

    DBG_PRINTF_INFO("[%d] Sent JOBDONE\n", save_job_num);

    return save_resource_ptr;
}

void SendTotalPagesPrinted(JOB_DATA *pCurJobData, void *pResourcePtr)
{
	SYS_UI_MESSAGE msg;
	error_type_t err;

	if(pCurJobData){
		 //DBG_PRINTF_CRIT( "SendTotalPagesPrinted [%d] JobType=%d Pagecount=%d PagePrinted=%d\n", pCurJobData->JobNum,
			//	 pCurJobData->pjob_info->jobDocType, pCurJobData->PAGECOUNT, pCurJobData->pjob_info->pages_printed);

		msg.msgSender = JOBMGRID;
		msg.cmd = CMD_INT_JOB_DONE_INFO;
		memset(msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

		stIC_JobDoneInfo stJobDoneInfo = {0};
		JOB_RESOURCES *tempJobResourcePtr = ((CURRENT_RESOURCE *)pResourcePtr)->JobResources;
		DBG_PRINTF_CRIT("%s Resource=0x%08X userID:%s\n", __FUNCTION__,
				tempJobResourcePtr, tempJobResourcePtr->UserId);
		strncpy(stJobDoneInfo.userID, tempJobResourcePtr->UserId, JOB_INFO_USER_ID_SIZE-1);
		stJobDoneInfo.jobNum = tempJobResourcePtr->JobId;
		strncpy(&g_sILPrintJobName, pCurJobData->pjob_info->jobname, JOB_INFO_USER_ID_SIZE-1);
		stJobDoneInfo.copies = pCurJobData->COPIES;
		stJobDoneInfo.duplex = (pCurJobData->DUPLEX == DMDUP_SIMPLEX) ? 0 : 1;
		stJobDoneInfo.jobDocType = pCurJobData->pjob_info->jobDocType;
		stJobDoneInfo.totalPages = pCurJobData->pjob_info->pages_printed;
		stJobDoneInfo.paperSize = pCurJobData->PrintedPaperSize;

		memcpy(msg.data, &stJobDoneInfo, sizeof(stIC_JobDoneInfo));
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
		XASSERT( err==OK, err );
	}
}

void NotifyPageDone(JOB_DATA *pCurJobData, SHEET_DATA *pCurSheetData)
{
	//DBG_PRINTF_CRIT("[%s]\n", __FUNCTION__);
	SYS_UI_MESSAGE msg;
	error_type_t err;
	bool sendToSystemFlag = false;

	if(pCurJobData){
		if(pCurJobData->pjob_info){
			DOCTYPE curJobDocType = pCurJobData->pjob_info->jobDocType;
			// if(curJobDocType){
			if(karasUprintSettingFlag == true) {
				sendToSystemFlag = true;
			}
			else {
				if((curJobDocType == e_FAX) || (curJobDocType == e_FAX_LONGPAGE)) {
					sendToSystemFlag = true;
				}
			}

			if(sendToSystemFlag) {
				msg.msgSender = JOBMGRID;
				msg.cmd = CMD_INT_PAGE_DONE_INFO;
				memset(msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

				stIC_PrintPageDoneInfo stPrintPageDoneInfo = {0};
				stPrintPageDoneInfo.jobNum = pCurJobData->JobNum;
				stPrintPageDoneInfo.jobDocType = curJobDocType;
				stPrintPageDoneInfo.pagesPrinted = pCurJobData->pjob_info->pages_printed;
				stPrintPageDoneInfo.pageCount = pCurJobData->PAGECOUNT;
				if(pCurSheetData){
					if(pCurSheetData->BackPage){
						stPrintPageDoneInfo.sheetPageCount = 2;
					}
					else{
						stPrintPageDoneInfo.sheetPageCount = 1;
					}
				}
				memcpy(msg.data, &stPrintPageDoneInfo, sizeof(stIC_PrintPageDoneInfo));
				err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
				XASSERT( err==OK, err );
			}
		}
	}
}


/* FUNCTION NAME: DeletePage */
/**
 * \brief Get rid of the page pointed to by TempPage
 *
 * \param TempPage (PAGE_DATA*) The page to delete.
 **/
static void DeletePage(PAGE_DATA *page)
{
    int i;
    DBG_PRINTF_CRIT("DeletePage( page=0x%08X )\n", page);
    ASSERT(IS_PAGE_DATA(page));

    FreeRasterFromPage(page, YES_CLEAR);
    currentPMQueueOpenSlots++;
    sem_post(page_token_semaphore);

    for (i=0; i<NUM_MARKING_COLOR_CHANNELS; i++) 
    {
        ASSERT(MIsListEmpty(&page->planeDataPendingPrintList[i]));
        MList_DeleteLock(&page->planeDataPendingPrintList[i]);
    }

#if defined(PIXELCNT_BYHOST) || defined(PIXELCNT_BYFW)
    // typically this structure is detached from the page and forwarded
    // to the consumables code when we are doing the page done statistics.
    // however, on a job cancel the statistics logic isn't run so check
    // and delete here if it hasn't been detached to prevent memory leaks
    // during page cancel.
    if(page->swPixCountPtr != NULL)
    {
        DBG_PRINTF_NOTICE("DeletePage freeing sw pix counts @ %#x\n", page->swPixCountPtr);
        MEM_FREE_AND_NULL(page->swPixCountPtr);
    }
#endif

    page->page_data_cookie = 0;

    // if there are any manual swaths attached to the page object, 
    // Walk the list and delete them all.
    while ( NULL != page->manual_swaths )
    {
        manual_swath_t * swath_temp = page->manual_swaths;
        page->manual_swaths = page->manual_swaths->next_swath;
        MEM_FREE_AND_NULL( swath_temp );
    }

    if (page->print_mode)
    {
        MEM_FREE_AND_NULL(page->print_mode);
    }
    MEM_FREE_AND_NULL(page);
}

static void  collate_memory_out_revert_to_onecopy(JOB_DATA* job_ptr)
{    
    SHEET_DATA *TempSheet;
    ATLISTENTRY* sheet_node;

    DBG_PRINTF_ERR("*** ERROR - OUT of MEMORY can't COLLATE ***\n");

    if ( job_ptr == NULL )
        return;

    ASSERT(IS_JOB_DATA(job_ptr));

    // for each page set use counts to 1, set collate off set copies 1   
    sheet_node = ATListHead(&job_ptr->sheet_list);
    while (sheet_node != NULL)
    {
        TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);
        sheet_node = ATListNext(&job_ptr->sheet_list, sheet_node);

        ASSERT(TempSheet->FrontPage != NULL);

        if (TempSheet->FrontPage->PagesPrinted > 0)
        {
            if (TempSheet->FrontPage) // already printed 1 copy, gave back one slot
                --currentPMQueueOpenSlots;
            if (TempSheet->BackPage) --currentPMQueueOpenSlots;
            ATRemoveEntryList(&TempSheet->node);
            DeleteSheet(TempSheet);         // get rid of the sheet.
            MEM_FREE_AND_NULL(TempSheet); // now that sheet is finished, get rid of the sheet.
        }
        else
        {
            TempSheet->FrontPage->COPIES = 1;
            TempSheet->FrontPage->COLLATE = 0;
        }
    }
}

/* FUNCTION NAME: DeleteSheet */
/**
 * \brief Delete a sheet from a page header
 *
 * \param TempSheet (SHEET_DATA*) Pointer to the sheet to delete.
 **/
static void DeleteSheet (SHEET_DATA *TempSheet)
{
    DBG_PRINTF_CRIT("DeleteSheet(TempSheet=0x%08X)\n", TempSheet);
    if(TempSheet->FrontPage != NULL)           // check pages in the sheet
    {
        // get rid of everything
        DeletePage(TempSheet->FrontPage);
        // 
        // This page has been removed.  Just continue.
        //
        TempSheet->FrontPage = NULL;
    }

    // See if there is a back page.  Clean up if there is.
    if(TempSheet->BackPage != NULL)            // check pages in the sheet.
    {
        DeletePage(TempSheet->BackPage);
        TempSheet->BackPage = NULL;
    }
} // end DeleteSheet()

/* FUNCTION NAME: DeleteSheets */
/**
 *  \brief Get rid of sheets and pages in a job, but NOT the job memory !
 */
static void DeleteSheets(JOB_DATA* job)
{
    ATLISTENTRY* sheet_node;
    SHEET_DATA  *TempSheet;

    if(job == NULL)
        return;
    DBG_PRINTF_NOTICE("DeleteSheets() job=0x%08X\n", job);
    ASSERT(IS_JOB_DATA(job));

    while((sheet_node = ATRemoveHeadList(&job->sheet_list)) != NULL)   // check all the sheets
    {
        TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);
        DeleteSheet(TempSheet);         // get rid of the sheet.
        // now that sheet is finished, get rid of the sheet.
        MEM_FREE_AND_NULL(TempSheet);
    }

    DBG_PRINTF_NOTICE("DeleteSheets() done");
}

/*
   \brief Delete the JOB attached and the link passed in, caller must remove link from lists.
   \param[in] link The link to delete
 */
static void delete_job(JOB_DATA* job)
{
    if (job) 
    {
        DBG_PRINTF_DEBUG("-------------------- delete job\n" );
        /// real destructor would be nice.
        DBG_PRINTF_NOTICE("---*** delete job%x \n", job->pjob_info );
        ASSERT(IS_JOB_DATA(job));
        PrintJobInfo_destructor(job->pjob_info); 
        job->pjob_info = NULL;

        // free oem data if it exists.
        if(job->oem_data != NULL)
        {
            MEM_FREE_AND_NULL(job->oem_data);
        }

        MEM_FREE_AND_NULL(job);      // get rid of the job memory
    }
}


/* FUNCTION NAME: FixPage */
/**
 * \brief Given a page that has partially printed, clean up the data.
 *
 *  If the data
 *  has already been deleted or we are racing, get rid of the data.
 *
 *  If all the data is in the printer, fix the usecounts and return.
 *
 * \return uint32_t
 * \retval 0 We already deleted something here. Page is cleaned up.
 * \retval 1 Page needs its use counts updated.
 */
uint32_t FixPage(PAGE_DATA *PageHead)
{
    // See if we have already deleted something here.  If so clear the page and exit.
    // If we were racing this page, throw out everything.
    DBG_PRINTF_NOTICE( "[%d]: FixPage DataDeleted %d Racing %d\n",
               PageHead->pMyJob->JobNum, PageHead->DataDeleted, ReturnRacingMode(PageHead));
    ASSERT(IS_PAGE_DATA(PageHead));

    if((PageHead->DataDeleted == 1))
    {
        DBG_PRINTF_INFO( "[%d] FixPage Deleting Page %d, Job %d\n", 
                   PageHead->pMyJob->JobNum, PageHead->PageCount, PageHead->pMyJob->JobNum);
        // Yes we started deleting from this page.
        //
        DeletePage(PageHead);
        return(0);              // all done here, return
    }

    //
    // The page has to have its use counts updated.  Get the use count from the last block and
    // use that for all the others.
    //
    UpdatePlanes(PageHead);
    return(1);
}

/* FUNCTION NAME: ScanBuffers */
/**
 * \brief When we get a message from the ISR saying that it has updated some
 *        use counts, this routine is called to release any blocks of data
 *        that have their use counts at zero.
 *
 * \param job_list_local Pointer to head of current job list to scan.
 * \param color - the color of the data plane
 * \return void
 */
void ScanBuffers (ATLISTENTRY *job_list_local, COLOR color)
{
    SHEET_DATA  *TempSheet;
    JOB_DATA *TmpJobData;
    ATLISTENTRY* job_node;
    ATLISTENTRY* sheet_node;
    uint32_t i;

    if(ATIsListEmpty(job_list_local))
    {
        return;                 // we have already cleaned up.
    }

    job_node = ATListHead(job_list_local);
    ASSERT(job_node);
    TmpJobData = CONTAINING_RECORD(job_node, JOB_DATA, node);
    ASSERT(IS_JOB_DATA(TmpJobData));
    if(ATIsListEmpty(&TmpJobData->sheet_list))
    {
        return;
    }

    sheet_node = ATListHead(&TmpJobData->sheet_list);
    for(i = 0; (i < PRINT_SCAN_Q_DEPTH) && sheet_node; i++)
    {
        TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);

        if (TempSheet->FrontPage)
        {
            TempSheet->FrontPage->DataDeleted |= FreeRasterFromPage(TempSheet->FrontPage, CLEAR_IF_USECOUNT_IS_ZERO);
        }

        if (TempSheet->BackPage)
        {
            TempSheet->BackPage->DataDeleted |= FreeRasterFromPage(TempSheet->BackPage, CLEAR_IF_USECOUNT_IS_ZERO);
        }

        sheet_node = ATListNext(&TmpJobData->sheet_list, sheet_node);
    }
}

/* FUNCTION NAME: ClearPage */
/**
 * \brief Given a Page of data, release the data if racing,
 *        or decrement the use count and release if not racing.
 */
void ClearPage(PAGE_DATA *Page)
{
    ASSERT(IS_PAGE_DATA(Page));

    Page->CurrentUseCount--;                 // decrement use count.
    if((ReturnRacingMode(Page) == 1) ||
       (Page->JamRecovery == (UINT8)eOn) ||
       Page->COLLATE) // collated pages need jamrecovery
    {
        FreeRasterFromPage(Page, BY_1);
        Page->CurrentUseCount--;
    }
    // we are not racing, make sure we have cleared everything.
    SanitizeUseCounts(Page);  // verify page use count matches plane use counts, and free if zero
}



/*
void DbgPrintTrace(BOOL detailed)
{

   UINT32 i = 0;
   UINT16 code;
   UINT16 channel, info;
   char* str;

 //  DBG_PRINTF_NOTICE("Video Trace\n");

   if (detailed == FALSE)
   {
      while ((Trace[i].Code != 0) && (i < 10000))
      {
       //  DBG_PRINTF_NOTICE("%#x: %#x %#x %#x %#x %#x %#x %#x %#x %#x %#x\n",i,Trace[i].Code,Trace[i+1].Code,Trace[i+2].Code,
          //       Trace[i+3].Code, Trace[i+4].Code, Trace[i+5].Code, Trace[i+6].Code, Trace[i+7].Code,
        //         Trace[i+8].Code, Trace[i+9].Code);
         i += 10;
}
   }
   else
   {
    //  DBG_PRINTF_NOTICE("C    Y    M    K\n");

      while ((Trace[i].Code != 0) && (i < 10000))
      {
         info = Trace[i].Code & 0x00FF;
         code = Trace[i].Code >> 12;
         channel = (Trace[i].Code & 0x0F00) >> 8;

         if (code == 2)
         {
            if (info == 0x01)
            {
               str = "TOP";
            }
            else if (info == 0x20)
            {
               str = "EOS";
            }
            else if (info == 0x02)
            {
               str = "EOP";
            }
            else
            {
               str = "EOP";
            }
         }
         else if (code == 4)
         {
            if (info == 100)
               str = "Q100";
            else
               str = "Qend";
         }
         else
         {
            str = "?";
         }


         switch (channel)
         {
            case 0:
            //   DBG_PRINTF_NOTICE("%s\n", str);
               break;
            case 1:
             //  DBG_PRINTF_NOTICE("     %s\n", str);
               break;
            case 2:
             //  DBG_PRINTF_NOTICE("          %s\n", str);
               break;
            case 3:
             //  DBG_PRINTF_NOTICE("               %s\n", str);
               break;
         }

         i ++;
      }        
   }

}
*/


/* FUNCTION NAME: UpdateDataStore */
/**
 * \brief Given Job data TempJob, update the data store with the info
 *        for this job.  This info is used in PJL and the front panel.
 */
void UpdateDataStore(JOB_DATA *TempJob)
{
	/// LSPTODO kill this missnamed function 
	//uint32_t     tempVal;



    // tempVal = TempJob->DocType;
    //printvar_set_doctype(&tempVal);
    TempJob->JobStartSent = 1;      // set the flag saying the message has been sent.
}


static void pagedone_freesheet( SHEET_DATA *sheet )
{
    if (sheet->BackPage)
    {
        sheet->BackPage = IncrementPagesPrintedAndPossiblyDisposeOfPage(sheet->BackPage);
    }
    if (sheet->FrontPage)
    {
        sheet->FrontPage = IncrementPagesPrintedAndPossiblyDisposeOfPage(sheet->FrontPage);
    }

    JOB_DATA  *page_done_job_ptr = GET_JOB_LIST_HEAD_JOB_PTR();

    // If the sheet is empty, get rid of it.
    //
    if ((sheet->FrontPage == NULL) && (sheet->BackPage == NULL))
    {
        ATLISTENTRY* node;
        SHEET_DATA* head_sheet = 0; UNUSED_VAR(head_sheet); 
        XASSERT(page_done_job_ptr && IS_JOB_DATA(page_done_job_ptr), (uint32_t)page_done_job_ptr);

        node = ATRemoveHeadList(&page_done_job_ptr->sheet_list);
        ASSERT(node != NULL);
        head_sheet = CONTAINING_RECORD(node, SHEET_DATA, node);
        ASSERT(head_sheet != NULL);

        // this assert checks the assumption built into the code that the item pulled off
        // the head of the sheet list in the line above actually matches the passed in sheet.
        ASSERT(head_sheet == sheet);

        MEM_FREE_AND_NULL(sheet);
    }
/*
    else
    {
        DBG_PRINTF_NOTICE(
                "didn't free sheet: %d page %d FrontPage=0x%08X Collate %d Copies %d sent %d printed %d link %x \n",
                 sheet->FrontPage->pMyJob->JobNum, 
                 sheet->FrontPage->PageCount,
                 sheet->FrontPage,
                 sheet->FrontPage->COLLATE,
                 sheet->FrontPage->COPIES,
                 sheet->FrontPage->PageSent,
                 sheet->FrontPage->PagesPrinted,
                 sheet->FrontPage->KPlane.LinkHead
                    );
    }
*/        
}

//------------------------------------------------------------------------------
/* FUNCTION NAME: IncrementPagesPrintedAndPossiblyDisposeOfPage */
/**
 * \brief Accounts for number of copies of this page that have been printed,
 *        and cleans up if all copies have been printed.
 *        Since memory is freed the proper usage is:
 *        ptr = function(ptr);  // setting your pointer to null or leaving it alone.
 *
 * \param pPageData PAGE_DATA* Pointer to PAGE_DATA in question.
 *                             
 *
 * - pPageData is a pointer to the PAGE_DATA in question.
 * - pPageData->member is a member of the PAGE_DATA in question.
 *
 */
static PAGE_DATA * IncrementPagesPrintedAndPossiblyDisposeOfPage( PAGE_DATA* pPageData )
{
    if (pPageData)
    {
        DBG_PRINTF_NOTICE(
                "increment and ...: Job %d, Page %d, CurrentUseCount %d\n",
                 pPageData->pMyJob->JobNum,
                 pPageData->PageCount,
                 pPageData->CurrentUseCount);

        ASSERT(IS_PAGE_DATA(pPageData));

        ClearPage(pPageData);
        pPageData->PagesPrinted++; // The number of copies printed of this page.

        if (pPageData->COPIES == pPageData->PagesPrinted)  /// bug can't happen because of copies * 2 
        {
            DBG_PRINTF_NOTICE("[%d] Page Done; Free Memory: Page %d, PAGE_DATA*=0x%08X\n",
                                 pPageData->pMyJob->JobNum,
                                 pPageData->PageCount,
                                 pPageData);
#if defined(HAVE_ZPSL3_SUPPORT)
            if (pPageData->foreignPageHandle)
            {
                GenesisToForeignCode_NotifyPagePrinted(pPageData->foreignPageHandle);
            }
#endif // defined(HAVE_ZPSL3_SUPPORT)
            DeletePage(pPageData);
            pPageData = NULL;
        }
	else
	{
            DBG_PRINTF_NOTICE("[%d] Page Done; copies Page %d : of %d, PAGE_DATA*=0x%08X\n",
                                 pPageData->pMyJob->JobNum,
                                 pPageData->PageCount,
                                 pPageData->COPIES,
                                 pPageData);
	    currentPMQueueOpenSlots++;  
            // free slot will be double incremented in memory out case
	    // see collate_memory_out_revert_to_onecopy() for matching decrement, in that case
	}
    }
    return pPageData;
} // end IncrementPagesPrintedAndPossiblyDisposeOfPage()


/* FUNCTION NAME: JobMgrTranslateSettings */
/******************************************************************************
 * Description: Translate driver settings to internal settings where appropriate
 *
 ******************************************************************************/
void JobMgrTranslateSettings(PAGE_DATA* pPage)
{
    ASSERT(IS_PAGE_DATA(pPage));

    //driver can send two different custom codes, internally they are the
    //same to us so we map them all to the first kind
    if(pPage->PAPER == MEDIASIZE_CUSTOM_USER2)
        pPage->PAPER = MEDIASIZE_CUSTOM_USER;

}


/**
 * \brief Process a pagestart message from the system
 * \param Mesag The message that was received.
 */
void process_page_start(MESSAGE *Mesag)
{
//    MESSAGE msg = {0};
    //
    //  Add a page to the last sheet on the list.
    //
    ASSERT( !ATIsListEmpty(&job_list) );
    SHEET_DATA* TempSheet = jm_get_last_job_sheet_ptr(); //GET_LAST_JOB_LAST_SHEET_PTR();
    PAGE_DATA*  pPage = (PAGE_DATA*)Mesag->param3;
    ASSERT(pPage != NULL);
    ASSERT(IS_PAGE_DATA(pPage));
    ASSERT(TempSheet != NULL);
    JOB_DATA*   job_page_start_job_ptr = GET_VALID_JOB_LIST_TAIL_JOB_PTR();

    //
    // Initialize page variables.
    //
    pPage->PageCount = ++ReceivedPageCount;  // number of active pages in current job; could be scoped better to print job object
    pPage->PagesPrinted = 0; // incremented when paper is safe.
    pPage->PageSent = 0; // incremented when sent to print mgr ?
    pPage->DataDeleted = 0;
    pPage->CanDeleteData = 0;
    pPage->pMyJob = job_page_start_job_ptr;
    pPage->ImageOption = pPage->pMyJob->ImageOption;
    pPage->parser_info = pPage->pMyJob->parser_info;
    job_page_start_job_ptr->PAGECOUNT += 1;

    DBG_PRINTF_CRIT("Page Start Received: Job %d, Page %d @ 0x%08X\n", pPage->pMyJob->JobNum, pPage->PageCount, pPage);
	if(TempSheet != NULL)
    {
		DBG_PRINTF_CRIT("Page Start Received: sheet_ptr : 0x%08X\n", TempSheet);
	}
	
    NOTIFY_OBSERVERS( job_page_start_job_ptr->pjob_info, pagestart_observers );

    // Print out the page type, is set in the parser or by the
    // internal page job.
    if ( pPage->page_output_color_mode == e_Mono )
    {
        DBG_PRINTF_NOTICE("Mono Job\n");
    }
    else if ( pPage->page_output_color_mode == e_Color )
    {
        DBG_PRINTF_NOTICE("Color Job\n");
    }
    else
    {
        DBG_PRINTF_NOTICE("********* PAGE COLOR MODE NOT SET **********\n");
        ASSERT(0);
    }

     
//        pPage->PageReadyToSend = 0;      // this is not ready to send.
    {
    /**
     * In DataStore, e_JamRecovery may be eOff, eOn, or eAuto.
     * From JobMgr down, JamRecovery must be either eOff or eOn.
     * For now, eAuto becomes eOn. Some products may want to
     * decide this based on the amount of memory available.
     */

        uint8_t JamRec;

        printvar_get_jamrecovery(&JamRec);
        pPage->JamRecovery = ((uint8_t)(JamRec) ? eOn : eOff);
    }

    if (GetAutoDuplex(pPage->DUPLEX))
    {
        if (GetBackSide(pPage->DUPLEX))
        {
            TempSheet->BackPage = pPage;
            DBG_PRINTF_CRIT("Page Start Received: DUPLEX(Back) : Sheet 0x%08X  Page 0x%08X\n", TempSheet, pPage);
		}
        else
        {
            TempSheet->FrontPage = pPage;
            DBG_PRINTF_CRIT("Page Start Received: DUPLEX(Front) : Sheet 0x%08X  Page 0x%08X\n", TempSheet, pPage);
		}
    }
    else // Simplex; therefore Front side.
    {
        TempSheet->FrontPage = pPage;
        DBG_PRINTF_CRIT("Page Start Received: SIMPLEX : Sheet 0x%08X  Page 0x%08X\n", TempSheet, pPage);
    } // end else // Front side.

    //map driver settings to internal settings
    JobMgrTranslateSettings(pPage);
//    lspto: why here this is page not plane? : 
// JobMgr_Notify_PlaneDataEvent( e_JOB_PAGE_START, pPage, NULL );

    if (pPage->COLLATE && pPage->COPIES > 1 && pPage->PageCount > PM_QUEUE_SIZE - 2 ) 
    {
        // number of active pages in current job; could be scoped better to print job object
        DBG_PRINTF_NOTICE("[%d] COPIES set to 1 copies %d cnt %d Q %d \n", 
			  pPage->pMyJob->JobNum, pPage->COPIES, pPage->PageCount, PM_QUEUE_SIZE );
        collate_memory_out_revert_to_onecopy(job_page_start_job_ptr);
    }
    SendPageStart(pPage);
    SendPages();     // if needed send the pages.
    //
    // Now send the pre-start info.
// removed the prestart message; seemed to be causing system instability with regard to paper-out/cartridge out
//    msg.msgType = MSG_PAGEPRESTART;
//    msg.param3 = pPage;
//    SYMsgSend(PRINTMGRID, &msg);  // send it on.
    JobList_Print(&job_list);
}


/**
 * \brief Process a jobstart message from the system
 * \param Mesag The message that was received.
 */
void process_job_start(MESSAGE *Mesag)
{
    MESSAGE TempMesag;

    JOB_DATA* job_start_job_ptr = (JOB_DATA*)Mesag->param3;

    ASSERT(IS_JOB_DATA(job_start_job_ptr));

/* TODO
    if (e_DOCUMENT == job_start_job_ptr->DocType)
    {
        Printer_Notify_host_print_job( true );
    }
*/

    ReceivedPageCount = 0;
    job_start_job_ptr->pjob_info->num_bytes = 0;
    job_start_job_ptr->JobReceiveActive = 1;        // we are getting a job
    job_start_job_ptr->JobStartSent = 0;            // we have not sent the start yet.
    job_start_job_ptr->JobReceived = 0;             // initialize the flag to tell when we have finished a job.
    job_start_job_ptr->paper_out_all = false;

    job_start_job_ptr->JobNum = job_start_job_ptr->pjob_info->jobnum;
    job_start_job_ptr->pjob_info->job = job_start_job_ptr;
    job_start_job_ptr->mismatch_continue = 0;

    jobmgr_limit_pages_inflight( job_start_job_ptr->COLLATE && job_start_job_ptr->COPIES > 1 );

    ATInitList(&job_start_job_ptr->sheet_list);

    // Make sure we are part of a job, if not get out.
    //
    ASSERT(fJMHaveResources);

    DBG_PRINTF_NOTICE("[%d] JOBSTART\n", job_start_job_ptr->JobNum);

    
    //
    // First job in list, update job variables in data store
    //
    if (ATIsListEmpty(&job_list))
    {
        //PrintJobInfo_Notify_jobstart(job_start_job_ptr->pjob_info ); -- Job start notify already sent from copy constuctor of parser
        UpdateDataStore (job_start_job_ptr);
        //JobMgr_Notify_devicestatus(STATUS_JOBINFO_START_JOB);

        //
        // If cal data, get everything running.
        //
        if ( job_start_job_ptr->DocType == e_CALPAGE )
        {
            //send cal start to print manager and wait for engine to
            //request the needed page
#ifdef COLOR_ENG
         // REVISIT DAB    StartCal();
#endif 
        }
    }
  
    // Add the job to the linked list and exit

    // Put the resource info into the job header.
    job_start_job_ptr->Resources = JMResourceMsg.param3;
    fJMHaveResources = FALSE;
    //20170518:L Set SJM's job ID to print_job_t
    ((print_job_t*) job_start_job_ptr->oem_data)->job_id =
            ((CURRENT_RESOURCE *) job_start_job_ptr->Resources)->JobResources->JobId;
    job_start_job_ptr->oem_data = NULL;

    DBG_PRINTF_INFO( "[%d] Jobstart Resource 0x%08X\n",job_start_job_ptr->JobNum, job_start_job_ptr->Resources);
    ATInsertTailList(&job_list, &job_start_job_ptr->node); // put it on the list.
    JobMgr_Notify_devicestatus(STATUS_JOBINFO_START_JOB);
    // Let the print manager know that a job is starting. Print manager 
    // uses this information to manage engine "pre warmup" operations
    // to optimize laser print performance.
    TempMesag.msgType = MSG_JOBSTART;
    TempMesag.param1 = 0;
    TempMesag.param2 = 0;
    TempMesag.param3 = (void*)job_start_job_ptr;
    SYMsgSend(PRINTMGRID, &TempMesag);

    JobList_Print(&job_list);
}
/**
 * \brief Handle the processing of jobabort.
 * 
    // We have a partial job in and have received a parse error.  
    // Kill most of the last job on the job list.
    // The reason this is done this way is to prevent a very complex 
    // cancel.  If we just cancel from here then we can have the situation
    // where we have one job in progress and a page from the 2nd job staged.
    // Now for a cancel of the 2nd job, which this would be, we have to
    // unstage a single page and continue the 1st job without messing
    // up the 1st job.  Much easier to continue the 2nd job and cancel
    // any partial pages in the 2nd job.  That is what we do here.
    //Parsers send this message if they encounter a problem while parsing 
    //a job. We need to delete the job we are currently receiving.
    //Here are the scenarios we need to handle here:
    // 1. Not the first job in the list, so we are receiving the job 
    //    but not printing it. Delete the entire job
    // 2. First job in the list, so we are receiving some pages while
    //    printing others.  Delete the final page in the list if we
    //    haven't sent it to the engine because we don't know if
    //    it is a full page. Let all other pages print
 */
void JobAbortProcessing(void)
{
    ATLISTENTRY* sheet_node;
    JOB_DATA *TempJob;
    SHEET_DATA *TempSheet;

    DBG_PRINTF_CRIT("ABORT JOB!!!!!!!!\n");
    //
    //
    //
    if (RecoveryState != RECOVERY_IDLE)
    {
        //
        // if we are cancelling, clear the job receive active and let
        // the cancel code clean up the rest of the code.
        //
        if (ATIsListEmpty(&job_list)) 
        {
            DBG_PRINTF_NOTICE("ABORT JOB No Job\n");
            return; // if nothing there get out.
        }

        //
        // Since we had a parser error, we need to say that we have received all 
        // the data for the current job.  This will allow the cancel code
        // to clean up correctly.
        //
        DBG_PRINTF_NOTICE("ABORT JOB RecoverState != Recover_IDLE\n");
        GET_VALID_JOB_LIST_TAIL_JOB_PTR()->JobReceiveActive = 0;
        return;      // done here.
    }

    if (ATIsListEmpty(&job_list))
    {
        DBG_PRINTF_NOTICE("ABORT JOB No Job 2\n");
        // The only way we can get here is if we had a cancel pressed on the front panel
        // and then they deleted the job from the spooler.
        return;
    }

    TempJob = GET_VALID_JOB_LIST_TAIL_JOB_PTR();
    ASSERT(IS_JOB_DATA(TempJob));
    TempJob->job_data_aborted = true;

    // NOTE: network protocols like IPP require notification of aborted/canceled jobs even if we
    // don't have any sheets at this time 
    PrintJobInfo_Set_jobstatus( TempJob->pjob_info, PARSEERROR );
    PrintJobInfo_Notify_canceljobend( TempJob->pjob_info );
    // Fix this: Temporarily commented out because STATUS_JOBINFO_JOB_ABORTED needs
    // to be defined across multiple projects.
    //JobMgr_Notify_devicestatus(STATUS_JOBINFO_JOB_ABORTED);

    // 
    // See if we have any sheets to check
    //
    sheet_node = ATListTail(&TempJob->sheet_list);
    if (sheet_node != NULL)
    {
        // We have a sheet.
        //
        TempSheet = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);

        //
        // Check here for data in the pages.
        //
        if ((TempSheet->BackPage != NULL) && (TempSheet->BackPage->PageSent == 0))
        {
			DBG_PRINTF_CRIT("ABORT of Back Page %x\n",TempSheet->FrontPage);
            // We have a back page.  Clear out the page.
            //
            DeletePage (TempSheet->BackPage);
            // 
            // This page has been removed.  Just continue.
            //
            TempSheet->BackPage = NULL;
        }
        else if ((TempSheet->FrontPage != NULL) && (TempSheet->FrontPage->PageSent == 0))
        {
            DBG_PRINTF_CRIT("ABORT of Front Page %x\n",TempSheet->FrontPage);
            DeletePage (TempSheet->FrontPage);
            // 
            // This page has been removed.  Just continue.
            //
            TempSheet->FrontPage = NULL;
        }

        if ((TempSheet->BackPage == NULL) && (TempSheet->FrontPage == NULL))
        {
            // we have a sheet with nothing.  Get rid of this sheet.
            //
            DBG_PRINTF_NOTICE("ABORT Get rid of sheet\n");
            ATRemoveEntryList(&TempSheet->node);
            memFree(TempSheet);
        }
    }

    TempJob->JobReceiveActive = 0;       // we are finished getting the job

    //
    // if there is nothing left of the job, get rid of the job from the queue.
    if (ATIsListEmpty(&TempJob->sheet_list))
    {
        // If some of the job has been printed, do a remove job so that
        // the job will be removed and message sent.  Else just junk the job.
        //
        if (TempJob->JobStartSent == 1)
        {
	    void *param3 = 0; UNUSED_VAR(param3);
	    param3 = SendJobDone(TempJob);
            JMResourceMsg.msgType = MSG_FREERECIPE;
	    ASSERT( param3 == JMResourceMsg.param3 ); // testing...
            SYMsgSend(SJMID, &JMResourceMsg); // param3 is untouched and now sent back.
        }
        else
        {
            ATLISTENTRY* del_node;
            JOB_DATA* del_job;
            del_node = ATRemoveTailList(&job_list);
            if ( NULL != del_node )
            {
                del_job = CONTAINING_RECORD(del_node, JOB_DATA, node);
                ASSERT(del_job == TempJob);
                delete_job(del_job);
            }
        }
    }
    else
    {
        TempJob->JobReceived = 1;
    }

    DBG_PRINTF_NOTICE("ABORT JOB DONE\n");
}


#if defined(HAVE_JOBINFO_ARRAY_SUPPORT)

/* FUNCTION NAME: ManageJobInfo */
/******************************************************************************
 *
 * Description: Update the e_JobInfo array in datastore based on the type of
 *              the message sent to Job Manager. 
 ******************************************************************************/
static void ManageJobInfo(MESSAGE *msg)
{
    ATTR_ENTRY   *curAttr;
    ATTR_ENTRY   *lastAttr;
    bool          clearEntry;
    int           size;
    JOB_DATA     *job;
    JOBINFOTABLE  jobtab;
    PAGE_DATA    *page;
    SHEET_DATA   *sheet;
    tDSVarData    idVar;
    tDSVarData    jobtabVar;
    tDSVarData    slotVar;
    uint8_t         index;
    uint8_t         slot;
    uint32_t        id;
    
    idVar.name = e_CurrentJobID;
    idVar.pVar = &id;
    jobtabVar.name = e_JobInfo;
    jobtabVar.pVar = &jobtab;
    slotVar.name = e_JobInfoCurSlot;
    slotVar.pVar = &slot;
    clearEntry = FALSE;

    switch(msg->msgType)
    {
    case MSG_JOBSTART:
        
        job = (JOB_DATA *)msg->param3;
        DSGetVar(&idVar);
        id = job->JobNum;
        DSSetVar(&idVar);
        DSGetVar(&slotVar);
        DSGetVar(&jobtabVar);
        
        if ((jobtab.entries[slot].id == INVALID_JOB_ID) || (jobtab.entries[slot].id == WAITING_FOR_JOB_ID))
        {
            // current slot is empty or already contains info and is waiting for an ID to be assigned so use it
            DSUnlockVar(e_JobInfoCurSlot);
        }
        else
        {
            // move to the next slot - if it isn't empty, use it anyway because this is a circular buffer where the
            // oldest entry drops off when a new one comes in if it is full

            slot = (slot + 1) % MAX_JOB_INFO_ENTRIES;
            DSSetVar(&slotVar);
        }

        if (jobtab.entries[slot].name != NULL)
        {
            MEM_FREE_AND_NULL(jobtab.entries[slot].name);   // leftover name, free it
        }

        if (job->JobInfo.JobName)
        {
            size = strlen(job->JobInfo.JobName);

            if (size > JOB_NAME_LEN)
            {
                size = JOB_NAME_LEN;
            }

            jobtab.entries[slot].name = (char *)MEM_MALLOC(size + 1);

            if (jobtab.entries[slot].name != NULL)
            {
                // don't use strcpy in case value is larger than JOB_NAME_LEN
                strncpy(jobtab.entries[slot].name, job->JobInfo.JobName, size);
                jobtab.entries[slot].name[size] = 0;   // terminate the string
            }
        }
        
        jobtab.entries[slot].stage = JOB_STAGE_SRC_PROC;
        jobtab.entries[slot].ioSource = 0;
        jobtab.entries[slot].pagesProcessed = 0;
        jobtab.entries[slot].pagesPrinted = 0;
        jobtab.entries[slot].pagesToPrint = 0;
        jobtab.entries[slot].bytesProcessed = 0;
        jobtab.entries[slot].state = JOB_STATE_PROCESSING;
        jobtab.entries[slot].outcome = JOB_OUTCOME_OK;
        jobtab.entries[slot].outbinsUsed = JOB_OUTBIN_1;
        jobtab.entries[slot].physicalOutbinsUsed = JOB_OUTBIN_1;

        if (jobtab.entries[slot].id != WAITING_FOR_JOB_ID)
        {
            // free any leftover attributes

            lastAttr = jobtab.entries[slot].attr;
                
            while (lastAttr != NULL)
            {
                curAttr = lastAttr->nextAttr;

                if (lastAttr->attrText != NULL)
                {
                    MEM_FREE_AND_NULL(lastAttr->attrText);
                }

                MEM_FREE_AND_NULL(lastAttr);
                lastAttr = curAttr;
            }

            jobtab.entries[slot].attr = NULL;
        }
        
        jobtab.entries[slot].id = id;   // set last so WAITING_FOR_JOB_ID can be checked
        DSSetVar(&jobtabVar);
        break;
        
    case MSG_JOBPAGEEND:

        job = GET_JOB_LIST_TAIL_JOB_PTR();

            if (job != NULL)
            {
                sheet = GET_LAST_JOB_LAST_SHEET_PTR();
                
                if (sheet != NULL)
                {
                    page = GetActivePageSide(sheet);

                    if (page != NULL)
                    {
                        index = ManageJobInfoIndex(job->JobNum);

                        if (index != INVALID_JOB_INFO_INDEX)
                        {
                            DSGetVar(&jobtabVar);
                            jobtab.entries[index].stage |= JOB_STAGE_DEST;
                            ++jobtab.entries[index].pagesProcessed;
                            jobtab.entries[index].pagesToPrint += page->COPIES ? page->COPIES : 1;

                            if (jobtab.entries[index].state == JOB_STATE_WAITING)
                            {
                                jobtab.entries[index].state = JOB_STATE_PROCESSING;
                            }

                            DSSetVar(&jobtabVar);
                        }
                    }
                }
            }
        
        break;

    case MSG_JOBEND:

        job = GET_JOB_LIST_TAIL_JOB_PTR();
            
            if (job != NULL)
            {
                index = ManageJobInfoIndex(job->JobNum);
                
                if (index != INVALID_JOB_INFO_INDEX)
                {
                    clearEntry = TRUE;
                    DSGetVar(&jobtabVar);
                    jobtab.entries[index].stage = JOB_STAGE_DEST;    // done processing
                    
                    if (jobtab.entries[index].state == JOB_STATE_ABORTING)
                    {
                        jobtab.entries[index].state = JOB_STATE_ABORTED;
                    }
                    else if (jobtab.entries[index].state == JOB_STATE_CANCELLING)
                    {
                        jobtab.entries[index].state = JOB_STATE_CANCELLED;
                    }
                    else if (jobtab.entries[index].pagesPrinted == jobtab.entries[index].pagesToPrint)
                    {
                        jobtab.entries[index].state = JOB_STATE_PRINTED;
                    }
                    else
                    {
                        clearEntry = FALSE;
                    }

                    DSSetVar(&jobtabVar);   // trip the callback that is looking for changes to e_JobInfo
                }
            }

        break;

    case MSG_PAGEDONE:
    case MSG_DUPLEXPAGESDONE:

        job = GET_JOB_LIST_HEAD_JOB_PTR();
        index = ManageJobInfoIndex(job->JobNum);

        if (index != INVALID_JOB_INFO_INDEX)
        {
            clearEntry = TRUE;
            DSGetVar(&jobtabVar);
            ++jobtab.entries[index].pagesPrinted;
#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
            if (msg->msgType == MSG_DUPLEXPAGESDONE)
            {
                ++jobtab.entries[index].pagesPrinted;   // increment again since duplex done indicates two pages have printed
            }
#endif // defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

            if (jobtab.entries[index].state == JOB_STATE_ABORTING)
            {
                jobtab.entries[index].state = JOB_STATE_ABORTED;
            }
            else if (jobtab.entries[index].state == JOB_STATE_CANCELLING)
            {
                jobtab.entries[index].state = JOB_STATE_CANCELLED;
            }
            else if (jobtab.entries[index].pagesPrinted == jobtab.entries[index].pagesToPrint)
            {
                jobtab.entries[index].state = JOB_STATE_PRINTED;
            }
            else if (jobtab.entries[index].state == JOB_STATE_WAITING)
            {
                jobtab.entries[index].state = JOB_STATE_PROCESSING;
                clearEntry = FALSE;
            }
            else
            {
                clearEntry = FALSE;
            }

            DSSetVar(&jobtabVar);   // trip the callback that is looking for changes to e_JobInfo
        }
        
        break;

    case MSG_OUT_OF_MEMORY:

        job = GET_JOB_LIST_HEAD_JOB_PTR();
        index = ManageJobInfoIndex(job->JobNum);
        
        if (index != INVALID_JOB_INFO_INDEX)
        {
            DSGetVar(&jobtabVar);
            jobtab.entries[index].state = JOB_STATE_WAITING;
            DSSetVar(&jobtabVar);
        }
        
        break;

    case MSG_CANCELJOB: 

        job = GET_JOB_LIST_HEAD_JOB_PTR();
        if ( job != NULL )
        {
            index = ManageJobInfoIndex(job->JobNum);
            
            if (index != INVALID_JOB_INFO_INDEX)
            {
                DSGetVar(&jobtabVar);
                jobtab.entries[index].state = JOB_STATE_CANCELLING;
                DSSetVar(&jobtabVar);
            }
        }
        
        break;

    case MSG_JOBABORT:

        DBG_PRINTF_ERR("ManageJobInfo: ATTENTION! MSG_JOBABORT is now DEAD CODE!\n");

        break;

    default:

        break;
    }
    
    if (clearEntry)
    {
        // note that index was set by the code which set clearEntry to TRUE
        
        DSGetVar(&jobtabVar);

        // free name if one was allocated

        if (jobtab.entries[index].name != NULL)
        {
            MEM_FREE_AND_NULL(jobtab.entries[index].name);
        }

        // free any attributes that may have been allocated

        lastAttr = jobtab.entries[index].attr;

        while (lastAttr != NULL)
        {
            curAttr = lastAttr->nextAttr;

            if (lastAttr->attrText != NULL)
            {
                MEM_FREE_AND_NULL(lastAttr->attrText);
            }
            
            MEM_FREE_AND_NULL(lastAttr);
            lastAttr = curAttr;
        }

        memset(&jobtab.entries[index], 0, sizeof(JOBINFOENTRY));   // clear out the entry since we are done
        DSSetVar(&jobtabVar);
    }
    
    return;
} // end ManageJobInfo()

/* FUNCTION NAME: ManageJobInfoIndex */
/******************************************************************************
 * Description: Looks up the specified ID to determine the index value of its
 * entry in e_JobInfo.  Returns index of the specified ID, or
 * INVALID_JOB_INFO_INDEX if the ID is INVALID_JOB_ID or not found in the table.
 ******************************************************************************/
static uint8_t ManageJobInfoIndex(uint32_t id)
{
    int   i;
    uint8_t index = INVALID_JOB_INFO_INDEX;
    const JOBINFOTABLE *jobtab;

    jobtab = (JOBINFOTABLE *)DSGetROVar(e_JobInfo);

    if (id != INVALID_JOB_ID)   // incoming ID is valid
    {
        for (i = 0; i < MAX_JOB_INFO_ENTRIES; ++i)
        {
            if (jobtab->entries[i].id == id)
            {
                index = i;                   
                break;
            }
        }
    }

    return (index);
} // end ManageJobInfoIndex()

#endif // defined(HAVE_JOBINFO_ARRAY_SUPPORT)

#if defined(HAVE_DBG_PRINTF) || defined (HAVE_CMD)

/* FUNCTION NAME: ToString_RECOVERYSTATE */
const char* ToString_RECOVERYSTATE( RECOVERYSTATE enumValue )
{
    switch (enumValue)
    {
        CASE_ENUM_RETURN_STRING(RECOVERY_IDLE);
        CASE_ENUM_RETURN_STRING(RECOVERY_ACTIVE);
        DEFAULT_CASE_ENUM_RETURN_STRING(RECOVERYSTATE);
    } // end switch (enumValue)
    return "bug";
} // end ToString_RECOVERYSTATE()

#endif // ifdef HAVE_DBG_PRINTF

//------------------------------------------------------------------------------

#if defined (HAVE_DBG_PRINTF) || defined (HAVE_CMD)

/* FUNCTION NAME: PlaneDList_Print */
void PlaneDList_Print(MLISTENTRY* plane_list)
{
    ATLISTENTRY* node;

    if (!gActivateJobMgrPrintLists)
        return;

    node = MListHead(plane_list);

    if (!node)
    {
        cmd_printf("JM:       PlaneDList_Print: EMPTY!\n");
    }
    else while (node)
    {
        PLANED* pPlaneD = CONTAINING_RECORD(node, PLANED, listNode);

        cmd_printf("JM:       PlaneDList_Print: pPlaneD=0x%08X\n", (uint32_t)pPlaneD);
        if (pPlaneD)
        {
            if ( pPlaneD->big_buffer != NULL )
            {
                cmd_printf("JM:       PlaneDList_Print:   Data=0x%08X, DataLength=0x%6X, LastData=%s\n", (uint32_t)pPlaneD->big_buffer->data, (uint32_t)pPlaneD->DataLength, ToStringCond(pPlaneD->LastData));
            }
            else
            {
                cmd_printf("JM:       PlaneDList_Print:   Data=NULL (empty plane), DataLength=0x%6X, LastData=%s\n", pPlaneD->DataLength, ToStringCond(pPlaneD->LastData));
            }
        }

        node = MListNext(plane_list, node);
    }

    cmd_printf("JM:       PlaneDList_Print: exit\n");
} // end PlaneDList_Print()

/* FUNCTION NAME: PageList_Print */
void PageList_Print(SHEET_DATA* pSheetData)
{
    if (!gActivateJobMgrPrintLists)
        return;

    if (!pSheetData)
    {
        DBG_PRINTF_ERR("    PageList_Print: EMPTY!\n");
    }
    else // pSheetData is non-NULL. Its "list" of pages consists of 2 members, FrontPage and BackPage.
    {
        DBG_PRINTF_ERR("    PageList_Print: FrontPage=0x%08X\n", pSheetData->FrontPage);
        if (pSheetData->FrontPage)
        {
            PlaneDList_Print(&pSheetData->FrontPage->planeDataPendingPrintList[0]);
            cmd_printf("      PlaneDList_Print: <skipped>\n");
            // REVISIT TODO: Deal with C, M, and Y planes.
        }
        cmd_printf("JM:     PageList_Print: BackPage=0x%08X\n", (uint32_t)pSheetData->BackPage);
        if (pSheetData->BackPage)
        {
            PlaneDList_Print(&pSheetData->BackPage->planeDataPendingPrintList[0]);
            cmd_printf("JM:       PlaneDList_Print: <skipped>\n");
            // REVISIT TODO: Deal with C, M, and Y planes.
        }
    }

    cmd_printf("JM:     PageList_Print: exit\n");
} // end PageList_Print()

/* FUNCTION NAME: SheetList_Print */
void SheetList_Print(ATLISTENTRY* sheet_list)
{
    ATLISTENTRY* sheet_node;

    if (!gActivateJobMgrPrintLists || sheet_list == NULL)
        return;

    sheet_node = ATListHead(sheet_list);

    if (!sheet_node)
    {
        cmd_printf("JM:   SheetList_Print: EMPTY!\n");
    }
    else while (sheet_node)
    {
        SHEET_DATA* pSheetData = CONTAINING_RECORD(sheet_node, SHEET_DATA, node);

        cmd_printf("JM:   SheetList_Print: pSheetData=0x%08X\n", (uint32_t)pSheetData);
        PageList_Print(pSheetData);

        sheet_node = ATListNext(sheet_list, sheet_node);
    }

    cmd_printf("JM:   SheetList_Print: exit\n");
} // end SheetList_Print()

/* FUNCTION NAME: JobList_Print */
void JobList_Print(ATLISTENTRY* job_list_local)
{
    ATLISTENTRY* node;

    if (!gActivateJobMgrPrintLists)
        return;

    if (!job_list_local)
    {
        return;
    }

    node = ATListHead(job_list_local);
    if (!node)
    {
        cmd_printf("JM: JobList_Print: EMPTY!\n");
    }
    else while (node)
    {
        JOB_DATA* pJobData = CONTAINING_RECORD(node, JOB_DATA, node);

        cmd_printf("JM: JobList_Print: pJobData=0x%08X\n", (uint32_t)pJobData);
        if (pJobData)
        {
            SheetList_Print(&pJobData->sheet_list);
        }

        node = ATListNext(job_list_local, node);
    }

    cmd_printf("JM: JobList_Print: exit\n");
} // end JobList_Print()


//------------------------------------------------------------------------------

/* FUNCTION NAME: JobMgrPrintState */
/**
 * \brief Debug system callback to print internal state of JobMgr.
 */
int JobMgrPrintState(int argc, char* argv[])
{
    cmd_printf("JM STATE: ---------- enter ----------\n");

    cmd_printf("JM STATE: RecoveryState=%s\n", ToString_RECOVERYSTATE(RecoveryState));
  //  cmd_printf("JM STATE: RecoveryType=%s=0x%X=%d\n", DecodeSysMsgEnum(RecoveryType), RecoveryType, RecoveryType);
    cmd_printf("JM STATE: RecoveryPtr=0x%08X\n", (uint32_t)RecoveryPtr);
    cmd_printf("JM STATE: ReceivedPageCount=%d\n", ReceivedPageCount);
    cmd_printf("JM STATE: currentPMQueueOpenSlots=%d\n", currentPMQueueOpenSlots);
    cmd_printf("JM STATE: PmgrMsgSent=%d\n", PmgrMsgSent);
    cmd_printf("JM STATE: MsgReceived=%d\n", MsgReceived);
    cmd_printf("JM STATE: JamRecovery=%d\n", JamRecovery);
    cmd_printf("JM STATE: CurrentJamState=%d\n", CurrentJamState);
    cmd_printf("JM STATE: fJMHaveResources=%d\n", fJMHaveResources);

    bool saveActive = gActivateJobMgrPrintLists;
    gActivateJobMgrPrintLists = TRUE;
    JobList_Print(&job_list);
    gActivateJobMgrPrintLists = saveActive;

    cmd_printf("JM STATE: ---------- exit ----------\n");
    return 0;
} // end JobMgrPrintState()

//------------------------------------------------------------------------------
/**
 * \brief return string representation of duplex mode.
 */
const char* ToString_DUPLEX_MODE( DUPLEX_MODE duplexModeEnum )
{
    switch ( duplexModeEnum )
    {
    CASE_ENUM_RETURN_STRING( DMDUP_SIMPLEX );
    CASE_ENUM_RETURN_STRING( DMDUP_VERTICAL );
    CASE_ENUM_RETURN_STRING( DMDUP_HORIZONTAL );
    DEFAULT_CASE_ENUM_RETURN_STRING( DUPLEX_MODE );
    } // end switch ( duplexModeEnum )
    return "bug";
} // end ToString_DUPLEX_MODE()

#endif // ifdef HAVE_DBG_PRINTF


bool print_low_memory_strategy(mlimiter_t *lim, uint32_t failed_alloc_size)
{
    static int Time = 0;
    MESSAGE Message;


    // If the limiter isn't active, don't allow allocation attempts!
    ASSERT(lim->active);

    // Try to increase the limiter max through a relevel
    mlimiter_relevel_memory( lim );
    if ( lim->max - lim->current >= failed_alloc_size )
    {
        // With relevel we now have enough memory - return immediately
        return true;
    }

    if (Time < 5)
    {
        posix_sleep_ms( 10 );
        ++Time;
    }
    else
    {
        // After a while, tell the job manager we are out of memory so it can
        // try and get us some.
        //
        Message.msgType =  MSG_OUT_OF_MEMORY;
        SYMsgSend (JOBMGRID, &Message);     // tell the job manager we are tired of waiting and have no memory
        Time = 0;
    }

    return true; // keep trying, loop forever
}
#if 0
static void print_jobmgr_global_page_end_observer_constructor( void )
{
    print_jobmgr_global_page_end_observer_data.job_data = NULL;
    print_jobmgr_global_page_end_observer_data.subject_observer_oid_key = PRINT_JOBMGR_GLOBAL_PAGE_END_ID;
    MInitList(&print_jobmgr_global_page_end_observer_data.observers);
}

static void print_jobmgr_global_page_end_observer_notify_event( JOB_DATA * job_data, PAGE_DATA * page_data )
{
    print_jobmgr_global_page_end_observer_data_t * my_status;

    print_jobmgr_global_page_end_observer_data.job_data = job_data;
    print_jobmgr_global_page_end_observer_data.page_data = page_data;

    my_status = &print_jobmgr_global_page_end_observer_data;

    NOTIFY_OBSERVERS( my_status, observers );
}

JOB_DATA * print_jobmgr_global_page_end_observer_get_job_data( Subject_t *s )
{
    ASSERT( VALIDATE_OBJECT_KEY( s,
                                 print_jobmgr_global_page_end_observer_data_t, 
                                 subject_observer_oid_key,
                                 PRINT_JOBMGR_GLOBAL_PAGE_END_ID ) );

    // message causes notify causes get so no lock needed. 
    return print_jobmgr_global_page_end_observer_data.job_data;
}

PAGE_DATA * print_jobmgr_global_page_end_observer_get_page_data( Subject_t *s )
{
    ASSERT( VALIDATE_OBJECT_KEY( s,
                                 print_jobmgr_global_page_end_observer_data_t, 
                                 subject_observer_oid_key,
                                 PRINT_JOBMGR_GLOBAL_PAGE_END_ID ) );

    // message causes notify causes get so no lock needed. 
    return print_jobmgr_global_page_end_observer_data.page_data;
}

void print_jobmgr_global_page_end_observer_detach(Observer_t *o)
{
    DETACH_OBSERVER( system_pji->pageend_observers, o );
}

void print_jobmgr_global_page_end_observer_attach( Observer_t *o )
{
    ATTACH_OBSERVER( system_pji->pageend_observers, o );
}

//-------------------------
// job end observer
static void print_jobmgr_global_job_end_observer_constructor( void )
{
    print_jobmgr_global_job_end_observer_data.job_data = NULL;
    print_jobmgr_global_job_end_observer_data.subject_observer_oid_key = PRINT_JOBMGR_GLOBAL_JOB_END_ID;
    MInitList(&print_jobmgr_global_job_end_observer_data.observers);
}

static void print_jobmgr_global_job_end_observer_notify_event( JOB_DATA * job_data )
{
    print_jobmgr_global_job_end_observer_data_t * my_status;

    print_jobmgr_global_job_end_observer_data.job_data = job_data;

    my_status = &print_jobmgr_global_job_end_observer_data;

    NOTIFY_OBSERVERS( my_status, observers );
}

JOB_DATA * print_jobmgr_global_job_end_observer_get_job_data( Subject_t *s )
{
    ASSERT( VALIDATE_OBJECT_KEY( s,
                                 print_jobmgr_global_job_end_observer_data_t, 
                                 subject_observer_oid_key,
                                 PRINT_JOBMGR_GLOBAL_JOB_END_ID ) );

    // message causes notify causes get so no lock needed. 
    return print_jobmgr_global_job_end_observer_data.job_data;
}

void print_jobmgr_global_job_end_observer_detach(Observer_t *o)
{
    DETACH_OBSERVER( print_jobmgr_global_job_end_observer_data.observers, o );
}

void print_jobmgr_global_job_end_observer_attach( Observer_t *o )
{
    ATTACH_OBSERVER( print_jobmgr_global_job_end_observer_data.observers, o );
}
#endif
//------------------------------------------------------------------------------

/**
 * <pre>
 * Job layout
 *
 *  JobList --> ============= --> more JOB_DATA 
 *              | JOB_DATA  |     
 *              |           |
 *              | sheetlist | --> ============== --> more SHEET_DATA
 *              =============     | SHEET_DATA |
 *                                |            |
 *                                | FrontPage, | --> ============= --> more PAGE_DATA
 *                                | BackPage   | --> | PAGE_DATA |
 *                                ==============     |           |
 *                                                   | plane     | --> ========== --> more PLANED
 *                                                   =============     | PLANED |
 *                                                                     ==========
 *                                
 * </pre>
 */

