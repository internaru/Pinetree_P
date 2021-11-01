/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

// ***TODO*** move to system header
#define SY_ATTR_WEAK  __attribute__((weak))

/**
 * @file sm_job_dflt_config.c
 * 
 * @brief The default OEM-specific smjob configuration options
 *
 * The purpose of the OEM configuration files for this module is to provide
 * a mechanism for the user to modify configurable options without having 
 * to recompile the core module that depend on them. The goal is to isolate
 * OEM and core specific configuration values, and provide an abstraction layer
 * to simplify the porting.
 *
 * This file is to define the capabilities of the specific device, for all
 * the job types (print, scan, faxin, and faxout). The SM Job API includes
 * functions which will read this in order for the user to appropriately
 * edit job and document tickets.
 *
 * Refer to the sm_job_api.h file for descriptions of the capabilities and
 * supporting functions.
 *  
 */  

#include "debug.h"
#include "dprintf.h"
#include "logger.h"
#include "net_logger.h"
#include "sm_job_api.h"
#include "sm_job_config.h"

#define DBG_PRFX "SM JOB CORE: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_SM_JOB

//=============================================================================
// Structures
//=============================================================================

typedef struct
{
    smjob_compression_t *print[2];
    smjob_compression_t *scan[2];
    smjob_compression_t *faxin[2];
    smjob_compression_t *faxout[2];
} smjob_oem_compression_table_t;

typedef struct
{
    smjob_format_t *print[4];
    smjob_format_t *scan[3];
    smjob_format_t *faxin[3];
    smjob_format_t *faxout[3];
} smjob_oem_format_table_t;

typedef struct
{
    smjob_finishing_t print[1];
} smjob_oem_finishings_table_t;

typedef struct
{
    smjob_orientation_t print[4];
    smjob_orientation_t scan[4];
    smjob_orientation_t faxin[4];
    smjob_orientation_t faxout[4];
} smjob_oem_orientation_table_t;

typedef struct
{
    smjob_input_source_t scan[3];
} smjob_oem_input_source_table_t;

typedef struct
{
    smjob_sides_t *print[3];
} smjob_oem_sides_table_t;

typedef struct
{
    smjob_quality_t print[3];
} smjob_oem_quality_table_t;

//=============================================================================
// Global Variables
//=============================================================================

/**
* @brief The OEM device settings
*        For each job type, add the options supported by the device. In some cases,
*        an option is only applicable for limited job types.
*        Afterward for each option, edit the define statements to indicate the number 
*        of options that are valid. The maximum allowed can be found in the 
*        structure definitions.
*/

/**
* @brief The compression settings
*        Applicable for all job types.
*        This is accessed by the api function: smjob_get_supported_compression
*/
static smjob_oem_compression_table_t g_smjob_oem_compression_table = 
{
    { SMJOB_COMPRESSION_NONE, SMJOB_COMPRESSION_APPLICATION_GZIP }, // print
    { SMJOB_COMPRESSION_NONE, SMJOB_COMPRESSION_APPLICATION_GZIP }, // scan
    { SMJOB_COMPRESSION_NONE, SMJOB_COMPRESSION_APPLICATION_GZIP }, // faxin
    { SMJOB_COMPRESSION_NONE, SMJOB_COMPRESSION_APPLICATION_GZIP }, // faxout
};
#define SMJOB_NUM_COMP_PRINT 2
#define SMJOB_NUM_COMP_SCAN 2
#define SMJOB_NUM_COMP_FAXIN 2
#define SMJOB_NUM_COMP_FAXOUT 2

