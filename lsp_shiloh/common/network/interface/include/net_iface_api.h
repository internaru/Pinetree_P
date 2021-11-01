/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef NET_IFACE_API_H
#define NET_IFACE_API_H

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
#ifdef HAVE_CMD
#include "dbg_raw_socket.h"
#endif


// enable NET_IFACE_EVENT_LINK_STATUS_CHANGE notifications (non-polling method)

#define NET_LINK_STATUS_CHANGE_POLLING 0 /* non-zero for ~1 second periodic poll */

/**
 * @brief Indicates a status change (up vs. down) in the link layer of an interface, e.g.
 *        after plugging or unplugging the cable for an Ethernet link.
 *  
 * If the logical link is shared by multiple underlying physical links, the event will 
 * also signal any status changes to the physical links, even if status of the logical 
 * link remains unchanged. 
 *  
 * For example, assume a logical link is shared by both an Ethernet link and a WLAN link,
 * Ethernet having priority, with the Ethernet link down but the physical WLAN link up. In
 * this state, a call to net_iface_get_var_network_status() would indicate that the 
 * logical link is 'up', and a call to net_iface_get_var_active_link() would reveal the 
 * current binding to the physical WLAN link. If the Ethernet link subsequently becomes 
 * active, Ethernet will preempt the WLAN link forcing it to go down. At this 
 * point, a NET_IFACE_EVENT_LINK_STATUS_CHANGE will be generated even though the logical 
 * link status remains 'up', but a call to net_iface_get_var_active_link() will reveal 
 * the change in the physical link binding from WLAN to Ethernet. 
 */
#define NET_IFACE_EVENT_LINK_STATUS_CHANGE          0x0008


/**
 * @brief Indicates a change in IPv4 configuration status. Call net_iface_get_var_ipv4_configured()
 *        for current status.
 */
#define NET_IFACE_EVENT_IPV4_CONFIG_CHANGE          0x0001

/**
 * @brief Indicates that an IPv4 shutdown is imminent. The shutdown will be delayed until all user
 *        callback instances registered to receive this event have been notified, and a subsequent
 *        call to net_iface_continue_shutdown() has been made for each callback instance.
 */
#define NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING       0x1000

#ifdef HAVE_IPV6

/**
 * @brief Indicates a change in link-local IPv6 configuration status. Call
 *        net_iface_get_var_ipv6_local_configured() for current status.
 */
#define NET_IFACE_EVENT_IPV6_LOCAL_CONFIG_CHANGE    0x0002

/**
 * @brief Indicates a change in IPv6 configuration status. Call net_iface_get_var_ipv6_configured()
 *        for current status.
 */
#define NET_IFACE_EVENT_IPV6_CONFIG_CHANGE          0x0004

/**
 * @brief Indicates that an IPv6 shutdown is imminent. The shutdown will be delayed until all user
 *        callback instances registered to receive this event have been notified, and a subsequent
 *        call to net_iface_continue_shutdown() has been made for each callback instance.
 */
#define NET_IFACE_EVENT_IPV6_SHUTDOWN_PENDING       0x2000


/**
 * @brief Test for the IPv6 unspecified address.
 * 
 * @param ipv6_addr An IPv6 address of type net_iface_ipv6_addr_t.
 * 
 * @return "true" if ipv6_addr is the IPv6 unspecified address, "false" otherwise.
 * 
 * Use this macro to test for the IPv6 unspecified address (i.e. a zero address, or "::").
 */
#define NET_IFACE_IPV6_ADDR_UNSPECIFIED(ipv6_addr) \
    ((!(ipv6_addr).addr.addr32[0]) && \
     (!(ipv6_addr).addr.addr32[1]) && \
     (!(ipv6_addr).addr.addr32[2]) && \
     (!(ipv6_addr).addr.addr32[3]))

/**
 * @brief Test if two IPv6 addresses are equivalent.
 * 
 * @param ipv6_addr1 An IPv6 address of type net_iface_ipv6_addr_t.
 * 
 * @param ipv6_addr2 A second IPv6 address of type net_iface_ipv6_addr_t.
 * 
 * @return "true" if ipv6_addr1 is equivalent to ipv6_addr2, "false" otherwise.
 * 
 * Use this macro to test if two IPv6 addresses are equivalent. The addresses are equivalent if the
 * addresses match byte for byte and have an identical prefix length.
 */
#define NET_IFACE_IPV6_ADDRS_EQUAL(ipv6_addr1, ipv6_addr2) \
    (((ipv6_addr1).addr.addr32[0] == (ipv6_addr2).addr.addr32[0]) && \
     ((ipv6_addr1).addr.addr32[1] == (ipv6_addr2).addr.addr32[1]) && \
     ((ipv6_addr1).addr.addr32[2] == (ipv6_addr2).addr.addr32[2]) && \
     ((ipv6_addr1).addr.addr32[3] == (ipv6_addr2).addr.addr32[3]) && \
     ((ipv6_addr1).prefix == (ipv6_addr2).prefix))

#endif // HAVE_IPV6


/**
 * @brief Initialize the Network Interface layer.
 *
 * This routine should be called exactly once following a system startup.  It initializes any
 * underlying link layers that have been registered with the network interface layer and spawns
 * threads for the network receive handler, timer, and interface state machine.
 */
