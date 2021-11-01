/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2011-2013 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file bstream_filter_jpeg_decode
 *
 */

#include <stdint.h>

#include "bstream_filter_jpeg.h"
#include "memAPI.h"
#include <stdio.h>
#include <string.h> // memset
#include "logger.h"

// Config the printf's from logger.h 
#define DBG_PRFX "bstream_rle: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 7 )



#include "writefile_from_ram.h"

#ifdef HAVE_JPEG_HW_ASSIST
static mlimiter_t *g_mlimiter=0;
#include "jpeg_hw_driver.h"

static bool g_mlimiter_started=false;

typedef struct bstream_filter_jpeg_s bstream_filter_jpeg_t;
struct bstream_filter_jpeg_s
{
    uint32_t pitch; 
    image_info *info;
};

uint8_t * read_jpeg_strip( bstream_t *s, uint32_t jpeg_datalen )
{
    uint8_t *jpeg_data=0, *pjpeg_data;
    uint32_t bytes_read=0, total_bytes_read=0;
    uint32_t bytes_wanted=0;
    uint32_t cache_line_size=cpu_get_dcache_line_size();
    uint32_t allocation_size=0;

    allocation_size = jpeg_datalen + (cache_line_size - (jpeg_datalen % cache_line_size));

   // dbg_printf("read_jpeg_file...\n");
    jpeg_data = (uint8_t*)MEM_MALLOC_ALIGN_LIMITED(g_mlimiter,
	            allocation_size, cache_line_size);
    ASSERT(jpeg_data != NULL);

    while(total_bytes_read < jpeg_datalen)
    {
	uint8_t *pipe_data=0;
	
	bytes_wanted = jpeg_datalen - total_bytes_read;
	pjpeg_data = jpeg_data + total_bytes_read;

	bytes_read = bstream_read(s, &pipe_data, bytes_wanted);
	memcpy(pjpeg_data, pipe_data, bytes_read);
	total_bytes_read += bytes_read;
    }

    cpu_dcache_writeback_region(jpeg_data, allocation_size);

    return jpeg_data;
}

static bool first_read=false;
static uint32_t opencounter=0;
bstream_t * bstream_filter_jpeg_open( bstream_t *s )
{
    //g_mlimiter = mlimiter_by_name("jpeg");
    // for now use system memory until the kinoma limiter min
    // is determined
    g_mlimiter = mlimiter_by_name("syslimiter");

    // don't start if using the system limiter and only if
    // not already started otherwise
    if(g_mlimiter != mlimiter_by_name("syslimiter") &&
       ! g_mlimiter_started)
    {
	mlimiter_start(g_mlimiter);
    }
    g_mlimiter_started = true;
    opencounter++;
    bstream_filter_jpeg_t *jpeg = (bstream_filter_jpeg_t *)
        MEM_MALLOC_LIMITED(g_mlimiter, sizeof(bstream_filter_jpeg_t));
    ASSERT(jpeg != NULL);

    s->state = jpeg;
    first_read=true;

    //jpeg->pitch = jpeg->info->mcu_aligned_image_width * jpeg->info->num_planes_m1;
    s->close = bstream_filter_jpeg_close;
    s->decode = bstream_filter_jpeg_decode;

    return s;
}

void bstream_filter_jpeg_close( bstream_t *s )
{
    bstream_filter_jpeg_t *jpeg = (bstream_filter_jpeg_t *)s->state;
    //dbg_printf("%s\n", __FUNCTION__);

    MEM_FREE_AND_NULL(jpeg->info->jpeg_data);
    jpegdrvr_close(jpeg->info);
    MEM_FREE_AND_NULL(jpeg);
}

static uint8_t *data=0;
void bstream_filter_jpeg_cleanup( void )
{
    dbg_printf("%s\n", __FUNCTION__);
    if(data != 0)
    {
	MEM_FREE_AND_NULL(data);
    }
    if(g_mlimiter != mlimiter_by_name("syslimiter") &&
       g_mlimiter_started)
    {
	mlimiter_stop(g_mlimiter);
    }
}

