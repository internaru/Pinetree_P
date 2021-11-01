/**
 *
 * ============================================================================
 * Copyright (c) 2011-2014 Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file generator_pip.c 
 * \brief Print Image Pipeline generator.  Transforms incoming 
 *        raw contone page data to print ready halftoned data.
 */

/** include files **/
#include <stdint.h>
#include "pthread.h"
#include "posix_ostools.h"
#include "agRouter.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "SJMgr.h"
#include "print_job_types.h"
#include "print_job_api.h"
#include "generator_inits.h"
#include "pip_api.h"
#include "mlimiter.h"
#include "print_mode_api.h"
#include "dma_buffer.h"
#include "dbg_pnm.h"


#ifndef cp_mono_only
// decouple for builds without cp_strip.h
// #include "cp_strip.h"
#  if (NUM_MARKING_COLOR_CHANNELS == 1) 
#    define cp_mono_only() (true)
#  else
#    define cp_mono_only() (false)
#  endif
#endif // end cp_mono_only

static bool allow_scaling = true; // false;
extern error_type_t pip_pqcmd_init(void); // PQ commands

#if defined(HAVE_CMD) && !defined(COLOR_ENG)
#include <string.h>
#include "cmd_proc_api.h"

extern COLOR next_forced_color( COLOR c );
#define map_plane_color_to_output_color(x) COLOR_BLACK 
#else 
/// lsp todo want to define this for a real color printer.
#define next_forced_color( x ) (x)
#define map_plane_color_to_output_color(x) (x)
#endif


/** local definitions **/
#define GENPIP_STACK_SIZE POSIX_MIN_STACK_SIZE
#define GENPIP_QUEUE_SIZE 50

#define DBG_PRFX "GENPIP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT(4)

#define PIP_STRIP_DUMP	0

/** default settings */
static uint8_t  use_override_pipmode = 0;
static uint32_t override_pipmode = IPC_UNUSED;
 

/** external data **/

/** internal functions **/
void* genpip_task( void* );

/** public data **/

/** private data **/
static pthread_t    genpip_thread_id;
static mqd_t        genpip_msgq;
static ALIGN8 uint8_t genpip_stack[GENPIP_STACK_SIZE];
static mlimiter_t* limiter = NULL;

// This is set at page_start and used during process_strip.  That means we
// cannot overlap pages.
static uint32_t current_pip_mode = IPC_UNUSED;
                
static bool first_strip; // first strip of page used for scaling.   
static PLANED* short_strip; // temporary storage for strip too short to be processed

/** public functions **/                              

/** 
 * \brief Initialize the PIP generator.
 * 
 * \param None.
 *
 * \retval uint32_t 0
 * 
 **/
uint32_t genpip_init( void )
{
    // Create a mail box and register it.
    posix_create_message_queue(&genpip_msgq, "/GenpipQ", GENPIP_QUEUE_SIZE, sizeof(MESSAGE));
      
    // Register the queue with the router.
    router_register_queue(GENPIP_ID, genpip_msgq);
      
    // Register the resource
    rm_register(e_GenPip,  GENPIP_ID);

   // Register 'pq' commands, don't worry about result.
   (void) pip_pqcmd_init();

   // start the generator task
    posix_create_thread(&genpip_thread_id, genpip_task, 0, "genpip_task", genpip_stack, GENPIP_STACK_SIZE, POSIX_THR_PRI_REALTIME);

   return(0);
}

int genpip_force_mode(int value)
{
    if (value < 0) {
       use_override_pipmode = 0;
       override_pipmode = IPC_UNUSED;
       return 1;
    } else if (pip_is_mode_supported(value)) {
       use_override_pipmode = 1;
       override_pipmode = value;
       return 1;
    }
    return 0;
}
int genpip_get_mode(void)
{
   return (use_override_pipmode == 0) ? -1 : override_pipmode;
}

/** private functions **/

