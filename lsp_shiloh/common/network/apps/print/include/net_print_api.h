/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_print_api.h
 * 
 * @brief Network Application for TCP Raw Port Printing 
 * 
 */

#ifndef NET_PRINT_API_H
#define NET_PRINT_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "io_device.h"

/**
 * @name Net Print return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
#define NET_PRINT_OK            OK
#define NET_PRINT_FATAL_ERROR   -11  ///< An unrecoverable error occurred. Operation will likely continue to fail on repeated attempts.
#define NET_PRINT_LOW_MEM       -12  ///< The operation failed because system memory is low. A future attempt may succeed. 
#define NET_PRINT_WOULDBLOCK    -13
//@}

/**
 * @brief Initialize deamon to listen for incoming job requests on the raw TCP port
 *
 * @retval NET_PRINT_SUCCESS Initialization was successful.
 * 
 * @retval NET_PRINT_LOW_MEM Initialization failed because system memory is low. A future attempt may
 *         succeed.
 * 
 * @retval NET_PRINT_FATAL_ERROR Initialization failed because an unrecoverable error has occurred.
 * 
 * This routine should be called exactly once following a system startup.
 */
int32_t net_print_init(void);

#ifdef __cplusplus
}
#endif

#endif // NET_PRINT_API_H
