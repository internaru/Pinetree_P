/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_string_mgr.c
 *
 * \brief This file implements the OID back-end functions for the
 * string mgr API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "string_mgr_api.h"

#include "dprintf.h"
#include "lassert.h"


/** \brief The base we want to use for strtoul function */
#define BASE_10 10


/**
 * 
 * \brief The backend oid string mgr API module function for retreiving
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_string_mgr_get_int( oid_t oid,
                                                   uint32_t index,
                                                   uint32_t * value)
{
    error_type_t e_res;
    string_mgr_language_t language;

    e_res = OK;

    switch ( oid )
    {
        case OID_STRING_MGR_LANGUAGE_CODE:
            e_res = string_mgr_get_language( &language );

            if ( OK == e_res )
            {
                *value = (uint32_t)language;
            }
            break;

        case OID_STRING_MGR_LANGUAGE_IS_RTL:
            e_res = string_mgr_get_language( &language );
            if ( OK != e_res )
            {
                break;
            }
            *value = ( uint32_t ) is_right_to_left_lang( language );
            break;

        case OID_STRING_MGR_DEF_LANGUAGE_CODE:
            e_res = string_mgr_get_default_language( &language );
            if ( OK == e_res )
            {
                *value = (uint32_t)language;
            }
            break;

        default:
            /* bad oid */
            e_res = FAIL;

            break;
    }

    return e_res;
}


/**
 * 
 * \brief The backend oid module for the string mgr set functionality for 
 * retreiving data associated with an OID. 
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value New value for oid
 *
 * \return One of the error_type_t return codes:
 * OK - Successful 
 * OID_ERROR_VALUE_TOO_BIG 
 * OID_ERROR_VALUE_TOO_SMALL 
 * FAIL - bad oid, other error 
 *
 */
static error_type_t oid_module_string_mgr_set_int( oid_t oid,
                                                   uint32_t index,
                                                   uint32_t value)
{
    error_type_t e_res;

    switch ( oid )
    {
        case OID_STRING_MGR_LANGUAGE_CODE:
            e_res = string_mgr_set_language( (string_mgr_language_t)value );
            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
            break;
			
        case OID_STRING_MGR_DEF_LANGUAGE_CODE:
            e_res = string_mgr_set_default_language( (string_mgr_language_t)value );
            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
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
 * \brief Registers each string mgr API OID with the oid controller
 *
 */
void oid_register_module_string_mgr( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_STRING_MGR_LANGUAGE_CODE,
                                    oid_module_string_mgr_get_int,
                                    oid_module_string_mgr_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_STRING_MGR_LANGUAGE_IS_RTL,
                                    oid_module_string_mgr_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_STRING_MGR_DEF_LANGUAGE_CODE,
                                    oid_module_string_mgr_get_int,
                                    oid_module_string_mgr_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

}

