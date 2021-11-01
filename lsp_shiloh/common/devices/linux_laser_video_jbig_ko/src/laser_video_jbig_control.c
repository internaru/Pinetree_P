/** 
 *
 * 
 * ============================================================================
 * Copyright (c) 2006-2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *  Description:
 *  This file contains the functionality for managing the video hardware for
 *  printing on monochrome, color multipass and color inline laser priners.
 * 
 *  Note: With the video hardware, many of the hardware registers exist for
 *  each video channel. In an inline color laser print engine, each color is 
 *  processed with a fixed physical delay (spacing between development
 *  stations). This means that one or more colors could be printing a given
 *  page, while a different color could be printing on the next page. Pages from
 *  subsequent jobs can follow one another with no gaps, as long as the engine
 *  speed and bit depth remain constant. If the bit depth changes, this requires
 *  the reprogramming of the video pulse code (PCODE) register table, as this
 *  table applies to ALL colors. For an inline color printer, the current page
 *  will be allowed to complete, then the video hardware will be setup for the
 *  new bit depth and printing can resume. For multipass color printers, the bit
 *  depth can be changed seamlessly between pages with no delay between pages.
 * 
 * 
 *  COLOR INLINE and MULTIPASS PRINTING
 *  
 *  Color "inline" printers print all color planes (four) simultaneously. The
 *  color developers are "inline", spaced a short distance apart. Multipass 
 *  printers print four planes sequentially, essentially this is the same
 *  as monochrome printing - one monochrome page per color. Inline color 
 *  printers have one print plane for each color, and multipass color printers
 *  share one print plane among all colors. Either printer may have one or
 *  two laser beams per print plane.
 * 
 *  For inline printers, the relationship between input compressed buffers and 
 *  output video buffers is one to one. The output video buffer is sized to 
 *  accomodate one compressed input buffer. Historically, the buffer size has 
 *  been 100 lines - and has to accomodate the worst case bit depth. Note that
 *  each compressed input buffer is a unique JBIG "image".
 * 
 *  For multipass printers, the input data may be one or more compressed 
 *  buffers, and the input buffers will represent one JBIG "image" for each
 *  color. This is significantly different than the color inline case! The
 *  relationship between input and output buffers can be one to many, or 
 *  several to many. The number of input buffers may vary, but the number of
 *  output video buffers will be constant for each page size.
 * 
 *  In either of the above cases, the input data buffer can be released back to 
 *  the system when the JBIG decompression is completed.
 *  
 *  
 * CONFIGURATION OF VIDEO HARDWARE: 
 *  
 *  The video hardware configuration is determined by the print mechanism. This
 *  includes the number of active video channels, the mapping of color planes to
 *  video channels, pixel frequency, etc. For each active video channel, a video
 *  handle will be created via an "open" call to the low level hardware
 *  interface. If the video channel is not in use, it will be null. Many of the
 *  operations in this code will loop through all the available video channels
 *  provided by the hardware implementation for various operations. If the
 *  handle is null for that particulare channel, the requested operation will
 *  return immediately with a FAIL error. This is referring specifically to
 *  function calls made to functions in the mrvl_video.c file.
 * 
 * MANAGEMENT OF DATA BUFFERS:
 * 
 *  This file handles two types of data buffers, JBIG compress and RAW or 
 *  EMPTY buffers. Host based print files consist of JBIG compressed data 
 *  buffers and internal calibration pages are RAW or EMPTY buffers.
 * 
 *  The JBIG input compress buffers are decompressed to produce video ready
 *  data, contained in video output buffers. The state of the JBIG input 
 *  buffers is managed by maintaining "JBIG read DMA nodes" on different 
 *  lists. The lists are as follows:
 * 
 *      JBIG Read DMA in progress
 *      JBIG Read DMA completed
 * 
 *  When the read DMA operation has completed, the buffer can be released to 
 *  allow its memory to be freed. As the decode completion may occur at a 
 *  different time than read complete, the following lists are used to
 *  maintain the state of video output buffers, which are filled by the JBIG
 *  decompression. 
 * 
 *      JBIG Decode in progress
 *      JBIG Decode completed
 * 
 *  If the print data isn't compressed it is either RAW or EMPTY. In that
 *  case the above lists are not needed and the buffers flow directly 
 *  through the video pipe. To manage the video data buffers, several more
 *  lists are created. Essentially, these lists each represent a "state" in 
 *  the video processing. A typedef of a structure "container" is 
 *  defined to allow creation of video channel "nodes". These nodes are 
 *  passed from list to list as the video processing takes place. These lists
 *  are as follows:
 * 
 *      Video idle
 *      Video DMA in progress
 * 
 *  Video output buffers arrive for video processing either directly as RAW
 *  or EMPTY buffers, or via the "JBIG Decode Completed" list. The JBIG 
 *  buffers and RAW buffers take slightly different paths through the
 *  video code:
 *      1. JBIG buffers are decompressed into one or many output data buffers 
 *  (see discussion on inline and multipass color printing) and the output 
 *  data buffers are committed to video. When decompression is completed, the 
 *  input buffer can be moved directly to the "JBIG Read DMA Completed" list. 
 *  Although JBIG decompression may not be complete, and video isn't complete, 
 *  the JBIG input buffer is no longer needed. When JBIG decompression is
 *  completed, the video output buffer is now available to move to the "Video
 *  DMA in Progress" list be configured for Video DMA. This will happen when the
 *  hardware demands more data (via video "descriptor complete" interrupt).
 *      2. RAW buffers are directly printed, when they are attached to a video 
 *  descriptor they are moved to the "Video DMA in Progess" list. When the 
 *  video descriptor has completed the DMA operation, the RAW buffer can be 
 *  moved to the "Video idle" list.
 * 
 *  When buffers are moved to the "Video idle" list, they are ready to be 
 *  released back to the system; i.e. freed back to memory heap. 
 * 
 *  A cancel or abort operation will cause all of data on all the buffers to be 
 *  removed from the lists, and the buffers will be "released" so they can be
 *  freed by job management.
 * 
 *  Management of page lists of print data:
 * 
 *  The following lists are maintained on a per page basis, and are part of the
 *  page allocation:
 *      > Pending print list: contains data buffers not yet decompressed and
 *      printed. This list is left intact throughout the process of printing the
 *      page. A pointer to the next buffer to decompress and print is maintained
 *      and updated throughout the process of printing the page. This list is
 *      left intact to facilitate reprinting the page if necessary; e.g. paper
 *      jam. If the buffers on this list are compressed, they are placed on the
 *      "printing" list when JBIG decompression commences.
 *      > Printing list: contains data buffers that are in process of being
 *      decompressed. RAW or EMPTY buffers do not transition to this list.
 *      > Printed list: it contains input data buffers that have been
 *      successfully decompressed. RAW or EMPTY buffers transition to this list
 *      directly from the "pending" list when video is completed.
 * 
 * 
 *  Compensation for "bow/tilt" via Pixel Frequency Adjust:
 * 
 *  For some inline printers, realtime video frequency compensation is required
 *  due to nonlinear velocity in the laser beam(s). This can cause misalignment
 *  errors between the print planes. For multipass printers, the same beam is 
 *  used for all print colors, so misalignment isn't a visual problem. To 
 *  compensate for this error, special video pixel frequency adjustment 
 *  hardware is incorporated into the video frequency generation. The frequency
 *  error results in a phenomenon known as "dot runout", and is compensated for
 *  by "warping" the video frequency. This is accomplished by stepwise changing
 *  the frequency up to 64 times across a page.
 * 
 *  Video "dot runout" compensation:
 * 
 *  1. Setup the video pixel frequency adjustment hardware to compensate for 
 *     dot runout error caused by bow/tilt optical problems. This is done at 
 *     powerup, and whenever video pixel frequency changes; e.g. engine speed 
 *     change. This establishes a baseline video pixel frequency value for 
 *     every video channel.
 * 
 *  2. Use the video pixel frequency correction generated above, plus width 
 *     and margin error (top and side) to correct placement of each color plane 
 *     on the page.
 * 
 *  Descriptor based DMA:
 *      The Marvell video hardware uses "descriptor" based DMA, which allows
 *  for configurable DMA "chains". Each descriptor contains a configuration 
 *  word, a data buffer address, a pointer to the next descriptor, and line 
 *  count. These video descriptors must reside in non-cacheable memory as the
 *  DMA controller reads them directly from memory. The video code configures
 *  an array of descriptors, intializes the next descriptor pointer and fills
 *  out the remaining fields when the descriptor is "committed" to the DMA
 *  operation.
 *  
 *  Vertical scaling:
 *      Vertical scaling as performed by the MIP block will cause a reduction in
 *  output lines in the image; e.g. 2/3 scaling allows for conversion of 600 dpi
 *  data to 400 dpi for use on a 400 dpi native resolution print engine.
 *      In this case, the variables that are used to keep track of number of
 *  image lines for video DMA and the number of lines processed from the input
 *  image are not the same. These variables are used to keep track of and manage
 *  the JBIG decompression and video DMA operations to ensure the finish
 *  simultaneously with the hardware marking End Of Image (EOI).
 *
 *  To Do list:
 *    
 * 
 **/

 /*
  * There are two types of JBIG compressed pages that are handled by this file; 
  * multiple JBIG images per page and one JBIG image per page. The JBIG driver 
  * treats each JBIG image as a transaction, with a corresponding start and stop 
  * transaction API call for each JBIG image. This is important as it affects 
  * the processing of the two types of page compression. The characteristics of 
  * the processing of these two types is as follow: 
  *  
  * One JBIG image per page: 
  *  
  * Multiple JBIG images per page: 
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "dros.h"
#include "laser_video_jbig_control_api.h"
#include "laser_engine_config_types.h"
#include "video_api.h"
#include "list.h"
#include "lassert.h"
#include "memAPI.h"
#include "jbig_decode_api.h"


/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define DBG_PRFX "VIDEO_LASER: "
#ifdef __KERNEL__
#define ERR(...) printk(KERN_ERR DBG_PRFX __VA_ARGS__); printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define MSG(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define SLOG_ERR(...) printk(KERN_ERR DBG_PRFX __VA_ARGS__); printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define SLOG_MSG(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define SLOG_WRN(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define SLOG_INFO(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define DBG_SLOG_ENTRY //printk(KERN_DEBUG DBG_PRFX "==> %s\n", __FUNCTION__);
#define DBG_SLOG_EXIT  //printk(KERN_DEBUG DBG_PRFX "<== %s\n", __FUNCTION__);
#else
#define LOGGER_SUBMOD LOGGER_SUBMODULE_BIT( 2 )
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_VIDEO | LOGGER_SUBMOD

#if 1 // printf using video module controls
#  define ERR DBG_PRINTF_ERR
#  define MSG DBG_PRINTF_DEBUG
#else // printf using dprintf module controls so that printf and log controls differ.
#  define ERR(...) DPRINTF(DBG_LOUD|DBG_OUTPUT, (DBG_PRFX __VA_ARGS__))
#  define MSG(...) DPRINTF(DBG_LOUD|DBG_OUTPUT, (DBG_PRFX __VA_ARGS__))
#endif

#define SLOG_ERR DBG_MEMLOG_ERR
#define SLOG_MSG DBG_MEMLOG_INFO
#define SLOG_INFO DBG_MEMLOG_INFO

#define DBG_SLOG_ENTRY DBG_MEMLOG_DEBUG( "%s <<<==== ENTRY\n", __FUNCTION__);
#define DBG_SLOG_EXIT  DBG_MEMLOG_DEBUG( "%s ====>>> EXIT\n", __FUNCTION__);
#endif
                       
/** local definitions **/

