/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <stdarg.h>
#include <stdbool.h>
#include "strfmt.h"
#include <pthread.h>
#include <posix_ostools.h>
#ifdef __linux__
#include <stdio.h>
#include <termios.h>
#else
#include "hwconfig_api.h"
#include "uart_api.h"
#endif
#include "dbg_api.h"
#include "dbg_outbuf.h"
#include "dbg_telnet.h"
#include "dbg_raw_socket.h"
#include "dbg_context.h"
#include "dbg_cmd.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

static int _initted = 0;

/*----------------------------------------------------------------------------*/
static int _printf_enable = DEBUG_ENABLE;
static int _printf_blocking = DEBUG_BLOCKING;

/*----------------------------------------------------------------------------*/
static void _add_char(char c, void* unused)
{
  if (_printf_enable) dbg_outbuf_add_char(c);
}

/*----------------------------------------------------------------------------*/
static int _printf(const char* fmt, ...)
{
  int len;	
  va_list ap;
  va_start(ap, fmt);
  len = vfstrfmt(_add_char, NULL, fmt, ap);
  va_end(ap);
  return len;
}

/*----------------------------------------------------------------------------*/
static int  _timestamp_enable = 0;
extern bool logger_get_timestamp_on( void );

#ifdef HAVE_TIMER
#include "timer_api.h"
static int _handle_timestamp(void)
{
  extern bool logger_get_timestamp_usec( void );
  uint32_t (*fn)(void) = NULL;
  int len = 0;

  if ( logger_get_timestamp_on() )
  {
    if ( logger_get_timestamp_usec() )
    {
      fn = timer_get_time_usec;
    }
    else
    {
      fn = posix_gettime_ms;
    }
  }
  else if ( _timestamp_enable )
  {
    fn = posix_gettime_ms;
  }
  if (fn) len = _printf("%u: ", fn());
  return len;
}
#else
static int _handle_timestamp(void)
{
  int len = 0;	
  if ( _timestamp_enable || logger_get_timestamp_on() )
    len = _printf("%u: ", posix_gettime_ms());
  return len;
}

#endif

/*----------------------------------------------------------------------------*/
static int _namestamp_enable = 0;
static int _handle_namestamp(void)
{
  int len = 0;
  if (_namestamp_enable)
  {
    if (dbg_context_is_thread())  
      len = _printf("[%d] ", pthread_self());
    else
      len = _printf("[%s] ", "ISR/Timer");
  }
  return len;
}

/*----------------------------------------------------------------------------*/
int 
dbg_printf(const char* fmt, ...)
{
  va_list ap;
  int r;

  if (!fmt || !*fmt) return -1; 
  if (!_initted) return -2;

  va_start(ap, fmt);
  dbg_outbuf_mutex(1);
  r = _handle_timestamp();
  r += _handle_namestamp();
  r += vfstrfmt(_add_char, NULL, fmt, ap);
  dbg_outbuf_mutex(0);
  va_end(ap);
  if (_printf_blocking) dbg_outbuf_sync();
  return ((r >= 0)?r:-1);
}

#ifdef HAVE_UFS
/*----------------------------------------------------------------------------*/
#include "ufs_stdio_api.h"
static ufs_ssize_t _stdout(const void* buf, ufs_size_t count)
{
  /* Skip the entire thing if printf is turned off */
  if (!_printf_enable) return count;

  /* Copy all data to the output buffer (with mutex protection) */
  ufs_size_t c = count;
  char* b = (char*)buf;
  dbg_outbuf_mutex(1);
  while (c--) dbg_outbuf_add_char(*b++);
  dbg_outbuf_mutex(0);
  return count;
}
#endif


/*----------------------------------------------------------------------------*/
void dbg_sync(void)
{
  dbg_outbuf_sync();
}

#ifdef HAVE_CMD
/* -------------------------------------------------------------------------- */
static const char* _dbg_sync_usage ="";
static const char* _dbg_sync_notes ="Use this command to wait for the "
  "uart to catch up with the queued messages. This can be used during periods "
  "of massive amount of uart traffic to synchronize the foreground process(es) "
  "with the background uart task.";
