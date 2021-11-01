/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**\file fax_app_context.c 
 * This file contains context management.  It handles creating, managing through all 
 * states, and proper status management. This is the highest level state machine
 * managing the job.
 *
 * All changes to the context are made within the protection of this thread.
*/
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ATypes.h>

#include "dprintf.h"
#include "posix_ostools.h"
#include "fax_app_api.h"
#include "fax_app_prv.h"
#include "fax_app_raw.h"
#include "lassert.h"
#include "utils.h"
#include "memAPI.h"

#define SEND_FILENAME FAXER_STORAGE_DRIVE"/send.dat"
#define FAXAPPCONTEXT_STACK_SIZE (MAX((1024 * 10), POSIX_MIN_STACK_SIZE))

typedef struct 
{
  char fname[50];
  fax_store_handle_t store;
} fax_data_t;

typedef struct
{
  fax_data_t data;
  bool new_page;
  uint16_t cur_page;
  uint16_t pages_completed;
} scan_info_t;

typedef struct
{
  fax_app_context_id_t id; 
  fax_app_state_t state; /* superset of the subjob states */
  fax_context_type_t  type;
  fax_app_ticket_t    ticket;
  void *status;
  /* subjob management, state information is mostly for debug */
  fax_subjob_state_t  faxer_state; 
  fax_data_t          faxer_data;
  fax_app_fax_info_t  fax_info;
  uint16_t            fax_send_page; /* page number last sent to fax */
  fax_subjob_state_t  scan_state;
  scan_info_t         scan_info;
  fax_data_t          scan_data;
  uint16_t            print_page; /* page number last sent to print */
  fax_subjob_state_t  print_state;  
  /* waiting on more pages from external source */
  bool                waiting_on_raw_pages;
  uint32_t            raw_pages_received;
  /* status data to ensure we update when needed */
  fax_app_context_status_data_t status_data;
} fax_context_t;


typedef struct context_list_item_st context_list_item_t;
struct context_list_item_st
{
  fax_context_t instance;  /* instance must be first entry in the struct since it is 
                              the base class passed around and expanded to the list item when 
                              needed */
  uint8_t in_use;
  context_list_item_t *next;
  context_list_item_t *prev;
};

typedef struct
{
  __attribute__ ((aligned (8))) char stack[FAXAPPCONTEXT_STACK_SIZE]; /* stack size */
  mqd_t  queue;
  pthread_t pthread_tid;
} _px_thread_data_t;

static _px_thread_data_t _px_thread_data;
#define QUEUE_SIZE (20)

typedef enum
{
  FAX_MSG_START_RECV,
  FAX_MSG_START_SEND,
  FAX_MSG_START_REPRINT,
  FAX_MSG_END_ALL,
  FAX_MSG_FAX_SUBJOB_UPDATE,
  FAX_MSG_PRINT_SUBJOB_UPDATE,
  FAX_MSG_SCAN_SUBJOB_UPDATE,
  FAX_MSG_ADD_RAW_PAGE,
} _msg_type_t; 

typedef union
{
  fax_app_ticket_t    ticket;      
  fax_subjob_update_t subjob_update;
  fax_app_ext_page_data_t page;
  uint32_t job_id;
} _msg_data_t;

typedef struct
{
  _msg_type_t type;
  _msg_data_t data;
  bool used;
} _msg_t;

#define _ID_TO_CONTEXT_ITEM(_x)((fax_context_t*)&_context_list[_x])
static context_list_item_t _context_list[MAX_FAX_INSTANCES];
static context_list_item_t *_head;
static int _num_items;

static fax_app_res_t _print_subjob_callback(fax_subjob_update_t *update);
static fax_app_res_t _scan_subjob_callback(fax_subjob_update_t *update);
static fax_app_res_t _faxer_subjob_callback(fax_subjob_update_t *update);

static void* _entry(void* unused);

static void _update_overall_state(fax_context_t *context);
static void _context_thread_cancel_context(fax_context_t *context);


/**************************************************/
/** \brief Intialize the context library within fax app  */
void fax_app_context_initialize()
{ 
  /* create queue */
  posix_create_message_queue(&_px_thread_data.queue, "/faxappQ", QUEUE_SIZE, sizeof(_msg_t*));

  /* create thread for fax context management */
  posix_create_thread(&_px_thread_data.pthread_tid, _entry, (void *) 1, "faxapp", 
                      &_px_thread_data.stack, sizeof(_px_thread_data.stack),
                      POSIX_THR_PRI_NORMAL);
}


/**************************************************/
/**\brief Fax thread and the helper functions     */

/*------------------------------------------------*/
#define ALL_SUBJOBS_DONE(_inst)((_inst->faxer_state == FAX_SUBJOB_CANCELED || \
                                      _inst->faxer_state == FAX_SUBJOB_COMPLETE  || \
                                      _inst->faxer_state == FAX_SUBJOB_IDLE) && \
                                     (_inst->print_state == FAX_SUBJOB_CANCELED || \
                                      _inst->print_state == FAX_SUBJOB_COMPLETE  || \
                                      _inst->print_state == FAX_SUBJOB_IDLE) && \
                                     (_inst->scan_state == FAX_SUBJOB_CANCELED || \
                                      _inst->scan_state == FAX_SUBJOB_COMPLETE  || \
                                      _inst->scan_state == FAX_SUBJOB_IDLE))
/*-----------------------------------------------*/
/** \brief Add new context instance to the list.  Returns the handle to new context, invalid otherwise. */
static fax_context_t *_add_new_context(fax_context_t *context)
{
  fax_context_t *ret = NULL;
  int i;

  if(_num_items == MAX_FAX_INSTANCES)
    return(ret);

  if(!_head)
  {
    _head = _context_list;
    _head->in_use = 1;
    _head->prev = NULL; 
    _head->next = NULL; 
    ret = &_head->instance;
    ret->id = 0;
  }
  else
  {
    for(i = 0; i<ARRAY_SIZE(_context_list);i++)
    {
      if(!_context_list[i].in_use) 
      {
        _context_list[i].in_use = 1;
        _context_list[i].next = _head; 
        _context_list[i].prev = NULL; 
        _head->prev = &_context_list[i];
        _head = &_context_list[i];
        ret = &_context_list[i].instance;
        ret->id = i;
        break;
      }
    }
  }
  ASSERT(_head && ret);
  if(ret)
  {
    _num_items++;
    memcpy(ret, context, sizeof(fax_context_t));
    dbg_printf("FAXAPP: Added context, head = 0x%08x\n", _head);
  }
  else
  {
    dbg_printf("FAXAPP: Added context failed\n");
  }
  return(ret);
}

