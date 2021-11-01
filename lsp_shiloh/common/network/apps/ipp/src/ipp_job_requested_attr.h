/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_JOB_REQUESTED_ATTR_H__
#define __INC_IPP_JOB_REQUESTED_ATTR_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
   JOB_PRIORITY = 0,
   JOB_HOLD_UNTIL,
   JOB_SHEETS,
   MULTIPLE_DOCUMENT_HANDLING,
   COPIES,
   FINISHINGS,
   PAGE_RANGES,
   SIDES,
   NUMBER_UP,
   ORIENTATION_REQUESTED,
   MEDIA,
   PRINTER_RESOLUTION,
   PRINT_QUALITY,
   JOB_URI,
   JOB_ID,
   JOB_PRINTER_URI,
   JOB_NAME,
   JOB_ORIGINATING_USER_NAME,
   JOB_STATE,
   JOB_STATE_REASONS,
   TIME_AT_CREATION,
   TIME_AT_PROCESSING,
   TIME_AT_COMPLETED,
   JOB_PRINTER_UP_TIME,
   JOB_UUID,
   JOB_IMPRESSIONS,
   JOB_IMPRESSIONS_COMPLETED,
   
   // Keep this as the last value.
   NUM_REQ_JOB_ATTRIBUTES,

} requested_job_attributes_e;

void generate_req_job_attr_tbl();
void set_job_attributes_requested(ipp_requested_attrs_t *requested_attributes,
                                  bool jobs_operation);
bool has_job_attr_been_requested(requested_job_attributes_e job_attr);

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_JOB_REQUESTED_ATTR_H__ */

