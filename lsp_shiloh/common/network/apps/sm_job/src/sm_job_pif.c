/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * @file sm_job_pif.c
 *
 * @brief The print interface between the sm job manager and the net_io layer.
 *
 */

#include <stdint.h>             // for uint32_t, ... 
#include <stdbool.h>            // for bool
#include <string.h>             // for memset()
#include <unistd.h>             // for unlink
#include <fcntl.h>              // for O_RDONLY
#include "lassert.h"            // for ASSERT() & XASSERT()
#include "error_types.h"        // for error_type_t
#include "paper_size_config.h"  // for mediasize_t
#include "paper_types_config.h" // for mediatype_t
#include "dprintf.h"            // for dprintf
#include "logger.h"             // for logging debug macros
#include "debug.h"
#include "memAPI.h"             // for MEM_FREE_AND_NULL, MEM_MALLOC
#include "list.h"               // for ATLISTENTRY
#include "mlist.h"              // for MInitList
#include "event_observer.h"     // for Subject_T_BaseClassMembers
#include "ATypes.h"             // for THR_PRI_NORMAL (posix_create_thread)
#include "net_logger.h"
#include "net_io_api.h"         // for net_io_*
#include "sm_job_api.h"         // for sm_job_*
#include "sm_job_internal_api.h"
#ifdef HAVE_IPP
 #include "ipp_api.h"
#endif 


//=============================================================================
// Defines
//=============================================================================

#define DBG_PRFX "SMJOBPIF: "             // DBG_VERBOSE, DBG_ERR, ...
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_SM_JOB

//// TODO TEMP release debug
//#define DEBUG
//#define DBG_ERR(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_MSG(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) dbg_printf(DBG_PRFX __VA_ARGS__)

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)

#define SMJOB_PIF_STACK_SIZE POSIX_MIN_STACK_SIZE         // The thread stack size.
#define SMJOB_PIF_NUM_MSG_BUFS   20             // The number of message buffers.
#define SMJOB_PIF_MQ_MSG_BUF_SIZE  2   //TX_2_ULONG The number of unsigned longs per message buffer.
#define SMJOB_PIF_MQ_MSG_BUF_BYTES (SMJOB_PIF_MQ_MSG_BUF_SIZE * sizeof(uint32_t)) // The size of each message buffer.
#define smjob_pif_job_done_ID 0x736D7069        // smpi

//=============================================================================
// Enums
//=============================================================================

// Messages sent to the print interface state machine.
typedef enum
{
    SMJOB_PIF_MSG_PRINT_JOB, 
    SMJOB_PIF_MSG_SESSION_INACTIVE,
    SMJOB_PIF_MSG_RECV_DATA_PENDING,
    SMJOB_PIF_MSG_SESSION_ACTIVE,
    SMJOB_PIF_MSG_HAVE_SEND_DATA,
    SMJOB_PIF_MSG_CONNECTION_CLOSED,
    SMJOB_PIF_MSG_CLOSE,
    SMJOB_PIF_MSG_CANCEL_PRINT_JOB,
    SMJOB_PIF_MSG_UNKNOWN,

    // add new msgs above this line
    SMJOB_PIF_NUM_MSGS

} smjob_pif_msg_type_t;

// States for the print interface state machine.
typedef enum
{
    SMJOB_STATE_OPENING,
    SMJOB_STATE_WAIT_RECV_DATA,
    SMJOB_STATE_READY,
    SMJOB_STATE_ACTIVE,
    SMJOB_STATE_CLOSING, 
    //SMJOB_STATE_PRINT_DONE,

    // add new states above this line
    SMJOB_PIF_NUM_STATES

} smjob_pif_state_t;

//=============================================================================
// Structures
//=============================================================================

// Message format
typedef struct smjob_pif_msg_s
{
    smjob_pif_msg_type_t type;
    // App-specific message data
    void *data;
} smjob_pif_msg_t;

// Message queue buffer
typedef union
{
    smjob_pif_msg_t msg;
    uint8_t     buf[SMJOB_PIF_MQ_MSG_BUF_BYTES]; 
} smjob_pif_msg_buf_t;

// The print interface session context
typedef struct smjob_pif_session_ctxt_s
{
    smjob_pif_state_t    state;
    net_io_session_hndl_t session_hndl;
    bool                  net_closed;
    bool                  net_io_job_started;
    bool                  net_io_job_completed;
    net_io_job_hndl_t       job_hndl;
} smjob_pif_session_ctxt_t;

// The print interface context structure
typedef struct smjob_pif_ctxt_s
{
    Channel_type_t       type; // io_device.h
    const char           *name;
    mqd_t                msg_queue;
    pthread_t            thread;
//    smjob_pif_msg_buf_t msg_bufs[SMJOB_PIF_NUM_MSG_BUFS];
    uint8_t              *thread_stack;
    smjob_t             *cur_print_job_info;
    smjob_pif_session_ctxt_t *cur_session;
} smjob_pif_ctxt_t;

typedef struct smjob_pif_job_status_s
{
    Subject_T_BaseClassMembers;  ///< the base class members.

    MLISTENTRY print_job_done_observers; 

    uint32_t status;

} smjob_pif_job_status_t;

//=============================================================================
// Local variables
//=============================================================================

static smjob_pif_ctxt_t *g_smjob_pif_ctxt_ptr = NULL; // The saved context pointer.
static smjob_list_t g_completed_jobs_list;             // The list of jobs that have been completed.
static bool g_printer_busy = false;                     // Whether the printer is busy with a job or not.
static smjob_pif_job_status_t g_smjob_pif_job_status;
static int32_t g_total_bytes = 0;

//=============================================================================
// Local function declarations
//=============================================================================

static smjob_pif_session_ctxt_t *pif_init_net_io();
static void* pif_loop(void* unused);
static int32_t pif_recv_CB_func(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt);
static void pif_notify_event_CB_func(net_io_session_event_t event, void *user_ctxt);
static void pif_job_event_notify_CB_func(net_io_job_event_t event, net_io_job_hndl_t job_hndl, void *user_ctxt);
static smjob_rcode_t pif_send_msg_wait_opt(smjob_pif_msg_type_t type, void *data, uint32_t pthread_wait_opt);
static void pif_handle_send_data(smjob_pif_session_ctxt_t *session_ctxt);
static void pif_notify_print_job_done(smjob_pif_job_status_t *s);

#ifndef NET_SM_STREAMING
static int32_t pif_get_recvd_data(char *buf, uint32_t len, uint32_t *timeout_msec, 
                                  char *rx_file_name, uint32_t *bytes_rd_so_far);
#endif

