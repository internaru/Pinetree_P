/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the TEMS block.
  * 
 **/

#include "tems_api.h"

void tems_cfg_driver(TEMS_REGS_t *reg, tems_cfg_t *cfg)
{
    uint32_t reg_val;
 
    reg_val = reg->CR;
    reg_val = TEMS_CR_BYPASS_REPLACE_VAL(reg_val, cfg->bypass);
    reg_val = TEMS_CR_SERPENTINE_DATA_REPLACE_VAL(reg_val, cfg->serpentine_data);
    reg_val = TEMS_CR_SERP_START_DIR_REPLACE_VAL(reg_val, cfg->serp_start_dir);
    reg_val = TEMS_CR_SAVE_RESTORE_MODE_REPLACE_VAL(reg_val, 0);
    reg->CR = reg_val;
 
    reg_val = reg->TEXT;
    reg_val = TEMS_TEXT_REPLACE_VAL_REPLACE_VAL(reg_val, cfg->replace_val);
    reg_val = TEMS_TEXT_TEXT_RES_REPLACE_VAL(reg_val, cfg->text_res);
    reg_val = TEMS_TEXT_OVERRIDE_FUNC_REPLACE_VAL(reg_val, cfg->override_func);
    reg_val = TEMS_TEXT_TEXT_ENABLE_REPLACE_VAL(reg_val, cfg->text_enable);
    reg_val = TEMS_TEXT_TEXT_BIT0_SEL_REPLACE_VAL(reg_val, cfg->text_bit0_sel);
    reg_val = TEMS_TEXT_TEXT_BIT1_SEL_REPLACE_VAL(reg_val, cfg->text_bit1_sel);
    reg_val = TEMS_TEXT_TEXT_BIT2_SEL_REPLACE_VAL(reg_val, cfg->text_bit2_sel);
    reg_val = TEMS_TEXT_TEXT_BIT3_SEL_REPLACE_VAL(reg_val, cfg->text_bit3_sel);
    reg->TEXT = reg_val;
 
    reg_val = reg->CLEAR;
    reg_val = TEMS_CLEAR_CLEAR_1_3_REPLACE_VAL(reg_val, cfg->clear_1_3);
    reg_val = TEMS_CLEAR_CLEAR_4_REPLACE_VAL(reg_val, cfg->clear_4);
    reg->CLEAR = reg_val;
 
    reg_val = reg->SCALE;
    reg_val = TEMS_SCALE_VSCALE_M1_REPLACE_VAL(reg_val, cfg->vscale_m1);
    reg_val = TEMS_SCALE_HSCALE_M1_REPLACE_VAL(reg_val, cfg->hscale_m1);
    reg->SCALE = reg_val;
}
