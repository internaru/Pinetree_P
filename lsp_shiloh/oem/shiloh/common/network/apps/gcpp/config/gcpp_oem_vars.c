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
#include <time.h>
#include "error_types.h"
#include "dprintf.h"
#include "lassert.h"
#include "logger.h"
#include "nvram_api.h"
//#include "net_app_api.h"
#include "gcpp_oem_config.h"
#include "net_gcpp_api.h"
#include "posix_ostools.h"

// values which reside in dynamic permanent storage
// IMPORTANT:  increment GCPP_OEM_VARS_VERSION whenever this structure changes
#define GCPP_OEM_VARS_VERSION       1
#define GCPP_OEM_VARS_ID            "GOEM"
typedef enum
{
    // perm vars
    GCPP_OEM_VAR_REG_DATE_TIME,
    GCPP_OEM_VAR_PRINT_OUTPUT,
    // temp vars

    // add new vars above this line
    GCPP_OEM_VAR_ALL
} gcpp_oem_var_id_t;

typedef struct
{      
    char  reg_date_time[GCPP_REG_DATE_TIME_MAX_LEN];
    uint8_t print_output;
    
} gcpp_oem_perm_vars_t;


//--------------------------------------
// Local Variables
//--------------------------------------

static bool g_gcpp_oem_nvram_is_registered = false;
static nvram_handle_t *g_gcpp_oem_nvram_hndl;
static gcpp_oem_perm_vars_t g_gcpp_oem_perm_vars;
static pthread_mutex_t g_gcpp_oem_var_mutex;

static error_type_t gcpp_oem_var_restore_defaults(gcpp_oem_var_id_t var_id);
static error_type_t gcpp_oem_nvram_block_write(void);
static error_type_t gcpp_oem_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *context);


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define GCPP_OEM_VAR_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&g_gcpp_oem_var_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_OEM_VAR_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&g_gcpp_oem_var_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 


/** 
 *  \brief Boot-time initialization
 * 
 *  This function is called at system startup following initialization of the NVRAM driver and
 *  partition manager.  It initializes the module datastore variables to either their default values
 *  or values previously stored in NVRAM.
 **/ 
