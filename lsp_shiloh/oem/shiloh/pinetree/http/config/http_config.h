/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file http_config.h
 *
 * \brief This file is the public configuration API for the HTTP module.
 * This file should only be included by the HTTP module.
 * 
 **/

#ifndef INC_HTTP_CONFIG_H
#define INC_HTTP_CONFIG_H

#include "oid_api.h"
#include "error_types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
// HACK- NEEDS TO BE REMOVER WHEN CPU cpu_get_dcache_line_size function is ported
#define CACHE_LINE_SIZE 32
#define cpu_get_dcache_line_size() CACHE_LINE_SIZE
#endif

/** \brief The maximum length of the xml version string */
#define XML_VERSION_STRING_LEN 10

/** \brief The maximum length of the xml version string */
#define ORDER_SUPPLIES_LINK_LEN 256 

/* \brief Authentication Realm for passwords */
#define HTTP_BASIC_REALM    "Networking"

/* \brief Server Description */
#define HTTP_SERVER_DESCRIPTION "SindohPrinter"

#define HTTP_SOCKET_TCP_80      0x01
#define HTTP_SOCKET_TCP_443     0x02

/* Index for 'special' consumable (extra black, infra-red...)*/
#define CUSTOM_CONSUMABLE 4

/** \brief The action the HTTP/xml parser should take when it encounters an XML
 * tag or data */
typedef enum http_xml_to_oid_lut_action_e
{
    HTTP_XML_TO_OID_LUT_NO_ACTION, /*!< When the HTTP parser/xml parser reaches
                                     a LUT entry with this enum it should not
                                    take any action on the data or tag */

    HTTP_XML_TO_OID_LUT_ACTION, /*!< When the HTTP parser/xml parser reaches
                                     a LUT entry with this enum it should
                                    use the data or tag to write an OID */

} http_xml_to_oid_lut_action_t;


typedef struct
{
    char *                          xml_string; 
    http_xml_to_oid_lut_action_t    action;
    oid_t                           oid;
    uint32_t                        index;
} http_xml_to_oid_lut_t;


/** \brief Enumeration for the socket protocol types */
typedef enum http_net_socket_protocol_e
{
    HTTP_NET_SOCKET_UDP, /*!< UDP socket enumeration */
    HTTP_NET_SOCKET_TCP, /*!< TCP socket enumeration */
} http_net_socket_protocol_t;


/** \brief The HTTP socket configuration structure */
typedef struct 
{
    uint32_t                    port_number;
    http_net_socket_protocol_t  socket_type;
} http_socket_config_t;


/** \brief The HTTP module's configuration structure */
typedef struct http_config_s
{
    char xml_version_string[XML_VERSION_STRING_LEN]; /*!< The XML protocol
                                                       version for this
                                                       product */

    char order_supplies_link[ORDER_SUPPLIES_LINK_LEN]; /*!< The consumables
                                                         supplies link */

    http_xml_to_oid_lut_t * normal_table; /*!< A lookup table for normal XML tags
                                          that contain OID data */

    uint32_t normal_table_size; /*!< The number of elements in the
                                  normal table */

    http_xml_to_oid_lut_t * tag_wout_values_table; /*!< A lookup table for 
                                                     xml tags without values */
                                                     

    uint32_t tag_wout_values_table_size; /*!< The number of elements in the
                                           tag without values table */

    http_xml_to_oid_lut_t * create_table; /*!< A lookup table for xml tags
                                            that contain data that will cause
                                            something in the system to be
                                            created, as opposed to simply
                                            changing a value */

    uint32_t create_table_size; /*!< The number of elements in the
                                  create table */

	char * default_root_page; /*!< The default page to direct a user
									 to when accessing the root server folder */

    uint32_t max_active_http_connections; /*!< The maximum number of HTTP connections that the 
                                                web server will allow to be open before it starts 
                                                to shut down old connections*/

    http_socket_config_t *socket_table; /*!< A structure that defines what port numbers should
                                                be opened by default by the web server */

    uint32_t socket_table_size; /*!< Number of entries in socket_table */

    uint32_t udp_read_buffer_size;   /*!< Buffer size to allocate for incoming UDP packets */

    uint32_t tcp_read_buffer_size;   /*!< Buffer size to allocate for incoming TCP packets */

} http_config_t;


/**
 *
 * \brief Retrieve the HTTP module's configuration structure
 *
 * \param config A pointer to a buffer large enough to hold an http_config_t
 * structure (caller allocated).
 *
 * \return error_type_t OK upon success
 * FAIL otherwise
 *
 */
error_type_t http_config_get( http_config_t * config );


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
error_type_t http_translate_oid_to_xml_protocol( oid_t oid, uint32_t index, char * output_string, uint32_t maximum_string_size );


