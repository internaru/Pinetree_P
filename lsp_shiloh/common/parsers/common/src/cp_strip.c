/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * common parser strip handling functions.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "cp_strip.h"
#include "commonio.h"
#ifdef HAVE_JBIGHW
#include "JbigHW.h"
#endif
#include "lassert.h"
#include "logger.h"
#include "print_job_api.h"
#include "memAPI.h"
#include "string.h"
#include "mlimiter.h"
#include "jbig.h"
#include "posix_ostools.h"
//#include "cpu_api.h"
#include "dma_buffer.h"

/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */
#if 0 //-jrs
 error_type_t 
cp_jbig_compress_mono_strips( uint8_t *data_in,
                              uint32_t Xd,
                              uint32_t Yd,
                              uint32_t ld,
                              uint32_t size_of_input_data,
                              uint8_t *data_out,
                              uint32_t size_of_output_data,
                              uint32_t *compressed_size );

 error_type_t 
cp_jbig_decompress_mono_strips( uint8_t *data_in,
                                uint32_t Xd,
                                uint32_t Yd,
                                uint32_t ld,
                                uint32_t size_of_compressed_data,
                                uint8_t *data_out,
                                uint32_t size_of_output_data );

#endif
/// validate compressor by comparing against decompressor output 
#define DBG_VERIFY_COMPRESSOR 0

#ifdef LASER_CMYK_STRIP_PAGE
static PLANE_TYPE cpstrip_comp_type = JBIG; //jbig_not_raw = true;
#endif
void cp_compress_strip( 
    mlimiter_t *mlimiter,
    uint32_t rawsize, 
    uint8_t *raw_compression_buf,
    uint32_t lines,
    uint32_t cols,
    uint8_t **compressed_buf,
    uint32_t *compressed_size,
    uint8_t *static_compression_buffer
    )
{
#ifdef LASER_CMYK_STRIP_PAGE
    uint8_t *compressed_data = 0;
    bool result;

    if (static_compression_buffer)
        compressed_data = static_compression_buffer;
    else
        compressed_data = (uint8_t *)ALIGNED_PRINT_MEMORY(mlimiter, rawsize, e_32_byte);
        
    REL_ASSERT(compressed_data);

    result = JBIGConvert( true /* compress */, 
                          raw_compression_buf, rawsize, 
                          compressed_data, rawsize, compressed_size,
                          /*num_rows_per_strip*/ lines, cols,
                          0 /* not used padrows*/, false /* no rotation */ );

    if ( !result ) 
    {
        DBG_PRINTF_ERR("decomp test res %d in %d out %p src %p dest \n", 
                       result, rawsize, compressed_size, raw_compression_buf, compressed_data);
    }
    if (DBG_VERIFY_COMPRESSOR) { // test that compressor and decompressor output match. 
        uint8_t *decomp = (uint8_t *)ALIGNED_PRINT_MEMORY(
            mlimiter,
            rawsize, 
            e_32_byte);
        uint32_t res_size;
        uint32_t i;
        result = JBIGConvert( false /* compress */, compressed_data, *compressed_size, 
                              decomp, rawsize, &res_size,
                              /*num_rows_per_strip*/ lines,
                              cols, 0 /* not used padrows*/, false /* no rotation */ );

        DBG_PRINTF_ERR("decomp test res %d in %d out %d\n", result, rawsize, res_size);
        // ASSERT(res_size == rawsize);
        for (i = 0; i < res_size; i++) 
            ASSERT(raw_compression_buf[i] == decomp[i]);  // yup it's lossless compression.
        memFree(decomp);
    }



    if (0 && DBG_WOULD_PRINTF(LOG_NOTICE) ) {
        // memDumpHeap(mlimiter);
        dbg_printf("LIM [] (%08p)\n"
                   "   :       .ms 0x%08x     max %08d\n"
                   "   :      parent %08p \n"
                   "   : highwater   %08d current %08d\n", 
                   mlimiter,
                   mlimiter->ms, 
                   mlimiter->max, 
                   mlimiter->parent,
                   mlimiter->high_water,
                   mlimiter->current);
    }

    if (static_compression_buffer)
    {
        // low memory printer copy from reused strip sized compression buffer to compressed size destination.
        *compressed_buf = (uint8_t*) ALIGNED_PRINT_MEMORY(mlimiter, *compressed_size, e_32_byte );

        if( *compressed_buf != NULL )
        {
            memcpy(*compressed_buf, compressed_data, *compressed_size );
        }
    }
    else
    {
        // trim compressed buffer in place, set return pointer to shrunk block
        // caveats: must compress and not expand, 
        // caller of realloc must remember the original mlimiter used to allocate the block.

        *compressed_buf = (uint8_t*) MEM_REALLOC_LIMITED(mlimiter, compressed_data, *compressed_size);
    }

    if (0 && DBG_WOULD_PRINTF(LOG_NOTICE) ) {
        dbg_printf("LIM [] (%08p)\n"
                   "   :       .ms 0x%08x     max %08d\n"
                   "   :      parent %08p \n"
                   "   : highwater   %08d current %08d\n", 
                   mlimiter,
                   mlimiter->ms, 
                   mlimiter->max, 
                   mlimiter->parent,
                   mlimiter->high_water,
                   mlimiter->current);

        //memDumpHeap(mlimiter);
    }
    //static int strip_counter = 0;
    //DBG_PRINTF_ERR("jbighw: strips %d size %d <-- %d\n" , ++strip_counter, *compressed_size, rawsize);
#endif
}

