
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
#include "jpeglib.h"
#include "urf.h"
#include "urfio.h"
#include "jpeg_decompress.h"
			  
#define DBG_PRFX "jpeg: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

typedef struct decode_jpeg_s decode_jpeg_t;
struct decode_jpeg_s
{
    urf_bstream_t *stream;

    struct jpeg_decompress_struct cinfo;
    struct jpeg_source_mgr jpeg_src_mgr;
    struct jpeg_error_mgr jpeg_err_mgr;
    //uint32_t byte_width; 
    char msg[JMSG_LENGTH_MAX];
};

static void init_source(j_decompress_ptr cinfo)
{ } // nop

static void term_source(j_decompress_ptr cinfo)
{ } // nop

static void error_exit(j_common_ptr cinfo)
{
    decode_jpeg_t *jpeg;
    jpeg = cinfo->client_data;
    //DBG_PRINTF_ERR("%s %s\n", __FUNCTION__, jpeg->msg );
    // ASSERT( 0 == 1 );
}

static boolean fill_input_buffer( j_decompress_ptr cinfo )
{
    struct jpeg_source_mgr *src = cinfo->src;
    decode_jpeg_t *jpeg = cinfo->client_data;
    urf_bstream_t *s = jpeg->stream;
    uint8_t *ptr;
    int32_t cnt;

    cnt = urf_bstream_read( s, &ptr, URF_STREAM_BUFFER_SIZE );
    if ( 0 >= cnt )
        cnt = 0; // EOF on input 
    
    src->next_input_byte = ptr;
    src->bytes_in_buffer = cnt;

    if (src->bytes_in_buffer == 0)
    {
        static uint8_t jpeg_eoi[2] = { 0xFF, JPEG_EOI };
        //DBG_PRINTF_ERR( "EOF with io timeout inside jpeg_decode %s\n", __FUNCTION__ );
        src->next_input_byte = jpeg_eoi;
        src->bytes_in_buffer = 2;
    }
    return 1;
}

static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    if (num_bytes > 0)
    {
        while (num_bytes > cinfo->src->bytes_in_buffer)
        {
            num_bytes -= cinfo->src->bytes_in_buffer;
            (void) cinfo->src->fill_input_buffer(cinfo);
        }
        cinfo->src->next_input_byte += num_bytes;
        cinfo->src->bytes_in_buffer -= num_bytes;
    }
}

int jpeg_decoder_open( urf_bstream_t *s, urf_page_t *urf_page )
{
    decode_jpeg_t *jpeg = (decode_jpeg_t *) MEM_MALLOC( sizeof(decode_jpeg_t) );
    int32_t cnt;
    uint8_t *ptr;

    memset(jpeg, 0, sizeof(decode_jpeg_t));
    memset(urf_page, 0, sizeof(urf_page_t));

    jpeg->stream = s;
    s->client_data = jpeg;

    jpeg->cinfo.client_data = jpeg;
    jpeg->cinfo.err = &jpeg->jpeg_err_mgr;
    jpeg_std_error( &jpeg->jpeg_err_mgr );
    jpeg->cinfo.err->error_exit = error_exit;
    jpeg_create_decompress(&jpeg->cinfo);

    jpeg->cinfo.src = &jpeg->jpeg_src_mgr;
    jpeg->cinfo.src->fill_input_buffer = fill_input_buffer;
    jpeg->cinfo.src->skip_input_data = skip_input_data;
    jpeg->cinfo.src->resync_to_restart = jpeg_resync_to_restart;
    jpeg->cinfo.src->init_source = init_source;
    jpeg->cinfo.src->term_source = term_source;

    cnt = urf_bstream_read( s, &ptr, URF_STREAM_BUFFER_SIZE );
    if ( 0 >= cnt )
        return 0 ;  // EOF on input 
    
    jpeg->cinfo.src->next_input_byte = ptr;
    jpeg->cinfo.src->bytes_in_buffer = cnt;

    jpeg->cinfo.dct_method = JDCT_FASTEST;
    jpeg->cinfo.do_fancy_upsampling = FALSE;

    jpeg_read_header( &jpeg->cinfo, 1 );

    if (jpeg->cinfo.num_components == 1) 
    {
        jpeg->cinfo.jpeg_color_space = JCS_GRAYSCALE;
    }
    else if (jpeg->cinfo.saw_Adobe_marker)
    {
        if (jpeg->cinfo.Adobe_transform)
            jpeg->cinfo.jpeg_color_space = JCS_YCbCr;
        else
            jpeg->cinfo.jpeg_color_space = JCS_RGB;
    }
    else
    {
        jpeg->cinfo.jpeg_color_space = JCS_YCbCr;
    }    
    jpeg_start_decompress( &jpeg->cinfo );

    urf_page->bitsPerPixel = jpeg->cinfo.output_components * 8;
    urf_page->height = jpeg->cinfo.image_height;
    urf_page->width = jpeg->cinfo.image_width;
    urf_page->resolution = 300; // DAWTEST
    urf_page->copies = 1; // DAWTEST

    return 1;
}

