/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the 3x3 CSC block config.
  * 
 **/

#ifndef __CSC_CFG_HEADER
  #define __CSC_CFG_HEADER

#include <stdint.h>

typedef struct csc_cfg_struct 
{
    uint32_t image_height;
    uint32_t image_width;
    uint32_t coeff[9];
    uint8_t bypass;
    uint8_t preoffset_bypass;
    uint8_t postoffset_bypass;
    uint8_t offset_bypass;
    uint8_t inlut_bypass;
    uint8_t outlut_bypass;
    uint8_t loadinlut;
    uint8_t loadoutlut;
    uint32_t *inlutptr;
    uint32_t ilor[3];
    uint32_t *outlutptr;
    uint32_t olor[3];
} csc_cfg_t;

extern csc_cfg_t csc_cfg;
extern csc_cfg_t csc_testpip2_cfg;
extern csc_cfg_t csc_testpip2_a_cfg;

#endif
