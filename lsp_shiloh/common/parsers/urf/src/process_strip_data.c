
/******************************************************************************
 * Copyright (c) 2010-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file process_strip_data.c
 *
 * \brief process strip data and send to printer
 *
 */
#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"
#include "cp_strip.h"
#include "print_job_types.h"
#include "print_job_api.h"
#include "urf.h"
#include "jpeg_decompress.h"
#include "jpeg_hw_driver.h"
#include "dma_buffer.h"

#define DBG_PRFX "urf: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

static uint32_t rows_printed = 0;

void scale_strip(image_info *info,
		 urf_page_t *urf_page, uint8_t **data, uint32_t *datalen,
		 uint32_t image_width, uint32_t image_height, 
		 uint32_t *strip_rows)
{
    uint32_t bpp = urf_page->bitsPerPixel/BITS_PER_BYTE;
    uint32_t new_strip_rows = 0;
    uint32_t new_datalen = 0;
    uint32_t h_int_part=0, h_fract_part=0;
    uint32_t v_int_part=0, v_fract_part=0;
    uint32_t r=0, rsrc=0, oldrsrc=0, c=0, csrc=0, e=0;
    uint32_t src=0, dst=0;
    uint8_t *pdata=0, *pnew_data=0;

    new_strip_rows = (*strip_rows * urf_page->height)/image_height;

    // scale slightly large and let print clip the image
    // another solution is scale normally and pad the end
    // of the image with white space
    if(new_strip_rows % MIN_STRIP_HEIGHT_MULTIPLE)
    {
	new_strip_rows += MIN_STRIP_HEIGHT_MULTIPLE
		 - (new_strip_rows % MIN_STRIP_HEIGHT_MULTIPLE);
    }

    new_datalen = new_strip_rows * urf_page->width * bpp;

    uint8_t *new_data = 
	 (uint8_t*)mem_retry_malloc_align_limited( info->mlimiter,
	     new_datalen, cpu_get_dcache_line_size(), __FILE__, __LINE__);
    ASSERT(new_data != NULL);

    // use a simple Bresenham scaler
    h_int_part = image_width / urf_page->width;
    h_fract_part = image_width % urf_page->width;
    v_int_part = *strip_rows / new_strip_rows;
    v_fract_part = *strip_rows % new_strip_rows;

    //dbg_printf("scale strip from %d to %d\n", image_width, urf_page->width);
    //dbg_printf("scale strips from %d to %d(%d)\n", 
	       //*strip_rows, new_strip_rows, urf_page->height);

    for(r=0, rsrc=0; r<new_strip_rows; r++)
    {
	// copy pixels duplicating or removing pixels
	// as necessary
	for(c=0, csrc=0; c<urf_page->width; c++)
	{
	    src = (csrc * bpp) + (rsrc * image_width * bpp);
	    dst = (c * bpp) + (r * urf_page->width * bpp);
	    ASSERT(src < *datalen);
	    ASSERT(dst < new_datalen);

	    pdata = *data + src;
	    pnew_data = new_data + dst;
	    memcpy(pnew_data, pdata, bpp);

	    csrc += h_int_part;
	    e += h_fract_part;
	    if(e >= urf_page->width)
	    {
		e -= urf_page->width;
		csrc++;
	    }
	}
	oldrsrc = rsrc;
	rsrc += v_int_part;
	e += v_fract_part;
	if(e >= new_strip_rows)
	{
	    e -= new_strip_rows;
	    rsrc++;
	}
	if(oldrsrc == rsrc)
	{
	    // duplicate last row to speed things up
	    // save a trip through the above pixel copies
	    r++;
	    if(r < new_strip_rows)
	    {
	        src = (r-1) * urf_page->width * bpp;
	        dst = r * urf_page->width * bpp;
		pdata = new_data + src;
		pnew_data = new_data + dst;
		memcpy(pnew_data, pdata, urf_page->width * bpp);
		e += v_fract_part;
	    }
	}
    }
    // TODO: pad end of page if scaling does not reach end of page

    // free the original memory
    MEM_FREE_AND_NULL(*data);

    *data = new_data;
    *datalen = new_datalen;
    *strip_rows = new_strip_rows;
}

void process_strip(urf_bstream_t *s,
                 urf_page_t *urf_page, print_page_t *p_page, 
		 uint8_t *data, uint32_t image_width, 
		 uint32_t image_height, uint32_t strip_rows,
		 uint32_t datalen, bool last_strip)
{
    BigBuffer_t *big_buffer=0;
    PLANE_TYPE plane_type=GRAYSCALE;
    compression_info_t compression_info;
    uint32_t image_bytes_per_pixel = urf_page->bitsPerPixel/BITS_PER_BYTE;
    COLOR color=COLOR_BLACK;
    uint32_t padded_byte_width = urf_page->width * image_bytes_per_pixel;
    bool print_right_to_left=false;

    //dbg_printf("process_strip %dx%d -> %dx%d\n", 
	  //image_width, image_height, urf_page->width, urf_page->height);

    image_info *info = (image_info *)s->client_data;

    if(urf_page->scale_image)
    {
        scale_strip(info, urf_page, &data, &datalen, 
		    image_width, image_height, &strip_rows);
    }

    big_buffer = dma_buffer_adopt(data, datalen);

    if(image_bytes_per_pixel == 1)
    {
	plane_type = GRAYSCALE;
	compression_info.comp_type = COMPTYPE_NONE;
	compression_info.comp_details.rgb.bits_per_pixel = BITS_PER_BYTE;
    }
    else
    {
	plane_type = PACKED_RGB;
	compression_info.comp_type = COMPTYPE_NONE;
	compression_info.comp_details.rgb.bits_per_pixel = BITS_PER_BYTE*3;
    }

    if(rows_printed + strip_rows > urf_page->height)
    {
	// truncate the image to the requested height

	// DAW -- no need to do this, video does it for us
	//strip_rows = urf_page->height - rows_printed;
    }
    rows_printed += strip_rows;

    //dbg_printf("send data to printer T:%d H:%d\n", 
	       //rows_printed, urf_page->height);
    //dbg_printf("  bs:%d pbw:%d bbp:%d strip_rows:%d ls:%d\n",
	       //datalen,padded_byte_width,image_bytes_per_pixel,
	       //strip_rows, last_strip);

    print_plane_add_data( p_page, color, big_buffer, datalen, plane_type, 
		  last_strip, urf_page->resolution, urf_page->resolution,
		  padded_byte_width / image_bytes_per_pixel, 
		  strip_rows, image_bytes_per_pixel * BITS_PER_BYTE,
		  print_right_to_left, &compression_info);

    if(last_strip)
    {
	rows_printed = 0;
    }
}
