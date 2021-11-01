/* include this once C file within a C file this is one record in an array of records */

{  // duplicate 7 == 11
    .cfg_mode = IPC_PRINT_MONO8_DETAIL,
    .cfg_name = "Mono Print Detail",
    .mono_nColor = true,                  // 8bit gray input		
    .imagew = 0,                          // Image Width
    .imageh = 0,                          // Image Height
    .bpp = 2,                             // Bits Per Pixel
    .ahs_cfg = NULL,                      // Auto Halftone
    .cr_cfg = NULL,                       // Color Replacement
    .csc_cfg = NULL,                      // Color Space Conversion
    .fhs_cfg = &fhs_generic_cfg,          // Fractional Horizontal Scaling
    .th_cfg[0] = &th_106lpi_1bit_cfg,     // Halftone (Threshold) 
    .th_cfg[1] = &th_106lpi_1bit_cfg,     // Halftone (Threshold)
    .th_cfg[2] = &th_106lpi_1bit_cfg,     // Halftone (Threshold) 
//    .th_cfg[3] = &th_141lpi_1bit_cfg,     // Halftone (Threshold)  141 lpi screen.
    .th_cfg[3] = &th_106lpi_1bit_cfg, // &th_cmyk_106lpi_2bit_K_cfg, //&th_106lpi_1bit_cfg,        // Halftone (Threshold)
    .ued_cfg = NULL,                      // Halftone (UED)
    .kmod_cfg = NULL,                     // BlacK Modification
    .nga_cfg = NULL,                      // Neutral Grey Adjust
    .raster_odma_cfg[0] = NULL,           // Output DMA 0
    .raster_odma_cfg[1] = NULL,           // Output DMA 1
    .raster_odma_cfg[2] = NULL,           // Output DMA 2
    .raster_odma_cfg[3] = NULL,           // Output DMA 3
    .raster_odma_cfg[4] = &raster_odma_2_bpp_no_serp_cfg, // Output DMA 4
    .raster_odma_cfg[5] = NULL,           // Output DMA 5
    .raster_odma_cfg[6] = NULL,           // Output DMA 6
    .raster_odma_cfg[7] = NULL,           // Output DMA 7
    .pipe_access_cfg = &pipe_access_mono8_cfg, // Pipeline Access; 1D LUT required for gray.
    .pa_idma_cfg = NULL,                  // Pipeline Access IDMA
    .pa_odma_cfg = NULL,                  // Pipeline Access ODMA
    .serp_cfg = &serp_300_cfg,         // Serpentine
    .sccsc_cfg = &sccsc_bypass_cfg,       // Subcube CSC
    .tp_cfg = NULL,                       // Text Pad
    .cp_cfg = NULL,                       // Contone Pad
    .mip_cfg = NULL,                      // mip_cfg
    .yipi_cfg = NULL,                     // Yellow Insertion Pixel ID
    .stat_cfg = &stat_bypass_cfg,         // PIP Statistics
#if ASIC_REV >= ASIC_REV_B0  && ASIC_gemstone
    .sharpen_cfg = &sharpen_bypass_cfg,   // Sharpen
    .ctwin_cfg = NULL,                    // CTWIN
    .tems_cfg = &tems_bypass_cfg,         // TEMS

    .cre_cfg = &color_reg_enhance_bypass_cfg, // CRE
#endif
    // cre is hacked at the moment. lsptodo
    .db_ued_cfg = &db_ued_generic_cfg,    // DavBuf for UED
    .db_kmod_cfg = &db_kmod_generic_cfg,  // DavBuf for KMOD
    .db_serp_cfg = &db_serp_generic_cfg,  // DavBuf for SERP
    .sccsc_table = (uint8_t *)ScCSC_i3DLUT_Identity, // Subcube CSC Table
    .onedlut0 = (uint8_t *)PA_zero,    // 1D LUT 0
    .onedlut1 = (uint8_t *)PA_zero,    // 1D LUT 1
    .onedlut2 = (uint8_t *)PA_zero,    // 1D LUT 2
    .onedlut3 = (uint8_t *)PA_zero,    // 1D LUT 3
    .onedlut4 = (uint8_t *)PA_identity, //inverse, // 1D LUT 4  invert !
    .onedlut5 = (uint8_t *)PA_zero     // 1D LUT 5
},
