/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
// work in progress to distinguish between common/network and oem/network
// Includes WEAK default globals subject to redefinition by oem-specific settings

#ifdef __cplusplus
extern "C" {
#endif

// os includes
#include <string.h>
#include <stdio.h>

// common headers
#include "os_network.h"
#include "memAPI.h"
#include "sys_init_api.h"
#include "error_types.h"
#include "data_access_api.h" // NET_IFACE_VARS_ID
#include "nvram_api.h"

// local includes
#include "net_api.h"
#include "net_iface_config.h" // net_iface_instance_oem_config_t
#include "net_iface_api.h"
#include "net_intf_api.h" // get_intf_mac_addr()
#include "net_iface.h" // net_interface_t
#include "net_conf_api.h" // DEFAULT_PLATFORM_HOSTNAME
#include "oid_api.h" // oid_t
#include "oid_producer_api.h" // oid_t


// globals  (base default settings -- intended to be overridden w/oem-specifics)

WEAK UserBootEntry_t g_boot_entry_default;
WEAK net_interface_t g_net_lo =
{
    .name = NET_LO_LINK_NAME,
    .link_handle = NET_IF_TYPE_LO,
    .local_idx = NET_IF_TYPE_LO,
};
WEAK net_interface_t g_net_if_eth =
{
    .name = NET_ETH_LINK_NAME,
    .link_handle = NET_IF_TYPE_ETH,
    .local_idx = NET_IF_TYPE_ETH,
};
WEAK net_interface_t* g_net_interface = &g_net_if_eth; // default to wired
WEAK net_iface_context_t g_interface_context = 
{
   .iface_cnt = 0, // runtime-determined ifaces[]
   .ifaces = { NULL },
   .event_list_head = NULL,
   .initialized = false, 
   .mtx = {},
   .oem_func_table = {},
   .oem_config = {},
};

// Global singleton (bound with oem-specifics @ runtime)
net_iface_context_t* g_net_context = &g_interface_context; // initial intf

//--------------------------------------
// Local Variables
//--------------------------------------

static bool g_net_iface_nvram_is_registered = false;
static nvram_handle_t *g_net_iface_nvram_hndl[NET_MAX_INTERFACES];
net_iface_perm_vars_t g_net_iface_perm_vars[NET_MAX_INTERFACES]
= {
{ 0 }
};
static pthread_mutex_t g_net_iface_var_mutex;

static net_iface_rcode_t net_iface_var_restore_shadow_defaults(uint8_t iface_enum, net_iface_var_id_t var_id);
static net_iface_rcode_t net_iface_nvram_block_write(unsigned int if_index);
static net_iface_rcode_t net_iface_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *user_data);

// local utility/helper routines
static int validate_hostname_chars(const char *Test);
static int validate_domain_chars(const char *Test);


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "net.if.var: "
#define DBG_ON /* DBG_MSG */
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON

#include "net_debug.h" // DBG_ default macros

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_IFACE_VAR_LOCK() \
        { \
            int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_iface_var_mutex); \
            XASSERT(os_rcode == 0, os_rcode); \
        }

#define NET_IFACE_VAR_UNLOCK() \
        { \
            int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_iface_var_mutex); \
            XASSERT(os_rcode == 0, os_rcode); \
        } 


/** 
 *  Boot-time initialization
 * 
 *  This function is called at system startup following initialization of the NVRAM driver and
 *  partition manager.  It initializes the module datastore variables to either their default values
 *  or values previously stored in NVRAM.
 * 
 *  \param[in] init_type type of initialization being done
 **/ 
void net_iface_vars_init(void)
{
    error_type_t retval;
    unsigned os_rcode;

    os_rcode= posix_mutex_init(&g_net_iface_var_mutex);
    XASSERT(os_rcode==0, os_rcode);

    char vars_id[5]; // 4 char NVRAM label (as defined in data_access_api.h) + NULL terminator
    char vars_id_prefix[4]; // 3 char prefix + NULL terminator 
    strncpy(vars_id_prefix, NET_IFACE_VARS_ID, 3);
    vars_id_prefix[3] = '\0';

    // net_iface_context_t *net_ctxt;
    // net_ctxt = 
    net_get_ctxt();

    uint8_t iface_enum;
    for( iface_enum = 0; iface_enum < NET_MAX_INTERFACES; iface_enum++) // allocate (possibly) sparse regions
    {
        /*
         *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
         *  net_iface_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
         *  values.
         */
        if(iface_enum>63) break; // hard limit imposed by base64 encoding of index
        sprintf(vars_id, "%s%c", vars_id_prefix, net_base64_uint8_to_char(iface_enum));
        DBG_VERBOSE("registering NVRAM label %s (cb=%08x)\n", vars_id, net_iface_nvram_init_cb);
        retval = nvram_variable_register(
            g_net_iface_nvram_hndl+iface_enum, 
            vars_id,
            NET_IFACE_VARS_VERSION, 
            sizeof(net_iface_perm_vars_t), 
            net_iface_nvram_init_cb, (void *)(uint32_t)iface_enum);
        if(retval != OK)
        {
            // NVRAM is not available so just set defaults and operate on the local copies       
            net_iface_var_restore_shadow_defaults(iface_enum, NET_IFACE_VAR_ALL);
        }
        DBG_ASSERT(g_net_iface_nvram_hndl+iface_enum);
    }
    g_net_iface_nvram_is_registered = true;

    return;
}

net_iface_rcode_t net_iface_var_restore_defaults(unsigned int if_index, net_iface_var_id_t var_id)
{
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    net_iface_rcode_t rcode;
    rcode = net_iface_var_restore_shadow_defaults(iface->local_idx, var_id);
    if(rcode == NET_IFACE_OK)
    {
        net_iface_nvram_block_write(if_index);
        net_config_change_notify(if_index);
    }
    return rcode;
}

static net_iface_rcode_t net_iface_var_restore_shadow_defaults(uint8_t iface_enum, net_iface_var_id_t var_id)
{
    bool all_vars;
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    all_vars = (var_id == NET_IFACE_VAR_ALL);

    net_iface_context_t *net_ctxt;
    net_ctxt = net_get_ctxt();
    net_interface_t *iface =  net_ctxt->ifaces[iface_enum];
    if(!iface)
    {
        // enumeration failed for this interface, so skip restore
        // 
        // ***TODO*** make oem_config global and not part of iface, so we can access even if
        // iface enum failed. Otherwise we will have invalid defaults when iface enum does
        // succeed (e.g. after burning static NVRAM to get valid MAC addr, on Ethernet-only
        // products where iface not shared with wifi STA, resulting in iface failing to enum).
        // One symptom is that both ipv4 and ipv6 settings will default to disabled, so that 
        // we do not acquire an IP address.  Do restore network defaults from ctrl pnl to recover.
        goto done;
    }

    NET_IFACE_VAR_LOCK();

    // set defaults using per-interface OEM config 
    switch(var_id)
    {
    case NET_IFACE_VAR_ALL:
    case NET_IFACE_VAR_HOSTNAME_ORIGIN:
        g_net_iface_perm_vars[iface_enum].hostname_origin = iface->oem_config.hostname_origin;
        if (!all_vars) break;

    case NET_IFACE_VAR_DOMAIN_NAME_ORIGIN:
        g_net_iface_perm_vars[iface_enum].domain_name_origin = iface->oem_config.domain_name_origin;
        if (!all_vars) break;

    case NET_IFACE_VAR_MANUAL_HOSTNAME:
        strncpy(g_net_iface_perm_vars[iface_enum].manual_hostname, iface->oem_config.manual_hostname, NET_IFACE_HOSTNAME_SIZE);
        g_net_iface_perm_vars[iface_enum].manual_hostname[NET_IFACE_HOSTNAME_SIZE-1] = '\0';
        if (!all_vars) break;

    case NET_IFACE_VAR_MANUAL_DOMAIN_NAME:
        g_net_iface_perm_vars[iface_enum].manual_domain_name[0] = '\0';
        strncpy(g_net_iface_perm_vars[iface_enum].manual_domain_name, iface->oem_config.manual_domain_name, NET_IFACE_DOMAINNAME_SIZE);
        g_net_iface_perm_vars[iface_enum].manual_domain_name[NET_IFACE_DOMAINNAME_SIZE-1] = '\0';
        if (!all_vars) break;

    case NET_IFACE_VAR_INTF_ENABLED:
        g_net_iface_perm_vars[iface_enum].intf_enabled = iface->oem_config.intf_enabled;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_ENABLED:
        g_net_iface_perm_vars[iface_enum].ipv4_enabled = iface->oem_config.ipv4_enabled;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_CONFIG_OPTIONS:
        g_net_iface_perm_vars[iface_enum].ipv4_config_options = iface->oem_config.ipv4_config_options;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_LAST_CONFIG_METHOD:
        g_net_iface_perm_vars[iface_enum].ipv4_last_config_method = NET_IFACE_IPV4_CONFIGURED_DHCP;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_MANUAL_ADDR:
        g_net_iface_perm_vars[iface_enum].ipv4_manual_addr = iface->oem_config.ipv4_manual_addr;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_MANUAL_SUBNET_MASK:
        g_net_iface_perm_vars[iface_enum].ipv4_manual_subnet_mask = iface->oem_config.ipv4_manual_subnet_mask;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_MANUAL_GATEWAY:
        g_net_iface_perm_vars[iface_enum].ipv4_manual_gateway = iface->oem_config.ipv4_manual_gateway;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_MANUAL_WINS_ADDR:
        g_net_iface_perm_vars[iface_enum].ipv4_manual_wins_addr = iface->oem_config.ipv4_manual_wins_addr;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_AUTOIP_ADDR:
        g_net_iface_perm_vars[iface_enum].ipv4_autoip_addr = 0;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_MANUAL_DNS_PREFERRED_ADDR:
        g_net_iface_perm_vars[iface_enum].ipv4_manual_dns_preferred_addr = iface->oem_config.ipv4_manual_dns_preferred_addr;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV4_MANUAL_DNS_ALTERNATE_ADDR:
        g_net_iface_perm_vars[iface_enum].ipv4_manual_dns_alternate_addr = iface->oem_config.ipv4_manual_dns_alternate_addr;
        if (!all_vars) break;

#ifdef HAVE_IPV6
    case NET_IFACE_VAR_IPV6_ENABLED:
        g_net_iface_perm_vars[iface_enum].ipv6_enabled = iface->oem_config.ipv6_enabled;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV6_CONFIG_OPTIONS:
        g_net_iface_perm_vars[iface_enum].ipv6_config_options = iface->oem_config.ipv6_config_options;
        if (!all_vars) break;

    case NET_IFACE_VAR_IPV6_MANUAL_ADDR:
        memcpy(&g_net_iface_perm_vars[iface_enum].ipv6_manual_addr, &iface->oem_config.ipv6_manual_addr,
               sizeof(g_net_iface_perm_vars[iface_enum].ipv6_manual_addr));
        if (!all_vars) break;
#endif

#ifdef TM_USE_PPPOE_CLIENT
    case NET_IFACE_VAR_PPPOE_ENABLED:
        g_net_iface_perm_vars[iface_enum].pppoe_enabled = false;
        if (!all_vars) break;

    case NET_IFACE_VAR_PPPOE_USERNAME:
        memset(g_net_iface_perm_vars[iface_enum].pppoe_username, 0, PPPOE_USERNAME_MAX_LENGTH);
        if (!all_vars) break;

    case NET_IFACE_VAR_PPPOE_PASSWORD:
        memset(g_net_iface_perm_vars[iface_enum].pppoe_password, 0, PPPOE_PASSWORD_MAX_LENGTH);
        if (!all_vars) break;
#endif // TM_USE_PPPOE_CLIENT

    default:
        // do nothing
        break;
    }

    NET_IFACE_VAR_UNLOCK();

done:
    return net_iface_rcode;
}

