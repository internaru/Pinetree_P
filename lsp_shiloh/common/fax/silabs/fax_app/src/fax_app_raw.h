/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef FAX_APP_RAW_H_
#define FAX_APP_RAW_H_

#include <stdint.h>
#include "fax_store_api.h"
#include "fax_app_api.h"

uint32_t fax_app_raw_jpeg_start( fax_store_handle_t store_param, fax_app_resolution_t resolution );

uint32_t fax_app_raw_mono_start( char * filename, fax_app_resolution_t resolution );

uint32_t fax_app_raw_add_page( uint8_t *buf, fax_app_input_data_format_t src_format, 
                               uint32_t pix_width, uint32_t pix_height,
                               bool complete );

uint32_t fax_app_raw_cancel( void );

#endif