static void genpip_page_start(PAGE_DATA* page)
{
    uint32_t x_scale, y_scale;
    uint32_t padded_image_width_in_pixels;
    uint32_t line_size_in_pixels;
    uint32_t width_pix_out;

    page->print_mode = print_mode_get(page);
    current_pip_mode = print_mode_get_image_pipe_configuration(page->print_mode);

    DBG_PRINTF_INFO("%s:  DocType %s, Quality %s\n", __func__,
        (page->DocType == e_DOCUMENT) ? "Document" : (page->DocType == e_COPYPAGE) ? "Copy" : "Other",
        (page->PrintQuality == PRINTQUALITY_DRAFT) ? "Draft" : (page->PrintQuality == PRINTQUALITY_BEST) ? "Best" : "Normal/Better" );
    DBG_PRINTF_INFO("%s:  Selected pip mode: %d '%s'\n", __func__,
        current_pip_mode, pip_mode_name_get(current_pip_mode));

    if (use_override_pipmode)
        current_pip_mode = override_pipmode;
    // TODO: turn this on once we get the info from pip_cfg.c
    //XASSERT( genpip_is_mode_supported(current_pip_mode), current_pip_mode );

    // default scaling is none
    x_scale = y_scale = 1;

    // get page characteristics
    if ( allow_scaling ) {
	if(page->image_output_info.image_dpi.x == 300) { 
	    x_scale = 2; 
	}
	if(page->image_output_info.image_dpi.y == 300) { 
	    y_scale = 2; 
	}
    }

    if (page->input_color_space == e_additive_gray || e_subtractive_gray)
    {
        line_size_in_pixels = page->image_output_info.image_width_in_pixels;
    }
    else
    {
        line_size_in_pixels = page->image_output_info.image_data_width_in_bytes / 3; // 4992; // plane->image_info.image_width_in_pixels;
        DBG_ASSERT(!(line_size_in_pixels & 0x1f));
    }

    // Let pip know a new page is starting.
    first_strip = true;
    pip_prepare_page(current_pip_mode,
                     x_scale,
                     y_scale,
                     line_size_in_pixels,
                     line_size_in_pixels, // fhs scaling is enabled with ( scale_x * width_pix_in )
                     page->image_output_info.image_height_in_lines,     // Input, non-scaled lines
                     1,    // 
                     true,            // pad
                     0,            // text merge
                     0,            // flip horiz
                     &width_pix_out); // only updated once per page.

    // Adjust output settings
    padded_image_width_in_pixels = pip_padded_image_width(page->image_output_info.image_width_in_pixels, x_scale);

	page->image_output_info.image_engshift = 0;
	if( (page->nup_image_params.num_images == 1) || (page->nup_image_params.ID_nup == IDNUP_SDH_ACTIVE) )// && !page->fRotate_oneup_image)
	{
		page->image_output_info.image_engshift = (int32_t)(((int32_t)page->image_output_info.image_width_in_pixels * x_scale) - (int32_t)padded_image_width_in_pixels)/2;

#if 0
		if( GetAutoDuplex(page->DUPLEX) && ( page != (void*)page->sheet->BackPage ))
		{
			DBG_PRINTF_ERR("Back page : %d, %p(%p)\n", GetAutoDuplex(page->DUPLEX), page, (void*)page->sheet->BackPage);
			page->image_output_info.image_engshift = -(page->image_output_info.image_engshift);
		}
#endif
	}

	DBG_PRINTF_ERR("image_engshift : %d\n", page->image_output_info.image_engshift);
	
    page->image_output_info.image_bpp = pip_mode_bpp_get(current_pip_mode);
    page->image_output_info.image_width_in_pixels = padded_image_width_in_pixels;
    page->image_output_info.image_data_width_in_bytes = (padded_image_width_in_pixels * page->image_output_info.image_bpp) / 8; 
    page->image_output_info.image_height_in_lines *= y_scale;
    page->image_output_info.image_dpi.x *= x_scale;
    page->image_output_info.image_dpi.y *= y_scale;

    page->nup_image_params.image_width *= x_scale;
    page->nup_image_params.image_height *= y_scale;

    page->video_justification_mode = pip_mode_justification_get(current_pip_mode);

    DBG_PRINTF_ERR("%s: page: bpp = %d width_in_pixels = %d, data_width_in_bytes = %d, height_in_lines = %d, ResX = %d, ResY = %d\n", 
                   __func__,
                   page->image_output_info.image_bpp,
                   page->image_output_info.image_width_in_pixels,
                   page->image_output_info.image_data_width_in_bytes,
                   page->image_output_info.image_height_in_lines,
                   page->image_output_info.image_dpi.x,
                   page->image_output_info.image_dpi.y);
}

