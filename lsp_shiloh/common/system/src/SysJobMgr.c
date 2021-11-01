/***********************************************************
* (c) Copyright 2008-2009 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/
/**
 * \file SysJobMgr.c
 *
 * \brief System Job Manager. Handles the scheduling, contention, and
 *        canceling of jobs.
 *****************************************************************************/
 
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "agRouter.h"
#include "memAPI.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "logger.h"
#include "SJMgr.h"
#include "ResourceMap.h"
#include "lassert.h"
#include "sys_init_api.h"
#ifdef HAVE_POWER_MGR
#include "pwr_mgr_api.h"
#endif // HAVE_POWER_MGR
#include "error_types.h"
#include "debug.h"
#include "sjm_observers_api.h"
#include "response_subject.h"
#include "posix_ostools.h"
#include "SysUI_Interface.h"
#include "SysCardReaderMsg.h"
#ifdef HAVE_IMAGELOG
#include "imagelog.h"
#else
#include "stdlib.h"
#include "stdio.h"
#endif

// uint32_t sys_dbg_flags=0xf3;   // debug control flags, 

#define DBG_PRFX "SJM: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 12 ) 

#define SJMSTACK_SIZE POSIX_MIN_STACK_SIZE          ///< Size of the stack
#define NUM_MESSAGES 200             ///< Size of the message queue.

static ALIGN8 UINT8 SJMStack[SJMSTACK_SIZE];      ///< Stack for the system job manager
static pthread_t sysjobmgr_thd_id;
static mqd_t sysjobmgr_queue;

static ATLISTENTRY current_job_list;       ///< Linked list of current jobs.
static int total_num_jobs; ///< Total number of running jobs 

extern uint32_t g_nCurrentUIScreenID;
extern uint32_t UI_SCREEN_UP_ID;
extern uint32_t UI_SCREEN_SP_ID;
extern uint32_t gPCUDrumStatus;
extern bool gLaminationJobStatus;
extern bool gInitCompleted;
extern uint32_t gSystemAuthType;

#define USE_UNIQUE_JOB_ID ///< Defines to enable unique job id

#define CANCEL_JOB_WAIT_ACK	1
#if CANCEL_JOB_WAIT_ACK
#define ISPRINTJOB(pjtype) ((pjtype==ejob_PrintIO)||(pjtype==ejob_InternalPagePrint)||(pjtype==ejob_Copy)||(pjtype==ejob_FaxPrint)||(pjtype==ejob_zjs_host_print)||(pjtype==ejob_PrintFile))
#endif

#ifdef HAVE_POWER_MGR
static uint32_t sys_job_mgr_pwr_mgr_uid;
#endif

typedef struct SJM_JobStatus_s
{
    Response_Subject_T_BaseClassMembers;
    sjm_jobstate_enum_t job_status;
    JOB_TYPES job;
    uint32_t JobId;
} SJM_JobStatus_t;

static SJM_JobStatus_t SJM_JobStatus;


#ifdef HAVE_DBG_PRINTF
const char* to_string_sjm_jobstate_enum(sjm_jobstate_enum_t enum_i)
{
    switch(enum_i) {
	 CASE_ENUM_RETURN_STRING(esjm_JobReceived);
	 CASE_ENUM_RETURN_STRING(esjm_JobStart);
	 CASE_ENUM_RETURN_STRING(esjm_JobDone);
	 CASE_ENUM_RETURN_STRING(esjm_JobCancel_at_device);
	 CASE_ENUM_RETURN_STRING(esjm_JobCancel_by_user);
	 CASE_ENUM_RETURN_STRING(esjm_JobCancelDone);
	 CASE_ENUM_RETURN_STRING(esjm_JobDropped);
	 DEFAULT_CASE_ENUM_RETURN_STRING(sjm_jobstate_enum_t);
    }
    return "bug";
} 
#endif

#ifdef HAVE_IMAGELOG
extern uint32_t g_nImageLogMemFull; ///0: Not Full 1: Memory Full (Do not allow job in SJM)
extern uint32_t g_nImageLogSDCardError;
extern uint32_t g_nImageLogSendingFailed; ///0: No problem 1: Sending failed
void SetImageLogStartInfo(JOB_RESOURCES *CurResource)
{
	char jobTypeChar = '0';
	int subJobType = 0;
	switch(CurResource->JobType)
	{
		case ejob_PrintIO:
		case ejob_InternalPagePrint:
		case ejob_InternalPageHttp:
		case ejob_zjs_host_print:
			jobTypeChar = 'P';
			break;
		case ejob_Copy:
		case ejob_CopyToHost:
			jobTypeChar = 'C';
			break;
		case ejob_ScanToHost:
		case ejob_ScanToEmail:
		case ejob_ScanToMemCard:
			jobTypeChar = 'S';
			break;
		case ejob_ScanFax: //Fax Tx Send
			jobTypeChar = 'F';
			subJobType = 1;
			break;
		case ejob_FaxPrint:	//Fax Rx Receive
			jobTypeChar = 'F';
			subJobType = 2;
			break;
		//case ejob_Fax:
		//	break;
		default:
			break;
	//ejob_PJL,
	//ejob_ACL,
	//ejob_CMD,
	//ejob_ScanCal,
	//ejob_DemoScanApp,
	//ejob_FaxParse,
	//ejob_FaxPrint,
	//ejob_ThumbnailPhoto,
	//ejob_HttpScan,
	//ejob_ParseMisc,
	//ejob_PrintFile,
	}

	if(jobTypeChar != '0'){
		set_metafile_start_info(jobTypeChar, subJobType, CurResource->JobId, CurResource->UserId);
	}
}
#endif

static void SJM_JobStatus_constructor(void)
{
    SJM_JobStatus.job = ejob_NULL;
    SJM_JobStatus.job_status = esjm_JobDone;
    SJM_JobStatus.subject_observer_oid_key = SJM_JobStatus_ID;
    MInitList(&SJM_JobStatus.observers);

}

static void SJM_Notify_JobStatus( sjm_jobstate_enum_t JobStatus, JOB_TYPES job, uint32_t job_id )
{
    SJM_JobStatus_t * my_status;
    SJM_JobStatus.job = job;
    SJM_JobStatus.job_status = JobStatus;
    SJM_JobStatus.JobId = job_id;
    my_status = &SJM_JobStatus;

    NOTIFY_OBSERVERS( my_status, observers );
}

sjm_jobstate_enum_t SJM_Get_JobStatus( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, SJM_JobStatus_t, 
                                subject_observer_oid_key, SJM_JobStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return SJM_JobStatus.job_status ;
}
JOB_TYPES SJM_Get_JobType(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, SJM_JobStatus_t, 
                                subject_observer_oid_key, SJM_JobStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return SJM_JobStatus.job;
}
uint32_t SJM_Get_JobId(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(s, SJM_JobStatus_t,
                               subject_observer_oid_key, SJM_JobStatus_ID));
    return SJM_JobStatus.JobId;
}
void SJM_AttachObserver_JobStatus(Observer_t *o)
{
    ATTACH_OBSERVER( SJM_JobStatus.observers, o );
}

void SJM_DetachObserver_JobStatus(Observer_t *o)
{
    DETACH_OBSERVER( SJM_JobStatus.observers, o );
}

static void print_job_early_cancel( JOB_TYPES job_type )
{
    if ( job_type == ejob_PrintIO ||
	 job_type == ejob_InternalPagePrint ||
	 job_type == ejob_InternalPageHttp ||
	 job_type == ejob_Copy ||
	 job_type == ejob_FaxPrint ||
	 job_type == ejob_PrintFile)
    {
	extern void jobmgr_free_page_token_on_cancel( void );
	jobmgr_free_page_token_on_cancel();
    }
}

#ifdef USE_UNIQUE_JOB_ID
#define UNIQUE_JOB_ID_PATH "/app/nvram/jobid.dat"
#define JOB_ID_BUF_MAX 6
#define UNIQUE_JOB_ID_LIM_MIN 1
#define UNIQUE_JOB_ID_LIM_MAX 99999
static uint32_t next_unique_job_id = 0;
static pthread_mutex_t gUniqueJobIdMutex;
/**
 * \brief  Read saved job id
 * Check range (1~99999) and set to global job id
 * \date 04-Jun-2015
 * Author: Lalit
 */
