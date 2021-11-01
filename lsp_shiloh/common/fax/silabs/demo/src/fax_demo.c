/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**\ fax_demo.c
* This file simulates the UI through debug calls.  The simulated UI disply output will be 
* sent to debug.
*/

#include <string.h>
#include <dprintf.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "agRouter.h"
#include "lassert.h"
#include "faxer_api.h"
#include "fax_tiff_io.h"
#include "debug.h"
#include "SJMgr.h"
#include "fax_app_api.h"
#include "lassert.h"
#include "fax_store_api.h"
#include "fax_dbg_test.h"

#include "fax_demo_jpeg_data.h"
#include "cmd_proc_api.h"
#include "fax_dbg_net.h"
#include "faxtest_msgs.h"
//#include "fax_reports_api.h"
#include "cmd_proc_api.h"

#define  _STRINGIFY(x) __STRINGIFY(x)
#define __STRINGIFY(x) #x

#ifdef HAVE_CMD
#define fax_dbg_net_log_fax_host_status(thingy)
#define fax_dbg_net_log_fax_app_state(state)
#define fax_dbg_net_log_fax_app_fax_sub_state(state)
#define fax_dbg_net_open(parm) 0
#define fax_dbg_net_close()
#endif

static Observer_t *context_observer;
static Observer_t *modem_observer;
static int _observer_registered = 0;

static fax_app_resolution_t _resolution = FAX_TYPE_STANDARD;
static fax_app_brightness_t _brightness = FAX_APP_BRIGHTNESS_STANDARD;

/* Print a simulated UI message to debug */
static void _print_ui_msg(char *line1, char *line2)
{
  dbg_printf("\n--FAXDEMO Status--| %s; %s|----\n",
             line1 ? line1 : "", line2 ? line2 : "");
}


static char * _get_res_ui_string(fax_app_res_t res)
{
  char * string = NULL;
  switch(res)
  {
    case FAX_APP_RES_LINE_UNAVAILABLE:
      string = "Line is not available";
    case FAX_APP_RES_FAX_ACTIVE:
      string = "Fax job currently active";
    case FAX_APP_RES_NO_FAX_TO_RECEIVE:
      string = "No incomming call";
    case FAX_APP_RES_FILE_DOES_NOT_EXIST:
      string = "Fax file does not exist";
      break;
    case FAX_APP_RES_INVALID_PARAMS:
    default:
      ASSERT(0);
  }
  return(string);
}

static int _testsend(int argc, char* argv[])
{
  /* sends the fax over and over until turned off */
  if      (argc == 2) _fax_dbg_start_send_loop(FAX_TYPE_BW,   argv[1], NULL);
  else if (argc == 3) _fax_dbg_start_send_loop(atoi(argv[2]), argv[1], NULL);
  else if (argc == 4) _fax_dbg_start_send_loop(atoi(argv[2]), argv[1], argv[3]);
  else                return CMD_USAGE_ERROR;
  return CMD_OK;
}

static int _endtest(int argc, char* argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
  _fax_dbg_end_send_loop(); 
  return CMD_OK;
}

static int _reprint(int argc, char* argv[])
{
  if (argc != 2) return CMD_USAGE_ERROR;
  fax_app_reprint_fax(cmd_atoi(argv[1]));
  return CMD_OK;
}

static int _stamp(int argc, char* argv[])
{
  if (argc > 2) return CMD_USAGE_ERROR;
  if (argc > 1) fax_app_print_set_stamp_faxes(cmd_atoi(argv[1]));
  cmd_append_result("fax stamp = %d [no=0|yes=1]\n", fax_app_print_get_stamp_faxes());
  return CMD_OK;
}

static void _context_status_update(Observer_t *observer, Subject_t *s)
{
#define STATUS_COMP_MSG_SIZE (50)
    fax_app_state_t state;
    char *line2;

    ASSERT( VALIDATE_OBJECT_KEY( observer,
                               Observer_t,
                               subject_observer_oid_key,
                               FAXAPP_ContextStatus_ID ) );
    state = fax_app_get_context_state(s);
  
#ifdef HAVE_NETWORK
    fax_dbg_net_log_fax_app_state(state);
#endif

    /* ID must match */ 
    switch(state)
    {
      case FAX_APP_STATE_NONE:
        /* no status, context is idle */
        dbg_printf("FAXDEMO: context has no status!\n");
        break;
      case FAX_APP_STATE_SCANNING:
        _print_ui_msg("Fax Scanning...", NULL);
        break;
      case FAX_APP_STATE_PRINTING:
        _print_ui_msg("Fax Printing...", NULL);
        break;
      case FAX_APP_STATE_RECEIVING:
      case FAX_APP_STATE_RECEIVING_PRINTING:
#ifdef HAVE_NETWORK
        fax_dbg_net_log_fax_app_fax_sub_state(fax_app_get_context_faxSubState(s));
#endif
        switch(fax_app_get_context_faxSubState(s))
        {
          case FAX_APP_FAX_STATE_NEGOTIATING:
            _print_ui_msg("Fax Connecting...", NULL);
            break;
          case FAX_APP_FAX_STATE_RECEIVING:
            _print_ui_msg("Fax Receiving...", NULL);
            break;
          case FAX_APP_FAX_STATE_COMPLETE:
           line2 = _fax_dbg_get_error_string(fax_app_get_context_faxCompletionError(s));
            _print_ui_msg("Fax Receive Complete...", line2); 
            break;
          default:
            cmd_printf("FAX DEMO ERROR: Invalid Fax Sub Event\n");
        }
        break;
      case FAX_APP_STATE_SENDING:
      case FAX_APP_STATE_SCANNING_SENDING:
#ifdef HAVE_NETWORK
        fax_dbg_net_log_fax_app_fax_sub_state(fax_app_get_context_faxSubState(s));
#endif
        switch(fax_app_get_context_faxSubState(s))
        {
          case FAX_APP_FAX_STATE_DIALING:
          case FAX_APP_FAX_STATE_NONE:
            _print_ui_msg("Fax Dialing...", NULL);
            break;
          case FAX_APP_FAX_STATE_NEGOTIATING:
            _print_ui_msg("Fax Connecting...", NULL);
            break;
          case FAX_APP_FAX_STATE_SENDING:
            _print_ui_msg("Fax Sending...", NULL);
            break;
          case FAX_APP_FAX_STATE_COMPLETE:
           line2 = _fax_dbg_get_error_string(fax_app_get_context_faxCompletionError(s));
            _print_ui_msg("Fax Send Complete...", line2); 
            break;
          default:
            cmd_printf("FAX DEMO ERROR: Invalid Fax Sub Event\n");
        }
        break;
      case FAX_APP_STATE_CANCELING:
        _print_ui_msg("Fax Canceling...", NULL);
        break;
      case FAX_APP_STATE_COMPLETE:
        line2 = _fax_dbg_get_error_string(fax_app_get_context_faxCompletionError(s));
        _print_ui_msg("Fax Completed", line2);
        break;
      case FAX_APP_STATE_CANCELED:
        _print_ui_msg("Fax Canceled", NULL);
        break;
      default:
        ASSERT(0); /* invalid state, assert since this is a debug callback */
    }
}

