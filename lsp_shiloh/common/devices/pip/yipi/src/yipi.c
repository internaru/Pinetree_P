/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the YIPI block.
  * 
 **/

#include "yipi_api.h"
#include "logger.h"

#define DBG_PRFX "YIPI: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

//********************************
// yipi_cfg_driver - setup the YIPI block
//********************************
void yipi_cfg_driver(YIPI_REGS_t *yipi_reg, yipi_cfg_t *yipi_cfg)
{
    uint32_t reg_val;
    uint32_t iw = yipi_cfg->up0_width-1;

    DBG_PRINTF_NOTICE("Called yipi_cfg_driver() \n\r");

    // Yellow Insertion Pixel Identification control and configuration register (YCCR)
    reg_val        = yipi_reg->YCCR;
    reg_val        = YIPI_YCCR_B_REPLACE_VAL(reg_val, yipi_cfg->bypass);
    reg_val        = YIPI_YCCR_CH_REPLACE_VAL(reg_val, yipi_cfg->ch_sel);
    reg_val        = YIPI_YCCR_SS_REPLACE_VAL(reg_val, yipi_cfg->serp_start_dir);
    reg_val        = YIPI_YCCR_SD_REPLACE_VAL(reg_val, yipi_cfg->serpentine_data);
    reg_val        = YIPI_YCCR_D_REPLACE_VAL(reg_val, yipi_cfg->duplicate);
    reg_val        = YIPI_YCCR_V_REPLACE_VAL(reg_val, yipi_cfg->v_space);
    reg_val        = YIPI_YCCR_H_REPLACE_VAL(reg_val, yipi_cfg->h_space);
    yipi_reg->YCCR = reg_val;

    // YIPI inserted code and function select register (YCFR)
    reg_val        = yipi_reg->YCFR;
    reg_val        = YIPI_YCFR_C1_REPLACE_VAL(reg_val, yipi_cfg->repl_pixel[0]);
    reg_val        = YIPI_YCFR_C2_REPLACE_VAL(reg_val, yipi_cfg->repl_pixel[1]);
    reg_val        = YIPI_YCFR_C3_REPLACE_VAL(reg_val, yipi_cfg->repl_pixel[2]);
    reg_val        = YIPI_YCFR_C4_REPLACE_VAL(reg_val, yipi_cfg->repl_pixel[3]);
    reg_val        = YIPI_YCFR_F1_REPLACE_VAL(reg_val, yipi_cfg->func_sel[0]);
    reg_val        = YIPI_YCFR_F2_REPLACE_VAL(reg_val, yipi_cfg->func_sel[1]);
    reg_val        = YIPI_YCFR_F3_REPLACE_VAL(reg_val, yipi_cfg->func_sel[2]);
    reg_val        = YIPI_YCFR_F4_REPLACE_VAL(reg_val, yipi_cfg->func_sel[3]);
    yipi_reg->YCFR = reg_val;

    //YIPI inserted array information lines 1 and 2 (YAL12)
    reg_val         = yipi_reg->YAL12;
    reg_val         = YIPI_YAL12_A1_REPLACE_VAL(reg_val, yipi_cfg->ref_array[0]);
    reg_val         = YIPI_YAL12_A2_REPLACE_VAL(reg_val, yipi_cfg->ref_array[1]);
    yipi_reg->YAL12 = reg_val;

    //YIPI inserted array information lines 3 and 4 (YAL34)
    reg_val         = yipi_reg->YAL34;
    reg_val         = YIPI_YAL34_A3_REPLACE_VAL(reg_val, yipi_cfg->ref_array[2]);
    reg_val         = YIPI_YAL34_A4_REPLACE_VAL(reg_val, yipi_cfg->ref_array[3]);
    yipi_reg->YAL34 = reg_val;

    //YIPI inserted array information lines 5 and 6 (YAL56)
    reg_val         = yipi_reg->YAL56;
    reg_val         = YIPI_YAL56_A5_REPLACE_VAL(reg_val, yipi_cfg->ref_array[4]);
    reg_val         = YIPI_YAL56_A6_REPLACE_VAL(reg_val, yipi_cfg->ref_array[5]);
    yipi_reg->YAL56 = reg_val;

    //YIPI inserted array information lines 7 and 8 (YAL78)
    reg_val         = yipi_reg->YAL78;
    reg_val         = YIPI_YAL78_A7_REPLACE_VAL(reg_val, yipi_cfg->ref_array[6]);
    reg_val         = YIPI_YAL78_A8_REPLACE_VAL(reg_val, yipi_cfg->ref_array[7]);
    yipi_reg->YAL78 = reg_val;

    //YIPI image width register (YIWR)
    reg_val        = yipi_reg->YIWR;
    reg_val        = YIPI_YIWR_W_REPLACE_VAL(reg_val, iw);
    yipi_reg->YIWR = reg_val;
}
