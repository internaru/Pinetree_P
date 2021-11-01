/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef SM_JOB_INTERNAL_H
#define SM_JOB_INTERNAL_H

#include "io_device.h"
#include "list.h"
#include "event_observer.h"
#include "sm_job_api.h"
#include "sm_job_config.h"
#include "sm_job.h"

// rdj 15-NOV-2012: enable data streaming via user's registered smjob_recv_func_t
#ifndef NET_SM_STREAMING 
#define NET_SM_STREAMING
#endif

/**
 * @brief Document context
 * 
 * Each document context will point to the next document context in a job, else will
 * point to NULL.
 */
struct smjob_s;
typedef struct smjob_doc_s
{
    struct smjob_doc_s      *next;
    struct smjob_s          *sm_job; // parent job context
    smjob_doc_ticket_t      *doc_ticket;
    smjob_doc_status_t      *doc_status;
} smjob_doc_t;

/**
 * @brief Job context
 * 
 * When a job is initially created with smjob_create, sm_doc will point to NULL. After
 * smjob_add_document is called, this will point to a valid document context. 
 */
typedef struct smjob_s
{
    smjob_ticket_t          *job_ticket;
    smjob_status_func_t     job_status_func;
    smjob_status_t          *job_status;
    smjob_status_events_t   job_event_mask;
    smjob_doc_t             *sm_doc;
    ATLISTENTRY             listnode;
    bool                    eof;
    bool                    errors;
    bool                    cancelled;
#ifndef NET_SM_STREAMING 
    uint32_t                bytes_read;
#endif
    void                    *user_data;
} smjob_t;

/**
 * @brief Job stack
 * 
 * This is a list of pointers to job handles that is indexed by job_id. This
 * allows the API to access jobs by their job_ids instead of by job_handles.
 *
 */
#define INITIAL_JOB_ID_VALUE 100
#define MAX_JOB_ID_VALUE 65536 // 2^16, 2^32 = 4294967296
#define INITIAL_DOC_ID_VALUE 100
#define MAX_DOC_ID_VALUE 65536 // 2^16
typedef struct smjob_stack_s smjob_stack_t;
struct smjob_stack_s
{
    /**
     * @brief The array of job handles, or entries.  The macro JOB_HISTORY_MAX_ENTRIES
     *        is defined in the oem config layer.
     */
    smjob_t *job_entry_list[SMJOB_HISTORY_MAX_ENTRIES];
    smjob_ctxt_t *jobs[SMJOB_HISTORY_MAX_ENTRIES]; // rdj transitioning to new job context

    /**
     * @brief A pointer to the first valid job handle in the list.
     *        To be incremented by 1 after lifetime of a job entry expires and the job 
     *        context is freed. Starts at INITIAL_JOB_ID_VALUE % JOB_HISTORY_MAX_ENTRIES
     *        and resets after JOB_HISTORY_MAX_ENTRIES -1.  
     *        If next_job_id % JOB_HISTORY_MAX_ENTRIES ever equals first_valid index, 
     *        then we've exceeded max entries and the entry at position 
     *        next_job_id % JOB_HISTORY_MAX_ENTRIES must first be freed.
     *        Lifetime of a job is limited by JOB_ENTRY_LIFETIME_SEC, defined in the 
     *        oem config layer.
     */
    uint32_t first_valid_job_index;

    /**
     * @brief The next job_id to be assigned.
     *        Accessed via mutex protection, incremented by 1 after each assignment of a 
     *        new job ID to a dynamically allocated struct smjob_s. Typically job ID starts 
     *        at 100 and goes up to 2^32 -1 before starting back at 100. Assign a pointer 
     *        to the newly allocated struct smjob_s to the array entry at index 
     *        next_job_id % JOB_HISTORY_MAX_ENTRIES, and then increment next_job_id.
     */
    uint32_t next_job_id;

    /**
     * @brief Mutex to protect accessing this struct of job entry pointers.
     */
    pthread_mutex_t stack_mutex;
};

/**
 * @brief Job list types for mgr
 *
 * Description of different types of job lists in smjob_mgr
 */
