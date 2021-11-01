/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the FHS block.
  * 
 **/

#ifndef __FHS_FUNC_H_INCLUDED
#define __FHS_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "regAddrs.h"
#include "logger.h"
#include "fhs_cfg.h"

void fhs_setup(FHS_REGS_t *fhs_reg, fhs_cfg_t *fhs_cfg);

//extern FHS_REGS_t *fhs_reg;

#endif


