 /*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
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
#include "net_api.h"
#include "net_iface.h" // net_iface_enum_from_if_index()
#include "net_iface_api.h" 
#include "net_uap_vars.h"
#include "net_uap_api.h" // DEFAULT_UAP_SSID
#include "net_conf_api.h" // uAP_e
#include "net_wlan_api.h" // NET_WLAN_STATS
#include "os_network.h" // inet_pton
#include "platform_api.h" // platvars_get_prtnamestr()

// debug
#define DBG_PRFX "net.uap.var: "
#define DBG_ON
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

// local stubs
#define tbd DBG_ERR("%s - revisit impl\n", __func__); return OK
static error_type_t net_uap_notify_var_server_addr(unsigned index) { tbd; }
static error_type_t net_uap_notify_var_subnet_mask(unsigned index) { tbd; }

// values which reside in dynamic permanent storage
// IMPORTANT:  increment NET_UAP_VARS_VERSION whenever this structure changes
#define NET_UAP_VARS_VERSION 0
typedef struct
{
    uint32_t uap_enabled; // bool
    char     ssid_name[MAX_SSID_NAME_LEN];
    uint32_t ip_addr;

    // uap udhcp settings
    uint32_t lease_time;
    int32_t  address_range;
    uint32_t server_addr;
    uint32_t subnet_mask;
    char     starting_address[MAX_IP_ADDR_STR_SIZE];
    char     last_address[MAX_IP_ADDR_STR_SIZE];

    // filter_list?

} wifi_uap_perm_vars_t;


//--------------------------------------
// Local Variables
//--------------------------------------

static bool g_net_uap_nvram_is_registered = false;
pthread_mutex_t g_net_uap_var_mutex;

static nvram_handle_t *g_net_uap_nvram_hndl[WIFI_NUM_IFACES];
static wifi_uap_perm_vars_t g_wifi_uap_perm_vars[WIFI_NUM_IFACES];
static wifi_uap_perm_vars_t g_net_uap_prev_vars; // must lock mutex before use

static wifi_uap_perm_vars_t g_wifi_uap_ctxt[WIFI_NUM_IFACES]; // locally available

static error_type_t net_uap_var_restore_shadow_defaults(unsigned int index, net_uap_var_id_t var_id);
static error_type_t net_uap_nvram_block_write(unsigned int index);
static error_type_t net_uap_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *user_data);

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================
//unused static pthread_mutex_t g_net_uap_context_mutex;
#define UAP_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_uap_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define UAP_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_uap_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define NET_UAP_VAR_LOCK() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_uap_var_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define NET_UAP_VAR_UNLOCK() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_uap_var_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

// uap notifications
void net_uap_notify_var_is_enabled(unsigned int index)
{
    NET_UAP_VAR_LOCK();
    uint32_t uap_enabled;
    uint8_t iface_enum = index; // revisit = net_get_active_wifi_intf();

    // uap-enabled?
    uap_get_var_is_enabled(index, &uap_enabled);
    DBG_MSG("enabled uap var change notify for index=%d (uap %d->%d)\n", index, g_wifi_uap_ctxt[iface_enum].uap_enabled, uap_enabled);
    if(uap_enabled && !(g_wifi_uap_ctxt[iface_enum].uap_enabled))
    {
        g_wifi_uap_ctxt[iface_enum].uap_enabled = 1; // true
#ifdef __linux__
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_START, NET_SERVICE_UAP, NULL, NULL); // start_uap_service();
#else
        start_uap_service();

#if 1 // tickle link-layer
        net_link_t* wifi_uap_link = net_link_lookup_by_name(NET_UAP_LINK_NAME);
        net_link_event_handler_cb(NET_LINK_EVENT_UP, wifi_uap_link);
#else // tickle intf-layer
        net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_UAP);
        net_iface_notify(intf, NET_LINK_EVENT_UP);
#endif // tickle net-layer
#endif // ! HAVE_NET_SERVICES

#else // ! __linux__
        UAP_SEND_MSG(UAP_MSG_ENABLE, g_wifi_uap_ctxt[iface_enum]);
#endif
    }
    else if(!uap_enabled && g_wifi_uap_ctxt[iface_enum].uap_enabled)
    {
        g_wifi_uap_ctxt[iface_enum].uap_enabled = 0; // false
#ifdef __linux__

#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_STOP, NET_SERVICE_UAP, NULL, NULL); // stop_uap_service();
#else
        stop_uap_service();
#if 1 // tickle link-layer
        net_link_t* wifi_uap_link = net_link_lookup_by_name(NET_UAP_LINK_NAME);
        net_link_event_handler_cb(NET_LINK_EVENT_DOWN, wifi_uap_link);
#else // tickle intf-layer
        net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_UAP);
        net_iface_notify(intf, NET_LINK_EVENT_DOWN);
#endif
#endif // ! HAVE_NET_SERVICES

#else // ! __linux__
        UAP_SEND_MSG(UAP_MSG_DISABLE, g_wifi_uap_ctxt[iface_enum]);
#endif // ! __linux__
    }
    NET_UAP_VAR_UNLOCK();
    return;
}

void net_uap_notify_var_ssid_name(unsigned int index)
{
#ifdef __linux__
    uint8_t ssid[MAX_SSID_NAME_LEN];
    uap_get_var_ssid((uint8_t)index, ssid, MAX_SSID_NAME_LEN);
    DBG_MSG("%s() name change notify '%s'\n", __func__, (char*)ssid);
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_CONFIG, NET_SERVICE_UAP, (void*)ssid, NULL);
#else
    update_uap_conf((char*)ssid); // cast
    net_interface_t* intf = net_iface_lookup_by_name(NET_IF_NAME_UAP);
    net_iface_notify(intf, NET_LINK_EVENT_RESET);
#endif
#else
    DBG_MSG("service name var change notify\n");
    uint8_t iface_enum = index; // revisit = net_get_active_wifi_intf(index);
    UAP_SEND_MSG(UAP_MSG_REREGISTER_START, g_wifi_uap_ctxt[iface_enum]);
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
void net_uap_var_init(uint8_t iface_count)
{
    unsigned int os_rcode;
    error_type_t retval;

#ifdef __linux__
    os_rcode = posix_mutex_init(&g_net_uap_var_mutex);
#else
    os_rcode = os_mutex_create(&g_net_uap_var_mutex, "uapVar", OS_INHERIT);
#endif

    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating mutex (os_rcode=0x%02x)\n", os_rcode);
        goto error;
    }

    if (iface_count > 1) return; // limit uap support to 1 dataset

    char vars_id[5]; // 4 char NVRAM label (as defined in data_access_api.h) + NULL terminator
    char vars_id_prefix[4]; // 3 char prefix + NULL terminator 
    strncpy(vars_id_prefix, UAP_IFACE_VAR_ID, 3);
    vars_id_prefix[3] = '\0';

    uint8_t iface_enum;
        
    for( iface_enum = 0; iface_enum < iface_count; iface_enum++)
    {
        /*
         *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
         *  net_iface_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
         *  values.
         */
        sprintf(vars_id, "%s%c", vars_id_prefix, net_base64_uint8_to_char(iface_enum));
        DBG_VERBOSE("registering NVRAM label %s (cb=%08x)\n", vars_id, net_uap_nvram_init_cb);
        retval = nvram_variable_register(
            g_net_uap_nvram_hndl+iface_enum, 
            vars_id,
            NET_UAP_VARS_VERSION, 
            sizeof(wifi_uap_perm_vars_t), 
            net_uap_nvram_init_cb, (void *)(uint32_t)iface_enum);
        if(retval != OK)
        {
            // NVRAM is not available so just set defaults and operate on the local copies            
            net_uap_var_restore_shadow_defaults(iface_enum, NET_UAP_VAR_ALL);
        }
    }
    g_net_uap_nvram_is_registered = true;

