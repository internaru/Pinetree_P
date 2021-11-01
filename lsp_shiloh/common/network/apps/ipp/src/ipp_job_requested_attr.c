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

#include "ipp_job_requested_attr.h"
#include "ipp_handle_attr_reqs.h"

//=========================================================================================
// Defines
//=========================================================================================

//=========================================================================================
// Local variables
//=========================================================================================
static requested_attribute_info_t *job_attr_req_tbl = NULL;
static bool current_jobs_operation = false;

//=========================================================================================
// Local functions
//=========================================================================================
static void _set_default_req_attr_to_be_ret();
static const char *_get_ipp_request_attribute_string(uint8_t tag);

void generate_req_job_attr_tbl()
{
   uint32_t   i;
   uint32_t   j;
   const char *req_attr_str    = NULL;
   uint32_t   req_attr_str_len = 0;

   job_attr_req_tbl = IPP_MALLOC(NUM_REQ_JOB_ATTRIBUTES * 
                                   sizeof(requested_attribute_info_t));
   ASSERT(job_attr_req_tbl);

   for (i = 0; i < NUM_REQ_JOB_ATTRIBUTES; ++i)
   {
       req_attr_str = _get_ipp_request_attribute_string(i);

       req_attr_str_len = strlen(req_attr_str);
       job_attr_req_tbl[i].attr_name = IPP_CALLOC(req_attr_str_len + 1, 1);
       ASSERT(job_attr_req_tbl[i].attr_name);

       strncpy(job_attr_req_tbl[i].attr_name, req_attr_str, req_attr_str_len);


       for (j = 0; j < req_attr_str_len; ++j)
       {
          if (job_attr_req_tbl[i].attr_name[j] == '_')
          {
             job_attr_req_tbl[i].attr_name[j] = '-';
             continue;
          }

          job_attr_req_tbl[i].attr_name[j] = 
             tolower(job_attr_req_tbl[i].attr_name[j]);
       }

       job_attr_req_tbl[i].check_attr_with_hash  = true;
       job_attr_req_tbl[i].hash_of_attr_name     = 0;
       job_attr_req_tbl[i].attr_was_requested    = false;
       job_attr_req_tbl[i].reset_attr_on_new_req = true;
   }

   ipp_init_attr_hash_tbl(job_attr_req_tbl, NUM_REQ_JOB_ATTRIBUTES);
}

void set_job_attributes_requested(ipp_requested_attrs_t *requested_attributes,
                                  bool jobs_operation)
{
   current_jobs_operation = jobs_operation;

   ipp_set_attributes_requested(job_attr_req_tbl,
                                NUM_REQ_JOB_ATTRIBUTES,
                                requested_attributes,
                                _set_default_req_attr_to_be_ret);
}


bool has_job_attr_been_requested(requested_job_attributes_e job_attr)
{
   if (job_attr_req_tbl[job_attr].attr_was_requested)
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
   
   if (current_jobs_operation)
   {
      for (i = 0; i < NUM_REQ_JOB_ATTRIBUTES; ++i)
      {
         // Default is only job-id and job-uri.
         if (i == JOB_URI || i == JOB_ID)
         {
            job_attr_req_tbl[i].attr_was_requested = true;
         }
      }
   }
   else
   {
      for (i = 0; i < NUM_REQ_JOB_ATTRIBUTES; ++i)
      {
         job_attr_req_tbl[i].attr_was_requested = true;
      }
   }
}


const char *_get_ipp_request_attribute_string(uint8_t tag)
{
#define _CASE(x) case x: return #x

   switch (tag)
   {
      _CASE(JOB_PRIORITY);
      _CASE(JOB_HOLD_UNTIL);
      _CASE(JOB_SHEETS);
      _CASE(MULTIPLE_DOCUMENT_HANDLING);
      _CASE(COPIES);
      _CASE(FINISHINGS);
      _CASE(PAGE_RANGES);
      _CASE(SIDES);
      _CASE(NUMBER_UP);
      _CASE(ORIENTATION_REQUESTED);
      _CASE(MEDIA);
      _CASE(PRINTER_RESOLUTION);
      _CASE(PRINT_QUALITY);
      _CASE(JOB_URI);
      _CASE(JOB_ID);
      _CASE(JOB_PRINTER_URI);
      _CASE(JOB_NAME);
      _CASE(JOB_ORIGINATING_USER_NAME);
      _CASE(JOB_STATE);
      _CASE(JOB_STATE_REASONS);
      _CASE(TIME_AT_CREATION);
      _CASE(TIME_AT_PROCESSING);
      _CASE(TIME_AT_COMPLETED);
      _CASE(JOB_PRINTER_UP_TIME);
      _CASE(JOB_UUID);
      _CASE(JOB_IMPRESSIONS);
      _CASE(JOB_IMPRESSIONS_COMPLETED);

   }

     ASSERT(0);
     return(NULL);
}
