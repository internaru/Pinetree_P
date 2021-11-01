/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

//  Provide a cmd interface to 'net' interactions

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// common includes
#include "os_network.h"
#include "oid_api.h" // oid_t
#include "net_api.h"
#include "net_conf_api.h" // DEFAULT_PLATFORM_HOSTNAME
#include "net_iface.h" // g_net_context
#include "net_iface_vars.h" // NET_IFACE_HOSTNAME_SIZE
#include "net_link_api.h"
#include "net_intf_api.h" // net_get_active_link()
#include "os_system.h" // os_shell()

// if-socket
#include <sys/ioctl.h> // ioctl
#include <linux/sockios.h> // SIOCGIFCONF

#ifdef HAVE_WLAN
#include "net_wifi_api.h" // WIFI_SERVICE_RESTART_DELAY
#include "wifi_intf_api.h" // MAX_SSID_NAME_LEN
#endif


#if 1 // include for both release and debug //#if defined DEBUG || defined HAVE_CMD

static pthread_mutex_t g_net_if_context_mutex = PTHREAD_MUTEX_INITIALIZER;

#define NET_LOCK_IF_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_if_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define NET_UNLOCK_IF_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_if_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define CMD_BUCKET "net"

#include "error_types.h"
#define DBG_PRFX "if.cmd: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
#include "net_debug.h" // DBG_ family defaults

#undef DBG_INFO
#define DBG_INFO DBG_ERR /* elevate -> DBG_ERR */

// intf_active() -- query os-socket layer for intf flags -- true iff flags & IFF_RUNNING
bool intf_active(char* intf_of_interest)
{
    net_link_status_t intf_rcode = NET_LINK_STATUS_UP;
    int sock;
    struct ifconf current_intf;
    struct ifreq intf_set[NET_MAX_INTERFACES];
    int active_intfs;
    int i;

    // Create a socket or return an error.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        DBG_ERR("%s: bad socket",__func__);
        return false;
    }

    // Point ifconf's ifc_buf to our array of interface ifreqs.
    current_intf.ifc_buf = (char*)intf_set;

    // Set ifconf's ifc_len to the length of our array of interface ifreqs.
    current_intf.ifc_len = sizeof(intf_set);

    //  Populate ifconf.ifc_buf (ifreq) with a list of interface names and addresses.
    if (ioctl(sock, SIOCGIFCONF, &current_intf) == -1)
    {
        DBG_ERR("%s: bad SIOCGIFCONF call..", __func__);
        close(sock);
        return false;
    }

    // Divide the length of the interface list by the size of each entry.
    // This gives us the number of interfaces on the system.
    active_intfs = current_intf.ifc_len / sizeof(intf_set[0]);

    intf_rcode = NET_LINK_STATUS_DOWN; // unless otherwise demonstrated
    for (i = 0; i < active_intfs; i++) {
        if (!strcmp(intf_set[i].ifr_name, intf_of_interest)) // matching intf?
        {
            // query flags for current intf
            struct ifreq* intf = &intf_set[i]; // current intf
            ioctl(sock, SIOCGIFFLAGS, intf); // get flags for the current intf

            if (intf->ifr_flags & IFF_RUNNING)
            {
                close(sock);
                return true; // "Active";
            }
            else break; // done
        }
    }
    close(sock);
    UNUSED_VAR( intf_rcode ); 
    return false; // "Not Active";
}

