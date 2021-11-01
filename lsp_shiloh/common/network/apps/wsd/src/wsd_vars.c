/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * \file wsd_vars.c
 *
 * \brief This file contains the variables to configure WSD
 * 
 **/

//--------------------------------------
// Included files
//--------------------------------------

#include <string.h>
#include "memAPI.h"
#include "lassert.h"
#include "logger.h"
#define DBG_PRFX "WSD.var: "
#include "debug.h"
#include "dprintf.h"
#include "sys_init_api.h"
#include "error_types.h"
#include "platform.h"
#include "nvram_api.h"
#include "net_iface_vars.h"
#include "net_wsd_vars.h"
#include "wsd.h"
#include "uuid.h"

#include "net_iface.h" // net_interface_t*

//--------------------------------------
// Local Definitions and Types
//--------------------------------------
#define ALL_WSD_VARS 0xff
#define SCANNER_NAME_LENGTH     64 // set arbitrly

#define WSD_DEFAULT_SCANNER_NAME      "WSD-Enabled Scanner"

// values which reside in dynamic permanent storage
// IMPORTANT:  increment WSD_VARS_VERSION whenever this structure changes
#define WSD_VARS_VERSION 4

typedef enum
{
    // perm vars
    WSD_VAR_ENABLED,
    WSD_VAR_META_DATA_CHECKSUM,
    WSD_VAR_META_VERSION,
    WSD_VAR_SCANNER_NAME,

    // temp vars - none

    // add new vars above this line
    WSD_NUM_VARS
} wsd_var_id_t;

typedef struct
{                            
    bool        enabled;
    uint32_t    meta_data_checksum;
    uint32_t    meta_version;
    char        scanner_name[ SCANNER_NAME_LENGTH ];
} wsd_perm_vars_t;


//--------------------------------------
// Global Definitions
//--------------------------------------
static bool             wsd_init_done = false;
static bool             wsd_perm_local_only = false; // set to true if NVRAM is not working for some reason
static char             *sem_name = "/wsd_ds";
static nvram_handle_t   *wsd_nvram_hndl;
static wsd_perm_vars_t  wsd_perm_vars;
static sem_t            wsd_var_sem[WSD_NUM_VARS];

//--------------------------------------
// Local Function Declarations
//--------------------------------------
static error_type_t wsd_nvram_init( nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused );
static void wsd_perm_var_init();
static void wsd_temp_var_init( void );
static error_type_t wsd_set_perm_var_defaults( int id, bool do_write );
void populate_uuid_sections( char *uuid, char *serial, uint8_t *mac );
//--------------------------------------
// Local Function Definitions
//--------------------------------------

// local utility/helper routines

// FUNCTION NAME:  wsd_var_init
/** 
 * \brief Boot time initialization function for the wsd variables.
 * 
 * This function is called at system startup after the NVRAM driver and partition manager have been
 * initialized.  It sets all the sd variables to either default values or previously stored values
 * from NVRAM.
 * 
 **/ 
void net_wsd_var_init( void )
{
    int i;

    // create semaphores for all variables

    for (i = 0; i < WSD_NUM_VARS; ++i)
    {
        wsd_var_sem[i] = *(posix_sem_open(sem_name, 1));
        //REL_ASSERT( tx_semaphore_create( &wsd_var_sem[i], sem_name, 1 ) == TX_SUCCESS );
    }

    // create mutex to keep groups of variables in sync
    wsd_perm_var_init();  // initialize items stored in dynamic NVRAM next (may depend on static NVRAM items)
    wsd_temp_var_init();  // initialize items stored in RAM last (may depend on static and/or dynamic NVRAM items)
    wsd_init_done = true;

    // enable wsd listeners for interface(s) of interest
    if ( wsd_perm_vars.enabled )
    {
        wsd_set_enabled(); // trigger listener init and HELLO
    }

    return;
}

// FUNCTION NAME:  wsd_nvram_init
/** 
 * \brief Function called by the dynamic NVRAM driver to initialize or repair a block.
 * 
 * This function is called by the dynamic NVRAM driver during an NvramVariableRegister()
 * request.  It will be called either to initialize a new block of data or to repair an
 * existing block which has a bad CRC, version discrepancy, or change in size. 
 * 
 * \param[in,out] dataLoc pointer to existing block data on entry, destination for new
 * block data to be returned 
 * 
 * \return error_type_t
 * \retval WSD_VARS_OK
 *
 **/ 