/*-----------------------------------------------*/
/** \brief Remove context instance from list and return to pool.  */
static void _remove_context(fax_context_t *context)
{
  context_list_item_t *context_item = (context_list_item_t*)context;
  /* handle is index into array */
  ASSERT(context_item->in_use);
  if(context_item == _head)
  {
    _head = context_item->next; 
    if(_head) _head->prev = 0;
  }
  else 
  {
    if(context_item->next) context_item->next->prev = context_item->prev; 
    if(context_item->prev) context_item->prev->next = context_item->next; 
  }
  context_item->in_use = 0;
  _num_items--;
  dbg_printf("FAXAPP: Removed context, head = 0x%08x\n", _head);
  if(_head != NULL)
  {
    dbg_printf("- head info: state = %d, fax state = %d, print state = %d, scan state = %d\n", 
               _head->instance.state, _head->instance.faxer_state, _head->instance.print_state, 
               _head->instance.scan_state);
  }
}

/*-----------------------------------------------*/
/* *\brief Create and intialize a fax app context */
fax_context_t *fax_app_context_create(fax_context_type_t type, fax_app_ticket_t *ticket)
{
  fax_context_t *ret;
  fax_context_t context;  
  memset(&context, 0, sizeof(context));
  /* initialize a context */       
  context.state = FAX_APP_STATE_NONE;
  context.type = type;
  memcpy(&context.ticket, ticket, sizeof(fax_app_ticket_t));
  context.faxer_state = FAX_SUBJOB_IDLE; 
  context.scan_state  = FAX_SUBJOB_IDLE; 
  context.print_state = FAX_SUBJOB_IDLE; 
  /* add to the list */
  ret = _add_new_context(&context);
  if(ret)
  {
    dbg_printf("FAXAPP: Created context %p\n", ret);
    ret->status = fax_app_status_create(ret->id); 
  }
  else dbg_printf("FAXAPP: Failed to create context\n");
  return(ret);
}

/*-----------------------------------------------*/
/** \brief Destroy a fax app context */
fax_app_res_t fax_app_context_destroy(fax_context_t *context)
{
  /* destroy anything that is needed, remove from list.  */

  if(context->faxer_state != FAX_SUBJOB_IDLE)
  {
    /* leave receive data in case reprint is desired */
    if(context->type == FAX_APP_SEND)
    {
      fax_app_faxer_clear_files(context->fax_info.job_id);
    }
  }
  if(context->scan_data.store)
  {
    fax_store_destroy(context->scan_data.store);
    context->scan_data.store = 0;
  }
  /* check to make sure our scan file does not exist anymore since it is out of our 
     control as to what the fax svc does with the file (copies, moves, etc) */
  unlink(context->scan_data.fname);
  if(context->faxer_data.store)
  {
    fax_store_destroy(context->faxer_data.store);
    context->scan_data.store = 0;
  }
  fax_app_status_destroy(context->status);
  context->id = FAX_APP_CONTEXT_INVALID_ID;
  _remove_context(context);
  return(FAX_APP_RES_SUCCESS);  
}

/*-----------------------------------------------*/
static fax_context_t * _find_context_waiting_on_raw_pages(void)
{
  context_list_item_t *item = _head;
  /* look for a queued print job and start it */
  while(item)
  {
    if(item->in_use && item->instance.waiting_on_raw_pages)
    {
      return( &item->instance );
    } 
    item = item->next;
  } 
  dbg_printf("FAXAPP: no jobs waiting on RAW data found!\n");
  return( NULL );
}

/*-----------------------------------------------*/
static void _start_print_job(fax_context_t *context)
{
  struct stat st;
  dbg_printf("\nFAXAPP: starting print for context = %p\n", context); 
  /* currently existance of file name is how we determine color vs mono for receive */
  if(!stat(context->faxer_data.fname, &st))
  {
    fax_app_print_start(context->faxer_data.fname, context, context->fax_info.job_id, _print_subjob_callback,
                        context->fax_info.pages_completed,
                        context->fax_info.state==FAX_APP_FAX_STATE_COMPLETE); 
  }
  else
  {
    fax_app_print_start_jpeg(context->faxer_data.store, context, _print_subjob_callback,
                            context->fax_info.pages_completed,
                            context->fax_info.state==FAX_APP_FAX_STATE_COMPLETE); 
  }
  /* record print pages sent to print */
  context->print_page = context->fax_info.pages_completed;

  context->print_state = FAX_SUBJOB_QUEUED; 
}

/*-----------------------------------------------*/
void _start_queued_print_job(fax_context_t *context)
{
  context_list_item_t *item = _head;
  /* look for a queued print job and start it */
  while(item)
  {
    if(item->instance.print_state == FAX_SUBJOB_QUEUED)
    {
      _start_print_job(&item->instance);
      //_update_overall_state(context); //shouldn't be needed, although wouldn't harm anything  
      return;
    } 
    item = item->next;
  } 
  dbg_printf("FAXAPP: no queued print job found.\n");
}

/*-----------------------------------------------*/
static void _invalid_state(fax_context_t *context)
{
  /* assert for now */
  ASSERT(0);
}

/*-----------------------------------------------*/
static void _update_status(fax_context_t *context)
{
  /* update status if things have changed */
  if(memcmp(&context->status_data.fax_info, &context->fax_info, sizeof(context->fax_info)) ||
    context->status_data.state != context->state) 
  {
    context->status_data.context_id = context->id;
    context->status_data.state      = context->state;
    context->status_data.fax_info   = context->fax_info;
    fax_app_status_update(context->status, &context->status_data);
  }
}


/*-----------------------------------------------*/
static void _send_update_sending(fax_context_t *context)
{
  if(context->faxer_state == FAX_SUBJOB_COMPLETE)
  {
    context->state = FAX_APP_STATE_COMPLETE;
    _update_overall_state(context);
  }
}

/*-----------------------------------------------*/
static void _receving_raw_update(fax_context_t *context)
{
  /* nothing to do */
}

