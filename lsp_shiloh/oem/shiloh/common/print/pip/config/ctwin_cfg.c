/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains CTWIN block configuration data.
  * 
 **/

#include "ctwin_cfg.h"

ctwin_cfg_t ctwin_bypass_cfg =
{
    .bypass = 1
};

ctwin_cfg_t ctwin_generic_cfg =
{
    .bypass = 0,
    .line_width = 0,    // set at runtime
    .serp = 0,
    .lb_sram_access_enable = 0,
    .save_restore_mode = 0,
    .kt_text_mode = 1,
    .kt_merge_func = 0,
    .kt_merge_val = {0, 256, 512, 768, 1023},
    .win1_thresh0 = {1, 1, 1, 1, 1, 1023},
    .win1_thresh1 = {1, 1, 1, 1, 1, 1023},
    .win1_thresh2 = {1, 1, 1, 1, 1, 1023},
    .win1_input_sel = 0x1F,
    .win1_input_func = 0,
    .win1_input_inv = 0x0,
    .win1_pat_sel = 0x2,
    .win1_pat_func = 0,
    .win1_pat_inv = 0x2,
    .win2_thresh0 = {1, 1, 1, 1, 50, 0},
    .win2_thresh1 = {1, 1, 1, 1, 700, 0},
    .win2_thresh2 = {1, 1, 1, 1, 700, 0},
    .win2_input_sel = 0x0,
    .win2_input_func = 0,
    .win2_input_inv = 0,
    .win2_pat_sel = 0,
    .win2_pat_func = 0,
    .win2_pat_inv = 0,
    .pattern1 = {1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1},
    .pattern2 = {3, 3, 3, 3, 3, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                 3, 3, 0, 0, 0, 3, 3,
                 3, 3, 0, 0, 0, 3, 3,
                 3, 3, 0, 0, 0, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                 3, 3, 3, 3, 3, 3, 3},
    .pattern3 = {3, 3, 3, 3, 3, 3, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 3, 3, 3, 3, 3, 3},
    .pattern4 = {3, 3, 3, 3, 3, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                 3, 3, 3, 3, 3, 3, 3},
    .out_win_sel = {0, 0, 0, 0, 0, 1},
    .out_win_func = 0x0,
    .out_mode = {0, 0, 0, 0, 0, 2},
    .out_val = {10, 20, 30, 25, 512, 600},
     .edge_pat_sel = 0x0,
    .edge_sc_oneaway = {307, 256, 154, 358, 0, 0},
    .edge_sc_twoaway = {348, 307, 225, 384, 0, 0},
    .edge_sc_threeaway = {430, 410, 368, 451, 0, 0},
    .edge_th_oneaway = {410, 307, 205, 80, 0, 0},
    .edge_th_twoaway = {614, 512, 307, 90, 0, 0},
    .edge_th_threeaway = {768, 670, 512, 100, 0, 0}
};

