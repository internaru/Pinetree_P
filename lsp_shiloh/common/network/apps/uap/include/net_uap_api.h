/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  @file net_uap_api.h
 *  
 *  @brief uap (wpa and udhcpd) interaction(s)
 * 
 */

#ifndef _NET_UAP_API_H
#define _NET_UAP_API_H

#include "error_types.h"
#include "net_api.h" // NET_UAP_LINK_NAME


#define DEFAULT_UAP_SSID_PREFIX  "" /* null prefix by default */
#define DEFAULT_UAP_SSID         "A611_616Series_AP"

#define UAP_MAX_UDHCP_LEASE_TIME      7200 /* 12 minutes */
#define UAP_DEFAULT_STARTING_ADDRESS "192.168.1.10"
#define UAP_UDHCP_ADDRESS_RANGE       30
#define UAP_DEFAULT_ENDING_ADDRESS   "192.168.1.39"
#define UAP_DEFAULT_SUBNET_MASK      "255.255.255.0"

/**
 * @brief Initialize the Network uAP Link layer
 *
 * This routine should be called exactly once following a system startup. 
 */
net_rcode_t net_uap_init(net_link_api_funcs_t *api_funcs);

/**
 * @brief net_uap_init_common()
 *  
 *     Initiates the common/network startup of the uap and wpa_supplicant modules
 *
 * PREREQUISITES: None
 */
error_type_t net_uap_init_common(void);

/**
 * @brief uap_startup(interface)
 *  
 *     Retrieves the current Enabled and Service Name settings
 *   for the given interface (enum) and starts the wpa_supplicant
 *   service if an active interface is enabled.
 *
 * PREREQUISITES: Vars for uap module need to be ready
 */
void uap_startup(uint8_t interface);

/**
 * @brief Refresh contents of the uap (wpa_supplicant) config file
 *
 *    Rewrites the wpa_supplicant config file prior to restarting
 *  the wpa_supplicant service because of a settings change.
 */
error_type_t update_uap_conf(char* ssid);

/**
 * @brief Generates and returns the entire formatted content for 
 * wpa_supplicant service.
 *
 * PREREQUISITES: None
 */
char* format_uap_conf(char* given_buffer, int buf_len);

/**
 * @brief Update the (on-disk fs) bonjour (mDNS) configuration file.
 *
 * Operates by in a two-step process:
 *    1 - generate a new file given new content.
 *    2 - moves current conf -> conf.kng
 *    3 - moves     new conf -> conf
 */
error_type_t uap_write_conf(char* buffer);


/**
 * @brief notify app of a change in the uap (wpa_supplicant) config file
 *
 *    Typically restarts the uap (wpa) service because of a name change,
 *  which requires an update to the device's advertisements.
 */
error_type_t propogate_uap_conf_change(void);

/**
 * @brief stop uap service
 *
 *  Stops uap service.
 */
error_type_t stop_uap_service(void);

/**
 * @brief start uap service
 *
 *  Starts uap service.
 */
error_type_t start_uap_service(void);

/**
 * @brief register_uap_cmds()
 *  
 *     Register uap cmds (debug builds)
 *
 * PREREQUISITES: None
 */
void register_uap_cmds(void);

/**
 * @brief oid_register_module_uap()
 *  
 *     Register uap oids
 *
 * PREREQUISITES: None
 */
void oid_register_module_uap(void);

/**
 * @brief format_uap_udhcpd_conf()
 *  
 *     Build uap's udhcpd config file image
 *
 * PREREQUISITES: None
 */
char* format_uap_udhcpd_conf(char* buf, int len);

/**
 * @brief uap_udhcpd_write_conf()
 *  
 *     (re)Write uap's udhcpd config file
 *
 *  Typically updated if udhcpd lease time is changed
 *
 * PREREQUISITES: None
 */
error_type_t uap_udhcpd_write_conf(uint32_t lease_time);

bool wifi_uap_enabled(void);               // true or false, oid determined
bool wifi_uap_hw_ready(void);              // true or false, hw determined
bool wifi_uap_enabled_and_available(void); // true or false, oid & hw determined
bool wifi_uap_running(void);               // true/false running/not-running

error_type_t rebuild_uap_start_daemon_sh(void);
error_type_t rebuild_stop_uap_daemon_sh(void);
error_type_t rebuild_stop_udhcpd_daemon_sh(void);

// rebuild entire uap service/daemon file set
error_type_t rebuild_uap_sh_files(void);

// rewrite uap conf file(s)
error_type_t uap_update_conf_files(char* ssid);

#endif // _NET_UAP_API_H

//eof
