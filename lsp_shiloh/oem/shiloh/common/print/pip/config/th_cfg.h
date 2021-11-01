/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the Threshold Halftone block config.
  * 
 **/

#ifndef TH_CFG_H
#define TH_CFG_H

#include <stdint.h>

typedef struct th_ucell_cfg_struct
{
    uint8_t   bpp;
    uint8_t   concat;

    uint8_t   height;
    uint8_t   width;
    uint8_t   offset;
    uint8_t   stoc_height;
    uint8_t   stoc_width;
    uint8_t   start_height;
    uint8_t   start_width;
    uint32_t  thresh_hi;
    uint32_t  thresh_lo;

    uint32_t  lut_minval;
    uint32_t  lut_maxval;
    uint16_t  lut_data[256];    // Ignored if lut_maxval > 0;

    uint32_t  ucell_data[256];
} th_ucell_cfg_t;

typedef struct th_cfg_struct
{
    uint8_t  bypass;
    uint8_t  alt_lut_dbl;
    uint8_t  use_obj_type;
    uint8_t  en_partial_ht;
    uint8_t  direct_thresh;
    uint8_t  mix_off;
    uint8_t  alt_lut_lfsr;

    uint32_t alt_lut_bpp_conv;
    uint32_t cfg_concat;
    uint32_t output_just_concat;
    uint32_t pw_mux_bpp_conv;
    uint32_t edge_override;
    uint8_t  thresh_edge;
    uint8_t  thresh_pw;
    uint16_t LFSR;

    th_ucell_cfg_t  *ucell0;
    th_ucell_cfg_t  *ucell1;

    uint8_t  *stoc_data;
    uint32_t  stoc_size;

    uint8_t  *alt_lut_data;
} th_cfg_t;

extern th_cfg_t th_bypass_cfg;
extern th_cfg_t th_106lpi_1bit_cfg;
extern th_cfg_t th_106lpi_2bit_cfg;
extern th_cfg_t th_141lpi_1bit_cfg;
extern th_cfg_t th_141lpi_2bit_cfg;
extern th_cfg_t th_cmyk_106lpi_1bit_C_cfg;
extern th_cfg_t th_cmyk_106lpi_1bit_M_cfg;
extern th_cfg_t th_cmyk_106lpi_1bit_Y_cfg;
extern th_cfg_t th_cmyk_106lpi_1bit_K_cfg;
extern th_cfg_t th_cmyk_106lpi_2bit_K_cfg;

#endif
