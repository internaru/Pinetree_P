/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
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
#include "ipp_handle_attr_reqs.h"

//=========================================================================================
// Local variables
//=========================================================================================


//=========================================================================================
// Local functions
//=========================================================================================
static unsigned int _hash_attr(const char* s, unsigned int seed);
static bool _check_hash_value_detected(uint32_t current_index, uint32_t hash_value,
                                       requested_attribute_info_t *req_attr_info_tbl);
static void _reset_whether_attrib_req(requested_attribute_info_t *req_attr_info_tbl,
                                      uint32_t size_req_attr_info_tbl);
static void _get_client_req_attr_hash_values(ipp_requested_attrs_t *all_req_attributes,
                                             uint32_t **req_attr_hash_tbl,
                                             uint32_t *num_req_attr);
static bool all_req_attr_to_be_returned(uint32_t *cur_hash_tbl_ptr, uint32_t num_entries);


void ipp_set_attributes_requested(requested_attribute_info_t *req_attr_info_tbl,
                                  uint32_t                   size_req_attr_info_tbl,
                                  ipp_requested_attrs_t      *requested_attributes,
                                  ret_default_attributes     set_default_attr_ret_func)
{
   uint32_t i;
   uint32_t j;
   uint32_t *hash_tbl_ptr = NULL;
   uint32_t num_req_attr  = 0;

   _reset_whether_attrib_req(req_attr_info_tbl, size_req_attr_info_tbl);

   if (requested_attributes)
   {
      // Create a hash table of the values of the attributes that the client has
      // requested.
      _get_client_req_attr_hash_values(requested_attributes, &hash_tbl_ptr, &num_req_attr);
   }

   if (num_req_attr > 0)
   {
      // Check to see if the "all" string was sent. This makes it easier in that we
      // will just send back everything.
      if (all_req_attr_to_be_returned(hash_tbl_ptr, num_req_attr))
      {
         for (i = 0; i < size_req_attr_info_tbl; ++i)
         {
            req_attr_info_tbl[i].attr_was_requested = true;
         }
      }
      // Going to do it the hard way. Compare each of the hashed requested attributes
      // with the hashed values of the attributes we can return.
      else
      {
         for (i = 0; i < num_req_attr; ++i)
         {
            for (j = 0; j < size_req_attr_info_tbl; ++j)
            {
               // NEED SOMETHING IN HERE TO CHECK THE STRINGS IF THE HASH VALUE NOT
               //  AVAILABLE. LOOK AT VALUE check_attr_with_hash.

               if (hash_tbl_ptr[i] == req_attr_info_tbl[j].hash_of_attr_name)
               {
                  req_attr_info_tbl[j].attr_was_requested = true;
                  break;
               }
            }
         }
      }
   }
   else
   {
      set_default_attr_ret_func();
   }
   
   if (hash_tbl_ptr)
   {
      IPP_FREE(hash_tbl_ptr);
   }
}

void ipp_init_attr_hash_tbl(requested_attribute_info_t *req_attr_info_tbl,
                            uint32_t                   size_req_attr_info_tbl)
{
   uint32_t i;
   uint32_t hash_value;

   for (i = 0; i < size_req_attr_info_tbl; ++i)
   {
      hash_value = _hash_attr(req_attr_info_tbl[i].attr_name, 0);
      
      if (_check_hash_value_detected(i, hash_value, req_attr_info_tbl))
      {
         req_attr_info_tbl[i].hash_of_attr_name    = 0;
         req_attr_info_tbl[i].check_attr_with_hash = false;
         ASSERT(0);
      }
      else
      {
         req_attr_info_tbl[i].hash_of_attr_name    = hash_value;
         req_attr_info_tbl[i].check_attr_with_hash = true;
      }
   }
}

uint32_t ipp_hash_attr_name(const char *attr_name)
{
   uint32_t seed = 0;

   return(_hash_attr(attr_name, seed));
}

void _reset_whether_attrib_req(requested_attribute_info_t *req_attr_info_tbl,
                               uint32_t                   size_req_attr_info_tbl)
{
   uint32_t i;

   for (i = 0; i < size_req_attr_info_tbl; ++i)
   {
      if (req_attr_info_tbl[i].reset_attr_on_new_req)
      {
         req_attr_info_tbl[i].attr_was_requested = false;
      }
   }
}

unsigned int _hash_attr(const char* s, unsigned int seed)
{
    unsigned int hash = seed;

    while (*s)
    {
        hash = hash * 101  +  *s++;
    }
    return hash;
}

bool _check_hash_value_detected(uint32_t                   current_index, 
                                uint32_t                   hash_value, 
                                requested_attribute_info_t *req_attr_info_tbl)
{
   uint32_t i;
   bool     hash_value_detected = false;

   for (i = 0; i < current_index; ++i)
   {
      if (hash_value == req_attr_info_tbl[i].hash_of_attr_name)
      {
         hash_value_detected = true;
         ASSERT(0);
         break;
      }
   }

   return(hash_value_detected);
}


void _get_client_req_attr_hash_values(ipp_requested_attrs_t *all_req_attributes,
                                      uint32_t               **req_attr_hash_tbl,
                                      uint32_t              *num_req_attr)
{
   uint32_t i;
   uint32_t offset        = 0;
   uint32_t *hash_tbl_ptr = NULL;

   hash_tbl_ptr = IPP_MALLOC(sizeof(uint32_t) * all_req_attributes->num_req_attr);
   ASSERT(hash_tbl_ptr);

   for (i = 0; i < all_req_attributes->num_req_attr; ++i)
   {
      hash_tbl_ptr[i] = _hash_attr(&all_req_attributes->req_attr_str[offset], 0);
      
      offset += strlen(&all_req_attributes->req_attr_str[offset]);
      offset += 1;
   }

   *req_attr_hash_tbl = hash_tbl_ptr;
   *num_req_attr      = all_req_attributes->num_req_attr;

}

bool all_req_attr_to_be_returned(uint32_t *cur_hash_tbl_ptr, uint32_t num_entries)
{
   uint32_t i;
   bool     ret_all_req_attr  = false;
   uint32_t hash_value_of_all = _hash_attr("all", 0);

   for (i = 0; i < num_entries; ++i)
   {
      if (hash_value_of_all == cur_hash_tbl_ptr[i])
      {
         ret_all_req_attr = true;
         break;
      }
   }

   return(ret_all_req_attr);
}

