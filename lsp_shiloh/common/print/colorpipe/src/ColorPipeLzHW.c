/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file ColorPipeLzHW.c
 *
 *
 *****************************************************************************/
#include "ATypes.h"
#include "agJobUtils.h"
#include "ColorPipeTables.h"
#include "ColorPipe.h"
#include "colorPipeLzHWPriv.h"
#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include "memAPI.h" 
#include <stdlib.h>
#include <string.h>
#include "cmd_proc_api.h"

static color_pipe_t * default_color_pipe = NULL;

void color_pipe_init(void);

color_pipe_t * color_pipe_page_open( uint32_t output_bits_per_pixel, 
                                     color_space_conv_t color_space_conv,
                                     uint32_t pixels_per_line,
                                     uint32_t output_pitch,
                                     uint32_t lines_per_strip,
                                     uint32_t x_scale, 
                                     color_pipe_mode_t mode)
{
    uint32_t i;
    color_pipe_t * color_pipe;

    ASSERT(pixels_per_line < 0x3fff);
    ASSERT(output_pitch >= (pixels_per_line * output_bits_per_pixel)/8);

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
#ifdef MONO_ENGINE 
#define CP_STRIP_BYTES_PIXEL 1
#else
#define CP_STRIP_BYTES_PIXEL 3
#endif 
    color_pipe->color_space_conv = color_space_conv;
    color_pipe->curr_line = 0;
    color_pipe->output_strip_pitch = output_pitch;
    color_pipe->lines_per_strip = lines_per_strip;
    color_pipe->color_planes_out = NULL;
    if (color_space_conv == e_color_space_3_to_4) 
    {
        color_pipe->bytes_per_pixel = 3;
    }
    else
    {
        color_pipe->bytes_per_pixel = 1;
    }
    if (!color_pipe->strip[0].contone_strip_data) 
    {
        for (i = 0; i < 2; i++) 
        {
            color_pipe->strip[i].contone_strip_data = MEM_MALLOC_ALIGN_LIMITED(color_pipe->mlimiter,
                                                                               CP_MAX_LINE_LEN * CP_STRIP_BYTES_PIXEL * CP_MAX_LINES_STRIP,
                                                                               CP_STRIP_ALIGN);
            color_pipe->strip[i].color_space_conv = color_space_conv;
            color_pipe->strip[i].state = IDLE;
            color_pipe->strip[i].halftoned_strip_data = NULL;
            color_pipe->strip[i].curr_line_ptr = NULL;
            color_pipe->strip[i].curr_line_pos = 0;
            color_pipe->strip[i].last_line_ptr = NULL;
        }
    }
    
#ifdef NEEDTODO
/*
INIT HW COLORPIPE
malloc space PING PONG CSC STRIP BUFFERS. Need one for the one you fill and the one you are ripping
*/
    
    
    /*
    Do something here for synco with Parser, may use events
    */
#endif
    
    return(color_pipe);

}



void color_pipe_new_halftone_strip(color_pipe_t * color_pipe, 
                                   cmyk_byte_array_t *color_planes_out,
                                   void(*strip_halftoned_callback)( void *callback_private_data, cmyk_byte_array_t *color_planes_out, uint32_t size  ), 
                                   void *callback_private_data  )
{
    ASSERT(color_pipe != NULL);

    color_pipe->color_planes_out = color_planes_out;
    color_pipe->strip_halftoned_callback = strip_halftoned_callback;
    color_pipe->strip_callback_private_data = callback_private_data;
    while(!color_pipe->curr_strip_ptr)
    {
        if (color_pipe->strip[0].state == IDLE && color_pipe->strip[0].last_line_ptr == NULL) 
        {
            color_pipe->curr_strip_ptr = &color_pipe->strip[0];
        }
        else if (color_pipe->strip[1].state == IDLE && color_pipe->strip[1].last_line_ptr == NULL)
        {
            color_pipe->curr_strip_ptr = &color_pipe->strip[0];
        }
        else
        {
            //Wait for strip to complete
        }
    }
    color_pipe->curr_strip_ptr->state = FILLING;
    color_pipe->curr_strip_ptr->curr_line_ptr = color_pipe->curr_strip_ptr->contone_strip_data;
    color_pipe->curr_strip_ptr->curr_line_pos = color_pipe->curr_strip_ptr->contone_strip_data;
    color_pipe->curr_strip_ptr->halftoned_strip_data = color_planes_out;
}