uint32_t num_rows_sent = 0;

void cp_send_strip( print_page_t *p_page, 
                    MODULE_ID modid, 
                    uint8_t *ptr, 
                    uint32_t datalen,
                    uint32_t bits_per_row, 
                    uint32_t num_rows, 
                    uint32_t LinesPerPage, 
                    uint32_t xres, 
                    uint32_t yres,
                    bool endPage,  
                    COLOR color, 
                    PLANE_TYPE plane_type )
{    
    compression_info_t compression_info;

    DBG_PRINTF_NOTICE("%s bits_per_row %d, num_rows %d, lines per page %d, x res %d, y res %d, color %d, plane type %d\n",
               __func__, bits_per_row, num_rows, LinesPerPage, xres, yres, color, plane_type);

    num_rows_sent += num_rows;
  //  dbg_printf(">>>>>>>>>>>>> number rows sent: %d out of %d\n", num_rows_sent, LinesPerPage);

    switch (plane_type)
    {
    case PACKED_RGB:
    case PACKED_RGBX:
    case PACKED_XRGB:
        compression_info.comp_type = COMPTYPE_NONE;
        compression_info.comp_details.rgb.bits_per_pixel = 24;
        compression_info.comp_details.rgb.Xd = bits_per_row;
        compression_info.comp_details.rgb.Yd = num_rows;
        break;

    case GRAYSCALE:
    case GRAYSCALE_SUBTRACTIVE:
        compression_info.comp_type = COMPTYPE_NONE;
        compression_info.comp_details.rgb.bits_per_pixel = 8;
        compression_info.comp_details.rgb.Xd = bits_per_row;
        compression_info.comp_details.rgb.Yd = num_rows;
        break;

    default:
        ASSERT(0);
        break;
    }

    DBG_PRINTF_NOTICE("%s sending 0x%x as plane data\n", __func__, ptr);

    print_plane_add_data( p_page, 
                          color, 
                          dma_buffer_adopt(ptr, datalen), 
                          datalen, 
                          plane_type, 
                          endPage, 
                          xres, 
                          yres,
                          bits_per_row/compression_info.comp_details.rgb.bits_per_pixel, 
                          num_rows, 
                          compression_info.comp_details.rgb.bits_per_pixel,
                          true, // left to right image data
                          &compression_info);

    // DBG_PRINTF_NOTICE( "%s color %s\n", __FUNCTION__, ToString_COLOR(color));
#if defined(LASER_CMYK_STRIP_PAGE) || defined(LASER_RGB_STRIP_PAGE)
    if(endPage)
    {
        // DBG_PRINTF_NOTICE( "print_plane_end on %s\n", ToString_COLOR(color));
        print_plane_end(p_page, color);
    }
#endif
}


