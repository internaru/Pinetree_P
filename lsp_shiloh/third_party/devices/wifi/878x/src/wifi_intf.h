/*
 * ============================================================================
 * Copyright (c) 2011-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __WIFI_INTF_H__
#define __WIFI_INTF_H__

#include "trsocket.h"
#include "trmacro.h"
#include "trtype.h"
#include "trproto.h"

#include <stdlib.h>
#include <stdbool.h>

#include "net_iface_api.h"
#include "marvellTypeMap.h"
#include "wifi_intf_api.h"
#include "wifi_intf_link_api.h"
#include "wifi_intf_vars.h"

#define MAX_FILTERED_ADDRS 16   // (restricted by hardware)

// values which reside in dynamic permanent storage
// IMPORTANT:  increment WIFI_HW_VARS_VERSION whenever this structure changes
#define WIFI_HW_VARS_VERSION 3

typedef struct
{
    bool enabled;
    wifi_intf_rf_tx_mode_t  RFTxMode;
    wifi_intf_antenna_t TxAntenna;
    wifi_intf_antenna_t RxAntenna;
    uint8_t RegionCode;
    uint8_t WwsConfig;
    #ifdef HAVE_HOTSPOT_SUPPORT
    bool hotspot_enabled;
    #endif // HAVE_HOTSPOT_SUPPORT
} wifi_hw_info_t;

// IMPORTANT:  increment WIFI_INTF_VARS_VERSION whenever this structure changes
#define WIFI_INTF_VARS_VERSION 2

//Maximum number of custom IEs that can be tagged along on the WLAN management frames 
#define MAX_NUM_CUSTOM_IE 4

typedef struct
{
    bool    enabled;
    uint8_t MacAddr[MAC_ADDR_LENGTH];

    uint8_t SSID[MAX_SSID_NAME_LEN];
    uint8_t Channel;

    wifi_intf_security_mode_t SecurityMode;

    wifi_intf_wpa_encryp_mode_t WPAEncryptionMode;
    uint8_t WPAPassPhrase[MAX_PASS_PHRASE + 1];

    wifi_intf_wep_auth_mode_t WEPAuthMode;
    uint8_t WEPKeyLength;  // should be the same for all WEP keys
    uint8_t WEPKey[NUM_WEP_KEYS][WEP_KEY_LEN + 1];
    union
    {
        uint8_t WEPCurKey;
        uint8_t WFDBSSRole;
    };

    union
    {
        struct 
        {
            wifi_intf_comm_mode_t CommunicationMode;
        } sta;
        struct
        {
            uint8_t SSIDPrefix[MAX_SSID_NAME_LEN];
            wifi_intf_mac_filter_mode_t MACFilterMode;
            uint8_t MACFilterList[MAX_FILTERED_ADDRS][MAC_ADDR_LENGTH];
        } uap;
    };
} wifi_intf_info_t;

wifi_intf_wps_outcome_t wifi_intf_get_var_wps_state();
void wifi_intf_set_var_wps_state(wifi_intf_wps_outcome_t state);

bool wifi_intf_get_var_is_wps_running();
void wifi_intf_set_var_is_wps_running(bool running);

void wifi_intf_set_var_is_wireless_radio_present(bool present);

error_type_t wifi_intf_set_var_scan_info(wifi_intf_scan_info_t * info); // in wifi_intf_vars.c
error_type_t wifi_intf_set_var_is_wireless_scanning(bool is_scanning);

void wifi_intf_sta_set_link_status(wifi_intf_link_event_t link_event);
void wifi_intf_uap_set_state(wlan_type_t status, char *device_name, unsigned int device_name_length, unsigned char *mac_addr, unsigned char *ip_addr);
void wifi_intf_shutdown(void);
void wifi_intf_wait_for_link_down_ack(uint32_t delay);

void wifi_intf_rx_packet_recvd(char* pktBuff, uint32_t pktLen, void* buffDesc);
void wifi_uap_intf_rx_packet_recvd(char* pktBuff, uint32_t pktLen, void* buffDesc);
void wifi_intf_get_rx_buffer(char** rxBuff, int* buffLen, void** buffDesc);
void wifi_intf_tx_packet_sent(char* pktBuff, uint32_t pktLen, void * handle);
void wifi_intf_startup(void);
void wifi_intf_shutdown_notify(void);
void wifi_wfd_clear_active_ssid_flag(void);
void wifi_intf_create_resources(void);
void wifi_intf_disallow_pwr_mgr(uint32_t * uid);
void wifi_intf_allow_pwr_mgr(uint32_t * uid);
int WlanSetUserScanCfg(char * ssid, int ssid_len);
#endif