/** private video messages **/
#define VIDEO_MSG_BASE                  (0)
#define VIDEO_MSG_ADD_PAGE              (VIDEO_MSG_BASE + 1)
#define VIDEO_MSG_VIDEO_DMA_BUFFER_DONE (VIDEO_MSG_BASE + 2)
#define VIDEO_MSG_PROC_END_PLANE        (VIDEO_MSG_BASE + 3)
#define VIDEO_MSG_JBIG_IDMA_BUFFER_DONE (VIDEO_MSG_BASE + 4)
#define VIDEO_MSG_JBIG_ODMA_BUFFER_DONE (VIDEO_MSG_BASE + 5)
#define VIDEO_MSG_JBIG_DONE             (VIDEO_MSG_BASE + 6)    // TODO: remove if not needed
#define VIDEO_MSG_CANCEL_PAGES          (VIDEO_MSG_BASE + 7)

#define VIDEO_PAGE_CANCEL (VIDEO_LAST_EVENT + 1)

#define NUM_VIDEO_DMA_BUFFERS_PER_COLOR 16
#define MAX_LINES_PER_VIDEO_DMA_BUFFER 132 // 128 when scaling last buffer is larger :(
// A3 pixel width of 11.7 * 600 dpi * 4bpp / 8bits/byte and rounded to a multiple of 64 bytes for padding
#define MAX_LINE_WIDTH_IN_BYTES (((uint32_t)(11.7*600*4/8) + 64) & ~0x3F)
#define MAX_VIDEO_DMA_BUFFER_SIZE (MAX_LINES_PER_VIDEO_DMA_BUFFER * MAX_LINE_WIDTH_IN_BYTES)

/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */

/** Defines used in flags field of video_dma_buffer_t */
#define VIDEO_DMA_BUFFER_FLAG_IN_USE    (1<<0)      ///< DMA buffer is in use
#define VIDEO_DMA_BUFFER_FLAG_SOI       (1<<1)      ///< DMA buffer is start-of-image
#define VIDEO_DMA_BUFFER_FLAG_EOI       (1<<2)      ///< DMA buffer is end-of-image

typedef struct video_dma_buffer_s
{
    dma_alloc_t* buffer;
    COLOR process_color;
    uint32_t data_len;
    uint32_t num_lines;
    uint32_t flags;
}video_dma_buffer_t;

typedef struct _video_dma_buffer_block
{
    video_dma_buffer_t buffers[NUM_VIDEO_DMA_BUFFERS_PER_COLOR];
    uint32_t next;
    uint32_t last;
    uint32_t num_in_use;
}video_dma_buffer_block_t;

typedef struct _laser_video_page_wrapper
{
    ATLISTENTRY node;
    laser_video_page_t page_desc;
    ATLISTENTRY planeDataPendingPrintList[MAX_MARKING_COLOR_CHANNELS];
}laser_video_page_wrapper_t;

typedef struct _laser_video_plane_wrapper
{
    ATLISTENTRY node;
    laser_video_plane_t plane_desc;
    video_dma_buffer_t  plane_dma_context;
}laser_video_plane_wrapper_t;

typedef struct _video_msg_page_done_params
{
    uint32_t chan;
    uint32_t video_event;
}video_msg_page_done_params_t;

typedef union _video_msg_params
{
    laser_video_page_wrapper_t* add_page_param;
    video_dma_buffer_t* video_dma_done_param;
    video_msg_page_done_params_t page_done_params;
    laser_video_plane_wrapper_t* jbig_idma_done_plane;
    COLOR jbig_odma_done_color;
}video_msg_param_t;

typedef struct _vidctl_msg
{
    uint32_t id;
    video_msg_param_t param;
}vidctl_msg_t;

typedef enum 
{
    JBIG_ONE_IMAGE_PER_PAGE,
    JBIG_MULTIPLE_IMAGES_PER_PAGE,  // Implies that Yd = L0 for all compressed buffers for the page
} video_jbig_data_style_t;

// Local structure to contain state information related to printing of the
// current page.
typedef struct image_context_s
{
    uint32_t bitsPerPixel;    // Bit depth of the current page.

    // Image placement information - top and side margin. Keep this on a per
    // color basis, even for multipass color printing that only has one print
    // plane.
    uint32_t side_margin_in_pixels[MAX_NUM_VIDEO_CHAN];
    uint32_t side_margin_in_subpixels[MAX_NUM_VIDEO_CHAN];   
    uint32_t sub_pixel_denominator;           // Define size of subpixel; e.g. 1/8
    uint32_t top_margin[MAX_NUM_VIDEO_CHAN];          // Top of page margin

    // Keep track of how many JBIG write lines are remaining for the current
    // decompression cycle.
    uint32_t lines_remaining_to_write_to_laser[MAX_MARKING_COLOR_CHANNELS]; 

    // Intermediate video buffers.  Used when printing JBIG compressed images.
    // The jbig images are decoded into these buffers before being sent to video.
    video_dma_buffer_block_t video_dma_block[MAX_MARKING_COLOR_CHANNELS];

    uint32_t line_width_in_bytes; // video image line width in bytes
    uint32_t page_height_in_lines;

    uint32_t resolutionX;
    uint32_t resolutionY;

    laser_video_page_wrapper_t *current_page; 
    uint32_t colorCompleted[MAX_MARKING_COLOR_CHANNELS]; // Used to manage multipass printing and prevent multiple 
                                                         // completions per plane during error handling.
                                                         // if 0 plane is not completed.  if completed holds non-zero
                                                         // completion cause.  Typically one of the video event
                                                         // causes from video_api.h or VIDEO_PAGE_CANCEL if page cancelled.
                                                        
    uint32_t planes_printed;
    bool video_active;

    compression_types_t page_compression_mode;
    video_jbig_data_style_t jbig_images_per_page;
    bool jbig_color_is_active[MAX_MARKING_COLOR_CHANNELS];
    COLOR current_jbig_color;

    bool jbig_decode_running; 
    uint32_t current_marking_color_channel;

    ATLISTENTRY *pNextPrintNode[MAX_MARKING_COLOR_CHANNELS];
} image_context_t;

/* -------------------------------------------------------------------------- */
/*                   F O R W A R D  D E C L A R A T I O N S                   */
/* -------------------------------------------------------------------------- */
static void video_init_general( void );

static int video_thread( void* value );

static void video_start_next_page( void );
static void video_start_plane (laser_video_page_wrapper_t *page, uint32_t marking_color_channel);

static void imageRAW(COLOR process_color);

void videoProcEndOfPlane(uint32_t process_color, uint32_t plane_completion_cause);
void videoProcEndOfPage( uint32_t video_event );

static void video_release_plane_buffers(laser_video_page_wrapper_t *page, COLOR plane_color);
static void video_release_page(laser_video_page_wrapper_t* page);

static void video_jbig_event_callback(jbig_decode_handle_t* handle, void* context, uint32_t status);
static void video_jbig_idma_complete( jbig_decode_handle_t* handle, void* context );
static void video_jbig_odma_complete( jbig_decode_handle_t* handle, void* context );

static error_type_t video_add_jbig_output_buffer( COLOR process_color, video_dma_buffer_t *dma_buffer, uint32_t num_lines, bool end_of_image);

static void video_dma_buffer_complete_callback( void *handle, void* context );
static void viddrv_event_handler(video_handle_t *handle,  
                                uint32_t video_event, 
                                void* context);

static bool video_alloc_dma_buffer_block(video_dma_buffer_block_t* block);
static void video_reset_dma_buffer_block(video_dma_buffer_block_t* block);
void video_free_dma_buffer_block(video_dma_buffer_block_t* block);

static void video_run_jbig_image_per_plane(COLOR process_color);
static void video_run_jbig_image_per_strip( void );
static void video_start_jbig(COLOR process_color);

static bool video_write_plane_data_to_jbig(COLOR process_color, laser_video_plane_wrapper_t* plane);
void video_write_dma_buffer_to_viddrv(COLOR process_color, video_dma_buffer_t* dma_buf, bool start_of_image, bool end_of_image);


/* -------------------------------------------------------------------------- */
/*               P R I V A T E  M O D U L E  V A R I A B L E S                */
/* -------------------------------------------------------------------------- */
static laser_engine_video_config_t vid_eng_config = {0};
static image_context_t image_context;

// Video thread variables and defines
static dros_thread_t* video_thread_id;
#define VIDEO_MIN_STACK_SIZE 4096
static dros_mq_t video_msgq;
#define QUEUE_SIZE 100

static dros_smp_lock_t video_smplock;

static bool paperless_enabled = false;

static vidctl_page_done_callback_t end_of_page_callback = 0;  ///< to engine 
static void* end_of_page_callback_context = 0;

static jbig_decode_handle_t * jbig_handle = NULL;

// Maintain a list of pending pages.
static ATLISTENTRY video_page_list;
static laser_video_page_wrapper_t* incoming_page;

static video_handle_t *video_handle[MAX_NUM_VIDEO_CHAN];   
static uint32_t dot_count[MAX_NUM_VIDEO_CHAN]={0, };

/** public functions **/

/** 
 * \brief Initialize the video control driver.
 *
 * This function will initialize the video conrol driver. This must be called before
 * using any other driver function.
 **/
void vidctl_init(laser_engine_video_config_t* eng_video_config)
{
    SLOG_MSG("%s num_print_planes 0x%x\n", __func__, vid_eng_config.num_print_planes);
    if(vid_eng_config.num_print_planes == 0)  // guard againt multiple inits
    {
        memcpy(&vid_eng_config, eng_video_config, sizeof(laser_engine_video_config_t));
        ASSERT(is_engine_video_config_valid(&vid_eng_config));
        viddrv_init(&vid_eng_config);  // init the low-level video HW driver component
    
        jbig_decode_init();  // init the low-level jbig HW driver component

        video_init_general();  // init the jbig control component
    }
    else												// for run-time changing.
    {
        memcpy(vid_eng_config.pulse_code_1_bit_array, eng_video_config->pulse_code_1_bit_array, sizeof(vid_eng_config.pulse_code_1_bit_array));
        memcpy(vid_eng_config.pixel_freq_in_hz, eng_video_config->pixel_freq_in_hz, sizeof(vid_eng_config.pixel_freq_in_hz));
        ASSERT(is_engine_video_config_valid(&vid_eng_config));
    }
}


/**
 * \brief Set the pcode table to new values.  Used for runtime tuning of pcode values.
 **/
