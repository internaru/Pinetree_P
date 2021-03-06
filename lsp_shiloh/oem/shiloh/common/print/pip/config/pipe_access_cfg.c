/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the Pipeline Access block config.
  * 
 **/

#include "pipe_access_cfg.h"

pipe_access_cfg_t pipe_access_bypass_cfg = 
{
    .up0_chans = 5,
    .up0_height = 8,
    .up0_width = 8,
    .up1_chans = 8,
    .up1_height = 8,
    .up1_width = 8,
    .lut_cbi = 0,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_DISABLE,
    .bypass = 1,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 0,
    .lut_4_input_sel = 0,
    .lut_3_input_sel = 0,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_lut_only_cfg = 
{
    .up0_chans = 5,
    .up0_height = 8,
    .up0_width = 8,
    .up1_chans = 8,
    .up1_height = 8,
    .up1_width = 8,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_DISABLE,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_mono8_cfg = 
{
    .up0_chans = 5,
    .up0_height = 8,
    .up0_width = 8,
    .up1_chans = 8,
    .up1_height = 8,
    .up1_width = 8,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_DISABLE,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 2,
//    .lut_4_input_sel = 4,
    .lut_4_input_sel = 0,   // map input ch0 to output ch4 (for use with 3d identity lut)
    .lut_3_input_sel = 1,  // channel 2 also goes to channel 3
    .per_pixel_enable = 0, // should be 0 testing object tag....
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_testpip2_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_DISABLE,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_testpip2_a_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_DISABLE,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_store_6chan_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_6_CHANNEL,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 1,
    .stats_en = 1,
    .stats_thresh0 = 0x0,
    .stats_thresh1 = 0x0,
    .stats_thresh2 = 0x0,
    .stats_thresh3 = 0x0,
    .stats_thresh4 = 0x0,
    .stats_thresh5 = 0x0,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_store_4chan_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_4_CHANNEL,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 1,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_load_6chan_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_6_CHANNEL,
    .pipeline_store_en = PA_STORE_DISABLE,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_load_4chan_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_4_CHANNEL,
    .pipeline_store_en = PA_STORE_DISABLE,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 1,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 0,
    .stats_en = 0,
    .stats_thresh0 = 0x3ff,
    .stats_thresh1 = 0x3ff,
    .stats_thresh2 = 0x3ff,
    .stats_thresh3 = 0x3ff,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_store_filter_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_DISABLE,
    .pipeline_store_en = PA_STORE_6_CHANNEL,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 1,
    .stats_en = 1,
    .stats_thresh0 = 0x0,
    .stats_thresh1 = 0x0,
    .stats_thresh2 = 0x0,
    .stats_thresh3 = 0x0,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};

pipe_access_cfg_t pipe_access_store_load_filter_cfg = 
{
    .up0_chans = 5,
    .up0_height = 10,
    .up0_width = 128,
    .up1_chans = 8,
    .up1_height = 10,
    .up1_width = 128,
    .lut_cbi = 1,
    .pipeline_load_en = PA_LOAD_6_CHANNEL,
    .pipeline_store_en = PA_STORE_6_CHANNEL,
    .bypass = 0,
    .pack_8_chan_sel = 0,
    .pack_32_chan_sel = 0,
    .pack_sideband_msb = 0,
    .pack_shift_2 = 0,
    .pack_mode = 0,
    .unpack_sideband_fw_val = 0,
    .unpack_8_chan_sel = 0,
    .unpack_32_chan_sel = 0,
    .unpack_use_sideband_fw = 0,
    .unpack_sideband_msb = 0,
    .unpack_shift_2 = 0,
    .unpack_mode = 0,
    .lut_5_input_sel = 3,
    .lut_4_input_sel = 4,
    .lut_3_input_sel = 1,
    .per_pixel_enable = 0,
    .per_pixel_polarity = 0,
    .per_pixel_sel = 0,
    .stats_image_width = 0,
    .stats_patch_height = 1,
    .stats_en = 1,
    .stats_thresh0 = 0x0,
    .stats_thresh1 = 0x0,
    .stats_thresh2 = 0x0,
    .stats_thresh3 = 0x0,
    .stats_thresh4 = 0x3ff,
    .stats_thresh5 = 0x3ff,
    .writehex = 1
};
