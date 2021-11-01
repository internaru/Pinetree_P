/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <string.h>  // for memset
#include <pthread.h>
#include <posix_ostools.h>
#include <limits.h>
#ifdef __linux__
#include <stdio.h>
#else
#include "cpu_api.h"
#include "uart_api.h"
#endif // __linux__
#include "ATypes.h"
#include "lassert.h"
#include "dbg_api.h"
#include "dbg_outbuf.h"
#include "dbg_telnet.h"
#include "dbg_context.h"
#include "memAPI.h"

#ifndef UNUSED_VAR
#define UNUSED_VAR(x) ((x) = (x))
#endif

#ifndef __linux__
#define _OUTBUF_PRI  (30)
#endif
#define _BUFFER_SIZE_MASK (DEBUG_OUTBUF_SIZE - 1)
static char         _buffer[DEBUG_OUTBUF_SIZE];
static unsigned     _inpos, _outpos, _overflow = 0;

#ifdef __linux__
static pthread_t       _threadout;
static pthread_mutex_t _outbuf_lock = POSIX_MUTEX_INITIALIZER;

// In Linux, it is assumed all printing modules/apps will be executing the
// this debug module from a thread, so we can use pthread_cond_wait - in
// threadx (non-linux), we will continue to use event flags which can handle 
// calls from an ISR
static pthread_mutex_t dbg_data_mutex;
static pthread_cond_t  dbg_data_cond;
static uint32_t dbg_data_flags = 0;

#else
static TX_EVENT_FLAGS_GROUP _flags;
static TX_THREAD    _threadout;
#endif

static pthread_mutex_t _rcvr_mutex;
static pthread_mutex_t _outbuf_mutex;

#define _FLAG_DATA   (0x01)
#define _FLAG_NODATA (0x02)

static int          _shutdown = 0;

#define _RECVR_KEY1 (0x3456de23) /* Random */
#define _RECVR_KEY2 (0xea65e2c2) /* Random */
typedef struct __rcvr_t
{
    unsigned int      key1;
    dbg_receiver_cb_t cb;
    void*             cbdata;
    struct __rcvr_t  *next;
    unsigned int      key2;
} _rcvr_t;
static _rcvr_t* _rcvrs = NULL;

static unsigned char* _driver_buffer = NULL;
static int            _driver_bytes = 0;

/*----------------------------------------------------------------------------*/
void dbg_outbuf_shutdown(void)
{
    _shutdown = 1;
}

/*----------------------------------------------------------------------------*/
void dbg_outbuf_sync(void)
{
    if (dbg_context_is_thread())
    {
#ifdef __linux__
        pthread_t mythread;
#else
        TX_THREAD *mythread;
#endif

        /* Do not allow output thread to sync upon itself */
#ifdef __linux__
        mythread = pthread_self();
        if ( mythread != _threadout )
#else
        mythread = tx_thread_identify();
        if ( mythread != &_threadout )
#endif        
        {
#ifdef __linux__

            int px_status = 0;
            int policy;
            struct sched_param param;

	    UNUSED_VAR( px_status ); // lsptodo: xassert error handling in release build revisit
            px_status = pthread_getschedparam(mythread, &policy, &param);
            XASSERT( px_status==0, px_status );

            // we only adjust if the thread is following our system standard policy.
            // on linux the main thread does not follow the policy unless the process was
            // started with root access.
            if(policy == POSIX_DEFAULT_SCHED_POLICY)
            {
                /* Elevate (or set) the output thread priority to match our own */
                px_status = pthread_setschedparam(_threadout, policy, &param);
                XASSERT( px_status==0, px_status );
            }

            /* Wait until the output thread has caught up */
            px_status = pthread_mutex_lock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );
            while(!(dbg_data_flags & _FLAG_NODATA))
            {
                px_status = pthread_cond_wait(&dbg_data_cond, &dbg_data_mutex);
                XASSERT( px_status==0, px_status );
            }
            px_status = pthread_mutex_unlock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );

            /* Now drop priority back to its normal level */
            if(policy == POSIX_DEFAULT_SCHED_POLICY)
            {
                memset(&param, 0, sizeof(param));
                param.sched_priority = POSIX_THR_PRI_SERIAL_OUT;
                px_status = pthread_setschedparam(_threadout, POSIX_DEFAULT_SCHED_POLICY, &param);
                XASSERT( px_status==0, px_status );
            }
#else
            ULONG actual;
            UINT tx_res;
            CHAR *name;
            UINT state;
            ULONG run_count;
            UINT priority;
            UINT preemption_threshold;
            ULONG time_slice;
            TX_THREAD *next_thread;
            TX_THREAD *suspended_thread;
            UINT pri;

            tx_res = tx_thread_info_get( mythread,
                                         &name,
                                         &state,
                                         &run_count,
                                         &priority,
                                         &preemption_threshold,
                                         &time_slice,
                                         &next_thread,
                                         &suspended_thread );

            /* Elevate (or set) the output thread priority to match our own */
            pri = priority;
            tx_thread_priority_change(&_threadout, pri, &pri);

            /* Wait until the output thread has caught up */
            tx_event_flags_get(&_flags, _FLAG_NODATA, TX_OR,
                               &actual, TX_WAIT_FOREVER);

            /* Now drop priority back to its normal level */
            pri = _OUTBUF_PRI;
            tx_thread_priority_change(&_threadout, pri, &pri);
#endif            
        }
    }
}

