/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// Network oid collection info:  generates a string literal given oid_t

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// common headers
#include "oid_api.h" // oid_t

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "net.oid: "
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
#include "net_debug.h"


// return a literal string
#define str_oid(oid) case oid: return #oid;

// api
static char heap_str[64];  // contents valid after most-recent call only..
char* get_oid_literal(oid_t oid)
{
    // list filled with any interesting oids from oid_api.h
    switch (oid)
    {
        str_oid(OID_NETWORK_STATUS);
        str_oid(OID_NETWORK_MAC_ADDRESS);
        str_oid(OID_NETWORK_HOST_NAME);             
        str_oid(OID_NETWORK_HOST_NAME_ORIGIN);         
        str_oid(OID_NETWORK_HOST_NAME_PREF_ORIGIN);    
        str_oid(OID_NETWORK_HOST_NAME_DEFAULT);  
        str_oid(OID_NETWORK_DOMAIN_NAME);
        str_oid(OID_NETWORK_DOMAIN_NAME_ORIGIN);
        str_oid(OID_NETWORK_DOMAIN_NAME_PREF_ORIGIN);
        str_oid(OID_NETWORK_DOMAIN_NAME_MANUAL);
        str_oid(OID_NETWORK_RESTORE_HOST_DOMAIN_NAMES);
        str_oid(OID_NETWORK_CONNECTION_TIMEOUT);
        str_oid(OID_NETWORK_LINK_SPEED);
        str_oid(OID_NETWORK_LINK_CONFIG);
        str_oid(OID_NETWORK_LPD_ENABLED);
        str_oid(OID_NETWORK_SLP_ENABLED);
        str_oid(OID_NETWORK_IPV4_ENABLED);
        str_oid(OID_NETWORK_IPV4_DOMAIN_NAME);
        str_oid(OID_NETWORK_IPV4_HOST_NAME);
        str_oid(OID_NETWORK_IPV4_LAST_CONFIGURED_BY);
        str_oid(OID_NETWORK_IPV4_MANUAL_CONFIG);
        str_oid(OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG);
        str_oid(OID_NETWORK_IPV4_ALLOW_BOOTP_CONFIG);
        str_oid(OID_NETWORK_IPV4_ALLOW_AUTOIP_CONFIG);
        str_oid(OID_NETWORK_IPV4_DHCP_SERVER_ADDR);
        str_oid(OID_NETWORK_IPV4_DHCP_LEASE_TIME);
        str_oid(OID_NETWORK_IPV4_ADDRESS);
        str_oid(OID_NETWORK_IPV4_SUBNET_MASK);
        str_oid(OID_NETWORK_IPV4_WINS_SERVER);
        str_oid(OID_NETWORK_IPV4_DEFAULT_GATEWAY);
        str_oid(OID_NETWORK_IPV4_WINS_CONFIGURED);
        str_oid(OID_NETWORK_REACTIVATE_IFACE);
        str_oid(OID_NETWORK_RESET);
        str_oid(OID_NETWORK_IPV6_SUPPORT);    
        str_oid(OID_NETWORK_IPV6_ENABLED);  
        str_oid(OID_NETWORK_IPV6_DOMAIN_NAME);
        str_oid(OID_NETWORK_IPV6_HOST_NAME);
        str_oid(OID_NETWORK_IPV6_LINK_LOCAL_ADDRESS);
        str_oid(OID_NETWORK_IPV6_LINK_LOCAL_PREFIX_LEN);
        str_oid(OID_NETWORK_IPV6_DHCPV6_ADDRESS);
        str_oid(OID_NETWORK_IPV6_DHCPV6_PREFIX_LEN);
        str_oid(OID_NETWORK_IPV6_DHCPV6_PREFERRED_LIFETIME);
        str_oid(OID_NETWORK_IPV6_DHCPV6_VALID_LIFETIME);
        str_oid(OID_NETWORK_IPV6_STATELESS_ADDRESS);
        str_oid(OID_NETWORK_IPV6_STATELESS_PREFIX_LEN);
        str_oid(OID_NETWORK_IPV6_STATELESS_PREFERRED_LIFETIME);
        str_oid(OID_NETWORK_IPV6_STATELESS_VALID_LIFETIME);
        str_oid(OID_NETWORK_DHCPV6_ENABLED);
        str_oid(OID_NETWORK_IPV6_PREFERRED_ADDR);
        str_oid(OID_NETWORK_IPV6_STATUS);
        str_oid(OID_NETWORK_BONJOUR_ENABLED);
        str_oid(OID_NETWORK_BONJOUR_SERVICE_NAME);
        str_oid(OID_NETWORK_BONJOUR_DOMAIN_NAME);
        str_oid(OID_NETWORK_STAT_TOT_PACKETS); 
        str_oid(OID_NETWORK_STAT_TOT_PACKETS_RECV);
        str_oid(OID_NETWORK_STAT_TOT_PACKETS_XMITTED);
        str_oid(OID_NETWORK_STAT_BCAST_PACKETS_XMITTED);
        str_oid(OID_NETWORK_STAT_BCAST_PACKETS_RECV);
        str_oid(OID_NETWORK_STAT_UNICAST_PACKETS);
        str_oid(OID_NETWORK_STAT_UNICAST_PACKETS_RECV);
        str_oid(OID_NETWORK_STAT_UNICAST_PACKETS_XMITTED);
        str_oid(OID_NETWORK_STAT_FRAMING_ERRORS);
        str_oid(OID_NETWORK_STAT_FRAMING_ERRORS_RECV);
        str_oid(OID_NETWORK_STAT_FRAMING_ERRORS_XMITTED);
        str_oid(OID_NETWORK_STAT_BAD_PACKETS);
        str_oid(OID_NETWORK_STAT_BAD_PACKETS_RECV);
        str_oid(OID_NETWORK_STAT_BAD_PACKETS_XMITTED);
        str_oid(OID_NETWORK_STAT_COLLISIONS_XMITTED);
        str_oid(OID_NETWORK_STAT_COLLISIONS_XMITTED_LATE);
        str_oid(OID_NETWORK_SNMP_ENABLED);
        str_oid(OID_NETWORK_SNMP_RW_ENABLED);
        str_oid(OID_NETWORK_SNMP_COMMUNITY_SET_STRING);
        str_oid(OID_NETWORK_SNMP_COMMUNITY_GET_STRING);
        str_oid(OID_NETWORK_SNMP_PUBLIC_ACCESS_EN);
        str_oid(OID_NETWORK_IP_ADDRESS_FROM_CONNECTION);
        str_oid(OID_NETWORK_UAP_SUPPORT);
        str_oid(OID_NETWORK_WSD_ENABLED);
        str_oid(OID_NETWORK_IN_USE);
        str_oid(OID_PRTMIB_TEST_STRING);
        str_oid(OID_PRTMIB_TEST_UNSIGNEDNUM);
        str_oid(OID_PRTMIB_TEST_SIGNEDNUM);
        str_oid(OID_PRTMIB_TEST_NUMLIST);
        str_oid(OID_PRTMIB_TEST_ROWCOUNT);
        str_oid(OID_PRTMIB_DEFAULT_ORIENTATION);
        str_oid(OID_NETWORK_PPPOE_ENABLE);
        str_oid(OID_NETWORK_PPPOE_STATE);
        str_oid(OID_NETWORK_PPPOE_USERNAME);
        str_oid(OID_NETWORK_PPPOE_PASSWORD);
        str_oid(OID_WIFI_INTF_HW_ENABLED);
        str_oid(OID_WIFI_INTF_ENABLED);
        str_oid(OID_UAP_INTF_ENABLED);
        str_oid(OID_WIFI_INTF_SSID);
        str_oid(OID_UAP_INTF_SSID);
        str_oid(OID_WIFI_INTF_SSID_PREFIX);
        str_oid(OID_WIFI_INTF_SEC_MODE);
        str_oid(OID_WIFI_INTF_WPA_ENCR_MODE);
        str_oid(OID_WIFI_INTF_WPA_PASSPHRASE);
        str_oid(OID_WIFI_INTF_WEP_AUTH_MODE);
        str_oid(OID_WIFI_INTF_WEP_KEY_LEN);
        str_oid(OID_WIFI_INTF_WEP_KEY_1);
        str_oid(OID_WIFI_INTF_WEP_KEY_2);
        str_oid(OID_WIFI_INTF_WEP_KEY_3);
        str_oid(OID_WIFI_INTF_WEP_KEY_4);
        str_oid(OID_WIFI_INTF_WEP_CUR_KEY);
        str_oid(OID_WIFI_INTF_WEP_CUR_KEY_VALUE);
        str_oid(OID_WIFI_INTF_CHANNEL);
        str_oid(OID_WIFI_INTF_SIGNAL_STRENGTH);
        str_oid(OID_WIFI_INTF_MAC_ADDR);
        str_oid(OID_WIFI_INTF_BSSID);
        str_oid(OID_WIFI_UAP_GET_DHCP_ADDR);
        str_oid(OID_WIFI_INTF_LOCALE);
        str_oid(OID_WIFI_INTF_NUM_ATTACHED_DEVICES);
        str_oid(OID_WIFI_HOTSPOT_ENABLED);
        str_oid(OID_WIFI_HOTSPOT_RESTORE);
        str_oid(OID_WIFI_HOTSPOT_REBOOT);
        str_oid(OID_WIFI_INTF_START_WPS);
        str_oid(OID_WIFI_INTF_STOP_WPS);
        str_oid(OID_WIFI_INTF_WPS_PIN);
        str_oid(OID_WIFI_INTF_COMM_MODE);
        str_oid(OID_WIFI_INTF_SSID_SUFFIX_LEN);
        str_oid(OID_WIFI_INTF_GENERATE_WPS_PIN);
        str_oid(OID_WIFI_INTF_WLAN_STATUS);
        str_oid(OID_WIFI_INTF_DOWNLINK_COUNT);
        str_oid(OID_WIFI_UAP_DHCP_ADDR);
        str_oid(OID_WIFI_UAP_DHCP_ADDR_PREFIX);
        str_oid(OID_WIFI_UAP_DHCP_SUBNET_MASK);
        str_oid(OID_WIFI_UAP_DHCP_START_ADDR);
        str_oid(OID_WIFI_UAP_DHCP_END_ADDR);
        str_oid(OID_WIFI_UAP_DHCP_NUM_ADDRS);
        str_oid(OID_WIFI_UAP_DHCP_LEASE_TIME);
        str_oid(OID_WIFI_INTF_MAC_FILTER_MODE);
        str_oid(OID_WIFI_INTF_MAC_ADD_FILTER);
        str_oid(OID_WIFI_INTF_MAC_REMOVE_FILTER);
        str_oid(OID_WIFI_INTF_MAC_FILTER_COUNT);
        str_oid(OID_WIFI_INTF_MAC_FILTER_LIST_UAP);
        str_oid(OID_WIFI_INTF_MAC_FILTER_LIST);
        default:
            sprintf(heap_str, "<network oid x%x>", oid);
    }
    return NULL;
}

// network specific api
char* net_get_oid_literal_str(oid_t oid)
{
    return get_oid_literal(oid);
}

#ifdef __cplusplus
}
#endif

//eof
