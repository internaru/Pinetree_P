/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the TP block config.
  * 
 **/

#include "tp_cfg.h"

tp_cfg_t tp_cfg = 
{
    .bpp = 0,
    .outsel = 0,
    .pixorder = 0,
    .leftedgefill = 0,
    .leftval = 0,
    .leftlen = 0,         // set at run-time
    .rightedgefill = 0,
    .rightval = 0,
    .rightlen = 0         // set at run-time
};

tp_cfg_t tp_jbig_txtmerge_cfg = 
{  
    .bpp = 0,
    .outsel = 1,
    .pixorder = 1,
    .leftedgefill = 0,
    .leftval = 0,
    .leftlen = 0,         // set at run-time
    .rightedgefill = 0,
    .rightval = 0,
    .rightlen = 0         // set at run-time
};
