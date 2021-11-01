/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <limits.h>
#include <time.h>
#include <string.h>  // for memset
#include <stdio.h>
#ifdef __linux__
#include <errno.h>
#include <limits.h>
#endif
#include "pthread.h"
#include "lassert.h"
#include "logger.h"
#include "posix_ostools.h"

#define DBG_PRFX "posix: " 
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 10 ) 


void posix_calc_future_usec( struct timespec *ts, 
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

//    DBG_PRINTF_NOTICE( "%s ns=%ld to=%ld tvsec=%ld\n", __FUNCTION__, num_seconds,
//                       timeout_usec, ts->tv_sec );
    
    /* check for potential overflow */
    XASSERT( timeout_usec < ULONG_MAX/1000, timeout_usec );

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
 * \brief  subtract two timespec values
 *
 * result = a - b
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
 */
#define NANOSEC_PER_SECOND  1000000000 /* one BILLLION */
int posix_timespec_sub( const struct timespec *a,
                        const struct timespec *b,
                        struct timespec *result )
{
    int64_t sec;
    uint32_t nsec;


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


/**
 * \brief  Wait a specific number of microseconds for a message
 *
 * Convenience wrapper function around mq_timedreceive()
 *
 * \retval integer ; the errno from the mq_timedreceive() call
 *
 * \author David Poole
 * \date 30-Jun-2008
 *
 */

int debug_posix = 0;
int debug_posix_cb() {
    debug_posix ^= 1;
    DBG_PRINTF_ERR("debug_posix: %d\n", debug_posix);
    return 0;
}

int posix_wait_for_message( mqd_t msgq, char *msg, int msg_size, unsigned long int timeout_in_usec )
{
    int mqretcode;
    struct timespec abs_timeout;
    int wait_errno;
    int error;
    struct mq_attr attr;

    if (timeout_in_usec == POSIX_WAIT_FOREVER)
    {
        mqretcode = (int)mq_receive( msgq, (char *)msg, msg_size, 0 );
    }
    else if (timeout_in_usec == 0 ) {
//      if ( debug_posix ) DBG_PRINTF_ERR ("[posix]tid:%d, time:%ld\n", gettid(), timeout_in_usec);
        mq_getattr(msgq, &attr);
        if ( attr.mq_curmsgs ) {
            mqretcode = (int)mq_receive( msgq, (char *)msg, msg_size, 0 );
        }
        else {
            return EAGAIN;
        }
    }
    else
    {
//      if ( debug_posix ) DBG_PRINTF_ERR("[posix]tid:%d, time:%ld\n", gettid(), timeout_in_usec);
        // always use clock_realtime as that is the clock used by mq_timedreceive
        error = clock_gettime( CLOCK_REALTIME, &abs_timeout );
        DBG_ASSERT(error == 0);
	if ( error ) return ETIMEDOUT; // release error 
        posix_calc_future_usec( &abs_timeout, timeout_in_usec );

        mqretcode = (int)mq_timedreceive( msgq, (char *)msg, msg_size, NULL, &abs_timeout );
    }

    wait_errno = 0;
    if ( mqretcode == -1 )
    {
        wait_errno = thread_errno;

        /* only a timeout error is acceptable at this point */
        XASSERT( wait_errno==ETIMEDOUT, wait_errno );
    }

    return wait_errno;
}

/**
 * \brief  Send a message waiting a specific number of 
 *         microseconds.
 *
 * Convenience wrapper function around mq_timedsend()
 *
 * \retval integer ; the errno from the mq_timedsend() call
 *
 */
int posix_message_send( mqd_t msgq, char *msg, int msg_size, unsigned long int priority, unsigned long int timeout_in_usec )
{
    int mqret;
    struct timespec abs_timeout;
    int wait_errno = 0;
    int error;

    if (timeout_in_usec == POSIX_WAIT_FOREVER)
    {
        mqret = mq_send( msgq, (char *)msg, msg_size, priority );
        REL_ASSERT(mqret == 0);
    }
    else
    {
        // always use clock_realtime as that is the clock used by mq_timedreceive
        error = clock_gettime( CLOCK_REALTIME, &abs_timeout );
        DBG_ASSERT(error == 0);
	if(error) 
	    return ETIMEDOUT;

        posix_calc_future_usec( &abs_timeout, timeout_in_usec );

        mqret = mq_timedsend( msgq, (char *)msg, msg_size, priority, &abs_timeout );
        if(mqret != 0)
        {
            /* only a timeout error is acceptable at this point */
            XASSERT( thread_errno==ETIMEDOUT, thread_errno );
            wait_errno = thread_errno;
        }
        else
        {
            XASSERT(mqret == 0, thread_errno);
        }
    }
    return wait_errno;
}



void posix_create_message_queue( mqd_t *msgq_ptr, const char *queue_name, int num_messages, int message_size )
{
    struct mq_attr mq_attrib;
    int mq_errno;
    mqd_t msgq;

    DBG_PRINTF_NOTICE( "%s name=%s nummsgs=%d\n", __FUNCTION__, queue_name, num_messages);

    memset( &mq_attrib, 0, sizeof(mq_attrib) );
    mq_attrib.mq_msgsize = message_size;
    mq_attrib.mq_maxmsg = num_messages;

    mq_unlink( queue_name ); // previous process might have left it open 
    //    msgq = mq_open( queue_name, O_RDWR|O_CREAT|O_EXCL, 0700, &mq_attrib );
    msgq = mq_open( queue_name, O_RDWR|O_CREAT|O_CLOEXEC, 0777, &mq_attrib );
    if (msgq == -1)
    {
        mq_errno = thread_errno;

        if (mq_errno == EEXIST)
        {
            DBG_PRINTF_NOTICE("%s queue=%s already exists so try to open\n",
                              __FUNCTION__, queue_name);

            msgq = mq_open(queue_name, O_RDWR);
            XASSERT(msgq != (mqd_t) -1, mq_errno);

            DBG_PRINTF_NOTICE("%s queue=%s opened successfully\n",
                              __FUNCTION__, queue_name);
        }
        else
        {
            XASSERT(msgq != (mqd_t) -1, mq_errno);
        }
    }

    *msgq_ptr = msgq;
}

/**
 * \brief  return the number of messages pending in a queue
 *
 * Originally created to help track memory leaks (messages can contain pointers
 * to malloc'd memory)
 *
 * \author David Poole
 * \date 01-Jul-2008
 *
 */

long int posix_get_queue_pending( mqd_t msgq )
{
    int mq_retcode;
    struct mq_attr attr;

    mq_retcode = (int)mq_getattr( msgq, &attr );
    REL_ASSERT( mq_retcode==0 );

    return attr.mq_curmsgs;
}

#ifdef DEBUG
void dump_thread_attr(pthread_attr_t* attr)
{
    int policy;
    struct sched_param sched_param;
    pthread_attr_getschedpolicy(attr, &policy);
    pthread_attr_getschedparam( attr, &sched_param );
    DBG_PRINTF_NOTICE("Thread attr:\n");
    DBG_PRINTF_NOTICE("sched_policy = %d\n", policy);
    DBG_PRINTF_NOTICE("sched pri = %d\n", sched_param.sched_priority);
    DBG_PRINTF_NOTICE("max pri for policy = %d\n", sched_get_priority_max(policy));
    DBG_PRINTF_NOTICE("min pri for policy = %d\n", sched_get_priority_min(policy));

    DBG_PRINTF_NOTICE("max pri for RR policy (%d) = %d\n", SCHED_RR, sched_get_priority_max(SCHED_RR));
    DBG_PRINTF_NOTICE("min pri for RR policy (%d) = %d\n", SCHED_RR, sched_get_priority_min(SCHED_RR));
}
#endif

/**
 * \brief  Create a thread, with optional stack and priority.
 *
 * Wrapper around the pthread calls to create a thread assigned to a specific
 * stack.
 *
 */

int posix_create_thread(pthread_t *thread_id, void *(*start_routine)(void*), void *thread_arg, const char* thread_name, void *stack_ptr, size_t stack_size, int priority)
{
    int retcode;
    pthread_attr_t attr;
    struct sched_param sched_param;
    char short_thread_name[16] = "un-named";

    DBG_PRINTF_NOTICE("%s start=%p stack=%p stack_size=%d\n", __FUNCTION__, start_routine,
                stack_ptr, stack_size );
                
    retcode = pthread_attr_init( &attr );
	XASSERT( retcode==0, retcode );
    if( retcode != 0 ) {
        return retcode;
    }
    
	if (stack_ptr)
	{
#ifdef __linux__
        XASSERT(stack_size >= PTHREAD_STACK_MIN, stack_size);
#endif
		retcode = pthread_attr_setstack( &attr, stack_ptr, stack_size );
		XASSERT( retcode==0, retcode );
	}

    if (priority >= POSIX_THR_PRI_REALTIME)
    {
        // set thread schedling policy
        retcode = pthread_attr_setschedpolicy(&attr, SCHED_RR);
        XASSERT( retcode==0, retcode );

        // set thread priority
        XASSERT(priority >= POSIX_THR_PRI_LOWEST && priority <= POSIX_THR_PRI_HIGHEST, priority);
        retcode = pthread_attr_getschedparam( &attr, &sched_param );
        XASSERT( retcode==0, retcode );

        sched_param.sched_priority = priority;

        retcode = pthread_attr_setschedparam( &attr, &sched_param );
        XASSERT( retcode==0, retcode );
    }
    else
    {
        retcode = pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        XASSERT( retcode==0, retcode );

        // set thread priority
        retcode = pthread_attr_getschedparam( &attr, &sched_param );
        XASSERT( retcode==0, retcode );

        sched_param.sched_priority = 0; // SCHED_OTHER threads must be created with priority of 0

        retcode = pthread_attr_setschedparam( &attr, &sched_param );
        XASSERT( retcode==0, retcode );
    }

    retcode = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    XASSERT( retcode==0, retcode );

#ifdef DEBUG
    dump_thread_attr(&attr);    
#endif

#ifdef __linux__
    retcode = pthread_create( thread_id, &attr, start_routine, thread_arg );
    XASSERT( retcode==0, retcode );
    if( (thread_name != NULL) && (0 != thread_name[0]) )
    {
        strncpy(short_thread_name,thread_name,15);
        short_thread_name[15] = 0;
    }
    retcode = pthread_setname_np(*thread_id, short_thread_name);
#else
    retcode = pthread_create_named( thread_id, thread_name, &attr, start_routine, thread_arg );    //named not supported on linux
#endif    
    XASSERT( retcode==0, retcode );
	
    pthread_attr_destroy(&attr);

	return retcode;
}

int posix_sleep_ms(uint32_t timeout_ms)
{
    struct timespec sleep_time;

    sleep_time.tv_sec = timeout_ms/MILLISEC_PER_SECOND;
    sleep_time.tv_nsec = (timeout_ms%MILLISEC_PER_SECOND) * (NANOSEC_PER_USEC*USEC_PER_MILLISEC);

    return nanosleep(&sleep_time, NULL);
}

int posix_sleep_us(uint32_t timeout_us)
{
    struct timespec sleep_time;

    sleep_time.tv_sec = timeout_us/USEC_PER_SECOND;
    sleep_time.tv_nsec = (timeout_us%USEC_PER_SECOND) * (NANOSEC_PER_USEC);

    return nanosleep(&sleep_time, NULL);
}

uint32_t posix_gettime_ms( void )
{
    uint32_t mstime;
    struct timespec time;
    int px_status;
    
    // attempt to get CLOCK_MONOTONIC value first
    px_status = clock_gettime( CLOCK_MONOTONIC, &time );
    if (px_status != 0)
    {
        // if CLOCK_MONOTONIC is not supported, then use CLOCK_REALTIME
        px_status = clock_gettime( CLOCK_REALTIME, &time );
    }
    XASSERT( px_status==0, px_status );
    
    mstime = (time.tv_sec * 1000) + (time.tv_nsec/1000000);
    
    return mstime;
}

int posix_mutex_init(pthread_mutex_t *mutex)
{
    int px_status;
    pthread_mutexattr_t   mta;

    memset(&mta, 0, sizeof(pthread_mutexattr_t));

    px_status = pthread_mutexattr_init(&mta);
    XASSERT(px_status == 0, px_status);
    
    px_status = pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
    XASSERT(px_status == 0, px_status);

    px_status = pthread_mutex_init( mutex, &mta );
    XASSERT(px_status == 0, px_status);

    pthread_mutexattr_destroy(&mta);
    
    return px_status;
}

sem_t* posix_sem_open(const char* name, uint32_t init_value)
{
    sem_t* sem;

    ASSERT(name);
    ASSERT(name[0] == '/');  // on linux name must begin with a '/' character 
                             // (essentially be fully qualified in namespace)

    if(!sem_unlink( name )) // previous process might have left it open 
    {
        DBG_PRINTF_NOTICE("%s: Unlinked %s prior to open\n", __func__, name);
    }
    sem = sem_open(name, O_CREAT|O_CLOEXEC, 0700, init_value);
    ASSERT(sem != SEM_FAILED);
    if (sem == SEM_FAILED)
    {
        sem = NULL;
    }
    return sem;
}

int posix_sem_timedwait_ms(sem_t *sem, uint32_t timeout_ms)
{
    struct timespec wait_time;
    int error;

    // always use clock_realtime as that is the clock used by sem_timedwait
    error = clock_gettime( CLOCK_REALTIME, &wait_time );
    ASSERT(error == 0);
        
    XASSERT(timeout_ms*USEC_PER_MILLISEC >= timeout_ms, timeout_ms); // test for rollover; >= instead of > handles timeout of 0
    posix_calc_future_usec(&wait_time, (timeout_ms*USEC_PER_MILLISEC));

    error = sem_timedwait(sem, &wait_time);
    if(error != 0)
    {
        error = -errno;
        ASSERT(error == -ETIMEDOUT);
    }
    return error;
}

void posix_display_thread_sched_attr(const char* name)
{
    int policy;
    struct sched_param param;

    pthread_getschedparam(pthread_self(), &policy, &param);

    dbg_printf("%s: TID = %d policy=%s, priority=%d\n",
            name,
            gettid(),
            (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
            (policy == SCHED_RR)    ? "SCHED_RR" :
            (policy == SCHED_OTHER) ? "SCHED_OTHER" :
            "UNKNOWN",
            param.sched_priority);
}

#include <stdio.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>	 
pid_t gettid( void )
{
        return syscall( __NR_gettid );
}
