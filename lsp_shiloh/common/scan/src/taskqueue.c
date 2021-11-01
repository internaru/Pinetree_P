/**
 * \file taskqueue.c
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

#include <string.h>

#include "scos.h"

#include "list.h"
#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "ostools.h"
#include "taskqueue.h"
#include "scantools.h"

//#define TASK_QUEUE_DEBUG 1

#ifndef TASK_QUEUE_DEBUG
    #define task_queue_sanity()
#endif

#define MAX_NUM_REQUESTS 10

#define TASK_QUEUE_SEMAPHORE_NAME "/scantq"
#define TASK_QUEUE_COUNTING_SEMAPHORE_NAME "/scantq123"

/* event flags */
#define TASK_QUEUE_EVENT_TASK_READY 1

/* memory corruption checker */
#define SCAN_TASK_REQUEST_COOKIE 0xADB7305F

struct scan_task_request {
    uint32_t cookie;

    ATLISTENTRY listnode;
    uint32_t id;

    scan_msg_t msg;
};

struct scan_task_queue {
    /* lists of struct scan_task_request */
    ATLISTENTRY free_list; 
    ATLISTENTRY ready_list;

    /* count of elements in the lists */
    int num_free;
    int num_ready;

    /* we can only run one request at a time */
    struct scan_task_request *running;

    /* total number of scan_task_request structures we control */
    int max_request_count;

    /* task requests have an id so they can be canceled if necessary */
    uint32_t next_task_id;

    /* davep 07-Sep-2012 ; switch to a data structure lock and a counting
     * semaphore.
     */
    struct scos_sem lock;
    struct scos_sem counting_sem;
};

/* space used in the task queue */
static struct scan_task_request request_list[MAX_NUM_REQUESTS];

/* data struture to manage the task queue */
static struct scan_task_queue task_queue;

#ifdef TASK_QUEUE_DEBUG
static void task_queue_sanity( void )
{
    int cnt;

    cnt = count_list( &task_queue.ready_list );
    XASSERT( cnt==task_queue.num_ready, cnt );

    cnt = count_list( &task_queue.free_list );
    XASSERT( cnt==task_queue.num_free, cnt );

    cnt = task_queue.num_free + task_queue.num_ready;
    if( task_queue.running ) {
        /* +1 for the running request */
        XASSERT( cnt+1==task_queue.max_request_count, cnt+1 );
    }
    else {
        XASSERT( cnt==task_queue.max_request_count, cnt );
    }
}
#endif

static void semaphore_init( void )
{
    scan_err_t scerr;

    scerr = scos_sem_init( &task_queue.lock, TASK_QUEUE_SEMAPHORE_NAME, 1);
    XASSERT( scerr==SCANERR_NONE, scerr ); 

    scerr = scos_sem_init( &task_queue.counting_sem, TASK_QUEUE_COUNTING_SEMAPHORE_NAME, 0);
    XASSERT( scerr==SCANERR_NONE, scerr ); 
}

static scan_err_t task_queue_lock_nowait( void )
{
    scan_err_t scerr;

    scerr = scos_sem_trywait( &task_queue.lock );
    return scerr;
}

static void task_queue_lock( void )
{
    scan_err_t scerr;
    struct timespec abs_timeout;

    /* no one should hold this lock for very long */
    clock_gettime( CLOCK_REALTIME, &abs_timeout );

    /* wait 1 second */
    calc_future_usec( &abs_timeout, 1*USEC_PER_SECOND );

    scerr = scos_sem_timedwait( &task_queue.lock, &abs_timeout );
//    scerr = scos_sem_wait( &task_queue.lock );
    XASSERT( scerr==SCANERR_NONE, scerr ); 
}

static void task_queue_unlock( void )
{
    scan_err_t scerr;

    scerr = scos_sem_post( &task_queue.lock );
    XASSERT( scerr==SCANERR_NONE, scerr ); 
}