typedef enum 
{
    JOBS_W_DOCS_LIST,
    JOBS_WO_DOCS_LIST,
    JOBS_HELD_LIST

} smjob_mgr_list_type_t;

/**
 * @brief Job list types for pif
 *
 * Description of different types of job lists in smjob_pif
 */
typedef enum 
{
    COMPLETED_JOBS_LIST

} smjob_pif_list_type_t;

/**
 * @brief Job list
 *
 * This combines a job list with a mutex.
 */
typedef struct smjob_list_s
{
    ATLISTENTRY job_list;
    pthread_mutex_t    list_mutex;

} smjob_list_t;

/**
 * @brief Job commands: Initialize the smjob commands for debug
 *
 */
void smjob_cmd_init( void );

/**
 * @brief Job manager: Initialize the job mgr
 *
 * @param[in] *name - Used in the context, mutex, and thread names
 *
 */
void smjob_mgr_init( const char *name );

/**
 * @brief Job manager: Get a specified doc entry if it exists
 *
 * @param[in] doc_id - The doc to find
 *
 * @param[out] **sm_doc - The document context matching doc_id
 *
 * @retval SM_JOB_OK - Doc found
 *  
 * @retval SM_JOB_FAIL - Doc not found
 *
 */
smjob_rcode_t smjob_mgr_doc_lookup_stack(uint32_t doc_id, smjob_doc_t **sm_doc);

/**
 * @brief Job manager: Get a specified job entry if it exists
 *
 * @param[in] job_id - The job to find, or SM_JOB_INVALID_ID
 *
 * @param[out] *ret_job_entry - The job handle matching job_id, or the first valid job handle
 *
 * @retval SM_JOB_OK - Job found
 *  
 * @retval SM_JOB_FAIL - Job not found
 *
 */
smjob_rcode_t smjob_mgr_job_lookup_stack(uint32_t job_id, smjob_t **ret_job_entry);
smjob_rcode_t smjob_mgr_job_lookup_stack_ctxt(uint32_t job_id, smjob_t **ret_job_entry, smjob_ctxt_t **ret_job_ctxt);

/**
 * @brief Job manager: Insert a job into the job stack. This will also assign
 *                     the job id and manage the pointers in the struct.
 *
 * @param[in] sm_job - The job to add to the stack
 *
 * @retval SM_JOB_OK - Job successfully added to the stack
 *  
 * @retval SM_JOB_FAIL - Failed to add to stack
 *
 */
smjob_rcode_t smjob_mgr_insert_job_in_stack( smjob_t *sm_job, smjob_ctxt_t *job );

/**
 * @brief Job manager: Remove a job from the job stack.
 *
 * @param[in] smjob_id - The job_id to remove from the stack
 *
 * @retval SM_JOB_OK - Job successfully removed from the stack
 *  
 * @retval SM_JOB_FAIL - Failed to remove from stack
 *
 */
smjob_rcode_t smjob_mgr_remove_from_stack( uint32_t smjob_id );

/**
 * @brief Job manager: Insert a job into the proper list based on whether
 *                     a document is present.
 *
 * @param[in] sm_job - The job to add to the list
 *
 * @retval SM_JOB_OK - Job successfully added to the appropriate list
 *  
 * @retval SM_JOB_FAIL - Failed to add to a list
 *
 */
smjob_rcode_t smjob_mgr_insert_job_in_list( smjob_t *sm_job );

/**
 * @brief Job manager: Get a specified job entry if it exists
 *
 * @param[in] job_id - The job to find
 *
 * @param[out] *ret_job_entry - The job handle matching job_id
 *
 * @retval SM_JOB_OK - Job found
 *  
 * @retval SM_JOB_FAIL - Job not found
 *
 */
smjob_rcode_t smjob_mgr_get_job_entry(uint32_t job_id,
                                        smjob_t **ret_job_entry);
/**
 * @brief Job manager: Get the next active job
 *
 * @param[in] *prev_job_stat_ptr - The starting point to search from
 * Or NULL if start from current job
 *
 * @param[out] **next_job_stat_ptr - The next job in list
 *
 * @retval SM_JOB_OK - Job found
 *  
 * @retval SM_JOB_FAIL - Job not found
 *
 */
