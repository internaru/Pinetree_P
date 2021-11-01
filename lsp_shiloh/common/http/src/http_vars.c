
/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                      Marvell Confidential
 * ============================================================================
 */

/** 
 * \file http_vars.c
 * 
 * \brief Implements the public interface for the HTTP component specific
 * variables.
 * 
 **/

//--------------------------------------
// Included Files
//--------------------------------------

#include "data_access_api.h"
#include "http_config.h"
#include "http_vars.h"
#include "error_types.h"
#include "nvram_api.h"
#include "lassert.h"
#include <stdint.h>
#include <string.h>


/** \brief The HTTP nvram manager block version number.
 * This number should be incremented every time the structure
 * "http_vars_t" changes */
#define HTTP_VARS_VERSION 2

typedef struct http_vars_s
{
    char password[ HTTP_VAR_PASSWORD_STRING_MAX_LEN ]; /*!< The EWS password used
                                                         to protect access to
                                                         specific sites within
                                                         the EWS */

} http_vars_t;


/** \brief The NVM manager handle, used to store language variables in NVram */
static nvram_handle_t * http_nvm_handle = NULL;


/** An instance of the NVM vars */
static http_vars_t http_vars;


/** The system reset/factory default password */
#define DEFAULT_PASSWORD ""


static error_type_t http_vars_internal_init( nvram_init_type_t init_type,
                                             uint16_t version,
                                             void * init_location,
                                             uint32_t init_location_size,
                                             void * unused )
{
    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
 
            strncpy( http_vars.password, DEFAULT_PASSWORD, HTTP_VAR_PASSWORD_STRING_MAX_LEN );

            ASSERT(sizeof(http_vars) <= init_location_size);
            memcpy( init_location, &http_vars, sizeof(http_vars));
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(&http_vars, init_location, sizeof(http_vars));
            break;
    }
    return ( OK );
}


void http_vars_init(void)
{
    error_type_t e_res;

    e_res = nvram_variable_register( &http_nvm_handle,
                                     HTTP_VARS_ID,
                                     HTTP_VARS_VERSION,
                                     sizeof(http_vars),
                                     http_vars_internal_init, 
                                     NULL );
    ASSERT( OK == e_res );
}


error_type_t http_get_xml_version_string( char * data, uint32_t string_len )
{
    error_type_t e_res;
    http_config_t my_config;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        e_res = http_config_get( &my_config );
    }

    if ( OK == e_res )
    {
        strncpy( data, my_config.xml_version_string, string_len );
    }

    return e_res;
}

error_type_t http_get_order_supplies_link( char * data, uint32_t string_len )
{
    error_type_t e_res;
    http_config_t my_config;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        e_res = http_config_get( &my_config );
    }

    if ( OK == e_res )
    {
        strncpy( data, my_config.order_supplies_link, string_len );
    }

    return e_res;
}


error_type_t http_get_var_password( char * data, uint32_t string_len )
{
    error_type_t e_res;
    uint32_t min_string_len;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        min_string_len = string_len;

        /* use the smaller of the two possible string lengths */
        if ( string_len >= HTTP_VAR_PASSWORD_STRING_MAX_LEN )
        {
            min_string_len = HTTP_VAR_PASSWORD_STRING_MAX_LEN - 1;
        }

        strncpy( data, http_vars.password, min_string_len );
        data[ min_string_len ] = '\0';
    }

    return e_res;
}


error_type_t http_set_var_password( char * data, uint32_t string_len )
{
    error_type_t e_res;
    uint32_t min_string_len;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        min_string_len = string_len;

        /* use the smaller of the two possible string lengths */
        if ( string_len >= HTTP_VAR_PASSWORD_STRING_MAX_LEN )
        {
            // Allow space for null character
            min_string_len = HTTP_VAR_PASSWORD_STRING_MAX_LEN - 1;
        }

        if ( min_string_len == 0 )
        {
           http_vars.password[0] = '\0';
        }
        else
        {
            strncpy( http_vars.password, data, min_string_len );
            http_vars.password[ min_string_len ] = '\0';
        }
        e_res = nvram_set_var( http_nvm_handle, &http_vars );
    }

    return e_res;
}