static error_type_t wsd_nvram_init(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused )
{
    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            wsd_set_perm_var_defaults( ALL_WSD_VARS, false );  // do NOT write values to NVRAM
            ASSERT(sizeof(wsd_perm_vars_t) <= init_loc_size);
            memcpy( init_loc, &wsd_perm_vars, sizeof( wsd_perm_vars_t ) );
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(&wsd_perm_vars, init_loc, sizeof(wsd_perm_vars_t));
            break;
    }
    return WSD_VARS_OK ;
}

// FUNCTION NAME:  wsd_perm_var_init
/** 
 * \brief Boot time initialization function for the wsd variables stored in dynamic NVRAM.
 * 
 * This function is called at system startup to read the variables out of dynamic NVRAM.  These
 * values are updated as appropriate if an e_DefaultAllValues or e_DefaultUserSettableValues reset
 * is specified.
 * 
 **/ 
static void wsd_perm_var_init( void )
{
    error_type_t retval = FAIL; // internal use only

    if( wsd_init_done == false )
    {
        // ask for block - if it doesn't exist or is corrupted, wsd_nvram_init() gets called by nvram_variable_register()
        retval = nvram_variable_register(&wsd_nvram_hndl, NET_WSD_VARS_ID, WSD_VARS_VERSION, sizeof(wsd_perm_vars_t), wsd_nvram_init, NULL);
    }

    if( retval != OK )
    {
        // there is something seriously wrong with NVRAM, so just set defaults and use the local copy       
        wsd_set_perm_var_defaults( ALL_WSD_VARS, false );  // in this case do NOT write values to NVRAM
        wsd_perm_local_only = true;
    }

    return;
}

// FUNCTION NAME:  wsd_temp_var_init
/** 
 * \brief Boot time initialization function for the wsd variables stored in RAM.
 * 
 * This function is called at system startup to initialize the RAM based variables.  These
 * are either set to default values or initialized from a corresponding NVRAM based variable.
 * These values are not affected by any type of reset since they are already default values or
 * else based on values that have already been set to their default value.
 * 
 **/ 
static void wsd_temp_var_init( void )
{
    // no temp vars exist
    return;
}

// FUNCTION NAME:  wsd_set_perm_var_defaults
/** 
 * \brief Function to initialize one or all variables stored in dynamic NVRAM.
 * 
 * This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 * individual values when their default function is called.  The individual API functions may be called
 * from outside the system and are required to write the new values to NVRAM.  This function is
 * intended to provide the ability to simply set the local copy without writing it out (useful for
 * initial block setup by the NVRAM driver and in the unlikely event that NVRAM is not working).
 * 
 * \param[in] id variable to initialize to default value (valid id's are listed under the "perm vars"
 * heading in the wsd_var_id_t enum - ALL_WSD_VARS refers to all perm vars)
 *
 * \param[in] doWrite true if value is to be written to NVRAM, false otherwise 
 * 
 * \return error_type_t
 * \retval WSD_VARS_OK
 * \retval WSD_VARS_FAIL if write fails
 *
 **/ 
static error_type_t wsd_set_perm_var_defaults( int id, bool do_write )
{
    bool all_vars;
    error_type_t retval = WSD_VARS_OK;
    net_wsd_config_t wsd_config_vals;
    net_wsd_config( &wsd_config_vals );

    all_vars = ( id == ALL_WSD_VARS) ? true : false;

    if( all_vars == false )
    {
        while (sem_wait(&wsd_var_sem[id]) != 0 )
        {
            ; //do nothing but wait
        }
        //REL_ASSERT(tx_semaphore_get( &wsd_var_sem[id], TX_WAIT_FOREVER) == TX_SUCCESS);
    }

    switch( id )
    {
        case ALL_WSD_VARS:
        case WSD_VAR_ENABLED:
            wsd_perm_vars.enabled = wsd_config_vals.sys_restore_wsd_enabled;
            if ( all_vars == false )    break;
        case WSD_VAR_META_DATA_CHECKSUM:
            wsd_perm_vars.meta_data_checksum = 0;
            if ( all_vars == false )    break;
        case WSD_VAR_META_VERSION:
            wsd_perm_vars.meta_version = 0;
            if ( all_vars == false )    break;
        case WSD_VAR_SCANNER_NAME:
            {
              memset(wsd_perm_vars.scanner_name, 0, sizeof(wsd_perm_vars.scanner_name));
              strcpy(wsd_perm_vars.scanner_name, WSD_DEFAULT_SCANNER_NAME);
            }
            if ( all_vars == false )    break;
        default:
            // do nothing
            break;
    }

    if( do_write && ( wsd_perm_local_only == false ) )
    {
        if( nvram_set_var( wsd_nvram_hndl, &wsd_perm_vars ) < 0 )
        {
            retval = WSD_VARS_FAIL;
        }
    }

    if( all_vars == false )
    {
        sem_post(&wsd_var_sem[id]);
        //REL_ASSERT( tx_semaphore_put( &wsd_var_sem[ id ] ) == TX_SUCCESS );
    }

    return retval;
}


