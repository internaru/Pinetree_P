/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the UED block config.
  * 
 **/

#ifndef __UED_CFG_HEADER
#define __UED_CFG_HEADER

#if (!defined(ASIC_granite)) && ((!defined(ASIC_gemstone)) || (ASIC_REV < ASIC_REV_Z3))
    #error "This configuration file is for Granite or Gemstone Z3 and later"
#endif

#include <stdint.h>

#define UED_PLUT_DEPTH 1024
#define UED_BLUT_DEPTH 4096
#define UED_CLUT_DEPTH 256
#define UED_WLUT_DEPTH 128

typedef struct blut_data_s
{
    uint32_t* blut0_data;
    uint32_t* blut1_data;
    uint32_t* blut2_data;
    uint32_t* blut3_data;
    uint32_t* blut4_data;
    uint32_t* blut5_data;
} ued_blut_data_t;

typedef struct clut_data_s
{
    uint8_t* clut_bias0_data;
    uint16_t* clut_thresh0_data;
    uint8_t* clut_bias1_data;
    uint16_t* clut_thresh1_data;
    uint8_t* clut_bias2_data;
    uint16_t* clut_thresh2_data;
    uint8_t* clut_bias3_data;
    uint16_t* clut_thresh3_data;
    uint8_t* clut_bias4_data;
    uint16_t* clut_thresh4_data;
    uint8_t* clut_bias5_data;
    uint16_t* clut_thresh5_data;
} ued_clut_data_t;


typedef struct wlut_data_s
{
    uint32_t* wlut0_data;
    uint32_t* wlut1_data;
    uint32_t* wlut2_data;
    uint32_t* wlut3_data;
    uint32_t* wlut4_data;
    uint32_t* wlut5_data;
    uint32_t* wlut6_data;
    uint32_t* wlut7_data;
} ued_wlut_data_t;

typedef struct ued_lut_data_s
{
    ued_blut_data_t *blut;
    ued_clut_data_t *clut;
    ued_wlut_data_t *wlut;
} ued_lut_data_t;

typedef struct ued_cfg_struct 
{
    uint32_t up0_chans;
    uint32_t up0_height;
    uint32_t up0_width;
    uint32_t dn_chans;
    uint32_t dn_height;
    uint32_t dn_width;
    uint32_t mode;
    uint32_t laser_channels_built[6];
    uint32_t th_per_ch;
    uint32_t bypass;
    uint32_t writehexinput;
    uint32_t writehex;
    uint32_t writedv;
    uint32_t removedtfromdvoutput;
    uint32_t keepdvoutwidth;
    uint32_t s0;
    uint32_t s1;
    uint32_t s3;
    uint32_t s4;
    uint32_t h2x;
    uint32_t bline;
    uint32_t bd;
    uint32_t shift_value[6];
    uint32_t lfsr_ch_val[6];
    uint32_t ch_en[6];
    uint32_t ch_max_en[6];
    uint32_t ch_min_en[6];
    uint32_t ch_tdt_en[6];
    uint32_t serpentine;
    uint32_t start_r2l;
    uint32_t forward_error_serpentine;
    uint32_t first_line_error_mode;
    uint32_t scale1;
    uint32_t scale2;
    uint32_t offset;
    uint32_t ch_tdw_en[6];
    uint32_t weight8_en;
    uint32_t err_crumbs;
    uint32_t rand_crumbs;
    uint32_t ed_disable;
    uint32_t decay_wh_en;
    uint32_t decay_bk_en;
    uint32_t dweights[8];
    uint32_t weights[8];
    uint32_t outbpp;
    uint32_t levels;
    uint32_t threshold[30];
    uint32_t density[30];
    uint32_t size[30];
    uint32_t diffusion[30];
    uint32_t ch_psa_en[6];
    uint32_t psathresh[9];
    uint32_t psasize[9];
    uint32_t psa_bsel0;
    uint32_t psa_bsel1;
    uint32_t psa_ch34_sel;
    uint32_t psa_max_level;
    uint32_t psa_blk_prior;
    uint32_t psa_filter_en;
    uint32_t ch_tdb_en[6];
    uint32_t ch_tds_en[6];
    uint32_t sbmodify;
    uint32_t sbrgen;
    uint32_t sbselect;
    uint32_t sbias1;
    uint32_t sbias2;
    uint32_t sbias_height[6];
    uint32_t sbias_width[6];
    uint32_t cliff1;
    uint32_t range;
    uint32_t lastlevel;
    ued_lut_data_t *lut_data;
} ued_cfg_t;


extern ued_cfg_t ued_photo_cfg;
extern ued_cfg_t ued_plain_normal_cfg;
extern ued_cfg_t ued_plain_draft_cfg;
extern ued_cfg_t ued_mono8_1bpp_cfg;
extern ued_cfg_t ued_mono8_2bpp_cfg;
extern ued_cfg_t ued_mono8_2bpp_demo_cfg;
extern ued_cfg_t ued_mono8_2bpp_paired_demo_cfg;
extern ued_cfg_t ued_mono8_4bpp_demo_cfg;

#endif
