/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_usb_device.c
 *
 * \brief This file implements the OID back-end functions for the
 * USB device API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "usb_device_api.h"

#include "dprintf.h"
#include "lassert.h"


/**
 * 
 * \brief The backend oid USB Device API module function for retreiving
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
static error_type_t oid_module_usb_device_get( oid_t oid,
        uint32_t index,
        char * data_ptr,
        uint32_t max_string_len_in_bytes )
{
    error_type_t e_res;
    usb_sys_config_t config;

    e_res = usb_dev_get_config( &config );

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_USB_DEVICE_VID:
            {
                unsigned int string_size;

                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len_in_bytes,
                        &string_size,
                        "%x",
                        config.USBVendorID );

                ASSERT( string_size <= max_string_len_in_bytes );
                ASSERT( 0 != string_size );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }

                break;
            }

            case OID_USB_DEVICE_PID:
            {
                unsigned int string_size;

                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len_in_bytes,
                        &string_size,
                        "%x",
                        config.USBProductID );

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
    else
    {
        e_res = FAIL;
    }

    return e_res;
}


/**
 *
 * \brief Registers each USB Device API OID with the oid controller
 *
 */
void oid_register_module_usb_device( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_USB_DEVICE_PID,
            oid_module_usb_device_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_USB_DEVICE_VID,
            oid_module_usb_device_get,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

}