//=========================================================================================================================
// DATASTORE: WSD configuration variables
//=========================================================================================================================

error_type_t wsd_get_var_meta_data_checksum( uint32_t *checksum )
{
    UINT threadx_rcode;
    
    threadx_rcode = sem_wait(&wsd_var_sem[WSD_VAR_META_DATA_CHECKSUM]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    *checksum = wsd_perm_vars.meta_data_checksum;

    threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_META_DATA_CHECKSUM ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    return WSD_VARS_OK;
}

error_type_t wsd_set_var_meta_data_checksum( uint32_t checksum )
{
    UINT threadx_rcode;
    threadx_rcode = sem_wait( &wsd_var_sem[ WSD_VAR_META_DATA_CHECKSUM ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    if( wsd_perm_vars.meta_data_checksum == checksum )
    {
        // no change - ignore request
        threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_META_DATA_CHECKSUM ] );
        return WSD_VARS_OK;
    }

    wsd_perm_vars.meta_data_checksum = checksum; 
    if( wsd_perm_local_only == false )
    {
        if( nvram_set_var( wsd_nvram_hndl, &wsd_perm_vars ) < OK )
        {
            threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_META_DATA_CHECKSUM ] );
            return WSD_VARS_FAIL;
        }
    }

    threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_META_DATA_CHECKSUM ] );
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    return WSD_VARS_OK;
}


error_type_t wsd_get_var_meta_version( uint32_t *meta_version )
{
    UINT threadx_rcode;
    threadx_rcode = sem_wait( &wsd_var_sem[ WSD_VAR_META_VERSION ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    *meta_version = wsd_perm_vars.meta_version;

    threadx_rcode = sem_post(&wsd_var_sem[ WSD_VAR_META_VERSION ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    return WSD_VARS_OK;
}

error_type_t wsd_set_var_meta_version( uint32_t meta_version )
{
    UINT threadx_rcode;
    threadx_rcode = sem_wait( &wsd_var_sem[ WSD_VAR_META_VERSION ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    if( wsd_perm_vars.meta_version == meta_version )
    {
        // no change - ignore request
        threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_META_VERSION ] );
        return WSD_VARS_OK;
    }

    wsd_perm_vars.meta_version = meta_version; 
    if( wsd_perm_local_only == false )
    {
        if( nvram_set_var( wsd_nvram_hndl, &wsd_perm_vars ) < OK )
        {
            threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_META_VERSION ] );
            return WSD_VARS_FAIL;
        }
    }

    threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_META_VERSION ] );
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    return WSD_VARS_OK;
}

error_type_t net_wsd_get_var_enabled( bool *enabled )
{
    UINT threadx_rcode;
    threadx_rcode = sem_wait( &wsd_var_sem[ WSD_VAR_ENABLED ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    *enabled = wsd_perm_vars.enabled;

    threadx_rcode = sem_post(&wsd_var_sem[ WSD_VAR_ENABLED ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    return WSD_VARS_OK;
}


error_type_t net_wsd_set_var_enabled( bool enabled )
{
    UINT threadx_rcode;
    threadx_rcode = sem_wait( &wsd_var_sem[ WSD_VAR_ENABLED ]);
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    if( wsd_perm_vars.enabled == enabled )
    {
        // no change - ignore request
        threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_ENABLED ] );
        return WSD_VARS_OK;
    }

    wsd_perm_vars.enabled = enabled; 
    if( wsd_perm_local_only == false )
    {
        if( nvram_set_var( wsd_nvram_hndl, &wsd_perm_vars ) < OK )
        {
            threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_ENABLED ] );
            return WSD_VARS_FAIL;
        }
    }

    threadx_rcode = sem_post( &wsd_var_sem[ WSD_VAR_ENABLED ] );
    if ( threadx_rcode != 0 )  return WSD_VARS_FAIL;

    // Initiate WSD shutdown process (if disabling)
    if ( enabled == false )
    {
        DBG_ERR("%s() BYE..\n", __func__);
        dpws_stop_server( 1000 ); // blocking TIME_FOR_BYE_TO_BE_SENT_IN_MS
        wsd_clear_flags( NULL, WSD_FLAG_ENABLED );
    }
    else
    {
        wsd_set_flags( NULL, WSD_FLAG_ENABLED );
    }

    return WSD_VARS_OK;
}

error_type_t net_wsd_get_var_uuid( char *uuid, uint32_t length )
{
    return get_device_uuid_string(uuid, length);
}

// eof wsd_vars.c
