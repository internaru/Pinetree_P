/**
 * \file txtools.c
 *
 * \brief Useful ThreadX OS utility functions.
 *
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <limits.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scantask.h"
#include "ostools.h"

/**
 * \brief  Create a future time
 *
 * Add a number of microseconds to a struct timespec making sure to handle
 * nanosecond rollover.
 *
 * Originally written to calculate future times for mq_timedreceive() calls
 *
 * \author David Poole
 * \date 30-Jun-2008
 *
 */

void calc_future_usec( struct timespec *ts, 
                       unsigned long int timeout_usec )
{
    unsigned long int timeout_nsec;
    unsigned long int num_seconds;

    /* pull out as many seconds from the microseconds as we can (will make the
     * nanosecond calculation easier)
     */
    num_seconds = timeout_usec / USEC_PER_SECOND;
    timeout_usec -= num_seconds * USEC_PER_SECOND;
    ts->tv_sec += num_seconds; 

//    dbg2( "%s ns=%ld to=%ld tvsec=%ld\n", __FUNCTION__, num_seconds,
//                timeout_usec, ts->tv_sec );
    
    /* check for potential overflow */
    XASSERT( timeout_usec < ULONG_MAX/1000, (uint32_t)timeout_usec );

    /* remaining microseconds to nanoseconds */
    timeout_nsec = timeout_usec * 1000;

    /* handle rollover if tv_nsec + timeout_nsec > ULONG_MAX */
    while( timeout_nsec > NANOSEC_PER_SECOND ) {
        ts->tv_sec += 1;
        timeout_nsec -= NANOSEC_PER_SECOND;
    }

    ts->tv_nsec += timeout_nsec;

    /* max value of tv_nsec is 1e9 so bring it down to that range if necessary */
    while( ts->tv_nsec >= NANOSEC_PER_SECOND ) {
        ts->tv_nsec -= NANOSEC_PER_SECOND;
        ts->tv_sec += 1;
    }
}

/**
 * \brief  Create a thread with a stack.
 *
 * Wrapper around the pthread calls to create a thread assigned to a specific
 * stack.
 *
 *
 * \author David Poole
 * \date 30-Jun-2008
 *
 */
#ifdef __KERNEL__
scan_err_t create_stacked_thread( pthread_t *thd_id, 
                            void *(*start_routine)(void *),
                            void *thread_arg,
                            void *stack_ptr,
                            size_t stack_size )
{
    return SCANERR_NOT_IMPLEMENTED;
}
#endif

// if we are building the printer/mfp application for linux map to posix shim layer thread create
// so that threads are consistently created with the correct priority and realtime attributes
#if defined(__linux__) && defined(HAVE_OS_SHIM)
#include "posix_ostools.h"
scan_err_t create_stacked_thread( pthread_t *thd_id, 
                            void *(*start_routine)(void *),
                            void *thread_arg,
                            void *stack_ptr,
                            size_t stack_size )
{
    posix_create_thread(thd_id, start_routine, thread_arg, "scan_thd", stack_ptr, stack_size, POSIX_THR_PRI_NORMAL);
    return SCANERR_NONE;
}
#endif

