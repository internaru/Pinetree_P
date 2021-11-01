/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the FHS block.
  * 
 **/

#ifndef __FHS_FUNC_C_INCLUDED
#define __FHS_FUNC_C_INCLUDED

#include <stdint.h>
#include "fhs_api.h"

void fhs_setup(FHS_REGS_t *fhs_reg, fhs_cfg_t *fhs_cfg) 
{
    uint32_t reg_val;

    // input image pixel width register
    reg_val = fhs_reg->inImageWidth;
    reg_val = FHS_INIMAGEWIDTH_INPUTCOUNT_REPLACE_VAL(reg_val,fhs_cfg->imageinwidth);
    fhs_reg->inImageWidth = reg_val; 

    // output image pixel width register
    reg_val = fhs_reg->outImageWidth;
    reg_val = FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_REPLACE_VAL(reg_val,fhs_cfg->imageoutwidth);
    fhs_reg->outImageWidth = reg_val; 

    // control register
    reg_val = fhs_reg->control;
    reg_val = FHS_CONTROL_BYPASS_REPLACE_VAL(reg_val,fhs_cfg->bypass);
    reg_val = FHS_CONTROL_STALLONERROR_REPLACE_VAL(reg_val,fhs_cfg->stallonerror);
    fhs_reg->control = reg_val;
}

#endif