/*----------------------------------------------------------------------------*/
void dbg_outbuf_add_char(char c)
{
#ifdef __linux__
    int px_status = 0; UNUSED_VAR(px_status); 
#endif
    
    if (_shutdown) return;

    /* We accept characters between 0x20 and 0x7f and certain control codes.
       Acceptable control codes are 8 - backspace, \t - tab, \n - LF
       Note that we are going to reject \r - CR to save space. CR's are put
       back into the stream later as it is read and emitted to the output */
    if ( (c > 0x7f) ||
         ((c < 0x20) && (c != 8) && (c != '\t') && (c != '\n')) )
        return;
        
#ifdef __linux__
    pthread_mutex_lock(&_outbuf_lock);
#else
    uint32_t interrupts;
    interrupts = cpu_disable_interrupts();
#endif        
    
    int nextpos = (_inpos + 1) & _BUFFER_SIZE_MASK;
    if (nextpos != _outpos) _buffer[_inpos = nextpos] = c;
    else                    _overflow = 1;
    
#ifdef __linux__
    pthread_mutex_unlock(&_outbuf_lock);
    
    px_status = pthread_mutex_lock(&dbg_data_mutex);
    XASSERT( px_status==0, px_status );
    dbg_data_flags &= ~_FLAG_NODATA;
    dbg_data_flags |= _FLAG_DATA;
    px_status = pthread_cond_broadcast(&dbg_data_cond);
    XASSERT( px_status==0, px_status );
    px_status = pthread_mutex_unlock(&dbg_data_mutex);
    XASSERT( px_status==0, px_status );
#else
    cpu_restore_interrupts(interrupts);
    tx_event_flags_set(&_flags, ~_FLAG_NODATA, TX_AND);
    tx_event_flags_set(&_flags,  _FLAG_DATA,   TX_OR);
#endif
}

