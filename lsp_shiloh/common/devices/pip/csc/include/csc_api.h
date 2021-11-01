/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __CSC_FUNC_H_INCLUDED
#define __CSC_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "logger.h"
#include "csc_cfg.h"

void csc_setup(CSC_DAVINCI_REGS_t *csc_reg, csc_cfg_t *csc_cfg);
void csc_print(CSC_DAVINCI_REGS_t *csc_reg);
void csc_loadcoeff(CSC_DAVINCI_REGS_t *csc_reg, uint32_t coeff[9]);
void csc_loadinlut(CSC_DAVINCI_REGS_t *csc_reg, uint32_t *ptr, uint32_t overflow [3]);
void csc_loadoutlut(CSC_DAVINCI_REGS_t *csc_reg, uint32_t *ptr, uint32_t overflow [3]);

#endif