int init_unique_job_id( void )
{
    char buff[JOB_ID_BUF_MAX];
    int retVal = 0;
    int lastJobId = 0;
    int px_status;
    px_status = posix_mutex_init( &gUniqueJobIdMutex );
    XASSERT( px_status==0, px_status );

    int fd = open(UNIQUE_JOB_ID_PATH, O_RDWR | O_CREAT | O_CLOEXEC);
	if( fd == -1){
		DBG_PRINTF_ERR("[%s] Open Failed UNIQUE_JOB_ID_PATH\n", __FUNCTION__);
		return -1;
	}

	if((retVal = read (fd, &buff, JOB_ID_BUF_MAX)) > 0){
		lastJobId = atoi(buff);
		DBG_PRINTF_ERR("[%s] buff:%s UniqueJobId:%d\n", __FUNCTION__, buff, lastJobId);
	}
	else{
		DBG_PRINTF_ERR("[%s] Unable to read from UNIQUE_JOB_ID_PATH ret:%d\n", __FUNCTION__, retVal);
	}

	if((lastJobId >= UNIQUE_JOB_ID_LIM_MIN) && (lastJobId <= UNIQUE_JOB_ID_LIM_MAX) ){
		next_unique_job_id = lastJobId;
	}
	else{
		next_unique_job_id = 0;
	}
	close(fd);
    return next_unique_job_id;
}

/**
 * \brief  Find the next available job id (Range 1~99999)
 * Increment the job id wraps around at MAX LIMIT=99999
 * \date 04-Jun-2015
 * Author: Lalit
 */
UINT32 create_unique_job_id( void )
{
	pthread_mutex_lock( &gUniqueJobIdMutex );
    ++next_unique_job_id;
    if(next_unique_job_id > UNIQUE_JOB_ID_LIM_MAX ){
    	next_unique_job_id = UNIQUE_JOB_ID_LIM_MIN;
	}

    char writeBuff[JOB_ID_BUF_MAX];
	int fd = open(UNIQUE_JOB_ID_PATH, O_RDWR | O_CLOEXEC);
	if( fd == -1){
		DBG_PRINTF_ERR("[%s] Open Failed UNIQUE_JOB_ID_PATH\n", __FUNCTION__);
	}
	else{
		snprintf(writeBuff, JOB_ID_BUF_MAX, "%05d", next_unique_job_id);
		//DBG_PRINTF_ERR("[%s] Write buff:%s UniqueJobId:%05d\n", __FUNCTION__, writeBuff, next_unique_job_id);
		write(fd, writeBuff, sizeof(writeBuff));
		close(fd);
	}
	pthread_mutex_unlock( &gUniqueJobIdMutex );
    return next_unique_job_id;
}
#endif

/**
 * \brief  Find the next avaible job id
 *
 * Increment the global job id wraps around at 2^32 :)
 *
 *
 */

UINT32 create_job_id( void )
{
    static uint32_t next_job_id = 0;    
    return ++next_job_id;
}

/**
 * \brief  return the number of currently running jobs
 *
 * Originally created so we could know exactly when the system has returned to
 * an jobless state for continuous copy testing.
 *
 * \date 06-Nov-2008
 *
 */

int SJMGetTotalNumJobs( void )
{
    int tmp;
    //LSPTODO: TX_INTERRUPT_SAVE_AREA;

    /* switch from a semaphore to interrupt protection
     * because we need to call SJMGetTotalNumJobs() from interrupt/timer context
     */
    //LSPTODO: TX_DISABLE;        

    tmp = total_num_jobs;

    //LSPTODO: TX_RESTORE;

    return tmp;
}

/**
 * \brief  return currently running job type
 *
 * Added to get job type of top most job if any job running.
 * If no job running than return ejob_NULL.
 *
 * \date 07-July-2014
 *
 */
JOB_TYPES SJMGetCurJobType(void)
{
	JOB_TYPES curJobType = ejob_NULL;
	ATLISTENTRY* node;
	JOB_RESOURCES *TempResource = NULL;

	node = ATListHead(&current_job_list);
	if(node != NULL)
	{
		TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
		DBG_PRINTF_ERR("[%d] RO=%d TempResource=0x%08X, JobType=%d=%s\n",
		TempResource->JobId, TempResource->ResourceObtained, TempResource, TempResource->JobType, to_string_job_types(TempResource->JobType));
		curJobType = TempResource->JobType;
		if(TempResource->ResourceObtained){
			curJobType += 1000;
		}
	}
#if 0 //DEBUG Code
    while(node != NULL)
    {
        TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
        DBG_PRINTF_ERR(" *** [%d] RO=%d TempResource=0x%08X, JobType=%d=%s\n",
       	TempResource->JobId, TempResource->ResourceObtained, TempResource, TempResource->JobType, to_string_job_types(TempResource->JobType));
        node = ATListNext(&current_job_list, node);
    }
#endif
	return curJobType;
}

/**
 * \brief  Debug current available jobs and their status
 *
 * Added to get job type of top most job if any job running.
 * If no job running than do nothing.
 *
 * \date 08-January-2015
 *
 */
void SJMGetAllJobInfo(void)
{
	ATLISTENTRY* node;
	JOB_RESOURCES *TempResource = NULL;
#if 1 //DEBUG Code
	DBG_PRINTF_ERR("---------------------- SJM_DEBUG_START -----------------------\n");

	node = ATListHead(&current_job_list);
    while(node != NULL)
    {
        TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
        if(TempResource != NULL)
        {
        	DBG_PRINTF_ERR("************** JOB ID:%d Type:%d=%s **************\n",
        			TempResource->JobId, TempResource->JobType, to_string_job_types(TempResource->JobType));
			DBG_PRINTF_ERR("*** [%d] RO:%d TempResource:0x%08X JobState:%d RecoveryAction:%d\n",
				TempResource->JobId, TempResource->ResourceObtained, TempResource, TempResource->JobState, TempResource->RecoveryAction);

			UINT32 i = 0;
			for(i = 0; TempResource->ReqResources[i] != 0; i++)
			{
				DBG_PRINTF_ERR("------>> [%d] ReqResource[%d] R:0x%X=%d\n",TempResource->JobId, i, TempResource->ReqResources[i], TempResource->ReqResources[i]);
			}

			if((TempResource->AvailResourceList != NULL))
			{
				UINT32 k = 0;
				while(TempResource->AvailResourceList[k] != NULL)
				{
					DBG_PRINTF_ERR("======>> [%d] AvailResource[%d]:%d=%s R:0x%X\n",
						TempResource->JobId, k,	TempResource->AvailResourceList[k]->ModID, ToString_MODULE_ID(TempResource->AvailResourceList[k]->ModID),
						TempResource->AvailResourceList[k]->ResourceType);
					k++;
				}
			}
        }
        node = ATListNext(&current_job_list, node);
    }
    DBG_PRINTF_ERR("---------------------- SJM_DEBUG_END -----------------------\n");
#endif
}

static void increment_total_num_jobs( void )
{
    //LSPTODO: TX_INTERRUPT_SAVE_AREA;

    //LSPTODO: TX_DISABLE;

    total_num_jobs += 1;

    //LSPTODO: TX_RESTORE;
}

static void decrement_total_num_jobs( void )
{
    //LSPTODO: TX_INTERRUPT_SAVE_AREA;

    //LSPTODO: TX_DISABLE;

    total_num_jobs -= 1;

    //LSPTODO: TX_RESTORE;
}

//------------------------------------------------------------------------------
/**
 * \brief Only allow a fixed number of Cancels to be active in the system at
 *        the same time.
 *
 * If this throttle is not in place, the SJM message Queue can overflow,
 * causing the printer to hang. The overflow was happening when the user
 * pressed the Job Cancel button repeatedly. Yet we must allow at least two
 * cancels to be active, for the case in which we have a Cancel PAGE due to
 * a paper jam or other "status" condition, and then the user presses the
 * Cancel JOB button.
 */