/*-----------------------------------------------*/
static void _send_update_scanning_sending(fax_context_t *context)
{
   /* both scan and fax completed */ 
   if(context->scan_state == FAX_SUBJOB_COMPLETE &&
      context->faxer_state == FAX_SUBJOB_COMPLETE)
   {
     context->state = FAX_APP_STATE_COMPLETE;
     _update_overall_state(context);
   }
   /* subjob canceled, cancel context */
   else if(context->scan_state == FAX_SUBJOB_CANCELING ||
          context->scan_state == FAX_SUBJOB_CANCELED  ||
          context->faxer_state == FAX_SUBJOB_CANCELING ||
          context->faxer_state == FAX_SUBJOB_CANCELED)
   {
     _context_thread_cancel_context(context);
   }
   /* fax and scan are active */
   else if((context->faxer_state == FAX_SUBJOB_ACTIVE || 
           (context->faxer_state == FAX_SUBJOB_WAITING_ON_DATA)) 
           && (context->scan_state != FAX_SUBJOB_IDLE))
   {
     bool complete = (context->scan_state==FAX_SUBJOB_COMPLETE);
     dbg_printf("FAXAPP: scan pages completd = %d support color = %d support jpeg = %d complete = %d\n", 
                context->scan_info.pages_completed, 
                fax_app_faxer_check_color_support(context->fax_info.job_id), 
                fax_app_faxer_check_jpeg_support(context->fax_info.job_id), complete);  
     if((context->scan_info.pages_completed == 1) && (context->fax_send_page == 0))
     {
       bool complete = (context->scan_state==FAX_SUBJOB_COMPLETE);
       if(context->ticket.type == FAX_TYPE_COLOR && 
          fax_app_faxer_check_color_support(context->fax_info.job_id))
       {
         fax_app_faxer_set_data(true, context->scan_data.store,     
                                1 /*pages*/, complete/*data complete*/);
       }
       else if(context->ticket.type == FAX_TYPE_GRAY &&
               fax_app_faxer_check_jpeg_support(context->fax_info.job_id))
       {
         fax_app_faxer_set_data(true, context->scan_data.store,     
                                1 /*pages*/, complete/*data complete*/);
       }
       else
       {
         fax_app_faxer_set_data(false, context->scan_data.fname,     
                                1 /*pages*/, complete/*data complete*/);
       }
       context->fax_send_page++;
     }
     else if(context->fax_send_page < context->scan_info.pages_completed)
     {
       /* notify of page added */     
       fax_app_faxer_notify_page_added(complete);
       context->fax_send_page++;
       /* we should never get here with multiple new pages */
       ASSERT(context->fax_send_page == context->scan_info.pages_completed);
     }
     if(context->faxer_state == FAX_SUBJOB_COMPLETE)
     {
       fax_app_print_notify_document_completed();
     }
   }  
   else if(context->faxer_state == FAX_SUBJOB_COMPLETE && 
           context->scan_state == FAX_SUBJOB_IDLE)
   {
      /* fax subjob ended before scan started, end job */
      _context_thread_cancel_context(context);
   }
   else if(context->faxer_state == FAX_SUBJOB_COMPLETE &&
           context->scan_state != FAX_SUBJOB_COMPLETE)
           
   {
     _context_thread_cancel_context(context);
   }
   else if((context->faxer_state == FAX_SUBJOB_WAITING_ON_DATA) && 
           (context->scan_state != FAX_SUBJOB_ACTIVE))
   {
     /* create data, start scan */
     if(context->ticket.type == FAX_TYPE_COLOR && fax_app_faxer_check_color_support(context->fax_info.job_id))
     {
        fax_store_creation_t params;
        memset(&params, 0, sizeof(params));
        params.type = FAX_STORE_TYPE_RAMFS;
        
        context->scan_data.store = fax_store_create(&params);
        ASSERT(context->scan_data.store);

        /* make sure the res is supported */
        if(context->ticket.resolution == FAX_TYPE_PHOTO)
        {
          int res_ok;
          res_ok = fax_app_faxer_check_jpeg_resolution(context->fax_info.job_id, context->ticket.resolution); 
          if(!res_ok)
          {
             dbg_printf("FAXAPP: Receiver does not support 300x300 res, DOWNGRADING to 200x200\n");
             context->ticket.resolution = FAX_TYPE_STANDARD;
          }
        }
        fax_app_scan_start_color(context->scan_data.store, context->ticket.brightness, context->ticket.resolution, 
                                 context, _scan_subjob_callback);
     }
     else if(context->ticket.type == FAX_TYPE_GRAY && fax_app_faxer_check_jpeg_support(context->fax_info.job_id))
     {
        fax_store_creation_t params;
        memset(&params, 0, sizeof(params));
        params.type = FAX_STORE_TYPE_RAMFS;
        
        context->scan_data.store = fax_store_create(&params);
        ASSERT(context->scan_data.store);

        /* make sure the res is supported */
        if(context->ticket.resolution == FAX_TYPE_PHOTO)
        {
          int res_ok;
          res_ok = fax_app_faxer_check_jpeg_resolution(context->fax_info.job_id, context->ticket.resolution); 
          if(!res_ok) 
          {
             dbg_printf("FAXAPP: Receiver does not support 300x300 res, DOWNGRADING to 200x200\n");
             context->ticket.resolution = FAX_TYPE_STANDARD;
          }
        }
        fax_app_scan_start_gray(context->scan_data.store, context->ticket.brightness, context->ticket.resolution, 
                                context, 
                                _scan_subjob_callback);
     }
     else
     {     
        strncpy(context->scan_data.fname, SEND_FILENAME, sizeof(context->scan_data.fname));
        fax_app_scan_start_bw(context->scan_data.fname, context->ticket.brightness, context->ticket.resolution, 
                              context, _scan_subjob_callback);
     }
   }
}


/*-----------------------------------------------*/
static void _recv_update_receiving(fax_context_t *context)
{
   if(context->faxer_state == FAX_SUBJOB_COMPLETE)
   {
      if(!test_mode_no_print_on)
      {
        _start_print_job(context);
        context->state = FAX_APP_STATE_PRINTING;
        _update_overall_state(context);
      }
      else
      {
        dbg_printf("\nFAXAPP: received but no print test mode is on\n"); 
        context->state = FAX_APP_STATE_COMPLETE;
        _update_overall_state(context);
      }
   } 
   else if(context->faxer_state == FAX_SUBJOB_CANCELING ||
           context->faxer_state == FAX_SUBJOB_CANCELED)
   {
     _context_thread_cancel_context(context);
   }
}

