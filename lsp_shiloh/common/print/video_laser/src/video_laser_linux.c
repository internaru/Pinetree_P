/** 
 *
 * 
 * ============================================================================
 * Copyright (c) 2013-2014 Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *  Description:
 *  This file contains the application level interface for sending raw
 *  or jbig compressed laser video page data to the video driver to be
 *  imaged on the laser print mechanism.
 * 
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>

#include "lassert.h"
#include "logger.h"
#include "print_job_types.h"
#include "list.h"
#include "JobMgr.h"
#include "sys_init_api.h"
#include "memAPI.h"
#include "dma_buffer.h"

#include "video_laser_api.h"
#include "laser_video_chardev_api.h"
#include "laser_video_types.h"
#include "laser_engine_config_api.h"

//#include "text_pad_api.h"
#include "print_mode_api.h"


/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define DBG_PRFX "VIDEO_LASER: "
#define LOGGER_SUBMOD LOGGER_SUBMODULE_BIT( 2 )
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_VIDEO | LOGGER_SUBMOD

#if 1 // printf using video module controls
#  define ERR DBG_PRINTF_ERR
#  define INFO DBG_PRINTF_INFO
#  define MSG DBG_PRINTF_DEBUG
#  define NOTICE DBG_PRINTF_NOTICE
#else // printf using dprintf module controls so that printf and log controls differ.
#  define ERR(...) DPRINTF(DBG_LOUD|DBG_OUTPUT, (DBG_PRFX __VA_ARGS__))
#  define MSG(...) DPRINTF(DBG_LOUD|DBG_OUTPUT, (DBG_PRFX __VA_ARGS__))
#endif

#define SLOG_ERR DBG_MEMLOG_ERR
#define SLOG_MSG DBG_MEMLOG_INFO
#define SLOG_INFO DBG_MEMLOG_INFO

#define DBG_SLOG_ENTRY DBG_MEMLOG_DEBUG( "%s <<<==== ENTRY\n", __FUNCTION__);
#define DBG_SLOG_EXIT  DBG_MEMLOG_DEBUG( "%s ====>>> EXIT\n", __FUNCTION__);
                       
/** local definitions **/

#define VIDCTL_PAGE_STATUS_OK            0  ///< page completed with no errors
#define VIDCTL_PAGE_STATUS_CANCELLED    -1  ///< page cancelled by uppper level SW request
#define VIDCTL_PAGE_STATUS_ERROR        -2  ///< page completed with error; image not fully printed

/** private video messages **/
//#define VIDEO_MSG_PROC_END_PAGE         ((AGMSG)(MSG_LAST_MSG + 1))
#define VIDEO_MSG_PROC_END_PLANE        ((AGMSG)(MSG_LAST_MSG + 2))
//#define VIDEO_MSG_REFRESH_WRITE_DMA     ((AGMSG)(MSG_LAST_MSG + 3))
#define MSG_VIDEO_RAW_BUFFER_DONE       ((AGMSG)(MSG_LAST_MSG + 4))
#define MSG_VIDEO_DATA_BUFFER_COMPLETE  ((AGMSG)(MSG_LAST_MSG + 5))
#define VIDEO_MSG_RELEASE_INPUT_DATA    ((AGMSG)(MSG_LAST_MSG + 6))
#define VIDEO_MSG_JBIG_IMAGE_COMPLETE            ((AGMSG)(MSG_LAST_MSG + 7))

#define VIDEO_DEV_NAME "/dev/lvc0"
#define VIDEO_PAPERLESS_SYSFS_NAME "/sys/devices/platform/laser_video_chardev.0/paperless"

/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */

typedef struct _video_page_container
{
    ATLISTENTRY node;
    PAGE_DATA* page;
} video_page_container_t;



/* -------------------------------------------------------------------------- */
/*                   F O R W A R D  D E C L A R A T I O N S                   */
/* -------------------------------------------------------------------------- */

static void* video_thread( void* value );

/* -------------------------------------------------------------------------- */
/*               P R I V A T E  M O D U L E  V A R I A B L E S                */
/* -------------------------------------------------------------------------- */
static int video_driver_fd = -1;

