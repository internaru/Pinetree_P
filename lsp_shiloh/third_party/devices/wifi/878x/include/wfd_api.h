/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file wfd_api.h
 * 
 * @brief Wireless Wifi-Direct API
 * 
 * This API exposes routines for the Wifi-Direct
 */

#ifndef WFD_API_H
#define WFD_API_H

typedef struct _wfd_peer_info_t
{
    unsigned char group_capability;
    unsigned char *device_id;
    unsigned char *device_addr;
    unsigned char *go_ssid;
    unsigned char *intended_addr;
    unsigned char *wps_device_name;
} wfd_peer_info_t;


/** Function Prototype Declaration */

/**
 * @brief Get number of detected peers
 *
 * @param num_peers Pointer to number of peer detected
  *
 * @return OK or FAIL.
 */
error_type_t
wfd_num_of_peers(int *num_peers);


/**
 * @brief Get peer by index
 *
 * @param index     Peer index
 * @param peer_info Pointer to peer information
 *
 * @return OK or FAIL.
 */
error_type_t
wfd_peer_device(int index, wfd_peer_info_t *peer_info);


/**
 * @brief Select peer by index
 *
 * @param sel_index     Selected peer index
  *
 * @return OK or FAIL.
 */
error_type_t
wfd_peer_device_selected(int sel_index);


/**
 * @brief Start Find Phase
 *
 * @return OK or FAIL.
 */
error_type_t
wlan_wfd_start_find_phase(void);

/**
 * @brief Get Wifi-Direct GO SSID
 *
 * @param ssid_prefix The Wifi-Direct GO SSID.
 *
 * @return N/A.
 */
void wifi_get_active_wfd_ssid_prefix(char *ssid_prefix);


#endif //WFD_API_H

