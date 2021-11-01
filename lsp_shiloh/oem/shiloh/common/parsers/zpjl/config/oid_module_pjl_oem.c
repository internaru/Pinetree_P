/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_pjl_oem.c
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

// USB status monitor	2013.12.17	by Juny
#include "db_api.h"
#include "dprintf.h"

#define DBG_PRFX "pjl_var: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(9)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD


#define var_pjl_oem_version 1

static nvram_handle_t *nvram_handle = 0;
static pthread_mutex_t var_pjl_lock = POSIX_MUTEX_INITIALIZER;


static char user_id[40]; // temporary string storage! 

/* Tonersave/TonerDarkness */
typedef struct var_pjl_oem_s
{
    uint32_t resolution;
    uint32_t economode;	// false, true
    uint32_t tonerdarkness;	// 1 ~ 10
} var_pjl_oem_t;

static var_pjl_oem_t var_pjl_oem_factory_default = {
    .resolution = 600,
    .economode = 0,
    .tonerdarkness = 6,
};

static var_pjl_oem_t var_pjl_oem;


inline uint32_t var_pjl_oem_get_resolution()
{
    uint32_t value;
    pthread_mutex_lock( &var_pjl_lock );
    
    value = var_pjl_oem.resolution;

    pthread_mutex_unlock( &var_pjl_lock );
    return value;
}

inline void var_pjl_oem_set_resolution( uint32_t value )
{
    pthread_mutex_lock( &var_pjl_lock );

    // shiloh engine specifics:
    // limit set to valid inputs on this device
    var_pjl_oem.resolution = 
	value == 600 ? 600 : value ==  1200 ? 1200 : 600;
    nvram_set_var(nvram_handle, &var_pjl_oem);
    
    pthread_mutex_unlock( &var_pjl_lock );
}

inline uint32_t var_pjl_oem_get_economode()
{
    uint32_t value;
    pthread_mutex_lock( &var_pjl_lock );
    
    value = var_pjl_oem.economode;

    pthread_mutex_unlock( &var_pjl_lock );
    return value;
}

inline void var_pjl_oem_set_economode( uint32_t value )
{
    pthread_mutex_lock( &var_pjl_lock );

    var_pjl_oem.economode = value;
    nvram_set_var(nvram_handle, &var_pjl_oem);
    
    pthread_mutex_unlock( &var_pjl_lock );
}

inline uint32_t var_pjl_oem_get_tonerdarkness()
{
    uint32_t value;
    pthread_mutex_lock( &var_pjl_lock );
    
    value = var_pjl_oem.tonerdarkness;

    pthread_mutex_unlock( &var_pjl_lock );
    return value;
}

inline void var_pjl_oem_set_tonerdarkness( uint32_t value )
{
    pthread_mutex_lock( &var_pjl_lock );

    var_pjl_oem.tonerdarkness = value;
    nvram_set_var(nvram_handle, &var_pjl_oem);
    
    pthread_mutex_unlock( &var_pjl_lock );
}

/* USB Status Monitor	2013.12.17 by Juny */
inline uint32_t var_pjl_oem_get_intray1size()
{
/*
    uint32_t value = 0;	// step3 set 0

    error_type_t db_retval = FAIL;
*/
// step 2
/*
    char *pjlLine;
    pjlLine = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), 256);
*/

    pthread_mutex_lock( &var_pjl_lock );
    
//    value = (uint32) db_get_var_int("CONF_PRNT", "SizeTypeTray_tray", &db_retval);
    DBG_PRINTF_ERR("intray1size is act. \n");    
// step 3    find seg fault
//    value = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", &db_retval);

/*
    if(db_retval == OK)
    {
        ;
    }
	else 
    DBG_PRINTF_ERR("ERR to read DB\n");    
*/
    pthread_mutex_unlock( &var_pjl_lock );
// step 4
//    return value;
    return OK;
}


inline uint32_t var_pjl_oem_get_intray2size()
{
    pthread_mutex_lock( &var_pjl_lock );

    DBG_PRINTF_ERR("intray2size is act. \n");    

    pthread_mutex_unlock( &var_pjl_lock );
    return OK;
}

inline uint32_t var_pjl_oem_get_intray3size()
{
    pthread_mutex_lock( &var_pjl_lock );

    DBG_PRINTF_ERR("intray3size is act. \n");    

    pthread_mutex_unlock( &var_pjl_lock );
    return OK;
}

static error_type_t oid_module_pjl_get_int( oid_t oid,
					    uint32_t index,
					    uint32_t * value)
{
    error_type_t e_res = OK;

    switch ( oid )
    {
    case OID_PJL_RESOLUTION:
	*value = var_pjl_oem_get_resolution();
	break;
	case OID_PJL_ECONOMODE:
		*value = var_pjl_oem_get_economode();
		break;
	case OID_PJL_TONERDARKNESS:
		*value = var_pjl_oem_get_tonerdarkness();
		break;
	case OID_PJL_INTRAY1SIZE:		/* 2013.12.17		by Juny  */
		*value = var_pjl_oem_get_intray1size();
/*
//    	DBG_PRINTF_INFO("intray1size is act. value is %d ret is %d\n", value, db_retval);    
		//#include "dprintf.h"
		//

    strcpy(pjlLine,AT_PJL);
    strcat(pjlLine,cWS);
    if(deviceStatus)
        strcat(pjlLine,cDEVICE);
    else
        strcat(pjlLine, cTIMED);
    strcat(pjlLine, cCRLF);
    strcat(pjlLine, cCODE);
    minSprintf(&pjlLine[strlen(pjlLine)],"%d",value);
    strcat(pjlLine, cCRLF);
    strcat(pjlLine,cFF);

    pjl_output_string(Pipe, pjlLine);


*/
		break;
	case OID_PJL_INTRAY2SIZE:		/* 2013.12.17		by Juny  */
	case OID_PJL_INTRAY3SIZE:		/* 2013.12.17		by Juny  */
		*value = var_pjl_oem_get_intray1size();
		break;
    default:
	e_res = FAIL;  // not found
    }
    DBG_PRINTF_INFO("%s get oid %d index %d value %d\n", __FUNCTION__, oid, index, * value );
    return e_res;
}