void vidctl_set_pcodes(e_pcode_index index, uint32_t *eng_video_pcode_config, uint32_t num_entries)
{
    DBG_SLOG_ENTRY

    if (num_entries > MAX_NUM_PCODE_REGS)
    {
        SLOG_ERR("%s: Max pcode entries exceeded.  Max = %d, request = %d, truncating to max.\n", __func__, MAX_NUM_PCODE_REGS, num_entries);
        num_entries = MAX_NUM_PCODE_REGS;
    }

    switch (index)
    {
        case PCODE_INDEX_1BIT:
            memcpy(vid_eng_config.pulse_code_1_bit_array, eng_video_pcode_config, (sizeof(uint32_t)*num_entries));
            break;
        case PCODE_INDEX_2BIT:
            memcpy(vid_eng_config.pulse_code_2_bit_array, eng_video_pcode_config, (sizeof(uint32_t)*num_entries));
            break;
        case PCODE_INDEX_4BIT:
            memcpy(vid_eng_config.pulse_code_4_bit_array, eng_video_pcode_config, (sizeof(uint32_t)*num_entries));
            break;
        case PCODE_INDEX_ED1BIT:
            memcpy(vid_eng_config.ed_pulse_code_1_bit_array, eng_video_pcode_config, (sizeof(uint32_t)*num_entries));
            break;
        case PCODE_INDEX_ED2BIT:
            memcpy(vid_eng_config.ed_pulse_code_2_bit_array, eng_video_pcode_config, (sizeof(uint32_t)*num_entries));
            break;
        case PCODE_INDEX_ED4BIT:
            memcpy(vid_eng_config.ed_pulse_code_4_bit_array, eng_video_pcode_config, (sizeof(uint32_t)*num_entries));
            break;
        default:
            SLOG_ERR("%s: Invalid pcode table index: %d\n", __func__, index);
            break;
    }
}

static void video_init_general( void )
{
    int i;
    dros_err_t oserr;
    bool block_alloc_status;

    memset(&image_context, 0, sizeof(image_context));

    image_context.video_active = false;
    image_context.jbig_decode_running = false;

    // create locks
    dros_smplock_init(&video_smplock);
    
    // Initialize the video page list
    ATInitList(&video_page_list);

    SLOG_MSG("%s dma_buffer_size = %d\n", __func__, MAX_VIDEO_DMA_BUFFER_SIZE );

    // allocate the video intermediate buffers
    for (i = 0; i < vid_eng_config.num_print_colors; i++)
    {
        block_alloc_status = video_alloc_dma_buffer_block(&image_context.video_dma_block[i]);
        ASSERT(block_alloc_status);
    }

    // create the message queue
    oserr = dros_mq_init(&video_msgq, "laser_vid_mq", QUEUE_SIZE, sizeof(vidctl_msg_t));
    ASSERT(oserr == DROSERR_NONE);

    // create the thread
    oserr = dros_create_thread(&video_thread_id, video_thread, 0, "laser_video", VIDEO_MIN_STACK_SIZE, DROS_THR_PRI_IMAGE_DRIVER);
    ASSERT(oserr == DROSERR_NONE);

    SLOG_MSG("%s done\n", __func__ );
}

/// \brief Post a message directly to the video message loop; this routine posts
/// with at timeout of 0 and so may be called directly from an ISR.  It asserts
/// if the video_dma_buffer_blockqueue is full.
static inline bool video_post_message( uint32_t msgid, video_msg_param_t* vid_msg_param)
{
    vidctl_msg_t msg;
    msg.id = msgid;
    msg.param = *vid_msg_param;

//    SLOG_MSG("%s post the message 0x%x\n", __func__, msg.id);

    if(DROSERR_NONE == dros_mq_send_nowait(&video_msgq, &msg))
    {
        return true;
    }
    ASSERT(0);
    return false;
}

static bool video_alloc_dma_buffer_block(video_dma_buffer_block_t* block)
{
    int i;

    SLOG_MSG( "%s: block %#x\n", __func__, (uint32_t)block);

    memset(block, 0, sizeof(video_dma_buffer_block_t));

    for (i = 0; i < NUM_VIDEO_DMA_BUFFERS_PER_COLOR; i++)
    {
        block->buffers[i].buffer = dma_alloc(MAX_VIDEO_DMA_BUFFER_SIZE);
        //SLOG_ERR( "%s: buffer %d = %#x\n", __func__, i, (uint32_t)block->buffers[i].buffer);
        if (block->buffers[i].buffer)
        {
            dma_buffer_map_single(block->buffers[i].buffer, DMA_TO_DEVICE);
        }
        else
        {
            video_free_dma_buffer_block(block);
            return false;
        }
    }

    return true;
}

static void video_reset_dma_buffer_block(video_dma_buffer_block_t* block)
{
    int i;
    if(block->num_in_use)
    {
        SLOG_MSG( "%s: num_in_use = %d\n", __func__, block->num_in_use );
        for (i = 0; i < NUM_VIDEO_DMA_BUFFERS_PER_COLOR; i++)
        {
            block->buffers[i].flags = 0;
        }
    }
    block->last = 0;
    block->next = 0;
    block->num_in_use = 0;
}

void video_free_dma_buffer_block(video_dma_buffer_block_t* block)
{
    int i;
    SLOG_MSG( "%s: block %#x\n", __func__, (uint32_t)block);

    for (i = 0; i < NUM_VIDEO_DMA_BUFFERS_PER_COLOR; i++)
    {
        if (block->buffers[i].buffer)
        {
            dma_buffer_unmap_single(block->buffers[i].buffer);
            dma_free(block->buffers[i].buffer);
        }
    }
    memset(block, 0, sizeof(video_dma_buffer_block_t));
}

static video_dma_buffer_t *video_get_buffer_from_block(video_dma_buffer_block_t* block)
{
    video_dma_buffer_t *buf = NULL;
//    uint32_t lockflags;

    XASSERT(block->num_in_use <= NUM_VIDEO_DMA_BUFFERS_PER_COLOR, block->num_in_use);

//    SLOG_MSG( "%s: block %#x num_in_use = %d, next = %d\n", __func__, (uint32_t)block, block->num_in_use, block->next);

//    dros_smplock_acquire_isr(&video_smplock, &lockflags);
    if(block->num_in_use < NUM_VIDEO_DMA_BUFFERS_PER_COLOR)
    {
        buf = &block->buffers[block->next];
        ASSERT(buf->flags == 0);

        buf->flags |= VIDEO_DMA_BUFFER_FLAG_IN_USE;
        if(++block->next == NUM_VIDEO_DMA_BUFFERS_PER_COLOR)
        {
            block->next = 0;
        }
        block->num_in_use++;
    }
//    dros_smplock_release_isr(&video_smplock, lockflags);

    return buf;
}

static void video_return_buffer_to_block(video_dma_buffer_block_t* block, video_dma_buffer_t *buf, bool return_to_tail)
{
    ASSERT(buf != NULL);
    ASSERT(buf->flags & VIDEO_DMA_BUFFER_FLAG_IN_USE);
    XASSERT((block->num_in_use <= NUM_VIDEO_DMA_BUFFERS_PER_COLOR) && (block->num_in_use > 0), block->num_in_use);

    SLOG_MSG( "%s: block %#x num_in_use = %d, next = %d last = %d\n", __func__, (uint32_t)block, block->num_in_use, block->next, block->last);

    //    dros_smplock_acquire_isr(&video_smplock, &lockflags);
    buf->flags = 0;
    block->num_in_use--;

    if (return_to_tail)
    {
        if(buf != &block->buffers[block->last])
        {
            SLOG_ERR( "%s: ERROR: buf = %p, expected %p, num_lines = %d, last = %d, num_in_use = %d\n", __func__, buf, &block->buffers[block->last], buf->num_lines, block->last, block->num_in_use);
        }
//-jrs        ASSERT(buf == &block->buffers[block->last]);  // buffers must be returned in order and to the correct block

        if(++block->last == NUM_VIDEO_DMA_BUFFERS_PER_COLOR)
        {
            block->last = 0;
        }
    }
    else
    {
        // returning to head of list
        if (--block->next > NUM_VIDEO_DMA_BUFFERS_PER_COLOR-1)  // if we roll under from 0 to 0xFFFFFFFF
        {
            block->next = NUM_VIDEO_DMA_BUFFERS_PER_COLOR-1;
        }
        ASSERT(buf == &block->buffers[block->next]);  // buffers must be returned in order and to the correct block
    }
//    dros_smplock_release_isr(&video_smplock, lockflags);
}


void video_handle_msg_add_page(vidctl_msg_t* msg)
{
    laser_video_page_wrapper_t* page_wrapper = msg->param.add_page_param;

    XASSERT(msg->id == VIDEO_MSG_ADD_PAGE, msg->id);
    ASSERT(page_wrapper);

    SLOG_MSG( "%s: page 0x%x\n", __func__, (uint32_t)page_wrapper );

    if(page_wrapper)
    {
        // LOCKME ?
        ATInsertTailList(&video_page_list, &page_wrapper->node);
    
        // Start the video for this page. If video is already active
        // from another page this will simply return. It will be called
        // again via message when the current page finishes.
        video_start_next_page();
    }
}

void video_handle_msg_dma_buffer_done(vidctl_msg_t* msg)
{
    video_dma_buffer_t* dma_buffer_context = msg->param.video_dma_done_param;
    ASSERT(dma_buffer_context);
    
    if(dma_buffer_context)
    {
        switch (image_context.page_compression_mode)
        {
            case COMPTYPE_JBIG:
                // return buffer to the pool
                video_return_buffer_to_block(&image_context.video_dma_block[dma_buffer_context->process_color], dma_buffer_context, true);

                // continue running jbig
                if (image_context.jbig_images_per_page == JBIG_ONE_IMAGE_PER_PAGE)
                {
                    video_run_jbig_image_per_plane(dma_buffer_context->process_color);
                }
                else
                {
                    video_run_jbig_image_per_strip();
                }
                break;
               
            case COMPTYPE_NONE:
                dma_buffer_unmap_single(dma_buffer_context->buffer);
                imageRAW(dma_buffer_context->process_color);
                break;

            default:
                ASSERT(0);
        }
    }
}

void video_handle_msg_jbig_idma_done(vidctl_msg_t* msg)
{
    laser_video_plane_wrapper_t* plane = msg->param.jbig_idma_done_plane;
    laser_video_page_wrapper_t* page = image_context.current_page;
    COLOR process_color;
    ASSERT(plane);
        
    process_color = plane->plane_desc.sysPlaneColor;
    dma_buffer_unmap_single(&plane->plane_desc.dma_buffer);

    // Process more jbig strips if there are multiple strips per image.  In the case of single image
    // per strip we will restart processing during the jbig odma or video dma completion processing.
    if (JBIG_ONE_IMAGE_PER_PAGE == image_context.jbig_images_per_page)
    {
        // setup input (read) dmas; we will add as many input buffers as jbig will accept
        while (image_context.pNextPrintNode[process_color])
        {
            plane = CONTAINING_RECORD(image_context.pNextPrintNode[process_color], laser_video_plane_wrapper_t, node);
            ASSERT(plane);

            ASSERT(COMPTYPE_JBIG == plane->plane_desc.compression_info.comp_type);

            if (plane->plane_desc.end_of_image)
            {
                SLOG_MSG("%s: SENDING LAST JBIG INPUT DATA.  NOW AT INPUT EOI\n", __func__);
            }

            if (!video_write_plane_data_to_jbig(process_color, plane))
            {
                break;  // jbig did not accept the buffer; we're done, do not advance nextPrintNode
            }
            // Get the next data buffer in the plane
            image_context.pNextPrintNode[process_color] = ATListNext( &page->planeDataPendingPrintList[process_color], 
                                                                     image_context.pNextPrintNode[process_color]);
        }
    }
}

