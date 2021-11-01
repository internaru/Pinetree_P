/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file http_translate_oid_to_html.h
 *
 * \brief This file implements the protocol-specific OID to HTML translation
 * needed for the product's HTML/Toolbox.  The HTTP module reads pages and parses
 * them for OIDs.  When an OID is needed, this function is called to translate
 * the OID into the HTML/Toolbox specific protocol value.
 * 
 **/

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "oid_api.h"
#include "error_types.h"
#include "ATypes.h" /* for NULL */
#include "dprintf.h"
#include "lassert.h"

#include "location_api.h"
#include "string_mgr_api.h"
#include "string_mgr_api.h"
#include "http_config.h"
#ifdef WIRED_NETWORK
#include "net_iface_vars.h"
#ifdef HAVE_ETHERNET
#include "eth_mac_api.h"
#include "eth_phy_api.h"
#endif
#endif
#ifdef HAVE_PRINT_SUPPORT
#include "agprint.h"
#endif
#ifdef HAVE_PAPER
#include "paper_size_api.h"
#include "paper_types_config.h"
#endif
#include "logger.h"

/** \brief The base we want to use for strtoul function */
#define BASE_10 10


/** \brief A helper macro for getting the number of elements in an array */
#define NUM_ELEMENTS_IN_ARRAY( array ) sizeof( array ) / sizeof( array[0] )


#ifdef HAVE_ETHERNET
/**
 * \brief Map the network status to the HTML network status code 
 *
 * \param network_status The network modules's status
 *
 * \param html_network_status A pointer to a location large enough to hold the
 * HTML network status 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_network_status_to_html_network_status( eth_mac_link_status_t network_status, string_mgr_code_t * html_network_status );
#endif

/**
 *
 * \brief Translate an OID string for a particular OID to its protocol
 * specific value.  For example, an HTML protocol might want the network status
 * type converted from the OID string value ("1") to an english string 
 * value, like "Connected".
 *
 * \param oid A valid oid_t from oid_api.h
 *
 * \param output_string A string buffer that is at least 
 * maximum_string_size in bytes
 *
 * \param maximum_string_size The minimum size of the output_string that
 * is passed in
 *
 * \return error_type_t OK upon success
 * FAIL otherwise
 *
 * Note:
 * This function is product and/or OEM specific and therefore multiple
 * files may implement the same function.  It is up to the team developing
 * the product to choose the correct function/file corresponding to the OEM
 * the product is for.
 *
 * This function calls the appropriate OID APIs to get the original OID value.
 *
 */
error_type_t http_translate_oid_to_html_protocol( oid_t oid, uint32_t index, char * output_string, uint32_t maximum_string_size )
{
    error_type_t e_res;
    uint8_t type;
    uint32_t value = 0;

    e_res = OK;

    if ( ( NULL == output_string ) || ( 0 == maximum_string_size ) )
    {
        e_res = FAIL;
    }

    type = oid_type_id_get( oid );
    if ( OK == e_res )
    {
        /* get the OID */
        if ( type == OID_TYPE_UINT32 )
        {
            e_res = oid_get_uint32( oid, index, &value );
        }
        else if ( type == OID_TYPE_STRING )
        {
            e_res = oid_get_string( oid,
                                    index,
                                    output_string,
                                    maximum_string_size );
        }
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_NETWORK_STATUS:
            {
#ifdef HAVE_ETHERNET
                unsigned int string_size;
                string_mgr_code_t html_network_status;
                const char * network_status_string;

                e_res = map_network_status_to_html_network_status( (eth_mac_link_status_t)value, &html_network_status );

                if ( OK == e_res )
                {
                     network_status_string = string_mgr_get_string( html_network_status, string_mgr_loc_full );

                     if ( NULL == network_status_string )
                     {
                         e_res = FAIL;
                     }

                     if ( OK == e_res )
                     {
                         if ( '\0' == network_status_string[0] )
                         {
                             e_res = FAIL;
                         }
                     }
                }

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%s",
                            network_status_string );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }
#endif

                break;
            }

            case OID_STRING_MGR_LANGUAGE_CODE:
            {
                unsigned int string_size;
                char * midas_language_code_string;

                e_res = map_language_code_to_midas_language_code_string( ( string_mgr_language_t )value, &midas_language_code_string );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%s",
                            midas_language_code_string );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }

                break;
            }

            case OID_NETWORK_LINK_SPEED:
            {
#ifdef HAVE_ETHERNET
                unsigned int string_size;
                uint32_t midas_network_speed_code;

                e_res = map_network_speed_to_midas_network_speed_code( (eth_mac_link_status_t)value, &midas_network_speed_code );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%u",
                            midas_network_speed_code );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }
#endif

                break;
            }

            case OID_PRINT_MEDIA_SIZE_LETTER_ID:
            /* fall through */
            case OID_PRINT_MEDIA_SIZE_DEFAULT:
            /* fall through */
            case OID_PRINT_INPUT_TRAY_MEDIA_SIZE:
            {
#ifdef HAVE_PAPER
                unsigned int string_size;
                uint32_t midas_media_size;

                e_res = map_media_size_to_midas_media_size( (mediasize_t)value, &midas_media_size );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                                             (int)maximum_string_size,
                                             &string_size,
                                             "%u",
                                             midas_media_size );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }
#endif                

                break;
            }

			case OID_PRINT_INPUT_TYPE_DEFAULT:
            {
#ifdef HAVE_PRINT_SUPPORT
                unsigned int string_size;
                uint32_t midas_media_source;

                e_res = map_media_source_to_midas_media_source( (INPUTTYPE)value, &midas_media_source );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                                             (int)maximum_string_size,
                                             &string_size,
                                             "%u",
                                             midas_media_source );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }
#endif

                break;
            }

            case OID_PRINT_INPUT_TRAY_MEDIA_TYPE:
            /* Fall through */
            case OID_PRINT_MEDIA_TYPE_DEFAULT:
            /* Fall through */
            case OID_PRINT_MEDIA_TYPE_PLAIN_ID:
            {
#ifdef HAVE_PAPER
                unsigned int string_size;
                uint32_t midas_media_type;

                e_res = map_media_type_to_midas_media_type( (mediatype_t)value, &midas_media_type );
                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%u",
                            midas_media_type );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }
#endif

                break;
            }

            default:
            {
                // If it is an integer value that is not mapped to something else
                // convert it to a string at this point
                if ( type == OID_TYPE_UINT32 )
                {
                    unsigned int string_size;
                    e_res = min_n_k_sprintf( output_string,
                                             maximum_string_size,
                                             &string_size,
                                             "%u",
                                             value );
                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );
                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }
                /* return the regular OID, do nothing here for the remainder of the OIDS */
                break;
            }
        }
    }

    return e_res;
}


/**
 *
 * \brief Translate a HTML protocol string to it's OID string for a particular
 * OID.  The function also calls the oid_set_string() function to set the
 * new value.  For example, an HTML protocol might want give the A4 paper
 * type as "17" and it would be converted to the OID string value of: "A4"
 *
 * \param oid A valid oid_t from oid_api.h
 *
 * \param html_string A null-terminated string representing the OID, but in the
 * html protocol format
 *
 * \param html_string_len The length of the html_string to be converted
 *
 * \return error_type_t OK upon success
 * FAIL otherwise
 *
 * Note:
 * This function is product and/or OEM specific and therefore multiple
 * files may implement the same function.  It is up to the team developing
 * the product to choose the correct function/file corresponding to the OEM
 * the product is for.
 *
 * This function calls the appropriate OID APIs to set the new OID value.
 *
 */