/*----------------------------------------------------------------------------*/
char dbg_outbuf_get_char(void)
{
    /* If we have shutdown and there were unfinished bytes left in the uart
       driver than we replay the entire buffer as sent to the uart driver before
       providing data from the regular buffer. */
    while (_shutdown && _driver_bytes)
    {
        char c = *_driver_buffer++;
        _driver_bytes -= 1;
        if (c) return c;
    }
    while (_outpos != _inpos)
    {
        char c = _buffer[_outpos = (_outpos + 1) & _BUFFER_SIZE_MASK];
        if (c) return c;
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
void dbg_outbuf_replay(dbg_receiver_cb_t cb, void* arg)
{
    int s, i;
    if (!cb) return;
    s = (_inpos + 1) & _BUFFER_SIZE_MASK;
    for (i=0;i<DEBUG_OUTBUF_SIZE;i++)
    {
        char c = _buffer[s++];
        if (c) cb(&c, 1, arg); /* JSA - we can do better here */
        s &= _BUFFER_SIZE_MASK;
    }
}

/*----------------------------------------------------------------------------*/
static void _access_rcvrs(int lock_nUnlock)
{
    int px_status = 0; UNUSED_VAR(px_status);

    if (lock_nUnlock)
    {
        px_status = pthread_mutex_lock( &_rcvr_mutex );
        XASSERT( px_status==0, px_status );
    }
    else
    {
        px_status = pthread_mutex_unlock( &_rcvr_mutex );
        XASSERT( px_status==0, px_status );
    }
}

/*----------------------------------------------------------------------------*/
void dbg_watcher_unregister(void* handle)
{
    _rcvr_t* rcvr = (_rcvr_t*)handle;

    _access_rcvrs(1);
    _rcvr_t *r = _rcvrs;
    _rcvr_t *p = NULL;
    while (r && (r != rcvr)) { p = r; r = r->next; }
    if (r)
    {
        if (p) p->next = r->next;
        else   _rcvrs  = r->next;
    }
    _access_rcvrs(0);

    if ((rcvr->key1 == _RECVR_KEY1) && (rcvr->key2 == _RECVR_KEY2))
    {
        rcvr->key1 = ~_RECVR_KEY1;
        rcvr->key2 = ~_RECVR_KEY2;
        MEM_FREE_AND_NULL(rcvr);
    }
}

/*----------------------------------------------------------------------------*/
void* dbg_watcher_register(dbg_receiver_cb_t cb, void* arg)
{
    _rcvr_t *r;

    if (!cb) return NULL;

    r = (_rcvr_t*)MEM_MALLOC(sizeof(_rcvr_t));
    if (!r) return NULL;

    r->key1   = _RECVR_KEY1;
    r->key2   = _RECVR_KEY2;
    r->cb     = cb;
    r->cbdata = arg;
    _access_rcvrs(1);
    r->next = _rcvrs;
    _rcvrs = r;
    _access_rcvrs(0);
    return (void*)r;
}

#ifdef __linux__
static int _stream_send(FILE *stream, unsigned char *data, unsigned int len)
{
    int ret = 0;
    
    if (stream == 0 || data == 0 || len == 0)
    {
        return 0;
    }

    ret = fwrite(data, 1, len, stream);
    fflush(stream);
 
    return ret;
}
#endif

/*----------------------------------------------------------------------------*/
static void _send(void* hdl, unsigned char *data, unsigned int len)
{
    _rcvr_t* r;

    /* Send to uart unless telnet user wants local serial turned off */
#ifdef HAVE_CMD
if (!dbg_telnet_inhibit_local_console())
#endif    
    {
#ifdef __linux__
        _stream_send(hdl, _driver_buffer=data, _driver_bytes=len);
#else
        uartTransmit(hdl, UART_TRANSFER_MODE_CPU, _driver_buffer=data,
                                                 _driver_bytes=len, NULL);
#endif
      _driver_bytes = 0;
    }

    /* Now send to receivers. */
    _access_rcvrs(1);
    r = _rcvrs;
    while (r)
    {
        _rcvr_t* n = r->next;
        if      (r->key1 != _RECVR_KEY1) dbg_watcher_unregister(r);
        else if (r->key2 != _RECVR_KEY2) dbg_watcher_unregister(r);
        else if (!r->cb)                 dbg_watcher_unregister(r);
        else                             r->cb((char*)data, len, r->cbdata);
        r = n;
    }
    _access_rcvrs(0);
}

/*----------------------------------------------------------------------------*/
#ifdef __linux__
static void *_thread(void *arg)
#else
static void _thread(ULONG arg)
#endif
{
    static char _buf[512]; /* Must be 3 or larger */
#ifdef __linux__
    int px_status = 0; UNUSED_VAR(px_status);
#else
    ULONG actual;
    uint32_t interrupts;
#endif    
    
    void* hdl = (void*)arg;

    /* Terminal color formatting is as follows:
       fg   bg   color
       30 - 40 - black
       31 - 41 - red
       32 - 42 - green
       33 - 43 - yellow
       34 - 44 - blue
       35 - 45 - magenta
       36 - 46 - cyan
       37 - 47 - white
       0-reset, 1-bright, 2-dim, 4-underscore, 5-blink, 7-reverse, 8-hidden */
    static const char* _overflow_msg = "\033[30;41mCHARS DROPPED\033[0m\n\r";

    int pos, overflow;

    while (1)
    {
#ifdef __linux__
        px_status = pthread_mutex_lock(&dbg_data_mutex);
        XASSERT( px_status==0, px_status );
        while(!(dbg_data_flags & _FLAG_DATA))
        {
            px_status = pthread_cond_wait(&dbg_data_cond, &dbg_data_mutex);
            XASSERT( px_status==0, px_status );
        }
        dbg_data_flags &= ~_FLAG_DATA;
        px_status = pthread_mutex_unlock(&dbg_data_mutex);
        XASSERT( px_status==0, px_status );
#else
        tx_event_flags_get(&_flags,_FLAG_DATA,TX_OR_CLEAR,&actual,TX_WAIT_FOREVER);
#endif        
      
        pos = overflow = 0;
        
#ifdef __linux__
        pthread_mutex_lock(&_outbuf_lock);
#else
        interrupts = cpu_disable_interrupts();
#endif        
            while (1)
            {
                if (_outpos != _inpos)
                {
                char c = _buffer[_outpos = ((_outpos+1)&_BUFFER_SIZE_MASK)];
                    
                    if (c == '\n') 
                        _buf[pos++] = '\r';
                        
                    _buf[pos++] = c;
                    
                    if (pos < sizeof(_buf)-2) 
                        continue;
                }
                else if (_overflow) /* Detect overflow only once buffer is drained */
                {
                    overflow = !(_overflow = 0);
                }
                
                break;
            }
#ifdef __linux__
        pthread_mutex_unlock(&_outbuf_lock);
#else
        cpu_restore_interrupts(interrupts);
#endif

        if (pos)
        {
#ifdef __linux__
            px_status = pthread_mutex_lock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );
            dbg_data_flags &= ~_FLAG_NODATA;
            dbg_data_flags |= _FLAG_DATA;
            px_status = pthread_cond_broadcast(&dbg_data_cond);
            XASSERT( px_status==0, px_status );
            px_status = pthread_mutex_unlock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );
#else
            tx_event_flags_set(&_flags, ~_FLAG_NODATA, TX_AND);
            tx_event_flags_set(&_flags,  _FLAG_DATA,   TX_OR);
#endif
            
            _send(hdl, (unsigned char*)_buf, pos);
        }
        else
        {
#ifdef __linux__
            px_status = pthread_mutex_lock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );
            dbg_data_flags |= _FLAG_NODATA;
            px_status = pthread_cond_broadcast(&dbg_data_cond);
            XASSERT( px_status==0, px_status );
            px_status = pthread_mutex_unlock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );
#else
            tx_event_flags_set(&_flags, _FLAG_NODATA, TX_OR);
#endif
        }

        if (overflow)
        {
#ifdef __linux__
            px_status = pthread_mutex_lock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );
            dbg_data_flags |= _FLAG_NODATA;
            px_status = pthread_cond_broadcast(&dbg_data_cond);
            XASSERT( px_status==0, px_status );
            px_status = pthread_mutex_unlock(&dbg_data_mutex);
            XASSERT( px_status==0, px_status );
