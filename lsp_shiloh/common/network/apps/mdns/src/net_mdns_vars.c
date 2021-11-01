 /*
 * ============================================================================
 * Copyright (c) 2009-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <string.h>
#include <stdio.h>

// common headers
#include "memAPI.h"
#include "error_types.h"
#include "nvram_api.h"
#include "os_network.h" // treck stubs
#include "net_iface_api.h" 
#include "net_api.h"
#include "net_iface.h" // g_net_context
#include "net_mdns_vars.h"
#include "net_mdns_api.h" // DEFAULT_BONJOUR_SERVICE_NAME
#include "platform_api.h" // platvars_get_prtnamestr();

// local includes
#ifdef __linux__
#include "os_network.h"
#else
#include "mdns.h"
#include "mDNSPlatform.h"
#ifdef HAVE_LPD
#include "net_lpd_api.h"
#endif // HAVE_LPD
#endif

// values which reside in dynamic permanent storage
// IMPORTANT:  increment NET_MDNS_VARS_VERSION whenever this structure changes
#define NET_MDNS_VARS_VERSION 0
typedef struct
{                                   
    bool    mdns_enabled;
    char    service_name[NET_MDNS_SERVICE_NAME_SIZE];
    char    domain_name[NET_MDNS_DOMAIN_NAME_SIZE];

#ifdef HAVE_LPD
    bool    lpd_enabled;
#endif // HAVE_LPD

} net_mdns_perm_vars_t;


//--------------------------------------
// Local Variables
//--------------------------------------

static bool g_net_mdns_nvram_is_registered = false;
static pthread_mutex_t g_net_mdns_var_mutex;

static nvram_handle_t *g_net_mdns_nvram_hndl[NET_MAX_INTERFACES];
static net_mdns_perm_vars_t g_net_mdns_perm_vars[NET_MAX_INTERFACES];
static net_mdns_perm_vars_t g_net_mdns_prev_vars; // must lock mutex before use
static error_type_t net_mdns_var_restore_shadow_defaults(unsigned int index, net_mdns_var_id_t var_id);
static error_type_t net_mdns_nvram_block_write(unsigned int index);
static error_type_t net_mdns_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *user_data);

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "net.mdns.var: "
#define DBG_ON
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_MDNS_VAR_LOCK() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_mdns_var_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define NET_MDNS_VAR_UNLOCK() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_mdns_var_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

// Local functions

// mdns notifications (from .../common/network/apps/mdns/src/mDNSPlatform.c)
void net_mdns_notify_var_enabled(unsigned int index)
{
    NET_MDNS_VAR_LOCK();
    bool enabled;

    net_mdns_get_var_enabled(index, &enabled);
    DBG_VERBOSE("enabled mdns var change notify for index=%d (enabled=%d)\n", index, enabled);
    if(enabled)
    {
#ifdef __linux__
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, NULL, NULL);
#else
        start_mdns_service();
#endif
#else
        MDNS_SEND_MSG(MDNS_MSG_ENABLE, g_net_mdns_perm_vars[index] );
#endif
    }
    else if(!enabled)
    {
#ifdef __linux__
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_STOP, NET_SERVICE_MDNS, NULL, NULL);
#else 
        stop_mdns_service();
#endif // ! HAVE_NET_SERVICES
#else // ! __linux__
        MDNS_SEND_MSG(MDNS_MSG_DISABLE, g_net_mdns_perm_vars[index] );
#endif // ! __linux__
    }
    NET_MDNS_VAR_UNLOCK();
    return;
}

void net_mdns_notify_var_service_name(unsigned int index)
{
#ifdef __linux__
    char service_name[NET_MDNS_SERVICE_NAME_SIZE];
    net_mdns_get_var_service_name(index, service_name, NET_MDNS_SERVICE_NAME_SIZE);
    DBG_VERBOSE("%s() name change notify '%s'\n", __func__, service_name);
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_CONFIG, NET_SERVICE_MDNS, NULL, NULL);
#else
    update_mdns_conf(service_name);
#endif
#else
    DBG_MSG("service name var change notify\n");
    MDNS_SEND_MSG(MDNS_MSG_REREGISTER_START, g_net_mdns_perm_vars[index]);
#endif
    return;
}

/** 
 *  Boot-time initialization
 * 
 *  This function is called at system startup following initialization of the NVRAM driver and
 *  partition manager.  It initializes the module datastore variables to either their default values
 *  or values previously stored in NVRAM.
 * 
 **/ 
