 /*
 **************************************************************************************
 *
 * Copyright (c) 2010 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
 *
 * \file DPWM_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DPWM_TOP_REGS_REGMASKS_H_
#define _DPWM_TOP_REGS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: dpwm_top_regs (dpwm_top_regs)
//
//====================================================================

//====================================================================
//Register: PWM Enable register (en)
//====================================================================

#define  DPWM_TOP_REGS_EN_ENABLE_MASK 0xffffffff
#define  DPWM_TOP_REGS_EN_ENABLE_SHIFT 0
#define  DPWM_TOP_REGS_EN_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_TOP_REGS_EN_ENABLE_MASK) >> DPWM_TOP_REGS_EN_ENABLE_SHIFT)
#define  DPWM_TOP_REGS_EN_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_TOP_REGS_EN_ENABLE_MASK) | (((uint32_t)val) << DPWM_TOP_REGS_EN_ENABLE_SHIFT))

//====================================================================
//Register: PWM Interrupt Pending register (intp)
//====================================================================

#define  DPWM_TOP_REGS_INTP_INTP_MASK 0xffffffff
#define  DPWM_TOP_REGS_INTP_INTP_SHIFT 0
#define  DPWM_TOP_REGS_INTP_INTP_MASK_SHIFT(reg) (((reg) & DPWM_TOP_REGS_INTP_INTP_MASK) >> DPWM_TOP_REGS_INTP_INTP_SHIFT)
#define  DPWM_TOP_REGS_INTP_INTP_REPLACE_VAL(reg,val) (((reg) & ~DPWM_TOP_REGS_INTP_INTP_MASK) | (((uint32_t)val) << DPWM_TOP_REGS_INTP_INTP_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 1 of 32) (dpwm_regs0)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS0_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS0_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS0_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_CFG_RESERVED1_MASK) >> DPWM_REGS0_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS0_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS0_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS0_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS0_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS0_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS0_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS0_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS0_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS0_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS0_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_CFG_HIGH_LOW_MASK) >> DPWM_REGS0_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS0_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS0_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS0_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS0_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS0_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_CFG_OFF_STATE_MASK) >> DPWM_REGS0_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS0_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS0_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS0_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS0_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS0_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS0_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS0_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS0_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS0_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS0_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS0_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_CFG_ENABLE_MASK) >> DPWM_REGS0_CFG_ENABLE_SHIFT)
#define  DPWM_REGS0_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS0_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS0_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS0_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS0_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_COUNT0_RESERVED1_MASK) >> DPWM_REGS0_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS0_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS0_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS0_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS0_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_COUNT0_COUNT0_MASK) >> DPWM_REGS0_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS0_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS0_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS0_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS0_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS0_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_COUNT1_RESERVED1_MASK) >> DPWM_REGS0_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS0_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS0_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS0_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS0_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_COUNT1_COUNT1_MASK) >> DPWM_REGS0_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS0_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS0_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS0_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS0_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS0_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_COUNT2_RESERVED1_MASK) >> DPWM_REGS0_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS0_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS0_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS0_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS0_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_COUNT2_COUNT2_MASK) >> DPWM_REGS0_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS0_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS0_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS0_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS0_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS0_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_EN_RESERVED1_MASK) >> DPWM_REGS0_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS0_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS0_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS0_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS0_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS0_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS0_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS0_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS0_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS0_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS0_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS0_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS0_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS0_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS0_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS0_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS0_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS0_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS0_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS0_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_ST_RESERVED1_MASK) >> DPWM_REGS0_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS0_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS0_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS0_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS0_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS0_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS0_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS0_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS0_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS0_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS0_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS0_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS0_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS0_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS0_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS0_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS0_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS0_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS0_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS0_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_CL_RESERVED1_MASK) >> DPWM_REGS0_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS0_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS0_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS0_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS0_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS0_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS0_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS0_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS0_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS0_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS0_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS0_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS0_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS0_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS0_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS0_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS0_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS0_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS0_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS0_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_FO_RESERVED1_MASK) >> DPWM_REGS0_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS0_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS0_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS0_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS0_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS0_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS0_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS0_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS0_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS0_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS0_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS0_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS0_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS0_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS0_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS0_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS0_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS0_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS0_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS0_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS0_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_STATUS_RUNNING_MASK) >> DPWM_REGS0_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS0_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS0_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS0_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS0_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS0_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_STATUS_DELAY_MASK) >> DPWM_REGS0_STATUS_DELAY_SHIFT)
#define  DPWM_REGS0_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS0_STATUS_DELAY_SHIFT))

#define  DPWM_REGS0_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS0_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS0_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_STATUS_SEMI_NUM_MASK) >> DPWM_REGS0_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS0_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS0_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS0_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS0_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS0_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_STATUS_PWM_OUT_MASK) >> DPWM_REGS0_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS0_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS0_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS0_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS0_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS0_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_STATUS_RESERVED1_MASK) >> DPWM_REGS0_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS0_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS0_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS0_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS0_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS0_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS0_STATUS_CUR_COUNT_MASK) >> DPWM_REGS0_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS0_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS0_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS0_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 2 of 32) (dpwm_regs1)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS1_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS1_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS1_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_CFG_RESERVED1_MASK) >> DPWM_REGS1_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS1_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS1_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS1_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS1_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS1_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS1_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS1_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS1_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS1_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS1_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_CFG_HIGH_LOW_MASK) >> DPWM_REGS1_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS1_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS1_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS1_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS1_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS1_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_CFG_OFF_STATE_MASK) >> DPWM_REGS1_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS1_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS1_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS1_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS1_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS1_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS1_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS1_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS1_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS1_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS1_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS1_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_CFG_ENABLE_MASK) >> DPWM_REGS1_CFG_ENABLE_SHIFT)
#define  DPWM_REGS1_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS1_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS1_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS1_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS1_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_COUNT0_RESERVED1_MASK) >> DPWM_REGS1_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS1_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS1_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS1_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS1_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_COUNT0_COUNT0_MASK) >> DPWM_REGS1_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS1_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS1_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS1_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS1_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS1_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_COUNT1_RESERVED1_MASK) >> DPWM_REGS1_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS1_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS1_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS1_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS1_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_COUNT1_COUNT1_MASK) >> DPWM_REGS1_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS1_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS1_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS1_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS1_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS1_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_COUNT2_RESERVED1_MASK) >> DPWM_REGS1_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS1_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS1_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS1_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS1_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_COUNT2_COUNT2_MASK) >> DPWM_REGS1_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS1_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS1_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS1_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS1_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS1_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_EN_RESERVED1_MASK) >> DPWM_REGS1_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS1_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS1_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS1_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS1_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS1_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS1_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS1_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS1_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS1_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS1_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS1_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS1_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS1_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS1_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS1_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS1_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS1_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS1_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS1_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_ST_RESERVED1_MASK) >> DPWM_REGS1_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS1_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS1_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS1_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS1_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS1_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS1_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS1_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS1_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS1_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS1_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS1_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS1_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS1_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS1_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS1_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS1_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS1_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS1_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS1_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_CL_RESERVED1_MASK) >> DPWM_REGS1_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS1_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS1_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS1_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS1_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS1_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS1_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS1_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS1_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS1_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS1_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS1_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS1_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS1_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS1_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS1_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS1_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS1_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS1_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS1_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_FO_RESERVED1_MASK) >> DPWM_REGS1_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS1_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS1_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS1_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS1_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS1_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS1_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS1_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS1_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS1_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS1_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS1_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS1_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS1_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS1_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS1_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS1_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS1_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS1_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS1_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS1_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_STATUS_RUNNING_MASK) >> DPWM_REGS1_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS1_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS1_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS1_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS1_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS1_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_STATUS_DELAY_MASK) >> DPWM_REGS1_STATUS_DELAY_SHIFT)
#define  DPWM_REGS1_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS1_STATUS_DELAY_SHIFT))

#define  DPWM_REGS1_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS1_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS1_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_STATUS_SEMI_NUM_MASK) >> DPWM_REGS1_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS1_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS1_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS1_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS1_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS1_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_STATUS_PWM_OUT_MASK) >> DPWM_REGS1_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS1_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS1_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS1_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS1_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS1_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_STATUS_RESERVED1_MASK) >> DPWM_REGS1_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS1_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS1_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS1_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS1_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS1_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS1_STATUS_CUR_COUNT_MASK) >> DPWM_REGS1_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS1_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS1_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS1_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 3 of 32) (dpwm_regs2)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS2_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS2_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS2_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_CFG_RESERVED1_MASK) >> DPWM_REGS2_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS2_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS2_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS2_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS2_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS2_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS2_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS2_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS2_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS2_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS2_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_CFG_HIGH_LOW_MASK) >> DPWM_REGS2_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS2_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS2_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS2_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS2_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS2_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_CFG_OFF_STATE_MASK) >> DPWM_REGS2_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS2_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS2_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS2_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS2_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS2_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS2_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS2_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS2_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS2_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS2_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS2_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_CFG_ENABLE_MASK) >> DPWM_REGS2_CFG_ENABLE_SHIFT)
#define  DPWM_REGS2_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS2_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS2_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS2_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS2_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_COUNT0_RESERVED1_MASK) >> DPWM_REGS2_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS2_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS2_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS2_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS2_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_COUNT0_COUNT0_MASK) >> DPWM_REGS2_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS2_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS2_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS2_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS2_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS2_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_COUNT1_RESERVED1_MASK) >> DPWM_REGS2_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS2_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS2_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS2_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS2_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_COUNT1_COUNT1_MASK) >> DPWM_REGS2_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS2_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS2_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS2_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS2_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS2_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_COUNT2_RESERVED1_MASK) >> DPWM_REGS2_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS2_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS2_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS2_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS2_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_COUNT2_COUNT2_MASK) >> DPWM_REGS2_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS2_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS2_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS2_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS2_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS2_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_EN_RESERVED1_MASK) >> DPWM_REGS2_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS2_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS2_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS2_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS2_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS2_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS2_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS2_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS2_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS2_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS2_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS2_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS2_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS2_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS2_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS2_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS2_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS2_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS2_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS2_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_ST_RESERVED1_MASK) >> DPWM_REGS2_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS2_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS2_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS2_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS2_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS2_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS2_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS2_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS2_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS2_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS2_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS2_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS2_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS2_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS2_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS2_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS2_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS2_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS2_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS2_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_CL_RESERVED1_MASK) >> DPWM_REGS2_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS2_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS2_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS2_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS2_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS2_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS2_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS2_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS2_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS2_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS2_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS2_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS2_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS2_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS2_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS2_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS2_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS2_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS2_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS2_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_FO_RESERVED1_MASK) >> DPWM_REGS2_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS2_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS2_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS2_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS2_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS2_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS2_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS2_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS2_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS2_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS2_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS2_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS2_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS2_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS2_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS2_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS2_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS2_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS2_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS2_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS2_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_STATUS_RUNNING_MASK) >> DPWM_REGS2_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS2_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS2_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS2_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS2_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS2_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_STATUS_DELAY_MASK) >> DPWM_REGS2_STATUS_DELAY_SHIFT)
#define  DPWM_REGS2_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS2_STATUS_DELAY_SHIFT))

#define  DPWM_REGS2_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS2_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS2_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_STATUS_SEMI_NUM_MASK) >> DPWM_REGS2_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS2_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS2_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS2_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS2_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS2_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_STATUS_PWM_OUT_MASK) >> DPWM_REGS2_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS2_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS2_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS2_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS2_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS2_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_STATUS_RESERVED1_MASK) >> DPWM_REGS2_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS2_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS2_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS2_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS2_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS2_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS2_STATUS_CUR_COUNT_MASK) >> DPWM_REGS2_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS2_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS2_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS2_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 4 of 32) (dpwm_regs3)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS3_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS3_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS3_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_CFG_RESERVED1_MASK) >> DPWM_REGS3_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS3_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS3_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS3_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS3_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS3_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS3_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS3_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS3_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS3_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS3_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_CFG_HIGH_LOW_MASK) >> DPWM_REGS3_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS3_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS3_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS3_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS3_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS3_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_CFG_OFF_STATE_MASK) >> DPWM_REGS3_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS3_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS3_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS3_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS3_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS3_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS3_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS3_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS3_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS3_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS3_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS3_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_CFG_ENABLE_MASK) >> DPWM_REGS3_CFG_ENABLE_SHIFT)
#define  DPWM_REGS3_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS3_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS3_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS3_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS3_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_COUNT0_RESERVED1_MASK) >> DPWM_REGS3_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS3_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS3_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS3_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS3_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_COUNT0_COUNT0_MASK) >> DPWM_REGS3_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS3_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS3_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS3_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS3_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS3_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_COUNT1_RESERVED1_MASK) >> DPWM_REGS3_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS3_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS3_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS3_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS3_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_COUNT1_COUNT1_MASK) >> DPWM_REGS3_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS3_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS3_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS3_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS3_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS3_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_COUNT2_RESERVED1_MASK) >> DPWM_REGS3_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS3_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS3_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS3_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS3_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_COUNT2_COUNT2_MASK) >> DPWM_REGS3_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS3_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS3_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS3_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS3_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS3_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_EN_RESERVED1_MASK) >> DPWM_REGS3_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS3_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS3_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS3_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS3_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS3_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS3_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS3_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS3_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS3_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS3_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS3_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS3_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS3_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS3_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS3_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS3_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS3_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS3_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS3_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_ST_RESERVED1_MASK) >> DPWM_REGS3_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS3_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS3_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS3_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS3_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS3_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS3_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS3_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS3_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS3_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS3_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS3_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS3_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS3_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS3_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS3_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS3_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS3_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS3_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS3_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_CL_RESERVED1_MASK) >> DPWM_REGS3_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS3_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS3_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS3_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS3_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS3_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS3_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS3_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS3_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS3_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS3_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS3_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS3_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS3_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS3_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS3_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS3_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS3_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS3_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS3_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_FO_RESERVED1_MASK) >> DPWM_REGS3_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS3_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS3_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS3_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS3_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS3_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS3_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS3_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS3_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS3_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS3_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS3_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS3_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS3_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS3_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS3_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS3_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS3_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS3_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS3_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS3_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_STATUS_RUNNING_MASK) >> DPWM_REGS3_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS3_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS3_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS3_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS3_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS3_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_STATUS_DELAY_MASK) >> DPWM_REGS3_STATUS_DELAY_SHIFT)
#define  DPWM_REGS3_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS3_STATUS_DELAY_SHIFT))

#define  DPWM_REGS3_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS3_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS3_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_STATUS_SEMI_NUM_MASK) >> DPWM_REGS3_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS3_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS3_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS3_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS3_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS3_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_STATUS_PWM_OUT_MASK) >> DPWM_REGS3_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS3_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS3_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS3_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS3_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS3_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_STATUS_RESERVED1_MASK) >> DPWM_REGS3_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS3_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS3_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS3_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS3_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS3_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS3_STATUS_CUR_COUNT_MASK) >> DPWM_REGS3_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS3_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS3_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS3_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 5 of 32) (dpwm_regs4)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS4_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS4_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS4_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_CFG_RESERVED1_MASK) >> DPWM_REGS4_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS4_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS4_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS4_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS4_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS4_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS4_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS4_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS4_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS4_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS4_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_CFG_HIGH_LOW_MASK) >> DPWM_REGS4_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS4_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS4_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS4_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS4_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS4_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_CFG_OFF_STATE_MASK) >> DPWM_REGS4_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS4_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS4_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS4_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS4_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS4_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS4_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS4_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS4_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS4_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS4_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS4_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_CFG_ENABLE_MASK) >> DPWM_REGS4_CFG_ENABLE_SHIFT)
#define  DPWM_REGS4_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS4_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS4_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS4_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS4_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_COUNT0_RESERVED1_MASK) >> DPWM_REGS4_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS4_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS4_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS4_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS4_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_COUNT0_COUNT0_MASK) >> DPWM_REGS4_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS4_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS4_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS4_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS4_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS4_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_COUNT1_RESERVED1_MASK) >> DPWM_REGS4_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS4_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS4_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS4_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS4_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_COUNT1_COUNT1_MASK) >> DPWM_REGS4_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS4_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS4_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS4_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS4_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS4_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_COUNT2_RESERVED1_MASK) >> DPWM_REGS4_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS4_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS4_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS4_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS4_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_COUNT2_COUNT2_MASK) >> DPWM_REGS4_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS4_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS4_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS4_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS4_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS4_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_EN_RESERVED1_MASK) >> DPWM_REGS4_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS4_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS4_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS4_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS4_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS4_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS4_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS4_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS4_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS4_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS4_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS4_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS4_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS4_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS4_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS4_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS4_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS4_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS4_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS4_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_ST_RESERVED1_MASK) >> DPWM_REGS4_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS4_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS4_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS4_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS4_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS4_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS4_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS4_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS4_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS4_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS4_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS4_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS4_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS4_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS4_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS4_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS4_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS4_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS4_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS4_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_CL_RESERVED1_MASK) >> DPWM_REGS4_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS4_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS4_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS4_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS4_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS4_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS4_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS4_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS4_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS4_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS4_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS4_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS4_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS4_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS4_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS4_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS4_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS4_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS4_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS4_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_FO_RESERVED1_MASK) >> DPWM_REGS4_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS4_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS4_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS4_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS4_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS4_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS4_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS4_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS4_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS4_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS4_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS4_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS4_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS4_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS4_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS4_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS4_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS4_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS4_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS4_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS4_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_STATUS_RUNNING_MASK) >> DPWM_REGS4_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS4_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS4_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS4_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS4_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS4_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_STATUS_DELAY_MASK) >> DPWM_REGS4_STATUS_DELAY_SHIFT)
#define  DPWM_REGS4_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS4_STATUS_DELAY_SHIFT))

#define  DPWM_REGS4_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS4_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS4_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_STATUS_SEMI_NUM_MASK) >> DPWM_REGS4_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS4_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS4_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS4_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS4_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS4_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_STATUS_PWM_OUT_MASK) >> DPWM_REGS4_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS4_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS4_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS4_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS4_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS4_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_STATUS_RESERVED1_MASK) >> DPWM_REGS4_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS4_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS4_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS4_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS4_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS4_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS4_STATUS_CUR_COUNT_MASK) >> DPWM_REGS4_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS4_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS4_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS4_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 6 of 32) (dpwm_regs5)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS5_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS5_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS5_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_CFG_RESERVED1_MASK) >> DPWM_REGS5_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS5_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS5_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS5_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS5_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS5_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS5_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS5_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS5_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS5_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS5_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_CFG_HIGH_LOW_MASK) >> DPWM_REGS5_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS5_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS5_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS5_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS5_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS5_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_CFG_OFF_STATE_MASK) >> DPWM_REGS5_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS5_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS5_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS5_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS5_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS5_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS5_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS5_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS5_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS5_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS5_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS5_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_CFG_ENABLE_MASK) >> DPWM_REGS5_CFG_ENABLE_SHIFT)
#define  DPWM_REGS5_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS5_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS5_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS5_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS5_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_COUNT0_RESERVED1_MASK) >> DPWM_REGS5_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS5_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS5_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS5_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS5_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_COUNT0_COUNT0_MASK) >> DPWM_REGS5_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS5_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS5_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS5_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS5_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS5_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_COUNT1_RESERVED1_MASK) >> DPWM_REGS5_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS5_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS5_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS5_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS5_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_COUNT1_COUNT1_MASK) >> DPWM_REGS5_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS5_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS5_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS5_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS5_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS5_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_COUNT2_RESERVED1_MASK) >> DPWM_REGS5_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS5_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS5_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS5_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS5_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_COUNT2_COUNT2_MASK) >> DPWM_REGS5_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS5_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS5_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS5_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS5_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS5_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_EN_RESERVED1_MASK) >> DPWM_REGS5_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS5_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS5_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS5_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS5_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS5_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS5_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS5_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS5_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS5_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS5_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS5_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS5_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS5_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS5_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS5_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS5_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS5_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS5_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS5_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_ST_RESERVED1_MASK) >> DPWM_REGS5_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS5_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS5_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS5_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS5_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS5_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS5_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS5_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS5_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS5_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS5_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS5_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS5_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS5_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS5_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS5_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS5_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS5_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS5_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS5_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_CL_RESERVED1_MASK) >> DPWM_REGS5_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS5_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS5_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS5_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS5_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS5_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS5_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS5_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS5_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS5_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS5_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS5_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS5_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS5_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS5_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS5_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS5_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS5_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS5_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS5_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_FO_RESERVED1_MASK) >> DPWM_REGS5_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS5_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS5_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS5_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS5_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS5_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS5_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS5_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS5_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS5_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS5_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS5_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS5_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS5_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS5_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS5_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS5_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS5_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS5_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS5_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS5_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_STATUS_RUNNING_MASK) >> DPWM_REGS5_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS5_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS5_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS5_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS5_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS5_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_STATUS_DELAY_MASK) >> DPWM_REGS5_STATUS_DELAY_SHIFT)
#define  DPWM_REGS5_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS5_STATUS_DELAY_SHIFT))

#define  DPWM_REGS5_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS5_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS5_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_STATUS_SEMI_NUM_MASK) >> DPWM_REGS5_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS5_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS5_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS5_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS5_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS5_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_STATUS_PWM_OUT_MASK) >> DPWM_REGS5_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS5_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS5_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS5_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS5_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS5_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_STATUS_RESERVED1_MASK) >> DPWM_REGS5_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS5_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS5_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS5_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS5_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS5_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS5_STATUS_CUR_COUNT_MASK) >> DPWM_REGS5_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS5_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS5_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS5_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 7 of 32) (dpwm_regs6)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS6_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS6_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS6_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_CFG_RESERVED1_MASK) >> DPWM_REGS6_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS6_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS6_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS6_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS6_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS6_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS6_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS6_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS6_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS6_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS6_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_CFG_HIGH_LOW_MASK) >> DPWM_REGS6_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS6_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS6_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS6_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS6_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS6_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_CFG_OFF_STATE_MASK) >> DPWM_REGS6_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS6_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS6_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS6_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS6_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS6_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS6_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS6_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS6_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS6_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS6_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS6_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_CFG_ENABLE_MASK) >> DPWM_REGS6_CFG_ENABLE_SHIFT)
#define  DPWM_REGS6_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS6_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS6_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS6_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS6_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_COUNT0_RESERVED1_MASK) >> DPWM_REGS6_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS6_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS6_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS6_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS6_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_COUNT0_COUNT0_MASK) >> DPWM_REGS6_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS6_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS6_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS6_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS6_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS6_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_COUNT1_RESERVED1_MASK) >> DPWM_REGS6_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS6_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS6_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS6_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS6_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_COUNT1_COUNT1_MASK) >> DPWM_REGS6_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS6_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS6_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS6_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS6_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS6_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_COUNT2_RESERVED1_MASK) >> DPWM_REGS6_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS6_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS6_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS6_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS6_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_COUNT2_COUNT2_MASK) >> DPWM_REGS6_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS6_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS6_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS6_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS6_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS6_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_EN_RESERVED1_MASK) >> DPWM_REGS6_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS6_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS6_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS6_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS6_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS6_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS6_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS6_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS6_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS6_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS6_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS6_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS6_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS6_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS6_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS6_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS6_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS6_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS6_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS6_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_ST_RESERVED1_MASK) >> DPWM_REGS6_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS6_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS6_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS6_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS6_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS6_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS6_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS6_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS6_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS6_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS6_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS6_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS6_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS6_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS6_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS6_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS6_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS6_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS6_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS6_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_CL_RESERVED1_MASK) >> DPWM_REGS6_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS6_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS6_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS6_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS6_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS6_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS6_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS6_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS6_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS6_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS6_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS6_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS6_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS6_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS6_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS6_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS6_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS6_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS6_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS6_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_FO_RESERVED1_MASK) >> DPWM_REGS6_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS6_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS6_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS6_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS6_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS6_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS6_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS6_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS6_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS6_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS6_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS6_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS6_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS6_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS6_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS6_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS6_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS6_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS6_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS6_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS6_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_STATUS_RUNNING_MASK) >> DPWM_REGS6_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS6_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS6_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS6_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS6_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS6_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_STATUS_DELAY_MASK) >> DPWM_REGS6_STATUS_DELAY_SHIFT)
#define  DPWM_REGS6_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS6_STATUS_DELAY_SHIFT))

#define  DPWM_REGS6_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS6_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS6_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_STATUS_SEMI_NUM_MASK) >> DPWM_REGS6_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS6_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS6_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS6_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS6_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS6_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_STATUS_PWM_OUT_MASK) >> DPWM_REGS6_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS6_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS6_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS6_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS6_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS6_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_STATUS_RESERVED1_MASK) >> DPWM_REGS6_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS6_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS6_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS6_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS6_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS6_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS6_STATUS_CUR_COUNT_MASK) >> DPWM_REGS6_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS6_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS6_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS6_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 8 of 32) (dpwm_regs7)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS7_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS7_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS7_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_CFG_RESERVED1_MASK) >> DPWM_REGS7_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS7_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS7_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS7_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS7_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS7_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS7_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS7_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS7_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS7_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS7_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_CFG_HIGH_LOW_MASK) >> DPWM_REGS7_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS7_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS7_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS7_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS7_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS7_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_CFG_OFF_STATE_MASK) >> DPWM_REGS7_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS7_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS7_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS7_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS7_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS7_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS7_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS7_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS7_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS7_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS7_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS7_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_CFG_ENABLE_MASK) >> DPWM_REGS7_CFG_ENABLE_SHIFT)
#define  DPWM_REGS7_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS7_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS7_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS7_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS7_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_COUNT0_RESERVED1_MASK) >> DPWM_REGS7_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS7_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS7_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS7_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS7_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_COUNT0_COUNT0_MASK) >> DPWM_REGS7_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS7_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS7_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS7_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS7_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS7_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_COUNT1_RESERVED1_MASK) >> DPWM_REGS7_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS7_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS7_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS7_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS7_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_COUNT1_COUNT1_MASK) >> DPWM_REGS7_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS7_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS7_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS7_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS7_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS7_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_COUNT2_RESERVED1_MASK) >> DPWM_REGS7_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS7_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS7_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS7_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS7_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_COUNT2_COUNT2_MASK) >> DPWM_REGS7_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS7_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS7_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS7_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS7_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS7_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_EN_RESERVED1_MASK) >> DPWM_REGS7_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS7_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS7_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS7_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS7_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS7_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS7_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS7_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS7_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS7_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS7_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS7_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS7_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS7_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS7_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS7_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS7_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS7_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS7_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS7_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_ST_RESERVED1_MASK) >> DPWM_REGS7_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS7_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS7_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS7_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS7_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS7_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS7_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS7_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS7_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS7_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS7_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS7_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS7_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS7_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS7_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS7_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS7_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS7_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS7_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS7_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_CL_RESERVED1_MASK) >> DPWM_REGS7_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS7_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS7_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS7_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS7_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS7_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS7_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS7_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS7_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS7_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS7_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS7_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS7_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS7_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS7_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS7_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS7_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS7_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS7_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS7_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_FO_RESERVED1_MASK) >> DPWM_REGS7_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS7_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS7_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS7_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS7_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS7_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS7_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS7_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS7_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS7_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS7_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS7_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS7_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS7_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS7_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS7_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS7_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS7_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS7_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS7_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS7_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_STATUS_RUNNING_MASK) >> DPWM_REGS7_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS7_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS7_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS7_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS7_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS7_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_STATUS_DELAY_MASK) >> DPWM_REGS7_STATUS_DELAY_SHIFT)
#define  DPWM_REGS7_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS7_STATUS_DELAY_SHIFT))

#define  DPWM_REGS7_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS7_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS7_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_STATUS_SEMI_NUM_MASK) >> DPWM_REGS7_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS7_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS7_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS7_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS7_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS7_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_STATUS_PWM_OUT_MASK) >> DPWM_REGS7_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS7_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS7_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS7_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS7_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS7_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_STATUS_RESERVED1_MASK) >> DPWM_REGS7_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS7_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS7_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS7_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS7_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS7_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS7_STATUS_CUR_COUNT_MASK) >> DPWM_REGS7_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS7_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS7_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS7_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 9 of 32) (dpwm_regs8)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS8_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS8_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS8_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_CFG_RESERVED1_MASK) >> DPWM_REGS8_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS8_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS8_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS8_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS8_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS8_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS8_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS8_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS8_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS8_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS8_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_CFG_HIGH_LOW_MASK) >> DPWM_REGS8_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS8_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS8_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS8_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS8_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS8_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_CFG_OFF_STATE_MASK) >> DPWM_REGS8_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS8_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS8_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS8_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS8_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS8_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS8_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS8_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS8_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS8_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS8_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS8_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_CFG_ENABLE_MASK) >> DPWM_REGS8_CFG_ENABLE_SHIFT)
#define  DPWM_REGS8_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS8_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS8_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS8_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS8_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_COUNT0_RESERVED1_MASK) >> DPWM_REGS8_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS8_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS8_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS8_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS8_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_COUNT0_COUNT0_MASK) >> DPWM_REGS8_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS8_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS8_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS8_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS8_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS8_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_COUNT1_RESERVED1_MASK) >> DPWM_REGS8_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS8_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS8_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS8_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS8_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_COUNT1_COUNT1_MASK) >> DPWM_REGS8_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS8_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS8_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS8_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS8_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS8_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_COUNT2_RESERVED1_MASK) >> DPWM_REGS8_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS8_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS8_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS8_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS8_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_COUNT2_COUNT2_MASK) >> DPWM_REGS8_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS8_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS8_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS8_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS8_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS8_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_EN_RESERVED1_MASK) >> DPWM_REGS8_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS8_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS8_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS8_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS8_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS8_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS8_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS8_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS8_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS8_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS8_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS8_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS8_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS8_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS8_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS8_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS8_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS8_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS8_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS8_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_ST_RESERVED1_MASK) >> DPWM_REGS8_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS8_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS8_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS8_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS8_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS8_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS8_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS8_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS8_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS8_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS8_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS8_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS8_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS8_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS8_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS8_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS8_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS8_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS8_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS8_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_CL_RESERVED1_MASK) >> DPWM_REGS8_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS8_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS8_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS8_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS8_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS8_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS8_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS8_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS8_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS8_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS8_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS8_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS8_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS8_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS8_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS8_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS8_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS8_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS8_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS8_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_FO_RESERVED1_MASK) >> DPWM_REGS8_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS8_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS8_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS8_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS8_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS8_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS8_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS8_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS8_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS8_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS8_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS8_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS8_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS8_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS8_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS8_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS8_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS8_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS8_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS8_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS8_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_STATUS_RUNNING_MASK) >> DPWM_REGS8_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS8_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS8_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS8_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS8_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS8_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_STATUS_DELAY_MASK) >> DPWM_REGS8_STATUS_DELAY_SHIFT)
#define  DPWM_REGS8_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS8_STATUS_DELAY_SHIFT))

#define  DPWM_REGS8_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS8_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS8_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_STATUS_SEMI_NUM_MASK) >> DPWM_REGS8_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS8_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS8_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS8_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS8_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS8_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_STATUS_PWM_OUT_MASK) >> DPWM_REGS8_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS8_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS8_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS8_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS8_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS8_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_STATUS_RESERVED1_MASK) >> DPWM_REGS8_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS8_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS8_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS8_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS8_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS8_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS8_STATUS_CUR_COUNT_MASK) >> DPWM_REGS8_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS8_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS8_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS8_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 10 of 32) (dpwm_regs9)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS9_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS9_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS9_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_CFG_RESERVED1_MASK) >> DPWM_REGS9_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS9_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS9_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS9_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS9_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS9_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS9_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS9_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS9_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS9_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS9_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_CFG_HIGH_LOW_MASK) >> DPWM_REGS9_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS9_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS9_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS9_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS9_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS9_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_CFG_OFF_STATE_MASK) >> DPWM_REGS9_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS9_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS9_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS9_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS9_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS9_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS9_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS9_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS9_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS9_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS9_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS9_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_CFG_ENABLE_MASK) >> DPWM_REGS9_CFG_ENABLE_SHIFT)
#define  DPWM_REGS9_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS9_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS9_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS9_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS9_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_COUNT0_RESERVED1_MASK) >> DPWM_REGS9_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS9_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS9_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS9_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS9_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_COUNT0_COUNT0_MASK) >> DPWM_REGS9_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS9_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS9_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS9_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS9_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS9_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_COUNT1_RESERVED1_MASK) >> DPWM_REGS9_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS9_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS9_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS9_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS9_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_COUNT1_COUNT1_MASK) >> DPWM_REGS9_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS9_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS9_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS9_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS9_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS9_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_COUNT2_RESERVED1_MASK) >> DPWM_REGS9_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS9_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS9_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS9_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS9_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_COUNT2_COUNT2_MASK) >> DPWM_REGS9_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS9_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS9_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS9_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS9_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS9_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_EN_RESERVED1_MASK) >> DPWM_REGS9_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS9_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS9_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS9_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS9_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS9_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS9_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS9_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS9_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS9_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS9_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS9_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS9_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS9_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS9_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS9_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS9_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS9_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS9_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS9_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_ST_RESERVED1_MASK) >> DPWM_REGS9_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS9_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS9_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS9_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS9_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS9_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS9_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS9_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS9_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS9_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS9_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS9_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS9_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS9_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS9_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS9_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS9_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS9_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS9_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS9_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_CL_RESERVED1_MASK) >> DPWM_REGS9_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS9_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS9_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS9_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS9_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS9_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS9_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS9_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS9_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS9_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS9_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS9_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS9_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS9_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS9_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS9_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS9_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS9_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS9_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS9_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_FO_RESERVED1_MASK) >> DPWM_REGS9_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS9_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS9_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS9_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS9_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS9_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS9_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS9_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS9_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS9_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS9_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS9_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS9_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS9_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS9_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS9_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS9_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS9_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS9_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS9_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS9_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_STATUS_RUNNING_MASK) >> DPWM_REGS9_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS9_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS9_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS9_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS9_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS9_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_STATUS_DELAY_MASK) >> DPWM_REGS9_STATUS_DELAY_SHIFT)
#define  DPWM_REGS9_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS9_STATUS_DELAY_SHIFT))

#define  DPWM_REGS9_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS9_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS9_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_STATUS_SEMI_NUM_MASK) >> DPWM_REGS9_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS9_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS9_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS9_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS9_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS9_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_STATUS_PWM_OUT_MASK) >> DPWM_REGS9_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS9_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS9_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS9_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS9_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS9_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_STATUS_RESERVED1_MASK) >> DPWM_REGS9_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS9_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS9_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS9_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS9_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS9_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS9_STATUS_CUR_COUNT_MASK) >> DPWM_REGS9_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS9_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS9_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS9_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 11 of 32) (dpwm_regs10)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS10_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS10_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS10_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_CFG_RESERVED1_MASK) >> DPWM_REGS10_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS10_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS10_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS10_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS10_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS10_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS10_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS10_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS10_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS10_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS10_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_CFG_HIGH_LOW_MASK) >> DPWM_REGS10_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS10_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS10_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS10_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS10_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS10_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_CFG_OFF_STATE_MASK) >> DPWM_REGS10_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS10_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS10_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS10_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS10_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS10_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS10_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS10_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS10_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS10_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS10_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS10_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_CFG_ENABLE_MASK) >> DPWM_REGS10_CFG_ENABLE_SHIFT)
#define  DPWM_REGS10_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS10_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS10_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS10_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS10_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_COUNT0_RESERVED1_MASK) >> DPWM_REGS10_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS10_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS10_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS10_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS10_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_COUNT0_COUNT0_MASK) >> DPWM_REGS10_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS10_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS10_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS10_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS10_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS10_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_COUNT1_RESERVED1_MASK) >> DPWM_REGS10_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS10_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS10_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS10_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS10_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_COUNT1_COUNT1_MASK) >> DPWM_REGS10_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS10_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS10_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS10_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS10_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS10_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_COUNT2_RESERVED1_MASK) >> DPWM_REGS10_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS10_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS10_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS10_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS10_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_COUNT2_COUNT2_MASK) >> DPWM_REGS10_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS10_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS10_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS10_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS10_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS10_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_EN_RESERVED1_MASK) >> DPWM_REGS10_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS10_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS10_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS10_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS10_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS10_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS10_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS10_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS10_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS10_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS10_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS10_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS10_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS10_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS10_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS10_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS10_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS10_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS10_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS10_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_ST_RESERVED1_MASK) >> DPWM_REGS10_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS10_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS10_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS10_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS10_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS10_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS10_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS10_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS10_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS10_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS10_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS10_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS10_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS10_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS10_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS10_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS10_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS10_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS10_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS10_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_CL_RESERVED1_MASK) >> DPWM_REGS10_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS10_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS10_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS10_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS10_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS10_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS10_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS10_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS10_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS10_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS10_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS10_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS10_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS10_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS10_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS10_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS10_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS10_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS10_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS10_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_FO_RESERVED1_MASK) >> DPWM_REGS10_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS10_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS10_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS10_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS10_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS10_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS10_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS10_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS10_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS10_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS10_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS10_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS10_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS10_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS10_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS10_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS10_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS10_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS10_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS10_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS10_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_STATUS_RUNNING_MASK) >> DPWM_REGS10_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS10_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS10_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS10_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS10_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS10_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_STATUS_DELAY_MASK) >> DPWM_REGS10_STATUS_DELAY_SHIFT)
#define  DPWM_REGS10_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS10_STATUS_DELAY_SHIFT))

#define  DPWM_REGS10_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS10_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS10_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_STATUS_SEMI_NUM_MASK) >> DPWM_REGS10_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS10_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS10_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS10_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS10_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS10_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_STATUS_PWM_OUT_MASK) >> DPWM_REGS10_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS10_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS10_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS10_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS10_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS10_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_STATUS_RESERVED1_MASK) >> DPWM_REGS10_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS10_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS10_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS10_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS10_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS10_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS10_STATUS_CUR_COUNT_MASK) >> DPWM_REGS10_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS10_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS10_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS10_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 12 of 32) (dpwm_regs11)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS11_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS11_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS11_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_CFG_RESERVED1_MASK) >> DPWM_REGS11_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS11_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS11_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS11_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS11_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS11_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS11_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS11_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS11_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS11_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS11_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_CFG_HIGH_LOW_MASK) >> DPWM_REGS11_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS11_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS11_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS11_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS11_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS11_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_CFG_OFF_STATE_MASK) >> DPWM_REGS11_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS11_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS11_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS11_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS11_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS11_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS11_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS11_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS11_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS11_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS11_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS11_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_CFG_ENABLE_MASK) >> DPWM_REGS11_CFG_ENABLE_SHIFT)
#define  DPWM_REGS11_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS11_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS11_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS11_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS11_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_COUNT0_RESERVED1_MASK) >> DPWM_REGS11_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS11_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS11_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS11_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS11_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_COUNT0_COUNT0_MASK) >> DPWM_REGS11_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS11_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS11_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS11_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS11_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS11_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_COUNT1_RESERVED1_MASK) >> DPWM_REGS11_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS11_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS11_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS11_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS11_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_COUNT1_COUNT1_MASK) >> DPWM_REGS11_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS11_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS11_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS11_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS11_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS11_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_COUNT2_RESERVED1_MASK) >> DPWM_REGS11_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS11_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS11_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS11_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS11_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_COUNT2_COUNT2_MASK) >> DPWM_REGS11_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS11_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS11_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS11_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS11_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS11_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_EN_RESERVED1_MASK) >> DPWM_REGS11_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS11_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS11_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS11_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS11_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS11_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS11_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS11_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS11_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS11_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS11_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS11_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS11_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS11_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS11_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS11_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS11_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS11_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS11_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS11_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_ST_RESERVED1_MASK) >> DPWM_REGS11_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS11_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS11_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS11_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS11_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS11_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS11_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS11_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS11_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS11_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS11_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS11_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS11_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS11_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS11_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS11_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS11_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS11_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS11_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS11_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_CL_RESERVED1_MASK) >> DPWM_REGS11_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS11_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS11_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS11_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS11_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS11_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS11_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS11_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS11_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS11_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS11_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS11_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS11_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS11_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS11_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS11_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS11_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS11_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS11_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS11_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_FO_RESERVED1_MASK) >> DPWM_REGS11_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS11_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS11_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS11_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS11_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS11_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS11_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS11_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS11_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS11_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS11_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS11_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS11_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS11_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS11_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS11_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS11_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS11_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS11_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS11_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS11_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_STATUS_RUNNING_MASK) >> DPWM_REGS11_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS11_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS11_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS11_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS11_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS11_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_STATUS_DELAY_MASK) >> DPWM_REGS11_STATUS_DELAY_SHIFT)
#define  DPWM_REGS11_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS11_STATUS_DELAY_SHIFT))

#define  DPWM_REGS11_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS11_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS11_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_STATUS_SEMI_NUM_MASK) >> DPWM_REGS11_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS11_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS11_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS11_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS11_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS11_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_STATUS_PWM_OUT_MASK) >> DPWM_REGS11_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS11_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS11_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS11_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS11_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS11_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_STATUS_RESERVED1_MASK) >> DPWM_REGS11_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS11_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS11_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS11_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS11_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS11_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS11_STATUS_CUR_COUNT_MASK) >> DPWM_REGS11_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS11_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS11_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS11_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 13 of 32) (dpwm_regs12)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS12_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS12_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS12_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_CFG_RESERVED1_MASK) >> DPWM_REGS12_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS12_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS12_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS12_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS12_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS12_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS12_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS12_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS12_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS12_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS12_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_CFG_HIGH_LOW_MASK) >> DPWM_REGS12_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS12_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS12_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS12_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS12_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS12_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_CFG_OFF_STATE_MASK) >> DPWM_REGS12_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS12_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS12_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS12_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS12_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS12_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS12_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS12_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS12_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS12_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS12_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS12_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_CFG_ENABLE_MASK) >> DPWM_REGS12_CFG_ENABLE_SHIFT)
#define  DPWM_REGS12_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS12_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS12_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS12_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS12_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_COUNT0_RESERVED1_MASK) >> DPWM_REGS12_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS12_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS12_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS12_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS12_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_COUNT0_COUNT0_MASK) >> DPWM_REGS12_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS12_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS12_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS12_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS12_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS12_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_COUNT1_RESERVED1_MASK) >> DPWM_REGS12_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS12_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS12_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS12_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS12_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_COUNT1_COUNT1_MASK) >> DPWM_REGS12_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS12_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS12_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS12_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS12_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS12_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_COUNT2_RESERVED1_MASK) >> DPWM_REGS12_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS12_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS12_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS12_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS12_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_COUNT2_COUNT2_MASK) >> DPWM_REGS12_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS12_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS12_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS12_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS12_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS12_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_EN_RESERVED1_MASK) >> DPWM_REGS12_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS12_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS12_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS12_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS12_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS12_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS12_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS12_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS12_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS12_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS12_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS12_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS12_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS12_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS12_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS12_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS12_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS12_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS12_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS12_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_ST_RESERVED1_MASK) >> DPWM_REGS12_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS12_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS12_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS12_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS12_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS12_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS12_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS12_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS12_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS12_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS12_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS12_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS12_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS12_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS12_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS12_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS12_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS12_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS12_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS12_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_CL_RESERVED1_MASK) >> DPWM_REGS12_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS12_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS12_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS12_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS12_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS12_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS12_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS12_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS12_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS12_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS12_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS12_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS12_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS12_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS12_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS12_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS12_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS12_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS12_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS12_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_FO_RESERVED1_MASK) >> DPWM_REGS12_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS12_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS12_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS12_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS12_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS12_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS12_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS12_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS12_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS12_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS12_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS12_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS12_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS12_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS12_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS12_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS12_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS12_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS12_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS12_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS12_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_STATUS_RUNNING_MASK) >> DPWM_REGS12_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS12_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS12_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS12_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS12_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS12_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_STATUS_DELAY_MASK) >> DPWM_REGS12_STATUS_DELAY_SHIFT)
#define  DPWM_REGS12_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS12_STATUS_DELAY_SHIFT))

#define  DPWM_REGS12_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS12_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS12_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_STATUS_SEMI_NUM_MASK) >> DPWM_REGS12_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS12_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS12_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS12_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS12_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS12_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_STATUS_PWM_OUT_MASK) >> DPWM_REGS12_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS12_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS12_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS12_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS12_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS12_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_STATUS_RESERVED1_MASK) >> DPWM_REGS12_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS12_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS12_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS12_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS12_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS12_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS12_STATUS_CUR_COUNT_MASK) >> DPWM_REGS12_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS12_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS12_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS12_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 14 of 32) (dpwm_regs13)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS13_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS13_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS13_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_CFG_RESERVED1_MASK) >> DPWM_REGS13_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS13_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS13_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS13_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS13_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS13_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS13_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS13_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS13_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS13_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS13_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_CFG_HIGH_LOW_MASK) >> DPWM_REGS13_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS13_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS13_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS13_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS13_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS13_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_CFG_OFF_STATE_MASK) >> DPWM_REGS13_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS13_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS13_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS13_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS13_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS13_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS13_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS13_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS13_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS13_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS13_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS13_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_CFG_ENABLE_MASK) >> DPWM_REGS13_CFG_ENABLE_SHIFT)
#define  DPWM_REGS13_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS13_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS13_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS13_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS13_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_COUNT0_RESERVED1_MASK) >> DPWM_REGS13_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS13_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS13_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS13_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS13_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_COUNT0_COUNT0_MASK) >> DPWM_REGS13_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS13_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS13_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS13_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS13_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS13_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_COUNT1_RESERVED1_MASK) >> DPWM_REGS13_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS13_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS13_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS13_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS13_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_COUNT1_COUNT1_MASK) >> DPWM_REGS13_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS13_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS13_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS13_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS13_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS13_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_COUNT2_RESERVED1_MASK) >> DPWM_REGS13_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS13_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS13_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS13_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS13_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_COUNT2_COUNT2_MASK) >> DPWM_REGS13_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS13_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS13_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS13_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS13_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS13_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_EN_RESERVED1_MASK) >> DPWM_REGS13_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS13_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS13_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS13_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS13_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS13_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS13_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS13_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS13_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS13_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS13_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS13_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS13_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS13_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS13_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS13_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS13_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS13_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS13_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS13_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_ST_RESERVED1_MASK) >> DPWM_REGS13_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS13_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS13_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS13_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS13_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS13_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS13_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS13_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS13_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS13_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS13_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS13_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS13_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS13_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS13_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS13_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS13_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS13_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS13_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS13_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_CL_RESERVED1_MASK) >> DPWM_REGS13_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS13_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS13_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS13_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS13_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS13_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS13_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS13_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS13_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS13_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS13_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS13_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS13_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS13_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS13_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS13_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS13_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS13_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS13_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS13_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_FO_RESERVED1_MASK) >> DPWM_REGS13_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS13_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS13_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS13_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS13_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS13_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS13_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS13_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS13_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS13_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS13_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS13_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS13_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS13_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS13_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS13_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS13_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS13_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS13_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS13_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS13_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_STATUS_RUNNING_MASK) >> DPWM_REGS13_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS13_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS13_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS13_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS13_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS13_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_STATUS_DELAY_MASK) >> DPWM_REGS13_STATUS_DELAY_SHIFT)
#define  DPWM_REGS13_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS13_STATUS_DELAY_SHIFT))

#define  DPWM_REGS13_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS13_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS13_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_STATUS_SEMI_NUM_MASK) >> DPWM_REGS13_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS13_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS13_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS13_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS13_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS13_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_STATUS_PWM_OUT_MASK) >> DPWM_REGS13_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS13_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS13_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS13_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS13_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS13_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_STATUS_RESERVED1_MASK) >> DPWM_REGS13_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS13_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS13_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS13_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS13_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS13_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS13_STATUS_CUR_COUNT_MASK) >> DPWM_REGS13_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS13_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS13_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS13_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 15 of 32) (dpwm_regs14)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS14_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS14_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS14_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_CFG_RESERVED1_MASK) >> DPWM_REGS14_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS14_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS14_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS14_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS14_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS14_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS14_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS14_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS14_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS14_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS14_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_CFG_HIGH_LOW_MASK) >> DPWM_REGS14_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS14_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS14_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS14_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS14_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS14_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_CFG_OFF_STATE_MASK) >> DPWM_REGS14_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS14_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS14_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS14_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS14_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS14_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS14_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS14_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS14_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS14_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS14_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS14_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_CFG_ENABLE_MASK) >> DPWM_REGS14_CFG_ENABLE_SHIFT)
#define  DPWM_REGS14_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS14_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS14_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS14_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS14_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_COUNT0_RESERVED1_MASK) >> DPWM_REGS14_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS14_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS14_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS14_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS14_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_COUNT0_COUNT0_MASK) >> DPWM_REGS14_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS14_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS14_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS14_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS14_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS14_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_COUNT1_RESERVED1_MASK) >> DPWM_REGS14_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS14_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS14_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS14_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS14_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_COUNT1_COUNT1_MASK) >> DPWM_REGS14_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS14_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS14_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS14_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS14_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS14_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_COUNT2_RESERVED1_MASK) >> DPWM_REGS14_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS14_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS14_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS14_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS14_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_COUNT2_COUNT2_MASK) >> DPWM_REGS14_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS14_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS14_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS14_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS14_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS14_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_EN_RESERVED1_MASK) >> DPWM_REGS14_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS14_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS14_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS14_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS14_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS14_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS14_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS14_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS14_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS14_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS14_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS14_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS14_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS14_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS14_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS14_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS14_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS14_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS14_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS14_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_ST_RESERVED1_MASK) >> DPWM_REGS14_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS14_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS14_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS14_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS14_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS14_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS14_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS14_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS14_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS14_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS14_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS14_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS14_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS14_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS14_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS14_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS14_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS14_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS14_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS14_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_CL_RESERVED1_MASK) >> DPWM_REGS14_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS14_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS14_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS14_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS14_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS14_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS14_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS14_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS14_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS14_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS14_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS14_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS14_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS14_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS14_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS14_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS14_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS14_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS14_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS14_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_FO_RESERVED1_MASK) >> DPWM_REGS14_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS14_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS14_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS14_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS14_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS14_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS14_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS14_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS14_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS14_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS14_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS14_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS14_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS14_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS14_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS14_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS14_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS14_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS14_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS14_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS14_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_STATUS_RUNNING_MASK) >> DPWM_REGS14_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS14_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS14_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS14_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS14_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS14_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_STATUS_DELAY_MASK) >> DPWM_REGS14_STATUS_DELAY_SHIFT)
#define  DPWM_REGS14_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS14_STATUS_DELAY_SHIFT))

#define  DPWM_REGS14_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS14_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS14_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_STATUS_SEMI_NUM_MASK) >> DPWM_REGS14_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS14_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS14_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS14_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS14_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS14_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_STATUS_PWM_OUT_MASK) >> DPWM_REGS14_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS14_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS14_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS14_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS14_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS14_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_STATUS_RESERVED1_MASK) >> DPWM_REGS14_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS14_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS14_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS14_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS14_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS14_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS14_STATUS_CUR_COUNT_MASK) >> DPWM_REGS14_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS14_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS14_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS14_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 16 of 32) (dpwm_regs15)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS15_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS15_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS15_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_CFG_RESERVED1_MASK) >> DPWM_REGS15_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS15_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS15_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS15_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS15_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS15_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS15_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS15_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS15_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS15_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS15_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_CFG_HIGH_LOW_MASK) >> DPWM_REGS15_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS15_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS15_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS15_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS15_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS15_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_CFG_OFF_STATE_MASK) >> DPWM_REGS15_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS15_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS15_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS15_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS15_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS15_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS15_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS15_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS15_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS15_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS15_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS15_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_CFG_ENABLE_MASK) >> DPWM_REGS15_CFG_ENABLE_SHIFT)
#define  DPWM_REGS15_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS15_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS15_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS15_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS15_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_COUNT0_RESERVED1_MASK) >> DPWM_REGS15_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS15_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS15_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS15_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS15_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_COUNT0_COUNT0_MASK) >> DPWM_REGS15_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS15_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS15_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS15_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS15_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS15_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_COUNT1_RESERVED1_MASK) >> DPWM_REGS15_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS15_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS15_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS15_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS15_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_COUNT1_COUNT1_MASK) >> DPWM_REGS15_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS15_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS15_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS15_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS15_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS15_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_COUNT2_RESERVED1_MASK) >> DPWM_REGS15_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS15_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS15_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS15_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS15_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_COUNT2_COUNT2_MASK) >> DPWM_REGS15_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS15_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS15_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS15_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS15_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS15_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_EN_RESERVED1_MASK) >> DPWM_REGS15_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS15_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS15_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS15_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS15_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS15_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS15_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS15_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS15_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS15_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS15_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS15_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS15_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS15_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS15_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS15_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS15_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS15_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS15_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS15_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_ST_RESERVED1_MASK) >> DPWM_REGS15_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS15_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS15_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS15_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS15_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS15_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS15_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS15_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS15_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS15_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS15_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS15_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS15_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS15_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS15_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS15_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS15_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS15_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS15_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS15_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_CL_RESERVED1_MASK) >> DPWM_REGS15_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS15_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS15_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS15_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS15_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS15_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS15_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS15_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS15_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS15_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS15_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS15_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS15_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS15_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS15_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS15_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS15_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS15_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS15_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS15_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_FO_RESERVED1_MASK) >> DPWM_REGS15_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS15_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS15_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS15_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS15_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS15_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS15_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS15_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS15_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS15_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS15_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS15_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS15_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS15_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS15_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS15_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS15_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS15_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS15_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS15_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS15_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_STATUS_RUNNING_MASK) >> DPWM_REGS15_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS15_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS15_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS15_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS15_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS15_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_STATUS_DELAY_MASK) >> DPWM_REGS15_STATUS_DELAY_SHIFT)
#define  DPWM_REGS15_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS15_STATUS_DELAY_SHIFT))

#define  DPWM_REGS15_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS15_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS15_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_STATUS_SEMI_NUM_MASK) >> DPWM_REGS15_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS15_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS15_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS15_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS15_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS15_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_STATUS_PWM_OUT_MASK) >> DPWM_REGS15_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS15_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS15_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS15_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS15_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS15_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_STATUS_RESERVED1_MASK) >> DPWM_REGS15_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS15_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS15_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS15_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS15_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS15_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS15_STATUS_CUR_COUNT_MASK) >> DPWM_REGS15_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS15_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS15_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS15_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 17 of 32) (dpwm_regs16)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS16_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS16_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS16_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_CFG_RESERVED1_MASK) >> DPWM_REGS16_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS16_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS16_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS16_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS16_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS16_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS16_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS16_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS16_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS16_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS16_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_CFG_HIGH_LOW_MASK) >> DPWM_REGS16_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS16_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS16_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS16_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS16_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS16_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_CFG_OFF_STATE_MASK) >> DPWM_REGS16_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS16_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS16_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS16_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS16_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS16_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS16_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS16_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS16_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS16_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS16_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS16_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_CFG_ENABLE_MASK) >> DPWM_REGS16_CFG_ENABLE_SHIFT)
#define  DPWM_REGS16_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS16_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS16_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS16_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS16_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_COUNT0_RESERVED1_MASK) >> DPWM_REGS16_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS16_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS16_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS16_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS16_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_COUNT0_COUNT0_MASK) >> DPWM_REGS16_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS16_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS16_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS16_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS16_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS16_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_COUNT1_RESERVED1_MASK) >> DPWM_REGS16_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS16_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS16_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS16_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS16_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_COUNT1_COUNT1_MASK) >> DPWM_REGS16_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS16_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS16_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS16_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS16_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS16_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_COUNT2_RESERVED1_MASK) >> DPWM_REGS16_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS16_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS16_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS16_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS16_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_COUNT2_COUNT2_MASK) >> DPWM_REGS16_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS16_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS16_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS16_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS16_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS16_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_EN_RESERVED1_MASK) >> DPWM_REGS16_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS16_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS16_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS16_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS16_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS16_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS16_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS16_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS16_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS16_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS16_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS16_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS16_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS16_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS16_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS16_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS16_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS16_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS16_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS16_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_ST_RESERVED1_MASK) >> DPWM_REGS16_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS16_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS16_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS16_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS16_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS16_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS16_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS16_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS16_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS16_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS16_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS16_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS16_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS16_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS16_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS16_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS16_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS16_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS16_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS16_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_CL_RESERVED1_MASK) >> DPWM_REGS16_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS16_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS16_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS16_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS16_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS16_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS16_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS16_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS16_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS16_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS16_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS16_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS16_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS16_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS16_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS16_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS16_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS16_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS16_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS16_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_FO_RESERVED1_MASK) >> DPWM_REGS16_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS16_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS16_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS16_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS16_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS16_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS16_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS16_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS16_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS16_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS16_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS16_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS16_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS16_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS16_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS16_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS16_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS16_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS16_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS16_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS16_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_STATUS_RUNNING_MASK) >> DPWM_REGS16_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS16_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS16_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS16_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS16_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS16_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_STATUS_DELAY_MASK) >> DPWM_REGS16_STATUS_DELAY_SHIFT)
#define  DPWM_REGS16_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS16_STATUS_DELAY_SHIFT))

#define  DPWM_REGS16_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS16_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS16_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_STATUS_SEMI_NUM_MASK) >> DPWM_REGS16_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS16_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS16_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS16_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS16_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS16_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_STATUS_PWM_OUT_MASK) >> DPWM_REGS16_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS16_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS16_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS16_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS16_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS16_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_STATUS_RESERVED1_MASK) >> DPWM_REGS16_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS16_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS16_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS16_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS16_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS16_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS16_STATUS_CUR_COUNT_MASK) >> DPWM_REGS16_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS16_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS16_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS16_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 18 of 32) (dpwm_regs17)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS17_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS17_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS17_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_CFG_RESERVED1_MASK) >> DPWM_REGS17_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS17_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS17_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS17_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS17_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS17_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS17_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS17_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS17_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS17_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS17_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_CFG_HIGH_LOW_MASK) >> DPWM_REGS17_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS17_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS17_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS17_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS17_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS17_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_CFG_OFF_STATE_MASK) >> DPWM_REGS17_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS17_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS17_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS17_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS17_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS17_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS17_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS17_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS17_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS17_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS17_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS17_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_CFG_ENABLE_MASK) >> DPWM_REGS17_CFG_ENABLE_SHIFT)
#define  DPWM_REGS17_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS17_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS17_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS17_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS17_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_COUNT0_RESERVED1_MASK) >> DPWM_REGS17_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS17_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS17_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS17_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS17_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_COUNT0_COUNT0_MASK) >> DPWM_REGS17_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS17_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS17_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS17_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS17_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS17_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_COUNT1_RESERVED1_MASK) >> DPWM_REGS17_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS17_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS17_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS17_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS17_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_COUNT1_COUNT1_MASK) >> DPWM_REGS17_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS17_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS17_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS17_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS17_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS17_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_COUNT2_RESERVED1_MASK) >> DPWM_REGS17_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS17_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS17_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS17_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS17_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_COUNT2_COUNT2_MASK) >> DPWM_REGS17_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS17_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS17_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS17_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS17_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS17_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_EN_RESERVED1_MASK) >> DPWM_REGS17_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS17_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS17_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS17_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS17_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS17_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS17_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS17_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS17_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS17_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS17_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS17_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS17_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS17_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS17_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS17_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS17_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS17_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS17_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS17_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_ST_RESERVED1_MASK) >> DPWM_REGS17_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS17_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS17_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS17_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS17_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS17_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS17_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS17_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS17_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS17_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS17_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS17_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS17_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS17_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS17_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS17_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS17_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS17_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS17_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS17_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_CL_RESERVED1_MASK) >> DPWM_REGS17_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS17_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS17_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS17_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS17_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS17_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS17_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS17_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS17_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS17_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS17_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS17_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS17_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS17_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS17_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS17_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS17_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS17_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS17_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS17_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_FO_RESERVED1_MASK) >> DPWM_REGS17_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS17_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS17_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS17_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS17_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS17_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS17_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS17_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS17_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS17_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS17_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS17_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS17_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS17_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS17_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS17_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS17_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS17_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS17_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS17_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS17_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_STATUS_RUNNING_MASK) >> DPWM_REGS17_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS17_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS17_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS17_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS17_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS17_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_STATUS_DELAY_MASK) >> DPWM_REGS17_STATUS_DELAY_SHIFT)
#define  DPWM_REGS17_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS17_STATUS_DELAY_SHIFT))

#define  DPWM_REGS17_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS17_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS17_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_STATUS_SEMI_NUM_MASK) >> DPWM_REGS17_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS17_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS17_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS17_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS17_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS17_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_STATUS_PWM_OUT_MASK) >> DPWM_REGS17_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS17_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS17_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS17_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS17_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS17_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_STATUS_RESERVED1_MASK) >> DPWM_REGS17_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS17_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS17_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS17_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS17_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS17_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS17_STATUS_CUR_COUNT_MASK) >> DPWM_REGS17_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS17_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS17_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS17_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 19 of 32) (dpwm_regs18)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS18_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS18_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS18_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_CFG_RESERVED1_MASK) >> DPWM_REGS18_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS18_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS18_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS18_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS18_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS18_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS18_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS18_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS18_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS18_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS18_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_CFG_HIGH_LOW_MASK) >> DPWM_REGS18_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS18_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS18_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS18_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS18_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS18_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_CFG_OFF_STATE_MASK) >> DPWM_REGS18_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS18_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS18_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS18_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS18_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS18_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS18_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS18_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS18_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS18_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS18_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS18_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_CFG_ENABLE_MASK) >> DPWM_REGS18_CFG_ENABLE_SHIFT)
#define  DPWM_REGS18_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS18_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS18_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS18_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS18_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_COUNT0_RESERVED1_MASK) >> DPWM_REGS18_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS18_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS18_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS18_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS18_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_COUNT0_COUNT0_MASK) >> DPWM_REGS18_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS18_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS18_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS18_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS18_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS18_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_COUNT1_RESERVED1_MASK) >> DPWM_REGS18_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS18_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS18_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS18_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS18_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_COUNT1_COUNT1_MASK) >> DPWM_REGS18_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS18_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS18_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS18_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS18_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS18_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_COUNT2_RESERVED1_MASK) >> DPWM_REGS18_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS18_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS18_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS18_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS18_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_COUNT2_COUNT2_MASK) >> DPWM_REGS18_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS18_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS18_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS18_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS18_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS18_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_EN_RESERVED1_MASK) >> DPWM_REGS18_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS18_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS18_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS18_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS18_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS18_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS18_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS18_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS18_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS18_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS18_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS18_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS18_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS18_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS18_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS18_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS18_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS18_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS18_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS18_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_ST_RESERVED1_MASK) >> DPWM_REGS18_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS18_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS18_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS18_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS18_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS18_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS18_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS18_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS18_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS18_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS18_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS18_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS18_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS18_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS18_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS18_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS18_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS18_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS18_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS18_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_CL_RESERVED1_MASK) >> DPWM_REGS18_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS18_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS18_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS18_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS18_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS18_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS18_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS18_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS18_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS18_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS18_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS18_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS18_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS18_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS18_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS18_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS18_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS18_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS18_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS18_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_FO_RESERVED1_MASK) >> DPWM_REGS18_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS18_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS18_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS18_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS18_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS18_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS18_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS18_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS18_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS18_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS18_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS18_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS18_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS18_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS18_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS18_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS18_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS18_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS18_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS18_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS18_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_STATUS_RUNNING_MASK) >> DPWM_REGS18_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS18_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS18_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS18_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS18_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS18_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_STATUS_DELAY_MASK) >> DPWM_REGS18_STATUS_DELAY_SHIFT)
#define  DPWM_REGS18_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS18_STATUS_DELAY_SHIFT))

#define  DPWM_REGS18_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS18_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS18_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_STATUS_SEMI_NUM_MASK) >> DPWM_REGS18_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS18_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS18_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS18_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS18_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS18_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_STATUS_PWM_OUT_MASK) >> DPWM_REGS18_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS18_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS18_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS18_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS18_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS18_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_STATUS_RESERVED1_MASK) >> DPWM_REGS18_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS18_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS18_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS18_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS18_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS18_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS18_STATUS_CUR_COUNT_MASK) >> DPWM_REGS18_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS18_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS18_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS18_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 20 of 32) (dpwm_regs19)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS19_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS19_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS19_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_CFG_RESERVED1_MASK) >> DPWM_REGS19_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS19_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS19_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS19_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS19_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS19_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS19_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS19_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS19_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS19_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS19_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_CFG_HIGH_LOW_MASK) >> DPWM_REGS19_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS19_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS19_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS19_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS19_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS19_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_CFG_OFF_STATE_MASK) >> DPWM_REGS19_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS19_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS19_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS19_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS19_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS19_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS19_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS19_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS19_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS19_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS19_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS19_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_CFG_ENABLE_MASK) >> DPWM_REGS19_CFG_ENABLE_SHIFT)
#define  DPWM_REGS19_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS19_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS19_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS19_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS19_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_COUNT0_RESERVED1_MASK) >> DPWM_REGS19_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS19_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS19_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS19_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS19_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_COUNT0_COUNT0_MASK) >> DPWM_REGS19_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS19_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS19_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS19_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS19_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS19_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_COUNT1_RESERVED1_MASK) >> DPWM_REGS19_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS19_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS19_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS19_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS19_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_COUNT1_COUNT1_MASK) >> DPWM_REGS19_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS19_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS19_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS19_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS19_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS19_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_COUNT2_RESERVED1_MASK) >> DPWM_REGS19_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS19_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS19_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS19_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS19_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_COUNT2_COUNT2_MASK) >> DPWM_REGS19_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS19_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS19_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS19_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS19_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS19_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_EN_RESERVED1_MASK) >> DPWM_REGS19_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS19_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS19_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS19_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS19_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS19_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS19_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS19_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS19_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS19_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS19_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS19_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS19_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS19_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS19_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS19_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS19_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS19_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS19_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS19_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_ST_RESERVED1_MASK) >> DPWM_REGS19_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS19_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS19_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS19_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS19_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS19_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS19_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS19_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS19_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS19_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS19_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS19_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS19_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS19_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS19_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS19_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS19_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS19_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS19_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS19_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_CL_RESERVED1_MASK) >> DPWM_REGS19_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS19_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS19_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS19_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS19_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS19_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS19_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS19_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS19_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS19_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS19_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS19_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS19_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS19_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS19_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS19_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS19_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS19_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS19_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS19_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_FO_RESERVED1_MASK) >> DPWM_REGS19_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS19_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS19_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS19_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS19_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS19_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS19_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS19_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS19_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS19_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS19_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS19_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS19_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS19_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS19_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS19_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS19_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS19_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS19_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS19_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS19_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_STATUS_RUNNING_MASK) >> DPWM_REGS19_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS19_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS19_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS19_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS19_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS19_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_STATUS_DELAY_MASK) >> DPWM_REGS19_STATUS_DELAY_SHIFT)
#define  DPWM_REGS19_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS19_STATUS_DELAY_SHIFT))

#define  DPWM_REGS19_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS19_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS19_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_STATUS_SEMI_NUM_MASK) >> DPWM_REGS19_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS19_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS19_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS19_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS19_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS19_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_STATUS_PWM_OUT_MASK) >> DPWM_REGS19_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS19_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS19_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS19_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS19_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS19_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_STATUS_RESERVED1_MASK) >> DPWM_REGS19_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS19_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS19_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS19_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS19_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS19_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS19_STATUS_CUR_COUNT_MASK) >> DPWM_REGS19_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS19_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS19_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS19_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 21 of 32) (dpwm_regs20)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS20_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS20_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS20_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_CFG_RESERVED1_MASK) >> DPWM_REGS20_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS20_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS20_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS20_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS20_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS20_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS20_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS20_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS20_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS20_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS20_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_CFG_HIGH_LOW_MASK) >> DPWM_REGS20_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS20_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS20_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS20_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS20_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS20_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_CFG_OFF_STATE_MASK) >> DPWM_REGS20_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS20_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS20_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS20_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS20_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS20_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS20_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS20_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS20_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS20_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS20_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS20_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_CFG_ENABLE_MASK) >> DPWM_REGS20_CFG_ENABLE_SHIFT)
#define  DPWM_REGS20_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS20_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS20_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS20_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS20_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_COUNT0_RESERVED1_MASK) >> DPWM_REGS20_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS20_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS20_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS20_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS20_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_COUNT0_COUNT0_MASK) >> DPWM_REGS20_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS20_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS20_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS20_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS20_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS20_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_COUNT1_RESERVED1_MASK) >> DPWM_REGS20_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS20_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS20_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS20_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS20_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_COUNT1_COUNT1_MASK) >> DPWM_REGS20_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS20_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS20_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS20_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS20_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS20_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_COUNT2_RESERVED1_MASK) >> DPWM_REGS20_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS20_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS20_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS20_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS20_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_COUNT2_COUNT2_MASK) >> DPWM_REGS20_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS20_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS20_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS20_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS20_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS20_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_EN_RESERVED1_MASK) >> DPWM_REGS20_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS20_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS20_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS20_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS20_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS20_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS20_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS20_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS20_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS20_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS20_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS20_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS20_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS20_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS20_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS20_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS20_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS20_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS20_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS20_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_ST_RESERVED1_MASK) >> DPWM_REGS20_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS20_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS20_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS20_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS20_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS20_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS20_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS20_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS20_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS20_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS20_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS20_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS20_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS20_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS20_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS20_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS20_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS20_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS20_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS20_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_CL_RESERVED1_MASK) >> DPWM_REGS20_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS20_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS20_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS20_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS20_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS20_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS20_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS20_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS20_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS20_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS20_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS20_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS20_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS20_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS20_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS20_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS20_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS20_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS20_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS20_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_FO_RESERVED1_MASK) >> DPWM_REGS20_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS20_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS20_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS20_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS20_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS20_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS20_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS20_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS20_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS20_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS20_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS20_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS20_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS20_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS20_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS20_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS20_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS20_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS20_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS20_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS20_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_STATUS_RUNNING_MASK) >> DPWM_REGS20_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS20_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS20_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS20_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS20_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS20_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_STATUS_DELAY_MASK) >> DPWM_REGS20_STATUS_DELAY_SHIFT)
#define  DPWM_REGS20_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS20_STATUS_DELAY_SHIFT))

#define  DPWM_REGS20_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS20_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS20_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_STATUS_SEMI_NUM_MASK) >> DPWM_REGS20_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS20_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS20_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS20_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS20_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS20_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_STATUS_PWM_OUT_MASK) >> DPWM_REGS20_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS20_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS20_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS20_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS20_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS20_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_STATUS_RESERVED1_MASK) >> DPWM_REGS20_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS20_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS20_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS20_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS20_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS20_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS20_STATUS_CUR_COUNT_MASK) >> DPWM_REGS20_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS20_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS20_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS20_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 22 of 32) (dpwm_regs21)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS21_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS21_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS21_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_CFG_RESERVED1_MASK) >> DPWM_REGS21_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS21_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS21_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS21_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS21_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS21_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS21_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS21_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS21_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS21_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS21_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_CFG_HIGH_LOW_MASK) >> DPWM_REGS21_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS21_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS21_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS21_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS21_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS21_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_CFG_OFF_STATE_MASK) >> DPWM_REGS21_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS21_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS21_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS21_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS21_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS21_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS21_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS21_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS21_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS21_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS21_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS21_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_CFG_ENABLE_MASK) >> DPWM_REGS21_CFG_ENABLE_SHIFT)
#define  DPWM_REGS21_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS21_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS21_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS21_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS21_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_COUNT0_RESERVED1_MASK) >> DPWM_REGS21_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS21_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS21_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS21_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS21_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_COUNT0_COUNT0_MASK) >> DPWM_REGS21_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS21_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS21_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS21_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS21_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS21_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_COUNT1_RESERVED1_MASK) >> DPWM_REGS21_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS21_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS21_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS21_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS21_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_COUNT1_COUNT1_MASK) >> DPWM_REGS21_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS21_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS21_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS21_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS21_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS21_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_COUNT2_RESERVED1_MASK) >> DPWM_REGS21_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS21_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS21_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS21_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS21_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_COUNT2_COUNT2_MASK) >> DPWM_REGS21_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS21_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS21_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS21_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS21_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS21_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_EN_RESERVED1_MASK) >> DPWM_REGS21_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS21_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS21_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS21_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS21_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS21_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS21_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS21_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS21_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS21_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS21_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS21_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS21_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS21_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS21_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS21_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS21_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS21_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS21_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS21_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_ST_RESERVED1_MASK) >> DPWM_REGS21_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS21_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS21_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS21_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS21_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS21_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS21_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS21_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS21_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS21_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS21_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS21_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS21_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS21_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS21_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS21_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS21_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS21_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS21_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS21_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_CL_RESERVED1_MASK) >> DPWM_REGS21_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS21_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS21_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS21_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS21_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS21_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS21_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS21_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS21_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS21_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS21_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS21_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS21_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS21_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS21_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS21_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS21_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS21_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS21_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS21_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_FO_RESERVED1_MASK) >> DPWM_REGS21_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS21_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS21_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS21_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS21_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS21_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS21_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS21_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS21_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS21_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS21_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS21_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS21_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS21_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS21_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS21_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS21_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS21_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS21_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS21_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS21_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_STATUS_RUNNING_MASK) >> DPWM_REGS21_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS21_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS21_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS21_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS21_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS21_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_STATUS_DELAY_MASK) >> DPWM_REGS21_STATUS_DELAY_SHIFT)
#define  DPWM_REGS21_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS21_STATUS_DELAY_SHIFT))

#define  DPWM_REGS21_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS21_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS21_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_STATUS_SEMI_NUM_MASK) >> DPWM_REGS21_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS21_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS21_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS21_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS21_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS21_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_STATUS_PWM_OUT_MASK) >> DPWM_REGS21_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS21_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS21_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS21_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS21_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS21_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_STATUS_RESERVED1_MASK) >> DPWM_REGS21_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS21_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS21_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS21_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS21_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS21_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS21_STATUS_CUR_COUNT_MASK) >> DPWM_REGS21_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS21_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS21_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS21_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 23 of 32) (dpwm_regs22)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS22_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS22_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS22_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_CFG_RESERVED1_MASK) >> DPWM_REGS22_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS22_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS22_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS22_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS22_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS22_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS22_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS22_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS22_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS22_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS22_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_CFG_HIGH_LOW_MASK) >> DPWM_REGS22_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS22_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS22_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS22_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS22_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS22_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_CFG_OFF_STATE_MASK) >> DPWM_REGS22_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS22_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS22_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS22_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS22_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS22_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS22_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS22_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS22_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS22_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS22_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS22_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_CFG_ENABLE_MASK) >> DPWM_REGS22_CFG_ENABLE_SHIFT)
#define  DPWM_REGS22_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS22_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS22_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS22_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS22_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_COUNT0_RESERVED1_MASK) >> DPWM_REGS22_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS22_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS22_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS22_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS22_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_COUNT0_COUNT0_MASK) >> DPWM_REGS22_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS22_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS22_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS22_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS22_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS22_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_COUNT1_RESERVED1_MASK) >> DPWM_REGS22_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS22_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS22_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS22_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS22_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_COUNT1_COUNT1_MASK) >> DPWM_REGS22_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS22_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS22_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS22_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS22_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS22_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_COUNT2_RESERVED1_MASK) >> DPWM_REGS22_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS22_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS22_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS22_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS22_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_COUNT2_COUNT2_MASK) >> DPWM_REGS22_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS22_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS22_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS22_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS22_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS22_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_EN_RESERVED1_MASK) >> DPWM_REGS22_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS22_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS22_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS22_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS22_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS22_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS22_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS22_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS22_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS22_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS22_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS22_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS22_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS22_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS22_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS22_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS22_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS22_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS22_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS22_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_ST_RESERVED1_MASK) >> DPWM_REGS22_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS22_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS22_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS22_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS22_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS22_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS22_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS22_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS22_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS22_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS22_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS22_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS22_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS22_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS22_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS22_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS22_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS22_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS22_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS22_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_CL_RESERVED1_MASK) >> DPWM_REGS22_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS22_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS22_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS22_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS22_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS22_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS22_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS22_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS22_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS22_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS22_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS22_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS22_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS22_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS22_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS22_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS22_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS22_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS22_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS22_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_FO_RESERVED1_MASK) >> DPWM_REGS22_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS22_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS22_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS22_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS22_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS22_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS22_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS22_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS22_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS22_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS22_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS22_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS22_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS22_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS22_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS22_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS22_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS22_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS22_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS22_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS22_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_STATUS_RUNNING_MASK) >> DPWM_REGS22_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS22_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS22_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS22_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS22_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS22_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_STATUS_DELAY_MASK) >> DPWM_REGS22_STATUS_DELAY_SHIFT)
#define  DPWM_REGS22_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS22_STATUS_DELAY_SHIFT))

#define  DPWM_REGS22_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS22_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS22_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_STATUS_SEMI_NUM_MASK) >> DPWM_REGS22_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS22_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS22_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS22_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS22_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS22_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_STATUS_PWM_OUT_MASK) >> DPWM_REGS22_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS22_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS22_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS22_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS22_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS22_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_STATUS_RESERVED1_MASK) >> DPWM_REGS22_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS22_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS22_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS22_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS22_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS22_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS22_STATUS_CUR_COUNT_MASK) >> DPWM_REGS22_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS22_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS22_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS22_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 24 of 32) (dpwm_regs23)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS23_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS23_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS23_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_CFG_RESERVED1_MASK) >> DPWM_REGS23_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS23_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS23_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS23_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS23_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS23_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS23_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS23_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS23_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS23_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS23_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_CFG_HIGH_LOW_MASK) >> DPWM_REGS23_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS23_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS23_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS23_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS23_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS23_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_CFG_OFF_STATE_MASK) >> DPWM_REGS23_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS23_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS23_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS23_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS23_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS23_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS23_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS23_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS23_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS23_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS23_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS23_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_CFG_ENABLE_MASK) >> DPWM_REGS23_CFG_ENABLE_SHIFT)
#define  DPWM_REGS23_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS23_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS23_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS23_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS23_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_COUNT0_RESERVED1_MASK) >> DPWM_REGS23_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS23_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS23_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS23_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS23_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_COUNT0_COUNT0_MASK) >> DPWM_REGS23_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS23_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS23_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS23_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS23_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS23_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_COUNT1_RESERVED1_MASK) >> DPWM_REGS23_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS23_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS23_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS23_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS23_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_COUNT1_COUNT1_MASK) >> DPWM_REGS23_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS23_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS23_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS23_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS23_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS23_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_COUNT2_RESERVED1_MASK) >> DPWM_REGS23_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS23_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS23_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS23_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS23_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_COUNT2_COUNT2_MASK) >> DPWM_REGS23_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS23_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS23_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS23_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS23_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS23_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_EN_RESERVED1_MASK) >> DPWM_REGS23_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS23_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS23_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS23_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS23_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS23_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS23_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS23_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS23_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS23_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS23_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS23_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS23_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS23_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS23_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS23_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS23_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS23_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS23_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS23_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_ST_RESERVED1_MASK) >> DPWM_REGS23_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS23_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS23_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS23_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS23_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS23_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS23_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS23_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS23_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS23_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS23_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS23_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS23_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS23_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS23_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS23_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS23_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS23_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS23_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS23_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_CL_RESERVED1_MASK) >> DPWM_REGS23_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS23_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS23_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS23_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS23_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS23_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS23_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS23_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS23_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS23_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS23_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS23_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS23_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS23_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS23_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS23_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS23_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS23_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS23_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS23_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_FO_RESERVED1_MASK) >> DPWM_REGS23_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS23_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS23_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS23_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS23_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS23_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS23_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS23_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS23_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS23_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS23_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS23_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS23_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS23_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS23_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS23_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS23_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS23_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS23_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS23_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS23_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_STATUS_RUNNING_MASK) >> DPWM_REGS23_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS23_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS23_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS23_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS23_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS23_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_STATUS_DELAY_MASK) >> DPWM_REGS23_STATUS_DELAY_SHIFT)
#define  DPWM_REGS23_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS23_STATUS_DELAY_SHIFT))

#define  DPWM_REGS23_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS23_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS23_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_STATUS_SEMI_NUM_MASK) >> DPWM_REGS23_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS23_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS23_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS23_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS23_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS23_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_STATUS_PWM_OUT_MASK) >> DPWM_REGS23_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS23_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS23_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS23_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS23_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS23_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_STATUS_RESERVED1_MASK) >> DPWM_REGS23_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS23_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS23_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS23_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS23_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS23_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS23_STATUS_CUR_COUNT_MASK) >> DPWM_REGS23_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS23_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS23_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS23_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 25 of 32) (dpwm_regs24)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS24_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS24_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS24_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_CFG_RESERVED1_MASK) >> DPWM_REGS24_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS24_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS24_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS24_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS24_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS24_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS24_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS24_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS24_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS24_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS24_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_CFG_HIGH_LOW_MASK) >> DPWM_REGS24_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS24_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS24_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS24_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS24_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS24_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_CFG_OFF_STATE_MASK) >> DPWM_REGS24_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS24_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS24_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS24_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS24_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS24_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS24_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS24_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS24_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS24_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS24_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS24_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_CFG_ENABLE_MASK) >> DPWM_REGS24_CFG_ENABLE_SHIFT)
#define  DPWM_REGS24_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS24_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS24_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS24_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS24_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_COUNT0_RESERVED1_MASK) >> DPWM_REGS24_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS24_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS24_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS24_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS24_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_COUNT0_COUNT0_MASK) >> DPWM_REGS24_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS24_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS24_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS24_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS24_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS24_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_COUNT1_RESERVED1_MASK) >> DPWM_REGS24_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS24_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS24_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS24_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS24_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_COUNT1_COUNT1_MASK) >> DPWM_REGS24_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS24_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS24_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS24_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS24_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS24_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_COUNT2_RESERVED1_MASK) >> DPWM_REGS24_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS24_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS24_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS24_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS24_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_COUNT2_COUNT2_MASK) >> DPWM_REGS24_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS24_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS24_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS24_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS24_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS24_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_EN_RESERVED1_MASK) >> DPWM_REGS24_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS24_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS24_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS24_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS24_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS24_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS24_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS24_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS24_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS24_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS24_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS24_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS24_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS24_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS24_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS24_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS24_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS24_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS24_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS24_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_ST_RESERVED1_MASK) >> DPWM_REGS24_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS24_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS24_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS24_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS24_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS24_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS24_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS24_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS24_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS24_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS24_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS24_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS24_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS24_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS24_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS24_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS24_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS24_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS24_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS24_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_CL_RESERVED1_MASK) >> DPWM_REGS24_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS24_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS24_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS24_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS24_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS24_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS24_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS24_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS24_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS24_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS24_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS24_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS24_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS24_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS24_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS24_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS24_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS24_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS24_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS24_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_FO_RESERVED1_MASK) >> DPWM_REGS24_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS24_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS24_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS24_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS24_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS24_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS24_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS24_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS24_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS24_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS24_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS24_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS24_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS24_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS24_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS24_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS24_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS24_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS24_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS24_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS24_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_STATUS_RUNNING_MASK) >> DPWM_REGS24_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS24_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS24_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS24_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS24_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS24_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_STATUS_DELAY_MASK) >> DPWM_REGS24_STATUS_DELAY_SHIFT)
#define  DPWM_REGS24_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS24_STATUS_DELAY_SHIFT))

#define  DPWM_REGS24_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS24_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS24_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_STATUS_SEMI_NUM_MASK) >> DPWM_REGS24_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS24_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS24_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS24_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS24_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS24_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_STATUS_PWM_OUT_MASK) >> DPWM_REGS24_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS24_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS24_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS24_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS24_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS24_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_STATUS_RESERVED1_MASK) >> DPWM_REGS24_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS24_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS24_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS24_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS24_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS24_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS24_STATUS_CUR_COUNT_MASK) >> DPWM_REGS24_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS24_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS24_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS24_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 26 of 32) (dpwm_regs25)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS25_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS25_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS25_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_CFG_RESERVED1_MASK) >> DPWM_REGS25_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS25_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS25_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS25_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS25_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS25_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS25_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS25_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS25_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS25_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS25_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_CFG_HIGH_LOW_MASK) >> DPWM_REGS25_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS25_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS25_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS25_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS25_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS25_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_CFG_OFF_STATE_MASK) >> DPWM_REGS25_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS25_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS25_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS25_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS25_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS25_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS25_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS25_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS25_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS25_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS25_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS25_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_CFG_ENABLE_MASK) >> DPWM_REGS25_CFG_ENABLE_SHIFT)
#define  DPWM_REGS25_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS25_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS25_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS25_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS25_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_COUNT0_RESERVED1_MASK) >> DPWM_REGS25_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS25_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS25_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS25_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS25_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_COUNT0_COUNT0_MASK) >> DPWM_REGS25_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS25_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS25_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS25_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS25_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS25_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_COUNT1_RESERVED1_MASK) >> DPWM_REGS25_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS25_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS25_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS25_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS25_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_COUNT1_COUNT1_MASK) >> DPWM_REGS25_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS25_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS25_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS25_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS25_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS25_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_COUNT2_RESERVED1_MASK) >> DPWM_REGS25_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS25_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS25_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS25_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS25_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_COUNT2_COUNT2_MASK) >> DPWM_REGS25_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS25_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS25_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS25_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS25_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS25_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_EN_RESERVED1_MASK) >> DPWM_REGS25_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS25_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS25_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS25_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS25_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS25_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS25_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS25_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS25_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS25_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS25_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS25_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS25_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS25_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS25_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS25_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS25_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS25_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS25_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS25_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_ST_RESERVED1_MASK) >> DPWM_REGS25_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS25_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS25_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS25_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS25_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS25_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS25_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS25_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS25_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS25_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS25_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS25_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS25_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS25_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS25_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS25_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS25_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS25_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS25_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS25_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_CL_RESERVED1_MASK) >> DPWM_REGS25_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS25_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS25_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS25_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS25_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS25_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS25_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS25_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS25_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS25_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS25_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS25_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS25_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS25_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS25_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS25_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS25_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS25_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS25_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS25_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_FO_RESERVED1_MASK) >> DPWM_REGS25_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS25_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS25_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS25_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS25_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS25_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS25_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS25_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS25_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS25_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS25_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS25_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS25_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS25_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS25_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS25_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS25_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS25_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS25_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS25_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS25_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_STATUS_RUNNING_MASK) >> DPWM_REGS25_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS25_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS25_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS25_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS25_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS25_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_STATUS_DELAY_MASK) >> DPWM_REGS25_STATUS_DELAY_SHIFT)
#define  DPWM_REGS25_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS25_STATUS_DELAY_SHIFT))

#define  DPWM_REGS25_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS25_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS25_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_STATUS_SEMI_NUM_MASK) >> DPWM_REGS25_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS25_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS25_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS25_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS25_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS25_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_STATUS_PWM_OUT_MASK) >> DPWM_REGS25_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS25_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS25_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS25_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS25_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS25_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_STATUS_RESERVED1_MASK) >> DPWM_REGS25_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS25_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS25_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS25_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS25_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS25_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS25_STATUS_CUR_COUNT_MASK) >> DPWM_REGS25_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS25_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS25_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS25_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 27 of 32) (dpwm_regs26)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS26_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS26_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS26_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_CFG_RESERVED1_MASK) >> DPWM_REGS26_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS26_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS26_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS26_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS26_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS26_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS26_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS26_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS26_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS26_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS26_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_CFG_HIGH_LOW_MASK) >> DPWM_REGS26_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS26_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS26_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS26_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS26_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS26_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_CFG_OFF_STATE_MASK) >> DPWM_REGS26_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS26_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS26_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS26_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS26_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS26_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS26_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS26_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS26_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS26_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS26_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS26_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_CFG_ENABLE_MASK) >> DPWM_REGS26_CFG_ENABLE_SHIFT)
#define  DPWM_REGS26_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS26_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS26_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS26_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS26_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_COUNT0_RESERVED1_MASK) >> DPWM_REGS26_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS26_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS26_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS26_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS26_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_COUNT0_COUNT0_MASK) >> DPWM_REGS26_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS26_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS26_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS26_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS26_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS26_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_COUNT1_RESERVED1_MASK) >> DPWM_REGS26_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS26_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS26_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS26_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS26_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_COUNT1_COUNT1_MASK) >> DPWM_REGS26_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS26_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS26_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS26_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS26_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS26_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_COUNT2_RESERVED1_MASK) >> DPWM_REGS26_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS26_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS26_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS26_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS26_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_COUNT2_COUNT2_MASK) >> DPWM_REGS26_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS26_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS26_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS26_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS26_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS26_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_EN_RESERVED1_MASK) >> DPWM_REGS26_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS26_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS26_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS26_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS26_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS26_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS26_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS26_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS26_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS26_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS26_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS26_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS26_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS26_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS26_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS26_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS26_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS26_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS26_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS26_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_ST_RESERVED1_MASK) >> DPWM_REGS26_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS26_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS26_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS26_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS26_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS26_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS26_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS26_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS26_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS26_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS26_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS26_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS26_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS26_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS26_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS26_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS26_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS26_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS26_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS26_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_CL_RESERVED1_MASK) >> DPWM_REGS26_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS26_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS26_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS26_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS26_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS26_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS26_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS26_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS26_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS26_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS26_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS26_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS26_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS26_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS26_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS26_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS26_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS26_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS26_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS26_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_FO_RESERVED1_MASK) >> DPWM_REGS26_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS26_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS26_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS26_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS26_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS26_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS26_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS26_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS26_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS26_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS26_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS26_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS26_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS26_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS26_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS26_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS26_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS26_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS26_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS26_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS26_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_STATUS_RUNNING_MASK) >> DPWM_REGS26_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS26_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS26_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS26_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS26_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS26_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_STATUS_DELAY_MASK) >> DPWM_REGS26_STATUS_DELAY_SHIFT)
#define  DPWM_REGS26_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS26_STATUS_DELAY_SHIFT))

#define  DPWM_REGS26_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS26_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS26_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_STATUS_SEMI_NUM_MASK) >> DPWM_REGS26_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS26_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS26_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS26_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS26_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS26_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_STATUS_PWM_OUT_MASK) >> DPWM_REGS26_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS26_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS26_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS26_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS26_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS26_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_STATUS_RESERVED1_MASK) >> DPWM_REGS26_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS26_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS26_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS26_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS26_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS26_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS26_STATUS_CUR_COUNT_MASK) >> DPWM_REGS26_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS26_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS26_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS26_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 28 of 32) (dpwm_regs27)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS27_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS27_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS27_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_CFG_RESERVED1_MASK) >> DPWM_REGS27_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS27_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS27_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS27_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS27_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS27_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS27_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS27_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS27_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS27_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS27_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_CFG_HIGH_LOW_MASK) >> DPWM_REGS27_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS27_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS27_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS27_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS27_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS27_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_CFG_OFF_STATE_MASK) >> DPWM_REGS27_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS27_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS27_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS27_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS27_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS27_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS27_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS27_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS27_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS27_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS27_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS27_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_CFG_ENABLE_MASK) >> DPWM_REGS27_CFG_ENABLE_SHIFT)
#define  DPWM_REGS27_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS27_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS27_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS27_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS27_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_COUNT0_RESERVED1_MASK) >> DPWM_REGS27_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS27_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS27_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS27_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS27_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_COUNT0_COUNT0_MASK) >> DPWM_REGS27_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS27_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS27_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS27_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS27_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS27_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_COUNT1_RESERVED1_MASK) >> DPWM_REGS27_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS27_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS27_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS27_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS27_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_COUNT1_COUNT1_MASK) >> DPWM_REGS27_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS27_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS27_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS27_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS27_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS27_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_COUNT2_RESERVED1_MASK) >> DPWM_REGS27_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS27_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS27_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS27_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS27_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_COUNT2_COUNT2_MASK) >> DPWM_REGS27_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS27_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS27_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS27_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS27_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS27_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_EN_RESERVED1_MASK) >> DPWM_REGS27_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS27_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS27_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS27_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS27_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS27_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS27_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS27_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS27_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS27_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS27_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS27_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS27_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS27_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS27_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS27_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS27_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS27_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS27_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS27_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_ST_RESERVED1_MASK) >> DPWM_REGS27_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS27_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS27_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS27_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS27_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS27_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS27_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS27_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS27_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS27_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS27_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS27_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS27_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS27_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS27_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS27_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS27_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS27_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS27_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS27_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_CL_RESERVED1_MASK) >> DPWM_REGS27_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS27_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS27_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS27_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS27_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS27_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS27_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS27_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS27_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS27_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS27_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS27_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS27_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS27_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS27_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS27_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS27_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS27_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS27_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS27_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_FO_RESERVED1_MASK) >> DPWM_REGS27_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS27_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS27_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS27_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS27_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS27_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS27_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS27_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS27_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS27_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS27_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS27_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS27_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS27_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS27_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS27_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS27_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS27_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS27_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS27_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS27_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_STATUS_RUNNING_MASK) >> DPWM_REGS27_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS27_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS27_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS27_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS27_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS27_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_STATUS_DELAY_MASK) >> DPWM_REGS27_STATUS_DELAY_SHIFT)
#define  DPWM_REGS27_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS27_STATUS_DELAY_SHIFT))

#define  DPWM_REGS27_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS27_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS27_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_STATUS_SEMI_NUM_MASK) >> DPWM_REGS27_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS27_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS27_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS27_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS27_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS27_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_STATUS_PWM_OUT_MASK) >> DPWM_REGS27_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS27_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS27_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS27_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS27_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS27_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_STATUS_RESERVED1_MASK) >> DPWM_REGS27_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS27_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS27_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS27_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS27_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS27_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS27_STATUS_CUR_COUNT_MASK) >> DPWM_REGS27_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS27_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS27_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS27_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 29 of 32) (dpwm_regs28)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS28_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS28_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS28_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_CFG_RESERVED1_MASK) >> DPWM_REGS28_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS28_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS28_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS28_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS28_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS28_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS28_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS28_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS28_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS28_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS28_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_CFG_HIGH_LOW_MASK) >> DPWM_REGS28_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS28_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS28_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS28_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS28_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS28_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_CFG_OFF_STATE_MASK) >> DPWM_REGS28_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS28_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS28_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS28_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS28_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS28_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS28_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS28_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS28_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS28_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS28_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS28_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_CFG_ENABLE_MASK) >> DPWM_REGS28_CFG_ENABLE_SHIFT)
#define  DPWM_REGS28_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS28_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS28_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS28_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS28_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_COUNT0_RESERVED1_MASK) >> DPWM_REGS28_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS28_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS28_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS28_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS28_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_COUNT0_COUNT0_MASK) >> DPWM_REGS28_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS28_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS28_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS28_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS28_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS28_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_COUNT1_RESERVED1_MASK) >> DPWM_REGS28_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS28_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS28_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS28_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS28_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_COUNT1_COUNT1_MASK) >> DPWM_REGS28_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS28_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS28_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS28_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS28_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS28_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_COUNT2_RESERVED1_MASK) >> DPWM_REGS28_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS28_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS28_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS28_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS28_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_COUNT2_COUNT2_MASK) >> DPWM_REGS28_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS28_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS28_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS28_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS28_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS28_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_EN_RESERVED1_MASK) >> DPWM_REGS28_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS28_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS28_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS28_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS28_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS28_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS28_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS28_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS28_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS28_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS28_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS28_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS28_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS28_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS28_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS28_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS28_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS28_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS28_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS28_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_ST_RESERVED1_MASK) >> DPWM_REGS28_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS28_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS28_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS28_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS28_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS28_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS28_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS28_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS28_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS28_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS28_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS28_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS28_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS28_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS28_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS28_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS28_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS28_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS28_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS28_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_CL_RESERVED1_MASK) >> DPWM_REGS28_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS28_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS28_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS28_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS28_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS28_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS28_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS28_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS28_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS28_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS28_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS28_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS28_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS28_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS28_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS28_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS28_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS28_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS28_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS28_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_FO_RESERVED1_MASK) >> DPWM_REGS28_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS28_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS28_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS28_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS28_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS28_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS28_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS28_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS28_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS28_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS28_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS28_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS28_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS28_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS28_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS28_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS28_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS28_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS28_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS28_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS28_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_STATUS_RUNNING_MASK) >> DPWM_REGS28_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS28_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS28_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS28_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS28_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS28_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_STATUS_DELAY_MASK) >> DPWM_REGS28_STATUS_DELAY_SHIFT)
#define  DPWM_REGS28_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS28_STATUS_DELAY_SHIFT))

#define  DPWM_REGS28_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS28_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS28_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_STATUS_SEMI_NUM_MASK) >> DPWM_REGS28_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS28_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS28_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS28_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS28_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS28_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_STATUS_PWM_OUT_MASK) >> DPWM_REGS28_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS28_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS28_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS28_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS28_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS28_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_STATUS_RESERVED1_MASK) >> DPWM_REGS28_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS28_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS28_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS28_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS28_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS28_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS28_STATUS_CUR_COUNT_MASK) >> DPWM_REGS28_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS28_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS28_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS28_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 30 of 32) (dpwm_regs29)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS29_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS29_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS29_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_CFG_RESERVED1_MASK) >> DPWM_REGS29_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS29_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS29_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS29_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS29_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS29_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS29_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS29_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS29_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS29_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS29_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_CFG_HIGH_LOW_MASK) >> DPWM_REGS29_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS29_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS29_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS29_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS29_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS29_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_CFG_OFF_STATE_MASK) >> DPWM_REGS29_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS29_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS29_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS29_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS29_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS29_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS29_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS29_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS29_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS29_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS29_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS29_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_CFG_ENABLE_MASK) >> DPWM_REGS29_CFG_ENABLE_SHIFT)
#define  DPWM_REGS29_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS29_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS29_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS29_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS29_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_COUNT0_RESERVED1_MASK) >> DPWM_REGS29_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS29_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS29_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS29_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS29_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_COUNT0_COUNT0_MASK) >> DPWM_REGS29_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS29_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS29_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS29_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS29_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS29_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_COUNT1_RESERVED1_MASK) >> DPWM_REGS29_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS29_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS29_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS29_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS29_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_COUNT1_COUNT1_MASK) >> DPWM_REGS29_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS29_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS29_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS29_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS29_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS29_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_COUNT2_RESERVED1_MASK) >> DPWM_REGS29_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS29_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS29_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS29_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS29_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_COUNT2_COUNT2_MASK) >> DPWM_REGS29_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS29_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS29_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS29_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS29_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS29_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_EN_RESERVED1_MASK) >> DPWM_REGS29_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS29_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS29_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS29_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS29_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS29_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS29_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS29_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS29_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS29_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS29_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS29_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS29_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS29_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS29_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS29_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS29_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS29_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS29_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS29_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_ST_RESERVED1_MASK) >> DPWM_REGS29_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS29_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS29_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS29_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS29_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS29_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS29_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS29_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS29_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS29_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS29_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS29_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS29_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS29_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS29_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS29_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS29_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS29_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS29_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS29_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_CL_RESERVED1_MASK) >> DPWM_REGS29_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS29_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS29_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS29_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS29_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS29_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS29_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS29_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS29_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS29_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS29_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS29_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS29_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS29_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS29_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS29_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS29_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS29_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS29_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS29_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_FO_RESERVED1_MASK) >> DPWM_REGS29_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS29_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS29_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS29_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS29_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS29_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS29_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS29_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS29_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS29_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS29_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS29_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS29_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS29_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS29_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS29_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS29_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS29_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS29_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS29_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS29_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_STATUS_RUNNING_MASK) >> DPWM_REGS29_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS29_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS29_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS29_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS29_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS29_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_STATUS_DELAY_MASK) >> DPWM_REGS29_STATUS_DELAY_SHIFT)
#define  DPWM_REGS29_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS29_STATUS_DELAY_SHIFT))

#define  DPWM_REGS29_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS29_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS29_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_STATUS_SEMI_NUM_MASK) >> DPWM_REGS29_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS29_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS29_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS29_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS29_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS29_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_STATUS_PWM_OUT_MASK) >> DPWM_REGS29_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS29_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS29_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS29_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS29_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS29_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_STATUS_RESERVED1_MASK) >> DPWM_REGS29_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS29_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS29_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS29_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS29_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS29_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS29_STATUS_CUR_COUNT_MASK) >> DPWM_REGS29_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS29_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS29_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS29_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 31 of 32) (dpwm_regs30)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS30_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS30_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS30_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_CFG_RESERVED1_MASK) >> DPWM_REGS30_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS30_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS30_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS30_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS30_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS30_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS30_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS30_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS30_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS30_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS30_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_CFG_HIGH_LOW_MASK) >> DPWM_REGS30_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS30_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS30_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS30_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS30_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS30_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_CFG_OFF_STATE_MASK) >> DPWM_REGS30_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS30_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS30_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS30_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS30_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS30_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS30_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS30_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS30_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS30_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS30_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS30_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_CFG_ENABLE_MASK) >> DPWM_REGS30_CFG_ENABLE_SHIFT)
#define  DPWM_REGS30_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS30_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS30_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS30_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS30_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_COUNT0_RESERVED1_MASK) >> DPWM_REGS30_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS30_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS30_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS30_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS30_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_COUNT0_COUNT0_MASK) >> DPWM_REGS30_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS30_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS30_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS30_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS30_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS30_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_COUNT1_RESERVED1_MASK) >> DPWM_REGS30_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS30_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS30_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS30_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS30_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_COUNT1_COUNT1_MASK) >> DPWM_REGS30_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS30_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS30_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS30_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS30_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS30_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_COUNT2_RESERVED1_MASK) >> DPWM_REGS30_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS30_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS30_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS30_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS30_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_COUNT2_COUNT2_MASK) >> DPWM_REGS30_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS30_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS30_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS30_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS30_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS30_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_EN_RESERVED1_MASK) >> DPWM_REGS30_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS30_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS30_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS30_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS30_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS30_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS30_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS30_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS30_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS30_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS30_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS30_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS30_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS30_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS30_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS30_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS30_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS30_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS30_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS30_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_ST_RESERVED1_MASK) >> DPWM_REGS30_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS30_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS30_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS30_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS30_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS30_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS30_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS30_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS30_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS30_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS30_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS30_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS30_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS30_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS30_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS30_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS30_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS30_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS30_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS30_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_CL_RESERVED1_MASK) >> DPWM_REGS30_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS30_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS30_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS30_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS30_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS30_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS30_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS30_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS30_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS30_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS30_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS30_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS30_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS30_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS30_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS30_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS30_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS30_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS30_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS30_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_FO_RESERVED1_MASK) >> DPWM_REGS30_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS30_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS30_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS30_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS30_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS30_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS30_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS30_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS30_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS30_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS30_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS30_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS30_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS30_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS30_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS30_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS30_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS30_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS30_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS30_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS30_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_STATUS_RUNNING_MASK) >> DPWM_REGS30_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS30_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS30_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS30_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS30_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS30_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_STATUS_DELAY_MASK) >> DPWM_REGS30_STATUS_DELAY_SHIFT)
#define  DPWM_REGS30_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS30_STATUS_DELAY_SHIFT))

#define  DPWM_REGS30_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS30_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS30_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_STATUS_SEMI_NUM_MASK) >> DPWM_REGS30_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS30_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS30_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS30_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS30_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS30_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_STATUS_PWM_OUT_MASK) >> DPWM_REGS30_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS30_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS30_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS30_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS30_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS30_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_STATUS_RESERVED1_MASK) >> DPWM_REGS30_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS30_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS30_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS30_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS30_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS30_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS30_STATUS_CUR_COUNT_MASK) >> DPWM_REGS30_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS30_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS30_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS30_STATUS_CUR_COUNT_SHIFT))

//====================================================================
//
//Register File: DPWM (Instance 32 of 32) (dpwm_regs31)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  DPWM_REGS31_CFG_RESERVED1_MASK 0xffffff00
#define  DPWM_REGS31_CFG_RESERVED1_SHIFT 8
#define  DPWM_REGS31_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_CFG_RESERVED1_MASK) >> DPWM_REGS31_CFG_RESERVED1_SHIFT)
#define  DPWM_REGS31_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_CFG_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_CFG_RESERVED1_SHIFT))

#define  DPWM_REGS31_CFG_TIMEBASE_SEL_MASK 0xf0
#define  DPWM_REGS31_CFG_TIMEBASE_SEL_SHIFT 4
#define  DPWM_REGS31_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_CFG_TIMEBASE_SEL_MASK) >> DPWM_REGS31_CFG_TIMEBASE_SEL_SHIFT)
#define  DPWM_REGS31_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << DPWM_REGS31_CFG_TIMEBASE_SEL_SHIFT))

#define  DPWM_REGS31_CFG_HIGH_LOW_MASK 0x8
#define  DPWM_REGS31_CFG_HIGH_LOW_SHIFT 3
#define  DPWM_REGS31_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_CFG_HIGH_LOW_MASK) >> DPWM_REGS31_CFG_HIGH_LOW_SHIFT)
#define  DPWM_REGS31_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << DPWM_REGS31_CFG_HIGH_LOW_SHIFT))

#define  DPWM_REGS31_CFG_OFF_STATE_MASK 0x4
#define  DPWM_REGS31_CFG_OFF_STATE_SHIFT 2
#define  DPWM_REGS31_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_CFG_OFF_STATE_MASK) >> DPWM_REGS31_CFG_OFF_STATE_SHIFT)
#define  DPWM_REGS31_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_CFG_OFF_STATE_MASK) | (((uint32_t)val) << DPWM_REGS31_CFG_OFF_STATE_SHIFT))

#define  DPWM_REGS31_CFG_SOFT_DISABLE_MASK 0x2
#define  DPWM_REGS31_CFG_SOFT_DISABLE_SHIFT 1
#define  DPWM_REGS31_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_CFG_SOFT_DISABLE_MASK) >> DPWM_REGS31_CFG_SOFT_DISABLE_SHIFT)
#define  DPWM_REGS31_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << DPWM_REGS31_CFG_SOFT_DISABLE_SHIFT))

#define  DPWM_REGS31_CFG_ENABLE_MASK 0x1
#define  DPWM_REGS31_CFG_ENABLE_SHIFT 0
#define  DPWM_REGS31_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_CFG_ENABLE_MASK) >> DPWM_REGS31_CFG_ENABLE_SHIFT)
#define  DPWM_REGS31_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_CFG_ENABLE_MASK) | (((uint32_t)val) << DPWM_REGS31_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  DPWM_REGS31_COUNT0_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS31_COUNT0_RESERVED1_SHIFT 16
#define  DPWM_REGS31_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_COUNT0_RESERVED1_MASK) >> DPWM_REGS31_COUNT0_RESERVED1_SHIFT)
#define  DPWM_REGS31_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_COUNT0_RESERVED1_SHIFT))

#define  DPWM_REGS31_COUNT0_COUNT0_MASK 0xffff
#define  DPWM_REGS31_COUNT0_COUNT0_SHIFT 0
#define  DPWM_REGS31_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_COUNT0_COUNT0_MASK) >> DPWM_REGS31_COUNT0_COUNT0_SHIFT)
#define  DPWM_REGS31_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_COUNT0_COUNT0_MASK) | (((uint32_t)val) << DPWM_REGS31_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  DPWM_REGS31_COUNT1_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS31_COUNT1_RESERVED1_SHIFT 16
#define  DPWM_REGS31_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_COUNT1_RESERVED1_MASK) >> DPWM_REGS31_COUNT1_RESERVED1_SHIFT)
#define  DPWM_REGS31_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_COUNT1_RESERVED1_SHIFT))

#define  DPWM_REGS31_COUNT1_COUNT1_MASK 0xffff
#define  DPWM_REGS31_COUNT1_COUNT1_SHIFT 0
#define  DPWM_REGS31_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_COUNT1_COUNT1_MASK) >> DPWM_REGS31_COUNT1_COUNT1_SHIFT)
#define  DPWM_REGS31_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_COUNT1_COUNT1_MASK) | (((uint32_t)val) << DPWM_REGS31_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  DPWM_REGS31_COUNT2_RESERVED1_MASK 0xffff0000
#define  DPWM_REGS31_COUNT2_RESERVED1_SHIFT 16
#define  DPWM_REGS31_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_COUNT2_RESERVED1_MASK) >> DPWM_REGS31_COUNT2_RESERVED1_SHIFT)
#define  DPWM_REGS31_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_COUNT2_RESERVED1_SHIFT))

#define  DPWM_REGS31_COUNT2_COUNT2_MASK 0xffff
#define  DPWM_REGS31_COUNT2_COUNT2_SHIFT 0
#define  DPWM_REGS31_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_COUNT2_COUNT2_MASK) >> DPWM_REGS31_COUNT2_COUNT2_SHIFT)
#define  DPWM_REGS31_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_COUNT2_COUNT2_MASK) | (((uint32_t)val) << DPWM_REGS31_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  DPWM_REGS31_INT_EN_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS31_INT_EN_RESERVED1_SHIFT 3
#define  DPWM_REGS31_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_EN_RESERVED1_MASK) >> DPWM_REGS31_INT_EN_RESERVED1_SHIFT)
#define  DPWM_REGS31_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_EN_RESERVED1_SHIFT))

#define  DPWM_REGS31_INT_EN_DONE_INT_EN_MASK 0x4
#define  DPWM_REGS31_INT_EN_DONE_INT_EN_SHIFT 2
#define  DPWM_REGS31_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_EN_DONE_INT_EN_MASK) >> DPWM_REGS31_INT_EN_DONE_INT_EN_SHIFT)
#define  DPWM_REGS31_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_EN_DONE_INT_EN_SHIFT))

#define  DPWM_REGS31_INT_EN_FALL_INT_EN_MASK 0x2
#define  DPWM_REGS31_INT_EN_FALL_INT_EN_SHIFT 1
#define  DPWM_REGS31_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_EN_FALL_INT_EN_MASK) >> DPWM_REGS31_INT_EN_FALL_INT_EN_SHIFT)
#define  DPWM_REGS31_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_EN_FALL_INT_EN_SHIFT))

#define  DPWM_REGS31_INT_EN_RISE_INT_EN_MASK 0x1
#define  DPWM_REGS31_INT_EN_RISE_INT_EN_SHIFT 0
#define  DPWM_REGS31_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_EN_RISE_INT_EN_MASK) >> DPWM_REGS31_INT_EN_RISE_INT_EN_SHIFT)
#define  DPWM_REGS31_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  DPWM_REGS31_INT_ST_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS31_INT_ST_RESERVED1_SHIFT 3
#define  DPWM_REGS31_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_ST_RESERVED1_MASK) >> DPWM_REGS31_INT_ST_RESERVED1_SHIFT)
#define  DPWM_REGS31_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_ST_RESERVED1_SHIFT))

#define  DPWM_REGS31_INT_ST_DONE_INT_ST_MASK 0x4
#define  DPWM_REGS31_INT_ST_DONE_INT_ST_SHIFT 2
#define  DPWM_REGS31_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_ST_DONE_INT_ST_MASK) >> DPWM_REGS31_INT_ST_DONE_INT_ST_SHIFT)
#define  DPWM_REGS31_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_ST_DONE_INT_ST_SHIFT))

#define  DPWM_REGS31_INT_ST_FALL_INT_ST_MASK 0x2
#define  DPWM_REGS31_INT_ST_FALL_INT_ST_SHIFT 1
#define  DPWM_REGS31_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_ST_FALL_INT_ST_MASK) >> DPWM_REGS31_INT_ST_FALL_INT_ST_SHIFT)
#define  DPWM_REGS31_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_ST_FALL_INT_ST_SHIFT))

#define  DPWM_REGS31_INT_ST_RISE_INT_ST_MASK 0x1
#define  DPWM_REGS31_INT_ST_RISE_INT_ST_SHIFT 0
#define  DPWM_REGS31_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_ST_RISE_INT_ST_MASK) >> DPWM_REGS31_INT_ST_RISE_INT_ST_SHIFT)
#define  DPWM_REGS31_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  DPWM_REGS31_INT_CL_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS31_INT_CL_RESERVED1_SHIFT 3
#define  DPWM_REGS31_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_CL_RESERVED1_MASK) >> DPWM_REGS31_INT_CL_RESERVED1_SHIFT)
#define  DPWM_REGS31_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_CL_RESERVED1_SHIFT))

#define  DPWM_REGS31_INT_CL_DONE_INT_CL_MASK 0x4
#define  DPWM_REGS31_INT_CL_DONE_INT_CL_SHIFT 2
#define  DPWM_REGS31_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_CL_DONE_INT_CL_MASK) >> DPWM_REGS31_INT_CL_DONE_INT_CL_SHIFT)
#define  DPWM_REGS31_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_CL_DONE_INT_CL_SHIFT))

#define  DPWM_REGS31_INT_CL_FALL_INT_CL_MASK 0x2
#define  DPWM_REGS31_INT_CL_FALL_INT_CL_SHIFT 1
#define  DPWM_REGS31_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_CL_FALL_INT_CL_MASK) >> DPWM_REGS31_INT_CL_FALL_INT_CL_SHIFT)
#define  DPWM_REGS31_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_CL_FALL_INT_CL_SHIFT))

#define  DPWM_REGS31_INT_CL_RISE_INT_CL_MASK 0x1
#define  DPWM_REGS31_INT_CL_RISE_INT_CL_SHIFT 0
#define  DPWM_REGS31_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_CL_RISE_INT_CL_MASK) >> DPWM_REGS31_INT_CL_RISE_INT_CL_SHIFT)
#define  DPWM_REGS31_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  DPWM_REGS31_INT_FO_RESERVED1_MASK 0xfffffff8
#define  DPWM_REGS31_INT_FO_RESERVED1_SHIFT 3
#define  DPWM_REGS31_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_FO_RESERVED1_MASK) >> DPWM_REGS31_INT_FO_RESERVED1_SHIFT)
#define  DPWM_REGS31_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_FO_RESERVED1_SHIFT))

#define  DPWM_REGS31_INT_FO_DONE_INT_FO_MASK 0x4
#define  DPWM_REGS31_INT_FO_DONE_INT_FO_SHIFT 2
#define  DPWM_REGS31_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_FO_DONE_INT_FO_MASK) >> DPWM_REGS31_INT_FO_DONE_INT_FO_SHIFT)
#define  DPWM_REGS31_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_FO_DONE_INT_FO_SHIFT))

#define  DPWM_REGS31_INT_FO_FALL_INT_FO_MASK 0x2
#define  DPWM_REGS31_INT_FO_FALL_INT_FO_SHIFT 1
#define  DPWM_REGS31_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_FO_FALL_INT_FO_MASK) >> DPWM_REGS31_INT_FO_FALL_INT_FO_SHIFT)
#define  DPWM_REGS31_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_FO_FALL_INT_FO_SHIFT))

#define  DPWM_REGS31_INT_FO_RISE_INT_FO_MASK 0x1
#define  DPWM_REGS31_INT_FO_RISE_INT_FO_SHIFT 0
#define  DPWM_REGS31_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_INT_FO_RISE_INT_FO_MASK) >> DPWM_REGS31_INT_FO_RISE_INT_FO_SHIFT)
#define  DPWM_REGS31_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << DPWM_REGS31_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  DPWM_REGS31_STATUS_RUNNING_MASK 0x80000000
#define  DPWM_REGS31_STATUS_RUNNING_SHIFT 31
#define  DPWM_REGS31_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_STATUS_RUNNING_MASK) >> DPWM_REGS31_STATUS_RUNNING_SHIFT)
#define  DPWM_REGS31_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_STATUS_RUNNING_MASK) | (((uint32_t)val) << DPWM_REGS31_STATUS_RUNNING_SHIFT))

#define  DPWM_REGS31_STATUS_DELAY_MASK 0x40000000
#define  DPWM_REGS31_STATUS_DELAY_SHIFT 30
#define  DPWM_REGS31_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_STATUS_DELAY_MASK) >> DPWM_REGS31_STATUS_DELAY_SHIFT)
#define  DPWM_REGS31_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_STATUS_DELAY_MASK) | (((uint32_t)val) << DPWM_REGS31_STATUS_DELAY_SHIFT))

#define  DPWM_REGS31_STATUS_SEMI_NUM_MASK 0x20000000
#define  DPWM_REGS31_STATUS_SEMI_NUM_SHIFT 29
#define  DPWM_REGS31_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_STATUS_SEMI_NUM_MASK) >> DPWM_REGS31_STATUS_SEMI_NUM_SHIFT)
#define  DPWM_REGS31_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << DPWM_REGS31_STATUS_SEMI_NUM_SHIFT))

#define  DPWM_REGS31_STATUS_PWM_OUT_MASK 0x10000000
#define  DPWM_REGS31_STATUS_PWM_OUT_SHIFT 28
#define  DPWM_REGS31_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_STATUS_PWM_OUT_MASK) >> DPWM_REGS31_STATUS_PWM_OUT_SHIFT)
#define  DPWM_REGS31_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << DPWM_REGS31_STATUS_PWM_OUT_SHIFT))

#define  DPWM_REGS31_STATUS_RESERVED1_MASK 0xfff0000
#define  DPWM_REGS31_STATUS_RESERVED1_SHIFT 16
#define  DPWM_REGS31_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_STATUS_RESERVED1_MASK) >> DPWM_REGS31_STATUS_RESERVED1_SHIFT)
#define  DPWM_REGS31_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_STATUS_RESERVED1_MASK) | (((uint32_t)val) << DPWM_REGS31_STATUS_RESERVED1_SHIFT))

#define  DPWM_REGS31_STATUS_CUR_COUNT_MASK 0xffff
#define  DPWM_REGS31_STATUS_CUR_COUNT_SHIFT 0
#define  DPWM_REGS31_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & DPWM_REGS31_STATUS_CUR_COUNT_MASK) >> DPWM_REGS31_STATUS_CUR_COUNT_SHIFT)
#define  DPWM_REGS31_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DPWM_REGS31_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << DPWM_REGS31_STATUS_CUR_COUNT_SHIFT))

#endif // DPWM_TOP_REGS
