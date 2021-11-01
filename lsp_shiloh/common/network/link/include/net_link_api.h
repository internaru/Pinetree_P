/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _NET_LINK_API_HEADER_
#define _NET_LINK_API_HEADER_

// public headers
#include <string.h>
#include <memAPI.h>
#include "error_types.h"
#include "os_network.h"
#include "error_types.h"
#include "net_link_enum.h"
#include "delay_api.h" // delay_msec_timer_t as net_timer_handle_t

#define NET_LINK_NAME_SIZE 16
#define MAC_ADDR_BYTE_SIZE 6

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

extern const char* g_net_link_state_strs[];
extern const char* g_net_link_msg_strs[];

//=========================================================================================================================
// link notification delay
//=========================================================================================================================

#define LINK_DOWN_NOTIFY_DELAY_MSEC         2200
/*  When a link-down event occurs on a bound and configured link, the netdrvr state machine will
 *  delay closing the interface for LINK_DOWN_NOTIFY_DELAY_MSEC milliseconds.  If the link
 *  comes back up before the timeout then the interface will remain in the configured
 *  state. Otherwise the interface will be closed.  The delay helps to filter out link
 *  up/down glitches that would otherwise trigger a lengthy reconfiguration sequence.
 */

#define LINK_UP_NOTIFY_DELAY_SEC            2
/*  When a lower priority link-up event occurs and no links are currently bound to Treck interface,
 *  the link event handler will delay LINK_UP_NOTIFY_DELAY_SEC seconds before generating a link-up
 *  message. The delay helps avoid race conditions that would otherwise result in link up/down
 *  glitches when multiple links are enabled simultaneously (e.g. during boot up), and a lower
 *  priority link acquires link status faster than a higher priority link.
 */

#define EXCLUSIVE_LINK_ENABLE_DELAY_SEC     (LINK_UP_NOTIFY_DELAY_SEC+2)
/*  When a higher priority link is up, lower priority links that are registered as "exclusive" are
 *  disabled, e.g. in an effort to conserve power.  When a higher prioriy link goes down, the
 *  netdrvr state machine will delay EXCLUSIVE_LINK_ENABLE_DELAY_SEC seconds before sending an
 *  "Enable" notification to underlying link drivers.  The delay helps avoid enable/disable glitches
 *  caused when a higher priority link goes down and back up in short period of time.
 */

typedef enum
{
    NET_LINK_STATUS_DOWN,
    NET_LINK_STATUS_UP
} net_link_status_t;

/**
 * @brief Network link registration flags
 *
 * Use the NET_LINK_FLAG_X macros to access the bit fields of this uint32_t. A user can 
 * use these flags to toggle various options during link registration. 
 */
typedef uint32_t net_link_flags_t;

/**
 * @name Network interface link flag options
 *
 * Use these NET_LINK_FLAG_X macros to access the bit fields of net_link_flags_t.
 */


#define NET_LINK_FLAG_NONE                      0x00000000

/**
 * @brief Shared exclusive interface binding.
 * 
 * This flag modifies behavior of the link such that the link is disabled whenever another
 * higher-priority link is bound to the interface. Meaningful only when the overlying 
 * network interface is shared with other links and ignored otherwise. Note that only the 
 * highest priority link in an "up" state will be bound to the shared interface regardless 
 * of which links have been registered with this flag set. 
 *  
 * The link layer will receive the IOCTLs NET_LINK_IOCTL_ENABLE and NET_LINK_IOCTL_DISABLE
 * via its registered IOCTL handler to signal the link when to enable and disable itself,
 * respectively.
 * 
 * For example, this property might be assigned to a link with high power consumption on a
 * device with a single shared network interface, allowing the link hardware to be powered
 * off when a higher-priority link is bound to the interface.
 */
#define NET_LINK_FLAG_SHARED_EXCLUSIVE          0x00000001

/**
 * @brief Delay link down notification to filter out up/down glitches
 */
