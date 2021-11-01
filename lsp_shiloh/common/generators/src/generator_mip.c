/**
 *
 * ============================================================================
 * Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file generator_mip.c 
 * \brief 
 */

/** include files **/
#include <stdint.h>
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
#include "generator_inits.h"
#include "mono_pipe_cfg.h"
#include "color_pipe_cfg.h"
#include "string.h"
#include "print_mode.h"
#include "print_mode_api.h"
#include "asic.h"

#include "pipr_api.h"
#include "pipr_idma_api.h"
#include "raster_odma_api.h"

#include "mip_api.h"  // REVISIT DAB 

// REVISIT DAB  these are included for use in setting configuration. There should
// probably be a separate configuration interface.
#include "pipr_api.h"


/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define GEN_STACK_SIZE 4096
#define GEN_QUEUE_SIZE 10

#define DBG_PRFX "GEN_MIP: "
#define LOGGER_SUBMOD (1<<12)   //TODO: change submod id; should be unique for each generator
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMOD

#define PIP_DBG_LVL (DBG_LOUD | DBG_OUTPUT)

#define DBG_SLOG_ENTRY DBG_MEMLOG(LOG_DEBUG_H,"%s ENTRY\n", __func__);
#define DBG_SLOG_EXIT DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */

typedef struct pip_context_s
{
    uint8_t input_bpp;  // Input bit depth of original page data
    uint8_t output_bpp; // Bit depth of image after halftoning
    uint8_t video_bpp;  // Bit depth of the video ready image data
    uint8_t color;
    uint32_t idma;      // Specify which IDMA channels are active
    COLOR plane_color;
    PAGE_DATA *page;
    bool new_page;
    bool start_of_image;
    uint32_t num_rows_sent;
    uint32_t idma_channel;
    mlimiter_t *limiter;
} pip_context_t;


/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */

static pip_context_t pip_context;
static TX_QUEUE gen_queue;
static TX_THREAD gen_thread;
static ALIGN8 uint8_t gen_stack[GEN_STACK_SIZE];
static MESSAGE gen_msg_buf[GEN_QUEUE_SIZE];
static CURRENT_RESOURCE  *our_resource = NULL;


static idma_pipr_handle_t *idma_handle[NUM_PIPR_IDMA_CHANNELS]; 
static odma_handle_t *odma_handle[NUM_RASTER_ODMA_CHANNELS];
static pipr_handle_t *pipr_handle;

//static TX_MUTEX gen_mutex;

// The following are used to keep track of allocations that need to be freed
// at close page time. The allocations are maintained in a simple list.
static dma_config_data_t *last_dma_config = 0;
static dma_config_data_t *first_dma_config = 0;


/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */
static void gen_task( uint32_t );
static void pip_setup_config( print_mode_t *mode );

static void 
pip_proc_strip( PLANED *plane,
                uint32_t datalen,
                uint32_t bits_per_row, 
                uint32_t num_rows, 
                uint32_t LinesPerPage, 
                uint32_t xres, 
                uint32_t yres,
                uint32_t output_bpp ) ;

static void 
gen_print_plane_add_data(  PLANED* plane, 
                           COLOR plane_color, 
                           BigBuffer_t* data, 
                           uint32_t data_len_in_bytes, 
                           PLANE_TYPE strip_type,
                           bool last_strip, 
                           uint32_t resolution_x, 
                           uint32_t resolution_y, 
                           uint32_t strip_width_in_pixels, 
                           uint32_t strip_height,
                           uint32_t bits_per_pixel,
                           bool left_to_right,
                           compression_info_t* comp_info );

static error_type_t gen_open_image_pipe( void );
static error_type_t gen_close_image_pipe( void );

static void 
gen_process_mono_strip( uint8_t *input_buffer,
                        uint32_t input_line_width,
                        uint32_t input_num_lines,
                        uint8_t *output_buffer,
                        uint32_t output_line_width,
                        uint32_t output_num_lines,
                        bool soi,
                        bool eoi,
                        bool left_to_right,
                        uint32_t idma );



