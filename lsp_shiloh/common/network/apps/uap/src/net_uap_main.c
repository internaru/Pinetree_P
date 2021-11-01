/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// net_uap_main.c -- provides uap service support via net_uap_init()

#ifdef __cplusplus
extern "C" {
#endif

// os headers
#include <stdio.h>
#include <string.h>

// common headers
#include "oid_api.h"
#include "net_api.h"
#include "net_uap_api.h"
#include "net_uap_vars.h"
#include "net_conf_api.h" // DEFAULT_UAP_SSID
#include "net_intf_api.h" // get_intf_ipv4_addr()
#include "os_system.h"

// Debug configuration
#define DBG_PRFX "net.uap: "
#define DBG_ON
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h"    // DBG_ macro family

// uap service
timer_t defer_uap_start_timer; // uap start timer
static void uap_service_cb(union sigval sig)
{
    DBG_VERBOSE("%s() timer event %d .. (now %d)\n", __func__, sig, posix_gettime_ticks());
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_RENEW, NET_SERVICE_UAP, NULL, NULL);
#else
    uap_startup(0); // 1st uap interface
#endif
}

// setup a future startup of the uap service
int initiate_uap_service_timer(long delta)
{
    return start_ms_timer(&defer_uap_start_timer, delta, uap_service_cb);
}

//   generate an updated config file and store in the filesystem
error_type_t net_uap_init_common(void)
{
    // initialize global (shadow) vars from non-volatile set
    net_uap_var_init(1); // singleton uap data set

    // activate uap
    unsigned future_time = 1103; // revisit time/delay tuning
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_START, NET_SERVICE_UAP, (void*)future_time, NULL);
#else
    initiate_uap_service_timer(future_time);
#endif

    register_uap_cmds(); // register cmds

    return OK;
}

//===========================================================================================================================
//      uAP states
//===========================================================================================================================

bool wifi_uap_enabled(void) // oid-lookup
{
    uint32_t enabled = 0;
    oid_get_uint32(OID_UAP_INTF_ENABLED, 0, &enabled); // 1st-intf index
    return (enabled == 0 ? false : true);
}

// wifi/uap hw available? (true or false)
bool wifi_uap_hw_available(void) // hw lookup
{
    // NULL returned if current link/intf address is unavailable
    return ((bool)intf_up(NET_UAP_LINK_NAME) ? true : false); // cast
}

bool wifi_uap_hw_ready(void) // hw-lookup
{
    return wifi_uap_hw_available();
}

bool wifi_uap_ready(void) // intf lookup
{
    if ((bool)get_intf_ipv4_addr(NET_UAP_LINK_NAME)) // hw addr bound?
        return true;
    return false;
}

bool wifi_uap_enabled_and_available(void) // oid- and hw-lookup
{
    return (wifi_uap_enabled() & wifi_uap_hw_ready());
}
 
// udhcpd_running() -- query os-processes for udhcp presence
bool udhcpd_running(void) // true/false running or not-running
{
    bool udhcpd_running = (bool) os_system("test -z \"$(ps | grep -v grep | grep udhcpd)\""); // cast

    // changed state?
    static bool running_last_time = false; // 1st-time init
    if (udhcpd_running & !running_last_time) DBG_VERBOSE("%s() %s\n", __func__, (udhcpd_running?"up":"DOWN"));
    running_last_time = udhcpd_running;

    return udhcpd_running;
}

// wifi_uap_running() -- query os-socket layer and services
bool wifi_uap_running(void) // true/false -- both os available and udhcpd running?
{
    static bool running_last_time = false; // 1st-time init
    bool running = false; // not-yet determined
    bool uap_up = intf_up(NET_UAP_LINK_NAME); // check intf

    if (uap_up)
    {
        running = udhcpd_running(); // service running too?

        // changed state?
        if (running & !running_last_time)
        {
            DBG_VERBOSE("%s() %s\n", __func__, (running?"up":"DOWN"));
        }
    }

    running_last_time = running;

    return running;
}

//===========================================================================================================================
//      uAP startup
//===========================================================================================================================

// uap_startup() -- if uap enabled, initiate uap service
void uap_startup(uint8_t iface_enum)
{
    bool enabled = wifi_uap_enabled(); // oid check
    
    //UAP_LOCK_CONTEXT();

    char ssid[MAX_SSID_NAME_LEN];
    oid_get_string(OID_UAP_INTF_SSID, iface_enum, ssid, MAX_SSID_NAME_LEN);
    DBG_VERBOSE("%s('%s') %s\n", __func__, ssid, (enabled?"enabled":"disabled"));

    if (enabled) {
#ifdef HAVE_NET_SERVICES
        int reply = OK;
        net_service_request(NET_SERVICE_START, NET_SERVICE_UAP, (void*)ssid, &reply);
        if (reply != OK)
        {
            DBG_ERR("%s('%s') err %d\n", __func__, ssid, reply);
        }
#else
        start_uap_service();
#endif
    }

    //UAP_UNLOCK_CONTEXT();

    return;
}

