
/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file jpeg_decompress.c
 *
 * \brief JPEG decompression
 *
 */

#include "string.h"
#include "tx_api.h"
#include "cpu_api.h"
#include "agRouter.h"
#include "agConnectMgr.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "ResourceMap.h"
#include "SJMgr.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "print_job_api.h"
#include "memAPI.h"
#include "lassert.h"
#include "logger.h"
#include "PrintJobInfo.h"
#include "cp_strip.h"
#include "urf.h"
#include "urfio.h"
#include "jpeg_decompress.h"
#include "jpeg_hw_driver.h"
		
#define DATA_CHUNK_SIZE 10240
#define JPEG_DECOMPRESSOR_STRIP_HEIGHT 128
#define PRINT_STRIP_HEIGHT 128

uint8_t * read_jpeg_file( mlimiter_t *limiter, urf_bstream_t *s, uint32_t *jpeg_datalen )
{
    uint8_t *jpeg_data, *pjpeg_data;
    uint32_t bytes_read=DATA_CHUNK_SIZE;
    *jpeg_datalen =0;

    //dbg_printf("read_jpeg_file...\n");
    jpeg_data = (uint8_t*)MEM_MALLOC_ALIGN_LIMITED(limiter, DATA_CHUNK_SIZE,
					  cpu_get_dcache_line_size());

    while(bytes_read == DATA_CHUNK_SIZE)
    {
	//dbg_printf("bstream_read...\n");
	pjpeg_data = jpeg_data + *jpeg_datalen;
        bytes_read = s->Pipe->pipe_read(s->Pipe, pjpeg_data, 
					DATA_CHUNK_SIZE, PARSEREAD_EXACT);
	*jpeg_datalen += bytes_read;

	if(bytes_read == DATA_CHUNK_SIZE)
	{
	    uint32_t new_size = *jpeg_datalen + DATA_CHUNK_SIZE;
	    jpeg_data = MEM_REALLOC_LIMITED(limiter, jpeg_data, new_size);
	    ASSERT(jpeg_data != NULL);
	}
    }

    return jpeg_data;
}

int jpeg_decoder_open( urf_bstream_t *s, urf_page_t *urf_page )
{
    uint32_t jpeg_datalen=0;
    uint32_t media_width;
    uint32_t media_height;

    //dbg_printf("%s\n", __FUNCTION__);

    // save the desired media size
    media_width = urf_page->width;
    media_height = urf_page->height;

    //mlimiter_t *limiter = mlimiter_by_name("jpeg"); 
    mlimiter_t *limiter = mlimiter_by_name("print"); 
    mlimiter_start(limiter);

    // rewind the pipe in case anything was read during
    // the uel search in unirast.c
    urf_bstream_rewind(s);
    uint8_t *jpeg_data = read_jpeg_file( limiter, s, &jpeg_datalen );
    urf_bstream_rewind(s);

    image_info *info;

    info = jpegdrvr_open(limiter, jpeg_data, jpeg_datalen);

    if(jpegdrvr_parse_header(info) == JPEG_DRVR_FAILURE)
    {
	dbg_printf("JPEG file contains unsupported or corrupt data\n");
	ASSERT(0);
    }
    jpegdrvr_program_hw(info);

    s->client_data = info;

    urf_page->rotate_image=false; 

    if(info->image_width > info->image_height &&
       (urf_page->orientation == e_portrait || 
        urf_page->orientation == e_auto))
    {
	urf_page->rotate_image = true;
    }

    // calculate the non-scaled page dimensions
    if(urf_page->rotate_image)
    {
	urf_page->height = info->image_width;
	urf_page->height -= urf_page->height % MIN_STRIP_HEIGHT_MULTIPLE;
	urf_page->width = info->mcu_aligned_image_height;
    }
    else
    {
	urf_page->width = info->mcu_aligned_image_width;
	urf_page->height = info->mcu_aligned_image_height;
    }
    
    // calculate the scaled page dimensions
    if(urf_page->scale_image)
    {
	// determine the dominant dimension
	if(urf_page->width > urf_page->height)
	{
	    // width is dominant, calculate height
	    urf_page->height = (urf_page->height * media_width) /
		                        urf_page->width;
	    urf_page->width = media_width;
	}
	else
	{
	    // height is dominant, calculate width
	    urf_page->width = (urf_page->width * media_height) /
		                        urf_page->height;
	    urf_page->height = media_height;
	}
	urf_page->width -= BUFFER_DMA_MOD; // allow for dma padding
    }

    // adjust width for dma alignment
    if(urf_page->width % BUFFER_DMA_MOD != 0)
    {
	urf_page->width += BUFFER_DMA_MOD - 
	                 (urf_page->width % BUFFER_DMA_MOD);
    }

    urf_page->bitsPerPixel = info->num_planes_m1 * BITS_PER_BYTE;

    if(urf_page->scale_image)
    {
	// calculate the prescaled strip height
	if(urf_page->rotate_image)
	{
	    urf_page->stripHeight = (PRINT_STRIP_HEIGHT * info->image_width)
	                             / urf_page->height;
	}
	else
	{
	    urf_page->stripHeight = (PRINT_STRIP_HEIGHT * info->image_height)
	                             / urf_page->height;
	}

	if(urf_page->stripHeight < 32)
	{
	    urf_page->stripHeight = 32;
	}
    }
    else
    {
	urf_page->stripHeight = PRINT_STRIP_HEIGHT;
    }

    return 1;
}