static dma_config_data_t *gen_get_dma_config_data( void );
static void gen_free_dma_config_data_objects( void );


                              
/** public functions **/                              
 
/** 
 * \brief Initialize the compression subsystem.
 * 
 * \param None.
 *
 * \retval uint32_t 0
 * 
 **/
uint32_t gen_mip_init( void )
{
    uint32_t status;

   // Create a mail box and register it.
   status = tx_queue_create(&gen_queue,
                            0, 
                            SYS_MSG_SIZE, 
                            gen_msg_buf, 
                            GEN_QUEUE_SIZE * sizeof(MESSAGE) ); /* create the queue */
   ASSERT(status == TX_SUCCESS);

   // Register the queue with the router.
   SYRegister(GEN_MIP_ID, &gen_queue);

   // Register the resource
   rm_register(e_GenMIP,  GEN_MIP_ID);

   // start the generator task
   tx_thread_create(&gen_thread,
                    "tgen_mip",
                    gen_task,
                    0,
                    gen_stack,
                    GEN_STACK_SIZE,
                    THR_PRI_NORMAL,
                    THR_PRI_NORMAL,
                    DEFAULT_TIME_SLICE,
                    TX_AUTO_START);

   return(0);
}  


/** private functions **/

/**
 *  \brief Main generator task.
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
static void gen_task( uint32_t value )
{
    MESSAGE msg, new_msg;
    
    error_type_t ret;
    UINT txret;
    PAGE_DATA *page = NULL;
    PLANED *plane = NULL;
   
    pip_context.limiter = mlimiter_by_name("print");
    SysWaitForInit();

    // main loop
    while (1)
    {
        txret = tx_queue_receive (&gen_queue, &msg, TX_WAIT_FOREVER);     // get a message
        XASSERT( txret==TX_SUCCESS, txret );

        switch (msg.msgType)
        {
            case MSG_RESOURCES:
            {
                dbg_printf("MSG_RESOURCES resource = %#x\n", msg.param3);
                page = NULL;
                plane = NULL;

                our_resource = (CURRENT_RESOURCE *) msg.param3;
                dbg_printf("%s: our_resource.destination = %d\n", __func__, our_resource->Destination);
                DPRINTF(PIP_DBG_LVL, ("JOB type: %s\n", to_string_job_types( our_resource->JobResources->JobType)));

                break;
            }
            case MSG_ACKRECIPE:
            {
                // Job is starting, we do nothing here, just wait for messages.
                break;
            }
            case MSG_JOBSTART:
            {
                dbg_printf("MSG_JOBSTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  tx_thread_sleep(1000);
                break;
            }
            case MSG_JOBSHEETSTART:
            {
                dbg_printf("MSG_JOBSHEETSTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }
            case MSG_JOBPAGESTART:
            {
                ASSERT( NULL != our_resource );

                page = (PAGE_DATA *)msg.param3;
                dbg_printf("MIP GEN %s MSG_JOBPAGESTART page = %#x\n", __func__, (uint32_t)page);
                ASSERT(page != NULL);
                ASSERT(IS_PAGE_DATA(page));

                // Initialize the print image pipe context for the new page
                pip_context.page = page;
                pip_context.new_page = true;
                pip_context.num_rows_sent = 0;

                DPRINTF(PIP_DBG_LVL, ("Document type: %s\n", to_string_document_type( page->DocType )));
                
                // Get the print mode configuration for this print job.
         //       mode = print_mode_get( page );

                pip_setup_config(page->print_mode);

//                pip_context.output_bpp = mode->output_bpp;
                
                pip_context.new_page = true;
                pip_context.start_of_image = true;

                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );
                break;
            }

            case MSG_JOBPLANESTART:
            {
                dbg_printf("MIP GENX %s MSG_JOBPLANESTART\n", __func__);
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                dbg_printf("**********************************************\n");
                break;
            }

            case MSG_JOBPLANEDATA:
            {
                ASSERT(NULL != our_resource);
                dbg_printf("MIP GEN %s MIP MSG_JOBPLANEDATA\n", __func__);

                plane = (PLANED *)(msg.param3);
                ASSERT(IS_PLANED(plane));

                // dbg_printf("%s MSG_JOBPLANEDATA color %d, %d, 0x%x, plane = %#x\n", __func__, msg.param1,msg.param2, msg.param3, (uint32_t)plane);

                pip_context.new_page = false;

                gen_open_image_pipe();

                // Process the strip of data. Note that an RGB input plane 
                // results in four output planes - CMYK. All message passing
                // for the plane data is handled in pip_proc_strip()
                pip_proc_strip( plane,
                                plane->DataLength, 
                                plane->image_info.image_data_width_in_bytes*8,  
                                plane->image_info.image_height_in_lines, 
                                pip_context.page->RASTER_Y, 
                                pip_context.page->RESOLUTION_X, 
                                pip_context.page->RESOLUTION_Y, 
                                pip_context.output_bpp );

                gen_close_image_pipe();
                break;
            }

            case MSG_JOBPLANEEND:
            {
                // due to the fact that host based color jobs process "1 color" at a time we need to reset the initial start of image
                // in order for the soi flag to get set correctly. This should be replaced with a better solution at some time.
                pip_context.start_of_image = true;
                pip_context.num_rows_sent = 0;
                dbg_printf("%s MIP MSG_JOBPLANEEND %d %d 0x%x\n", __func__, msg.param1, msg.param2, msg.param3);
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }

            case MSG_JOBPAGEEND:
            {
                dbg_printf("%s MIP MSG_JOBPAGEEND\n", __func__);
                ASSERT( NULL != our_resource );

                pip_context.page = 0;
                page = NULL;

                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }

            case MSG_JOBEND:
            {
                 dbg_printf("%s MSG_JOBEND\n", __func__);
                                            
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );                         
                
                // free resources
                new_msg.msgType = MSG_FREERECIPE;
                new_msg.param1 = 0;
                new_msg.param2 = 0;
                new_msg.param3 = our_resource;

                our_resource = NULL;
                page = NULL;
                plane = NULL;
               
                ret = SYMsgSend( SJMID, &new_msg );
                ASSERT( OK==ret );
                break;
            }

        case MSG_CANCELJOB: 
            {
                dbg_printf("MSG_CANCELJOB\n");

                ASSERT( NULL != our_resource );
                // ACK the system job mgr
                new_msg.msgType = MSG_CANCELJOB;
                new_msg.param1 = SYS_ACK;
                new_msg.param2 = GEN_PIPP_PIPR_ID;
                new_msg.param3 = msg.param3;
                ret = SYMsgSend( SJMID, &new_msg);
                ASSERT( OK == ret );

                pip_context.page = 0;

                // free resources
                new_msg.msgType = MSG_FREERECIPE;
                new_msg.param1 = 0;
                new_msg.param2 = 0;
                new_msg.param3 = our_resource;

                our_resource = NULL;
                page = NULL;
                plane = NULL;

                ret = SYMsgSend( SJMID, &new_msg );
                ASSERT( OK==ret );
                break;
            }

            case MSG_JOBABORT:
            {
                DBG_PRINTF_ERR("Unexpected message JOB_ABORT\n");
                ASSERT(0); // we should never see this message!
                break;
            }
            default:
            {
                DBG_PRINTF_ERR("Unknown message %d\n", msg.msgType);
                ASSERT(0);  // we need to identify all of these messages
                break;
            }
        } // switch

    } // while
}


// This is needed later to set the configuration for each color
static mono_pipe_cfg_mode_t mono_pipe_cfg = 0;

static void pip_setup_config( print_mode_t *mode )
{
    
    switch (mode->image_pipe_configuration)
    {
    case  IPC_COPY_TEXT_MONO_300DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Copy TEXT MONO 300 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_2BPP;
        break;

    case IPC_COPY_TEXT_COLOR_300DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Copy TEXT COLOR 300 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_2BPP;
        break;

    case IPC_COPY_MIXED_MONO_300DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Copy MIXED MONO 300 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_COPY;
        break;

    case IPC_COPY_MIXED_COLOR_300DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Copy MIXED COLOR 300 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_COLOR_COPY;
        break;

    case IPC_COPY_PHOTO_MONO_300DPI_4BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Copy PHOTO MONO 300 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_2BPP;
        break;

    case IPC_COPY_PHOTO_COLOR_300DPI_4BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Copy PHOTO COLOR 300 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_2BPP;
        break;

    case IPC_PRINT_DRAFT_MONO_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print DRAFT MONO 600 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_2BPP;
        break;

    case IPC_PRINT_NORMAL_MONO_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print NORMAL MONO 600 dpi 2 bpp\n", __func__));
        // mono_pipe_cfg = PIPR_PRINT_2BPP;
        mono_pipe_cfg = PIPR_ALL_BYPASS;
        break;

    case IPC_PRINT_PHOTO_MONO_600DPI_4BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print PHOTO MONO 600 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_4BPP;
        break;

    case IPC_PRINT_DRAFT_COLOR_600DPI_2BPP:
    case IPC_PRINT_NORMAL_COLOR_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print DRAFT / NORMAL COLOR 600 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_COLOR_PRINT_2BPP;
        break;

    case IPC_PRINT_PHOTO_COLOR_600DPI_4BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print PHOTO COLOR 600 dpi 4 bpp\n", __func__));
        ASSERT(0);
        break;

    case IPC_PRINT_NORMAL_MONO_FROM_RGB_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print NORMAL MONO from COLOR INPUT 600 dpi 2 bpp\n", __func__));
        ASSERT(0);
        break;

    case IPC_ZJS_PRINT_DRAFT_MONO_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print ZJS DRAFT MONO 600 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_READY_DATA_2BPP;
        break;

    case IPC_ZJS_PRINT_NORMAL_MONO_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print ZJS NORMAL MONO 600 dpi 2 bpp\n", __func__));
        ASSERT(0);
        break;

    case IPC_ZJS_PRINT_DRAFT_COLOR_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print ZJS DRAFT COLOR 600 dpi 2 bpp\n", __func__));
        mono_pipe_cfg = PIPR_PRINT_READY_DATA_2BPP;
        break;

    case IPC_ZJS_PRINT_NORMAL_COLOR_600DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print ZJS NORMAL COLOR 600 dpi 2 bpp\n", __func__));
        ASSERT(0);
        break;

    case IPC_ZJS_PRINT_DRAFT_MONO_400DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print ZJS DRAFT MONO 400 dpi 2 bpp\n", __func__));
        ASSERT(0);
        break;

    case IPC_ZJS_PRINT_NORMAL_MONO_400DPI_2BPP:
        DPRINTF(PIP_DBG_LVL, ("%s Print ZJS NORMAL MONO 400 dpi 2 bpp\n", __func__));
        ASSERT(0);
        break;

    default:
        DPRINTF(PIP_DBG_LVL, ("%s NOT SUPPORTED BY THIS GENERATOR!!!\n", __func__));
        ASSERT(0);
        break;

    }

    pip_context.idma = mono_pipe_cfg_array[mono_pipe_cfg]->pipr_cfg->active_idma;

    pip_context.output_bpp = mode->output_bpp;
    dbg_printf("%s setting mono pipe cfg %d  idma %d\n",__func__, mono_pipe_cfg, pip_context.idma );

    mono_pipe_set_config( mono_pipe_cfg_array[mono_pipe_cfg] );
}


static void pip_proc_strip( PLANED *plane,
                    uint32_t datalen,
                    uint32_t bits_per_row, 
                    uint32_t num_data_rows, 
                    uint32_t LinesPerPage, 
                    uint32_t xres, 
                    uint32_t yres,
                    uint32_t output_bpp )
{
    uint8_t *data_out = 0; 
    uint8_t *output_buffer;

    uint32_t data_out_size;  // used to determine the allocation of the image pipe in bytes
    uint32_t data_out_size_pad = cpu_get_dcache_line_size();
    uint32_t data_out_size_padded;
    uint32_t num_pixels_in_row;
    uint32_t num_output_rows = 0;
    uint32_t row_adjust;
    uint32_t mip_adjusted_num_lines;
    uint32_t input_line_width = 0;
    uint32_t output_line_width_in_bytes = 0;

    compression_info_t compression_info;
    COLOR color = COLOR_BLACK;
    bool dealloc_data_out_bufs = false;
    bool last_buffer;
    PLANE_TYPE plane_type = plane->BlockDataType;
    MESSAGE     msg;
    
    DBG_SLOG_ENTRY

    last_buffer = plane->LastData;
    msg.msgType = MSG_JOBPLANEDATA;

    // Setup the memory limiter
    pip_context.limiter = mlimiter_by_name("print");
    mlimiter_start(pip_context.limiter);

    dbg_printf("%s ENTRY plane 0x%x. bits_per_row %d, num_rows %d, lines per page %d, x res %d, y res %d, plane type %d\n",
               __func__, plane, bits_per_row, num_output_rows, LinesPerPage, xres, yres, plane_type);
   
    pip_context.num_rows_sent += num_output_rows;

    // Modify the page height to match the number of rows sent when the end of
    // data marker is detected. This is specifically to solve the problem of
    // unknown page length when copying from an ADF because the page size isn't
    // known until the entire page has passed through the feeder. 
    if (plane->LastData != 0)
    {
        if (pip_context.page->VIDEO_Y != pip_context.num_rows_sent)
        {
            logStringImpl("%s page length discrepancy detected - MODIFY PAGE DATA LENGTH\n", __func__);
            pip_context.page->VIDEO_Y = pip_context.num_rows_sent;
        }
    }

    // Now setup the processing based on the plane type. RGB data is converted
    // to either CMYK for color printing, or grayscale for monochrome printing.
    // Grayscale input data is obviously processed for monochrome printing.
    switch (plane_type)
    {
    case PACKED_RGB: 
    case PACKED_XRGB:
    case PACKED_RGBX:
    case GRAYSCALE: 
        ASSERT(0);
        break;

        // Planar data only needs to be processed by the raster pipe, so the
        // color pipe is bypassed.
    case PLANAR_C:
        color = COLOR_CYAN;
        break;

    case PLANAR_M:
        color = COLOR_MAGENTA;
        break;

    case PLANAR_Y:
        color = COLOR_YELLOW;
        break;

    case PLANAR_K:
        color = COLOR_BLACK;
        break;
        
        // All other plane types can not be processed by this generator.
    default:
        ASSERT(0); 
        break;
    }

    // Calculate the data output size, in this hardware pipe the input data will
    // always be 8 bpp, so one byte per pixel.
    num_pixels_in_row = bits_per_row/compression_info.comp_details.rgb.bits_per_pixel;
    data_out_size = num_output_rows * num_pixels_in_row;  

    // dbg_printf("%s num_output_rows %d, num_pixels_in_row %d, data out size %d\n", __func__, num_output_rows, num_pixels_in_row, data_out_size );

    // Process the packed RGB input data to yield either CMYK or monochrome
    // grayscale output.
    input_line_width = plane->image_info.image_data_width_in_bytes;
    output_line_width_in_bytes = input_line_width;
    if(plane->image_resolution.y == 400)
    {
        output_line_width_in_bytes *= 2;
    }
    dbg_printf("OUTPUT bpp = %d line width in bytes = %d\n",output_bpp,output_line_width_in_bytes);
    data_out = plane->big_buffer->data; 

    // This input and output line widths need to be set correctly for ALL types
    // of print jobs.
    dbg_printf("plane width bytes %d  pixels %d height %d\n", plane->image_info.image_data_width_in_bytes,
               plane->image_info.image_width_in_pixels, plane->image_info.image_height_in_lines);

    row_adjust = mip_get_windowing_line_adjustment(last_buffer);
    mip_adjusted_num_lines = pip_context.start_of_image ? num_output_rows - row_adjust :  num_output_rows;
    mip_adjusted_num_lines = last_buffer ?  num_output_rows + row_adjust : mip_adjusted_num_lines;

    dbg_printf("processing raster strip color %d, input width %d, output width %d\n", color, input_line_width, output_line_width_in_bytes);

    data_out_size = output_line_width_in_bytes * mip_adjusted_num_lines;

    // Pad the data output buffer to ensure dcache alignment.  
    data_out_size_padded = data_out_size + data_out_size_pad; 

    output_buffer = MEM_MALLOC_ALIGN_LIMITED( pip_context.limiter, 
                                              data_out_size_padded, 
                                              e_32_byte ); // REVISIT DAB  move outside for loop
    ASSERT(output_buffer);

    dbg_printf("%s plane_type %d color %d\n", __func__, plane_type, color);


    // REVISIT DAB JUST RESTORE MIP HERE
    // Restore the state of the raster pipe for use by the color to be 
    // processed. Set the halftone configuration for this color.
    mip_restore_state(color);

    gen_process_mono_strip( data_out,
                            input_line_width,
                            num_output_rows,
                            output_buffer,
                            output_line_width_in_bytes,
                            mip_adjusted_num_lines,
                            pip_context.start_of_image,
                            last_buffer,    // EOI
                            false,          // left_to_right
                            pip_context.idma ); 

    // Save the state of the raster pipe so it can be restored later for the
    // next strip of this color in the image.
    mip_save_state( color );

    pip_context.start_of_image = false;

    // Free the input buffer and only keep the newly allocated output 
    // buffer. It's the raw data that needs to get appended to the page.
    if ( dealloc_data_out_bufs )
    {
        logString("%s freeing: 0x%x\n", __func__, data_out);
        MEM_FREE_AND_NULL(data_out);
    }

    // dbg_printf("%s data out size %d, padded %d,  adjusted %d\n", __func__,  data_out_size, data_out_size_padded, (data_out_size_padded / data_out_size_pad) * data_out_size_pad);

    // Flush the data cache so the output buffer contents are really in 
    // RAM for the next DMA operation.
    cpu_dcache_writeback_region(output_buffer, (data_out_size_padded / data_out_size_pad) * data_out_size_pad);

    // Add the data to the page, re-use the current plane attached to the 
    // page. Simply repopulate it with the new transformed values.

    // Free the old data and attach the new.
    BigBuffer_Free(plane->big_buffer);

    dbg_printf("%s mip adjusted lines %d, output line width %d\n", __func__, mip_adjusted_num_lines,
               output_line_width_in_bytes);
    // Now update the plane to reflect the transformation.
    gen_print_plane_add_data( plane, 
                              color, 
                              BigBuffer_adopt( output_buffer, data_out_size_padded ),
                              mip_adjusted_num_lines * output_line_width_in_bytes, 
                              plane_type, 
                              last_buffer, 
                              xres, 
                              yres,
                              num_pixels_in_row * pip_context.output_bpp,  
                              mip_adjusted_num_lines, 
                              output_bpp, 
                              true, // left to right image data
                              &compression_info);

    // REVISIT DAB GET THE COLOR CORRECT FOR HOST BASED???
    msg.param1 = color;
    msg.param2 = 0;
    msg.param3 = plane;

    SYMsgSend( our_resource->Destination, &msg );

    dbg_printf("%s updating color %d plane_type %d, last data %d, output bpp %d\n", __func__, color, plane_type, plane->LastData, output_bpp );

    mlimiter_stop( pip_context.limiter );
}


static void 
gen_print_plane_add_data(  PLANED *plane, 
                           COLOR plane_color, 
                           BigBuffer_t* data, 
                           uint32_t data_len_in_bytes, 
                           PLANE_TYPE strip_type,
                           bool last_strip, 
                           uint32_t resolution_x, 
                           uint32_t resolution_y, 
                           uint32_t bits_per_row, //strip_width_in_pixels, 
                           uint32_t strip_height,
                           uint32_t bits_per_pixel,
                           bool left_to_right,
                           compression_info_t* comp_info )
{
    plane->sysPlaneColor = plane_color;
    plane->big_buffer    = data;
    plane->DataLength    = data_len_in_bytes;
    plane->LastData      = last_strip;
    plane->image_resolution.x = resolution_x;
    plane->image_resolution.y = resolution_y;
    plane->RASTER_X      = bits_per_row; // strip_width_in_pixels;
    plane->RASTER_Y      = strip_height;
    plane->VIDEO_BPP     = bits_per_pixel;

    plane->image_info.left_to_right = left_to_right;
    plane->image_info.image_bpp = bits_per_pixel;
    plane->image_info.image_data_width_in_bytes = bits_per_row/8; // (strip_width_in_pixels * bits_per_pixel)/8;
    plane->image_info.image_height_in_lines     = strip_height;
    plane->image_info.image_width_in_pixels     = bits_per_row/bits_per_pixel; //strip_width_in_pixels;

    // dbg_printf("%s data length: %d, bytes per row %d\n", __func__, data_len_in_bytes, bits_per_row/8);
    /* dbg_printf("\n\n%s color %d, plane raster x %d, raster y %d, image info pixels: %d, bytes: %d, height: %d\n\n", 
               __func__,
               plane_color,
               plane->RASTER_X,
               plane->RASTER_Y,
               plane->image_info.image_width_in_pixels,
               plane->image_info.image_data_width_in_bytes,
               plane->image_info.image_height_in_lines); */


    // If no compression was specified, set it to none.
    if ( comp_info == 0 )
    {
        plane->compression_info.comp_type = COMPTYPE_NONE;
    }
    else
    {
        plane->compression_info = *comp_info;
    }

    plane->BlockDataType = strip_type;

}
 