int32_t bstream_filter_jpeg_decode( bstream_t *s, uint8_t *dest, 
				 uint32_t dest_len, int32_t smax )
{
    static uint8_t *jpeg_data;
    static uint8_t *pdata=0;
    static uint32_t bytes_read=0;
    static uint32_t bufsize=0;
    bstream_filter_jpeg_t *jpeg = (bstream_filter_jpeg_t *)s->state;

    if(first_read)
    {
	int retstat;
	uint32_t newbufsize=0;
	jpeg_data = read_jpeg_strip(s, smax);

	first_read = false;
	bytes_read = 0;

	jpeg->info = jpegdrvr_open(g_mlimiter, jpeg_data, smax);
	jpegdrvr_parse_header(jpeg->info);

	newbufsize = jpeg->info->dma_aligned_image_width * 
	             jpeg->info->mcu_aligned_image_height *
	             jpeg->info->num_planes_m1;
	uint32_t dcache_line_size = cpu_get_dcache_line_size();
	newbufsize += dcache_line_size - (newbufsize % dcache_line_size);

	// allocate a new buffer if the old buffersize is not correct
	if(newbufsize != bufsize || data == 0)
	{
	    bufsize = newbufsize;

	    if(data != 0)
	    {
		MEM_FREE_AND_NULL(data);
	    }
	    //dbg_printf("allocating %d bytes\n", bufsize);
	    data = (uint8_t*)mem_retry_malloc_align_limited( 
		jpeg->info->mlimiter, bufsize, 
		cpu_get_dcache_line_size(), __FILE__, __LINE__);
	    XASSERT(data != NULL, bufsize);
	}

        jpegdrvr_program_hw(jpeg->info);

	// continue decompressing until we get an error free decompression
	#define MAX_TRYS 10
	#define MARGIN 99
	uint32_t trys;
	bool artifacts;
	artifacts = false;

	retstat = JPEG_DRVR_FAILURE; // force entry into for loop
	for(trys=0; trys<MAX_TRYS && retstat != JPEG_DRVR_SUCCESS; trys++)
	{
	    cpu_dcache_invalidate_region(data, bufsize);

	    retstat = jpegdrvr_get_data(jpeg->info, &data, 
		      jpeg->info->mcu_aligned_image_height, &bytes_read);
	    pdata = data;

	    if(retstat == JPEG_DRVR_FAILURE)
	    {
                 jpegdrvr_reset_input(jpeg->info);
	    } 
	}

	#if 0
	// draw horz grid lines to mark troubled strips
        if(trys > 1)
	{
	    dbg_printf("Drawing grid lines for strip %d\n", opencounter);
	    memset(data, 0x00, jpeg->info->mcu_aligned_image_width * 
		   jpeg->info->num_planes_m1 -600);
	    if(artifacts)
	    {
		memset(data +63*jpeg->info->dma_aligned_image_width * 
		       jpeg->info->num_planes_m1, 0x00, 900);
	    }
	    else
	    {
		memset(data +63*jpeg->info->dma_aligned_image_width * 
		       jpeg->info->num_planes_m1, 0x00, 300);
	    }
	    memset(data +127*jpeg->info->dma_aligned_image_width * 
		   jpeg->info->num_planes_m1,
		   0x00, jpeg->info->mcu_aligned_image_width * 
		   jpeg->info->num_planes_m1 -600);
	}
	#endif
	XASSERT(trys < MAX_TRYS, trys);
    }

    memcpy(dest, pdata, dest_len);
    pdata += jpeg->info->dma_aligned_image_width * jpeg->info->num_planes_m1;

    return dest_len;
}

#else // HAVE_JPEG_HW_ASSIST

#include "jpeglib.h"

typedef struct bstream_filter_jpeg_s bstream_filter_jpeg_t;
struct bstream_filter_jpeg_s
{
    bstream_t *stream;

    struct jpeg_decompress_struct cinfo;
    struct jpeg_source_mgr jpeg_src_mgr;
    struct jpeg_error_mgr jpeg_err_mgr;
    uint32_t pitch; 
	char msg[JMSG_LENGTH_MAX];
};


