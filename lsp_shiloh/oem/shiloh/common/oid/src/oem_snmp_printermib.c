/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* Implementation for oids mapped to snmp Printer-MIB oids: (work in progress)

// initial set:
prtGeneralTable	(number of rows)
	prtGeneralCurrentOperator
	prtInputDefaultIndex
	prtMarkerDefaultIndex
prtInputTable	(number of rows)
	prtInputIndex
	prtInputMaxCapacity
	prtInputMediaName
prtMediaPathTable	(number of rows)
	prtMediaPathIndex
	prtMediaPathMaxSpeedPrintUnit
prtInterpreterTable	(number of rows)
	prtInterpreterIndex
	prtInterpreterDefaultOrientation
*/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "platform_api.h"
#include "platform.h"
#include "utils.h"
#include "oid_sm_api.h" // OID_SM_PRINT_MEDIA_DEFAULT

#include "location_api.h"

#define DBG_PRFX "oem.snmp.mib: "
//off #define DBG_VERBOSE_ON
#include "net_debug.h" // DBG_ family macros

#include <string.h>
#include "uuid.h"
#include "nvram_api.h"

#include "string_mgr_config.h" // string_mgr_localization_type_t
#include "oid_vendor_api.h"    // snmp/pjl/job-info oids
#include "cmd_proc_api.h"      // cmd_append_result()
#include "consumables_api.h"   // consumable_set_default_orientation()

// Global routines
error_type_t register_oem_marker_oids(void); // revisit extern cast

/** Test OID Values */
static uint32_t printermib_test_rowcount = 3;
static char printermib_test_string[3][128] = 
    { "Test String 1", "Test String 2", "Test String 3" };
static uint32_t printermib_test_unsignednum[3] =
    { 11111, 22222, 33333 };
static uint32_t printermib_test_signednum[3] =
    { 0x80000000 | 1, 0x80000000 | 2, 0 };
static uint32_t printermib_test_numlist[3] =
    { 0, 1, 2 };


// job-info[NUMBER_OF_JOB_HISTORIES]

#define NUMBER_OF_JOB_HISTORIES 2
typedef struct job_info_s {
    char* name;
    int   printed;
    int   requested;
} job_info_t;
job_info_t job_info[NUMBER_OF_JOB_HISTORIES] = {
{ "job0 (tbd)", 0, 0 },
{ "jobN (tbd)", 0, 0 }
};


static inline char* get_job_info_name(int index)
{
    ASSERT(index >= 0 && index < NUMBER_OF_JOB_HISTORIES);
    return job_info[index].name;
}

error_type_t retrieve_pjl_job_name(int history, int index_ignored, char* data_ptr, int bytes_max)
{
    error_type_t e_res = OK;
    uint32_t string_size;

    e_res = min_n_k_sprintf(data_ptr, bytes_max, &string_size, "%s", job_info[history].name);

    ASSERT(string_size <= bytes_max);

    if ( DEBUG_STRING_CAPPED == e_res )
    {
        e_res = OID_ERROR_VALUE_CAPPED;
    }

return e_res;
}

error_type_t oem_oid_get_string( oid_t oid,
                                             uint32_t index,
                                             char * data_ptr,
                                             uint32_t max_string_len_in_bytes )
{
    uint32_t string_size;
    error_type_t e_res = OK;


    if ( NULL == data_ptr || 
            max_string_len_in_bytes < 1 ||
                index >= printermib_test_rowcount )
    {
        cmd_append_result("%s: FAILED! lookup oid %x\n", __FUNCTION__, oid);
        return FAIL;
    }

    cmd_append_result("%s: lookup oid %x\n", __FUNCTION__, oid);

    switch ( oid )
    {
        case OID_PRTMIB_TEST_STRING:
        {
            e_res = min_n_k_sprintf(    data_ptr, max_string_len_in_bytes, 
                                        &string_size, "%s",
                                        printermib_test_string[ index ] );

            ASSERT( string_size <= max_string_len_in_bytes );

            if ( DEBUG_STRING_CAPPED == e_res )
            {
                e_res = OID_ERROR_VALUE_CAPPED;
            }

            break;
        }

        case OID_JOB_INFO_JOBNAME:
        case OID_JOB_INFO_NAME1:
            e_res = retrieve_pjl_job_name(0, index, data_ptr, max_string_len_in_bytes);
        break;

        case OID_JOB_INFO_NAME2:
            e_res = retrieve_pjl_job_name(1, index, data_ptr, max_string_len_in_bytes);
        break;

        case OID_CONSUMABLE_MODEL_NAME:
        {
            oid_callback_get_string_t retrieve_string = (oid_callback_get_string_t) oid_producer_get_func(oid);
            e_res = OID_ERROR_NOT_IMPLEMENTED; // unless demonstrated otherwise ..
            if (retrieve_string != NULL)
            {
                //cmd_append_result("%s: oid get fun %x\n", __FUNCTION__, (unsigned int)retrieve_string);
                e_res = oid_get_string(oid, index, data_ptr, max_string_len_in_bytes);
            }

            if ( DEBUG_STRING_CAPPED == e_res )
            {
                e_res = OID_ERROR_VALUE_CAPPED;
            }

          break;
        }
        case OID_SNMP_prtInputMediaName: // case OID_SNMP_PRINT_MEDIA_SIZE:
        {
            e_res = oid_get_string(OID_SM_PRINT_MEDIA_DEFAULT, index, data_ptr, max_string_len_in_bytes);

            if ( DEBUG_STRING_CAPPED == e_res )
            {
                e_res = OID_ERROR_VALUE_CAPPED;
            }

            break;
        }

        default:

            e_res = OID_ERROR_OID_INVALID;
            break;
    }

    return e_res;
}