/**
* @brief The format settings
*        Applicable for all job types.
*        This is accessed by the api function: smjob_get_supported_format
*/
static smjob_oem_format_table_t g_smjob_oem_format_table = 
{
    { 
#ifdef HAVE_AIRPRINT
        SMJOB_FORMAT_IMAGE_URF, 
#endif // HAVE_AIRPRINT
        SMJOB_FORMAT_APPLICATION_OCTET_STREAM, 
// TODO: 3/2014 temporarily removed until we determine how to deliver ipp attributes like scaling and 
// rotation to the print subsystem
//        SMJOB_FORMAT_IMAGE_JPEG, 
        SMJOB_FORMAT_APPLICATION_PCLM }, // print
    { SMJOB_FORMAT_UNKNOWN, SMJOB_FORMAT_APPLICATION_OCTET_STREAM, SMJOB_FORMAT_IMAGE_JPEG }, // scan
    { SMJOB_FORMAT_UNKNOWN, SMJOB_FORMAT_APPLICATION_OCTET_STREAM, SMJOB_FORMAT_IMAGE_JPEG }, // faxin
    { SMJOB_FORMAT_UNKNOWN, SMJOB_FORMAT_APPLICATION_OCTET_STREAM, SMJOB_FORMAT_IMAGE_JPEG }, // faxout
};
#ifdef HAVE_AIRPRINT
//#define SMJOB_NUM_FORMAT_PRINT 4
#define SMJOB_NUM_FORMAT_PRINT 3
#else // !HAVE_AIRPRINT
//#define SMJOB_NUM_FORMAT_PRINT 3
#define SMJOB_NUM_FORMAT_PRINT 2
#endif // HAVE_AIRPRINT

#define SMJOB_NUM_FORMAT_SCAN 3
#define SMJOB_NUM_FORMAT_FAXIN 3
#define SMJOB_NUM_FORMAT_FAXOUT 3

/**
* @brief The finishing settings
*        Applicable for all job types.
*        This is accessed by the api function: smjob_get_supported_finishings
*/
static smjob_oem_finishings_table_t g_smjob_oem_finishings_table = 
{
    { SMJOB_FINISHING_NONE }, // print
};
#define SMJOB_NUM_FINISHINGS_PRINT 1

/**
* @brief The orientation settings
*        Applicable for all job types.
*        This is accessed by the api function: smjob_get_supported_orientation
*/
static smjob_oem_orientation_table_t g_smjob_oem_orientation_table = 
{
    { SMJOB_ORIENTATION_PORTRAIT, SMJOB_ORIENTATION_LANDSCAPE, 
      SMJOB_ORIENTATION_REVERSE_LANDSCAPE, SMJOB_ORIENTATION_REVERSE_PORTRAIT }, // print
    { SMJOB_ORIENTATION_PORTRAIT, SMJOB_ORIENTATION_LANDSCAPE, 
      SMJOB_ORIENTATION_REVERSE_PORTRAIT, SMJOB_ORIENTATION_REVERSE_LANDSCAPE }, // scan
    { SMJOB_ORIENTATION_PORTRAIT, SMJOB_ORIENTATION_LANDSCAPE, 
      SMJOB_ORIENTATION_REVERSE_PORTRAIT, SMJOB_ORIENTATION_REVERSE_LANDSCAPE }, // faxin
    { SMJOB_ORIENTATION_PORTRAIT, SMJOB_ORIENTATION_LANDSCAPE, 
      SMJOB_ORIENTATION_REVERSE_PORTRAIT, SMJOB_ORIENTATION_REVERSE_LANDSCAPE }, // faxout
};
#define SMJOB_NUM_ORIENTATION_PRINT 4
#define SMJOB_NUM_ORIENTATION_SCAN 4
#define SMJOB_NUM_ORIENTATION_FAXIN 4
#define SMJOB_NUM_ORIENTATION_FAXOUT 4

/**
* @brief The resolution settings
*        Applicable for all job types.
*        NOTE here can only specify one type of units
*        This is accessed by the api function: smjob_get_supported_resolution
*/
static smjob_resolution_t g_smjob_oem_resolution_table [] = 
{
//    width height  units
    { 600,  600,    SMJOB_RES_UNIT_DOTS_PER_INCH }, // print
    { 600,  600,    SMJOB_RES_UNIT_DOTS_PER_INCH }, // scan
    { 600,  600,    SMJOB_RES_UNIT_DOTS_PER_INCH }, // faxin
    { 600,  600,    SMJOB_RES_UNIT_DOTS_PER_INCH }, // faxout
};
#define SMJOB_NUM_RESOLUTION_PRINT 1

