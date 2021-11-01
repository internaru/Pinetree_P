 /*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// public includes
#include <string.h>

// common headers
#ifdef __linux__

#include "os_network.h"
#include "nvram_api.h"
#include "net_api.h"

#else // ! __linux__
#include "memAPI.h"
#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include "dprintf.h"
#include "SysInit.h"
#include "error_types.h"
#include "platform.h"
#include "nvram_api.h"
#include "tx_api.h"
#endif // ! __linux__

#include "data_access_api.h"

// local headers
#include "eth_phy_api.h"
#include "eth_mac_api.h"
#include "net_eth_vars.h"
#include "net_eth_api.h"
#include "net_eth.h"

// values which reside in dynamic permanent storage
// IMPORTANT:  increment NET_ETH_VARS_VERSION whenever this structure changes
#define NET_ETH_VARS_VERSION 0
typedef struct
{                                   
    eth_mac_link_config_t   link_config;
} net_eth_perm_vars_t;


//--------------------------------------
// Local Variables
//--------------------------------------

static bool g_net_eth_nvram_is_registered = false;
static nvram_handle_t *g_net_eth_nvram_hndl;
static net_eth_perm_vars_t g_net_eth_perm_vars;
static net_eth_perm_vars_t g_net_eth_prev_vars; // must lock mutex before use
static OS_MUTEX g_net_eth_var_mutex;

static error_type_t net_eth_var_restore_shadow_defaults(net_eth_var_id_t var_id);
static error_type_t net_eth_nvram_block_write(void);
static error_type_t net_eth_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused);

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#ifdef __linux__

#define DBG_PRFX "eth.var: "

#define DBG_ERR_ON
#define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

#else

#define DBG_PRFX "ETHVAR: "
#define DBG_ON
#define DBG_ERR_ON
//#define DBG_VERBOSE_ON
//#define DBG_RELEASE_ON

#ifdef DBG_RELEASE_ON
#define DBG_PRINTF_MACRO(...) dbg_printf(DBG_PRFX __VA_ARGS__)
#else
#define DBG_PRINTF_MACRO(...) DPRINTF((DBG_SOFT|DBG_OUTPUT), (DBG_PRFX __VA_ARGS__))
#endif
#ifdef DBG_ON
#define DBG_MSG(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_MSG(...)
#endif
#ifdef DBG_ERR_ON
#define DBG_ERR(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_ERR(...)
#endif
#ifdef DBG_VERBOSE_ON
#define DBG_VERBOSE(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_VERBOSE(...)
#endif
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#define DBG_CMD(...) cmd_printf(__VA_ARGS__) // omit module prefix for debug command output
#endif
#endif


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#ifdef __linux__
#define NET_ETH_VAR_LOCK() \
        { \
            unsigned int os_rcode; \
            os_rcode = os_mutex_get(&g_net_eth_var_mutex, OS_WAIT_FOREVER); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define NET_ETH_VAR_UNLOCK() \
        { \
            unsigned int os_rcode; \
            os_rcode = os_mutex_put(&g_net_eth_var_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 
#else
#define NET_ETH_VAR_LOCK() \
        { \
            UINT threadx_rcode; \
            threadx_rcode = tx_mutex_get(&g_net_eth_var_mutex, TX_WAIT_FOREVER); \
            XASSERT(threadx_rcode == OS_SUCCESS, threadx_rcode); \
        } 

#define NET_ETH_VAR_UNLOCK() \
        { \
            UINT threadx_rcode; \
            threadx_rcode = tx_mutex_put(&g_net_eth_var_mutex); \
            XASSERT(threadx_rcode == OS_SUCCESS, threadx_rcode); \
        } 
#endif


/** 
 *  Boot-time initialization
 * 
 *  This function is called at system startup following initialization of the NVRAM driver and
 *  partition manager.  It initializes the module datastore variables to either their default values
 *  or values previously stored in NVRAM.
 * 
 **/ 
void net_eth_var_init(void)
{
    unsigned int os_rcode;
    error_type_t retval;

#ifdef __linux__
    os_rcode = posix_mutex_init(&g_net_eth_var_mutex);
#else
    os_rcode = tx_mutex_create(&g_net_eth_var_mutex, "ethVar", TX_INHERIT);
#endif
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating mutex (os_rcode=0x%02x)\n", os_rcode);
        goto error;
    }

    /*
     *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
     *  net_eth_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
     *  values.
     */
    retval = nvram_variable_register(&g_net_eth_nvram_hndl, NET_ETH_VARS_ID, NET_ETH_VARS_VERSION, sizeof(net_eth_perm_vars_t), net_eth_nvram_init_cb, NULL);
    if(retval == OK)
    {
        g_net_eth_nvram_is_registered = true;
    }
    else
    {
        // NVRAM is not available so just set defaults and operate on the local copies       
        net_eth_var_restore_shadow_defaults(NET_ETH_VAR_ALL);
    }

