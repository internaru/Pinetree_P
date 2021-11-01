/******************************************************************************
 * Copyright (c) 2009-2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/** 
 * \file oid_module_consumable.c
 *
 * \brief This file implements the OID back-end functions for the
 * consumable API module OIDs
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "consumables_common_api.h"

#include "dprintf.h"
#include "lassert.h"


/**
 * 
 * \brief The backend oid consumable API module function for retreiving
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
static error_type_t oid_module_consumable_get( oid_t oid,
                                               uint32_t index,
                                               char * data_ptr,
                                               uint32_t max_string_len_in_bytes )
{
    error_type_t e_res;

    ASSERT( NULL != data_ptr );

    e_res = OK;

    switch ( oid )
    {
        case OID_CONSUMABLE_MODEL_NAME:
            {
                int string_size;

                string_size = max_string_len_in_bytes;

                e_res = consumable_get_model( index,
                                              data_ptr,
                                              &string_size );
                ASSERT( string_size <= max_string_len_in_bytes );

                break;
            }

        case OID_CONSUMABLE_SERIAL_NUMBER:
            {
                int string_size;

                string_size = max_string_len_in_bytes;

                e_res = consumable_get_serial_number( index,
                                                      data_ptr,
                                                      &string_size );
                ASSERT( string_size <= max_string_len_in_bytes );

                break;
            }

        case OID_CONSUMABLE_INSTALL_DATE:
            {
                int string_size;

                string_size = max_string_len_in_bytes;

                e_res = consumable_get_date_installed( index,
                                                       data_ptr,
                                                       &string_size );
                ASSERT( string_size <= max_string_len_in_bytes );

                break;
            }

        case OID_CONSUMABLE_LAST_DATE_USED:
            {
                int string_size;

                string_size = max_string_len_in_bytes;

                e_res = consumable_get_date_last_used( index,
                                                       data_ptr,
                                                       &string_size );
                ASSERT( string_size <= max_string_len_in_bytes );

                break;
            }

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
 * \brief The backend oid consumables API module function 
 * for retreiving data associated with an OID. 
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
static error_type_t oid_module_consumable_get_int( oid_t oid,
                                                   uint32_t index,
                                                   uint32_t * value)
{
    error_type_t e_res;

    e_res = OK;
    ASSERT( NULL != value );

    switch ( oid )
    {
        case OID_CONSUMABLE_PERCENT_USED:
            {
                int percent_remaining;
                percent_remaining = consumable_get_percent_remaining(index);
               *value = 100 - (uint32_t)percent_remaining;
                break;
            }

        case OID_CONSUMABLE_PERCENT_REMAINING:
            *value = (uint32_t)consumable_get_percent_remaining(index);
            break;

        case OID_CONSUMABLE_PERCENT_REMAINING_STEPPED:
            *value = (uint32_t)consumable_get_percent_remaining_stepped(index);
            break;

        case OID_CONSUMABLE_TOTAL_PAGES_PRINTED:
            *value = (uint32_t )consumable_get_num_impressions( index );
            break;

        case OID_CONSUMABLE_LOW_LEVEL_INDICATOR:
            {
                bool marker_low;
                consumable_status_t status;

                status = consumable_get_status(index);
                marker_low = ( status == CONSUMABLE_STATUS_LOW );
                *value = (uint32_t)marker_low;

                break;
            }

        case OID_CONSUMABLE_SUPPLY_OUT:
            {
                bool supply_out;
                consumable_status_t status;

                status = consumable_get_status(index);
                supply_out = ( status == CONSUMABLE_STATUS_OUT );
                *value = (uint32_t)supply_out;

                break;
            }

        case OID_CONSUMABLE_TERNARY:
            {
                consumable_status_t status;

                status = consumable_get_status(index);

                /* ternary 0: ok, 1: low, 2: out */
                if ( CONSUMABLE_STATUS_OUT == status )
                {
                    *value = 2;
                }
                else if ( CONSUMABLE_STATUS_LOW == status )
                {
                    *value = 1;
                }
                else
                {
                    *value = 0;
                }

                break;
            }

        case OID_CONSUMABLE_ANY_MARKER_TERNARY:
            {
                bool any_out;
                bool any_low;
                consumable_status_t status;
                int num_consumables;
                int i;

                any_out = false;
                any_low = false;

                num_consumables = consumable_get_num_consumables();

                for ( i = 0; i < num_consumables; i++ )
                {
                    status = consumable_get_status(i);

                    /* ternary 0: ok, 1: low, 2: out */
                    if ( CONSUMABLE_STATUS_OUT == status )
                    {
                        any_out = true;
                    }
                    else if ( CONSUMABLE_STATUS_LOW == status )
                    {
                        any_out = true;
                    }
                }

                /* ternary 0: ok, 1: low, 2: out */
                if ( true == any_out )
                {
                    *value = 2;
                }
                else if ( true == any_low )
                {
                    *value = 1;
                }
                else
                {
                    *value = 0;
                }

                break;
            }

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
 * \brief Registers each consumable API OID with the oid controller
 *
 */
void oid_register_module_consumable( void )
{
    error_type_t e_res;

    e_res = oid_register_callbacks( OID_CONSUMABLE_MODEL_NAME,
            oid_module_consumable_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_SERIAL_NUMBER,
            oid_module_consumable_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_INSTALL_DATE,
            oid_module_consumable_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_PERCENT_REMAINING,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_PERCENT_REMAINING_STEPPED,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_PERCENT_USED,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_TOTAL_PAGES_PRINTED,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_LOW_LEVEL_INDICATOR,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_LAST_DATE_USED,
            oid_module_consumable_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_SUPPLY_OUT,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_TERNARY,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_CONSUMABLE_ANY_MARKER_TERNARY,
            oid_module_consumable_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

}


