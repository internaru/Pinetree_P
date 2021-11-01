/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file DelayMgr.c
 *
 * \brief This file implements an API that allows other tasks to register to
 *        receive messages after arbitrary time delays.  Each user can specify
 *        whether he wants one-shot or repeating notifications. One example of
 *        a need for a repeating notification is PJL TIMED USTATUS.
 *
 *  Shown below is typical code demonstrating how to use the API offered
 *  by the delay manager.  Note that you only need to unregister from
 *  repeating notifications, the unregister is automatic from one-shot
 *  notifications.
<pre>
BOOL DMNotifyRegister( MODULE_ID modID, UINT32 nTicks, BOOL bRepeat, UINT32 uiParam );
BOOL DMNotifyUnregister( MODULE_ID modID, UINT32 uiParam );

void SomeThread(UINT32 Temp)
{
    MESSAGE     Mesag;
    UINT        txRetCode;
    static UINT nCount = 0;

    // Wait for the system init to complete before handling messages.
    SysWaitForInit();

    DMNotifyRegister( SOMETHREADID, 1000, FALSE, 8 );
    DMNotifyRegister( SOMETHREADID, 100,  TRUE, 7 );

    while( TRUE )
    {
        txRetCode = tx_queue_receive( &Inbox, (UINT32 *) &Mesag, TX_WAIT_FOREVER );

        if ( txRetCode != TX_SUCCESS )
            break;

        switch( Mesag.msgType )
        {

        case MSG_DELAY_NOTIFY:
            DPRINTF(DBG_SOFT|DBG_OUTPUT, ("SomeThread() received MSG_DELAY_NOTIFY with uiParam =%u\n", Mesag.param1 );

            if ( Mesag.param1 == 7 )
            {
                nCount++;
                if ( nCount > 5 )
                {
                    DMNotifyUnregister( SOMETHREADID, 7);
                }
            }
            break;
        case <other messages>:
        }
    }
}
</pre>
 *****************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <limits.h>           // defines UINT_MAX

#include <pthread.h>

#include "lassert.h"           // defines XASSERT()
#include "dprintf.h"          // defines dprintf()

#include "ATypes.h"           // defines UINT, SINT32, BOOL, etc.
#include "delay_api.h"
#include "agRouter.h"
#include "agMessage.h"        // defines MESSAGE struct
#include "logger.h"        // defines ALogString(), etc.
#include "sys_init_api.h"
#include "timer_api.h"
#include "delay_priv.h"

////////////////////////////////////////////////////////////////////////////////
// Defines and Typedefs
////////////////////////////////////////////////////////////////////////////////


#define LOGEVENT ALogString

#define QUEUE_SIZE        10       // number of messages in our queue

#define THREAD_STACK_SIZE  1024 * 4

#define MAX_SUBSCRIPTIONS  10

#define ADD_SUBSCRIPTION      0    // used with MSG_DELAY_SUBSCRIPTION
#define DELETE_SUBSCRIPTION   1
/*see delay_priv.h*/

typedef struct _Subscription
{
    MODULE_ID    modID;
    UINT32       uiRequestedDelay; // in timer ticks
    BOOL         bRepeat;
    UINT32       uiParam;          // arbitrary callback data
    UINT32       uiRemainingDelay; // in timer ticks
} Subscription;


////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Statics (local globals)
////////////////////////////////////////////////////////////////////////////////

static pthread_t     delaymgr_thread_id;
static mqd_t         inbox_msgq;
ALIGN8 static unsigned char ThreadStack[THREAD_STACK_SIZE];

////////////////////////////////////////////////////////////
// Statics ("local" globals) protected by our Synchronizer.
////////////////////////////////////////////////////////////
static Subscription subscriptions[MAX_SUBSCRIPTIONS];
static UINT32       nSubscriptions;
static UINT32       uiCurrentTickCount;
static UINT32       uiLastTickCount;
static UINT32       uiTranspiredTickCount;
static UINT32       uiWaitInTicks;

////////////////////////////////////////////////////////////////////////////////
// { Begin Object: Synchronizer
////////////////////////////////////////////////////////////////////////////////

/**
 * Because the DelayMgr() thread and the DMNotifyRegister() and
 * DMNotifyUnregister() functions all use the subscriptions[] matrix
 * we need to use a mutex to protect against simultaneous access.
 *
 * Use a mutex/semaphore to accomplish mutual exclusion.
 */
static pthread_mutex_t mutex;

////////////////////////////////////////////////////////////
static void CreateSynchronizer(void)
{
    int px_status;

    px_status = posix_mutex_init( &mutex );
    XASSERT( px_status==0, px_status );
} // end CreateSynchronizer()

////////////////////////////////////////////////////////////
static void LockSynchronizer(void)
{
    int px_status;

    px_status = pthread_mutex_lock( &mutex );
    XASSERT( px_status==0, px_status );
} // end LockSynchronizer()

////////////////////////////////////////////////////////////
static void UnlockSynchronizer(void)
{
    int px_status;

    px_status = pthread_mutex_unlock( &mutex );
    XASSERT( px_status==0, px_status );
} // end UnlockSynchronizer()

////////////////////////////////////////////////////////////////////////////////
// } End   Object: Synchronizer
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Debugging Symbols
////////////////////////////////////////////////////////////////////////////////

//#define DBG_DELAYMGR  1  // Comment out this line to deactivate debug output.

#ifdef DBG_DELAYMGR

////////////////////////////////////////////////////////////
static void DebugPrintfIthSubscription(int i, const char* tagString)
{
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("DelayMgr: %s subscriptions[%d].modID=%d .Param=0x%08X .Request=0x%X .Remain=0x%X\n",
                                  tagString, i,
                                  subscriptions[i].modID,
                                  subscriptions[i].uiParam,
                                  subscriptions[i].uiRequestedDelay,
                                  subscriptions[i].uiRemainingDelay));
} // end DebugPrintfIthSubscription()