/**
* @brief The input source settings
*        Applicable only for scan.
*        This is accessed by the api function: smjob_get_supported_input_source
*/
static smjob_oem_input_source_table_t g_smjob_oem_input_source_table = 
{
    { SMJOB_INPUT_SOURCE_ADF, SMJOB_INPUT_SOURCE_ADF_DUPLEX, SMJOB_INPUT_SOURCE_PLATEN },
};
#define SMJOB_NUM_INPUT_SOURCE_SCAN 3

/**
* @brief The minimum and maxiumum range of copies allowed
*        Applicable only for print.
*        This is accessed by the api function: smjob_get_supported_copies
*/
#define SMJOB_PRINT_MIN_COPIES 1
#define SMJOB_PRINT_MAX_COPIES 500 

/**
* @brief The collation setting
*        Applicable only for print.
*        This is accessed by the api function: smjob_get_supported_sheet_collate
*/
#define SMJOB_PRINT_SHEET_COLLATE true

/**
* @brief The sides settings
*        Applicable only for print.
*        This is accessed by the api function: smjob_get_supported_sides
*/
static smjob_oem_sides_table_t g_smjob_oem_sides_table = 
{
    { SMJOB_SIDES_ONE_SIDED,
      SMJOB_SIDES_TWO_SIDED_LONG_EDGE,
      SMJOB_SIDES_TWO_SIDED_SHORT_EDGE }
};
#define SMJOB_NUM_SIDES_PRINT 3

/**
* @brief The quality settings
*        Applicable only for print.
*        This is accessed by the api function: smjob_get_supported_quality
*/
static smjob_oem_quality_table_t g_smjob_oem_quality_table = 
{
    { SMJOB_QUALITY_DRAFT, SMJOB_QUALITY_NORMAL, SMJOB_QUALITY_HIGH },
};
#define SMJOB_NUM_QUALITY_PRINT 3

/**
* @brief The input source settings
*        Applicable only for scan.
*        This is accessed by the api function: smjob_get_supported_input_source
*/

/**
* @brief The print content optimize settings.
*/
static smjob_print_content_optimize_t *g_smjob_print_content_optimize_table[] =
{
   SMJOB_PRINT_CONTENT_OPTIMIZE_AUTO,
   SMJOB_PRINT_CONTENT_OPTIMIZE_GRAPHICS,
   SMJOB_PRINT_CONTENT_OPTIMIZE_PHOTO,
   SMJOB_PRINT_CONTENT_OPTIMIZE_TEXT,
   SMJOB_PRINT_CONTENT_OPTIMIZE_TEXT_AND_GRAPHICS
};

static uint32_t g_smjob_print_content_optimize_table_size = 
                                            (sizeof(g_smjob_print_content_optimize_table) /
                                             sizeof(smjob_print_content_optimize_t *));


//=============================================================================
// Function Definitions
//=============================================================================

