/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _COMMON_NET_API_H
#define _COMMON_NET_API_H

#include <stdint.h>
#include "error_types.h"

// common header
#include "oid_api.h" // oid_t
#include "posix_ostools.h" // timer_t
#include "delay_api.h" // delay_msec_timer_t

// #########TODO move to common network location
/**
 * @name Return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
typedef error_type_t net_rcode_t;

#define NOT_BOUND           -1
#define NET_OK              OK
#define NET_FAIL            FAIL
#define NET_BAD_INDEX       -11 ///< interface index out of range
#define NET_BAD_PARAM       -12 ///< invalid parameter value
#define NET_NOT_PERMITTED   -13 ///< operation not permitted
#define NET_WOULD_BLOCK      1
#define NET_IN_PROGRESS      2


//=========================================================================================================================
// Oneshot Timers
//=========================================================================================================================

/*
 * Handle to a network oneshot timer instance
 */
typedef delay_msec_timer_t* net_timer_handle_t;
#define NET_TIMER_INVALID_HANDLE NULL

/**
 * @name Timer flags
 */
typedef uint32_t net_timer_flags_t;

#define NET_TIMER_FLAG_PERIODIC         0x00000001 /// periodic vs. oneshot (default oneshot)
#define NET_TIMER_FLAG_REAL_TIME        0x00000010 /// Accounts for time lost while in sleep state (default OS time)

typedef void (*net_timer_expiration_func_t)(void *user_data);

void net_utils_init(void);

// network general purpose utility routines

// millisecond timer
net_timer_handle_t net_timer_start_ms(
    net_timer_flags_t flags,
    uint32_t timer_ms,
    net_timer_expiration_func_t expiration_func,
    void *user_data);

// tick timer
net_timer_handle_t net_timer_start_ticks(
    net_timer_flags_t flags,
    uint32_t timer_ticks,
    net_timer_expiration_func_t expiration_func,
    void *user_data);


void net_timer_cancel(net_timer_handle_t timer_handle);


char net_base64_uint8_to_char(uint8_t i);



// #########TODO remove this!! no longer used
/**
 * @brief Network Link types *** DEPRECATED ***
 * 
 * DEPRECATED: For any new links, use use direct mapping to link name via OEM config.
 * 
 * All network link-layer types supported by the system must be enumerated below.  However, it
 * should not be assumed that all the link types enumerated below are available on the system.
 * 
 * A link type becomes available to the system only after successful registration with the network
 * interface.  During system startup, link-layers must register their implementation of the abstract
 * network link interface defined by the abstract link base class.  This is done by populating the
 * net_iface_link_func_table_t dispatch table and calling net_iface_register_link().
 */
typedef enum
{
    NET_IFACE_LINK_TYPE_NONE = -1,
    NET_IFACE_LINK_TYPE_LO = 0,
    NET_IFACE_LINK_TYPE_ETH,  // wired
    NET_IFACE_LINK_TYPE_WLAN, // WiFi Station
    NET_IFACE_LINK_TYPE_STA = NET_IFACE_LINK_TYPE_WLAN, // WiFi Station
    NET_IFACE_LINK_TYPE_UAP,  // WiFi uAP
    NET_IFACE_LINK_TYPE_WLAN_UAP = NET_IFACE_LINK_TYPE_UAP, // WiFi uAP
    NET_IFACE_LINK_TYPE_WLAN_WFD_GROUP, // Wifi-direct (group owner)
    NET_IFACE_LINK_TYPE_WLAN_WFD_CLIENT, // Wifi-direct (client)

    /* insert new link IDs above this line */
    NET_IFACE_NUM_LINK_TYPES
} net_iface_link_type_t;

