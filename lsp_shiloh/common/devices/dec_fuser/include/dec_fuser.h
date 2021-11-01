#ifndef DEC_FUSER_H
#define DEC_FUSER_H
/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** \file dec_fuser.h
 *
 *  Fuser control.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "key_table.h"
#include "spec.h"

typedef struct dec_fuser_s dec_fuser_t;

typedef struct dec_fuser_temp_s dec_fuser_temp_t;

typedef struct dec_fuser_weight_s dec_fuser_weight_t;


//#define USE_ZERO_CROSS_INT //Use to enable Zero-Cross Interrupt

#ifdef USE_ZERO_CROSS_INT
void dec_fuser_cb_add(dec_fuser_t * sensor,void(*cb)( void *cb_private_data ),void *cb_private_data);
/// enables a single fuser's interrupt
void dec_fuser_enable_isr( dec_fuser_t *fuser );
/// disables a single fuser's interrupt
void dec_fuser_disable_isr( dec_fuser_t *fuser );
/// poll the state of a fuser
bool dec_fuser_poll(const dec_fuser_t *fuser);

/// block waiting for fuser to trigger,
int ///< 0 on success, positive timeout_ticks on timeout, negative on error, bad params.
dec_fuser_block(
    dec_fuser_t *fuser, ///< Sensor to wait on.
    uint32_t timeout_ticks  ///< number of OS ticks to wait. -1 is wait forever
    );

/// unblock semaphore from isr.
void dec_fuser_unblock( void *cb_data );

/// block waiting for fuser to reach the requested state.
/// works for both normal and gpio fuser
int ///< 0 on success, positive timeout_ticks on timeout, negative on error, bad params
dec_fuser_block_state(
    dec_fuser_t *fuser,   ///< fuser to wait on.
    bool fuser_state,      ///< requested state
    uint32_t timeout_ticks  ///< number of OS ticks to wait. -1 is wait forever
    );
#endif	//USE_ZERO_CROSS_INT

//#define ADD_ZC

void dec_fuser_set_target_temp(dec_fuser_t *fuser, dec_fuser_temp_t * target_temp);
void dec_fuser_stop(dec_fuser_t *fuser);
bool dec_fuser_spin_phase_exec(dec_fuser_t *fuser, uint32_t phase, uint32_t *min_delay, uint32_t *wd_delay );

void dec_fuser_set_duty_cycle( /* [KSF050201] - start */
    dec_fuser_t *dec_fuser,
    uint32_t duty_cycle //< 0 - 100 int as a duty cycle percentage
    );

/// set the fuser zero crossing registers
void dec_fuser_set_zero_crossing( 
    dec_fuser_t *dec_fuser,//it doesn't matter which one affects all instances
    uint32_t filter, //< ZCFR
    uint32_t lockout //<ZCLR
    );

/// set the fuser phase config only for Sync* modes currently we only support syncPWM mode 
/// (although syncDSM mode is ifdefed out and works, syncPWM gives better results)
void dec_fuser_set_phase_config( 
    dec_fuser_t *dec_fuser,
    uint32_t pulse_width_uSec,
    uint32_t pulse_offset_uSec
    ); /* [KSF050201] - end */

/**
 * @brief Enables the fuser 24V power supply
 * 
 * @param power_on [in] TRUE to turn power on; FALSE to turn 
 *                 power off.
 * 
 */
void dec_fuser_enable_power( bool power_on );

/**
 * @brief set the fuser to a given temperature
 * 
 * @param fuser [in] This fuser
 * 
 * @param e_temp [in] Target temperature.
 */
#if ZX_ENABLE
void dec_fuser_set_temp( 
    dec_fuser_t *fuser, 
    uint32_t target_temp );
#else
void dec_fuser_set_temp( 
    dec_fuser_t *fuser, 
    dec_fuser_temp_t *target_temp );
#endif    

/**
 * @brief Block the current thread until fuser is at temperature.
 * 
 * @param fuser[in] he will block you.
 * 
 */

void dec_fuser_block_till_at_temp( 
    dec_fuser_t *fuser 
    );


/**
 * \brief Set the callback to be notified the fuser hits the target temperature
 *
 * \param fuser[in] the fuser in question.
 *
 * \param target_pow [in] target power
 *
 * \param at_temp_callback [in] users function.
 *
 * \param callback_private_data [in] users data
 */

void dec_fuser_set_at_temp_callback( 
    dec_fuser_t* fuser,
    void(*at_temp_callback)( void *callback_private_data ), 
    void *callback_private_data); 

/**
 * 
 * \brief sets table to use to get the fuser to a given temperature enum.
 * 
 * \param fuser [in] the fuser in question.
 * 
 * \param weight [in] the paper weight.
 * 
 */

void dec_fuser_set_paper_weight(
    dec_fuser_t* fuser,  
    dec_fuser_weight_t * weight  
    );

/**
 * \brief Initialize the fuser at power up.
 */

void dec_fuser_init(void); 

/// int key - string - object mapping, from dec_key_table
KEY_TABLE_DECLARE_PUBLIC_3( dec, fuser, dec_fuser_t );
KEY_TABLE_DECLARE_PUBLIC_2( dec_fuser_temp, dec_fuser_temp_t );
KEY_TABLE_DECLARE_PUBLIC_2( dec_fuser_weight, dec_fuser_weight_t );
#ifdef __cplusplus
}
#endif

#endif 
