/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_sm.c
 *
 * \brief This file implements the OID back-end functions for the
 * PWG Semantic Model API module OIDs
 *
 **/

//=========================================================================================
// Includes
//=========================================================================================
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "os_network.h" // HAVE_IPV6
#include "platform_api.h"
#include "oid_api.h"
#include "oid_producer_api.h"
#include "oid_sm_api.h"
#include "sm_job_api.h"
#include "net_iface_vars.h"

#if defined(HAVE_PRINT_SUPPORT) || defined(HAVE_VPI_SUPPORT)
#include "print_events_common_api.h"
#include "print_status_codes.h"
#ifdef HAVE_COMMON_CONSUMABLES_API
#include "consumables_common_api.h"
#endif // HAVE_COMMON_CONSUMABLES_API
#endif // HAVE_PRINT_SUPPORT || HAVE_VPI_SUPPORT
#ifdef HAVE_SCAN_SUPPORT
#include "scantypes.h"
#include "scands.h"
#endif // HAVE_SCAN_SUPPORT

#ifdef HAVE_VPI_SUPPORT
#include "device_identification_vars.h"
#endif // HAVE_VPI_SUPPORT




//=========================================================================================
// Enums
//=========================================================================================

/* 
 * These strings must have one-to-one correspondence with sm_printer_state_reasons_t
 * defines.
 * 
 * These strings must match the keywords defined by the 'printer-state-reasons' attribute:
 * http://tools.ietf.org/html/rfc2911#section-4.4.12
 */
char *g_sm_printer_state_reasons_map[] =
{
    "none",
    "door-open",
    "media-jam",
    "media-empty",
    "media-needed",
    "toner-empty",
    "toner-low",
    "other",
};
uint32_t g_sm_printer_state_reasons_map_size = sizeof(g_sm_printer_state_reasons_map)/sizeof(char *);

/* 
 * These strings must have one-to-one correspondence with sm_scanner_state_t defines.
 * 
 * These strings must match the keywords defined by the 'Scanner state' attribute:
 * Network Scan Service Semantic Model and Service Interface v1.0 [PWG 5108.02-2009]
 * section 7.1.6.10:
 * ftp://ftp.pwg.org/pub/pwg/candidates/cs-sm20-scan10-20090410-5108.02.pdf
 */
char *g_sm_scanner_state_map[] =
{
    "Idle",
    "Processing",
    "Stopped",
    "Testing",
    "Down",
    "Unknown",
};
uint32_t g_sm_scanner_state_map_size = sizeof(g_sm_scanner_state_map)/sizeof(char *);

/* 
 * These strings must have one-to-one correspondence with sm_scanner_state_reasons_t
 * defines.
 * 
 * These strings must match the keywords defined by the 'Scanner state reasons' attribute:
 * Network Scan Service Semantic Model and Service Interface v1.0 [PWG 5108.02-2009]
 * section 7.1.6.11:
 * ftp://ftp.pwg.org/pub/pwg/candidates/cs-sm20-scan10-20090410-5108.02.pdf
 */
char *g_sm_scanner_state_reasons_map[] =
{
    "AttentionRequired",
    "Calibrating",
};
uint32_t g_sm_scanner_state_reasons_map_size = sizeof(g_sm_scanner_state_reasons_map)/sizeof(char *);

static char *media_col_supported[] =
{
    "media-type", "media-size", "media-top-margin", "media-left-margin", 
    "media-right-margin", "media-bottom-margin", "media-source", "duplex-supported"
};

static uint32_t media_col_supported_size = sizeof(media_col_supported) / sizeof(char *);

//=========================================================================================
// Local function declarations
//=========================================================================================

/* TODO 1-NOV-2012 status word via pec_get_current_printstatus() only gives "worser"
 * status, so even though IPP printer-state-reason is multi-value, we can currently only
 * ever have a single bit asserted.  This could be problem for APVT conformance.  If so
 * may need to go down one layer to engine interface.
 */