#define NET_LINK_FLAG_DEBOUNCE_ENABLE           0x00000002


/**
 * @brief Network link priority
 *
 * Link priority relative to other registered links sharing the same interface. When 
 * multiple registered links are up (i.e. have acquired link status) on a shared network 
 * interface, only the highest priority link will be bound to the overlying interface.The 
 * NET_IFACE_LINK_PRIORITY_X macros define valid values for this uint8_t. 
 */
typedef uint8_t link_priority_t;

/**
 * @name Network link priority values
 *
 * Use these NET_IFACE_LINK_PRIORITY_X macros to assign or test the values of 
 * link_priority_t (highest priority = 0). 
 * 
 * @note Every link enumerated on a shared interface must be assigned a unique 
 *       link_priority_t value per interface during link registration. No two links on the
 *       same interface may be registered with the same priority. Meaningful only when the
 *       overlying network interface is shared with other links and ignored otherwise
 */

#define NET_LINK_PRIORITY_NONE      254 ///< Assign to links that do not share an interface.

#define NET_LINK_PRIORITY_LOWEST    255 ///< Reserved value for lowest possible priority


/**
 * @name Network interface IOCTLs
 * 
 * The following IOCTLs are an extension of the Treck IOCTLs defined in trsocket.h.  Each underlying
 * link must register a ttDevIoctlFuncPtr function callback capable of processing all pertinent
 * IOCTLs.
 */

#define NET_LINK_IOCTL_BASE             0x0000

/**
 * @brief Instructs the link to check for any completed network packet send operations and to notify
 *        the overlying Treck stack of send completions as described in the Treck documentation.
 *        This IOCTL can be ignored by the link if an alternate technique is used to notify the
 *        Treck stack of send completions. Note that the Treck stack must not be notified of send
 *        completions from the context of an ISR.
 */
#define NET_LINK_IOCTL_SEND_COMPLETE    (NET_LINK_IOCTL_BASE + 0x0001)

/**
 * @brief Instructs the link to enable itself and attempt to acquire link status. Sent only to links
 *        registered as "exclusive" via net_iface_register_link(). Non-exclusive links can ignore
 *        this IOCTL.
 */
#define NET_LINK_IOCTL_ENABLE           (NET_LINK_IOCTL_BASE + 0x0002)

/**
 * @brief Instructs the link to disable itself. Sent to links registered as "exclusive" via
 *        net_iface_register_link() when a higher priority link has come up (i.e. has acquired link
 *        status). Non-exclusive links can ignore this IOCTL.
 */
#define NET_LINK_IOCTL_DISABLE          (NET_LINK_IOCTL_BASE + 0x0004)

/**
 * @brief Instructs the link to proceed with a pending link down operation. After notifying the
 *        overlying network interface of a NET_LINK_EVENT_DOWN_PENDING event, a link must wait
 *        for this IOCTL before completing the link down operation.
 */
#define NET_LINK_IOCTL_DOWN_ACK         (NET_LINK_IOCTL_BASE + 0x0008)


/**
 * @brief Network Link events
 * 
 * A network link generates these events to signal changes in link status, to warn that an
 * internally triggered loss of link status is imminent, or to request a network reset following
 * link layer configuration changes. To receive these events, the network interface registers a
 * callback function pointer of type net_iface_link_event_func_ptr_t when calling the link's
 * initialization callback (included in the link's net_iface_link_func_table_t dispatch table).
 */
