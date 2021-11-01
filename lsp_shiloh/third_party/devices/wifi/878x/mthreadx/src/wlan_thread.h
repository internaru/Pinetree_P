/*	
 *	File: 	wlan_thread.h
 *
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2006
 */

#ifndef _KTHREAD_H_
#define _KTHREAD_H_

#include "list.h"
#include "tx_api.h"
//#include "MfpSystem.h"

#define KTHREAD_TIMESLICE   20
//#define KTHREAD_PRIORITY    THR_PRI_WIRELESS
#define KTHREAD_PRIORITY    THR_PRI_NORMAL
//#define KTHREAD_STACKSIZE   4096    // REVISIT: tune this value
#define KTHREAD_STACKSIZE   2*8192    // REVISIT: tune this value
#define KTHREAD_WAIT_FLAG   0x00000001

#define WLAN_THREAD_STOPPED	0
#define WLAN_THREAD_RUNNING	1

/* A structure to store all information we need for our thread. */
typedef struct
{
	/* private data */
	TX_THREAD txThread;
	TX_SEMAPHORE startstop_sem;
	TX_EVENT_FLAGS_GROUP wait_q;
	u32 state;
	void* priv;
	u8 threadStack[KTHREAD_STACKSIZE] ALIGN8;
} wlan_thread ALIGN8;

/* Create a new kernel thread. Called by the creator. */
static inline void wlan_create_thread(int (*func)(void*), wlan_thread *thread, 
								char* name)
{
    // create binary semaphore with inital count of 0 when the 
    // thread starts it will increment the semaphore after it 
    // initializes itself.  This is done to allow the new thread 
    // to run before the creating thread continues
    tx_semaphore_create(&thread->startstop_sem, name, 0);

    // initialize the thread's event flags
    tx_event_flags_create(&thread->wait_q, name);

    // create and start the thread
    tx_thread_create(&thread->txThread, name, (void*)func, (u32)thread, 
            thread->threadStack, KTHREAD_STACKSIZE, 
            KTHREAD_PRIORITY, KTHREAD_PRIORITY, 
            KTHREAD_TIMESLICE, TX_AUTO_START);

    // wait for it to start and init (thread will do a sem_put when it activates)
    tx_semaphore_get(&thread->startstop_sem, TX_WAIT_FOREVER);
}

/* Stop a kernel thread. Called by the removing instance. */
static inline void wlan_terminate_thread(wlan_thread *thread)
{
	thread->state = WLAN_THREAD_STOPPED;

	// wake the thread in case it's sleeping
	tx_event_flags_set(&thread->wait_q, KTHREAD_WAIT_FLAG, TX_OR);

	// wait for thread to die; semaphore will be 0 if thread has not 
	// exited because mrvl_create_thread routines leaves it at 0.
	tx_semaphore_get(&thread->startstop_sem, TX_WAIT_FOREVER);

	tx_thread_delete(&thread->txThread);
	tx_event_flags_delete(&thread->wait_q);
	tx_semaphore_delete(&thread->startstop_sem);
}

/* Initialize new created thread. Called by the new thread. */
static inline void wlan_activate_thread(wlan_thread *thread)	//init_kthread
{
	thread->state = WLAN_THREAD_RUNNING;
	tx_semaphore_put(&thread->startstop_sem);
}

/* Cleanup of thread. Called by the exiting thread. */
static inline void wlan_deactivate_thread(wlan_thread *thread)	//exit_kthread
{
    UINT oldPri;
    // boost thread priority to highest before putting the semaphore so that the
    // exiting thread runs to completeion
    tx_thread_priority_change(&thread->txThread, THR_PRI_HIGHEST, &oldPri);
	tx_semaphore_put(&thread->startstop_sem);
}

static inline wlan_thread* get_current_kthread()
{
    return NULL;
}

#endif /* _KTHREAD_H_ */

