/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the YIPI block config.
  * 
 **/

#ifndef __YIPI_CFG_HEADER
  #define __YIPI_CFG_HEADER

#include <stdint.h>

typedef struct yipi_cfg_struct 
{
    uint32_t up0_chans;
    uint32_t up0_height;
    uint32_t up0_width;
    uint32_t write_to_dv;
    uint32_t write_to_hex;
    uint32_t echo_config;
    uint32_t bypass;
    uint32_t ch_sel;
    uint32_t serpentine_data;
    uint32_t serp_start_dir;
    uint32_t duplicate;
    uint32_t h_space;
    uint32_t v_space;
    uint32_t func_sel[4];
    uint32_t repl_pixel[4];
    uint32_t ref_array[8];
} yipi_cfg_t;

extern yipi_cfg_t yipi_bypass_cfg;
extern yipi_cfg_t yipi_basic_cfg;
extern yipi_cfg_t yipi_testpip2_cfg;
extern yipi_cfg_t yipi_testpip2_a_cfg;
extern yipi_cfg_t yipi_test_cfg;

#endif