static int _dbg_sync(int argc, char *argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
  dbg_sync();
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
void dbg_printf_control(int enable, int blocking)
{
  _printf_enable = enable;
  _printf_blocking = blocking;
}

/* -------------------------------------------------------------------------- */
static const char* _enable_usage = "[<enable>]";
static const char* _enable_notes ="Use this command to set or query the enable "
  "for dbg_printf statements.";
static int _enable(int argc, char *argv[])
{
  if (argc > 2) return CMD_USAGE_ERROR;
  if (argc > 1) _printf_enable = cmd_atoi(argv[1]);
  cmd_append_result("%d", _printf_enable);
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static const char* _blocking_usage = "[<blocking>]";
static const char* _blocking_notes ="Use this command to set or query the "
  "blocking behavior for dbg_printf statements.";
static int _blocking(int argc, char *argv[])
{
  if (argc > 2) return CMD_USAGE_ERROR;
  if (argc > 1) _printf_blocking = cmd_atoi(argv[1]);
  cmd_append_result("%d", _printf_blocking);
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static const char* _replay_usage ="";
static const char* _replay_notes ="Use this command to replay the console "
  "output messages. This will dump the internal buffer of debug messages to "
  "the CMD result.";
static void _replay_cb(char* c, int len, void * arg)
{
  while (len--) cmd_append_result("%c", *c++);
}
static int _replay(int argc, char *argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
  dbg_outbuf_replay(_replay_cb, NULL);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
void dbg_printf_timestamp(int enable)
{
  _timestamp_enable = enable;
}

/* -------------------------------------------------------------------------- */
static const char* _timestamp_usage = "[<enable>]";
static const char* _timestamp_notes = "Use this command to set or query the "
  "enable for dbg_printf timestamping.";
static int _timestamp(int argc, char *argv[])
{
  if (argc > 2) return CMD_USAGE_ERROR;
  if (argc > 1) _timestamp_enable = cmd_atoi(argv[1]);
  cmd_append_result("%d", _timestamp_enable);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
void dbg_printf_namestamp(int enable)
{
  _namestamp_enable = enable;
}

/* -------------------------------------------------------------------------- */
static const char* _namestamp_usage = "[<enable>]";
static const char* _namestamp_notes = "Use this command to set or query the "
  "enable for dbg_printf namestamping.";
static int _namestamp(int argc, char *argv[])
{
  if (argc > 2) return CMD_USAGE_ERROR;
  if (argc > 1) _namestamp_enable = cmd_atoi(argv[1]);
  cmd_append_result("%d", _namestamp_enable);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
#include "lassert.h"
static int _assert_cmd(int argc, char *argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
  REL_ASSERT(0);
  return CMD_OK;
}
#endif // HAVE_CMD

/*----------------------------------------------------------------------------*/
void dbg_console_task_init(void) /* Called from main() */
{
  static void* _hdl_in = NULL;
  static void* _hdl_out = NULL;

#ifdef __linux__
  // only if stdin/stdout is a terminal will stdin/stdout be used
  if(tcgetpgrp(fileno(stdin)) == getpgrp()) 
  {
      struct termios term_attr;

      // set stdin to non-canonical mode.  This allows characters to
      // be read immediatly rather than waiting for eol, which in turn
      // allows arrow keys to be handled correctly. 
      tcgetattr(fileno(stdin), &term_attr);
      term_attr.c_lflag &= ~(ICANON);
      tcsetattr(fileno(stdin), TCSANOW, &term_attr);

      _hdl_in = stdin;
  }
  
//  if(tcgetpgrp(fileno(stdout)) == getpgrp()) 
  {
    _hdl_out = stdout;
  }
#else
  static void* _uart = NULL;
  static UART_CONFIG _config = { 115200,
                                 UART_DATA_BITS_8,
                                 UART_STOP_BITS_1,
                                 UART_PARITY_NONE };

  uartOpen(hwGetDebugUARTNumber(), &_uart);
  uartSetConfig(_uart, &_config);
  
  if (_uart)
  {
     _hdl_in = _uart;
     _hdl_out = _uart;
  }
#endif

  dbg_outbuf_init(_hdl_out);
#ifdef HAVE_CMD
  dbg_telnet_init();
  dbg_cmd_init(_hdl_in);
#endif  

  _initted = 1;
  
#ifdef HAVE_CMD
  cmd_register("dbg", "Debug commands", NULL, NULL, NULL);
  cmd_register("dbg sync", "Wait for the background UART to catch up",
               _dbg_sync_usage, _dbg_sync_notes, _dbg_sync);
  cmd_register("dbg enable", "Enable/disable debug statements",
               _enable_usage, _enable_notes, _enable);
  cmd_register("dbg blocking", "Enable/disable dbg blocking IO",
               _blocking_usage, _blocking_notes, _blocking);
  cmd_register("dbg replay", "Replay the most recent debug messages",
               _replay_usage, _replay_notes, _replay);
  cmd_register("dbg stamp", "Enable/disable dbg stamping", NULL, NULL, NULL);
  cmd_register("dbg stamp time", "Enable/disable dbg time stamping",
               _timestamp_usage, _timestamp_notes, _timestamp);
  cmd_register("dbg stamp name", "Enable/disable dbg name stamping",
               _namestamp_usage, _namestamp_notes, _namestamp);
  cmd_register("dbg assert", "Invoke the assert handler (for test)",
               NULL, NULL, _assert_cmd);
#endif
#ifdef HAVE_UFS
  ufs_stdio_register_stdout_hdlr(_stdout);
  ufs_stdio_register_stderr_hdlr(_stdout);
#endif
}
