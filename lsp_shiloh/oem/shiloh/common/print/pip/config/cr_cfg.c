/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the CR block config.
  * 
 **/

#include "cr_cfg.h"
#include "debug_flags.h"

cr_cfg_t cr_cfg = 
{
    .up0_chans = 5,
    .up0_height = 5,
    .up0_width = 56,
    .up1_chans = 2,
    .up1_height = 5,
    .up1_width = 56,
    .dn_chans = 5,
    .dn_height = 5,
    .dn_width = 56,
    .zxen = 0x1,
    .jbigen = 0x0,
    .sbandmap = 0x1,
    .bypass = 0x0,
    .sbanden = 0x0,
    .sbandpol = 0x1,
    .sbandval = 0x19,
    .sbandmask = 0xb8,
    .rep = { 0x8fe0dd, 0x1d4619, 0xcc847d, 0x4bb35c },
    .lower = { 0x000000, 0xe57e5d, 0x000000, 0x16083f },
    .upper = { 0xffffff, 0xa05b14, 0xffffff, 0xe20df7 }
};

cr_cfg_t cr_testpip2_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 2,
    .up1_height = 10,
    .up1_width = 256, 
    .dn_chans = 5,
    .dn_height = 10,
    .dn_width = 128,
    .zxen = 0x1,
    .jbigen = 0x0,
    .sbandmap = 0x7,
    .bypass = 0x0,
    .sbanden = 0x0,
    .sbandpol = 0x1,
    .sbandval = 0x01,
    .sbandmask = 0x01,
    .rep = { 0xff8000, 0x000000, 0x000000, 0x000000 },
    .lower = { 0x305000, 0xffffff, 0xffffff, 0xffffff },
    .upper = { 0x507020, 0x000000, 0x000000, 0x000000 }
};

cr_cfg_t cr_testpip2_a_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 2,
    .up1_height = 10,
    .up1_width = 256, 
    .dn_chans = 5,
    .dn_height = 10,
    .dn_width = 128,
    .zxen = 0x1,
    .jbigen = 0x0,
    .sbandmap = 0x7,
    .bypass = 0x0,
    .sbanden = 0x0,
    .sbandpol = 0x1,
    .sbandval = 0x01,
    .sbandmask = 0x01,
#if debug_cr_replace
    .rep = { 0xff8000, 0x000000, 0x000000, 0x000000 },
    .lower = { 0x305000, 0xffffff, 0xffffff, 0xffffff },
    .upper = { 0x507020, 0x000000, 0x000000, 0x000000 }
#else
    .rep = { 0x000000, 0x000000, 0x000000, 0x000000 },
    .lower = { 0xffffff, 0xffffff, 0xffffff, 0xffffff },
    .upper = { 0x000000, 0x000000, 0x000000, 0x000000 }
#endif
};
