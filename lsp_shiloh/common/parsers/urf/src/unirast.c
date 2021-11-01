/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file unirast.c
 *
 * \brief Print parser for Universal Raster Format, aka airprint
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
#include "urfio.h"
#include "posix_ostools.h"

#include "print_job_types.h"
#include "print_job_api.h"
#include "memAPI.h"
#include "lassert.h"
#include "logger.h"
#ifdef HAVE_JBIGHW
#include "JbigHW.h"
#endif
#include "ColorPipe.h"
#include "ColorPipeTables.h"
#include "PrintJobInfo.h"

#include "cp_strip.h" 
#include "urf.h"

#define DBG_PRFX "urf: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

#include <stdio.h>
#include <string.h>
#define DBG_VERBOSE_ON
#include "net_debug.h"

#include "debug.h" // hex_dump

// debug tools to test array of pixels input to color converter
#define DBG_PROCESS_PIXELS_SUPPORTED 0
static bool PROCESS_PIXELS_SUPPORTED = true; // DBG_PROCESS_PIXELS_SUPPORTED;

// print limiter
static mlimiter_t *mlimiter = 0;

PROFILE_RENDERING( static uint32_t io_time = 0 );
/// multiple of 8 will help with jpeg later.
static const uint32_t strip_height = 64; // 128 kernel alloc issues; // 128,64 :ok, 16:fails on video latency

#ifdef HAVE_LOW_MEMORY_PRINT
static uint8_t *compressed_strip_buffer = 0;
#endif

/* 600, 300, 150, 75dpi ==>  1, 2, 4, 8 returns 0 on bad input resolution. */ 
#define scale_from_resolution( resolution ) ((resolution) == 300 ? 2 : (resolution) == 150 ? 4 : (resolution) == 75 ? 8 : (resolution) == 600 ? 1 : 0) 

/// notify from strip done to page constructor, 
/// 
bool urf_last_strip_sent = false;


void urf_parser_init( void );

mqd_t unirast_parse_px_queue;
#define NUM_MESSAGES 10
pthread_t unirast_px_thread;

#define STACK_SIZE POSIX_MIN_STACK_SIZE // 6kB stack req'd for wireless and WSD
uint8_t  unirast_stack[STACK_SIZE] ALIGN8;

void* urf_parser_thread( void* unused );
#define MAXURFPARSERS 5
static urf_parser_t * parser_list[MAXURFPARSERS];
static int32_t urf_do_parsing(IOReg *Pipe, MODULE_ID Destination);
extern urf_parser_t * internal_urf_parser_init(void);
extern urf_parser_t * uni1_parser_init(void);
extern urf_parser_t * cups2_parser_init(void);
extern urf_parser_t * cups3_parser_init(void);
extern urf_parser_t * jpeg_parser_init(void);
 
void urf_parser_init( void )
{
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("urf:Starting URF parser\n"));
    DBG_PRINTF_NOTICE("urf:Starting URF parser\n");
    int parser_num;
    for (parser_num = 0; parser_num < MAXURFPARSERS;parser_num++) 
    {
        parser_list[parser_num] = NULL;
    }

    //Gets current supported parsers
    parser_num = 0;
#ifdef HAVE_PARSER_INTERNAL_URF
    DBG_PRINTF_NOTICE("urf:Adding Internal URF Parser\n");
    parser_list[parser_num++] = internal_urf_parser_init();
#endif
#ifdef HAVE_PARSER_UNI10
    DBG_PRINTF_NOTICE("urf:Adding Unirast Parser\n");
    parser_list[parser_num++] = uni1_parser_init();
#endif
#ifdef HAVE_PARSER_CUPS20
    DBG_PRINTF_NOTICE("urf:Adding CUPS2.0 Parser\n");
    parser_list[parser_num++] = cups2_parser_init();
#endif
#ifdef HAVE_PARSER_CUPS30
    DBG_PRINTF_NOTICE("urf:Adding CUPS3.0 Parser\n");
    parser_list[parser_num++] = cups3_parser_init();
#endif
#ifdef HAVE_PARSER_JPEG
    DBG_PRINTF_NOTICE("urf:Adding JPEG Parser\n");
    parser_list[parser_num++] = jpeg_parser_init();
#endif
    ASSERT(parser_num <= MAXURFPARSERS);

    //Registers all the parsers with Connection manager
    for (parser_num = 0; parser_num < MAXURFPARSERS;parser_num++) 
    {
        if (parser_list[parser_num] != NULL) 
        {
            DBG_PRINTF_NOTICE("urf:Registering Parser %d %x\n",parser_num, parser_list[parser_num]->parser_register);
            (*parser_list[parser_num]->parser_register)();
        }
    }

    logString("URF: Init\n");

    posix_create_message_queue( &unirast_parse_px_queue, "/unirast_parse_px_queue", 
                                NUM_MESSAGES, sizeof(MESSAGE) );

    router_register_queue(UNIRAST_MODULE_ID, unirast_parse_px_queue);  // Register a message queue.
    rm_register(e_UnirastParser, UNIRAST_MODULE_ID);

    posix_create_thread( &unirast_px_thread, urf_parser_thread, (void *)0, "unirast_parser", 
                       unirast_stack, STACK_SIZE, POSIX_THR_PRI_NORMAL);
}

