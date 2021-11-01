/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

//=========================================================================================
// Includes
//=========================================================================================
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#include "sm_job_api.h"
#include "memAPI.h"
#include "debug.h"
#include "ipp_api.h"
#include "logger.h"
#include "net_logger.h"

#include "ipp.h"
#include "ipp_types.h"
#include "ipp_const.h"
#include "ipp_attribute.h"
#include "ipp_dbg.h"
#include "ipp_cntxt.h"
#include "ipp_request.h"

#include "ipp_printer_requested_attr.h"
#include "ipp_handle_attr_reqs.h"

//=========================================================================================
// Defines
//=========================================================================================

//=========================================================================================
// Local variables
//=========================================================================================
static requested_attribute_info_t *print_attr_req_tbl = NULL;

//=========================================================================================
// Local functions
//=========================================================================================
static void _set_default_req_attr_to_be_ret();
static const char *_get_ipp_request_attribute_string(requested_printer_attributes_e tag);

void generate_req_printer_attr_tbl()
{
   uint32_t i;
   uint32_t j;
   const char     *req_attr_str    = NULL;
   uint32_t req_attr_str_len = 0;

   print_attr_req_tbl = IPP_MALLOC(NUM_REQ_PRINTER_ATTRIBUTES * 
                                   sizeof(requested_attribute_info_t));
   ASSERT(print_attr_req_tbl);

   for (i = 0; i < NUM_REQ_PRINTER_ATTRIBUTES; ++i)
   {
       req_attr_str = _get_ipp_request_attribute_string(i);

       req_attr_str_len = strlen(req_attr_str);
       print_attr_req_tbl[i].attr_name = IPP_CALLOC(req_attr_str_len + 1, 1);
       ASSERT(print_attr_req_tbl[i].attr_name);

       strncpy(print_attr_req_tbl[i].attr_name, req_attr_str, req_attr_str_len);


       for (j = 0; j < req_attr_str_len; ++j)
       {
          if (print_attr_req_tbl[i].attr_name[j] == '_')
          {
             print_attr_req_tbl[i].attr_name[j] = '-';
             continue;
          }

          print_attr_req_tbl[i].attr_name[j] = 
             tolower(print_attr_req_tbl[i].attr_name[j]);
       }

       print_attr_req_tbl[i].check_attr_with_hash  = true;
       print_attr_req_tbl[i].hash_of_attr_name     = 0;
       print_attr_req_tbl[i].attr_was_requested    = false;
       print_attr_req_tbl[i].reset_attr_on_new_req = true;
   }

   ipp_init_attr_hash_tbl(print_attr_req_tbl, NUM_REQ_PRINTER_ATTRIBUTES);
}

void set_printer_attributes_requested(ipp_requested_attrs_t *requested_attributes)
{
   ipp_set_attributes_requested(print_attr_req_tbl,
                                NUM_REQ_PRINTER_ATTRIBUTES,
                                requested_attributes,
                                _set_default_req_attr_to_be_ret);
}


bool has_printer_attr_been_requested(uint8_t print_attr)
{
   if (print_attr_req_tbl[print_attr].attr_was_requested)
   {
      return(true);
   }
   return(false);
}


//=========================================================================================
// Definitions for local functions
//=========================================================================================

void _set_default_req_attr_to_be_ret()
{
   uint32_t i;

   for (i = 0; i < NUM_REQ_PRINTER_ATTRIBUTES; ++i)
   {
      if (i != MEDIA_COL_DATABASE)
      {
         print_attr_req_tbl[i].attr_was_requested = true;
      }
   }
}


