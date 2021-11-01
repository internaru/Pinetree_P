/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the CTWIN block config.
  * 
 **/

#ifndef CTWIN_CFG_H
#define CTWIN_CFG_H

#include <stdint.h>


typedef struct ctwin_cfg_s
{
    uint32_t bypass;
    uint32_t line_width;
    uint32_t serp;
    uint32_t lb_sram_access_enable;
    uint32_t save_restore_mode;
    uint32_t kt_text_mode;
    uint32_t kt_merge_func;
    uint32_t kt_merge_val[5];
 
    uint32_t win1_thresh0[6];
    uint32_t win1_thresh1[6];
    uint32_t win1_thresh2[6];
    uint32_t win1_input_sel;
    uint32_t win1_input_func;
    uint32_t win1_input_inv;
    uint32_t win1_pat_sel;
    uint32_t win1_pat_func;
    uint32_t win1_pat_inv;
 
    uint32_t win2_thresh0[6];
    uint32_t win2_thresh1[6];
    uint32_t win2_thresh2[6];
    uint32_t win2_input_sel;
    uint32_t win2_input_func;
    uint32_t win2_input_inv;
    uint32_t win2_pat_sel;
    uint32_t win2_pat_func;
    uint32_t win2_pat_inv;
 
    uint32_t pattern1[49];
    uint32_t pattern2[49];
    uint32_t pattern3[49];
    uint32_t pattern4[49];
 
    uint32_t out_win_sel[6];
    uint32_t out_win_func;
    uint32_t out_mode[6];
    uint32_t out_val[6];
    uint32_t edge_pat_sel;
    uint32_t edge_sc_oneaway[6];
    uint32_t edge_sc_twoaway[6];
    uint32_t edge_sc_threeaway[6];
    uint32_t edge_th_oneaway[6];
    uint32_t edge_th_twoaway[6];
    uint32_t edge_th_threeaway[6];
} ctwin_cfg_t;

extern ctwin_cfg_t ctwin_bypass_cfg;
extern ctwin_cfg_t ctwin_generic_cfg;
extern ctwin_cfg_t ctwin_bleed_cfg;
extern ctwin_cfg_t ctwin_test_cfg;

#endif
