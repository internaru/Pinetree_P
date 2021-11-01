/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_VALIDATE_REQUEST_H__
#define __INC_IPP_VALIDATE_REQUEST_H__

#include "ipp_attribute.h"

#ifdef __cplusplus
extern "C" {
#endif

bool ipp_chk_operation_attr_tag_present(ipp_attr_groups_t *attr_groups, 
                                        uint32_t *attr_index);
bool ipp_chk_job_template_attr_tag_present(ipp_attr_groups_t *attr_groups, 
                                           uint32_t *attr_index);
bool ipp_chk_valid_cups_get_printers_req(ipp_attr_groups_t *attr_groups);
bool ipp_chk_valid_get_print_attr_req(ipp_attr_groups_t *attr_groups);
bool ipp_chk_valid_print_job_attr_req(ipp_attr_groups_t *attr_groups);
bool ipp_chk_valid_send_document_attr_req(ipp_attr_groups_t *attr_groups);
bool ipp_chk_valid_document_addtion(ipp_attr_groups_t *attr_groups, 
                                    smjob_status_t *job_status, bool *add_the_document);

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_VALIDATE_REQUEST_H__ */

