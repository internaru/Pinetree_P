/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the TEMS block.
  * 
 **/

#ifndef TEMS_H
#define TEMS_H

#include <stdint.h>
#include "TEMS_regstructs.h"
#include "TEMS_regmasks.h"
#include "tems_cfg.h"


void tems_cfg_driver(TEMS_REGS_t *reg, tems_cfg_t *cfg);

#endif
