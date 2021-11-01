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
#include "ipp_validate_request.h"

//=========================================================================================
// Typedefs
//=========================================================================================
typedef struct
{
   const char *attr_name;
   bool       attr_found;
   uint32_t   attr_index;

} attr_node_t;

//=========================================================================================
// Local variables
//=========================================================================================
static attr_node_t req_cups_get_printers_tbl[] = 
{
   {.attr_name = "attributes-charset", .attr_found = false, .attr_index = -1},

   {.attr_name = "attributes-natural-language", .attr_found = false, .attr_index = -1}
};

static uint32_t req_cups_get_printers_tbl_size = (sizeof(req_cups_get_printers_tbl) /
                                               sizeof(attr_node_t));

static attr_node_t req_get_print_attr_tbl[] = 
{
   {.attr_name = "attributes-charset", .attr_found = false, .attr_index = -1},

   {.attr_name = "attributes-natural-language", .attr_found = false, .attr_index = -1},

   {.attr_name = "printer-uri", .attr_found = false, .attr_index = -1}
};

static uint32_t req_get_print_attr_tbl_size = (sizeof(req_get_print_attr_tbl) /
                                               sizeof(attr_node_t));

static attr_node_t req_print_job_oper_attr_tbl[] = 
{
   {.attr_name = "attributes-charset", .attr_found = false, .attr_index = -1},

   {.attr_name = "attributes-natural-language", .attr_found = false, .attr_index = -1},

   {.attr_name = "printer-uri", .attr_found = false, .attr_index = -1},

   {.attr_name = "requesting-user-name", .attr_found = false, .attr_index = -1},
};

static uint32_t req_print_job_oper_attr_tbl_size = (sizeof(req_print_job_oper_attr_tbl) /
                                                    sizeof(attr_node_t));


static attr_node_t opt_print_job_oper_attr_tbl[] = 
{
   {.attr_name = "job-name", .attr_found = false, .attr_index = -1},
   {.attr_name = "ipp-attribute-fidelity", .attr_found = false, .attr_index = -1},
   {.attr_name = "document-name", .attr_found = false, .attr_index = -1},
   {.attr_name = "compression", .attr_found = false, .attr_index = -1},
   {.attr_name = "document-format", .attr_found = false, .attr_index = -1},
   {.attr_name = "document-natural-language", .attr_found = false, .attr_index = -1},
   {.attr_name = "job-k-octets", .attr_found = false, .attr_index = -1},
   {.attr_name = "job-impressions", .attr_found = false, .attr_index = -1},
   {.attr_name = "job-media-sheets", .attr_found = false, .attr_index = -1},
   {.attr_name = "output-mode", .attr_found = false, .attr_index = -1},
};

static uint32_t opt_print_job_oper_attr_tbl_size = (sizeof(opt_print_job_oper_attr_tbl) / 
                                                    sizeof(attr_node_t));

static attr_node_t req_send_document_attr_tbl[] = 
{
   {.attr_name = "attributes-charset", .attr_found = false, .attr_index = -1},

   {.attr_name = "attributes-natural-language", .attr_found = false, .attr_index = -1},

   {.attr_name = "last-document", .attr_found = false, .attr_index = -1}
};

static uint32_t req_send_document_attr_tbl_size = (sizeof(req_send_document_attr_tbl) /
                                                   sizeof(attr_node_t));



static attr_node_t req_send_doc_attr_target1_tbl[] = 
{
   {.attr_name = "printer-uri", .attr_found = false, .attr_index = -1},

   {.attr_name = "job-id", .attr_found = false, .attr_index = -1},
};

static uint32_t req_send_doc_attr_target1_tbl_size = (sizeof(req_send_doc_attr_target1_tbl) /
                                                      sizeof(attr_node_t));


static attr_node_t req_send_doc_attr_target2_tbl[] = 
{
   {.attr_name = "job-uri", .attr_found = false, .attr_index = -1},
};

static uint32_t req_send_doc_attr_target2_tbl_size = (sizeof(req_send_doc_attr_target2_tbl) /
                                                      sizeof(attr_node_t));



//=========================================================================================
// Local functions
//=========================================================================================
static void _populate_attrs_that_are_present(ipp_attr_grp_t *oper_attr_group,
                                             attr_node_t *attr_tbl_ptr, 
                                             uint32_t attr_tbl_size);
static bool _check_all_attributes_found(attr_node_t *attr_tbl, uint32_t attr_tbl_size);
bool _check_all_req_cups_get_printers_present(ipp_attr_grp_t *oper_attr_group);
static bool _check_all_req_get_print_attr_present(ipp_attr_grp_t *oper_attr_group);
static bool _check_print_job_opt_job_attr(ipp_attr_grp_t *oper_attr_group);
static bool _check_opt_attr_valid(ipp_attr_grp_t *oper_attr_group,
                                  attr_node_t *attr_tbl_ptr, uint32_t attr_tbl_size);