/// early terminiation of page via enpage marked strip.
///
void cp_send_empty_endpage_strip( cp_strip_t *strip )
{
#ifdef LASER_CMYK_STRIP_PAGE
    COLOR color;
    uint32_t lines;
    uint8_t *compressed_buf;  // this will be sent to video to be freed after page prints.
    uint32_t compressed_size;

    strip->endpage = true;

    for(color = COLOR_FIRST; color < strip->num_output_colors; color++) // number of planes
    {cpu_dcache_writeback_region( (void *)raw, lines * strip->pitch );
        // todo: page_height - row_counter as input ?   
        lines = strip->actual_lines;
        compressed_size = strip->rawsize = 4;
        compressed_buf = (uint8_t *)ALIGNED_PRINT_MEMORY( strip->mlimiter,
                                                          strip->rawsize, 
                                                          e_32_byte );
        compressed_buf[0] = 0xff;
        compressed_buf[1] = 0x02;
        compressed_buf[2] = 0x00;
        compressed_buf[3] = 0x00;
        
        cp_send_strip(
            strip->p_page,
            strip->dest, 
            compressed_buf, compressed_size,  
            strip->pitch * 8, //bits_per_row,
            lines, //rows,
            strip->page_height, // lines_per_page lines,
            strip->res,
            strip->res,
            strip->endpage,
            color, // KCYM
            cpstrip_comp_type
            );
    }
    strip->inuse = false;  // can reuse the strip.
    if (strip->endpage && (color == strip->num_output_colors) ) // color == COLOR_LAST)
    {
        if (strip->end_page_cb) 
        {
            DBG_PRINTF_NOTICE("%s strip %p %d end %d ------- end_page_cb !!!!!!! lines %d \n",
                              __FUNCTION__, strip, strip->inuse, strip->endpage, lines ); 
            strip->end_page_cb(strip);
        }
    }
#endif
}

