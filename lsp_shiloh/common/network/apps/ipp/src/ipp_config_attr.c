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
#include <ctype.h>
#include <stdbool.h>
#include "sm_job_api.h"
#include "ipp_api.h"
#include "logger.h"
#include "ipp.h"
#include "ipp_types.h"
#include "ipp_const.h"
#include "ipp_attribute.h"
#include "ipp_dbg.h"
#include "ipp_cntxt.h"
#include "ipp_request.h"
#include "ipp_printer_requested_attr.h"
#include "ipp_config_attr.h"
#include "ipp_validate_request.h"
#include "ipp_handle_attr_reqs.h"
#include "memAPI.h"


//=========================================================================================
// Typedefs
//=========================================================================================

typedef enum
{
   COMPRESSION = 0,
   DOCUMENT_FORMAT,
   COPIES,
   FINISHINGS,
   MEDIA,
   MEDIA_COL,
   ORIENTATION_REQUESTED,
   OUTPUT_BIN,
   PRINT_COLOR_MODE,
   PRINT_CONTENT_OPTIMIZE,
   PRINT_QUALITY,
   PRINTER_RESOLUTION,
   PRINT_SCALING,
   SIDES,

   // Keep this as the last value.
   NUM_PRINT_JOB_CONFIG_ATTR,

} print_job_config_attr_e;

typedef enum
{
   CONFIG_JOB_TICKET,
   CONFIG_DOC_TICKET,

} config_ticket_e;

typedef void (*set_attr_func_ptr)(ipp_attr_t *, ipp_print_job_cfg_attr_t*);

//=========================================================================================
// Local variables
//=========================================================================================
static ipp_print_job_cfg_attr_t *base_job_config_attr_tbl = NULL;

//=========================================================================================
// Local function declarations
//=========================================================================================
static void ipp_init_job_config_attr_tbl();
static void free_ipp_job_req_attr(ipp_print_job_cfg_attr_t *ipp_job_attr_req);
static const char * _get_ipp_print_job_config_attr_string(print_job_config_attr_e tag);
static ipp_print_job_cfg_attr_t *generate_job_config_attr_tbl();
static void populate_attr_from_specific_group(ipp_attr_grp_t *attr_group,
                                              ipp_print_job_cfg_attr_t *job_cfg_attr_ptr,
                                              uint32_t num_job_cfg_attrs);
static bool ipp_populate_given_job_attr(ipp_attr_groups_t *attr_groups, 
                                        ipp_print_job_cfg_attr_t **ipp_job_attr_req,
                                        config_ticket_e ticket_type);
static set_attr_func_ptr _get_ipp_print_job_config_attr_func(print_job_config_attr_e tag);
static uint32_t get_hash_of_member_name(char* data, int len);
static void set_resolution_attr(ipp_attr_t *ipp_attr, 
                                ipp_print_job_cfg_attr_t *print_job_cfg_attr);
static void set_media_col_attr(ipp_attr_t *ipp_attr, 
                               ipp_print_job_cfg_attr_t *print_job_cfg_attr);
static void set_integer_attr(ipp_attr_t *ipp_attr, 
                             ipp_print_job_cfg_attr_t *print_job_cfg_attr);
static void set_string_attr(ipp_attr_t *ipp_attr, 
                            ipp_print_job_cfg_attr_t *print_job_cfg_attr);

//=========================================================================================
// Start of global functions
//=========================================================================================