static bool _check_all_req_print_job_attr_present(ipp_attr_grp_t *oper_attr_group);
static bool _check_all_req_send_document_attr_present(ipp_attr_grp_t *oper_attr_group);



bool ipp_chk_valid_cups_get_printers_req(ipp_attr_groups_t *attr_groups)
{
   uint32_t oper_attr_grp_index;
   bool     valid_attrs_req = false;
   bool     found_oper_attr;

   found_oper_attr = ipp_chk_operation_attr_tag_present(attr_groups, 
                                                        &oper_attr_grp_index);

   if (found_oper_attr)
   {
      ipp_attr_grp_t *attr_group = &attr_groups->grp[oper_attr_grp_index];

      valid_attrs_req = _check_all_req_cups_get_printers_present(attr_group);
   }

   return(valid_attrs_req);
}

bool ipp_chk_valid_get_print_attr_req(ipp_attr_groups_t *attr_groups)
{
   uint32_t oper_attr_grp_index;
   bool     valid_attrs_req = false;
   bool     found_oper_attr;

   found_oper_attr = ipp_chk_operation_attr_tag_present(attr_groups, 
                                                        &oper_attr_grp_index);

   if (found_oper_attr)
   {
      ipp_attr_grp_t *attr_group = &attr_groups->grp[oper_attr_grp_index];

      valid_attrs_req = _check_all_req_get_print_attr_present(attr_group);
   }

   return(valid_attrs_req);
}

bool ipp_chk_valid_print_job_attr_req(ipp_attr_groups_t *attr_groups)
{
   uint32_t oper_attr_grp_index;
   bool     valid_attr_req;
   bool     found_attr;
   bool     tmp;

   found_attr = ipp_chk_operation_attr_tag_present(attr_groups, 
                                                   &oper_attr_grp_index);

   if (!found_attr)
   {
      return(false);
   }

   ipp_attr_grp_t *attr_group = &attr_groups->grp[oper_attr_grp_index];

   valid_attr_req = _check_all_req_print_job_attr_present(attr_group);
   
   if (!valid_attr_req)
   {
      return(valid_attr_req);
   }

   tmp = _check_print_job_opt_job_attr(attr_group);


   found_attr = ipp_chk_job_template_attr_tag_present(attr_groups, &oper_attr_grp_index);

   return(valid_attr_req);
}

bool ipp_chk_valid_send_document_attr_req(ipp_attr_groups_t *attr_groups)
{
   uint32_t oper_attr_grp_index;
   bool     valid_attrs_req = false;
   bool     found_oper_attr;

   found_oper_attr = ipp_chk_operation_attr_tag_present(attr_groups, 
                                                        &oper_attr_grp_index);

   if (found_oper_attr)
   {
      ipp_attr_grp_t *attr_group = &attr_groups->grp[oper_attr_grp_index];

      valid_attrs_req = _check_all_req_send_document_attr_present(attr_group);
   }

   return(valid_attrs_req);
}

bool ipp_chk_valid_document_addtion(ipp_attr_groups_t *attr_groups, 
                                    smjob_status_t    *job_status,
                                    bool              *add_the_document)
{
   uint32_t oper_attr_grp_index;
   bool     valid_doc_add = false;
   bool     found_oper_attr;

   *add_the_document = false;

   if (job_status->num_docs == 0)
   {
      valid_doc_add     = true;
      *add_the_document = true;
   }
   else if (job_status->num_docs > 0)
   {

      found_oper_attr = ipp_chk_operation_attr_tag_present(attr_groups, 
                                                           &oper_attr_grp_index);

      if (found_oper_attr)
      {
         uint32_t       i;
         ipp_attr_grp_t *attr_group        = &attr_groups->grp[oper_attr_grp_index];
         uint32_t       num_attributes     = attr_group->num_attr;
         bool           found_last_doc     = false;
         ipp_attr_t     *current_attribute = NULL;

         for (i = 0; i < num_attributes; ++i)
         {
            current_attribute = &attr_group->attr[i];

            // Besides finding the last-document, it also needs to be true.
            if (!strcmp(current_attribute->name, "last-document"))
            {
               found_last_doc = current_attribute->val;
               break;
            }
         }

         if (found_last_doc)
         {
            bool found_doc_name = false;

            // There should not be a document-name or this needs to be rejected.
            for (i = 0; i < num_attributes; ++i)
            {
               current_attribute = &attr_group->attr[i];

               if (!strcmp(current_attribute->name, "document-name"))
               {
                  found_doc_name = true;
                  break;
               }
            }

            if (!found_doc_name)
            {
               valid_doc_add = true;
            }

         }
      }
   }

   return(valid_doc_add);
}

