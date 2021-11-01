/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the HT (UED) block.
  * 
 **/

#if ASIC_gemstone && (ASIC_REV < ASIC_REV_Z3)
    #error "This UED driver is for Gemstone Z3 and later"
#endif

#include <stdint.h>
#include "logger.h"
#include "ued_api.h"

#define DBG_PRFX "UED: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )


// -----------------------------
// UED private function prototypes
// -----------------------------
static void ued_cpu_sram_access(UED_ADVANCED_REGS_t *ued_reg,
                                uint32_t bit);
static void ued_blut_load(UED_ADVANCED_REGS_t *ued_reg,
                          uint32_t* blut0_data,
                          uint32_t* blut1_data,
                          uint32_t* blut2_data,
                          uint32_t* blut3_data,
                          uint32_t* blut4_data,
                          uint32_t* blut5_data);
static void ued_blut_check(UED_ADVANCED_REGS_t *ued_reg,
                           uint32_t* blut0_data,
                           uint32_t* blut1_data,
                           uint32_t* blut2_data,
                           uint32_t* blut3_data,
                           uint32_t* blut4_data,
                           uint32_t* blut5_data);

static void ued_clut_load(UED_ADVANCED_REGS_t *ued_reg,
                          uint8_t* clut_bias0_data,
                          uint16_t* clut_thresh0_data,
                          uint8_t* clut_bias1_data,
                          uint16_t* clut_thresh1_data,
                          uint8_t* clut_bias2_data,
                          uint16_t* clut_thresh2_data,
                          uint8_t* clut_bias3_data,
                          uint16_t* clut_thresh3_data,
                          uint8_t* clut_bias4_data,
                          uint16_t* clut_thresh4_data,
                          uint8_t* clut_bias5_data,
                          uint16_t* clut_thresh5_data);

//static
 void ued_clut_check(UED_ADVANCED_REGS_t *ued_reg,
                           uint8_t* clut_bias0_data,
                           uint16_t* clut_thresh0_data,
                           uint8_t* clut_bias1_data,
                           uint16_t* clut_thresh1_data,
                           uint8_t* clut_bias2_data,
                           uint16_t* clut_thresh2_data,
                           uint8_t* clut_bias3_data,
                           uint16_t* clut_thresh3_data,
                           uint8_t* clut_bias4_data,
                           uint16_t* clut_thresh4_data,
                           uint8_t* clut_bias5_data,
                           uint16_t* clut_thresh5_data);

static void ued_wlut_load(UED_ADVANCED_REGS_t *ued_reg,
                          uint32_t* wlut0_data,
                          uint32_t* wlut1_data,
                          uint32_t* wlut2_data,
                          uint32_t* wlut3_data,
                          uint32_t* wlut4_data,
                          uint32_t* wlut5_data,
                          uint32_t* wlut6_data,
                          uint32_t* wlut7_data);
static void ued_wlut_check(UED_ADVANCED_REGS_t *ued_reg,
                           uint32_t* wlut0_data,
                           uint32_t* wlut1_data,
                           uint32_t* wlut2_data,
                           uint32_t* wlut3_data,
                           uint32_t* wlut4_data,
                           uint32_t* wlut5_data,
                           uint32_t* wlut6_data,
                           uint32_t* wlut7_data);
static void ued_wlut4_load(UED_ADVANCED_REGS_t *ued_reg,
                           uint32_t* wlut0_data,
                           uint32_t* wlut1_data,
                           uint32_t* wlut2_data,
                           uint32_t* wlut3_data);
static void ued_wlut4_check(UED_ADVANCED_REGS_t *ued_reg,
                            uint32_t* wlut0_data,
                            uint32_t* wlut1_data,
                            uint32_t* wlut2_data,
                            uint32_t* wlut3_data);


//********************************
// ued_cfg_driver - write registers
//********************************

