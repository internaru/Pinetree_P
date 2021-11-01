/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
//  Provide a method to setup wireless advertisements (an alternative to ews)

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
#include "oid_api.h" // oid_t
#include "net_api.h"
#include "net_link_api.h" // net_link_send_msg_no_wait()
#include "net_iface.h" // net_interface_t
#include "net_wifi_api.h" // propogate_wifi_conf_change()
#include "net_wifi_vars.h" // net_wifi_notify_var_ssid_name
#include "wifi_intf_api.h" // MAX_SSID_NAME_LEN
#include "wifi_intf_vars.h" // wifi_intf_get_var_..
#include "net_intf_api.h" // get_intf_mac_addr()

#if 1 // include for both release and debug //#if defined DEBUG || defined HAVE_CMD

#define CMD_BUCKET "wifista" /*"wifi" " " "sta"*/

#include "error_types.h"
#define DBG_PRFX "wifi.sta: "
#define DBG_VERBOSE_ON
#define DBG_ERR_ON
#include "net_debug.h" // DBG_ family defaults

static const char* on_cmd_usage = "";
static const char* on_cmd_desc  = "sets " NET_WLAN_LINK_NAME " ON";
static const char* off_cmd_usage = "";
static const char* off_cmd_desc  = "sets " NET_WLAN_LINK_NAME " OFF";
static const char* info_cmd_usage = "";
static const char* info_cmd_desc  = "display current " NET_WLAN_LINK_NAME " settings";
static const char* ssid_cmd_usage = "[<update>]";
static const char* ssid_cmd_desc  = "retrieves " NET_WLAN_LINK_NAME " ssid [<stores update>]";
static const char* bssid_cmd_usage = "[<update>]";
static const char* bssid_cmd_desc  = "retrieves " NET_WLAN_LINK_NAME " bssid [<stores update>]";
static const char* passphrase_cmd_usage = "[<update>]";
static const char* passphrase_cmd_desc  = "retrieves " NET_WLAN_LINK_NAME " wpa passphrase [<stores update>]";
static const char* reset_cmd_usage = "";
static const char* reset_cmd_desc  = "resets " NET_WLAN_LINK_NAME "factory settings";

// create a virtual union { char* str_version; uint8_t u8_version[] };
static uint8_t oid_ssid_u8[MAX_SSID_NAME_LEN];
static char* oid_ssid = (char*) oid_ssid_u8; // cast


// restart wifi interface with recent changes
int restart_wifi_if(unsigned active_intf)
{
    net_link_t* wifi_link = 0; // get_active_wifi_link();
    if (wifi_link) {
        net_link_send_msg_no_wait(NET_LINK_MSG_ENABLE_LINKS, wifi_link, wifi_link->logical_link, NULL);
    }
    return OK;
}

char* get_active_wifi_link_name()
{
    // prefer uap
    if (intf_up(NET_UAP_LINK_NAME) && intf_active(NET_UAP_LINK_NAME))
        return NET_UAP_LINK_NAME;

    // default wlan
    return NET_WLAN_LINK_NAME;
}

bool delay_wifi_service_restart(long ms_delay)
{
    return (initiate_wifi_service_timer(ms_delay) ? true : false);
}

// trigger wifi-settings update
void trigger_wifi_config(oid_t oid)
{
    delay_wifi_service_restart(WIFI_SERVICE_RESTART_DELAY); // allow accumulated changes
    unsigned active_if = 0;
    bool enabled = false;
    wifi_intf_get_var_is_enabled(active_if, &enabled);

#ifdef DBG_VERBOSE_ON
    char* oid_literal = get_oid_literal(oid);
    
    if (!oid_literal)
    {
        char literal_name[64];
        oid_literal = literal_name;
        sprintf(oid_literal, "<wifi oid x%x>", oid);
        DBG_VERBOSE("updated %s\n", oid_literal);
    }
#endif

    switch (oid)
    {
        case OID_WIFI_INTF_ENABLED:
        case OID_WIFI_INTF_SSID:
        case OID_WIFI_INTF_BSSID:
        case OID_WIFI_INTF_WPA_PASSPHRASE:
        {
            char ssid[MAX_SSID_NAME_LEN];
            oid_get_string(OID_WIFI_INTF_SSID, active_if, ssid, MAX_SSID_NAME_LEN);
            
#ifdef HAVE_NET_SERVICES
            net_service_request(NET_SERVICE_CONFIG, NET_SERVICE_STA, NULL, NULL);
#else
            wifi_write_conf(ssid);
            DBG_VERBOSE("%s() oid x%x (enabled %d)..\n", __func__, (unsigned int)oid, enabled);
            if (enabled) restart_wifi_if(active_if);
#endif
        } break;

        default:
            DBG_ERR("%s() passthru x%x\n", __func__, oid);
        break;
    }
}

