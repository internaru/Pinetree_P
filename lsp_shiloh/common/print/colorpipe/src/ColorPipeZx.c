/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file ColorPipeZx.c
 *
 *
 *****************************************************************************/
#include "ATypes.h"
#include "agJobUtils.h"
#include "ColorPipeTables.h"
#include "ColorPipe.h"

#include "ColorPipeZxPriv.h"


#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include "memAPI.h" // needed for MEM_MALLOC macro definition
#include <stdlib.h>
#include <string.h>
#include "cmd_proc_api.h"
#include "ZxStream.h"
//#define DBG_RLE (DBG_LOUD|DBG_OUTPUT)
#define DBG_RLE 0
#define DBG_CPIPE 0
//#define DBG_CPIPE (DBG_LOUD|DBG_OUTPUT)
static color_pipe_t * default_color_pipe = NULL;

void color_pipe_init(void);
/**
 * @brief see header file
 * @param output_bits_per_pixel s/b 8.
 * @param color_space_conv 3 to 4 or 3 to 1 is supported.
 * @param pixels_per_line full bleed line
 * @param output_pitch not used
 * @param lines_per_strip Size of output buffer yes, its a bit
 *                        of a hack
 * @param x_scale no need to scale for ZxStream.
 * @param mode only support default_color_pipe for now.
 * 
 * @return color_pipe_t* ptr to color pipe, NULL if error.
 */
color_pipe_t * color_pipe_page_open( uint32_t output_bits_per_pixel, 
                                     color_space_conv_t color_space_conv,
                                     uint32_t pixels_per_line,//not used in Zx
                                     uint32_t output_pitch,//not used in Zx
                                     uint32_t lines_per_strip,//Size of buffer
                                     uint32_t x_scale, 
                                     color_pipe_mode_t mode)
{

    color_pipe_t * color_pipe;

    ASSERT(pixels_per_line < CP_MAX_LINE_LEN);

    if (default_color_pipe == NULL) 
    {
        color_pipe_init();
    }
    //TO DO:May want to add some asserts here
    
    if (mode == e_color_pipe_default) 
    {
        color_pipe = default_color_pipe;
    }
    else 
    {
       ASSERT(0);//someone forgot to add the new modes
       return (NULL); 
    }
    
#ifdef MONO_ENGINE
    if (color_space_conv == e_color_space_3_to_4) 
    {
       ASSERT(0);//Not allowed
       return (NULL);    
    }
#endif
    
    color_pipe->curr_line = 0;
    color_pipe->output_strip_pitch = output_pitch;
    color_pipe->lines_per_strip = lines_per_strip;
    color_pipe->color_planes_out = NULL;
    color_pipe->num_bytes_not_written = 0;
    if (color_space_conv == e_color_space_3_to_4) 
    {
        color_pipe->bytes_per_pixel = 3;
    }
    else
    {
        color_pipe->bytes_per_pixel = 1;
    }
    if (color_pipe->line_data == NULL) 
    {
        color_pipe->line_data = MEM_MALLOC_LIMITED(color_pipe->mlimiter,CP_MAX_LINE_LEN * 3 * 3);
        
    }
    color_pipe->comp.curr_pos = NULL;
    color_pipe->page_init = false;
    color_pipe->comp.state = e_between_lines;
    return(color_pipe);
}
/**
 * @brief See header file
 * @param color_pipe ptr to color_pipe
 * @param color_planes_out ptr to output strip, only pK strip
 *                         used.
 * @param strip_halftoned_callback ptr to callback function
 *                                 which is called when strip
 *                                 buffer is full.
 * @param callback_private_data private data returned when
 *                              callback function is called.
 */
void color_pipe_new_halftone_strip(color_pipe_t * color_pipe, 
                                   cmyk_byte_array_t *color_planes_out,
                                   void(*strip_halftoned_callback)( void *callback_private_data, cmyk_byte_array_t *color_planes_out, uint32_t size ), 
                                   void *callback_private_data  )
{

    ASSERT(color_pipe != NULL);
    ASSERT(color_pipe->bytes_per_pixel == 3 ||color_pipe->bytes_per_pixel == 1); //Only support sRGB & sGray
  
    color_pipe->color_planes_out = color_planes_out;
    if (color_pipe->page_init == false)
    {
        static uint8_t init_color_buffer[] = {
            ZxSTREAM_ESCAPE, ZxSTREAM_REVISION, ZxSTREAM_CURRENT_REVISION, //rev 8
            ZxSTREAM_ESCAPE, ZxSTREAM_SIZEOF_DIRECT_COLOR, 0x03,//pixel size 3 
            ZxSTREAM_ESCAPE, ZxSTREAM_BACKGROUND_COLOR, 0x03,0xFF,0xFF,0xFF //Background pixel white
        };
        static uint8_t init_grey_buffer[] = {
            ZxSTREAM_ESCAPE, ZxSTREAM_REVISION, ZxSTREAM_CURRENT_REVISION, //rev 8
            ZxSTREAM_ESCAPE, ZxSTREAM_SIZEOF_DIRECT_COLOR, 0x01, //pixel size 1 
            ZxSTREAM_ESCAPE, ZxSTREAM_BACKGROUND_COLOR, 0x01, 0xFF //Background pixel white
        };
        uint8_t * init_ptr = (color_pipe->bytes_per_pixel == 3)?init_color_buffer:init_grey_buffer;
        uint32_t init_size = (color_pipe->bytes_per_pixel == 3)?sizeof(init_color_buffer):sizeof(init_grey_buffer);
        DPRINTF(DBG_CPIPE,("colorpipe: init page %d %d\n",color_pipe->bytes_per_pixel,init_size));
        memcpy(color_planes_out->pK, init_ptr, init_size);
        color_pipe->curr_strip_pos = color_planes_out->pK + init_size;
        color_pipe->num_bytes_written = init_size;
        color_pipe->num_bytes_not_written = 0;
        color_pipe->page_init = true;
    }
    else if (color_pipe->num_bytes_not_written != 0) 
    {
        memcpy(color_planes_out->pK, &color_pipe->line_data[0], color_pipe->num_bytes_not_written);
        color_pipe->curr_strip_pos = color_planes_out->pK + color_pipe->num_bytes_not_written;
        color_pipe->num_bytes_written = color_pipe->num_bytes_not_written;
        color_pipe->num_bytes_not_written = 0;
    }
    else
    {
        color_pipe->curr_strip_pos = color_planes_out->pK;
        color_pipe->num_bytes_written = 0;
        color_pipe->num_bytes_not_written = 0;
    }
    DPRINTF(DBG_CPIPE,("CP: out ptr %x\n",color_planes_out->pK));
    color_pipe->strip_halftoned_callback = strip_halftoned_callback;
    color_pipe->strip_callback_private_data = callback_private_data;

}

