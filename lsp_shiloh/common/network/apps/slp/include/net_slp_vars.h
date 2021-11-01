/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_slp_vars.h
 * 
 * @brief User Interface API for the Service Location Protocol (SLP) network application
 * 
 * This API exposes a set of variables (i.e. properties) representing a high-level view of the
 * Service Location Protocol (SLP) network application. Use this API to access SLP configuration
 * and status information for use in system User-Interface layers or other high-level system
 * components.
 *
 * Some variable values are stored in NVRAM and persist across power cycles. The Lifetime of each
 * value is indicated in the detailed variable descriptions below. Default values are defined in the
 * SLP platform configuration file.
 */

#ifndef NET_SLP_VARS_H
#define NET_SLP_VARS_H

#include <stdint.h>
#include "error_types.h"
#include "data_access_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Return codes
 *
 * @note Error code values 0 through -10 are reserved in error_types.h.
 */
#define NET_SLP_OK          OK
#define NET_SLP_FAIL        FAIL
//@}

//=========================================================================================================================
/// @name SLP variables
//=========================================================================================================================

/**
 * @brief Get the current value of the SLP "enabled" variable.
 *
 * @param[out] enabled Pointer to storage allocated by caller used to pass back the current variable
 *       value.
 *
 * A value of "true" indicates that the SLP responder is active and registered device services are
 * discoverable via SLPv1 and SLPv2 queries. A value of "false" indicates that the SLP responder is
 * not currently active.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_slp_get_var_enabled(bool *enabled);

/**
 * Set the current value of the SLP "enabled" variable.
 *
 * @param enabled The new variable value.
 *
 * A value of "true" enables the SLP responder (if previously disabled). A value of "false" disables
 * the SLP responder (if previously enabled).
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_slp_set_var_enabled(bool enabled);

#ifdef __cplusplus
}
#endif

#endif // NET_SLP_VARS_H