#ifdef DEBUG
static const char* pif_get_smjob_pif_msg_str(smjob_pif_msg_type_t tag);
static const char* pif_get_smjob_pif_state_str(smjob_pif_state_t tag);
static const char *_get_job_event_str(net_io_job_event_t event);
static const char *_get_job_state_reason_str(smjob_state_reason_t reason);
#else
static inline const char* pif_get_smjob_pif_msg_str(smjob_pif_msg_type_t tag){return 0;}
static inline const char* pif_get_smjob_pif_state_str(smjob_pif_state_t tag){return 0;}
static inline const char *_get_job_event_str(net_io_job_event_t event){return 0;}
static inline const char *_get_job_state_reason_str(smjob_state_reason_t reason){return 0;}
#endif

//=============================================================================
// Parameterized Macros
//=============================================================================
#define SMJOB_PIF_STATE_TRANSITION(session_ctxt, next_state) \
do                                                            \
{                                                             \
   DBG_VERBOSE("Transition from state %s to state %s\n", \
   pif_get_smjob_pif_state_str(session_ctxt->state),         \
   pif_get_smjob_pif_state_str(next_state));                 \
   session_ctxt->state = next_state;                          \
} while(0)

// Add a job to the completed job list.
#define ADD_JOB_TO_COMPLETED_LIST(sm_job)                                 \
do                                                                           \
{                                                                            \
       uint32_t px_ret_code = pthread_mutex_lock(&g_completed_jobs_list.list_mutex); \
       XASSERT(px_ret_code == 0, px_ret_code);                      \
       ATInsertTailList(&g_completed_jobs_list.job_list, sm_job->listnode); \
       px_ret_code = pthread_mutex_unlock(&g_completed_jobs_list.list_mutex);          \
       XASSERT(pthread_rcode == 0, pthread_rcode);                  \
} while (0)


//=============================================================================
// Thread
//=============================================================================

void smjob_pif_init( Channel_type_t type, const char *name )
{
    uint32_t pthread_rcode;

    // Create the pif context
    g_smjob_pif_ctxt_ptr = (smjob_pif_ctxt_t *)MEM_MALLOC(sizeof(smjob_pif_ctxt_t));
    ASSERT(g_smjob_pif_ctxt_ptr);

    g_smjob_pif_ctxt_ptr->type               = type;
    g_smjob_pif_ctxt_ptr->name               = name;
    g_smjob_pif_ctxt_ptr->thread_stack       = NULL;
    g_smjob_pif_ctxt_ptr->cur_print_job_info = NULL;
    g_smjob_pif_ctxt_ptr->cur_session        = NULL;

    // Initialize the apparatus for the observers to come.
    MInitList(&g_smjob_pif_job_status.print_job_done_observers);

    g_smjob_pif_job_status.subject_observer_oid_key = smjob_pif_job_done_ID;

    // Initialize the completed jobs list and its mutex.
    ATInitList( &g_completed_jobs_list.job_list );

    pthread_rcode = posix_mutex_init( &g_completed_jobs_list.list_mutex);
    ASSERT(pthread_rcode == 0);


    posix_create_message_queue(&g_smjob_pif_ctxt_ptr->msg_queue, 
                                    (CHAR *)g_smjob_pif_ctxt_ptr->name, 
                                    SMJOB_PIF_NUM_MSG_BUFS,  sizeof(smjob_pif_msg_buf_t));

    /* 8-byte stack align req'd by ThreadX */
    g_smjob_pif_ctxt_ptr->thread_stack = MEM_MALLOC(SMJOB_PIF_STACK_SIZE); 
    ASSERT(g_smjob_pif_ctxt_ptr->thread_stack);

    pthread_rcode = posix_create_thread(&g_smjob_pif_ctxt_ptr->thread, pif_loop,
                                        (void *)g_smjob_pif_ctxt_ptr, (char *)name, 
                                     g_smjob_pif_ctxt_ptr->thread_stack, 
                                     SMJOB_PIF_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating thread for %s (pthread_rcode=0x%02x)\n", name, 
                        pthread_rcode);
        ASSERT(0);
    }

    DBG_VERBOSE("%s: successful \n", __func__);

}

