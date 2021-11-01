/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_wifi_vars.h
 *  
 * @brief Wifi (wpa) var interface API for the Network layer
 *
 * This API exposes a set of variables (i.e. properties) representing a high-level view of the
 * Wireless via wpa_supplicant network application. Use this API to access wifi
 * configuration and status information for use in system User-Interface layers
 * or other high-level system components.
 *
 * Some variable values are stored in NVRAM and persist across power cycles. The Lifetime of each
 * value is indicated in the detailed variable descriptions below. Default values are defined in the
 * mDNS platform configuration file.
 */

#ifndef _NET_WIFI_VARS_H
#define _NET_WIFI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#include "error_types.h"
#include "data_access_api.h"
#include "net_wifi_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief mDNS network application variable initialization 
 *  
 *   Initializes a set of variables for each specified
 *   interface e.g, multiple calls for multiple interfaces
 */
void net_wifi_var_init(uint8_t interface);

/**
 * @brief mDNS network application variable IDs
 * 
 * An enumeration of all persistent variables exposed by the mDNS network application.
 * 
 * @note Includes both RAM and NVRAM-based persistent variables. Does not include 
 *       variables calculated on-the-fly with no associated persistent storage. The values
 *       of volatile RAM-based variables are restored to default values after a power
 *       cycle.
 */
typedef enum
{
    // perm vars
    NET_WIFI_VAR_HW_ENABLED, // bool    hw_enabled;
    NET_WIFI_VAR_ENABLED, ///<bool intf enabled variable ID.
    NET_WIFI_VAR_SERVICE_NAME, ///< mDNS service_name variable ID.
    NET_WIFI_VAR_DOMAIN_NAME, ///< mDNS domain_name variable ID.

    NET_WIFI_VAR_SSID_NAME, // char ssid_name[NET_WIFI_SERVICE_NAME_SIZE];
    NET_WIFI_VAR_SSID_PREFIX, // char ssid_prefix[16];
    NET_WIFI_VAR_BSSID, // char bssid[BSSID_LENGTH];

    // intf
    NET_WIFI_VAR_SEC_MODE, // uint8_t sec_mode;
    NET_WIFI_VAR_ENCRYP_MODE, // uint8_t encryp_mode;
    NET_WIFI_VAR_INTF_CHANNEL, // uint8_t channel;
    NET_WIFI_VAR_COMM_MODE, // uint8_t comm_mode;
    NET_WIFI_VAR_HW_LOCALE, // uint8_t hw_locale;
    NET_WIFI_VAR_FILTER_COUNT, // uint8_t filter_count;

    // wpa
    NET_WIFI_VAR_WPA_ENABLED, // bool wpa_enabled;
    NET_WIFI_VAR_WPA_PASSPHRASE, // char wpa_passphrase[128]; // literal size?

    // wep
    NET_WIFI_VAR_WEP_ENABLED, // bool wep_enabled;
    NET_WIFI_VAR_WEP_KEY_CURRENT, // int wep_key_current;
    NET_WIFI_VAR_WEP_KEY_NUM, // int wep_key_num; (redunant w/KEY_CURRENT?)
    NET_WIFI_VAR_WEP_AUTH_MODE, // int wep_auth_mode;
    NET_WIFI_VAR_WEP_KEY_LEN, // int wep_key_len;

    // wep keys as array?
    NET_WIFI_VAR_WEP_KEY_1, // uint8_t wep_key_1[4]; // literal size?
    NET_WIFI_VAR_WEP_KEY_2, // uint8_t wep_key_2[4]; // literal size?
    NET_WIFI_VAR_WEP_KEY_3, // uint8_t wep_key_3[4]; // literal size?
    NET_WIFI_VAR_WEP_KEY_4, // uint8_t wep_key_4[4]; // literal size?

    // rssi
    NET_WIFI_VAR_RSSI_RANGE, // int rssi_range;

    // add new vars above this line
    NET_WIFI_NUM_VARS, ///< A count of the number of mDNS network application variables.
    dupNET_WIFI_VAR_ALL ///< A special variable ID meaning "all variables". // see also wifi_intf_vars.h
} net_wifi_var_id_t;

/**
 * @name Return codes
 *
 * @note Error code values 0 through -10 are reserved in error_types.h.
 */
#define NET_WIFI_OK          OK
#define NET_WIFI_FAIL        FAIL
//@}

/**
 * @name Maximum buffer sizes in bytes
 */
#define NET_WIFI_DOMAIN_NAME_SIZE  64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char
#define NET_WIFI_SERVICE_NAME_SIZE  64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char
//@}

//=========================================================================================================================
/// @name mDNS variables
//=========================================================================================================================

/** 
 *  @brief Restore one or all persistent variables to their default values.
 * 
 *  @param var_id ID of the variable to be restored to its default value, or
 *  NET_WIFI_VAR_ALL for all variables.
 *  
 *  @return OK on success or FAIL if write fails.
 * 
 *  Use this function to restore one or all mDNS network application RAM and NVRAM-based
 *  persistent variables to their default values as defined in the mDNS module's platform
 *  configuration file.
 */
