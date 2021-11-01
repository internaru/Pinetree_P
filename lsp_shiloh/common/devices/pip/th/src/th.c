/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  * This file contains functions for the Threshold Halftone block.
  * 
 **/

#include "th_api.h"
#include "logger.h"
#include "lassert.h"

#define DBG_PRFX "TH: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

static void th_cpu_sram_access(CORE_REGS_t *th_reg, uint32_t bit)
{
    uint32_t reg_val;

    reg_val      = th_reg->TCR;
    reg_val      = CORE_TCR_CPUACCESS_REPLACE_VAL(reg_val, bit);
    th_reg->TCR  = reg_val;
}

//********************************
// th_cfg_driver - write registers
//********************************

void th_cfg_driver(CORE_REGS_t *th_reg,
                   SRAM_REGS_t *th_sram_reg,
                   th_cfg_t *th_cfg,
                   uint32_t start_row)
{
    uint32_t reg_val;
    uint32_t i;

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);
    //if (th_cfg->bypass) {
    //  DPRINTF((DBG_OUTPUT|DBG_LOUD), ("     th put in bypass\n"));
    //}

    // -------------
    // TCR register config
    // -------------
    // Read
    reg_val      = th_reg->TCR;
    // Modify
    reg_val      = CORE_TCR_BYPASS_REPLACE_VAL       (reg_val, th_cfg->bypass);
    reg_val      = CORE_TCR_USEOBJTYPE_REPLACE_VAL   (reg_val, th_cfg->use_obj_type);
    reg_val      = CORE_TCR_ENABLEPARTIALHALFTONE_REPLACE_VAL (reg_val, th_cfg->en_partial_ht);
    reg_val      = CORE_TCR_CFGCONCAT_REPLACE_VAL    (reg_val, th_cfg->cfg_concat);
    reg_val      = CORE_TCR_OUTPUTJUSTCONCAT_REPLACE_VAL (reg_val, th_cfg->output_just_concat);
    reg_val      = CORE_TCR_PWMUXBPPCONV_REPLACE_VAL (reg_val, th_cfg->pw_mux_bpp_conv);
    reg_val      = CORE_TCR_EDGEOVERRIDE_REPLACE_VAL (reg_val, th_cfg->edge_override);
    // Alt LUT:
#if ASIC_REV >= ASIC_REV_B0 && ASIC_gemstone
    reg_val      = CORE_TCR_ALTLUTLFSR_REPLACE_VAL   (reg_val, th_cfg->alt_lut_lfsr);
    reg_val      = CORE_TCR_MIXOFF_REPLACE_VAL       (reg_val, th_cfg->mix_off);
#endif
    reg_val      = CORE_TCR_ALTLUTDBL_REPLACE_VAL    (reg_val, th_cfg->alt_lut_dbl);
    reg_val      = CORE_TCR_ALTLUTBPPCONV_REPLACE_VAL(reg_val, th_cfg->alt_lut_bpp_conv);
    // Threshold:
    reg_val      = CORE_TCR_DIRECTTHRESH_REPLACE_VAL (reg_val, th_cfg->direct_thresh);
    // Write
    DBG_PRINTF_NOTICE( "threshold TCR %x\n", reg_val);
    th_reg->TCR  = reg_val;

#if ASIC_REV >= ASIC_REV_B0 && ASIC_gemstone
    // TALR
    reg_val      = th_reg->TALR;
    reg_val      = CORE_TALR_ALTLUTLFSR_REPLACE_VAL(reg_val, th_cfg->LFSR);
    th_reg->TALR = reg_val;
