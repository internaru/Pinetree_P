/*
 * ============================================================================
 * Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _LASER_ENGINE_CONFIG_API_H_
#define _LASER_ENGINE_CONFIG_API_H_

#include <stdint.h>
#include "asic.h"   
#include "print_job_types.h"
#include "agprint.h" // REVISIT DAB for COLOR definition, should move!!!
#include "video_laser_api.h"

/**
 * \brief Define available DMA burst sizes
 */
typedef enum {  
    BYTES_16 = 16,
    BYTES_32 = 32,
    BYTES_64 = 64
    } burst_size_t;

typedef enum { 
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT
    } beam_direction_t;

typedef enum { 
    COLOR_INLINE,
    COLOR_MULTIPASS,
    MONOCHROME
    } video_processing_t;

/*
typedef enum { DPI_300 = 300,
               DPI_400 = 400,
               DPI_600 = 600,
               DPI_1200 = 1200 } resolution_t;
*/
#define MAX_MARKING_COLOR_CHANNELS 4
#define MAX_PRINT_PROCESSES 2
#define LASER_ENGINE_VIDEO_COOKIE1 0x56434b01   // VCK1
#define LASER_ENGINE_VIDEO_COOKIE2 0x56434b02   // VCK2
typedef struct laser_engine_video_config_s {
    uint32_t cookie1;
    // Product specific configuration. This is a collection of values that do
    // not change at run time and therefore need to only be initialized.
    uint32_t num_print_planes;
    uint32_t num_beams_per_print_plane;
    uint32_t num_video_channels;
    uint32_t num_hsync_inputs;
    uint32_t num_print_colors;
    bool report_video_event[NUM_VIDEO_CHAN];    // Map which channel should reports video events.
                                                // Only select one for multiple beams per color engine

    uint32_t density_cal_side_margin;
    uint32_t density_cal_top_margin;
    uint32_t color_registration_cal_side_margin;
    uint32_t color_registration_cal_top_margin;

    // Characteristics of the print engine
    video_processing_t video_processing;
    resolution_t print_engine_native_resolution; 

    // Map color to a physical print plane
    COLOR color_process_order[MAX_PRINT_PROCESSES][MAX_MARKING_COLOR_CHANNELS];
    beam_direction_t beam_direction[NUM_VIDEO_CHAN];

    // The following array maps the video channel to the print plane; e.g. in a
    // tandem color printer, there are four print planes. There can be more than
    // one video channel per print plane; e.g. dual lasers for improved data
    // throughput.
    COLOR chan_to_print_plane_color[NUM_VIDEO_CHAN];

    // Map the horizontal sync input to the video channel, one hsync can support
    // multiple channels. Hardware will never have more than one hsync input
    // for every two video channels. Refer to the HVSYNC Ouput Select register 
    // documentation for details.
    uint8_t video_chan_hsync_input[NUM_VIDEO_CHAN]; 

    // Select which of the top queues will be used as the top signal for
    // each video channel.  Refer to the HVSYNC Ouput Select register 
    // documentation for details.
    uint8_t top_to_print_plane[NUM_VIDEO_CHAN];

    // Select which HSYNC routes to each of the Top Queues.
    // Refer to the Top Queue HSYNC Select (TQSEL) register documentation for details.
    uint8_t topq_hsync_sel[NUM_HSYNC_INPUTS];

    // Used in concert with "chan_to_print_plane", this is used to determine
    // which video channels are active. Initialize it to "true" for each video
    // channel in use.
    bool active_video_channels[NUM_VIDEO_CHAN];

    // Map the video channel to the color plane and beam for that plane. Each
    // color print plane can have up to two laser beams. This array is addressed
    // using color and beam number; e.g. COLOR_BLACK 0, COLOR_BLACK 1,
    // COLOR_CYAN 0, COLOR_CYAN 1, etc.
    uint8_t beam_to_video_hw_chan[MAX_MARKING_COLOR_CHANNELS][MAX_CHANNELS_PER_COLOR];

    // User configurable values for memory allocated for video buffers and the 
    // number of DMA descriptors. Minimum of two DMA descriptors are required.
    uint32_t video_buffer_size_in_bytes;
    uint8_t num_video_descriptors;

    uint8_t sub_pixel_denominator;

    // The following parameters are settable on a per video channel basis, but
    // it is expected that they will be identical for ALL video channels.
    burst_size_t burst_size;
    bool serpentine;
    uint32_t hsync_lockout_period_in_hipwm_clocks;
    bool invert_video; // Set true if the video signal should be inverted

    // Two sets of pulse codes are supported, one for use with threshold 
    // halftoned data, and the other for error diffusion halftone. Threshold
    // pulse codes are here:
    uint32_t pulse_code_1_bit_array[NUM_PCODE_REGS];
    uint32_t pulse_code_2_bit_array[NUM_PCODE_REGS];
    uint32_t pulse_code_4_bit_array[NUM_PCODE_REGS];

    // And error diffusion pulse codes are here:
    uint32_t ed_pulse_code_1_bit_array[NUM_PCODE_REGS];
    uint32_t ed_pulse_code_2_bit_array[NUM_PCODE_REGS];
    uint32_t ed_pulse_code_4_bit_array[NUM_PCODE_REGS];

    // Top margin for each color.  Indexed by the COLOR enum.
    uint16_t top_of_page_margin_in_lines[MAX_MARKING_COLOR_CHANNELS];

    // The following values are the skew from the top margin for each beam.
    // For a dual beam engine if the beams are offset typically one of the
    // beams would have a skew of 0 and one would have a skew of the offset
    // in lines.
    uint8_t  top_of_page_skew_in_lines[NUM_VIDEO_CHAN];

    // The side margin values will be identical for a multipass color laser printer.
    // They may differ for a multiple beam per plane engine.
    uint32_t side_margin_reference_in_sub_pixels[NUM_VIDEO_CHAN];

    uint32_t pixel_freq_in_hz[NUM_VIDEO_CHAN];  // Expect these all to be the same, except for tandem color

    // some engines require we turn of the LVDO pad after each page to conserver power.
    bool power_down_lvdo_between_pages;

    // Mirror configuration
    uint32_t num_mirrors;
    uint32_t num_mirror_facets[NUM_MIRRORS_MAX];   
    uint32_t cookie2;
} laser_engine_video_config_t;

