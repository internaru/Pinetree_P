/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_feature_support.c
 *
 * \brief This file implements the OID back-end functions for
 * the feature support API module OIDs
 *
 **/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "lassert.h"
#include "engPaperHandlingAPI.h"

/**
 * 
 * \brief The backend oid feature support module function for
 * retreiving integer data associated with an OID.
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
 * FAIL                   - bad oid, other error5
 */
static error_type_t oid_module_feature_support_get_int( oid_t oid,
                                                        uint32_t index,
                                                        uint32_t * value)
{
   error_type_t e_res = OK;

   switch (oid)
   {
   case OID_NETWORK_SUPPORT:
      {
#ifdef HAVE_NETWORK
         *value = 1;
#else
         *value = 0;
#endif
         break;
      }
   case OID_PRINT_HAVE_PCL:
      {
#ifdef HAVE_PCL
         *value = 1;
#else
         *value = 0;
#endif
         break;
      }
   case OID_WIFI_INFT_SUPPORT:
      {
#ifdef HAVE_WLAN
         *value = 1;
#else
         *value = 0;
#endif
         break;
      }
   case OID_PRINT_HAVE_COLOR:
      {
          if(index > 0)
          {
              e_res = OID_ERROR_INDEX_INVALID;
              break;
          }
#ifdef COLOR_ENG
         *value = 1;
#else
         *value = 0;
#endif
         break;
      }
   case OID_FAX_SUPPORT:
      {
#ifdef HAVE_FAX_SUPPORT
         *value = 1;
#else
         *value = 0;
#endif
         break;
      }
   case OID_DUPLEXER_SUPPORT:
      {
          if(index > 0)
          {
              e_res = OID_ERROR_INDEX_INVALID;
              break;
          }

          *value = EngGetHaveAutoDuplexer() ? 1 : 0;
          break;
      }
   case OID_SCAN_SUPPORT:
      {
          if(index > 0)
          {
              e_res = OID_ERROR_INDEX_INVALID;
              break;
          }
#ifdef HAVE_SCAN_SUPPORT
         *value = 1;
#else
         *value = 0;
#endif
         break;
      }
   case OID_NETWORK_UAP_SUPPORT:
      {
#ifdef UAP_SUPPORT
         *value = 1;
#else
         *value = 0;
#endif
         break;
      }
   case OID_RTC_SUPPORT:
#ifdef HAVE_RTC
      *value = 1;
#else
      *value = 0;
#endif
      break;

   case OID_PRINT_COLLATE_SUPPORT:
       if(index > 0)
       {
           e_res = OID_ERROR_INDEX_INVALID;
           break;
       }
      *value = 0;
      break;

   case OID_PRINT_BIND_SUPPORT:
       if(index > 0)
       {
           e_res = OID_ERROR_INDEX_INVALID;
           break;
       }
      *value = 0;
      break;

   case OID_PRINT_SORT_SUPPORT:
      *value = 0;
      break;

   case OID_PRINT_STAPLE_SUPPORT:
      *value = 0;
      break;

   case OID_ETHERNET_SUPPORT:
#ifdef HAVE_ETHERNET
      *value = 1;
#else
      *value = 0;
#endif 
      break;

   case OID_NETWORK_EXTERNAL_INTERFACE:
#if defined( HAVE_ETHERNET ) || defined ( HAVE_WLAN )
      *value = 1;
#else
      *value = 0;
#endif
      break;

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
 * \brief The backend oid feature support module function for 
 *        retreiving string data associated with an OID.
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
 * * OID_ERROR_VALUE_CAPPED  - buffer of size 'oid_len' was too small to fit the entire 
 *                             string. String has been truncated to fit the in buffer.
 * * FAIL                    - bad oid, other error
 *
 */
static error_type_t oid_module_feature_support_get_string( oid_t    oid,
                                                           uint32_t oid_index,
                                                           char     *oid_value,
                                                           uint32_t oid_len )
{
   error_type_t e_res = OK;

   switch (oid)
   {
   case OID_PRINT_PUNCH_SUPPORT:
      // The punch is only one character long. Make sure we have that.
      ASSERT(oid_len > 0);
      
      // This is the value for no punch supported from the RFC 2911 specification.
      oid_value[0] = '3';
      break;

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
 * \brief Registers each feature support API OID with the oid
 * controller
 *
 */
void oid_register_module_feature_support( void )
{
    error_type_t e_res;

    e_res = oid_register_callbacks( OID_NETWORK_SUPPORT,
                                    oid_module_feature_support_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PRINT_HAVE_PCL,
                                    oid_module_feature_support_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INFT_SUPPORT ,
                                    oid_module_feature_support_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_PRINT_HAVE_COLOR,
                                    oid_module_feature_support_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks(  OID_FAX_SUPPORT,
                                     oid_module_feature_support_get_int,
                                     oid_module_null_set,
                                     oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks(  OID_DUPLEXER_SUPPORT,
                                     oid_module_feature_support_get_int,
                                     oid_module_null_set,
                                     oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks(  OID_RTC_SUPPORT,
                                     oid_module_feature_support_get_int,
                                     oid_module_null_set,
                                     oid_module_null_test );
    ASSERT( FAIL != e_res );
 
    e_res = oid_register_callbacks( OID_ETHERNET_SUPPORT,
                                    oid_module_feature_support_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( e_res == OK );

    e_res = oid_register_callbacks( OID_NETWORK_EXTERNAL_INTERFACE,
                                    oid_module_feature_support_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( e_res == OK );

    e_res = oid_register_callbacks(  OID_SCAN_SUPPORT,
                                     oid_module_feature_support_get_int,
                                     oid_module_null_set,
                                     oid_module_null_test );
    ASSERT( FAIL != e_res );
 
    e_res = oid_register_callbacks(  OID_NETWORK_UAP_SUPPORT,                       
                                     oid_module_feature_support_get_int,     
                                     oid_module_null_set,                    
                                     oid_module_null_test );                 
    ASSERT( FAIL != e_res );


    e_res = oid_register_callbacks(  OID_PRINT_COLLATE_SUPPORT,                       
                                     oid_module_feature_support_get_int,     
                                     oid_module_null_set,                    
                                     oid_module_null_test );                 
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks(  OID_PRINT_BIND_SUPPORT,                       
                                     oid_module_feature_support_get_int,     
                                     oid_module_null_set,                    
                                     oid_module_null_test );                 
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks(  OID_PRINT_SORT_SUPPORT,                       
                                     oid_module_feature_support_get_int,     
                                     oid_module_null_set,                    
                                     oid_module_null_test );                 
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks(  OID_PRINT_STAPLE_SUPPORT,                       
                                     oid_module_feature_support_get_int,     
                                     oid_module_null_set,                    
                                     oid_module_null_test );                 
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks(  OID_PRINT_PUNCH_SUPPORT,                       
                                     oid_module_feature_support_get_string,     
                                     oid_module_null_set,                    
                                     oid_module_null_test );                 
    ASSERT( FAIL != e_res );  

}
