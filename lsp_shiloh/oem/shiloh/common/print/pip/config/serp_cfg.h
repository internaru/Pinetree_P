/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the SERP block config.
  * 
 **/

#ifndef __SERP_CFG_HEADER
#define __SERP_CFG_HEADER

#include <stdint.h>

typedef struct serp_cfg_struct 
{
    uint32_t bypass;
    uint32_t serpentine_data;
    uint32_t serp_start_dir;
    uint32_t text_merge_enable;
    uint32_t hr0_en;
    uint32_t hr1_en;
    uint32_t black_core_en;
    uint32_t edge_strength_en;
    uint32_t obj_merge_enable;
    uint32_t obj_encoded_select;
    uint32_t hr0_outchan;
    uint32_t hr0_outpos;
    uint32_t hr0_inconfig;
    uint32_t hr0_inchan;
    uint32_t hr0_bitwidth;
    uint32_t hr0_bitpos;
    uint32_t hr1_outchan;
    uint32_t hr1_outpos;
    uint32_t hr1_inconfig;
    uint32_t hr1_inchan;
    uint32_t hr1_bitwidth;
    uint32_t hr1_bitpos;
    uint32_t color_thresh[6];
    uint32_t contone_h_scale;
    uint32_t h_bilinear;
    uint32_t contone_v_scale;
    uint32_t v_bilinear;
    uint32_t text_h_scale;
    uint32_t text_v_scale;
    uint32_t text_threshold;
    uint32_t text_replace_mode;
    uint32_t text_source;
    uint32_t text_chan;
    uint32_t text_bits;
    uint32_t text_bit_position;
    uint32_t text_map_0;
    uint32_t text_map_1;
    uint32_t text_map_2;
    uint32_t text_map_3;
    uint32_t black_core_ch5_en;
    uint32_t black_core_ch3_en;
    uint32_t black_core_ch2_en;
    uint32_t black_core_ch1_en;
    uint32_t black_core_ch0_en;
    uint32_t black_core_thresh;
    uint32_t edge_out_msb;
    uint32_t edge_in_msb;
    uint32_t edge_vscale;
    uint32_t edge_hscale;
    uint32_t edge_zero;
    uint32_t edge_invert_data;
    uint32_t edge_thresh;
    uint32_t edge_scale;
    uint32_t edge_offset;
    uint32_t high_res_text_enable;
    uint32_t text_res;
    uint32_t high_res_source;
} serp_cfg_t;

extern serp_cfg_t serp_bypass_cfg;
extern serp_cfg_t serp_basic_cfg;
extern serp_cfg_t serp_testpip2_cfg;
extern serp_cfg_t serp_testpip2_a_cfg;
extern serp_cfg_t serp_ht_2bpp_cfg;
extern serp_cfg_t serp_ht_1bpp_cfg;
extern serp_cfg_t serp_balloons_cfg;
extern serp_cfg_t serp_generic_cfg;
extern serp_cfg_t serp_300_cfg;
// following is serp doubling configuration
extern serp_cfg_t serp_genx2_cfg;
// following is serp quadrupling configuration
extern serp_cfg_t serp_genx4_cfg;
// following is serp text merge configuration
extern serp_cfg_t serp_text_merge_cfg;

#endif