/*-----------------------------------------------*/
static void _recv_update_receiving_printing(fax_context_t *context)
{
   /* both print and fax completed */ 
   if(context->print_state == FAX_SUBJOB_COMPLETE &&
      context->faxer_state == FAX_SUBJOB_COMPLETE)
   {
     context->state = FAX_APP_STATE_COMPLETE;
     _update_overall_state(context);
   }
   /* subjob canceled, cancel context */
   else if(context->print_state == FAX_SUBJOB_CANCELING ||
          context->print_state == FAX_SUBJOB_CANCELED  ||
          context->faxer_state == FAX_SUBJOB_CANCELING ||
          context->faxer_state == FAX_SUBJOB_CANCELED)
   {
     _context_thread_cancel_context(context);
   }
   /* Start print for multi page receive after first page received */
   else if(context->fax_info.pages_completed>0 &&
           context->print_state != FAX_SUBJOB_QUEUED &&
           context->print_state != FAX_SUBJOB_ACTIVE)
   {
     _start_print_job(context);
   }
   /* Start print for single page receive */
   else if(context->fax_info.pages_completed ==1 &&
           context->faxer_state == FAX_SUBJOB_COMPLETE &&
           context->print_state != FAX_SUBJOB_QUEUED &&
           context->print_state != FAX_SUBJOB_ACTIVE)
   {
     _start_print_job(context);
   }
   /* Fax completed, but no pages exist.  End job. */ 
   else if(context->faxer_state == FAX_SUBJOB_COMPLETE &&
           context->print_state != FAX_SUBJOB_QUEUED &&
           context->print_state != FAX_SUBJOB_ACTIVE)
   {
     context->state = FAX_APP_STATE_COMPLETE;
     _update_overall_state(context);
   }
   /* print is active, notify of new pages as they come */
   else if(context->print_state == FAX_SUBJOB_ACTIVE)
   {
     if(context->print_page < context->fax_info.pages_completed)
     {
       /* notify of page added */     
       fax_app_print_notify_page_added();
       context->print_page++;
       /* we should never get here with multiple new pages */
       ASSERT(context->print_page == context->fax_info.pages_completed);
     }
     if(context->faxer_state == FAX_SUBJOB_COMPLETE)
     {
       fax_app_print_notify_document_completed();
     }
   }  
}

/*-----------------------------------------------*/
static void _send_update_scanning(fax_context_t *context)
{
   /* tansition to sending if the scan is complete */
   if(context->scan_state == FAX_SUBJOB_COMPLETE)
   {
     if((context->ticket.type == FAX_TYPE_COLOR) || (context->ticket.type == FAX_TYPE_GRAY))
     {
       /* try to send the job */
       if(fax_app_faxer_start_jpeg_send(context->ticket.digits, 
                                        (context->ticket.type == FAX_TYPE_COLOR),
                                        context->ticket.resolution,
                                        context, context->scan_data.store, 
                                        _faxer_subjob_callback) == 0)
       {
         context->faxer_state = FAX_SUBJOB_QUEUED;
         context->state = FAX_APP_STATE_SENDING;
       }
       else
       {
         _context_thread_cancel_context( context ); 
         return;
       }
     }
     else
     {
       /* try to send the job */
       if(fax_app_faxer_send(context->ticket.digits, context->scan_data.fname, 
                             context->ticket.resolution, context, 
                             _faxer_subjob_callback) == 0)
       {
         context->faxer_state = FAX_SUBJOB_QUEUED;
         context->state = FAX_APP_STATE_SENDING;
         unlink(context->scan_data.fname);
       }
       else
       {
         _context_thread_cancel_context(context);
         return;
       }
     }
   }
  else if(context->scan_state == FAX_SUBJOB_CANCELING ||
          context->scan_state == FAX_SUBJOB_CANCELED)
  {
    _context_thread_cancel_context(context);
    return;
  }
}

/*-----------------------------------------------*/
static void _recv_update_printing(fax_context_t *context)
{
  /* final phase, once complete mark the job completed */
  if(context->print_state == FAX_SUBJOB_COMPLETE)
  {
    context->state = FAX_APP_STATE_COMPLETE;
    _update_overall_state(context);
  } 
  else if(context->print_state == FAX_SUBJOB_CANCELING ||
          context->print_state == FAX_SUBJOB_CANCELED)
  {
    _context_thread_cancel_context(context);
  }
}

/*-----------------------------------------------*/
static void _update_canceling(fax_context_t *context)
{
  dbg_printf("CANCELING update: fax substate = %d scan substate = %d print substate = %d\n", 
             context->faxer_state, context->scan_state, context->print_state); 
  if(ALL_SUBJOBS_DONE(context))
  {
    context->state = FAX_APP_STATE_CANCELED;
    _update_overall_state(context);
  } 
}

/*-----------------------------------------------*/
static void _update_complete(fax_context_t *context)
{
  fax_context_type_t type = context->type;
  ASSERT(ALL_SUBJOBS_DONE(context));
  dbg_printf("\nFAXAPP: complete update for context = %p\n", context); 
  _update_status(context);
  if(type == FAX_APP_RECV)
  {
    fax_app_faxer_clear_print_pending(context->fax_info.job_id);
  }

  /* destroy the context */
  fax_app_context_destroy(context); 

  if(type == FAX_APP_RECV)
  {
    /* check if we need to start a queued print job */
    _start_queued_print_job(context);
  } 
}

/*-----------------------------------------------*/
typedef struct
{
  fax_app_state_t state;    /* current state */
  void (*state_update)(fax_context_t *context);  /* update function */
} _state_tbl_t;


/* TODO Cancel can take place from within any of the subjobs, handle cancel substate change for
   all states. */
