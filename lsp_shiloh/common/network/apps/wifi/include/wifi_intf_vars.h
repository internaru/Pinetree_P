/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file wifi_intf_vars.h
 * 
 * @brief Wireless interface variables
 * 
 * This API exposes a set of wireless specific variables utilized by the
 * wireless stack.
 * 
 * Variable values are stored in NVRAM and persist across power cycles.
 */

#ifndef WIFI_INTF_VARS_H
#define WIFI_INTF_VARS_H

#include "error_types.h"
#include "stdbool.h"
#include "event_observer.h"
#include "wifi_intf_event_api.h"

#define WIFI_IFACE_ID_STATION    0
#define WIFI_IFACE_ID_UAP        1
#define WIFI_IFACE_ID_WFD        2
#define WIFI_IFACE_ID_UAP_PRI0   3   
#define WIFI_NUM_IFACES          4

#define WIFIDIRECT_GO_ROLE                  1
#define WIFIDIRECT_CLIENT_ROLE              2

/**
 * @name Return codes
 *
 * @note Error code values 0 through -10 are reserved in error_types.h.
 */
#define WIFI_INTF_OK          OK
#define WIFI_INTF_FAIL        FAIL
#define WIFI_INTF_UNAVAILABLE -12   // requested command not available on specified interface

/** Bit masks for wifiOptions field in scan data
 */
#define WPS_PIN         0x01
#define WPS_PUSH_BUTTON 0x02
#define WPS_PBC_ACTIVE  0x04

//#define WIFI_STA_BAD_WEP_KEY_TIMER_CALLBACK_TIME  3000 //To detect bad WEP key in open auth mode
#define WIFI_STA_BAD_KEY_RETRY_COUNT 3 //In the case of WEP shared auth mode with a bad key or bad WPA/WPA2 passphrase, 
                                       //try WIFI_STA_BAD_KEY_RETRY_COUNT times before reporting an error	

/** Communication modes
 */
typedef enum
{
    AD_HOC = 1,
    INFRASTRUCTURE
} wifi_intf_comm_mode_t;

/** Encryption modes
 */
typedef enum
{
    OPEN_SYSTEM = 0,
    WEP,
    WPA_PSK,
    WPA2_PSK,
    WPA_ENTERPRISE,
    WPA2_ENTERPRISE
} wifi_intf_security_mode_t;

/** WEP authentication modes
 */
typedef enum
{
    AUTH_OPEN = 0,
    AUTH_SHARED_KEY,
    AUTH_AUTO
} wifi_intf_wep_auth_mode_t;

/** WPA encryption modes
 */
typedef enum
{
    WPA_TKIP = 0,
    WPA_AES,
    WPA_TKIP_AND_AES
} wifi_intf_wpa_encryp_mode_t;

/** WEP input key type
 */
typedef enum
{
    ALPHANUMERIC = 0,
    HEXADECIMAL
}wifi_intf_input_key_t;

/** Encryption Key defines
 */
#define NUM_WEP_KEYS 4
#define WEP_KEY_LEN 26
#define WEP_KEY_LENGTH_OF_ALPHANUM_64BIT_KEY    5
#define WEP_KEY_LENGTH_OF_ALPHANUM_128BIT_KEY   13
#define WEP_KEY_LENGTH_OF_HEX_64BIT_KEY         10
#define WEP_KEY_LENGTH_OF_HEX_128BIT_KEY        26
#define PW_KEY_LEN 16
#define GW_KEY_LEN 16
#define MIN_PASS_PHRASE 8
#define MAX_PASS_PHRASE 64

#define BSSID_LENGTH 6
#define MAC_ADDR_LENGTH 6
#define DEFAULT_WIFI_CHAN 11

/** Region Codes
 */
#define REG_US  0x10
#define REG_CAN 0x20
#define REG_EUR 0x30
#define REG_ESP 0x31
#define REG_FRA 0x32
#define REG_JPN 0x40

/** Board status
 */
typedef enum
{
    WB_OK,              // wireless board installed and operational
    WB_NOT_INSTALLED,   // wireless board not installed
    WB_BAD_FIRMWARE     // wireless board installed, but firmware corrupt
}wifi_intf_board_status_t;

/** RX TX modes
 */
typedef enum {
    RF_TX_MODE_NORMAL = 0,
    RF_TX_MODE_CONT_MODE,   // continuous modulated
    RF_TX_MODE_CONT_CARR,   // continuous carried
} wifi_intf_rf_tx_mode_t;

