/****************************************************************************** 
 * Copyright (c) 2009 - 2013  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/// marvell ip
///

th_ucell_cfg_t th_ucell_106_1bit =
{
  .bpp         = 1,
  .concat      = 0,
  .height      = 8,
  .width       = 8,
  .offset      = 0,
  .stoc_height = 64,
  .stoc_width  = 64,
  .start_height = 0,
  .start_width = 0,
  .thresh_hi   = 65535,
  .thresh_lo   = 0,

  .lut_minval  = 144,
  .lut_maxval  = 1167,
  .lut_data    = {0},

  .ucell_data  = {
    0x10010039, 0x1000003F, 0x1000003D, 0x1000001A, 0x10000008, 0x10000002,
    0x10000004, 0x10000016, 0x00010031, 0x0000003B, 0x00000033, 0x0000002B,
    0x00000010, 0x00000006, 0x0000000E, 0x0000001E, 0x00010029, 0x00000013,
    0x0000001B, 0x00000023, 0x00000020, 0x00000018, 0x0000002E, 0x00000026,
    0x00010011, 0x00000009, 0x0000000B, 0x00000028, 0x00000030, 0x00000038,
    0x00000036, 0x00000021, 0x00010007, 0x00000001, 0x00000003, 0x00000015,
    0x0000003A, 0x00000040, 0x0000003E, 0x00000019, 0x0001000F, 0x00000005,
    0x0000000D, 0x0000001D, 0x00000032, 0x0000003C, 0x00000034, 0x0000002C,
    0x0001001F, 0x00000017, 0x0000002D, 0x00000025, 0x0000002A, 0x00000014,
    0x0000001C, 0x00000024, 0x0001002F, 0x00000037, 0x00000035, 0x00000022,
    0x00000012, 0x0000000A, 0x0000000C, 0x00000027 },
};

th_cfg_t th_106lpi_1bit_cfg =
{
    .bypass          = 0,
    .alt_lut_dbl     = 0,
    .use_obj_type    = 0,
    .en_partial_ht   = 1,
    .direct_thresh   = 0,
    .mix_off         = 1,  // 1 = false
    .alt_lut_lfsr    = 0,

    .alt_lut_bpp_conv   = 0,  // (0 => 1bpp, 1 => 2bpp, 2 => 3bpp, 3 => 4bpp)
    .cfg_concat         = 0,
    .output_just_concat = 0,
    .pw_mux_bpp_conv    = 0,  // (0 => 1bpp, 1 => 2bpp, 2 => 3bpp, 3 => 4bpp)
    .edge_override      = 1,
    .thresh_edge        = 0,
    .thresh_pw          = 0,
    .LFSR               = 0,

    .ucell0         = &th_ucell_106_1bit,
    .ucell1         = &th_ucell_zero, // &th_ucell_106_2bit,

    .stoc_data      = th_stoc_k_data,
    .stoc_size      = TH_STOC_K_SIZE,

    .alt_lut_data   = 0,
};
