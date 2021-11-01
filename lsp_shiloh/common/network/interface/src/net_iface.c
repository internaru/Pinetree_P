/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// common headers
#include "os_network.h"
#include <ifaddrs.h> // getifaddrs()
#include <netdb.h> // NI_NUMERICHOST()

// debug
#define DBG_PRFX "net.if: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
#define EXTRA_LOCAL_DEBUG 0 /* additional dbg when enabled (non-zero) */
#define LOGGER_SUBMOD_IFACE 0x80000000
#define LOGGER_MODULE_MASK (DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMOD_IFACE)
#include "net_debug.h"
#define DBG_INFO DBG_MSG /* align with logger set log_info */

// common headers
#include "net_api.h" // network apis
#include "net_iface.h" // net_iface_context_t net_interface_t

// ipv6 addr accessors
#ifdef __linux__
#define ipv6_addr_field(addr) &((struct sockaddr_in6*)(addr))->sin6_addr
#define ipv6_addr_offset(saddr) (void*)&(*saddr.__ss_padding) /* revisit */
#else
#define ipv6_addr_field(addr) (&saddr->addr.ipv6.sin6_addr)
#endif

#if EXTRA_LOCAL_DEBUG // extra debug on?

#define adv_state(intf, updated_state) \
{ char* if_name = (intf?intf->name:"<unbound intf>"); UNUSED_VAR(if_name); dbg_printf("adv_if(intf x%x '%s' state %d -> %d):%d\n", intf, if_name, intf->state, updated_state, __LINE__); intf->state = updated_state; }
#define adv_v4_state(intf, updated_state) \
{ char* if_name = (intf?intf->name:"<unbound intf>"); UNUSED_VAR(if_name); dbg_printf("adv_v4(intf x%x '%s' state %d -> %d):%d\n", intf, if_name, intf->ipv4_cfg_state, updated_state, __LINE__); intf->ipv4_cfg_state = updated_state; }
#define add_v4_state(intf, updated_state) \
{ char* if_name = (intf?intf->name:"<unbound intf>"); UNUSED_VAR(if_name); dbg_printf("add_v4(intf x%x '%s' state %d -> %d):%d\n", intf, if_name, intf->ipv4_cfg_state, updated_state, __LINE__); intf->ipv4_cfg_state |= updated_state; }
#define adv_v6_state(intf, updated_state) \
{ char* if_name = (intf?intf->name:"<unbound intf>"); UNUSED_VAR(if_name); dbg_printf("adv_v6(intf x%x '%s' state %d -> %d):%d\n", intf, if_name, intf->ipv6_cfg_state, updated_state, __LINE__); intf->ipv6_cfg_state = updated_state; }
#define add_v6_state(intf, updated_state) \
{ char* if_name = (intf?intf->name:"<unbound intf>"); UNUSED_VAR(if_name); dbg_printf("add_v6(intf x%x '%s' state %d -> %d):%d\n", intf, if_name, intf->ipv6_cfg_state, updated_state, __LINE__); intf->ipv6_cfg_state |= updated_state; }
#undef DBG_IGNORED
#define DBG_IGNORED DBG_MSG

static void dbg_noisy(const char* func, net_interface_t* iface)
{
    char* str_v4_state = str_ipv4_cfg_state(iface->ipv4_cfg_state);
    char* str_v6_state = str_ipv6_cfg_state(iface->ipv6_cfg_state);
    dbg_printf("%s() %5s (state v4 %s:%d v6 %s:%d\n", func, iface->name,
        str_v4_state, iface->ipv4_cfg_state, str_v6_state, iface->ipv6_cfg_state);

    dbg_printf("%s() %5s v4 config method %d dhcp-started(v4/v6 %d/%d)\n", func, iface->name,
        iface->ipv4_pending_config,
        iface->ipv4_dhcp_started,
        iface->ipv6_dhcp_started
    );

}
#define show_current_if_states() dbg_noisy(__func__, iface) /* both __func__ and iface lexical-scope */

#else
#define adv_state(intf, updated_state) intf->state = updated_state
#define adv_v4_state(intf, updated_state) intf->ipv4_cfg_state = updated_state
#define add_v4_state(intf, updated_state) intf->ipv4_cfg_state |= updated_state
#define adv_v6_state(intf, updated_state) intf->ipv6_cfg_state = updated_state
#define add_v6_state(intf, updated_state) intf->ipv6_cfg_state |= updated_state
#define DBG_IGNORED(...) { (void)0; } /* optimize out */
#define show_current_if_states(...) { (void)0; } /* optimize out */
#define dbg_noisy(...) { (void)0; }
#endif

#ifdef HAVE_NETIO
#include "net_io_api.h"
#endif
#ifdef HAVE_NET_RAW_IO
#include "net_raw_io_api.h"
#endif
#ifdef HAVE_NETPRINT
#include "net_print_api.h"
#endif

#ifdef HAVE_NET_LPD
#include "net_lpd_api.h"
#endif

#ifdef HAVE_NET_SCAN
#include "net_scan_api.h"
#endif

// global boot entry
UserBootEntry_t g_boot_entry_default;

#define NET_IFACE_MQ_NAME         "/if-msgq"
#define NET_IFACE_THREAD_NAME     "if-layer"
#define NET_IFACE_MQ_NUM_MSG_BUFS 8

typedef struct net_iface_msg_s
{
    net_iface_msg_type_t type;
    net_interface_t*     iface;
    void                *udata;
    void                *data2; // type-specific message data
} net_iface_msg_t;

typedef union net_iface_msg_buf_s
{
    net_iface_msg_t msg;
    uint8_t         buf[16];
} net_iface_msg_buf_t;

// net iface state machine
#define NET_IFACE_THREAD_STACK_SIZE PTHREAD_STACK_MIN
static pthread_t net_iface_thread; // thread on which state machine runs
static ALIGN8 uint8_t net_iface_stack[NET_IFACE_THREAD_STACK_SIZE];
net_iface_msg_buf_t   g_net_iface_msg_buf[NET_IFACE_MQ_NUM_MSG_BUFS]; // msg queue buffer space
static mqd_t          g_net_iface_msg_queue; // msg queue control block

#define g_net_ctxt (*g_net_context) /* revisit */

// internal defaults

#define NET_RESET_DELAY_SEC                 2
/*  When netdrvr detects a configuration change that requires a network reset to incorporate the
 *  change, the netdrvr state machine will delay NET_RESET_DELAY_SEC seconds before triggering the
 *  reset.  Network reset involves a lengthy reconfiguration sequence, and the delay helps ensure
 *  that successive network configuration changes that are all made within a short period of time
 *  will invoke only a single network reset.
 */

// local headers
#include "net_iface_api.h"
#include "os_system.h"

// interface configuration
#ifdef __linux__
#define STATIC
#else
#define STATIC static
#endif // ! __linux__
STATIC void open_interface(net_interface_t *iface);
STATIC void close_interface(net_interface_t *iface);
STATIC void finish_close_interface(net_interface_t *iface);
STATIC void ipv4_open_interface(net_interface_t *iface);
STATIC void ipv4_open_interface_complete(net_interface_t* iface);
STATIC void ipv4_reconfigure_interface(net_interface_t *iface);
STATIC int32_t ipv4_close_interface(net_interface_t *iface);
#ifdef __linux__
STATIC void ipv4_addr_notify_func(net_interface_t* iface, int boot_notify_rcode, int multihome_idx);
#else
STATIC void ipv4_boot_notify_func(ttUserInterface treck_interface, int boot_notify_rcode, int multihome_idx);
#endif
STATIC void ipv4_finish_close_interface(net_interface_t *iface);
STATIC void ipv4_cancel_collision_detection(net_interface_t *iface);
STATIC void ipv4_add_autoip_static_route(net_interface_t* iface, uint32_t ipv4_addr);
STATIC void ipv4_open_interface_manual(net_interface_t *iface, int default_open_flags);
STATIC void ipv4_open_interface_dhcp(net_interface_t *iface, int default_open_flags);
STATIC void ipv4_open_interface_bootp(net_interface_t *iface, int default_open_flags);
STATIC void ipv4_open_interface_autoip(net_interface_t *iface, int default_open_flags);

#ifdef HAVE_IPV6
STATIC void ipv6_open_interface(net_interface_t *iface);
STATIC void ipv6_reconfigure_dhcp(net_interface_t *iface);
STATIC int32_t ipv6_configure_dhcp(net_interface_t *iface);
STATIC int32_t ipv6_close_interface(net_interface_t *iface);
STATIC void ipv6_finish_close_interface(net_interface_t *iface);
#ifdef __linux__
STATIC void ipv6_addr_notify_func(net_interface_t* iface, unsigned int multihome_idx,
                                  struct sockaddr_storage *saddr, int event);
#else
STATIC void ipv6_addr_notify_func(ttUserInterface treck_interface, unsigned int multihome_idx,
                                  struct sockaddr_storage *saddr, int event);
#endif
STATIC void ipv6_dhcp_notify_func(ttUserInterface treck_interface, int ia_idx, int dhcp_addr_idx, tt6DhcpEvent event);
STATIC void ipv6_global_autoconfig_timeout_func(void *user_data);
STATIC void ipv6_local_config_complete(net_interface_t *iface);
STATIC void ipv6_global_config_complete(net_interface_t *iface);
STATIC void ipv6_set_addr_lifetimes(net_interface_t *iface, internal_ipv6_addr_t *internal_addr);
STATIC void ipv6_global_addr_insert(net_interface_t *iface, internal_ipv6_addr_t *internal_addr);
STATIC internal_ipv6_addr_t * ipv6_global_addr_remove(net_interface_t *iface, int multihome_idx);
STATIC net_iface_ipv6_config_method_t ipv6_index_to_method(int multihome_idx);
STATIC void ipv6_unconfigure_linklocal(net_interface_t *iface, unsigned int multihome_idx);
#endif // HAVE_IPV6

// proto
STATIC uint8_t net_iface_save_local_context(net_interface_t *net_interface, ttUserInterface treck_interface);

// netdrvr event registration
static void event_list_remove(net_iface_event_node_t *node);
static void event_list_insert(net_iface_event_node_t *node);

// netdrvr utility/helper routines
static void delayed_msg_timer_func(void *user_data);

// interface-link boundary
uint8_t* get_intf_mac_addr(char* intf_of_interest, uint8_t* given_buf);

#ifdef HAVE_CMD
#include "dbg_raw_socket.h"
#endif

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode = os_mutex_get(&g_net_ctxt.mtx, OS_WAIT_FOREVER); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#define NET_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode = os_mutex_put(&g_net_ctxt.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

// oem interface name lookup (by oem index)
char *iface_name_lookup_by_index(int index)
{
    if (index >= net_iface_enum_get_count()) return NULL;
    return g_net_ctxt.ifaces[index]->name;
}

// depends upon global msg g_net_iface_msg_buf
int check_link_discrepancy(char* iname, int alive)
{
    net_iface_msg_t* msg;
    msg = (net_iface_msg_t*)&g_net_iface_msg_buf;
    if (msg->iface != NULL)
    {
        if(!strcmp(iname,msg->iface->name))
        {	
            if ((msg->type == NET_IFACE_MSG_LINK_UP) && alive == 0)
                return 1;
            else
            if (((msg->type == NET_IFACE_MSG_LINK_DOWN) ||
                 (msg->type == NET_IFACE_MSG_NET_RESET)) && alive == 1)
                return 0;
        }
    }
    return alive;
}

// returns runtime-determined count of network interfaces
uint8_t net_iface_enum_get_count(void)
{
    return g_net_ctxt.iface_cnt;
}

unsigned int net_iface_enum_to_if_index(uint8_t iface_enum)
{
    int number_of_intfs = net_iface_enum_get_count();
    XASSERT(iface_enum < number_of_intfs, iface_enum);
    if (iface_enum > number_of_intfs) return 0; // no if_index
    if(!g_net_ctxt.ifaces[iface_enum])
    {
        DBG_VERBOSE("==>%s(%d) pending..\n", __func__, iface_enum);
        return 0;
    }

#if __linux__
    net_interface_t* intf = g_net_ctxt.ifaces[iface_enum];
    if (intf)
    {
        return if_nametoindex(intf->name); // if_name
    }
    return 0; // err no interface found
#else // treck/threadx
    DBG_VERBOSE("<==%s(%d) %d\n", __func__, iface_enum, g_net_ctxt.ifaces[iface_enum]->treck_idx);
    return g_net_ctxt.ifaces[iface_enum]->treck_idx;
#endif // 
}

// link -> iface notification handler
STATIC void net_iface_link_event_notify(net_link_event_t link_event, void *user_data)
{
    net_iface_rcode_t net_iface_rcode;
    net_interface_t *iface = (net_interface_t *)user_data;

    // unbound interface?
    if (!iface)
    {
        DBG_ERR("%s() event %d ignored\n", __func__, link_event);
        return;
    }

#ifdef DBG_VERBOSE_ON
    {
        char* if_name = iface->name;
        UNUSED_VAR(if_name);
        DBG_VERBOSE("%s(%d) '%s' x%x event %d\n", __func__, link_event,
            (if_name? if_name: "<undefined>"), iface, link_event);
    }
#endif

    switch(link_event)
    {
        case NET_LINK_EVENT_DOWN_PENDING:
        {
            net_iface_rcode = net_send_msg_no_wait(NET_IFACE_MSG_LINK_DOWN_PENDING, iface, (void *)0, (void *)0);
            if(net_iface_rcode == NET_IFACE_WOULDBLOCK)
            {
                // msg queue full
                DBG_ERR("queue full while sending NET_IFACE_MSG_LINK_DOWN_PENDING on %s\n", iface->name);
                DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
            }
        }
        break;

        case NET_LINK_EVENT_DOWN:
        {
            net_iface_rcode = net_send_msg_no_wait(NET_IFACE_MSG_LINK_DOWN, iface, (void *)0, (void *)0);
            if(net_iface_rcode == NET_IFACE_WOULDBLOCK)
            {
                // msg queue full
                DBG_ERR("queue full while sending NET_IFACE_MSG_LINK_DOWN on %s\n", iface->name);
                DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
            }
        } break;

/*
 *  Load a link's OEM configuration and prepare it for being attached later
 */

        case NET_LINK_EVENT_UP:
        {
            net_iface_rcode = net_send_msg_no_wait(NET_IFACE_MSG_LINK_UP, iface, (void *)0, (void *)0);
            if(net_iface_rcode == NET_IFACE_WOULDBLOCK)
            {
                // msg queue full
                DBG_ERR("queue full while sending NET_IFACE_MSG_LINK_UP on %s\n", iface->name);
                DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
            }
        } break;

        case NET_LINK_EVENT_HW_PRESENCE: // hw presence-detect
        {
            DBG_VERBOSE("%s() %s hw detected\n", __func__, iface->name);
#ifdef HAVE_WIRELESS
            uint32_t config_method = NET_IFACE_IPV4_USE_DHCP; // nearly auto
#ifdef HAVE_UAP
 
            net_iface_set_var_ipv4_config_options(iface->treck_idx, &config_method);
            if (!strcmp(iface->name, NET_IF_NAME_UAP))
            {   // revisit tickle to open_interface(iface->name);
                // revisit msg_send ( case NET_IFACE_MSG_LINK_UP_POLL )
#ifdef HAVE_NET_SERVICES
                net_service_request(NET_SERVICE_START, NET_SERVICE_IFCONFIG, NET_IF_NAME_UAP, 0);
#else
                start_uap_service(); // hw present but uninitialized
#endif
            }
#endif // HAVE_UAP
#ifdef HAVE_WLAN
            net_iface_set_var_ipv4_config_options(iface->treck_idx, &config_method);
            if (!strcmp(iface->name, NET_IF_NAME_STA))
            {   // revisit tickle to open_interface(iface->name);
                // revisit msg_send ( case NET_IFACE_MSG_LINK_UP_POLL )
#ifdef HAVE_NET_SERVICES
                net_service_request(NET_SERVICE_START, NET_SERVICE_IFCONFIG, NET_IF_NAME_STA, 0);
#else
                start_sta_service(); // hw present but uninitialized
#endif
            }
#endif // HAVE_WLAN
#endif // HAVE_WIRELESS
        } break;

        case NET_LINK_EVENT_RESET:
            net_config_change_notify(iface->treck_idx);
        break;

        default:
            // unhandled event
            DBG_ERR("%s() unhandled '%s' event %d\n", __func__, iface->name, link_event);
            XASSERT(0, link_event);
        break;
    }

    return;
}

//=========================================================================================================================
// Network event registration
//=========================================================================================================================

/*
 *  intf/link event registration
 */

// register one or more event flags with an event callback
net_iface_event_handle_t net_iface_register_events(
    unsigned int if_index,
    net_iface_event_flags_t events,
    net_iface_event_func_ptr_t event_cb,
    void *data)
{
    ASSERT(event_cb);

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        // bad interface index
// This can happen if static NVRAM has been deleted and MAC addr is zeroed. Don't assert
// because we need FW loaded in order to burn new NVRAM.
//        XASSERT(0, if_index);
        return NULL;
    }

    // create a new node and register the events
    net_iface_event_node_t *node;
    node = (net_iface_event_node_t *)MEM_MALLOC(sizeof(net_iface_event_node_t));
    if(!node)
    {
        ASSERT(0);
        return NULL;
    }
    node->event_cb = event_cb;
    node->iface = iface;
    node->user_context = data;
    node->event_flags = events;
    event_list_insert(node);

    return (net_iface_event_handle_t)node;
}

// deregister any event associated with this callback
void net_iface_deregister_events(net_iface_event_handle_t hndl)
{
    if(!hndl)
    {
        // invalid handle - do nothing
        return;
    }

    net_iface_event_node_t *node = (net_iface_event_node_t *)hndl;
    if(node)
    {
        event_list_remove(node);
        MEM_FREE_AND_NULL(node);
    }

    return;
}

static void event_list_insert(net_iface_event_node_t *node)
{
    // insert at beginning of linked list
    NET_LOCK_CONTEXT();
    node->next = g_net_ctxt.event_list_head;
    g_net_ctxt.event_list_head = node;
    NET_UNLOCK_CONTEXT();
}

// returns NULL if node not found
static void event_list_remove(net_iface_event_node_t *node)
{
    ASSERT(node);

    NET_LOCK_CONTEXT();
    net_iface_event_node_t *cur_node = g_net_ctxt.event_list_head;
    net_iface_event_node_t **prev = &g_net_ctxt.event_list_head;
    while(cur_node)
    {
        if(cur_node == node)
        {
            // remove callback from linked list
            *prev = cur_node->next;
            cur_node->next = NULL;
            break;
        }
        prev = &cur_node->next;
        cur_node = cur_node->next;
    }
    NET_UNLOCK_CONTEXT();

    return;
}

// notify all callbacks registered to receive the events
STATIC void event_list_notify(net_interface_t *iface, net_iface_event_flags_t events)
{
    net_iface_event_node_t *next_node;
    net_iface_event_func_ptr_t node_cb;
    net_iface_event_flags_t tmp_flags;
    void *tmp_ctxt;

#if 0 && DEBUG && defined  __linux__
    //dbg -- generate interface/link event confirmation
    if (   (events & NET_IFACE_EVENT_LINK_STATUS_CHANGE)
        || (events & NET_IFACE_EVENT_IPV4_CONFIG_CHANGE)
#ifdef HAVE_IPV6
        || (events & NET_IFACE_EVENT_IPV6_LOCAL_CONFIG_CHANGE)
        || (events & NET_IFACE_EVENT_IPV6_CONFIG_CHANGE)
#endif
	)
    {
        const char* str_event_fmt = "notify_link_event(%s)"; // __func__
        char str_event[64];
        sprintf(str_event,  str_event_fmt, iface->name);
    
        if (events & NET_IFACE_EVENT_LINK_STATUS_CHANGE)
        {
            strncat(str_event, " LINK_STATUS_CHANGE", sizeof(str_event)-strlen(str_event));
        }
        else if (events & NET_IFACE_EVENT_IPV4_CONFIG_CHANGE)
        {
            strncat(str_event, " IPV4_CONFIG_CHANGE", sizeof(str_event)-strlen(str_event));
        }
#ifdef HAVE_IPV6
        else if (events & NET_IFACE_EVENT_IPV6_LOCAL_CONFIG_CHANGE)
        {
            strncat(str_event, " IPV6_LOCAL_CONFIG_CHANGE", sizeof(str_event)-strlen(str_event));
        }
        else if (events & NET_IFACE_EVENT_IPV6_CONFIG_CHANGE)
        {
            strncat(str_event, " IPV6_CONFIG_CHANGE", sizeof(str_event)-strlen(str_event));
        }
#endif
        strncat(str_event, "\n", sizeof(str_event)-strlen(str_event));
      
        DBG_PRINTF(LOG_NOTICE, str_event); // revisit DBG_INFO
    }
#else // ! DEBUG || __linux__

    // must notify registered callbacks *after* changing state but *before* closing the interface
    DBG_VERBOSE("%s(%#x) event %d\n", __func__, iface, events);

#endif

    NET_LOCK_CONTEXT();
    net_iface_event_node_t *node = g_net_ctxt.event_list_head;
    while(node)
    {
        next_node = node->next; // save now in case node gets remove while unlocked below
        if( (node->iface == iface) && (node->event_flags & events) )
        {
            // save now in case node gets remove while unlocked below
            node_cb = node->event_cb;
            tmp_flags = node->event_flags;
            tmp_ctxt = node->user_context;

            NET_UNLOCK_CONTEXT();
            node_cb(tmp_flags & events, tmp_ctxt);
            NET_LOCK_CONTEXT();
        }
        node = next_node;
    }
    NET_UNLOCK_CONTEXT();

    return;
}

void net_iface_continue_shutdown(net_iface_event_handle_t hndl, net_iface_event_flags_t events)
{
    // TODO implement this
    impl();
    return;
}

//=========================================================================================================================
// Public utility routines
//=========================================================================================================================

int32_t net_send_msg_wait_opt(
    net_iface_msg_type_t type,
    net_interface_t *iface,
    void *data2,
    void *udata,
    unsigned long wait_opt)
{
    net_iface_msg_buf_t buf;

    buf.msg.type = type;
    buf.msg.iface = iface;
    buf.msg.udata = udata;
    buf.msg.data2 = data2;

#ifdef __linux__
    unsigned int os_rcode = posix_message_send(g_net_iface_msg_queue, (char *)&buf, sizeof(buf),
                                            MQ_DEFAULT_PRIORITY, wait_opt);
#else
    static OS_QUEUE       g_msg_queue;
    unsigned int os_rcode = tx_queue_send(&g_msg_queue, &buf, wait_opt);
#endif
    if(os_rcode == OS_QUEUE_FULL)
    {
        /* 
         *  msg queue full - restart the timer to retry
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and wait_opt must
         *  therefore be set to TX_NO_WAIT
         */
        return NET_IFACE_WOULDBLOCK;
    }
    else if(os_rcode != OS_SUCCESS)
    {
        // any other errors are unexpected and unhandled
        DBG_ERR("%s(x%x,x%x,x%x) %s (err %d)\n", __func__, type, iface, udata, strerror(thread_errno), thread_errno);
        return FAIL; // revisit XASSERT(0, os_rcode);
    }

    return NET_IFACE_OK;
}


static net_iface_rcode_t net_iface_load_link_configuration(net_link_handle_t link_handle)
{
    DBG_VERBOSE("==>%s(%d)\n", __func__, link_handle);

    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
    net_interface_t *net_interface = NULL;

    uint8_t iface_enum;
    iface_enum = net_link_get_enum(link_handle); // returns 0-based 'link' offset ..
    DBG_VERBOSE("==>%s(%d) -> %d\n", __func__, link_handle, iface_enum);

    if(iface_enum >= NET_MAX_INTERFACES)
    {
        DBG_ASSERT(0);
        net_iface_rcode = NET_IFACE_FAIL;
        goto done;
    }

    /*
     *  allocate local interface context
     */

    net_interface = (net_interface_t *)MEM_MALLOC(sizeof(net_interface_t));
    if(!net_interface)
    {
        DBG_ERR("%s: error detected line %d)\n", __func__, __LINE__);
        net_iface_rcode = NET_IFACE_FAIL;
        goto done;
    }
    memset(net_interface, 0, sizeof(net_interface_t));

    /*
     *  get interface-specific OEM config
     */

    net_interface->oem_config.func_table = &net_interface->oem_func_table;
    ASSERT(g_net_ctxt.oem_config.func_table->instance_get_oem_config);
    net_iface_rcode = g_net_ctxt.oem_config.func_table->instance_get_oem_config(iface_enum, &net_interface->oem_config);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        DBG_ERR("%s(%d): (err %d)\n", __func__, link_handle, net_iface_rcode);
        goto done;
    }