void* urf_parser_thread( void* unused )
{
    MESSAGE in_message;
    MESSAGE out_message;
    CURRENT_RESOURCE *OurResource = 0;
    IOReg *Pipe = 0;
    bool parsing = false;

                
    SysWaitForInit();

    //color_pipe_init needs to be called after SysWaitForInit
    extern void color_pipe_init(void);
    color_pipe_init();

    if (mlimiter == 0)
    {

        mlimiter = mlimiter_by_name("print");
        DBG_PRINTF_NOTICE("urf:mlimiter %x,%d\n",mlimiter,mlimiter->active);
    }


    while (1)
    {
        posix_wait_for_message (unirast_parse_px_queue, (char *)&in_message, 
                                sizeof(MESSAGE), POSIX_WAIT_FOREVER);  

        switch (in_message.msgType)
        {
        case MSG_START_PARSE:
        {
            STARTRECIPE *StartIt;
            DBG_PRINTF_NOTICE("unirast start parse\n");
            logString("URF: start parse\n");
            StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
            DBG_ASSERT(StartIt);
            // we have a connection, get the resources and start.
            //
            Pipe = (IOReg *) in_message.param3;        // save the pipe info.
            out_message.msgType = MSG_STARTRECIPE;
            StartIt->Job = ejob_PrintIO;
            StartIt->AdditionalResource = e_UnirastParser;
            StartIt->Pipe = Pipe;
            StartIt->Wait = e_WaitForever;
            out_message.param3 = StartIt;       // freed by sjm
            
            SYMsgSend(SJMID, &out_message);         // send the message.
            parsing = TRUE;
        }
        break;
        
        case MSG_RESOURCES:
            DBG_PRINTF_NOTICE("urf: resources\n");
            if ( parsing )
            {
                OurResource = (CURRENT_RESOURCE *) in_message.param3;
                DBG_PRINTF_NOTICE("urf: Resource %p to %d\n", 
                                  OurResource, OurResource->Destination);
            }
            else
            {
                out_message.msgType = MSG_FREERECIPE;
                SYMsgSend( SJMID, &out_message );
            }
            break;


        case MSG_ACKRECIPE:
        {
            int32_t parser_return_val;

            DBG_PRINTF_NOTICE("unirast ackrecipe\n");
            // Update status to indicate that a print job is being received.
            // This is necessary to cause the "processing" status message
            // to be displayed.
            //statusmgr_post_status_event(UNIRAST_MODULE_ID, STATUS_INFO_PRINTING);

            // We have the resources, start the job.
            mlimiter_start( mlimiter );

            parser_return_val = urf_do_parsing(Pipe, OurResource->Destination);

            mlimiter_stop( mlimiter);

            DBG_PRINTF_NOTICE("Parse Done: free recipe: %#x, pipe: %#x\n", OurResource, Pipe);

            // finished parsing, free the resources.
            parsing = false;
            out_message.msgType = MSG_FREERECIPE;
            out_message.param1 = 0; // in_message.param1;
            out_message.param2 = 0; // in_message.param2;
            out_message.param3 = OurResource;
            SYMsgSend(SJMID, &out_message);
            OurResource = 0;

            // Tell the connection manager the results of the parse.

            out_message.msgType = MSG_END_PARSE;
            out_message.param1 = parser_return_val;
            out_message.param2 = 0;
            out_message.param3 = Pipe;          // The pipe that we are done parsing.
            SYMsgSend(CMPARSER, &out_message);      // send the response.
            Pipe = 0;

            DBG_PRINTF_NOTICE("urf: Done Parsing\n");
        }
        break;    

        case MSG_CANCELJOB:
        {
		/// lsptodo: fix cancel job to correctly eat only its own data
		/// design: seperate thread for parsing and for message with abort flag and
		/// parse to end of data stream.

            DBG_PRINTF_NOTICE("urf: got MSG_CANCELJOB\n");
/*
            DBG_PRINTF_NOTICE("got MSG_CANCELJOB param1=%d=%s param2=%d=%s param3=0x%08X\n",
                              in_message.param1, ToString_DOACTION((DOACTION)in_message.param1),
                              in_message.param2, ToString_MODULE_ID((MODULE_ID)in_message.param2),
                              in_message.param3 ));
*/
            if ( in_message.param1 == SYS_START )
            {
                parsing = false;

                if ( OurResource )
                {
                    out_message.msgType = MSG_FREERECIPE;
                    out_message.param1 = 0;
                    out_message.param2 = 0;
                    out_message.param3 = OurResource;
                    SYMsgSend( SJMID, &out_message );
                    OurResource = 0;
                }

                if ( Pipe )
                {
                    out_message.msgType = MSG_END_PARSE;
                    out_message.param1 = (UINT32)PARSER_ERROR;
                    out_message.param2 = 0;
                    out_message.param3 = Pipe;
                    SYMsgSend( CMPARSER, &out_message );
                    Pipe = 0;
                }
                mlimiter_stop( mlimiter );
                out_message.msgType = MSG_CANCELJOB;
                out_message.param1 = SYS_ACK;
                out_message.param2 = UNIRAST_MODULE_ID;
                out_message.param3 = in_message.param3;
                SYMsgSend( SJMID, &out_message ); // param3 preserved.
            }
            // else // Ignore SYS_ACK coming back to us.
        }
        break;

        default:
            DBG_PRINTF_ERR("unknown message ignored %d\n", in_message.msgType);
            break;
        } // end switch
    }
    return 0;
}

/**
 * @brief this sniffs the stream for a UEL and complains if if
 *        finds one, otherwise it goes on like nothing ever
 *        happened.
 * @param bstream
 * 
 * @return int
 */
static int urf_check_UEL(urf_bstream_t *bstream)
{
    uint8_t *data_ptr;
    const char UEL[] = "\x1b%-12345X";

#define LEN_UEL         (sizeof(UEL)-1)
    if (bstream->have >= LEN_UEL) 
    {
        if(strncmp((char *)bstream->cur,UEL,LEN_UEL) == 0)
        {
            DPRINTF(DBG_LOUD|DBG_OUTPUT, ("urf:UEL found\n"));
            return 1;
        }
    }
    if (LEN_UEL != urf_bstream_read( bstream, &data_ptr, LEN_UEL ))//returned a -1
    {
        DPRINTF(DBG_LOUD|DBG_OUTPUT, ("urf:EOF found\n"));
        return -1;
    }
    if(strncmp((char *)data_ptr,UEL,LEN_UEL) == 0) //does it match?
    {
            bstream->cur -= LEN_UEL;
            bstream->have += LEN_UEL;
            DPRINTF(DBG_LOUD|DBG_OUTPUT, ("urf:UEL found\n"));
            return 1; //yes, rewind stream
    }
    bstream->cur -= LEN_UEL;
    bstream->have += LEN_UEL;
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("urf:No UEL found\n"));
    return 0; //no, rewind stream
}

// eat all the data on the pipe until it generates EOF 
//
// static 
bool urf_parser_poll_for_cancel( urf_bstream_t *bstr )
{
    MESSAGE in_message;
    uint32_t error;
    
    /// caveat emptor: message wait zero timeout
    /// will create LOTS of timers for linux to cleanup 50x hit in performance.
    error = posix_wait_for_message(unirast_parse_px_queue, (char *)&in_message, sizeof(MESSAGE),0 );  
    if ( error == 0 )
    {
        uint8_t *ptr;
        DBG_PRINTF_NOTICE("urf: Got message %d during parse. Will ack now!\n",in_message.msgType);

        if( in_message.msgType == MSG_CANCELJOB )
        {
            while ( 0 < urf_bstream_read( bstr, &ptr, STREAM_BUFFER_SIZE ) );

            SYMsgSend( UNIRAST_MODULE_ID, &in_message );

        }
        return true;
    }
    return false;
}


static void urf_endpage_cb( cp_strip_t *strip )
{
    // stefan revisit change to semaphore
    urf_last_strip_sent = true;
}