void jpeg_decoder_close( urf_bstream_t *s )
{
    image_info *info = (image_info *)s->client_data;
    mlimiter_stop(info->mlimiter);
    MEM_FREE_AND_NULL(info->jpeg_data);
    jpegdrvr_close(info);
}

int jpeg_decompress(urf_bstream_t *bstream,  
           urf_page_t *urf_page, print_page_t* p_page)
{
    uint32_t strip_height = urf_page->stripHeight;

    image_info *info = (image_info *)bstream->client_data;

    // make sure strip_height is a multiple of MCU height
    if(strip_height % info->mcu_height != 0)
    {
	strip_height -= strip_height % info->mcu_height;
    }

    // printer requires at least two strips
    ASSERT(strip_height > info->mcu_height);
    if(info->mcu_aligned_image_height <= strip_height)
    {
	strip_height = info->mcu_aligned_image_height -
	         info->mcu_height;
    }

    DBG_PRINTF_NOTICE(" jpeg parser HW decompress\n");
    //dbg_printf("mcu %dx%d mcu aligned %dx%d\n", info->mcu_width, info->mcu_height,
	       //info->mcu_aligned_image_width, info->mcu_aligned_image_height);
    //dbg_printf("urf %dx%d \n", urf_page->width, urf_page->height);


    uint32_t total_bytes_read=0, total_bytes_needed=0;
    uint8_t *data=0;
    uint32_t total_rows=0;
    uint32_t padded_byte_width;
    uint32_t image_bytes_per_pixel = urf_page->bitsPerPixel / BITS_PER_BYTE;
    uint32_t dst_strip_counter = 0;
    uint32_t last_row;
    uint32_t image_width;  // image width (after rotation if needed)
    uint32_t image_height;  // imgae height (after rotation if needed)

    padded_byte_width = image_bytes_per_pixel * info->dma_aligned_image_width;
    total_bytes_needed = padded_byte_width * info->mcu_aligned_image_height;

    bool last_strip=false;
    do
    {
	uint32_t rows_wanted=0;
	uint32_t bytes_read;
	uint32_t rows_read=0;

	last_row = info->mcu_aligned_image_height;

	rows_wanted = strip_height;
	if(total_bytes_needed - total_bytes_read < padded_byte_width * strip_height)
	{
	    rows_wanted = (total_bytes_needed - total_bytes_read) / padded_byte_width;
	}
	// make sure rows_wanted is a multiple of MCU height
	if(rows_wanted % info->mcu_height != 0)
	{
	    rows_wanted -= rows_wanted % info->mcu_height;
	}

	image_width = info->dma_aligned_image_width;
	image_height = info->mcu_aligned_image_height;

	if(urf_page->rotate_image)
	{
	    // when rotating the decompress strip height does not have
	    // to match the print strip height
	    rows_wanted = JPEG_DECOMPRESSOR_STRIP_HEIGHT;

            #define INPUT_MCU_HEIGHT 20
	    uint8_t *rotdata=0, *srcdata=0;
	    uint32_t tmp_total_rows=0;
	    uint32_t ih = info->mcu_aligned_image_height;
	    uint32_t iw = info->dma_aligned_image_width;
	    uint32_t rh = info->image_width -    // rotated height
		         (info->image_width % MIN_STRIP_HEIGHT_MULTIPLE);
	    uint32_t rw = info->mcu_aligned_image_height +  // rotated width (DMA padded)
		BUFFER_DMA_MOD - (info->mcu_aligned_image_height % BUFFER_DMA_MOD);
	    uint32_t get_rows=0;        // requested input strip height
	    uint32_t got_rows=0;        // actual input strip height
	    uint32_t ish=0;             // input strip height
	    uint32_t rsh = strip_height;   // rotated strip height
	    uint32_t bpp = info->num_planes_m1;
	    int32_t src=0;   // index within buffer of src data chunk
	    int32_t dst=0;   // index within buffer of dst data chunk
	    uint32_t src_strip_counter=0;  // keep count of the src strips
	    uint8_t *src_byte_loc=0; // src location within the buffer
	    uint8_t *dst_byte_loc=0; // dst location within the buffer

	    padded_byte_width = image_bytes_per_pixel * info->dma_aligned_image_width;
	    total_bytes_needed = padded_byte_width * rh;

	    last_row = rh; // last row is the rotated height

	    uint32_t cache_aligned_num_bytes = 
	         info->dma_aligned_image_width *
		 rows_wanted * info->num_planes_m1;

	    rotdata = (uint8_t*)mem_retry_malloc_align_limited( 
		info->mlimiter, cache_aligned_num_bytes, 
		cpu_get_dcache_line_size(), __FILE__, __LINE__);
	    ASSERT(rotdata != 0);

	    src_strip_counter = 0;

	    get_rows = INPUT_MCU_HEIGHT*info->mcu_height;
	    ish = get_rows;

	    // grab all the collumns of the input image and place them
	    // as rows in the output image
	    while(tmp_total_rows < ih)
	    {
		uint32_t ir; // in row counter
		uint32_t ic; // in column counter

		// if this is the final strip and the desired row count
		// exceeds the number of rows left in the image, adjust
		// the row count to equal the remaining rows
		if(ih - tmp_total_rows < get_rows)
		{
		    get_rows = ih - tmp_total_rows;
		}
		
		// get a strip of input data
		//srcdata = jpegdrvr_get_data(info, get_rows, &bytes_read);
		srcdata = 0;
		jpegdrvr_get_data(info, &srcdata, get_rows, &bytes_read);
		got_rows = bytes_read / (iw * bpp);
		tmp_total_rows += got_rows;

		// rotate the data from the input strip to the output strip
		uint32_t s,d;
		for(ir = 0; ir < got_rows; ir++)
		{
		    src = ir * (iw * bpp) + (rsh * bpp * dst_strip_counter);
		    dst = (rw - ir) * bpp - bpp - (ish * bpp * src_strip_counter);
		    
		    // put some boundary conditions on the memory locations
		    if(dst < 0 || src > iw * ih * bpp)
		    {
			break;
		    }

		    for(ic = 0; ic < rows_wanted; ic++)
		    {
		        src_byte_loc = srcdata + src + (ic * bpp);
		        dst_byte_loc = rotdata + dst + (ic * rw * bpp);
		        s = src + (ic * bpp);
		        d = dst + (ic * rw * bpp);

		        memcpy(dst_byte_loc, src_byte_loc, bpp);
		    }
		}

		src_strip_counter++;
		MEM_FREE_AND_NULL(srcdata);
	    }

	    // update read counters
	    rows_read = rsh; 
	    bytes_read = rows_read * rw * bpp;
	    total_bytes_read += bytes_read;
	    total_rows += rows_read;
	    //dbg_printf("---total_rows:%d\n", total_rows);

	    // if we rotated more rows that what the rotation height
	    // calls for just trunc the read values
	    if(total_rows > rh)
	    {
		// adjust read values to actually what was read
		rows_read -= total_rows - rh;
		bytes_read = rows_read * rw * bpp;
	    }

	    data = rotdata;
	    dst_strip_counter++;

	    // if rotation did not fill the leading edge with data
	    // due to dma padding on input rows, fill in the leading
	    // data on the rotated rows with white space
	    if(dst > 1)
	    {
		uint32_t rr;
		uint8_t pad_data=0xff;
		//if(bpp > 1) pad_data=0xff;

		// pad the leading edge of rotated image with white
		for(rr = 0; rr < rows_wanted; rr++)
		{
		    dst_byte_loc = rotdata + (rr * rw * bpp);
		    memset(dst_byte_loc, pad_data, dst -1);
		}
	    }
	    
	    image_width = rw;
	    image_height = rh;

	    jpegdrvr_reset_input(info);
	}
	else
	{
	    // non-rotated image, just get the data and pass it along
	    //data = jpegdrvr_get_data(info, rows_wanted, &bytes_read);
	    data = 0;
	    jpegdrvr_get_data(info, &data, rows_wanted, &bytes_read);
	    if(bytes_read == 0)
	    {
		DBG_PRINTF_NOTICE("%s. Got zero data\n", __FUNCTION__);
		continue;
	    }

	    total_bytes_read += bytes_read;
	    rows_read = bytes_read / padded_byte_width;
	    total_rows += rows_read;
	}

	if(total_rows >= last_row)
	{
	    DBG_PRINTF_NOTICE(" last strip!!\n");
	    last_strip = true;
	}

	//dbg_printf("---total_rows %d last_row %d\n", total_rows, last_row);

        process_strip(bstream, urf_page, p_page, data, image_width, 
		      image_height, rows_read, bytes_read, last_strip);

    } while(! last_strip);

    DBG_PRINTF_NOTICE("\njpeg finished reading total_rows:%d\n", total_rows);

    return 0;
}
