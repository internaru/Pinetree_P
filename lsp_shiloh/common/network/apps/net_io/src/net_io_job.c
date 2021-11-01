/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef HAVE_PRINT_SUPPORT
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "error_types.h"
#include "lassert.h"
#include "logger.h"
#include "memAPI.h"
#include "agConnectMgr.h"
#include "io_device.h"
#include "PrintJobInfo.h"
#include "net_logger.h"
#include "net_io_api.h"

// private headers
#include "net_io.h"


//=======================================================================================
// Debug configuration
//=======================================================================================

#define DBG_PRFX "NETIOJOB: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_NET_IO 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)


//=======================================================================================
// Data types
//=======================================================================================

typedef enum
{
    NET_IO_JOB_STATE_PENDING,
    NET_IO_JOB_STATE_STARTED,
    NET_IO_JOB_STATE_COMPLETED,
    NET_IO_JOB_STATE_CANCELED,
    NET_IO_JOB_STATE_ABORTED,

    // add new states above this line
    NET_IO_JOB_NUM_STATES
} net_io_job_state_t;


static const char *g_net_io_job_state_strs[NET_IO_JOB_NUM_STATES] = 
{
#ifdef HAVE_DBG_PRINTF
    "NET_IO_JOB_STATE_PENDING", 
    "NET_IO_JOB_STATE_STARTED", 
    "NET_IO_JOB_STATE_COMPLETED",
    "NET_IO_JOB_STATE_CANCELED",
    "NET_IO_JOB_STATE_ABORTED"  
#endif // HAVE_DBG_PRINTF
};


typedef struct net_io_job_s
{
    uint32_t    signature;
    net_io_job_state_t state;
    net_io_job_state_reason_t reason;
    uint32_t    koctets_processed;
    uint32_t    octets_processed; // stores koctet remainder
    uint32_t    pages_completed;
    uint32_t    sheets_completed;
    net_io_job_event_func_t event_func;
    void        *user_ctxt;
    net_io_session_t *session;
} net_io_job_t;

// sanity check should be unique but not a catastrophy if it isn't 
#define net_io_jobstart_observer_id     0x5D83A981
#define net_io_pageend_observer_id      0x5D83A982
#define net_io_jobend_observer_id       0x5D83A983
#define net_io_jobcancel_observer_id    0x5D83A984

#define NET_IO_JOB_CONTEXT_SIGNATURE    0x91827364


//=======================================================================================
// Local function prototypes
//=======================================================================================

// Observer Notification Callbacks
static void netio_jobstart_notify(Observer_t *observer, Subject_t *s);
static void netio_pageend_notify(Observer_t *observer, Subject_t *s);
static void netio_jobend_notify(Observer_t *observer, Subject_t *s);
static void netio_jobcancel_notify(Observer_t *observer, Subject_t *s);

/* PrintJobInfo Callback
 * 
 * This routine is registered with the print subsystem, which then calls back at the start
 * of each new job at the time the PrintJobInfo object is copied from the channel/pipe.
 * The callback provides an opportunity for netio to assign the local job context to the
 * "foreign context" member of the pji object, so that it can be recovered in the various
 * job observer callbacks for that job (jobstart, pageend, jobend, etc.).
 */
void netio_pji_copy_notify(PrintJobInfo_t *pji_copy, void *user_ctxt);

// local helper Routines
static net_io_job_t *netio_job_construct(net_io_session_t *session);
static void netio_job_attach(net_io_session_t *session, net_io_job_t *job);
static void netio_job_destruct(net_io_job_t *job);
static void netio_job_attach_observers(net_io_channel_t *net_channel);
static void netio_job_detach_observers(net_io_channel_t *net_channel);


//=======================================================================================
// Parameterized macros
//=======================================================================================

