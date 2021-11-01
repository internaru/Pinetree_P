/* 
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \brief Internal diagnostics page
 *
 * This file generates an internal diagnostics page that will print out on
 * paper a variety of system infomation to aid in system diagnostics.
 *
 */

#include <string.h>
#include <stdlib.h>
#include "ATypes.h"
#include "lassert.h"
#include "dprintf.h"
#include "logger.h"
#include "internal_page_api.h"
#include "oid_api.h"
#include "rommgrapi.h"
#include "deviceid.h"
#include "data_access_api.h"
#include "string_mgr_api.h"
#ifdef HAVE_NETWORK
#include "net_iface_vars.h"
#include "net_eth_vars.h"
#endif
#include "printvars_api.h"
#ifdef HAVE_EVENT_LOGGER
#include "event_logger_api.h"
#endif

#define IP_DIAG_RES_X 300
#define IP_DIAG_RES_Y 300
#define IP_DIAG_BLOCK_TOP 0
#define IP_DIAG_LEFT_MARGIN_X 50
#define IP_DIAG_LEFT_2ND_MARGIN_X 1350
#define IP_DIAG_PAGE_BLOCK_Y 3300 + IP_DIAG_BLOCK_TOP

#if 0 /* disabled because text rendering is disabled when no fonts are available */
static char header[25]="";
static char network_header[20] = "";
static char system_name_text[30]="";
static char system_name[21];
static char serial_number_text[40]="";
static char serial_number[21];
static char fw_version_text[30]="";
static char fw_version[21];
static char system_memory_text[30]="";
static char system_memory[21];
static char color_best_text[30]="";
static char color_best[21];
static char color_norm_text[30]="";
static char color_norm[21];
static char mono_best_text[30]="";
static char mono_best[21];
static char mono_norm_text[30]="";
static char mono_norm[21];
static char network_status_text[30]="";
static char network_status[21];
static char link_speed_text[30]="";
static char link_speed[21];
static char mac_address_text[40]="";
static char mac_address[21];
static char host_name_text[30]="";
static char host_name[21];
static char dhcp_enabled_text[30]="";
static char dhcp_enabled[21];
static char ip_address_text[30]="";
static char ip_address[21];
static char subnet_mask_text[30]="";
static char subnet_mask[21];
static char default_gateway_text[30]="";
static char default_gateway[21];
static char domain_name_text[30]="";
static char domain_name[21];
static char wins_server_text[30]="";
static char wins_server[21];
#endif

/** \brief The base we want to use for strtoul function */
#define BASE_10 10

/** \brief A helper macro for getting the number of elements in an array */
#define NUM_ELEMENTS_IN_ARRAY( array ) sizeof( array ) / sizeof( array[0] )

#ifdef HAVE_NETWORK
/** \brief Contains the lookup information for the network status string */
typedef struct network_status_to_string_s
{
    net_iface_status_t network_status; /*!< The network status code */

    string_mgr_code_t network_status_string_code; /*!< The network status code string */

} network_status_to_string_t;

/** \brief Contains the lookup information for the network speed string */
typedef struct network_speed_to_string_s
{
    eth_mac_link_status_t network_speed; /*!< The network speed code */

    string_mgr_code_t network_speed_string_code; /*!< The network speed string */

} network_speed_to_string_t;
#endif

static void build_diagnostics_page( internal_page_doc_t * diagnostics_page_doc );
#ifdef HAVE_NETWORK
#if 0 /* build text is currently not supported, need font license */
static error_type_t translate_oid_to_string( oid_t oid, char * output_string, uint32_t maximum_string_size );
static error_type_t map_network_status_to_string( net_iface_status_t network_status, string_mgr_code_t * network_status_string_code );
static error_type_t map_network_speed_to_string( eth_mac_link_status_t network_speed, string_mgr_code_t * network_speed_code );
#endif
#endif
static uint32_t print_logs( internal_page_doc_t *diagnotics_page, uint32_t y_offset );
static uint32_t print_platform_nvm( internal_page_doc_t *diagnostics_page, uint32_t y_offset );


