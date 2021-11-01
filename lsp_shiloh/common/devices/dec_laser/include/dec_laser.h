#ifndef DEC_LASER_H
#define DEC_LASER_H
/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/// \file dec_laser.h
/** Direct Engine Control block Laser Control
 *  
 *  Laser control, laser mirror control
 */


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "key_table.h"
#include "error_types.h"
#ifdef HAVE_DEC_SENSOR
#include "dec_sensor.h"
#endif

/// Laser, mirror and motor.
///
typedef struct dec_laser_s dec_laser_t;

/**
 *
 * @brief Initializes the DEC laser internal structures and
 * driver interface.
 *
 */

void dec_laser_init();

/**
 * @brief estimated time to spin up mirror and start laser
 * 
 * @param[in] laser:  Laser to be started.
 * 
 * @return min_delay_ms[out] the expected minimum time in mSec
 *                    that it will take to prepare the laser for
 *                    printing.
 * 
 */

uint32_t dec_laser_eta(dec_laser_t * laser);
/**
 * @brief starts up the laser, non-blocking.
 * 
 * @param[in] laser: laser to be started.
 * 
 * 
 * @return error_type_t OK: start command accepted. else:
 *         failure see below.
 * 
 */
#define DEC_LASER_START_ERROR_START_WHILE_STARTED           -1001 /*< programmer error */
#define DEC_LASER_START_ERROR_STARTING_UNKNOWN_MOTOR_ERROR  -1002  /*< Unknown motor error */

error_type_t dec_laser_start(dec_laser_t *laser);

/**
 * @brief blocks until Laser to finish starting up.
 * 
 * @param[in] laser: Laser being started
 * 
 * @return error_type_t true: fail, WD popped either mechanical
 *         failure or WD time too short. 0K = good.
 */
#define DEC_LASER_WAIT_ERROR_WATCHDOG_EXPIRED                -1003 /*< WD too short or Unknown motor error */

error_type_t dec_laser_wait(dec_laser_t *laser);

/**
 * \brief turns video on
 * 
 * \param laser [in] First laser.
 * 
 * \param on_notoff [in] true turns on, false turns off.
 */
void dec_enable_vdo(dec_laser_t * laser, bool on_notoff);
#ifdef HAVE_DEC_SENSOR
/**
 * \brief turns video on
 * 
 * \param laser [in] First laser.
 * 
 * \param on_notoff [in] true turns on, false turns off.
 */
void dec_enable_vdo_top_sensor(dec_laser_t * laser, dec_sensor_t *sensor, bool on_notoff);
#endif
/**
 * \brief turns off laser
 * 
 * \param dec_laser[in] laser to turned off.
 */

void dec_stop_laser_mirror(dec_laser_t * dec_laser);

bool spinning_mirror_lock(dec_laser_t * dec_laser); // namja
void mirror_lock_clear(dec_laser_t * dec_laser); // namja


uint32_t dec_laser_target_period(dec_laser_t *laser);

uint32_t dec_laser_period( dec_laser_t *laser, uint32_t *baseline);

#if 1  // cut n paste expansion of key_table interface for dec_laser_t

 /// Get static non-owned object (dec_laser_t *) pointer by name.
 dec_laser_t * dec_laser_by_name( const char * name );

 /// Get static non-owned object (dec_laser_t *) pointer by zero based array index.
 dec_laser_t * dec_laser_by_index( unsigned index );

 /// Get name of object of type (dec_laser_t *) at zero based array index provided.
 const char * dec_laser_name_by_index( unsigned index ); 

 /// Get index of dec_laser by name.
 unsigned dec_laser_index_by_name( const char * name ); 

 /// Get name of dec_laser via dec_laser_t *pointer.
 const char * dec_laser_name_by_dec_laser_t( dec_laser_t *obj );

 void dec_laser_set_top_sync( dec_laser_t *laser, bool set );


#else // macro expansion of key_table interface for dec_laser_t
 KEY_TABLE_DECLARE_PUBLIC_2( dec_laser, dec_laser_t );
#endif


#ifdef __cplusplus
}
#endif

#endif
