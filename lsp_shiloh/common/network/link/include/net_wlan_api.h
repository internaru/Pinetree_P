/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_WLAN_API_H
#define NET_WLAN_API_H

#include "net_api.h"
#include "net_link_api.h"

typedef struct {
    uint32_t down_link_count;
    uint32_t link_status;
    uint32_t tx_count;
    uint32_t rx_count;
} NET_WLAN_STATS;

/**
 * @brief Initialize the Network WLAN Link layer
 *
 * This routine should be called exactly once following a system startup.
 */
net_rcode_t net_wlan_init(net_link_api_funcs_t *api_funcs);

/**
 * @brief Get the WLAN statistics
 */
NET_WLAN_STATS * net_iface_wlan_get_stats(void);
    
#endif // NET_WLAN_API_H