/**
 * \brief Routine to construct the diagnostics page
 * 
 * \param[in,out] diagnostics_page_doc  The configuration page document to build
 * 
 */
static void build_diagnostics_page( internal_page_doc_t * diagnostics_page_doc )
{
    uint32_t    new_y;

    ASSERT( NULL != diagnostics_page_doc );

    // Put in a header.
	#if 0 /* build text is currently not supported, need fonts */
    strcpy( header, string_mgr_get_string( STRING_EWS_DIAG_PAGE, string_mgr_loc_full ) );
    /*ipage_build_text_header( diagnostics_page_doc, 100, 200, header, IPAGE_JUSTIFY_LEFT,
      26, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );*/
    ipage_build_text( diagnostics_page_doc, 100, 200, header, IPAGE_JUSTIFY_LEFT,
            26, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
	#endif

    ipage_build_line( diagnostics_page_doc, 100, 300, 2300, 300, 3, 0xFF);

    // System info
    // Printer name
	#if 0 /* build text is currently not supported, need fonts */
    oid_get_string(OID_PLATFORM_PRINT_NAME_STRING, system_name, 20);
    strcpy( system_name_text, string_mgr_get_string( STRING_EWS_SYSTEM_NAME, string_mgr_loc_full ) );
    strcat( system_name_text, " " );
    strcat( system_name_text, system_name );
    ipage_build_text( diagnostics_page_doc, 100, 400, system_name_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    // Printer serial number
    oid_get_string(OID_PLATFORM_PRODUCT_SERIAL_NUM, serial_number, 20);
    strcpy( serial_number_text, string_mgr_get_string( STRING_EWS_DEV_SERIAL, string_mgr_loc_full ) );
    strcat( serial_number_text, " " );
    strcat( serial_number_text, serial_number );
    ipage_build_text( diagnostics_page_doc, 100, 450, serial_number_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    // Printer FW version
    oid_get_string(OID_PLATFORM_FW_VERSION, fw_version, 20);
    strcpy( fw_version_text, string_mgr_get_string( STRING_EWS_FIRMWARE, string_mgr_loc_full ) );
    strcat( fw_version_text, " " );
    strcat( fw_version_text, fw_version );
    ipage_build_text( diagnostics_page_doc, 100, 500, fw_version_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    // System memory
    oid_get_string(OID_MEMORY_TOTAL_SIZE_IN_MBYTES, system_memory, 20);
    strcpy( system_memory_text, string_mgr_get_string( STRING_EWS_MEMORY, string_mgr_loc_full ) );
    strcat( system_memory_text, " " );
    strcat( system_memory_text, system_memory );
    ipage_build_text( diagnostics_page_doc, 100, 550, system_memory_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );

    // Pages printed info
    // Color best
    oid_get_string(OID_PRINT_NUM_COLOR_BEST, color_best, 20);
    strcpy( color_best_text, string_mgr_get_string( STRING_EWS_COLOR_BEST_QUALITY, string_mgr_loc_full ) );
    strcat( color_best_text, " " );
    strcat( color_best_text, color_best );
    ipage_build_text( diagnostics_page_doc, 1200, 400, color_best_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    // Color norm
    oid_get_string(OID_PRINT_NUM_COLOR_NORM, color_norm, 20);
    strcpy( color_norm_text, string_mgr_get_string( STRING_EWS_COLOR_GOOD_QUALITY, string_mgr_loc_full ) );
    strcat( color_norm_text, " " );
    strcat( color_norm_text, color_norm );
    ipage_build_text( diagnostics_page_doc, 1200, 450, color_norm_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    // Mono best
    oid_get_string(OID_PRINT_NUM_MONO_BEST, mono_best, 20);
    strcpy( mono_best_text, string_mgr_get_string( STRING_EWS_MONO_BEST_QUALITY, string_mgr_loc_full ) );
    strcat( mono_best_text, " " );
    strcat( mono_best_text, mono_best );
    ipage_build_text( diagnostics_page_doc, 1200, 500, mono_best_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    // Mono norm
    oid_get_string(OID_PRINT_NUM_MONO_NORM, mono_norm, 20);
    strcpy( mono_norm_text, string_mgr_get_string( STRING_EWS_MONO_GOOD_QUALITY, string_mgr_loc_full ) );
    strcat( mono_norm_text, " " );
    strcat( mono_norm_text, mono_norm );
    ipage_build_text( diagnostics_page_doc, 1200, 550, mono_norm_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );

    
    strcpy( network_header, string_mgr_get_string( STRING_EWS_NET_SETTINGS, string_mgr_loc_full ) );
    ipage_build_text( diagnostics_page_doc, 800, 650, network_header, IPAGE_JUSTIFY_LEFT,
            14, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
	#endif
    ipage_build_line( diagnostics_page_doc, 100, 700, 2300, 700, 3, 0xFF);

#ifdef HAVE_NETWORK
    // Network info
    // Network status
	#if 0 /* build text is currently not supported, need font license */
    translate_oid_to_string( OID_NETWORK_STATUS, network_status, 20 );
    strcpy( network_status_text, string_mgr_get_string( STRING_EWS_NETWORK_STATUS, string_mgr_loc_full ) );
    strcat( network_status_text, " " );
    strcat( network_status_text, network_status );
    // Link speed
    translate_oid_to_string(OID_NETWORK_LINK_SPEED, link_speed, 20);
    strcpy( link_speed_text, string_mgr_get_string( STRING_EWS_LINK_SPEED, string_mgr_loc_full ) );
    strcat( link_speed_text, " " );
    strcat( link_speed_text, link_speed );
    // MAC address
    oid_get_string(OID_NETWORK_MAC_ADDRESS, mac_address, 20);
    strcpy( mac_address_text, string_mgr_get_string( STRING_EWS_MAC_ADDRESS, string_mgr_loc_full ) );
    strcat( mac_address_text, " " );
    strcat( mac_address_text, mac_address );
    // Host name
    oid_get_string(OID_NETWORK_HOST_NAME, host_name, 20);
    strcpy( host_name_text, string_mgr_get_string( STRING_EWS_HOST, string_mgr_loc_full ) );
    strcat( host_name_text, " " );
    strcat( host_name_text, host_name );
    // DHCP enabled
    translate_oid_to_string(OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG, dhcp_enabled, 20);
    strcpy( dhcp_enabled_text, string_mgr_get_string( STRING_EWS_DHCP_ENABLED, string_mgr_loc_full ) );
    strcat( dhcp_enabled_text, " " );
    strcat( dhcp_enabled_text, dhcp_enabled );
    // IP address
    oid_get_string(OID_NETWORK_IPV4_ADDRESS, ip_address, 20);
    strcpy( ip_address_text, string_mgr_get_string( STRING_EWS_IPV4_ADDRESS, string_mgr_loc_full ) );
    strcat( ip_address_text, " " );
    strcat( ip_address_text, ip_address );
    // Subnet mask
    oid_get_string(OID_NETWORK_IPV4_SUBNET_MASK, subnet_mask, 20);
    strcpy( subnet_mask_text, string_mgr_get_string( STRING_EWS_SUBNET_MASK, string_mgr_loc_full ) );
    strcat( subnet_mask_text, " " );
    strcat( subnet_mask_text, subnet_mask );
    // Default gateway
    oid_get_string(OID_NETWORK_IPV4_DEFAULT_GATEWAY, default_gateway, 20);
    strcpy( default_gateway_text, string_mgr_get_string( STRING_EWS_DEFAULT_GATE, string_mgr_loc_full ) );
    strcat( default_gateway_text, " " );
    strcat( default_gateway_text, default_gateway );
    // Domain name
    oid_get_string(OID_NETWORK_IPV4_DOMAIN_NAME, domain_name, 20);
    strcpy( domain_name_text, string_mgr_get_string( STRING_EWS_DOMAIN_IPV4, string_mgr_loc_full ) );
    strcat( domain_name_text, " " );
    strcat( domain_name_text, domain_name );
    // WINS Server
    oid_get_string(OID_NETWORK_IPV4_WINS_SERVER, wins_server, 20);
    strcpy( wins_server_text, string_mgr_get_string( STRING_EWS_WINS_SERVER, string_mgr_loc_full ) );
    strcat( wins_server_text, " " );
    strcat( wins_server_text, wins_server );
    // Display the network info
    ipage_build_text( diagnostics_page_doc, 100, 800, network_status_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 100, 850, link_speed_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 100, 900, mac_address_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 100, 950, host_name_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 100, 1000, dhcp_enabled_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );

    ipage_build_text( diagnostics_page_doc, 1200, 800, ip_address_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 1200, 850, subnet_mask_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 1200, 900, default_gateway_text,
            IPAGE_JUSTIFY_LEFT, 12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 1200, 950, domain_name_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_text( diagnostics_page_doc, 1200, 1000, wins_server_text, IPAGE_JUSTIFY_LEFT,
            12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
	#endif
	#endif

    new_y = print_logs( diagnostics_page_doc, 1100 );
    print_platform_nvm( diagnostics_page_doc, new_y + 100 );
}


/** 
 * \brief Routine to build and print a diagnostics page
 * 
 * \param[in] copies Number of copies
 * \param[in] dest Destination module
 */
void internalpages_renderer_diagnostics_page( uint32_t copies, MODULE_ID dest )
{
    internal_page_doc_t *diagnostics_page;
    ipage_config_t config;

    ASSERT( copies > 0 );

    diagnostics_page = ipage_create_page( e_CONFIGPAGE );   
    ASSERT( NULL != diagnostics_page );

    // set the resolution and margins
    ipage_get_config(diagnostics_page, &config);
    config.resolution_x = IP_DIAG_RES_X;
    config.resolution_y = IP_DIAG_RES_Y;
    config.left_margin = IP_DIAG_LEFT_MARGIN_X;
    config.left_2nd_margin = IP_DIAG_LEFT_2ND_MARGIN_X;
    config.page_block_y = IP_DIAG_PAGE_BLOCK_Y;
    config.block_top = IP_DIAG_BLOCK_TOP;
    ipage_set_config(diagnostics_page, &config);

    build_diagnostics_page( diagnostics_page );
    
    ipage_print_page( diagnostics_page, copies, dest );
    
    ipage_delete_page( diagnostics_page );
}

#ifdef HAVE_NETWORK
#if 0 /* build text is currently not supported, need font license */
static error_type_t translate_oid_to_string( oid_t oid, char * output_string, uint32_t maximum_string_size )
{
    error_type_t e_res;

    ASSERT( oid != 0 );
    ASSERT( NULL != output_string );

    e_res = OK;

    if ( ( NULL == output_string ) || ( 0 == maximum_string_size ) )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /* get the OID */
        e_res = oid_get_string( oid,
                output_string,
                maximum_string_size );
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_NETWORK_STATUS:
            {
                unsigned int string_size;
				net_iface_status_t network_status;
                string_mgr_code_t network_status_code;
                const char * network_status_string;

                /* convert string to uint32_t */
                network_status = (net_iface_status_t )strtoul(output_string, NULL, BASE_10);

                e_res = map_network_status_to_string( network_status, &network_status_code );

                if ( OK == e_res )
                {
                     network_status_string = string_mgr_get_string( network_status_code, string_mgr_loc_full );

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

                break;
            }

            case OID_NETWORK_LINK_SPEED:
            {
                unsigned int string_size;
                eth_mac_link_status_t network_speed;
                string_mgr_code_t network_speed_code;
                const char * network_speed_string;

                /* convert string */
                network_speed = (eth_mac_link_status_t)strtoul(output_string, NULL, BASE_10);

                e_res = map_network_speed_to_string( network_speed, &network_speed_code );

                if ( OK == e_res )
                {
                    network_speed_string = string_mgr_get_string( network_speed_code, string_mgr_loc_full );
                    if ( NULL == network_speed_string )
                    {
                        e_res = FAIL;
                    }

                    if ( OK == e_res )
                    {
                        if ( '\0' == network_speed_string[0] )
                        {
                            e_res = FAIL;
                        }
                    }
                    e_res = min_n_k_sprintf( output_string,
                            (int)maximum_string_size,
                            &string_size,
                            "%s",
                            network_speed_string );

                    ASSERT( string_size <= maximum_string_size );
                    ASSERT( 0 != string_size );

                    if ( DEBUG_STRING_CAPPED == e_res )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                }

                break;
            }

            case OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG:
            {
                unsigned int string_size;
                const char * dhcp_string;

                /* convert string to uint32_t */
                if( (uint32_t)strtoul(output_string, NULL, BASE_10) == 1 )
                {
                    dhcp_string = string_mgr_get_string( STRING_GEN_YES, string_mgr_loc_full );
                }
                else
                {
                    dhcp_string = string_mgr_get_string( STRING_GEN_NO, string_mgr_loc_full );
                }

                if ( NULL == dhcp_string )
                {
                    e_res = FAIL;
                }

                if ( '\0' == dhcp_string[0] )
                {
                    e_res = FAIL;
                }

                e_res = min_n_k_sprintf( output_string,
                        (int)maximum_string_size,
                        &string_size,
                        "%s",
                        dhcp_string );

                ASSERT( string_size <= maximum_string_size );
                ASSERT( 0 != string_size );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }

                break;
            }

            default:
            {
                /* return the regular OID, do nothing here for the remainder of the OIDS */
                break;
            }
        }
    }

    return e_res;
}
#endif

/** \brief lookup table for the network status to HTML network status */
#if 0 /* build text is currently not supported, need font license */
static network_status_to_string_t network_status_to_string_table[] = {
    { NET_IFACE_STATUS_IPV4_CONFIGURED, STRING_EWS_NETWORK_CONNECTED },
    { NET_IFACE_STATUS_IPV4_DUPLICATE_ADDR, STRING_EWS_NETWORK_ERROR },
    { NET_IFACE_STATUS_IPV4_BAD_MANUAL_CONFIG, STRING_EWS_NETWORK_ERROR },
    { NET_IFACE_STATUS_IPV4_AUTOCONFIG_TIMEOUT, STRING_EWS_NETWORK_ERROR },
};
#endif


#if 0 /* build text is currently not supported, need font license */
static error_type_t map_network_status_to_string( net_iface_status_t network_status, string_mgr_code_t * network_status_string_code )
{
    error_type_t e_res;

    ASSERT( NULL != network_status_string_code );

    e_res = OK;

    if ( NULL == network_status_string_code )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( network_status_to_string_table ); table_index++ )
        {
            if ( network_status == network_status_to_string_table[table_index].network_status )
            {
                *network_status_string_code = network_status_to_string_table[table_index].network_status_string_code;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *network_status_string_code = STRING_EWS_NETWORK_DISCONNECTED;
            e_res = OK;
        }
    }

    return e_res;
}

/** \brief lookup table for the network speed to string */
static network_speed_to_string_t network_speed_to_string_table[] = {
    { NET_ETH_LINK_STATUS_DOWN, STRING_EWS_LINK_SPEED_NO_LINK },
    { NET_ETH_LINK_STATUS_10_HALF, STRING_EWS_LINK_SPEED_10H_OPTION },
    { NET_ETH_LINK_STATUS_10_FULL, STRING_EWS_LINK_SPEED_10F_OPTION },
    { NET_ETH_LINK_STATUS_100_HALF, STRING_EWS_LINK_SPEED_100H_OPTION },
    { NET_ETH_LINK_STATUS_100_FULL, STRING_EWS_LINK_SPEED_100F_OPTION },
    { NET_ETH_LINK_STATUS_1G_HALF, STRING_EWS_LINK_SPEED_1000H_OPTION },
    { NET_ETH_LINK_STATUS_1G_FULL, STRING_EWS_LINK_SPEED_1000F_OPTION },
};

static error_type_t map_network_speed_to_string( eth_mac_link_status_t network_speed, string_mgr_code_t * network_speed_string_code )
{
    error_type_t e_res;

    ASSERT( NULL != network_speed_string_code );

    e_res = OK;

    if ( NULL == network_speed_string_code )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        uint32_t table_index;
        bool found_entry;

        found_entry = false;
        for ( table_index = 0; table_index < NUM_ELEMENTS_IN_ARRAY( network_speed_to_string_table ); table_index++ )
        {
            if ( network_speed == network_speed_to_string_table[table_index].network_speed )
            {
                *network_speed_string_code = network_speed_to_string_table[table_index].network_speed_string_code;
                found_entry = true;
                break;
            }
        }

        /* if we didn't find the entry */
        if ( false == found_entry )
        {
            *network_speed_string_code = STRING_EWS_NETWORK_ERROR;
            e_res = FAIL;
        }
    }

    return e_res;
}
#endif
#endif

static uint32_t print_logs( internal_page_doc_t *diagnostics_page, uint32_t y_offset )
{
	#ifdef HAVE_EVENT_LOGGER
    error_type_t e_res;
    uint32_t i;
    uint32_t num_log_entries;
    uint32_t line_y, line_x;
    event_logger_log_entry_t log_entry;
    char log_num[20]="";
    char num_pages[20]="";
    char log_code[20]="";
    char log_date[20]="";
    char log_dump_header[30] = "";

    ASSERT( NULL != diagnostics_page );

    // System log dump
    strcpy( log_dump_header, string_mgr_get_string( STRING_EWS_EVENT_LOG, 
                string_mgr_loc_full ) );
    ipage_build_text( diagnostics_page, 800, y_offset, log_dump_header, IPAGE_JUSTIFY_LEFT,
            14, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    ipage_build_line( diagnostics_page, 100, y_offset + 50, 2300, y_offset + 50, 3, 0xFF);

    line_y = y_offset + 150;

    e_res = event_logger_get_max_log_entries( &num_log_entries );
    ASSERT( OK == e_res);

    for( i = 0 ; i < num_log_entries ; i++ )
    {
        if( i % 2 )
        {
            // Odd lines
            line_x = 1200;
        }
        else
        {
            // Even lines
            line_x = 100;
            if( i > 0 )
            {
                line_y += 250;
            }
        }

        e_res = event_logger_get_log_entry( i, &log_entry );
        ASSERT( OK == e_res);
        // Log number
        strcpy( log_num, string_mgr_get_string( STRING_EWS_EVENT_LOG, string_mgr_loc_full ) );
        minSprintf( log_num, "%s: %d", log_num, i);
        ipage_build_text( diagnostics_page, line_x, line_y, log_num, IPAGE_JUSTIFY_LEFT,
                12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
        // Pages printed
        strcpy( num_pages, string_mgr_get_string( STRING_EWS_PAGES, string_mgr_loc_full ) );
        minSprintf( num_pages, "%s %d", num_pages, log_entry.page_count);
        ipage_build_text( diagnostics_page, line_x, line_y + 50, num_pages, IPAGE_JUSTIFY_LEFT,
                12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
        // Log code
        strcpy( log_code, string_mgr_get_string( STRING_EWS_EVENT_LOG_ID_HEADING,
                    string_mgr_loc_full ) );
        minSprintf( log_code, "%s: %d", log_code, log_entry.code);
        ipage_build_text( diagnostics_page, line_x, line_y + 100, log_code, IPAGE_JUSTIFY_LEFT,
                12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
        // Log date
		strcpy( log_date, string_mgr_get_string( STRING_EWS_EVENT_LOG_DATE_HEADING,
												 string_mgr_loc_full ) );
		#ifdef HAVE_RTC
        minSprintf( log_date, "%s: %u-%u-%u", log_date, log_entry.time.year + 2000,
                log_entry.time.month, log_entry.time.day );
		#endif

        ipage_build_text( diagnostics_page, line_x, line_y + 150, log_date, IPAGE_JUSTIFY_LEFT,
                12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
    }


    return ( line_y + 150 );

	#else

	return ( y_offset + 150 );

	#endif

}

static uint32_t print_platform_nvm( internal_page_doc_t *diagnostics_page, uint32_t y_offset )
{
    char formatted_data[16];
    int8_t      *nvm_data;
    uint32_t    i, j;
    uint32_t    bytes_read;
    char output_line[80] = "";
	#if 0 /* build text is currently not supported, need fonts */
    char nvm_dump_header[30] = "";
	#endif
    error_type_t e_res;

    ASSERT( NULL != diagnostics_page );

    // Platform memory dump
	#if 0 /* build text is currently not supported, need fonts */
    strcpy( nvm_dump_header, string_mgr_get_string( STRING_EWS_PLAT_NVM_PART, string_mgr_loc_full ) );
    ipage_build_text( diagnostics_page, 700, y_offset, nvm_dump_header, IPAGE_JUSTIFY_LEFT,
            14, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );
	#endif
    //ipage_build_line( diagnostics_page, 100, (y_offset + 50), 2300, (y_offset + 50), 3, 0xFF);

    nvm_data = (int8_t *)RomMgrReadData(IM_CFG_SECTOR, 0, &bytes_read);
    ASSERT( (bytes_read != 0) || (nvm_data != NULL) );


    for(i = 0 ; i < ( bytes_read / 4 ) ; i++ )
    {
        if( (i % 16) == 0 )
        {
            if( i )
            {
                for(j = i-16; j < i; j++)
                {
                    // Print ASCII or "."
                    if( ( nvm_data[j] > 31 ) && ((uint8_t)nvm_data[j] < 128 ) )
                    {
                        e_res = minSprintf( formatted_data, "%c", nvm_data[j] );
                        strcat( output_line, formatted_data );
                    }
                    else
                    {
                        strcat( output_line, "." );
                    }
                }
                ipage_build_text( diagnostics_page, 100, ( y_offset + 50 + ( i * 4 ) ),
                        output_line, IPAGE_JUSTIFY_LEFT, 12, LANG_ENGLISH, 
                        COLOR_RGB_BLACK, IPAGE_NOT_AND );
            }
            // Start a new line
            output_line[0] = '\0';
            // Row address
            e_res = minSprintf( formatted_data, "%04.4x ", i);
            strcat( output_line, formatted_data );

        }
        if( ( i % 8 ) == 0 )
        {
            // Seperator between every 8 bytes
            strcat( output_line, "- " );
        }
        // Actual data in hex
        e_res = minSprintf( formatted_data, "%02.2x ", (uint8_t)(nvm_data[i]) );
        strcat( output_line, formatted_data );
    }
    // Print last ASCII row
    for( j = i - 16 ; j < i ; j++ )
    {
        // Print ASCII or "."
        if( ( nvm_data[j] > 31 ) && ((uint8_t)nvm_data[j] < 128 ) )
        {
            e_res = minSprintf( formatted_data, "%c", nvm_data[j] );
            strcat( output_line, formatted_data );
        }
        else
        {
            strcat( output_line, "." );
        }
    }
    // End of memory dump
    ipage_build_text( diagnostics_page, 100, ( y_offset + 50 + ( i * 4 ) ), output_line,
            IPAGE_JUSTIFY_LEFT, 12, LANG_ENGLISH, COLOR_RGB_BLACK, IPAGE_NOT_AND );

    return ( y_offset + 50 + ( i * 4 ) );
}
