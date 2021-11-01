/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the KMOD block.
  * 
 **/

#ifndef __KMOD_FUNC_C_INCLUDED
#define __KMOD_FUNC_C_INCLUDED

#include "kmod_api.h"

void kmod_cfg_driver(KMOD_REGS_t *kmod_reg, kmod_cfg_t *kmod_cfg)
{
    uint32_t reg_val;

    // Configuration Register 1
    reg_val = kmod_reg->KMDC1R;
    reg_val = KMOD_KMDC1R_DMRS_REPLACE_VAL(reg_val, kmod_cfg->dmrs);
    reg_val = KMOD_KMDC1R_DMCS_REPLACE_VAL(reg_val, kmod_cfg->dmcs);
    reg_val = KMOD_KMDC1R_TRAPEN_REPLACE_VAL(reg_val, kmod_cfg->trap);
    reg_val = KMOD_KMDC1R_STARTDIR_REPLACE_VAL(reg_val, kmod_cfg->startdir);
    reg_val = KMOD_KMDC1R_SERP_REPLACE_VAL(reg_val, kmod_cfg->serp);
    reg_val = KMOD_KMDC1R_PAD_REPLACE_VAL(reg_val, kmod_cfg->pad);
    reg_val = KMOD_KMDC1R_CTHRESH_REPLACE_VAL(reg_val, kmod_cfg->cthresh);
    reg_val = KMOD_KMDC1R_MTHRESH_REPLACE_VAL(reg_val, kmod_cfg->mthresh);
    reg_val = KMOD_KMDC1R_KTHRESH_REPLACE_VAL(reg_val, kmod_cfg->kthresh);
    reg_val = KMOD_KMDC1R_WINSIZE_REPLACE_VAL(reg_val, kmod_cfg->window);
    reg_val = KMOD_KMDC1R_BYPASS_REPLACE_VAL(reg_val, kmod_cfg->bypass);
    kmod_reg->KMDC1R = reg_val;

    // Configuration Register 2
    reg_val = kmod_reg->KMDC2R;
    reg_val = KMOD_KMDC2R_MMODE_REPLACE_VAL(reg_val, kmod_cfg->mmode);
    reg_val = KMOD_KMDC2R_CMODE_REPLACE_VAL(reg_val, kmod_cfg->cmode);
    reg_val = KMOD_KMDC2R_KDBYPASS_REPLACE_VAL(reg_val, kmod_cfg->kdbypass);
    reg_val = KMOD_KMDC2R_KFBYPASS_REPLACE_VAL(reg_val, kmod_cfg->kfbypass);
    kmod_reg->KMDC2R = reg_val;
}

void kmod_load_pattern(KMOD_REGS_t *kmod_reg, char *pat0, char *pat1, char* pat2, char *pat3)
{
    uint32_t reg_val;
    uint32_t i;

    // Set CPU Access Bit
    kmod_reg->KMDC1R = KMOD_KMDC1R_CPU_REPLACE_VAL(kmod_reg->KMDC1R, 1);
    for(i=0; i<55; i++)
    {
         reg_val = kmod_reg->KMODPMM[i];
         reg_val = KMOD_KMODPMM_PATTERN3_REPLACE_VAL(reg_val, (pat3[i] & 0x1));
         reg_val = KMOD_KMODPMM_PATTERN2_REPLACE_VAL(reg_val, (pat2[i] & 0x1));
         reg_val = KMOD_KMODPMM_PATTERN1_REPLACE_VAL(reg_val, (pat1[i] & 0x1));
         reg_val = KMOD_KMODPMM_PATTERN0_REPLACE_VAL(reg_val, (pat0[i] & 0x1));
         kmod_reg->KMODPMM[i] = reg_val;
    }

    // Clear CPU Access Bit
    kmod_reg->KMDC1R = KMOD_KMDC1R_CPU_REPLACE_VAL(kmod_reg->KMDC1R, 0);
}

void kmod_load_dither(KMOD_REGS_t *kmod_reg, char *c_dither, char *m_dither, char *k_dither)
{
    uint32_t reg_val;
    uint32_t i;

    // Set CPU Access Bit
    kmod_reg->KMDC1R = KMOD_KMDC1R_CPU_REPLACE_VAL(kmod_reg->KMDC1R, 1);

    for(i=0; i<25; i++)
    {
        reg_val = kmod_reg->KMODDMM[i];
        reg_val = KMOD_KMODDMM_K_REPLACE_VAL(reg_val, (k_dither[i] & 0xF));
        reg_val = KMOD_KMODDMM_C_REPLACE_VAL(reg_val, (c_dither[i] & 0xF));
        reg_val = KMOD_KMODDMM_M_REPLACE_VAL(reg_val, (m_dither[i] & 0xF));
        kmod_reg->KMODDMM[i] = reg_val;
    }
    // Clear CPU Access Bit
    kmod_reg->KMDC1R = KMOD_KMDC1R_CPU_REPLACE_VAL(kmod_reg->KMDC1R, 0);
}

void kmod_enable(KMOD_REGS_t *kmod_reg)
{
    kmod_reg->KMDC1R = KMOD_KMDC1R_ENABLE_REPLACE_VAL(kmod_reg->KMDC1R, 1);
}

void kmod_disable(KMOD_REGS_t *kmod_reg)
{
    kmod_reg->KMDC1R = KMOD_KMDC1R_ENABLE_REPLACE_VAL(kmod_reg->KMDC1R, 0);
}

#endif
