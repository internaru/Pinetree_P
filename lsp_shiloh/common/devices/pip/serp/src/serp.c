/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the SERP block.
  * 
 **/

#include "asic.h"
#include "serp_api.h"
#include "logger.h"

#define DBG_PRFX "SERP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

// TODO: Add accessor for SCR tlb_sram_access_enable bit

//********************************
// serp_cfg_driver - setup the majority of the SERP block
//********************************
void serp_cfg_driver(SERP_REGS_t *serp_reg, serp_cfg_t *serp_cfg)
{
    uint32_t reg_val;

    DBG_PRINTF_NOTICE("Called serp_cfg_driver() \n\r");

    // SERP Configuration Register (SCR)
    reg_val       = serp_reg->SCR;
    reg_val       = SERP_SCR_EDGE_STRENGTH_ENABLE_REPLACE_VAL(reg_val, serp_cfg->edge_strength_en);
    reg_val       = SERP_SCR_OBJ_MERGE_ENABLE_REPLACE_VAL(reg_val, serp_cfg->obj_merge_enable);
    reg_val       = SERP_SCR_OBJ_ENCODED_SELECT_REPLACE_VAL(reg_val, serp_cfg->obj_encoded_select);
    reg_val       = SERP_SCR_BLACK_CORE_ENABLE_REPLACE_VAL(reg_val, serp_cfg->black_core_en);
    reg_val       = SERP_SCR_HR1_ENABLE_REPLACE_VAL(reg_val, serp_cfg->hr1_en);
    reg_val       = SERP_SCR_HR0_ENABLE_REPLACE_VAL(reg_val, serp_cfg->hr0_en);
    reg_val       = SERP_SCR_TEXT_MERGE_ENABLE_REPLACE_VAL(reg_val, serp_cfg->text_merge_enable);
    reg_val       = SERP_SCR_SERP_START_DIR_REPLACE_VAL(reg_val, serp_cfg->serp_start_dir);
    reg_val       = SERP_SCR_SERPENTINE_DATA_REPLACE_VAL(reg_val, serp_cfg->serpentine_data);
    reg_val       = SERP_SCR_BYPASS_ENABLE_REPLACE_VAL(reg_val, serp_cfg->bypass);
    DBG_PRINTF_INFO(" scr %x\n", reg_val);
    serp_reg->SCR = reg_val;

    // SERP Halftone Replace Register (SHRR)
    reg_val        = serp_reg->SHRR;
    reg_val        = SERP_SHRR_HR1_OUTCHAN_REPLACE_VAL(reg_val, serp_cfg->hr1_outchan);
    reg_val        = SERP_SHRR_HR1_INCONFIG_REPLACE_VAL(reg_val, serp_cfg->hr1_inconfig);
    reg_val        = SERP_SHRR_HR0_OUTCHAN_REPLACE_VAL(reg_val, serp_cfg->hr0_outchan);
    reg_val        = SERP_SHRR_HR0_INCONFIG_REPLACE_VAL(reg_val, serp_cfg->hr0_inconfig);
    serp_reg->SHRR = reg_val;

    // SERP Blank Threshold A Register (SBTAR)
    reg_val         = serp_reg->SBTAR;
    reg_val         = SERP_SBTAR_CH1_BLANKTHRESH_REPLACE_VAL(reg_val, serp_cfg->color_thresh[1]);
    reg_val         = SERP_SBTAR_CH0_BLANKTHRESH_REPLACE_VAL(reg_val, serp_cfg->color_thresh[0]);
    serp_reg->SBTAR = reg_val;

    // SERP Blank Threshold B Register (SBTBR)
    reg_val         = serp_reg->SBTBR;
    reg_val         = SERP_SBTBR_CH3_BLANKTHRESH_REPLACE_VAL(reg_val, serp_cfg->color_thresh[3]);
    reg_val         = SERP_SBTBR_CH2_BLANKTHRESH_REPLACE_VAL(reg_val, serp_cfg->color_thresh[2]);
    serp_reg->SBTBR = reg_val;

    // SERP Blank Threshold C Register (SBTCR)
    reg_val         = serp_reg->SBTCR;
    reg_val         = SERP_SBTCR_CH5_BLANKTHRESH_REPLACE_VAL(reg_val, serp_cfg->color_thresh[5]);
    reg_val         = SERP_SBTCR_CH4_BLANKTHRESH_REPLACE_VAL(reg_val, serp_cfg->color_thresh[4]);
    serp_reg->SBTCR = reg_val;

    // SERP Scale Register (SSR)
    reg_val       = serp_reg->SSR;
    reg_val       = SERP_SSR_TEXT_VSCALE_REPLACE_VAL(reg_val, serp_cfg->text_v_scale);
    reg_val       = SERP_SSR_TEXT_HSCALE_REPLACE_VAL(reg_val, serp_cfg->text_h_scale);
    reg_val       = SERP_SSR_CONTONE_VSCALE_REPLACE_VAL(reg_val, serp_cfg->contone_v_scale);
    reg_val       = SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_REPLACE_VAL(reg_val, serp_cfg->v_bilinear);
    reg_val       = SERP_SSR_CONTONE_HSCALE_REPLACE_VAL(reg_val, serp_cfg->contone_h_scale);
    reg_val       = SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_REPLACE_VAL(reg_val, serp_cfg->h_bilinear);
    serp_reg->SSR = reg_val;

    // SERP Text Merge Register (STMCR)
    reg_val         = serp_reg->STMCR;
    reg_val         = SERP_STMCR_TEXT_THRESHOLD_REPLACE_VAL(reg_val, serp_cfg->text_threshold);
    reg_val         = SERP_STMCR_TEXT_REPLACE_MODE_REPLACE_VAL(reg_val, serp_cfg->text_replace_mode);
    reg_val         = SERP_STMCR_TEXT_SOURCE_REPLACE_VAL(reg_val, serp_cfg->text_source);
    serp_reg->STMCR = reg_val;

    // SERP Text Mapping A Register (STMAR)
    reg_val         = serp_reg->STMAR;
    reg_val         = SERP_STMAR_TEXT1_MAPPING_REPLACE_VAL(reg_val, serp_cfg->text_map_1);
    reg_val         = SERP_STMAR_TEXT0_MAPPING_REPLACE_VAL(reg_val, serp_cfg->text_map_0);
    serp_reg->STMAR = reg_val;

    // SERP Text Mapping B Register (STMBR)
    reg_val         = serp_reg->STMBR;
    reg_val         = SERP_STMBR_TEXT3_MAPPING_REPLACE_VAL(reg_val, serp_cfg->text_map_3);
    reg_val         = SERP_STMBR_TEXT2_MAPPING_REPLACE_VAL(reg_val, serp_cfg->text_map_2);
    serp_reg->STMBR = reg_val;

    // SERP Black Core Register (SBCR)
    reg_val        = serp_reg->SBCR;
    reg_val        = SERP_SBCR_BLACK_CORE_CH5_EN_REPLACE_VAL(reg_val, serp_cfg->black_core_ch5_en);
    reg_val        = SERP_SBCR_BLACK_CORE_CH3_EN_REPLACE_VAL(reg_val, serp_cfg->black_core_ch3_en);
    reg_val        = SERP_SBCR_BLACK_CORE_CH2_EN_REPLACE_VAL(reg_val, serp_cfg->black_core_ch2_en);
    reg_val        = SERP_SBCR_BLACK_CORE_CH1_EN_REPLACE_VAL(reg_val, serp_cfg->black_core_ch1_en);
    reg_val        = SERP_SBCR_BLACK_CORE_CH0_EN_REPLACE_VAL(reg_val, serp_cfg->black_core_ch0_en);
    reg_val        = SERP_SBCR_BLACK_CORE_THRESH_REPLACE_VAL(reg_val, serp_cfg->black_core_thresh);
    serp_reg->SBCR = reg_val;

    // SERP Edge Strength Config A Register (SECA)
    reg_val        = serp_reg->SECA;
    reg_val        = SERP_SECA_EDGE_OUT_MSB_REPLACE_VAL(reg_val, serp_cfg->edge_out_msb);
    reg_val        = SERP_SECA_EDGE_IN_MSB_REPLACE_VAL(reg_val, serp_cfg->edge_in_msb);
    reg_val        = SERP_SECA_VSCALE_REPLACE_VAL(reg_val, serp_cfg->edge_vscale);
    reg_val        = SERP_SECA_HSCALE_REPLACE_VAL(reg_val, serp_cfg->edge_hscale);
    reg_val        = SERP_SECA_INVERT_DATA_REPLACE_VAL(reg_val, serp_cfg->edge_invert_data);
    reg_val        = SERP_SECA_EDGE_ZERO_REPLACE_VAL(reg_val, serp_cfg->edge_zero);
    serp_reg->SECA = reg_val;

    // SERP Edge Strength Config B Register (SECB)
    reg_val        = serp_reg->SECB;
    reg_val        = SERP_SECB_EDGE_THRESH_REPLACE_VAL(reg_val, serp_cfg->edge_thresh);
    reg_val        = SERP_SECB_EDGE_SCALE_REPLACE_VAL(reg_val, serp_cfg->edge_scale);
    reg_val        = SERP_SECB_EDGE_OFFSET_REPLACE_VAL(reg_val, serp_cfg->edge_offset);
    serp_reg->SECB = reg_val;

#if ASIC_REV >= ASIC_REV_B0 && ASIC_gemstone
    // SERP High Resolution Text Register (SHI)
    reg_val = serp_reg->SHI;
    reg_val = SERP_SHI_HIGH_RES_SOURCE_REPLACE_VAL(reg_val, serp_cfg->high_res_source);
    reg_val = SERP_SHI_TEXT_RES_REPLACE_VAL(reg_val, serp_cfg->text_res);
    reg_val = SERP_SHI_HIGH_RES_TEXT_ENABLE_REPLACE_VAL(reg_val, serp_cfg->high_res_text_enable);
    serp_reg->SHI = reg_val;
#endif
}