/**
 * @brief Writes a compressed pixel line to the current buffer
 *        if there is room.  If not saves enough information so
 *        it can be written when the next zx_buffer is sent
 *        down.
 * @param color_pipe ptr to color_pipe
 * 
 * @return bool:  returns if a zx_buffer (strip) is full
 */
static bool write_zxstream_line(color_pipe_t * color_pipe)
{
    uint32_t bytes_to_write = 0;
    ASSERT(color_pipe != NULL);
    //DPRINTF(DBG_LOUD|DBG_OUTPUT,("CP:Write Line Bytes written %d bytes to write %d strip ptrs %x:%x \n",color_pipe->num_bytes_written,color_pipe->comp.num_bytes_in_line,color_pipe->color_planes_out->pK,color_pipe->curr_strip_pos));
    //DPRINTF(DBG_LOUD|DBG_OUTPUT,("CP:curr_strip_pos %x line_data %x \n",color_pipe->curr_strip_pos, color_pipe->line_data));
    bool strip_state = COLOR_PIPE_STRIP_NOT_FULL;
    ASSERT (color_pipe->comp.num_bytes_in_line < CP_MAX_LINE_LEN * 3 * 6);
    if (color_pipe->comp.num_bytes_in_line + color_pipe->num_bytes_written <= color_pipe->lines_per_strip) 
    {
        memcpy(color_pipe->curr_strip_pos, color_pipe->line_data, color_pipe->comp.num_bytes_in_line);
        color_pipe->num_bytes_written += color_pipe->comp.num_bytes_in_line;
        color_pipe->curr_strip_pos += color_pipe->comp.num_bytes_in_line;
        if (color_pipe->num_bytes_written == color_pipe->lines_per_strip) 
        {
            bytes_to_write = color_pipe->num_bytes_written;
            strip_state = COLOR_PIPE_STRIP_FULL;
            color_pipe->num_bytes_not_written = 0;
        }
    }
    else
    {
        bytes_to_write = color_pipe->num_bytes_written;
        color_pipe->num_bytes_not_written = color_pipe->comp.num_bytes_in_line;
        strip_state = COLOR_PIPE_STRIP_FULL;
    }
    
    if (strip_state == COLOR_PIPE_STRIP_FULL) 
    {
        cmyk_byte_array_t * tmp_color_planes_out = color_pipe->color_planes_out;
        color_pipe->color_planes_out = NULL;
        color_pipe->num_bytes_written = 0;
        if (color_pipe->strip_halftoned_callback != NULL)
        {
            (*color_pipe->strip_halftoned_callback)(color_pipe->strip_callback_private_data, tmp_color_planes_out, bytes_to_write );
        }
    }
    color_pipe->comp.curr_pos = NULL;
    color_pipe->comp.state = e_between_lines;
    return (strip_state);
}

/**
 * @brief See header file
 * @param color_pipe ptr to color_pipe
 * @param line_ptr ptr to line to be processed
 * @param ot_ptr ptr to object tag
 * 
 * @return bool returns if strip full
 */
bool color_pipe_process_line(color_pipe_t * color_pipe,  
                             uint8_t *line_ptr,                       //inputs 3 byte/pixel, packed RGB or 1 byte/pixel grey
                             object_tag_t *ot_ptr)                  //  1byte/pixel, packed OT or boolean flag.
                             
{

    ASSERT (0); //NOT IMPEMENTED YET
    ASSERT(color_pipe != NULL);

    //PROCESS WHOLE LINE
    return write_zxstream_line( color_pipe);
}

/**
 * @brief compares two pixels of arb. size for equality, tuned
 *        for sRGB & sGray path.
 * @param pixel ptr to PixelA
 * @param curr_pixel ptr to PixelB
 * @param bytes_per_pixel # of bytes in Pixel ASSUMES 1 or 3
 *                        pixels per byte.
 * 
 * @return bool if equal returns true, else false.
 */
