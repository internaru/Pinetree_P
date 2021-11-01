/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 *  \brief Kernel video driver test app.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <endian.h>

#include <sys/ioctl.h>

#include "../../include/laser_video_chardev_api.h"
#include "../../include/laser_video_types.h"
#include "../../src/laser_engine_config_types.h"
#include "list.h"
#include "jbig.h"

#define VIDEO_DEV_NAME "/dev/lvc0"

static int pageid = 1;

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif

/******************* Sample video configuration ***************************/

// The following configuration is for a dual beam, monochrome laser printer.

// HIPWM clock frequency in Hz
#define HIPWM_CLOCK_FREQ 156.25E6

#define PIXEL_FREQ 16150000

#define BD_PERIOD     538.8E-6   // 296E-6

#define NUM_PRINT_VIDEO_CHANNELS 2

#define SUB_PIXEL_DENOM       64
#define CENTERLINE            3500
#define BEAM_OFFSET_PIXELS    -3     // pixel offset for beam 1 from beam 0
#define BEAM_OFFSET_SUBPIXELS 38     // subpixel offset for beam 1, in 64ths.

#define TOP_OFFSET 150

// Offset between beams in lines
#define BEAM_OFFSET_LINES 0

#define BD_LOCKOUT_PERIOD (uint32_t)(0.95 * BD_PERIOD * HIPWM_CLOCK_FREQ)

// The following LUT tables are default values for the video pulse codes for 
// various bit depths. It is recommended that a copy be created of these tables
// and modified as appropriate for a particular application. 

#define PWM_1BIT   0x2c2c2c2c
static const uint32_t pulse_code_lut_1_bit[MAX_NUM_PCODE_REGS] = {
  0, 0, PWM_1BIT,              PWM_1BIT,               // 00 center just
  0, 0, PWM_1BIT | 0x40404040, PWM_1BIT | 0x40404040,  // 01 left/early just
  0, 0, PWM_1BIT | 0x80808080, PWM_1BIT | 0x80808080,  // 10 right/late just
  0, 0, PWM_1BIT | 0xC0C0C0C0, PWM_1BIT | 0xC0C0C0C0}; // 11 split just

#define PWM_1BIT_EM   0x1A1A1A1A
static const uint32_t pulse_code_lut_bit_draft[MAX_NUM_PCODE_REGS] = {
  0, 0, PWM_1BIT_EM,            PWM_1BIT_EM,             // 00 center just
  0, 0, PWM_1BIT_EM|0x40404040, PWM_1BIT_EM|0x40404040,  // 01 left/early just
  0, 0, PWM_1BIT_EM|0x80808080, PWM_1BIT_EM|0x80808080,  // 10 right/late just
  0, 0, PWM_1BIT_EM|0xC0C0C0C0, PWM_1BIT_EM|0xC0C0C0C0}; // 11 split just

                            
//  Default 2 bit table. "OR" in the justification bits below

#define PWM_2BIT_0   0x00000000
#define PWM_2BIT_1   0x19191919
#define PWM_2BIT_2   0x26262626
#define PWM_2BIT_3   0x3f3f3f3f
static const uint32_t pulse_code_lut_2_bit[MAX_NUM_PCODE_REGS] = {
  PWM_2BIT_0, PWM_2BIT_1, PWM_2BIT_2, PWM_2BIT_3,      // 00 center just
  PWM_2BIT_0 | 0x40404040, PWM_2BIT_1 | 0x40404040,    // 01 left/early just
  PWM_2BIT_2 | 0x40404040, PWM_2BIT_3 | 0x40404040,
  PWM_2BIT_0 | 0x80808080, PWM_2BIT_1 | 0x80808080,    // 10 right/late just
  PWM_2BIT_2 | 0x80808080, PWM_2BIT_3 | 0x80808080,
  PWM_2BIT_0 | 0xC0C0C0C0, PWM_2BIT_1 | 0xC0C0C0C0,    // 11 split just
  PWM_2BIT_2 | 0xC0C0C0C0, PWM_2BIT_3 | 0xC0C0C0C0};

