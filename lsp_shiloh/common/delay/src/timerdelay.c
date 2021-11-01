/*
 * ============================================================================
 * Copyright (c) 2008  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <posix_ostools.h>

#include "logger.h"
#include "debug.h"
#include <string.h>
#include "delay_api.h"
#include "timer_api.h"
#include "lassert.h"
#include "memAPI.h"
#include "cpu_api.h"
#include "mlist.h"
#include "delay_priv.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static int delay_cmd_cb( int argc, char *argv[] );
static const char *delay_cmd_desc;
static const char *delay_cmd_usage;
static const char *delay_cmd_notes;
#endif

struct DelayTimer_s
{
    sem_t timerBasedDelaysemaphore;  
    TimerDevice_t *delayTimer;
    TIMER_CONFIG config;
};
struct delay_msec_timer_s
{
    ATLISTENTRY node;
    sem_t timerBasedDelaysemaphore;  
    uint32_t delta_ticks;
    bool blocking;
    void(*timer_complete_callback)( void *callback_private_data);
    void *timer_complete_data; //< callback data on interrupt.
    bool repeating;
    uint32_t orig_ticks;
    bool canceled;
};
/** disable timer based Delays when in assert disables interrupts.
 */
static bool global_disabled_interrupts = false;


static void atassert_timerDelay(void)
{
    global_disabled_interrupts = true;
}

static void putSemaphore(void *userData)
{
    int px_status;
    px_status = sem_post( userData );
    ASSERT( px_status==0 );
}


DelayTimer_t * Construct_DelayTimer()
{
    DelayTimer_t * dt = 0;
    TimerInit();
    TimerDevice_t *timer = TimerOpen(0xff);

    if (timer)
    {
        dt = MEM_MALLOC(sizeof(DelayTimer_t));
        if (dt) 
        {
            int px_status;
            
            dt->delayTimer = timer;

            px_status = sem_init( &dt->timerBasedDelaysemaphore, 0, 0 );
            XASSERT( px_status != -1, px_status );
            atassert(atassert_timerDelay);
            
            dt->config.FuncPtr = putSemaphore;
            dt->config.userData = &dt->timerBasedDelaysemaphore;
            dt->config.RepCount = 1;
        }
    }
    return dt;
}

DelayTimer_t * Free_DelayTimer(DelayTimer_t *timer)
{
    if (timer)
    {
        int px_status;
        
        TimerClose(timer->delayTimer);

        px_status = sem_destroy( &timer->timerBasedDelaysemaphore );
        XASSERT( px_status==0, px_status );
        
        memFree(timer);
    }
    return 0;
}


