/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _NET_IFACE_H
#define _NET_IFACE_H

// os headers
#include <stdint.h>

// common headers
#include "os_network.h"
#include "error_types.h"
#include "net_iface_api.h"
#include "net_iface_config.h"
#include "net_link_api.h"

#ifdef HAVE_IPV6
  #define NET_IFACE_IPV6_INTERNAL_ADDR_SIGNATURE  0xf0e1d2c3
#endif // HAVE_IPV6


#define NET_RESET_DELAY_SEC                 2
/*  When netdrvr detects a configuration change that requires a network reset to incorporate the
 *  change, the netdrvr state machine will delay NET_RESET_DELAY_SEC seconds before triggering the
 *  reset.  Network reset involves a lengthy reconfiguration sequence, and the delay helps ensure
 *  that successive network configuration changes that are all made within a short period of time
 *  will invoke only a single network reset.
 */


#define NET_LOCK_IFACE_CONTEXT(iface) \
        { \
            unsigned int os_rcode = pthread_mutex_lock(&iface->mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#define NET_UNLOCK_IFACE_CONTEXT(iface) \
        { \
            unsigned int os_rcode = pthread_mutex_unlock(&iface->mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 


//=========================================================================================================================
// IP configuration states
//=========================================================================================================================

/*
 *  IPv4 configuration states
 */

// IMPORTANT: IPv4 state logic relies on the order in which states are defined below
typedef enum
{
    /*  IPv4 is not configured on the interface. Check IPv4 config flags for errors, if any. */
    IPV4_NOT_CONFIGURED,

    /*  IPv4 is in the process of being closed on the interface. */
    IPV4_CLOSING,

    /*  IPv4 configuration is in progress.  Possible configuration methods are pre-selected by user. */
    IPV4_CONFIGURING,

    /* IPv4 was previously configured but is now in the process of being reconfigured */
    IPV4_RECONFIGURING,

    /*  Treck interface is guaranteed to be up/open while in state IPV4_CONFIGURED. */
    IPV4_CONFIGURED
} ipv4_cfg_state_t;

/*
 *  IPv4 configuration status flags
 */

#define IPV4_DUPLICATE_ADDR         0x0001
/*  A duplicate address has been detected on the network for manual, DHCP, or BOOTP config methods.
 *  Does NOT apply to the auto-IP config method, where a collision simply triggers selection of a
 *  new auto-IP address.
 */

#define IPV4_BAD_MANUAL_CONFIG      0x0002
/*  The manual IPv4 address configured by the user is invalid.
 */

#define IPV4_AUTOCONFIG_TIMEOUT     0x0004
/*  Auto-configuration timed out before an IPv4 address was acquired.  This will only happen when
 *  DHCP/BOOTP config methods have timed out and the auto-IP config method has not been selected by
 *  the user.
 */

#define IPV4_DHCP_NAK               0x0010
#define IPV4_DHCP_LEASE_EXPIRED     0x0020

#define IPV4_INTERNAL_ERROR         0x8000
/*  IPv4 configuration failed due to an internal system error.
 */

typedef uint16_t ipv4_cfg_status_flags_t;

/*
 *  IPv6 configuration states
 */

#ifdef HAVE_IPV6

// IMPORTANT: IPv6 state logic relies on the order in which states are defined below
typedef enum
{
    IPV6_NOT_CONFIGURED,
    /*  IPv6 is not configured on the interface.  Check IPv6 config flags for errors, if any.
     */

    IPV6_CLOSING,
    /*  IPv6 is in the process of being closed on the interface.
     */

    IPV6_CONFIGURING,
    /*  The interface is performing DAD on the link-local IPv6 address
     */

    IPV6_LOCAL_CONFIGURED,
    /*  The interface is configured with a link-local IPv6 address, but global IPv6 address
     *  auto-configuration has not yet completed.  IPv6 is still "configuring", waiting for additional
     *  global IPv6 addressess to be acquired from routers and/or DHCPv6 servers and complete Duplicate
     *  Address Detection.
     */

    IPV6_CONFIGURED
    /*  The interface is configured with at least a link-local IPv6 address, and the global IPv6 address
     *  auto-configuration timeout has expired.
     */

} ipv6_cfg_state_t;

/*
 *  IPv6 configuration status flags
 */

#define IPV6_GLOBAL_CONFIG_TIMEOUT  0x0001
/*  A manual global IPv6 address is either not configured or has failed Duplicate Address Detection,
 *  and no other global IPv6 addresses were acquired before the IPv6 auto-configuration timeout.
 *  IPv6 can only be used to communicate with devices on the local subnet via the link-local IPv6
 *  address.
 */

#define IPV6_GLOBAL_LOST_CONFIG     0x0002
/*  A manual global IPv6 address is either not configured or has failed Duplicate Address Detection,
 *  and all auto-configured global IPv6 addresses have either expired (i.e. valid lifetime has
 *  expired) or have been displaced by duplicate addresses detected on the network after IPv6
 *  configuration was completed.  IPv6 can only be used to communicate with devices on the local
 *  subnet via the link-local IPv6 address.
 */

#define IPV6_DUPLICATE_ADDR         0x0010
/*  Either the manual IPv6 address (user configured) or the auto-configured link-local IPv6
 *  address has failed Duplicate Address Detection. Loss of the link-local IPv6 address will result
 *  in IPv6 being disabled on the interface (see ipv6_user.pdf pp50-51).
 */

#define IPV6_BAD_MANUAL_CONFIG      0x0020
/*  The manual IPv6 address configured by the user is invalid.
 */

#define IPV6_INTERNAL_ERROR         0x8000

typedef uint16_t ipv6_cfg_status_flags_t;

#endif // HAVE_IPV6


//=========================================================================================================================
// State machine
//=========================================================================================================================

/*
 *  states
 */

// IMPORTANT: after modifying this table, must also modify g_net_iface_state_strs in net_iface.c
typedef enum
{
    NET_IFACE_STATE_NOT_STARTED = 0,
    NET_IFACE_STATE_ACQUIRE_LINK,
    NET_IFACE_STATE_OPENING_INTERFACE,
    NET_IFACE_STATE_INTERFACE_OPEN,
    NET_IFACE_STATE_CLOSING_INTERFACE,
    NET_IFACE_STATE_INTERNAL_ERROR,

    // add new states above this point
    NET_IFACE_NUM_STATES
} net_iface_state_t;

/*
 *  messages
 */

// IMPORTANT: after modifying this table, must also modify g_net_iface_msg_strs in net_iface.c
typedef enum
{
    NET_IFACE_MSG_LINK_UP,            // a 'link-up' event was received from link driver
    NET_IFACE_MSG_LINK_UP_POLL,       // poll for link-up status independent of any event
    NET_IFACE_MSG_LINK_DOWN,
    NET_IFACE_MSG_LINK_DOWN_PENDING,
    NET_IFACE_MSG_NET_RESET,
    NET_IFACE_MSG_IPV4_CONFIG_COMPLETE,
    NET_IFACE_MSG_IPV4_CONFIG_FAILED,
    NET_IFACE_MSG_IPV4_CONFIG_RETRY,
    NET_IFACE_MSG_IPV4_ARP_ANNOUNCE,
    NET_IFACE_MSG_IPV4_CLOSE_FINISH,
#ifdef HAVE_IPV6
    NET_IFACE_MSG_IPV6_LOCAL_CFG_COMPLETE,
    NET_IFACE_MSG_IPV6_GLOBAL_CFG_COMPLETE,
    NET_IFACE_MSG_IPV6_CONFIG_FAILED,
    NET_IFACE_MSG_IPV6_CONFIG_RETRY,
    NET_IFACE_MSG_IPV6_CLOSE_FINISH,
#endif // HAVE_IPV6
    NET_IFACE_MSG_UNRECOVERABLE_ERROR,
    NET_IFACE_MSG_ENABLE_LINKS,

    NET_IFACE_MSG_EXTERNAL_CLOSE_DETECTED,
    NET_IFACE_MSG_ALL_CLOSE_FINISH,
    NET_IFACE_MSG_CHECK_PPPoE_CONNECTION,

    // add new msgs above this point
    NET_IFACE_NUM_MSGS
} net_iface_msg_type_t;


//=========================================================================================================================
// Assigned IPv6 address context
//=========================================================================================================================

#ifdef HAVE_IPV6

#define IPV6_MAX_ADDRS                      TM_6_MAX_MHOME

// zero-based indicies into the Treck Multi-home table
#define IPV6_MANUAL_MULTIHOME_IDX           0 // first index in the Treck IPv6 manual range
#define IPV6_LINKLOCAL_MULTIHOME_IDX        TM_MAX_IPS_PER_IF // first index in the Treck IPv6 AUTOCONF range
#define IPV6_STATELESS_MULTIHOME_IDX_FIRST  (TM_MAX_IPS_PER_IF+1) // configured via router (does not include link-local)
#define IPV6_STATELESS_MULTIHOME_IDX_LAST   (TM_MAX_IPS_PER_IF+TM_6_MAX_AUTOCONF_IPS_PER_IF-1)
#define IPV6_STATEFUL_MULTIHOME_IDX_FIRST   (TM_MAX_IPS_PER_IF+TM_6_MAX_AUTOCONF_IPS_PER_IF) // configured via DHCPv6
#define IPV6_STATEFUL_MULTIHOME_IDX_LAST    (TM_MAX_IPS_PER_IF+TM_6_MAX_AUTOCONF_IPS_PER_IF+TM_6_MAX_DHCP_IPS_PER_IA-1)
 
// states must appear in order of addr preference (used to sort configured global IPv6 addr list)
typedef enum {
    NET_IFACE_IPV6_CONFIG_STARTED,
    NET_IFACE_IPV6_CONFIG_FAILED_DAD,
    NET_IFACE_IPV6_CONFIG_COMPLETE,
    NET_IFACE_IPV6_ADDR_DEPRECATED,
    NET_IFACE_IPV6_LATE_DUP_ADDR_DETECTED, // duplicate IPv6 addr detected after config complete (i.e. passed initial DAD)
} net_iface_ipv6_addr_state_t;

struct net_interface_s;

typedef struct internal_ipv6_addr_s
{
    uint32_t                    signature; // **MUST BE FIRST** used to determine when an IPv6 addr pointer is valid
    struct internal_ipv6_addr_s *next; // next addr in linked list
    net_iface_ipv6_addr_t       ipv6_addr;
    net_iface_ipv6_config_method_t config_method;
    unsigned long               sys_time_acquired; // system ticks when address was acquired
    uint32_t                    preferred_lifetime;
    uint32_t                    valid_lifetime;
    struct net_interface_s      *iface;
    int                         multihome_idx; // Treck index
    net_iface_ipv6_addr_state_t state;
} internal_ipv6_addr_t;

#endif // HAVE_IPV6


//=========================================================================================================================
// Network event registration
//=========================================================================================================================

typedef uint32_t net_iface_event_flags_t;
typedef void (*net_iface_event_func_ptr_t)(net_iface_event_flags_t events, void* context);

struct net_interface_s;

typedef struct net_iface_event_node_s
{
    struct net_iface_event_node_s   *next;
    net_iface_event_flags_t         event_flags;
    net_iface_event_func_ptr_t      event_cb;
    struct net_interface_s          *iface;
    void                            *user_context;
} net_iface_event_node_t;

//=========================================================================================================================
// Network timers
//=========================================================================================================================

typedef struct {
    net_timer_handle_t      handle;
    struct net_interface_s  *iface;
    net_iface_msg_type_t    msg;
} net_iface_msg_timer_t;

#define LCP_ECHO_INTERVAL     (1000 * 2)  // once every other second, make sure the LCP connection is still alive
#define LCP_ECHO_MAX_FAILURES (4)         // max times an LCP echo can fail before we disconnect and reconnect

//=========================================================================================================================
// Network interface context
//=========================================================================================================================

typedef struct net_interface_s
{
    OS_THREAD               *recv_thread_ctxt;
    uint8_t                 *recv_thread_stack;
    ttUserInterface         treck_interface;
    ttUserLinkLayer         treck_link_handle;      // Handle to the link layer in use
    tt32Bit                 treck_link_protocol;
    /*const*/ uint8_t       local_idx; // index into local ifaces array (revisit const?)
    uint8_t                 treck_idx; // index into Treck internal array, as returned by if_nametoindex()
    char                    *name; // reference to interface name from OEM config struct
    OS_MUTEX                mtx;

    // interface-specific OEM config
    net_iface_instance_oem_func_table_t oem_func_table;  
    net_iface_instance_oem_config_t oem_config; 

    // oneshot timers
    net_timer_handle_t      timer_hndl_reset;
    net_timer_handle_t      timer_hndl_v6autoconf;
    net_iface_msg_timer_t   msg_timer_arp;
    net_iface_msg_timer_t   msg_timer_v4autoip;
    
    net_link_handle_t       link_handle; // attached link
    net_link_event_handle_t link_event_handle;

    net_iface_state_t       state; // current state of the interface state machine
    ipv4_cfg_state_t        ipv4_cfg_state;
    ipv4_cfg_status_flags_t ipv4_cfg_status;
#ifdef HAVE_IPV6
    ipv6_cfg_state_t        ipv6_cfg_state;
    ipv6_cfg_status_flags_t ipv6_cfg_status;
#endif // HAVE_IPV6

    tt32Bit                 ipv4_dhcp_lease_start;
    ttUser32Bit             ipv4_dhcp_lease_time;
    uint16_t                ipv4_autoip_probe_collision_cnt; // ARP probe collisions (not including ARP cache collisions)
    uint32_t                ipv4_coll_detect_addr; // non-zero when addr registered for collision detection

    /*  An IPv4 config method that has been started but not yet successfully completed.  Valid only
     *  while in IPv4 config state IPV4_CONFIGURING.
     */
    net_iface_ipv4_config_method_t  ipv4_pending_config; 

    bool                    ipv4_dhcp_started; // tfDhcpUserStart() is in progress
    bool                    ipv4_bootp_started; // tfBootpUserStart() is in progress

#ifdef HAVE_IPV6            
    bool                    ipv6_dhcp_started; // when true, must call tf6DhcpUserStop() when closing
    internal_ipv6_addr_t    *ipv6_preferred_addr; // linked list of configured global addrs in order of preference
    internal_ipv6_addr_t    *addr_map[IPV6_MAX_ADDRS]; // maps Treck multihome_idx to internal_addr (includes addrs not yet configured)
#endif // HAVE_IPV6
#ifdef HAVE_HOTSPOT_SUPPORT
    bool                    nat_enabled;
#endif // HAVE_HOTSPOT_SUPPORT

    /* PPPoE related vars */
    bool                    pppoe_iface_closing;         // Is PPPoE in the process of closing down?
    uint32_t                pppoe_steps_completed;       // How far we've gotten through the config process
    int                     pppoe_steps_completed_this_attempt;
    int                     pppoe_prev_flags;

    uint8_t                 lcp_echo_id_current;         // 
    uint8_t                 lcp_echo_id_prev;            // 
    int                     lcp_echo_failure_count;      // Consecutive times PPPoE server has failed to respond to an echo
    bool                    lcp_implicit_echo_received;  // Used to suppress echo requests while traffic is active
    net_timer_handle_t      lcp_echo_timer;              // Used for timeouts and determining when next echo should be sent
} net_interface_t;

typedef struct
{
    uint8_t                 iface_cnt; // number of interfaces in "ifaces" array
    net_iface_event_node_t  *event_list_head;
    bool                    initialized; // network interface module has completed initialization
    OS_MUTEX                mtx;
    net_iface_oem_func_table_t oem_func_table;
    net_iface_oem_config_t oem_config;
    net_interface_t         *ifaces[NET_MAX_INTERFACES]; // NET_MAX_INTERFACES?
} net_iface_context_t;


//=========================================================================================================================
// Private function prototypes
//=========================================================================================================================

// utility routines
int32_t net_send_msg_wait_opt(
    net_iface_msg_type_t type,
    net_interface_t *iface,
    void *udata,
    void *data2,
    unsigned long os_wait_opt);
void net_config_change_notify(unsigned int if_index);
net_interface_t *net_iface_lookup_by_name(const char *name);
net_interface_t *net_iface_treck_index_to_local_context(unsigned int treck_index);

// bind ifaces[] offset w/os-interface mapping
char* iface_name_lookup_by_index(int offset); // ifaces[]

// Local interface context accessors
net_iface_context_t *net_get_ctxt(void);
net_iface_context_t *net_lock_ctxt(void);
void net_unlock_ctxt(net_iface_context_t *net_ctxt);

// network private datastore accessors
error_type_t net_set_manual_hostname(unsigned int if_index, const char *hostname);
error_type_t net_set_ipv4_last_config(unsigned int if_index, const net_iface_ipv4_config_method_t *method);
error_type_t net_set_ipv4_manual_addr(unsigned int if_indexe, const uint32_t *ipv4_addr);
error_type_t net_set_ipv4_manual_subnet_mask(unsigned int if_index, const uint32_t *ipv4_addr);
error_type_t net_set_ipv4_manual_gateway(unsigned int if_index, const uint32_t *ipv4_addr);
error_type_t net_set_ipv4_manual_wins_addr(unsigned int if_index, const uint32_t *ipv4_addr);
error_type_t net_get_ipv4_autoip_addr(unsigned int if_index, uint32_t *ipv4_addr);
error_type_t net_set_ipv4_autoip_addr(unsigned int if_index, uint32_t *ipv4_addr);

#ifdef TM_USE_PPPOE_CLIENT
error_type_t net_get_pppoe_enabled(unsigned int local_idx, bool *enabled);
#endif // TM_USE_PPPOE_CLIENT

#ifdef HAVE_CMD
void net_iface_register_debug_commands();
#endif // HAVE_CMD

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

// called from threads
#define net_send_msg(type, iface, udata, data2) \
    net_send_msg_wait_opt(type, iface, udata, data2, OS_WAIT_FOREVER)

// called from non-threads (e.g. timers)
#define net_send_msg_no_wait(type, iface, udata, data2) \
    net_send_msg_wait_opt(type, iface, udata, data2, OS_NO_WAIT)

#define NET_IPV6_PREFERRED_LIFETIME_REMAINING(cur_ticks, internal_addr) \
    ((internal_addr->preferred_lifetime == -1)?(-1):((int32_t)(internal_addr->preferred_lifetime - (cur_ticks - internal_addr->sys_time_acquired)/SYS_TICK_FREQ)))

#define NET_IPV6_VALID_LIFETIME_REMAINING(cur_ticks, internal_addr) \
    ((internal_addr->preferred_lifetime == -1)?(-1):((int32_t)(internal_addr->valid_lifetime - (cur_ticks - internal_addr->sys_time_acquired)/SYS_TICK_FREQ)))

#define NET_IFACE_IPV6_IS_ADDR_LINKLOCAL(internal_ipv6_addr_ptr) \
            (((internal_ipv6_addr_ptr)->ipv6_addr.addr.addr8[0] == (uint8_t)0xfe) && \
            (((internal_ipv6_addr_ptr)->ipv6_addr.addr.addr8[1] & (uint8_t)0xc0) == (uint8_t)0x80))

#define NET_IFACE_IPV4_IS_ADDR_LINKLOCAL(ipv4_addr) \
        (tm_ip_is_local(ipv4_addr) && (ipv4_addr >= TM_IP_LOCAL_FIRST) && (ipv4_addr <= TM_IP_LOCAL_LAST))

#define NET_IPV4_OCTET(addr, octet) (0x000000FF&(addr>>((3-octet)*8)))

/* return current state for given interface */
net_link_status_t get_intf_status(char* name);

void net_iface_task(unsigned long unused);
int net_iface_config_interfaces(void);
int net_iface_enumerate_interfaces(void);
uint8_t net_iface_enum_from_if_index(unsigned int if_index);
void net_iface_vars_init(void);


// notifications
error_type_t net_iface_send_link_msg(net_iface_msg_type_t type, net_interface_t* iface, void* udata, void* data2);
error_type_t net_iface_notify(net_interface_t* intf, net_link_event_t event);
void net_iface_link_event_notify(net_link_event_t link_event, void *user_data);

// lookup interface 'enabled' state
bool net_intf_enabled(struct net_interface_s* intf); // by intf->treck_idx
bool net_if_name_enabled(char* intf);         // by if_name -> if_index

// global data
extern net_iface_context_t* g_net_context; // aka g_net_ctxt

// debug
char* str_iface_state(int state); // returns str corresponding to int state
char* str_iface_msg(int type);
char* str_ipv4_cfg_state(int);
char* str_ipv6_cfg_state(int);

#if ! defined DO_NOT_INCLUDE_IF_H
// os-provided
#include <net/if.h> // unsigned if_nametoindex(char* name_buffer), char* if_indextoname(unsigned index, char* name_buffer); // assumes name_buffer[IF_NAMESIZE]
#endif // DO_NOT_INCLUDE_IF_H

#endif // _NET_IFACE_H

//eof