static int          gActiveCancelCount = 0;
static const int    gActiveCancelLimit = 1;	//Print JobMgr Can't handle 2 Cancel Job same time.

static int DecrementActiveCancelCount( void )
{
    if ( gActiveCancelCount > 0 )
    {
        gActiveCancelCount--;
    }

    DBG_PRINTF_NOTICE("DecrementActiveCancelCount=%d\n", gActiveCancelCount);
    return gActiveCancelCount;
}

static int IncrementActiveCancelCount( void )
{
    gActiveCancelCount++;

    DBG_PRINTF_NOTICE("IncrementActiveCancelCount=%d\n", gActiveCancelCount );
    return gActiveCancelCount;
}

//------------------------------------------------------------------------------

extern JOB_RECIPE SystemJobs[];

static void *SysJbMgrThread(void * unused);         ///< Proto type

#ifdef OBSOLETE
static uint32_t sys_job_mgr_get_active_jobs_total_mem( void );
static uint32_t sys_job_mgr_get_print_jobs_count( void );
#endif

static uint32_t sys_job_mgr_get_jobs_count( void );

//Job states
//
typedef enum
{
    JOBSTATE_PENDING,    //waiting for resources
    JOBSTATE_ACTIVE,     //job active
    JOBSTATE_RECOVERY,   //doing job cancel, page cancel, page restart, or page retry
    JOBSTATE_FINISH,     //waiting for resources to be released
    JOBSTATE_DONE,       //finished
} SYSJOBSTATE;


/**
 * Function name: SysJbMgrInit
 *
 * \brief Initialize the system job manager
 *
 * Initialize data structures, create the queue, register the mainbox and
 *              start the task for the system job manager
 **/
void SysJbMgrInit(void)
{
    ATInitList(&current_job_list);

    SJM_JobStatus_constructor();
#ifdef USE_UNIQUE_JOB_ID
    init_unique_job_id();
#endif
    total_num_jobs = 0;
    posix_create_MESSAGE_queue( &sysjobmgr_queue, "/sysjobmgr_queue", NUM_MESSAGES );
    router_register_queue(SJMID, sysjobmgr_queue);  // Register a message queue.
    posix_create_thread(&sysjobmgr_thd_id, SysJbMgrThread, 0, "sysjobmgr", SJMStack, SJMSTACK_SIZE, POSIX_THR_PRI_NORMAL);
}

/**
 * Function name: SysJbMgrShutdown
 *
 * \brief Shutdown the system job manager
 * 
 * Description: Cancel current job and wait for all jobs to end
 * 
 * Author: 
 *
 * Notes: 
 *
 **/
void SysJbMgrShutdown(void)
{
    MESSAGE TempMsg;
    TempMsg.msgType = MSG_CANCELJOB;
    TempMsg.param1= SYS_REQUSER;
    SYMsgSend(SJMID, &TempMsg);

    while(sys_job_mgr_get_jobs_count())
        sleep(1);
}
 
 
/* FUNCTION NAME: SysVerifyJob */
 
/** 
 * \brief  Verify the given job pointer is still in the list of active jobs.  Local call.
 * 
 * \retval 0 if it exists, 1 if not
 * 
 * \param pJob The job to find.
 * 
 * \date 10/24/2005
 **/
BOOL SysVerifyJob(JOB_RESOURCES* pJob)
{
    JOB_RESOURCES *TempResource = NULL;
    ATLISTENTRY* node;
    BOOL valid = FALSE;

    if(pJob == NULL)
    {
        return valid;
    }

    node = ATListHead(&current_job_list);
    while (node != NULL)     // loop till we have seen everything.
    {
        TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
        if(TempResource == pJob)
        {
            valid = TRUE;
            break;
        }
        node = ATListNext(&current_job_list, node);
    }
    DBG_PRINTF_NOTICE("SysVerifyJob(0x%08X): %s\n", pJob, ToStringCond(valid));
    return valid;
}


/* FUNCTION NAME: SysGetJob */
 
/** 
 * \brief  Search for the job that contains the given resource
 * 
 * \param resID The resource to find
 * 
 * \return A pointer to the job that was found.
 * 
 * \date 10/24/2005
 * 
 **/
JOB_RESOURCES *SysGetJob(RESOURCES resID)
{
    JOB_RESOURCES *TempResource = NULL;
    ATLISTENTRY* node;
    UINT32 i;

    node = ATListHead(&current_job_list);
    while (node != NULL)     // loop till we have seen everything.
    {
        TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
        DBG_PRINTF_NOTICE("SysGetJob(resID=%d) TempResource=0x%08X\n", resID, TempResource );

        for(i = 0; TempResource->ReqResources[i] != 0; i++)
        {
            //
            // See if this is the one we want.  We must match the resourceid
            // and we must be part of a job.  Otherwise we skip this.
            //
            if((TempResource->ReqResources[i] == resID) &&
               (TempResource->ResourceObtained == 1))
            {
	        DBG_PRINTF_NOTICE("SysGetJob(resID=%d): return TempResource=0x%08X\n", resID, TempResource );
                return TempResource;
            }
        }
        node = ATListNext(&current_job_list, node);
    }
    DBG_PRINTF_NOTICE("SysGetJob(resID=%d): return NULL\n", resID );
    return NULL;
}

/**\brief Search for the recipe for a job type defined in a recipe array
 * 
 * The recipe list should be ended with a ejob_NULL job type. To avoid 
 * infinite loop in search, the array should not contain more than 32 recipes.  
 * 
 * \param recipe_list   The list of recipes to search
 * \param type          Type of job 
 * 
 * \return   NULL   No recipe is found
 *           Others Pointer to the recipe
 */
static JOB_RECIPE *searchJobRecipe(JOB_RECIPE recipe_list[], JOB_TYPES type)
{
    int i;
    JOB_RECIPE *cur_recipe = NULL;
    
    //Only allow up to 32 slots
    for(i=0;i<32;i++)
    {
        if(recipe_list[i].myJob == ejob_NULL)
            break;
        else if(recipe_list[i].myJob == type)
        {
            cur_recipe= &(recipe_list[i]);
            break;
        }
    }
    
    return cur_recipe;
}

/**
 * Function name: SysJbMgrThread
 *
 * \brief Main thread of the system job manager
 *
 * This thread handles the upper level of the system job manager
 **/