void ued_cfg_driver(UED_ADVANCED_REGS_t *ued_reg, ued_cfg_t *ued_cfg)
{
     uint32_t reg_val;
 
     DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);
 
     // -------------
     // CFGA register
     // -------------
     // Read
     reg_val      = ued_reg->CFGA;
     // Modify
     reg_val      = UED_ADVANCED_CFGA_MODE_REPLACE_VAL      (reg_val, ued_cfg->mode                    );  // ED non-paired
     reg_val      = UED_ADVANCED_CFGA_BYPASS_REPLACE_VAL    (reg_val, ued_cfg->bypass                  );
     reg_val      = UED_ADVANCED_CFGA_SERP_WRAP_REPLACE_VAL (reg_val, ued_cfg->forward_error_serpentine);
     reg_val      = UED_ADVANCED_CFGA_S1_REPLACE_VAL        (reg_val, ued_cfg->s1                      );
     reg_val      = UED_ADVANCED_CFGA_BLINE_REPLACE_VAL     (reg_val, ued_cfg->bline                   );
     reg_val      = UED_ADVANCED_CFGA_CH_EN_REPLACE_VAL     (reg_val, (ued_cfg->ch_en    [5] << 5) |
                                                                   (ued_cfg->ch_en    [4] << 4) |
                                                                   (ued_cfg->ch_en    [3] << 3) |
                                                                   (ued_cfg->ch_en    [2] << 2) |
                                                                   (ued_cfg->ch_en    [1] << 1) |
                                                                   (ued_cfg->ch_en    [0] << 0));
     reg_val      = UED_ADVANCED_CFGA_CH_MAX_EN_REPLACE_VAL (reg_val, (ued_cfg->ch_max_en[5] << 5) |
                                                                   (ued_cfg->ch_max_en[4] << 4) |
                                                                   (ued_cfg->ch_max_en[3] << 3) |
                                                                   (ued_cfg->ch_max_en[2] << 2) |
                                                                   (ued_cfg->ch_max_en[1] << 1) |
                                                                   (ued_cfg->ch_max_en[0] << 0));
     reg_val      = UED_ADVANCED_CFGA_CH_MIN_EN_REPLACE_VAL (reg_val, (ued_cfg->ch_min_en[5] << 5) |
                                                                   (ued_cfg->ch_min_en[4] << 4) |
                                                                   (ued_cfg->ch_min_en[3] << 3) |
                                                                   (ued_cfg->ch_min_en[2] << 2) |
                                                                   (ued_cfg->ch_min_en[1] << 1) |
                                                                   (ued_cfg->ch_min_en[0] << 0));

    reg_val       = UED_ADVANCED_CFGA_CH_TDT_EN_REPLACE_VAL(reg_val, (ued_cfg->ch_tdt_en[5] << 5) |
                                                                  (ued_cfg->ch_tdt_en[4] << 4) |
                                                                  (ued_cfg->ch_tdt_en[3] << 3) |
                                                                  (ued_cfg->ch_tdt_en[2] << 2) |
                                                                  (ued_cfg->ch_tdt_en[1] << 1) |
                                                                  (ued_cfg->ch_tdt_en[0] << 0));

    reg_val       = UED_ADVANCED_CFGA_TH_PER_CH_REPLACE_VAL (reg_val, ued_cfg->th_per_ch);
    // Write
    ued_reg->CFGA = reg_val;

    // -------------
    // CFGB register
    // -------------
    reg_val      = ued_reg->CFGB;
    // values for CFGB are not yet run through the libconfuse config file, reset values are fine for now
    reg_val      = UED_ADVANCED_CFGB_S0_REPLACE_VAL(reg_val, ued_cfg->s0);
    reg_val      = UED_ADVANCED_CFGB_S3_REPLACE_VAL(reg_val, ued_cfg->s3);
    reg_val      = UED_ADVANCED_CFGB_S4_REPLACE_VAL(reg_val, ued_cfg->s4);
    reg_val      = UED_ADVANCED_CFGB_H2X_REPLACE_VAL(reg_val, ued_cfg->h2x);
    reg_val      = UED_ADVANCED_CFGB_BD_REPLACE_VAL(reg_val, ued_cfg->bd);
    reg_val      = UED_ADVANCED_CFGB_SV0_REPLACE_VAL(reg_val, ued_cfg->shift_value[0]);
    reg_val      = UED_ADVANCED_CFGB_SV1_REPLACE_VAL(reg_val, ued_cfg->shift_value[1]);
    reg_val      = UED_ADVANCED_CFGB_SV2_REPLACE_VAL(reg_val, ued_cfg->shift_value[2]);
    reg_val      = UED_ADVANCED_CFGB_SV3_REPLACE_VAL(reg_val, ued_cfg->shift_value[3]);
    reg_val      = UED_ADVANCED_CFGB_SV4_REPLACE_VAL(reg_val, ued_cfg->shift_value[4]);
    reg_val      = UED_ADVANCED_CFGB_SV5_REPLACE_VAL(reg_val, ued_cfg->shift_value[5]);
    ued_reg->CFGB = reg_val;

    // -------------
    // LW register
    // -------------
    reg_val    = ued_reg->LW;
    reg_val    = UED_ADVANCED_LW_WIDTH_REPLACE_VAL(reg_val, ued_cfg->up0_width);
    ued_reg->LW = reg_val;

    // -------------
    // L1 register
    // -------------
    reg_val    = ued_reg->L1;
    reg_val    = UED_ADVANCED_L1_MODE_REPLACE_VAL  (reg_val, ued_cfg->first_line_error_mode);
    reg_val    = UED_ADVANCED_L1_OFFSET_REPLACE_VAL(reg_val, ued_cfg->offset               );
    reg_val    = UED_ADVANCED_L1_SCALE1_REPLACE_VAL(reg_val, ued_cfg->scale1               );
    reg_val    = UED_ADVANCED_L1_SCALE2_REPLACE_VAL(reg_val, ued_cfg->scale2               );
    ued_reg->L1 = reg_val;

    // -------------
    // PSA register
    // -------------
    reg_val     = ued_reg->PSA;
    reg_val     =   UED_ADVANCED_PSA_EN0_REPLACE_VAL(reg_val, ued_cfg->ch_psa_en[0]);
    reg_val     =   UED_ADVANCED_PSA_EN1_REPLACE_VAL(reg_val, ued_cfg->ch_psa_en[1]);
    reg_val     =   UED_ADVANCED_PSA_EN2_REPLACE_VAL(reg_val, ued_cfg->ch_psa_en[2]);
    reg_val     =   UED_ADVANCED_PSA_EN3_REPLACE_VAL(reg_val, ued_cfg->ch_psa_en[3]);
    reg_val     =   UED_ADVANCED_PSA_EN4_REPLACE_VAL(reg_val, ued_cfg->ch_psa_en[4]);
    reg_val     =   UED_ADVANCED_PSA_EN5_REPLACE_VAL(reg_val, ued_cfg->ch_psa_en[5]);
    reg_val     =   UED_ADVANCED_PSA_BSEL0_REPLACE_VAL(reg_val, ued_cfg->psa_bsel0);
    reg_val     =   UED_ADVANCED_PSA_BSEL1_REPLACE_VAL(reg_val, ued_cfg->psa_bsel1);
    reg_val     =   UED_ADVANCED_PSA_CH34_SEL_REPLACE_VAL(reg_val, ued_cfg->psa_ch34_sel);
    reg_val     =   UED_ADVANCED_PSA_MAX_LEVEL_REPLACE_VAL(reg_val, ued_cfg->psa_max_level);
    reg_val     =   UED_ADVANCED_PSA_FILTER_EN_REPLACE_VAL(reg_val, ued_cfg->psa_filter_en);
    reg_val     =   UED_ADVANCED_PSA_BLK_PRIOR_REPLACE_VAL(reg_val, ued_cfg->psa_blk_prior);
    ued_reg->PSA = reg_val;

    // -------------
    // EWC register
    // -------------
    reg_val     = ued_reg->EWC;
    reg_val     = UED_ADVANCED_EWC_CH_TDW_EN_REPLACE_VAL (reg_val,(ued_cfg->ch_tdw_en[5] << 5) |
                                                       (ued_cfg->ch_tdw_en[4] << 4) |
                                                       (ued_cfg->ch_tdw_en[3] << 3) |
                                                       (ued_cfg->ch_tdw_en[2] << 2) |
                                                       (ued_cfg->ch_tdw_en[1] << 1) |
                                                       (ued_cfg->ch_tdw_en[0] << 0) );
    reg_val     = UED_ADVANCED_EWC_WEIGHT8_REPLACE_VAL   (reg_val, ued_cfg->weight8_en         );
    reg_val     = UED_ADVANCED_EWC_ERR_CRUMBS_REPLACE_VAL(reg_val, ued_cfg->err_crumbs         );
    reg_val     = UED_ADVANCED_EWC_RAND_CRUMBS_REPLACE_VAL(reg_val, ued_cfg->rand_crumbs         );
    reg_val     = UED_ADVANCED_EWC_ED_DISABLE_REPLACE_VAL(reg_val, ued_cfg->ed_disable         );
    ued_reg->EWC = reg_val;

    // -------------
    // EDC Register
    // -------------
    reg_val     = ued_reg->EDC;
    reg_val     = UED_ADVANCED_EDC_WH_EN_REPLACE_VAL(reg_val, ued_cfg->decay_wh_en);
    reg_val     = UED_ADVANCED_EDC_BK_EN_REPLACE_VAL(reg_val, ued_cfg->decay_bk_en);
    ued_reg->EDC = reg_val;

    // -------------
    // EDD1 Register
    // -------------
    reg_val     = ued_reg->EDD1;
    reg_val     = UED_ADVANCED_EDD1_W0_REPLACE_VAL(reg_val, ued_cfg->dweights[0]);
    reg_val     = UED_ADVANCED_EDD1_W1_REPLACE_VAL(reg_val, ued_cfg->dweights[1]);
    reg_val     = UED_ADVANCED_EDD1_W2_REPLACE_VAL(reg_val, ued_cfg->dweights[2]);
    reg_val     = UED_ADVANCED_EDD1_W3_REPLACE_VAL(reg_val, ued_cfg->dweights[3]);
    ued_reg->EDD1 = reg_val;

    // -------------
    // EDD2 Register
    // -------------
    reg_val     = ued_reg->EDD2;
    reg_val     = UED_ADVANCED_EDD2_W4_REPLACE_VAL(reg_val, ued_cfg->dweights[4]);
    reg_val     = UED_ADVANCED_EDD2_W5_REPLACE_VAL(reg_val, ued_cfg->dweights[5]);
    reg_val     = UED_ADVANCED_EDD2_W6_REPLACE_VAL(reg_val, ued_cfg->dweights[6]);
    reg_val     = UED_ADVANCED_EDD2_W7_REPLACE_VAL(reg_val, ued_cfg->dweights[7]);
    ued_reg->EDD2 = reg_val;

    // -------------
    // EWD1 register
    // -------------
    reg_val      = ued_reg->EWD1;
    reg_val      = UED_ADVANCED_EWD1_W0_REPLACE_VAL (reg_val, ued_cfg->weights[0]);
    reg_val      = UED_ADVANCED_EWD1_W1_REPLACE_VAL (reg_val, ued_cfg->weights[1]);
    reg_val      = UED_ADVANCED_EWD1_W2_REPLACE_VAL (reg_val, ued_cfg->weights[2]);
    reg_val      = UED_ADVANCED_EWD1_W3_REPLACE_VAL (reg_val, ued_cfg->weights[3]);
    ued_reg->EWD1 = reg_val;

    // -------------
    // EWD2 register
    // -------------
    reg_val      = ued_reg->EWD2;
    reg_val      = UED_ADVANCED_EWD2_W4_REPLACE_VAL (reg_val, ued_cfg->weights[4]);
    reg_val      = UED_ADVANCED_EWD2_W5_REPLACE_VAL (reg_val, ued_cfg->weights[5]);
    reg_val      = UED_ADVANCED_EWD2_W6_REPLACE_VAL (reg_val, ued_cfg->weights[6]);
    reg_val      = UED_ADVANCED_EWD2_W7_REPLACE_VAL (reg_val, ued_cfg->weights[7]);
    ued_reg->EWD2 = reg_val;

    // -------------
    // B1 register
    // -------------
    reg_val    = ued_reg->B1;
    reg_val    = UED_ADVANCED_B1_SBMODIFY_REPLACE_VAL (reg_val, ued_cfg->sbmodify           );
    reg_val    = UED_ADVANCED_B1_SBRGEN_REPLACE_VAL   (reg_val, ued_cfg->sbrgen             );
    reg_val    = UED_ADVANCED_B1_SBSELECT_REPLACE_VAL (reg_val, ued_cfg->sbselect           );
    reg_val    = UED_ADVANCED_B1_CH_TDB_EN_REPLACE_VAL(reg_val, (ued_cfg->ch_tdb_en[5] << 5) |
                                                             (ued_cfg->ch_tdb_en[4] << 4) |
                                                             (ued_cfg->ch_tdb_en[3] << 3) |
                                                             (ued_cfg->ch_tdb_en[2] << 2) |
                                                             (ued_cfg->ch_tdb_en[1] << 1) |
                                                             (ued_cfg->ch_tdb_en[0] << 0) );

    reg_val    = UED_ADVANCED_B1_CH_TDS_EN_REPLACE_VAL(reg_val, (ued_cfg->ch_tds_en[5] << 5) |
                                                             (ued_cfg->ch_tds_en[4] << 4) |
                                                             (ued_cfg->ch_tds_en[3] << 3) |
                                                             (ued_cfg->ch_tds_en[2] << 2) |
                                                             (ued_cfg->ch_tds_en[1] << 1) |
                                                             (ued_cfg->ch_tds_en[0] << 0) );

    ued_reg->B1 = reg_val;

    // -------------
    // B2 register
    // -------------
    reg_val    = ued_reg->B2;
    reg_val    = UED_ADVANCED_B2_LEVELS_REPLACE_VAL    (reg_val, ued_cfg->levels   );
    reg_val    = UED_ADVANCED_B2_LAST_LEVEL_REPLACE_VAL(reg_val, ued_cfg->lastlevel);
    reg_val    = UED_ADVANCED_B2_RANGE_REPLACE_VAL     (reg_val, ued_cfg->range    );
    reg_val    = UED_ADVANCED_B2_CLIFF1_REPLACE_VAL    (reg_val, ued_cfg->cliff1   );
    ued_reg->B2 = reg_val;

    // -------------
    // B3 register
    // -------------
    reg_val    = ued_reg->B3;
    reg_val    = UED_ADVANCED_B3_SBIAS1_REPLACE_VAL    (reg_val, ued_cfg->sbias1);
    reg_val    = UED_ADVANCED_B3_SBIAS2_REPLACE_VAL    (reg_val, ued_cfg->sbias2);
    ued_reg->B3 = reg_val;

    // -------------
    // BWS0 register
    // -------------
    reg_val    = ued_reg->BWS0;
    reg_val    = UED_ADVANCED_BWS0_HEIGHT_REPLACE_VAL(reg_val, ued_cfg->sbias_height[0]);
    reg_val    = UED_ADVANCED_BWS0_WIDTH_REPLACE_VAL(reg_val, ued_cfg->sbias_width[0]);
    ued_reg->BWS0 = reg_val;

    // -------------
    // BWS1 register
    // -------------
    reg_val    = ued_reg->BWS1;
    reg_val    = UED_ADVANCED_BWS1_HEIGHT_REPLACE_VAL(reg_val, ued_cfg->sbias_height[1]);
    reg_val    = UED_ADVANCED_BWS1_WIDTH_REPLACE_VAL(reg_val, ued_cfg->sbias_width[1]);
    ued_reg->BWS1 = reg_val;

    // -------------
    // BWS2 register
    // -------------
    reg_val    = ued_reg->BWS2;
    reg_val    = UED_ADVANCED_BWS2_HEIGHT_REPLACE_VAL(reg_val, ued_cfg->sbias_height[2]);
    reg_val    = UED_ADVANCED_BWS2_WIDTH_REPLACE_VAL(reg_val, ued_cfg->sbias_width[2]);
    ued_reg->BWS2 = reg_val;

    // -------------
    // BWS3 register
    // -------------
    reg_val    = ued_reg->BWS3;
    reg_val    = UED_ADVANCED_BWS3_HEIGHT_REPLACE_VAL(reg_val, ued_cfg->sbias_height[3]);
    reg_val    = UED_ADVANCED_BWS3_WIDTH_REPLACE_VAL(reg_val, ued_cfg->sbias_width[3]);
    ued_reg->BWS3 = reg_val;

    // -------------
    // BWS4 register
    // -------------
    reg_val    = ued_reg->BWS4;
    reg_val    = UED_ADVANCED_BWS4_HEIGHT_REPLACE_VAL(reg_val, ued_cfg->sbias_height[4]);
    reg_val    = UED_ADVANCED_BWS4_WIDTH_REPLACE_VAL(reg_val, ued_cfg->sbias_width[4]);
    ued_reg->BWS4 = reg_val;

    // -------------
    // BWS5 register
    // -------------
    reg_val    = ued_reg->BWS5;
    reg_val    = UED_ADVANCED_BWS5_HEIGHT_REPLACE_VAL(reg_val, ued_cfg->sbias_height[5]);
    reg_val    = UED_ADVANCED_BWS5_WIDTH_REPLACE_VAL(reg_val, ued_cfg->sbias_width[5]);
    ued_reg->BWS5 = reg_val;

    // -------------
    // LFSRS registers
    // -------------
    ued_reg->LFSRS0 = UED_ADVANCED_LFSRS0_RNUM_REPLACE_VAL (ued_reg->LFSRS0, ued_cfg->lfsr_ch_val[0]);
    ued_reg->LFSRS1 = UED_ADVANCED_LFSRS1_RNUM_REPLACE_VAL (ued_reg->LFSRS1, ued_cfg->lfsr_ch_val[1]);
    ued_reg->LFSRS2 = UED_ADVANCED_LFSRS2_RNUM_REPLACE_VAL (ued_reg->LFSRS2, ued_cfg->lfsr_ch_val[2]);
    ued_reg->LFSRS3 = UED_ADVANCED_LFSRS3_RNUM_REPLACE_VAL (ued_reg->LFSRS3, ued_cfg->lfsr_ch_val[3]);
    ued_reg->LFSRS4 = UED_ADVANCED_LFSRS4_RNUM_REPLACE_VAL (ued_reg->LFSRS4, ued_cfg->lfsr_ch_val[4]);
    ued_reg->LFSRS5 = UED_ADVANCED_LFSRS5_RNUM_REPLACE_VAL (ued_reg->LFSRS5, ued_cfg->lfsr_ch_val[5]);

    // -------------
    // THA0/THB0 registers
    // -------------
    reg_val      = ued_reg->THA0;
    reg_val      = UED_ADVANCED_THA0_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [0]);
    reg_val      = UED_ADVANCED_THA0_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [0]);
    reg_val      = UED_ADVANCED_THA0_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[0]);
    ued_reg->THA0 = reg_val;
	ued_reg->THB0 = UED_ADVANCED_THB0_DIFF_REPLACE_VAL(ued_reg->THB0, ued_cfg->diffusion[0]);

    // -------------
    // THA1/THB1 registers
    // -------------
    reg_val      = ued_reg->THA1;
    reg_val      = UED_ADVANCED_THA1_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [1]);
    reg_val      = UED_ADVANCED_THA1_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [1]);
    reg_val      = UED_ADVANCED_THA1_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[1]);
    ued_reg->THA1 = reg_val;
	ued_reg->THB1 = UED_ADVANCED_THB1_DIFF_REPLACE_VAL(ued_reg->THB1, ued_cfg->diffusion[1]);

    // -------------
    // THA2/THB2 registers
    // -------------
    reg_val      = ued_reg->THA2;
    reg_val      = UED_ADVANCED_THA2_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [2]);
    reg_val      = UED_ADVANCED_THA2_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [2]);
    reg_val      = UED_ADVANCED_THA2_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[2]);
    ued_reg->THA2 = reg_val;
	ued_reg->THB2 = UED_ADVANCED_THB2_DIFF_REPLACE_VAL(ued_reg->THB2, ued_cfg->diffusion[2]);

    // -------------
    // THA3/THB3 registers
    // -------------
    reg_val      = ued_reg->THA3;
    reg_val      = UED_ADVANCED_THA3_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [3]);
    reg_val      = UED_ADVANCED_THA3_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [3]);
    reg_val      = UED_ADVANCED_THA3_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[3]);
    ued_reg->THA3 = reg_val;
	ued_reg->THB3 = UED_ADVANCED_THB3_DIFF_REPLACE_VAL(ued_reg->THB3, ued_cfg->diffusion[3]);

    // -------------
    // THA4/THB4 registers
    // -------------
    reg_val      = ued_reg->THA4;
    reg_val      = UED_ADVANCED_THA4_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [4]);
    reg_val      = UED_ADVANCED_THA4_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [4]);
    reg_val      = UED_ADVANCED_THA4_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[4]);
    ued_reg->THA4 = reg_val;
	ued_reg->THB4 = UED_ADVANCED_THB4_DIFF_REPLACE_VAL(ued_reg->THB4, ued_cfg->diffusion[4]);

    // -------------
    // THA5/THB5 registers
    // -------------
    reg_val      = ued_reg->THA5;
    reg_val      = UED_ADVANCED_THA5_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [5]);
    reg_val      = UED_ADVANCED_THA5_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [5]);
    reg_val      = UED_ADVANCED_THA5_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[5]);
    ued_reg->THA5 = reg_val;
	ued_reg->THB5 = UED_ADVANCED_THB5_DIFF_REPLACE_VAL(ued_reg->THB5, ued_cfg->diffusion[5]);

    // -------------
    // THA6/THB6 registers
    // -------------
    reg_val      = ued_reg->THA6;
    reg_val      = UED_ADVANCED_THA6_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [6]);
    reg_val      = UED_ADVANCED_THA6_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [6]);
    reg_val      = UED_ADVANCED_THA6_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[6]);
    ued_reg->THA6 = reg_val;
	ued_reg->THB6 = UED_ADVANCED_THB6_DIFF_REPLACE_VAL(ued_reg->THB6, ued_cfg->diffusion[6]);

    // -------------
    // THA7/THB7 registers
    // -------------
    reg_val      = ued_reg->THA7;
    reg_val      = UED_ADVANCED_THA7_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [7]);
    reg_val      = UED_ADVANCED_THA7_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [7]);
    reg_val      = UED_ADVANCED_THA7_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[7]);
    ued_reg->THA7 = reg_val;
	ued_reg->THB7 = UED_ADVANCED_THB7_DIFF_REPLACE_VAL(ued_reg->THB7, ued_cfg->diffusion[7]);

    // -------------
    // THA8/THB8 registers
    // -------------
    reg_val      = ued_reg->THA8;
    reg_val      = UED_ADVANCED_THA8_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [8]);
    reg_val      = UED_ADVANCED_THA8_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [8]);
    reg_val      = UED_ADVANCED_THA8_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[8]);
    ued_reg->THA8 = reg_val;
	ued_reg->THB8 = UED_ADVANCED_THB8_DIFF_REPLACE_VAL(ued_reg->THB8, ued_cfg->diffusion[8]);

    // -------------
    // THA9/THB9 registers
    // -------------
    reg_val      = ued_reg->THA9;
    reg_val      = UED_ADVANCED_THA9_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [9]);
    reg_val      = UED_ADVANCED_THA9_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [9]);
    reg_val      = UED_ADVANCED_THA9_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[9]);
    ued_reg->THA9 = reg_val;
	ued_reg->THB9 = UED_ADVANCED_THB9_DIFF_REPLACE_VAL(ued_reg->THB9, ued_cfg->diffusion[9]);

    // -------------
    // THA10/THB10 registers
    // -------------
    reg_val       = ued_reg->THA10;
    reg_val       = UED_ADVANCED_THA10_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [10]);
    reg_val       = UED_ADVANCED_THA10_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [10]);
    reg_val       = UED_ADVANCED_THA10_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[10]);
    ued_reg->THA10 = reg_val;
    ued_reg->THB10 = UED_ADVANCED_THB10_DIFF_REPLACE_VAL(ued_reg->THB10, ued_cfg->diffusion[10]);

    // -------------
    // THA11/THB11 registers
    // -------------
    reg_val       = ued_reg->THA11;
    reg_val       = UED_ADVANCED_THA11_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [11]);
    reg_val       = UED_ADVANCED_THA11_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [11]);
    reg_val       = UED_ADVANCED_THA11_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[11]);
    ued_reg->THA11 = reg_val;
    ued_reg->THB11 = UED_ADVANCED_THB11_DIFF_REPLACE_VAL(ued_reg->THB11, ued_cfg->diffusion[11]);

    // -------------
    // THA12/THB12 registers
    // -------------
    reg_val       = ued_reg->THA12;
    reg_val       = UED_ADVANCED_THA12_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [12]);
    reg_val       = UED_ADVANCED_THA12_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [12]);
    reg_val       = UED_ADVANCED_THA12_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[12]);
    ued_reg->THA12 = reg_val;
    ued_reg->THB12 = UED_ADVANCED_THB12_DIFF_REPLACE_VAL(ued_reg->THB12, ued_cfg->diffusion[12]);

    // -------------
    // THA13/THB13 registers
    // -------------
    reg_val       = ued_reg->THA13;
    reg_val       = UED_ADVANCED_THA13_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [13]);
    reg_val       = UED_ADVANCED_THA13_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [13]);
    reg_val       = UED_ADVANCED_THA13_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[13]);
    ued_reg->THA13 = reg_val;
    ued_reg->THB13 = UED_ADVANCED_THB13_DIFF_REPLACE_VAL(ued_reg->THB13, ued_cfg->diffusion[13]);

    // -------------
    // THA14/THB14 registers
    // -------------
    reg_val       = ued_reg->THA14;
    reg_val       = UED_ADVANCED_THA14_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [14]);
    reg_val       = UED_ADVANCED_THA14_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [14]);
    reg_val       = UED_ADVANCED_THA14_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[14]);
    ued_reg->THA14 = reg_val;
    ued_reg->THB14 = UED_ADVANCED_THB14_DIFF_REPLACE_VAL(ued_reg->THB14, ued_cfg->diffusion[14]);

    // -------------
    // THA15/THB15 registers
    // -------------
    reg_val       = ued_reg->THA15;
    reg_val       = UED_ADVANCED_THA15_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [15]);
    reg_val       = UED_ADVANCED_THA15_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [15]);
    reg_val       = UED_ADVANCED_THA15_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[15]);
    ued_reg->THA15 = reg_val;
    ued_reg->THB15 = UED_ADVANCED_THB15_DIFF_REPLACE_VAL(ued_reg->THB15, ued_cfg->diffusion[15]);

    // -------------
    // THA16/THB16 registers
    // -------------
    reg_val       = ued_reg->THA16;
    reg_val       = UED_ADVANCED_THA16_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [16]);
    reg_val       = UED_ADVANCED_THA16_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [16]);
    reg_val       = UED_ADVANCED_THA16_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[16]);
    ued_reg->THA16 = reg_val;
    ued_reg->THB16 = UED_ADVANCED_THB16_DIFF_REPLACE_VAL(ued_reg->THB16, ued_cfg->diffusion[16]);

    // -------------
    // THA17/THB17 registers
    // -------------
    reg_val       = ued_reg->THA17;
    reg_val       = UED_ADVANCED_THA17_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [17]);
    reg_val       = UED_ADVANCED_THA17_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [17]);
    reg_val       = UED_ADVANCED_THA17_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[17]);
    ued_reg->THA17 = reg_val;
    ued_reg->THB17 = UED_ADVANCED_THB17_DIFF_REPLACE_VAL(ued_reg->THB17, ued_cfg->diffusion[17]);

    // -------------
    // THA18/THB18 registers
    // -------------
    reg_val       = ued_reg->THA18;
    reg_val       = UED_ADVANCED_THA18_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [18]);
    reg_val       = UED_ADVANCED_THA18_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [18]);
    reg_val       = UED_ADVANCED_THA18_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[18]);
    ued_reg->THA18 = reg_val;
    ued_reg->THB18 = UED_ADVANCED_THB18_DIFF_REPLACE_VAL(ued_reg->THB18, ued_cfg->diffusion[18]);

    // -------------
    // THA19/THB19 registers
    // -------------
    reg_val       = ued_reg->THA19;
    reg_val       = UED_ADVANCED_THA19_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [19]);
    reg_val       = UED_ADVANCED_THA19_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [19]);
    reg_val       = UED_ADVANCED_THA19_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[19]);
    ued_reg->THA19 = reg_val;
    ued_reg->THB19 = UED_ADVANCED_THB19_DIFF_REPLACE_VAL(ued_reg->THB19, ued_cfg->diffusion[19]);

    // -------------
    // THA20/THB20 registers
    // -------------
    reg_val       = ued_reg->THA20;
    reg_val       = UED_ADVANCED_THA20_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [20]);
    reg_val       = UED_ADVANCED_THA20_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [20]);
    reg_val       = UED_ADVANCED_THA20_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[20]);
    ued_reg->THA20 = reg_val;
    ued_reg->THB20 = UED_ADVANCED_THB20_DIFF_REPLACE_VAL(ued_reg->THB20, ued_cfg->diffusion[20]);

    // -------------
    // THA21/THB21 registers
    // -------------
    reg_val       = ued_reg->THA21;
    reg_val       = UED_ADVANCED_THA21_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [21]);
    reg_val       = UED_ADVANCED_THA21_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [21]);
    reg_val       = UED_ADVANCED_THA21_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[21]);
    ued_reg->THA21 = reg_val;
    ued_reg->THB21 = UED_ADVANCED_THB21_DIFF_REPLACE_VAL(ued_reg->THB21, ued_cfg->diffusion[21]);

    // -------------
    // THA22/THB22 registers
    // -------------
    reg_val       = ued_reg->THA22;
    reg_val       = UED_ADVANCED_THA22_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [22]);
    reg_val       = UED_ADVANCED_THA22_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [22]);
    reg_val       = UED_ADVANCED_THA22_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[22]);
    ued_reg->THA22 = reg_val;
    ued_reg->THB22 = UED_ADVANCED_THB22_DIFF_REPLACE_VAL(ued_reg->THB22, ued_cfg->diffusion[22]);

    // -------------
    // THA23/THB23 registers
    // -------------
    reg_val       = ued_reg->THA23;
    reg_val       = UED_ADVANCED_THA23_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [23]);
    reg_val       = UED_ADVANCED_THA23_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [23]);
    reg_val       = UED_ADVANCED_THA23_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[23]);
    ued_reg->THA23 = reg_val;
    ued_reg->THB23 = UED_ADVANCED_THB23_DIFF_REPLACE_VAL(ued_reg->THB23, ued_cfg->diffusion[23]);

    // -------------
    // THA24/THB24 registers
    // -------------
    reg_val       = ued_reg->THA24;
    reg_val       = UED_ADVANCED_THA24_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [24]);
    reg_val       = UED_ADVANCED_THA24_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [24]);
    reg_val       = UED_ADVANCED_THA24_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[24]);
    ued_reg->THA24 = reg_val;
    ued_reg->THB24 = UED_ADVANCED_THB24_DIFF_REPLACE_VAL(ued_reg->THB24, ued_cfg->diffusion[24]);

    // -------------
    // THA25/THB25 registers
    // -------------
    reg_val       = ued_reg->THA25;
    reg_val       = UED_ADVANCED_THA25_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [25]);
    reg_val       = UED_ADVANCED_THA25_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [25]);
    reg_val       = UED_ADVANCED_THA25_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[25]);
    ued_reg->THA25 = reg_val;
    ued_reg->THB25 = UED_ADVANCED_THB25_DIFF_REPLACE_VAL(ued_reg->THB25, ued_cfg->diffusion[25]);

    // -------------
    // THA26/THB26 registers
    // -------------
    reg_val       = ued_reg->THA26;
    reg_val       = UED_ADVANCED_THA26_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [26]);
    reg_val       = UED_ADVANCED_THA26_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [26]);
    reg_val       = UED_ADVANCED_THA26_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[26]);
    ued_reg->THA26 = reg_val;
    ued_reg->THB26 = UED_ADVANCED_THB26_DIFF_REPLACE_VAL(ued_reg->THB26, ued_cfg->diffusion[26]);

    // -------------
    // THA27/THB27 registers
    // -------------
    reg_val       = ued_reg->THA27;
    reg_val       = UED_ADVANCED_THA27_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [27]);
    reg_val       = UED_ADVANCED_THA27_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [27]);
    reg_val       = UED_ADVANCED_THA27_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[27]);
    ued_reg->THA27 = reg_val;
    ued_reg->THB27 = UED_ADVANCED_THB27_DIFF_REPLACE_VAL(ued_reg->THB27, ued_cfg->diffusion[27]);

    // -------------
    // THA28/THB28 registers
    // -------------
    reg_val       = ued_reg->THA28;
    reg_val       = UED_ADVANCED_THA28_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [28]);
    reg_val       = UED_ADVANCED_THA28_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [28]);
    reg_val       = UED_ADVANCED_THA28_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[28]);
    ued_reg->THA28 = reg_val;
    ued_reg->THB28 = UED_ADVANCED_THB28_DIFF_REPLACE_VAL(ued_reg->THB28, ued_cfg->diffusion[28]);

    // -------------
    // THA29/THB29 registers
    // -------------
    reg_val       = ued_reg->THA29;
    reg_val       = UED_ADVANCED_THA29_DENS_REPLACE_VAL (reg_val, ued_cfg->density  [29]);
    reg_val       = UED_ADVANCED_THA29_SIZE_REPLACE_VAL (reg_val, ued_cfg->size     [29]);
    reg_val       = UED_ADVANCED_THA29_COMP_REPLACE_VAL (reg_val, ued_cfg->threshold[29]);
    ued_reg->THA29 = reg_val;
    ued_reg->THB29 = UED_ADVANCED_THB29_DIFF_REPLACE_VAL(ued_reg->THB29, ued_cfg->diffusion[29]);

    // -------------
    // THP0 register
    // -------------
    reg_val       = ued_reg->THP0;
    reg_val       = UED_ADVANCED_THP0_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[0]);
    reg_val       = UED_ADVANCED_THP0_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[0]);
    ued_reg->THP0 = reg_val;

    // -------------
    // THP1 register
    // -------------
    reg_val       = ued_reg->THP1;
    reg_val       = UED_ADVANCED_THP1_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[1]);
    reg_val       = UED_ADVANCED_THP1_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[1]);
    ued_reg->THP1 = reg_val;

    // -------------
    // THP2 register
    // -------------
    reg_val       = ued_reg->THP2;
    reg_val       = UED_ADVANCED_THP2_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[2]);
    reg_val       = UED_ADVANCED_THP2_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[2]);
    ued_reg->THP2 = reg_val;

    // -------------
    // THP3 register
    // -------------
    reg_val       = ued_reg->THP3;
    reg_val       = UED_ADVANCED_THP3_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[3]);
    reg_val       = UED_ADVANCED_THP3_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[3]);
    ued_reg->THP3 = reg_val;

    // -------------
    // THP4 register
    // -------------
    reg_val       = ued_reg->THP4;
    reg_val       = UED_ADVANCED_THP4_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[4]);
    reg_val       = UED_ADVANCED_THP4_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[4]);
    ued_reg->THP4 = reg_val;

    // -------------
    // THP5 register
    // -------------
    reg_val       = ued_reg->THP5;
    reg_val       = UED_ADVANCED_THP5_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[5]);
    reg_val       = UED_ADVANCED_THP5_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[5]);
    ued_reg->THP5 = reg_val;

    // -------------
    // THP6 register
    // -------------
    reg_val       = ued_reg->THP6;
    reg_val       = UED_ADVANCED_THP6_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[6]);
    reg_val       = UED_ADVANCED_THP6_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[6]);
    ued_reg->THP6 = reg_val;

    // -------------
    // THP7 register
    // -------------
    reg_val       = ued_reg->THP7;
    reg_val       = UED_ADVANCED_THP7_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[7]);
    reg_val       = UED_ADVANCED_THP7_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[7]);
    ued_reg->THP7 = reg_val;

    // -------------
    // THP8 register
    // -------------
    reg_val       = ued_reg->THP8;
    reg_val       = UED_ADVANCED_THP8_COMP_REPLACE_VAL(reg_val, ued_cfg->psathresh[8]);
    reg_val       = UED_ADVANCED_THP8_SIZE_REPLACE_VAL(reg_val, ued_cfg->psasize[8]);
    ued_reg->THP8 = reg_val;
}
  
