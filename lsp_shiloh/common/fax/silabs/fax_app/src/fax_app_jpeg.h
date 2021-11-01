/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef FAX_APP_JPEG_H_
#define FAX_APP_JPEG_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "jpeglib.h"
#include "fax_store_api.h"
#include "paper_size_config.h"
#include "fax_app_api.h"

#define FAX_ENCODED_WIDTH_200 (1728) /* For 200 DPI, must be exactly this width, see T.4 table 1...most machines will
                                       not print if another width */
#define FAX_ENCODED_WIDTH_300 (2592) /* For 300 DPI, must be exactly this width, see T.4 table 1...most machines will
                                       not print if another width */

void fax_app_jpeg_start_compress(struct jpeg_compress_struct *cinfo, fax_app_input_data_format_t src_format, int width, int height, int res,
                                 fax_store_handle_t store );

void fax_app_jpeg_write(struct jpeg_compress_struct *cinfo, unsigned char *data, int rows);

int fax_app_jpeg_finish_compress(struct jpeg_compress_struct *cinfo);

int fax_app_jpeg_destroy_compress(struct jpeg_compress_struct *cinfo);


int fax_app_jpeg_start_decompress(struct jpeg_decompress_struct *cinfo, fax_store_pg_handle_t page, uint32_t *stride,
                                  mediasize_t media_size );

void fax_app_jpeg_get_output_dimensions(struct jpeg_decompress_struct *cinfo, uint32_t *pix_width, uint32_t *pix_height);
void fax_app_jpeg_get_output_components(struct jpeg_decompress_struct *cinfo, int *comps);
void fax_app_jpeg_get_output_resolution(struct jpeg_decompress_struct *cinfo, uint16_t *res);

int fax_app_jpeg_read_data_scanline(struct jpeg_decompress_struct *cinfo, uint8_t **buff, bool *completed);

void fax_app_jpeg_end_decompress(struct jpeg_decompress_struct *cinfo);

#endif