smjob_rcode_t smjob_mgr_get_next_active_job(smjob_status_t *prev_job_stat_ptr,
                                              smjob_status_t **next_job_stat_ptr);
 
/**
 * @brief Job manager: Find a particular job in the lists and remove it
 *
 * @param[in] cur_job_id - The job to find and remove
 *
 * @retval SM_JOB_OK - Job found in a list and removed
 *  
 * @retval SM_JOB_FAIL - Job not found in any list; nothing removed
 *
 */
smjob_rcode_t smjob_mgr_cancel_job(uint32_t cur_job_id);

/**
 * @brief Job manager: Add a document to a job and move the job to
 * the proper list.
 *
 * @param[in] sm_job - The job to add doc to
 *
 * @param[in] last_doc - Bool to indicate if this is the last doc
 *
 * @param[in] sm_doc - Doc to be added
 *
 * @retval SM_JOB_OK - Doc successfully added to the job
 *  
 * @retval SM_JOB_FAIL - Failed to add doc to the job
 *
 */
smjob_rcode_t smjob_mgr_add_document(smjob_t *sm_job,
                                       bool last_doc,
                                       smjob_doc_t *sm_doc);

/**
 * @brief Job manager: Return the requested list of jobs
 *
 * @param[in] req_list - The requested type of job list
 *
 * @retval ATLISTENTRY - The address of the requested jobs list
 *  
 */
const ATLISTENTRY *smjob_mgr_get_requested_list( smjob_mgr_list_type_t req_list );

/**
 * @brief Job manager: Check whether a particular job is in a list
 *
 * @param[in] *mgr_list_ptr - The list to check
 *
 * @param[in] job_id - The job to find
 *
 * @param[out] *ret_job_entry - The found matching job
 *
 * @retval bool - TRUE if found, FALSE if not
 *  
 */
bool smjob_mgr_check_list_for_job_id(smjob_list_t *mgr_list_ptr,
                                      uint32_t job_id,
                                      smjob_t **ret_job_entry);

/**
 * @brief Job manager: Turn off printing
 *
 * For debug purposes, this will halt the smjob_mgr state machine.
 *
 */
void smjob_mgr_turn_off_printing();

/**
 * @brief Job manager: Turn on printing
 *
 * For debug purposes, this will continue the smjob_mgr state machine processing.
 *
 */
void smjob_mgr_turn_on_printing();

/**
 * @brief Job manager: Query the state of printing
 *
 * For debug purposes, return the state of printing of the smjob_mgr state machine.
 *
 * @retval bool - TRUE if printing turned off, FALSE if printing turned on
 *  
 */
bool smjob_mgr_query_print_state();

/**
 * @brief Job manager: Cleanup the resources from a job entry
 *
 * @param[in] sm_job - The job to free from memory
 *
 */
void smjob_mgr_cleanup_job(smjob_t *sm_job);

/**
 * @brief Job manager: Get number of jobs currently in the queue
 *
 * Return the queued job count
 *
 * @retval uint32_t - job count
 *  
 */
uint32_t smjob_mgr_get_queued_job_count();

/**
 * @brief Job manager: Query if printer is accepting jobs
 *
 * Return whether the printer is accepting jobs
 *
 * @retval bool - TRUE if accepting jobs, FALSE if not accepting jobs
 *  
 */
bool smjob_mgr_accepting_jobs();


/**
 * @brief Job manager: Set job state and reasons
 *
 * Set the job state and reasons
 *
 * @param[in] state - Job State
 * @param[in] reason0 - Job Reason[0]
 * @param[in] reason1 - Job Reason[1]
 * @param[in] reason2 - Job Reason[2]
 * @param[in] reason3 - Job Reason[3]
 *  
 */
void smjob_mgr_set_job_state(smjob_status_t *job_status,
                             smjob_state_t state, 
                             smjob_state_reason_t reason0, 
                             smjob_state_reason_t reason1, 
                             smjob_state_reason_t reason2, 
                             smjob_state_reason_t reason3);