static void *SysJbMgrThread(void *unused)
{
    MESSAGE             nextMsg;
    uint16_t            Temp;
    JOB_RESOURCES       *CancelJob;
    JOB_RESOURCES       *TempResource;
    RESOURCES           CancelRes;
    BOOL                valid;
    sjm_jobstate_enum_t sjm_jobstate;

    SysWaitForInit ();

#ifdef HAVE_POWER_MGR
    sys_job_mgr_pwr_mgr_uid = pwr_mgr_get_uid();
#endif

    // Wait for a message
    while (1)
    {
        REL_XASSERT( sizeof(MESSAGE) == mq_receive(sysjobmgr_queue, (char*)&nextMsg, sizeof(MESSAGE), 0), 
		     thread_errno);
#if 0 //DEBUG: Debugging for Message Queue
//		int mq_retcode;
//		struct mq_attr attr;
//		mq_retcode = (int)mq_getattr( sysjobmgr_queue, &attr );	
//	
// 		 DBG_PRINTF_ERR(" SysJbMgrThread:  ATTR: MaxMsg=%ld, CurMSGS = %ld \n", attr.mq_maxmsg,  attr.mq_curmsgs);			 
#endif
        DBG_PRINTF_NOTICE("got message '%s'=0x%X=%d; param3=0x%08X\n",
                         DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType, nextMsg.msgType, nextMsg.param3 );
        switch(nextMsg.msgType)
        {
        	case MSG_AUTH_RESULT:
            {
                int userAuthResult = nextMsg.param1;
                char *userId = (char*)nextMsg.param3;
                DBG_PRINTF_CRIT("MSG_AUTH_RESULT AuthResult=%d, userID=%s strlen=%d\n", userAuthResult, userId, strlen(userId));
                if(userAuthResult){
                    ///TODO: Save user ID
                }
                else{
                    ///TODO: Send Auth Fail result
                }
                break;
            }
            case    MSG_STARTRECIPE:
            {
                JOB_RESOURCES *TempResource;
                ATLISTENTRY* node;
                JOB_RECIPE *CurrentRecipe;
                STARTRECIPE *Parameters;
                uint32_t i;

                //
                // Get the correct recipe for this job.
                //
                Parameters = (STARTRECIPE *) nextMsg.param3;
                CurrentRecipe = searchJobRecipe(SystemJobs, (JOB_TYPES)Parameters->Job); //CurrentRecipe = &SystemJobs[(UINT32)Parameters->Job];
                DBG_PRINTF_NOTICE("Start Job-Get Resources %s for recipe: %x\n", to_string_job_types(Parameters->Job), CurrentRecipe );

                if(CurrentRecipe == NULL)
                {
                    DBG_PRINTF_NOTICE("Undefined job type=%d\n", Parameters->Job );
                    ASSERT(0);
                }

                //
                // See if this job can be queued.
                // One of the parameters is from the requester of a job.  He 
                // tells us if we need to wait for the resources, or return
                // an error if we cannot get them.  Inside the recipe is a
                // notification of how many of a given job can be queued at
                // a given time.
                //
                if ((Parameters->Wait == e_WaitForever) &&
                    (CurrentRecipe->Queued != e_Unlimited))   // only check if we need to wait
                {
                    //
                    // We have a job that can be queued.  Check the job list
                    // to see how many times we have the job in the queue and
                    // if it is too many times, chuck the job.
                    //
                    UINT32 ChuckJob=0;
                    node = ATListHead(&current_job_list);
                    //
                    // See how many times this job type is currently in the queue.
                    //
                    i = 0;      // temp job counter.
                    while (node != NULL)
                    {
                        TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
                        if (TempResource->JobType == Parameters->Job)
                            i++;
                        node = ATListNext(&current_job_list, node);
                    }
                    // i is the number of times this job is queued.
                    //
                    // See what we should do with this job.
                    //
                    switch (CurrentRecipe->Queued)
                    {
                        case e_One:
                            if (i != 0)
                            {
                                ChuckJob = 1;
                            }
                            break;
                        case e_Two:
                            if(i > 1)
                                ChuckJob = 1;
                            break;
                        default:
                            XASSERT(0, CurrentRecipe->Queued);
                            break;
                    }
                    //
                    // If this job does not meet the parameters, chuck it.
                    //
                    if(ChuckJob)
                    {
                    	DBG_PRINTF_CRIT("GetRid of job %d\n",i);
                        MEM_FREE_AND_NULL(Parameters);
                        break;          // job is gone, get out of here.
                        //
                        // We do not need to notify the task that sent the 
                        // request because he does not retain any state that
                        // he sent the request.
                        //
                    }
                }
                // tell anyone that is interested that a job is in.
                // there is not a job id till running so make this zero.
                SJM_Notify_JobStatus(esjm_JobReceived, (JOB_TYPES)Parameters->Job, 0); 

#ifdef HAVE_POWER_MGR
                // If the job is requesting power management, restore power in 
                // the system. Need to wait for active state before the job
                // can progress. Consider delaying the call to the wait function
                // until later to reduce the delay! 
                DBG_PRINTF_NOTICE("Got job with WAKE parameter! Waiting...\n");
                pwr_mgr_go_active_wait(sys_job_mgr_pwr_mgr_uid);
                DBG_PRINTF_NOTICE("WAKE completed!\n");
#endif //HAVE_POWER_MGR

                // Get the number of resources needed for this job.
                Temp = CurrentRecipe->NumResources;
                
                // in the Parameters additionalresource is the resource requesting this job.  
                // If this is non-null then we need to include this in the list.
                // This allows parsers to initiate jobs and be in the chain.
                // and allows the keyboard to initiate jobs and not be in
                // the chain.
                //
                if(Parameters->AdditionalResource != e_NoResource)  // if we have an included resource, 
                    Temp++;         // the number of resources is one greater.
                //
                // Call the creator for the JOB_RESOURCE structure.  This
                // structure is used to keep information on jobs and is stored
                // in the job list.
                //
                TempResource = CreateJobResource (Temp+1);  // initialize the jobresource
                TempResource->JobState = JOBSTATE_PENDING;  // waiting for running.
                TempResource->RecoveryAction = 0;
                TempResource->RecoveryResource = NULL;
                TempResource->JobType = Parameters->Job;    // save job type
                if(gSystemAuthType){	//Check for AuthType if not basic then copy userID
                    //memcpy( TempResource->UserId, AuthInfo.authID, AUTH_BASIC_LENGTH );
                    strcpy( TempResource->UserId, AuthInfo.authID);
                }
                else{
                    memset( TempResource->UserId, 0x00, AUTH_BASIC_LENGTH );
                    memset( TempResource->UserId, '\0', 1);
                }
                DBG_PRINTF_CRIT("AuthType:%d UserID=%s strlen=%d Addr=0x%X\n", gSystemAuthType, TempResource->UserId, strlen(TempResource->UserId), TempResource->UserId);

                DBG_PRINTF_NOTICE("job type is: %s\n", to_string_job_types(Parameters->Job));

                // Get the other info into the structure.
                TempResource->RequestMod = Parameters->SendingModule; // module if
                TempResource->WaitType = Parameters->Wait;  // wait parameter.  
                                                            // If no wait returns an error
                //
                // Update the cancel action, tells if this job can be canceled.
                //
                TempResource->CancelAction = CurrentRecipe->CancelAction;
                TempResource->PercentCPU = CurrentRecipe->PercentCPU;
                //
                // Now make a list of the resources required for this job.
                // In the job structure.
                //
                Temp = 0;
                if (Parameters->AdditionalResource != e_NoResource)
                {
                    // First add in the additional resource.
                    TempResource->ReqResources[0] = Parameters->AdditionalResource;     
                                           // This is the resource that is requesting the job
                    Temp++;     // number on the list
                }
                //
                // Now put the rest of the required resources into the required
                // resource list.  This comes from the job recipe.
                //
                for (i = Temp; i < CurrentRecipe->NumResources+Temp; i++)
                {
                    ASSERT( (int32_t)((int32_t)i - (int32_t)Temp ) >= 0 );
                    ASSERT( ( i - Temp ) <
                            ( sizeof(CurrentRecipe->RequiredResources) - sizeof(CurrentRecipe->RequiredResources[0]) ) );
                    TempResource->ReqResources[i] = CurrentRecipe->RequiredResources[i-Temp];
                }
                TempResource->ReqResources[CurrentRecipe->NumResources+Temp] = e_NoResource;  // Terminate the list

                // Put the job on the list of jobs.
                //
                DBG_PRINTF_NOTICE("MSG_STARTRECIPE: TempResource=0x%08X\n", TempResource);
                ATInsertTailList(&current_job_list, &TempResource->node);
                increment_total_num_jobs();
                MEM_FREE_AND_NULL(Parameters);     // done with the job info, return to system.
                //
                // Scan the list now.
                //
                nextMsg.msgType = MSG_SCANJOBS; // forces a scan of the list.
                SYMsgSend (SJMID, &nextMsg);     // start it.
            }
                break;
                //
                // Something in the system has changed.  Scan all the jobs
                // on the job list looking for jobs that need resources.  When
                // you have found those jobs, try and get the resources and
                // run them.
                //
            case MSG_SCANJOBS:
            {
                ATLISTENTRY* node;

                node = ATListHead(&current_job_list);

                DBG_PRINTF_INFO("Scan system Jobs %p\n",node);
                while (node != NULL)     // loop till we have seen everything.
                {
                    //
                    // Get a pointer to the job information.
                    //
                    TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
                    DBG_PRINTF_NOTICE("MSG_SCANJOBS: TempResource=0x%08X, id = %d, type = %s\n", 
                                      TempResource, TempResource->JobId, to_string_job_types(TempResource->JobType));

#ifdef HAVE_IMAGELOG
                    if(g_nImageLogMemFull || g_nImageLogSendingFailed || g_nImageLogSDCardError){
                        DBG_PRINTF_CRIT("ImageLog: MemFull:%d LogSendingFailed:%d SDCardError:%d\n",
                        		g_nImageLogMemFull, g_nImageLogSendingFailed, g_nImageLogSDCardError);
                        if( TempResource && (TempResource->ResourceObtained == 0 ) ) {
                            sleep(2);
                            SYMsgSend(SJMID,&nextMsg);	// send the message to me(SJM)
                            break;
                        }
                    }
#endif
                    //Do not allow Print Job now and wait till condition satisfied
                    //case 1: If system initialization or initial machine setup is not finished: Do not allow any Print job
                    //case 2: PCU DRUM END:  Don't allow Print job  but allow Scan job
                    //case 3: Lamination Job running:  Don't allow Print job but allow Scan job
extern int gInitialMachineSetupProceeding;
                    if((gPCUDrumStatus == DRUM_END_LOCK)
                    		|| (gInitCompleted == false) || (gInitialMachineSetupProceeding == true)
							|| (gLaminationJobStatus == true)){
                        if( ISPRINTJOB(TempResource->JobType) && (TempResource->ResourceObtained == 0 ) ) {
                        	if(!gInitCompleted){
                        		DBG_PRINTF_ERR("Waiting... InitCompetion=%d\n", gInitCompleted);
                        	}
							else if ( gInitialMachineSetupProceeding ) {
                        		DBG_PRINTF_ERR("Waiting... InitialMachineSetup=%d\n", gInitialMachineSetupProceeding);
							}
                        	else{
                        		if(gPCUDrumStatus == DRUM_END_LOCK)
                        			DBG_PRINTF_NOTICE("DRUM_END_LOCK %d: Waiting for Reset or Drum change.\n", gPCUDrumStatus);
                        		if(gLaminationJobStatus == true)
                        			DBG_PRINTF_NOTICE("Waiting for Lamination:%d to finish.\n", gLaminationJobStatus);

                                if(sys_job_mgr_get_jobs_count() > 1){
                                    DBG_PRINTF_ERR("DRUM_END_LOCK:%d Lamination:%d Job_Count=%d.\n",
                                    		gPCUDrumStatus, gLaminationJobStatus, sys_job_mgr_get_jobs_count());
                                    //Remove current Print job from list and insert at Tail list.
                                    ATRemoveEntryList(&TempResource->node);
                                    ATInsertTailList(&current_job_list, &TempResource->node);
                                }
                            }
                            sleep(1);
                            SYMsgSend(SJMID,&nextMsg);	// send the message to me(SJM)
                            break;
                        }
                    }

#if CANCEL_JOB_WAIT_ACK
                    if( ISPRINTJOB(TempResource->JobType) && (TempResource->ResourceObtained == 0 ) ) {
                        if( ( gActiveCancelCount > 0)){
                            DBG_PRINTF_NOTICE("CANCELING IN PREGRESS %d: TRY AGAIN LATER.\n", gActiveCancelCount);
                            usleep(50*1000);
                            SYMsgSend(SJMID,&nextMsg);	// send the message to me(SJM)
                            break;
                        }
                    }
#endif

                    //
                    // See if this has the resources it needs.
                    //
                    if (TempResource && TempResource->ResourceObtained == 0)
                    {
                        // we do not have the resource, try and get it.  
                        error_type_t gotResource;

                        gotResource = OK;

                        //If JobType is ScanToHost and Current screen is UP or SP then discard this job(NAK).
                        if(TempResource->JobType == ejob_ScanToHost){
                            if((g_nCurrentUIScreenID == UI_SCREEN_UP_ID) ||(g_nCurrentUIScreenID == UI_SCREEN_SP_ID)){
                                gotResource = FAIL;
                            }
                        }

                        // if we have enough memory to run, then attempt
                        // to get other resources and to allocate memory
                        if ( OK == gotResource )
                        {
                            gotResource = GetSystemResource (TempResource);
                        }
                        else
                        {
//                            DBG_PRINTF_NOTICE("Not enough memory to run this job needed %d have %d\n",
//                                       total_job_memory_needed, total_job_memory_available);
                        }

                        if ( OK == gotResource )
                        {
                            UINT32 i = 0;
                            //
                            // we got the resource.  Get things running.
                            // Send a message to the process saying we have the data.
                            //
                            DBG_PRINTF_INFO("Got the resources\n");
#if 1
                            //TODO: If FAX received job then don't wait.
                            //Don't execute job if current screen is 8:UP or 9:SP
                            if(( TempResource->JobType != ejob_NULL ) || ( TempResource->JobType != ejob_Fax) ){
                                    while((g_nCurrentUIScreenID == UI_SCREEN_UP_ID) ||(g_nCurrentUIScreenID == UI_SCREEN_SP_ID)){
                                            //DBG_PRINTF_ERR("wait...\n\r");
                                            sleep(1);	//wait for User to finish Setting. 
                                    }
                            }
#endif
                            /* Lalit 20170531 moved code before MSG_RESOURCES,
							 * scan app using JobId in the MSG_RESOURCES.
							 */
                            /* davep 26-Apr-2009 ; adding a "job id", like a
                             * UNIX process id. Need a way for observers to
                             * match the observer callbacks to the appropriate
                             * job.
                             */
#ifdef USE_UNIQUE_JOB_ID
                            TempResource->JobId = create_unique_job_id();
#else
                            TempResource->JobId = create_job_id();
#endif
                            //
                            // Send a message to all parts of the job telling 
                            // them that a job is about to start.
                            //
                            while (TempResource->AvailResourceList[i] != NULL)
                            {
                                CURRENT_RESOURCE  *OurResource;
                                UINT32 ErrorCount;

                                //
                                // Tell all the resources they are in a job and their source and
                                // destination addresses.
                                //
                                ErrorCount = 0;
                                while( ( OurResource = (CURRENT_RESOURCE *)MEM_MALLOC( sizeof(CURRENT_RESOURCE) ) ) == NULL)
                                {
                                    sleep(1);    // wait for something to come free.
                                    ASSERT(ErrorCount++ < 10); // assert after a bit.
                                    REL_ASSERT(ErrorCount++ < 10*60); // time to die after 10min infinite loop.
                                }

                                nextMsg.msgType = MSG_RESOURCES;
                                //
                                // Fill in the source and destination pointers in the resource
                                // message.  This tells the module who is sending him data and
                                // where he needs to send data.
                                if(i > 0)
                                    OurResource->Source = TempResource->AvailResourceList[i-1]->ModID;
                                else
                                    OurResource->Source = INVALID_MODULE_ID; // source of data gets no data
                                //
                                // Now fill in the destination module id.
                                //
                                if(TempResource->AvailResourceList[i+1] != NULL)
                                    OurResource->Destination = TempResource->AvailResourceList[i+1]->ModID;
                                else
                                    OurResource->Destination = INVALID_MODULE_ID;
                                // In the resource include the pointer to the
                                // job we are running.
                                OurResource->JobResources = TempResource;   // the job we are in.
                                // Also include a pointer to our resource
                                // structure.  This id's the given resource.
                                OurResource->Me = TempResource->AvailResourceList[i];
                                OurResource->FreedResourceNotList = FALSE;  // not freed.
                                nextMsg.param3 = OurResource;  // also send the address of the resource.    
                                SYMsgSend (TempResource->AvailResourceList[i]->ModID, &nextMsg);
                                i++;
                            }

                            //mark job as active
                            TempResource->JobState = JOBSTATE_ACTIVE;

                            //
                            // Now we have told all the resources they are in a job we
                            // need to tell them to start.
                            //
                            i = 0;

                            while (TempResource->AvailResourceList[i] != NULL)
                            {
                                nextMsg.msgType = MSG_ACKRECIPE;
                                SYMsgSend (TempResource->AvailResourceList[i]->ModID, &nextMsg);
                                i++;
                            }

                            TempResource->ResourceObtained = 1;     // this job is in process.

                            // tell anyone that is interested that a job is running.
                            SJM_Notify_JobStatus(esjm_JobStart, TempResource->JobType,
                                                 TempResource->JobId); 
#ifdef HAVE_IMAGELOG
                            //Save Image Log Data
                            SetImageLogStartInfo(TempResource);
#endif

                            DBG_PRINTF_NOTICE("Job Started Type %s JobID %d\n",
                                       to_string_job_types(TempResource->JobType), TempResource->JobId);
                            // no break here, want to check all the jobs.
                        }
                        else
                        {
                            DBG_PRINTF_NOTICE("Did not get the resources\n" );
                            //
                            // We are not able to get the resources.  Check now
                            // to see if we want to queue up and try later or
                            // just fail.
                            //
                            if(TempResource->WaitType == e_DoNotWait)
                            {
                                //
                                // We were unable to get the resources, nak 
                                // the start recipe message. to fail the 
                                // job.
                                //
                                nextMsg.msgType = MSG_NAKRECIPE;
                                SYMsgSend(TempResource->RequestMod, &nextMsg);
                                //
                                // Pull the job off of the linked list.
                                // and get rid of the memory for this job
                                // this job is gone.
                                //
                                node = ATListNext(&current_job_list, node);
                                ATRemoveEntryList(&TempResource->node);
                                DBG_PRINTF_CRIT("no resources & no wait; delete: TempResource=0x%08X\n", TempResource );
                                decrement_total_num_jobs();
                                //LALIT: Notify anyone who may be interested that the job is dropped
                                SJM_Notify_JobStatus(esjm_JobDropped, TempResource->JobType, TempResource->JobId);
                                MEM_FREE_AND_NULL(TempResource);   // free the job resources.
                                continue;               // do the next loop.
                            } 
                        }
                    }
                    // go to the next job on the list.
                    node = ATListNext(&current_job_list, node);
                }
                break;
            }
           
            // This message is received whenever a module completes its job.  
            // The resource is freed, then see if all the 
            // resources are free.  If they are then free the sub-resources
            // and then free the memory and get rid of the job.  This job
            // is then complete.
            case MSG_FREERECIPE:
            
        case MSG_FREEIO:
                {
                    CURRENT_RESOURCE *FreeResource;
                    UINT32 CurRes=0,k;

                    // Free the resource that sent this.
                    //
                    // param3 should point to a ALLOC_RESOURCE structure.
                    //
                    DBG_PRINTF_NOTICE("Free Job Received %s, param = %#x\n", ((nextMsg.msgType == MSG_FREERECIPE) ? "MSG_FREERECIPE" : "MSG_FREEIO"), nextMsg.param3);

                    FreeResource = (CURRENT_RESOURCE *)nextMsg.param3;  // the resource to free.

                    // The resources for some job types were being freed twice - causing the Printer to Hang
                    // Make sure all pointers are valid before doing checks on the resources
                    if(FreeResource == NULL 
                       || FreeResource->JobResources == NULL 
                       || FreeResource->JobResources->AvailResourceList == NULL 
                       || FreeResource->JobResources->AvailResourceList[CurRes] == NULL)
                    {
                        DBG_PRINTF_NOTICE("Resource 0x%08X is already free\n", FreeResource );
                        // Scan the list now.  Send a message to myself to scan the 
                        // job list and see if we can start any more jobs.
                        //
                        nextMsg.msgType = MSG_SCANJOBS;
                        SYMsgSend (SJMID, &nextMsg);     // start it.
                        break;
                    }

                    // The FreeResource->JobResources points to the job that we need to process
                    // This job is stored on the CurrentJob linked list.
                    // Find the resource in the resource list..
                    //
                    while ((FreeResource->JobResources->AvailResourceList[CurRes] != NULL)
                           && (FreeResource->JobResources->AvailResourceList[CurRes] != FreeResource->Me))
                    {
                        DBG_PRINTF_NOTICE("Resource %d %x\n", CurRes, FreeResource->JobResources->AvailResourceList[CurRes] );
                        CurRes++;
                    }
                    //
                    // See if we found the resource.
                    //
                    if ((FreeResource->JobResources->AvailResourceList[CurRes] != NULL) &&
                        (FreeResource->JobResources->AvailResourceList[CurRes] == FreeResource->Me))
                    {
                        // we have the resource.  Free it.
                        if(FreeResource->FreedResourceNotList == FALSE)
                            ReleaseResource (FreeResource);
                        FreeResource->FreedResourceNotList = TRUE;
                        //
                        // Now get rid of it from the list.  Since this is an
                        // array we delete the current entry by moving all 
                        // following entries up by one.
                        //
                        if(nextMsg.msgType != MSG_FREEIO)
                        {
                            k = CurRes;
                            DBG_PRINTF_NOTICE("Freed Resource %d\n",k);
                            do
                            {
                                FreeResource->JobResources->AvailResourceList[k] = FreeResource->JobResources->AvailResourceList[k+1];
                            } while(FreeResource->JobResources->AvailResourceList[k++] != NULL);
                        }
                    }
                    //
                    // if the list is now empty, we need to free the memory allocation.
                    //
                    if ((FreeResource->JobResources->AvailResourceList[0] == NULL) &&
                        (nextMsg.msgType != MSG_FREEIO))
                    {
                        ATLISTENTRY* node;

                        DBG_PRINTF_NOTICE("Freeing resources for job type: %s\n", to_string_job_types(FreeResource->JobResources->JobType));

                        FreeResource->JobResources->JobState = JOBSTATE_FINISH;
                        //
                        // Decrement the mips used.
                        //
                        FreeCPUResource(FreeResource->JobResources->PercentCPU);
                        
                        //
                        // Everything is cleaned up, get rid of the job stuff.
                        //
                        node = ATListHead(&current_job_list);
                        while (node != NULL)
                        {
                            TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
                            // Find the job of interest.
                            //
                            if (TempResource == FreeResource->JobResources)
                            {
#ifdef HAVE_POWER_MGR
                                DBG_PRINTF_NOTICE("MSG_FREERECIPE GO TO PWR IDLE STATE ******************* %d\n", nextMsg.msgType);
                                pwr_mgr_go_idle(sys_job_mgr_pwr_mgr_uid);
#endif // HAVE_POWER_MGR
                                // This is the one we want to remove.
                                //
                                DBG_PRINTF_NOTICE("MSG_FREERECIPE: FreeResource=0x%08X, TempResource=0x%08X\n", FreeResource, TempResource);
                                ATRemoveEntryList(node);
                                decrement_total_num_jobs();
                                
                                // notify anyone who may be interested that the job is done
                                SJM_Notify_JobStatus(esjm_JobDone, FreeResource->JobResources->JobType,
                                                     FreeResource->JobResources->JobId);
                                DBG_PRINTF_NOTICE("Job Complete Type %s JobID %d\n",
                                           to_string_job_types(FreeResource->JobResources->JobType), FreeResource->JobResources->JobId);
                                MEM_FREE_AND_NULL(FreeResource->JobResources->AvailResourceList);
                                MEM_FREE_AND_NULL(FreeResource->JobResources);
                                TempResource = NULL;
                                break;      // done get out of here
                            }
                            node = ATListNext(&current_job_list, node);
                        }
                    }
                    // return the memory.
                    // don't want to free this if a freeio comes.  Since we received
                    // a freeio we will be getting a freerecipe later.  The freerecipe
                    // is when we want to free this resource allocation.
                    //
                    if(nextMsg.msgType != MSG_FREEIO)
                    {
		        DBG_PRINTF_NOTICE("MEM_FREE_AND_NULL(FreeResource=0x%08X)\n", FreeResource);
                        MEM_FREE_AND_NULL(FreeResource);
                    }
                    // Scan the list now.  Send a message to myself to scan the 
                    // job list and see if we can start any more jobs.
                    // Now done with nextMsg so we can use it.
                    //
                    nextMsg.msgType = MSG_SCANJOBS;
                    SYMsgSend (SJMID, &nextMsg);     // start it.
                    break;
                }

            case MSG_CANCELJOB: 
                CancelJob = NULL;

                //Need to cancel the entire job. Send the request on to all resources in the job 
                //starting at the first. We need to wait for each resource to ack before sending to the next
                //param1 contains the specific action. We can get SYS_REQUEST and SYS_ACK here
                //param3 contains the resource id of the resource module needing to cancel
                DBG_PRINTF_ERR("got MSG_CANCELJOB param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                 nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
                                 nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
                                 nextMsg.param3 );

                if((nextMsg.param1 == SYS_REQUEST) ||
                   (nextMsg.param1 == SYS_REQUSER))
                {
                    //To stop sending cancellation msg for more then 1 job.
                    if ( gActiveCancelCount >= gActiveCancelLimit )
                    {
                        // We have reached maximum limit of ActiveCancel.
                        // So wait for a bit and then send the message back to myself(SJM) for handling.
                        DBG_PRINTF_ERR("Now %d, Max %d Active cancel allowed: SJM will try later.\n",gActiveCancelCount, gActiveCancelLimit);
                        sleep(1);
                        SYMsgSend(SJMID,&nextMsg);  // send the message to me.
                        break;
                    }

                    sjm_jobstate = esjm_JobCancel_by_user;

                    IncrementActiveCancelCount();

                    if(nextMsg.param1 == SYS_REQUEST)
                    {
                        CancelRes    = (RESOURCES)(UINT32)nextMsg.param3;
                        CancelJob    = SysGetJob(CancelRes);
                        sjm_jobstate = esjm_JobCancel_at_device;
                    } 
                    else
                    {
                        ATLISTENTRY* node;
                        ATLISTENTRY* node_to_cancel = NULL;

                        node = ATListHead(&current_job_list);
                        //
                        // If there are no jobs, ignore the cancel message.
                        //
                       
                        if(node == NULL)
                        {
                            DecrementActiveCancelCount();
                            break;
                        }

                        // Now for the case of requser
                        //
                        // Find the first job on that list that can be cancelled.
                        // For a job to be canceled we need the job to be 
                        // running and the type of job allow cancelling.
                        //
                        while (node != NULL)
                        {
                            TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
                            if((TempResource->ResourceObtained == 1) &&
                               (TempResource->CancelAction != e_NoCancel))
                            {
                                node_to_cancel = node;
                                break;
                            }
                            node = ATListNext(&current_job_list, node);
                        }
                        //
                        // If cancellink is null then there is no job to be cancelled.
                        //
                        if (node_to_cancel == NULL)
                        {
                            DecrementActiveCancelCount();
                            break;
                        }
                        CancelJob = CONTAINING_RECORD(node_to_cancel, JOB_RESOURCES, node);
                    }
                    if(CancelJob)   // anything to cancel?
                    {
                        //if already canceling.
                        if(CancelJob->JobState == JOBSTATE_RECOVERY)
                        {
                            //
                            // We are already doing another cancel.  Wait for a bit
                            // and then send the message back to myself for
                            // handling.  The other cancel could be either a button
                            // press or a status cancel.  If a status cancel occurs then
                            // this will cancel the job after that cancel has
                            // completed its operation.   If it is a button 
                            // press (we can't tell)
                            // then when that cancel completes this will run
                            // and it will see the job is gone and do
                            // nothing.  
                            //
                            if ( gActiveCancelCount <= gActiveCancelLimit )
                            {
                                DBG_PRINTF_NOTICE("Cancel already active: try again later.\n" );
                                sleep(1);  // random value, 30msec seems long enough
                                SYMsgSend(SJMID,&nextMsg);  // send the message to me.
                            }
                            else
                            {
                                DBG_PRINTF_NOTICE("Cancels already active: Ignore this one.\n" );
                            }
                            DecrementActiveCancelCount();
                            break;
                        }

                        //send cancel to first one on the list
                        CancelJob->RecoveryResource = CancelJob->AvailResourceList[0];

                        //if job is empty, consider it as canceled
                        DBG_PRINTF_INFO("RecoveryResource %x\n",CancelJob->RecoveryResource);
                        if(CancelJob->RecoveryResource == NULL)
                        {
                            DecrementActiveCancelCount();
                            break;
                        }

                        CancelJob->JobState = JOBSTATE_RECOVERY;  // state to cancel
                        // Send a notice that this job is being cancelled.
                        // 
                        
                        // What type of cancel was previously set as the MSG_CANCELJOB state was entered.
                        SJM_Notify_JobStatus(sjm_jobstate, CancelJob->JobType,
                                             CancelJob->JobId);
                        DBG_PRINTF_CRIT("Job Cancel begin Type %s JobID %d\n",
                                   to_string_job_types(CancelJob->JobType), CancelJob->JobId);
			print_job_early_cancel( CancelJob->JobType );
                        //start at beginning of resource list and start canceling. The resources will
                        //be sending free recipe messages while we do this so our list will be shrinking.
                        CancelJob->RecoveryAction = MSG_CANCELJOB;
                        nextMsg.param1 = SYS_START;
                        nextMsg.param2 = SJMID;
                        nextMsg.param3 = CancelJob;
                        DBG_PRINTF_NOTICE("cancel job to first ModID=%d=%s\n",
                                         CancelJob->RecoveryResource->ModID,
                                         ToString_MODULE_ID( CancelJob->RecoveryResource->ModID ) );
                        SYMsgSend(CancelJob->RecoveryResource->ModID,&nextMsg);

                    }
                    else
                    {
                        DecrementActiveCancelCount();
                    }
                }
                else if(nextMsg.param1 == SYS_ACK)
                {
                    CancelJob = (JOB_RESOURCES*)nextMsg.param3;

                    // Make sure job is still in the list. If it is not, we are done.
                    valid = SysVerifyJob(CancelJob);
                    if(!valid)
                    {
                        DecrementActiveCancelCount();
#if CANCEL_JOB_WAIT_ACK	
                            DBG_PRINTF_CRIT("MSG_CANCELJOB: %d SysVerifyJob=FALSE, p1=%s, p2=%s, p3=0x%08X\n", gActiveCancelCount,
                                 ToString_DOACTION((DOACTION)nextMsg.param1), ToString_MODULE_ID((MODULE_ID)nextMsg.param2), nextMsg.param3 );
#endif
                        DBG_PRINTF_NOTICE("SysVerifyJob(nextMsg.param3=0x%08X): FALSE\n", nextMsg.param3 );
                        CancelJob = SysGetJob(e_Printer);
                        valid = SysVerifyJob(CancelJob);
                        DBG_PRINTF_NOTICE("SysVerifyJob(SysGetJob(e_Printer)=0x%08X): %s\n", CancelJob, ToStringCond(valid) );
                        break;
                    }

                    //got cancel ack. As we cancel the resources, they will be
                    //  sending free recipe msgs and removing themselves from 
                    // the list so the guy we got the ack from may or may not still
                    //be on the list. If he is on the list we want to move to the 
                    // next guy. If he's not still on
                    //the list then the next to cancel is the first guy (index 0)
                    //
                    int ii=0, idx=0;

                    for(ii=0; CancelJob->AvailResourceList[ii] != NULL; ii++)
                    {
                        if(CancelJob->RecoveryResource == CancelJob->AvailResourceList[ii])
                        {
                            idx = ii +1;
                            break;
                        }
                    }
                    CancelJob->RecoveryResource = CancelJob->AvailResourceList[idx];

                    //if job is empty, consider it as canceled
                    if(CancelJob->RecoveryResource == NULL) 
                    {
                        DecrementActiveCancelCount();

                        SJM_Notify_JobStatus(esjm_JobCancelDone, CancelJob->JobType,
                                             CancelJob->JobId); 
                        DBG_PRINTF_CRIT("Job Cancel Done ACC %d Type %s JobID %d\n", gActiveCancelCount,
                                   to_string_job_types(CancelJob->JobType), CancelJob->JobId);
                        DBG_PRINTF_NOTICE("no recovery resource\n" );
                        CancelJob->JobState = JOBSTATE_ACTIVE;  // finished with the cancel.
                        break;
                    }

                   DBG_PRINTF_NOTICE("cancel job to next ModID=%d=%s\n",
                                     CancelJob->RecoveryResource->ModID,
                                     ToString_MODULE_ID( CancelJob->RecoveryResource->ModID ) );
                    nextMsg.param1 = SYS_START;
                    nextMsg.param2 = SJMID;
                    nextMsg.param3 = CancelJob;
                    SYMsgSend(CancelJob->RecoveryResource->ModID,&nextMsg);
                }
                else
                {
                    ASSERT(0);
                }
                break;

            case MSG_CANCELPAGE:
            case MSG_RESTARTPAGE:
            case MSG_RETRYPAGE: 
                //
                // These are normally generated by a status event.
                //Need to cancel or restart a page in the job. Send the request to the resource
                //who signalled the status (param3). Wait for the ack.
                //param1 contains the specific action. We can get SYS_REQUEST and SYS_ACK here
                //param3 contains the resource id of the resource module needing to cancel
                DBG_PRINTF_NOTICE("got %s param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                 DecodeSysMsgEnum(nextMsg.msgType),
                                 nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
                                 nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
                                 nextMsg.param3 );

                if(nextMsg.param1 == SYS_REQUEST)
                {
                    UINT32 i;

                    CancelRes = (RESOURCES)(UINT32)nextMsg.param3;
                    CancelJob = SysGetJob(CancelRes);

                    if(CancelJob)
                    {
		        DBG_PRINTF_INFO("found job for resource: %d\n", CancelRes);
                        // see if another cancel is going on, and if so dump this cancel.
                        // The only other cancel would be a cancel button request
                        // which would mean this is not needed anyway.
                        //
                        if(CancelJob->JobState == JOBSTATE_RECOVERY)
                        {
                            break;
                        }

                        IncrementActiveCancelCount();

                        //search the job for the matching resource and send it the message
                        for(i = 0; CancelJob->AvailResourceList[i] != 0; i++)
                        {
                            if(CancelJob->AvailResourceList[i]->ResourceType == CancelRes)
                            {
                                CancelJob->JobState = JOBSTATE_RECOVERY;
                                CancelJob->RecoveryResource = CancelJob->AvailResourceList[i];

                                DBG_PRINTF_INFO("send recovery to modid: %d\n", 
						CancelJob->RecoveryResource->ModID);

                                CancelJob->RecoveryAction = nextMsg.msgType;
                                nextMsg.param1 = SYS_START;
                                nextMsg.param2 = SJMID;
                                nextMsg.param3 = CancelJob;
                                SYMsgSend(CancelJob->RecoveryResource->ModID,&nextMsg);
                            }
                        }
                    }
                }
                else if(nextMsg.param1 == SYS_ACK)
                {
                    // Handle the acknowledge to the cancel.

                    DecrementActiveCancelCount();

                    //for retry we need to send a resume message
                    CancelJob = (JOB_RESOURCES*)nextMsg.param3;

                    // Make sure job is still in the list. If it is not, we are done.
                    valid = SysVerifyJob(CancelJob);
                    if(!valid)
                        break;
                    CancelJob->JobState = JOBSTATE_ACTIVE;  // state back to active.
                    if(CancelJob->RecoveryAction == MSG_RESTARTPAGE || 
                       CancelJob->RecoveryAction == MSG_RETRYPAGE)
                    {
                        //
                        // Send a resume to the module.
                        // cancel is done and we can continue.
                        //
                        nextMsg.param1 = SYS_RESUME;
                        nextMsg.param2 = SJMID;
                        nextMsg.param3 = CancelJob;
                        SYMsgSend(CancelJob->RecoveryResource->ModID,&nextMsg);
                    }
                }
                else
                {
                    ASSERT(0);
                }
                break;
            default:
                break;
        } // end switch ( nextMsg.msgType )
    } // end while (1)
    return 0;
} // end SysJbMgrThread()

