/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the AHS block config.
  * 
 **/

#ifndef __AHS_CFG_HEADER
  #define __AHS_CFG_HEADER

#include <stdint.h>

typedef struct ahs_cfg_struct 
{
    uint32_t up0_chans;
    uint32_t up0_height;
    uint32_t up0_width;
    uint32_t up1_chans;
    uint32_t up1_height;
    uint32_t up1_width;
    uint32_t write_to_dv;
    uint32_t write_to_hex;
    uint32_t write_to_hex_input;
    uint32_t serpentine_data;
    uint32_t serp_start_dir;
    uint32_t echo_config;
    uint32_t en_bypass;
    uint32_t en_ah0;
    uint32_t en_ah1;
    uint32_t en_stats0;
    uint32_t en_stats1;
    uint32_t ah0_mode_select;
    uint32_t ah1_mode_select;
    uint32_t ch0_thresh;
    uint32_t ch1_thresh;
    uint32_t ch4_thresh;
    uint32_t stats0_swath_height;
    uint32_t ch2_thresh;
    uint32_t ch3_thresh;
    uint32_t ch5_thresh;
    uint32_t stats1_swath_height;
    uint32_t ahs0_chan;
    uint32_t ahs1_chan;
} ahs_cfg_t;

extern ahs_cfg_t ahs_cfg;
extern ahs_cfg_t ahs_bypass_cfg;
extern ahs_cfg_t ahs_basic_cfg;
extern ahs_cfg_t ahs_testpip2_cfg;
extern ahs_cfg_t ahs_testpip2_a_cfg;

#endif
