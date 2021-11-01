/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_eth.c
  * 
  *  This file contains ethernet link settings.
  * 
  */

#define HAVE_ETH_LINK_STATUS_POLLING NET_LINK_STATUS_CHANGE_POLLING /* see net_iface_api.h */

#ifdef __cplusplus
extern "C" {
#endif

// public headers
#include <stdint.h>
#include <string.h>

// common headers
#include "error_types.h"
#include "os_network.h"
#include "net_api.h"
#include "net_link_api.h" // net_link_event_notify_func_t
#include "net_iface.h"    // net_iface_lookup_by_name()
#include "net_eth.h"  // net_eth_context_t
#include "net_eth_api.h"
#include "net_intf_api.h" // get_intf_mac_addr

#ifdef HAVE_ETH_LINK_STATUS_POLLING

    #define NET_ETH_LINK_STATUS_POLL_PERIOD_MS  1000
    #define NET_ETH_LINK_STATUS_POLL_MS         NET_ETH_LINK_STATUS_POLL_PERIOD_MS
    #define NET_ETH_LINK_STATUS_POLL_TICKS      ((NET_ETH_LINK_STATUS_POLL_PERIOD_MS*SYS_TICK_FREQ)/1000)
    static OS_TIMER link_status_timer;

#endif // HAVE_ETH_LINK_STATUS_POLLING

// debug
#define DBG_PRFX "net.link.eth: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
//off #define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h"

// private utilities
#ifdef HAVE_ETH_LINK_STATUS_POLLING
static void net_eth_timer_cb(union sigval sig); // see general_timer_cb
#endif // HAVE_ETH_LINK_STATUS_POLLING

// eth context defaults
static net_eth_context_t g_net_eth =
{
    .msg_queue_initialized = false,
    .dev_index = 0,
    .prev_link_status = 0 /* revisit ETH_MAC_LINK_STATUS_DOWN */,
    .tx_frames_complete_pending = 0,
    .link_event_notify_func = NULL,
    .treck_interface = 0,
#ifdef HAVE_POWER_MGR
    .phy_hndl = 0, // TODO TEMP for pwr mgr -- normally we should not access phy layer directly from net_eth
    .pwr_level = pwr_mgr_on_e,
#endif // HAVE_POWER_MGR
};

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_ETH_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_lock(&g_net_eth.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#define NET_ETH_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_unlock(&g_net_eth.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

//=========================================================================================================================
// Ethernet class API functions
//=========================================================================================================================

static void net_eth_event_register(net_link_event_notify_func_t notify_func, void *user_data)
{
    DBG_VERBOSE("%s() interface x%x data x%x\n", __func__, notify_func, user_data);
    ASSERT(notify_func);
    g_net_eth.link_event_notify_func = notify_func;
    g_net_eth.link_event_user_data = user_data;
    return;
}

static int net_eth_open(ttUserInterface if_index)
{
#ifdef __linux__
    DBG_VERBOSE("%s() intf x%x\n", __func__, if_index);
    NET_ETH_LOCK_CONTEXT();
    if(g_net_eth.treck_interface) // revisit do nothing ..
    {
        // already open
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_OKAY;
    }
    g_net_eth.tx_frames_complete_pending = 0;
#else
    net_rcode_t eth_mac_rcode;
    eth_mac_rcode = eth_mac_open(g_net_eth.dev_index);
    if(eth_mac_rcode != OK)
    {
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_ERROR;
    }
    // set *after* calling underlying open
    // non-NULL value implies we are fully open able to send/recv
    g_net_eth.treck_interface = if_index;

#endif
    NET_ETH_UNLOCK_CONTEXT();
    return TM_DEV_OKAY;
}

int net_eth_close(ttUserInterface if_index)
{
    NET_ETH_LOCK_CONTEXT();
    if(!g_net_eth.treck_interface)
    {
        // already closed
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_OKAY;
    }

    // set *before* calling underlying close
    // non-NULL value implies we are fully open able to send/recv
    g_net_eth.treck_interface = NULL;

    // rdj 11-NOV-2012: don't process any more acks via net_eth_send_complete. Necessary to
    // prevent accidental callback to eth_mac driver after closing mac below.
    g_net_eth.tx_frames_complete_pending = 0;

    //revisit eth_mac_close(g_net_eth.dev_index);
    NET_ETH_UNLOCK_CONTEXT();
    return(TM_DEV_OKAY);
}

static void net_eth_send_complete(void)
{
    DBG_ERR("%s()!\n", __func__);
}

static int net_eth_send(ttUserInterface if_index, char *data_ptr, int data_len, int flag)
{
    bool is_last = false;
    int send_res = TM_DEV_OKAY;

    NET_ETH_LOCK_CONTEXT();

    if(!g_net_eth.treck_interface)
    {
        send_res = TM_DEV_ERROR;
        goto done;
    }

    // see if is last buffer of the frame
    if(flag == TM_USER_BUFFER_LAST)
    {
        is_last = true;
    }

    // should always get packets in a single buffer for now.
    XASSERT(is_last, flag);

#ifdef __linux__
    net_rcode_t eth_mac_rcode = OK;
    DBG_VERBOSE("%s()! revisit\n", __func__);
#else
    eth_mac_rcode_t eth_mac_rcode;
    eth_mac_rcode = eth_mac_insert_tx_buffer(g_net_eth.dev_index, data_ptr, data_len,
                                             is_last, NET_ETH_SRC_TRECK_XMIT, (void *)0);
#endif
    if(eth_mac_rcode != OK /*ETH_MAC_OK*/)
    {
        send_res = TM_DEV_ERROR;
        goto done;
    }

done:
    NET_ETH_UNLOCK_CONTEXT();

    // send ack to stack on any completed frames
    net_eth_send_complete();

    return send_res;
}

void net_eth_add_multicast(uint8_t* option_ptr, int option_len)
{
    DBG_ERR("%s(x%x,%d)! revisit\n", option_ptr, option_len);
}

static int net_eth_ioctl(ttUserInterface if_index, int req, void *option_ptr, int option_len)
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

    NET_ETH_LOCK_CONTEXT();
    if(!g_net_eth.treck_interface)
    {
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_ERROR;
    }

    if(req & TM_DEV_SET_MCAST_LIST)
    {
        //hash each of the ethernet addresses and set the bit in the hash table
        net_eth_add_multicast((uint8_t *)option_ptr, option_len);

    }

    if(req & TM_DEV_SET_ALL_MCAST)
    {
        //hash each of the ethernet addresses and set the bit in the hash table
        net_eth_add_multicast((uint8_t *)NULL, 0xFFFFFFFF);
    }

#if 0 // revisit TM_DEVO options
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
    if ((req & TM_DEV_IOCTL_OFFLOAD_GET) && option_ptr)
    {
        ttDevIoctlOffloadStruct *ols = (ttDevIoctlOffloadStruct *) option_ptr;

        ols->offloadFlags = TM_DEVO_IP_RECV_CHKSUM_VALIDATE |
                            TM_DEVO_TCP_RECV_CHKSUM_VALIDATE |
                            TM_DEVO_UDP_RECV_CHKSUM_VALIDATE |
                            TM_DEVO_ICMP6_RECV_CHKSUM_VALIDATE;

        DBG_VERBOSE("informing stack of recv offload capabilities 0x%08x\n", ols->offloadFlags);
    }
    if ((req & TM_DEV_IOCTL_OFFLOAD_SET) && option_ptr)
    {
//        ttDevIoctlOffloadStruct *ols = (ttDevIoctlOffloadStruct *) option_ptr;

        // MFM TODO - confirm that the requested offload parameters do not exceed our
        // published capabilities in the GET operation
    }
#endif // HAVE_ETH_RECV_CHKSUM_OFFLOAD
#endif

    if(req & NET_LINK_IOCTL_SEND_COMPLETE)
    {
        net_eth_send_complete();
    }

    NET_ETH_UNLOCK_CONTEXT();
    return TM_DEV_OKAY;
}

static int net_eth_recv(ttUserInterface if_index,
                     char **data_ptr,
                     int *data_len,
                     ttUserBufferPtr user_buf_ptr
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
                     , ttDevRecvOffloadPtr devRecvOffloadPtr
#endif
                     )
{
    if(!g_net_eth.treck_interface)
    {
        return TM_DEV_ERROR;
    }

    void *user_data = NULL;

    ASSERT(data_ptr);
    ASSERT(data_len);
    ASSERT(user_buf_ptr);
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
    ASSERT(devRecvOffloadPtr);
#endif
    *user_buf_ptr = (ttUserBuffer)NULL;
    {
        //revisit eth_mac_rx_flags_t rx_flags;
        //revisit eth_mac_rx_info_t rx_info;
        // big block of unity code here // revisit
    }

    // BUGBUG -- we only return the buffer on success due to Treck bug explained above.
    *user_buf_ptr = (ttUserBuffer)user_data;

    return TM_DEV_OKAY;

}

static int net_eth_get_phys_addr(ttUserInterface if_index, char * phys_addr)
{
    net_rcode_t eth_mac_rcode = OK;
    DBG_VERBOSE("%s() interface x%x\n", __func__, if_index);

    // query interface-layer
    get_intf_mac_addr(NET_ETH_LINK_NAME, (uint8_t *)phys_addr); // cast inplace
    if(eth_mac_rcode != OK)
    {
        return TM_DEV_ERROR;
    }
    return eth_mac_rcode;
}


// private utilities

#ifdef HAVE_ETH_LINK_STATUS_POLLING
static void net_eth_timer_cb(union sigval sig)
{
#ifdef __linux__
    static void* previous_state = NULL; // long-running monitor
    char* current_state; // NULL if disconnected
    // retrieve eth link state (NULL == disconnected, non-NULL connected)
    current_state = get_intf_ipv4_addr(NET_IF_NAME_ETH);
    if (current_state != previous_state)
    {
        DBG_VERBOSE("%s() interface %s state change!\n", __func__, NET_IF_NAME_ETH);
        net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_ETH);
        if (intf) // valid at this time?
        {
            if (current_state) // up?
                 net_iface_link_event_notify(NET_LINK_EVENT_UP, (void*)intf);
            else net_iface_link_event_notify(NET_LINK_EVENT_DOWN, (void*)intf);
        }
    }
    previous_state = (void*)current_state; // cast
#else
    net_eth_send_msg(NET_ETH_MSG_POLL_LINK_STATUS, (void *)0);
#endif
    return;
}
#endif // HAVE_ETH_LINK_STATUS_POLLING