void video_handle_msg_jbig_odma_done(vidctl_msg_t* msg)
{
    // ODMA done should only be notified when running in multiple images per page mode
    ASSERT(image_context.jbig_images_per_page == JBIG_MULTIPLE_IMAGES_PER_PAGE);

    if (image_context.jbig_decode_running)
    {
        // note that we only stop the transaction here, not in the video dma callback
        jbig_decode_stop_transaction(jbig_handle);
        image_context.jbig_decode_running = false;    

        // continue running jbig
        video_run_jbig_image_per_strip();
    }
    else
    {
        // Under normal circumstances we only get odma done notifications while jbig is running.
        // However, in the case of video underrun errors we may get notifcations in after page
        // cancel.  Just ignore these notifications.
        SLOG_ERR("%s: JBIG NOT RUNNING.  Notification dropped.\n", __func__);
    }

    return;
}

void video_handle_msg_proc_end_plane(vidctl_msg_t* msg)
{
    uint32_t i;
    uint32_t chan = msg->param.page_done_params.chan;
    uint32_t video_event = msg->param.page_done_params.video_event;
    uint32_t process_color;
    laser_video_page_wrapper_t *page;
    bool all_planes_completed = true;

    // Get the contents of the message parameters.
    process_color = vid_eng_config.chan_to_print_plane_color[chan];
    page = image_context.current_page;

    SLOG_MSG("VIDEO_MSG_PROC_END_PLANE for page 0x%x, color %d, chan %d, video event %d\n", (uint32_t)page, process_color, chan, video_event);
    if(video_handle[chan] == 0)
    {
        // channel closed; usually this is due to a previous error
        ERR("Received END_PLANE notification on closed channel %d\n", chan);
        return;    
    }

    videoProcEndOfPlane( process_color, video_event);

    // See if there is another color plane to process. If not fall
    // through and process the end of page.
    if (page->page_desc.page_output_color_mode == e_Color)
    {
        if (image_context.planes_printed < 4)
        {
            all_planes_completed = false;

            if (vid_eng_config.video_processing == COLOR_MULTIPASS)
            {
                for (i=0; i < MAX_MARKING_COLOR_CHANNELS; i++)
                {
                    if (image_context.colorCompleted[i] == 0)
                    {
                        // dbg_printf("starting the next plane %d\n");
                        SLOG_MSG("starting the next plane %d\n", i);
                        video_start_plane (image_context.current_page, i);
                        break;
                    }
                    else
                    {
                        // dbg_printf("PLANE %d is MARKED COMPLETE\n", i);
                    }
                }
            }
        }
    }

    if (all_planes_completed)
    {
        // Only process the end of page for the master channel in a multi
        // laser system.  We should only get events for the master channel.
        ASSERT(vid_eng_config.report_video_event[chan] == true);
        SLOG_MSG( "VIDEO_MSG_PROC_END_PAGE 0x%x, video event %d\n", (uint32_t)page, video_event);
        videoProcEndOfPage( (uint32_t)video_event );
    }
}

void video_handle_msg_cancel_pages(vidctl_msg_t* msg)
{
    SLOG_ERR( "%s\n", __func__);

    if( image_context.current_page)
    {
        // complete outstanding page with cancelled status
        videoProcEndOfPage( VIDEO_PAGE_CANCEL );
    }
}

static int video_thread(void* value)
{
    vidctl_msg_t msg;

    MSG("%s starting\n", __func__);

    ASSERT(vid_eng_config.num_video_channels > 0); // check for correct init

    // Setup the video register configuration values for all the various 
    // engines and configurations supported for this product.
    // REVISIT videoInitConfigVals();
    //
    while(1)
    {
        // TODO: deal w/ exiting this loop and shutting down the driver
        dros_mq_receive(&video_msgq, &msg, DROS_WAIT_FOREVER);

        switch (msg.id)
        {
            case VIDEO_MSG_ADD_PAGE:
                video_handle_msg_add_page(&msg);
                break;

            case VIDEO_MSG_VIDEO_DMA_BUFFER_DONE:
                video_handle_msg_dma_buffer_done(&msg);
                break;

            case VIDEO_MSG_PROC_END_PLANE:
                video_handle_msg_proc_end_plane(&msg);
                break;

            case VIDEO_MSG_JBIG_IDMA_BUFFER_DONE:
                video_handle_msg_jbig_idma_done(&msg);
                break;

            case VIDEO_MSG_JBIG_ODMA_BUFFER_DONE:
                video_handle_msg_jbig_odma_done(&msg);
                break;

            case VIDEO_MSG_CANCEL_PAGES:
                video_handle_msg_cancel_pages(&msg);
                break;

            default:
                ERR("%s WARNING: unhandled mesg %d\n", __func__, msg.id);
                break;
        }
    }
    return 0;
}


// store pointer to incoming page; at this point we have only
// received the page header, not the data.  We will actually
// put the page on the list and start it after we have received
// all of the data
int vidctl_start_page(laser_video_page_t* lvpage)
{
    int errcode = FAIL;
    int i;

    MSG("\n%s\n", __func__);

    ASSERT(is_laser_video_page(lvpage));

    // LOCKME? incoming page lock?
    ASSERT(incoming_page == NULL);
    if(incoming_page == NULL)
    {
        incoming_page = MEM_MALLOC(sizeof(laser_video_page_wrapper_t));
        ASSERT(incoming_page != NULL);
    
        if(incoming_page)
        {
            incoming_page->page_desc = *lvpage;
            for(i = 0; i < MAX_MARKING_COLOR_CHANNELS; i++)
            {
                ATInitList(&incoming_page->planeDataPendingPrintList[i]);
            }
            errcode = OK;
        }
    }
    return errcode;
}

int vidctl_end_page(uint32_t page_id)
{
    int errcode = FAIL;
    video_msg_param_t msgparam;

    MSG("\n%s\n", __func__);

    // LOCKME
    if(incoming_page != NULL)
    {
        ASSERT(incoming_page->page_desc.page_id == page_id);
        if(incoming_page->page_desc.page_id == page_id)
        {
            msgparam.add_page_param = incoming_page;
            if(video_post_message(VIDEO_MSG_ADD_PAGE, &msgparam))
            {
                incoming_page = NULL;
                errcode = OK;
            }
        }
    }

    return errcode;
}

int vidctl_add_plane_data_to_current_page(laser_video_plane_t* lvplane)
{
    int errcode = FAIL;
    laser_video_plane_wrapper_t* plane_wrapper;
#if 0	// del.lsh too much amount of kernel log 
    MSG("\n%s\n", __func__);
#endif
    ASSERT(is_laser_video_plane(lvplane));

    // LOCKME?
    ASSERT(incoming_page != NULL);
    if(incoming_page != NULL)
    {
        plane_wrapper = MEM_MALLOC(sizeof(laser_video_plane_wrapper_t));
        ASSERT(plane_wrapper != NULL);
    
        if(plane_wrapper)
        {
            plane_wrapper->plane_desc = *lvplane;
            ATInsertTailList(&incoming_page->planeDataPendingPrintList[lvplane->sysPlaneColor], &plane_wrapper->node);
            errcode = OK;
        }
    }
    
    return errcode;
}

int vidctl_cancel_pages( void )
{
    error_type_t return_val = FAIL;
    video_msg_param_t msgparam = {0};  // empty parameters, but need to send to fill the message

    MSG("\n%s\n", __func__);

    if(video_post_message(VIDEO_MSG_CANCEL_PAGES, &msgparam))
    {
        return_val = OK;
    }

    return return_val;
}

void vidctl_set_paperless_mode(bool enable)
{
    MSG("%s %s\n", __func__, enable ? "ENABLED" : "DISABLED");
    paperless_enabled = enable;
}

bool vidctl_get_paperless_mode(void)
{
    return paperless_enabled;
}

void vidctl_query_pixel_count(uint32_t* non_white_pixels)
{
    uint32_t i = 0;
    for(i = 0; i < MAX_NUM_VIDEO_CHAN; i++)
        non_white_pixels[i] = dot_count[i];
}

#if 0

error_type_t video_cancel_page( void )
{
    error_type_t return_val;

    return_val = video_post_message(MSG_CANCELPAGE, 0, 0, 0);

    return return_val;
}
#endif

static void reset_image_context(image_context_t* context)
{
    // the image context structure contains some statically allocated
    // items such as video_dma_block.  Because of this we can't clear
    // the struct with memset.
    int i;

    context->bitsPerPixel = 0;
    context->sub_pixel_denominator = 0;
    context->line_width_in_bytes = 0;
    context->page_height_in_lines = 0;
    context->resolutionX = 0;
    context->resolutionY = 0;
    context->current_page = 0;
    context->planes_printed = 0;
    context->video_active = false;
    context->jbig_decode_running = false; 

    for (i = 0; i < MAX_NUM_VIDEO_CHAN; i++)
    {
        context->side_margin_in_pixels[i] = 0;
        context->side_margin_in_subpixels[i] = 0;   
        context->top_margin[i] = 0;
    }

    for (i = 0; i < MAX_MARKING_COLOR_CHANNELS; i++)
    {
        context->lines_remaining_to_write_to_laser[i] = 0; 
        context->colorCompleted[i] = 0;
        context->jbig_color_is_active[i] = 0;
        context->pNextPrintNode[i] = 0;
    }
}


