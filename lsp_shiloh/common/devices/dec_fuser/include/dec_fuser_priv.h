/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef DEC_FUSER_PRIV_H
#define DEC_FUSER_PRIV_H

/// \file dec_fuser_priv.h


#include <stdint.h>
#include "posix_ostools.h"
#include "fuser_regstructs.h"
#include "fuser_regmasks.h"
#include "logger.h"
#include "devices_logger.h"

#include "dec_fuser.h"
#include "gpio_api.h"

#define DBG_PRFX "FUSER: "         // simple string prepend to the front of all strings.
//#define LOGGER_SUBMOD 0x2000  // (0x1<<13) DEVICES:FUSER  submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_DEC_FUSER

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)          // LOG_ERR is the level   
#define DBG_DEBUG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 

#define MAX_FUSERS 3  ///< 3 hw blocks

typedef struct voltage_power_pair_s 
{
    uint32_t v; 
    uint32_t p;
} voltage_power_pair_t;

#define DEC_FUSER_DATATYPE_COOKIE 0x42012345 
struct dec_fuser_temp_s 
{
    uint32_t index;
};

struct dec_fuser_weight_s 
{
    voltage_power_pair_t *table;
    const char * name;
};
struct dec_fuser_s
{
    uint32_t cookie; ///< MUST be DEC_FUSER_DATATYPE_COOKIE
    mqd_t mq;
    uint32_t rcmd;
    uint32_t scmd;
    uint32_t quedata[4]; //< empty should be the norm, more than 1 per 100ms is too fast.
    uint32_t tempHistory[5];
    uint32_t powHistory[5];
    struct dec_fuser_weight_s * paper_weight;
    struct dec_fuser_temp_s * target_temp;
    int32_t target_voltage; //< internal use, set from target_temp/paper_weight.
    void(*at_temp_callback)( void *callback_private_data );
    void *at_temp_data; //< callback data on interrupt.
    void(*at_spin_complete_callback)( void *callback_private_data, uint32_t next_phase ); ///< used when fuser has reached target temp. /* [KSF050201] */
    void *at_spin_complete_data; ///< callback data for next layer up when fuser has reached target temp. /* [KSF050201] */
    void (*fuser_service_callback)( dec_fuser_t *dec_fuser );  ///< fuser instance specific callback to service the fuser
#ifdef USE_ZERO_CROSS_INT
    void (*callback)(void *callback_private_data ); ///< Used for blocking delay
    void *callback_private_data; ///< Used for blocking delay
    void (*cb)(void *cb_private_data ); ///< User supplied function
    void *cb_private_data; ///< User supplied data
    gpio_handle_t *gpio;
#endif //USE_ZERO_CROSS_INT
    sem_t semaphore;
    FUSER_REGS_t *regs;
    uint32_t instance;  ///< which hw block
    uint32_t num_points_curve; ///< number of points per power curve array
    bool syncPWMMode; ///< if true mod is set to DEC_FUSER_Mode_SyncPWM /* [KSF050201] */
    bool fatal_error;  ///< set to handle fuser block asserts
};

typedef struct dec_fuser_config_s
{
    uint32_t output_mux;                ///< output pin mux
    uint32_t power_polarity;            ///< 0 = active low, 1 = active high
    uint32_t power_output_enable;
    uint32_t power_output;
    uint32_t fuser_over_temp_polarity;  ///< 0 = active low, 1 = active high
    uint32_t fuser_over_temp_enable;
    uint32_t fuser_output_polarity;     ///< 0 = active low, 1 = active high
} dec_fuser_config_t;


// bits within the registers...
#define DEC_FUSER_Timebase_Clks	0x0
#define DEC_FUSER_Timebase_1us	0x1
#define DEC_FUSER_Timebase_10us	0x2
#define DEC_FUSER_Timebase_100us 0x3
#define DEC_FUSER_Timebase_1ms	0x4
#define DEC_FUSER_Timebase_10ms	0x5
#define DEC_FUSER_Timebase_100ms 0x6

#define DEC_FUSER_Mode_SyncPWM      0x0
#define DEC_FUSER_Mode_AsyncPWM     0x1
#define DEC_FUSER_Mode_SyncDSM      0x2
#define DEC_FUSER_Mode_AsyncDSM     0x3

/// set the fuser zero crossing registers /* [KSF050201] */
void dec_fuser_set_zero_crossing( 
    dec_fuser_t *dec_fuser,//it doesn't matter which one affects all instances
    uint32_t filter, //< ZCFR
    uint32_t lockout //<ZCLR
    );

/// set the fuser phase config only for Sync* modes currently we only support syncPWM mode /* [KSF050201] */
/// (although syncDSM mode is ifdefed out and works, syncPWM gives better results)
void dec_fuser_set_phase_config( 
    dec_fuser_t *dec_fuser,
    uint32_t pulse_width_uSec,
    uint32_t pulse_offset_uSec
    );

/// set the fuser duty cycle
void dec_fuser_set_duty_cycle( 
    dec_fuser_t *dec_fuser,
    uint32_t duty_cycle //< 0 - 100 int as a duty cycle percentage
    );

/// per board customization 
///
void dec_fuser_local_init(void);

/// private initializer 
///
void dec_fuser_initialize( dec_fuser_t *fuser, uint32_t instance );

#ifdef USE_ZERO_CROSS_INT
dec_fuser_t *
dec_fuser_create_gpio(dec_fuser_t *dec_fuser,
                       gpio_bank_t bank,
                       uint32_t pin,
                       gpio_logic_setting_t sense);
void *dec_fuser_get_dev();
#endif //USE_ZERO_CROSS_INT
FUSER_REGS_t* dec_fuser_get_regs_base();

#endif
