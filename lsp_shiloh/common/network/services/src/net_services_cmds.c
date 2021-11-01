/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
//  Provide a cmd interface to 'net services' interactions

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
#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
#include "net_uap_vars.h"
#endif
#ifdef HAVE_WLAN
#include "net_wifi_api.h"
#endif
#endif // HAVE_WIRELESS
#include "os_system.h" // os_shell()

#define CMD_BUCKET "net" /* logger_common.c network #12 */

#include "error_types.h"
#define DBG_PRFX "service.cmd: "
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
#define DBG_ERR_ON
#include "net_debug.h" // DBG_ family defaults

// local debug helpers
char* str_service(int service)
{
#define service_req(state) case state: return #state
    switch (service)
    {
        case NET_SERVICE_MDNS:  return "mdns"; // service_req(NET_SERVICE_MDNS);
        case NET_SERVICE_SNMP:  return "snmp"; // service_req(NET_SERVICE_SNMP);
        case NET_SERVICE_STA:   return "mlan"; // service_req(NET_SERVICE_STA);
        case NET_SERVICE_DHCP:  return "dhcp"; // service_req(NET_SERVICE_DHCP);
        case NET_SERVICE_UAP:   return "uap";  // service_req(NET_SERVICE_UAP);
        case NET_SERVICE_UDHCP: return "udhcp";// service_req(NET_SERVICE_UDHCP);
        case NET_SERVICE_IFCONFIG: return "if";// service_req(NET_SERVICES_ALL);
        case NET_SERVICES_ALL:  return "all";  // service_req(NET_SERVICES_ALL);
      default: break;
    }
return "<unknown service>";
}

char* str_request(int req)
{
#define msg_req(state) case state: return #state
    switch (req)
    {
        case NET_SERVICE_CONFIG: return "CONFIG"; // msg_req(NET_SERVICE_CONFIG);
        case NET_SERVICE_STATUS: return "STATUS"; // msg_req(NET_SERVICE_STATUS);
        case NET_SERVICE_START:  return "START";  // msg_req(NET_SERVICE_START);
        case NET_SERVICE_STOP:   return "STOP";   // msg_req(NET_SERVICE_STOP);
        case NET_SERVICE_RENEW:  return "RENEW";  // msg_req(NET_SERVICE_RENEW);
      default: break;
    }
return "<unknown request>";
}

bool snmp_running(void) // true/false running or not-running -- os-determined
{
#if defined HAVE_SNMP || defined HAVE_SNMP_SUBAGENT
    // query os process-list for a running snmp service
    static bool running_last_time = false; // 1-time init
    bool service_running = (bool) os_system("test -z \"$(ps | grep -v grep | grep snmpd)\"");
    bool running = service_running; // by definition: service started?

    // changed state?
    if (running & !running_last_time)
    {
        DBG_VERBOSE("%s() %s\n", __func__, (running?"up":"DOWN"));
    }
    running_last_time = running;
    return running;
#else
    return false;
#endif
}

#if 1 // include for both release and debug //#if defined DEBUG || defined HAVE_CMD


static const char* services_cmd_usage = "";
static const char* services_cmd_desc  = "display current services";

__attribute__ ((aligned (4))) ns_packet_req_t g_log_req[NUM_TRACKED_REQUESTS+1];
//               [mdns, uap, etc..][config,start,stop,renew]
long req_counters[NET_SERVICES_ALL][NUM_SERVICE_ACTIONS_ALL];

// accumulate 'net service' requests in a circular log
bool track_request(int action, int service_client)
{
    // sanity check array bounds
    if ((action < NUM_SERVICE_ACTIONS_ALL) &&
        (service_client <= NET_SERVICES_ALL))
    {
        req_counters[service_client][action] += 1;
        return true;
    }

    DBG_ERR("%s(%d,%d) invalid!?\n", __func__, action, service_client);
    // account errors?
    return false;
}

// report current service state(s)
void show_net_services_activity(void)
{
    extern timer_t work_packet_ttl_timer; // revisit extern
    long ms_window = timer_active(&work_packet_ttl_timer);
    if (ms_window > 0)
    {
        DBG_VERBOSE("%s() busy (%d ms remaining)\n", __func__, ms_window);
    }
    else
    {
        DBG_VERBOSE("%s() idle\n", __func__);
    }

#if defined HAVE_SNMP | defined HAVE_SNMP_SUBAGENT
    dbg_printf("  snmp service %srunning\n", (snmp_running()?"":"NOT "));
#endif

#ifdef HAVE_MDNS
    char mdns_service[NET_MDNS_SERVICE_NAME_SIZE];
    oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, 0, mdns_service, sizeof(mdns_service));
    if (!mdns_service[0]) strcpy(mdns_service, "<undefined service name?>");
    if (!strcmp(mdns_service, DEFAULT_PLATFORM_HOSTNAME))
    {
        generate_uniq_service_name(mdns_service);
    }
    dbg_printf("  mdns service '%s' %s %srunning\n", mdns_service, (mdns_enabled()?"enabled":"disabled"), (mdns_running()?"":"NOT "));
#endif
}

