th_ucell_cfg_t th_ucell_141_1bit =
{
  .bpp         = 1,
  .concat      = 0,
  .height      = 6,
  .width       = 6,
  .offset      = 0,
  .stoc_height = 64,
  .stoc_width  = 64,
  .start_height = 0,
  .start_width = 0,
  .thresh_hi   = 65535,
  .thresh_lo   = 0,

  .lut_minval  = 144,
  .lut_maxval  = 719,
  .lut_data    = {0},

  .ucell_data  = {
    0x1001000F, 0x10000003, 0x1000000B, 0x10000015, 0x10000021, 0x10000019,
    0x00010007, 0x00000001, 0x00000005, 0x0000001D, 0x00000023, 0x0000001F,
    0x0001000D, 0x00000009, 0x00000011, 0x00000017, 0x0000001B, 0x00000013,
    0x00010016, 0x00000022, 0x0000001A, 0x00000010, 0x00000004, 0x0000000C,
    0x0001001E, 0x00000024, 0x00000020, 0x00000008, 0x00000002, 0x00000006,
    0x00010018, 0x0000001C, 0x00000014, 0x0000000E, 0x0000000A, 0x00000012 },
};

th_cfg_t th_141lpi_1bit_cfg =
{
    .bypass          = 0,
    .alt_lut_dbl     = 0,
    .use_obj_type    = 0,
    .en_partial_ht   = 0,
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

    .ucell0         = &th_ucell_141_1bit,
    .ucell1         = &th_ucell_141_1bit,

    .stoc_data      = th_stoc_k_data,
    .stoc_size      = TH_STOC_K_SIZE,

    .alt_lut_data   = 0,
};
