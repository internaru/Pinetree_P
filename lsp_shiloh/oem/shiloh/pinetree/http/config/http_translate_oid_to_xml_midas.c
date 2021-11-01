/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file http_translate_oid_to_xml_midas.h
 *
 * \brief This file implements the protocol-specific OID to XML translation
 * needed for the MIDAS protocol.  The HTTP module reads pages and parses
 * them for OIDs.  When an OID is needed, this function is called to translate
 * the OID into the MIDAS specific protocol value.
 * 
 **/

#include "oid_api.h"
#include "error_types.h"
#include <stdint.h>
#include "ATypes.h" /* for NULL */
#include <string.h>
#include <stdlib.h>
#include "dprintf.h"
#include "lassert.h"


#include "location_api.h"
#include "string_mgr_api.h"
#ifdef HAVE_PAPER
#include "paper_size_api.h"
#include "paper_types_config.h"
#endif
#ifdef HAVE_PRINT_SUPPORT
#include "agprint.h"
#include "print_status_codes.h"
#endif

#include "http_midas_translation_enums.h"
#include "http_config.h"

#ifdef HAVE_ETHERNET
#include "net_iface.h"
#include "net_iface_vars.h"
#include "eth_mac_api.h"
#endif
#include "logger.h"

/** \brief The base we want to use for strtoul function */
#define BASE_10 10


/** \brief A helper macro for getting the number of elements in an array */
#define NUM_ELEMENTS_IN_ARRAY( array ) sizeof( array ) / sizeof( array[0] )


/**
 * \brief Map the system status to the MIDAS protocol status enum
 *
 * \param sys_status The system status
 *
 * \param midas_dev_status A pointer to a memory location where the midas enum
 * will be filled in
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_sys_status_to_midas_dev_status_type( uint32_t sys_status, uint32_t * midas_dev_status );


#ifdef WIRED_NETWORK
/**
 * \brief Map the network info's last boot info to the MIDAS ipv4 config method enum
 *
 * \param last_boot The network info's last boot code 
 *
 * \param midas_ipv4_config_method A pointer to a memory location where the midas enum
 * will be filled in
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_last_boot_to_midas_ipv4_config_method( net_iface_ipv4_config_method_t last_boot, uint32_t * midas_ipv4_config_method );
#endif

/**
 * \brief Map the system country code to the MIDAS ISO country string 
 *
 * \param code A valid e_CountryCode enum
 *
 * \param midas_country_code_string The MIDAS ISO country string
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_country_code_to_midas_country_code_string( e_CountryCode code, char ** midas_country_code_string );


#ifdef HAVE_PRINT_SUPPORT
/**
 * \brief Map the tray type to the MIDAS tray type code 
 *
 * \param tray_type A valid INPUTTYPE enum
 *
 * \param midas_tray_type A pointer to a location large enough to hold the
 * MIDAS tray type 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_tray_type_to_midas_tray_type( INPUTTYPE tray_type, uint32_t * midas_tray_type );
#endif


#ifdef HAVE_ETHERNET
/**
 * \brief Map the network status to the MIDAS network status code 
 *
 * \param network_status The network modules's status
 *
 * \param midas_network_status A pointer to a location large enough to hold the
 * MIDAS network status 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_network_status_to_midas_network_status( eth_mac_link_status_t network_status, uint32_t * midas_network_status );
#endif

/**
 * \brief Map the supply position to the MIDAS supply position code 
 *
 * \param supply_position supply position
 *
 * \param midas_supply_position A pointer to a location large enough to hold the
 * MIDAS supply position 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_supply_position_to_midas_supply_position( uint32_t supply_position, uint32_t * midas_supply_position );


/* The following functions map from MIDAS XML back to the system firmware standard */


