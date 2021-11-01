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

#ifndef __TP_FUNC_H_INCLUDED
#define __TP_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "regAddrs.h"
#include "logger.h"
#include "tp_cfg.h"

void tp_setup(TP_TEXT_REGS_t *tp_reg, tp_cfg_t *tp_cfg);
void tp_print(TP_TEXT_REGS_t *tp_reg);

#endif


