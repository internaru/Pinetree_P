/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains TEMS block configuration data.
  * 
 **/

#include "tems_cfg.h"

tems_cfg_t tems_bypass_cfg = 
{
    .bypass = 1
};

tems_cfg_t tems_generic_cfg = 
{
    .serpentine_data = 1,
    .serp_start_dir = 0,
    .bypass = 0,
    .replace_val = 768,
    .text_res = 0,
    .override_func = 0,
    .text_enable = 1,
    .text_bit0_sel = 0,
    .text_bit1_sel = 1,
    .text_bit2_sel = 2,
    .text_bit3_sel = 3,
    .clear_1_3 = 31,
    .clear_4 = 0,
    .vscale_m1 = 0,    // set at runtime
    .hscale_m1 = 0     // set at runtime
};

tems_cfg_t tems_text_disable_cfg = 
{
    .serpentine_data = 1,
    .serp_start_dir = 0,
    .bypass = 0,
    .replace_val = 768,
    .text_res = 0,
    .override_func = 0,
    .text_enable = 0,
    .text_bit0_sel = 0,
    .text_bit1_sel = 1,
    .text_bit2_sel = 2,
    .text_bit3_sel = 3,
    .clear_1_3 = 31,
    .clear_4 = 0,
    .vscale_m1 = 0,    // set at runtime
    .hscale_m1 = 0     // set at runtime
};