void cp_strip_done_cb( void *data, cmyk_byte_array_t *planes, uint32_t size)
{
#ifdef ZX_STREAM_PAGE
    // strip come back in K plane
    uint32_t lines;
    cp_strip_t *strip = (cp_strip_t *) data;
    lines =  strip->endpage ? strip->actual_lines : strip->lines ;
    DBG_ASSERT(strip->cpo->pK == planes->pK);
    if (DBG_EXTRA) 
    {
        if (strip->cpo->pK[strip->rawsize] != 0xa5) 
        {
            DBG_PRINTF_ERR("WARNING: strip data overrun\n");
        }
    }
  
    //NEED TO may want to realloc buffer to size strip->cpo->pK buffer
    DBG_PRINTF_NOTICE( "%s strip %p ptr %x lines %d eop %d slines %d alines %d size %d\n", __FUNCTION__, strip,strip->cpo->pK, lines, strip->endpage, strip->lines, strip->actual_lines, size ); 
        cp_send_strip(
            strip->p_page,
            strip->dest, 
            strip->cpo->pK, size,  
            0,//strip->pitch , //bits_per_row,
            0,//strip->page_height, //rows,
            0,//strip->page_height, // lines_per_page lines,
            0,//strip->res,
            0,//strip->res,
            0, //strip->endpage,//TO DO REVISIT we may need to flush out last line from color pipe before sending EOP
            COLOR_BLACK,
            ZX_STREAM
            );

    strip->cpo->pK = (uint8_t *)ALIGNED_PRINT_MEMORY( strip->mlimiter,
                                                      strip->rawsize + DBG_EXTRA, 
                                                      e_32_byte );
    strip->inuse = false;  // can reuse the strip.
    if (strip->endpage )
    {
        // need common semaphore here. stefan revisit
        if (strip->end_page_cb) 
        {
            DBG_PRINTF_NOTICE("a %s strip %p %d end %d ------- end_page_cb !!!!!!! lines %d \n",
                              __FUNCTION__, strip, strip->inuse, strip->endpage, lines ); 
            strip->end_page_cb(strip);
        }
    }

#elif defined(LASER_CMYK_STRIP_PAGE)
    COLOR color;
    uint8_t *compressed_buf = 0;  // this will be sent to video to be freed after page prints.
    uint32_t compressed_size;
    cp_strip_t *strip = (cp_strip_t *) data;
    uint8_t **raw; 
    uint32_t lines;

    
    DBG_ASSERT(strip->cpo->pK == planes->pK);
    if (DBG_EXTRA) 
    {
        if (strip->cpo->pY[strip->rawsize] != 0xa5) 
        {
            DBG_PRINTF_ERR("WARNING: strip data overrun\n");
        }
    }



    for(color = COLOR_FIRST; color < strip->num_output_colors; color++) // number of planes
    {
        raw = 
            color == COLOR_BLACK ? &planes->pK : 
            color == COLOR_CYAN ? &planes->pC : 
            color == COLOR_MAGENTA ? &planes->pM : 
            &planes->pY ;  // array to discrete convert

        lines =  strip->endpage ? strip->actual_lines : strip->lines ;

        if ( cpstrip_comp_type == JBIG )
        {
            cp_compress_strip( strip->mlimiter, strip->rawsize, *raw, lines, strip->cols, 
                               &compressed_buf, &compressed_size, strip->static_compression_buffer );
        }
        else
        {
            // copy the pointer to the strip for passing on down to video
            // and allocate a new video buffer.
            compressed_buf = *raw;
            compressed_size = strip->rawsize + DBG_EXTRA;

            *raw  = (uint8_t *)ALIGNED_PRINT_MEMORY( strip->mlimiter,
                                                     strip->rawsize + DBG_EXTRA, 
                                                     e_32_byte );
            ASSERT(*raw);
            REL_ASSERT(*raw);
        }
        DBG_PRINTF_NOTICE( "%s strip %p lines %d eop %d slines %d alines %d \n", __FUNCTION__, strip, lines, strip->endpage, strip->lines, strip->actual_lines ); 
        cp_send_strip(
            strip->p_page,
            strip->dest, 
            compressed_buf, compressed_size,  
            strip->pitch * 8, //bits_per_row,
            lines, //rows,
            strip->page_height, // lines_per_page lines,
            strip->res,
            strip->res,
#if (NUM_MARKING_COLOR_CHANNELS != 1) 
            strip->endpage,
#else
            0,
#endif
            color, // KCYM
            cpstrip_comp_type
            );
        if (DBG_EXTRA) 
        {
            // debug marker after end of real strip data.
            memset(*raw + strip->rawsize, 0xa5, DBG_EXTRA);
        }
    }
    strip->inuse = false;  // can reuse the strip.
    if (strip->endpage && (color == strip->num_output_colors) ) // color == COLOR_LAST)
    {
        // need common semaphore here. stefan revisit
        if (strip->end_page_cb) 
        {
            DBG_PRINTF_NOTICE("a %s strip %p %d end %d ------- end_page_cb !!!!!!! lines %d \n",
                              __FUNCTION__, strip, strip->inuse, strip->endpage, lines ); 
#if (NUM_MARKING_COLOR_CHANNELS != 1) // color engine
            strip->end_page_cb(strip);
#else // mono engine requires a blank strip at end of page but, color video code does not like this. paw
            cp_send_empty_endpage_strip( strip );
#endif

        }
    }
#elif defined(LASER_RGB_STRIP_PAGE)
    cp_strip_t *strip = (cp_strip_t *) data;
    uint8_t *raw;
    uint32_t lines;
    
    DBG_ASSERT(strip->cpo->pK == planes->pK);
    if (DBG_EXTRA) 
    {
        if (strip->cpo->pK[strip->rawsize] != 0xa5) 
        {
            DBG_PRINTF_ERR("WARNING: strip data overrun\n");
        }
    }

    raw = planes->pK;  

    // lines =  strip->endpage ? size : strip->lines ;
    // This is to address a stray strip that was being printed via air-print.
    lines =  strip->endpage ? strip->actual_lines : strip->lines ;


    // Flush the cache to memory before submitting the strip for future DMA
#ifdef HAVE_MPDF_SUPPORT
    {
       extern void mpdf_blit_string_raster(uint8_t *raw, uint32_t pitch, uint32_t height, bool is_color);
       mpdf_blit_string_raster(raw,strip->pitch,lines,(strip->num_input_colors != 1));
       cpu_dcache_writeback_region( (void *)raw, lines * strip->pitch );
    }   
#endif
//    cpu_dcache_writeback_region( (void *)raw, lines * strip->pitch );


    //DBG_PRINTF_NOTICE( "%s strip %p lines %d eop %d slines %d alines %d \n", __FUNCTION__, strip, lines, strip->endpage, strip->lines, strip->actual_lines ); 
    cp_send_strip(strip->p_page,
                  strip->dest, 
                  raw, lines * strip->pitch,  
                  strip->pitch * 8, //bits_per_row,
                  lines, //rows,
                  strip->page_height, // lines_per_page lines,
                  strip->res,
                  strip->res,
                  strip->endpage,
                  COLOR_BLACK, // KCYM
                  strip->num_input_colors == 1 ? GRAYSCALE_SUBTRACTIVE : PACKED_RGB
                  );
    
    strip->cpo->pK = (uint8_t *)ALIGNED_PRINT_MEMORY( strip->mlimiter,
                                                      strip->rawsize + DBG_EXTRA, 
                                                      e_32_byte );
    // DBG_PRINTF_ERR("%s mallocing %d bytes at addr 0x%x\n", __func__, strip->rawsize + DBG_EXTRA, strip->cpo->pK);

    if (DBG_EXTRA) 
    {
        // debug marker after end of real strip data.
        memset(raw + strip->rawsize, 0xa5, DBG_EXTRA);
    }
    
    strip->inuse = false;  // can reuse the strip.
    if (strip->endpage )
    {
        // need common semaphore here. stefan revisit
        if (strip->end_page_cb) 
        {
            DBG_PRINTF_NOTICE("a %s strip %p %d end %d ------- end_page_cb !!!!!!! lines %d \n",
                              __FUNCTION__, strip, strip->inuse, strip->endpage, lines ); 
            strip->end_page_cb(strip);
        }
    }
#else
#error
#endif
}

