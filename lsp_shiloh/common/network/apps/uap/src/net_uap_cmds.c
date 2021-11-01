/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
//  Provide a method to setup wireless advertisements (revisit w/ews)

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
#include "os_system.h"
#include "net_api.h"
#include "net_iface.h" // net_context_t
#include "oid_api.h" // oid_t
#include "net_uap_api.h" // propogate_uap_conf_change()
#include "net_uap_vars.h" // net_uap_notify_var_ssid
#include "wifi_intf_api.h"
#include "wifi_intf_vars.h"
#include "net_conf_api.h" // ipv4_b2str()
#include "net_intf_api.h" // get_intf_mac_addr()


#if 1 // build for both release and debug //#if defined DEBUG || defined HAVE_CMD

#define CMD_BUCKET "uap"

#include "error_types.h"
#define DBG_PRFX "wifi.uap: "
//off #define DBG_VERBOSE_ON
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#include "net_debug.h" // DBG_ family defaults

#undef DBG_MSG
#define DBG_MSG DBG_ERR /* raise DBG_MSG verbosity to 'error' */

static const char* on_cmd_usage = "";
static const char* on_cmd_desc  = "enables uap ON";
static const char* off_cmd_usage = "";
static const char* off_cmd_desc  = "disables uap OFF";
static const char* reset_cmd_usage = "";
static const char* reset_cmd_desc  = "resets uap factory settings";
static const char* ssid_cmd_usage = "[<update>]";
static const char* ssid_cmd_desc  = "retrieves uap ssid [<stores update>]";
static const char* info_cmd_usage = "";
static const char* info_cmd_desc  = "retrieves uap info";
static const char* dhcp_cmd_usage = "[<updated addr>]";
static const char* dhcp_cmd_desc  = "";
static const char* mask_cmd_usage = "[<updated subnet mask>]";
static const char* mask_cmd_desc  = "";

static uint8_t oid_ssid_u8[MAX_SSID_NAME_LEN];
static char*   oid_ssid_str = (char*)oid_ssid_u8; // cast u8 -> char*

// sets wireless (uap) on (if not already) -- restarts airprint/bonjour
int cmd_uap_set_on_cb(int argc, char* argv[])
{
    //UAP_LOCK_CONTEXT();
    bool enabled = wifi_uap_enabled();
    UNUSED_VAR(enabled);
    error_type_t res = ~OK;

    // retrieve current ssid
    res = oid_get_string(OID_UAP_INTF_SSID, 0, oid_ssid_str, MAX_SSID_NAME_LEN);
    if (res != OK)
    {
        DBG_ERR("%s enabled %d?\n", (oid_ssid_str[0]?(char*)oid_ssid_str:"<undefined?>"), enabled);
        ASSERT(res == OK);
    }
    DBG_MSG("'%s' %s -> enabled", oid_ssid_str, (enabled?"enabled":"disabled"));

    // propogate change
    oid_set_uint32(OID_UAP_INTF_ENABLED, 0, 1); 

    net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_UAP);
    if (intf)
    { // enable ipv4 in general oids
        int active_intf = if_nametoindex(NET_IF_NAME_UAP);
        res |= oid_set_uint32(OID_NETWORK_IPV4_ENABLED, active_intf, 1);

        // restart mdns (revisit time and place)
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_RENEW, NET_SERVICE_UAP, NULL, NULL);
#else
        start_uap_service(); // initiate uap online
#endif // HAVE_NET_SERVICES
        net_iface_notify(intf, NET_LINK_EVENT_UP);
    }

    //UAP_UNLOCK_CONTEXT();
    return res;
}

