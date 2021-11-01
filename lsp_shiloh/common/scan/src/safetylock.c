/**
 * \file safetylock.c
 *
 * \brief Ha-ha portable mutual exclusion lock.
 *
 * The scantask code is one thread but multiple interrupt handlers. This data
 * structure is called a 'safety lock' only because I only use it to verify I'm
 * not corrupting data structures across thread/interrupt boundaries. 
 *
 * davep 7-Jun-2012
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "ostools.h"

#ifdef __KERNEL__
/*****************************************************
 *  Linux kernel semaphores
 */

void safety_lock_init( struct safety_lock *slock )
{
    memset( slock, 0, sizeof(struct safety_lock) );
    sema_init( &slock->lock, 1 );
    slock->cookie = SAFETY_LOCK_COOKIE;
}

void safety_lock_free( struct safety_lock *slock )
{
    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );
    /* ??? */
}

scan_err_t safety_lock_try_lock( struct safety_lock *slock )
{
    int retcode;

    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    retcode = down_trylock( &slock->lock );
    if( retcode != 0 ) {
        return SCANERR_BUSY;
    }
    return SCANERR_NONE;
}

scan_err_t safety_lock_lock_with_timeout( struct safety_lock *slock, uint32_t timeout_in_usec )
{
    unsigned long jiffies;
    struct timeval tv_timeout;
    int retcode;

    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    /* calculate our future timeout in jiffies */
    tv_timeout.tv_sec = 0;
    tv_timeout.tv_usec = timeout_in_usec;

    jiffies = timeval_to_jiffies( &tv_timeout );

    retcode = down_timeout( &slock->lock, jiffies );
    if( retcode != 0 ) {
        /* only a timeout error is acceptable at this point */
        XASSERT( retcode==-ETIME, retcode );
        return SCANERR_TIMEDOUT;
    }

    return SCANERR_NONE;
}

void safety_lock_lock( struct safety_lock *slock )
{
    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    down( &slock->lock );
}

void safety_lock_unlock( struct safety_lock *slock )
{
    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    up( &slock->lock );
}

#else
/*****************************************************
 *  POSIX semaphores
 */

void safety_lock_init( struct safety_lock *slock )
{
    int retcode;

    memset( slock, 0, sizeof(struct safety_lock) );
    retcode = sem_init( &slock->lock, 0, 1 ); 
    XASSERT( retcode==0, retcode );
    slock->cookie = SAFETY_LOCK_COOKIE;
}

void safety_lock_free( struct safety_lock *slock )
{
    int retcode;

    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    retcode = sem_destroy( &slock->lock );
    XASSERT( retcode==0, scos_get_errno() );
}

scan_err_t safety_lock_try_lock( struct safety_lock *slock )
{
    int retcode; 

    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    retcode=sem_trywait( &slock->lock ); 
    if( retcode != 0 ) {
        return SCANERR_BUSY;
    }
    return SCANERR_NONE;
}

scan_err_t safety_lock_lock_with_timeout( struct safety_lock *slock, uint32_t timeout_in_usec )
{
    struct timespec abs_timeout;
    int retcode;

    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    /* calculate our future timeout in microseconds */
    clock_gettime( CLOCK_REALTIME, &abs_timeout );
    calc_future_usec( &abs_timeout, timeout_in_usec );

    retcode = sem_timedwait( &slock->lock, &abs_timeout );
    if( retcode != 0 ) {
        /* TODO check for error */
        return SCANERR_BUSY;
    }

    return SCANERR_NONE;
}

void safety_lock_lock( struct safety_lock *slock )
{
    int retcode; 

    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    retcode=sem_wait( &slock->lock ); 
    XASSERT( retcode==0, retcode );
}

void safety_lock_unlock( struct safety_lock *slock )
{
    int retcode; 

    XASSERT( slock->cookie==SAFETY_LOCK_COOKIE, (uint32_t)slock );

    retcode=sem_post( &slock->lock ); 
    XASSERT( retcode==0, scos_get_errno() ); 
}
#endif

void safety_lock_test_self( void )
{
    scan_err_t scerr;
    struct safety_lock slock;

    dbg2( "%s start\n", __FUNCTION__ );

    memset( &slock, 0, sizeof(struct safety_lock) );

    safety_lock_init( &slock );

    safety_lock_lock( &slock );
    safety_lock_unlock( &slock );

    scerr = safety_lock_try_lock( &slock );
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = safety_lock_try_lock( &slock );
    XASSERT( scerr==SCANERR_BUSY, scerr );

    safety_lock_unlock( &slock );

    /* lock it. try the lock with timeout */
    safety_lock_lock( &slock );
    scerr = safety_lock_lock_with_timeout( &slock, USEC_PER_SECOND/2 );
    XASSERT( scerr==SCANERR_TIMEDOUT, scerr );
    safety_lock_unlock( &slock );

    safety_lock_free( &slock );

    dbg2( "%s done\n", __FUNCTION__ );
}