static net_iface_rcode_t net_iface_nvram_block_write(unsigned int if_index)
{
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    error_type_t retval = NET_IFACE_OK;

    if(g_net_iface_nvram_is_registered)
    {
        retval = nvram_set_var(g_net_iface_nvram_hndl[iface->local_idx], g_net_iface_perm_vars+(iface->local_idx));
        if(retval < 0)
        {
            retval = NET_IFACE_FAIL;
        }
    }

    return retval;
}

/** 
 *  Initialize or repair a discrete dynamic NVRAM block
 *  
 *  This function is called by the dynamic NVRAM driver during the call to nvram_variable_register()
 *  either to initialize a new block of data or to repair an existing block which has a bad CRC,
 *  version discrepancy, or change in size.
 *  
 *  \param[in] init_type type of handling needed:  nvram_init_type_t enum value from nvram_api.h
 *  
 *  \param[in,out] init_loc pointer to existing block data on entry, destination for new
 *  block data to be returned 
 *  
 *  \return net_iface_rcode_t
 *  \retval OK
 *
 **/ 
static net_iface_rcode_t net_iface_nvram_init_cb(nvram_init_type_t init_type, uint16_t version,
                                            void *init_loc, uint32_t init_loc_size, void *user_data)
{
    ASSERT( NULL != init_loc );
    uint8_t iface_enum = (uint8_t)(uint32_t)user_data;
    XASSERT(iface_enum < NET_MAX_INTERFACES, iface_enum);

    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            NET_IFACE_VAR_LOCK();
            net_iface_var_restore_shadow_defaults(iface_enum, NET_IFACE_VAR_ALL);
            ASSERT(sizeof(net_iface_perm_vars_t) <= init_loc_size);
            memcpy(init_loc, g_net_iface_perm_vars+iface_enum, sizeof(net_iface_perm_vars_t));
            NET_IFACE_VAR_UNLOCK();
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(g_net_iface_perm_vars+iface_enum, init_loc, sizeof(net_iface_perm_vars_t));
            break;
    }
    return NET_IFACE_OK;
}


//=========================================================================================================================
// DATASTORE: Network configuration variables
//=========================================================================================================================

net_iface_rcode_t net_iface_get_var_network_status(
    unsigned int if_index,
    net_iface_status_t *status)
{
    DBG_VERBOSE(" ==> %s()\n", __func__);
    bool link_up = false;
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    ASSERT( NULL != status );
    *status = 0;

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        net_iface_rcode = NET_IFACE_BADINDEX;
        goto done;
    }

    net_iface_context_t *net_ctxt;
    net_ctxt = net_lock_ctxt();

    /* Check if network has been started */
    if(iface->state == NET_IFACE_STATE_NOT_STARTED)
    {
        net_unlock_ctxt(net_ctxt);
        goto done;
    }
    *status |= NET_IFACE_STATUS_NETWORK_STARTED;

    /* Check if logical link is up */
    net_link_status_t link_status;
    ASSERT(iface->link_handle != (net_link_handle_t)NET_LINK_INVALID_HANDLE);
    link_status = net_link_get_status(iface->link_handle);
    if( link_status == NET_LINK_STATUS_UP )
    {
        *status |= NET_IFACE_STATUS_LINK_UP;
        link_up = true;
    }

    bool ipv4_enabled;
    net_iface_rcode = net_iface_get_var_ipv4_enabled(if_index, &ipv4_enabled);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        net_unlock_ctxt(net_ctxt);
        goto done;
    }

    if(ipv4_enabled)
    {
        *status |= NET_IFACE_STATUS_IPV4_ENABLED;

        if( link_up && (iface->state == NET_IFACE_STATE_OPENING_INTERFACE
            || iface->state == NET_IFACE_STATE_INTERFACE_OPEN) )
        {
            if(iface->ipv4_cfg_state == IPV4_CONFIGURING)
            {
                *status |= NET_IFACE_STATUS_IPV4_INITIALIZING;
            }
            else if(iface->ipv4_cfg_state == IPV4_CONFIGURED)
            {
                *status |= NET_IFACE_STATUS_IPV4_CONFIGURED;
            }
            // otherwise not configured, e.g. while closing due to net reset

            if(iface->ipv4_cfg_status & IPV4_DUPLICATE_ADDR)
            {
                *status |= NET_IFACE_STATUS_IPV4_DUPLICATE_ADDR;
            }

            if(iface->ipv4_cfg_status & IPV4_BAD_MANUAL_CONFIG)
            {
                *status |= NET_IFACE_STATUS_IPV4_BAD_MANUAL_CONFIG;
            }

            if(iface->ipv4_cfg_status & IPV4_AUTOCONFIG_TIMEOUT)
            {
                *status |= NET_IFACE_STATUS_IPV4_AUTOCONFIG_TIMEOUT;
            }
        }
    }

#ifdef HAVE_IPV6
    bool ipv6_enabled;
    net_iface_rcode = net_iface_get_var_ipv6_enabled(if_index, &ipv6_enabled);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        net_unlock_ctxt(net_ctxt);
        goto done;
    }

    if(ipv6_enabled)
    {
        *status |= NET_IFACE_STATUS_IPV6_ENABLED;

        if( link_up && (iface->state == NET_IFACE_STATE_OPENING_INTERFACE
            || iface->state == NET_IFACE_STATE_INTERFACE_OPEN) )
        {
            if(iface->ipv6_cfg_state == IPV6_CONFIGURING ||
               iface->ipv6_cfg_state == IPV6_LOCAL_CONFIGURED)
            {
                *status |= NET_IFACE_STATUS_IPV6_INITIALIZING;
            }

            if(iface->ipv6_cfg_state >= IPV6_LOCAL_CONFIGURED)
            {
                *status |= NET_IFACE_STATUS_IPV6_LOCAL_CONFIGURED;
            }

            if(iface->ipv6_cfg_state == IPV6_CONFIGURED)
            {
                *status |= NET_IFACE_STATUS_IPV6_CONFIGURED;
            }

            if(iface->ipv6_cfg_status & IPV6_GLOBAL_CONFIG_TIMEOUT)
            {
                *status |= NET_IFACE_STATUS_IPV6_GLOBAL_CONFIG_TIMEOUT;
            }

            if(iface->ipv6_cfg_status & IPV6_GLOBAL_LOST_CONFIG)
            {
                *status |= NET_IFACE_STATUS_IPV6_GLOBAL_LOST_CONFIG;
            }

            if(iface->ipv6_cfg_status & IPV6_DUPLICATE_ADDR)
            {
                *status |= NET_IFACE_STATUS_IPV6_DUPLICATE_ADDR;
            }

            if(iface->ipv6_cfg_status & IPV6_BAD_MANUAL_CONFIG)
            {
                *status |= NET_IFACE_STATUS_IPV6_BAD_MANUAL_CONFIG;
            }
        }
    }
#endif // HAVE_IPV6

    net_unlock_ctxt(net_ctxt);

done:
    DBG_VERBOSE(" ==> %s(%d): %s (x%x)\n", __func__, if_index, (*status?"enabled":"disabled"),*status);
    return net_iface_rcode;
}

net_iface_rcode_t net_iface_get_var_active_link(
    unsigned int if_index,
    unsigned int *link_index)
{
    DBG_VERBOSE("==>%s(%d)\n", __func__, if_index);
    *link_index = 0;
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        DBG_VERBOSE("<==%s(%d) interface undefined, ignored\n", __func__, if_index);
        return NET_IFACE_BADINDEX;
    }

    if(iface->link_handle == (net_link_handle_t)NET_LINK_INVALID_HANDLE)
    {
        DBG_ERR("<==%s(%d) invalid link, ignored\n", __func__, if_index);
        // link has not yet been enumerated
        return NET_IFACE_NOT_PERMITTED;
    }
    net_link_get_active_link((unsigned int)(iface->link_handle), link_index); // cast

    // if_index -> link_index
    DBG_VERBOSE("<==%s(%d) link_index %d\n", __func__, if_index, *link_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_phys_addr(
    uint8_t *phys_addr,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    if(!len)
    { 
        return NET_IFACE_OK;
    }
    ASSERT( NULL != phys_addr );
    memset(phys_addr, 0, len); // default

    // select representitive link
    // by default use first enumerated link as defined in OEM config (or default config file)

    XASSERT(len >= MAC_ADDR_BYTE_SIZE, len); // get_intf_mac_addr() assumes MAC_ADDR_BYTE_SIZE buffer
    get_intf_mac_addr(NET_ETH_LINK_NAME, phys_addr); // revisit wired-only mac

    return NET_IFACE_OK;
}

// NOTE: The link_index value is not the same as the oem_enum value defined by the 
// OEM-specific link config file (or by the default link config file if an OEM file
// has not been provided).  link_index is offset by one (1-based) vs. OEM-assigned
// oem_enum index (0-based).  This is to maintain consistency with official BSD
// socket API if_index (also 1-based).
net_iface_rcode_t net_iface_get_var_link_phys_addr(
    unsigned int link_index,
    uint8_t *phys_addr,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    return net_link_get_phys_addr(link_index, phys_addr, len);
}

net_iface_rcode_t net_iface_get_var_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    if(!len)
    { 
        return NET_IFACE_OK;
    }
    ASSERT( NULL != hostname );

    net_iface_config_origin_t origin = { 0 };
    net_iface_rcode = net_iface_get_var_hostname_origin(if_index, &origin);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    switch(origin)
    {
    case NET_IFACE_CONFIG_ORIGIN_DEFAULT:
        net_iface_rcode = net_iface_get_var_default_hostname(hostname, len);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
        break;

    case NET_IFACE_CONFIG_ORIGIN_MANUAL:
        net_iface_rcode = net_iface_get_var_manual_hostname(if_index, hostname, len);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
        break;

    case NET_IFACE_CONFIG_ORIGIN_DHCP_V4:
        net_iface_rcode = net_iface_get_var_dhcpv4_hostname(if_index, hostname, len);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
        break;

#ifdef HAVE_IPV6
    case NET_IFACE_CONFIG_ORIGIN_DHCP_V6:
        net_iface_rcode = net_iface_get_var_dhcpv6_hostname(if_index, hostname, len);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
        break;
#endif // HAVE_IPV6

    default:
        // unhandled preferred_origin value
        XASSERT(0, origin);
        break;
    }

done:
    return net_iface_rcode;
}