// force uap off (if not already)
int cmd_uap_set_off_cb(int argc, char* argv[])
{
    //UAP_LOCK_CONTEXT();
    bool enabled = wifi_uap_enabled();
    UNUSED_VAR(enabled);
    error_type_t res = ~OK;

    // retrieve current oids
    res = oid_get_string(OID_UAP_INTF_SSID, 0, oid_ssid_str, MAX_SSID_NAME_LEN);
    if (res != OK)
    {
        DBG_ERR("'%s' enabled %d?\n", (oid_ssid_u8[0]?(char*)oid_ssid_u8:"<undefined?>"), enabled);
        ASSERT(res == OK);
    }
    DBG_MSG("'%s' %s -> disabled", oid_ssid_str, (enabled?"enabled":"disabled"));

    // propogate change
    int if_index = if_nametoindex(NET_IF_NAME_UAP);
    oid_set_uint32(OID_UAP_INTF_ENABLED, 0, 0); // off
    net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_UAP);
    if (intf)
    { // disable ipv4 in general oids
        res |= oid_set_uint32(OID_NETWORK_IPV4_ENABLED, if_index, 0);

#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_STOP, NET_SERVICE_UAP, NULL, NULL);
#else
        stop_uap_service();
#endif
        net_iface_notify(intf, NET_LINK_EVENT_DOWN);
    }

    //UAP_UNLOCK_CONTEXT();
    return res;
}

// retrieves current uap info
int cmd_uap_info_cb(int argc, char* argv[])
{
    //UAP_LOCK_CONTEXT();
    int active_if = 0; // 1st uap interface
    error_type_t res = ~OK;

    // retrieve current ssid
    res = oid_get_string(OID_UAP_INTF_SSID, active_if, oid_ssid_str, MAX_SSID_NAME_LEN);
    if (res != OK)
    {
        DBG_ERR("'%s' enabled %d?\n",
            (oid_ssid_u8[0]?oid_ssid_str:"<undefined?>"), wifi_uap_enabled());
        ASSERT(res == OK);
    }

    char oid_dhcp_addr[16];
    oid_get_string(OID_WIFI_UAP_DHCP_ADDR, active_if, oid_dhcp_addr, 16);
    DBG_MSG("'%s' %s w/dhcp server %s\n",
        (oid_ssid_u8[0]?(char*)oid_ssid_str:"<unbound?>"),
        (wifi_uap_enabled()?"enabled":"Disabled"), oid_dhcp_addr);

    // retrieve current oid
    oid_get_string(OID_WIFI_UAP_DHCP_START_ADDR, active_if, oid_dhcp_addr, 16);
    DBG_MSG("     start: %s\n", oid_dhcp_addr);

    oid_get_string(OID_WIFI_UAP_DHCP_END_ADDR, active_if, oid_dhcp_addr, 16); 
    DBG_MSG("     last:  %s\n", oid_dhcp_addr);

    char oid_dhcp_subnet[16];
    oid_get_string(OID_WIFI_UAP_DHCP_SUBNET_MASK, active_if, oid_dhcp_subnet, 16);
    DBG_MSG("     subnet mask:  %s\n", oid_dhcp_subnet);

    //UAP_UNLOCK_CONTEXT();
    return 0;
}

// trigger uap-settings update
void trigger_uap_config(oid_t oid)
{
    switch (oid)
    {
        case OID_WIFI_UAP_DHCP_ADDR:
        case OID_WIFI_UAP_DHCP_SUBNET_MASK:
        case OID_UAP_INTF_SSID:
        {
            char ssid[MAX_SSID_NAME_LEN];
            oid_get_string(OID_UAP_INTF_SSID, 0, ssid, MAX_SSID_NAME_LEN);
            uap_update_conf_files(ssid);

            uint32_t enabled = false;
            oid_get_uint32(OID_UAP_INTF_ENABLED, 0, &enabled);
            if ((bool)enabled)
            {
#ifdef HAVE_NET_SERVICES
                net_service_request(NET_SERVICE_CONFIG, NET_SERVICE_UAP, (void*)ssid, NULL);
#else
                start_uap_service(); // cast
#endif
            }
            DBG_VERBOSE("%s() oid x%x (enabled %d)..\n", __func__, (unsigned int)oid, enabled);
        } break;

        default:
            DBG_ERR("%s() ignored change x%x\n", __func__, oid);
        break;
    }
}

