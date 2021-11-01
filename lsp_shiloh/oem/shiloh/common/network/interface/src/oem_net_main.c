/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// Provides the oem-specific network interface init routine.
// Overrides the common/network's default net_iface_init()

// 'intf' layer - binds oem-specific network configuration
//
//    Provides an oem-specific intf-layer lookup function:
//        net_interface_t* net_iface_treck_index_to_local_context(intf);

#ifdef __cplusplus
extern "C" {
#endif

// os includes
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h> // getenv()

// common headers
#include "error_types.h"
#include "os_network.h"
#include "net_api.h"
#include "net_link_api.h"
#include "net_iface_api.h"
#include "net_iface_config.h"
#include "net_conf_api.h" // DEFAULT_PLATFORM_HOSTNAME
#include "net_iface.h" // net_iface_enumerate_interfaces()
// Debug configuration
#define DBG_PRFX "oem.net.main: "
#define LOGGER_MODULE_MASK (DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_IFACE) 
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
#include "net_debug.h"


#ifdef HAVE_HOTSPOT_SUPPORT
#include "wifi_intf_vars.h"
#endif // HAVE_HOTSPOT_SUPPORT

#ifdef TM_USE_PPPOE_CLIENT
#include "uuid.h"
#endif // TM_USE_PPPOE_CLIENT

#include "os_system.h"    // os_system()
#if defined HAVE_WIRELESS
#include "net_wifi_api.h" // net_wifi_init()
#ifdef HAVE_UAP
#include "net_uap_api.h"  // net_uap_init_main()
#endif
#endif

//=========================================================================================================================
// Internal configuration defaults
//=========================================================================================================================

// oem-specific globals

net_interface_t g_net_lo =
    {
        .name = NET_LO_LINK_NAME,
        .link_handle = NET_IF_TYPE_LO,
        .local_idx = NET_IF_TYPE_LO,
    };

net_interface_t g_net_eth =
    {
        .name = NET_ETH_LINK_NAME,
        .link_handle = NET_IF_TYPE_ETH,
        .local_idx = NET_IF_TYPE_ETH,
    };

#ifdef HAVE_WLAN
net_interface_t g_net_wlan =
    {
        .name = NET_WLAN_LINK_NAME,
        .link_handle = NET_IF_TYPE_STA,
        .local_idx = NET_IF_TYPE_WLAN,
    };
#endif // HAVE_WLAN

#ifdef HAVE_UAP
net_interface_t g_net_uap =
    {
        .name = NET_UAP_LINK_NAME,
        .link_handle = NET_IF_TYPE_UAP,
        .local_idx = NET_IF_TYPE_WLAN_UAP,
    };
#endif // HAVE_UAP

net_interface_t* g_net_interface = &g_net_eth; // default to wired eth
net_iface_context_t g_interface_context =
{
   .iface_cnt = 0, // runtime determined
   .ifaces = {NULL,NULL,NULL,NULL,NULL,NULL},
   .event_list_head = NULL,
   .initialized = false,
   .mtx = {},
   .oem_func_table = {},
   .oem_config = {},
};

net_iface_oem_config_t oem_config[NET_MAX_INTERFACES]
= {
};


//=========================================================================================================================
// Network interface context
//=========================================================================================================================

uint16_t gPowerCycleCnt = 0;

//=========================================================================================================================
// Module entry point
//=========================================================================================================================

// override common/network's net_iface_init() w/oem-custom network definition
void net_iface_init(void)
{
    DBG_VERBOSE("==>%s-oem\n",__func__);

#ifdef HAVE_NETIO
    // net IO init
    net_io_init();
#endif

#ifdef HAVE_CMD
    dbg_raw_socket_init();
#endif

#ifdef HAVE_NET_RAW_IO
    // raw IO init
    net_raw_io_init();
#endif

#ifdef HAVE_NETPRINT
    // network print init
    net_print_init();
#endif

#ifdef HAVE_NET_LPD
    // LPD init
    net_lpd_init();
#endif


#ifdef HAVE_NET_SCAN
    // network scan init
    net_scan_init();
#endif

    // initialize link-layer
    net_utils_init(); // network timers, etc..
    net_link_init();  // link-layer state machine (e.g, mac/phy events)

#if defined HAVE_WIRELESS
    net_conf_init(); // vars & cmds
    net_wifi_init(); // vars oids & cmds
#endif

#ifdef HAVE_NET_SERVICES
    net_services_init();
    logger_submodule_register(DEBUG_LOGGER_MODULE_NETWORK, 12, "services"); /* SERVICES */
#endif

#ifdef HAVE_ETHERNET
    net_iface_task(0); // needs link(s)
#endif

    DBG_VERBOSE("%s-oem() done!\n", __func__);
}

//=========================================================================================================================
// Public Network Interface Notification
//=========================================================================================================================

// oem-replacement for a 'treck interface lookup' -- use g_net_context->ifaces[] directly
net_interface_t* net_iface_index_to_local_context(unsigned if_index)
{
    net_interface_t* intf = NULL;
    char if_name_buf[IFNAMSIZ], *if_name = if_name_buf;
    if_name = if_indextoname(if_index, if_name_buf);
    if (if_name != NULL)
    {
        intf = net_iface_lookup_by_name(if_name);
        DBG_VERBOSE("%s(%d) -> %#x (%s)\n", __func__, index, intf, intf? intf->name: "<unbound>");
    }
    return intf;
}

// oem-replacement for a (dynamic) os interface if_index -> oem interface lookup
net_interface_t* net_if_index_to_local_context(unsigned if_index)
{
    net_interface_t* intf = NULL;

    char if_name_buf[IFNAMSIZ], *if_name = if_name_buf;
    if_name = if_indextoname(if_index, if_name);
    if (if_name != NULL)
    {
        intf = net_iface_lookup_by_name(if_name);
        if (intf)
        {
            DBG_ASSERT( intf->treck_idx == if_index);
        }
    }
    return intf;
}

// treck specific impl (oem override)
net_interface_t* net_iface_treck_index_to_local_context(unsigned index) // offset or if_index tbd?
{
    return net_iface_index_to_local_context(index);
}

#ifdef __cplusplus
}
#endif

// eof oem_net_main.c
