/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_link_config_dflt.c
  * 
  *  This file contains common configuration settings for the net_link module.
  * 
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

// common headers
#include "net_api.h" // api-collection

#ifdef HAVE_ETHERNET
#include "net_eth_api.h"
#include "net_eth.h"
#endif // HAVE_ETHERNET

// debug
#define DBG_PRFX "oem.link.config: "
//off #define DBG_VERBOSE_ON
#include "net_debug.h"

// common headers
#include "net_link_enum.h" // net_link_t

//=========================================================================================================================
// Network interface mapping config
//=========================================================================================================================

/* 
 *  Enumeration of logical network links.
 * 
 *  A logical network link is a logical network link encapsulating one or more physical
 *  network links.
 * 
 *  Each logical network link is automatically attached to the network interface having
 *  the same enumeration value.  Therefore, every logical network link value of type
 *  net_link_logical_enum_t defined below must map one-to-one with a network interface
 *  enumeration value of type net_iface_enum_t defined in net_iface_config_dflt.c (or
 *  net_iface_config.c when HAVE_NET_LINK_OEM_CONFIG is defined).
 *   
 *  To bind one or more physical network links to a network interface, assign the physical
 *  links to the same logical link and then order the logical link enum below such
 *  that its enum value matches the desired network interface enum value.  If multiple
 *  physical links are assigned to the same logical link, those physical links will
 *  share the same network interface.
 */
typedef enum // aka composite
{
#ifdef HAVE_ETHERNET
     NET_LINK_LOGICAL_ENUM_ETH,
#endif // HAVE_ETHERNET
#ifdef HAVE_WLAN
    NET_LINK_LOGICAL_ENUM_WLAN,
#endif // HAVE_WLAN
#ifdef HAVE_UAP
    NET_LINK_LOGICAL_ENUM_UAP,
#endif // HAVE_UAP

    // add new values above this line
    NET_LINK_LOGICAL_ENUM_COUNT
}  net_link_logical_enum_t;


//=========================================================================================================================
// Network link instance config
//=========================================================================================================================

/*
 *  IMPORTANT: Any modifications to the following identifier names require corresponding
 *  updates to dependent HTML and XML source files.
 */
typedef enum
{
#ifdef HAVE_ETHERNET
    NET_LINK_ENUM_ETH,
#endif // HAVE_ETHERNET
#ifdef HAVE_WLAN
    NET_LINK_ENUM_WLAN,
#endif // HAVE_WLAN
#ifdef HAVE_UAP
    NET_LINK_ENUM_UAP,
#endif // HAVE_UAP

    // add new values above this line
    NET_LINK_ENUM_COUNT
} net_link_enum_t;

/*
 *  IMPORTANT: The following net_link_instance_oem_func_table_t array elements must map
 *  one-to-one with the net_link_enum_t enumeration constants.
 */
WEAK net_link_instance_oem_func_table_t g_net_link_instance_oem_func_table[] = 
{
#ifdef HAVE_ETHERNET
    {
        net_eth_init,
    },
#endif // HAVE_ETHERNET
#ifdef HAVE_WLAN
    {
        net_wlan_init,
    },
#endif // HAVE_WLAN
#ifdef HAVE_UAP
    {
        net_uap_init,
    },
#endif // HAVE_UAP
};

/*
 *  IMPORTANT: The following g_net_link_instance_oem_config array elements must map
 *  one-to-one with the net_link_enum_t enumeration constants.
 */
