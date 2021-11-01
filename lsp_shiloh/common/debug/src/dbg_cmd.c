/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <string.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "memAPI.h"
#include "ATypes.h"
#ifdef __linux__
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#else
#include "uart_api.h"
#endif
#include "cmd_wrapper_api.h"
#include "dbg_outbuf.h"
#include "dbg_telnet.h"
#include "dbg_raw_socket.h"

#define _CMD_HIST_NUM (128)
#define _CMD_HIST_MAX (_CMD_HIST_NUM-1)
static char* _hist[_CMD_HIST_NUM];

#ifndef UNUSED_VAR 
 /*!
 * \def UNUSED_VAR(x) 
 * a simple macro that will turn off the unused var compiler warning for a given var
 **/
  #define UNUSED_VAR(x) ((x) = (x))  
#endif

/*----------------------------------------------------------------------------*/
static void _putc(char  c)
{
  dbg_outbuf_add_char(c);
}

/*----------------------------------------------------------------------------*/
static int _puts(char* s)
{
  int len = 0;
  char c;
  while ((c = *s++))
  {
    len++;
    dbg_outbuf_add_char(c);
  }
  return len;
}

/*----------------------------------------------------------------------------*/
static void _cmd_output(void* data, char* d, unsigned int l)
{
  dbg_outbuf_mutex(1);
  while (l--) dbg_outbuf_add_char(*d++);
  dbg_outbuf_mutex(0);
  dbg_outbuf_sync();
}

/*----------------------------------------------------------------------------*/
static void _add_to_history(char* cmd, int len)
{
  if (!_hist[0] || strcmp(cmd, _hist[0]))
  {
    char* newptr = (char*)MEM_MALLOC(len+1);
    if (newptr)
    {
      int i;
      if (_hist[_CMD_HIST_MAX]) MEM_FREE_AND_NULL(_hist[_CMD_HIST_MAX]);
      for (i=_CMD_HIST_MAX;i>0;i--)
        _hist[i] = _hist[i-1];
      _hist[0] = newptr;
      strcpy(_hist[0], cmd);
    }
  }
}


/* -------------------------------------------------------------------------- */
static void* _serial_cmd(void)
{
  static void* _interp = NULL;
  if (!_interp) _interp = cmd_wrapper_create("Serial");
  return _interp;
}

#ifdef __linux__
static int _is_stream_ready (int fd, uint32_t timeout_ms)
{
    int ret = 0;
    
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);
    
    struct timeval timeout;
    timeout.tv_sec = timeout_ms/MILLISEC_PER_SECOND;
    timeout.tv_usec = (timeout_ms%MILLISEC_PER_SECOND) * USEC_PER_MILLISEC;

    if (select(fd+1, &fdset, NULL, NULL, &timeout) == 1)
    {
        int len = 0;
        if (ioctl(fd, FIONREAD, &len) != -1)
        {
            ret = len;
        }
    }
    
    return ret;
}

static int _stream_receive(FILE *stream, unsigned char *data, unsigned int len, unsigned int timeout_ms)
{
    int bytes;
    
    if (stream == 0 || data == 0 || len == 0 )
    {
        return 0;
    }
    
    if ((bytes = _is_stream_ready(fileno(stream), timeout_ms)) > 0)
    {
        if (bytes > len)
            bytes = len;
            
        return fread(data, 1, bytes, stream);
    }
 
    return 0;
}
#endif

