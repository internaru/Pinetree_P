/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the DAVBUF block config.
  * 
 **/

#include "davbuf_cfg.h"

#define NULL ((void *) 0x0UL)


db_cfg_t db_ued_generic_cfg =
{
    .imagewidth = 0,
    .imageheight = 0,
    .bpp = 0,
    .mode = 0,
    .buffer = 0,
    .buffer_end = 0,
    .pixbursts = 16,
    .beatsperpixburst = 0,
    .sram_start = 0,
    .sram_end = 10239
};

db_cfg_t db_serp_generic_cfg =
{
    .imagewidth = 0,
    .imageheight = 0,
    .bpp = 0,
    .mode = 0,
    .buffer = 0,
    .buffer_end = 0,
    .pixbursts = 16,
    .beatsperpixburst = 0,
    .sram_start = 0,
    .sram_end = 5119
};

db_cfg_t db_kmod_generic_cfg =
{
    .imagewidth = 0,
    .imageheight = 0,
    .bpp = 0,
    .mode = 0,
    .buffer = 0,
    .buffer_end = 0,
    .pixbursts = 16,
    .beatsperpixburst = 0,
    .sram_start = 0,
    .sram_end = 10239,
};
