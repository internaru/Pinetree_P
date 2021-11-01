/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_iface_config_dflt.c
  * 
  *  This file contains the net_iface module OEM configuration defaults.
  * 
  *  These common default values are used in the absence of any OEM-specific configuration
  *  under the "oem" directory.
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "lassert.h"
#include "logger.h"
#include "net_api.h"
#include "net_logger.h"
#include "net_iface_vars.h"
#include "net_iface_config.h"

#ifdef HAVE_UAP
#ifdef __linux__
#include "net_uap_vars.h"
#else
#include "dhcpd_vars.h"
#endif
#endif // HAVE_UAP


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#ifdef __linux__
#define DBG_PRFX "net.if.config: "
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
#include "net_debug.h"

#else

#define DBG_PRFX "NET: "
#define LOGGER_MODULE_MASK (DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_IFACE) 

#define DBG_CRIT(...) DBG_PRINTF(LOG_CRIT, DBG_PRFX __VA_ARGS__)
#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#ifdef HAVE_CMD
#define DBG_CMD(...) cmd_printf( __VA_ARGS__ )
#endif
#endif // ! __linux__


//=========================================================================================================================
// Network interface instance config
//=========================================================================================================================

/*
 *  IMPORTANT: Any modifications to the following identifier names require corresponding
 *  updates to dependent HTML and XML source files.
 */
typedef enum
{
#if defined HAVE_ETHERNET || defined HAVE_WLAN
    NET_IFACE_ENUM_ETH,
#endif // HAVE_ETHERNET | HAVE_WLAN
#ifdef HAVE_WLAN
    NET_IFACE_ENUM_STA,
#endif
#ifdef HAVE_UAP
    NET_IFACE_ENUM_UAP,
#endif // HAVE_UAP

    // add new values above this line
    NET_IFACE_ENUM_COUNT
} net_iface_enum_t;

/*
 *  IMPORTANT: The following g_net_iface_names array elements must map one-to-one with the
 *  net_iface_enum_t enumeration constants.
 */ 
static char *g_net_iface_names[] =
{
#if defined HAVE_ETHERNET || defined HAVE_WLAN
    NET_IFACE_INTERFACE_NAME,
#endif // HAVE_ETHERNET | HAVE_WLAN
#ifdef HAVE_WLAN
    NET_IF_NAME_STA, // == NET_LINK_NAME_MLAN
#endif
#ifdef HAVE_UAP
    NET_IF_NAME_UAP, // "uAP_drvr",
#endif // HAVE_UAP
};


static net_rcode_t net_iface_instance_get_oem_config(uint8_t iface_enum, net_iface_instance_oem_config_t *instance_oem_config);

//=========================================================================================================================
// Public config API
//=========================================================================================================================

net_rcode_t net_iface_get_oem_config(net_iface_oem_config_t *oem_config)
{
    ASSERT((sizeof(g_net_iface_names)/sizeof(char *)) == NET_IFACE_ENUM_COUNT);

    ASSERT(oem_config);
    ASSERT(oem_config->func_table);
    oem_config->func_table->init = NULL;
    oem_config->func_table->instance_get_oem_config = net_iface_instance_get_oem_config;

    oem_config->iface_cnt = NET_IFACE_ENUM_COUNT;

    return NET_OK;
}


//=========================================================================================================================
// Private routines
//=========================================================================================================================


static net_rcode_t net_iface_instance_get_oem_config(uint8_t iface_enum, net_iface_instance_oem_config_t *instance_oem_config)
{
    DBG_VERBOSE("==>%s (iface_enum=%d)\n",__func__, iface_enum);

    ASSERT(instance_oem_config);
    ASSERT(instance_oem_config->func_table);

    net_rcode_t net_rcode = NET_OK;

    if(iface_enum == NET_IFACE_ENUM_COUNT || iface_enum > NET_IFACE_ENUM_COUNT)
    {
        net_rcode = NET_BAD_INDEX;
        DBG_ERR("==>%s(%d) 'bad index' err %d \n",__func__, iface_enum, net_rcode);
        goto done;
    }

    /*
     *  initialize OEM config with default values
     */

    // init function table
    instance_oem_config->func_table->init = NULL;

    ASSERT(g_net_iface_names[iface_enum]);
    strncpy(instance_oem_config->name, g_net_iface_names[iface_enum], NET_IFACE_NAME_SIZE);
    instance_oem_config->name[NET_IFACE_NAME_SIZE-1] = '\0';

    // init NVRAM variable defaults to generic values 
    instance_oem_config->hostname_origin = NET_IFACE_CONFIG_ORIGIN_DEFAULT;
    instance_oem_config->domain_name_origin = NET_IFACE_CONFIG_ORIGIN_DHCP_V4;
    instance_oem_config->manual_domain_name[0] = '\0';
    instance_oem_config->ipv4_enabled = true;
    instance_oem_config->ipv4_config_options = (NET_IFACE_IPV4_USE_DHCP | NET_IFACE_IPV4_USE_AUTOIP);
    instance_oem_config->ipv4_manual_addr = 0;
    instance_oem_config->ipv4_manual_subnet_mask = 0;
    instance_oem_config->ipv4_manual_gateway = 0;
    instance_oem_config->ipv4_manual_wins_addr = 0;
    instance_oem_config->ipv4_manual_dns_preferred_addr = 0;
    instance_oem_config->ipv4_manual_dns_alternate_addr = 0;
#ifdef HAVE_IPV6
    instance_oem_config->ipv6_enabled = true;
    instance_oem_config->ipv6_config_options = NET_IFACE_IPV6_USE_DHCPV6;
    memset(&instance_oem_config->ipv6_manual_addr, 0, sizeof(net_iface_ipv6_addr_t));
#endif // HAVE_IPV6
    instance_oem_config->intf_enabled = true;
    net_rcode = net_iface_get_var_default_hostname(instance_oem_config->manual_hostname, NET_IFACE_HOSTNAME_SIZE);
    if(net_rcode != NET_OK)
    {
        DBG_ERR("==>%s(%d) 'bad hostname' err %d \n",__func__, iface_enum, net_rcode);
        goto done;
    }

    // update NVRAM variable defaults with any interface-specific values
    switch(iface_enum)
    {
#ifdef HAVE_UAP
    case NET_IFACE_ENUM_UAP:
    {
        int index = 0; // revisit 1st table
        wifi_intf_udhcp_get_var_server_addr(index, &instance_oem_config->ipv4_manual_addr);
        wifi_intf_udhcp_get_var_subnet_mask(index, &instance_oem_config->ipv4_manual_subnet_mask);
        //revisit wifi_intf_udhcp_get_var_gateway(index, &instance_oem_config->ipv4_manual_gateway);
        instance_oem_config->ipv4_config_options = NET_IFACE_IPV4_USE_DHCP;

    } break;
        // use defaults for remaining values
#endif // HAVE_UAP

    default:
        // use default values
        break;
    }

done:
    return net_rcode;
}

#ifdef __cplusplus
}
#endif

//eof
