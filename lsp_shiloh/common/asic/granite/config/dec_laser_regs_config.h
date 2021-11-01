/*
 * ======================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

#ifndef _DEC_LASER_REGS_CONFIG_H
#define _DEC_LASER_REGS_CONFIG_H

#include "asic.h"

#ifdef __cplusplus
extern "C" {
#endif

DEC_CONFIG_REGS_REGS_t* dec_laser_get_dec_config_regs_base();
APB_CONFIG_REGS_t* dec_laser_open_apb_config_regs();
void dec_laser_close_apb_config_regs();
WAVEGEN_REGS_t* dec_laser_get_wavegen_regs(uint32_t index);
WFG0_REGS_t* dec_laser_get_lscan_wfg_regs(uint32_t index);
MIRROR0_REGS_t* dec_laser_get_lscan_mirror_regs(uint32_t index);
WFS_REGS_t* dec_laser_get_lscan_wfs_regs();
HSYNC_REGS_t* dec_laser_get_lscan_hsync_regs();
HIPWM0_REGS_t* dec_laser_open_hipwm_regs(uint32_t index);
void dec_laser_close_hipwm_regs(uint32_t index);

#ifdef __cplusplus
}
#endif

#endif //_DEC_LASER_REGS_CONFIG_H
