/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_print.c
 *
 * \brief This file implements the OID back-end functions for the
 * print API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "printvars_api.h"
#include "paper_types_api.h"
#include "agprint.h"

#include "dprintf.h"
#include "lassert.h"
#include "utils.h"
#ifdef HAVE_STATISTICS
#include "statistics_api.h"
#include "statistics_vars.h"
#endif
#ifdef HAVE_RTC
#include "rtc_api.h"
#endif
#ifdef HAVE_EVENT_LOGGER
#include "event_logger_api.h"
#endif
#include "string_mgr_api.h"


void oid_module_print_translate_status_to_string( uint32_t ui_status, char ** status_string );


/* TODO REMOVE: Temporary until back-end for custom size is implemented */
static uint32_t custom_size_width = 4;
static uint32_t custom_size_height = 6;
static uint32_t custom_size_unit_of_measure = 230001;



/**
 * 
 * \brief The backend oid print API module function for retreiving
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
static error_type_t oid_module_print_get( oid_t oid,
                                          uint32_t index,
                                          char * data_ptr,
                                          uint32_t max_string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    switch ( oid )
    {
        case OID_PRINT_LOG_DATE:
        {
#ifdef HAVE_EVENT_LOGGER
            unsigned int string_size;
            event_logger_log_entry_t log;

            e_res = event_logger_get_log_entry( index, &log );

            if ( OK == e_res )
            {
                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len_in_bytes,
                        &string_size,
                        "%u-%u-%u",
                        log.time.year + 2000,
                        log.time.month,
                        log.time.day );

                ASSERT( string_size <= max_string_len_in_bytes );
                ASSERT( 0 != string_size );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }
            }
#else
            e_res = FAIL;
#endif

            break;
        }

        case OID_PRINT_HEAD_MODEL_NAME:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_HEAD_SERIAL_NUMBER:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_HEAD_PART_NUMBER:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_HEAD_INSTALL_DATE:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_HEAD_TOTAL_PAGES_PRINTED:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_HEAD_LAST_DATE_USED:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_HEAD_TOTAL_MAINT_CYCLES:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_HEAD_FACTORY_DATA:
        {
            e_res = FAIL;
            break;
        }

        case OID_PRINT_GENERAL_STATUS:
        {
            unsigned int string_size;
            e_res = min_n_k_sprintf( data_ptr,
                    max_string_len_in_bytes,
                    &string_size,
                    "Online" );

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

    return e_res;
}


/**
 * 
 * \brief The backend oid print API module function for 
 * retreiving data associated with an OID. 
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
static error_type_t oid_module_print_get_int( oid_t oid,
                                              uint32_t index,
                                              uint32_t * value)
{
    error_type_t e_res;
    e_res = OK;

    switch ( oid )
    {
        case OID_PRINT_LOG_CODE:
        {
#ifdef HAVE_EVENT_LOGGER
            event_logger_log_entry_t log;

            e_res = event_logger_get_log_entry( index, &log );

            if ( OK == e_res )
            {
                *value = log.code;
            }
#else
            e_res = FAIL;
#endif
            break;
        }

        case OID_PRINT_INPUT_TRAY_TRAY_ID:
        {
            printer_setup_t printer_setup;
            INPUTTRAY * input_tray;

            input_tray = NULL;
            e_res = FAIL;

            if ( index < NUM_INPUTS )
            {
                e_res = printvar_get_printersetup( &printer_setup );
            }

            if ( OK == e_res )
            {
                input_tray = &(printer_setup.inputTrays[index]);
                if (  NULL != input_tray )
                {
                    *value = input_tray->inputType;
                }
                else
                {
                    e_res = FAIL;
                }
            }

            break;
        }

        case OID_PRINT_INPUT_TRAY_MEDIA_SIZE:
        {
            mediacnfg_t media_config;

            e_res = FAIL;
            if ( index < NUM_INPUTS )
            {
                e_res = printvar_get_trayconfig( &media_config );
            }
            if ( OK == e_res )
            {
                *value = media_config.DefmediaSize;
            }
            break;
        }

        case OID_PRINT_INPUT_TRAY_MEDIA_TYPE:
        {
            mediacnfg_t media_config;

            e_res = FAIL;
            if ( index < NUM_INPUTS )
            {
                e_res = printvar_get_trayconfig( &media_config );
            }
            if ( OK == e_res )
            {
                *value = media_config.DefmediaType;
            }
            break;
        }
#ifdef HAVE_STATISTICS
        case OID_PRINT_NUM_COLOR_BEST:
        {
            uint32_t num_best;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_best = statistics_counts.print_page_counts.color_best_count;
            num_best = 0;

            if ( OK == e_res )
            {
                *value = num_best;
            }

            break;
        }

        case OID_PRINT_NUM_COLOR_NORM:
        {
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            if ( OK == e_res )
            {
                *value = statistics_counts.print_page_counts.page_count;
            }

            break;
        }

        case OID_PRINT_NUM_MONO_BEST:
        {
            uint32_t num_best;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_best = statistics_counts.print_page_counts.mono_best_count;
            num_best = 0;

            if ( OK == e_res )
            {
                *value = num_best;
            }

            break;
        }

        case OID_PRINT_NUM_MONO_NORM:
        {
            uint32_t num_norm;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_norm = statistics_counts.print_page_counts.mono_normal_count;
            num_norm = 0;

            if ( OK == e_res )
            {
                *value = num_norm;
            }

            break;
        }
#endif
        case OID_PRINT_MEDIA_SIZE_DEFAULT:
        {
            mediasize_t size;
            e_res = printvar_get_default_papersize( &size );
            if ( OK == e_res )
            {
                *value = size;
            }
            break;
        }

        case OID_PRINT_MEDIA_SIZE_LETTER_ID:
        {
            *value = MEDIASIZE_LETTER;
            break;
        }
#ifdef HAVE_STATISTICS
        case OID_PRINT_MEDIA_SIZE_LETTER_NUM_BEST:
        {
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );

            if ( OK == e_res )
            {
                //*value = statistics_counts.print_page_counts.a4_letter_page_count;
                *value = statistics_counts.print_page_counts.page_count;
            }

            break;
        }

        case OID_PRINT_MEDIA_SIZE_LETTER_NUM_NORM:
        {
            uint32_t num_norm;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_norm = statistics_counts.print_page_counts.a4_letter_page_count;
            num_norm = 0;

            if ( OK == e_res )
            {
                *value = num_norm;
            }

            break;
        }
#endif
        case OID_PRINT_MEDIA_SIZE_LEGAL_ID:
        {
            *value = MEDIASIZE_LEGAL;
            break;
        }

        case OID_PRINT_MEDIA_TYPE_PLAIN_ID:
        {
            *value = MEDIATYPE_PLAIN_PAPER;
            break;
        }
#ifdef HAVE_STATISTICS
        case OID_PRINT_MEDIA_TYPE_PLAIN_NUM_BEST:
        {
            uint32_t num_best;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_best = statistics_counts.print_page_counts.plain_paper_count;
            num_best = statistics_counts.print_page_counts.page_count;

            if ( OK == e_res )
            {
                *value = num_best;
            }

            break;
        }

        case OID_PRINT_MEDIA_TYPE_PLAIN_NUM_NORM:
        {
            uint32_t num_norm;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_norm = statistics_counts.print_page_counts.plain_paper_count;
            num_norm = 0;

            if ( OK == e_res )
            {
                *value = num_norm;
            }

            break;
        }
#endif

        case OID_PRINT_MEDIA_TYPE_GLOSSY_ID:
        {
            *value = MEDIATYPE_GLOSSY1;
            break;
        }

#ifdef HAVE_STATISTICS
        case OID_PRINT_MEDIA_TYPE_GLOSSY_NUM_BEST:
        {
            uint32_t num_best;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_best = statistics_counts.print_page_counts.glossy_paper_count;
            num_best = 0;

            if ( OK == e_res )
            {
                *value = num_best;
            }

            break;
        }

        case OID_PRINT_MEDIA_TYPE_GLOSSY_NUM_NORM:
        {
            uint32_t num_norm;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_norm = statistics_counts.print_page_counts.glossy_paper_count;
            num_norm = 0;

            if ( OK == e_res )
            {
                *value = num_norm;
            }

            break;
        }
#endif
        case OID_PRINT_INPUT_TYPE_DEFAULT:
        {
            size_type_tray_t print_size_type_tray_info;
            printer_setup_t printer_setup;
            INPUTTRAY * input_tray;

            input_tray = NULL;

            e_res = printvar_get_sizetypetray( &print_size_type_tray_info );

            if ( OK == e_res )
            {
                e_res = printvar_get_printersetup( &printer_setup );
            }

            if ( OK == e_res )
            {
                input_tray = &( printer_setup.inputTrays[ print_size_type_tray_info.tray ] );
                if ( NULL != input_tray )
                {
                    *value = input_tray->inputType;
                }
                else
                {
                    e_res = FAIL;
                }
            }

            break;
        }

        case OID_PRINT_MEDIA_TYPE_DEFAULT:
        {
            mediatype_t type;
            e_res = printvar_get_default_papertype( &type );
            if ( OK == e_res )
            {
                *value = type; 
            }
            break;
        }
#ifdef HAVE_STATISTICS
        case OID_PRINT_MEDIA_SIZE_LEGAL_NUM_NORM:
        {
            uint32_t num_norm;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_norm = statistics_counts.print_page_counts.legal_page_count;
            num_norm = 0;

            if ( OK == e_res )
            {
                *value = num_norm;
            }

            break;
        }

        case OID_PRINT_MEDIA_SIZE_LEGAL_NUM_BEST:
        {
            uint32_t num_best;
            statistics_t    statistics_counts;

            e_res = statistics_get ( &statistics_counts );
            //num_best = statistics_counts.print_page_counts.legal_page_count;
            num_best = 0;

            if ( OK == e_res )
            {
                *value = num_best;
            }

            break;
        }
#endif

        case OID_PRINT_MEDIA_SIZE_CUSTOM_WIDTH:
        {
            /* TODO USE REAL BACK-END IMPLEMENTATION ONCE IMPLEMENTED */
            *value = custom_size_width;
            break;
        }

        case OID_PRINT_MEDIA_SIZE_CUSTOM_HEIGHT:
        {
            /* TODO USE REAL BACK-END IMPLEMENTATION ONCE IMPLEMENTED */
            *value = custom_size_height;
            break;
        }

        case OID_PRINT_MEDIA_SIZE_CUSTOM_UNIT_OF_MEASURE:
        {
            /* TODO USE REAL BACK-END IMPLEMENTATION ONCE IMPLEMENTED */
            *value = custom_size_unit_of_measure;
            break;
        }

        case OID_PRINT_JAM_RECOVERY:
        {
            uint8_t jam_recovery;
            e_res = printvar_get_jamrecovery( &jam_recovery );
            if ( OK == e_res )
            {
                *value = (uint32_t)jam_recovery;
            }
            break;
        }
        case OID_PRINT_NUM_INPUT_TRAYS:
        {
            *value = NUM_INPUTS;
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
 * \brief The backend oid print API module function for 
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
static error_type_t oid_module_print_set_int( oid_t oid,
                                              uint32_t index,
                                              uint32_t value)
{
    error_type_t e_res = OK;

    switch ( oid )
    {
        case OID_PRINT_INPUT_TRAY_MEDIA_TYPE:
        {
            mediacnfg_t val;
          
            e_res = paper_media_type_is_valid( (mediatype_t)value );
            
            if ( OK != e_res )
            {
                e_res = OID_ERROR_VALUE_TOO_BIG;
            }
                 
            if ( OK == e_res )
            {
                e_res = printvar_get_trayconfig( &val );
            }

            if ( OK == e_res )
            {
		    if (index < NUM_INPUTS)
			    val.mediaType[index] =  (mediatype_t)value;
		    else
			    val.DefmediaType = (mediatype_t)value;
		    e_res=printvar_set_trayconfig(&val);
            }

            break;
        }

        case OID_PRINT_MEDIA_TYPE_DEFAULT:
            e_res = printvar_set_default_papertype( (mediatype_t)value );
            break;

        case OID_PRINT_INPUT_TRAY_MEDIA_SIZE:
         {
            mediacnfg_t val;
            uint32_t width;
            uint32_t height;

            if ( OK == e_res )
            {
                /* validate user input */
                e_res = paper_get_media_size( (mediasize_t)value, &width, &height);

                if ( OK != e_res )
                {
                    e_res = OID_ERROR_VALUE_TOO_BIG;
                }
            }       

            if ( OK == e_res )
            {
                e_res = printvar_get_trayconfig( &val );
            }

            if ( OK == e_res )
            {
		    if (index < NUM_INPUTS)
			    val.mediaType[index] = (mediasize_t)value;
		    else
			    val.DefmediaSize = (mediasize_t)value;
                e_res=printvar_set_trayconfig(&val);
            }

            break;
        }
        
        case OID_PRINT_MEDIA_SIZE_DEFAULT:
        { 
            e_res = printvar_set_default_papersize( (mediasize_t *)&value );
            break;
        }

        case OID_PRINT_INPUT_TYPE_DEFAULT:
        {
            size_type_tray_t print_size_type_tray_info;
            printer_setup_t printer_setup;
            INPUTTRAY * input_tray;

            input_tray = NULL;

            e_res = printvar_get_sizetypetray( &print_size_type_tray_info );
            if ( OK == e_res )
            {
                e_res = printvar_get_printersetup( &printer_setup );
            }

            if ( OK == e_res )
            {
                input_tray = &( printer_setup.inputTrays[ print_size_type_tray_info.tray ] );
                if ( NULL != input_tray )
                {
                    /* save value into input tray inputType */
                    input_tray->inputType = (INPUTTYPE)value;
                    e_res = printvar_set_printersetup( &printer_setup );
                }
                else
                {
                    e_res = FAIL;
                }
            }
            break;
        }

        case OID_PRINT_MEDIA_SIZE_CUSTOM_WIDTH:
        {
            /* TODO USE REAL BACK-END IMPLEMENTATION ONCE IMPLEMENTED */
            custom_size_width = value;
            break;
        }

        case OID_PRINT_MEDIA_SIZE_CUSTOM_HEIGHT:
        {
            /* TODO USE REAL BACK-END IMPLEMENTATION ONCE IMPLEMENTED */
            custom_size_height = value;
            break;
        }

        case OID_PRINT_MEDIA_SIZE_CUSTOM_UNIT_OF_MEASURE:
        {
            /* TODO USE REAL BACK-END IMPLEMENTATION ONCE IMPLEMENTED */
            custom_size_unit_of_measure = value;
            break;
        }

        case OID_PRINT_JAM_RECOVERY:
        {
            uint8_t tempio;
            tempio = (uint8_t) value;
            e_res = printvar_set_jamrecovery(&tempio);
            break;
        }

        default:
            e_res = FAIL;
            break;
    }
    return e_res;
}



/**
 *
 * \brief Registers each print API OID with the oid controller
 *
 */
void oid_register_module_print( void )
{
    error_type_t e_res;

    e_res = oid_register_callbacks( OID_PRINT_LOG_DATE,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res ); // code review check for OK not a specific error

    e_res = oid_register_callbacks( OID_PRINT_LOG_CODE,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_INPUT_TRAY_TRAY_ID,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_INPUT_TRAY_MEDIA_SIZE,
                                    oid_module_print_get_int,
                                    oid_module_print_set_int,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_INPUT_TRAY_MEDIA_TYPE,
                                    oid_module_print_get_int,
                                    oid_module_print_set_int,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_MODEL_NAME,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_SERIAL_NUMBER,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_PART_NUMBER,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_INSTALL_DATE,
                                    oid_module_print_get,
                                    oid_module_null_set,    
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_TOTAL_PAGES_PRINTED,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_LAST_DATE_USED,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_TOTAL_MAINT_CYCLES,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_HEAD_FACTORY_DATA,
                                    oid_module_print_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_NUM_COLOR_BEST,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_NUM_COLOR_NORM,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_NUM_MONO_BEST,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_NUM_MONO_NORM,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_SIZE_LETTER_ID,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_SIZE_LETTER_NUM_BEST,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_SIZE_LETTER_NUM_NORM,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_SIZE_LEGAL_ID,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_SIZE_LEGAL_NUM_BEST,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_SIZE_LEGAL_NUM_NORM,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_TYPE_PLAIN_ID,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_TYPE_PLAIN_NUM_BEST,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_TYPE_PLAIN_NUM_NORM,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_TYPE_GLOSSY_ID,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_TYPE_GLOSSY_NUM_BEST,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_TYPE_GLOSSY_NUM_NORM,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_INPUT_TYPE_DEFAULT,
                                    oid_module_print_get_int,
                                    oid_module_print_set_int,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_SIZE_DEFAULT,
                                    oid_module_print_get_int,
                                    oid_module_print_set_int,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_MEDIA_TYPE_DEFAULT,
                                    oid_module_print_get_int,
                                    oid_module_print_set_int,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks(OID_PRINT_JAM_RECOVERY,
                                   oid_module_print_get_int,
                                   oid_module_print_set_int,
                                   oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks(OID_PRINT_GENERAL_STATUS,
                                   oid_module_print_get,
                                   oid_module_null_set,
                                   oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks(OID_PRINT_MEDIA_SIZE_CUSTOM_WIDTH,
                                   oid_module_print_get_int,
                                   oid_module_print_set_int,
                                   oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks(OID_PRINT_MEDIA_SIZE_CUSTOM_HEIGHT,
                                   oid_module_print_get_int,
                                   oid_module_print_set_int,
                                   oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks(OID_PRINT_MEDIA_SIZE_CUSTOM_UNIT_OF_MEASURE,
                                   oid_module_print_get_int,
                                   oid_module_print_set_int,
                                   oid_module_null_test );
    ASSERT( OK == e_res );

    e_res = oid_register_callbacks( OID_PRINT_PPM_MONO,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );

     e_res = oid_register_callbacks( OID_PRINT_PPM_COLOR,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );
    e_res = oid_register_callbacks( OID_PRINT_NUM_INPUT_TRAYS,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );
    e_res = oid_register_callbacks( OID_PRINT_SIMPLEX_PAGES,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );
    e_res = oid_register_callbacks( OID_PRINT_DUPLEX_PAGES,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );
    e_res = oid_register_callbacks( OID_PRINT_PCL_PAGES,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );
    e_res = oid_register_callbacks( OID_PRINT_OUTPUT_JAM_COUNT,
                                    oid_module_print_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( OK == e_res );


}


#if 0

//rowberry: Can't use strings from the oem directory in common code - this function needs to move to somewhere in OEM
  
/**
 *
 * \brief Translates a status value to a string
 * to be displayed on the EWS
 *
 */
void oid_module_print_translate_status_to_string( uint32_t ui_status, char ** status_string )
{
    string_mgr_code_t string_mgr_code;

    ASSERT( status_string != NULL );

    switch ( ui_status )
    {
        case STATUS_ONLINE:
            string_mgr_code = STRING_STAT_READY;
            break;

        case STATUS_MAINTENANCE_BUSY:
            string_mgr_code = STRING_EWS_MAINTENANCE;

            break;

        case STATUS_DOOROPEN:
            /* fall through */
        case STATUS_DOOROPEN_FRONT:
            /* fall through */
        case STATUS_DOOROPEN_FD:
            string_mgr_code = STRING_EWS_DOOR_OPEN;

            break;

        case STATUS_PAPER_OUT:
            /* fall through */
        case STATUS_PAPER_OUT_TRAY1:
            /* fall through */
        case STATUS_PAPER_OUT_TRAY2:
            /* fall through */
        case STATUS_PAPER_OUT_TRAY3:
            /* fall through */
        case STATUS_PAPER_OUT_TRAY4:
            /* fall through */
        case STATUS_PAPER_OUT_ALL:
            string_mgr_code = STRING_CP_PAPER_OUT;

            break;

        case STATUS_PAPERPATH_ERROR:
            /* fall through */
        case STATUS_MAINTENANCE_JAM:
            /* fall through */
        case STATUS_MISPRINT:
            /* fall through */
        case STATUS_MISPRINT_RESERVATION:
            /* fall through */
        case STATUS_MISPRINT_BD:
            /* fall through */
        case STATUS_MISPRINT_PAPER_SIZE:
            /* fall through */
        case STATUS_MISPRINT_SENDING_ERROR:
            /* fall through */
        case STATUS_MISPRINT_MISPICK:
            /* fall through */
        case STATUS_PAPERJAM:
            /* fall through */
        case STATUS_PAPER_JAM_INPUT:
            /* fall through */
        case STATUS_PAPER_JAM_TRAY1:
            /* fall through */
        case STATUS_PAPER_JAM_TRAY2:
            /* fall through */
        case STATUS_PAPER_JAM_TRAY3:
            /* fall through */
        case STATUS_PAPER_JAM_OUTPUT:
            /* fall through */
        case STATUS_PAPER_JAM_DOOR_OPEN:
            string_mgr_code = STRING_CP_PAPER_JAMMED;

            break;

        #if 0
        /* handled in default case */
        case STATUS_CONNECTION_FAIL:
        case STATUS_CONNECTION_DUPLEXOR_FAIL:
        case STATUS_CONNECTION_READ_FAIL:
        #endif

        case STATUS_JOBINFO_PAGE:
            /* fall through */
        case STATUS_JOBINFO_START_JOB:
            /* fall through */
        case STATUS_INFO_PRINTING:
            /* fall through */
        case STATUS_INFO_PRINTING_INTERNAL:
            /* fall through */
        case STATUS_INFO_PRINTING_DEMO:
            /* fall through */
        case STATUS_INFO_PRINTING_CNFG:
            /* fall through */
        case STATUS_INFO_PRINTING_MENU:
            /* fall through */
        case STATUS_INFO_PRINTING_SUPPLIES:
            string_mgr_code = STRING_EWS_PRINT_ONLY;

            break;

        default:
            string_mgr_code = STRING_EWS_NETWORK_ERROR;

            break;
    }

    *status_string = (char *)string_mgr_get_string( string_mgr_code, string_mgr_loc_full );
    ASSERT( NULL != *status_string );
}
#endif