/**
 *
 * \brief Translate an OID string for a particular OID to its protocol
 * specific value.  For example, a particular product's HTML pages might want
 * the memory size converted from the OID string value in bytes to a value in
 * megabytes.
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
error_type_t http_translate_oid_to_html_protocol( oid_t oid, uint32_t index, char * output_string, uint32_t maximum_string_size );


/**
 *
 * \brief Translate a XML protocol string to it's OID string for a particular
 * OID.  The function also calls the oid_set_string() function to set the
 * new value.  For example, an XML protocol might want give the A4 paper
 * type as "206001" and it would be converted to the OID string value of: "A4"
 *
 * \param oid A valid oid_t from oid_api.h
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
error_type_t http_translate_xml_protocol_to_oid( oid_t oid, uint32_t index, char * xml_string, uint32_t xml_string_len );

/**
 *
 * \brief Translate a HTML protocol string to it's OID string 
 * for a particular OID.  The function also calls the oid set 
 * function (for example: oid_set_string for strings ) to set 
 * the new value. For example, an XML protocol might want give 
 * the A4 paper type as "206001" and it would be converted to 
 * the OID string value of: "A4" 
 *
 * \param oid A valid oid_t from oid_api.h
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
error_type_t http_translate_html_protocol_to_oid( oid_t oid, uint32_t index, char * xml_string, uint32_t xml_string_len );


#include "string_mgr_api.h"
/**
 * \brief Map the MIDAS ISO language string to the system language code 
 *
 * \param midas_language_code_string The MIDAS ISO language string
 *
 * \param code[out] A pointer to a location large enough to hold a
 * valid string_mgr_language_t enum
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 * This function prototype is temporary located in this file, so we do not
 * have to add a new file TODO add this prototype to a new file called
 * http_translate.h
 *
 */
error_type_t map_midas_language_code_string_to_language_code( char * midas_language_code_string, string_mgr_language_t * code );


/**
 * \brief Map the system language code to the MIDAS ISO language string 
 *
 * \param code A valid string_mgr_language_t enum
 *
 * \param midas_language_code_string The MIDAS ISO language string
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 * This function prototype is temporary located in this file, so we do not
 * have to add a new file TODO add this prototype to a new file called
 * http_translate.h
 *
 */
error_type_t map_language_code_to_midas_language_code_string( string_mgr_language_t code, char ** midas_language_code_string );


#ifdef HAVE_ETHERNET
#include "eth_mac_api.h"
/**
 * \brief Map the network speed to the MIDAS network speed code 
 *
 * \param network_speed A valid network speed
 *
 * \param midas_network_speed_code The MIDAS network speed code 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t map_network_speed_to_midas_network_speed_code( eth_mac_link_status_t network_speed, uint32_t * midas_network_speed_code );
#endif


#ifdef HAVE_PAPER
#include "paper_size_config.h"
/**
 * \brief Map the MIDAS media size code to the system media size 
 *
 * \param midas_media_size A valid MIDAS media size code
 *
 * \param media_size A pointer to a location large enough to hold a 
 * mediasize_t enum 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t map_midas_media_size_to_media_size( uint32_t midas_media_size, mediasize_t * media_size );


#include "paper_types_config.h"
/**
 * \brief Map the MIDAS media type code to the system media type 
 *
 * \param midas_media_type A valid MIDAS media type code
 *
 * \param media_type A pointer to a location large enough to hold a 
 * mediatype_t enum 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t map_midas_media_type_to_media_type( uint32_t midas_media_type,  mediatype_t * media_type );
#endif

#ifdef HAVE_PRINT_SUPPORT
#include "agprint.h"
/**
 * \brief Map the MIDAS media source code to the system media source 
 *
 * \param midas_media_source A valid MIDAS media source code
 *
 * \param media_source A pointer to a location large enough to hold a 
 * INPUTTYPE enum 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t map_midas_media_source_to_media_source( uint32_t midas_media_source,  INPUTTYPE * media_source );
#endif


#ifdef HAVE_PAPER
/**
 * \brief Map the media size to the MIDAS media size code 
 *
 * \param media_size A valid mediasize_t enum
 *
 * \param midas_media_size A pointer to a location large enough to hold the
 * MIDAS media size 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t map_media_size_to_midas_media_size( mediasize_t media_size, uint32_t * midas_media_size );
#endif


#ifdef HAVE_PRINT_SUPPORT
/**
 * \brief Map the media source to the MIDAS media source code 
 *
 * \param media_source A valid INPUTTYPE enum
 *
 * \param midas_media_source A pointer to a location large enough to hold the
 * MIDAS media source 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t map_media_source_to_midas_media_source( INPUTTYPE media_source, uint32_t * midas_media_source );
#endif


#ifdef HAVE_PAPER
/**
 * \brief Map the media type to the MIDAS media type code 
 *
 * \param media_type A valid mediatype_t enum
 *
 * \param midas_media_type A pointer to a location large enough to hold the
 * MIDAS media type 
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t map_media_type_to_midas_media_type( mediatype_t media_type, uint32_t * midas_media_type );
#endif



#ifdef __cplusplus
}
#endif

#endif