static void video_start_next_page( void )
{
    laser_video_page_wrapper_t* page_wrapper;
    ATLISTENTRY *myNode;
    error_type_t status;
    uint32_t chan;
    uint32_t i;

    DBG_SLOG_ENTRY

    // If video is active, that means that another page has already gotten
    // started. There can be a race between new pages coming from the page
    // manager via MSG_PAGE_START and pages started by the video ISR sending
    // a message to start new page when the current page finishes.

    //LOCKME ?
    if (image_context.video_active == true)
    {
        SLOG_MSG( "%s **early** EXIT\n", __func__ );
        return;
    }

    reset_image_context(&image_context);
    image_context.video_active = true;
    SLOG_MSG(" video is active!!!!\n");

    // LOCKME
    myNode = ATRemoveHeadList(&video_page_list);

    SLOG_MSG( "video_start_new_page() REMOVE page from list: 0x%x\n", (uint32_t)myNode);

    if (myNode)
    {
        SLOG_MSG( " video start new page! node exists!\n");
        if( jbig_handle == NULL )
        {
            status = jbig_decode_open( &jbig_handle );
            ASSERT(status == OK);
            jbig_decode_set_event_callback(jbig_handle, video_jbig_event_callback, &image_context);
            jbig_decode_set_idma_callback(jbig_handle, video_jbig_idma_complete);
            jbig_decode_set_odma_callback(jbig_handle, video_jbig_odma_complete);

            SLOG_MSG("VIDEO: jbig_open 0x%x\n", (uint32_t)jbig_handle);
        }

        page_wrapper = CONTAINING_RECORD(myNode, laser_video_page_wrapper_t, node);
        SLOG_MSG( "video page: %d @ 0x%x\n", page_wrapper->page_desc.page_id, (uint32_t)page_wrapper);

        image_context.current_page = page_wrapper;
        image_context.page_compression_mode = COMPTYPE_NONE;     // default to no compression

        // Open all the active video channels for the given configuration. Channels
        // that aren't opened will have their handle assigned to 0.
        for (chan=0; chan < MAX_NUM_VIDEO_CHAN; chan++)
        {
            if (vid_eng_config.active_video_channels[chan] == true)
            {
                SLOG_MSG("%s opening channel %d rotation %d\n", __func__, chan, page_wrapper->page_desc.top_to_bottom);
                status = viddrv_open(&video_handle[chan], chan, page_wrapper->page_desc.top_to_bottom );
                ASSERT(status != FAIL);

                // report video events for only the channels configured for reporting
                if(vid_eng_config.report_video_event[chan])
                {
                    viddrv_register_event_callback(video_handle[chan], viddrv_event_handler, (void*)chan);
                    // Register a callback for each video channel that is configured to
                    // report for a print plane. Only one video channel handles events
                    // and callbacks for each print plane; e.g. 4 for inline color and
                    // 1 for monochrome and multipass color.
                    viddrv_register_dma_complete_callback( video_handle[chan], 
                                                          video_dma_buffer_complete_callback,
                                                          (void*)chan );
                }
                MSG("opened channel %d, handle 0x%x\n", chan, (uint32_t)&video_handle[chan]);

                viddrv_paperless_clear_CRC(chan);

                if(paperless_enabled)
                {
                    viddrv_paperless_setup(chan);
                    viddrv_paperless_clear_CRC(chan);
                }
            } 
            else
            {
                video_handle[chan] = 0;
            }
        }

        image_context.current_jbig_color = COLOR_LAST;
        for (i=0; i < MAX_MARKING_COLOR_CHANNELS; i++)
        {
            image_context.colorCompleted[i] = 0;
            image_context.jbig_color_is_active[i] = 0;
            video_reset_dma_buffer_block(&image_context.video_dma_block[i]);
        }

        MSG("page resolution is %d %d\n", page_wrapper->page_desc.image_info.image_dpi.x, page_wrapper->page_desc.image_info.image_dpi.y);
       
        image_context.bitsPerPixel = page_wrapper->page_desc.image_info.image_bpp;

        MSG("VID: output bits per pixel ***** %d\n", image_context.bitsPerPixel);

        // Set up image context values from the page description. These values are
        // used to manage the JBIG read and write DMA operations, and video
        // descriptors.
        image_context.resolutionX = page_wrapper->page_desc.image_info.image_dpi.x;
        image_context.resolutionY = page_wrapper->page_desc.image_info.image_dpi.y;

        image_context.line_width_in_bytes = page_wrapper->page_desc.image_info.image_data_width_in_bytes;
        image_context.page_height_in_lines = page_wrapper->page_desc.image_info.image_height_in_lines;

        MSG("%s line width = %d\n",__func__, image_context.line_width_in_bytes);

        // Start all the planes if this is an inline color printer
        if (vid_eng_config.video_processing == COLOR_INLINE)
        {
            for (i=0; i<vid_eng_config.num_print_colors; i++)
            {
                video_start_plane(page_wrapper, i);
            }
        }
        else
        {
            image_context.current_marking_color_channel = 0;
            video_start_plane(page_wrapper, image_context.current_marking_color_channel);
        }
    }
    else
    {
        SLOG_MSG( " video DO NOT start new page! nodeless!\n");
        image_context.video_active = false;
        SLOG_MSG(" video is **NOT** active!!!!\n");
    }

    DBG_SLOG_EXIT
}

static bool video_write_plane_data_to_jbig(COLOR process_color, laser_video_plane_wrapper_t* plane)
{
    void *flushed_ptr;

    ASSERT(process_color == plane->plane_desc.sysPlaneColor);

    flushed_ptr = dma_buffer_map_single( &plane->plane_desc.dma_buffer, DMA_TO_DEVICE );
#if 0	// del.lsh too much amount of kernel log 
    SLOG_MSG("%s: adding plane %#x color %d to JBIG strip data_len = %d, height: %d, width: %d, Lines remaining %d\n",
                 __func__, (uint32_t)plane, process_color, plane->plane_desc.DataLength, plane->plane_desc.image_info.image_height_in_lines,
                 image_context.line_width_in_bytes, image_context.lines_remaining_to_write_to_laser[process_color]);
#endif
    if (OK == jbig_decode_add_input_buffer(jbig_handle, flushed_ptr, plane->plane_desc.DataLength, plane))
    {
        return true;
    }
    else
    {
        dma_buffer_unmap_single(&plane->plane_desc.dma_buffer);
        return false;
    }
}

static error_type_t video_add_jbig_output_buffer( COLOR process_color, video_dma_buffer_t *dma_buffer, uint32_t num_lines, bool end_of_image)
{
    error_type_t result;
    uint32_t xfer_size = num_lines * image_context.line_width_in_bytes;
#if 0	// del.lsh too much amount of kernel log 
    SLOG_MSG("%s submit 0x%x num_lines = %d, Lines remaining %d\n", 
               __func__, 
               (uint32_t)dma_buffer,
               num_lines, 
               image_context.lines_remaining_to_write_to_laser[process_color]);
#endif 
    if (xfer_size > MAX_VIDEO_DMA_BUFFER_SIZE)
    {
        ASSERT(image_context.line_width_in_bytes > 0);
        SLOG_ERR("%s: ERROR: jbig output buffer size exceeded.  Max = %d, requested = %d.\n\tnum_lines = %d, line_width = %d, limiting to %d lines\n",
                 __func__, MAX_VIDEO_DMA_BUFFER_SIZE, xfer_size, num_lines, image_context.line_width_in_bytes, MAX_VIDEO_DMA_BUFFER_SIZE/image_context.line_width_in_bytes);
        xfer_size = MAX_VIDEO_DMA_BUFFER_SIZE/image_context.line_width_in_bytes;
    }

    // dbg_printf("\ndata 0x%x size of xfer %d, size in bytes %d,  ptr 0x%x lines remaining to decompress %d\n", ptr->data, ptr->size_of_transfer, ptr->size_in_bytes, ptr, image_context.lines_remaining_to_decompress[color])
    dma_buffer->process_color = process_color;
    dma_buffer->num_lines = num_lines;
    dma_buffer->data_len = xfer_size;
    dma_buffer->flags |= (end_of_image ? VIDEO_DMA_BUFFER_FLAG_EOI : 0);
    result = jbig_decode_add_output_buffer(jbig_handle, (void*)dma_buffer->buffer->hw_addr, xfer_size, dma_buffer);
    return result;
}


void video_configure_channel(uint32_t chan, laser_video_page_wrapper_t *page)
{
    const uint32_t *pcode = 0;

    // Setup the "video bender" default register. This establishes the base 
    // video frequency in the hardware. The frequency can be adjusted to
    // compensate for measured width errors; e.g. in color printers that
    // have color plane registration (CPR) calibration. 
    viddrv_program_pixel_freq_default(video_handle[chan], vid_eng_config.pixel_freq_in_hz[chan]);
    viddrv_set_resolution( video_handle[chan],
                           vid_eng_config.pixel_freq_in_hz[chan],
                           vid_eng_config.print_engine_native_resolution,
                           image_context.resolutionX,
                           image_context.resolutionY );

    // Set up beam manager to select and steer beams correctly. See hardware
    // documentation for selection of numerical steering value.
    // REVISIT videoSetHSyncSteering(chan, 0);

//-jrs            video_set_hsync_lockout(video_handle[chan], chan, vid_eng_config.hsync_lockout_period_in_hipwm_clocks);

    viddrv_set_page_bpp(video_handle[chan], image_context.bitsPerPixel);  

    // Setup the side and top margin registers. This needs to be
    // done for each video channel. The margins will be the same for
    // each beam for the given color.
    viddrv_program_side_margin( video_handle[chan], 
                                image_context.side_margin_in_pixels[chan],
                                image_context.side_margin_in_subpixels[chan]); 

    viddrv_program_top_margin( video_handle[chan], 
                               image_context.top_margin[chan]);


    // Setup the video line width.
    viddrv_set_line_width(video_handle[chan], image_context.line_width_in_bytes);

    // Select pulse codes for either threshold or error diffusion 
    // halftoning. They are independent.
    switch (image_context.bitsPerPixel)
    {
        case 1: 
            pcode = (page->page_desc.halftone_type == THRESHOLD_HALFTONE) ? vid_eng_config.pulse_code_1_bit_array : vid_eng_config.ed_pulse_code_1_bit_array;
            break;

        case 2: 
            pcode = (page->page_desc.halftone_type == THRESHOLD_HALFTONE) ? vid_eng_config.pulse_code_2_bit_array : vid_eng_config.ed_pulse_code_2_bit_array;
            break;

        case 4:
            pcode = (page->page_desc.halftone_type == THRESHOLD_HALFTONE) ? vid_eng_config.pulse_code_4_bit_array : vid_eng_config.ed_pulse_code_4_bit_array;
            break;

        default:
            ASSERT(0);
            break;
    }

    viddrv_set_pulse_code_regs( video_handle[chan], pcode );

    // Set video pulse code justification. Turn off forced 
    // justification in case it was turned on in the last page.
    viddrv_clear_forced_justification( video_handle[chan] );

    switch (page->page_desc.video_justification_mode)
    {
        case VIDEO_JUSTIFY_AUTO:
            viddrv_set_auto_justification(video_handle[chan], image_context.bitsPerPixel);
            break;

        case VIDEO_JUSTIFY_CENTERED:
            viddrv_set_centered_justification(video_handle[chan], image_context.bitsPerPixel);
            break;

        case VIDEO_JUSTIFY_PAIRED:

            // Set the forced justification setting for this print mode.
            // Initial and toggle settings will alternate late/early.
            viddrv_set_forced_justification( video_handle[chan],
                                             chan & 1,                               /* initial justification */
                                             vid_eng_config.num_beams_per_print_plane == 1       /* toggle */ );
            break;

        default:
            ASSERT(0);
            break;
    }
}

/**
 * \brief Start the video dma for all channels mapped to the specified color.
 *
 * \param process_color Color of print process to start.
 *
 * \return void
 */
static inline void video_start_dmas_for_plane_color(COLOR process_color)
{
    uint32_t beam;
    uint32_t chan = 0;

    for (beam=0; beam < vid_eng_config.num_beams_per_print_plane; beam++)
    {
        // Look up which video channel is configured for this color print
        // plane and laser beam.  
        chan = vid_eng_config.beam_to_video_hw_chan[process_color][beam];

        viddrv_dma_start_xfer(video_handle[chan]);
    }

    if(paperless_enabled)
    {
        for (beam=0; beam < vid_eng_config.num_beams_per_print_plane; beam++)
        {
            chan = vid_eng_config.beam_to_video_hw_chan[process_color][beam];
            viddrv_paperless_enable(chan, true);
        }
    }
}


/**
 * \brief Pass the buffer associated with the specfied plane to all 
 * video channels associated with the specified process color. 
 *
 * \param process_color Color of print process. 
 * \param plane Plane struct containing the buffer 
 * \param start_of_image Indicator that this is the first plane data in this image plane. 
 *
 * \return void
 */
void video_write_plane_data_to_viddrv(COLOR process_color, laser_video_plane_wrapper_t* plane, bool start_of_image)
{
    ASSERT(process_color == plane->plane_desc.sysPlaneColor);
    ASSERT(start_of_image == plane->plane_desc.start_of_image);  // TODO: we probably don't need explicit param; check assumption w/ assert

    dma_buffer_map_single( &plane->plane_desc.dma_buffer, DMA_TO_DEVICE );

    plane->plane_dma_context.buffer = &plane->plane_desc.dma_buffer;
    plane->plane_dma_context.data_len = plane->plane_desc.dma_buffer.len;
    plane->plane_dma_context.num_lines = plane->plane_desc.image_info.image_height_in_lines;
    plane->plane_dma_context.process_color = process_color;

    video_write_dma_buffer_to_viddrv(process_color, &plane->plane_dma_context, start_of_image, plane->plane_desc.end_of_image);
}

