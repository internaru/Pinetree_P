/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the YIPI block config.
  * 
 **/

#include "yipi_cfg.h"

yipi_cfg_t yipi_bypass_cfg = 
{
    .up0_chans = 7,
    .up0_height = 8,
    .up0_width = 8,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .echo_config = 0x0,
    .bypass = 0x1,
    .ch_sel = 0x0,
    .serpentine_data = 0x0,
    .serp_start_dir = 0,
    .duplicate = 0,
    .h_space = 1,
    .v_space = 1,
    .func_sel = {0, 0, 0, 0,},
    .repl_pixel = {15, 15, 15, 15},
    .ref_array = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,}
};

yipi_cfg_t yipi_basic_cfg = 
{
    .up0_chans = 7,
    .up0_height = 8,
    .up0_width = 8,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .echo_config = 0x0,
    .bypass = 0x1,
    .ch_sel = 0x0,
    .serpentine_data = 0x0,
    .serp_start_dir = 0,
    .duplicate = 0,
    .h_space = 1,
    .v_space = 1,
    .func_sel = {3, 3, 3, 3},
    .repl_pixel = {15, 15, 15, 15},
    .ref_array = {0xc0c0, 0xc0c0, 0xc0c0, 0xc0c0, 0xc0c0, 0xc0c0, 0xc0c0, 0xc0c0}
};

yipi_cfg_t yipi_testpip2_cfg = 
{
    .up0_chans = 7,
    .up0_height = 40,
    .up0_width = 512,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .echo_config = 0x1,
    .bypass = 0x0,
    .ch_sel = 0x0,
    .serpentine_data = 0x1,
    .serp_start_dir = 1,
    .duplicate = 3,
    .h_space = 1,
    .v_space = 1,
    .func_sel = {1, 1, 1, 1},
    .repl_pixel = {2, 2, 2, 2},
    .ref_array = {0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa}
};

yipi_cfg_t yipi_testpip2_a_cfg = 
{
    .up0_chans = 7,
    .up0_height = 40,
    .up0_width = 512,
    .write_to_dv = 0x1,
    .write_to_hex = 0x1,
    .echo_config = 0x1,
    .bypass = 0x0,
    .ch_sel = 0x0,
    .serpentine_data = 0x1,
    .serp_start_dir = 1,
    .duplicate = 3,
    .h_space = 1,
    .v_space = 1,
    .func_sel = {1, 1, 1, 1},
    .repl_pixel = {2, 2, 2, 2},
    .ref_array = {0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa}
};

yipi_cfg_t yipi_test_cfg = 
{
    .up0_chans = 7,
    .up0_height = 8,
    .up0_width = 8,
    .write_to_dv = 0x0,
    .write_to_hex = 0x0,
    .echo_config = 0x0,
    .bypass = 0x0,
    .ch_sel = 0x1,
    .serpentine_data = 0x1,
    .serp_start_dir = 0,
    .duplicate = 3,
    .h_space = 4,
    .v_space = 4,
    .func_sel = {1, 1, 1, 1},
    .repl_pixel = {0, 0, 0, 0},
    .ref_array = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
};