void ipp_add_req_attr_to_job_ticket(ipp_attr_groups_t *attr_groups,
                                    smjob_ticket_t    *job_ticket)
{
   ipp_print_job_cfg_attr_t *ipp_job_attr_req = NULL;

   if (ipp_populate_given_job_attr(attr_groups, &ipp_job_attr_req , CONFIG_JOB_TICKET))
   {
      if (ipp_job_attr_req[COPIES].config_val_set)
      {
         ASSERT(ipp_job_attr_req[COPIES].config_data_len == sizeof(uint32_t));

         job_ticket->doc_processing.type.print.copies = ipp_job_attr_req[COPIES].data_val;
      }

      if (ipp_job_attr_req[FINISHINGS].config_val_set)
      {
         // This is not part of the job ticket or document ticket so nothing to do.
      }

      if (ipp_job_attr_req[MEDIA].config_val_set)
      {
         ASSERT(ipp_job_attr_req[MEDIA].config_data_len == sizeof(uint32_t));

         job_ticket->doc_processing.type.print.media_type = ipp_job_attr_req[MEDIA].data_val;
      }

      if (ipp_job_attr_req[MEDIA_COL].config_val_set)
      {
         ASSERT(ipp_job_attr_req[MEDIA_COL].config_data_len == sizeof(smjob_media_col_t));

         memcpy(&job_ticket->doc_processing.type.print.media_col,
                ipp_job_attr_req[MEDIA_COL].data_ptr,
                ipp_job_attr_req[MEDIA_COL].config_data_len);
      }

      if (ipp_job_attr_req[ORIENTATION_REQUESTED].config_val_set)
      {
         ASSERT(ipp_job_attr_req[ORIENTATION_REQUESTED].config_data_len == sizeof(uint32_t));

         job_ticket->doc_processing.orientation = 
            ipp_job_attr_req[ORIENTATION_REQUESTED].data_val;
      }

      if (ipp_job_attr_req[OUTPUT_BIN].config_val_set)
      {
         ASSERT(ipp_job_attr_req[OUTPUT_BIN].config_data_len < SMJOB_MAX_NAME_LEN);

         memcpy(&job_ticket->doc_processing.type.print.media_col.output_bin,
                ipp_job_attr_req[OUTPUT_BIN].data_ptr,
                ipp_job_attr_req[OUTPUT_BIN].config_data_len);
      }

      if (ipp_job_attr_req[PRINT_COLOR_MODE].config_val_set)
      {
         ASSERT(ipp_job_attr_req[PRINT_COLOR_MODE].config_data_len < COLOR_MODE_STR_LEN);

         memcpy(&job_ticket->doc_processing.type.print.color_mode,
                ipp_job_attr_req[PRINT_COLOR_MODE].data_ptr,
                ipp_job_attr_req[PRINT_COLOR_MODE].config_data_len);
      }

      if (ipp_job_attr_req[PRINT_CONTENT_OPTIMIZE].config_val_set)
      {
         ASSERT(ipp_job_attr_req[PRINT_CONTENT_OPTIMIZE].config_data_len <
                PRINT_CONTENT_OPTIMIZE_STR_LEN);
  
         memcpy(&job_ticket->doc_processing.type.print.print_content_optimize,
                ipp_job_attr_req[PRINT_CONTENT_OPTIMIZE].data_ptr,
                ipp_job_attr_req[PRINT_CONTENT_OPTIMIZE].config_data_len);
      }

      if (ipp_job_attr_req[PRINT_QUALITY].config_val_set)
      {
         ASSERT(ipp_job_attr_req[PRINT_QUALITY].config_data_len == sizeof(uint32_t));

         job_ticket->doc_processing.type.print.quality = 
            ipp_job_attr_req[PRINT_QUALITY].data_val;
      }

      if (ipp_job_attr_req[PRINTER_RESOLUTION].config_val_set)
      {
         ASSERT(ipp_job_attr_req[PRINTER_RESOLUTION].config_data_len == 
                sizeof(smjob_resolution_t));

         memcpy(&job_ticket->doc_processing.resolution, 
                ipp_job_attr_req[PRINTER_RESOLUTION].data_ptr, 
                ipp_job_attr_req[PRINTER_RESOLUTION].config_data_len);
      }

      if (ipp_job_attr_req[SIDES].config_val_set)
      {
         ASSERT(ipp_job_attr_req[SIDES].config_data_len < SIDES_STR_LEN);

         memcpy(&job_ticket->doc_processing.type.print.sides,
                ipp_job_attr_req[SIDES].data_ptr,
                ipp_job_attr_req[SIDES].config_data_len);
      }

      free_ipp_job_req_attr(ipp_job_attr_req);
   }
}

