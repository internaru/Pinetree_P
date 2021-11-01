/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

//  Provide additional 'net' cmds

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// common includes
#include "error_types.h" // CMD_OK
#include "os_network.h"
#include "oid_api.h" // oid_t
#include "net_api.h"
#include "net_conf_api.h" // DEFAULT_PLATFORM_HOSTNAME
#include "net_iface_vars.h" // NET_IFACE_HOSTNAME_SIZE
#include "net_link_api.h"
#include "net_intf_api.h" // net_get_active_link()
#include "net_iface.h" // g_net_context
#include "os_system.h" // os_shell()

// debug
#define CMD_BUCKET "net" /* logger_common.c network #12 */
#define DBG_PRFX "net.link.cmd: "
#define DBG_ERR_ON
#define DBG_ON 1 /* DBG_MSG */
#define DBG_VERBOSE_ON
#include "net_debug.h" // DBG_ family defaults

#undef DBG_MSG
#define DBG_MSG DBG_ERR /* elevate cmd feedback */

// if-socket
#include <sys/ioctl.h> // ioctl
#include <linux/sockios.h> // SIOCGIFCONF

#ifdef HAVE_WLAN
#include "net_wifi_api.h" // WIFI_SERVICE_RESTART_DELAY
#include "wifi_intf_api.h" // MAX_SSID_NAME_LEN
#endif

// stubs
#define MAX_HOSTNAME_LEN NET_IFACE_HOSTNAME_SIZE + 1

// local work area
static union {
    uint8_t name_u8[MAX_HOSTNAME_LEN]; // w/padding
    char hostname[MAX_HOSTNAME_LEN];
} oid;
static char* oid_hostname = oid.hostname; // &oid.hostname[0];

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

static pthread_mutex_t g_net_link_context_mutex;

#define NET_LOCK_LINK_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_link_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define NET_UNLOCK_LINK_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_link_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 



#if DBG_ON // include for both release and debug //#if defined DEBUG || defined HAVE_CMD

// link->logical_link (lll?)
static void show_llinks(net_link_logical_t* ll)
{
    if (ll)
    {
        DBG_VERBOSE("    lll %#x l#%d #links %d status %d\n", (unsigned int)ll, ll->logical_enum, ll->link_cnt, ll->status);
    }
}

// show network link structure
void show_net_links(net_link_context_t* intf)
{
    int index = 0;
    if (!intf)
    {
        return;
    }

    if (intf)
    {
        DBG_MSG("intf %#x %d-link %d-logical (enumerated %d)\n",
            (unsigned int)intf, intf->link_cnt, intf->logical_link_cnt, intf->enumerated);
    }

    while (index < intf->logical_link_cnt)
    {
        net_link_logical_t* llink = intf->logical_links[index];
        while (llink)
        {
#ifdef DBG_VERBOSE_ON
            net_link_t* link = llink->links;
            if (link)
            {
                DBG_VERBOSE("  link %#x index %d %6s oem-%d (enabled %d) llink %#x\n",
                    link, index, link->name, link->oem_enum, link->enabled, link->logical_link);
                if (link->logical_link) show_llinks(link->logical_link);
            }
#endif
            llink = llink->next;
        }
        index++;
    }
}

#define str_state(state) case state: sprintf(str_link_state, #state); break
char* str_link_state(int link_state)
{
    static char str_link_state[32] = { 0 };
    memset(str_link_state, 0, sizeof(str_link_state));
    switch(link_state)
    {
        str_state(NET_LINK_STATE_DOWN_DELAY);
        str_state(NET_LINK_STATE_DOWN_PENDING);
        str_state(NET_LINK_STATE_DOWN);
        str_state(NET_LINK_STATE_UP_DELAY);
        str_state(NET_LINK_STATE_UP);
      default:
        sprintf(str_link_state, "<link state %d>", link_state); break;
    }
    return str_link_state;
}

void show_link(net_link_t* link)
{
    DBG_VERBOSE("%s(link %#x)\n", __func__, link);
    if (link)
    {
        int link_status = NET_LINK_STATUS_DOWN; // until otherwise determined
        net_link_logical_t *llink = link->logical_link;
        DBG_ASSERT(llink);

        // as in net_link_get_status() ..
        if(llink->priority_link_up)
        {
            link_status = NET_LINK_STATUS_UP;
        }

        char* str_link_status = "<unknown>";
        switch (link_status)
        {
            case NET_LINK_STATUS_UP: str_link_status = "up"; break;
            case NET_LINK_STATUS_DOWN: str_link_status = "down"; break;
        }
        DBG_MSG(" link %s %#x status '%s' state %s %d\n", link->name, link, str_link_status, str_link_state(link->state), link->state);
        show_llinks(link->logical_link);
    }
}

