/*
 * ============================================================================
 * Copyright (c) 2009-2010,2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file oid_module_platform.c
 *
 * \brief This file implements the OID back-end functions for the
 * platform API module OIDs
 *
 **/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "platform_api.h"
#include "platform.h"
#include "utils.h"
#ifdef HAVE_RTC
#include "rtc_api.h"
#endif

#include "location_api.h"

#include "dprintf.h"
#include "lassert.h"
#include <string.h>
#include "uuid.h"
#include "nvram_api.h"

static error_type_t oid_module_platform_get( oid_t oid,
                                             uint32_t index,
                                             char * data_ptr,
                                             uint32_t max_string_len_in_bytes )
{
    error_type_t    e_res;

    switch ( oid )
    {
        case OID_PLATFORM_PRINT_NAME_STRING:
        {
            char * print_name;

            if(index > 0)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                break;
            }

            e_res = platvars_get_prtnamestr( &print_name );

            if ( OK == e_res )
            {
                strncpy( data_ptr, print_name, max_string_len_in_bytes );
                data_ptr[ max_string_len_in_bytes -1 ] = '\0';
            }

            break;
        }

        case OID_PLATFORM_SKU:
        {
            char * print_name;
            e_res = platvars_get_usb_prodstr( &print_name );

            if ( OK == e_res )
            {
                strncpy( data_ptr, print_name, max_string_len_in_bytes );
                data_ptr[ max_string_len_in_bytes -1 ] = '\0';
            }

            break;
        }

        case OID_PLATFORM_PRODUCT_SERIAL_NUM:
        {
            char * name;
            e_res = platvars_get_prod_serialnum( &name );

            if ( OK == e_res )
            {
                strncpy( data_ptr, name, max_string_len_in_bytes );
                data_ptr[ max_string_len_in_bytes -1 ] = '\0';
            }

            break;
        }

        case OID_PLATFORM_FW_VERSION:
        {
            char * name;
            e_res = platvars_get_fw_version( &name );

            if ( OK == e_res )
            {
                strncpy( data_ptr, name, max_string_len_in_bytes );
                data_ptr[ max_string_len_in_bytes -1 ] = '\0';
            }

            break;
        }

        case OID_PLATFORM_UUID:
            e_res = get_device_uuid_string( data_ptr,  max_string_len_in_bytes);
            break;

        case OID_PLATFORM_UUID_BINARY:
        {
            int len = max_string_len_in_bytes;
            e_res = get_device_uuid_binary( data_ptr, &len);
            break;
        }

        case OID_PLATFORM_FORMATTER_SERIAL_NUM:
        {
            char * name;
            e_res = platvars_get_formater_ser_num( &name );

            if ( OK == e_res )
            {
                strncpy( data_ptr, name, max_string_len_in_bytes );
                data_ptr[ max_string_len_in_bytes -1 ] = '\0';
#ifdef __linux__
                if (!strlen(name)) 
                {
                    // fabricate a serial number
                    name = "0123456789";
                    e_res = platvars_set_formater_ser_num( name );
                }
#else
                ASSERT(strlen(name));
#endif

            }

            break;
        }

        #ifdef HAVE_RTC
        case OID_PLATFORM_INSTALL_DATE:
        {
            date_time_t time;
            char *version;

            e_res = plat_get_install_date( &time, &version );

            if ( OK == e_res )
            {
                sprintf( data_ptr,
                         "%u-%02u-%02u",
                         (time.year + 2000),
                         time.month + 1,
                         time.day );
            }

            break;
        }

        case OID_PLATFORM_CURRENT_TIME:
        {
            struct tm current_time;
            
            e_res = rtc_get_time_tm( &current_time );
            sprintf( data_ptr, "%04d-%02d-%02dT%02d:%02d:%02d",
                     1900 + current_time.tm_year,
                     current_time.tm_mon + 1,
                     current_time.tm_mday,
                     current_time.tm_hour,
                     current_time.tm_min,
                     current_time.tm_sec );
            break;
        }
        #endif

    case OID_PLATFORM_1284_STRING:
       {
          char *_1284_str;
          e_res = platvars_get_1284str( &_1284_str );

          if ( OK == e_res )
          {
              // NOTE: the first two bytes of the 1284 string are length bytes, which could
              //       contain a value of zero. It seems that folks coming through this interface
              //       don't care, so strip them off.
              strncpy( data_ptr, &_1284_str[2], max_string_len_in_bytes );
              data_ptr[ max_string_len_in_bytes -1 ] = '\0';
          }

          break;

       }

    case OID_PLATFORM_USB_MFG_STRING:
       {
          char *USB_mfg_str;

          if(index > 0)
          {
              e_res = OID_ERROR_INDEX_INVALID;
              break;
          }

          e_res = platvars_get_usb_mfgstr( &USB_mfg_str );

          if ( OK == e_res )
          {
             strncpy( data_ptr, USB_mfg_str, max_string_len_in_bytes );
             data_ptr[ max_string_len_in_bytes -1 ] = '\0';
          }

          break;
       }

    case OID_PLATFORM_USB_PROD_STRING:
       {
          char *USB_prod_str;
          e_res = platvars_get_1284str( &USB_prod_str );

          if ( OK == e_res )
          {
              strncpy( data_ptr, USB_prod_str, max_string_len_in_bytes );
              data_ptr[ max_string_len_in_bytes -1 ] = '\0';
          }

          break;
       }

    case OID_PLATFORM_BONJOUR_PROD_STRING:
       {

          char     *USB_mfg_str;
          char     *USB_prod_str;
          uint32_t cat_str_len;

          e_res = platvars_get_usb_mfgstr( &USB_mfg_str );

          if ( OK == e_res )
          {
             strncpy( &data_ptr[0], USB_mfg_str, max_string_len_in_bytes );
             data_ptr[ max_string_len_in_bytes -1 ] = '\0';
          }
          else
          {
             break;
          }

          cat_str_len = max_string_len_in_bytes - strlen(USB_mfg_str);
          e_res = platvars_get_1284str( &USB_prod_str );

          if ( OK == e_res )
          {
              strncat( data_ptr, USB_prod_str, cat_str_len );
              data_ptr[ max_string_len_in_bytes -1 ] = '\0';
          }
          else
          {
             break;
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

static error_type_t oid_module_platform_set( oid_t oid,
                                             uint32_t index,
                                             char * data_ptr,
                                             uint32_t max_string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == data_ptr )
    {
        e_res = FAIL;
    }
    if ( max_string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_PLATFORM_RESTORE_DEFAULT:
            {
                nvram_init_type_t value;
                value = NVRAM_LANG_DEFAULTS;
                nvram_reset_all_vars(value);
                break;
            }

            #ifdef HAVE_RTC
            case OID_PLATFORM_CURRENT_TIME:
            {
                char *cur;
                struct tm ctime;
                memset( &ctime, 0x00, sizeof(ctime) );
                cur = data_ptr;
                if ( strlen( cur ) < 19 )
                {
                    e_res = OID_ERROR_BUFFER_TOO_SMALL;
                }
                else
                {
                    ctime.tm_year  = atoi( cur ) - 1900;
                    cur += 5;
                    ctime.tm_mon   = atoi( cur ) - 1;
                    cur += 3;
                    ctime.tm_mday  = atoi( cur );
                    cur += 3;
                    ctime.tm_hour  = atoi( cur );
                    cur += 3;
                    ctime.tm_min   = atoi( cur );
                    cur += 3;
                    ctime.tm_sec   = atoi( cur );

                    rtc_set_time_tm( (const struct tm *)&ctime );
                }
                break;
            }
            #endif // HAVE_RTC

            default:
                e_res = FAIL;
                break;
         }
     }
    return e_res;
}

/**
 *
 * \brief The backend oid platform API module function for
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
static error_type_t oid_module_platform_get_int( oid_t oid,
                                                 uint32_t index,
                                                 uint32_t * value)
{
    error_type_t e_res;

    switch ( oid )
    {
       case OID_PLATFORM_COUNTRY_NAME:

       case OID_PLATFORM_COUNTRY_CODE:
        {
            e_CountryCode code;

            /* get the country code */
            e_res = plat_get_current_country_code( &code );
            if ( OK == e_res )
            {
                *value = ( uint32_t )code;
            }

            break;
        }
        case OID_PLATFORM_IO_TIMEOUT:
            {
                uint16_t ioTimeout;
                e_res = platvars_get_io_timeout(&ioTimeout);
                if( OK == e_res)
                {
                    *value = ( uint32_t )ioTimeout;
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
 * \brief The backend oid platform API module function for
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
static error_type_t oid_module_platform_set_int( oid_t oid,
                                                 uint32_t index,
                                                 uint32_t value)
{
    error_type_t e_res;

    switch ( oid )
    {
        case OID_PLATFORM_IO_TIMEOUT:
            {
                uint16_t ioTimeout;
                ioTimeout = (uint16_t)value;
                e_res = platvars_set_io_timeout( &ioTimeout );
                break;
            }

        case OID_PLATFORM_COUNTRY_NAME:

        case OID_PLATFORM_COUNTRY_CODE:
        {
            e_CountryCode code;
            code = ( e_CountryCode )value;
            e_res = plat_set_current_country_code( code );
            break;
        }


        default:
            e_res = FAIL;
            break;
    }
    return e_res;
}

error_type_t oid_module_platform_test_int( oid_t oid, uint32_t index, uint32_t value)
{
    error_type_t e_res;

    e_res = OK;
    switch ( oid )
    {
        case OID_PLATFORM_IO_TIMEOUT:
            {
                if ( value > MAX_IO_TIMEOUT )
                {
                    e_res = OID_ERROR_VALUE_TOO_BIG;
                }
                else if ( value < MIN_IO_TIMEOUT )
                {
                    e_res = OID_ERROR_VALUE_TOO_SMALL;
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
 * \brief Registers each platform API OID with the oid controller
 *
 */
void oid_register_module_platform( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_PLATFORM_PRINT_NAME_STRING,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_SKU,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_PRODUCT_SERIAL_NUM,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_FW_VERSION,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    
    e_res = oid_register_callbacks(OID_PLATFORM_IO_TIMEOUT,
                                   oid_module_platform_get_int,
                                   oid_module_platform_set_int,
                                   oid_module_platform_test_int );
    ASSERT(FAIL != e_res);

    e_res = oid_register_callbacks( OID_PLATFORM_FORMATTER_SERIAL_NUM,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_COUNTRY_CODE,
                                    oid_module_platform_get_int,
                                    oid_module_platform_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
    e_res = oid_register_callbacks( OID_PLATFORM_COUNTRY_NAME,
                                    oid_module_platform_get_int,
                                    oid_module_platform_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_UUID,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_UUID_BINARY,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_RESTORE_DEFAULT,
                                    oid_module_null_get,
                                    oid_module_platform_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    #ifdef HAVE_RTC
    e_res = oid_register_callbacks( OID_PLATFORM_INSTALL_DATE,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_CURRENT_TIME,
                                    oid_module_platform_get,
                                    oid_module_platform_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
    
    #endif

    e_res = oid_register_callbacks( OID_PLATFORM_1284_STRING,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_USB_MFG_STRING,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );


    e_res = oid_register_callbacks( OID_PLATFORM_USB_PROD_STRING,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PLATFORM_BONJOUR_PROD_STRING,
                                    oid_module_platform_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

}