ctwin_cfg_t ctwin_bleed_cfg =
{
    .bypass = 0,
    .line_width = 0,   // set at runtime
    .serp = 0,
    .lb_sram_access_enable = 0,
    .save_restore_mode = 0,
    .kt_text_mode = 0,
    .kt_merge_func = 0,
    .kt_merge_val = {0, 1023, 1023, 1023, 1023},
    .win1_thresh0 = {80, 80, 80, 1023, 256, 1023},
    .win1_thresh1 = {40, 40, 40, 1023, 512, 1023},
    .win1_thresh2 = {20, 20, 20, 1023, 650, 1023},
    .win1_input_sel = 0x17,
    .win1_input_func = 1,
    .win1_input_inv = 0x07,
    .win1_pat_sel = 0x0,
    .win1_pat_func = 1,
    .win1_pat_inv = 0x0,
    .win2_thresh0 = {1, 1, 1, 1, 50, 0},
    .win2_thresh1 = {1, 1, 1, 1, 700, 0},
    .win2_thresh2 = {1, 1, 1, 1, 700, 0},
    .win2_input_sel = 0x10,
    .win2_input_func = 0,
    .win2_input_inv = 0x00,
    .win2_pat_sel = 0x0,
    .win2_pat_func = 0,
    .win2_pat_inv = 0x0,
    .pattern1 = {1, 1, 1, 1, 1, 1, 1,
	             1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1},
    .pattern2 = {0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0},
    .pattern3 = {3, 3, 3, 3, 3, 3, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 3, 3, 3, 3, 3, 3},
    .pattern4 = {3, 3, 3, 3, 3, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                3, 3, 3, 3, 3, 3, 3},
    .out_win_sel = {1, 1, 1, 1, 0, 0},
    .out_win_func = 0x00,
    .out_mode = {1, 1, 1, 1, 1, 1},
    .out_val = {0, 0, 0, 0, 512, 0},
    .edge_pat_sel = 0x1,
    .edge_sc_oneaway = {307, 256, 154, 358, 0, 0},
    .edge_sc_twoaway = {348, 307, 225, 384, 0, 0},
    .edge_sc_threeaway = {430, 410, 368, 451, 0, 0},
    .edge_th_oneaway = {410, 307, 205, 80, 0, 0},
    .edge_th_twoaway = {614, 512, 307, 90, 0, 0},
    .edge_th_threeaway = {768, 670, 512, 100, 0, 0}
};

ctwin_cfg_t ctwin_test_cfg =
{
    .bypass = 0,
    .line_width = 0,   // set at runtime
    .serp = 0,
    .lb_sram_access_enable = 0,
    .save_restore_mode = 0,
    .kt_text_mode = 0,
    .kt_merge_func = 0,
    .kt_merge_val = {0, 1023, 1023, 1023, 1023},
    .win1_thresh0 = {80, 80, 80, 1023, 256, 1023},
    .win1_thresh1 = {40, 40, 40, 1023, 512, 1023},
    .win1_thresh2 = {20, 20, 20, 1023, 650, 1023},
    .win1_input_sel = 0x17,
    .win1_input_func = 1,
    .win1_input_inv = 0x07,
    .win1_pat_sel = 0x0,
    .win1_pat_func = 1,
    .win1_pat_inv = 0x0,
    .win2_thresh0 = {1, 1, 1, 1, 20, 0},
    .win2_thresh1 = {1, 1, 1, 1, 180, 0},
    .win2_thresh2 = {1, 1, 1, 1, 180, 0},
    .win2_input_sel = 0x10,
    .win2_input_func = 0,
    .win2_input_inv = 0x00,
    .win2_pat_sel = 0x1,
    .win2_pat_func = 1,
    .win2_pat_inv = 0x0,
    .pattern1 = {3, 1, 1, 1, 1, 1, 3,
 	             1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 1, 1, 1, 1, 1, 1, 1,
                 3, 1, 1, 1, 1, 1, 3},
    .pattern2 = {0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0},
    .pattern3 = {3, 3, 3, 3, 3, 3, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 1, 1, 1, 1, 1, 3,
                 3, 3, 3, 3, 3, 3, 3},
    .pattern4 = {3, 3, 3, 3, 3, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 1, 1, 1, 3, 3,
                 3, 3, 3, 3, 3, 3, 3,
                 3, 3, 3, 3, 3, 3, 3},
    .out_win_sel = {0, 0, 0, 0, 2, 0},
    .out_win_func = 0x00,
    .out_mode = {1, 1, 1, 1, 0, 1},
    .out_val = {256, 256, 256, 256, 0, 0},
    .edge_pat_sel = 0x0,
    .edge_sc_oneaway = {307, 256, 154, 358, 0, 0},
    .edge_sc_twoaway = {348, 307, 225, 384, 0, 0},
    .edge_sc_threeaway = {430, 410, 368, 451, 0, 0},
    .edge_th_oneaway = {410, 307, 205, 80, 0, 0},
    .edge_th_twoaway = {614, 512, 307, 90, 0, 0},
    .edge_th_threeaway = {768, 670, 512, 100, 0, 0}
};

