/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/// pip PLANE begin of file

#include "pip_strip_adaptor.h"
#include "pip_PLANE_strip_adaptor.h"
#include "logger.h"
#include "memAPI.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "dbg_pnm.h"
#include "print_job_api.h"
#include "pip_api.h"


#define DBG_PRFX "strip_adaptor: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT(4)


static BigBuffer_t *pip_PLANE_get_bb_in( struct pip_strip_adaptor *strip )
{
    if ( strip && ((struct pip_PLANE_strip_adaptor *)strip)->strip  )
	return ((struct pip_PLANE_strip_adaptor *)strip)->strip->big_buffer;
    return 0;
}
static void pip_PLANE_attach_from_strip( struct pip_strip_adaptor *base, void *from_blob )
{
    struct pip_PLANE_strip_adaptor *psa = (struct pip_PLANE_strip_adaptor *) base;
    
    // psa->is_start_of_page <== pip_new_page() sets
    psa->strip = (PLANED*) from_blob;
    psa->scale_y = psa->strip->image_info.image_dpi.x == 300 ? 2 : 1;
    psa->scale_x = psa->strip->image_info.image_dpi.y == 300 ? 2 : 1;

    if (psa->strip->BlockDataType == PACKED_RGB) 
	psa->width_pix_in = psa->strip->image_info.image_data_width_in_bytes / 3; 
    else
	psa->width_pix_in = psa->strip->image_info.image_width_in_pixels;

    psa->lines_in = psa->strip->image_info.image_height_in_lines;
    psa->width_pix_out = pip_padded_image_width(psa->width_pix_in, psa->scale_x);
    psa->out_strip_height = 128; // max output height will/may be converted to actual output size
    psa->is_end_of_page = psa->strip->LastData;
}
static void pip_PLANE_free_in_strip( struct pip_strip_adaptor *base )
{
    struct pip_PLANE_strip_adaptor *psa = (struct pip_PLANE_strip_adaptor *) base;
    memFree(psa->strip);
    psa->strip = 0;
}
static void pip_PLANE_mono_send_strip( struct pip_strip_adaptor *to_strip, BigBuffer_t *bb[6] )
{
    struct pip_PLANE_strip_adaptor *psa = (struct pip_PLANE_strip_adaptor *) to_strip;
    PLANED *plane = psa->strip;
    MESSAGE msg;

    if (0) { // DEBUG TOOL: hack in a line of black at the top of each strip.
        dma_buffer_map_single(bb[4], DMA_FROM_DEVICE);
        dma_buffer_unmap_single(bb[4], DMA_FROM_DEVICE);
        void *after = dma_buffer_mmap_forcpu(bb[4]);
        char *p = after;
        int i;
	int width = psa->width_pix_out * psa->output_bpp / 8;

        for (i=0; i < psa->out_strip_height; i++) {

            // print tick mark at begin of strip
            if ( 1 && i == 0 ) {
		p[0] = 0xff;
		p[1] = 0xff;
		p[2] = 0xff;
		p[3] = 0xff;
		p[4] = 0xff;
	    }
	    p += width;
        }
        dma_buffer_unmmap_forcpu( bb[4] );
    }

    // reuse plane, attach new data buffer to the plane descriptor
    plane->big_buffer = bb[4]; 
    plane->DataLength = bb[4]->datalen; // output_buffer_size;
    plane->image_info.image_bpp = psa->output_bpp;
    plane->image_info.image_width_in_pixels = psa->width_pix_out;
    plane->image_info.image_data_width_in_bytes = plane->image_info.image_width_in_pixels * psa->output_bpp / 8;
    plane->image_info.image_height_in_lines = psa->out_strip_height; 
    plane->image_info.image_dpi.x *= psa->scale_x;
    plane->image_info.image_dpi.y *= psa->scale_y;

    msg.msgType = MSG_JOBPLANEDATA;
    msg.param1 = COLOR_BLACK;
    msg.param2 = 0;
    msg.param3 = plane; // plane;
    DBG_PRINTF_DEBUG("%s: strip height = %d, last_data = %d\n", 
		     __func__, plane->image_info.image_height_in_lines, plane->LastData);
    SYMsgSend( psa->destination, &msg );  
}

static inline PLANE_TYPE map_color_to_plane_type(COLOR color)
{
    static PLANE_TYPE plane_type_map[] = {PLANAR_K, PLANAR_C, PLANAR_Y, PLANAR_M};
    // map declaration above relies on the order enforced by this assert
    ASSERT(COLOR_BLACK == 0 && COLOR_CYAN == 1 && COLOR_YELLOW == 2 && COLOR_MAGENTA == 3);

    return plane_type_map[color];
}