// May be called from either ISR or userlevel
void video_write_dma_buffer_to_viddrv(COLOR process_color, video_dma_buffer_t* dma_buf, bool start_of_image, bool end_of_image)
{
    uint32_t beam;
    uint32_t chan = 0;
    beam_direction_t beam_dir;
    bool reverse;
    void* user_data;
    uint32_t lockflags;

    ASSERT(dma_buf != NULL);
    ASSERT(dma_buf->buffer->hw_addr != 0);

    dros_smplock_acquire_isr(&video_smplock, &lockflags);
    image_context.lines_remaining_to_write_to_laser[process_color] -= dma_buf->num_lines;
    dros_smplock_release_isr(&video_smplock, lockflags);

    for (beam=0; beam < vid_eng_config.num_beams_per_print_plane; beam++)
    {
        // Look up which video channel is configured for this color print
        // plane and laser beam.  
        chan = vid_eng_config.beam_to_video_hw_chan[process_color][beam];
        beam_dir = vid_eng_config.beam_direction[chan];

        if(image_context.current_page->page_desc.top_to_bottom)
        {
            reverse =  beam_dir == LEFT_TO_RIGHT ? false : true ;
        }
        else
        {
            reverse =  beam_dir == LEFT_TO_RIGHT ? true : false ;
        }

        if (start_of_image)
        {
            SLOG_MSG("Setting SOI for chan %d\n", chan);
        }
#if 0	// del.lsh too much amount of kernel log 
        SLOG_MSG("%s: adding dma buffer %#x to channel %d SOI %d EOI %d strip height: %d, width: %d, Lines remaining %d\n",
                 __func__, (uint32_t)dma_buf, chan, start_of_image, end_of_image, dma_buf->num_lines,
                 image_context.line_width_in_bytes, image_context.lines_remaining_to_write_to_laser[process_color]);
#endif
        if ((dma_buf->num_lines / vid_eng_config.num_beams_per_print_plane) == 0)
        {
            SLOG_ERR("%s: ERROR: adding ZERO LEN dma buffer %#x to channel %d SOI %d EOI %d strip height: %d, width: %d, Lines remaining %d\n",
                     __func__, (uint32_t)dma_buf, chan, start_of_image, end_of_image, dma_buf->num_lines,
                     image_context.line_width_in_bytes, image_context.lines_remaining_to_write_to_laser[process_color]);
        }

        // Only attach the user data pointer to the master laser 
        // channel in a multiple laser configuration.
        user_data = vid_eng_config.report_video_event[beam] ? dma_buf : 0;

        viddrv_add_input_buffer( video_handle[chan],
                                (void*)dma_buf->buffer->hw_addr,
                                dma_buf->data_len,
                                image_context.line_width_in_bytes,
                                dma_buf->num_lines / vid_eng_config.num_beams_per_print_plane,
                                beam, 
                                user_data,
                                start_of_image,
                                end_of_image,
                                false,  // TODO: need to set blank here based on val in plane_desc!
                                reverse );
    } // for beam
    
    if (start_of_image)
    {
        video_start_dmas_for_plane_color(process_color);
    }
}

/*
 ** video_start_plane
 *
 *  PARAMETERS:  Page.  The page pointer to the data to xfer.
 *
 *  DESCRIPTION:  The video needs to be started.  Put the data on the lists and if the video
 *                is not running, get it running or start the jbig block de-compressing data.
 *                This function is called at the start of each print plane.  For mono pages
 *                will be called once per page.  For color pages it will be called multiple
 *                times (once for each color).
 * 
 *  RETURNS:
 *
 */
void video_start_plane (laser_video_page_wrapper_t *page, uint32_t marking_color_channel)
{
    uint32_t i;
    COLOR process_color;
    laser_video_plane_wrapper_t *plane = 0;

    DBG_SLOG_ENTRY

    process_color = vid_eng_config.color_process_order[page->page_desc.page_output_color_mode][marking_color_channel];

//-jrs    dbg_printf( "video_start_plane: marking_color_channel %d, process color %d, bpp %d\n", 
//-jrs                   marking_color_channel,
//-jrs                   process_color,
//-jrs                   image_context.bitsPerPixel);

    SLOG_MSG( "video_start_plane: marking_color_channel %d, process color %d, bpp %d\n", 
                   marking_color_channel,
                   process_color,
                   image_context.bitsPerPixel);

    // grab the head of the print plane data list for this plane; 
    // note that we never remove nodes from the actual list, we just traverse the list
    // with this pointer
    image_context.pNextPrintNode[process_color] = ATListHead(&page->planeDataPendingPrintList[process_color]);
    ASSERT(image_context.pNextPrintNode[process_color] != NULL);
    plane = CONTAINING_RECORD(image_context.pNextPrintNode[process_color], laser_video_plane_wrapper_t, node);
    ASSERT(plane);

    MSG("%s process color %d page 0x%x next print node 0x%x\n", __func__, process_color, (uint32_t)page, (uint32_t)image_context.pNextPrintNode[process_color]);
    SLOG_MSG("%s color: %d, next print NODE: 0x%x \n", __func__, process_color, (uint32_t)image_context.pNextPrintNode[process_color]);

    // Get the top and side margins for each video channel
    image_context.sub_pixel_denominator = vid_eng_config.sub_pixel_denominator;

    // configure and enable each active channel
    for (i = 0; i < MAX_NUM_VIDEO_CHAN; i++)
    {
        if((vid_eng_config.active_video_channels[i] == true) && (vid_eng_config.chan_to_print_plane_color[i] == process_color))
        {
            // set the margins    
            image_context.side_margin_in_pixels[i]    = page->page_desc.side_margin_in_subpixels[i] / image_context.sub_pixel_denominator;
            image_context.side_margin_in_subpixels[i] = page->page_desc.side_margin_in_subpixels[i] % image_context.sub_pixel_denominator;
            image_context.top_margin[i] = page->page_desc.top_margin_in_lines[process_color];
            
            MSG("%d side margin: %d+%d/%d, top margin: %d, resolution: %d pixels: %d\n", 
                                i, 
                                image_context.side_margin_in_pixels[i], 
                                image_context.side_margin_in_subpixels[i], 
                                image_context.sub_pixel_denominator, 
                                image_context.top_margin[i], 
                                page->page_desc.image_info.image_dpi.x,
                                page->page_desc.image_info.image_width_in_pixels);

            video_configure_channel(i, page);
            viddrv_enable_video(video_handle[i], true);
        }
    }

#if 0
    dbg_printf("%s plane 0x%x image info pixels: %d, bytes: %d, height: %d\n", 
               __func__,
                plane,
               plane->image_info.image_width_in_pixels,
               plane->image_info.image_data_width_in_bytes,
               plane->image_info.image_height_in_lines);
#endif

    image_context.lines_remaining_to_write_to_laser[process_color] = image_context.page_height_in_lines;

    switch (plane->plane_desc.compression_info.comp_type)
    {
        case COMPTYPE_NONE:
            {
                image_context.page_compression_mode = COMPTYPE_NONE;

                // Load up all the video DMA descriptors
                for (i=0; i<NUM_VIDEO_DMA_BUFFERS_PER_COLOR; i++)
                {
                    video_write_plane_data_to_viddrv(process_color, plane, (i == 0));  // last param is SOI; set on first buffer

                    SLOG_MSG("lines remaining RAW to write to laser %d\n", image_context.lines_remaining_to_write_to_laser[process_color]);

                    // Get the next data buffer in the plane
                    image_context.pNextPrintNode[process_color] = ATListNext( &page->planeDataPendingPrintList[process_color], 
                                                                             image_context.pNextPrintNode[process_color]);

                    // Get the next plane data
                    if (image_context.pNextPrintNode[process_color])
                    {
                        plane = CONTAINING_RECORD(image_context.pNextPrintNode[process_color], laser_video_plane_wrapper_t, node);
                        ASSERT(plane);
                    }
                    else
                    {
                        MSG("NO ADDITIONAL NODE %d\n ", i);
                        break;
                    }
                } // for num video buffers
            }
            break;

        case COMPTYPE_JBIG:
            {
                image_context.page_compression_mode = COMPTYPE_JBIG;

                // Determine if this plane has one jbig image per page. Each JBIG image
                // is processed in a jbig transaction, so if there are mulitple JBIG
                // images per page the processing flow is adjusted.
                if (plane->plane_desc.compression_info.comp_details.jbig.L0 == plane->plane_desc.compression_info.comp_details.jbig.Yd)
                {
                    // dbg_printf("\n >>>>>>>> MULTIPLE IMAGES PER PAGE ***********************\n\n");
                    image_context.jbig_images_per_page = JBIG_MULTIPLE_IMAGES_PER_PAGE;
                }
                else
                {
                    image_context.jbig_images_per_page = JBIG_ONE_IMAGE_PER_PAGE;
                }

                ASSERT(image_context.line_width_in_bytes == plane->plane_desc.compression_info.comp_details.jbig.Xd/8);

                //dbg_printf("*** input image width Xd %d %d %d\n", plane->compression_info.comp_details.jbig.Xd,
                //       image_context.input_image_width_in_bytes,
                //       image_context.line_width_in_bytes);

                // Start JBIG rolling!
                video_start_jbig(process_color);
            } 
            break;

        default:
            ASSERT(0);
            break;
    }

    SLOG_MSG( "video_start_plane: x res: %d  y res: %d  line width bytes: %d  lines in page: %d\n",
              image_context.resolutionX, image_context.resolutionY, image_context.line_width_in_bytes, image_context.page_height_in_lines);


    MSG("VM: Starting Video BPP = %d\n", page->page_desc.image_info.image_bpp);

    DBG_SLOG_EXIT
}

static void video_start_jbig_image_per_plane(COLOR process_color)
{
    error_type_t retval;
    laser_video_page_wrapper_t *page;
    laser_video_plane_wrapper_t *plane;
    video_dma_buffer_t* vid_dma_buf;

    page = image_context.current_page;

    if (page == 0)
        return; /// race condition check.

    if(image_context.pNextPrintNode[process_color] == 0)
    {
        SLOG_MSG("%s NO NEXT PRINT NODE!\n", __func__);
        return;
    }

    SLOG_MSG("%s\n", __func__);

    image_context.jbig_decode_running = true;

    // get the first strip.  The information from the first strip is used to configure the core
    plane = CONTAINING_RECORD(image_context.pNextPrintNode[process_color], laser_video_plane_wrapper_t, node);
    ASSERT(plane != NULL);

    // configure the core
    retval = jbig_decode_set_parms( jbig_handle,
                                    plane->plane_desc.compression_info.comp_details.jbig.Xd,
                                    plane->plane_desc.compression_info.comp_details.jbig.Yd,
                                    plane->plane_desc.compression_info.comp_details.jbig.L0,
                                    plane->plane_desc.compression_info.comp_details.jbig.option_flags );

    // setup input (read) dmas; we will add as many input buffers as jbig will accept
    while (image_context.pNextPrintNode[process_color])
    {
        plane = CONTAINING_RECORD(image_context.pNextPrintNode[process_color], laser_video_plane_wrapper_t, node);
        ASSERT(plane);

        ASSERT(COMPTYPE_JBIG == plane->plane_desc.compression_info.comp_type);

        if (!video_write_plane_data_to_jbig(process_color, plane))
        {
            break;  // jbig did not accept the buffer; we're done, do not advance nextPrintNode
        }
        // Get the next data buffer in the plane
        image_context.pNextPrintNode[process_color] = ATListNext( &page->planeDataPendingPrintList[process_color], 
                                                                 image_context.pNextPrintNode[process_color]);
    }
    
    // setup write dma; add as many output buffers as we have and jbig will accept
    while ((vid_dma_buf = video_get_buffer_from_block(&image_context.video_dma_block[process_color])) != NULL)
    {
        if(video_add_jbig_output_buffer( process_color, vid_dma_buf, MAX_LINES_PER_VIDEO_DMA_BUFFER, false ) != OK)
        {
            video_return_buffer_to_block(&image_context.video_dma_block[process_color], vid_dma_buf, false);
            break; // jbig did not accept the buffer; we're done
        }
    }

    // start the decode
    if (OK != jbig_decode_start_transaction( jbig_handle ))
    {
        image_context.jbig_decode_running = false;
        SLOG_ERR("jbig_start_transaction FAILED\n");
        ASSERT(0);
    }
}

