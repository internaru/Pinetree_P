/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

#include <stdint.h>

/**
 * \file error_types.h
 *
 * \brief Define the error_types_t type so that all modules may return 
 * the same kind of error variable.
 *
 * error_type_t is a global error return variable.  All modules that use this 
 * all handled through the event reporting module so this type is a prototype for
 * doing subsystem errors. All modules that use error_type_t
 * shall extend this with their own errors using the following guidelines.
 *
 * -# Use the defined returns of SYS_OK and SYS_FAIL 
 * -# Reserve 0 through -10 for the system
 * -# Error numbers are locally defined.  Should not span subsystems
 * -# Errors are all negative numbers of < -10
 * -# Positive numbers may be used however the module decides
 * -# Subsystems should consider a consistent error numbering scheme.
 * -# Subsystems should put a unique prefix on their errors for easy identification.
 * For example, SPI_OUT_OF_MEMORY or USB_DEVICE_DISCONNECTED
*/

// \typedef Error type define
typedef int32_t error_type_t;

#define SYS_OK 0    ///< Unique in the system
#define SYS_FAIL -1 ///< Unique in the system

// *** DEPRECATED - DO NOT USE ***
// OK and FAIL are too generic and collide with various third-party sources (e.g. FAIL is defined as 0)
#ifndef OK
#define OK 0    ///< Unique in the system
#endif
#ifndef FAIL
#define FAIL -1 ///< Unique in the system
#endif

#endif