net_iface_rcode_t net_iface_get_var_default_hostname(
    char *hostname,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    char tmp_hostname[2*NET_IFACE_HOSTNAME_SIZE];
    uint8_t phys_addr[TM_ETHERNET_PHY_ADDR_LEN];

    if(!len)
    { 
        return NET_IFACE_OK;
    }
    ASSERT( NULL != hostname );
    hostname[0] = '\0';

#if __linux__
    char* current_hostname = DEFAULT_PLATFORM_HOSTNAME;
    //net_iface_get_var_hostname(0, current_hostname, 32); // default to 1st interface
    net_iface_get_var_phys_addr(phys_addr, TM_ETHERNET_PHY_ADDR_LEN);
    minSprintf(tmp_hostname, "%s[%02X%02X%02X]", current_hostname, (uint8_t)phys_addr[3], (uint8_t)phys_addr[4], (uint8_t)phys_addr[5]);
    strncpy(hostname, tmp_hostname, len);
#else
    net_iface_get_var_phys_addr(phys_addr, TM_ETHERNET_PHY_ADDR_LEN);
    minSprintf(tmp_hostname, "DEV%02X%02X%02X", (uint8_t)phys_addr[3], (uint8_t)phys_addr[4], (uint8_t)phys_addr[5]);
    strncpy(hostname, tmp_hostname, len);
#endif
    hostname[len-1] = '\0';

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_hostname_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    // get just enough of hostname to determine if it's populated (i.e. one char plus null terminator)
    char hostname[20]; 

    ASSERT( NULL != origin );
    net_iface_rcode = net_iface_get_var_manual_hostname(if_index, hostname, sizeof(hostname));
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    bool have_manual = (hostname[0] != '\0');

    net_iface_config_origin_t preferred_origin;
    net_iface_rcode = net_iface_get_var_hostname_preferred_origin(if_index, &preferred_origin);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    *origin = NET_IFACE_CONFIG_ORIGIN_DEFAULT;
    switch(preferred_origin)
    {
    case NET_IFACE_CONFIG_ORIGIN_DEFAULT:
        break;

    case NET_IFACE_CONFIG_ORIGIN_MANUAL:
        if(have_manual)
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
        }
        break;

    case NET_IFACE_CONFIG_ORIGIN_DHCP_V4:
        net_iface_rcode = net_iface_get_var_dhcpv4_hostname(if_index, hostname, sizeof(hostname));
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }

        if(hostname[0] != '\0')
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_DHCP_V4;
        }
        else if(have_manual)
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
        }
        break;

#ifdef HAVE_IPV6
    case NET_IFACE_CONFIG_ORIGIN_DHCP_V6:
        net_iface_rcode = net_iface_get_var_dhcpv6_hostname(if_index, hostname, sizeof(hostname));
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }

        if(hostname[0] != '\0')
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_DHCP_V6;
        }
        else if(have_manual)
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
        }
        break;
#endif // HAVE_IPV6

    default:
        // unhandled preferred_origin value
        DBG_ERR("%s() %d?\n", __func__, preferred_origin);
        XASSERT(0, preferred_origin);
    }

done:
    return net_iface_rcode;
}

net_iface_rcode_t net_iface_get_var_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    // verify that the value defined in the vars API matches value defined by Treck
    ASSERT(NET_IFACE_DOMAINNAME_SIZE == TM_BOOTSNAME_SIZE);
    ASSERT( NULL != domain );

    if(!len)
    { 
        goto done;
    }

    net_iface_config_origin_t origin;
    net_iface_rcode = net_iface_get_var_domain_origin(if_index, &origin);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }


    switch(origin)
    {
    case NET_IFACE_CONFIG_ORIGIN_DEFAULT:
        domain[0] = '\0'; // no default value for domain
        break;

    case NET_IFACE_CONFIG_ORIGIN_MANUAL:
        net_iface_rcode = net_iface_get_var_manual_domain(if_index, domain, len);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
        break;

    case NET_IFACE_CONFIG_ORIGIN_DHCP_V4:
        net_iface_rcode = net_iface_get_var_dhcpv4_domain(if_index, domain, len);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
        break;

#ifdef HAVE_IPV6
    case NET_IFACE_CONFIG_ORIGIN_DHCP_V6:
        net_iface_rcode = net_iface_get_var_dhcpv6_domain(if_index, domain, len);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
        break;
#endif // HAVE_IPV6

    default:
        // unhandled preferred_origin value
        XASSERT(0, origin);
    }

done:
    return net_iface_rcode;
}

net_iface_rcode_t net_iface_get_var_domain_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    // get just enough of domain to determine if it's populated (i.e. one char plus null terminator)
    char domain[2]; 

    ASSERT( NULL != origin );
    net_iface_rcode = net_iface_get_var_manual_domain(if_index, domain, sizeof(domain));
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    bool have_manual = (domain[0] != '\0');

    net_iface_config_origin_t preferred_origin;
    net_iface_rcode = net_iface_get_var_domain_preferred_origin(if_index, &preferred_origin);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    *origin = NET_IFACE_CONFIG_ORIGIN_DEFAULT;
    switch(preferred_origin)
    {
    case NET_IFACE_CONFIG_ORIGIN_DEFAULT:
        break;

    case NET_IFACE_CONFIG_ORIGIN_MANUAL:
        if(have_manual)
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
        }
        break;

    case NET_IFACE_CONFIG_ORIGIN_DHCP_V4:
        net_iface_rcode = net_iface_get_var_dhcpv4_domain(if_index, domain, sizeof(domain));
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }

        if(domain[0] != '\0')
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_DHCP_V4;
        }
        else if(have_manual)
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
        }
        break;

#ifdef HAVE_IPV6
    case NET_IFACE_CONFIG_ORIGIN_DHCP_V6:
        net_iface_rcode = net_iface_get_var_dhcpv6_domain(if_index, domain, sizeof(domain));
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }

        if(domain[0] != '\0')
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_DHCP_V6;
        }
        else if(have_manual)
        {
            *origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
        }
        break;
#endif // HAVE_IPV6

    default:
        // unhandled preferred_origin value
        XASSERT(0, preferred_origin);
    }

done:
    return net_iface_rcode;
}

net_iface_rcode_t net_iface_get_var_hostname_preferred_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != origin );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }
    
    NET_IFACE_VAR_LOCK();
    *origin = g_net_iface_perm_vars[iface->local_idx].hostname_origin;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_hostname_preferred_origin(
    unsigned int if_index, 
    const net_iface_config_origin_t *origin)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != origin );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }
    
    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].hostname_origin == *origin)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    // get actual origin before modifying requested origin
    net_iface_config_origin_t prev_origin = NET_IFACE_CONFIG_ORIGIN_DEFAULT;
	net_iface_config_origin_t cur_origin = NET_IFACE_CONFIG_ORIGIN_DEFAULT;
    net_iface_get_var_hostname_origin(if_index, &prev_origin);

    g_net_iface_perm_vars[iface->local_idx].hostname_origin = *origin; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    // only trigger net reset if modifying requested origin results in a change to actual origin
    net_iface_get_var_hostname_origin(if_index, &cur_origin);
    if(prev_origin != cur_origin)
    {
        // assume name has changed -- trigger registration of the new name
        net_config_change_notify(if_index);
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_domain_preferred_origin(
    unsigned int if_index,
    net_iface_config_origin_t *origin)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != origin );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }
    
    NET_IFACE_VAR_LOCK();
    *origin = g_net_iface_perm_vars[iface->local_idx].domain_name_origin;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_domain_preferred_origin(
    unsigned int if_index,
    const net_iface_config_origin_t *origin)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != origin );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }
    
    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].domain_name_origin == *origin)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].domain_name_origin = *origin; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    // NOTE: net reset is not necessary because this setting has no system dependencies

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_manual_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    if(!len)
    { 
        return NET_IFACE_OK;
    }
    ASSERT( NULL != hostname );
    hostname[0] = '\0'; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }
    
    NET_IFACE_VAR_LOCK();
    strncpy(hostname, g_net_iface_perm_vars[iface->local_idx].manual_hostname, len);
    hostname[len-1] = '\0';
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_manual_hostname(
    unsigned int if_index,
    const char *hostname)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != hostname );
    ASSERT(NET_IFACE_HOSTNAME_SIZE <= TM_BOOTSNAME_SIZE);
    net_iface_rcode_t net_iface_rcode;

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }
    
    if(!hostname || validate_hostname_chars(hostname))
    {
        // invalid parameter
        return NET_IFACE_FAIL;
    }

    NET_IFACE_VAR_LOCK();
    if(strncmp(g_net_iface_perm_vars[iface->local_idx].manual_hostname, hostname, NET_IFACE_HOSTNAME_SIZE) == 0)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    net_iface_config_origin_t tmp_origin = NET_IFACE_CONFIG_ORIGIN_DEFAULT;
    net_iface_get_var_hostname_origin(if_index, &tmp_origin);

    net_iface_rcode = net_set_manual_hostname(if_index, hostname);
    NET_IFACE_VAR_UNLOCK();

    if(net_iface_rcode == NET_IFACE_OK && tmp_origin == NET_IFACE_CONFIG_ORIGIN_MANUAL)
    {
        net_config_change_notify(if_index); // trigger DNS registration for new name
    }
    return net_iface_rcode;
}

