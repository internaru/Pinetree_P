/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_link_events.c
  * 
  *  This file contains wifi-link specifics, as well as general network interface
  * events.
  * 
  */

 /* More reading for netlink socket data and events */
 /* For more informations on some functions :
  * http://www.kernel.org/doc/man-pages/online/pages/man3/netlink.3.html
  * http://www.kernel.org/doc/man-pages/online/pages/man7/rtnetlink.7.html
  */

 // see also .../wlan_src/mapp/mlan2040coex/mlan2040coex.c (custom events)

#ifdef __cplusplus
extern "C" {
#endif

// public headers
#include <string.h>
#include <stdint.h>

// os-headers -- influenced by wireless inclusion
#ifdef HAVE_WIRELESS
#define DO_NOT_INCLUDE_IF_H /* .../if.h supplied w/wireless.h */
#endif

// common headers
#include "error_types.h"
#include "os_network.h" // treck-port includes
#include "net_link_api.h"
/// lsptodo : compiler / clibrary version 
unsigned int if_nametoindex(const char* if_name); // revisit 
//#include <net/if.h>
#include "net_iface.h"

// more os-headers -- influenced by wireless inclusion
#ifdef HAVE_WIRELESS
#include <linux/wireless.h> /* includes <linux/netlink.h> */
#else
// piece together what <wireless.h> otherwise provides ..
#include <linux/if_link.h> // IFLA_IFNAME

#define IW_EV_LCP_LEN 4

// missing IFF_ flags from <net/if.h> ..
#define IFF_LOWER_UP	0x10000		/* driver signals L1 up		*/
#define IFF_DORMANT	0x20000		/* driver signals dormant	*/
#define IFF_ECHO	0x40000		/* echo sent packets		*/

/* RFC 2863 operational status */ //see also #include <linux/if.h> // IF_OPER_UP/DOWN
enum {
	IF_OPER_UNKNOWN,
	IF_OPER_NOTPRESENT,
	IF_OPER_DOWN,
	IF_OPER_LOWERLAYERDOWN,
	IF_OPER_TESTING,
	IF_OPER_DORMANT,
	IF_OPER_UP,
};
#endif // HAVE_WIRELESS

#include <linux/rtnetlink.h> // RTMGRP_LINK

// custom event signatures (empirically determined)
#define CUS_EVT_CONFIG_CHANGE "EVENT=WMM_CONF_CHANGE.indication"
#define CUS_EVT_OBSS_SCAN_PARAM "EVENT=OBSS_SCAN_PARAM"
#define UAP_DISCONNECT "EVENT=STA_DISCONNECTED"
#define STA_DISCONNECT "Disconnected from AP"

// netlink dbg - nlmsg headers etc..
typedef struct nlmsghdr nlmsghdr_t;
typedef struct ifinfomsg ifinfomsg_t;
typedef struct rtattr rtattr_t; // msg attr

// Debug configuration
#define DBG_PRFX "net.link.event: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros
#define DBG_INFO DBG_MSG


// local debug
#define EXTRA_LOCAL_DEBUG 0 /* additional dbg when enabled (non-zero) */
#if EXTRA_LOCAL_DEBUG // debug
#define adv_state(link, updated_state) \
{ char* link_name = (link?link->name:"<unbound link>"); dbg_printf("adv(link %s x%x state %d -> %d)\n", link_name, link, link->state, updated_state); link->state = updated_state; }
#define adv_status(llink, updated_status) \
{ dbg_printf("adv(llink x%x state %d -> %d)\n", llink, llink->status, updated_status); llink->status = updated_status; }
#define DBG_UNEXPECTED dbg_printf
#else
#define adv_state(link, updated_state)     link->state = updated_state
#define adv_status(llink, updated_status) llink->status = updated_status
#define DBG_IGNORED(...) { (void)0; } /* optimize out */
#define DBG_UNEXPECTED(...) { (void)0; } /* optimize out */
#endif


#ifdef HAVE_WIRELESS


#endif // HAVE_WIRELESS

#undef DBG_IGNORED
#if EXTRA_LOCAL_DEBUG
#define DBG_IGNORED DBG_MSG
#else
#define DBG_IGNORED DBG_VERBOSE
#endif


// net link event monitor
extern mqd_t g_net_link_event_msg_queue; // link-event msg queue control block

// create an AF_NETLINK event registration/notification handler
error_type_t open_net_event_handler(int* socket_fd)
{
    DBG_ASSERT(socket_fd);
    if (!socket_fd) return FAIL;

    // create PF_NETLINK socket
    *socket_fd = -1;
    int socket_opts = NETLINK_ROUTE;
    int netlink_socket = socket(AF_NETLINK, SOCK_DGRAM | O_CLOEXEC, socket_opts);
    if (netlink_socket < 0)
    {
        DBG_ERR("%s() '%s' (err %d %d)\n", __func__, strerror(thread_errno), thread_errno, netlink_socket);
        return FAIL;
    }
    
    // bind AF_NETLINK address
    struct sockaddr_nl addr = {0};
    addr.nl_pid    = getpid();
    addr.nl_groups = RTMGRP_LINK; // RTM_DELLINK RTM_GETLINK RTM_NEWLINK
    //addr.nl_groups |= RTMGRP_IPV4_IFADDR; // RTM_DELADDR RTM_GETADDR RTM_NEWADDR
    //addr.nl_groups |= RTMGRP_IPV4_ROUTE; // RTM_DELROUTE RTM_GET_ROUTE RTM_ADDROUTE
    //addr.nl_groups |= RTMGRP_NOTIFY;
    addr.nl_family = AF_NETLINK;
  
    int bound_fd = bind(netlink_socket, (struct sockaddr*)&addr, sizeof(addr));
    if (bound_fd < 0)
    {
        DBG_ERR("%s() bind '%s' (err %d)\n", __func__, strerror(thread_errno), bound_fd);
        close(netlink_socket);
        return FAIL;
    }

    *socket_fd = netlink_socket; // success
    DBG_VERBOSE("%s() using %d\n", __func__, *socket_fd);
    return OK;
}

// shutdown netlink listener socket
error_type_t close_net_event_handler(int socket_fd)
{
    DBG_VERBOSE("%s(%d)\n", __func__, socket_fd);
    if (socket_fd>=0) close(socket_fd);
    return (socket_fd >= 0 ? OK : FAIL);
}



#define str_type_literal(type) case type: return #type;
char* str_type(int type)
{
static char str_unknown[32] = { "<unknown type %d>" };
    switch(type)
    {
        str_type_literal(IFLA_UNSPEC);
        str_type_literal(IFLA_ADDRESS);
        str_type_literal(IFLA_BROADCAST);
        str_type_literal(IFLA_IFNAME);
        str_type_literal(IFLA_MTU);
        str_type_literal(IFLA_LINK);
        str_type_literal(IFLA_QDISC);
        str_type_literal(IFLA_STATS);
        str_type_literal(IFLA_COST);
#define IFLA_COST IFLA_COST
        str_type_literal(IFLA_PRIORITY);
#define IFLA_PRIORITY IFLA_PRIORITY
        str_type_literal(IFLA_MASTER);
#define IFLA_MASTER IFLA_MASTER
        str_type_literal(IFLA_WIRELESS);
#define IFLA_WIRELESS IFLA_WIRELESS
        str_type_literal(IFLA_PROTINFO);
#define IFLA_PROTINFO IFLA_PROTINFO
        str_type_literal(IFLA_TXQLEN);
#define IFLA_TXQLEN IFLA_TXQLEN
        str_type_literal(IFLA_MAP);
#define IFLA_MAP IFLA_MAP
        str_type_literal(IFLA_WEIGHT);
#define IFLA_WEIGHT IFLA_WEIGHT
        str_type_literal(IFLA_OPERSTATE);
        str_type_literal(IFLA_LINKMODE);
        str_type_literal(IFLA_LINKINFO);
#define IFLA_LINKINFO IFLA_LINKINFO
        str_type_literal(IFLA_NET_NS_PID);
        str_type_literal(IFLA_IFALIAS);
        str_type_literal(IFLA_NUM_VF);
        str_type_literal(IFLA_VFINFO_LIST);
        str_type_literal(IFLA_STATS64);
        str_type_literal(IFLA_VF_PORTS);
        str_type_literal(IFLA_PORT_SELF);
        str_type_literal(IFLA_AF_SPEC);
        str_type_literal(IFLA_GROUP);
        str_type_literal(IFLA_NET_NS_FD);
      default:
        sprintf(str_unknown, str_unknown, type);
    }
    return str_unknown;
}
#if EXTRA_LOCAL_DEBUG || defined DBG_VERBOSE_ON
#endif // DBG_VERBOSE_ON

// dbg - traverse attr list
char* walk_ifla_attrs(char* link_name, void* payload, int payload_overall_len, int* event)
{
    if (!payload) return NULL;
    if (!link_name) link_name = "<unknown intf>";
    int16_t bytes_remaining = (int16_t)(*(uint16_t*)payload);

    ifinfomsg_t* info = (ifinfomsg_t*)((uint32_t)payload + sizeof(nlmsghdr_t));
    DBG_VERBOSE("info: family %d type %2d seq %d dflags x%x\n",
        info->ifi_family, info->ifi_type, info->ifi_index, info->ifi_flags);
    DBG_VERBOSE(" IFF flags ");
    if (info->ifi_flags & IFF_BROADCAST) DBG_VERBOSE("broadcast ");
    if (info->ifi_flags & IFF_MULTICAST) DBG_VERBOSE("multicast ");
    if (info->ifi_flags & IFF_LOWER_UP) DBG_VERBOSE("layer1 up ");
    if (info->ifi_flags & IFF_RUNNING) DBG_VERBOSE("running ");
    if (info->ifi_flags & IFF_DORMANT) DBG_VERBOSE("dormant ");
    DBG_VERBOSE("\n");

    ifinfomsg_t* attribute_set = (ifinfomsg_t*) ((uint32_t)payload + sizeof(nlmsghdr_t));
    struct rtattr* attr = NULL;

    bytes_remaining = payload_overall_len;
    DBG_VERBOSE("%s %s(x%x) %d byte payload\n", link_name, __func__, payload, payload_overall_len);

    // travel attribute list (until ! OK)
    for (attr = IFLA_RTA(attribute_set); RTA_OK(attr, bytes_remaining); attr = RTA_NEXT(attr, bytes_remaining))
    {
        switch (attr->rta_type)
        {
        // display attribute contents
        default:

            //dbg hex_dump((void*)RTA_DATA(attr), attr->rta_len); // noisy

        // display header info only:
        case IFLA_IFNAME:
            DBG_VERBOSE("%s %s(x%x) type %2d (%s) len %d\n", link_name, __func__, attr, attr->rta_type, str_type(attr->rta_type), attr->rta_len); break;

#ifdef HAVE_WIRELESS
        case IFLA_WIRELESS:
            // see also .../wlan_src/mapp/mlan2040coex/mlan2040coex.c
            {
                bool assoc_flag = FALSE;
                bool is_ht_ap = FALSE;
                struct iw_event* iwe;
                int rta_len = IW_EV_LCP_LEN; // revisit RTA_PAYLOAD(attr);
                char* pos = ((char *) attr) + rta_len;
                char* end = pos + (attr->rta_len - rta_len);
                char cur_obss_scan_param[64];

                while (pos + IW_EV_LCP_LEN <= end)
                {
                    iwe = (struct iw_event *) pos;
                    if (iwe->len <= IW_EV_LCP_LEN)
                    {
                        DBG_ERR("%s() short! iw_event x%x %d bytes (<= %d)\n", __func__, iwe, iwe->len, IW_EV_LCP_LEN);
                        break;
                    }

                    switch (iwe->cmd) {
                    case IWEVCUSTOM:
                    {
                        unsigned short dlen;
                        char *custom;
                        custom = pos + IW_EV_POINT_LEN;
                        if (IW_EV_POINT_LEN ==
                            IW_EV_LCP_LEN + sizeof(struct iw_point)) {
                            dlen = iwe->u.data.length;
                        } else {    /* WIRELESS_EXT >= 19 */
                            dlen =
                                *((unsigned short *) (pos + IW_EV_LCP_LEN));
                        }
                        if (custom + dlen > end)
                        {
                            DBG_ERR("%s() iw_event custom '%s' (%d byte) > (end x%x)!\n", __func__, custom, dlen, end);
                            break;
                        }

                        if (!strncmp (CUS_EVT_OBSS_SCAN_PARAM, custom, strlen(CUS_EVT_OBSS_SCAN_PARAM)))
                        {
                            memset(&cur_obss_scan_param, 0, sizeof(cur_obss_scan_param));
                            memcpy(&cur_obss_scan_param,
                                   (custom + strlen(CUS_EVT_OBSS_SCAN_PARAM) + 1), sizeof(cur_obss_scan_param));
                            /** set TRUE, since it is an HT AP */
                            is_ht_ap = TRUE;
                        }

                        if (!strcmp(custom, STA_DISCONNECT))
                        {
                            if (event) *event = NET_LINK_EVENT_DOWN;
                            return "Disconnect";
                        }
                        else 
                        {
                            DBG_VERBOSE("--- %s < %s >---\n", link_name, custom);
                            //dbg DBG_VERBOSE("%s() iw_event pos x%x end x%x %d byte cmd x%x\n", __func__, pos, end, iwe->len, iwe->cmd);
                            //dbg hex_dump((void*)iwe, iwe->len);
                        }

                        pos += iwe->len;
                    } break;

                    case SIOCGIWAP:
                    {
                        typedef struct { uint8_t u8[6]; } ether_addr_t;

                        ether_addr_t* wap;
                        char buf[32];
                        wap = (ether_addr_t*)
                            ((struct sockaddr *) (&iwe->u.ap_addr))->sa_data;
                        ether_addr_t etherzero =
                            { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

                        if (!memcmp(wap, &etherzero, sizeof(ether_addr_t)))
                        {
                            DBG_VERBOSE("---< Disconnected from AP >---\n");
                            memset(&cur_obss_scan_param, 0,
                                   sizeof(cur_obss_scan_param));
                            assoc_flag = FALSE;
                            is_ht_ap = FALSE;
                            if (event) *event = NET_LINK_EVENT_DOWN;
                            return "Disconnect";
                        }
                        else
                        {
                            memset(buf, 0, sizeof(buf));
                            snprintf(buf, sizeof(buf),
                                     "%02X:%02X:%02X:%02X:%02X:%02X",
                                     wap->u8[0],
                                     wap->u8[1],
                                     wap->u8[2],
                                     wap->u8[3],
                                     wap->u8[4],
                                     wap->u8[5]);
                            /** set TRUE, if connected */
                            assoc_flag = TRUE;
                            if (event) *event = NET_LINK_EVENT_UP;
                            return "Connect";
                        }
                        //pos += iwe->len; //UNREACHABLE Code
                    } break;
                    case IWEVEXPIRED:
                        DBG_VERBOSE("%s() %s expired\n", __func__, link_name); break;
   
                    default:
                        pos += iwe->len;
                        break;
                    }
                }
		UNUSED_VAR( is_ht_ap );
		UNUSED_VAR( assoc_flag );
            }
#endif

            DBG_VERBOSE("%s %s(x%x) type %2d (%s) len %d\n", link_name, __func__, attr, attr->rta_type, str_type(attr->rta_type), attr->rta_len); break;

          break;
        }
    }
return NULL;
}

// troll through wireless attributes looking for dis/connect indicators
bool sift_ifla_attrs(char* link_name, void* payload, int payload_overall_len, int* event)
{
    char* str_event = walk_ifla_attrs(link_name, payload, payload_overall_len, event);
    if (str_event != NULL)
    {
        DBG_INFO("%s() %s event %d '%s'\n", __func__, link_name, (event?*event:0), str_event);
        return true;
    }
    return false;
}

// dbg -- dump rta attribute
void show_attr(rtattr_t* attr, char* annotation)
{
    if (annotation) DBG_MSG("%s:\n", annotation);
    //dbg if (attr) hex_dump(attr, attr->rta_len+4);
}

// dbg -- unpack nlmsg
#define str_notification_type(type) case type: sprintf(str_buf, #type); break
char* str_event(event)
{
    static char str_buf[32] = { 0 };
    switch (event)
    {
        str_notification_type(RTM_NEWLINK);
        str_notification_type(RTM_DELLINK);
        str_notification_type(RTM_GETLINK);
        str_notification_type(RTM_NEWADDR);
        str_notification_type(RTM_DELADDR);
        str_notification_type(RTM_GETADDR);
        str_notification_type(RTM_NEWROUTE);
        str_notification_type(RTM_DELROUTE);
        str_notification_type(RTM_GETROUTE);
        default:
            sprintf(str_buf, "<unknown event %d>", event);
    }
    return str_buf;
}

void show_nlmsg(void* given, void* packet, char* link_name)
{
    struct nlmsghdr* buf = given;
#ifdef DBG_VERBOSE_ON
    //dbg if (given) hex_dump(given, 2*sizeof(struct nlmsghdr)); // extra bytes displayed
    int16_t* len_ptr = (int16_t*)&buf;
    int packet_len = *len_ptr;
    UNUSED_VAR(packet_len);
#endif

    // dbg open
    DBG_VERBOSE("nlmsg: '%s' %s (%d) flags x%x len %d (%d byte packet)",
        (link_name ? link_name : "<unknown>"),
        str_event(buf->nlmsg_type), buf->nlmsg_type,
        buf->nlmsg_flags,
        buf->nlmsg_len, packet_len);

    // misc
    if (buf->nlmsg_pid != 0 || buf->nlmsg_seq>0)
    {
        DBG_VERBOSE(" pid %d seq %d", buf->nlmsg_pid, buf->nlmsg_seq);
    }
    // dbg close
    DBG_VERBOSE("\n");

    walk_ifla_attrs(link_name, buf, buf->nlmsg_len, NULL);
}


// tlv (type-length-value) encoding
// return the attribute record corresponding to the requested one
rtattr_t* extract_ifla_attr(void* payload, int payload_overall_len, int attr_type_req) // tlv (type-length-value) encoded
{
    if (!payload) return NULL;
    int16_t bytes_remaining = (int16_t)(*(uint16_t*)payload);
    
    ifinfomsg_t* info = (ifinfomsg_t*) ((uint32_t)payload + sizeof(nlmsghdr_t));
    rtattr_t* attr = NULL;

    bytes_remaining = payload_overall_len;

    // 1st attr
    for (attr = IFLA_RTA(info); RTA_OK(attr, bytes_remaining); attr = RTA_NEXT(attr, bytes_remaining))
    {
        switch (attr->rta_type)
        {
          default:
            if (attr->rta_type == attr_type_req)
            {
#ifdef DBG_VERBOSE_ON // embellish dbg if link name is known
                if (attr_type_req == IFLA_IFNAME)
                {
                    char* link_name = RTA_DATA(attr);
                    UNUSED_VAR(link_name);
                    DBG_VERBOSE("'%s' %s(x%x) type %2d (%s) len %d val x%x\n",
                        link_name, __func__, attr, attr->rta_type, str_type(attr->rta_type), attr->rta_len, link_name);
                }
                else
                {
                    DBG_VERBOSE("       %s(x%x) type %2d (%s) len %d val x%x\n",
                        __func__, attr, attr->rta_type, str_type(attr->rta_type), attr->rta_len, RTA_DATA(attr));
                }
#endif // DBG_VERBOSE_ON
                return attr;
            }
          break;
        }
    }
return NULL;
}


// propogate link events
void net_link_event_handler_cb(net_link_event_t link_event, void *user_data)
{
    error_type_t net_rcode;
    ASSERT(user_data);
    net_link_t *link = (net_link_t *)user_data;

    DBG_VERBOSE("==>%s (link=%s event=%d)\n",__func__, link->name, link_event);

    void *msg_udata = (void *)0;
    void *msg_data2 = (void *)0;
    net_link_msg_type_t msg_type = NET_LINK_NUM_MSGS; 

    switch(link_event)
    {
    case NET_LINK_EVENT_DOWN_PENDING:

        msg_type = NET_LINK_MSG_DOWN_PENDING;
        break;

    case NET_LINK_EVENT_DOWN:

        msg_type = NET_LINK_MSG_DOWN_EVENT;
        break;

    case NET_LINK_EVENT_HW_PRESENCE: // hw presence-detect
        DBG_VERBOSE("==>%s %s hw presence detected\n",__func__, link->name);
        msg_type = NET_LINK_MSG_ENABLE_LINKS; // possibly NET_LINK_MSG_RESET_EVENT?
        msg_udata = user_data; // forward intf/link-of-interest
        break;

    case NET_LINK_EVENT_UP:

        msg_type = NET_LINK_MSG_UP_EVENT;
        break;

    case NET_LINK_EVENT_RESET:
        msg_type = NET_LINK_MSG_RESET_EVENT;
        break;

    default:
        // unexpected link event - ignore
        DBG_UNEXPECTED("'%s' unexpected event %d ignored \n", link->name, link_event);
        XASSERT(0, link_event); 
        break;
    }

    if(msg_type < NET_LINK_NUM_MSGS)
    {
        net_rcode = net_link_send_msg_no_wait(msg_type, link, msg_udata, msg_data2);
        if(net_rcode == NET_WOULD_BLOCK)
        {
            // msg queue full
            DBG_ERR("'%s' link os_queue full while sending msg %s\n", link->name, g_net_link_msg_strs[msg_type]);
            DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
        }
    }

    DBG_VERBOSE("<== %s()\n",__func__);
    return;
}

// notify additional network layer(s) as appropriate
//
//    events are filtered (ignored) for a variety of criteria:
//
// 1 - specified intf is not enabled
// 2 - back-to-back events are folded into a single event
// 3 - other?
//

#define LAST_HISTORICAL_EVENT 3
static struct last_event_s { // persistant
    long at_time;
    net_interface_t* intf;
    net_link_event_t sent;
} event_history[LAST_HISTORICAL_EVENT] = { {0,NULL,-1}, {0,NULL,-1}, {0,NULL,-1} };
timer_t link_event_window_timer;

// netlink event handler
void propogate_link_event(net_link_t* link, net_link_event_t link_event)
{
    static short latest = -1; // advanced -> 0 before 1st usage
    static short previous = 0; // advanced after 1st usage

#ifdef DBG_VERBOSE_ON
    char str_event[16] = { 0 };
    strcat(str_event,
        (link_event==NET_LINK_EVENT_UP?"up":
            (link_event==NET_LINK_EVENT_DOWN?"down":
                (link_event==NET_LINK_EVENT_DOWN_PENDING?"down pending":
                    (link_event==NET_LINK_EVENT_RESET?"up reset":
                        (link_event==NET_LINK_EVENT_HW_PRESENCE?"hw detected":
                "<unknown event>"))))));
#endif

    char* link_name = link ? link->name : "<unbound>";

    // determine last 'event-time' -- filter out back-to-back intf events
    static long previous_event_time = 0;

    latest++; if (latest >= LAST_HISTORICAL_EVENT) latest = 0; // adv history

    net_interface_t* intf = net_iface_lookup_by_name(link_name); // (linux) 1:1 correspondence between intf<->link names
    event_history[latest].intf = intf;

    long current_event_time = posix_gettime_ticks();
    event_history[latest].at_time = current_event_time;

    #define SETTLE_TIME_MS 500 /* half-second */
    #define SETTLE_TIME_TICKS (SETTLE_TIME_MS/10)
    long delta_event_t = current_event_time - previous_event_time;

    previous_event_time = current_event_time;
    if (delta_event_t < SETTLE_TIME_TICKS)
    {
        if (event_history[latest].intf == event_history[previous].intf)
        {
            previous = latest;
            DBG_IGNORED("%s() '%s' event '%s' ignored (dt %d)\n", __func__, link_name, str_event, delta_event_t);
            return;
        }
        // given a different intf, fallthrough
    }
    previous = latest;

    if (intf) // ignore events for disabled interface
    {
        unsigned int if_index = if_nametoindex(link->name); // NOTE: link->name corresponds 1:1 w/intf->name
        bool intf_enabled = false;
        net_iface_get_var_intf_enabled(if_index, &intf_enabled);
        if (!intf_enabled) return;
    }

    event_history[latest].intf = intf; // update
    event_history[latest].sent = link_event; // update

    // propogate event
    DBG_VERBOSE("%s() '%s' event '%s'\n", __func__, link_name, str_event);

    // link-layer
    if (link)
    {
        net_link_event_handler_cb(link_event, link);
        net_link_notify(link->logical_link, link_event); // broadcast via llink
    }

    // interface-layer
    net_iface_link_event_notify(link_event, (void*)intf); // notify intf-layer
}

// listen to network events via kernel NETLINK socket
#define NLMSG_PACKET_LEN 8192
#define ESTABLISH_LISTENER_ATTEMPTS 3
static char nl_msg_packet[NLMSG_PACKET_LEN] = { 0 };
void* link_status_change_event_listener(void* reply)
{
    DBG_ASSERT(reply != NULL);
    error_type_t res = FAIL;
    int knock = 0;
    int listener = -1;
    while (res != OK) //retry?
    {
        res = open_net_event_handler(&listener);
        knock++;
        if (listener < 0) // no socket yet..
        {
            DBG_ERR("%s():%d '%s' (err %d)\n", __func__, knock, strerror(thread_errno), thread_errno);
            if (knock >= ESTABLISH_LISTENER_ATTEMPTS) // successful?
            {
                if (reply) *(uint32_t*)reply = res; // err
                return (void*)FAIL;
            }
            posix_sleep_ms(1); // token pause
        }
    }
    if (reply) *(uint32_t*)reply = OK;  // optimistic

    DBG_VERBOSE("%s() listener %d\n", __func__, listener);

    while (listener >= 0) // no returns
    {
        //memset(nl_msg_packet, 0, sizeof(nl_msg_packet));
        struct nlmsghdr nl_msg_buf = { 0 }, *nl_msg = &nl_msg_buf;

        // block while waiting for a network notification
        int bytes_read = recv(listener, (void*)&nl_msg_buf, sizeof(nl_msg_buf), MSG_PEEK); // peek -- data available?
        if (bytes_read <= 0)
        {
            if (thread_errno == EAGAIN || thread_errno == EINTR)
            {
                DBG_ERR("%s(%d) %d read %s (err %d) .. retry\n", __func__, listener, bytes_read, strerror(thread_errno), thread_errno);
                continue;
            }
            DBG_ERR("%s(%d) %d read %s (err %d)\n", __func__, listener, bytes_read, strerror(thread_errno), thread_errno);
        }

        int exp_packet_size = NLMSG_LENGTH(nl_msg_buf.nlmsg_len);
        // need to grow msg buffer? -- revisit this tune
        if (exp_packet_size > sizeof(nl_msg_packet))
        {
            DBG_ERR("%s() (%d read) buffer overflow (%d>%d)\n", __func__, bytes_read, exp_packet_size, sizeof(nl_msg_packet));
            DBG_ASSERT(exp_packet_size < sizeof(nl_msg_packet));
        }

        // grab the entire packet
        int packet_size = recv(listener, (void*)nl_msg_packet, exp_packet_size, MSG_WAITFORONE); // entire packet
        if (packet_size < 0)
        {
            if (thread_errno != EAGAIN && thread_errno != EINTR)
            {
                DBG_ERR("%s() '%s' (err %d)\n", __func__, strerror(thread_errno), thread_errno);
            }
            continue;
        }

        // the entire nlmsg packet
        nl_msg = (struct nlmsghdr*) &nl_msg_packet; // cast
        int remaining_packet_bytes = packet_size; // packet size diminishes as parsed..

        // determine intf/link name from event record
        char link_name[IFNAMSIZ] = { 0 };
        rtattr_t* link_event_attr = NULL;
        link_event_attr = extract_ifla_attr(nl_msg, remaining_packet_bytes, IFLA_IFNAME);
        if (link_event_attr) // found
        {
            strncpy(link_name, RTA_DATA(link_event_attr), sizeof(link_name)); // copy ifname
        }

        // parse through this notification for items of interest
        while (NLMSG_OK(nl_msg, remaining_packet_bytes))
        {
            // is there an associated link w/current notification?
            net_link_t* link = NULL;
            link = net_link_lookup_by_name(link_name);
            if (reply) *(uint32_t*)reply = (uint32_t)link; // link record

            // only examine this event if it's an interface-of-interest
            if (link)
            {
                //dbg show_nlmsg((struct nlmsghdr*)nl_msg, nl_msg, link_name);
                int link_event = 0; // sofar indication is too general..
                switch(nl_msg->nlmsg_type)
                {
                    case RTM_DELADDR:
#if EXTRA_LOCAL_DEBUG
                        DBG_MSG("link '%s' event down (%d->%d)\n", link_name, nl_msg->nlmsg_type, link_event);
#endif
                    break;
                    case RTM_NEWADDR:
                        link_event = NET_LINK_EVENT_UP; // wired
#if EXTRA_LOCAL_DEBUG
                        DBG_MSG("link '%s' event up (%d->%d)\n", link_name, nl_msg->nlmsg_type, link_event);
#endif
                    break;

                    case RTM_DELLINK: // fallthrough

                    // all interface/link changes generate this msg:
                    case RTM_NEWLINK:
                    {
                        //dbg walk_ifla_attrs(link_name, nl_msg, nl_msg->nlmsg_len, NULL);
                        ifinfomsg_t* info = (ifinfomsg_t*)NLMSG_DATA(nl_msg);
                        DBG_VERBOSE("info: family %d type %2d seq %d dflags x%x mask x%x\n",
                            info->ifi_family, info->ifi_type, info->ifi_index, info->ifi_flags, info->ifi_change);

                        // wired interface checks
                        bool intf_up = (IFF_LOWER_UP|IFF_RUNNING) == (info->ifi_flags & (IFF_LOWER_UP|IFF_RUNNING));
                        if (intf_up)
                        {
                            //noisy DBG_MSG("link '%s' %s event up & running\n", link_name, str_event(nl_msg->nlmsg_type));
                            propogate_link_event(link, NET_LINK_EVENT_UP);
                            break; // skip the rest of the nl_msg
                        }

#if 0 &&  DEBUG
                        // https://www.kernel.org/doc/Documentation/networking/operstates.txt

                        // OPERSTATE reflects any change (not necessarily a 'new' link)
                        link_event_attr = extract_ifla_attr(nl_msg, remaining_packet_bytes, IFLA_OPERSTATE);
                        if (link_event_attr)
                        {
                            uint8_t state = *(uint8_t*)RTA_DATA(link_event_attr);
                            // IF_OPER_DOWN -- Interface is unable to transfer data on L1
                            if (state == IF_OPER_DOWN) // disconnect
                            {
                                //dbg show_attr(link_event_attr, "NEWLINK OPERSTATE -- down");
                                DBG_VERBOSE("link '%s' %s event -- NOT connected\n",link_name, str_event(nl_msg->nlmsg_type));
                            }
                            else
                            if (state == IF_OPER_UP) // connect
                            {
                                //dbg show_attr(link_event_attr, "NEWLINK OPERSTATE -- up");
                                DBG_VERBOSE("link '%s' %s event -- connected\n", link_name, str_event(nl_msg->nlmsg_type));
                            }
                            else
                            {
                                show_attr(link_event_attr, "NEWLINK (any) OPERSTATE");
                            }
                        }
#endif // DEBUG

                        // filter out wired up/down events
                        if (!strcmp(link_name, NET_IF_NAME_ETH) && link_event_attr)
                        {
                            break; // skip the rest of the nl_msg
                        }
#ifdef HAVE_WIRELESS
                        if (link_name[0]) // wireless info buried further..
                        {
                            // wireless event captured in an additional IFLA_WIRELESS record
                            link_event_attr = extract_ifla_attr(nl_msg, remaining_packet_bytes, IFLA_WIRELESS);
                            if (link_event_attr)
                            {
                                // what's this attr?
                                int link_event;
                                bool event_of_interest = sift_ifla_attrs(link_name, nl_msg, nl_msg->nlmsg_len, &link_event);
                                if (event_of_interest && !strcmp(link_name,NET_IF_NAME_UAP)) // disqualify wifi-sta events -- revisit
                                {
                                    DBG_VERBOSE("%s() %s event %d\n", __func__, link_name, link_event);
                                    propogate_link_event(link, link_event);
                                }
                                //dbg else hex_dump((void*)nl_msg, nl_msg->nlmsg_len);
                            }
                            else // unexpected attr format
                            {
                                // what's this attr? (revisit ifi_flags settings)
                                link_event_attr = extract_ifla_attr(nl_msg, remaining_packet_bytes, IFLA_AF_SPEC); // AF_SPEC attr size (>4 up, ==4 down)
                                if (link_event_attr)
                                {
                                    if (link_event_attr->rta_len > 4) // might also be configured
                                    {
                                        // tbd: revisit question of what's the best way to indicate a plug&play event.
                                        //    A new yet unconfigured link (present but no ipv4 binding) may not be the most precise indicator
                                        char* get_intf_ipv4_addr(char* name); // revisit extern cast
                                        if (!get_intf_ipv4_addr(link_name)) // ignore if already configured
                                        {
                                            DBG_VERBOSE("link '%s' %s detected and unconfigured\n", link_name, str_event(nl_msg->nlmsg_type));
                                            link_event = NET_LINK_EVENT_HW_PRESENCE;
                                        }
                                    }
                                    else
                                    {
                                        DBG_VERBOSE("link '%s' %s down\n", link_name, str_event(nl_msg->nlmsg_type));
                                        link_event = NET_LINK_EVENT_DOWN;
                                    }
                                }
                            }

#if EXTRA_LOCAL_DEBUG
                            if (!link_event) // no change detected
                            {
                                link_event_attr = extract_ifla_attr(nl_msg, remaining_packet_bytes, IFLA_WIRELESS);
                                if (link_event_attr)
                                {
                                    DBG_IGNORED("'%s' ignored event WIRELESS (%d) \n", link_name, link_event_attr->rta_len);
                                }
                                else
                                {
                                    walk_ifla_attrs(link_name, nl_msg, nl_msg->nlmsg_len, NULL);
                                    DBG_IGNORED("'%s' ignored event %s (%d) \n", link_name, str_event(nl_msg->nlmsg_type), nl_msg->nlmsg_type);
                                }
                            }
#endif
                        }
#endif // HAVE_WIRELESS
                    } break; // RTM_NEWLINK

                    default:
                        // eth0 'disconnect' generates a 'new link' before 'del addr' revisit?
                        break;
                }

                // propogate event -> link-status monitor and beyond
                if (link_event)
                {
                    propogate_link_event(link, link_event);
                }
            }
#ifdef DBG_VERBOSE_ON
            else
            {
                DBG_IGNORED("'%s' %s ignored event %s (%d)\n", link_name, __func__, str_event(nl_msg->nlmsg_type), nl_msg->nlmsg_type);
                //noisy show_nlmsg((struct nlmsghdr*)nl_msg, nl_msg, link_name);
            }
#endif

            nl_msg = NLMSG_NEXT(nl_msg, remaining_packet_bytes); // adv
        }
    } // forever do (no returns)

    // unlikely
    close_net_event_handler(listener);
    if (reply) *(uint32_t*)reply = FAIL;
    return (void*)FAIL;
}

#ifdef __cplusplus
}
#endif

// eof net_link_events.c
