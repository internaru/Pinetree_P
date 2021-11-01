/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_iface_vars.h
 * 
 * @brief User Interface API for the Network Interface layer
 *
 * This API exposes a set of variables (i.e. properties) representing a high-level view of the
 * Network Interface layer. Use this API to access network interface configuration and status
 * information for use in system User-Interface layers or other high-level system components.
 *
 * Some variable values are stored in NVRAM and persist across power cycles. The Lifetime of each
 * value is indicated in the detailed variable descriptions below. Default values are defined in the
 * Network Interface module platform configuration file.
 *
 * Unless otherwise specified, assume network byte ordering for all values exposed by this API.
 */

#ifndef NET_IFACE_VARS_H
#define NET_IFACE_VARS_H

// Current version of the Network Interface module discrete dynamic NVRAM block
// IMPORTANT: Increment NET_IFACE_VARS_VERSION whenever the net_iface_perm_vars_t structure changes.

#define NET_IFACE_VARS_VERSION 1

#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"
#include "net_iface_enum.h" // net_iface_var_id_t
#ifndef WEAK
#define WEAK __attribute__((weak))
#endif
#include "os_network.h" // #include "trsystem.h"

/**
 * @name Return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
typedef error_type_t net_iface_rcode_t;

#define NET_IFACE_OK            OK
#define NET_IFACE_FAIL          FAIL
#define NET_IFACE_BADINDEX      -11 ///< interface index out of range
#define NET_IFACE_BADPARAM      -12 ///< invalid parameter value
#define NET_IFACE_NOT_PERMITTED -13 ///< operation not permitted
#define NET_IFACE_WOULDBLOCK    1
#define NET_IFACE_INPROGRESS    2

/**
 * @name Maximum buffer sizes in bytes
 */
#define NET_IFACE_HOSTNAME_SIZE         16 ///< 15 plus NULL (Microsoft WINS maximum -- NetBIOS allows up to 16 plus a NULL).
#define NET_IFACE_DOMAINNAME_SIZE       64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char.
#define NET_IFACE_PHYS_ADDR_SIZE        6 ///< 48 bit MAC address used by Ethernet.
#define NET_IFACE_NAME_SIZE             16 ///< 15 plus terminating NULL
#define NET_IFACE_DEVICE_URL_SIZE       128 ///< 
//@}

/**
 * @brief Network interface configuration status
 *
 * Use the NET_IFACE_STATUS_X macros to access the bit fields of this uint32_t.
 *
 * This set of flags provides a high-level overview of the current network interface configuration
 * status.
 */
typedef uint32_t net_iface_status_t;

/**
 * @name Network interface configuration status flags
 *
 * Use these NET_IFACE_STATUS_X macros to access the bit fields of net_iface_status_t.
 */

/**
 * @brief System has finished booting and network is started.
 */
#define NET_IFACE_STATUS_NETWORK_STARTED                0x00000001

/**
 * @brief The physical link (e.g. Ethernet, 802.11, etc) is up.
 */
#define NET_IFACE_STATUS_LINK_UP                        0x00000002
                                                    
/**
 * @brief The IPv4 protocol has been enabled by the user.
 */
#define NET_IFACE_STATUS_IPV4_ENABLED                   0x00000100

/**
 * @brief The IPv4 protocol is in the process of being configured on the network interface.
 */
#define NET_IFACE_STATUS_IPV4_INITIALIZING              0x00000200

/**
 * @brief The IPv4 protocol is configured on the network interface and ready to send/receive data.
 */
#define NET_IFACE_STATUS_IPV4_CONFIGURED                0x00000400

/**
 * @brief A duplicate address has been detected on the network for manual, DHCP, or BOOTP config methods.
 * Does NOT apply to the auto-IP config method, where a collision simply triggers selection of a
 * new auto-IP address.
 */
#define NET_IFACE_STATUS_IPV4_DUPLICATE_ADDR            0x00001000

/**
 * @brief The manual IPv4 address configured by the user is invalid.
 */
#define NET_IFACE_STATUS_IPV4_BAD_MANUAL_CONFIG         0x00002000

/**
 * @brief Auto-configuration timed out before an IPv4 address was acquired.  This will only happen when
 * DHCP/BOOTP config methods have timed out and the auto-IP config method has not been selected by
 * the user.
 */
#define NET_IFACE_STATUS_IPV4_AUTOCONFIG_TIMEOUT        0x00004000

#ifdef HAVE_IPV6

/**
 * @brief The IPv6protocol has been enabled by the user.
 */
#define NET_IFACE_STATUS_IPV6_ENABLED                   0x00100000

/**
 * @brief The IPv6 protocol is in the process of being configured on the network interface.
 */
#define NET_IFACE_STATUS_IPV6_INITIALIZING              0x00200000

/**
 * @brief The interface is configured with a link-local IPv6 address, but global IPv6 address
 * auto-configuration has not yet completed.  IPv6 is still "configuring", waiting for additional
 * global IPv6 addresses to be acquired from routers and/or DHCPv6 servers and then complete
 * Duplicate Address Detection.
 */
#define NET_IFACE_STATUS_IPV6_LOCAL_CONFIGURED          0x00400000

/**
 * @brief The IPv6 protocol is configured on the network interface and ready to send/receive data.
 */
#define NET_IFACE_STATUS_IPV6_CONFIGURED                0x00800000

/**
 * @brief A manual global IPv6 address is either not configured or has failed Duplicate Address Detection,
 * and no other global IPv6 addresses were acquired before the IPv6 auto-configuration timeout.
 * IPv6 can only be used to communicate with devices on the local subnet via the link-local IPv6
 * address.
 */
#define NET_IFACE_STATUS_IPV6_GLOBAL_CONFIG_TIMEOUT     0x01000000

/**
 * @brief A manual global IPv6 address is either not configured or has failed Duplicate Address Detection,
 * and all auto-configured global IPv6 addresses have either expired (i.e. valid lifetime has
 * expired) or have been displaced by duplicate addresses detected on the network after IPv6
 * configuration was completed.  IPv6 can only be used to communicate with devices on the local
 * subnet via the link-local IPv6 address.
 */
#define NET_IFACE_STATUS_IPV6_GLOBAL_LOST_CONFIG        0x02000000

/**
 * @brief Either the manual IPv6 address (user configured) or the auto-configured link-local IPv6 address
 * has failed Duplicate Address Detection. Loss of the link-local IPv6 address will result in IPv6
 * being disabled on the interface.
 */
#define NET_IFACE_STATUS_IPV6_DUPLICATE_ADDR            0x10000000

/**
 * @brief The manual IPv6 address configured by the user is invalid.
 */
#define NET_IFACE_STATUS_IPV6_BAD_MANUAL_CONFIG         0x20000000

#endif // HAVE_IPV6


/**
 * @brief Host/Domain name configuration origins
 * 
 * A hostname and domain name can be assigned to the network interface.  Names can be selected
 * from any of the origins defined below.
 */
typedef enum
{
    NET_IFACE_CONFIG_ORIGIN_DEFAULT,
    NET_IFACE_CONFIG_ORIGIN_MANUAL,
    NET_IFACE_CONFIG_ORIGIN_DHCP_V4
#ifdef HAVE_IPV6
    , NET_IFACE_CONFIG_ORIGIN_DHCP_V6
#endif // HAVE_IPV6
} net_iface_config_origin_t;


