/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the SERP block.
  * 
 **/

#include "serp_cfg.h"

#ifndef SERP_FUNC_H
#define SERP_FUNC_H

//********************************
// SERP_cfg_driver - setup the majority of the SERP block
//********************************
void serp_cfg_driver(SERP_REGS_t *serp_reg, serp_cfg_t *serp_cfg);

#endif // SERP_FUNC_H
