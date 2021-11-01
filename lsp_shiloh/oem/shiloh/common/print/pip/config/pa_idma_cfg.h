/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the PA IDMA block config.
  * 
 **/

#ifndef __PA_IDMA_CFG_HEADER
#define __PA_IDMA_CFG_HEADER

#include <stdint.h>

typedef struct pa_idma_cfg_s
{
    uint32_t in_dwidth;
    uint32_t upper_half;
    uint32_t msb_in;
    uint32_t in_width;
    uint32_t bigend;
    uint32_t image_width;
    uint32_t image_height;
    uint32_t serpentine;
    uint32_t line_rev;
    uint32_t tran_rev;
} pa_idma_cfg_t;

extern pa_idma_cfg_t pa_idma_load_cfg;

#endif
