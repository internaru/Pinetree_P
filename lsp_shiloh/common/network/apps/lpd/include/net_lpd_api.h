/**
 * @file net_lpd_api.h
 * 
 * @brief Network Application for TCP LPD Port Printing 
 * 
 */

#ifndef NET_LPD_API_H
#define NET_LPD_API_H

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
#define NET_LPD_PORT           515
#define NET_LPD_IO_OK          OK
#define NET_LPD_FATAL_ERROR   -11  ///< An unrecoverable error occurred. Operation will likely continue to fail on repeated attempts.
#define NET_LPD_LOW_MEM       -12  ///< The operation failed because system memory is low. A future attempt may succeed. 
#define NET_LPD_WOULDBLOCK    -13


/**
 * @brief Initialize deamon to listen for incoming job requests on the LPD TCP port
 *
 * @retval NET_LPD_SUCCESS Initialization was successful.
 * 
 * @retval NET_LPD_LOW_MEM Initialization failed because system memory is low. A future attempt may
 *         succeed.
 * 
 * @retval NET_LPD_FATAL_ERROR Initialization failed because an unrecoverable error has occurred.
 * 
 * This routine should be called exactly once following a system startup.
 */
int32_t net_lpd_init(void);

#ifdef __cplusplus
}
#endif

#endif // NET_LPD_API_H
