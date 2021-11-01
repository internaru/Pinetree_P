 /*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// public includes
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
//#include "trsocket.h"
//#include "trmacro.h"
//#include "trtype.h"
#include "posix_ostools.h"
#include "error_types.h"
#include "dprintf.h"
#include "lassert.h"
#include "logger.h"
#include "nvram_api.h"
#include "data_access_api.h"
//#include "net_app_api.h"
#include "net_gcpp_vars.h"
#include "net_gcpp_api.h"

// private includes
#include "gcpp.h"

// values which reside in dynamic permanent storage
// IMPORTANT:  increment NET_GCPP_VARS_VERSION whenever this structure changes
#define NET_GCPP_VARS_VERSION 5
typedef enum
{
    // perm vars
    NET_GCPP_VAR_ENABLED,
    NET_GCPP_VAR_REGISTERED,
    NET_GCPP_VAR_PROXY,
    NET_GCPP_VAR_PRINTER_NAME,
    NET_GCPP_VAR_DISPLAY_NAME,
    NET_GCPP_VAR_XMPP_JID,
    NET_GCPP_VAR_REFRESH_TOKEN,
    NET_GCPP_VAR_HTTP_PROXY_ENABLED,
    NET_GCPP_VAR_HTTP_PROXY_SERVER,
    NET_GCPP_VAR_HTTP_PROXY_PORT,
    NET_GCPP_VAR_HTTP_PROXY_AUTH,
    NET_GCPP_VAR_HTTP_PROXY_USERNAME,
    NET_GCPP_VAR_HTTP_PROXY_PASSWORD,

    // temp vars

    // add new vars above this line
    NET_GCPP_VAR_ALL
} net_gcpp_var_id_t;

typedef struct
{      
    bool enabled;
    bool registered;
    char proxy[GCPP_MAX_NAME_LEN];
    char printer_name[GCPP_MAX_NAME_LEN];
    char display_name[GCPP_MAX_NAME_LEN];
    char xmpp_jid[GCPP_MAX_AUTH_CREDENTIALS_LEN];
    char refresh_token[GCPP_MAX_AUTH_CREDENTIALS_LEN];
    bool http_proxy_enabled;
    char http_proxy_server[GCPP_MAX_NAME_LEN];
    uint32_t http_proxy_port;
    bool http_proxy_auth;
    char http_proxy_username[GCPP_MAX_NAME_LEN];
    char http_proxy_password[GCPP_MAX_NAME_LEN];
    
} net_gcpp_perm_vars_t;


//--------------------------------------
// Local Variables
//--------------------------------------

static bool g_net_gcpp_nvram_is_registered = false;
static nvram_handle_t *g_net_gcpp_nvram_hndl;
static net_gcpp_perm_vars_t g_net_gcpp_perm_vars;
//static TX_MUTEX g_net_gcpp_var_mutex;
static pthread_mutex_t g_net_gcpp_var_mutex;

static error_type_t net_gcpp_var_restore_defaults(net_gcpp_var_id_t var_id);
static error_type_t net_gcpp_nvram_block_write(void);
static error_type_t net_gcpp_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *context);


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_GCPP_VAR_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&g_net_gcpp_var_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define NET_GCPP_VAR_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&g_net_gcpp_var_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 


/** 
 *  \brief Boot-time initialization
 * 
 *  This function is called at system startup following initialization of the NVRAM driver and
 *  partition manager.  It initializes the module datastore variables to either their default values
 *  or values previously stored in NVRAM.
 **/ 
void net_gcpp_var_init(void)
{
    uint32_t pthread_rcode;
    error_type_t retval;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    pthread_rcode = posix_mutex_init(&g_net_gcpp_var_mutex);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating mutex (pthread_rcode=0x%02x)\n", pthread_rcode);
        goto error;
    }

    /*
     *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
     *  net_gcpp_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
     *  values.
     */
    retval = nvram_variable_register(&g_net_gcpp_nvram_hndl, GCPP_VARS_ID, NET_GCPP_VARS_VERSION, sizeof(net_gcpp_perm_vars_t), net_gcpp_nvram_init_cb, 0);
    if(retval != OK)
    {
        // NVRAM is not available so just set defaults and operate on the local copies       
        net_gcpp_var_restore_defaults(NET_GCPP_VAR_ALL);
    }
    else
    {
        g_net_gcpp_nvram_is_registered = true;
    }

error:
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return;
}