/**
 * @brief IPv4 configuration options
 *
 * Use the NET_IFACE_IPV4_USE_X macros to access the bit fields of this uint32_t.
 * 
 * This set of flags identifies the methods by which the network interface will attempt to
 * configure itself for communication over IPv4, including the aquisition of an IPv4 address,
 * subnet mask, gateway, and various other configuration parameters.  Multiple methods may be
 * selected, and each method will be attempted in order or priority until configuration has
 * successfully completed.  Configuration methods in order of highest to lowest priority are:
 * manual, DHCP, BOOTP, Auto-IP.
 * 
 * If both DCHP/BOOTP and AUTOIP auto-configuration methods are selected but a DHCP/BOOTP server
 * does not respond before the configuration timeout, then the interface will be configured using
 * AUTOIP.  However, periodic attempts to contact a DHCP/BOOTP server will continue, and if a
 * server ever responds to these queries then the system will be reconfigured using information
 * from the DHCP/BOOTP server.
 */
typedef uint32_t net_iface_ipv4_config_options_t;

/**
 * @name IPv4 configuration option flags
 * 
 * Use these NET_IFACE_IPV4_USE_X macros to access the bit fields of
 * net_iface_ipv4_config_options_t.
 */

/**
 * @brief Manual configuration using information provided by the ipv4_manual_addr, pv4_manual_subnet_mask,
 * ipv4_manual_gateway, and ipv4_manual_wins_addr variables.
 */
#define NET_IFACE_IPV4_USE_MANUAL   0x00000001

/**
 * @brief Auto-configuration via DHCP
 */
#define NET_IFACE_IPV4_USE_DHCP     0x00000002

/**
 * @brief Auto-configuration via BOOTP
 */
#define NET_IFACE_IPV4_USE_BOOTP    0x00000004

/**
 * @brief Auto-configuration via RFC 3927 "Dynamic Configuration of IPv4 Link-Local Addresses" (a.k.a. Auto-IP)
 */
#define NET_IFACE_IPV4_USE_AUTOIP   0x00000008

/**
 * @brief Auto-configuration via IPCP (used with PPPoE)
 */
#define NET_IFACE_IPV4_USE_IPCP     0x00010000


/**
 * @brief IPv4 configuration method used for the most recent successful configuration of IPv4.
 * 
 * Whereas multiple configuration methods may have been attempted, only a single method will have
 * been used to configure the network interface.
 */
typedef enum {
    NET_IFACE_IPV4_CONFIGURED_MANUAL,
    NET_IFACE_IPV4_CONFIGURED_DHCP,
    NET_IFACE_IPV4_CONFIGURED_BOOTP,
    NET_IFACE_IPV4_CONFIGURED_AUTOIP,
    NET_IFACE_IPV4_CONFIGURED_IPCP
} net_iface_ipv4_config_method_t;

#ifdef HAVE_IPV6


/**
 * @brief IPv6 configuration options
 *
 * Use the NET_IFACE_IPV6_USE_X macros to access the bit fields of this uint32_t.
 *
 */
typedef uint16_t net_iface_ipv6_config_options_t;

/**
 * @name IPv6 configuration flags
 *
 * Optional methods by which the network interface will attempt to configure itself for
 * communication over IPv6.
 */

/**
 * @brief Manual configuration via the ipv6_manual_addr variable.
 */
#define NET_IFACE_IPV6_USE_MANUAL                   0x0001

/**
 * @brief Stateful auto-configuration via DHCPv6.
 */
#define NET_IFACE_IPV6_USE_DHCPV6                   0x0002

/**
 * @brief IPv6 configuration method used to acquire an IPv6 address
 * 
 * Note that an IPv6 network interface is inherently multi-homed, so the IPv6 configuration method
 * is distinct for every IPv6 address configured on the interface.
 */
typedef enum {
    NET_IFACE_IPV6_CONFIGURED_MANUAL, ///< Address was provided manually via the ipv6_manual_addr variable.
    NET_IFACE_IPV6_CONFIGURED_LINKLOCAL, ///< Address was auto-generated and is suitable only for communication on the local link. 
    NET_IFACE_IPV6_CONFIGURED_STATELESS, ///< Address was derived from a stateless address prefix provided by an IPv6 router.
    NET_IFACE_IPV6_CONFIGURED_DHCPV6 ///< Address was provided by a DHCPv6 server.
} net_iface_ipv6_config_method_t;

/**
 * @brief IPv6 address
 */
typedef struct
{
    union {
        uint8_t     addr8[16];
        uint16_t    addr16[8];
        uint32_t    addr32[4];
    } addr; ///< 128 bit representation of an IPv6 address (native byte order).
    uint8_t     prefix; ///< IPv6 address prefix length in bits.
} net_iface_ipv6_addr_t;

/**
 * @brief IPv6 address handle
 *
 * An opaque handle generated by the net_iface_get_var_ipv6_preferred_addr() and
 * net_iface_get_var_ipv6_next_addr() functions, used to reference a particular IPv6 address
 * in successive API calls.
 * 
 * The provision of this handle implies that the corresponding IPv6 address was still configured on
 * the interface at the time the handle was created.  However, if the IPv6 address is later
 * unconfigured from the interface then any handles referencing the address are considered invalid.
 * 
 * One way to determine the validity of a handle is to use it. If the handle is invalid then the
 * function will return an error code indicating so, at which point the handle should be discarded.
 */
typedef struct internal_ipv6_addr_s *net_iface_ipv6_addr_hndl_t;

/**
 * @brief IPv6 address metadata
 */
typedef struct {

    /**
     * Method whereby the IPv6 address was configured on the interface.
     */
    net_iface_ipv6_config_method_t  config_method;

    /**
     * IPv6 preferred lifetime remaining in seconds (-1 means infinite). When this lifetime expires the
     * IPv6 address will transition to the "deprecated" configuration state.
     */
    uint32_t                        preferred_lifetime_remaining;

    /**
     * IPv6 valid lifetime remaining in seconds (-1 means infinite). When this lifetime expires, the
     * IPv6 address will be unconfigured from the network interface.
     */
    uint32_t                        valid_lifetime_remaining;
} net_iface_ipv6_addr_info_t;

#endif // HAVE_IPV6

#ifdef TM_USE_PPPOE_CLIENT
#define PPPOE_USERNAME_MAX_LENGTH 256
#define PPPOE_PASSWORD_MAX_LENGTH 256
#endif // TM_USE_PPPOE_CLIENT

/** 
 *  @brief Restore one or all persistent variables to their default values.
 * 
 *  @param var_id ID of the variable to be restored to its default value, or NET_IFACE_VAR_ALL
 *      for all variables.
 *  
 *  @return OK on success or DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails.
 * 
 *  Use this function to restore one or all network interface RAM and NVRAM-based persistent
 *  variables to their default values as defined in the Network Interface module's platform
 *  configuration file.
 */
net_iface_rcode_t net_iface_var_restore_defaults(unsigned int if_index, net_iface_var_id_t var_id);


//=========================================================================================================================
/// @name Network configuration variables
//=========================================================================================================================