error:
    return;
}

error_type_t net_uap_var_restore_defaults(unsigned int index, net_uap_var_id_t var_id)
{
    error_type_t rcode;

    NET_UAP_VAR_LOCK();

    // save current var values for later comparison
    memcpy(&g_net_uap_prev_vars, &g_wifi_uap_perm_vars[index], sizeof(g_net_uap_prev_vars));

    // restore defaults
    rcode = net_uap_var_restore_shadow_defaults(index, var_id);
    if(rcode != OK)
    {
        goto done;
    }
    net_uap_nvram_block_write(index);

    // send out any necessary change notifications
    if(g_net_uap_prev_vars.uap_enabled != g_wifi_uap_perm_vars[index].uap_enabled)
    {
        net_uap_notify_var_is_enabled(index);
    }
    else if( g_net_uap_prev_vars.uap_enabled && g_wifi_uap_perm_vars[index].uap_enabled && 
        (strcmp(g_net_uap_prev_vars.ssid_name, g_wifi_uap_perm_vars[index].ssid_name) != 0) )
    {
        // previously enabled and still enabled AND ssid_name name setting has changed
        net_uap_notify_var_ssid_name(index);
    }

done:
    NET_UAP_VAR_UNLOCK();
    return rcode;
}

// return the factory default uAP ssid string
static char* uap_get_default_ssid_name(int index_unused)
{
    return DEFAULT_UAP_SSID; // factory default
}