//********************************
// ued_setup_luts - program the LUTs
//********************************
void ued_setup_luts(UED_ADVANCED_REGS_t *ued_reg, ued_lut_data_t *lut_data)
{ 
    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // load 'em
    ued_blut_load(ued_reg,
                  lut_data->blut->blut0_data,
                  lut_data->blut->blut1_data,
                  lut_data->blut->blut2_data,
                  lut_data->blut->blut3_data,
                  lut_data->blut->blut4_data,
                  lut_data->blut->blut5_data);
    ued_clut_load(ued_reg,
                  lut_data->clut->clut_bias0_data,
                  lut_data->clut->clut_thresh0_data,
                  lut_data->clut->clut_bias1_data,
                  lut_data->clut->clut_thresh1_data,
                  lut_data->clut->clut_bias2_data,
                  lut_data->clut->clut_thresh2_data,
                  lut_data->clut->clut_bias3_data,
                  lut_data->clut->clut_thresh3_data,
                  lut_data->clut->clut_bias4_data,
                  lut_data->clut->clut_thresh4_data,
                  lut_data->clut->clut_bias5_data,
                  lut_data->clut->clut_thresh5_data);

    ued_wlut_load (ued_reg,
                   lut_data->wlut->wlut0_data,
                   lut_data->wlut->wlut1_data,
                   lut_data->wlut->wlut2_data,
                   lut_data->wlut->wlut3_data,
                   lut_data->wlut->wlut4_data,
                   lut_data->wlut->wlut5_data,
                   lut_data->wlut->wlut6_data,
                   lut_data->wlut->wlut7_data);

    // check 'em

    ued_blut_check(ued_reg,
                   lut_data->blut->blut0_data,
                   lut_data->blut->blut1_data,
                   lut_data->blut->blut2_data,
                   lut_data->blut->blut3_data,
                   lut_data->blut->blut4_data,
                   lut_data->blut->blut5_data);
    ued_clut_check(ued_reg,
                   lut_data->clut->clut_bias0_data,
                   lut_data->clut->clut_thresh0_data,
                   lut_data->clut->clut_bias1_data,
                   lut_data->clut->clut_thresh1_data,
                   lut_data->clut->clut_bias2_data,
                   lut_data->clut->clut_thresh2_data,
                   lut_data->clut->clut_bias3_data,
                   lut_data->clut->clut_thresh3_data,
                   lut_data->clut->clut_bias4_data,
                   lut_data->clut->clut_thresh4_data,
                   lut_data->clut->clut_bias5_data,
                   lut_data->clut->clut_thresh5_data);

    ued_wlut_check(ued_reg,
                   lut_data->wlut->wlut0_data,
                   lut_data->wlut->wlut1_data,
                   lut_data->wlut->wlut2_data,
                   lut_data->wlut->wlut3_data,
                   lut_data->wlut->wlut4_data,
                   lut_data->wlut->wlut5_data,
                   lut_data->wlut->wlut6_data,
                   lut_data->wlut->wlut7_data);

}