/**
 * @brief Get configuration status of the network interface.
 * 
 * @param[out] status Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_network_status(
    unsigned int if_index,
    net_iface_status_t *status);

/**
 * @brief Get the index of the link attached to a network interface.
 * 
 * @param[out] link_index Pointer to storage allocated by caller used to pass back a 
 *       link_index value.  Note that this is not the oem_enum index value
 *       from the OEM-specific link config file (or by the default link config file if an
 *       OEM file has not been provided).
 * 
 * @return Always returns NET_IFACE_OK.
 *  
 * This function outputs the link_index of the active link currently bound to the 
 * interface.  A link_index value of 0 means all network links are down (i.e. none are 
 * bound to the interface). 
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_active_link(
    unsigned int if_index,
    unsigned int *link_index);

/**
 * @brief Get the physical address of the device.
 * 
 * @param[out] phys_addr Pointer to storage allocated by caller used to pass back the 
 *       representative physical address for the device.
 * 
 * @param len Size in bytes of the phys_addr storage buffer.  Actual size is dependent on the link
 *       type but will be no larger than NET_IFACE_PHYS_ADDR_SIZE.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * This function is intended for use in situations where a consistent physical address is needed to
 * represent the device, regardless of how many physical links are actually configured or currently
 * active on the device.  For example, use this routine to get a unique physical address to use as
 * a GUID component for the device or to uniquely qualify a device hostname. 
 *  
 * When a device is configured for multiple simultaneous network interfaces, the user may
 * specify the name of the link to be used for this purpose in the network interface 
 * module's OEM config file.  If no name is provided then the highest-priority link 
 * attached to the first enumerated interface will be used. 
 * 
 * If the physical address of a particular link is desired, use net_iface_get_var_link_phys_addr()
 * instead.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_phys_addr(
    uint8_t *phys_addr,
    uint8_t len);

/**
 * @brief Get the physical address of a particular link.
 *  
 * @param[in] link_index Index of the link. Note that this is not the oem_enum index value
 *       from the OEM-specific link config file (or by the default link config file if an
 *       OEM file has not been provided).
 *  
 * @param[out] phys_addr Pointer to storage allocated by caller used to pass back the 
 *       physical address of the specified link.  If the requested link type has not been
 *       registered with the network interface then a zeroed address is given.
 * 
 * @param len Size in bytes of the phys_addr storage buffer.  Actual size is dependent on the link
 *       type but will be no larger than NET_IFACE_PHYS_ADDR_SIZE.
 * 
 * @return Always returns NET_IFACE_OK.
 *  
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_link_phys_addr(
    unsigned int link_index,
    uint8_t *phys_addr,
    uint8_t len);

/**
 * @brief Get the device hostname.
 * 
 * @param[out] hostname Pointer to storage allocated by caller used to pass back a null-terminated
 *       hostname string.
 * 
 * @param len Size in bytes of the hostname storage buffer.  The returned hostname will be no larger
 *       than NET_IFACE_HOSTNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.  If the buffer length is too small to hold the entire
 *       hostname string then the response will be truncated (but still null terminated).
 * 
 * Retrieves the local device hostname (i.e. not including the parent domain name) from the origin
 * specified by net_iface_get_var_hostname_origin(). When selecting a hostname origin, the origin
 * specified by net_iface_set_var_hostname_preferred_origin() is given precedence. However, if a
 * hostname is not available from that origin then the next highest priority origin with an
 * available hostname is used. Hostname origins from highest to lowest priority are: DHCPv4/v6,
 * manual, default.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len);

/**
 * @brief Get the default device hostname.
 * 
 * @param[out] hostname Pointer to storage allocated by caller used to pass back a null-terminated
 *       hostname string.
 * 
 * @param len Size in bytes of the hostname storage buffer.  The returned hostname will be no
 *       larger than NET_IFACE_HOSTNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.  If the buffer length is too small to hold the entire
 *       hostname string then the response will be truncated (but still null terminated).
 * 
 * Retrieves the default device hostname, which consists of the last six digits of the device's
 * physical address appended to a common prefix defined at compile-time.  This hostname is only
 * used by the device when net_iface_get_var_hostname_origin() responds with
 * NET_IFACE_CONFIG_ORIGIN_DEFAULT. 
 *  
 * The default device hostname is consistent across all device network interfaces.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_default_hostname(
    char *hostname,
    uint8_t len);

/**
 * @brief Get the device hostname origin.
 * 
 * @param[out] origin Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * When selecting a hostname origin, the origin specified by
 * net_iface_set_var_hostname_preferred_origin() is given precedence.  However, if a hostname is
 * not available from that origin then the next highest priority origin with an available hostname
 * is used. Hostname origins from highest to lowest priority are: DHCPv4/v6, manual, default.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_hostname_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin);

/**
 * @brief Get the device domain name.
 * 
 * @param[out] domain Pointer to storage allocated by caller used to pass back a null-terminated
 *       domain name string.
 * 
 * @param len Size in bytes of the domain storage buffer.  The returned domain name will be no
 *            larger than NET_IFACE_DOMAINNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.  If the buffer length is too small to hold the entire
 *         domain name string then the response will be truncated (but still null terminated).
 * 
 * Retrieves the device's parent domain name (i.e. not including the local device hostname) from
 * the origin specified by net_iface_get_var_domain_origin().  When selecting a domain name origin,
 * the origin specified by net_iface_set_var_domain_preferred_origin() is given precedence.
 * However, if a domain name is not available from that origin then the next highest priority
 * origin with an available domain name is used.  Domain name origins from highest to lowest
 * priority are: DHCPv4/v6, manual, default.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len);

/**
 * @brief Get the device domain name origin.
 * 
 * @param[out] origin Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * When selecting a domain name origin, the origin specified by
 * net_iface_set_var_domain_preferred_origin() is given precedence.  However, if a domain name is
 * not available from that origin then the next highest priority origin with an available domain
 * name is used. Domain name origins from highest to lowest priority are: DHCPv4/v6, manual,
 * default.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_domain_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin);

/**
 * @brief Get the preferred hostname origin.
 * 
 * @param[out] origin Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Use net_iface_get_var_hostname_origin() to retrieve the actual hostname origin.  If a hostname
 * is not available from the preferred origin, the next highest priority origin with an available
 * hostname is retrieved.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_hostname_preferred_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin);

/**
 * @brief Set the preferred hostname origin.
 * 
 * @param[in] origin Pointer to storage populated by caller with the new variable value. Upon
 *       return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Use net_iface_get_var_hostname_origin() to retrieve the actual hostname origin.  If a hostname
 * is not available from the preferred origin, the next highest priority origin with an available
 * hostname is retrieved.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_hostname_preferred_origin(
    unsigned int if_index,
    const net_iface_config_origin_t *origin);

/**
 * @brief Get the preferred domain name origin.
 * 
 * @param[out] origin Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Use net_iface_get_var_domain_origin() to retrieve the actual domain name origin.  If a domain
 * name is not available from the preferred origin, the next highest priority origin with an
 * available domain name is retrieved.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_domain_preferred_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin);

/**
 * @brief Set the preferred domain name origin.
 * 
 * @param[in] origin Pointer to storage populated by caller with the new variable value. Upon
 *       return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Use net_iface_get_var_domain_origin() to retrieve the actual domain name origin.  If a domain
 * name is not available from the preferred origin, the next highest priority origin with an
 * available domain name is retrieved.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_domain_preferred_origin(
    unsigned int if_index,
    const net_iface_config_origin_t *origin);

/**
 * @brief Get the manually configured domain name.
 * 
 * @param[out] domain Pointer to storage allocated by caller used to pass back a null-terminated
 *       domain name string.
 * 
 * @param len Size in bytes of the domain storage buffer.  The returned domain name will be no
 *       larger than NET_IFACE_DOMAINNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Retrieves the manually configured domain name.  This domain name is only used by the device when
 * net_iface_get_var_domain_origin() responds with NET_IFACE_CONFIG_ORIGIN_MANUAL.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_manual_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len);

/**
 * @brief Set the manually configured domain name.
 * 
 * @param[in] domain Pointer to a null-terminated character string. Upon return, this storage may be
 *       freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * The null terminated domain name string must conform to naming conventions defined in RFC 1034.
 * It must start with a letter, end with a letter or digit, and have as interior characters only
 * letters, digits, and hyphen.  Length must be 63 characters or less. Note that this is the
 * device's parent domain name only (i.e. not including the local device hostname).
 *
 * This domain name is only used by the device when net_iface_get_var_domain_origin() responds with
 * NET_IFACE_CONFIG_ORIGIN_MANUAL.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_manual_domain(
    unsigned int if_index,
    const char *domain);

/* URL for device-specific info. URL hostname is the device's currently configured IPv4
 * addr relative to incoming interface identified by if_index via the OID 'index' param.
 * This OID might be used when the source address of a corresponding network request was
 * IPv4.
 */
