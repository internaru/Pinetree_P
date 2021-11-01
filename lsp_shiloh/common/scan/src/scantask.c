/**
 * \file scantask.c
 *
 * \brief Scantask is the one and only one thread in the scanning subsystem.
 *
 *  About Scantask.
 *
 *  The scanning subsystem is essentially one thread. One and only one thread.
 *  Only one. Period. Scantask has been designed for testability, reliability,
 *  and simplicity. We can run scans and capture data without having the rest
 *  of the system working. We can do scans with nothing more than a running
 *  RTOS and a serial port. 
 *
 *  Scantask is the only code that talks to the hardware. We don't have to
 *  worry about stepping on someone's toes.
 *
 *  Scantask is the only code that talks to its data structures (with some
 *  exceptions that are, actually, bugs). We only have to worry about interrupt
 *  protection.
 *
 *  All the sensor control, motor control, yadda yadda yadda is all contained
 *  in one thread. 
 *
 *  Scantask will perform scans via messages.  See the SMSG_xxx in scantypes.h
 *
 */
/* 
 * ============================================================================
 * Copyright (c) 2006   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

#include "scos.h"
#ifdef __KERNEL__
#include "dros.h"
#endif

#include "list.h"           
#include "memAPI.h"  
#include "posix_ostools.h"
#ifdef __KERNEL__ 
    #include "scos_cmd_proc.h"
#else
    #include "cmd_proc_api.h"
#endif

#include "lassert.h"
#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanhwerr.h"
#include "scanvars.h"
#include "scantask.h"
#include "scan.h"
#include "scanlib.h"
#include "piehw.h"
#include "pie.h"
#include "pic.h"
#include "cal.h"
#include "scanpipe.h"
//#include "scanman.h"
#include "scanimg.h"
#include "ostools.h"
#include "scanmech.h"
#include "scancmdq.h"
#include "taskqueue.h"
#include "scands.h"
#include "scancap.h"
#ifdef HAVE_ICETEST
    #include "icetest.h"
#else
    #define icetest_onetime_init scan_success
#endif
#ifdef HAVE_CISX
    #include "cisx.h"
#else
    #define cisx_onetime_init scan_success
#endif
#include "smirb.h"
#include "scanpower.h"
#include "scanmem.h"
#include "cmdline.h"
#include "../../debug/include/logger.h"
/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "SCN TASK: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(6)
///Using Method.
#define DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)


#define SCANTASK_QUEUE_NAME "/scantask"

#define SCAN_TASK_PRI     THR_PRI_NORMAL

#ifdef PTHREAD_STACK_MIN
    /* pthreads will refuse a stack smaller than this size (#define'd in
     * limits.h)
     */
    #define THREAD_STACK_SIZE  (PTHREAD_STACK_MIN * 4)
#else
    #define THREAD_STACK_SIZE  8192 * 2
#endif

/* message queue for messages from the rest of the system to the scan task */
#if defined __KERNEL__
    #define SCANTASK_QUEUE_SIZE 256
#elif defined __linux__
    #define SCANTASK_QUEUE_SIZE 10
#else
    /* use the value from tx_posix.h */ 
    #define SCANTASK_QUEUE_SIZE MQ_MAXMSG
#endif

#if defined __KERNEL__
static dros_thread_t* scan_task_id;
#else
static bool use_scanman_sdh;
static scan_callback_fn_t old_callback_fn_sdh;
static uint8_t scan_stack[THREAD_STACK_SIZE] __attribute__ ((aligned (8)));
static pthread_t scan_task_id; 
#endif
static struct scos_mq scantask_msgq;
static struct scos_sem scantask_global_lock;
static scan_callback_fn_t scanman_callback_fn;

typedef scan_err_t (*scan_init_function)(void);

#define MLIMITER_SCAN_MEMORY 72*1024*1024
mlimiter_t *scan_heap;
    
static scan_err_t scan_success( void )
{
    /* stub function to put into init list, substitute for subsystems that
     * don't exist
     */
    return SCANERR_NONE;
}

/* davep 13-Jun-2006 ; initialize a small subset of the system that will
 * allow us to test scanning without a sensor/afe/motor mech (for ICE
 * testing, etc). 
 *
 * Please do not delete this code! I'm checking it in because it's useful
 * to be able to run ICE without having a lot of external hardware.
 */
static const scan_init_function short_init_list[] = {
    scanmem_onetime_init,
    pipe_onetime_init,
    icetest_onetime_init,
    cisx_onetime_init,
    pic_onetime_init,
    pie_onetime_init,
    scan_success, /* added here to avoid "called by not used" warning */
    NULL, /* end of list */
};
    
static const scan_init_function full_init_list[] = {
    scanmem_onetime_init,
    scan_onetime_init,
    scanimg_onetime_init,
    pipe_onetime_init,
    icetest_onetime_init,
    cisx_onetime_init,
    pic_onetime_init,
    pie_onetime_init,
    cal_onetime_init,
    scanpower_onetime_init,
    //scanmech_selftest,    //mod.kks move to the scan man done after.
    NULL, /* end of list */
};

struct scos_mq * scantask_get_msgq( void )
{
    return &scantask_msgq;
}

