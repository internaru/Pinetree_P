/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_eth_api.h
 *  
 * @brief Network Ethernet Link layer
 * 
 * This module implements the link interface defined by net_iface_link_func_table_t. A network link
 * that conforms to this interface definition can register itself with the overlying Treck network
 * stack via net_iface_register_link().
 * 
 * The dispatch table defined by net_iface_link_func_table_t includes function pointer types
 * specific to the Treck network stack API (www.treck.com). The purpose of this module is to map the
 * Treck-specific link interface onto the Ethernet MAC device driver API defined by eth_mac_api.h.
 */

#ifndef NET_ETH_API_H
#define NET_ETH_API_H

#include "net_link_api.h"

/**
 * @brief Initialize the Network Ethernet Link layer
 *
 * This routine should be called exactly once following a system startup. 
 *
 * PREREQUISITES: Requires prior initialization of the Network Interface module.
 */
net_rcode_t net_eth_init(net_link_api_funcs_t *api_funcs);

/**
 * @brief Initialize the NVRAM block for the Network Ethernet Link layer.
 *
 * @param init_type Type of init to perform (use stored values, manufacturer defaults, etc.)
 *
 * This routine should be called exactly once following a system startup.  It initializes the
 * module's persistent variables to either the values previously stored in NVRAM or manufacturer
 * defaults.
 *
 * PREREQUISITES: Requires prior initialization of the NVRAM driver and partition manager.
 */
void net_eth_var_init(void);

#endif // NET_ETH_API_H