// intf_up() -- query os-socket layer for intf flags -- true iff flags & IFF_UP
bool intf_up(char* intf_of_interest)
{
    net_link_status_t intf_rcode = NET_LINK_STATUS_UP;
    int sock;
    struct ifconf current_intf;
    struct ifreq intf_set[NET_MAX_INTERFACES];
    int active_intfs;
    int i;

    // Create a socket or return an error.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        DBG_ERR("%s: bad socket",__func__);
        return false;
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
        return false;
    }

    // Divide the length of the interface list by the size of each entry.
    // This gives us the number of interfaces on the system.
    active_intfs = current_intf.ifc_len / sizeof(intf_set[0]);

    intf_rcode = NET_LINK_STATUS_DOWN; // unless otherwise demonstrated
    for (i = 0; i < active_intfs; i++) 
    {
        if (!strcmp(intf_set[i].ifr_name, intf_of_interest)) // matching intf?
        {
            // query flags for current intf
            struct ifreq* intf = &intf_set[i]; // current intf
            ioctl(sock, SIOCGIFFLAGS, intf); // get flags for the current intf

            if (intf->ifr_flags & IFF_UP)
            {
                close(sock);
                return true; // "Up"
            }
            else break; // done
        }
    }
    close(sock);
    UNUSED_VAR( intf_rcode );
    return false; // "Down";
}

// u8 -> str converter
static char str_mac[20]; // padded
char* u8mac2str(uint8_t* mac_u8) // use 1st MAC_ADDR_BYTE_SIZE (6) bytes only ..
{
    memset(str_mac, 0, sizeof(str_mac));
    sprintf(str_mac, "%02x-%02x-%02x-%02x-%02x-%02x",
        mac_u8[0], mac_u8[1], mac_u8[2], mac_u8[3], mac_u8[4], mac_u8[5]);
    return str_mac;
}

// int -> str literal representation
static char str_ipv4_state[32];
char* str_ipv4_cfg_state(int state)
{
#define str_ipv4_state(state) case state: sprintf(str_ipv4_state, #state); break
    switch(state)
    {
        str_ipv4_state(IPV4_NOT_CONFIGURED);
        str_ipv4_state(IPV4_CLOSING);
        str_ipv4_state(IPV4_CONFIGURING);
        str_ipv4_state(IPV4_RECONFIGURING);
        str_ipv4_state(IPV4_CONFIGURED);
     default:
        sprintf(str_ipv4_state, "<ipv4 state %d>", state); break;
    }
    return str_ipv4_state;
}

static char str_ipv6_state[32];
char* str_ipv6_cfg_state(int state)
{
#define str_ipv6_state(state) case state: sprintf(str_ipv6_state, #state); break
    switch(state)
    {
#ifdef HAVE_IPV6
        str_ipv6_state(IPV6_NOT_CONFIGURED);
        str_ipv6_state(IPV6_CLOSING);
        str_ipv6_state(IPV6_CONFIGURING);
        str_ipv6_state(IPV6_LOCAL_CONFIGURED);
        str_ipv6_state(IPV6_CONFIGURED);
#endif
     default:
        sprintf(str_ipv6_state, "<ipv6 state %d>", state); break;
    }
    return str_ipv6_state;
}

