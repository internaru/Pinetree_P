/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the STAT block.
  * 
 **/

#ifndef STAT_FUNC_H
#define STAT_FUNC_H

#include "error_types.h"
#include "pip_utils.h"
#include "pip_api.h"
#include "stat_cfg.h"


//********************************
// STAT_cfg_driver - setup the majority of the STAT block
//********************************
void stat_cfg_driver(PIP_STATS_REGS_t *stat_reg, stat_cfg_t *stat_cfg);

error_type_t stat_cfg_buffer(uint32_t perfid);

error_type_t stat_dma_start(uint32_t *buf, uint32_t bufsize, uint32_t *srcaddr, uint32_t perfid);

void stat_cdma_close(void);

void stat_read_stats(uint32_t *bufa, 
                     uint32_t *bufb, 
                     uint32_t *bufc, 
                     pip_stats_entry_t *combined_stats,
                     pip_stats_entry_t chan_stats[]);

#endif // STAT_FUNC_H