//Array compared against 0 (NO_EFFECT)
#if 0
	if (!net_interface->oem_config.name)
    {
        DBG_VERBOSE("%s: ! oem_config.name\n", __func__);
        DBG_ASSERT(net_interface->oem_config.name);
    }
#endif
    DBG_VERBOSE("%s: name '%s'\n", __func__, net_interface->oem_config.name);
    char *iface_name = net_interface->oem_config.name;

    /*
     *  initialize interface context
     */

    net_interface->treck_interface = NULL;
    net_interface->recv_thread_ctxt = NULL;
    net_interface->recv_thread_stack = NULL;
    net_interface->link_handle = link_handle;
    net_interface->link_event_handle = NET_LINK_EVENT_INVALID_HANDLE;

    net_interface->timer_hndl_reset = NET_TIMER_INVALID_HANDLE;
    net_interface->timer_hndl_v6autoconf = NET_TIMER_INVALID_HANDLE;
    net_interface->msg_timer_arp.handle = NET_TIMER_INVALID_HANDLE;
    net_interface->msg_timer_arp.iface = net_interface;
    net_interface->msg_timer_v4autoip.handle = NET_TIMER_INVALID_HANDLE;
    net_interface->msg_timer_v4autoip.iface = net_interface;

    net_interface->name = iface_name;
    net_interface->name[NET_IFACE_NAME_SIZE-1] = '\0';

    #ifdef TM_USE_PPPOE_CLIENT
    net_interface->lcp_echo_id_current = 0;
    net_interface->lcp_echo_id_prev    = 0xFF;
    net_interface->lcp_echo_failure_count = 0;
    net_interface->lcp_echo_timer      = NET_TIMER_INVALID_HANDLE;
    #endif // TM_USE_PPPOE_CLIENT

    // store a reference in global network interface array
    NET_LOCK_CONTEXT();
    XASSERT(g_net_ctxt.ifaces[iface_enum] == NULL, iface_enum);
    XASSERT(g_net_ctxt.iface_cnt < NET_MAX_INTERFACES, g_net_ctxt.iface_cnt);

    // cross-reference offsets and indices
    net_interface->local_idx = iface_enum; // 1-time
    net_interface->treck_idx = if_nametoindex(net_interface->name); // dynamic

    DBG_VERBOSE("%s(%d) %s #%d (if_index %d)\n", __func__,
        link_handle, net_interface->name, net_interface->local_idx+1, net_interface->treck_idx);

    g_net_ctxt.ifaces[iface_enum] = net_interface;
    NET_UNLOCK_CONTEXT();

done:
    if(net_iface_rcode != NET_IFACE_OK)
    {
        if(net_interface)
        {
            MEM_FREE_AND_NULL(net_interface);
        }
    }
    DBG_VERBOSE("<==%s (net_iface_rcode=%d)\n",__func__, net_iface_rcode);
    return net_iface_rcode;
}

/*
 *  Attach link to a net_interface, creating the interface if it does not yet exist.
 */
static net_iface_rcode_t net_iface_attach_link(net_link_handle_t link_handle)
{
    DBG_VERBOSE("==>%s(%d)\n", __func__, link_handle);

    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
#ifndef __linux__
    bool mutex_created = false;
#endif
    net_interface_t *net_interface = NULL;

    uint8_t iface_enum;
    iface_enum = net_link_get_enum(link_handle); // returns 0-based 'link' offset ..
    if(iface_enum >= NET_MAX_INTERFACES)
    {
        ASSERT(0);
        net_iface_rcode = NET_IFACE_FAIL;
        goto done;
    }

    // Get a pointer to the previously created interface descriptor
    net_interface = g_net_ctxt.ifaces[iface_enum];
    // Sanity check to make sure the interface config has already been loaded
    ASSERT(net_interface != NULL);
    ASSERT(net_interface->oem_config.func_table != NULL);

    /*
     *  create interface mutex
     */

#ifdef __linux__
    posix_mutex_init(&net_interface->mtx);
#else
    #define NET_IFACE_MUTEX_NAME_PREFIX "mutex_"
    char* mutex_name = (char *)MEM_MALLOC(strlen(NET_IFACE_MUTEX_NAME_PREFIX)+strlen(net_interface->name)+1); // +1 NULL terminator
    if(!mutex_name)
    {
        DBG_ERR("%s: error detected line %d)\n", __func__, __LINE__);
        net_iface_rcode = NET_IFACE_FAIL;
        goto done;
    }
    sprintf(mutex_name, "%s%s", NET_IFACE_MUTEX_NAME_PREFIX, net_interface->name);
    os_rcode = os_mutex_create(&net_interface->mtx, mutex_name, TX_INHERIT);
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("%s: error detected line %d)\n", __func__, __LINE__);
        net_iface_rcode = NET_IFACE_FAIL;
        MEM_FREE_AND_NULL(mutex_name);
        goto done;
    }
    mutex_created = true;
    DBG_VERBOSE("%s: created mutex %s\n", __func__, mutex_name);
    MEM_FREE_AND_NULL(mutex_name);
#endif // ! __linux__

    /*
     *  add Treck interface
     */

    net_link_bindings_t bindings;
    memset(&bindings, 0, sizeof(net_link_bindings_t));
    net_link_get_bindings(link_handle, &bindings);
    ASSERT(bindings.open);


#ifdef __linux__
    net_interface->treck_idx = if_nametoindex(net_interface->name);
#else
    unsigned int os_rcode;
    ttUserInterface treck_interface;
    int treck_rcode;

    #ifdef TM_USE_PPPOE_CLIENT
    bool pppoe_enabled;
    net_get_pppoe_enabled(iface_enum, &pppoe_enabled);
    if (pppoe_enabled)
    {
        net_interface->treck_link_handle   = tfUseClientPppoe(pppNotify);
        net_interface->treck_link_protocol = LINK_PROTOCOL_PPPOE;
    }
    else
    #endif
    {
        net_interface->treck_link_handle   = tfUseEthernet();
        net_interface->treck_link_protocol = LINK_PROTOCOL_ETHERNET;
    }

    treck_interface = tfAddInterface(
                          net_interface->name,
                          net_interface->treck_link_handle,
                          bindings.open,
                          bindings.close,
                          bindings.send,
                          bindings.recv,
                          NULL, // ttDevFreeRecvFuncPtr
                          bindings.ioctl,
                          bindings.get_phys_addr,
                          &treck_rcode);
    if(!treck_interface)
    {
        // note: treck_rcode is valid only when treck_interface handle is NULL
        DBG_ERR("%s: tfAddInterface error (treck_rcode=%d)\n", __func__, treck_rcode);
        net_iface_rcode = NET_IFACE_FAIL;
        goto done;
    }
    net_interface->treck_interface = treck_interface;

    // TEMP Debug - uncomment to export Treck interface (e.g. for access to devLockEntry in the modified trlock.c)
    //    g_treck_iface = treck_interface;

    net_iface_rcode = net_iface_create_recv_thread(net_interface);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    /*
     *  save references to net_interface context
     */

    // store a reference in the Treck interface
    net_interface->treck_idx = net_iface_save_local_context(net_interface, treck_interface);
#endif // ! __linux__

    // store a reference in the link 
    net_link_set_user_data(link_handle, (void *)net_interface);

    // store a reference in global network interface array
    NET_LOCK_CONTEXT();
    XASSERT(g_net_ctxt.ifaces[iface_enum] != NULL, iface_enum);
    XASSERT(g_net_ctxt.iface_cnt < NET_MAX_INTERFACES, g_net_ctxt.iface_cnt);
    g_net_ctxt.iface_cnt++;
    NET_UNLOCK_CONTEXT();

    /*
     *  register for link events
     */

    net_interface->link_event_handle = net_link_register_events(net_interface->link_handle,
                                                                net_iface_link_event_notify,
                                                                (void *)net_interface);

#ifdef __linux__
#else
    /*
     *  start receive thread - do this last
     */

    tx_thread_resume(net_interface->recv_thread_ctxt);
#endif

done:
#ifndef __linux__
    if(net_iface_rcode != NET_IFACE_OK)
    {
        if(mutex_created)
        {
            os_mutex_delete(&net_interface->mtx);
        }
    }
#endif
    DBG_VERBOSE("<==%s (net_iface_rcode=%d)\n",__func__, net_iface_rcode);
    return net_iface_rcode;
}

net_interface_t *net_iface_lookup_by_name(const char *name)
{
    if (!name) return NULL;
    uint8_t i;
    net_interface_t* intf = NULL;

    NET_LOCK_CONTEXT();
    for(i = 0; i < g_net_ctxt.oem_config.iface_cnt; i++)
    {
        if(!g_net_ctxt.ifaces[i])
        {
            // skip links that failed init/enumeration
            continue;
        }
        if(strcmp(g_net_ctxt.ifaces[i]->name, name) == 0)
        {
            intf = g_net_ctxt.ifaces[i];
            DBG_INFO("%s(%s) -> %#x\n", __func__, name, intf);
            break; // done
        }
    }
    NET_UNLOCK_CONTEXT();
    return intf;
}

//=========================================================================================================================
// Private utility routines
//=========================================================================================================================

/* 
 *  net_iface_save_local_context ()
 * 
 *  Save a reference to net_iface context in the Treck interface.
 * 
 *  Used for efficient recovery of net_iface context in Treck callbacks. This is
 *  technically a hack because we are breaking rules of encapsulation by directly
 *  accessing an internal Treck structure.  However it is a necessary evil since Treck
 *  neglected to provide an opaque user data mechanism in their interface callbacks.
 * 
 *  Efficient recovery of context (e.g. vs. looping through g_net_ctxt->ifaces array) is
 *  important to avoid loss of network throughput in the send and recv callbacks.
 * 
 *  This routine relies on a MARVELL MODIFICATION adding the user_data field to Treck
 *  ttDeviceEntry struct at ~line 2304 of trtype.h.
 * 
 *  returns:
 *      Treck interface index, as returned by if_nametoindex().
 */
STATIC uint8_t net_iface_save_local_context(net_interface_t *net_interface, ttUserInterface treck_interface)
{
    ASSERT(net_interface->link_handle != (net_link_handle_t)NET_LINK_INVALID_HANDLE);
#ifdef __linux__
    return if_nametoindex(net_interface->name); // current os' if_index
#else
    ttDeviceEntryPtr devEntryPtr;
    devEntryPtr = (ttDeviceEntryPtr)treck_interface;
    devEntryPtr->user_data = (void *)net_interface->link_handle;
    DBG_VERBOSE("saving net link_handle %08x in treck iface %08x (treck_idx=%d)\n",
                net_interface->link_handle, treck_interface, devEntryPtr->devIndex);
    unsigned int tmp_idx = devEntryPtr->devIndex;
    XASSERT( !(tmp_idx & 0xFFFFFF00), tmp_idx);
    return (uint8_t)(tmp_idx & 0xFF);
#endif
}

/* 
 *  net_iface_recover_local_context()
 * 
 *  Recover reference to net_iface context previously stored in the Treck interface.
 * 
 *  This routine relies on a MARVELL MODIFICATION adding the user_data field to Treck
 *  ttDeviceEntry struct at ~line 2304 of trtype.h.
 */
/*static*/ net_interface_t *net_iface_recover_local_context(ttUserInterface treck_interface)
{
    ttDeviceEntryPtr devEntryPtr;
    devEntryPtr = (ttDeviceEntryPtr)treck_interface;
    uint8_t iface_enum = net_link_get_enum((net_link_handle_t)devEntryPtr->user_data);
    XASSERT(iface_enum < g_net_ctxt.oem_config.iface_cnt, iface_enum);
    XASSERT(g_net_ctxt.ifaces[iface_enum], iface_enum);
    return g_net_ctxt.ifaces[iface_enum];
}

static void delayed_msg_timer_func(void *user_data)
{
    ASSERT(user_data);
    net_iface_msg_timer_t *msg_timer = (net_iface_msg_timer_t *)user_data;

    // assume ThreadX timer callback (i.e. a non-thread)
    ASSERT(msg_timer->iface);
    int32_t net_iface_rcode = net_send_msg_no_wait(msg_timer->msg, msg_timer->iface, (void *)0, (void *)0);
    if(net_iface_rcode == NET_IFACE_WOULDBLOCK)
    {
        // msg queue full
        DBG_MSG("msg queue full while sending delayed msg '%s' on %s\n", 
                str_iface_msg(msg_timer->msg), msg_timer->iface->name);
        DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
    }

    return;
}

// intf ip addresses; returns NULL if currently unbound
typedef struct sockaddr_in6 ipv6_addr_t;
char* get_intf_addr(char* intf_of_interest, int protocol_family, char* given_buf) // return str version
{
    static char last_inquired_address_heap_str[INET6_ADDRSTRLEN]; // large enough for MAX(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)
    net_link_status_t intf_rcode = NET_LINK_STATUS_UP;
    int sock;
    struct ifconf current_intf;
    struct ifreq intf_set[NET_MAX_INTERFACES];
    int number_of_intf;
    int i = 0;

    ASSERT(protocol_family == AF_INET || protocol_family == AF_INET6);

    // Create a socket or return an error.
    //sock = socket(protocol_family, SOCK_STREAM, 0);
    if (protocol_family == AF_INET6)
    {
        sock = socket(PF_INET6, SOCK_STREAM, 0);
    }
    else
    {
        sock = socket(PF_INET, SOCK_STREAM, 0);
    }
    if (sock < 0)
    {
        DBG_ERR("%s: bad socket",__func__);
        return NULL;
    }

    // Point ifconf's ifc_buf to our array of interface ifreqs.
    current_intf.ifc_buf = (char *) intf_set;

    // Set ifconf's ifc_len to the length of our array of interface ifreqs.
    current_intf.ifc_len = sizeof intf_set;

    //  Populate ifconf.ifc_buf (ifreq) with a list of interface names and addresses.
    if (ioctl(sock, SIOCGIFCONF, &current_intf) == -1)
    {
        DBG_ERR("%s: bad SIOCGIFCONF call..", __func__);
        close(sock);
        return NULL;
    }

    // Divide the length of the interface list by the size of each entry.
    // This gives us the number of interfaces on the system.
    number_of_intf = current_intf.ifc_len / sizeof(intf_set[0]);

    intf_rcode = NET_LINK_STATUS_DOWN; // unless otherwise demonstrated

    while (i < number_of_intf)
    {
        // if this is the interface of interest,
        //    Convert the binary IP address into a readable string.
        char* this_if_name = intf_set[i].ifr_name;
        if (this_if_name != NULL && !strcmp(this_if_name, intf_of_interest)) // matching intf?
        {
            char ip[INET6_ADDRSTRLEN]; // MAX(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)]
            const char* str_rep = NULL;

            if (protocol_family == AF_INET) // IPv4
            {
                struct sockaddr_in* ipv4_addr = (struct sockaddr_in *) &intf_set[i].ifr_addr;
                str_rep = inet_ntop(AF_INET, &(ipv4_addr->sin_addr), ip, INET_ADDRSTRLEN); // 16 char
            }
            else // AF_INET6 family requires a 2ndary lookup
                 // ref http://www.linuxquestions.org/questions/linux-networking-3/how-to-get-ipv6-address-using-ioctl-siocgifaddr-808792/
            {
                struct ifaddrs* if_addr, *if_addr_table;
                if (0 < getifaddrs(&if_addr_table))
                {
                    DBG_ERR("%s() getifaddrs() (err %d)\n", __func__, strerror(thread_errno), thread_errno);
                }
                if_addr = if_addr_table;

                // travel through intf addresses looking for a match
                if (if_addr)
                {
                  while (if_addr->ifa_next)
                  {
                    if ((if_addr->ifa_addr->sa_family == AF_INET6))
                    {
                        if (!strcmp(if_addr->ifa_name, this_if_name)) // same interface name
                        {
                            int err = getnameinfo(if_addr->ifa_addr, sizeof(ipv6_addr_t), ip, sizeof(ip), NULL, 0, NI_NUMERICHOST);
                            if (!err)
                            {
                                str_rep = strtok(ip, "%"); // trim interface name %<intf_name>
                                DBG_VERBOSE("ifaddr x%x %s\n", if_addr, str_rep);
                                break; // done
                            }
                            else
                            {
                                DBG_ERR("getnameinfo %s (err %d %d)\n", strerror(thread_errno), thread_errno, err);
                            }
                        }
                    }
                    if_addr = if_addr->ifa_next; // next if_address
                  }
                  freeifaddrs(if_addr_table);
                }
            }

            if (str_rep)
            {
                snprintf(last_inquired_address_heap_str,sizeof(last_inquired_address_heap_str), "%s", str_rep);
                if (given_buf) strcpy(given_buf, str_rep);
                intf_rcode = NET_LINK_STATUS_UP;
            }
            break; // done
        }
        i++; // next intf record
    }
    close(sock);

    if (intf_rcode != NET_LINK_STATUS_UP)
    {
        sprintf(last_inquired_address_heap_str, "DISCONNECTED");
        return NULL; // currently unavailable
    }

    return last_inquired_address_heap_str; // short-lived valid time
}

char* get_intf_ipv4_addr(char* intf_of_interest) // str version of x.y.z.w, NULL if unbound
{
    static char ipv4_addr[ INET_ADDRSTRLEN ];
    return get_intf_addr(intf_of_interest, AF_INET, ipv4_addr); // str version of x.y.z.w
}


char* get_intf_ipv6_addr(char* intf_of_interest) // str version of x:y:z:w:a:b:c, NULL if unbound
{
    static char ipv6_addr[ INET6_ADDRSTRLEN ];
    return get_intf_addr(intf_of_interest, AF_INET6, ipv6_addr);
}

// int -> str representation
static char str_msg_type_on_heap[40];
char* str_iface_msg(int type)
{
#define str_msg_type(type) case type: snprintf(str_msg_type_on_heap,sizeof(str_msg_type_on_heap),#type); break
    switch(type)
    {
        str_msg_type(NET_IFACE_MSG_LINK_UP);
        str_msg_type(NET_IFACE_MSG_LINK_UP_POLL);
        str_msg_type(NET_IFACE_MSG_LINK_DOWN);
        str_msg_type(NET_IFACE_MSG_LINK_DOWN_PENDING);
        str_msg_type(NET_IFACE_MSG_NET_RESET);
        str_msg_type(NET_IFACE_MSG_IPV4_CONFIG_COMPLETE);
        str_msg_type(NET_IFACE_MSG_IPV4_CONFIG_FAILED);
        str_msg_type(NET_IFACE_MSG_IPV4_CONFIG_RETRY);
        str_msg_type(NET_IFACE_MSG_IPV4_ARP_ANNOUNCE);
        str_msg_type(NET_IFACE_MSG_IPV4_CLOSE_FINISH);
#ifdef HAVE_IPV6
        str_msg_type(NET_IFACE_MSG_IPV6_LOCAL_CFG_COMPLETE);
        str_msg_type(NET_IFACE_MSG_IPV6_GLOBAL_CFG_COMPLETE);
        str_msg_type(NET_IFACE_MSG_IPV6_CONFIG_FAILED);
        str_msg_type(NET_IFACE_MSG_IPV6_CONFIG_RETRY);
        str_msg_type(NET_IFACE_MSG_IPV6_CLOSE_FINISH);
#endif // HAVE_IPV6
        str_msg_type(NET_IFACE_MSG_UNRECOVERABLE_ERROR);
        str_msg_type(NET_IFACE_MSG_ENABLE_LINKS);
        str_msg_type(NET_IFACE_MSG_EXTERNAL_CLOSE_DETECTED);
        str_msg_type(NET_IFACE_MSG_ALL_CLOSE_FINISH);
        str_msg_type(NET_IFACE_MSG_CHECK_PPPoE_CONNECTION);
     default:
        snprintf(str_msg_type_on_heap, sizeof(str_msg_type_on_heap),"<unknown msg %d>", type); break;
    }
    return str_msg_type_on_heap;
}

char* str_iface_state(int state)
{
    #define str_intf_state(state) case state: snprintf(state_str,sizeof(state_str),#state); break
    static char state_str[40] = { 0 };
    switch(state)
    {
        str_intf_state(NET_IFACE_STATE_ACQUIRE_LINK);
        str_intf_state(NET_IFACE_STATE_OPENING_INTERFACE);
        str_intf_state(NET_IFACE_STATE_INTERFACE_OPEN);
        str_intf_state(NET_IFACE_STATE_CLOSING_INTERFACE);
        str_intf_state(NET_IFACE_STATE_INTERNAL_ERROR);
      default:
        snprintf(state_str,sizeof(state_str),"<unknown if state %d>", state); break;
    }
    return state_str;
}

//=========================================================================================================================
// Module entry point
//=========================================================================================================================