static void 
gen_process_mono_strip( uint8_t *input_buffer,
                        uint32_t input_line_width,
                        uint32_t input_num_lines,
                        uint8_t *output_buffer,
                        uint32_t output_line_width,
                        uint32_t output_num_lines,
                        bool soi,
                        bool eoi,
                        bool left_to_right,
                        uint32_t idma )
{
    dma_config_data_t *user_data = 0;
    uint32_t chan = idma >> 1;

    DBG_SLOG_ENTRY

    dbg_printf("%s idma %d chan %d SOI %d, EOI %d\n", __func__, idma, chan, soi, eoi);

    // MIP generator only supports data from the Object Type IDMA. This data
    // is halftoned and 1, 2 or 4 bits per pixel. 
    ASSERT( idma == IDMA_OBJECT_TYPE );

    // Setup raster idma with the output data buffer from the color pipe
    pipr_idma_set_line_width_in_bytes( idma_handle[chan], input_line_width );

    pipr_idma_add_input_buffer( idma_handle[chan], 
                                (char *)input_buffer, 
                                input_line_width * input_num_lines, 
                                soi, 
                                eoi, 
                                left_to_right, 
                                false, // repeat, 
                                user_data ); 

    // Setup the raster output dma with the provided data buffer
    raster_odma_set_line_width_in_bytes( odma_handle[0], output_line_width );

    user_data = gen_get_dma_config_data();

    raster_odma_add_input_buffer( odma_handle[0], 
                                  (char *)output_buffer, 
                                  output_line_width*output_num_lines, 
                                  output_num_lines, 
                                  soi, 
                                  eoi, 
                                  user_data );

    // Kick off the DMA's
    raster_odma_start_xfer( odma_handle[0] );
    pipr_idma_start_xfer( idma_handle[chan] );

    // Wait for the DMA's to finish. Waiting for the output DMA should be good
    // enough. Perhaps should confirm input has finished as a debug step.
    raster_odma_wait_for_desc_done(0);

    // Save the state of the raster pipe
    // REVISIT DAB 

    // Free up the dma_data_config_t objects that were used in the DMA
    gen_free_dma_config_data_objects();

    DBG_SLOG_EXIT
}



