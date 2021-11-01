/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <string.h>
#include <dprintf.h>

#include "agRouter.h"
#include "faxer_api.h"
#include "fax_app_api.h"
#include "fax_app_prv.h"
#include "debug.h"

static void *fax_service_handle = NULL;

typedef struct
{
  void *context;
  fax_app_subjob_callback_t update_callback; 
  fax_context_type_t type; /* send/receive */
  bool jpeg;
  uint32_t job_id;
  uint16_t active_page;
  uint16_t pages_completed;
  fax_store_handle_t store;
  fax_app_comp_error_t comp_error;
  bool data_set;
}_faxer_job_info_t;
static _faxer_job_info_t _faxer_info;

static void _reset_info(void)
{
  memset(&_faxer_info, 0, sizeof(_faxer_info));
}

/*---------------------------------------------*/
static void _update_completion_status(fax_subjob_state_t state, fax_app_fax_sub_state_t substate, fax_app_comp_error_t error)
{
  fax_subjob_update_t update;
  /* clear it out */
  memset(&update, 0, sizeof(update));
  /* only context id and state are needed */
  update.context = _faxer_info.context;
  update.state = state;
  update.job_id = _faxer_info.job_id;
  update.fax_substate = substate;
  update.active_page = _faxer_info.active_page;
  update.pages_completed = _faxer_info.pages_completed;
  if(_faxer_info.comp_error)
  {
    update.completion_error = _faxer_info.comp_error;
  }
  else
  {
    update.completion_error = error;
    _faxer_info.comp_error = error;
  }
  if(_faxer_info.update_callback)
    (*_faxer_info.update_callback)(&update);
  else
    dbg_printf("FAXAPP: WARNING, received update from FAXER but NO context exists!\n");
}

/*---------------------------------------------*/
static void _update_status(fax_subjob_state_t state, fax_app_fax_sub_state_t substate)
{
  _update_completion_status(state, substate, FAXER_ACT_STATUS_NULL);
}


/*---------------------------------------------*/
void *fax_app_faxer_init(void)
{
    memset(&_faxer_info, 0, sizeof(_faxer_info));
    if(!fax_service_handle)
    {
        fax_service_handle = faxer_service_start(fax_app_event_processor);
        if(!fax_service_handle)
        {
            dbg_printf("Fail to start fax service!!!!!!!!!!!!!!");
        }
    }
    dbg_printf("FAXER service handle = 0x%08x\n", fax_service_handle);
    return(&fax_service_handle);
}

