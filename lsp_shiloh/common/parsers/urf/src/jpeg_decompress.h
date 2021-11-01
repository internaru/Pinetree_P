/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file jpeg_decompress.h
 *
 * \brief JPEG decompression
 *
 */
#include "urf.h"
#include "urfio.h"

#define BITS_PER_BYTE 8
#define MIN_STRIP_HEIGHT_MULTIPLE 4

int jpeg_decoder_open( urf_bstream_t *s, urf_page_t *urf_page );

void jpeg_decoder_close( urf_bstream_t *s );

int jpeg_decompress(urf_bstream_t *s,  
           urf_page_t *urf_page, print_page_t* p_page);

void process_strip(urf_bstream_t *s,
           urf_page_t *urf_page, print_page_t *p_page, 
	   uint8_t *data, uint32_t image_width, 
	   uint32_t image_height, uint32_t strip_rows,
           uint32_t datalen, bool last_strip);