// force wireless on (if not already)
int cmd_wifi_set_on_cb(int argc, char* argv[])
{
    //WIFI_LOCK_CONTEXT();
    int active_intf = 0;
    uint32_t wifi_enabled = false;
    error_type_t res = FAIL;

    // retrieve current vars
    res  = oid_get_string(OID_WIFI_INTF_SSID, active_intf, oid_ssid, MAX_SSID_NAME_LEN);
    res |= oid_get_uint32(OID_WIFI_INTF_ENABLED, active_intf, &wifi_enabled);
    if (res != OK)
    {
        DBG_ERR("%s enabled %d? (res %d)\n", ((oid_ssid && oid_ssid[0])?oid_ssid:"<undefined?>"), wifi_enabled, res);
    }
    if (oid_ssid && oid_ssid[0])
    {
        DBG_VERBOSE("'%s' %s -> ", oid_ssid, (wifi_enabled?"enabled":"disabled"));
    }

    // force-ful change (regardless of current state) -- update state
    net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_STA);
    if (intf)
    {
        wifi_hw_set_var_is_enabled(active_intf, 1);
        wifi_intf_set_var_is_enabled(active_intf, 1);
        { // enable ipv4 in general oids
            active_intf = intf->local_idx;
            res |= oid_set_uint32(OID_NETWORK_IPV4_ENABLED, active_intf, 1);

            // enable interface as well
            net_iface_set_var_intf_enabled(intf->treck_idx /*if_index*/, true);
        }
        oid_get_uint32(OID_WIFI_INTF_ENABLED, 0, &wifi_enabled);
    }
    DBG_VERBOSE("%s\n", (wifi_enabled?"enabled":"disabled"));

    if (wifi_enabled)
    {
        DBG_VERBOSE("%s\n", (wifi_enabled?"enabled":"disabled"));
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_RENEW, NET_SERVICE_STA, NULL, NULL);
#else // ! HAVE_NET_SERVICES
        start_sta_service(); // bring sta online
#endif // ! HAVE_NET_SERVICES
        net_iface_notify(intf, NET_LINK_EVENT_UP);
    }

    //WIFI_UNLOCK_CONTEXT();
    return 0;
}

// force wireless off (if not already)
int cmd_wifi_set_off_cb(int argc, char* argv[])
{
    //WIFI_LOCK_CONTEXT();
    int active_intf = 0; // 1st wifi interface -- revisit lookup 'wifi interface of interest'?
    uint32_t wifi_enabled = 0;
    error_type_t res = FAIL;

    // retrieve current vars
    res  = oid_get_string(OID_WIFI_INTF_SSID, active_intf, oid_ssid, MAX_SSID_NAME_LEN);
    res |= oid_get_uint32(OID_WIFI_INTF_ENABLED, active_intf, &wifi_enabled);
    if (res != OK)
    {
        DBG_ERR("'%s' enabled %d?\n", (oid_ssid[0]?oid_ssid:"<undefined?>"), wifi_enabled);
        ASSERT(res == OK);
    }
    if (oid_ssid && oid_ssid[0])
    {
        DBG_VERBOSE("'%s' %s -> ", oid_ssid, (wifi_enabled?"enabled":"disabled"));
    }

    // take sta down regardless of 'enable' settings
    {
        wifi_intf_set_var_is_enabled(active_intf, 0);
//        wifi_hw_set_var_is_enabled(active_intf, 0);
        net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_STA);
        if (intf)
        { // disable ipv4 in general oids
            active_intf = intf->local_idx;
            res |= oid_set_uint32(OID_NETWORK_IPV4_ENABLED, active_intf, 0);

            // disable interface as well
            net_iface_set_var_intf_enabled(intf->treck_idx /*if_index*/, false);
        }
        oid_get_uint32(OID_WIFI_INTF_ENABLED, 0, &wifi_enabled); // named oid
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_RENEW, NET_SERVICE_STA, NULL, NULL);
#else
        stop_sta_service();
