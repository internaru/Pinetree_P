/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the FHS block config.
  * 
 **/

#ifndef __FHS_CFG_HEADER
  #define __FHS_CFG_HEADER

#include <stdint.h>

typedef struct fhs_cfg_struct 
{
    uint32_t bypass;
    uint32_t stallonerror;
    uint32_t imageinwidth;
    uint32_t imageoutwidth;
    uint32_t datatypechanid;
} fhs_cfg_t;

extern fhs_cfg_t fhs_cfg;
extern fhs_cfg_t fhs_testpip2_cfg;
extern fhs_cfg_t fhs_testpip2_a_cfg;
extern fhs_cfg_t fhs_128pad_cfg;
extern fhs_cfg_t fhs_generic_cfg;

#endif
