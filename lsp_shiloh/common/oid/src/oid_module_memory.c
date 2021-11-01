/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_memory.c
 *
 * \brief This file implements the OID back-end functions for the
 * memory module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "memAPI.h"

#include "dprintf.h"
#include "lassert.h"


/**
 * 
 * \brief The backend oid memory module function for retreiving
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
static error_type_t oid_module_memory_get_int( oid_t oid,
                                               uint32_t index,
                                               uint32_t * value)
{
    error_type_t e_res;
    uint32_t mem_avail;
    e_res = OK;
    switch ( oid )
    {
        case OID_MEMORY_TOTAL_SIZE_IN_MBYTES:
        {
#ifdef __linux__
			mem_avail = 0; // not yet defined -> memGetSystemMemorySize();
#else        
			mem_avail = memGetSystemMemorySize();
#endif            

			/* convert to MB [ ( #bytes / 1048576 ) = MB ]  */
			*value = mem_avail / 1048576;

            break;
        }

        case OID_MEMORY_SYS_HEAP_SIZE_IN_MBYTES:
        {
#ifdef __linux__
			mem_avail = 0; // not yet defined -> memGetHeapSize( NULL );
#else        
            mem_avail = memGetHeapSize( NULL );
#endif

            /* convert to MB [ ( #bytes / 1048576 ) = MB ]  */
			*value = mem_avail / 1048576;
            break;
        }

        case OID_MEMORY_SYS_HEAP_AVAIL_IN_MBYTES:
        {
#ifdef __linux__
			mem_avail = 0; // not yet defined -> memTotalFreeMemAvailable();
#else        
            mem_avail = memTotalFreeMemAvailable();
#endif
            
			/* convert to MB [ ( #bytes / 1048576 ) = MB ]  */
			*value = mem_avail / 1048576;
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
 * \brief Registers each memory module OID with the oid controller
 *
 */
void oid_register_module_memory( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_MEMORY_TOTAL_SIZE_IN_MBYTES,
                                    oid_module_memory_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_MEMORY_SYS_HEAP_SIZE_IN_MBYTES,
                                    oid_module_memory_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_MEMORY_SYS_HEAP_AVAIL_IN_MBYTES,
                                    oid_module_memory_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
}


