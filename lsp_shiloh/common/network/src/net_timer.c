/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "posix_ostools.h"
#include "memAPI.h"
#include "platform.h"
#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include "net_api.h"
#include "net_logger.h"
#ifdef HAVE_ASIC_POWER_DRIVER
#include "asic_pwr_drv_api.h"
#endif // HAVE_ASIC_POWER_DRIVER
#include "delay_api.h" // delay_msec_timer_non_blocking()

#ifdef __linux__
#include <signal.h> // SIGSTOP
#include "timerfd.h" // TFD_CLOEXEC
#define MSEC_PER_SEC  1000L
#define USEC_PER_MSEC 1000L
#define NSEC_PER_USEC 1000L
#define NSEC_PER_MSEC (long)(NSEC_PER_USEC * MSEC_PER_SEC)
#define NSEC_PER_SEC  (long)(NSEC_PER_MSEC * MSEC_PER_SEC) /* 1000000000L guarded w/__KERNEL__ in <time.h> */
#endif // __linux__

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#ifdef __linux__
#define DBG_PRFX "net.timer: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
#include "net_debug.h"

#else // ! __linux__
#define DBG_PRFX "NET: "
#define LOGGER_MODULE_MASK (DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_IFACE) 

#define DBG_CRIT(...) DBG_PRINTF(LOG_CRIT, DBG_PRFX __VA_ARGS__)
#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#ifdef HAVE_CMD
#define DBG_CMD(...) cmd_printf( __VA_ARGS__ )
#endif

#endif // ! __linux__

#define NET_TIMER_ID            0xD8E7F6A9  // unique struct id for error checking

#define NET_TIMER_MAX_HANDLES   20 // limits max simultaneous timers


//=========================================================================================================================
// RTOS net timer implementation
//=========================================================================================================================

typedef struct net_timer_s
{
    uint32_t            timer_id;
    net_timer_flags_t   flags;
    net_timer_expiration_func_t expiration_func;
    uint32_t            expiration_ticks; // tick count at which timer is set to expire
    uint32_t            period_ticks;
    uint32_t            index; // index into "handles" array
    struct net_timer_s  *next;
    void                *user_data;
} net_timer_t;

/*
 * map indexed handle to net_timer_t pointer
 * 
 * NOTE:
 * 
 * Indexed handles are required because we expect the timer API routines to be called with
 * invalid handles as a matter of course (i.e. not a programming error) and we must have
 * guaranteed detection of the invalid handle. For example, the user could cancel a timer
 * after timer expiration when handle is invalidated but before user expiration callback
 * is called.
 * 
 * Using a pointer to net timer as a handle is too fragile because the uint32_t timer_id 
 * cannot guarantee the validity of the handle -- it can only provide best-effort sanity
 * checking, e.g. for debug builds. This is because a new timer context could be allocated
 * at the exact same address in memory as a previous timer context. Also a uint32_t does
 * not provide a statistical guarantee of a unique ID match.
 */
typedef struct
{
#ifdef __linux__
    void*               owner;
#else
    uint32_t            owner;
#endif
    net_timer_t         *net_timer;
} net_timer_map_t;

typedef struct
{
    bool                initialized;
    OS_MUTEX            mtx;
    net_timer_map_t     handles[NET_TIMER_MAX_HANDLES];             
    net_timer_t         *os_timers; // (os ticks only, not including low-power sleep ticks)
    net_timer_t         *rt_timers; // real-time timers (os ticks + low-power sleep ticks)
} net_context_t;