// cycle through network interfaces
void show_intfs(void) // travel g_net_context data
{
    uint32_t t_uint32_t; // temp, reused

    if (g_net_context->iface_cnt < 1)
    {
        DBG_ERR("%s() pending..\n", __func__);
        return;
    }
    
    // interfaces
    int max_intfs = net_iface_enum_get_count(); // g_net_context->iface_cnt;
    int if_offset = 0; // 0-based lookup aka local_idx oem_offset
    while (if_offset < max_intfs)
    {
        net_interface_t* intf = g_net_context->ifaces[if_offset]; // direct lookup
        if (intf)
        {
            int if_index = if_nametoindex(intf->name);
            char* ipv4_addr = NULL;
            char* str_ipv4_state = str_ipv4_cfg_state((int)(intf->ipv4_cfg_state)); UNUSED_VAR(str_ipv4_state); // cast
#ifdef HAVE_IPV6
            char ipv6_addr_str[64], *ipv6_addr = NULL;
            char* str_ipv6_state = str_ipv6_cfg_state((int)(intf->ipv6_cfg_state)); UNUSED_VAR(str_ipv6_state); // cast
            ipv6_addr = get_intf_ipv6_addr(intf->name);
            if (ipv6_addr)
            {
                strncpy(ipv6_addr_str, ipv6_addr, sizeof(ipv6_addr_str));
                ipv6_addr_str[sizeof(ipv6_addr_str)-1] = 0;
                ipv6_addr = ipv6_addr_str; 
            }
#endif
            ipv4_addr = get_intf_ipv4_addr(intf->name);

            // intf enabled? (link may be active but intf disabled)
            bool intf_enabled = false;
            net_iface_get_var_intf_enabled(if_index, &intf_enabled);

            // prefer ipv4 to ipv6, unbound otherwise
            DBG_INFO("  %5s (if_index %d) x%x '%s' ipv4 %s %s%s\n", intf->name, if_index,
                intf, str_iface_state((int)(intf->state)),
                (ipv4_addr? ipv4_addr: "<unbound>"),
#ifdef HAVE_IPV6
                (ipv6_addr? ipv6_addr: ""),
                intf_enabled? "": " (disabled)");
            DBG_INFO("       state v4 %s (%d) v6 %s (%d)\n", str_ipv4_state, intf->ipv4_cfg_state, str_ipv6_state, intf->ipv6_cfg_state);
#else
                "",
                intf_enabled? "": " (disabled)");
            DBG_INFO("       state v4 %s (%d)\n", str_ipv4_state, intf->ipv4_cfg_state);
#endif // HAVE_IPV6
        }
        if_offset++;
    }

    // intf_mapp[]
    int if_index = if_nametoindex(NET_IF_NAME_ETH);
    int res = oid_get_uint32(OID_NETWORK_INTF_COUNT, if_index, &t_uint32_t);
    if (res!=OK){DBG_MSG("INTF_COUNT oid err %d\n", res);}
    else { DBG_MSG("\n  Overall INTF_COUNT \t%d\n", t_uint32_t); }

}

static const char* up_cmd_usage = "[<interface(s)>] -- shows interfaces currently 'up' (starts <interface(s)>)";
static const char* up_cmd_desc  = "sets <interface [ "
NET_IF_NAME_ETH
#ifdef HAVE_WIRELESS
" | " NET_IF_NAME_STA
" | " NET_IF_NAME_UAP
#endif
" ]> up";

// actively tear down a network interface
int do_cmd_net_set_up(net_interface_t* intf)
{
    if (!intf) return 0;
    char os_req[64] = {0}; // need stack buffer -- dynamic allocation freed before os finishes ..
    const char* up_req = "ifup -f %s\n\0";

    snprintf(os_req, sizeof(os_req), up_req, intf->name);
    DBG_ERR("sh %s\n", os_req);

    return os_system(os_req); // 1 call, big list
}