#ifdef HAVE_ETHERNET
/**
 * \brief Map the MIDAS network speed string to the network link speed 
 *
 * \param midas_network_speed_code The MIDAS network speed code
 *
 * \param[out] network_speed A pointer to a location large enough to hold a
 * valid eth_phy_link_status_t enum
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
static error_type_t map_midas_network_speed_string_to_network_speed( uint32_t midas_network_speed_code, eth_mac_link_status_t * network_speed );
#endif


/**
 *
 * \brief Translate an OID string for a particular OID to its protocol
 * specific value.  For example, an XML protocol might want the A4 paper
 * type converted from the OID string value ("A4") to an enumerated
 * value, like "206001".
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
error_type_t http_translate_oid_to_xml_protocol( oid_t oid, uint32_t index, char * output_string, uint32_t maximum_string_size )
{
    error_type_t e_res;
    uint8_t type;
    uint32_t value;

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
            case OID_STATUS_GENERAL:
            {
                unsigned int string_size;
                uint32_t midas_status;
 
                e_res = map_sys_status_to_midas_dev_status_type( value, &midas_status );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%u",
                            midas_status );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }

                break;
            }

            case OID_NETWORK_IPV4_LAST_CONFIGURED_BY:
            {
#ifdef WIRED_NETWORK
                unsigned int string_size;
                uint32_t midas_ipv4_config_method;

                e_res = map_last_boot_to_midas_ipv4_config_method( (net_iface_ipv4_config_method_t)value, &midas_ipv4_config_method );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%u",
                            midas_ipv4_config_method );

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

            case OID_PLATFORM_COUNTRY_CODE:
            {
                unsigned int string_size;
                char * midas_country_code_string;

                e_res = map_country_code_to_midas_country_code_string( (e_CountryCode)value, &midas_country_code_string );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%s",
                            midas_country_code_string );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }

                break;
            }

            case OID_STRING_MGR_LANGUAGE_CODE:
            {
                unsigned int string_size;
                char * midas_language_code_string;

                e_res = map_language_code_to_midas_language_code_string( (string_mgr_language_t)value, &midas_language_code_string );

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

            case OID_PRINT_INPUT_TRAY_TRAY_ID:
            {
#ifdef HAVE_PRINT_SUPPORT
                unsigned int string_size;
                uint32_t midas_tray_type;

                e_res = map_tray_type_to_midas_tray_type( ( INPUTTYPE )value, &midas_tray_type );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%u",
                            midas_tray_type );

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

            case OID_NETWORK_STATUS:
            {
#ifdef HAVE_ETHERNET
                unsigned int string_size;
                uint32_t midas_network_status;

                e_res = map_network_status_to_midas_network_status( (eth_mac_link_status_t)value, &midas_network_status );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%u",
                            midas_network_status );

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

            case OID_CONSUMABLE_SUPPLY_POSITION:
            {
                unsigned int string_size;
                uint32_t midas_supply_position;

                e_res = map_supply_position_to_midas_supply_position( value, &midas_supply_position );

                if ( OK == e_res )
                {
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%u",
                            midas_supply_position );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }

                break;
            }

            default:
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

    return e_res;
}


/**
 *
 * \brief Translate a XML protocol string to it's OID string for a particular
 * OID.  The function also calls the oid_set_string() function to set the
 * new value.  For example, an XML protocol might want give the A4 paper
 * type as "206001" and it would be converted to the OID string value of: "A4"
 *
 * \param oid A valid oid_t from oid_api.h
 *  
 * \param index Index associated with oid 
 *  
 * \param xml_string A null-terminated string representing the OID, but in the
 * XML protocol format
 *
 * \param xml_string_len The length of the xml_string to be converted
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
error_type_t http_translate_xml_protocol_to_oid( oid_t oid, uint32_t index, char * xml_string, uint32_t xml_string_len )
{
    error_type_t e_res;
    char oid_string_temp[OID_MAX_STRING_SIZE_IN_BYTES];
    char * oid_string;
    uint32_t oid_string_len;
    uint32_t oid_int;
    uint8_t type;

    e_res = OK;
    oid_string_len = xml_string_len;
    oid_string = &( oid_string_temp[0] );
    oid_int = 0;
    type = oid_type_id_get( oid );

    if ( ( NULL == xml_string ) || ( 0 == xml_string_len ) )
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

                /* get the string manager language code */
                e_res = map_midas_language_code_string_to_language_code( xml_string, &code );
                if ( OK == e_res )
                {
                    oid_int = (uint32_t)code;
                }

                break;
            }

            case OID_NETWORK_LINK_SPEED:
            {
#ifdef HAVE_ETHERNET
                eth_mac_link_status_t network_speed;
                uint32_t midas_network_speed_code;

                /* convert string */
                midas_network_speed_code = (uint32_t)strtoul(xml_string, NULL, BASE_10);

                e_res = map_midas_network_speed_string_to_network_speed( midas_network_speed_code, &network_speed );

                if ( OK == e_res )
                {
                    oid_int = (uint32_t)network_speed;
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
                midas_input_type_code = (uint32_t)strtoul(xml_string, NULL, BASE_10);

                e_res = map_midas_media_source_to_media_source( midas_input_type_code, &input_type );
                if ( OK == e_res )
                {
                    oid_int = (uint32_t)input_type;
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
                midas_media_type_code = (uint32_t)strtoul(xml_string, NULL, BASE_10);

                e_res = map_midas_media_type_to_media_type( midas_media_type_code, &media_type );

                if ( OK == e_res )
                {
                    oid_int = (uint32_t)media_type;
                }
#endif

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
                midas_media_size_code = (uint32_t)strtoul(xml_string, NULL, BASE_10);

                e_res = map_midas_media_size_to_media_size( midas_media_size_code, &media_size );

                if ( OK == e_res )
                {
                    oid_int = (uint32_t)media_size;
                }
#endif

                break;
            }
            
            default:
            {
                /* return the regular OID, use the XML string as the oid string
                 * since no conversion is needed */
                switch ( type )
                {
                    case OID_TYPE_STRING:
                        oid_string = xml_string;
                        break;
                    case OID_TYPE_UINT32:
                        oid_int = strtoul(xml_string, NULL, BASE_10);
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
                DPRINTF( (DBG_ERROR), ( "%s Unhandled type in X set: %d\n", __FUNCTION__, type ) );
                break;
        }
    }

    return e_res;
}


static error_type_t map_sys_status_to_midas_dev_status_type( uint32_t sys_status, uint32_t * midas_dev_status )
{
    /* TODO turn this into a table lookup like the rest of the map functions */
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_dev_status )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        if ( STATUS_INFO_PRINTING == ( sys_status & STATUS_INFO_PRINTING ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PRINTING;
        }
        else if ( STATUS_INITIALIZING == ( sys_status & STATUS_INITIALIZING ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_INITIALIZING;
        }
        else if ( STATUS_INFO_CLEANING == ( sys_status & STATUS_INFO_CLEANING ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CLEANING;
        }
        else if ( STATUS_INFO_BEGIN_MANUALDUPLEX == ( sys_status & STATUS_INFO_BEGIN_MANUALDUPLEX ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_BEGIN_MANUAL_DUPLEX;
        }
        else if ( STATUS_INPUT_MANUAL_DUPLEX == ( sys_status & STATUS_INPUT_MANUAL_DUPLEX ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_INPUT_MANUAL_DUPLEX;
        }
        /* TODO */
        else if ( STATUS_INFO_PRINTING_CAL_GRAPH == ( sys_status & STATUS_INFO_PRINTING_CAL_GRAPH ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CALIBRATING;
        }
        else if ( STATUS_PAPER_OUT_TRAY1 == ( sys_status & STATUS_PAPER_OUT_TRAY1 ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_OUT_TRAY1;
        }
        else if ( STATUS_PAPER_OUT_TRAY2 == ( sys_status & STATUS_PAPER_OUT_TRAY2 ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_OUT_TRAY2;
        }
        else if ( STATUS_PAPER_OUT_TRAY3 == ( sys_status & STATUS_PAPER_OUT_TRAY3 ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_OUT_TRAY3;
        }
        /*else if ( STATUS_INK_LOW_BLACK == ( sys_status & STATUS_INK_LOW_BLACK ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_LOW_BLACK;
        }
        else if ( STATUS_INK_LOW_CYAN == ( sys_status & STATUS_INK_LOW_CYAN ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_LOW_CYAN;
        }
        else if ( STATUS_INK_LOW_MAGENTA == ( sys_status & STATUS_INK_LOW_MAGENTA ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_LOW_MAGENTA;
        }
        else if ( STATUS_INK_LOW_YELLOW == ( sys_status & STATUS_INK_LOW_YELLOW ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_LOW_YELLOW;
        }
        else if ( STATUS_INK_OUT_CYAN == ( sys_status & STATUS_INK_OUT_CYAN ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_OUT_CYAN;
        }
        else if ( STATUS_INK_OUT_MAGENTA == ( sys_status & STATUS_INK_OUT_MAGENTA ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_OUT_MAGENTA;
        }
        else if ( STATUS_INK_OUT_YELLOW == ( sys_status & STATUS_INK_OUT_YELLOW ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_OUT_YELLOW;
        }
        else if ( STATUS_INK_OUT_BLACK == ( sys_status & STATUS_INK_OUT_BLACK ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_MARKER_OUT_BLACK;
        }
        else if ( STATUS_CARTRIDGE_UNAUTHORIZED_C == ( sys_status & STATUS_CARTRIDGE_UNAUTHORIZED_C ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_UNAUTHORIZED_CYAN;
        }
        else if ( STATUS_CARTRIDGE_UNAUTHORIZED_Y == ( sys_status & STATUS_CARTRIDGE_UNAUTHORIZED_Y ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_UNAUTHORIZED_YELLOW;
        }
        else if ( STATUS_CARTRIDGE_UNAUTHORIZED_M == ( sys_status & STATUS_CARTRIDGE_UNAUTHORIZED_M ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_UNAUTHORIZED_MAGENTA;
        }
        else if ( STATUS_CARTRIDGE_UNAUTHORIZED_K == ( sys_status & STATUS_CARTRIDGE_UNAUTHORIZED_K ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_UNAUTHORIZED_BLACK;
        }
        else if ( STATUS_CARTRIDGE_UNAUTHORIZED == ( sys_status & STATUS_CARTRIDGE_UNAUTHORIZED ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_UNAUTHORIZED;
        }*/
        else if ( STATUS_PAPER_OUT_ALL == ( sys_status & STATUS_PAPER_OUT_ALL ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_OUT_ALL;
        }
        else if ( STATUS_PAPER_JAM_INPUT == ( sys_status & STATUS_PAPER_JAM_INPUT ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_JAM_INPUT;
        }
        else if ( STATUS_PAPER_JAM_OUTPUT == ( sys_status & STATUS_PAPER_JAM_OUTPUT ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_JAM_OUTPUT;
        }
        /* TODO */
        /*else if ( STATUS_CARTRIDGE_BADQA_K == ( sys_status & STATUS_CARTRIDGE_BADQA_K ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_DATA_ERROR_BLACK;
        }*/
        /* TODO */
        /*else if ( STATUS_CARTRIDGE_BADQA_C == ( sys_status & STATUS_CARTRIDGE_BADQA_C ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_DATA_ERROR_CYAN;
        }*/
        /* TODO */
        /*else if ( STATUS_CARTRIDGE_BADQA_M == ( sys_status & STATUS_CARTRIDGE_BADQA_M ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_DATA_ERROR_MAGENTA;
        }*/
        /* TODO */
        /*else if ( STATUS_CARTRIDGE_BADQA_Y == ( sys_status & STATUS_CARTRIDGE_BADQA_Y ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_DATA_ERROR_YELLOW;
        }*/
        else if ( STATUS_DOOROPEN_FD == ( sys_status & STATUS_DOOROPEN_FD ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_DOOROPEN_REAR;
        }
        else if ( STATUS_DOOROPEN_FRONT == ( sys_status & STATUS_DOOROPEN_FRONT ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_DOOROPEN_FRONT;
        }
        else if ( STATUS_PAPER_JAM_TRAY1 == ( sys_status & STATUS_PAPER_JAM_TRAY1 ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_JAM_TRAY1_AREA;
        }
        else if ( STATUS_PAPER_JAM_TRAY2 == ( sys_status & STATUS_PAPER_JAM_TRAY2 ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_JAM_TRAY2_AREA;
        }
        else if ( STATUS_PAPER_JAM_TRAY3 == ( sys_status & STATUS_PAPER_JAM_TRAY3 ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_JAM_TRAY3_AREA;
        }
        else if ( STATUS_PAPER_JAM_DOOR_OPEN == ( sys_status & STATUS_PAPER_JAM_DOOR_OPEN ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_PAPER_JAM_DOOR_OPEN;
        }
       /* else if ( STATUS_CARTRIDGE_MISSING_K == ( sys_status & STATUS_CARTRIDGE_MISSING_K ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_MISSING_BLACK;
        }
        else if ( STATUS_CARTRIDGE_MISSING_C == ( sys_status & STATUS_CARTRIDGE_MISSING_C ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_MISSING_CYAN;
        }
        else if ( STATUS_CARTRIDGE_MISSING_M == ( sys_status & STATUS_CARTRIDGE_MISSING_M ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_MISSING_MAGENTA;
        }
        else if ( STATUS_CARTRIDGE_MISSING_Y == ( sys_status & STATUS_CARTRIDGE_MISSING_Y ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_MISSING_YELLOW;
        }
        else if ( STATUS_CARTRIDGE_MISSING_AUX == ( sys_status & STATUS_CARTRIDGE_MISSING_AUX ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CARTRIDGE_MISSING_AUX;
        }*/
        else if ( STATUS_INFO_CANCELING == ( sys_status & STATUS_INFO_CANCELING ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_CANCELING;
        }
        /* TODO: determine if this status is actually equivalent */
        else if ( STATUS_MEMORY_JOB_MEM_LOW == ( sys_status & STATUS_MEMORY_JOB_MEM_LOW ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_OUT_OF_MEMORY;
        }
        /* TODO: determine if this status is actually equivalent */
        else if ( STATUS_CONNECTION_FAIL == ( sys_status & STATUS_CONNECTION_FAIL ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_ENGINE_COMMUNICATION_ERROR;
        }
        else if ( STATUS_ONLINE == ( sys_status & STATUS_ONLINE ) )
        {
            *midas_dev_status = MIDAS_DEV_STATUS_READY;
        }
        else
        {
            *midas_dev_status = MIDAS_DEV_STATUS_NULL;
        }
    }

    return e_res;
}


#ifdef WIRED_NETWORK
/** \brief Contains the lookup information for the MIDAS ipv4 config method */
typedef struct last_boot_to_ipv4_config_method_entry_s
{
    net_iface_ipv4_config_method_t last_boot; /*!< The network info's last boot code */

    uint32_t ipv4_config_method; /*!< The MIDAS enum ipv4 config method code */

} last_boot_to_ipv4_config_method_entry_t;

static last_boot_to_ipv4_config_method_entry_t last_boot_to_ipv4_config_method_table[] = {

    { NET_IFACE_IPV4_CONFIGURED_MANUAL, MIDAS_IPV4_CONFIG_METHOD_MANUAL },
    { NET_IFACE_IPV4_CONFIGURED_DHCP, MIDAS_IPV4_CONFIG_METHOD_DHCP },
    { NET_IFACE_IPV4_CONFIGURED_BOOTP, MIDAS_IPV4_CONFIG_METHOD_BOOTP },
    { NET_IFACE_IPV4_CONFIGURED_AUTOIP, MIDAS_IPV4_CONFIG_METHOD_AUTOIP }
};


static error_type_t map_last_boot_to_midas_ipv4_config_method( net_iface_ipv4_config_method_t last_boot, uint32_t * midas_ipv4_config_method )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_ipv4_config_method )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( last_boot_to_ipv4_config_method_table ); table_index++ )
        {
            if ( last_boot == last_boot_to_ipv4_config_method_table[table_index].last_boot )
            {
                *midas_ipv4_config_method = last_boot_to_ipv4_config_method_table[table_index].ipv4_config_method;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_ipv4_config_method = MIDAS_IPV4_CONFIG_METHOD_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif


/** \brief Contains the lookup information for the MIDAS country code string */
typedef struct country_code_to_midas_country_code_string_entry_s
{
    e_CountryCode code; /*!< The system country code */

    char * midas_country_code; /*!< A pointer to the MIDAS ISO country code */

} country_code_to_midas_country_code_string_entry_t;


/** \brief lookup table for the country code to MIDAS code */
static country_code_to_midas_country_code_string_entry_t country_code_to_midas_country_code_string_table[] = {

    { USA, MIDAS_ISO_COUNTRY_CODE_USA },
    { UK, MIDAS_ISO_COUNTRY_CODE_UNITED_KINGDOM }
};


static error_type_t map_country_code_to_midas_country_code_string( e_CountryCode code, char ** midas_country_code_string )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_country_code_string )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( country_code_to_midas_country_code_string_table ); table_index++ )
        {
            if ( code == country_code_to_midas_country_code_string_table[table_index].code )
            {
                *midas_country_code_string = country_code_to_midas_country_code_string_table[table_index].midas_country_code;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_country_code_string = MIDAS_ISO_COUNTRY_CODE_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}


/** \brief Contains the lookup information for the MIDAS language code string */
typedef struct language_code_to_midas_language_code_string_entry_s
{
    string_mgr_language_t code; /*!< The system language code */

    char * midas_language_code; /*!< A pointer to the MIDAS ISO language code */

} language_code_to_midas_language_code_string_entry_t;


/** \brief lookup table for the language code to MIDAS code */
static language_code_to_midas_language_code_string_entry_t language_code_to_midas_language_code_string_table[] = {

    { LANG_ENGLISH, MIDAS_ISO_LANGUAGE_CODE_ENGLISH },
    { LANG_SPANISH, MIDAS_ISO_LANGUAGE_CODE_SPANISH }
};


error_type_t map_language_code_to_midas_language_code_string( string_mgr_language_t code, char ** midas_language_code_string )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_language_code_string )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( language_code_to_midas_language_code_string_table ); table_index++ )
        {
            if ( code == language_code_to_midas_language_code_string_table[table_index].code )
            {
                *midas_language_code_string = language_code_to_midas_language_code_string_table[table_index].midas_language_code;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_language_code_string = MIDAS_ISO_LANGUAGE_CODE_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}


#ifdef HAVE_ETHERNET
/** \brief Contains the lookup information for the MIDAS network speed */
typedef struct network_speed_to_midas_network_speed_code_s
{
    eth_mac_link_status_t network_speed; /*!< The network speed code */

    uint32_t midas_network_speed_code; /*!< The MIDAS network speed code */

} network_speed_to_midas_network_speed_code_t;


/** \brief lookup table for the network speed to MIDAS code */
//FIXME
static network_speed_to_midas_network_speed_code_t network_speed_to_midas_network_speed_code_table[] = {

    { ETH_MAC_LINK_STATUS_DOWN, MIDAS_LINK_SPEED_NULL },
    { ETH_MAC_LINK_STATUS_10_FULL, MIDAS_LINK_SPEED_10F },
    { ETH_MAC_LINK_STATUS_10_HALF, MIDAS_LINK_SPEED_10H },
    { ETH_MAC_LINK_STATUS_100_FULL, MIDAS_LINK_SPEED_100F },
    { ETH_MAC_LINK_STATUS_100_HALF, MIDAS_LINK_SPEED_100H },
    { ETH_MAC_LINK_STATUS_1G_FULL, MIDAS_LINK_SPEED_1000F },
    { ETH_MAC_LINK_STATUS_1G_HALF, MIDAS_LINK_SPEED_1000H },
};


error_type_t map_network_speed_to_midas_network_speed_code( eth_mac_link_status_t network_speed, uint32_t * midas_network_speed_code )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_network_speed_code )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( network_speed_to_midas_network_speed_code_table ); table_index++ )
        {
            if ( network_speed == network_speed_to_midas_network_speed_code_table[table_index].network_speed )
            {
                *midas_network_speed_code = network_speed_to_midas_network_speed_code_table[table_index].midas_network_speed_code;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_network_speed_code = MIDAS_LINK_SPEED_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif


#ifdef HAVE_PAPER
/** \brief Contains the lookup information for the MIDAS media size enum */
typedef struct media_size_to_midas_media_size_s
{
    mediasize_t media_size; /*!< The system media size code */

    uint32_t midas_media_size; /*!< The MIDAS media size code */

} media_size_to_midas_media_size_t;


/** \brief lookup table for the system media size to MIDAS media size */
static media_size_to_midas_media_size_t media_size_to_midas_media_size_table[] = {
    { 0, MIDAS_MEDIA_SIZE_MEDIA_LETTER }, /* TODO have print module report real value
                                              and remove this hack */
    { MEDIASIZE_USER_DEFINED, MIDAS_MEDIA_SIZE_MEDIA_CUSTOM_SIZE },
    { MEDIASIZE_LETTER, MIDAS_MEDIA_SIZE_MEDIA_LETTER },
    { MEDIASIZE_LEGAL, MIDAS_MEDIA_SIZE_MEDIA_LEGAL },
    { MEDIASIZE_9X11, MIDAS_MEDIA_SIZE_MEDIA_9X11 },
    { MEDIASIZE_10X11, MIDAS_MEDIA_SIZE_MEDIA_10X11 },
    { MEDIASIZE_10X14, MIDAS_MEDIA_SIZE_MEDIA_10X14 },
    { MEDIASIZE_15X11, MIDAS_MEDIA_SIZE_MEDIA_15X11 },
    { MEDIASIZE_11X17, MIDAS_MEDIA_SIZE_MEDIA_11X17 },
    { MEDIASIZE_12X11, MIDAS_MEDIA_SIZE_MEDIA_12X11 },
    { MEDIASIZE_A2, MIDAS_MEDIA_SIZE_MEDIA_A2 },
    { MEDIASIZE_A3, MIDAS_MEDIA_SIZE_MEDIA_A3 },
    { MEDIASIZE_A3_EXTRA, MIDAS_MEDIA_SIZE_MEDIA_A3_EXTRA },
    { MEDIASIZE_A4, MIDAS_MEDIA_SIZE_MEDIA_A4 },
    { MEDIASIZE_A4_EXTRA, MIDAS_MEDIA_SIZE_MEDIA_A4_EXTRA },
    { MEDIASIZE_A4_PLUS, MIDAS_MEDIA_SIZE_MEDIA_A4_PLUS },
    { MEDIASIZE_A4SMALL, MIDAS_MEDIA_SIZE_MEDIA_A4_SMALL },
    { MEDIASIZE_A5, MIDAS_MEDIA_SIZE_MEDIA_A5 },
    { MEDIASIZE_A5_EXTRA, MIDAS_MEDIA_SIZE_MEDIA_A5_EXTRA },
    { MEDIASIZE_A6, MIDAS_MEDIA_SIZE_MEDIA_A6 },
    { MEDIASIZE_A_PLUS, MIDAS_MEDIA_SIZE_MEDIA_A_PLUS },
    { MEDIASIZE_B4, MIDAS_MEDIA_SIZE_MEDIA_B4 },
    { MEDIASIZE_B5, MIDAS_MEDIA_SIZE_MEDIA_B5 },
    //UNUSED { , MIDAS_MEDIA_SIZE_MEDIA_B5_JIS },
    //UNUSED { , MIDAS_MEDIA_SIZE_MEDIA_B5_EXTRA },
    //UNUSED { , MIDAS_MEDIA_SIZE_MEDIA_B6_JIS },
    { MEDIASIZE_B_PLUS, MIDAS_MEDIA_SIZE_MEDIA_B_PLUS },
    { MEDIASIZE_CSHEET, MIDAS_MEDIA_SIZE_MEDIA_CSHEET },
    { MEDIASIZE_DBL_JAPANESE_POSTCARD, MIDAS_MEDIA_SIZE_MEDIA_DBL_JAPANESE_POSTCARD },
    { MEDIASIZE_DSHEET, MIDAS_MEDIA_SIZE_MEDIA_DSHEET },
    { MEDIASIZE_ENV_9, MIDAS_MEDIA_SIZE_MEDIA_ENV_9 },
    { MEDIASIZE_ENV_10, MIDAS_MEDIA_SIZE_MEDIA_ENV_10 },
    { MEDIASIZE_ENV_11, MIDAS_MEDIA_SIZE_MEDIA_ENV_11 },
    { MEDIASIZE_ENV_12, MIDAS_MEDIA_SIZE_MEDIA_ENV_12 },
    { MEDIASIZE_ENV_14, MIDAS_MEDIA_SIZE_MEDIA_ENV_14 },
    { MEDIASIZE_ENV_C5, MIDAS_MEDIA_SIZE_MEDIA_ENV_C5 },
    { MEDIASIZE_ENV_C3, MIDAS_MEDIA_SIZE_MEDIA_ENV_C3 },
    { MEDIASIZE_ENV_C4, MIDAS_MEDIA_SIZE_MEDIA_ENV_C4 },
    { MEDIASIZE_ENV_C6, MIDAS_MEDIA_SIZE_MEDIA_ENV_C6 },
    { MEDIASIZE_ENV_C65, MIDAS_MEDIA_SIZE_MEDIA_ENV_C65 },
    { MEDIASIZE_ENV_B4, MIDAS_MEDIA_SIZE_MEDIA_ENV_B4 },
    { MEDIASIZE_ENV_B5, MIDAS_MEDIA_SIZE_MEDIA_ENV_B5 },
    { MEDIASIZE_ENV_B6, MIDAS_MEDIA_SIZE_MEDIA_ENV_B6 },
    { MEDIASIZE_ENV_DL, MIDAS_MEDIA_SIZE_MEDIA_ENV_DL },
    { MEDIASIZE_ENV_INVITE, MIDAS_MEDIA_SIZE_MEDIA_ENV_INVITE },
    { MEDIASIZE_ENV_ITALY, MIDAS_MEDIA_SIZE_MEDIA_ENV_ITALY },
    { MEDIASIZE_ENV_MONARCH, MIDAS_MEDIA_SIZE_MEDIA_ENV_MONARCH },
    { MEDIASIZE_ENV_PERSONAL, MIDAS_MEDIA_SIZE_MEDIA_ENV_PERSONAL },
    { MEDIASIZE_ESHEET, MIDAS_MEDIA_SIZE_MEDIA_ESHEET },
    { MEDIASIZE_EXECUTIVE, MIDAS_MEDIA_SIZE_MEDIA_EXECUTIVE },
    { MEDIASIZE_FANFOLD_US, MIDAS_MEDIA_SIZE_MEDIA_FANFOLD_US },
    { MEDIASIZE_FANFOLD_STD_GERMAN, MIDAS_MEDIA_SIZE_MEDIA_FANFOLD_STD_GERMAN },
    { MEDIASIZE_FANFOLD_LGL_GERMAN, MIDAS_MEDIA_SIZE_MEDIA_FANFOLD_LGL_GERMAN },
    //UNUSED { , MIDAS_MEDIA_SIZE_MEDIA_FOLIO },
    { MEDIASIZE_ISO_B4, MIDAS_MEDIA_SIZE_MEDIA_ISO_B4 },
    { MEDIASIZE_JAPANESE_POSTCARD, MIDAS_MEDIA_SIZE_MEDIA_JAPANESE_POSTCARD },
    { MEDIASIZE_JENV_CHOU3, MIDAS_MEDIA_SIZE_MEDIA_JENV_CHOU3 },
    { MEDIASIZE_JENV_CHOU4, MIDAS_MEDIA_SIZE_MEDIA_JENV_CHOU4 },
    { MEDIASIZE_JENV_KAKU2, MIDAS_MEDIA_SIZE_MEDIA_JENV_KAKU2 },
    { MEDIASIZE_JENV_KAKU3, MIDAS_MEDIA_SIZE_MEDIA_JENV_KAKU3 },
    { MEDIASIZE_JENV_YOU4, MIDAS_MEDIA_SIZE_MEDIA_JENV_YOU4 },
    //UNUSED { , MIDAS_MEDIA_SIZE_MEDIA_LAST },
    { MEDIASIZE_LEDGER, MIDAS_MEDIA_SIZE_MEDIA_LEDGER },
    { MEDIASIZE_LEGAL_EXTRA, MIDAS_MEDIA_SIZE_MEDIA_LEGAL_EXTRA },
    { MEDIASIZE_LETTER_EXTRA, MIDAS_MEDIA_SIZE_MEDIA_LETTER_EXTRA },
    { MEDIASIZE_LETTER_ROTATED, MIDAS_MEDIA_SIZE_MEDIA_LETTER_ROTATED },
    { MEDIASIZE_LETTERSMALL, MIDAS_MEDIA_SIZE_MEDIA_LETTERSMALL },
    { MEDIASIZE_NOTE, MIDAS_MEDIA_SIZE_MEDIA_NOTE },
    { MEDIASIZE_P16K, MIDAS_MEDIA_SIZE_MEDIA_P16K },
    { MEDIASIZE_P32K, MIDAS_MEDIA_SIZE_MEDIA_P32K },
    { MEDIASIZE_P32KBIG, MIDAS_MEDIA_SIZE_MEDIA_P32KBIG },
    { MEDIASIZE_PENV_1, MIDAS_MEDIA_SIZE_MEDIA_PENV_1 },
    { MEDIASIZE_PENV_2, MIDAS_MEDIA_SIZE_MEDIA_PENV_2 },
    { MEDIASIZE_PENV_3, MIDAS_MEDIA_SIZE_MEDIA_PENV_3 },
    { MEDIASIZE_PENV_4, MIDAS_MEDIA_SIZE_MEDIA_PENV_4 },
    { MEDIASIZE_PENV_5, MIDAS_MEDIA_SIZE_MEDIA_PENV_5 },
    { MEDIASIZE_PENV_6, MIDAS_MEDIA_SIZE_MEDIA_PENV_6 },
    { MEDIASIZE_PENV_7, MIDAS_MEDIA_SIZE_MEDIA_PENV_7 },
    { MEDIASIZE_PENV_8, MIDAS_MEDIA_SIZE_MEDIA_PENV_8 },
    { MEDIASIZE_PENV_9, MIDAS_MEDIA_SIZE_MEDIA_PENV_9 },
    { MEDIASIZE_PENV_10, MIDAS_MEDIA_SIZE_MEDIA_PENV_10 },
    { MEDIASIZE_QUARTO, MIDAS_MEDIA_SIZE_MEDIA_QUARTO },
    { MEDIASIZE_STATEMENT, MIDAS_MEDIA_SIZE_MEDIA_STATEMENT },
    { MEDIASIZE_TABLOID, MIDAS_MEDIA_SIZE_MEDIA_TABLOID },
    { MEDIASIZE_TABLOID_EXTRA, MIDAS_MEDIA_SIZE_MEDIA_TABLOID_EXTRA }
};


error_type_t map_media_size_to_midas_media_size( mediasize_t media_size, uint32_t * midas_media_size )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_media_size )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( media_size_to_midas_media_size_table ); table_index++ )
        {
            if ( media_size == media_size_to_midas_media_size_table[table_index].media_size )
            {
                *midas_media_size = media_size_to_midas_media_size_table[table_index].midas_media_size;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_media_size = MIDAS_MEDIA_SIZE_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}


/** \brief Contains the lookup information for the MIDAS media source enum */
typedef struct media_source_to_midas_media_source_s
{
#ifdef HAVE_PRINT_SUPPORT
    INPUTTYPE media_source; /*!< The system media-source code */
#endif

    uint32_t midas_media_source; /*!< The MIDAS media-source code */

} media_source_to_midas_media_source_t;


#ifdef HAVE_PRINT_SUPPORT
/** \brief lookup table for the system media source to MIDAS media source */
static media_source_to_midas_media_source_t media_source_to_midas_media_source_table[] = {
    { INPUT_UPPER, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_LOWER, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_MIDDLE, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_MANUAL, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_TRACTOR, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_SMALLFMT, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_LARGEFMT, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_LARGECAPACITY, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },
    { INPUT_FORMSOURCE, MIDAS_MEDIA_SOURCE_SOURCE_MANUAL },

    { INPUT_AUTO, MIDAS_MEDIA_SOURCE_SOURCE_AUTO },
    { INPUT_ENVELOPE, MIDAS_MEDIA_SOURCE_SOURCE_ENVELOPE },
    { INPUT_ENVMANUAL, MIDAS_MEDIA_SOURCE_SOURCE_ENVELOPE_MANUAL },
    { INPUT_CASSETTE, MIDAS_MEDIA_SOURCE_SOURCE_CASSETTE }
};
#endif

#ifdef HAVE_PRINT_SUPPORT
error_type_t map_media_source_to_midas_media_source( INPUTTYPE media_source, uint32_t * midas_media_source )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_media_source )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( media_source_to_midas_media_source_table ); table_index++ )
        {
            if ( media_source == media_source_to_midas_media_source_table[table_index].media_source )
            {
                *midas_media_source = media_source_to_midas_media_source_table[table_index].midas_media_source;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_media_source = MIDAS_MEDIA_SOURCE_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif


/** \brief Contains the lookup information for the MIDAS tray type enum */
typedef struct tray_type_to_midas_tray_type_s
{
#ifdef HAVE_PRINT_SUPPORT
    INPUTTYPE tray_type; /*!< The system tray id code */
#endif

    uint32_t midas_tray_type; /*!< The MIDAS tray type code */

} tray_type_to_midas_tray_type_t;


#ifdef HAVE_PRINT_SUPPORT
/** \brief lookup table for the system tray id to MIDAS tray type */
static tray_type_to_midas_tray_type_t tray_type_to_midas_tray_type_table[] = {
    { 0, MIDAS_MEDIA_TRAY_TRAY_MANUAL }, /* TODO have print module report real value
                                              and remove this hack */
    { INPUT_LOWER, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_UPPER, MIDAS_MEDIA_TRAY_TRAY_UPPER_CASSETTE },
    { INPUT_MANUAL, MIDAS_MEDIA_TRAY_TRAY_MANUAL },
    { INPUT_ENVELOPE, MIDAS_MEDIA_TRAY_TRAY_ENVELOPE },
    /* The rest aren't implemented in MIDAS so we just report lower
     * TODO Ask if these should be implemented in MIDAS */
    { INPUT_MIDDLE, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_ENVMANUAL, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_AUTO, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_TRACTOR, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_SMALLFMT, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_LARGEFMT, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_LARGECAPACITY, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_CASSETTE, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE },
    { INPUT_FORMSOURCE, MIDAS_MEDIA_TRAY_TRAY_LOWER_CASSETTE }
    //UNUSED { , MIDAS_MEDIA_TRAY_TRAY_PHOTO }
    //UNUSED { , MIDAS_MEDIA_TRAY_TRAY_AUX_CASSETTE }
};
#endif


#ifdef HAVE_PRINT_SUPPORT
static error_type_t map_tray_type_to_midas_tray_type( INPUTTYPE tray_type, uint32_t * midas_tray_type )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_tray_type )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( tray_type_to_midas_tray_type_table ); table_index++ )
        {
            if ( tray_type == tray_type_to_midas_tray_type_table[table_index].tray_type )
            {
                *midas_tray_type = tray_type_to_midas_tray_type_table[table_index].midas_tray_type;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_tray_type = MIDAS_MEDIA_TRAY_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif


/** \brief Contains the lookup information for the MIDAS media type enum */
typedef struct media_type_to_midas_media_type_s
{
    mediatype_t media_type; /*!< The system media type code */

    uint32_t midas_media_type; /*!< The MIDAS media type code */

} media_type_to_midas_media_type_t;


/** \brief lookup table for the system media type to MIDAS media type */
static media_type_to_midas_media_type_t media_type_to_midas_media_type_table[] = {
    { MEDIATYPE_ANY, MIDAS_MEDIA_TYPE_MEDIA_PLAIN }, /* TODO have print module report real value
                                              and remove this hack */
    { MEDIATYPE_PLAIN_PAPER, MIDAS_MEDIA_TYPE_MEDIA_PLAIN },
    { MEDIATYPE_GLOSSY1, MIDAS_MEDIA_TYPE_MEDIA_GLOSSY },
    { MEDIATYPE_GLOSSY2, MIDAS_MEDIA_TYPE_MEDIA_GLOSSY },
    { MEDIATYPE_GLOSSY3, MIDAS_MEDIA_TYPE_MEDIA_GLOSSY },
    { MEDIATYPE_HEAVY_GLOSSY, MIDAS_MEDIA_TYPE_MEDIA_GLOSSY },
    { MEDIATYPE_CARD_GLOSSY, MIDAS_MEDIA_TYPE_MEDIA_GLOSSY },
    { MEDIATYPE_MIDWEIGHT_GLOSSY, MIDAS_MEDIA_TYPE_MEDIA_GLOSSY },
    { MEDIATYPE_TRANSPARENCY, MIDAS_MEDIA_TYPE_MEDIA_TRANSPARENT },
};


error_type_t map_media_type_to_midas_media_type( mediatype_t media_type, uint32_t * midas_media_type )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_media_type )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( media_type_to_midas_media_type_table ); table_index++ )
        {
            if ( media_type == media_type_to_midas_media_type_table[table_index].media_type )
            {
                *midas_media_type = media_type_to_midas_media_type_table[table_index].midas_media_type;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_media_type = MIDAS_MEDIA_TYPE_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif


#ifdef HAVE_ETHERNET
/** \brief Contains the lookup information for the MIDAS network status enum */
typedef struct network_status_to_midas_network_status_s
{
    eth_mac_link_status_t network_status; /*!< The network status code */

    uint32_t midas_network_status; /*!< The MIDAS network status code */

} network_status_to_midas_network_status_t;


/** \brief lookup table for the network status to MIDAS network status */
static network_status_to_midas_network_status_t network_status_to_midas_network_status_table[] = {
    { ETH_MAC_LINK_STATUS_10_HALF, MIDAS_NETWORK_STATUS_CONNECTED },
    { ETH_MAC_LINK_STATUS_10_FULL, MIDAS_NETWORK_STATUS_CONNECTED },
    { ETH_MAC_LINK_STATUS_100_HALF, MIDAS_NETWORK_STATUS_CONNECTED },
    { ETH_MAC_LINK_STATUS_100_FULL, MIDAS_NETWORK_STATUS_CONNECTED },
    { ETH_MAC_LINK_STATUS_1G_HALF, MIDAS_NETWORK_STATUS_CONNECTED },
    { ETH_MAC_LINK_STATUS_1G_FULL, MIDAS_NETWORK_STATUS_CONNECTED },
    { ETH_MAC_LINK_STATUS_DOWN, MIDAS_NETWORK_STATUS_DISCONNECTED },
};


static error_type_t map_network_status_to_midas_network_status( eth_mac_link_status_t network_status, uint32_t * midas_network_status )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_network_status )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( network_status_to_midas_network_status_table ); table_index++ )
        {
            if ( network_status == network_status_to_midas_network_status_table[table_index].network_status )
            {
                *midas_network_status = network_status_to_midas_network_status_table[table_index].midas_network_status;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_network_status = MIDAS_NETWORK_STATUS_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif

/** \brief Contains the lookup information for the MIDAS supply position enum */
typedef struct supply_position_to_midas_supply_position_s
{
    uint32_t supply_position; /*!< The supply position code */

    uint32_t midas_supply_position; /*!< The MIDAS supply position code */

} supply_position_to_midas_supply_position_t;


/** \brief lookup table for the supply position to MIDAS supply position */
static supply_position_to_midas_supply_position_t supply_position_to_midas_supply_position_table[] = {
    { 0, MIDAS_SUPPLY_POSITION_SUPPLY_POSITION_1 },
    { 1, MIDAS_SUPPLY_POSITION_SUPPLY_POSITION_2 },
    { 2, MIDAS_SUPPLY_POSITION_SUPPLY_POSITION_3 },
    { 3, MIDAS_SUPPLY_POSITION_SUPPLY_POSITION_4 },
    { 4, MIDAS_SUPPLY_POSITION_SUPPLY_POSITION_5 },
    { 5, MIDAS_SUPPLY_POSITION_SUPPLY_POSITION_6 }
};


static error_type_t map_supply_position_to_midas_supply_position( uint32_t supply_position, uint32_t * midas_supply_position )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == midas_supply_position )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( supply_position_to_midas_supply_position_table ); table_index++ )
        {
            if ( supply_position == supply_position_to_midas_supply_position_table[table_index].supply_position )
            {
                *midas_supply_position = supply_position_to_midas_supply_position_table[table_index].midas_supply_position;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *midas_supply_position = MIDAS_SUPPLY_POSITION_NULL;
            e_res = FAIL;
        }
    }

    return e_res;
}


error_type_t map_midas_language_code_string_to_language_code( char * midas_language_code_string, string_mgr_language_t * code )
{
    error_type_t e_res;

    e_res = OK;

    if ( ( NULL == midas_language_code_string ) || ( NULL == code ) )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( language_code_to_midas_language_code_string_table ); table_index++ )
        {
            if ( strncmp( midas_language_code_string,
                        language_code_to_midas_language_code_string_table[table_index].midas_language_code,
                        2 ) == 0 )
            {
                *code = language_code_to_midas_language_code_string_table[table_index].code;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *code = LANG_ENGLISH;
            e_res = FAIL;
        }
    }

    return e_res;
}


#ifdef HAVE_ETHERNET
static error_type_t map_midas_network_speed_string_to_network_speed( uint32_t midas_network_speed_code, eth_mac_link_status_t * network_speed )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == network_speed )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( network_speed_to_midas_network_speed_code_table ); table_index++ )
        {
            if ( midas_network_speed_code == network_speed_to_midas_network_speed_code_table[table_index].midas_network_speed_code )
            {
                *network_speed = network_speed_to_midas_network_speed_code_table[table_index].network_speed;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *network_speed = ETH_MAC_LINK_STATUS_DOWN;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif


#ifdef HAVE_PRINT_SUPPORT
error_type_t map_midas_media_source_to_media_source( uint32_t midas_media_source,  INPUTTYPE * media_source )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == media_source )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( media_source_to_midas_media_source_table ); table_index++ )
        {
            if ( midas_media_source == media_source_to_midas_media_source_table[table_index].midas_media_source )
            {
                *media_source = media_source_to_midas_media_source_table[table_index].media_source;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *media_source = INPUT_AUTO;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif


#ifdef HAVE_PAPER
error_type_t map_midas_media_type_to_media_type( uint32_t midas_media_type, mediatype_t * media_type )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == media_type )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( media_type_to_midas_media_type_table ); table_index++ )
        {
            if ( midas_media_type == media_type_to_midas_media_type_table[table_index].midas_media_type )
            {
                *media_type = media_type_to_midas_media_type_table[table_index].media_type;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *media_type = MEDIATYPE_ANY;
            e_res = FAIL;
        }
    }

    return e_res;
}


error_type_t map_midas_media_size_to_media_size( uint32_t midas_media_size, mediasize_t * media_size )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == media_size )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( media_size_to_midas_media_size_table ); table_index++ )
        {
            if ( midas_media_size == media_size_to_midas_media_size_table[table_index].midas_media_size )
            {
                *media_size = media_size_to_midas_media_size_table[table_index].media_size;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *media_size = MEDIASIZE_LETTER;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif

