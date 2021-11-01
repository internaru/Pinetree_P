/**
 * \file scos.c
 *
 * \brief Scanner platform specific operating system routines.
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "scos.h"

#include "memAPI.h"
#include "lassert.h"
/* need to jump through some hoops for IN_INTERRUPT */
#if defined(__linux__) && !defined(__KERNEL__)
    /* userspace linux */
    #define IN_INTERRUPT() 0
#else
    #include "cpu_api.h" 
    #include "debug.h" 
#endif

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "ostools.h"

/**
 * \brief  Return the errno
 *
 *  Currently supports either the tx_posix layer or the POSIX global errno
 *
 * \author David Poole
 * \date 06-Oct-2008
 *
 */

int scos_get_errno( void )
{
#if defined HAVE_TX_POSIX_2008
    /* ThreadX tx_posix.c */
    return tx_posix_get_errno();
#elif defined HAVE_TX_POSIX_2011
    /* ThreadX tx_posix.c */
    return posix_get_pthread_errno(pthread_self());
#elif defined __linux__ || defined __APPLE__
    return errno;
#else
    /* TODO return an errno from your platform */
    #error return errno for your platform
    return -4242;
#endif
}

/* davep 03-Nov-2008
 *
 * The following functions are for interfacing with an independent RTOS that
 * uses POSIX primatives of unknown size.
 *
 * I cannot create a pthread_cond_t or pthread_mutex_t on the stack or in global
 * space because I cannot know when I compile how large the final size of the
 * structure might be.
 *
 * Instead, the scan uses pointers to all the pthread primatives. 
 *
 */

pthread_cond_t * scos_alloc_pthread_cond( void )
{
    return MEM_MALLOC(sizeof(pthread_cond_t));
}

void scos_free_pthread_cond( pthread_cond_t ** cond )
{
    PTR_FREE( *cond );
}

pthread_mutex_t * scos_alloc_pthread_mutex( void )
{
    return MEM_MALLOC( sizeof(pthread_mutex_t) );
}

void scos_free_pthread_mutex( pthread_mutex_t ** mutex )
{
    PTR_FREE( *mutex );
}

pthread_mutexattr_t * scos_alloc_pthread_mutexattr( void )
{
    return MEM_MALLOC( sizeof(pthread_mutexattr_t) );
}

void scos_free_pthread_mutexattr( pthread_mutexattr_t ** mutexattr )
{
    PTR_FREE( *mutexattr );
}

pthread_attr_t * scos_alloc_pthread_attr( void )
{
    return MEM_MALLOC( sizeof(pthread_attr_t));
}

void scos_free_pthread_attr( pthread_attr_t **attr )
{
    PTR_FREE( *attr );
}

void *scos_alloc_pthread_stack( size_t size_in, size_t *size_out )
{
    void *ptr;

    *size_out = size_in;

    /* davep 03-Dec-2010 ; for my code running under both firmware and chimera,
     * need to be able to change the stack size to match the local system
     * requirements
     */
#ifdef PTHREAD_STACK_MIN
    if( *size_out < PTHREAD_STACK_MIN ) {
        *size_out = PTHREAD_STACK_MIN;
    }
#endif

    /* davep 03-Dec-2010 ; as of this writing, our stacks needed to be 32-byte
     * aligned. Will probably change in future. Everything else does.
     */
    ptr = MEM_MALLOC_ALIGN( *size_out, e_32_byte );

//    dbg_printf( "%s ptr=%#x\n", __FUNCTION__, ptr );

    return ptr;
}

void scos_free_pthread_stack( void **stack_ptr )
{
    PTR_FREE( *stack_ptr );
}

#if defined HAVE_TX_POSIX_2011 || defined LINUX || defined linux

int posix_sleep_seconds( unsigned long seconds )
{
    int retcode;
    struct timespec req, rem;
    
    req.tv_sec = seconds;
    req.tv_nsec = 0;
    
    memset( &rem, 0, sizeof(struct timespec) );

    retcode = nanosleep( &req, &rem );
    XASSERT( retcode==0, scos_get_errno() );

    return retcode;
}

int scos_sleep_milliseconds( unsigned long msec_count )
{
    int retcode;
    struct timespec req, rem;
    
    /* note I'm not bothering to catch tv_nsec overflowing into seconds */
    req.tv_sec = 0;
    req.tv_nsec = NANOSEC_PER_MILLISEC * msec_count;
    
    memset( &rem, 0, sizeof(struct timespec) );

    retcode = nanosleep( &req, &rem );
    XASSERT( retcode==0, scos_get_errno() );

    return retcode;
}

int scos_sleep_microseconds( unsigned long useconds )
{
    int retcode;
    struct timespec req, rem;
    
    /* note I'm not bothering to catch usec_count overflowing into seconds */
    req.tv_sec = 0;
    req.tv_nsec = NANOSEC_PER_USEC * useconds;
    
    memset( &rem, 0, sizeof(struct timespec) );

    retcode = nanosleep( &req, &rem );
    XASSERT( retcode==0, scos_get_errno() );

    return retcode;
}
#endif

