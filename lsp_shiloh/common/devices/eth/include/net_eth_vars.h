/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_eth_vars.h
 * 
 * @brief User Interface API for the Network Ethernet Link layer
 *
 * This API exposes a set of variables (i.e. properties) representing a high-level view of the
 * Network Ethernet Link layer. Use this API to access Ethernet link configuration and status
 * information for use in system User-Interface layers or other high-level system components.
 *
 * Some variable values are stored in NVRAM and persist across power cycles. The Lifetime of each
 * value is indicated in the detailed variable descriptions below. Default values are defined in the
 * Network Ethernet Link layer platform configuration file.
 */

#ifndef NET_ETH_VARS_H
#define NET_ETH_VARS_H

#include "error_types.h"
#include "eth_mac_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Variable IDs for Ethernet link layer
 * 
 * An enumeration of all persistent variables exposed by the Ethernet link layer.
 * 
 * @note Includes both RAM and NVRAM-based persistent variables. Does not include 
 *       variables calculated on-the-fly with no associated persistent storage. The values
 *       of volatile RAM-based variables are restored to default values after a power
 *       cycle.
 */
typedef enum
{
    // perm vars
    NET_ETH_VAR_LINK_CONFIG,

    // temp vars

    // add new vars above this line
    NET_ETH_NUM_VARS, ///< A count of the number of variables. 
    NET_ETH_VAR_ALL ///< A special variable ID meaning "all variables".                 
} net_eth_var_id_t;

/**
 * @name Return codes
 *
 * @note Error code values 0 through -10 are reserved in error_types.h.
 */
#define NET_ETH_OK          OK
#define NET_ETH_FAIL        FAIL
#define NET_ETH_WOULDBLOCK  1
#define NET_ETH_INPROGRESS  2
//@}


//=========================================================================================================================
// Ethernet link configuration variables
//=========================================================================================================================

/** 
 *  @brief Restore one or all persistent variables to their default values.
 * 
 *  @param var_id ID of the variable to be restored to its default value, or
 *  NET_ETH_VAR_ALL for all variables.
 *  
 *  @return OK on success or FAIL if write fails.
 * 
 *  Use this function to restore one or all of the Ethernet link layer RAM and NVRAM-based
 *  persistent variables to their default values as defined in the Ethernet link module's
 *  platform configuration file.
 */
error_type_t net_eth_var_restore_defaults(net_eth_var_id_t var_id);

/**
 * @brief Get the Ethernet link configuration method.
 * 
 * @param[out] link_config Pointer to storage allocated by caller used to pass back the current link
 *       configuration method.
 * 
 * @return Always returns NET_ETH_OK.
 * 
 * Indicates the method by which the Ethernet link will attempt to configure itself and acquire link
 * status (e.g. auto-negotiation vs. forced speed and duplex). Note the distinction between Ethernet
 * link configuration retrieved using this routine vs. Ethernet link status retrieved using
 * net_eth_get_var_link_status().
 */
error_type_t net_eth_get_var_link_config(eth_mac_link_config_t *link_config);

/**
 * @brief Set the Ethernet link configuration method.
 * 
 * @param[in] The new variable value.
 * 
 * @return Always returns NET_ETH_OK.
 * 
 * Determines the method by which the Ethernet link will attempt to configure itself and acquire
 * link status (e.g. auto-negotiation vs. forced speed and duplex).
 */
error_type_t net_eth_set_var_link_config(eth_mac_link_config_t link_config);

/**
 * @brief Get the Ethernet link status.
 * 
 * @param[out] link_status Pointer to storage allocated by caller used to pass back the current link
 *       status.
 * 
 * @return Always returns NET_ETH_OK.
 * 
 * The current link status information will include link up/down state and resolved PHY speed and
 * duplex. Note the distinction between Ethernet link status retrieved using this routine vs. the
 * Ethernet link configuration method retrieved using net_eth_get_var_link_config().
 */
error_type_t net_eth_get_var_link_status(eth_mac_link_status_t *link_status);

/**
 * @brief Get Ethernet packet statistics.
 * 
 * @param[out] rx_stats Pointer to storage allocated by caller used to pass back received 
 *       packet statistics. Caller may use NULL if don't need these stats.
 * 
 * @param[out] tx_stats Pointer to storage allocated by caller used to pass back 
 *       transmitted packet statistics. Caller may use NULL if don't need these stats.
 * 
 * @return Always returns NET_ETH_OK.
 * 
 * All statistical counters represent running totals since device power up.
 */
error_type_t net_eth_get_var_link_statistics(eth_mac_rx_stats_t *rx_stats, eth_mac_tx_stats_t *tx_stats);

#ifdef __cplusplus
}
#endif

#endif // NET_ETH_VARS_H
