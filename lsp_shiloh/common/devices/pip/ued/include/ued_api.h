/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the UED block.
  * 
 **/

#ifndef __UED_FUNC_H_INCLUDED
#define __UED_FUNC_H_INCLUDED

#include <stdint.h>
#include "regAddrs.h"
#include "logger.h"
#include "ued_cfg.h"

#ifdef ASIC_gemstone
#define UED_REGS_t  UED_BASIC_REGS_t 
#elif ASIC_granite
#define UED_REGS_t  UED_ADVANCED_REGS_t 
#else
#error No Supported ASIC
#endif

// -----------------------------
// UED function prototypes
// -----------------------------

#ifdef ASIC_granite
#include "UED_advanced_regstructs.h"
#include "UED_advanced_regmasks.h"
// basic is size compatible, has reserved regions where the advanced registers are.
//
#define UED_BASIC_REGS_t UED_ADVANCED_REGS_t
#define UED_REGS_t UED_ADVANCED_REGS_t
#else
#include "asic.h"
#endif

void ued_cfg_driver(UED_REGS_t *ued_reg, ued_cfg_t *ued_cfg);

void ued_setup_luts(UED_REGS_t *ued_reg, ued_lut_data_t *lut_data);

void ued_setup_luts_wlut4(UED_REGS_t *ued_reg, ued_lut_data_t *lut_data);

#endif