////////////////////////////////////////////////////////////
static void DebugPrintfAllSubscriptions(const char* tagString)
{
    int i;

    for ( i=0; i < nSubscriptions; i++ )
    {
        DebugPrintfIthSubscription(i, tagString);
    }
} // end DebugPrintfAllSubscriptions()

#else  // !DBG_DELAYMGR
# define DebugPrintfAllSubscriptions(tagString)
# define DebugPrintfIthSubscription(i, tagString)
#endif // !DBG_DELAYMGR


////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////

void    DelayMgrInit( void );
void    *DelayMgr( void *unused );

static void UpdateTickCountsWhileLockSynchronizer(void);
static void UpdateWaitInTicksWhileLockSynchronizer(void);


////////////////////////////////////////////////////////////////////////////////
// DelayMgrInit()
////////////////////////////////////////////////////////////////////////////////

void    DelayMgrInit(void)
{
    int px_status;

    CreateSynchronizer();

    // Create our message queue and register it with the system so we can receive mail.

    posix_create_message_queue(&inbox_msgq, "/DelayMgrQ", QUEUE_SIZE, sizeof(MESSAGE));
    
    px_status = posix_create_thread(&delaymgr_thread_id, DelayMgr, 0, "delay_mgr", &ThreadStack, THREAD_STACK_SIZE, POSIX_THR_PRI_IMAGE);
    XASSERT( px_status==0, px_status );

    nSubscriptions = 0;

} // DelayMgrInit()


////////////////////////////////////////////////////////////////////////////////
// DelayMgr()
////////////////////////////////////////////////////////////////////////////////

void    *DelayMgr( void *unused )
{
    MESSAGE     incomingMsg, outgoingMsg;
    
    //int px_status;
    int         i;

    // Wait for the system initialization to finish.
    init_msecDelayTimer();
    SysWaitForInit();
    

    // Our first wait can be indefinite since no subscriptions exist until
    // we receive the first MSG_NEW_DELAY_SUBSCRIPTION message.
    // OK not to call LockSynchronizer() and UnlockSynchronizer(), since no one
    // else can send us messages yet, since our queue is yet to be created.

    uiWaitInTicks = POSIX_WAIT_FOREVER;

    uiLastTickCount = posix_gettime_ticks();
    
    while (1)
    {

        // Wait for receipt of a message or else a timeout.

        while ( posix_wait_for_message( inbox_msgq, (char*)&incomingMsg, sizeof(MESSAGE), 
                  ( (uiWaitInTicks == POSIX_WAIT_FOREVER) ? POSIX_WAIT_FOREVER : (uiWaitInTicks*10*1000) ) )  == ETIMEDOUT )
        {
            LockSynchronizer();
#ifdef DBG_DELAYMGR
            DPRINTF(DBG_ERROR|DBG_OUTPUT, ("DelayMgr: timeout\n"));
#endif // DBG_DELAYMGR

            UpdateTickCountsWhileLockSynchronizer();

            // Figure out who should get notified.
            for( i=0; i < nSubscriptions; i++ )
            {
                if ( subscriptions[i].uiRemainingDelay == 0 )
                {
                    // This subscriber should be notified.

                    outgoingMsg.msgType = MSG_DELAY_NOTIFY;
                    outgoingMsg.param1 = subscriptions[i].uiParam;

#ifdef DBG_DELAYMGR
                    DPRINTF(DBG_ERROR|DBG_OUTPUT, ("DelayMgr: sending MSG_DELAY_NOTIFY to %u\n", subscriptions[i].modID));
#endif // DBG_DELAYMGR

                    if (SYMsgSendNoWait(subscriptions[i].modID, &outgoingMsg) != OK)
                    {
                        DPRINTF(DBG_ERROR|DBG_OUTPUT, ("DelayMgr: ERROR! cannot send MSG_DELAY_NOTIFY to %u\n", subscriptions[i].modID));
                        // Try again later, even if not subscribed to repeat.
                        subscriptions[i].uiRemainingDelay = subscriptions[i].uiRequestedDelay;
                    }

                    if ( subscriptions[i].bRepeat )
                    {
                        // Set up for another delay.
                        subscriptions[i].uiRemainingDelay = subscriptions[i].uiRequestedDelay;
                    }
                } // end if ( subscriptions[i].uiRemainingDelay == 0 )
            } // end for ( i < nSubscriptions )

            // Now clean up any one-shot subscriptions that just fired.

            for( i=0; i < nSubscriptions; i++ )
            {
                if( subscriptions[i].uiRemainingDelay == 0 )
                {
                    int    j;

                    // To delete this subscription we must shift all subsequent
                    // subscriptions down in the subscriptions[] array.

                    for( j=i; j < nSubscriptions-1; j++ )
                    {
                        subscriptions[j] = subscriptions[j+1];   // struct copy
                    }

                    nSubscriptions--;

#ifdef DBG_DELAYMGR
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, ("DelayMgr: reduced nSubscriptions to %u by eliminating subscription #%u\n", nSubscriptions, i));
#endif // DBG_DELAYMGR

                    i--;   // tricky, and only works if it has type int rather than UINT
                }
            }

            DebugPrintfAllSubscriptions("timing:");

            UpdateWaitInTicksWhileLockSynchronizer();

            UnlockSynchronizer();
        } // end while (timed out)
        if (incomingMsg.param1 >= TIMER_POPPED)
        {
            if (incomingMsg.param1 == TIMER_POPPED) 
            {
                handle_timer_pop( );
            }
            else if (incomingMsg.param1 == TIMER_CANCELLED) 
            {
                handle_timer_cancel(incomingMsg.param2 );
            }
            
            if (uiLastTickCount == posix_gettime_ticks())
                 continue;
        }
        // We only get to here when we have received a message, and the
        // only message we ever receive is MSG_DELAY_SUBSCRIPTION which
        // notifies us that someone has either added or deleted a subscription.
        // We update our state variables in subscriptions[] and decide upon the
        // timeout we should use in our next call to tx_queue_receive().

        LockSynchronizer();

#ifdef DBG_DELAYMGR
        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("DelayMgr: received msg %u\n", incomingMsg.msgType));
#endif // DBG_DELAYMGR

        UpdateTickCountsWhileLockSynchronizer();

        UpdateWaitInTicksWhileLockSynchronizer();

        UnlockSynchronizer();
    } // end while (1)

    return 0;
} // end DelayMgr()


////////////////////////////////////////////////////////////////////////////////
// DMNotifyRegister()
////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Register to be notified when a certain time span has elapsed.
 *
 * \param modID (MODULE_ID) Thread asking for notification.
 * \param nTicks (UINT32) Number of clock ticks after which to be notified.
 * \param bRepeat (BOOL) true sets up repeating notifications; false sets up a
 *                       one-time notification.
 * \param uiParam (UINT32) Arbitrary information word passed in at registration,
 *                         and returned as param1 in notify message.
 * 
 * \return BOOL true means registration was successful; false means it was
 *              refused because there was no capacity left.
 *
 * By calling this function a thread can request either a single
 * MSG_DELAY_NOTIFY message to be delivered back to that thread's
 * mailbox after an arbitrary delay (bRepeat=FALSE) or else a
 * continual stream of such messages (bRepeat=TRUE) until a future
 * call to DMNotifyUnregister().
 *
 * Note that a thread can register for multiple delay notifications
 * as long as each subscription has a unique uiParam.
 */