#define NET_IO_JOB_LOCK_CONTEXT() \
        { \
            int px_status; \
            px_status = pthread_mutex_lock( net_io_get_mtx() ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_IO_JOB_UNLOCK_CONTEXT() \
        { \
            int px_status; \
            px_status = pthread_mutex_unlock( net_io_get_mtx() ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_IO_JOB_STATE_TRANSITION(job, next_state, next_reason)                        \
        {                                                                                \
            DBG_VERBOSE("%08x from %s to %s\n", job, g_net_io_job_state_strs[job->state],\
                         g_net_io_job_state_strs[next_state]);                           \
            job->state = next_state;                                                     \
            job->reason = next_reason;                                                   \
        }


//=======================================================================================
// Module API
//=======================================================================================

int32_t net_io_job_register_event_cb(net_io_job_event_func_t event_func, 
                                     net_io_session_hndl_t session_hndl, void *user_ctxt)
{
    int32_t net_io_rcode = NET_IO_SUCCESS;
    net_io_session_t *session = NULL;

    ASSERT(session_hndl);
    session = (net_io_session_t *)session_hndl;
    ASSERT(session->net_channel);
    XASSERT(session->net_channel->channel_type == e_Print, session->net_channel->channel_type);

    DBG_VERBOSE("==>%s (session=%08x chan=%08x)\n", __func__, 
                session, session->net_channel);

    NET_IO_JOB_LOCK_CONTEXT();
    switch(session->state)
    {
    case NET_IO_STATE_ACQUIRING:
        // don't have a channel so can't access job info template
        // fall through

    case NET_IO_STATE_ACTIVE:
        // too late because job info template could have already been copied
        net_io_rcode = NET_IO_NOT_PERMITTED;
        break;

    case NET_IO_STATE_INACTIVE:
        {
            /* set pji copy notify callback
             * 
             * NOTE: This is called at start of each new printer subsystem "job", when 
             * the pji copy constructor is invoked. For each new job, it effectively 
             * registers us for the observer callbacks defined below.
             */
            ASSERT(session->net_channel->pipe_hndl);
            ((IOReg *)(session->net_channel->pipe_hndl))->pjob_info->copy_notify_func = netio_pji_copy_notify;
            ((IOReg *)(session->net_channel->pipe_hndl))->pjob_info->copy_notify_user_ctxt = (void *)session->net_channel;
            session->job_event_func = event_func;
            session->job_event_user_ctxt = user_ctxt;
            netio_job_attach_observers(session->net_channel);
        }
        break;

    default:
        // unexpected session state
        XASSERT(0, session->state);
    }
    NET_IO_JOB_UNLOCK_CONTEXT();

    return net_io_rcode;
}

void net_io_job_cancel(net_io_job_hndl_t job_hndl)
{
    ASSERT(job_hndl);
    net_io_job_t *job = (net_io_job_t *)job_hndl;
    DBG_VERBOSE("==>%s (job=%08x)\n", __func__, job);
#ifdef DEBUG
    ASSERT(job->signature == NET_IO_JOB_CONTEXT_SIGNATURE);
#endif

    // signal that no more PDL/scan data should be transferred to/from the printer
    ASSERT(job->session); // job must always be associated with a netio session
    XASSERT(job == job->session->cur_job, (int)job->session->cur_job);
    job->session->cancel_data = true; 

    // TODO request printer subsystem cancel this specific job -- currently only a generic 
    // "control panel" cancel is supported by subsystem where the first job in pipeline is
    // cancelled. NOTE: job context will be freed by observer callback

    return;
}

net_io_job_state_reason_t net_io_job_get_state_reason(net_io_job_hndl_t job_hndl)
{
    ASSERT(job_hndl);
    net_io_job_t *job = (net_io_job_t *)job_hndl;

    if(!job || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE) )
    {
        return 0;
    }

    return job->reason;
}

uint32_t net_io_job_get_koctets_processed(net_io_job_hndl_t job_hndl)
{
    ASSERT(job_hndl);
    net_io_job_t *job = (net_io_job_t *)job_hndl;

    if(!job || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE) )
    {
        return 0;
    }

    return job->koctets_processed;
}

uint32_t net_io_job_get_pages_completed(net_io_job_hndl_t job_hndl)
{
    ASSERT(job_hndl);
    net_io_job_t *job = (net_io_job_t *)job_hndl;

    if(!job || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE) )
    {
        return 0;
    }

    return job->pages_completed;
}

uint32_t net_io_job_get_sheets_completed(net_io_job_hndl_t job_hndl)
{
    ASSERT(job_hndl);
    net_io_job_t *job = (net_io_job_t *)job_hndl;

    if(!job || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE) )
    {
        return 0;
    }

    return job->sheets_completed;
}