cp_strip_t * cp_free_rawstrips( cp_strip_t *strip )
{
    if (strip)
    {
        while (strip->inuse)
        {
            DBG_PRINTF_ERR( "%s 1 waiting for strip to be finished\n", __FUNCTION__ );
        }
        REL_ASSERT(strip->cpo);
        memFree(strip->cpo->pK); 
#if defined(LASER_CMYK_STRIP_PAGE)
        memFree(strip->cpo->pC); 
        memFree(strip->cpo->pM);     
        memFree(strip->cpo->pY); 
#endif // LASER_CMYK_STRIP_PAGE
        memFree(strip->cpo); 
        memFree(strip);
    }
    return 0;
}


#ifdef PIXELCNT_BYFW
/// convert between pixel counting interfaces, compiled out to "return" NULL on mono printers.
PIXELCNT *cp_pixel_count( mlimiter_t *mlimiter, color_pipe_t *cp, bool reset_counts )
{
    uint32_t white_pixels[4]; // unused in output
    PIXELCNT *p_pixel_counts = 0; // pointer ownership is passed and deleted by message receiver

    p_pixel_counts = (PIXELCNT *)PRINT_MEMORY( mlimiter, sizeof(PIXELCNT) ); 
    memset( p_pixel_counts, 0, sizeof(PIXELCNT) ); 

    color_pipe_get_pixel_counts(
        cp,
        (cmyk_uint32_t*)&p_pixel_counts->pixelCntPerPage[0], 
        (cmyk_uint32_t*)&p_pixel_counts->nonWhitePixelCntPerPage[0], 
        (cmyk_uint32_t*)&white_pixels[0], 
        reset_counts);

    dbg_printf("urf: pixel counts KCMY %d %d %d %d : %d %d %d %d \n",
                      p_pixel_counts->pixelCntPerPage[0], 
                      p_pixel_counts->pixelCntPerPage[1], 
                      p_pixel_counts->pixelCntPerPage[2], 
                      p_pixel_counts->pixelCntPerPage[3],
                      p_pixel_counts->nonWhitePixelCntPerPage[0], 
                      p_pixel_counts->nonWhitePixelCntPerPage[1], 
                      p_pixel_counts->nonWhitePixelCntPerPage[2], 
                      p_pixel_counts->nonWhitePixelCntPerPage[3]
        );
    MEM_FREE_AND_NULL(p_pixel_counts);
    return p_pixel_counts;  // ownership transfered to caller.
}
#endif


#if 0 // DEBUG
void dbg_print_cp_strip_t( cp_strip_t *strip )
{
    if (strip)
    {
        dbg_printf(" cp_strip: %p inuse %d page %p rawsize %d pitch %d lines %d actual_lines %d cols %d res %d numcolors %d endpage %d dest %d buf %p lim %p ", 
                   strip,
                   strip->inuse,
                   strip->page,
                   strip->rawsize,
                   strip->pitch,
                   strip->lines,
                   strip->actual_lines,
                   strip->page_height,
                   strip->cols,
                   strip->res,
                   strip->numcolors,
                   strip->endpage,
                   strip->dest,
                   strip->buf,
                   strip->mlimiter
            );    
    }
    else 
    {
        dbg_printf(" cp_strip: %p ", strip );    
    }
}
#endif

