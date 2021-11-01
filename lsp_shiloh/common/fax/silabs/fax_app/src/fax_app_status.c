/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**\file fax_app_status.c 
 * This file contains the status functionality for the general scan app that handles 
 * scan and receive.  
*/
#include <string.h>
#include <dprintf.h>

#include "logger.h"
#include "agRouter.h"
#include "lassert.h"
#include "fax_app_api.h"
#include "fax_app_prv.h"
#include "utils.h"
#include "mlist.h"
#include "response_subject.h"
#include "fax_app_api.h"

typedef struct 
{
    Response_Subject_T_BaseClassMembers;
    fax_app_context_id_t id;
    fax_app_state_t state;
    fax_app_fax_info_t fax_info;
} context_status_t;

/* single status structure for notification, data is modified each time we notify.
   No need for protection since this is only used by the fax app thread. */
static context_status_t _context_status; 


#if 0
/* multiple data, one for each possible active context */
typedef struct
{ 
  context_status_data_t data:;
  uint8_t  in_use;
} status_list_item_t;
/* one status for each possible context instance, this allows the caller (context manager) to
   update only the required fields each time since the status is owned by the given 
   context for the life of that context */
static status_list_item_t _status_list[MAX_FAX_INSTANCES];

#endif


/***************************************************/
void fax_app_status_initialize(void)
{
 /* intitial values just in case */
  _context_status.id = FAX_APP_CONTEXT_INVALID_ID;
  _context_status.state = FAX_APP_STATE_NONE;
  _context_status.fax_info.state = FAX_APP_FAX_STATE_NONE;
  _context_status.fax_info.completion_error = FAXER_ACT_STATUS_NULL;
  _context_status.fax_info.cur_page = 0;
  _context_status.subject_observer_oid_key = FAXAPP_ContextStatus_ID;
  MInitList(&_context_status.observers);
};


/*********************************************
* Context Status
*********************************************/
/*---------------------------------------*/
void fax_app_AttachObserver_contextStatus(Observer_t *o)
{
    ATTACH_OBSERVER( _context_status.observers, o );
};

/*---------------------------------------*/
void fax_app_DetachObserver_contextStatus(Observer_t *o)
{
    DETACH_OBSERVER( _context_status.observers, o );
};

/* get info for a given notification */
/*---------------------------------------*/
fax_app_context_id_t fax_app_get_context_id(Subject_t *s)
{
  ASSERT(VALIDATE_OBJECT_KEY( s, context_status_t, 
                                subject_observer_oid_key, FAXAPP_ContextStatus_ID) );

  return(_context_status.id);
}

/*---------------------------------------*/
fax_app_state_t         fax_app_get_context_state(Subject_t *s)
{
  ASSERT(VALIDATE_OBJECT_KEY( s, context_status_t, 
                              subject_observer_oid_key, FAXAPP_ContextStatus_ID) );

  return(_context_status.state);
}

/*---------------------------------------*/
fax_app_fax_sub_state_t fax_app_get_context_faxSubState(Subject_t *s)
{
  ASSERT(VALIDATE_OBJECT_KEY( s, context_status_t, 
                                subject_observer_oid_key, FAXAPP_ContextStatus_ID) );

  return(_context_status.fax_info.state);
}

/*---------------------------------------*/
fax_app_comp_error_t fax_app_get_context_faxCompletionError(Subject_t *s)
{
  ASSERT(VALIDATE_OBJECT_KEY( s, context_status_t, 
                                subject_observer_oid_key, FAXAPP_ContextStatus_ID) );

  return(_context_status.fax_info.completion_error);
}

/*---------------------------------------*/
uint8_t fax_app_get_context_faxCurrentPage(Subject_t *s)
{
  return(_context_status.fax_info.cur_page);
}


/****************************************************
** Internal Status functions                        
****************************************************/


/***************************************************/
void *fax_app_status_create(fax_app_context_id_t id)
{
  return((void*)1);
}

/***************************************************/
void fax_app_status_destroy(void * handle)
{
}


/***************************************************/
void fax_app_status_update(void * handle, fax_app_context_status_data_t *data)
{
  context_status_t *status;
  _context_status.id    = data->context_id;
  _context_status.state = data->state;
  memcpy(&_context_status.fax_info, &data->fax_info, sizeof(_context_status.fax_info));
  /* notify */
  status = &_context_status;
  NOTIFY_OBSERVERS( status, observers);
}

