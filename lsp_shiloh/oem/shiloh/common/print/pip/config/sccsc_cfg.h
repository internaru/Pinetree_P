/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the ScCSC block config.
  * 
 **/

#ifndef __SCCSC_CFG_HEADER
#define __SCCSC_CFG_HEADER

#include <stdint.h>

typedef struct sccsc_cfg_struct 
{
    uint32_t outchans;
    uint32_t outbpp;
    uint32_t chanmap[6];
    uint32_t bypassall;
    uint32_t ialg;
    uint32_t nge;
    uint32_t wme;
    uint32_t kme;
    uint32_t lutsel;
    uint32_t sband_en;
    uint32_t sband_mask;
    uint32_t sband_value;
    uint32_t lutsize;
    uint32_t wt[3];
    uint32_t wr[6];
    uint32_t kr[6];
    uint32_t imgheight;
    uint32_t imgwidth;
} sccsc_cfg_t;

// extern sccsc_cfg_t sccsc_cfg;
extern sccsc_cfg_t sccsc_bypass_cfg;
extern sccsc_cfg_t sccsc_plain_cfg;
extern sccsc_cfg_t sccsc_photo_cfg;
extern sccsc_cfg_t sccsc_mono8_cfg;

#endif
