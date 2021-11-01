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
 * \file generator_decompress.c 
 * \brief Implements a decompression generator. JBIG is supported initially. 
 *  
 *  The intended usage for this generator is to decompress JBIG encoded planar
 *  print data. Specifically it is intended to be used with JBIG compressed
 *  host rendered data, historically this has been ZJS data.
 *  
 *  There are two cases to be considered here, the first is where every JBIG
 *  image is contained in a single input buffer; i.e. there is a one to one
 *  correspondence to input and output buffers. The other case is where a JBIG
 *  image may be transmitted in multiple input buffers; e.g. an entire page
 *  is one JBIG image, but there are several input buffers represented as
 *  "plane" data that are transmitted. In the first case, the output buffer
 *  should be allocated to match the size specified by the input buffer and
 *  the output buffer will replace the input buffer in the plane. In the other
 *  case, the output buffers are fixed in size and one or more may be produced
 *  for each input buffer. Note that a partial output buffer may be filled in
 *  this case and therefore the state must be remembered when the next input
 *  buffer is received.
 */

/** include files **/
#include <stdint.h>
#include <string.h>

#include "agRouter.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "generator_inits.h"
#include "jbig.h"
#include "jbig_api.h"
#include "print_mode_api.h"



/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define GEN_STACK_SIZE 2048
#define GEN_QUEUE_SIZE 10
#define NUM_LINES_OUTPUT_BUFFER 258 // switched to 258 to work with both scaled and non-scaled color data. Still should consider making
                                    // part of the print mode condiguration.

#define DBG_PRFX "GEN_DECOMPRESS: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT( 3 )


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */
typedef struct gen_decompress_context_s
{
    mlimiter_t *limiter;
    PAGE_DATA *page;
    bool single_jbig_image_per_buffer; 
    uint32_t lines_left_to_decompress;
    bool image_decode_in_progress;
    uint32_t plane_count;
} gen_decompress_context_t;

typedef struct decompressed_output_buffer_s
{
    struct decompressed_output_buffer_s *next_buffer;
    uint32_t num_output_lines;
    BigBuffer_t *big_buffer;
    uint32_t size;
    uint8_t *data;
    bool buffer_full;
    bool last_buffer;
} decompressed_output_buffer_t;


/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */
static TX_QUEUE gen_decompress_queue;
static TX_THREAD gen_thread;
static ALIGN8 uint8_t gen_stack[GEN_STACK_SIZE];
static MESSAGE gen_msg_buf[GEN_QUEUE_SIZE];
static gen_decompress_context_t gen_context;
static CURRENT_RESOURCE  *our_resource = NULL;
static bool out_buffer_full;
static decompressed_output_buffer_t *reuse_container;
static jbig_handle_t *jbig_decode_handle;


/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */
static void 
gen_decompress_task( uint32_t );

static bool 
pip_proc_strip( PLANED *plane,
                uint32_t datalen,
                uint32_t bits_per_row, 
                uint32_t num_rows, 
                uint32_t lines_per_page, 
                uint32_t xres, 
                uint32_t yres,
                uint32_t output_bpp,
                bool start_new_odma ) ;

