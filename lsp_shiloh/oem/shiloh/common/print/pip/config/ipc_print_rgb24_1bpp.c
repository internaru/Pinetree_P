/****************************************************************************** 
 * Copyright (c) 2009 - 2013  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/* This is one record in an array of records 
** do not compile and link this as a file.
** this is a single array entry in pip_cfg.c 
** of type: 
** pip_cfg_t 
*/

{
    .cfg_mode = IPC_PRINT_RGB24_1BPP,
    .cfg_name = "Mono 1 bit K output RGB input",
    .mono_nColor = false,             // output dma is 1 channel not 4.
    .imagew = 0,                          // Image Width
    .imageh = 0,                          // Image Height
    .bpp = 1,                             // Bits Per Pixel
    .ahs_cfg = NULL,                      // Auto Halftone
    .cr_cfg = NULL,                       // Color Replacement
    .csc_cfg = NULL,                      // Color Space Conversion
    .fhs_cfg = &fhs_generic_cfg,          // Fractional Horizontal Scaling
    .th_cfg[0] = &th_cmyk_106lpi_1bit_C_cfg,        // Halftone (Threshold)
    .th_cfg[1] = &th_cmyk_106lpi_1bit_M_cfg,        // Halftone (Threshold)
    .th_cfg[2] = &th_cmyk_106lpi_1bit_Y_cfg,        // Halftone (Threshold)
    .th_cfg[3] = &th_cmyk_106lpi_1bit_K_cfg, //&th_106lpi_1bit_cfg,        // Halftone (Threshold)
    .ued_cfg = NULL,                      // Halftone (UED)
    .kmod_cfg = NULL,                     // BlacK Modification
    .nga_cfg = NULL,                      // Neutral Grey Adjust
    .raster_odma_cfg[0] = NULL,           // Output DMA 0
    .raster_odma_cfg[1] = NULL,           // Output DMA 1
    .raster_odma_cfg[2] = NULL,           // Output DMA 2
    .raster_odma_cfg[3] = NULL,           // Output DMA 3
    .raster_odma_cfg[4] = &raster_odma_1_bpp_no_serp_cfg, // Output DMA 4
    .raster_odma_cfg[5] = NULL,           // Output DMA 5
    .raster_odma_cfg[6] = NULL,           // Output DMA 5
    .raster_odma_cfg[7] = NULL,           // Output DMA 5
    .pipe_access_cfg = &pipe_access_mono8_cfg, // Pipeline Access
    .pa_idma_cfg = NULL,                  // Pipeline Access IDMA
    .pa_odma_cfg = NULL,                  // Pipeline Access ODMA
    .serp_cfg = &serp_300_cfg, // &serp_bypass_cfg,         // Serpentine
    .sccsc_cfg = &sccsc_bypass_cfg,       // Subcube CSC
    .tp_cfg = NULL,                       // Text Pad
    .cp_cfg = NULL,                       // Contone Pad
    .mip_cfg = NULL,                      // mip_cfg
    .yipi_cfg = NULL,                     // Yellow Insertion Pixel ID
    .stat_cfg = &stat_test_cfg,           // PIP Statistics
#if ASIC_REV >= ASIC_REV_B0  && ASIC_gemstone
    .sharpen_cfg = &sharpen_bypass_cfg,   // Sharpen
    .ctwin_cfg = NULL,                    // CTWIN
    .tems_cfg = &tems_bypass_cfg,         // TEMS
    .cre_cfg = &color_reg_enhance_bypass_cfg, // CRE
#endif
    .db_ued_cfg = &db_ued_generic_cfg,    // DavBuf for UED
    .db_kmod_cfg = &db_kmod_generic_cfg,  // DavBuf for KMOD
    .db_serp_cfg = &db_serp_generic_cfg,  // DavBuf for SERP
		//.sccsc_table = (uint8_t *)sccsc_6ch_8bit_colormap_linear_5bgr, // Subcube CSC Table
    .sccsc_table = (uint8_t *)ScCSC_i3DLUT_Identity, // Subcube CSC Table
    .onedlut0 = (uint8_t *)PA_identity,    // 1D LUT 0
    .onedlut1 = (uint8_t *)PA_identity,    // 1D LUT 1
    .onedlut2 = (uint8_t *)PA_identity,    // 1D LUT 2
    .onedlut3 = (uint8_t *)PA_zero,    // 1D LUT 3
    .onedlut4 = (uint8_t *)PA_inverse, //PA_inv1d_th_smooth, // 1D LUT 4
    .onedlut5 = (uint8_t *)PA_zero     // 1D LUT 5
},
