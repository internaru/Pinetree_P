/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCOS_KERNEL_H
#define SCOS_KERNEL_H

struct scos_mq {
    struct semaphore sem;
    struct kfifo fifo;
    spinlock_t lock;
};

#define MAX_SEM_NAME 32
#define SCOS_SEM_COOKIE 0x281F476E
struct scos_sem {
    uint32_t cookie; /* used with validity tests */
    struct semaphore sem;
    char name[MAX_SEM_NAME+1];
};

typedef struct task_struct * pthread_t;
typedef void * pthread_attr_t;
typedef void * pthread_condattr_t;
typedef void * pthread_cond_t;
typedef void * pthread_mutex_t;
typedef void * pthread_mutexattr_t;

int nanosleep(struct timespec *req, struct timespec *rem);

void pthread_yield(void);

int clock_gettime( clockid_t clk_id, struct timespec *now );

typedef int thread_return_t;

#endif