#endif
        net_iface_notify(intf, NET_LINK_EVENT_DOWN);
    }
    DBG_VERBOSE("%s\n", (wifi_enabled?"enabled":"disabled"));

    //WIFI_UNLOCK_CONTEXT();
    return 0;
}


// retrieve wifi ssid -- update if given
int cmd_wifi_ssid_name_cb(int argc, char* argv[])
{
    //WIFI_LOCK_CONTEXT();
    int active_intf = 0;
    int next = 1; // 1-based index
    char* ssid_update = argv[next];

    // retrieve current oid
    oid_get_string(OID_WIFI_INTF_SSID, active_intf, oid_ssid, MAX_SSID_NAME_LEN);

    if (ssid_update && ssid_update[0] && strcmp(oid_ssid, ssid_update)) // non-""
    {
        DBG_VERBOSE("OID_WIFI_INTF_SSID -> %s\n", ssid_update);

        // update wifi ssid to match ..
        wifi_intf_set_var_ssid(active_intf, (uint8_t*)ssid_update, MAX_SSID_NAME_LEN); // cast

        // retrieve latest
        oid_get_string(OID_WIFI_INTF_SSID, active_intf, oid_ssid, MAX_SSID_NAME_LEN);

        if (strcmp(oid_ssid, ssid_update)) // incomplete update?
        {
            DBG_ERR("'%s' != '%s'\n", ssid_update, oid_ssid);
            //WIFI_UNLOCK_CONTEXT();
            return FAIL;
        }
        trigger_wifi_config(OID_WIFI_INTF_SSID);
    }

    DBG_VERBOSE("ssid (intf %d) '%s'\n", active_intf, oid_ssid);

    //WIFI_UNLOCK_CONTEXT();
    return 0;
}

int cmd_wifi_bssid_cb(int argc, char* argv[])
{
    //WIFI_LOCK_CONTEXT();
    int active_intf = 0;
    int next = 1; // 1-based index
    char* name_update = argv[next];
    bool bssid_set = false;

    // retrieve current oid
    wifi_intf_get_var_bssid_set(active_intf, &bssid_set);
    oid_get_string(OID_WIFI_INTF_BSSID, active_intf, oid_ssid, MAX_SSID_NAME_LEN);
    DBG_VERBOSE("bssid '%s' (set %d)\n", oid_ssid, bssid_set);

    if (name_update && name_update[0] && strcmp(oid_ssid, name_update)) // non-""
    {
        DBG_VERBOSE("OID_WIFI_INTF_BSSID -> %s\n", name_update);

        // update change
        wifi_intf_set_var_bssid(active_intf, (const uint8_t*)name_update, MAX_SSID_NAME_LEN); // cast
        wifi_intf_set_var_bssid_set(active_intf, true);

        // retrieve latest
        oid_get_string(OID_WIFI_INTF_BSSID, active_intf, oid_ssid, MAX_SSID_NAME_LEN);

        if (strcmp(oid_ssid, name_update)) // mismatched
        {
            DBG_ERR("'%s' != '%s'\n", name_update, oid_ssid);
            //WIFI_UNLOCK_CONTEXT();
            return FAIL;
        }
        else
        {
            DBG_VERBOSE("OID_WIFI_INTF_BSSID - %s\n", oid_ssid);
            trigger_wifi_config(OID_WIFI_INTF_BSSID);
        }
    }

    //WIFI_UNLOCK_CONTEXT();
    return 0;
}

