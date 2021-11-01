/* 
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2006
 */

#ifndef	_OS_TIMERS_H
#define _OS_TIMERS_H

//-jrs#include "marvellTypeMap.h"
#include "agMessage.h"

void wtimerFunc(void *);

static inline void InitializeTimer(PWLAN_DRV_TIMER timer,
                                   void (*TimerFunction)(void *context),
                                   void *FunctionContext)
{
#if 1
    MESSAGE *msg;

    msg = (MESSAGE *) MEM_MALLOC(sizeof(MESSAGE));

    msg->msgType = 0;
    msg->param1 = (UINT32) TimerFunction;
    msg->param2 = (UINT32) FunctionContext;

    tx_timer_create(timer, "mlanTimer", (void*)wtimerFunc, 
            (u32) msg, 1, 0, TX_NO_ACTIVATE);

#else
    tx_timer_create(timer, "wlanTimer", (void*)TimerFunction, 
			(u32) FunctionContext, 1, 0, TX_NO_ACTIVATE);
#endif
}

static inline void SetTimer(PWLAN_DRV_TIMER timer, UINT MillisecondPeriod)
{
	/* 
	 * Divide by 10 to convert ms to ticks
	 */
	tx_timer_change(timer, MillisecondPeriod/10, 0);
	tx_timer_activate(timer);
}

static inline void ModTimer(PWLAN_DRV_TIMER timer, UINT MillisecondPeriod)
{
    UINT timeout;
	tx_timer_deactivate(timer);
	/* Divide by 10 to convert ms to ticks */
    timeout = MillisecondPeriod/10;
    if(timeout == 0)
    {
        // a timeout of 0 is used by the driver code to indicate that the timer
        // should fire immediately.  However, threadx considers 0 to be an illegal
        // value when tx_timer_activate is called, so instead we set at timeout for 1 tick.
        timeout = 1;
    }
	tx_timer_change(timer, timeout, 0); 
	tx_timer_activate(timer);
}

static inline void SetPeriodicTimer(PWLAN_DRV_TIMER timer, 
						UINT MillisecondPeriod)
{
	/* Divide by 10 to convert ms to ticks */
	tx_timer_change(timer, MillisecondPeriod/10, MillisecondPeriod/10); 
	tx_timer_activate(timer);
}

static inline void CancelTimer(WLAN_DRV_TIMER *timer)
{
    tx_thread_relinquish();
    tx_timer_deactivate(timer);
}

static inline void FreeTimer(TX_TIMER *timer)
{
    if (timer->tx_timer_internal.tx_timeout_param)
    {
        void *mem_ptr = (void *)timer->tx_timer_internal.tx_timeout_param;
        MEM_FREE_AND_NULL(mem_ptr);
    }
    tx_timer_delete(timer);
}

#endif	/* _OS_TIMERS_H */