bool DMNotifyRegister( MODULE_ID modID, uint32_t nTicks, bool bRepeat, uint32_t uiParam )
{
    MESSAGE    msg;
    BOOL       bReturn;

    LockSynchronizer();

    if ( nSubscriptions < MAX_SUBSCRIPTIONS )
    {
        int px_status;

        /**
         * We must update existing table entries at this point, for the
         * following reasons:
         * -# If the subscription table is empty, then we need a refreshed (and
         *    very small) value in uiTranspiredTickCount. Otherwise, the new
         *    subscription will likely fire right away, because its requested
         *    delay is probably less than whatever amount of time has transpired
         *    with the subscription table sitting empty.
         * -# If the subscription table is not empty, then whatever time span
         *    has transpired since the last time through must be deducted from
         *    the existing subscriptions, but not from the new subscription.
         * 
         * The timeout time will be taken care of when the message we send gets
         * processed.
         */
        UpdateTickCountsWhileLockSynchronizer();

        subscriptions[nSubscriptions].modID            = modID;
        subscriptions[nSubscriptions].uiRequestedDelay = nTicks;
        subscriptions[nSubscriptions].bRepeat          = bRepeat;
        subscriptions[nSubscriptions].uiParam          = uiParam;
        subscriptions[nSubscriptions].uiRemainingDelay = nTicks;
        nSubscriptions++;

        // We now inform our thread of this change.

        msg.msgType = MSG_DELAY_SUBSCRIPTION;
        msg.param1  = ADD_SUBSCRIPTION;

        px_status = posix_message_send( inbox_msgq, (char *)&msg, sizeof(msg), MQ_DEFAULT_PRIORITY, 0 );
        if (px_status != 0)
        {
            XASSERT( px_status==ETIMEDOUT, px_status );
        }

        DPRINTF( (DBG_SOFT | DBG_OUTPUT),
                ( "DelayMgr subscribed module %d for a %s callback in %u ticks\n",
                  modID,
                  (bRepeat ? "repeating" : "one-shot"),
                  nTicks ) );

        bReturn = (px_status == 0) ? TRUE : FALSE;
    }
    else
    {
        DPRINTF((DBG_LOUD | DBG_ERROR | DBG_OUTPUT),( "DelayMgr: WARNING: Subscribe FAILED. Max Subscriptions of %d exceeded.\n",  MAX_SUBSCRIPTIONS));
        XASSERT(nSubscriptions < MAX_SUBSCRIPTIONS, MAX_SUBSCRIPTIONS);
        bReturn = FALSE;
    }

    UnlockSynchronizer();

    return bReturn;
} // end DMNotifyRegister()


////////////////////////////////////////////////////////////////////////////////
// DMNotifyUnregister()
////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unregister a previously registered notification subscription.
 *
 * \param modID (MODULE_ID) Thread that asked for the notification that is to be
 *                          unregistered.
 * \param uiParam (UINT32) Arbitrary information word passed in at registration,
 *                         which is used here to identify the exact notification
 *                         that is to be unregistered.
 * 
 * \return BOOL true if the specified subscription was found and unregistered;
 *              false if it was not found in the current subscription table.
 *
 * Note that a thread can register for multiple delay notifications
 * as long as each subscription has a unique uiParam.
 */
bool DMNotifyUnregister( MODULE_ID modID, uint32_t uiParam )
{
    UINT       i;
    MESSAGE    msg;
    BOOL       bReturn = FALSE;

    LockSynchronizer();

    /**
     * No need to update tick counts here. Just delete the requested
     * subscription. Sending the message to DelayMgr() induces the necessary
     * tick count and timeout time updates.
     */

    for( i=0; i < nSubscriptions; i++ )
    {
        if( ( subscriptions[i].modID == modID )
         && ( subscriptions[i].uiParam == uiParam ) )
        {
            int px_status;

            // To delete this subscription we must shift all subsequent
            // subscriptions down in the subscriptions[] array.

            for( /* nada */; i < nSubscriptions-1; i++ )
            {
                subscriptions[i] = subscriptions[i+1];   // struct copy
            }

            nSubscriptions--;

#ifdef DBG_DELAYMGR
            DPRINTF(DBG_SOFT|DBG_OUTPUT, ("DMNotifyUnregister() reduced nSubscriptions to %u by eliminating subscription #%u\n", nSubscriptions, i));
#endif // DBG_DELAYMGR

            // We now inform our thread of this change.

            msg.msgType = MSG_DELAY_SUBSCRIPTION;
            msg.param1  = DELETE_SUBSCRIPTION;

            px_status = posix_message_send( inbox_msgq, (char *)&msg, sizeof(msg), MQ_DEFAULT_PRIORITY, 0 );
            if (px_status != 0)
            {
                XASSERT( px_status==ETIMEDOUT, px_status );
            }

            if ( px_status == 0 )
            {
                bReturn = TRUE;
            }
        }
    }

    DebugPrintfAllSubscriptions("after unregis:");

    UnlockSynchronizer();

    return bReturn;
} // end DMNotifyUnregister()

