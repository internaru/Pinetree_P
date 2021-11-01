/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_FAXHEADER_H__
#define __INC_FAXHEADER_H__

#include "date_time_api.h"

typedef enum
{
  FAX_HEADER_TYPE_MONO,
  FAX_HEADER_TYPE_GRAYSCALE,
  FAX_HEADER_TYPE_RGB,
  FAX_HEADER_TYPE_RGBX,
} fax_header_type_t;

typedef enum
{
  FAX_HEADER_FORMAT_DATE, 
  FAX_HEADER_FORMAT_DATE_AND_TIME,
} fax_header_date_format_t;

void *fax_header_build(int stride, int x_res, int y_res, fax_header_type_t type, int page,
                       char *name, char *number, struct tm *time, 
                       fax_header_date_format_t format);
void fax_header_get_data(void *handle, uint8_t ** data, int *rows);
void fax_header_destroy(void *handle);

#endif