/// cp_strip_t gets constructed with different values depending on the parser.
/// 
static cp_strip_t * alloc_rawstrips( cp_strip_t *strip, MODULE_ID dest, urf_page_t *page, print_page_t *p_page )
{
    uint32_t ppl;

    if (strip == 0)
    {
        strip = (cp_strip_t *)MEM_MALLOC_LIMITED(mlimiter, sizeof( cp_strip_t ));
        REL_ASSERT(strip);
        memset(strip, 0, sizeof( cp_strip_t ));
        strip->inuse = false;
        strip->cpo = (cmyk_byte_array_t *)MEM_MALLOC_LIMITED( mlimiter, sizeof(cmyk_byte_array_t) ); 
        REL_ASSERT(strip->cpo);
    }
    while (strip->inuse)
    {
        DBG_PRINTF_ERR("0waiting for strip to be finished\n");
    }

    //ASSERT(mlimiter); Having a mlimiter of NULL (sytem heap ) is valid on certain platforms so I'm removing this check. PAW.
    strip->mlimiter = mlimiter;
    strip->page = page;
    strip->p_page = p_page;
    strip->dest = dest;
    strip->endpage = false;

    
#ifdef ZX_STREAM_PAGE
#define ZX_BUFFER_SIZE 65535
    ppl = page->width;

    //ASSERT(ppl % 64 == 0 );   
    strip->pitch = ppl  ; // (ppl / (8bits_per_byte * 2 bits_per_pixel) modulo 16_byte_align ;
    strip->lines = strip_height;
    strip->actual_lines = 0;
    strip->cols = strip->pitch ; 
    strip->page_height = page->height;
    strip->res = page->resolution;
    strip->rawsize = ZX_BUFFER_SIZE;//64k
    strip->num_output_colors = 1;
#elif defined(LASER_CMYK_STRIP_PAGE)
    ppl = page->width;
    ppl *= scale_from_resolution(page->resolution);

    ASSERT(ppl % 64 == 0 );   // actually 128bit alignment with 2bit per pixel
    strip->pitch = ppl >> 2 ; // (ppl / (8bits_per_byte * 2 bits_per_pixel) modulo 16_byte_align ;
    strip->lines = strip_height;
    strip->actual_lines = 0;
    strip->cols = strip->pitch * 8; // ppl * 2; // times 2 bits per pixel * num output pixels
    strip->page_height = page->height;
    strip->res = 600; // page->resolution;
    strip->rawsize = strip->lines * strip->pitch;
    strip->num_output_colors = (page->colorSpace == 0 || cp_mono_only()) ? 1 : 4; // k or cmyk = 
#elif defined(LASER_RGB_STRIP_PAGE)
    ppl = page->width;
    //ppl *= scale_from_resolution(page->resolution);

    //ASSERT(ppl % 64 == 0 );   // actually 128bit alignment with 2bit per pixel
    strip->num_output_colors = (page->colorSpace == 0 || cp_mono_only()) ? 1 : 4; // k or rgb =
    strip->num_input_colors = (page->colorSpace == 0) ? 1 : 3; // k or rgb =
    strip->pitch = ppl * strip->num_input_colors; 
    strip->lines = strip_height;
    strip->actual_lines = 0;
    strip->cols = strip->pitch;
    strip->page_height = page->height;
    strip->res = page->resolution; // page->resolution;
    strip->rawsize = strip->lines * strip->pitch;
#else
#error
#endif
    

#ifdef HAVE_LOW_MEMORY_PRINT
    /// use memcpy from a page time scoped common compression buffer into strip time compressed buffer.
    if (compressed_strip_buffer)
        strip->static_compression_buffer = compressed_strip_buffer;
    else
        strip->static_compression_buffer = compressed_strip_buffer =
            (uint8_t *)MEM_MALLOC_ALIGN_LIMITED( strip->mlimiter, strip->rawsize, e_32_byte );
#else
    /// use realloc to shrink compressed buffer in place.
    strip->static_compression_buffer = 0;
#endif

    strip->end_page_cb = urf_endpage_cb;

    strip->cpo->pK = (uint8_t *)MEM_MALLOC_ALIGN_LIMITED( mlimiter,
                                                      strip->rawsize + DBG_EXTRA, 
                                                      e_32_byte );
    ASSERT(strip->cpo->pK);
    if (DBG_EXTRA) 
        memset(strip->cpo->pK + strip->rawsize, 0xa5, DBG_EXTRA);
#if defined(LASER_RGB_STRIP_PAGE)
    strip->cpo->pC = strip->cpo->pM = strip->cpo->pY = 0;
#else
    if (strip->num_output_colors == 4)
    {
        strip->cpo->pC = (uint8_t *)MEM_MALLOC_ALIGN_LIMITED( mlimiter,
                                                          strip->rawsize + DBG_EXTRA, 
                                                          e_32_byte );
        REL_ASSERT(strip->cpo->pC);
        if (DBG_EXTRA) 
            memset(strip->cpo->pC + strip->rawsize, 0xa5, DBG_EXTRA);
        strip->cpo->pM = (uint8_t *)MEM_MALLOC_ALIGN_LIMITED( mlimiter,
                                                          strip->rawsize + DBG_EXTRA, 
                                                          e_32_byte );
        REL_ASSERT(strip->cpo->pM);
        if (DBG_EXTRA) 
            memset(strip->cpo->pM + strip->rawsize, 0xa5, DBG_EXTRA);
        strip->cpo->pY = (uint8_t *)MEM_MALLOC_ALIGN_LIMITED( mlimiter,
                                                          strip->rawsize + DBG_EXTRA, 
                                                          e_32_byte );
        REL_ASSERT(strip->cpo->pY);
        if (DBG_EXTRA) 
            memset(strip->cpo->pY + strip->rawsize, 0xa5, DBG_EXTRA);
    }
    else
    {
        strip->cpo->pC = strip->cpo->pM = strip->cpo->pY = 0;
    }
#endif

    return strip;
}
#ifdef ZX_STREAM_PAGE
#define MAX_WIDTH_IN_PIXELS 5100
#define BUFFER_MOD 4
#define Y_MARGIN_CLIP 0
#define BITS_PER_PIXEL 8

#elif defined(LASER_CMYK_STRIP_PAGE)  /* software halftone with parameters defined here. */ 
#define MAX_WIDTH_IN_PIXELS 4928
#define BUFFER_MOD 64 /* width in pixels must be modulo 64, because its 2 bit so 128bit aligned. */
#define Y_MARGIN_CLIP 100 /* 1/6" top & bottom margin */
#define BITS_PER_PIXEL 2

#elif defined(LASER_RGB_STRIP_PAGE) /* hardware halftone project feature: HAVE_LASER_HW_STRIP_COLOR_PIPE */
#define MAX_WIDTH_IN_PIXELS 4992  // Pick a value providing roughly 100 pixel margin and even multiple of BUFFER_MOD
#define BUFFER_MOD 128   // Width in pixels must be even multiple of 32 bits for video DMA. */
#define Y_MARGIN_CLIP 100 /* 1/6" top & bottom margin */
#define BITS_PER_PIXEL 8

#else
#error  /* need to add zxcreek output */
#endif
static color_pipe_t *cp = 0;  // singleton "fetched" at page time closed at end of job

/// modifies width for best fit with video.
/// returns scale factor or 0 on error
static uint32_t urf_papersize_adjust( urf_page_t *urf_page, uint32_t *x_offset )
{
    uint32_t scale = 0;
    uint32_t width_max = 0;

    // Get the scale factor for sub 600 dpi resolutions
    scale = scale_from_resolution(urf_page->resolution);

    if (scale) 
    {
        width_max = MAX_WIDTH_IN_PIXELS / scale ;  // approx 100 pixel margin

        // set width to data width modulo BUFFER_MOD and offset to zero let video center
        width_max = urf_page->width / BUFFER_MOD;
        width_max *= BUFFER_MOD;
        
        *x_offset = (urf_page->width - width_max) / 2 ;

        // dbg_printf("%s scale %d, height %d, width %d\n", __func__, scale, urf_page->height, urf_page->width);

        urf_page->urf_parse_x = urf_page->width;
        urf_page->urf_parse_y = urf_page->height;
 
        if ( urf_page->doc_type != e_CALPAGE && !urf_page->this_is_jpeg_parser)
        {
            urf_page->height -= 2 * Y_MARGIN_CLIP / scale; 
            urf_page->curr_y_offset = Y_MARGIN_CLIP / scale;
        }
        else
        {
            urf_page->curr_y_offset = 0;
        }

        // dbg_printf("%s scale %d, height %d, width %d\n", __func__, scale, urf_page->height, urf_page->width);


        urf_page->width = width_max;
    }
    
    #if defined(LASER_RGB_STRIP_PAGE)
    // On color products the pipp pipe requires a multiple of 8? for the page height
    if ( ( urf_page->colorSpace == 1 ) &&
         ( urf_page->height % 8 != 0 ) )
    {
        DBG_PRINTF_NOTICE("Adjusting height from %d to %d\n", 
                        urf_page->height, urf_page->height - ( urf_page->height % 8 ) );
        urf_page->height -= urf_page->height % 8;
    }
    #endif // LASER_RGB_STRIP_PAGE

    DBG_PRINTF_NOTICE("URF Resolution %d page x %d:%d y %d:%d, scale = %d, x_offset %d\n", 
                      urf_page->resolution, urf_page->width, urf_page->urf_parse_x, urf_page->height, urf_page->urf_parse_y, scale, *x_offset);

#if defined (ZX_STREAM_PAGE) || defined(LASER_RGB_STRIP_PAGE)
    return 1;
#elif defined(LASER_CMYK_STRIP_PAGE)
    return scale;
#else
#error
#endif
}

