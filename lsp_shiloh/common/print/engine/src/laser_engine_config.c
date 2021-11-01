/*
 * ============================================================================
 * Copyright (c) 2010-2011 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <string.h>

#include "laser_engine_config_api.h"
#include "agprint.h" 
#include "lassert.h"
#include "dprintf.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

// The following sample configuration is for a single beam, monochrome laser printer.

// HIPWM clock frequency in Hz
#define HIPWM_CLOCK_FREQ 156.25E6
#define PIXEL_FREQ 16441414
#define BD_PERIOD 591E-6
#define CENTERLINE 3752
#define TOP_OFFSET 50
#define BD_LOCKOUT_PERIOD (uint32_t)(0.95 * BD_PERIOD * HIPWM_CLOCK_FREQ)
#define NUM_PRINT_VIDEO_CHANNELS 1

// The following LUT tables are default values for the video pulse codes for 
// various bit depths. It is recommended that a copy be created of these tables
// and modified as appropriate for a particular application. 
#ifdef HAVE_ENGINE_STUB

static const uint32_t pulse_code_lut_1_bit[NUM_PCODE_REGS] = {
                            0, 0, 0x3F3F3F3F, 0x3F3F3F3F,   // Center 00 justification
                            0, 0, 0x7F7F7F7F, 0x7F7F7F7F,   // Late 01 justification
                            0, 0, 0xBFBFBFBF, 0xBFBFBFBF,   // Early 10 justification
                            0, 0, 0xFFFFFFFF, 0xFFFFFFFF};  // Split 11 justification

//  Default 2 bit table. "OR" in the justification bits below
static const uint32_t pulse_code_lut_2_bit[NUM_PCODE_REGS] = {
                                0, 0x15151515, 0x2a2a2a2a, 0x3f3f3f3f,
				0, 0x15151515 | 0x40404040, 0x2a2a2a2a | 0x40404040, 0x3f3f3f3f | 0x40404040,
				0, 0x15151515 | 0x80808080, 0x2a2a2a2a | 0x80808080, 0x3f3f3f3f | 0x80808080,
                                0, 0x15151515 | 0xC0C0C0C0, 0x2a2a2a2a | 0xC0C0C0C0, 0x3f3f3f3f | 0xC0C0C0C0};


// Default 4 bit table. "OR" in the justification bits below
// REVISIT are the values in the right order?
static const uint32_t pulse_code_lut_4_bit[NUM_PCODE_REGS]= {
                                0x0C080400, // 00 center justification
                                0x1C181410, 
                                0x2C282420,
                                0x3F383430,
                                0x0C080400 | 0x40404040, // 01  left or early justification
                                0x1C181410 | 0x40404040, 
                                0x2C282420 | 0x40404040,
                                0x3F383430 | 0x40404040,
                                0x0C080400 | 0x80808080, // 10  right or latejustification
                                0x1C181410 | 0x80808080, 
                                0x2C282420 | 0x80808080,
                                0x3F383430 | 0x80808080,
                                0x0C080400 | 0xC0C0C0C0, // 11 split justification
                                0x1C181410 | 0xC0C0C0C0, 
                                0x2C282420 | 0xC0C0C0C0,
                                0x3F383430 | 0xC0C0C0C0};


static laser_engine_video_config_t video_platform_config = 
{
    .num_print_planes = 1,
    .num_beams_per_print_plane = 1,
    .num_video_channels = NUM_VIDEO_CHAN,
    .num_print_colors = NUM_MARKING_COLOR_CHANNELS,
    .top_to_print_plane = {0, 0},
    .report_video_event = {true, false},
    .chan_to_print_plane_color = {COLOR_BLACK, COLOR_BLACK},
//    .chan_to_print_plane = {0, 0},
//    .color_to_print_plane = {0},
    .active_video_channels =  {true, false}, // Video channel 0 is the only active channel for this print engine.
//    .beam_to_video_hw_chan = {0, 0},
    .video_buffer_size_in_bytes = 0x80000,
    .num_video_descriptors = 4,
    .sub_pixel_denominator = 1,

    .color_process_order = {{COLOR_BLACK}}, 
    .beam_direction = {LEFT_TO_RIGHT, 0},
        
    .burst_size = BYTES_16,
    .serpentine = false,
    .video_processing = MONOCHROME,
    .hsync_lockout_period_in_hipwm_clocks = 0,  
    .print_engine_native_resolution = DPI_600,

    //.top_of_page_marigin_in_lines = {TOP_OFFSET},
    .side_margin_reference_in_sub_pixels = {CENTERLINE},
    .pixel_freq_in_hz = {PIXEL_FREQ},
    .power_down_lvdo_between_pages = false,
};

error_type_t engine_get_video_config(laser_engine_video_config_t **config)
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

    *config = &video_platform_config;
    return OK;
}

#endif


