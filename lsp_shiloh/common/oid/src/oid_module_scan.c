/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_scan.c
 *
 * \brief This file implements the OID back-end functions for the
 * scan API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"

#include "dprintf.h"
#include "lassert.h"
#ifdef HAVE_COPY_SUPPORT
#include "response_subject.h" // required for copyapp_api.h
#include "print_job_types.h" // required for copyapp_api.h
#include "copyapp_api.h"
#endif // HAVE_COPY_SUPPORT
#include "scantypes.h"
#include "scancap.h"
#include "scanplat.h"



/**
 * 
 * \brief The backend oid scan API module function for retreiving
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
static error_type_t oid_module_scan_get_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t * value)
{
    error_type_t e_res;
    e_res = OK;
#ifdef HAVE_COPY_SUPPORT
    CopyConf conf;
#endif // HAVE_COPY_SUPPORT
    struct scan_platform_capabilities caps;

    scanplat_get_capabilities( &caps );
#ifdef HAVE_COPY_SUPPORT
    copyapp_get_copy_config( &conf ); // get nvram values
#endif // HAVE_COPY_SUPPORT

    switch ( oid )
    {
        case OID_ADF_SUPPORT:
            *value = caps.has_adf;
            break;

        case OID_FLATBED_SUPPORT:
            *value = caps.has_flatbed;
            break;

        case OID_SCAN_DUPLEX_SUPPORT:
            *value = caps.has_duplex;
            break;
#ifdef HAVE_COPY_SUPPORT
        case OID_SCAN_COPY_DEFAULT_MEDIA_TYPE:
            *value = conf.printPaperType;
            break;

        case OID_SCAN_COPY_DEFAULT_MEDIA_SIZE:
            *value = conf.printPaperSize;
            break;

        case OID_SCAN_COPY_QUALITY:
            *value = conf.copyQuality;
            break;

        case OID_SCAN_COPY_LIGHT_DARK:
            *value = conf.copyContrast;
            break;

        case OID_SCAN_COPY_NUMBER_OF_COPIES:
            *value = conf.copyNumber;
            break;

        case OID_SCAN_COPY_REDUCE_ENLARGE:
            *value = conf.copyResize;
            break;

        case OID_SCAN_COPY_REDUCE_ENLARGE_PERCENT:
            *value = conf.ResizePercentage;
            break;

        case OID_SCAN_COPY_COLLATE:
            *value = conf.copyCollation;
            break;
#endif //HAVE_COPY_SUPPORT
        case OID_SCAN_NUM_SCANS:
            /* Fall through */
        case OID_SCAN_NUM_COLOR_BEST_COPIES:
            /* Fall through */
        case OID_SCAN_NUM_COLOR_NORM_COPIES:
            /* Fall through */
        case OID_SCAN_NUM_MONO_BEST_COPIES:
            /* Fall through */
        case OID_SCAN_NUM_MONO_NORM_COPIES:
            /* Fall through */
        case OID_SCAN_TARGET_HOST_NAME_0:
            /* Fall through */
        case OID_SCAN_TARGET_HOST_SCAN_PENDING_0:

        {
            *value = 0;
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


#ifdef HAVE_COPY_SUPPORT
/**
 * 
 * \brief The backend oid scan API module function for 
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
static error_type_t oid_module_scan_set_int( oid_t oid,
                                              uint32_t index,
                                              uint32_t value)
{
    CopyConf conf;
    error_type_t e_res = OK;
    copyapp_get_copy_config( &conf ); // get nvram values

    switch ( oid )
    {
        case OID_SCAN_COPY_DEFAULT_MEDIA_TYPE:
            conf.printPaperType = (mediatype_t)value;
            copyapp_set_copy_config( &conf );
            break;

        case OID_SCAN_COPY_DEFAULT_MEDIA_SIZE:
            conf.printPaperSize = (mediasize_t)value;
            copyapp_set_copy_config( &conf );
            break;

        case OID_SCAN_COPY_QUALITY:
            copyapp_set_fp_quality( (e_COPYQUALITY)value, conf.ccmode );
            break;

        case OID_SCAN_COPY_LIGHT_DARK:
            copyapp_set_fp_contrast( (int) value );
            break;

        case OID_SCAN_COPY_NUMBER_OF_COPIES:
            copyapp_set_fp_num_copies( (int) value );
            break;

        case OID_SCAN_COPY_REDUCE_ENLARGE:
            copyapp_set_fp_resize( (e_COPYRESIZE) value, index );
            break;

        case OID_SCAN_COPY_REDUCE_ENLARGE_PERCENT:
            copyapp_set_fp_resize( e_CUSTOM_PCNT, value );
            break;

        case OID_SCAN_COPY_COLLATE:
            copyapp_set_fp_collation( (BOOL) value );
            break;
        default:
            e_res = FAIL;
            break;
    }
    return e_res;
}
#endif // HAVE_COPY_SUPPORT


/**
 *
 * \brief Registers each scan API OID with the oid controller
 *
 */
void oid_register_module_scan( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_ADF_SUPPORT,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_FLATBED_SUPPORT,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_DUPLEX_SUPPORT,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_NUM_SCANS,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_NUM_COLOR_BEST_COPIES,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_NUM_COLOR_NORM_COPIES,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_NUM_MONO_BEST_COPIES,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_NUM_MONO_NORM_COPIES,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_COPY_SUPPORT
    e_res = oid_register_callbacks( OID_SCAN_COPY_NUMBER_OF_COPIES,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_COPY_REDUCE_ENLARGE,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_COPY_REDUCE_ENLARGE_PERCENT,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_COPY_COLLATE,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_COPY_QUALITY,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_COPY_LIGHT_DARK,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_COPY_DEFAULT_MEDIA_TYPE,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_COPY_DEFAULT_MEDIA_SIZE,
                                    oid_module_scan_get_int,
                                    oid_module_scan_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#endif // HAVE_COPY_SUPPORT

    e_res = oid_register_callbacks( OID_SCAN_TARGET_HOST_NAME_0,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SCAN_TARGET_HOST_SCAN_PENDING_0,
                                    oid_module_scan_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

}


