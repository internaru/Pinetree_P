/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the CR block config.
  * 
 **/

#ifndef __CR_CFG_HEADER
  #define __CR_CFG_HEADER

#include <stdint.h>

typedef struct cr_cfg_struct 
{
    uint32_t up0_chans;
    uint32_t up0_height;
    uint32_t up0_width;
    uint32_t up1_chans;
    uint32_t up1_height;
    uint32_t up1_width;
    uint32_t dn_chans;
    uint32_t dn_height;
    uint32_t dn_width;
    uint32_t zxen;
    uint32_t jbigen;
    uint32_t sbandmap;
    uint32_t bypass;
    uint32_t sbanden;
    uint32_t sbandpol;
    uint32_t sbandval;
    uint32_t sbandmask;
    uint32_t rep[4];
    uint32_t lower[4];
    uint32_t upper[4];
} cr_cfg_t;

extern cr_cfg_t cr_cfg;
extern cr_cfg_t cr_testpip2_cfg;
extern cr_cfg_t cr_testpip2_a_cfg;

#endif
