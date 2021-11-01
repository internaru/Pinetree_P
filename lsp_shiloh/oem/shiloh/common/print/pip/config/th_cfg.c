/****************************************************************************** 
 * Copyright (c) 2011 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  * This file contains Threshold Halftone block configuration data.
  * 
 **/

#include <stdint.h>

#include "th_cfg.h"
#include "th_default_data.h"
#include "th_stoc_c_1bit.h"

th_cfg_t th_bypass_cfg =
{
    .bypass = 1
};

th_ucell_cfg_t th_ucell_zero =
{
  .bpp         = 1,
  .concat      = 0,  // ?
  .height      = 3,
  .width       = 7,
  .offset      = 4,
  .stoc_height = 64,
  .stoc_width  = 64,
  .start_height = 0,
  .start_width = 0,
  .thresh_hi   = 65535,
  .thresh_lo   = 0,

  .lut_minval  = 144,
  .lut_maxval  = 719,
  .lut_data    = {0},

  .ucell_data  = { 0 },
};

th_ucell_cfg_t th_ucell_zero_2bit =
{
  .bpp         = 2,
  .concat      = 0,  // ?
  .height      = 3,
  .width       = 7,
  .offset      = 4,
  .stoc_height = 64,
  .stoc_width  = 64,
  .start_height = 0,
  .start_width = 0,
  .thresh_hi   = 65535,
  .thresh_lo   = 0,

  .lut_minval  = 144,
  .lut_maxval  = 719,
  .lut_data    = {0},

  .ucell_data  = { 0 },
};

extern uint8_t alt_lut[256];
#include "th_106lpi_1bit_cfg.c"
#include "th_106lpi_2bit_cfg.c"
#include "th_141lpi_1bit_cfg.c"
#include "th_141lpi_2bit_cfg.c"
#include "th_cmyk_106lpi_1bit_cfg.c"
// #include "th_cmyk_106lpi_2bit_cfg.c"