static _state_tbl_t _state_tbl[] = 
{
  {FAX_APP_STATE_NONE,               _invalid_state },
  {FAX_APP_STATE_SENDING,            _send_update_sending },
  {FAX_APP_STATE_RECEIVING_RAW_DATA, _receving_raw_update },
  {FAX_APP_STATE_SCANNING_SENDING,   _send_update_scanning_sending },
  {FAX_APP_STATE_RECEIVING,          _recv_update_receiving },
  {FAX_APP_STATE_RECEIVING_PRINTING, _recv_update_receiving_printing },
  {FAX_APP_STATE_SCANNING,           _send_update_scanning },
  {FAX_APP_STATE_PRINTING,           _recv_update_printing },
  {FAX_APP_STATE_CANCELING,          _update_canceling },
  {FAX_APP_STATE_CANCELED,           _update_complete },
  {FAX_APP_STATE_COMPLETE,           _update_complete },
};

/*-----------------------------------------------*/
static void _update_overall_state(fax_context_t *context)
{
  dbg_printf("FAXAPP: top level state change for context %p, new state = %d\n", context, context->state);
  ASSERT(_state_tbl[context->state].state == context->state);
  _state_tbl[context->state].state_update(context);
  /* update status */ 
  if(context->id != FAX_APP_CONTEXT_INVALID_ID)
  {
    _update_status(context);
  }
}

/*------------------------------------------------*/
static void _context_thread_fax_subjob_update(fax_context_t *context, 
                                         fax_subjob_update_t *subjob_update)
{
  /* update subjob and fax info, then let the top level state machine handle the rest */
  context->faxer_state  = subjob_update->state;
  context->fax_info.state = subjob_update->fax_substate;
  context->fax_info.completion_error = subjob_update->completion_error;
  context->fax_info.cur_page = subjob_update->active_page;
  context->fax_info.pages_completed = subjob_update->pages_completed;
  context->fax_info.job_id = subjob_update->job_id;
  if(context->type == FAX_APP_RECV && subjob_update->state == FAX_SUBJOB_ACTIVE)
  {
    char * fname = fax_app_faxer_get_filename(subjob_update->job_id);
    if(fname && !strlen(context->faxer_data.fname))
    {
      strncpy(context->faxer_data.fname, fname, sizeof(context->faxer_data.fname));
      dbg_printf("FAXAPP: fname retrieved from faxer, fname = |%s|\n", fname);
    }
    else
    {
      dbg_printf("FAXAPP: fname was null when retrieved from faxer.\n");
    }
  } 
  dbg_printf("FAXAPP: Faxer subjob update, state = %d substate = %d\n", context->faxer_state, context->fax_info.state);
  /* update context state/status */
  _update_overall_state(context);
}


/*------------------------------------------------*/
static void _context_thread_scan_subjob_update(fax_context_t *context, 
                                               fax_subjob_update_t *subjob_update)
{
  context->scan_state = subjob_update->state; 
  dbg_printf("FAXAPP: Scan subjob update, state = %d\n", context->scan_state);
  context->scan_info.cur_page = subjob_update->active_page;
  context->scan_info.pages_completed = subjob_update->pages_completed; 
  /* update context state/status */
  _update_overall_state(context);
}

/*------------------------------------------------*/
static void _context_thread_print_subjob_update(fax_context_t *context, 
                                           fax_subjob_update_t *subjob_update)
{
  context->print_state = subjob_update->state; 
  dbg_printf("FAXAPP: Print subjob update, state = %d\n", context->print_state);
  /* update context state/status */
  _update_overall_state(context);
}

/*------------------------------------------------*/
static void _context_thread_start_reprint_job(fax_context_t *context)
{
   context->print_state = FAX_SUBJOB_QUEUED;
   context->state = FAX_APP_STATE_PRINTING;
   dbg_printf("FAX_APP: context->fax_info.job_id:%d \n", context->fax_info.job_id);
   fax_app_print_start(context->ticket.src_filename, context, context->fax_info.job_id, _print_subjob_callback, 0, 1);
   _update_status(context);
}

/*------------------------------------------------*/
static void _context_thread_start_receive_job(fax_context_t *context)
{
   fax_store_creation_t params;
   memset(&params, 0, sizeof(params));
   params.type = FAX_STORE_TYPE_RAMFS;
        
   /* create context in case this is a color receive...it will be destroyed 
      when the context is destroyed either way */
   context->faxer_data.store = fax_store_create(&params);   

   if(fax_app_faxer_receive(context, context->faxer_data.store, _faxer_subjob_callback) == 0)
   {
      context->faxer_state = FAX_SUBJOB_QUEUED;
      if(test_mode_no_print_on)
        context->state = FAX_APP_STATE_RECEIVING;
      else
        context->state = FAX_APP_STATE_RECEIVING_PRINTING;
   }
   _update_status(context);
}

