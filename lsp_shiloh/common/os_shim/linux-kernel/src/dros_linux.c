/*
 **************************************************************************************
 *
 * Copyright (c) 2013 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
*/

/**
 * \file dros_kernel.c
 *
 * \brief DRiver level Operating Systems primatives.
 *
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/delay.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "dros.h"

#include "lassert.h"

#if 0
int nanosleep(struct timespec *req, struct timespec *rem)
{
    if( req->tv_sec ) {
        ssleep( req->tv_sec );
    }
    if( req->tv_nsec ) {
        ndelay( req->tv_nsec );
    }

    return 0;
}

void pthread_yield(void)
{
    schedule();
}

int posix_sleep_seconds(unsigned long seconds)
{
    ssleep( seconds );
    return 0;
}

int clock_gettime( clockid_t clk_id, struct timespec *now )
{
    if( clk_id != CLOCK_REALTIME ) {
        return -1;
    }

    *now = current_kernel_time();

    return 0;
}
#endif

dros_err_t dros_mq_receive( dros_mq_t *msgq, void* msg, unsigned long int timeout_in_usec )
{
    int errcode = mesgq_wait_for_msg(msgq, msg, timeout_in_usec);
    if(errcode == 0)
    {
        return DROSERR_NONE;
    }
    else if(errcode == -ETIME)
    {
        return DROSERR_TIMEDOUT;
    }
    return DROSERR_FAIL;
}

#if 0
dros_err_t dros_mq_send( struct dros_mq_t *msgq, scan_msg_t *msg, uint32_t wait_usecs )
{
    int retcode;

    BUG_ON( in_interrupt() );

//    dbg2( "qs %d\n", kfifo_avail(&msgq->fifo) );

    /* FIXME how do I block if the queue is full? */
    retcode = kfifo_in_spinlocked( &msgq->fifo, msg, 
                        sizeof(scan_msg_t), &msgq->lock );
    if( retcode != sizeof(scan_msg_t) ) {

        dbg2( "%s kfifo_in retcode=%d\n", __FUNCTION__, retcode );

        dbg2( "%s full=%d len=%d avail=%d\n", __FUNCTION__, 
                kfifo_is_full(&msgq->fifo), kfifo_len(&msgq->fifo),
                kfifo_avail(&msgq->fifo) );

        return DROSERR_MSG_FAIL;
    }

    up( &msgq->sem );

    return DROSERR_NONE;
}
#endif

dros_err_t dros_mq_send_nowait( dros_mq_t *msgq, void *msg )
{
    int errcode;

    /*
     * BIG FAT NOTE!
     *
     * This can be called from interrupt context!
     */
    errcode = mesgq_send_nowait(msgq, msg);
    if(errcode)
    {
        return DROSERR_FAIL;
    }

    return DROSERR_NONE;
}

dros_err_t dros_mq_init( dros_mq_t *msgq, const char *name, int queue_depth, int msg_size )
{
    int errcode;

    errcode = mesgq_create(msgq, msg_size, queue_depth);
    if(errcode)
    {
        return DROSERR_FAIL;
    }

    return DROSERR_NONE;
}

void dros_mq_free( dros_mq_t *msgq )
{
    mesgq_free(msgq);
}

#if 0

int dros_sleep_microseconds(unsigned long microseconds)
{
    udelay( microseconds );
    return 0;
}

int dros_sleep_milliseconds(unsigned long milliseconds)
{
    mdelay( milliseconds );
    return 0;
}
#endif

dros_err_t dros_sem_init( dros_sem_t *sem, const char *name, int initial_value )
{
    memset( sem, 0, sizeof(dros_sem_t) );

    strncpy( sem->name, name, MAX_SEM_NAME );

    /* davep 11-Sep-2012 ; cookie is mostly to catch attempting to use
     * uninitialized semaphores (happens often in regression test code that
     * doesn't call the proper setup functions)
     */
    sem->cookie = DROS_SEM_COOKIE;

    sema_init( &sem->sem, initial_value );

    return DROSERR_NONE;
}