// retrieves (and possibly modifies) current udhcp addr
int cmd_uap_udhcp_cb(int argc, char* argv[])
{
    //UAP_LOCK_CONTEXT();
    int active_if = 0;
    error_type_t res = ~OK;
    uint32_t current_dhcp_server = 0;

    // retrieve current dhcp server addr
    res = oid_get_uint32(OID_WIFI_UAP_DHCP_ADDR, active_if, &current_dhcp_server);
    DBG_VERBOSE("dhcp server: %s\n", ipv4_b2str(current_dhcp_server));

    // update w/valid updated server addr
    int next = 1;
    if (argv[next])
    {
        uint32_t updated_dhcp_addr = ipv4_str2b32(argv[next]);

        // update if changed
        if (updated_dhcp_addr != current_dhcp_server)
        {
            wifi_intf_udhcp_set_var_server_addr(active_if, updated_dhcp_addr);
            oid_get_uint32(OID_WIFI_UAP_DHCP_ADDR, active_if, &updated_dhcp_addr);

            trigger_uap_config(OID_WIFI_UAP_DHCP_ADDR);
        }
        else
        {
            DBG_ERR("no change dhcp addr '%s' ..\n", argv[next]);
        }
    }

    //UAP_UNLOCK_CONTEXT();
    return 0;
}

// retrieves current uap dhcp subnet mask (updates if change provided)
int cmd_uap_subnet_mask_cb(int argc, char* argv[])
{
    //UAP_LOCK_CONTEXT();
    int active_if = 0;
    error_type_t res = ~OK;
    char current_subnet_mask[16];

    // retrieve current subnet mask
    res = oid_get_string(OID_WIFI_UAP_DHCP_SUBNET_MASK, active_if, current_subnet_mask, 16);
    DBG_VERBOSE("udhcp subnet mask %s\n", current_subnet_mask);

    // update w/valid updated server addr
    int next = 1;
    if (argv[next])
    {
        char* updated_subnet = argv[next];
        uint32_t updated_subnet_mask = ipv4_str2b32(updated_subnet);

        // update if changed
        if (updated_subnet_mask != ipv4_str2b32(current_subnet_mask))
        {
            oid_set_string(OID_WIFI_UAP_DHCP_SUBNET_MASK, active_if, updated_subnet, 16);
            trigger_uap_config(OID_WIFI_UAP_DHCP_SUBNET_MASK);
        }
        else
        {
            DBG_ERR("no change udhcp subnet '%s' ..\n", argv[next]);
        }
    }

    cmd_uap_info_cb(0, NULL);

    //UAP_UNLOCK_CONTEXT();
    return 0;
}

int cmd_uap_ssid_cb(int argc, char* argv[])
{
    //UAP_LOCK_CONTEXT();
    int active_if = 0;
    int next = 1; // 1-based index
    uint8_t* ssid_update = (uint8_t*)argv[next]; // cast

    // retrieve current oid
    oid_get_string(OID_UAP_INTF_SSID, active_if, oid_ssid_str, MAX_SSID_NAME_LEN);
    DBG_MSG("ssid '%s'\n", oid_ssid_str);

    if (ssid_update && ssid_update[0] &&
        strcmp(oid_ssid_str, (char*)ssid_update)) // defined && different
    {
        DBG_MSG("OID_UAP_INTF_SSID from %s -> %s\n", oid_ssid_str, ssid_update);

        // update uap's ssid
        uap_set_var_ssid(active_if, ssid_update, MAX_SSID_NAME_LEN); // update perm var

        // retrieve latest uap ssid
        memset(oid_ssid_str, 0, MAX_SSID_NAME_LEN);
        oid_get_string(OID_UAP_INTF_SSID, active_if, oid_ssid_str, MAX_SSID_NAME_LEN);

        if (strcmp(oid_ssid_str, (char*)ssid_update)) // incomplete change?
        {
            DBG_ERR("'%s' != '%s'\n", ssid_update, oid_ssid_str);
        }
        else
        {
            DBG_VERBOSE("OID_UAP_INTF_SSID - %s\n", oid_ssid_str);
            trigger_uap_config(OID_UAP_INTF_SSID);
        }
    }

    //UAP_UNLOCK_CONTEXT();
    return 0;
}

