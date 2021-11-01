/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <pthread.h>
#include <posix_ostools.h>
#include "lassert.h"
#include "dbg_telnet.h"
#include "dbg_api.h"

#ifdef HAVE_DBG_TELNET
#include "net_telnet_api.h"

static net_telnet_hndl_t    _hdl;
static int                  _net = 0;      /* Are we currently connected */
static char                *_inbuf = 0;    /* Network input */
static uint32_t             _inlen = 0;    /* How much network input */
static pthread_mutex_t dbg_telnet_mutex;
static pthread_cond_t  dbg_telnet_cond;
static uint32_t dbg_telnet_data_flags = 0;
#define _DISCONNECTED  (0x01)
#define _DATA_AVAIL    (0x02)
#define _DATA_CONSUMED (0x04)

/* Return 0 unless telnet wishes to have local console output inhibitted */
int dbg_telnet_inhibit_local_console(void)
{
  return _net;
}

/* Return <0 if telnet is not open */
int dbg_telnet_getchar(void)
{
  ULONG actual;
  if (!_net) return -1;
  
  pthread_mutex_lock(&dbg_telnet_mutex);
  while(!(dbg_telnet_data_flags & (_DATA_AVAIL | _DISCONNECTED)))
  {
    pthread_cond_wait(&dbg_telnet_cond, &dbg_telnet_mutex);
    actual = dbg_telnet_data_flags;
  }
  pthread_mutex_unlock(&dbg_telnet_mutex);
  
  if (_inlen && (actual & _DATA_AVAIL))
  {
    char c = *_inbuf++;
    if (!--_inlen)
    {
      pthread_mutex_lock(&dbg_telnet_mutex);
      dbg_telnet_data_flags &= ~_DATA_AVAIL;
      dbg_telnet_data_flags |= _DATA_CONSUMED;
      pthread_cond_broadcast(&dbg_telnet_cond);
      pthread_mutex_unlock(&dbg_telnet_mutex);
    }
    return c;
  }
  return -1;
}

void _telnet_rcvr(char* c, int len, void* arg)
{
  if (_net) net_telnet_outgoing_data(c, len, _hdl);
}

void _telnet_open(net_telnet_hndl_t hdl)
{
  pthread_mutex_lock(&dbg_telnet_mutex);
  dbg_telnet_data_flags &= ~_DISCONNECTED;
  dbg_telnet_data_flags &= ~_DATA_AVAIL;
  pthread_cond_broadcast(&dbg_telnet_cond);
  pthread_mutex_unlock(&dbg_telnet_mutex);
      
  _inlen = 0;
  _net = 1;
}

void _telnet_closed(net_telnet_hndl_t hdl)
{
  _net = 0;
  pthread_mutex_lock(&dbg_telnet_mutex);
  dbg_telnet_data_flags |= _DISCONNECTED;
  pthread_cond_broadcast(&dbg_telnet_cond);
  pthread_mutex_unlock(&dbg_telnet_mutex);
}

void _telnet_data(char* buf, uint32_t len, net_telnet_hndl_t hdl)
{
  _inlen = len;
  _inbuf = buf;
  
  pthread_mutex_lock(&dbg_telnet_mutex);
  dbg_telnet_data_flags &= ~_DATA_CONSUMED;
  dbg_telnet_data_flags |= _DATA_AVAIL;
  pthread_cond_broadcast(&dbg_telnet_cond);

  while(!(dbg_telnet_data_flags & _DATA_CONSUMED))
  {
    pthread_cond_wait(&dbg_telnet_cond, &dbg_telnet_mutex);
  }
  pthread_mutex_unlock(&dbg_telnet_mutex);
}

static net_telnet_func_table_t _telnet_tbl = 
{
  .opened_func = _telnet_open,
  .closed_func = _telnet_closed,
  .incoming_data_func = _telnet_data
};

void dbg_telnet_init(void)
{
    int px_status;

    px_status = posix_mutex_init( &dbg_telnet_mutex );
    XASSERT( px_status==0, px_status );
    px_status = pthread_cond_init( &dbg_telnet_cond, NULL );
    XASSERT( px_status==0, px_status );

    dbg_telnet_data_flags = 0;
    dbg_telnet_data_flags |= _DISCONNECTED;
    
    net_telnet_register(_telnet_tbl, NET_TELNET_DEFAULT_PORT, &_hdl);
    dbg_watcher_register(_telnet_rcvr, (void*)&_net);
}

#else

/* Return <0 if telnet is not open */
int dbg_telnet_getchar(void)
{
  return -1;
}

/* Return <0 if telnet is not open or an error occurred */
int dbg_telnet_putchar(char c)
{
  return -1;
}

void dbg_telnet_init(void)
{
}

/* Return 0 unless telnet wishes to have local console output inhibitted */
int dbg_telnet_inhibit_local_console(void)
{
  return 0;
}

#endif