static void 
gen_print_plane_add_data(  PLANED* plane, 
                           COLOR plane_color, 
                           BigBuffer_t* big_buffer, 
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



static error_type_t 
pip_jbig_decompress_mono_strips( uint8_t *data_in,
                                 uint32_t Xd,
                                 uint32_t Yd,
                                 uint32_t ld,
                                 uint32_t num_data_rows,
                                 uint32_t size_of_compressed_data,
                                 uint32_t size_of_output_data,
                                 decompressed_output_buffer_t **decompressed_data,
                                 bool start_new_odma );

static TX_EVENT_FLAGS_GROUP jbig_flags;
#define JBIG_DECODE_DONE 1
#define JBIG_DECOMPRESS_ODMA_DONE 0x10
#define JBIG_DECOMPRESS_IDMA_DONE 0x20
#define JBIG_DECOMPRESS_ODMA_STOP 0x40


static void pip_jbig_decode_callback (void *user_data, uint32_t int_status)
{
    DBG_PRINTF_NOTICE("%s status 0x%x\n", __func__, int_status );

    if (int_status & JBIG_CONTROL_FLAG)
    {
        tx_event_flags_set( &jbig_flags, JBIG_DECODE_DONE, TX_OR );
    }

    if(int_status & JBIG_ODMA_COMPLETE_FLAG)
    {
        DBG_MEMLOG_INFO("decompress lines left = %d\n", int_status & JBIG_EVENT_STATUS_MASK);

        if(( int_status & JBIG_ODMA_LINES_LEFT_MASK) == 0)
        {
             tx_event_flags_set( &jbig_flags, JBIG_DECOMPRESS_ODMA_STOP, TX_OR );
        }
        else
        {
            tx_event_flags_set( &jbig_flags, JBIG_DECOMPRESS_ODMA_DONE, TX_OR );
        }
        
    }

    if (int_status & JBIG_IDMA_COMPLETE_FLAG)
    {
        tx_event_flags_set( &jbig_flags, JBIG_DECOMPRESS_IDMA_DONE, TX_OR );
    }

}


/** public functions **/                              
 
/** 
 * \brief Initialize the compression generator.
 * 
 * \param None.
 *
 * \retval uint32_t 0
 * 
 **/
uint32_t gen_decompress_init( void )
{
    uint32_t status;

    // Create a mail box and register it.
    status = tx_queue_create(&gen_decompress_queue,
                             0, 
                             SYS_MSG_SIZE, 
                             gen_msg_buf, 
                             GEN_QUEUE_SIZE * sizeof(MESSAGE) ); /* create the queue */
    ASSERT(status == TX_SUCCESS);

    // Register the queue with the router.
    SYRegister(GEN_DECOMPRESS_ID, &gen_decompress_queue);

    // Register the resource
    rm_register(e_GenDecompress,  GEN_DECOMPRESS_ID);

    // tx_event_flags_create(&jbig_flags, "jbig flags");

    // start the skeleton generator task
    tx_thread_create(&gen_thread,
                     "gen_decompress",
                     gen_decompress_task,
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
 * \param value Dummy.
 * 
 * \retval None
 *
 * 
 **/
void gen_decompress_task( uint32_t value )
{
    MESSAGE msg, new_msg;
    
    error_type_t ret;
    UINT txret;
    PAGE_DATA *page = NULL;
    PLANED *plane = NULL;
   
    gen_context.limiter = mlimiter_by_name("print");
    SysWaitForInit();

    DBG_PRINTF_INFO("%s STARTED\n", __func__);

    // main loop
    while (1)
    {
        txret = tx_queue_receive (&gen_decompress_queue, &msg, TX_WAIT_FOREVER);     // get a message
        XASSERT( txret==TX_SUCCESS, txret );

        DBG_PRINTF_INFO("%s got message %d\n", __func__, msg.msgType);

        switch (msg.msgType)
        {
            case MSG_RESOURCES:
            {
                page = NULL;
                plane = NULL;

                our_resource = (CURRENT_RESOURCE *) msg.param3;
                DBG_PRINTF_INFO("%s: our_resource.destination = %d\n", __func__, our_resource->Destination);

                DBG_PRINTF_INFO("JOB type: %s\n", to_string_job_types( our_resource->JobResources->JobType));
                break;
            }

            case MSG_ACKRECIPE:
            {
                // Job is starting, we do nothing here, just wait for messages.
                break;
            }

            case MSG_JOBSTART:
            {
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }

            case MSG_JOBSHEETSTART:
            {
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }

            case MSG_JOBPAGESTART:
            {
                ASSERT( NULL != our_resource );

                page = (PAGE_DATA *)msg.param3;
                ASSERT(page != NULL);
                ASSERT(IS_PAGE_DATA(page));

                DBG_PRINTF_INFO("%s Document type: %s\n", __func__, to_string_document_type( page->DocType ));
                
                gen_context.plane_count = 0;
                gen_context.page = page;
                gen_context.single_jbig_image_per_buffer = false; // REVISIT DAB hardcode for now
                gen_context.lines_left_to_decompress = page->RASTER_Y;
                out_buffer_full = true; // Force creation of new output buffer for first plane on page
                reuse_container = 0;

                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );

                if( jbig_open( &jbig_decode_handle, JBIG_DECODE_BLOCK ) != OK )
                {
                    DBG_PRINTF_ERR("JBIG: DECOMPRESSION FAILED TO GET HANDLE\n");
                    ASSERT(0);
                }

                break;
            }

            case MSG_JOBPLANESTART:
            {
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }

            case MSG_JOBPLANEDATA:
            {
                ASSERT(NULL != our_resource);

                plane = (PLANED *)(msg.param3);
                ASSERT(IS_PLANED(plane));

                DBG_PRINTF_INFO("%s MSG_JOBPLANEDATA color %d, %d, 0x%x, plane = %#x, last %d\n",
                                __func__, msg.param1,msg.param2, msg.param3, (uint32_t)plane, plane->LastData );
                DBG_PRINTF_INFO("image data width in bytes %d, height in lines %d, bpp %d\n",
                                plane->image_info.image_data_width_in_bytes,
                                plane->image_info.image_height_in_lines,
                                print_mode_get_input_bpp(page->print_mode) ));

                out_buffer_full = pip_proc_strip( plane,
                                                  plane->DataLength, 
                                                  plane->image_info.image_data_width_in_bytes*8,  
                                                  plane->image_info.image_height_in_lines, 
                                                  page->RASTER_Y, 
                                                  page->RESOLUTION_X, 
                                                  page->RESOLUTION_Y, 
                                                  print_mode_get_input_bpp(page->print_mode),
                                                  out_buffer_full );
                break;
            }

            case MSG_JOBPLANEEND:
            {
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret ); 

                jbig_close(jbig_decode_handle);

                gen_context.lines_left_to_decompress = page->RASTER_Y;
                out_buffer_full = true; // Force creation of new output buffer for first plane on page

                // Reuse container should be zero because all buffers will have
                // completed.
                ASSERT( reuse_container == 0 );
                reuse_container = 0;

                gen_context.plane_count++;

                if ( (page->page_output_color_mode == e_Color) && (gen_context.plane_count < 4) )
                {

                    if( jbig_open( &jbig_decode_handle, JBIG_DECODE_BLOCK ) != OK )
                    {
                        DBG_PRINTF_ERR("JBIG: DECOMPRESSION FAILED TO GET HANDLE\n");
                        ASSERT(0);
                    }
                }
                break;
            }

            case MSG_JOBPAGEEND:
            {
                ASSERT( NULL != our_resource );
                page = NULL;

                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  

                break;
            }

            case MSG_JOBEND:
            {
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
                DBG_PRINTF_INFO("MSG_CANCELJOB\n");

                ASSERT( NULL != our_resource );
                // ACK the system job mgr
                new_msg.msgType = MSG_CANCELJOB;
                new_msg.param1 = SYS_ACK;
                new_msg.param2 = GEN_PIPP_PIPR_ID;
                new_msg.param3 = msg.param3;
                ret = SYMsgSend( SJMID, &new_msg);
                ASSERT( OK == ret );

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

                jbig_close( jbig_decode_handle ); // REVISIT DAB is this necessary?
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


static bool 
pip_proc_strip( PLANED *plane,
                uint32_t datalen,
                uint32_t bits_per_row, 
                uint32_t num_rows, 
                uint32_t lines_per_page, 
                uint32_t xres, 
                uint32_t yres,
                uint32_t output_bpp,
                bool start_new_odma )
{
    uint32_t data_out_size;
    compression_info_t compression_info;
    COLOR color = 0;
    MESSAGE  msg;
    uint32_t output_line_width;
    PLANED *new_plane;
    uint32_t num_data_rows = num_rows;
    decompressed_output_buffer_t *data_container = 0;
    decompressed_output_buffer_t *container = 0;
    decompressed_output_buffer_t *temp_container = 0;
    PLANE_TYPE plane_type;

    DBG_MEMLOG_WARN("%s ENTRY\n", __func__);

    msg.msgType = MSG_JOBPLANEDATA;
    plane_type = plane->BlockDataType;

    DBG_PRINTF_INFO("DECOMPRESS %s ENTRY plane 0x%x, last data %d, bits_per_row %d, num_rows %d, plane type %d\n",  
                    __func__, plane, plane->LastData, bits_per_row, num_data_rows, plane_type);

    switch ( plane_type )
    {
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

        // These plane types are not supported by the compression generator.
    case PACKED_RGB: 
    case PACKED_XRGB:
    case PACKED_RGBX:
    case GRAYSCALE: 
    default:
        ASSERT(0);
        break;
    }

    msg.param1 = color;

    mlimiter_start(gen_context.limiter);

    // If this is a single JBIG image per input buffer, then decompress exactly
    // the number of lines specified in the input buffer. If it is a single 
    // image for the page then the output buffer size will be fixed.
    if (gen_context.single_jbig_image_per_buffer == false)
    {
        num_data_rows = NUM_LINES_OUTPUT_BUFFER;
    }
    
    // Calculate the data output size.
    DBG_PRINTF_DEBUG("bits per row %d, num_rows %d, input bpp %d\n", 
                     bits_per_row, num_rows, compression_info.comp_details.rgb.bits_per_pixel);
    data_out_size = num_data_rows * bits_per_row / 8;
    output_line_width = data_out_size/num_data_rows; // input line width in bytes

    compression_info.comp_type = COMPTYPE_NONE;

    data_container = reuse_container;

    pip_jbig_decompress_mono_strips( plane->big_buffer->data,
                                     plane->compression_info.comp_details.jbig.Xd,
                                     plane->compression_info.comp_details.jbig.Yd,
                                     plane->compression_info.comp_details.jbig.L0,
                                     num_data_rows,
                                     datalen,
                                     data_out_size,
                                     &data_container,
                                     start_new_odma );

    // Free the old data and attach the new.
    BigBuffer_Free(plane->big_buffer);
    MEM_FREE_AND_NULL( plane );

    container = data_container;
    while ( container )
    {
        DBG_PRINTF_DEBUG("CONTAINER 0x%x, DATA BUFFER 0x%x, lines: %d, full %d\n", 
                         container, container->data, container->num_output_lines, container->buffer_full);

        if (container->buffer_full == true)
        {
            new_plane = (PLANED*)MEM_MALLOC_LIMITED( gen_context.limiter, sizeof(PLANED));
            memset(link, 0, sizeof(PLANED));

            SET_PLANED_COOKIE( new_plane );

            msg.param3 = new_plane;

            // For color output, the original plane was a single RGB plane that
            // is now four planes: C, M, Y, K. So four new planes need to be
            // created and added to the page, and the original plane destroyed.
            msg.param1 = color;

            DBG_MEMLOG_WARN("container = %x last_buffer = %d num_lines = %d\n",
                            container,container->last_buffer,container->num_output_lines);

            // Now update the plane to reflect the transformation.
            gen_print_plane_add_data( new_plane, 
                                      color, 
                                      container->big_buffer, 
                                      data_out_size,  
                                      plane_type, 
                                      container->last_buffer,
                                      xres, 
                                      yres,
                                      bits_per_row,  
                                      container->num_output_lines, //num_data_rows, 
                                      output_bpp,
                                      true, // left to right image data
                                      &compression_info);

            DBG_MEMLOG_INFO("DECOMPRESS Sending plane data. Num rows %d \n", container->num_output_lines);
            SYMsgSend( our_resource->Destination, &msg ); 

            // Free the container now, it is no longer needed.
            temp_container = container;
            container = container->next_buffer;
            DBG_PRINTF_DEBUG("%s **freeing container 0x%x\n", __func__, temp_container);
            MEM_FREE_AND_NULL( temp_container );

            reuse_container = 0;
        }
        else
        {
            // This is the case of a partially filled output buffer. Remember
            // that a container must be re-used and exit the loop.
            reuse_container = container;

            // If this data buffer is not full, then there should NOT be another
            // data container in the list!
            ASSERT(container->next_buffer == 0);
            break;
        }
    }

    mlimiter_stop( gen_context.limiter );

    return container->buffer_full;
}

static
decompressed_output_buffer_t * pip_jbig_add_output_buffer(uint32_t size_of_output_data, uint32_t num_data_rows )
{
    uint32_t num_lines_in_output_buffer = num_data_rows;
    uint8_t *data_out = 0;
    BigBuffer_t *bb_out = 0;
    decompressed_output_buffer_t *data_out_container;

    if (gen_context.lines_left_to_decompress <= num_data_rows)
    {
        DBG_MEMLOG_WARN("setting last buffer with num_data_rows = %d\n",num_data_rows);
        num_lines_in_output_buffer = gen_context.lines_left_to_decompress;
        gen_context.lines_left_to_decompress = 0;
    }
    else
    {
        gen_context.lines_left_to_decompress -= num_data_rows; // REVISIT DAB
    }

    DBG_PRINTF_INFO("LINES LEFT TO DECOMPRESS %d\n", gen_context.lines_left_to_decompress );

    bb_out = dma_buffer_malloc( gen_context.limiter, size_of_output_data );
    flushed_ptr = dma_buffer_map_single( bb_out, DMA_FROM_DEVICE );
    //    data_out = MEM_MALLOC_ALIGN_LIMITED( gen_context.limiter, size_of_output_data, e_32_byte );
    //    cpu_dcache_invalidate_region( data_out, size_of_output_data );

    data_out_container = MEM_MALLOC_LIMITED( gen_context.limiter, sizeof( decompressed_output_buffer_t ));
    memset((void*)data_out_container,0,sizeof( decompressed_output_buffer_t ));
    data_out_container->big_buffer = bb_out;
    //    data_out_container->data = data_out;
    data_out_container->next_buffer = 0;
    data_out_container->buffer_full = false;
    data_out_container->num_output_lines = num_lines_in_output_buffer; 
    data_out_container->size = size_of_output_data;
    data_out_container->last_buffer = 0;

    DBG_MEMLOG_WARN("DECOMPRESS sending buffer to jbig %x\n", data_out);
    ASSERT(data_out);
    jbig_add_output_buffer( jbig_decode_handle, 
                            flushed_ptr,   
                            size_of_output_data, 
                            bb_out ); // REVISIT DAB need to add useful user data?  sk: data_out ? data_out_container ? big_buffer ?

    return data_out_container;
}

static error_type_t 
pip_jbig_decompress_mono_strips( uint8_t *data_in,
                                 uint32_t Xd,
                                 uint32_t Yd,
                                 uint32_t ld,
                                 uint32_t num_data_rows,
                                 uint32_t size_of_compressed_data,
                                 uint32_t size_of_output_data,
                                 decompressed_output_buffer_t **container,
                                 bool start_new_odma )
{
    uint32_t num_lines_in_output_buffer;
    uint32_t flags;
    bool decompress_done = false;
    decompressed_output_buffer_t *current_container = 0;
    decompressed_output_buffer_t *data_out_container = 0;
    decompressed_output_buffer_t *first_container = 0;
    bool last_input_buffer;
    
    DBG_PRINTF_INFO("%s input size: %d, output size %d, xd %d, yd %d, ld %d\n",
                    __func__, size_of_compressed_data, size_of_output_data, Xd, Yd, ld);

    DBG_PRINTF_INFO("container 0x%x\n", *container);
    
    last_input_buffer = size_of_compressed_data < 65536 ? true : false; // REVISIT DAB 

    num_lines_in_output_buffer = num_data_rows;
    
    DBG_MEMLOG_WARN("jbig test: set the decode\n");
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
                           (uint32_t)data_in, 
                           size_of_compressed_data, 
                           NULL );

    if(gen_context.image_decode_in_progress == false)
    {
        jbig_start_transaction( jbig_decode_handle, pip_jbig_decode_callback );
        gen_context.image_decode_in_progress = true;

    }


    if( !start_new_odma)
    {
       data_out_container = *container;
    }

    // we will loop here until the jbig input buffer has completed and/or the jbig output image is complete
    while ( decompress_done == false )
    {
        if (start_new_odma == true)
        {
            data_out_container = pip_jbig_add_output_buffer( size_of_output_data, num_data_rows );
            DBG_MEMLOG_WARN("%s adding jbig odma buffer %x container = %x\n", __func__, data_out_container->data, data_out_container );
            DBG_PRINTF_INFO("%s adding jbig odma buffer %x container = %x\n", __func__, data_out_container->data, data_out_container );
            start_new_odma = false;
        }

        if (data_out_container)
        {
            if (first_container == NULL)
            {
                first_container = data_out_container;
                // Return the head of the decompressed data container list
                *container = data_out_container;
                DBG_PRINTF_INFO("%s setting first container = %x\n",__func__, *container );
                current_container = data_out_container;
            }
            else
            {
                // Add the new container on to the end of the list.
                current_container->next_buffer = data_out_container;
                current_container = data_out_container;
            }
        }

        DBG_PRINTF_INFO("%s Waiting for OUTPUT DMA to complete \n", __func__);

        // Wait forever jbig dma's to complete.
        JBIG_BLOCKING_WAIT_ANY( jbig_decode_handle );
        
        tx_event_flags_get( &jbig_flags, 
                            JBIG_DECOMPRESS_ODMA_STOP | JBIG_DECOMPRESS_IDMA_DONE | JBIG_DECOMPRESS_ODMA_DONE | JBIG_DECODE_DONE, 
                            TX_OR_CLEAR, 
                            &flags, 
                            TX_WAIT_FOREVER );
        */
        DBG_MEMLOG_WARN("jbig flags = %x\n",flags);

        if(flags & JBIG_DECOMPRESS_IDMA_DONE)
        {
            // if we get here we need another input buffer so we mark the decompress_done flag true so that
            // we will bail out of the while loop and return so the caller can send us another input data.
            DBG_MEMLOG_WARN("JBIG_DECOMPRESS_IDMA_DONE\n");
            if ( false == last_input_buffer )
            {
                decompress_done = true;
                
            }

        }

        if(flags & JBIG_DECODE_DONE)
        {
            DBG_MEMLOG_WARN("JBIG_DECODE_DONE\n");
            // wait for ODMA to complete
        }

        if (flags & JBIG_DECOMPRESS_ODMA_DONE)
        {
            DBG_MEMLOG_WARN("JBIG_DECOMPRESS_ODMA_DONE\n");

            // Start Another ODMA
            start_new_odma = true;
            current_container->buffer_full = true;

        }

        if(flags & JBIG_DECOMPRESS_ODMA_STOP)
        {
            DBG_MEMLOG_WARN("JBIG_DECOMPRESS_ODMA_STOP\n");
            DBG_PRINTF_INFO("*** LAST *** DMA OUTPUT COMPLETE container 0x%x lines %d\n", current_container, current_container->num_output_lines));

            current_container->last_buffer = true;
            current_container->buffer_full = true;
            jbig_stop_transaction( jbig_decode_handle );
            gen_context.image_decode_in_progress = false;

            // If we get here we should be done with all Jbig events so we can just exit the while loop at this point without having to 
            // check any more jbig states.
            break;
        }

        // set data_out_container to 0 to indicate no output buffer has been added in case we loop back around on an IDMA complete but we don't
        // need to add another output buffer
        data_out_container = 0;

    } // while


    return OK;
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
                           uint32_t bits_per_row,  
                           uint32_t strip_height,
                           uint32_t bits_per_pixel,
                           bool left_to_right,
                           compression_info_t* comp_info )
{
    plane->sysPlaneColor = plane_color;
    plane->big_buffer    = data;
    plane->DataLength    = data_len_in_bytes;
    plane->LastData      = (uint16_t) last_strip;
    plane->image_resolution.x = resolution_x;
    plane->image_resolution.y = resolution_y;
    plane->RASTER_X      = bits_per_row; 
    plane->RASTER_Y      = strip_height;
    plane->VIDEO_BPP     = bits_per_pixel;

    plane->image_info.left_to_right = left_to_right;
    plane->image_info.image_bpp = bits_per_pixel;
    plane->image_info.image_data_width_in_bytes = bits_per_row/8; 
    plane->image_info.image_height_in_lines     = strip_height;
    plane->image_info.image_width_in_pixels     = bits_per_row/bits_per_pixel; 

    DBG_PRINTF_DEBUG("%s color %d, plane raster x %d, raster y %d, image info pixels: %d, bytes: %d, height: %d\n", 
                     __func__,
                     plane_color,
                     plane->RASTER_X,
                     plane->RASTER_Y,
                     plane->image_info.image_width_in_pixels,
                     plane->image_info.image_data_width_in_bytes,
                     plane->image_info.image_height_in_lines); 

    ASSERT(comp_info); 
    plane->compression_info = *comp_info;

    plane->BlockDataType = strip_type;
}