void net_mdns_var_init(uint8_t iface_count)
{
    unsigned int os_rcode;
    error_type_t retval;
    int index = 0;

    if (iface_count > 1) return; // only support 1 mdns dataset

#ifdef __linux__
    os_rcode = posix_mutex_init(&g_net_mdns_var_mutex);
#else
    os_rcode = os_mutex_create(&g_net_mdns_var_mutex, "mdnsVar", OS_INHERIT);
#endif

    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating mutex (os_rcode=0x%02x)\n", os_rcode);
        goto error;
    }

    char vars_id[5]; // 4 char NVRAM label (as defined in data_access_api.h) + NULL terminator
    char vars_id_prefix[4]; // 3 char prefix + NULL terminator 
    strncpy(vars_id_prefix, NET_MDNS_VARS_ID, 3);
    vars_id_prefix[3] = '\0';

    for( index = 0; index < iface_count; index++)
    {
        /*
         *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
         *  net_iface_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
         *  values.
         */
        sprintf(vars_id, "%s%c", vars_id_prefix, net_base64_uint8_to_char(index));
        DBG_VERBOSE("registering NVRAM label %s (cb=%08x)\n", vars_id, net_mdns_nvram_init_cb);
        retval = nvram_variable_register(
            g_net_mdns_nvram_hndl+index, 
            vars_id,
            NET_MDNS_VARS_VERSION, 
            sizeof(net_mdns_perm_vars_t), 
            net_mdns_nvram_init_cb, (void *)(uint32_t)index);
        if(retval != OK)
        {
            // NVRAM is not available so just set defaults and operate on the local copies            
            net_mdns_var_restore_shadow_defaults( index, NET_MDNS_VAR_ALL);
        }
    }
    g_net_mdns_nvram_is_registered = true;

error:
    return;
}

error_type_t net_mdns_var_restore_defaults(unsigned int index, net_mdns_var_id_t var_id)
{
    error_type_t rcode;

    NET_MDNS_VAR_LOCK();

    // save current var values for later comparison
    memcpy(&g_net_mdns_prev_vars, &g_net_mdns_perm_vars[index], sizeof(g_net_mdns_prev_vars));

    // restore defaults
    rcode = net_mdns_var_restore_shadow_defaults(index, var_id);
    if(rcode != OK)
    {
        goto done;
    }
    net_mdns_nvram_block_write(index);

    // send out any necessary change notifications
    if(g_net_mdns_prev_vars.mdns_enabled != g_net_mdns_perm_vars[index].mdns_enabled)
    {
        net_mdns_notify_var_enabled(index);
    }
    else if( g_net_mdns_prev_vars.mdns_enabled && g_net_mdns_perm_vars[index].mdns_enabled && 
        (strcmp(g_net_mdns_prev_vars.service_name, g_net_mdns_perm_vars[index].service_name) != 0) )
    {
        // previously enabled and still enabled AND service_name name setting has changed
        net_mdns_notify_var_service_name(index);
    }

done:
    NET_MDNS_VAR_UNLOCK();
    return rcode;
}

/** 
 *  Restore one or all variables stored in the discrete dynamic NVRAM block to their default values
 * 
 *  This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 *  individual values when their default function is called.  
 *  
 *  \param index treck index for interface
 *  \param var_id variable to initialize to default value, or NET_MDNS_VAR_ALL for all vars
 *  
 *  \return error_type_t
 *  \retval OK
 *  \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails
 **/ 
static error_type_t net_mdns_var_restore_shadow_defaults(unsigned int index, net_mdns_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;
    
    DBG_VERBOSE("%s: index=%d\n", __func__, index);
    if (index >= NET_MAX_INTERFACES)
    {
        DBG_ERR("%s: INVALID index=%d\n", __func__, index);
        return FAIL;
    }
             
    all_vars = (var_id == NET_MDNS_VAR_ALL);

    NET_MDNS_VAR_LOCK();

    switch(var_id)
    {
    case NET_MDNS_VAR_ALL:
        // fall through

    case NET_MDNS_VAR_ENABLED:        
        g_net_mdns_perm_vars[index].mdns_enabled = true; // factory default
        if (!all_vars) break;

    case NET_MDNS_VAR_SERVICE_NAME:
        {
            char *default_service_name;
#ifdef __linux__
            default_service_name = (char*)DEFAULT_BONJOUR_SERVICE_NAME; // factory default
#else
            default_service_name = (char*)mdns_get_default_service_name();
            if ( default_service_name == NULL )
            {
                platvars_get_prtnamestr(&default_service_name);
            } 
#endif
           
            strncpy(g_net_mdns_perm_vars[index].service_name, default_service_name, NET_MDNS_SERVICE_NAME_SIZE);
            g_net_mdns_perm_vars[index].service_name[NET_MDNS_SERVICE_NAME_SIZE-1] = '\0';
        }
        if (!all_vars) break;

#ifdef HAVE_LPD
	case NET_MDNS_LPD_ENABLED:
	g_net_mdns_perm_vars[index].lpd_enabled = true;
	if (!all_vars) break;
#endif // HAVE_LPD

    default:
        // do nothing
        break;
    }

    NET_MDNS_VAR_UNLOCK();

    return(retval);
}

