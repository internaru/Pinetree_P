/*
 * ============================================================================
 * Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <string.h>
#include "laser_engine_config_api.h"
#include "asic.h"
#include "agprint.h" // REVISIT DAB for COLOR definition, should move!!!
#include "lassert.h"
#include "spec.h"
  
// HIPWM clock frequency in Hz
#define HIPWM_CLOCK_FREQ 156.25E6

#if MAC_TYPE_M
#define PIXEL_FREQ  22524692        // 40ppm
#else
#define PIXEL_FREQ  24829692        // 47ppm          // from lscan_45ppm.xls spreadsheet
#endif

#define BD_PERIOD     284E-6

#define NUM_PRINT_VIDEO_CHANNELS 2

#define SUB_PIXEL_DENOM       64
#define CENTERLINE            2962      // middle of page in pixels
#define BEAM_OFFSET_PIXELS    3     // pixel offset for beam 1 from beam 0
#define BEAM_OFFSET_SUBPIXELS 0     // subpixel offset for beam 1, in units of SUB_PIXEL_DENOM (64ths).

// is this lines ? 
// #define TOP_OFFSET 100      // lscan_45ppm.xls spreadsheet says 276
#define TOP_OFFSET  170     //  100 + 10mm swag -> KARA Machine 130->170 @@2013.01.25. By CSPark

// Offset between beams in lines
#define BEAM_OFFSET_LINES 1

#define BD_LOCKOUT_PERIOD (uint32_t)(0.95 * BD_PERIOD * HIPWM_CLOCK_FREQ)

// The following LUT tables are default values for the video pulse codes for 
// various bit depths. It is recommended that a copy be created of these tables
// and modified as appropriate for a particular application. 

//-jrs#define PWM_1BIT   0x2c2c2c2c

/* Beam Power : @@2013.03.05. By CSPark */
//#define PWM_1BIT    0x3f3f3f3f	/* 100% */
//#define PWM_1BIT    0x35353535    /* 84% */
//#define PWM_1BIT    0x33333333    /* 80% */
#define PWM_1BIT    0x30303030	/*  75% */
//#define PWM_1BIT    0x29292929    /* 65% */
//#define PWM_1BIT    0x20202020		/*  50% */
//#define PWM_1BIT    0x1D1D1D1D		/*  45% */
//#define PWM_1BIT    0x19191919		/*  40% */
//#define PWM_1BIT    0x17171717      /*  37% */
//#define PWM_1BIT    0x13131313      /*  30% */

static const uint32_t pulse_code_lut_1_bit[NUM_PCODE_REGS] = {
  0, 0, PWM_1BIT,              PWM_1BIT,               // 00 center just
  0, 0, PWM_1BIT | 0x40404040, PWM_1BIT | 0x40404040,  // 01 left/early just
  0, 0, PWM_1BIT | 0x80808080, PWM_1BIT | 0x80808080,  // 10 right/late just
  0, 0, PWM_1BIT | 0xC0C0C0C0, PWM_1BIT | 0xC0C0C0C0}; // 11 split just

#define PWM_1BIT_EM   0x1A1A1A1A
static const uint32_t pulse_code_lut_bit_draft[NUM_PCODE_REGS] = {
  0, 0, PWM_1BIT_EM,            PWM_1BIT_EM,             // 00 center just
  0, 0, PWM_1BIT_EM|0x40404040, PWM_1BIT_EM|0x40404040,  // 01 left/early just
  0, 0, PWM_1BIT_EM|0x80808080, PWM_1BIT_EM|0x80808080,  // 10 right/late just
  0, 0, PWM_1BIT_EM|0xC0C0C0C0, PWM_1BIT_EM|0xC0C0C0C0}; // 11 split just

                            
//  Default 2 bit table. "OR" in the justification bits below

// daj 23 Dec 2011: Adjust values for 4729 engine.  My testing shows we cannot
// reliably hold a dot at 15, and 17 looks dubious, but 19 looks decent.
// Linear midpoint pulse would be 2C.
// Measured a 87% gray using 3f at 183/255, using 19 at 232/255.  Desired
// midpoint pulse should yield ~207/255, which is about a 26 pulse.
#if 1
#define PWM_2BIT_0   0x00000000
#define PWM_2BIT_1   0x19191919
#define PWM_2BIT_2   0x26262626
#define PWM_2BIT_3   0x3f3f3f3f
#else
#define PWM_2BIT_0   0x00000000
#define PWM_2BIT_1   0x13131313
#define PWM_2BIT_2   0x17171717
#define PWM_2BIT_3   0x19191919
#endif
static const uint32_t pulse_code_lut_2_bit[NUM_PCODE_REGS] = {
  PWM_2BIT_0, PWM_2BIT_1, PWM_2BIT_2, PWM_2BIT_3,      // 00 center just
  PWM_2BIT_0 | 0x40404040, PWM_2BIT_1 | 0x40404040,    // 01 left/early just
  PWM_2BIT_2 | 0x40404040, PWM_2BIT_3 | 0x40404040,
  PWM_2BIT_0 | 0x80808080, PWM_2BIT_1 | 0x80808080,    // 10 right/late just
  PWM_2BIT_2 | 0x80808080, PWM_2BIT_3 | 0x80808080,
  PWM_2BIT_0 | 0xC0C0C0C0, PWM_2BIT_1 | 0xC0C0C0C0,    // 11 split just
  PWM_2BIT_2 | 0xC0C0C0C0, PWM_2BIT_3 | 0xC0C0C0C0};


