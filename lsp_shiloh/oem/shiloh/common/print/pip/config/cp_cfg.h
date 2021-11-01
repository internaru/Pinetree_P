/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the CP block config.
  * 
 **/

#ifndef __CP_CFG_HEADER
#define __CP_CFG_HEADER

#include <stdint.h>

typedef struct cp_cfg_struct 
{
    uint32_t bpp;
    uint32_t outsel;
    uint32_t pixorder;
    uint32_t leftedgefill;
    uint32_t leftval;
    uint32_t leftlen;
    uint32_t rightedgefill;
    uint32_t rightval;
    uint32_t rightlen;
} cp_cfg_t;

extern cp_cfg_t cp_cfg;

#endif