//=========================================================================================================================
// run-time binding for ethernet interface
//=========================================================================================================================

net_rcode_t net_eth_init(net_link_api_funcs_t* api_funcs)
{
    net_rcode_t res = OK;
    DBG_VERBOSE("%s() bindings x%x\n", __func__, api_funcs);

    ASSERT(api_funcs);
    api_funcs->event_register = net_eth_event_register;
    api_funcs->open = net_eth_open;
    api_funcs->close = net_eth_close;
    api_funcs->send = net_eth_send;
    api_funcs->recv = net_eth_recv;
    api_funcs->ioctl = net_eth_ioctl;
    api_funcs->get_phys_addr = net_eth_get_phys_addr;

    g_net_eth.dev_index = 0; // 1st device

#ifdef HAVE_ETH_LINK_STATUS_POLLING
    DBG_VERBOSE("%s() initiate %d ms timer\n", __func__, NET_ETH_LINK_STATUS_POLL_MS);
    res = start_ms_timer_periodic(&link_status_timer, NET_ETH_LINK_STATUS_POLL_MS, net_eth_timer_cb);
    if (res != OK)
    {
        DBG_ERR("%s() %d ms timer failed?!\n", __func__, NET_ETH_LINK_STATUS_POLL_MS);
        ASSERT(link_status_timer);
    }
#endif

    return OK;
}

#ifdef __cplusplus
}
#endif

//eof