static void pip_PLANE_color_send_strip( struct pip_strip_adaptor *to_strip, BigBuffer_t *output_buffer[6] )
{
    struct pip_PLANE_strip_adaptor *psa = (struct pip_PLANE_strip_adaptor *) to_strip;
    PLANED *plane = psa->strip;
    PLANED *new_plane = 0;
    MESSAGE msg;
    int i;
    COLOR force_color = COLOR_BLACK;
    COLOR plane_color = COLOR_BLACK;
#define map_plane_color_to_output_color( e ) e
#define next_forced_color( e ) COLOR_BLACK

    for (i = 0; i < 5; i++)
    {
        if ( output_buffer[i] ) //cmy_k_
        {
            if ( i == 3 )
            {
                continue; // skip to k 
            }
            plane_color = i == 0 ? COLOR_CYAN : i == 1 ? COLOR_MAGENTA : i == 2 ? COLOR_YELLOW : COLOR_BLACK;
            force_color = next_forced_color( plane_color );
            if ( plane_color == force_color )
            {
		new_plane = 
		    print_construct_plane( mlimiter_by_name("print"),
					   map_plane_color_to_output_color(plane_color),
					   output_buffer[i],
					   output_buffer[i]->datalen,
					   map_color_to_plane_type(map_plane_color_to_output_color(plane_color)),
					   //plane->LastData, // last_strip, is this only on one plane or on all?
					   psa->is_end_of_page,
					   plane->image_info.image_dpi.x * psa->scale_x,
					   plane->image_info.image_dpi.y * psa->scale_y,
					   psa->width_pix_out,
					   psa->out_strip_height, 
					   psa->output_bpp,
					   plane->image_info.left_to_right,
					   &plane->compression_info  
			); 

		DBG_PRINTF_DEBUG(" datalen %d width out %d height out %d bpp %d laststrip %d %d\n", 
				 output_buffer[i]->datalen, 	
				 psa->width_pix_out,
				 psa->out_strip_height,  
				 psa->output_bpp,
				 psa->is_end_of_page,
				 plane->LastData
		    );

                msg.msgType = MSG_JOBPLANEDATA;
                msg.param1 = map_plane_color_to_output_color(plane_color);
                msg.param2 = 0;
                msg.param3 = new_plane; // plane;
                SYMsgSend( psa->destination, &msg );
            }
            else
            {
                BigBuffer_Free(output_buffer[i]);
                output_buffer[i] = 0;
            }
        }
    }
}

static int page_cnt( struct pip_strip_adaptor *from_strip )
{
    return 1;
}
static int plane_cnt( struct pip_strip_adaptor *from_strip )
{
    static int c = 0;
    ++c;
    return c;
}

void dbg_dump_PLANE_from_strip( struct pip_strip_adaptor *from_strip )
{
    struct pip_PLANE_strip_adaptor *psa = (struct pip_PLANE_strip_adaptor *) from_strip;

    DBG_ASSERT( psa && psa->strip ); // from_strip->attach_from_strip called before this.

    if (psa->strip->image_info.image_bpp == 24)
	dbg_dump_stripdata_to_ppm_file( page_cnt(from_strip), psa->strip, plane_cnt(from_strip) );
    else 
	dbg_dump_stripdata_to_pgm_file( page_cnt(from_strip), psa->strip, plane_cnt(from_strip) );
}

struct pip_strip_adaptor *construct_pip_PLANE_strip_adaptor( struct pip_PLANE_strip_adaptor *reuse_if_not_null )
{
    struct pip_PLANE_strip_adaptor *psa = 0;

    if (reuse_if_not_null) 
	psa = reuse_if_not_null;
    else
	psa = (struct pip_PLANE_strip_adaptor *) MEM_MALLOC( sizeof( struct pip_PLANE_strip_adaptor ) );
    if (!psa) 
	return 0;
    psa = (struct pip_PLANE_strip_adaptor*) construct_pip_strip_adaptor( (struct pip_strip_adaptor*)psa );

    psa->get_bb_in = pip_PLANE_get_bb_in;
    psa->attach_from_strip = pip_PLANE_attach_from_strip;
    psa->free_in_strip = pip_PLANE_free_in_strip;

    // use base class definition : psa->recv_strip  
    psa->pip_send_strip = pip_PLANE_mono_send_strip;
    psa->pip_send_strip = pip_PLANE_color_send_strip;


    
    psa->strip = 0; // attach_from_strip sets.

    return (struct pip_strip_adaptor *) psa;
}    
////////// pip PLANE eof
