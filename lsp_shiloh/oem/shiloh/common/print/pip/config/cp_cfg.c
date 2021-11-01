/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the CP block config.
  * 
 **/

#include "tp_cfg.h"

tp_cfg_t cp_cfg = 
{
    .bpp = 0,
    .outsel = 0,
    .pixorder = 1,
    .leftedgefill = 0,
    .leftval = 0xffffffff,
    .leftlen = 0,         // set at run-time
    .rightedgefill = 0,
    .rightval = 0xffffffff,
    .rightlen = 0         // set at run-time
};

