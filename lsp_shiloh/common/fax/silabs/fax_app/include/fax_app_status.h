/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/**\file fax_app_status.h
 * The file defines the fax app status API.  This is impemented using the observer
 * API.
 */
#ifndef __FAX_APP_STATUS_H__
#define __FAX_APP_STATUS_H__

#include "event_observer.h"
#include "fax_modem_status.h" /* modem status (line status, caller id, etc), exists in fax_svc module */
#include "faxer_api.h"

/*********************************************
* Context/Job Status 
* 
* Note that there can be more than one active 
* context/job at any given time.  For example,
* A fax could be printing while another one is
* being received.
**********************************************/ 
#define FAX_APP_CONTEXT_INVALID_ID (0xffffffff)
/* context id to keep track of multiple instances */
typedef uint32_t fax_app_context_id_t;

typedef enum
{
  FAX_APP_STATE_NONE = 0,
  FAX_APP_STATE_SENDING,
  FAX_APP_STATE_RECEIVING_RAW_DATA, /* for raw jobs that are sending data before starting the fax send */
  FAX_APP_STATE_SCANNING_SENDING, /* for job that are not scan first jobs such as color */
  FAX_APP_STATE_RECEIVING,
  FAX_APP_STATE_RECEIVING_PRINTING, /* some jobs will print as they receive such as color */ 
  FAX_APP_STATE_SCANNING,
  FAX_APP_STATE_PRINTING,
  FAX_APP_STATE_CANCELING,
  FAX_APP_STATE_CANCELED,
  FAX_APP_STATE_COMPLETE,
} fax_app_state_t;

typedef enum
{
  FAX_APP_FAX_STATE_NONE,
  FAX_APP_FAX_STATE_DIALING,
  FAX_APP_FAX_STATE_NEGOTIATING,
  FAX_APP_FAX_STATE_SENDING,
  FAX_APP_FAX_STATE_RECEIVING,
  FAX_APP_FAX_STATE_COMPLETE,
} fax_app_fax_sub_state_t;

#if 0 /* switched to use FAXER_ACT_STATUS to minimize remapping */
typedef enum
{
  FAX_APP_COMP_ERROR_NONE = 0,
  FAX_APP_COMP_ERROR_BUSY,
  FAX_APP_COMP_ERROR_NO_ANSWER,
  FAX_APP_COMP_ERROR_COMM,
  FAX_APP_COMP_ERROR_MEMORY_FULL,
  FAX_APP_COMP_ERROR_CANCELED,
} fax_app_comp_error_t;
#else
  typedef FAXER_ACT_STATUS fax_app_comp_error_t;
#endif

#define FAXAPP_ContextStatus_ID   0x321 /* uid? */
void fax_app_AttachObserver_contextStatus(Observer_t *o);
void fax_app_DetachObserver_contextStatus(Observer_t *o);
/* get info for a given notification */
fax_app_context_id_t    fax_app_get_context_id(Subject_t *s);
fax_app_state_t         fax_app_get_context_state(Subject_t *s);
/* - fax subinfo - */
fax_app_fax_sub_state_t fax_app_get_context_faxSubState(Subject_t *s);
fax_app_comp_error_t    fax_app_get_context_faxCompletionError(Subject_t *s);
uint8_t                 fax_app_get_context_faxCurrentPage(Subject_t *s);


#endif /* __FAX_APP_API__ */