// report currently 'up' interfaces, optionally enables given interfaces 'on'
int cmd_net_set_up_cb(int argc, char* argv[])
{
    int next = 1; // 1st argument
    char* if_name = argv[next++];

    { NET_LOCK_IF_CONTEXT();

        if (!if_name) // if no interfaces specified, show all interfaces in the 'up' state
        {
            int i = 0; // first intf
            net_interface_t* intf = g_net_context->ifaces[i++];
            while (intf)
            {
                net_link_status_t internal_state = net_link_get_status(intf->link_handle);
                bool ipv4_bound = intf_up(intf->name);
                if (ipv4_bound)
                {
                    if (internal_state == NET_LINK_STATUS_UP)
                    {
                        DBG_INFO("intf '%s' up\n", intf->name);
                    }
                    else
                    if (internal_state == NET_LINK_STATUS_DOWN)
                    {
                        DBG_ERR("intf '%s' crossed up\n", intf->name);
                        net_send_msg(NET_IFACE_MSG_LINK_UP_POLL, intf, (void *)0, (void *)0);
                    }
                }
                intf = g_net_context->ifaces[i++];
            }
        }

        // enable ipv4
        else while (if_name) // start if_name(s)
        {
            // check requested interface against known/expected if_name[]s
            if ( (!strcmp(if_name, NET_IF_NAME_ETH))
#ifdef HAVE_WIRELESS
#ifdef HAVE_WLAN
                 || (!strcmp(if_name, NET_IF_NAME_STA)) // wlan/mlan
#endif
#ifdef HAVE_UAP
                 || (!strcmp(if_name, NET_IF_NAME_UAP))
#endif
#endif // HAVE_WIRELESS
               )
            {
                net_interface_t* intf = net_iface_lookup_by_name(if_name);
                if (intf) // oem-known?
                {

                    do_cmd_net_set_up(intf); // actively bring interface up

                    unsigned int if_index = if_nametoindex(if_name);
                    if (if_index > 0) // os-known?
                    {
                        DBG_INFO("interface(%d) '%s' -> enabled\n", if_index, if_name);

                        net_iface_set_var_intf_enabled(if_index, true); // enable intf

                        // tickle iface state machine
                        net_send_msg(NET_IFACE_MSG_LINK_UP_POLL, intf, (void *)0, (void *)0);
                    }
                }
            }
            else // unknown req
            {
                DBG_IGNORED("unknown interface '%s' ignored\n", if_name);
            }

            if_name = argv[next++];
        }
    } NET_UNLOCK_IF_CONTEXT();

    return OK;
}

static const char* down_cmd_usage = "[<interface(s)>] -- shows interfaces currently 'down' (stops <interface(s)>)";
static const char* down_cmd_desc  = "sets <interface [ "
NET_IF_NAME_ETH
#ifdef HAVE_WIRELESS
" | " NET_IF_NAME_STA
" | " NET_IF_NAME_UAP
#endif
" ]> down";

// actively take a network interface down
int do_cmd_net_set_down(net_interface_t* intf)
{
    if (!intf) return 0;
    char os_req[64] = {0}; // need stack buffer -- dynamic allocation freed before os finishes ..
    const char* ifdown_req = "ifdown -f %s\n\0";

    snprintf(os_req, sizeof(os_req), ifdown_req, intf->name);
    DBG_ERR("sh %s\n", os_req);

    return os_system(os_req); // single call
}

// report interface(s) currently off, optionally disabled interface (if given)
int cmd_net_set_down_cb(int argc, char* argv[])
{
    int next = 1; // 1st argument
    char* if_name = argv[next++];

    //NET_LOCK_IF_CONTEXT();
    if (!if_name) // if no interfaces specified, show all interfaces in the 'up' state
    {
        int i = 0; // first intf
        net_interface_t* intf = g_net_context->ifaces[i++];
        while (intf)
        {
            if (!intf_up(intf->name))
            {
                DBG_INFO("intf '%s' down\n", intf->name);
            }
            intf = g_net_context->ifaces[i++];
        }
    }
    // per given interface..
    while (if_name)
    {
        if ( (!strcmp(if_name, NET_IF_NAME_ETH))
#ifdef HAVE_WIRELESS
#ifdef HAVE_WLAN
             || (!strcmp(if_name, NET_IF_NAME_STA)) // wlan/mlan
#endif
#ifdef HAVE_UAP
             || (!strcmp(if_name, NET_IF_NAME_UAP))
#endif
#endif // HAVE_WIRELESS
           )
        {
            net_interface_t* intf = net_iface_lookup_by_name(if_name);
            if (intf) // intf defined
            {
                unsigned int if_index = if_nametoindex(if_name);
                if (if_index > 0)
                {
                    DBG_INFO("interface(%d) '%s' -> disabled\n", if_index, if_name);

                    net_iface_set_var_intf_enabled(if_index, false); // disable intf
                    net_iface_notify(intf, NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING);
                    //net_send_msg(NET_IFACE_STATE_CLOSING_INTERFACE, intf, (void *)0, (void *)0);
                }
                do_cmd_net_set_down(intf); // take interface down
                net_send_msg(NET_IFACE_MSG_NET_RESET, intf, (void *)0, (void *)0);
            }
        }
        else // unknown req
        {
            DBG_ERR("unknown interface '%s' ignored\n", if_name);
        }
 
        if_name = argv[next++];
    }

    //NET_UNLOCK_IF_CONTEXT();
    return OK;
}

