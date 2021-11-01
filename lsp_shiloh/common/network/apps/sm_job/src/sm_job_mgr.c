/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * @file sm_job_mgr.c
 *
 * @brief This file manages the jobs for smjob, interfacing between sm_job_core
 * and the print interface (sm_job_pif).
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ATypes.h"
#include "lassert.h"
#include "error_types.h"
#include "list.h"
#include "memAPI.h"
#include "logger.h"
#include "event_observer.h"
#include "debug.h"
#include "net_logger.h"
#include "sm_job_api.h"
#include "sm_job_internal_api.h"

//=============================================================================
// Structures
//=============================================================================

typedef struct smjob_mgr_ctxt_s
{
    const char      *name;
    pthread_t       thread;
    uint8_t         *thread_stack;

} smjob_mgr_ctxt_t;

typedef struct smjob_mgr_observer_s
{
    Observer_t *smjob_pif_job_done_observer;

} smjob_mgr_observer_t;

//=============================================================================
// Local Variables
//=============================================================================

// The ultimo job list.
static smjob_stack_t g_smjob_stack;

// The next_doc_id counter
static uint32_t g_next_doc_id = INITIAL_DOC_ID_VALUE;

// The job lists.
//
// The jobs_with_docs list holds those jobs which are ready to be printed, i.e. 
// have documents.
//
// The g_jobs_wo_docs list holds jobs that have been created but still need documents.
// The jobs in this list will be moved to the g_jobs_w_docs list once docs are added.
// The g_jobs_held list contains those jobs that the user has chosen to be held.
//
static smjob_list_t g_jobs_w_docs;
static smjob_list_t g_jobs_wo_docs;
static smjob_list_t g_jobs_held;

// The job manager context pointer, observer, and flags
static smjob_mgr_ctxt_t *g_loc_smjob_mgr_ctxt_ptr = NULL;
static smjob_mgr_observer_t g_smjob_mgr_observer;
//sem_t           g_smjob_mgr_event_semaphore;
sem_t           g_smjob_mgr_tosend_semaphore;
sem_t           g_smjob_mgr_pjdone_semaphore;

// Flag used to turn off printing.
#ifdef DEBUG
static bool g_printing_turned_off = false;
#endif

//=============================================================================
// Defines
//=============================================================================

 // The manage jobs stack size.
#define SMJOB_MGR_STACK_SIZE POSIX_MIN_STACK_SIZE


// Debug prints
#define DBG_PRFX "SMJOBMGR: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_SM_JOB

//// TODO TEMP release debug
//#define DBG_ERR(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_MSG(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) dbg_printf(DBG_PRFX __VA_ARGS__)

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)


// The observer ID for this module.
#define SMJOB_MGR_JOB_DONE_OB_ID 0x736D6D6A

// For the event flags
#define SMJOB_MGR_EVENT_PRINT_JOB_DONE    0x1
#define SMJOB_MGR_EVENT_PRINT_JOB_TO_SEND 0x2

//=============================================================================
// Parameterized Macros
//=============================================================================
#define ADD_JOB_TO_LIST(sm_job, mgr_list)                                          \
do                                                                                \
{                                                                                 \
       uint32_t px_ret_code = pthread_mutex_lock(&mgr_list.list_mutex); \
       XASSERT(px_ret_code == 0, px_ret_code);                           \
       ATInsertTailList(&mgr_list.job_list, &sm_job->listnode);                    \
       px_ret_code = pthread_mutex_unlock(&mgr_list.list_mutex);                           \
       XASSERT(px_ret_code == 0, px_ret_code);                           \
}while (0)

#define GET_JOB_FROM_PRINT_LIST(node_ptr)                                             \
do                                                                                    \
{                                                                                     \
    uint32_t px_ret_code = pthread_mutex_lock(&g_jobs_w_docs.list_mutex);    \
    XASSERT(px_ret_code == 0, px_ret_code);                                  \
    node_ptr = NULL;                                                                  \
    if(!ATIsListEmpty(&g_jobs_w_docs.job_list))                                         \
    {                                                                                 \
        ATLISTENTRY *list_node_ptr = ATRemoveHeadList(&g_jobs_w_docs.job_list);         \
        node_ptr = CONTAINING_RECORD(list_node_ptr, struct smjob_s, listnode);       \
    }                                                                                 \
    px_ret_code = pthread_mutex_unlock(&g_jobs_w_docs.list_mutex);                              \
    XASSERT(px_ret_code == 0, px_ret_code);                                  \
} while (0)

#define PUT_JOB_AT_HEAD_PRINT_LIST(sm_job)                                            \
do                                                                                    \
{                                                                                     \
       uint32_t px_ret_code = pthread_mutex_lock(&g_jobs_w_docs.list_mutex); \
       XASSERT(px_ret_code == 0, px_ret_code);                               \
       ATInsertHeadList(&g_jobs_w_docs.job_list, &sm_job->listnode);                    \
       px_ret_code = pthread_mutex_unlock(&g_jobs_w_docs.list_mutex);                           \
       XASSERT(px_ret_code == 0, px_ret_code);                               \
}while (0)

#define GET_LIST_HEAD_JOB_STATUS(mgr_list, ret_job_status)             \
do                                                                    \
{                                                                     \
    uint32_t    px_ret_code;                                          \
    ATLISTENTRY *list_entry;                                          \
    smjob_t *sm_job;                                           \
    px_ret_code = pthread_mutex_lock(&mgr_list.list_mutex); \
    XASSERT(px_ret_code == 0, px_ret_code);                  \
    list_entry = ATListHead(&mgr_list.job_list);                       \
    XASSERT(list_entry != NULL, (uint32_t)list_entry);                \
    sm_job = CONTAINING_RECORD(list_entry, struct smjob_s, listnode);\
    XASSERT(sm_job != NULL, (uint32_t)sm_job);                        \
    *ret_job_status  = sm_job->job_status;                            \
    px_ret_code = pthread_mutex_unlock(&mgr_list.list_mutex);                  \
    XASSERT(px_ret_code == 0, px_ret_code);                  \
} while (0);

