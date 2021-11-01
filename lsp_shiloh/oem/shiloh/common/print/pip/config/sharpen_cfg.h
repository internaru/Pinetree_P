/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the Sharpen block config.
  * 
 **/

#ifndef SHARPEN_CFG_H
#define SHARPEN_CFG_H

#include <stdint.h>


typedef struct sharpen_cfg_s
{
    uint32_t bypass;
    uint32_t fill_nReplicate;
    uint32_t ot_nib_sel;
    uint32_t image_width;
    uint32_t fill_val_Y;
    uint32_t fill_val_C;
    int      Y_coeff[3][3];
    int      C_coeff[3][3];
    uint32_t Y_shiftfactor;
    uint32_t C_shiftfactor;
    uint32_t Y_shift;
    uint32_t C_shift;
} sharpen_cfg_t;

extern sharpen_cfg_t sharpen_bypass_cfg;
extern sharpen_cfg_t sharpen_generic_cfg;
extern sharpen_cfg_t sharpen_test_cfg;
extern sharpen_cfg_t sharpen_aggressive_cfg;

#endif
