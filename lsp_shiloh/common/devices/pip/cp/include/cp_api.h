/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the TP block.
  * 
 **/

#ifndef __CP_FUNC_H_INCLUDED
#define __CP_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "regAddrs.h"
#include "logger.h"
#include "cp_cfg.h"

void cp_setup(TP_CONTONE_REGS_t *cp_reg, cp_cfg_t *cp_cfg);
void cp_print(TP_CONTONE_REGS_t *cp_reg);

#endif