static bool compare_pixel(uint8_t * pixel, uint8_t * curr_pixel, uint32_t bytes_per_pixel)
{
    if (bytes_per_pixel > 1) 
    {
        if (pixel[0] == curr_pixel[0] && 
            pixel[1] == curr_pixel[1] &&
            pixel[2] == curr_pixel[2]) 
        {
            return true;
        }
        return false;
    }
    else
    {
        if (*pixel == *curr_pixel) 
        {
            return true;
        }
        return false;
    }
}
/**
 * Start of helper functions for ZzStream RLE compression
 */

//Maximum size before needing to do a long run or straight.

#define MAX_SHORT ZxSTREAM_STRAIGHT16

/**
 * @brief Puts a ZxStream short run into the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count Number of pixels in short run
 * @param ot_ptr ptr to object tag
 * @param pixel ptr to pixel
 */
static void do_short_run(color_pipe_t * color_pipe, uint32_t count, object_tag_t *ot_ptr, uint8_t *pixel)
{
    uint8_t *ptr;

    *color_pipe->comp.curr_pos++ = (uint8_t) ((0 - ((uint8_t)count)));
    
    if (color_pipe->bytes_per_pixel > 1) 
    {
        *color_pipe->comp.curr_pos++ = *pixel++;
        *color_pipe->comp.curr_pos++ = *pixel++;
        *color_pipe->comp.curr_pos++ = *pixel++;
        color_pipe->comp.num_bytes_in_line += 4;
        ptr = color_pipe->comp.curr_pos - 4;
        DPRINTF(DBG_RLE,("CP:Short Run line_data count %d %x %x %x %x\n",count, ptr[0], ptr[1], ptr[2], ptr[3]));
    }
    else
    {
        *color_pipe->comp.curr_pos++ = *pixel;
        color_pipe->comp.num_bytes_in_line += 2;
    }
}

/**
 * @brief Starts a ZxStream a long run into the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count number of pixels in start of long run
 * @param ot_ptr ptr to object tag
 * @param pixel ptr to pixel for long run
 */
static void start_long_run(color_pipe_t * color_pipe, uint32_t count, object_tag_t *ot_ptr, uint8_t *pixel)
{
    *color_pipe->comp.curr_pos++ = (uint8_t) (ZxSTREAM_RUN16);
    color_pipe->comp.len_pos = color_pipe->comp.curr_pos;
    color_pipe->comp.num_pixels = count;
    color_pipe->comp.curr_pos+= 2;
    color_pipe->comp.pixel = color_pipe->comp.curr_pos;
    if (color_pipe->bytes_per_pixel > 1) 
    {
        *color_pipe->comp.curr_pos++ = *pixel++;
        *color_pipe->comp.curr_pos++ = *pixel++;
        *color_pipe->comp.curr_pos++ = *pixel++;
        //uint8_t *ptr = color_pipe->comp.curr_pos - 6;
        DPRINTF( DBG_RLE,("CP:Long Start Run line_data count %d \n",count));
    }
    else
    {
        *color_pipe->comp.curr_pos++ = *pixel;
    }
}

/**
 * @brief Adds to a ZxStream long run in the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count Number of pixels to be added to this long run.
 */
static void add_to_long_run(color_pipe_t * color_pipe, uint32_t count)
{
    DPRINTF(DBG_RLE,("CP:Long Cont Run line_data count %d \n",count));
    color_pipe->comp.num_pixels += count;
}

/**
 * @brief Ends a ZxStream Long run in the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count Number of pixels to be added to this long run.
 *              May be zero.
 */
static void end_long_run(color_pipe_t * color_pipe, uint32_t count)
{
    uint8_t *ptr;

    color_pipe->comp.num_pixels += count;
    //*color_pipe->comp.len_pos++ = (uint8_t) (color_pipe->comp.num_pixels >> 8 & 0xff);// high byte;
    *color_pipe->comp.len_pos++ = color_pipe->comp.num_pixels / 256;
    //*color_pipe->comp.len_pos++ = (uint8_t) (color_pipe->comp.num_pixels & 0xff); //low byte;
    *color_pipe->comp.len_pos++ = color_pipe->comp.num_pixels % 256;
    ptr = color_pipe->comp.curr_pos - 6;
    DPRINTF(DBG_RLE,("CP:Long End Run line_data count %d %d %x %x %x %x %x %x\n",color_pipe->comp.num_pixels, count,ptr[0],  ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]));
    color_pipe->comp.num_bytes_in_line += color_pipe->bytes_per_pixel + 3;
}

/**
 * @brief Puts a ZxStream short straight into the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count  Number of pixels in this short straight.
 * @param ot_ptr ptr to object tag.
 * @param pixel ptr to pixel array for short straight. Assumes
 *              they are all there.
 */
static void do_short_straight(color_pipe_t * color_pipe, uint32_t count, object_tag_t *ot_ptr, uint8_t *pixel)
{
    uint8_t *ptr;

    *color_pipe->comp.curr_pos++ = (uint8_t) count;
    int i;
    
    if (color_pipe->bytes_per_pixel > 1) 
    {
        for (i = 0; i < count; i++) 
        {
            *color_pipe->comp.curr_pos++ = *pixel++;
            *color_pipe->comp.curr_pos++ = *pixel++;
            *color_pipe->comp.curr_pos++ = *pixel++;
        }
        ptr = color_pipe->comp.curr_pos - (count * 3 + 1);
        DPRINTF(DBG_RLE,("CP:Short Straight line_data count %d %x %x %x %x\n",count, ptr[0], ptr[1], ptr[2], ptr[3]));
        color_pipe->comp.num_bytes_in_line += count * 3 + 1;
    }
    else
    {
        for (i = 0; i < count; i++) 
        {
            *color_pipe->comp.curr_pos++ = *pixel;
        }
        color_pipe->comp.num_bytes_in_line += count + 1;
    }
}

