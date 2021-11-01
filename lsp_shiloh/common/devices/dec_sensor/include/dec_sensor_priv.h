/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/// \file dec_sensor_priv.h


#ifndef dec_sensor_priv_h
#define dec_sensor_priv_h
#include <stdint.h>
#include <stdbool.h>
#include "regAddrs.h"
#include "DEC_CONFIG_regmasks.h"
#include "DEC_CONFIG_regstructs.h"
#include "dec_sensor.h"
#include "SENSOR_regstructs.h"
#include "SENSOR_regmasks.h"
#include "posix_ostools.h"
#include "gpio_api.h"
#include "logger.h"
#include "devices_logger.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Local initialization function must be specialized per product.
///

#define DBG_PRFX "SENSOR: "                      // simple string prepend to the front of all strings.
//#define LOGGER_SUBMOD 0x4000              // (0x1 << 0x14) submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_DEC_SENSOR

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)          // LOG_ERR is the level 
#define DBG_DEBUG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 
#define DBG_LOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 
void dec_sensor_local_init(void);

#define DEC_SENSOR_REGS_t SENSOR_REGS_REGS_t

/// Private structure used between the sensor implementation
/// and the sensor configuration.
///
///
struct dec_sensor_s 
{
    uint32_t pin_number;  ///< matches the gpio pin number.
    /*Init end*/
    void (*callback)(void *callback_private_data ); ///< Used for blocking delay
    void *callback_private_data; ///< Used for blocking delay
    void (*cb)(void *cb_private_data ); ///< User supplied function
    void *cb_private_data; ///< User supplied data
    sem_t semaphore; ///< semaphore for isr to trigger for 
    volatile uint32_t *cfg; ///< hw cfg register
    volatile uint32_t *fthresh; ///< hw cfg register
    volatile uint32_t *frise; ///< hw cfg register
    volatile uint32_t *ffall; ///< hw cfg register
    bool active;  ///< is the sensor active
    DEC_SENSOR_REGS_t *sensors; ///< hw registers.
    gpio_handle_t *gpio;
};

dec_sensor_t *
dec_sensor_create_gpio(uint32_t num,
                       gpio_bank_t bank, 
                       uint32_t pin,
                       gpio_logic_setting_t sense);


/**
    *   Selecting Values 
    *   fall_low_weight, fall_hi_weight, and threshold define the "fall" transition behavior from high to low.
    *   rise_low_weight, rise_hi_weight, and threshold define the "rise" transition behavior from low to high.
    *   The basic HW algorithm for determining when to transition to the current logic state to the other
    *   logic state is as follows
    *
    *   uint8_t sum = 0;
    *   if (<current_transition> == fall) //this means <current_state> = high
    *       current_sensor_state = high;
    *   else  // <current_transition> = rise, <current_state> = low 
    *       current_sensor_state = low;
    *   {
    *       wait_for_timebase();
    *       sampled_state = raw_state_of_sensor();
    *       if (sampled_state == current_sensor_state)                  //
    *       {                                                           //So if we are in fall state it would be
    *           if (sum > <current_transition>_<current_state>_weight)  //fall_high_weight
    *               sum -= <current_transition>_<current_state>_weight;
    *           else
    *               sum = 0;
    *       }
    *       else
    *       {
    *           if (sum + <current_transition>_<not_current_state>_weight >= threshold)
    *               break;
    *           else
    *               sum += <current_transition>_<not_current_state>_weight;
    *       }
    *   } while (true);
    *   handle_state_transition(<not_current_state>);  
    *
    *   Example one
    *
    *   If you want a logic transistion to occur after 5 samples have all been at the 
    *   new logic state.  This implies that any sample at the current state will reset the count.
    *   the setting would be. 
    *
    *   fall_low_weight = 1, current state high, sampled state low, add 1 to sum.
    *   fall_hi_weight = 5, current state high, sampled state high, reset sum to 0.
    *   rise_low_weight = 5, current state low, sampled state low, reset sum to 0.
    *   rise_hi_weight = 1, current state low, sampled state high, add 1 to sum.
    *   threshold, need 5 in row to change state.  
    *
    *   Example Two 
    *
    *   If you have a fairly noisy rising edge and a low noise on the falling edge, then the sensor would be
    *   programmed with numbers like this.
    * 
    *   fall_low_weight = 4, current state high, sampled state low, add 4 to sum.
    *   fall_hi_weight = 7, current state high, sampled state high, reset sum to 0.
    *   rise_low_weight = 2, current state low, sampled state low, decrement sum by 2.
    *   rise_hi_weight = 1, current state low, sampled state high, add 1 to sum.
    *   threshold = 7, need 2 in row to change 1->0 state, 
    *                  need a window where (#(1)-2*#(0) == 7) to change state 0->1. 
    *
    *   Example below
    *
    *   If you can tolerate some noise/bounce, but you require the sensor to have stablized on the new state.
    *
    *   fall_low_weight = 1, current state high, sampled state low, add 1 to sum.
    *   fall_hi_weight = 10, current state high, sampled state high, decrement sum by 10.
    *   rise_low_weight = 10, current state low, sampled state low, decrement sum by 10.
    *   rise_hi_weight = 1, current state low, sampled state high, add 1 to sum.
    *   threshold, need a window where (#(~current_state)-10*#(current_state) == 100) to change state.
    * an interesting article on this subject.
    * http://www.eetimes.com/design/industrial-control/4006414/How-to-Choose-A-Sensible-Sampling-Rate   
*/
uint32_t 
sensor_filter_config(uint32_t fall_low_weight, 
                     uint32_t fall_hi_weight,
                     uint32_t rise_low_weight,
                     uint32_t rise_hi_weight
    );


/** see hw registers for documentation.
 */ 
uint32_t
sensor_config(  uint32_t alt_in,
                uint32_t out_sense,
                uint32_t time_base,
                uint32_t interupt_type,
                uint32_t out_type
    );

void *dec_sensor_get_dev();    
DEC_SENSOR_REGS_t* dec_sensor_get_regs_base();
    
#define SENSOR_ALTIN_Normal 0
#define SENSOR_ALTIN_Alt    1

#define SENSOR_OutputSense_Normal   0
#define SENSOR_OutputSense_Inverted 1  

#define SENSOR_Timebase_Busclks	0
#define SENSOR_Timebase_1us	1	
#define SENSOR_Timebase_10us	2
#define SENSOR_Timebase_100us	3   

#define SENSOR_INTTYPE_NONE	0
#define SENSOR_INTTYPE_Rise	1
#define SENSOR_INTTYPE_Fall	2
#define SENSOR_INTTYPE_Both	3
#define SENSOR_INTTYPE_Low	4
#define SENSOR_INTTYPE_High	5

#define SENSOR_OUTTYPE_Normal	0
#define SENSOR_OUTTYPE_Rise	1
#define SENSOR_OUTTYPE_Fall	2
#define SENSOR_OUTTYPE_Both	3


#ifdef __cplusplus
}
#endif

#endif