//=============================================================================
// Local function declarations
//=============================================================================
static void job_done_notify(Observer_t *observer, Subject_t *subject);
static void init_job_lists();
static void create_smjob_mgr_access_protection();
static void init_g_smjob_stack();
static void* smjob_mgr_loop(void* unused);
static smjob_rcode_t move_job_to_another_list( smjob_list_t *from_list_ptr,
                                                smjob_list_t *to_list_ptr,
                                                uint32_t job_id );
static bool get_next_job_status_from_list(smjob_list_t   *mgr_list_ptr,
                                          uint32_t job_id_to_match,
                                          smjob_status_t **job_status_ptr,
                                          bool *get_head_next_list);
static smjob_rcode_t get_next_active_job(smjob_status_t **job_status_ptr,
                                         bool check_cur_print_job);
static bool remove_job_from_list(smjob_list_t *mgr_list_ptr, uint32_t job_id);
static smjob_rcode_t mark_job_as_cancelled_in_stack(uint32_t cur_job_id);
static bool smjob_mgr_free_index(uint32_t *new_index, uint32_t start_index);

//=============================================================================
// Interface functions
//=============================================================================

//-----------------------------------------------------------------------------------
void smjob_mgr_init( const char *name )
{
    uint32_t pthread_rcode;

    // Initialize the g_smjob_stack
    init_g_smjob_stack();

    // Wrappers to create the access protection for the lists and to 
    // initialize the lists themselves
    create_smjob_mgr_access_protection();
    init_job_lists();

    // Create the mgr context
    g_loc_smjob_mgr_ctxt_ptr = (smjob_mgr_ctxt_t *)MEM_MALLOC(sizeof(smjob_mgr_ctxt_t));
    ASSERT(g_loc_smjob_mgr_ctxt_ptr);
       
    g_loc_smjob_mgr_ctxt_ptr->name = name;
    g_loc_smjob_mgr_ctxt_ptr->thread_stack = NULL;

    // Construct a job done observer.
    memset(&g_smjob_mgr_observer, 0, sizeof(smjob_mgr_observer_t));
    g_smjob_mgr_observer.smjob_pif_job_done_observer = 
       Observer_Constructor(SMJOB_MGR_JOB_DONE_OB_ID, job_done_notify);

    ASSERT(g_smjob_mgr_observer.smjob_pif_job_done_observer);

    // Create the mgr flags
    pthread_rcode = sem_init( &g_smjob_mgr_tosend_semaphore, 0, 0 );
    if (pthread_rcode != 0)
    {
        DBG_PRINTF_ERR("%s - failed to create event flags (return value = %#x)\n", __func__, pthread_rcode);
        ASSERT(0);
    }
    pthread_rcode = sem_init( &g_smjob_mgr_pjdone_semaphore, 0, 0 );
    if (pthread_rcode != 0)
    {
        DBG_PRINTF_ERR("%s - failed to create event flags (return value = %#x)\n", __func__, pthread_rcode);
        ASSERT(0);
    }

    // Start the thread that will watch for new jobs and start them as appropriate.
    g_loc_smjob_mgr_ctxt_ptr->thread_stack = MEM_MALLOC(SMJOB_MGR_STACK_SIZE);
    ASSERT(g_loc_smjob_mgr_ctxt_ptr->thread_stack);

    pthread_rcode = posix_create_thread(&g_loc_smjob_mgr_ctxt_ptr->thread, smjob_mgr_loop,
                                     (void *)g_loc_smjob_mgr_ctxt_ptr, (CHAR *)name,  
                                     g_loc_smjob_mgr_ctxt_ptr->thread_stack, 
                                     SMJOB_MGR_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if(pthread_rcode != 0)
    {
        DBG_ERR("%s - error creating thread %s (return value = %#x)\n", __func__,
                name, pthread_rcode);
        ASSERT(0);
    }

}

//-----------------------------------------------------------------------------------
smjob_rcode_t smjob_mgr_doc_lookup_stack(uint32_t doc_id, smjob_doc_t **sm_doc)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_t *sm_job;
    smjob_doc_t *cur_sm_doc;
    smjob_doc_t *found_sm_doc;
    uint32_t index;
    uint32_t iterations;
    uint32_t px_ret_code;

    // Check inputs
    ASSERT(sm_doc);
    XASSERT(doc_id >= INITIAL_DOC_ID_VALUE && doc_id <= MAX_DOC_ID_VALUE,  doc_id);

    // Lock access first
    px_ret_code = pthread_mutex_lock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    // Traverse the g_smjob_stack to find a matching doc_id
    // For each job, there could be multiple docs
    index = g_smjob_stack.first_valid_job_index;
    found_sm_doc = NULL;
    cur_sm_doc = NULL;
    iterations = 0;
    while ( ( SMJOB_HISTORY_MAX_ENTRIES >= iterations ) & ( NULL == found_sm_doc ) )
    {
        // Get the initial doc in this job entry
        sm_job = g_smjob_stack.job_entry_list[index];
        if ( NULL != sm_job )
        {
            cur_sm_doc = sm_job->sm_doc;
        }

        // Search for the doc_id
        while ( ( NULL == found_sm_doc ) & ( NULL != cur_sm_doc ) )
        {
            if ( doc_id == cur_sm_doc->doc_status->doc_id )
            {
                found_sm_doc = cur_sm_doc;
            }
            else
            {
                cur_sm_doc = cur_sm_doc->next;
            }
        }

        // Get next job entry
        if ( SMJOB_HISTORY_MAX_ENTRIES <= ( index++ ) )
        {
            index = 0;
        }

        // Since the job stack is circular, make sure we don't traverse more than once
        iterations++;

    }

    if ( found_sm_doc )
    {
        *sm_doc = found_sm_doc;
    }
    else
    {
        *sm_doc = NULL;
        sm_res = SMJOB_FAIL;
    }

    // Unlock access
    px_ret_code = pthread_mutex_unlock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    return sm_res;
}

//-----------------------------------------------------------------------------------
smjob_rcode_t smjob_mgr_job_lookup_stack(uint32_t job_id, smjob_t **ret_job_entry)
{
    return smjob_mgr_job_lookup_stack_ctxt(job_id, ret_job_entry, NULL);
}

