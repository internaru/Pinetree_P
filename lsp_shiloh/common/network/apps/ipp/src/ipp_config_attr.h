/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_CONFIG_ATTR_H__
#define __INC_IPP_CONFIG_ATTR_H__


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PRINT_JOB_CFG_ATTR_LEN 64

typedef struct ipp_print_job_cfg_attr_s
{
   char              str_identifier[MAX_PRINT_JOB_CFG_ATTR_LEN];
   uint32_t          hash_of_str;
   void              *data_ptr;
   uint32_t          data_val;
   uint32_t          config_data_len;
   bool              config_val_set;

} ipp_print_job_cfg_attr_t;

void ipp_add_req_attr_to_job_ticket(ipp_attr_groups_t *attr_groups,
                                    smjob_ticket_t    *job_ticket);
void ipp_add_req_attr_to_doc_ticket(ipp_attr_groups_t  *attr_groups,
                                    smjob_doc_ticket_t *doc_ticket);

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_CONFIG_ATTR_H__ */