void timerBasedDelay(DelayTimer_t *delayTimer, uint32_t ticks)
{
    int px_status;

#   ifdef TestTimer
    int start;
    int end;
#   endif

#   ifdef TestTimer
    start = cpu_get_ccount();
#   endif 
    if( global_disabled_interrupts || !delayTimer ) 
    {
        // can't use interrupts and semaphore when interupts are off, or no timer is available
        cpu_spin_delay(ticks); 
        return;
    }

    ASSERT(delayTimer->delayTimer);
    if (ticks < 50)  // ten is min resolution, 600 is debugging priorities. 
    {
        // Use spin loop delay when the delay time is very small.
        cpu_spin_delay(ticks); 
        return;
    } 
    else if (ticks > 100000)
    {
        posix_sleep_ms((ticks+9999)/10000);
        return;
    }
    else if (ticks > 10000)
    {
        delayTimer->config.Count = ticks / 1000;
        delayTimer->config.eTimebase = e_TIMEBASE_1_MS;
    }
    else if (ticks < 1024)
    {
        delayTimer->config.Count = ticks;
        delayTimer->config.eTimebase = e_TIMEBASE_1_US;
    }
    else 
    {
        delayTimer->config.Count = ticks / 10;
        delayTimer->config.eTimebase = e_TIMEBASE_10_US;
    }            

     TimerSetConfig(delayTimer->delayTimer, &delayTimer->config);

     TimerOn(delayTimer->delayTimer);

     // block waiting... 
    px_status = sem_wait( &delayTimer->timerBasedDelaysemaphore );
    ASSERT( px_status==0 );


#   ifdef TestTimer
    end = cpu_get_ccount();
    global_timer2_time_ticks = end - start;
    if (global_timer2_time_ticks < base * ticks * 591)

    {
        if (global_timer_short < global_timer2_time_ticks) 
        { 
            global_timer_short = global_timer2_time_ticks;
            global_timer2_time_ticks_base = base * ticks * 591;
        }
    }
#   endif 
}
static TimerDevice_t *msecDelayTimer = NULL;
TIMER_CONFIG msecDelayTimerConfig;
TX_SEMAPHORE msectimerBasedDelaysemaphore;
MLISTENTRY msecTimerHead;
MLISTENTRY msecTimerFreeHead;
#define STARTING_FREE_TIMERS 12
#define ENABLE_TIMER true
#define DISABLE_TIMER false
void push_free_msecTimer(delay_msec_timer_t * timer)
{
    MInsertHeadList(&msecTimerFreeHead, &timer->node);
}
void create_free_msecTimer(void)
{
    delay_msec_timer_t * dt = 0;
    dt = MEM_MALLOC(sizeof(delay_msec_timer_t));
    if (dt) 
    {
        int px_status;
        
        px_status = sem_init( &dt->timerBasedDelaysemaphore, 0, 0 );
        XASSERT( px_status != -1, px_status );
        
        ATInitNode(&dt->node);
        dt->delta_ticks = 0;
        push_free_msecTimer(dt);
    }
}
delay_msec_timer_t * pop_free_msecTimer(void)
{
    delay_msec_timer_t * dt = 0;
    ATLISTENTRY * headPtr;

    if (MIsListEmpty(&msecTimerFreeHead)) 
    {
        create_free_msecTimer();
    }
    headPtr = MRemoveHeadList(&msecTimerFreeHead);
    ATInitNode(headPtr);
    dt = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
    //DBG_LOG_DEBUG("Returned timer %x \n",dt);
    return (dt);
}


void init_msecDelayTimer(void)
{
    int i;
    msecDelayTimer = NULL; //there is a problem with downloading code, init_msec isn't called twice
    if (!msecDelayTimer) 
    {   
        //DBG_LOG_DEBUG("Inited msec timers %10d \n",timer_get_time_usec());
        TimerInit();
        msecDelayTimer = TimerOpen(0xff);
        msecDelayTimerConfig.Count = 0;     
        msecDelayTimerConfig.eTimebase = /*e_TIMEBASE_1_MS*/ e_TIMEBASE_100_US; 
        msecDelayTimerConfig.RepCount = 1;
        msecDelayTimerConfig.FuncPtr = DM_msecTimer_popped;
        msecDelayTimerConfig.userData = (void *) TIMER_POPPED;
        MInitList(&msecTimerHead);
        MInitList(&msecTimerFreeHead);
        atassert(atassert_timerDelay);
        for (i = 0; i< STARTING_FREE_TIMERS;i++) 
        {
            create_free_msecTimer();
        }

        #ifdef HAVE_CMD
        i = cmd_register_cmd( "delay", 
                                       NULL,
                              delay_cmd_desc,
                              delay_cmd_usage,
                              delay_cmd_notes,
                              delay_cmd_cb );
        REL_ASSERT( CMD_OK == i );
        #endif 
    }
}



#define LARGE_DELAY 500
#define MULT_FACTOR 10

delay_msec_timer_t *  delay_msec_timer_delay_aux(uint32_t ticks,
                                                 bool enable_timer,
                                                 delay_msec_timer_t * delayTimer);