typedef enum
{
    NET_LINK_EVENT_NONE = 0,

    /**
     * A link layer issues the this event to notify the overlying network interface layer that an
     * internally triggered link down operation is imminent, e.g. following a link-specific
     * configuration change. The link driver should delay the link down operation until the stack
     * responds with NET_LINK_IOCTL_DOWN_ACK via the link's registered IOCTL handler.
     */
    NET_LINK_EVENT_DOWN_PENDING,

    /**
     * A link layer issues the this event to notify the overlying network interface layer that the link
     * has gone down.
     * 
     * If NET_LINK_EVENT_DOWN follows a NET_LINK_IOCTL_DOWN_ACK IOCTL notification and the
     * link is currently bound to the interface, then a close of the network interface is 
     * assured even if a subsequent NET_LINK_EVENT_UP is received before the close is 
     * initiated. Otherwise, the close of the network interface will be delayed by 
     * LINK_DOWN_NOTIFY_DELAY_SEC in attempt to filter out glitches in link up/down status 
     * (i.e. a debouncing filter).  If the link is currently bound to the interface and 
     * the event results in a close of the interface, then the link driver will receive a 
     * subsequent call to its registered 'close' callback. 
     */
    NET_LINK_EVENT_DOWN,

    /**
     * A link layer issues the this event to notify the overlying network interface layer that
     * the link has recently come up and is ready to transmit data.
     *
     * If the link is the highest priority link currently up, then it will be bound to the overlying
     * network interface. Note that NET_LINK_EVENT_UP will not necessarily result in the link
     * being bound to the interface (e.g. if higher priority link is already up). The link may become
     * bound to the interface anytime following the NET_LINK_EVENT_UP notification, at which time
     * the link layer will receive a call to its registered 'open' callback.
     */
    NET_LINK_EVENT_UP,

    /** added presence-detect event, e.g, wifi-module-on-usb plugin */
    NET_LINK_EVENT_HW_PRESENCE,

    /**
     * A link layer issues the this event to request a reset of the overlying network interface. A reset
     * forces upper layers to acknowledge link-specific configuration changes that do not otherwise
     * require the physical link to go down and come back up, e.g. after re-assigning/cloning a MAC
     * address. If the link is bound to the interface then it will remain bound while the interface is
     * closed and reopened.
     *
     * The link layer will be notified of reset start and completion via calls to its registered
     * 'close' and 'open' callbacks, respectively.  If the link is not currently bound to the overlying
     * network stack then the request will simply be ignored.  The link layer should make no assumptions
     * about whether or not it is currently bound to the network interface, even if the link is up and
     * the link layer has notified the network interface layer of its transition to this state via the
     * NET_LINK_EVENT_UP event (e.g. a higher priority link could already be bound to the interface).
     *
     * NOTE: If the physical link is guaranteed to remain up following a configuration change, then
     * issuing NET_LINK_EVENT_RESET is the most efficient way to notify the stack.  This is
     * because lower priority links will not have an opportunity to bind to the interface only to be
     * torn down again when the higher priority link comes back up. However, if the physical link will
     * be going down and back up as a result of the configuration change, the link driver must not issue
     * NET_LINK_EVENT_RESET. Otherwise a race condition will result in which the interface could
     * reopen on a link that has not yet come back up.
     */
    NET_LINK_EVENT_RESET

} net_link_event_t;

//=========================================================================================================================
// State machine
//=========================================================================================================================

/*
 *  message queue
 */

#define NET_MAC_THREAD_NAME             "link-layer"
#define NET_LINK_MQ_NAME               "/link-layer msgq"
#define NET_LINK_MQ_NUM_MSG_BUFS       8

/* message format */
#define NET_LINK_MQ_MSG_BUF_SIZE       OS_4_ULONG // this value must be >= sizeof(net_link_msg_t)
typedef struct net_link_msg_s
{
    net_link_msg_type_t     type;
    struct net_link_s       *link;
    void                    *data1;
    void                    *data2; // type-specific message data
} net_link_msg_t;

/* message queue buffer */
#define NET_LINK_MQ_MSG_BUF_BYTES      (NET_LINK_MQ_MSG_BUF_SIZE*sizeof(unsigned long))
typedef union
{
      struct net_link_msg_s msg;
      uint8_t buf[NET_LINK_MQ_MSG_BUF_BYTES]; // forces net_link_msg_t size into valid os queue size option
} net_link_msg_buf_t;

