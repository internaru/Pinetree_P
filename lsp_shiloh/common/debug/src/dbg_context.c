/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __linux__
#include "tx_api.h"
#include "tx_ini.h" /* For TX_INITIALIZE_IN_PROGRESS */
#include "tx_thr.h" /* For _tx_thread_current_ptr and _tx_thread_system_state */
#endif
#include "dbg_context.h"

/*----------------------------------------------------------------------------*/
/* This logic actually comes from ExpressLogic and should, therefore be
   a correct method of determining that we are in thread context and not
   int interrupt or timer context */
int dbg_context_is_thread(void)
{
#ifdef __linux__
    // In Linux, it is assumed all printing modules/apps will be executing 
    // this debug module from a thread.
    return 1;
#else
    extern TX_THREAD _tx_timer_thread;

    /* We are in thread context if we are initializing or all of the following:
         We are not in the system state (which indicates interrupt)
         We have a valid thread pointer and
         That thread pointer does not point to the timer thread
     */
    return ((_tx_thread_system_state == TX_INITIALIZE_IN_PROGRESS) ||
            (!_tx_thread_system_state &&
              _tx_thread_current_ptr &&
             (_tx_thread_current_ptr != &_tx_timer_thread)));
#endif             
}