void ipp_add_req_attr_to_doc_ticket(ipp_attr_groups_t  *attr_groups,
                                    smjob_doc_ticket_t *doc_ticket)
{
   ipp_print_job_cfg_attr_t *ipp_job_attr_req = NULL;

   if (ipp_populate_given_job_attr(attr_groups, &ipp_job_attr_req, CONFIG_DOC_TICKET))
   {
      if (ipp_job_attr_req[COMPRESSION].config_val_set)
      {
         ASSERT(ipp_job_attr_req[COMPRESSION].config_data_len < COMPRESSION_STR_LEN);

         memcpy(doc_ticket->doc_description.type.print.compression,
                ipp_job_attr_req[COMPRESSION].data_ptr,
                ipp_job_attr_req[COMPRESSION].config_data_len);
      }

      if (ipp_job_attr_req[DOCUMENT_FORMAT].config_val_set)
      {
         ASSERT(ipp_job_attr_req[DOCUMENT_FORMAT].config_data_len < FORMAT_STR_LEN);

         memcpy(doc_ticket->doc_description.type.print.format,
                ipp_job_attr_req[DOCUMENT_FORMAT].data_ptr,
                ipp_job_attr_req[DOCUMENT_FORMAT].config_data_len);
      }

      free_ipp_job_req_attr(ipp_job_attr_req);
   }
}

void free_ipp_job_req_attr(ipp_print_job_cfg_attr_t *ipp_job_attr_req)
{
   if (ipp_job_attr_req != NULL)
   {
      uint32_t i;

      for (i = 0; i < NUM_PRINT_JOB_CONFIG_ATTR; ++i)
      {
         if (ipp_job_attr_req->data_ptr != NULL)
         {
            IPP_FREE(ipp_job_attr_req->data_ptr);
         }
      }

      IPP_FREE(ipp_job_attr_req);
   }

}

//=========================================================================================
// Local function definitions
//=========================================================================================

bool ipp_populate_given_job_attr(ipp_attr_groups_t        *attr_groups, 
                                 ipp_print_job_cfg_attr_t **ipp_job_attr_req,
                                 config_ticket_e          ticket_type)
{
   bool                     job_attr_success = false;
   bool                     attr_present;
   uint32_t                 attr_grp_index;
   ipp_print_job_cfg_attr_t *ret_job_config_attr_tbl = NULL;

   if (ticket_type == CONFIG_DOC_TICKET)
   {
      ret_job_config_attr_tbl = generate_job_config_attr_tbl();

      // Check for the attributes that are present in the operations attributes.
      attr_present = ipp_chk_operation_attr_tag_present(attr_groups, 
                                                        &attr_grp_index);

      if (attr_present)
      {
         ipp_attr_grp_t *oper_attr_group = &attr_groups->grp[attr_grp_index];

         populate_attr_from_specific_group(oper_attr_group,
                                           ret_job_config_attr_tbl,
                                           NUM_PRINT_JOB_CONFIG_ATTR);
      }

      job_attr_success = true;
   }
   else if (ticket_type == CONFIG_JOB_TICKET)
   {
      ret_job_config_attr_tbl = generate_job_config_attr_tbl();

      // Check for the attributes that are present in the job template attributes.
      attr_present = ipp_chk_job_template_attr_tag_present(attr_groups, 
                                                           &attr_grp_index);
      if (attr_present)
      {
         ipp_attr_grp_t *job_template_attr_group = &attr_groups->grp[attr_grp_index];

         populate_attr_from_specific_group(job_template_attr_group,
                                           ret_job_config_attr_tbl,
                                           NUM_PRINT_JOB_CONFIG_ATTR);
      }

      job_attr_success = true;
   }
   
   *ipp_job_attr_req = ret_job_config_attr_tbl;

   return(job_attr_success);
}