dros_err_t dros_sem_wait( dros_sem_t *sem )
{
    int retcode;

    XASSERT( sem->cookie==DROS_SEM_COOKIE, (uint32_t)sem );

    retcode = down_interruptible( &sem->sem );

    return DROSERR_NONE;
}

dros_err_t dros_sem_trywait( dros_sem_t *sem )
{
    int retcode;

    XASSERT( sem->cookie==DROS_SEM_COOKIE, (uint32_t)sem );

    retcode = down_trylock( &sem->sem );
    if( retcode != 0 ) {
        return DROSERR_BUSY;
    }

    return DROSERR_NONE;
}

dros_err_t dros_sem_post( dros_sem_t *sem )
{
    XASSERT( sem->cookie==DROS_SEM_COOKIE, (uint32_t)sem );

    up( &sem->sem );

    return DROSERR_NONE;
}

dros_err_t dros_sem_getvalue( dros_sem_t *sem, int *value )
{
    XASSERT( sem->cookie==DROS_SEM_COOKIE, (uint32_t)sem );

    /* davep 11-Sep-2012 ; XXX this is unsafe and violates the kernel data
     * structure encapsulation! 
     */
    *value = sem->sem.count;

    return DROSERR_NONE;
}

#if 0
dros_err_t dros_sem_timedwait( dros_sem_t *sem, struct timespec *abs_timeout )
{
    unsigned long jiffies;
//    struct timeval tv_timeout;
    int retcode;

    /* calculate our future timeout in jiffies */
//    tv_timeout.tv_sec = 0;
//    tv_timeout.tv_usec = timeout_in_usec;

    jiffies = timespec_to_jiffies(abs_timeout);

    retcode = down_timeout( &sem->sem, jiffies );
    if( retcode != 0 ) {
        /* only a timeout error is acceptable at this point */
        XASSERT( retcode==-ETIME, retcode );
        return DROSERR_TIMEDOUT;
    }

    return DROSERR_NONE;
}
#endif

dros_err_t dros_sem_is_initialized( dros_sem_t *sem )
{
    if( sem->cookie != DROS_SEM_COOKIE ) {
        /* not initialized */
        return DROSERR_NOT_PERMITTED;
    }
    
    return DROSERR_NONE;
}

// stack size of 0 means use system default stack min size
dros_err_t dros_create_thread(dros_thread_t **thread_id, int(*start_routine)(void*), void *thread_arg, const char* thread_name, size_t min_stack_size, int priority)
{
    struct sched_param sparam;
    dros_thread_t* tid = kmalloc( sizeof(dros_thread_t), GFP_KERNEL );

    if(!tid)
    {
        return DROSERR_OUT_OF_MEMORY;
    }
    tid->task = kthread_run(start_routine, thread_arg, thread_name);
    if(IS_ERR(tid->task))
    {
        kfree(tid);
        return DROSERR_FAIL;
    }

    sparam.sched_priority = priority;
	sched_setscheduler(tid->task, SCHED_RR, &sparam);

    *thread_id = tid;
    return DROSERR_NONE;
}

dros_err_t dros_destroy_thread(dros_thread_t *thread_id)
{
    kthread_stop(thread_id->task);
    kfree(thread_id);
    return DROSERR_NONE;
}

// Symmetric Multi Processor synchronization locks
/// \brief Initialize SMP lock
void dros_smplock_init(dros_smp_lock_t* lock)
{
    spin_lock_init(&lock->lock);
}

/// \brief Acquire SMP lock
void dros_smplock_acquire(dros_smp_lock_t* lock)
{
    spin_lock(&lock->lock);
}

/// \brief Release SMP lock
void dros_smplock_release(dros_smp_lock_t* lock)
{
    spin_unlock(&lock->lock);
}

/// \brief Acquire SMP lock and disable interrupts on current processor
void dros_smplock_acquire_isr(dros_smp_lock_t* lock, uint32_t* flags)
{
    unsigned long _flags;
    spin_lock_irqsave(&lock->lock, _flags);
    *flags = _flags;
}

/// \brief Release SMP lock and restore interrupts to previous posture
void dros_smplock_release_isr(dros_smp_lock_t* lock, uint32_t flags)
{
    spin_unlock_irqrestore(&lock->lock, flags);
}

