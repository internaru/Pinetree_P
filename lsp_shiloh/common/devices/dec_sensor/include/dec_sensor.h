#ifndef DEC_SENSOR_H
#define DEC_SENSOR_H
/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** \file dec_sensor.h 
 * Direct Engine Control block Sensor Block.
 *
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "DEC_CONFIG_regmasks.h"
#include "DEC_CONFIG_regstructs.h"
#include "key_table.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dec_sensor_s dec_sensor_t;

/// init dec sensors with everything off 1 time call at system startup.
///
void dec_sensor_init(void);

/// Construct a single sensor.
dec_sensor_t * ///< null on failure
dec_sensor_create(uint32_t num,  //< 0 to max number of sensors
                  uint32_t fcfg, //< Filter min/max values
                  uint32_t fthresh, //< filter threshhold value
                  uint32_t cfg, //< comlete register replace
                  void(*callback)( void *callback_private_data ) //< on interrupt
    );
///returns the pin_number of sensor ONLY WORKS for sensor not for GPIO!!!!
uint32_t
dec_sensor_pin_number( dec_sensor_t *sensor );

void dec_sensor_cb_add(dec_sensor_t * sensor,void(*cb)( void *cb_private_data ),void *cb_private_data);
/// enables a single sensor's interrupt
void dec_sensor_enable_isr( dec_sensor_t *sensor );
/// disables a single sensor's interrupt
void dec_sensor_disable_isr( dec_sensor_t *sensor );

/// poll the state of a sensor
bool dec_sensor_poll(const dec_sensor_t *sensor);
//bool dec_sensor_poll_num(uint32_t i);


/// block waiting for sensor to trigger, 
int ///< 0 on success, positive timeout_ticks on timeout, negative on error, bad params.
dec_sensor_block(
    dec_sensor_t *sensor, ///< Sensor to wait on.
    uint32_t timeout_ticks  ///< number of OS ticks to wait. -1 is wait forever
    );

/// unblock semaphore from isr.
void dec_sensor_unblock( void *cb_data );

/// block waiting for sensor to reach the requested state.
/// works for both normal and gpio sensors
int ///< 0 on success, positive timeout_ticks on timeout, negative on error, bad params
dec_sensor_block_state(
    dec_sensor_t *sensor,   ///< Sensor to wait on.
    bool sensor_state,      ///< requested state
    uint32_t timeout_ticks  ///< number of OS ticks to wait. -1 is wait forever
    );

/// int key - string - object mapping, from dec_key_table
KEY_TABLE_DECLARE_PUBLIC_3( dec, sensor, dec_sensor_t );

#ifdef __cplusplus
}
#endif

#endif 
