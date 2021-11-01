/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains Sharpen block configuration data.
  * 
 **/

#include "sharpen_cfg.h"

sharpen_cfg_t sharpen_bypass_cfg =
{
    .bypass = 1
};

sharpen_cfg_t sharpen_generic_cfg =
{
    .bypass = 0,
    .fill_nReplicate = 0,
    .ot_nib_sel = 0,
    .image_width = 0,     // set at runtime
    .fill_val_Y = 0,
    .fill_val_C = 0,
    .Y_coeff = {{ -5,  -9, -12},
                { -9, -20, -28},
                {-12, -28, 714}},
    .C_coeff = {{ -5,  -9, -12},
                { -9, -20, -28},
                {-12, -28, 714}},
    .Y_shiftfactor = 0xab9,
    .C_shiftfactor = 0xab9,
    .Y_shift = 0x9,
    .C_shift = 0x9
};

sharpen_cfg_t sharpen_test_cfg =
{
    .bypass = 0,
    .fill_nReplicate = 0,
    .ot_nib_sel = 0,
    .image_width = 0,     // set at runtime
    .fill_val_Y = 0,
    .fill_val_C = 0,
    .Y_coeff = {{ -30, -30, -30},
                { -30, -30, -33},
                { -30, -33, 988}},
    .C_coeff = {{ -30, -30, -30},
                { -30, -30, -33},
                { -30, -33, 988}},
    .Y_shiftfactor = 0x800,
    .C_shiftfactor = 0x800,
    .Y_shift = 0x8,
    .C_shift = 0x8
};

sharpen_cfg_t sharpen_aggressive_cfg =
{
    .bypass = 0,
    .fill_nReplicate = 0,
    .ot_nib_sel = 0,
    .image_width = 0,     // set at runtime
    .fill_val_Y = 0xFF,
    .fill_val_C = 0x7F,
    .Y_coeff = {{ -30, -30, -30},
                { -30, -30, -33},
                { -30, -33, 988}},
    .C_coeff = {{   1,   3,   6},
                {   3,  15,  25},
                {   6,  25,  44}},
    .Y_shiftfactor = 0x800,
    .C_shiftfactor = 0x800,
    .Y_shift = 0x8,
    .C_shift = 0x8
};