bool _check_particular_group_present(uint8_t           attr_in_question,
                                     ipp_attr_groups_t *attr_groups, 
                                     uint32_t          *index_of_attr)
{
   uint32_t i;
   uint32_t num_groups = attr_groups->num_grp;
   bool     found_oper_attr = false;

   for (i = 0; i < num_groups; ++i)
   {
      if (attr_groups->grp[i].tag == attr_in_question)
      {
         *index_of_attr  = i;
         found_oper_attr = true;
         break;
      }
   }

   return(found_oper_attr);
}

bool ipp_chk_operation_attr_tag_present(ipp_attr_groups_t *attr_groups, 
                                        uint32_t          *attr_index)
{
   return(_check_particular_group_present(IPP_TAG_OPERATION_ATTRIBUTES,
                                          attr_groups,
                                          attr_index));
}

bool ipp_chk_job_template_attr_tag_present(ipp_attr_groups_t *attr_groups, 
                                           uint32_t          *attr_index)
{
   return(_check_particular_group_present(IPP_TAG_JOB_ATTRIBUTES,
                                             attr_groups,
                                             attr_index));
}

void _reset_attr_tbl(attr_node_t *attr_tbl_ptr, uint32_t attr_tbl_size)
{
   uint32_t i;

   for (i = 0; i < attr_tbl_size; ++i)
   {
      attr_tbl_ptr[i].attr_found = false;
      attr_tbl_ptr[i].attr_index = -1;
   }
}

void _populate_attrs_that_are_present(ipp_attr_grp_t *oper_attr_group,
                                      attr_node_t    *attr_tbl_ptr, 
                                      uint32_t       attr_tbl_size)
{
   uint32_t   i;
   uint32_t   j;
   ipp_attr_t *current_attribute = NULL;
   uint32_t   num_attributes     = oper_attr_group->num_attr;

   for (i = 0; i < num_attributes; ++i)
   {
      current_attribute = &oper_attr_group->attr[i];

      for (j = 0; j < attr_tbl_size; ++j)
      {
         // If the name has already been found, just skip the compare.
         if (attr_tbl_ptr[j].attr_found)
         {
            continue;
         }

         if (!strcmp(current_attribute->name, attr_tbl_ptr[j].attr_name))
         {
            attr_tbl_ptr[j].attr_found = true;
            attr_tbl_ptr[j].attr_index = i;
            break;
         }
      }
   }
}

bool _check_all_attributes_found(attr_node_t *attr_tbl, uint32_t attr_tbl_size)
{
   uint32_t i;
   bool     all_attr_found = true;

   for (i = 0; i < attr_tbl_size; ++i)
   {
      if (!attr_tbl[i].attr_found)
      {
         all_attr_found = false;
         break;
      }
   }

   return(all_attr_found);
}

bool _check_all_req_cups_get_printers_present(ipp_attr_grp_t *oper_attr_group)
{
   uint32_t i;
   bool     valid_oper_attr = true;

   _reset_attr_tbl(&req_cups_get_printers_tbl[0], req_cups_get_printers_tbl_size);

   _populate_attrs_that_are_present(oper_attr_group,
                                    &req_cups_get_printers_tbl[0], 
                                    req_cups_get_printers_tbl_size);

   // Now go through the required operation atributes table and make sure that all 
   // the attributes have been found.
   for (i = 0; i < req_cups_get_printers_tbl_size; ++i)
   {
      if (!req_cups_get_printers_tbl[i].attr_found)
      {
         valid_oper_attr = false;
         break;
      }
   }

   if (valid_oper_attr)
   {
         for (i = 0; i < req_cups_get_printers_tbl_size; ++i)
         {
            if (req_cups_get_printers_tbl[i].attr_index != i)
            {
               valid_oper_attr = false;
               break;
            }
         }
   }

   return(valid_oper_attr);
}

bool _check_all_req_get_print_attr_present(ipp_attr_grp_t *oper_attr_group)
{
   uint32_t i;
   bool     valid_oper_attr = true;

   _reset_attr_tbl(&req_get_print_attr_tbl[0], req_get_print_attr_tbl_size);

   _populate_attrs_that_are_present(oper_attr_group,
                                    &req_get_print_attr_tbl[0], 
                                    req_get_print_attr_tbl_size);

   // Now go through the required operation atributes table and make sure that all 
   // the attributes have been found.
   for (i = 0; i < req_get_print_attr_tbl_size; ++i)
   {
      if (!req_get_print_attr_tbl[i].attr_found)
      {
         valid_oper_attr = false;
         break;
      }
   }

   if (valid_oper_attr)
   {
         for (i = 0; i < req_get_print_attr_tbl_size; ++i)
         {
            if (req_get_print_attr_tbl[i].attr_index != i)
            {
               valid_oper_attr = false;
               break;
            }
         }
   }

   return(valid_oper_attr);
}

