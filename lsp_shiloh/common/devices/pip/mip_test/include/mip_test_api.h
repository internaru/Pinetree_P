/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the MIP block.
  * 
 **/
#include "asic.h"
#include "mip_cfg.h"

#ifndef MIP_FUNC_H
#define MIP_FUNC_H

//********************************
// mip_cfg_driver - setup the majority of the MIP block
//********************************
void mip_cfg_driver(MIP_REGSET_REGS_t *mip_reg, mip_cfg_t *mip_cfg);

void mip_check_tielut(uint32_t *TIELUT, uint32_t locations, MIP_REGSET_REGS_t *mip_reg);
void mip_load_tielut(uint32_t *TIELUT, uint32_t locations, MIP_REGSET_REGS_t *mip_reg);


#endif // MIP_FUNC_H