//  draft 2 bit table. "OR" in the justification bits below
static const uint32_t pulse_code_lut_2_bit_draft[NUM_PCODE_REGS] = {
  0, 0x06060606,  0x11111111, 0x26262626,
  0, 0x06060606 | 0x40404040, 0x11111111 | 0x40404040, 0x26262626 | 0x40404040,
  0, 0x06060606 | 0x80808080, 0x11111111 | 0x80808080, 0x26262626 | 0x80808080,
  0, 0x06060606 | 0xC0C0C0C0, 0x11111111 | 0xC0C0C0C0, 0x26262626 | 0xC0C0C0C0};

// Default 4 bit table.
#define PWM_4BIT_0   0x0C080400
#define PWM_4BIT_1   0x1C181410
#define PWM_4BIT_2   0x2C282420
#define PWM_4BIT_3   0x3F383430
static const uint32_t pulse_code_lut_4_bit[NUM_PCODE_REGS]= {
  PWM_4BIT_0, PWM_4BIT_1, PWM_4BIT_2, PWM_4BIT_3,      // 00 center just
  PWM_4BIT_0 | 0x40404040, PWM_4BIT_1 | 0x40404040,    // 01 left/early just
  PWM_4BIT_2 | 0x40404040, PWM_4BIT_3 | 0x40404040,
  PWM_4BIT_0 | 0x80808080, PWM_4BIT_1 | 0x80808080,    // 10 right/late just
  PWM_4BIT_2 | 0x80808080, PWM_4BIT_3 | 0x80808080,
  PWM_4BIT_0 | 0xC0C0C0C0, PWM_4BIT_1 | 0xC0C0C0C0,    // 11 split just
  PWM_4BIT_2 | 0xC0C0C0C0, PWM_4BIT_3 | 0xC0C0C0C0};

// draft 4 bit table. "OR" in the justification bits below
// REVISIT are the values in the right order?
static const uint32_t pulse_code_lut_4_bit_draft[NUM_PCODE_REGS]= {
                                0x06040200, // 00 center justification
                                0x0E0C0A08, 
                                0x16141210,
                                0x2A282624,
                                0x06040200 | 0x40404040, // 01  left or early justification
                                0x0E0C0A08 | 0x40404040, 
                                0x16141210 | 0x40404040,
                                0x2A282624 | 0x40404040,
                                0x06040200 | 0x80808080, // 10  right or latejustification
                                0x0E0C0A08 | 0x80808080, 
                                0x16141210 | 0x80808080,
                                0x2A282624 | 0x80808080,
                                0x06040200 | 0xC0C0C0C0, // 11 split justification
                                0x0E0C0A08 | 0xC0C0C0C0, 
                                0x2C282420 | 0xC0C0C0C0,
                                0x2A282624 | 0xC0C0C0C0};


static laser_engine_video_config_t video_platform_config = 
{
    
    .num_print_planes = 1,
    .num_beams_per_print_plane = 2,
    .num_video_channels = NUM_PRINT_VIDEO_CHANNELS,
    .num_print_colors = NUM_MARKING_COLOR_CHANNELS,
    .report_video_event = {false, true}, // Use beam 1 to report all video  events for BOTH beams
    .chan_to_print_plane_color = {COLOR_BLACK, COLOR_BLACK},
    .top_to_print_plane = {0, 0},        // both beams use top signal 0
    .video_chan_hsync_input = {0, 0, 0, 0, 0, 0, 0, 0},
    .topq_hsync_sel = {0, 0, 0, 0},
    .active_video_channels = {true, true}, // Dual beam engine, both channels are active
    .beam_to_video_hw_chan = {{0, 1}},
    .video_buffer_size_in_bytes = 0x80000*8*2,
    .num_video_descriptors = 4,
    .sub_pixel_denominator = SUB_PIXEL_DENOM,

    .color_process_order = {{COLOR_BLACK}}, 
    .beam_direction = {LEFT_TO_RIGHT, LEFT_TO_RIGHT},
        
    .burst_size = BYTES_32,
    .serpentine = false,
    .video_processing = MONOCHROME,
    .hsync_lockout_period_in_hipwm_clocks = 0, // REVISIT 
    .invert_video = false,
    .print_engine_native_resolution = DPI_600,

//    .pulse_code_1_bit_array = {pulse_code_lut_1_bit, pulse_code_lut_1_bit},
//    .pulse_code_2_bit_array = {pulse_code_lut_2_bit, pulse_code_lut_2_bit},
//    .pulse_code_4_bit_array = {pulse_code_lut_4_bit, pulse_code_lut_4_bit},

    .top_of_page_skew_in_lines = {0, BEAM_OFFSET_LINES},

    .top_of_page_margin_in_lines = {TOP_OFFSET},

    .side_margin_reference_in_sub_pixels = { SUB_PIXEL_DENOM * CENTERLINE,
                                             SUB_PIXEL_DENOM * (CENTERLINE + BEAM_OFFSET_PIXELS) + BEAM_OFFSET_SUBPIXELS},
    .pixel_freq_in_hz = {PIXEL_FREQ, PIXEL_FREQ},
    .power_down_lvdo_between_pages = false,

    // Mirror configuration
    .num_mirrors = 1,
    .num_mirror_facets = {4},
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

