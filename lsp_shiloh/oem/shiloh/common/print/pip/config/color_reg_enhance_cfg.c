/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file color_reg_enhance_cfg.c
 * 
 * \brief
 * 
 **/

#include "color_reg_enhance_cfg.h"


// Bypass configuration - set bypass to true and let the compiler fill the rest
// of the variable with zeroes. The enable for each color MUST be set to get
// data through the block.
color_reg_enhance_cfg_t color_reg_enhance_bypass_cfg =
{
    .control.bypass_all = true,
    .control.hx2        =  false,
    .control.vx2        =  false,
    .control.enable_cpu_access_to_luts = false,
    .control.cyan_channel_enable    = true,
    .control.magenta_channel_enable = true,
    .control.yellow_channel_enable  = true,
    .control.black_channel_enable   = true,
};
color_reg_enhance_cfg_t color_reg_enhance_print_cfg =
{
    .control.bypass_all =  false,
    .control.enable_cpu_access_to_luts = false,
    .control.cyan_channel_enable    = true,
    .control.magenta_channel_enable = true,
    .control.yellow_channel_enable  = true,
    .control.black_channel_enable   = true,

    .trap_control.enable = true, 
    .trap_control.enable_black_trapping_only = false,
    .trap_control.enable_detection_of_edges_one_pixel_away    = true,
    .trap_control.enable_detection_of_edges_two_pixels_away   = true,
    .trap_control.enable_non_vert_horiz_k_line_detection      = true,
    .trap_control.enable_one_pixel_wide_k_line_detection      = true,
    .trap_control.enable_trapping_under_both_sides_of_an_edge = true,
    .trap_control.enable_two_pixel_wide_k_line_detection      = true,
    .trap_control.trap_object_type_mode = 0,

    .cyan_trap_threshold    =  16,
    .magenta_trap_threshold =  16,
    .yellow_trap_threshold  =  16,
    .black_trap_threshold   =  32,

    .trap_intensity_percent_limit =  5,
    .trap_intensity_threshold =  255,

    .trap_k_line_threshold =  32,
    .adjacent_k_pixels_per_line =  2,
    .max_trapping_limit = 0x3ff,

    .trap_cyan_intensity_lut    = { 0, 11, 23, 34, 45, 57, 68, 79, 91, 102, 113, 125, 136, 147, 159, 170 },
    // .trap_cyan_intensity_lut    = {0, 7, 14, 20, 27, 34, 41, 48, 54, 61, 68, 75, 82, 88, 95, 102},
    .trap_magenta_intensity_lut    = { 0, 11, 23, 34, 45, 57, 68, 79, 91, 102, 113, 125, 136, 147, 159, 170 },
    //.trap_magenta_intensity_lut = {0, 7, 14, 20, 27, 34, 41, 48, 54, 61, 68, 75, 82, 88, 95, 102 },
    .trap_yellow_intensity_lut  = {0, 4, 7, 11, 15, 18, 22, 26},
    .trap_black_intensity_lut   = {0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132, 140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255},

    .trap_cyan_fade_lut    = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},  //{0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15},
    .trap_magenta_fade_lut = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, // {0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15},
    .trap_yellow_fade_lut  = {0, 1, 2, 3, 4, 5, 6, 7}, // {0, 0, 1, 1, 2, 4, 5, 7},
    .trap_black_fade_lut   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
                            // {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7, 8, 9, 10, 12, 13, 14, 16, 17, 19, 20, 22, 24, 25, 27, 31},

    .sharpen_control.enable = false, // true,
    .sharpen_control.object_type_mode = 0,

    .cyan_sharpen_scale_factor    = 32,
    .magenta_sharpen_scale_factor = 32,
    .yellow_sharpen_scale_factor  = 32,
    .black_sharpen_scale_factor   = 32,

    .cyan_sharpen_offset =  32,
    .magenta_sharpen_offset =  32,
    .yellow_sharpen_offset =  32,
    .black_sharpen_offset =  32,

    .edge_strength_detect_enable = false, // true,

    .cyan_edge_strength_threshold =  12,
    .magenta_edge_strength_threshold =  6,
    .yellow_edge_strength_threshold =  6,
    .black_edge_strength_threshold =  2,

    .cyan_edge_strength_scale_factor =  2,
    .magenta_edge_strength_scale_factor =  2,
    .yellow_edge_strength_scale_factor =  2,
    .black_edge_strength_scale_factor =  2,

    .cyan_edge_strength_offset    = 200,
    .magenta_edge_strength_offset = 200,
    .yellow_edge_strength_offset  = 440,
    .black_edge_strength_offset   = 100,

    .coring_control.enable = false, // true,
    .coring_control.object_type_mode = 0,
    .coring_control.enable_checkerboard = true,
    .coring_control.mode = 0,

    .cyan_coring_threshold    = 200,
    .magenta_coring_threshold = 200,
    .yellow_coring_threshold  = 200,
    .black_coring_threshold   = 200,

    .cmyk_max_coring_threshold = 400,

    .cyan_coring_scale_factor = 128,
    .magenta_coring_scale_factor = 128,
    .yellow_coring_scale_factor = 128,
    .black_coring_scale_factor = 128,

    .pile_control.enable = false, // true,
    .pile_control.object_type_mode = 0,
    .pile_control.leading_edge_pile_mode = 0,
    .pile_control.trailing_edge_pile_mode = 0,

    .pile_edge_threshold = 50,

    .cyan_pile_threshold     = 204,
    .magenta_pile_threshold = 204,
    .yellow_pile_threshold   = 204,
    .black_pile_threshold    = 204,

    .cyan_leading_edge_pile_scale    = 7,
    .magenta_leading_edge_pile_scale = 7,
    .yellow_leading_edge_pile_scale  = 7,
    .black_leading_edge_pile_scale   = 7,

    .cyan_trailing_edge_pile_scale    = 7,
    .magenta_trailing_edge_pile_scale = 7,
    .yellow_trailing_edge_pile_scale  = 7,
    .black_trailing_edge_pile_scale   = 7,
};


