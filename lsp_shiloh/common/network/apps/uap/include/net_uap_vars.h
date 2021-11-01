/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_uap_vars.h
 *  
 * @brief Wifi (wpa) var interface API for the Network layer
 *
 * This API exposes a set of variables (i.e. properties) representing a high-level view of the
 * Wireless via wpa_supplicant network application. Use this API to access uap
 * configuration and status information for use in system User-Interface layers
 * or other high-level system components.
 *
 * Some variable values are stored in NVRAM and persist across power cycles. The Lifetime of each
 * value is indicated in the detailed variable descriptions below. Default values are defined in the
 * wifi platform configuration file.
 */

#ifndef _NET_UAP_VARS_H
#define _NET_UAP_VARS_H

#include <stdint.h>
#include <stdbool.h>

#include "error_types.h"
#include "data_access_api.h"

#include "net_uap_api.h"
#include "wifi_intf.h"

#define MAX_UDHCP_ADDRESS_RANGE 30 /* addresses available */

/**
 * @brief wifi network application variable initialization 
 *  
 *   Initializes a set of variables for each specified
 *   interface e.g, multiple calls for multiple interfaces
 */
void net_uap_var_init(uint8_t interface);

/**
 * @brief wifi network application variable IDs
 * 
 * An enumeration of all persistent variables exposed by the wifi network application.
 * 
 * @note Includes both RAM and NVRAM-based persistent variables. Does not include 
 *       variables calculated on-the-fly with no associated persistent storage. The values
 *       of volatile RAM-based variables are restored to default values after a power
 *       cycle.
 */
typedef enum
{
    // wifi vars
    NET_UAP_VAR_HW_ENABLED, ///< wifi hw enabled
    NET_UAP_VAR_ENABLED, ///< wifi enabled
    NET_UAP_VAR_SERVICE_NAME, ///< ssid variable ID.
    NET_UAP_VAR_SSID, ///< ssid variable ID.
    NET_UAP_VAR_IP_BSSID, ///<uint8_t [MAC_ADDR_LENGTH]
    NET_UAP_VAR_DOMAIN_NAME, ///< domain_name variable ID.
    NET_UAP_VAR_IP_ADDR, ///<32-bit v4 address

    NET_UAP_VAR_COMM_MODE, ///<comm mode
    NET_UAP_VAR_MAC_FILTER_MODE, ///<uint_8 mac filter mode
    NET_UAP_VAR_INTF_LOCALE, ///<uint_8 locale
    NET_UAP_VAR_INTF_CHANNEL, ///<uint_8 channel

    // udhcp settings
    NET_UAP_VAR_SERVER_ADDR, ///<32-bit v4 address
    NET_UAP_VAR_SUBNET_MASK, ///<32-bit v4 address mask
    NET_UAP_VAR_LEASE_TIME, ///<32-bit v4 address
    NET_UAP_VAR_STARTING_ADDRESS, ///<32-bit v4 address
    NET_UAP_VAR_NUMBER_OF_ADDRESS, ///<32-bit v4 address
    NET_UAP_VAR_LAST_ADDRESS, ///<32-bit

    // wep
    NET_UAP_VAR_INTF_WEP_CUR_KEY, ///<uint_8 comm mode
    NET_UAP_VAR_INTF_WEP_CUR_KEY_VALUE, ///<uint_8[] 
    NET_UAP_VAR_INTF_WEP_AUTH_MODE, ///<version of a v4 ip address
    NET_UAP_VAR_INTF_WEP_KEY_1, ///< wep key 1
    NET_UAP_VAR_INTF_WEP_KEY_2, ///< wep key 2
    NET_UAP_VAR_INTF_WEP_KEY_3, ///< wep key 3
    NET_UAP_VAR_INTF_WEP_KEY_4, ///< wep key 4

    // wpa
    NET_UAP_VAR_INTF_WPA_ENCR_MODE, ///<version of a v4 ip address
    NET_UAP_VAR_INTF_WPA_PASSPHRASE, ///<version of a v4 ip address
    NET_UAP_VAR_INTF_SEC_MODE, ///<version of a v4 ip address

    // add new vars above this line
    NET_UAP_NUM_VARS, ///< A count of the number of wifi network application variables.
    NET_UAP_VAR_ALL ///< A special variable ID meaning "all variables".
} net_uap_var_id_t;

/**
 * @name Return codes
 *
 * @note Error code values 0 through -10 are reserved in error_types.h.
 */
#define NET_UAP_OK          OK
#define NET_UAP_FAIL        FAIL
//@}

