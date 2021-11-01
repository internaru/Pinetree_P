/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the NGA block.
  * 
 **/

#ifndef __NGA_FUNC_H_INCLUDED
#define __NGA_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "nga_cfg.h"
#include "NGAdjust_DAVINCI_regstructs.h"
#include "NGAdjust_DAVINCI_regmasks.h"

void nga_setup(NGADJUST_DAVINCI_REGS_t *nga_reg, nga_cfg_t *nga_cfg);
void nga_print(NGADJUST_DAVINCI_REGS_t *nga_reg);
void nga_loadlut(NGADJUST_DAVINCI_REGS_t *nga_reg, uint8_t *c1Ptr, uint8_t *c2Ptr);

#endif