void* pif_loop(void* unused)
{
    UINT pthread_rcode;
    smjob_rcode_t smjob_rcode;
    smjob_pif_msg_buf_t msg_buf;
    smjob_pif_msg_t *msg;
    smjob_pif_session_ctxt_t *session_ctxt;

    while(1)
    {
        session_ctxt = NULL;

        // Wait for a message to show up.
        pthread_rcode = posix_wait_for_message(g_smjob_pif_ctxt_ptr->msg_queue, (char *)&msg_buf,
                                               sizeof(smjob_pif_msg_buf_t), POSIX_WAIT_FOREVER);
        if(pthread_rcode != 0)
        {
            DBG_VERBOSE("error reading g_msg_queue for %s (pthread_rcode=0x%02x)\n", 
                            g_smjob_pif_ctxt_ptr->name, pthread_rcode);
            break;
        }

        msg = &msg_buf.msg;
        ASSERT(msg->type < SMJOB_PIF_NUM_MSGS);

        // this check assumes g_smjob_pif_ctxt_ptr->cur_session will never be set to 
        // NULL outside the context of this state machine loop
        if(g_smjob_pif_ctxt_ptr->cur_session)
        {
            session_ctxt = (smjob_pif_session_ctxt_t *)msg->data;
            XASSERT(g_smjob_pif_ctxt_ptr->cur_session == session_ctxt, 
                    (unsigned int)g_smjob_pif_ctxt_ptr->cur_session);
        }

        // Message sent to cancel a print job.
        if (msg->type == SMJOB_PIF_MSG_CANCEL_PRINT_JOB)
        {
            DBG_VERBOSE("%s got the cancel message\n", g_smjob_pif_ctxt_ptr->name);
        }
        else // Either a message to change states or a message for a particular state.
        {
            if(session_ctxt)
            {
                DBG_VERBOSE("1: %s session %08x got msg %s in state %s\n", 
                               g_smjob_pif_ctxt_ptr->name, session_ctxt, 
                                pif_get_smjob_pif_msg_str(msg->type), 
                                pif_get_smjob_pif_state_str(session_ctxt->state));
            }
            else
            {
                DBG_VERBOSE("%s got stateless msg %s\n", 
                            g_smjob_pif_ctxt_ptr->name, 
                            pif_get_smjob_pif_msg_str(msg->type));
            }
        }

        //  Process stateless messages
        switch(msg->type)
        {
            case SMJOB_PIF_MSG_PRINT_JOB:
                {
                    // We received a message to start a print job. Start the process.
                    session_ctxt = pif_init_net_io();
                }
            
                // If successful, set the printer to being busy.
                if (session_ctxt)
                {
                    g_printer_busy = true;
                }

                ASSERT(session_ctxt);
                break;
            
            default:
                // ignore other messages
                break;
        }

        //  process stateful messages
        if(!session_ctxt)
        {
            // We already handled the non-stateful messages.
            continue;
        }

        ASSERT(session_ctxt->state < SMJOB_PIF_NUM_STATES);
        DBG_VERBOSE("2: %s session %08x got msg %s in state %s\n", 
                       g_smjob_pif_ctxt_ptr->name, session_ctxt, 
                       pif_get_smjob_pif_msg_str(msg->type), 
                       pif_get_smjob_pif_state_str(session_ctxt->state));

        switch(session_ctxt->state)
        {
         //=============================================================================
         // SMJOB_STATE_OPENING
         //=============================================================================
        case SMJOB_STATE_OPENING:
           DBG_VERBOSE("%s %08x got msg %s in state %s\n", 
                       g_smjob_pif_ctxt_ptr->name, session_ctxt, 
                       pif_get_smjob_pif_msg_str(msg->type), 
                       pif_get_smjob_pif_state_str(session_ctxt->state));

            switch(msg->type)
            {
            case SMJOB_PIF_MSG_SESSION_INACTIVE:

               // Now we can register a callback to see how the job is progressing.
               net_io_job_register_event_cb(pif_job_event_notify_CB_func,
                                            g_smjob_pif_ctxt_ptr->cur_session->session_hndl, 
                                            g_smjob_pif_ctxt_ptr->cur_session);

                // An I/O channel has been acquired. Setup to transition states and
                // signal netio that we are ready.
                SMJOB_PIF_STATE_TRANSITION(session_ctxt, SMJOB_STATE_READY);
                net_io_ready(session_ctxt->session_hndl);
                break;

            case SMJOB_PIF_MSG_CANCEL_PRINT_JOB:
                SMJOB_PIF_STATE_TRANSITION(session_ctxt, SMJOB_STATE_CLOSING);
                break;

            default:
                // Ignore any other messages in this state.
                DBG_VERBOSE("%s %08x ignoring msg %s!\n", g_smjob_pif_ctxt_ptr->name,
                            session_ctxt,
                            pif_get_smjob_pif_msg_str(msg->type));
                break;
            }
            break;

        //=============================================================================
        // SMJOB_STATE_READY
        //=============================================================================
        case SMJOB_STATE_READY:
           DBG_VERBOSE("%s %08x got msg %s in state %s\n", 
                       g_smjob_pif_ctxt_ptr->name, session_ctxt, 
                       pif_get_smjob_pif_msg_str(msg->type), 
                       pif_get_smjob_pif_state_str(session_ctxt->state));

            switch(msg->type)
            {
            case SMJOB_PIF_MSG_SESSION_ACTIVE:
                SMJOB_PIF_STATE_TRANSITION(session_ctxt, SMJOB_STATE_ACTIVE);
                break;

            case SMJOB_PIF_MSG_HAVE_SEND_DATA:
                pif_handle_send_data(session_ctxt);
                break;

            case SMJOB_PIF_MSG_CANCEL_PRINT_JOB:
                SMJOB_PIF_STATE_TRANSITION(session_ctxt, SMJOB_STATE_CLOSING);
                break;

            default:
                // Ignore any other messages in this state.
                DBG_VERBOSE("%s %08x ignoring msg %s!\n", g_smjob_pif_ctxt_ptr->name, 
                            session_ctxt, pif_get_smjob_pif_msg_str(msg->type));
                break;
            }
            break;

       //=============================================================================
       // SMJOB_STATE_ACTIVE
       //=============================================================================
        case SMJOB_STATE_ACTIVE:
          DBG_VERBOSE("%s %08x got msg %s in state %s\n", 
                      g_smjob_pif_ctxt_ptr->name, session_ctxt, 
                      pif_get_smjob_pif_msg_str(msg->type), 
                      pif_get_smjob_pif_state_str(session_ctxt->state));

            switch(msg->type)
            {
            case SMJOB_PIF_MSG_HAVE_SEND_DATA:
                pif_handle_send_data(session_ctxt);
                break;

            case SMJOB_PIF_MSG_SESSION_INACTIVE:
                // I/O channel has gone inactive 
                {
                    /* 
                       if the session goes inactive without parsting started, i.e. no job is recoginzed
                       in the data stream so all the data is thrown away, then the
                       pif_job_event_notify_CB_func function will not get called to close out
                       the job.  In this case, we abort the job and set a document format error.
                     */
                    if (session_ctxt->net_io_job_started == false)
                    {
                        smjob_t *sm_job;
                        
                        /* get the job */
                        sm_job = g_smjob_pif_ctxt_ptr->cur_print_job_info;
                        ASSERT(sm_job);
                        
                        DBG_ERR("%s %08x session goes inactive without job start, abort and indicate unsupported doc format\n", 
                                      g_smjob_pif_ctxt_ptr->name, session_ctxt);

                        smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_ABORTED, SMJOB_STATE_REASON_ABORTED_BY_SYSTEM, SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT, 0, 0);

                        /* send job close message */
                        smjob_rcode_t status = pif_send_msg_wait_opt(SMJOB_PIF_MSG_CLOSE,
                                                                     (void *)session_ctxt, 
                                                                     POSIX_WAIT_FOREVER);
                        if (status != SMJOB_OK)
                        {
                            DBG_ERR("ERROR %s - the call to pif_send_msg_wait_opt failed.\n",
                                    __func__);
                        }

                        // It's possible that the user didn't register a CB func in the smjob_create func
                        if ( sm_job->job_status_func )
                        {
                            /* call job_status_func callback indicating job state and reason
                               has changed */
                            smjob_status_events_t smjob_event = (SMJOB_STATUS_EVENT_STATE |
                                                               SMJOB_STATUS_EVENT_STATE_REASONS);
                            uint32_t job_id = sm_job->job_status->job_id;
                            
                            DBG_VERBOSE("%s - calling sm_job->job_status_func\n", __func__);
                            sm_job->job_status_func( job_id, smjob_event, sm_job->user_data );
                        }
                    }
                    else
                    {
                       DBG_VERBOSE("%s - setting to SMJOB_STATE_CLOSING\n", __func__);

                       SMJOB_PIF_STATE_TRANSITION(session_ctxt, SMJOB_STATE_CLOSING);
                    }
                }
                break;

            case SMJOB_PIF_MSG_CANCEL_PRINT_JOB:
                if(session_ctxt->job_hndl != NET_IO_INVALID_JOB_HANDLE)
                {
                    net_io_job_cancel(session_ctxt->job_hndl);
                }
                SMJOB_PIF_STATE_TRANSITION(session_ctxt, SMJOB_STATE_CLOSING);
                break;

            case SMJOB_PIF_MSG_CLOSE:
                SMJOB_PIF_STATE_TRANSITION(session_ctxt, SMJOB_STATE_CLOSING);
                smjob_rcode = pif_send_msg_wait_opt(SMJOB_PIF_MSG_CLOSE, (void *)session_ctxt, POSIX_WAIT_FOREVER);
                XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);
                UNUSED_VAR(smjob_rcode);
                break;

            default:
                // Ignore any other messages in this state.
                DBG_VERBOSE("%s %08x ignoring msg %s!\n", g_smjob_pif_ctxt_ptr->name, 
                            session_ctxt, pif_get_smjob_pif_msg_str(msg->type));
                break;
            }
            break;

        //=============================================================================
        // SMJOB_STATE_CLOSING
        //=============================================================================
        case SMJOB_STATE_CLOSING:
           DBG_VERBOSE("%s %08x got msg %s in state %s\n", 
                       g_smjob_pif_ctxt_ptr->name, session_ctxt, 
                       pif_get_smjob_pif_msg_str(msg->type), 
                       pif_get_smjob_pif_state_str(session_ctxt->state));

            switch(msg->type)
            {
            case SMJOB_PIF_MSG_CLOSE:

                // Tell netio that we are done.
                net_io_close(session_ctxt->session_hndl);                

                // Cleanup the session context.
                //tx_mutex_delete(&session_ctxt->mtx);
                g_smjob_pif_ctxt_ptr->cur_session = NULL;
                MEM_FREE_AND_NULL(session_ctxt);

                // Regardless of whether the job actually printed or not, put it in the
                // completed list. The final dispensation of the job will be set by
                // other routines and even if it was canceled, this will be set.
                ADD_JOB_TO_COMPLETED_LIST(&g_smjob_pif_ctxt_ptr->cur_print_job_info);

                // Get rid of the file if there was one.
#ifndef NET_SM_STREAMING
                smjob_pif_remove_file_if_exists(g_smjob_pif_ctxt_ptr->cur_print_job_info->sm_doc->doc_ticket->doc_description.doc_name);
#endif

                // No longer point to this job.
                g_smjob_pif_ctxt_ptr->cur_print_job_info = NULL;
                
                // Tell our observers that we are done.
                pif_notify_print_job_done(&g_smjob_pif_job_status);

                // Reset this flag.
                g_printer_busy = false;
                break;

            case SMJOB_PIF_MSG_CANCEL_PRINT_JOB:
                DBG_VERBOSE("Received SMJOB_PIF_MSG_CANCEL_PRINT_JOB but it is too late.\n");
                break;                

            default:
                // Ignore any other messages in this state.
                DBG_VERBOSE("%s %08x ignoring msg %s!\n", g_smjob_pif_ctxt_ptr->name, 
                            session_ctxt, pif_get_smjob_pif_msg_str(msg->type));
                break;
            }
            break;

        default:
            // unknown state - ignore
            XASSERT(0, session_ctxt->state);
            break;
        }

    } // end while(1)
    return 0;
}