void fax_app_event_processor(uint16_t evt, void *data)
{
    FAXER_APP_EVT               app_evt = (FAXER_APP_EVT)evt;
    uint32_t                    tmp;
    FAXER_ACT_STATUS   comp_status;

    switch(app_evt)
    {
        case FAXER_APP_EVT_LIURING:
            faxer_get_session_data(fax_service_handle, FAXER_SESSION_INT_RING_CNT, &tmp);
            dbg_printf(">>FAX_APP_FAXER: RING..%d..\n", tmp);
            break;

        case FAXER_APP_EVT_REM_ANSWERED:
            _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_NEGOTIATING);
            dbg_printf("FAX_APP_FAXER: Call is answered\n");
            break;

        case FAXER_APP_EVT_SESSION_START:
            _faxer_info.active_page = 0;
            _faxer_info.pages_completed = 0;
            dbg_printf(">>FAX_APP_FAXER: fax session start, job_id=%d\n", (uint32_t)data);
            break;

        case FAXER_APP_EVT_DOC_START: 
            if(_faxer_info.type == FAX_APP_SEND)
            {
              if(_faxer_info.data_set)
                _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_SENDING);
              else
                _update_status(FAX_SUBJOB_WAITING_ON_DATA, FAX_APP_FAX_STATE_SENDING);
            }
            else
            {
              _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_RECEIVING);
            }
            dbg_printf(">>FAX_APP_FAXER: Document start, job_id=%d\n", (uint32_t)data);
            break;

        case FAXER_APP_EVT_DOC_END:
            dbg_printf(">>FAX_APP_FAXER: Document end, job_id=%d\n", (uint32_t)data);
            break;

        case FAXER_APP_EVT_PAGE_END:
            _faxer_info.pages_completed++;
            if(_faxer_info.type == FAX_APP_SEND)
            {
              _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_SENDING);
            }
            else
            {
              _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_RECEIVING);
            }
            dbg_printf(">>FAX_APP_FAXER: SENT/RECEIVED PAGE..%d..\n", (uint32_t)data);
            break;

        case FAXER_APP_EVT_SESSION_END:
            /* at this point, the session is over and the data has been transmitted/received */
            comp_status =(FAXER_ACT_STATUS)data;
            _update_completion_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_COMPLETE, comp_status);
            break;

        /** many of the errors that can happen before the session is even started, so there will be no session end,
            however there should still be a JOB_END, and if not it is a bug :) */
        case FAXER_APP_EVT_REM_NOANSWER:
            _update_completion_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_COMPLETE, FAXER_ACT_STATUS_NO_ANSWER);
            break;
        case FAXER_APP_EVT_REM_BUSY: 
            _update_completion_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_COMPLETE, FAXER_ACT_STATUS_LINE_BUSY);
            break;
        case FAXER_APP_EVT_NO_DIALTONE:
            _update_completion_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_COMPLETE, FAXER_ACT_STATUS_NO_DIALTONE);
            break; 
        case FAXER_APP_EVT_DIALFAILURE: 
            _update_completion_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_COMPLETE, FAXER_ACT_STATUS_CALL_FAILED);
            break;
        case FAXER_APP_EVT_OFFHOOKFAILURE:
            _update_completion_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_COMPLETE, FAXER_ACT_STATUS_CALL_FAILED);
            break;
        /**/
      
        case FAXER_APP_EVT_JOB_END:
            dbg_printf(">>FAX_APP_FAXER: JOB END\n");
            _update_status(FAX_SUBJOB_COMPLETE, FAX_APP_FAX_STATE_COMPLETE);
            _reset_info();
            break;

        case FAXER_APP_EVT_WAITING_RETRY:
            dbg_printf(">>FAX_APP_FAXER: Waiting to retry...still active\n");
            break;

        case FAXER_APP_EVT_PAGE_START:
            _faxer_info.active_page++;
            if(_faxer_info.type == FAX_APP_SEND)
            {
              _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_SENDING);
            }
            else 
            {
              _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_RECEIVING);
            }
            dbg_printf(">>FAX_APP_FAXER: PAGE START...\n");
            break;

        case FAXER_APP_EVT_AUTO_ANSWER:
            fax_app_receive_fax(); 
            break;
            
        default:
            dbg_printf(">>FAX_APP_FAXER: Got fax app event %d\n", app_evt);
            break;

    }

}

/*---------------------------------------------*/
int fax_app_faxer_receive(void *context, fax_store_handle_t store, fax_app_subjob_callback_t callback)
{
  uint32_t job_id;
  int ret = 0;
    
  if(!(ret = faxer_start_receive(fax_service_handle, store, &job_id)))
  {
    _faxer_info.job_id = job_id;
    _faxer_info.type = FAX_APP_RECV;
    _faxer_info.context = context;
    _faxer_info.jpeg = FALSE;
    _faxer_info.store = store;
    _faxer_info.update_callback = callback;
    _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_NEGOTIATING);
  }
  return(ret);
}

/*---------------------------------------------*/
int fax_app_faxer_send(char *digits, char *fname, FAX_TYPES res, void *context, 
                       fax_app_subjob_callback_t callback )
{
  uint32_t job_id;
  int ret;

  /* try to send the job */
  if(!(ret = faxer_schedule_send(fax_service_handle, NULL, digits, res, fname, 
                                 &job_id)))
  {
    _faxer_info.job_id = job_id;
    _faxer_info.type = FAX_APP_SEND;
    _faxer_info.context = context;
    _faxer_info.jpeg = 0;
    _faxer_info.update_callback = callback;
    if(fname) _faxer_info.data_set = 1;
    _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_DIALING);
  }
  else
    ret = 1;
  return(ret);
}