#if defined HAVE_TX_POSIX_2011 || defined PLAT_CHIMERA
scan_err_t create_stacked_thread( pthread_t *thd_id, 
                            void *(*start_routine)(void *),
                            void *thread_arg,
                            void *stack_ptr,
                            size_t stack_size )
{
    int retcode;
    pthread_attr_t *thread_attr_ptr;

    dbg2( "%s start=%p stack=%p stack_size=%d\n", __FUNCTION__, start_routine,
                stack_ptr, stack_size );

    thread_attr_ptr = scos_alloc_pthread_attr();
    if( thread_attr_ptr==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    retcode = pthread_attr_init( thread_attr_ptr );
    if( retcode == ENOMEM ) {
        scos_free_pthread_attr( &thread_attr_ptr );
        return SCANERR_OUT_OF_MEMORY;
    }
    XASSERT( retcode==0, scos_get_errno() );

    /* need to make sure our stack size obeys the real POSIX standard if we're
     * running on a real POSIX OS (e.g., Linux) versus running on an RTOS
     * emulation layer (e.g. ThreadX's tx_posix.c)
     *
     * PTHREAD_STACK_MIN is in limits.h
     */
#ifdef PTHREAD_STACK_MIN
    XASSERT( stack_size >= PTHREAD_STACK_MIN, PTHREAD_STACK_MIN );
#endif

    /* davep 01-Jul-2008 ; I'm getting warnings from GCC 4.1.2 on Linux 2.6.22
     * saying pthread_attr_setstacksize() and pthread_attr_setstackaddr() has
     * been deprecated in favor of pthread_attr_setstack().  
     *
     * TODO Need to add this to the ThreadX layer, too.
     */
#if 1
    retcode = pthread_attr_setstack( thread_attr_ptr, stack_ptr, stack_size );
    XASSERT( retcode==0, retcode );
#else
    retcode = pthread_attr_setstacksize( thread_attr_ptr, stack_size );
    XASSERT( retcode==0, retcode );

    retcode = pthread_attr_setstackaddr( thread_attr_ptr, stack_ptr );
    XASSERT( retcode==0, retcode );
#endif

    retcode = pthread_create( thd_id, thread_attr_ptr, start_routine, thread_arg );
    XASSERT( retcode==0, retcode );

    retcode = pthread_attr_destroy( thread_attr_ptr );
    XASSERT( retcode==0, retcode );

    scos_free_pthread_attr( &thread_attr_ptr );

    return SCANERR_NONE;
}
#endif

#if defined PLAT_CHIMERA
int posix_create_thread(pthread_t *thread_id, void *(*start_routine)(void*), 
                        void *thread_arg, const char* thread_name, void *stack_ptr, 
                        size_t stack_size, int priority)
{
    scan_err_t scerr;

    scerr = create_stacked_thread(thread_id, start_routine, thread_arg,
                                    stack_ptr, stack_size); 
    if( scerr != SCANERR_NONE ) {
        return -1;
    }
    return 0;
}
#endif

/**
 * \brief  subtract two timespec values
 *
 *  Not a POSIX function!
 *
 * returns <0 if a<b  
 *          0 if a==b
 *         >0 if a>b 
 *
 * If a<b, the values in the timespec will be subject to integer underflow.
 *
 * \author David Poole
 * \date 07-Jul-2008
 *
 * davep 17-Apr-2012 ;  renamed from timespec_sub() to timespec_subtract() to
 * fix collision with Linux kernel function of same name.
 *
 */

int timespec_subtract( const struct timespec *a, 
                  const struct timespec *b, 
                  struct timespec *result )
{
    long int sec;
    uint32_t nsec;

//    dbg_printf( "%s %ld %ld %ld %ld\n", __FUNCTION__, 
//            a->tv_sec, a->tv_nsec, b->tv_sec, b->tv_nsec );

    /* result = a - b; 
     *
     * Callers' responsibility to make sure a >= b if they want to get a
     * sensical value in the 'result' parameter. If a<b and the timespec fields
     * are unsigned, result's fields will underflow.
     *
     * return <0 if a < b
     * return >0 if a > b
     *
     * Note: can't count on tv_sec or tv_nsec to be signed in spite of what the
     * standard might say
     */

    sec = a->tv_sec - b->tv_sec;

    if( a->tv_nsec < b->tv_nsec ) {
        /* borrow a second's worth of nanoseconds */
        sec -= 1;
        nsec = (a->tv_nsec + NANOSEC_PER_SECOND) - b->tv_nsec;
    }
    else {
        nsec = a->tv_nsec - b->tv_nsec;
    }

    result->tv_sec = sec;
    result->tv_nsec = nsec;

    if( !nsec && !sec ) return 0;
    if( sec < 0 ) return -1;
    return 1;
}