//********************************
// ued_setup_luts_wlut4 - program the LUTs with 4 entry wlut mode
//********************************
void ued_setup_luts_wlut4(UED_ADVANCED_REGS_t *ued_reg, ued_lut_data_t *lut_data)
{ 
    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // load 'em
    ued_blut_load(ued_reg,
                  lut_data->blut->blut0_data,
                  lut_data->blut->blut1_data,
                  lut_data->blut->blut2_data,
                  lut_data->blut->blut3_data,
                  lut_data->blut->blut4_data,
                  lut_data->blut->blut5_data);
    ued_clut_load(ued_reg,
                  lut_data->clut->clut_bias0_data,
                  lut_data->clut->clut_thresh0_data,
                  lut_data->clut->clut_bias1_data,
                  lut_data->clut->clut_thresh1_data,
                  lut_data->clut->clut_bias2_data,
                  lut_data->clut->clut_thresh2_data,
                  lut_data->clut->clut_bias3_data,
                  lut_data->clut->clut_thresh3_data,
                  lut_data->clut->clut_bias4_data,
                  lut_data->clut->clut_thresh4_data,
                  lut_data->clut->clut_bias5_data,
                  lut_data->clut->clut_thresh5_data);
    ued_wlut4_load(ued_reg,
                   lut_data->wlut->wlut0_data,
                   lut_data->wlut->wlut1_data,
                   lut_data->wlut->wlut2_data,
                   lut_data->wlut->wlut3_data);

    // check 'em
    ued_blut_check(ued_reg,
                   lut_data->blut->blut0_data,
                   lut_data->blut->blut1_data,
                   lut_data->blut->blut2_data,
                   lut_data->blut->blut3_data,
                   lut_data->blut->blut4_data,
                   lut_data->blut->blut5_data);

    ued_clut_check(ued_reg,
                   lut_data->clut->clut_bias0_data,
                   lut_data->clut->clut_thresh0_data,
                   lut_data->clut->clut_bias1_data,
                   lut_data->clut->clut_thresh1_data,
                   lut_data->clut->clut_bias2_data,
                   lut_data->clut->clut_thresh2_data,
                   lut_data->clut->clut_bias3_data,
                   lut_data->clut->clut_thresh3_data,
                   lut_data->clut->clut_bias4_data,
                   lut_data->clut->clut_thresh4_data,
                   lut_data->clut->clut_bias5_data,
                   lut_data->clut->clut_thresh5_data);

    ued_wlut4_check(ued_reg,
                    lut_data->wlut->wlut0_data,
                    lut_data->wlut->wlut1_data,
                    lut_data->wlut->wlut2_data,
                    lut_data->wlut->wlut3_data);
}