void jpeg_decoder_close( urf_bstream_t *s )
{
    ASSERT(s);
    decode_jpeg_t *jpeg = s->client_data;

    jpeg->stream->cur -= jpeg->cinfo.src->bytes_in_buffer;
    jpeg->stream->have += jpeg->cinfo.src->bytes_in_buffer;

    jpeg_finish_decompress( &jpeg->cinfo );
    jpeg_destroy_decompress( &jpeg->cinfo );
    
    memFree(jpeg);
    s->client_data = 0;
}

int32_t jpeg_decode( urf_bstream_t *s, uint8_t *dest, 
                        uint32_t row_width, int32_t num_rows )
{
    decode_jpeg_t *jpeg = s->client_data;
    uint8_t *p=dest;
    uint32_t row_cnt=0;

    for(row_cnt = 0; row_cnt < num_rows &&
	 jpeg->cinfo.output_scanline < jpeg->cinfo.output_height;
	 p += row_width, row_cnt++)
    {
        jpeg_read_scanlines( &(jpeg->cinfo), &p, 1 );
    }

    // swap R and B
    // TODO: change IJG to dynamically set RGB byte position
    if ( jpeg->cinfo.out_color_space == JCS_RGB )
    {
        uint8_t *cur_ptr;
        uint8_t tmp_val;

        cur_ptr = dest;
        while ( cur_ptr < p )
        {
            tmp_val = *cur_ptr;
            *cur_ptr = *(cur_ptr + 2);
            cur_ptr += 2;
            *cur_ptr = tmp_val;
            cur_ptr++;
        }
    }
    return p - dest ;
}

int jpeg_decompress(urf_bstream_t *bstream,  
           urf_page_t *urf_page, 
           print_page_t* p_page)
{
    DBG_PRINTF_ALERT("BUSTED.. FIX ME\n");
    DBG_PRINTF_NOTICE(" jpeg parser IJG decompress\n");

    // hack to get to build -- sw is broken now anyway..
    uint32_t strip_height=64;

    uint32_t bytes_read=1;
    uint32_t total_bytes_read=0, total_bytes_needed=0;
    uint32_t num_bytes=0;
    uint8_t *data=0;
    uint32_t buffer_size=0;
    uint32_t total_rows=0;
    uint32_t padded_byte_width;
    uint32_t image_bytes_per_pixel = urf_page->bitsPerPixel / 8;
    uint32_t image_width;
    uint32_t image_height;

    decode_jpeg_t *jpeg = bstream->client_data;

    image_width = jpeg->cinfo.image_width;
    image_height = jpeg->cinfo.image_height;

    padded_byte_width = urf_page->width * image_bytes_per_pixel;

    num_bytes = padded_byte_width * strip_height; 
    total_bytes_needed = padded_byte_width * urf_page->height;

    while(bytes_read > 0)
    {
	uint32_t rows=0;
	uint32_t idx=0;
	uint32_t strip_height=0;
	uint8_t *p=0;
	bool last_strip=false;

	if(total_bytes_needed - total_bytes_read < num_bytes)
	{
	    num_bytes = total_bytes_needed - total_bytes_read;
	}
	rows = num_bytes/padded_byte_width;
	total_rows += rows;

	data = MEM_MALLOC_ALIGN_LIMITED(mlimiter_by_name("print"), 
					num_bytes, cpu_get_dcache_line_size());
	memset(data, 0xff, num_bytes); // set RGB pixels to white
	p = data;

	DBG_PRINTF_NOTICE("rows:%d num_bytes:%d tb-need:%d tb-read:%d\n", rows, num_bytes,
		   total_bytes_needed, total_bytes_read);
	for(idx=0; idx<rows; idx++)
	{
            bytes_read = jpeg_decode(bstream, p, padded_byte_width, 1);
	    total_bytes_read += bytes_read;

	    p += bytes_read;

	    if(bytes_read == 0)
	    {
		DBG_PRINTF_NOTICE(" bytes_read==0 and idx:%d rows:%d\n", idx, rows);
		break;
	    }
	}

	strip_height = idx;
	if(strip_height == 0)
	{
	    DBG_PRINTF_NOTICE("stopping decompress early bytes_read:%d bytes_needed:%d\n", total_bytes_read, total_bytes_needed); 
	    break;
	}
	buffer_size = strip_height * padded_byte_width;
	DBG_PRINTF_NOTICE(" rows read:%d\n", strip_height);

	if(total_rows == urf_page->height)
	{
	    DBG_PRINTF_NOTICE(" last strip!!\n");
	    last_strip = true;
	}

	//process_strip(urf_page, p_page, data, buffer_size, last_strip);
        process_strip(bstream, urf_page, p_page, data, image_width, 
		   image_height, strip_height, bytes_read, last_strip);

	if(total_bytes_read >= total_bytes_needed) break; 
    }
    DBG_PRINTF_NOTICE("\njpeg finished reading total_rows:%d\n", total_rows);

    jpeg_decoder_close(bstream);
    return 0;
}