static dma_config_data_t *gen_get_dma_config_data( void )
{
    dma_config_data_t *ptr;

    ptr = (dma_config_data_t *)MEM_MALLOC_ALIGN(sizeof(dma_config_data_t)+8, 32);
    if (ptr != 0)
    {
        memset((void *)ptr, 0, sizeof(dma_config_data_t));
        DBG_MEMLOG(LOG_DEBUG_H,"MALLOC dma_config_data_t 0x%x\n", ptr);
             
        if (last_dma_config != 0)
        {
            last_dma_config->next = ptr;
        }
        else
        { 
            first_dma_config = ptr;
        }
        last_dma_config = ptr;

    }
    ASSERT(ptr != 0);
    return ptr;
}


static void gen_free_dma_config_data_objects( void )
{
    dma_config_data_t *ptr;

    // Free the DMA data configuration allocations. 
    while (first_dma_config)
    {
        ptr = first_dma_config->next;
        MEM_FREE_AND_NULL(first_dma_config);
        first_dma_config = ptr;
    }

    first_dma_config = 0;
    last_dma_config = 0;
}



// Open the display image pipe to print a page. Open all drivers necessary and
// prepare hardware to a known state; e.g. power save may have been active.
static error_type_t gen_open_image_pipe()
{
    error_type_t status = FAIL;
   // uint32_t value;

    DBG_SLOG_ENTRY

    dbg_printf("GEN MIP OPEN PIPE ENTRY\n");

  //  value = tx_mutex_get( &gen_mutex, TX_WAIT_FOREVER );
   
    // Open RASTER pipe required blocks

    while (status == FAIL)
    {
        status = pipr_open( &pipr_handle );
        tx_thread_sleep(1);
    }

    mip_soft_reset_and_enable(); // REVISIT DAB 

    //edger_reset(); // REVISIT DAB 

    // Open raster image pipe input DMAs.
    pipr_idma_open(&idma_handle[1], 1); // MIP generator will only use IDMA 1
    pipr_idma_register_notifcation_callback( idma_handle[0], 0);
    pipr_idma_register_notifcation_callback( idma_handle[1], 0);

    // Open raster image pipe output DMA 
    raster_odma_open( &odma_handle[0], 0 ); 
    raster_odma_register_notifcation_callback( odma_handle[0], 0);

    DBG_SLOG_EXIT

    return OK;
}


error_type_t gen_close_image_pipe()
{
    DBG_SLOG_ENTRY

    dbg_printf("GEN MIP CLOSE PIPE ENTRY\n");

    // Close PIPR
    pipr_close( pipr_handle );

    // Close RASTER pipe required blocks
    pipr_idma_register_notifcation_callback( idma_handle[1], 0);
    pipr_idma_close( idma_handle[1] );

    // Close raster image pipe output DMA 
    raster_odma_register_notifcation_callback( odma_handle[0], 0);
    raster_odma_close( odma_handle[0] ); 

//    tx_mutex_put( &gen_mutex );

    DBG_SLOG_EXIT

    return OK;
}


