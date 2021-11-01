/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  @file net_wifi_api.h
 *  
 *  @brief wifi (wpa_supplicant) interaction
 * 
 */

#ifndef _NET_WIFI_API_H
#define _NET_WIFI_API_H

#include "error_types.h"

#define DEFAULT_PLATFORM_SSID_PREFIX ""
#define DEFAULT_PLATFORM_SSID        "MarvellReference"
#define DEFAULT_WIFI_SSID_PREFIX     ""
#define DEFAULT_WIFI_SSID            "MarvellReference"
#define DEFAULT_WPA_PASSPHRASE       "password"

#define WIFI_SERVICE_RESTART_DELAY 1303 /* ~3+ seconds in units ms */

/**
 * @brief net_wifi_init
 *  
 *     Initiates the startup of the wifi via wpa_supplicant module
 *
 * PREREQUISITES: None
 */
error_type_t net_wifi_init(void);

/**
 * @brief wifi_startup(interface)
 *  
 *     Retrieves the current Enabled and Service Name settings
 *   for the given interface (enum) and starts the wpa_supplicant
 *   service if an active interface is enabled.
 *
 * PREREQUISITES: Vars for wifi module need to be ready
 */
void wifi_startup(uint8_t interface);

/**
 * @brief Refresh contents of the wifi (wpa_supplicant) config file
 *
 *    Rewrites the wpa_supplicant config file prior to restarting
 *  the wpa_supplicant service because of a settings change.
 */
error_type_t update_wifi_conf(char* requested_service);

/**
 * @brief Generates and returns the entire formatted content for 
 * wpa_supplicant service.
 *
 * PREREQUISITES: None
 */
char* format_wifi_conf(char* given_buffer, int buf_len);

/**
 * @brief Update the (on-disk fs) bonjour (mDNS) configuration file.
 *
 * Operates by in a two-step process:
 *    1 - generate a new file given new content.
 *    2 - moves current conf -> conf.kng
 *    3 - moves     new conf -> conf
 */
error_type_t wifi_write_conf(char* buffer);

/**
 * @brief notify app of a change in the wifi (wpa_supplicant) config file
 *
 *    Typically restarts the wifi (wpa) service because of a name change,
 *  which requires an update to the device's advertisements.
 */
error_type_t propogate_wifi_conf_change(void);

/**
 * @brief stop wifi service (os-level daemon)
 *
 *  Stops wifi service.
 */
error_type_t stop_sta_service(void);

/**
 * @brief start wifi service (os-level daemon)
 *
 *  Starts wifi service.
 */
error_type_t start_sta_service(void);

/**
 * @brief schedule wifi service
 *
 *  Schedules wifi service.
 */
long initiate_wifi_service_timer(long delta);
long initiate_wifi_settings_change_timer(long ms_delta);

/**
 * @brief register_wifi_cmds()
 *  
 *     Register wifi cmds (debug builds)
 *
 * PREREQUISITES: None
 */
void register_wifi_cmds(void);

bool wifi_enabled(void);               // true or false, oid
bool wifi_sta_hw_ready(void);          // true or false, hw determined
bool wifi_sta_ready(void);             // true or false, os-lookup ipv4 bound?
bool wifi_sta_enabled(void);           // true or false, oid & hw composite
bool wifi_sta_enabled_and_available(void); // true or false, oid & hw composite
bool wifi_sta_running(void);               // true/false (uap or wlan running?)

// rebuild service/daemon start/stop scripts
error_type_t rebuild_wifi_sh_files(void);

#endif // _NET_WIFI_API_H

//eof