net_iface_rcode_t net_iface_get_var_device_url(unsigned int if_index, char *device_url,  uint8_t len );

#ifdef HAVE_IPV6
/* URL for device-specific info (IPv6).  URL hostname is the device's most preferred IPv6
 * address currently configured on the incoming interface identified by if_index via the
 * OID 'index' param. This OID might be used when the source address of a corresponding
 * network request was IPv6.
 */
net_iface_rcode_t net_iface_get_var_device_url_ipv6(unsigned int if_index, char *device_url,  uint8_t len );
#endif // HAVE_IPV6


/**
 * @brief Get the manually configured hostname
 * 
 * @param[out] hostname Pointer to storage allocated by caller used to pass back a null-terminated
 *       hostname string.
 * 
 * @param len Size in bytes of the hostname buffer.  The returned hostname will be no larger than
 *       NET_IFACE_HOSTNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Retrieves the manually configured hostname.  This hostname is only used by the device when
 * net_iface_get_var_hostname_origin() responds with NET_IFACE_CONFIG_ORIGIN_MANUAL.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_manual_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len);

/**
 * @brief Set the manually configured hostname.
 * 
 * @param[in] hostname Pointer to a null-terminated character string. Upon return, this storage may
 *       be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * The null terminated hostname string must conform to naming conventions defined in RFC 1034.  It
 * must start with a letter, end with a letter or digit, and have as interior characters only
 * letters, digits, and hyphen.  Length must be 16 characters or less including terminating null
 * (required so that the hostname can also be used for NetBIOS name service registration on a WINS
 * server).  Note that this is the local device hostname only (i.e. not including the parent domain
 * name).
 *
 * This hostname is only used by the device when net_iface_get_var_domain_origin() responds with
 * NET_IFACE_CONFIG_ORIGIN_MANUAL.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_manual_hostname(
    unsigned int if_index,
    const char *hostname);

//@}

//=========================================================================================================================
/// @name Interface enabled variable
//=========================================================================================================================

/**
 * @brief Get the value of the intf_enabled variable.
 * 
 * @param[out] enabled Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of "true" indicates that the interface is enabled, and the network interface will attempt to
 * configure itself to send and receive packets.  A value of "false" indicates that interface
 * is disabled, and the interface will not be configured.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_intf_enabled(
    unsigned int if_index,
    bool *enabled);

/**
 * @brief Set the value of the intf_enabled variable.
 * 
 * @param enabled The new variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Setting a value of "true" enables the network interface to communicate. If previously disabled
 * and an underlying physical link is currently up, then the interface will attempt to
 * configure itself.
 * 
 * A value of "false" disables the interface.  If previously enabled and configured for IPv4, the interface
 * will unconfigure itself (including transmitting DHCP Release packets, etc.) and the
 * interface will no longer be able to send or receive packets.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_intf_enabled(
    unsigned int if_index,
    bool enabled);

//=========================================================================================================================
/// @name IPv4 configuration variables
//=========================================================================================================================

/**
 * @brief Get the value of the ipv4_enabled variable.
 * 
 * @param[out] enabled Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of "true" indicates that IPv4 is enabled, and the network interface will attempt to
 * configure itself to send and receive packets over IPv4.  A value of "false" indicates that IPv4
 * is disabled, and the interface will not be configured for communication over IPv4.
 * 
 * This variable is not a reliable indicator of the interface's IPv4 configuration status. For
 * current IPv4 configuration status use net_iface_get_var_ipv4_configured() instead.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_enabled(
    unsigned int if_index,
    bool *enabled);

/**
 * @brief Set the value of the ipv4_enabled variable.
 * 
 * @param enabled The new variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Setting a value of "true" enables the network interface to communicate over IPv4. If previously
 * disabled and an underlying physical link is currently up, then the interface will attempt to
 * configure itself for IPv4.
 * 
 * A value of "false" disables IPv4.  If previously enabled and configured for IPv4, the interface
 * will unconfigure itself for IPv4 (including transmitting DHCP Release packets, etc.) and the
 * interface will no longer be able to send or receive packets over IPv4.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv4_enabled(
    unsigned int if_index,
    bool enabled);

/**
 * @brief Get the value of the ipv4_config_options variable.
 * 
 * @param[out] options Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_config_options(
    unsigned int if_index,
    net_iface_ipv4_config_options_t *options);

/**
 * @brief Set the value of the ipv4_config_options variable.
 * 
 * @param[in] options Pointer to storage that has been populated by caller with the new variable
 *       value. Upon return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Determines the methods by which the network interface will attempt to configure itself for
 * communication over IPv4, including the aquisition of an IPv4 address, subnet mask, gateway, and
 * various other configuration parameters.  Multiple methods may be selected, and each method will
 * be attempted in order or priority until configuration has successfully completed.
 * 
 * If the new configuration method settings differ from existing settings, a network reset will be
 * triggered to reconfigure the interface using the new settings.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv4_config_options(
    unsigned int if_index,
    const net_iface_ipv4_config_options_t *options);

/**
 * @brief Get the configuration status of IPv4.
 * 
 * @param[out] configured Pointer to storage allocated by caller used to pass back the variable
 *       value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of "true" indicates that the network interface is configured to send and receive packets
 * over IPv4.  A value of "false" indicates that the interface is not configured for IPv4.
 * 
 * This function can be used to get IPv4 configuration status following a
 * NET_IFACE_IPV4_CONFIG_CHANGE event.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_configured(
    unsigned int if_index,
    bool *configured);

/**
 * @brief Get the method used for the most recent successful configuration of IPv4.
 * 
 * @param[out] method Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * This variable is not a reliable indicator of the interface's IPv4 configuration status. For
 * current IPv4 configuration status use net_iface_get_var_ipv4_configured() instead.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_last_config_method(
    unsigned int if_index,
    net_iface_ipv4_config_method_t *method);

/**
 * @brief Get the IPv4 address of the DHCP or BOOTP server used for auto-configuration of IPv4 on the
 * network interface.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the variable
 *       value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of 0 indicates that the interface is either not currenty configured for IPv4 or was
 * configured using a method other than DHCP or BOOTP.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_boot_server(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Get the domain name dynamically configured via DHCP.
 * 
 * @param[out] domain Pointer to storage allocated by caller used to pass back a null-terminated
 *       domain name string.
 * 
 * @param len Size in bytes of the domain storage buffer.  The returned domain name will be no
 *       larger than NET_IFACE_DOMAINNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Retrieves the domain name assigned to the interface via DHCP option 15.  If the DHCP server did
 * not provide a domain name or the interface was not configured via DHCP, the function will
 * respond with an empty string.
 * 
 * This domain name is used by the device when net_iface_get_var_domain_origin() responds with
 * NET_IFACE_CONFIG_ORIGIN_DHCP_V4.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_dhcpv4_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len);

/**
 * @brief Get the hostname dynamically configured via DHCP.
 * 
 * @param[out] hostname Pointer to storage allocated by caller used to pass back a null-terminated
 *       hostname string.
 * 
 * @param len Size in bytes of the hostname storage buffer.  The returned hostname will be no
 *            larger than NET_IFACE_HOSTNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Retrieves the hostname assigned to the interface via DHCP option 12.  If the DHCP server did not
 * provide a hostname or the interface was not configured via DHCP, the function will respond with
 * an empty string.
 * 
 * This hostname is used by the device when net_iface_get_var_hostname_origin() responds with
 * NET_IFACE_CONFIG_ORIGIN_DHCP_V4.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_dhcpv4_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len);

/**
 * \brief Lookup Origin of the DNS severs:  Ie: DHCP, Manual. 
 *
 * \param[out] net_iface_config_origin_t pointer to hold the the
 *       origin the DNS server.
 *
 * \returns NET_IFACE_OK
 *
 * \The value passed back through the pointer will either be 
 *      NET_IFACE_CONFIG_ORIGIN_DHCP_V4 or
 *      NET_IFACE_CONFIG_ORIGIN_MANUAL
 *
*/
net_iface_rcode_t net_iface_get_var_dns_server_origin(
    unsigned int if_index,
    net_iface_config_origin_t * origin);