#endif

    reg_val       = th_reg->TTER;
    reg_val       = CORE_TTER_THRESHEDGE_REPLACE_VAL(reg_val, th_cfg->thresh_edge);
    reg_val       = CORE_TTER_THRESHPW_REPLACE_VAL(reg_val, th_cfg->thresh_pw);
    th_reg->TTER  = reg_val;

    if (th_cfg->bypass) {
      // In bypass mode don't set up all the microcells and LUTs.
      return;
    }

    th_cpu_sram_access(th_reg, 1);

    // Alt LUT
    for (i = 0; i < 256; i++) {
      uint8_t lutval = (th_cfg->alt_lut_data == 0)
                       ?  0
                       :  th_cfg->alt_lut_data[i];
      uint32_t regval = th_sram_reg->ALUT[i];
      // stefan hack in all on alt lut. 
      // regval = 0xff;
      regval = SRAM_ALUT_FIELD_REPLACE_VAL(regval, lutval);
      th_sram_reg->ALUT[i] = regval;
    }

    // Stochastic matrix
    // Note:  In mode 7, we have a single HT unit that has 4096x32 bits.
    //        In modes 4/5/6, we have 4 HT units, each with 1024x32 bits.
    ASSERT( th_cfg->stoc_size <= 16384 );
    uint32_t stoc_words = (th_cfg->stoc_size+3)/4;
    if (th_cfg->stoc_data == 0) {
      for (i = 0; i < stoc_words; i++)
        th_sram_reg->STOC[i] = 0;
    } else {
      uint8_t* sdata = th_cfg->stoc_data;
      for (i = 0; i < stoc_words; i++) {
        // Note the swapping.
        uint32_t word = ((uint32_t)sdata[3] << 24) |
                        ((uint32_t)sdata[2] << 16) | 
                        ((uint32_t)sdata[1] <<  8) |
                        ((uint32_t)sdata[0] <<  0);
        sdata += 4;
        th_sram_reg->STOC[i] = word;
      }
    }

    // Set up uCells
    ASSERT(th_cfg->ucell0 != 0);
    ASSERT(th_cfg->ucell1 != 0);
    th_ucell_cfg_t* u0 = th_cfg->ucell0;
    th_ucell_cfg_t* u1 = th_cfg->ucell1;
    uint32_t u0size = u0->width * u0->height;
    uint32_t u1size = u1->width * u1->height;

    ASSERT( (u0->width > 0) && (u0->height > 0) && (u0size <= 256) );
    ASSERT( (u1->width > 0) && (u1->height > 0) && (u1size <= 256) );
    ASSERT( (u0->stoc_height * u0->stoc_width) <= th_cfg->stoc_size );
    ASSERT( (u1->stoc_height * u1->stoc_width) <= th_cfg->stoc_size );
    //    ASSERT( u0->offset < u0->width );
    // ASSERT( u1->offset < u1->width );

    // Verify uCell's bpp and concat match general config
