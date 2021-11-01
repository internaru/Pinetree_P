/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** \file pwm.h
 *  Public API for the PWM driver (Digital pulse width modulator).
 *  
 *  
 */

#ifndef PWM_API_H
#define PWM_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "key_table.h"
#include "error_types.h"

/// handle to a pwm object.
typedef struct pwm_s pwm_t;

typedef enum {
    eDPWM_Timebase_Clks = 0x0,
    eDPWM_Timebase_1us  = 0x1,
    eDPWM_Timebase_10us = 0x2,
    eDPWM_Timebase_100us = 0x3,
    eDPWM_Timebase_1ms   = 0x4,
    eDPWM_Timebase_10ms  = 0x5,
    eDPWM_Timebase_100ms = 0x6
} dpwm_timebase_e;


/// logging should be to : module == DEVICE && submodule/channel == LOGGER_SUBMOD_DEVICE_PWM
#define LOGGER_SUBMOD_DEVICE_PWM  0x01000000

/// Run a fan or other pwm, by setting it's speed
/// 
/// example: pwm_run(pwm_by_name("side_fan"), 255); // full on./// 
///
void
pwm_run(
    pwm_t *pwm,   ///< which pwm to run 
    uint16_t dutycycle  ///< 0 is off, 0xff is FULL on, or 0xffff depending on dpwm
    );

/// Run a fan or other pwm, by setting its percent of duty cycle.
/// 
void
pwm_run_dutycycle_percent(
    pwm_t *pwm,   ///< which pwm to run 
    uint32_t dutycycle_percent  ///< percent of configured period 0-100
    );

/// Turn a PWM on at it's last configured duty cycle
void pwm_on(const pwm_t *pwm);   ///< which pwm to turn on

/// Turn a PWM off
void pwm_off(const pwm_t *pwm);   ///< which pwm to turn off


/// returned object is not owned so don't delete it.
/// int key - string - object mapping, from key_table.h
/// object_by_index
/// object_by_name
/// name_by_index
/// index_by_name  
KEY_TABLE_DECLARE_PUBLIC_2( pwm, pwm_t );

/// General Digital Pulse Width Modulation initialization code always called, once.
///
void pwm_init(void);

/// open via the error = open( **handle, id ); interface
///
#define pwm_open( h, pwm_channel )                             \
    ((0 == (*h = pwm_by_index(pwm_channel)))                   \
     ? FAIL : OK)                                              \


/// closing has no behavior
///
#define pwm_close( h ) 

/// Expose the full hw interface to pwm on the client side.
/// xxxxxx______------
/// delay first second
///
/// This interface maybe useful to fine control a high frequency
/// pwm where (frequency desired) / (clock)) is small enough that 
/// fine tuning the exact number of clocks is required.
///
void
pwm_run_full(pwm_t *pwm,         ///< which pwm to control.
             bool first_cycle_high,    ///< is first semicycle high?
             uint16_t delay_cnt,       ///< initial skip before cycling.
             uint16_t first_semicycle, ///< number of clocks high/low
             uint16_t second_semicycle,///< number of clocks low/high
	     int dpwm_timebase         ///< -1 don't change, 0 - 6 matches dpwm_timebase_e  
    );

/// default the timebase when calling pwm_run_full().
/// backward compatible interface
#define pwm_run_fancy(pwm, first_cycle_high, delay_cnt, first_semicycle) \
  pwm_run_full(pwm, first_cycle_high, delay_cnt, first_semicycle, second_semicycle, -1 )         

/// Change the duty cycle of an active PWM.
void
pwm_change_duty_cycle(pwm_t *pwm, 
              uint16_t first_semicycle,
              uint16_t second_semicycle);

/// Change the timebase of an active PWM.
void
pwm_change_timebase(pwm_t *pwm, 
					uint8_t mode);
/**
 * \brief Return the current value of the first semi-cycle for 
 *        this pwm
 * If the handle is invalid this will assert. 
 * \param[in] pwm_handle The handle returned from the open call 
 * \returns uint16_t 
 * \retval The current value of the first semi-cycle
 */

uint16_t pwm_get_first_semicycle(const pwm_t *pwm_handle);
/**
 * \brief Return the current value of the second semi-cycle for 
 *        this pwm
 * If the handle is invalid this will assert. 
 * \param[in] pwm_handle The handle returned from the open call 
 * \returns uint16_t 
 * \retval The current value of the second semi-cycle 
 *  
 */

uint16_t pwm_get_second_semicycle(const pwm_t *pwm_handle);

#ifdef __cplusplus
}
#endif
#endif 

