/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the STAT block config.
  * 
 **/

#include "stat_cfg.h"

stat_cfg_t stat_bypass_cfg = 
{
    .image_width = 0,
    .accumulate = 0,
    .stats_enable = 0,
  
    // Config Register A
    .ch0_thresh = 0,
    .ch1_thresh = 0,
    .ch4_thresh = 0,
    .a_swath_height = 0,
  
    // Config Register B
    .ch2_thresh = 0,
    .ch3_thresh = 0,
    .b_swath_height = 0,
  
    // Config Register C
    .ch5_thresh = 0,
    .c_swath_height = 0
};

stat_cfg_t stat_test_cfg = 
{
    .image_width = 0,
    .accumulate = 1,
    .stats_enable = 1,
  
    // Config Register A
    .ch0_thresh = 0,
    .ch1_thresh = 0,
    .ch4_thresh = 0,
    .a_swath_height = 0,
  
    // Config Register B
    .ch2_thresh = 0,
    .ch3_thresh = 0,
    .b_swath_height = 0,
  
    // Config Register C
    .ch5_thresh = 0,
    .c_swath_height = 0
};