ipp_print_job_cfg_attr_t *generate_job_config_attr_tbl()
{
   ipp_print_job_cfg_attr_t *ret_job_config_attr_tbl = NULL;

   // If the base table has not been generated, do that now.
   if (base_job_config_attr_tbl == NULL)
   {
      ipp_init_job_config_attr_tbl();
   }

   uint32_t table_size = sizeof(ipp_print_job_cfg_attr_t) * NUM_PRINT_JOB_CONFIG_ATTR;
   ret_job_config_attr_tbl = IPP_CALLOC(table_size, 1);
   ASSERT(ret_job_config_attr_tbl);

   memcpy(ret_job_config_attr_tbl, 
          base_job_config_attr_tbl, 
          table_size);

   return(ret_job_config_attr_tbl);
}

void ipp_init_job_config_attr_tbl()
{
   uint32_t                 i;
   uint32_t                 j;
   const char               *config_attr_str;
   uint32_t                 config_attr_str_len;
   ipp_print_job_cfg_attr_t *base_cfg_ptr = NULL;
   uint32_t                 table_size    = 0;

   table_size = NUM_PRINT_JOB_CONFIG_ATTR * sizeof(ipp_print_job_cfg_attr_t);

   base_job_config_attr_tbl = IPP_CALLOC(table_size, 1);
   ASSERT(base_job_config_attr_tbl);

   for (i = 0; i < NUM_PRINT_JOB_CONFIG_ATTR; ++i)
   {
      base_cfg_ptr = &base_job_config_attr_tbl[i];

       config_attr_str = _get_ipp_print_job_config_attr_string(i);

       config_attr_str_len = strlen(config_attr_str);

       ASSERT(config_attr_str_len < MAX_PRINT_JOB_CFG_ATTR_LEN);

       strncpy(&base_cfg_ptr->str_identifier[0], 
               config_attr_str, 
               config_attr_str_len);

       for (j = 0; j < config_attr_str_len; ++j)
       {
          if (base_cfg_ptr->str_identifier[j] == '_')
          {
             base_cfg_ptr->str_identifier[j] = '-';
             continue;
          }

          base_cfg_ptr->str_identifier[j] = 
             tolower(base_cfg_ptr->str_identifier[j]);
       }

       base_cfg_ptr->hash_of_str = 
          ipp_hash_attr_name(&base_cfg_ptr->str_identifier[0]);
   }

}

void populate_attr_from_specific_group(ipp_attr_grp_t           *attr_group,
                                       ipp_print_job_cfg_attr_t *job_cfg_attr_tbl,
                                       uint32_t                 num_job_cfg_attrs)
{
   uint32_t                 i;
   uint32_t                 j;
   uint32_t                 cur_attr_hash;
   ipp_print_job_cfg_attr_t *job_cfg_attr_ptr = NULL;
   ipp_attr_t               *current_attribute = NULL;
   uint32_t                 num_attributes     = attr_group->num_attr;
   set_attr_func_ptr        set_attr_func      = NULL;

   for (i = 0; i < num_attributes; ++i)
   {
      current_attribute = &attr_group->attr[i];
      cur_attr_hash     = ipp_hash_attr_name(current_attribute->name);

      for (j = 0; j < num_job_cfg_attrs; ++j)
      {
         job_cfg_attr_ptr = &job_cfg_attr_tbl[j];

         // If the name has already been found, just skip the compare.
         if (job_cfg_attr_ptr->config_val_set)
         {
            continue;
         }

         if (job_cfg_attr_ptr->hash_of_str == cur_attr_hash)
         {
            set_attr_func = _get_ipp_print_job_config_attr_func(j);
            ASSERT(set_attr_func);

            set_attr_func(current_attribute, job_cfg_attr_ptr);
         }
      }
   }
}