static void* net_iface_state_loop(void* ignored)
{
    net_interface_t* iface = NULL;
    DBG_VERBOSE("%s()..\n", __func__);
    net_iface_msg_t* msg;

    net_iface_config_interfaces();
    net_iface_enumerate_interfaces();

    // listen for interface msgs
    while(1)
    {
        msg = (net_iface_msg_t*)&g_net_iface_msg_buf; // revisit msg = &g_net_link_msg_buf.msg;
        unsigned os_rcode = posix_wait_for_message(g_net_iface_msg_queue, (char*)&g_net_iface_msg_buf, sizeof(net_iface_msg_t), OS_WAIT_FOREVER);

        if(os_rcode != OS_SUCCESS)
        {
            DBG_MSG("%s() msg x%x intf x%x (err %d)\n", __func__, msg, msg->iface, os_rcode);
            DBG_ERR("error reading g_net_iface_msg_queue (os_rcode=0x%02x)\n", os_rcode);
            break; // goto error;
        }
        ASSERT(msg->type < NET_IFACE_NUM_MSGS);
        
        /*
         *  process stateless messages
         */

        switch(msg->type) // debug
        {
        default:
          if (msg->iface)
          {
              DBG_VERBOSE("if: '%s' msg '%s' (%d) data x%x..\n", msg->iface->name, str_iface_msg(msg->type), msg->type, msg->udata);
          }
          break;
        }
        DBG_ASSERT(msg->iface);
        DBG_ASSERT(msg->iface->state < NET_IFACE_NUM_STATES);

        /*
         *  process stateful messages
         */

        iface = msg->iface;
#ifdef DBG_VERBOSE_ON
        if (iface)
        {
            int type = msg->type; UNUSED_VAR(type);
            int state = iface->state; UNUSED_VAR(state);

            DBG_VERBOSE("%s (%#x) msg '%s' (%d) in state %s (%d)\n",
                iface->name, iface,
                str_iface_msg(type), type,
                str_iface_state(state), state);
        }
#endif // DBG_VERBOSE_ON

        switch(iface->state)
        {

        /***************************************************
         * NET_IFACE_STATE_ACQUIRE_LINK
         ***************************************************/
        case NET_IFACE_STATE_ACQUIRE_LINK:
            {
                net_link_status_t link_status;
                switch(msg->type)
                {
                case NET_IFACE_MSG_LINK_UP: // fallthru

                case NET_IFACE_MSG_LINK_UP_POLL:

                    ASSERT(msg->iface->link_handle != (net_link_handle_t)NET_LINK_INVALID_HANDLE);
                    link_status = net_link_get_status(msg->iface->link_handle);

#ifdef __linux__
                    // double-check intf state -- nail down link status if discrepancy exists
                    if (!intf_active(msg->iface->name) && link_status == NET_LINK_STATUS_UP)
                    {
                        DBG_ERR("%s() %s link state discrepancy .. resetting\n", __func__, msg->iface->name);
                        net_link_t* link = net_link_lookup_by_name(msg->iface->name);
                        net_link_send_msg_no_wait(NET_LINK_MSG_DOWN_EVENT, link, (void *)0, (void *)0);
                        link_status = NET_LINK_STATUS_DOWN;
                    }
#endif

                    NET_LOCK_IFACE_CONTEXT(msg->iface);
                    if(!intf_active(msg->iface->name))
                    {
                        // link is not up
                        NET_UNLOCK_IFACE_CONTEXT(msg->iface);
                        //char* if_name = (msg->iface ? msg->iface->name : "<unbound intf>"); UNUSED_VAR(if_name);
                        //int state = msg->iface->state; UNUSED_VAR(state);
                        DBG_VERBOSE("%s() %s link not ready .. (msg %d link-status %d if-state %s (%d)) ignored\n", __func__, msg->iface ? msg->iface->name : "<unbound intf>", msg->type, link_status, str_iface_state(msg->iface->state), msg->iface->state);
                        break;
                    }

                    /* 
                     *  link is up - open the interface.
                     */

                    // Cancel any pending net reset. Doing so ensures that we will ignore any delayed net reset
                    // triggered from an earlier NET_IFACE_STATE_OPENING_INTERFACE/NET_IFACE_STATE_INTERFACE_OPEN state.
                    net_timer_cancel(msg->iface->timer_hndl_reset);
                    msg->iface->timer_hndl_reset = NET_TIMER_INVALID_HANDLE;

                    // any config changes made to the interface after this point will trigger a net reset
                    adv_state(msg->iface, NET_IFACE_STATE_OPENING_INTERFACE);

                    NET_UNLOCK_IFACE_CONTEXT(msg->iface);

                    // notify of logical link up OR net reset triggered by possible physical link up/down
                    event_list_notify(msg->iface, NET_IFACE_EVENT_LINK_STATUS_CHANGE);

                    open_interface(msg->iface);
                    break;

                default:
                    DBG_IGNORED("ignored %s msg '%s' (%d) in state STATE_ACQUIRE_LINK(%d)\n", msg->iface->name, str_iface_msg(msg->type), msg->type, __LINE__);
                    // ignore other messages
                case NET_IFACE_MSG_LINK_DOWN: // fallthru
                    break;
                }
            }
            break;

        /***************************************************
         * NET_IFACE_STATE_OPENING_INTERFACE
         ***************************************************/
        case NET_IFACE_STATE_OPENING_INTERFACE:
        {

            switch(msg->type)
            {
            case NET_IFACE_MSG_NET_RESET:
                DBG_ASSERT(msg->iface);
                NET_LOCK_IFACE_CONTEXT(msg->iface);
                //char* if_name = (msg->iface ? msg->iface->name : "<unbound intf>"); UNUSED_VAR(if_name);
                DBG_MSG("resetting network interface %s (state: %d)\n", (msg->iface && msg->iface->name) ? msg->iface->name : "<unbound intf>", msg->iface->state);
                adv_state(msg->iface, NET_IFACE_STATE_ACQUIRE_LINK);
                net_send_msg(NET_IFACE_MSG_LINK_UP_POLL, msg->iface, (void *)0, (void *)0);
                NET_UNLOCK_IFACE_CONTEXT(msg->iface);
                break;

#ifdef __linux__ // notice & fallthrough
            case NET_IFACE_MSG_LINK_UP:
                DBG_VERBOSE("unexpected %s msg '%s' in state STATE_OPENING_INTERFACE(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
#endif // fallthrough

            case NET_IFACE_MSG_LINK_DOWN:
                DBG_ASSERT(msg->iface);
                NET_LOCK_IFACE_CONTEXT(msg->iface);
                adv_state(msg->iface, NET_IFACE_STATE_ACQUIRE_LINK);
                net_send_msg(NET_IFACE_MSG_LINK_UP_POLL, msg->iface, (void *)0, (void *)0);
                NET_UNLOCK_IFACE_CONTEXT(msg->iface);
                event_list_notify(msg->iface, NET_IFACE_EVENT_LINK_STATUS_CHANGE);
                break;


            default:
                // ignore other messages
                DBG_IGNORED("ignored %s msg '%s' in state STATE_OPENING_INTERFACE(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                break;
            }
        } break;

        /***************************************************
         * NET_IFACE_STATE_INTERFACE_OPEN
         ***************************************************/
        case NET_IFACE_STATE_INTERFACE_OPEN:
        {
            /*
             *  process IPv4 config state messages
             */

            switch(msg->iface->ipv4_cfg_state)
            {
            case IPV4_CONFIGURING:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV4_CONFIG_COMPLETE:
                    ipv4_open_interface_complete(msg->iface);
                    break;

                case NET_IFACE_MSG_IPV4_CONFIG_FAILED:
                    // IPv4 config failed (e.g. after trying all selected boot protocols)
                    // NOTE: we unconfigure IPv4 but leave the Treck interface open, even if IPv6 not configured
                    DBG_MSG("IPv4 configuration failed on %s\n", msg->iface->name);
                    ipv4_close_interface(msg->iface);
                    break;

                case NET_IFACE_MSG_IPV4_CONFIG_RETRY:
                    ipv4_reconfigure_interface(msg->iface);
                    break;

                default: // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' state v4 CONFIGURING (v6 %d)\n",
                        msg->iface->name, str_iface_msg(msg->type), msg->iface->ipv6_cfg_state);
                    break;
                }
                break;

            case IPV4_CONFIGURED:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV4_ARP_ANNOUNCE:
                    /*
                     *  send gratuitous ARP announcement
                     * 
                     *  NOTE: This will generate a spurrious response to our registered collision detection handler to
                     *  report that no collisions were detected. To prevent tfFinishOpenInterface from being called
                     *  again, we must be in state IPV4_CONFIGURED before making this call.
                     */
#ifdef __linux__
//unused
#else
                    {
                        uint32_t ipv4_addr;
                        net_iface_get_var_ipv4_addr(msg->iface->treck_idx, &ipv4_addr);

                        unsigned int os_rcode = tfUserStartArpSend(msg->iface->treck_interface,
                                                         ipv4_addr,
                                                         /* num announcements */ 1,
                                                         NET_IFACE_IPV4_ARP_ANNOUNCE_INTERVAL_MS,
                                                         NET_IFACE_IPV4_ARP_ANNOUNCE_INTERVAL_MS);
                        if(os_rcode != TM_ENOERROR)
                        {
                            DBG_ERR("tfUserStartArpSend failed on %s (error=%d)\n", msg->iface->name, os_rcode);
                        }
                    }
#endif
                    break;

                case NET_IFACE_MSG_IPV4_CONFIG_FAILED:
                    // IPv4 config failed (e.g. fatal error while finalizing configuration)
                    // NOTE: we unconfigure IPv4 but leave the Treck interface open, even if IPv6 not configured
                    DBG_MSG("IPv4 configuration failed on %s\n", msg->iface->name);
                    ipv4_close_interface(msg->iface);
                    break;

                case NET_IFACE_MSG_IPV4_CONFIG_RETRY:
                    ipv4_reconfigure_interface(msg->iface);
                    break;

                default: // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' in state IPV4_CONFIGURED\n", msg->iface->name, str_iface_msg(msg->type));
                    break;
                }
                break;

            case IPV4_CLOSING:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV4_CLOSE_FINISH:
                    ipv4_finish_close_interface(msg->iface);
                    break;

                default:
                    // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' in state IPV4_CLOSING(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                    break;
                }
                break;

            default:
                // skip other IPv4 config states
                DBG_IGNORED("ignored %s msg '%s' in state INTERFACE_OPEN(%d) (v4 state %d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__, msg->iface->ipv4_cfg_state);
                break;
            }

#ifdef HAVE_IPV6
            /*
             *  process IPv6 config state messages
             */

            switch(msg->iface->ipv6_cfg_state)
            {
            case IPV6_CONFIGURING:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV6_LOCAL_CFG_COMPLETE:
                    ipv6_local_config_complete(msg->iface);
                    break;

                default:
                    // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' in state IPV6_CONFIGURING(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                    break;
                }
                break;

            case IPV6_LOCAL_CONFIGURED:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV6_GLOBAL_CFG_COMPLETE:
                    ipv6_global_config_complete(msg->iface);
                    break;

                case NET_IFACE_MSG_IPV6_CONFIG_RETRY:
                    ipv6_reconfigure_dhcp(msg->iface);
                    break;

                default:
                    // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' in state IPV6_LOCAL_CONFIGURED(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                    break;
                }
                break;

            case IPV6_CONFIGURED:
                switch(msg->type)
                {

#ifdef __linux__
                case NET_IFACE_MSG_LINK_DOWN: ipv6_close_interface(msg->iface); break;
#endif

                case NET_IFACE_MSG_IPV6_CONFIG_RETRY:
                    ipv6_reconfigure_dhcp(msg->iface);
                    break;

                default:
                    DBG_IGNORED("ignored %s msg '%s' in state IPV6_CONFIGURED(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                    // ignore other messages
                    break;
                }
                break;

            case IPV6_CLOSING:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV6_CLOSE_FINISH:
                    DBG_MSG("DHCPv6 client stopped on %s\n", msg->iface->name);
                    ipv6_finish_close_interface(msg->iface);
                    break;

                default:
                    // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' in state IPV6_CLOSING(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                    break;
                }
                break;

            default:
                // skip other IPv6 config states
                break;
            }

            if(msg->iface->ipv6_cfg_state >= IPV6_CONFIGURING)
            {
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV6_CONFIG_FAILED:
                    // disable the IPv6 portion of interface
                    DBG_ERR("IPv6 configuration failed on %s\n", msg->iface->name);
                    ipv6_close_interface(msg->iface);
                    break;

                default:
                    // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' (v6 state %d) in STATE_INTERFACE_OPEN(%d)\n", msg->iface->name, str_iface_msg(msg->type), msg->iface->ipv6_cfg_state, __LINE__);
                    break;
                }
            }
#endif // HAVE_IPV6

            /*
             *  process generic messages
             */

            switch(msg->type)
            {
            case NET_IFACE_MSG_NET_RESET:
            {

                char* if_name = (msg->iface ? msg->iface->name : "<unbound intf>"); UNUSED_VAR(if_name);
                DBG_MSG("resetting network interface %s (state: %d)\n", if_name, msg->iface->state);
                DBG_ASSERT(msg->iface);
                close_interface(msg->iface);
            } break;

            case NET_IFACE_MSG_LINK_DOWN:

                DBG_ASSERT(msg->iface);
                event_list_notify(msg->iface, NET_IFACE_EVENT_LINK_STATUS_CHANGE);
                close_interface(msg->iface);
                break;

            default:
                // ignore other messages
                DBG_IGNORED("ignored %s msg '%s' in state STATE_INTERFACE_OPEN(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);

                break;
            }
        } break;

        /***************************************************
         * NET_IFACE_STATE_CLOSING_INTERFACE
         ***************************************************/
        case NET_IFACE_STATE_CLOSING_INTERFACE:
        {
            /*
             *  process IPv4 config state messages
             */

            switch(msg->iface->ipv4_cfg_state)
            {
            case IPV4_CLOSING:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV4_CLOSE_FINISH:
                    ipv4_finish_close_interface(msg->iface);
                    finish_close_interface(msg->iface);
                    break;
                default: // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' in v4 state %d (%d)\n", msg->iface->name, str_iface_msg(msg->type), msg->iface->ipv4_cfg_state, __LINE__);
                    break;
                }
                break;

            default: // skip other IPv4 config states
                DBG_IGNORED("ignored %s msg '%s' in v4 state %d (%d)\n", msg->iface->name, str_iface_msg(msg->type), msg->iface->ipv4_cfg_state, __LINE__);
                break;
            }

#ifdef HAVE_IPV6
            /*
             *  process IPv6 config state messages
             */
            switch(msg->iface->ipv6_cfg_state)
            {
            case IPV6_CLOSING:
                switch(msg->type)
                {
                case NET_IFACE_MSG_IPV6_CLOSE_FINISH:
                    DBG_MSG("DHCPv6 client stopped on %s\n", msg->iface->name);
                    ipv6_finish_close_interface(msg->iface);
                    finish_close_interface(msg->iface);
                    break;	
                default: // ignore other messages
                    DBG_IGNORED("ignored %s msg '%s' in v6 state %d (%d)\n", msg->iface->name, str_iface_msg(msg->type), msg->iface->ipv6_cfg_state, __LINE__);
                    break;
                }
                break;

            default: // skip other IPv6 config states
                DBG_IGNORED("ignored %s msg '%s' in v6 state %d (%d)\n", msg->iface->name, str_iface_msg(msg->type), msg->iface->ipv6_cfg_state, __LINE__);
                break;
            }
#endif // HAVE_IPV6

            /*
             *  process generic messages
             */

            switch(msg->type)
            {
            default: // ignore other messages
                DBG_IGNORED("ignored %s msg '%s' in state STATE_CLOSING_INTERFACE(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                break;
            }
        } break;

        /***************************************************
         * NET_IFACE_STATE_INTERNAL_ERROR
         ***************************************************/
        case NET_IFACE_STATE_INTERNAL_ERROR:
        {

            switch(msg->type)
            {
            case NET_IFACE_MSG_UNRECOVERABLE_ERROR:
                // rdj TODO - restart network
                DBG_ERR("An unrecoverable internal error has occurred on %s! Shutting down interface...\n", msg->iface->name);
#ifdef HAVE_NET_SERVICES
                net_service_request(NET_SERVICE_START, NET_SERVICE_IFCONFIG, NULL, NULL);
#else
                DBG_ASSERT(0);
#endif
                break;

            default:
                // ignore other messages
                DBG_IGNORED("ignored %s msg '%s' in state STATE_INTERNAL_ERROR(%d)\n", msg->iface->name, str_iface_msg(msg->type), __LINE__);
                break;
            }
        } break;

        default:
        {
            // unknown state - ignore
#ifdef DBG_VERBOSE_ON
            char* if_name = (msg->iface ? msg->iface->name : "<unknown intf>");
            if (!if_name) if_name = "<unbound intf>";
            int if_state = (msg->iface ? msg->iface->state : -1); UNUSED_VAR(if_state);
            
            DBG_IGNORED("%s x%x '%s', ignored msg '%s' in state %s (%d)\n", __func__, msg->iface, if_name, str_iface_msg(msg->type), str_iface_state(if_state), if_state);
#endif
            //revisit DBG_ASSERT(msg->iface->state);
        } break;

        }

    } // forever, no returns

    DBG_VERBOSE("%s() exitting..\n", __func__);
    return (void*)-1; 
}

STATIC void net_iface_start(unsigned long ignored)
{
    DBG_VERBOSE("==>%s\n",__func__);
    unsigned int os_rcode;

    /*
     *  initialize netdrvr context
     */

#ifdef __linux__
    os_rcode = posix_mutex_init(&g_net_ctxt.mtx);
#else
    os_rcode = tx_mutex_create(&g_net_ctxt.mtx, "netdrvrCtxt", TX_INHERIT);
#endif
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating g_net_ctxt mutex (os_rcode=0x%02x)\n", os_rcode);
        ASSERT(0);
    }

    /*
     * create net intf state machine message queue
     */

    posix_create_message_queue(&g_net_iface_msg_queue, NET_IFACE_MQ_NAME,
                                                            NET_IFACE_MQ_NUM_MSG_BUFS,
                                                            OS_4_ULONG /*NET_IFACE_MQ_MSG_BUF_SIZE*/);


    os_rcode = posix_create_thread(&net_iface_thread, net_iface_state_loop, NULL, NET_IFACE_THREAD_NAME,
                                        &net_iface_stack, NET_IFACE_THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if (os_rcode != OK)
    {
        DBG_ERR("%s() no thread!\n", __func__);
    }
#ifdef __linux__
    else
    // ensure that an /etc/network/interfaces exists
    {
        DBG_VERBOSE("<==%s\n",__func__);

        if (!file_exists(etc_network_interfaces_conf))
        {
            rebuild_etc_network_interfaces();
            os_system("sync");
        }
    }
#endif
}

// interface configuration
STATIC void open_interface(net_interface_t* iface)
{
    show_current_if_states();
    if (!iface)
    {
        DBG_ERR("%s(<undefined intf>)\n", __func__);
        DBG_ASSERT(iface);
        return;
    }

    NET_LOCK_IFACE_CONTEXT(iface);

    if (!net_intf_enabled(iface)) // ignore disabled interface
    {
        NET_UNLOCK_IFACE_CONTEXT(iface);
        return;
    }

    // refresh if_index
    unsigned int if_index = if_nametoindex(iface->name);
    if (if_index > 0)
    {
        DBG_INFO("%s() intf %s idx refresh (%d -> %d)\n", __func__, iface->name, if_index, iface->treck_idx);
        iface->treck_idx = if_index; // update with latest os' if_index
    }

    bool ipv4_enabled;
    net_iface_get_var_ipv4_enabled(iface->treck_idx, &ipv4_enabled);
#ifdef HAVE_IPV6
    bool ipv6_enabled;
    net_iface_get_var_ipv6_enabled(iface->treck_idx, &ipv6_enabled);
#endif // HAVE_IPV6
    NET_UNLOCK_IFACE_CONTEXT(iface);

    // must be bound to an underlying network link interface
    DBG_ASSERT(iface->link_handle != (net_link_handle_t)NET_LINK_INVALID_HANDLE);

    DBG_VERBOSE("opening network v4 interface %s%s\n", iface->name, (ipv4_enabled?"":" (NOT enabled)"));

#ifdef HAVE_IPV6
    // Reset the IPv6 interface ID based on the MAC address of the currently bound link.  Treck sets the
    // interface ID automatically the first time tfNgOpenInterface() is called, but it remains fixed 
    // thereafter unless explicity set by the user via tf6Eui48SetInterfaceId() -- bug 15079
    uint8_t active_phys_addr[TM_MAX_PHYS_ADDR] = {0};
    DBG_ASSERT(iface->link_handle != (net_link_handle_t)NET_LINK_INVALID_HANDLE);
    uint32_t link_index;
    net_link_get_active_link(iface->link_handle, &link_index);
    // bug 18511: handle the case where link goes down when open interface is in progress
    if(!link_index)
    {
        // abort and allow the link down message to be processed in the state loop
        DBG_MSG("link down - aborting open network interface %s\n", iface->name);
        return;
    }

    net_iface_rcode_t net_iface_rcode = net_link_get_phys_addr(link_index, active_phys_addr, TM_MAX_PHYS_ADDR);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        DBG_ERR("net_link_get_phys_addr error (link_index=%d net_iface_rcode=%d)\n", link_index, net_iface_rcode);
        DBG_ASSERT(0==net_iface_rcode);
        adv_state(iface, NET_IFACE_STATE_INTERNAL_ERROR);
        net_send_msg(NET_IFACE_MSG_UNRECOVERABLE_ERROR, iface, (void *)0, (void *)0);
        return;
    }

#ifndef __linux__ // using treck
    int treck_rcode = tf6Eui48SetInterfaceId(iface->treck_interface,
                                         (ttUser8Bit *)active_phys_addr,
                                         TM_6_DEV_CALLED_FROM_APP);
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tf6Eui48SetInterfaceId error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
    }
#endif // using treck
#endif // HAVE_IPV6

    if ( ipv4_enabled )
    {
        ipv4_open_interface(iface);
    }

#ifdef HAVE_IPV6
    if(ipv6_enabled)
    {
        ipv6_open_interface(iface);
    }
#endif // HAVE_IPV6

    // rdj TODO - handle case where both IPv4 and IPv6 are disabled
    if (ipv4_enabled 
#ifdef HAVE_IPV6
	|| ipv6_enabled
#endif
	)
    {
        adv_state(iface, NET_IFACE_STATE_INTERFACE_OPEN);
    }
    return;
}

STATIC void close_interface(net_interface_t* iface)
{
    int32_t net_iface_rcode;
    bool close_interface = true;

    show_current_if_states();

    adv_state(iface, NET_IFACE_STATE_CLOSING_INTERFACE);

    // close IPv4 if currently configured or in process of being configured
    if(iface->ipv4_cfg_state > IPV4_CLOSING)
    {
        net_iface_rcode = ipv4_close_interface(iface);
        if(net_iface_rcode == NET_IFACE_INPROGRESS)
        {
            // IPv4 close cannot be completed at this time
            close_interface = false;
        }
    }

    // close IPv6 if currently configured or in process of being configured
#ifdef HAVE_IPV6
    if(iface->ipv6_cfg_state > IPV6_CLOSING)
    {
        net_iface_rcode = ipv6_close_interface(iface);
        if(net_iface_rcode == NET_IFACE_INPROGRESS)
        {
            // IPv6 close cannot be completed at this time
            close_interface = false;
        }
    }
#endif // HAVE_IPV6

    if(close_interface)
    {
        finish_close_interface(iface);
    }
    return;
}

STATIC void finish_close_interface(net_interface_t* iface)
{
    show_current_if_states();
    if( (iface->ipv4_cfg_state == IPV4_NOT_CONFIGURED)
#ifdef HAVE_IPV6
       && (iface->ipv6_cfg_state == IPV6_NOT_CONFIGURED)
#endif // HAVE_IPV6
      )
    {
#ifdef __linux__
        int close_result = TM_ENOENT;
        {
            uint8_t mac_addr[MAC_ADDR_BYTE_SIZE];
            if (NULL != get_intf_mac_addr(iface->name, mac_addr)) // revisit intf_up()
            {
                DBG_VERBOSE("%s(x%x) '%s'\n", __func__, iface, iface->name);
                char os_request[20] = { "ifdown " };
                strcat(os_request, iface->name);
                os_system(os_request);
                close_result = TM_ENOERROR;
            }
        }
#else
        int close_result = tfCloseInterface(iface->treck_interface);
#endif
        if (close_result == TM_ENOERROR)
        {
            DBG_VERBOSE("closed network interface %s\n", iface->name);

#ifdef HAVE_HOTSPOT_SUPPORT
            if (wifi_hotspot_allowed_by_platform())
            {
                if (iface->nat_enabled)
                {
                    int treck_rcode;

                    treck_rcode = tfSetTreckOptions(TM_OPTION_IP_FORWARDING, 0UL);
                    XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                    treck_rcode = tfSetTreckOptions(TM_OPTION_IP_MCAST_FORWARD, 0UL);
                    XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);

                    tfNatSetTemporaryDisable(1);
                }

                // TODO: Revisit
                // If they changed the PPPoE 'enabled' setting we need to
                // apply those changes while the interface is closed
//                netdrvr_apply_pppoe_setting_if_changed();
            }
#endif // HAVE_HOTSPOT_SUPPORT

            // go to acquire state
            NET_LOCK_IFACE_CONTEXT(iface);
            adv_state(iface, NET_IFACE_STATE_ACQUIRE_LINK);
            net_send_msg(NET_IFACE_MSG_LINK_UP_POLL, iface, (void *)0, (void *)0);
            NET_UNLOCK_IFACE_CONTEXT(iface);
        }
    }

    return;
}

