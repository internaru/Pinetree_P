/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the Pipeline Access block config.
  * 
 **/

#ifndef __PIPE_ACCESS_CFG_HEADER
  #define __PIPE_ACCESS_CFG_HEADER

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define PA_STORE_DISABLE  0
#define PA_STORE_4_CHANNEL 1
#define PA_STORE_6_CHANNEL 2
#define PA_LOAD_DISABLE  0
#define PA_LOAD_4_CHANNEL 1
#define PA_LOAD_6_CHANNEL 2

typedef struct pipe_access_cfg_struct 
{
    uint32_t up0_chans;
    uint32_t up0_height;
    uint32_t up0_width;
    uint32_t up1_chans;
    uint32_t up1_height;
    uint32_t up1_width;
    uint32_t lut_cbi;
    uint32_t pipeline_load_en;
    uint32_t pipeline_store_en;
    uint32_t bypass;
    uint32_t pack_8_chan_sel;
    uint32_t pack_32_chan_sel;
    uint32_t pack_sideband_msb;
    uint32_t pack_shift_2;
    uint32_t pack_mode;
    uint32_t unpack_sideband_fw_val;
    uint32_t unpack_8_chan_sel;
    uint32_t unpack_32_chan_sel;
    uint32_t unpack_use_sideband_fw;
    uint32_t unpack_sideband_msb;
    uint32_t unpack_shift_2;
    uint32_t unpack_mode;
    uint32_t lut_5_input_sel;
    uint32_t lut_4_input_sel;
    uint32_t lut_3_input_sel;
    uint32_t per_pixel_enable;
    uint32_t per_pixel_polarity;
    uint32_t per_pixel_sel;
    uint32_t stats_image_width;
    uint32_t stats_patch_height;
    uint32_t stats_en;
    uint32_t stats_thresh0;
    uint32_t stats_thresh1;
    uint32_t stats_thresh2;
    uint32_t stats_thresh3;
    uint32_t stats_thresh4;
    uint32_t stats_thresh5;
    uint32_t writehex;
} pipe_access_cfg_t;

extern pipe_access_cfg_t pipe_access_bypass_cfg;
extern pipe_access_cfg_t pipe_access_lut_only_cfg;
extern pipe_access_cfg_t pipe_access_mono8_cfg;
extern pipe_access_cfg_t pipe_access_testpip2_cfg;
extern pipe_access_cfg_t pipe_access_testpip2_a_cfg;
extern pipe_access_cfg_t pipe_access_store_6chan_cfg;
extern pipe_access_cfg_t pipe_access_store_4chan_cfg;
extern pipe_access_cfg_t pipe_access_load_6chan_cfg;
extern pipe_access_cfg_t pipe_access_load_4chan_cfg;
extern pipe_access_cfg_t pipe_access_store_filter_cfg;
extern pipe_access_cfg_t pipe_access_store_load_filter_cfg;

#endif
