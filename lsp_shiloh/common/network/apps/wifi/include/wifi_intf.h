/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __WIFI_INTF_H__
#define __WIFI_INTF_H__

#include <stdlib.h>
#include <stdbool.h>

#ifdef __linux__

#include "os_network.h" // posix

#else

#include "trsocket.h"
#include "trmacro.h"
#include "trtype.h"
#include "trproto.h"
#include "marvellTypeMap.h"

#endif

#include "net_iface_api.h"
#include "wirelessQueue.h"
#include "wifi_intf_api.h"
#include "wifi_intf_link_api.h"
#include "wifi_intf_vars.h"


// values which reside in dynamic permanent storage
// IMPORTANT:  increment WIFI_INTF_VARS_VERSION whenever this structure changes
#define WIFI_INTF_VARS_VERSION 0
typedef struct
{
    wifi_intf_comm_mode_t CommunicationMode;
    wifi_intf_security_mode_t SecurityMode;
    wifi_intf_wep_auth_mode_t WEPAuthMode;
    wifi_intf_wpa_encryp_mode_t WPAEncryptionMode;
    wifi_intf_input_key_t InputKey;
    uint8_t Channel;
    uint8_t SSID[MAX_SSID_NAME_LEN];
    uint8_t PassPhrase[MAX_PASS_PHRASE+1];
    uint8_t CurrWepKey;
    uint8_t WEPKey[NUM_WEP_KEYS][WEP_KEY_LEN];
    uint8_t WEPKeyLength;  // should be the same for all WEP keys
    uint8_t WPAPWK[PW_KEY_LEN];  // WPA pair wise key; from supplicant
    uint8_t WPAGRPK[GW_KEY_LEN];  // WPA group key; from suppllicant
    bool IsWirelessHWEnabled;
    bool IsEnabled; // interface

    wifi_intf_rf_tx_mode_t  RFTxMode;
    wifi_intf_antenna_t TxAntenna;
    wifi_intf_antenna_t RxAntenna;
    uint8_t RegionCode;

    bool BssidSet;
    uint8_t BSSID[ BSSID_LENGTH ];
    uint8_t MacAddr[6];
} wifi_intf_wireless_info_t;


//Transmit queue
typedef struct wirelessQPkt
{
   uint32_t pktLen;  //length of buffer
   char* pktBuff;  //pointer to buffer
   char* pktDesc;  //pointer to misc info associated with buff
}WirelessQPkt;

typedef struct
{
   uint32_t head;
   uint32_t tail;
   uint32_t numPkts;   //number of full frames in the queue (each frame can have multiple packets)
   WirelessQPkt pkts[MAX_PKTS];
}WirelessQ;

typedef enum
{
    WIRELESSQ_OK = 0,
    WIRELESSQ_FULL = 1,
    WIRELESSQ_EMPTY = 2,
}WIRELESSQRES;

//Add pkt to end of pkt queue, 
WIRELESSQRES wifi_intf_q_add_pkt(WirelessQ* queue, uint32_t pktLen, char* pktBuff, void* pktMisc);

//get the first pkt in the queue, but does not remove it
//cal NetQRmPkt() for that
WIRELESSQRES wifi_intf_q_get_pkt(WirelessQ* queue, uint32_t *pktLen, char** pktBuff, void** pktMisc);

//remove the first pkt from the q
WIRELESSQRES wifi_intf_q_remove_pkt(WirelessQ* queue);

//this is called when we failed to add a pkt to the q, it flushes
//any pkts that might have been part of the frame. 
void WirelessQFlushLastFrame(WirelessQ* queue);

bool wifi_intf_q_have_more(WirelessQ* queue);

wifi_intf_wps_outcome_t wifi_intf_get_var_wps_state();
void wifi_intf_set_var_wps_state(wifi_intf_wps_outcome_t state);

bool wifi_intf_get_var_is_wps_running();
void wifi_intf_set_var_is_wps_running(bool running);

bool wifi_intf_get_var_is_wireless_radio_present();
void wifi_intf_set_var_is_wireless_radio_present(bool present);

void startApplyInfoTimer(); // in mlanInterface.c
error_type_t wifi_intf_set_var_scan_info(wifi_intf_scan_info_t * info); // in wifi_intf_vars.c
error_type_t wifi_intf_set_var_is_wireless_scanning(bool * is_scanning);
error_type_t wifi_intf_set_var_mac_addr(uint8_t index, const uint8_t * mac, uint8_t len);

void wifi_intf_set_link_status(wifi_intf_link_event_t link_event);
void wifi_intf_set_state(wlan_type_t status);
wlan_type_t wifi_intf_get_state(void);
void wifi_intf_shutdown(void);
wlan_tkip_workaround_t wifi_intf_get_tkip_workaround_state(void);
void wifi_intf_set_tkip_workaround_state(wlan_tkip_workaround_t tkip_info);
void wifi_intf_wait_for_link_down_ack(uint32_t delay);

void wifi_intf_rx_packet_recvd(char* pktBuff, uint32_t pktLen, void* buffDesc);
void wifi_intf_get_rx_buffer(char** rxBuff, int* buffLen, void** buffDesc);
void wifi_intf_tx_packet_sent(char* pktBuff, uint32_t pktLen, void * handle);
void wifi_intf_startup(void);
void wifi_intf_shutdown_notify(void);

#endif // __WIFI_INTF_H__