static void _modem_status_update(Observer_t *observer, Subject_t *s)
{
  bool cid_set;
  char cid_name[FAX_MAX_CID_NAME_SIZE+1];
  char cid_num[FAX_MAX_CID_NUMBER_SIZE+1];
  fax_modem_status_data_t data;
  ASSERT( VALIDATE_OBJECT_KEY( observer,
                               Observer_t,
                               subject_observer_oid_key,
                               FAX_ModemStatus_ID ) );
  fax_modem_get_status_info(s, &data);
  dbg_printf("Modem status changed.  \n  Line present = %d\n  Line off hook = %d\n  ext off hook = %d\n  line ringing = %d\n",
             data.line_present, data.line_off_hook, data.ext_off_hook, data.line_ringing); 
  cid_set = fax_modem_get_status_callerID(s, cid_name, sizeof(cid_name), cid_num, sizeof(cid_num));
  if(cid_set)
    dbg_printf("     caller id name = |%s| number = |%s|\n", cid_name, cid_num);
  else
    dbg_printf("     caller id not set.\n");
}

static int _regstat(int argc, char* argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
  if ( ! _observer_registered)
  {
    context_observer = Observer_Constructor(FAXAPP_ContextStatus_ID, _context_status_update);
    ASSERT(context_observer);
    fax_app_AttachObserver_contextStatus(context_observer); 
    modem_observer = Observer_Constructor(FAX_ModemStatus_ID, _modem_status_update);
    ASSERT(modem_observer);
    fax_AttachObserver_modemStatus(modem_observer); 
    cmd_printf("Attached to observers\n");
    _observer_registered = 1;
  }
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_REGSTAT_ACK);
#endif
  return CMD_OK;
}

#ifdef HAVE_NETWORK
static int _netopen(int argc, char* argv[])
{
  if (argc != 2) return CMD_USAGE_ERROR;
  if (!fax_dbg_net_open(argv[1]))
  {
      fax_dbg_net_log_fax_host_status(HOST_STATUS_NETOPEN_ACK);
      return CMD_OK;
  }
  cmd_printf("ERROR: netopen failed\n");
  return CMD_ERROR;
}

static int _netclose(int argc, char* argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
  fax_dbg_net_log_fax_host_status(HOST_STATUS_NETCLOSE_ACK);
  fax_dbg_net_close();
  return CMD_OK;
}
#endif

static int _noprint(int argc, char* argv[])
{
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_NOPRINT_ACK);
#endif
  if (argc > 1) fax_app_test_mode_no_print(cmd_atoi(argv[1]));
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _dialmode(int argc, char* argv[])
{
  uint32_t cur, new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_DIALMODE_ACK);