// Default 4 bit table.
#define PWM_4BIT_0   0x0C080400
#define PWM_4BIT_1   0x1C181410
#define PWM_4BIT_2   0x2C282420
#define PWM_4BIT_3   0x3F383430
static const uint32_t pulse_code_lut_4_bit[MAX_NUM_PCODE_REGS]= {
  PWM_4BIT_0, PWM_4BIT_1, PWM_4BIT_2, PWM_4BIT_3,      // 00 center just
  PWM_4BIT_0 | 0x40404040, PWM_4BIT_1 | 0x40404040,    // 01 left/early just
  PWM_4BIT_2 | 0x40404040, PWM_4BIT_3 | 0x40404040,
  PWM_4BIT_0 | 0x80808080, PWM_4BIT_1 | 0x80808080,    // 10 right/late just
  PWM_4BIT_2 | 0x80808080, PWM_4BIT_3 | 0x80808080,
  PWM_4BIT_0 | 0xC0C0C0C0, PWM_4BIT_1 | 0xC0C0C0C0,    // 11 split just
  PWM_4BIT_2 | 0xC0C0C0C0, PWM_4BIT_3 | 0xC0C0C0C0};

#define NUM_PRINT_VIDEO_CHANNELS 2
#define NUM_MARKING_COLOR_CHANNELS 1

static laser_engine_video_config_t video_platform_config = 
{
    
    .num_print_planes = 1,
    .num_beams_per_print_plane = 2,
    .num_video_channels = NUM_PRINT_VIDEO_CHANNELS,
    .num_print_colors = NUM_MARKING_COLOR_CHANNELS,
    .report_video_event = {false, true}, // Use beam 1 to report all video  events for BOTH beams
    .chan_to_print_plane_color = {COLOR_BLACK, COLOR_BLACK},
    .top_to_print_plane = {0, 0},
    .video_chan_hsync_input = {0, 0, 0, 0, 0, 0, 0, 0},
    .topq_hsync_sel = {0, 0, 0, 0},
    .active_video_channels = {true, true}, // Dual beam engine, both channels are active
    .beam_to_video_hw_chan = {{0, 1}},
    .video_buffer_size_in_bytes = 0x80000*8*2,
    .num_video_descriptors = 4,
    .sub_pixel_denominator = SUB_PIXEL_DENOM,

    .color_process_order = {{COLOR_BLACK}}, 
    .beam_direction = {LEFT_TO_RIGHT, LEFT_TO_RIGHT},
        
    .burst_size = BYTES_16,
    .serpentine = false,
    .video_processing = MONOCHROME,
    .hsync_lockout_period_in_hipwm_clocks = 0, // REVISIT 
    .invert_video = false,
    .print_engine_native_resolution = DPI_600,

    .top_of_page_margin_in_lines = {TOP_OFFSET},
    .side_margin_reference_in_sub_pixels = { SUB_PIXEL_DENOM * CENTERLINE,
                                             SUB_PIXEL_DENOM * (CENTERLINE + BEAM_OFFSET_PIXELS) + BEAM_OFFSET_SUBPIXELS},
    .pixel_freq_in_hz = {PIXEL_FREQ, PIXEL_FREQ},
    .power_down_lvdo_between_pages = false,

    // Mirror configuration
    .num_mirrors = 1,
    .num_mirror_facets = {4},
};

/**************************************************************************/
/* forward declarations */

#define MAX_PLANES 1
typedef struct _vid_test_page
{
    laser_video_page_t vid_page;
    ATLISTENTRY plane_lists[MAX_PLANES];
}vid_test_page_t;

typedef struct _vid_test_plane
{
    ATLISTENTRY node;
    laser_video_plane_t vid_plane;
}vid_test_plane_t;

vid_test_page_t* send_mono_page_jbig_full(int fd, char* jbig_page_file);

#define MONO_PAGE_DPI 600
#define LETTER_PAGE_WIDTH_IN 8.5
#define LETTER_PAGE_HEIGHT_IN 11
#define DEFAULT_MARGIN 200 // 100 dots on each side
#define STRIP_HEIGHT_IN_LINES 100
#define LETTER_PAGE_MAX_LINE_WIDTH_PIXELS (LETTER_PAGE_WIDTH_IN * MONO_PAGE_DPI)

