/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// net_iface_config.c -- generate /etc/network/network config-file 

#ifdef __cplusplus
extern "C" {
#endif

// system includes
#include <stdio.h> 
#include <string.h>
#include <sys/types.h>

// common headers
#include "error_types.h" // OK
#include "os_system.h" // os_system()
#include "net_iface_config.h" 
#include "net_api.h" // common net api
#include "net_conf_api.h" // INITD SBIN etc_network_interfaces_conf

// Debug configuration
#define DBG_PRFX "etc.net.conf: "
#define DBG_ON
#define DBG_ERR_ON
#define DBG_VERBOSE_ON 
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

const char* etc_network_interfaces_content[] = {
    "#/*",
    "# * ============================================================================",
    "# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
    "# *",
    "# * ============================================================================",
    "# */",
    "#",
    "# " etc_network_interfaces_conf,
    "",
    "# loopback",
    "auto lo",
    "iface lo inet loopback",
    "",
    "",
    "# wired " NET_ETH_LINK_NAME,
    "auto " NET_ETH_LINK_NAME,
    "iface " NET_ETH_LINK_NAME " inet dhcp",
    "",
#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
    "",
    "# wireless " NET_UAP_LINK_NAME " micro-ap",
    "# not auto " NET_UAP_LINK_NAME,
    "iface " NET_UAP_LINK_NAME " inet manual",
    "    pre-up " WIFI_DEV_DRIVER "/install_wifi_drivers.sh",
    "    pre-up " uap_start_sh,
    "    down " stop_uap_sh,
    "",
#endif // HAVE_UAP
#ifdef HAVE_WLAN
    "",
    "# wireless " NET_MLAN_LINK_NAME " station",
    "# not auto " NET_MLAN_LINK_NAME,
    "auto " NET_MLAN_LINK_NAME,
    "iface " NET_MLAN_LINK_NAME " inet dhcp",
    "    pre-up " WIFI_DEV_DRIVER "/install_wifi_drivers.sh",
//    "    pre-up " wifi_sta_start_sh,    TEMPTEMP
    "    down " stop_wifi_sta_sh,
    "    down " stop_udhcpc_sh,
    "",
#endif // HAVE_WLAN
#endif // HAVE_WIRELESS
    "",
    "#eof " etc_network_interfaces_conf ,
    NULL
};

// create a conf file for etc/network/interfaces ifconfig start/stop daemon
error_type_t rebuild_etc_network_interfaces(void)
{
    int resp = write_sh_file((char*)etc_network_interfaces_conf, (char**)&etc_network_interfaces_content);
    if (resp != OK)
    {
        DBG_ERR("%s() '%s' err %d\n", __func__, strerror(resp), resp);
    }
    return resp;
}

// stop etc/network/interfaces service daemon
// sh 'ifdown [<intf> | -a]'
error_type_t stop_etc_network_service(char* intf_name)
{
    // current etc/network/interfaces conf file?
    if (!file_exists(etc_network_interfaces_conf))
    {
        DBG_ERR("%s() no '%s'\n", __func__, etc_network_interfaces_conf);
        error_type_t rcode = rebuild_etc_network_interfaces(); // time to rebuild
        if (rcode != OK) // can't run it if it doesn't exist ..
        {
#if 0 // experimental
            recover_inode((char*)etc_network_interfaces_conf);
#endif
            DBG_ERR("%s() no new '%s'\n", __func__, etc_network_interfaces_conf);
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
        os_system("sync"); // sync fs
    }

    // cleanup any wired dhcp process
    if (!intf_name || 
       (intf_name && intf_name[0] && !strcmp(intf_name, NET_IF_NAME_ETH))) // wired intf
    {
        char* os_req =
            { "pidfile=" VAR_RUN "/udhcpc." NET_IF_NAME_ETH ".pid; " \
              "test -f $pidfile && kill `cat $pidfile` 2>/dev/null || true" };
        os_system(os_req);
    }

    // initiate os-level service/daemon stop
    char intf_request[32] = { SBIN "/ifdown -f " };
    int len_of_cmd = strlen(intf_request);
    strncat(intf_request, (intf_name == NULL ? "-a" : intf_name), sizeof(intf_request)-len_of_cmd);
    
    DBG_VERBOSE("%s() intf_request '%s'\n", __func__, intf_request);
#ifdef DBG_VERBOSE_ON
    return os_shell(intf_request);
#else
    return os_system(intf_request); // silent
#endif
}


// start etc/network/interfaces service daemon
//    rebuild etc/network/interfaces conf file (if not present)
//    sh 'ifup [-f] [<intf> | -a]' (where -f forced)
error_type_t start_etc_network_service(char* intf_name, bool forced)
{
    // current etc/network/interfaces file?
    if (!file_exists(etc_network_interfaces_conf))
    {
        DBG_VERBOSE("%s() no '%s', rebuilding..\n", __func__, etc_network_interfaces_conf);
        error_type_t rcode = rebuild_etc_network_interfaces(); // time to rebuild
        if (rcode != OK)
        {
#if 0 // debug experimental
            recover_inode((char*)etc_network_interfaces_conf);
#endif
            DBG_ERR("%s() no new '%s' err '%s' %d\n", __func__, etc_network_interfaces_conf, strerror(thread_errno), rcode);
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }

    char intf_request[32] = { SBIN "/ifup " };
    if (forced) strcat(intf_request, " -f "); // forceful ifup
    int len_of_cmd = strlen(intf_request);
    strncat(intf_request, (intf_name == NULL ? "-a" : intf_name), sizeof(intf_request)-len_of_cmd);
    DBG_VERBOSE("%s() intf_request '%s'\n", __func__, intf_request);
#ifdef DBG_VERBOSE_ON
    return os_shell(intf_request);
#else
    return os_system(intf_request); // silent
#endif
}

// restart etc_network deamon (post-conf file update)
error_type_t propogate_etc_network_conf_change(void)
{
    error_type_t rcode = ~OK;
    rcode = stop_etc_network_service(NULL); // all interfaces
    if (rcode == OK)
    {
        posix_sleep_ms(1); // nominal settle-time -- once around
        rcode = start_etc_network_service(NULL, true); // force all interfaces
    }
    return rcode;
}

#ifdef __cplusplus
}
#endif

// eof