/**
 * @brief Starts a ZxStream long straight in the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count  number of pixels in the start of this long
 *               straight.
 * @param ot_ptr ptr to object tag
 * @param pixel ptr to pixel array of this long straight.
 *              Assumes they are all there.
 */
static void start_long_straight(color_pipe_t * color_pipe, uint32_t count, object_tag_t *ot_ptr, uint8_t *pixel)
{
    int i;
    DPRINTF(DBG_RLE,("CP:Long Start Straight line_data count %d \n",count));
    *color_pipe->comp.curr_pos++ = (uint8_t) (ZxSTREAM_STRAIGHT16);
    color_pipe->comp.len_pos = color_pipe->comp.curr_pos;
    color_pipe->comp.num_pixels = count;
    color_pipe->comp.curr_pos+= 2;
    if (color_pipe->bytes_per_pixel > 1) 
    {
        for (i = 0; i < count; i++)
        {
            *color_pipe->comp.curr_pos++ = *pixel++;
            *color_pipe->comp.curr_pos++ = *pixel++;
            *color_pipe->comp.curr_pos++ = *pixel++;
        }
        
    }
    else
    {
        for (i = 0; i < count; i++)
        {
            *color_pipe->comp.curr_pos++ = *pixel;
        }
    }
}

/**
 * @brief Adds to a ZxStream long straight in the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count number of pixels to be added to current long
 *              straight.
 * @param ptr to pixel array to be added to this long straight.
 *              Assumes they are all there.
 */
static void add_to_long_straight(color_pipe_t * color_pipe, uint32_t count, uint8_t *pixel)
{
    int i;
    DPRINTF(DBG_RLE,("CP:Long Cont Straight line_data count %d \n",count));
    color_pipe->comp.num_pixels += count;
    if (color_pipe->bytes_per_pixel > 1) 
    {
        for (i = 0; i < count; i++)
        {
            *color_pipe->comp.curr_pos++ = *pixel++;
            *color_pipe->comp.curr_pos++ = *pixel++;
            *color_pipe->comp.curr_pos++ = *pixel++;
        }
        
    }
    else
    {
        for (i = 0; i < count; i++)
        {
            *color_pipe->comp.curr_pos++ = *pixel;
        }
    }
}

/**
 * @brief Finishes a ZxStream long straight in the line buffer.
 * @param color_pipe ptr to color_pipe
 * @param count number of pixels to be added to finish this long
 *              straight.  It may be zero.
 * @param pixel ptr to pixel array of this long straight. May be
 *              NULL. Assumes they are all there.
 */
static void end_long_straight(color_pipe_t * color_pipe, uint32_t count, uint8_t *pixel)
{
    int i;
    uint8_t *ptr;

    color_pipe->comp.num_pixels += count;
    if (count > 0) 
    {
        if (color_pipe->bytes_per_pixel > 1) 
        {
            for (i = 0; i < count; i++)
            {
                *color_pipe->comp.curr_pos++ = *pixel++;
                *color_pipe->comp.curr_pos++ = *pixel++;
                *color_pipe->comp.curr_pos++ = *pixel++;
            }
        
        }
        else
        {
            for (i = 0; i < count; i++)
            {
                *color_pipe->comp.curr_pos++ = *pixel;
            }
        }
    }
    //*color_pipe->comp.len_pos++ = (uint8_t) (color_pipe->comp.num_pixels >> 8 & 0xff);// high byte;
    *color_pipe->comp.len_pos++ = (color_pipe->comp.num_pixels)/ 256;
    //*color_pipe->comp.len_pos++ = (uint8_t) (color_pipe->comp.num_pixels & 0xff); //low byte;
    *color_pipe->comp.len_pos++ = (color_pipe->comp.num_pixels)% 256;
    ptr = color_pipe->comp.curr_pos - (color_pipe->comp.num_pixels * color_pipe->bytes_per_pixel + 3);
    DPRINTF(DBG_RLE,("CP:Long End Straight line_data count %d %d %x %x %x %x %x %x\n",color_pipe->comp.num_pixels, count, ptr[0],  ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]));
    color_pipe->comp.num_bytes_in_line += color_pipe->comp.num_pixels * color_pipe->bytes_per_pixel + 3;
}

/**
 * @brief Puts a ZxStream End of Line byte into the line buffer.
 * @param color_pipe ptr to color_pipe
 */
static void add_eol(color_pipe_t * color_pipe)
{
    uint8_t *ptr;

    *color_pipe->comp.curr_pos++ = 0;
    color_pipe->comp.num_bytes_in_line += 1;
    ptr = color_pipe->comp.curr_pos - (1);
    DPRINTF(DBG_RLE,("CP:End of Line %x\n",ptr[0]));
}

/**
 * @brief inplements the repeated_pixel to ZxStream state table
 *        closing long runs and straights and starting new runs
 *        if needed.
 * @param color_pipe ptr to color_pipe
 * @param count  number of repeated pixels to be added.
 * @param ot_ptr ptr to object tag.
 * @param pixel ptr to pixel
 */