/** 
 *  \brief Restore one or all variables stored in the discrete dynamic NVRAM block to their default values
 * 
 *  This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 *  individual values when their default function is called.  
 *  
 *  \param var_id variable to initialize to default value, or NET_GCPP_VAR_ALL for all vars
 *  
 *  \return error_type_t
 *  \retval OK
 *  \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails
 **/ 
static error_type_t net_gcpp_var_restore_defaults(net_gcpp_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    all_vars = (var_id == NET_GCPP_VAR_ALL);

    NET_GCPP_VAR_LOCK();

    switch(var_id)
    {
    case NET_GCPP_VAR_ALL:
        // fall through

    case NET_GCPP_VAR_ENABLED:
        g_net_gcpp_perm_vars.enabled = true;
        if (!all_vars) break;

    case NET_GCPP_VAR_REGISTERED:
        g_net_gcpp_perm_vars.registered = false;
        if (!all_vars) break;

    case NET_GCPP_VAR_PROXY:
        g_net_gcpp_perm_vars.proxy[0] = '\0'; 
        if (!all_vars) break;

    case NET_GCPP_VAR_PRINTER_NAME:
        g_net_gcpp_perm_vars.printer_name[0] = '\0'; 
        if (!all_vars) break;

    case NET_GCPP_VAR_DISPLAY_NAME:
        g_net_gcpp_perm_vars.display_name[0] = '\0'; 
        if (!all_vars) break;

    case NET_GCPP_VAR_HTTP_PROXY_ENABLED:
        g_net_gcpp_perm_vars.http_proxy_enabled = false; 
        if (!all_vars) break;

    case NET_GCPP_VAR_HTTP_PROXY_SERVER:
        g_net_gcpp_perm_vars.http_proxy_server[0] = '\0'; 
        if (!all_vars) break;

    case NET_GCPP_VAR_HTTP_PROXY_PORT:
        g_net_gcpp_perm_vars.http_proxy_port = 0; 
        if (!all_vars) break;

    case NET_GCPP_VAR_HTTP_PROXY_AUTH:
        g_net_gcpp_perm_vars.http_proxy_auth = false; 
        if (!all_vars) break;

    case NET_GCPP_VAR_HTTP_PROXY_USERNAME:
        g_net_gcpp_perm_vars.http_proxy_username[0] = '\0'; 
        if (!all_vars) break;

    case NET_GCPP_VAR_HTTP_PROXY_PASSWORD:
        g_net_gcpp_perm_vars.http_proxy_password[0] = '\0'; 
        if (!all_vars) break;

    case NET_GCPP_VAR_XMPP_JID:
        g_net_gcpp_perm_vars.xmpp_jid[0] = '\0'; 
        if (!all_vars) break;

    case NET_GCPP_VAR_REFRESH_TOKEN:
        g_net_gcpp_perm_vars.refresh_token[0] = '\0'; 
        if (!all_vars) break;

    default:
        // do nothing
        break;
    }

    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return(retval);
}

static error_type_t net_gcpp_nvram_block_write(void)
{
    error_type_t retval = OK;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (g_net_gcpp_nvram_is_registered)
    {
        retval = nvram_set_var(g_net_gcpp_nvram_hndl, &g_net_gcpp_perm_vars);
        if (retval < 0)
        {
            retval = FAIL;
        }
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return retval;
}

/** 
 *  \brief Initialize or repair a discrete dynamic NVRAM block
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
 *  \return error_type_t
 *  \retval OK
 *
 **/ 
static error_type_t net_gcpp_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *context)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    NET_GCPP_VAR_LOCK();
    switch( init_type )
    {
        case NVRAM_NEW:
        case NVRAM_ERROR:
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_VERSION_CHANGE: // for now, set defaults, production code should migrate versions
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        {
            net_gcpp_var_restore_defaults(NET_GCPP_VAR_ALL);
            ASSERT(sizeof(net_gcpp_perm_vars_t) <= init_loc_size);
            memcpy(init_loc, &g_net_gcpp_perm_vars, sizeof(net_gcpp_perm_vars_t));
            break;
        }
        default:
        {
            ASSERT(NVRAM_OK == init_type);

            // read out current values
            memcpy(&g_net_gcpp_perm_vars, init_loc, sizeof(net_gcpp_perm_vars_t));
            break;
        }
    }
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return OK;
}


//=========================================================================================================================
// DATASTORE: GCPP configuration variables
//=========================================================================================================================


gcpp_error_t net_gcpp_get_var_enabled(bool *enabled)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {

        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }
   
    NET_GCPP_VAR_LOCK();
    *enabled = g_net_gcpp_perm_vars.enabled;
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