void net_iface_init(void);

/**
 * @brief Network interface event handle
 * 
 * This handle is generated by net_iface_register_events() and is used to reference a particular
 * event callback instance in successive API calls.
 */
typedef struct net_iface_event_node_s *net_iface_event_handle_t;
typedef uint32_t net_iface_event_flags_t;
typedef void (*net_iface_event_func_ptr_t)(net_iface_event_flags_t events, void* context);

/**
 * @brief Register for notification of network interface events.
 * 
 * @param events One or more NET_IFACE_EVENT_X event flags logically OR'ed together.
 * 
 * @param event_cb Pointer to a function to be called whenever one of the requested events has
 *        occurred.
 * 
 * @param data Opaque user context to be passed to the registered callback during event
 *        notification.
 * 
 * @return Opaque handle used to reference a particular event callback instance in successive API
 *         calls, or NULL if event registration fails.
 * 
 * The registered function callback is used by the network interface layer to notify a higher layer
 * of changes in the network interface's configuration status or warn of an imminent shutdown of the
 * network interface.
 */

net_iface_event_handle_t net_iface_register_events(
    unsigned int if_index,
    net_iface_event_flags_t events,
    net_iface_event_func_ptr_t event_cb,
    void *data);

/**
 * @brief Deregister from notification of network interface events.
 * 
 * @param hndl Opaque handle returned after an earlier a call to net_iface_register_events(), used
 *        to reference a particular event callback instance.
 * 
 * After return from this call, the net_iface_event_handle_t hndl referenced by the caller is no
 * longer valid.
 */
void net_iface_deregister_events(net_iface_event_handle_t hndl);

/**
 * @brief Signal the network interface layer to proceed with a pending IPv4/IPv6 shutdown.
 * 
 * @param hndl Opaque handle returned after an earlier a call to net_iface_register_events(), used
 *        to reference a particular event callback instance.
 * 
 * @param events The corresponding NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING and/or
 *        NET_IFACE_EVENT_IPV6_SHUTDOWN_PENDING event(s) to which this call is responding. Multiple
 *        events should be logically OR'ed together.
 * 
 * An internally triggered shutdown of an IPv4 and/or IPv6 network interface will be delayed until
 * all user callback instances that registered to receive the NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING
 * and/or NET_IFACE_EVENT_IPV6_SHUTDOWN_PENDING event(s), respectively, have been notified of the
 * pending shutdown and a subsequent call to net_iface_continue_shutdown() has been made for each
 * corresponding callback instance to acknowledge the event(s).
 * 
 * Delaying the IPv4/IPv6 network interface shutdown gives overlying network applications an
 * opportunity to complete any final communication over the network interface, e.g. to announce that
 * network services provided by/through this device will no longer be available on the network or to
 * release network resources that had been previously acquired by this device.
 * 
 * Note that an externally triggered shutdown of the network interface (e.g. caused by an Ethernet
 * cable unplug or Wi-Fi Access Point power down) provides no advance warning and will therefore not
 * generate a shutdown event.
 */
void net_iface_continue_shutdown(net_iface_event_handle_t hndl, net_iface_event_flags_t events);


#define NET_IFACE_INVALID_ENUM     (-1)

/*
 *  returns number of interfaces actually enumerated
 * 
 *  NOTE: this value could be less than NET_IFACE_ENUM_COUNT defined by OEM config, e.g.
 *  if one or more interfaces fail to enumerate
 */
uint8_t net_iface_enum_get_count(void);

/*
 *  returns zero-origin interface enumeration value corresponding to the specified
 *  if_index value, or NET_IFACE_INVALID_ENUM if the if_index does not refer to a valid
 *  enumerated network interface
 */
uint8_t net_iface_enum_from_if_index(unsigned int if_index);

/*
 *  returns the if_index value corresponding to the specified zero-origin interface
 *  enumeration value 
 */
unsigned int net_iface_enum_to_if_index(uint8_t iface_enum);

/*
 *  returns the if_index value corresponding to the specified socket descriptor
 */
unsigned int net_iface_sock_to_if_index(int socket);

/*
 *  returns the if_index value corresponding to the specified Treck interface handle
 */
unsigned int net_iface_treck_interface_to_if_index(ttUserInterface treck_interface);

/*
 *  returns the (local oem) internal interface associated with if_index
 */
struct net_interface_s* net_iface_if_index_to_local_context(unsigned int if_index);

/*
 *  returns the (local internal) interface context associated with interface name
 */
struct net_interface_s* net_if_name_to_local_context(char* if_name);

/*
 *  returns the net_interface_t* ref corresponding to the specified if_index value
 *      NULL if not associated
 */
struct net_interface_s* net_iface_context_from_if_index(int if_index);

#ifdef HAVE_WSD
void net_wsd_shutdown( void );
#endif


// cmd & debug api
void register_net_iface_cmds(void);
void register_net_cmds(void);
void show_intfs(void);

// general api
void net_iface_task(unsigned long);
bool intf_up(char* interface_by_name);
bool intf_active(char* interface_by_name);
int  refresh_iface_bindings(void); // returns current os-interface inventory count

#endif // NET_IFACE_API_H

//eof net_iface_api.h