static sm_printer_state_reasons_t sm_get_printer_state_reasons(void)
{
#if defined(HAVE_PRINT_SUPPORT) || defined(HAVE_VPI_SUPPORT)

    sm_printer_state_reasons_t reasons = 0;

#ifdef HAVE_COMMON_CONSUMABLES_API
    // get consumable status
    consumable_status_t consumable_status;
    int num_consumables = consumable_get_num_consumables();
    int i;
    for(i=0; i < num_consumables; i++)
    {
        consumable_status = consumable_get_status(i);
        if(consumable_status == CONSUMABLE_STATUS_OUT)
        {
            reasons |= SM_PRINTER_STATE_REASON_MARKER_SUPPLY_EMPTY;
        }
        if(consumable_status == CONSUMABLE_STATUS_LOW)
        {
            reasons |= SM_PRINTER_STATE_REASON_MARKER_SUPPLY_LOW;
        }
    }
#endif // HAVE_COMMON_CONSUMABLES_API

    // get other status
    uint32_t status = pec_get_current_printstatus();

    if(ISUSERINPUT(status)) // means user attention required -- see ctrl panel for more info
    {
        // not sure this provides any real info to the user
        // reasons |= SM_PRINTER_STATE_REASON_OTHER;
    }
    if(STATCATCMP(status, STATCAT_DOOROPEN))
    {
        reasons |= SM_PRINTER_STATE_REASON_DOOR_OPEN;
    }
    if(STATCATCMP(status, STATCAT_PAPERJAM_TYPE) ||
       STATCATCMP(status, STATCAT_MISPRINT) ||     
       STATCATCMP(status, STATCAT_PAPERJAM_AREA))  
    {
        reasons |= SM_PRINTER_STATE_REASON_MEDIA_JAM;
    }
    if(STATCATCMP(status, STATCAT_INPUT) && STATCODEGRPCMP(status, STATUS_INPUT_LOAD_TRAY_1_NO_MEDIA_AVAILABLE))
    {
        /* * STATCAT_INPUT ==> status category (byte 1)
         * * STATUS_INPUT_LOAD_TRAY_1_NO_MEDIA_AVAILABLE ==> status code (byte 0) -- note status
         *   'codes' are meaningful only in context of status category
         * * STATCODEGRPCMP() ==> strips off code group so that the check applies to ALL TRAYS and
         *   not just TRAY_1
         */
        reasons |= SM_PRINTER_STATE_REASON_MEDIA_EMPTY;
        
        // right now assuming there always a print job active,
        // if not probably should only set media-empty
        reasons |= SM_PRINTER_STATE_REASON_MEDIA_NEEDED; 
    }
    if((STATCATCMP(status, STATCAT_TONERWRONG) || STATCATCMP(status, STATCAT_CARTRIDGE)) && (!STATCATCMP(status, STATUS_TONER_NO_ISSUE)))
    {
        reasons |= SM_PRINTER_STATE_REASON_MARKER_SUPPLY_EMPTY;
    }
    if(STATCATCMP(status, STATCAT_TONERLOW)  && (!STATCATCMP(status, STATUS_TONER_NO_ISSUE)))
    {
        reasons |= SM_PRINTER_STATE_REASON_MARKER_SUPPLY_LOW;
    }
    if(!reasons)
    {
        reasons |= SM_PRINTER_STATE_REASON_NONE;
    }
    ASSERT(reasons); // at least reason 'none'
    return reasons;
#else
    return 0;
#endif

}

static sm_scanner_state_t sm_get_scanner_state(void)
{
    sm_scanner_state_t sm_scanner_state = SM_SCANNER_STATE_UNKNOWN;

#ifdef HAVE_SCAN_SUPPORT
    scan_err_t scerr;
    uint32_t scands_state;
    scerr = scands_get_integer_with_default( "scanner_state", &scands_state, 0 );
    if(scerr != SCANERR_NONE)
    {
        goto done;
    }

    switch(scands_state)
    {
        case SCANNER_IS_IDLE:
            sm_scanner_state = SM_SCANNER_STATE_IDLE;
            break;
        case SCANNER_IS_OFFLINE:
            sm_scanner_state = SM_SCANNER_STATE_STOPPED;
            break;
        case SCANNER_IS_RUNNING:
            sm_scanner_state = SM_SCANNER_STATE_PROCESSING;
            break;
        case SCANNER_IS_CALIBRATING:
            sm_scanner_state = SM_SCANNER_STATE_TESTING;
            break;
        case SCANNER_IS_NOTHING: // fall through
        default:
            sm_scanner_state = SM_SCANNER_STATE_UNKNOWN;
            break;
    }

done:
#endif // HAVE_SCAN_SUPPORT

    return sm_scanner_state;
}

static sm_scanner_state_reasons_t sm_get_scanner_state_reasons(void)
{
    return 0;
}

/**
 * 
 * \brief The backend PWG Semantic Model API module function for retreiving
 * byte array data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *  
 * \param[in] index Index of the OID value to be accessed. Ignored for scalar OID values. 
 *  
 * \param[in] oid_data Pointer to storage allocated by caller populated with binary data 
 *       representing the OID value.
 *
 * \param[in/out] oid_len Size of the buffer pointed to by 'oid_value'.  The maximum 
 *       number of bytes that can be stored in the buffer. On successful return this
 *       parameter is set to the size of the data returned.  On OID_ERROR_BUFFER_TOO_SMALL
 *       this value is set to the size of the buffer needed to contain the data.
 *
 * \return One of the error_type_t return codes:
 * * OK                      - success
 * * OID_ERROR_INDEX_INVALID - index out of range
 * * OID_ERROR_BUFFER_TOO_SMALL - requested data cannot fit into a buffer of size 
 *                             'oid_len'.
 * * FAIL                    - bad oid, other error
 *
 */