#else
            tx_event_flags_set(&_flags, _FLAG_NODATA, TX_OR);
#endif
            
            _send(hdl, (unsigned char*)_overflow_msg, 27);
        }
    }

#ifdef __linux__
    return 0;
#endif
}

/*----------------------------------------------------------------------------*/
void dbg_outbuf_mutex(int acquire_nRelease)
{
    int px_status = 0; UNUSED_VAR(px_status);

    if (dbg_context_is_thread())
    {
        if (acquire_nRelease)
        {
            px_status = pthread_mutex_lock( &_outbuf_mutex );
            XASSERT( px_status==0, px_status );
        }
        else
        {
            px_status = pthread_mutex_unlock( &_outbuf_mutex );
            XASSERT( px_status==0, px_status );
        }
    }
}

/*----------------------------------------------------------------------------*/
#define THREADOUT_STACK_SIZE POSIX_MIN_STACK_SIZE

void dbg_outbuf_init(void* hdl)
{
    static ALIGN8 char _stackout[THREADOUT_STACK_SIZE]; /* JSA - size me */
    int px_status = 0; UNUSED_VAR(px_status);

    _inpos = _outpos = 0;
    px_status = posix_mutex_init( &_outbuf_mutex );
    XASSERT( px_status==0, px_status );
    px_status = posix_mutex_init( &_rcvr_mutex );
    XASSERT( px_status==0, px_status );
    
#ifdef __linux__
    px_status = posix_mutex_init( &dbg_data_mutex );
    XASSERT( px_status==0, px_status );
    px_status = pthread_cond_init( &dbg_data_cond, NULL );
    XASSERT( px_status==0, px_status );
    
    dbg_data_flags = 0;
    dbg_data_flags |= _FLAG_NODATA;
    
    px_status = posix_create_thread(&_threadout, _thread, hdl, "DebugOut", &_stackout, sizeof(_stackout), POSIX_THR_PRI_SERIAL_OUT);
    XASSERT( px_status==0, px_status );

#else
    tx_event_flags_create(&_flags, "DebugOut");
    tx_event_flags_set(&_flags,~_FLAG_DATA,   TX_AND);
    tx_event_flags_set(&_flags, _FLAG_NODATA, TX_OR);
    tx_thread_create(&_threadout, "DebugOut", _thread, (ULONG)hdl,
                     _stackout, sizeof(_stackout),
                     _OUTBUF_PRI, _OUTBUF_PRI, TX_NO_TIME_SLICE, TX_AUTO_START);
#endif

    /* Make sure DEBUG_OUTBUF_SIZE is a power of two. Note that this statement
       does not generate any actual code. It is simply checked by the
       compiler. If you get a compilation error on the next line(s) of code
       then you need to define DEBUG_OUTBUF_SIZE to be a power of two */
#define _COMPILE_TIME_ASSERT(pred) switch(0){case 0:case pred:;}
        _COMPILE_TIME_ASSERT(DEBUG_OUTBUF_SIZE && !(DEBUG_OUTBUF_SIZE & (DEBUG_OUTBUF_SIZE-1)));
}