void color_pipe_process_repeated_pixels(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t *pixel)  
{
    //DPRINTF(DBG_LOUD|DBG_OUTPUT,("CP:run %d pixels\n",count));
    ASSERT(color_pipe != NULL);
    if (color_pipe->comp.state == e_between_lines) // start of new line
    {
        color_pipe->comp.num_bytes_in_line = 0;
        color_pipe->comp.curr_pos = color_pipe->line_data;
        color_pipe->comp.state = e_idle;
    }
    if (count >= MAX_SHORT) //long run 
    {
        if (color_pipe->comp.state == e_idle) 
        {
            start_long_run(color_pipe, count, ot_ptr,pixel);
            color_pipe->comp.state = e_long_run;
        }
        else if (color_pipe->comp.state == e_long_straight) 
        {
            end_long_straight(color_pipe, 0, NULL);
            start_long_run(color_pipe, count, ot_ptr,pixel);
            color_pipe->comp.state = e_long_run;
        }
        else if (color_pipe->comp.state == e_long_run) 
        {
            if (compare_pixel(pixel, color_pipe->comp.pixel, color_pipe->bytes_per_pixel)) 
            {
                add_to_long_run(color_pipe, count);
            }
            else
            {
                end_long_run(color_pipe, 0);
                start_long_run(color_pipe, count, ot_ptr, pixel);
            }
        }
        else
        {
            ASSERT(0);//cant get here
        }
    }
    else
    {
        if (color_pipe->comp.state == e_idle) 
        {
            do_short_run(color_pipe, count, ot_ptr, pixel);
        }
        else if (color_pipe->comp.state == e_long_straight) 
        {
            end_long_straight(color_pipe, 0, NULL);
            do_short_run(color_pipe, count, ot_ptr, pixel);
        }
        else if (color_pipe->comp.state == e_long_run) 
        {
            if (compare_pixel(pixel, color_pipe->comp.pixel, color_pipe->bytes_per_pixel)) 
            {
                end_long_run(color_pipe, count);
            }
            else
            {
                end_long_run(color_pipe, 0);
                do_short_run(color_pipe, count, ot_ptr, pixel);
            }
        }
        else
        {
            ASSERT(0);//cant get here
        }
        color_pipe->comp.state = e_idle;
    }   
}
/**
 * @brief inplements the multiple_pixel to ZxStream state table
 *        closing long runs and starting new or adding to open
 *        straights if needed.
 * @param color_pipe ptr to color_pipe
 * @param count length of pixel array to be added.
 * @param ot_ptr ptr to object tag
 * @param pixel pixel array
 */
void color_pipe_process_multiple_pixels(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t *pixel)  
{
    ASSERT(color_pipe != NULL);
    //DPRINTF(DBG_LOUD|DBG_OUTPUT,("CP:straight %d pixels\n",count));
    if (color_pipe->comp.state == e_between_lines) // start of new line
    {
        color_pipe->comp.num_bytes_in_line = 0;
        color_pipe->comp.curr_pos = color_pipe->line_data;
        color_pipe->comp.state = e_idle;
    }

    if (count >= MAX_SHORT) //long run 
    {
        if (color_pipe->comp.state == e_idle) 
        {
            start_long_straight(color_pipe, count, ot_ptr,pixel);
            color_pipe->comp.state = e_long_straight;
        }
        else if (color_pipe->comp.state == e_long_straight) 
        {
            add_to_long_straight(color_pipe, count, pixel);
        }
        else if (color_pipe->comp.state == e_long_run) 
        {
            end_long_run(color_pipe, 0);
            start_long_straight(color_pipe, count, ot_ptr,pixel);
            color_pipe->comp.state = e_long_straight;
        }
        else
        {
            ASSERT(0);//cant get here
        }
    }
    else
    {
        if (color_pipe->comp.state == e_idle) 
        {
            do_short_straight(color_pipe, count, ot_ptr, pixel);
        }
        else if (color_pipe->comp.state == e_long_straight) 
        {
            end_long_straight(color_pipe, count, pixel);
        }
        else if (color_pipe->comp.state == e_long_run) 
        {
            end_long_run(color_pipe, 0);
            do_short_straight(color_pipe, count, ot_ptr, pixel);
        }
        else
        {
            ASSERT(0);//cant get here
        }
        color_pipe->comp.state = e_idle;
    }
}
/**
 * @brief inplements the multiple_pixel to ZxStream state table
 *        closing long runs and starting new or adding to open
 *        straights if needed. NOT IMPLEMENTED YET
 * @param color_pipe ptr to color_pipe
 * @param count length of pixel array to be added.
 * @param ot_ptr ptr to object tag
 * @param pixel pixel array
 */
void color_pipe_process_multiple_pixels_unsorted(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t *pixel)  
{
    ASSERT(0);//unsupported
    ASSERT(color_pipe != NULL);
    if (color_pipe->comp.curr_pos == NULL) // start of new line
    {
        color_pipe->comp.num_bytes_in_line = 0;
        color_pipe->comp.curr_pos = color_pipe->line_data;
        color_pipe->comp.state = e_idle;
    }
}
/**
 * @brief See header file
 * @param color_pipe ptr to color_pipe
 * @param HT_if_true if true add line to strip buffer, allows
 *                   for vertical clipping
 * 
 * @return bool returns if strip buffer full
 */
bool color_pipe_eol(color_pipe_t * color_pipe, bool HT_if_true)
                                                                                        
