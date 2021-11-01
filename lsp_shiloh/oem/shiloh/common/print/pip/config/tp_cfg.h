/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the TP block config.
  * 
 **/

#ifndef __TP_CFG_HEADER
#define __TP_CFG_HEADER

#include <stdint.h>

typedef struct tp_cfg_struct 
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
} tp_cfg_t;

extern tp_cfg_t tp_cfg;
extern tp_cfg_t tp_jbig_txtmerge_cfg;

#endif