// parallel (generic) 'net_iface_link'
typedef enum
{
    NET_IF_TYPE_NONE = -1,
    NET_IF_TYPE_LO = 0,
    NET_IF_TYPE_ETH, // wired
    NET_IF_TYPE_WLAN, // WiFi STA
    NET_IF_TYPE_STA = NET_IF_TYPE_WLAN, // WiFi STA
    NET_IF_TYPE_UAP, // WiFi uAP
    NET_IF_TYPE_WLAN_UAP = NET_IF_TYPE_UAP, // WiFi uAP
    NET_IF_TYPE_WLAN_WFD_GROUP,
    NET_IF_TYPE_WLAN_WFD_CLIENT,

    /* insert new link IDs above this line */
    NET_IF_NUM_LINK_TYPES
} net_if_type_t;

// #########TODO temp workaround for name-to-link-type mapping -- move to OEM config of respective modules
#ifdef __linux__
#define NET_LO_LINK_NAME   "lo" 
#define NET_ETH_LINK_NAME  "eth0" 
#define NET_MLAN_LINK_NAME "mlan0"
#define NET_WLAN_LINK_NAME NET_MLAN_LINK_NAME
#define NET_STA_LINK_NAME  NET_MLAN_LINK_NAME
#define NET_UAP_LINK_NAME  "uap0"
#define NET_WFD_LINK_NAME  "wfd0"
#else
#define NET_ETH_LINK_NAME   "ethernet" 
#define NET_WLAN_LINK_NAME  "WLAN"
#endif

// #########TODO temp workaround for name-to-link-type mapping
net_iface_link_type_t net_link_name_to_type(const char *link_name);

// #########TODO -- TEMP LOCATION move to OEM config layer
#define NET_MAX_INTERFACES          6
#define NET_MAX_PHYSICAL_LINKS      5
#define NET_IF_NAME_LO           NET_LO_LINK_NAME
#define NET_IF_NAME_ETH          NET_ETH_LINK_NAME // "NetEth"  ///< Unique identifier for the interface on this device.
#define NET_IFACE_INTERFACE_NAME NET_IF_NAME_ETH ///< Typical interface
#define NET_IF_NAME_WLAN         NET_WLAN_LINK_NAME // "NetWifi" ///< Unique identifier for the interface on this device.
#define NET_IF_NAME_STA          NET_IF_NAME_WLAN
#define NET_IF_NAME_UAP          NET_UAP_LINK_NAME // "NetUap" ///< Unique identifier for the interface on this device.
#define NET_IF_NAME_WFD          NET_WFD_LINK_NAME // wifi-direct (group or client)

#include "net_link_api.h"

#ifdef HAVE_ETHERNET
#include "net_eth_api.h"
#include "eth_mac_api.h" // eth_mac_link_status_t
#include "net_eth_vars.h"
#endif

#ifdef HAVE_MDNS
#include "net_mdns_api.h"
#endif
#ifdef HAVE_WLAN
#include "net_wifi_api.h"
#include "net_wlan_api.h"
#include "wifi_intf_api.h"
#endif
#ifdef HAVE_UAP
#include "net_uap_api.h"
#include "net_uap_vars.h" // wifi_intf_get_var_..
#endif
#ifdef HAVE_NET_SERVICES
#include "net_services_api.h"
#endif
#include "net_conf_api.h"

#if ! defined DO_NOT_INCLUDE_IF_H
#ifdef __linux__

// runtime bindings for interface name/index/if_index mapping
#include <net/if.h> // os-provided: unsigned if_nametoindex(char* interface_name);
char* if_indextoname(unsigned int, char* if_name);
extern struct if_nameindex* intf_mapp; // runtime-binding interface list
extern int    sizeof_intf_map; //
extern bool*  intf_status_active; // array by if_index marking (available) active intf

#else // ! __linux__

// oem-specific implementation can override the default impl
extern unsigned int if_nametoindex(char* interface_name);

#endif // ! __linux__
#endif // ! DO_NOT_INCLUDE_IF_H

// return the current network interface index
int get_active_net_if(void);
unsigned int current_if_index(unsigned int index);

// misc
char* get_oid_literal(oid_t);

#endif // _COMMON_NET_API_H

//eof