#define VIDCTL_PAGE_STATUS_OK            0  ///< page completed with no errors
#define VIDCTL_PAGE_STATUS_CANCELLED    -1  ///< page cancelled by uppper level SW request
#define VIDCTL_PAGE_STATUS_ERROR        -2  ///< page completed with error; image not fully printed

/**************************************************************************/

laser_engine_video_config_t* get_video_config(void)
{
    static bool initialized = false;
    if(!initialized)
    {
        memcpy(video_platform_config.pulse_code_1_bit_array,  pulse_code_lut_1_bit, sizeof(video_platform_config.pulse_code_1_bit_array));
        memcpy(video_platform_config.pulse_code_2_bit_array,  pulse_code_lut_2_bit, sizeof(video_platform_config.pulse_code_2_bit_array));
        memcpy(video_platform_config.pulse_code_4_bit_array,  pulse_code_lut_4_bit, sizeof(video_platform_config.pulse_code_4_bit_array));
        memcpy(video_platform_config.ed_pulse_code_1_bit_array,  pulse_code_lut_1_bit, sizeof(video_platform_config.pulse_code_1_bit_array));
        memcpy(video_platform_config.ed_pulse_code_2_bit_array,  pulse_code_lut_2_bit, sizeof(video_platform_config.pulse_code_2_bit_array));
        memcpy(video_platform_config.ed_pulse_code_4_bit_array,  pulse_code_lut_4_bit, sizeof(video_platform_config.pulse_code_4_bit_array));
        engine_video_config_set_cookies(&video_platform_config);
        initialized = true;
    }
    return &video_platform_config;
}