const char * _get_ipp_print_job_config_attr_string(print_job_config_attr_e tag)
{
#define _CASE(x) case x: return #x

   switch (tag)
   {
      _CASE(COMPRESSION);
      _CASE(DOCUMENT_FORMAT);
      _CASE(COPIES);
      _CASE(FINISHINGS);
      _CASE(MEDIA);
      _CASE(MEDIA_COL);
      _CASE(ORIENTATION_REQUESTED);
      _CASE(OUTPUT_BIN);
      _CASE(PRINT_COLOR_MODE);
      _CASE(PRINT_CONTENT_OPTIMIZE);
      _CASE(PRINT_QUALITY);
      _CASE(PRINTER_RESOLUTION);
      _CASE(PRINT_SCALING);
      _CASE(SIDES);

      default: 
		  ASSERT(0);
		  break;
   }

   return(NULL);
}

void set_string_attr(ipp_attr_t               *ipp_attr, 
                     ipp_print_job_cfg_attr_t *print_job_cfg_attr)
{
   uint32_t data_len;

   data_len = ipp_attr->val->len + 1;

   print_job_cfg_attr->data_ptr = IPP_CALLOC(data_len, 1);
   ASSERT(print_job_cfg_attr->data_ptr);

   print_job_cfg_attr->config_data_len = data_len;

   // Only copy over the actual data as it is not null terminated. The null
   // termination for this string will come about from the calloc above.
   strncpy(print_job_cfg_attr->data_ptr, ipp_attr->val->data, data_len - 1);

   print_job_cfg_attr->config_val_set = true;
}

void set_integer_attr(ipp_attr_t               *ipp_attr, 
                     ipp_print_job_cfg_attr_t *print_job_cfg_attr)
{
   print_job_cfg_attr->config_data_len = sizeof(uint32_t);

   print_job_cfg_attr->data_val = ntohl(*((uint32_t *)ipp_attr->val->data));

   print_job_cfg_attr->config_val_set = true;
}

void set_media_col_attr(ipp_attr_t               *ipp_attr, 
                        ipp_print_job_cfg_attr_t *print_job_cfg_attr)
{
   uint32_t       attr_index     = 0;
   ipp_attr_val_t *attr_val_ptr  = NULL;

   uint32_t media_bottom_margin_hash = ipp_hash_attr_name("media-bottom-margin");
   uint32_t media_top_margin_hash    = ipp_hash_attr_name("media-top-margin");
   uint32_t media_left_margin_hash   = ipp_hash_attr_name("media-left-margin");
   uint32_t media_right_margin_hash  = ipp_hash_attr_name("media-right-margin");
   uint32_t x_dimension_hash         = ipp_hash_attr_name("x-dimension");
   uint32_t y_dimension_hash         = ipp_hash_attr_name("y-dimension");
   uint32_t media_type_hash          = ipp_hash_attr_name("media-type");
   uint32_t media_source_hash        = ipp_hash_attr_name("media-source");

   smjob_media_col_t *media_col_ptr = IPP_CALLOC(sizeof(smjob_media_col_t), 1);
   ASSERT(media_col_ptr);

   while (attr_index < ipp_attr->num_val)
   {
      attr_val_ptr = &ipp_attr->val[attr_index];

      if (attr_val_ptr->syntax == IPP_TAG_MEMBER_ATTR_NAME)
      {
         uint32_t *media_col_val_ptr = NULL;
         char     *media_col_str_ptr = NULL;
         uint32_t hash_value         = get_hash_of_member_name(attr_val_ptr->data, 
                                                               attr_val_ptr->len);

         if (hash_value == media_bottom_margin_hash)
         {
            media_col_val_ptr = &media_col_ptr->bottom_margin;
         }
         else if (hash_value == media_top_margin_hash)
         {
            media_col_val_ptr = &media_col_ptr->top_margin;
         }
         else if (hash_value == media_left_margin_hash)
         {
            media_col_val_ptr = &media_col_ptr->left_margin;
         }
         else if (hash_value == media_right_margin_hash)
         {
            media_col_val_ptr = &media_col_ptr->right_margin;
         }
         else if (hash_value == x_dimension_hash)
         {
            media_col_val_ptr = &media_col_ptr->x_dimension;
         }
         else if (hash_value == y_dimension_hash)
         {
            media_col_val_ptr = &media_col_ptr->y_dimension;
         }
         else if (hash_value == media_type_hash)
         {
            media_col_str_ptr = &media_col_ptr->type[0];
         }
         else if (hash_value == media_source_hash)
         {
            media_col_str_ptr = &media_col_ptr->source[0];
         }

         if (media_col_val_ptr != NULL)
         {
            ++attr_index;

            ASSERT(attr_index < ipp_attr->num_val);

            attr_val_ptr = &ipp_attr->val[attr_index];

            if ((attr_val_ptr->syntax == IPP_TAG_INTEGER) && (attr_val_ptr->len > 0))
            {
               *media_col_val_ptr = ntohl(*((uint32_t *)attr_val_ptr->data));
            }
         }
         else if (media_col_str_ptr != NULL)
         {
            ++attr_index;

            ASSERT(attr_index < ipp_attr->num_val);

            attr_val_ptr = &ipp_attr->val[attr_index];

            if ((attr_val_ptr->syntax == IPP_TAG_KEYWORD) && (attr_val_ptr->len > 0))
            {
               strncpy(media_col_str_ptr, attr_val_ptr->data, attr_val_ptr->len);
            }
         }
      }

      ++attr_index;
   }

   print_job_cfg_attr->data_ptr = media_col_ptr;

   print_job_cfg_attr->config_data_len = sizeof(smjob_media_col_t);

   print_job_cfg_attr->config_val_set = true;  
}