/**
 * @brief generic pixbit decompressor
 * @param bstream[in]: input bstream
 * @param urf_page[in]: urf page data
 * @param p_page[in]: print page data
 * @param parser_128_method[in]: function for handling opcode
 *                         0x80
 * 
 * @return int status 0 good: -1 error
 */
int urf_parser_decompress(urf_bstream_t *bstream, 
                          urf_page_t *urf_page, 
                          print_page_t* p_page, 
                          bool (*parser_128_method) (color_pipe_t *cp, object_tag_t * object_tag, bool printable))
{
    uint8_t *pixel_ptr;
    uint8_t *byte_ptr;
    uint32_t bytes_per_pixel = urf_page->colorSpace == 0 ? 1 : 3;
    uint32_t lines;
    uint8_t cnt;
    uint32_t r;
    uint32_t i;
    int32_t bytes_read;
    uint32_t row_repeat;
    uint32_t repeat;
    object_tag_t object_tag; //  = { NULL, 0 }; // no object tags process black.
    uint32_t strip_lines = 0;
    uint32_t line_cnt = 0;
#if defined (ZX_STREAM_PAGE) || defined(LASER_RGB_STRIP_PAGE)
    uint32_t scale = 1;
#elif defined(LASER_CMYK_STRIP_PAGE)
    uint32_t scale = scale_from_resolution(urf_page->resolution);
#else
#error
#endif
    uint32_t curr_x_offset;
    
    PROFILE_RENDERING( uint32_t begin_time );
    PROFILE_RENDERING( uint32_t end_time );
    const uint8_t white[3] = {0xff, 0xff, 0xff};

    object_tag.line = 0; // no object tag ever for urf.
    object_tag.Pblack = (urf_page->UnderColorRemoval )? P_BLACK : K_ONLY; 
    bool is_short_page = false;
    strip_lines = 0;
    line_cnt = 0;
    r = 0;
    PROFILE_RENDERING( begin_time = (uint32_t)posix_gettime_ticks();  io_time = 0 );

    for ( lines = 0; lines < urf_page->urf_parse_y; )
    {

        bytes_read = urf_bstream_read( bstream, &byte_ptr, 1 );
        if ( bytes_read != 1 ) // || urf_parser_poll_for_cancel(bstream)  ) // lsptodo fix cancel/abort.
        {
            row_repeat = urf_page->urf_parse_y - lines;
            DBG_PRINTF_NOTICE("urf: Short input on line boundary recovering lines %d repeat %d\n", lines, row_repeat);
            is_short_page = true;
            color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
            goto eol_loop; // to fill rest of page.
        }
        curr_x_offset = urf_page->x_offset;
	UNUSED_VAR( curr_x_offset );
        row_repeat = *byte_ptr + 1;
        for ( r = 0; r < urf_page->urf_parse_x; ) // && row_repeat >= 0; )
        {
            bytes_read = urf_bstream_read( bstream, &byte_ptr, 1 );
            if (bytes_read != 1)
            {
                DBG_PRINTF_NOTICE("bytes_read != 1 inside of row\n");
                row_repeat = urf_page->urf_parse_y - lines;
                is_short_page = true;
                color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                goto eol_loop; // to fill rest of page.
            }
            cnt = *byte_ptr;
            if (cnt == 0x80)  // rest of line w/fill
            {
                if (parser_128_method) 
                {
                    if ((*parser_128_method)(cp, &object_tag,(r < urf_page->width + urf_page->x_offset)))
                    { 
                        break;
                    }          
                }     
            }
            else if (cnt & 0x80) // copy next 'n' bytes
            {
                repeat = 257 - cnt; //  (-(int)code)+1;
                if ( PROCESS_PIXELS_SUPPORTED && r > urf_page->x_offset && r + repeat < (urf_page->width + urf_page->x_offset))
                {
                    if ( DBG_PROCESS_PIXELS_SUPPORTED && r > urf_page->width )
                    {
                        DBG_PRINTF_NOTICE("-repeat %d  r %d r-off %d: %d\n", repeat, r, r - urf_page->x_offset, urf_page->urf_parse_x);
                    }
                    bytes_read = urf_bstream_read( bstream, &pixel_ptr, repeat * bytes_per_pixel);
                    if (bytes_read == repeat * bytes_per_pixel)
                    {
                        color_pipe_process_multiple_pixels( cp, repeat, &object_tag, pixel_ptr );
                        r += repeat;
                    }
                    else
                    {
                        DBG_PRINTF_NOTICE("(uint32_t)bytes_read(%d) != %d ( repeat * bytes_per_pixel(%dx%d) )\n",
                                          bytes_read, repeat * bytes_per_pixel, repeat, bytes_per_pixel);
                        hex_dump_named((void *)pixel_ptr, (int)bytes_read, (char*)"urf decompress");

                        row_repeat = urf_page->urf_parse_y - lines;
                        is_short_page = true;
                        color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                        goto eol_loop; // to fill rest of page.
                    }
                }
                else 
                {
                    for ( i = 0; i < repeat; i++ )
                    {   
                        ++r;
                        bytes_read = urf_bstream_read( bstream, &pixel_ptr, bytes_per_pixel);
                        if ((uint32_t)bytes_read != bytes_per_pixel)
                        {
                            DBG_PRINTF_NOTICE("(uint32_t)bytes_read(%d) != bytes_per_pixel(%d)\n", bytes_read, bytes_per_pixel);
                            row_repeat = urf_page->urf_parse_y - lines;
                            is_short_page = true;
                            color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                            goto eol_loop; // to fill rest of page.
                        }
                        if (r > urf_page->x_offset && r < (urf_page->width - urf_page->x_offset)) //truncate over width
                        {
                            color_pipe_process_repeated_pixels( cp, 1, &object_tag, pixel_ptr );
                        }
                    }
                }
            }
            else // zero or positive repeat pixel n times   
            {
                uint32_t tmp_repeat;
                bytes_read = urf_bstream_read( bstream, &pixel_ptr, bytes_per_pixel);
                if ((uint32_t)bytes_read != bytes_per_pixel)
                {
                    DBG_PRINTF_NOTICE("(uint32_t)bytes_read(%d) != bytes_per_pixel(%d)\n",
                                      bytes_read, bytes_per_pixel);
                    row_repeat = urf_page->urf_parse_y - lines;
                    is_short_page = true;
                    color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                    goto eol_loop; // to fill rest of page.
                }  
    
                repeat = (uint32_t)cnt + 1;
                // DBG_PRINTF_NOTICE("+repeat %d r %d : %d \n", repeat, r, urf_parse_x);
                if (r < urf_page->x_offset) // skip left margin
                {
                    if ( r + repeat > urf_page->x_offset )
                    {
                        // skip some process print rest.
                        tmp_repeat = r + repeat - urf_page->x_offset;
                        // DBG_PRINTF_NOTICE("+repeat %d r %d : %d \n", repeat, r, tmp_repeat);
                        color_pipe_process_repeated_pixels( cp, tmp_repeat, &object_tag, pixel_ptr );
                    }
                    // else skip repeat count pixelsp 
                }
                else if (r < (urf_page->width + urf_page->x_offset)) //output middle of page, skip right margin
                {
                    tmp_repeat = r + repeat < urf_page->width + urf_page->x_offset ? 
                        repeat : 
                        repeat - ((r + repeat) - (urf_page->width + urf_page->x_offset ));
                    // DBG_PRINTF_NOTICE("++repeat %d r %d : %d \n", repeat, r, tmp_repeat);
                    color_pipe_process_repeated_pixels( cp, tmp_repeat, &object_tag, pixel_ptr );
                }   
                r += repeat;    
            }
        }  // end for width
    eol_loop:
        do // process an input line
        {
            bool eol_res;
            uint32_t line_dup = 0;

            for (line_dup = 0; line_dup < scale; ++line_dup )
            {
                eol_res = color_pipe_eol(cp, ((urf_page->curr_y_offset == 0) && (line_cnt < urf_page->height * scale))); // strip is full indication on return false
                if (urf_page->curr_y_offset > 0) 
                {
                    --urf_page->curr_y_offset; //munch top margin
                }
                else
                {
                    ++strip_lines;  // output lines per strip
                    ++line_cnt;
                }

                if (is_short_page)
                {
                    color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                    is_short_page = false;
                }

            }
            
            ++lines;         // input lines per page
            if (!eol_res || is_short_page) 
            {
                urf_page->strip = urf_page->strip_1->inuse ? urf_page->strip_2 : urf_page->strip_1;
                if ( urf_page->strip->inuse == false )
                {
                    urf_page->strip->inuse = true; 
                    color_pipe_new_halftone_strip( cp, urf_page->strip->cpo, cp_strip_done_cb, (void*)urf_page->strip );
                }
                else
                {
                    DBG_PRINTF_ERR("eol with 2 active strips \n");
                }

                // dbg_printf("Adj height %d line_cnt %d\n", (urf_page->height * scale) , line_cnt);
                if (((urf_page->height * scale) - line_cnt) <= strip_height || is_short_page )
                {
                    urf_page->strip->endpage = true;
                    urf_page->strip->actual_lines = (urf_page->height * scale) - line_cnt ;
                    
                    DBG_PRINTF_NOTICE("urf: MARKING FINAL STRIP lines = %d, height = %d remaining %d\n", 
                                      lines, urf_page->height,  urf_page->strip->actual_lines);
                }
                else
                {
                    urf_page->strip->actual_lines = urf_page->strip->lines; 
                    urf_page->strip->endpage = false;
                }
                strip_lines = 0;
            }

            if (urf_page->spew)
            {
                DBG_PRINTF_NOTICE("lines %d c %d ++ %d h %d endpage %d  lines:%d actual %d\n", 
                                   lines, r, row_repeat, urf_page->height, urf_page->strip->endpage, urf_page->strip->actual_lines, strip_lines);
            }
        } while ( --row_repeat > 0 );
    }
    DBG_PRINTF_NOTICE("urf: Final line count: %d\n",lines);
    color_pipe_eop(cp);
    PROFILE_RENDERING( 
        end_time = (uint32_t)posix_gettime_ticks();
        DBG_PRINTF_NOTICE("urf: Rip time = %d, io_time = %d\n", (end_time - begin_time) * 10, io_time * 10)
        );
   
    return is_short_page ? -1:0;
    
}
/**
 * @brief generic pixbit decompressor
 * @param bstream[in]: input bstream
 * @param urf_page[in]: urf page data
 * @param p_page[in]: print page data
 * @param parser_128_method[in]: function for handling opcode
 *                         0x80
 * 
 * @return int status 0 good: -1 error
 */
