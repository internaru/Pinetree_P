/*
 * ============================================================================
 * Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _LASER_VIDEO_TYPES_H_
#define _LASER_VIDEO_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

#include "laser_video_defs.h"
#include "memAPI.h"
#ifndef __KERNEL__
#include "dma_alloc_api.h"
#endif

// LSPTODO: resolve multiple definitions of this
/* Generic defines for colors. the engine and video need to make sure and
 * convert from generic to actual color indices  when interacting with the 
 * print engine.
 */
#ifndef _PRINT_H_
typedef enum
{
    COLOR_BLACK = 0,
    COLOR_CYAN,
    COLOR_YELLOW,
    COLOR_MAGENTA,
    COLOR_FIRST = COLOR_BLACK,
    COLOR_LAST = COLOR_MAGENTA
} COLOR;
#endif

#ifndef PRINT_JOB_TYPES_H
typedef enum
{
    DPI_200 = 200,
    DPI_300 = 300,
    DPI_400 = 400,
    DPI_600 = 600,
    DPI_1200 = 1200
} resolution_t;

typedef enum
{
    VIDEO_JUSTIFY_CENTERED,
    VIDEO_JUSTIFY_AUTO,
    VIDEO_JUSTIFY_PAIRED,
} justification_mode_t;

typedef enum
{
    e_Mono = 0,
    e_Color = 1,
} page_color_mode_t;

#define THRESHOLD_HALFTONE 1
#define ERR_DIFFUSION_HALFTONE 2

/////////////////////////////////////////////////////////////////////////////////////////////////
// The following set of structures are used to describe the compression type and specify the
// compression parameters for the strips of data that make up a print plane.

/// enum representing the supported types of compression.  Currently only JBIG, but will be expanded.
typedef enum compression_types_s
{
    COMPTYPE_JBIG,
    COMPTYPE_RGB, // REVISIT DAB this should be deprecated and replaced with pixel packing info as this is not really compression type
    COMPTYPE_JPEG,
    COMPTYPE_ZX_STREAM,
    COMPTYPE_NONE
} compression_types_t;

/// JBIG specific compression parameters.
typedef struct jbig_compression_info_s
{
    uint32_t Xd; ///< Xd represents the padded data width, in bits, of each line in the video buffer.
                 //  Xd must be padded to a multiple of the video burst length to allow video hardware DMA to work correctly.
    uint32_t Yd; ///< JBIG total image length in lines
    uint32_t L0; ///< JBIG lines per stripe.
    uint32_t option_flags;
} jbig_compression_info_t;


typedef struct rgb_compression_info_s
{
    uint32_t bits_per_pixel;
    uint32_t Xd; ///< Xd represents the padded data width, in bits, of each line in the video buffer.
                 //  Xd must be padded to a multiple of the video burst length to allow video hardware DMA to work correctly.
    uint32_t Yd; ///< total image length in lines
} rgb_compression_info_t;

typedef struct raw_compression_info_s
{
    uint32_t bits_per_pixel;
    uint32_t Xd;
    uint32_t Yd;
} raw_compression_info_t;
 
/// compression information wrapper struct
typedef struct compression_info_s
{
    compression_types_t comp_type;
    union
    {
        jbig_compression_info_t jbig;
        rgb_compression_info_t rgb;
        raw_compression_info_t raw;
    } comp_details;
} compression_info_t;

typedef struct image_resolution_s
{
    uint32_t x;
    uint32_t y;
} image_resolution_t;

typedef struct image_info_s
{
    uint32_t image_width_in_pixels;     // This is the width of the image, used for calculating side margins
    uint32_t image_height_in_lines;
    uint32_t image_bpp;
    uint32_t image_data_width_in_bytes; // This is the data width of the white space padded image.
    uint32_t image_scale_right_padding;	// This is the padding size of scan scale. it will use rotation case in genup task.
    uint32_t image_top_padding;			// This is the padding size of top of image. it will use rotation case in genup task.
    
    int32_t image_engshift;				// it will be dead code.
	uint8_t sourceAPP;
    image_resolution_t image_dpi;       // x and y resolution of the image
    bool left_to_right;                 // Direction of the data in the image
} image_info_t;
#endif // PRINT_JOB_TYPES_H

typedef	enum {
	PCODE_INDEX_1BIT,
	PCODE_INDEX_2BIT,
	PCODE_INDEX_4BIT,
	PCODE_INDEX_ED1BIT,
	PCODE_INDEX_ED2BIT,
	PCODE_INDEX_ED4BIT,
} e_pcode_index;

 
#define LASER_VIDEO_PAGE_COOKIE (0x4C565047) // ascii hex LVPG

typedef struct _laser_video_page
{
    uint32_t page_id;
    void* page_context;
    image_info_t image_info;           ///< Dimensions of the output print data (to be printed)
    page_color_mode_t page_output_color_mode; ///< Color mode of the printed output (mono or color)
    justification_mode_t  video_justification_mode;  ///< video just, e.g. auto, centered, paired
    bool top_to_bottom;                 ///< Output video strips from top to bottom if true; else bottom to top
    uint32_t halftone_type;             ///< Type of halftone used on this page; threshold or error diffusion
    uint32_t side_margin_in_subpixels[MAX_NUM_VIDEO_CHAN];
    uint32_t top_margin_in_lines[MAX_MARKING_COLOR_CHANNELS];
    uint32_t cookie;                    ///< cookie for structure validation; must be last item in structure
}laser_video_page_t;

#define LASER_VIDEO_PLANE_COOKIE (0x4C56504E) // ascii hex LVPN

typedef struct _laser_video_plane
{
    COLOR  sysPlaneColor;  ///< The system representation of the plane color
    uint32_t DataLength;   ///< length of the attached data buffer; note that for compressed data this is not the same
                           ///  as image strip dimensions described by image_info
    bool end_of_image;
    bool start_of_image;

    // TODO: convert back to using bigbuffer for portability back to threadx
//-jrs    BigBuffer_t *big_buffer;
    dma_alloc_t dma_buffer;
    compression_info_t compression_info;
    image_info_t image_info;
    uint32_t cookie;        ///< cookie for structure validation; must be last item in structure
}laser_video_plane_t;

static inline bool is_laser_video_page(laser_video_page_t* page)
{
    return LASER_VIDEO_PAGE_COOKIE == page->cookie;
}

static inline void set_laser_video_page_cookie(laser_video_page_t* page)
{
    page->cookie = LASER_VIDEO_PAGE_COOKIE;
}

static inline void clear_laser_video_page_cookie(laser_video_page_t* page)
{
    page->cookie = -1;
}

static inline bool is_laser_video_plane(laser_video_plane_t* plane)
{
    return LASER_VIDEO_PLANE_COOKIE == plane->cookie;
}

static inline void set_laser_video_plane_cookie(laser_video_plane_t* plane)
{
    plane->cookie = LASER_VIDEO_PLANE_COOKIE;
}

static inline void clear_laser_video_plane_cookie(laser_video_plane_t* plane)
{
    plane->cookie = -1;
}

#endif //_LASER_VIDEO_TYPES_H_