//=======================================================================================
// local public routines (internal to netio module)
//=======================================================================================

// remove any job-related modifications to the channel template prior to returning to 
// free channel pool must call with channel context locked
void netio_job_deregister_channel_events(net_io_channel_t *net_channel)
{
    DBG_VERBOSE("==>%s (chan=%08x)\n", __func__, net_channel);
    ASSERT(net_channel);

    switch(net_channel->channel_type)
    {
    case e_Print:
        ((IOReg *)(net_channel->pipe_hndl))->pjob_info->copy_notify_func = NULL;
        ((IOReg *)(net_channel->pipe_hndl))->pjob_info->copy_notify_user_ctxt = 0;
        netio_job_detach_observers(net_channel);
        break;
    default:
        // do nothing
        break;
    }

    return;
}

void netio_job_channel_init(struct net_io_channel_s *net_channel)
{
    DBG_VERBOSE("==>%s (chan=%08x)\n", __func__, net_channel);
    XASSERT(net_channel->channel_type == e_Print, net_channel->channel_type);

    // create job observers
    net_channel->jobstart_observer = Observer_Constructor(net_io_jobstart_observer_id,
                                                          netio_jobstart_notify);
    ASSERT(net_channel->jobstart_observer);
    net_channel->pageend_observer = Observer_Constructor(net_io_pageend_observer_id,
                                                         netio_pageend_notify); 
    ASSERT(net_channel->pageend_observer);
    net_channel->jobend_observer = Observer_Constructor(net_io_jobend_observer_id,
                                                        netio_jobend_notify);  
    ASSERT(net_channel->jobend_observer);
    net_channel->jobcancel_observer = Observer_Constructor(net_io_jobcancel_observer_id,
                                                           netio_jobcancel_notify);
    ASSERT(net_channel->jobcancel_observer);

    return;
}

void netio_job_detach(net_io_session_t *session)
{
    DBG_VERBOSE("==>%s (session=%08x)\n", __func__, session);
    ASSERT(session);

    // remove job reference to the session and vice versa
    NET_IO_JOB_LOCK_CONTEXT();
    if(session->cur_job)
    {
        session->cur_job->session = NULL;
        session->cur_job->event_func = NULL;
        session->cur_job = NULL;
    }
    NET_IO_JOB_UNLOCK_CONTEXT();

    return;
}

void netio_job_report_octets_processed(net_io_channel_t *net_channel, uint32_t octets)
{
    if(!octets)
    {
        goto done;
    }

    NET_IO_JOB_LOCK_CONTEXT();

    ASSERT(net_channel);
    if(net_channel->channel_type != e_Print ||
       !net_channel->session ||
       !net_channel->session->cur_job )
    {
        NET_IO_JOB_UNLOCK_CONTEXT();
        goto done;
    }

    net_io_job_t *job = net_channel->session->cur_job;
    ASSERT(job);

    job->octets_processed += octets; // add to previous remainder

    if(!job->koctets_processed && octets)
    {
        // koctets must be rounded up to the nearest 1024 octets, so increment on first
        // octet
        job->koctets_processed++;
        job->octets_processed--;
    }

    job->koctets_processed += job->octets_processed/1024; // discard koctet remainder
    job->octets_processed = job->octets_processed%1024; // save remainder

    NET_IO_JOB_UNLOCK_CONTEXT();

done:
    return;
}


//=======================================================================================
// Observer Notification Callbacks
//=======================================================================================

