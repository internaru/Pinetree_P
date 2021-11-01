/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_wsd_api.h
 *
 * @brief WSD API layer
 *
 * This API provides a method to initialize and start WSD on a
 * system.  WSD requires that the network be configured as well
 * as having WSD enabled.
 */
#ifndef NET_WSD_API_H
#define NET_WSD_API_H

#include <stdint.h>
#include <stdbool.h>
#include "net_wsd_vars.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name WSD return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
#define NET_WSD_SUCCESS          OK
#define NET_WSD_FATAL_ERROR      -11  ///< An unrecoverable error occurred. Operation will likely continue to fail on repeated attempts.
#define NET_WSD_RESOURCE_FAILURE -12  ///< The operation failed because a required resource was unavailable.  A future attempt may succeed. 
#define NET_WSD_LOW_MEM          -13  ///< The operation failed because system memory is low. A future attempt may succeed. 
#define NET_WSD_NOT_PERMITTED    -14  ///< The operation is not permitted at this time.

//@}

/**
 * @brief Initialize the WSD code.
 *
 * This routine should be called exactly once following a system
 * startup.  It initializes a thread and set of event flags
 * which in turn can start the entire set of WSD threads
 * depending on the system configuration.
 */
void net_wsd_init( void );

void wsd_print_init( void );

/**
 * @brief Initialize the WSD NVRAM block.
 *
 * @param[in] init_type Type of init to perform (use stored values, manufacturer defaults, etc.)
 *
 * This routine should be called exactly once following a system startup.  It initializes the
 * module's persistent variables to either the values previously stored in NVRAM or manufacturer
 * defaults.
 *
 * PREREQUISITES: Requires prior initialization of the NVRAM driver and partition manager.
 */
void net_wsd_var_init( void );


#ifdef __cplusplus
}
#endif

#endif // NET_WSD_API_H

