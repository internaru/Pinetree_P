/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the AHS block.
  * 
 **/

#include "pip_utils.h"
#include "ahs_cfg.h"

#ifndef AHS_FUNC_H
#define AHS_FUNC_H

#define AHS_LUT_DEPTH 512

//********************************
// AHS_cfg_driver - setup the majority of the AHS block
//********************************
//void AHS_cfg_driver(AHS_cfg_t *ahs_cfg, AHS_REGS_t *ahs_reg);

//********************************
// AHS_lut_load
// Load the specified LUT with data at data_ptr
//********************************
void ahs_lut_load(volatile uint32_t *ahs_lut_ptr, 
                  uint32_t* data_ptr);

//********************************
// AHS_lut_check
// Compare the specified LUT with data at data_ptr
//********************************
void ahs_lut_check(volatile uint32_t *ahs_lut_ptr, 
                   uint32_t* data_ptr);

#endif // AHS_FUNC_H
