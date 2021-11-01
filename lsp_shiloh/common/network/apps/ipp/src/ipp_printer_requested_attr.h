/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_PRINTER_REQUESTED_ATTR_H__
#define __INC_IPP_PRINTER_REQUESTED_ATTR_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
   PRINTER_URI_SUPPORTED = 0,
   PRINTER_SUPPLY_INFO_URI,
   URI_SECURITY_SUPPORTED,
   URI_AUTHENTICATION_SUPPORTED,
   IDENTIFY_ACTIONS_DEFAULT,
   IDENTIFY_ACTIONS_SUPPORTED,
   PRINTER_NAME,
   COLOR_SUPPORTED,
   PRINT_COLOR_MODE_DEFAULT,
   PRINT_COLOR_MODE_SUPPORTED,
   PRINTER_STATE,
   PRINTER_STATE_REASONS,
   IPP_VERSIONS_SUPPORTED,
   OPERATIONS_SUPPORTED,
   CHARSET_CONFIGURED,
   CHARSET_SUPPORTED,
   NATURAL_LANGUAGE_CONFIGURED,
   GENERATED_NATURAL_LANGUAGE_SUPPORTED,
   DOCUMENT_FORMAT_DEFAULT,
   DOCUMENT_FORMAT_SUPPORTED,
   JOB_CREATION_ATTRIBUTES_SUPPORTED,
   PRINTER_IS_ACCEPTING_JOBS,
   QUEUED_JOB_COUNT,
   PDL_OVERRIDE_SUPPORTED,
   PRINTER_UP_TIME,
   PRINTER_MAKE_AND_MODEL,
   PRINTER_LOCATION,
   PRINTER_INFO,
   PRINTER_DEVICE_ID,
   PRINTER_MORE_INFO,
   OPEN_SPACE,
   PRINTER_DNS_SD_NAME,
   COMPRESSION_SUPPORTED,
   JOB_HOLD_UNTIL_DEFAULT,
   MULTIPLE_DOCUMENT_JOBS_SUPPORTED,
   MULTIPLE_OPERATION_TIME_OUT,
   JOB_HOLD_UNTIL_SUPPORTED,
   JOB_SHEETS_DEFAULT,
   JOB_SHEETS_SUPPORTED,
   PAGES_PER_MINUTE,
   PAGES_PER_MINUTE_COLOR,
   COPIES_DEFAULT,
   COPIES_SUPPORTED,
   JPEG_X_DIMENSION_SUPPORTED,
   JPEG_Y_DIMENSION_SUPPORTED,
   LANDSCAPE_ORIENTATION_REQUESTED_PREFERRED,
   FINISHINGS_DEFAULT,
   FINISHINGS_SUPPORTED,
   PAGE_RANGES_SUPPORTED,
   SIDES_DEFAULT,
   SIDES_SUPPORTED,
   NUMBER_UP_DEFAULT,
   NUMBER_UP_SUPPORTED,
   ORIENTATION_REQUESTED_DEFAULT,
   ORIENTATION_REQUESTED_SUPPORTED,
   MEDIA_DEFAULT,
   MEDIA_SUPPORTED,
   MEDIA_TYPE_SUPPORTED,
   MEDIA_SOURCE_SUPPORTED,
   MEDIA_READY,
   PRINTER_RESOLUTION_DEFAULT,
   PRINTER_RESOLUTION_SUPPORTED,
   PRINT_QUALITY_DEFAULT,
   PRINT_QUALITY_SUPPORTED,
   PRINTER_UUID,
   JPEG_K_OCTETS_SUPPORTED,
   PDF_K_OCTETS_SUPPORTED,
   MEDIA_COL_SUPPORTED,
   MEDIA_LEFT_MARGIN_SUPPORTED,
   MEDIA_RIGHT_MARGIN_SUPPORTED,
   MEDIA_TOP_MARGIN_SUPPORTED,
   MEDIA_BOTTOM_MARGIN_SUPPORTED,
   OUTPUT_BIN_DEFAULT,
   OUTPUT_BIN_SUPPORTED,
   MEDIA_COL_DEFAULT,
   MEDIA_COL_DATABASE,
   MEDIA_SIZE_SUPPORTED,
   MEDIA_COL_READY,
   PRINTER_ICONS,
   IPP_FEATURES_SUPPORTED,
   PRINT_CONTENT_OPTIMIZE_DEFAULT,
   PRINT_CONTENT_OPTIMIZE_SUPPORTED,
   PRINTER_GEO_LOCATION,
   EPCL_VERSION_SUPPORTED,
   MARGINS_PRE_APPLIED,
   PCLM_COMPRESSION_METHOD_PREFERRED,
   PCLM_RASTER_BACK_SIDE,
   PCLM_SOURCE_RESOLUTION,
   PCLM_SOURCE_RESOLUTION_SUPPORTED,
   PCLM_STRIP_HEIGHT_PREFERRED,
   PCLM_STRIP_HEIGHT_SUPPORTED,

   // Keep this as the last value.
   NUM_REQ_PRINTER_ATTRIBUTES,

} requested_printer_attributes_e;

void generate_req_printer_attr_tbl();
void set_printer_attributes_requested(ipp_requested_attrs_t *requested_attributes);
bool has_printer_attr_been_requested(uint8_t print_attr);

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_PRINTER_REQUESTED_ATTR_H__ */