/**
 * @brief Print inferface: Initialize the print interface
 *
 * An initialization function, creates the pif context, thread,
 * observer, and the completed jobs list and mutex.
 *
 * @param[in] type - Channel type set to e_Print when registering
 *
 * @param[in] *name - Used in the context, mutex, and thread names
 *
 */
void smjob_pif_init(Channel_type_t type, const char *name);

#ifdef HAVE_SCAN_SUPPORT
void smjob_sif_init(void);
smjob_rcode_t smjob_sif_start_scan(smjob_ctxt_t *job);
smjob_rcode_t smjob_sif_cancel_scan(smjob_ctxt_t *job);
#endif // HAVE_SCAN_SUPPORT

/**
 * @brief Print inferface: Start the print job
 *
 * Called by smjob_add_document, triggers the pif thread. This will
 * initialize with the net_io layer which will start the process to print.
 *
 * @param[in] *cur_print_job - Channel type set to e_Print when registering
 *
 * @retval SM_JOB_OK - Message successfully sent to pif thread
 *  
 * @retval SM_JOB_FAIL - Message unsuccessfully sent to pif thread (queue full)
 *
 */
smjob_rcode_t smjob_pif_start_job( smjob_t *cur_print_job );

/**
 * @brief Print inferface: Check if printer is busy
 *
 * Called by the smjob_mgr, checks status of printer,
 * set in the pif state machine.
 *
 * @retval bool - TRUE if busy, FALSE if not
 *  
 */
bool smjob_pif_check_printer_busy();

/**
 * @brief Print inferface: Return the job ID of the current print job
 *
 * @retval uint32_t - The job ID
 *  
 */
uint32_t smjob_pif_get_cur_print_job_id();

/**
 * @brief Print inferface: Get the current print job information.
 *
 * @param[out] *sm_job - The current print job
 *
 */
void smjob_pif_get_cur_print_job_entry(smjob_t **sm_job);

/**
 * @brief Print inferface: Get completed print job
 *
 * Used to get the completed print jobs. If the prev_comp_job is NULL, get
 * the first completed print job. Else we get the next completed job after
 * the prev_comp_job if it exists.
 *
 * @param[in] *prev_comp_job - Either a pointer to the previously completed
 * job in the list, or NULL.
 *
 * @param[out] **job_status_ptr - The next completed print job
 *
 * @retval SM_JOB_OK - A job was found
 *  
 * @retval SM_JOB_FAIL - A job was not found
 *
 */
smjob_rcode_t smjob_pif_get_comp_print_job(smjob_status_t *prev_comp_job,
                                             smjob_status_t **job_status_ptr);

/**
 * @brief Print inferface: Send a message to cancel the current print job
 *
 * @retval SM_JOB_OK - Message successfully sent
 *  
 * @retval SM_JOB_FAIL - Message failed to be sent
 *
 */
smjob_rcode_t smjob_pif_cancel_cur_print_job();

/**
 * @brief Print inferface: Attach observer for print done
 *
 * Used to attach an observer to the print done status.
 *
 * @param[in] *o - Observer to attach
 *
 */
void smjob_pif_AttachObserver_print_done( Observer_t *o );

/**
 * @brief Print inferface: Get requested list.
 * A debug command to return a pointer to the requested list. 
 * In this case it is only the completed list.
 *
 * @param[in] req_list - The requested list
 *
 * @retval *ATLISTENTRY - A pointer to the requested list; otherwise NULL
 *
 */
const ATLISTENTRY *smjob_pif_get_requested_list(smjob_pif_list_type_t req_list);

/**
 * @brief Print inferface: Get a job entry from the completed list if it exists
 *
 * @param[in] job_id - Job ID of desired job entry
 *
 * @param[out] *ret_job_entry - The job entry from the completed list that 
 * matches the input job ID
 *
 * @retval SM_JOB_OK - Job was found
 *  
 * @retval SM_JOB_FAIL - Job was not found
 *
 */
smjob_rcode_t smjob_pif_get_job_entry(uint32_t job_id, smjob_t **ret_job_entry);

/**
 * @brief Print inferface: Remove a file if there was one
 *
 * @param[in] *file_name - File to remove
 *
 */
void smjob_pif_remove_file_if_exists(char *file_name);


#endif // SM_JOB_INTERNAL_H