void show_links(net_link_context_t* links) // traverse links list
{
    int l = 0; // link-index

    if (links->link_cnt <= 0)
    {
        DBG_ERR("net links unavailable (enumerated?)\n");
        return;
    }

    DBG_MSG(" g_link %#x %d link%s %d llink%s\n", (unsigned int)links, links->link_cnt, (links->link_cnt==1?"":"s"), links->logical_link_cnt, (links->logical_link_cnt==1?"":"s"));
    net_link_t* link = links->links[l]; // net_link_t
    while (l < links->logical_link_cnt)
    {
        net_link_t* link = links->links[l]; // net_link_t
        if (link)
        {
            DBG_VERBOSE("  %d: %5s %senabled (llink %#x)\n", link->oem_enum, link->name, (link->enabled?"":"NOT "), link->logical_link);
            show_link(link);
        }
        else
        {
            DBG_IGNORED("  link %d undefined?\n", l);
        }
        l++;
    }

    // start at the beginning of the logical-link-list
    net_link_logical_t* llink = links->logical_links[0];
    if (llink)
    while (llink->next)
    {
        link = llink->links; // net_link_t
        while (link)
        {
            DBG_ERR("  %d: '%5s' enabled %d (llink %#x)\n", link->oem_enum, link->name, link->enabled, link->logical_link);
            if (link->logical_link)
            {
                show_llinks(link->logical_link);
            }
            link = link->next;
        }
        llink = llink->next;
    }
}

// retrieves current link data
int cmd_net_links_cb(int argc, char* argv[])
{
    NET_LOCK_LINK_CONTEXT();

    // retrieve current vars
    oid_get_string(OID_NETWORK_HOST_NAME, 0, oid_hostname, MAX_HOSTNAME_LEN);
    DBG_MSG("hostname: %s\n", oid_hostname);

    show_links(&g_net_link_ctxt);

    NET_UNLOCK_LINK_CONTEXT();
    return 0;
}

static const char* host_cmd_usage = "";
static const char* host_cmd_desc  = "display current hostname";
int cmd_net_host_cb(int argc, char* argv[])
{
    char* new_hostname = argv[1]; // any 1st argument
    if (new_hostname)
    {
        int name_len = strlen(new_hostname);
        if (new_hostname[0] && (name_len < MAX_HOSTNAME_LEN))
        {
            int res = oid_set_string(OID_NETWORK_HOST_NAME, 0, new_hostname, name_len);
	    if (res != OK)
            {
                DBG_ERR("hostname '%s' ignored (err %d)\n", new_hostname, res);
            }
        }
        else
        {
            DBG_ERR("hostname '%s' too long\n", new_hostname);
            return ~OK;
        }
    }

    oid_get_string(OID_NETWORK_HOST_NAME, 0, oid_hostname, MAX_HOSTNAME_LEN);
    if (oid_hostname[0])
    {
        DBG_MSG("  hostname '%s'", oid_hostname);
    }

    return OK;
}

static const char* stats_cmd_usage = "stat [<option(s)>] e,g stat -W -a ]";
static const char* stats_cmd_desc  = "generates netstat [<options>]";
int cmd_net_stats_cb(int argc, char* argv[])
{
    int next = 1;
    char* next_arg = argv[next++];
    char os_req[128] = { "netstat " };

    while (next_arg) // collect args for os_req
    {
        // don't add anything that exceeds internal buffer
        if ((strlen(os_req) + strlen(next_arg)) < sizeof(os_req))
        {
            strcat(os_req, " ");
            strcat(os_req, next_arg);
            strcat(os_req, " ");
        }
        next_arg = argv[next++];
    }

    os_shell(os_req);

    return OK;
}

