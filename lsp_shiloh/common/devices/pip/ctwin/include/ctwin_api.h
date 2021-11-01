/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the CTWIN block.
  * 
 **/

#ifndef CTWIN_H
#define CTWIN_H

#include <stdint.h>
#include "CTWIN_regstructs.h"
#include "CTWIN_regmasks.h"
#include "ctwin_cfg.h"


void ctwin_cfg_driver(CTWIN_REGS_t *reg, ctwin_cfg_t *cfg);

#endif