bool _check_all_req_print_job_attr_present(ipp_attr_grp_t *oper_attr_group)
{
   uint32_t i;
   bool     valid_oper_attr = true;
   

   _reset_attr_tbl(&req_print_job_oper_attr_tbl[0], req_print_job_oper_attr_tbl_size);

   _populate_attrs_that_are_present(oper_attr_group,
                                    &req_print_job_oper_attr_tbl[0], 
                                    req_print_job_oper_attr_tbl_size);

   // Now go through the required operation atributes table and make sure that all 
   // the attributes have been found.
   for (i = 0; i < req_print_job_oper_attr_tbl_size; ++i)
   {
      if (!req_print_job_oper_attr_tbl[i].attr_found)
      {
         valid_oper_attr = false;
         break;
      }
   }

   return(valid_oper_attr);
}

bool _check_opt_attr_valid(ipp_attr_grp_t *oper_attr_group,
                           attr_node_t    *attr_tbl_ptr, 
                           uint32_t       attr_tbl_size)
{
   uint32_t   i;
   uint32_t   j;
   ipp_attr_t *current_attribute = NULL;
   uint32_t   num_attributes     = oper_attr_group->num_attr;
   bool       attr_in_list;
   bool       opt_attr_valid     = true;

   for (i = 0; i < num_attributes; ++i)
   {
      current_attribute = &oper_attr_group->attr[i];
      attr_in_list      = false;

      for (j = 0; j < attr_tbl_size; ++j)
      {
         // If the name has already been used, just skip the compare.
         if (attr_tbl_ptr[j].attr_found)
         {
            continue;
         }

         if (!strcmp(current_attribute->name, attr_tbl_ptr[j].attr_name))
         {
            attr_tbl_ptr[i].attr_found = true;
            attr_tbl_ptr[i].attr_index = i;
            attr_in_list               = true;
            break;
         }
      }

      if (!attr_in_list)
      {
         opt_attr_valid = false;
         //dbg_printf("%s - job template attribute: %s not present in list.\n", __func__,
         //           current_attribute->name);
         break;
      }
   }

   return(opt_attr_valid);
}

bool _check_print_job_opt_job_attr(ipp_attr_grp_t *oper_attr_group)
{
   _reset_attr_tbl(&opt_print_job_oper_attr_tbl[0], opt_print_job_oper_attr_tbl_size);

   return(_check_opt_attr_valid(oper_attr_group,
                                &opt_print_job_oper_attr_tbl[0],
                                opt_print_job_oper_attr_tbl_size));
}


bool _check_all_req_send_document_attr_present(ipp_attr_grp_t *oper_attr_group)
{
   bool valid_oper_attr;
   
   _reset_attr_tbl(&req_send_document_attr_tbl[0], 
                   req_send_document_attr_tbl_size);

   _populate_attrs_that_are_present(oper_attr_group,
                                    &req_send_document_attr_tbl[0],
                                    req_send_document_attr_tbl_size);

   valid_oper_attr = _check_all_attributes_found(req_send_document_attr_tbl, 
                                                 req_send_document_attr_tbl_size);

   if (valid_oper_attr)
   {
      _reset_attr_tbl(&req_send_doc_attr_target1_tbl[0], 
                      req_send_doc_attr_target1_tbl_size);

      _populate_attrs_that_are_present(oper_attr_group,
                                       &req_send_doc_attr_target1_tbl[0],
                                       req_send_doc_attr_target1_tbl_size);


      valid_oper_attr = _check_all_attributes_found(req_send_doc_attr_target1_tbl, 
                                                    req_send_doc_attr_target1_tbl_size);

      if (valid_oper_attr)
      {
         return(valid_oper_attr);
      }
      _reset_attr_tbl(&req_send_doc_attr_target2_tbl[0], 
                         req_send_doc_attr_target2_tbl_size);

      _populate_attrs_that_are_present(oper_attr_group,
                                       &req_send_doc_attr_target2_tbl[0],
                                       req_send_doc_attr_target2_tbl_size);

      valid_oper_attr = _check_all_attributes_found(req_send_doc_attr_target2_tbl, 
                                                       req_send_doc_attr_target2_tbl_size);
   }

   return(valid_oper_attr);
}