/** Antenna type
 */
typedef enum {
    ANT_RX = 0,
    ANT_TX,
} wifi_intf_antenna_type_t;

/** Antenna number
 */
typedef enum {
    ANT_DIVERSITY = 0xFFFF,
    ANT_1 = 1,
    ANT_2 = 2,
} wifi_intf_antenna_num_t;            

/** Antenna information (deprecated?)
 */
typedef struct {
    wifi_intf_antenna_type_t Type;
    wifi_intf_antenna_num_t Num;
} wifi_intf_antenna_t;

#define WPS_PIN_LENGTH 9 // 8 + 1 for null
/** WPS outcome states
 */
typedef enum
{
    WPS_STATE_NOT_YET_RUN,      // Stays in this state from bootup until first run
    WPS_STATE_PROCESSING,       // Attempting to run WPS
    WPS_STATE_CONNECTING,       // Attempting to connect
    WPS_STATE_SUCCESS,          // WPS worked the last time someone tried it
    WPS_STATE_TIMEOUT,          // WPS tried to run, but never found an active AP
    WPS_STATE_SESSION_OVERLAP,  // WPS found multiple active access points and quit
    WPS_STATE_UNKNOWN_FAILURE,  // Encryption and handshake failures, loss of connectivity, etc.
    WPS_STATE_CANCELLED         // User cancelled WPS
} wifi_intf_wps_outcome_t;

typedef enum
{
    MAC_FILTER_MODE_DISABLED = 0,
    MAC_FILTER_MODE_WHITELIST = 1,
    MAC_FILTER_MODE_BLACKLIST = 2,
} wifi_intf_mac_filter_mode_t;

/** NVRAM Variables
 */
typedef enum
{
    // perm vars general to the hardware
    WIFI_HW_ENABLED,
    WIFI_HW_RFTX_MODE,
    WIFI_HW_TX_ANTENNA,
    WIFI_HW_RX_ANTENNA,
    WIFI_HW_REGIONCODE,
    WIFI_HW_LOCALE,
    #ifdef HAVE_HOTSPOT_SUPPORT
    // perm vars not tied to a particular interface
    WIFI_HOTSPOT_ENABLED,
    #endif // HAVE_HOTSPOT_SUPPORT
    // add new vars above this line
    WIFI_HW_NUM_VARS,
    WIFI_HW_VAR_ALL
} wifi_hw_var_id_t;

/** NVRAM Variables
 */
typedef enum
{
    // perm vars available on all interfaces
    WIFI_INTF_ENABLED,
    WIFI_INTF_SSID,
    WIFI_INTF_SEC_MODE,
    WIFI_INTF_ENCRYP_MODE,
    WIFI_INTF_WPA_ENABLED,
    WIFI_INTF_WPA_ENCR_MODE,
    WIFI_INTF_WPA_PASSPHRASE,
    WIFI_INTF_WEP_ENABLED,
    WIFI_INTF_WEP_AUTH_MODE,
    WIFI_INTF_WEP_KEY_LEN,
    WIFI_INTF_WEP_KEYS,
    WIFI_INTF_WEP_KEY_1,
    WIFI_INTF_WEP_KEY_2,
    WIFI_INTF_WEP_KEY_3,
    WIFI_INTF_WEP_KEY_4,
    WIFI_INTF_WEP_CUR_KEY,
    WIFI_INTF_WEP_KEY_NUM,
    WIFI_INTF_CHANNEL,

    // perm vars only available on STA
    WIFI_INTF_COMM_MODE,

    // perm vars only available on UAP
    WIFI_INTF_SSID_PREFIX,
    WIFI_INTF_MAC_FILTER_MODE,
    WIFI_INTF_MAC_FILTER_LIST,

    // blend
    WIFI_INTF_INPUT_KEY,
    WIFI_INTF_PASSPHRASE,
    WIFI_INTF_CURR_WEP_KEY,
    WIFI_INTF_WPAPWK,
    WIFI_INTF_WPAGRPK,
    WIFI_INTF_IS_WIRELESS_ENABLED,
    WIFI_INTF_RFTX_MODE,
    WIFI_INTF_TX_ANTENNA,
    WIFI_INTF_RX_ANTENNA,
    WIFI_INTF_BSSID_SET,
    WIFI_INTF_BSSID,

    WIFI_INTF_RSSI_RANGE,
    WIFI_INTF_FILTER_COUNT,

    // add new vars above this line
    WIFI_INTF_NUM_VARS,
    WIFI_INTF_VAR_ALL,
    NET_WIFI_VAR_ALL = WIFI_INTF_VAR_ALL
} wifi_intf_var_id_t;

