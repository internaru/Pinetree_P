/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_raw_io_api.h
 * 
 * @brief Network layer for raw port I/O applications
 * 
 */

#ifndef NET_RAW_IO_API_H
#define NET_RAW_IO_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "io_device.h"

/**
 * @name Net Raw I/O return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
#define NET_RAW_IO_OK              OK
#define NET_RAW_IO_FATAL_ERROR     -11  ///< An unrecoverable error occurred. Operation will likely continue to fail on repeated attempts.
#define NET_RAW_IO_LOW_MEM         -12  ///< The operation failed because system memory is low. A future attempt may succeed. 
#define NET_RAW_IO_WOULDBLOCK      -13
//@}

// callback for custom client configuration of listen socket
// return true if setting sock options was succussful, false otherwise
typedef bool (*net_raw_io_sock_config_func_ptr_t)(int listen_sock);

    
/**
 * @brief Initialize the Network raw port I/O layer.
 *
 * @retval NET_RAW_IO_OK Initialization was successful.
 * 
 * @retval NET_RAW_IO_LOW_MEM Initialization failed because system memory is low. A future attempt may
 *         succeed.
 * 
 * @retval NET_RAW_IO_FATAL_ERROR Initialization failed because an unrecoverable error has occurred.
 * 
 * This routine should be called exactly once following a system startup.
 */
int32_t net_raw_io_init(void);

// sock_config_func_ptr may be NULL if no custom listen sock option configuration is required
int32_t net_raw_io_register(Channel_type_t type, uint16_t port, const char *name,
                            net_raw_io_sock_config_func_ptr_t sock_config_func_ptr);

#ifdef __cplusplus
}
#endif

#endif // NET_RAW_IO_API_H