void delay_msec_timer(uint32_t msec)
{
    delay_msec_timer_t * delayTimer;

    if (msec > LARGE_DELAY)
    {
        posix_sleep_ms((msec + 9) / 10);
        return;
    }
     
    delayTimer = pop_free_msecTimer();

    delayTimer->orig_ticks = msec * MULT_FACTOR;
    delayTimer->blocking = true;
    delayTimer->timer_complete_callback =  NULL;
    delayTimer->timer_complete_data = NULL;
    delayTimer->repeating = false;
    delayTimer->canceled = false;   
    DBG_LOG_DEBUG("Delay blocking %x %d\n",delayTimer,delayTimer->orig_ticks);
    delay_msec_timer_delay_aux(delayTimer->orig_ticks, ENABLE_TIMER, delayTimer);
}

delay_msec_timer_t * 
delay_msec_timer_non_blocking(uint32_t msec,
                              void(*timer_complete_callback)( void *callback_private_data),
                              void *timer_complete_data, //< callback data on interrupt.
                              bool repeating_if_true)
{
    delay_msec_timer_t * delayTimer;

    DBG_ASSERT(msec > 0);
    DBG_ASSERT(timer_complete_callback != NULL);
    
    delayTimer = pop_free_msecTimer();

    delayTimer->orig_ticks = msec * MULT_FACTOR;
    delayTimer->blocking = false;
    delayTimer->timer_complete_callback =  timer_complete_callback;
    delayTimer->timer_complete_data = timer_complete_data;
    delayTimer->repeating = repeating_if_true;
    delayTimer->canceled = false;
    DBG_LOG_DEBUG("Delay non-blocking ticks %x %d\n",delayTimer,delayTimer->orig_ticks);
    return (delay_msec_timer_delay_aux(delayTimer->orig_ticks,  ENABLE_TIMER, delayTimer));
}

