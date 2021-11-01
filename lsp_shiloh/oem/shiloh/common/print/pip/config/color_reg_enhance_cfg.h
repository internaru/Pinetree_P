/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file color_reg_enhance_cfg.h
 * 
 * \brief
 * 
 **/
 
#ifndef COLOR_REG_ENHANCE_CFG_H
#define COLOR_REG_ENHANCE_CFG_H

#include <stdbool.h>
#include <stdint.h>
 

// Control register
typedef struct cre_cntrl_s
{
    bool bypass_all;
    bool hx2;
    bool vx2;
    bool enable_cpu_access_to_luts;
    bool cyan_channel_enable;
    bool magenta_channel_enable;
    bool yellow_channel_enable;
    bool black_channel_enable;
} cre_cntrl_t;


// Trapping control register
typedef struct cre_trap_cntrl_s
{
    bool enable;
    uint8_t trap_object_type_mode;
    bool enable_trapping_under_both_sides_of_an_edge;
    bool enable_non_vert_horiz_k_line_detection;
    bool enable_one_pixel_wide_k_line_detection;
    bool enable_two_pixel_wide_k_line_detection;
    bool enable_detection_of_edges_two_pixels_away;
    bool enable_detection_of_edges_one_pixel_away;
    bool enable_black_trapping_only;
} cre_trap_cntrl_t;


typedef struct cre_coring_control_s
{
    bool enable;
    uint8_t object_type_mode;
    bool enable_checkerboard;
    bool mode;
} cre_coring_control_t;


typedef struct cre_pile_control_s
{
    bool enable;
    uint8_t object_type_mode;
    bool trailing_edge_pile_mode;
    bool leading_edge_pile_mode;
} cre_pile_control_t;

typedef struct cre_sharpening_control_s
{
    bool enable;
    uint8_t object_type_mode;
} cre_sharpening_control_t;


// The BIG structure containing the configuration for the entire block
typedef struct cre_config_s
{
    cre_cntrl_t control;

    cre_trap_cntrl_t trap_control;

    uint8_t cyan_trap_threshold;
    uint8_t magenta_trap_threshold;
    uint8_t yellow_trap_threshold;
    uint8_t black_trap_threshold;

    uint8_t trap_intensity_percent_limit;
    uint8_t trap_intensity_threshold;

    uint8_t trap_k_line_threshold;
    uint8_t adjacent_k_pixels_per_line;
    uint16_t max_trapping_limit;

    uint8_t trap_cyan_intensity_lut[16];
    uint8_t trap_magenta_intensity_lut[16];
    uint8_t trap_yellow_intensity_lut[8];
    uint8_t trap_black_intensity_lut[32];

    uint8_t trap_cyan_fade_lut[16];
    uint8_t trap_magenta_fade_lut[16];
    uint8_t trap_yellow_fade_lut[8];
    uint8_t trap_black_fade_lut[32];

    cre_sharpening_control_t sharpen_control;

    uint8_t cyan_sharpen_scale_factor;
    uint8_t magenta_sharpen_scale_factor;
    uint8_t yellow_sharpen_scale_factor;
    uint8_t black_sharpen_scale_factor;

    uint8_t cyan_sharpen_offset;
    uint8_t magenta_sharpen_offset;
    uint8_t yellow_sharpen_offset;
    uint8_t black_sharpen_offset;

    bool edge_strength_detect_enable;

    uint16_t cyan_edge_strength_threshold;
    uint16_t magenta_edge_strength_threshold;
    uint16_t yellow_edge_strength_threshold;
    uint16_t black_edge_strength_threshold;

    uint8_t cyan_edge_strength_scale_factor;
    uint8_t magenta_edge_strength_scale_factor;
    uint8_t yellow_edge_strength_scale_factor;
    uint8_t black_edge_strength_scale_factor;

    uint16_t cyan_edge_strength_offset;
    uint16_t magenta_edge_strength_offset;
    uint16_t yellow_edge_strength_offset;
    uint16_t black_edge_strength_offset;

    cre_coring_control_t coring_control;

    uint8_t cyan_coring_threshold;
    uint8_t magenta_coring_threshold;
    uint8_t yellow_coring_threshold;
    uint8_t black_coring_threshold;

    uint16_t cmyk_max_coring_threshold;

    uint8_t cyan_coring_scale_factor;
    uint8_t magenta_coring_scale_factor;
    uint8_t yellow_coring_scale_factor;
    uint8_t black_coring_scale_factor;

    cre_pile_control_t pile_control;

    uint16_t pile_edge_threshold;

    uint8_t cyan_pile_threshold;
    uint8_t magenta_pile_threshold;
    uint8_t yellow_pile_threshold;
    uint8_t black_pile_threshold;

    uint8_t cyan_leading_edge_pile_scale;
    uint8_t magenta_leading_edge_pile_scale;
    uint8_t yellow_leading_edge_pile_scale;
    uint8_t black_leading_edge_pile_scale;

    uint8_t cyan_trailing_edge_pile_scale;
    uint8_t magenta_trailing_edge_pile_scale;
    uint8_t yellow_trailing_edge_pile_scale;
    uint8_t black_trailing_edge_pile_scale;

} color_reg_enhance_cfg_t;

extern color_reg_enhance_cfg_t color_reg_enhance_default_cfg;
extern color_reg_enhance_cfg_t color_reg_enhance_bypass_cfg;
extern color_reg_enhance_cfg_t color_reg_enhance_print_cfg;
#endif