/**
* @brief The oem function definitions
*        These accessor functions are called by the smjob_api functions (smjob_get_supported_*)
*/

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_icons(smjob_type_t job_type, uint32_t index, char** icon)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    static char *printer_icons[] =
    {
       "/images/airprint_48.png",
       "/images/airprint_128.png",
       "/images/airprint_512.png",
    };

    // Check inputs
    if( !icon )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    // This default code will say that each media table entry is stationery type.
    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            uint32_t num_printer_icons = sizeof(printer_icons)/sizeof(char *);

            if (index >= num_printer_icons)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                *icon = (char *)printer_icons[index];
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_compression(smjob_type_t job_type, uint32_t index, smjob_compression_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Lookup the value per job_type and index
    // For each job_type, verify that the index isn't out of bounds
    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
//            if ( SMJOB_NUM_COMP_PRINT <= index )
//            {
                *value = SMJOB_COMPRESSION_NONE;
//            }
//            else
//            {
//                *value = g_smjob_oem_compression_table.print[index];
//            }
            break;

        case SMJOB_TYPE_SCAN:
            if ( SMJOB_NUM_COMP_SCAN <= index )
            {
                *value = SMJOB_COMPRESSION_NONE;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                *value = g_smjob_oem_compression_table.scan[index];
            }
            break;

        case SMJOB_TYPE_FAXIN:
            if ( SMJOB_NUM_COMP_FAXIN <= index )
            {
                *value = SMJOB_COMPRESSION_NONE;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                *value = g_smjob_oem_compression_table.faxin[index];
            }
            break;

        case SMJOB_TYPE_FAXOUT:
            if ( SMJOB_NUM_COMP_FAXOUT <= index )
            {
                *value = SMJOB_COMPRESSION_NONE;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                *value = g_smjob_oem_compression_table.faxout[index];
            }
            break;

        default:
            *value = SMJOB_COMPRESSION_NONE;
            break;
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_format(smjob_type_t job_type, smjob_format_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    smjob_format_t *format;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Lookup the value per job_type and index
    // For each job_type, verify that the index isn't out of bounds
    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
            if ( SMJOB_NUM_FORMAT_PRINT <= 0 )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.print[0];
            }
            break;

        case SMJOB_TYPE_SCAN:
            if ( SMJOB_NUM_FORMAT_SCAN <= 0 )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.scan[0];
            }
            break;

        case SMJOB_TYPE_FAXIN:
            if ( SMJOB_NUM_FORMAT_FAXIN <= 0 )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.faxin[0];
            }
            break;

        case SMJOB_TYPE_FAXOUT:
            if ( SMJOB_NUM_FORMAT_FAXOUT <= 0 )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.faxout[0];
            }
            break;

        default:
            format = SMJOB_FORMAT_UNKNOWN;
            break;
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *value = format;
    }

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_format(smjob_type_t job_type, uint32_t index, smjob_format_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    smjob_format_t *format;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Lookup the value per job_type and index
    // For each job_type, verify that the index isn't out of bounds
    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
            if ( SMJOB_NUM_FORMAT_PRINT <= index )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.print[index];
            }
            break;

        case SMJOB_TYPE_SCAN:
            if ( SMJOB_NUM_FORMAT_SCAN <= index )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.scan[index];
            }
            break;

        case SMJOB_TYPE_FAXIN:
            if ( SMJOB_NUM_FORMAT_FAXIN <= index )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.faxin[index];
            }
            break;

        case SMJOB_TYPE_FAXOUT:
            if ( SMJOB_NUM_FORMAT_FAXOUT <= index )
            {
                format = SMJOB_FORMAT_UNKNOWN;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                format = g_smjob_oem_format_table.faxout[index];
            }
            break;

        default:
            format = SMJOB_FORMAT_UNKNOWN;
            break;
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *value = format;
    }

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_finishings(smjob_type_t job_type, uint32_t index, smjob_finishing_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    smjob_finishing_t finishing;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    finishing = SMJOB_FINISHING_NONE; // default returned on error

    // Lookup the value per job_type and index
    // For each job_type, verify that the index isn't out of bounds
    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
            if ( SMJOB_NUM_FINISHINGS_PRINT <= index )
            {
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                finishing = g_smjob_oem_finishings_table.print[index];
            }
            break;

        default:
            sm_res = SMJOB_FAIL; // unsupported/invalid job type
            break;
    }

    // Assign result to the return value
    *value = finishing;

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_orientation(smjob_type_t job_type, smjob_orientation_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        return SMJOB_FAIL;
    }

    // Everyone loves PORTRAIT!
    *value = SMJOB_ORIENTATION_PORTRAIT;

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_orientation(smjob_type_t job_type, uint32_t index, smjob_orientation_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    smjob_orientation_t orientation = SMJOB_ORIENTATION_PORTRAIT;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Lookup the value per job_type and index
    // For each job_type, verify that the index isn't out of bounds
    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
            if ( SMJOB_NUM_ORIENTATION_PRINT <= index )
            {
                orientation = SMJOB_ORIENTATION_PORTRAIT;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                orientation = g_smjob_oem_orientation_table.print[index];
            }
            break;

        case SMJOB_TYPE_SCAN:
            if ( SMJOB_NUM_ORIENTATION_SCAN <= index )
            {
                orientation = SMJOB_ORIENTATION_PORTRAIT;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                orientation = g_smjob_oem_orientation_table.scan[index];
            }
            break;

        case SMJOB_TYPE_FAXIN:
            if ( SMJOB_NUM_ORIENTATION_FAXIN <= index )
            {
                orientation = SMJOB_ORIENTATION_PORTRAIT;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                orientation = g_smjob_oem_orientation_table.faxin[index];
            }
            break;

        case SMJOB_TYPE_FAXOUT:
            if ( SMJOB_NUM_ORIENTATION_FAXOUT <= index )
            {
                orientation = SMJOB_ORIENTATION_PORTRAIT;
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                orientation = g_smjob_oem_orientation_table.faxout[index];
            }
            break;

        default:
            orientation = SMJOB_ORIENTATION_PORTRAIT;
            break;
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *value = orientation;
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_resolution(smjob_type_t job_type, smjob_resolution_t *value)
{
    static const smjob_resolution_t resolution =
    {
        .xfeed_dir = 600,
        .feed_dir  = 600,
        .units     = SMJOB_RES_UNIT_DOTS_PER_INCH,
    };

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        return SMJOB_FAIL;
    }

    *value = resolution;
    return SMJOB_OK;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_resolution(smjob_type_t job_type, uint32_t index, smjob_resolution_t *value)
{
    smjob_rcode_t      sm_res = SMJOB_OK;
    smjob_resolution_t resolution = {600,600,SMJOB_RES_UNIT_DOTS_PER_INCH};

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        return SMJOB_FAIL;
    }

    // Start with an initialized resoltion
    //memset(value, 0, sizeof(smjob_resolution_t));

    // Lookup the value per job_type and index
    // For each job_type, verify that the index isn't out of bounds
    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
            // Verify that the index isn't out of bounds
            if ( SMJOB_NUM_RESOLUTION_PRINT <= index )
            {
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                resolution = g_smjob_oem_resolution_table[0];
            }
            break;

        case SMJOB_TYPE_SCAN:
            resolution = g_smjob_oem_resolution_table[1];
            break;

        case SMJOB_TYPE_FAXIN:
            resolution = g_smjob_oem_resolution_table[2];
            break;

        case SMJOB_TYPE_FAXOUT:
            resolution = g_smjob_oem_resolution_table[3];
            break;

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *value = resolution;
    }

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_size_x_dim(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if(index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }
            *value = table[index].width;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_size_y_dim(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if(index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }
            *value = table[index].height;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_top_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if (index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                // Default value
                *value = 300;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_bottom_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if (index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                // Default value
                *value = 300;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_left_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if (index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                // Default value
                *value = 300;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_right_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if (index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                // Default value
                *value = 300;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_aiprint_enabled(smjob_type_t job_type, 
                                                         uint32_t     index, 
                                                         uint32_t     *value)
{
   smjob_rcode_t sm_res = SMJOB_OK;

   if (!value)
   {
      sm_res = SMJOB_FAIL;
   }
   else
   {
      *value = 1;
   }

   return sm_res;
}


//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_media_col(smjob_type_t job_type, smjob_media_col_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            // No default implementation of this; core code not wired to use this (yet)
            memset(value, 0, sizeof(smjob_media_col_t));
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_size_name(smjob_type_t job_type, uint32_t index, char** name)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !name )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if(index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }
            *name = table[index].name;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_default_media_size_name(smjob_type_t job_type, char** name)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !name )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            *name = sm_media_size_enum_to_name(MEDIASIZE_LETTER);
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

smjob_rcode_t SY_ATTR_WEAK smjob_oem_set_supported_default_media_size_name(smjob_type_t job_type, char* name, uint32_t name_len)
{
    return SMJOB_FAIL;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_type_name(smjob_type_t job_type, uint32_t index, char** name)
{
   smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !name )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_type_entry_t *table;
            uint32_t cnt = sm_media_type_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if(index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }
            *name = table[index]->name;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_default_media_type_name(smjob_type_t job_type, char** name)
{
   smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !name )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            *name = sm_media_type_enum_to_name(MEDIATYPE_MIDWEIGHT_96_110G);
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_media_table_type(smjob_type_t job_type, uint32_t index, char** name)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    static const char *media_type = "stationery";

    // Check inputs
    if( !name )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    // This default code will say that each media table entry is stationery type.
    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if (index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                *name = (char *)media_type;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_media_table_duplex(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    // This default code will say that each media table entry is not duplexable.
    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if (index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                *value = 0;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_media_source(smjob_type_t job_type, uint32_t index, char** source)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    static const char *media_source = "main";

    // Check inputs
    if( !source )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    // This default code will say that we only have the 'main' tray.
    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            switch(index)
            {
              case 0:
                *source = (char *)media_source;
                break;
              default:
                *source = NULL;
                sm_res = SM_BAD_INDEX;
                break;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_media_table_source(smjob_type_t job_type, uint32_t index, char** source)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    static const char *media_source = "main";

    // Check inputs
    if( !source )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    // This default code will say that each media table entry is in the 'main' tray.
    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if (index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
            }
            else
            {
                *source = (char *)media_source;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}


//------------------------------------------------------------------------------------------------------
// Scan only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_input_source(uint32_t index, smjob_input_source_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    smjob_input_source_t input_source = SMJOB_INPUT_SOURCE_ADF;

    // Check inputs
    if ( 0 == value )
    {
        DBG_PRINTF_ERR("%s: Invalid value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Verify that the index isn't out of bounds
    if ( SMJOB_NUM_INPUT_SOURCE_SCAN <= index )
    {
        sm_res = SM_BAD_INDEX;
    }
    else
    {
        input_source = g_smjob_oem_input_source_table.scan[index];
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *value = input_source;
    }

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_copies(uint32_t *value)
{
    // Check inputs
    if ( NULL == value )
    {
        DBG_PRINTF_ERR("%s: Invalid value provided\n", __FUNCTION__);
        return SMJOB_FAIL;
    }

    // Assign result to the return value
    *value = SMJOB_PRINT_MIN_COPIES;

    return SMJOB_OK;
}

//------------------------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_copies(uint32_t *min_value, uint32_t *max_value)
{
    // Check inputs
    if ( ( NULL == min_value ) || ( NULL == max_value ) )
    {
        DBG_PRINTF_ERR("%s: Invalid min or max value provided\n", __FUNCTION__);
        return SMJOB_FAIL;
    }

    // Assign result to the return value
    *min_value = SMJOB_PRINT_MIN_COPIES;
    *max_value = SMJOB_PRINT_MAX_COPIES;

    return SMJOB_OK;
}

//------------------------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_sheet_collate(bool *is_supported)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( 0 == is_supported )
    {
        DBG_PRINTF_ERR("%s: Invalid bool provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *is_supported = SMJOB_PRINT_SHEET_COLLATE;
    }

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_sides(smjob_sides_t **value)
{
    // Check inputs
    if ( NULL == value )
    {
        DBG_PRINTF_ERR("%s: Invalid value provided\n", __FUNCTION__);
        return SMJOB_FAIL;
    }

    *value = SMJOB_SIDES_ONE_SIDED;

    return SMJOB_OK;
}

//------------------------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_sides(uint32_t      index, 
                                                         smjob_sides_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_sides_t *sides = NULL;

    // Check inputs
    if ( NULL == value )
    {
        DBG_PRINTF_ERR("%s: Invalid value provided\n", __FUNCTION__);
        return SMJOB_FAIL;
    }

    // Verify that the index isn't out of bounds
    if ( SMJOB_NUM_SIDES_PRINT <= index )
    {
        sm_res = SM_BAD_INDEX;
    }
    else
    {
        sides = g_smjob_oem_sides_table.print[index];
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *value = sides;
    }

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_quality(smjob_quality_t *value)
{
    // Check inputs
    if ( NULL == value )
    {
        DBG_PRINTF_ERR("%s: Invalid value provided\n", __FUNCTION__);
        return SMJOB_FAIL;
    }

    *value = SMJOB_QUALITY_NORMAL;

    return SMJOB_OK;
}

//------------------------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_quality(uint32_t index, smjob_quality_t *value)
{
    smjob_rcode_t   sm_res = SMJOB_OK;
    smjob_quality_t quality = SMJOB_QUALITY_DRAFT;

    // Check inputs
    if ( NULL == value )
    {
        DBG_PRINTF_ERR("%s: Invalid value provided\n", __FUNCTION__);
        return SMJOB_FAIL;
    }

    // Verify that the index isn't out of bounds
    if ( SMJOB_NUM_QUALITY_PRINT <= index )
    {
        sm_res = SM_BAD_INDEX;
    }
    else
    {
        quality = g_smjob_oem_quality_table.print[index];
    }

    // Assign result to the return value
    if ( SMJOB_OK == sm_res )
    {
        *value = quality;
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_color_mode(smjob_type_t       job_type,
                                                            smjob_color_mode_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        return SMJOB_FAIL;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
#ifdef COLOR_ENG
            *value = SMJOB_COLOR_MODE_COLOR;
#else
            *value = SMJOB_COLOR_MODE_MONO;
#endif
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_color_mode(smjob_type_t       job_type,
                                                              uint32_t           index,
                                                              smjob_color_mode_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        return SMJOB_FAIL;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            switch(index)
            {
                case 0:
                    *value = SMJOB_COLOR_MODE_MONO;
                    break;
                case 1:
                    *value = SMJOB_COLOR_MODE_COLOR;
                    break;
                case 2:
                    *value = SMJOB_COLOR_MODE_AUTO;
                    break;
                default:
                    // index out of range
                    sm_res = SM_BAD_INDEX;
                    break;
            }
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

    return sm_res;
}

//------------------------------------------------------------------------------------------------------
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_output_bin_default(smjob_type_t job_type, char** name)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    static const char *default_bin_name = "face-down";

    // Check inputs
    if( !name )
    {
        return SMJOB_FAIL;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            *name = (char *)default_bin_name;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

    return sm_res;
}

smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_output_bin_supported(smjob_type_t job_type, uint32_t index, char** name)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    static const char *bin_name = "face-down";

    // Check inputs
    if( !name )
    {
        return SMJOB_FAIL;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            if (index > 0)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }

            *name = (char *)bin_name;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

    return sm_res;
}

smjob_rcode_t SY_ATTR_WEAK smjob_oem_markers_available(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    smjob_rcode_t   sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        goto done;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            if (index > 0)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }

            *value = 0; // markers not available
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

done:
    return sm_res;
}

//---------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_default_print_content_optimize(smjob_print_content_optimize_t **value)
{
   *value = g_smjob_print_content_optimize_table[0];

   return(SMJOB_OK);
}

//---------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_print_content_optimize(uint32_t                       index, 
                                               smjob_print_content_optimize_t **value)
{
   smjob_rcode_t sm_res = SMJOB_OK;

   if (index < g_smjob_print_content_optimize_table_size)
   {
      *value = g_smjob_print_content_optimize_table[index];
   }
   else
   {
      sm_res = SM_BAD_INDEX;
   }

   return (sm_res);
}

smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_printer_geo_location(smjob_type_t job_type, char** location)
{
    // TODO - remove hardcode 

// per AirPrint specification ver. 1.3 section 6.7.48 and PWG 5100.13 (JSP3) section 5.6.28,
//  if value not specified or is unknown then we should use out-of-band value 'unknown' (value 
// tag defined in rfc2910 3.5.2).  However APVT will fail unless we use a dummy uri value, e.g. geo:0.0
    *location = "geo:0.0";

    return SMJOB_OK;
}