/** 
 *  Restore one or all variables stored in the discrete dynamic NVRAM block to their default values
 * 
 *  This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 *  individual values when their default function is called.  
 *  
 *  \param index local index for interface
 *  \param var_id variable to initialize to default value, or NET_UAP_VAR_ALL for all vars
 *  
 *  \return error_type_t
 *  \retval OK
 *  \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails
 **/ 
static error_type_t net_uap_var_restore_shadow_defaults(unsigned int index, net_uap_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;
    
    DBG_VERBOSE("%s: index=%d)\n", __func__, index);

    all_vars = (var_id == NET_UAP_VAR_ALL);

    NET_UAP_VAR_LOCK();

    switch(var_id)
    {
    case NET_UAP_VAR_ALL:
        // fall through

    case NET_UAP_VAR_ENABLED:        
        g_wifi_uap_perm_vars[index].uap_enabled = 0; // factory default 'false'
        if (!all_vars) break;

    case NET_UAP_VAR_SERVICE_NAME:
        {
            char *default_ssid_name;
            default_ssid_name = (char*)uap_get_default_ssid_name(index);
            if ( default_ssid_name == NULL )
            {
                platvars_get_prtnamestr(&default_ssid_name);
            }            
            strncpy(g_wifi_uap_perm_vars[index].ssid_name, default_ssid_name, MAX_SSID_NAME_LEN);
            g_wifi_uap_perm_vars[index].ssid_name[MAX_SSID_NAME_LEN-1] = '\0';
        }
        if (!all_vars) break;

    case NET_UAP_VAR_SERVER_ADDR:
            g_wifi_uap_perm_vars[index].server_addr = ipv4_str2b32(UAP_DEFAULT_STARTING_ADDRESS);
	    if (!all_vars) break;

    case NET_UAP_VAR_SUBNET_MASK:
            g_wifi_uap_perm_vars[index].subnet_mask = ipv4_str2b32(UAP_DEFAULT_SUBNET_MASK);
	    if (!all_vars) break;

    case NET_UAP_VAR_LEASE_TIME:
	    g_wifi_uap_perm_vars[index].lease_time = UAP_MAX_UDHCP_LEASE_TIME;
	    if (!all_vars) break;

    case NET_UAP_VAR_NUMBER_OF_ADDRESS:
           g_wifi_uap_perm_vars[index].address_range = UAP_UDHCP_ADDRESS_RANGE;
           if (!all_vars) break;

    case NET_UAP_VAR_STARTING_ADDRESS:
            strncpy(g_wifi_uap_perm_vars[index].starting_address, UAP_DEFAULT_STARTING_ADDRESS, strlen(UAP_DEFAULT_STARTING_ADDRESS));
            g_wifi_uap_perm_vars[index].starting_address[MAX_IP_ADDR_STR_SIZE-1] = '\0';
    	if (!all_vars) break;

    case NET_UAP_VAR_LAST_ADDRESS:
            strncpy(g_wifi_uap_perm_vars[index].last_address, UAP_DEFAULT_ENDING_ADDRESS, strlen(UAP_DEFAULT_ENDING_ADDRESS));
            g_wifi_uap_perm_vars[index].last_address[MAX_IP_ADDR_STR_SIZE-1] = '\0';
    	if (!all_vars) break;

    default:
        // do nothing
        break;
    }

    NET_UAP_VAR_UNLOCK();

    return(retval);
}

