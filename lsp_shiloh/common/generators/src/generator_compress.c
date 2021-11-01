/**
 *
 * ============================================================================
 * Copyright (c) 2011-2013 Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file generator_compress.c 
 * \brief Implements a compression generator. JBIG is supported initially.
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
#include "dma_buffer.h"
#include "dbg_pnm.h"
#ifdef HAVE_IMAGELOG
#include "print_job_api.h"
#include "imagelog.h"
#endif

//#define DEBUG_VERIFY_COMPRESSION_RESULTS

#ifdef DEBUG_VERIFY_COMPRESSION_RESULTS
#define DEBUG_BUF_SIZE (132*5120*2/8)  // 132 lines * 5120 pixels * 2bpp / 8 bits/byte
static BigBuffer_t* gencomp_dbg_buf = NULL;
#endif


/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define GEN_STACK_SIZE POSIX_MIN_STACK_SIZE
#define GEN_QUEUE_SIZE 10

#define DBG_PRFX "GEN_COMPRESS: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT( 2 )


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */
typedef struct gen_compress_context_s
{
    mlimiter_t *limiter;
    jbig_handle_t* jbig_handle;
} gen_compress_context_t;


/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */
static mqd_t gen_compress_queue;
static pthread_t gen_thread;
static ALIGN8 uint8_t gen_stack[GEN_STACK_SIZE];
static gen_compress_context_t gen_context;


/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */
static void* 
gen_compress_task( void *unused );

static void 
gen_compress_proc_strip(MODULE_ID dest, PLANED *plane );

/** public functions **/                              
 
/** 
 * \brief Initialize the compression generator.
 * 
 * \param None.
 *
 * \retval uint32_t 0
 * 
 **/