static void video_run_jbig_image_per_plane(COLOR process_color)
{
    video_dma_buffer_t* vid_dma_buf;

    // Input buffers are loaded during IDMA complete in this mode so we just need to add the output
    // buffers back
    
    // setup write dma; add as many output buffers as we have and jbig will accept
    while ((vid_dma_buf = video_get_buffer_from_block(&image_context.video_dma_block[process_color])) != NULL)
    {
        if(video_add_jbig_output_buffer( process_color, vid_dma_buf, MAX_LINES_PER_VIDEO_DMA_BUFFER, false) != OK)
        {
            video_return_buffer_to_block(&image_context.video_dma_block[process_color], vid_dma_buf, false);
            break; // jbig did not accept the buffer; we're done
        }
    }
}

// run through active colors in round-robin order to find the next active color
// note that the next active color is allowed to be the same as the current active
// color
static inline bool video_get_next_active_jbig_decode_color(COLOR* active_color)
{
    COLOR next_color = image_context.current_jbig_color;    
    do
    {
        next_color = (next_color+1 <= COLOR_LAST) ? next_color+1 : COLOR_FIRST;
        if (image_context.jbig_color_is_active[next_color])
        {
            image_context.current_jbig_color = next_color;
            *active_color = next_color;
            return true;
        }        
    }while(next_color != image_context.current_jbig_color);
    return false;
}

static void video_run_jbig_image_per_strip()
{
    int i;
    error_type_t retval;
    laser_video_page_wrapper_t *page;
    laser_video_plane_wrapper_t *plane;
    video_dma_buffer_t* vid_dma_buf;
    COLOR process_color;

    page = image_context.current_page;

    if (page == 0)
        return; /// race condition check.

    if(image_context.jbig_decode_running)
    {
        // Decode is in progress.  This is a normal case
        // because jbig will be run for multiple colors and we try
        // to restart image per strip decodes from both the jbig odma
        // complete callback and from the video dma complete callback.
        // If in progress just return and wait for another event to
        // try again.
        SLOG_ERR("%s: in progress\n", __func__);
        return;
    }

    for (i = 0; i < MAX_MARKING_COLOR_CHANNELS; i++)
    {
        if(video_get_next_active_jbig_decode_color(&process_color))
        {
            // if we have data to decode for this color and there is an output buffer available
            if (image_context.pNextPrintNode[process_color] &&
                ((vid_dma_buf = video_get_buffer_from_block(&image_context.video_dma_block[process_color])) != NULL))
            {
                // get the strip
                plane = CONTAINING_RECORD(image_context.pNextPrintNode[process_color], laser_video_plane_wrapper_t, node);
                if (plane != NULL)
                {
                    ASSERT(COMPTYPE_JBIG == plane->plane_desc.compression_info.comp_type);

                    // configure the core
                    retval = jbig_decode_set_parms( jbig_handle,
                                                    plane->plane_desc.compression_info.comp_details.jbig.Xd,
                                                    plane->plane_desc.compression_info.comp_details.jbig.Yd,
                                                    plane->plane_desc.compression_info.comp_details.jbig.L0,
                                                    plane->plane_desc.compression_info.comp_details.jbig.option_flags );

                    if (!video_write_plane_data_to_jbig(process_color, plane))
                    {
                        ASSERT(0); // in image per strip mode jbig should always accept the input data
                    }

                    if (plane->plane_desc.image_info.image_height_in_lines <= 1)
                    {
                        SLOG_ERR("%s: ERROR: requesting decode of jbig strip with height %d, eoi = %d\n", __func__, plane->plane_desc.image_info.image_height_in_lines, plane->plane_desc.end_of_image);
                    }

                    if(video_add_jbig_output_buffer( process_color, vid_dma_buf, plane->plane_desc.image_info.image_height_in_lines, plane->plane_desc.end_of_image) != OK)
                    {
                        ASSERT(0); // in image per strip mode jbig should always accept the output buffer
                    }

                    // Advance to the next data buffer in the plane
                    image_context.pNextPrintNode[process_color] = ATListNext( &page->planeDataPendingPrintList[process_color], 
                                                                             image_context.pNextPrintNode[process_color]);

                    // start the decode
                    image_context.jbig_decode_running = true;
                    if (OK == jbig_decode_start_transaction( jbig_handle ))
                    {
                        break;
                    }
                    else
                    {
                        image_context.jbig_decode_running = false;
                        SLOG_ERR("jbig_start_transaction FAILED\n");
                        ASSERT(0);
                    }
                }
            }
        }
    }
}

/**
 * \brief Look for available data to decompress. Search all the color lists
 *  of pending data, and if found commence the JBIG decompression.
 *
 **/
static void video_start_jbig(COLOR process_color)
{
    image_context.jbig_color_is_active[process_color] = true;
    switch(vid_eng_config.video_processing)
    {
       case COLOR_INLINE:
           // assume multiple images/plane
           video_run_jbig_image_per_strip();

       case COLOR_MULTIPASS:
       case MONOCHROME:
           // may be either multiple or single image per plane
           if (image_context.jbig_images_per_page == JBIG_ONE_IMAGE_PER_PAGE)
           {
               video_start_jbig_image_per_plane(process_color);
           }
           else
           {
               video_run_jbig_image_per_strip();
           }
    }
}


static void imageRAW(COLOR process_color)
{
    laser_video_page_wrapper_t *page;
    laser_video_plane_wrapper_t *plane;

    page = image_context.current_page;

    if (page == 0 )
        return; /// race condition check.

//    DBG_SLOG_ENTRY  

    // Look at the pending plane data list to see if there is more data 
    // available for this color.
    SLOG_MSG( "%s: process_color: %d next print NODE: 0x%x \n", __func__, process_color, (uint32_t)image_context.pNextPrintNode[process_color]);

    if (image_context.pNextPrintNode[process_color])
    {
        plane = CONTAINING_RECORD(image_context.pNextPrintNode[process_color], laser_video_plane_wrapper_t, node);
        ASSERT(plane);

        ASSERT(COMPTYPE_NONE == plane->plane_desc.compression_info.comp_type);

        if ( COMPTYPE_NONE == plane->plane_desc.compression_info.comp_type )
        {
            video_write_plane_data_to_viddrv(process_color, plane, false);  // last param is SOI

            MSG("%s lines remaining %d color %d image height %d\n", __func__, 
                image_context.lines_remaining_to_write_to_laser[process_color], process_color, plane->plane_desc.image_info.image_height_in_lines);

            // Get the next data buffer in the plane
            image_context.pNextPrintNode[process_color] = ATListNext( &page->planeDataPendingPrintList[process_color], 
                                                                     image_context.pNextPrintNode[process_color]);
        }
    }
//    DBG_SLOG_EXIT
}
 
/* This is the callback function for JBIG events including read DMA completion, 
 * write DMA completion, and end of image events. It is executed in thread 
 * context - the JBIG driver thread.
 *
 * Note that for JBIG decode operations of a JBIG image consisting of one
 * stripe, the input DMA completion event is reported first, then the decode
 * complete is reported. This file only uses JBIG for decode or decompression,
 * so when the JBIG_CONTROL_FLAG is set below, it means that the decode
 * operation is complete.
 */
static void video_jbig_event_callback(jbig_decode_handle_t* handle, void* context, uint32_t status)
{
#if 0	// del.lsh too much amount of kernel log 
    DBG_SLOG_ENTRY
#endif
    if(status != 0)
    {
        SLOG_ERR("JBIG image completion ERROR status %#x\n", status);
    }
    else
    {
#if 0	// del.lsh too much amount of kernel log 
        SLOG_MSG("JBIG image complete status %#x\n", status);
#endif
    }
}

// Functions for host based print to provide support for DMA event callbacks
// Called back at ISR level from JBIG Driver
static void video_jbig_odma_complete( jbig_decode_handle_t* handle, void* context )
{
    video_msg_param_t vid_msg_param;
    video_dma_buffer_t* dma_buffer = (video_dma_buffer_t*)context;
    COLOR process_color;
    bool start_of_image = false;
    bool end_of_image = false;
#if 0	// del.lsh too much amount of kernel log 
    DBG_SLOG_ENTRY;
#endif
    ASSERT(dma_buffer != NULL);

    process_color = dma_buffer->process_color;

    dros_smplock_acquire(&video_smplock);
    if (image_context.lines_remaining_to_write_to_laser[process_color] == image_context.page_height_in_lines)
    {        
        start_of_image = true;
    }
    else if ((image_context.jbig_images_per_page == JBIG_ONE_IMAGE_PER_PAGE) && (image_context.lines_remaining_to_write_to_laser[process_color] <= dma_buffer->num_lines))
    {
        // handle eoi for image per page mode.  In image per page mode the dma buffer does not
        // have the eoi info because there is not a 1-to-1 correspondance between input and output
        // buffers

        // at end of page, so force to true number of lines remaining; in image per page mode num lines is always
        // initialized to max allowed per strip, so we need to reset to correct value at eoi
        dma_buffer->num_lines = image_context.lines_remaining_to_write_to_laser[process_color];
        end_of_image = true;
    }
    else if (dma_buffer->flags & VIDEO_DMA_BUFFER_FLAG_EOI)
    {
        // handle eoi for image per strip mode.  In image per strip mode there is a 1-to-1 correspondance between
        // input and output and the num lines is set to the correct value so we only need to pass along eoi.
        end_of_image = true;
    }
    dros_smplock_release(&video_smplock);

    if (dma_buffer->num_lines <= 1)
    {
        SLOG_ERR("%s: ERROR: decoded strip with %d lines; lines remaining to write to laser %d color %d soi = %d, eoi = %d\n", 
                 __func__, dma_buffer->num_lines, image_context.lines_remaining_to_write_to_laser[process_color], process_color, start_of_image, end_of_image);
    }

    // give the buffer to video
    video_write_dma_buffer_to_viddrv(process_color, dma_buffer, start_of_image, end_of_image);
#if 0	// del.lsh too much amount of kernel log 
    SLOG_MSG("%s: lines remaining to write to laser %d color %d soi = %d, eoi = %d\n", 
             __func__, image_context.lines_remaining_to_write_to_laser[process_color], process_color, start_of_image, end_of_image);
#endif
    if (image_context.jbig_images_per_page == JBIG_MULTIPLE_IMAGES_PER_PAGE)
    {
        vid_msg_param.jbig_odma_done_color = process_color;
        video_post_message( VIDEO_MSG_JBIG_ODMA_BUFFER_DONE, &vid_msg_param );
    }
#if 0	// del.lsh too much amount of kernel log 
    DBG_SLOG_EXIT
#endif
}