#if defined(DEBUG)
//
//  showSJM() - generate a quick overview of sjm's joblist
//
void showSJM(const char *hint)
{
    JOB_RESOURCES *TempResource = NULL;
    ATLISTENTRY *node;
    UINT32 i;

    node = ATListHead(&current_job_list);
    while (node != NULL)     // loop till we have seen everything.
    {
        TempResource = CONTAINING_RECORD(node, JOB_RESOURCES, node);
        for (i = 0; TempResource->ReqResources[i] != 0; i++)
        {
            RESOURCES jobResource = TempResource->ReqResources[i];
            dbg_printf("SJM: %s job resource (%x)[%d] -> %s %d\n",
                       hint, TempResource, i,
                       (jobResource == e_Printer     ? "e_Printer"
                        : jobResource == e_ZJParser    ? "e_ZJParser"
                        : jobResource == e_PJLParser   ? "e_PJLParser"
                        : jobResource == e_HBParser    ? "e_HBParser"
                        : jobResource == e_NoResource  ? "e_NoResource"
                        : "other"),
                       jobResource);
        }
        node = ATListNext(&current_job_list, node);
    }
}

void showActivePrintJob(void)
{ 
  JOB_RESOURCES *activePrintJob = SysGetJob(e_Printer);
  BOOL valid = SysVerifyJob(activePrintJob);

  dbg_printf("SJM: gActiveCancelCount=%d\n", gActiveCancelCount );

  if (!activePrintJob) {
    dbg_printf("SJM: no active print jobs!\n");
    showSJM("   idle");
    return;
  }
  if (!valid) {
    dbg_printf("SJM: job x%x not valid!\n", activePrintJob);
    return;
  }

  dbg_printf("SJM: current print job x%x\n", activePrintJob);
  showSJM(" active");

}
#endif // if defined(DEBUG)


