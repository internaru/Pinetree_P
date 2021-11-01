/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  @file net_mdns_api.h
 *  
 *  @brief Multicast Domain Name System (mDNS) network application
 * 
 *  An implementation of the Apple Bonjour mDNS Responder conforming to IETF Internet-Drafts
 *  draft-cheshire-dnsext-multicastdns-11.txt "Multicast DNS" and
 *  draft-cheshire-dnsext-dns-sd-06.txt "DNS-Based Service Discovery". Additionally, the application
 *  advertises printer services conforming to Bonjour Printing Specification 1.0.2.
 * 
 */

#ifndef _NET_MDNS_API_H
#define _NET_MDNS_API_H

#include "error_types.h"
#include "oid_api.h"

#define DEFAULT_BONJOUR_SERVICE_NAME "A61x_Series"

/**
 * @name Maximum buffer sizes in bytes
 */
#define NET_MDNS_DOMAIN_NAME_SIZE   64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char
#define NET_MDNS_SERVICE_NAME_SIZE  64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char

/**
 * @name Return codes
 *
 * @note Error code values 0 through -10 are reserved in error_types.h.
 */
#define NET_MDNS_OK          OK
#define NET_MDNS_FAIL        FAIL

/**
 * @brief net_mdns_init
 *  
 *     Initiates the startup of the mdns (bonjour) module
 *
 * PREREQUISITES: None
 */
error_type_t net_mdns_init(void);

/**
 * @brief mdns_startup(interface)
 *  
 *     Retrieves the current Enabled and Service Name settings
 *   for the given interface (enum) and starts the bonjour
 *   service if an active interface is enabled.
 *
 * PREREQUISITES: Vars for mdns module need to be ready
 */
void mdns_startup(uint8_t interface);

/**
 * @brief Refresh contents of the mdns config file
 *
 *    Rewrites the mdns config file prior to restarting
 *  the bonjour service because of a settings change.
 */
error_type_t update_mdns_conf(char* requested_service);

/**
 * @brief Generates and returns the entire formatted content for 
 * mDNSResponderPosix-style configuration file.  Includes everything
 * required for SRV and TXT records;
 *
 * PREREQUISITES: None
 */
char* format_mdns_conf(char* given_buffer, int buf_len);

/**
 * @brief Update the (on-disk fs) bonjour (mDNS) configuration file.
 *
 * Operates by in a two-step process:
 *    1 - generate a new file given new content.
 *    2 - moves current conf -> conf.kng
 *    3 - moves     new conf -> conf
 */
error_type_t mdns_write_conf(char* buffer);

/**
 * @brief notify app of a change in the mdns config file
 *
 *    Typically restarts the mdns daemon because of a bonjour name change,
 *  which requires an update to the device's advertisements.
 */
error_type_t propogate_mdns_conf_change(void);

/**
 * @brief stop mdns/bonjour service
 *
 *  Stops mDNSResponderPosix service.
 */
error_type_t stop_mdns_service(void);

/**
 * @brief start mdns/bonjour service
 *
 *  Starts mDNSResponderPosix service.
 */
error_type_t start_mdns_service(void);

#define RESTART_MDNS_SERVICE_DEFAULT_DELAY 1004 /* ~1 second */

error_type_t restart_mdns_service(uint8_t intf_index, long ms_delay); // 0 ms_delay uses default

/**
 * @brief register_mdns_cmds()
 *  
 *     Register mdns cmds
 *
 * PREREQUISITES: None
 */
void register_mdns_cmds(void);

/**
 * @brief oid_register_module_mdns()
 *  
 *     Register mdns oids
 *
 * PREREQUISITES: None
 */
void oid_register_module_mdns(void);

bool mdns_enabled(void); // true or false (oid determined)
bool mdns_running(void); // true or false (os determined)

// delay start the mdns service
error_type_t activate_mdns_restart_timer(unsigned long ms_delay);

// rebuild service/daemon start/stop scripts
error_type_t rebuild_mdns_sh_files(void);

char* generate_uniq_service_name(char* buf); // glom 'DEFAULT_PLATFORM_HOSTNAME [mac]'

/* 
 * translation function callback
 * 
 * If the value is composed of a list of items, e.g. as acquired via multiple OID
 * indicies, it is the responsibility of the translation function to concatenate the items
 * together.
 * 
 * separator [in] - NULL terminated separator string used to concatenate multiple value
 * items together, or NULL if default "," should be used.
 * 
 * buf [out] - caller-provided storage to be filled with null-terminated value string (not
 * including name)
 * 
 * len [in/out] - total buf length is passed in and must be re-assigned to strlen, not
 * including NULL-terminator
 */
typedef int32_t(*mdns_translation_func_t)(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);

typedef struct mdns_txt_entry_s {
    char *name;
    char *value; // for hardcoded scalar values - must be a NULL-terminated string or NULL
    oid_t oid; // for dynamic and/or multi-values
    mdns_translation_func_t trans_func;
    char *separator; // NULL terminated separator string used to concatenate multiple value items, or NULL for default ","
    uint32_t flags;
    struct mdns_txt_entry_s *next; // for value extensions
} mdns_txt_entry_t;

#endif // _NET_MDNS_API_H

//eof
