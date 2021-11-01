/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// net_wifi_config.c -- generate wifi's config-file settings and start/stop
//                      daemon scripts

#ifdef __cplusplus
extern "C" {
#endif

// system includes
#include <stdio.h> 
#include <string.h>

// common headers
#include "error_types.h" // OK
#include "os_system.h" // os_system()
#include "net_iface_config.h" 
#include "oid_api.h" // oid_t
#include "net_api.h" // common net api
#include "net_intf_api.h" // get_intf_ipv4_addr()

#include "net_conf_api.h" // Station_e
#include "net_wifi_api.h" // format_wifi_conf()
#include "wifi_intf_api.h" // MAX_SSID_NAME_LEN

// Debug configuration
#define DBG_PRFX "net.wifi.conf: "
#define DBG_ON
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
static const char* start_wifi_sta_script[] = {
"#!/bin/sh",
"#/*",
"# * ============================================================================",
"# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
"# *",
"# * ============================================================================",
"# */",
"#",
"# " wifi_sta_start_sh,
"",
"export PATH=$PATH:" SBIN,
"",
"intf=" NET_IF_NAME_STA,
"ifdown $intf",
"delay_before_timeout=11 # currently takes ~8 seconds",
"",
"# cleanup residuals",
"rm -f " VAR_RUN "/wpa_supplicant/$intf",
"",
"modprobe cfg80211 > /dev/null #load if not built-in",
"#test $? -ne 0 && echo \"invalid wifi driver cfg80211\" && exit 1",
"",
"cd " WIFI_DEV_DRIVER,
"",
"test -f mlan.ko    || echo \"missing wifi driver mlan\"",
"test -f usb8xxx.ko || echo \"missing wifi driver usb8xxx\"",
"test ! -z \"$(lsmod | grep mlan)\" || insmod mlan.ko",
"test $? -ne 0 && echo \"invalid wifi driver mlan\" && exit 1",
"test ! -z \"$(lsmod | grep usb8xxx)\" || insmod usb8xxx.ko",
"test $? -ne 0 && echo \"invalid wifi driver usb8xxx\"",
"",
"# wifi module physically present?",
"export wifi_hw_presence=/proc/mwlan",
"test ! -d $wifi_hw_presence && echo \"missing wifi module\" && exit 1",
"",
"# already running a wpa_supplicant?",
"pid=$(ps -w | grep -v grep | grep wpa_supplicant | awk '{print $1}')",
"",
"# exit if already \"Connected\"",
"test -z \"$pid\" || exit 0",
"",
"# otherwise initiate a new one..",
USRSBIN "/wpa_supplicant -B -dd -Dwext -i$intf -c " INITD "/wpa_supplicant.conf",
"",
"# wait for wpa_supplicant to initiate .. (timeout possible)",
"",
"export supplicant_running=\"no\"",
"while (test $supplicant_running != \"yes\")",
"do",
"    let counter=$counter+1",
"        sleep 1s",
"        if (test -e $wifi_hw_presence/$intf/info)",
"        then",
"",
"            if (test $counter -gt $delay_before_timeout)",
"            then",
"                echo \"$0: timed out after $counter seconds\"",
"                exit 1",
"            else",
"                echo \"waiting $counter seconds..\"",
"            fi",
"",
"            grep \"media_state=\\\"Connected\\\"\" $wifi_hw_presence/$intf/info",
"            test $? -eq 0 && supplicant_running=yes",
"        fi",
"done",
"",
"ifup $intf -f # bind ipv4",
#ifdef HAVE_MDNS // initiate responder post-ipv4 association
"",
"# (re)start bonjour with newest $intf",
stop_mdns_sh  " || true",
mdns_start_sh " || true",
#endif // HAVE_MDNS
"",
"exit 0 # ok",
"",
"#eof " wifi_sta_start_sh,
NULL
};

/** 
 *  run-time on-demand
 * 
 *  This function is called anytime the executable wifi_sta_start_sh script is
 * required but unavailable.
 * 
 **/ 
static error_type_t rebuild_start_sta_daemon_sh(void)
{
    return write_sh_file((char*)wifi_sta_start_sh, (char**)&start_wifi_sta_script);
}

const char* stop_udhcpc_script[] = {
"#!/bin/sh",
"#/*",
"# * ============================================================================",
"# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
"# *",
"# * ============================================================================",
"# */",
"#",
"# " stop_udhcpc_sh,
"",
"# stop current " NET_IF_NAME_STA " udhcpc process",
"pidfile=" VAR_RUN "/udhcpc." NET_IF_NAME_STA ".pid",
"test -f $pidfile && kill `cat $pidfile` 2>/dev/null || true",
"",
"# stop any udhcpc-for-" NET_IF_NAME_STA " orphans",
"pid=$(ps -w|grep -v grep|grep $pidfile|awk '{print $1}')",
"test ! -z \"$pid\" && kill -9 $pid || true",
"rm -rf $pidfile",
"",
"exit 0 # ok",
"",
"#eof " stop_udhcpc_sh,
NULL
};

static const char* stop_wifi_sta_script[] = {
"#!/bin/sh",
"#/*",
"# * ============================================================================",
"# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
"# *",
"# * ============================================================================",
"# */",
"#",
"# " stop_wifi_sta_sh,
"",
stop_wpa_sh, " # stop wpa_supplicant(s)",
"",
SBIN "/ifconfig " NET_IF_NAME_STA " down # remove interface ",
"",
"exit 0",
"#eof " stop_wifi_sta_sh,
NULL
};


static const char* stop_wpa_script[] = {
"#!/bin/sh",
"#/*",
"# * ============================================================================",
"# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
"# *",
"# * ============================================================================",
"# */",
"#",
"# " stop_wpa_sh,
"",
"pid=$(ps -w|grep -v grep|grep wpa_supplicant|awk '{print $1}')",
"test -z \"$pid\" || kill -9 $pid || true",
"",
"exit 0",
"#eof " stop_wpa_sh,
NULL
};

// build executable stop_wifi_sta_sh fs-image
static error_type_t rebuild_stop_sta_daemon_sh(void)
{
    // basic stop
    error_type_t ret = write_sh_file((char*)stop_wifi_sta_sh, (char**)&stop_wifi_sta_script);

    // client dhcp cleanup
    ret |= write_sh_file((char*)stop_udhcpc_sh, (char**)&stop_udhcpc_script);

    // supplementary stop 'wpa_supplicant' also needed
    ret |= write_sh_file((char*)stop_wpa_sh, (char**)&stop_wpa_script);

    return ret;
}

// public api
error_type_t rebuild_wifi_sh_files(void)
{
    error_type_t res;
    res  = rebuild_start_sta_daemon_sh();
    res |= rebuild_stop_sta_daemon_sh();
    return res;
}

// os-level service/daemon interaction
error_type_t stop_sta_service(void)
{
    error_type_t rcode = FAIL;

    // current stop_daemon script?
    if (!file_exists(stop_wifi_sta_sh))
    {
        DBG_ERR("%s() no '%s'\n", __func__, stop_wifi_sta_sh);
        rcode = rebuild_stop_sta_daemon_sh(); // time to rebuild
        if (rcode != OK)
        {
            DBG_ASSERT(0); // fix it now if error building script
            return rcode;
        }
        os_system("sync"); // sync fs
    }

#ifdef DBG_VERBOSE_ON
    return rcode = os_shell((char*)stop_wifi_sta_sh); // noisy
#else
    return rcode = os_system((char*)stop_wifi_sta_sh);
#endif
}

error_type_t initiate_wifi_service(void)
{
    DBG_VERBOSE("%s() ..\n", __func__);
    return initiate_wifi_service_timer(101); // revisit: tune schedule interval
}

// os-level service/daemon interaction
error_type_t start_sta_service(void)
{
    error_type_t rcode = FAIL;

    // current start_daemon?
    if (!file_exists(wifi_sta_start_sh))
    {
        DBG_ERR("%s() no '%s'..\n", __func__, wifi_sta_start_sh);
        rcode = rebuild_start_sta_daemon_sh(); // time to rebuild
        if (rcode != OK)
        {
            DBG_ASSERT(0); // fix it now if error building script
            return rcode;
        }
    }

    // current config-file?
    char conf_file[MAX_FILENAME_BYTE_SIZE] = {0};
    lookup_fs_name(Station_e, conf_file, MAX_FILENAME_BYTE_SIZE);

    if (!file_exists(conf_file))
    {
        DBG_ERR("%s() no '%s'..\n", __func__, conf_file);

        char ssid[MAX_SSID_NAME_LEN] = {0};
        oid_get_string(OID_WIFI_INTF_SSID, 0, ssid, MAX_SSID_NAME_LEN);
        if (!ssid[0]) generate_uniq_ssid(ssid);

        rcode = wifi_write_conf(ssid);
        if (rcode != OK) // can't run it if it doesn't exist ..
        {
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }

    // start request if enabled
    if (wifi_sta_enabled() && !get_intf_ipv4_addr(NET_IF_NAME_STA))
    {
        // initiate os-level service/daemon
        //rcode = os_system((char*)wifi_sta_start_sh);  TEMPTEMP
    }

    return rcode;
}

// restart bonjour deamon (post-conf file update)
error_type_t propogate_wifi_conf_change(void)
{
    error_type_t rcode = FAIL;

#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_RENEW, NET_SERVICE_STA, NULL, NULL);
#else
    rcode = stop_sta_service();
    if (rcode == OK)
    {
        posix_sleep_ms(1); // token settle-time -- once around
        rcode = initiate_wifi_service();
    }
#endif
    return rcode;
}

// replace configuration file (with backup)
error_type_t update_wifi_conf(char* with_ssid)
{
    error_type_t rcode = FAIL;

    DBG_VERBOSE("%s() ssid '%s'\n", __func__, with_ssid);

    // generate updated file content
    char* conf_file_image = (char*) malloc(MAX_CONF_FILE_SIZE);
    if (conf_file_image != NULL)
    {
        memset(conf_file_image, 0, MAX_CONF_FILE_SIZE);
        format_wifi_conf(conf_file_image, MAX_CONF_FILE_SIZE);
        rcode = write_conf_file(Station_e, conf_file_image);
        free(conf_file_image);
    }

    if (rcode != OK)
    {
        DBG_ERR("%s() '%s' (err %d)\n", __func__, strerror(rcode), rcode);
    }
    return rcode;
}

// return next writable string location (e.g, add a newline after tok)
#define newline '\n'
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
// add_token_plus_newline() helper macro - add_strnl()
#define add_strnl(token) add_token_plus_newline(n, token)

// static wpa_supplicant conf file settings and capabilities
static const char* boiler_plate[] = {
    "##### wpa_supplicant configuration file template #####",
    "update_config=1",
    "ctrl_interface=" VAR_RUN "/wpa_supplicant",
    "eapol_version=1",
    "ap_scan=2",
    "fast_reauth=1",
    "",
    NULL // required end-of-boilerplate
};

const char* net_wifi_strings_open[] = {
    "network={",
    "#    key_mgmt=NONE",
    NULL // required end-of-list
};

const char* net_wifi_strings_misc[] = {
    "    proto=WPA RSN",
    "    key_mgmt=WPA-PSK",
    "    pairwise=CCMP TKIP",
    "    group=CCMP TKIP",
    NULL // required end-of-list
};

// some ap devices do not broadcast -- search for device '<ssid>' explicitly
const char* net_wifi_scan_ssid[] = {
    "    scan_ssid=1",
    NULL // required end-of-list
};

const char* net_wifi_strings_close[] = {
    "}",
    "",
    NULL // required end-of-list
};

// add double-quoted ssid
static char* add_ssid_record(char* starting_loc)
{
    char* next = starting_loc;
    const char* key = "    ssid=";
    strcat(next,key);
    int chars_added = strlen(key);
    next += chars_added;
    char ssid[MAX_SSID_NAME_LEN]={0};
    unsigned wifi_sta_intf = 0; // wifi-sta
    oid_get_string(OID_WIFI_INTF_SSID, wifi_sta_intf, ssid, MAX_SSID_NAME_LEN);
    if (!ssid[0])
    {
        sprintf(ssid, "%s", DEFAULT_PLATFORM_SSID);
        wifi_intf_set_var_ssid(wifi_sta_intf, (uint8_t*)ssid, MAX_SSID_NAME_LEN); // seed oids, cast
    }
    if (!strcmp(ssid, DEFAULT_WIFI_SSID))
    {
        generate_uniq_ssid(ssid); // sprintf(ssid, "%s", DEFAULT_PLATFORM_SSID);
    }

#define double_quote 0x22

    *next++ = (char)double_quote;
    strcat(next, ssid); 
    next += strlen(ssid);
    *next++ = (char)double_quote;
    chars_added += strlen(ssid) + 2;

    DBG_VERBOSE("%s() ssid %s\n", __func__, ssid);
    strcat(next, "\n\0"); // newline finishes ssid record
    chars_added++;
    
    return starting_loc + chars_added;
}

// add double-quoted passphrase
static char* add_passphrase_record(char* starting_loc)
{
    char* next = starting_loc;
    const char* key = "    psk=";
    strcat(next,key);
    int chars_added = strlen(key);
    next += chars_added;
    char passphrase[MAX_PASS_PHRASE];
    unsigned wifi_sta_intf = 0; // wifi-sta
    oid_get_string(OID_WIFI_INTF_WPA_PASSPHRASE, wifi_sta_intf, passphrase, MAX_PASS_PHRASE);
    if (!passphrase[0])
    {
        sprintf(passphrase, "%s", DEFAULT_WPA_PASSPHRASE);
    }

    *next++ = (char)double_quote;
    strcat(next, passphrase); 
    next += strlen(passphrase);
    *next++ = (char)double_quote;
    chars_added += strlen(passphrase) + 2;

    DBG_VERBOSE("%s() passphrase %s\n", __func__, passphrase);
    strcat(next, "\n\0"); // newline finishes ssid record
    chars_added++;
    
    return starting_loc + chars_added;
}

// formats for dynamic string replacements
// NOTE:  embedded spaces within product name MAY require literal \032
static char* format_product_str(char* product, char* buf)
{
    const char* prod_fmt_string = "product=(%s)";
    DBG_ASSERT(buf);
    DBG_ASSERT(product);
    if (!buf || !product) return NULL; // both buffers required
    sprintf(buf, prod_fmt_string, product);
    return buf;
}


// format mac addr string for "mac=..." advertisement
static char* format_mac_str(uint8_t* ma, char* buf)
{
    const char* mac_fmt_string = "mac=%02x:%02x:%02x:%02x:%02x:%02x\0";
    if (!buf || !ma) return NULL; // both buffers required

    sprintf(buf, mac_fmt_string, ma[0],ma[1],ma[2],ma[3],ma[4],ma[5]);
    return buf;
}

// format_wifi_conf() generates the entire wifi conf file image suitable for
// wpa_supplicant implementation
//
// returns 'given_buffer' (filled as appropriate)
//
char* format_wifi_conf(char* given_buffer, int buf_len)
{
    char*    next = given_buffer; // string buffer starts here
    char     name_buf[MAX_SSID_NAME_LEN], *ssid_name = name_buf;
    uint8_t  oid_mac[MAC_ADDR_BYTE_SIZE] = {0};
    char     uniq_devname[MAX_SSID_NAME_LEN] = {};
    char     str_mac[32], *mac_ptr = NULL;
    char     product_str_record[2*MAX_SSID_NAME_LEN], *product_ptr = NULL;
    int      intf = 0;

    // retrieve app's stored ssid_name
    if (OK != oid_get_string(OID_WIFI_INTF_SSID, intf, ssid_name, MAX_SSID_NAME_LEN))
    {
        ssid_name[0] = '\0'; // nullify current ssid
        oid_set_string(OID_WIFI_INTF_SSID, intf, DEFAULT_PLATFORM_SSID, strlen(DEFAULT_PLATFORM_SSID)); 
    }

    // retrieve mac addr
    memset(str_mac, 0, sizeof(str_mac));
    mac_ptr = format_mac_str(oid_mac,str_mac); // dynamic mac addr
    if (mac_ptr == NULL) *str_mac = '\0';

    // generate default service (if undefined)
    if (!ssid_name || !ssid_name[0] || !strcmp(ssid_name, DEFAULT_PLATFORM_SSID)) // generate ssid_name (if generic or unbound)
    {
        generate_uniq_ssid(ssid_name); // '<default> [<mac[3:5]>]'
    }
    else
    {
        DBG_VERBOSE("%s() ssid_name -> '%s'\n", __func__, ssid_name);
        snprintf(uniq_devname, MAX_SSID_NAME_LEN, "%s", ssid_name);
    }

    // create product record "product=(<product name>)"
    if (0)
    {
        char* product_str = DEFAULT_PLATFORM_SSID;
        product_ptr = format_product_str(product_str, product_str_record);
        if (product_ptr == NULL) *product_str_record = '\0';
    }

    // output conf file header/banner
    char* n = next;
    n = add_strnl("#/*");
    n = add_strnl("# * ============================================================================");
    n = add_strnl("# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved");
    n = add_strnl("# *");
    n = add_strnl("# * ============================================================================");
    n = add_strnl("# */");
    n = add_strnl("# " INITD "/wpa_supplicant.conf");
    n = add_strnl("#\n");

    // general TXT records possibly overriden
    n = add_txt_records(n, (char**)boiler_plate);
    n = add_txt_records(n, (char**)net_wifi_strings_open);
    n = add_ssid_record(n);
    n = add_txt_records(n, (char**)net_wifi_strings_misc);
    n = add_passphrase_record(n);
    n = add_txt_records(n, (char**)net_wifi_scan_ssid); // provoke ssid
    n = add_txt_records(n, (char**)net_wifi_strings_close);
    n = add_strnl("# eof " INITD "/wpa_supplicant.conf");

    DBG_VERBOSE("%s():'\n%s\n'\n", __func__, given_buffer);

    return given_buffer;
}

// rewrite wifi wpa_supplicant config file with 'current ssid'
error_type_t wifi_write_conf(char* with_requested_ssid)
{
    error_type_t rcode = FAIL;

    // update (if changed)
    if (with_requested_ssid)
    {
        rcode = update_wifi_conf(with_requested_ssid); // update wifi fs
        if (OK == rcode) // update wifi fs
        {
            rcode = propogate_wifi_conf_change(); // restart wifi_daemon
        }
    }
    return rcode;
}

#ifdef __cplusplus
}
#endif

// eof