WEAK net_link_instance_oem_config_t g_net_link_instance_oem_config[] = 
{
#ifdef HAVE_ETHERNET
    {
        .name = NET_ETH_LINK_NAME,
        .priority = NET_LINK_PRIORITY_NONE, // highest == 0 (must be unique per interface)
        .flags = NET_LINK_FLAG_DEBOUNCE_ENABLE,
        .logical_link_enum = NET_LINK_LOGICAL_ENUM_ETH, // this enum must map to a network interface enum
    },
#endif // HAVE_ETHERNET
#ifdef HAVE_WLAN
    {
        .name = NET_WLAN_LINK_NAME,
        .priority = 1, // highest == 0 (must be unique per interface)
#ifdef __linux__
        .flags = NET_LINK_FLAG_NONE, // multiple simultaneous
#else // threadx
        .flags = NET_LINK_FLAG_SHARED_EXCLUSIVE,
#endif // threadx
        .logical_link_enum = NET_LINK_LOGICAL_ENUM_WLAN,  // this enum must map to a network interface enum
    },
#endif // HAVE_WLAN
#ifdef HAVE_UAP
    {
        .name = NET_UAP_LINK_NAME,
        .priority = NET_LINK_PRIORITY_NONE, // highest == 0 (must be unique per interface)
#ifdef __linux__
        .flags = NET_LINK_FLAG_NONE, // multiple simultaneous
#else // threadx
        .flags = NET_LINK_FLAG_SHARED_EXCLUSIVE,
#endif // threadx
        .logical_link_enum = NET_LINK_LOGICAL_ENUM_UAP,  // this enum must map to a network interface enum
    },
#endif // HAVE_UAP
};

static net_rcode_t net_link_instance_get_oem_config(uint8_t link_enum, net_link_instance_oem_config_t *instance_oem_config);


//=========================================================================================================================
// Public config API
//=========================================================================================================================

net_rcode_t net_link_get_oem_config(net_link_oem_config_t *oem_config)
{
    ASSERT(oem_config);
    ASSERT(oem_config->func_table);
    oem_config->func_table->init = NULL;
    oem_config->func_table->instance_get_oem_config = net_link_instance_get_oem_config;
    oem_config->link_cnt = NET_LINK_ENUM_COUNT;
    oem_config->links = g_net_link_instance_oem_config;

    // confirm number of oem array elements matches number of net_link_enum_t enumeration constants
    ASSERT((sizeof(g_net_link_instance_oem_func_table)/sizeof(net_link_instance_oem_func_table_t)) == oem_config->link_cnt);
    ASSERT((sizeof(g_net_link_instance_oem_config)/sizeof(net_link_instance_oem_config_t)) ==  oem_config->link_cnt);

    // init the func_table reference in each of the link_config entries
    uint8_t i;
    for(i=0; i<oem_config->link_cnt; i++)
    {
        g_net_link_instance_oem_config[i].func_table = &g_net_link_instance_oem_func_table[i];
    }

    return NET_OK;
}


//=========================================================================================================================
// Private routines
//=========================================================================================================================

static net_rcode_t net_link_instance_get_oem_config(uint8_t link_enum, net_link_instance_oem_config_t *instance_oem_config)
{
    DBG_VERBOSE("==>%s (link_enum=%d)\n", __func__, link_enum);

    ASSERT(instance_oem_config);
    ASSERT(instance_oem_config->func_table);

    net_rcode_t net_rcode = NET_OK;

    uint8_t link_cnt = sizeof(g_net_link_instance_oem_config)/sizeof(net_link_instance_oem_config_t);
    if(link_enum >= link_cnt)
    {
        net_rcode = NET_BAD_INDEX;
        goto done;
    }

    instance_oem_config->func_table->init = g_net_link_instance_oem_config->func_table[link_enum].init;
    instance_oem_config->priority = g_net_link_instance_oem_config[link_enum].priority;
    instance_oem_config->flags = g_net_link_instance_oem_config[link_enum].flags;
    instance_oem_config->logical_link_enum = g_net_link_instance_oem_config[link_enum].logical_link_enum;

    strncpy(instance_oem_config->name, g_net_link_instance_oem_config[link_enum].name, NET_LINK_NAME_SIZE);
    instance_oem_config->name[NET_LINK_NAME_SIZE-1] = '\0';

done:
    DBG_VERBOSE("<==%s (net_rcode=%d)\n", __func__, net_rcode);
    return net_rcode;
}

#ifdef __cplusplus
}
#endif

// eof