static bool halftone_lines(color_pipe_t * color_pipe, uint32_t num_lines)
{

    ASSERT(color_pipe != NULL);
    ASSERT(num_lines == 1);

    
    
    color_pipe->curr_line += num_lines;
    if (color_pipe->curr_line == color_pipe->lines_per_strip) 
    {
        color_pipe->curr_strip_ptr->state = CSC;
        
        //NEED TO SEND STRIP TO CSC HERE
        
        //if (color_pipe->strip_halftoned_callback != NULL)
        //{
        //    (*color_pipe->strip_halftoned_callback)(color_pipe->strip_callback_private_data, color_pipe->color_planes_out); 
        //}
        color_pipe->curr_line = 0;
        color_pipe->color_planes_out = NULL;
        return (COLOR_PIPE_STRIP_FULL);
    }
    if (color_pipe->curr_line > color_pipe->lines_per_strip) 
    {
        ASSERT(0);
    }

    return (COLOR_PIPE_STRIP_NOT_FULL);
}


bool color_pipe_process_line(color_pipe_t * color_pipe,  
                             uint8_t *line_ptr,                       //inputs 3 byte/pixel, packed RGB or 1 byte/pixel grey
                             object_tag_t *ot_ptr)                  //  1byte/pixel, packed OT or boolean flag.
                             
{
    ASSERT(color_pipe != NULL);
    
    memcpy(color_pipe->curr_strip_ptr->curr_line_ptr, line_ptr,color_pipe->pixel_per_line);
    if (color_pipe->pixel_per_line < color_pipe->csc_pitch) 
    {
        memset(color_pipe->curr_strip_ptr->curr_line_ptr + color_pipe->pixel_per_line, 0, color_pipe->csc_pitch - color_pipe->pixel_per_line);
    }
    color_pipe->num_pixels = color_pipe->csc_pitch;
    return halftone_lines( color_pipe, 1);
}

void color_pipe_process_repeated_pixels(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t *pixel)  
{
    uint32_t i;
    ASSERT(color_pipe != NULL);
    color_pipe->num_pixels += count;
    if (color_pipe->bytes_per_pixel == 3) 
    {
        for (i = 0; i < count; i++) 
        {
            *color_pipe->curr_strip_ptr->curr_line_pos++ = pixel[0];
            *color_pipe->curr_strip_ptr->curr_line_pos++ = pixel[1];
            *color_pipe->curr_strip_ptr->curr_line_pos++ = pixel[2];
        }
    }
    else 
    {
        memset(color_pipe->curr_strip_ptr->curr_line_pos, *pixel, count);
        color_pipe->curr_strip_ptr->curr_line_pos += count;
    }
    
}

void color_pipe_process_multiple_pixels(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t *pixel)  
{
    uint32_t byte_cnt = color_pipe->bytes_per_pixel * count;
    ASSERT(color_pipe != NULL);
    color_pipe->num_pixels += count;
    memcpy(color_pipe->curr_strip_ptr->curr_line_pos, pixel, byte_cnt);
    color_pipe->curr_strip_ptr->curr_line_pos += byte_cnt;
}

void color_pipe_process_multiple_pixels_unsorted(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t *pixel)  
{
    ASSERT(color_pipe != NULL);
    color_pipe_process_multiple_pixels(color_pipe,count,ot_ptr,pixel); 
}
bool color_pipe_eol(color_pipe_t * color_pipe, bool HT_if_true)
                                                                                        
