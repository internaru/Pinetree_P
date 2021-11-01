/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the YIPI block.
  * 
 **/

#include "asic.h"
#include "yipi_cfg.h"

#ifndef YIPI_FUNC_H
#define YIPI_FUNC_H

//********************************
// YIPI_cfg_driver - setup the majority of the YIPI block
//********************************
void yipi_cfg_driver(YIPI_REGS_t *yipi_reg, yipi_cfg_t *yipi_cfg);

#endif // PIPE_ACCESS_FUNC_H