STATIC void ipv4_open_interface(net_interface_t *iface)
{
    show_current_if_states();
    DBG_VERBOSE("==>%s(%s)\n", __func__, iface->name);

    NET_LOCK_IFACE_CONTEXT(iface);

#ifdef __linux__
    if (!intf_up(iface->name))
    {
        char os_req[32] = { SBIN "/ifup -f " };
        strcat(os_req, iface->name);
        DBG_VERBOSE("%s(%s) %s\n", __func__, iface->name, os_req);
        os_system(os_req);
    }
#endif // ! __linux__

    int default_open_flags = 0;

#ifdef TM_USE_PPPOE_CLIENT
    // TODO - temporarily reboot when changing PPPoE settings.  In time we will switch this
    // to remove and re-add the interface with the correct link layer.  For now, though,
    // a reboot is required if the link layer doesn't match the desired setting.
    bool pppoe_enabled;
    net_iface_get_var_pppoe_enabled(iface->treck_idx, &pppoe_enabled);
    if (( pppoe_enabled && (iface->treck_link_protocol != LINK_PROTOCOL_PPPOE)) ||
        (!pppoe_enabled && (iface->treck_link_protocol == LINK_PROTOCOL_PPPOE)))
    {
        DBG_CRIT("PPPoE FORCING REBOOT\n");
        void hwPlatformReset();
        hwPlatformReset();
    }
#endif // TM_USE_PPPOE_CLIENT


    // cancel any ongoing ipv4 collision detection (i.e. from a previous config attempt)
    ipv4_cancel_collision_detection(iface);

    // eliminate the boot protocols we have already tried
    net_iface_ipv4_config_options_t tmp_cfg_flags = 0;
    switch(iface->ipv4_cfg_state)
    {
    case IPV4_NOT_CONFIGURED:
        {
            /*
             *  Initialize and begin IPv4 configuration
             * 
             *  NOTE:  State IPV4_NOT_CONFIGURED implies there have been no calls to tfOpenInterface() since
             *  transitioning to state NET_IFACE_STATE_INTERFACE_OPEN (i.e we are neither retrying a failed IPv4
             *  config attempt nor reconfiguring a previously configured IPv4 interface).
             */

            // initialize IPv4 interface context
            iface->ipv4_autoip_probe_collision_cnt = 0;
            iface->ipv4_coll_detect_addr = 0;
            iface->ipv4_dhcp_started = false;
            iface->ipv4_bootp_started = false;

#ifdef HAVE_IPV6
            /*  Bug 15097 - By default, when the stack is operating in dual IPv4/IPv6 mode and a global IPv4
             *  address is acquired through BOOTP/DHCP, an IPv4-compatible IPv6 address corresponding to the
             *  acquired address is also configured.  This causes Treck to open the IPv6 interface *before* we
             *  have started it ourselves. If that happens, DHCP does not set a callback routine to notify us
             *  when new v6 addresses are configured.  Setting this option to 1 disables this behavior.
             * 
             *  NOTE: we do this regardless of the type of IPv4 config method, because once we open the
             *  interface the value cannot be changed (e.g. in case the IPv4 is later reconfigured using
             *  DHCP/BOOTP, which we need to be able to do without completely closing the IPv4/IPv6 interface).
             */
            unsigned char dhcp_option = 1; UNUSED_VAR(dhcp_option);
            int treck_rcode = tfInterfaceSetOptions(iface->treck_interface,
                                                TM_6_DEV_OPTIONS_NO_DHCP_CONF, &dhcp_option, sizeof(dhcp_option));
            if(treck_rcode != TM_ENOERROR)
            {
                DBG_ERR("tfInterfaceSetOptions.0 error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
                iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
                goto error;
            }
#endif // HAVE_IPV6

            tmp_cfg_flags = NET_IFACE_IPV4_USE_MANUAL | NET_IFACE_IPV4_USE_IPCP | NET_IFACE_IPV4_USE_DHCP | NET_IFACE_IPV4_USE_BOOTP | NET_IFACE_IPV4_USE_AUTOIP;
        }
        break;

    case IPV4_CONFIGURING:
    {
        /*
         *  We are either retrying a failed IPv4 config attempt or reconfiguring a previously configured
         *  IPv4 interface
         */
        switch(iface->ipv4_pending_config)
        {
        case NET_IFACE_IPV4_CONFIGURED_MANUAL:
            // no more config methods to try - allow IPv4 config to fail
            break;

        case NET_IFACE_IPV4_CONFIGURED_DHCP:
            // either previous DHCP config attempt failed or DHCP renew was NAK'ed or timed out
            iface->ipv4_dhcp_started = 0;
            tmp_cfg_flags = NET_IFACE_IPV4_USE_BOOTP | NET_IFACE_IPV4_USE_AUTOIP;
            if(iface->ipv4_cfg_status & (IPV4_DHCP_NAK | IPV4_DHCP_LEASE_EXPIRED))
            {
                // a DHCP request packet was NAK'ed or timed out -- attempt to rebind DHCP
                tmp_cfg_flags |= NET_IFACE_IPV4_USE_DHCP;
            }
            break;
        case NET_IFACE_IPV4_CONFIGURED_BOOTP:
            // previous BOOTP config attempt failed
            iface->ipv4_bootp_started = 0;
            tmp_cfg_flags = NET_IFACE_IPV4_USE_AUTOIP;
            break;

        case NET_IFACE_IPV4_CONFIGURED_AUTOIP:
            if(iface->ipv4_cfg_status & IPV4_DUPLICATE_ADDR)
            {
                // collision detected -- retry auto-IP using a different addr
                tmp_cfg_flags = NET_IFACE_IPV4_USE_AUTOIP;
            }
            // for any other auto-IP error status, we allow IPv4 config to fail
            break;

        case NET_IFACE_IPV4_CONFIGURED_IPCP:
            // IPCP config failed
            // ToDo: What should we do here?  Fall back to try again with manual?
            tmp_cfg_flags = 0;  // Don't try other methods.  They won't work over PPPoE yet.
            break;

        default: // invalid IPv4 config method
            DBG_IGNORED("%s() unexpected %s v4 config method %d\n", __func__, iface->name, iface->ipv4_pending_config);
            XASSERT(0, iface->ipv4_pending_config);
            break;
        }
    } break;

    default: // unexpected IPv4 config state
        DBG_IGNORED("%s() unexpected %s v4 state %d\n", __func__, iface->name, iface->ipv4_cfg_state);
        XASSERT(0, iface->ipv4_cfg_state);
        break;
    }

    // compare with user-selected IPv4 config options
    net_iface_ipv4_config_options_t cfg_flags = 0;
    net_iface_get_var_ipv4_config_options(iface->treck_idx, &cfg_flags);
    tmp_cfg_flags &= cfg_flags;
    if(!tmp_cfg_flags)
    {
#ifdef __linux__ // no more config method options -- reset auto config
        DBG_ERR("%s(%s) no more config options ..\n", __func__, iface->name);
        tmp_cfg_flags = NET_IFACE_IPV4_USE_DHCP;
        net_iface_set_var_ipv4_config_options(iface->treck_idx, &tmp_cfg_flags); // reset nvram
        if (!intf_up(iface->name))
        {
            adv_v4_state(iface, IPV4_NOT_CONFIGURED);
            goto error;
        }
#else
        // no more config options - allow IPv4 config to fail
        goto error;
#endif
    }

    // reset error condition flags and try next config method
    adv_v4_state(iface, IPV4_NOT_CONFIGURED);

    // set default flags for tfOpenInterface
    default_open_flags |= TM_DEV_MCAST_ENB; //enable multicast
    #ifdef HAVE_HOTSPOT_SUPPORT
    if (wifi_hotspot_allowed_by_platform())
    {
        default_open_flags |= NETDRVR_IPV4_HOTSPOT_DEVICE_OPTIONS;
    }
    #endif // HAVE_HOTSPOT_SUPPORT

    // attempt the next boot protocol in the sequence
    adv_v4_state(iface, IPV4_CONFIGURING);
    #ifdef TM_USE_PPPOE_CLIENT
    if (tmp_cfg_flags & NET_IFACE_IPV4_USE_IPCP)
    {
        ipv4_open_interface_ipcp(iface, default_open_flags);
    }
    else
    #endif // TM_USE_PPPOE_CLIENT
    if(tmp_cfg_flags & NET_IFACE_IPV4_USE_MANUAL)
    {
        ipv4_open_interface_manual(iface, default_open_flags);
    }
    else if(tmp_cfg_flags & NET_IFACE_IPV4_USE_DHCP)
    {
        ipv4_open_interface_dhcp(iface, default_open_flags);
    }
    else if(tmp_cfg_flags & NET_IFACE_IPV4_USE_BOOTP)
    {
        ipv4_open_interface_bootp(iface, default_open_flags);
    }
    else if(tmp_cfg_flags & NET_IFACE_IPV4_USE_AUTOIP)
    {
        ipv4_open_interface_autoip(iface, default_open_flags);
    }

    DBG_VERBOSE("<==%s(%s) success\n", __func__, iface->name);
    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;

error:
    DBG_ERR("<==%s(%s) failed!!\n", __func__, iface->name);
    net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_FAILED, iface, (void *)0, (void *)0);
    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;
}

STATIC void ipv4_reconfigure_interface(net_interface_t *iface)
{
    show_current_if_states();
    if(iface->ipv4_cfg_state == IPV4_CONFIGURED)
    {
        // we must first unconfigure the IPv4 interface before retrying IPv4 configuration
        net_iface_ipv4_config_method_t config_method = NET_IFACE_IPV4_CONFIGURED_DHCP;
        net_iface_get_var_ipv4_last_config_method(iface->treck_idx, &config_method);
        iface->ipv4_pending_config = config_method;
        adv_v4_state(iface, IPV4_RECONFIGURING);
        ipv4_close_interface(iface);
    }
    else if(iface->ipv4_cfg_state == IPV4_CONFIGURING)
    {
        // last config attempt was not successful, so retry IPv4 configuration
        ipv4_open_interface(iface);
    }
    return;
}

STATIC int32_t ipv4_close_interface(net_interface_t *iface)
{
    show_current_if_states();
    int treck_rcode = 0;
    // cleanup IPv4 context
    iface->ipv4_dhcp_lease_time = 0;

    // cancel any ongoing ipv4 collision detection
    ipv4_cancel_collision_detection(iface);

    // must change IPv4 config state *before* interface is closed (interface is guaranteed to be up while
    // in state IPV4_CONFIGURED)
    bool was_configured = false;
    if(iface->ipv4_cfg_state >= IPV4_RECONFIGURING)
    {
        was_configured = true;
    }

    // if not reconfiguring then close IPv4
    if(iface->ipv4_cfg_state != IPV4_RECONFIGURING)
    {
        adv_v4_state(iface, IPV4_CLOSING);
    }

    if(was_configured)
    {
        // must notify registered callbacks *after* changing state but *before* closing the interface
        event_list_notify(iface, NET_IFACE_EVENT_IPV4_CONFIG_CHANGE);
    }

    DBG_VERBOSE("==>%s\n",__func__);
    if(iface->ipv4_dhcp_started)
    {
        // release DHCP address and free the associated boot entry 
        // NOTE: if we don't do this then Treck will continue renewing the DHCP addr even after
        // tfUnConfigInterface() is called
        iface->ipv4_dhcp_started = false;
        treck_rcode = tfDhcpUserRelease(iface->treck_interface, 0 /* multi-home index */);
        XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
    }

    if(iface->ipv4_bootp_started)
    {
        // cancel previous BOOTP request and free the associated boot entry
        iface->ipv4_bootp_started = false;
        treck_rcode = tfBootpUserStop(iface->treck_interface, 0 /* multi-home index */);
        XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
    }

    ipv4_finish_close_interface(iface);
    return NET_IFACE_OK;
}