static void netio_jobstart_notify(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, subject_observer_oid_key, net_io_jobstart_observer_id));

    NET_IO_JOB_LOCK_CONTEXT();

    net_io_job_t *job = (net_io_job_t *)PrintJobInfo_Get_foreign_context(s);
    if(!job  || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE))
    {
        // dbg_printf("SKY IS FALLING %s\n", __func__);
        NET_IO_JOB_UNLOCK_CONTEXT();

        // no context associated with this job -- ignore the event
        return;
    }

    DBG_MSG("jobstart notification (job=%08x)\n", job);
#ifdef NETIO_DEBUG
    dbg_printf("NETIO: %08x JOBSTART job=%08x state=%d\n", job->session, job, job->state);
#endif // NETIO_DEBUG

    /*
     * It's possible to get some job observer notifications out of order, so any events 
     * we send to a network application must be conditional on our internal job state.
     */

    switch(job->state)
    {
    case NET_IO_JOB_STATE_PENDING:
        NET_IO_JOB_STATE_TRANSITION(job, 
                                    NET_IO_JOB_STATE_STARTED,
                                    NET_IO_JOB_STATE_REASON_NONE);
        if(job->event_func) // event_func may have been removed, e.g. by net_io_close()
        {
            job->event_func(NET_IO_JOB_EVENT_STARTED, 
                            (net_io_job_hndl_t)job, 
                            job->user_ctxt);
        }
        break;

    case NET_IO_JOB_STATE_STARTED:
        // invalid state
        ASSERT(0);
        //fall through

    case NET_IO_JOB_STATE_ABORTED:
    case NET_IO_JOB_STATE_CANCELED:
    case NET_IO_JOB_STATE_COMPLETED:
    default:
        // unexpected state
        DBG_VERBOSE("%s: BAD STATE %d\n", __func__, job->state);
        XASSERT(0, job->state);
        break;
    }

    NET_IO_JOB_UNLOCK_CONTEXT();
    return;
}

static void netio_pageend_notify(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, 
                               Observer_t, 
                               subject_observer_oid_key, 
                               net_io_pageend_observer_id));

    NET_IO_JOB_LOCK_CONTEXT();

    net_io_job_t *job = (net_io_job_t *)PrintJobInfo_Get_foreign_context(s);

    if(!job  || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE))
    {
        // dbg_printf("SKY IS FALLING %s\n", __func__);
        NET_IO_JOB_UNLOCK_CONTEXT();

        // no context associated with this job -- ignore the event
        return;
    }
  
    DBG_MSG("%s: (job=%08x state=%d)\n", __func__, job, job->state);

    // no duplex or pdl N-up, so pages == sheets
    job->pages_completed++;
    job->sheets_completed++;

    if(job->state == NET_IO_JOB_STATE_STARTED && job->event_func)
    {
        DBG_VERBOSE("%s: sending NET_IO_JOB_EVENT_SHEET_COMPLETED\n", __func__);
        job->event_func(NET_IO_JOB_EVENT_SHEET_COMPLETED, 
                        (net_io_job_hndl_t)job, 
                        job->user_ctxt);
    }

    NET_IO_JOB_UNLOCK_CONTEXT();
    return;
}

static void netio_jobend_notify(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, 
                               Observer_t, 
                               subject_observer_oid_key, 
                               net_io_jobend_observer_id));

    NET_IO_JOB_LOCK_CONTEXT();

    net_io_job_t *job = (net_io_job_t *)PrintJobInfo_Get_foreign_context(s);

    if(!job  || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE))
    {
        // dbg_printf("SKY IS FALLING %s\n", __func__);
        NET_IO_JOB_UNLOCK_CONTEXT();

        // no context associated with this job -- ignore the event
        return;
    }

    DBG_MSG("%s: (job=%08x state=%d koctets=%d sheets=%d)\n",
            __func__, job, job->state, job->koctets_processed, job->sheets_completed);
#ifdef NETIO_DEBUG
    dbg_printf("NETIO: %08x JOBEND job=%08x state=%d\n", job->session, job, job->state);
