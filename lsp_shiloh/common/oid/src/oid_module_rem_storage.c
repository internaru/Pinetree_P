/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_rem_storage.c
 *
 * \brief This file implements the OID back-end functions for the
 * removable storage API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "mass_storage_app_api.h"

#include "dprintf.h"
#include "lassert.h"

static void oid_mass_storage_app_notify_callback( 
        mass_storage_app_notify_event_t event,
        mass_storage_app_handle_t * handle );

/** \brief  The number of mass storage devices we'll support in the OID layer,
 * we'll only support a fixed number of drives in the OID layer */
#define OID_NUM_MASS_STORAGE_HANDLES 1

/** \brief an array of handles to track the mass storage devices plugged
 * into the system */
static mass_storage_app_handle_t *
 oid_mass_storage_handles[OID_NUM_MASS_STORAGE_HANDLES];


/**
 * 
 * \brief The backend oid removable storage API module function for retreiving
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
static error_type_t oid_module_rem_storage_get( oid_t oid,
                                                uint32_t index,
                                                char * data_ptr,
                                                uint32_t max_string_len_in_bytes )
{
    error_type_t e_res;
    e_res = OK;

    if ( OK == e_res )
    {

        ASSERT( index < OID_NUM_MASS_STORAGE_HANDLES );

        switch ( oid )
        {
            case OID_REM_STORAGE_DEVICE_STATUS:
            {
                unsigned int string_size;

                /* TODO, determine what we need to provide in this "status"
                 * OID */
                if ( oid_mass_storage_handles[index] != NULL )
                {
                    e_res = min_n_k_sprintf( data_ptr,
                            max_string_len_in_bytes,
                            &string_size,
                            "%s",
                            "AVAILABLE" );
                }
                else
                {
                    e_res = min_n_k_sprintf( data_ptr,
                            max_string_len_in_bytes,
                            &string_size,
                            "%s",
                            "NOT AVAILABLE" );
                }

                ASSERT( string_size <= max_string_len_in_bytes );
                ASSERT( 0 != string_size );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }

                break;
            }

            case OID_REM_STORAGE_LIST:
            {
                unsigned int string_size;

                if ( oid_mass_storage_handles[index] != NULL )
                {
                    /* TODO determine if mass storage driver can support
                     * telling us what type of media is inserted */
                    e_res = min_n_k_sprintf( data_ptr,
                            max_string_len_in_bytes,
                            &string_size,
                            "%s",
                            "usb flash" );
                }
                else
                {
                    e_res = min_n_k_sprintf( data_ptr,
                            max_string_len_in_bytes,
                            &string_size,
                            "%s",
                            "none" );
                }

                ASSERT( string_size <= max_string_len_in_bytes );
                ASSERT( 0 != string_size );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
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
    }

    return e_res;
}

/**
 * 
 * \brief The backend oid removable storage API module function 
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
static error_type_t oid_module_rem_storage_get_int( oid_t oid,
                                                    uint32_t index,
                                                    uint32_t * value)
{
    error_type_t e_res = OK;

    switch ( oid )
    {
        case OID_REM_STORAGE_DEVICE_PRESENT:
        {
            if ( oid_mass_storage_handles[index] != NULL )
            {
                *value = 1;
            }
            else
            {
                *value = 0;
            }
            break;
        }

        case OID_REM_STORAGE_DEVICE_MEM_AVAIL:
        /* TODO: fall through for now until we implement filesystem access
         * seperate from photo job mgr. */
        case OID_REM_STORAGE_DEVICE_MEM_CAP:
        {
            uint64_t sector_size_u64;
            uint64_t sectors_u64;
            uint64_t num_bytes_u64;

            if ( oid_mass_storage_handles[index] != NULL )
            {
                sector_size_u64 = mass_storage_app_sector_get_size(
                        oid_mass_storage_handles[index] );

                sectors_u64 = mass_storage_app_sector_get_total_sectors(
                        oid_mass_storage_handles[index] );

                num_bytes_u64 = ( sector_size_u64 * sectors_u64 ) ;
                *value = (uint32_t)num_bytes_u64;
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



static void oid_mass_storage_app_notify_callback( 
        mass_storage_app_notify_event_t event,
        mass_storage_app_handle_t * handle )
{

    uint32_t i;

    switch ( event )
    {
        case mass_storage_app_notify_event_lun_inserted:
        {

            /* add the drive to our tracking list */
            for ( i = 0; i < OID_NUM_MASS_STORAGE_HANDLES; i++ )
            {
                if ( NULL == oid_mass_storage_handles[i] )
                {
                    oid_mass_storage_handles[i] = handle;
                    break;
                }
            }

            break;
        }

        default:
        {
            ASSERT( mass_storage_app_notify_event_lun_removed == event );

            /* remove the drive from our tracking list */
            for ( i = 0; i < OID_NUM_MASS_STORAGE_HANDLES; i++ )
            {
                if ( handle == oid_mass_storage_handles[i] )
                {
                    oid_mass_storage_handles[i] = NULL;
                    break;
                }
            }

            break;
        }
    }

}


/**
 *
 * \brief Registers each removable storage API OID with the oid controller
 *
 */
void oid_register_module_rem_storage( void )
{

    error_type_t e_res;
    uint32_t i;

    for ( i = 0; i < OID_NUM_MASS_STORAGE_HANDLES; i++ )
    {
        oid_mass_storage_handles[i] = NULL;
    }

    /* register for notification if disk insertion and removal so we can respond
     * to OID requests appropriately */
    e_res = mass_storage_app_notify_register(
            oid_mass_storage_app_notify_callback );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_REM_STORAGE_DEVICE_PRESENT,
                                    oid_module_rem_storage_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_REM_STORAGE_DEVICE_MEM_AVAIL,
                                    oid_module_rem_storage_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_REM_STORAGE_DEVICE_MEM_CAP,
                                    oid_module_rem_storage_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_REM_STORAGE_DEVICE_STATUS,
                                    oid_module_rem_storage_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_REM_STORAGE_LIST,
                                    oid_module_rem_storage_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

}