int urf_parser_uncompressed_data(urf_bstream_t *bstream, 
                                 urf_page_t *urf_page, 
                                 print_page_t* p_page, 
                                 bool (*parser_128_method) (color_pipe_t *cp, object_tag_t * object_tag, bool printable))
{
    uint8_t *pixel_ptr;
    uint32_t bytes_per_pixel = urf_page->colorSpace == 0 ? 1 : 3;
    uint32_t lines;
    int32_t bytes_read;
    uint32_t row_repeat;
    object_tag_t object_tag; //  = { NULL, 0 }; // no object tags process black.
    uint32_t strip_lines = 0;
    uint32_t line_cnt = 0;
    const uint32_t bytes_per_x_offset = urf_page->x_offset * bytes_per_pixel;
    const uint32_t max_pixels_per_buffer = STREAM_BUFFER_SIZE / bytes_per_pixel;
    const uint32_t max_bytes_per_buffer = max_pixels_per_buffer * bytes_per_pixel;
    const uint32_t max_print_pixels_per_line = urf_page->width ;
    const uint32_t max_print_bytes_per_line = max_print_pixels_per_line * bytes_per_pixel;
    const int32_t trailing_bytes = (urf_page->urf_parse_x - (max_print_pixels_per_line + urf_page->x_offset)) * bytes_per_pixel;
    uint32_t pixels_remaining;
    uint32_t bytes_remaining;
    uint32_t bytes_to_read;
    uint32_t pixels_to_read;
    
    ASSERT(bytes_per_x_offset <= max_bytes_per_buffer);
#if defined (ZX_STREAM_PAGE) || defined(LASER_RGB_STRIP_PAGE)
    uint32_t scale = 1;
#elif defined(LASER_CMYK_STRIP_PAGE)
    uint32_t scale = scale_from_resolution(urf_page->resolution);
#else
#error
#endif

    PROFILE_RENDERING( uint32_t begin_time );
    PROFILE_RENDERING( uint32_t end_time );
    const uint8_t white[3] = {0xff, 0xff, 0xff};

    object_tag.line = 0; // no object tag ever for urf.
    object_tag.Pblack = (urf_page->UnderColorRemoval )? P_BLACK : K_ONLY; 
    bool is_short_page = false;
    strip_lines = 0;
    line_cnt = 0;
    PROFILE_RENDERING( begin_time = (uint32_t)posix_gettime_ticks();  io_time = 0 );
    for ( lines = 0; lines < urf_page->urf_parse_y; )
    {
        row_repeat = 1;
        //DBG_PRINTF_NOTICE("urf: Line %d width %d\n", lines, urf_page->urf_parse_x );
        //read xoffset
        //DBG_PRINTF_NOTICE("urf: left Munching %d %d pixels\n",urf_page->x_offset,bytes_per_x_offset  );
        bytes_read = urf_bstream_read( bstream, &pixel_ptr, bytes_per_x_offset );
        if (bytes_per_x_offset > 0) 
        {
            if (bytes_read != bytes_per_x_offset)
            {
                DBG_PRINTF_NOTICE("bytes_read != %d inside of row\n", bytes_per_pixel);
                row_repeat = urf_page->urf_parse_y - lines;
                is_short_page = true;
                color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                goto eol_loop; // to fill rest of page.    
            }
        }
        pixels_remaining = max_print_pixels_per_line;
        bytes_remaining = max_print_bytes_per_line;
        while ( pixels_remaining > 0 ) // && row_repeat >= 0; )
        {
            if (pixels_remaining > max_pixels_per_buffer) 
            {
                bytes_to_read = max_bytes_per_buffer;
                pixels_to_read = max_pixels_per_buffer;
            }
            else
            {
                bytes_to_read = bytes_remaining;
                pixels_to_read = pixels_remaining;
            }
            //DBG_PRINTF_NOTICE("urf: Reading %d %d pixels\n",pixels_to_read,bytes_to_read  );
            bytes_read = urf_bstream_read( bstream, &pixel_ptr, bytes_to_read );
            if (bytes_read != bytes_to_read)
            {
                DBG_PRINTF_NOTICE("bytes_read != %d inside of row\n", bytes_per_pixel);
                row_repeat = urf_page->urf_parse_y - lines;
                is_short_page = true;
                color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                goto eol_loop; // to fill rest of page.
                
            }

            color_pipe_process_multiple_pixels( cp, pixels_to_read, &object_tag, pixel_ptr );
            pixels_remaining -= pixels_to_read;
            bytes_remaining -= bytes_to_read;   
        }  // end for width
        //read xoffset
        if (trailing_bytes > 0) 
        {
            //DBG_PRINTF_NOTICE("urf: right Munching  %d trailing_bytes\n",trailing_bytes  );
            bytes_read = urf_bstream_read( bstream, &pixel_ptr, trailing_bytes );
            if (bytes_read != trailing_bytes)
            {
                DBG_PRINTF_NOTICE("bytes_read != %d inside of row\n", bytes_per_pixel);
                row_repeat = urf_page->urf_parse_y - lines;
                is_short_page = true;
                color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                goto eol_loop; // to fill rest of page.    
            }
        }
    eol_loop:
        do // process an input line
        {
            bool eol_res;
            uint32_t line_dup = 0;

            for (line_dup = 0; line_dup < scale; ++line_dup )
            {
                //DBG_PRINTF_NOTICE("urf: EOL line %d max lines %d width %d\n",lines,urf_page->urf_parse_y, r );
                eol_res = color_pipe_eol(cp, ((urf_page->curr_y_offset == 0) && (line_cnt < urf_page->height * scale))); // strip is full indication on return false
                if (urf_page->curr_y_offset > 0) 
                {
                    --urf_page->curr_y_offset; //munch top margin
                }
                else
                {
                    ++strip_lines;  // output lines per strip
                    ++line_cnt;
                }

                if (is_short_page)
                {
                    color_pipe_process_repeated_pixels( cp, 1, &object_tag, (uint8_t*) &white[0] ); // const cast
                    is_short_page = false;
                }

            }
            
            ++lines;         // input lines per page
            if (!eol_res || is_short_page) 
            {
                urf_page->strip = urf_page->strip_1->inuse ? urf_page->strip_2 : urf_page->strip_1;
                if ( urf_page->strip->inuse == false )
                {
                    urf_page->strip->inuse = true; 
                    color_pipe_new_halftone_strip( cp, urf_page->strip->cpo, cp_strip_done_cb, (void*)urf_page->strip );
                }
                else
                {
                    DBG_PRINTF_ERR("eol with 2 active strips \n");
                }
                //DBG_PRINTF_NOTICE("Adj height %d line_cnt %d\n", 
                //                      (urf_page->height * scale) , line_cnt);
                if (((urf_page->height * scale) - line_cnt) <= strip_height || is_short_page )
                {
                    urf_page->strip->endpage = true;
                    urf_page->strip->actual_lines = (urf_page->height * scale) - line_cnt ;
                    
                    DBG_PRINTF_NOTICE("urf: 2 MARKING FINAL STRIP lines = %d, height = %d remaining %d\n", 
                                      lines, urf_page->height,  urf_page->strip->actual_lines);
                }
                else
                {
                    urf_page->strip->actual_lines = urf_page->strip->lines; 
                    urf_page->strip->endpage = false;
                }
                strip_lines = 0;
            }

            if (urf_page->spew)
            {
                DBG_PRINTF_NOTICE("lines %d c ++ %d h %d endpage %d  lines:%d actual %d\n", 
                                   lines, row_repeat, urf_page->height, urf_page->strip->endpage, urf_page->strip->actual_lines, strip_lines);
            }
        } while ( --row_repeat > 0 );
    }
    DBG_PRINTF_NOTICE("urf: Final line count: %d\n",lines);
    color_pipe_eop(cp);
    PROFILE_RENDERING( 
        end_time = (uint32_t)posix_gettime_ticks();
        DBG_PRINTF_NOTICE("urf: Rip time = %d, io_time = %d\n", (end_time - begin_time) * 10, io_time * 10)
        );
   
    return is_short_page ? -1:0;
    
}
/**
 * @brief processes a urf page.
 * @param bstream[in] input stream
 * @param dest
 * @param urf_page[in] urf page data
 * @param p_page[in] print page data
 * 
 * @return int status 0 good: -1 error
 */