static void task_queue_signal_raise( void )
{
    scan_err_t scerr;

    scerr = scos_sem_post( &task_queue.counting_sem );
    XASSERT( scerr==SCANERR_NONE, scerr ); 
}

static void task_queue_signal_wait( void )
{
    scan_err_t scerr;

    scerr = scos_sem_wait( &task_queue.counting_sem );
    XASSERT( scerr==SCANERR_NONE, scerr ); 
}

static void task_request_init( struct scan_task_request *req )
{
    memset( req, 0, sizeof(struct scan_task_request) );

    /* cookie is for RTTI / memory corruption checking */
    req->cookie = SCAN_TASK_REQUEST_COOKIE;
}

static scan_err_t task_queue_init( void )
{
    int i;
    struct scan_task_request *request;

    memset( &task_queue, 0, sizeof(struct scan_task_queue) );

    ATInitList( &task_queue.free_list );
    ATInitList( &task_queue.ready_list );

    for( i=0 ; i<MAX_NUM_REQUESTS ; i++ ) {
        request = &request_list[i];

        task_request_init( request );

        /* put it on the free list */
        ATInsertTailList( &task_queue.free_list, &request->listnode );
        task_queue.num_free += 1;
    }

    /* for leak checking, dup checking */
    task_queue.max_request_count = MAX_NUM_REQUESTS;

    semaphore_init();

    return SCANERR_NONE;
}

static struct scan_task_request *task_queue_find_id( ATLISTENTRY *list, uint32_t id )
{
    ATLISTENTRY *curr, *first, *last;
    struct scan_task_request *request;

    /* 
     *  Assumes list(s) locked by caller! 
     */

    first = list->nextEntry;
    last = list;

    if( first==last ) {
        /* list is empty; don't bother searching */
        return NULL;
    }

    curr = first;
    while( curr != last ) {
        request = CONTAINING_RECORD( curr, struct scan_task_request, listnode );
        if( request->id == id ) {
            return request;
        }

        /* Nope, not found. Try the next one */
        curr = curr->nextEntry;

        /* Note: at this point curr might not be pointing into an
         * struct scan_task_request. Our lists are cirularly linked but the
         * root of the list is a bare ATLISTENTRY pointer, not contained within
         * a structure.
         */
    }

    return NULL;
}

static struct scan_task_request *task_queue_remove_id( ATLISTENTRY *list, uint32_t id )
{
    struct scan_task_request *request;

    /* 
     *  Assumes list(s) locked by caller! 
     */

    request = task_queue_find_id( list, id );
    if( request == NULL ) {
        return NULL;
    }

    ATRemoveEntryList( &request->listnode );
    request->listnode.nextEntry = NULL;
    request->listnode.prevEntry = NULL;
    return request;
}

static struct scan_task_request *task_queue_get_ready( void )
{
    ATLISTENTRY *entry;
    struct scan_task_request *request;

    /* 
     *  Assumes list(s) locked by caller! 
     */

    entry = ATRemoveHeadList( &task_queue.ready_list );
    if( entry==NULL ) {
        return NULL;
    }

    XASSERT( task_queue.num_ready > 0, task_queue.num_ready );
    task_queue.num_ready--;

    request = CONTAINING_RECORD( entry, struct scan_task_request, listnode );
    XASSERT( request->cookie==SCAN_TASK_REQUEST_COOKIE, request->cookie );

    return request;
}