static error_type_t oid_module_pjl_set_int( oid_t oid,
					    uint32_t index,
					    uint32_t value)
{
    error_type_t e_res = OK;

    switch ( oid )
    {
    case OID_PJL_RESOLUTION:
       DBG_PRINTF_INFO("%s set Resolution oid %d index %d value %d (%d)\n", __FUNCTION__, oid, index, value );
	var_pjl_oem_set_resolution( value );
	break;

	case OID_PJL_ECONOMODE:
       DBG_PRINTF_INFO("%s set Econo oid %d index %d value %d (%d)\n", __FUNCTION__, oid, index, value );
		var_pjl_oem_set_economode( value );
		break;
	case OID_PJL_TONERDARKNESS:
       DBG_PRINTF_INFO("%s set Dark oid %d index %d value %d (%d)\n", __FUNCTION__, oid, index, value );
		var_pjl_oem_set_tonerdarkness( value );		// fix from get		2014.02.21	by Juny
		break;
    default:
	e_res = FAIL;  // not found
    }
    
    return e_res;
}

static error_type_t oid_module_pjl_set_userid( oid_t oid,
					       uint32_t index,
					       const char *value,
					       uint32_t len
	)
{
	DBG_ASSERT( oid == OID_PJL_AUTHENTICATIONUSERID );
	pthread_mutex_lock( &var_pjl_lock );
	strncpy(user_id, value, 40);
	pthread_mutex_unlock( &var_pjl_lock );
	return OK;
}

/// OEM todo needs implementation.
error_type_t __attribute__((weak)) oem_password_validate(const char *user, const char *password)  
{
	error_type_t valid;
	pthread_mutex_lock( &var_pjl_lock );
	valid = strncmp(user, password, 40);
	pthread_mutex_unlock( &var_pjl_lock );
	return valid;  // OK on valid password, error on not valid.
}

static error_type_t oid_module_pjl_set_password( oid_t oid,
					       uint32_t index,
					       const char *value,
					       uint32_t len
	)
{
	DBG_ASSERT( oid == OID_PJL_AUTHENTICATIONPASSWORD );
	
	return oem_password_validate(user_id, value);
}

static error_type_t pjl_oem_vars_init_callback(nvram_init_type_t InitType, uint16_t Version, void *InitLocation, uint32_t InitLocationSize, void *unused)
{
	switch( InitType )
	{
        case NVRAM_VERSION_CHANGE:     /**< Version change */
		DBG_ASSERT( Version == var_pjl_oem_version ); /// implement version migration
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
		ASSERT(sizeof(var_pjl_oem_t) <= InitLocationSize);
		memcpy(&var_pjl_oem, &var_pjl_oem_factory_default, sizeof(var_pjl_oem_t));
		memcpy(InitLocation, &var_pjl_oem, sizeof(var_pjl_oem_t));
		break;

        case NVRAM_OK: 
		// read out current values
		memcpy(&var_pjl_oem, InitLocation, sizeof(var_pjl_oem_t));
            break;
        default:
		ASSERT( NVRAM_OK == InitType );  // undefined init type
    }
    return ( OK );
}

static void oid_register_module_pjl_oem( void )
{
	error_type_t error;

	error = oid_register_callbacks( OID_PJL_RESOLUTION,
					oid_module_pjl_get_int,
					oid_module_pjl_set_int,
					oid_module_null_test );  
	error = oid_register_callbacks( OID_PJL_ECONOMODE,
					oid_module_pjl_get_int,
					oid_module_pjl_set_int,
					oid_module_null_test );  
	error = oid_register_callbacks( OID_PJL_TONERDARKNESS,
					oid_module_pjl_get_int,
					oid_module_pjl_set_int,
					oid_module_null_test );  
	error = oid_register_callbacks( OID_PJL_AUTHENTICATIONUSERID,
					oid_module_null_get,
					oid_module_pjl_set_userid,
					oid_module_null_test );  
	error = oid_register_callbacks( OID_PJL_AUTHENTICATIONPASSWORD,
					oid_module_null_get,
					oid_module_pjl_set_password,
					oid_module_null_test );  
	ASSERT( !error ); 
}


void var_module_pjl_oem_init() 
{
	static bool init_done = false;
	error_type_t error;

	if (init_done) return; 

	error = nvram_variable_register(&nvram_handle, PJL_OEM_VARS_ID, var_pjl_oem_version, 
					sizeof(var_pjl_oem_t), 
					pjl_oem_vars_init_callback, NULL);
	DBG_ASSERT( !error );

	oid_register_module_pjl_oem( );
	init_done = true;
}