#if 0
#include "jbig_api.h"
pthread_mutex_t jbig_mutex = POSIX_MUTEX_INITIALIZER;
pthread_cond_t jbig_cond =  PTHREAD_COND_INITIALIZER;
uint32_t jbig_flags = 0;
#define JBIG_ENCODE_DONE 1
#define JBIG_DECODE_DONE 2
#define JBIG_ODMA_DONE 4
#define JBIG_IDMA_DONE 8

void jbig_encode_callback (void *user_data, uint32_t int_status)
{
    // dbg_printf("%s status 0x%x\n", __func__, int_status );

    if(int_status & JBIG_ODMA_COMPLETE_FLAG)
    {
        pthread_mutex_lock(&jbig_mutex);
        jbig_flags |= JBIG_ODMA_DONE;
        pthread_cond_broadcast(&jbig_cond);
        pthread_mutex_unlock(&jbig_mutex);
    }

    if (int_status & JBIG_IDMA_COMPLETE_FLAG)
    {
        pthread_mutex_lock(&jbig_mutex);
        jbig_flags |= JBIG_IDMA_DONE;
        pthread_cond_broadcast(&jbig_cond);
        pthread_mutex_unlock(&jbig_mutex);
    }

    if (int_status & JBIG_CONTROL_FLAG)
    {
        pthread_mutex_lock(&jbig_mutex);
        jbig_flags |= JBIG_ENCODE_DONE;
        pthread_cond_broadcast(&jbig_cond);
        pthread_mutex_unlock(&jbig_mutex);
    }
}

void jbig_decode_callback (void *user_data, uint32_t int_status)
{
    // dbg_printf("%s ******************************************************* status 0x%x\n", __func__, int_status );

    if (int_status & JBIG_CONTROL_FLAG)
    {
      pthread_mutex_lock(&jbig_mutex);
      jbig_flags |= JBIG_DECODE_DONE;
      pthread_cond_broadcast(&jbig_cond);
      pthread_mutex_unlock(&jbig_mutex);
    }

    if(int_status & JBIG_ODMA_COMPLETE_FLAG)
    {
      pthread_mutex_lock(&jbig_mutex);
      jbig_flags |= JBIG_ODMA_DONE;
      pthread_cond_broadcast(&jbig_cond);
      pthread_mutex_unlock(&jbig_mutex);
    }

    if (int_status & JBIG_IDMA_COMPLETE_FLAG)
    {
      pthread_mutex_lock(&jbig_mutex);
      jbig_flags |= JBIG_IDMA_DONE;
      pthread_cond_broadcast(&jbig_cond);
      pthread_mutex_unlock(&jbig_mutex);
    }
}


