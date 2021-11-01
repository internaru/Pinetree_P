/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the TEMS block config.
  * 
 **/

#ifndef TEMS_CFG_H
#define TEMS_CFG_H

#include <stdint.h>


typedef struct tems_cfg_s
{
    uint32_t serpentine_data;
    uint32_t serp_start_dir;
    uint32_t bypass;
    uint32_t replace_val;
    uint32_t text_res;
    uint32_t override_func;
    uint32_t text_enable;
    uint32_t text_bit0_sel;
    uint32_t text_bit1_sel;
    uint32_t text_bit2_sel;
    uint32_t text_bit3_sel;
    uint32_t clear_1_3;
    uint32_t clear_4;
    uint32_t vscale_m1;
    uint32_t hscale_m1;
} tems_cfg_t;

extern tems_cfg_t tems_bypass_cfg;
extern tems_cfg_t tems_generic_cfg;
extern tems_cfg_t tems_text_disable_cfg;

#endif
