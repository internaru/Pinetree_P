/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_status.c
 *
 * \brief This file implements the OID back-end functions for the
 * status API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "print_events_common_api.h"

#include "dprintf.h"
#include "lassert.h"


/**
 * 
 * \brief The backend oid status API module function 
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
static error_type_t oid_module_status_get_int( oid_t oid,
                                               uint32_t index,
                                               uint32_t * value)
{
    error_type_t e_res;

    e_res = OK;
    switch ( oid )
    {
        case OID_STATUS_GENERAL:
        {
            *value = pec_get_current_printstatus();
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
 * \brief Registers each status API OID with the oid controller
 *
 */
void oid_register_module_status( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_STATUS_GENERAL,
            oid_module_status_get_int,
            oid_module_null_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

}