// *** internal only ***
// updates setting without triggering net reset
net_iface_rcode_t net_set_manual_hostname(
    unsigned int if_index,
    const char *hostname)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != hostname );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    strncpy(g_net_iface_perm_vars[iface->local_idx].manual_hostname, hostname, NET_IFACE_HOSTNAME_SIZE);
    g_net_iface_perm_vars[iface->local_idx].manual_hostname[NET_IFACE_HOSTNAME_SIZE-1] = '\0';
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_manual_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    // verify that the value defined in the vars API matches value defined by Treck
    ASSERT(NET_IFACE_DOMAINNAME_SIZE == TM_BOOTSNAME_SIZE);

    if(!len)
    { 
        return NET_IFACE_OK;
    }
    ASSERT( NULL != domain );
    domain[0] = '\0'; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    strncpy(domain, g_net_iface_perm_vars[iface->local_idx].manual_domain_name, len);
    domain[len-1] = '\0';
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_manual_domain(
    unsigned int if_index,
    const char *domain)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != domain );
    if(!domain || validate_domain_chars(domain))
    {
        // invalid parameter
        return NET_IFACE_FAIL;
    }

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(strncmp(g_net_iface_perm_vars[iface->local_idx].manual_domain_name, domain, NET_IFACE_DOMAINNAME_SIZE) == 0)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    strncpy(g_net_iface_perm_vars[iface->local_idx].manual_domain_name, domain, NET_IFACE_DOMAINNAME_SIZE);
    g_net_iface_perm_vars[iface->local_idx].manual_domain_name[NET_IFACE_DOMAINNAME_SIZE-1] = '\0';
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

// rdj - net reset isn't necessary because this setting has no system dependencies
//    net_config_change_notify();

    return NET_IFACE_OK;
}

// OID_SM_SYSTEM_MORE_INFO
net_iface_rcode_t net_iface_get_var_device_url(unsigned int if_index, char *device_url,  uint8_t len )
{
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    if(!len)
    { 
        goto done;
    }
    ASSERT( NULL != device_url );
    device_url[0] = '\0'; // default

    uint32_t ip_addr;
    net_iface_rcode = net_iface_get_var_ipv4_addr(if_index, &ip_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }
    uint8_t *ip = (uint8_t*)(&ip_addr);
    minNSprintf (device_url, len, "http://%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    device_url[len-1] = '\0';

done:
    return net_iface_rcode;

}

#ifdef HAVE_IPV6
// OID_SM_SYSTEM_MORE_INFO_IPV6
net_iface_rcode_t net_iface_get_var_device_url_ipv6(unsigned int if_index, char *device_url,  uint8_t len )
{
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    if(!len)
    { 
        goto done;
    }
    ASSERT( NULL != device_url );
    device_url[0] = '\0'; // default

    /* 
     * TODO rdj 10-2012:
     * 
     * When creating a device URL with local IPv6 addr as hostname, we should select from
     * available muti-homed IPv6 address based on the rules defined in
     * http://tools.ietf.org/html/draft-ietf-ipv6-default-addr-select-08#section-5 . These
     * rules are implemented in Treck by tf6SelectSourceAddress() in t6ip.c, accessed via
     * the public API getsockname().
     * 
     * getsockname requires socket descriptor as a parameter, since local addr selection
     * is dependent on peer addr.  For now we will use if_index and select most preferred
     * IPv6 addr independent of dest addr.  possibly add another API that accepts sock
     * descriptor when available, so local addr can be correctly chosen based on peer addr
     * (including IPv4 vs. IPv6 addr -- see OID_NETWORK_IP_ADDRESS_FROM_CONNECTION for
     * example).
     */

    net_iface_ipv6_addr_t ipv6_addr;
    net_iface_ipv6_addr_hndl_t addr_hndl;
    net_iface_rcode = net_iface_get_var_ipv6_preferred_addr(if_index, &ipv6_addr, &addr_hndl);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    if(NET_IFACE_IPV6_ADDR_UNSPECIFIED(ipv6_addr))
    {
        // use link local IPv6 address
        net_iface_get_var_ipv6_link_local_addr(if_index, &ipv6_addr, &addr_hndl);
    }

    if(!NET_IFACE_IPV6_ADDR_UNSPECIFIED(ipv6_addr))
    {
        // got a local IPv6 addr -- create the URL
        char addr_str[INET6_ADDRSTRLEN];
        addr_str[0] = '\0';
        inet_ntop(AF_INET6, (void *)(&ipv6_addr.addr), addr_str, INET6_ADDRSTRLEN);
        minNSprintf (device_url, len, "http://%s", addr_str);
        device_url[len-1] = '\0';
    }

done:
    return net_iface_rcode;
}
#endif // HAVE_IPV6

//=========================================================================================================================
// DATASTORE: IPv4 configuration variables
//=========================================================================================================================

net_iface_rcode_t net_iface_get_var_intf_enabled(
    unsigned int if_index,
    bool *ref_enabled)
{
    DBG_VERBOSE("==> %s\n", __func__);
    DBG_ASSERT( NULL != ref_enabled );
    if (ref_enabled) *ref_enabled = false; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if (ref_enabled)
    {
        NET_IFACE_VAR_LOCK();
  
        *ref_enabled = g_net_iface_perm_vars[iface->local_idx].intf_enabled;

        NET_IFACE_VAR_UNLOCK();
        DBG_VERBOSE("<== %s(%d) %d\n", __func__, if_index, *ref_enabled);
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_intf_enabled(
    unsigned int if_index,
    bool enabled)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].intf_enabled == enabled)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].intf_enabled = enabled; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_enabled(
    unsigned int if_index,
    bool* ref_enabled)
{
    DBG_ASSERT( NULL != ref_enabled );
    if (ref_enabled) *ref_enabled = false; // default

    net_interface_t* iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        DBG_ERR("%s(%d) undefined\n", __func__, if_index);
        return NET_IFACE_BADINDEX;
    }

    if (ref_enabled)
    {
        NET_IFACE_VAR_LOCK();
        *ref_enabled = g_net_iface_perm_vars[iface->local_idx].ipv4_enabled;
        NET_IFACE_VAR_UNLOCK();
        DBG_VERBOSE("<== %s(%d) %d\n", __func__, if_index, *ref_enabled);
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv4_enabled(
    unsigned int if_index,
    bool enabled)
{
    DBG_VERBOSE("==>%s(%d) -> %d\n", __func__, if_index, enabled);
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        DBG_ERR("%s(%d) NO intf\n", __func__, if_index);
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_enabled == enabled)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    //DBG_VERBOSE("%s(%d) (offset %d) enabled %d\n", __func__, if_index, iface->local_idx, enabled);
    g_net_iface_perm_vars[iface->local_idx].ipv4_enabled = enabled; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_config_options(
    unsigned int if_index,
    net_iface_ipv4_config_options_t *options)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != options );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *options = g_net_iface_perm_vars[iface->local_idx].ipv4_config_options;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv4_config_options(
    unsigned int if_index,
    const net_iface_ipv4_config_options_t *options)
{
    DBG_VERBOSE("==>%s(%d) %d\n", __func__, if_index, options? *options: -1);
    ASSERT( NULL != options );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_config_options == *options)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_config_options = *options; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_configured(
    unsigned int if_index,
    bool *configured)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != configured );
    *configured = false; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    net_iface_context_t *net_ctxt;
    net_ctxt = net_lock_ctxt();

    if(iface->ipv4_cfg_state == IPV4_CONFIGURED)
    {
        *configured = true;
    }

    net_unlock_ctxt(net_ctxt);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_last_config_method(
    unsigned int if_index,
    net_iface_ipv4_config_method_t *method)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != method );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *method = g_net_iface_perm_vars[iface->local_idx].ipv4_last_config_method;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

