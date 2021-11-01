/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef FAX_APP_PRV_H_
#define FAX_APP_PRV_H_

#include <stdint.h>
#include "fax_app_api.h"
#include "lassert.h"


#define START_JOB(job_type, module, addRsc) do{\
					 StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));\
					 ASSERT(StartIt != NULL);\
					 job_msg.msgType = MSG_STARTRECIPE;\
					 StartIt->Job = (job_type);\
					 StartIt->AdditionalResource = addRsc;\
					 StartIt->Pipe = NULL;\
					 StartIt->Wait = e_NoWait;\
					 StartIt->SendingModule = (module);\
					 job_msg.param3 = StartIt;\
					 SYMsgSend(SJMID, &job_msg);}while(0)

#define START_JOB_PIPE(job_type, module, addRsc, wait, pipe) do{\
					 StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));\
					 ASSERT(StartIt != NULL);\
					 job_msg.msgType = MSG_STARTRECIPE;\
					 StartIt->Job = (job_type);\
					 StartIt->AdditionalResource = addRsc;\
					 StartIt->Pipe = pipe;\
					 StartIt->Wait = wait;\
					 StartIt->SendingModule = (module);\
					 job_msg.param3 = StartIt;\
					 SYMsgSend(SJMID, &job_msg);}while(0)

#define END_JOB(resource, module)   do{\
                     job_msg.msgType = MSG_FREERECIPE;\
                     job_msg.param1 = 0;\
                     job_msg.param3 = (resource);\
                     SYMsgSend(SJMID, &job_msg);\
                     resource = NULL; }while(0)

#define CANCEL_JOB(resource, module) do{\
                     job_msg.msgType = MSG_CANCELJOB;\
                     job_msg.param1 = SYS_REQUSER;\
                     job_msg.param2 = 0;\
                     job_msg.param3 = 0;\
                     SYMsgSend( SJMID, &job_msg);}while(0)

#define ACK_CANCEL(resource, module) do{\
                     job_msg.msgType = MSG_CANCELJOB;\
                     job_msg.param1 = SYS_ACK;\
                     job_msg.param2 = module;\
                     job_msg.param3 = resource;\
                     SYMsgSend( SJMID, &job_msg);}while(0)

// Send a directed cancel to the job containing the specified resource.
// (For the most part, used to make sure a job kills itself rather than
// another unsuspecting job in the system.)
#define CANCEL_JOB_CONTAINING_MODULE(cancel_source, cancel_dest) do{\
                     job_msg.msgType = MSG_CANCELJOB;\
                     job_msg.param1 = SYS_REQUEST;\
                     job_msg.param2 = cancel_source;\
                     job_msg.param3 = (void*)cancel_dest;\
                     SYMsgSend(SJMID, &job_msg);\
                     }while(0)


#define MAX_FAX_INSTANCES (10)


void* fax_app_agent_print(void* Input);
void* fax_app_agent_scan(void* Input);
void fax_app_event_processor(uint16_t evt, void *data);
void *fax_app_faxer_init(void);

/***************************************
* Context 
***************************************/
typedef enum
{
  FAX_SUBJOB_IDLE = 0,
  FAX_SUBJOB_QUEUED,
  FAX_SUBJOB_ACTIVE, 
  FAX_SUBJOB_WAITING_ON_DATA, 
  FAX_SUBJOB_DATA_AVAILABLE, 
  FAX_SUBJOB_CANCELING,
  FAX_SUBJOB_CANCELED,
  FAX_SUBJOB_COMPLETE,
} fax_subjob_state_t;

typedef enum
{
  FAX_APP_SEND,
  FAX_APP_RECV,
} fax_context_type_t; 


void fax_app_context_initialize(void);

/* start a send context */
fax_app_res_t fax_app_context_start_send(fax_app_ticket_t *ticket);
/* add a raw page data to the active fax send job */
fax_app_res_t fax_app_context_add_page( fax_app_ext_page_data_t *data );
/* start a receive context */
fax_app_res_t fax_app_context_start_recv(void);
/* end all active contexts */
fax_app_res_t fax_app_end_all(void);

typedef struct
{
  fax_app_fax_sub_state_t state;
  fax_app_comp_error_t    completion_error; 
  uint16_t cur_page;
  uint16_t pages_completed;
  uint32_t job_id;
} fax_app_fax_info_t;

typedef struct
{
  fax_app_context_id_t context_id;
  fax_app_state_t      state;
  fax_app_fax_info_t   fax_info;
} fax_app_context_status_data_t; 


typedef struct
{
  void *context; 
  fax_subjob_state_t      state; 
  uint32_t job_id;
  fax_app_fax_sub_state_t fax_substate;
  char * receive_filename;
  fax_app_comp_error_t    completion_error;
  uint16_t active_page;  
  uint16_t pages_completed;  
} fax_subjob_update_t;

/* Subjob callback for status updates */
typedef fax_app_res_t (*fax_app_subjob_callback_t)(fax_subjob_update_t *data);

int fax_app_faxer_send(char *digits, char *fname, FAX_TYPES res, void *context, 
                       fax_app_subjob_callback_t callback );
int fax_app_faxer_receive(void *context, fax_store_handle_t store, 
                          fax_app_subjob_callback_t callback);
char *fax_app_faxer_get_filename(uint32_t job_id);
int fax_app_faxer_start_jpeg_send(char *digits, bool color, FAX_TYPES res, void *context, 
                                   fax_store_handle_t store, 
                                   fax_app_subjob_callback_t callback );
int fax_app_faxer_notify_page_added(bool last_page);
int fax_app_faxer_set_data(bool jpeg, void *data, 
                           uint16_t pages, bool data_complete);
int fax_app_faxer_cancel(void *context);
int fax_app_faxer_clear_files(uint32_t job_id);
int fax_app_faxer_check_color_support(uint32_t job_id);
int fax_app_faxer_check_jpeg_support(uint32_t job_id);
int fax_app_faxer_check_jpeg_resolution(uint32_t job_id, FAX_TYPES req_res);
int fax_app_faxer_free_storage(int space_required);
int fax_app_faxer_clear_print_pending(uint32_t job_id);


int fax_app_print_start(char *fname, void *context, uint32_t job_id,
                       fax_app_subjob_callback_t callback, int pages, bool complete);
int fax_app_print_start_jpeg(fax_store_handle_t store, void *context, 
                              fax_app_subjob_callback_t callback, int pages, bool complete);
int fax_app_print_notify_page_added(void);
int fax_app_print_notify_document_completed(void);

int fax_app_print_cancel(void *context);


int fax_app_scan_start_bw(char *fname, fax_app_brightness_t brightness, fax_app_resolution_t resolution, 
                          void *context, fax_app_subjob_callback_t callback);
int fax_app_scan_start_color(fax_store_handle_t store, fax_app_brightness_t brightness, 
                             fax_app_resolution_t resolution, void *context, 
                             fax_app_subjob_callback_t callback);
int fax_app_scan_start_gray(fax_store_handle_t store, fax_app_brightness_t brightness, 
                            fax_app_resolution_t resolution, void *context, 
                            fax_app_subjob_callback_t callback);
fax_app_res_t fax_app_context_start_reprint(uint32_t job_id);

int fax_app_scan_cancel(void *context);


/***************************************
* Status 
***************************************/
void *fax_app_status_create(fax_app_context_id_t id);
void fax_app_status_destroy(void * handle);
void fax_app_status_initialize(void);
void fax_app_status_update(void * handle, fax_app_context_status_data_t *data);


/***************************************
* Test modes 
***************************************/
extern bool test_mode_no_print_on;
#endif /* FAX_APP_PRV_H_ */
