/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the CTWIN block.
  * 
 **/

#include "ctwin_api.h"


void ctwin_cfg_driver(CTWIN_REGS_t *reg, ctwin_cfg_t *cfg)
{
    uint32_t reg_val;
 
    reg_val = reg->CCR;
    reg_val = CTWIN_CCR_BYPASS_REPLACE_VAL(reg_val, cfg->bypass);
    reg_val = CTWIN_CCR_LINE_WIDTH_REPLACE_VAL(reg_val, cfg->line_width);
    reg_val = CTWIN_CCR_KT_MERGE_FUNC_REPLACE_VAL(reg_val, cfg->kt_merge_func);
    reg_val = CTWIN_CCR_TEXT_MODE_REPLACE_VAL(reg_val, cfg->kt_text_mode);
    reg_val = CTWIN_CCR_SERP_REPLACE_VAL(reg_val, cfg->serp);
    reg_val = CTWIN_CCR_LB_SRAM_ACCESS_ENABLE_REPLACE_VAL(reg_val, cfg->lb_sram_access_enable);
    reg_val = CTWIN_CCR_SAVE_RESTORE_MODE_REPLACE_VAL(reg_val, cfg->save_restore_mode);
    reg->CCR = reg_val;
 
    reg_val = reg->W1CR;
    reg_val = CTWIN_W1CR_WIN1_PAT_SEL_REPLACE_VAL(reg_val, cfg->win1_pat_sel);
    reg_val = CTWIN_W1CR_WIN1_PAT_INV_REPLACE_VAL(reg_val, cfg->win1_pat_inv);
    reg_val = CTWIN_W1CR_WIN1_PAT_FUNC_REPLACE_VAL(reg_val, cfg->win1_pat_func);
    reg_val = CTWIN_W1CR_WIN1_INPUT_FUNC_REPLACE_VAL(reg_val, cfg->win1_input_func);
    reg_val = CTWIN_W1CR_WIN1_INPUT_INV_REPLACE_VAL(reg_val, cfg->win1_input_inv);
    reg_val = CTWIN_W1CR_WIN1_INPUT_SEL_REPLACE_VAL(reg_val, cfg->win1_input_sel);
    reg->W1CR = reg_val;
 
    reg_val = reg->W2CR;
    reg_val = CTWIN_W2CR_WIN2_PAT_SEL_REPLACE_VAL(reg_val, cfg->win2_pat_sel);
    reg_val = CTWIN_W2CR_WIN2_PAT_INV_REPLACE_VAL(reg_val, cfg->win2_pat_inv);
    reg_val = CTWIN_W2CR_WIN2_PAT_FUNC_REPLACE_VAL(reg_val, cfg->win2_pat_func);
    reg_val = CTWIN_W2CR_WIN2_INPUT_FUNC_REPLACE_VAL(reg_val, cfg->win2_input_func);
    reg_val = CTWIN_W2CR_WIN2_INPUT_INV_REPLACE_VAL(reg_val, cfg->win2_input_inv);
    reg_val = CTWIN_W2CR_WIN2_INPUT_SEL_REPLACE_VAL(reg_val, cfg->win2_input_sel);
    reg->W2CR = reg_val;
 
    reg_val = reg->OCR;
    reg_val = CTWIN_OCR_OUT_MODE0_REPLACE_VAL(reg_val, cfg->out_mode[0]);
    reg_val = CTWIN_OCR_OUT_MODE1_REPLACE_VAL(reg_val, cfg->out_mode[1]);
    reg_val = CTWIN_OCR_OUT_MODE2_REPLACE_VAL(reg_val, cfg->out_mode[2]);
    reg_val = CTWIN_OCR_OUT_MODE3_REPLACE_VAL(reg_val, cfg->out_mode[3]);
    reg_val = CTWIN_OCR_OUT_MODE4_REPLACE_VAL(reg_val, cfg->out_mode[4]);
    reg_val = CTWIN_OCR_OUT_MODE5_REPLACE_VAL(reg_val, cfg->out_mode[5]);
    reg_val = CTWIN_OCR_OUT_WIN_SEL0_REPLACE_VAL(reg_val, cfg->out_win_sel[0]);
    reg_val = CTWIN_OCR_OUT_WIN_SEL1_REPLACE_VAL(reg_val, cfg->out_win_sel[1]);
    reg_val = CTWIN_OCR_OUT_WIN_SEL2_REPLACE_VAL(reg_val, cfg->out_win_sel[2]);
    reg_val = CTWIN_OCR_OUT_WIN_SEL3_REPLACE_VAL(reg_val, cfg->out_win_sel[3]);
    reg_val = CTWIN_OCR_OUT_WIN_SEL4_REPLACE_VAL(reg_val, cfg->out_win_sel[4]);
    reg_val = CTWIN_OCR_OUT_WIN_SEL5_REPLACE_VAL(reg_val, cfg->out_win_sel[5]);
    reg_val = CTWIN_OCR_OUT_WIN_FUNC_REPLACE_VAL(reg_val, cfg->out_win_func);
    reg_val = CTWIN_OCR_EDGE_PAT_SEL_REPLACE_VAL(reg_val, cfg->edge_pat_sel);
    reg->OCR = reg_val;
 
    reg_val = reg->OVR1;
    reg_val = CTWIN_OVR1_OUT_VAL0_REPLACE_VAL(reg_val, cfg->out_val[0]);
    reg_val = CTWIN_OVR1_OUT_VAL1_REPLACE_VAL(reg_val, cfg->out_val[1]);
    reg_val = CTWIN_OVR1_OUT_VAL2_REPLACE_VAL(reg_val, cfg->out_val[2]);
    reg->OVR1 = reg_val;
 
    reg_val = reg->OVR2;
    reg_val = CTWIN_OVR2_OUT_VAL3_REPLACE_VAL(reg_val, cfg->out_val[3]);
    reg_val = CTWIN_OVR2_OUT_VAL4_REPLACE_VAL(reg_val, cfg->out_val[4]);
    reg_val = CTWIN_OVR2_OUT_VAL5_REPLACE_VAL(reg_val, cfg->out_val[5]);
    reg->OVR2 = reg_val;
 
    reg_val = reg->KMR1;
    reg_val = CTWIN_KMR1_KT_MERGE_VAL_0_REPLACE_VAL(reg_val, cfg->kt_merge_val[0]);
    reg_val = CTWIN_KMR1_KT_MERGE_VAL_1_REPLACE_VAL(reg_val, cfg->kt_merge_val[1]);
    reg_val = CTWIN_KMR1_KT_MERGE_VAL_2_REPLACE_VAL(reg_val, cfg->kt_merge_val[2]);
    reg->KMR1 = reg_val;
 
    reg_val = reg->KMR2;
    reg_val = CTWIN_KMR2_KT_MERGE_VAL_3_REPLACE_VAL(reg_val, cfg->kt_merge_val[3]);
    reg_val = CTWIN_KMR2_KT_MERGE_VAL_4_REPLACE_VAL(reg_val, cfg->kt_merge_val[4]);
    reg->KMR2 = reg_val;
 
    reg_val = reg->W1TH0R1;
    reg_val = CTWIN_W1TH0R1_WIN1_THRESH0_PLANE0_REPLACE_VAL(reg_val, cfg->win1_thresh0[0]);
    reg_val = CTWIN_W1TH0R1_WIN1_THRESH0_PLANE1_REPLACE_VAL(reg_val, cfg->win1_thresh0[1]);
    reg_val = CTWIN_W1TH0R1_WIN1_THRESH0_PLANE2_REPLACE_VAL(reg_val, cfg->win1_thresh0[2]);
    reg->W1TH0R1 = reg_val;
 
    reg_val = reg->W1TH0R2;
    reg_val = CTWIN_W1TH0R2_WIN1_THRESH0_PLANE3_REPLACE_VAL(reg_val, cfg->win1_thresh0[3]);
    reg_val = CTWIN_W1TH0R2_WIN1_THRESH0_PLANE4_REPLACE_VAL(reg_val, cfg->win1_thresh0[4]);
    reg_val = CTWIN_W1TH0R2_WIN1_THRESH0_PLANE5_REPLACE_VAL(reg_val, cfg->win1_thresh0[5]);
    reg->W1TH0R2 = reg_val;
 
    reg_val = reg->W2TH0R1;
    reg_val = CTWIN_W2TH0R1_WIN2_THRESH0_PLANE0_REPLACE_VAL(reg_val, cfg->win2_thresh0[0]);
    reg_val = CTWIN_W2TH0R1_WIN2_THRESH0_PLANE1_REPLACE_VAL(reg_val, cfg->win2_thresh0[1]);
    reg_val = CTWIN_W2TH0R1_WIN2_THRESH0_PLANE2_REPLACE_VAL(reg_val, cfg->win2_thresh0[2]);
    reg->W2TH0R1 = reg_val;
 
    reg_val = reg->W2TH0R2;
    reg_val = CTWIN_W2TH0R2_WIN2_THRESH0_PLANE3_REPLACE_VAL(reg_val, cfg->win2_thresh0[3]);
    reg_val = CTWIN_W2TH0R2_WIN2_THRESH0_PLANE4_REPLACE_VAL(reg_val, cfg->win2_thresh0[4]);
    reg_val = CTWIN_W2TH0R2_WIN2_THRESH0_PLANE5_REPLACE_VAL(reg_val, cfg->win2_thresh0[5]);
    reg->W2TH0R2 = reg_val;
 
    reg_val = reg->W1TH1R1;
    reg_val = CTWIN_W1TH1R1_WIN1_THRESH1_PLANE0_REPLACE_VAL(reg_val, cfg->win1_thresh1[0]);
    reg_val = CTWIN_W1TH1R1_WIN1_THRESH1_PLANE1_REPLACE_VAL(reg_val, cfg->win1_thresh1[1]);
    reg_val = CTWIN_W1TH1R1_WIN1_THRESH1_PLANE2_REPLACE_VAL(reg_val, cfg->win1_thresh1[2]);
    reg->W1TH1R1 = reg_val;
 
    reg_val = reg->W1TH1R2;
    reg_val = CTWIN_W1TH1R2_WIN1_THRESH1_PLANE3_REPLACE_VAL(reg_val, cfg->win1_thresh1[3]);
    reg_val = CTWIN_W1TH1R2_WIN1_THRESH1_PLANE4_REPLACE_VAL(reg_val, cfg->win1_thresh1[4]);
    reg_val = CTWIN_W1TH1R2_WIN1_THRESH1_PLANE5_REPLACE_VAL(reg_val, cfg->win1_thresh1[5]);
    reg->W1TH1R2 = reg_val;
 
    reg_val = reg->W2TH1R1;
    reg_val = CTWIN_W2TH1R1_WIN2_THRESH1_PLANE0_REPLACE_VAL(reg_val, cfg->win2_thresh1[0]);
    reg_val = CTWIN_W2TH1R1_WIN2_THRESH1_PLANE1_REPLACE_VAL(reg_val, cfg->win2_thresh1[1]);
    reg_val = CTWIN_W2TH1R1_WIN2_THRESH1_PLANE2_REPLACE_VAL(reg_val, cfg->win2_thresh1[2]);
    reg->W2TH1R1 = reg_val;
 
    reg_val = reg->W2TH1R2;
    reg_val = CTWIN_W2TH1R2_WIN2_THRESH1_PLANE3_REPLACE_VAL(reg_val, cfg->win2_thresh1[3]);
    reg_val = CTWIN_W2TH1R2_WIN2_THRESH1_PLANE4_REPLACE_VAL(reg_val, cfg->win2_thresh1[4]);
    reg_val = CTWIN_W2TH1R2_WIN2_THRESH1_PLANE5_REPLACE_VAL(reg_val, cfg->win2_thresh1[5]);
    reg->W2TH1R2 = reg_val;
 
    reg_val = reg->W1TH2R1;
    reg_val = CTWIN_W1TH2R1_WIN1_THRESH2_PLANE0_REPLACE_VAL(reg_val, cfg->win1_thresh2[0]);
    reg_val = CTWIN_W1TH2R1_WIN1_THRESH2_PLANE1_REPLACE_VAL(reg_val, cfg->win1_thresh2[1]);
    reg_val = CTWIN_W1TH2R1_WIN1_THRESH2_PLANE2_REPLACE_VAL(reg_val, cfg->win1_thresh2[2]);
    reg->W1TH2R1 = reg_val;
 
    reg_val = reg->W1TH2R2;
    reg_val = CTWIN_W1TH2R2_WIN1_THRESH2_PLANE3_REPLACE_VAL(reg_val, cfg->win1_thresh2[3]);
    reg_val = CTWIN_W1TH2R2_WIN1_THRESH2_PLANE4_REPLACE_VAL(reg_val, cfg->win1_thresh2[4]);
    reg_val = CTWIN_W1TH2R2_WIN1_THRESH2_PLANE5_REPLACE_VAL(reg_val, cfg->win1_thresh2[5]);
    reg->W1TH2R2 = reg_val;
 
    reg_val = reg->W2TH2R1;
    reg_val = CTWIN_W2TH2R1_WIN2_THRESH2_PLANE0_REPLACE_VAL(reg_val, cfg->win2_thresh2[0]);
    reg_val = CTWIN_W2TH2R1_WIN2_THRESH2_PLANE1_REPLACE_VAL(reg_val, cfg->win2_thresh2[1]);
    reg_val = CTWIN_W2TH2R1_WIN2_THRESH2_PLANE2_REPLACE_VAL(reg_val, cfg->win2_thresh2[2]);
    reg->W2TH2R1 = reg_val;
 
    reg_val = reg->W2TH2R2;
    reg_val = CTWIN_W2TH2R2_WIN2_THRESH2_PLANE3_REPLACE_VAL(reg_val, cfg->win2_thresh2[3]);
    reg_val = CTWIN_W2TH2R2_WIN2_THRESH2_PLANE4_REPLACE_VAL(reg_val, cfg->win2_thresh2[4]);
    reg_val = CTWIN_W2TH2R2_WIN2_THRESH2_PLANE5_REPLACE_VAL(reg_val, cfg->win2_thresh2[5]);
    reg->W2TH2R2 = reg_val;
 
    reg_val = reg->ESCR1;
    reg_val = CTWIN_ESCR1_EDGE_SC_ONE_AWAY0_REPLACE_VAL(reg_val, cfg->edge_sc_oneaway[0]);
    reg_val = CTWIN_ESCR1_EDGE_SC_ONE_AWAY1_REPLACE_VAL(reg_val, cfg->edge_sc_oneaway[1]);
    reg_val = CTWIN_ESCR1_EDGE_SC_ONE_AWAY2_REPLACE_VAL(reg_val, cfg->edge_sc_oneaway[2]);
    reg->ESCR1 = reg_val;
 
    reg_val = reg->ESCR2;
    reg_val = CTWIN_ESCR2_EDGE_SC_ONE_AWAY3_REPLACE_VAL(reg_val, cfg->edge_sc_oneaway[3]);
    reg_val = CTWIN_ESCR2_EDGE_SC_ONE_AWAY4_REPLACE_VAL(reg_val, cfg->edge_sc_oneaway[4]);
    reg_val = CTWIN_ESCR2_EDGE_SC_ONE_AWAY5_REPLACE_VAL(reg_val, cfg->edge_sc_oneaway[5]);
    reg->ESCR2 = reg_val;
 
    reg_val = reg->ESCR3;
    reg_val = CTWIN_ESCR3_EDGE_SC_TWO_AWAY0_REPLACE_VAL(reg_val, cfg->edge_sc_twoaway[0]);
    reg_val = CTWIN_ESCR3_EDGE_SC_TWO_AWAY1_REPLACE_VAL(reg_val, cfg->edge_sc_twoaway[1]);
    reg_val = CTWIN_ESCR3_EDGE_SC_TWO_AWAY2_REPLACE_VAL(reg_val, cfg->edge_sc_twoaway[2]);
    reg->ESCR3 = reg_val;
 
    reg_val = reg->ESCR4;
    reg_val = CTWIN_ESCR4_EDGE_SC_TWO_AWAY3_REPLACE_VAL(reg_val, cfg->edge_sc_twoaway[3]);
    reg_val = CTWIN_ESCR4_EDGE_SC_TWO_AWAY4_REPLACE_VAL(reg_val, cfg->edge_sc_twoaway[4]);
    reg_val = CTWIN_ESCR4_EDGE_SC_TWO_AWAY5_REPLACE_VAL(reg_val, cfg->edge_sc_twoaway[5]);
    reg->ESCR4 = reg_val;
 
    reg_val = reg->ESCR5;
    reg_val = CTWIN_ESCR5_EDGE_SC_THREE_AWAY0_REPLACE_VAL(reg_val, cfg->edge_sc_threeaway[0]);
    reg_val = CTWIN_ESCR5_EDGE_SC_THREE_AWAY1_REPLACE_VAL(reg_val, cfg->edge_sc_threeaway[1]);
    reg_val = CTWIN_ESCR5_EDGE_SC_THREE_AWAY2_REPLACE_VAL(reg_val, cfg->edge_sc_threeaway[2]);
    reg->ESCR5 = reg_val;
 
    reg_val = reg->ESCR6;
    reg_val = CTWIN_ESCR6_EDGE_SC_THREE_AWAY3_REPLACE_VAL(reg_val, cfg->edge_sc_threeaway[3]);
    reg_val = CTWIN_ESCR6_EDGE_SC_THREE_AWAY4_REPLACE_VAL(reg_val, cfg->edge_sc_threeaway[4]);
    reg_val = CTWIN_ESCR6_EDGE_SC_THREE_AWAY5_REPLACE_VAL(reg_val, cfg->edge_sc_threeaway[5]);
    reg->ESCR6 = reg_val;
 
    reg_val = reg->ETHR1;
    reg_val = CTWIN_ETHR1_EDGE_TH_ONE_AWAY0_REPLACE_VAL(reg_val, cfg->edge_th_oneaway[0]);
    reg_val = CTWIN_ETHR1_EDGE_TH_ONE_AWAY1_REPLACE_VAL(reg_val, cfg->edge_th_oneaway[1]);
    reg_val = CTWIN_ETHR1_EDGE_TH_ONE_AWAY2_REPLACE_VAL(reg_val, cfg->edge_th_oneaway[2]);
    reg->ETHR1 = reg_val;
 
    reg_val = reg->ETHR2;
    reg_val = CTWIN_ETHR2_EDGE_TH_ONE_AWAY3_REPLACE_VAL(reg_val, cfg->edge_th_oneaway[3]);
    reg_val = CTWIN_ETHR2_EDGE_TH_ONE_AWAY4_REPLACE_VAL(reg_val, cfg->edge_th_oneaway[4]);
    reg_val = CTWIN_ETHR2_EDGE_TH_ONE_AWAY5_REPLACE_VAL(reg_val, cfg->edge_th_oneaway[5]);
    reg->ETHR2 = reg_val;
 
    reg_val = reg->ETHR3;
    reg_val = CTWIN_ETHR3_EDGE_TH_TWO_AWAY0_REPLACE_VAL(reg_val, cfg->edge_th_twoaway[0]);
    reg_val = CTWIN_ETHR3_EDGE_TH_TWO_AWAY1_REPLACE_VAL(reg_val, cfg->edge_th_twoaway[1]);
    reg_val = CTWIN_ETHR3_EDGE_TH_TWO_AWAY2_REPLACE_VAL(reg_val, cfg->edge_th_twoaway[2]);
    reg->ETHR3 = reg_val;
 
    reg_val = reg->ETHR4;
    reg_val = CTWIN_ETHR4_EDGE_TH_TWO_AWAY3_REPLACE_VAL(reg_val, cfg->edge_th_twoaway[3]);
    reg_val = CTWIN_ETHR4_EDGE_TH_TWO_AWAY4_REPLACE_VAL(reg_val, cfg->edge_th_twoaway[4]);
    reg_val = CTWIN_ETHR4_EDGE_TH_TWO_AWAY5_REPLACE_VAL(reg_val, cfg->edge_th_twoaway[5]);
    reg->ETHR4 = reg_val;
 
    reg_val = reg->ETHR5;
    reg_val = CTWIN_ETHR5_EDGE_TH_THREE_AWAY0_REPLACE_VAL(reg_val, cfg->edge_th_threeaway[0]);
    reg_val = CTWIN_ETHR5_EDGE_TH_THREE_AWAY1_REPLACE_VAL(reg_val, cfg->edge_th_threeaway[1]);
    reg_val = CTWIN_ETHR5_EDGE_TH_THREE_AWAY2_REPLACE_VAL(reg_val, cfg->edge_th_threeaway[2]);
    reg->ETHR5 = reg_val;
 
    reg_val = reg->ETHR6;
    reg_val = CTWIN_ETHR6_EDGE_TH_THREE_AWAY3_REPLACE_VAL(reg_val, cfg->edge_th_threeaway[3]);
    reg_val = CTWIN_ETHR6_EDGE_TH_THREE_AWAY4_REPLACE_VAL(reg_val, cfg->edge_th_threeaway[4]);
    reg_val = CTWIN_ETHR6_EDGE_TH_THREE_AWAY5_REPLACE_VAL(reg_val, cfg->edge_th_threeaway[5]);
    reg->ETHR6 = reg_val;
 
    reg_val = reg->P0R0;
    reg_val = CTWIN_P0R0_LOC0_REPLACE_VAL(reg_val, cfg->pattern1[0]);
    reg_val = CTWIN_P0R0_LOC1_REPLACE_VAL(reg_val, cfg->pattern1[1]);
    reg_val = CTWIN_P0R0_LOC2_REPLACE_VAL(reg_val, cfg->pattern1[2]);
    reg_val = CTWIN_P0R0_LOC3_REPLACE_VAL(reg_val, cfg->pattern1[3]);
    reg_val = CTWIN_P0R0_LOC4_REPLACE_VAL(reg_val, cfg->pattern1[4]);
    reg_val = CTWIN_P0R0_LOC5_REPLACE_VAL(reg_val, cfg->pattern1[5]);
    reg_val = CTWIN_P0R0_LOC6_REPLACE_VAL(reg_val, cfg->pattern1[6]);
    reg_val = CTWIN_P0R0_LOC7_REPLACE_VAL(reg_val, cfg->pattern1[7]);
    reg_val = CTWIN_P0R0_LOC8_REPLACE_VAL(reg_val, cfg->pattern1[8]);
    reg_val = CTWIN_P0R0_LOC9_REPLACE_VAL(reg_val, cfg->pattern1[9]);
    reg_val = CTWIN_P0R0_LOC10_REPLACE_VAL(reg_val, cfg->pattern1[10]);
    reg_val = CTWIN_P0R0_LOC11_REPLACE_VAL(reg_val, cfg->pattern1[11]);
    reg_val = CTWIN_P0R0_LOC12_REPLACE_VAL(reg_val, cfg->pattern1[12]);
    reg_val = CTWIN_P0R0_LOC13_REPLACE_VAL(reg_val, cfg->pattern1[13]);
    reg_val = CTWIN_P0R0_LOC14_REPLACE_VAL(reg_val, cfg->pattern1[14]);
    reg_val = CTWIN_P0R0_LOC15_REPLACE_VAL(reg_val, cfg->pattern1[15]);
    reg->P0R0 = reg_val;
 
    reg_val = reg->P0R1;
    reg_val = CTWIN_P0R1_LOC16_REPLACE_VAL(reg_val, cfg->pattern1[16]);
    reg_val = CTWIN_P0R1_LOC17_REPLACE_VAL(reg_val, cfg->pattern1[17]);
    reg_val = CTWIN_P0R1_LOC18_REPLACE_VAL(reg_val, cfg->pattern1[18]);
    reg_val = CTWIN_P0R1_LOC19_REPLACE_VAL(reg_val, cfg->pattern1[19]);
    reg_val = CTWIN_P0R1_LOC20_REPLACE_VAL(reg_val, cfg->pattern1[20]);
    reg_val = CTWIN_P0R1_LOC21_REPLACE_VAL(reg_val, cfg->pattern1[21]);
    reg_val = CTWIN_P0R1_LOC22_REPLACE_VAL(reg_val, cfg->pattern1[22]);
    reg_val = CTWIN_P0R1_LOC23_REPLACE_VAL(reg_val, cfg->pattern1[23]);
    reg_val = CTWIN_P0R1_LOC24_REPLACE_VAL(reg_val, cfg->pattern1[24]);
    reg_val = CTWIN_P0R1_LOC25_REPLACE_VAL(reg_val, cfg->pattern1[25]);
    reg_val = CTWIN_P0R1_LOC26_REPLACE_VAL(reg_val, cfg->pattern1[26]);
    reg_val = CTWIN_P0R1_LOC27_REPLACE_VAL(reg_val, cfg->pattern1[27]);
    reg_val = CTWIN_P0R1_LOC28_REPLACE_VAL(reg_val, cfg->pattern1[28]);
    reg_val = CTWIN_P0R1_LOC29_REPLACE_VAL(reg_val, cfg->pattern1[29]);
    reg_val = CTWIN_P0R1_LOC30_REPLACE_VAL(reg_val, cfg->pattern1[30]);
    reg_val = CTWIN_P0R1_LOC31_REPLACE_VAL(reg_val, cfg->pattern1[31]);
    reg->P0R1 = reg_val;
 
    reg_val = reg->P0R2;
    reg_val = CTWIN_P0R2_LOC32_REPLACE_VAL(reg_val, cfg->pattern1[32]);
    reg_val = CTWIN_P0R2_LOC33_REPLACE_VAL(reg_val, cfg->pattern1[33]);
    reg_val = CTWIN_P0R2_LOC34_REPLACE_VAL(reg_val, cfg->pattern1[34]);
    reg_val = CTWIN_P0R2_LOC35_REPLACE_VAL(reg_val, cfg->pattern1[35]);
    reg_val = CTWIN_P0R2_LOC36_REPLACE_VAL(reg_val, cfg->pattern1[36]);
    reg_val = CTWIN_P0R2_LOC37_REPLACE_VAL(reg_val, cfg->pattern1[37]);
    reg_val = CTWIN_P0R2_LOC38_REPLACE_VAL(reg_val, cfg->pattern1[38]);
    reg_val = CTWIN_P0R2_LOC39_REPLACE_VAL(reg_val, cfg->pattern1[39]);
    reg_val = CTWIN_P0R2_LOC40_REPLACE_VAL(reg_val, cfg->pattern1[40]);
    reg_val = CTWIN_P0R2_LOC41_REPLACE_VAL(reg_val, cfg->pattern1[41]);
    reg_val = CTWIN_P0R2_LOC42_REPLACE_VAL(reg_val, cfg->pattern1[42]);
    reg_val = CTWIN_P0R2_LOC43_REPLACE_VAL(reg_val, cfg->pattern1[43]);
    reg_val = CTWIN_P0R2_LOC44_REPLACE_VAL(reg_val, cfg->pattern1[44]);
    reg_val = CTWIN_P0R2_LOC45_REPLACE_VAL(reg_val, cfg->pattern1[45]);
    reg_val = CTWIN_P0R2_LOC46_REPLACE_VAL(reg_val, cfg->pattern1[46]);
    reg_val = CTWIN_P0R2_LOC47_REPLACE_VAL(reg_val, cfg->pattern1[47]);
    reg->P0R2 = reg_val;
 
    reg_val = reg->P0R3;
    reg_val = CTWIN_P0R3_LOC48_REPLACE_VAL(reg_val, cfg->pattern1[48]);
    reg->P0R3 = reg_val;
 
 
    reg_val = reg->P1R0;
    reg_val = CTWIN_P1R0_LOC0_REPLACE_VAL(reg_val, cfg->pattern2[0]);
    reg_val = CTWIN_P1R0_LOC1_REPLACE_VAL(reg_val, cfg->pattern2[1]);
    reg_val = CTWIN_P1R0_LOC2_REPLACE_VAL(reg_val, cfg->pattern2[2]);
    reg_val = CTWIN_P1R0_LOC3_REPLACE_VAL(reg_val, cfg->pattern2[3]);
    reg_val = CTWIN_P1R0_LOC4_REPLACE_VAL(reg_val, cfg->pattern2[4]);
    reg_val = CTWIN_P1R0_LOC5_REPLACE_VAL(reg_val, cfg->pattern2[5]);
    reg_val = CTWIN_P1R0_LOC6_REPLACE_VAL(reg_val, cfg->pattern2[6]);
    reg_val = CTWIN_P1R0_LOC7_REPLACE_VAL(reg_val, cfg->pattern2[7]);
    reg_val = CTWIN_P1R0_LOC8_REPLACE_VAL(reg_val, cfg->pattern2[8]);
    reg_val = CTWIN_P1R0_LOC9_REPLACE_VAL(reg_val, cfg->pattern2[9]);
    reg_val = CTWIN_P1R0_LOC10_REPLACE_VAL(reg_val, cfg->pattern2[10]);
    reg_val = CTWIN_P1R0_LOC11_REPLACE_VAL(reg_val, cfg->pattern2[11]);
    reg_val = CTWIN_P1R0_LOC12_REPLACE_VAL(reg_val, cfg->pattern2[12]);
    reg_val = CTWIN_P1R0_LOC13_REPLACE_VAL(reg_val, cfg->pattern2[13]);
    reg_val = CTWIN_P1R0_LOC14_REPLACE_VAL(reg_val, cfg->pattern2[14]);
    reg_val = CTWIN_P1R0_LOC15_REPLACE_VAL(reg_val, cfg->pattern2[15]);
    reg->P1R0 = reg_val;
 
    reg_val = reg->P1R1;
    reg_val = CTWIN_P1R1_LOC16_REPLACE_VAL(reg_val, cfg->pattern2[16]);
    reg_val = CTWIN_P1R1_LOC17_REPLACE_VAL(reg_val, cfg->pattern2[17]);
    reg_val = CTWIN_P1R1_LOC18_REPLACE_VAL(reg_val, cfg->pattern2[18]);
    reg_val = CTWIN_P1R1_LOC19_REPLACE_VAL(reg_val, cfg->pattern2[19]);
    reg_val = CTWIN_P1R1_LOC20_REPLACE_VAL(reg_val, cfg->pattern2[20]);
    reg_val = CTWIN_P1R1_LOC21_REPLACE_VAL(reg_val, cfg->pattern2[21]);
    reg_val = CTWIN_P1R1_LOC22_REPLACE_VAL(reg_val, cfg->pattern2[22]);
    reg_val = CTWIN_P1R1_LOC23_REPLACE_VAL(reg_val, cfg->pattern2[23]);
    reg_val = CTWIN_P1R1_LOC24_REPLACE_VAL(reg_val, cfg->pattern2[24]);
    reg_val = CTWIN_P1R1_LOC25_REPLACE_VAL(reg_val, cfg->pattern2[25]);
    reg_val = CTWIN_P1R1_LOC26_REPLACE_VAL(reg_val, cfg->pattern2[26]);
    reg_val = CTWIN_P1R1_LOC27_REPLACE_VAL(reg_val, cfg->pattern2[27]);
    reg_val = CTWIN_P1R1_LOC28_REPLACE_VAL(reg_val, cfg->pattern2[28]);
    reg_val = CTWIN_P1R1_LOC29_REPLACE_VAL(reg_val, cfg->pattern2[29]);
    reg_val = CTWIN_P1R1_LOC30_REPLACE_VAL(reg_val, cfg->pattern2[30]);
    reg_val = CTWIN_P1R1_LOC31_REPLACE_VAL(reg_val, cfg->pattern2[31]);
    reg->P1R1 = reg_val;
 
    reg_val = reg->P1R2;
    reg_val = CTWIN_P1R2_LOC32_REPLACE_VAL(reg_val, cfg->pattern2[32]);
    reg_val = CTWIN_P1R2_LOC33_REPLACE_VAL(reg_val, cfg->pattern2[33]);
    reg_val = CTWIN_P1R2_LOC34_REPLACE_VAL(reg_val, cfg->pattern2[34]);
    reg_val = CTWIN_P1R2_LOC35_REPLACE_VAL(reg_val, cfg->pattern2[35]);
    reg_val = CTWIN_P1R2_LOC36_REPLACE_VAL(reg_val, cfg->pattern2[36]);
    reg_val = CTWIN_P1R2_LOC37_REPLACE_VAL(reg_val, cfg->pattern2[37]);
    reg_val = CTWIN_P1R2_LOC38_REPLACE_VAL(reg_val, cfg->pattern2[38]);
    reg_val = CTWIN_P1R2_LOC39_REPLACE_VAL(reg_val, cfg->pattern2[39]);
    reg_val = CTWIN_P1R2_LOC40_REPLACE_VAL(reg_val, cfg->pattern2[40]);
    reg_val = CTWIN_P1R2_LOC41_REPLACE_VAL(reg_val, cfg->pattern2[41]);
    reg_val = CTWIN_P1R2_LOC42_REPLACE_VAL(reg_val, cfg->pattern2[42]);
    reg_val = CTWIN_P1R2_LOC43_REPLACE_VAL(reg_val, cfg->pattern2[43]);
    reg_val = CTWIN_P1R2_LOC44_REPLACE_VAL(reg_val, cfg->pattern2[44]);
    reg_val = CTWIN_P1R2_LOC45_REPLACE_VAL(reg_val, cfg->pattern2[45]);
    reg_val = CTWIN_P1R2_LOC46_REPLACE_VAL(reg_val, cfg->pattern2[46]);
    reg_val = CTWIN_P1R2_LOC47_REPLACE_VAL(reg_val, cfg->pattern2[47]);
    reg->P1R2 = reg_val;
 
    reg_val = reg->P1R3;
    reg_val = CTWIN_P1R3_LOC48_REPLACE_VAL(reg_val, cfg->pattern2[48]);
    reg->P1R3 = reg_val;
 
    reg_val = reg->P2R0;
    reg_val = CTWIN_P2R0_LOC0_REPLACE_VAL(reg_val, cfg->pattern3[0]);
    reg_val = CTWIN_P2R0_LOC1_REPLACE_VAL(reg_val, cfg->pattern3[1]);
    reg_val = CTWIN_P2R0_LOC2_REPLACE_VAL(reg_val, cfg->pattern3[2]);
    reg_val = CTWIN_P2R0_LOC3_REPLACE_VAL(reg_val, cfg->pattern3[3]);
    reg_val = CTWIN_P2R0_LOC4_REPLACE_VAL(reg_val, cfg->pattern3[4]);
    reg_val = CTWIN_P2R0_LOC5_REPLACE_VAL(reg_val, cfg->pattern3[5]);
    reg_val = CTWIN_P2R0_LOC6_REPLACE_VAL(reg_val, cfg->pattern3[6]);
    reg_val = CTWIN_P2R0_LOC7_REPLACE_VAL(reg_val, cfg->pattern3[7]);
    reg_val = CTWIN_P2R0_LOC8_REPLACE_VAL(reg_val, cfg->pattern3[8]);
    reg_val = CTWIN_P2R0_LOC9_REPLACE_VAL(reg_val, cfg->pattern3[9]);
    reg_val = CTWIN_P2R0_LOC10_REPLACE_VAL(reg_val, cfg->pattern3[10]);
    reg_val = CTWIN_P2R0_LOC11_REPLACE_VAL(reg_val, cfg->pattern3[11]);
    reg_val = CTWIN_P2R0_LOC12_REPLACE_VAL(reg_val, cfg->pattern3[12]);
    reg_val = CTWIN_P2R0_LOC13_REPLACE_VAL(reg_val, cfg->pattern3[13]);
    reg_val = CTWIN_P2R0_LOC14_REPLACE_VAL(reg_val, cfg->pattern3[14]);
    reg_val = CTWIN_P2R0_LOC15_REPLACE_VAL(reg_val, cfg->pattern3[15]);
    reg->P2R0 = reg_val;
 
    reg_val = reg->P2R1;
    reg_val = CTWIN_P2R1_LOC16_REPLACE_VAL(reg_val, cfg->pattern3[16]);
    reg_val = CTWIN_P2R1_LOC17_REPLACE_VAL(reg_val, cfg->pattern3[17]);
    reg_val = CTWIN_P2R1_LOC18_REPLACE_VAL(reg_val, cfg->pattern3[18]);
    reg_val = CTWIN_P2R1_LOC19_REPLACE_VAL(reg_val, cfg->pattern3[19]);
    reg_val = CTWIN_P2R1_LOC20_REPLACE_VAL(reg_val, cfg->pattern3[20]);
    reg_val = CTWIN_P2R1_LOC21_REPLACE_VAL(reg_val, cfg->pattern3[21]);
    reg_val = CTWIN_P2R1_LOC22_REPLACE_VAL(reg_val, cfg->pattern3[22]);
    reg_val = CTWIN_P2R1_LOC23_REPLACE_VAL(reg_val, cfg->pattern3[23]);
    reg_val = CTWIN_P2R1_LOC24_REPLACE_VAL(reg_val, cfg->pattern3[24]);
    reg_val = CTWIN_P2R1_LOC25_REPLACE_VAL(reg_val, cfg->pattern3[25]);
    reg_val = CTWIN_P2R1_LOC26_REPLACE_VAL(reg_val, cfg->pattern3[26]);
    reg_val = CTWIN_P2R1_LOC27_REPLACE_VAL(reg_val, cfg->pattern3[27]);
    reg_val = CTWIN_P2R1_LOC28_REPLACE_VAL(reg_val, cfg->pattern3[28]);
    reg_val = CTWIN_P2R1_LOC29_REPLACE_VAL(reg_val, cfg->pattern3[29]);
    reg_val = CTWIN_P2R1_LOC30_REPLACE_VAL(reg_val, cfg->pattern3[30]);
    reg_val = CTWIN_P2R1_LOC31_REPLACE_VAL(reg_val, cfg->pattern3[31]);
    reg->P2R1 = reg_val;
 
    reg_val = reg->P2R2;
    reg_val = CTWIN_P2R2_LOC32_REPLACE_VAL(reg_val, cfg->pattern3[32]);
    reg_val = CTWIN_P2R2_LOC33_REPLACE_VAL(reg_val, cfg->pattern3[33]);
    reg_val = CTWIN_P2R2_LOC34_REPLACE_VAL(reg_val, cfg->pattern3[34]);
    reg_val = CTWIN_P2R2_LOC35_REPLACE_VAL(reg_val, cfg->pattern3[35]);
    reg_val = CTWIN_P2R2_LOC36_REPLACE_VAL(reg_val, cfg->pattern3[36]);
    reg_val = CTWIN_P2R2_LOC37_REPLACE_VAL(reg_val, cfg->pattern3[37]);
    reg_val = CTWIN_P2R2_LOC38_REPLACE_VAL(reg_val, cfg->pattern3[38]);
    reg_val = CTWIN_P2R2_LOC39_REPLACE_VAL(reg_val, cfg->pattern3[39]);
    reg_val = CTWIN_P2R2_LOC40_REPLACE_VAL(reg_val, cfg->pattern3[40]);
    reg_val = CTWIN_P2R2_LOC41_REPLACE_VAL(reg_val, cfg->pattern3[41]);
    reg_val = CTWIN_P2R2_LOC42_REPLACE_VAL(reg_val, cfg->pattern3[42]);
    reg_val = CTWIN_P2R2_LOC43_REPLACE_VAL(reg_val, cfg->pattern3[43]);
    reg_val = CTWIN_P2R2_LOC44_REPLACE_VAL(reg_val, cfg->pattern3[44]);
    reg_val = CTWIN_P2R2_LOC45_REPLACE_VAL(reg_val, cfg->pattern3[45]);
    reg_val = CTWIN_P2R2_LOC46_REPLACE_VAL(reg_val, cfg->pattern3[46]);
    reg_val = CTWIN_P2R2_LOC47_REPLACE_VAL(reg_val, cfg->pattern3[47]);
    reg->P2R2 = reg_val;
 
    reg_val = reg->P2R3;
    reg_val = CTWIN_P2R3_LOC48_REPLACE_VAL(reg_val, cfg->pattern3[48]);
    reg->P2R3 = reg_val;
 
    reg_val = reg->P3R0;
    reg_val = CTWIN_P3R0_LOC0_REPLACE_VAL(reg_val, cfg->pattern4[0]);
    reg_val = CTWIN_P3R0_LOC1_REPLACE_VAL(reg_val, cfg->pattern4[1]);
    reg_val = CTWIN_P3R0_LOC2_REPLACE_VAL(reg_val, cfg->pattern4[2]);
    reg_val = CTWIN_P3R0_LOC3_REPLACE_VAL(reg_val, cfg->pattern4[3]);
    reg_val = CTWIN_P3R0_LOC4_REPLACE_VAL(reg_val, cfg->pattern4[4]);
    reg_val = CTWIN_P3R0_LOC5_REPLACE_VAL(reg_val, cfg->pattern4[5]);
    reg_val = CTWIN_P3R0_LOC6_REPLACE_VAL(reg_val, cfg->pattern4[6]);
    reg_val = CTWIN_P3R0_LOC7_REPLACE_VAL(reg_val, cfg->pattern4[7]);
    reg_val = CTWIN_P3R0_LOC8_REPLACE_VAL(reg_val, cfg->pattern4[8]);
    reg_val = CTWIN_P3R0_LOC9_REPLACE_VAL(reg_val, cfg->pattern4[9]);
    reg_val = CTWIN_P3R0_LOC10_REPLACE_VAL(reg_val, cfg->pattern4[10]);
    reg_val = CTWIN_P3R0_LOC11_REPLACE_VAL(reg_val, cfg->pattern4[11]);
    reg_val = CTWIN_P3R0_LOC12_REPLACE_VAL(reg_val, cfg->pattern4[12]);
    reg_val = CTWIN_P3R0_LOC13_REPLACE_VAL(reg_val, cfg->pattern4[13]);
    reg_val = CTWIN_P3R0_LOC14_REPLACE_VAL(reg_val, cfg->pattern4[14]);
    reg_val = CTWIN_P3R0_LOC15_REPLACE_VAL(reg_val, cfg->pattern4[15]);
    reg->P3R0 = reg_val;
 
    reg_val = reg->P3R1;
    reg_val = CTWIN_P3R1_LOC16_REPLACE_VAL(reg_val, cfg->pattern4[16]);
    reg_val = CTWIN_P3R1_LOC17_REPLACE_VAL(reg_val, cfg->pattern4[17]);
    reg_val = CTWIN_P3R1_LOC18_REPLACE_VAL(reg_val, cfg->pattern4[18]);
    reg_val = CTWIN_P3R1_LOC19_REPLACE_VAL(reg_val, cfg->pattern4[19]);
    reg_val = CTWIN_P3R1_LOC20_REPLACE_VAL(reg_val, cfg->pattern4[20]);
    reg_val = CTWIN_P3R1_LOC21_REPLACE_VAL(reg_val, cfg->pattern4[21]);
    reg_val = CTWIN_P3R1_LOC22_REPLACE_VAL(reg_val, cfg->pattern4[22]);
    reg_val = CTWIN_P3R1_LOC23_REPLACE_VAL(reg_val, cfg->pattern4[23]);
    reg_val = CTWIN_P3R1_LOC24_REPLACE_VAL(reg_val, cfg->pattern4[24]);
    reg_val = CTWIN_P3R1_LOC25_REPLACE_VAL(reg_val, cfg->pattern4[25]);
    reg_val = CTWIN_P3R1_LOC26_REPLACE_VAL(reg_val, cfg->pattern4[26]);
    reg_val = CTWIN_P3R1_LOC27_REPLACE_VAL(reg_val, cfg->pattern4[27]);
    reg_val = CTWIN_P3R1_LOC28_REPLACE_VAL(reg_val, cfg->pattern4[28]);
    reg_val = CTWIN_P3R1_LOC29_REPLACE_VAL(reg_val, cfg->pattern4[29]);
    reg_val = CTWIN_P3R1_LOC30_REPLACE_VAL(reg_val, cfg->pattern4[30]);
    reg_val = CTWIN_P3R1_LOC31_REPLACE_VAL(reg_val, cfg->pattern4[31]);
    reg->P3R1 = reg_val;
 
    reg_val = reg->P3R2;
    reg_val = CTWIN_P3R2_LOC32_REPLACE_VAL(reg_val, cfg->pattern4[32]);
    reg_val = CTWIN_P3R2_LOC33_REPLACE_VAL(reg_val, cfg->pattern4[33]);
    reg_val = CTWIN_P3R2_LOC34_REPLACE_VAL(reg_val, cfg->pattern4[34]);
    reg_val = CTWIN_P3R2_LOC35_REPLACE_VAL(reg_val, cfg->pattern4[35]);
    reg_val = CTWIN_P3R2_LOC36_REPLACE_VAL(reg_val, cfg->pattern4[36]);
    reg_val = CTWIN_P3R2_LOC37_REPLACE_VAL(reg_val, cfg->pattern4[37]);
    reg_val = CTWIN_P3R2_LOC38_REPLACE_VAL(reg_val, cfg->pattern4[38]);
    reg_val = CTWIN_P3R2_LOC39_REPLACE_VAL(reg_val, cfg->pattern4[39]);
    reg_val = CTWIN_P3R2_LOC40_REPLACE_VAL(reg_val, cfg->pattern4[40]);
    reg_val = CTWIN_P3R2_LOC41_REPLACE_VAL(reg_val, cfg->pattern4[41]);
    reg_val = CTWIN_P3R2_LOC42_REPLACE_VAL(reg_val, cfg->pattern4[42]);
    reg_val = CTWIN_P3R2_LOC43_REPLACE_VAL(reg_val, cfg->pattern4[43]);
    reg_val = CTWIN_P3R2_LOC44_REPLACE_VAL(reg_val, cfg->pattern4[44]);
    reg_val = CTWIN_P3R2_LOC45_REPLACE_VAL(reg_val, cfg->pattern4[45]);
    reg_val = CTWIN_P3R2_LOC46_REPLACE_VAL(reg_val, cfg->pattern4[46]);
    reg_val = CTWIN_P3R2_LOC47_REPLACE_VAL(reg_val, cfg->pattern4[47]);
    reg->P3R2 = reg_val;
 
    reg_val = reg->P3R3;
    reg_val = CTWIN_P3R3_LOC48_REPLACE_VAL(reg_val, cfg->pattern4[48]);
    reg->P3R3 = reg_val;

}