static error_type_t net_uap_nvram_block_write(unsigned int index)
{
    error_type_t retval = NET_UAP_OK;
      
    if(g_net_uap_nvram_is_registered)
    {        
        retval = nvram_set_var(g_net_uap_nvram_hndl[index], &g_wifi_uap_perm_vars[index]);
        if(retval < 0)
        {
            DBG_ERR("%s[%d] failed!\n", __func__, index);
            retval = NET_UAP_FAIL;
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
static error_type_t net_uap_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *user_data)
{
    ASSERT( NULL != init_loc );
    unsigned int index = (uint32_t)user_data; // cast
    XASSERT(index < WIFI_NUM_IFACES, index);

    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            NET_UAP_VAR_LOCK();  
            net_uap_var_restore_shadow_defaults(index, NET_UAP_VAR_ALL);
            ASSERT(sizeof(wifi_uap_perm_vars_t) <= init_loc_size);
            memcpy(init_loc, g_wifi_uap_perm_vars+index, sizeof(wifi_uap_perm_vars_t));
            NET_UAP_VAR_UNLOCK();
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(g_wifi_uap_perm_vars+index, init_loc, sizeof(wifi_uap_perm_vars_t));
            break;
    }
    return ( OK );
}

//=========================================================================================================================
// DATASTORE: uAP configuration variables
//=========================================================================================================================

// udhcp lease time
error_type_t wifi_intf_udhcp_get_var_lease_time(unsigned int index, uint32_t* lease_time)
{
    if (!lease_time)
    {
       return NET_UAP_FAIL;
    }

    NET_UAP_VAR_LOCK();
    
    *lease_time = g_wifi_uap_perm_vars[index].lease_time;

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

error_type_t wifi_intf_udhcp_set_var_lease_time(unsigned int index, uint32_t lease_time)
{
    NET_UAP_VAR_LOCK();
    
    g_wifi_uap_perm_vars[index].lease_time = lease_time;
    net_uap_nvram_block_write(index);

    NET_UAP_VAR_UNLOCK();

    //net_uap_notify_var_lease_time(index);

    return NET_UAP_OK;
}

// dhcp address range
error_type_t wifi_intf_udhcp_get_var_num_addrs(unsigned int index, uint8_t* address_range)
{
    if (!address_range) return NET_UAP_FAIL;



    NET_UAP_VAR_LOCK();
    
    *address_range = g_wifi_uap_perm_vars[index].address_range;

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

error_type_t wifi_intf_udhcp_set_var_num_addrs(unsigned int index, uint8_t address_range)
{
    if (address_range > MAX_UDHCP_ADDRESS_RANGE)
    {
        return NET_UAP_FAIL;
    }

    NET_UAP_VAR_LOCK();
    
    g_wifi_uap_perm_vars[index].address_range = address_range;
    net_uap_nvram_block_write(index);

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

// udhcp starting address
error_type_t wifi_intf_udhcp_get_var_starting_addr(unsigned int index, uint32_t* starting_address)
{
    // valid parameter?
    ASSERT( NULL != starting_address );
    if(!starting_address)
    {
        return NET_UAP_FAIL;
    }

    NET_UAP_VAR_LOCK();

    inet_pton(AF_INET, g_wifi_uap_perm_vars[index].starting_address, (void*)starting_address);

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

error_type_t wifi_intf_udhcp_set_var_starting_addr(unsigned int index, uint32_t updated_addr)
{
    NET_UAP_VAR_LOCK();
    
    uint32_t current_stored_address = ipv4_str2b32(g_wifi_uap_perm_vars[index].starting_address);
    if (updated_addr == current_stored_address)
    {
        // no change - ignore request
        NET_UAP_VAR_UNLOCK();
        return NET_UAP_OK;
    }

    ipv4_b2str_inplace(updated_addr, g_wifi_uap_perm_vars[index].starting_address);
    net_uap_nvram_block_write(index);

    NET_UAP_VAR_UNLOCK();

    //revisit net_uap_notify_var_starting_address(index);

    return NET_UAP_OK;
}

// udhcp current address
error_type_t wifi_intf_udhcp_get_var_server_addr(unsigned int index, uint32_t* current_server)
{
    // valid parameter?
    ASSERT( NULL != current_server );
    if (!current_server)
    {
        return NET_UAP_FAIL;
    }

    NET_UAP_VAR_LOCK();

    *current_server = g_wifi_uap_perm_vars[index].server_addr;

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

error_type_t wifi_intf_udhcp_set_var_server_addr(unsigned int index, uint32_t updated_addr)
{
    NET_UAP_VAR_LOCK();
    
    uint32_t current_stored_address = g_wifi_uap_perm_vars[index].server_addr;
    if (updated_addr == current_stored_address)
    {
        // no change - ignore request
        NET_UAP_VAR_UNLOCK();
        return NET_UAP_OK;
    }

    g_wifi_uap_perm_vars[index].server_addr = updated_addr;

    net_uap_nvram_block_write(index);
    NET_UAP_VAR_UNLOCK();

    net_uap_notify_var_server_addr(index);

    return NET_UAP_OK;
}

// udhcp subnet mask
error_type_t wifi_intf_udhcp_get_var_subnet_mask(unsigned int index, uint32_t* subnet_mask)
{
    // valid parameter?
    ASSERT( NULL != subnet_mask );
    if (!subnet_mask)
    {
        return NET_UAP_FAIL;
    }

    NET_UAP_VAR_LOCK();

    *subnet_mask = g_wifi_uap_perm_vars[index].subnet_mask;

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

error_type_t wifi_intf_udhcp_set_var_subnet_mask(unsigned int index, uint32_t updated_mask)
{
    NET_UAP_VAR_LOCK();
    
    uint32_t current_stored_mask = g_wifi_uap_perm_vars[index].subnet_mask;
    if (updated_mask == current_stored_mask)
    {
        // no change - ignore request
        NET_UAP_VAR_UNLOCK();
        return NET_UAP_OK;
    }

    g_wifi_uap_perm_vars[index].subnet_mask = updated_mask;

    net_uap_nvram_block_write(index);
    NET_UAP_VAR_UNLOCK();

    net_uap_notify_var_subnet_mask(index);

    return NET_UAP_OK;
}

// udhcp ending address (build last address as starting address + range)
error_type_t wifi_intf_udhcp_get_var_last_addr(unsigned int index, uint32_t* last_address)
{
    ASSERT( NULL != last_address );
    if(!last_address)
    { 
        return NET_UAP_FAIL;
    }

    NET_UAP_VAR_LOCK();

    // convert str -> u32
    uint32_t new_addr = ipv4_str2b32(g_wifi_uap_perm_vars[index].last_address);
    DBG_VERBOSE("%s() addr x%x -> x%x\n", __func__, new_addr, last_address);

    *last_address = new_addr;

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

// 'last' addr generated from 'start' + range
error_type_t net_uap_get_var_last_address_str(unsigned int index, char* last_address_str, uint8_t len)
{
    ASSERT( NULL != last_address_str );
    if(!last_address_str)
    { 
        return NET_UAP_FAIL;
    }

    NET_UAP_VAR_LOCK();

    strncpy(last_address_str, g_wifi_uap_perm_vars[index].last_address, len);
    last_address_str[len-1] = '\0';

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

// more stubs
error_type_t wifi_intf_get_var_station_list(uint8_t index, uap_mlan_ds_sta_list* list) { tbd; }
error_type_t wifi_intf_set_var_station_list(uint8_t index, uap_mlan_ds_sta_list list) { tbd; }

error_type_t wifi_intf_get_var_ssid_prefix(uint8_t index, uint8_t* list, uint8_t len) { tbd; }
error_type_t wifi_intf_set_var_ssid_prefix(uint8_t index, const uint8_t* list, uint8_t len) { tbd; } // const

error_type_t uap_get_var_ssid(uint8_t index, uint8_t* ssid_buffer_u8, uint8_t len)
{
    char* ssid_buffer = (char*) ssid_buffer_u8; // cast

    if(!ssid_buffer)
    { 
        return NET_UAP_FAIL;
    }

    memset(ssid_buffer_u8, 0, len);

    { NET_UAP_VAR_LOCK();

        strncpy(ssid_buffer, g_wifi_uap_perm_vars[index].ssid_name, len);
        ssid_buffer[len-1] = '\0';

    } NET_UAP_VAR_UNLOCK();

    //DBG_VERBOSE("%s(%s)[%d]..\n", __func__, (char*)ssid_buffer_u8, index);

    return NET_UAP_OK;
}

error_type_t uap_set_var_ssid(uint8_t index, uint8_t* ssid_buffer_u8, uint8_t len)
{
    char* ssid_buffer = (char*) ssid_buffer_u8; // cast

    ASSERT( NULL != ssid_buffer );

    if(!ssid_buffer)
    { 
        return NET_UAP_FAIL;
    }

    { NET_UAP_VAR_LOCK();

        strncpy(g_wifi_uap_perm_vars[index].ssid_name, ssid_buffer, len);
        g_wifi_uap_perm_vars[index].ssid_name[MAX_SSID_NAME_LEN-1] = '\0'; // not len-1
        net_uap_nvram_block_write(index);

    } NET_UAP_VAR_UNLOCK();

    DBG_VERBOSE("%s(%s)[%d].. %s\n", __func__, (char*)ssid_buffer_u8, index, g_wifi_uap_perm_vars[index].ssid_name);

    return NET_UAP_OK;
}

error_type_t uap_get_var_is_enabled(uint8_t index, uint32_t* enabled)
{
    DBG_VERBOSE("%s(%d)[%d]..\n", __func__, g_wifi_uap_perm_vars[index].uap_enabled, index);

    NET_UAP_VAR_LOCK();

    *enabled = g_wifi_uap_perm_vars[index].uap_enabled;

    NET_UAP_VAR_UNLOCK();

    return NET_UAP_OK;
}

error_type_t uap_set_var_is_enabled(uint8_t index, uint32_t update)
{
    { NET_UAP_VAR_LOCK();

        if (g_wifi_uap_perm_vars[index].uap_enabled != update)
        {
            g_wifi_uap_perm_vars[index].uap_enabled = update;
            net_uap_nvram_block_write(index);
            net_uap_notify_var_is_enabled(index);
        }

    } NET_UAP_VAR_UNLOCK();

    DBG_VERBOSE("%s(%d)[%d]..\n", __func__, g_wifi_uap_perm_vars[index].uap_enabled, index);

    return NET_UAP_OK;
}

// net iface entries (stub)
NET_WLAN_STATS* net_iface_wlan_get_stats(void) { tbd; return (NET_WLAN_STATS*)NULL; } // cast

#ifdef __cplusplus
}
#endif

// eof
