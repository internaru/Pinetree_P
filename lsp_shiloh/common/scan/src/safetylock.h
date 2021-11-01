/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SAFETYLOCK_H
#define SAFETYLOCK_H

#define SAFETY_LOCK_SEM_NAME_SIZE (10+1) /* +1 for NULL */

/* if you seen a failed assert on this value, we're probably trying to operate
 * on an uninitialized (or corrupted) semaphore
 */
#define SAFETY_LOCK_COOKIE 0x4EFD469D

#ifdef __KERNEL__
struct safety_lock {
    uint32_t cookie; /* used with validity tests */
    struct semaphore lock;
};
#else
struct safety_lock {
    uint32_t cookie; /* used with validity tests */
    sem_t lock;
};
#endif

void safety_lock_init( struct safety_lock *slock );

void safety_lock_free( struct safety_lock *slock );

scan_err_t safety_lock_try_lock( struct safety_lock *slock );

scan_err_t safety_lock_lock_with_timeout( struct safety_lock *slock, uint32_t timeout_in_usec );

void safety_lock_lock( struct safety_lock *slock );

void safety_lock_unlock( struct safety_lock *slock );

void safety_lock_test_self( void );

#endif