/*------------------------------------------------*/
static void _context_thread_start_send_job(fax_context_t *context)
{
  context->type = FAX_APP_SEND; 
  if((context->ticket.type == FAX_TYPE_COLOR) || (context->ticket.type == FAX_TYPE_GRAY))
  {
      if(context->ticket.src == FAX_SRC_FILE)
      {
        FILE *file;
        uint32_t bread;
        fax_store_pg_handle_t page;
        fax_store_creation_t params;
        uint8_t buf[100];
        memset(&params, 0, sizeof(params));
        params.type = FAX_STORE_TYPE_RAMFS;
        
        context->scan_data.store = fax_store_create(&params);
        page = fax_store_page_open_new(context->scan_data.store, FAX_STORE_MODE_WRITE);
        file = fopen(context->ticket.src_filename, "rb");
        if(page && file)
        {
          while((bread = fread(buf, 1, sizeof(buf), file)))   
          {
            fax_store_page_write_data(page, buf, bread, true); 
          }
          fclose(file);
          fax_store_page_close(page);
        
          /* add a paqe and copy the data from the file into the store */
          if(fax_app_faxer_start_jpeg_send(context->ticket.digits, 
                                            (context->ticket.type == FAX_TYPE_COLOR),
                                            context->ticket.resolution,
                                            context, context->scan_data.store, 
                                            _faxer_subjob_callback) == 0)
          {
            context->faxer_state = FAX_SUBJOB_QUEUED;
            context->state = FAX_APP_STATE_SENDING;
          }
          else
          {
            _context_thread_cancel_context( context );
            return;
          }
        }
      }
      else if(context->ticket.src == FAX_SRC_EXTERNAL)
      {
        if(context->ticket.scan_before_sending) 
        {
          fax_store_creation_t params;
          memset(&params, 0, sizeof(params));
          params.type = FAX_STORE_TYPE_RAMFS;
        
          context->scan_data.store = fax_store_create(&params);
          ASSERT(context->scan_data.store);

          if(fax_app_raw_jpeg_start( context->scan_data.store, context->ticket.resolution ))
          {
            _context_thread_cancel_context( context );
            return;
          }

          context->state = FAX_APP_STATE_RECEIVING_RAW_DATA;
        }
        else
        {
          /* add a paqe and copy the data from the file into the store */
          if(fax_app_faxer_start_jpeg_send(context->ticket.digits, 
                                         (context->ticket.type == FAX_TYPE_COLOR),
                                          context->ticket.resolution,
                                          context, NULL, 
                                          _faxer_subjob_callback) == 0)
           {
             /* create the store as if it were the scan store */
             fax_store_creation_t params;
             memset(&params, 0, sizeof(params));
             params.type = FAX_STORE_TYPE_RAMFS;
        
             context->scan_data.store = fax_store_create(&params);

             if(!context->scan_data.store)
             {
               _context_thread_cancel_context( context );
               return;
             }
             else
             {
               if(fax_app_raw_jpeg_start( context->scan_data.store, context->ticket.resolution ))
               {
                 _context_thread_cancel_context( context );
                 return;
               }
               else
               {
                 context->waiting_on_raw_pages = 1;
                 context->faxer_state = FAX_SUBJOB_QUEUED;
                 context->state = FAX_APP_STATE_SENDING;
               }
             }
           }
           else
           {
             _context_thread_cancel_context( context );
             return;
           }
        }
      }
      else
      {
        context->scan_info.new_page = 0;
        context->scan_info.cur_page = 0;
        context->scan_info.pages_completed = 0;
        context->fax_send_page = 0;
        /* force to scan after send just in case the UI doesn't do it, we will leave 
           support for scan before sending just in case */
        context->ticket.scan_before_sending = 0;
        if(context->ticket.scan_before_sending) 
        {
          /* DEAD branch for now */
          fax_store_creation_t params;
          memset(&params, 0, sizeof(params));
          params.type = FAX_STORE_TYPE_RAMFS;
        
          context->scan_data.store = fax_store_create(&params);
          ASSERT(context->scan_data.store);

          if(context->ticket.type == FAX_TYPE_COLOR)
          {
            fax_app_scan_start_color(context->scan_data.store, context->ticket.brightness, 
                                     context->ticket.resolution, context, 
                                     _scan_subjob_callback);
          }
          else
          {
            fax_app_scan_start_gray(context->scan_data.store, context->ticket.brightness, 
                                    context->ticket.resolution, context, 
                                    _scan_subjob_callback);
          }

          context->state = FAX_APP_STATE_SCANNING;
        }
        else
        {
          if(fax_app_faxer_start_jpeg_send(context->ticket.digits, 
                                            (context->ticket.type == FAX_TYPE_COLOR),
                                            context->ticket.resolution,
                                            context, NULL,
                                            _faxer_subjob_callback) == 0)
          {
          /* wait for negotiation before scanning, 
             set state to scanning_sending where the rest of the flow will be handled */
            context->state = FAX_APP_STATE_SCANNING_SENDING;
          }
          else
          {
            _context_thread_cancel_context( context );
            return;
          }
        }
      }
  }
  else if(context->ticket.type == FAX_TYPE_BW)
  {    
    if(context->ticket.src == FAX_SRC_FILE)
    {
      /* skip the scan state if we sending from file */ 
      /* try to send the job */
      if(fax_app_faxer_send(context->ticket.digits, context->ticket.src_filename, 
                            context->ticket.resolution, context, 
                           _faxer_subjob_callback) == 0)
      {
        context->faxer_state = FAX_SUBJOB_QUEUED;
        context->state = FAX_APP_STATE_SENDING;
      }
      else
      {
        _context_thread_cancel_context(context);
        return;
      }
    }
    else if(context->ticket.src == FAX_SRC_EXTERNAL)
    {
      if(context->ticket.scan_before_sending)
      {
        strncpy(context->scan_data.fname, SEND_FILENAME, sizeof(context->scan_data.fname));
        if(fax_app_raw_mono_start(context->scan_data.fname, context->ticket.resolution))
        {
            _context_thread_cancel_context( context );
            return;
        }
        context->waiting_on_raw_pages = 1;
        context->state = FAX_APP_STATE_RECEIVING_RAW_DATA;
      }
      else
      {
        if(fax_app_faxer_send(context->ticket.digits, NULL, 
                              context->ticket.resolution, context, 
                              _faxer_subjob_callback) == 0)
        {
          strncpy(context->scan_data.fname, SEND_FILENAME, sizeof(context->scan_data.fname));
          fax_app_raw_mono_start(context->scan_data.fname, context->ticket.resolution); 
          if(fax_app_raw_mono_start(context->scan_data.fname, context->ticket.resolution))
          {
              _context_thread_cancel_context( context );
              return;
          }
          context->waiting_on_raw_pages = 1;
          context->faxer_state = FAX_SUBJOB_QUEUED;
          context->state = FAX_APP_STATE_SENDING;
        }
        else
        {
          _context_thread_cancel_context(context);
          return;
        }
      }
    }
    else
    {
      if(context->ticket.scan_before_sending)
      {
        strncpy(context->scan_data.fname, SEND_FILENAME, sizeof(context->scan_data.fname));
        fax_app_scan_start_bw(context->scan_data.fname, context->ticket.brightness, 
                              context->ticket.resolution, 
                              context, _scan_subjob_callback);
        context->scan_state = FAX_SUBJOB_QUEUED;
        context->state = FAX_APP_STATE_SCANNING;
      }
      else
      {
        if(fax_app_faxer_send(context->ticket.digits, NULL, context->ticket.resolution,
                              context, 
                              _faxer_subjob_callback) == 0)
        {
          /* wait for negotiation before scanning, 
             set state to scanning_sending where the rest of the flow will be handled */
          context->state = FAX_APP_STATE_SCANNING_SENDING;
        }
        else
        {
          _context_thread_cancel_context(context);
          return;
        }
      }
    }
  }
  else
    ASSERT(0); /* invalid type */
  _update_status(context);
}