delay_msec_timer_t *  
delay_msec_timer_delay_aux(uint32_t ticks,
                           bool enable_timer,
                           delay_msec_timer_t * delayTimer)
{
    bool startTimer = false;
    uint32_t ticks_done;
    ATLISTENTRY * headPtr;
    delay_msec_timer_t * dataPtr;
    
    MList_Lock( &msecTimerHead );


    ASSERT(msecDelayTimer);
       
    uint32_t begin_time, end_time;

    begin_time = timer_get_time_usec();
    //DBG_LOG_DEBUG("starting overhead %10d \n",begin_time);
    
    
    begin_time = timer_get_time_usec();
    if (MIsListEmpty(&msecTimerHead)) 
    {
        delayTimer->delta_ticks = ticks;
        MInsertHeadList(&msecTimerHead, &delayTimer->node);
        startTimer = true;
    }
    if (!startTimer) {
   
        if(ENABLE_TIMER == enable_timer)
        {
            // adding a new timer at an asynchronous point, so we need to see how far
            // into the head timer's count we current are and adjust appropriately
            ticks_done = timer_get_count(msecDelayTimer);
        }
        else
        {
            // at a timer expiration point, so no adjustment necessary
            ticks_done = 0;  
        }
        DBG_LOG_DEBUG("Ticks done %10d \n",ticks_done);
        headPtr = MListHead(&msecTimerHead);
        dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
        if (( dataPtr->delta_ticks - ticks_done) >  ticks) //< needs to go on head of list
        {
            if (enable_timer) TimerOff(msecDelayTimer);//< disable timer if it isn't already off
            dataPtr->delta_ticks -= (ticks + ticks_done);
            delayTimer->delta_ticks = ticks;
            MInsertHeadList(&msecTimerHead, &delayTimer->node);
            startTimer = true;
            DBG_LOG_DEBUG("Head  %10d \n",ticks);
        }
        else
        {
            headPtr = MListHead(&msecTimerHead);
            dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
            ticks += ticks_done; //we will be subtracting all of the first delta_tick when we should only be subtracting the smaller time.
            ticks -= dataPtr->delta_ticks;
            while (MListNext(&msecTimerHead,&dataPtr->node))//checking for end of list
            {
                ATLISTENTRY * nextPtr = MListNext(&msecTimerHead,&dataPtr->node);
                dataPtr = CONTAINING_RECORD(nextPtr, delay_msec_timer_t, node);
                if (ticks < dataPtr->delta_ticks) 
                {
                    dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
                    break;
                }
                
                headPtr = nextPtr;
                dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
                ticks -= dataPtr->delta_ticks;
            } 

            if (MListNext(&msecTimerHead,&dataPtr->node)) //< insert in middle of list
            {
                DBG_LOG_DEBUG("inserting in middle\n");
                ATLISTENTRY * nextPtr = MListNext(&msecTimerHead,&dataPtr->node);

                dataPtr = CONTAINING_RECORD(nextPtr, delay_msec_timer_t, node);
                dataPtr->delta_ticks -= ticks;
            }
            DBG_LOG_DEBUG("inserting after %x %x\n",headPtr,&delayTimer->node);
            delayTimer->delta_ticks = ticks;
            MListInsertAfter(&msecTimerHead, headPtr, &delayTimer->node);
            DBG_LOG_DEBUG("in list timer   %10d \n",ticks);
        }
    }

    if (enable_timer && startTimer)
    {
        headPtr = MListHead(&msecTimerHead);
        dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
        //DBG_LOG_DEBUG("delay programmed %10d \n",dataPtr->delta_ticks);
        msecDelayTimerConfig.Count = dataPtr->delta_ticks;
        TimerSetConfig(msecDelayTimer, &msecDelayTimerConfig);
        DBG_LOG_DEBUG("start timer  %x %10d %s\n", dataPtr,dataPtr->delta_ticks,(dataPtr->blocking == true)?"blocking":"non-blocking");
        TimerOn(msecDelayTimer);
    }
    MList_UnLock( &msecTimerHead );
    end_time = timer_get_time_usec();
    //DBG_LOG_DEBUG("ending overhead   %10d \n",end_time);
    //DBG_LOG_DEBUG("overhead delay uSec %d \n",end_time - begin_time);
    // block waiting...
    if (delayTimer->blocking == true) 
    {
        int px_status;
        
        //DBG_LOG_ERR("BLOCKING    \n");
        px_status = sem_wait( &delayTimer->timerBasedDelaysemaphore );
        ASSERT( px_status==0 );
        
        push_free_msecTimer(delayTimer);
        DBG_LOG_DEBUG("UNBLOCKED\n");
        return(NULL);
    }
    else
    {
        //DBG_LOG_ERR("NON BLOCKING    \n");
        return (delayTimer);
    }
    

    //done waiting
     
    

}

delay_msec_timer_t * delay_msec_timer_cancel(delay_msec_timer_t * non_blocking_timer)
{
    
    non_blocking_timer->canceled = true;
    DM_msecTimer_cancelled( TIMER_CANCELLED ,(uint32_t) non_blocking_timer);
    
    return NULL;
}