// Sent to the print done observers.
int smjob_status_pif_job_status(Subject_t *s)
{
    return ((smjob_pif_job_status_t *)s)->status;
}
void pif_notify_print_job_done(smjob_pif_job_status_t *s)
{
    //DBG_VERBOSE("\n\n%s - sent the message observers that the job is done.\n\n", __func__);

    NOTIFY_OBSERVERS(s, print_job_done_observers);
}

//=============================================================================
// Local functions
//=============================================================================

smjob_pif_session_ctxt_t *pif_init_net_io()
{
    int32_t               net_io_rcode;
    net_io_func_table_t   func_table;
    smjob_pif_session_ctxt_t *session_ctxt = NULL; 

    // Setup the function table for net_io
    func_table.io_notify = pif_notify_event_CB_func;
    func_table.io_recv   = pif_recv_CB_func;

    session_ctxt = (smjob_pif_session_ctxt_t *)MEM_MALLOC(sizeof(smjob_pif_session_ctxt_t));
    if(!session_ctxt)
    {
        return NULL;
    }

    // Initialize context
    session_ctxt->state      = SMJOB_STATE_OPENING;
    session_ctxt->net_closed = false;
    session_ctxt->net_io_job_started = false;
    session_ctxt->net_io_job_completed = false;
    session_ctxt->job_hndl = NET_IO_INVALID_JOB_HANDLE;

    net_io_rcode = net_io_open(g_smjob_pif_ctxt_ptr->type, &func_table, 
                               (void *)session_ctxt, &session_ctxt->session_hndl);

    if ( net_io_rcode != NET_IO_SUCCESS )
    {
        if ( session_ctxt )
        {
            MEM_FREE_AND_NULL( session_ctxt );
        }
        return NULL;
    }

    XASSERT(g_smjob_pif_ctxt_ptr->cur_session == NULL, 
            (unsigned int)g_smjob_pif_ctxt_ptr->cur_session);
    g_smjob_pif_ctxt_ptr->cur_session = session_ctxt;

    return session_ctxt;
}

// Handle any data that is sent our way.
void pif_handle_send_data(smjob_pif_session_ctxt_t *session_ctxt)
{
    ASSERT(session_ctxt);

    uint32_t len;
    char *buf;

    buf = net_io_get_send_data(session_ctxt->session_hndl, &len);

    // In our case, we simply dump the data.
    while(buf)
    {
        MEM_FREE_AND_NULL(buf);
        buf = net_io_get_send_data(session_ctxt->session_hndl, &len);
    }

    DBG_VERBOSE("%s - Got send data and just dumped it.\n", __func__);
}

// Remove a file if one was used in this print job.
void smjob_pif_remove_file_if_exists(char *file_name)
{
    int status;

    if (file_name)
    {
        status = unlink(file_name);

        if (status != 0 && status != ENOENT)
        {
            DBG_ERR("ERROR %s - failure to delete file: %s, returned: %#x\n", __func__,
                            file_name, status);
        }
    }
}

// Debug code to convert a print interface message to a string.
#ifdef DEBUG
const char* pif_get_smjob_pif_msg_str(smjob_pif_msg_type_t tag)
{
#define _CASE(x) case x: return #x
  switch (tag)
  {
    _CASE(SMJOB_PIF_MSG_PRINT_JOB);
    _CASE(SMJOB_PIF_MSG_SESSION_INACTIVE);
    _CASE(SMJOB_PIF_MSG_RECV_DATA_PENDING);
    _CASE(SMJOB_PIF_MSG_SESSION_ACTIVE);
    _CASE(SMJOB_PIF_MSG_HAVE_SEND_DATA);
    _CASE(SMJOB_PIF_MSG_CONNECTION_CLOSED);
    _CASE(SMJOB_PIF_MSG_CLOSE);
    _CASE(SMJOB_PIF_MSG_CANCEL_PRINT_JOB);
    _CASE(SMJOB_PIF_MSG_UNKNOWN);
    default: break;
  }

  return "NEED_TO_ADD_SMJOB_PIF_MSG";
#undef _CASE
}