// *** internal only ***
net_iface_rcode_t net_set_ipv4_last_config(
    unsigned int if_index,
    const net_iface_ipv4_config_method_t *method)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != method );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_last_config_method == *method)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_last_config_method = *method; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_boot_server(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ttUserBtEntryPtr boot_entry;
    //unused uint8_t multihome_idx = 0;

    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if(iface->ipv4_cfg_state == IPV4_CONFIGURED)
    {
        net_iface_ipv4_config_method_t config_method;
        net_iface_get_var_ipv4_last_config_method(if_index, &config_method);
        switch(config_method)
        {
        case NET_IFACE_IPV4_CONFIGURED_DHCP:
            boot_entry = tfDhcpUserGetBootEntry(iface->treck_interface, multihome_idx);
            if(boot_entry)
            {
                *ipv4_addr = boot_entry->btuDhcpServerId;
            }
            break;

        case NET_IFACE_IPV4_CONFIGURED_BOOTP:
            boot_entry = tfBootpUserGetBootEntry(iface->treck_interface, multihome_idx);
            if(boot_entry)
            {
                *ipv4_addr = boot_entry->btuBootSIpAddress;
            }
            break;

        default:
            // other config methods not applicable
            break;    
        }
    }
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_dhcpv4_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ttUserBtEntryPtr boot_entry = NULL;
    //unused uint8_t multihome_idx = 0;
    
    // verify that the value defined in the vars API matches value defined by Treck
    ASSERT(NET_IFACE_DOMAINNAME_SIZE == TM_BOOTSNAME_SIZE);
    ASSERT( NULL != domain );

    if(!len)
    { 
        return NET_IFACE_OK;
    }
    domain[0] = '\0'; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if(iface->ipv4_cfg_state == IPV4_CONFIGURED)
    {
        net_iface_ipv4_config_method_t config_method = NET_IFACE_IPV4_CONFIGURED_DHCP;
        net_iface_get_var_ipv4_last_config_method(if_index, &config_method);
        if(config_method == NET_IFACE_IPV4_CONFIGURED_DHCP)
        {
            boot_entry = tfDhcpUserGetBootEntry(iface->treck_interface, multihome_idx);
        }
    }
    // bug 15096 - verify length and valid chars. NOTE: btuNamesLengths[TM_BT_DOMAIN_INDEX] includes terminating NULL
    if(boot_entry && boot_entry->btuNamesLengths[TM_BT_DOMAIN_INDEX] &&
       validate_domain_chars((char *)boot_entry->btuDomainName) == 0)
    {
        strncpy(domain, (char *)boot_entry->btuDomainName, len);
        domain[len-1] = '\0';
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_dhcpv4_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ttUserBtEntryPtr boot_entry = NULL;
    //unused uint8_t multihome_idx = 0;

    if(!len)
    { 
        return NET_IFACE_OK;
    }
    ASSERT( NULL != hostname );
    hostname[0] = '\0'; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if(iface->ipv4_cfg_state == IPV4_CONFIGURED)
    {
        net_iface_ipv4_config_method_t config_method = NET_IFACE_IPV4_CONFIGURED_DHCP;
        net_iface_get_var_ipv4_last_config_method(if_index, &config_method);
        if(config_method == NET_IFACE_IPV4_CONFIGURED_DHCP)
        {
            boot_entry = tfDhcpUserGetBootEntry(iface->treck_interface, multihome_idx);
        }
    }
    // bug 15096 - verify length and valid chars. NOTE: btuDhcpRxHostNameLength includes terminating NULL
    if(boot_entry && boot_entry->btuDhcpRxHostNameLength &&
       validate_hostname_chars((char *)boot_entry->btuDhcpRxHostNamePtr) == 0)
    {
        strncpy(hostname, (char *)boot_entry->btuDhcpRxHostNamePtr, len);
        hostname[len-1] = '\0';
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_dhcpv4_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ttUserBtEntryPtr boot_entry = NULL;
    //unused uint8_t multihome_idx = 0;
    net_iface_rcode_t ret = NET_IFACE_FAIL;
    net_iface_ipv4_config_method_t config_method = NET_IFACE_IPV4_CONFIGURED_DHCP;

    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    if( dns_index > 1 )
    {
        return NET_IFACE_BADPARAM;
    }

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if( iface->ipv4_cfg_state == IPV4_CONFIGURED )
    {
        net_iface_get_var_ipv4_last_config_method( if_index, &config_method );
        if( config_method == NET_IFACE_IPV4_CONFIGURED_DHCP )
        {
            boot_entry = tfDhcpUserGetBootEntry( iface->treck_interface, multihome_idx );
            // Success if we have at least one non-zero address
            if( ( NULL != boot_entry ) && 
                ( ( 0 != boot_entry->btuDns1ServerIpAddress ) ||
                  ( 0 != boot_entry->btuDns2ServerIpAddress ) ) )
            {
                if ( dns_index == 0 )
                {
                    *ipv4_addr = boot_entry->btuDns1ServerIpAddress;
                }
                else
                {
                    *ipv4_addr = boot_entry->btuDns2ServerIpAddress;
                }
                ret = NET_IFACE_OK;
            }
        }
    }
    return ret;
}

#ifdef TM_USE_PPPOE_CLIENT
net_iface_rcode_t net_iface_get_var_ipcp_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t ret = NET_IFACE_FAIL;
    net_iface_ipv4_config_method_t config_method;

    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    if( dns_index > 1 )
    {
        return NET_IFACE_BADPARAM;
    }

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if( iface->ipv4_cfg_state == IPV4_CONFIGURED )
    {
        net_iface_get_var_ipv4_last_config_method( if_index, &config_method );
        if( config_method == NET_IFACE_IPV4_CONFIGURED_IPCP )
        {
            int treck_retval;

            treck_retval = tfGetPppDnsIpAddress(iface->treck_interface, ipv4_addr, TM_DNS_PRIMARY + dns_index);
            if (treck_retval == TM_ENOERROR)
            {
                ret = NET_IFACE_OK;
            }
        }
    }
    return ret;
}
#endif // TM_USE_PPPOE_CLIENT

net_iface_rcode_t net_iface_get_var_manual_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    if( dns_index > 1 )
    {
        return NET_IFACE_BADPARAM;
    }

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if( dns_index == 0 )
    {
        *ipv4_addr = g_net_iface_perm_vars[iface->local_idx].ipv4_manual_dns_preferred_addr;
    }
    else
    {
        *ipv4_addr = g_net_iface_perm_vars[iface->local_idx].ipv4_manual_dns_alternate_addr;
    }
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_manual_dns_addr(
    unsigned int if_index,
    uint8_t dns_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );

    if( dns_index > 1 )
    {
        return NET_IFACE_BADPARAM;
    }

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if ( ( ( dns_index == 0 ) && ( g_net_iface_perm_vars[iface->local_idx].ipv4_manual_dns_preferred_addr == *ipv4_addr ) ) ||
         ( ( dns_index == 1 ) && ( g_net_iface_perm_vars[iface->local_idx].ipv4_manual_dns_alternate_addr == *ipv4_addr ) ) )
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    if( dns_index == 0 )
    {
        g_net_iface_perm_vars[iface->local_idx].ipv4_manual_dns_preferred_addr = *ipv4_addr; 
    }
    else
    {
        g_net_iface_perm_vars[iface->local_idx].ipv4_manual_dns_alternate_addr = *ipv4_addr; 
    }
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;

}

net_iface_rcode_t  net_iface_get_var_dns_server_origin(
    unsigned int if_index,
    net_iface_config_origin_t * origin)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
    net_iface_ipv4_config_options_t flags = 0;

    ASSERT( NULL != origin );
    
    net_iface_rcode = net_iface_get_var_ipv4_config_options(if_index, &flags);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }
    
    /* bitwise: manual overrides other origins */
    if(flags & NET_IFACE_IPV4_USE_MANUAL)
    {
        *origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
    }
    else 
    {
        *origin = NET_IFACE_CONFIG_ORIGIN_DHCP_V4;
    }
    
done:
    return net_iface_rcode;
}

net_iface_rcode_t net_iface_get_var_dns_server_addr(
    unsigned int if_index,
    uint8_t dns_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
    net_iface_ipv4_config_options_t flags = 0;

    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0;

    if( dns_index > 1 )
    {
        // Currently only support 2 dns addresses
        return NET_IFACE_BADPARAM;
    }

    net_iface_rcode = net_iface_get_var_ipv4_config_options(if_index, &flags);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        goto done;
    }

    /* bitwise: manual overrides other origins */
    if(flags & NET_IFACE_IPV4_USE_MANUAL)
    {
        net_iface_rcode = net_iface_get_var_manual_dns_addr(if_index, dns_index, ipv4_addr);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
    }
    #ifdef TM_USE_PPPOE_CLIENT
    else if (flags & NET_IFACE_IPV4_USE_IPCP)
    {
        net_iface_rcode = net_iface_get_var_ipcp_dns_addr(if_index, dns_index, ipv4_addr);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
    }
    #endif // TM_USE_PPPOE_CLIENT
    else
    {
        net_iface_rcode = net_iface_get_var_dhcpv4_dns_addr(if_index, dns_index, ipv4_addr);
        if(net_iface_rcode != NET_IFACE_OK)
        {
            goto done;
        }
    }
        
done:        
    return net_iface_rcode;
}