static const char* mapp_cmd_usage = "";
static const char* mapp_cmd_desc  = "show if -> intf mappings";
int cmd_net_mapp_cb(int argc, char* argv[])
{
    refresh_iface_bindings(); // any changes in the os-intf inventory?

    int if_offset = 0; // os interface offset
    while (if_offset < sizeof_intf_map) // dynamic
    {
        char* if_name = intf_mapp[if_offset].if_name;

        // interface currently bound to ipv4?
        net_interface_t* intf = net_iface_lookup_by_name(if_name);
        int oem_offset = NOT_BOUND;
        if (intf)
        {
            oem_offset = intf->local_idx;
        }
        if (!if_name) if_name = "<unbound>";

        {
            dbg_printf(" %5s if_offset %d if_index %2d", if_name, if_offset, intf_mapp[if_offset].if_index);
            if (oem_offset > NOT_BOUND) dbg_printf(" oem_offset %d", oem_offset);
            dbg_printf("\n");
        }

        if_offset++;
    }

    return OK;
}

#if HAVE_WSD
#include "net_wsd_vars.h" // WSD_UUID_LENGTH
#endif // HAVE_WSD
static const char* intf_cmd_usage = "";
static const char* intf_cmd_desc  = "show intf";
int cmd_net_intf_cb(int argc, char* argv[])
{
    show_intfs();

    return OK;
}

// cmd: 'net info'
static const char* info_cmd_usage = "";
static const char* info_cmd_desc  = "display current intf settings";
int cmd_net_info_cb(int argc, char* argv[])
{
    // iterate through known interfaces
    int oem_offset = 0;
    while (oem_offset < net_iface_enum_get_count())
    {
        DBG_INFO("  offset2if_index(%d) -> %d\n", oem_offset, net_iface_enum_to_if_index(oem_offset));

        char* if_name = iface_name_lookup_by_index(oem_offset);
        if (if_name)
        {
            net_interface_t* intf = net_iface_lookup_by_name(if_name);
            if (intf)
            {
                unsigned int if_index = if_nametoindex(if_name); // what if interface is enabled but not present, e.g, wifi over unplugged usb
                bool intf_enabled = false;
                if (if_index > 0)
                {
                    // intf enabled? (link may be active but intf disabled)
                    net_iface_get_var_intf_enabled(if_index, &intf_enabled);
                }

                DBG_INFO("  intf #%d %5s (if_index %3d) %s\n",
                    intf->local_idx, intf->name, intf->treck_idx, intf_enabled? "": "(disabled)"); 

            }
        }
        oem_offset++;
    }

#ifdef HAVE_NET_SERVICES
    show_net_services_activity();
#endif

    return 0;
}

