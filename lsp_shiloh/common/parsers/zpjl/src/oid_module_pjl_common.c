/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_pjl_common.c
 *
 * \brief This file implements the OID back-end functions for the
 *  PJL printer job language variables.
 *  Notice that the underlying variables are NOT exposed.
 *  This file will also initialize the oem portions of PJL in 
 *  oid_module_pjl_oem.c
 **/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "data_access_api.h"
#include "nvram_api.h"
#include "posix_ostools.h"
#include "lassert.h"
#include "logger.h"


#define DBG_PRFX "pjloidcommon: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(9)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

/// nvram version
#define var_pjl_common_version 1

static nvram_handle_t *nvram_handle = 0;
static pthread_mutex_t var_pjl_lock = POSIX_MUTEX_INITIALIZER;

typedef struct var_pjl_common_s
{
	uint32_t default_copies;  // 1 - 999  zero is not supported.
	uint32_t default_duplex;  // false, true
	uint32_t default_binding; // 0 == longedge, 1 == shortedge
	uint32_t default_collated; // false, true 
} var_pjl_common_t;

static var_pjl_common_t var_pjl_common_factory_default = {
	.default_copies = 1,
	.default_duplex = 0,
	.default_binding = 0,
	.default_collated = 0,
};

static var_pjl_common_t var_pjl_common;


inline uint32_t var_pjl_common_get_default_copies()
{
	uint32_t value;
	pthread_mutex_lock( &var_pjl_lock );

	value = var_pjl_common.default_copies;

	pthread_mutex_unlock( &var_pjl_lock );
	return value;
}

inline void var_pjl_common_set_default_copies( uint32_t value )
{
	pthread_mutex_lock( &var_pjl_lock );

	var_pjl_common.default_collated = false;
	var_pjl_common.default_copies = value > 999 ? 999 : value < 1 ? 1 : value;
	nvram_set_var(nvram_handle, &var_pjl_common);

	pthread_mutex_unlock( &var_pjl_lock );
}

inline uint32_t var_pjl_common_get_default_collated_copies()
{
	uint32_t value;
	pthread_mutex_lock( &var_pjl_lock );

	value = var_pjl_common.default_collated ? var_pjl_common.default_copies : 0;

	pthread_mutex_unlock( &var_pjl_lock );
	return value;
}

inline void var_pjl_common_set_default_collated_copies( uint32_t value )
{
	pthread_mutex_lock( &var_pjl_lock );

	var_pjl_common.default_collated = value > 1 ? true : false;
	
	var_pjl_common.default_copies = value > 999 ? 999 : value < 1 ? 1 : value;

	nvram_set_var(nvram_handle, &var_pjl_common);

	pthread_mutex_unlock( &var_pjl_lock );
}


inline uint32_t var_pjl_common_get_default_duplex()
{
	uint32_t value;
	pthread_mutex_lock( &var_pjl_lock );

	value = var_pjl_common.default_duplex;

	pthread_mutex_unlock( &var_pjl_lock );
	return value;
}

inline void var_pjl_common_set_default_duplex( uint32_t value )
{
	pthread_mutex_lock( &var_pjl_lock );

	/// is duplex supported ? 
//	var_pjl_common.default_copies = value;  // boolean 
	var_pjl_common.default_duplex = value;  // boolean // simple bug @@ 2013.03.28	by Juny
	nvram_set_var(nvram_handle, &var_pjl_common);

	pthread_mutex_unlock( &var_pjl_lock );
}

inline uint32_t var_pjl_common_get_default_binding()
{
	uint32_t value;
	pthread_mutex_lock( &var_pjl_lock );

	value = var_pjl_common.default_binding;

	pthread_mutex_unlock( &var_pjl_lock );
	return value;
}

inline void var_pjl_common_set_default_binding( uint32_t value )
{
	pthread_mutex_lock( &var_pjl_lock );

	/// is duplex supported ? 
	var_pjl_common.default_binding = value;
	nvram_set_var(nvram_handle, &var_pjl_common);

	pthread_mutex_unlock( &var_pjl_lock );
}




static error_type_t oid_module_pjl_get_int( oid_t oid,
					    uint32_t index,
					    uint32_t * value)
{
	error_type_t e_res = OK;

	switch ( oid )
	{
        case OID_PJL_DEFAULT_COPIES:
		DBG_PRINTF_CRIT("%s get copies %d\n", __FUNCTION__, value);	// @@ fix		2013.07.17	by Juny 
		*value = var_pjl_common_get_default_copies();
		break;
	case OID_PJL_DEFAULT_COLLATED_COPIES:
		*value = var_pjl_common_get_default_collated_copies();
		break;
        case OID_PJL_DEFAULT_DUPLEX:
		*value = var_pjl_common_get_default_duplex();
		break;
        case OID_PJL_DEFAULT_BINDING:
		*value = var_pjl_common_get_default_binding();
		break;
	default:
		e_res = FAIL;  // not found
	}
	return e_res;
}