//=========================================================================================================================
// Registered link driver context
//=========================================================================================================================

/**
 * @brief Network link layer event notification function pointer
 * 
 * This function is implemented at the network interface layer and is registered with an underlying
 * link by calling the link's net_iface_link_init_func_ptr_t initialization callback (included in
 * the net_iface_link_func_table_t dispatch table).
 */
typedef void (*net_link_event_notify_func_t)(net_link_event_t link_event, void *user_data);

/**
 * @brief Network link layer event registration function pointer
 * 
 * This function is implemented at the link layer and is called by the overlying network interface
 * layer to initialize a registered link.
 */
typedef void (*net_link_event_register_func_t)(net_link_event_notify_func_t notify_func, void *user_data);

typedef struct 
{
    net_link_event_register_func_t  event_register; ///< Populate with a link event registration routine (required).
    ttDevOpenCloseFuncPtr           open; ///< Populate with an "open" handler as defined by the Treck API (required).
    ttDevOpenCloseFuncPtr           close; ///< Populate with a "close" handler as defined by the Treck API (required).
    ttDevSendFuncPtr                send; ///< Populate with a "send" handler as defined by the Treck API (required).
    ttDevRecvFuncPtr                recv; ///< Populate with a "recv" handler as defined by the Treck API (required).
    ttDevIoctlFuncPtr               ioctl; ///< Populate with an "IOCTL" handler as defined by the Treck API (required).
    ttDevGetPhysAddrFuncPtr         get_phys_addr; ///< Populate with a "get physical address" handler as defined by the Treck API (required).
} net_link_api_funcs_t;

#include "net_iface_config.h" // oem_config -- needs net_link_api_funcs_t
#include "net_link_config.h"

typedef struct net_link_s
{
    struct net_link_s       *next; // next network link in the logical link list

    net_link_api_funcs_t    api_funcs;
    uint8_t                 oem_enum; // enumeration index assigned via OEM config file
    char                    name[NET_LINK_NAME_SIZE];
    net_link_flags_t        flags;
    link_priority_t         priority;
    bool                    enabled;
    link_state_t            state; // private internal state
    net_timer_handle_t      link_up_timer;
    net_timer_handle_t      link_down_timer;
    struct net_link_logical_s *logical_link; // logical link encapsulating this link
} net_link_t;

typedef struct net_link_logical_s
{
    struct net_link_logical_s *next; // next logical_s link

    uint8_t                 logical_enum;
    uint8_t                 link_cnt; // physical link count
    net_link_t              *links; // physical links sorted in order of decreasing priority (i.e. list head == highest priority)
    bool                    have_exclusive_link;
    net_link_t              *priority_link_up;
    net_link_t              *bound_link; // a shared link is bound when the overlying driver calls net_link_shared_open
    net_link_status_t       status; // user-facing public link status
    net_timer_handle_t      timer_hndl_enable; // exclusive link enable delay timer
    struct net_link_event_node_s *event_nodes; // linked list
    void                    *user_data;
} net_link_logical_t;


typedef struct 
{
    ttDevOpenCloseFuncPtr           open; ///< Populate with an "open" handler as defined by the Treck API (required).
    ttDevOpenCloseFuncPtr           close; ///< Populate with a "close" handler as defined by the Treck API (required).
    ttDevSendFuncPtr                send; ///< Populate with a "send" handler as defined by the Treck API (required).
    ttDevRecvFuncPtr                recv; ///< Populate with a "recv" handler as defined by the Treck API (required).
    ttDevIoctlFuncPtr               ioctl; ///< Populate with an "IOCTL" handler as defined by the Treck API (required).
    ttDevGetPhysAddrFuncPtr         get_phys_addr; ///< Populate with a "get physical address" handler as defined by the Treck API (required).
} net_link_bindings_t;

