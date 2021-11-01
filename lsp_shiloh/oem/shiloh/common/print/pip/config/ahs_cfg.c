/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the AHS block config.
  * 
 **/


#include "ahs_cfg.h"

ahs_cfg_t ahs_bypass_cfg = 
{
    .up0_chans = 4,
    .up0_height = 8,
    .up0_width = 8,
    .up1_chans = 4,
    .up1_height = 8,
    .up1_width = 8,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .write_to_hex_input = 0x1,
    .serpentine_data = 0x0,
    .serp_start_dir = 0x0,
    .echo_config = 0x0,
    .en_bypass = 0x1,
    .en_ah0 = 0x0,
    .en_ah1 = 0x0,
    .en_stats0 = 0x0,
    .en_stats1 = 0x0,
    .ah0_mode_select = 0,
    .ah1_mode_select = 0,
    .ch0_thresh = 0,
    .ch1_thresh = 0,
    .ch4_thresh = 0,
    .stats0_swath_height = 1,
    .ch2_thresh = 0,
    .ch3_thresh = 0,
    .ch5_thresh = 0,
    .stats1_swath_height = 1,
    .ahs0_chan = 4,
    .ahs1_chan = 5
};

ahs_cfg_t ahs_basic_cfg = 
{
    .up0_chans = 4,
    .up0_height = 8,
    .up0_width = 8,
    .up1_chans = 4,
    .up1_height = 8,
    .up1_width = 8,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .write_to_hex_input = 0x1,
    .serpentine_data = 0x0,
    .serp_start_dir = 0x0,
    .echo_config = 0x0,
    .en_bypass = 0x0, 
    .en_ah0 = 0x1,
    .en_ah1 = 0x1,
    .en_stats0 = 0x0,
    .en_stats1 = 0x0,  
    .ah0_mode_select = 0,
    .ah1_mode_select = 0,
    .ch0_thresh = 0,
    .ch1_thresh = 0,
    .ch4_thresh = 0,
    .stats0_swath_height = 1,
    .ch2_thresh = 0,
    .ch3_thresh = 0,
    .ch5_thresh = 0,
    .stats1_swath_height = 1,
    .ahs0_chan = 4,
    .ahs1_chan = 5
};

ahs_cfg_t ahs_testpip2_cfg = 
{
    .up0_chans = 4,
    .up0_height = 40,
    .up0_width = 512,
    .up1_chans = 4,
    .up1_height = 40,
    .up1_width = 512,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .write_to_hex_input = 0x1,
    .serpentine_data = 0x1,
    .serp_start_dir = 0x1,
    .echo_config = 0x0,
    .en_bypass = 0x0,   
    .en_ah0 = 0x0,
    .en_ah1 = 0x1,
    .en_stats0 = 0x0,
    .en_stats1 = 0x0,
    .ah0_mode_select = 0,
    .ah1_mode_select = 0,
    .ch0_thresh = 0,
    .ch1_thresh = 0,
    .ch4_thresh = 0,
    .stats0_swath_height = 5,
    .ch2_thresh = 0,
    .ch3_thresh = 0,
    .ch5_thresh = 0,
    .stats1_swath_height = 5,
    .ahs0_chan = 4,
    .ahs1_chan = 5
};

ahs_cfg_t ahs_testpip2_a_cfg = 
{
    .up0_chans = 4,
    .up0_height = 40,
    .up0_width = 512,
    .up1_chans = 4,
    .up1_height = 40,
    .up1_width = 512,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .write_to_hex_input = 0x1,
    .serpentine_data = 0x1,
    .serp_start_dir = 0x1,
    .echo_config = 0x0,
    .en_bypass = 0x0,   
    .en_ah0 = 0x0,
    .en_ah1 = 0x1,
    .en_stats0 = 0x0,
    .en_stats1 = 0x0,
    .ah0_mode_select = 0,
    .ah1_mode_select = 0,
    .ch0_thresh = 0,
    .ch1_thresh = 0,
    .ch4_thresh = 0,
    .stats0_swath_height = 5,
    .ch2_thresh = 0,
    .ch3_thresh = 0,
    .ch5_thresh = 0,
    .stats1_swath_height = 5,
    .ahs0_chan = 4,
    .ahs1_chan = 5
};
