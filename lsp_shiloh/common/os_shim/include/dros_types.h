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
 * \file dros_typtes.h
 *
 * \brief DRiver level Operating Systems types.  This is an OS abstraction 
 * layer for device drivers.  It is intended to allow portability of drivers 
 * to multiple operating systems such as Linux, ThreadX, etc. 
 *
 */


#ifndef _DROS_TYPES_H
#define _DROS_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SEM_NAME 32
#define DROS_SEM_COOKIE 0x281F476E

#define DROS_WAIT_FOREVER  (~0)
#define DROS_NO_WAIT         0

#ifdef __KERNEL__
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include "mesg_queue.h"

typedef struct dros_sem_s {
    uint32_t cookie; /* used with validity tests */
    struct semaphore sem;
    char name[MAX_SEM_NAME+1];
}dros_sem_t;

typedef mesg_queue_t dros_mq_t;

/** 
 *  \brief The thread priority defines; on Linux all dros threads are realtime priority.
 **/
#define DROS_THR_PRI_LOWEST       (MAX_RT_PRIO)
#define DROS_THR_PRI_HIGHEST       (0)
#define DROS_THR_PRI_NORMAL        ((DROS_THR_PRI_HIGHEST + DROS_THR_PRI_LOWEST)/2)
#define DROS_THR_PRI_DRIVER        (DROS_THR_PRI_NORMAL - 10)
#define DROS_THR_PRI_IMAGE_DRIVER  (DROS_THR_PRI_DRIVER - 1)

typedef struct dros_thread_s
{
    struct task_struct *task;
}dros_thread_t;

typedef struct dros_smp_lock_s
{
    spinlock_t lock;
}dros_smp_lock_t;

#else
#error "Support for this OS is currently unimplemented"
#endif

#endif //_DROS_H