//    ASSERT( u0->bpp == (th_cfg->pw_mux_bpp_conv + 1) );
//    ASSERT( u1->bpp == (th_cfg->pw_mux_bpp_conv + 1) );
    ASSERT( u0->concat == th_cfg->cfg_concat );
    //    ASSERT( u1->concat == th_cfg->cfg_concat );

    // -------------
    // Cell Dimensions
    // -------------

    // The microcell input is in normal form.  For example, if you have a uCell
    // that is 8 wide, 4 high, and wants to evenly tile, then you it should
    // contain:  height: 4, width: 8, offset: 4.  This is both easier to read
    // and also isolates our microcell definitions from the hardware choices.
    //
    // The hardware wants the height-1, width-1, and for offset it wants the
    // hardware width (i.e. the real width minus one) minus the desired offset
    // plus one.

    reg_val       = th_reg->TMCR0;
    reg_val       = CORE_TMCR0_CELLHEIGHT_REPLACE_VAL(reg_val, u0->height>0 ? u0->height - 1 : 0);
    reg_val       = CORE_TMCR0_CELLWIDTH_REPLACE_VAL(reg_val, u0->width>0 ? u0->width - 1 : 0);
    // reg_val       = CORE_TMCR0_CELLOFFSET_REPLACE_VAL(reg_val, u0->offset>0 ? (u0->width-1) - u0->offset + 1 : 0);
    reg_val       = CORE_TMCR0_CELLOFFSET_REPLACE_VAL(reg_val, u0->offset );
    th_reg->TMCR0 = reg_val;

    reg_val       = th_reg->TMCR1;
    reg_val       = CORE_TMCR1_CELLHEIGHT_REPLACE_VAL(reg_val, u1->height>0 ? u1->height - 1 : 0);
    reg_val       = CORE_TMCR1_CELLWIDTH_REPLACE_VAL(reg_val, u1->width>0 ? u1->width - 1 : 0);
    // reg_val       = CORE_TMCR1_CELLOFFSET_REPLACE_VAL(reg_val, u1->offset>0 ? (u1->width-1) - u1->offset + 1 : 0);
    reg_val       = CORE_TMCR1_CELLOFFSET_REPLACE_VAL(reg_val, u0->offset );
    th_reg->TMCR1 = reg_val;

    {
      uint32_t start_height = u0->start_height;
      uint32_t start_width  = u0->start_width;
      uint32_t cell_offset = (start_height + start_row) % u0->height;
      reg_val       = th_reg->TCSR0;
      reg_val       = CORE_TCSR0_STARTHEIGHT_REPLACE_VAL(reg_val, cell_offset);
      reg_val       = CORE_TCSR0_STARTWIDTH_REPLACE_VAL(reg_val, start_width);
      th_reg->TCSR0 = reg_val;
    }

    {
      uint32_t start_height = u1->start_height;
      uint32_t start_width  = u1->start_width;
      uint32_t cell_offset = (start_height + start_row) % u1->height;
      reg_val       = th_reg->TCSR1;
      reg_val       = CORE_TCSR1_STARTHEIGHT_REPLACE_VAL(reg_val, cell_offset);
      reg_val       = CORE_TCSR1_STARTWIDTH_REPLACE_VAL(reg_val, start_width);
      th_reg->TCSR1 = reg_val;
    }

    reg_val       = th_reg->TSSR0;
    reg_val       = CORE_TSSR0_STOCHEIGHT_REPLACE_VAL(reg_val, u0->stoc_height-1);
    reg_val       = CORE_TSSR0_STOCWIDTH_REPLACE_VAL(reg_val, u0->stoc_width-1);
    th_reg->TSSR0 = reg_val;

    reg_val       = th_reg->TSSR1;
    reg_val       = CORE_TSSR1_STOCHEIGHT_REPLACE_VAL(reg_val, u1->stoc_height-1);
    reg_val       = CORE_TSSR1_STOCWIDTH_REPLACE_VAL(reg_val, u1->stoc_width-1);
    th_reg->TSSR1 = reg_val;

    reg_val       = th_reg->TTR0;
    reg_val       = CORE_TTR0_THRESHHI_REPLACE_VAL(reg_val, u0->thresh_hi);
    reg_val       = CORE_TTR0_THRESHLO_REPLACE_VAL(reg_val, u0->thresh_lo);
    th_reg->TTR0  = reg_val;

    reg_val       = th_reg->TTR1;
    reg_val       = CORE_TTR1_THRESHHI_REPLACE_VAL(reg_val, u1->thresh_hi);
    reg_val       = CORE_TTR1_THRESHLO_REPLACE_VAL(reg_val, u1->thresh_lo);
    th_reg->TTR1  = reg_val;


    // Program microcells and LUTs.

    for (i = 0; i < 256; i++)
    {
        th_sram_reg->CELL0[i] = (i < u0size) ? u0->ucell_data[i] : 0;
        th_sram_reg->CELL1[i] = (i < u1size) ? u1->ucell_data[i] : 0;
    }

    // Program the INLUT with the data supplied if lut_maxval is 0.  Otherwise
    // set the INLUT to a linear ramp based on the min and max LUT values.
    if (u0->lut_maxval == 0) {
      for (i = 0; i < 256; i++) 
        th_sram_reg->ILUT[    i] = u0->lut_data[i];
    } else {
      th_sram_reg->ILUT[    0] = 0;
      for (i = 1; i < 256; i++)
        th_sram_reg->ILUT[    i] = u0->lut_minval + (((i-1) * (u0->lut_maxval - u0->lut_minval) + 127) / 254);
    }

    if (u1->lut_maxval == 0) {
      for (i = 0; i < 256; i++) 
        th_sram_reg->ILUT[256+i] = u1->lut_data[i];
    } else {
      th_sram_reg->ILUT[256+0] = 0;
      for (i = 1; i < 256; i++)
        th_sram_reg->ILUT[256+i] = u1->lut_minval + (((i-1) * (u1->lut_maxval - u1->lut_minval) + 127) / 254);
    }

    th_cpu_sram_access(th_reg, 0);
}