error_type_t 
cp_jbig_compress_mono_strips( uint8_t *data_in,
                              uint32_t Xd,
                              uint32_t Yd,
                              uint32_t ld,
                              uint32_t size_of_input_data,
                              uint8_t *data_out,
                              uint32_t size_of_output_data,
                              uint32_t *compressed_size )
{
    jbig_handle_t *jbigEncodeHandle;
    uint32_t flags;
    uint32_t bytes_left;
    

    if( jbig_open( &jbigEncodeHandle, JBIG_CODEC_BLOCK ) != OK )
    {
        
        DPRINTF((DBG_OUTPUT|DBG_LOUD),("JBIG: COMPRESSION FAILED TO GET HANDLE\n"));
        return FAIL;
    }

    //dbg_printf("%s input size: %d, output size %d, xd %d, yd %d, ld %d\n",
    //           __func__, size_of_input_data, size_of_output_data, Xd, Yd, ld);

    logString("Set JBIG encode\n");
    jbig_set_encode( jbigEncodeHandle );
    
    jbig_disable_cbi( jbigEncodeHandle );

    jbig_set_x_dimension( jbigEncodeHandle, Xd );
    jbig_set_y_dimension( jbigEncodeHandle, Yd );
    jbig_set_stripe_length( jbigEncodeHandle, ld );

    jbig_enable_tpbon(jbigEncodeHandle);
    
    // The input data should be resident in memory, no cache coherency concerns
    // should exist at this point.
    jbig_add_input_buffer( jbigEncodeHandle, 
                           data_in, 
                           size_of_input_data, 
                           NULL );


    jbig_add_output_buffer( jbigEncodeHandle, 
                            data_out, 
                            size_of_output_data, 
                            data_out ); // REVISIT DAB need to add useful user data?

    jbig_start_transaction( jbigEncodeHandle, jbig_encode_callback );

    // Wait for encode to complete. This will occur when the encode operation
    // complete bit is set as well as the input DMA complete bit. Output DMA 
    // will most likely NOT complete as the buffer is intentionally larger than
    // necessary.
    // dbg_printf("WAITING FOR ENCODE DONE\n");
    pthread_mutex_lock(&jbig_mutex);
    while (!(jbig_flags & JBIG_ENCODE_DONE))
    {
      pthread_cond_wait(&jbig_cond, &jbig_mutex);
    }
    flags = jbig_flags; 
    jbig_flags = 0;
    pthread_mutex_unlock(&jbig_mutex);

    // dbg_printf("WAITING FOR IDMA DONE\n");
    pthread_mutex_lock(&jbig_mutex);
    while (!(jbig_flags & JBIG_IDMA_DONE))
    {
      pthread_cond_wait(&jbig_cond, &jbig_mutex);
    }
    flags = jbig_flags; 
    jbig_flags = 0;
    pthread_mutex_unlock(&jbig_mutex);

    jbig_read_bytes_left_to_transfer_output_dma( jbigEncodeHandle, &bytes_left );

    // dbg_printf("input size: %d, output size: %d, bytes left in output %d\n", size_of_input_data, size_of_output_data, bytes_left );

    *compressed_size = size_of_output_data - bytes_left;

    jbig_stop_transaction( jbigEncodeHandle );

    jbig_close( jbigEncodeHandle );

    return OK;
}



// REVISIT DAB this is for debug only to test the compression 
 error_type_t 
cp_jbig_decompress_mono_strips( uint8_t *data_in,
                                uint32_t Xd,
                                uint32_t Yd,
                                uint32_t ld,
                                uint32_t size_of_compressed_data,
                                uint8_t *data_out,
                                uint32_t size_of_output_data)
{
    jbig_handle_t *jbig_decode_handle;
    uint32_t flags;

    if( jbig_open( &jbig_decode_handle, JBIG_CODEC_BLOCK ) != OK )
    {
        
        DPRINTF((DBG_OUTPUT|DBG_LOUD),("JBIG: DECOMPRESSION FAILED TO GET HANDLE\n"));
        return FAIL;
    }

    // dbg_printf("%s input size: %d, output size %d, xd %d, yd %d, ld %d\n",
    //           __func__, size_of_compressed_data, size_of_output_data, Xd, Yd, ld);
    
    logString("jbig test: set the decode\n");
    jbig_set_decode( jbig_decode_handle );
    
    jbig_disable_cbi( jbig_decode_handle );

    jbig_set_x_dimension( jbig_decode_handle, Xd );
    jbig_set_y_dimension( jbig_decode_handle, Yd );
    jbig_set_stripe_length( jbig_decode_handle, ld );

    jbig_enable_tpbon(jbig_decode_handle);

    jbig_set_output_dma_line_width( (void *)jbig_decode_handle, Xd/8 );  
    
    // The input data should be resident in memory, no cache coherency concerns
    // should exist at this point.
    jbig_add_input_buffer( jbig_decode_handle, 
                           data_in, 
                           size_of_compressed_data, 
                           NULL );


    jbig_add_output_buffer( jbig_decode_handle, 
                            data_out, 
                            size_of_output_data, 
                            data_out ); // REVISIT DAB need to add useful user data?

    jbig_start_transaction( jbig_decode_handle, jbig_decode_callback );

    // dbg_printf("waiting for decode to complete\n");
    // Wait for decode to complete.
    pthread_mutex_lock(&jbig_mutex);
    while (!(jbig_flags & (JBIG_DECODE_DONE|JBIG_ODMA_DONE|JBIG_IDMA_DONE)))
    {
      pthread_cond_wait(&jbig_cond, &jbig_mutex);
    }
    flags = jbig_flags; 
    jbig_flags = 0;
    pthread_mutex_unlock(&jbig_mutex);

    // dbg_printf("DECODE DONE!!!!!\n");


    jbig_stop_transaction( jbig_decode_handle );


    jbig_close( jbig_decode_handle );

    return OK;
}
#endif
