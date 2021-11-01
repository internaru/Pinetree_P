/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the Sharpen block.
  * 
 **/


#include "sharpen_api.h"

void sharpen_cfg_driver(SHARPEN_REGS_t *reg, sharpen_cfg_t *cfg)
{
    uint32_t reg_val;
 
    reg_val = reg->Config;
    reg_val = SHARPEN_CONFIG_BYPASS_REPLACE_VAL(reg_val, cfg->bypass);
    reg_val = SHARPEN_CONFIG_FILL_NREPLICATE_REPLACE_VAL(reg_val, cfg->fill_nReplicate);
    reg_val = SHARPEN_CONFIG_OT_NIBBLE_SEL_REPLACE_VAL(reg_val, cfg->ot_nib_sel);
    reg_val = SHARPEN_CONFIG_LINE_WIDTH_REPLACE_VAL(reg_val, cfg->image_width);
    reg->Config = reg_val;
 
    reg->fill_val_Y = cfg->fill_val_Y;
    reg->fill_val_C = cfg->fill_val_C;
 
    reg->Y_coeff0 = cfg->Y_coeff[0][0];
    reg->Y_coeff1 = cfg->Y_coeff[0][1];
    reg->Y_coeff2 = cfg->Y_coeff[0][2];
    reg->Y_coeff3 = cfg->Y_coeff[1][0];
    reg->Y_coeff4 = cfg->Y_coeff[1][1];
    reg->Y_coeff5 = cfg->Y_coeff[1][2];
    reg->Y_coeff6 = cfg->Y_coeff[2][0];
    reg->Y_coeff7 = cfg->Y_coeff[2][1];
    reg->Y_center_coeff = cfg->Y_coeff[2][2];
 
    reg->C_coeff0 = cfg->C_coeff[0][0];
    reg->C_coeff1 = cfg->C_coeff[0][1];
    reg->C_coeff2 = cfg->C_coeff[0][2];
    reg->C_coeff3 = cfg->C_coeff[1][0];
    reg->C_coeff4 = cfg->C_coeff[1][1];
    reg->C_coeff5 = cfg->C_coeff[1][2];
    reg->C_coeff6 = cfg->C_coeff[2][0];
    reg->C_coeff7 = cfg->C_coeff[2][1];
    reg->C_center_coeff = cfg->C_coeff[2][2];
 
    reg_val = reg->Y_Norm;
    reg_val = SHARPEN_Y_NORM_SHIFT_REPLACE_VAL(reg_val, cfg->Y_shift);
    reg_val = SHARPEN_Y_NORM_SHIFTFACTOR_REPLACE_VAL(reg_val, cfg->Y_shiftfactor);
    reg->Y_Norm = reg_val;
    
    reg_val = reg->C_Norm;
    reg_val = SHARPEN_C_NORM_SHIFT_REPLACE_VAL(reg_val, cfg->C_shift);
    reg_val = SHARPEN_C_NORM_SHIFTFACTOR_REPLACE_VAL(reg_val, cfg->C_shiftfactor);
    reg->C_Norm = reg_val;
}

