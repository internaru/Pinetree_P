/*
 * ============================================================================
 * Copyright (c) 2008-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef DELAYAPI_H
#define DELAYAPI_H
/** 
 * \file delay_api.h
 *
 * \brief Timer based delay routines.
 *
 *  There are three api's in the file: delay_msec_timer, DelayTimer and the DMNotify* api's.
 * 
 *  The delay_msec_timer interface has both blocking,
 *  non-blocking (both one shot and repeating) and a cancel
 *  interface for the non-blocking, timers.  It shares one
 *  timer, is accurate to the msec, and has a
 *  maximum value of MAX_INT / 10.  This is the prefered
 *  interface.
 * 
 *  The DelayTimer api gets and keeps a timer resource.  
 *  Use this interface if you need a faster than 1 millisecond delay timer.
 * 
 *  DMNotify* is a legacy interface that should not be used if
 *  possible.  It can either be replaced by txTimers or
 *  delay_msec_timer non-blocking calls.
 *
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** delay_msec_timer interface supports blocking and callback msec timers.
 */
typedef struct delay_msec_timer_s delay_msec_timer_t;

/**
 * @brief Blocks task for given number of milliseconds.
 * 
 * @param[in] msec time to wait in milliseconds.
 */
void delay_msec_timer(uint32_t msec);

/// Gets the number of micro seconds since the system start.
/// 32bit counter will rollover every 1.19 hours, accurate,
/// on average the time will be 0.5 usec slow with a maximum
/// of 1usec slow, put differently: 
/// actual expired time may be 1 usec more than return value.
/// This service comes up early in initialization, calling before 
/// initialization, will return 0.
/// See timerDelay() for a blocking Delay.
/// See cpu_get_ccount() for timing faster than a microsecond.
///
uint32_t timer_get_time_usec();

/**
 * @brief calls callback with data after msec, repeats if true
 * until cancelled
 * 
 * @param[in] msec Time to wait
 * 
 * @param[in] timer_complete_callback function to call when time
 *            expires NEEDS TO BE LIGHT WEIGHT!!!!!
 * @param[in] timer_complete_data callers data is returned in callback
 *            function.
 * @param[in] repeating true:repeat timer. false: one shot.
 *            
 * @return delay_msec_timer_t* handle to timer for canceling.
 *            if your repeating or you plan on canceling prior to firing
 *            you need to hang on to the timer.  A non-repeating timer 
 *            (ie oneshot) without the need for canceling 
 *            will auto expire and you don't need to keep the handle.
 */
delay_msec_timer_t *
delay_msec_timer_non_blocking(
    uint32_t msec,
    void(*timer_complete_callback)(void *timer_complete_callback_data),
    void *timer_complete_callback_data, 
    bool repeating_if_true);

/**
 * Cancel an existing non_blocking timer, 
 * 
 * Proper calling method is
 * @code timer = delay_msec_timer_cancel ( timer ); @nocode
 * 
 * If the timer has popped just prior to canceling, the timer's
 * callback may be called before the timer is canceled.
 * canceling a non-repeating timer that has already fired can only be
 * expect to work correctly in the time frame of the fired race condition.
 * a fired one shot timer need not be cancelled, it self expires.
 * 
 * \param[in] non_blocking_timer handle to timer.
 * 
 * \return delay_msec_timer_t* NULL , is used to destroy your reference.
 */
delay_msec_timer_t * delay_msec_timer_cancel(delay_msec_timer_t * non_blocking_timer);




/** DelayTimer_t interface hangs onto a timer for exclusive use of one client.
 */
typedef struct DelayTimer_s DelayTimer_t;

#ifdef BOOTCODE_BUILD
#include "cpu_api.h"
#define timerDelay( timer, u_sec ) cpu_spin_delay( u_sec )
#else
#define timerDelay( timer, u_sec ) timerBasedDelay( timer, u_sec )
#endif

/** 
 * This function blocks the given number of microseconds allowing other threads to run.
 * delays of less than 50microseconds are cpu_spin_delay loops.
 * precision is based on the underlying timer, with rounding to nearest unit,
 * this is not a high precision timer : 10000004 will not have 1 microsecond precision
 *
 * @param[in] timer The timer to use.
 *
 * @param[in] microsecondsToDelay The number of microseconds to delay.
 */
void timerBasedDelay(DelayTimer_t *timer, uint32_t microsecondsToDelay);

/** 
 * Construct a DelayTimer.
 * @return a valid timer or 0 on error.
 */
DelayTimer_t * Construct_DelayTimer();

/** 
 * Free a DelayTimer.
 * 
 * @return NULL  uses the pattern: ptr = free(ptr) 
 *               to null your pointer after destruction.
 */
DelayTimer_t * Free_DelayTimer(DelayTimer_t *timer);

#ifdef __cplusplus
}
#endif

#endif // DELAYAPI_H
