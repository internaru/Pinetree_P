/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the CR block.
  * 
 **/

#ifndef __CR_FUNC_H_INCLUDED
#define __CR_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "logger.h"
#include "cr_cfg.h"

void cr_setup(CR_REGS_t *CR_reg, cr_cfg_t *cr_cfg);
void cr_start(CR_REGS_t *CR_reg, cr_cfg_t *cr_cfg);
void cr_print(CR_REGS_t *CR_reg);
void cr_clear(CR_REGS_t *CR_reg);

#endif