{
    ASSERT(color_pipe != NULL);

    if (color_pipe->num_pixels == 0 && HT_if_true) 
    {
        if (color_pipe->last_line_strip->last_line_ptr != NULL)
        {
            memcpy(color_pipe->curr_strip_ptr->curr_line_ptr,color_pipe->last_line_strip->last_line_ptr,color_pipe->csc_pitch);
        }
        else if (color_pipe->last_line_strip->last_line_ptr == color_pipe->curr_strip_ptr->curr_line_ptr) 
        {
            //do nothing
        }
        else
        {
            memset(color_pipe->curr_strip_ptr->curr_line_ptr,0xff,color_pipe->csc_pitch);
        }  
    }
    if (color_pipe->last_line_strip != NULL) 
    {
        color_pipe->last_line_strip->last_line_ptr = NULL;
    }
    
    color_pipe->last_line_strip = color_pipe->curr_strip_ptr;
    color_pipe->last_line_strip->last_line_ptr = color_pipe->curr_strip_ptr->curr_line_ptr;
    if (HT_if_true) 
    {
        color_pipe->curr_strip_ptr->curr_line_ptr += color_pipe->csc_pitch;
    }
        
    color_pipe->num_pixels = 0;
    color_pipe->curr_strip_ptr->curr_line_pos = color_pipe->curr_strip_ptr->curr_line_ptr;
    
    
    if (HT_if_true) 
    {
        return halftone_lines(color_pipe, 1);
    }
    else
    {
        return COLOR_PIPE_STRIP_NOT_FULL;
    }    
}
void color_pipe_eop(color_pipe_t * color_pipe) 
{
    ASSERT(color_pipe != NULL);

    if (color_pipe->curr_line != 0) //partial strip
    {
        static uint8_t white_pixel[] = {0xff,0xff,0xff};
        //color_transform_create_white_line(color_pipe->color_transform, &temp_RLE_color_planes);
        color_pipe_process_repeated_pixels(color_pipe,
                                           1, 
                                           NULL,
                                           &white_pixel[0]);
        while(color_pipe_eol(color_pipe, true) == COLOR_PIPE_STRIP_NOT_FULL)
        {
        }
    }  
}

void color_pipe_get_pixel_counts(color_pipe_t * color_pipe,
                                 cmyk_uint32_t * weighted_pixels, 
                                 cmyk_uint32_t * nonwhite_pixels, 
                                 cmyk_uint32_t * white_pixels, 
                                 bool reset_counts)
{
#if 0
    ASSERT(color_pipe != NULL);

    color_halftone_calc_pixel_counts(color_pipe->color_halftone, weighted_pixels, nonwhite_pixels, white_pixels, reset_counts);
#endif
}


color_pipe_t * color_pipe_flush(color_pipe_t * color_pipe)
{
    //ASSERT(color_pipe != NULL);

    if ( color_pipe )
    {
        //color_transform_page_flush(color_pipe->color_transform);
        //color_halftone_page_flush(color_pipe->color_halftone);
        MEM_FREE_AND_NULL(color_pipe->strip[0].contone_strip_data);
        MEM_FREE_AND_NULL(color_pipe->strip[1].contone_strip_data);
    }
    return NULL;
}

extern void color_pipe_local_init( color_pipe_t ** color_pipe );

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
#endif
#if 0
static void dbg_strip_halftoned_callback ( void *callback_private_data, cmyk_byte_array_t *color_planes_out ) 
{

    cmyk_byte_array_t ** done_ptr = (cmyk_byte_array_t **) callback_private_data;

    *done_ptr = color_planes_out;
    /*send HT done to msg to task*/
    DPRINTF(DBG_LOUD|DBG_OUTPUT,("debug:color_pipe halftone complete %x done\n\n",color_planes_out));
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

void color_pipe_init(void)

{
    if (default_color_pipe != NULL) 
        return;
    color_pipe_t * color_pipe = NULL; 
    color_pipe_local_init(&color_pipe);
    color_pipe_register(color_pipe);

    color_table_init_ACL();
    
    ASSERT( CMD_OK == cmd_register("cpipe",
                                   NULL,
                                   NULL,
                                   NULL,
                                   color_pipe_dbcallback));
}



