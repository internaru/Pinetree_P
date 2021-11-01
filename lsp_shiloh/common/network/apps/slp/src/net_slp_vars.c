 /*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// public includes
#include <string.h>
#include "memAPI.h"
#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include "dprintf.h"
#include "sys_init_api.h"
#include "error_types.h"
#include "platform_api.h"
#include "nvram_api.h"
#include "net_slp_vars.h"

// local includes
#include "slp.h"

// values which reside in dynamic permanent storage
// IMPORTANT:  increment NET_SLP_VARS_VERSION whenever this structure changes
#define NET_SLP_VARS_VERSION 1
typedef enum
{
    // perm vars
    NET_SLP_VAR_ENABLED,

    // temp vars

    // add new vars above this line
    NET_SLP_NUM_VARS,
    NET_SLP_VAR_ALL
} net_slp_var_id_t;


typedef struct
{                                   
    bool        enabled;
} net_slp_perm_vars_t;


//--------------------------------------
// Local Variables
//--------------------------------------

static bool g_net_slp_nvram_is_registered = false;
static nvram_handle_t *g_net_slp_nvram_hndl;
static net_slp_perm_vars_t g_net_slp_perm_vars;
static pthread_mutex_t g_net_slp_var_mutex;

static error_type_t net_slp_var_restore_defaults(net_slp_var_id_t var_id);
static error_type_t net_slp_nvram_block_write(void);
static error_type_t net_slp_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused);

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "SLPVAR: "
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


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_SLP_VAR_LOCK() \
        { \
            UINT pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&g_net_slp_var_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define NET_SLP_VAR_UNLOCK() \
        { \
            UINT pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&g_net_slp_var_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 


/** 
 *  Boot-time initialization
 * 
 *  This function is called at system startup following initialization of the NVRAM driver and
 *  partition manager.  It initializes the module datastore variables to either their default values
 *  or values previously stored in NVRAM.
 * 
 **/ 
void net_slp_var_init(void)
{
    UINT px_rcode;
    error_type_t retval;

//    threadx_rcode = tx_mutex_create(&g_net_slp_var_mutex, "slpVar", TX_INHERIT);
    px_rcode = posix_mutex_init(&g_net_slp_var_mutex);
    if(px_rcode != 0)
    {
        DBG_ERR("error creating mutex (threadx_rcode=0x%02x)\n", px_rcode);
        goto error;
    }

    /*
     *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
     *  net_slp_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
     *  values.
     */
    retval = nvram_variable_register(&g_net_slp_nvram_hndl, NET_SLP_VARS_ID, 
                                     NET_SLP_VARS_VERSION, sizeof(net_slp_perm_vars_t),
                                     net_slp_nvram_init_cb, NULL);
    if(retval == OK)
    {
        g_net_slp_nvram_is_registered = true;
    }
    else
    {
        // NVRAM is not available so just set defaults and operate on the local copies       
        net_slp_var_restore_defaults(NET_SLP_VAR_ALL);
    }

    error:
    return;
}

/** 
 *  Restore one or all variables stored in the discrete dynamic NVRAM block to their default values
 * 
 *  This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 *  individual values when their default function is called.  
 *  
 *  \param var_id variable to initialize to default value, or NET_SLP_VAR_ALL for all vars
 *  
 *  \return error_type_t
 *  \retval OK
 *  \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails
 **/ 
static error_type_t net_slp_var_restore_defaults(net_slp_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;

    all_vars = (var_id == NET_SLP_VAR_ALL);

    NET_SLP_VAR_LOCK();

    switch(var_id)
    {
    case NET_SLP_VAR_ALL:
        // fall through

    case NET_SLP_VAR_ENABLED:
        g_net_slp_perm_vars.enabled = true;
        if (!all_vars) break;

    default:
        // do nothing
        break;
    }

    NET_SLP_VAR_UNLOCK();

    return(retval);
}

static error_type_t net_slp_nvram_block_write(void)
{
    error_type_t retval = NET_SLP_OK;

    if(g_net_slp_nvram_is_registered)
    {
        retval = nvram_set_var(g_net_slp_nvram_hndl, &g_net_slp_perm_vars);
        if(retval < 0)
        {
            retval = NET_SLP_FAIL;
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
 *  \param[in,out] init_loc pointer to existing block data on entry, destination for new
 *  block data to be returned 
 *  
 *  \return error_type_t
 *  \retval OK
 *
 **/ 
static error_type_t net_slp_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused)
{
    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            NET_SLP_VAR_LOCK();
            net_slp_var_restore_defaults(NET_SLP_VAR_ALL);
            ASSERT(sizeof(net_slp_perm_vars_t) <= init_loc_size);
            memcpy(init_loc, &g_net_slp_perm_vars, sizeof(net_slp_perm_vars_t));
            NET_SLP_VAR_UNLOCK();
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(&g_net_slp_perm_vars, init_loc, sizeof(net_slp_perm_vars_t));
            break;
    }
    return ( OK );
}

//=========================================================================================================================
// DATASTORE: SLP configuration variables
//=========================================================================================================================

error_type_t net_slp_get_var_enabled(bool *enabled)
{
    NET_SLP_VAR_LOCK();
    *enabled = g_net_slp_perm_vars.enabled;
    NET_SLP_VAR_UNLOCK();

    return NET_SLP_OK;
}

error_type_t net_slp_set_var_enabled(bool enabled)
{
    NET_SLP_VAR_LOCK();
    if(g_net_slp_perm_vars.enabled == enabled)
    {
        // no change - ignore request
        NET_SLP_VAR_UNLOCK();
        return NET_SLP_OK;
    }

    g_net_slp_perm_vars.enabled = enabled; 
    net_slp_nvram_block_write();
    NET_SLP_VAR_UNLOCK();

    net_slp_notify_var_enabled();

    return NET_SLP_OK;
}

