/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the Pipeline Access block.
  * 
 **/

#include "pipe_access_cfg.h"
#include "asic.h"

#ifndef PIPE_ACCESS_FUNC_H
#define PIPE_ACCESS_FUNC_H

#ifndef PIPE_ACCESS_LUT_DEPTH
#define PIPE_ACCESS_LUT_DEPTH 1024
#endif

//********************************
// Pipe Access Helper Function Prototypes
//********************************
void pipe_access_cfg_driver(PIPE_ACCESS_REGS_t *pa_reg, pipe_access_cfg_t *pa_cfg);
void pipe_access_setup_luts(PIPE_ACCESS_REGS_t *pa_reg,
                            uint32_t* lut0_data,
                            uint32_t* lut1_data,
                            uint32_t* lut2_data,
                            uint32_t* lut3_data,
                            uint32_t* lut4_data,
                            uint32_t* lut5_data);
void pipe_access_bypass(PIPE_ACCESS_REGS_t *pa_reg);
void pipe_access_inv_lut(PIPE_ACCESS_REGS_t *pa_reg);
void pipe_access_lut_load(PIPE_ACCESS_REGS_t *pa_reg, 
                          uint32_t  lut_num, 
                          uint32_t* data_ptr);
void pipe_access_lut_check(PIPE_ACCESS_REGS_t *pa_reg, 
                           uint32_t  lut_num, 
                           uint32_t* data_ptr);
void pipe_access_stats_en(PIPE_ACCESS_REGS_t *pa_reg,
                          uint32_t image_width,
                          uint32_t stats_patch_height,
                          uint32_t thresh0,
                          uint32_t thresh1,
                          uint32_t thresh2,
                          uint32_t thresh3,
                          uint32_t thresh4,
                          uint32_t thresh5);
void pipe_access_store_4(PIPE_ACCESS_REGS_t *pa_reg);
void pipe_access_load_4(PIPE_ACCESS_REGS_t *pa_reg);
void pipe_access_store_6(PIPE_ACCESS_REGS_t *pa_reg);
void pipe_access_load_6(PIPE_ACCESS_REGS_t *pa_reg);
void pipe_access_lut_en(PIPE_ACCESS_REGS_t *pa_reg);
void pipe_access_lut_disable(PIPE_ACCESS_REGS_t *pa_reg);
#if 0 // lsptodo cdma
uint32_t pipe_access_regtest(PIPE_ACCESS_REGS_t *pa_reg);
void pa_odma_setup(PA_ODMA_REGS_t *odma_reg, uint32_t desc_addr, uint32_t data_addr, uint32_t xferCnt);
error_type_t pa_stat_cfg_buffer(uint32_t *buf, uint32_t bufsize, uint32_t *srcaddr, uint32_t perfid);
void pa_stat_cdma_close(void);
#endif
#endif // PIPE_ACCESS_FUNC_H