/*----------------------------------------------------------------------------*/
#define _PROMPT "\nCMD==> "
#ifdef __linux__
#define _RUBOUT_ARROW "\b\b\b\b    \b\b\b\b"
#define _RUBOUT_CNTL "\b\b  \b\b"
#endif
#define _RUBOUT "\b \b"
static void *_thread(void *arg)
{
  static unsigned char _buf[256];
  static char          _line[256];
  uint32_t num;  
  int i, cmdlen, escape, arrow, hidx;
  void* hdl = (void*)arg;

  hidx = 0;
  cmdlen = escape = arrow = 0;
  while (1)
  {
    /* Acquire new data */
    i = dbg_telnet_getchar();
    if (i >= 0)
    {
      _buf[0] = i;
      num = 1;
    }
    else
    {
        num = dbg_socket_receive(_buf, (int)sizeof(_buf), 100);
        if (num <= 0)
        {
#ifdef __linux__
            num = _stream_receive(hdl, _buf, sizeof(_buf), 100);
            if (num <= 0)
#else
            if (OK != uartReceive(hdl, UART_TRANSFER_MODE_CPU,
                                sizeof(_buf), 100, _buf, &num))
#endif                            
            {
                posix_sleep_ticks(1);
                continue;
            }
        }
    }

    for (i=0; i<num; i++)
    {
      unsigned c = _buf[i];
      if (c == 0x1b) /* ESC */
        escape = !(arrow = 0);
      else if (escape && (c == '['))
        arrow = !(escape = 0);
      else if (arrow) /* Handle arrow commands */
      {
#ifdef __linux__
        // linux will echo the arrow keys
        _puts(_RUBOUT_ARROW);
#endif
        if      ((c == 'A') && (hidx < _CMD_HIST_MAX) && (_hist[hidx]))
        {
          while (cmdlen--) 
            _puts(_RUBOUT);
          cmdlen = _puts(strcpy(_line, _hist[hidx++]));
        }
        else if ((c == 'B') && hidx) /* Down */
        {
          while (cmdlen--)
            _puts(_RUBOUT);
          cmdlen = _puts(strcpy(_line, _hist[--hidx]));
        }
        else if ((c == 'D') && cmdlen) /* Left is like delete from end */
        {
          _line[--cmdlen] = 0;
          _puts(_RUBOUT);
        }
        arrow = escape = 0;
      }
      else if ((c == 0x0d) || (c == 0x0a)) /* CR or LF */
      {
        if (cmdlen)
        {
          _line[cmdlen] = 0;
          _putc('\n');
          cmd_wrapper_runcmd(_serial_cmd(), _line, _cmd_output, NULL);
          _add_to_history(_line, cmdlen);
          _line[cmdlen = 0] = 0;
        }
        _puts(_PROMPT);
        hidx = arrow = escape = 0;
      }
      else if ((c == 0x08) || (c == 0x7f)) /* BS or DEL */
      {
#ifdef __linux__
        // linux will echo the control keys
        _puts(_RUBOUT_CNTL);
#endif
        if (cmdlen)
        {
          _line[--cmdlen] = 0;
          _puts(_RUBOUT);
        }
        arrow = escape = 0;
      }
      else if ((c == 0x15) || (c == 0x03)) /* Ctrl-U or Ctrl-C; Start over */
      {
#ifdef __linux__
        // linux will echo the control keys
        _puts(_RUBOUT_CNTL);
#endif
        while (cmdlen--) _puts(_RUBOUT);
        _line[++cmdlen] = 0;
        hidx = arrow = escape = 0;
      }
      else if ((c >= 0x20) && (c <= 0x7e))
      {
        _line[cmdlen++] = c;
        if (cmdlen == sizeof(_line)) cmdlen--;
#ifdef __linux__
        //  already being echo'd on linux, so don't repeat
#else
        else                         _putc(c);
#endif        
        _line[cmdlen] = 0;
        arrow = escape = 0;
      }
    }
  }
  
  return NULL;
}

void dbg_cmd_init(void* hdl)
{
  _serial_cmd(); /* Make sure it is created as part of init */
  static pthread_t   _threadin;
  static ALIGN8 uint8_t _stackin[POSIX_MIN_STACK_SIZE]; /* JSA - size me */
  int px_status = 0; UNUSED_VAR(px_status); // release ignore OS failure  

  px_status = posix_create_thread(&_threadin, _thread, hdl, "DebugIn", &_stackin, sizeof(_stackin), POSIX_THR_PRI_SERIAL_IN);
  XASSERT( px_status==0, px_status );
}

void serial_console_cmd_run(char *cmd){
	cmd_wrapper_runcmd(_serial_cmd(), cmd, _cmd_output, NULL);
}