// Debug code to convert the print interface state to a string.
const char* pif_get_smjob_pif_state_str(smjob_pif_state_t tag)
{
#define _CASE(x) case x: return #x
  switch (tag)
  {
    _CASE(SMJOB_STATE_OPENING);
    _CASE(SMJOB_STATE_WAIT_RECV_DATA);
    _CASE(SMJOB_STATE_READY);
    _CASE(SMJOB_STATE_ACTIVE);
    _CASE(SMJOB_STATE_CLOSING);
    default: break;
  }

  return "NEED_TO_ADD_SMJOB_STATE_MSG";
#undef _CASE
}
#endif // DEBUG

// Send a message to the print interface state machine.
static smjob_rcode_t pif_send_msg_wait_opt(smjob_pif_msg_type_t type,
                                            void *data, 
                                            uint32_t pthread_wait_opt)
{
    smjob_pif_msg_buf_t buf;

    buf.msg.type = type;
    buf.msg.data = data;

    uint32_t pthread_rcode = posix_message_send( g_smjob_pif_ctxt_ptr->msg_queue, (char *)&buf,
                                                 sizeof(buf), MQ_DEFAULT_PRIORITY,
                                                 pthread_wait_opt );
    if( pthread_rcode == ETIMEDOUT )
    {
        /* 
         *  msg queue full
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and
         *  pthread_wait_opt must therefore be set to TX_NO_WAIT
         */
        return( SMJOB_FAIL );
    }
    else if( pthread_rcode != 0 )
    {
        // any other errors are unexpected and unhandled
        XASSERT(0, pthread_rcode);
    }

    return( SMJOB_OK );
}


// Get the data from a file.
#ifndef NET_SM_STREAMING
int32_t pif_get_recvd_data(char     *buf, 
                           uint32_t len, 
                           uint32_t *timeout_msec, 
                           char     *rx_file_name,
                           uint32_t *bytes_rd_so_far)
{
    int32_t fd;
    int32_t offset;
    int32_t bytes_rd = 0;

    fd = open(rx_file_name, O_RDONLY);
    if (fd != -1)
    {
        // Set the offset in the file to where we have read so far.
        offset = lseek(fd, *bytes_rd_so_far, 0);
        if (offset != -1)
        {
            bytes_rd = read(fd, buf, len);

            if (bytes_rd != -1)
            {
                *bytes_rd_so_far += bytes_rd;
                DBG_VERBOSE("%s - read %d bytes so far.\n", __func__, *bytes_rd_so_far);
            }
        }

        close(fd);
    }
    else
    {
        DBG_ERR("ERROR %s - could not open file: %s\n", __func__, rx_file_name);
    }

    return(bytes_rd);
}
#endif

// Get data for net_io, called from pif_recv_CB_func
// TODO: Need to handle case of multiple doc_handles added to the job; this just takes the first doc
int32_t pif_get_data(char            *buf, 
                     uint32_t        len, 
                     uint32_t        *timeout_msec, 
                     smjob_t *sm_job)
{
    int32_t bytes_rx = 0;
    char *jpeg_hdr_str = "@PJL ENTER LANGUAGE = JPG\n";
    smjob_print_doc_description_t print_desc;
    
    // If reading a jpg file, need to inject header for this pipe to be picked up by the jpeg parser
    print_desc = sm_job->sm_doc->doc_ticket->doc_description.type.print;
    if (!strcmp(SMJOB_FORMAT_IMAGE_JPEG, print_desc.format))
    {
        if ( ( 0 == g_total_bytes ) && ( len >= strlen(jpeg_hdr_str) ) )
        {
            strncpy( buf, jpeg_hdr_str, strlen( jpeg_hdr_str ) );
            buf += strlen(jpeg_hdr_str);
            len -= strlen( jpeg_hdr_str );
        }
    }

#ifdef NET_SM_STREAMING
    smjob_recv_func_t recv_func;
    XASSERT(sm_job->job_ticket->job_type == SMJOB_TYPE_PRINT, sm_job->job_ticket->job_type);
    recv_func = sm_job->sm_doc->doc_ticket->doc_description.type.print.recv_func;
    ASSERT( recv_func );
    bytes_rx = recv_func(buf, len, timeout_msec, 
                         (void*)sm_job->sm_doc->doc_ticket->doc_description.type.print.user_data);
#else
    bytes_rx = pif_get_recvd_data(buf, len, timeout_msec, sm_job->sm_doc->doc_ticket->doc_description.doc_name, &sm_job->bytes_read);
#endif

    // NOTE: Events like an IO timeout can return negative byte counts
    if (bytes_rx > 0)
    {
        g_total_bytes += bytes_rx;
    }

    return(bytes_rx);
}

//=============================================================================
// Callback Routines
//=============================================================================

// A callback that is registered with the netio module and is used to get the
// data for a print job when it is requested.
int32_t pif_recv_CB_func(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt)
{
    int32_t bytes_rx = 0;
    smjob_t *sm_job;
    sm_job = g_smjob_pif_ctxt_ptr->cur_print_job_info;

    smjob_print_doc_description_t *print_desc;
    print_desc = &sm_job->sm_doc->doc_ticket->doc_description.type.print;

    // If reading a jpg file, need to inject header for this pipe to be picked up by the jpeg parser
    char *jpeg_hdr_str = "@PJL ENTER LANGUAGE = JPG\n";
    if (!strcmp(SMJOB_FORMAT_IMAGE_JPEG, print_desc->format))
    {
        if ( ( 0 == g_total_bytes ) && ( len >= strlen(jpeg_hdr_str) ) )
        {
            strncpy( buf, jpeg_hdr_str, strlen( jpeg_hdr_str ) );
            buf += strlen(jpeg_hdr_str);
            len -= strlen( jpeg_hdr_str );
        }
    }

    XASSERT(sm_job->job_ticket->job_type == SMJOB_TYPE_PRINT, sm_job->job_ticket->job_type);
    ASSERT( print_desc->recv_func );
    bytes_rx = print_desc->recv_func(buf, len, timeout_msec, print_desc->user_data);

    // NOTE: Events like an IO timeout can return negative byte counts
    if (bytes_rx > 0)
    {
        g_total_bytes += bytes_rx;
    }

    if(bytes_rx <= 0)
    {
        if(bytes_rx == 0)
        {
            // eof
            DBG_VERBOSE("%s: recv EOF detected\n", __func__);
            sm_job->eof = true;
        }
        else if (*timeout_msec != 0) // ignore timeout errors - these are recoverable
        {
            // read error - indicate eof w/ errors
            DBG_ERR("%s: recv ERRORS detected\n", __func__);
            sm_job->eof = true;
            sm_job->errors = true;
        }
    }
    
    return(bytes_rx);
}