uint32_t get_hash_of_member_name(char* data, int len)
{
   uint32_t hash_value;
   char     *str;

   str = IPP_MALLOC(len + 1);

   if (str)
   {
     memcpy(str, data, len);
     str[len] = 0;
   }
   else
   {
      ASSERT(0);
   }

   hash_value = ipp_hash_attr_name(str);

   IPP_FREE(str);

   return(hash_value);
}

void set_resolution_attr(ipp_attr_t               *ipp_attr, 
                         ipp_print_job_cfg_attr_t *print_job_cfg_attr)
{
   uint32_t data_val;
   uint32_t index;
   uint8_t  *byte_array_ptr;

   smjob_resolution_t *resolution = IPP_MALLOC(sizeof(smjob_resolution_t));
   ASSERT(resolution);

   byte_array_ptr = (uint8_t *)ipp_attr->val->data;

   index = 0;

   memcpy(&data_val, byte_array_ptr + index, sizeof(uint32_t));

   resolution->xfeed_dir = ntohl(data_val);

   index += 4;

   memcpy(&data_val, byte_array_ptr + index, sizeof(uint32_t));

   resolution->feed_dir = ntohl(data_val);

   index += 4;

   resolution->units = *(byte_array_ptr + index);

   print_job_cfg_attr->data_ptr = resolution;

   print_job_cfg_attr->config_data_len = sizeof(smjob_resolution_t);

   print_job_cfg_attr->config_val_set = true;  
}

set_attr_func_ptr _get_ipp_print_job_config_attr_func(print_job_config_attr_e tag)
{
   switch (tag)
   {
   case COMPRESSION:            return(&set_string_attr);
   case DOCUMENT_FORMAT:        return(&set_string_attr);
   case COPIES:                 return(&set_integer_attr);
   case FINISHINGS:             return(&set_integer_attr);
   case MEDIA:                  return(&set_integer_attr);
   case MEDIA_COL:              return(&set_media_col_attr);
   case ORIENTATION_REQUESTED:  return(&set_integer_attr);
   case OUTPUT_BIN:             return(&set_string_attr);
   case PRINT_COLOR_MODE:       return(&set_string_attr);
   case PRINT_CONTENT_OPTIMIZE: return(&set_string_attr);
   case PRINT_QUALITY:          return(&set_integer_attr);
   case PRINTER_RESOLUTION:     return(&set_resolution_attr);
   case PRINT_SCALING:          return(&set_string_attr);
   case SIDES:                  return(&set_string_attr);

   default: 
	   ASSERT(0);
	   break;
   }

   return(NULL);
}