/**
 * @brief IPV4 DNS server lookup 
 *  
 * @param[in] dns_index Zero based array index to which DNS address to return 
 *  
 * @param[out] ipv4_addr pointer to hold the address of the DNS server. 
 *
 * @Passes back the ipv4_addr of server based on the index that was provided and the 
 *         current server origin
 *
 * @retval NET_IFACE_OK The operation was successful.
 * 
 * @retval NET_IFACE_BADPARAM The DNS index provided is outside the supported range 
 *
 * @The dereferenced value of the ipv4_addr is NULL if a DNS server has not been manually 
 *      configured or a preferred DNS server cannot be discovered by DHCP, depending on
 *      the origin that has been selected
 *
 */
net_iface_rcode_t net_iface_get_var_dns_server_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr);

/**
 * @brief Get a DNS server IP addresses dynamically 
 *        configured via DHCP.
 *  
 * @param[in] dns_index Zero based array index to which DNS address to return 
 *  
 * @param[out] ipv4_addr Pointer to storage allocated by caller,
 * used to pass back the DNS server IP address. 
 * 
 * @retval NET_IFACE_OK The operation was successful.
 * 
 * @retval NET_IFACE_FAIL The interface was not configured via DHCP or is
 * not currently configured. 
 *  
 * @retval NET_IFACE_BADPARAM The DNS index provided is outside the supported range 
 * 
 * Retrieves a DNS server IP address assigned to the
 * interface via DHCP.  If the DHCP server did not  provide DNS 
 * server addresses or the interface was not configured via 
 * DHCP, the function will respond with zero value address. 
 * 
 * This variable is calculated on-the-fly.  It is not
 * persistently stored in NVRAM. 
 */
net_iface_rcode_t net_iface_get_var_dhcpv4_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr);

#ifdef TM_USE_PPPOE_CLIENT
/**
 * @brief Get a DNS server IP addresses dynamically 
 *        configured via IPCP.
 *  
 * @param[in] dns_index Zero based array index to which DNS address to return 
 *  
 * @param[out] ipv4_addr Pointer to storage allocated by caller,
 * used to pass back the DNS server IP address. 
 * 
 * @retval NET_IFACE_OK The operation was successful.
 * 
 * @retval NET_IFACE_FAIL The interface was not configured via DHCP or is
 * not currently configured. 
 *  
 * @retval NET_IFACE_BADPARAM The DNS index provided is outside the supported range 
 * 
 * Retrieves a DNS server IP address assigned to the
 * interface via IPCP.  If the PPPoE server did not provide DNS
 * server addresses or the interface was not configured via 
 * IPCP, the function will respond with zero value address. 
 * 
 * This variable is calculated on-the-fly.  It is not
 * persistently stored in NVRAM. 
 */
net_iface_rcode_t net_iface_get_var_ipcp_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr);
#endif // TM_USE_PPPOE_CLIENT

/**
 * @brief Get a DNS server IP address that has been manually 
 *        configured for IPV4
 *  
 * @param[in] dns_index Zero based array index to which DNS address to return 
 *  
 * @param[out] ipv4_addr Pointer to storage allocated by caller,
 * used to pass back the DNS server IP address. 
 * 
 * @retval NET_IFACE_OK The operation was successful.
 * 
 * @retval NET_IFACE_FAIL The interface was not configured via DHCP or is
 * not currently configured. 
 *  
 * @retval NET_IFACE_BADPARAM The DNS index provided is outside the supported range 
 * 
 * Retrieves the DNS server IP address that has been manually 
 * assigned to the interface.  This variable is stored in NVRAM.
 *
*/
net_iface_rcode_t net_iface_get_var_manual_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr);

/**
 * @brief Set a manually configured DNS server IP address for 
 *        the provided index
 *
 * @param[in] dns_index Zero based array index to which DNS address to return 
 *  
 * @param[in] pointer to the ipv4_addr of the DNS server. 
 * 
 * @retval NET_IFACE_OK The operation was successful.
 * 
 * @retval NET_IFACE_FAIL Error occured
 *  
 * @retval NET_IFACE_BADPARAM The DNS index provided is outside the supported range 
 *  
 * Manually set a DNS server address at the specified index. 
 * This variable is stored in NVRAM. 
 *
*/
net_iface_rcode_t net_iface_set_var_manual_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    const uint32_t *ipv4_addr);