#endif // NETIO_DEBUG

    /*
     * It's possible to get some job observer notifications out of order, so any events 
     * we send to a network application must be conditional on our internal state 
     * representation.
     * 
     * Note: it's possible to get a "jobend" notification after a "jobcancel", in which 
     * case we must ignore this notification. We must send one and only one of 
     * NET_IO_JOB_EVENT_COMPLETED, NET_IO_JOB_EVENT_ABORTED, or NET_IO_JOB_EVENT_CANCELED 
     * to notify overlying network application of job completion.
     */

    // if we're still pending, send the required "started" event prior to "completed" event
    if(job->state == NET_IO_JOB_STATE_PENDING)
    {
        NET_IO_JOB_STATE_TRANSITION(job, 
                                    NET_IO_JOB_STATE_STARTED,
                                    NET_IO_JOB_STATE_REASON_NONE);
        if(job->event_func) // event_func may have been removed, e.g. by net_io_close()
        {
            DBG_VERBOSE("%s: sending NET_IO_JOB_EVENT_STARTED\n", __func__);
            job->event_func(NET_IO_JOB_EVENT_STARTED, 
                            (net_io_job_hndl_t)job, 
                            job->user_ctxt);
        }
    }

    // This assert is invalid. It is valid for a job to be orphaned with no attached
    // session if net_io_close() happens to get called before the job completes.
    //    XASSERT(job->session, (int)job);
    if(job->state == NET_IO_JOB_STATE_STARTED) // && job->session->eof)
    {
       uint32_t job_status = PrintJobInfo_Get_jobstatus(s);

       if(job_status == PARSEERROR)
       {
          NET_IO_JOB_STATE_TRANSITION(job, 
                                      NET_IO_JOB_STATE_COMPLETED, 
                                      NET_IO_JOB_STATE_REASON_PARSER_ERROR);          
       }
       else
       {
          NET_IO_JOB_STATE_TRANSITION(job, 
                                      NET_IO_JOB_STATE_COMPLETED, 
                                      NET_IO_JOB_STATE_REASON_NONE);
       }
       
        if(job->event_func) // event_func may have been removed, e.g. by net_io_close()
        {
            job->event_func(NET_IO_JOB_EVENT_COMPLETED, 
                            (net_io_job_hndl_t)job, 
                            job->user_ctxt);
        }
    }

    netio_job_destruct(job);

    NET_IO_JOB_UNLOCK_CONTEXT();
    
    return;
}

static void netio_jobcancel_notify(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, 
                               Observer_t, 
                               subject_observer_oid_key, 
                               net_io_jobcancel_observer_id));

    NET_IO_JOB_LOCK_CONTEXT();

    net_io_job_t *job = (net_io_job_t *)PrintJobInfo_Get_foreign_context(s);

    if(!job || (job->signature != NET_IO_JOB_CONTEXT_SIGNATURE) )
    {
        // dbg_printf("SKY IS FALLING %s\n", __func__);
        NET_IO_JOB_UNLOCK_CONTEXT();
        // no context associated with this job -- ignore the event
        return;
    }

    DBG_MSG("%s: (job=%08x state=%d)\n", __func__, job, job->state);
#ifdef NETIO_DEBUG
    dbg_printf("NETIO: %08x JOBCANCEL job=%08x state=%d\n", job->session, job, job->state);