tt32Bit /*uint32_t*/ tvTime; // dhcp lease time?
net_iface_rcode_t net_iface_get_var_dhcpv4_lease_remaining(
    unsigned int if_index,
    uint32_t *lease_remaining)
{
    DBG_VERBOSE("==>%s\n", __func__);
    extern tt32Bit tvTime;
    ASSERT( NULL != lease_remaining );
    *lease_remaining = 0;

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    net_iface_context_t *net_ctxt;
    net_ctxt = net_lock_ctxt();

    if(iface->ipv4_dhcp_lease_time == -1)
    {
        *lease_remaining = -1;
        net_unlock_ctxt(net_ctxt);
        return NET_IFACE_OK;
    }

    uint32_t elapsed_time = tvTime - iface->ipv4_dhcp_lease_start;
    if( iface->ipv4_dhcp_lease_time && (elapsed_time < iface->ipv4_dhcp_lease_time) )
    {
        *lease_remaining = (iface->ipv4_dhcp_lease_time - elapsed_time)/1000;
    }

    net_unlock_ctxt(net_ctxt);
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    int treck_rcode;
    //unused uint8_t multihome_idx = 0; // currently we only allow one addr per ipv4 if_index, so just default to 0
    
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    // If manually configured then just return the manual value
    net_iface_ipv4_config_options_t cfg_flags = NET_IFACE_IPV4_USE_DHCP;
    net_iface_get_var_ipv4_config_options(if_index, &cfg_flags);
    if(cfg_flags & NET_IFACE_IPV4_USE_MANUAL)
    {
        net_iface_get_var_ipv4_manual_addr(if_index, ipv4_addr);
        return NET_IFACE_OK;
    }

    if(iface->ipv4_cfg_state != IPV4_CONFIGURED)
    {
        // no configured IP addr, so just return 0.0.0.0
        return NET_IFACE_OK;
    }

    treck_rcode = tfGetIpAddress(iface->treck_interface, (ttUserIpAddress *)ipv4_addr, multihome_idx);
    if(treck_rcode != TM_ENOERROR)
    {
        // error retrieving IP address
        return NET_IFACE_FAIL;
    }
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_subnet_mask(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    int treck_rcode;
    //unused uint8_t multihome_idx = 0; // currently we only allow one addr per ipv4 if_index, so just default to 0
    
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    // If manually configured then just return the manual value
    net_iface_ipv4_config_options_t cfg_flags = NET_IFACE_IPV4_USE_MANUAL;
    net_iface_get_var_ipv4_config_options(if_index, &cfg_flags);
    if(cfg_flags & NET_IFACE_IPV4_USE_MANUAL)
    {
        net_iface_get_var_ipv4_manual_subnet_mask(if_index, ipv4_addr);
        return NET_IFACE_OK;
    }

    if(iface->ipv4_cfg_state != IPV4_CONFIGURED)
    {
        // no configured IP addr, so just return 0.0.0.0
        return NET_IFACE_OK;
    }

    treck_rcode = tfGetNetMask(iface->treck_interface, (ttUserIpAddress *)ipv4_addr, multihome_idx);
    if(treck_rcode != TM_ENOERROR)
    {
        // error retrieving net mask
        return NET_IFACE_FAIL;
    }
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_gateway(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    int treck_rcode;
    
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    // If manually configured then just return the manual value
    net_iface_ipv4_config_options_t cfg_flags = NET_IFACE_IPV4_USE_DHCP;
    net_iface_get_var_ipv4_config_options(if_index, &cfg_flags);
    if(cfg_flags & NET_IFACE_IPV4_USE_MANUAL)
    {
        net_iface_get_var_ipv4_manual_gateway(if_index, ipv4_addr);
        return NET_IFACE_OK;
    }

    if(iface->ipv4_cfg_state != IPV4_CONFIGURED)
    {
        // no configured IP addr, so just return 0.0.0.0
        return NET_IFACE_OK;
    }

#ifdef TM_USE_STRONG_ESL
    treck_rcode = tfGetDefaultGateway(iface->treck_interface, (ttUserIpAddress *)ipv4_addr);
#else
    treck_rcode = tfGetDefaultGateway((ttUserIpAddress *)ipv4_addr);
#endif
    if(treck_rcode != TM_ENOERROR)
    {
        // error retrieving net mask
        return NET_IFACE_FAIL;
    }
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_wins_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    // If manually configured then just return the manual value
    net_iface_ipv4_config_options_t cfg_flags = NET_IFACE_IPV4_USE_DHCP;
    net_iface_get_var_ipv4_config_options(if_index, &cfg_flags);
    if(cfg_flags & NET_IFACE_IPV4_USE_MANUAL)
    {
        net_iface_get_var_ipv4_manual_wins_addr(if_index, ipv4_addr);
        return NET_IFACE_OK;
    }

    // If auto-configured then addr depends on boot method.  Otherwise just return zeroed addr.
    if(iface->ipv4_cfg_state == IPV4_CONFIGURED)
    {
        net_iface_ipv4_config_method_t method = NET_IFACE_IPV4_CONFIGURED_DHCP;
        net_iface_get_var_ipv4_last_config_method(if_index, &method);
        switch(method)
        {
        case NET_IFACE_IPV4_CONFIGURED_DHCP:
            // return the auto-configured value (ipv4 wins addr can be auto-configured only via DHCP)
            {
                //unused uint8_t multihome_idx = 0; // currently we only allow one addr per ipv4 if_index, so just default to 0
                ttUserBtEntryPtr boot_entry;
                boot_entry = tfDhcpUserGetBootEntry(iface->treck_interface, multihome_idx);
                if(boot_entry && boot_entry->btuNetBiosNumNameServers)
                {
                    *ipv4_addr = boot_entry->btuNetBiosNameServers[0];
                }
            }
            break;
    
        case NET_IFACE_IPV4_CONFIGURED_BOOTP:
            // return the manual value (ipv4 wins addr can be auto-configured only via DHCP)
            net_iface_get_var_ipv4_manual_wins_addr(if_index, ipv4_addr);
            break;
    
        case NET_IFACE_IPV4_CONFIGURED_AUTOIP:
            /*
             *  Zero the WINS addr.  Doing so causes printer to bypass the NBNS registration that might
             *  otherwise generate numerous ARP retries while attempting to contact a server not on the local
             *  link. Any such spurious ARPs can cause Bonjour Conformance Test ver 1.2.3 to fail in Phase I:
             *  "Link-Local Address Allocation" at step I.5 "Subsequent Conflict" - bug 11312.
             */
            break;

        case NET_IFACE_IPV4_CONFIGURED_IPCP:
            /*
             *  Zero the WINS addr.  Doing so causes printer to bypass the NBNS registration that might
             *  otherwise generate numerous ARP retries while attempting to contact a server not on the local
             *  link. Any such spurious ARPs can cause Bonjour Conformance Test ver 1.2.3 to fail in Phase I:
             *  "Link-Local Address Allocation" at step I.5 "Subsequent Conflict" - bug 11312.
             */
            break;

        default:
            // unexpected IPv4 boot method
            XASSERT(0, method);
        }
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_manual_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *ipv4_addr = g_net_iface_perm_vars[iface->local_idx].ipv4_manual_addr;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv4_manual_addr(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
    ASSERT( NULL != ipv4_addr );

    NET_IFACE_VAR_LOCK();
    uint32_t cur_ipv4_addr;
    net_iface_rcode = net_iface_get_var_ipv4_manual_addr(if_index, &cur_ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    if(cur_ipv4_addr == *ipv4_addr)
    {
        // no change - ignore the request
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    net_iface_rcode = net_set_ipv4_manual_addr(if_index, ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

done:
    return net_iface_rcode;
}

// *** internal only ***
// updates setting without triggering net reset
net_iface_rcode_t net_set_ipv4_manual_addr(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_manual_addr == *ipv4_addr)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_manual_addr = *ipv4_addr; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_manual_subnet_mask(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *ipv4_addr = g_net_iface_perm_vars[iface->local_idx].ipv4_manual_subnet_mask;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv4_manual_subnet_mask(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
    ASSERT( NULL != ipv4_addr );

    NET_IFACE_VAR_LOCK();
    uint32_t cur_ipv4_addr;
    net_iface_rcode = net_iface_get_var_ipv4_manual_subnet_mask(if_index, &cur_ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    if(cur_ipv4_addr == *ipv4_addr)
    {
        // no change - ignore the request
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    net_iface_rcode = net_set_ipv4_manual_subnet_mask(if_index, ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

done:
    return net_iface_rcode;
}

// *** internal only ***
// updates setting without triggering net reset
net_iface_rcode_t net_set_ipv4_manual_subnet_mask(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_manual_subnet_mask == *ipv4_addr)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_manual_subnet_mask = *ipv4_addr; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_manual_gateway(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *ipv4_addr = g_net_iface_perm_vars[iface->local_idx].ipv4_manual_gateway;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv4_manual_gateway(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;

    NET_IFACE_VAR_LOCK();
    uint32_t cur_ipv4_addr;
    net_iface_rcode = net_iface_get_var_ipv4_manual_gateway(if_index, &cur_ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    if(cur_ipv4_addr == *ipv4_addr)
    {
        // no change - ignore the request
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    net_iface_rcode = net_set_ipv4_manual_gateway(if_index, ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

done:
    return net_iface_rcode;
}

// *** internal only ***
// updates setting without triggering net reset
net_iface_rcode_t net_set_ipv4_manual_gateway(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_manual_gateway == *ipv4_addr)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_manual_gateway = *ipv4_addr; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv4_manual_wins_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *ipv4_addr = g_net_iface_perm_vars[iface->local_idx].ipv4_manual_wins_addr;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv4_manual_wins_addr(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_iface_rcode_t net_iface_rcode = NET_IFACE_OK;
    ASSERT( NULL != ipv4_addr );

    NET_IFACE_VAR_LOCK();
    uint32_t cur_ipv4_addr;
    net_iface_rcode = net_iface_get_var_ipv4_manual_wins_addr(if_index, &cur_ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }
    if(cur_ipv4_addr == *ipv4_addr)
    {
        // no change - ignore the request
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    net_iface_rcode = net_set_ipv4_manual_wins_addr(if_index, ipv4_addr);
    if(net_iface_rcode != NET_IFACE_OK)
    {
        NET_IFACE_VAR_UNLOCK();
        goto done;
    }

    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

done:
    return net_iface_rcode;
}

// *** internal only ***
// updates setting without triggering net reset
net_iface_rcode_t net_set_ipv4_manual_wins_addr(
    unsigned int if_index,
    const uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_manual_wins_addr == *ipv4_addr)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_manual_wins_addr = *ipv4_addr; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

// *** internal only ***
net_iface_rcode_t net_get_ipv4_autoip_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );
    *ipv4_addr = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *ipv4_addr = g_net_iface_perm_vars[iface->local_idx].ipv4_autoip_addr;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

// *** internal only ***
net_iface_rcode_t net_set_ipv4_autoip_addr(
    unsigned int if_index,
    uint32_t *ipv4_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv4_addr );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv4_autoip_addr == *ipv4_addr)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_autoip_addr = *ipv4_addr; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}


//=========================================================================================================================
// DATASTORE: IPv6 configuration variables
//=========================================================================================================================

#ifdef HAVE_IPV6
net_iface_rcode_t net_iface_get_var_ipv6_enabled(
    unsigned int if_index,
    bool *enabled)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != enabled );
    *enabled = false; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *enabled = g_net_iface_perm_vars[iface->local_idx].ipv6_enabled;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv6_enabled(
    unsigned int if_index,
    bool enabled)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv6_enabled == enabled)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv6_enabled = enabled; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_config_options(
    unsigned int if_index,
    net_iface_ipv6_config_options_t *options)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != options );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    *options = g_net_iface_perm_vars[iface->local_idx].ipv6_config_options;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv6_config_options(
    unsigned int if_index,
    const net_iface_ipv6_config_options_t *options)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != options );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].ipv6_config_options == *options)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv6_config_options = *options; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_local_configured(
    unsigned int if_index,
    bool *configured)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != configured );
    *configured = false; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if(iface->ipv6_cfg_state >= IPV6_LOCAL_CONFIGURED)
    {
        *configured = true;
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_configured(
    unsigned int if_index,
    bool *configured)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != configured );
    *configured = false; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if(iface->ipv6_cfg_state == IPV6_CONFIGURED)
    {
        *configured = true;
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_dhcpv6_domain(
    unsigned int if_index,
    char *domain,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != domain );
    // verify that the value defined in the vars API matches value defined by Treck
    ASSERT(NET_IFACE_DOMAINNAME_SIZE == TM_BOOTSNAME_SIZE);

    if(!len)
    { 
        return NET_IFACE_OK;
    }
    domain[0] = '\0'; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    tt6UserBtEntryPtr bt_entry;
    bt_entry = tf6DhcpGetBootEntry(iface->treck_interface, 
                                   0, // Identity Association (Treck currently supports only one per iface, index=0)
                                   0); // flags (currently unused by Treck)
    if(bt_entry)
    {
        /*
         *  bug 15553 - If the name returned via option 39 is fully qualified then use the domain name from
         *  option 39.  Otherwise, assume device is in the same domain as first DNS search suffix provided
         *  in the list from option 24.
         */

        if(bt_entry->btFqdnFlags & TM_6_DHCP_FQDN_SUPPORTED)
        {
            char *pos = strchr(bt_entry->btFqdnStr, '.');
            if(pos)
            {
                strncpy(domain, pos+1, len);
                domain[len-1] = '\0';
            }
        }

        if(domain[0] == '\0' && bt_entry->btDomainCnt)
        {
            strncpy(domain, bt_entry->btDomainList[0], len);
            domain[len-1] = '\0';
        }
        
        tf6DhcpFreeBootEntry(bt_entry, 0 /* flags (currently unused by Treck) */);
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_dhcpv6_hostname(
    unsigned int if_index,
    char *hostname,
    uint8_t len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    if(!len)
    { 
        return NET_IFACE_OK;
    }
    ASSERT( NULL != hostname );
    hostname[0] = '\0'; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    tt6UserBtEntryPtr bt_entry;
    bt_entry = tf6DhcpGetBootEntry(iface->treck_interface, 
                                   0, // Identity Association (Treck currently supports only one per iface, index=0)
                                   0); // flags (currently unused by Treck)
    if(bt_entry)
    {
        if(bt_entry->btFqdnFlags & TM_6_DHCP_FQDN_SUPPORTED)
        {
/*
            if(bt_entry->btFqdnFlags & TM_6_DHCP_FQDN_FULL)
            {
                // server gave us fully qualified but we only want partial
                char *pos = strchr(bt_entry->btFqdnStr, '.');
                if(pos)
                {
                    uint32_t num_chars = pos - bt_entry->btFqdnStr;
                    num_chars = (num_chars < len-1)?num_chars:(len-1);
                    strncpy(hostname, bt_entry->btFqdnStr, num_chars);
                    hostname[num_chars] = '\0';
                }
            }
            else // partial
            {
                strncpy(hostname, bt_entry->btFqdnStr, len);
                hostname[len-1] = '\0';
            }
*/
            /*  Windows server 2008 returns a malformed DNS name in which the '.'s are explicit. Treck then
             *  interprets the entire domain name as a single-label DNS and does not set the TM_6_DHCP_FQDN_FULL
             *  flag.  Fix requires us to just ignore the TM_6_DHCP_FQDN_FULL flag altogether.
             */
            char *pos = strchr(bt_entry->btFqdnStr, '.');
            if(pos)
            {
                uint32_t num_chars = pos - bt_entry->btFqdnStr;
                num_chars = (num_chars < len-1)?num_chars:(len-1);
                strncpy(hostname, bt_entry->btFqdnStr, num_chars);
                hostname[num_chars] = '\0';
            }
            else // partial
            {
                strncpy(hostname, bt_entry->btFqdnStr, len);
                hostname[len-1] = '\0';
            }
        }

        tf6DhcpFreeBootEntry(bt_entry, 0 /* flags (currently unused by Treck) */); 
    }

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_link_local_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr,
    net_iface_ipv6_addr_hndl_t *addr_hndl)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv6_addr );
    memset(ipv6_addr, 0, sizeof(net_iface_ipv6_addr_t)); // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_LOCK_IFACE_CONTEXT(iface);

    if(addr_hndl)
    {
        *addr_hndl = (net_iface_ipv6_addr_hndl_t)NULL;
    }

    internal_ipv6_addr_t *tmp_addr = iface->addr_map[IPV6_LINKLOCAL_MULTIHOME_IDX];
    if(!tmp_addr)
    {
        // no link-local IPv6 addr has been auto-configured
        NET_UNLOCK_IFACE_CONTEXT(iface);
        return NET_IFACE_OK;
    }

    memcpy(&ipv6_addr->addr, &tmp_addr->ipv6_addr.addr, sizeof(ipv6_addr->addr));
    ipv6_addr->prefix = tmp_addr->ipv6_addr.prefix;
    if(addr_hndl)
    {
        *addr_hndl = (net_iface_ipv6_addr_hndl_t)tmp_addr;
    }

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_preferred_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr,
    net_iface_ipv6_addr_hndl_t *addr_hndl)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv6_addr );
    memset(ipv6_addr, 0, sizeof(net_iface_ipv6_addr_t)); // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_LOCK_IFACE_CONTEXT(iface);

    if(addr_hndl)
    {
        *addr_hndl = (net_iface_ipv6_addr_hndl_t)NULL;
    }

    internal_ipv6_addr_t *tmp_addr = iface->ipv6_preferred_addr;
    if(!tmp_addr)
    {
        // no global IPv6 addrs have been configured
        NET_UNLOCK_IFACE_CONTEXT(iface);
        return NET_IFACE_OK;
    }

    memcpy(&ipv6_addr->addr, &tmp_addr->ipv6_addr.addr, sizeof(ipv6_addr->addr));
    ipv6_addr->prefix = tmp_addr->ipv6_addr.prefix;
    if(addr_hndl)
    {
        *addr_hndl = (net_iface_ipv6_addr_hndl_t)tmp_addr;
    }

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_next_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr,
    net_iface_ipv6_addr_hndl_t *addr_hndl,
    net_iface_ipv6_addr_hndl_t prev_addr_hndl)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT(ipv6_addr);
    ASSERT(addr_hndl);

    memset(ipv6_addr, 0, sizeof(net_iface_ipv6_addr_t)); // default
    *addr_hndl = (net_iface_ipv6_addr_hndl_t)NULL; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_LOCK_IFACE_CONTEXT(iface);

    if((uint32_t *)prev_addr_hndl == NULL ||
       *((uint32_t *)prev_addr_hndl) != NET_IFACE_IPV6_INTERNAL_ADDR_SIGNATURE)
    {
        // prev_addr_hndl is invalid or has expired (i.e. addr no longer exists)
        NET_UNLOCK_IFACE_CONTEXT(iface);
        return NET_IFACE_FAIL;
    }

    internal_ipv6_addr_t *tmp_addr = ((internal_ipv6_addr_t *)prev_addr_hndl)->next;
    if(!tmp_addr)
    {
        // end of global IPv6 addr list
        NET_UNLOCK_IFACE_CONTEXT(iface);
        return NET_IFACE_OK;
    }

    ASSERT(tmp_addr->iface == iface);

    memcpy(&ipv6_addr->addr, &tmp_addr->ipv6_addr.addr, sizeof(ipv6_addr->addr));
    ipv6_addr->prefix = tmp_addr->ipv6_addr.prefix;
    *addr_hndl = (net_iface_ipv6_addr_hndl_t)tmp_addr;
    
    NET_UNLOCK_IFACE_CONTEXT(iface);
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_addr_info(
    unsigned int if_index,
    net_iface_ipv6_addr_info_t *addr_info,
    net_iface_ipv6_addr_hndl_t addr_hndl)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != addr_info );
    memset(addr_info, 0, sizeof(net_iface_ipv6_addr_info_t)); // default
  
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_LOCK_IFACE_CONTEXT(iface);

    if((uint32_t *)addr_hndl == NULL ||
       *((uint32_t *)addr_hndl) != NET_IFACE_IPV6_INTERNAL_ADDR_SIGNATURE)
    {
        // prev_addr_hndl is invalid or has expired (i.e. addr no longer exists)
        NET_UNLOCK_IFACE_CONTEXT(iface);
        return NET_IFACE_FAIL;
    }

    internal_ipv6_addr_t *tmp_addr = (internal_ipv6_addr_t *)addr_hndl;
    ASSERT(tmp_addr->iface == iface);

    unsigned long cur_sys_ticks = tx_time_get();

    addr_info->config_method = tmp_addr->config_method;
    addr_info->preferred_lifetime_remaining = NET_IPV6_PREFERRED_LIFETIME_REMAINING(cur_sys_ticks, tmp_addr); 
    addr_info->valid_lifetime_remaining = NET_IPV6_VALID_LIFETIME_REMAINING(cur_sys_ticks, tmp_addr);

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_ipv6_manual_addr(
    unsigned int if_index,
    net_iface_ipv6_addr_t *ipv6_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv6_addr );
    memset(ipv6_addr, 0, sizeof(net_iface_ipv6_addr_t)); // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    memcpy(&ipv6_addr->addr, &g_net_iface_perm_vars[iface->local_idx].ipv6_manual_addr.addr, sizeof(ipv6_addr->addr));
    ipv6_addr->prefix = g_net_iface_perm_vars[iface->local_idx].ipv6_manual_addr.prefix;
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_ipv6_manual_addr(
    unsigned int if_index,
    const net_iface_ipv6_addr_t *ipv6_addr)
{
    DBG_VERBOSE("==>%s\n", __func__);
    ASSERT( NULL != ipv6_addr );

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(NET_IFACE_IPV6_ADDRS_EQUAL(g_net_iface_perm_vars[iface->local_idx].ipv6_manual_addr, *ipv6_addr))
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    memcpy(&g_net_iface_perm_vars[iface->local_idx].ipv6_manual_addr.addr, &ipv6_addr->addr, 
           sizeof(g_net_iface_perm_vars[iface->local_idx].ipv6_manual_addr.addr));
    g_net_iface_perm_vars[iface->local_idx].ipv6_manual_addr.prefix = ipv6_addr->prefix;
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}
#endif // HAVE_IPV6

#ifdef TM_USE_PPPOE_CLIENT
error_type_t net_get_pppoe_enabled(unsigned int local_idx, bool *enabled)
{
    NET_IFACE_VAR_LOCK();
    *enabled = g_net_iface_perm_vars[local_idx].pppoe_enabled;
    NET_IFACE_VAR_UNLOCK();
    return OK;
}


net_iface_rcode_t net_iface_get_var_pppoe_enabled(
    unsigned int  if_index,
    bool         *enabled)
{
    DBG_VERBOSE("==>%s\n", __func__);
    if ( NULL == enabled )
    {
        return NET_IFACE_BADPARAM;
    }
    *enabled = false; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    net_get_pppoe_enabled(iface->local_idx, enabled);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_pppoe_enabled(
    unsigned int if_index,
    bool         enabled)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    if(g_net_iface_perm_vars[iface->local_idx].pppoe_enabled == enabled)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    g_net_iface_perm_vars[iface->local_idx].ipv4_config_options &= ~NET_IFACE_IPV4_USE_IPCP;
    if (enabled)
    {
        g_net_iface_perm_vars[iface->local_idx].ipv4_config_options |= NET_IFACE_IPV4_USE_IPCP;
    }

    g_net_iface_perm_vars[iface->local_idx].pppoe_enabled = enabled; 
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_pppoe_state(
    unsigned int if_index,
    uint32_t *pppoe_state)
{
    DBG_VERBOSE("==>%s\n", __func__);
    if ( NULL == pppoe_state )
    {
        return NET_IFACE_BADPARAM;
    }
    *pppoe_state = 0; // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    *pppoe_state = iface->pppoe_steps_completed;
       
    return NET_IFACE_OK;
}


net_iface_rcode_t net_iface_get_var_pppoe_username(
    unsigned int  if_index,
    char         *username,
    uint16_t      buffer_len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    if ( (NULL == username) || (buffer_len <= 1) )
    {
        return NET_IFACE_BADPARAM;
    }
    strcpy(username, ""); // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    strncpy(username, g_net_iface_perm_vars[iface->local_idx].pppoe_username, buffer_len);
    username[buffer_len - 1] = '\0';
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_pppoe_username(
    unsigned int  if_index,
    char         *username)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if (NULL == username)
    {
        username = "";
    }

    NET_IFACE_VAR_LOCK();
    if(strcmp(g_net_iface_perm_vars[iface->local_idx].pppoe_username, username) == 0)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    strncpy(g_net_iface_perm_vars[iface->local_idx].pppoe_username, username, PPPOE_USERNAME_MAX_LENGTH + 1);
    g_net_iface_perm_vars[iface->local_idx].pppoe_username[PPPOE_USERNAME_MAX_LENGTH] = '\0';
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_get_var_pppoe_password(
    unsigned int  if_index,
    char         *password,
    uint16_t      buffer_len)
{
    DBG_VERBOSE("==>%s\n", __func__);
    if ( (NULL == password) || (buffer_len <= 1) )
    {
        return NET_IFACE_BADPARAM;
    }
    strcpy(password, ""); // default

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    NET_IFACE_VAR_LOCK();
    strncpy(password, g_net_iface_perm_vars[iface->local_idx].pppoe_password, buffer_len);
    password[buffer_len - 1] = '\0';
    NET_IFACE_VAR_UNLOCK();

    return NET_IFACE_OK;
}

net_iface_rcode_t net_iface_set_var_pppoe_password(
    unsigned int  if_index,
    char         *password)
{
    DBG_VERBOSE("==>%s\n", __func__);
    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        return NET_IFACE_BADINDEX;
    }

    if (NULL == password)
    {
        password = "";
    }

    NET_IFACE_VAR_LOCK();
    if(strcmp(g_net_iface_perm_vars[iface->local_idx].pppoe_password, password) == 0)
    {
        // no change - ignore request
        NET_IFACE_VAR_UNLOCK();
        return NET_IFACE_OK;
    }

    strncpy(g_net_iface_perm_vars[iface->local_idx].pppoe_password, password, PPPOE_PASSWORD_MAX_LENGTH + 1);
    g_net_iface_perm_vars[iface->local_idx].pppoe_password[PPPOE_PASSWORD_MAX_LENGTH] = '\0';
    net_iface_nvram_block_write(if_index);
    NET_IFACE_VAR_UNLOCK();

    net_config_change_notify(if_index);

    return NET_IFACE_OK;
}
#endif // TM_USE_PPPOE_CLIENT

//=========================================================================================================================
// Utility routines
//=========================================================================================================================

// need to check and see if this is a valid domain name. 
// all chars and ints and underscores, no spaces allowed
static int validate_hostname_chars(const char *Test)
{
    ASSERT( NULL != Test );
    int Bad = FALSE;
    if( (*Test == '_') || ( *Test == '-' ) )
    {
        Bad = TRUE;
    }
    
    while( ( *Test != '\0' ) && ( Bad == FALSE ) )
    {
        if( !( ( *Test >= '0' && *Test <= '9' ) || 
               ( *Test >= 'a' && *Test <= 'z' ) ||
               ( *Test >= 'A' && *Test <= 'Z' ) || 
               ( *Test == '_' ) || 
               ( *Test == '-' ) ) )
        {
            Bad = TRUE;
            break;
        }
        // last character cannot be a '-' or a '_'
        if ( ( *( Test + 1 ) == '\0' ) &&
             ( ( *Test == '_' ) ||
               ( *Test == '-' ) ) )
        {
            Bad = TRUE;
            break;
        }
        Test++;
    } /*
       * End of while
       */
    return Bad;
}

// same as hostname validation except also allows '.' char
static int validate_domain_chars(const char *Test)
{
    ASSERT( NULL != Test );
    int Bad = FALSE;
    if( (*Test == '_') || ( *Test == '-' )  || ( *Test == '.' ))
    {
        Bad = TRUE;
    }
    
    while( ( *Test != '\0' ) && ( Bad == FALSE ) )
    {
        if( !( ( *Test >= '0' && *Test <= '9' ) || 
               ( *Test >= 'a' && *Test <= 'z' ) ||
               ( *Test >= 'A' && *Test <= 'Z' ) || 
               ( *Test == '_' ) || 
               ( *Test == '.' ) ||
               ( *Test == '-' ) ) )
        {
            Bad = TRUE;
            break;
        }
        // last character cannot be a '-' or a '_'
        if ( ( *( Test + 1 ) == '\0' ) &&
             ( ( *Test == '_' ) ||
               ( *Test == '-' ) ) )
        {
            Bad = TRUE;
            break;
        }
        Test++;
    } /*
       * End of while
       */
    return Bad;
}

// variation of official base-64 decode mapping
// starts with 0-9 vs. A-Z
//static uint8_t net_base64_char_to_uint8(char c)
//{
//    if (c >= '0' && c <= 9) return (c - 48);
//    if (c >= 'A' && c <= 'Z') return (c - 55);
//    if (c >= 'a' && c <= 'z') return (c - 61);
//    if (c == '/') return 63;
//    if (c == '+') return 62;
//    return -1; // invalid character
//}

// variation of official base-64 encode mapping
// starts encoding at 0-9 vs. A-Z
char net_base64_uint8_to_char(uint8_t i)
{
    if(i < 10) return (i + 48); // 0-9
    if(i < 36) return (i + 55); // A-Z
    if(i < 62) return (i + 61); // a-z
    if(i == 62) return '+';
    if(i == 63) return '/';
    return '\0'; // value out of range
}

// Startup init routines

void init_network_vars(void) 
{

    net_iface_vars_init();

return;
}

// WEAK declarations subject to oem-override

WEAK net_interface_t* get_current_active_intf(void)
{
    int intf_index = NET_IFACE_LINK_TYPE_ETH; // wired default
    intf_mapp = if_nameindex(); // refresh current os-interface bindings

    int   if_index = intf_mapp[intf_index].if_index;
#ifdef DBG_VERBOSE_ON
    char* if_name  = intf_mapp[intf_index].if_name; UNUSED_VAR(if_name);
    DBG_VERBOSE("%s() intf %d->%d '%s'\n", __func__, intf_index, if_index, if_name);
#endif
    return net_iface_if_index_to_local_context(if_index);
}

net_iface_context_t* net_lock_ctxt()
{
#ifdef DBG_VERBOSE_ON
    net_interface_t* intf = get_current_active_intf(); UNUSED_VAR(intf);
    char* intf_name = (intf ? intf->name : "<inactive intf>"); UNUSED_VAR(intf_name);
    DBG_VERBOSE("%s() ret context x%x %s\n", __func__, g_net_context, intf_name);
#endif
    return g_net_context;
}

net_iface_context_t* net_get_ctxt()
{
#ifdef DBG_VERBOSE_ON
    net_interface_t* intf = get_current_active_intf(); UNUSED_VAR(intf);
    char* intf_name = (intf ? intf->name : "<inactive intf>"); UNUSED_VAR(intf_name);
    DBG_VERBOSE("%s() ret context x%x %s\n", __func__, g_net_context, intf_name);
#endif
    return g_net_context;
}

void net_unlock_ctxt(net_iface_context_t* context)
{
    XASSERT(context == g_net_context, (long unsigned int) context);
    return; // default &g_interface_context
}

static void net_reset_timer_func(void *user_data)
{
    ASSERT(user_data);
    net_interface_t *iface = (net_interface_t *)user_data;

    //DBG_VERBOSE("%s(x%x) '%s'\n", __func__, iface, iface->name);

    // assume timer callback (i.e. a non-thread)
    int32_t net_iface_rcode;
    net_iface_rcode = net_send_msg_no_wait(NET_IFACE_MSG_NET_RESET, iface, (void *)0, (void *)0);
    if(net_iface_rcode == NET_IFACE_WOULDBLOCK)
    {
        // msg queue full
        DBG_ERR("%s() queue full while sending NET_IFACE_MSG_NET_RESET on %s\n",
                iface->name);
        DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
    }
    return;
}

uint8_t gDataOnNet = false;
void net_config_change_notify(unsigned if_index)
{
    // rdj TODO - do we need to delay net reset when job data is on print pipe? look at gDataOnNet
    // previously used by iptask

    //DBG_VERBOSE("%s() if_index %d\n", __func__, if_index);

    net_interface_t *iface = net_iface_if_index_to_local_context(if_index);
    if(!iface)
    {
        // bad index
        DBG_ERR("%s: bad index (line %d, idx=%d)\n", __func__, __LINE__, if_index);
        DBG_ASSERT(0);
        return;
    }

    DBG_VERBOSE("%s(%d) %s state %s\n", __func__, if_index, iface->name, str_iface_state(iface->state));

    NET_LOCK_IFACE_CONTEXT(iface);

    if( (iface->state != NET_IFACE_STATE_OPENING_INTERFACE)
        && (iface->state != NET_IFACE_STATE_INTERFACE_OPEN) )
    {
        DBG_VERBOSE("%s(%d) %s state %s\n", __func__, if_index, iface->name, str_iface_state(iface->state));
        // ignore config changes made when interface not opening or opened
        NET_UNLOCK_IFACE_CONTEXT(iface);
        return;
    }

    // cancel any previous reset timer and restart the NET_RESET_DELAY_SEC countdown
    net_timer_cancel(iface->timer_hndl_reset);
    iface->timer_hndl_reset = net_timer_start_ms(
                                NET_TIMER_FLAG_REAL_TIME,
                                NET_RESET_DELAY_SEC*1000,
                                net_reset_timer_func,
                                (void *)iface);
    DBG_ASSERT(iface->timer_hndl_reset != NET_TIMER_INVALID_HANDLE);

    NET_UNLOCK_IFACE_CONTEXT(iface);
    return;
}

// general interface lookup (map if_index->interface*) -- see oem for any suitable replacement
net_interface_t* WEAK net_iface_if_index_to_local_context(unsigned if_index)
{
    net_interface_t* intf = NULL;
    char if_name_buf[IFNAMSIZ], *if_name = if_name_buf;
    if_name = if_indextoname(if_index, if_name);
    if (if_name != NULL)
    {
        intf = net_iface_lookup_by_name(if_name);
    }
    return intf;
}

// entrypoint required from oid_network_sm.c
net_rcode_t net_get_var_location(char *location, uint8_t len)
{
    if(!len)
    { 
        return NET_OK;
    }
    ASSERT( NULL != location );
    location[0] = '\0';

    // TODO pull from net_var storage

    return NET_OK;
}

#ifdef __cplusplus
}
#endif

// eof net_iface_vars.c