// rdj 3/2013: transitioning to new smjob_ctxt_t
smjob_rcode_t smjob_mgr_job_lookup_stack_ctxt(uint32_t job_id, smjob_t **ret_job_entry, smjob_ctxt_t **ret_job_ctxt)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    uint32_t index;
    uint32_t px_ret_code;

    // Check inputs
    if ( 0 == ret_job_entry && 0 == ret_job_ctxt) 
    {
        DBG_ERR("%s: Invalid ret_job_entry pointer provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }
    if ( ( SMJOB_INVALID_JOB_ID != job_id) && 
         ( ( INITIAL_JOB_ID_VALUE > job_id ) || ( MAX_JOB_ID_VALUE < job_id ) ) )
    {
        DBG_ERR("%s: Invalid job_id provided, out of range \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Lock access first
    px_ret_code = pthread_mutex_lock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    // Determine if the first valid job was requested 
    if ( SMJOB_INVALID_JOB_ID == job_id )
    {
        index = g_smjob_stack.first_valid_job_index;
    }
    else
    {
        index = job_id % SMJOB_HISTORY_MAX_ENTRIES;
    }

    ASSERT(!g_smjob_stack.job_entry_list[index] || !g_smjob_stack.jobs[index]); // may be in one slot or the other but not both 
    if(ret_job_entry)
    {
        // Find the job entry in the deprecated g_smjob_stack
        *ret_job_entry = g_smjob_stack.job_entry_list[index];
        if( *ret_job_entry && job_id != SMJOB_INVALID_JOB_ID && 
            (*ret_job_entry)->job_status->job_id != job_id )
        {
            *ret_job_entry = NULL;
        }
    }
    if(ret_job_ctxt)
    {
        *ret_job_ctxt = g_smjob_stack.jobs[index];
        if( *ret_job_ctxt && job_id != SMJOB_INVALID_JOB_ID && 
            (*ret_job_ctxt)->job_id != job_id )
        {
            *ret_job_ctxt = NULL;
        }
    }

    if( !((ret_job_entry && *ret_job_entry) || (ret_job_ctxt && *ret_job_ctxt)) )
    {
        // job not found
        sm_res = SMJOB_FAIL;
    }

    // Unlock access
    px_ret_code = pthread_mutex_unlock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    return sm_res;
}

//-----------------------------------------------------------------------------------
bool smjob_mgr_free_index(uint32_t *new_index, uint32_t start_index)
{
   smjob_state_t cur_state;
   bool          found_index = false;
   uint32_t      next_entry_index;
   uint32_t      num_indices_checked = 0;

   // Only check the indices we have.
   while (num_indices_checked < SMJOB_HISTORY_MAX_ENTRIES)
   {
      // The next entry to check will be where we were told to start plus what
      // we have checked already, modulo the array size.
      next_entry_index = (start_index + num_indices_checked) % SMJOB_HISTORY_MAX_ENTRIES;

      if (g_smjob_stack.job_entry_list[next_entry_index] != NULL)
      {
          // Get the state, this is what we care about.
          cur_state = g_smjob_stack.job_entry_list[next_entry_index]->job_status->state;

          // If the state is either completed, cancelled or aborted, we found the next
          // index.
          if (cur_state == SMJOB_STATE_COMPLETED || cur_state == SMJOB_STATE_ABORTED ||
              cur_state == SMJOB_STATE_CANCELED)
          {
             found_index = true;
             *new_index  = next_entry_index;
             break;
          }
      }
      else
      {
         found_index = true;
         *new_index  = next_entry_index;
         break;
      }
      
      // Nothing found yet.
      ++num_indices_checked;
   }

   return(found_index);
}

//-----------------------------------------------------------------------------------
smjob_rcode_t smjob_mgr_insert_job_in_stack( smjob_t *sm_job, smjob_ctxt_t *job )
{
    smjob_rcode_t sm_res = SMJOB_OK;
    uint32_t update_next_job_id;
    uint32_t index;
    uint32_t px_ret_code;
    
    // Check inputs
    if ( 0 == sm_job && 0 == job)
    {
        DBG_ERR("%s: Invalid sm_job pointer provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Lock access first
    px_ret_code = pthread_mutex_lock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    // Add the job id to the job
    if(sm_job)
    {
        sm_job->job_status->job_id = g_smjob_stack.next_job_id;
    }
    else
    {
        ASSERT(job);
        job->job_id = g_smjob_stack.next_job_id;
    }

    // Check that the stack isn't full first.
    // If it is full, get an index where the job has been cancelled, aborted, or
    // completed. We will let the code later on do the cleanup of the entry.
    index = g_smjob_stack.next_job_id % SMJOB_HISTORY_MAX_ENTRIES;
    if ( ( index == g_smjob_stack.first_valid_job_index ) && 
         ( NULL != g_smjob_stack.job_entry_list[index] ) )
    {
       bool free_index_avail;

       free_index_avail = smjob_mgr_free_index(&index, g_smjob_stack.first_valid_job_index);

       // We really are full.
       if (!free_index_avail)
       {
          DBG_ERR("%s: g_smjob_stack is full; can't accept more jobs \n", __func__);
          sm_res = SMJOB_FAIL;
	      px_ret_code = pthread_mutex_unlock(&g_smjob_stack.stack_mutex);
          XASSERT(px_ret_code == 0, px_ret_code);
          return sm_res;
       }

       // The first valid index has to move forward. Wrap around if needed.
       g_smjob_stack.first_valid_job_index += 1;

       if (g_smjob_stack.first_valid_job_index >= SMJOB_HISTORY_MAX_ENTRIES)
       {
          g_smjob_stack.first_valid_job_index = 0;
       }
    }

    if(sm_job)
    {
        // Add the job to the stack at the next available position
        if ( 0 != g_smjob_stack.job_entry_list[index] )
        {
            // Spot is not emtpy; free the job at this index first
            // Ideally, this shouldn't happen, as jobs should be cleaned up
            // when completed/aborted/cancelled/timed-out.
            smjob_mgr_cleanup_job( g_smjob_stack.job_entry_list[index] );
        }
        g_smjob_stack.job_entry_list[index] = sm_job;
    }
    else
    {
        g_smjob_stack.jobs[index] = job;
    }

    // Update next_job_id
    update_next_job_id = g_smjob_stack.next_job_id + 1;
    if ( ( MAX_JOB_ID_VALUE ) < update_next_job_id)
    {
        DBG_PRINTF_DEBUG("%s: Rollover to zero for next_job_id \n", __func__);
        update_next_job_id = INITIAL_JOB_ID_VALUE;
    }
    g_smjob_stack.next_job_id = update_next_job_id;

    // Unlock access
    px_ret_code = pthread_mutex_unlock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    return sm_res;
}

//-----------------------------------------------------------------------------------
smjob_rcode_t smjob_mgr_remove_from_stack( uint32_t smjob_id )
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_t *sm_job;
    uint32_t px_ret_code;
    uint32_t next_index;
    uint32_t index;

    // Check inputs
    if ( 0 == smjob_id )
    {
        DBG_ERR("%s: Invalid smjob_id provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Lock access
    px_ret_code = pthread_mutex_lock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    // Find the job matching the id
    sm_res = smjob_mgr_job_lookup_stack(smjob_id, &sm_job);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, smjob_id);
        return sm_res;
    }
    index = smjob_id % SMJOB_HISTORY_MAX_ENTRIES;

    // Adjust first_valid_job_index if necessary
    if ( g_smjob_stack.first_valid_job_index == index )
    {
        // Skip over any gaps in the stack
        next_index = g_smjob_stack.first_valid_job_index + 1;
        if ( SMJOB_HISTORY_MAX_ENTRIES <= next_index )
        {
            next_index = 0;
        }
        while ( ( NULL == g_smjob_stack.job_entry_list[next_index] ) && 
                ( next_index != g_smjob_stack.first_valid_job_index ) )
        {
            // Increment the next_index; remember that the stack is circular
            if ( SMJOB_HISTORY_MAX_ENTRIES == ( next_index + 1 ) )
            {
                next_index = 0;
            }
            else
            {
                next_index++;
            }
        }

        // It could be that there are no other valid jobs in stack and it will be empty
        // In this case, set the tail (first_valid_job_index) to what the next valid job
        // will be
        if ( next_index == g_smjob_stack.first_valid_job_index )
        {
            if ( SMJOB_HISTORY_MAX_ENTRIES <= ( next_index + 1 ) )
            {
                g_smjob_stack.first_valid_job_index = (INITIAL_JOB_ID_VALUE %
                                                       SMJOB_HISTORY_MAX_ENTRIES);
            }
            else
            {
                g_smjob_stack.first_valid_job_index = (g_smjob_stack.next_job_id %
                                                       SMJOB_HISTORY_MAX_ENTRIES);
            }
        }
        else
        {
            // Found the next job in stack
            g_smjob_stack.first_valid_job_index = next_index;
        }
    }

    // Free the job entry memory
    smjob_mgr_cleanup_job( sm_job );
    g_smjob_stack.job_entry_list[index] = NULL;

    // Unlock access
    px_ret_code = pthread_mutex_unlock(&g_smjob_stack.stack_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    return sm_res;
}

//-----------------------------------------------------------------------------------
// Add a job to the appropriate list, depending on if a doc is present or not
smjob_rcode_t smjob_mgr_insert_job_in_list( smjob_t *sm_job )
{
    sm_job->job_status->koctets_processed = 0;
    sm_job->job_status->sheets_completed = 0;
#ifndef NET_SM_STREAMING
    sm_job->bytes_read = 0;
#endif

    if ( NULL != sm_job->sm_doc )
    {
        error_type_t e_res;

        smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_PENDING, SMJOB_STATE_REASON_JOB_QUEUED, 0, 0, 0);

        ADD_JOB_TO_LIST(sm_job, g_jobs_w_docs);

        e_res = sem_post( &g_smjob_mgr_tosend_semaphore );
        ASSERT( e_res == 0 );
    }
    else
    {
        smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_PENDING_HELD, SMJOB_STATE_REASON_JOB_INCOMING, 0, 0, 0);
        
        ADD_JOB_TO_LIST(sm_job, g_jobs_wo_docs);
    }


    return( SMJOB_OK );
}

//-----------------------------------------------------------------------------------
// Get a specified job entry if it exists.
smjob_rcode_t smjob_mgr_get_job_entry(uint32_t job_id,
                                        smjob_t **ret_job_entry)
{
    bool job_id_in_list;

    // Check whether the current print job is the one requested.
    // If so return the information on the job.
    if (smjob_pif_get_cur_print_job_id() == job_id)
    {
        smjob_pif_get_cur_print_job_entry(ret_job_entry);
        return( SMJOB_OK );
    }
    
    // It was not the current job, look in the g_jobs_w_docs with
    // docs list.
    job_id_in_list = smjob_mgr_check_list_for_job_id(&g_jobs_w_docs,
                                                      job_id,
                                                      ret_job_entry);
    if ( TRUE == job_id_in_list )
    {
        return( SMJOB_OK );
    }

    // Now check the g_jobs_wo_docs list.
    job_id_in_list = smjob_mgr_check_list_for_job_id(&g_jobs_wo_docs, 
                                                      job_id,
                                                      ret_job_entry);
    if ( TRUE == job_id_in_list )
    {
        return( SMJOB_OK );
    }

    // Finally, check the g_jobs_held list.
    job_id_in_list = smjob_mgr_check_list_for_job_id(&g_jobs_held, 
                                                      job_id,
                                                      ret_job_entry);
    if ( TRUE == job_id_in_list ) 
    {
        return( SMJOB_OK );
    }

    // The job was not found.
    return( SMJOB_FAIL );
}

//-----------------------------------------------------------------------------------
// Get the next active job.
smjob_rcode_t smjob_mgr_get_next_active_job(smjob_status_t *prev_job_stat_ptr,
                                              smjob_status_t **next_job_stat_ptr)
{
    smjob_rcode_t status = SMJOB_OK;
    bool           check_cur_print_job;
    bool           found_job_id;
    bool           get_head_next_list;

    // If the previous job pointer is NULL, just look at the current job.
    if (!prev_job_stat_ptr)
    {
        check_cur_print_job = TRUE;

        status = get_next_active_job(next_job_stat_ptr, check_cur_print_job);
    }
    else
    {
        // Check to see if the current print job ID is the same as the previous
        // job ID.
        if (smjob_pif_get_cur_print_job_id() == prev_job_stat_ptr->job_id)
        {
            // Make the call to get the next active job. Set the flag that the routine
            // is not to check the current print job as that has already been done here.
            check_cur_print_job = FALSE;
            status = get_next_active_job(next_job_stat_ptr, check_cur_print_job);
        }
        // Start to check the various job lists for the next active job following the
        // previous job that was passed in.
        else
        {
            found_job_id = get_next_job_status_from_list(&g_jobs_w_docs,
                                                         prev_job_stat_ptr->job_id, 
                                                         next_job_stat_ptr,
                                                         &get_head_next_list);
            if (!found_job_id)
            {
                // See if we are to get the head of the next list. This occurs when the
                // job ID of the last entry matches the job ID we are looking for.
                if (get_head_next_list)
                {
                    ATLISTENTRY *list_entry;

                    list_entry = ATListHead(&g_jobs_held.job_list);
                    if (list_entry)
                    {
                        GET_LIST_HEAD_JOB_STATUS(g_jobs_held, 
                                                 next_job_stat_ptr);
                    }
                    else
                    {
                        status = SMJOB_FAIL;
                    }
                }
                // Nope, search this list for the job ID and return the next status.
                else
                {
                    found_job_id = get_next_job_status_from_list(&g_jobs_held,
                                                                 prev_job_stat_ptr->job_id, 
                                                                 next_job_stat_ptr,
                                                                 &get_head_next_list);
                    if (!found_job_id)
                    {
                        status = SMJOB_FAIL;
                    }
                }
            }
        }
    }

    return( status );
}

smjob_rcode_t mark_job_as_cancelled_in_stack(uint32_t job_id)
{
    smjob_rcode_t sm_res;
    smjob_t       *sm_job = NULL;

    sm_res = smjob_mgr_job_lookup_stack(job_id, &sm_job);
    if (sm_res != SMJOB_OK)
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return sm_res;
    }

    smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_CANCELED, SMJOB_STATE_REASON_JOB_CANCELED_BY_USER, 0, 0, 0);

    return(SMJOB_OK);
}


//-----------------------------------------------------------------------------------
// Find a particular job in the lists and remove it. Also mark it as cancelled in the
// stack.
smjob_rcode_t smjob_mgr_cancel_job(uint32_t cur_job_id)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    uint32_t       job_id;
    bool           found_job_id = TRUE;

    // Start looking for the job ID, is it the current print job?
    job_id = smjob_pif_get_cur_print_job_id();

    if (job_id == cur_job_id)
    {
        sm_res = smjob_pif_cancel_cur_print_job();
    }
    // It is not the current print job, start looking through the various lists.
    else
    {
        found_job_id = remove_job_from_list(&g_jobs_w_docs, cur_job_id);
        if ( !found_job_id )
        {
            found_job_id = remove_job_from_list(&g_jobs_held, cur_job_id);
            if ( !found_job_id )
            {
                found_job_id = remove_job_from_list(&g_jobs_wo_docs, cur_job_id);
                if ( !found_job_id )
                {
                    // The job was not found in any list.
                    DBG_ERR("%s: The job is not found in any list. \n", __func__);
                    sm_res = SMJOB_FAIL;
                }
            }
        }
        
        if (found_job_id)
        {
            sm_res = mark_job_as_cancelled_in_stack(cur_job_id);
        }
    }

// rdj 24-apr-2013  Do not remove from stack. This obliterates the job and makes it
// impossible to retrieve job history, including querying for job status immediately
// following cancel. More importantly, if job is current job then we would free the
// context when it is still being used resulting in mem corruption.
//    // Remove from the job stack
//    sm_res = smjob_mgr_remove_from_stack( cur_job_id );
//    if ( SMJOB_FAIL == sm_res )
//    {
//        DBG_ERR("%s: The job is not found in the job history stack. \n", __func__);
//    }

    // Fail if either not in any list nor the job history stack
//  if ( !found_job_id )
//  {
//      sm_res = SMJOB_FAIL;
//  }
    
    return( sm_res );
}

