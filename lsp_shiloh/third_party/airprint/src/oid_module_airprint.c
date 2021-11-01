/*
 * ============================================================================
 * (C) Copyright 2013   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * @file oid_airprint.c  
 *
 * @brief Contains OID implementations for the proprietary Apple AirPrint specification,
 * which is an extension of the PWG IPP specification.
 * 
 * NOTE: Release of this source file to SDK customers is restricted.
 */

//=========================================================================================
// Includes
//=========================================================================================
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "oid_airprint_api.h"
#include "sm_job_api.h"
#include "airprint_api.h"
#include "engPaperHandlingAPI.h"

#ifdef HAVE_VPI_SUPPORT
#include "print_events_common_api.h" // for hack
#include "vpi_printer_cfg_api.h"
#else
#include "consumables_common_api.h"
#endif // HAVE_VPI_SUPPORT


//=========================================================================================
// Enums
//=========================================================================================

// Used to develop the URF string passed back in Bonjour and IPP.
typedef enum
{
    CP = 0,
    IS,
    OB,
    PQ,
    RS,
    W,
    V,
    FN,
    DM,
    SRGB,

    //  Make this the last one.
    NUM_URF_KEY_PREFIXES

} URF_key_prefixes_e;


// TODO move to config layer
/**
* @brief The print scaling settings.
*/
static airprint_scaling_t *g_airprint_scaling_table[] =
{
   AIRPRINT_SCALING_AUTO, // default
   AIRPRINT_SCALING_AUTO_FIT,
   AIRPRINT_SCALING_FILL,
   AIRPRINT_SCALING_FIT,
   AIRPRINT_SCALING_NONE
};

static uint32_t g_airprint_scaling_table_size = (sizeof(g_airprint_scaling_table) /
                                                    sizeof(airprint_scaling_t *));


//=========================================================================================
// Local function declarations
//=========================================================================================

static bool find_URF_str_given_index(uint32_t index, char *buffer, 
                                     uint32_t buffer_len, uint32_t *bytes_written);
static void add_URF_str_delimiter(char URF_delimit, char *buffer, uint32_t buffer_len,
                                  uint32_t *bytes_written);
static void handle_URF_copies(char *buffer, uint32_t buffer_len, uint32_t *bytes_written);
static void handle_input_slot(char *buffer, uint32_t buffer_len, uint32_t *bytes_written);
static void handle_output_bin(char *buffer, uint32_t buffer_len, uint32_t *bytes_written);
static void handle_print_quality(char *buffer, uint32_t buffer_len, uint32_t *bytes_written);
static void handle_resolution(char *buffer, uint32_t buffer_len, uint32_t *bytes_written);
static void handle_luminance_color_space(char *buffer, uint32_t buffer_len, 
                                         uint32_t *bytes_written);
static void handle_URF_version(char *buffer, uint32_t buffer_len, uint32_t *bytes_written);
static void handle_duplex(char *buffer, uint32_t buffer_len, uint32_t *bytes_written);
static void handle_sRGB_color_space(char *buffer, uint32_t buffer_len, 
                                    uint32_t *bytes_written);
static char *get_URF_prefix_str(URF_key_prefixes_e URF_prefix);


//=========================================================================================
// Defines
//=========================================================================================

#define URF_STR_DELIMITER ','


/**
 * 
 * \brief The backend Apple AirPrint API module function for retreiving
 * string data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *  
 * \param[in] index Index of the OID value to be accessed. Ignored for scalar OID values. 
 *  
 * \param[in] oid_data Pointer to storage allocated by caller populated with a 
 *       NULL-terminated string representing the OID value.
 *
 * \param[in] oid_len Size of the buffer pointed to by 'oid_value'.  The maximum number of
 *       characters that can be stored in the buffer including the terminating NULL
 *       character.
 *
 * \return One of the error_type_t return codes:
 * * OK                      - success
 * * OID_ERROR_INDEX_INVALID - index out of range
 * * OID_ERROR_BUFFER_TOO_SMALL  - buffer of size 'oid_len' was too small to fit the 
 *                             entire string
 * * FAIL                    - bad oid, other error
 *
 */