int cmd_wifi_passphrase_cb(int argc, char* argv[])
{
    //WIFI_LOCK_CONTEXT();
    int active_intf = 0;
    int next = 1; // 1-based index
    char* passphrase_update = argv[next];

    uint8_t passphrase_proper[MAX_PASS_PHRASE+1];
    char* passphrase = (char*)&passphrase_proper; // unionize uint8_t* <-> char*

    wifi_intf_get_var_passphrase(active_intf, passphrase_proper, MAX_PASS_PHRASE);
    passphrase_proper[MAX_PASS_PHRASE] = 0;

    DBG_VERBOSE("passphrase '%s'\n", passphrase);

    if (passphrase_update && passphrase_update[0] && strcmp(passphrase, passphrase_update)) // non-""
    {
        // update change
        wifi_intf_set_var_passphrase((uint8_t)active_intf, (const uint8_t*)passphrase_update, (uint8_t)MAX_PASS_PHRASE); // cast

        // retrieve latest
        wifi_intf_get_var_passphrase((uint8_t)active_intf, passphrase_proper, (uint8_t)MAX_PASS_PHRASE); // revisit oid_get_string(OID_WIFI_INTF_WPA_PASSPHRASE, active_intf, passphrase, MAX_PASS_PHRASE);

        if (strcmp(passphrase, passphrase_update)) // incomplete update?
        {
            DBG_ERR("'%s' != '%s'\n", passphrase_update, passphrase);
            //WIFI_UNLOCK_CONTEXT();
            return FAIL;
        }
        else
        {
            DBG_VERBOSE("OID_WIFI_INTF_WPA_PASSPHRASE - %s\n", passphrase);
            trigger_wifi_config(OID_WIFI_INTF_WPA_PASSPHRASE);
        }
    }

    //WIFI_UNLOCK_CONTEXT();
    return 0;
}

// local stubs
#define tbd dbg_printf("%s() impl?\n", __func__); return OK;
int cmd_wifi_dhcp_name_cb(int argc, char* argv[]) { tbd; }
int cmd_wifi_wep_keys_name_cb(int argc, char* argv[]) { tbd; }

// retrieves current wifi info
int cmd_wifi_info_cb(int argc, char* argv[])
{
    //WIFI_LOCK_CONTEXT();
    int active_if = 0;
    bool wifi_enabled = false;
    error_type_t res = FAIL;
    char oid_ssid[MAX_SSID_NAME_LEN];

    // retrieve current vars
    res  = oid_get_string(OID_WIFI_INTF_SSID, active_if, oid_ssid, MAX_SSID_NAME_LEN);
    res |= wifi_intf_get_var_is_enabled(active_if, &wifi_enabled);
    if (res != OK)
    {
        DBG_ERR("'%s' enabled %d?\n", (oid_ssid[0]?(char*)oid_ssid:"<undefined?>"), wifi_enabled);
        ASSERT(res == OK);
    }
    dbg_printf("ssid: '%s' %s\n", oid_ssid, (wifi_enabled? "enabled" : "NOT enabled"));

    // retrieve current passphrase
    char oid_wifi_passphrase[MAX_PASS_PHRASE];
    oid_get_string(OID_WIFI_INTF_WPA_PASSPHRASE, active_if, oid_wifi_passphrase, MAX_PASS_PHRASE);
    DBG_VERBOSE("passphrase: '%s'\n", oid_wifi_passphrase);

    //WIFI_UNLOCK_CONTEXT();
    return 0;
}