static error_type_t oid_module_pjl_set_int( oid_t oid,
					    uint32_t index,
					    uint32_t value)
{
	error_type_t e_res = OK;

	switch ( oid )
	{
        case OID_PJL_DEFAULT_COPIES:
		DBG_PRINTF_CRIT("%s set copies %d\n", __FUNCTION__, value);	// @@ fix		2013.07.17	by Juny 
		var_pjl_common_set_default_copies( value );
		break;
	case OID_PJL_DEFAULT_COLLATED_COPIES:
		var_pjl_common_set_default_collated_copies( value );
		break;
        case OID_PJL_DEFAULT_DUPLEX:
		var_pjl_common_set_default_duplex( value );	// @@ fix		2013.07.17	by Juny 
		break;
        case OID_PJL_DEFAULT_BINDING:
		var_pjl_common_set_default_binding( value );
		break;
	default:
		e_res = FAIL;  // not found
	}

	return e_res;
}

#if 0
static error_type_t oid_module_pjl_get_str( oid_t oid,
					    uint32_t index,
					    char * data_ptr,
					    uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = OK;

    switch ( oid )
    {
        case OID_PJL_DEFAULT_COPIES:
		*data_ptr = var_pjl_common_get_default_copies();
		break;
	default:
		e_res = FAIL; // not found
    }
    return e_res;
}
#endif

static error_type_t pjl_common_vars_init_callback(nvram_init_type_t InitType, uint16_t Version, void *InitLocation, uint32_t InitLocationSize, void *unused)
{
	switch( InitType )
	{
        case NVRAM_VERSION_CHANGE:     /**< Version change */
		DBG_ASSERT( Version == var_pjl_common_version ); /// implement version migration
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
		ASSERT(sizeof(var_pjl_common_t) <= InitLocationSize);
		memcpy(&var_pjl_common, &var_pjl_common_factory_default, sizeof(var_pjl_common_t));
		memcpy(InitLocation, &var_pjl_common, sizeof(var_pjl_common_t));
		break;

        case NVRAM_OK: 
		// read out current values
		memcpy(&var_pjl_common, InitLocation, sizeof(var_pjl_common_t));
            break;
        default:
		ASSERT( NVRAM_OK == InitType );  // undefined init type
    }
    return ( OK );
}

#ifndef UNUSED_VAR
/*!
 * \def UNUSED_VAR(x) 
 * a simple macro that will turn off the unused var compiler warning for a given var
 **/
#define UNUSED_VAR(x) ((x) = (x))  
#endif


static void oid_register_module_pjl_common( void )
{
    error_type_t error = 0; UNUSED_VAR(error);

    error = oid_register_callbacks( OID_PJL_DEFAULT_COPIES,
				    oid_module_pjl_get_int,
				    oid_module_pjl_set_int,
				    oid_module_null_test ); 
    error = oid_register_callbacks( OID_PJL_DEFAULT_COLLATED_COPIES,
				    oid_module_pjl_get_int,
				    oid_module_pjl_set_int,
				    oid_module_null_test ); 
    error = oid_register_callbacks( OID_PJL_DEFAULT_DUPLEX,
				    oid_module_pjl_get_int,
				    oid_module_pjl_set_int,
				    oid_module_null_test ); 
    error = oid_register_callbacks( OID_PJL_DEFAULT_BINDING,
				    oid_module_pjl_get_int,
				    oid_module_pjl_set_int,
				    oid_module_null_test ); 
    DBG_ASSERT( !error ); 
}


extern void var_module_pjl_cmd_debug_init();

// void var_module_pjl_oem_init()  
void __attribute__((weak)) var_module_pjl_oem_init()  
{
	// stub.
	DBG_PRINTF_CRIT("oem must link in %s \n", __FUNCTION__);
	ASSERT(nvram_handle);
}

void var_module_pjl_init(void)
{
	static bool init_done = false;
	error_type_t error = 0; UNUSED_VAR(error);

	if (init_done) return; 

	error = nvram_variable_register(&nvram_handle, PJL_VARS_ID, var_pjl_common_version, 
					sizeof(var_pjl_common_t), 
					pjl_common_vars_init_callback, NULL);
	DBG_ASSERT( !error );
	oid_register_module_pjl_common( );
	var_module_pjl_oem_init();
	var_module_pjl_cmd_debug_init();
	init_done = true;
}
