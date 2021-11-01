/*
 * ============================================================================
 * Copyright (c) 2010-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file wifi_intf_api.h
 * 
 * @brief Wireless Interface API
 * 
 * This API exposes routines to utilize WPS (WiFi Protected Setup), start/stop
 * the wireless radio stack and perform wireless scans.
 */

#ifndef _WIFI_INTF_API_H
#define _WIFI_INTF_API_H

#include "data_access_api.h"
#include "wifi_intf_vars.h"

#ifdef __cplusplus
extern "C" {
#endif

// SSID defines
// Maximum number of SSIDs stored during a wireless scan
#define MAX_SSID_NAMES 25
// Maximum SSID name length
#define MAX_SSID_NAME_LEN 33 // 32 plus null

// Wireless network information
// Scan information for one BSSID
typedef struct wifi_intf_network_info_s
{
    uint8_t SSIDNames [ MAX_SSID_NAME_LEN ];
    uint8_t BSSID [ BSSID_LENGTH ];
    wifi_intf_comm_mode_t communicationMode;
    wifi_intf_security_mode_t securityMode;
    uint8_t signalStrength;
    int8_t  dBm;
    uint8_t rank;
    uint8_t channel;
    uint8_t wifiOptions; // Bit field to indicate if options are enabled
                         // on the access point, currently only used for WPS
} wifi_intf_network_info_t;

// Wireless scan information
// Wireless scan information for MAX_SSID_NAMES scanned BSSIDs
// Allocating the storage for this structure on the stack is NOT recommended!
typedef struct wifi_intf_scan_info_s
{
    uint8_t NumSSID;
    uint8_t minSignalStrength;
    uint8_t maxSignalStrength;
    wifi_intf_network_info_t networks[ MAX_SSID_NAMES ];
} wifi_intf_scan_info_t;


/** 
 * @brief Start WPS in PIN or PBC mode
 * 
 * @param ssid When in pin mode, this is used to select
 * which access point to connect to.
 * @param ssid_len The length of the SSID buffer
 * @param pin Start pin mode with given pin. (NULL for
 * push-button mode)
 * @param pin_len The length of the pin buffer
 */
void wifi_intf_start_wps(uint8_t * ssid, uint8_t ssid_len, uint8_t * pin, uint8_t pin_len);

/** 
 * @brief Start WPS as registrar on UAP in PIN or PBC mode
 * 
 * @param pin Start pin mode with given pin. (NULL for
 * push-button mode)
 * @param pin_len The length of the pin buffer
 */
void wifi_intf_start_uap_wps_registrar(uint8_t * pin, uint8_t pin_len);

/** 
 * @brief Start WPS as registrar on WFD GO in PIN or PBC mode
 * 
 * @param pin Start pin mode with given pin. (NULL for
 * push-button mode)
 * @param pin_len The length of the pin buffer
 */
void wifi_intf_start_wfd_wps_registrar(uint8_t * pin, uint8_t pin_len);

/**
 * @brief Cancels a WPS session in progress
 */
void wifi_intf_stop_wps(void);

/**
 * @brief Turns on the radio. Use wifi_intf_set_var_is_wireless_hw_enabled() 
 * to enable/disable the radio power state to be persistent through power cycles.
 */
void wifi_intf_start_radio(void);

/**
 * @brief Turns off the radio. Use wifi_intf_set_var_is_wireless_hw_enabled() 
 * to enable/disable the radio power state to be persistent through power cycles.
 */
void wifi_intf_stop_radio(void);

/*
 * @brief Starts a wireless scan for access points
 *
 * Use wifi_intf_get_var_is_wireless_scanning() to determine when the scan
 * is complete. Get the scan data with wifi_intf_get_var_scan_info().
 */
void wifi_intf_start_scan(uint8_t * ssid, uint8_t len);

/**
 * @brief Getter for information about wireless networks scanned - see
 * the wifi_intf_start_scan() method.
 *
 * @param info Pointer to a user allocated buffer into which
 * the contents of the scan will be copied.  The scan data is large enough that
 * allocating the buffer on the stack is NOT recommended!
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_scan_info(wifi_intf_scan_info_t * info);

/**
 * @brief Getter to determine if the wireless network scan has completed.
 *
 * @param is_scanning The result will be copied here.
 * 
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_get_var_is_wireless_scanning(bool * is_scanning);

/**
 * @brief Suspend wifi USB port.
 *  
 * @param remote_wakeup_callback    Pointer to remote wakeup 
 *                                  callback
 * @return N/A.
 */
void wifi_intf_usb_suspend(void * remote_wakeup_callback);

/**
 * @brief Shutdown wifi USB port.
 *
 * @return N/A.
 */
void wifi_intf_shutdown(void);

/**
 * @brief Resume wifi USB port.
 *
 * @return N/A.
 */
void wifi_intf_usb_resume(void);

/**
 * @brief Set wifi USB port.
 *
 * @param port The wifi USB port.
 *
 * @return N/A.
 */
void wifi_intf_set_usb_port(int port);

/**
 * \brief generate wps pin
 */

char *generate_wps_pin( void );

void notify_wifi_sta_vars_changed(); // driver routine
void notify_wifi_uap_vars_changed(); // driver routine
void notify_wifi_wfd_vars_changed(); // driver routine
void notify_wifi_hw_vars_changed();  // driver routine
void notify_wifi_uap_pri0_vars_changed(); //driver routine

/**
 * @brief Get wifi STA link speed
 *
 * @param link_speed  The link speed will be copied here
 *
 * @return Always returns WIFI_INTF_OK.
 */
error_type_t wifi_intf_sta_get_link_speed(int *link_speed);

void notify_wifi_sta_vars_changed(); // driver routine
void notify_wifi_uap_vars_changed(); // driver routine
void notify_wifi_wfd_vars_changed(); // driver routine
void notify_wifi_hw_vars_changed();  // driver routine
void notify_wifi_uap_pri0_vars_changed(); //driver routine
void wifi_intf_uap_device_connected(char * device_name, 
                                    unsigned int device_name_length, 
                                    unsigned char * mac_addr, 
                                    unsigned char * ip_addr); // driver routine

#ifdef UAP_SUPPORT

#define DRVR_MAX_NUM_CLIENTS  16
#define MAC_LENGTH            6
#define MAX_IE_SIZE  256

/** station info */
typedef struct _uap_sta_info
{
    /** STA MAC address */
    unsigned char mac_address[MAC_LENGTH];
    /** Power mfg status */
    unsigned char power_mfg_status;
    /** RSSI */
    char rssi;
} uap_sta_info;

/** mlan_ds_sta_list structure for UAP_STA_LIST */
typedef struct _uap_mlan_ds_sta_list
{
    /** station count */
    unsigned short sta_count;
    /** station list */
    uap_sta_info info[DRVR_MAX_NUM_CLIENTS];
} uap_mlan_ds_sta_list;
/** Custom IE info */
typedef struct
{
    uint16_t mgmt_subtype_mask;    
    int      ie_length;
    uint8_t  buffer[MAX_IE_SIZE];    
} wifi_intf_customie_info_t;

#endif

// Get info about the stations currently attached to the uAP
// returns number of stations attached (or -1 on error)
int wifi_intf_get_var_station_list(uint8_t iface_id, uap_mlan_ds_sta_list *sta_list);

/** 
 * \brief Set custom IE 
 *  
 * Set custom information element. 
 *
 * <iface_id> :     1   uAP interface
 *                  2   WFD interface
 *
 * <ie_index> :     -1  Append/Delete IE automatically 
 *                      Delete will delete the IE from the matching
 *                      IE buffer
 *                      Append will append the IE to the buffer with
 *                      the same mask
 *                  0   Set IE index 0 setting
 *                  ...
 *                  X   Set IE index X setting
 *  
 * <mgmt_subtype_mask> : Management subtype mask value as per
 *                       bit definition
 *                  Bit 0 - Association request
 *                  Bit 1 - Association response
 *                  Bit 2 - Reassociation request
 *                  Bit 3 - Reassociation response
 *                  Bit 4 - Probe request
 *                  Bit 5 - Probe response
 *                  Bit 8 - Beacon
 *                  mask = 0 to clear the mask and the IE buffer
 *  
 * <ie_data> :      IE Buffer (max 256 bytes) 
 *  
 *  @return bool  
 */ 
bool wifi_intf_set_custom_ie(uint8_t iface_id,
                             int ie_index,
                             uint16_t mgmt_subtype_mask,
                             uint8_t * ie_buffer,
                             int ie_length);

wifi_intf_customie_info_t *wifi_intf_get_customie_info(uint8_t index);

#ifdef __cplusplus
}
#endif

#endif // _WIFI_INTF_API_H