static const char* oids_cmd_usage = "[< oem# > default=0], where oem# is 0,1..";
static const char* oids_cmd_desc  = "display OID_NETWORK... oids for <oem#>";
int cmd_net_oids_cb(int argc, char* argv[]) // NET_WLAN_LINK_NAME
{
    uint32_t t_uint32_t;
    char t_str[128];
    error_type_t res;

    int next = 1;
    char* arg = argv[next++]; // 1st argument
    if (!arg) arg = "0", next--; // peek at default 'wired'
    int oem_offset = 0; // default wired
    char* if_name = "<unbound>";

    while (arg)
    {
        sscanf(arg, "%d", &oem_offset);
        // range check oem_offset -- snap to wired if out of range
        if (oem_offset < 0 || oem_offset >= NET_MAX_INTERFACES)
        {
            oem_offset = if_nametoindex(NET_IF_NAME_ETH);
        }
        if_name = iface_name_lookup_by_index(oem_offset);
        if (if_name)
        {
            net_interface_t* intf = net_iface_lookup_by_name(if_name);
            if (intf) if_name = intf->name;
       
            DBG_MSG("\nintf %s (oem %d) if_index %d\n\n", if_name, oem_offset, if_nametoindex(if_name)); // banner

            int i = if_nametoindex(if_name);

            res =
                oid_get_uint32(OID_NETWORK_IPV4_ENABLED, i, &t_uint32_t); // enabled
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_ENABLED \t\treturns %d\n", t_uint32_t); }
#ifdef HAVE_MDNS
            res =
                oid_get_uint32(OID_NETWORK_BONJOUR_ENABLED, 0, &t_uint32_t); // enabled
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid BONJOUR ENABLED \t\treturns %d\n", t_uint32_t); }
#endif
#ifdef HAVE_WLAN
            res =
                oid_get_uint32(OID_WIFI_INTF_ENABLED, 0, &t_uint32_t); // enabled
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid wifi-sta ENABLED \t\treturns %d\n", t_uint32_t); }
#endif
#ifdef HAVE_UAP
            res =
                oid_get_uint32(OID_UAP_INTF_ENABLED, 0, &t_uint32_t); // enabled
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid wifi-uap ENABLED \t\treturns %d\n", t_uint32_t); }
#endif
            res =
                oid_get_string(OID_NETWORK_HOST_NAME, 0, t_str, MAX_HOSTNAME_LEN);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid HOST_NAME \t\t\treturns '%s'\n", t_str); }
            res =
                oid_get_uint32(OID_NETWORK_STATUS, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid NETWORK_STATUS \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_string(OID_NETWORK_MAC_ADDRESS, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid MAC_ADDRESS \t\treturns '%s'\n", t_str); }
            res =
                oid_get_uint32(OID_NETWORK_HOST_NAME_ORIGIN, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid NAME_ORIGIN \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_HOST_NAME_PREF_ORIGIN, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid NAME_PREF_ORIGIN \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_string(OID_NETWORK_HOST_NAME_DEFAULT, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid HOST_NAME_DEFAULT \t\treturns '%s'\n", t_str); }
            res =
                oid_get_string(OID_NETWORK_DOMAIN_NAME, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid DOMAIN_NAME \t\treturns '%s'\n", t_str); }
            res =
                oid_get_uint32(OID_NETWORK_DOMAIN_NAME_ORIGIN, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid DOMAIN_NAME_ORIGIN \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_DOMAIN_NAME_PREF_ORIGIN, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid NAME_PREF_ORIGIN \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_string(OID_NETWORK_DOMAIN_NAME_MANUAL, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid DOMAIN_NAME_MANUAL \t\treturns '%s'\n", t_str); }
            res =
                oid_get_uint32(OID_NETWORK_RESTORE_HOST_DOMAIN_NAMES, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("  oid RESTORE_HOST_DOMAIN_NAMES returned %d\n", res);}
            res =
                oid_get_uint32(OID_NETWORK_CONNECTION_TIMEOUT, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid CONNECTION_TIME \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_LINK_SPEED, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid LINK_SPEED \t\t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_LINK_CONFIG, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid LINK_CONFIG \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_LPD_ENABLED, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid LPD_ENABLED \t\treturns %d\n", t_uint32_t); }
#ifdef HAVE_SLP 
            res =
                oid_get_uint32(OID_NETWORK_SLP_ENABLED, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid SLP_ENABLED \t\treturns %d\n", t_uint32_t); }
#endif
            res =
                oid_get_uint32(OID_NETWORK_IPV4_ENABLED, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_ENABLED \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_string(OID_NETWORK_IPV4_DOMAIN_NAME, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_DOMAIN_NAME \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_string(OID_NETWORK_IPV4_HOST_NAME, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_HOST_NAME \t\treturns '%s'\n", t_str); }
            res =
                oid_get_uint32(OID_NETWORK_IPV4_LAST_CONFIGURED_BY, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_LAST_CONFIGURED_BY \treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_IPV4_MANUAL_CONFIG, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_MANUAL_CONFIG \t\treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_ALLOW_DHCP_CONFIG \treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_IPV4_ALLOW_BOOTP_CONFIG, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_ALLOW_BOOTP_CONFIG \treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_IPV4_ALLOW_AUTOIP_CONFIG, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_ALLOW_AUTOIP_CONFIG \treturns %d\n", t_uint32_t); }
            res =
                oid_get_string(OID_NETWORK_IPV4_DHCP_SERVER_ADDR, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_DHCP_SERVER_ADDR \treturns '%s'\n", t_str); }
            res =
                oid_get_uint32(OID_NETWORK_IPV4_DHCP_LEASE_TIME, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_DHCP_LEASE_TIME \treturns %d\n", t_uint32_t); }
            res =
                oid_get_string(OID_NETWORK_IPV4_ADDRESS, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_ADDRESS \t\treturns '%s'\n", t_str); }
            res =
                oid_get_string(OID_NETWORK_IPV4_SUBNET_MASK, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_SUBNET_MASK \t\treturns '%s'\n", t_str); }
            res =
                oid_get_string(OID_NETWORK_IPV4_WINS_SERVER, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_WINS_SERVER \t\treturns '%s'\n", t_str); }
            res =
                oid_get_string(OID_NETWORK_IPV4_DEFAULT_GATEWAY, i, t_str, sizeof(t_str));
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_DEFAULT_GATEWAY \treturns '%s'\n", t_str); }
            res =
                oid_get_uint32(OID_NETWORK_IPV4_WINS_CONFIGURED, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid IPV4_WINS_CONFIGURED \treturns %d\n", t_uint32_t); }
            res =
                oid_get_uint32(OID_NETWORK_REACTIVATE_IFACE, i, &t_uint32_t);
            if (res!=OK){DBG_MSG("oid returned %d\n", res);}
            else { DBG_MSG("  oid REACTIVE_IFACE \treturns %d\n", t_uint32_t); }
        }

        arg = argv[next++]; // next argument
    }

    return OK;
}

#endif // DBG_ON

static const char* hex_cmd_usage = "[<#>], where # is 0 (off) 1..# <# of bytes>";
static const char* hex_cmd_desc  = "hex_dump # bytes";
int dumping_hex = 0; // off by default
int cmd_hex_dumping_cb(int argc, char* argv[]) // NET_WLAN_LINK_NAME
{
    char* arg = argv[1]; // 1st argument

    if (arg)
    {
        sscanf(arg, "%d", &dumping_hex); // update
    }
    else dumping_hex = 0; // turn off

    if (dumping_hex>0)
    {
        DBG_ERR("updated hex_dump(<addr>, %d bytes)\n", dumping_hex);
    }

    return dumping_hex;
}

// cmd registration(s) -- refer to build-time DBG_MSG binding
static void register_cmd_plugin()
{
    error_type_t cmd_res;

    // add net cmds
    cmd_res = cmd_register_cmd(CMD_BUCKET, NULL, NULL, NULL, NULL, NULL);
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s'\n", __func__, CMD_BUCKET);
    }

#if DBG_ON
    // net host(name)
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "host",
                                   host_cmd_desc,
                                   host_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_net_host_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "host");
    }

    // net stats
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "stats",
                                   stats_cmd_desc,
                                   stats_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_net_stats_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "stats");
    }

    static const char* links_cmd_usage = "";
    static const char* links_cmd_desc  = "retrieves current intf/link info";
    // net links
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "links",
                                   links_cmd_desc,
                                   links_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_net_links_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "links");
    }

    // net oids
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "oids",
                                   oids_cmd_desc,
                                   oids_cmd_usage,
                                   NULL, //notes,
                                   cmd_net_oids_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "oids");
    }

    // hex dump size setting
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "_hex",
                                   hex_cmd_desc,
                                   hex_cmd_usage,
                                   NULL, //notes,
                                   cmd_hex_dumping_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "oids");
    }

#endif // DBG_ON
}

// collects 'sh' cmd arguments and passes it onto the os
int cmd_sh_cb(int argc, char* argv[])
{
    char os_req[128] = { 0 };
    int next = 1;
    char* arg = NULL;

    while ((arg = argv[next++])) // add arguments upto max req length
    {
        if (strlen(os_req) + strlen(arg) < sizeof(os_req))
        {
            strcat(os_req, " ");
            strcat(os_req, arg);
        }
    }
    strcat(os_req, "\n");
    return (strlen(os_req) ? os_shell(os_req) : 0); // displays stdout & stderr
}

void add_general_cmds(void) // general cmd(s)
{
    error_type_t cmd_res;

    // add 'sh' cmd
    cmd_res = cmd_register_cmd("sh", NULL, NULL, NULL, NULL, cmd_sh_cb);
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- 'sh'\n", __func__);
    }
}

void register_net_cmds(void)
{
    add_general_cmds(); // add 'sh' cmd
    register_cmd_plugin();

return;
}

#ifdef __cplusplus
}
#endif

// eof
