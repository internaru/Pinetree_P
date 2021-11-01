#ifndef DCMOTOR_H
#define DCMOTOR_H
/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/// \file dcmotor.h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "key_table.h"

/** The dcmotor motor instance(s).
 */
typedef struct dcmotor_s dcmotor_t;

/** Different motor speeds from off to full on.
 */
typedef struct dcmotor_speed_s dcmotor_speed_t;

typedef struct DCMOTOR_BASE_REGS_s MOTOR_REGS;

/// Ramp the given motor up to the speed, 
/// stepper_wait() will unblock when the motor is up to speed
/// speed zero calls will not block 
void dcmotor_run(dcmotor_t *dcmotor, dcmotor_speed_t *speed);

/** Caller must always run then wait to keep in sync,
 *  speed zero calls will not block.
 * @return 0 on success, otherwise its a timeout.
 */
uint32_t dcmotor_wait(dcmotor_t *dcmotor, uint32_t timeout_msecs);

/** Estimate of how long it will take to ramp up to speed in milliseconds.
 *  This can be used to estimate a watchdog on failure to ramp.
 */
uint32_t dcmotor_speed_msecs_to_ramp(dcmotor_t * dcmotor, dcmotor_speed_t * speed);

/**
 * Initializes stepper driver, called at system initialization time.
 */
void dcmotor_init();



#if 1  // cut n paste expansion of key_table interface for dcmotor_t

 /// Get static non-owned object (dcmotor_t *) pointer by name.
 dcmotor_t * dcmotor_by_name( const char *name );

 /// Get static non-owned object (dcmotor_t *) pointer by zero based array index.
 dcmotor_t * dcmotor_by_index( unsigned index );

 /// Get name of object of type (dcmotor_t *) at zero based array index provided.
 const char * dcmotor_name_by_index( unsigned index ); 

 /// Get index of dcmotor by name.
 unsigned dcmotor_index_by_name( const char *name ); 

 /// Get name of dcmotor via stepper_t *pointer.
 const char * dcmotor_name_by_stepper_t( dcmotor_t *obj );

#else // macro expansion of key_table interface for dcmotor_t
 KEY_TABLE_DECLARE_PUBLIC_2( dcmotor, dcmotor_t );
#endif




#if 1  // cut n paste expansion of key_table interface for stepper_speed_t

 /// Get static non-owned object (dcmotor_speed_t *) pointer by name.
 dcmotor_speed_t * dcmotor_speed_by_name( const char * name );

 /// Get static non-owned object (dcmotor_speed_t *) pointer by zero based array index.
 dcmotor_speed_t * dcmotor_speed_by_index( unsigned index );

 /// Get name of object of type (dcmotor_speed_t *) at zero based array index provided.
 const char * dcmotor_speed_name_by_index( unsigned index ); 

 /// Get index of dcmotor_speed by name.
 unsigned dcmotor_speed_index_by_name( const char * name ); 

 /// Get name of dcmotor_speed via dcmotor_speed_t *pointer.
 const char * dcmotor_speed_name_by_dcmotor_speed_t( dcmotor_speed_t *obj );

#else // macro expansion of key_table interface for dcmotor_speed_t
 KEY_TABLE_DECLARE_PUBLIC_2( dcmotor_speed, dcmotor_speed_t );
#endif


#ifdef __cplusplus
}
#endif
#endif