#define SUBJOB_ACTIVE(_state) ( _state != FAX_SUBJOB_COMPLETE  && \
                                _state != FAX_SUBJOB_IDLE && \
                                _state != FAX_SUBJOB_CANCELING && \
                                _state != FAX_SUBJOB_CANCELED) 

#define SUBJOB_CANCELING(_state) (_state == FAX_SUBJOB_CANCELING)

/*------------------------------------------------*/
static void _context_thread_cancel_context(fax_context_t *context)
{
  bool job_active = false;
  dbg_printf("FAX_APP: End context, fax state = %d scan state = %d.\n", context->faxer_state, context->scan_state);
  /* cancel all subjobs */
  if(SUBJOB_ACTIVE(context->faxer_state)) 
  {
    dbg_printf(" Faxer active \n ");
    fax_app_faxer_cancel(context);
    job_active = true;
  }

  if(SUBJOB_ACTIVE(context->scan_state))
  {
    dbg_printf(" Scan active \n ");
    fax_app_scan_cancel(context); 
    job_active = true;
  }

  if(SUBJOB_ACTIVE(context->print_state))
  {
    dbg_printf(" Print active \n ");
    fax_app_print_cancel(context); 
    job_active = true;
  }

  if(context->waiting_on_raw_pages)
  {
    fax_app_raw_cancel();
  }

  /* place into canceling state */
  if( job_active || 
      SUBJOB_CANCELING(context->faxer_state) ||
      SUBJOB_CANCELING(context->scan_state) ||
      SUBJOB_CANCELING(context->print_state) )
  {
    context->state = FAX_APP_STATE_CANCELING;
    _update_status(context);
  }
  else
  {
    context->state = FAX_APP_STATE_CANCELED;
    _update_overall_state(context);
  }
}

/*------------------------------------------------*/
static void _context_thread_end_all(void)
{
  int i;
  /* Cancel all active contexts and their subjobs */
  for(i = 0; i<ARRAY_SIZE(_context_list);i++)
  {
    if(_context_list[i].in_use) 
    {
      _context_thread_cancel_context(&_context_list[i].instance); 
    }
  } 
}

/*------------------------------------------------*/
static void _add_page(fax_context_t *context, fax_app_ext_page_data_t * page)
{
  if(fax_app_raw_add_page( page->data, page->pix_format,  
                           page->pix_width, page->pix_height,
                           page->complete) == 0) 
  {
    context->raw_pages_received++;

    if(context->ticket.scan_before_sending)
    {
      if(page->complete)
      {
        if((context->ticket.type == FAX_TYPE_COLOR) || (context->ticket.type == FAX_TYPE_GRAY))
        {  
          /* try to send the job */
          if(fax_app_faxer_start_jpeg_send(context->ticket.digits, 
                                        (context->ticket.type == FAX_TYPE_COLOR),
                                        context->ticket.resolution,
                                        context, context->scan_data.store, 
                                        _faxer_subjob_callback) == 0)
          {
            context->faxer_state = FAX_SUBJOB_QUEUED;
            context->state = FAX_APP_STATE_SENDING;
          }
          else
          {
            _context_thread_cancel_context( context ); 
            return;
          }
        }
        else
        {
          /* try to send the job */
          if(fax_app_faxer_send(context->ticket.digits, context->scan_data.fname, 
                             context->ticket.resolution, context, 
                             _faxer_subjob_callback) == 0)
          {
            context->faxer_state = FAX_SUBJOB_QUEUED;
            context->state = FAX_APP_STATE_SENDING;
            unlink(context->scan_data.fname);
          }
          else
          {
            _context_thread_cancel_context(context);
            return;
          }
        }
      }
    }
    else
    {
      if(context->raw_pages_received == 0)
      {
        if(context->ticket.type == FAX_TYPE_COLOR || context->ticket.type == FAX_TYPE_GRAY) 
        {
          fax_app_faxer_set_data(true, context->scan_data.store,     
                                 1 /*pages*/, page->complete);
        }
        else
        {
          fax_app_faxer_set_data(false, context->scan_data.fname,     
                                 1 /*pages*/, page->complete/*data complete*/);
        }
      }
      else
      {
        fax_app_faxer_notify_page_added(context->scan_data.store);
      }
    }
  } 
  else
  {
    _context_thread_cancel_context( context );
  } 
}

/*------------------------------------------------*/
static void *_entry(void* unused)
{
  fax_context_t *context; 
  _msg_t *msg;

  while(1)
  {
    if(0 != posix_wait_for_message(_px_thread_data.queue, (char*)&msg,  sizeof(_msg_t*), POSIX_WAIT_FOREVER))
      continue;

    dbg_printf("FAX_APP: processing message from queue, type: %d.\n", msg->type);

    switch(msg->type)
    {
      case FAX_MSG_START_REPRINT:
        dbg_printf("FAX_APP: Starting reprint\n"); 
        context = fax_app_context_create(FAX_APP_RECV, &msg->data.ticket);
        if (context)
        {
            context->fax_info.job_id = msg->data.job_id;
            dbg_printf("FAX_APP: msg->data.job_id:%d context->fax_info.job_id:%d \n",
                    msg->data.job_id, context->fax_info.job_id);
            _context_thread_start_reprint_job(context);
        }
        else
            dbg_printf("FAX_APP: Could NOT create context for reprint, job never started.\n");
        break;
      case FAX_MSG_START_SEND:
        dbg_printf("FAX_APP: Starting context for sending.\n");
        /* start a context/job based on ticket */
        context = fax_app_context_create(FAX_APP_SEND, &msg->data.ticket);
        if(context)
          _context_thread_start_send_job(context);
        else
          dbg_printf("FAX_APP: Could NOT create context for sending, job never started.\n");
        break;
      case FAX_MSG_ADD_RAW_PAGE:
        dbg_printf("FAX_APP: Adding RAW page data.\n");
        /* find the context to add to */
        context = _find_context_waiting_on_raw_pages();
        if(context)
          _add_page(context, &msg->data.page);
        else
          dbg_printf("FAX_APP: Could NOT find context to add page to.\n");

        /* make the callback if one exists */
        if(msg->data.page.cb)
        {
          msg->data.page.cb(msg->data.page.data);
        }
        break;
      case FAX_MSG_START_RECV:
        dbg_printf("FAX_APP: Starting context for receive.\n");
        /* start a context/job based on ticket */
        context = fax_app_context_create(FAX_APP_RECV, &msg->data.ticket);
        if(context)
          _context_thread_start_receive_job(context);
        else
          dbg_printf("FAX_APP: Could NOT create context for receiving, job never started.\n");
        break;
      case FAX_MSG_END_ALL:
        dbg_printf("FAX_APP: End all msg received.\n");
        _context_thread_end_all();
        break;
      case FAX_MSG_FAX_SUBJOB_UPDATE:
        _context_thread_fax_subjob_update(msg->data.subjob_update.context, &msg->data.subjob_update);
        break;
      case FAX_MSG_PRINT_SUBJOB_UPDATE:
        _context_thread_print_subjob_update(msg->data.subjob_update.context, &msg->data.subjob_update);
        break;
     case FAX_MSG_SCAN_SUBJOB_UPDATE:
        _context_thread_scan_subjob_update(msg->data.subjob_update.context, &msg->data.subjob_update);
        break;
      default:
        ASSERT(0); 
    }
    MEM_FREE_AND_NULL(msg);
  }
  return 0;
}
/*****End thread functions ************/

