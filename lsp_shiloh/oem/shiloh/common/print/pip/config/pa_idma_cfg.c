/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the PA DMA block config.
  * 
 **/

#include "pa_idma_cfg.h"

pa_idma_cfg_t pa_idma_load_cfg = 
{
    .in_dwidth = 32,
    .upper_half = 0,
    .msb_in = 0,
    .in_width = 5,
    .bigend = 0,
    .image_width = 512,
    .image_height = 40,
    .serpentine = 0,
    .line_rev = 0,
    .tran_rev = 0
};