static error_type_t oid_module_airprint_get_string( oid_t oid,
                                                    uint32_t oid_index,
                                                    char * oid_value,
                                                    uint32_t oid_len )
{
    ASSERT(oid_len >= 1); // at least room for null string
    oid_value[0] = '\0'; // default to NULL string
    error_type_t e_res = OK;

    /* any string assigned to this variable will be verified for length less than oid_len and
       then copied to oid_value at end of the oid switch statement */
    char *tmp_value = NULL; 

    switch ( oid )
    {

        case OID_AIRPRINT_MARKER_COLORS:
        {
#ifdef HAVE_VPI_SUPPORT // TODO expose this via consumables_common_api.h
            vpi_prt_rcode_t vpi_res;
            vpi_res = vpi_prt_get_marker_color(oid_index, &tmp_value);
            if (vpi_res == VPI_PRT_BAD_INDEX)
            {
                e_res = OID_ERROR_INDEX_INVALID;
            }
            else if (vpi_res != VPI_PRT_OK) 
            {
                e_res = FAIL;
            }
#else // !HAVE_VPI_SUPPORT
            int num_consumables = consumable_get_num_consumables();
            if(oid_index >= num_consumables)
            {
                // index out of range
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            consumable_color_t color; 
            color = consumable_get_color(oid_index);
            switch(color)
            {
                case CONSUMABLE_COLOR_BLACK:
                    tmp_value = "#000000";
                    break;
                case CONSUMABLE_COLOR_CYAN:
                    tmp_value = "#00FFFF";
                    break;
                case CONSUMABLE_COLOR_YELLOW:
                    tmp_value = "#FFFF00";
                    break;
                case CONSUMABLE_COLOR_MAGENTA:
                    tmp_value = "#FF00FF";
                    break;
                case CONSUMABLE_COLOR_NONE:
                    tmp_value = "none";
                    break;
                default:
                    // TODO: add mappings for additional colors
                    XASSERT(0, color);
                    tmp_value = "";
                    e_res = FAIL;
                    break;
            }
#endif // HAVE_VPI_SUPPORT
            break;
        }

        case OID_AIRPRINT_MARKER_NAMES:
        {
#ifdef HAVE_VPI_SUPPORT // TODO expose this via consumables_common_api.h
            vpi_prt_rcode_t vpi_res;
            vpi_res = vpi_prt_get_marker_name(oid_index, &tmp_value);
            if (vpi_res == VPI_PRT_BAD_INDEX)
            {
                e_res = OID_ERROR_INDEX_INVALID;
            }
            else if (vpi_res != VPI_PRT_OK) 
            {
                e_res = FAIL;
            }
#else // !HAVE_VPI_SUPPORT
            int num_consumables = consumable_get_num_consumables();
            if(oid_index >= num_consumables)
            {
                // index out of range
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            consumable_type_t type;
            type = consumable_get_type(oid_index);

            // TODO: add support for other types
            XASSERT(type == CONSUMABLE_TYPE_TONER, type);

            consumable_color_t color; 
            color = consumable_get_color(oid_index);
            switch(color)
            {
                case CONSUMABLE_COLOR_BLACK:
                    tmp_value = "Black";
                    break;
                case CONSUMABLE_COLOR_CYAN:
                    tmp_value = "Cyan";
                    break;
                case CONSUMABLE_COLOR_YELLOW:
                    tmp_value = "Yellow";
                    break;
                case CONSUMABLE_COLOR_MAGENTA:
                    tmp_value = "Magenta";
                    break;
                default:
                    // TODO: add additional mappings
                    XASSERT(0, color);
                    tmp_value = "";
                    e_res = FAIL;
                    break;
            }
#endif // HAVE_VPI_SUPPORT
            break;
        }

        case OID_AIRPRINT_MARKER_TYPES:
        {
#ifdef HAVE_VPI_SUPPORT // TODO expose this via consumables_common_api.h
            vpi_prt_rcode_t vpi_res;
            vpi_res = vpi_prt_get_marker_type(oid_index, &tmp_value);
            if (vpi_res == VPI_PRT_BAD_INDEX)
            {
                e_res = OID_ERROR_INDEX_INVALID;
            }
            else if (vpi_res != VPI_PRT_OK) 
            {
                e_res = FAIL;
            }
#else // !HAVE_VPI_SUPPORT
            int num_consumables = consumable_get_num_consumables();
            if(oid_index >= num_consumables)
            {
                // index out of range
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            consumable_type_t type;
            type = consumable_get_type(oid_index);

            // TODO: add support for other types
            XASSERT(type == CONSUMABLE_TYPE_TONER, type);
            tmp_value = "toner-cartridge";
#endif // HAVE_VPI_SUPPORT
            break;
        }

        case OID_AIRPRINT_PDF_VERSIONS_SUPPORTED:
        {
            switch(oid_index)
            {
                case 0:
                    tmp_value = "none";
                    break;
                default:
                    e_res = OID_ERROR_INDEX_INVALID;
                    break;
            }

            break;
        }

        case OID_AIRPRINT_PRINT_SCALING_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

// TODO get from config layer
            XASSERT(g_airprint_scaling_table_size >= 1, g_airprint_scaling_table_size);
            tmp_value = g_airprint_scaling_table[0];

            break;
        }

        case OID_AIRPRINT_PRINT_SCALING_SUPPORTED:
        {
            if (oid_index < g_airprint_scaling_table_size)
            {
               tmp_value = g_airprint_scaling_table[oid_index];
            }
            else
            {
               e_res = OID_ERROR_INDEX_INVALID;
            }

            break;
        }

        case OID_AIRPRINT_PRINTER_KIND:
        {
            switch(oid_index)
            {
                case 0:
                    tmp_value = "document";
                    break;
                case 1:
                    tmp_value = "envelope";
                    break;
                case 2:
                    tmp_value = "label";
                    break;
                case 3:
                    tmp_value = "photo";
                    break;
                default:
                    e_res = OID_ERROR_INDEX_INVALID;
                    break;
            }

            break;
        }

        case OID_AIRPRINT_URF_SUPPORTED:
        {
            if (oid_index < NUM_URF_KEY_PREFIXES)
            {
                bool found_URF_str;
                uint32_t bytes_written = 0;

                found_URF_str = find_URF_str_given_index(oid_index, 
                                                         oid_value, 
                                                         oid_len, 
                                                         &bytes_written);
                if (!found_URF_str)
                {
                    e_res = OID_ERROR_INDEX_UNAVAILABLE;
                }
            }
            else
            {
                e_res = OID_ERROR_INDEX_INVALID;
            }

            break;
        }

        case OID_AIRPRINT_PRINT_URF_CONFIGURED:
        {
            uint32_t i;
            bool     found_URF_str;
            uint32_t bytes_remaining    = oid_len;
            uint32_t bytes_written      = 0;
            uint32_t tot_bytes_written  = 0;

            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            // Go through and create a string with all the values for the URF string.
            for (i = 0; i < NUM_URF_KEY_PREFIXES; ++i)
            {
                bytes_written = 0;

                // Add a delimiter if it is not the first value.
                if ((i != 0))
                {
                    add_URF_str_delimiter(URF_STR_DELIMITER, 
                                          oid_value, 
                                          bytes_remaining, 
                                          &bytes_written);

                    bytes_remaining   -= 1;
                    tot_bytes_written += 1;

                    // Reset this once the delimiter is added.
                    bytes_written = 0;
                }

                // Got get the URF string given the index. We will be keeping track
                // of the bytes written and the offset into the buffer.
                found_URF_str = find_URF_str_given_index(i, 
                                                         &oid_value[tot_bytes_written], 
                                                         bytes_remaining, 
                                                         &bytes_written);

                if (found_URF_str)
                {
                   bytes_remaining   -= bytes_written;
                   tot_bytes_written += bytes_written;
                }
                
                // If we do not find the string, erase the delimiter we just added.
                else
                {
                   bytes_remaining   += 1;
                   tot_bytes_written -= 1;
                   
                   oid_value[tot_bytes_written] = 0;
                }

            }

          break;
       }

       default:
       {
    	    /* bad oid */
    	    e_res = OID_ERROR_OID_INVALID;
            ASSERT(0);
            break;
       }
    }

    if(tmp_value)
    {
        int str_len = strlen(tmp_value);
        if( oid_len < (str_len + 1))
        {
            e_res = OID_ERROR_BUFFER_TOO_SMALL;
        }
        else
        {
            strncpy(oid_value, tmp_value, oid_len); 
            oid_value[oid_len - 1] = '\0';
        }
    }

    return e_res;
}

/**
 * 
 * \brief The backend PWG Semantic Model API module function for retreiving
 * integer data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *  
 * \param[in] oid_index Index of the OID value to be accessed. Ignored for scalar OID 
 *       values.
 *
 * \param[out] oid_value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes: 
 * * OK                      - success
 * * OID_ERROR_INDEX_INVALID - index out of range
 * * FAIL                    - bad oid, other error
 *
 */
static error_type_t oid_module_airprint_get_int( oid_t oid,
                                                 uint32_t oid_index,
                                                 uint32_t *oid_value)
{
    error_type_t e_res;
    e_res = OK;

    switch ( oid )
    {
        
        case OID_AIRPRINT_MARKER_HIGH_LEVELS:
        {
#ifdef HAVE_VPI_SUPPORT // TODO expose this via consumables_common_api.h
            vpi_prt_rcode_t vpi_res;
            vpi_res =  vpi_prt_get_marker_level_high(oid_index, oid_value);
            if (vpi_res == VPI_PRT_BAD_INDEX)
            {
                e_res = OID_ERROR_INDEX_INVALID;
            }
            else if (vpi_res != VPI_PRT_OK) 
            {
                e_res = FAIL;
            }
#else // !HAVE_VPI_SUPPORT
            int num_consumables = consumable_get_num_consumables();
            if(oid_index >= num_consumables)
            {
                // index out of range
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            consumable_type_t type;
            type = consumable_get_type(oid_index);

            // TODO: add support for other types
            XASSERT(type == CONSUMABLE_TYPE_TONER || type == CONSUMABLE_TYPE_INK, type);
            *oid_value = 100;
#endif // HAVE_VPI_SUPPORT
            break;
        }

        case OID_AIRPRINT_MARKER_LEVELS:
        {
#ifdef HAVE_VPI_SUPPORT // TODO expose this via consumables_common_api.h
            // HACK: it is possible for IPP and the EWS to ask for marker levels without having
            //   previously asked for printer status.  This means the status job may not have 
            //   been invoked to get device status/marker levels.  Ask for print status to kick
            //   the status job (multiple quick calls to this will not always kick the status
            //   job).
            pec_get_current_printstatus();

            vpi_prt_rcode_t vpi_res;
            vpi_res =  vpi_prt_get_marker_level(oid_index, oid_value);
            if (vpi_res == VPI_PRT_BAD_INDEX)
            {
                e_res = OID_ERROR_INDEX_INVALID;
            }
            else if (vpi_res != VPI_PRT_OK) 
            {
                e_res = FAIL;
            }
#else // !HAVE_VPI_SUPPORT
            int num_consumables = consumable_get_num_consumables();
            if(oid_index >= num_consumables)
            {
                // index out of range
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            // get consumable status
            int remain;
            remain = consumable_get_percent_remaining(oid_index);
            if(remain == -1)
            {
                *oid_value = -2; // "unknown"
            }
            else
            {
                XASSERT((remain >= 0) && (remain <= 100), remain);
                *oid_value = remain;
            }
#endif // HAVE_VPI_SUPPORT
            break;
        }

        case OID_AIRPRINT_MARKER_LOW_LEVELS:
        {
#ifdef HAVE_VPI_SUPPORT // TODO expose this via consumables_common_api.h
            vpi_prt_rcode_t vpi_res;
            vpi_res =  vpi_prt_get_marker_level_low(oid_index, oid_value);
            if (vpi_res == VPI_PRT_BAD_INDEX)
            {
                e_res = OID_ERROR_INDEX_INVALID;
            }
            else if (vpi_res != VPI_PRT_OK) 
            {
                e_res = FAIL;
            }
#else // !HAVE_VPI_SUPPORT
            int num_consumables = consumable_get_num_consumables();
            if(oid_index >= num_consumables)
            {
                // index out of range
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            consumable_type_t type;
            type = consumable_get_type(oid_index);

            // TODO: add support for other types
            XASSERT(type == CONSUMABLE_TYPE_TONER || type == CONSUMABLE_TYPE_INK, type);
            *oid_value = 10;
#endif // HAVE_VPI_SUPPORT
            break;
        }

        default:
        {
            /* bad oid */
            e_res = OID_ERROR_OID_INVALID;
            ASSERT(0);
            break;
        }
    }

    return e_res;
}


/**
 *
 * \brief Registers each AirPrint OID with the oid controller
 *
 */
void oid_register_module_airprint( void )
{
    error_type_t e_res;

    e_res = oid_register_callbacks( OID_AIRPRINT_MARKER_COLORS,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_MARKER_HIGH_LEVELS,
                                    oid_module_airprint_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_MARKER_LEVELS,
                                    oid_module_airprint_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_MARKER_LOW_LEVELS,
                                    oid_module_airprint_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_MARKER_NAMES,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_MARKER_TYPES,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_PDF_VERSIONS_SUPPORTED,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_PRINT_SCALING_SUPPORTED,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_PRINT_SCALING_DEFAULT,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_PRINTER_KIND,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_AIRPRINT_URF_SUPPORTED,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );

    e_res = oid_register_callbacks( OID_AIRPRINT_PRINT_URF_CONFIGURED,
                                    oid_module_airprint_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

}



// A local function to the get the requested portion of the URF string.
// TODO 12/2013: instead of just returning the printer capabilities, we should also get the corresponding unirast
//  parser capabilities and then return the least common denominator of values (e.g. bitwise AND)
bool find_URF_str_given_index(uint32_t index, 
                              char     *buffer, 
                              uint32_t buffer_len,
                              uint32_t *bytes_written)
{
    bool str_found = true;

    switch (index)
    {
    case CP:
        handle_URF_copies(buffer, buffer_len, bytes_written);
        break;
    case IS:
        handle_input_slot(buffer, buffer_len, bytes_written);
        break;
    case OB:
        handle_output_bin(buffer, buffer_len, bytes_written);
        break;
    case PQ:
        handle_print_quality(buffer, buffer_len, bytes_written);
        break;
    case RS:
        handle_resolution(buffer, buffer_len, bytes_written);
        break;
    case W:
        handle_luminance_color_space(buffer, buffer_len, bytes_written);
        break;
    case V:
        handle_URF_version(buffer, buffer_len, bytes_written);
        break;

    case DM:
       {
           if(EngGetHaveAutoDuplexer())
           {
                handle_duplex(buffer, buffer_len, bytes_written);
           }
           else
           {
                str_found = false;
           }
          break;
       }
    case SRGB:
        {
            smjob_rcode_t      smjob_rcode;
            smjob_color_mode_t *color_mode;

            smjob_rcode = smjob_get_default_color_mode(SMJOB_TYPE_PRINT, 
                                                       &color_mode);
            if (smjob_rcode == SMJOB_OK)
            {
               // Only add this one if color is supported.
               if (!strcmp(color_mode, "color"))
               {
                  handle_sRGB_color_space(buffer,
                                          buffer_len,
                                          bytes_written);
               }
               else
               {
                  str_found = false;
               }
            }
            // This is bad, tell the world about it.
            else
            {
               ASSERT(0);
            }
        }
        break;
    default:
        str_found = false;
        break;
    }

    return(str_found);
}

void add_URF_str_delimiter(char     URF_delimit,
                           char     *buffer,
                           uint32_t buffer_len,
                           uint32_t *bytes_written)
{
    uint32_t len = sizeof(URF_delimit);

    ASSERT((buffer_len - len) > 0);

    strncat(buffer, &URF_delimit, len);

    *bytes_written += len;
}

void handle_URF_copies(char     *buffer,
                       uint32_t buffer_len,
                       uint32_t *bytes_written)
{
    char     *URF_prefix = NULL;
    char     *num_copies = "1";
    uint32_t tot_str_len;

    URF_prefix = get_URF_prefix_str(CP);
    ASSERT(URF_prefix);

    // We are going to set it to CP1 for now so only one digit.
    tot_str_len = strlen(URF_prefix) + strlen(num_copies);

    ASSERT((buffer_len - tot_str_len) > 0);

    sprintf(buffer, "%s%s", URF_prefix, num_copies);

    *bytes_written += tot_str_len;
}

void handle_input_slot(char     *buffer,
                       uint32_t buffer_len,
                       uint32_t *bytes_written)
{
    char     *URF_prefix = NULL;
    char     *input_slot = "1";
    uint32_t tot_str_len;

    URF_prefix = get_URF_prefix_str(IS);
    ASSERT(URF_prefix);

    // We only will have one slot for now so one digit.
    tot_str_len = strlen(URF_prefix) + strlen(input_slot);

    ASSERT((buffer_len - tot_str_len) > 0);

    sprintf(buffer, "%s%s", URF_prefix, input_slot);

    *bytes_written += tot_str_len;
}

void handle_output_bin(char     *buffer,
                       uint32_t buffer_len,
                       uint32_t *bytes_written)
{
    smjob_rcode_t smjob_rcode;
    char          *URF_prefix = NULL;
    char          *name       = NULL;

    URF_prefix = get_URF_prefix_str(OB);
    ASSERT(URF_prefix);

    smjob_rcode = smjob_get_output_bin_default(SMJOB_TYPE_PRINT, &name);

    if (smjob_rcode == SMJOB_OK)
    {
        uint32_t tot_str_len = strlen(URF_prefix);       

        if (!strcmp("face-up", name))
        {
            char *more_face_up_str = "OFU0";

            // The total string we will copy is the digit "9" for face-up found
            // in PWG 5100.2 plus the output media placed in bin face up, plus
            // the the one comma.
            tot_str_len += (1 + strlen(more_face_up_str) + 1);

            ASSERT((buffer_len - tot_str_len) > 0);

            sprintf(buffer, "%s%s,%s", URF_prefix, "9", more_face_up_str);

            *bytes_written += tot_str_len;
        }
        else if (!strcmp("face-down", name))
        {
            // The total string we will copy is the digit "10" for face-up found
            // in PWG 5100.2.
            tot_str_len += 2;

            ASSERT((buffer_len - tot_str_len) > 0);

            sprintf(buffer, "%s%s", URF_prefix, "10");

            *bytes_written += tot_str_len;
        }
        else
        {
            ASSERT(0);
        }
    }
}

void handle_print_quality(char     *buffer,
                          uint32_t buffer_len,
                          uint32_t *bytes_written)
{
    smjob_rcode_t smjob_rcode;
    uint32_t      doc_quality;
    uint32_t      index;
    uint32_t      tot_str_len;
    bool          first_quality_seen = false;
    char          *URF_prefix        = NULL;


    URF_prefix = get_URF_prefix_str(PQ);
    ASSERT(URF_prefix);

    tot_str_len = strlen(URF_prefix);

    index = 0;
    while (true)
    {
        uint32_t added_str_len = 0;
        char     *format_str = NULL;

        smjob_rcode = smjob_get_supported_quality(index, &doc_quality);
        if(smjob_rcode == SMJOB_OK)
        {
           // Check if this is the first time through.
           if (!first_quality_seen)
            {
                ASSERT((buffer_len - tot_str_len) > 0);

                sprintf(buffer, "%s", URF_prefix);

                *bytes_written += tot_str_len;

                // Only going to add a single digit.
                added_str_len = 1;

                format_str = "%01d";

                first_quality_seen = true;
            }
            else
            {
                // We are going to add a "-" plus a single digit.
                added_str_len = 2;

                format_str = "-%01d";
            }

            ASSERT((buffer_len - (tot_str_len + added_str_len)) > 0);

            sprintf(&buffer[tot_str_len], format_str, doc_quality);

            tot_str_len += added_str_len;

            *bytes_written += added_str_len;
        }
        else
        {
            break;
        }

        ++index;
    }
}

void handle_resolution(char     *buffer,
                       uint32_t buffer_len,
                       uint32_t *bytes_written)
{
   smjob_rcode_t      smjob_rcode;
   smjob_resolution_t resolution;
   uint32_t           index;
   uint32_t           tot_str_len;
   bool               first_val_added = false;
   char               *URF_prefix     = NULL;


   URF_prefix = get_URF_prefix_str(RS);
   ASSERT(URF_prefix);

   tot_str_len = strlen(URF_prefix);

   index = 0;
   while (true)
   {
      uint32_t added_str_len = 0;
      char     *format_str   = NULL;

      smjob_rcode = smjob_get_supported_resolution(SMJOB_TYPE_PRINT,
                                                   index,
                                                   &resolution);
      if (smjob_rcode == SMJOB_OK)
      {
         // Check to see if this is the first time through.
         if (!first_val_added)
         {
            ASSERT((buffer_len - tot_str_len) > 0);

            sprintf(buffer, "%s", URF_prefix);

            *bytes_written += tot_str_len;

            first_val_added = true;
         }
         else
         {
            added_str_len = 1;

            ASSERT((buffer_len - (tot_str_len + added_str_len)) > 0);

            strncat(&buffer[tot_str_len], "-", added_str_len);

            tot_str_len  += added_str_len;

            *bytes_written += added_str_len;
         }

         // Use the number of digits needed for the resolution. If the resolution
         // is above 4 digits, add a new if/else.
         if (resolution.xfeed_dir < 1000)
         {
            added_str_len = 3;
            format_str    = "%03d";
         }
         else
         {
            added_str_len = 4;
            format_str    = "%04d";
         }

         ASSERT((buffer_len - (tot_str_len + added_str_len)) > 0);

         sprintf(&buffer[tot_str_len], format_str, resolution.xfeed_dir);

         tot_str_len += added_str_len;

         *bytes_written += added_str_len;
      }
      else
      {
         break;
      }

      ++index;
   }
}

void handle_luminance_color_space(char     *buffer,
                                  uint32_t buffer_len,
                                  uint32_t *bytes_written)
{
    char     *URF_prefix = NULL;
    char     *lum_cs     = "8";
    uint32_t tot_str_len;

    URF_prefix = get_URF_prefix_str(W);
    ASSERT(URF_prefix);

    // Add one to the size for the ending comma.
    tot_str_len = strlen(URF_prefix) + strlen(lum_cs);

    ASSERT((buffer_len - tot_str_len) > 0);

    sprintf(buffer, "%s%s", URF_prefix, lum_cs);

    *bytes_written += tot_str_len;
}

void handle_URF_version(char     *buffer,
                        uint32_t buffer_len,
                        uint32_t *bytes_written)
{
    char     *URF_prefix = NULL;
    char     *version    = "1.3";
    uint32_t tot_str_len;

    URF_prefix = get_URF_prefix_str(V);
    ASSERT(URF_prefix);

    tot_str_len = strlen(URF_prefix) + strlen(version);

    ASSERT((buffer_len - tot_str_len) > 0);

    sprintf(buffer, "%s%s", URF_prefix, version);

    *bytes_written += tot_str_len;
}

void handle_duplex(char     *buffer,
                   uint32_t buffer_len,
                   uint32_t *bytes_written)
{
    char     *URF_prefix  = NULL;
    char     *duplex_type = "1";
    uint32_t tot_str_len;

    URF_prefix = get_URF_prefix_str(DM);
    ASSERT(URF_prefix);

    tot_str_len = strlen(URF_prefix) + strlen(duplex_type);

    ASSERT((buffer_len - tot_str_len) > 0);

    sprintf(buffer, "%s%s", URF_prefix, duplex_type);

    *bytes_written += tot_str_len;
}

void handle_sRGB_color_space(char     *buffer,
                             uint32_t buffer_len,
                             uint32_t *bytes_written)
{
    char     *URF_prefix     = NULL;
    char     *bits_per_pixel = "24";
    uint32_t tot_str_len;

    URF_prefix = get_URF_prefix_str(SRGB);
    ASSERT(URF_prefix);

    tot_str_len = strlen(URF_prefix) + strlen(bits_per_pixel);

    ASSERT((buffer_len - tot_str_len) > 0);

    sprintf(buffer, "%s%s", URF_prefix, bits_per_pixel);

    *bytes_written += tot_str_len;
}

char *get_URF_prefix_str(URF_key_prefixes_e URF_prefix)
{
    switch (URF_prefix)
    {
#define _CASE(x) case x: return #x
        _CASE(CP);
        _CASE(IS);
        _CASE(OB);
        _CASE(PQ);
        _CASE(RS);
        _CASE(W);
        _CASE(V);
        _CASE(FN);
        _CASE(DM);
        _CASE(SRGB);
#undef _CASE
    default:
        return(NULL);
    }
}

