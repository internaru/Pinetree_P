/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**\file fax_app_main.c
 *
 * This file contains the main API impl and init for the fax app
 */


#include <string.h>
#include <dprintf.h>
#include "agRouter.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "agMessage.h"
#include "SJMgr.h"
#include "ResourceMap.h"
#include "lassert.h"

#include "faxer_api.h"
#include "debug.h"
#include "fax_app_api.h"
#include "fax_phbook.h"
#include "fax_app_prv.h"
//#include "fax_reports_api.h"
//#include "fax_pipe_api.h"
#include "posix_ostools.h"

#define FAXAPPMAIN_STACK_SIZE (MAX((1024 * 10), POSIX_MIN_STACK_SIZE))

typedef struct{
       char name[20];
       char *pStack;
       mqd_t *pQueue;
       pthread_t *pTCB;
       MESSAGE *pMSGBuffer;
 }FAXER_JOB_AGENT_CONTROL;

bool test_mode_no_print_on = 0;

/**
 * Start a job agent to handle job related details.
 *
 */
static void *run_fax_app_job_agent(char *name, void* (*agent_function)(void* Input),
                                 MODULE_ID id, RESOURCES type, int stack_size)
{
    #define FAXER_JOB_AGENT_TASK_QUEUE_SIZE 100

    FAXER_JOB_AGENT_CONTROL *pAgentCtrl;

    if(stack_size < 512)
    {
        dbg_printf("Stack size is too small %d, need at least 512.\n", stack_size);
        return NULL;
    }

    do
    {
        pAgentCtrl = MEM_MALLOC(sizeof(FAXER_JOB_AGENT_CONTROL));
        if (pAgentCtrl == NULL) posix_sleep_ticks(1);
    } while(pAgentCtrl == NULL);

    strcpy(pAgentCtrl->name, name);

    do
    {
        pAgentCtrl->pStack = MEM_MALLOC(stack_size);
        if ( pAgentCtrl->pStack == NULL) posix_sleep_ticks(1);
    } while( pAgentCtrl->pStack == NULL);

    do
    {
        pAgentCtrl->pQueue = MEM_MALLOC(sizeof(mqd_t));
        if (pAgentCtrl->pQueue == NULL) posix_sleep_ticks(1);
    } while(pAgentCtrl->pQueue == NULL);

    do
    {
        pAgentCtrl->pTCB= MEM_MALLOC(sizeof(pthread_t));
        if (pAgentCtrl->pTCB == NULL) posix_sleep_ticks(1);
    } while(pAgentCtrl->pTCB == NULL);

    do
    {
        pAgentCtrl->pMSGBuffer = MEM_MALLOC(FAXER_JOB_AGENT_TASK_QUEUE_SIZE* sizeof(MESSAGE));
        if (pAgentCtrl->pMSGBuffer == NULL) posix_sleep_ticks(1);
    } while(pAgentCtrl->pMSGBuffer == NULL);

    posix_create_message_queue(pAgentCtrl->pQueue, pAgentCtrl->name, 
                               FAXER_JOB_AGENT_TASK_QUEUE_SIZE, sizeof(MESSAGE));
    // Register the queue with the routcd er.
    router_register_queue(id, *(pAgentCtrl->pQueue));  // Register a message queue.

        // Register the resource
    rm_register(type, id);

    if(posix_create_thread( pAgentCtrl->pTCB, agent_function, (void*)pAgentCtrl->pQueue,
                         pAgentCtrl->name,  pAgentCtrl->pStack, stack_size,
                         POSIX_THR_PRI_NORMAL))
    {
        MEM_FREE_AND_NULL(pAgentCtrl->pStack);
        MEM_FREE_AND_NULL(pAgentCtrl->pQueue);
        MEM_FREE_AND_NULL(pAgentCtrl->pTCB);
        MEM_FREE_AND_NULL(pAgentCtrl->pMSGBuffer);
        MEM_FREE_AND_NULL(pAgentCtrl);
        return NULL;
    }
    return pAgentCtrl;
}

