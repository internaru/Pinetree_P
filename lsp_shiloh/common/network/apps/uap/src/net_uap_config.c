/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// net_uap_config.c -- generate uap's config-file settings and start/stop
//                      daemon scripts

#ifdef __cplusplus
extern "C" {
#endif

#define HAVE_OPEN_UAP 1 /* simple uap access */
#define USE_CUSTOM_TXPOWERLEVEL_TABLE 1 /* override default TxPowerLevel setting */

// system includes
#include <stdio.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> // struct stat, chmod()
#include <netinet/in.h>
#include <arpa/inet.h>

// common headers
#include "error_types.h" // OK
#include "os_system.h" // os_system()
#include "net_iface_config.h" 
#include "oid_api.h" // oid_t
#include "net_api.h" // common net api

#ifndef OK_BUSY
#define OK_BUSY EBUSY /* errno 16 */
#endif

#include "net_conf_api.h" // INITD
#include "net_uap_api.h"  // format_uap_conf()
#include "net_uap_vars.h" // wifi_intf_get_var_ssid
#include "net_intf_api.h" // get_intf_mac_addr()

// Debug configuration
#define DBG_PRFX "net.uap.conf: "
#define DBG_ON /* DBG_MSG */
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON 
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

/** 
 *  run-time on-demand
 * 
 *  This function is called anytime the script is required but unavailable
 * 
 *  rebuild the start-daemon script
 **/ 
static char* start_uap_script_open[] = {
"#!/bin/sh",
"#/*",
"# * ============================================================================",
"# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
"# *",
"# * ============================================================================",
"# */",
"#",
"# " uap_start_sh,
"",
"export PATH=$PATH:" SBIN,
"",
"intf=" NET_IF_NAME_UAP,
"",
"ifdown $intf",
"",
"# default conf files found in " WIFI_DEV_DRIVER "/config",
"cd " WIFI_DEV_DRIVER "  # make ./config file(s) available..",
"",
"./uaputl.exe sys_config " INITD "/uaputl.conf || true",
"./uaputl.exe bss_start || true",
#if USE_CUSTOM_TXPOWERLEVEL_TABLE
"",
"# setup custom pwr profile",
"./mlanconfig $intf hostcmd ./config/txpwrlimit_cfg.conf txpwrlimit_2g_cfg_set || true",
#endif // USE_CUSTOM_TXPOWERLEVEL_TABLE
"",
NULL
};

static char* start_uap_script_close[] = {
"",
"# restart the udhcpd service",
"test -d " VAR_LIB " || mkdir -p " VAR_LIB,
"test -d " VAR_LIB "/misc || mkdir -p " VAR_LIB "/misc",
"touch "   VAR_LIB "/misc/udhcpd.leases || true",
"",
SBIN "/udhcpd "  INITD   "/udhcpd.conf",
#ifdef HAVE_MDNS // initiate responder post-ipv4 association
"",
stop_mdns_sh  " || true",
mdns_start_sh " || true",
#endif // HAVE_MDNS
"",
"exit 0 # ok",
"",
"#eof " uap_start_sh,
NULL
};

// add_token_plus_newline() helper macro - add_strnl()
#define add_strnl(token) add_token_plus_newline(n, token)
#define newline '\n'

// return next writable string location (e.g, add a newline after tok)
static char* add_token_plus_newline(char* next_loc, char* tok)
{
    int chars_added = 0;
    if (tok != NULL)
    {
        int token_len = strlen(tok);
        chars_added = sprintf(next_loc, tok);
        // single byte characters assumed
        if (chars_added != token_len) // error fixup?
        {
            DBG_ERR("%s(): bad write length %d/%d?\n", __func__, chars_added, token_len);
        }
        next_loc += chars_added;
        *next_loc++ = newline;
    }
    return next_loc;
}

// add supported comma-delimited interpretes
static char* add_ipv4_ifconfig_addr_record(char* starting_loc)
{
    char* next = starting_loc;
    const char* key = SBIN "/ifconfig " NET_IF_NAME_UAP " "; // trailing-space needed
    strcat(next,key);
    int chars_added = strlen(key);
    char ip_str[MAX_IP_ADDR_STR_SIZE] = {0};
    oid_get_string(OID_WIFI_UAP_DHCP_START_ADDR, 0, ip_str, sizeof(ip_str)); // revisit index
    strcat(next, ip_str);
    chars_added += strlen(ip_str);
    next += chars_added;
    strcat(next, "\n\0"); // newline finishes ip record
    chars_added++;
    
    return starting_loc + chars_added;
}


// create a startup script for uap's service/daemon
error_type_t rebuild_uap_start_daemon_sh(void)
{
    error_type_t ret = ~OK;
    char* script_image = malloc(2048);

    if (!script_image) return FAIL;

    char* script_lines[] = { script_image, NULL }; // need end-of-image marker
    char* n = script_image;

    memset(script_image, 0, 2048); // zero-fill file image

    // initial sh contents
    int index = 0;
    char* next_line = start_uap_script_open[index];
    while (next_line)
    {
        n = add_strnl(next_line);
        index++;
        next_line = start_uap_script_open[index];
    }

    n = add_ipv4_ifconfig_addr_record(n); // "ifconfig <intf> <preferred start addr>"

    // finish sh contents
    index = 0;
    next_line = start_uap_script_close[index];
    while (next_line)
    {
        n = add_strnl(next_line);
        index++;
        next_line = start_uap_script_close[index];
    }

    // commit image -> fs
    DBG_VERBOSE("%s() script: \n%s\n", __func__, script_image);
    ret = write_sh_file((char*)uap_start_sh, script_lines);
    free(script_image);

    if (ret != OK)
    {
        DBG_ERR("%s() '%s' (err %d)\n", __func__, strerror(ret), ret);
    }

    return ret;
}

/** 
 *  run-time on-demand
 * 
 *  This function is called anytime the script is required but unavailable
 * 
 *  rebuild the stop-daemon script
 **/ 
static const char* stop_uap_script[] = {
    "#!/bin/sh",
    "#/*",
    "# * ============================================================================",
    "# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
    "# *",
    "# * ============================================================================",
    "# */",
    "#",
    "# " stop_uap_sh,
    "",
    WIFI_DEV_DRIVER "/uaputl.exe bss_stop",
    "",
    SBIN "/ifconfig " NET_IF_NAME_UAP " 0.0.0.0 # unbind ipv4",
    "",
    "pidfile=" VAR_RUN "/udhcpd.pid",
    "test -f $pidfile && kill `cat $pidfile` 2>/dev/null",
    "rm -rf $pidfile # cleanup pidfile",
    "",
    stop_udhcpd_sh,
    "exit 0 # ok",
    "#eof " stop_uap_sh,
    NULL
};

const char* stop_udhcpd_script[] = {
    "#!/bin/sh",
    "#/*",
    "# * ============================================================================",
    "# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
    "# *",
    "# * ============================================================================",
    "# */",
    "#",
    "# " stop_udhcpd_sh,
    "",
    "pid=$(ps -w |grep -v grep |grep udhcpd |awk '{print $1}')",
    "test -z \"$pid\" || (echo \"udhcpd: stop $pid\" && kill -9 $pid)",
    "",
    "exit 0 # ok",
    "",
    "#eof " stop_udhcpd_sh,
    NULL
};

// create a stop script for uap's service/daemon
error_type_t rebuild_stop_uap_daemon_sh(void)
{
    return write_sh_file((char*)stop_uap_sh, (char**)&stop_uap_script);
}

// create a stop script for udchp daemon(s)
error_type_t rebuild_stop_udhcpd_daemon_sh(void)
{
    return write_sh_file((char*)stop_udhcpd_sh, (char**)&stop_udhcpd_script);
}

error_type_t rebuild_uap_conf_files(void)
{
    char ssid[MAX_SSID_NAME_LEN];
    oid_get_string(OID_UAP_INTF_SSID, 0, ssid, MAX_SSID_NAME_LEN);

    return uap_update_conf_files(ssid);
}

// public api
error_type_t rebuild_uap_sh_files(void)
{
    error_type_t res;

    res  = rebuild_uap_start_daemon_sh();
    res |= rebuild_stop_uap_daemon_sh();
    res |= rebuild_stop_udhcpd_daemon_sh();

    res |= rebuild_uap_conf_files();

    return res;
}

#if 0 // experimental debug bad-inode file recovery
error_type_t recover_inode(char* filename)
{
    int err = unlink(filename);
    if (err)
    {
        DBG_ERR("%s(%s) unlink '%s' (err %d)\n", __func__, filename, strerror(thread_errno), thread_errno);
    }

    struct stat bad_inode_status;
    struct stat good_inode_status;

    err = stat(filename, &bad_inode_status);
    if (err)
    {
        DBG_ERR("%s(%s) stat '%s' (err %d)\n", __func__, filename, strerror(thread_errno), thread_errno);
        stat(stop_uap_sh, &good_inode_status);
    }

    return (error_type_t) err;
}
#endif

// restart bonjour deamon (post-conf file update)
error_type_t propogate_uap_conf_change(void)
{
#ifdef HAVE_NET_SERVICES
    return net_service_request(NET_SERVICE_RENEW, NET_SERVICE_UAP, NULL, NULL);
#else
    error_type_t rcode = stop_uap_service();
    if (rcode == OK)
    {
        posix_sleep_ms(1); // nominal settle-time -- once around
        rcode = start_uap_service();
    }
    return rcode;
#endif
}

// replace udhcpd configuration file (with backup)
error_type_t update_uap_udhcpd_conf(uint32_t lease_time)
{
    error_type_t rcode = ~OK;
    char* conf_file_image = (char*) malloc(MAX_CONF_FILE_SIZE);

    DBG_VERBOSE("%s() lease_time %d seconds\n", __func__, lease_time);

    if (conf_file_image != NULL)
    {
        memset(conf_file_image, 0, MAX_CONF_FILE_SIZE);
        wifi_intf_udhcp_set_var_lease_time(0, lease_time); // revisit set now?

        // generate updated file content
        format_uap_udhcpd_conf(conf_file_image, MAX_CONF_FILE_SIZE);

        // commit image -> fs
        rcode = write_conf_file(uAP_udhcp_e, conf_file_image); // udhcp_e subset of uAP_e
        free(conf_file_image);
    }
    else
    {
        DBG_ERR("%s(%d) memout, fail!\n", __func__, lease_time);
        DBG_ASSERT(conf_file_image);
        return rcode; // MEMOUT?
    }

    if (rcode != OK)
    {
        DBG_ERR("%s() '%s' (err %d)\n", __func__, strerror(rcode), rcode);
    }
    return rcode;
}

// replace configuration file (with backup)
error_type_t update_uap_conf(char* ssid)
{
    uint32_t current_lease_time;
    wifi_intf_udhcp_get_var_lease_time(0, &current_lease_time);
    DBG_VERBOSE("%s(%s) lease_time %d\n", __func__, (ssid?ssid:"<unbound ssid>"), current_lease_time);

    error_type_t rcode = ~OK;
    char* conf_file_image = (char*) malloc(MAX_CONF_FILE_SIZE);

    if (conf_file_image != NULL)
    {
        memset(conf_file_image, 0, MAX_CONF_FILE_SIZE);
        // generate updated file content
        format_uap_conf(conf_file_image, MAX_CONF_FILE_SIZE);

        // commit image -> fs
        rcode = write_conf_file(uAP_e, conf_file_image);
        free(conf_file_image);
    }
    else
    {
        DBG_ERR("%s(%s) memout, fail!\n", __func__, ssid);
        DBG_ASSERT(conf_file_image);
        return rcode; // MEMOUT?
    }

    if (rcode != OK)
    {
        DBG_ERR("%s() '%s' (err %d)\n", __func__, strerror(rcode), rcode);
    }
    return rcode;
}

// static uaputl conf file settings and capabilities
static const char* boiler_plate[] = {
    "##### " uap_conf " file template #####",
    "",
    NULL // required end-of-boilerplate
};

const char* uap_keys_part1[] = {
    "ap_config={",
    NULL // required end-of-boilerplate
};

const char* uap_keys_part2[] = {
"    BeaconPeriod=100                # Beacon period in TU",
"    Channel=6                       # Radio Channel	6",
"#   Channel=0,1                     # auto channel select (MCS mode)",
"#   Channel=6,2                     # primary channel 6, secondary channel above.",
"#   Channel=6,4                     # primary channel 6, secondary channel below",
"    ChanList=1,6,11                 # Scan channel list ",
"#   AP_MAC=00:34:22:77:46:41        # MAC address of AP",
"    Band=0                          # 0 for 2.4GHz band",
"                                    # 1 for 5GHz band   ",
"    RxAntenna=0                     # 0 for Antenna A",
"                                    # 1 for Antenna B",
"    TxAntenna=0                     # 0 for Antenna A ",
"                                    # 1 for Antenna B",
"    Rate=0x82,0x84,0x8b,0x96,0x0C,0x12,0x18,0x24,0x30,0x48,0x60,0x6c  ",
"                                    # Set of data rate that a station",
"                                    # in the BSS may use",
"                                    # (in unit of 500 kilobit/s)",
#if USE_CUSTOM_TXPOWERLEVEL_TABLE
"#    TxPowerLevel=13                # Transmit power level in dBm (unset, defaults to 127)",
#else
"    TxPowerLevel=13                 # Transmit power level in dBm",
#endif // USE_CUSTOM_TXPOWERLEVEL_TABLE
"    BroadcastSSID=1                 # Broadcast SSID feature",
"                                    # 1: Enable  0:Disable",
"    RTSThreshold=2347               # RTS threshold value",
"    FragThreshold=2346              # Fragmentation threshold value",
"    DTIMPeriod=1                    # DTIM period in beacon periods ",
"    MCBCdataRate=0                  # MCBC rate to use for packet transmission",
"                                    # 0:auto",
"                                    # >0 fix rate (in unit of 500 kilobit/s) 	 ",
"    PktFwdCtl=1                     # Packet forwarding control",
"                                    # 0: forward all packets to the host",
"                                    # 1: firmware handles intr-BSS packets",
"    StaAgeoutTimer=1800             # Inactive client station age out timer value",
"                                    # in units of 100ms",
"    PSStaAgeoutTimer=400            # Inactive client PS station age out timer value",
"                                    # in units of 100ms",
"    MaxStaNum=8                     # Max number of stations allowed to connect",
"    Retrylimit=7                    # Retry limit to use for packet transmissions",
"    AuthMode=0                      # 0:Open authentication",
"                                    # 1:shared key authentication",
#ifdef HAVE_OPEN_UAP // retain OPEN wireless network
"    Protocol=1                      # No security",
#else
"    Protocol=40                     # protocol WPA2 Mixed Mode",
#endif
"                                    # 1: No security  	2: Static WEP",
"                                    # 8: WPA	32: WPA2    40:WPA2 Mixed Mode",
NULL };

const char* uap_security_strength[] = {
"    RSNReplayProtection=0           # RSN replay protection 0: disabled, 1: enabled",
"    PairwiseUpdateTimeout=100       #Pairwise Handshake update timeout: 100ms",
"    PairwiseHandshakeRetries=3      #Pairwise Handshake retries: 3",
"    GroupwiseUpdateTimeout=100      #Groupwise Handshake update timeout: 100ms",
"    GroupwiseHandshakeRetries=3           #Groupwise Handshake retries: 3",
"",
"# **** WEP security setting******",
"#   KeyIndex=0                      # Index of WEP key to use (0 to 3)",
"#   Sample Key configurations are ",
"#       Key_0=\"55555\"",
"#       Key_1=1234567890",
"#       Key_2=\"1234567890123\"",
"#       Key_3=12345678901234567890123456",
"",
NULL };

const char* uap_cipher_settings[] = {
"# **** Pairwise Cipher setting******",
"#    Configure both of the following for Protocol=40",
#ifdef HAVE_OPEN_UAP
"#    PwkCipherWPA=1                  # Pairwise cipher type",
"                                    # 4: TKIP     8: AES CCMP",
"                                    # 12: AES CCMP + TKIP",
"#    PwkCipherWPA2=1                 # Pairwise cipher type",
"                                    # 4: TKIP     8: AES CCMP",
"                                    # 12: AES CCMP + TKIP",
#else
"    PwkCipherWPA=12                 # Pairwise cipher type",
"                                    # 4: TKIP     8: AES CCMP",
"                                    # 12: AES CCMP + TKIP",
"    PwkCipherWPA2=12                # Pairwise cipher type",
"                                    # 4: TKIP     8: AES CCMP",
"                                    # 12: AES CCMP + TKIP",
#endif
"",
"# **** Group Cipher setting******",
"",
"#    GwkCipher=4                    # group cipher type",
"                                    # 4: TKIP     8: AES CCMP",
"",
"#    PSK=\"01234567890\"              # WPA/WPA2 passphrase 	",
NULL };

// PSK override follows uap_cipher_settings ..

const char* uap_keys_part5[] = {
"     GroupRekeyTime= 86400          # Group key re-key interval, in second.",
"                                    # 0 mean never re-key",
"",
"    Enable11n=1                     # 1 to enable, 0 to disable ",
"    HTCapInfo=0x111c                # HTCapInfo ",
"                                    # Bit 15-13: Reserved set to 0",
"                                    # Bit 12: DSS/CCK mode in 40MHz enable/disable",
"                                    # Bit 11-10: Reserved set to 0",
"                                    # Bit 9-8: Reserved set to 0x01",
"                                    # Bit 7: Reserved set to 0",
"                                    # Bit 6: Short GI in 40 Mhz enable/disable",
"                                    # Bit 5: Short GI in 20 Mhz enable/disable",
"                                    # Bit 4: Green field enable/disble",
"                                    # Bit 3-2: Reserved set to 1",
"                                    # Bit 1: 20/40 Mhz enable disable.",
"                                    # Bit 0: Reserved set to 0",
"    AMPDU=0x03                      # AMPDU",
"                                    # Bit 7-5: Reserved set to 0",
"                                    # Bit 4-2: Minimum MPDU Start spacing",
"                                    #          Set to 0 for no restriction",
"                                    #          Set to 1 for 1/4 us",
"                                    #          Set to 2 for 1/2 us",
"                                    #          Set to 3 for 1 us",
"                                    #          Set to 4 for 2 us",
"                                    #          Set to 5 for 4 us",
"                                    #          Set to 6 for 8 us",
"                                    #          Set to 7 for 16 us",
"                                    # Bit 1-0: Max A-MPDU length",
"     HT_MCS_MAP=0x000000ff          # Bit 7-0: MCS_SET_0",
"                                    # Bit 15-8:MCS_SET_1",
"#    Enable2040Coex=1                #Enable 20/40 coex feature",
"",
"                                    #802.11D specific configuration",
"    11d_enable=0                    # 0-disable 1-enable",
"#   country=US                      # country information",
"",
"}",
"",
"ap_mac_filter={",
"    FilterMode=0                    # Mode of filter table",
"                                    # 0: filter table is disabled",
"                                    # 1: allow MAC address in the filter table to associate",
"                                    # 2: block MAC address in the filter table       ",
"    Count=0                         # Number of entries in filter table,up to 16",
"#Sample mac settings are ",
"#    mac_1=00:50:23:45:76:22         # mac address",
"#    mac_2=00:34:22:77:46:34         # mac address   	",
"}",
"",
"# Wmm param setting",
"Wmm_parameters={",
"    Qos_info=0x80",
"    AC_BE",
"    Aifsn=1",
"    Ecw_max=1",
"    Ecw_min=1",
"    Tx_op=1",
"    AC_BK",
"    Aifsn=2",
"    Ecw_max=2",
"    Ecw_min=2",
"    Tx_op=2",
"    AC_VI",
"    Aifsn=3",
"    Ecw_max=3",
"    Ecw_min=3",
"    Tx_op=3",
"    AC_VO",
"    Aifsn=4",
"    Ecw_max=4",
"    Ecw_min=4",
"    Tx_op=4",
"}",
"",
"# eof " uap_conf,
NULL // required end-of-strings
};

// add double-quoted string ssid record
#define double_quote 0x22
static char* add_ssid_record(char* starting_loc)
{
    const char* key="    SSID=";
    strcat(starting_loc,key);
    int chars_added = strlen(key);
    char* next = starting_loc + chars_added;
    int active_intf = 0; // revisit uap's link lookup?

    // lookup current ssid
    uint8_t ssid_u8[MAX_SSID_NAME_LEN]; char* ssid = (char*) ssid_u8; // cast
    oid_get_string(OID_UAP_INTF_SSID, active_intf, ssid, MAX_SSID_NAME_LEN);

    if (!ssid[0] || !strcmp(ssid, DEFAULT_UAP_SSID))
    {
        generate_uniq_ssid(ssid);
    }

    *next++ = double_quote;
    strcat(next, ssid);
     next += strlen(ssid);
    *next++ = double_quote;

    chars_added += strlen(ssid) + 2;
    DBG_VERBOSE("%s() ssid %s\n", __func__, ssid);

    // close ssid record
    strcat(next, "\n\0"); // newline finishes ssid record
    chars_added++;
    
    return starting_loc + chars_added;
}

// format mac addr string for "mac=..." advertisement
static char* format_mac_str(uint8_t* ma, char* buf)
{
    const char* mac_fmt_string = "mac=%02x:%02x:%02x:%02x:%02x:%02x\0";
    if (!buf || !ma) return NULL; // both buffers required

    sprintf(buf, mac_fmt_string, ma[0],ma[1],ma[2],ma[3],ma[4],ma[5]);
    return buf;
}

// format_uap_conf() generates an entire uap conf file image suitable for
// uaputl
//
// returns 'given_buffer' (filled as appropriate)
//
char* format_uap_conf(char* given_buffer, int buf_len)
{
    char*    next = given_buffer; // string buffer starts here
    char     name_buf[MAX_SSID_NAME_LEN], *ssid_name = name_buf;
    uint8_t  oid_mac[MAC_ADDR_BYTE_SIZE] = {0};
    char     uniq_devname[MAX_SSID_NAME_LEN] = {};
    char     str_mac[32], *mac_ptr = NULL;
    int      intf = 0;

    // retrieve stored ssid_name
    if (OK != oid_get_string(OID_UAP_INTF_SSID, intf, ssid_name, MAX_SSID_NAME_LEN))
    {
        ssid_name[0] = '\0'; // nullify current service name
        oid_set_string(OID_UAP_INTF_SSID, intf, DEFAULT_UAP_SSID, strlen(DEFAULT_UAP_SSID)); 
    }

    // retrieve mac addr
    memset(str_mac, 0, sizeof(str_mac));
    mac_ptr = format_mac_str(oid_mac,str_mac); // dynamic mac addr
    if (mac_ptr == NULL) *str_mac = '\0';

    // generate default service (if undefined)
    if (!ssid_name || !ssid_name[0] || !strcmp(ssid_name, DEFAULT_UAP_SSID)) // generate default ssid_name (if currently unbound)
    {
        generate_uniq_ssid(uniq_devname); // factory default + suffix
    }
    else
    {
        DBG_VERBOSE("%s() ssid_name -> '%s'\n", __func__, ssid_name);
        snprintf(uniq_devname, MAX_SSID_NAME_LEN, "%s", ssid_name);
    }

    // output conf file header/banner
    char* n = next;
    n = add_strnl("# " INITD "/uaputl.conf");
    n = add_strnl("#\n");

    // for uap conf settings..
    {
        n = add_txt_records(n, (char**)boiler_plate);
        n = add_txt_records(n, (char**)uap_keys_part1);
        n = add_ssid_record(n); // dynamic name
        n = add_txt_records(n, (char**)uap_keys_part2);
        n = add_txt_records(n, (char**)uap_security_strength);
        n = add_txt_records(n, (char**)uap_cipher_settings);
        n = add_strnl("     PSK=\"min8chars\""); // tbd: LOOKUP passkey here
        add_txt_records(n, (char**)uap_keys_part5);
    }
    DBG_VERBOSE("%s():'\n%s\n'\n", __func__, given_buffer);
    return given_buffer;
}

// create a uaputl configuration file image
char* format_uap_udhcpd_conf(char* buf, int len)
{
    char* n = buf;
    char onetime_buf[128];
    char formatted_buf[140]; // sizeof(onetime_buf) + margin
    uint32_t lease_time;
    int wifi_intf = 0; // index revisit dynamic lookup -- get_active_wifi_intf()?
    
    uint32_t v4_ip_addr;
    char start_addr[MAX_IP_ADDR_STR_SIZE];
#if 1 // use var (revisit)
    wifi_intf_udhcp_get_var_starting_addr(wifi_intf, &v4_ip_addr);
    ipv4_b2str_inplace(v4_ip_addr, start_addr); // 
#else // use oid
    oid_get_uint32(OID_WIFI_UAP_DHCP_ADDR, wifi_intf, &v4_ip_addr);
#endif
  
    // first udhcp address
    sprintf(formatted_buf, "start\t\t%s", start_addr);
    n = add_strnl(formatted_buf);

    // last udhcp address
#if 1 // use var (revisit)
    wifi_intf_udhcp_get_var_last_addr(wifi_intf, &v4_ip_addr);
    ipv4_b2str_inplace(v4_ip_addr, onetime_buf);
#else // use oid
    oid_get_string(OID_WIFI_UAP_DHCP_END_ADDR, wifi_intf, onetime_buf, MAX_IP_ADDR_STR_SIZE);
#endif
    sprintf(formatted_buf, "end\t\t%s", onetime_buf);
    n = add_strnl(formatted_buf);

    sprintf(formatted_buf, "\ninterface\t%s", NET_IF_NAME_UAP);
    n = add_strnl(formatted_buf);
	
    sprintf(formatted_buf, "\noption\tdns\t%s", start_addr);
    n = add_strnl(formatted_buf);

#if 0 // revisit dns conf file settings
    oid_get_string(0, OID_NETWORK_DNS_ADDR , onetime_buf, sizeof(onetime_buf));
    sprintf(formatted_buf, "option dns 	\t%s", onetime_buf);
    n = add_strnl(formatted_buf);
#else
    char subnet_str[16];
    char mask_str[16];
    oid_get_string(OID_WIFI_UAP_DHCP_START_ADDR, wifi_intf, subnet_str, sizeof(subnet_str));
    oid_get_string(OID_WIFI_UAP_DHCP_SUBNET_MASK, wifi_intf, mask_str, sizeof(mask_str));
    sprintf(formatted_buf, "\noption\tsubnet\t%s", mask_str); //strcpy(mask, "255.255.255.1"); // revisit factory default override
    n = add_strnl(formatted_buf);
#endif

#if 0 // revisit subnet conf file settings
    oid_get_string(0, OID_NETWORK_IPV4_DEFAULT_GATEWAY , onetime_buf, sizeof(onetime_buf));
    sprintf(formatted_buf, "option subnet 	\t%s", onetime_buf);
    n = add_strnl(formatted_buf);
#endif


    sprintf(formatted_buf, "option\trouter\t%s", start_addr);
    n = add_strnl(formatted_buf);

    n = add_strnl("option\tdomain\tlocal");
#if 1 // use var
    wifi_intf_udhcp_get_var_lease_time(wifi_intf, &lease_time);
#else // use oid
    oid_get_uint32(OID_WIFI_UAP_DHCP_LEASE_TIME, wifi_intf, &lease_time);
#endif
    sprintf(formatted_buf, "option\tlease\t%d", lease_time);
    n = add_strnl(formatted_buf);

    DBG_VERBOSE("%s() \n%s\n", __func__, buf);
    return n;
}

// rewrite uap uaputl.conf file with updated lease_time (if necessary)
error_type_t uap_udhcpd_write_conf(uint32_t req_lease_time)
{
    // range-validate requested lease time
    if (req_lease_time < 1 || req_lease_time > UAP_MAX_UDHCP_LEASE_TIME)
    {
        req_lease_time = UAP_MAX_UDHCP_LEASE_TIME;
    }

    error_type_t rcode = update_uap_udhcpd_conf(req_lease_time); // update uap's udhcpd fs conf
    return rcode;
}

// rewrite uap uaputl.conf file with 'current ssid'
error_type_t uap_write_conf(char* ssid)
{
    error_type_t rcode = ~OK;

    DBG_VERBOSE("%s(%s) ..\n", __func__, ssid);

    // update
    if (ssid)
    {
        rcode = update_uap_conf(ssid); // update uap fs conf file
        if (OK == rcode) // update uap fs
        {
            rcode = propogate_uap_conf_change(); // restart uap service/daemon
        }
    }
    return rcode;
}

// rewrite uap configuration file(s)
error_type_t uap_update_conf_files(char* ssid)
{
    error_type_t res = OK;

    int wifi_intf = 0; // revisit = get_active_wifi_intf()
    res = uap_write_conf(ssid);

    uint32_t lease_time;
    wifi_intf_udhcp_get_var_lease_time(wifi_intf, &lease_time);
    res |= uap_udhcpd_write_conf(lease_time);
    return res;
}

#ifdef __cplusplus
}
#endif

// eof