STATIC void ipv4_finish_close_interface(net_interface_t* iface)
{
    show_current_if_states();
    int treck_rcode = TM_ENOENT;
    DBG_VERBOSE("%s(x%x) '%s'\n", __func__, iface, iface->name);
#ifdef __linux__
    uint8_t mac_addr[MAC_ADDR_BYTE_SIZE];
    if (NULL != get_intf_mac_addr(iface->name, mac_addr))
    {
        char os_req[32] = { SBIN "/ifdown -f " };
        strcat(os_req, iface->name);
        os_system(os_req);
        treck_rcode = TM_ENOERROR;
    }
#else
    int treck_rcode = tfUnConfigInterface(iface->treck_interface, 0 /* multiHomeIndex */);
    if(treck_rcode != TM_ENOERROR && treck_rcode != TM_ENOENT)
    {
        DBG_ERR("tfUnConfigInterface error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        adv_v4_state(iface, IPV4_NOT_CONFIGURED);
        add_v4_state(iface, IPV4_INTERNAL_ERROR);
        goto done;
    }
#endif

    // IPv4 close complete 
    // NOTE: the actual Treck interface is still open at this point
    DBG_VERBOSE("IPv4 is down on %s\n", iface->name);

    switch(iface->ipv4_cfg_state)
    {
    case IPV4_CLOSING:
        adv_v4_state(iface, IPV4_NOT_CONFIGURED);
        break;

    case IPV4_RECONFIGURING:
        // reconfigure the IPv4 interface
        adv_v4_state(iface, IPV4_CONFIGURING);
        ipv4_open_interface(iface);
        break;

    default:
        // unexpected ipv4 state
#ifdef __linux__
        DBG_IGNORED("%s(%s):%d unexpected v4 state %d\n", __func__, iface->name, __LINE__, iface->ipv4_cfg_state);
#else
        XASSERT(0, iface->ipv4_cfg_state);
#endif
        break;
    }
#ifndef __linux__
done:
#endif
    return;
}

STATIC int ipv4_collision_detect_notify_func(ttUserInterface treck_interface, ttUserIpAddress ipv4_addr,
                                             int collision_detect_rcode, ttUserGenericUnion input)
{
    DBG_VERBOSE("%s() ...\n", __func__);
    net_interface_t *iface = net_iface_recover_local_context(treck_interface);
    ASSERT(iface);
    show_current_if_states();

    switch(iface->ipv4_cfg_state)
    {

    default:
        // unexpected collision detected -- ignore
        DBG_IGNORED("%s() ignored %s intf %d v4-state %d\n", __func__, iface->name, treck_interface, iface->ipv4_cfg_state);
        break;
    }

    return 0;
}

STATIC void ipv4_open_interface_manual(net_interface_t *iface, int default_open_flags)
{
    DBG_VERBOSE("starting IPv4 manual config on %s\n", iface->name);
    show_current_if_states();

    iface->ipv4_pending_config = NET_IFACE_IPV4_CONFIGURED_MANUAL;

    int treck_rcode;

    // set manual-specific open flags for tfOpenInterface
    int manual_open_flags = (default_open_flags | TM_DEV_IP_USER_BOOT); UNUSED_VAR(manual_open_flags);

    // get user-defined manual config info

    // validate user-defined manual config info
    // rdj TODO -- add additional validation, e.g. not broadcast or multicast addr, and not directed
    // broadcast for given mask, etc.? or is this done from the UI?
    uint32_t ipv4_addr;
    net_iface_get_var_ipv4_manual_addr(iface->treck_idx, &ipv4_addr);
    if(!ipv4_addr)
    {
        DBG_MSG("IPv4 manual addr %u.%u.%u.%u is invalid on %s\n",
                NET_IPV4_OCTET(ipv4_addr, 0), NET_IPV4_OCTET(ipv4_addr, 1),
                NET_IPV4_OCTET(ipv4_addr, 2), NET_IPV4_OCTET(ipv4_addr, 3),
                iface->name);
        iface->ipv4_cfg_status |= IPV4_BAD_MANUAL_CONFIG;
        goto error;
    }

    /*  open the interface for user control
     *
     *  NOTE: we can get TM_EINPROGRESS if tfOpenInterface() has already been called for a previous
     *  config attempt without a corresponding tfFinishOpenInterface()
     */
    treck_rcode = tfOpenInterface(iface->treck_interface,
                                  /*ip addr*/ 0, /*net mask*/ 0,
                                  manual_open_flags, NET_IFACE_TX_BUFS_PER_FRAME);
    if(treck_rcode != TM_ENOERROR && treck_rcode != TM_EINPROGRESS)
    {
        DBG_ERR("tfOpenInterface error (treck_rcode=%d)\n", treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }
#ifdef __linux__
// currently unused -- revisit
#else

    /* 
     * Initialize collision detection.  tfUseCollisionDetection will return immediately with
     * TM_EADDRINUSE if the address is found in the ARP cache (since we know another host is using the
     * addr without needing to send ARP probes). Otherwise a series of ARP probes will be sent, and we
     * will get collision detection status via the callback.
     */
#ifdef TM_USE_STRONG_ESL
    treck_rcode = tfUseCollisionDetection(
        iface->treck_interface,
        ipv4_addr,
        ipv4_collision_detect_notify_func,
        (ttUserGenericUnion)(void *)iface);
#else
    treck_rcode = tfUseCollisionDetection(
        ipv4_addr,
        ipv4_collision_detect_notify_func,
        (ttUserGenericUnion)(void *)iface);
#endif
    if(treck_rcode != TM_ENOERROR && treck_rcode != TM_EADDRINUSE)
    {
        DBG_ERR("tfUseCollisionDetection error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }
    if(treck_rcode == TM_EADDRINUSE)
    {
        DBG_MSG("IPv4 manual addr %u.%u.%u.%u already in use on network (arp cache)\n", NET_IPV4_OCTET(ipv4_addr, 0),
                 NET_IPV4_OCTET(ipv4_addr, 1), NET_IPV4_OCTET(ipv4_addr, 2), NET_IPV4_OCTET(ipv4_addr, 3));
        iface->ipv4_cfg_status |= IPV4_DUPLICATE_ADDR;
        goto error;
    }
    ASSERT(!iface->ipv4_coll_detect_addr);
    iface->ipv4_coll_detect_addr = ipv4_addr;

    // the manual addr was not found in the ARP cache, so start sending ARP probes
    treck_rcode = tfUserStartArpSend (iface->treck_interface, ipv4_addr,
                                       NET_IFACE_IPV4_MANUAL_NUM_ARP_PROBES,
                                       NET_IFACE_IPV4_MANUAL_ARP_PROBE_INTERVAL_MS,
                                       NET_IFACE_IPV4_MANUAL_ARP_PROBE_TIMEOUT_MS);
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfUserStartArpSend error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }
#endif // ! __linux__

    return;

error:
    // retry with another addr or config type
    net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);

    return;
}

STATIC void ipv4_open_interface_dhcp(net_interface_t *iface, int default_open_flags)
{
    int treck_rcode = TM_ERROR;

    iface->ipv4_pending_config = NET_IFACE_IPV4_CONFIGURED_DHCP;

    DBG_MSG("starting IPv4 DHCP config on %s\n", iface->name);

#ifdef __linux__
    show_current_if_states();
    uint8_t mac_addr[MAC_ADDR_BYTE_SIZE];
    if (NULL != get_intf_mac_addr(iface->name, mac_addr)) // hw_present? revisit intf_up()?
    {
        treck_rcode = TM_EINPROGRESS;
        ipv4_addr_notify_func(iface, TM_ENOERROR, 0 /*multi-home*/);
    }
#else

    int dhcp_conf_flags = 0;
    int multihome_idx; // used for assigning multiple IP addrs on the same interface (multihoming)
    ttUserIpAddress requested_ip_addr;

    // set DHCP-specific flags for tfOpenInterface
    int dhcp_open_flags = (default_open_flags | TM_DEV_IP_USER_BOOT);

    // must use 0 for first multihome index (which in our case is the interface's only index)
    multihome_idx = 0;

    // we explicitly request our previous IP addr
    net_iface_get_var_ipv4_manual_addr(iface->treck_idx, &requested_ip_addr);

    /*
     *  configure Treck DHCP options
     */

    unsigned char dhcp_option;

    dhcp_option = NET_IFACE_IPV4_BOOT_BASE_TIMEOUT;
    treck_rcode = tfInterfaceSetOptions(iface->treck_interface,
                                        TM_DEV_OPTIONS_BOOT_TIMEOUT, &dhcp_option, sizeof(dhcp_option));
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfInterfaceSetOptions.1 error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    dhcp_option = NET_IFACE_IPV4_BOOT_MAX_RETRIES;
    treck_rcode = tfInterfaceSetOptions(iface->treck_interface,
                                        TM_DEV_OPTIONS_BOOT_RETRIES, &dhcp_option, sizeof(dhcp_option));
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfInterfaceSetOptions.2 error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }
    dhcp_conf_flags |= TM_DHCPF_SUPPRESS_CLIENT_ID;
    if(requested_ip_addr)
    {
        dhcp_conf_flags |= TM_DHCPF_REQUESTED_IP_ADDRESS;
    }
    treck_rcode = tfDhcpUserSet(iface->treck_interface, multihome_idx,
                                dhcp_conf_flags, requested_ip_addr, (unsigned char *)NULL, 0);
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfDhcpConfSet IPaddr error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    // configure DHCP FQDN option (rcf 4702)
    char hostname[NET_IFACE_HOSTNAME_SIZE];
    net_iface_get_var_hostname(iface->treck_idx, hostname, NET_IFACE_HOSTNAME_SIZE);
    if(hostname[0])
    {
        int flags = TM_DHCPF_FQDN_PARTIAL;
        treck_rcode = tfUserSetFqdn(hostname, strlen(hostname), flags);
        if(treck_rcode != TM_ENOERROR)
        {
            DBG_ERR("tfUserSetFqdn error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
            iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
            goto error;
        }

        flags = TM_DHCPF_FQDN_PARTIAL | TM_DHCPF_FQDN_ENABLE;
        treck_rcode = tfDhcpUserSet(iface->treck_interface, multihome_idx, flags, 0,0,0);
        if(treck_rcode != TM_ENOERROR)
        {
            DBG_ERR("tfDhcpConfSet FQDN error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
            iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
            goto error;
        }

        // bug 11801 - some router gateways use the hostname sent in option 81 'Client FQDN'
        // to populate lists internal to the router gateway - others require us to explicity 
        // send the hostname in option 12
        treck_rcode = tfDhcpUserSetOption(iface->treck_interface, multihome_idx,
                                          12, hostname, strlen(hostname));
        if(treck_rcode != TM_ENOERROR)
        {
            DBG_ERR("tfDhcpConfSetOption 12 error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
            iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
            goto error;
        }
    }

    /*
     *  configure DHCP request options (rcf 2132)
     */

    // set DHCP Parameter Request List (option 55)
    // NOTE: overrides treck default, which included options 66 (boot server name), 67 (boot file name),
    // 13 (boot file size), and 6 (DNS Server)
    // bug 13250 - remove DHCP option 6 DNS server
    // option 1 = Subnet Mask
    // option 3 = Gateway
    // option 6 = Domain Name Servers
    // option 12 = Host Name
    // option 15 = Domain Name
    // option 44 = NetBIOS name server 
    char opt_data[] = {0x03,0x01,0x0f,0x2c,0x0c,0x06};
    treck_rcode = tfDhcpUserSetOption(iface->treck_interface, multihome_idx,
                                      55, opt_data, sizeof(opt_data));
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfDhcpConfSetOption 55 error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    /*
     *  open the interface for user control
     * 
     *  NOTE: we can get TM_EINPROGRESS if tfOpenInterface() has already been called for a previous
     *  config attempt without a corresponding tfFinishOpenInterface()
     */

    treck_rcode = tfOpenInterface(iface->treck_interface,
                                  /*ip addr*/ 0, /*net mask*/ 0,
                                  dhcp_open_flags, NET_IFACE_TX_BUFS_PER_FRAME);
    if(treck_rcode != TM_ENOERROR && treck_rcode != TM_EINPROGRESS)
    {
        DBG_ERR("tfOpenInterface error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    /*
     *  initiate DHCP and wait for response
     */

    treck_rcode = tfDhcpUserStart(iface->treck_interface, multihome_idx, ipv4_boot_notify_func);

#endif // ! __linux__

    if(treck_rcode != TM_EINPROGRESS)
    {
        DBG_ERR("tfDhcpUserStart error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }
    iface->ipv4_dhcp_started = true;

    return;

error:
    // retry with another addr or config type
    net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);
    return;
}

STATIC void ipv4_open_interface_bootp(net_interface_t *iface, int default_open_flags)
{
    DBG_MSG("starting IPv4 BOOTP config on %s\n", iface->name);

    iface->ipv4_pending_config = NET_IFACE_IPV4_CONFIGURED_BOOTP;

#ifdef __linux__
    show_current_if_states();
    if (intf_up(iface->name))
    {
        ipv4_addr_notify_func(iface, TM_ENOERROR, 0 /*multi-home*/);
    }
#else

    int treck_rcode;

    // set BOOTP-specific flags for tfOpenInterface
    int bootp_open_flags = (default_open_flags | TM_DEV_IP_USER_BOOT);

    /*
     *  configure Treck BOOTP options
     */

    unsigned char bootp_option;

    bootp_option = NET_IFACE_IPV4_BOOT_BASE_TIMEOUT;
    treck_rcode = tfInterfaceSetOptions(iface->treck_interface,
                                        TM_DEV_OPTIONS_BOOT_TIMEOUT, &bootp_option, sizeof(bootp_option));
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfInterfaceSetOptions.1 error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    bootp_option = NET_IFACE_IPV4_BOOT_MAX_RETRIES;
    treck_rcode = tfInterfaceSetOptions(iface->treck_interface,
                                        TM_DEV_OPTIONS_BOOT_RETRIES, &bootp_option, sizeof(bootp_option));
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfInterfaceSetOptions.2 error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }
    /*
     *  open the interface for user control
     * 
     *  NOTE: we can get TM_EINPROGRESS if tfOpenInterface() has already been called for a previous
     *  config attempt without a corresponding tfFinishOpenInterface()
     */

    treck_rcode = tfOpenInterface(iface->treck_interface,
                                  /*ip addr*/ 0, /*net mask*/ 0,
                                  bootp_open_flags, NET_IFACE_TX_BUFS_PER_FRAME);
    if(treck_rcode != TM_ENOERROR && treck_rcode != TM_EINPROGRESS)
    {
        DBG_ERR("tfOpenInterface error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    /*
     *  initiate BOOTP and wait for response
     */

    treck_rcode = tfBootpUserStart(iface->treck_interface, 0 /* multi-home index */, ipv4_boot_notify_func);
    if(treck_rcode != TM_EINPROGRESS)
    {
        DBG_ERR("tfBootpUserStart error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }
#endif // ! __linux__

    iface->ipv4_bootp_started = true;

    return;

#ifndef __linux__
error:
#endif
    // retry with another addr or config type
    net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);
    return;
}

STATIC void ipv4_open_interface_autoip(net_interface_t *iface, int default_open_flags)
{
    show_current_if_states();
    DBG_MSG("starting IPv4 auto-IP config on %s\n", iface->name);
    return;
}

#ifdef TM_USE_PPPOE_CLIENT
STATIC void ipv4_open_interface_ipcp(net_interface_t *iface, int default_open_flags)
{
    show_current_if_states();
    static    uint16_t pppoed_attempt_number = 0;
    char     *pppoe_data_buff = NULL;
    int       data_buff_len = MAX(PPPOE_USERNAME_MAX_LENGTH + 1, PPPOE_PASSWORD_MAX_LENGTH + 1);
    uint32_t  ipv4_addr = 0;

    DBG_MSG("starting IPv4 PPPoE config on iface %s\n", iface->name);

    return;
}
#endif // TM_USE_PPPOE_CLIENT

STATIC void ipv4_open_interface_complete(net_interface_t *iface)
{
    show_current_if_states();
    bool need_to_finish_open = true;

    if(iface->ipv4_cfg_state != IPV4_CONFIGURING)
    {
        XASSERT(0, iface->ipv4_cfg_state);
        return;
    }

    int treck_rcode = 0;
    uint8_t multihome_idx = 0; UNUSED_VAR(multihome_idx);

    // populate the following vars with appropriate values for each of the possible config methods
    uint32_t ipv4_addr = 0;
    uint32_t ipv4_netmask = 0;
    uint32_t ipv4_gateway = 0;
    switch(iface->ipv4_pending_config)
    {
    case NET_IFACE_IPV4_CONFIGURED_MANUAL:

        net_iface_get_var_ipv4_manual_addr(iface->treck_idx, &ipv4_addr);
        net_iface_get_var_ipv4_manual_subnet_mask(iface->treck_idx, &ipv4_netmask);
        net_iface_get_var_ipv4_manual_gateway(iface->treck_idx, &ipv4_gateway);
        break;

    case NET_IFACE_IPV4_CONFIGURED_DHCP:
    case NET_IFACE_IPV4_CONFIGURED_BOOTP:
        {
            ttUserBtEntryPtr boot_entry;
            if(iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_BOOTP)
            {
                boot_entry = tfBootpUserGetBootEntry(iface->treck_interface, multihome_idx);
                if(!boot_entry)
                {
                    DBG_ERR("tfBootpUserGetBootEntry failed on %s\n", iface->name);
                    iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
                    goto error;
                }
            }
            else // ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_DHCP
            {
                boot_entry = tfDhcpUserGetBootEntry(iface->treck_interface, multihome_idx);
#ifndef __linux__
                if(!boot_entry)
                {
                    DBG_ERR("tfDhcpUserGetBootEntry failed on %s\n", iface->name);
                    iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
                    goto error;
                }
#endif

                /*  Track the DHCP lease time remaining.  Note that btuDhcpLeaseTime is the total lease time (in
                 *  milliseconds) remaining at the time the server response was processed by Treck, i.e. Treck
                 *  has already adjusted for the time elapsed since the DHCP request was sent.
                 * 
                 *  We must save the current value of btuDhcpLeaseTime because it will be updated later by Treck
                 *  after the DHCP T1 timer expires (i.e. when Treck's DHCP client enters the RENEWING state).
                 */
                extern tt32Bit tvTime;
                iface->ipv4_dhcp_lease_start = tvTime; // internal Treck system time (milliseconds)
                iface->ipv4_dhcp_lease_time = boot_entry->btuDhcpLeaseTime;
                net_set_ipv4_manual_wins_addr(iface->treck_idx, &boot_entry->btuNetBiosNameServers[0]);
#if 0
                if(boot_entry->btuNetBiosNumNameServers)
                {
                    // use internal routine to prevent trigger of net reset
                    net_set_ipv4_manual_wins_addr(iface->treck_idx, &boot_entry->btuNetBiosNameServers[0]);
                }
#endif
            }
            ipv4_addr = boot_entry->btuYiaddr;
            ipv4_netmask = boot_entry->btuNetMask;
            ipv4_gateway = boot_entry->btuDefRouter;

            // store network configuration (use internal routines to prevent trigger of net reset)
            net_set_ipv4_manual_addr(iface->treck_idx, &ipv4_addr);
            net_set_ipv4_manual_subnet_mask(iface->treck_idx, &ipv4_netmask);
            net_set_ipv4_manual_gateway(iface->treck_idx, &ipv4_gateway);
        }
        break;

    case NET_IFACE_IPV4_CONFIGURED_AUTOIP:
        {
            ipv4_addr = iface->ipv4_coll_detect_addr; // currently configured local auto-IP addr
            ipv4_netmask = htonl(TM_IP_LOCAL_NETMASK);

            /*
             *  Support mixed networks (link-local to routable).
             * 
             *  In this case we need the ability to send from the link-local source addr (i.e. auto-IP) to a
             *  routable dest addr. To do this, we add a default route (dest addr 0.0.0.0, netmask 0.0.0.0) with
             *  the local auto-IP addr as the gateway. Doing so effectively routes all otherwise unroutable
             *  packets (e.g. non 169.254/16 and non loopback addrs) out onto the local link.
             * 
             *  NOTE: Bonjour Conformance Test ver. 1.2.3 also requires us to support the inverse case (routable
             *  addr to link-local addr), which we account for elsewhere in the code by adding a static route
             *  for 169.254/16 whenever printer is configured with a routable addr.
             */
            ipv4_gateway = ipv4_addr;
        }
        break;

    case NET_IFACE_IPV4_CONFIGURED_IPCP:
#ifdef TM_USE_PPPOE_CLIENT
        {
            need_to_finish_open = false;
            tfGetPppPeerIpAddress(iface->treck_interface, &ipv4_gateway);
            tfGetIpAddress(iface->treck_interface, &ipv4_addr, 0);
            tfGetNetMask(iface->treck_interface, &ipv4_netmask, 0);
        }
#endif // TM_USE_PPPOE_CLIENT
        break;

    default:
        // invalid config method
        XASSERT(0, iface->ipv4_pending_config);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    DBG_VERBOSE("%s() %s v4 addr %s netmask %s gateway %s (finishing %d)\n", __func__, iface->name, ipv4_b2str(ipv4_addr), ipv4_b2str(ipv4_netmask), ipv4_b2str(ipv4_gateway), need_to_finish_open);

    if (need_to_finish_open)
    {
        // insert the newly acquired ipaddress/netmask into the routing table
        // NOTE: at this point Treck considers the IPv4 interface "configured", and tfUnConfigInterface()
        // must be called prior to any reconfiguring (i.e. must transition to state IPV4_CONFIGURED)
        treck_rcode = tfFinishOpenInterface(iface->treck_interface, ipv4_addr, ipv4_netmask);
        if(treck_rcode != TM_ENOERROR)
        {
            DBG_ERR("tfFinishOpenInterface error on %s (error=%d)\n", iface->name, treck_rcode);
            iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
            goto error;
        }
    }
    net_set_ipv4_last_config(iface->treck_idx, &iface->ipv4_pending_config);
    adv_v4_state(iface, IPV4_CONFIGURED);

    // use this interface as the default when sending outgoing multicast packets
    treck_rcode = tfSetMcastInterface(iface->treck_interface, multihome_idx);
    if(treck_rcode != TM_ENOERROR && treck_rcode != TM_EALREADY)
    {
        DBG_ERR("tfSetMcastInterface error on %s (error=%d)\n", iface->name, treck_rcode);
        iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
        goto error;
    }

    // add system default gateway to the routing table
    if(ipv4_gateway)
    {
#ifdef TM_USE_STRONG_ESL
        treck_rcode = tfAddDefaultGateway(iface->treck_interface, ipv4_gateway, NET_IFACE_INTERFACE_HOPS);
#else
        treck_rcode = tfAddDefaultGateway(iface->treck_interface, ipv4_gateway);
#endif
        if(treck_rcode != TM_ENOERROR &&
           treck_rcode != TM_EHOSTUNREACH)
        {
            DBG_ERR("tfAddDefaultGateway error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
            iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
            goto error;
        }
    }

    // support mixed networks (routable to link-local)
    if(iface->ipv4_pending_config != NET_IFACE_IPV4_CONFIGURED_AUTOIP)
    {
        ipv4_add_autoip_static_route(iface, ipv4_addr);
    }

    // Start collision detection to detect late collisions for DHCP and BOOTP. This must
    // not happen until after we transition to state IPV4_CONFIGURED.
    if( (iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_DHCP)
        || (iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_BOOTP) )
    {
        /* 
         * Initialize collision detection.  tfUseCollisionDetection will return immediately with
         * TM_EADDRINUSE if the address is found in the ARP cache (since we know another host is using the
         * addr without needing to send ARP probes). Otherwise a series of ARP probes will be sent, and we
         * will get collision detection status via the callback.
         */
#ifdef TM_USE_STRONG_ESL
        treck_rcode = tfUseCollisionDetection(
            iface->treck_interface,
            ipv4_addr,
            ipv4_collision_detect_notify_func,
            (ttUserGenericUnion)(void *)iface);
#else
        treck_rcode = tfUseCollisionDetection(
            ipv4_addr,
            ipv4_collision_detect_notify_func,
            (ttUserGenericUnion)(void *)iface);
#endif
        if(treck_rcode != TM_ENOERROR && treck_rcode != TM_EADDRINUSE)
        {
            DBG_ERR("tfUseCollisionDetection error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
            iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
            goto error;
        }
#ifndef __linux__
        if(treck_rcode == TM_EADDRINUSE)
        {
            DBG_MSG("Late collision detected for IPv4 addr %u.%u.%u.%u (arp cache) on %s\n",
                    NET_IPV4_OCTET(ipv4_addr, 0), NET_IPV4_OCTET(ipv4_addr, 1),
                     NET_IPV4_OCTET(ipv4_addr, 2), NET_IPV4_OCTET(ipv4_addr, 3),
                    iface->name);
            iface->ipv4_cfg_status |= IPV4_DUPLICATE_ADDR;
            goto error;
        }
#endif
        ASSERT(!iface->ipv4_coll_detect_addr);
        iface->ipv4_coll_detect_addr = ipv4_addr;
    }

    /*  Send a second announcement. Treck sends a single gratuitous ARP announcement, but Bonjour
     *  Conformance Test 1.2.3 requires two for auto-IP (we also generate a second announcement for the
     *  other IPv4 config methods, i.e. DHCP, BOOTP, and manual config).
     * 
     *  NOTE: Collision detection will not start for DHCP or BOOTP until after this message is sent and
     *  tfUserStartArpSend() is called.  Treck documentation for tfUseCollisionDetection() states that
     *  "collision detection will only start after the user calls tfUserStartArpSend."
     */
    NET_LOCK_IFACE_CONTEXT(iface);
    net_timer_cancel(iface->msg_timer_arp.handle); // cancel outstanding msg timer if any
    iface->msg_timer_arp.msg = NET_IFACE_MSG_IPV4_ARP_ANNOUNCE;
    iface->msg_timer_arp.handle = net_timer_start_ms(
                                    NET_TIMER_FLAG_REAL_TIME,
                                    NET_IFACE_IPV4_ARP_ANNOUNCE_INTERVAL_MS,
                                    delayed_msg_timer_func,
                                    (void *)&iface->msg_timer_arp);

    ASSERT(iface->msg_timer_arp.handle != NET_TIMER_INVALID_HANDLE);
    NET_UNLOCK_IFACE_CONTEXT(iface);

    DBG_MSG("IPv4 is up on %s\n", iface->name);

    ASSERT(iface->ipv4_cfg_state == IPV4_CONFIGURED);
    event_list_notify(iface, NET_IFACE_EVENT_IPV4_CONFIG_CHANGE);

#ifdef HAVE_HOTSPOT_SUPPORT
    if (wifi_hotspot_allowed_by_platform())
    {
        bool hotspot_enabled;

        wifi_get_var_hotspot_enabled(&hotspot_enabled);

        if (strcmp(iface->name, NET_IFACE_INTERFACE_NAME) == 0)
        {
            if (hotspot_enabled)
            {
                hotspot_subnet_blacklist_t *blacklisted_wans;
                uint32_t blacklist_addr;
                uint32_t blacklist_mask;

                blacklisted_wans = wifi_hotspot_get_blacklisted_wans();
                while ((blacklisted_wans != NULL) && (blacklisted_wans->network != NULL) && (blacklisted_wans->subnet != NULL))
                {
                    blacklist_addr = inet_aton(blacklisted_wans->network);
                    blacklist_mask = inet_aton(blacklisted_wans->subnet);

                    if ((ipv4_addr & blacklist_mask) == (blacklist_addr & blacklist_mask))
                    {
                        DBG_ERR("ERROR: Trying to connect to blacklisted WAN.  For security reasons NAT and packet routing will NOT be enabled.\n");
                        hotspot_enabled = false;
                        ASSERT("Attempted to connect hotspot to a forbidden network." == 0);
                    }

                    // Check next subnet in the list
                    blacklisted_wans++;
                }
            }

            if (hotspot_enabled)
            {
                uint8_t cur_wifi_sta_bssid[TM_ETHERNET_PHY_ADDR_LEN];
                wifi_intf_get_var_bssid(WIFI_IFACE_ID_STATION, cur_wifi_sta_bssid, TM_ETHERNET_PHY_ADDR_LEN);

                tfNatSetTemporaryDisable(0);

                if (iface->nat_enabled)
                {
#if 0 // TODO - Revisit
                    // NAT is still enabled from a previuos network connection.  We want to keep the
                    // triggers if possible, so check to see if they got the same IP address and are
                    // on the same network as before.  If so, just leave everything alone.  If not, tear
                    // down NAT and set it back up again.
                    if ((prev_nat_public_ip != ipv4_addr) || 
                        (prev_nat_wan_link_type != g_ctxt.bound_link->type) ||
                        ((g_ctxt.bound_link->type == NETDRVR_LINK_TYPE_ETH) && (g_ctxt.iface.treck_link_protocol == LINK_PROTOCOL_ETHERNET)) ||   // We can't guarantee we didn't switch networks on Ethernet
                        ((g_ctxt.bound_link->type == NETDRVR_LINK_TYPE_WLAN) && (memcmp(prev_wifi_bssid, cur_wifi_sta_bssid, TM_ETHERNET_PHY_ADDR_LEN) != 0)))
#endif // TODO
                    {
                        DBG_MSG("NAT was previously configured, but we are now on a different network - tear down and reconfigure\n");

                        iface->nat_enabled = false;
                        // Unconfigure previous instance of NAT
                        tfNatUnConfig(iface->treck_interface);
                    }
#if 0 // TODO - Revisit
                    else
                    {
                        treck_rcode = tfSetTreckOptions(TM_OPTION_IP_FORWARDING, 1UL);
                        XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                        treck_rcode = tfSetTreckOptions(TM_OPTION_IP_MCAST_FORWARD, 1UL);
                        XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                    }
#endif // TODO
                }

                if (!iface->nat_enabled)
                {
                    DBG_MSG("Configuring NAT...\n");

                    treck_rcode = tfSetTreckOptions(TM_OPTION_IP_MCAST_FORWARD, 1UL);
                    XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                    treck_rcode = tfSetTreckOptions(TM_OPTION_IP_FORWARDING, 1UL);
                    if (treck_rcode == TM_ENOERROR)
                    {
                        tfNatConfigMaxEntries(NETDRVR_NAT_MAX_ENTRIES);
                        treck_rcode = tfNatConfig(iface->treck_interface);
                        if (treck_rcode == TM_ENOERROR)
                        {
                            treck_rcode = tfNatConfigNapt(iface->treck_interface, ipv4_addr, NETDRVR_NAPT_PORT_START, NETDRVR_NAPT_PORT_END);
                            if (treck_rcode == TM_ENOERROR)
                            {
                                DBG_MSG("NAT is now configured\n");
                                iface->nat_enabled = true;

                                // Remember info about how we were connected when we configured NAT so we can know
                                // whether to keep the settings on link open events
                                prev_nat_public_ip = ipv4_addr;
                                // TODO - Revisit
    //                            prev_nat_wan_link_type = g_ctxt.bound_link->type;
                                memcpy(prev_wifi_bssid, cur_wifi_sta_bssid, TM_ETHERNET_PHY_ADDR_LEN);
                            }
                            else
                            {
                                DBG_ERR("Failed to configure NAPT: %d\n", treck_rcode);
                                tfNatUnConfig(iface->treck_interface);
                                treck_rcode = tfSetTreckOptions(TM_OPTION_IP_FORWARDING, 0UL);
                                XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                                treck_rcode = tfSetTreckOptions(TM_OPTION_IP_MCAST_FORWARD, 0UL);
                                XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                            }
                        }
                        else
                        {
                            DBG_ERR("Failed to configure NAT on public interface: %d\n", treck_rcode);
                        }
                    }
                    else
                    {
                        DBG_ERR("Failed to enable forwarding (routing) between internal interfaces: %d\n", treck_rcode);
                    }
                }
                else
                {
                    DBG_MSG("Re-using existing NAT configuration\n");
                }
            }

            else
            {
                if (iface->nat_enabled)
                {
                    iface->nat_enabled = false;
                    // Make sure forwarding and NAT are disabled
                    tfNatUnConfig(iface->treck_interface);
                    treck_rcode = tfSetTreckOptions(TM_OPTION_IP_FORWARDING, 0UL);
                    XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                    treck_rcode = tfSetTreckOptions(TM_OPTION_IP_MCAST_FORWARD, 0UL);
                    XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                }
            }
        }

#if 0 // TODO - Revisit
        // If we got a valid DNS setting, keep track of it in case we bring up the uAP later
        // before we are able to get a good setting.
        error_type_t err_result;
        uint32_t     tmp_addr;
        err_result = net_dns_get_var_preferred(&tmp_addr);
        if (err_result == NETDRVR_OK)
        {
            net_dns_set_var_manual_preferred_internal(&tmp_addr, false);
        }
        err_result = net_dns_get_var_alternate(&tmp_addr);
        if (err_result == NETDRVR_OK)
        {
            net_dns_set_var_manual_alternate_internal(&tmp_addr, false);
        }
#endif // TODO
    }
#endif // HAVE_HOTSPOT_SUPPORT

    return;

error:
    net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_FAILED, iface, (void *)0, (void *)0);
    return;
}

#ifdef __linux__
STATIC void ipv4_addr_notify_func(net_interface_t* iface, int boot_notify_rcode, int multihome_idx)
#else
STATIC void ipv4_boot_notify_func(ttUserInterface treck_interface, int boot_notify_rcode, int multihome_idx)
#endif
{
#ifndef __linux__
    net_interface_t *iface = net_iface_recover_local_context(treck_interface);
#endif
    ASSERT(iface);

    show_current_if_states();

    switch(iface->ipv4_cfg_state)
    {
    case IPV4_CONFIGURING:
        if(boot_notify_rcode == TM_ENOERROR)
        {
            ASSERT( (iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_DHCP)
                    || (iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_BOOTP) );
            net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_COMPLETE, iface, (void *)0, (void *)0);
        }
        else if(boot_notify_rcode == TM_ETIMEDOUT)
        {
            if(iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_BOOTP)
            {
                DBG_MSG("IPv4 BOOTP config timed out on %s\n", iface->name);
            }
            else
            {
                DBG_MSG("IPv4 DHCP config timed out on %s\n", iface->name);
            }
            iface->ipv4_cfg_status |= IPV4_AUTOCONFIG_TIMEOUT;
            goto error;
        }
        else if(boot_notify_rcode == TM_EPERM &&
                iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_DHCP)
        {
            // assume initial DHCP request was NAK'ed - rebind DHCP
            DBG_MSG("IPv4 initial DHCP request was NAK'ed on %s\n", iface->name);
            iface->ipv4_cfg_status |= IPV4_DHCP_NAK;
            net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);
            break;
        }
        else if(boot_notify_rcode == TM_EADDRINUSE &&
                iface->ipv4_pending_config == NET_IFACE_IPV4_CONFIGURED_BOOTP)
        {
            // collision detected during initial BOOTP probing (i.e. the addr is owned by another NIC)
            // NOTE: DHCP will automatically Decline a duplicate address and retry config
            DBG_MSG("IPv4 BOOTP config failed on %s. A duplicate address is already in use.\n", iface->name);
            iface->ipv4_cfg_status |= IPV4_DUPLICATE_ADDR;
            net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);
            break;
        }
        else
        {
            DBG_ERR("unhandled boot_notify_rcode %d on %s\n", boot_notify_rcode, iface->name);
            iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
            goto error;
        }
        break;

    case IPV4_CONFIGURED:
        {
            net_iface_ipv4_config_method_t config_method = NET_IFACE_IPV4_CONFIGURED_DHCP;
            net_iface_get_var_ipv4_last_config_method(iface->treck_idx, &config_method);
            if(boot_notify_rcode == TM_ENOERROR && config_method == NET_IFACE_IPV4_CONFIGURED_DHCP)
            {
                // assume notification is for a successful DHCP renewal
                ttUserBtEntryPtr boot_entry;
                boot_entry = tfDhcpUserGetBootEntry(iface->treck_interface, multihome_idx);
#ifndef __linux__
                if(!boot_entry)
                {
                    DBG_ERR("tfDhcpUserGetBootEntry failed on %s (renew)\n", iface->name);
                    iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
                    goto error;
                }
#endif

                /*  Track the DHCP lease time remaining.  Note that btuDhcpLeaseTime is the total lease time (in
                 *  milliseconds) remaining at the time the server response was processed by Treck, i.e. Treck
                 *  has already adjusted for the time elapsed since the DHCP request was sent.
                 * 
                 *  We must save the current value of btuDhcpLeaseTime because it will be updated later by Treck
                 *  after the DHCP T1 timer expires (i.e. when Treck's DHCP client enters the RENEWING state).
                 */
                extern tt32Bit tvTime;
                iface->ipv4_dhcp_lease_start = tvTime; // internal Treck system time (milliseconds)
                iface->ipv4_dhcp_lease_time = boot_entry->btuDhcpLeaseTime;

                // check if gateway has changed
                uint32_t ipv4_gateway = 0;
#ifdef TM_USE_STRONG_ESL
                int treck_rcode = tfGetDefaultGateway(iface->treck_interface, (ttUserIpAddress *)&ipv4_gateway);
#else
                int treck_rcode = tfGetDefaultGateway((ttUserIpAddress *)&ipv4_gateway);
#endif
#ifndef __linux__
                if((treck_rcode == TM_ENOERROR || treck_rcode == TM_ENOENT) && ipv4_gateway != boot_entry->btuDefRouter)
#else
                if(ipv4_gateway != boot_entry->btuDefRouter)
#endif
                {
                    // store the new gateway addr (use internal routine to prevent trigger of net reset)
                    net_set_ipv4_manual_gateway(iface->treck_idx, &boot_entry->btuDefRouter);

                    // update routing table
                    if(ipv4_gateway)
                    {
                        // remove the old entry
#ifdef TM_USE_STRONG_ESL
                        treck_rcode = tfDelDefaultGateway(iface->treck_interface, ipv4_gateway);
#else
                        treck_rcode = tfDelDefaultGateway(ipv4_gateway);
#endif
                        XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                    }
                    if(boot_entry->btuDefRouter)
                    {
                        // add the new entry
#ifdef TM_USE_STRONG_ESL
                        treck_rcode = tfAddDefaultGateway(iface->treck_interface, boot_entry->btuDefRouter, NET_IFACE_INTERFACE_HOPS);
#else
                        treck_rcode = tfAddDefaultGateway(iface->treck_interface, boot_entry->btuDefRouter);
#endif
                        XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
                    }
                }
            }
            else if(boot_notify_rcode == TM_EPERM && config_method == NET_IFACE_IPV4_CONFIGURED_DHCP)
            {
                // assume DHCP renewal was NAK'ed - rebind DHCP
                DBG_MSG("IPv4 DHCP renew was NAK'ed on %s\n", iface->name);
                iface->ipv4_cfg_status |= IPV4_DHCP_NAK;
                net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);
                break;
            }
            else if(boot_notify_rcode == TM_ETIMEDOUT && config_method == NET_IFACE_IPV4_CONFIGURED_DHCP)
            {
                // assume DHCP renewal timed out and lease has expired - rebind DHCP
                DBG_MSG("IPv4 DHCP lease has expired on %s (renew failed).  Rebinding...\n", iface->name);
                iface->ipv4_cfg_status |= IPV4_DHCP_LEASE_EXPIRED;
                net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);
                break;
            }
            else // assume internal system error
            {
                DBG_ERR("unhandled boot_notify_rcode %d on %s\n", boot_notify_rcode, iface->name);
                iface->ipv4_cfg_status |= IPV4_INTERNAL_ERROR;
                goto error;
            }
        }
        break;

    default:
        // we should not encounter any other IPv4 config states
        XASSERT(0, iface->ipv4_cfg_state);
        break;
    }
    return;

error:
    // DHCP/BOOTP config failed - retry with another config type
    net_send_msg(NET_IFACE_MSG_IPV4_CONFIG_RETRY, iface, (void *)0, (void *)0);
    return;
}


STATIC void ipv4_cancel_collision_detection(net_interface_t *iface)
{
#ifdef __linux__
    show_current_if_states();
#else
    int treck_rcode;
    NET_LOCK_IFACE_CONTEXT(iface);
    if(iface->ipv4_coll_detect_addr)
    {
        uint32_t tmp_addr = iface->ipv4_coll_detect_addr;
        iface->ipv4_coll_detect_addr = 0;
        NET_UNLOCK_IFACE_CONTEXT(iface);
#ifdef TM_USE_STRONG_ESL
        treck_rcode = tfCancelCollisionDetection(iface->treck_interface, tmp_addr);
#else
        treck_rcode = tfCancelCollisionDetection(tmp_addr);
#endif
        XASSERT(treck_rcode == TM_ENOERROR, treck_rcode);
    }
    else
    {
        NET_UNLOCK_IFACE_CONTEXT(iface);
    }
#endif // ! __linux__
    return;
}

/*
 *  ipv4_add_autoip_static_route()
 * 
 *  Support sending from routable source address to link-local dest address (i.e. auto-IP) in mixed
 *  networks. Create a static route for the auto-IP network address 169.254/16 using the local
 *  interface IP addr as the gateway. Doing so routes auto-IP packets onto local network, which
 *  prevents Bonjour Conformance Test ver. 1.2.3 from failing in Phase III: "Mixed Network
 *  interoperability" at step III.2 "Routable to Link-Local Communication" - bug 11312
 */
STATIC void ipv4_add_autoip_static_route(net_interface_t *iface, uint32_t ipv4_local_addr)
{
    show_current_if_states();
    int treck_rcode;

    treck_rcode = tfAddStaticRoute(iface->treck_interface,
                                   (tt4IpAddress)tm_const_htonl(TM_UL(0xA9FE0000)), // 169.254/16
                                   (tt4IpAddress)tm_const_htonl(TM_UL(0xFFFF0000)), // 255.255.0.0
                                   ipv4_local_addr, // use local IP addr as gateway since this is a local route
                                   /* hops */ 1);
#if 0
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfAddStaticRoute failed on %s (addr=%08x, error=%d)\n", iface->name, ipv4_local_addr, treck_rcode);
    }
#endif
    return;
}

#ifdef HAVE_IPV6
STATIC void ipv6_open_interface(net_interface_t *iface)
{
    DBG_VERBOSE("==>%s\n", __func__);
    bool use_dhcp;
#ifndef __linux__
    int ipv6_open_flags = 0;
#endif
    net_iface_rcode_t net_iface_rcode;

    show_current_if_states();

    // transition to CONFIGURING state
    adv_v6_state(iface, IPV6_CONFIGURING);
    iface->ipv6_cfg_status = 0;
    iface->ipv6_dhcp_started = false;
    DBG_VERBOSE("starting IPv6 config on %s\n", iface->name);

    net_iface_ipv6_config_options_t cfg_flags = 0;
    net_iface_get_var_ipv6_config_options(iface->treck_idx, &cfg_flags);
    use_dhcp = (cfg_flags & NET_IFACE_IPV6_USE_DHCPV6) != 0;

    if (iface->treck_link_protocol == LINK_PROTOCOL_PPPOE)
    {
        use_dhcp = false;
    }

    int treck_rcode;
    struct sockaddr_storage bin_v6_addr = { AF_INET6, 0, {sizeof(struct sockaddr_in6)} }; // revisit
#ifdef __linux__
    // implicitly configured already
    char* str_v6_addr = get_intf_ipv6_addr(iface->name);
    if (str_v6_addr) // revisit unbound address
    {
        inet_pton(AF_INET6, str_v6_addr, &bin_v6_addr);
    }
    ipv6_addr_notify_func(iface, 0 /*multi-home*/, &bin_v6_addr, TM_6_DEV_ADDR_CONFIG_STARTED);
    treck_rcode = TM_EINPROGRESS;
#else
    int addr_prefix = 0;
    int open_flags = TM_DEV_MCAST_ENB;

    // Construct the manual IPv6 sockaddr for open request. Use a zeroed addr (IPv6 unspecified address) for none.
    // NOTE: saddr is used in the call to tfNgOpenInterface() regardless of setting for NET_IFACE_IPV6_USE_MANUAL
    struct sockaddr_storage saddr;
    net_iface_ipv6_addr_t ipv6_addr;
    net_iface_get_var_ipv6_manual_addr(iface->treck_idx, &ipv6_addr);
    tfMemSet(&saddr, 0, sizeof(saddr)); // defaults addr to IPv6 unspecified address
    saddr.ss_family = AF_INET6;
    saddr.ss_len = sizeof(struct sockaddr_in6);
    if(cfg_flags & NET_IFACE_IPV6_USE_MANUAL)
    {
        memcpy(&saddr.addr.ipv6.sin6_addr.ip6Addr, &ipv6_addr.addr, sizeof(saddr.addr.ipv6.sin6_addr.ip6Addr));
        addr_prefix = ipv6_addr.prefix;
    }
#endif // ! __linux__

    if(use_dhcp)
    {
//        ipv6_open_flags |= TM_6_DEV_USER_DHCP;
#ifndef __linux__
        ipv6_open_flags |= TM_6_DEV_USER_DHCP_RTR_CTRLD; // process M & O flags from RA packets (bug 15322)
#endif

        net_iface_rcode = ipv6_configure_dhcp(iface);
        if(NET_IFACE_OK != net_iface_rcode)
        {
            goto error;
        }
    }

#ifdef __linux__
    ipv6_addr_notify_func(iface, 0 /*multi-home*/, &bin_v6_addr, TM_6_DEV_ADDR_CONFIG_COMPLETE);
#else
    // Set IP6 fragmentation TTL to 60. Treck default is 64 which causes an IPv6 certification test to fail.
    treck_rcode = tfSetTreckOptions(TM_6_OPTION_IP_FRAG_TTL, 60);
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("ipv6 tfSetTreckOptions error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        goto error;
    }

    treck_rcode = tfNgOpenInterface(iface->treck_interface,
                                    &saddr,
                                    addr_prefix,
                                    open_flags,
                                    ipv6_open_flags,
                                    NET_IFACE_TX_BUFS_PER_FRAME,
                                    ipv6_addr_notify_func);
#endif // ! __linux__

    if(treck_rcode == TM_EALREADY)
    {
        if (iface->treck_link_protocol != LINK_PROTOCOL_PPPOE)
        {
            DBG_ERR("tfNgOpenInterface is already in process (unexpected)\n");
            add_v6_state(iface, IPV6_INTERNAL_ERROR);
            goto error;
        }
    }
    else if(treck_rcode != TM_EINPROGRESS)
    {
        DBG_ERR("tfNgOpenInterface error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        goto error;
    }

    if(use_dhcp)
    {
        /* Once DHCPv6 is in progress (e.g. after a successful call to tfNgOpenInterface()),
         * tf6DhcpUserStop() must be called in order to unconfigure IPv6 on the interface.
         */
        iface->ipv6_dhcp_started = true;
    }

    DBG_VERBOSE("<==%s: success\n", __func__);
    return;

error:
    DBG_VERBOSE("<==%s: failed\n", __func__);
    net_send_msg(NET_IFACE_MSG_IPV6_CONFIG_FAILED, iface, (void *)0, (void *)0);
    return;
}

/*  rdj TODO - need to implement a backoff algorithm, e.g. after 10 successive DHCPv6 Declines due
 *  to DAD failure, only retry Solicit at 60 second intervals.  Otherwise, DAD can potentially
 *  fail continuously and inundate the server with DHCPv6 Declines and Solicits. For
 *  example, if printer has a reservation with DHCPv6 server then we will repeatedly get the same
 *  IPv6 addr, and DAD will continue to fail until the conflict is resolved.
 */
STATIC void ipv6_reconfigure_dhcp(net_interface_t *iface)
{
    int treck_rcode;
    struct sockaddr_storage bin_v6_addr = { AF_INET6, 0, {sizeof(struct sockaddr_in6)} }; // revisit

    // must reset to false even if NET_IFACE_IPV6_USE_DHCPV6 is no longer set
    iface->ipv6_dhcp_started = false;

#ifdef __linux__
    show_current_if_states();
    treck_rcode = TM_ENOERROR; // revisit unconditional
    ipv6_addr_notify_func(iface, 0 /*multi-home*/, &bin_v6_addr, TM_6_DEV_ADDR_CONFIG_COMPLETE);
#else
    int ipv6_open_flags = 0;
    int open_flags = TM_DEV_MCAST_ENB;
    int addr_prefix = 0;
    int32_t net_iface_rcode;

    net_iface_ipv6_config_options_t cfg_flags;
    net_iface_get_var_ipv6_config_options(iface->treck_idx, &cfg_flags);
    if(!(cfg_flags & NET_IFACE_IPV6_USE_DHCPV6))
    {
        return;
    }

    // clear all out any previous DHCPv6 addrs from the global addr list
    // NOTE: prior DHCPv6 addrs will remain in list even if they failed DAD
    NET_LOCK_IFACE_CONTEXT(iface);
    int i;
    for(i=IPV6_STATEFUL_MULTIHOME_IDX_FIRST; i<=IPV6_STATEFUL_MULTIHOME_IDX_LAST; i++)
    {
        if(iface->addr_map[i])
        {
            XASSERT(iface->addr_map[i]->config_method == NET_IFACE_IPV6_CONFIGURED_DHCPV6,
                    iface->addr_map[i]->config_method);
            ipv6_global_addr_remove(iface, i);
            MEM_FREE_AND_NULL(iface->addr_map[i]);
        }
    }
    NET_UNLOCK_IFACE_CONTEXT(iface);

    struct sockaddr_storage saddr;
    net_iface_ipv6_addr_t ipv6_addr;
    net_iface_get_var_ipv6_manual_addr(iface->treck_idx, &ipv6_addr);
    tfMemSet(&saddr, 0, sizeof(saddr)); // defaults addr to IPv6 unspecified address
    saddr.ss_family = AF_INET6;
    saddr.ss_len = sizeof(struct sockaddr_in6);

//    ipv6_open_flags |= TM_6_DEV_USER_DHCP;
    ipv6_open_flags |= TM_6_DEV_USER_DHCP_RTR_CTRLD; // process M & O flags from RA packets (bug 15322)

    net_iface_rcode = ipv6_configure_dhcp(iface);
    if(NET_IFACE_OK != net_iface_rcode)
    {
        goto error;
    }

#ifdef HAVE_HOTSPOT_SUPPORT
    if (wifi_hotspot_allowed_by_platform())
    {
        ipv6_open_flags |= NETDRVR_IPV6_HOTSPOT_DEVICE_OPTIONS;
    }
#endif // HAVE_HOTSPOT_SUPPORT

    treck_rcode = tfNgConfigInterface(iface->treck_interface,
                                      &saddr,
                                      addr_prefix,
                                      open_flags,
                                      ipv6_open_flags,
                                      NET_IFACE_TX_BUFS_PER_FRAME,
                                      ipv6_addr_notify_func,
                                      0 /* multi-home index */);
#endif // ! __linux__

    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tfNgConfigInterface error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        goto error;
    }

    iface->ipv6_dhcp_started = true;

    return;

error:
    net_send_msg(NET_IFACE_MSG_IPV6_CONFIG_FAILED, iface, (void *)0, (void *)0);
    return;
}

STATIC int32_t ipv6_configure_dhcp(net_interface_t *iface)
{
    show_current_if_states();
    int treck_rcode;

    // allocate memory and initialize the DHCPv6 client
    treck_rcode = tf6UseDhcp(iface->treck_interface,
                           ipv6_dhcp_notify_func,
                           NET_IFACE_IPV6_DHCP_PRIVATE_ENTERPRISE_NUM_HP,
                           NULL, // DUID-EN ptr (NULL => auto-generate DUID-LL)
                           0, // DUID-EN ptr len (NULL =>auto-generate DUID-LL)
                           1, // accept Reconfigure messages
                           0); // flags (currently unused by Treck)
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tf6UseDhcp error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        return NET_IFACE_FAIL;
    }

    // set up FQDN option for DHCPv6 server
    char hostname[NET_IFACE_HOSTNAME_SIZE] = {0};
    net_iface_get_var_hostname(iface->treck_idx, hostname, NET_IFACE_HOSTNAME_SIZE);
    if(hostname[0])
    {
        /*  NOTE: by default, TM_6_DEV_OPTIONS_DHCP_FQDN_S_BIT is already set (instructs server to
         *  perform AAAA RR DNS updates) and TM_6_DEV_OPTIONS_DHCP_FQDN_N_BIT is not set (when set,
         *  instructs server NOT to perform any DNS updates, and when cleared instructs the server to ALWAYS
         *  handle the PTR RR and possibly the AAAA RR depending on the status of the S bit)
         */
        treck_rcode = tfInterfaceSetOptions(iface->treck_interface,
                                            TM_6_DEV_OPTIONS_DHCP_FQDN_PART, hostname, strlen(hostname));
        if(treck_rcode != TM_ENOERROR)
        {
            DBG_ERR("tfInterfaceSetOptions error on %s (line %d, treck_rcode=%d)\n", iface->name, __LINE__, treck_rcode);
            add_v6_state(iface, IPV6_INTERNAL_ERROR);
            return NET_IFACE_FAIL;
        }
    }

    uint32_t dhcpv6_option = NET_IFACE_IPV6_DHCP_RELEASE_MRD_SEC;
    treck_rcode = tf6DhcpSetOption(TM_6_DHCP_GOPT_RELEASE_MRD,
                                   (ttUserVoidPtr)&dhcpv6_option, sizeof(dhcpv6_option));
    UNUSED_VAR( dhcpv6_option ); // BAD interface design caught by compiler :)
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tf6DhcpSetOption error on %s (treck_rcode=%d) %x\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        goto error;
    }

    // Set user-preferred configuration options, including the time for renewal and rebinding.
    // NOTE: The DHCPv6 server is not required to honor these requests
    treck_rcode = tf6DhcpSetIaOption(iface->treck_interface,
                                     0, // Identity Association (Treck currently supports only one per iface, index=0)
                                     NET_IFACE_IPV6_DHCP_DESIRED_T1_SEC,
                                     NET_IFACE_IPV6_DHCP_DESIRED_T2_SEC,
                                     0); // flags (currently unused by Treck)
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tf6DhcpSetIaOption error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        goto error;
    }

    struct in6_addr ipv6Addr;
    memset(&ipv6Addr, 0, sizeof(ipv6Addr)); // use IPv6 Unspecified address since we are only requesting lifetimes
    treck_rcode = tf6DhcpSetAddrOption(iface->treck_interface,
                                       0, // Identity Association (Treck currently supports only one per iface, index=0)
                                       0, // 0-based index of the DHCPv6 address for which to set the options
                                       &ipv6Addr,
                                       NET_IFACE_IPV6_DHCP_DESIRED_PREF_LIFETIME,
                                       NET_IFACE_IPV6_DHCP_DESIRED_VALID_LIFETIME,
                                       0); // flags (currently unused by Treck)
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tf6DhcpSetAddrOption error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        goto error;
    }

// Original iptask.c code is below
// rdj TODO - figure out if we need to save DHCPv6 addrs in NVRAM and request again after
// next boot (Treck supports up to 4 DHCPv6 addrs per IA)
       // Set user-preferred addresses as well as the preferred and valid lifetimes of the address.
       // The DHCPv6 server is not required to honor these requests.
//     tm_memset(&ip6Addr, 0, sizeof(ip6Addr));
//     errorCode = tf6DhcpSetAddrOption(gInterfaceHandle, 0, 0, &ip6Addr, 2000, 3000, 0);

    return NET_IFACE_OK;

error:
    return NET_IFACE_FAIL;
}

/*
 *  ipv6_close_interface()
 * 
 *  Disables the IPv6 portion of an interface and performs cleanup of internal netdrvr IPv6 addr
 *  context.
 * 
 *  NOTE: tfCloseInterface will not trigger the call to link layer net_iface_close routine until
 *  DHCPv6 is stopped, so when DHCPv6 is running we wait for the TM_6_DHCP_EVENT_STOPPED event
 *  before calling tfCloseInterface and unbinding the underlying link interface.
 * 
 */
STATIC int32_t ipv6_close_interface(net_interface_t *iface)
{
    int32_t net_iface_rcode = NET_IFACE_OK;
    int treck_rcode;
    show_current_if_states();

    // must change IPv6 config state *before* interface is closed (interface is guaranteed to be up while
    // in state IPV6_CONFIGURED)
    ipv6_cfg_state_t prev_ipv6_cfg_state = iface->ipv6_cfg_state;
    adv_v6_state(iface, IPV6_CLOSING);

    if(prev_ipv6_cfg_state >= IPV6_LOCAL_CONFIGURED)
    {
        // must notify registered callbacks *after* changing state but *before* closing the interface
        event_list_notify(iface, NET_IFACE_EVENT_IPV6_LOCAL_CONFIG_CHANGE);
    }
    if(prev_ipv6_cfg_state == IPV6_CONFIGURED)
    {
        // must notify registered callbacks *after* changing state but *before* closing the interface
        event_list_notify(iface, NET_IFACE_EVENT_IPV6_CONFIG_CHANGE);
    }

    /* The ipv6_dhcp_started flag is necessary because tf6DhcpUserStop() can return TM_ENOERROR even if
     * DHCPv6 is not currently running, in which case we would get stuck waiting for a
     * TM_6_DHCP_EVENT_STOPPED event that would never come.
     */
    if(iface->ipv6_dhcp_started)
    {
        DBG_MSG("waiting for DHCPv6 client to stop on %s...\n", iface->name);
        treck_rcode = tf6DhcpUserStop(iface->treck_interface,
                                      0); // flags (currently unused by Treck)
        if(treck_rcode != TM_ENOERROR)
        {
            // unexpected error
            DBG_ERR("tf6DhcpUserStop error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
            net_iface_rcode = NET_IFACE_FAIL;
            DBG_ASSERT(0);
            goto done;
        }

        // must wait for TM_6_DHCP_EVENT_STOPPED event before closing the interface and unbinding the link
        net_iface_rcode = NET_IFACE_INPROGRESS;
    }

#ifdef __linux__
// close v6 interface regardless
#else
    if(net_iface_rcode != NET_IFACE_INPROGRESS)
#endif
    {
        ipv6_finish_close_interface(iface);
    }

done:
    return net_iface_rcode;
}

STATIC void ipv6_finish_close_interface(net_interface_t *iface)
{
    show_current_if_states();
    // IPv6 state must be set to a value less than IPV6_CONFIGURING *before* calling this routine
    ASSERT(iface->ipv6_cfg_state < IPV6_CONFIGURING);

    // first disable the IPv6 interface by unconfiguring the link-local IPv6 addr
    ipv6_unconfigure_linklocal(iface, IPV6_LINKLOCAL_MULTIHOME_IDX);// unconfigure the stateless link-local addr
    ipv6_unconfigure_linklocal(iface, IPV6_MANUAL_MULTIHOME_IDX);// unconfigure any manual link-local addr

    NET_LOCK_IFACE_CONTEXT(iface);

    // cancel any pending timers
    net_timer_cancel(iface->timer_hndl_v6autoconf);
    iface->timer_hndl_v6autoconf = NET_TIMER_INVALID_HANDLE;

    // cleanup context for all internal netdrvr IPv6 addrs
    int i;
    for(i=0; i<IPV6_MAX_ADDRS; i++)
    {
        if(iface->addr_map[i])
        {
            // not all addrs are in configured global IPv6 addr list - if not in list then this does nothing
            ipv6_global_addr_remove(iface, i);
            MEM_FREE_AND_NULL(iface->addr_map[i]);
        }
    }
    ASSERT(!iface->ipv6_preferred_addr); // the list of configured global IPv6 addrs should now be empty

    // IPv6 close complete - overall interface remains open
    iface->ipv6_dhcp_started = false;
    adv_v6_state(iface, IPV6_NOT_CONFIGURED);
    DBG_MSG("IPv6 is down on %s\n", iface->name);

#ifdef __linux__ // request close_finish
    net_send_msg(NET_IFACE_MSG_IPV6_CLOSE_FINISH, iface, (void *)0, (void *)0);
#endif

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;
}

/*
 *  ipv6_addr_notify_func()
 * 
 *  WARNING: Do not call user callbacks from this callback routine. It could have been called from
 *  the network timer thread, which expects low-latency iterations of a timer loop and also could be
 *  running at an elevated priority.
 */
#ifdef __linux__
STATIC void ipv6_addr_notify_func(net_interface_t* iface, // caller-provided iface, no lookup required
                                  unsigned int multihome_idx,
                                  struct sockaddr_storage *saddr,
                                  int event)
#else
STATIC void ipv6_addr_notify_func(ttUserInterface treck_interface,
                                  unsigned int multihome_idx,
                                  struct sockaddr_storage *saddr,
                                  int event)
#endif
{
#ifdef __linux__
    ttUserInterface treck_interface = 0; UNUSED_VAR(treck_interface);
#else
    net_interface_t *iface = net_iface_recover_local_context(treck_interface);
#endif
    ASSERT(iface);

    show_current_if_states();

    NET_LOCK_IFACE_CONTEXT(iface);

    switch(event)
    {
    case TM_6_DEV_ADDR_CONFIG_STARTED:
        /*  Configuration of the IPv6 address at the specified multi-home index has been started (i.e. the
         *  address is undergoing Duplicate Address Detection).
         */
        DBG_VERBOSE("ipv6_addr_notify_func got event TM_6_DEV_ADDR_CONFIG_STARTED on %s\n", iface->name);
        {
            if(iface->ipv6_cfg_state < IPV6_CONFIGURING)
            {
                // IPv6 interface is disabled or in the process of being disabled -- ignore the event
                DBG_VERBOSE("interface %s closing - discarding the event\n", iface->name);
                break;
            }

            internal_ipv6_addr_t *internal_addr = (internal_ipv6_addr_t *)MEM_MALLOC(sizeof(internal_ipv6_addr_t));
            if(!internal_addr)
            {
                DBG_ERR("error allocating mem for TM_6_DEV_ADDR_CONFIG_STARTED on %s\n", iface->name);
                add_v6_state(iface, IPV6_INTERNAL_ERROR);
                goto error;
            }
            XASSERT(!iface->addr_map[multihome_idx], multihome_idx);
            iface->addr_map[multihome_idx] = internal_addr;

            internal_addr->signature = NET_IFACE_IPV6_INTERNAL_ADDR_SIGNATURE;
            internal_addr->next = NULL;
            internal_addr->config_method = ipv6_index_to_method(multihome_idx);
            internal_addr->sys_time_acquired = 0;
            internal_addr->preferred_lifetime = 0;
            internal_addr->valid_lifetime = 0;
            internal_addr->multihome_idx = multihome_idx;
            internal_addr->iface = iface;
            internal_addr->ipv6_addr.prefix = 0;
            internal_addr->state = NET_IFACE_IPV6_CONFIG_STARTED;
#ifdef __linux__
            memcpy(&internal_addr->ipv6_addr.addr, &(saddr->__ss_padding) /*ipv6_addr_offset(saddr)*/,
                   sizeof(internal_addr->ipv6_addr.addr));
#else
            memcpy(&internal_addr->ipv6_addr.addr, &saddr->addr.ipv6.sin6_addr.ip6Addr,
                   sizeof(internal_addr->ipv6_addr.addr));
#endif
        }
        break;

    case TM_6_DEV_ADDR_CONFIG_FAILED:
    {
        /*  The IPv6 address at the specified multi-home index was not configured on the interface because
         *  it failed Duplicate Address Detection.
         */
        DBG_VERBOSE("ipv6_addr_notify_func got event TM_6_DEV_ADDR_CONFIG_FAILED on %s\n", iface->name);
        {
            internal_ipv6_addr_t *internal_addr;

            if(iface->ipv6_cfg_state < IPV6_CONFIGURING)
            {
                // IPv6 interface is disabled or in the process of being disabled -- ignore the event
                DBG_VERBOSE("interface %s closing - discarding the event\n", iface->name);
                break;
            }

            ASSERT(iface->addr_map[multihome_idx]); // should be set by TM_6_DEV_ADDR_CONFIG_STARTED
            internal_addr = iface->addr_map[multihome_idx];
            ASSERT(internal_addr->state == NET_IFACE_IPV6_CONFIG_STARTED);

            // if this is the link-local addr, then IPv6 config has failed
            // NOTE: we use ip address vs. config_method since manual addr can be link local, which is in 
            //    a different mhome index range than stateless link local (i.e. auto-configured).
#ifdef __linux__
//tbd
#else
            if(IN6_IS_ADDR_LINKLOCAL(&saddr->addr.ipv6.sin6_addr))
#endif
            {
                // if link-local addr config fails then Treck cannot configure IPv6 on the interface
                iface->ipv6_cfg_status |= IPV6_DUPLICATE_ADDR;
                goto error;
            }
#ifndef __linux__
            // rdj TODO - we are assuming the addr stays in Trecks multihome array.  Need to verify this.
            internal_addr->state = NET_IFACE_IPV6_CONFIG_FAILED_DAD;

            char addr_str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (void *)(&internal_addr->ipv6_addr.addr), addr_str, INET6_ADDRSTRLEN);
            DBG_MSG("DAD failed for IPv6 addr %s on %s\n", addr_str, iface->name);
#endif
        }
    } break;

    case TM_6_DEV_ADDR_CONFIG_COMPLETE:
    {
        /* The IPv6 address at the specified multi-home index has been configured on the interface.
         */
        DBG_VERBOSE("ipv6_addr_notify_func got event TM_6_DEV_ADDR_CONFIG_COMPLETE on %s\n", iface->name);
        {
            // check for unexpected addr types -- we make assumptions that such addrs will not be encountered
            ASSERT(!IN6_IS_ADDR_LOOPBACK(ipv6_addr_field(saddr)) &&
                   !IN6_IS_ADDR_V4MAPPED(ipv6_addr_field(saddr)) &&
                   !IN6_IS_ADDR_MULTICAST(ipv6_addr_field(saddr)));

            if(iface->ipv6_cfg_state < IPV6_CONFIGURING)
            {
                // IPv6 interface is disabled or in the process of being disabled. If addr is link-local
                // then unconfigure the addr (unconfiguring all link-local addrs on a Treck interface disables
                // IPv6 on that interface).
                ipv6_unconfigure_linklocal(iface, multihome_idx);
                DBG_VERBOSE("interface %s closing - discarding the event\n", iface->name);
                break;
            }

            int treck_rcode;
            internal_ipv6_addr_t *internal_addr;

            ASSERT(iface->addr_map[multihome_idx]); // should be set by TM_6_DEV_ADDR_CONFIG_STARTED
            internal_addr = iface->addr_map[multihome_idx];
            ASSERT(internal_addr->state == NET_IFACE_IPV6_CONFIG_STARTED);

            treck_rcode = tfNgGetPrefixLen(treck_interface, &internal_addr->ipv6_addr.prefix, AF_INET6, multihome_idx);
            if(treck_rcode != TM_ENOERROR)
            {
                DBG_ERR("tfNgGetPrefixLen error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
                add_v6_state(iface, IPV6_INTERNAL_ERROR);
                goto error;
            }

            // must be called *after* internal addr prefix has been populated
            ipv6_set_addr_lifetimes(iface, internal_addr);

            internal_addr->state = NET_IFACE_IPV6_CONFIG_COMPLETE;

            char addr_str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (void *)(&internal_addr->ipv6_addr.addr), addr_str, INET6_ADDRSTRLEN);
            DBG_VERBOSE("got IPv6 addr %s on %s\n", addr_str, iface->name);

            // NOTE: we use ip address vs. config_method since manual addr can be link local, which is in 
            //    a different mhome index range than auto-configured (i.e. stateless) link local.
#ifdef __linux__
//tbd
            if (1)
#else
            if(IN6_IS_ADDR_LINKLOCAL(&saddr->addr.ipv6.sin6_addr))
#endif
            {
                if(iface->ipv6_cfg_state == IPV6_CONFIGURING)
                {
                    // Treck does not consider IPv6 operational until a link-local addr is configured on the interface 
                    NET_LOCK_IFACE_CONTEXT(iface);
                    net_timer_cancel(iface->timer_hndl_v6autoconf); // cancel outstanding timer if any
                    iface->timer_hndl_v6autoconf = net_timer_start_ms(
                                                    NET_TIMER_FLAG_REAL_TIME,
                                                    IPV6_GLOBAL_AUTOCONFIG_TIMEOUT_SEC*1000,
                                                    ipv6_global_autoconfig_timeout_func,
                                                    (void *)iface);
                    ASSERT(iface->timer_hndl_v6autoconf != NET_TIMER_INVALID_HANDLE);
                    NET_UNLOCK_IFACE_CONTEXT(iface);

                    // high priority thread, so don't block on message
                    int32_t net_iface_rcode = net_send_msg_no_wait(NET_IFACE_MSG_IPV6_LOCAL_CFG_COMPLETE, iface, (void *)0, (void *)0);
                    if(net_iface_rcode == NET_IFACE_WOULDBLOCK)
                    {
                        // msg queue full
                        DBG_MSG("tx_queue full while sending NET_IFACE_MSG_IPV6_LOCAL_CFG_COMPLETE on %s\n", iface->name);
                        ASSERT(0); // if hit this then *may* need to increase msg queue size
                    }
                }
            }
            else
                // Treat all other addr types as global.
                // NOTE: site-local addrs defined in RFC3513 have since been deprecated by RFC3879. We will
                // treat any site-local addrs as global addrs.
            {
                // clear any bits that might have been set to indicate no global IPv6 configuration.
                iface->ipv6_cfg_status &= ~(IPV6_GLOBAL_CONFIG_TIMEOUT|IPV6_GLOBAL_LOST_CONFIG);

                // add to configured global IPv6 addr list
                ipv6_global_addr_insert(iface, internal_addr);
            }
        }
    } break;

    case TM_6_DEV_ADDR_DUP_DETECTED:
    {
        /*  Detected that a different node is using an IPv6 address that we have already configured on the
         *  interface.
         */
        DBG_VERBOSE("ipv6_addr_notify_func got event TM_6_DEV_ADDR_DUP_DETECTED on %s\n", iface->name);
        {
            if(iface->ipv6_cfg_state < IPV6_CONFIGURING)
            {
                // IPv6 interface is disabled or in the process of being disabled -- ignore the event
                DBG_VERBOSE("interface %s closing - discarding the event\n", iface->name);
                break;
            }

            internal_ipv6_addr_t *internal_addr;
            internal_ipv6_addr_t *tmp_addr;

            ASSERT(iface->addr_map[multihome_idx]);
            internal_addr = iface->addr_map[multihome_idx];
            ASSERT(internal_addr->state >= NET_IFACE_IPV6_CONFIG_COMPLETE); // late collision - verify addr is configured

            internal_addr->state = NET_IFACE_IPV6_LATE_DUP_ADDR_DETECTED;

            // re-prioritize in the configured global IPv6 addr list based on new state
            tmp_addr = ipv6_global_addr_remove(iface, multihome_idx);
            ipv6_global_addr_insert(iface, internal_addr);
            ASSERT(internal_addr == tmp_addr);

            char addr_str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (void *)(&internal_addr->ipv6_addr.addr), addr_str, INET6_ADDRSTRLEN);
            DBG_MSG("duplicate detected for IPv6 addr %s on %s\n", addr_str, iface->name);

            /*  rdj TODO - should we maintain a dup count and unconfigure the addr via fNgUnConfigInterface
             *  when max count is exceeded? 
             * 
             *  Note: if the only link-local scope IPv6 address on the interface is unconfigured, the IPv6 part
             *  of the interface will be disabled (ipv6_user.pdf p50).
             */
        }
    } break;


    case TM_6_DEV_ADDR_DEPRECATED:
    {
        /*  The preferred lifetime of an IPv6 address has expired, and the address has transitioned state
         *  from preferred to deprecated.
         */
        DBG_VERBOSE("ipv6_addr_notify_func got event TM_6_DEV_ADDR_DEPRECATED on %s\n", iface->name);
        {
            if(iface->ipv6_cfg_state < IPV6_CONFIGURING) {
                // IPv6 interface is in the process of being disabled -- ignore the event
                DBG_VERBOSE("interface %s closing - discarding the event\n", iface->name);
                break;
            }

            internal_ipv6_addr_t *internal_addr;
            internal_ipv6_addr_t *tmp_addr;

            ASSERT(iface->addr_map[multihome_idx]);
            internal_addr = iface->addr_map[multihome_idx];
            ASSERT(internal_addr->state >= NET_IFACE_IPV6_CONFIG_COMPLETE); // verify addr is configured

            internal_addr->state = NET_IFACE_IPV6_ADDR_DEPRECATED;

            char addr_str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (void *)(&internal_addr->ipv6_addr.addr), addr_str, INET6_ADDRSTRLEN);
            DBG_MSG("deprecating IPv6 addr %s on %s\n", addr_str, iface->name);

            // re-prioritize in the configured global IPv6 addr list based on new state
            tmp_addr = ipv6_global_addr_remove(iface, multihome_idx);
            ipv6_global_addr_insert(iface, internal_addr);
            ASSERT(internal_addr == tmp_addr);
        }
    } break;

    case TM_6_DEV_ADDR_PREFERRED:
    {
        /*  The preferred lifetime of an IPv6 address has been extended, and the address has transitioned
         *  state from deprecated to preferred.
         */
        DBG_VERBOSE("ipv6_addr_notify_func got event TM_6_DEV_ADDR_PREFERRED on %s\n", iface->name);
        {
            if(iface->ipv6_cfg_state < IPV6_CONFIGURING)
            {
                // IPv6 interface is in the process of being disabled -- ignore the event
                DBG_VERBOSE("interface %s closing - discarding the event\n", iface->name);
                break;
            }

            internal_ipv6_addr_t *internal_addr;
            internal_ipv6_addr_t *tmp_addr;

            ASSERT(iface->addr_map[multihome_idx]);
            internal_addr = iface->addr_map[multihome_idx];
            ASSERT(internal_addr->state >= NET_IFACE_IPV6_CONFIG_COMPLETE); // verify addr is configured

            internal_addr->state = NET_IFACE_IPV6_CONFIG_COMPLETE;

            char addr_str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (void *)(&internal_addr->ipv6_addr.addr), addr_str, INET6_ADDRSTRLEN);
            DBG_MSG("restoring IPv6 addr %s on %s\n", addr_str, iface->name);

            // must assume addr lifetimes have been increased, so get the new values
            ipv6_set_addr_lifetimes(iface, internal_addr);

            // re-prioritize in the configured global IPv6 addr list based on new state
            tmp_addr = ipv6_global_addr_remove(iface, multihome_idx);
            ipv6_global_addr_insert(iface, internal_addr);
            ASSERT(internal_addr == tmp_addr);
        }
    } break;

    case TM_6_DEV_ADDR_INVALIDATED:
    {
        /*  The valid lifetime of an IPv6 address has expired and the address has been removed from the
         *  interface.
         */
        DBG_VERBOSE("ipv6_addr_notify_func got TM_6_DEV_ADDR_INVALIDATED on %s\n", iface->name);
        {
            if(iface->ipv6_cfg_state < IPV6_CONFIGURING)
            {
                // IPv6 interface is in the process of being disabled -- ignore the event
                DBG_VERBOSE("interface %s closing - discarding the event\n", iface->name);
                break;
            }

            internal_ipv6_addr_t *internal_addr;
            internal_ipv6_addr_t *tmp_addr;

            if(!iface->addr_map[multihome_idx])
            {
                // we are in the process of closing the IPv6 interface -- ignore the event
                break;
            }
            internal_addr = iface->addr_map[multihome_idx];
            //ASSERT(internal_addr->state == NET_IFACE_IPV6_ADDR_DEPRECATED); // addr must have already been deprecated

            char addr_str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (void *)(&internal_addr->ipv6_addr.addr), addr_str, INET6_ADDRSTRLEN);
            DBG_MSG("invalidating IPv6 addr %s on %s\n", addr_str, iface->name);

            // remove addr from the configured global IPv6 addr list and clean up addr context
            tmp_addr = ipv6_global_addr_remove(iface, multihome_idx);
            ASSERT(internal_addr == tmp_addr);
            iface->addr_map[multihome_idx] = NULL;
            internal_addr->signature = 0; // must erase the signature before free
            MEM_FREE_AND_NULL(internal_addr);

            if(!iface->ipv6_preferred_addr)
            {
                // set a status bit to indicate that global IPv6 configuration has been lost
                iface->ipv6_cfg_status |= IPV6_GLOBAL_LOST_CONFIG;
            }
        }
    } break;

    case TM_6_DEV_ADDR_ALL_RELEASED:
        DBG_VERBOSE("ipv6_addr_notify_func got TM_6_DEV_ADDR_ALL_RELEASED on %s\n", iface->name);
        break;

    case TM_6_DEV_ADDR_ALL_DECLINED:
        /*  All IPv6 addresses offered by a DHCPv6 server have been declined due to failed duplicate address
         *  detection, and the DHCPv6 client has been stopped
         */
        DBG_VERBOSE("ipv6_addr_notify_func got TM_6_DEV_ADDR_ALL_DECLINED on %s\n", iface->name);

        // retry DHCPv6 config
        net_send_msg(NET_IFACE_MSG_IPV6_CONFIG_RETRY, iface, (void *)0, (void *)0);
        break;

    default:
        // unexpected event
        XASSERT(0, event);
    }

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;