static int urf_raster_data(urf_bstream_t *bstream, MODULE_ID dest, urf_page_t *urf_page, print_page_t* p_page)
{
    uint32_t ppl = 0;
    uint32_t strip_size;
    uint32_t scale;

    PROFILE_RENDERING( uint32_t begin_time );
    PROFILE_RENDERING( uint32_t end_time );

    bool is_short_page = false;
    urf_page->spew = false;
    urf_page->strip_1 = 0;
    urf_page->strip_2 = 0;
    urf_page->strip = 0; 

    urf_page->strip_1 = alloc_rawstrips(urf_page->strip_1, dest, urf_page, p_page); 
    urf_page->strip_2 = alloc_rawstrips(urf_page->strip_2, dest, urf_page, p_page); 
    urf_page->strip = urf_page->strip_1->inuse ? urf_page->strip_2 : urf_page->strip_1;
    ASSERT( urf_page->strip->inuse == false );
    urf_last_strip_sent = true;
    urf_page->strip->inuse = true; 
    urf_page->strip->endpage = false;
    color_table_set_defaults();
    color_table_set_output_bits_per_pixel( BITS_PER_PIXEL );

    // todo:
    // if ( hw doesn't support color we need to exit here with some grace )

    // no plans to support adobe_RGB since this will almost always be 16bit per component
    color_table_set_input_space( urf_page->colorSpace == 0 ? e_subtractive_gray : e_sRGB );
    // set undercolor removal algorithm forcing process black for gray axis, REVISIT

#ifdef ZX_STREAM_PAGE
    ppl = urf_page->width;
    strip_size = urf_page->strip->rawsize;
    scale = 1;
#elif defined(LASER_CMYK_STRIP_PAGE)
    ppl = urf_page->strip->cols >> 1; // bits per row -> pixels per row, with 2 bit_per_pixel
    strip_size = strip_height;
    scale = scale_from_resolution(urf_page->resolution);
#elif defined(LASER_RGB_STRIP_PAGE) 
    ppl = urf_page->width; // bits per row -> pixels per row, with 2 bit_per_pixel
    strip_size = strip_height;
    scale = 1;
#else
#error //no technology defined
#endif
    cp = color_pipe_page_open( BITS_PER_PIXEL, // 2bit native 
                               ((urf_page->colorSpace == 0) ? e_color_space_1_to_1 : e_color_space_3_to_4), 
                               ppl, // pixels per line
                               urf_page->strip->pitch, // cache_line
                               strip_size, // lines per strip or Zx buffer size
                               scale, // 4,2,1 : 150, 300, 600 (native)
                               e_color_pipe_default);

    color_pipe_new_halftone_strip( cp, urf_page->strip->cpo, cp_strip_done_cb, (void*)urf_page->strip );
#ifdef ZX_STREAM_PAGE
//TO DO WHAT GOES HERE?
    //print_plane_start(p_page, COLOR_BLACK); Zx doesn't do a plane start
#elif defined(LASER_CMYK_STRIP_PAGE)
    print_plane_start(p_page, COLOR_BLACK);
    if (urf_page->colorSpace != 0)
    {
        print_plane_start(p_page, COLOR_YELLOW);
        print_plane_start(p_page, COLOR_MAGENTA);
        print_plane_start(p_page, COLOR_CYAN);
    }
#elif defined(LASER_RGB_STRIP_PAGE)
    print_plane_start(p_page, COLOR_BLACK);
#else
#error //need technology
#endif
    //uncompress and consume page data based on parser.
    if ((*urf_page->curr_parser->parser_decompress)(bstream, urf_page, p_page, urf_page->curr_parser->parser_128_method))
    {
        is_short_page = true;
    }

    // wait for last strip colorspace and halftone completion.
    while( urf_last_strip_sent == false );
    {
        DBG_PRINTF_NOTICE("spin waiting for halftoner completion\n");
    }
#ifdef ZX_STREAM_PAGE //TO DO This isn't the best way
        // DBG_PRINTF_NOTICE( "print_plane_end on %s\n", ToString_COLOR(color));
        print_plane_end(p_page, COLOR_BLACK);
#endif
    if (urf_page->strip) {
        urf_page->strip_1->inuse = false;
        urf_page->strip_2->inuse = false;
    }
    urf_page->strip_1 = cp_free_rawstrips( urf_page->strip_1 );
    urf_page->strip_2 = cp_free_rawstrips( urf_page->strip_2 );

#ifdef HAVE_LOW_MEMORY_PRINT
    MEM_FREE_AND_NULL( compressed_strip_buffer );
#endif

    return is_short_page ? -1 : 0;
}