const char *_get_ipp_request_attribute_string(requested_printer_attributes_e tag)
{
#define _CASE(x) case x: return #x

   switch (tag)
   {
      _CASE(PRINTER_URI_SUPPORTED);
      _CASE(PRINTER_SUPPLY_INFO_URI);
      _CASE(URI_SECURITY_SUPPORTED);
      _CASE(URI_AUTHENTICATION_SUPPORTED);
      _CASE(IDENTIFY_ACTIONS_DEFAULT);
      _CASE(IDENTIFY_ACTIONS_SUPPORTED);
      _CASE(PRINTER_NAME);
      _CASE(COLOR_SUPPORTED);
      _CASE(PRINT_COLOR_MODE_DEFAULT);
      _CASE(PRINT_COLOR_MODE_SUPPORTED);
      _CASE(PRINTER_STATE);
      _CASE(PRINTER_STATE_REASONS);
      _CASE(IPP_VERSIONS_SUPPORTED);
      _CASE(OPERATIONS_SUPPORTED);
      _CASE(CHARSET_CONFIGURED);
      _CASE(CHARSET_SUPPORTED);
      _CASE(NATURAL_LANGUAGE_CONFIGURED);
      _CASE(GENERATED_NATURAL_LANGUAGE_SUPPORTED);
      _CASE(DOCUMENT_FORMAT_DEFAULT);
      _CASE(DOCUMENT_FORMAT_SUPPORTED);
      _CASE(JOB_CREATION_ATTRIBUTES_SUPPORTED);
      _CASE(PRINTER_IS_ACCEPTING_JOBS);
      _CASE(QUEUED_JOB_COUNT);
      _CASE(PDL_OVERRIDE_SUPPORTED);
      _CASE(PRINTER_UP_TIME);
      _CASE(PRINTER_MAKE_AND_MODEL);
      _CASE(PRINTER_LOCATION);
      _CASE(PRINTER_INFO);
      _CASE(PRINTER_DEVICE_ID);
      _CASE(PRINTER_MORE_INFO);
      _CASE(OPEN_SPACE);
      _CASE(PRINTER_DNS_SD_NAME);
      _CASE(COMPRESSION_SUPPORTED);
      _CASE(JOB_HOLD_UNTIL_DEFAULT);
      _CASE(JOB_HOLD_UNTIL_SUPPORTED);
      _CASE(JOB_SHEETS_DEFAULT);
      _CASE(JOB_SHEETS_SUPPORTED);
      _CASE(MULTIPLE_DOCUMENT_JOBS_SUPPORTED);
      _CASE(MULTIPLE_OPERATION_TIME_OUT);
      _CASE(PAGES_PER_MINUTE);
      _CASE(PAGES_PER_MINUTE_COLOR);
      _CASE(COPIES_DEFAULT);
      _CASE(COPIES_SUPPORTED);
      _CASE(JPEG_X_DIMENSION_SUPPORTED);
      _CASE(JPEG_Y_DIMENSION_SUPPORTED);
      _CASE(LANDSCAPE_ORIENTATION_REQUESTED_PREFERRED);
      _CASE(FINISHINGS_DEFAULT);
      _CASE(FINISHINGS_SUPPORTED);
      _CASE(PAGE_RANGES_SUPPORTED);
      _CASE(SIDES_DEFAULT);
      _CASE(SIDES_SUPPORTED);
      _CASE(NUMBER_UP_DEFAULT);
      _CASE(NUMBER_UP_SUPPORTED);
      _CASE(ORIENTATION_REQUESTED_DEFAULT);
      _CASE(ORIENTATION_REQUESTED_SUPPORTED);
      _CASE(MEDIA_DEFAULT);
      _CASE(MEDIA_SUPPORTED);
      _CASE(MEDIA_TYPE_SUPPORTED);
      _CASE(MEDIA_SOURCE_SUPPORTED);
      _CASE(MEDIA_READY);
      _CASE(PRINTER_RESOLUTION_DEFAULT);
      _CASE(PRINTER_RESOLUTION_SUPPORTED);
      _CASE(PRINT_QUALITY_DEFAULT);
      _CASE(PRINT_QUALITY_SUPPORTED);
      _CASE(PRINTER_UUID);
      _CASE(JPEG_K_OCTETS_SUPPORTED);
      _CASE(PDF_K_OCTETS_SUPPORTED);
      _CASE(MEDIA_COL_SUPPORTED);
      _CASE(MEDIA_LEFT_MARGIN_SUPPORTED);
      _CASE(MEDIA_RIGHT_MARGIN_SUPPORTED);
      _CASE(MEDIA_TOP_MARGIN_SUPPORTED);
      _CASE(MEDIA_BOTTOM_MARGIN_SUPPORTED);
      _CASE(OUTPUT_BIN_DEFAULT);
      _CASE(OUTPUT_BIN_SUPPORTED);
      _CASE(MEDIA_COL_DEFAULT);
      _CASE(MEDIA_COL_DATABASE);
      _CASE(MEDIA_SIZE_SUPPORTED);
      _CASE(MEDIA_COL_READY);
      _CASE(PRINTER_ICONS);
      _CASE(IPP_FEATURES_SUPPORTED);
      _CASE(PRINT_CONTENT_OPTIMIZE_DEFAULT);
      _CASE(PRINT_CONTENT_OPTIMIZE_SUPPORTED);
      _CASE(PRINTER_GEO_LOCATION);
      _CASE(EPCL_VERSION_SUPPORTED);
      _CASE(MARGINS_PRE_APPLIED);
      _CASE(PCLM_COMPRESSION_METHOD_PREFERRED);
      _CASE(PCLM_RASTER_BACK_SIDE);
      _CASE(PCLM_SOURCE_RESOLUTION);
      _CASE(PCLM_SOURCE_RESOLUTION_SUPPORTED);
      _CASE(PCLM_STRIP_HEIGHT_PREFERRED);
      _CASE(PCLM_STRIP_HEIGHT_SUPPORTED);

      default: 
		  ASSERT(0);
		  break;
   }

   return(NULL);
}