error:
    net_send_msg(NET_IFACE_MSG_IPV6_CONFIG_FAILED, iface, (void *)0, (void *)0);
    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;
}


STATIC void ipv6_dhcp_notify_func(ttUserInterface treck_interface,
                                  int ia_idx, // Identity Association (Treck currently supports only one per iface, index=0)
                                  int dhcp_addr_idx, // 0-based index of the DHCPv6 address on which the event occurred
                                  tt6DhcpEvent event) // DHCPv6 client event
{
    net_interface_t *iface = net_iface_recover_local_context(treck_interface);
    ASSERT(iface);

    // rdj TODO - use these events to implement 1) IPV6_GLOBAL_AUTOCONFIG_TIMEOUT 2) confirm after suspect switched network
    switch(event)
    {
    case TM_6_DHCP_EVENT_SOLICIT_TIMEOUT:
        DBG_VERBOSE("ipv6_dhcp_notify_func got TM_6_DHCP_EVENT_SOLICIT_TIMEOUT on %s\n", iface->name);
        break;
    case TM_6_DHCP_EVENT_NO_SERVER:
        DBG_VERBOSE("ipv6_dhcp_notify_func got TM_6_DHCP_EVENT_NO_SERVER on %s\n", iface->name);
        break;
    case TM_6_DHCP_EVENT_CONFIRM_FAILED:
        DBG_VERBOSE("ipv6_dhcp_notify_func got TM_6_DHCP_EVENT_CONFIRM_FAILED on %s\n", iface->name);
        break;
    case TM_6_DHCP_EVENT_CONFIRM_OK:
        DBG_VERBOSE("ipv6_dhcp_notify_func got TM_6_DHCP_EVENT_CONFIRM_OK on %s\n", iface->name);
        break;
    case TM_6_DHCP_EVENT_INFO_RECEIVED:
        DBG_VERBOSE("ipv6_dhcp_notify_func got TM_6_DHCP_EVENT_INFO_RECEIVED on %s\n", iface->name);
        break;
    case TM_6_DHCP_EVENT_LEASE_RENEWED:
        DBG_VERBOSE("ipv6_dhcp_notify_func got TM_6_DHCP_EVENT_LEASE_RENEWED on %s\n", iface->name);
        {
            tt6UserBtEntryPtr bt_entry;

            bt_entry = tf6DhcpGetBootEntry(iface->treck_interface,
                               0, // Identity Association (Treck currently supports only one per iface, index=0)
                               0); // flags (currently unused by Treck)
            if(bt_entry)
            {
                unsigned long sys_time_acquired;
                struct sockaddr_storage saddr = { AF_INET6, sizeof(struct sockaddr_in) }; // revisit assumption { ss_family; ss_len; __ss_padding };
                internal_ipv6_addr_t *internal_addr;

                sys_time_acquired = os_time_get();
                tfMemSet(&saddr, 0, sizeof(saddr));

                // update preferred & valid lease times for configured DHCPv6 addrs
                int i, j;
                for(i=IPV6_STATEFUL_MULTIHOME_IDX_FIRST; i<=IPV6_STATEFUL_MULTIHOME_IDX_LAST; i++)
                {
                    if(iface->addr_map[i])
                    {
                        XASSERT(iface->addr_map[i]->config_method == NET_IFACE_IPV6_CONFIGURED_DHCPV6,
                                iface->addr_map[i]->config_method);

                        internal_addr = iface->addr_map[i];
#ifdef __linux__
                        memcpy(ipv6_addr_offset(saddr), &internal_addr->ipv6_addr.addr, sizeof(internal_addr->ipv6_addr.addr));
#else
                        memcpy(&saddr.addr.ipv6.sin6_addr, &internal_addr->ipv6_addr.addr, sizeof(saddr.addr.ipv6.sin6_addr.ip6Addr));
#endif

                        // dhcp_addr_idx is always 0 here, so just loop through all addrs in the boot record
                        j = 0;
                        while(j < bt_entry->btAddrCnt)
                        {
#ifdef __linux__
                            if(IN6_ARE_ADDR_EQUAL(&bt_entry->btAddrList[j].addrValue, ipv6_addr_offset(saddr)))
#else
                            if(IN6_ARE_ADDR_EQUAL(&bt_entry->btAddrList[j].addrValue, &saddr.addr.ipv6.sin6_addr))
#endif
                            {
                                internal_addr->sys_time_acquired = sys_time_acquired;
                                internal_addr->preferred_lifetime = bt_entry->btAddrList[j].addrPrefLifeTime;
                                internal_addr->valid_lifetime = bt_entry->btAddrList[j].addrValidLifeTime;
                                break;
                            }
                            j++;
                        }
                    }
                }
                tf6DhcpFreeBootEntry(bt_entry, 0 /* flags (currently unused by Treck) */);
            }
        }
        break;


    case TM_6_DHCP_EVENT_STOPPED:
        /* The DHCPv6 client has been stopped and we may finish closing the interface
         */
        DBG_VERBOSE("ipv6_dhcp_notify_func got TM_6_DHCP_EVENT_STOPPED on %s\n", iface->name);
        net_send_msg(NET_IFACE_MSG_IPV6_CLOSE_FINISH, iface, (void *)0, (void *)0);
        break;

    default:
        // unexpected event
        XASSERT(0, event);
        break;
    }

    return;
}