static inline PLANE_TYPE map_color_to_plane_type(COLOR color)
{
    static PLANE_TYPE plane_type_map[] = {PLANAR_K, PLANAR_C, PLANAR_Y, PLANAR_M};
    // map declaration above relies on the order enforced by this assert
    ASSERT(COLOR_BLACK == 0 && COLOR_CYAN == 1 && COLOR_YELLOW == 2 && COLOR_MAGENTA == 3);

    return plane_type_map[color];
}

static void genpip_process_color_strip(PLANED* plane, uint32_t page_height, MODULE_ID destination)
{
    uint32_t line_size_in_pixels;
    uint32_t x_scale, y_scale;
    uint32_t output_bpp;
    uint32_t out_height; // input height * y_scale with shift if retained lines are needed for cross band area operations
    int have_output;
    static BigBuffer_t* output_buffer[6] = {0,0,0,0,0,0};
    MESSAGE msg;
    int i;
    int ret;
    COLOR force_color = COLOR_BLACK;
    COLOR plane_color = COLOR_BLACK;
    PLANED *new_plane = 0;

    DBG_PRINTF_DEBUG("%s: big_buffer = %#x\n", __func__, plane->big_buffer);

    if(plane->big_buffer == NULL)
    {
        // if this is an empty strip just pass it along unmodified
        // lsptodo send it to destination here...
        XASSERT(plane->DataLength == 0, plane->DataLength);
        return;
    }
    DBG_PRINTF_DEBUG("%s: big_buffer.data = %#x, len = %d\n", __func__, plane->big_buffer->data, plane->big_buffer->datalen);

    // default scaling is none
    x_scale = y_scale = 1;
    output_bpp = pip_mode_bpp_get(current_pip_mode);
    
    // BUGBUG: fix this: for now we are assuming we should always sending down 600 XbDPI data   
    // so scale up if needed 
    if ( allow_scaling ) {
	    if(plane->image_info.image_dpi.x == 300) { x_scale = 2; }
	    if(plane->image_info.image_dpi.y == 300) { y_scale = 2; }
    }

    if (y_scale > 1) 
	out_height = 128;
    else
	out_height = 64; /// lsptodo: scaling for copy might need adjust to protect video's 128 max.
    line_size_in_pixels = plane->image_info.image_data_width_in_bytes / 3; // 4992; 
                          // plane->image_info.image_width_in_pixels;

    have_output = pip_process_strip(current_pip_mode,
				    plane->big_buffer,
				    line_size_in_pixels, // input
				    plane->image_info.image_height_in_lines,     // input
				    out_height,
				    output_bpp,
				    page_height,
				    x_scale,
				    y_scale,
				    first_strip,
				    plane->LastData,
				    output_buffer);

    first_strip = false;
    if (have_output <= 0) {
      	DBG_PRINTF_DEBUG("no output bb  %p\n", plane->big_buffer);
	memFree(plane);  
	return ; // have_output; // negative error, zero ok no output yet.
    } else if (have_output != out_height) {
	DBG_PRINTF_DEBUG("changed output height %d  %d\n", out_height, have_output);
	out_height = have_output;
    }

    if ( cp_mono_only() ) { // only generate color on color engines.
	    //BigBuffer_Free(output_buffer[0]);
	    //BigBuffer_Free(output_buffer[1]);
	    //BigBuffer_Free(output_buffer[2]);
    }
   
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
		    print_construct_plane( limiter,
					   map_plane_color_to_output_color(plane_color),
					   output_buffer[i],
					   output_buffer[i]->datalen,
					   map_color_to_plane_type(map_plane_color_to_output_color(plane_color)),
					   plane->LastData, // last_strip, is this only on one plane or on all?
					   plane->image_info.image_dpi.x * x_scale,
					   plane->image_info.image_dpi.y * y_scale,
					   line_size_in_pixels * x_scale,
					   out_height, //plane->RASTER_Y * y_scale,
					   output_bpp,
					   plane->image_info.left_to_right,
					   &plane->compression_info  
			); 

                msg.msgType = MSG_JOBPLANEDATA;
                msg.param1 = map_plane_color_to_output_color(plane_color);
                msg.param2 = 0;
                msg.param3 = new_plane; // plane;
                ret = SYMsgSend( destination, &msg );
                ASSERT( OK==ret );
            }
            else
            {
                BigBuffer_Free(output_buffer[i]);
                output_buffer[i] = 0;
            }
        }
    }
    memFree(plane);
}