//-----------------------------------------------------------------------------------
// Add a document to a job and move the job to the proper list.
smjob_rcode_t smjob_mgr_add_document(smjob_t *sm_job,
                                       bool last_doc, 
                                       smjob_doc_t *sm_doc)
{
    smjob_rcode_t  sm_res;
    smjob_doc_t *next_sm_doc;
    smjob_doc_t *last_sm_doc;
    error_type_t e_res;

    // Check the inputs
    if ( ( !sm_job ) | ( !sm_doc ) )
    {
        DBG_ERR("%s: Invalid sm_job or sm_doc pointer provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }
    
    // Add the doc_id
    sm_doc->doc_status->doc_id = g_next_doc_id;
    if ( MAX_DOC_ID_VALUE < ( g_next_doc_id + 1 ) )
    {
        g_next_doc_id = INITIAL_DOC_ID_VALUE;
    }
    else
    {
        g_next_doc_id++;
    }
    DBG_ERR("%s: doc_id = %d, g_next_doc_id = %d \n", __func__, 
            sm_doc->doc_status->doc_id, g_next_doc_id);
    
    // Attach the sm_doc to the job
    if ( NULL == sm_job->sm_doc )
    {
        // There are no docs yet attached to this job
        sm_job->sm_doc = sm_doc;
    }
    else
    {
        // There are docs attached, find end of list of docs
        next_sm_doc = sm_job->sm_doc->next;
        last_sm_doc = NULL;
        while ( NULL != next_sm_doc )
        {
            last_sm_doc = next_sm_doc;
            next_sm_doc = next_sm_doc->next;
        }
        // Then add the new doc handle to the end of the list
        last_sm_doc->next = sm_doc;
    }

    // We have just received a document for a job that should be in the g_jobs_wo_docs
    // list. Move it to the g_jobs_w_docs list.
    sm_res = move_job_to_another_list( &g_jobs_wo_docs,
                                       &g_jobs_w_docs,
                                       sm_job->job_status->job_id );

    // Increment the doc count in job_status
    sm_job->job_status->num_docs++;

    // Wave a mgr event flag if done adding docs to this job
    if ( TRUE == last_doc )
    {
        smjob_mgr_set_job_state(sm_job->job_status, SMJOB_STATE_PENDING, SMJOB_STATE_REASON_JOB_QUEUED, 0, 0, 0);
        e_res = sem_post( &g_smjob_mgr_tosend_semaphore );
        ASSERT( e_res == 0 );
    }

    return( sm_res );
}

//-----------------------------------------------------------------------------------
const ATLISTENTRY *smjob_mgr_get_requested_list( smjob_mgr_list_type_t req_list )
{
    switch ( req_list )
    {
        case JOBS_W_DOCS_LIST: 
        {
            return( &g_jobs_w_docs.job_list );
        }
        case JOBS_WO_DOCS_LIST:   
        {
            return( &g_jobs_wo_docs.job_list );
        }
        case JOBS_HELD_LIST:      
        {
            return( &g_jobs_held.job_list );
        }
    }

    return( NULL );
}

//-----------------------------------------------------------------------------------
// Check whether a particular job is in a list.
// Used in this module and by the print module.
bool smjob_mgr_check_list_for_job_id(smjob_list_t *mgr_list_ptr,
                                      uint32_t job_id,
                                      smjob_t **ret_job_entry)
{
    ATLISTENTRY     *list_entry;
    smjob_t *sm_job;
    uint32_t        px_ret_code;
    bool            job_id_found = false;

    px_ret_code = pthread_mutex_lock(&mgr_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    list_entry = ATListHead(&mgr_list_ptr->job_list);

    while (list_entry)
    {
        sm_job = CONTAINING_RECORD(list_entry, struct smjob_s, listnode);

        if (sm_job->job_status->job_id == job_id)
        {
            job_id_found   = true;
            *ret_job_entry = sm_job;
            break;
        }

        list_entry = ATListNext(&mgr_list_ptr->job_list, list_entry);
    }

    px_ret_code = pthread_mutex_unlock(&mgr_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);
    
    return(job_id_found);
}

//-----------------------------------------------------------------------------------
// Cleanup the resources from a job entry
void smjob_mgr_cleanup_job(smjob_t *sm_job)
{
    smjob_doc_t *next_doc;
    smjob_doc_t *doc_to_free;

    if ( sm_job )
    {
        MEM_FREE_AND_NULL(sm_job->job_ticket);
        MEM_FREE_AND_NULL(sm_job->job_status);

        if ( sm_job->sm_doc )
        {
            next_doc = sm_job->sm_doc->next;
            while ( NULL != next_doc )
            {
                doc_to_free = next_doc;
                next_doc = next_doc->next;
                MEM_FREE_AND_NULL(doc_to_free->doc_status);
                MEM_FREE_AND_NULL(doc_to_free->doc_ticket);
                MEM_FREE_AND_NULL(doc_to_free);
            }
            MEM_FREE_AND_NULL(sm_job->sm_doc->doc_status);
            MEM_FREE_AND_NULL(sm_job->sm_doc->doc_ticket);
            MEM_FREE_AND_NULL(sm_job->sm_doc);
#ifndef NET_SM_STREAMING
        smjob_pif_remove_file_if_exists(sm_job->sm_doc->doc_ticket->doc_description.doc_name);
#endif
        }

        MEM_FREE_AND_NULL(sm_job);
    }
}

uint32_t smjob_mgr_get_queued_job_count()
{
   smjob_state_t cur_state;
   uint32_t      jobs_queued = 0;
   uint32_t      next_entry_index;
   uint32_t      num_indices_checked = 0;

   // Only check the indices we have.
   while (num_indices_checked < SMJOB_HISTORY_MAX_ENTRIES)
   {
      // The next entry to check, modulo the array size.
      next_entry_index = num_indices_checked % SMJOB_HISTORY_MAX_ENTRIES;

      if (g_smjob_stack.job_entry_list[next_entry_index] != NULL)
      {
          // Get the state, this is what we care about.
          cur_state = g_smjob_stack.job_entry_list[next_entry_index]->job_status->state;

          // If the state is either completed, cancelled or aborted, we found a new index.
          if (cur_state != SMJOB_STATE_COMPLETED && cur_state != SMJOB_STATE_ABORTED &&
              cur_state != SMJOB_STATE_CANCELED)
          {
             jobs_queued++;
          }
      }
      
      // go to next
      ++num_indices_checked;
   }

   return(jobs_queued);
}

bool smjob_mgr_accepting_jobs()
{
   smjob_state_t cur_state;
   bool          accepting_jobs = false;
   uint32_t      next_entry_index;
   uint32_t      num_indices_checked = 0;

   // Only check the indices we have.
   while (num_indices_checked < SMJOB_HISTORY_MAX_ENTRIES)
   {
      // The next entry to check, modulo the array size.
      next_entry_index = num_indices_checked % SMJOB_HISTORY_MAX_ENTRIES;

      if (g_smjob_stack.job_entry_list[next_entry_index] != NULL)
      {
          // Get the state, this is what we care about.
          cur_state = g_smjob_stack.job_entry_list[next_entry_index]->job_status->state;

          // If the state is either completed, cancelled or aborted, we found a new index.
          if (cur_state == SMJOB_STATE_COMPLETED || cur_state == SMJOB_STATE_ABORTED ||
              cur_state == SMJOB_STATE_CANCELED)
          {
             accepting_jobs = true;
             break;
          }
      }
      else
      {
         accepting_jobs = true;
         break;
      }

      // Nothing found yet.
      ++num_indices_checked;
   }

   return(accepting_jobs);
}

void smjob_mgr_set_job_state(smjob_status_t *job_status,
                             smjob_state_t state, 
                             smjob_state_reason_t reason0, 
                             smjob_state_reason_t reason1, 
                             smjob_state_reason_t reason2, 
                             smjob_state_reason_t reason3)
{
    if (job_status)
    {
        int i;
        
        // re-init job status structure
        job_status->state = state;
        for (i=0; i<SMJOB_MAX_STATE_REASONS; i++)
        {
            job_status->reasons[i] = SMJOB_STATE_REASON_UNKNOWN;
        }
        job_status->num_reasons = 0;

        
        if (reason0 != SMJOB_STATE_REASON_UNKNOWN)
        {
            job_status->reasons[job_status->num_reasons] = reason0;
            job_status->num_reasons++;
        }
        
        if (reason1 != SMJOB_STATE_REASON_UNKNOWN)
        {
            job_status->reasons[job_status->num_reasons] = reason1;
            job_status->num_reasons++;
        }
        
        if (reason2 != SMJOB_STATE_REASON_UNKNOWN)
        {
            job_status->reasons[job_status->num_reasons] = reason2;
            job_status->num_reasons++;
        }
        
        if (reason3 != SMJOB_STATE_REASON_UNKNOWN)
        {
            job_status->reasons[job_status->num_reasons] = reason3;
            job_status->num_reasons++;
        }
    }
}                             


#ifdef DEBUG

//-----------------------------------------------------------------------------------
// Turn off printing of new documents.
void smjob_mgr_turn_off_printing()
{
    g_printing_turned_off = true;
}

//-----------------------------------------------------------------------------------
// Turn printing back on.
void smjob_mgr_turn_on_printing()
{
    g_printing_turned_off = false;
}

//-----------------------------------------------------------------------------------
// Query whether printing is on.
bool smjob_mgr_query_print_state()
{
    return( g_printing_turned_off );
}

#endif

//=============================================================================
// Local functions
//=============================================================================

// Send the signal that the print job is done. This function has previously
// been registered as an observer to the print interface subsystem.
static void job_done_notify( Observer_t *observer, Subject_t *subject )
{
    uint32_t status;
    extern int smjob_status_pif_job_status(Subject_t *);
    DBG_PRINTF_DEBUG("%s - send the print job done event. %d\n", __func__, smjob_status_pif_job_status(subject));

    status = sem_post( &g_smjob_mgr_pjdone_semaphore );
    ASSERT(status == 0);




}

// Wrapper to initialize the job lists.
static void init_job_lists()
{
    ATInitList(&g_jobs_w_docs.job_list);
    ATInitList(&g_jobs_wo_docs.job_list);
    ATInitList(&g_jobs_held.job_list);
}

// Create the mutexes for the different job lists
static void create_smjob_mgr_access_protection()
{
    uint32_t pthread_rcode;

    pthread_rcode = posix_mutex_init(&g_jobs_w_docs.list_mutex);
    ASSERT(pthread_rcode == 0);

    pthread_rcode = posix_mutex_init(&g_jobs_wo_docs.list_mutex);
    ASSERT(pthread_rcode == 0);

    pthread_rcode = posix_mutex_init(&g_jobs_held.list_mutex);
    ASSERT(pthread_rcode == 0);
}

// Initialize the g_smjob_stack list
static void init_g_smjob_stack()
{
    uint32_t pthread_rcode;

    memset(&g_smjob_stack, 0, sizeof(g_smjob_stack));

    // Initialize the components
    g_smjob_stack.first_valid_job_index = INITIAL_JOB_ID_VALUE % SMJOB_HISTORY_MAX_ENTRIES;
    g_smjob_stack.next_job_id = INITIAL_JOB_ID_VALUE;

    // Create the mutex
    pthread_rcode = posix_mutex_init(&g_smjob_stack.stack_mutex);
    ASSERT(pthread_rcode == 0);
}

// Move a job from one list to another.
smjob_rcode_t move_job_to_another_list( smjob_list_t *from_list_ptr,
                                         smjob_list_t *to_list_ptr,
                                         uint32_t job_id )
{
    ATLISTENTRY     *list_entry;
    smjob_t *job;
    uint32_t        px_ret_code;
    smjob_rcode_t  status = SMJOB_FAIL;

    px_ret_code = pthread_mutex_lock(&from_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    px_ret_code = pthread_mutex_lock(&to_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    // Make sure that the list we are to get the job from is not empty....
    if ( !ATIsListEmpty( &from_list_ptr->job_list ) )
    {
        list_entry = ATListHead( &from_list_ptr->job_list );

        // Start to look for the job ID.
        while ( list_entry )
        {
            job = CONTAINING_RECORD( list_entry, struct smjob_s, listnode );

            // Found it.
            if ( job->job_status->job_id == job_id )
            {
                // Remove the job from the first list...
                ATRemoveEntryList( list_entry );

                // And place it in the second.
                ATInsertTailList( &to_list_ptr->job_list, &job->listnode );
                
                // All done.
                status = SMJOB_OK;
                break;
            }

            // Get the next entry.
            list_entry = ATListNext( &from_list_ptr->job_list, list_entry );
        }
    }

    px_ret_code = pthread_mutex_unlock(&from_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    px_ret_code = pthread_mutex_unlock(&to_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);
    
    return( status );
}

// Check a list for a job 
bool get_next_job_status_from_list(smjob_list_t   *mgr_list_ptr,
                                   uint32_t        job_id_to_match,
                                   smjob_status_t **job_status_ptr,
                                   bool            *get_head_next_list)
{
    ATLISTENTRY     *list_entry;
    smjob_t *sm_job;
    uint32_t        px_ret_code;
    bool            job_id_found = FALSE;

    // Start with some initialization.
    *get_head_next_list = FALSE;

    if (!ATIsListEmpty(&mgr_list_ptr->job_list))
    {
        px_ret_code = pthread_mutex_lock(&mgr_list_ptr->list_mutex);
        XASSERT(px_ret_code == 0, px_ret_code);

        list_entry = ATListHead(&mgr_list_ptr->job_list);

        while (list_entry)
        {
            sm_job = CONTAINING_RECORD(list_entry, struct smjob_s, listnode);
            ASSERT(sm_job);
    
            // We found the job entry, now get the next one in the list.
            if (sm_job->job_status->job_id == job_id_to_match)
            {
                list_entry = ATListNext(&mgr_list_ptr->job_list, list_entry);
                if (list_entry)
                {
                    sm_job = CONTAINING_RECORD(list_entry, struct smjob_s, listnode);
                    ASSERT(sm_job);

                    *job_status_ptr = sm_job->job_status;
                    job_id_found = TRUE;
                    break;
                }
                // That was the last entry in the list, break. We will still return
                // that it was not found but set the flag that if the next list exists,
                // get the first element in it.
                else
                {
                    *get_head_next_list = TRUE;
                    break;
                }
            }

            list_entry = ATListNext(&mgr_list_ptr->job_list, list_entry);
        }

        px_ret_code = pthread_mutex_unlock(&mgr_list_ptr->list_mutex);
        XASSERT(px_ret_code == 0, px_ret_code);
    }

    return(job_id_found);
}

// Get the next active job
smjob_rcode_t get_next_active_job(smjob_status_t **job_status_ptr,
                                   bool check_cur_print_job)
{
    smjob_rcode_t status = SMJOB_OK;
    smjob_t *sm_job;

    // The caller can request that the current print job be returned.
    // However first check to see if the printer is busy or not. If not then
    // there is no current job to return.
    if (check_cur_print_job && smjob_pif_check_printer_busy())
    {
        smjob_pif_get_cur_print_job_entry(&sm_job);
        *job_status_ptr = sm_job->job_status;
    }
    // Get the head of the jobs_with_docs list if not empty.
    else if (!ATIsListEmpty(&g_jobs_w_docs.job_list))
    {
        GET_LIST_HEAD_JOB_STATUS(g_jobs_w_docs, 
                                 job_status_ptr);
    }
    // Even though the jobs in this list are being held, they are still
    // technically active. If not empty, send back the head of the list.
    else if (!ATIsListEmpty(&g_jobs_held.job_list))
    {
        GET_LIST_HEAD_JOB_STATUS(g_jobs_held, 
                                 job_status_ptr);
    }
    // No jobs found.
    else
    {
        status = SMJOB_FAIL;
    }

    return(status);
}

// Remove a job from a list if it exists there.
bool remove_job_from_list(smjob_list_t *mgr_list_ptr, uint32_t job_id)
{
    ATLISTENTRY *list_entry;
    smjob_t *sm_job;
    uint32_t    px_ret_code;
    bool        job_id_found = false;

    px_ret_code = pthread_mutex_lock(&mgr_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);

    // Get the list head and start the search.
    list_entry = ATListHead(&mgr_list_ptr->job_list);

    while (list_entry)
    {
        sm_job = CONTAINING_RECORD(list_entry, struct smjob_s, listnode);

        if (sm_job->job_status->job_id == job_id)
        {
            ATRemoveEntryList(list_entry);
            /// Note: not needed here as this is called from smjob_mgr_cancel_job,
            ///       and this is called from smjob_mgr_remove_from_stack
            ///       This would all change if the lists are consolidated to one list
            ///smjob_mgr_cleanup_job(sm_job);
            job_id_found = true;
            break;
        }

        list_entry = ATListNext(&mgr_list_ptr->job_list, list_entry);
    }

    px_ret_code = pthread_mutex_unlock(&mgr_list_ptr->list_mutex);
    XASSERT(px_ret_code == 0, px_ret_code);
    
    return(job_id_found);
}

//=============================================================================
// Thread
//
// The code that handles reading to see if there are any jobs available
// to give to the printer.
//=============================================================================

static void* smjob_mgr_loop( void* unused )
{
    bool list_empty;
    smjob_t *next_job_entry = NULL;
    smjob_rcode_t sm_res;
    uint32_t px_retcode;

    smjob_pif_AttachObserver_print_done( g_smjob_mgr_observer.smjob_pif_job_done_observer );

    while ( true )
    {
        list_empty = ATIsListEmpty(&g_jobs_w_docs.job_list);

        // If the list is empty, wait for something to show up in it.
        if ( list_empty )
        {
            DBG_PRINTF_DEBUG("%s - waiting for a new print job to show up.\n\n", __func__);
            px_retcode = sem_wait( &g_smjob_mgr_tosend_semaphore);
            XASSERT( px_retcode == 0, px_retcode );
        }

        // If the printer is busy at the moment, wait for it not to be.
        if ( smjob_pif_check_printer_busy() )
        {
            DBG_PRINTF_DEBUG("%s - printer is busy on previous job, waiting for it to be done.\n\n", __func__);
            px_retcode = sem_wait( &g_smjob_mgr_pjdone_semaphore );
            XASSERT( px_retcode == 0, px_retcode );
        }

#ifdef DEBUG
        // If the user has turned off printing, wait here until it is turned on again.
        while ( g_printing_turned_off )
        {
            posix_sleep_ticks(100);
        }
#endif
        
        // Get a job from the list of jobs ready to be printed.
        GET_JOB_FROM_PRINT_LIST( next_job_entry );

        // If for some reason the job has been removed, i.e. asynchronous cancel, just
        // go back around and wait for the next job.
        if ( !next_job_entry )
        {
           DBG_PRINTF_DEBUG("%s - where did the job go? You lied to me.\n", __func__);
            continue;
        }

        smjob_mgr_set_job_state(next_job_entry->job_status, SMJOB_STATE_PROCESSING, SMJOB_STATE_REASON_JOB_PRINTING, 0, 0, 0);

        // Start the job.
        DBG_PRINTF_DEBUG("%s - starting a new job %d.\n", __func__,
                         next_job_entry->job_status->job_id);

        sm_res = smjob_pif_start_job( next_job_entry );

        // Wait for the printer to finish before sending another job.
        if ( SMJOB_OK == sm_res )
        {
            DBG_PRINTF_DEBUG("%s - printer now has job, waiting for it to be done.\n", __func__);
            px_retcode = sem_wait( &g_smjob_mgr_pjdone_semaphore );
            XASSERT( px_retcode == 0, px_retcode );

            DBG_PRINTF_DEBUG("%s - got the print job done event.", __func__);
        }
        // The queue is full, put the job back at the head of the
        // print list.
        else
        {
            DBG_PRINTF_DEBUG("%s - the print queue is full, putting job back in list.\n", 
                       __func__);

            smjob_mgr_set_job_state(next_job_entry->job_status, SMJOB_STATE_PENDING, SMJOB_STATE_REASON_JOB_QUEUED, 0, 0, 0);

            PUT_JOB_AT_HEAD_PRINT_LIST( next_job_entry );
        }
    }
    return 0;
}


