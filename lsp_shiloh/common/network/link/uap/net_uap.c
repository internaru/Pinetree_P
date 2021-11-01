/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_uap.c
  * 
  *  This file contains UAP-specfic details for the wifi-uap module.
  * 
  */

#define HAVE_UAP_LINK_STATUS_POLLING NET_LINK_STATUS_CHANGE_POLLING /* see net_iface_api.h */

#ifdef __cplusplus
extern "C" {
#endif

// public headers
#include <stdint.h>
#include <string.h>

// common headers
#include "error_types.h"
#include "os_network.h"   // generic network
#include "net_api.h"
#include "net_iface.h"    // net_iface_lookup_by_name()
#include "net_uap_api.h"  // net_uap_init_common
#include "net_uap.h"      // net_uap_context_t
#include "net_intf_api.h" // get_intf_mac_addr()

#ifdef HAVE_UAP_LINK_STATUS_POLLING

    #define NET_UAP_LINK_STATUS_POLL_PERIOD_MS  1000
    #define NET_UAP_LINK_STATUS_POLL_MS         NET_UAP_LINK_STATUS_POLL_PERIOD_MS
    #define NET_UAP_LINK_STATUS_POLL_TICKS      ((NET_UAP_LINK_STATUS_POLL_PERIOD_MS*SYS_TICK_FREQ)/1000)
    static OS_TIMER link_status_timer;

#endif // HAVE_UAP_LINK_STATUS_POLLING

// debug
#define DBG_PRFX "net.link.uap: "
#define DBG_ON /* DBG_MSG */
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h"

// private utilities
#ifdef HAVE_UAP_LINK_STATUS_POLLING
static void net_uap_timer_cb(union sigval sig); // see general_timer_cb
#endif // HAVE_UAP_LINK_STATUS_POLLING

// uap context defaults
static net_uap_context_t g_net_uap =
{
    .msg_queue_initialized = false,
    .dev_index = 0,
    .prev_link_status = 0 /* revisit UAP_MAC_LINK_STATUS_DOWN */,
    .tx_frames_complete_pending = 0,
    .link_event_notify_func = NULL,
    //revisit .treck_interface = NULL,
#ifdef HAVE_POWER_MGR
    .phy_hndl = 0, // TODO TEMP for pwr mgr -- normally we should not access phy layer directly from net_uap
    .pwr_level = pwr_mgr_on_e,
#endif // HAVE_POWER_MGR
};

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_UAP_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_lock(&g_net_uap.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#define NET_UAP_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_unlock(&g_net_uap.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

//=========================================================================================================================
// UAP class API functions
//=========================================================================================================================

static void net_uap_event_register(net_link_event_notify_func_t notify_func, void *user_data)
{
    // need more substance
    DBG_VERBOSE("%s() link x%x data x%x\n", __func__, notify_func, user_data);

    ASSERT(notify_func);
    g_net_uap.link_event_notify_func = notify_func;
    g_net_uap.link_event_user_data = user_data;
    return;
}

static int net_uap_open(ttUserInterface treck_interface)
{
    DBG_ERR("%s() stubbed! revisit\n", __func__);
    return TM_DEV_OKAY;
}

int net_uap_close(ttUserInterface treck_interface)
{
    DBG_ERR("%s() stubbed! revisit\n", __func__);
    g_net_uap.treck_interface = NULL;
    g_net_uap.tx_frames_complete_pending = 0;

    return(TM_DEV_OKAY);
}

static int net_uap_send(ttUserInterface treck_interface, char *data_ptr, int data_len, int flag)
{
    int send_res = TM_DEV_OKAY;

    DBG_ERR("%s() stubbed! revisit\n", __func__);

    return send_res;
}

static int net_uap_ioctl(ttUserInterface if_index, int req, void *option_ptr, int option_len)
{
    switch (req)
    {
    case NET_LINK_IOCTL_ENABLE:
        DBG_VERBOSE("%s()[%d] up ..\n", __func__, if_index); break;
    case NET_LINK_IOCTL_DISABLE:
        DBG_VERBOSE("%s()[%d] down ..\n", __func__, if_index); break;
    case NET_LINK_IOCTL_DOWN_ACK:
    default:
        DBG_ERR("%s(%d)[%d] !impl revisit\n", __func__, req, if_index);
        break;
    }
    return TM_DEV_OKAY;
}

static int net_uap_recv(ttUserInterface treck_interface,
                     char **data_ptr,
                     int *data_len,
                     ttUserBufferPtr user_buf_ptr
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
                     , ttDevRecvOffloadPtr devRecvOffloadPtr
#endif
                     )
{
    DBG_ERR("%s() stubbed! revisit\n", __func__);
    return TM_DEV_ERROR;
}

static int net_uap_get_phys_addr(ttUserInterface treck_interface, char * phys_addr)
{
    net_rcode_t mac_rcode = OK;
    DBG_VERBOSE("%s() intf x%x\n", __func__, treck_interface);

    // possibly more direct retrieval?
    (void)get_intf_mac_addr(NET_UAP_LINK_NAME, (uint8_t *)phys_addr); // returns buf addr
    if(mac_rcode != OK)
    {
        return TM_DEV_ERROR;
    }
    return mac_rcode;
}

// private utilities

#ifdef HAVE_UAP_LINK_STATUS_POLLING
static void net_uap_timer_cb(union sigval sig)
{
#ifdef __linux__
    static void* previous_state = NULL; // long-running monitor
    char* current_state; // NULL if disconnected
    // retrieve uap link state (NULL == disconnected, non-NULL connected)
    current_state = get_intf_ipv4_addr(NET_IF_NAME_UAP);
    if (current_state != previous_state)
    {
        DBG_VERBOSE("%s() interface %s state change!\n", __func__, NET_IF_NAME_UAP);
        net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_UAP);
        if (intf) // valid at this time?
        {
            if (current_state) // up?
                 net_iface_link_event_notify(NET_LINK_EVENT_UP, (void*)intf);
            else net_iface_link_event_notify(NET_LINK_EVENT_DOWN, (void*)intf);
        }
    }
    previous_state = (void*)current_state; // cast
#else
    net_uap_send_msg(NET_UAP_MSG_POLL_LINK_STATUS, (void *)0);
#endif
    return;
}
#endif // HAVE_UAP_LINK_STATUS_POLLING

//=========================================================================================================================
// run-time binding for wireless/uap interface
//=========================================================================================================================

net_rcode_t net_uap_init(net_link_api_funcs_t* api_funcs)
{
    DBG_VERBOSE("%s() bindings x%x\n", __func__, api_funcs);

    ASSERT(api_funcs);
    api_funcs->event_register = net_uap_event_register;
    api_funcs->open = net_uap_open;
    api_funcs->close = net_uap_close;
    api_funcs->send = net_uap_send;
    api_funcs->recv = net_uap_recv;
    api_funcs->ioctl = net_uap_ioctl;
    api_funcs->get_phys_addr = net_uap_get_phys_addr;

    g_net_uap.dev_index = 0; // 1st device (although 1 device 1-or-more intfs)

#ifdef HAVE_UAP_LINK_STATUS_POLLING
    DBG_VERBOSE("%s() initiate %d ms timer\n", __func__, NET_UAP_LINK_STATUS_POLL_MS);
    error_type_t res = start_ms_timer_periodic(&link_status_timer, NET_UAP_LINK_STATUS_POLL_MS, net_uap_timer_cb);
    if (res != OK)
    {
        DBG_ERR("%s() %d ms timer failed?!\n", __func__, NET_UAP_LINK_STATUS_POLL_MS);
        ASSERT(link_status_timer);
    }
#endif

    //  finish w/common uap
    return net_uap_init_common();
}

#ifdef __cplusplus
}
#endif

//eof