void sharpen_save_sram(SHARPEN_REGS_t *reg, uint32_t *data0, uint32_t *data1, uint32_t *data2, uint32_t *data3)
{
    uint32_t reg_val;
    int      i;
 
    reg_val = reg->Config;
    reg_val = SHARPEN_CONFIG_SAVE_RESTORE_MODE_REPLACE_VAL(reg_val, 1);
    reg_val = SHARPEN_CONFIG_LB_SRAM_ACCESS_ENABLE_REPLACE_VAL(reg_val, 1);
    reg->Config = reg_val;
 
    reg_val = reg->LineBuffControl;
    reg_val = SHARPEN_LINEBUFFCONTROL_LB_RNW_REPLACE_VAL(reg_val, 1);
    reg_val = SHARPEN_LINEBUFFCONTROL_INC_ADDR_MODE_REPLACE_VAL(reg_val, 1);
    reg->LineBuffControl = reg_val;
 
    reg->LineBuffAddrWr = 0x0;
    for(i=0; i<5120; i++)
    {
        data0[i] = reg->LineBuffData0Rd;
    }
 
    reg->LineBuffAddrWr = 0x0;
    for(i=0; i<5120; i++)
    {
        data1[i] = reg->LineBuffData1Rd;
    }
 
    reg->LineBuffAddrWr = 0x0;
    for(i=0; i<5120; i++)
    {
        data2[i] = reg->LineBuffData2Rd;
    }
 
    reg->LineBuffAddrWr = 0x0;
    for(i=0; i<5120; i++)
    {
        data3[i] = reg->LineBuffData3Rd;
    }
 
    reg->LineBuffAddrWr = 0x0;
 
    reg_val = reg->LineBuffControl;
    reg_val = SHARPEN_LINEBUFFCONTROL_LB_RNW_REPLACE_VAL(reg_val, 0);
    reg_val = SHARPEN_LINEBUFFCONTROL_INC_ADDR_MODE_REPLACE_VAL(reg_val, 0);
    reg->LineBuffControl = reg_val;
 
    reg_val = reg->Config;
    reg_val = SHARPEN_CONFIG_SAVE_RESTORE_MODE_REPLACE_VAL(reg_val, 0);
    reg_val = SHARPEN_CONFIG_LB_SRAM_ACCESS_ENABLE_REPLACE_VAL(reg_val, 0);
    reg->Config = reg_val;
}

void sharpen_restore_sram(SHARPEN_REGS_t *reg, uint32_t *data0, uint32_t *data1, uint32_t *data2, uint32_t *data3)
{
    uint32_t reg_val;
    int      i;
 
    reg_val = reg->Config;
    reg_val = SHARPEN_CONFIG_SAVE_RESTORE_MODE_REPLACE_VAL(reg_val, 1);
    reg_val = SHARPEN_CONFIG_LB_SRAM_ACCESS_ENABLE_REPLACE_VAL(reg_val, 1);
    reg->Config = reg_val;
 
    reg_val = reg->LineBuffControl;
    reg_val = SHARPEN_LINEBUFFCONTROL_LB_RNW_REPLACE_VAL(reg_val, 0);
    reg_val = SHARPEN_LINEBUFFCONTROL_INC_ADDR_MODE_REPLACE_VAL(reg_val, 1);
    reg->LineBuffControl = reg_val;
 
    reg->LineBuffAddrWr = 0x0;
 
    for(i=0; i<5120; i++)
    {
        reg->LineBuffData0Wr = data0[i];
    }
 
    reg->LineBuffAddrWr = 0x0;
    for(i=0; i<5120; i++)
    {
        reg->LineBuffData1Wr = data1[i];
    }
 
    reg->LineBuffAddrWr = 0x0;
    for(i=0; i<5120; i++)
    {
        reg->LineBuffData2Wr = data2[i];
    }
 
    reg->LineBuffAddrWr = 0x0;
    for(i=0; i<5120; i++)
    {
        reg->LineBuffData3Wr = data3[i];
    }
 
    reg->LineBuffAddrWr = 0x0;
 
    reg_val = reg->LineBuffControl;
    reg_val = SHARPEN_LINEBUFFCONTROL_LB_RNW_REPLACE_VAL(reg_val, 0);
    reg_val = SHARPEN_LINEBUFFCONTROL_INC_ADDR_MODE_REPLACE_VAL(reg_val, 0);
    reg->LineBuffControl = reg_val;
 
    reg_val = reg->Config;
    reg_val = SHARPEN_CONFIG_SAVE_RESTORE_MODE_REPLACE_VAL(reg_val, 0);
    reg_val = SHARPEN_CONFIG_LB_SRAM_ACCESS_ENABLE_REPLACE_VAL(reg_val, 0);
    reg->Config = reg_val;
}