typedef struct
{
    uint8_t                 link_cnt;
    net_link_t              *links[NET_MAX_PHYSICAL_LINKS];
    uint8_t                 logical_link_cnt;
    net_link_logical_t     *logical_links[NET_MAX_INTERFACES];
    bool                    enumerated;
    pthread_mutex_t         mtx;
    net_link_oem_func_table_t oem_func_table;
    net_link_oem_config_t   oem_config;
} net_link_context_t;


/**
 * @brief Network link event handle
 * 
 * This handle is generated by net_link_register_events() and is used to reference a
 * particular event callback instance in successive API calls.
 */
typedef struct net_link_event_node_s *net_link_event_handle_t;

#define NET_LINK_EVENT_INVALID_HANDLE       (NULL)

/**
 * @brief Network link-layer interface function table
 *
 * This function table represents the public interface of an abstract base class for a 
 * network link object. The function table is populated by the net_link_oem_init_func_t 
 * system initialization routine defined by each network link module instance. Following 
 * link initialization, any interaction with the link must occur via this function table. 
 * The table includes function pointer types specific to the Treck network stack API 
 * (www.treck.com) and is intended for use only with an overlying Treck stack. 
 */

#define NET_LINK_NAME_SIZE          16 ///< 15 plus terminating NULL

#define NET_LINK_INVALID_ENUM       (-1)

#define NET_LINK_INVALID_INDEX      (0)

#define NET_LINK_INVALID_HANDLE     (-1)

//=========================================================================================================================
// Network link event registration
//=========================================================================================================================

typedef struct net_link_event_node_s
{
    struct net_link_event_node_s    *next;
    net_link_event_notify_func_t    notify_func;
    net_link_logical_t             *logical_link;
    void                            *user_data;
} net_link_event_node_t;

/*
 *  Handle to a logical network link. 
 * 
 *  Note the distinction between logical links and physical links.  Logical links may
 *  encapsulate one or more physical links that share the same network interface.
 */
typedef unsigned int net_link_handle_t;

//=========================================================================================================================
// Module context
//=========================================================================================================================

extern net_link_context_t g_net_link_ctxt;
extern net_link_msg_buf_t g_net_link_msg_buf[NET_LINK_MQ_NUM_MSG_BUFS]; // msg queue buffer space
extern mqd_t              g_net_link_msg_queue; // msg queue control block

// local link management routines
void net_link_init(void); // link-layer
void net_link_event_handler_cb(net_link_event_t link_event, void *user_data);

// local utility routines
void *net_link_get_user_data(net_link_handle_t link_handle);
void net_link_set_user_data(net_link_handle_t link_handle, void *user_data);
unsigned int net_link_name_to_index(const char *link_name);
net_rcode_t net_link_send_msg_wait_opt(
    net_link_msg_type_t type,
    net_link_t *link,
    void *data1,
    void *data2,
    unsigned long os_wait_opt);

net_link_t *net_link_lookup_by_name(const char *link_name);
net_link_t *net_link_lookup_by_link_index(unsigned int link_index);

// network link event registration
void net_link_notify(net_link_logical_t *logical_link, net_link_event_t link_event);

/* 
 *  Get link name of the specified physical network link.  The returned link name will be
 *  no larger than NET_LINK_NAME_SIZE (including null terminator).
 * 
 *  IMPORTANT: note that link_index used at public API layer is offset by one (1-based)
 *  vs. OEM-assigned oem_enum index (0-based).  This is to maintain consistency with
 *  official BSD socket API if_index (also 1-based)
 */
void net_link_get_name(unsigned int link_index, char *link_name, uint8_t len);

/* 
 *  Requests 1-based link-index of the physical network link currently active for the
 *  specific logical network link.
 * 
 *  Note the distinction between logical links and physical links.  Logical links may
 *  encapsulate one or more physical links that share the same network interface.
 * 
 *  @return Link index (i.e.link_index), or 0 if no link is currently active and bound to
 *  the interface.
 */
error_type_t net_link_get_active_link(unsigned int link_handle, unsigned int* link_index);