static error_type_t oid_module_sm_get_byte_array( oid_t oid,
                                                  uint32_t oid_index,
                                                  uint8_t *oid_value,
                                                  uint32_t *oid_len )
{
    error_type_t e_res = OK;

    switch ( oid )
    {
        case OID_SM_PRINT_RESOLUTION_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            smjob_rcode_t smjob_rcode;
            smjob_resolution_t resolution;
            smjob_rcode = smjob_get_default_resolution(SMJOB_TYPE_PRINT, &resolution);
            if (smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            uint8_t res_units = (uint8_t)resolution.units;
            uint32_t len;
            len = sizeof(resolution.xfeed_dir) + 
                  sizeof(resolution.feed_dir) + 
//                  sizeof(resolution.units);
                  sizeof(res_units);
            if( len > *oid_len)
            {
                *oid_len = len;
                e_res = OID_ERROR_BUFFER_TOO_SMALL;
                break;
            }
            *oid_len = len;

            uint32_t pos = 0;
            uint32_t long_val;
            long_val = htonl(resolution.xfeed_dir);
            memcpy(oid_value+pos, &long_val, sizeof(long_val));
            pos += sizeof(long_val);
            long_val = htonl(resolution.feed_dir);
            memcpy(oid_value+pos, &long_val, sizeof(long_val));
            pos += sizeof(long_val);
// BUGFIX: we must copy only 1 byte, but can't count on enum size to be consistent across platforms
// (e.g. smjob_resolution_units_t enum size is 1 byte in threadx but 4 bytes in linux)
//            memcpy(oid_value+pos, &resolution.units, sizeof(resolution.units));
//            pos += sizeof(resolution.units);
            memcpy(oid_value+pos, &res_units, sizeof(res_units));
            pos += sizeof(res_units);
            XASSERT(pos == len, pos);
            break;
        }

        case OID_SM_PRINT_RESOLUTION_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_resolution_t resolution;
            smjob_rcode = smjob_get_supported_resolution(SMJOB_TYPE_PRINT, 
                                                         oid_index, 
                                                         &resolution);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            uint8_t res_units = (uint8_t)resolution.units;
            uint32_t len;
            len = sizeof(resolution.xfeed_dir) + 
                  sizeof(resolution.feed_dir) + 
//                  sizeof(resolution.units);
                  sizeof(res_units);
            if( len > *oid_len)
            {
                *oid_len = len;
                e_res = OID_ERROR_BUFFER_TOO_SMALL;
                break;
            }
            *oid_len = len;

            uint32_t pos = 0;
            uint32_t long_val;
            long_val = htonl(resolution.xfeed_dir);
            memcpy(oid_value+pos, &long_val, sizeof(long_val));
            pos += sizeof(long_val);
            long_val = htonl(resolution.feed_dir);
            memcpy(oid_value+pos, &long_val, sizeof(long_val));
            pos += sizeof(long_val);
// BUGFIX: we must copy only 1 byte, but can't count on enum size to be consistent across platforms
// (e.g. smjob_resolution_units_t enum size is 1 byte in threadx but 4 bytes in linux)
//            memcpy(oid_value+pos, &resolution.units, sizeof(resolution.units));
//            pos += sizeof(resolution.units);
            memcpy(oid_value+pos, &res_units, sizeof(res_units));
            pos += sizeof(res_units);
            XASSERT(pos == len, pos);
            break;
        }

        case OID_SM_COPIES_SUPPORTED:
        {
           uint32_t      min_value = 0;
           uint32_t      max_value = 0;
           smjob_rcode_t smjob_rcode;

           if(oid_index > 0)
           {
               e_res = OID_ERROR_INDEX_INVALID;
               break;
           }

           smjob_rcode = smjob_get_supported_copies(&min_value, &max_value);
           if (smjob_rcode != SMJOB_OK)
           {
              e_res = OID_ERROR_INDEX_INVALID;
              break;
           }

           uint32_t len;

           len = sizeof(min_value) + sizeof(max_value);

           if (len > *oid_len)
           {
              e_res = OID_ERROR_BUFFER_TOO_SMALL;
              break;
           }

           uint32_t pos = 0;
           uint32_t long_val;
           uint32_t long_val_len = sizeof(long_val);
           
           long_val = htonl(min_value);
           memcpy(oid_value + pos, &long_val, long_val_len);
           
           pos += long_val_len;

           // We need to set the OID length to that which we return.
           *oid_len = long_val_len;

           long_val = htonl(max_value);
           memcpy(oid_value + pos, &long_val, long_val_len);

           pos += long_val_len;

           // Add in the last chunk.
           *oid_len += long_val_len;

           XASSERT(pos == len, pos);

           break;
        }

        default:
        {
            /* bad oid */
            e_res = FAIL;
            ASSERT(0);
            break;
        }
    }

    return e_res;
}

/**
 * 
 * \brief The backend PWG Semantic Model API module function for retreiving
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
 *                             entire string.
 * * FAIL                    - bad oid, other error
 *
 */
