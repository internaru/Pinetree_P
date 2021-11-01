/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

void show_wifi_oids(void)
{
#define tbd break
switch (oid)
{
/**
 * wifi Module OIDS
 */
    case OID_WIFI_INTF_HW_ENABLED           : tbd;
    case OID_WIFI_INTF_ENABLED              : tbd;
    case OID_UAP_INTF_ENABLED               : tbd;
    case OID_WIFI_INTF_SSID                 : tbd;
    case OID_UAP_INTF_SSID                  : tbd;
    case OID_WIFI_INTF_SSID_PREFIX          : tbd;
    case OID_WIFI_INTF_SEC_MODE             : tbd;
    case OID_WIFI_INTF_WPA_ENCR_MODE        : tbd;
    case OID_WIFI_INTF_WPA_PASSPHRASE       : tbd;
    case OID_WIFI_INTF_WEP_AUTH_MODE        : tbd;
    case OID_WIFI_INTF_WEP_KEY_LEN          : tbd;
    case OID_WIFI_INTF_WEP_KEY_1            : tbd;
    case OID_WIFI_INTF_WEP_KEY_2            : tbd;
    case OID_WIFI_INTF_WEP_KEY_3            : tbd;
    case OID_WIFI_INTF_WEP_KEY_4            : tbd;
    case OID_WIFI_INTF_WEP_CUR_KEY          : tbd;
    case OID_WIFI_INTF_WEP_CUR_KEY_VALUE    : tbd;
    case OID_WIFI_INTF_CHANNEL              : tbd;
    case OID_WIFI_INTF_SIGNAL_STRENGTH      : tbd;
    case OID_WIFI_INTF_MAC_ADDR             : tbd;
    case OID_WIFI_INTF_BSSID                : tbd;
    case OID_WIFI_UAP_GET_DHCP_ADDR    : tbd;
    case OID_WIFI_INTF_LOCALE               : tbd;
    case OID_WIFI_INTF_NUM_ATTACHED_DEVICES  : tbd;
    case OID_WIFI_HOTSPOT_ENABLED           : tbd;
    case OID_WIFI_HOTSPOT_RESTORE           : tbd;
    case OID_WIFI_HOTSPOT_REBOOT            : tbd;

/** wps feature*/
    case OID_WIFI_INTF_START_WPS       : tbd;
    case OID_WIFI_INTF_STOP_WPS        : tbd;
    case OID_WIFI_INTF_WPS_PIN         : tbd;

    case OID_WIFI_INTF_COMM_MODE        : tbd;
    case OID_WIFI_INTF_SSID_SUFFIX_LEN  : tbd;
    case OID_WIFI_INTF_GENERATE_WPS_PIN : tbd;
    case OID_WIFI_INTF_WLAN_STATUS      : tbd;
    case OID_WIFI_INTF_DOWNLINK_COUNT   : tbd;

/** DHCPD Oids */
    case OID_WIFI_UAP_DHCP_ADDR          : tbd;
    case OID_WIFI_UAP_DHCP_ADDR_PREFIX   : tbd;
    case OID_WIFI_UAP_DHCP_SUBNET_MASK   : tbd;
    case OID_WIFI_UAP_DHCP_START_ADDR    : tbd;
    case OID_WIFI_UAP_DHCP_END_ADDR      : tbd;
    case OID_WIFI_UAP_DHCP_NUM_ADDRS     : tbd;
    case OID_WIFI_UAP_DHCP_LEASE_TIME    : tbd;

/** MAC Filter */
    case OID_WIFI_INTF_MAC_FILTER_MODE     : tbd;
    case OID_WIFI_INTF_MAC_ADD_FILTER      : tbd;
    case OID_WIFI_INTF_MAC_REMOVE_FILTER   : tbd;
    case OID_WIFI_INTF_MAC_FILTER_COUNT    : tbd;
    case OID_WIFI_INTF_MAC_FILTER_LIST_UAP : tbd;
    case OID_WIFI_INTF_MAC_FILTER_LIST : tbd;
};
}