static boolean fill_input_buffer( j_decompress_ptr cinfo )
{
    struct jpeg_source_mgr *src = cinfo->src;
    bstream_filter_jpeg_t *jpeg = cinfo->client_data;
    bstream_t *s = jpeg->stream;
    uint8_t *ptr;
    int32_t cnt;

    cnt = bstream_read( s, &ptr, STREAM_BUFFER_SIZE );
    if ( 0 >= cnt )
        cnt = 0; // EOF on input 
    
    src->next_input_byte = ptr;
    src->bytes_in_buffer = cnt;

    if (src->bytes_in_buffer == 0)
    {
        static uint8_t jpeg_eoi[2] = { 0xFF, JPEG_EOI };
        DBG_PRINTF_ERR( "EOF with io timeout inside jpeg_decode %s\n", __FUNCTION__ );
        src->next_input_byte = jpeg_eoi;
        src->bytes_in_buffer = 2;
    }
    return 1;
}

static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    struct jpeg_source_mgr *src = cinfo->src;
    if (num_bytes > 0)
    {
        while (num_bytes > src->bytes_in_buffer)
        {
            num_bytes -= src->bytes_in_buffer;
            (void) src->fill_input_buffer(cinfo);
        }
        src->next_input_byte += num_bytes;
        src->bytes_in_buffer -= num_bytes;
    }
}

static void error_exit(j_common_ptr cinfo)
{
    // revisit this 
    bstream_filter_jpeg_t *jpeg;
    jpeg = cinfo->client_data;
    DBG_PRINTF_ERR("%s %s\n", __FUNCTION__, jpeg->msg );
    // ASSERT( 0 == 1 );
}

static void init_source(j_decompress_ptr cinfo)
{ 
    // nop
}

static void term_source(j_decompress_ptr cinfo)
{ 
    // nop
}

bstream_t *
bstream_filter_jpeg_open( bstream_t *s )
{
    bstream_filter_jpeg_t *jpeg = (bstream_filter_jpeg_t *) MEM_MALLOC( sizeof(bstream_filter_jpeg_t) );
    ASSERT(jpeg != NULL);

    int32_t cnt;
    uint8_t *ptr;

    memset(jpeg, 0, sizeof(bstream_filter_jpeg_t));

    jpeg->stream = s;
    s->state = jpeg;

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

    cnt = bstream_read( s, &ptr, STREAM_BUFFER_SIZE );
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

    jpeg->pitch = jpeg->cinfo.output_width * jpeg->cinfo.output_components;
    s->close = bstream_filter_jpeg_close;
    s->decode = bstream_filter_jpeg_decode;
    return s;
}

void
bstream_filter_jpeg_close( bstream_t *s )
{
    ASSERT(s);
    bstream_filter_jpeg_t *jpeg = s->state;

    jpeg->stream->cur -= jpeg->cinfo.src->bytes_in_buffer;
    jpeg->stream->have += jpeg->cinfo.src->bytes_in_buffer;

    jpeg_finish_decompress( &jpeg->cinfo );
    jpeg_destroy_decompress( &jpeg->cinfo );
    
    memFree(jpeg);
    s->state = 0;
}

void bstream_filter_jpeg_cleanup( void )
{
    // a do nothing stub.  this cleanup is needed in the
    // hardware decoder above ( HAVE_JPEG_HW_ASSIST )
}

int32_t
bstream_filter_jpeg_decode( bstream_t *s, uint8_t *dest, uint32_t dest_len, int32_t smax )
{
    bstream_filter_jpeg_t *jpeg = s->state;
    uint8_t *p;

    for ( p = dest; 
          jpeg->pitch <= dest_len &&
              jpeg->cinfo.output_scanline < jpeg->cinfo.output_height ;
          p += jpeg->pitch, 
          dest_len -= jpeg->pitch
        )
    {
        jpeg_read_scanlines( &jpeg->cinfo, &p, 1 );
    }

    // REVISIT - Why do we have to swap the datastream???
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
#endif // HAVE_JPEG_HW_ASSIST