STATIC void ipv6_global_autoconfig_timeout_func(void *user_data)
{
    ASSERT(user_data);
    net_interface_t *iface = (net_interface_t *)user_data;

    // assume ThreadX timer callback (i.e. a non-thread)
    int32_t net_iface_rcode;
    net_iface_rcode = net_send_msg_no_wait(NET_IFACE_MSG_IPV6_GLOBAL_CFG_COMPLETE,
                                           iface, (void *)0, (void *)0);
    if(net_iface_rcode == NET_IFACE_WOULDBLOCK)
    {
        // msg queue full
        DBG_MSG("tx_queue full while sending NET_IFACE_MSG_IPV6_UPDATE_GLOBAL_CFG_STATUS on %s\n",
                 iface->name);
        ASSERT(0); // if hit this then *may* need to increase msg queue size
    }
    return;
}

STATIC void ipv6_local_config_complete(net_interface_t *iface)
{
    NET_LOCK_IFACE_CONTEXT(iface);
    // set interface as default when sending outgoing multicast IPv6 packets
    int treck_rcode = tf6SetMcastInterface(iface->treck_interface);
    if(treck_rcode != TM_ENOERROR)
    {
        DBG_ERR("tf6SetMcastInterface error on %s (error=%d)\n", iface->name, treck_rcode);
        add_v6_state(iface, IPV6_INTERNAL_ERROR);
        goto error;
    }

    DBG_MSG("IPv6 local is up on %s\n", iface->name);
    adv_v6_state(iface, IPV6_LOCAL_CONFIGURED);
    NET_UNLOCK_IFACE_CONTEXT(iface);

    event_list_notify(iface, NET_IFACE_EVENT_IPV6_LOCAL_CONFIG_CHANGE);

    return;

error:
    net_send_msg(NET_IFACE_MSG_IPV6_CONFIG_FAILED, iface, (void *)0, (void *)0);
    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;
}

STATIC void ipv6_global_config_complete(net_interface_t *iface)
{
    ASSERT(iface->ipv6_cfg_state == IPV6_LOCAL_CONFIGURED);

    NET_LOCK_IFACE_CONTEXT(iface);
    adv_v6_state(iface, IPV6_CONFIGURED);
    if(!iface->ipv6_preferred_addr)
    {
        // no global IPv6 addresses are configured on the interface
        DBG_MSG("IPv6 global config timed out on %s\n", iface->name);
        iface->ipv6_cfg_status |= IPV6_GLOBAL_CONFIG_TIMEOUT;
    }
    else
    {
        // at least one global IPv6 address has passed DAD and is configured on the interface
        DBG_MSG("IPv6 global is up on %s\n", iface->name);
    }
    NET_UNLOCK_IFACE_CONTEXT(iface);

    // overall IPv6 protocol is now considered configured on the network interface
    event_list_notify(iface, NET_IFACE_EVENT_IPV6_CONFIG_CHANGE);
    return;
}


