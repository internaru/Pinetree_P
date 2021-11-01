/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** Contains test cases for fax. This is intended for testing only. */
#include <string.h>
#include <dprintf.h>
#include <stdlib.h>
#include <signal.h>

#include "lassert.h"
#include "debug.h"
#include "fax_app_api.h"
#include "fax_dbg_test.h"
#include "posix_ostools.h"

static timer_t _timer;
static bool _initialized;
static bool _active;
fax_app_ticket_t _ticket;
int _test_num;

fax_app_brightness_t brightness = FAX_APP_BRIGHTNESS_STANDARD;
fax_app_resolution_t resolution = FAX_TYPE_STANDARD;

static Observer_t *_observer;

char * _fax_dbg_get_error_string(fax_app_comp_error_t err)
{
  char * string = NULL;
  switch(err)
  {
    case FAXER_ACT_STATUS_NULL:
      string = "??";
      break;
    case FAXER_ACT_STATUS_CALL_FAILED:
      string = "Failed";
      break;
    case FAXER_ACT_STATUS_NO_ANSWER:
      string = "No Answer";
      break;
    case FAXER_ACT_STATUS_NO_DIALTONE:
      string = "No Dialtone";
      break;
    case FAXER_ACT_STATUS_LINE_BUSY:
      string = "Line Busy";
      break;
    case FAXER_ACT_STATUS_FAX_COMPLETE:
      string = "Success";
      break;
    case FAXER_ACT_STATUS_COMPLETE_RTN:
      string = "Success - RTN";
      break;
    case FAXER_ACT_STATUS_PARTIAL_FAX:
      string = "Partial Fax";
      break;
    case FAXER_ACT_STATUS_NO_FAX:
      string = "No Fax";
      break;
    case FAXER_ACT_STATUS_COMM_ERROR:
      string = "Comm Error";
      break;
    case FAXER_ACT_STATUS_FS_ERROR:
      string = "FS Error";
      break;
    case FAXER_ACT_STATUS_FS_FULL:
      string = "FS Full";
      break;
    case FAXER_ACT_STATUS_RETRY:
      string = "Waiting to Retry";
      break;
    case FAXER_ACT_STATUS_BLOCKED:
      string = "Blocked";
      break;
    case FAXER_ACT_STATUS_CANCELED:
      string = "Canceled";
      break;
    case FAXER_ACT_STATUS_UNSCHEDULED:
      /* ??? */
    default:
      ASSERT(0);
  } 
  return(string);
}

/*---------------------------------------------*/
static void _timer_fire( union sigval unused)
{
  /* launch the job again */
  if(_active) 
  {
    fax_app_res_t res;
    res = fax_app_send_fax(&_ticket); 
    if(res != FAX_APP_RES_SUCCESS)
    {
      dbg_printf("FAX_LOOP_TEST: error launching job, ending test!!!\n");
      _fax_dbg_end_send_loop();
    }
    else 
      dbg_printf("FAX_LOOP_TEST: Lauching job.\n");
  }
}

/*---------------------------------------------*/
static void _context_status_update(Observer_t *observer, Subject_t *s)
{
    fax_app_state_t state;

    ASSERT( VALIDATE_OBJECT_KEY( observer,
                               Observer_t,
                               subject_observer_oid_key,
                               FAXAPP_ContextStatus_ID ) );
    state = fax_app_get_context_state(s);

    if((state == FAX_APP_STATE_COMPLETE) || (state == FAX_APP_STATE_CANCELED))
    {
      fax_app_comp_error_t err;
      err = fax_app_get_context_faxCompletionError(s);
      _test_num++;
      dbg_printf("FAX_LOOP_TEST: Fax send num %d completed with status = |%s|\n", _test_num, _fax_dbg_get_error_string(err));
      /* 
       * Activate 10 second periodic timer.
       */
      /*
      int px_status;
      struct itimerspec its;
      memset( &its, 0, sizeof(struct itimerspec) );
      its.it_interval.tv_sec = 8*100*MILLISEC_PER_TICK/MILLISEC_PER_SECOND;
      its.it_interval.tv_nsec = ((8*100*MILLISEC_PER_TICK)%MILLISEC_PER_SECOND) * 
        (NANOSEC_PER_USEC*USEC_PER_MILLISEC);;
      its.it_value = its.it_interval;

      px_status = timer_settime(_timer, 0, &its, NULL);
      if(px_status)
      {
        dbg_printf("Failed to set(activate) timer for fax test.\n");
      }
      */
      _timer_fire((union sigval)NULL);
    }
}


/*---------------------------------------------*/
void _fax_dbg_start_send_loop(fax_app_type_t type, char *number, char *file)
{
  fax_app_res_t res;
  int px_status;
 
  if(_active) return;
 
  /* start a loop where we send a fax 10 seconds after the last one */
  if(!_initialized)
  {
     /* timer */
    struct sigevent evp;

     // callback fuction is _timer_fire  
     memset( &evp, 0, sizeof(struct sigevent) );
     evp.sigev_notify = SIGEV_THREAD;
     evp.sigev_notify_function = _timer_fire;
     evp.sigev_signo = SIGSTOP;

     px_status = timer_create(CLOCK_REALTIME, &evp, &_timer);
     if(px_status)
     {
       dbg_printf("Failed to create timer for fax test.\n");
     }

     _observer = Observer_Constructor(FAXAPP_ContextStatus_ID, _context_status_update);
     ASSERT(_observer);
     _initialized = true;
  }
  strncpy(_ticket.digits, number, sizeof(_ticket.digits));
  if(file)
  {
    strncpy(_ticket.src_filename, file, sizeof(_ticket.src_filename)); 
    _ticket.src = FAX_SRC_FILE;
  }
  else 
  {
    memset(&_ticket.src_filename, 0, sizeof(_ticket.src_filename)); 
    _ticket.src = FAX_SRC_SCANNER_AUTO;
  }
  _ticket.type = type;
  _ticket.brightness = brightness;
  _ticket.scan_before_sending = 0; 
  if(type == FAX_TYPE_COLOR)
  { 
    _ticket.resolution = FAX_TYPE_STANDARD;
  } else
  {
    _ticket.resolution = resolution;
  }
  fax_app_AttachObserver_contextStatus(_observer); 
  dbg_printf("FAX_LOOP_TEST: starting fax loop test with the following params:\n");
  dbg_printf("  number=%s\n  file=%s\n  type=%d\n brightness=%d\n  res=%d\n",
             _ticket.digits, _ticket.src_filename, _ticket.type, _ticket.brightness, _ticket.resolution);
  _test_num = 0;
  res = fax_app_send_fax(&_ticket); 
  if(res != FAX_APP_RES_SUCCESS)
  {
    dbg_printf("FAX_LOOP_TEST: error launching job, ending test!!!\n");
    _fax_dbg_end_send_loop();
  }
  else
  {
    dbg_printf("FAX_LOOP_TEST: Lauching job.\n");
    _active = true;
  }
}


/*---------------------------------------------*/
void _fax_dbg_end_send_loop(void)
{
  _active = false;

  // disarm periodic timer
  /*
  struct itimerspec its;
  memset( &its, 0, sizeof(struct itimerspec) ); // clear structure values
  int px_status = timer_settime(_timer, 0, &its, NULL);
  if(px_status != 0)
  {
    dbg_printf("FAX_LOOP_TEST: error deactivating timer!!!\n");
  }
  */

  fax_app_DetachObserver_contextStatus(_observer); 
}

