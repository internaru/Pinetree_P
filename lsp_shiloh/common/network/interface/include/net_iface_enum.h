/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_iface_enum.h
 * 
 * @brief Enum definitions for the Network Interface layer
 *
 * This file defines a set of enumerated literals
 */

#ifndef NET_IFACE_ENUMS_H
#define NET_IFACE_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Network interface variable IDs
 * 
 * An enumeration of all persistent variables exposed by the Network Interface layer.
 * 
 * @note Includes both RAM and NVRAM based persistent variables. Does not include variables
 *       calculated on-the-fly with no associated persistent storage. The values of volatile
 *       RAM-based variables are restored to default values after a power cycle.
 */
typedef enum
{
    NET_IFACE_VAR_ZERO_BASE_SKIPPED = 0, ///< push 1st 0-based -> valid 1st 1-based
    NET_IFACE_VAR_NETWORK_STATUS, ///< network status
    NET_IFACE_VAR_IPV4_ADDRESS, ///< ipv4 address
    NET_IFACE_VAR_HOSTNAME_ORIGIN, ///< hostname_origin variable ID.
    NET_IFACE_VAR_DOMAIN_NAME_ORIGIN, ///< domain_name variable ID.
    NET_IFACE_VAR_MANUAL_HOSTNAME, ///< manual_hostname variable ID.
    NET_IFACE_VAR_MANUAL_DOMAIN_NAME, ///< manual_domain_name variable ID.
    NET_IFACE_VAR_IPV4_ENABLED, ///< ipv4_enabled variable ID.
    NET_IFACE_VAR_IPV4_CONFIG_OPTIONS, ///< ipv4_config_options variable ID.
    NET_IFACE_VAR_IPV4_LAST_CONFIG_METHOD, ///< ipv4_last_config_method variable ID.
    NET_IFACE_VAR_IPV4_MANUAL_ADDR, ///< ipv4_manual_addr variable ID.
    NET_IFACE_VAR_IPV4_MANUAL_SUBNET_MASK, ///< ipv4_manual_subnet_mask variable ID.
    NET_IFACE_VAR_IPV4_MANUAL_GATEWAY, ///< ipv4_manual_gateway variable ID.
    NET_IFACE_VAR_IPV4_MANUAL_WINS_ADDR, ///< ipv4_manual_wins_addr variable ID.
    NET_IFACE_VAR_IPV4_AUTOIP_ADDR, ///< ipv4_autoip_addr variable ID.
    NET_IFACE_VAR_IPV4_MANUAL_DNS_PREFERRED_ADDR, ///< ipv4_manual_dns_preferred_addr variable ID.
    NET_IFACE_VAR_IPV4_MANUAL_DNS_ALTERNATE_ADDR, ///< ipv4_manual_dns_alternate_addr variable ID.
#ifdef HAVE_IPV6
    NET_IFACE_VAR_IPV6_ENABLED, ///< ipv6_enabled variable ID.
    NET_IFACE_VAR_IPV6_CONFIG_OPTIONS, ///< ipv6_config_options variable ID.
    NET_IFACE_VAR_IPV6_MANUAL_ADDR, ///< ipv6_manual_addr variable ID.
#endif // HAVE_IPV6

#ifdef TM_USE_PPPOE_CLIENT
    NET_IFACE_VAR_PPPOE_ENABLED,  ///< Is the PPPoE client connection enabled on this interface?
    NET_IFACE_VAR_PPPOE_USERNAME, ///< Username for connecting to a PPPoE Server
    NET_IFACE_VAR_PPPOE_PASSWORD, ///< Password for connecting to a PPPoE Server
#endif // TM_USE_PPPOE_CLIENT

    NET_IFACE_VAR_INTF_ENABLED, ///< Is the interface enabled (independent of status, presence, ipv4/6 states..)
    // add new vars above this line

    NET_IFACE_NUM_VARS, ///< A count of the number of network interface variables.
    NET_IFACE_VAR_ALL ///< Special network interface variable ID meaning "all variables".
} net_iface_var_id_t;

#ifdef __cplusplus
}
#endif

#endif // NET_IFACE_ENUMS_H

