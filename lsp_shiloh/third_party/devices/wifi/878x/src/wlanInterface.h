/*
 * ============================================================================
 * Copyright (c) 2011-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _WLANINTERFACE_H
#define _WLANINTERFACE_H

#include "marvellTypeMap.h"
#include "wifi_intf_vars.h"

#define WIFI_IFACE_NAME_STATION "mlan0"
#define WIFI_IFACE_NAME_UAP     "uap0"
#define WIFI_IFACE_NAME_WFD     "wfd0"

#define WIFI_UAP_INTRA_BSS_TRAFFIC_ALLOWED 1

// Start a directed or generic scan (ssid = NULL for generic)
int  wirelessStartScan(unsigned char *ssid);

typedef struct
{
    int   role;
    char *iface;
    char *ssid;
    char *pin;    
} t_wps_request;

void wirelessStartWPSEnrollee(char *ssid, char *pin);
void wirelessStartWPSRegistrar(char *pin);
void wirelessStartWFDWPSRegistrar(char *pin);
void wirelessStartWFDWPSEnrollee(char *ssid, char *pin);
void wirelessStartWPS(char *iface, int role, char *ssid, char *pin);
void wirelessCancelWPS(void);

void WlanShimDRecvPacket(unsigned char *Pkt, unsigned long Len, void *buffdesc, int bss_type);
void WlanShimDGetRxBuf(char **rcv_buf, int *buf_len, void ** buf_desc);
void WlanShimDTxPktSent(char *pkt, unsigned long pkt_len, void * handle);
BOOL WirelessLowerLayerHasPacketOutstanding(int bss_type);

BOOL WlanGetMacAddr(char *iface, char* Addr);
void WlanSetMulticast(unsigned char Num, unsigned char *List[6], int bss_type);
int WlanGetBssid(char *extra);

void WlanMainProcess(void);
BOOL WlanIsConnected(void);
BOOL WlanIsWirelessHWEnabled(void);
BOOL WlanIsWirelessHWActive(void);
BOOL WlanIsShutdownCompleted(void);
void WlanWirelessHWInit(void);
void WlanWirelessHWTurnPowerOff(void);
void WlanWirelessHWTurnPowerOn(void);
BOOL WlanIsWirelessConfigured(void);
void WlanWirelessHWDisable(void);
void WlanWirelessHWEnable(void);
BOOL WlanIsWirelessADHOC(void);
BOOL WlanSetInfraBandBG(void);
BOOL WlanSetInfraBandBGN(void);
int WlanIoctl(int command, void * buf);
int WlanGetChannel(void);
int wlan_set_bss_type(char *iface, int bss_type);
int wlan_get_bss_type(char *iface, int *bss_type);

ATSTATUS WlanStartup(void);
ATSTATUS WlanShutdown(void);

int WlanARPFilter(void * buf, int len);
int WlanUSBResume(void);
int WlanUSBSuspend(void * remote_wakeup_callback);
int WlanSetUSBPort(int port);
int WlanGetRate(void);

int WlanSetMode(wifi_intf_comm_mode_t wlanMode);
int WlanAssoc(char * ssid, int len, int skip_scan);
int WlanSetWAP(const CHAR* bssid);
int WlanClearEncryption(void);

void WirelessInit(void);
struct net_device * wlan_get_net_dev();
void WlanWmmCleanupQueues(struct net_device *);
struct net_device *WlanGetNetDev(int bss_type);

void  updateWPSOutcome(wifi_intf_wps_outcome_t newState);

// relocate or eliminate
void creat_del_evnt_flags(unsigned char);

// internal wireless only; do not expose to outside
int wlan_init_module(void);
void wlan_cleanup_module(void);

// Get a series of bytes that is randomly distributed and different for each printer, 
// but will always be identical for the same printer on repeated calls.
void get_printer_uuid(char *uuid, int len);




// Add a MAC address to the MAC filter (returns 0 on success)
int wifi_uap_mac_filter_add(char *mac_as_str);

// Removes a MAC address from the MAC filter (returns 0 on success)
int wifi_uap_mac_filter_remove(char *mac_as_str);

// Allow the wired link to enable/disable the wifi station
void wifi_sta_set_disable_by_wired_link(BOOL disabled_by_wired);

// Add WPS IE to probe requests
int wlan_set_scan_wps_ie(void);

// Remove WPS IE from probe requests
int wlan_reset_scan_wps_ie(void);



#endif	//_WLANINTERFACE_H