typedef enum
{
    WLAN_INACTIVE,
    WLAN_ADHOC,
    WLAN_WPS_RUNNING,
    WLAN_ASSOCIATING,
    WLAN_ASSOCIATED,
    WLAN_CONNECTED,
    WLAN_WPS_ERROR,
    WLAN_BAD_KEY_ERROR, //for bad WEP key or bad WPA/WPA2 passphrase
    WLAN_WPS_REGISTRAR_DEVICE_CONNECTING,
} wlan_type_t;

typedef enum
{
    NO_TKIP_WORKAROUND,
    TKIP_WORKAROUND,
    TKIP_ASSOCIATE_FAIL,
} wlan_tkip_workaround_t;


/********************************************* 
 *  WIFI SUBJECT
 *********************************************/

typedef enum wifi_event_e
{ 
    WIFI_LINK_DOWN_EVENT,    	
    WIFI_LINK_CONNECTING_EVENT,
    WIFI_LINK_CONNECTED_EVENT,
    WIFI_LINK_ERROR_EVENT,
    WIFI_STA_ENABLE_EVENT,
    WIFI_STA_DISABLE_EVENT,
    WIFI_SELF_TEST_STARTED_EVENT,
    WIFI_SELF_TEST_RESULTS_READY,
    WIFI_SELF_TEST_FAILED,  
} wifi_event_t;

/** 
 *  @brief Restore one or all wifi hardware persistent variables
 *         to their default values.
 * 
 *  @param var_id ID of the variable to be restored to its default value, or WIFI_INTF_VAR_ALL
 *      for all variables.
 *  
 *  @return OK on success or DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails.
 * 
 *  Use this function to restore one or all wireless interface RAM and NVRAM-based persistent
 *  variables to their default values.
 */
#ifdef __linux__
error_type_t wifi_hw_var_restore_defaults(uint8_t index, wifi_hw_var_id_t var_id);
#else
error_type_t wifi_hw_var_restore_defaults(wifi_hw_var_id_t var_id);
#endif

/**
 * @brief Getter to determine if the wireless harware is enabled.
 *
 * @param hw_enabled Pointer into a buffer given by the caller.
 * A boolean for whether the hardware is enabled is copied there.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_get_var_is_enabled(uint8_t index, bool * hw_enabled);

/**
 * @brief Setter for the wireless hardware enable
 *
 * @param hw_enabled A boolean to show whether the hardware should
 * be enabled.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_set_var_is_enabled(uint8_t index, bool hw_enabled);

/**
 * @brief Getter for the transmit mode of the hardware
 *
 * @param mode The type of transmit mode (continuous modulated,
 * continuous carried, etc). Space should be allocated by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_get_var_rf_tx_mode(wifi_intf_rf_tx_mode_t * mode);

/**
 * @brief Setter for the transmit mode of the hardware
 *
 * @param mode The type of transmit mode (continuous modulated,
 * continuous carried, etc)
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_set_var_rf_tx_mode(wifi_intf_rf_tx_mode_t mode);

/**
 * @brief Getter for the current transmit antenna
 *
 * @param tx_antenna The type of antenna used. Space should be allocated
 * by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_get_var_tx_antenna(wifi_intf_antenna_t * tx_antenna);

/**
 * @brief Setter for the current transmit antenna
 *
 * @param tx_antenna The type of antenna used
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_set_var_tx_antenna(wifi_intf_antenna_t tx_antenna);

/**
 * @brief Getter for the current receive antenna
 *
 * @param rx_antenna The type of antenna used. Space should be allocated
 * by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_get_var_rx_antenna(wifi_intf_antenna_t * rx_antenna);

/**
 * @brief Setter for the current receive antenna
 *
 * @param rx_antenna The type of antenna used.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_set_var_rx_antenna(wifi_intf_antenna_t rx_antenna);

/**
 * @brief Getter for the current region code.
 *
 * @param region_code Buffer into which the current region code will
 * be copied.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_get_var_region_code(uint8_t * region_code);

/**
 * @brief Setter for the current region code.
 *
 * @param region_code The new region code.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_hw_set_var_region_code(uint8_t region_code);


/** 
 *  @brief Restore one or all wireless persistent variables to their default values.
 * 
 *  @param iface_id Which interface to use
 *  @param var_id ID of the variable to be restored to its default value, or WIFI_INTF_VAR_ALL
 *      for all variables.
 *  
 *  @return OK on success or DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails.
 * 
 *  Use this function to restore one or all wireless interface RAM and NVRAM-based persistent
 *  variables to their default values.
 */
