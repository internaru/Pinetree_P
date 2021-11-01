/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_INTF_API_H
#define NET_INTF_API_H

#include <net/if.h> // if_nametoindex(), if_indextoname()

#include "os_network.h"
#include "error_types.h"
#include "net_api.h"
#include "net_iface_config.h" // oem_config

// current dynamic os-host settings
char* os_get_hostname(char* given_name_buf); // returns current hostname
char* get_intf_name(int interface_index); // interface name str
char* get_intf_addr(char* interface_name, int protocol_family); // ip addr str -- returns NULL if currently unbound
char* get_intf_ipv4_addr(char* interface_by_name);
char* get_intf_ipv6_addr(char* interface_by_name);
uint8_t* get_intf_mac_addr(char* interface_name, uint8_t* buf); // mac addr returned in buf

// debug prototypes
void show_intfs(void);

#endif // NET_INTF_API_H