//********************************
// ued_cpu_sram_access - needs set for CPU to access LUTs
//********************************
static void ued_cpu_sram_access(UED_ADVANCED_REGS_t *ued_reg,
                                uint32_t bit)
{
    uint32_t reg_val;

    reg_val      = ued_reg->CFGA;
    reg_val      = UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_REPLACE_VAL(reg_val, bit);
    ued_reg->CFGA = reg_val;
}

//********************************
// ued_blut_load
//********************************
static void ued_blut_load(UED_ADVANCED_REGS_t *ued_reg,
                          uint32_t* blut0_data,
                          uint32_t* blut1_data,
                          uint32_t* blut2_data,
                          uint32_t* blut3_data,
                          uint32_t* blut4_data,
                          uint32_t* blut5_data)
{
    uint32_t i;

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // first set the CPU SRAM access bit
    ued_cpu_sram_access(ued_reg,1); 

    // blast the data into the LUT
    // The incoming data form the SREC is in 16-bit sign extended form, even though
    // it is really only 10-bits, so have to mask it first (0x3ff).

    ued_reg->BLAC0 = UED_ADVANCED_BLAC0_INCA_REPLACE_VAL(ued_reg->BLAC0, 1);
    ued_reg->BLAC0 = UED_ADVANCED_BLAC0_MODE_REPLACE_VAL(ued_reg->BLAC0, 0);
    ued_reg->BLAC1 = UED_ADVANCED_BLAC1_INCA_REPLACE_VAL(ued_reg->BLAC1, 1);
    ued_reg->BLAC1 = UED_ADVANCED_BLAC1_MODE_REPLACE_VAL(ued_reg->BLAC1, 0);
    ued_reg->BLAC2 = UED_ADVANCED_BLAC2_INCA_REPLACE_VAL(ued_reg->BLAC2, 1);
    ued_reg->BLAC2 = UED_ADVANCED_BLAC2_MODE_REPLACE_VAL(ued_reg->BLAC2, 0);
    ued_reg->BLAC3 = UED_ADVANCED_BLAC3_INCA_REPLACE_VAL(ued_reg->BLAC3, 1);
    ued_reg->BLAC3 = UED_ADVANCED_BLAC3_MODE_REPLACE_VAL(ued_reg->BLAC3, 0);
    ued_reg->BLAC4 = UED_ADVANCED_BLAC4_INCA_REPLACE_VAL(ued_reg->BLAC4, 1);
    ued_reg->BLAC4 = UED_ADVANCED_BLAC4_MODE_REPLACE_VAL(ued_reg->BLAC4, 0);
    ued_reg->BLAC5 = UED_ADVANCED_BLAC5_INCA_REPLACE_VAL(ued_reg->BLAC5, 1);
    ued_reg->BLAC5 = UED_ADVANCED_BLAC5_MODE_REPLACE_VAL(ued_reg->BLAC5, 0);

    ued_reg->BLAA0 = 0x0;
    ued_reg->BLAA1 = 0x0;
    ued_reg->BLAA2 = 0x0;
    ued_reg->BLAA3 = 0x0;
    ued_reg->BLAA4 = 0x0;
    ued_reg->BLAA5 = 0x0;

    for(i=0; i<UED_BLUT_DEPTH; i++)
    {
        ued_reg->BLAD0 = UED_ADVANCED_BLAD0_DATA_REPLACE_VAL(ued_reg->BLAD0, (blut0_data[i] & 0x3FF));
        ued_reg->BLAD1 = UED_ADVANCED_BLAD1_DATA_REPLACE_VAL(ued_reg->BLAD1, (blut1_data[i] & 0x3FF));
        ued_reg->BLAD2 = UED_ADVANCED_BLAD2_DATA_REPLACE_VAL(ued_reg->BLAD2, (blut2_data[i] & 0x3FF));
        ued_reg->BLAD3 = UED_ADVANCED_BLAD3_DATA_REPLACE_VAL(ued_reg->BLAD3, (blut3_data[i] & 0x3FF));
        ued_reg->BLAD4 = UED_ADVANCED_BLAD4_DATA_REPLACE_VAL(ued_reg->BLAD4, (blut4_data[i] & 0x3FF));
        ued_reg->BLAD5 = UED_ADVANCED_BLAD5_DATA_REPLACE_VAL(ued_reg->BLAD5, (blut5_data[i] & 0x3FF));
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0); 
}

