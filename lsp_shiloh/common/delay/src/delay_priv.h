/*
 * ============================================================================
 * Copyright (c) 2008-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * \file delay_priv.h
 *
 * \brief shared info for delay subsystem.
 *
**/


#ifndef DELAYPRIV_H
#define DELAYPRIV_H

#include "ATypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DBG_PRFX "timer: "                 // simple string prepend to the front of all strings.
#define LOGGER_SUBMOD 0x80000              // submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | LOGGER_SUBMOD

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)          // LOG_ERR is the level 
#define DBG_LOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 

/** internal delay_timer_msec message */
#define TIMER_POPPED          3
/** internal delay_timer_msec message */
#define TIMER_CANCELLED       4

// todo: no comments they need to look like they belong together
// change the names to oba: foo_bar_convention
void DM_msecTimer_cancelled(UINT32 userData,UINT32 userData1);
void DM_msecTimer_popped(void * userData);
void handle_timer_pop( void );
void init_msecDelayTimer(void);
void handle_timer_cancel(UINT32 param );

#ifdef __cplusplus
}
#endif

#endif // DELAYPRIV_H