/*---------------------------------------------*/
int fax_app_faxer_start_jpeg_send(char *digits, bool color, FAX_TYPES res, void *context, 
                                  fax_store_handle_t store, fax_app_subjob_callback_t callback )
{
  uint32_t job_id;
  int ret;
 
  /* try to send the job */
  if(!(ret = faxer_start_jpeg_send(fax_service_handle, digits, color, res, store, &job_id)))
  {
    _faxer_info.job_id = job_id;
    _faxer_info.type = FAX_APP_SEND;
    _faxer_info.context = context;
    _faxer_info.jpeg = 1;
    _faxer_info.store = store;
    _faxer_info.update_callback = callback;
    if(store) _faxer_info.data_set = 1;
    _update_status(FAX_SUBJOB_ACTIVE, FAX_APP_FAX_STATE_DIALING);
  }
  else
    ret = 1;
  return(ret);
}
  
/*---------------------------------------------*/
int fax_app_faxer_cancel(void *context)
{
  faxer_terminate_fax(fax_service_handle);
  return(0);
}

/*---------------------------------------------*/
int fax_app_faxer_clear_files(uint32_t job_id)
{
  /* clear fax basicly clears the log of any files that were created/used for fax */
  faxer_clear_fax(fax_service_handle, job_id, true); 
  return(0);
}

/*---------------------------------------------*/
int fax_app_faxer_set_data(bool jpeg, void *data, 
                           uint16_t pages, bool data_complete)
{
   dbg_printf("Setting data, jpeg = %d data = %p\n", jpeg, data);
   faxer_send_set_data(fax_service_handle, _faxer_info.job_id, jpeg, data, pages, data_complete);
   return(0);
}

/*---------------------------------------------*/
int fax_app_faxer_notify_page_added(bool last_page)
{
  int ret = 0;
  dbg_printf(">>FAXAPP: faxer notify page added\n");
  ret = (faxer_send_add_page(fax_service_handle, _faxer_info.job_id, last_page)); 
  if(ret != 0) _reset_info();
  return(ret);
}

/*---------------------------------------------*/
char *fax_app_faxer_get_filename(uint32_t job_id)
{
  /* TODO: fix this */
  static FAXER_ACTIVITY_ENTRY job_log;
  if(!faxer_get_log(fax_service_handle, job_id, &job_log))
  {
    return(job_log.fname);
  }
  return(NULL);
}

/*---------------------------------------------*/
int fax_app_faxer_check_color_support(uint32_t job_id)
{
  int res;
  faxer_get_session_data(fax_service_handle, FAXER_SESSION_INT_COLOR_SUPPORTED, &res);
  return(res);
}

/*---------------------------------------------*/
int fax_app_faxer_check_jpeg_support(uint32_t job_id)
{
  int res;
  faxer_get_session_data(fax_service_handle, FAXER_SESSION_INT_JPEG_SUPPORTED, &res);
  return(res);
}

/*---------------------------------------------*/
int fax_app_faxer_check_jpeg_resolution(uint32_t job_id, FAX_TYPES req_res)
{
  int res;
  int ok = 1;

  /* check to make sure that the receiver supports 300x300 if we are sending photo.
     receivers are required to support 200x200 so no need to check that */
  faxer_get_session_data(fax_service_handle, FAXER_SESSION_INT_RESOLUTIONS, &res);
  if(req_res == FAX_TYPE_PHOTO)
  {
    if(!(res & 0x40)) 
    {
      ok = 0; 
    }
    dbg_printf("Check resolution res = %d ok = %d\n", res, ok); 
  }
  return(ok);
}

/*---------------------------------------------*/
int fax_app_faxer_free_storage(int space_required)
{
  int ok = 1;

  if(!faxer_free_storage(fax_service_handle, space_required)) ok = 0;
  return(ok);
}

/*---------------------------------------------*/
int fax_app_faxer_clear_print_pending(uint32_t job_id)
{
  uint16_t act = 0;
  int ok = 1;

  if(!faxer_pending_action(fax_service_handle, job_id, &act, 0xffff)) ok = 0; 
  return(ok);
}


/******************
* Settings 
******************/
fax_app_res_t fax_app_get_setting(FAXER_SETTING setting, void *buf, int buf_size)
{
  faxer_config_get_setting(fax_service_handle, setting, buf, buf_size);
  return(FAX_APP_RES_SUCCESS);
}

fax_app_res_t fax_app_set_setting(FAXER_SETTING setting, void *val)
{
  faxer_config_set_setting(fax_service_handle, setting, val);
  return(FAX_APP_RES_SUCCESS);
}

fax_app_res_t fax_app_restore_settings(void)
{
  return(FAX_APP_RES_SUCCESS);
}
