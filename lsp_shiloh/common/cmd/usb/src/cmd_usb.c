/*
 * ============================================================================
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

//--------------------------------------
// Included files
//--------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "tx_api.h"
#include "dprintf.h"
#include "lassert.h"
#include "platform.h"
#include "sys_init_api.h"
#include "platform_api.h"
#include "memAPI.h"

#include "usb_device_api.h"
#include "cmd_wrapper_api.h"

#define _dbg(fmt, ...)
#define _inf(fmt, ...)
#define _err(fmt, ...) dbg_printf(fmt, ## __VA_ARGS__)

/*----------------------------------------------------------------------------*/
static void _write(void *client_data, char *data, unsigned int data_len)
{
  char* d;
  if (!data || !data_len) return;
  _dbg("USBCMD: _write(%u)\n", data_len);
  
  if (!(d = MEM_MALLOC(data_len)))
  {
    _err("USBCMD: memory allocation failed!\n");
    return;
  }
  memcpy(d, data, data_len);
  if (0 >= usb_cmd_device_write( (uint8_t*)d, data_len, 0 ))
  {
    _err("USBCMD: write failed!\n");
  }
  /* d will be freed by the usb write */
}

/*----------------------------------------------------------------------------*/
static uint32_t _read( char * buffer, uint32_t len, uint32_t timeout )
{
    int n, m;

    _dbg("USBCMD: _read(%lu)\n", len);

    /* Wait for the first byte */
    n = usb_cmd_device_read( (uint8_t*)buffer, 1, timeout);
    if (n > 0)
    {
      /* Read the rest of the available data (no timeout) */
      m = usb_cmd_device_read( (uint8_t*)buffer+1, len-1, 0);
      if (m > 0) n += m;
      _inf("USBCMD: read %d bytes from usb\n", n);
    }
    return n;
}

/*----------------------------------------------------------------------------*/
static void _thread_entry( ULONG thread_param )
{
  static char _buffer[4096];
  error_type_t e_res;
  uint16_t io_timeout_in_seconds;
  int restart = 1;
  void* wrapper = NULL;

  /* Wait for System Init */
  SysWaitForInit( );

  e_res = platvars_get_io_timeout(&io_timeout_in_seconds);
  ASSERT( OK == e_res );

  while (1)
  {
    if (restart)
    {
      _dbg("USBCMD: restarting\n");
      if (wrapper)
      {
        _dbg("USBCMD: destroying\n");
        cmd_wrapper_delete( wrapper );
        wrapper = NULL;
      }
      _dbg("USBCMD: creating\n");
      wrapper = cmd_wrapper_create( "usb_cmd_interface");
      ASSERT( wrapper );
      restart = 0;
    }

    int n = _read( _buffer, sizeof(_buffer),
                   io_timeout_in_seconds * TIMER_INTS_PER_SECOND);
    _dbg("USBCMD: got %d from _read()\n", n);
    if (n < 0) /* Error - avoid tight loop - wait before polling again */
    {
      tx_thread_sleep(TIMER_INTS_PER_SECOND);
    }
    if (n <= 0)
    {
      restart = 1;
    }
    else
    {
      cmd_wrapper_pipeline(wrapper, _buffer, n, _write, NULL);
      _inf("USBCMD: %d bytes have been processed\n", n);
    }
  }
}

/*----------------------------------------------------------------------------*/
void cmd_usb_device_init(void)
{
  static TX_THREAD      _thread;
  static ALIGN8 uint8_t _stack[ 8192 ];

  /* start the thread */
  tx_thread_create( &_thread, "CMD USB Parser",
                    _thread_entry, 0,
                    _stack, sizeof(_stack),
                    THR_PRI_NORMAL, THR_PRI_NORMAL,
                    DEFAULT_TIME_SLICE, TX_AUTO_START );
}
