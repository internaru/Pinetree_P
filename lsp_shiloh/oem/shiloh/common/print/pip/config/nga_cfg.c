/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the NGA block config.
  * 
 **/

#include "nga_cfg.h"

nga_cfg_t nga_cfg = 
{
    .image_height = 8,
    .image_width = 8,
    .bypass = 0x0,
    .pull_nshift = 0x0,
    .perpixenable = 0x1,
    .perpixenpolarity = 0x0,
    .perpixenmask = 0x3,
    .perpixenvalue = 0x3,
    .loadlut = 0x1,
    .c1lutptr = (uint8_t *)0x1000,
    .c2lutptr = (uint8_t *)0x2000
};

nga_cfg_t nga_testpip2_cfg = 
{
    .image_height = 8,
    .image_width = 8,
    .bypass = 0x0,
    .pull_nshift = 0x0,  
    .perpixenable = 0x0,
    .perpixenpolarity = 0x0,
    .perpixenmask = 0x0,
    .perpixenvalue = 0x0,
    .loadlut = 0x1,
    .c1lutptr = (uint8_t *)0x1000,
    .c2lutptr = (uint8_t *)0x2000
};

nga_cfg_t nga_testpip2_a_cfg = 
{
    .image_height = 8,
    .image_width = 8,
    .bypass = 0x0,
    .pull_nshift = 0x0,  
    .perpixenable = 0x0,
    .perpixenpolarity = 0x0,
    .perpixenmask = 0x0,
    .perpixenvalue = 0x0,
    .loadlut = 0x1,
    .c1lutptr = (uint8_t *)0x1000,
    .c2lutptr = (uint8_t *)0x2000
};
