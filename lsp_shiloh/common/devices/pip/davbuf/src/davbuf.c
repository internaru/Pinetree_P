/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the DAVBUF block.
  * 
 **/

#include <stdint.h>
#include "davbuf_api.h"
#include "davbuf_cfg.h"
#include "asic.h"



void davbuf_cfg_driver(DB_REGS_t *db_regs, db_cfg_t *db_cfg)
{
    db_regs->DMA_START = db_cfg->buffer;
    db_regs->DMA_END = db_cfg->buffer_end;
    db_regs->SRAM_START = db_cfg->sram_start;
    db_regs->SRAM_END = db_cfg->sram_end;
}