/**
 * @brief parses parser specific page header
 * @param bstream[in]:input bstream
 * @param urf_page[in/out]: urf page data
 * 
 * @return int status 0 good: -1 error
 */
static int urf_get_header_data(urf_bstream_t *bstream, urf_page_t * urf_page)
{
    //dbg_printf("urf_get_header_data...\n");
    if (urf_page->curr_parser != NULL) 
    {
	//dbg_printf("urf_get_header_data using current parser...\n");
        return (*urf_page->curr_parser->parser_header_read)(bstream, urf_page);
    }
    else
    {
        ASSERT(0);
        return -1;
    }
}
/**
 * @brief gets the parser token from the input stream
 * @param pipe[in]: input pipe
 * @param urf_page[out]: page data
 * 
 * @return int:status 0 good: -1 fail
 */
static int urf_get_job_token(IOReg *pipe, urf_page_t * urf_page)
{
    uint32_t parser_num;

    urf_page->curr_parser = NULL;
    for (parser_num = 0; parser_num < MAXURFPARSERS;parser_num++) 
    {
        if (parser_list[parser_num] != NULL) 
        {
            if ((*parser_list[parser_num]->parser_detect)(pipe, urf_page))
            {
                return -1;
            }
            else if (urf_page->curr_parser != NULL) 
            {
                return 0;
            }
        }
    }
    return -1;
}

/**
 * @brief Read the pipe and discard the data until pipe is empty or UEL found. 
 * This is used to clean the pipe in error conditions to prevent garbage data 
 * from being passed along to other parsers such as PCL. 
 * @param pipe[in]: input pipe
 * 
 * @return void
 */
void urf_flush_pipe_to_uel(IOReg *pipe)
{
    #define BUF_SIZE 512
    int cnt = 0;
    int bytes_read;
    static uint8_t buf[BUF_SIZE];

    DBG_PRINTF_ERR( "%s: START\n", __func__ );
    while (0 < (bytes_read = pipe->ReadConnect(pipe, buf, BUF_SIZE, PARSEREAD_SOME)))
    {
        int i;
        cnt += bytes_read;
        // dprintf(" - ------------ read cnt %d %d\n", cnt, bytes_read); 
        for(i = 0; bytes_read > 8 && i < bytes_read; i++) {
            if (
                buf[i] == 0x1b &&
                buf[i+1] == '%' &&
                buf[i+2] == '-' &&
                buf[i+3] == '1' &&
                buf[i+4] == '2' &&
                buf[i+5] == '3' &&
                buf[i+6] == '4' &&
                buf[i+7] == '5' &&
                buf[i+8] == 'X' 
                ) 
            {
                // Found UEL put data back and exit.
                pipe->RewindData (pipe, buf+i, bytes_read-1);
                DBG_PRINTF_ERR(" - ------------ found uel on parser cancel read %d i %d cnt %d\n", 
                        bytes_read, i, cnt);
                goto DONE;
            }
        }
    }
DONE:
    DBG_PRINTF_ERR( "%s: END\n", __func__ );
}

/**
 * @brief Parsers a urf job.
 * @param pipe[in]: input pipe
 * @param Destination
 * 
 * @return int32_t status 0 good: -1 error
 */