static error_type_t oid_module_sm_get_string( oid_t oid,
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
        case OID_SM_CHARSET_CONFIGURED:
        {
            // TODO TEMP hardcoded
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            tmp_value = "utf-8";
            break;
        }

        case OID_SM_CHARSET_SUPPORTED:
        {
            // TODO TEMP hardcoded
            switch(oid_index) 
            {
            case 0:
                tmp_value = "utf-8";
                break;
            case 1:
                tmp_value = "us-ascii";
                break;
            default:
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            break;
        }

        case OID_SM_NATURAL_LANGUAGE_CONFIGURED:
        {
            // TODO TEMP hardcoded
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            tmp_value = "en";

            break;
        }

        case OID_SM_GENERATED_NATURAL_LANGUAGE_SUPPORTED:
        {
            // TODO TEMP hardcoded
            switch(oid_index) 
            {
            case 0:
                tmp_value = "en";
                break;
            default:
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            break;
        }

#if defined(HAVE_PRINT_SUPPORT) || defined(HAVE_VPI_SUPPORT)
        case OID_SM_PRINT_DOC_FORMAT_DEFAULT:
        {
           if(oid_index > 0)
           {
               e_res = OID_ERROR_INDEX_INVALID;
               break;
           }

           // Use the first entry as the default format.
           smjob_rcode_t  smjob_rcode;
           smjob_rcode = smjob_get_default_format(SMJOB_TYPE_PRINT, &tmp_value);
           if (smjob_rcode != SMJOB_OK)
           {
               tmp_value = NULL;
               e_res = FAIL;
           }

           break;
        }

        case OID_SM_PRINT_DOC_FORMAT_SUPPORTED:
        {
            smjob_rcode_t  smjob_rcode;
            smjob_rcode = smjob_get_supported_format(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }

        case OID_SM_PRINT_SIDES_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_default_sides(SMJOB_TYPE_PRINT, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
            }

            break;
        }

        case OID_SM_PRINT_SIDES_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_sides(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }

        case OID_SM_PRINT_COLOR_MODE_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_default_color_mode(SMJOB_TYPE_PRINT, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
            }

            break;
        }

        case OID_SM_PRINT_COLOR_MODE_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_color_mode(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }

        case OID_SM_PRINT_MEDIA_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_default_media_size_name(SMJOB_TYPE_PRINT, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
            }

            break;
        }

        case OID_SM_PRINT_MEDIA_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_media_size_name(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }

        case OID_SM_PRINT_MEDIA_TYPE_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_default_media_type_name(SMJOB_TYPE_PRINT, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
            }

            break;
        }

        case OID_SM_PRINT_MEDIA_TYPE_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_media_type_name(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }

        case OID_SM_PRINT_MEDIA_TABLE_TYPE:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_media_table_type(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }
#endif

        case OID_SM_SYSTEM_NAME:
        {
            if( oid_len < NET_IFACE_HOSTNAME_SIZE )
            {
                e_res = OID_ERROR_BUFFER_TOO_SMALL;
                break;
            }

            // TODO must account for multiple simultaneous interfaces
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

            net_iface_rcode_t net_iface_rcode;
            net_iface_rcode = net_iface_get_var_hostname( if_index, oid_value, oid_len );
            if(net_iface_rcode != NET_IFACE_OK)
            {
                oid_value[0] = '\0';
                e_res = FAIL;
            }

            break;
        }

        case OID_SM_SYSTEM_LOCATION:
        {
            if( oid_len < NET_LOCATION_SIZE )
            {
                e_res = OID_ERROR_BUFFER_TOO_SMALL;
                break;
            }
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
#ifdef HAVE_VPI_SUPPORT
            e_res = device_id_get_location(oid_value, oid_len);
#else // !HAVE_VPI_SUPPORT
            net_iface_rcode_t net_iface_rcode;
            net_iface_rcode = net_get_var_location(oid_value,  NET_LOCATION_SIZE );
            if(net_iface_rcode != NET_IFACE_OK)
            {
                oid_value[0] = '\0';
                e_res = FAIL;
            }
#endif // HAVE_VPI_SUPPORT

            break;
        }

        case OID_SM_SYSTEM_MORE_INFO:
        {
            if( oid_len < NET_IFACE_DEVICE_URL_SIZE )
            {
                e_res = OID_ERROR_BUFFER_TOO_SMALL;
                break;
            }

            net_iface_rcode_t net_iface_rcode;
            net_iface_rcode = net_iface_get_var_device_url(oid_index, oid_value, NET_IFACE_DEVICE_URL_SIZE);
            if(net_iface_rcode != NET_IFACE_OK)
            {
                oid_value[0] = '\0';
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            break;
        }

        case OID_SM_SYSTEM_MORE_INFO_IPV6:
        {
            if( oid_len < NET_IFACE_DEVICE_URL_SIZE )
            {
                e_res = OID_ERROR_BUFFER_TOO_SMALL;
                break;
            }

            net_iface_rcode_t net_iface_rcode;
#ifdef HAVE_IPV6
            net_iface_rcode = net_iface_get_var_device_url_ipv6(oid_index, oid_value, NET_IFACE_DEVICE_URL_SIZE);
#else // !HAVE_IPV6
            net_iface_rcode = net_iface_get_var_device_url(oid_index, oid_value, NET_IFACE_DEVICE_URL_SIZE);
#endif // HAVE_IPV6
            if(net_iface_rcode != NET_IFACE_OK)
            {
                oid_value[0] = '\0';
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            break;
        }

        case OID_SM_SYSTEM_MAKE_AND_MODEL:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            platvars_get_prtnamestr(&tmp_value);

            break;
        }

        case OID_SM_PRINTER_STATE_REASONS:
        {
            sm_printer_state_reasons_t reasons;
            reasons = sm_get_printer_state_reasons();
            sm_printer_state_reasons_t bit = 1; 
            uint32_t cur_oid_index = 0;
            uint32_t str_index = 0;
            ASSERT(bit == 0x00000001); // must start at hex 0x00000001
            while(1)
            {
                // search for next asserted 'reason' bit
                while(!(reasons & bit))
                {
                    // current 'reason' bit is not asserted -- advance to next bit
                    bit <<= 1;
                    str_index++;
                    if( !bit || (str_index >= g_sm_printer_state_reasons_map_size) )
                    {
                        // out of range
                        break;
                    }
                }
                if( !bit || (str_index >= g_sm_printer_state_reasons_map_size) )
                {
                    // requested oid_index out of range
                    ASSERT(oid_index > 0); // reasons should never be empty -- at least 'none'
                    e_res = OID_ERROR_INDEX_INVALID;
                    break;
                }
                // found an asserted 'reason' bit

                if(cur_oid_index == oid_index)
                {
                    // found 'reason' at requested oid_index
                    XASSERT(str_index < g_sm_printer_state_reasons_map_size, str_index);
                    tmp_value = g_sm_printer_state_reasons_map[str_index];
                    break; 
                }

                // advance to next OID index
                cur_oid_index++;
                bit <<= 1;
                str_index++;
                ASSERT(cur_oid_index<1000); // sanity check for infinite loop
            }

            break;
        }

        case OID_SM_SCANNER_STATE:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            sm_scanner_state_t scanner_state;
            scanner_state = sm_get_scanner_state();

            XASSERT(scanner_state < g_sm_scanner_state_map_size, scanner_state);
            tmp_value = g_sm_scanner_state_map[scanner_state];

            break;
        }

        case OID_SM_SCANNER_STATE_REASONS:
        {
            sm_scanner_state_reasons_t reasons;
            reasons = sm_get_scanner_state_reasons();
            sm_scanner_state_reasons_t bit = 1; 
            uint32_t cur_oid_index = 0;
            uint32_t str_index = 0;
            ASSERT(bit == 0x00000001); // must start at hex 0x00000001
            while(1)
            {
                // search for next asserted 'reason' bit
                while(!(reasons & bit))
                {
                    // current 'reason' bit is not asserted -- advance to next bit
                    bit <<= 1;
                    str_index++;
                    if( !bit || (str_index >= g_sm_scanner_state_reasons_map_size) )
                    {
                        // out of range
                        break;
                    }
                }
                if( !bit || (str_index >= g_sm_scanner_state_reasons_map_size) )
                {
                    // requested oid_index out of range
                    e_res = OID_ERROR_INDEX_INVALID;
                    break;
                }
                // found an asserted 'reason' bit

                if(cur_oid_index == oid_index)
                {
                    // found 'reason' at requested oid_index
                    XASSERT(str_index < g_sm_scanner_state_reasons_map_size, str_index);
                    tmp_value = g_sm_scanner_state_reasons_map[str_index];
                    break; 
                }

                // advance to next OID index
                cur_oid_index++;
                bit <<= 1;
                str_index++;
                ASSERT(cur_oid_index<1000); // sanity check for infinite loop
            }

            break;
        }

        case OID_SM_COMPRESSION_SUPPORTED:
    	{
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

    	    smjob_rcode_t smjob_rcode;
    	    smjob_rcode = smjob_get_supported_compression(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if (smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

    	    break;
    	}

        case OID_SM_PRINTER_ICON:
    	{
    	    smjob_rcode_t  smjob_rcode;
            smjob_rcode = smjob_get_icons(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if (smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

    	    break;
    	}

        case OID_SM_JOB_CREATION_ATTR_SUPPORTED:
        {
            switch(oid_index)
            {
            case 0:
                tmp_value = "copies";
                break;
            case 1:
                tmp_value = "sides";
                break;
            case 2:
                tmp_value = "orientation-requested";
                break;
            case 3:
                tmp_value = "media";
                break;
            case 4:
                tmp_value = "print-quality";
                break;
            case 5:
                tmp_value = "printer-resolution";
                break;
            case 6:
                tmp_value = "output-bin";
                break;
            case 7:
                tmp_value = "media-col";
                break;
            case 8:
                tmp_value = "output-mode";
                break;
            case 9:
                tmp_value = "job-name";
                break;
            case 10:
                tmp_value = "print-color-mode";
                break;
            case 11:
                tmp_value = "finishings";
                break;
            case 12:
                tmp_value = "ipp-attribute-fidelity";
                break;
            default:
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            break;   
        }   
            
        case OID_SM_MEDIA_COL_DEFAULT:
        {    
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            tmp_value = media_col_supported[0];

            break;   
        }   
            
            
        case OID_SM_MEDIA_COL_SUPPORTED:
        {   
            ASSERT(media_col_supported_size >= 1);
            if(oid_index > media_col_supported_size-1)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }
            tmp_value = media_col_supported[oid_index];

            break;   
        }
                
        case OID_SM_OUTPUT_BIN_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_output_bin_default(SMJOB_TYPE_PRINT, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
            }

            break;
        }

        case OID_SM_OUTPUT_BIN_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_output_bin_supported(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if (smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;   
        }

        case OID_SM_MEDIA_SOURCE_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_media_source(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }

        case OID_SM_PRINT_MEDIA_TABLE_SOURCE:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_media_table_source(SMJOB_TYPE_PRINT, oid_index, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }

            break;
        }

        case OID_SM_IPP_VERSIONS_SUPPORTED:
        {
            switch(oid_index)
            {
            case 0:
                tmp_value = "1.1";
                break;
            case 1:
                tmp_value = "2.0";
                break;
            default:
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            break;
        }

        case OID_SM_PRINTER_GEO_LOCATION: // PWG 5100.13
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_printer_geo_location(SMJOB_TYPE_PRINT, &tmp_value);
            if(smjob_rcode != SMJOB_OK)
            {
                tmp_value = NULL;
                e_res = FAIL;
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
 * \brief The backend PWG Semantic Model API module function for set functionality.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[out] data A pointer to the data buffer that contains the
 * string data.  This buffer must be at least "string_len_in_bytes" long.
 *
 * \param[in] string_len_in_bytes The length in bytes that the 
 * data buffer, "data", is, NOT the null terminating character.
 * The buffer must be at least string_len_in_bytes + 1 bytes long.
 *
 * \return error_type_t
 * OK                        - successful
 *
 * OID_ERROR_ILLEGAL_CHARS   - the string data had characters that the 
 *                             consumer could not use or interpret
 *
 * OID_ERROR_VALUE_TOO_BIG   - the sring_len_in_bytes was too large
 *                             or the data value represented by the string
 *                             was too large for the intended use
 *
 * OID_ERROR_VALUE_TOO_SMALL - the string_len_in_bytes was too small
 *                             or the data value represented by the
 *                             string was too small for the intended use
 *
 * FAIL                      - bad oid, other error
 *
 */
error_type_t oid_module_sm_set_string( oid_t oid,
                                       uint32_t index,
                                       const char * data,
                                       uint32_t string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_SM_PRINT_MEDIA_DEFAULT:
            {
                smjob_rcode_t smjob_rcode;
                smjob_rcode = smjob_set_supported_default_media_size_name(SMJOB_TYPE_PRINT, (char*)data, string_len_in_bytes);
                if(smjob_rcode != SMJOB_OK)
                {
                    e_res = FAIL;
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
static error_type_t oid_module_sm_get_int( oid_t oid,
                                           uint32_t oid_index,
                                           uint32_t *oid_value)
{
    error_type_t e_res;
    e_res = OK;

    switch ( oid )
    {
        case OID_SM_COPIES_DEFAULT:
        {
            smjob_rcode_t smjob_rcode;
            uint32_t      copies;

            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            smjob_rcode = smjob_get_default_copies(&copies);
            if (smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                copies = 1;
            }
            *oid_value = copies;
            break;
        }

        case OID_SM_PRINT_FINISHINGS_DEFAULT:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            // TODO TEMP hardcoded - use OEM config
            *oid_value = SMJOB_FINISHING_NONE;
            break;
        }

        case OID_SM_PRINT_FINISHINGS_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_finishing_t doc_finishing;
            smjob_rcode = smjob_get_supported_finishings(SMJOB_TYPE_PRINT, 
							 oid_index, 
							 &doc_finishing);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                doc_finishing = SMJOB_FINISHING_NONE;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }
            *oid_value = (uint32_t)doc_finishing;
            break;
        }

        case OID_SM_PRINT_ORIENTATION_REQUESTED_DEFAULT:
        {
            smjob_rcode_t       smjob_rcode;
            smjob_orientation_t doc_orientation;

            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            smjob_rcode = smjob_get_default_orientation(SMJOB_TYPE_PRINT, &doc_orientation);
            if (smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                doc_orientation = SMJOB_ORIENTATION_PORTRAIT;
            }
            *oid_value = (uint32_t)doc_orientation;
            break;
        }

        case OID_SM_PRINT_ORIENTATION_REQUESTED_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_orientation_t doc_orientation;
            smjob_rcode = smjob_get_supported_orientation(SMJOB_TYPE_PRINT, oid_index, &doc_orientation);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                doc_orientation = SMJOB_ORIENTATION_PORTRAIT;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }
            *oid_value = (uint32_t)doc_orientation;
            break;
        }
  
        case OID_SM_PRINT_QUALITY_DEFAULT:
        {
            smjob_rcode_t   smjob_rcode;
            smjob_quality_t doc_quality;

            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            smjob_rcode = smjob_get_default_quality(&doc_quality);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                doc_quality = SMJOB_QUALITY_NORMAL;
            }
            *oid_value = (uint32_t)doc_quality;
            break;
        }

        case OID_SM_PRINT_QUALITY_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_quality_t doc_quality;
            smjob_rcode = smjob_get_supported_quality(oid_index, &doc_quality);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                doc_quality = SMJOB_QUALITY_NORMAL;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }
            *oid_value = (uint32_t)doc_quality;
            break;
        }
  
         case OID_SM_PRINT_MEDIA_X_DIM_DEFAULT:
         {
            smjob_rcode_t smjob_rcode;

            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            // Index 0 is assumed to be the default
            smjob_rcode = smjob_get_supported_media_size_x_dim(SMJOB_TYPE_PRINT, 0, oid_value);
            if (smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                *oid_value = 0;
            }
            break;
        }

        case OID_SM_PRINT_MEDIA_X_DIM_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_media_size_x_dim(SMJOB_TYPE_PRINT, oid_index, oid_value);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                *oid_value = 0;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }
            break;
        }

        case OID_SM_PRINT_MEDIA_Y_DIM_DEFAULT:
        {
            smjob_rcode_t smjob_rcode;

            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            // Index 0 is assumed to be the default
            smjob_rcode = smjob_get_supported_media_size_y_dim(SMJOB_TYPE_PRINT, 0, oid_value);
            if (smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                *oid_value = 0;
            }
            break;
        }

        case OID_SM_PRINT_MEDIA_Y_DIM_SUPPORTED:
        {
            smjob_rcode_t smjob_rcode;
            smjob_rcode = smjob_get_supported_media_size_y_dim(SMJOB_TYPE_PRINT, oid_index, oid_value);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                *oid_value = 0;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }
            break;
        }

        case OID_SM_PRINT_ACCEPTING_JOBS:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            // TODO TEMP hardcoded - add dynamic detection e.g. based on job queue size
            *oid_value = 1; // true
            break;
        }