#endif

  if(argc > 1) /* Set */
  {
    if      (!strcmp(argv[1], "pulse")) new = 0;
    else if (!strcmp(argv[1], "tone"))  new = 1;
    else                                new = cmd_atoi(argv[1]);
    if (new > 1) return CMD_USAGE_ERROR;
    fax_app_set_setting(FAXER_SETTING_INT_DIALING_MODE, &new);
    fax_app_get_setting(FAXER_SETTING_INT_DIALING_MODE, &cur, sizeof(cur));
    if(cur != new)
    {
      cmd_printf("Failed to set dial mode to %d, currently set to %d\n",
                 (int)new, (int)cur);
      return CMD_ERROR;
    }
  }
  fax_app_get_setting(FAXER_SETTING_INT_DIALING_MODE, &cur, sizeof(cur));
  cmd_append_result("%s", cur?"tone":"pulse");
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _ecm(int argc, char* argv[])
{
  uint32_t cur, new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_ECM_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if      (!strcmp(argv[1], "on"))  new = 1;
    else if (!strcmp(argv[1], "off")) new = 0;
    else                              new = cmd_atoi(argv[1]);
    if (new > 1) return CMD_USAGE_ERROR;
    fax_app_set_setting(FAXER_SETTING_SW_ECM_MODE, &new);
    fax_app_get_setting(FAXER_SETTING_SW_ECM_MODE, &cur, sizeof(cur));
    if(cur != new)
    {
      cmd_printf("Failed to set ECM mode to %d, currently set to %d\n",
                 (int)new, (int)cur);
      return CMD_ERROR;
    }
  }
  fax_app_get_setting(FAXER_SETTING_SW_ECM_MODE, &cur, sizeof(cur));
  cmd_append_result("%s", cur?"on":"off");
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _vol(int argc, char* argv[])
{
  uint32_t cur, new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_VOL_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if      (!strcmp(argv[1], "off"))  new = 0;
    else if (!strcmp(argv[1], "low"))  new = 1;
    else if (!strcmp(argv[1], "med"))  new = 2;
    else if (!strcmp(argv[1], "high")) new = 3;
    else                               new = cmd_atoi(argv[1]);
    if (new > 3) return CMD_USAGE_ERROR;
    fax_app_set_setting(FAXER_SETTING_SW_AUDIO_VOLUME, &new);
    fax_app_get_setting(FAXER_SETTING_SW_AUDIO_VOLUME, &cur, sizeof(cur));
    if(cur != new)
    {
      cmd_printf("Failed to set volume to %d, currently set to %d\n",
                 (int)new, (int)cur);
      return CMD_ERROR;
    }
  }
  fax_app_get_setting(FAXER_SETTING_SW_AUDIO_VOLUME, &cur, sizeof(cur));
  cmd_append_result("%d", (int)cur);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _printsize(int argc, char* argv[])
{
  uint32_t new;
  if (argc != 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_PRINTSIZE_ACK);
#endif

  if      (!strcmp(argv[1], "letter")) new = 1;
  else if (!strcmp(argv[1], "legal"))  new = 5;
  else if (!strcmp(argv[1], "a4"))     new = 9;
  else                                 new = cmd_atoi(argv[1]);
  if ((new != 1) && (new != 5) && (new != 9)) return CMD_USAGE_ERROR;
  fax_app_print_set_media_size((mediasize_t)new);
  cmd_append_result("%d", (int)new);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _disring(int argc, char* argv[])
{
  uint32_t cur, new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_DISRING_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if      (!strcmp(argv[1], "all"))    new = 0;
    else if (!strcmp(argv[1], "single")) new = 1;
    else if (!strcmp(argv[1], "double")) new = 2;
    else if (!strcmp(argv[1], "triple")) new = 3;
    else if (!strcmp(argv[1], "both"))   new = 4;
    else                                 new = cmd_atoi(argv[1]);
    if (new > 4) return CMD_USAGE_ERROR;
    fax_app_set_setting(FAXER_SETTING_INT_RING_PATTERN, &new);
    fax_app_get_setting(FAXER_SETTING_INT_RING_PATTERN, &cur, sizeof(cur));
    if(cur != new)
    {
      cmd_printf("Failed to set ring pattern to %d, currently set to %d\n",
                 (int)new, (int)cur);
      return CMD_ERROR;
    }
  }
  fax_app_get_setting(FAXER_SETTING_INT_RING_PATTERN, &cur, sizeof(cur));
  cmd_append_result("%d", (int)cur);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _res(int argc, char* argv[])
{
  uint32_t new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_RES_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if      (!strcmp(argv[1], "standard")) new = 0;
    else if (!strcmp(argv[1], "fine"))     new = 1;
    else if (!strcmp(argv[1], "veryfine")) new = 2;
    else if (!strcmp(argv[1], "photo"))    new = 3;
    else                                   new = cmd_atoi(argv[1]);
    if (new > 3) return CMD_USAGE_ERROR;
    _resolution = new;
  }
  cmd_append_result("%d", (int)_resolution);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _bright(int argc, char* argv[])
{
  uint32_t new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_BRIGHT_ACK);
#endif

  if (argc > 1) /* Set */
  {
    new = cmd_atoi(argv[1]);
    if ((new < FAX_APP_BRIGHTNESS_MIN) || (new > FAX_APP_BRIGHTNESS_MAX))
      return CMD_USAGE_ERROR;
    _brightness = new;
  }
  cmd_append_result("%d", (int)_brightness);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _speed(int argc, char* argv[])
{
  uint32_t cur, new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_SPEED_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if      (!strcmp(argv[1], "fast"))   new = 0;
    else if (!strcmp(argv[1], "medium")) new = 1;
    else if (!strcmp(argv[1], "slow"))   new = 2;
    else                                 new = cmd_atoi(argv[1]);
    if (new > 2) return CMD_USAGE_ERROR;
    fax_app_set_setting(FAXER_SETTING_INT_MAX_SPEED, &new);
    fax_app_get_setting(FAXER_SETTING_INT_MAX_SPEED, &cur, sizeof(cur));
    if(cur != new)
    {
      cmd_printf("Failed to set speed to %d, currently set to %d\n",
                 (int)new, (int)cur);
      return CMD_ERROR;
    }
  }
  fax_app_get_setting(FAXER_SETTING_INT_MAX_SPEED, &cur, sizeof(cur));
  cmd_append_result("%d", (int)cur);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _discallw(int argc, char* argv[])
{
  uint32_t cur, new;
  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_DISCALLW_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if      (!strcmp(argv[1], "off"))  new = 0;
    else if (!strcmp(argv[1], "on"))   new = 1;
    else                               new = cmd_atoi(argv[1]);
    if (new > 1) return CMD_USAGE_ERROR;
    fax_app_set_setting(FAXER_SETTING_SW_DIAL_PREFIX, &new);
    fax_app_get_setting(FAXER_SETTING_SW_DIAL_PREFIX, &cur, sizeof(cur));
    if(cur != new)
    {
      cmd_printf("Failed to set speed to %d, currently set to %d\n",
                 (int)new, (int)cur);
      return CMD_ERROR;
    }
  }
  fax_app_get_setting(FAXER_SETTING_SW_DIAL_PREFIX, &cur, sizeof(cur));
  cmd_append_result("%d", (int)cur);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _hname(int argc, char* argv[])
{
  char cur[FAXER_MAX_HEADER_SIZE+1];

  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_HNAME_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if (strlen(argv[1]) > FAXER_MAX_HEADER_SIZE)
    {
      cmd_printf("Requested header name is too long (%s)\n", argv[1]);
      return CMD_ERROR;
    }
    fax_app_set_setting(FAXER_SETTING_CHR_HEADER_NAME, argv[1]);
    memset(cur, 0, sizeof(cur));
    fax_app_get_setting(FAXER_SETTING_CHR_HEADER_NAME, cur, sizeof(cur));
    cur[sizeof(cur)-1] = 0;
    if (strcmp(cur, argv[1]))
    {
      cmd_printf("Failed to set header name to \"%s\", currently \"%s\"\n",
                 argv[1], cur);
      return CMD_ERROR;
    }
  }
  memset(cur, 0, sizeof(cur));
  fax_app_get_setting(FAXER_SETTING_CHR_HEADER_NAME, cur, sizeof(cur));
  cur[sizeof(cur)-1] = 0;
  cmd_append_result("%s", cur);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _hnumber(int argc, char* argv[])
{
  char cur[FAXER_MAX_SID_SIZE+1];

  if (argc > 2) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_HNUMBER_ACK);
#endif

  if (argc > 1) /* Set */
  {
    if (strlen(argv[1]) > FAXER_MAX_SID_SIZE)
    {
      cmd_printf("Requested header number is too long (%s)\n", argv[1]);
      return CMD_ERROR;
    }
    fax_app_set_setting(FAXER_SETTING_CHR_SID, argv[1]);
    memset(cur, 0, sizeof(cur));
    fax_app_get_setting(FAXER_SETTING_CHR_SID, cur, sizeof(cur));
    cur[sizeof(cur)-1] = 0;
    if (strcmp(cur, argv[1]))
    {
      cmd_printf("Failed to set header number to \"%s\", currently \"%s\"\n",
                 argv[1], cur);
      return CMD_ERROR;
    }
  }
  memset(cur, 0, sizeof(cur));
  fax_app_get_setting(FAXER_SETTING_CHR_SID, cur, sizeof(cur));
  cur[sizeof(cur)-1] = 0;
  cmd_append_result("%s", cur);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _autoans(int argc, char* argv[])
{
  int rings;

#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_AUTOANS_ACK);
#endif
  if (argc > 2) return CMD_USAGE_ERROR;
  if (argc > 1)
  {
    rings = cmd_atoi(argv[1]);
    if (rings > 15)
    {
      cmd_printf("Setting number of rings to 1\n");
      rings = 1;
    }
    if (rings)
    {
      fax_app_set_setting(FAXER_SETTING_INT_RINGS_TO_ANSWER, &rings);
      rings = 1;
    }
    fax_app_set_setting(FAXER_SETTING_INT_ANSWER_MODE, &rings);
  }
  fax_app_get_setting(FAXER_SETTING_INT_ANSWER_MODE, &rings, sizeof(rings));
  if (rings)
    fax_app_get_setting(FAXER_SETTING_INT_RINGS_TO_ANSWER,&rings,sizeof(rings));
  cmd_append_result("%d", rings);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _ans(int argc, char* argv[])
{
  fax_app_res_t res;
  if (argc != 1) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_ANS_ACK);
#endif
  if (FAX_APP_RES_SUCCESS != (res = fax_app_receive_fax()))
  {
    _print_ui_msg("Error:", _get_res_ui_string(res));
    return CMD_ERROR;
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _fsend_fax(int argc, char* argv[], int mode)
{
  fax_app_ticket_t ticket;
  char *filename, *digits = "";
       
  if (argc < 2) return CMD_USAGE_ERROR;
  if (argc > 1) filename = argv[1];
  if (argc > 2) digits = argv[2];
  if (argc > 3) return CMD_USAGE_ERROR;

  if (strlen(filename)+1 > sizeof(ticket.src_filename))
  {
    cmd_printf("Filename is too long for fax ticket\n");
    return CMD_ERROR;
  }

  if (strlen(digits)+1 > sizeof(ticket.digits))
  {
    cmd_printf("Number is too long for fax ticket\n");
    return CMD_ERROR;
  }

  memset(&ticket, 0, sizeof(ticket));
  switch (mode)
  {
    case 0: /* fsend */
            ticket.type = FAX_TYPE_BW;
            ticket.resolution = FAX_TYPE_VERY_FINE;
            break;
    case 1: /* fcolorsend */
            ticket.type = FAX_TYPE_COLOR;
            ticket.resolution = FAX_TYPE_STANDARD;
            break;
    default: cmd_printf("Unknown fax send mode (internal error)\n");
             return CMD_ERROR;
  }
  ticket.brightness = _brightness;
  ticket.src = FAX_SRC_FILE;
  strcpy(ticket.src_filename, filename);
  strcpy(ticket.digits, digits);
  fax_app_send_fax(&ticket);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _fsend(int argc, char* argv[])
{
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_FSEND_ACK);
#endif
  return _fsend_fax(argc, argv, 0);
}

/*----------------------------------------------------------------------------*/
static int _fcolorsend(int argc, char* argv[])
{
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_FCOLORSEND_ACK);
#endif
  return _fsend_fax(argc, argv, 1);
}

/*----------------------------------------------------------------------------*/
static int _send_fax(int argc, char* argv[], int mode)
{
  fax_app_ticket_t ticket;
  char *digits;
  int cache = mode?0:1; /* Only black and white defaults to cached */
       
  if (argc < 2) return CMD_USAGE_ERROR;
  if (argc > 1) digits = argv[1];
  if (argc > 2)
  {
    if (!mode) cache = cmd_atoi(argv[2]);
    else       return CMD_USAGE_ERROR; /* We cannot cache color or grayscale */
  }
  if (argc > 3) return CMD_USAGE_ERROR;

  if (strlen(digits)+1 > sizeof(ticket.digits))
  {
    cmd_printf("Number is too long for fax ticket\n");
    return CMD_ERROR;
  }

  memset(&ticket, 0, sizeof(ticket));
  switch (mode)
  {
    case 0: /* bwsend */
            ticket.type = FAX_TYPE_BW;
            break;
    case 1: /* colorsend */
            ticket.type = FAX_TYPE_COLOR;
            break;
    case 2: /* graysend */
            ticket.type = FAX_TYPE_GRAY; 
            break;
    default: cmd_printf("Unknown fax send mode (internal error)\n");
             return CMD_ERROR;
  }
  ticket.brightness = _brightness;
  ticket.resolution = _resolution;
  ticket.scan_before_sending = cache?1:0;
  ticket.src = FAX_SRC_SCANNER_AUTO;
  strcpy(ticket.digits, digits);
  fax_app_send_fax(&ticket);
  return CMD_OK;
}

static void _add_page_cb(uint8_t *data_ptr)
{
  dbg_printf(">>Freeing memory for page\n");
  MEM_FREE_AND_NULL(data_ptr);
}

/*----------------------------------------------------------------------------*/
static int _rawsend(int argc, char* argv[])
{
  int y;
  fax_app_ticket_t ticket;
  fax_app_ext_page_data_t page_data;
  char *digits;
  int line_cnt = 100 * 11;
  int line_size = 224 * 8;  //This need to match the real resolution of fax

  if (argc < 2) return CMD_USAGE_ERROR;
  if (argc > 1) digits = argv[1];
  if (argc > 2) return CMD_USAGE_ERROR;

  if (strlen(digits)+1 > sizeof(ticket.digits))
  {
    cmd_printf("Number is too long for fax ticket\n");
    return CMD_ERROR;
  }

  memset(&ticket, 0, sizeof(ticket));
#if 0
  switch (mode)
  {
    case 0: /* bwsend */
            ticket.type = FAX_TYPE_BW;
            break;
    case 1: /* colorsend */
            ticket.type = FAX_TYPE_COLOR;
            break;
    case 2: /* graysend */
            ticket.type = FAX_TYPE_GRAY; 
            break;
    default: cmd_printf("Unknown fax send mode (internal error)\n");
             return CMD_ERROR;
  }
#endif
  ticket.type = FAX_TYPE_BW;
  ticket.brightness = _brightness;
  ticket.resolution = FAX_TYPE_STANDARD;
  ticket.scan_before_sending = 0;
  ticket.src = FAX_SRC_EXTERNAL;
  strcpy(ticket.digits, digits);
  fax_app_send_fax(&ticket);

  /* create page for sending */
  page_data.data = MEM_MALLOC(line_cnt*line_size);
  for(y=0;y<line_cnt;y++)
  {
    memset(page_data.data+y*line_size, 0, line_size);
    memset(page_data.data+y*line_size+line_size/2, 0xFF,30);
  }
  page_data.pix_width = line_size;
  page_data.pix_height = line_cnt;
  page_data.cb = _add_page_cb;
  page_data.complete = 1;
  if(fax_app_send_add_raw_page(&page_data) != OK)
  {
    MEM_FREE_AND_NULL(page_data.data);
  }
  return CMD_OK;
}


/*----------------------------------------------------------------------------*/
static int _bwsend(int argc, char* argv[])
{
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_BWSEND_ACK);
#endif
  return _send_fax(argc, argv, 0);
}

/*----------------------------------------------------------------------------*/
static int _colorsend(int argc, char* argv[])
{
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_COLORSEND_ACK);
#endif
  return _send_fax(argc, argv, 1);
}