//********************************
// ued_blut_check
//********************************
static void ued_blut_check(UED_ADVANCED_REGS_t *ued_reg,
                           uint32_t* blut0_data,
                           uint32_t* blut1_data,
                           uint32_t* blut2_data,
                           uint32_t* blut3_data,
                           uint32_t* blut4_data,
                           uint32_t* blut5_data)
{
    uint32_t blut0_aval, blut0_eval;	// aval = actual value, eval = expected value
    uint32_t blut1_aval, blut1_eval;
    uint32_t blut2_aval, blut2_eval;
    uint32_t blut3_aval, blut3_eval;
    uint32_t blut4_aval, blut4_eval;
    uint32_t blut5_aval, blut5_eval;
    //  uint32_t lut_val;
    uint32_t i;
    volatile uint32_t temp;

    // first set the LUT access bits
    ued_cpu_sram_access(ued_reg,1); 

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    ued_reg->BLAC0 = UED_ADVANCED_BLAC0_INCA_REPLACE_VAL(ued_reg->BLAC0, 0);
    ued_reg->BLAC0 = UED_ADVANCED_BLAC0_MODE_REPLACE_VAL(ued_reg->BLAC0, 1);
    ued_reg->BLAC1 = UED_ADVANCED_BLAC1_INCA_REPLACE_VAL(ued_reg->BLAC1, 0);
    ued_reg->BLAC1 = UED_ADVANCED_BLAC1_MODE_REPLACE_VAL(ued_reg->BLAC1, 1);
    ued_reg->BLAC2 = UED_ADVANCED_BLAC2_INCA_REPLACE_VAL(ued_reg->BLAC2, 0);
    ued_reg->BLAC2 = UED_ADVANCED_BLAC2_MODE_REPLACE_VAL(ued_reg->BLAC2, 1);
    ued_reg->BLAC3 = UED_ADVANCED_BLAC3_INCA_REPLACE_VAL(ued_reg->BLAC3, 0);
    ued_reg->BLAC3 = UED_ADVANCED_BLAC3_MODE_REPLACE_VAL(ued_reg->BLAC3, 1);
    ued_reg->BLAC4 = UED_ADVANCED_BLAC4_INCA_REPLACE_VAL(ued_reg->BLAC4, 0);
    ued_reg->BLAC4 = UED_ADVANCED_BLAC4_MODE_REPLACE_VAL(ued_reg->BLAC4, 1);
    ued_reg->BLAC5 = UED_ADVANCED_BLAC5_INCA_REPLACE_VAL(ued_reg->BLAC5, 0);
    ued_reg->BLAC5 = UED_ADVANCED_BLAC5_MODE_REPLACE_VAL(ued_reg->BLAC5, 1);

    // read and compare the LUT data
    for(i=0; i<UED_BLUT_DEPTH; i++)
    {
        ued_reg->BLAA0 = i;
        ued_reg->BLAA1 = i;
        ued_reg->BLAA2 = i;
        ued_reg->BLAA3 = i;
        ued_reg->BLAA4 = i;
        ued_reg->BLAA5 = i;

        // Need to read values twice on RevB0 for this check.
        temp = ued_reg->BLAD0;
        blut0_aval = ued_reg->BLAD0;
        temp = ued_reg->BLAD1;
        blut1_aval = ued_reg->BLAD1;
        temp = ued_reg->BLAD2;
        blut2_aval = ued_reg->BLAD2;
        temp = ued_reg->BLAD3;
        blut3_aval = ued_reg->BLAD3;
        temp = ued_reg->BLAD4;
        blut4_aval = ued_reg->BLAD4;
        temp = ued_reg->BLAD5;
        blut5_aval = ued_reg->BLAD5;

        blut0_eval = blut0_data[i] & (UED_ADVANCED_BLAD0_DATA_MASK >> UED_ADVANCED_BLAD0_DATA_SHIFT);
        blut1_eval = blut1_data[i] & (UED_ADVANCED_BLAD1_DATA_MASK >> UED_ADVANCED_BLAD1_DATA_SHIFT);
        blut2_eval = blut2_data[i] & (UED_ADVANCED_BLAD2_DATA_MASK >> UED_ADVANCED_BLAD2_DATA_SHIFT);
        blut3_eval = blut3_data[i] & (UED_ADVANCED_BLAD3_DATA_MASK >> UED_ADVANCED_BLAD3_DATA_SHIFT);
        blut4_eval = blut4_data[i] & (UED_ADVANCED_BLAD4_DATA_MASK >> UED_ADVANCED_BLAD4_DATA_SHIFT);
        blut5_eval = blut5_data[i] & (UED_ADVANCED_BLAD5_DATA_MASK >> UED_ADVANCED_BLAD5_DATA_SHIFT);

        if(blut0_aval != blut0_eval) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect blut0 data at Addr=0x%8x, Act=0x%3x, Exp=0x%3x\n\r", __FUNCTION__, ued_reg->BLAA0,           blut0_aval, blut0_eval);
        }
        if(blut1_aval != blut1_eval) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect blut1 data at Addr=0x%8x, Act=0x%3x, Exp=0x%3x\n\r", __FUNCTION__, ued_reg->BLAA1,           blut1_aval, blut1_eval);
        }
        if(blut2_aval != blut2_eval) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect blut2 data at Addr=0x%8x, Act=0x%3x, Exp=0x%3x\n\r", __FUNCTION__, ued_reg->BLAA2,           blut2_aval, blut2_eval);
        }
        if(blut3_aval != blut3_eval) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect blut3 data at Addr=0x%8x, Act=0x%3x, Exp=0x%3x\n\r", __FUNCTION__, ued_reg->BLAA3,           blut3_aval, blut3_eval);
        }
        if(blut4_aval != blut4_eval) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect blut4 data at Addr=0x%8x, Act=0x%3x, Exp=0x%3x\n\r", __FUNCTION__, ued_reg->BLAA4,           blut4_aval, blut4_eval);
        }
        if(blut5_aval != blut5_eval) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect blut5 data at Addr=0x%8x, Act=0x%3x, Exp=0x%3x\n\r", __FUNCTION__, ued_reg->BLAA5,           blut5_aval, blut5_eval);
        }
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0);
}