scan_err_t scos_mq_receive( struct scos_mq *msgq, scan_msg_t *msg, unsigned long int timeout_in_usec )
{
    scan_err_t final_scerr;
    int mqretcode;
    struct timespec abs_timeout;
    int wait_errno;

    /* sleep 1/2 second waiting for messages */
    clock_gettime( CLOCK_REALTIME, &abs_timeout );
    calc_future_usec( &abs_timeout, timeout_in_usec );

    mqretcode = (int)mq_timedreceive( msgq->mq, (char *)msg, sizeof(scan_msg_t), NULL, &abs_timeout );

    final_scerr = SCANERR_NONE;
    wait_errno = 0;
    if( mqretcode == -1 ) {
        wait_errno = scos_get_errno();
        /* only a timeout error is acceptable at this point */
        XASSERT( wait_errno==ETIMEDOUT, wait_errno );
        final_scerr = SCANERR_TIMEDOUT;
    }

    return final_scerr;
}

scan_err_t scos_mq_send( struct scos_mq *msgq, scan_msg_t *msg, uint32_t wait_usecs )
{
    int mqretcode;
    struct timespec abs_timeout;

    if( wait_usecs==SCOS_WAIT_FOREVER ) {
        mqretcode = (int)mq_send( msgq->mq, (const char *)msg, sizeof(scan_msg_t),
                                    MQ_DEFAULT_PRIORITY );
    }
    else {
        clock_gettime( CLOCK_REALTIME, &abs_timeout );
        calc_future_usec( &abs_timeout, wait_usecs );

        mqretcode = (int)mq_timedsend( msgq->mq, (const char *)msg, sizeof(scan_msg_t),
                                    MQ_DEFAULT_PRIORITY, &abs_timeout );
    }

    /* There are a lot of reasons why mq_send() and mq_timedsend() can fail.
     * Don't attempt to try to map those errors onto our scan_err_t but rather
     * return a general failure.
     */
    if( mqretcode != 0 ) {
        return SCANERR_MSG_FAIL;
    }

    return SCANERR_NONE;
}

scan_err_t scos_mq_send_nowait( struct scos_mq *msgq, scan_msg_t *msg )
{
    struct timespec abs_timeout;
    int mqretcode;
    /* !!!!!!
     * WARNING!  This function can be called from interrupt context! 
     * !!!!!!
     */

    /* No timeout. By using a timespec set to zero, I'm tricking mq_timesend()
     * to force it to never wait.
     */
    abs_timeout.tv_sec = 0;
    abs_timeout.tv_nsec = 0;

    mqretcode = (int)mq_timedsend( msgq->mq, (const char *)msg, sizeof(scan_msg_t),
                        MQ_DEFAULT_PRIORITY, &abs_timeout );

    /* There are a lot of reasons why mq_timedsend() can fail.  Don't attempt to
     * try to map those errors onto our scan_err_t but rather return a general
     * failure.
     */
    if( mqretcode != 0 ) {
        return SCANERR_MSG_FAIL;
    }

    return SCANERR_NONE;
}

scan_err_t scos_mq_init( struct scos_mq *msgq, const char *name, int queue_size )
{
    struct mq_attr mq_attrib;
    int mq_errno;
    int num_messages;

    num_messages = queue_size;

    dbg2( "%s name=%s nummsgs=%d msgsize=%d\n", __FUNCTION__, name,
            num_messages, sizeof(scan_msg_t) );
    
    memset( msgq, 0, sizeof(struct scos_mq) );
    strncpy( msgq->name, name, MAX_MQ_NAME );

    memset( &mq_attrib, 0, sizeof(mq_attrib) );
    mq_attrib.mq_msgsize = sizeof(scan_msg_t);
    mq_attrib.mq_maxmsg = num_messages;

    msgq->mq = mq_open( msgq->name, O_RDWR|O_CREAT|O_EXCL,
                                    0700, &mq_attrib );
    mq_errno = scos_get_errno();

    if( mq_errno==EEXIST ) {
        dbg2( "%s queue=%s already exists so try to open\n", 
                    __FUNCTION__, msgq->name );

        msgq->mq = mq_open( msgq->name, O_RDWR );
        XASSERT( msgq->mq > 0, mq_errno );

        dbg2( "%s queue=%s opened successfully\n", 
                    __FUNCTION__, msgq->name );
    }
    else {
        dbg1( "%s mq_open failed errno=%d\n", __FUNCTION__, mq_errno );
        XASSERT( msgq->mq != (mqd_t)-1, mq_errno );
    }

    return SCANERR_NONE;
}

