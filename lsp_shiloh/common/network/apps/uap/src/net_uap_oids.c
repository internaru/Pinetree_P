/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_uap_oids.c
 *
 * \brief This file implements the OID back-end functions for the
 * UAP API OIDs
 *
 **/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "oid_api.h"
#include "oid_producer_api.h"
#include "net_iface_vars.h"

#ifdef __linux__

#include <sys/socket.h>
#include "net_uap_api.h"
#include "net_wifi_api.h"
#include "net_uap_vars.h"
#include "net_uap_api.h"
#include "net_uap_vars.h"

#define DBG_PRFX "net.uap.oid: "
#define DBG_ERR_ON
#include "net_debug.h"

#else // ! __linux__

#include "trsocket.h"
#include "ioutils.h"
#include "platform_api.h"
#include "dprintf.h"
#include "lassert.h"
#include "wifi_intf_vars.h"
#include "wifi_intf_api.h"
#include "dhcpd_vars.h"
#include <pthread.h>

#endif

#include "memAPI.h"
#include <posix_ostools.h>

#define MAX_FILTERED_ADDRS 16


/** \brief The base we want to use for strtoul function */
#define BASE_10 10
/** \brief Number of bytes for a null character */
#define LENGTH_OF_NULL          1
//local functions

#define DHCP_ENTRY_LINE_SIZE 36
/**
 * 
 * \brief The backend oid wifi API module function for retreiving
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
static error_type_t oid_module_uap_get_string( oid_t oid,
                                         uint32_t index,
                                         char * data_ptr,
                                         uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = OK;
    if(index != WIFI_IFACE_ID_STATION && index !=WIFI_IFACE_ID_UAP )
        e_res = OID_ERROR_ILLEGAL_CHARS;
    if(OK == e_res) 
    {
        //revisit index lookup index->if, if->index?? unsigned int active_if = get_active_net_if();
        switch ( oid )
        {
            case OID_UAP_INTF_SSID:
#ifdef __linux__
    // don't enforce return-buffer size here -- revisit
#else
                XASSERT( max_string_len_in_bytes >= MAX_SSID_NAME_LEN, max_string_len_in_bytes );
#endif
                e_res = uap_get_var_ssid(index, (uint8_t*)data_ptr, MAX_SSID_NAME_LEN );
            break;

            default:
                {
                    DBG_ERR("%s(x%x) undefined?\n", __func__, oid);
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
 * \brief The backend oid module function for the wifi module set
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
error_type_t oid_module_uap_set_string( oid_t oid,
                                  uint32_t index,
                                  const char * data,
                                  uint32_t string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    if(index != WIFI_IFACE_ID_STATION && index !=WIFI_IFACE_ID_UAP )
        e_res = OID_ERROR_ILLEGAL_CHARS;


    if ( NULL == data )
    {
        DBG_ERR("%s() err: no buffer\n", __func__);
        e_res = FAIL;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        DBG_ERR("%s() err: len %d > %d\n", __func__, string_len_in_bytes, MAX_SSID_NAME_LEN);
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    if ( OK == e_res )
    {
        //revisit unused unsigned int active_if = index; // get_active_net_if() ?
        switch ( oid )
        {
            case  OID_UAP_INTF_SSID:
            {
                if(string_len_in_bytes > MAX_SSID_NAME_LEN) 
                {
                    DBG_ERR("err: len %d > %d\n", string_len_in_bytes, MAX_SSID_NAME_LEN);
                    e_res= OID_ERROR_VALUE_TOO_BIG;
                    break; // return e_res
                }
                if(OK == e_res) 
                {
                    e_res = uap_set_var_ssid(index, (uint8_t* )data, string_len_in_bytes);
                }
                if(OK != e_res)
                {
                    DBG_ERR("err: ILLEGAL_CHARS '%s'\n", data);
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }
            break;

#if 0 // tbd
            case OID_WIFI_INTF_SSID_PREFIX:
                e_res = wifi_intf_set_var_ssid_prefix(index, (const uint8_t* )data, string_len_in_bytes);
                if(OK != e_res)
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
                break;
#endif // tbd

            default:
                DBG_ERR("%s(x%x) undefined?\n", __func__, oid);
                /* bad oid */
                e_res = FAIL;
                break;
        }
    }

    return e_res;
}


/**
 * 
 * \brief The backend oid wifi module function for retreiving
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
static error_type_t oid_module_uap_get_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t * value)
{
    error_type_t e_res = OK;

    if( ( index != WIFI_IFACE_ID_STATION ) && 
        ( index !=WIFI_IFACE_ID_UAP ) )
    {
        e_res = OID_ERROR_ILLEGAL_CHARS;
    }
    if( OK == e_res ) 
    {
        //revisit unused unsigned int active_if = get_active_net_if();
        switch ( oid )
        {
            case OID_UAP_INTF_ENABLED:
            { 
                uint32_t uap_intf_enabled;
                e_res = uap_get_var_is_enabled(index, &uap_intf_enabled);
                if(OK == e_res) 
                {
                    *value = uap_intf_enabled;
                }
                else
                {
                    DBG_ERR("%s(x%x) err %d\n", __func__, (unsigned int)oid, e_res);
                }
                break;
            }

#if 0 //tbd: revisit prefix?
            case OID_UAP_SSID_SUFFIX_LEN:
            {
                uint8_t len;
                len = MAX_SSID_NAME_LEN;
                uint8_t prefix[ MAX_SSID_NAME_LEN ];
                e_res = wifi_intf_get_var_ssid_prefix((uint8_t)index, prefix, len);
                if ( e_res == OK )
                {
                    *value = MAX_SSID_NAME_LEN - strlen( (char*)prefix ) - LENGTH_OF_NULL;
                }
                break; 
            }
#endif
            default:
            {
                DBG_ERR("%s(x%x) undefined?\n", __func__, oid);
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
 * \brief The backend oid wifi API module function for 
 * retrieving data associated with an OID. 
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
static error_type_t oid_module_uap_set_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t value)
{
    error_type_t e_res;
    e_res = OK;
    switch ( oid )
    {
        case OID_UAP_INTF_ENABLED:
        {
            e_res = uap_set_var_is_enabled(index, value); // cast
            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        default:
            DBG_ERR("%s(x%x) undefined?\n", __func__, oid);
            e_res = FAIL;
            break;
    }
    return e_res;
}


/**
 *
 * \brief Registers each Network API OID with the oid controller
 *
 */
void oid_register_module_uap( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_UAP_INTF_ENABLED,
                                    oid_module_uap_get_int,
                                    oid_module_uap_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_UAP_INTF_SSID,
                                    oid_module_uap_get_string,
                                    oid_module_uap_set_string,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

}

#ifdef __cplusplus
}
#endif

//eof