void show_service_requests(void)
{
    int first_index = (g_all_workq_req_counter % NUM_TRACKED_REQUESTS) + 1;
    int next = g_all_workq_req_counter < NUM_TRACKED_REQUESTS ? 0 : first_index;
    int i = 0;

    DBG_VERBOSE("\n%s() %d/%d overall (%d/%d backend)\n", __func__,
        (g_net_service_all_req_counter-g_net_service_folded_req_counter), g_net_service_all_req_counter,
        (g_all_workq_req_counter-g_folded_workq_req_counter), g_all_workq_req_counter);

    // currently running a task?
    extern int task_running_ttl; // revisit extern cast
    if (task_running_ttl)
    {
        ns_packet_req_t* logd = &g_log_req[g_current_req];
        if (logd->req.request)
        {
            DBG_VERBOSE(" %2d: %-10s %8s x%x x%x t(%d,%d)\n", i,
                str_request(logd->req.request), str_service(logd->req.service),
                logd->req.udata, logd->req.reply,
                logd->start_time, logd->finish_time); 
        }
    }

    // dump history
    while (i++ < NUM_TRACKED_REQUESTS)
    {
        ns_packet_req_t* logd = &g_log_req[next];
        if (logd->req.request)
        {
            dbg_printf(" %2d: %8s %5s x%x x%x t(%d,%d) \tdt %d\n", i,
                str_request(logd->req.request), str_service(logd->req.service),
                logd->req.udata, logd->req.reply,
                logd->start_time, logd->finish_time,
                logd->finish_time - logd->start_time); 
        }
        next = (next < NUM_TRACKED_REQUESTS ? next+1 : 0 /*beginning*/); // advance circular log ptr
    }
    dbg_printf("\n");
    
#ifdef DBG_VERBOSE_ON
    // show metrics by client (subset -- relies on enum order [ see net_service.h ])
    int service = NET_SERVICE_MDNS;
    while (service <= NET_SERVICE_IFCONFIG)
    {
        long starts  = req_counters[service][NET_SERVICE_START]; UNUSED_VAR(starts);
        long stops   = req_counters[service][NET_SERVICE_STOP]; UNUSED_VAR(stops);
        long configs = req_counters[service][NET_SERVICE_CONFIG]; UNUSED_VAR(configs);
        long renews  = req_counters[service][NET_SERVICE_RENEW]; UNUSED_VAR(renews);
        DBG_VERBOSE("%5s config %3d start %3d stop %3d renews %3d\n",
            str_service(service), 
                configs, starts, stops, renews);
        service++;
    }
#endif // DBG_VERBOSE_ON

    show_net_services_activity();
}


int cmd_net_services_cb(int argc, char* argv[])
{
    bool running = false;
    dbg_printf("snmp %s\n", (snmp_running()?"Running":"NOT"));

#ifdef HAVE_MDNS
    char service_name[NET_MDNS_SERVICE_NAME_SIZE];
    running = mdns_running(); // ask once
    oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, 0, service_name, sizeof(service_name));
    if (service_name[0] && !strcmp(service_name,DEFAULT_PLATFORM_HOSTNAME))
        generate_uniq_service_name(service_name);
    dbg_printf("mdns %s  %s\n", (running?"Running":"NOT"), (running?service_name:""));
#endif

#ifdef HAVE_WIRELESS
    char ssid_buf[MAX_SSID_NAME_LEN];
#ifdef HAVE_WLAN
    running = wifi_sta_running(); // ask once
    oid_get_string(OID_WIFI_INTF_SSID, 0, ssid_buf, MAX_SSID_NAME_LEN);
    if (ssid_buf[0] && !strcmp(ssid_buf, DEFAULT_WIFI_SSID))
        generate_uniq_ssid(ssid_buf); // provide a uniq ssid
    dbg_printf("wifi/sta %srunning %s\n", (running?"":"NOT "), (running?ssid_buf:""));
#endif
#ifdef HAVE_UAP
    running = wifi_uap_running(); // ask once
    oid_get_string(OID_UAP_INTF_SSID, 0, ssid_buf, MAX_SSID_NAME_LEN);
    if (ssid_buf[0] && !strcmp(ssid_buf, DEFAULT_UAP_SSID))
        generate_uniq_ssid(ssid_buf); // provide a uniq ssid
    dbg_printf("wifi/uap %srunning %s\n", (running?"":"NOT "), (running?ssid_buf:""));
#endif
#endif //HAVE_WIRELESS

    show_service_requests();

    return 0;
}

static void register_cmd_plugin()
{
    error_type_t cmd_res;

    // add net cmds
    cmd_res = cmd_register_cmd(CMD_BUCKET, NULL, NULL, NULL, NULL, NULL);
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s'\n", __func__, CMD_BUCKET);
    }

    // net services
    cmd_res = cmd_register_subcmd( CMD_BUCKET,
                                  "services",
                                   services_cmd_desc,
                                   services_cmd_usage, //usage
                                   NULL, //notes,
                                   cmd_net_services_cb );
    if (cmd_res != CMD_OK)
    {
        DBG_ERR("%s() failed registration -- '%s %s'\n", __func__, CMD_BUCKET, "services");
    }
}

#else
#define register_cmd_plugin() { (void)0; }
#endif // HAVE_CMD || DEBUG

void register_net_services_cmds(void)
{
    register_cmd_plugin();

return;
}

#ifdef __cplusplus
}
#endif

// eof
