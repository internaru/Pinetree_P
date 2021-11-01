/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
//  Provide a method to setup mDNS/bonjour advertisements (revisit w/ews)

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
#include "os_system.h" // os_system()
#include "net_api.h" // INITD
#include "net_mdns_vars.h"

#if 1 // include for both release and debug //#if defined DEBUG || defined HAVE_CMD

#define CMD_BUCKET "mdns"

#include "error_types.h"
#define DBG_PRFX "mdns.cmd: "
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
#define DBG_ERR_ON
#include "net_debug.h" // DBG_ family defaults

const char* on_cmd_usage = "";
const char* on_cmd_desc  = "turns bonjour ON";
const char* off_cmd_usage = "";
const char* off_cmd_desc  = "turns bonjour OFF";
const char* info_cmd_usage = "[<yes|no|true|false|1|0>]";
const char* info_cmd_desc  = "retrieves bonjour enabled?";
const char* service_cmd_usage = "[<update>]";
const char* service_cmd_desc  = "retrieves bonjour service name [<stores update>]";
static char oid_service_name[NET_MDNS_SERVICE_NAME_SIZE];

// retrieves current info
int oid_bonjour_info_cb(int argc, char* argv[])
{
    //MDNS_LOCK_CONTEXT();
    int active_intf = 0;
    bool mdns_enabled = false;
    error_type_t res = ~OK;

    // retrieve current vars
    res  = net_mdns_get_var_service_name(active_intf, oid_service_name, NET_MDNS_SERVICE_NAME_SIZE);
    res |= net_mdns_get_var_enabled(active_intf, &mdns_enabled);
    if (res != OK)
    {
        DBG_ERR("'%s' enabled %d?\n", (oid_service_name[0]?oid_service_name:"<undefined?>"), mdns_enabled);
        DBG_ASSERT(res == OK);
        return FAIL;
    }

    // retrieve current service and running status
    oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, active_intf, oid_service_name, NET_MDNS_SERVICE_NAME_SIZE);
    DBG_ERR("'%s' %s %srunning\n", oid_service_name,
        (mdns_enabled?"enabled":"disabled"),
        (mdns_running()?"":"NOT "));

    //MDNS_UNLOCK_CONTEXT();
    return 0;
}

// force airprint/bonjour on (if not already)
int oid_bonjour_set_on_cb(int argc, char* argv[])
{
    //MDNS_LOCK_CONTEXT();
    int active_intf = 0;
    bool mdns_enabled = false;
    error_type_t res = ~OK;

    // retrieve current vars
    res  = net_mdns_get_var_service_name(active_intf, oid_service_name, NET_MDNS_SERVICE_NAME_SIZE);
    res |= net_mdns_get_var_enabled(active_intf, &mdns_enabled);
    if (res != OK)
    {
        DBG_ERR("%s enabled %d?\n", (oid_service_name[0]?oid_service_name:"<undefined?>"), mdns_enabled);
        DBG_ASSERT(res == OK);
    }

    DBG_MSG("'%s' %s -> ", oid_service_name, (mdns_enabled?"enabled":"disabled"));

    // update 'enabled' if changed
    if (!mdns_enabled)
    {
        net_mdns_set_var_enabled(active_intf, true);
    }

    oid_bonjour_info_cb(0, NULL); // report settings

    //MDNS_UNLOCK_CONTEXT();
    return 0;
}

// force airprint/bonjour off (if not already)
int oid_bonjour_set_off_cb(int argc, char* argv[])
{
    //MDNS_LOCK_CONTEXT();
    int active_intf = 0;
    bool mdns_enabled = false;
    error_type_t res = ~OK;

    // retrieve current vars
    res  = net_mdns_get_var_service_name(active_intf, oid_service_name, NET_MDNS_SERVICE_NAME_SIZE);
    res |= net_mdns_get_var_enabled(active_intf, &mdns_enabled);
    if (res != OK)
    {
        DBG_ERR("'%s' enabled %d?\n", (oid_service_name[0]?oid_service_name:"<undefined?>"), mdns_enabled);
        DBG_ASSERT(res == OK);
        return FAIL;
    }
    DBG_MSG("'%s' %s -> ", oid_service_name, (mdns_enabled?"enabled":"disabled"));

    if (mdns_enabled)
    {
        net_mdns_set_var_enabled(active_intf, false);
    }

    oid_bonjour_info_cb(0, NULL); // report settings

    //MDNS_UNLOCK_CONTEXT();
    return 0;
}

int oid_bonjour_service_name_cb(int argc, char* argv[])
{
    //MDNS_LOCK_CONTEXT();
    int active_intf = 0; // 1st intf
    int next = 1; // 1-based index
    char* name_update = argv[next];

    // retrieve current oid
    oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, active_intf, oid_service_name, NET_MDNS_SERVICE_NAME_SIZE);

    if (name_update && name_update[0] && strcmp(oid_service_name, name_update)) // non-""
    {
        // update service name
        net_mdns_set_var_service_name(active_intf, name_update);

        // retrieve latest
        oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, active_intf, oid_service_name, NET_MDNS_SERVICE_NAME_SIZE);

    }
    DBG_ERR("service '%s'\n", oid_service_name);

    //MDNS_UNLOCK_CONTEXT();
    return 0;
}

void register_cmd_plugin()
{
    error_type_t cmd_res;

    // add bonjour (airprint) cmds
    cmd_res = cmd_register_cmd(CMD_BUCKET, NULL, NULL, NULL, NULL, NULL);
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // note
    {
        DBG_ERR("%s() failed registration -- '%s'\n", __func__, CMD_BUCKET);
    }

    // bonjour service enabled
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "on",
                                   on_cmd_desc,
                                   on_cmd_usage, //usage
                                   NULL, //notes,
                                   oid_bonjour_set_on_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "on");
    }

    // bonjour service enabled
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "off",
                                   off_cmd_desc,
                                   off_cmd_usage, //usage
                                   NULL, //notes,
                                   oid_bonjour_set_off_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "off");
    }

    // bonjour service enabled
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "info",
                                   info_cmd_desc,
                                   info_cmd_usage, //usage
                                   NULL, //notes,
                                   oid_bonjour_info_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "info");
    }

    // bonjour service name
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "service",
                                   service_cmd_desc,
                                   service_cmd_usage, //usage
                                   NULL, //notes,
                                   oid_bonjour_service_name_cb );
    if (cmd_res != CMD_OK) // don't XASSERT(cmd_res==0, cmd_res); // make-a-note
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "service");
    }
}

#else
#define register_cmd_plugin() { (void)0; }
#endif // HAVE_CMD || DEBUG

void register_mdns_cmds(void)
{
    register_cmd_plugin();

return;
}

#ifdef __cplusplus
}
#endif

// eof