error_type_t wifi_intf_var_restore_defaults(uint8_t iface_id, wifi_intf_var_id_t var_id);

/* Getters and setters for wifi_intf_wireless_info_t structure */


error_type_t wifi_intf_get_var_is_enabled(uint8_t iface_id, bool * enabled);
error_type_t wifi_intf_set_var_is_enabled(uint8_t iface_id, bool enabled);

/**
 * @brief Getter for the communication mode. Space must be allocated
 * by the caller.
 * 
 * @param comm_mode The type of communication mode being used by
 * the wireless module.
 *
 * @return Always returns WIFI_INTF_OK
 */
error_type_t wifi_intf_get_var_comm_mode(uint8_t iface_id, wifi_intf_comm_mode_t * comm_mode);

/**
 * @brief Setter for the communication mode.
 * 
 * @param comm_mode The type of communication mode used by the module.
 * A pointer to the value is passed.
 *
 * @return Always returns WIFI_INTF_OK
 */
error_type_t wifi_intf_set_var_comm_mode(uint8_t iface_id, wifi_intf_comm_mode_t comm_mode);

/**
 * @brief Getter for the type of security currently being used.
 *
 * @param sec_mode The type of security mode. Space for this variable
 * must be allocated by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_sec_mode(uint8_t iface_id, wifi_intf_security_mode_t * sec_mode);

/**
 * @brief Setter for the type of security to use.
 *
 * @param sec_mode The type of security mode. A pointer to the value
 * is passed.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_sec_mode(uint8_t iface_id, wifi_intf_security_mode_t sec_mode);

/**
 * @brief Getter for the authentication mode.
 *
 * @param wep_auth_mode The authentication mode. Space for this variable
 * must be allocated by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_wep_auth_mode(uint8_t iface_id, wifi_intf_wep_auth_mode_t * wep_auth_mode);

/**
 * @brief Setter for the authentication mode.
 *
 * @param wep_auth_mode The authentication mode.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_wep_auth_mode(uint8_t iface_id, wifi_intf_wep_auth_mode_t wep_auth_mode);

/**
 * @brief Getter for the encryption mode.
 *
 * @param encryp_mode The encryption mode. Space should be allocated
 * by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_wpa_encryp_mode(uint8_t iface_id, wifi_intf_wpa_encryp_mode_t * encryp_mode);
/**
 * @brief Setter for the encryption mode.
 *
 * @param encryp_mode The encryption mode.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_wpa_encryp_mode(uint8_t iface_id, const wifi_intf_wpa_encryp_mode_t encryp_mode);

/**
 * @brief Getter for the 802.11 channel.
 *
 * @param chan The channel currently being used by the system. Space should
 * be allocated by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_channel(uint8_t iface_id, uint8_t * chan);

/**
 * @brief Setter for the 802.11 channel.
 *
 * @param chan The channel that the system should use.
 * A pointer to the value is passed.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_channel(uint8_t iface_id, uint8_t chan);

/**
 * @brief Getter for the SSID
 *
 * @param ssid The SSID currently in use. Null characters
 * are not currently supported in the middle of the string at
 * this time, though the spec does allow them.
 * @param len The length of the ssid buffer. The length
 * should be MAX_SSID_NAME_LEN, which supports a 32
 * character SSID and a null character. If a buffer
 * that is too small is passed, only that many bytes are
 * copied.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_ssid(uint8_t iface_id, uint8_t * ssid, uint8_t len);

/**
 * @brief Setter for the SSID
 *
 * @param ssid The SSID currently in use. Null characters
 * are not currently supported in the middle of the string at
 * this time, though the spec does allow them.
 * @param len The length of the ssid buffer. The length
 * must be less than MAX_SSID_NAME_LEN, which supports a 32
 * character SSID and a null character. That is the max number
 * of bytes that will be copied.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_ssid(uint8_t iface_id, const uint8_t * ssid, uint8_t len);

/**
 * @brief Getter for the current passphrase
 *
 * @param passphrase The current passphrase.
 * @param len The length of the passphrase buffer, which
 * should be MAX_PASS_PHRASE bytes.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_passphrase(uint8_t iface_id, uint8_t * passphrase, uint8_t len);

/**
 * @brief Setter for the current passphrase
 *
 * @param passphrase The current passphrase.
 * @param len The length of the passphrase buffer, of which
 * up to MAX_PASS_PHRASE bytes will be copied.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_passphrase(uint8_t iface_id, const uint8_t * passphrase, uint8_t len);

/**
 * @brief Getter for the index of the current wep key.
 *
 * @param key A pointer to a buffer where they key will be copied.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_wep_cur_key(uint8_t iface_id, uint8_t * key);

/**
 * @brief Setter for the index of the current wep key
 *
 * @param key The index of the current wep key. Must be
 * less than NUM_WEP_KEYS bytes.
 *
 * @return WIFI_INTF_OK if the index is less than NUM_WEP_KEYS
 * @return WIFI_INTF_FAIL if the index is invalid
 */
