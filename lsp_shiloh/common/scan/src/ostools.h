/**
 * \file ostools.h
 *
 * \brief Header file for ostools.c
 *
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef OSTOOLS_H
#define OSTOOLS_H

void calc_future_usec( struct timespec *ts, 
                       unsigned long int timeout_usec );

scan_err_t create_stacked_thread( pthread_t *thd_id, 
                            void *(*start_routine)(void *),
                            void *thread_arg,
                            void *stack_ptr,
                            size_t stack_size );

/* subtract two struct timespec values (not POSIX) */
int timespec_subtract( const struct timespec *a, 
                       const struct timespec *b, 
                       struct timespec *result );

/* davep 18-Jun-2012 ; new message queue wrapper for threadx and linux kernel
 * portability. Can't put this into scos.h because scos.h preceeds scantypes.h
 * and I'm surrendering to the inevitable and using scan_err_t and scan_msg_t
 * directly. So there. Nyah.
 */

scan_err_t scos_mq_receive( struct scos_mq *msgq, scan_msg_t *msg, 
                                unsigned long int timeout_in_usec );

scan_err_t scos_mq_send_nowait( struct scos_mq *msgq, scan_msg_t *msg );

scan_err_t scos_mq_send( struct scos_mq *msgq, scan_msg_t *msg, 
                                uint32_t wait_usecs );

scan_err_t scos_mq_init( struct scos_mq *msgq, const char *name, int queue_size );

void scos_mq_free( struct scos_mq *msgq );

uint32_t scos_mq_num_pending( struct scos_mq *msgq );

/* davep 31-Aug-2012 ; adding wrapper around semaphore */
scan_err_t scos_sem_wait( struct scos_sem *sem );
scan_err_t scos_sem_trywait( struct scos_sem *sem );
scan_err_t scos_sem_post( struct scos_sem *sem );
scan_err_t scos_sem_init( struct scos_sem *sem, const char *name, int initial_value );
scan_err_t scos_sem_getvalue( struct scos_sem *sem, int *value );
scan_err_t scos_sem_timedwait( struct scos_sem *sem, struct timespec *abs_timeout );
scan_err_t scos_sem_is_initialized( struct scos_sem *sem );

#endif