static error_type_t oem_oid_set_string( oid_t oid,
                                             uint32_t index,
                                             char * data_ptr,
                                             uint32_t string_len_in_bytes )
{
    error_type_t e_res = OK;

    if ( NULL == data_ptr || index >= printermib_test_rowcount )
    {
        return FAIL;
    }

    switch ( oid )
    {
        case OID_PRTMIB_TEST_STRING :
        {
            strcpy( printermib_test_string[ index ],
                    ( const char * ) data_ptr );
            break;
        }

        default:
            e_res = OID_ERROR_OID_INVALID;
            break;
    }

    return e_res;
}

static error_type_t oem_oid_get_int( oid_t oid,
                                                    uint32_t index,
                                                    uint32_t * value)
{
    error_type_t e_res = OK;

    if ( NULL == value )
    {
        DBG_ERR("%s(x%x:%d) no destination buffer?\n", __func__, oid, index);
        return FAIL;
    }

    switch ( oid )
    {
        case OID_PRTMIB_TEST_UNSIGNEDNUM:
        {
            if ( index >= printermib_test_rowcount )
            {
                return FAIL;
            }

            * value = ( uint32_t ) printermib_test_unsignednum[ index ];
            break;
        }

        case OID_PRTMIB_TEST_SIGNEDNUM:
        {
            if ( index >= printermib_test_rowcount )
            {
                return FAIL;
            }

            * value = ( uint32_t ) printermib_test_signednum[ index ];
            break;
        }

        case OID_PRTMIB_TEST_NUMLIST:
        {
            if ( index >= printermib_test_rowcount )
            {
                return FAIL;
            }

            * value = ( uint32_t ) printermib_test_numlist[ index ];
            break;
        }

        case OID_PRTMIB_TEST_ROWCOUNT:
        {
            * value = ( uint32_t ) printermib_test_rowcount;
            break;
        }


        uint32_t var_pjl_common_get_default_copies(void); // revisit extern cast
        case OID_PJL_DEFAULT_COPIES:
        {
            *value = var_pjl_common_get_default_copies();
            e_res = OK;
        }
        break;

        case OID_PRTMIB_DEFAULT_ORIENTATION:
        {
	    e_res = consumable_get_default_orientation(index, value); // index treated as marker #
	    break;
        }

        case OID_SNMP_prtMarkerSuppliesTable:
        case OID_SNMP_prtMarkerSuppliesMarkerIndex:
	{
	    *value = consumable_get_num_consumables();
	    e_res = OK;
	}

        default:
            e_res = OID_ERROR_OID_INVALID;
            break;
    }

    return e_res;
}

static error_type_t oem_oid_set_int( oid_t oid,
                                                    uint32_t index,
                                                    uint32_t value)
{
    error_type_t e_res = OK;

    if ( index >= printermib_test_rowcount )
    {
        return FAIL;
    }

    switch ( oid )
    {
        case OID_PRTMIB_TEST_UNSIGNEDNUM:
        {
            printermib_test_unsignednum[ index ] = value;
            break;
        }

        case OID_PRTMIB_TEST_SIGNEDNUM:
        {
            printermib_test_signednum[ index ] = value;
            break;
        }

        case OID_PRTMIB_TEST_NUMLIST:
        {
            printermib_test_numlist[ index ] = value;
            break;
        }

        case OID_PRTMIB_DEFAULT_ORIENTATION:
        {
            e_res = consumable_set_default_orientation(index, value);
            break;
        }


        default:
            e_res = OID_ERROR_OID_INVALID;
            break;
    }
    return e_res;
}

void oid_register_module_printermib( void )
{

    error_type_t e_res;

    e_res = OK;

    e_res = oid_register_callbacks( OID_PRTMIB_TEST_STRING,
                                    oem_oid_get_string,
                                    oem_oid_set_string,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PRTMIB_TEST_UNSIGNEDNUM,
                                    oem_oid_get_int,
                                    oem_oid_set_int,
                                    oid_module_null_test );
    ASSERT(FAIL != e_res);

    e_res = oid_register_callbacks( OID_PRTMIB_TEST_SIGNEDNUM,
                                    oem_oid_get_int,
                                    oem_oid_set_int,
                                    oid_module_null_test );
    ASSERT(FAIL != e_res);

    e_res = oid_register_callbacks( OID_PRTMIB_TEST_NUMLIST,
                                    oem_oid_get_int,
                                    oem_oid_set_int,
                                    oid_module_null_test );
    ASSERT(FAIL != e_res);

    e_res = oid_register_callbacks( OID_PRTMIB_TEST_ROWCOUNT,
                                    oem_oid_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT(FAIL != e_res);

// additions
    e_res = oid_register_callbacks( OID_JOB_INFO_NAME1,
                                    oem_oid_get_string,
                                    oem_oid_set_string,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
    e_res = oid_register_callbacks( OID_JOB_INFO_NAME2,
                                    oem_oid_get_string,
                                    oem_oid_set_string,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PRTMIB_DEFAULT_ORIENTATION,
                                    oem_oid_get_int,
                                    oem_oid_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    // register additional oem/snmp oids here ..
    e_res = register_oem_marker_oids();
    ASSERT( OK == e_res );
}