#ifdef OBSOLETE
/**
 *
 * \brief Returns the total number of bytes required to run all queued jobs
 *
 * \return Returns the total number of bytes required to run all queued jobs
 *
 **/
static uint32_t sys_job_mgr_get_active_jobs_total_mem( void )
{
    uint32_t total_job_memory_needed;
    LINK_MEMBER *job_count_link;
    JOB_RESOURCES * my_job_resource;
	bool counted_print_job;
	bool count_memory;

	/* Use this flag to ensure we only count print jobs once,
	 * as multiple print jobs can use the same memory */
	counted_print_job = false;


    /* see how many jobs there are currently running
     * and determine memory needs */
    job_count_link = agGetHead(&CurrentJobs);

    total_job_memory_needed = 0;
    while (job_count_link != NULL)
    {
		ASSERT( NULL != job_count_link->Data );
		my_job_resource = (JOB_RESOURCES *)(job_count_link->Data);

		/* Use this flag to decide whether or not we count this job's memory */
		count_memory = true;

        /* count all currently _active_ job memory */
		if ( 1 == my_job_resource->ResourceObtained )
		{
			/* If we have already counted a print job memory, then we won't
			 * count it again because print jobs share memory */
            if ( ejob_PrintIO == my_job_resource->JobType )
			{
				if ( true == counted_print_job )
				{
					count_memory = false;
				}
			}

			if ( true == count_memory )
			{
				total_job_memory_needed += my_job_resource->MemoryRequired;
			}
		}

		job_count_link = job_count_link->NextLink;
    }

    return total_job_memory_needed;
}
#endif //OBSOLETE

/**
 *
 * \brief Returns the total number of jobs (queued or running)
 *
 * \return Returns the total number of jobs (queued or running)
 *
 **/
static uint32_t sys_job_mgr_get_jobs_count( void )
{
    return ATNumListNodes(&current_job_list);
}

#ifdef OBSOLETE
/**
 *
 * \brief Returns the total number of PRINT jobs (queued or running)
 *
 * \return Returns the total number of PRINT jobs (queued or running)
 *
 **/
static uint32_t sys_job_mgr_get_print_jobs_count( void )
{
    uint32_t print_job_count;
    LINK_MEMBER *job_count_link;
    JOB_RESOURCES * my_job_resources;

    // see how many print jobs there are currently running
    job_count_link = agGetHead(&CurrentJobs);
    print_job_count = 0;

    while (job_count_link != NULL)
    {
        my_job_resources = job_count_link->Data;
        if ( NULL != my_job_resources )
        {
            if ( ejob_PrintIO == my_job_resources->JobType )
            {
                // count number of active jobs
                print_job_count++;
            }
        }

        job_count_link = job_count_link->NextLink;
    }

    return print_job_count;
}
#endif // OBSOLETE