// resets uap settings to factory defaults
int cmd_uap_reset_factory_settings_cb(int argc, char* argv[])
{
    //UAP_LOCK_CONTEXT();
    int active_if = 0;

    oid_set_string(OID_UAP_INTF_SSID, active_if, DEFAULT_UAP_SSID, strlen(DEFAULT_UAP_SSID));
    //tbd oid_set_string(OID_UAP_INTF_SSID_PREFIX, active_if, DEFAULT_UAP_SSID_PREFIX, strlen(DEFAULT_UAP_SSID_PREFIX));
    oid_set_string(OID_WIFI_UAP_GET_DHCP_ADDR, active_if, UAP_DEFAULT_STARTING_ADDRESS, strlen(UAP_DEFAULT_STARTING_ADDRESS));
    oid_set_string(OID_WIFI_UAP_DHCP_ADDR, active_if, UAP_DEFAULT_STARTING_ADDRESS, strlen(UAP_DEFAULT_STARTING_ADDRESS));
    oid_set_string(OID_WIFI_UAP_DHCP_START_ADDR, active_if, UAP_DEFAULT_STARTING_ADDRESS, strlen(UAP_DEFAULT_STARTING_ADDRESS));
    // #define OID_WIFI_UAP_DHCP_ADDR_PREFIX   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 52, OID_TYPE_STRING)
    oid_set_string(OID_WIFI_UAP_DHCP_SUBNET_MASK, active_if, UAP_DEFAULT_SUBNET_MASK, strlen(UAP_DEFAULT_SUBNET_MASK));
    oid_set_string(OID_WIFI_UAP_DHCP_END_ADDR, active_if, UAP_DEFAULT_ENDING_ADDRESS, strlen(UAP_DEFAULT_ENDING_ADDRESS)); // revisit use range and compute value?
    oid_set_uint32(OID_WIFI_UAP_DHCP_NUM_ADDRS, active_if, UAP_UDHCP_ADDRESS_RANGE);
    oid_set_uint32(OID_WIFI_UAP_DHCP_LEASE_TIME, active_if, UAP_MAX_UDHCP_LEASE_TIME);
    oid_set_uint32(OID_UAP_INTF_ENABLED, active_if, true);

    // echo factory settings
    cmd_uap_info_cb(0, NULL);

    //UAP_UNLOCK_CONTEXT();
    return OK;
}

static void register_cmd_plugin()
{
    error_type_t cmd_res;

    // add uap cmds
    cmd_res = cmd_register_cmd(CMD_BUCKET, NULL, NULL, NULL, NULL, NULL);
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // note
    {
        DBG_ERR("%s() failed registration -- '%s'\n", __func__, CMD_BUCKET);
    }

    // enable uap service
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "on",
                                   on_cmd_desc,
                                   on_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_uap_set_on_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "on");
    }

    // disable uap service
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "off",
                                   off_cmd_desc,
                                   off_cmd_usage,
                                   NULL, //notes,
                                   cmd_uap_set_off_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "off");
    }

    // uap ssid name
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "ssid",
                                   ssid_cmd_desc,
                                   ssid_cmd_usage,
                                   NULL, //notes,
                                   cmd_uap_ssid_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "ssid");
    }

    // uap service status
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "_reset", // hidden
                                   reset_cmd_desc,
                                   reset_cmd_usage,
                                   NULL, //notes,
                                   cmd_uap_reset_factory_settings_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "reset");
    }

    // uap service status
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "info",
                                   info_cmd_desc,
                                   info_cmd_usage,
                                   NULL, //notes,
                                   cmd_uap_info_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "info");
    }

    // uap dhcp settings
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "udhcp",
                                   dhcp_cmd_desc,
                                   dhcp_cmd_usage,
                                   NULL, //notes,
                                   cmd_uap_udhcp_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "dhcp");
    }

    // uap dhcp subnet mask
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "subnet",
                                   mask_cmd_desc,
                                   mask_cmd_usage,
                                   NULL, //notes,
                                   cmd_uap_subnet_mask_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "subnet");
    }
}

#else
#define register_cmd_plugin() { (void)0; }
#endif // HAVE_CMD || DEBUG

void register_uap_cmds(void)
{
    register_cmd_plugin();

return;
}

#ifdef __cplusplus
}
#endif

// eof