int scantask_get_queue_pending( void )
{
    /* TODO */
    ASSERT(0);
    return 0;
//    return get_queue_pending( &scantask_msgq );
}

pthread_t scantask_get_id( void )
{
    #ifdef __KERNEL__
    return scan_task_id->task;
    #else
    return scan_task_id;
    #endif
}

static void scantask_lock( void )
{
    scan_err_t scerr;

    /* davep 08-Aug-2009 ; note I'm doing a blocking lock here. The lock is
     * shared with scantask_lock(), scantask_unlock(), and scantask_is_busy().
     *
     * If scantask_is_busy() captures the lock, he immediately frees it. So
     * scantask shouldn't get blocked on the semaphore for too long. 
     *
     * scantask_is_busy() must use trylock because he can be called from timer
     * context. And trylock prevents scantask from accidentally deadlocking.
     */

    scerr = scos_sem_wait( &scantask_global_lock );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void scantask_unlock( void )
{
    scan_err_t scerr;

    scerr = scos_sem_post( &scantask_global_lock );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

/**
 * \brief  Check if Scantask is running
 *
 * Please note this is not a mutual exclusion tool.  Two (or more) messages
 * could be queued for scantask before the scantask thread runs. 
 *
 * Algorithm looks like:
 *
 * while 1 :
 *     unlock_scantask  (scantask is not busy)
 *     wait for messages
 *     lock_scantask    (scantask is busy)
 *     process message
 *
 * \author David Poole
 * \date 2009
 *
 */

bool scantask_is_busy( void )
{
    scan_err_t scerr;
    int semval;

    /* davep 16-Nov-2010 ; use sem_getvalue() instead of sem_trylock() */
    scerr = scos_sem_getvalue( &scantask_global_lock, &semval );
    XASSERT( scerr==SCANERR_NONE, scerr );

    return semval<=0;
}

scan_err_t scantask_msg_send_nowait( scan_msg_t *msg )
{
    return scos_mq_send_nowait( &scantask_msgq, msg );
}

scan_err_t scantask_msg_send( scan_msg_t *msg, uint32_t wait_usecs )
{
    return scos_mq_send( &scantask_msgq, msg, wait_usecs );
}

static scan_callback_fn_t scantask_set_callback( scan_callback_fn_t new_callback )
{
    scan_callback_fn_t old_callback;

    /* If you compare scantask_set_callback() to scanlib_set_callback(), you'll
     * notice scanlib won't attempt to preserve the old callback. Scantask will
     * be given a "default" message callback on startup for success/failure
     * messages. We want to preserve that callback for the life of Scantask so
     * we can continue to inform the outside world of what we're doing.
     */
    old_callback = scanman_callback_fn;
    scanman_callback_fn = new_callback;

//    DBG_PRINTF_SDH_DEBUG( "%s old=%p new=%p\n", __FUNCTION__, old_callback, new_callback );

    return old_callback;
}

scan_err_t scantask_msg_callback( scan_msg_t *msg )
{
    ASSERT( scanman_callback_fn!=NULL );
    return scanman_callback_fn( msg );
}

scan_err_t scantask_cancel_msg( void )
{
    scan_err_t scerr;
    scan_msg_t msg;

    memset( &msg, 0, sizeof(scan_msg_t) );
    msg.msgType = SMSG_SCAN_CANCEL;

    scerr = scantask_msg_send( &msg, SCOS_WAIT_FOREVER );

	DBG_PRINTF_SDH_DEBUG( "%s scerr=%d\n", __FUNCTION__, scerr );

    return scerr;
}

scan_err_t scantask_scan_cancel( uint32_t task_id, scan_task_status_t *task_status )
{
    scan_err_t scerr;

    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    scerr = scan_task_queue_cancel_task( task_id );

    if( scerr==SCANERR_BUSY ) {
        /* The task is currently in-flight so we'll have to send a cancel
         * message to Scantask asking for an active cancel.
         */
        *task_status = SCAN_TASK_STATUS_RUNNING;
        scantask_cancel_msg();
        return SCANERR_NONE;
    }

    /* at this point, the task is no longer in the task queue */

    if( scerr==SCANERR_NONE ) {
        /* The task was pending so send back the status saying it had been
         * pending before we canceled it. (The pending is useful so we have
         * states between 'running' and 'no such task'.)
         */
        *task_status = SCAN_TASK_STATUS_PENDING;
    }
    else if( scerr==SCANERR_NO_ENTRY ) {
        /* what you talking about, willis? */
        *task_status = SCAN_TASK_STATUS_NO_SUCH_TASK;
    }
    else {
        /* wtf? */
        XASSERT( 0, scerr );
        scerr = SCANERR_NO_ENTRY;
        *task_status = SCAN_TASK_STATUS_NO_SUCH_TASK;
    }

    DBG_PRINTF_SDH_DEBUG( "%s task_id=%d scerr=%d\n", __FUNCTION__, task_id, scerr );

    return scerr;
}

scan_task_status_t scantask_get_task_status( uint32_t task_id )
{
    int task_status;

    task_status = scan_task_queue_get_task_status( task_id );

    /* outside world doesn't know we use taskqueue.c/taskqueue.h underneath so
     * we have to translate the taskqueue status to our scantask status codes.
     */
    switch( task_status ) {
        case TASK_QUEUE_NO_SUCH_TASK :
            return SCAN_TASK_STATUS_NO_SUCH_TASK;
            break;

        case TASK_QUEUE_TASK_IS_RUNNING :
            return SCAN_TASK_STATUS_RUNNING;
            break;

        case TASK_QUEUE_TASK_IS_PENDING :
            return SCAN_TASK_STATUS_PENDING;
            break;

        default :
            /* wha? */
            XASSERT( 0, task_status );
    }

    /* should never get here */
    return SCAN_TASK_STATUS_NO_SUCH_TASK;
}

/**
 * \brief  send a scandbg test/debug message to scantask
 *
 * \author David Poole
 * \date 23-Jul-2009
 *
 */

scan_err_t scantask_debug_msg_send( scan_msg_t *scan_dbg_msg )
{
    uint32_t task_id;

    return scan_task_queue_add_task( scan_dbg_msg, &task_id );
}

scan_err_t scantask_scan_start( const struct scanvars *sv, uint32_t *task_id )
{
    scan_err_t scerr;
    scan_msg_t send_msg;

    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    memset( &send_msg, 0, sizeof(scan_msg_t) );
    send_msg.msgType = SMSG_SCAN_START;
    send_msg.param3 = (void *)sv;

    scerr = scan_task_queue_add_task( &send_msg, task_id );
    if( scerr != 0 ) {
        DBG_PRINTF_SDH_DEBUG( "%s failed to queue msg=%d\n", __FUNCTION__, send_msg.msgType );
        return scerr;
    }

    return SCANERR_NONE;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 27-Sep-2011
 *
 */

scan_err_t scantask_ioctl_request( struct scanman_ioctl_request_block *smirb, uint32_t *task_id )
{
    scan_msg_t send_msg;
    scan_err_t scerr;

    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType = SMSG_SCAN_MECH_IOCTL;
    send_msg.param3 = (void *)smirb;

    scerr = scan_task_queue_add_task( &send_msg, task_id );
    if( scerr != 0 ) {
        DBG_PRINTF_SDH_DEBUG( "%s failed to queue msg=%d\n", __FUNCTION__, send_msg.msgType );
        return scerr;
    }

    return scerr;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 27-Sep-2011
 *
 */

static scan_err_t handle_smech_ioctl_message( scan_msg_t *msg )
{
    scan_err_t ioctl_scerr, scerr;
    scan_msg_t return_msg;
    struct scanman_ioctl_request_block *smirb;

    smirb = msg->param3;
    scanman_smirb_sanity( smirb );

    DBG_PRINTF_SDH_DEBUG( "%s smech ioctl request req=%d arg=%d ptr=%p id=%d\n", 
                __FUNCTION__, smirb->request, smirb->arg, smirb->ptr, smirb->id );

    ioctl_scerr = smech_ioctl( smirb->request, smirb->arg, smirb->ptr );
    if( ioctl_scerr != SCANERR_NONE ) {
        DBG_PRINTF_SDH_DEBUG( "%s req=%d arg=%d ptr=%p failed scerr=%d\n", __FUNCTION__, 
                smirb->request, smirb->arg, smirb->ptr, ioctl_scerr );
    }

    /* note I'm not returning the ioctl return code from this function. The
     * success/failure of the ioctl is returned through the smirb.
     */
    smirb->scerr = ioctl_scerr;

    /* Build and send a response to scanman. Scan man will call the observers
     * notifying them of the response to the ioctl.
     */
    memset( &return_msg, 0, sizeof(scan_msg_t));
    return_msg.msgType = SMSG_SCAN_MECH_IOCTL;
    return_msg.param3 = (void *)smirb;

    scerr = scantask_msg_callback( &return_msg );

    return scerr;
}


scan_err_t scantask_smech_poll( void )
{
    scan_msg_t send_msg;
    scan_err_t scerr;
    uint32_t id;

    /* Poll is called from timer context and so use the no_wait add task. 
     * If we fail to send the message, we'll try again later.
         */
    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType = SMSG_SMECH_POLL;

    scerr = scan_task_queue_add_task_nowait( &send_msg, &id );
    XASSERT( scerr==SCANERR_NONE||scerr==SCANERR_BUSY, scerr );

    return scerr;
}

static scan_err_t power_msg( uint32_t msgType )
{
    scan_msg_t send_msg;
    scan_err_t scerr;
    uint32_t task_id;

    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType = msgType;

    scerr = scan_task_queue_add_task( &send_msg, &task_id );
    XASSERT( scerr==SCANERR_NONE, scerr );
        
    return scerr;
}

scan_err_t scantask_power_on( void )
{
    return power_msg( SMSG_SCAN_MECH_PWR_ON );
}

scan_err_t scantask_power_off( void )
{
    return power_msg( SMSG_SCAN_MECH_PWR_OFF );
}

/**
 * \brief send a message to scanman informing him of our failure to scan
 *
 *  If we are unable to properly finish a scan (e.g., ADF paper jam or the user
 *  canceled while we were scanning), we inform scanman of our shame and beg his
 *  forgiveness for our abject failure.
 *
 *  (Consolidated some cut/paste code into this function.)
 *
 * \author David Poole
 * \date 15-Sep-2007
 *
 * davep 17-Jul-2008 ; added sending the scan_err_t along with the message
 *
 */

static void
send_failure_to_scanman( scan_err_t scerr )
{
    scan_msg_t send_msg;
    scan_err_t msg_send_scerr;

    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType= SMSG_SCAN_FAILED;
    send_msg.param1 = scerr;

    msg_send_scerr = scantask_msg_callback( &send_msg );
    XASSERT( msg_send_scerr==SCANERR_NONE, msg_send_scerr );
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 11-Nov-2008
 *
 */

void scantask_scan_end( struct scanvars **sv )
{
    scan_msg_t send_msg;
    scan_err_t scerr;

    /* If no message callback is set, we can't send the pointer back. Assume the
     * other side (that sent the SMSG_SCAN_START) knew what they were doing when
     * they didn't set a message callback.
     *
     * Most commonly no message callback is during a "scan var new"/"scan start"
     * from the command line.
     */
    if( (*sv)->scan_callback_fn ) {

        memset( &send_msg, 0, sizeof(send_msg) );
        /* davep 07-Jun-2009 ; return the scanvar in a new message sent after
         * everything is complete (can't use pageend anymore since multiples can
         * flow when doing an ADF scan)
         */
        send_msg.msgType = SMSG_SCAN_END;
        PTR_ASSIGN( send_msg.param3, *sv ); 

        scerr = scantask_msg_callback( &send_msg );
        XASSERT( scerr==SCANERR_NONE, scerr );
    }
}

/**
 * \brief  Send the SMSG_SCAN_READY
 *
 *
 * \author David Poole
 * \date 14-Nov-2008
 *
 */

static void scantask_ready( void )
{
    scan_msg_t send_msg;
    scan_err_t scerr;
    const struct scanvars *sv;

    /* if we're sending the SMSG_SCAN_READY message, we better be completely
     * cleaned up 
     */
    sv = scanvar_peek();
    XASSERT( sv==NULL, sv->id );

    /* Note I can't check if message callback function is set (because the
     * scanvar stack is empty). So I'll just force the decision back up to the
     * caller.
     *
     * Note this will cause problems (probably an assertion failure in Scanman)
     * if we're doing an internal scan and we send the SMSG_SCAN_READY.
     */
    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType = SMSG_SCAN_READY;

    scerr = scantask_msg_callback( &send_msg );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

/**
 * \brief Verify the scantask_msgq is empty 
 *
 * Called when we leave our idle message loop on receiving a task request.
 * 
 * As of this writing, the scantask_msgq is now protected. Only scantask can
 * write to scantask_msgq (mostly; see cancel below).  See also Bug 13728.
 *
 * (davep 21-Sep-2009) Scanask now sleeps on a task queue (see taskqueue.c)
 * instead of the old voicemail msgq. Same priciple applies. We could have stray
 * crap in our scantask_msgq when we wake up from the task queue.
 *
 * Note this is not what could be considered a "clean" solution but, hey, here
 * we are.
 *
 * \author David Poole
 * \date 10-Aug-2009
 *
 */

static void clean_scantask_msgq( void )
{
    scan_msg_t scan_msg;
    scan_err_t scerr;

    while( 1 ) {
        /* zero means don't wait */
        scerr = scos_mq_receive( &scantask_msgq, &scan_msg, 0 );
        if( scerr==SCANERR_TIMEDOUT ) {
            /* our scantask msgq is now empty and valid */
            break;
        }
        /* only a timeout error is acceptable at this point */
        XASSERT( scerr==SCANERR_NONE, scerr );

        /* only message we should ever see when we come out of our idle loop is
         * cancel (because scantask_cancel() always writes to our scantask_msgq)
         */
        switch( scan_msg.msgType ) {
            case SMSG_SCAN_CANCEL :
				DBG_PRINTF_SDH_DEBUG( "%s : SMSG_SCAN_CANCEL\n", __FUNCTION__ );
                break;

            case SMSG_SCAN_ADF_PAPER_EVENT :
                /* davep 27-Aug-2009 ; XXX cheap hack; when we come out of power
                 * save, the GPIO throws a paper path interrupt.
                 *
                 * garbage disposal cares not about adf paper 
                 */
                DBG_PRINTF_SDH_DEBUG( "%s ignore adf paper event=%d value=%d\n", 
                            __FUNCTION__, (uint32_t)scan_msg.param1, (uint32_t)scan_msg.param2 );
                break;

            case SMSG_SMECH_POLL :
                /* davep 21-Sep-2009 ; adding mech poll because I'm tired of
                 * fighting the %@#$@ing thing
                 */
                /* burtp 10-Nov-2009 ; If we just eat the message then the mech poll may not
                 *  get scheduled to run again (the poll routine sets up the next poll timer).
                 *  That could be bad ... so don't just ignore it here.
                 */
 				DBG_PRINTF_SDH_DEBUG( "%s : smech_poll started.", __FUNCTION__ );
                scerr = smech_poll();
 				DBG_PRINTF_SDH_DEBUG( "%s : smech_poll end.", __FUNCTION__ );
                if( scerr != 0 ) {
                    DBG_PRINTF_SDH_DEBUG( "%s mech poll failed! scerr=%d\n", __FUNCTION__, scerr );
                }
                break;

            default : 
                DBG_PRINTF_SDH_DEBUG("%s unknown message %#x param1=%#lx param2=%#lx param3=%p\n",
                      __FUNCTION__, scan_msg.msgType, scan_msg.param1, 
                      scan_msg.param2, scan_msg.param3 );

                XASSERT( 0, scan_msg.msgType );
                break;
        }

        /* go back and read some more */
    }
}

/**************************************************
 * Function name   :  scan_dispatch_message
 *    returns      :  none
 *    arg1         :  msg - pointer to received message
 * Created by      :  David Poole
 * Date created    :  13-Feb-05
 * Description     : 
 *
 *  Decode and do "something" with messages sent to the Scan task.
 *
 * Notes           : 
 *  Must be called from scan_task() context.
 **************************************************/

static void scan_dispatch_message( scan_msg_t *recv_scan_msg )
{
    scan_err_t scerr;
    struct scanvars *sv;
    bool use_scanman;
    scan_callback_fn_t old_callback_fn;
    uint32_t scanner_state;

    switch( recv_scan_msg->msgType ) {

        case SMSG_SCAN_DEBUG :
            scan_do_debug_msg( recv_scan_msg->param1, 
                               recv_scan_msg->param2, 
                               recv_scan_msg->param3 );
            break;

        case SMSG_SCAN_MECH_PWR_ON :
            // Power on the Scan top. Start Polling ADF
            scerr = scanpower_up();
            XASSERT(scerr == SCANERR_NONE, scerr);

            /* davep 19-Dec-2011 ; set state back to normal so we're open for
             * business
             */
            scerr = scands_set_integer( "scanner_state", SCANNER_IS_IDLE );
            XASSERT( scerr==SCANERR_NONE, scerr );
            break;

        case SMSG_SCAN_MECH_PWR_OFF :
            /* davep 19-Dec-2011 ; set so we can block certain requests while
             * if we're asleep (safety check)
             */
            scerr = scands_set_integer( "scanner_state", SCANNER_IS_ASLEEP );
            XASSERT( scerr==SCANERR_NONE, scerr );

            // Power off the Scan top. 
            scerr = scanpower_down();
            XASSERT(scerr == SCANERR_NONE, scerr);
            break;
   
        case SMSG_SCAN_START :

			dbg1( "[S_T] Step1. SMSG_SCAN_START\n");
			
            old_callback_fn = NULL;
#ifndef __KERNEL__
			old_callback_fn_sdh = NULL;
			use_scanman_sdh = false;
#endif
            /* davep 11-Nov-2008 ; Bug 11161 ; now must get scanvar via start
             * message
             */
            ASSERT( recv_scan_msg->param3 != NULL );
            PTR_ASSIGN( sv, recv_scan_msg->param3 );
            XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
            
            /* control of sv will be passed to scanlib (pointer will be NULL
             * later) so save flag and callback ptr for later use
             */
            use_scanman = false;
            if( sv->scan_callback_fn != NULL ) {
                use_scanman = true;
                old_callback_fn = scantask_set_callback( sv->scan_callback_fn );
#ifndef __KERNEL__
                use_scanman_sdh = true;
                old_callback_fn_sdh = old_callback_fn;
#endif
            }

            /* davep 19-Dec-2011 ; check if we're powered down (sanity check) */
            scerr = scands_get_integer( "scanner_state", &scanner_state );
            XASSERT( scerr==SCANERR_NONE, scerr );
            if( scanner_state==SCANNER_IS_ASLEEP ) {
                DBG_PRINTF_SDH_DEBUG( "%s scanner is in power save mode; cannot scan\n", __FUNCTION__ );
                if( use_scanman ) {
                    /* must send the ready even if we didn't scan so scanman
                     * knows we can accept another scan request
                     */
                    send_failure_to_scanman( SCANERR_NOT_PERMITTED );
                    scantask_ready();
                    scantask_set_callback( old_callback_fn );
                }
                break;
            }

            /* davep 15-Feb-2011 ; set fake-o state so driver apps are happy */
            scerr = scands_set_integer( "scanner_state", SCANNER_IS_RUNNING );
            XASSERT( scerr==SCANERR_NONE, scerr );

            scanpower_scanning_is_active( true );

            /* Everything the scan might depend on should be in the scanvar.
             *
             * After the call to scanlib2_setup_and_run(), sv might be NULL
             * because the scanvar is no longer scantask.c's responsibility;
             * scanlib needs to decide what to do with it.
             */
            scerr = scanlib2_setup_and_run( &sv );

            if( scerr != 0 ) {
                DBG_PRINTF_SDH_DEBUG( "%s scan run failed with scerr=%d\n", __FUNCTION__, scerr );

                /* davep 5-Dec-2005 ; The scan failed to start for some reason.
                 * Usual reason could be a cancel during calibration or a
                 * hardware failure during calibration. 
                 *
                 * If the scanvars was setup by scanman (which implies the
                 * SCAN_START message came from scanman), we need to tell
                 * scanman we failed so he knows not to expect any data from
                 * us.
                 */
                if( use_scanman ) {  //If the mech state change to SCANERR_SCAN_CANCELLED state, send the SMSG_SCAN_FAILED message to APP level  in original marvell code.  mod.kks memo.
                    send_failure_to_scanman( scerr );
                }
            }
#ifndef __KERNEL__
///			Blocked by SINDOH yhlee	
//        mlimiter_start( scan_heap );
//        DBG_PRINTF_SDH_DEBUG( "%s mlimiter_start\n", __FUNCTION__ );
#endif
#ifdef __KERNEL__
            /* davep 11-Nov-2008 ; send SMSG_SCAN_READY; note I'm sending the
             * 'ready' even if the scan failed. We need to tell ScanMan we're
             * available for another scan.
             */
            if( use_scanman ) {
                scantask_ready();
                scantask_set_callback( old_callback_fn );
            }

            scanpower_scanning_is_active( false);

            /* davep 15-Feb-2011 ; set fake-o state so driver apps are happy */
            scerr = scands_set_integer( "scanner_state", SCANNER_IS_IDLE );
            XASSERT( scerr==SCANERR_NONE, scerr );
            
            dbg1( "[S_T] Step8. scan completed()\n");
#endif
            break;

        case SMSG_SCAN_CANCEL :
            /* davep 18-Dec-2008 ; ignore cancel while in idle state (could have
             * a stray cancel from mech polling) (fix from OwenZ)
             */
            DBG_PRINTF_SDH_DEBUG( "%s ignoring cancel in idle state\n", __FUNCTION__ );
            break;

        case SMSG_SMECH_POLL :
            /* davep 23-Dec-2011 ; check if we're powered down (sanity check) */
            scerr = scands_get_integer( "scanner_state", &scanner_state );
            XASSERT( scerr==SCANERR_NONE, scerr );
            if( scanner_state==SCANNER_IS_ASLEEP ) {
                DBG_PRINTF_SDH_DEBUG( "%s poll ignored; scanner is in power save mode\n", __FUNCTION__ );
                break;
            }

            /* davep 21-Oct-2008 ; adding support for scan mech polling */
            scerr = smech_poll();
            if( scerr != 0 ) {
                DBG_PRINTF_SDH_DEBUG( "%s mech poll failed! scerr=%d\n", __FUNCTION__, scerr );
            }
            break;

        case SMSG_CAL_RUN_CALIBRATION :
            /* davep 11-Nov-2008 ; TODO this code is untested with the most
             * recent BSDK changes ; don't use yet 
             */
            ASSERT( 0 );
#if 0
            /* force the cal-only debugging flag enabled */
            DBG_PRINTF_SDH_DEBUG( "setting scan cal only debug flag\n" );
            num32 = scan_get_config();
            scan_set_config( num32 | SCAN_CONFIG_CAL_ONLY );

            scerr = scanlib2_setup_and_run(); 
            if( retval != 0 ) {
                DBG_PRINTF_SDH_DEBUG( "internal calibration failed! scerr=%d\n", scerr );
            }

            /* put the debugging flags back */
            DBG_PRINTF_SDH_DEBUG( "restoring scan config flags\n" );
            scan_set_config( num32 );
#endif
            break;

        case SMSG_SCAN_MECH_STATUS:
            /* davep 12-Dec-2008 ; XXX starting to see this coming with the new
             * mech code.  Not sure what to do with it so do the usual
             * for now.
             */
            DBG_PRINTF_SDH_DEBUG( "%s mech status message in idle state, mech_status=%d\n",
                        __FUNCTION__, recv_scan_msg->param1 );
            scan_cmdq_mech_status_change((t_ScanMechStatus)recv_scan_msg->param1); 
            break;

        case SMSG_SCAN_MECH_IOCTL : 
            /* davep 27-Sep-2011 ; adding free fire zone ioctl */
            scerr = handle_smech_ioctl_message( recv_scan_msg );
            /* ignore error */
            break;

        default :
            /* epic fail! */
            DBG_PRINTF_SDH_DEBUG("%s unknown message %#x param1=%#lx param2=%#lx param3=%p\n",
                  __FUNCTION__,
                  recv_scan_msg->msgType, recv_scan_msg->param1, 
                  recv_scan_msg->param2, recv_scan_msg->param3 );
            XASSERT( 0, recv_scan_msg->msgType );
            break;
    }

    /* davep 13-Nov-2008 ; Bug 11161 ; stack must be clean at this point */
    XASSERT( scanvar_peek()==NULL, scanvar_peek()->id );

    /* aaaaaaand we're done */
}

/** 
 * \brief deal with scan hardware failure. 
 *
 * Send a msg to indicate the failure and then drop into a simplified message
 * loop.
 *
 * Continue in a message loop instead of quitting so we can handle debug/test
 * messages.  
 * 
 */

static void scan_hw_failure( scan_err_t scerr )
{
    scan_msg_t scan_msg;
    uint32_t task_id;
    ScannerHWError scanner_hw_err;

    if( scerr==SCANERR_HARDWARE_FAIL ) {
        scanner_hw_err = scan_get_hwerr();
    }
    else {
        scanner_hw_err = ScannerHWError_GeneralFailure;
    }

    DBG_PRINTF_SDH_DEBUG( "SCAN: scanner init failed! hwerr=%d\n", scanner_hw_err );

    /* shouldn't get in here unless we have a real error */
    XASSERT( scanner_hw_err != ScannerHWError_None, scanner_hw_err ); 

    scan_set_hwerr( scanner_hw_err );

    /* tell scanman we're hosed */
    memset( &scan_msg, 0, sizeof(scan_msg_t) );
    scan_msg.msgType = SMSG_SCAN_INIT_DONE;
    scan_msg.param1 = scanner_hw_err;
    scerr = scantask_msg_callback( &scan_msg );
    XASSERT( scerr==SCANERR_NONE, scerr );

    while(1)
    { 
        /* wait forever for a message */
        scan_task_queue_get_task( &scan_msg, &task_id );

        /* davep 25-Jul-2006 ; handle scan debug messages so we can peek at
         * hardware while we're in a dead state
         */
        if( scan_msg.msgType == SMSG_SCAN_DEBUG ) {
             scan_do_debug_msg( scan_msg.param1, scan_msg.param2, scan_msg.param3 );
        }
        else {
            DBG_PRINTF_SDH_DEBUG( "scanner offline due to hardware err=%d; msgtype=%d ignored\n", 
                        scanner_hw_err, scan_msg.msgType );
        }

        clean_scantask_msgq();
        scan_task_queue_release_task( task_id );
    }

   /* never returns! */
}

/**
 * \brief  tell the world scan is open for business
 *
 * \author David Poole
 * \date 30-Sep-2010
 *
 */

static void scan_lives( void )
{
    scan_err_t scerr;
    scan_msg_t sendmsg;

    DBG_PRINTF_SDH_DEBUG("SCAN: scanning ready\n" );

    scan_set_hwerr( ScannerHWError_None );

    memset( &sendmsg, 0, sizeof(scan_msg_t) );
    sendmsg.msgType = SMSG_SCAN_INIT_DONE;
    scerr = scantask_msg_callback( &sendmsg );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 15-Feb-2011 ; set fake-o state so driver apps are happy */
    scerr = scands_set_integer( "scanner_state", SCANNER_IS_IDLE );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

/**
 * \brief  The One, the Only, the Mighty Scan Task!
 *
 *  The scan task is created by scantask_init().
 *
 *  The scan task is responsible for scanning. Initializes the scan hardware,
 *  watches for messages from the job manager telling it what to do, etc. As of
 *  this writing (11-Feb-05) this is the only task created for scanning.
 *
 * \author David Poole
 * \date ??-Dec-2004
 *
 */

static void *scan_task( void *input )
{
    scan_msg_t msg;
    scan_err_t scerr;
    uint32_t task_id;
    int i;
    const scan_init_function *init_list;

    scerr = scos_sem_init( &scantask_global_lock, "/scantask", 1 );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 10-Sep-2012 ; clean up stray locks (usually happens in Linux
     * userspace where the semaphore persists)
     */
    if( scantask_is_busy() ) {
        scantask_unlock();
    }

    /* create the message queue we'll use to interact with ourself */
    scerr = scos_mq_init( &scantask_msgq, SCANTASK_QUEUE_NAME, SCANTASK_QUEUE_SIZE );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 23-Sep-2013 ; splitting scantask and scanman into separate
     * modules. Scantask must own cmdline due to huge number of external
     * functions. 
     */
#ifdef __KERNEL__
    scan_cmdline_init();
#endif

    /* davep 30-Nov-2005 ; initialize the scanvar semaphore, etc. (no hardware
     * initialization!) 
     */
    scanvar_onetime_init();

    /* davep 23-May-2010 ; adding the scands */
    scerr = scands_onetime_init();
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 15-Feb-2011 ; set fake-o state so driver apps are happy */
    scerr = scands_set_integer( "scanner_state", SCANNER_IS_OFFLINE );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 22-Oct-2008 ; initialize the scanlib semaphore, etc. (no hardware
     * initializations!)
     */
    scerr = scanlib_onetime_init();
    if (scerr != SCANERR_NONE)
    {
        return NULL;
    }

    /* davep 05-Apr-2012 ; adding scandbg run-time registration so scandbg now
     * needs an init 
     */
    scandbg_onetime_init();

    /* davep 20-Sep-2009 ; initialize the taskqueue data structures (no hardware
     * intializations!) 
     */
    scerr = scan_task_queue_onetime_init();
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 16-Dec-2010 ; add scanity checks on what other developers are
     * plugging into our code. Will assfail if we don't like something.
     */
    scancap_sanity_check();

    /* davep 24-Aug-2011 ; adding memlimiter */
    scan_heap = mlimiter_by_name( "scan" );
    /* if this assert fails, I need to learn how/where the 'scan' mlimiter is
     * created so we can do it here */
    ASSERT( scan_heap );
    mlimiter_start( scan_heap );

    /* 
     * 3-Mar-05 ; NO hardware initialization to be done before this point! 
     */

    scantask_lock();

    /* we now start calling hardware init functions */

    /* davep 30-Sep-2010 ; changing from explicit huge 'if' block to an array
     * of init functions. 
     *
     * We pick an init list to run. The 'short' list doesn't initalize any
     * external hardware. Useful when running just the chip (no working sensor
     * or mech).
     *
     * TODO add run-time switch instead of compile-time switch (the if(0))
     */
    if( 0 ) {
        init_list = short_init_list;
    }
    else {
        init_list = full_init_list;
    }

    for( i=0 ; ; i++ ) {
        /* end of list marker */
        if( init_list[i]==NULL ) {
            break;
        }

        DBG_PRINTF_SDH_DEBUG( "%s calling init function[%d]=%p\n", __FUNCTION__, i, init_list[i] );
        scerr = init_list[i]();
        
        /* if a boot-time function fails, we go into a "debug" mode where we
         * can't scan but we will accept debug messages
         */
        if( scerr != SCANERR_NONE ) {
            mlimiter_stop( scan_heap );
            scan_hw_failure(scerr);
            /* never returns */
        }
    }

    /* tell everyone scan is alive */
    scan_lives();
    
#ifndef __KERNEL__    
    scanlib_kmsgs_task_init(); // add.lsh. not to get kernel msg before SMSG_SCAN_INIT_DONE from scantask
#endif    
        
    scantask_unlock();

    mlimiter_stop( scan_heap );

    /* 
     * Scantask will sit in this loop for the rest of his life.
     */
    while( 1 ) {
        /* block forever until we have something to do */
        scan_task_queue_get_task( &msg, &task_id );

        /* ballen 10/18/2011 - paper present scans are very chatty, but we want to keep
         * this for everyone else because regression testing uses it */
        if(msg.msgType != SMSG_SMECH_POLL) {
            DBG_PRINTF_SDH_DEBUG("%s received id=%d\n", __FUNCTION__, task_id );
        }

        /* davep 17-Jan-2013 ; from Scott Rowberry; dial down our memory
         * requirements if we're doing a paper present scan
         */
        if ( msg.msgType == SMSG_SMECH_POLL ) {
            // Put limiter to amount required for paper present scan
            mlimiter_adjust_limits( scan_heap, 4*1024*1024, 48*1024*1024 );
        }
        else {
            // Scan Request
            mlimiter_adjust_limits( scan_heap, MLIMITER_SCAN_MEMORY, MLIMITER_SCAN_MEMORY );
        }
//#ifdef __KERNEL__
		///Enabled by SINDOH yhlee
        mlimiter_start( scan_heap );
//#endif
        /* do something with message */
        scantask_lock();
        scan_dispatch_message( &msg );
        clean_scantask_msgq();
        scantask_unlock();

//#ifdef __KERNEL__
		///Enabled by SINDOH yhlee
        mlimiter_stop( scan_heap );
//#endif
        scan_task_queue_release_task( task_id );
        if(msg.msgType != SMSG_SMECH_POLL) {
            DBG_PRINTF_SDH_DEBUG("%s released id=%d\n", __FUNCTION__, task_id );
        }
    }

    return NULL;
}

/**
 * \brief  Calls scan_task(). Exists to fix prototype problems in thread
 * functions.
 *
 * \author David Poole
 * \date 10-Sep-2012
 */

#ifdef __KERNEL__
static int scan_task_kernel( void *input )
{
    scan_task(input);

    /* should never get here! */

    return 0;
}

int scantask_cmd_line_run( const char *cmdline ) 
{
    return cmd_line_run(cmdline);
}
#endif

#ifndef __KERNEL__
void scan_ready_scantask( void )
{
	scan_err_t scerr;
	
	if( use_scanman_sdh ) {
		scantask_ready();
		scantask_set_callback( old_callback_fn_sdh );
	}

	scanpower_scanning_is_active( false);

	/* davep 15-Feb-2011 ; set fake-o state so driver apps are happy */
	scerr = scands_set_integer( "scanner_state", SCANNER_IS_IDLE );
	XASSERT( scerr==SCANERR_NONE, scerr );
	
#if 0	//Blocked By SINDOH yhlee
	mlimiter_stop( scan_heap );
	DBG_PRINTF_SDH_DEBUG( "%s mlimiter_stop\n", __FUNCTION__ );
#endif
	return;
}
#endif

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date ??-Dec-2004
 *
 */

scan_err_t scantask_init( scan_callback_fn_t init_callback_fn )
{
//    scan_err_t scerr;

    /* davep 09-Mar-2011 ; moved semaphore and queue create to inside scantask;
     * fixes race condition between txposix2008 and txposix2011
     */

    scantask_set_callback( init_callback_fn );

#ifdef __KERNEL__
    //dros_create_thread(&scan_task_id,  scan_task_kernel, NULL, "kscantask", THREAD_STACK_SIZE, DROS_THR_PRI_IMAGE_DRIVER);
    dros_create_thread(&scan_task_id,  scan_task_kernel, NULL, "kscantask", THREAD_STACK_SIZE, DROS_THR_PRI_NORMAL);	// mod.lsh.potential contermeasure for SC221,222,223
//-jrs    scan_task_id =  kthread_run( scan_task_kernel, NULL, "kscantask" );

#else
    uint32_t pxretcode;
    pxretcode = posix_create_thread( &scan_task_id, &scan_task, 0, "scantask",
                                    scan_stack, THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL );
    XASSERT(pxretcode==0, pxretcode);
#endif

    return SCANERR_NONE;
}

