/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_HANDLE_ATTR_REQS_H__
#define __INC_IPP_HANDLE_ATTR_REQS_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "ipp_attribute.h"

//=========================================================================================
// Typedefs
//=========================================================================================
typedef struct requested_attribute_info_s
{
   char     *attr_name;
   uint32_t hash_of_attr_name;
   bool     check_attr_with_hash;
   bool     reset_attr_on_new_req;
   bool     attr_was_requested;

} requested_attribute_info_t;


typedef void (*ret_default_attributes)();

void ipp_set_attributes_requested(requested_attribute_info_t *req_attr_info_tbl,
                                  uint32_t  size_req_attr_info_tbl,
                                  ipp_requested_attrs_t *requested_attributes,
                                  ret_default_attributes set_default_attr_ret_func);
void ipp_init_attr_hash_tbl(requested_attribute_info_t *req_attr_info_tbl,
                            uint32_t size_req_attr_info_tbl);
uint32_t ipp_hash_attr_name(const char *attr_name);

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_HANDLE_ATTR_REQS_H__ */

