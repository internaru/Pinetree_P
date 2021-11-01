/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scos_kernel.c
 *
 * \brief RTOS abstraction functions, Linux kernel implementation
 *
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/delay.h>

#include "scos.h"

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "ostools.h"

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

int scos_get_errno( void )
{
    return -1;
}

int clock_gettime( clockid_t clk_id, struct timespec *now )
{
    if( clk_id != CLOCK_REALTIME ) {
        return -1;
    }

    *now = current_kernel_time();

    return 0;
}

scan_err_t scos_mq_receive( struct scos_mq *msgq, scan_msg_t *msg, unsigned long int timeout_in_usec )
{
    int retcode;
    unsigned long jiffies;
    struct timeval tv_timeout;

//    dbg2( "%s\n", __FUNCTION__ );

    if( timeout_in_usec == SCOS_WAIT_FOREVER ) {
        retcode = down_interruptible( &msgq->sem );
    }
    else { 
        tv_timeout.tv_sec = 0;
        tv_timeout.tv_usec = timeout_in_usec;

        jiffies = timeval_to_jiffies( &tv_timeout );

        retcode = down_timeout( &msgq->sem, jiffies );
    }

//    dbg2( "%s down retcode=%d\n", __FUNCTION__, retcode );
    if( retcode != 0 ) {
        /* only a timeout error is acceptable at this point */
        XASSERT( retcode==-ETIME, retcode );
        return SCANERR_TIMEDOUT;
    }

    memset( msg, 0, sizeof(scan_msg_t));
    retcode = kfifo_out_spinlocked( &msgq->fifo, msg, 
                        sizeof(scan_msg_t), 
                        &msgq->lock );

//    dbg2( "%s kfifo_out retcode=%d\n", __FUNCTION__, retcode );
    XASSERT( retcode==sizeof(scan_msg_t), retcode );

    return SCANERR_NONE;
}

scan_err_t scos_mq_send( struct scos_mq *msgq, scan_msg_t *msg, uint32_t wait_usecs )
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

        return SCANERR_MSG_FAIL;
    }

    up( &msgq->sem );

    return SCANERR_NONE;
}

scan_err_t scos_mq_send_nowait( struct scos_mq *msgq, scan_msg_t *msg )
{
    int retcode;
    /*
     * BIG FAT NOTE!
     *
     * This can be called from interrupt context!
     */

//    dbg2( "%s msg=%d\n", __FUNCTION__, msg->msgType );

    retcode = kfifo_in_spinlocked( &msgq->fifo, msg, 
                        sizeof(scan_msg_t), &msgq->lock );
    if( retcode != sizeof(scan_msg_t) ) {
        dbg2( "%s kfifo_in retcode=%d size=%d\n", __FUNCTION__, retcode, sizeof(scan_msg_t) );
        XASSERT( retcode==sizeof(scan_msg_t), retcode );
        return SCANERR_MSG_FAIL;
    }

    up( &msgq->sem );

    return SCANERR_NONE;
}

scan_err_t scos_mq_init( struct scos_mq *msgq, const char *name, int queue_size )
{
    int retcode;

    dbg2( "%s \"%s\" size=%d\n", __FUNCTION__, name, queue_size );

    retcode = kfifo_alloc( &msgq->fifo,
                            roundup_pow_of_two(queue_size*sizeof(scan_msg_t)), 
                            GFP_KERNEL );
//    retcode = kfifo_alloc( &msgq->fifo, queue_size*sizeof(scan_msg_t), GFP_KERNEL );
    if( retcode != 0 ) {
        printk( KERN_INFO "kfifo_alloc() retcode=%d\n", retcode );
        return SCANERR_MSG_FAIL;
    }

    spin_lock_init( &msgq->lock );
    sema_init( &msgq->sem, 0 );

    return SCANERR_NONE;
}

void scos_mq_free( struct scos_mq *msgq )
{
    kfifo_free( &msgq->fifo );
}

uint32_t scos_mq_num_pending( struct scos_mq *msgq )
{
    /* FIXME - I should probably be locking this structure before peeking into
     * it.  But this function only used by test/debug code so I'll throw the
     * dice and read unlocked.
     */
    return kfifo_len(&msgq->fifo);
}

int scos_sleep_microseconds(unsigned long microseconds)
{
    udelay( microseconds );
    return 0;
}

int scos_sleep_milliseconds(unsigned long milliseconds)
{
    mdelay( milliseconds );
    return 0;
}

scan_err_t scos_sem_init( struct scos_sem *sem, const char *name, int initial_value )
{
    memset( sem, 0, sizeof(struct scos_sem) );

    strncpy( sem->name, name, MAX_SEM_NAME );

    /* davep 11-Sep-2012 ; cookie is mostly to catch attempting to use
     * uninitialized semaphores (happens often in regression test code that
     * doesn't call the proper setup functions)
     */
    sem->cookie = SCOS_SEM_COOKIE;

    sema_init( &sem->sem, initial_value );

    return SCANERR_NONE;
}

scan_err_t scos_sem_wait( struct scos_sem *sem )
{
    int retcode;

    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    retcode = down_interruptible( &sem->sem );

    return SCANERR_NONE;
}

scan_err_t scos_sem_trywait( struct scos_sem *sem )
{
    int retcode;

    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    retcode = down_trylock( &sem->sem );
    if( retcode != 0 ) {
        return SCANERR_BUSY;
    }

    return SCANERR_NONE;
}

scan_err_t scos_sem_post( struct scos_sem *sem )
{
    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    up( &sem->sem );

    return SCANERR_NONE;
}

scan_err_t scos_sem_getvalue( struct scos_sem *sem, int *value )
{
    XASSERT( sem->cookie==SCOS_SEM_COOKIE, (uint32_t)sem );

    /* davep 11-Sep-2012 ; XXX this is unsafe and violates the kernel data
     * structure encapsulation! 
     */
    *value = sem->sem.count;

    return SCANERR_NONE;
}

scan_err_t scos_sem_timedwait( struct scos_sem *sem, struct timespec *abs_timeout )
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
        return SCANERR_TIMEDOUT;
    }

    return SCANERR_NONE;
}

scan_err_t scos_sem_is_initialized( struct scos_sem *sem )
{
    if( sem->cookie != SCOS_SEM_COOKIE ) {
        /* not initialized */
        return SCANERR_NOT_PERMITTED;
    }
    
    return SCANERR_NONE;
}

/**
 * \brief scos wrapper to tell if we're in interrupt context or not
 *
 * (Note: Linux kernel in_interrupt() is not 100% reliable.)
 * TODO more research to understand why
 *
 * \author David Poole
 * \date 14-Jan-2013
 */

int scos_in_interrupt( void )
{
    return in_interrupt();
}