/*----------------------------------------------------------------------------*/
static int _graysend(int argc, char* argv[])
{
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_GRAYSEND_ACK);
#endif
  return _send_fax(argc, argv, 2);
}

/*----------------------------------------------------------------------------*/
static int _cancel(int argc, char* argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_CANCEL_ACK);
#endif
  fax_app_cancel();
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _fnew(int argc, char* argv[])
{
  int pages = 1;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_FNEW_ACK);
#endif

  if (argc < 2) return CMD_USAGE_ERROR;
  if (argc > 2) pages = cmd_atoi(argv[2]);
  if (argc > 3) return CMD_USAGE_ERROR;
  if (!pages)
  {
    cmd_printf("I cannot create a file with 0 pages!\n");
    return CMD_ERROR;
  }

//  if (fax_generate_sample_fax(argv[1], pages))
//  {
//    cmd_printf("File not created, file=%s pages=%d\n", argv[1], pages);
//    return CMD_ERROR;
//  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _fjpegnew(int argc, char* argv[])
{
  int fd;
#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_FJPEGNEW_ACK);
#endif

  if (argc != 2) return CMD_USAGE_ERROR;

  if (0 > (fd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0777)))
  {
    cmd_printf("Failed to create file, %s\n", argv[1]);
    return CMD_ERROR;
  }
  write(fd, jpgarray, sizeof(jpgarray));
  close(fd);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _list_reprintable(int argc, char* argv[])
{
#define _MAX_ENTRIES (30)
  uint32_t cnt;
  int i;
  FAXER_ACTIVITY_ENTRY *entries;
  FAXER_ACTIVITY_ENTRY *act;

#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_LIST_ACK);
#endif

  entries = MEM_MALLOC( sizeof( FAXER_ACTIVITY_ENTRY ) * _MAX_ENTRIES );
  if(!entries)
  {
      cmd_printf("Failed to malloc memory\n");
      return CMD_ERROR;
  }

  fax_app_reprint_get_list( entries, _MAX_ENTRIES, &cnt );

  for(i=0;i < cnt;i++)
  {
      act = &entries[i];
      cmd_printf("%d)\n", i);
      cmd_printf("job_id:         %u\n", (unsigned int)act->job_id);
      cmd_printf("type:           %d\n", act->type);
      cmd_printf("fname:          %s\n", act->fname);
      cmd_printf("remId (number): %s\n", act->remoteId);
      cmd_printf("remName (name): %s\n", act->remoteName);
      cmd_printf("status:         %d\n", act->status);
      cmd_printf("pending:        %d\n", act->pending);
      cmd_printf("duration:       %d\n", act->duration);
      cmd_printf("pages:          %d\n", act->page_cnt);
      cmd_printf("date:           %d/%d/%d\n", act->time.tm_mon, act->time.tm_mday, act->time.tm_year + 2000);
      cmd_printf("time:           %d:%02d:%02d\n", act->time.tm_hour, act->time.tm_min, act->time.tm_sec);
      cmd_printf("-----\n");
  }
  MEM_FREE_AND_NULL(entries);
  return CMD_OK;
}
/*----------------------------------------------------------------------------*/
static int _list(int argc, char* argv[])
{
  FAXER_ACT_TYPE type;
  uint32_t cnt;
  uint32_t *index_set;
  FAXER_ACTIVITY_ENTRY act;
  int i;
  bool last_fax_printed;

#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_LIST_ACK);
#endif
  if (argc == 1)
  {
    type = FAXER_ACT_TYPE_NULL;
  }
  else if (argc == 2)
  {
    i = cmd_atoi(argv[1]);
    if (i<0 || i>3)
    {
      cmd_printf("Invalid fax type\n");
      return CMD_ERROR;
    }
    type= (FAXER_ACT_TYPE)i;
  }
  else
    return CMD_USAGE_ERROR;

  cnt = faxer_find_fax(NULL,type, 0xffff, 0, 0);
  cmd_printf("Fax log count = %d\n", (int)cnt);

  if (cnt > 0)
  {
    if (!(index_set = MEM_MALLOC(sizeof(uint32_t) * cnt)))
    {
      cmd_printf("Failed to malloc memory\n");
      return CMD_ERROR;
    }
    cnt = faxer_find_fax(NULL, type, 0xffff, index_set, cnt);
    for(i=0;i < cnt;i++)
    {
      faxer_get_log(NULL,index_set[i],&act);
      cmd_printf("%d)\n", i);
      cmd_printf("job_id:         %u\n", (unsigned int)act.job_id);
      cmd_printf("type:           %d\n", act.type);
      cmd_printf("fname:          %s\n", act.fname);
      cmd_printf("remId (number): %s\n", act.remoteId);
      cmd_printf("remName (name): %s\n", act.remoteName);
      cmd_printf("status:         %d\n", act.status);
      cmd_printf("pending:        %d\n", act.pending);
      cmd_printf("duration:       %d\n", act.duration);
      cmd_printf("pages:          %d\n", act.page_cnt);
      cmd_printf("date:           %d/%d/%d\n", act.time.tm_mon, act.time.tm_mday, act.time.tm_year + 2000);
      cmd_printf("time:           %d:%02d:%02d\n", act.time.tm_hour, act.time.tm_min, act.time.tm_sec);
      cmd_printf("-----\n");
    }
    MEM_FREE_AND_NULL(index_set);
  }
  fax_app_check_if_last_fax_printed(&last_fax_printed);
  dbg_printf("- Did last fax print, answer is %d\n", last_fax_printed);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
/*
static int _relog(int argc, char* argv[])
{
  fax_report_t r;

#ifdef HAVE_NETWORK
  fax_dbg_net_log_fax_host_status(HOST_STATUS_RELOG_ACK);
#endif
  if (argc != 2) return CMD_USAGE_ERROR;

  if      (!strcmp(argv[1], "cover"))    r = FAX_REPORT_COVER_PAGE;
  else if (!strcmp(argv[1], "activity")) r = FAX_REPORT_ACTIVITY_LOG;
  else if (!strcmp(argv[1], "settings")) r = FAX_REPORT_FAX_SETTING;
  else if (!strcmp(argv[1], "confirm"))  r = FAX_REPORT_FAX_CONFIRM;
  else                                   r = cmd_atoi(argv[1]);
  if (r >= FAX_REPORT_NUM_REPORTS) return CMD_USAGE_ERROR;
  if (fax_app_report_print(r))
  {
    cmd_printf("Encountered an error starting fax report\n");
    return CMD_ERROR;
  }
  return CMD_OK;
}
*/
/*----------------------------------------------------------------------------*/
static int _dnl(int argc, char* argv[])
{
  extern void _file_replace_dnl(char *filename);
  if (argc != 2) return CMD_USAGE_ERROR;
  _file_replace_dnl(argv[1]);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
/* This used by the "store" family of commands */
static fax_store_handle_t _store;

/*----------------------------------------------------------------------------*/
static int _store_create(fax_store_type_t type)
{
  fax_store_creation_t parms;
  if (_store)
  {
    cmd_printf("Store already in use!\n");
    return CMD_ERROR;
  }
  memset(&parms, 0, sizeof(parms));
  parms.type = type;
  _store = fax_store_create(&parms);
  if (_store) return CMD_OK;
  return CMD_ERROR;
}

/*----------------------------------------------------------------------------*/
static int _store_file( int argc, char *argv[] )
{
  if (argc != 1) return CMD_USAGE_ERROR;
  return _store_create(FAX_STORE_TYPE_RAMFS);
}

/*----------------------------------------------------------------------------*/
static int _store_destroy( int argc, char *argv[] )
{
  if (argc != 1) return CMD_USAGE_ERROR;
  if (!_store) return CMD_OK; /* note that we do not fail in this case */
  fax_store_destroy(_store);
  _store = NULL;
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _store_newpage( int argc, char *argv[] )
{
  fax_store_pg_handle_t page;
  if (argc != 1) return CMD_USAGE_ERROR;
  if (!_store)
  {
    cmd_printf("Store has not been created!\n");
    return CMD_ERROR;
  }
  if ((page = fax_store_page_open_new(_store, FAX_STORE_MODE_WRITE)))
  {
    cmd_printf("Page created\n");
    fax_store_page_close(page);
    return CMD_OK;
  }
  cmd_printf("Error creating page in store\n");
  return CMD_ERROR;
}

/*----------------------------------------------------------------------------*/
static int _store_write( int argc, char *argv[] )
{
  fax_store_pg_handle_t page;

  if (argc != 2) return CMD_USAGE_ERROR;

  if (!_store)
  {
    cmd_printf("Store has not been created!\n");
    return CMD_ERROR;
  }
  if (!(page = fax_store_page_open_first(_store, FAX_STORE_MODE_WRITE)))
  {
    cmd_printf("Page cannot be openned for writing\n");
    return CMD_ERROR;
  }
  if (OK!=fax_store_page_write_data(page,(uint8_t*)argv[1],strlen(argv[1])+1,1))
  {
    cmd_printf("Failed to write page\n");
    fax_store_page_close(page);
    return CMD_ERROR;
  }
  fax_store_page_close(page);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _store_read( int argc, char *argv[] )
{
  fax_store_pg_handle_t page;

  if (argc != 1) return CMD_USAGE_ERROR;

  if (!_store)
  {
    cmd_printf("Store has not been created!\n");
    return CMD_ERROR;
  }
  if (!(page = fax_store_page_open_first(_store, FAX_STORE_MODE_READ)))
  {
    cmd_printf("Page cannot be openned for reading\n");
    return CMD_ERROR;
  }
  while (1)
  {
    char data[40];
    bool comp;
    int n;
    n = fax_store_page_read_data(page, (uint8_t*)data, sizeof(data)-1, &comp);
    if (!n) break;
    data[n] = 0;
    cmd_printf("Read %d bytes: %s\n", n, data);
    if (comp) break;
  }
  fax_store_page_close(page);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _store_delpage( int argc, char *argv[] )
{
  fax_store_pg_handle_t page;

  if (argc != 1) return CMD_USAGE_ERROR;

  if (!_store)
  {
    cmd_printf("Store has not been created!\n");
    return CMD_ERROR;
  }
  if (!(page = fax_store_page_open_first(_store, FAX_STORE_MODE_READ)))
  {
    cmd_printf("Page cannot be openned for reading\n");
    return CMD_ERROR;
  }
  if (OK == fax_store_page_destroy(page))
    return CMD_OK;
  cmd_printf("Page cannot be destroyed\n");
  fax_store_page_close(page);
  return CMD_ERROR;
}

/*----------------------------------------------------------------------------*/
void fax_demo_init(void)
{
   cmd_register("fax", "Fax Commands", NULL, NULL, NULL);
   cmd_register("fax testsend", "Turn on test send loop", NULL, NULL,_testsend);
   cmd_register("fax endtest", "end test send loop", NULL, NULL, _endtest);
   cmd_register("fax reprint", "Reprint a fax", "<id>", NULL, _reprint);
   cmd_register("fax regstat", "Register for status callbacks",
                NULL, NULL, _regstat);
#ifdef HAVE_NETWORK
   cmd_register("fax netopen",
                "Set output IP address for fax app state notification",
                "<ip address>" , NULL, _netopen);
   cmd_register("fax netclose",
                "Disable network status of fax app states",
                NULL, NULL, _netclose);
#endif
   cmd_register("fax fnew", "Create a sample tif fax file",
                "<filename> [<pages>=1]", NULL, _fnew);
   cmd_register("fax fjpegnew", "Create a sample jpg fax file",
                "<filename> [<pages>=1]", NULL, _fjpegnew);
   cmd_register("fax list", "Show a list of faxes", "[<type>]", NULL, _list);
   cmd_register("fax list_reprint", "Show a list of reprintable faxes", NULL, NULL, _list_reprintable);
//   cmd_register("fax relog", "Print a report",
//                "0|cover|1|activity|2|settings|3|confirm", NULL, _relog);
   cmd_register("fax dnl", "Replace dnl (from a fax jpg file)",
                "<filename>", NULL, _dnl);

   /* Fax commands */
   cmd_register("fax cmd", "fax commands", NULL, NULL, NULL);
   cmd_register("fax cmd ans","Manually answer an incoming fax",NULL,NULL,_ans);
   cmd_register("fax cmd fsend", "Send a B&W fax from a file",
                "<filename> [<number>]",
                "This command sends a B&W fax from a file. The file "
                "should be a valid, fax tiff file.", _fsend);
   cmd_register("fax cmd fcolorsend", "Send a color fax from a file",
                "<filename> [<number>]",
                "This command sends a color fax from a file. The file "
                "should be a valid, fax jpg file.", _fcolorsend);
   cmd_register("fax cmd bwsend", "Send a black and white fax",
                "<number> [<cache>=1]",
                "This command sends a B&W fax from a scan. If cache "
                "is 1, which is the default, then the image is scanned "
                "before the fax is initiated. When cache is 0 the scan "
                "occurs during faxing.", _bwsend);
   cmd_register("fax cmd colorsend", "Send a color fax",
                "<number>", NULL, _colorsend);
   cmd_register("fax cmd graysend", "Send a grayscale fax",
                "<number>", NULL, _graysend);
   cmd_register("fax cmd rawsend", "Send raw fax using generated test data",
                "<number>", NULL, _rawsend);
   cmd_register("fax cmd cancel", "Cancel any current fax process",
                NULL, NULL, _cancel);

   /* Fax Settings */
   cmd_register("fax setting", "Get or Set Fax Settings", NULL, NULL, NULL);
   cmd_register("fax setting dialmode", "Get or Set the fax dialmode",
                "[1|pulse|0|tone]" , "0 is tone. 1 is pulse", _dialmode);
   cmd_register("fax setting ecm", "Get or Set the ECM setting",
                "[1|on|0|off]" , NULL, _ecm);
   cmd_register("fax setting vol", "Get or Set the fax audio volume",
                "[0|off|1|low|2|med|3|high]", NULL, _vol);
   cmd_register("fax setting printsize", "Specify the fax print size",
                "1|letter|5|legal|9|a4", NULL, _printsize);
   cmd_register("fax setting disring", "Get or Set the ring pattern",
                "0|all|1|single|2|double|3|tripple|4|both",
                "Specify what ring patterns to recognize. "
                "Setting 4 (or both) indicates to answer to either "
                "double or triple rings.", _disring);
   cmd_register("fax setting res","Get or Set the fax send resolution",
                "[0|standard|1|fine|2|veryfine|3|photo]", NULL, _res);
   cmd_register("fax setting bright","Get or Set the brightness",
                "[<brightness>]", "The range for brightness settings is "
                _STRINGIFY(FAX_APP_BRIGHTNESS_MIN) " to "
                _STRINGIFY(FAX_APP_BRIGHTNESS_MAX) ". Higher numbers "
                "are brighter.", _bright);
   cmd_register("fax setting speed","Get or Set the max fax speed",
                "[0|fast|1|medium|2|slow]", NULL, _speed);
   cmd_register("fax setting discallw",
                "Get or Set the disabled call waiting setting",
                "[0|off|1|on]", "note that this is a \"disable\" "
                "setting and therefore disable=1 or disable=on actually "
                "disables call waiting. What this command actually does "
                "is enable or disable the calling prefix. The calling "
                "prefix is usually such as necessary to disable call "
                "waiting. Very confusing isn't it?", _discallw);
   cmd_register("fax setting hname","Get or Set the header name",
                "[<new name>]", NULL, _hname);
   cmd_register("fax setting hnumber","Get or Set the header number",
                "[<new number>]", NULL, _hnumber);
   cmd_register("fax setting autoans",
                "Get or Set the auto answer setting",
                "[<new setting>]", "This can set then number of rings "
                "upon which to autoanswer. Use 0 to disable auto "
                "answer.", _autoans);
   cmd_register("fax setting stamp", "Enable or disable the stamp",
                "[1|0]", NULL, _stamp);
   cmd_register("fax setting noprint",
                "Enable or disable noprint mode",
                "[1|0]", NULL, _noprint);

   /* Fax Store cmds */
   cmd_register("fax store", "Fax store comamnds", NULL, NULL, NULL);
   cmd_register("fax store file", "Open a new store using file",
                NULL, NULL, _store_file);
   cmd_register("fax store destroy", "Open a new store using file",
                NULL, NULL, _store_destroy);
   cmd_register("fax store newpage", "Open a new store using file",
                NULL, NULL, _store_newpage);
   cmd_register("fax store write", "Open a new store using file",
                "<data>", "You will likely have to quote the data if "
                "you want to write a message that is more than one word",
                _store_write);
   cmd_register("fax store read", "Open a new store using file",
                NULL, NULL, _store_read);
   cmd_register("fax store delpage", "Open a new store using file",
                NULL, NULL, _store_delpage);
}