int cmd_wifi_reset_factory_settings_cb(int argc, char* argv[]) // NET_WLAN_LINK_NAME
{
    int active_if = 0;
    oid_set_string(OID_WIFI_INTF_SSID_PREFIX, active_if, DEFAULT_WIFI_SSID_PREFIX, strlen(DEFAULT_WIFI_SSID_PREFIX));
    oid_set_uint32(OID_WIFI_INTF_SEC_MODE, active_if,         0); // NONE?
    oid_set_uint32(OID_WIFI_INTF_WPA_ENCR_MODE, active_if,    0); // NONE?
    oid_set_string(OID_WIFI_INTF_WPA_PASSPHRASE, active_if,"password", strlen("password"));
    oid_set_uint32(OID_WIFI_INTF_WEP_AUTH_MODE, active_if,    0); // NONE?
    oid_set_uint32(OID_WIFI_INTF_WEP_KEY_LEN, active_if,      0); // empty
    oid_set_string(OID_WIFI_INTF_WEP_KEY_1, active_if,   NULL, 0); // empty
    oid_set_string(OID_WIFI_INTF_WEP_KEY_2, active_if,   NULL, 0); // empty
    oid_set_string(OID_WIFI_INTF_WEP_KEY_3, active_if,   NULL, 0); // empty
    oid_set_string(OID_WIFI_INTF_WEP_KEY_4, active_if,   NULL, 0); // empty
    oid_set_uint32(OID_WIFI_INTF_WEP_CUR_KEY, active_if,     0); // empty
    oid_set_uint32(OID_WIFI_INTF_WEP_CUR_KEY_VALUE, active_if, (uint32_t)NULL); // empty
    oid_set_uint32(OID_WIFI_INTF_CHANNEL, active_if,         1); // why 1 and not 0? revisit
    oid_set_uint32(OID_WIFI_INTF_SIGNAL_STRENGTH, active_if, 0); // off
    { // mac addr
        uint8_t intf_mac_addr_u8[8], *mac = get_intf_mac_addr(NET_WLAN_LINK_NAME, intf_mac_addr_u8);
        oid_set_string(OID_WIFI_INTF_MAC_ADDR, active_if, (char*)mac, 8); // cast
    }
    {
        uint8_t intf_mac_addr_u8[8], *mac = get_intf_mac_addr(NET_WLAN_LINK_NAME, intf_mac_addr_u8);
        oid_set_string(OID_WIFI_INTF_BSSID, active_if, (char*)mac, 8); // cast
    }
    oid_set_uint32(OID_WIFI_INTF_LOCALE, active_if, 0); // revisit default 'en-us'
    oid_set_uint32(OID_WIFI_INTF_NUM_ATTACHED_DEVICES, active_if, 1); // how many can there be? 1-time or dynamic?
    return OK;
}

// add wifi cmds
static void register_cmd_plugin()
{
    error_type_t cmd_res;

    // add wifi cmds
    cmd_res = cmd_register_cmd(CMD_BUCKET, NULL, NULL, NULL, NULL, NULL);
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s'\n", __func__, CMD_BUCKET);
    }

    // wifi enable
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "on",
                                   on_cmd_desc,
                                   on_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_wifi_set_on_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "on");
    }

    // wifi disable
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "off",
                                   off_cmd_desc,
                                   off_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_wifi_set_off_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "off");
    }

    // wifi info
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "info",
                                   info_cmd_desc,
                                   info_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_wifi_info_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "info");
    }

    // wifi ssid
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "ssid",
                                   ssid_cmd_desc,
                                   ssid_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_wifi_ssid_name_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "ssid");
    }

    // wifi bssid
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "bssid",
                                   bssid_cmd_desc,
                                   bssid_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_wifi_bssid_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "bssid");
    }

    // wifi wpa passphrase
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "passphrase",
                                   passphrase_cmd_desc,
                                   passphrase_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_wifi_passphrase_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "passphrase");
    }

    // wifi reset to factory defaults
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "_reset", // hidden
                                   reset_cmd_desc,
                                   reset_cmd_usage,
                                   NULL, //notes,
                                   cmd_wifi_reset_factory_settings_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "reset");
    }
}

#else
#define register_cmd_plugin() { (void)0; }
#endif // HAVE_CMD || DEBUG

void register_wifi_cmds(void)
{
    register_cmd_plugin();

return;
}

#ifdef __cplusplus
}
#endif

// eof