void gcpp_oem_var_init(void)
{
    uint32_t pthread_rcode;
    error_type_t retval;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    pthread_rcode = posix_mutex_init(&g_gcpp_oem_var_mutex);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating mutex (pthread_rcode=0x%02x)\n", pthread_rcode);
        goto error;
    }

    /*
     *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
     *  gcpp_oem_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
     *  values.
     */
    retval = nvram_variable_register(&g_gcpp_oem_nvram_hndl, GCPP_OEM_VARS_ID, GCPP_OEM_VARS_VERSION, sizeof(gcpp_oem_perm_vars_t), gcpp_oem_nvram_init_cb, 0);
    if(retval != OK)
    {
        // NVRAM is not available so just set defaults and operate on the local copies       
        gcpp_oem_var_restore_defaults(GCPP_OEM_VAR_ALL);
    }
    else
    {
        g_gcpp_oem_nvram_is_registered = true;
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
static error_type_t gcpp_oem_var_restore_defaults(gcpp_oem_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    all_vars = (var_id == GCPP_OEM_VAR_ALL);

    GCPP_OEM_VAR_LOCK();

    switch(var_id)
    {
    case GCPP_OEM_VAR_ALL:
        // fall through

    case GCPP_OEM_VAR_REG_DATE_TIME:
        memset(g_gcpp_oem_perm_vars.reg_date_time, 0x00, GCPP_REG_DATE_TIME_MAX_LEN);
        g_gcpp_oem_perm_vars.reg_date_time[0] = '\0'; 

        if (!all_vars) break;

    case GCPP_OEM_VAR_PRINT_OUTPUT:
        g_gcpp_oem_perm_vars.print_output = 0;

        if (!all_vars) break;

    default:
        // do nothing
        break;
    }

    GCPP_OEM_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return(retval);
}

static error_type_t gcpp_oem_nvram_block_write(void)
{
    error_type_t retval = OK;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (g_gcpp_oem_nvram_is_registered)
    {
        retval = nvram_set_var(g_gcpp_oem_nvram_hndl, &g_gcpp_oem_perm_vars);
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
static error_type_t gcpp_oem_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *context)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    GCPP_OEM_VAR_LOCK();
    switch( init_type )
    {
        case NVRAM_NEW:
        case NVRAM_ERROR:
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        {
            gcpp_oem_var_restore_defaults(GCPP_OEM_VAR_ALL);
            ASSERT(sizeof(gcpp_oem_perm_vars_t) <= init_loc_size);
            memcpy(init_loc, &g_gcpp_oem_perm_vars, sizeof(gcpp_oem_perm_vars_t));
            break;
        }

        case NVRAM_VERSION_CHANGE:     /**< Version change */

            // Read out current values from the previous map, so they will retain their values
            memcpy(&g_gcpp_oem_perm_vars, init_loc, sizeof(gcpp_oem_perm_vars_t));

			// Now, go through all versions between previous version and current version, updating the newly
			// added values with a valid default value.
			switch (version)	// Version contains the original NVRam version before the change
			{
				case 0:
					// do nothing for these - version 1 was the version when we implemented this change.
					
// Next time the version rolls, ucomment this 'case ', and add the new fields here.					
//				case 1:	// these fields were added after version 1
//					g_gcpp_oem_perm_vars.placeholder = 0;

					/* IMPORTANT: Only break after all the cases have been walked through to get us to latest */
					break;
					
				default:	
					if (GCPP_OEM_VARS_VERSION < version)
					{
						DBG_ERR("I think we're using older firmware. Current_NV: %d  Prev_NV:\n", GCPP_OEM_VARS_VERSION, version);
					}
					else
					{
						DBG_ERR("Make sure to handle newly added NVRam locations. Old: %d  Current: %d\n", version, GCPP_OEM_VARS_VERSION);
						ASSERT(0);	// ASSERT here to make sure newly added fields are accounted for above.
					}
					break;
			}	
			break;

        default:
        {
            ASSERT(NVRAM_OK == init_type);

            // read out current values
            memcpy(&g_gcpp_oem_perm_vars, init_loc, sizeof(gcpp_oem_perm_vars_t));
            break;
        }
    }
    GCPP_OEM_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return OK;
}


//=========================================================================================================================
// DATASTORE: GCPP configuration variables
//=========================================================================================================================


// reg_date_time is guaranteed not to be truncated if len <= GCPP_OEM_REG_DATE_TIME_LEN
gcpp_error_t gcpp_oem_get_var_reg_date_time(char *reg_date_time, uint32_t len)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!reg_date_time || !len)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    reg_date_time[0] = '\0';

    GCPP_OEM_VAR_LOCK();
    strncpy(reg_date_time, g_gcpp_oem_perm_vars.reg_date_time, len);
    GCPP_OEM_VAR_UNLOCK();

    reg_date_time[len-1] = '\0';

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

// reg_date_time will be truncated if strlen >= GCPP_OEM_REG_DATE_TIME_MAX_LEN
gcpp_error_t gcpp_oem_set_var_reg_date_time(const char *reg_date_time)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if(!reg_date_time)
    { 
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    GCPP_OEM_VAR_LOCK();
    if(strncmp(g_gcpp_oem_perm_vars.reg_date_time, reg_date_time, GCPP_REG_DATE_TIME_MAX_LEN) == 0)
    {
        // no change - ignore request
        GCPP_OEM_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    memset(g_gcpp_oem_perm_vars.reg_date_time, 0, GCPP_REG_DATE_TIME_MAX_LEN);
    strncpy(g_gcpp_oem_perm_vars.reg_date_time, reg_date_time, GCPP_REG_DATE_TIME_MAX_LEN);
    g_gcpp_oem_perm_vars.reg_date_time[GCPP_REG_DATE_TIME_MAX_LEN-1] = '\0';
    gcpp_oem_nvram_block_write();
    GCPP_OEM_VAR_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

gcpp_error_t gcpp_oem_get_var_print_output(uint8_t *print_output)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if(!print_output)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_FAIL;
    }
   
    GCPP_OEM_VAR_LOCK();
    *print_output = g_gcpp_oem_perm_vars.print_output;
    GCPP_OEM_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

gcpp_error_t gcpp_oem_set_var_print_output(uint8_t print_output)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    GCPP_OEM_VAR_LOCK();
    if(g_gcpp_oem_perm_vars.print_output == print_output)
    {
        // no change - ignore request
        GCPP_OEM_VAR_UNLOCK();
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return NET_GCPP_SUCCESS;
    }

    g_gcpp_oem_perm_vars.print_output = print_output; 
    gcpp_oem_nvram_block_write();
    GCPP_OEM_VAR_UNLOCK();

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

