/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the NGA block config.
  * 
 **/

#ifndef __NGA_CFG_HEADER
  #define __NGA_CFG_HEADER

#include <stdint.h>

typedef struct nga_cfg_struct 
{
    uint32_t image_height;
    uint32_t image_width;
    uint8_t bypass;
    uint8_t pull_nshift;
    uint8_t perpixenable;
    uint8_t perpixenpolarity;
    uint8_t perpixenmask;
    uint8_t perpixenvalue;
    uint8_t loadlut;
    uint8_t *c1lutptr;
    uint8_t *c2lutptr;
} nga_cfg_t;

extern nga_cfg_t nga_cfg;
extern nga_cfg_t nga_testpip2_cfg;
extern nga_cfg_t nga_testpip2_a_cfg;

#endif