static _msg_t *_get_free_msg_data_slot(void)
{
  /* for now, malloc the data */
  return((_msg_t*)MEM_MALLOC(sizeof(_msg_t)));
}


/**************************************************/
fax_app_res_t fax_app_context_start_send(fax_app_ticket_t *ticket)
{
  fax_app_res_t ret = FAX_APP_RES_SUCCESS;
  _msg_t *msg = _get_free_msg_data_slot();
 
  msg->type = FAX_MSG_START_SEND;
  msg->data.ticket = *ticket;
  /* don't hold the caller up, just return error if we can't add the message */
  if(posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*),  
                        MQ_DEFAULT_PRIORITY, 0))
  {
    ret = FAX_APP_RES_ERROR;
  }

  /* add msg to the queue to start fax context/job */
  return(ret);  
}

/**************************************************/
fax_app_res_t fax_app_context_add_page(fax_app_ext_page_data_t *data)
{
  fax_app_res_t ret = FAX_APP_RES_SUCCESS;
  _msg_t *msg = _get_free_msg_data_slot();
 
  msg->type = FAX_MSG_ADD_RAW_PAGE;
  msg->data.page = *data;
  /* don't hold the caller up, just return error if we can't add the message */
  if(posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*), 
                        MQ_DEFAULT_PRIORITY, 0))
  {
    ret = FAX_APP_RES_ERROR;
  }

  /* add msg to the queue to start fax context/job */
  return(ret);  
}

/**************************************************/
fax_app_res_t fax_app_context_start_recv(void)
{
  fax_app_res_t ret = FAX_APP_RES_SUCCESS;
  _msg_t *msg = _get_free_msg_data_slot();
  UINT status;
 
  msg->type = FAX_MSG_START_RECV;
  /* don't hold the caller up, just return error if we can't add the message */
  status = posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*), 
                              MQ_DEFAULT_PRIORITY, 0) ;
  if(status) 
  {
    ret = FAX_APP_RES_ERROR;
    dbg_printf("Failed posix_message_send() with status: %d, in fax_app_context_start_recv(), returning %d\n", status, ret);
  }

  /* add msg to the queue to start fax context/job */
  return(ret);  
}

/**************************************************/
fax_app_res_t fax_app_context_start_reprint(uint32_t job_id)
{
  fax_app_res_t ret = FAX_APP_RES_SUCCESS;
  _msg_t *msg = _get_free_msg_data_slot();
  FAXER_ACTIVITY_ENTRY entry; 
 
  msg->data.job_id = job_id;
  msg->type = FAX_MSG_START_REPRINT;
  /* first find the file to reprint */
  if(faxer_get_log(NULL, job_id, &entry)) 
  {
    ret = FAX_APP_RES_ERROR;
  }
  else 
  {
    struct stat st;
    if(stat(entry.fname, &st)) ret = FAX_APP_RES_ERROR;
  }
  if(ret == FAX_APP_RES_SUCCESS)
  {
    memcpy(msg->data.ticket.src_filename, entry.fname, sizeof(msg->data.ticket.src_filename)); 
    /* don't hold the caller up, just return error if we can't add the message */
    if(posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*), 
                          MQ_DEFAULT_PRIORITY, 0)) 
    {
      ret = FAX_APP_RES_ERROR;
    }
  }

  /* add msg to the queue to start fax context/job */
  return(ret);  
}

/**************************************************/
fax_app_res_t fax_app_end_all(void)
{
  _msg_t *msg = _get_free_msg_data_slot();
 
  msg->type = FAX_MSG_END_ALL;
  posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*), 
                     MQ_DEFAULT_PRIORITY, POSIX_WAIT_FOREVER); 
 
  return(FAX_APP_RES_SUCCESS);
}


/***************************************************
* Subjob callbacks 
***************************************************/
static fax_app_res_t _print_subjob_callback(fax_subjob_update_t *data)
{
  _msg_t *msg = _get_free_msg_data_slot();
  
  msg->type = FAX_MSG_PRINT_SUBJOB_UPDATE;
  msg->data.subjob_update = *data;
  posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*), 
                     MQ_DEFAULT_PRIORITY, POSIX_WAIT_FOREVER); 
  return(FAX_APP_RES_SUCCESS);  
}

/**************************************************/
static fax_app_res_t _scan_subjob_callback(fax_subjob_update_t *data)
{
  _msg_t *msg = _get_free_msg_data_slot();

  msg->type = FAX_MSG_SCAN_SUBJOB_UPDATE;
  msg->data.subjob_update = *data;
  posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*), 
                     MQ_DEFAULT_PRIORITY, POSIX_WAIT_FOREVER); 
  return(FAX_APP_RES_SUCCESS);  
}

/**************************************************/
static fax_app_res_t _faxer_subjob_callback(fax_subjob_update_t *data)
{
  _msg_t *msg = _get_free_msg_data_slot();

  msg->type = FAX_MSG_FAX_SUBJOB_UPDATE;
  msg->data.subjob_update = *data;
  posix_message_send(_px_thread_data.queue, (char*)&msg, sizeof(_msg_t*), 
                     MQ_DEFAULT_PRIORITY, POSIX_WAIT_FOREVER); 
  return(FAX_APP_RES_SUCCESS);  
}
