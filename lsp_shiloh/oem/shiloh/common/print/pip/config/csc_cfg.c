/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the 3x3 CSC block config.
  * 
 **/

#include "csc_cfg.h"

csc_cfg_t csc_cfg = 
{
    .image_height = 8,
    .image_width = 8,
    .coeff = {0x4c9, 0x964, 0x1d3, 0x3d4e, 0x3ab3, 0x800, 0x800, 0x394e, 0x3eb3},
    .bypass = 0x0,
    .preoffset_bypass = 0x1,
    .postoffset_bypass = 0x0,
    .offset_bypass = 0x0,
    .inlut_bypass = 0x1,
    .outlut_bypass = 0x1,
    .loadinlut = 0x0,
    .loadoutlut = 0x0,
    .inlutptr = (uint32_t *)0x1000,
    .ilor = {0xfff, 0xfff, 0xfff},
    .outlutptr = (uint32_t *)0x2000,
    .olor = {0xff, 0xff, 0xff}
};

csc_cfg_t csc_testpip2_a_cfg = 
{
    .image_height = 10,
    .image_width = 128,
    .coeff = {0x4c8, 0x964, 0x1d2, 0x3d4d, 0x3ab3, 0x800, 0x800, 0x394d, 0x3eb3},
    .bypass = 0x0,
    .preoffset_bypass = 0x1,
    .postoffset_bypass = 0x0,
    .offset_bypass = 0x0,
    .inlut_bypass = 0x1,
    .outlut_bypass = 0x1,
    .loadinlut = 0x0,
    .loadoutlut = 0x0,
    .inlutptr = (uint32_t *)0x1000,
    .ilor = {0x0, 0x0, 0x0},
    .outlutptr = (uint32_t *)0x2000,
    .olor = {0x0, 0x0, 0x0}
};