static int32_t urf_do_parsing(IOReg *pipe, MODULE_ID Destination)
{
    uint32_t scale = 1;
    int result = 0;
    urf_page_t urf_page;
    print_job_t *p_job = NULL;

    memset( &urf_page, 0x00, sizeof( urf_page ) );

    //determines which parser needs to be run for job
    if (urf_get_job_token(pipe, &urf_page))
    {
        // todo optimize early exit
        // eat_till_uel(pipe);
        return -1;
    }

#ifdef HAVE_LASER_COLOR_SUPPORT
    DBG_PRINTF_NOTICE( "urf: jobname %s\n", pipe->pjob_info->jobname );
#endif

    urf_bstream_t *bstream = urf_bstream_construct(pipe);
    if ( bstream == NULL )
    {
        return -1;
    }
    
    do 
    {  
        print_page_t *p_page;
        uint32_t ppl;  // compute 16bit aligned width to please the video gods.

        //reads in header data for the page, based on the which parser was choosen.
        //puts it into our urf generic format
        if (urf_check_UEL(bstream )  != 0)
        {
            break;//found a UEL or ran out of data trying
        }

        result = urf_get_header_data(bstream, &urf_page);
        if ( result == 0 )
        {
            if (!p_job)
            {
                // we construct the job after we have received the first page since
                // we take some job settings from the first page's header
                p_job = print_job_construct( mlimiter, pipe );
                if ( urf_page.doc_type != 0 )
                {
                    print_job_set_doc_type( p_job, urf_page.doc_type );
                }
                if (urf_page.duplex_specified)
                {
                    print_job_set_duplex(p_job, urf_page.duplex_mode);
                    print_job_set_duplex_rotate_backside(p_job, urf_page.rotate_backside);
                }
                print_job_start( p_job, Destination );
            }
	    //dbg_printf("-----------h:%d\n", urf_page.height);
            if (!(scale = urf_papersize_adjust( &urf_page, &urf_page.x_offset )))
                break; // error handling 

	    //dbg_printf("-----------h:%d\n", urf_page.height);
            p_page = print_page_construct(p_job);

            // revisit: copies currently set to 0; WTF?       
            // print_page_set_num_copies(p_page, unirast_page.copies == 1 ? 0 : unirast_page.copies);
            
            print_page_set_color_mode(p_page, ((urf_page.colorSpace == 0 || cp_mono_only()) ? e_Mono : e_Color));
            print_page_set_num_input_colors( p_page, urf_page.colorSpace == 0 ? 1 : 3 );
            print_page_set_colorspace( p_page, urf_page.colorSpace == 0 ? e_additive_gray : e_sRGB );
#ifdef ZX_STREAM_PAGE
            print_page_set_dpi_x( p_page, urf_page.resolution );
            print_page_set_dpi_y( p_page, urf_page.resolution );
#elif defined(LASER_CMYK_STRIP_PAGE)
            print_page_set_dpi_x( p_page, 600 );
            print_page_set_dpi_y( p_page, 600 );
#elif defined(LASER_RGB_STRIP_PAGE) 
            print_page_set_dpi_x( p_page, urf_page.resolution );
            print_page_set_dpi_y( p_page, urf_page.resolution );
#else
#error
#endif

            // output is 600 by 2
            ppl = urf_page.width;
            ppl *= scale;  // 1,2,4,8

#if defined(ZX_STREAM_PAGE) || defined(LASER_RGB_STRIP_PAGE)
            print_page_set_raster_x(p_page, urf_page.width * BITS_PER_PIXEL * ((urf_page.colorSpace == 0) ? 1 : 3));
#elif defined(LASER_CMYK_STRIP_PAGE)
            print_page_set_raster_x(p_page, ppl * 2); // stefan revisit: resolution scaling: assumes 600x2
#else
#error
#endif
            print_page_set_image_height_lines(p_page, urf_page.height * scale);
            print_page_set_image_width_in_pixels(p_page,  urf_page.width * scale );
            print_page_set_video_x(p_page, urf_page.width * scale);
#if defined(ZX_STREAM_PAGE) 
            print_page_set_output_bpp(p_page, BITS_PER_PIXEL * ((urf_page.colorSpace == 0) ? 1 : 3)); 
#elif defined(LASER_CMYK_STRIP_PAGE) //REVIST WHAT SHOULD GO HERE FOR SS1000?
            print_page_set_output_bpp(p_page, BITS_PER_PIXEL); 
#elif defined(LASER_RGB_STRIP_PAGE)  //REVIST WHAT SHOULD GO HERE FOR SS1000?
            print_page_set_output_bpp(p_page, BITS_PER_PIXEL * ((urf_page.colorSpace == 0) ? 1 : 3)); // 8 bit per pixel
#else
#error
#endif
#ifdef ZX_STREAM_PAGE
            // Set the Zx compress information in the Page header.
            // This must be done before the page start is sent.
            print_page_set_print_quality(p_page, PRINTQUALITY_NORMAL); // hardcode to normal for now
            print_page_add_zx_color_size(p_page, ((urf_page.colorSpace == 0) ? 1 : 3));
            print_page_add_zx_raster_x(p_page, urf_page.width);
            print_page_add_zx_raster_y(p_page, urf_page.height);
            print_page_add_zx_res_x(p_page, 0); //this is what is what we found in a trace from a ZJ job.  No idea why.
            print_page_add_zx_res_y(p_page, 0); //this is what is what we found in a trace from a ZJ job.  No idea why.
#else
            print_page_set_print_quality(p_page, urf_page.print_quality);
            
#endif
            if ( urf_page.copies > 1 )
            {
                print_page_set_num_copies( p_page, urf_page.copies );
            }
            print_page_start( p_page );


            //consume a page worth of data
            if ( 0 > urf_raster_data( bstream, Destination, &urf_page, p_page) )
            {
                DBG_PRINTF_NOTICE("urf: --------------- URF Cancel -----------------\n");
                result = 2;
                print_page_end( p_page, cp_pixel_count( mlimiter, cp, true ) );
                print_page_free( p_page );
                break;
            }
            else
            {
                print_page_end( p_page, cp_pixel_count( mlimiter, cp, true ) );
                print_page_free( p_page );
            }

            DBG_PRINTF_NOTICE("urf: page done\n");
            if(urf_parser_poll_for_cancel(bstream)){
                DBG_PRINTF_ERR("urf: Abort job because received Cancel Job message during parsing\n");
                result = 2;
            }
            
        }
        else if ( result == URF_ABORT_PARSE )
        {
            result = 1;
            break;
        }
        else
        {
            DBG_PRINTF_NOTICE("---- urf read header error? \n");
            break;  // spec doesn't have any eof marker so we rely on read error
            // needs performance tweek
        }
        DBG_PRINTF_NOTICE("job_pages_done %d job_pages %d\n",urf_page.job_pages_done,urf_page.job_pages);
    }
    while (++urf_page.job_pages_done < urf_page.job_pages);

    if (p_job)
    {
        if (result == 0) 
        {
            p_job = print_job_end( p_job );
            result = PARSER_SUCCESS;
        }
        else if ( result == 1 )
        {
            p_job = print_job_abort( p_job );
            result = PARSER_ERROR;
        }
        else
        {
            p_job = print_job_cancel( p_job );
            result = PARSER_ERROR;
        }
    }
    else
    {
        // in the case where we never started a job because
        // we errored out early we still create and then abort a job to let
        // the upperlevel code know that the job was aborted.
        p_job = print_job_construct( mlimiter, pipe );
        print_job_start( p_job, Destination );
        p_job = print_job_abort( p_job );

        result = PARSER_ERROR;
    }
    // job end
    cp = color_pipe_flush(cp);
    bstream = urf_bstream_close(bstream);

    if (result != PARSER_SUCCESS)
    {
        urf_flush_pipe_to_uel(pipe);
    }

    return result;
}

#ifdef be32_to_cpu
uint32_t be32_to_cpu_little_endian (uint32_t in)
{
    uint32_t tmpvar = ((uint32_t) in ) & 0xff;
    tmpvar <<= 8; 
    tmpvar |= ((uint32_t) in >> 8) & 0xff;
    tmpvar <<= 8; 
    tmpvar |= ((uint32_t) in >> 16) & 0xff;
    tmpvar <<= 8; 
    tmpvar |= ((uint32_t) in >> 24) & 0xff;
    return tmpvar;
}
#endif