// stop uap service daemon
//    rebuild stop shell (if not present)
//    rebuild dhcp stop sh file (if not present)
//    sh stop_uap_sh
error_type_t stop_uap_service(void)
{
    error_type_t rcode = OK;

    // current uap stop_daemon?
    if (!file_exists(stop_uap_sh))
    {
        DBG_ERR("%s() no '%s'\n", __func__, stop_uap_sh);
        rcode = rebuild_stop_uap_daemon_sh(); // time to rebuild
        if (rcode != OK) // can't run it if it doesn't exist ..
        {
#if 0 // experimental
            recover_inode((char*)stop_uap_sh);
#endif
            DBG_ERR("%s() no new '%s'\n", __func__, stop_uap_sh);
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }

    // current udhcpd stop_daemon?
    if (!file_exists(stop_udhcpd_sh))
    {
        DBG_ERR("%s() no '%s'\n", __func__, stop_udhcpd_sh);
        rcode = rebuild_stop_udhcpd_daemon_sh(); // time to rebuild
        if (rcode != OK) // can't run it if it doesn't exist ..
        {
#if 0 // experimental
            recover_inode((char*)stop_udhcpd_sh);
#endif
            DBG_ERR("%s() no new '%s'\n", __func__, stop_udhcpd_sh);
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }

    // initiate os-level service/daemon stop
    return os_system( SBIN "/ifdown " NET_UAP_LINK_NAME);
}

// start uap service daemon
//    rebuild uap startup shell (if not present)
//    rebuild udhcp conf file (if not present)
//    rebuild uap conf file (if not present)
//    sh uap_start_sh
error_type_t start_uap_service(void)
{
    error_type_t rcode = OK;
    int wifi_uap_intf = 0; // revisit dynamic lookup?
#if 0
    // current start_daemon?
    if (!file_exists(uap_start_sh))
    {
        DBG_VERBOSE("%s() no '%s', rebuilding..\n", __func__, uap_start_sh);
        rcode = rebuild_uap_start_daemon_sh(); // time to rebuild
        if (rcode != OK)
        {
#if 0 // debug experimental
            recover_inode((char*)uap_start_sh);
#endif
            DBG_ERR("%s() no new '%s' err '%s' %d\n", __func__, uap_start_sh, strerror(thread_errno), rcode);
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }
#endif
    // generate conf file(s) if necessary..
    char conf_file[MAX_FILENAME_BYTE_SIZE];
#if 0
    lookup_fs_name(uAP_udhcp_e, conf_file, MAX_FILENAME_BYTE_SIZE);
    if (!file_exists(conf_file))
    {
        DBG_ERR("%s() no '%s', rebuilding..\n", __func__, conf_file);

        // retrieve current lease time
        uint32_t lease_time;
        oid_get_uint32(OID_WIFI_UAP_DHCP_LEASE_TIME, wifi_uap_intf, &lease_time);
        if (lease_time <= 0 || lease_time > UAP_MAX_UDHCP_LEASE_TIME)
            lease_time = UAP_MAX_UDHCP_LEASE_TIME;
        rcode = uap_udhcpd_write_conf(lease_time);
        if (rcode != OK)
        {
            DBG_ERR("%s() '%s' rebuild err %d..\n", __func__, "udhcpd.conf", rcode);
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }
#endif
    // verify uap's conf file exists
    lookup_fs_name(uAP_e, conf_file, MAX_FILENAME_BYTE_SIZE);
    if (!file_exists(conf_file))
    {
        DBG_ERR("%s() no '%s', rebuilding..\n", __func__, conf_file);

        // retrieve current ssid
        char ssid[MAX_SSID_NAME_LEN] = { 0 };
        oid_get_string(OID_UAP_INTF_SSID, wifi_uap_intf, ssid, MAX_SSID_NAME_LEN);
        if (!ssid[0] || !strcmp(ssid, DEFAULT_UAP_SSID)) // uninitialized or generic?
        {
            generate_uniq_ssid(ssid); // factory default + mac suffix
        }
        rcode = uap_write_conf(ssid);

        if (rcode != OK) // can't run it if it doesn't exist ..
        {
            DBG_ERR("%s() rebuilt '%s' err %d..\n", __func__, "uaputl.conf", rcode);
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }
#if 0
    // start request if enabled
    if (wifi_uap_enabled() && !get_intf_ipv4_addr(NET_IF_NAME_UAP))
    {
        // initiate os-level service/daemon
        rcode |= os_system( SBIN "/ifup " NET_UAP_LINK_NAME );
    }
#endif
    return rcode;
}

#ifdef __cplusplus
}
#endif

// eof