{

    ASSERT(color_pipe != NULL);
    if (color_pipe->comp.state != e_between_lines) {
        if (color_pipe->comp.state == e_idle) 
        {
            //Nothing needed
        }
        else if (color_pipe->comp.state == e_long_straight) 
        {
            end_long_straight(color_pipe, 0, NULL);
        }
        else if (color_pipe->comp.state == e_long_run) 
        {
            end_long_run(color_pipe, 0);
        }
        else
        {
            ASSERT(0);//cant get here
        }
        add_eol(color_pipe);

        color_pipe->comp.state = e_between_lines;
    }
    if (HT_if_true) 
    {
        return write_zxstream_line(color_pipe);
    }
    else
    {
        return COLOR_PIPE_STRIP_NOT_FULL;
    }   
}

/**
 * @brief See header file
 * @param color_pipe ptr to color_pipe
 */
void color_pipe_eop(color_pipe_t * color_pipe) 
{

    ASSERT(color_pipe != NULL);
    DPRINTF(DBG_LOUD|DBG_OUTPUT,("CP:Flushing last strip\n"));
    if (color_pipe->num_bytes_written > 0) //partial strip
    {
        if (color_pipe->strip_halftoned_callback != NULL)
        {
            DPRINTF(DBG_CPIPE,("CP:Flushing last strip data\n"));
            (*color_pipe->strip_halftoned_callback)(color_pipe->strip_callback_private_data, color_pipe->color_planes_out, color_pipe->num_bytes_written); 
        } 
    }
   
}
/**
 * @brief See header file
 * @param color_pipe ptr to color_pipe
 * @param weighted_pixels
 * @param nonwhite_pixels
 * @param white_pixels
 * @param reset_counts
 */
void color_pipe_get_pixel_counts(color_pipe_t * color_pipe,
                                 cmyk_uint32_t * weighted_pixels, 
                                 cmyk_uint32_t * nonwhite_pixels, 
                                 cmyk_uint32_t * white_pixels, 
                                 bool reset_counts)
{

    ASSERT(color_pipe != NULL);
    DPRINTF(DBG_LOUD|DBG_OUTPUT,("colorpipe: color_pipe_get_pixel_counts not implemented yet\n"));
}

/**
 * @brief See header file
 * 
 * @param color_pipe ptr to color_pipe
 * 
 * @return color_pipe_t*
 */
color_pipe_t * color_pipe_flush(color_pipe_t * color_pipe)
{

    //ASSERT(color_pipe != NULL);  parsers don't check

    if ( color_pipe )
    {
        MEM_FREE_AND_NULL(color_pipe->line_data);
    }

    return NULL;
}

extern void color_pipe_local_init( color_pipe_t ** color_pipe );

/**
 * @brief init color pipe.
 * @param color_pipeptr to color_pipe
 */
static void color_pipe_register(color_pipe_t * color_pipe)
{
    default_color_pipe = color_pipe;
}

#if 0
static void print_color_planes_dbg(cmyk_byte_array_t * color_planes_out,uint32_t offset)
{
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("offset %d \n\n",offset));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("color_pipe:color_planes_out.pC %2x %2x %2x %2x %2x %2x %2x %2x  \n",
                color_planes_out->pC[0 + offset], color_planes_out->pC[1 + offset], color_planes_out->pC[2 + offset], color_planes_out->pC[3 + offset],color_planes_out->pC[4 + offset], color_planes_out->pC[5 + offset], color_planes_out->pC[6 + offset], color_planes_out->pC[7 + offset]));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("color_pipe:color_planes_out.pM %2x %2x %2x %2x %2x %2x %2x %2x \n",
                color_planes_out->pM[0 + offset], color_planes_out->pM[1 + offset], color_planes_out->pM[2 + offset], color_planes_out->pM[3 + offset],color_planes_out->pM[4 + offset], color_planes_out->pM[5 + offset], color_planes_out->pM[6 + offset], color_planes_out->pM[7] + offset));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("color_pipe:color_planes_out.pY %2x %2x %2x %2x %2x %2x %2x %2x \n",
                color_planes_out->pY[0 + offset], color_planes_out->pY[1 + offset], color_planes_out->pY[2 + offset], color_planes_out->pY[3 + offset],color_planes_out->pY[4 + offset], color_planes_out->pY[5 + offset], color_planes_out->pY[6 + offset], color_planes_out->pY[7 + offset]));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("color_pipe:color_planes_out.pK %2x %2x %2x %2x %2x %2x %2x %2x \n",
                color_planes_out->pK[0 + offset], color_planes_out->pK[1 + offset], color_planes_out->pK[2 + offset], color_planes_out->pK[3 + offset],color_planes_out->pK[4 + offset], color_planes_out->pK[5 + offset], color_planes_out->pK[6 + offset], color_planes_out->pK[7 + offset]));
}