// A callback that is registered with the netio module and is used to update
// the print interface state machine.
void pif_notify_event_CB_func(net_io_session_event_t event, void *user_ctxt)
{
    smjob_pif_session_ctxt_t *session_ctxt = (smjob_pif_session_ctxt_t *)user_ctxt;
    smjob_pif_msg_type_t smjob_pif_msg_type = SMJOB_PIF_MSG_UNKNOWN;
    smjob_rcode_t status;

    ASSERT( user_ctxt );

    switch( event )
    {
        case NET_IO_SESSION_EVENT_INACTIVE:

            DBG_VERBOSE("%s: event NET_IO_SESSION_EVENT_INACTIVE context x%x \n", __func__, user_ctxt);
            smjob_pif_msg_type = SMJOB_PIF_MSG_SESSION_INACTIVE;
            break;

        case NET_IO_SESSION_EVENT_ACTIVE:
            DBG_VERBOSE("%s: event = NET_IO_SESSION_EVENT_ACTIVE x%x \n", __func__, user_ctxt);
            smjob_pif_msg_type = SMJOB_PIF_MSG_SESSION_ACTIVE;
            break;

        case NET_IO_SESSION_EVENT_HAVE_SEND_DATA:
            DBG_VERBOSE("%s: event = NET_IO_SESSION_EVENT_HAVE_SEND_DATA x%x\n", __func__, user_ctxt);
            smjob_pif_msg_type = SMJOB_PIF_MSG_HAVE_SEND_DATA;
            break;

        default:
            // unhandled event
            XASSERT(0, event);
            break;
    }

    status = pif_send_msg_wait_opt(smjob_pif_msg_type, 
                                  (void *)session_ctxt, 
                                  POSIX_WAIT_FOREVER);
    if (status != SMJOB_OK)
    {
        DBG_ERR("ERROR %s - the call to pif_send_msg_wait_opt failed.\n", __func__);
    }

}

// A function that is registered with the netio module to update the status of a job
void pif_job_event_notify_CB_func(net_io_job_event_t event, 
                                  net_io_job_hndl_t job_hndl, 
                                  void *user_ctxt)
{
    uint32_t job_id;
    smjob_pif_session_ctxt_t *session_ctxt;
    smjob_t *sm_job;

    // Let us unwind and set some pointers now.
    session_ctxt = (smjob_pif_session_ctxt_t *)user_ctxt;
    sm_job = g_smjob_pif_ctxt_ptr->cur_print_job_info;
    job_id = sm_job->job_status->job_id;

    int i;
    smjob_status_t prev_smjob_status;
    
    // Need this to determine if a change occured
    prev_smjob_status.state = sm_job->job_status->state;
    prev_smjob_status.num_reasons = sm_job->job_status->num_reasons;
    for ( i = 0; i < SMJOB_MAX_STATE_REASONS; i++ )
    {
        prev_smjob_status.reasons[i] = sm_job->job_status->reasons[i];
    }
    prev_smjob_status.koctets_processed = sm_job->job_status->koctets_processed;
    prev_smjob_status.sheets_completed = sm_job->job_status->sheets_completed;

    // Sanity check
    ASSERT(sm_job->job_status->job_id == job_id);

    DBG_MSG( "Enter %s - event: %s for job id: %d\n", __FUNCTION__, 
             _get_job_event_str(event), job_id );
    sm_job->job_status->koctets_processed = net_io_job_get_koctets_processed(job_hndl);
    sm_job->job_status->sheets_completed = net_io_job_get_sheets_completed(job_hndl);

    switch (event)
    {
    case NET_IO_JOB_EVENT_STARTED:
        smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_PROCESSING, SMJOB_STATE_REASON_JOB_PRINTING, 0, 0, 0);
        
        XASSERT(session_ctxt->job_hndl == NET_IO_INVALID_JOB_HANDLE, 
                (unsigned int)session_ctxt->job_hndl);
        session_ctxt->job_hndl = job_hndl;
        session_ctxt->net_io_job_started = true;
        DBG_VERBOSE( "%s: job %08x started on session %08x\n", __FUNCTION__, job_hndl,
                     session_ctxt);
        break;
    case NET_IO_JOB_EVENT_PAUSED:
        smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_PROCESSING_STOPPED, SMJOB_STATE_REASON_PRINTER_STOPPED, 0, 0, 0);
        break;
    case NET_IO_JOB_EVENT_COMPLETED:
        // we can get multiple NET_IO_JOB_EVENT_COMPLETED event per smjob job since
        // multiple netio jobs may be sent in a single PDL data stream, so we only
        // acknowledge the last one ASSUMPTION: we will get the final
        // NET_IO_JOB_EVENT_COMPLETED event only *after* receiving all PDL data.
        //
        // NOTE: this check for eof depends upon multiple documents per job being
        // supported. If they are not then we can just go ahead and set the
        // states and reasons.
#ifdef HAVE_IPP
        if(sm_job->eof || !ipp_chk_multiple_docs_per_job_supported()) 
#else
        if(sm_job->eof) 