void fax_app_init()
{
    void *svc_handle; /* for passing to reports */

    fax_app_context_initialize();
  
    /* required for PC print to fax */ 
//    fax_pipe_init(); 

    fax_phbook_init();
    fax_app_status_initialize();
    svc_handle = fax_app_faxer_init();
//    fax_reports_init(svc_handle);

    //Plug-in print agent
    if(!run_fax_app_job_agent("print_job_agent", fax_app_agent_print, FAXPRINTID, 
                              e_FaxPrintApp, FAXAPPMAIN_STACK_SIZE))
    {
          dbg_printf("Fail to start faxer print job agent! Abort!");
          return;
    }
    //Plug-in scan agent
    if(!run_fax_app_job_agent("scan_job_agent", fax_app_agent_scan, SCANTOFAXID, 
                              e_ScanToFax,  FAXAPPMAIN_STACK_SIZE))
    {
          dbg_printf("Fail to start faxer scan job agent! Abort!");
          return;
    }
}

/** Send a fax
*/
fax_app_res_t fax_app_send_fax(fax_app_ticket_t *fax_ticket)
{
  //fax_redial_store(fax_ticket->digits);
  return(fax_app_context_start_send(fax_ticket));
}

/** Add raw image data to the fax
*/
fax_app_res_t fax_app_send_add_raw_page( fax_app_ext_page_data_t *data )
{
  return(fax_app_context_add_page(data));
}

/** Trigger a receive of the fax job
*/
fax_app_res_t fax_app_receive_fax(void)
{
  return(fax_app_context_start_recv());
}

/** Trigger a reprint of a particular job
*/
fax_app_res_t fax_app_reprint_fax(uint32_t job_id)
{
  return(fax_app_context_start_reprint(job_id));
}

/** Get a list of reprintable faxes
*/
fax_app_res_t fax_app_reprint_get_list(FAXER_ACTIVITY_ENTRY *list, uint32_t max_items, uint32_t *num_items_ret )
{
  int i;
  uint32_t *index_set;
  uint32_t cnt;
  FAXER_ACTIVITY_ENTRY entry;
  uint32_t item_index = 0;

  cnt = faxer_find_fax(NULL, FAXER_ACT_TYPE_RECEIVE, 0xffff, 0, 0);

  if (!(index_set = MEM_MALLOC(sizeof(uint32_t) * cnt)))
  {
    return(FAIL);
  }

  cnt = faxer_find_fax(NULL, FAXER_ACT_TYPE_RECEIVE, 0xffff, index_set, cnt);
  for(i=0; (i < cnt) && (i < max_items); i++)
  {
    faxer_get_log(NULL, index_set[i], &entry);
    if(strlen(entry.fname) && 
      ((entry.status == FAXER_ACT_STATUS_FAX_COMPLETE) || (entry.status == FAXER_ACT_STATUS_COMPLETE_RTN)))
    {
      memcpy(&list[item_index], &entry, sizeof(FAXER_ACTIVITY_ENTRY));
      item_index++;
    }
  }

  *num_items_ret = item_index;

  MEM_FREE_AND_NULL(index_set);
  return(OK);
}

/** Check if last fax was printed or not
*/
fax_app_res_t fax_app_check_if_last_fax_printed(bool *printed)
{
  FAXER_ACTIVITY_ENTRY entry;
  uint32_t cnt;
  uint32_t *index_set;

  *printed = true;

  cnt = faxer_find_fax(NULL, FAXER_ACT_TYPE_RECEIVE, 0xffff, 0, 0);

  if(cnt == 0)
  {
    return(OK);
  }
 
  if (!(index_set = MEM_MALLOC(sizeof(uint32_t) * cnt)))
  {
    return(FAIL);
  }

  cnt = faxer_find_fax(NULL, FAXER_ACT_TYPE_RECEIVE, 0xffff, index_set, cnt);
  faxer_get_log(NULL, index_set[cnt-1], &entry);

  if(entry.pending)
  {
    *printed = false;  
    fax_app_faxer_clear_print_pending(entry.job_id);
  }

  MEM_FREE_AND_NULL(index_set);
  return(OK);
}

/** Cancel all fax jobs
*/
fax_app_res_t fax_app_cancel(void)
{
  return(fax_app_end_all());
}

/** Test mode - no print on/off 
*/
fax_app_res_t fax_app_test_mode_no_print(bool on)
{
  test_mode_no_print_on = on;
  return(FAX_APP_RES_SUCCESS);
}