error_type_t http_translate_html_protocol_to_oid( oid_t oid, uint32_t index, char * html_string, uint32_t html_string_len )
{
    error_type_t e_res;
    char oid_string_temp[OID_MAX_STRING_SIZE_IN_BYTES];
    char * oid_string;
    uint32_t oid_string_len;
    uint32_t oid_int;
    uint8_t type;

    e_res = OK;
    oid_string_len = html_string_len;
    oid_string = &( oid_string_temp[0] );
    oid_int = 0;
    type = oid_type_id_get( oid );

    if ( ( NULL == html_string ) || ( 0 == html_string_len ) )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_STRING_MGR_LANGUAGE_CODE:
            {
                string_mgr_language_t code;

				/* HTML uses the MIDAS protocol to send values */
                /* get the string manager language code */
                e_res = map_midas_language_code_string_to_language_code( html_string, &code );
                if ( OK == e_res )
                {
                    oid_int = code;
                }

                break;
            }

            case OID_PRINT_INPUT_TRAY_MEDIA_SIZE:
            /* fall through */
            case OID_PRINT_MEDIA_SIZE_DEFAULT:
            {
#ifdef HAVE_PAPER
                mediasize_t media_size;
                uint32_t midas_media_size_code;

                /* convert string */
                midas_media_size_code = (uint32_t)strtoul(html_string, NULL, BASE_10);

                e_res = map_midas_media_size_to_media_size( midas_media_size_code, &media_size );
                if ( OK == e_res )
                {
                    oid_int = (uint32_t)media_size;
                }
#endif

                break;
            }

            case OID_PRINT_INPUT_TRAY_MEDIA_TYPE:
            /* fall through */
            case OID_PRINT_MEDIA_TYPE_DEFAULT:
            {
#ifdef HAVE_PAPER
                mediatype_t media_type;
                uint32_t midas_media_type_code;

                /* convert string */
                midas_media_type_code = (uint32_t)strtoul(html_string, NULL, BASE_10);

                e_res = map_midas_media_type_to_media_type( midas_media_type_code, &media_type );

                if ( OK == e_res )
                {
                    oid_int = media_type;
                }
#endif

                break;
            }

            case OID_PRINT_INPUT_TYPE_DEFAULT:
            {
#ifdef HAVE_PRINT_SUPPORT
                INPUTTYPE input_type;
                uint32_t midas_input_type_code;

                /* convert string */
                midas_input_type_code = (uint32_t)strtoul(html_string, NULL, BASE_10);

                e_res = map_midas_media_source_to_media_source( midas_input_type_code, &input_type );

                if ( OK == e_res )
                {
                    oid_int = midas_input_type_code;
                }
#endif

                break;
            }

            default:
            {
                /* return the regular OID, use the html string as the oid string
                 * since no conversion is needed */
                switch ( type )
                {
                    case OID_TYPE_STRING:
                        oid_string = html_string;
                        break;
                    case OID_TYPE_UINT32:
                        oid_int = strtoul(html_string, NULL, BASE_10);
                        break;
                    default:
                        DPRINTF( (DBG_ERROR), ( "%s Unhandled type in convert: %d\n", __FUNCTION__, type ) );
                        e_res = FAIL;
                        break;
                }
                break;
            }
        }
    }

    if ( OK == e_res )
    {
        /* attempt to set the oid */
        switch ( type )
        {
            case OID_TYPE_STRING:
                e_res = oid_set_string( oid, index, oid_string, oid_string_len );
                break;
            case OID_TYPE_UINT32:
                e_res = oid_set_uint32( oid, index, oid_int );
                break;
            default:
                DPRINTF( (DBG_ERROR), ( "%s Unhandled type in set: %d\n", __FUNCTION__, type ) );
                break;
        }
        
    }

    return e_res;
}


#ifdef HAVE_ETHERNET
/** \brief Contains the lookup information for the HTML network status enum */
typedef struct network_status_to_html_network_status_s
{
    eth_mac_link_status_t network_status; /*!< The network status code */

    uint32_t html_network_status; /*!< The HTML network status code */

} network_status_to_html_network_status_t;


/** \brief lookup table for the network status to HTML network status */
static network_status_to_html_network_status_t network_status_to_html_network_status_table[] = {
    { ETH_MAC_LINK_STATUS_10_HALF, STRING_EWS_NETWORK_CONNECTED },
    { ETH_MAC_LINK_STATUS_10_FULL, STRING_EWS_NETWORK_CONNECTED },
    { ETH_MAC_LINK_STATUS_100_HALF, STRING_EWS_NETWORK_CONNECTED },
    { ETH_MAC_LINK_STATUS_100_FULL, STRING_EWS_NETWORK_CONNECTED },
    { ETH_MAC_LINK_STATUS_1G_HALF, STRING_EWS_NETWORK_CONNECTED },
    { ETH_MAC_LINK_STATUS_1G_FULL, STRING_EWS_NETWORK_CONNECTED },
    { ETH_MAC_LINK_STATUS_DOWN, STRING_EWS_NETWORK_DISCONNECTED },
};


static error_type_t map_network_status_to_html_network_status( eth_mac_link_status_t network_status, string_mgr_code_t * html_network_status )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == html_network_status )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( network_status_to_html_network_status_table ); table_index++ )
        {
            if ( network_status == network_status_to_html_network_status_table[table_index].network_status )
            {
                *html_network_status = network_status_to_html_network_status_table[table_index].html_network_status;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *html_network_status = STRING_EWS_NETWORK_ERROR;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif
