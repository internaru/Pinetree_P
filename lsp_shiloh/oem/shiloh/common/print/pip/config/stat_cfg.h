/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the STAT block config.
  * 
 **/

#ifndef __STAT_CFG_HEADER
#define __STAT_CFG_HEADER

#include <stdint.h>

typedef struct stat_cfg_struct 
{
    uint32_t image_width;
    uint32_t accumulate;
    uint32_t stats_enable;

    // Config Register A
    uint32_t ch0_thresh;
    uint32_t ch1_thresh;
    uint32_t ch4_thresh;
    uint32_t a_swath_height;

    // Config Register B
    uint32_t ch2_thresh;
    uint32_t ch3_thresh;
    uint32_t b_swath_height;

    // Config Register C
    uint32_t ch5_thresh;
    uint32_t c_swath_height;
} stat_cfg_t;

extern stat_cfg_t stat_bypass_cfg;
extern stat_cfg_t stat_test_cfg;

#endif