//********************************
// ued_clut_load
//********************************
static void ued_clut_load(UED_ADVANCED_REGS_t *ued_reg,
                          uint8_t* clut_bias0_data,
                          uint16_t* clut_thresh0_data,
                          uint8_t* clut_bias1_data,
                          uint16_t* clut_thresh1_data,
                          uint8_t* clut_bias2_data,
                          uint16_t* clut_thresh2_data,
                          uint8_t* clut_bias3_data,
                          uint16_t* clut_thresh3_data,
                          uint8_t* clut_bias4_data,
                          uint16_t* clut_thresh4_data,
                          uint8_t* clut_bias5_data,
                          uint16_t* clut_thresh5_data)
{
    uint32_t i;
    uint32_t reg_val;

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // first set the CPU SRAM access bit
    ued_cpu_sram_access(ued_reg,1); 

    // blast the data into the LUT
    for(i=0; i<UED_CLUT_DEPTH; i++)
    {
	    reg_val = 0;
        reg_val = UED_ADVANCED_CLUT0_BIAS_REPLACE_VAL(reg_val, clut_bias0_data[i]);
        reg_val = UED_ADVANCED_CLUT0_THRESH_REPLACE_VAL(reg_val, (clut_thresh0_data[i] & 0x3FF));
        ued_reg->CLUT0[i] = reg_val;
	    reg_val = 0;
        reg_val = UED_ADVANCED_CLUT1_BIAS_REPLACE_VAL(reg_val, clut_bias1_data[i]);
        reg_val = UED_ADVANCED_CLUT1_THRESH_REPLACE_VAL(reg_val, (clut_thresh1_data[i] & 0x3FF));
        ued_reg->CLUT1[i] = reg_val;
	    reg_val = 0;
        reg_val = UED_ADVANCED_CLUT2_BIAS_REPLACE_VAL(reg_val, clut_bias2_data[i]);
        reg_val = UED_ADVANCED_CLUT2_THRESH_REPLACE_VAL(reg_val, (clut_thresh2_data[i] & 0x3FF));
        ued_reg->CLUT2[i] = reg_val;
	    reg_val = 0;
        reg_val = UED_ADVANCED_CLUT3_BIAS_REPLACE_VAL(reg_val, clut_bias3_data[i]);
        reg_val = UED_ADVANCED_CLUT3_THRESH_REPLACE_VAL(reg_val, (clut_thresh3_data[i] & 0x3FF));
        ued_reg->CLUT3[i] = reg_val;
	    reg_val = 0;
        reg_val = UED_ADVANCED_CLUT4_BIAS_REPLACE_VAL(reg_val, clut_bias4_data[i]);
        reg_val = UED_ADVANCED_CLUT4_THRESH_REPLACE_VAL(reg_val, (clut_thresh4_data[i] & 0x3FF));
        ued_reg->CLUT4[i] = reg_val;
	    reg_val = 0;
        reg_val = UED_ADVANCED_CLUT5_BIAS_REPLACE_VAL(reg_val, clut_bias5_data[i]);
        reg_val = UED_ADVANCED_CLUT5_THRESH_REPLACE_VAL(reg_val, (clut_thresh5_data[i] & 0x3FF));
        ued_reg->CLUT5[i] = reg_val;
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0); 
}

//********************************
// ued_clut_check
//********************************
//static
 void ued_clut_check(UED_ADVANCED_REGS_t *ued_reg,
                           uint8_t* clut_bias0_data,
                           uint16_t* clut_thresh0_data,
                           uint8_t* clut_bias1_data,
                           uint16_t* clut_thresh1_data,
                           uint8_t* clut_bias2_data,
                           uint16_t* clut_thresh2_data,
                           uint8_t* clut_bias3_data,
                           uint16_t* clut_thresh3_data,
                           uint8_t* clut_bias4_data,
                           uint16_t* clut_thresh4_data,
                           uint8_t* clut_bias5_data,
                           uint16_t* clut_thresh5_data)
{
    uint32_t i;
    uint32_t reg_val;
    uint32_t bias;
    uint32_t thresh;

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // first set the CPU SRAM access bit
    ued_cpu_sram_access(ued_reg,1); 

    // check the LUT entries
    for(i=0; i<UED_CLUT_DEPTH; i++)
    {
        reg_val = ued_reg->CLUT0[i];
        bias = (reg_val & UED_ADVANCED_CLUT0_BIAS_MASK) >> UED_ADVANCED_CLUT0_BIAS_SHIFT;
        thresh = (reg_val & UED_ADVANCED_CLUT0_THRESH_MASK) >> UED_ADVANCED_CLUT0_THRESH_SHIFT;
        if((bias != clut_bias0_data[i]) || (thresh != (clut_thresh0_data[i] & 0x3FF)))
        {
            DBG_PRINTF_NOTICE("ERROR: %s : chck_cluts0 mismatch, entry %u\n", __FUNCTION__, i);
            DBG_PRINTF_NOTICE("   bias: expected 0x%08x, saw 0x%08x\r\n", clut_bias0_data[i], bias);
            DBG_PRINTF_NOTICE("   thresh: expected 0x%08x, saw 0x%08x\r\n", (clut_thresh0_data[i] & 0x3FF), thresh); 
        }
        reg_val = ued_reg->CLUT1[i];
        bias = (reg_val & UED_ADVANCED_CLUT1_BIAS_MASK) >> UED_ADVANCED_CLUT1_BIAS_SHIFT;
        thresh = (reg_val & UED_ADVANCED_CLUT1_THRESH_MASK) >> UED_ADVANCED_CLUT1_THRESH_SHIFT;
        if((bias != clut_bias1_data[i]) || (thresh != (clut_thresh1_data[i] & 0x3FF)))
        {
            DBG_PRINTF_NOTICE("ERROR: %s : chck_cluts1 mismatch, entry %u\n", __FUNCTION__, i);
            DBG_PRINTF_NOTICE("   bias: expected 0x%08x, saw 0x%08x\r\n", clut_bias1_data[i], bias);
            DBG_PRINTF_NOTICE("   thresh: expected 0x%08x, saw 0x%08x\r\n", (clut_thresh1_data[i] & 0x3FF), thresh); 
        }
        reg_val = ued_reg->CLUT2[i];
        bias = (reg_val & UED_ADVANCED_CLUT2_BIAS_MASK) >> UED_ADVANCED_CLUT2_BIAS_SHIFT;
        thresh = (reg_val & UED_ADVANCED_CLUT2_THRESH_MASK) >> UED_ADVANCED_CLUT2_THRESH_SHIFT;
        if((bias != clut_bias2_data[i]) || (thresh != (clut_thresh2_data[i] & 0x3FF)))
        {
            DBG_PRINTF_NOTICE("ERROR: %s : chck_cluts2 mismatch, entry %u\n", __FUNCTION__, i);
            DBG_PRINTF_NOTICE("   bias: expected 0x%08x, saw 0x%08x\r\n", clut_bias2_data[i], bias);
            DBG_PRINTF_NOTICE("   thresh: expected 0x%08x, saw 0x%08x\r\n", (clut_thresh2_data[i] & 0x3FF), thresh); 
        }
        reg_val = ued_reg->CLUT3[i];
        bias = (reg_val & UED_ADVANCED_CLUT3_BIAS_MASK) >> UED_ADVANCED_CLUT3_BIAS_SHIFT;
        thresh = (reg_val & UED_ADVANCED_CLUT3_THRESH_MASK) >> UED_ADVANCED_CLUT3_THRESH_SHIFT;
        if((bias != clut_bias3_data[i]) || (thresh != (clut_thresh3_data[i] & 0x3FF)))
        {
            DBG_PRINTF_NOTICE("ERROR: %s : chck_cluts3 mismatch, entry %u\n", __FUNCTION__, i);
            DBG_PRINTF_NOTICE("   bias: expected 0x%08x, saw 0x%08x\r\n", clut_bias3_data[i], bias);
            DBG_PRINTF_NOTICE("   thresh: expected 0x%08x, saw 0x%08x\r\n", (clut_thresh3_data[i] & 0x3FF), thresh); 
        }
        reg_val = ued_reg->CLUT4[i];
        bias = (reg_val & UED_ADVANCED_CLUT4_BIAS_MASK) >> UED_ADVANCED_CLUT4_BIAS_SHIFT;
        thresh = (reg_val & UED_ADVANCED_CLUT4_THRESH_MASK) >> UED_ADVANCED_CLUT4_THRESH_SHIFT;
        if((bias != clut_bias4_data[i]) || (thresh != (clut_thresh4_data[i] & 0x3FF)))
        {
            DBG_PRINTF_NOTICE("ERROR: %s : chck_cluts4 mismatch, entry %u\n", __FUNCTION__, i);
            DBG_PRINTF_NOTICE("   bias: expected 0x%08x, saw 0x%08x\r\n", clut_bias4_data[i], bias);
            DBG_PRINTF_NOTICE("   thresh: expected 0x%08x, saw 0x%08x\r\n", (clut_thresh4_data[i] & 0x3FF), thresh); 
        }
        reg_val = ued_reg->CLUT5[i];
        bias = (reg_val & UED_ADVANCED_CLUT5_BIAS_MASK) >> UED_ADVANCED_CLUT5_BIAS_SHIFT;
        thresh = (reg_val & UED_ADVANCED_CLUT5_THRESH_MASK) >> UED_ADVANCED_CLUT5_THRESH_SHIFT;
        if((bias != clut_bias5_data[i]) || (thresh != (clut_thresh5_data[i] & 0x3FF)))
        {
            DBG_PRINTF_NOTICE("ERROR: %s : chck_cluts5 mismatch, entry %u\n", __FUNCTION__, i);
            DBG_PRINTF_NOTICE("   bias: expected 0x%08x, saw 0x%08x\r\n", clut_bias5_data[i], bias);
            DBG_PRINTF_NOTICE("   thresh: expected 0x%08x, saw 0x%08x\r\n", (clut_thresh5_data[i] & 0x3FF), thresh); 
        }
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0); 
}