////////////////////////////////////////////////////////////////////////////////
/**
 * As its name suggests, this function should only be called after a call to
 * LockSynchronizer() and before the matching call to UnlockSynchronizer().
 */
static void UpdateTickCountsWhileLockSynchronizer(void)
{
    uint32_t i;

    uiCurrentTickCount = posix_gettime_ticks();

    if ( uiCurrentTickCount >= uiLastTickCount )
        uiTranspiredTickCount = uiCurrentTickCount - uiLastTickCount;
    else
        uiTranspiredTickCount = UINT_MAX - uiLastTickCount + uiCurrentTickCount;

#ifdef DBG_DELAYMGR
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ( "DelayMgr: ticks: last=0x%X, current=0x%X, transpired=0x%X\n", uiLastTickCount, uiCurrentTickCount, uiTranspiredTickCount));
#endif // DBG_DELAYMGR

    uiLastTickCount = uiCurrentTickCount;

    for ( i=0; i < nSubscriptions; i++ )
    {
        DebugPrintfIthSubscription(i, "before:");

        if ( subscriptions[i].uiRemainingDelay <= uiTranspiredTickCount ) // <= is important
        {
            subscriptions[i].uiRemainingDelay = 0;
        }
        else
        {
            subscriptions[i].uiRemainingDelay -= uiTranspiredTickCount;
        }

        DebugPrintfIthSubscription(i, "after: ");
    }
} // end UpdateTickCountsWhileLockSynchronizer()

////////////////////////////////////////////////////////////////////////////////
/**
 * As its name suggests, this function should only be called after a call to
 * LockSynchronizer() and before the matching call to UnlockSynchronizer().
 */
static void UpdateWaitInTicksWhileLockSynchronizer(void)
{
    uint32_t i;

    uiWaitInTicks = POSIX_WAIT_FOREVER;

    for ( i=0; i < nSubscriptions; i++ )
    {
        if( subscriptions[i].uiRemainingDelay < uiWaitInTicks )
            uiWaitInTicks = subscriptions[i].uiRemainingDelay;

        DebugPrintfIthSubscription(i, "wait:  ");
    }

#ifdef DBG_DELAYMGR
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("DelayMgr: wait: return to queue with uiWaitInTicks=0x%X\n", uiWaitInTicks));
#endif // DBG_DELAYMGR
} // end UpdateWaitInTicksWhileLockSynchronizer()


void DM_msecTimer_cancelled(UINT32 userData,UINT32 userData1)
{
    int px_status;
    MESSAGE    msg;
    
    msg.msgType = MSG_DELAY_SUBSCRIPTION;
    msg.param1  =  userData;
    msg.param2  =  userData1;
    
    //DBG_LOG_ERR("Got timer cancelled %10d \n",timer_get_time_usec()/100);
    px_status = posix_message_send( inbox_msgq, (char *)&msg, sizeof(msg), MQ_DEFAULT_PRIORITY, 0 );
    if (px_status != 0)
    {
        XASSERT( px_status==ETIMEDOUT, px_status );
    }
}

void DM_msecTimer_popped(void * userData)
{
    int px_status;
    MESSAGE    msg;
    
    msg.msgType = MSG_DELAY_SUBSCRIPTION;
    msg.param1  = (UINT32) userData;
    
    //DBG_LOG_ERR("Got timer pop ISR %10d \n",timer_get_time_usec()/100);
    px_status = posix_message_send( inbox_msgq, (char *)&msg, sizeof(msg), MQ_DEFAULT_PRIORITY, 0 );
    if (px_status != 0)
    {
        XASSERT( px_status==ETIMEDOUT, px_status );
    }
}