void handle_timer_cancel(uint32_t param )
{
    delay_msec_timer_t * repeatingTimer = (delay_msec_timer_t *) param;
    ATLISTENTRY * headPtr;
    ATLISTENTRY * nextPtr;
    delay_msec_timer_t * dataPtr;
    delay_msec_timer_t * nextDataPtr;
    uint32_t ticks_done;

    MList_Lock( &msecTimerHead );
    if (repeatingTimer->canceled == false) //< Timer has already been canceled
    {
        MList_UnLock( &msecTimerHead );
        return;
    }
    headPtr = MListHead(&msecTimerHead);
    dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
    if (repeatingTimer == dataPtr) //<on head of list
    {
        ticks_done = timer_get_count(msecDelayTimer);
        if ((repeatingTimer->delta_ticks - ticks_done) <= 1) //< danger, danger
        {
            /// Just wait
            MList_UnLock( &msecTimerHead );
            return; 
        }
        TimerOff(msecDelayTimer);
        if ((nextPtr = MListNext(&msecTimerHead, &repeatingTimer->node)))//< more timers on list 
        {
            nextDataPtr = CONTAINING_RECORD(nextPtr, delay_msec_timer_t, node);
            nextDataPtr->delta_ticks += (repeatingTimer->delta_ticks - ticks_done) ;
            msecDelayTimerConfig.Count = nextDataPtr->delta_ticks;
            TimerSetConfig(msecDelayTimer, &msecDelayTimerConfig);
            TimerOn(msecDelayTimer);
        }
        MRemoveHeadList(&msecTimerHead);
        push_free_msecTimer(repeatingTimer);
    }
    else
    {           
        if ((nextPtr = MListNext(&msecTimerHead, &repeatingTimer->node)))//< timer Not the last 
        {
            nextDataPtr = CONTAINING_RECORD(nextPtr, delay_msec_timer_t, node);
            nextDataPtr->delta_ticks += repeatingTimer->delta_ticks;
        }
        MRemoveEntryList(&msecTimerHead, &repeatingTimer->node);
        push_free_msecTimer(repeatingTimer);
    }
    MList_UnLock( &msecTimerHead );
}

void tickle_pop_timer(delay_msec_timer_t * timer, ATLISTENTRY  * repeatHead)
{
    if (timer->blocking == true)
    {
        int px_status;
        
        DBG_LOG_DEBUG("sem put  %x\n",timer);
        px_status = sem_post( &timer->timerBasedDelaysemaphore );
        ASSERT( px_status==0 );
    }
    else //< Non-blocking timers
    {
        if (!timer->canceled) //< this will catch timers that were canceled right when they popped.
        {
            DBG_LOG_DEBUG("Callback  %x\n",timer);
            (*timer->timer_complete_callback)(timer->timer_complete_data);
        }
        if (timer->repeating && !timer->canceled)//< if they are canceled, don't repeat them delete them instead
        {
            ATInsertHeadList(repeatHead,&timer->node); //< Timer is reused
        }
        else
        {
            timer->canceled = false;
            push_free_msecTimer(timer); //< timer is cleaned up
        }
    }
}


void handle_timer_pop( void )
{
    ATLISTENTRY * headPtr;
    delay_msec_timer_t * dataPtr;
    ATLISTENTRY repeatHead;
    uint32_t max_ticks = 3;
    uint32_t delta_ticks = 0;

    ATInitList (&repeatHead);
    MList_Lock( &msecTimerHead );
    TimerOff(msecDelayTimer);
    //DBG_LOG_ERR("Got timer pop %10d \n",timer_get_time_usec()/100);
    headPtr = MRemoveHeadList(&msecTimerHead);
    dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
    
    tickle_pop_timer(dataPtr, &repeatHead);
    headPtr = MListHead(&msecTimerHead);
    while (headPtr) //remove all timers that have same time
    {
        //DBG_LOG_DEBUG("In Multiple\n");
        dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
        if (dataPtr->delta_ticks == 0)
        {
            headPtr = MRemoveHeadList(&msecTimerHead);
            dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
        }
        else if (dataPtr->delta_ticks <= max_ticks)//close enough for government work
        {
            headPtr = MRemoveHeadList(&msecTimerHead);
            dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
            max_ticks -= dataPtr->delta_ticks;
            delta_ticks += dataPtr->delta_ticks;
        }
        else
        {
            break;
        }
        tickle_pop_timer(dataPtr, &repeatHead);
        headPtr = MListHead(&msecTimerHead);
    }
    if (!ATIsListEmpty(&repeatHead)) //< re-add all the repeat timers
    {
        //DBG_DEBUG("In Repeat \n");
        headPtr = ATRemoveHeadList(&repeatHead);
        do {
            dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
            delay_msec_timer_delay_aux(dataPtr->orig_ticks, 
                                       DISABLE_TIMER, 
                                       dataPtr);
            headPtr = ATRemoveHeadList(&repeatHead);
        } while (headPtr);
    }
    if (!MIsListEmpty(&msecTimerHead)) //< restart timer if still timers pending
    {
        //DBG_DEBUG("In Restart\n");
        headPtr = MListHead(&msecTimerHead);
        dataPtr = CONTAINING_RECORD(headPtr, delay_msec_timer_t, node);
        dataPtr->delta_ticks += delta_ticks;
        msecDelayTimerConfig.Count = dataPtr->delta_ticks;
        DBG_LOG_DEBUG("Restart  %x, %d %s\n",dataPtr, dataPtr->delta_ticks,dataPtr->blocking == true ? "blocking":"non-blocking");
        TimerSetConfig(msecDelayTimer, &msecDelayTimerConfig);
        TimerOn(msecDelayTimer);
    }
    MList_UnLock( &msecTimerHead );
}