static const char* reset_cmd_usage = "";
static const char* reset_cmd_desc  = "resets/restarts network";
int cmd_net_reset_factory_settings_cb(int argc, char* argv[]) // NET_WLAN_LINK_NAME
{
    DBG_VERBOSE("%s() .. \n", __func__);
    char os_req[1024] = {0}; // need stack buffer -- dynamic allocation freed before os finishes ..

    stop_etc_network_service(NULL); // broad network interfaces (ifconfig)

    // tickle oid reset (revisit for discrete interfaces)
    oid_set_uint32(OID_NETWORK_RESET, 0, 1); // basic var set
    oid_set_uint32(OID_NETWORK_RESET, 1, 1); // intf 1
    oid_set_uint32(OID_NETWORK_RESET, 2, 1); // intf 2
    oid_set_uint32(OID_NETWORK_RESET, 3, 1); // intf 3
    oid_set_uint32(OID_NETWORK_RESET, 4, 1); // intf 4

    // cleanup service/daemon files
    if (1)
    {
        // cleanup well-defined file names
        char* service_files_list[] = {
            mdns_start_sh,
            mdns_conf,
            stop_mdns_sh,
            uap_start_sh,
            uap_conf,
            udhcpd_conf,
            stop_udhcpd_sh,
            stop_uap_sh,
            wifi_sta_start_sh,
            stop_wpa_sh,
            stop_wifi_sta_sh,
            etc_network_interfaces_conf,
            NULL // required marker last
        };

        if (os_req != NULL)
        {
            int next = 0;
            strcat(os_req, "rm -rf ");
            char* service_file = service_files_list[next++];
            while (service_file)
            {
                strcat(os_req, service_file);
                strcat(os_req, " ");
                service_file = service_files_list[next++];
            }
            strcat(os_req, "; sync\n\0");
            os_system(os_req); // 1 call, big list
        }
        else
        {
            DBG_ERR("%s() incomplete reset\n", __func__);
            DBG_ASSERT(0); // fix allocation
        }
    }

    uint32_t config_method = NET_IFACE_IPV4_USE_DHCP; // nearly auto
    net_iface_set_var_ipv4_config_options(NET_IF_TYPE_ETH, &config_method);

    // (re)generate network configuration and service scripts

#ifdef HAVE_UAP
    rebuild_uap_sh_files();  // generate uap_start/stop scripts
    net_iface_set_var_ipv4_config_options(NET_IF_TYPE_UAP, &config_method);
#endif
#ifdef HAVE_WLAN
    rebuild_wifi_sh_files(); // generate wifi_sta_start/stop scripts
    net_iface_set_var_ipv4_config_options(NET_IF_TYPE_STA, &config_method);
#endif
#ifdef HAVE_MDNS
    rebuild_mdns_sh_files(); // generate mdns_start/stop scripts
#endif

    start_etc_network_service(NULL, true); // rebuild_etc_network_interfaces() implied

    return OK;
}

static void register_cmd_plugin()
{
    error_type_t cmd_res;

    // add net cmds
    cmd_res = cmd_register_cmd(CMD_BUCKET, NULL, NULL, NULL, NULL, NULL);
    if (cmd_res != CMD_OK)
    {
        DBG_VERBOSE("%s() already registration.. '%s'\n", __func__, CMD_BUCKET);
    }

    // net info
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "info",
                                   info_cmd_desc,
                                   info_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_net_info_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "stats");
    }

    // net enable
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "up",
                                   up_cmd_desc,
                                   up_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_net_set_up_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "up");
    }

    // net disable
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "down",
                                   down_cmd_desc,
                                   down_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_net_set_down_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "down");
    }

    // net intf
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "intf",
                                   intf_cmd_desc,
                                   intf_cmd_usage,
                                   NULL, //notes,
                                   cmd_net_intf_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration '%s %s'\n", __func__, CMD_BUCKET, "intf");
    }

    // net mapp
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "mapp",
                                   mapp_cmd_desc,
                                   mapp_cmd_usage,
                                   NULL, //notes,
                                   cmd_net_mapp_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration '%s %s'\n", __func__, CMD_BUCKET, "mapp");
    }

    // net reset settings to factory defaults
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "_reset", // hidden
                                   reset_cmd_desc,
                                   reset_cmd_usage,
                                   NULL, //notes,
                                   cmd_net_reset_factory_settings_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "_reset");
    }

    return;
}

#else
#define register_cmd_plugin() { (void)0; }
#endif // HAVE_CMD || DEBUG

void register_net_iface_cmds(void)
{
    register_cmd_plugin();

return;
}

#ifdef __cplusplus
}
#endif

//eof net_iface_cmds.c