static scan_err_t add_task( scan_msg_t *msg, uint32_t *id )
{
    ATLISTENTRY *entry;
    struct scan_task_request *request;

    /*
     *  BIG FAT NOTE!  
     *
     *  Can be called from interrupt/timer context!
     *
     *  (via scan_task_queue_add_no_wait())
     *
     */

    entry = ATRemoveHeadList( &task_queue.free_list );
    if( entry==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    XASSERT( task_queue.num_free>0, task_queue.num_free );
    task_queue.num_free -= 1;

    request = CONTAINING_RECORD( entry, struct scan_task_request, listnode );
    XASSERT( request->cookie==SCAN_TASK_REQUEST_COOKIE, request->cookie );

    request->id = ++task_queue.next_task_id;

    /* davep 27-Sep-2011 ; keep taskid==0 a reserved value (also handles
     * rollover) 
     */
    if( request->id==0 ) {
        request->id = ++task_queue.next_task_id;
    }

    *id = request->id;
    memcpy( &request->msg, msg, sizeof(scan_msg_t) );

    /* put it on the ready list */
    ATInsertTailList( &task_queue.ready_list, &request->listnode );
    task_queue.num_ready += 1;

    return SCANERR_NONE;
}

scan_err_t scan_task_queue_add_task_nowait( scan_msg_t *msg, uint32_t *id )
{
    scan_err_t scerr;

    scerr = task_queue_lock_nowait();
    if( scerr != SCANERR_NONE ) {
        /* If someone already holds the lock, we don't touch the list. The
         * caller will have to decide how to recover.
         */
        return scerr;
    }

    scerr = add_task( msg, id );

    task_queue_unlock();
        
    /* wake up any sleepers */
    task_queue_signal_raise();

    return scerr;
}

scan_err_t scan_task_queue_add_task( scan_msg_t *msg, uint32_t *id )
{
    scan_err_t final_scerr;

    task_queue_lock();

    final_scerr = add_task( msg, id );

    task_queue_unlock();
        
    /* wake up any sleepers */
    task_queue_signal_raise();

    dbg2( "%s scerr=%d id=%d\n", __FUNCTION__, final_scerr, *id );

    return final_scerr;
}

scan_err_t scan_task_queue_cancel_task( uint32_t id )
{
    struct scan_task_request *request;

    task_queue_lock();
    
    /* if we don't have anything ready or running, we know that request isn't
     * here so leave with an error
     */
    if( task_queue.num_ready==0 && task_queue.running==NULL ) {
        task_queue_unlock();
        return SCANERR_NO_ENTRY;
    }

    /* is it running? */
    if( task_queue.running && task_queue.running->id==id ) {
        /* yup, it's on the running list so we can't cancel it from here */
        task_queue_unlock();
        dbg2( "%s id=%d is busy\n", __FUNCTION__, id );
        return SCANERR_BUSY;
    }

    /* (maybe) remove from the ready list */
    request = task_queue_remove_id( &task_queue.ready_list, id );
    if( request == NULL  ) {
        /* nope, wasn't on the ready list. I have no record of that request */
        task_queue_unlock();
        dbg2( "%s id=%d is unknown\n", __FUNCTION__, id );
        return SCANERR_NO_ENTRY;
    }
    /* adjust the ready count */
    XASSERT( task_queue.num_ready > 0, task_queue.num_ready );
    task_queue.num_ready -= 1;

    /* stupid human check */
    XASSERT( request->id==id, request->id );

    /* Clean the request, put it back on the free list.  Note I'm
     * unconditionally stomping over whatever might be inside request->msg. 
     *
     * It's the caller's responsibility that any memory or whatever from the
     * request is properly freed.
     */
    /* clean it */
    task_request_init( request );

    ATInsertTailList( &task_queue.free_list, &request->listnode );
    task_queue.num_free += 1;

    task_queue_unlock();

    return SCANERR_NONE;
}

void scan_task_queue_get_task( scan_msg_t *msg, uint32_t *id )
{
    bool found_task;
    struct scan_task_request *request;

#ifdef TASK_QUEUE_DEBUG
    dbg2( "%s\n", __FUNCTION__ );
#endif

    memset( msg, 0, sizeof(scan_msg_t) );

    found_task = false;

    do {
        task_queue_lock();

        /* check for a request on the pending list */
        request = task_queue_get_ready();

        if( request != NULL ) {
#ifdef TASK_QUEUE_DEBUG
            dbg2( "%s found id=%d\n", __FUNCTION__, request->id );
#endif
            /* give caller a copy of the message */
            memcpy( msg, &request->msg, sizeof(scan_msg_t) );
            *id = request->id;
            found_task = true;
            /* move to running */
            PTR_ASSIGN( task_queue.running, request );
        }

        task_queue_unlock();

        /* block until we have something in the queue */
        if( !found_task ) {
            task_queue_signal_wait();
        }

    } while( !found_task );

}

scan_err_t scan_task_queue_release_task( uint32_t id )
{
    struct scan_task_request *request;

#ifdef TASK_QUEUE_DEBUG
    dbg2( "%s id=%d\n", __FUNCTION__, id );
#endif

    task_queue_lock();

    /* as of this writing (20-Sep-2009), we can only have one running task so
     * make sure the one we're trying to release is the one that's running.
     */
    XASSERT( task_queue.running->id==id, id );

    /* pop running, clean it, put it back onto free list */
    PTR_ASSIGN( request, task_queue.running );

    XASSERT( request->cookie==SCAN_TASK_REQUEST_COOKIE, request->cookie );

    /* clean it */
    task_request_init( request );

    XASSERT( request->cookie==SCAN_TASK_REQUEST_COOKIE, request->cookie );

    /* put it back where we can use it again later */
    ATInsertTailList( &task_queue.free_list, &request->listnode );
    task_queue.num_free += 1;

    task_queue_sanity();

    task_queue_unlock();

    return SCANERR_NONE;
}

int scan_task_queue_get_task_status( uint32_t id )
{
    struct scan_task_request *request;

    task_queue_lock();

    /* we're idle so that id has no meaning to us */
    if( task_queue.num_ready==0 && task_queue.running==NULL ) {
        task_queue_unlock();
        return TASK_QUEUE_NO_SUCH_TASK;
    }

    /* is it running? */
    if( task_queue.running && task_queue.running->id==id ) {
        /* yup, it's on the running list so we can't cancel it from here */
        task_queue_unlock();
        return TASK_QUEUE_TASK_IS_RUNNING;
    }

    /* is it on the ready list? */
    request = task_queue_find_id( &task_queue.ready_list, id );
    if( request != NULL ) {
        task_queue_unlock();
        /* found it! */
        return TASK_QUEUE_TASK_IS_PENDING;
    }

    task_queue_unlock();

    /* I know nothing! */
    return TASK_QUEUE_NO_SUCH_TASK;
}

void scan_task_queue_print( void )
{
    task_queue_lock();

    dbg1( "free=%d ready=%d running=%p ", task_queue.num_free, task_queue.num_ready, task_queue.running );
    dbg1( "free=%d ready=%d running=%p\n", count_list(&task_queue.free_list),
                count_list(&task_queue.ready_list), task_queue.running );

    task_queue_unlock();
}

scan_err_t scan_task_queue_onetime_init( void )
{
    scan_err_t scerr;

    scerr = task_queue_init();

    /* more? */

    return scerr;
}

#ifdef xxx_LINUX
void scan_task_queue_shutdown( void )
{
    int retcode;

    dbg2( "%s\n", __FUNCTION__ );

    task_queue_sanity();

    /* Don't bother with lock. Rest of system better know the task queue is
     * going away.
     */
    retcode = sem_close( task_queue.lock );
    XASSERT( retcode==0, retcode );

    retcode = sem_unlink( TASK_QUEUE_SEMAPHORE_NAME );
    XASSERT( retcode==0, retcode );

    retcode = pthread_cond_destroy( task_queue.wait_cond_ptr );
    XASSERT( retcode==0, retcode );

    retcode = pthread_mutex_destroy( task_queue.wait_mutex_ptr );
    XASSERT( retcode==0, retcode );

    scos_free_pthread_cond( &task_queue.wait_cond_ptr );
    scos_free_pthread_mutex( &task_queue.wait_mutex_ptr );
}
#endif