error_type_t wifi_intf_set_var_wep_cur_key(uint8_t iface_id, uint8_t key);

/**
 * @brief Getter for an indexed WEP key
 *
 * @param index The index of a WEP key, from 0 to NUM_WEP_KEYS
 * @param key The buffer into which the key should be copied. The caller
 * is responsible for allocating this memory.
 * @param len The number of bytes in the the available buffer. This should be
 * at least WEP_KEY_LEN bytes.
 *
 * @return WIFI_INTF_OK when the index is valid (less than NUM_WEP_KEYS)
 * @return WIFI_INTF_FAIL when the index is invalid
 */
error_type_t wifi_intf_get_var_wep_key(uint8_t iface_id, uint8_t key_index, uint8_t * key, uint8_t len);

/**
 * @brief Setter for an indexed WEP key
 *
 * @param index The index of a WEP key, from 0 to NUM_WEP_KEYS
 * @param key The buffer from which the key is copied.
 * @param len The number of bytes in the the available buffer. This should be
 * at least WEP_KEY_LEN bytes.
 *
 * @return WIFI_INTF_OK when the index is valid (less than NUM_WEP_KEYS)
 * @return WIFI_INTF_FAIL when the index is invalid
 */
error_type_t wifi_intf_set_var_wep_key(uint8_t iface_id, uint8_t key_index, const uint8_t * key, uint8_t len);