#endif // NETIO_DEBUG


    /*
     * It's possible to get some job observer notifications out of order, so any events 
     * we send to a network application must be conditional on our internal state 
     * representation.
     * 
     * For example, jobcancel notification can be sent before jobstart if an error occurs 
     * while parsing the PJL preceeding a job, e.g. LANG command referencing an unsupported 
     * PDL.
     */

    // if we're still pending, send the required "started" event prior to "aborted" or
    // "canceled" event
    if(job->state == NET_IO_JOB_STATE_PENDING)
    {
        NET_IO_JOB_STATE_TRANSITION(job, 
                                    NET_IO_JOB_STATE_STARTED, 
                                    NET_IO_JOB_STATE_REASON_NONE);
        if(job->event_func) // event_func may have been removed, e.g. by net_io_close()
        {
            DBG_VERBOSE("%s: sending NET_IO_JOB_EVENT_STARTED\n", __func__);
            job->event_func(NET_IO_JOB_EVENT_STARTED, 
                            (net_io_job_hndl_t)job, 
                            job->user_ctxt);
        }
    }

    if(job->state == NET_IO_JOB_STATE_STARTED)
    {
        uint32_t job_status = PrintJobInfo_Get_jobstatus(s);
        if(job_status == PARSEERROR)
        {
            NET_IO_JOB_STATE_TRANSITION(job, 
                                        NET_IO_JOB_STATE_ABORTED,
                                        NET_IO_JOB_STATE_REASON_PARSER_ERROR);
            if(job->event_func) // event_func may have been removed, e.g. by net_io_close()
            {
                DBG_VERBOSE("%s: sending NET_IO_JOB_EVENT_ABORTED\n", __func__);
                job->event_func(NET_IO_JOB_EVENT_ABORTED, 
                                (net_io_job_hndl_t)job, 
                                job->user_ctxt);
            }
        }
        else if(job_status == UNSUPPORTED_PDL)
        {
            NET_IO_JOB_STATE_TRANSITION(job, 
                                        NET_IO_JOB_STATE_ABORTED,
                                        NET_IO_JOB_STATE_REASON_UNSUPPORTED_PDL);
            if(job->event_func) // event_func may have been removed, e.g. by net_io_close()
            {
                DBG_VERBOSE("%s: sending NET_IO_JOB_EVENT_ABORTED\n", __func__);
                job->event_func(NET_IO_JOB_EVENT_ABORTED, 
                                (net_io_job_hndl_t)job, 
                                job->user_ctxt);
            }
        }
        else // normal job cancel
        {
            NET_IO_JOB_STATE_TRANSITION(job, 
                                        NET_IO_JOB_STATE_CANCELED,
                                        NET_IO_JOB_STATE_REASON_NONE);
            if(job->event_func) // event_func may have been removed, e.g. by net_io_close()
            {
                DBG_VERBOSE("%s: sending NET_IO_JOB_EVENT_CANCELED\n", __func__);
                job->event_func(NET_IO_JOB_EVENT_CANCELED, 
                                (net_io_job_hndl_t)job,
                                job->user_ctxt);
            }
        }
    }

    PrintJobInfo_Set_foreign_context((PrintJobInfo_t *)s, (void *)0);
    netio_job_destruct(job);

    NET_IO_JOB_UNLOCK_CONTEXT();

    return;
}


//=======================================================================================
// Print Job Info Callback
//=======================================================================================
void netio_pji_copy_notify(PrintJobInfo_t *pji_copy, void *user_ctxt)
{
    ASSERT(user_ctxt);
    net_io_channel_t *net_channel = (net_io_channel_t *)user_ctxt;

    DBG_VERBOSE("==>%s (chan=%08x session=%08x)\n", __func__, 
                net_channel, net_channel->session);

    net_io_job_t *job;

    ASSERT(net_channel);
    ASSERT(net_channel->session);
    job = netio_job_construct(net_channel->session);
    if(job)
    {
        netio_job_attach(net_channel->session, job);
        ASSERT(job == net_channel->session->cur_job);
        PrintJobInfo_Set_foreign_context(pji_copy, (void *)job);
    }
    else
    {
        // job construct failed due to low memory
        // we will get no completion status event
        // TODO send an immediate event to the registered user callback and remove 
        // the assert
        ASSERT(0);
    }

    return;
}


//=======================================================================================
// local helper Routines
//=======================================================================================

// malloc and init a new netio job context
// returns a reference to the newly created job, or NULL if malloc fails
static net_io_job_t *netio_job_construct(net_io_session_t *session)
{
    net_io_job_t *job = NULL;

    ASSERT(session);

    job = (net_io_job_t *)MEM_MALLOC(sizeof(net_io_job_t));
    if(!job)
    {
        DBG_ERR("error allocating mem for job context!\n");
        goto done;
    }

    // initialize job context
    job->state = NET_IO_JOB_STATE_PENDING;
    job->reason = NET_IO_JOB_STATE_REASON_NONE;
    job->koctets_processed = 0;
    job->octets_processed = 0;
    job->pages_completed = 0;
    job->sheets_completed = 0;
    ASSERT(session->job_event_func);
    job->event_func = session->job_event_func;
    job->user_ctxt = session->job_event_user_ctxt;
    job->session = NULL; // job context will eventually be bound to a session
    job->signature = NET_IO_JOB_CONTEXT_SIGNATURE;

done:
    return job;
}