static inline void engine_video_config_set_cookies(laser_engine_video_config_t* config)
{
    config->cookie1 = LASER_ENGINE_VIDEO_COOKIE1;
    config->cookie2 = LASER_ENGINE_VIDEO_COOKIE2;
}

static inline bool is_engine_video_config_valid(laser_engine_video_config_t* config)
{
    return ((config->cookie1 == LASER_ENGINE_VIDEO_COOKIE1) &&
            (config->cookie2 == LASER_ENGINE_VIDEO_COOKIE2));
}

error_type_t engine_get_video_config(laser_engine_video_config_t **config);

uint32_t engine_get_side_margin_in_subpixels( PAGE_DATA *page,
                                              uint32_t channel_number, 
                                              uint32_t image_width_in_pixels,
                                              resolution_t page_x_resolution );

uint32_t engine_get_default_side_margin_in_subpixels( PAGE_DATA *page,
                                              uint32_t channel_number, 
                                              uint32_t image_width_in_pixels,
                                              resolution_t page_x_resolution );

// implemented in engine code; NOT in config code; for engines that support
// cal this should return the default modified by the cal information.  For
// engines that do not support cal the return value from default can be used.
uint32_t engine_get_top_margin_in_lines( PAGE_DATA *page, COLOR process_color );

// implemented in config code
uint32_t engine_get_default_top_margin_in_lines( PAGE_DATA *page, COLOR process_color );


// The following functions are for getting and setting video pulse code values.
// They support unique pulse codes for threshold and error diffusion halftones.
void engine_set_video_pcode_1Bit( bool threshold_halftone, uint32_t one);
void engine_set_video_pcode_2Bit( bool threshold_halftone, uint32_t one, uint32_t two, uint32_t three );
void engine_set_video_pcode_4Bit( bool threshold_halftone, uint32_t one, uint32_t two, uint32_t three, uint32_t four );

void engine_get_video_pcode_1Bit( bool threshold_halftone, uint32_t * one);
void engine_get_video_pcode_2Bit( bool threshold_halftone, uint32_t * one, uint32_t * two, uint32_t * three );
void engine_get_video_pcode_4Bit( bool threshold_halftone, uint32_t * one, uint32_t * two, uint32_t * three, uint32_t * four );

void engine_setup_pcode_debug_commands( void );

uint32_t engine_get_ppm();

bool engine_supports_slowdown_mode( void );
// True --> In slowdown mode, False --> Normal mode
error_type_t engine_get_slowdown_mode( bool *enabled );
error_type_t engine_set_slowdown_mode( bool enable );

#endif