/**
 * @brief Getter for the length of a WEP key
 *
 * @param key_len The length of the WEP key. Space must be allocated
 * by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_wep_key_len(uint8_t iface_id, uint8_t * key_len);

/**
 * @brief Setter for the length of a WEP key
 *
 * @param key_len The length of the WEP key.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_wep_key_len(uint8_t iface_id, uint8_t key_len);

/**
 * @brief Getter for the WFD BSS Role
 *
 * @param role  The WFD BSS Role. Space must be allocated
 * by the caller.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_wfd_bss_role(uint8_t iface_id, uint8_t * role);

/**
 * @brief Setter for the WFD BSS Role
 *
 * @param role  The WFD BSS Role.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_set_var_wfd_bss_role(uint8_t iface_id, uint8_t role);

/**
 * @brief Getter for the current mac address
 *
 * @param mac Pointer to the caller allocated buffer
 * into which the mac address will be copied
 * @param len The length of the buffer, which should be 6 bytes.
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_mac_addr(uint8_t iface_id, uint8_t * mac, uint8_t len);
error_type_t wifi_intf_set_var_mac_addr(uint8_t iface_id, const uint8_t * mac, uint8_t len);

error_type_t wifi_intf_get_var_ssid_prefix(uint8_t iface_id, uint8_t * prefix, uint8_t len);
error_type_t wifi_intf_set_var_ssid_prefix(uint8_t iface_id, const uint8_t * prefix, uint8_t len);

error_type_t wifi_intf_get_var_mac_filter_mode(uint8_t iface_id, wifi_intf_mac_filter_mode_t * filter_mode);
error_type_t wifi_intf_set_var_mac_filter_mode(uint8_t iface_id, const wifi_intf_mac_filter_mode_t filter_mode);

error_type_t wifi_intf_var_mac_filter_add(uint8_t iface_id, char *mac_as_str);
error_type_t wifi_intf_var_mac_filter_remove(uint8_t iface_id, char *mac_as_str);
error_type_t wifi_intf_get_var_mac_filter_count(uint8_t iface_id, uint8_t *filter_count);
error_type_t wifi_intf_var_mac_filter_flush(uint8_t iface_id);
error_type_t wifi_intf_get_var_mac_filter_list(uint8_t iface_id, uint8_t (*filter_list)[MAC_ADDR_LENGTH]);


error_type_t wifi_intf_get_var_rssi_dbm(uint8_t iface_id, int32_t *rssi);
int rssi_dbm_to_signal_strength(int rssi);  // output range 1-5

error_type_t wifi_intf_get_var_bssid(uint8_t iface_id, uint8_t * bssid, uint8_t len);
error_type_t wifi_intf_set_var_bssid(uint8_t iface_id, const uint8_t * bssid, uint8_t len); // const

error_type_t wifi_intf_get_var_bssid_set(uint8_t index, bool * bssid_set);
error_type_t wifi_intf_set_var_bssid_set(uint8_t index, bool bssid_set);

error_type_t wifi_hw_get_var_is_radio_present(bool *present);
error_type_t wifi_hw_get_var_is_active(bool *active);
error_type_t wifi_hw_get_var_is_shutdown_completed(bool *shutdown);

void wifi_enable_bad_wepkey_timer(void);
void wifi_kill_bad_wepkey_timer(void);

int generate_random_passphrase(uint8_t * block, int blockLen);

void wifi_intf_sta_attach_event_observer(Observer_t *o);
void wifi_intf_sta_detach_event_observer(Observer_t *o);

wifi_event_t wifi_intf_sta_get_latest_event_type(Subject_t *s);
int wifi_intf_sta_get_latest_event_details(Subject_t *s);

void wifi_intf_uap_attach_event_observer(Observer_t *o);
void wifi_intf_uap_detach_event_observer(Observer_t *o);

wifi_intf_link_status_t wifi_intf_uap_get_link_status( Subject_t *s);
error_type_t wifi_intf_uap_get_device_name( Subject_t *s, char * device_name, unsigned int *device_name_len);
error_type_t wifi_intf_uap_get_mac_addr( Subject_t *s, unsigned char * mac_addr);
error_type_t wifi_intf_uap_get_ip_addr( Subject_t *s, unsigned char * ip_addr);

//Event notifier
void wifi_intf_sta_notify_event( wifi_event_t event, int details );

//routines used for wireless self test 
void wifi_intf_sta_set_state(wlan_type_t status);
wlan_type_t wifi_intf_sta_get_state(void);
wlan_tkip_workaround_t wifi_intf_get_tkip_workaround_state(void);
void wifi_intf_set_tkip_workaround_state(wlan_tkip_workaround_t tkip_info);

void wifi_oem_update(uint8_t iface_id);
void wifi_change_uap_priority(uint8_t priority);
uint8_t wifi_get_uap_active_config(void);
void wifi_change_uap_active_config(uint8_t iface_id);

#ifdef HAVE_HOTSPOT_SUPPORT
typedef struct
{
    char *network;
    char *subnet;
} hotspot_subnet_blacklist_t;

/**
 * @brief Whether or not hotspot functionality should be allowed
 *        to run in the code.
 *
 * @return true if hotspot functionality is allowed.  False
 *         otherwise.
 *
 * @note This function is defined in OEM code.  Each OEM should
 *       provide their own way of determining if the
 *       functionality should be allowed on a product.
 */
bool wifi_hotspot_allowed_by_platform();

/**
 * @brief Gets a list of subnets the OEM wants to prevent from
 *        having connected to a wifi hotspot
 *
 * @return Pointer to list of blacklisted subnets with the final
 *         entry being {null, null}
 *
 * @note For security reasons OEMs may wish to prevent the
 *       hotspot from accidentally being connected to their
 *       corporate network during development.  By adding their
 *       own subnet to this list they can help limit that risk.
 *       For an actual product release, this routine should
 *       generally just return NULL or an empty list.
 */
hotspot_subnet_blacklist_t *wifi_hotspot_get_blacklisted_wans();

#endif // HAVE_HOTSPOT_SUPPORT

#endif // WIFI_INTF_VARS_H

//eof