#if defined(HAVE_PRINT_SUPPORT) || defined(HAVE_VPI_SUPPORT)
       case OID_SM_PRINT_MEDIA_TABLE_DUPLEX:
       {
            smjob_rcode_t smjob_rcode;
            uint32_t      duplex;

            smjob_rcode = smjob_get_media_table_duplex(SMJOB_TYPE_PRINT, oid_index, &duplex);
            if(smjob_rcode != SMJOB_OK)
            {
                e_res = FAIL;
                duplex = 0;
                if(smjob_rcode == SM_BAD_INDEX)
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
            }
            *oid_value = duplex;
            break;
        }

        case OID_SM_PRINTER_STATE:
        {
            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            uint32_t status = pec_get_current_printstatus();
            if(ISOFFLINE(status))
            {
                *oid_value = SM_PRINTER_STATE_STOPPED;
            }
            else if( (status & (STATUS_SEVERITY_MASK | 
                                STATUS_CATEGORY_MASK)) == STATUS_INFO )
            {
                *oid_value = SM_PRINTER_STATE_PROCESSING;
            }
            else
            {
                *oid_value = SM_PRINTER_STATE_IDLE;
            }
            break;
        }


        case OID_SM_MEDIA_LEFT_MARGIN_SUPPORTED:
        {
           smjob_rcode_t  smjob_rcode;
           uint32_t       margin;

           smjob_rcode = smjob_get_supported_media_left_margin(SMJOB_TYPE_PRINT, oid_index, &margin);
           if (smjob_rcode != SMJOB_OK)
           {
               e_res = FAIL;
               *oid_value = 0;
               if (smjob_rcode == SM_BAD_INDEX)
               {
                   e_res = OID_ERROR_INDEX_INVALID;
               }
           }
           else
           {
               *oid_value = margin;
           }
           break;
        }

        case OID_SM_MEDIA_RIGHT_MARGIN_SUPPORTED:
        {
           smjob_rcode_t  smjob_rcode;
           uint32_t       margin;

           smjob_rcode = smjob_get_supported_media_right_margin(SMJOB_TYPE_PRINT, oid_index, &margin);
           if (smjob_rcode != SMJOB_OK)
           {
               e_res = FAIL;
               *oid_value = 0;
               if (smjob_rcode == SM_BAD_INDEX)
               {
                   e_res = OID_ERROR_INDEX_INVALID;
               }
           }
           else
           {
               *oid_value = margin;
           }
           break;
        }

        case OID_SM_MEDIA_TOP_MARGIN_SUPPORTED:
        {
           smjob_rcode_t  smjob_rcode;
           uint32_t       margin;

           smjob_rcode = smjob_get_supported_media_top_margin(SMJOB_TYPE_PRINT, oid_index, &margin);
           if (smjob_rcode != SMJOB_OK)
           {
               e_res = FAIL;
               *oid_value = 0;
               if (smjob_rcode == SM_BAD_INDEX)
               {
                   e_res = OID_ERROR_INDEX_INVALID;
               }
           }
           else
           {
               *oid_value = margin;
           }
           break;
        }

        case OID_SM_MEDIA_BOTTOM_MARGIN_SUPPORTED:
        {
           smjob_rcode_t  smjob_rcode;
           uint32_t       margin;

           smjob_rcode = smjob_get_supported_media_bottom_margin(SMJOB_TYPE_PRINT, 
                                                                 oid_index, 
                                                                 &margin);
           if (smjob_rcode != SMJOB_OK)
           {
               e_res = FAIL;
               *oid_value = 0;
               if (smjob_rcode == SM_BAD_INDEX)
               {
                   e_res = OID_ERROR_INDEX_INVALID;
               }
           }
           else
           {
               *oid_value = margin;
           }
           break;
        }

        case OID_SM_AIRPRINT_ENABLED:
        {
            smjob_rcode_t smjob_rcode;

            if(oid_index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            smjob_rcode = smjob_get_aiprint_enabled(SMJOB_TYPE_PRINT, oid_index, oid_value);
            if(smjob_rcode != SMJOB_OK)
            {
                *oid_value = 0;
                e_res = FAIL;
            }

            break;
        }

#endif
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
 * \brief Registers each PWG Semantic Model OID with the oid controller
 *
 */
void oid_register_module_sm( void )
{
    error_type_t e_res;

    e_res = oid_register_callbacks( OID_SM_CHARSET_CONFIGURED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_CHARSET_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_NATURAL_LANGUAGE_CONFIGURED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_GENERATED_NATURAL_LANGUAGE_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_COPIES_DEFAULT,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_COPIES_SUPPORTED,
                                    oid_module_sm_get_byte_array,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_DOC_FORMAT_DEFAULT,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_DOC_FORMAT_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_SIDES_DEFAULT,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_SIDES_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_COLOR_MODE_DEFAULT,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_COLOR_MODE_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_DEFAULT,
                                    oid_module_sm_get_string,
                                    oid_module_sm_set_string,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_TYPE_DEFAULT,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_TYPE_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_TABLE_TYPE,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_TABLE_DUPLEX,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_X_DIM_DEFAULT,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_X_DIM_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_Y_DIM_DEFAULT,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_Y_DIM_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_RESOLUTION_DEFAULT,
                                    oid_module_sm_get_byte_array,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_RESOLUTION_SUPPORTED,
                                    oid_module_sm_get_byte_array,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_FINISHINGS_DEFAULT,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_FINISHINGS_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_ORIENTATION_REQUESTED_DEFAULT,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_ORIENTATION_REQUESTED_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_QUALITY_DEFAULT,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_QUALITY_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_ACCEPTING_JOBS,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_SYSTEM_NAME,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_SYSTEM_LOCATION,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_SYSTEM_MORE_INFO,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_SYSTEM_MORE_INFO_IPV6,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_SYSTEM_MAKE_AND_MODEL,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINTER_STATE,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINTER_STATE_REASONS,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_SCANNER_STATE,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_SCANNER_STATE_REASONS,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_COMPRESSION_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINTER_ICON,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_JOB_CREATION_ATTR_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );


    e_res = oid_register_callbacks( OID_SM_MEDIA_COL_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_MEDIA_COL_DEFAULT,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );


    e_res = oid_register_callbacks( OID_SM_MEDIA_LEFT_MARGIN_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_MEDIA_RIGHT_MARGIN_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );


    e_res = oid_register_callbacks( OID_SM_MEDIA_TOP_MARGIN_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_MEDIA_BOTTOM_MARGIN_SUPPORTED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_OUTPUT_BIN_DEFAULT,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_OUTPUT_BIN_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_MEDIA_SOURCE_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINT_MEDIA_TABLE_SOURCE,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_IPP_VERSIONS_SUPPORTED,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_AIRPRINT_ENABLED,
                                    oid_module_sm_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_SM_PRINTER_GEO_LOCATION,
                                    oid_module_sm_get_string,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

}