/**
 * @name Maximum buffer sizes in bytes
 */
#define NET_UAP_DOMAIN_NAME_SIZE  64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char
#define NET_UAP_SERVICE_NAME_SIZE  64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char

//=========================================================================================================================
/// @name wifi variables
//=========================================================================================================================

/** 
 *  @brief Restore one or all persistent variables to their default values.
 * 
 *  @param var_id ID of the variable to be restored to its default value, or
 *  NET_UAP_VAR_ALL for all variables.
 *  
 *  @return OK on success or FAIL if write fails.
 * 
 *  Use this function to restore one or all wifi network application RAM and NVRAM-based
 *  persistent variables to their default values as defined in the wifi module's platform
 *  configuration file.
 */
error_type_t net_uap_var_restore_defaults(unsigned int if_index, net_uap_var_id_t var_id);

/**
 * Get the current value of the wifi "enabled" variable.
 * 
 * @param[in] if_index treck index for the interface 
 * @param[out] enabled Pointer to storage allocated by caller used to pass back the current
 *       variable value.
 *
 * @return Always returns NET_UAP_OK.
 *
 * A value of "true" indicates that the wifi responder is active and the device is discoverable by
 * wifi queriers on the network.  A value of "false" indicates that the device is not discoverable
 * by wifi queriers.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t wifi_intf_get_var_is_enabled(uint8_t if_index, bool* enabled);

/**
 * Set the current value of the wifi "enabled" variable.
 *  
 * @param if_index treck index for the interface 
 * @param enabled The new variable value.
 *
 * @return Always returns NET_UAP_OK.
 *
 * This variable's value is persistently stored in NVRAM.
 */
error_type_t wifi_intf_set_var_is_enabled(uint8_t if_index, bool enabled);

/**
 * net_uap_notify_var_ssid_name
 *  
 * @param if_index treck index for the interface 
 *
 * @return void
 *
 * Notify uap of a service name change
 * 
 * This variable's value is persistently stored in NVRAM.
 */
void net_uap_notify_var_ssid_name(unsigned int if_index);

/**
 * net_uap_notify_var_enabled
 *  
 * @param if_index treck index for the interface 
 *
 * @return void
 *
 * Notify uap of a service 'enabled' change
 * 
 * This variable's value is persistently stored in NVRAM.
 */
void net_uap_notify_var_enabled(unsigned int if_index);

error_type_t wifi_intf_get_var_ip_addr(unsigned int if_index, uint32_t* ip_addr);
error_type_t wifi_intf_set_var_ip_addr(unsigned int if_index, uint32_t ip_addr);

error_type_t wifi_intf_get_var_hw_locale(unsigned int active_if, uint8_t* region);   
error_type_t wifi_intf_set_var_hw_locale(unsigned int active_if, uint8_t region);   

// udhcp specific
error_type_t wifi_intf_udhcp_get_var_lease_time(unsigned int if_index, uint32_t* lease_time);
error_type_t wifi_intf_udhcp_set_var_lease_time(unsigned int if_index, uint32_t lease_time);
error_type_t wifi_intf_udhcp_get_var_starting_addr(unsigned int if_index, uint32_t* starting_address);
error_type_t wifi_intf_udhcp_set_var_starting_addr(unsigned int if_index, uint32_t starting_address);
error_type_t wifi_intf_udhcp_get_var_last_addr(unsigned int if_index, uint32_t *last_address);
error_type_t wifi_intf_udhcp_get_var_server_addr(unsigned int if_index, uint32_t* server_address);
error_type_t wifi_intf_udhcp_set_var_server_addr(unsigned int if_index, uint32_t server_address);
error_type_t wifi_intf_udhcp_get_var_subnet_mask(unsigned int if_index, uint32_t* mask);
error_type_t wifi_intf_udhcp_set_var_subnet_mask(unsigned int if_index, uint32_t mask);
error_type_t wifi_intf_udhcp_get_var_num_addrs(unsigned int if_index, uint8_t* address_range);
error_type_t wifi_intf_udhcp_set_var_num_addrs(unsigned int if_index, uint8_t address_range);

// uap var set/getters
error_type_t uap_get_var_is_enabled(uint8_t intf_index, uint32_t* enable);
error_type_t uap_set_var_is_enabled(uint8_t intf_index, uint32_t enable);

error_type_t uap_get_var_ssid(uint8_t intf_index, uint8_t* ssid, uint8_t ssid_len);
error_type_t uap_set_var_ssid(uint8_t intf_index, uint8_t* ssid, uint8_t ssid_len);

#endif // _NET_UAP_VARS_H

//eof
