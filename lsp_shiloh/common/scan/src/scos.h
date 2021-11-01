/**
 * \file scos.h
 *
 * \brief Generic Operating Systems primatives for ScanTask.
 *
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCOS_H
#define SCOS_H

#ifdef HAVE_TX_POSIX_2008
    #error tx_posix_2008 deprecated in favor of pthread.h
    /* POSIX -> ThreadX shim */
#elif defined HAVE_TX_POSIX_2011
    /* POSIX -> ThreadX shim */
    #include <stdint.h>
    #include <stdbool.h>
    #include "pthread.h"

    #define MAX_MQ_NAME 32
    struct scos_mq {
        mqd_t mq;
        char name[MAX_MQ_NAME+1];
    };

    #define MAX_SEM_NAME 32
    #define SCOS_SEM_COOKIE 0xF2EEFFEE
    struct scos_sem {
        uint32_t cookie; /* used with validity tests */
        sem_t *posix_sem;
        char name[MAX_SEM_NAME+1];
    };

    typedef void * thread_return_t;
#elif defined __KERNEL__ 
    /* running in Linux kernel */
    #include <linux/module.h> 
    #include <linux/kernel.h> 
    #include <linux/mutex.h>
    #include <linux/time.h>
    #include <linux/kthread.h>
    #include <linux/interrupt.h>
    #include <linux/dma-mapping.h>
    #include <linux/platform_device.h>
    #include <linux/slab.h>
    #include <linux/semaphore.h>
    #include <linux/kfifo.h>

    #include "scos_kernel.h"
#elif defined __linux__
    /* use a real POSIX (running in Unix-y userspace) */
    #include <stdint.h>
    #include <stdbool.h>
    #include <errno.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include <mqueue.h>
    #include <signal.h>

    /* cheap hack to handle some core code header files using ThreadX
     * primatives 
     */
    typedef void TX_EVENT_FLAGS_GROUP;

    #define MAX_MQ_NAME 32
    struct scos_mq {
        mqd_t mq;
        char name[MAX_MQ_NAME+1];
    };

    #define MAX_SEM_NAME 32
    #define SCOS_SEM_COOKIE 0xB6DD8096
    struct scos_sem {
        uint32_t cookie; /* used with validity tests */
        sem_t *posix_sem;
        char name[MAX_SEM_NAME+1];
    };

    typedef void * thread_return_t;
#elif defined __APPLE__
    #include <stdint.h>
    #include <stdbool.h>
    #include <errno.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <semaphore.h>

    #include "scos_osx.h"
#else
    /* assume we're building standalone and require linking against an external
     * library to provide us with the POSIX functions
     */
    #error no known RTOS
#endif

/* default priority on calls to mq_send(), mq_receive() */
#define MQ_DEFAULT_PRIORITY  0

/* most of the my functions are using microsoconds in an attempt to be RTOS
 * cross platform  (as opposed to using ticks)
 */
#define MSEC_PER_SECOND     1000     /* one thousand */
#define USEC_PER_SECOND     1000000  /* one million */
#define USEC_PER_MILLISEC   1000     /* one thousand */
#define NANOSEC_PER_SECOND  1000000000 /* one BILLLION */
#define NANOSEC_PER_USEC    1000     /* one thousand */
#define NANOSEC_PER_MILLISEC  1000000  /* one million */

/* wrapper functions around the waitable functions will pass these flags */
#define SCOS_WAIT_FOREVER  (~0)
#define SCOS_NO_WAIT         0

/* instead of using a global 'errno', this function needs to return the errno of
 * the last call
 */
int scos_get_errno( void );

/* davep 02-Jul-2008 ; rename to scos_sleep_seconds() to obviate risk of
 * colliding with C library's sleep()
 */
int posix_sleep_seconds(unsigned long seconds);

/* davep 18-Oct-2011 ; wrapper around nanosleep(); convenience function for
 * microsecond sleeps 
 */
int scos_sleep_microseconds( unsigned long usec_count );

int scos_sleep_milliseconds( unsigned long msec_count );

pthread_cond_t * scos_alloc_pthread_cond( void );
void scos_free_pthread_cond( pthread_cond_t ** cond );

pthread_mutex_t * scos_alloc_pthread_mutex( void );
void scos_free_pthread_mutex( pthread_mutex_t ** mutex );

pthread_mutexattr_t * scos_alloc_pthread_mutexattr( void );
void scos_free_pthread_mutexattr( pthread_mutexattr_t ** mutex_attr );

pthread_attr_t * scos_alloc_pthread_attr( void );
void scos_free_pthread_attr( pthread_attr_t **pthread_attr );

void * scos_alloc_pthread_stack( size_t size_in, size_t *size_out );
void scos_free_pthread_stack( void **stack_ptr );

int scos_in_interrupt( void );

#endif

