#ifndef PWM_PRIV_H
#define PWM_PRIV_H
/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** \file pwm_priv.h 
 * Digital Pulse Width Modulation, control private header file
 *  only used by the implementation.
 *  
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "logger.h"
#include "devices_logger.h"
#include "pwm_api.h"
#include "DPWM_regmasks.h"
#include "DPWM_regstructs.h"


#define DBG_PRFX "PWM: "         // simple string prepend to the front of all strings.
//#define LOGGER_SUBMOD 0x10000  // (0x1<<16) DEVICES:PWM  submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_PWM

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)          // LOG_ERR is the level   
#define DBG_DEBUG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 

/// Constuct a pwm with 2**width period high then low ----____ 
/// allowing the client to change the duty cycle.
/// board specific code will instantiate as many pwn's as needed using 
/// constructors.
///
/// @return null or pointer to digital PWM object.
pwm_t * 
pwm_simple_construct(
    uint32_t pwm_block_num, ///< register block number in the asic
    uint32_t width, ///< 16 bit max pwm counter, 8  is common.
    dpwm_timebase_e timebase ///< time base signal, period
    );

/// board specialized pwm initialization code must be 
/// provided.
void pwm_local_init();


/// Allows a more complex pwm wave than the simple dutycycle
/// not limited to power of 2 waveform periods.
/// allows delay after setting before starting waveform.
/// 
/// If this is constructed then the users dpwm_run function 
/// is really an on off with the pre-configured settings,
/// unless the dpwm_run_fancy interface is used, to reset
/// the settings on each use.
/// 
/// @return a valid object or null on allocation failure.
pwm_t *
pwm_config_construct(
    uint32_t pwm_block_num, ///< register block number in the asic
    bool first_cycle_high,  ///< 1st high / 2nd low, or reversed.
    uint16_t delay_cnt,     ///< low delay clks, before pwm starts.
    uint16_t first_semicycle, ///< number of clocks in first cycle.
    uint16_t second_semicycle,///< number of clocks in second cycle.   
    bool off_is_low,          ///< true if off is low, false if off is high.
    dpwm_timebase_e timebase  ///< size of a clock tick 
    );

void *dec_pwm_get_regs_base(void);
void *dec_config_get_regs_base(void);

#ifdef __cplusplus

}
#endif

#endif 