/**
 * @brief Get the DHCPv4 lease time remaining in seconds.
 * 
 * @param[out] lease_remaining Pointer to storage allocated by caller used to pass back the DHCPv4 lease
 *       time remaining in seconds.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A lease_remaining value of 0 indicates that the interface is either not currenty configured for
 * IPv4 or was configured by a method other than DHCP.  A lease_remaining value of -1 indicates an
 * infinite lease time.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_dhcpv4_lease_remaining(
    unsigned int if_index,
    uint32_t *lease_remaining);

/**
 * @brief Get the interface's currently configured IPv4 address.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the IPv4 address.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty configured with an IPv4
 * address.
 * 
 * This variable is not a reliable indicator of the interface's current IPv4 configuration status.
 * For current IPv4 configuration status use net_iface_get_var_ipv4_configured() instead.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Get the interface's currently configured IPv4 subnet mask.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the IPv4 subnet
 *       mask.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty configured with an IPv4
 * subnet mask.
 * 
 * This variable is not a reliable indicator of the interface's current IPv4 configuration status.
 * For current IPv4 configuration status use net_iface_get_var_ipv4_configured() instead.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_subnet_mask(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Get the interface's currently configured IPv4 gateway address.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the IPv4 gateway
 *       address.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty configured with an IPv4
 * gateway address.
 * 
 * This variable is not a reliable indicator of the interface's current IPv4 configuration status.
 * For current IPv4 configuration status use net_iface_get_var_ipv4_configured() instead.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_gateway(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Get the interface's currently configured NetBIOS Name Server (NBNS) address.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the IPv4 NBNS
 *       address.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty configured with an IPv4
 * NBNS address.
 * 
 * This variable is not a reliable indicator of the interface's current IPv4 configuration status.
 * For current IPv4 configuration status use net_iface_get_var_ipv4_configured() instead.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_wins_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Get the interface's currently assigned manual IPv4 address.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the IPv4 address.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty assigned a manual IPv4
 * address.
 * 
 * The manually assigned IPv4 address is only used to configure the IPv4 interface when
 * net_iface_get_var_ipv4_configured() responds with "true" and
 * net_iface_get_var_ipv4_last_config_method() responds with NET_IFACE_IPV4_CONFIGURED_MANUAL.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_manual_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Assign a manual IPv4 address to the interface.
 * 
 * @param[in] ipv4_addr Pointer to storage that has been populated by caller with the new variable
 *       value. Upon return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * The interface must be assigned a manual IPv4 address prior to setting the
 * NET_IFACE_IPV4_USE_MANUAL flag in the ipv4_config_options variable, or manual IPv4 configuration
 * will fail.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv4_manual_addr(
    unsigned int if_index,
    const uint32_t *ipv4_addr);

/**
 * @brief Get the interface's currently assigned manual IPv4 subnet mask.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the IPv4 subnet
 *       mask.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty assigned a manual IPv4
 * subnet mask.
 * 
 * The manually assigned IPv4 subnet mask is only used to configure the IPv4 interface when
 * net_iface_get_var_ipv4_configured() responds with "true" and
 * net_iface_get_var_ipv4_last_config_method() responds with NET_IFACE_IPV4_CONFIGURED_MANUAL.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_manual_subnet_mask(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Assign a manual IPv4 subnet mask to the interface.
 * 
 * @param[in] ipv4_addr Pointer to storage that has been populated by caller with the new variable
 *       value. Upon return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * The interface should be assigned a manual IPv4 subnet mask prior to setting the
 * NET_IFACE_IPV4_USE_MANUAL flag in the ipv4_config_options variable.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv4_manual_subnet_mask(
    unsigned int if_index,
    const uint32_t *ipv4_addr);

/**
 * @brief Get the interface's currently assigned manual IPv4 gateway address.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the IPv4 subnet
 *       mask.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty assigned a manual IPv4
 * gateway address.
 * 
 * The manually assigned IPv4 gateway address is only used to configure the IPv4 interface when
 * net_iface_get_var_ipv4_configured() responds with "true" and
 * net_iface_get_var_ipv4_last_config_method() responds with NET_IFACE_IPV4_CONFIGURED_MANUAL.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_manual_gateway(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Assign a manual IPv4 gateway address to the interface.
 * 
 * @param[in] ipv4_addr Pointer to storage that has been populated by caller with the new variable
 *       value. Upon return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * If the interface has not been assigned a manual IPv4 gateway address prior to manual
 * configuration of the interface (i.e. net_iface_get_var_ipv4_configured() responds with "true"
 * and net_iface_get_var_ipv4_last_config_method() responds with NET_IFACE_IPV4_CONFIGURED_MANUAL),
 * then any outgoing IPv4 packets IPv4 with a destination address outside the local subnet will be
 * dropped.
 * 
 * To force all IPv4 packets to be routed onto the local subnet regardless of the network address
 * of the target IPv4 address, manually assign a subnet mask that matches the interface's manually
 * assigned IPv4 address and select manual IPv4 configuration.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv4_manual_gateway(
    unsigned int if_index,
    const uint32_t *ipv4_addr);

/**
 * @brief Get the interface's currently assigned manual NetBIOS Name Server (NBNS) address.
 * 
 * @param[out] ipv4_addr Pointer to storage allocated by caller used to pass back the NBNS address.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * An ipv4_addr value of 0 indicates that the interface is not currenty assigned a manual NBNS
 * address.
 * 
 * The manually assigned NBNS address is only used when an NBNS address has not been dynamically
 * assigned via DHCP option 44.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv4_manual_wins_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr);

/**
 * @brief Assign a manual NetBIOS Name Server (NBNS) address to the interface.
 * 
 * @param[in] ipv4_addr Pointer to storage that has been populated by caller with the new variable
 *       value. Upon return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * The NetBIOS Name Server (NBNS) address is used by the net_nbns network application to register
 * the device hostname with the corresponding NetBIOS Name Server.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv4_manual_wins_addr(
    unsigned int if_index,
    const uint32_t *ipv4_addr);

//@}

#ifdef HAVE_IPV6
//=========================================================================================================================
/// @name IPv6 configuration variables
//=========================================================================================================================

/**
 * @brief Get the value of the ipv6_enabled variable.
 * 
 * @param[out] enabled Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of "true" indicates that IPv6 is enabled and the network interface will attempt to
 * configure itself to send and receive packets over IPv6.  A value of "false" indicates that IPv6
 * is disabled and the interface will not be configured for communication over IPv6.
 * 
 * This variable is not a reliable indicator of the interface's IPv6 configuration status. For
 * current IPv6 configuration status use net_iface_get_var_ipv6_configured() and
 * net_iface_get_var_ipv6_local_configured() instead.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_enabled(
    unsigned int if_index,
    bool *enabled);

/**
 * @brief Set the value of the ipv6_enabled variable.
 * 
 * @param enabled The new variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of "true" enables the network interface to communicate over IPv6.  If previously
 * disabled and an underlying physical link is currently up, then the interface will attempt to
 * configure itself for IPv6.
 * 
 * A value of "false" disables IPv6.  If previously enabled and configured for IPv6, the interface
 * will unconfigure itself for IPv6 (including transmitting DHCPv6 Release packets, etc.) and the
 * interface will no longer be able to send or receive packets over IPv6.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv6_enabled(
    unsigned int if_index,
    bool enabled);

/**
 * @brief Get the value of the ipv6_config_options variable.
 * 
 * @param[out] options Pointer to storage allocated by caller used to pass back the variable value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_config_options(
    unsigned int if_index,
    net_iface_ipv6_config_options_t *options);

/**
 * @brief Set the value of the ipv6_config_options variable.
 * 
 * @param[in] options Pointer to storage that has been populated by caller with the new variable
 *       value. Upon return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Determines the method that will be used to configure the network interface for communication
 * over IPv6, including aquisition of an IPv6 address and various other configuration parameters.
 * Multiple methods may be selected, and each method will be attempted in order of priority until
 * configuration has successfully completed.
 * 
 * If the new configuration method settings differ from existing settings, a network reset will be
 * triggered to reconfigure the interface using the new settings.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv6_config_options(
    unsigned int if_index,
    const net_iface_ipv6_config_options_t *options);

/**
 * @brief Get the link-local configuration status of IPv6.
 * 
 * @param[out] configured Pointer to storage allocated by caller used to pass back the variable
 *       value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of "true" indicates that the network interface is configured to send and receive packets
 * over IPv6 using a link-local address.  When using the link-local address, IPv6 communication is
 * limited to the local link.
 * 
 * A value of "false" indicates that the interface is not configured for link-local communication
 * over IPv6.
 * 
 * This function can be used to get link-local IPv6 configuration status following a
 * NET_IFACE_IPV6_LOCAL_CONFIG_CHANGE event.  Note that if only link-local IPv6 communication is
 * required, the NET_IFACE_IPV6_CONFIG_CHANGE event can be ignored.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_local_configured(
    unsigned int if_index,
    bool *configured);

/**
 * @brief Get the overall configuration status of IPv6.
 * 
 * @param[out] configured Pointer to storage allocated by caller used to pass back the variable
 *       value.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A value of "true" indicates that the network interface is configured to send and receive packets
 * over IPv6 using ether the link-local address or one or more global (i.e. routable) IPv6
 * addresses. When using the link-local address, IPv6 communication is limited to the local link.
 * 
 * A value of "false" indicates that the interface is not configured for communication over IPv6
 * outside the local link.
 * 
 * This function can be used to get IPv6 configuration status following a
 * NET_IFACE_IPV6_CONFIG_CHANGE event.  Note that if only link-local IPv6 communication is
 * required, the NET_IFACE_IPV6_CONFIG_CHANGE event can be ignored.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_configured(
    unsigned int if_index,
    bool *configured);

/**
 * @brief Get the domain name dynamically assigned by DHCPv6.
 * 
 * @param[out] domain Pointer to storage allocated by caller used to pass back a null-terminated
 *       domain name string.
 * 
 * @param len Size in bytes of the domain storage buffer.  The returned domain name will be no
 *       larger than NET_IFACE_DOMAINNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Retrieves the domain name assigned to the interface via DHCPv6.  If the DHCPv6 server did not
 * provide a domain name or the interface was not configured via DHCPv6, the function responds with
 * an empty string.
 * 
 * This domain name is used by the device when net_iface_get_var_domain_origin() responds with
 * NET_IFACE_CONFIG_ORIGIN_DHCP_V6.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_dhcpv6_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len);

/**
 * @brief Get the hostname dynamically assigned by DHCPv6.
 * 
 * @param[out] hostname Pointer to storage allocated by caller used to pass back a null-terminated
 *       hostname string.
 * 
 * @param len Size in bytes of the hostname storage buffer.  The returned hostname will be no
 *       larger than NET_IFACE_HOSTNAME_SIZE (including null terminator).
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Retrieves the hostname assigned to the interface via DHCPv6.  If the DHCPv6 server did not
 * provide a hostname or the interface was not configured via DHCPv6, the function responds with an
 * empty string.
 * 
 * This hostname is used by the device when net_iface_get_var_hostname_origin() responds with
 * NET_IFACE_CONFIG_ORIGIN_DHCP_V6.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_dhcpv6_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len);

/**
 * @brief Get the link-local IPv6 address currently configured on the interface.
 * 
 * @param[out] ipv6_addr Pointer to storage allocated by caller used to pass back the link-local
 *       IPv6 address.
 * 
 * @param[out] addr_hndl Pointer to storage allocated by caller used to pass back the IPv6 address
 *       handle.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * If IPv6 is disabled or not configured then this function will respond with the unspecified IPv6
 * address (i.e. a zero address, or "::").  An IPv6 unspecified address can be detected using
 * NET_IFACE_IPV6_ADDR_UNSPECIFIED()
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_link_local_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr,
    net_iface_ipv6_addr_hndl_t *addr_hndl);

/**
 * @brief Get the most preferred IPv6 address of all configured global IPv6 addresses.
 * 
 * @param[out] ipv6_addr Pointer to storage allocated by caller used to pass back the most
 *       preferred global IPv6 address.
 * 
 * @param[out] addr_hndl Pointer to storage allocated by caller used to pass back the IPv6 address
 *       handle.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * Does not include the link_local IPv6 address.  If IPv6 is disabled or no global IPv6 addresses
 * are configured then this function will respond with the unspecified address (i.e. a zero
 * address, or "::").  An IPv6 unspecified address can be detected using
 * NET_IFACE_IPV6_ADDR_UNSPECIFIED().
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_preferred_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr,
    net_iface_ipv6_addr_hndl_t *addr_hndl);

/**
 * @brief Get the next most preferred global IPv6 address relative to the referenced IPv6 address handle.
 * 
 * @param[out] ipv6_addr Pointer to storage allocated by caller used to pass back the link-local
 *       IPv6 address.
 * 
 * @param[out] addr_hndl Pointer to storage allocated by caller used to pass back the IPv6 address
 *       handle.  To facilitate iterating through the list of configured IPv6 addresses, addr_hndl
 *       may be a pointer to the same storage used for prev_addr_hndl.
 * 
 * @param[in] prev_addr_hndl Handle of a previously retrieved IPv6 address, used as a reference for
 *                       finding the next most preferred IPv6 address.
 * 
 * @retval NET_IFACE_OK The operation was successful.
 * 
 * @retval NET_IFACE_FAIL The IPv6 address handle prev_addr_hndl is invalid or has expired.  This
 *         can happen if the valid lifetime of an IPv6 address expires or the IPv6 address is
 *         otherwise unconfigured from the interface sometime after the handle was acquired.
 * 
 * Does not include the link_local IPv6 address.  If IPv6 is disabled or no global IPv6 addresses
 * are configured then this function will respond with the unspecified address (i.e. a zero
 * address, or "::").  An IPv6 unspecified address can be detected using
 * NET_IFACE_IPV6_ADDR_UNSPECIFIED()
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_next_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr,
    net_iface_ipv6_addr_hndl_t *addr_hndl,
    net_iface_ipv6_addr_hndl_t prev_addr_hndl);

/**
 * @brief Get IPv6 address metadata including the method by which the address was configured and its
 * preferred and valid lifetimes.
 * 
 * @param[out] addr_info Pointer to storage allocated by caller used to pass back the IPv6 address
 *       metadata.
 * 
 * @param[in] addr_hndl Handle of the IPv6 address for which metadata is to be retrieved.
 * 
 * @retval NET_IFACE_OK The operation was successful.
 * 
 * @retval NET_IFACE_FAIL The IPv6 address handle is invalid or has expired.  This can happen if
 *         the valid lifetime of an IPv6 address expires or the IPv6 address is otherwise
 *         unconfigured from the interface sometime after the handle was acquired.
 * 
 * This variable's value is calculated on-the-fly.  It is not persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_addr_info(
    unsigned int if_index,
    net_iface_ipv6_addr_info_t *addr_info,
    net_iface_ipv6_addr_hndl_t addr_hndl);

/**
 * @brief Get the manual IPv6 address assigned to the interface.
 * 
 * @param[out] ipv6_addr Pointer to storage allocated by caller used to pass back the IPv6 address.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * If no manual IPv6 address has been assigned to the interface then this function will respond
 * with the unspecified address (i.e. a zero address, or "::").  An IPv6 unspecified address can
 * be detected using NET_IFACE_IPV6_ADDR_UNSPECIFIED().
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_get_var_ipv6_manual_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr);

/**
 * @brief Assign a manual IPv6 address to the interface.
 * 
 * @param[in] ipv6_addr Pointer to storage that has been populated by caller with the manual IPv6
 *       address. Upon return, this storage may be freed by the caller.
 * 
 * @return Always returns NET_IFACE_OK.
 * 
 * A manually assigned IPv6 address is not configured on the interface until the manual address has
 * passed IPv6 Duplicate Address Detection and net_iface_get_var_ipv6_configured() responds with
 * "true".  IPv6 interfaces are inherently multi-homed, so the manual IPv6 address will co-exist
 * along side any other IPv6 addresses configured on the interface.
 * 
 * This variable's value is persistently stored in NVRAM.
 */
