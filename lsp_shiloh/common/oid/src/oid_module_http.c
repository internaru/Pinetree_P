/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_http.c
 *
 * \brief This file implements the OID back-end functions for the
 * http API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "http_vars.h"

#include "dprintf.h"
#include "lassert.h"


/**
 * 
 * \brief The backend oid http API module function for retreiving
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] data_ptr A pointer to the data buffer that receives the
 * string data.  This buffer must be at least "max_string_len_in_bytes" long.
 *
 * \param[in] max_string_len_in_bytes The maximum length in bytes that the 
 * character buffer can hold, NOT including the NULL terminating character. The
 * buffer won't be filled past this length + 1 for the NULL terminating
 * character.
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 *
 * OID_ERROR_VALUE_CAPPED - producer string was not able to fit into buffer
 *                          max_string_len_in_bytes was too small to fit the
 *                          entire string.
 *
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_http_get( oid_t oid,
        uint32_t index,
        char * data_ptr,
        uint32_t max_string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    switch ( oid )
    {
        case OID_HTTP_XML_VERSION:
        {
            unsigned int string_size;
            #define MY_XML_VERSION_STRING_LEN 10
            char xml_version_string[MY_XML_VERSION_STRING_LEN];

            e_res = http_get_xml_version_string( xml_version_string,
                    MY_XML_VERSION_STRING_LEN );

            if ( OK == e_res )
            {
                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len_in_bytes,
                        &string_size,
                        "%s",
                        xml_version_string );

                ASSERT( string_size <= max_string_len_in_bytes );
                ASSERT( 0 != string_size );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }
            }

            break;
        }

        case OID_HTTP_ORDER_SUPPLIES_LINK:
        {
            unsigned int string_size;
            #define MY_ORDER_SUPPLIES_STRING_LEN 256 
            char my_string[MY_ORDER_SUPPLIES_STRING_LEN];

            e_res = http_get_order_supplies_link( my_string,
                    MY_ORDER_SUPPLIES_STRING_LEN );

            if ( OK == e_res )
            {
                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len_in_bytes,
                        &string_size,
                        "%s",
                        my_string );

                ASSERT( string_size <= max_string_len_in_bytes );
                ASSERT( 0 != string_size );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }
            }

            break;
        }

        case OID_HTTP_PASSWORD:
            e_res = http_get_var_password( data_ptr, max_string_len_in_bytes );
            break;

        default:
        {
            /* bad oid */
            e_res = FAIL;

            break;
        }
    }

    return e_res;
}


/**
 * 
 * \brief The backend oid module function for the http module 
 * set functionality. 
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[out] data A pointer to the data buffer that contains the
 * string data.  This buffer must be at least "string_len_in_bytes" long.
 *
 * \param[in] string_len_in_bytes The length in bytes that the 
 * data buffer, "data", is, NOT the null terminating character.
 * The buffer must be at least string_len_in_bytes + 1 bytes long.
 *
 * \return error_type_t
 * OK                        - successful
 *
 * OID_ERROR_ILLEGAL_CHARS   - the string data had characters that the 
 *                             consumer could not use or interpret
 *
 * OID_ERROR_VALUE_TOO_BIG   - the sring_len_in_bytes was too large
 *                             or the data value represented by the string
 *                             was too large for the intended use
 *
 * OID_ERROR_VALUE_TOO_SMALL - the string_len_in_bytes was too small
 *                             or the data value represented by the
 *                             string was too small for the intended use
 *
 * FAIL                      - bad oid, other error
 *
 */
error_type_t oid_module_http_set( oid_t oid,
                                  uint32_t index,
                                  const char * data,
                                  uint32_t string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_HTTP_PASSWORD:
            {
                e_res = http_set_var_password( (char*)data, string_len_in_bytes );
                break;
            }
            default:
            {
                /* bad oid */
                e_res = FAIL;

                break;
            }
        }
    }

    return e_res;
}

error_type_t oid_module_http_test( oid_t oid, uint32_t index, const char * value, uint32_t string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == value )
    {
        e_res = FAIL;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    return e_res;
}


/**
 *
 * \brief Registers each http API OID with the oid controller
 *
 */
void oid_register_module_http( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_HTTP_XML_VERSION,
            oid_module_http_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_HTTP_ORDER_SUPPLIES_LINK,
            oid_module_http_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );


    e_res = oid_register_callbacks( OID_HTTP_PASSWORD,
                                    oid_module_http_get,
                                    oid_module_http_set,
                                    oid_module_http_test );
    ASSERT( FAIL != e_res );

}


