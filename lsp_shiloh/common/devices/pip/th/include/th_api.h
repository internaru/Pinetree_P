/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the Threshold Halftone block.
  * 
 **/

#ifndef __TH_FUNC_H_INCLUDED
#define __TH_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "regAddrs.h"
#include "logger.h"
#include "th_cfg.h"

// -----------------------------
// TH function prototypes
// -----------------------------

// DAJ: I'm not sure why TH_ has registers generated with CORE_ as the name.

void th_cfg_driver(CORE_REGS_t *th_reg,
                   SRAM_REGS_t *th_sram_reg,
                   th_cfg_t *th_cfg,
                   uint32_t start_row);

#endif

