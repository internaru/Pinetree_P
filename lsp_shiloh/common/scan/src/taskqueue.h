/**
 * \file taskqueue.h
 *
 * \brief Scan task task queue.
 *
 * A message queue with cancelable messages.
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef TASKQUEUE_H
#define TASKQUEUE_H

/* return values from scan_task_queue_get_task_status() */
#define TASK_QUEUE_NO_SUCH_TASK    1
#define TASK_QUEUE_TASK_IS_RUNNING 2
#define TASK_QUEUE_TASK_IS_PENDING 3

scan_err_t scan_task_queue_onetime_init( void );
void scan_task_queue_shutdown( void );
scan_err_t scan_task_queue_add_task( scan_msg_t *msg, uint32_t *id );
scan_err_t scan_task_queue_add_task_nowait( scan_msg_t *msg, uint32_t *id );
scan_err_t scan_task_queue_release_task( uint32_t id );
scan_err_t scan_task_queue_cancel_task( uint32_t id );
void scan_task_queue_get_task( scan_msg_t *msg, uint32_t *id );
int scan_task_queue_get_task_status( uint32_t id );
void scan_task_queue_print( void );

#endif