net_iface_rcode_t net_iface_set_var_ipv6_manual_addr(
    unsigned int if_index,
    const net_iface_ipv6_addr_t *ipv6_addr);

//@}

#endif // HAVE_IPV6

#ifdef TM_USE_PPPOE_CLIENT
/**
 * @brief Determine whether the PPPoE client is enabled on a 
 *        particular interface.
 * 
 * @param if_index Numeric index specifying which network 
 *                 interface the caller wants to interact with.
 *  
 * @param[out] enabled Pointer to storage allocated by caller 
 *       used to pass back whether the PPPoE client is enabled.
 * 
 * @return Returns NET_IFACE_OK on success.
 * 
 */
net_iface_rcode_t net_iface_get_var_pppoe_enabled(
    unsigned int  if_index,
    bool         *enabled);

/**
 * @brief Enable or disable the PPPoE client on an interface.
 * 
 * @param if_index Numeric index specifying which network 
 *                 interface the caller wants to interact with.
 *  
 * @param[out] enabled Boolean value specifying whether or not 
 *       to enable the PPPoE client on this interface.
 * 
 * @return Returns NET_IFACE_OK on success.
 * 
 */
net_iface_rcode_t net_iface_set_var_pppoe_enabled(
    unsigned int if_index,
    bool         enabled);