static void video_jbig_idma_complete( jbig_decode_handle_t* handle, void* context )
{
    video_msg_param_t vid_msg_param;
#if 0	// del.lsh too much amount of kernel log 
    SLOG_MSG("%s\n", __func__);
#endif
    ASSERT(context);

    vid_msg_param.jbig_idma_done_plane = (laser_video_plane_wrapper_t*)context;
    video_post_message( VIDEO_MSG_JBIG_IDMA_BUFFER_DONE, &vid_msg_param );
}


static void video_release_plane_buffers(laser_video_page_wrapper_t *page, COLOR plane_color)
{
    ATLISTENTRY *myNode;
    laser_video_plane_wrapper_t* plane_wrapper;

    if(page == NULL)
    {
        ASSERT(page != NULL);
        return;
    }

//-jrs    video_enable_all_chan_interrupts(false);
    while (0 != (myNode = ATRemoveHeadList(&page->planeDataPendingPrintList[plane_color])))
    {
        plane_wrapper = CONTAINING_RECORD(myNode, laser_video_plane_wrapper_t, node);
        memFree(plane_wrapper);
    }
//-jrs    video_enable_all_chan_interrupts(true);

}


// Perform processing at the end of printing a color plane. This includes the
// following:
//  > Releasing buffers back into the memory pool.
//  > For multipass color printers, move the video idle list nodes to the next
//    color plane list. Multipass printers share the video descriptors and 
//    associated data buffers, but maintain separate video and jbig lists.
//  > Make sure the JBIG lists are in order.
// 
void videoProcEndOfPlane(uint32_t process_color, uint32_t plane_completion_cause)
{
    SLOG_MSG("%s ENTRY process_color %d, page_lines %d, remaining %d\n", __func__, process_color, image_context.page_height_in_lines, image_context.lines_remaining_to_write_to_laser[process_color]);

    if(image_context.colorCompleted[process_color] != 0)
    {
        // plane already completed.  sometime we get multiple completions in 
        // error case due to multiple indications from video driver such as
        // both link underrun and premature end of image.  we only want to
        // do end of plane processing once per plane though.
        SLOG_INFO("%s discarding redundant notification.  orig = %#x, current = %#x\n", __func__, image_context.colorCompleted[process_color], plane_completion_cause );
        return;
    }

    image_context.planes_printed++;
    image_context.colorCompleted[process_color] = plane_completion_cause;  // store non-zero completion cause; 0 means plane not complete.
    image_context.jbig_color_is_active[process_color] = false;

    if ((image_context.jbig_images_per_page == JBIG_ONE_IMAGE_PER_PAGE) && image_context.jbig_decode_running)
    {
        SLOG_MSG("%s: stopping JBIG\n", __func__);
        jbig_decode_stop_transaction(jbig_handle);
        image_context.jbig_decode_running = false;
    }

    video_release_plane_buffers(image_context.current_page, process_color);

    // on multipass advance to the next marking channel
    if (COLOR_MULTIPASS == vid_eng_config.video_processing)
    {
        image_context.current_marking_color_channel = 
            (image_context.current_marking_color_channel + 1) % vid_eng_config.num_print_planes;
    }

    DBG_SLOG_EXIT
}

static void video_release_page(laser_video_page_wrapper_t* page)
{
    int i;
    ASSERT(page != NULL);
    
    // plane data lists should be empty before we free the page
    for(i = 0; i < MAX_MARKING_COLOR_CHANNELS; i++)
    {
        ASSERT(ATIsListEmpty(&page->planeDataPendingPrintList[i]));
    }

    memFree(page);
}

static void video_cancel_queued_pages(void)
{
    laser_video_page_wrapper_t *page_wrapper;
    ATLISTENTRY* node;
    int i;

    while((node = ATRemoveHeadList(&video_page_list)) != NULL)
    {
        page_wrapper = CONTAINING_RECORD(node, laser_video_page_wrapper_t, node);

        SLOG_MSG( "%s: cancelling page 0x%x\n", __func__, (uint32_t)page_wrapper);

        // free all planes
        for(i = 0; i < MAX_MARKING_COLOR_CHANNELS; i++)
        {
            video_release_plane_buffers(page_wrapper, i);
        }

        // notify page complete
        if (end_of_page_callback) 
        {
            // complete the page with cancelled status
            (end_of_page_callback)(end_of_page_callback_context, page_wrapper->page_desc.page_context, VIDCTL_PAGE_STATUS_CANCELLED);
        }

        // release the page
        video_release_page(page_wrapper);
    }
}

void videoProcEndOfPage( uint32_t video_event )
{
    laser_video_page_wrapper_t *page;
    uint32_t chan;
    uint32_t page_completion_status;
    error_type_t status;
    int i;

    page = image_context.current_page;

    MSG("VIDEO %s !!!!!\n", __func__);

    SLOG_MSG("%s ENTRY\n", __func__);

    // stop jbig if needed
    if ((video_event == VIDEO_PAGE_CANCEL) || image_context.jbig_decode_running)
    {
        if (video_event != VIDEO_PAGE_CANCEL)
        {
            SLOG_MSG("%s: WARNING: JBIG STILL RUNNING AT END OF PAGE!\n", __func__);
        }
        jbig_decode_stop_transaction(jbig_handle);
        image_context.jbig_decode_running = false;
    }

    SLOG_MSG("jbig_close 0x%x\n", (uint32_t)jbig_handle);
    status = jbig_decode_close(jbig_handle);
    jbig_handle = NULL;

    if (video_event == VIDEO_PAGE_CANCEL)
    {
        // cancel video
        viddrv_cancel();
    }

    // Close all the active video channels for the given configuration.  
    for (chan=0; chan < MAX_NUM_VIDEO_CHAN; chan++)
    {
        if (vid_eng_config.active_video_channels[chan] == true)
        {
            if(paperless_enabled)
            {
                viddrv_paperless_enable(chan, false);
            }
            viddrv_query_pixel_count( video_handle[chan], dot_count, NULL );

//            MSG("Page(%d) CRC = 0x%x\n", chan, viddrv_paperless_get_CRC(chan));
//            ERR("Page(%d) chan(%d) CRC = 0x%x\n", page->page_desc.page_id, chan, viddrv_paperless_get_CRC(chan));

            viddrv_close( video_handle[chan] );
            MSG("closed channel %d, handle 0x%x\n", chan, (uint32_t)&video_handle[chan]);
            video_handle[chan] = 0;
        }
    }

    switch ( video_event )
    {
        case VIDEO_EVENT_LINK_OR_BUS_UNDERRUN:
        case VIDEO_EVENT_OWNERSHIP_ERROR:
        case VIDEO_EVENT_PREMATURE_END_OF_PAGE:
            ERR("%s page %#x finished with ERROR 0x%x\n", __func__, (uint32_t)page, video_event);
            // free all planes
            for(i = 0; i < MAX_MARKING_COLOR_CHANNELS; i++)
            {
                video_release_plane_buffers(page, i);
            }
            page_completion_status = VIDCTL_PAGE_STATUS_ERROR;
            break;

        case VIDEO_EVENT_END_OF_IMAGE:
            MSG("%s page finished normally 0x%x\n", __func__, (uint32_t)page);
            page_completion_status = VIDCTL_PAGE_STATUS_OK;
            break;

        case VIDEO_PAGE_CANCEL:
            MSG("%s PAGE CANCEL 0x%x\n", __func__, (uint32_t)page);
            // free all planes
            for(i = 0; i < MAX_MARKING_COLOR_CHANNELS; i++)
            {
                video_release_plane_buffers(page, i);
            }
            page_completion_status = VIDCTL_PAGE_STATUS_CANCELLED;
            break;

        default:
            XASSERT(0, video_event);
            page_completion_status = VIDCTL_PAGE_STATUS_ERROR;
            break;
    }

    // REVISIT doVideoPixelCounting( image_context.current_page, lastColor );

    // Video is done on this page.  
    image_context.video_active = false;
    SLOG_MSG(" video is **NOT** active!!!!\n");

    image_context.current_page = 0;

    if (end_of_page_callback) 
    {
        (end_of_page_callback)(end_of_page_callback_context, page->page_desc.page_context, page_completion_status);
    }

    video_release_page(page);

    if (video_event == VIDEO_PAGE_CANCEL)
    {
        video_cancel_queued_pages();
    }

    // start the next page if any
    video_start_next_page();

    DBG_SLOG_EXIT
}


/// This is how the engine knows video is done.
/// It is used to miss-match physical paper size vs video "paper" length 
/// for example: A4 image, on A5 paper loaded in the manual tray.
/// Engine will then tell print manager video done or an error.
///
int vidctl_register_page_done_callback(vidctl_page_done_callback_t func, void* context)
{
    end_of_page_callback = func; 
    end_of_page_callback_context = context;
    return OK;
}

/* **************** L O C A L  S T A T I C  F U N C T I O N S *************** */


static void video_dma_buffer_complete_callback( void *handle, void* context )
{
    video_msg_param_t vid_msg_param;

    ASSERT(context);

    vid_msg_param.video_dma_done_param = (video_dma_buffer_t*)context;
    video_post_message( VIDEO_MSG_VIDEO_DMA_BUFFER_DONE, &vid_msg_param );
}

// This function is called back by the video driver in ISR context.
static void viddrv_event_handler(video_handle_t *handle,  
                                uint32_t video_event, 
                                void* context)
{
    uint32_t chan;
    video_msg_param_t msg_param;
    COLOR plane_color;

    DBG_SLOG_ENTRY

    chan = (uint32_t)context;

    plane_color = vid_eng_config.chan_to_print_plane_color[chan];
   
    MSG("%s: ENTRY chan %d, color %d, event %#x\n", __func__, chan, plane_color, video_event);
    SLOG_MSG("%s: ENTRY chan %d, color %d, event %#x\n", __func__, chan, plane_color, video_event);    

    msg_param.page_done_params.chan = chan;
    msg_param.page_done_params.video_event = video_event;

    switch (video_event)
    {
        case VIDEO_EVENT_END_OF_IMAGE:
            MSG("EOI (end of image) VIDEO event\n" );
            SLOG_MSG("EOI (end of image) event!\n");
            break;

        case VIDEO_EVENT_LINK_OR_BUS_UNDERRUN:
            SLOG_MSG("link or bus underrun!\n");
            MSG("video link or bus underrun event\n");
            if (image_context.jbig_decode_running && jbig_handle)
            {
                jbig_dump_all_regs(jbig_handle);
            }
            break;

        case VIDEO_EVENT_OWNERSHIP_ERROR:
            SLOG_MSG("video descriptor ownership error!\n");
            MSG("video ownership error event\n");
            break;

        case VIDEO_EVENT_PREMATURE_END_OF_PAGE:
            SLOG_MSG("premature end of page!\n");
            MSG("premature end of page\n");

            break;

        default:
            XASSERT(0, video_event);
            break;

    }

    // Always post the process end of plane message.
    video_post_message( VIDEO_MSG_PROC_END_PLANE, &msg_param);
}