color_reg_enhance_cfg_t color_reg_enhance_default_cfg =
{
    .control.bypass_all =  false,
    .control.hx2        =  false,
    .control.vx2        =  false,
    .control.enable_cpu_access_to_luts = false,
    .control.cyan_channel_enable    = true,
    .control.magenta_channel_enable = true,
    .control.yellow_channel_enable  = true,
    .control.black_channel_enable   = true,

    .trap_control.enable =  false, //true,
    .trap_control.enable_black_trapping_only = false,
    .trap_control.enable_detection_of_edges_one_pixel_away    = true,
    .trap_control.enable_detection_of_edges_two_pixels_away   = true,
    .trap_control.enable_non_vert_horiz_k_line_detection      = true,
    .trap_control.enable_one_pixel_wide_k_line_detection      = true,
    .trap_control.enable_trapping_under_both_sides_of_an_edge = true,
    .trap_control.enable_two_pixel_wide_k_line_detection      = true,
    .trap_control.trap_object_type_mode = 0,

    .cyan_trap_threshold    =  16, //20,
    .magenta_trap_threshold =  16, //20,
    .yellow_trap_threshold  =  16, //33,
    .black_trap_threshold   =  32, //20,

    .trap_intensity_percent_limit =  5, // 10,
    .trap_intensity_threshold =  255, // 20,

    .trap_k_line_threshold =  32, //200,
    .adjacent_k_pixels_per_line =  2,
    .max_trapping_limit = 0x3ff,

    .trap_cyan_intensity_lut    = { 0, 11, 23, 34, 45, 57, 68, 79, 91, 102, 113, 125, 136, 147, 159, 170 },
    // .trap_cyan_intensity_lut    = {0, 7, 14, 20, 27, 34, 41, 48, 54, 61, 68, 75, 82, 88, 95, 102},
    .trap_magenta_intensity_lut    = { 0, 11, 23, 34, 45, 57, 68, 79, 91, 102, 113, 125, 136, 147, 159, 170 },
    //.trap_magenta_intensity_lut = {0, 7, 14, 20, 27, 34, 41, 48, 54, 61, 68, 75, 82, 88, 95, 102 },
    .trap_yellow_intensity_lut  = {0, 4, 7, 11, 15, 18, 22, 26},
    .trap_black_intensity_lut   = {0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132, 140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255},

    .trap_cyan_fade_lut    = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},  
                           //{0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15},
    .trap_magenta_fade_lut = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
                           //{0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15},
    .trap_yellow_fade_lut  = {0, 1, 2, 3, 4, 5, 6, 7}, // {0, 0, 1, 1, 2, 4, 5, 7},
    .trap_black_fade_lut   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
                          // {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7, 8, 9, 10, 12, 13, 14, 16, 17, 19, 20, 22, 24, 25, 27, 31},


    .sharpen_control.enable = false, // true,
    .sharpen_control.object_type_mode = 0,

    .cyan_sharpen_scale_factor    = 128,
    .magenta_sharpen_scale_factor = 128,
    .yellow_sharpen_scale_factor  = 128,
    .black_sharpen_scale_factor   = 128,

    .cyan_sharpen_offset =  64,
    .magenta_sharpen_offset =  64,
    .yellow_sharpen_offset =  64,
    .black_sharpen_offset =  64,

    .edge_strength_detect_enable =  true,

    .cyan_edge_strength_threshold =  0x3ff,
    .magenta_edge_strength_threshold = 0x3ff,
    .yellow_edge_strength_threshold =  0x3ff,
    .black_edge_strength_threshold =  0x3ff,

    .cyan_edge_strength_scale_factor =  128,
    .magenta_edge_strength_scale_factor = 128,
    .yellow_edge_strength_scale_factor =  128,
    .black_edge_strength_scale_factor =  16, // 32, // 64, 128,

    .cyan_edge_strength_offset    = 64,
    .magenta_edge_strength_offset = 64,
    .yellow_edge_strength_offset  = 64,
    .black_edge_strength_offset   = 128 , // 64,

    .coring_control.enable = false, //true,
    .coring_control.object_type_mode = 0,
    .coring_control.enable_checkerboard = true,
    .coring_control.mode = 0,

    .cyan_coring_threshold    = 200,
    .magenta_coring_threshold = 200,
    .yellow_coring_threshold  = 200,
    .black_coring_threshold   = 200,

    .cmyk_max_coring_threshold = 400,

    .cyan_coring_scale_factor = 128,
    .magenta_coring_scale_factor = 128,
    .yellow_coring_scale_factor = 128,
    .black_coring_scale_factor = 128,

    .pile_control.enable = false, // true,
    .pile_control.object_type_mode = 0,
    .pile_control.leading_edge_pile_mode = 0,
    .pile_control.trailing_edge_pile_mode = 0,

    .pile_edge_threshold = 50,

    .cyan_pile_threshold     = 204,
    .magenta_pile_threshold = 204,
    .yellow_pile_threshold   = 204,
    .black_pile_threshold    = 204,

    .cyan_leading_edge_pile_scale    = 7,
    .magenta_leading_edge_pile_scale = 7,
    .yellow_leading_edge_pile_scale  = 7,
    .black_leading_edge_pile_scale   = 7,

    .cyan_trailing_edge_pile_scale    = 7,
    .magenta_trailing_edge_pile_scale = 7,
    .yellow_trailing_edge_pile_scale  = 7,
    .black_trailing_edge_pile_scale   = 7,
};