//********************************
// ued_wlut_load
//********************************
static void ued_wlut_load(UED_ADVANCED_REGS_t *ued_reg,
                          uint32_t* wlut0_data,
                          uint32_t* wlut1_data,
                          uint32_t* wlut2_data,
                          uint32_t* wlut3_data,
                          uint32_t* wlut4_data,
                          uint32_t* wlut5_data,
                          uint32_t* wlut6_data,
                          uint32_t* wlut7_data)
{
    uint32_t reg_val;
    uint32_t i;

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // first set the CPU SRAM access bit
    ued_cpu_sram_access(ued_reg,1); 

    // blast the data into the LUT
    for(i=0; i<UED_WLUT_DEPTH; i++)
    {
        reg_val = 0;
        reg_val         = UED_ADVANCED_WLUPW_W7_REPLACE_VAL(reg_val, wlut7_data[i]);
        reg_val         = UED_ADVANCED_WLUPW_W6_REPLACE_VAL(reg_val, wlut6_data[i]);
        reg_val         = UED_ADVANCED_WLUPW_W5_REPLACE_VAL(reg_val, wlut5_data[i]);
        reg_val         = UED_ADVANCED_WLUPW_W4_REPLACE_VAL(reg_val, wlut4_data[i]);
        ued_reg->WLupW = reg_val;
        reg_val = 0;
        reg_val         = UED_ADVANCED_WLUT0_W3_REPLACE_VAL   (reg_val, wlut3_data[i]);
        reg_val         = UED_ADVANCED_WLUT0_W2_REPLACE_VAL   (reg_val, wlut2_data[i]);
        reg_val         = UED_ADVANCED_WLUT0_W1_REPLACE_VAL   (reg_val, wlut1_data[i]);
        reg_val         = UED_ADVANCED_WLUT0_W0_REPLACE_VAL   (reg_val, wlut0_data[i]);
        ued_reg->WLUT0[i] = reg_val;
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0); 
}

//********************************
// ued_wlut_check
//********************************
static void ued_wlut_check(UED_ADVANCED_REGS_t *ued_reg,
                           uint32_t* wlut0_data,
                           uint32_t* wlut1_data,
                           uint32_t* wlut2_data,
                           uint32_t* wlut3_data,
                           uint32_t* wlut4_data,
                           uint32_t* wlut5_data,
                           uint32_t* wlut6_data,
                           uint32_t* wlut7_data)
{
    uint32_t wlut0_val;
    uint32_t wlut1_val;
    uint32_t wlut2_val;
    uint32_t wlut3_val;
    uint32_t wlut4_val;
    uint32_t wlut5_val;
    uint32_t wlut6_val;
    uint32_t wlut7_val;
    uint32_t lut_val;
    uint32_t i;

    // first set the LUT access bits
    ued_cpu_sram_access(ued_reg,1); 

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // read and compare the LUT data
    for(i=0; i<UED_WLUT_DEPTH; i++)
    {
        lut_val = ued_reg->WLUT0[i];
        wlut0_val = UED_ADVANCED_WLUT0_W0_MASK_SHIFT(lut_val);
        wlut1_val = UED_ADVANCED_WLUT0_W1_MASK_SHIFT(lut_val);
        wlut2_val = UED_ADVANCED_WLUT0_W2_MASK_SHIFT(lut_val);
        wlut3_val = UED_ADVANCED_WLUT0_W3_MASK_SHIFT(lut_val);
        lut_val = ued_reg->WLupR0;
        wlut4_val = UED_ADVANCED_WLUPR0_W4_MASK_SHIFT(lut_val);
        wlut5_val = UED_ADVANCED_WLUPR0_W5_MASK_SHIFT(lut_val);
        wlut6_val = UED_ADVANCED_WLUPR0_W6_MASK_SHIFT(lut_val);
        wlut7_val = UED_ADVANCED_WLUPR0_W7_MASK_SHIFT(lut_val);
        
        if(wlut0_val != wlut0_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut0 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut0_val, wlut0_data[i]);
        }
        if(wlut1_val != wlut1_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut1 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut1_val, wlut1_data[i]);
        }
        if(wlut2_val != wlut2_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut2 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut2_val, wlut2_data[i]);
        }
        if(wlut3_val != wlut3_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut3 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut3_val, wlut3_data[i]);
        }
        if(wlut4_val != wlut4_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut4 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut4_val, wlut4_data[i]);
        }
        if(wlut5_val != wlut5_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut5 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut5_val, wlut5_data[i]);
        }
        if(wlut6_val != wlut6_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut6 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut6_val, wlut6_data[i]);
        }
        if(wlut7_val != wlut7_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut7 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut7_val, wlut7_data[i]);
        }
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0);
}

//********************************
// ued_wlut4_load
//********************************
static void ued_wlut4_load(UED_ADVANCED_REGS_t *ued_reg,
                           uint32_t* wlut0_data,
                           uint32_t* wlut1_data,
                           uint32_t* wlut2_data,
                           uint32_t* wlut3_data)
{
    uint32_t reg_val;
    uint32_t i;

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // first set the CPU SRAM access bit
    ued_cpu_sram_access(ued_reg,1);

    // blast the data into the LUT
    for(i=0; i<UED_WLUT_DEPTH*2; i=i+2)
    {
        reg_val = 0;
        reg_val = UED_ADVANCED_WLUPW_W7_REPLACE_VAL(reg_val, wlut3_data[i+1]);
        reg_val = UED_ADVANCED_WLUPW_W6_REPLACE_VAL(reg_val, wlut2_data[i+1]);
        reg_val = UED_ADVANCED_WLUPW_W5_REPLACE_VAL(reg_val, wlut1_data[i+1]);
        reg_val = UED_ADVANCED_WLUPW_W4_REPLACE_VAL(reg_val, wlut0_data[i+1]);
        ued_reg->WLupW = reg_val;
        reg_val = 0;
        reg_val = UED_ADVANCED_WLUT0_W3_REPLACE_VAL (reg_val, wlut3_data[i]);
        reg_val = UED_ADVANCED_WLUT0_W2_REPLACE_VAL (reg_val, wlut2_data[i]);
        reg_val = UED_ADVANCED_WLUT0_W1_REPLACE_VAL (reg_val, wlut1_data[i]);
        reg_val = UED_ADVANCED_WLUT0_W0_REPLACE_VAL (reg_val, wlut0_data[i]);
        ued_reg->WLUT0[i/2] = reg_val;
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0);
}

//********************************
// ued_wlut4_check
//********************************
static void ued_wlut4_check(UED_ADVANCED_REGS_t *ued_reg,
                            uint32_t* wlut0_data,
                            uint32_t* wlut1_data,
                            uint32_t* wlut2_data,
                            uint32_t* wlut3_data)
{
    uint32_t wlut0_val;
    uint32_t wlut1_val;
    uint32_t wlut2_val;
    uint32_t wlut3_val;
    uint32_t wlut4_val;
    uint32_t wlut5_val;
    uint32_t wlut6_val;
    uint32_t wlut7_val;
    uint32_t lut_val;
    uint32_t i;

    // first set the LUT access bits
    ued_cpu_sram_access(ued_reg,1); 

    DBG_PRINTF_NOTICE("%s called\n\r", __FUNCTION__);

    // read and compare the LUT data
    for(i=0; i<(UED_WLUT_DEPTH*2); i+=2)
    {
        lut_val = ued_reg->WLUT0[i/2];
        wlut0_val = UED_ADVANCED_WLUT0_W0_MASK_SHIFT(lut_val);
        wlut1_val = UED_ADVANCED_WLUT0_W1_MASK_SHIFT(lut_val);
        wlut2_val = UED_ADVANCED_WLUT0_W2_MASK_SHIFT(lut_val);
        wlut3_val = UED_ADVANCED_WLUT0_W3_MASK_SHIFT(lut_val);
        lut_val = ued_reg->WLupR0;
        wlut4_val = UED_ADVANCED_WLUPR0_W4_MASK_SHIFT(lut_val);
        wlut5_val = UED_ADVANCED_WLUPR0_W5_MASK_SHIFT(lut_val);
        wlut6_val = UED_ADVANCED_WLUPR0_W6_MASK_SHIFT(lut_val);
        wlut7_val = UED_ADVANCED_WLUPR0_W7_MASK_SHIFT(lut_val);
        if(wlut0_val != wlut0_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut0 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut0_val, wlut0_data[i]);
        }
        if(wlut1_val != wlut1_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut1 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut1_val, wlut1_data[i]);
        }
        if(wlut2_val != wlut2_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut2 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut2_val, wlut2_data[i]);
        }
        if(wlut3_val != wlut3_data[i]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut3 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut3_val, wlut3_data[i]);
        }
        if(wlut4_val != wlut0_data[i+1]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut4 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut4_val, wlut0_data[i+1]);
        }
        if(wlut5_val != wlut1_data[i+1]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut5 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut5_val, wlut1_data[i+1]);
        }
        if(wlut6_val != wlut2_data[i+1]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut6 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut6_val, wlut2_data[i+1]);
        }
        if(wlut7_val != wlut3_data[i+1]) 
        {
            DBG_PRINTF_NOTICE("ERROR: %s : Incorrect wlut7 data at Addr=0x%8x, Act=0x%2x, Exp=0x%2x\n\r", __FUNCTION__, &ued_reg->WLUT0           [i], wlut7_val, wlut3_data[i+1]);
        }
    }

    // disable LUT access
    ued_cpu_sram_access(ued_reg,0);
}