error:
    return;
}

error_type_t net_eth_var_restore_defaults(net_eth_var_id_t var_id)
{
    error_type_t rcode;

    NET_ETH_VAR_LOCK();

    // save current var values for later comparison
    memcpy(&g_net_eth_prev_vars, &g_net_eth_perm_vars, sizeof(g_net_eth_prev_vars));

    // restore defaults
    rcode = net_eth_var_restore_shadow_defaults(var_id);
    if(rcode != OK)
    {
        goto done;
    }
    net_eth_nvram_block_write();

    // send out any necessary change notifications
    if(g_net_eth_prev_vars.link_config != g_net_eth_perm_vars.link_config)
    {
        net_eth_context_t *net_eth_ctxt;
        net_eth_ctxt = net_eth_get_ctxt();
        eth_mac_apply_config_options(net_eth_ctxt->dev_index, g_net_eth_perm_vars.link_config);
    }

done:
    NET_ETH_VAR_UNLOCK();
    return rcode;
}

/** 
 *  Restore one or all variables stored in the discrete dynamic NVRAM block to their default values
 * 
 *  This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 *  individual values when their default function is called.  
 *  
 *  \param var_id variable to initialize to default value, or NET_ETH_VAR_ALL for all vars
 *  
 *  \return error_type_t
 *  \retval OK
 *  \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails
 **/ 
static error_type_t net_eth_var_restore_shadow_defaults(net_eth_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;

    all_vars = (var_id == NET_ETH_VAR_ALL);

    NET_ETH_VAR_LOCK();

    switch(var_id)
    {
    case NET_ETH_VAR_ALL:
        // fall through

    case NET_ETH_VAR_LINK_CONFIG:
        g_net_eth_perm_vars.link_config = ETH_MAC_LINK_CONFIG_AUTO;
        if (!all_vars) break;

    default:
        // do nothing
        break;
    }

    NET_ETH_VAR_UNLOCK();

    return(retval);
}

static error_type_t net_eth_nvram_block_write(void)
{
    error_type_t retval = NET_ETH_OK;

    if(g_net_eth_nvram_is_registered)
    {
        retval = nvram_set_var(g_net_eth_nvram_hndl, &g_net_eth_perm_vars);
        if(retval < 0)
        {
            retval = NET_ETH_FAIL;
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
static error_type_t net_eth_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused)
{
    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            NET_ETH_VAR_LOCK();
            net_eth_var_restore_shadow_defaults(NET_ETH_VAR_ALL);
            ASSERT(sizeof(net_eth_perm_vars_t) <= init_loc_size);
            memcpy(init_loc, &g_net_eth_perm_vars, sizeof(net_eth_perm_vars_t));
            NET_ETH_VAR_UNLOCK();
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(&g_net_eth_perm_vars, init_loc, sizeof(net_eth_perm_vars_t));
            break;
    }
    return ( OK );
}


//=========================================================================================================================
// DATASTORE: Ethernet link configuration variables
//=========================================================================================================================

error_type_t net_eth_get_var_link_config(eth_mac_link_config_t *link_config)
{
    NET_ETH_VAR_LOCK();
    *link_config = g_net_eth_perm_vars.link_config;
    NET_ETH_VAR_UNLOCK();

    return NET_ETH_OK;
}

error_type_t net_eth_set_var_link_config(eth_mac_link_config_t link_config)
{
    NET_ETH_VAR_LOCK();
    if(g_net_eth_perm_vars.link_config == link_config)
    {
        // no change - ignore request
        NET_ETH_VAR_UNLOCK();
        return NET_ETH_OK;
    }
    g_net_eth_perm_vars.link_config = link_config; 
    net_eth_nvram_block_write();
    NET_ETH_VAR_UNLOCK();

    net_eth_context_t *net_eth_ctxt;
    net_eth_ctxt = net_eth_get_ctxt();
    eth_mac_apply_config_options(net_eth_ctxt->dev_index, link_config);

    return NET_ETH_OK;
}

error_type_t net_eth_get_var_link_status(eth_mac_link_status_t *link_status)
{
    net_eth_context_t *net_eth_ctxt;
    net_eth_ctxt = net_eth_get_ctxt();

    *link_status = eth_mac_get_link_status(net_eth_ctxt->dev_index);

    return NET_ETH_OK;
}

error_type_t net_eth_get_var_link_statistics(eth_mac_rx_stats_t *rx_stats, eth_mac_tx_stats_t *tx_stats)
{
    net_eth_context_t *net_eth_ctxt;
    net_eth_ctxt = net_eth_lock_ctxt();
    ASSERT(net_eth_ctxt);
    if(rx_stats)
    {
        *rx_stats = net_eth_ctxt->rx_stats;
    }
    if(tx_stats)
    {
        *tx_stats = net_eth_ctxt->tx_stats;
    }
    net_eth_unlock_ctxt(net_eth_ctxt);

    return NET_ETH_OK;
}