// attach netio job context to the session
static void netio_job_attach(net_io_session_t *session, net_io_job_t *job)
{
    ASSERT(session);
    ASSERT(job);

    NET_IO_JOB_LOCK_CONTEXT();

    /*
     *  discard any pre-exising job event registration
     * 
     *  This can happen if user registers again before job processing complete
     *  (i.e. job start notification and subsequent transition to inactive -- NOT
     *  the same as job completion).
    */
    if(session->cur_job)
    {
        ASSERT(0); // assert since normally this is indication of a programming error
        netio_job_detach(session);
    }

    // save job context
    DBG_VERBOSE("%s: binding job ctxt %08x to session %08x\n", __func__, job, session);
    XASSERT(session->cur_job == NULL, (uint32_t)session->cur_job);
    session->cur_job = job;
    job->session = session;

    NET_IO_JOB_UNLOCK_CONTEXT();

    return;
}

static void netio_job_destruct(net_io_job_t *job)
{
    DBG_VERBOSE("==>%s (job=%08x)\n", __func__, job);
    ASSERT(job);

    if(job->session)
    {
        netio_job_detach(job->session);
    }

    job->signature = 0;

    MEM_FREE_AND_NULL(job);

    return;
}

// must call with channel context locked
static void netio_job_attach_observers(net_io_channel_t *net_channel)
{
    DBG_VERBOSE("==>%s (chan=%08x)\n", __func__, net_channel);
    IOReg *pipe = (IOReg *)(net_channel->pipe_hndl);
    ASSERT(pipe);
    ASSERT(!net_channel->observers_attached);
    if(net_channel->jobstart_observer)
    {
        ATTACH_OBSERVER(pipe->pjob_info->jobstart_observers, 
                        net_channel->jobstart_observer);
    }
    if(net_channel->pageend_observer)
    {
        ATTACH_OBSERVER(pipe->pjob_info->pageend_observers, 
                        net_channel->pageend_observer);
    }
    if(net_channel->jobend_observer)
    {
        ATTACH_OBSERVER(pipe->pjob_info->jobend_observers, 
                        net_channel->jobend_observer);
    }
    if(net_channel->jobcancel_observer)
    {
        ATTACH_OBSERVER(pipe->pjob_info->canceljobend_observers, 
                        net_channel->jobcancel_observer);
    }
    net_channel->observers_attached = true;
    return;
}

// must call with channel context locked
static void netio_job_detach_observers(net_io_channel_t *net_channel)
{
    DBG_VERBOSE("==>%s (chan=%08x)\n", __func__, net_channel);
    IOReg *pipe = (IOReg *)(net_channel->pipe_hndl);
    ASSERT(pipe);
    if(!net_channel->observers_attached)
    {
        return;
    }

    if(net_channel->jobstart_observer)
    {
        DETACH_OBSERVER(pipe->pjob_info->jobstart_observers, 
                        net_channel->jobstart_observer);
    }
    if(net_channel->pageend_observer)
    {
        DETACH_OBSERVER(pipe->pjob_info->pageend_observers, 
                        net_channel->pageend_observer);
    }
    if(net_channel->jobend_observer)
    {
        DETACH_OBSERVER(pipe->pjob_info->jobend_observers, 
                        net_channel->jobend_observer);
    }
    if(net_channel->jobcancel_observer)
    {
        DETACH_OBSERVER(pipe->pjob_info->canceljobend_observers, 
                        net_channel->jobcancel_observer);
    }
    net_channel->observers_attached = false;
    return;
}
#endif //ifdef HAVE_PRINT_SUPPORT