/**
 * @brief Determine the state of the PPPoE client connection. 
 * 
 * @param if_index Numeric index specifying which network 
 *                 interface the caller wants to interact with.
 *  
 * @param[out] pppoe_state Pointer to storage allocated by caller 
 *       used to pass back the pppoe client's current state.
 * 
 * @return Returns NET_IFACE_OK on success.
 *  
 *  The state provides a bitfield for PPPOE_STEP_... values that
 *  are defined in net_iface_api.h.  By checking the flags
 *  callers can determine how far they PPPoE client is getting
 *  through the configuration process.
 *  
 */
net_iface_rcode_t net_iface_get_var_pppoe_state(
    unsigned int if_index,
    uint32_t *pppoe_state);

/**
 * @brief Get the username used by the interface's PPPoE client.
 * 
 * @param if_index Numeric index specifying which network 
 *                 interface the caller wants to interact with.
 *  
 * @param[out] username Pointer to storage allocated by caller 
 *       used to pass back the PPPoE client's username.
 * 
 * @param buffer_len Size of caller's username buffer.
 *  
 * @return Returns NET_IFACE_OK on success.
 * 
 */
net_iface_rcode_t net_iface_get_var_pppoe_username(
    unsigned int  if_index,
    char         *username,
    uint16_t      buffer_len);

/**
 * @brief Set the username that this interface should use when 
 *        running the PPPoE client.
 * 
 * @param if_index Numeric index specifying which network 
 *                 interface the caller wants to interact with.
 *  
 * @param username Null terminated string containing the 
 *                 username that should be used.
 * 
 * @return Returns NET_IFACE_OK on success.
 * 
 */
net_iface_rcode_t net_iface_set_var_pppoe_username(
    unsigned int  if_index,
    char         *username);

/**
 * @brief Get the password used by the interface's PPPoE client.
 * 
 * @param if_index Numeric index specifying which network 
 *                 interface the caller wants to interact with.
 *  
 * @param[out] password Pointer to storage allocated by caller 
 *       used to pass back the PPPoE client's password.
 * 
 * @param buffer_len Size of caller's password buffer.
 *  
 * @return Returns NET_IFACE_OK on success.
 * 
 */
net_iface_rcode_t net_iface_get_var_pppoe_password(
    unsigned int  if_index,
    char         *password,
    uint16_t      buffer_len);

/**
 * @brief Set the password that this interface should use when 
 *        running the PPPoE client.
 * 
 * @param if_index Numeric index specifying which network 
 *                 interface the caller wants to interact with.
 *  
 * @param password Null terminated string containing the 
 *                 password that should be used.
 * 
 * @return Returns NET_IFACE_OK on success.
 * 
 */
net_iface_rcode_t net_iface_set_var_pppoe_password(
    unsigned int  if_index,
    char         *password);

#endif // TM_USE_PPPOE_CLIENT

typedef struct
{                                   
    net_iface_config_origin_t       hostname_origin;
    net_iface_config_origin_t       domain_name_origin;
    char                            manual_hostname[NET_IFACE_HOSTNAME_SIZE];
    char                            manual_domain_name[NET_IFACE_DOMAINNAME_SIZE];
    bool                            ipv4_enabled;
    net_iface_ipv4_config_options_t ipv4_config_options;
    net_iface_ipv4_config_method_t  ipv4_last_config_method;
    uint32_t                        ipv4_manual_addr;
    uint32_t                        ipv4_manual_subnet_mask;
    uint32_t                        ipv4_manual_gateway;
    uint32_t                        ipv4_manual_wins_addr;
    uint32_t                        ipv4_autoip_addr;
    uint32_t                        ipv4_manual_dns_preferred_addr;
    uint32_t                        ipv4_manual_dns_alternate_addr;
#ifdef HAVE_IPV6
    bool                            ipv6_enabled;
    net_iface_ipv6_config_options_t ipv6_config_options;
    net_iface_ipv6_addr_t           ipv6_manual_addr;
#endif // HAVE_IPV6
#ifdef TM_USE_PPPOE_CLIENT
    bool                            pppoe_enabled;
    char                            pppoe_username[PPPOE_USERNAME_MAX_LENGTH + 1];
    char                            pppoe_password[PPPOE_PASSWORD_MAX_LENGTH + 1];
#endif // TM_USE_PPPOE_CLIENT
    bool                            intf_enabled;
} net_iface_perm_vars_t;

#include "net_api.h" // NET_MAX_INTERFACES

/**
 * @name Maximum buffer sizes in bytes
 */
#define NET_LOCATION_SIZE           128 // per RFC 2911 printer-location def of 127 (+1 NULL terminator)

net_rcode_t net_get_var_location(char *location, uint8_t len);


// export common/network/vars for oem/network/vars
extern __attribute__((weak)) net_iface_perm_vars_t g_net_iface_perm_vars[NET_MAX_INTERFACES];

#endif // NET_IFACE_VARS_H

//eof