#endif        
        {
            DBG_MSG("%s: got EOF (job %08x)\n", __func__, sm_job);

            // Check to see if the cancel was set on this job before we got to the callback.
            // If so then don't change it.
            if (sm_job->cancelled)
            {
                DBG_MSG("%s: Job %08x was cancelled!\n", __func__, sm_job);
                
                smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_CANCELED, SMJOB_STATE_REASON_JOB_CANCELED_BY_USER, 0, 0, 0);
            }
            else
            {
                switch (net_io_job_get_state_reason(job_hndl))
                {
                    case NET_IO_JOB_STATE_REASON_UNSUPPORTED_PDL:
                    case NET_IO_JOB_STATE_REASON_PARSER_ERROR:
                        if (session_ctxt->net_io_job_started)
                            smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_ABORTED, SMJOB_STATE_REASON_ABORTED_BY_SYSTEM, SMJOB_STATE_REASON_DOC_FORMAT_ERROR, 0, 0);
                        else
                            smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_ABORTED, SMJOB_STATE_REASON_ABORTED_BY_SYSTEM, SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT, 0, 0);
                        break;

                    default:
                        // See if the job was cancelled by the user before we got here. If so
                        // then don't change the state/reason.
                        if (sm_job->job_status->state != SMJOB_STATE_CANCELED)
                        {
                            if (sm_job->errors)
                            {
                              smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_COMPLETED, SMJOB_STATE_REASON_JOB_COMPLETED_WITH_ERRORS, 0, 0, 0);
                            }
                            else
                            {
                               smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_COMPLETED, SMJOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY, 0, 0, 0);
                            }
                        }
                        break;
                }
            }


            DBG_MSG("%s - with event: %s\n", __func__, _get_job_event_str(event));
            DBG_MSG("setting reasons[0] to %s\n", 
                    _get_job_state_reason_str(sm_job->job_status->reasons[0]));
            
            session_ctxt->net_io_job_completed = true;
        }

        // set the job handle to invalid when we get the job complete event, so if another
        // job inside the PDL is encountered we store the new job handle passed in.
        session_ctxt->job_hndl = NET_IO_INVALID_JOB_HANDLE;
        break;
    case NET_IO_JOB_EVENT_ABORTED:
       {
          net_io_job_event_t new_event;

          new_event = net_io_job_get_state_reason(job_hndl);

          switch(new_event)
          {
            case NET_IO_JOB_STATE_REASON_UNSUPPORTED_PDL:
                smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_ABORTED, SMJOB_STATE_REASON_ABORTED_BY_SYSTEM, SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT, 0, 0);
                break;
            case NET_IO_JOB_STATE_REASON_PARSER_ERROR:
                if (session_ctxt->net_io_job_started)
                    smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_ABORTED, SMJOB_STATE_REASON_ABORTED_BY_SYSTEM, SMJOB_STATE_REASON_DOC_FORMAT_ERROR, 0, 0);
                else
                    smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_ABORTED, SMJOB_STATE_REASON_ABORTED_BY_SYSTEM, SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT, 0, 0);
                break;
            default:
                smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_ABORTED, SMJOB_STATE_REASON_ABORTED_BY_SYSTEM, 0, 0, 0);
                break;
          }

          DBG_MSG("%s - aborted with event: %s\n", __func__, _get_job_event_str(new_event));
          DBG_MSG("setting reasons[0] to %s\n", 
                  _get_job_state_reason_str(sm_job->job_status->reasons[0]));

        session_ctxt->net_io_job_completed = true;
        break;
       }
    case NET_IO_JOB_EVENT_CANCELED:
        if (sm_job->cancelled)
        {
            smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_CANCELED, SMJOB_STATE_REASON_JOB_CANCELED_BY_USER, 0, 0, 0);
        }
        else
        {
            // this job has been canceled at device
            smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_CANCELED, SMJOB_STATE_REASON_JOB_CANCELED_AT_DEVICE, 0, 0, 0);
        }

        DBG_MSG("%s - with event: %s\n", __func__, _get_job_event_str(event));
        DBG_MSG("setting reasons[0] to %s\n", 
                _get_job_state_reason_str(sm_job->job_status->reasons[0]));

        session_ctxt->net_io_job_completed = true;
        break;
    case NET_IO_JOB_EVENT_SHEET_COMPLETED:
        sm_job->job_status->sheets_completed++;
        break;
    default:
        XASSERT( 0, event );
        break;
    }

    if (session_ctxt->net_io_job_completed && !session_ctxt->net_closed)
    {
        session_ctxt->net_closed = true;

        smjob_rcode_t status = pif_send_msg_wait_opt(SMJOB_PIF_MSG_CLOSE,
                                                     (void *)session_ctxt, 
                                                     POSIX_WAIT_FOREVER);
        if (status != SMJOB_OK)
        {
            DBG_ERR("ERROR %s - the call to pif_send_msg_wait_opt failed.\n", __func__);
        }
    }

    smjob_status_events_t smjob_event = 0;
    
    // Check if a change occurred
    if ( prev_smjob_status.state != sm_job->job_status->state )
    {
        smjob_event |= SMJOB_STATUS_EVENT_STATE;
    }
    if ( prev_smjob_status.num_reasons != sm_job->job_status->num_reasons )
    {
        smjob_event |= SMJOB_STATUS_EVENT_STATE_REASONS;
    }
    for ( i = 0; i < SMJOB_MAX_STATE_REASONS; i++ )
    {
        if ( prev_smjob_status.reasons[i] != sm_job->job_status->reasons[i] )
        {
            smjob_event |= SMJOB_STATUS_EVENT_STATE_REASONS;
        }
    }
    if ( prev_smjob_status.koctets_processed != sm_job->job_status->koctets_processed )
    {
        smjob_event |= SMJOB_STATUS_EVENT_KOCTETS_PROCESSED;
    }
    if ( prev_smjob_status.sheets_completed != sm_job->job_status->sheets_completed )
    {
        smjob_event |= SMJOB_STATUS_EVENT_SHEETS_COMPLETED;
    }

    // It's possible that the user didn't register a CB func in the smjob_create func
    if ( sm_job->job_status_func )
    {
        // Only call the status CB if an event of interest occured
        if ( 0 < (sm_job->job_event_mask & smjob_event ) )
        {
            DBG_MSG("%s - Calling the job_status_func callback (event %d)\n", __FUNCTION__, event);
            sm_job->job_status_func( job_id, smjob_event, sm_job->user_data );
        }
    }

}

//=============================================================================
// Interface function definitions
//=============================================================================

// The func called from smjob_mgr when it wants to start a print job.
smjob_rcode_t smjob_pif_start_job( smjob_t *cur_print_job )
{
    smjob_rcode_t sm_res;

    g_smjob_pif_ctxt_ptr->cur_print_job_info = cur_print_job;

    sm_res = pif_send_msg_wait_opt(SMJOB_PIF_MSG_PRINT_JOB, 
                                   g_smjob_pif_ctxt_ptr->cur_session, 
                                   0);

    return( sm_res );

}

// Determine whether the printer is busy or not. Set in the state machine.
bool smjob_pif_check_printer_busy()
{
    return( g_printer_busy );
}

// Used to the job ID of the current print job.
uint32_t smjob_pif_get_cur_print_job_id()
{
    if (g_smjob_pif_ctxt_ptr->cur_print_job_info)
    {
        ASSERT(g_smjob_pif_ctxt_ptr);
        ASSERT(g_smjob_pif_ctxt_ptr->cur_print_job_info);
        ASSERT(g_smjob_pif_ctxt_ptr->cur_print_job_info->job_status);
        return(g_smjob_pif_ctxt_ptr->cur_print_job_info->job_status->job_id);    
    }

    return(0);
}

// Get the current print job information.
void smjob_pif_get_cur_print_job_entry(smjob_t **sm_job)
{
    ASSERT(g_smjob_pif_ctxt_ptr->cur_print_job_info);
    *sm_job = g_smjob_pif_ctxt_ptr->cur_print_job_info;
}

// Used to get the completed print jobs. If the prev_comp_job is NULL, get
// the first completed print job. Else we get the next completed job after
// the prev_comp_job if it exists.
smjob_rcode_t smjob_pif_get_comp_print_job(smjob_status_t *prev_comp_job,
                                             smjob_status_t **job_status_ptr)
{
    uint32_t job_id_to_match = prev_comp_job->job_id;
    ATLISTENTRY *list_entry;
    smjob_t *sm_job;
    uint32_t px_ret_code;
    smjob_rcode_t job_id_found = FAIL;

    // Find the completed job that matches the job ID. Later this list and
    // functionality should be replaced by code that queries the print subsystem
    // for this information.
    if (!ATIsListEmpty(&g_completed_jobs_list.job_list))
    {
        px_ret_code = pthread_mutex_lock(&g_completed_jobs_list.list_mutex);
        XASSERT(px_ret_code == 0, px_ret_code);

        list_entry = ATListHead(&g_completed_jobs_list.job_list);

        while (list_entry)
        {
            sm_job = CONTAINING_RECORD(list_entry, struct smjob_s, listnode);
            ASSERT(sm_job);
    
            if (sm_job->job_status->job_id == job_id_to_match)
            {
                *job_status_ptr = sm_job->job_status;
                job_id_found = OK;
                break;
            }

            list_entry = ATListNext(&g_completed_jobs_list.job_list, list_entry);
        }

        px_ret_code = pthread_mutex_unlock(&g_completed_jobs_list.list_mutex);
        XASSERT(px_ret_code == 0, px_ret_code);
    }

    return( job_id_found );
}