static void genpip_process_mono_strip(PLANED* plane, uint32_t page_height, MODULE_ID destination)
{
    uint32_t line_size_in_pixels;
    uint32_t out_line_size_in_pixels;
    uint32_t x_scale, y_scale;
    uint32_t output_bpp;
    uint32_t out_height; // input height * y_scale
    MESSAGE msg;
    error_type_t ret;
    int have_output;
    static BigBuffer_t *bb[6] = {0, 0, 0, 0, 0, 0};  // retained between calls to combine strips.
    

    DBG_PRINTF_DEBUG("%s: big_buffer = %#x\n", __func__, plane->big_buffer);

    // if this is an empty strip just pass it along unmodified
    if(plane->big_buffer == NULL)
    {
        XASSERT(plane->DataLength == 0, plane->DataLength);
        return;
    }

    DBG_PRINTF_DEBUG("%s: big_buffer.data = %#x, len = %d\n", __func__, plane->big_buffer->data, plane->big_buffer->datalen);

    ASSERT(plane->image_info.image_height_in_lines > 0);

    // default scaling is none
    x_scale = y_scale = 1;

    output_bpp = pip_mode_bpp_get(current_pip_mode);
    // BUGBUG: fix this: for now we are assuming we should always sending down 600 DPI data   
    // so scale up if needed 
    if ( allow_scaling ) {
	if(plane->image_info.image_dpi.x == 300) { x_scale = 2; } // mod = 63; }
	if(plane->image_info.image_dpi.y == 300) { y_scale = 2; }
    }

    // out_height = pip_calculate_output_strip_height(plane->image_info.image_height_in_lines, first_strip, plane->LastData);
    if (y_scale > 1) 
	out_height = 128;
    else
	out_height = 64; /// lsptodo: scaling for copy might need adjust to protect video's 128 max.

    line_size_in_pixels = plane->image_info.image_width_in_pixels;
    
    out_line_size_in_pixels = ((line_size_in_pixels * x_scale) + 127) & ~127; //todo: one place to set this.

    have_output = pip_process_strip(current_pip_mode,  // color mono ?
				    plane->big_buffer, 
				    line_size_in_pixels, //< input 
				    // out_line_size_in_pixels, 
				    plane->image_info.image_height_in_lines,
				    out_height,
				    output_bpp,
				    page_height,
				    x_scale,
				    y_scale,
				    first_strip,
				    plane->LastData,
				    bb  //< allocated on demand below.
	    );
    first_strip = false;
    if (have_output <= 0) {
        // bb has been removed/combined, free plane  
	DBG_PRINTF_DEBUG("no output bb  %p\n", plane->big_buffer);
	memFree(plane);  
	return ; // have_output; // negative error, zero ok no output yet.
    } else if (have_output != out_height) {
	// reuse plane data modifying its contents.
	DBG_PRINTF_DEBUG("changed output height %d  %d\n", out_height, have_output);
	out_height = have_output;
	// todo: EOD flag
    }

    if (0) { // DEBUG TOOL: hack in a line of black at the top of each strip.
        dma_buffer_map_single(bb[4], DMA_FROM_DEVICE);
        dma_buffer_unmap_single(bb[4], DMA_FROM_DEVICE);
        void *after = dma_buffer_mmap_forcpu(bb[4]);
        char *p = after;
        int i;
	int width = out_line_size_in_pixels * output_bpp / 8;

        for (i=0; i < out_height; i++) {

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
    plane->image_info.image_bpp = output_bpp;
    plane->image_info.image_width_in_pixels = out_line_size_in_pixels;
    plane->image_info.image_data_width_in_bytes = plane->image_info.image_width_in_pixels * output_bpp / 8;
    plane->image_info.image_height_in_lines = out_height; 
    plane->image_info.image_dpi.x *= x_scale;
    plane->image_info.image_dpi.y *= y_scale;

    msg.msgType = MSG_JOBPLANEDATA;
    msg.param1 = COLOR_BLACK;
    msg.param2 = 0;
    msg.param3 = plane; // plane;
    DBG_PRINTF_DEBUG("%s: strip height = %d, last_data = %d\n", 
		     __func__, plane->image_info.image_height_in_lines, plane->LastData);
    ret = SYMsgSend( destination, &msg );  
    ASSERT( OK==ret );  
}

/**
 *  \brief PIP generator task.
 *  The task sits between the print_job_api interface and the print job manager. 
 *  It processes the print job manager messages and data and then passes the
 *  processed messages and data on to the print job manager.
 * 
 * \param temp Dummy.
 * 
 * \retval None
 *
 * 
 **/
void* genpip_task(void* temp)
{
    MESSAGE msg, new_msg;
    CURRENT_RESOURCE  *our_resource = NULL;
    error_type_t ret;
    int mq_ret;
    PAGE_DATA *page = NULL;
    uint32_t page_height = 0;
    PLANED *plane = NULL;
    static int pagecnt = 0; //debug output counter
    static int planecnt; //debug output counter
#if PIP_STRIP_DUMP
	static bool first_strip_dump;
#endif
	
    SysWaitForInit();

    DBG_PRINTF_NOTICE("%s\n", __func__);

    // main loop
    while (1)
    {
        mq_ret = mq_receive(genpip_msgq, (char*)&msg, sizeof(MESSAGE), 0);
        XASSERT(mq_ret == sizeof(MESSAGE), mq_ret);
        switch (msg.msgType)
        {
            case MSG_RESOURCES:
            {
                DBG_PRINTF_NOTICE("MSG_RESOURCES resource = %#x\n", msg.param3);
                page = NULL;
                plane = NULL;

                our_resource = (CURRENT_RESOURCE *) msg.param3;
                DBG_PRINTF_NOTICE("%s: our_resource.destination = %d\n", __func__, our_resource->Destination);
                break;
            }
            case MSG_ACKRECIPE:
            {
                // Job is starting, we do nothing here, just wait for messages.
                break;
            }
            case MSG_JOBSTART:
            {
                DBG_PRINTF_NOTICE("MSG_JOBSTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );

                // set the limiter
                // BUGBUG: this should really be passed along with the job since we may not be using the print limiter
                limiter = mlimiter_by_name("print");
                mlimiter_start(limiter);
                ASSERT( OK==ret );  
#if PIP_STRIP_DUMP
                first_strip_dump = true;
#endif
                break;
            }
            case MSG_JOBSHEETSTART:
            {
                DBG_PRINTF_NOTICE("MSG_JOBSHEETSTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }
            case MSG_JOBPAGESTART:
            {
                ASSERT( NULL != our_resource );
                planecnt = 0;
                pagecnt++;
                page = (PAGE_DATA *)msg.param3;
                DBG_PRINTF_NOTICE("MSG_JOBPAGESTART page = %#x\n", (uint32_t)page);
                ASSERT(page != NULL);
                ASSERT(IS_PAGE_DATA(page));
                ASSERT(short_strip == NULL);

                page_height = page->image_output_info.image_height_in_lines;
                genpip_page_start(page);

                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );
                break;
            }
            case MSG_JOBPLANESTART:
            {
                DBG_PRINTF_NOTICE("MSG_JOBPLANESTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }
            case MSG_JOBPLANEDATA:
            {
                ASSERT(NULL != our_resource);

                plane = (PLANED *)msg.param3;
                ASSERT(IS_PLANED(plane));

                DBG_PRINTF_NOTICE("MSG_JOBPLANEDATA plane = %#x\n", (uint32_t)plane);

#if PIP_STRIP_DUMP
				if (first_strip_dump)
				{
					static FILE *image_ryu=NULL;
					char path[200];
					
					sprintf(path, "/mnt/sda1/pip_first_strip_%d_%d.raw",  plane->image_info.image_data_width_in_bytes, plane->image_info.image_height_in_lines);
					image_ryu = fopen(path,"wb");
					dma_buffer_map_single(plane->big_buffer, DMA_TO_DEVICE);
					dma_buffer_unmap_single(plane->big_buffer, DMA_TO_DEVICE);
					void *data = dma_buffer_mmap_forcpu(plane->big_buffer);
					
					fwrite(  data , 1, (plane->image_info.image_data_width_in_bytes * plane->image_info.image_height_in_lines), image_ryu );
					fflush(image_ryu);
					fclose(image_ryu);
					dma_buffer_unmmap_forcpu( plane->big_buffer );
					first_strip_dump = false;
				}
#endif
                // process data as needed and pass to destination
                if (plane->BlockDataType == PACKED_RGB) {
                    dbg_dump_stripdata_to_ppm_file( pagecnt, plane, planecnt );
                    genpip_process_color_strip(plane, page_height, our_resource->Destination );
                } else {
                    dbg_dump_stripdata_to_pgm_file( pagecnt, plane, planecnt );
                    genpip_process_mono_strip(plane, page_height, our_resource->Destination); 
                }
                planecnt++;
                break;
            }
            case MSG_JOBPLANEEND:
            {
                DBG_PRINTF_NOTICE("MSG_JOBPLANEEND\n");
                ASSERT( NULL != our_resource );
                SYMsgSend( our_resource->Destination, &msg );
                break;
            }
            case MSG_JOBPAGEEND:
            {
                DBG_PRINTF_ERR("MSG_JOBPAGEEND\n");
                ASSERT( NULL != our_resource );
                page = NULL;
                pip_close_page();
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }

            case MSG_JOBABORT:
            case MSG_JOBEND:
            {
                DBG_PRINTF_NOTICE("MSG_JOBEND\n");
                                            
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );                         
                
                // free resources
                new_msg.msgType = MSG_FREERECIPE;
                new_msg.param1 = 0;
                new_msg.param2 = 0;
                new_msg.param3 = our_resource;
                mlimiter_stop(limiter);
                our_resource = NULL;
                page = NULL;
                plane = NULL;

                if (short_strip != NULL)
                {
                    DBG_PRINTF_ERR("MSG_JOBEND: ERROR: short_strip should be NULL; discarding\n");
                    BigBuffer_Free(short_strip->big_buffer);
                    MEM_FREE_AND_NULL(short_strip);
                }

                ret = SYMsgSend( SJMID, &new_msg );
                ASSERT( OK==ret );
                break;
            }
            case MSG_CANCELJOB: 
            {
                DBG_PRINTF_NOTICE("MSG_CANCELJOB\n");
                mlimiter_stop(limiter);
                // ASSERT( NULL != our_resource );
                // ACK the system job mgr
                new_msg.msgType = MSG_CANCELJOB;
                new_msg.param1 = SYS_ACK;
                new_msg.param2 = GENPIP_ID;
                new_msg.param3 = msg.param3;
                ret = SYMsgSend( SJMID, &new_msg);
                ASSERT( OK == ret );

                // free resources
                new_msg.msgType = MSG_FREERECIPE;
                new_msg.param1 = 0;
                new_msg.param2 = 0;
                new_msg.param3 = our_resource;

                if (page != NULL)
                {
                    pip_close_page();
                }
                our_resource = NULL;
                page = NULL;
                plane = NULL;

                if (short_strip != NULL)
                {
                    BigBuffer_Free(short_strip->big_buffer);
                    MEM_FREE_AND_NULL(short_strip);
                }

                ret = SYMsgSend( SJMID, &new_msg );
                ASSERT( OK==ret );
                break;
            }
            default:
            {
                DBG_PRINTF_ERR("Unknown message %d\n", msg.msgType);
                ASSERT(0);  // we need to identify all of these messages
                break;
            }
        }
    }
    return 0;
}