gcpp_error_t net_gcpp_set_var_enabled(bool enabled)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(g_net_gcpp_perm_vars.enabled == enabled)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    g_net_gcpp_perm_vars.enabled = enabled; 
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

gcpp_error_t net_gcpp_get_var_registered(bool *registered)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    *registered = g_net_gcpp_perm_vars.registered;
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}
gcpp_error_t net_gcpp_set_var_registered(bool registered)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(g_net_gcpp_perm_vars.registered == registered)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    g_net_gcpp_perm_vars.registered = registered; 
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// proxy is guaranteed not to be truncated if len <= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_get_var_proxy(char *proxy, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!proxy || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    proxy[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(proxy, g_net_gcpp_perm_vars.proxy, len);
    NET_GCPP_VAR_UNLOCK();

    proxy[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// proxy will be truncated if strlen >= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_set_var_proxy(const char *proxy)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!proxy)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.proxy, proxy, GCPP_MAX_NAME_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.proxy, 0, GCPP_MAX_NAME_LEN);
    strncpy(g_net_gcpp_perm_vars.proxy, proxy, GCPP_MAX_NAME_LEN);
    g_net_gcpp_perm_vars.proxy[GCPP_MAX_NAME_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// printer_name is guaranteed not to be truncated if len <= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_get_var_printer_name(char *printer_name, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!printer_name || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    printer_name[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(printer_name, g_net_gcpp_perm_vars.printer_name, len);
    NET_GCPP_VAR_UNLOCK();

    printer_name[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// printer_name will be truncated if strlen >= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_set_var_printer_name(const char *printer_name)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!printer_name)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.printer_name, printer_name, GCPP_MAX_NAME_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.printer_name, 0, GCPP_MAX_NAME_LEN);
    strncpy(g_net_gcpp_perm_vars.printer_name, printer_name, GCPP_MAX_NAME_LEN);
    g_net_gcpp_perm_vars.printer_name[GCPP_MAX_NAME_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// display_name is guaranteed not to be truncated if len <= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_get_var_display_name(char *display_name, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!display_name || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    display_name[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(display_name, g_net_gcpp_perm_vars.display_name, len);
    NET_GCPP_VAR_UNLOCK();

    display_name[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// display_name will be truncated if strlen >= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_set_var_display_name(const char *display_name)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!display_name)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.display_name, display_name, GCPP_MAX_NAME_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.display_name, 0, GCPP_MAX_NAME_LEN);
    strncpy(g_net_gcpp_perm_vars.display_name, display_name, GCPP_MAX_NAME_LEN);
    g_net_gcpp_perm_vars.display_name[GCPP_MAX_NAME_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// xmpp_jid is guaranteed not to be truncated if len <= GCPP_MAX_AUTH_CREDENTIALS_LEN
gcpp_error_t net_gcpp_get_var_xmpp_jid(char *xmpp_jid, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!xmpp_jid || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    xmpp_jid[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(xmpp_jid, g_net_gcpp_perm_vars.xmpp_jid, len);
    NET_GCPP_VAR_UNLOCK();

    xmpp_jid[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// xmpp_jid will be truncated if strlen >= GCPP_MAX_AUTH_CREDENTIALS_LEN
gcpp_error_t net_gcpp_set_var_xmpp_jid(const char *xmpp_jid)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!xmpp_jid)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.xmpp_jid, xmpp_jid, GCPP_MAX_AUTH_CREDENTIALS_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.xmpp_jid, 0, GCPP_MAX_AUTH_CREDENTIALS_LEN);
    strncpy(g_net_gcpp_perm_vars.xmpp_jid, xmpp_jid, GCPP_MAX_AUTH_CREDENTIALS_LEN);
    g_net_gcpp_perm_vars.xmpp_jid[GCPP_MAX_AUTH_CREDENTIALS_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// refresh token is guaranteed not to be truncated if len <= GCPP_MAX_AUTH_CREDENTIALS_LEN
gcpp_error_t net_gcpp_get_var_refresh_token(char *refresh_token, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!refresh_token || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    refresh_token[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(refresh_token, g_net_gcpp_perm_vars.refresh_token, len);
    NET_GCPP_VAR_UNLOCK();

    refresh_token[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// refresh_token will be truncated if strlen >= GCPP_MAX_AUTH_CREDENTIALS_LEN
gcpp_error_t net_gcpp_set_var_refresh_token(const char *refresh_token)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!refresh_token)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.refresh_token, refresh_token, GCPP_MAX_AUTH_CREDENTIALS_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.refresh_token, 0, GCPP_MAX_AUTH_CREDENTIALS_LEN);
    strncpy(g_net_gcpp_perm_vars.refresh_token, refresh_token, GCPP_MAX_AUTH_CREDENTIALS_LEN);
    g_net_gcpp_perm_vars.refresh_token[GCPP_MAX_AUTH_CREDENTIALS_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

gcpp_error_t net_gcpp_get_var_http_proxy_enabled(bool *enabled)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {

        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }
   
    NET_GCPP_VAR_LOCK();
    *enabled = g_net_gcpp_perm_vars.http_proxy_enabled;
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

gcpp_error_t net_gcpp_set_var_http_proxy_enabled(bool enabled)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(g_net_gcpp_perm_vars.http_proxy_enabled == enabled)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    g_net_gcpp_perm_vars.http_proxy_enabled = enabled; 
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_server is guaranteed not to be truncated if len <= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_get_var_http_proxy_server(char *server, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!server || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    server[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(server, g_net_gcpp_perm_vars.http_proxy_server, len);
    NET_GCPP_VAR_UNLOCK();

    server[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_server will be truncated if strlen >= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_set_var_http_proxy_server(const char *server)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!server)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.http_proxy_server, server, GCPP_MAX_NAME_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.http_proxy_server, 0, GCPP_MAX_NAME_LEN);
    strncpy(g_net_gcpp_perm_vars.http_proxy_server, server, GCPP_MAX_NAME_LEN);
    g_net_gcpp_perm_vars.http_proxy_server[GCPP_MAX_NAME_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_port is guaranteed not to be truncated if len <= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_get_var_http_proxy_port(uint32_t *proxy_port)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {

        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }
   
    NET_GCPP_VAR_LOCK();
    *proxy_port = g_net_gcpp_perm_vars.http_proxy_port;
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_port will be truncated if strlen >= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_set_var_http_proxy_port(uint32_t proxy_port)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(g_net_gcpp_perm_vars.http_proxy_port == proxy_port)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    g_net_gcpp_perm_vars.http_proxy_port = proxy_port; 
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}


gcpp_error_t net_gcpp_get_var_http_proxy_auth(bool *auth)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {

        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }
   
    NET_GCPP_VAR_LOCK();
    *auth = g_net_gcpp_perm_vars.http_proxy_auth;
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

gcpp_error_t net_gcpp_set_var_http_proxy_auth(bool auth)
{
    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(g_net_gcpp_perm_vars.http_proxy_auth == auth)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    g_net_gcpp_perm_vars.http_proxy_auth = auth; 
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_username is guaranteed not to be truncated if len <= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_get_var_http_proxy_username(char *username, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!username || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    username[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(username, g_net_gcpp_perm_vars.http_proxy_username, len);
    NET_GCPP_VAR_UNLOCK();

    username[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_username will be truncated if strlen >= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_set_var_http_proxy_username(const char *username)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!username)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.http_proxy_username, username, GCPP_MAX_NAME_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.http_proxy_username, 0, GCPP_MAX_NAME_LEN);
    strncpy(g_net_gcpp_perm_vars.http_proxy_username, username, GCPP_MAX_NAME_LEN);
    g_net_gcpp_perm_vars.http_proxy_username[GCPP_MAX_NAME_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_password is guaranteed not to be truncated if len <= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_get_var_http_proxy_password(char *password, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!password || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    password[0] = '\0';

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    strncpy(password, g_net_gcpp_perm_vars.http_proxy_password, len);
    NET_GCPP_VAR_UNLOCK();

    password[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// http_proxy_password will be truncated if strlen >= GCPP_MAX_NAME_LEN
gcpp_error_t net_gcpp_set_var_http_proxy_password(const char *password)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!password)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    net_gcpp_context_t *net_gcpp_ctxt = gcpp_get_context();
    if(!net_gcpp_ctxt || !net_gcpp_ctxt->initialized)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_NOT_INITIALIZED;
    }

    NET_GCPP_VAR_LOCK();
    if(strncmp(g_net_gcpp_perm_vars.http_proxy_password, password, GCPP_MAX_NAME_LEN) == 0)
    {
        // no change - ignore request
        NET_GCPP_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_net_gcpp_perm_vars.http_proxy_password, 0, GCPP_MAX_NAME_LEN);
    strncpy(g_net_gcpp_perm_vars.http_proxy_password, password, GCPP_MAX_NAME_LEN);
    g_net_gcpp_perm_vars.http_proxy_password[GCPP_MAX_NAME_LEN-1] = '\0';
    net_gcpp_nvram_block_write();
    NET_GCPP_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}