// Send a message to cancel the current print job.
smjob_rcode_t smjob_pif_cancel_cur_print_job()
{
    smjob_rcode_t status;
    smjob_t *sm_job;
    
    sm_job = g_smjob_pif_ctxt_ptr->cur_print_job_info;
    if (sm_job != NULL)
    {
        // premature end of job
        sm_job->eof = true;
        sm_job->cancelled = true;
        
        smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_PROCESSING, SMJOB_STATE_REASON_PROCESSING_TO_STOP_POINT, SMJOB_STATE_REASON_JOB_CANCELED_BY_USER, 0, 0);
    }

    // Send the message to the print interface state machine to cancel the job.
    status = pif_send_msg_wait_opt(SMJOB_PIF_MSG_CANCEL_PRINT_JOB, 
                                   g_smjob_pif_ctxt_ptr->cur_session,
                                   0);
    return( status );
}

// Used to attach an observer to the print done.
void smjob_pif_AttachObserver_print_done( Observer_t *o )
{

    ATTACH_OBSERVER(g_smjob_pif_job_status.print_job_done_observers, o );
}

// A debug command to return a pointer to the requested list. 
// In this case it is only the completed list.
const ATLISTENTRY *smjob_pif_get_requested_list(smjob_pif_list_type_t req_list)
{
    switch ( req_list )
    {
        case COMPLETED_JOBS_LIST: return(&g_completed_jobs_list.job_list);
    }

    return( NULL );
}

// Get a job entry from the completed list if it exists.
smjob_rcode_t smjob_pif_get_job_entry(uint32_t job_id, smjob_t **ret_job_entry)
{
    bool job_id_in_list;
    
    // It was not the current job, look in the jobs_with_docs with docs list.
    job_id_in_list = smjob_mgr_check_list_for_job_id(&g_completed_jobs_list,
                                                      job_id, ret_job_entry);
    if ( job_id_in_list ) 
    {
        return( SMJOB_OK );
    }

    // The job was not found.
    return( SMJOB_FAIL );
}

// Return the job_id of the currently running job (if available)
uint32_t smjob_get_current_job_id(void)
{
    uint32_t job_id = SMJOB_INVALID_JOB_ID;
    smjob_t *sm_job;

    if (g_smjob_pif_ctxt_ptr != NULL)
    {
        sm_job = g_smjob_pif_ctxt_ptr->cur_print_job_info;
        if (sm_job != NULL)
        {
            job_id = sm_job->job_status->job_id;
        }
    }

    return job_id;
}

#ifdef DEBUG
const char *_get_job_event_str(net_io_job_event_t event)
{
#define _CASE(x) case x: return #x

   switch (event)
   {
      _CASE(NET_IO_JOB_EVENT_STARTED);
      _CASE(NET_IO_JOB_EVENT_PAUSED);
      _CASE(NET_IO_JOB_EVENT_COMPLETED);
      _CASE(NET_IO_JOB_EVENT_ABORTED);
      _CASE(NET_IO_JOB_EVENT_CANCELED);
      _CASE(NET_IO_JOB_EVENT_SHEET_COMPLETED);

   default: return(NULL);
   }
#undef _CASE
}
#endif
#ifdef DEBUG
const char *_get_job_state_reason_str(smjob_state_reason_t reason)
{
#define _CASE(x) case x: return #x

   switch (reason)
   {
      _CASE(SMJOB_STATE_REASON_UNKNOWN);
      _CASE(SMJOB_STATE_REASON_NONE);
      _CASE(SMJOB_STATE_REASON_JOB_INCOMING);
      _CASE(SMJOB_STATE_REASON_JOB_DATA_INSUFFICIENT);
      _CASE(SMJOB_STATE_REASON_DOC_ACCESS_ERROR);
      _CASE(SMJOB_STATE_REASON_DOC_TRANSFER_ERROR);
      _CASE(SMJOB_STATE_REASON_JOB_OUTGOING);
      _CASE(SMJOB_STATE_REASON_JOB_HOLD_UNTIL_SPECIFIED);
      _CASE(SMJOB_STATE_REASON_RESOURCES_ARE_NOT_READY);
      _CASE(SMJOB_STATE_REASON_PRINTER_STOPPED_PARTLY);
      _CASE(SMJOB_STATE_REASON_PRINTER_STOPPED);
      _CASE(SMJOB_STATE_REASON_JOB_INTERPRETING);
      _CASE(SMJOB_STATE_REASON_JOB_QUEUED);
      _CASE(SMJOB_STATE_REASON_JOB_TRANSFORMING);
      _CASE(SMJOB_STATE_REASON_JOB_QUEUED_FOR_MARKER);
      _CASE(SMJOB_STATE_REASON_JOB_PRINTING);
      _CASE(SMJOB_STATE_REASON_JOB_CANCELED_BY_USER);
      _CASE(SMJOB_STATE_REASON_JOB_CANCELED_BY_OPERATOR);
      _CASE(SMJOB_STATE_REASON_JOB_CANCELED_AT_DEVICE);
      _CASE(SMJOB_STATE_REASON_ABORTED_BY_SYSTEM);
      _CASE(SMJOB_STATE_REASON_UNSUPPORTED_COMPRESSION);
      _CASE(SMJOB_STATE_REASON_COMPRESSION_ERROR);
      _CASE(SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT);
      _CASE(SMJOB_STATE_REASON_DOC_FORMAT_ERROR);
      _CASE(SMJOB_STATE_REASON_PROCESSING_TO_STOP_POINT);
      _CASE(SMJOB_STATE_REASON_SERVICE_OFF_LINE);
      _CASE(SMJOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY);
      _CASE(SMJOB_STATE_REASON_JOB_COMPLETED_WITH_WARNINGS);
      _CASE(SMJOB_STATE_REASON_JOB_COMPLETED_WITH_ERRORS);
      _CASE(SMJOB_STATE_REASON_JOB_RESTARTABLE);
      _CASE(SMJOB_STATE_REASON_QUEUE_IN_DEVICE);
      _CASE(SMJOB_STATE_REASON_DOC_TIMEOUT_ERROR);
      _CASE(SMJOB_STATE_REASON_JOB_PASSWORD_WAIT);
         
   default: return(NULL);
   }

#undef _CASE
}
#endif