static error_type_t net_mdns_nvram_block_write(unsigned int index)
{
    error_type_t retval = NET_MDNS_OK;
      
    if(g_net_mdns_nvram_is_registered)
    {        
        retval = nvram_set_var(g_net_mdns_nvram_hndl[index], &g_net_mdns_perm_vars[index]);
        if(retval < 0)
        {
            retval = NET_MDNS_FAIL;
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
 *  
 *  \param[in,out] init_loc pointer to existing block data on entry, destination for new
 *  block data to be returned 
 *  
 *  \return error_type_t
 *  \retval OK
 *
 **/ 
static error_type_t net_mdns_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *user_data)
{
    ASSERT( NULL != init_loc );
    uint8_t index = (uint8_t)(uint32_t)user_data;
    XASSERT(index < NET_MAX_INTERFACES, index);

    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            NET_MDNS_VAR_LOCK();  
            net_mdns_var_restore_shadow_defaults(index, NET_MDNS_VAR_ALL);
            ASSERT(sizeof(net_mdns_perm_vars_t) <= init_loc_size);
            memcpy(init_loc, g_net_mdns_perm_vars+index, sizeof(net_mdns_perm_vars_t));
            NET_MDNS_VAR_UNLOCK();
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(g_net_mdns_perm_vars+index, init_loc, sizeof(net_mdns_perm_vars_t));
            break;
    }
    return ( OK );
}

//=========================================================================================================================
// DATASTORE: mDNS configuration variables
//=========================================================================================================================
#ifdef HAVE_LPD
error_type_t net_lpd_get_var_enabled(unsigned int index, bool *enabled)
{
    ASSERT( NULL != enabled );
    NET_MDNS_VAR_LOCK();
    *enabled = g_net_mdns_perm_vars[index].lpd_enabled;
    NET_MDNS_VAR_UNLOCK();

    return NET_MDNS_OK;
}

error_type_t net_lpd_set_var_enabled(unsigned int index, bool enabled)
{
    NET_MDNS_VAR_LOCK();
    if(g_net_mdns_perm_vars[index].lpd_enabled == enabled)
    {
        // no change - ignore request
        NET_MDNS_VAR_UNLOCK();
        return NET_MDNS_OK;
    }

    g_net_mdns_perm_vars[index].lpd_enabled = enabled; 
    net_mdns_nvram_block_write(index);
    NET_MDNS_VAR_UNLOCK();

    net_lpd_notify_var_enabled(index);

    return NET_MDNS_OK;
}
#endif // HAVE_LPD

error_type_t net_mdns_get_var_enabled(unsigned int index, bool *enabled)
{
    ASSERT( NULL != enabled );
    NET_MDNS_VAR_LOCK();
    *enabled = g_net_mdns_perm_vars[index].mdns_enabled;
    NET_MDNS_VAR_UNLOCK();

    return NET_MDNS_OK;
}

error_type_t net_mdns_set_var_enabled(unsigned int index, bool enabled)
{
    NET_MDNS_VAR_LOCK();
    if(g_net_mdns_perm_vars[index].mdns_enabled == enabled)
    {
        // no change - ignore request
        NET_MDNS_VAR_UNLOCK();
        return NET_MDNS_OK;
    }

    g_net_mdns_perm_vars[index].mdns_enabled = enabled; 
    net_mdns_nvram_block_write(index);
    NET_MDNS_VAR_UNLOCK();

    net_mdns_notify_var_enabled(index);

    return NET_MDNS_OK;
}

error_type_t net_mdns_get_var_service_name(unsigned int index, char *service_name, uint8_t len)
{
    ASSERT( NULL != service_name );

    if(!len)
    { 
        return NET_MDNS_OK;
    }

    NET_MDNS_VAR_LOCK();
    strncpy(service_name, g_net_mdns_perm_vars[index].service_name, len);
    service_name[len-1] = '\0';
    NET_MDNS_VAR_UNLOCK();

    return NET_MDNS_OK;
}

error_type_t net_mdns_set_var_service_name(unsigned int index, const char *service_name)
{
    if(!service_name)
    {
        // invalid parameter
        return NET_MDNS_FAIL;
    }

    NET_MDNS_VAR_LOCK();
    
    if(strncmp(g_net_mdns_perm_vars[index].service_name, service_name, NET_MDNS_SERVICE_NAME_SIZE) == 0)
    {
        // no change - ignore request
        NET_MDNS_VAR_UNLOCK();
        return NET_MDNS_OK;
    }

    strncpy(g_net_mdns_perm_vars[index].service_name, service_name, NET_MDNS_SERVICE_NAME_SIZE);
    g_net_mdns_perm_vars[index].service_name[NET_MDNS_SERVICE_NAME_SIZE-1] = '\0';
    net_mdns_nvram_block_write(index);
    NET_MDNS_VAR_UNLOCK();

    net_mdns_notify_var_service_name(index);

    return NET_MDNS_OK;
}

#ifdef __cplusplus
}
#endif

// eof