void scos_mq_free( struct scos_mq *msgq )
{
    int mqretcode;

    mqretcode = (int)mq_close( msgq->mq );
    XASSERT( mqretcode==0, (uint32_t)mqretcode );

    mqretcode = (int)mq_unlink( msgq->name );
    XASSERT( mqretcode==0, (uint32_t)mqretcode );
}

uint32_t scos_mq_num_pending( struct scos_mq *msgq )
{
    int mq_retcode;
    struct mq_attr attr;

    mq_retcode = (int)mq_getattr( msgq->mq, &attr );
    XASSERT( mq_retcode==0, scos_get_errno() );

    return attr.mq_curmsgs;
}

scan_err_t scos_sem_init( struct scos_sem *sem, const char *name, 
                          int initial_value )
{
    scan_err_t scerr;
    int err;
    int sem_value;

    memset( sem, 0, sizeof(struct scos_sem) );

    strncpy( sem->name, name, MAX_SEM_NAME );

    sem->posix_sem = sem_open( sem->name, O_CREAT|O_EXCL, 0700, initial_value );
    if( sem->posix_sem == SEM_FAILED ) {
        err = scos_get_errno();
        if( err==EEXIST ) {
            dbg1( "semaphore \"%s\" exists; attempt to open and clear\n", sem->name );
            sem->posix_sem = sem_open( sem->name, O_EXCL, 0700, 1 );
            sem->cookie = SCOS_SEM_COOKIE;

            scerr = scos_sem_getvalue( sem, &sem_value );
            XASSERT( scerr==SCANERR_NONE, scerr );

            dbg2( "%s sem \"%s\" value=%d\n", __FUNCTION__, sem->name, sem_value );

//            /* davep 13-Aug-2012 ; leftover sem_wait() has semaphore locked
//             * (probably due to a firmware crash)
//             */
//            if( scantask_is_busy() ) {
//                err = sem_post( scantask_global_lock );
//                XASSERT( err==0, scos_get_errno() );
//            }
        }
    }

    XASSERT( sem->posix_sem != SEM_FAILED, scos_get_errno() );

    sem->cookie = SCOS_SEM_COOKIE;

    return SCANERR_NONE;
}

scan_err_t scos_sem_wait( struct scos_sem *sem )
{
    int retcode;

    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    retcode = sem_wait( sem->posix_sem );
    XASSERT( retcode==0, scos_get_errno() );

    return SCANERR_NONE;
}

scan_err_t scos_sem_trywait( struct scos_sem *sem )
{
    int retcode;

    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    retcode = sem_trywait( sem->posix_sem );
    if( retcode != 0 ) {
        return SCANERR_BUSY; 
    }
    return SCANERR_NONE;
}

scan_err_t scos_sem_post( struct scos_sem *sem )
{
    int retcode; 

    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    retcode = sem_post( sem->posix_sem );
    XASSERT( retcode==0, scos_get_errno() );

    return SCANERR_NONE;
}

scan_err_t scos_sem_getvalue( struct scos_sem *sem, int *value )
{
    int retcode;
#ifdef HAVE_TX_POSIX_2011
    unsigned long sem_value;
#else
    int sem_value;
#endif

    /* davep 29-Aug-2012 ; we need two different types for 'sval'. The threadx
     * posix layer uses unsigned long. Linux/POSIX uses int.
     *
     * http://pubs.opengroup.org/onlinepubs/7908799/xsh/sem_getvalue.html
     */

    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    retcode = sem_getvalue( sem->posix_sem, &sem_value );
    if( retcode != 0 ) {
        /* invalid weirdness */
        return SCANERR_NOT_PERMITTED;
    }

    *value = (int)sem_value;
    return SCANERR_NONE;
}

scan_err_t scos_sem_timedwait( struct scos_sem *sem, struct timespec *abs_timeout )
{
    int retcode;
    int err;

    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    retcode = sem_timedwait( sem->posix_sem, abs_timeout );
    if( retcode != 0 ) {
        err = scos_get_errno();
        if( err==ETIMEDOUT ) {
            return SCANERR_TIMEDOUT;
        }
    }
    XASSERT( retcode==0, scos_get_errno() );

    return SCANERR_NONE;
}

scan_err_t scos_sem_is_initialized( struct scos_sem *sem )
{
    scan_err_t scerr;
    int sem_value;

    /* is sem->posix_sem a valid semaphore? (sanity test) */

    if( sem->posix_sem == NULL ) {
        /* not initialized */
        return SCANERR_NOT_PERMITTED;
    }

    if( sem->cookie != SCOS_SEM_COOKIE ) {
        /* not initialized */
        return SCANERR_NOT_PERMITTED;
    }
    
    /* sanity test on the validity of the semaphore by getting its value */
    scerr = scos_sem_getvalue( sem, &sem_value );

    return scerr;
}

/**
 * \brief  scos wrapper to tell if we're in interrupt context or not
 *
 * \author David Poole
 * \date 14-Jan-2013
 */

int scos_in_interrupt( void )
{
    return IN_INTERRUPT();
}