static void dbg_strip_halftoned_callback ( void *callback_private_data, cmyk_byte_array_t *color_planes_out ) 
{

    cmyk_byte_array_t ** done_ptr = (cmyk_byte_array_t **) callback_private_data;

    *done_ptr = color_planes_out;
    /*send HT done to msg to task*/
    DPRINTF(DBG_CPIPE,("debug:color_pipe halftone complete %x done\n\n",color_planes_out));
}
#endif
int color_pipe_dbcallback( int argc, char *argv[] )
{
#if 0
    int test;
    int pblack;
    int bpp;
    color_pipe_t * color_pipe;
    uint8_t black_pixel[3] = { 0x0, 0x0, 0x0 };
    
    static const uint8_t line[32*3] = 
        {   0xff,0xff,0xff,
            0xff,0xff,0xff,
            0xff,0xff,0xff,
            0xff,0xff,0xff,
            0xff,0xff,0xff,
            0x0,0x0,0x0,
            0x0,0x0,0x0,
            0x0,0x0,0x0,

            0x0,0x0,0x0,
            0x0,0x0,0x0,
            0x0,0x0,0x0,
            0x1,0x2,0x3,
            0x33,0x34,0x35,
            0x56,0x57,0x58,
            0x0,0x0,0x0,
            0x0,0x0,0x0,

            0x79,0x7a,0x7b,
            0x9c,0x9d,0x9e,
            0xbf,0xb0,0xb1,
            0xd2,0xd3,0xd4,
            0xe5,0xe6,0xe7,
            0xff,0xff,0xff,
            0xff,0xff,0xff,
            0xff,0xff,0xff,

            0x00,0x00,0xff,
            0x00,0xff,0x00,
            0x00,0xff,0xff,
            0xff,0x00,0x00,
            0xff,0x00,0xff,
            0xff,0xff,0x00,
            0xff,0xff,0xff,
            0xff,0xff,0xff
    };
    object_tag_t ot;
    cmyk_byte_array_t color_planes_out;

    //int temp;

    if (argc < 4 )
    {
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("debug:color_pipe <run test number x> <Pblack=1 konly = 0> <bpp 1/2>\n"));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("debug:color_pipe Test 1 line, Test 2 RLE, Test 3 Strip \n"));
        
        /* not enough parameters */
        return 0;
    }

    test = atoi(argv[1]);
    pblack = atoi(argv[2]);
    bpp = atoi(argv[3]);

     
    switch (test ) 
    {
    case 1:
    {
        color_table_set_defaults( );
        color_table_set_output_bits_per_pixel( bpp );
        color_pipe =  color_pipe_page_open(bpp, 
                                           e_color_space_3_to_4, 
                                           32,
                                           (32*bpp)/8,
                                           1,
                                           1,
                                           e_color_pipe_default);
        ot.line = NULL;
        ot.Pblack = (pblack == 1);
        color_planes_out.pC = (uint8_t *) MEM_MALLOC(100);
        color_planes_out.pM = (uint8_t *) MEM_MALLOC(100);
        color_planes_out.pY = (uint8_t *) MEM_MALLOC(100);
        color_planes_out.pK = (uint8_t *) MEM_MALLOC(100);
        color_pipe_new_halftone_strip(color_pipe, 
                                      &color_planes_out,
                                      NULL, 
                                      NULL);
        color_pipe_process_line(color_pipe,  
                                (uint8_t *)line,
                                &ot);
        color_pipe = color_pipe_flush(color_pipe);
        print_color_planes_dbg(&color_planes_out, 0 );
        MEM_FREE_AND_NULL(color_planes_out.pC);
        MEM_FREE_AND_NULL(color_planes_out.pM);
        MEM_FREE_AND_NULL(color_planes_out.pY);
        MEM_FREE_AND_NULL(color_planes_out.pK);
        break;
    }
    case 2:
    {
        int i;
        bool eol;
        uint8_t * line_ptr = (uint8_t *) line;
        color_table_set_defaults( );
        color_table_set_output_bits_per_pixel( bpp );
        color_pipe =  color_pipe_page_open(bpp, 
                                           e_color_space_3_to_4, 
                                           5104 /*32*/,
                                           (5104*bpp)/8 ,
                                           4,
                                           2,
                                           e_color_pipe_default);
        ot.line = NULL;
        ot.Pblack = (pblack == 1);
        color_planes_out.pC = (uint8_t *) MEM_MALLOC(4*(5104*bpp)/8);
        color_planes_out.pM = (uint8_t *) MEM_MALLOC(4*(5104*bpp)/8);
        color_planes_out.pY = (uint8_t *) MEM_MALLOC(4*(5104*bpp)/8);
        color_planes_out.pK = (uint8_t *) MEM_MALLOC(4*(5104*bpp)/8);

         
        color_pipe_new_halftone_strip(color_pipe, 
                                      &color_planes_out,
                                      NULL, 
                                      NULL);
        color_pipe_process_repeated_pixels( color_pipe, 5, &ot, line_ptr);
        line_ptr += 5*3;
        color_pipe_process_repeated_pixels( color_pipe, 6, &ot, line_ptr);
        line_ptr += 6*3;
        for ( i = 0; i < 3 ; i++ ) 
        {
            color_pipe_process_repeated_pixels( color_pipe, 1, &ot, line_ptr);
            line_ptr += 3;
        }
        color_pipe_process_repeated_pixels( color_pipe, 2, &ot, line_ptr);
        line_ptr += 2*3;
        for ( i = 0; i < 5; i++ ) 
        {
            color_pipe_process_repeated_pixels( color_pipe, 1, &ot, line_ptr);
            line_ptr += 3;
        }
        color_pipe_process_repeated_pixels( color_pipe, 3, &ot, line_ptr);
        line_ptr += 3*3;
        for (i=0;i<8;i++) 
        {
            color_pipe_process_repeated_pixels(color_pipe, 1, &ot, line_ptr);
            line_ptr += 3;
        }
        int num_black_pixels;
        if (argc > 4 )
        {
            num_black_pixels = atoi(argv[4]);
        }
        else
        {
            num_black_pixels = 2007;
        }
        color_pipe_process_repeated_pixels(color_pipe, num_black_pixels, &ot, black_pixel);
        eol = color_pipe_eol( color_pipe, true );
#if 0
        color_pipe_new_halftone_strip(color_pipe, 
                                      &color_planes_out,
                                      NULL, 
                                      NULL);
#endif 
        print_color_planes_dbg(&color_planes_out, 0);
        eol = color_pipe_eol( color_pipe, true );
        print_color_planes_dbg(&color_planes_out, 0);
        if (argc > 5 )
        {
            num_black_pixels = atoi(argv[5]);
        }
        else
        {
            num_black_pixels = 1000;
        }
        color_pipe_process_repeated_pixels(color_pipe, num_black_pixels, &ot, black_pixel);
        eol = color_pipe_eol( color_pipe, true );
        eol = color_pipe_eol( color_pipe, true );
        color_pipe_eop( color_pipe );
        color_pipe = color_pipe_flush( color_pipe );
        MEM_FREE_AND_NULL(color_planes_out.pC);
        MEM_FREE_AND_NULL(color_planes_out.pM);
        MEM_FREE_AND_NULL(color_planes_out.pY);
        MEM_FREE_AND_NULL(color_planes_out.pK);
        break;
    }   
    case 3:
    {
        cmyk_byte_array_t * strip_in_pipe = NULL;
        cmyk_byte_array_t * strip_waiting_for_HT = NULL;
        cmyk_byte_array_t * strip_HTed = NULL;
        cmyk_byte_array_t * strip_compressed = &color_planes_out;
        uint32_t page_y = 0;
#define TEST_PAGE_HEIGHT 103
        color_table_set_defaults( );
        color_table_set_output_bits_per_pixel( bpp );
        color_pipe =  color_pipe_page_open(bpp, 
                                           e_color_space_3_to_4, 
                                           32,
                                           (32*bpp)/8,
                                           10,
                                           1,
                                           e_color_pipe_default);
        ot.line = NULL;
        ot.Pblack = (pblack == 1);
        color_planes_out.pC = (uint8_t *) MEM_MALLOC(100);
        color_planes_out.pM = (uint8_t *) MEM_MALLOC(100);
        color_planes_out.pY = (uint8_t *) MEM_MALLOC(100);
        color_planes_out.pK = (uint8_t *) MEM_MALLOC(100);
        
        do //get next msg
        {
            if (strip_in_pipe == NULL) /* and strip available */
            {
                DPRINTF(DBG_LOUD|DBG_OUTPUT,("debug:color_pipe New Strip %d added\n\n",strip_compressed ));
                color_pipe_new_halftone_strip(color_pipe, 
                                              strip_compressed,
                                              dbg_strip_halftoned_callback, 
                                              &strip_HTed);
                strip_in_pipe = strip_compressed; //pop strip off empty queue
                strip_compressed = NULL;
            }
            /* while ((line to process) and (strip_in_pipe != NULL))*/
            if (!color_pipe_process_line(color_pipe, (uint8_t *)line, &ot))
            {
                DPRINTF(DBG_LOUD|DBG_OUTPUT,("debug:color_pipe Strip %d done\n\n",page_y / 10));
                print_color_planes_dbg(&color_planes_out,(32*bpp/8) * (page_y % 10));
                page_y++;
                strip_waiting_for_HT = strip_in_pipe; //push strip on waiting queue
                strip_in_pipe = NULL;
            }
            else
            {
                print_color_planes_dbg(&color_planes_out,(32*bpp/8) * (page_y % 10));
                page_y++;
            }
            
            if ((strip_waiting_for_HT != NULL) && (strip_waiting_for_HT == strip_HTed)) 
            {
                strip_compressed = strip_waiting_for_HT; //push strip on compression queue
                strip_waiting_for_HT = NULL;
                strip_HTed = NULL; //pop strip_HTed
            }
        } while (  page_y <  TEST_PAGE_HEIGHT );

        color_pipe_eop( color_pipe );
        
        color_pipe = color_pipe_flush(color_pipe);
        print_color_planes_dbg(&color_planes_out,(32*bpp/8) * 3);
        print_color_planes_dbg(&color_planes_out,(32*bpp/8) * 4);
        print_color_planes_dbg(&color_planes_out,(32*bpp/8) * 5);
        MEM_FREE_AND_NULL(color_planes_out.pC);
        MEM_FREE_AND_NULL(color_planes_out.pM);
        MEM_FREE_AND_NULL(color_planes_out.pY);
        MEM_FREE_AND_NULL(color_planes_out.pK);
        break;
    }
    }
#endif
    return 0;
}

/**
 * @brief color pipe init code.
 * Must be called after tasks have started.
 */
void color_pipe_init(void)

{

    if (default_color_pipe != NULL) 
        return;
    color_pipe_t * color_pipe = NULL; 
    color_pipe_local_init(&color_pipe);
    color_pipe_register(color_pipe);
    color_pipe->mlimiter = mlimiter_by_name("print");
    color_table_init_ACL();
    color_pipe->line_data = NULL;
    ASSERT( CMD_OK == cmd_register("cpipe",
                                   NULL,
                                   NULL,
                                   NULL,
                                   color_pipe_dbcallback));

}