uint32_t gen_compress_init( void )
{
    // Create a mail box and register it.
    posix_create_MESSAGE_queue( &gen_compress_queue, "/gen_comress_mq", GEN_QUEUE_SIZE );
    // Register the queue with the router.
    router_register_queue(GEN_COMPRESS_ID, gen_compress_queue);
    // Register the resource
    rm_register(e_GenCompressJBIG,  GEN_COMPRESS_ID);
    // start the generator task
    posix_create_thread(&gen_thread, 
                        gen_compress_task,
                        0,
                        "GenCompThread",
                        gen_stack,
                        GEN_STACK_SIZE,
                        POSIX_THR_PRI_REALTIME);
    return 0;
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
void *gen_compress_task( void *unused )
{
    MESSAGE msg, new_msg;
    int iret;
    error_type_t ret;
    PAGE_DATA *page = NULL;
    PLANED *plane = NULL;
#ifdef HAVE_IMAGELOG
    PLANED *il_plane = NULL;
#endif
    error_type_t err;
    CURRENT_RESOURCE *our_resource = NULL;

    memset(&gen_context, 0, sizeof(gen_compress_context_t));
   
    SysWaitForInit();

    // main loop
    while (1)
    {
        iret = posix_wait_for_message(gen_compress_queue, (char*)&msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);
        if (iret != 0)
            continue;  // signal interrupt 

        switch (msg.msgType)
        {
            case MSG_RESOURCES:
            {
                page = NULL;
                plane = NULL;

                our_resource = (CURRENT_RESOURCE *) msg.param3;
                DBG_PRINTF_NOTICE("%s: our_resource.destination = %d\n", __func__, our_resource->Destination);

                DBG_PRINTF_NOTICE("JOB type: %s\n", to_string_job_types( our_resource->JobResources->JobType));
                break;
            }

            case MSG_ACKRECIPE:
            {
                // Job is starting, we do nothing here, just wait for messages.
                break;
            }

            case MSG_JOBSTART:
            {
                gen_context.limiter = mlimiter_by_name("print");
                mlimiter_start( gen_context.limiter );

                err = jbig_open(&gen_context.jbig_handle, JBIG_CODEC_BLOCK);
                ASSERT(err == OK);

#ifdef DEBUG_VERIFY_COMPRESSION_RESULTS
                gencomp_dbg_buf = BigBuffer_Malloc(gen_context.limiter, DEBUG_BUF_SIZE);
                ASSERT(gencomp_dbg_buf != NULL);
#endif


                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  

#ifdef HAVE_IMAGELOG
				if(get_PrintImageLogEnabled())
                    SYMsgSend(IMAGE_LOG_ID, &msg);
#endif
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

                DBG_PRINTF_NOTICE("%s Document type: %s\n", __func__, to_string_document_type( page->DocType ));
                
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );

#ifdef HAVE_IMAGELOG
				if(get_PrintImageLogEnabled())
                    SYMsgSend(IMAGE_LOG_ID, &msg);
//	IL_new_page_data(page->page_data->image_output_info.image_width_in_pixels, page->page_data->image_output_info.image_height_in_lines);
//	IL_PDF_NewPage(pPageInfo->nDataWidthBytes, pPageInfo->nImageHeightLines);
#endif
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

                DBG_PRINTF_DEBUG("%s MSG_JOBPLANEDATA color %d, %d, 0x%x, plane = %#x\n", __func__, msg.param1,msg.param2, msg.param3, (uint32_t)plane);

				if ( plane->LastData )
					DBG_PRINTF_ERR("[RYU] Last data\n");
#ifdef HAVE_IMAGELOG
				if(get_PrintImageLogEnabled())
				{
					MESSAGE il_msg;
					mlimiter_t* limiter = mlimiter_by_name("print");
					BigBuffer_t* databuf;


//					output_buffer = plane->big_buffer;
					dma_buffer_map_single(plane->big_buffer, DMA_TO_DEVICE);
                	dma_buffer_unmap_single(plane->big_buffer, DMA_TO_DEVICE);
					void* data = dma_buffer_mmap_forcpu(plane->big_buffer);
//					databuf = dma_buffer_copy_from( (void*)plane->big_buffer->data ,plane->DataLength);
					databuf = dma_buffer_copy_from( data ,plane->DataLength);
					
					il_plane = print_construct_plane( limiter,
								plane->sysPlaneColor,
								databuf,
								plane->DataLength,
								plane->BlockDataType,
								plane->LastData,
								plane->image_info.image_dpi.x,
								plane->image_info.image_dpi.y,
								plane->image_info.image_width_in_pixels,
								plane->image_info.image_height_in_lines,
								plane->image_info.image_bpp,
								plane->image_info.left_to_right,
								&plane->compression_info); 

					il_msg.msgType = MSG_JOBPLANEDATA;
					il_msg.param1 = 0;
					il_msg.param2 = 0;
					il_msg.param3 = il_plane;
					SYMsgSend(IMAGE_LOG_ID, &il_msg);
                    dma_buffer_unmmap_forcpu( plane->big_buffer );
//					IL_get_raw_data(plane);
				}
#endif
                gen_compress_proc_strip(our_resource->Destination, plane );
                break;
            }

            case MSG_JOBPLANEEND:
            {
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }

            case MSG_JOBPAGEEND:
            {
				DBG_PRINTF_ERR("MSG_JOBPAGEEND\n");
                ASSERT( NULL != our_resource );
                page = NULL;

                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
#ifdef HAVE_IMAGELOG
                if(get_PrintImageLogEnabled()) 
                    SYMsgSend(IMAGE_LOG_ID, &msg);
#endif
                break;
            }

            case MSG_JOBABORT:
            case MSG_JOBEND:
            {
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );        
                
#ifdef HAVE_IMAGELOG
                if(get_PrintImageLogEnabled())
                    SYMsgSend(IMAGE_LOG_ID, &msg);
#endif

#ifdef DEBUG_VERIFY_COMPRESSION_RESULTS
                if (gencomp_dbg_buf != NULL)
                {
                    BigBuffer_Free(gencomp_dbg_buf);
                    gencomp_dbg_buf = NULL;
                }
#endif

                if (gen_context.limiter)
                {
                    mlimiter_stop( gen_context.limiter );
                    gen_context.limiter = NULL;
                }

                if (gen_context.jbig_handle)
                {
                    jbig_close(gen_context.jbig_handle);
                    gen_context.jbig_handle = NULL;
                }

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

#ifdef DEBUG_VERIFY_COMPRESSION_RESULTS
                if (gencomp_dbg_buf != NULL)
                {
                    BigBuffer_Free(gencomp_dbg_buf);
                    gencomp_dbg_buf = NULL;
                }
#endif

                if (gen_context.limiter)
                {
                    mlimiter_stop( gen_context.limiter );
                    gen_context.limiter = NULL;
                }

                if (gen_context.jbig_handle)
                {
                    jbig_close(gen_context.jbig_handle);
                    gen_context.jbig_handle = NULL;
                }

//                ASSERT( NULL != our_resource );
                // ACK the system job mgr
                new_msg.msgType = MSG_CANCELJOB;
                new_msg.param1 = SYS_ACK;
                new_msg.param2 = GEN_COMPRESS_ID;
                new_msg.param3 = msg.param3;
                ret = SYMsgSend( SJMID, &new_msg);
                ASSERT( OK == ret );

                if (our_resource)
                {
                    // free resources
                    new_msg.msgType = MSG_FREERECIPE;
                    new_msg.param1 = 0;
                    new_msg.param2 = 0;
                    new_msg.param3 = our_resource;

                    our_resource = NULL;
                    page = NULL;
                    plane = NULL;

                    ret = SYMsgSend(SJMID, &new_msg);
                    ASSERT(OK == ret);
                }

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
    return 0;
}

#ifdef DEBUG_VERIFY_COMPRESSION_RESULTS
uint8_t last_good_buf[DEBUG_BUF_SIZE];
int gencomp_verify_decode(BigBuffer_t* orig_buf, BigBuffer_t* test_buf, uint32_t num_bytes, int strip_num)
{
    uint8_t* src_data;
    uint8_t* test_data;
    uint32_t i;
    int result = 0;

    // map buffers back to userspace since the have been converted to dma buffers by the jbig apis
    src_data = dma_buffer_mmap_forcpu(orig_buf);
    test_data = dma_buffer_mmap_forcpu(test_buf);

    // verify; source must equal test output
    for (i = 0; i < num_bytes; i++)
    {
        if (src_data[i] != test_data[i])
        {
            break;
        }
    }

    if (i != num_bytes)
    {
        DBG_PRINTF_ERR("%s: Decode verify failed strip %d at offset %d. Src is %d, decode is %d\n", __func__, strip_num, i, src_data[i], test_data[i]);
        DBG_PRINTF_ERR("%s: Decoded Buf:\n", __func__);
        hex_dump(test_data, num_bytes);
        DBG_PRINTF_ERR("%s: First 64 bytes of src:\n", __func__);
        hex_dump(src_data, MIN(num_bytes, 64));
        result = -1;
    }

    dma_buffer_unmmap_forcpu(orig_buf);
    dma_buffer_unmmap_forcpu(test_buf);
    return result;
}
#endif

static void 
gen_compress_proc_strip(MODULE_ID dest, PLANED *plane )
{
    MESSAGE  msg;
    BigBuffer_t* bb;
    uint32_t data_out_size;
    uint32_t bits_per_row; 
    uint32_t num_data_rows; 
    compression_info_t compression_info;
    uint32_t line_width_bytes;
    uint32_t compressed_size = 0;
    error_type_t err;

    // Calculate the data output buffer size.  It is padded to accommodate the
    // rare case of JBIG compression EXPANDING the intput data.
    data_out_size = plane->DataLength;
    data_out_size = (data_out_size*5)/4;

    line_width_bytes = plane->image_info.image_data_width_in_bytes;
    bits_per_row = line_width_bytes*8;
    num_data_rows = plane->image_info.image_height_in_lines;

    DBG_PRINTF_DEBUG("%s plane 0x%x. bits_per_row %d, num_rows %d, buf_in_size = %d, buf_out_size = %d\n",  
            __func__, plane, bits_per_row, num_data_rows, plane->DataLength, data_out_size);

    // Setup the compression info variable to describe the JBIG encoding
    compression_info.comp_type = COMPTYPE_JBIG;
    compression_info.comp_details.jbig.Xd = bits_per_row; 
    compression_info.comp_details.jbig.Yd = num_data_rows;
    compression_info.comp_details.jbig.L0 = num_data_rows;
    compression_info.comp_details.jbig.option_flags = JBIG_TPBON;

    bb = dma_buffer_malloc( gen_context.limiter, data_out_size );  // allocate buffer to compress into
    ASSERT(bb);

    err = jbig_encode_strip(gen_context.jbig_handle, plane->big_buffer, bb, bits_per_row, num_data_rows, num_data_rows, JBIG_TPBON, &compressed_size);
    ASSERT(err == OK && compressed_size > 0);

    DBG_PRINTF_DEBUG("%s Input size: %d  Output size: %d\n", __func__, plane->DataLength, compressed_size);

#ifdef DEBUG_VERIFY_COMPRESSION_RESULTS
    uint32_t bytes_decompressed;
    static int strip_num = 0;
    err = jbig_decode_strip(gen_context.jbig_handle, bb, compressed_size, gencomp_dbg_buf, 
                            compression_info.comp_details.jbig.Xd, 
                            compression_info.comp_details.jbig.Yd, 
                            compression_info.comp_details.jbig.L0, 
                            compression_info.comp_details.jbig.option_flags, &bytes_decompressed);
    XASSERT(err == OK, err);
    XASSERT(bytes_decompressed == plane->DataLength, bytes_decompressed);
    if(gencomp_verify_decode(plane->big_buffer, gencomp_dbg_buf, bytes_decompressed, ++strip_num) != 0)
    {
        uint8_t* encode_buf = dma_buffer_mmap_forcpu(bb);
        DBG_PRINTF_ERR("%s: Fail @ 1 Encode Result Buf size %d:\n", __func__, compressed_size);
        hex_dump(encode_buf, compressed_size);
        dma_buffer_unmmap_forcpu(bb);
        ASSERT(0);
    }
    uint8_t* dump_buf = dma_buffer_mmap_forcpu(bb);
    memcpy(last_good_buf, dump_buf, compressed_size);
    dma_buffer_unmmap_forcpu(bb);
#endif

    // Realloc the compressed buffer to free unused memory.
    dma_buffer_realloc(bb, compressed_size);

#ifdef DEBUG_VERIFY_COMPRESSION_RESULTS
//-jrs    uint32_t bytes_decompressed;
//-jrs    static int strip_num = 0;
    err = jbig_decode_strip(gen_context.jbig_handle, bb, compressed_size, gencomp_dbg_buf, 
                            compression_info.comp_details.jbig.Xd, 
                            compression_info.comp_details.jbig.Yd, 
                            compression_info.comp_details.jbig.L0, 
                            compression_info.comp_details.jbig.option_flags, &bytes_decompressed);
    XASSERT(err == OK, err);
    XASSERT(bytes_decompressed == plane->DataLength, bytes_decompressed);
    if(gencomp_verify_decode(plane->big_buffer, gencomp_dbg_buf, bytes_decompressed, strip_num) != 0)
    {
        uint8_t* encode_buf = dma_buffer_mmap_forcpu(bb);
        DBG_PRINTF_ERR("%s: Fail @ 2 Encode Result Buf size %d:\n", __func__, compressed_size);
        hex_dump(encode_buf, compressed_size);
        dma_buffer_unmmap_forcpu(bb);
        DBG_PRINTF_ERR("%s: Fail @ 2 Buf before realloc Buf size %d:\n", __func__, compressed_size);
        hex_dump(last_good_buf, compressed_size);
        ASSERT(0);
    }
#endif

//    DBG_MEMLOG_INFO("%s FREE MEMORY after free available %d\n", __func__, mlimiter_available( gen_context.limiter ));

    // Free the old data
    BigBuffer_Free(plane->big_buffer);

    // and attach the new.
    plane->big_buffer = bb;
    plane->DataLength = compressed_size;
    plane->compression_info = compression_info;

    // and forward the data along
    msg.msgType = MSG_JOBPLANEDATA;
    msg.param1 = plane->sysPlaneColor;
    msg.param2 = 0;
    msg.param3 = plane;

    DBG_PRINTF_DEBUG("Sending plane data along to job %d %d 0x%x\n", msg.param1, msg.param2, msg.param3);
    SYMsgSend( dest, &msg ); 
}