static net_context_t g_net_ctxt = 
{
    .initialized = false,
    .os_timers = NULL,
    .rt_timers = NULL
};


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_LOCK_CONTEXT() \
        { \
            ASSERT(g_net_ctxt.initialized); \
            UINT os_rcode = os_mutex_get(&g_net_ctxt.mtx, OS_WAIT_FOREVER); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#define NET_UNLOCK_CONTEXT() \
        { \
            ASSERT(g_net_ctxt.initialized); \
            UINT os_rcode = os_mutex_put(&g_net_ctxt.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#ifdef __linux__

//=========================================================================================================================
// LINUX net timer implementation
//=========================================================================================================================
void net_utils_init(void)
{
    DBG_VERBOSE("==>%s\n",__func__);
    UINT os_rcode;
    net_context_t *net_ctxt = &g_net_ctxt;

    /*
     *  initialize context
     */

    os_rcode = posix_mutex_init(&net_ctxt->mtx);
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating net_ctxt mutex (os_rcode=0x%02x)\n", os_rcode);
        ASSERT(0);
        goto error;
    }

    int i;
    for(i=0; i<NET_TIMER_MAX_HANDLES; i++)
    {
        net_ctxt->handles[i].owner = NET_TIMER_INVALID_HANDLE;
        net_ctxt->handles[i].net_timer = NULL;
    }

    net_ctxt->initialized = true;
    return;

error:
    DBG_ERR("network utils init failed!\n");
    return;
}

/*
 *  net_timer_start_ms()
 */

net_timer_handle_t net_timer_start_ms(
    net_timer_flags_t flags,
    uint32_t timer_ms,
    net_timer_expiration_func_t expiration_func,
    void *user_data)
{
    ASSERT(expiration_func);
    return (net_timer_handle_t) delay_msec_timer_non_blocking(timer_ms, expiration_func, user_data, (flags & NET_TIMER_FLAG_PERIODIC));
}

/*
 *  net_timer_start_ticks()
 * 
 *  If no timers are available the request will be ignored (or ASSERT in debug build).
 *  Increment the NUM_ONESHOT_TIMERS define if necessary.
 */
net_timer_handle_t net_timer_start_ticks(
    net_timer_flags_t flags,
    uint32_t timer_ticks,
    net_timer_expiration_func_t expiration_func,
    void *user_data)
{
    ASSERT(expiration_func);
    return (net_timer_handle_t) delay_msec_timer_non_blocking(timer_ticks*MILLISEC_PER_TICK, expiration_func, user_data, (flags & NET_TIMER_FLAG_PERIODIC));
}

// this call also invalidates timer_handle 
void net_timer_cancel(net_timer_handle_t timer_handle)
{
    DBG_VERBOSE("%s() timer x%x (@ %d)\n", __func__, (unsigned int)timer_handle, posix_gettime_ticks());
    delay_msec_timer_cancel((delay_msec_timer_t *)timer_handle);
}

// use a 'delay' timer for the ms-resolution timer
typedef void (*delay_timer_cb)(void* timer_complete_cb_data);
int start_ms_timer_delay(timer_t* timer, uint32_t ms_delay, general_timer_cb timer_cb)
{
    return (int) delay_msec_timer_non_blocking((uint32_t)ms_delay, (delay_timer_cb)timer_cb, (void*)timer, false); // oneshot
}

// initiate a posix (timer_t) ms-resolution timer with a given callback
int start_ms_timer(timer_t* timer, long ms_delay, general_timer_cb timer_cb)
{
    int px_status = OK;
    // start a timer to perform a repetitive auto-capture & print of the profile
    struct itimerspec time_remaining;

    DBG_VERBOSE("%s(%d ms) timer x%x (@ %d)\n", __func__, ms_delay, (unsigned int)timer, posix_gettime_ticks());

    if (!timer) return 0;

    if (*timer) // time remaining?
    {
        timer_gettime(*timer, &time_remaining);
#ifdef DBG_VERBOSE_ON
        if ((time_remaining.it_value.tv_sec + time_remaining.it_value.tv_nsec) > 0)
        {
            DBG_VERBOSE("%s() timer %d remaining (%d:%d)\n", __func__, *timer, time_remaining.it_value.tv_sec, time_remaining.it_value.tv_nsec);
        }
#endif
    }

    if (!*timer) // initialize timer
    {
        struct sigevent evp;
        memset( &evp, 0, sizeof(struct sigevent) );
        evp.sigev_notify = SIGEV_THREAD;
        evp.sigev_notify_function = timer_cb;
        evp.sigev_signo = SIGSTOP;
        //unused evp.sigev_value.sigval_ptr = (void*)timer;
        px_status = timer_create( CLOCK_REALTIME, &evp, timer );
        if (px_status != 0)
        {
            DBG_ERR("%s(%d) timer '%s' (err %d)\n", __func__, ms_delay, strerror(thread_errno), thread_errno);
            XASSERT( px_status==0, px_status );
            return px_status;
        }
    }

    // if timer is currently running reset extent to ms_delay
    /* 
     * Create one-shot timer.
     */
    struct itimerspec onetime;
    memset( &onetime, 0, sizeof(struct itimerspec) );
    memset( &time_remaining, 0, sizeof(struct itimerspec) );
    int sec_delay = ms_delay/MSEC_PER_SEC;
    onetime.it_value.tv_sec = sec_delay;
    //periodic .. onetime.it_interval.tv_sec = ms_delay/1000;
    long long ns_delay = ms_delay*1000000L - (sec_delay ? sec_delay*1000000000L : 0);
    onetime.it_value.tv_nsec = ns_delay;
    //periodic .. onetime.it_interval.tv_nsec = (long) ns_delay; // cast

    // initiate timer
    px_status = timer_settime(*timer, TFD_CLOEXEC, &onetime, NULL);
    if (px_status != 0)
    {
        DBG_ERR("%s() timer x%x error '%s' (%d)\n", __func__, (unsigned int)timer, strerror(thread_errno), thread_errno);
        XASSERT( px_status==0, px_status );
        return thread_errno;
    }

    DBG_VERBOSE("%s() x%x activated %d ms (@ %d)\n", __func__, (unsigned int)timer, ms_delay, posix_gettime_ticks());

    return px_status;
}

// upgrade a single-shot ms-resolution timer -> periodic
int start_ms_timer_periodic(timer_t* timer, long ms_delay, general_timer_cb timer_cb)
{
    error_type_t res = start_ms_timer(timer, ms_delay, timer_cb);
    if (res == OK)
    {
        /* 
         * Add periodic interval.
         */
        struct itimerspec periodic;
        memset( &periodic, 0, sizeof(struct itimerspec) );
        periodic.it_interval.tv_sec = ms_delay/1000;
        periodic.it_interval.tv_nsec = ms_delay*1000; // not quite right
        periodic.it_value.tv_sec = periodic.it_interval.tv_sec;
        periodic.it_value.tv_nsec = periodic.it_interval.tv_nsec;

        // initiate timer
        res = timer_settime(*timer, TFD_CLOEXEC, &periodic, NULL);
        if (res != 0)
        {
            DBG_ERR("%s(%d) timer x%x error '%s' (%d)\n", __func__, ms_delay, (unsigned int)timer, strerror(thread_errno), thread_errno);
            XASSERT( res==0, res );
            return thread_errno;
        }
    }
    return res;
}

#else


static net_timer_handle_t net_timer_bind_handle(net_timer_t *net_timer);
static net_timer_t *net_timer_get_context(net_timer_handle_t timer_handle);
static void net_timer_insert(net_timer_t *net_timer, net_timer_t **net_timer_list);
static bool net_timer_remove(net_timer_t *net_timer, net_timer_t **net_timer_list);

//=========================================================================================================================
// Module entry point
//=========================================================================================================================

void net_utils_init(void)
{
    DBG_VERBOSE("==>%s\n",__func__);
    UINT threadx_rcode;
    net_context_t *net_ctxt = &g_net_ctxt;

    /*
     *  initialize context
     */

    threadx_rcode = tx_mutex_create(&net_ctxt->mtx, "netUtilsCtxt", TX_INHERIT);
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_ERR("error creating net_ctxt mutex (threadx_rcode=0x%02x)\n", threadx_rcode);
        ASSERT(0);
        goto error;
    }

    int i;
    for(i=0; i<NET_TIMER_MAX_HANDLES; i++)
    {
        net_ctxt->handles[i].owner = NET_TIMER_INVALID_HANDLE;
        net_ctxt->handles[i].net_timer = NULL;
    }

    net_ctxt->initialized = true;
    return;

error:
    DBG_ERR("network utils init failed!\n");
    return;
}

net_rcode_t net_get_var_location(char *location, uint8_t len)
{
    if(!len)
    { 
        return NET_OK;
    }
    ASSERT( NULL != location );
    location[0] = '\0';

    // TODO pull from net_var storage

    return NET_OK;
}


//=========================================================================================================================
// Global routines
//=========================================================================================================================

/*
 *  net_timer_start_ms()
 */

net_timer_handle_t net_timer_start_ms(
    net_timer_flags_t flags,
    uint32_t timer_ms,
    net_timer_expiration_func_t expiration_func,
    void *user_data)
{
    return net_timer_start_ticks(flags, (timer_ms*TIMER_INTS_PER_SECOND)/1000, expiration_func, user_data);
}

/*
 *  net_timer_start_ticks()
 * 
 *  If no timers are available the request will be ignored (or ASSERT in debug build).
 *  Increment the NUM_ONESHOT_TIMERS define if necessary.
 */
net_timer_handle_t net_timer_start_ticks(
    net_timer_flags_t flags,
    uint32_t timer_ticks,
    net_timer_expiration_func_t expiration_func,
    void *user_data)
{
    ASSERT(expiration_func);

    net_context_t *net_ctxt = &g_net_ctxt;
    net_timer_t *net_timer = NULL;
    net_timer_handle_t timer_handle = NET_TIMER_INVALID_HANDLE;
    bool locked = false;

    if(!net_ctxt->initialized)
    {
        // not initialized or init failed
        goto done;
    }

    NET_LOCK_CONTEXT();
    locked = true;

    // allocate a net timer
    net_timer = (net_timer_t *)MEM_MALLOC(sizeof(net_timer_t));
    if(!net_timer)
    {
        goto done;
    }
    net_timer->timer_id = NET_TIMER_ID;
    net_timer->flags = flags;
    net_timer->expiration_func = expiration_func;
    net_timer->index = NET_TIMER_INVALID_HANDLE;
    net_timer->next = NULL;
    net_timer->user_data = user_data;

    net_timer->period_ticks = 0;
    if(flags & NET_TIMER_FLAG_PERIODIC)
    {
        net_timer->period_ticks = timer_ticks;
    }

    // bind timer to handle
    timer_handle = net_timer_bind_handle(net_timer);
    if(timer_handle == NET_TIMER_INVALID_HANDLE)
    {
        // no timer slots available
        goto done;
    }

    if(flags & NET_TIMER_FLAG_REAL_TIME)
    {
        /* Interpret timer_ticks param as real-time ticks.
         * 
         * Hook into Treck timer thread for real-time notification, which also accounts
         * for time spent in sleep state.
         */

#ifdef HAVE_ASIC_POWER_DRIVER
        uint32_t cur_ticks = asic_pwr_time_get();
#else // !HAVE_ASIC_POWER_DRIVER
        uint32_t cur_ticks = tx_time_get();
#endif // HAVE_ASIC_POWER_DRIVER

        net_timer->expiration_ticks = cur_ticks + timer_ticks;
        XASSERT( (net_timer->expiration_ticks > cur_ticks) &&
                 (net_timer->expiration_ticks > timer_ticks),
                 net_timer->expiration_ticks ); // check for overflow condition
        net_timer_insert(net_timer, &net_ctxt->rt_timers);
    }
    else 
    {
        /* Interpret timer_ticks param as ThreadX OS ticks.
         */

        uint32_t cur_ticks = tx_time_get();
        net_timer->expiration_ticks = cur_ticks + timer_ticks;
        XASSERT( (net_timer->expiration_ticks > cur_ticks) &&
                 (net_timer->expiration_ticks > timer_ticks),
                 net_timer->expiration_ticks ); // check for overflow condition
        net_timer_insert(net_timer, &net_ctxt->os_timers);
    }

    // claim ownership of the handle
    net_ctxt->handles[timer_handle % NET_TIMER_MAX_HANDLES].net_timer = net_timer;

done:
    if(timer_handle == NET_TIMER_INVALID_HANDLE)
    {
        // cleanup
        if(net_timer)
        {
            net_timer->timer_id = 0; // must wipe ID prior to free to catch future invalid references 
            MEM_FREE_AND_NULL(net_timer);
        }
    }
    if(locked)
    {
        NET_UNLOCK_CONTEXT();
    }
    return timer_handle;
}

// this call also invalidates timer_handle 
void net_timer_cancel(net_timer_handle_t timer_handle)
{
    net_context_t *net_ctxt = &g_net_ctxt;

    if(!net_ctxt->initialized)
    {
        // not initialized or init failed
        goto done;
    }

    NET_LOCK_CONTEXT();

    net_timer_t *net_timer = net_timer_get_context(timer_handle);
    if(!net_timer)
    {
        // invalid handle - ignore the request
        NET_UNLOCK_CONTEXT();
        goto done;
    }

    bool found = false;
    if(net_timer->flags & NET_TIMER_FLAG_REAL_TIME)
    {
        // unschedule the rt_timer
        found = net_timer_remove(net_timer, &net_ctxt->rt_timers);
    }
    else
    {
        // unschedule the os_timer
        found = net_timer_remove(net_timer, &net_ctxt->os_timers);
    }
    XASSERT(found, net_timer->flags);

    // detach the handle
    XASSERT(net_timer->index == timer_handle % NET_TIMER_MAX_HANDLES, net_timer->index);
    net_ctxt->handles[net_timer->index].net_timer = NULL;

    // free context
    ASSERT(net_timer->timer_id == NET_TIMER_ID); // sanity check valid ctxt (note this is not a guarantee)
    net_timer->timer_id = 0; // must wipe ID prior to free to catch future invalid references 
    MEM_FREE_AND_NULL(net_timer);

    NET_UNLOCK_CONTEXT();

done:
    return;
}


//=========================================================================================================================
// Private helper routines
//=========================================================================================================================

static net_timer_handle_t net_timer_bind_handle(net_timer_t *net_timer)
{
    net_context_t *net_ctxt = &g_net_ctxt;
    ASSERT(net_ctxt->initialized);

    net_timer_handle_t timer_handle = NET_TIMER_INVALID_HANDLE;

    NET_LOCK_CONTEXT();

    // locate a free handle
    int i;
    for(i=0; i<NET_TIMER_MAX_HANDLES; i++)
    {
        if(!net_ctxt->handles[i].net_timer)
        {
            break;
        }
    }
    if(i < NET_TIMER_MAX_HANDLES)
    {
        // continuously increment owner to detect invalid handle in successive net_timer operations
        timer_handle = (net_timer_handle_t)(net_ctxt->handles[i].owner + NET_TIMER_MAX_HANDLES);
        if( timer_handle < net_ctxt->handles[i].owner || timer_handle == NET_TIMER_INVALID_HANDLE)
        {
            // we have overflowed the handle - reinit
            timer_handle = (net_timer_handle_t)i;
        }
        net_ctxt->handles[i].owner = timer_handle;

        // bind handle to timer and timer to handle
        net_ctxt->handles[i].net_timer = net_timer;
        net_timer->index = i;
    }

    NET_UNLOCK_CONTEXT();

    return timer_handle;
}

static net_timer_t *net_timer_get_context(net_timer_handle_t timer_handle)
{
    net_context_t *net_ctxt = &g_net_ctxt;
    ASSERT(net_ctxt->initialized);

    net_timer_t *net_timer = NULL;

    if(timer_handle == NET_TIMER_INVALID_HANDLE)
    {
        return NULL;
    }

    NET_LOCK_CONTEXT();

    // recover handle index from user handle
    uint32_t i = ((uint32_t)timer_handle)%NET_TIMER_MAX_HANDLES;

    if(net_ctxt->handles[i].owner == timer_handle && net_ctxt->handles[i].net_timer)
    {
        // handle exists (matches "owner"), and timer has not expired or been canceled (non-NULL timer)
        net_timer = net_ctxt->handles[i].net_timer;
        ASSERT(net_timer->index == i);
    }
    // else handle is invalid

    NET_UNLOCK_CONTEXT();

    return net_timer;
}

static void net_timer_insert(net_timer_t *net_timer, net_timer_t **net_timer_list)
{
    ASSERT(net_timer);
    ASSERT(net_timer->timer_id == NET_TIMER_ID); // sanity check valid handle (note this is not a guarantee)
    ASSERT(net_timer_list);
    ASSERT(net_timer->next == NULL); // verify we are not already in a list

    net_timer_t **prev;
    net_timer_t *cur;

    NET_LOCK_CONTEXT();

    prev = net_timer_list;
    cur = *net_timer_list;

    while(cur)
    {
        // insertion sort in increasing order of expiration_ticks
        // i.e. list head will point to next timer to expire
        if(net_timer->expiration_ticks < cur->expiration_ticks)
        {
            break;
        }
        prev = &cur->next;
        cur = cur->next;
    }
    *prev = net_timer;
    net_timer->next = cur;

    NET_UNLOCK_CONTEXT();

    return;
}

// returns 'true' timer was found and removed from list, 'false' if timer not found
static bool net_timer_remove(net_timer_t *net_timer, net_timer_t **net_timer_list)
{
    ASSERT(net_timer);
    ASSERT(net_timer->timer_id == NET_TIMER_ID); // sanity check valid handle (note this is not a guarantee)
    ASSERT(net_timer_list);

    net_timer_t **prev;
    net_timer_t *cur;

    NET_LOCK_CONTEXT();

    prev = net_timer_list;
    cur = *net_timer_list;

    while(cur)
    {
        if(net_timer == cur)
        {
            *prev = net_timer->next;
            net_timer->next = NULL;
            break;
        }
        prev = &cur->next;
        cur = cur->next;
    }

    NET_UNLOCK_CONTEXT();
    
    return (cur != NULL);
}

// must be called periodically by Treck timer thread
// TODO register static callback func with Treck timer thread vs. direct call of global func 
void net_timer_update(void)
{
    net_context_t *net_ctxt = &g_net_ctxt;

    if(!net_ctxt->initialized)
    {
        // not initialized or init failed
        goto done;
    }

    net_timer_t *cur_timer;
    net_timer_t *expired_timers = NULL;

    uint32_t cur_os_ticks = tx_time_get();
#ifdef HAVE_ASIC_POWER_DRIVER
    uint32_t cur_rt_ticks = asic_pwr_time_get();
#else // !HAVE_ASIC_POWER_DRIVER
    uint32_t cur_rt_ticks = cur_os_ticks;
#endif // HAVE_ASIC_POWER_DRIVER

    NET_LOCK_CONTEXT();
    cur_timer = net_ctxt->rt_timers;
    while(cur_timer)
    {
        if(cur_timer->expiration_ticks <= cur_rt_ticks)
        {
            // remove from rt_timers list BEFORE calling the user callback
            net_ctxt->rt_timers = cur_timer->next;
            cur_timer->next = expired_timers;
            expired_timers = cur_timer;

            // if this is a oneshot timer then we must detach/free handle while context is locked
            if ((cur_timer->flags & NET_TIMER_FLAG_PERIODIC) == 0)
            {
                XASSERT(cur_timer->index < NET_TIMER_MAX_HANDLES, cur_timer->index);
                net_ctxt->handles[cur_timer->index].net_timer = NULL;
                cur_timer->index = NET_TIMER_INVALID_HANDLE;
            }

            cur_timer = net_ctxt->rt_timers;
        }
        else
        {
            // no more expired timers (assumes sorted list)
            break;
        }

    }
    cur_timer = net_ctxt->os_timers;
    while(cur_timer)
    {
        if(cur_timer->expiration_ticks <= cur_os_ticks)
        {
            // remove from os_timers list BEFORE calling the user callback
            net_ctxt->os_timers = cur_timer->next;
            cur_timer->next = expired_timers;
            expired_timers = cur_timer;

            // if this is a oneshot timer then we must detach/free handle while context is locked
            if ((cur_timer->flags & NET_TIMER_FLAG_PERIODIC) == 0)
            {
                XASSERT(cur_timer->index < NET_TIMER_MAX_HANDLES, cur_timer->index);
                net_ctxt->handles[cur_timer->index].net_timer = NULL;
                cur_timer->index = NET_TIMER_INVALID_HANDLE;
            }

            cur_timer = net_ctxt->os_timers;
        }
        else
        {
            // no more expired timers (assumes sorted list)
            break;
        }
    }
    NET_UNLOCK_CONTEXT();


    while(expired_timers)
    {
        cur_timer = expired_timers;
        expired_timers = expired_timers->next;
        cur_timer->next = NULL;

        // call user's registered callback
        // note handle is invalid at this point, so user must not reference from callback 
        ASSERT(cur_timer->expiration_func);
        cur_timer->expiration_func(cur_timer->user_data);

        // reset/free timer AFTER user callback returns
        if(cur_timer->flags & NET_TIMER_FLAG_PERIODIC)
        {
            // periodic timer -- reset it
            cur_timer->expiration_ticks = cur_timer->expiration_ticks + cur_timer->period_ticks;
            if(cur_timer->flags & NET_TIMER_FLAG_REAL_TIME)
            {
                net_timer_insert(cur_timer, &net_ctxt->rt_timers);
            }
            else
            {
                net_timer_insert(cur_timer, &net_ctxt->os_timers);
            }
        }
        else
        {
            // oneshot timer -- free it
            cur_timer->timer_id = 0; // must wipe ID prior to free to catch future invalid references 
            MEM_FREE_AND_NULL(cur_timer);
        }
    }

done:
    return;
}
#endif // __linux__
