/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_mdns_oids.c
 *
 * \brief This file implements the OID back-end functions for the
 * mDNS bonjour service data.
 *
 **/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#include "oid_api.h"
#include "oid_producer_api.h"
#include "platform_api.h"
#include "net_api.h"
#include "net_iface_vars.h"
#include "net_iface_api.h"

#define DBG_PRFX "net.mdns.oid: "
#define DBG_ERR_ON
#include "net_debug.h" // DBG_ family macros
#include "net_mdns_vars.h"

#define F_STRING_LEN NET_MDNS_SERVICE_NAME_SIZE

/**
 * 
 * \brief The backend oid Network API module function for retreiving
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
static error_type_t oid_get_mdns_string( oid_t oid,
                                            uint32_t if_index,
                                            char * data_ptr,
                                            uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = NET_IFACE_OK;
    unsigned int string_size;

    switch ( oid )
    {

        case OID_NETWORK_BONJOUR_SERVICE_NAME:
        {
            char mdns_name[NET_MDNS_SERVICE_NAME_SIZE];
            int max_string_len;

            max_string_len = (int)F_STRING_LEN;

            e_res = net_mdns_get_var_service_name(0, mdns_name, max_string_len );

            if ( F_STRING_LEN > max_string_len_in_bytes )
            {
                max_string_len = (int)max_string_len_in_bytes;
            }

            if ( NET_IFACE_OK == e_res )
            {
                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len,
                        &string_size,
                        "%s",
                        mdns_name );

                ASSERT( string_size <= max_string_len_in_bytes );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }
            }

            break;
        }

        case OID_NETWORK_BONJOUR_DOMAIN_NAME:
        {
            char hostname[NET_IFACE_HOSTNAME_SIZE];

            e_res = net_iface_get_var_hostname(0, hostname, NET_IFACE_HOSTNAME_SIZE);

            if ( NET_IFACE_OK == e_res )
            {
                // BONJOUR/mDNS domain name is hostname + ".local."
                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len_in_bytes,
                        &string_size,
                        "%s.local.",
                        hostname );

                ASSERT( string_size <= max_string_len_in_bytes );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }
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
 * \brief The backend oid module function for the network module set
 * functionality.
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
static error_type_t oid_set_mdns_string( oid_t oid,
        uint32_t if_index,
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

            case OID_NETWORK_BONJOUR_SERVICE_NAME:
            {
                e_res = net_mdns_set_var_service_name(0, data );

                if ( OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

                break;
            } 

            case OID_NETWORK_BONJOUR_DOMAIN_NAME:
            {
#if 0
                NETWORKINFORMATION network_info;
                char str_storage[OID_MAX_STRING_SIZE_IN_BYTES];
                char * seperator = ".";
                char * next_str;
                char * cur_str;
                uint32_t new_string_len;

                /* NOTE: proper bonjour domain name looks like:
                 * "<name>.local."  You can see from the "get"
                 * the name is just the domain name with ".local."
                 * added onto the end.  So here we just parse
                 * the data string and then change the domain
                 * name. */

                /* Get the current network info before changing only one item */
                e_res = network_get_var_networkinfo( &network_info );

                if ( OK == e_res )
                {
                    /* move the data string to temporary storage for
                     * manipulation */
                    strncpy( str_storage, data, string_len_in_bytes + 1 );
                    /* split up the string by the "." symbol */
                    cur_str = strtok_r( str_storage, seperator, &next_str );

                    /* make sure we got something */
                    if ( NULL == cur_str )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                if ( OK == e_res )
                {
                    /* now, copy the hostname into the network info */
                    new_string_len = strnlen( cur_str, string_len_in_bytes + 1 );
                    strncpy( network_info.HostName, cur_str, new_string_len + 1 );

                    /* make sure ".local." is next -- properly formed */
                    cur_str = strtok_r( NULL, seperator, &next_str );

                    if ( NULL == cur_str )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }

                }

                if ( OK == e_res )
                {
                    /* make sure ".local." is next -- properly formed */
                    if ( strncmp( cur_str, "local", 5 ) != 0 )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                if ( OK == e_res )
                {
                    /* now we should be at the last "." so strtok should
                     * return NULL, if it doesn't then the name is 
                     * mal-formed */
                    cur_str = strtok_r( NULL, seperator, &next_str );

                    if ( NULL != cur_str )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                if ( OK == e_res )
                {
                    /* now set the network info with the domain name
                     * changed, then we are done */
                    e_res = network_set_var_networkinfo( &network_info );

                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }
#endif

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
 * \brief The backend oid network module function for retreiving
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
static error_type_t oid_get_mdns_u32( oid_t oid,
                                      uint32_t if_index,
                                      uint32_t * value)
{
    error_type_t e_res;
    e_res = OK;

    switch ( oid )
    {

        case OID_NETWORK_BONJOUR_ENABLED:
        {
            bool mdns_enabled;

            e_res = net_mdns_get_var_enabled(0, &mdns_enabled );

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)mdns_enabled;
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
 * \brief The backend oid network API module function for 
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
static error_type_t oid_set_mdns_u32( oid_t oid,
                                         uint32_t if_index,
                                         uint32_t value)
{
    error_type_t e_res;
    e_res = OK;    


    switch ( oid )
    {
            case OID_NETWORK_BONJOUR_ENABLED:
            {
                bool mdns_enabled;

                mdns_enabled = (uint8_t)value;

                /* check range */
                if ( ( 0 != mdns_enabled ) && ( 1 != mdns_enabled ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

                if ( OK == e_res )
                {
                    e_res = net_mdns_set_var_enabled(0, mdns_enabled );
                }

                if ( OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

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
 * \brief Registers each mDNS OID with the oid controller
 *
 */
void oid_register_module_mdns( void )
{
    error_type_t e_res;

    e_res = oid_register_callbacks( OID_NETWORK_BONJOUR_ENABLED,
                                    oid_get_mdns_u32,
                                    oid_set_mdns_u32,
                                    oid_module_null_test );
    DBG_ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_BONJOUR_SERVICE_NAME,
                                    oid_get_mdns_string,
                                    oid_set_mdns_string,
                                    oid_module_null_test );
    DBG_ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_BONJOUR_DOMAIN_NAME,
                                    oid_get_mdns_string,
                                    oid_set_mdns_string,
                                    oid_module_null_test );
    DBG_ASSERT( FAIL != e_res );
}

#ifdef __cplusplus
}
#endif

// eof net_mdns_oids.c
