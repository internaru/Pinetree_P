/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef _NET_CONF_API_H
#define _NET_CONF_API_H

#include <stdbool.h> // bool
#include "net_conf_files.h" // network conf file locations

#define DEFAULT_PLATFORM_HOSTNAME  "MarvellReference"
#define UNBOUND_PLATFORM_HOSTNAME  "(none)"
#define OS_PREFERRED_HOSTNAME_FILE "/etc/hostname"

// static-defined well-known filenames (see also net_conf_files[])
#define wifi_sta_start_sh INITD "/wifi_sta_start.sh"
#define stop_udhcpc_sh    INITD "/stop_udhcpc.sh"
#define stop_wifi_sta_sh  INITD "/stop_wifi_sta.sh"

#define uap_start_sh  INITD "/uap_start.sh"
#define uap_conf      INITD "/uaputl.conf"
#define stop_uap_sh   INITD "/stop_uap.sh"

#define mdns_start_sh INITD "/mdns_start.sh"
#define mdns_conf     INITD "/service.mdns.conf"
#define stop_mdns_sh  INITD "/stop_mdns.sh"

#define stop_udhcpd_sh INITD "/stop_udhcpd.sh"
#define udhcpd_conf   INITD "/udhcpd.conf"
#define stop_wpa_sh   INITD "/stop_wpa.sh"
#define etc_network_interfaces_conf "/etc/network/interfaces"
#define stop_wired_dhcp_sh INITD "/stop_wired_dhcp.sh"


// buffer size limits
#define SYSTEM_CMD_STRING_SIZE           256
#define MAX_CONF_FILE_SIZE              8192

// directory & filename sizes trimmed for well-known conf file locations
#define MAX_CONF_NAME_SIZE                64 /* dir + file sizes double.. */

#define MAX_CONF_FILE_BYTE_SIZE         8192 
#define MAX_FILENAME_BYTE_SIZE           256 /* 255 + 1 newline */


// indicates a change in progress -- delay/defer service restart
extern bool g_defer_config_settings_reaction;

// configuration file demographics

typedef enum file_purpose_e {
    first_conf_file  = 0,
    Interface_e      = first_conf_file,
    Bonjour_e        = 1,
    DHCP_client_e    = 2,
    SNMP_e           = 3,
    Station_e        = 4,
    uAP_e            = 5,
    uAP_udhcp_e      = 6,
    WIFI_drivers_e   = 7,
    last_conf_file   = WIFI_drivers_e
} file_purpose_t;


typedef error_type_t (*read_txt_file) (char* filename);
typedef error_type_t (*write_txt_file) (file_purpose_t config, char* content);


// set of configuration files looks like:
typedef struct net_config_file_s {
    char  dirname[MAX_CONF_NAME_SIZE]; // directory
    char basename[MAX_CONF_NAME_SIZE]; // filename 
    file_purpose_t purp;
    write_txt_file toFS;
    read_txt_file fromFS;
} net_config_file_t;

extern net_config_file_t net_conf_files[];

// create a new file (rather than update in place) -- mv after
#define CONF_FILE_INTERIM_EXT         ".seed"
#define CONF_FILE_LAST_KNOWN_GOOD_EXT ".kng"

// write_conf_file() -- update is formatted file-ready content
error_type_t write_conf_file(file_purpose_t which_config, char* update);

// return the fully qualified (NOT basename) for the given type of file
char* lookup_fs_name(file_purpose_t, char* name_buffer, int buf_size);

// return a literal string version of config file 'type'
char* get_purpose_str(int type);

// add 'conf' cmds to debug cmd interface
void net_conf_init(void);

// read an entire txt file and return an allocated buffer
// NOTE: caller frees returned buffer
char* slurp_file(char* filename);

// write buffer -> file
char* commit_file_image(char* filename, char* buffer);

// create a txt file from a list of strings
error_type_t write_sh_file(char* filename, char** sh_line);

// file inode currently defined in the fs?
bool file_exists(const char* filename); // either true or false

// append txt_records[] to given buffer
char* add_txt_records(char* given_buffer, char** txt_records);
inline char* add_newline(char* given_buffer);

// ipv4 address conversions
char* ipv4_b2str(uint32_t);   // convert b32 -> x.y.z.w
uint32_t ipv4_str2b32(char*); // convert str x.y.z.w -> 32bit
char* ipv4_b2str_inplace(uint32_t value, char* buf); // convert into buf (assumes enough space)

char* generate_uniq_ssid(char* buf); // generates 'DEFAULT_UAP_SSID [<mac suffix>]'
bool null_mac(uint8_t*, int len); // scan a given buffer[len] for non-zero content

error_type_t rebuild_etc_network_interfaces(void);
error_type_t stop_etc_network_service(char* intf_name); // NULL implies 'all'
error_type_t start_etc_network_service(char* intf_name, bool forced); // NULL implies 'all'

#endif // _NET_CONF_API_H

//eof