static laser_engine_video_config_t *config = 0;
static pthread_mutex_t video_mutex = POSIX_MUTEX_INITIALIZER;

// Video thread variables and defines
static pthread_t video_thread_id;
#define STACK_SIZE POSIX_MIN_STACK_SIZE
static unsigned char video_stack[STACK_SIZE] ALIGN8;
//static uint32_t* dot_count;
static uint32_t dot_count;

// Maintain a list of pending pages.
static MLISTENTRY videoPageList;


video_laser_pagedone_callback_t end_of_page_callback = NULL;

/** public functions **/

/// Open and return the file descriptor associated with the video driver.
static inline int video_open_driver()
{
    // open driver
    return open(VIDEO_DEV_NAME, O_RDWR|O_CLOEXEC);
}

/// Close the video driver file desc.
static inline void video_close_driver()
{
    // close driver
    close(video_driver_fd);
    video_driver_fd = -1;
}

static int set_vid_eng_config(int video_fd, laser_engine_video_config_t *vid_eng_config)
{
    int retval = 0;

    ASSERT(video_fd > 0);

    // send ioctl
    if((retval = ioctl(video_fd, LVC_IOCTL_SET_CONFIG, vid_eng_config)) != 0)
    {
        ERR("%s 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_SET_CONFIG, errno);
    }    

    return retval;
}

static int get_vid_pixel_count(int video_fd, PAGE_DATA* page)
{
    MESSAGE msg;
    int retval = 0;
    int i = 0;
    uint32_t non_white_pixels[MAX_NUM_VIDEO_CHAN] = {0, };

    ASSERT(video_fd > 0);
    if((retval = ioctl(video_fd, LVC_IOCTL_QUERY_PIXEL_COUNT, non_white_pixels)) != 0)
    {
        ERR("%s 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_QUERY_PIXEL_COUNT, retval);
    }
    MSG("Dot count[0] : %d\tDot count[1] : %d\n", non_white_pixels[0], non_white_pixels[1]);

    //*dot_count = 0;
    dot_count = 0;
    for(i = 0; i < MAX_NUM_VIDEO_CHAN; i++){
        //*dot_count += non_white_pixels[i];
        dot_count += non_white_pixels[i];
    }
    //MSG("Total Dot count : %d\n", *dot_count);
    MSG("Total Dot count : %d\n", dot_count);
    msg.msgType = MSG_SUPPLIES_PAGE_DONE;
    msg.param1 = 0;
    //msg.param2 = 0;
    //msg.param3 = dot_count;
    msg.param2 = dot_count;
    msg.param3 = page;
    SYMsgSend(PRINTMGRID, &msg);

    return retval;
}
 
void video_thread_init (void)
{
    error_type_t errcode;
    MSG("video_thread_init called\n");
    
    // Initialize the video page list
    MInitList(&videoPageList);

    // All the video configuration depends on this - do it first!!!
    errcode = engine_get_video_config(&config);
    ASSERT(errcode == OK && config != NULL);

    video_driver_fd = video_open_driver();
    if(video_driver_fd < 0)
    {
        ERR("%s: FAILED TO OPEN DRIVER.  Video disabled! error = %d\n", __func__, errno);
    }
    else
    {
        if(set_vid_eng_config(video_driver_fd, config) != 0)
        {
            ERR("%s: FAILED TO CONFIG DRIVER.  Video disabled! error = %d\n", __func__, errno);
            video_close_driver();
        }
        else
        {
            posix_create_thread(&video_thread_id, video_thread, 0, "laser_video", video_stack, STACK_SIZE, POSIX_THR_PRI_IMAGE);    
        }
    }
    engine_setup_pcode_debug_commands();

    // for pixel count
    //dot_count = (uint32_t*)MEM_MALLOC(sizeof(uint32_t));
    //ASSERT(dot_count != NULL);;
}

/**
 * \brief Blocking wait for page completion by the kernel driver. 
 *  
 * \param page_done_info[OUT] On return buffer will contain the completion status of the page. 
 */
static int video_wait_page_done(int fd, page_done_notify_t* page_done_info)
{
    ssize_t bytes_read;

    bytes_read = read(fd, page_done_info, sizeof(page_done_notify_t));
    if(bytes_read < 0)
    {
        ERR("%s: read error: errno = %d\n", __func__, errno);
        return -1;
    }
    if(bytes_read != sizeof(page_done_notify_t))
    {
        ERR("%s: wrong num bytes read.  expected: %d, read %d\n", __func__, bytes_read, sizeof(page_done_notify_t));
        return -1;
    }
    NOTICE("%s: page done.  status = %d, context = %#x\n", __func__, page_done_info->page_status, page_done_info->page_context);
    return 0;
}

/**
 * \brief Translate driver level page status to video_laser_api error_type_t status.
 */
static error_type_t video_translate_page_status(uint32_t page_status)
{
    error_type_t page_err_type = VIDEO_PAGE_STATUS_ERROR;
    switch (page_status)
    {
        case VIDCTL_PAGE_STATUS_OK:
            page_err_type = VIDEO_PAGE_STATUS_OK;
            break;
        case VIDCTL_PAGE_STATUS_CANCELLED:
            page_err_type = VIDEO_PAGE_STATUS_CANCELLED;
            break;
        case VIDCTL_PAGE_STATUS_ERROR:
            page_err_type = VIDEO_PAGE_STATUS_ERROR;
            break;
        default:
            XASSERT(0 == "Unknown Page Completion Status", page_status);
            break;
    }
    return page_err_type;
}

/**
 * \brief Update buffer use counts and notify of completion for all planes;
 *  call page done notification if any.
 */
static void video_complete_page(PAGE_DATA* page, uint32_t page_status)
{
    PLANED* plane;
    ATLISTENTRY* plane_node;
    int plane_index;
    bool plane_released;
    
    INFO("%s: page @ %#x page %d of job %d. Status = %d\n", __func__, (uint32_t)page, page->PageCount, page->pMyJob->JobNum, video_translate_page_status(page_status));
        
    ASSERT(IS_PAGE_DATA(page));
        
    // for each PLANED decrement the use count and notify the job manager to free the memory
    for (plane_index = 0; plane_index < NUM_MARKING_COLOR_CHANNELS; plane_index++)
    {
        plane_released = false;
        plane_node = MListHead(&page->planeDataPendingPrintList[plane_index]);
        while(plane_node != NULL)
        {
            plane = CONTAINING_RECORD(plane_node, PLANED, listNode);
            ASSERT(plane != NULL);
            ASSERT(IS_PLANED(plane));

            plane->UseCount--;
            plane_released = true;

            plane_node = MListNext(&page->planeDataPendingPrintList[plane_index], plane_node);
        }
        if(plane_released)
        {
            job_mgr_free_memory((COLOR)plane_index);
        }
    }

    get_vid_pixel_count(video_driver_fd, page);

    // call page done callback with completion status.
    if(end_of_page_callback != NULL)
    {
        end_of_page_callback(page, video_translate_page_status(page_status));
    }
}

static void* video_thread(void* value)
{
    page_done_notify_t page_done_info;
    video_page_container_t* page_container;

    // Wait for system init to finish
    SysWaitForInit();

    MSG("%s: starting\n", __func__);

    while (video_driver_fd >= 0 && !video_wait_page_done(video_driver_fd, &page_done_info))
    {
	page_container = page_done_info.page_context;
	
        if(!page_container || page_container->page == 0 )
        {
            ERR("%s: Dropping Page with context %#x. Unable to locate on videoPageList!\n", __func__, page_done_info.page_context );
            ASSERT(page_container && page_container->page);
            ASSERT(0);
            continue;
        }
        // locate completed page on list.  Should be head of list, but...
        MRemoveEntryList(&videoPageList, &page_container->node);
        // update buffer use counts and notify of completion for all planes
        // call page done notification if any
        video_complete_page(page_container->page, page_done_info.page_status);

        // free the page container
        MEM_FREE_AND_NULL(page_container);
    }
    // free the variable of dot_count
    //MEM_FREE_AND_NULL(dot_count);

    ERR("%s: EXITING.  No more video page done notifications for you!\n");
    return 0;
}

/**
 * \brief Validate that the actual page strips match the specified dimensions of the page
 * 
 * \param page Page to be sent to be validated
 */
void video_validate_page(PAGE_DATA* page)
{
//#define VIDEO_DEBUG
#ifdef VIDEO_DEBUG
    int i;
    ATLISTENTRY* plane_node;
    PLANED* plane;
    bool EOI;

    uint32_t line_count;

    for (i = 0; i < NUM_MARKING_COLOR_CHANNELS; i++)
    {
        line_count = 0;
        EOI = false;
        plane_node = MListHead(&page->planeDataPendingPrintList[i]);
        while(plane_node != NULL)
        {
            plane = CONTAINING_RECORD(plane_node, PLANED, listNode);
            ASSERT(plane != NULL);
            ASSERT(IS_PLANED(plane));

//            ASSERT(plane->image_info.image_width_in_pixels == page->image_output_info.image_width_in_pixels);
            ASSERT(plane->image_info.image_data_width_in_bytes == page->image_output_info.image_data_width_in_bytes);
            ASSERT(plane->image_info.image_bpp == page->image_output_info.image_bpp);
            ASSERT(plane->image_info.image_dpi.x == page->image_output_info.image_dpi.x);
            ASSERT(plane->image_info.image_dpi.y == page->image_output_info.image_dpi.y);

            line_count += plane->image_info.image_height_in_lines;
            plane_node = MListNext(&page->planeDataPendingPrintList[i], plane_node);
        }

        if (line_count != page->image_output_info.image_height_in_lines)
        {
            ERR("%s: Line count mismatch plane %d, expected %d, received %d\n", __func__, i, page->image_output_info.image_height_in_lines, line_count);
        }
        if (!plane->LastData)
        {
            ERR("%s: EOI not set on last strip\n", __func__);
        }
    }

#endif
}

/**
 * \brief Send the page start notification and page header to the video driver.  
 * 
 * \param page Page to be sent to driver
 * \param context Opaque context value passed back in page done notification from the driver.
 *  
 * \return void
 */  
static void video_write_page_header(PAGE_DATA* page, void* context)
{
    laser_video_page_t vid_page;
    int i;
    int retval;

    // setup the page structure
    vid_page.page_id = page->PageCount;
    vid_page.page_context = context;

    // image_info
    vid_page.image_info = page->image_output_info;

    vid_page.page_output_color_mode = page->page_output_color_mode;
    vid_page.video_justification_mode = page->video_justification_mode;
    vid_page.top_to_bottom = !((ROTATE_DIR)GetRotation(page->DUPLEX) == e_Rotate180); 
    vid_page.halftone_type = print_mode_get_halftone( page->print_mode );

    // Set side margins
    for (i = 0; i < NUM_VIDEO_CHAN; i++)
    {
        if(config->active_video_channels[i] == true)
        {

			if(page->nup_image_params.num_images < 2)
			{
	            vid_page.side_margin_in_subpixels[i] = 
    	            engine_get_side_margin_in_subpixels( page,
                                                     i, 
                                                     page->image_output_info.image_width_in_pixels, 
                                                     page->image_output_info.image_dpi.x); 
            }
            else
            {
	            vid_page.side_margin_in_subpixels[i] = 
    	            engine_get_side_margin_in_subpixels( page,
                                                     i, 
                                                     page->nup_image_params.image_width, 
                                                     page->image_output_info.image_dpi.x); 
			}
        }
    }

    // set top margins
    for(i = 0; i < NUM_MARKING_COLOR_CHANNELS; i++)
    {
        vid_page.top_margin_in_lines[i] = engine_get_top_margin_in_lines(page, (COLOR)i);
    }

    set_laser_video_page_cookie(&vid_page);

    // send page start
    if((retval = ioctl(video_driver_fd, LVC_IOCTL_START_PAGE, &vid_page)) != 0)
    {
        ERR("%s IOCTL_START_PAGE 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_START_PAGE, errno);
        ASSERT(0);
    } 
}

/**
 * \brief Send the print plane data to the video driver.  
 * 
 * \param page Page to be sent to driver.
 *  
 * \return void
 */  
static void video_write_plane_data(PAGE_DATA* page)
{
    int errcode;
    int i;
    bool plane_start;
    laser_video_plane_t vid_plane;
    ATLISTENTRY* plane_node;
    PLANED* plane;

    for (i = 0; i < NUM_MARKING_COLOR_CHANNELS; i++)
    {
        plane_start = true;
        plane_node = MListHead(&page->planeDataPendingPrintList[i]);
        while(plane_node != NULL)
        {
            plane = CONTAINING_RECORD(plane_node, PLANED, listNode);
            ASSERT(plane != NULL);
            ASSERT(IS_PLANED(plane));

            vid_plane.sysPlaneColor = plane->sysPlaneColor;
            vid_plane.DataLength = plane->DataLength;
            vid_plane.end_of_image = plane->LastData;
            vid_plane.start_of_image = plane_start;
            plane_start = false;

            // the image height in lines must be an even multiple of the number of beams per plane
            // or a video underrun will occur.
            if (plane->image_info.image_height_in_lines < config->num_beams_per_print_plane)
            {
                ERR("%s WARNING: image height in lines(%d) < num_beams(%d); VIDEO UNDERRUN LIKELY\n", 
                    __func__, plane->image_info.image_height_in_lines, config->num_beams_per_print_plane);
            }
            else if ((plane->image_info.image_height_in_lines & 0x1) && (config->num_beams_per_print_plane > 1))
            {
                // on dual beam engines we need an even number of lines or video will drop the last line of the buffer
                ERR("%s WARNING: image height in lines(%d) is odd number; video will drop the last line\n", 
                    __func__, plane->image_info.image_height_in_lines);
            }

            // video will NOT be happy if sent a strip with a size < the number of beams
            ASSERT(plane->image_info.image_height_in_lines >= config->num_beams_per_print_plane);

            vid_plane.image_info = plane->image_info;
            vid_plane.compression_info = plane->compression_info;

            // convert the data buffer to a dma buffer; note that this
            // may already have been done by a parser or generator, but if that
            // is the case, no harm no foul to do it here also.
            plane->big_buffer = BigBuffer_convert_to_dma_buffer( plane->big_buffer ); 
            ASSERT(plane->big_buffer != NULL);
            vid_plane.dma_buffer = *plane->big_buffer->dma_alloc;

            set_laser_video_plane_cookie(&vid_plane);

            // send plane data
            if((errcode = ioctl(video_driver_fd, LVC_IOCTL_ADD_PLANE_DATA, &vid_plane)) != 0)
            {
                ERR("%s IOCTL_ADD_PLANE_DATA 0x%02x failed, plane %d, error = %d\n", __func__, LVC_IOCTL_ADD_PLANE_DATA, i, errno);
                XASSERT(errcode == 0, errcode);
                break;  // game over for this plane  
            }

            plane_node = MListNext(&page->planeDataPendingPrintList[i], plane_node);
        }
    }
}


/**
 * \brief Send the page end notification to the video driver.  
 * 
 * \param page Page to be sent to driver
 *  
 * \return void
 */  
void video_write_page_end(PAGE_DATA* page)
{
    int errcode;
    if((errcode = ioctl(video_driver_fd, LVC_IOCTL_END_PAGE, page->PageCount)) != 0)
    {
        ERR("%s IOCTL_START_PAGE 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_END_PAGE, errno);
        ASSERT(0);
    } 
}

/**
 * \brief Send a page to the video driver to be imaged.  If accepted 
 * completion of the page will be notified via the page done callback. 
 * 
 * \param page Page to be sent to driver
 *  
 * \return error_type_t 
 * \retval VIDEO_OK    Page sent to driver.
 * \retval VIDEO_FAIL  Page not accepted .
 * \retval VIDEO_NOMEM Page not accepted due to out of memory condition .
 */  
error_type_t video_print_page(PAGE_DATA *page)
{
    error_type_t retval = VIDEO_FAIL;
    video_page_container_t* page_container;

    ASSERT(page != NULL);
    ASSERT(IS_PAGE_DATA(page));

    MSG("\n%s ENTRY VALUES: \n", __func__);
    MSG("BPP %d, x_bytes %d, x_pixels %d\n\n", page->image_output_info.image_bpp, page->image_output_info.image_data_width_in_bytes, page->image_output_info.image_width_in_pixels);

    video_validate_page(page);

    pthread_mutex_lock(&video_mutex);
    if(video_driver_fd < 0)
    {
        ERR("%s: VIDEO DRIVER NOT OPEN\n", __func__);
        goto DONE;
    }

    // allocate page container
    page_container = (video_page_container_t*)MEM_MALLOC(sizeof(video_page_container_t));
    ASSERT(page_container != NULL);
    if(page_container == NULL)
    {
        ERR("%s: out of memory\n", __func__);   
        retval = VIDEO_NOMEM;
        goto DONE;
    }
    ATInitNode(&page_container->node);
    page_container->page = page;

    INFO("%s: page @ %#x page %d of job %d.  page_context = %#x\n", __func__, (uint32_t)page, page->PageCount, page->pMyJob->JobNum, page_container);

    // send page to the driver
    video_write_page_header(page, page_container);
    video_write_plane_data(page);
    video_write_page_end(page);

    // add page to list
    MInsertTailList(&videoPageList, &page_container->node);

    retval = VIDEO_OK;
DONE:    
    pthread_mutex_unlock(&video_mutex);
    return retval;
}

error_type_t video_cancel_pages( void )
{
    error_type_t func_return_val = VIDEO_OK;
    int retval;

    MSG("%s\n", __func__);

    ASSERT(video_driver_fd > 0);

    pthread_mutex_lock(&video_mutex);

    // send ioctl
    if((retval = ioctl(video_driver_fd, LVC_IOCTL_CANCEL_PAGES, 0)) != 0)
    {
        ERR("%s 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_CANCEL_PAGES, errno);
        func_return_val = VIDEO_FAIL;
    }

    pthread_mutex_unlock(&video_mutex);

    return func_return_val;
}


error_type_t video_set_paperless_mode(bool enable)
{
    int bytes_written = 0;
    error_type_t retval = VIDEO_FAIL;
    int fd;

    fd = open(VIDEO_PAPERLESS_SYSFS_NAME, O_RDWR|O_CLOEXEC);
    if(fd >= 0)
    {
        bytes_written = write(fd, enable ? "1" : "0", 2);
        if(bytes_written != 2)
        {
            ERR("ERROR: failed to write paperless mode. errno = %d", errno);
        }
        else
        {
            retval = VIDEO_OK;
        }
        close(fd);
    }
    else
    {
        ERR("ERROR: failed to open paperless sysfs: %s\n", VIDEO_PAPERLESS_SYSFS_NAME);
    }

    return retval; // VIDEO_OK;
}

/**
 * \brief Notification that video has completed a page.  The completion notification
 * includes a page completion status as defined in video_laser_api.h. 
 *  
 * \param func Page done callback notification function pointer. 
 */
error_type_t video_register_page_done_callback(video_laser_pagedone_callback_t func)
{
    pthread_mutex_lock(&video_mutex);
    end_of_page_callback = func; 
    pthread_mutex_unlock(&video_mutex);

    return VIDEO_OK;
}

error_type_t video_eng_pcode_config(uint32_t *vid_eng_config, uint32_t num_entries, e_pcode_index p_index)
{
    error_type_t retval = VIDEO_OK;
    pcode_config_request_t pcode_req;

    ASSERT(num_entries <= MAX_NUM_PCODE_REGS);

    pcode_req.index = p_index;
    pcode_req.num_entries = num_entries;
    memcpy(&(pcode_req.pcode_array), vid_eng_config, (num_entries*sizeof(uint32_t)));

    ASSERT(video_driver_fd > 0);

    // send ioctl
    if((retval = ioctl(video_driver_fd, LVC_IOCTL_SET_PCODE_CONFIG, &pcode_req)) != 0)
    {
        ERR("%s 0x%02x failed! p_index = %d\n", __func__, retval, p_index);
        retval = VIDEO_FAIL;
    }

    return retval;
}


uint32_t percentToPcode(uint32_t percent)
{
	 if( percent == 100 )
		percent = percent - 1;		// 0x40 is not supported in marvell code.
    return (int) (((double)percent/100.0*64.0) + 0.5);
}

int32_t db_pcode = 0;

error_type_t video_set_pcode(int32_t mode, int32_t one)
{
	if(one > 100 || one < -1)
	{
		ERR("pcode value error!!! %d\n", one);
		return VIDEO_FAIL;
	}

	if(mode == PWM_DB_SET_MODE)
		db_pcode = one;
	else if(mode == PWM_LOW_SPEED_MODE)
	{
#if ABBA2
		one = one + (db_pcode - 10);
#else
		one = one + 45;               // 45%
#endif
		if(one > 99)
			one = 99;
		else if(one < -1)
			one = 0;
	}
	else if(mode == PWM_ORG_SPEED_MODE)
	{
		one = one + db_pcode;         // DB value
		if(one > 99)
			one = 99;
		else if(one < -1)
			one = 0;
	}
/* ** dead code **
	else // PWM_SET_MODE
	    one = one;
*/

    if(one == -1)
        one = PWM_DEFAULT;
    else
        one = percentToPcode(one);
    ERR("PCODE 0x%x\n", one);
    // save pcode value
	engine_set_video_pcode_1Bit(true, one);
    return VIDEO_OK;
}

int32_t db_pixel_freq = 0;
#if MAC_TYPE_M
#define HMAG_1PERCENT_47PPM PIXEL_FREQ_47PPM / 100 / 5
#define HMAG_1PERCENT_DEFAULT PIXEL_FREQ_DEFAULT / 100 / 5
#else
#define HMAG_1PERCENT_DEFAULT PIXEL_FREQ_DEFAULT / 100 / 5
#endif

error_type_t video_set_pixel_freq(int32_t mode, int32_t one)
{
    uint32_t i = 0;
    uint32_t pixel_freq[NUM_VIDEO_CHAN] = {0, };

    MSG("PIXEL FREQ %d, db_pixel %d\n", one, db_pixel_freq);

	if(mode == PIXEL_FREQ_HMAG_MODE)
		db_pixel_freq = one;
	else if(mode == PIXEL_FREQ_LOW_SPEED_MODE || mode == PIXEL_FREQ_ORG_SPEED_MODE)
		one = one + db_pixel_freq;

    memcpy(pixel_freq, config->pixel_freq_in_hz, sizeof(pixel_freq));
    for(i = 0; i < config->num_beams_per_print_plane; i++)
    {
#if MAC_TYPE_M
			if(mode == PIXEL_FREQ_LOW_SPEED_MODE)
				pixel_freq[i] = PIXEL_FREQ_47PPM - one * (PIXEL_FREQ_47PPM / 100 / 2) + HMAG_1PERCENT_47PPM; // -one * 0.5mm + 0.2mm
			else
				pixel_freq[i] = PIXEL_FREQ_DEFAULT - one * (PIXEL_FREQ_DEFAULT / 100 / 2) + HMAG_1PERCENT_DEFAULT; // -one * 0.5mm + 0.2mm
#else
			pixel_freq[i] = PIXEL_FREQ_DEFAULT - one * (PIXEL_FREQ_DEFAULT / 100 / 2) + HMAG_1PERCENT_DEFAULT; // -one * 0.5mm + 0.2mm
#endif
    }

    memcpy(config->pixel_freq_in_hz, pixel_freq, sizeof(config->pixel_freq_in_hz));
    if(set_vid_eng_config(video_driver_fd, config) != 0)
    {
        ERR("%s: FAILED TO CONFIG DRIVER.  Video disabled! error = %d\n", __func__, errno);
        video_close_driver();
    }
    for(i = 0; i < config->num_beams_per_print_plane; i++)
        MSG("config->pixel[%d] : %d\n", i, config->pixel_freq_in_hz[i]);

    return VIDEO_OK;
}