#ifdef HAVE_CMD
static void test_timer_complete_callback( void *timer_complete_callback_data)
{
    uint32_t begin_time = (uint32_t) timer_complete_callback_data;
    uint32_t end_time;
    end_time = timer_get_time_usec()/100;
    //DBG_LOG_DEBUG("ending delay   %10d \n",end_time);
    //DBG_LOG_DEBUG("delay 0.1 msec  %d \n",end_time - begin_time);
    end_time -= begin_time;
}

static const char *delay_cmd_desc = "test commands for the delay apis";
static const char *delay_cmd_usage = "<type> <millisecond(s)>";
static const char *delay_cmd_notes = "type is one of 0, 1, 2 or 3 and mean the following: 0 is a blocking delay, 1 is a non-blocking (non-repeating) timer, 2 is a non-blocking, (repeating) timer, 3 cancels 2 and the millisecond parameter must be zero in this final case.  In all other cases millisecond(s) is the number of milliseconds to delay or wait for the timer to expire.";
static int delay_cmd_cb( int argc, char *argv[] )
{
    int type; 
    int msec;
    uint32_t begin_time;
    uint32_t end_time;
    static delay_msec_timer_t * repeatingTimer;
    
    if (argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }

    type = atoi(argv[1]);
    msec = atoi(argv[2]);
     
    /// dest validates input
    
    if (type == 0) 
    {

        begin_time = timer_get_time_usec()/100;
        //DBG_LOG_DEBUG("starting delay %10d \n",begin_time);
        delay_msec_timer( msec );
        end_time = timer_get_time_usec()/100;
        //DBG_LOG_DEBUG("ending delay   %10d \n",end_time);
        //DBG_LOG_DEBUG("delay 0.1 msec  %d \n",end_time - begin_time);
    }
    else if (type == 1) 
    {
        begin_time = timer_get_time_usec()/100;
        //DBG_LOG_DEBUG("starting delay %10d \n",begin_time);
        delay_msec_timer_non_blocking(msec,
                                      test_timer_complete_callback,
                                      (void *)begin_time, //< callback data on interrupt.
                                      false);
    }
    else if (type == 2) 
    {
        begin_time = timer_get_time_usec()/100;
        //DBG_LOG_DEBUG("starting delay %10d \n",begin_time);
        repeatingTimer = delay_msec_timer_non_blocking(msec,
                                                       test_timer_complete_callback,
                                                       (void *)begin_time, //< callback data on interrupt.
                                                       true);
    }
    else if (type == 3)
    {
        if (repeatingTimer) 
        {
            repeatingTimer = delay_msec_timer_cancel(repeatingTimer);
        }
    }
    else
    {
        return CMD_USAGE_ERROR;
    }

    return CMD_OK;
}

#endif