/* 
 *  IMPORTANT: note that link_index used at public API layer is offset by one (1-based)
 *  vs. OEM-assigned oem_enum index (0-based).  This is to maintain consistency with
 *  official BSD socket API if_index (also 1-based)
 */
net_rcode_t net_link_get_phys_addr(unsigned int link_index, uint8_t *phys_addr, uint8_t len);

net_link_status_t net_link_get_status(net_link_handle_t link_handle);

/*
 *  Enumeration of logical network links, which map one-to-one with network interfaces.
 * 
 *  Note the distinction between logical links and physical links.  Logical links may
 *  encapsulate one or more physical links that share the same network interface.
 */
net_link_handle_t net_link_get_next_logical_link(net_link_handle_t link_handle);

/*
 *  Get function bindings used to attach a logical network link to a network interface.
 */
void net_link_get_bindings(net_link_handle_t link_handle, net_link_bindings_t *bindings);

/*
 *  Get the enumeration value of the logical network link corresponding to link_handle.
 */
uint8_t net_link_get_enum(net_link_handle_t link_handle);

net_link_event_handle_t net_link_register_events(
    net_link_handle_t link_handle,
    net_link_event_notify_func_t notify_func,
    void *user_data);
void net_link_deregister_events(net_link_event_handle_t event_handle);

/*
 *  Returns number of physical links enumerated on the device.
 * 
 *  NOTE: this value could be less than NET_LINK_ENUM_COUNT defined by OEM config, e.g.
 *  if one or more interfaces fail to enumerate
 */
uint8_t net_link_enum_get_count(void);

/*
 *  Returns the zero-origin enumeration value of the physical network link corresponding
 *  to the specified link_index value, or NET_LINK_INVALID_ENUM if the link_index does not
 *  refer to a valid enumerated network link.
 * 
 *  Note the distinction between physical links and logical links.  Logical links may
 *  encapsulate one or more physical links that share the same network interface and are
 *  enumerated using net_link_get_next_logical_link().
 */
uint8_t net_link_enum_from_link_index(unsigned int link_index);

/*
 *  Returns the link_index value of the physical network link corresponding to the
 *  specified zero-origin enumeration value, or 0 if the enumeration value does not refer
 *  to a valid enumerated physical network link.
 * 
 *  Note the distinction between physical links and logical links.  Logical links
 *  encapsulate one or more physical links that share the same network interface and are
 *  enumerated using net_link_get_next_logical_link().
 */
unsigned int net_link_enum_to_link_index(uint8_t link_enum);


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_LINK_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_lock(&g_net_link_ctxt.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#define NET_LINK_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_unlock(&g_net_link_ctxt.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

// called from threads
#define net_link_send_msg(type, link, data1, data2) \
    net_link_send_msg_wait_opt(type, link, data1, data2, POSIX_WAIT_FOREVER)

// called from non-threads (e.g. timers)
#define net_link_send_msg_no_wait(type, link, data1, data2) \
    net_link_send_msg_wait_opt(type, link, data1, data2, 0)

// Global entrypoints
void net_link_init(void); // link-layer (phy/mac layer)

// api
error_type_t net_link_get_active_link(unsigned int link_handle, unsigned int* link_index);
net_link_logical_t *net_link_logical_lookup_by_enum(uint8_t llink_enum);

// general timer prototype
typedef void (*general_timer_cb)(union sigval sig);
int start_ms_timer(timer_t* timer, long ms_delay, general_timer_cb timer_cb);
int start_ms_timer_periodic(timer_t* timer, long ms_delay, general_timer_cb timer_cb);

// socket-layer queries
bool intf_up(char* intf_name); // os-socket determined (flags & IFF_UP)
bool intf_active(char* intf_name); // os-socket determined (flags & IFF_RUNNING)

void register_net_cmds(void); // instantiate 'net' cmd interface

#endif // _NET_LINK_API_HEADER_

//eof
