/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_wsd_vars.h
 *  
 * @brief User Interface API for the Network WSD Code
 *
 * This API exposes a set of variables (i.e. properties) representing a high-level view of the
 * Web Services on Devices (WSD) network application. Use this
 * API to access WSD configuration and status information for
 * use in system User-Interface layers or other high-level
 * system components.
 *
 * Some variable values are stored in NVRAM and persist across power cycles. The Lifetime of each
 * value is indicated in the detailed variable descriptions below. Default values are defined in the
 * WSD platform configuration files.
 */


#ifndef NET_WSD_VARS_H
#define NET_WSD_VARS_H

#include <stdint.h>
#include <stdbool.h>
#include <error_types.h>
#include <data_access_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
#define WSD_VARS_OK            OK
#define WSD_VARS_FAIL          FAIL

#define WSD_UUID_LENGTH        37 // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX + NULL

/*******************************************************************************
 *  WSD configuration variables
 ********************************************************************************/

/**
 * @brief Get the wsd enabled state variable
 * 
 * @param[out] enabled Pointer to storage allocated by caller
 *       used to pass back the variable value.
 * 
 * @retval WSD_VARS_OK The variable 'enabled' has been filled
 *         with current value
 * 
 * @retval WSD_VARS_FAIL Failed to obtain lock on variable
 * 
 * This will return whether or not WSD is enabled on the system.
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_wsd_get_var_enabled( bool *enabled );

/**
 * @brief Set whether WSD is enabled or disabled
 * 
 * @param[in] enabled new wsd enabled state
 * 
 * @retval WSD_VARS_OK The new enabled state has been saved to
 *         NVRAM
 * 
 * @retval WSD_VARS_FAIL Failed to obtain lock on variable
 * 
 * This will set whether WSD is enabled or not.  If disabled,
 * all stacks and threads (execpt main loop) will be destroyed
 * 
 * This variable's value is persistently stored in NVRAM.
 * 
 */
error_type_t net_wsd_set_var_enabled( bool enabled );


/**
 * Get the device UUID generated and used by WSD
 *  
 * @param[out] uuid User allocated buffer to store the UUID 
 *       (must be at least 37 bytes)
 * @param[in] length Size of uuid buffer 
 *  
 * @return OK for success, FAIL for error 
 * char* UUID Formatted as XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
 */
error_type_t net_wsd_get_var_uuid( char *uuid, uint32_t length );

/**
 * @brief Set the scanner name
 * 
 * @param[in] new scanner name
 * 
 * @retval WSD_VARS_OK The new scanner name has been saved to
 *         NVRAM
 * 
 * @retval WSD_VARS_FAIL Failed to obtain lock on variable
 * 
 * 
 * This variable's value is persistently stored in NVRAM.
 * 
 */
error_type_t net_wsd_set_var_scanner_name( char* scanner_name );


/**
 * Get the administratively assigned user-friendly name of the scanner
 * 
 * @return char* scanner_name 
 *
 */
char *net_wsd_get_var_scanner_name( );


#ifdef __cplusplus
}
#endif

#endif // NET_WSD_VARS_H

