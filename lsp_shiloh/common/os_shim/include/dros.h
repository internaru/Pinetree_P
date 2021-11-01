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
 * \file dros.h
 *
 * \brief DRiver level Operating Systems primatives.  This is an OS abstraction 
 * layer for device drivers.  It is intended to allow portability of drivers 
 * to multiple operating systems such as Linux, ThreadX, etc. 
 *
 */


#ifndef _DROS_H
#define _DROS_H

#include <stdint.h>
#include <stdbool.h>
#include "dros_types.h"

/** 
 * Error codes returned from any/all/most of the scan subsystem functions.
 *
 * Please try to keep the errors general enough to be used in more than one
 * situation but specific enough to be useful. See errno for inspiration.
 *
 * All these numbers should be negative so we'll behave like errno which most
 * people are already familiar.
 */
typedef enum {
    DROSERR_NONE          =   0, ///< no error. Huzzah!
    DROSERR_FAIL          =  -1, ///< something unexpected went wrong
    DROSERR_OUT_OF_MEMORY =  -2, ///< ran out of memory (ENOMEM)
    DROSERR_NO_ENTRY      =  -3, ///< a table lookup failed (ENOENT)
    DROSERR_INVALID_PARAM =  -6, ///< invalid parameters to function (EINVAL)
    DROSERR_BUSY          = -10, ///< scanner is busy (EBUSY)
    DROSERR_NOT_IMPLEMENTED=-13, ///< called a function with no implementation
    DROSERR_NOT_PERMITTED = -14, ///< operation not permitted (EPERM)
    DROSERR_ALREADY       = -15, ///< operation already in progress (EALREADY)
    DROSERR_TIMEDOUT      = -17, ///< operation timed out (ETIMEDOUT)

} dros_err_t;

// Semaphore API
dros_err_t dros_sem_wait( dros_sem_t *sem );
dros_err_t dros_sem_trywait( dros_sem_t *sem );
dros_err_t dros_sem_post( dros_sem_t *sem );
dros_err_t dros_sem_init( dros_sem_t *sem, const char *name, int initial_value );
dros_err_t dros_sem_getvalue( dros_sem_t *sem, int *value );
dros_err_t dros_sem_timedwait( dros_sem_t *sem, struct timespec *abs_timeout );
dros_err_t dros_sem_is_initialized( dros_sem_t *sem );

// Thread API
dros_err_t dros_create_thread(dros_thread_t **thread_id, int (*start_routine)(void*), void *thread_arg, const char* thread_name, size_t min_stack_size, int priority);
dros_err_t dros_destroy_thread(dros_thread_t *thread_id);

// Message Queue API
dros_err_t dros_mq_receive( dros_mq_t *msgq, void* msg, unsigned long int timeout_in_usec );
dros_err_t dros_mq_send_nowait( dros_mq_t *msgq, void *msg );
dros_err_t dros_mq_init( dros_mq_t *msgq, const char *name, int queue_depth, int msg_size );
void dros_mq_free( dros_mq_t *msgq );

// Symmetric Multi Processor synchronization locks
/// \brief Initialize SMP lock
void dros_smplock_init(dros_smp_lock_t* lock);

/// \brief Acquire SMP lock
void dros_smplock_acquire(dros_smp_lock_t* lock); 

/// \brief Release SMP lock
void dros_smplock_release(dros_smp_lock_t* lock);

/// \brief Acquire SMP lock and disable interrupts on current processor
void dros_smplock_acquire_isr(dros_smp_lock_t* lock, uint32_t* flags);

/// \brief Release SMP lock and restore interrupts to previous posture
void dros_smplock_release_isr(dros_smp_lock_t* lock, uint32_t flags);

#endif //_DROS_H