int set_vid_eng_config(void)
{
    int fd1;
    int retval = 0;
    laser_engine_video_config_t* vidcfg;

    vidcfg = get_video_config();

    // open driver
    fd1 = open(VIDEO_DEV_NAME, O_RDWR);
    if(fd1 < 0)
    {
        printf("ERROR: single open failed name = %s, err = %d\n", VIDEO_DEV_NAME, errno);
        retval = -1;
        goto FAIL;
    }

    // send ioctl
    if((retval = ioctl(fd1, LVC_IOCTL_SET_CONFIG, vidcfg)) != 0)
    {
        printf("%s 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_SET_CONFIG, retval);
    }    

    // close
    close(fd1); 
FAIL:
    return retval;
}

#define VIDEO_PAPERLESS_SYSFS_NAME "/sys/devices/platform/laser_video_chardev.0/paperless"
int set_paperless_mode(bool enable)
{
    int retval = 0;
    int fd;

    fd = open(VIDEO_PAPERLESS_SYSFS_NAME, O_RDWR);
    if(fd < 0)
    {
        printf("ERROR: failed to open paperless sysfs: %s\n", VIDEO_PAPERLESS_SYSFS_NAME);
        return -1;
    }

    retval = write(fd, enable ? "1" : "0", 2);
    if(retval != 2)
    {
        printf("ERROR: failed to write paperless mode. errno = %d", errno);
        retval = -1;
    }
    else
    {
        retval = 0;
    }
    close(fd);

    return retval;
}

vid_test_page_t* create_vid_page()
{
    vid_test_page_t* test_page = NULL;
    int i;
    test_page = malloc(sizeof(vid_test_page_t));
    
    if(test_page)
    {
        laser_video_page_t* vid_page = &test_page->vid_page;
        // setup the page structure
        vid_page->page_id = pageid++;
        vid_page->page_context = vid_page;

        // image_info
        vid_page->image_info.image_dpi.x = MONO_PAGE_DPI;
        vid_page->image_info.image_dpi.y = MONO_PAGE_DPI;
        vid_page->image_info.image_bpp = 1;
        vid_page->image_info.left_to_right = true;
        vid_page->image_info.image_width_in_pixels = ((uint32_t)(vid_page->image_info.image_dpi.x * LETTER_PAGE_WIDTH_IN)) - DEFAULT_MARGIN;
        vid_page->image_info.image_data_width_in_bytes = ((vid_page->image_info.image_width_in_pixels * vid_page->image_info.image_bpp + 7) / 8);  // round up to full byte
        vid_page->image_info.image_data_width_in_bytes = (vid_page->image_info.image_data_width_in_bytes + 15) & ~0xFul; // data width must be a multiple of 16 bytes
        vid_page->image_info.image_height_in_lines = ((uint32_t)(vid_page->image_info.image_dpi.y * LETTER_PAGE_HEIGHT_IN)) - DEFAULT_MARGIN;

        vid_page->page_output_color_mode = e_Mono;
        vid_page->video_justification_mode = VIDEO_JUSTIFY_AUTO;
        vid_page->top_to_bottom = true;
        vid_page->halftone_type = THRESHOLD_HALFTONE;

        // use default reference margin vals
        vid_page->side_margin_in_subpixels[0] = video_platform_config.side_margin_reference_in_sub_pixels[0];
        vid_page->side_margin_in_subpixels[1] = video_platform_config.side_margin_reference_in_sub_pixels[1];
        vid_page->top_margin_in_lines[0] = video_platform_config.top_of_page_margin_in_lines[0];

        for(i = 0; i < MAX_PLANES; i++)
        {
            ATInitList(&test_page->plane_lists[i]);
        }
        set_laser_video_page_cookie(vid_page);
    }
    return test_page;
}

vid_test_plane_t* create_plane(image_info_t* image_info, COLOR color, uint32_t page_data_pattern, uint32_t strip_height)
{
    vid_test_plane_t* plane = NULL;
    char* buf = NULL;
    int i;
    uint32_t bufsize = image_info->image_data_width_in_bytes * strip_height;

    buf = malloc(bufsize);
    plane = malloc(sizeof(vid_test_plane_t));
    if(!buf || !plane)
    {
        return NULL;
    }

    // fill the first line; note that we're filling in to the right edge pad here just because it's simpler
    for(i = 0; i < image_info->image_data_width_in_bytes; i += sizeof(uint32_t))
    {
        *(uint32_t*)(buf + i) = page_data_pattern;
    }
    // replicate the first line to the rest of the strip
    for(i = 1; i < strip_height; i++)
    {
        memcpy(buf + (i*image_info->image_data_width_in_bytes), buf, image_info->image_data_width_in_bytes);
    }

    plane->vid_plane.sysPlaneColor = color;
    plane->vid_plane.DataLength = bufsize;
    plane->vid_plane.start_of_image = false;
    plane->vid_plane.end_of_image = false;

    // image info is identical to passed in page info except for strip height
    plane->vid_plane.image_info = *image_info;
    plane->vid_plane.image_info.image_height_in_lines = strip_height;  

    plane->vid_plane.compression_info.comp_type = COMPTYPE_NONE;

    // convert the data buffer to a dma buffer
    dma_alloc_reuse(&plane->vid_plane.dma_buffer, bufsize);
    dma_alloc_write(&plane->vid_plane.dma_buffer, buf, bufsize);

    // data is now in kernel buffer, so free user level strip
    free(buf);

    set_laser_video_plane_cookie(&plane->vid_plane);

    return plane;
}

void free_page(vid_test_page_t* page)
{
    vid_test_plane_t* plane;
    ATLISTENTRY* node;
    int i;

    if(!page)
    {
        return;
    }

    // free the attached planes
    for(i = 0; i < MAX_PLANES; i++)
    {
        while(!ATIsListEmpty(&page->plane_lists[i]))
        {
            node = ATRemoveHeadList(&page->plane_lists[i]);
            plane = CONTAINING_RECORD(node, vid_test_plane_t, node);
            clear_laser_video_plane_cookie(&plane->vid_plane);
            dma_free(&plane->vid_plane.dma_buffer);            
            free(plane);
        }
    }
    clear_laser_video_page_cookie(&page->vid_page);
    free(page);
}

vid_test_page_t* send_mono_page_raw(int fd, uint32_t page_data_pattern)
{
    vid_test_page_t* vid_page = NULL;
    vid_test_plane_t *plane;
    int page_lines_sent = 0;
    int retval;

    vid_page = create_vid_page();

    // send page start
    if((retval = ioctl(fd, LVC_IOCTL_START_PAGE, &vid_page->vid_page)) != 0)
    {
        printf("%s IOCTL_START_PAGE 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_START_PAGE, retval);
    } 

    // create the page data pattern

    // send strips; remember SOI and EOI
    while(page_lines_sent < vid_page->vid_page.image_info.image_height_in_lines)
    {
        plane = create_plane(&vid_page->vid_page.image_info, COLOR_BLACK, page_data_pattern, 
                             MIN(STRIP_HEIGHT_IN_LINES, vid_page->vid_page.image_info.image_height_in_lines));
        assert(plane);

        if(page_lines_sent == 0)
        {
            plane->vid_plane.start_of_image = true;
        }
        page_lines_sent += plane->vid_plane.image_info.image_height_in_lines;
        if(page_lines_sent >= vid_page->vid_page.image_info.image_height_in_lines)
        {
            printf("TEST APP SETTING EOI MARKER\n");
            plane->vid_plane.end_of_image = true;
        }

        // send plane data
        if((retval = ioctl(fd, LVC_IOCTL_ADD_PLANE_DATA, &plane->vid_plane)) != 0)
        {
            printf("%s IOCTL_ADD_PLANE_DATA 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_ADD_PLANE_DATA, retval);
            return 0;  // game over           
        }

        ATInsertTailList(&vid_page->plane_lists[0], &plane->node);
                
    }

    // send page end
    if((retval = ioctl(fd, LVC_IOCTL_END_PAGE, vid_page->vid_page.page_id)) != 0)
    {
        printf("%s IOCTL_START_PAGE 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_END_PAGE, retval);
    } 
    return vid_page;
}

int wait_and_verify_page_done(int fd, vid_test_page_t* page, uint32_t* page_status)
{
    page_done_notify_t page_done_info;
    ssize_t bytes_read;

    bytes_read = read(fd, &page_done_info, sizeof(page_done_notify_t));
    if(bytes_read < 0)
    {
        printf("%s: read error: errno = %d\n", __func__, errno);
        return -1;
    }
    if(bytes_read != sizeof(page_done_notify_t))
    {
        printf("%s: wrong num bytes read.  expected: %d, read %d\n", __func__, bytes_read, sizeof(page_done_notify_t));
        return -1;
    }
    if(page_done_info.page_context != page)
    {
        printf("%s: wrong page context.  expected: %p, recevied %p\n", __func__, page, page_done_info.page_context);
        return -1;
    }
    printf("%s: page done.  status = %d\n", __func__, page_done_info.page_status);

    if (page_status)
    {
        *page_status = page_done_info.page_status;
    }
    return 0;
}

int test_page_cancel_raw(int fd)
{
    vid_test_page_t* page1;
    vid_test_page_t* page2;
    uint32_t status1;
    uint32_t status2;
    int retval;

    // send single raw page
    if (!(page1 = send_mono_page_raw(fd, 0xFF000000)))
    {
        printf("ERROR: %s failed to send page 1.\n", __func__);
        goto FAIL;
    }

    // send single raw page
    if (!(page2 = send_mono_page_raw(fd, 0xFF000000)))
    {
        printf("ERROR: %s failed to send page 2.\n", __func__);
        goto FAIL;
    }

    // send cancel request for all pages
    if((retval = ioctl(fd, LVC_IOCTL_CANCEL_PAGES, 0)) != 0)
    {
        printf("%s IOCTL_CANCEL_PAGES 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_CANCEL_PAGES, retval);
    } 

    if(wait_and_verify_page_done(fd, page1, &status1))
    {
        printf("%s: ERROR: page1 wait failed\n", __func__);
        goto FAIL;
    }

    if(wait_and_verify_page_done(fd, page2, &status2))
    {
        printf("%s: ERROR: page2 wait failed\n", __func__);
        goto FAIL;
    }

    // free the pages
    free_page(page1);
    free_page(page2);

    if (status1 != VIDCTL_PAGE_STATUS_CANCELLED)
    {
        printf("%s: ERROR: wrong page status.  Expected: %d, is: %d:\n", __func__, VIDCTL_PAGE_STATUS_CANCELLED, status1);
        goto FAIL;
    }

    if (status2 != VIDCTL_PAGE_STATUS_CANCELLED)
    {
        printf("%s: ERROR: wrong page status.  Expected: %d, is: %d:\n", __func__, VIDCTL_PAGE_STATUS_CANCELLED, status2);
        goto FAIL;
    }

    return 0;

FAIL:
    return -1;
}

int test_page_cancel_jbig(int fd, char* jbig_file_name)
{
    vid_test_page_t* page1;
    vid_test_page_t* page2;
    uint32_t status1;
    uint32_t status2;
    int retval;

    // send single jbig page
    if (!(page1 = send_mono_page_jbig_full(fd, jbig_file_name)))
    {
        printf("ERROR: %s failed to send page 1.\n", __func__);
        goto FAIL;
    }

    // send single jbig page
    if (!(page2 = send_mono_page_jbig_full(fd, jbig_file_name)))
    {
        printf("ERROR: %s failed to send page 2.\n", __func__);
        goto FAIL;
    }

    // send cancel request for all pages
    if((retval = ioctl(fd, LVC_IOCTL_CANCEL_PAGES, 0)) != 0)
    {
        printf("%s IOCTL_CANCEL_PAGES 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_CANCEL_PAGES, retval);
    } 

    if(wait_and_verify_page_done(fd, page1, &status1))
    {
        printf("%s: ERROR: page1 wait failed\n", __func__);
        goto FAIL;
    }

    if(wait_and_verify_page_done(fd, page2, &status2))
    {
        printf("%s: ERROR: page2 wait failed\n", __func__);
        goto FAIL;
    }

    // free the pages
    free_page(page1);
    free_page(page2);

    if (status1 != VIDCTL_PAGE_STATUS_CANCELLED)
    {
        printf("%s: ERROR: wrong page status.  Expected: %d, is: %d:\n", __func__, VIDCTL_PAGE_STATUS_CANCELLED, status1);
        goto FAIL;
    }

    if (status2 != VIDCTL_PAGE_STATUS_CANCELLED)
    {
        printf("%s: ERROR: wrong page status.  Expected: %d, is: %d:\n", __func__, VIDCTL_PAGE_STATUS_CANCELLED, status2);
        goto FAIL;
    }

    return 0;

FAIL:
    return -1;
}

int main(int argc, char** argv)
{
    vid_test_page_t* page;

    int fd1;
    int fd2;

    // open & close channel
    fd1 = open(VIDEO_DEV_NAME, O_RDWR);
    if(fd1 < 0)
    {
        printf("ERROR: single open failed name = %s, err = %d\n", VIDEO_DEV_NAME, errno);
        goto FAIL;
    }
    close(fd1);       

    // multiple open.  Second open should fail
    fd1 = open(VIDEO_DEV_NAME, O_RDWR);
    if(fd1 < 0)
    {
        printf("ERROR: multiple open failed fd1 name = %s, err = %d\n", VIDEO_DEV_NAME, errno);
        goto FAIL;
    }

    fd2 = open(VIDEO_DEV_NAME, O_RDWR);
    if(fd2 >= 0)
    {
        printf("ERROR: multiple open; open succeded when it should have failed name = %s, err = %d\n", VIDEO_DEV_NAME, errno);
        goto FAIL;
    }
    close(fd1);

    if(set_vid_eng_config())
    {
        goto FAIL;
    }

    // enable paperless mode
    if(set_paperless_mode(true))
    {
        goto FAIL;
    }

    fd1 = open(VIDEO_DEV_NAME, O_RDWR);
    if(fd1 < 0)
    {
        printf("ERROR: page print open failed name = %s, err = %d\n", VIDEO_DEV_NAME, errno);
        goto FAIL;
    }

    if (argc == 1)
    {
        printf("Sending Raw Page.");
        // send single raw page
        if(!(page = send_mono_page_raw(fd1, 0xFF000000)))
        {
            printf("ERROR: send single page failed.\n");
            goto FAIL;
        }
    }
    else
    {
        printf("Sending JBIG Page: %s", argv[1]);
        // send single raw page
        if(!(page = send_mono_page_jbig_full(fd1, argv[1])))
        {
            printf("ERROR: send single JBIG page failed.\n");
            goto FAIL;
        }
    }

    if(wait_and_verify_page_done(fd1, page, 0))
    {
        printf("ERROR: page wait failed\n");
        goto FAIL;
    }

    // free the page
    free_page(page);

    // test page cancel
    if (argc == 1)
    {
        if(test_page_cancel_raw(fd1))
        {
            goto FAIL;
        }
    }
    else
    {
        if(test_page_cancel_jbig(fd1, argv[1]))
        {
            goto FAIL;
        }
    }

    close(fd1);
    // disable paperless mode
    if(set_paperless_mode(false))
    {
        goto FAIL;
    }

    printf("vid_test: PASS\n");
    return 0;

FAIL:
    printf("vid_test: FAIL\n");
    return -1;
}

/************************* JBIG ************************************/

vid_test_page_t* create_jbig_page(int bpp, int line_width_pixels, int page_height_lines)
{
    vid_test_page_t* test_page = NULL;
    int i;
    test_page = malloc(sizeof(vid_test_page_t));
    
    if(test_page)
    {
        laser_video_page_t* vid_page = &test_page->vid_page;
        // setup the page structure
        vid_page->page_id = pageid++;
        vid_page->page_context = vid_page;

        // image_info
        vid_page->image_info.image_dpi.x = MONO_PAGE_DPI;
        vid_page->image_info.image_dpi.y = MONO_PAGE_DPI;
        vid_page->image_info.image_bpp = bpp;
        vid_page->image_info.left_to_right = true;
        vid_page->image_info.image_width_in_pixels = line_width_pixels;
        vid_page->image_info.image_data_width_in_bytes = (vid_page->image_info.image_width_in_pixels * bpp / 8);
        assert(vid_page->image_info.image_data_width_in_bytes & ~0xFul);  // data width must be a multiple of 16 bytes
        vid_page->image_info.image_height_in_lines = page_height_lines;

        vid_page->page_output_color_mode = e_Mono;
        vid_page->video_justification_mode = VIDEO_JUSTIFY_AUTO;
        vid_page->top_to_bottom = true;
        vid_page->halftone_type = THRESHOLD_HALFTONE;

        // use default reference margin vals
        vid_page->side_margin_in_subpixels[0] = video_platform_config.side_margin_reference_in_sub_pixels[0];
        vid_page->side_margin_in_subpixels[1] = video_platform_config.side_margin_reference_in_sub_pixels[1];
        vid_page->top_margin_in_lines[0] = video_platform_config.top_of_page_margin_in_lines[0];

        for(i = 0; i < MAX_PLANES; i++)
        {
            ATInitList(&test_page->plane_lists[i]);
        }
        set_laser_video_page_cookie(vid_page);
    }
    return test_page;
}

vid_test_plane_t* create_jbig_plane_from_page_image(FILE* jbig_file, JBIG_BIH* jbig_header, image_info_t* image_info, COLOR color)
{
    vid_test_plane_t* plane = NULL;
    char* buf = NULL;
    uint32_t bufsize = image_info->image_data_width_in_bytes * STRIP_HEIGHT_IN_LINES;
    uint32_t bytes_read;

    assert(image_info->image_data_width_in_bytes == jbig_header->Xd/8);

    buf = malloc(bufsize);
    plane = malloc(sizeof(vid_test_plane_t));
    if(!buf || !plane)
    {
        return NULL;
    }

    // read the file into the buffer
    bytes_read = fread(buf, 1, bufsize, jbig_file);

    if (bytes_read)
    {
        plane->vid_plane.sysPlaneColor = color;
        plane->vid_plane.DataLength = bytes_read;
        plane->vid_plane.start_of_image = false;
        plane->vid_plane.end_of_image = false;

        // image info is identical to passed in page info except for strip height
        // since this is jbig compressed image data in a single large buffer we
        // don't really know how many lines this strip will decompress to.
        plane->vid_plane.image_info = *image_info;
        plane->vid_plane.image_info.image_height_in_lines = 0;  

        plane->vid_plane.compression_info.comp_type = COMPTYPE_JBIG;
        plane->vid_plane.compression_info.comp_details.jbig.Xd = jbig_header->Xd;
        plane->vid_plane.compression_info.comp_details.jbig.Yd = jbig_header->Yd;
        plane->vid_plane.compression_info.comp_details.jbig.L0 = jbig_header->L0;
        plane->vid_plane.compression_info.comp_details.jbig.option_flags = jbig_header->Options;

        // convert the data buffer to a dma buffer
        dma_alloc_reuse(&plane->vid_plane.dma_buffer, bytes_read);
        dma_alloc_write(&plane->vid_plane.dma_buffer, buf, bytes_read);

        // data is now in kernel buffer, so free user level strip
        free(buf);

        set_laser_video_plane_cookie(&plane->vid_plane);
    }
    else
    {
        // at EOF so free and return NULL plane
        free(buf);
        free(plane);
        plane = NULL;
    }

    return plane;
}

// send full jbig page.  This is single image per page.
vid_test_page_t* send_mono_page_jbig_full(int fd, char* jbig_page_file)
{
    FILE* jbig_file;
    JBIG_BIH jbig_header;
    size_t bytes_read;
    int bpp;
    int line_width_pixels;
    vid_test_page_t* vid_page = NULL;
    vid_test_plane_t *plane;
    bool first_strip = true;
    int retval;

    // open jbig file and read header
    jbig_file = fopen(jbig_page_file, "rb");
    if (!jbig_file)
    {
        printf("%s: failed to open jbig file %s\n", __func__, jbig_page_file);
        return NULL;
    }

    bytes_read = fread(&jbig_header, 1, sizeof(jbig_header), jbig_file);
    if (bytes_read != sizeof(jbig_header))
    {
        printf("%s: failed to read jbig header expected %d, read %d\n", __func__, sizeof(jbig_header), bytes_read);
    }

    // header values are big endian, swap as needed
    jbig_header.Xd = be32toh(jbig_header.Xd);
    jbig_header.Yd = be32toh(jbig_header.Yd);
    jbig_header.L0 = be32toh(jbig_header.L0);

    // if Xd wider than letter assume 2bpp
    bpp = (jbig_header.Xd > LETTER_PAGE_MAX_LINE_WIDTH_PIXELS) ? 2 : 1;
    line_width_pixels = jbig_header.Xd / bpp;

    printf("%s: Sending JBIG file: Xd = %d, Yd = %d, L0 = %d\n", __func__, jbig_header.Xd, jbig_header.Yd, jbig_header.L0);
    printf("\tbpp = %d, line_width_pixels = %d\n", bpp, line_width_pixels);

    vid_page = create_jbig_page(bpp, line_width_pixels, jbig_header.Yd);

    // send page start
    if((retval = ioctl(fd, LVC_IOCTL_START_PAGE, &vid_page->vid_page)) != 0)
    {
        printf("%s IOCTL_START_PAGE 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_START_PAGE, retval);
    } 

    // send strips; remember SOI and EOI
    while((plane = create_jbig_plane_from_page_image(jbig_file, &jbig_header, &vid_page->vid_page.image_info, COLOR_BLACK)) != NULL)
    {
        assert(plane);

        if(first_strip)
        {
            plane->vid_plane.start_of_image = true;
            first_strip = false;
        }

        if (feof(jbig_file))
        {
            printf("TEST APP SETTING EOI MARKER\n");
            plane->vid_plane.end_of_image = true;
        }

        // send plane data
        if((retval = ioctl(fd, LVC_IOCTL_ADD_PLANE_DATA, &plane->vid_plane)) != 0)
        {
            printf("%s IOCTL_ADD_PLANE_DATA 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_ADD_PLANE_DATA, retval);
            return 0;  // game over           
        }

        ATInsertTailList(&vid_page->plane_lists[0], &plane->node);
    }

    // send page end
    if((retval = ioctl(fd, LVC_IOCTL_END_PAGE, vid_page->vid_page.page_id)) != 0)
    {
        printf("%s IOCTL_START_PAGE 0x%02x failed! error = %d\n", __func__, LVC_IOCTL_END_PAGE, retval);
    } 
    return vid_page;

}




