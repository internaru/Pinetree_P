/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_IFACE_CONFIG_H
#define NET_IFACE_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "net_iface_vars.h"


//=========================================================================================================================
// Enumerated network interface config (i.e. per interface instance)
//=========================================================================================================================

typedef net_rcode_t (*net_iface_instance_oem_init_func_t)(void);

typedef struct
{
    net_iface_instance_oem_init_func_t init;
} net_iface_instance_oem_func_table_t;

typedef struct
{
    net_iface_instance_oem_func_table_t *func_table;
    char name[NET_IFACE_NAME_SIZE];

    // the following items represent the default interface configuration values
    net_iface_config_origin_t       hostname_origin;
    net_iface_config_origin_t       domain_name_origin;
    char                            manual_hostname[NET_IFACE_HOSTNAME_SIZE];
    char                            manual_domain_name[NET_IFACE_DOMAINNAME_SIZE];
    bool                            ipv4_enabled;
    net_iface_ipv4_config_options_t ipv4_config_options;
    uint32_t                        ipv4_manual_addr;
    uint32_t                        ipv4_manual_subnet_mask;
    uint32_t                        ipv4_manual_gateway;
    uint32_t                        ipv4_manual_wins_addr;
    uint32_t                        ipv4_manual_dns_preferred_addr;
    uint32_t                        ipv4_manual_dns_alternate_addr;
#ifdef HAVE_IPV6
    bool                            ipv6_enabled;
    net_iface_ipv6_config_options_t ipv6_config_options;
    net_iface_ipv6_addr_t           ipv6_manual_addr;
#endif // HAVE_IPV6
    bool                            intf_enabled;
} net_iface_instance_oem_config_t;


//=========================================================================================================================
// General network interface config
//=========================================================================================================================

typedef net_rcode_t (*net_iface_oem_init_func_t)(void);
typedef net_rcode_t (*net_iface_instance_get_oem_config_func_t)(uint8_t iface_enum, net_iface_instance_oem_config_t *instance_oem_config);

typedef struct
{
    net_iface_oem_init_func_t init;
    net_iface_instance_get_oem_config_func_t instance_get_oem_config;
} net_iface_oem_func_table_t;

/**
 * @param[in] iface_name Pointer to a null-terminated character string representing the 
 *                 name of the interface to which the link should be attached (e.g.
 *                 "eth0"). The interface will automatically be created during link
 *                 enumeration. If multiple links use the same interface name then the
 *                 links will share the interface and link priority will determine which
 *                 link is bound to the interface when multiple links are up
 *                 simultaneously. Upon return the caller may free the memory used to
 *                 store the string.
 */
typedef struct
{        
    net_iface_oem_func_table_t      *func_table;
    uint8_t                         iface_cnt;
} net_iface_oem_config_t;


//=========================================================================================================================
// Public OEM config API
//=========================================================================================================================

net_rcode_t net_iface_get_oem_config(net_iface_oem_config_t *oem_config);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // NET_IFACE_CONFIG_H

//eof
