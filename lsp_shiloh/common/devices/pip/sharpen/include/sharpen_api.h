/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the Sharpen block.
  * 
 **/

#ifndef SHARPEN_H
#define SHARPEN_H

#include <stdint.h>
#include "SHARPEN_regstructs.h"
#include "SHARPEN_regmasks.h"
#include "sharpen_cfg.h"


void sharpen_cfg_driver(SHARPEN_REGS_t *reg, sharpen_cfg_t *cfg);
void sharpen_savesram(SHARPEN_REGS_t *reg, uint32_t *data0, uint32_t *data1, uint32_t *data2, uint32_t *data3);
void sharpen_restore_sram(SHARPEN_REGS_t *reg, uint32_t *data0, uint32_t *data1, uint32_t *data2, uint32_t *data3);

#endif