STATIC void ipv6_set_addr_lifetimes(net_interface_t *iface, internal_ipv6_addr_t *internal_addr)
{
    int treck_rcode;
    struct sockaddr_storage saddr;

    ASSERT(internal_addr);
//    ASSERT(internal_addr->state == NET_IFACE_IPV6_CONFIG_STARTED);

    internal_addr->sys_time_acquired = os_time_get();

    tfMemSet(&saddr, 0, sizeof(saddr));
#ifdef __linux__
//tbd
#else
    saddr.ss_family = AF_INET6;
    saddr.ss_len = sizeof(struct sockaddr_in6);
    memcpy(&saddr.addr.ipv6.sin6_addr, &internal_addr->ipv6_addr.addr, sizeof(saddr.addr.ipv6.sin6_addr.ip6Addr));
#endif

    switch(internal_addr->config_method)
    {
    case NET_IFACE_IPV6_CONFIGURED_MANUAL:
    case NET_IFACE_IPV6_CONFIGURED_LINKLOCAL:
        internal_addr->preferred_lifetime = (uint32_t)-1; // infinite
        internal_addr->valid_lifetime = (uint32_t)-1; // infinite
        break;

    case NET_IFACE_IPV6_CONFIGURED_STATELESS:
        {
            treck_rcode = tf6GetAddrLifetimes(iface->treck_interface, &saddr, internal_addr->ipv6_addr.prefix,
                                              &internal_addr->valid_lifetime, &internal_addr->preferred_lifetime);
            if(treck_rcode != TM_ENOERROR)
            {
                DBG_ERR("tf6GetAddrLifetimes error on %s (treck_rcode=%d)\n", iface->name, treck_rcode);
                goto error;
            }
        }
        break;


    case NET_IFACE_IPV6_CONFIGURED_DHCPV6:
        {
            tt6UserBtEntryPtr bt_entry;
            bt_entry = tf6DhcpGetBootEntry(iface->treck_interface,
                                           0, // Identity Association (Treck currently supports only one per iface, index=0)
                                           0); // flags (currently unused by Treck)
            int i=0;
            if(bt_entry)
            {
                while(i < bt_entry->btAddrCnt)
                {
#ifdef __linux__
                    if(IN6_ARE_ADDR_EQUAL(&bt_entry->btAddrList[i].addrValue, ipv6_addr_offset(saddr)))
#else
                    if(IN6_ARE_ADDR_EQUAL(&bt_entry->btAddrList[i].addrValue, &saddr.addr.ipv6.sin6_addr))
#endif
                    {
                        internal_addr->preferred_lifetime = bt_entry->btAddrList[i].addrPrefLifeTime;
                        internal_addr->valid_lifetime = bt_entry->btAddrList[i].addrValidLifeTime;
                        break;
                    }
                    i++;
                }
                tf6DhcpFreeBootEntry(bt_entry,
                                     0); /* flags (currently unused by Treck) */
            }
            if(!bt_entry || i == bt_entry->btAddrCnt)
            {
                // addr not found due to internal error
                DBG_ERR("tf6DhcpGetBootEntry error on %s (bt_entry=%x)\n", iface->name, bt_entry);
                goto error;
            }
        }
        break;
    default:
        // unexpected config method
        ASSERT(0);
    }

    return;

error:
    // failed to assign lifetime values due to an error
    net_send_msg(NET_IFACE_MSG_IPV6_CONFIG_FAILED, iface, (void *)0, (void *)0);
    return;
}

STATIC void ipv6_global_addr_insert(net_interface_t *iface, internal_ipv6_addr_t *internal_addr)
{
    NET_LOCK_IFACE_CONTEXT(iface);

    // only accept non-NULL configured global IPv6 addresses (i.e. DAD has completed successfully)
    ASSERT(internal_addr);
    ASSERT(internal_addr->state >= NET_IFACE_IPV6_CONFIG_COMPLETE);
    ASSERT(internal_addr->config_method != NET_IFACE_IPV6_CONFIGURED_LINKLOCAL);

    internal_ipv6_addr_t **prev = &iface->ipv6_preferred_addr;
    internal_ipv6_addr_t *cur_addr = iface->ipv6_preferred_addr;

    unsigned long cur_sys_ticks = os_time_get();

    while(cur_addr)
    {
        if( internal_addr->state <= cur_addr->state &&
            NET_IPV6_PREFERRED_LIFETIME_REMAINING(cur_sys_ticks, internal_addr) >
            NET_IPV6_PREFERRED_LIFETIME_REMAINING(cur_sys_ticks, cur_addr ) )
        {
            internal_addr->next = cur_addr;
            *prev = internal_addr;
            break;
        }
        prev = &cur_addr->next;
        cur_addr = cur_addr->next;
    }
    if(!cur_addr)
    {
        // add to end of list
        *prev = internal_addr;
    }

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;
}


STATIC internal_ipv6_addr_t * ipv6_global_addr_remove(net_interface_t *iface, int multihome_idx)
{
    NET_LOCK_IFACE_CONTEXT(iface);

    internal_ipv6_addr_t **prev = &iface->ipv6_preferred_addr;
    internal_ipv6_addr_t *cur_addr = iface->ipv6_preferred_addr;

    while(cur_addr)
    {
        if(cur_addr->multihome_idx == multihome_idx)
        {
            // found item - now remove from list
            *prev = cur_addr->next;
            cur_addr->next = NULL;
            break;
        }
        prev = &cur_addr->next;
        cur_addr = cur_addr->next;
    }
    // if addr not found then we will return NULL

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return cur_addr;
}


STATIC net_iface_ipv6_config_method_t ipv6_index_to_method(int multihome_idx)
{
    ASSERT(multihome_idx < TM_6_MAX_LOCAL_MHOME);
    if(multihome_idx < TM_MAX_IPS_PER_IF)
    {
        return NET_IFACE_IPV6_CONFIGURED_MANUAL;
    }
    if(multihome_idx == TM_MAX_IPS_PER_IF)
    {
        return NET_IFACE_IPV6_CONFIGURED_LINKLOCAL;
    }
    if(multihome_idx < TM_MAX_IPS_PER_IF + TM_6_MAX_AUTOCONF_IPS_PER_IF)
    {
        return NET_IFACE_IPV6_CONFIGURED_STATELESS;
    }
    return NET_IFACE_IPV6_CONFIGURED_DHCPV6;
}

/*
 *  ipv6_unconfigure_linklocal()
 * 
 *  Calls tfNgUnConfigInterface on the IPv6 addr at the given multihome_idx if addr is link-local.
 *  Unconfiguring all link-local addrs on a Treck interface disables IPv6 on that interface.
 */
STATIC void ipv6_unconfigure_linklocal(net_interface_t *iface, unsigned int multihome_idx)
{
    int treck_rcode = TM_ENETDOWN;
    show_current_if_states(); // lexical iface
    DBG_VERBOSE("%s() %s index %d\n", __func__, iface->name, multihome_idx);
    struct sockaddr_storage saddr;
#ifdef __linux__
    uint8_t mac_addr[MAC_ADDR_BYTE_SIZE];
    if (NULL == get_intf_mac_addr(iface->name, mac_addr)) // revisit intf_up()?
    {
        treck_rcode = TM_ENOERROR;
    }
#else
    treck_rcode = tfNgGetIpAddress(iface->treck_interface, &saddr, AF_INET6, multihome_idx);
    if(treck_rcode != TM_ENOERROR && treck_rcode != TM_ENETDOWN && treck_rcode != TM_ENOENT)
    {
        // TM_ENETDOWN or TM_ENOENT means no addr is configured at the given multihome idx.
        // Any other rcodes are considered unexpected error.
        DBG_ERR("tfNgGetIpAddress error on %s (line %d, idx=%d, treck_rcode=%d)\n",
                iface->name, __LINE__, multihome_idx, treck_rcode);
    }
#endif
    if (treck_rcode == TM_ENOERROR)
    {
        // an IPv6 addr is configured at the given multihome idx - if link-local then unconfigure
#ifdef __linux__
        if(IN6_IS_ADDR_LINKLOCAL(ipv6_addr_offset(saddr)))
#else
        if(IN6_IS_ADDR_LINKLOCAL(&saddr.addr.ipv6.sin6_addr))
#endif
        {
            DBG_VERBOSE("unconfiguring link-local IPv6 addr on %s (disables IPv6)\n", iface->name);
            treck_rcode = tfNgUnConfigInterface(iface->treck_interface, AF_INET6, multihome_idx);
            if(treck_rcode != TM_ENOERROR && treck_rcode != TM_ENOENT)
            {
                DBG_ERR("tfNgUnConfigInterface error on %s (line %d, idx=%d, treck_rcode=%d)\n",
                        iface->name, __LINE__, multihome_idx, treck_rcode);
            }
        }
    }
    return;
}
#endif // HAVE_IPV6

#ifdef TM_USE_PPPOE_CLIENT
STATIC int lcp_echo_reply_received(ttUserInterface interfaceHandle, ttUser8Bit echoRequestId, const char TM_FAR * dataPtr, int dataLen)
{
    net_interface_t *iface = net_iface_recover_local_context(interfaceHandle);

    if (echoRequestId == iface->lcp_echo_id_current)
    {
        iface->lcp_echo_id_current++;
    }
    return 0;
}

void ppp_implicit_lcp_echo(ttUserInterface interfaceHandle)
{
    net_interface_t *iface = net_iface_recover_local_context(interfaceHandle);

    iface->lcp_implicit_echo_received = true;
    iface->lcp_echo_id_current = iface->lcp_echo_id_prev + 1;
    iface->lcp_echo_failure_count = 0;
}

static void lcp_echo_timer_callback(void *param)
{
    net_interface_t *iface = (net_interface_t *)param;

    if (iface->lcp_echo_id_current == iface->lcp_echo_id_prev)
    {
        // We haven't gotten a reply yet
        iface->lcp_echo_failure_count++;
        if (iface->lcp_echo_failure_count >= LCP_ECHO_MAX_FAILURES)
        {
            // Presume they've lost their link - force a reset
            net_send_msg_no_wait(NET_IFACE_MSG_NET_RESET, iface, (void *)0, (void *)0);
        }
    }
    else
    {
        iface->lcp_echo_failure_count = 0;
    }
    iface->lcp_echo_id_prev = iface->lcp_echo_id_current;

    // Send the next echo
    if (!iface->lcp_implicit_echo_received)
    {
        int32_t netdrvr_rcode = net_send_msg_no_wait(NET_IFACE_MSG_CHECK_PPPoE_CONNECTION, iface, (void *)0, (void *)0);
        if (netdrvr_rcode == NET_IFACE_WOULDBLOCK)
        {
            // Don't count failures if we couldn't send the ping
            iface->lcp_echo_failure_count = 0;
        }
    }
    iface->lcp_implicit_echo_received = false;
}
#endif // TM_USE_PPPOE_CLIENT

//=========================================================================================================================
// Network interface enumeration
//=========================================================================================================================

net_iface_rcode_t net_iface_config_interfaces(void)
{
    DBG_VERBOSE("==>%s\n",__func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    // initialize oem func_table
    g_net_ctxt.oem_config.func_table = &g_net_ctxt.oem_func_table;
    net_iface_get_oem_config(&g_net_ctxt.oem_config);

    // Look through the list of links and configure each one
    net_link_handle_t link_handle = (net_link_handle_t)NET_LINK_INVALID_HANDLE; // bo-list
    while(1)
    {
        link_handle = net_link_get_next_logical_link(link_handle);
        DBG_VERBOSE("llink x%x\n",(unsigned int)link_handle);
        if(link_handle == (net_link_handle_t)NET_LINK_INVALID_HANDLE) // eo-list
        {
            DBG_VERBOSE("==>%s (%d) done..\n",__func__, g_net_ctxt.iface_cnt);
            // no more links
            break;
        }

        net_iface_rcode = net_iface_load_link_configuration(link_handle);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            DBG_ERR("<==%s link_handle %d (err %d)\n",__func__, link_handle, net_iface_rcode);
            XASSERT(net_iface_rcode == NET_IFACE_OK, net_iface_rcode); // fix it now
            goto done;
        }
    }

done:
    DBG_VERBOSE("<==%s (net_iface_rcode=%d)\n",__func__, net_iface_rcode);
    return net_iface_rcode;
}

net_iface_rcode_t net_iface_enumerate_interfaces(void)
{
    DBG_VERBOSE("==>%s\n",__func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
    int intf_index = 0;

    // attach each enumerated link to a corresponding network interface context
    net_link_handle_t link_handle = (net_link_handle_t)NET_LINK_INVALID_HANDLE; // bo-list
    g_net_ctxt.iface_cnt = 0; // none sofar
    while(1)
    {
        link_handle = net_link_get_next_logical_link(link_handle);
        if(link_handle == (net_link_handle_t)NET_LINK_INVALID_HANDLE) // eo-list
        {
            DBG_VERBOSE("==>%s (%d) done..\n",__func__, g_net_ctxt.iface_cnt);
            // no more links
            break;
        }

        net_iface_rcode = net_iface_attach_link(link_handle);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            XASSERT(0, net_iface_rcode);
            goto done;
        }

        // prepare interface for link acquisition
        net_interface_t* intf = g_net_ctxt.ifaces[intf_index];
        if (intf)
        {
            adv_state(intf, NET_IFACE_STATE_ACQUIRE_LINK);
        }
        else
        {
            DBG_ERR("==>%s intf#%d not defined?\n",__func__, intf_index);
        }
        intf_index++;
    }

    /*
     *  confirm that we enumerated all OEM-defined interfaces
     * 
     *  NOTE: failure of this assertion implies an enumeration mismatch between
     *  net_iface_config.c (or net_iface_config_dflt.c) and net_link_config.c (or
     *  net_link_config_dflt.c).
     */
// Don't assert here because we still want system to come up even if a link fails to init,
// e.g. failure of Ethernet link init due to zeroed/invalid MAC address.  In this case we could 
// still have other links available or at very least the loopback interface for IPC etc.
//    XASSERT(g_net_ctxt.iface_cnt == g_net_ctxt.oem_config.iface_cnt, g_net_ctxt.oem_config.iface_cnt);
    if (g_net_ctxt.iface_cnt != g_net_ctxt.oem_config.iface_cnt)
    {
        DBG_ERR("%s() oem-interfaces %d != %d enumerated @ runtime\n",__func__, g_net_ctxt.oem_config.iface_cnt, g_net_ctxt.iface_cnt);
    }

done:
    DBG_VERBOSE("<==%s (net_iface_rcode=%d)\n",__func__, net_iface_rcode);
    return net_iface_rcode;
}

// intf mac address; return given buffer (filled as appropriate)
uint8_t* get_intf_mac_addr(char* intf_of_interest, uint8_t* given_buf)
{
    struct ifconf current_intf;
    struct ifreq intf_set[NET_MAX_INTERFACES];
    int number_of_intf;
    int i;
    int sock;

    if (given_buf == NULL)
    {
        DBG_ERR("%s: invalid param: given_buf\n", __func__);
        return NULL;
    }
    memset(given_buf, 0, MAC_ADDR_BYTE_SIZE); // clear out given buffer

    // Create a socket or return an error.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        DBG_ERR("%s: socket err %s (%d)", __func__, strerror(thread_errno), thread_errno);
        return NULL;
    }

    // Point ifconf's ifc_buf to our array of interface ifreqs.
    current_intf.ifc_buf = (char *) intf_set;

    // Set ifconf's ifc_len to the length of our array of interface ifreqs.
    current_intf.ifc_len = sizeof intf_set;

    //  Populate ifconf.ifc_buf (ifreq) with a list of interface names and addresses.
    if (ioctl(sock, SIOCGIFCONF, &current_intf) == -1)
    {
        DBG_ERR("%s: SIOCGIFCONF err %s (%d)", __func__, strerror(thread_errno), thread_errno);
        close(sock);
        return NULL;
    }

    // Divide the length of the interface list by the size of each entry.
    // This gives us the number of interfaces on the system.
    number_of_intf = current_intf.ifc_len / sizeof(intf_set[0]);

    for (i = 0; i < number_of_intf; i++)
    {
        if (!strcmp(intf_set[i].ifr_name, intf_of_interest)) // matching intf?
        {
            struct ifreq* rec = &intf_set[i];

            if (ioctl(sock, SIOCGIFHWADDR, rec) == 0)
            {
                if (given_buf) memcpy(given_buf, rec->ifr_hwaddr.sa_data, MAC_ADDR_BYTE_SIZE);
            }
            DBG_VERBOSE("%s: %s mac: %02x:%02x:%02x:%02x:%02x:%02x\n", __func__, rec->ifr_name, 
                rec->ifr_hwaddr.sa_data[0], rec->ifr_hwaddr.sa_data[1], rec->ifr_hwaddr.sa_data[2],
                rec->ifr_hwaddr.sa_data[3], rec->ifr_hwaddr.sa_data[4], rec->ifr_hwaddr.sa_data[5]);
            break; // done
        }
    }
    close(sock);
    return given_buf; // return given buf
}


// declare the common/network init() such that an oem-specific net_iface_init() can override
void WEAK net_iface_init(void)
{
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

#ifdef HAVE_NET_SERVICES
    net_services_init();
#endif

#ifdef HAVE_ETHERNET
    net_utils_init(); // network timers, etc..
    net_link_init(); // spawn link-layer state machine (e.g, mac/phy events)
    net_iface_task(0);
#endif

    return;
}

// private
error_type_t net_iface_send_link_msg(net_iface_msg_type_t type, net_interface_t* iface, void* udata, void* data2)
{
    if (iface)
    {
        DBG_VERBOSE("%s() '%s' %d x%x (%s)\n", __func__, iface->name, type, udata, str_iface_msg(type));
    }
    else
    {
        DBG_ERR("%s(%d, x%x, x%x, x%x)\n", __func__, type, iface, udata, data2);
    }
    return net_send_msg_no_wait(type, iface, udata, data2);
}


// bonjour/mdns restart backoff strategy
#define MAX_WAIT_TICKS 400 /* ticks or ms? */


// net_iface_notify() -- tailored to bonjour/mdns restarts
//    add additional triggers as needed
error_type_t net_iface_notify(net_interface_t* intf, net_link_event_t event)
{
    error_type_t ret = FAIL;
    if (intf)
    {
        DBG_VERBOSE("%s(%s) %d\n", __func__, (intf->name? intf->name: "<undefined>"), event);

        // detect a link up/down/reset event
        if ((event & (NET_LINK_EVENT_RESET | NET_LINK_EVENT_UP | NET_LINK_EVENT_DOWN)))
        {
#ifdef HAVE_MDNS
#ifdef HAVE_NET_SERVICES
            net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, NULL, NULL);
#else
	    static int wait_ticks = 100; // initially -- increased as number of failures increases
            // mdns needs to know when an interface change -- start nearly immediately
            ret = restart_mdns_service(0, wait_ticks);

            DBG_INFO("%s() restart mdns (in %d) returned %d\n", __func__, wait_ticks, ret);
            if (ret != OK)
            {
                wait_ticks += wait_ticks; // double wait time
                wait_ticks = MIN(wait_ticks, MAX_WAIT_TICKS);
            }

#endif // HAVE_NET_SERVICES
#endif // HAVE_MDNS
        }
        ret = OK;
    }

    return ret;
}

// runtime interface initialization (and recurring refresh --
//    interface list can change dynamically as hw inventory
//    changes, e.g, (add/subtract wifi module over usb)).
struct if_nameindex* intf_mapp = NULL;
bool* intf_status_active = NULL; // bool 'active' tracker per interface
int refresh_iface_bindings(void)
{
    int if_count = 0;

    NET_LOCK_CONTEXT();

    // query os for current intf list
    if (intf_mapp) if_freenameindex(intf_mapp); // free previous table memory
    intf_mapp = if_nameindex(); // os-provided
    if (intf_mapp)
    {
        while (intf_mapp[if_count].if_name)
        {
            // update 'treck_idx' based on latest interface inventory (0 == inactive/not-available)
            int known_interface = 0;
            while (known_interface < NET_MAX_INTERFACES)
            {
                net_interface_t* intf = g_net_context->ifaces[known_interface];
                if (intf && intf->name && !strcmp(intf->name, intf_mapp[if_count].if_name)) // match
                {
                    intf->treck_idx = intf_mapp[if_count].if_index; // sync local intf w/dynamic if_index
                    DBG_VERBOSE("%s() if_index %d -> local %d\n", __func__, intf_mapp[if_count].if_index, intf->local_idx);
                    break;
                }
                known_interface++;
            }
            if_count++;
        }
    }
    NET_UNLOCK_CONTEXT();

    // update current interface inventory
    sizeof_intf_map = if_count; // runtime update

#if 0 // DEBUG
    int active_intfs = sizeof_intf_map;
    while (active_intfs-->0)
    { if (intf_mapp[active_intfs].if_name) DBG_VERBOSE("%s() offset %d intf %s if_index %d\n", __func__, active_intfs, intf_mapp[active_intfs].if_name, intf_mapp[active_intfs].if_index); }
#endif // DEBUG

    return sizeof_intf_map;
}

void initialize_iface_bindings(void)
{
    int iface_count = refresh_iface_bindings(); UNUSED_VAR(iface_count); // prime interface inventory
    DBG_VERBOSE("%s() %d os-interfaces enumerated\n", __func__, iface_count);

    // initialize interface active cache
    //     NOTE:  intf list size is reflected in sizeof_intf_map,
    //       although as a dynamic quantity (e.g, count changes with
    //       hot-swap usb wifi modules) allocate a minimum array..

    intf_status_active = (bool*)MEM_MALLOC(32); // waste a few bools
    memset(intf_status_active, 0, sizeof_intf_map); // zero-fill mapping

    // lo
    int i = NET_IFACE_LINK_TYPE_LO;
    char* if_name = NET_IF_NAME_LO;
    intf_status_active[i] = intf_active(if_name);

    // eth0
    i = NET_IFACE_LINK_TYPE_ETH;
    if_name = NET_IF_NAME_ETH;
    intf_status_active[i] = intf_active(if_name);

#ifdef HAVE_WIRELESS
#ifdef HAVE_WLAN
    // mlan0
    i = NET_IFACE_LINK_TYPE_STA;
    if_name = NET_IF_NAME_STA;
    intf_status_active[i] = intf_active(if_name);
#endif // HAVE_WLAN
#ifdef HAVE_UAP
        // uap0
    i = NET_IFACE_LINK_TYPE_UAP;
    if_name = NET_IF_NAME_UAP;
    intf_status_active[i] = intf_active(if_name);
#endif // HAVE_UAP
#endif // HAVE_WIRELESS
}

// public api
void net_iface_task(unsigned long arg)
{
    initialize_iface_bindings(); // runtime-dynamic bindings
    net_iface_start(arg);
    net_iface_vars_init(); // needs interface(s) and event/notifications

    refresh_iface_bindings();

    register_net_iface_cmds(); // cmd interface

    // bind link(s) for each interface
    int i = 0;
    net_interface_t* intf = g_net_ctxt.ifaces[0];
    while (intf) // foreach configured interface
    {
        // bind current interface index and if_index values
        intf->local_idx = i;
        intf->treck_idx = if_nametoindex(intf->name);

        net_send_msg_no_wait(NET_IFACE_MSG_LINK_UP_POLL, intf, NULL, NULL);
        i++;
        intf = g_net_ctxt.ifaces[i];
    }

#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_START, NET_SERVICE_UAP, 0, 0);
#ifdef HAVE_MDNS
    net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, 0, 0); // prime the bonjour service -- noop if already started
#endif
#else // inline startup
    start_uap_service(); //TEMPTEMP
#ifdef HAVE_MDNS
    start_mdns_service(); // prime the bonjour service -- noop if already started
#endif
#endif // HAVE_NET_SERVICES
#endif // HAVE_UAP
#endif // HAVE_WIRELESS
}

// returns true iff the given interface's CURRENT treck_idx (aka if_index) is enabled
bool net_intf_enabled(net_interface_t* intf)
{
    bool intf_enabled = false;
    if (intf)
    {
        net_iface_get_var_intf_enabled(intf->treck_idx, &intf_enabled);
    }
    return intf_enabled;
}

// returns true iff the named interface is of interest (e.g, oem-known) and enabled
bool net_if_name_enabled(char* if_name)
{
    bool intf_enabled = false;
    if (if_name)
    {
        unsigned int if_index = if_nametoindex(if_name);
        if (if_index > 0)
        {
            net_iface_get_var_intf_enabled(if_index, &intf_enabled);
        }
    }
    return intf_enabled;
}

#ifdef __cplusplus
}
#endif

//eof net_iface.c