error_type_t net_wifi_var_restore_defaults(unsigned int if_index, net_wifi_var_id_t var_id);

/**
 * Get the current value of the mDNS "enabled" variable.
 * 
 * @param[in] if_index treck index for the interface 
 * @param[out] enabled Pointer to storage allocated by caller used to pass back the current
 *       variable value.
 *
 * @return Always returns NET_WIFI_OK.
 *
 * A value of "true" indicates that the mDNS responder is active and the device is discoverable by
 * mDNS queriers on the network.  A value of "false" indicates that the device is not discoverable
 * by mDNS queriers.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_wifi_get_var_enabled(unsigned int if_index, bool *enabled);

/**
 * Set the current value of the mDNS "enabled" variable.
 *  
 * @param if_index treck index for the interface 
 * @param enabled The new variable value.
 *
 * @return Always returns NET_WIFI_OK.
 *
 * A value of "true" enables the mDNS Responder (if previously disabled) and announces registered
 * Bonjour services to any mDNS queriers listening on the network. A value of "false" disables the
 * mDNS responder (if previously enabled) and sends the final "goodbye" notification to any mDNS
 * queriers listening on the network.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_wifi_set_var_enabled(unsigned int if_index, bool enabled);

/**
 * Get the current value of the mDNS service_name variable.
 *  
 * @param[in] if_index treck index for the interface 
 * @param[out] service_name Pointer to storage allocated by caller used to pass back a
 *       null-terminated service name string.
 *
 * @param len Size in bytes of the service_name buffer. The returned service_name will be no larger
 *       than NET_WIFI_SERVICE_NAME_SIZE (including null terminator).
 *
 * @return Always returns NET_WIFI_OK. If the buffer is too small to hold the entire service name
 *         string then the response will be truncated (but still null terminated).
 *
 * The service_name variable stores a user-friendly name used to further qualify the standard
 * Bonjour service names advertised by the mDNS Responder. The service name is distinguished from
 * the device host name in that the service name is used exclusively for Bonjour service instance
 * enumeration. Instance enumeration helps end users identify a particular service instance when
 * multiple services of the same type are discovered on the local network. The service name is not
 * subject to the usual naming rules for DNS host names (RFC 1034) and may consist of up to 63 bytes
 * of any UTF-8 plus an additional byte for the null terminator.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_wifi_get_var_ssid_name(unsigned int if_index, char *service_name, uint8_t len);

/**
 * Set the current value of the mDNS service_name variable.
 *  
 * @param[in] if_index treck index for the interface 
 * @param[in] service_name Pointer to a null-terminated UTF-8 character string. Upon return, this
 *       storage may be freed by the caller.
 *
 * @return Always returns NET_WIFI_OK. If the service_name is larger than NET_WIFI_SERVICE_NAME_SIZE
 *         bytes then the name will be truncated.
 *
 * Per Bonjour Printing Specification ver 1.0.2, devices advertising one or more printer services
 * must use a service name that is unique on the local network. If this routine assigns a service
 * name that already exists on the local network, a unique identifier will be automatically appended
 * to the assigned service name. In this case, a subsequent access of the service name via
 * net_wifi_get_var_ssid_name() would retrieve the modified service name.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_wifi_set_var_ssid_name(unsigned int if_index, const char *service_name);

#ifdef HAVE_LPR
/**
 * Get the current value of the LPD "enabled" variable.
 * 
 * @param[in] if_index treck index for the interface 
 * @param[out] enabled Pointer to storage allocated by caller used to pass back the current
 *       variable value.
 *
 * @return Always returns NET_WIFI_OK.
 *
 * A value of "true" indicates that the mDNS responder is active and the device is discoverable by
 * mDNS queriers on the network.  A value of "false" indicates that the device is not discoverable
 * by mDNS queriers.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_lpd_get_var_enabled(unsigned int if_index, bool *enabled);

/**
 * Set the current value of the LPD "enabled" variable.
 *  
 * @param if_index treck index for the interface 
 * @param enabled The new variable value.
 *
 * @return Always returns NET_WIFI_OK.
 *
 * A value of "true" enables the mDNS Responder (if previously disabled) and announces registered
 * Bonjour services to any mDNS queriers listening on the network. A value of "false" disables the
 * mDNS responder (if previously enabled) and sends the final "goodbye" notification to any mDNS
 * queriers listening on the network.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t net_lpd_set_var_enabled(unsigned int if_index, bool enabled);

#endif // HAVE_LPR

/**
 * net_wifi_notify_var_ssid_name
 *  
 * @param if_index treck index for the interface 
 *
 * @return void
 *
 * Notify wifi of a service name change
 * 
 * This variable's value is persistently stored in NVRAM.
 */
void net_wifi_notify_var_ssid_name(unsigned int if_index);

/**
 * net_wifi_notify_var_enabled
 *  
 * @param if_index treck index for the interface 
 *
 * @return void
 *
 * Notify wifi of a service 'enabled' change
 * 
 * This variable's value is persistently stored in NVRAM.
 */
void net_wifi_notify_var_enabled(unsigned int if_index);

#ifdef __cplusplus
}
#endif

#endif // _NET_WIFI_VARS_H

