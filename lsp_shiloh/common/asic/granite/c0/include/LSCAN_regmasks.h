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
 * \file LSCAN_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _LSCAN_REGMASKS_H_
#define _LSCAN_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: LSCAN (LSCAN)
//
//====================================================================

//====================================================================
//
//Register File: HSync Control (hsync)
//
//====================================================================

//====================================================================
//Register: HSync Configuration register (hcfg)
//====================================================================

#define  HSYNC_HCFG_RESERVED1_MASK 0xe0000000
#define  HSYNC_HCFG_RESERVED1_SHIFT 29
#define  HSYNC_HCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_RESERVED1_MASK) >> HSYNC_HCFG_RESERVED1_SHIFT)
#define  HSYNC_HCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_HCFG_RESERVED1_SHIFT))

#define  HSYNC_HCFG_GALVO_EN_MASK 0x10000000
#define  HSYNC_HCFG_GALVO_EN_SHIFT 28
#define  HSYNC_HCFG_GALVO_EN_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_GALVO_EN_MASK) >> HSYNC_HCFG_GALVO_EN_SHIFT)
#define  HSYNC_HCFG_GALVO_EN_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_GALVO_EN_MASK) | (((uint32_t)val) << HSYNC_HCFG_GALVO_EN_SHIFT))

#define  HSYNC_HCFG_WFG_ENABLE_MASK 0xf000000
#define  HSYNC_HCFG_WFG_ENABLE_SHIFT 24
#define  HSYNC_HCFG_WFG_ENABLE_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_WFG_ENABLE_MASK) >> HSYNC_HCFG_WFG_ENABLE_SHIFT)
#define  HSYNC_HCFG_WFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_WFG_ENABLE_MASK) | (((uint32_t)val) << HSYNC_HCFG_WFG_ENABLE_SHIFT))

#define  HSYNC_HCFG_SOS_SEL_MASK 0x800000
#define  HSYNC_HCFG_SOS_SEL_SHIFT 23
#define  HSYNC_HCFG_SOS_SEL_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_SOS_SEL_MASK) >> HSYNC_HCFG_SOS_SEL_SHIFT)
#define  HSYNC_HCFG_SOS_SEL_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_SOS_SEL_MASK) | (((uint32_t)val) << HSYNC_HCFG_SOS_SEL_SHIFT))

#define  HSYNC_HCFG_RESERVED2_MASK 0x400000
#define  HSYNC_HCFG_RESERVED2_SHIFT 22
#define  HSYNC_HCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_RESERVED2_MASK) >> HSYNC_HCFG_RESERVED2_SHIFT)
#define  HSYNC_HCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_RESERVED2_MASK) | (((uint32_t)val) << HSYNC_HCFG_RESERVED2_SHIFT))

#define  HSYNC_HCFG_PULSE_TYPE_MASK 0x300000
#define  HSYNC_HCFG_PULSE_TYPE_SHIFT 20
#define  HSYNC_HCFG_PULSE_TYPE_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_PULSE_TYPE_MASK) >> HSYNC_HCFG_PULSE_TYPE_SHIFT)
#define  HSYNC_HCFG_PULSE_TYPE_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_PULSE_TYPE_MASK) | (((uint32_t)val) << HSYNC_HCFG_PULSE_TYPE_SHIFT))

#define  HSYNC_HCFG_RESERVED3_MASK 0xc0000
#define  HSYNC_HCFG_RESERVED3_SHIFT 18
#define  HSYNC_HCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_RESERVED3_MASK) >> HSYNC_HCFG_RESERVED3_SHIFT)
#define  HSYNC_HCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_RESERVED3_MASK) | (((uint32_t)val) << HSYNC_HCFG_RESERVED3_SHIFT))

#define  HSYNC_HCFG_PULSE_QUAL_MASK 0x30000
#define  HSYNC_HCFG_PULSE_QUAL_SHIFT 16
#define  HSYNC_HCFG_PULSE_QUAL_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_PULSE_QUAL_MASK) >> HSYNC_HCFG_PULSE_QUAL_SHIFT)
#define  HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_PULSE_QUAL_MASK) | (((uint32_t)val) << HSYNC_HCFG_PULSE_QUAL_SHIFT))

#define  HSYNC_HCFG_RESERVED4_MASK 0xc000
#define  HSYNC_HCFG_RESERVED4_SHIFT 14
#define  HSYNC_HCFG_RESERVED4_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_RESERVED4_MASK) >> HSYNC_HCFG_RESERVED4_SHIFT)
#define  HSYNC_HCFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_RESERVED4_MASK) | (((uint32_t)val) << HSYNC_HCFG_RESERVED4_SHIFT))

#define  HSYNC_HCFG_HSYNC_QUAL_MASK 0x3000
#define  HSYNC_HCFG_HSYNC_QUAL_SHIFT 12
#define  HSYNC_HCFG_HSYNC_QUAL_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_HSYNC_QUAL_MASK) >> HSYNC_HCFG_HSYNC_QUAL_SHIFT)
#define  HSYNC_HCFG_HSYNC_QUAL_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_HSYNC_QUAL_MASK) | (((uint32_t)val) << HSYNC_HCFG_HSYNC_QUAL_SHIFT))

#define  HSYNC_HCFG_OUT_POL_MASK 0xf00
#define  HSYNC_HCFG_OUT_POL_SHIFT 8
#define  HSYNC_HCFG_OUT_POL_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_OUT_POL_MASK) >> HSYNC_HCFG_OUT_POL_SHIFT)
#define  HSYNC_HCFG_OUT_POL_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_OUT_POL_MASK) | (((uint32_t)val) << HSYNC_HCFG_OUT_POL_SHIFT))

#define  HSYNC_HCFG_IN_POL_MASK 0xf0
#define  HSYNC_HCFG_IN_POL_SHIFT 4
#define  HSYNC_HCFG_IN_POL_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_IN_POL_MASK) >> HSYNC_HCFG_IN_POL_SHIFT)
#define  HSYNC_HCFG_IN_POL_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_IN_POL_MASK) | (((uint32_t)val) << HSYNC_HCFG_IN_POL_SHIFT))

#define  HSYNC_HCFG_ENABLE_MASK 0xf
#define  HSYNC_HCFG_ENABLE_SHIFT 0
#define  HSYNC_HCFG_ENABLE_MASK_SHIFT(reg) (((reg) & HSYNC_HCFG_ENABLE_MASK) >> HSYNC_HCFG_ENABLE_SHIFT)
#define  HSYNC_HCFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HCFG_ENABLE_MASK) | (((uint32_t)val) << HSYNC_HCFG_ENABLE_SHIFT))

//====================================================================
//Register: HSync Combination register (combine)
//====================================================================

#define  HSYNC_COMBINE_RESERVED1_MASK 0xf0000000
#define  HSYNC_COMBINE_RESERVED1_SHIFT 28
#define  HSYNC_COMBINE_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_RESERVED1_MASK) >> HSYNC_COMBINE_RESERVED1_SHIFT)
#define  HSYNC_COMBINE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_COMBINE_RESERVED1_SHIFT))

#define  HSYNC_COMBINE_HSYNC3_MASK 0xf000000
#define  HSYNC_COMBINE_HSYNC3_SHIFT 24
#define  HSYNC_COMBINE_HSYNC3_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_HSYNC3_MASK) >> HSYNC_COMBINE_HSYNC3_SHIFT)
#define  HSYNC_COMBINE_HSYNC3_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_HSYNC3_MASK) | (((uint32_t)val) << HSYNC_COMBINE_HSYNC3_SHIFT))

#define  HSYNC_COMBINE_RESERVED2_MASK 0xf00000
#define  HSYNC_COMBINE_RESERVED2_SHIFT 20
#define  HSYNC_COMBINE_RESERVED2_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_RESERVED2_MASK) >> HSYNC_COMBINE_RESERVED2_SHIFT)
#define  HSYNC_COMBINE_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_RESERVED2_MASK) | (((uint32_t)val) << HSYNC_COMBINE_RESERVED2_SHIFT))

#define  HSYNC_COMBINE_HSYNC2_MASK 0xf0000
#define  HSYNC_COMBINE_HSYNC2_SHIFT 16
#define  HSYNC_COMBINE_HSYNC2_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_HSYNC2_MASK) >> HSYNC_COMBINE_HSYNC2_SHIFT)
#define  HSYNC_COMBINE_HSYNC2_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_HSYNC2_MASK) | (((uint32_t)val) << HSYNC_COMBINE_HSYNC2_SHIFT))

#define  HSYNC_COMBINE_RESERVED3_MASK 0xf000
#define  HSYNC_COMBINE_RESERVED3_SHIFT 12
#define  HSYNC_COMBINE_RESERVED3_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_RESERVED3_MASK) >> HSYNC_COMBINE_RESERVED3_SHIFT)
#define  HSYNC_COMBINE_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_RESERVED3_MASK) | (((uint32_t)val) << HSYNC_COMBINE_RESERVED3_SHIFT))

#define  HSYNC_COMBINE_HSYNC1_MASK 0xf00
#define  HSYNC_COMBINE_HSYNC1_SHIFT 8
#define  HSYNC_COMBINE_HSYNC1_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_HSYNC1_MASK) >> HSYNC_COMBINE_HSYNC1_SHIFT)
#define  HSYNC_COMBINE_HSYNC1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_HSYNC1_MASK) | (((uint32_t)val) << HSYNC_COMBINE_HSYNC1_SHIFT))

#define  HSYNC_COMBINE_RESERVED4_MASK 0xf0
#define  HSYNC_COMBINE_RESERVED4_SHIFT 4
#define  HSYNC_COMBINE_RESERVED4_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_RESERVED4_MASK) >> HSYNC_COMBINE_RESERVED4_SHIFT)
#define  HSYNC_COMBINE_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_RESERVED4_MASK) | (((uint32_t)val) << HSYNC_COMBINE_RESERVED4_SHIFT))

#define  HSYNC_COMBINE_HSYNC0_MASK 0xf
#define  HSYNC_COMBINE_HSYNC0_SHIFT 0
#define  HSYNC_COMBINE_HSYNC0_MASK_SHIFT(reg) (((reg) & HSYNC_COMBINE_HSYNC0_MASK) >> HSYNC_COMBINE_HSYNC0_SHIFT)
#define  HSYNC_COMBINE_HSYNC0_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COMBINE_HSYNC0_MASK) | (((uint32_t)val) << HSYNC_COMBINE_HSYNC0_SHIFT))

//====================================================================
//Register: HSync Filter Configuration register (hfilt)
//====================================================================

#define  HSYNC_HFILT_RESERVED1_MASK 0xff000000
#define  HSYNC_HFILT_RESERVED1_SHIFT 24
#define  HSYNC_HFILT_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_HFILT_RESERVED1_MASK) >> HSYNC_HFILT_RESERVED1_SHIFT)
#define  HSYNC_HFILT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HFILT_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_HFILT_RESERVED1_SHIFT))

#define  HSYNC_HFILT_FALL_HI_MASK 0xf00000
#define  HSYNC_HFILT_FALL_HI_SHIFT 20
#define  HSYNC_HFILT_FALL_HI_MASK_SHIFT(reg) (((reg) & HSYNC_HFILT_FALL_HI_MASK) >> HSYNC_HFILT_FALL_HI_SHIFT)
#define  HSYNC_HFILT_FALL_HI_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HFILT_FALL_HI_MASK) | (((uint32_t)val) << HSYNC_HFILT_FALL_HI_SHIFT))

#define  HSYNC_HFILT_FALL_LO_MASK 0xf0000
#define  HSYNC_HFILT_FALL_LO_SHIFT 16
#define  HSYNC_HFILT_FALL_LO_MASK_SHIFT(reg) (((reg) & HSYNC_HFILT_FALL_LO_MASK) >> HSYNC_HFILT_FALL_LO_SHIFT)
#define  HSYNC_HFILT_FALL_LO_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HFILT_FALL_LO_MASK) | (((uint32_t)val) << HSYNC_HFILT_FALL_LO_SHIFT))

#define  HSYNC_HFILT_RISE_LO_MASK 0xf000
#define  HSYNC_HFILT_RISE_LO_SHIFT 12
#define  HSYNC_HFILT_RISE_LO_MASK_SHIFT(reg) (((reg) & HSYNC_HFILT_RISE_LO_MASK) >> HSYNC_HFILT_RISE_LO_SHIFT)
#define  HSYNC_HFILT_RISE_LO_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HFILT_RISE_LO_MASK) | (((uint32_t)val) << HSYNC_HFILT_RISE_LO_SHIFT))

#define  HSYNC_HFILT_RISE_HI_MASK 0xf00
#define  HSYNC_HFILT_RISE_HI_SHIFT 8
#define  HSYNC_HFILT_RISE_HI_MASK_SHIFT(reg) (((reg) & HSYNC_HFILT_RISE_HI_MASK) >> HSYNC_HFILT_RISE_HI_SHIFT)
#define  HSYNC_HFILT_RISE_HI_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HFILT_RISE_HI_MASK) | (((uint32_t)val) << HSYNC_HFILT_RISE_HI_SHIFT))

#define  HSYNC_HFILT_THRESHOLD_MASK 0xff
#define  HSYNC_HFILT_THRESHOLD_SHIFT 0
#define  HSYNC_HFILT_THRESHOLD_MASK_SHIFT(reg) (((reg) & HSYNC_HFILT_THRESHOLD_MASK) >> HSYNC_HFILT_THRESHOLD_SHIFT)
#define  HSYNC_HFILT_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_HFILT_THRESHOLD_MASK) | (((uint32_t)val) << HSYNC_HFILT_THRESHOLD_SHIFT))

//====================================================================
//Register: HSync Timers Configuration 1 register (tcfg1)
//====================================================================

#define  HSYNC_TCFG1_RESERVED1_MASK 0x80000000
#define  HSYNC_TCFG1_RESERVED1_SHIFT 31
#define  HSYNC_TCFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED1_MASK) >> HSYNC_TCFG1_RESERVED1_SHIFT)
#define  HSYNC_TCFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED1_SHIFT))

#define  HSYNC_TCFG1_EVENT08_MASK 0x70000000
#define  HSYNC_TCFG1_EVENT08_SHIFT 28
#define  HSYNC_TCFG1_EVENT08_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT08_MASK) >> HSYNC_TCFG1_EVENT08_SHIFT)
#define  HSYNC_TCFG1_EVENT08_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT08_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT08_SHIFT))

#define  HSYNC_TCFG1_RESERVED2_MASK 0x8000000
#define  HSYNC_TCFG1_RESERVED2_SHIFT 27
#define  HSYNC_TCFG1_RESERVED2_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED2_MASK) >> HSYNC_TCFG1_RESERVED2_SHIFT)
#define  HSYNC_TCFG1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED2_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED2_SHIFT))

#define  HSYNC_TCFG1_EVENT07_MASK 0x7000000
#define  HSYNC_TCFG1_EVENT07_SHIFT 24
#define  HSYNC_TCFG1_EVENT07_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT07_MASK) >> HSYNC_TCFG1_EVENT07_SHIFT)
#define  HSYNC_TCFG1_EVENT07_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT07_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT07_SHIFT))

#define  HSYNC_TCFG1_RESERVED3_MASK 0x800000
#define  HSYNC_TCFG1_RESERVED3_SHIFT 23
#define  HSYNC_TCFG1_RESERVED3_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED3_MASK) >> HSYNC_TCFG1_RESERVED3_SHIFT)
#define  HSYNC_TCFG1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED3_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED3_SHIFT))

#define  HSYNC_TCFG1_EVENT06_MASK 0x700000
#define  HSYNC_TCFG1_EVENT06_SHIFT 20
#define  HSYNC_TCFG1_EVENT06_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT06_MASK) >> HSYNC_TCFG1_EVENT06_SHIFT)
#define  HSYNC_TCFG1_EVENT06_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT06_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT06_SHIFT))

#define  HSYNC_TCFG1_RESERVED4_MASK 0x80000
#define  HSYNC_TCFG1_RESERVED4_SHIFT 19
#define  HSYNC_TCFG1_RESERVED4_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED4_MASK) >> HSYNC_TCFG1_RESERVED4_SHIFT)
#define  HSYNC_TCFG1_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED4_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED4_SHIFT))

#define  HSYNC_TCFG1_EVENT05_MASK 0x70000
#define  HSYNC_TCFG1_EVENT05_SHIFT 16
#define  HSYNC_TCFG1_EVENT05_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT05_MASK) >> HSYNC_TCFG1_EVENT05_SHIFT)
#define  HSYNC_TCFG1_EVENT05_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT05_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT05_SHIFT))

#define  HSYNC_TCFG1_RESERVED5_MASK 0x8000
#define  HSYNC_TCFG1_RESERVED5_SHIFT 15
#define  HSYNC_TCFG1_RESERVED5_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED5_MASK) >> HSYNC_TCFG1_RESERVED5_SHIFT)
#define  HSYNC_TCFG1_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED5_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED5_SHIFT))

#define  HSYNC_TCFG1_EVENT04_MASK 0x7000
#define  HSYNC_TCFG1_EVENT04_SHIFT 12
#define  HSYNC_TCFG1_EVENT04_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT04_MASK) >> HSYNC_TCFG1_EVENT04_SHIFT)
#define  HSYNC_TCFG1_EVENT04_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT04_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT04_SHIFT))

#define  HSYNC_TCFG1_RESERVED6_MASK 0x800
#define  HSYNC_TCFG1_RESERVED6_SHIFT 11
#define  HSYNC_TCFG1_RESERVED6_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED6_MASK) >> HSYNC_TCFG1_RESERVED6_SHIFT)
#define  HSYNC_TCFG1_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED6_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED6_SHIFT))

#define  HSYNC_TCFG1_EVENT03_MASK 0x700
#define  HSYNC_TCFG1_EVENT03_SHIFT 8
#define  HSYNC_TCFG1_EVENT03_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT03_MASK) >> HSYNC_TCFG1_EVENT03_SHIFT)
#define  HSYNC_TCFG1_EVENT03_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT03_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT03_SHIFT))

#define  HSYNC_TCFG1_RESERVED7_MASK 0x80
#define  HSYNC_TCFG1_RESERVED7_SHIFT 7
#define  HSYNC_TCFG1_RESERVED7_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED7_MASK) >> HSYNC_TCFG1_RESERVED7_SHIFT)
#define  HSYNC_TCFG1_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED7_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED7_SHIFT))

#define  HSYNC_TCFG1_EVENT02_MASK 0x70
#define  HSYNC_TCFG1_EVENT02_SHIFT 4
#define  HSYNC_TCFG1_EVENT02_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT02_MASK) >> HSYNC_TCFG1_EVENT02_SHIFT)
#define  HSYNC_TCFG1_EVENT02_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT02_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT02_SHIFT))

#define  HSYNC_TCFG1_RESERVED8_MASK 0x8
#define  HSYNC_TCFG1_RESERVED8_SHIFT 3
#define  HSYNC_TCFG1_RESERVED8_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_RESERVED8_MASK) >> HSYNC_TCFG1_RESERVED8_SHIFT)
#define  HSYNC_TCFG1_RESERVED8_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_RESERVED8_MASK) | (((uint32_t)val) << HSYNC_TCFG1_RESERVED8_SHIFT))

#define  HSYNC_TCFG1_EVENT01_MASK 0x7
#define  HSYNC_TCFG1_EVENT01_SHIFT 0
#define  HSYNC_TCFG1_EVENT01_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG1_EVENT01_MASK) >> HSYNC_TCFG1_EVENT01_SHIFT)
#define  HSYNC_TCFG1_EVENT01_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG1_EVENT01_MASK) | (((uint32_t)val) << HSYNC_TCFG1_EVENT01_SHIFT))

//====================================================================
//Register: HSync Timers Configuration 2 register (tcfg2)
//====================================================================

#define  HSYNC_TCFG2_RESERVED1_MASK 0x80000000
#define  HSYNC_TCFG2_RESERVED1_SHIFT 31
#define  HSYNC_TCFG2_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED1_MASK) >> HSYNC_TCFG2_RESERVED1_SHIFT)
#define  HSYNC_TCFG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED1_SHIFT))

#define  HSYNC_TCFG2_EVENT16_MASK 0x70000000
#define  HSYNC_TCFG2_EVENT16_SHIFT 28
#define  HSYNC_TCFG2_EVENT16_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT16_MASK) >> HSYNC_TCFG2_EVENT16_SHIFT)
#define  HSYNC_TCFG2_EVENT16_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT16_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT16_SHIFT))

#define  HSYNC_TCFG2_RESERVED2_MASK 0x8000000
#define  HSYNC_TCFG2_RESERVED2_SHIFT 27
#define  HSYNC_TCFG2_RESERVED2_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED2_MASK) >> HSYNC_TCFG2_RESERVED2_SHIFT)
#define  HSYNC_TCFG2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED2_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED2_SHIFT))

#define  HSYNC_TCFG2_EVENT15_MASK 0x7000000
#define  HSYNC_TCFG2_EVENT15_SHIFT 24
#define  HSYNC_TCFG2_EVENT15_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT15_MASK) >> HSYNC_TCFG2_EVENT15_SHIFT)
#define  HSYNC_TCFG2_EVENT15_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT15_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT15_SHIFT))

#define  HSYNC_TCFG2_RESERVED3_MASK 0x800000
#define  HSYNC_TCFG2_RESERVED3_SHIFT 23
#define  HSYNC_TCFG2_RESERVED3_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED3_MASK) >> HSYNC_TCFG2_RESERVED3_SHIFT)
#define  HSYNC_TCFG2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED3_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED3_SHIFT))

#define  HSYNC_TCFG2_EVENT14_MASK 0x700000
#define  HSYNC_TCFG2_EVENT14_SHIFT 20
#define  HSYNC_TCFG2_EVENT14_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT14_MASK) >> HSYNC_TCFG2_EVENT14_SHIFT)
#define  HSYNC_TCFG2_EVENT14_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT14_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT14_SHIFT))

#define  HSYNC_TCFG2_RESERVED4_MASK 0x80000
#define  HSYNC_TCFG2_RESERVED4_SHIFT 19
#define  HSYNC_TCFG2_RESERVED4_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED4_MASK) >> HSYNC_TCFG2_RESERVED4_SHIFT)
#define  HSYNC_TCFG2_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED4_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED4_SHIFT))

#define  HSYNC_TCFG2_EVENT13_MASK 0x70000
#define  HSYNC_TCFG2_EVENT13_SHIFT 16
#define  HSYNC_TCFG2_EVENT13_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT13_MASK) >> HSYNC_TCFG2_EVENT13_SHIFT)
#define  HSYNC_TCFG2_EVENT13_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT13_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT13_SHIFT))

#define  HSYNC_TCFG2_RESERVED5_MASK 0x8000
#define  HSYNC_TCFG2_RESERVED5_SHIFT 15
#define  HSYNC_TCFG2_RESERVED5_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED5_MASK) >> HSYNC_TCFG2_RESERVED5_SHIFT)
#define  HSYNC_TCFG2_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED5_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED5_SHIFT))

#define  HSYNC_TCFG2_EVENT12_MASK 0x7000
#define  HSYNC_TCFG2_EVENT12_SHIFT 12
#define  HSYNC_TCFG2_EVENT12_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT12_MASK) >> HSYNC_TCFG2_EVENT12_SHIFT)
#define  HSYNC_TCFG2_EVENT12_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT12_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT12_SHIFT))

#define  HSYNC_TCFG2_RESERVED6_MASK 0x800
#define  HSYNC_TCFG2_RESERVED6_SHIFT 11
#define  HSYNC_TCFG2_RESERVED6_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED6_MASK) >> HSYNC_TCFG2_RESERVED6_SHIFT)
#define  HSYNC_TCFG2_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED6_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED6_SHIFT))

#define  HSYNC_TCFG2_EVENT11_MASK 0x700
#define  HSYNC_TCFG2_EVENT11_SHIFT 8
#define  HSYNC_TCFG2_EVENT11_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT11_MASK) >> HSYNC_TCFG2_EVENT11_SHIFT)
#define  HSYNC_TCFG2_EVENT11_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT11_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT11_SHIFT))

#define  HSYNC_TCFG2_RESERVED7_MASK 0x80
#define  HSYNC_TCFG2_RESERVED7_SHIFT 7
#define  HSYNC_TCFG2_RESERVED7_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED7_MASK) >> HSYNC_TCFG2_RESERVED7_SHIFT)
#define  HSYNC_TCFG2_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED7_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED7_SHIFT))

#define  HSYNC_TCFG2_EVENT10_MASK 0x70
#define  HSYNC_TCFG2_EVENT10_SHIFT 4
#define  HSYNC_TCFG2_EVENT10_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT10_MASK) >> HSYNC_TCFG2_EVENT10_SHIFT)
#define  HSYNC_TCFG2_EVENT10_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT10_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT10_SHIFT))

#define  HSYNC_TCFG2_RESERVED8_MASK 0x8
#define  HSYNC_TCFG2_RESERVED8_SHIFT 3
#define  HSYNC_TCFG2_RESERVED8_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_RESERVED8_MASK) >> HSYNC_TCFG2_RESERVED8_SHIFT)
#define  HSYNC_TCFG2_RESERVED8_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_RESERVED8_MASK) | (((uint32_t)val) << HSYNC_TCFG2_RESERVED8_SHIFT))

#define  HSYNC_TCFG2_EVENT09_MASK 0x7
#define  HSYNC_TCFG2_EVENT09_SHIFT 0
#define  HSYNC_TCFG2_EVENT09_MASK_SHIFT(reg) (((reg) & HSYNC_TCFG2_EVENT09_MASK) >> HSYNC_TCFG2_EVENT09_SHIFT)
#define  HSYNC_TCFG2_EVENT09_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TCFG2_EVENT09_MASK) | (((uint32_t)val) << HSYNC_TCFG2_EVENT09_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 1 of 16) (timer1)
/** \brief  (Instance 1 of 16)*/
//====================================================================

#define  HSYNC_TIMER1_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER1_RESERVED1_SHIFT 24
#define  HSYNC_TIMER1_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER1_RESERVED1_MASK) >> HSYNC_TIMER1_RESERVED1_SHIFT)
#define  HSYNC_TIMER1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER1_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER1_RESERVED1_SHIFT))

#define  HSYNC_TIMER1_TIMER_MASK 0xffffff
#define  HSYNC_TIMER1_TIMER_SHIFT 0
#define  HSYNC_TIMER1_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER1_TIMER_MASK) >> HSYNC_TIMER1_TIMER_SHIFT)
#define  HSYNC_TIMER1_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER1_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER1_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 2 of 16) (timer2)
/** \brief  (Instance 2 of 16)*/
//====================================================================

#define  HSYNC_TIMER2_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER2_RESERVED1_SHIFT 24
#define  HSYNC_TIMER2_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER2_RESERVED1_MASK) >> HSYNC_TIMER2_RESERVED1_SHIFT)
#define  HSYNC_TIMER2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER2_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER2_RESERVED1_SHIFT))

#define  HSYNC_TIMER2_TIMER_MASK 0xffffff
#define  HSYNC_TIMER2_TIMER_SHIFT 0
#define  HSYNC_TIMER2_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER2_TIMER_MASK) >> HSYNC_TIMER2_TIMER_SHIFT)
#define  HSYNC_TIMER2_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER2_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER2_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 3 of 16) (timer3)
/** \brief  (Instance 3 of 16)*/
//====================================================================

#define  HSYNC_TIMER3_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER3_RESERVED1_SHIFT 24
#define  HSYNC_TIMER3_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER3_RESERVED1_MASK) >> HSYNC_TIMER3_RESERVED1_SHIFT)
#define  HSYNC_TIMER3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER3_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER3_RESERVED1_SHIFT))

#define  HSYNC_TIMER3_TIMER_MASK 0xffffff
#define  HSYNC_TIMER3_TIMER_SHIFT 0
#define  HSYNC_TIMER3_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER3_TIMER_MASK) >> HSYNC_TIMER3_TIMER_SHIFT)
#define  HSYNC_TIMER3_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER3_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER3_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 4 of 16) (timer4)
/** \brief  (Instance 4 of 16)*/
//====================================================================

#define  HSYNC_TIMER4_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER4_RESERVED1_SHIFT 24
#define  HSYNC_TIMER4_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER4_RESERVED1_MASK) >> HSYNC_TIMER4_RESERVED1_SHIFT)
#define  HSYNC_TIMER4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER4_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER4_RESERVED1_SHIFT))

#define  HSYNC_TIMER4_TIMER_MASK 0xffffff
#define  HSYNC_TIMER4_TIMER_SHIFT 0
#define  HSYNC_TIMER4_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER4_TIMER_MASK) >> HSYNC_TIMER4_TIMER_SHIFT)
#define  HSYNC_TIMER4_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER4_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER4_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 5 of 16) (timer5)
/** \brief  (Instance 5 of 16)*/
//====================================================================

#define  HSYNC_TIMER5_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER5_RESERVED1_SHIFT 24
#define  HSYNC_TIMER5_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER5_RESERVED1_MASK) >> HSYNC_TIMER5_RESERVED1_SHIFT)
#define  HSYNC_TIMER5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER5_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER5_RESERVED1_SHIFT))

#define  HSYNC_TIMER5_TIMER_MASK 0xffffff
#define  HSYNC_TIMER5_TIMER_SHIFT 0
#define  HSYNC_TIMER5_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER5_TIMER_MASK) >> HSYNC_TIMER5_TIMER_SHIFT)
#define  HSYNC_TIMER5_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER5_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER5_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 6 of 16) (timer6)
/** \brief  (Instance 6 of 16)*/
//====================================================================

#define  HSYNC_TIMER6_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER6_RESERVED1_SHIFT 24
#define  HSYNC_TIMER6_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER6_RESERVED1_MASK) >> HSYNC_TIMER6_RESERVED1_SHIFT)
#define  HSYNC_TIMER6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER6_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER6_RESERVED1_SHIFT))

#define  HSYNC_TIMER6_TIMER_MASK 0xffffff
#define  HSYNC_TIMER6_TIMER_SHIFT 0
#define  HSYNC_TIMER6_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER6_TIMER_MASK) >> HSYNC_TIMER6_TIMER_SHIFT)
#define  HSYNC_TIMER6_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER6_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER6_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 7 of 16) (timer7)
/** \brief  (Instance 7 of 16)*/
//====================================================================

#define  HSYNC_TIMER7_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER7_RESERVED1_SHIFT 24
#define  HSYNC_TIMER7_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER7_RESERVED1_MASK) >> HSYNC_TIMER7_RESERVED1_SHIFT)
#define  HSYNC_TIMER7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER7_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER7_RESERVED1_SHIFT))

#define  HSYNC_TIMER7_TIMER_MASK 0xffffff
#define  HSYNC_TIMER7_TIMER_SHIFT 0
#define  HSYNC_TIMER7_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER7_TIMER_MASK) >> HSYNC_TIMER7_TIMER_SHIFT)
#define  HSYNC_TIMER7_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER7_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER7_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 8 of 16) (timer8)
/** \brief  (Instance 8 of 16)*/
//====================================================================

#define  HSYNC_TIMER8_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER8_RESERVED1_SHIFT 24
#define  HSYNC_TIMER8_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER8_RESERVED1_MASK) >> HSYNC_TIMER8_RESERVED1_SHIFT)
#define  HSYNC_TIMER8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER8_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER8_RESERVED1_SHIFT))

#define  HSYNC_TIMER8_TIMER_MASK 0xffffff
#define  HSYNC_TIMER8_TIMER_SHIFT 0
#define  HSYNC_TIMER8_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER8_TIMER_MASK) >> HSYNC_TIMER8_TIMER_SHIFT)
#define  HSYNC_TIMER8_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER8_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER8_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 9 of 16) (timer9)
/** \brief  (Instance 9 of 16)*/
//====================================================================

#define  HSYNC_TIMER9_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER9_RESERVED1_SHIFT 24
#define  HSYNC_TIMER9_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER9_RESERVED1_MASK) >> HSYNC_TIMER9_RESERVED1_SHIFT)
#define  HSYNC_TIMER9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER9_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER9_RESERVED1_SHIFT))

#define  HSYNC_TIMER9_TIMER_MASK 0xffffff
#define  HSYNC_TIMER9_TIMER_SHIFT 0
#define  HSYNC_TIMER9_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER9_TIMER_MASK) >> HSYNC_TIMER9_TIMER_SHIFT)
#define  HSYNC_TIMER9_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER9_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER9_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 10 of 16) (timer10)
/** \brief  (Instance 10 of 16)*/
//====================================================================

#define  HSYNC_TIMER10_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER10_RESERVED1_SHIFT 24
#define  HSYNC_TIMER10_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER10_RESERVED1_MASK) >> HSYNC_TIMER10_RESERVED1_SHIFT)
#define  HSYNC_TIMER10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER10_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER10_RESERVED1_SHIFT))

#define  HSYNC_TIMER10_TIMER_MASK 0xffffff
#define  HSYNC_TIMER10_TIMER_SHIFT 0
#define  HSYNC_TIMER10_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER10_TIMER_MASK) >> HSYNC_TIMER10_TIMER_SHIFT)
#define  HSYNC_TIMER10_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER10_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER10_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 11 of 16) (timer11)
/** \brief  (Instance 11 of 16)*/
//====================================================================

#define  HSYNC_TIMER11_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER11_RESERVED1_SHIFT 24
#define  HSYNC_TIMER11_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER11_RESERVED1_MASK) >> HSYNC_TIMER11_RESERVED1_SHIFT)
#define  HSYNC_TIMER11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER11_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER11_RESERVED1_SHIFT))

#define  HSYNC_TIMER11_TIMER_MASK 0xffffff
#define  HSYNC_TIMER11_TIMER_SHIFT 0
#define  HSYNC_TIMER11_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER11_TIMER_MASK) >> HSYNC_TIMER11_TIMER_SHIFT)
#define  HSYNC_TIMER11_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER11_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER11_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 12 of 16) (timer12)
/** \brief  (Instance 12 of 16)*/
//====================================================================

#define  HSYNC_TIMER12_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER12_RESERVED1_SHIFT 24
#define  HSYNC_TIMER12_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER12_RESERVED1_MASK) >> HSYNC_TIMER12_RESERVED1_SHIFT)
#define  HSYNC_TIMER12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER12_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER12_RESERVED1_SHIFT))

#define  HSYNC_TIMER12_TIMER_MASK 0xffffff
#define  HSYNC_TIMER12_TIMER_SHIFT 0
#define  HSYNC_TIMER12_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER12_TIMER_MASK) >> HSYNC_TIMER12_TIMER_SHIFT)
#define  HSYNC_TIMER12_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER12_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER12_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 13 of 16) (timer13)
/** \brief  (Instance 13 of 16)*/
//====================================================================

#define  HSYNC_TIMER13_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER13_RESERVED1_SHIFT 24
#define  HSYNC_TIMER13_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER13_RESERVED1_MASK) >> HSYNC_TIMER13_RESERVED1_SHIFT)
#define  HSYNC_TIMER13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER13_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER13_RESERVED1_SHIFT))

#define  HSYNC_TIMER13_TIMER_MASK 0xffffff
#define  HSYNC_TIMER13_TIMER_SHIFT 0
#define  HSYNC_TIMER13_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER13_TIMER_MASK) >> HSYNC_TIMER13_TIMER_SHIFT)
#define  HSYNC_TIMER13_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER13_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER13_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 14 of 16) (timer14)
/** \brief  (Instance 14 of 16)*/
//====================================================================

#define  HSYNC_TIMER14_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER14_RESERVED1_SHIFT 24
#define  HSYNC_TIMER14_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER14_RESERVED1_MASK) >> HSYNC_TIMER14_RESERVED1_SHIFT)
#define  HSYNC_TIMER14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER14_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER14_RESERVED1_SHIFT))

#define  HSYNC_TIMER14_TIMER_MASK 0xffffff
#define  HSYNC_TIMER14_TIMER_SHIFT 0
#define  HSYNC_TIMER14_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER14_TIMER_MASK) >> HSYNC_TIMER14_TIMER_SHIFT)
#define  HSYNC_TIMER14_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER14_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER14_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 15 of 16) (timer15)
/** \brief  (Instance 15 of 16)*/
//====================================================================

#define  HSYNC_TIMER15_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER15_RESERVED1_SHIFT 24
#define  HSYNC_TIMER15_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER15_RESERVED1_MASK) >> HSYNC_TIMER15_RESERVED1_SHIFT)
#define  HSYNC_TIMER15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER15_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER15_RESERVED1_SHIFT))

#define  HSYNC_TIMER15_TIMER_MASK 0xffffff
#define  HSYNC_TIMER15_TIMER_SHIFT 0
#define  HSYNC_TIMER15_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER15_TIMER_MASK) >> HSYNC_TIMER15_TIMER_SHIFT)
#define  HSYNC_TIMER15_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER15_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER15_TIMER_SHIFT))

//====================================================================
//Register: HSync Timer register (Instance 16 of 16) (timer16)
/** \brief  (Instance 16 of 16)*/
//====================================================================

#define  HSYNC_TIMER16_RESERVED1_MASK 0xff000000
#define  HSYNC_TIMER16_RESERVED1_SHIFT 24
#define  HSYNC_TIMER16_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER16_RESERVED1_MASK) >> HSYNC_TIMER16_RESERVED1_SHIFT)
#define  HSYNC_TIMER16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER16_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_TIMER16_RESERVED1_SHIFT))

#define  HSYNC_TIMER16_TIMER_MASK 0xffffff
#define  HSYNC_TIMER16_TIMER_SHIFT 0
#define  HSYNC_TIMER16_TIMER_MASK_SHIFT(reg) (((reg) & HSYNC_TIMER16_TIMER_MASK) >> HSYNC_TIMER16_TIMER_SHIFT)
#define  HSYNC_TIMER16_TIMER_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_TIMER16_TIMER_MASK) | (((uint32_t)val) << HSYNC_TIMER16_TIMER_SHIFT))

//====================================================================
//Register: HSync Counter Enable register (count_en)
//====================================================================

#define  HSYNC_COUNT_EN_RESERVED1_MASK 0xfffffff0
#define  HSYNC_COUNT_EN_RESERVED1_SHIFT 4
#define  HSYNC_COUNT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_COUNT_EN_RESERVED1_MASK) >> HSYNC_COUNT_EN_RESERVED1_SHIFT)
#define  HSYNC_COUNT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNT_EN_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_COUNT_EN_RESERVED1_SHIFT))

#define  HSYNC_COUNT_EN_ENABLE_MASK 0xf
#define  HSYNC_COUNT_EN_ENABLE_SHIFT 0
#define  HSYNC_COUNT_EN_ENABLE_MASK_SHIFT(reg) (((reg) & HSYNC_COUNT_EN_ENABLE_MASK) >> HSYNC_COUNT_EN_ENABLE_SHIFT)
#define  HSYNC_COUNT_EN_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNT_EN_ENABLE_MASK) | (((uint32_t)val) << HSYNC_COUNT_EN_ENABLE_SHIFT))

//====================================================================
//Register: HSync Counter Clear register (count_cl)
//====================================================================

#define  HSYNC_COUNT_CL_RESERVED1_MASK 0xfffffff0
#define  HSYNC_COUNT_CL_RESERVED1_SHIFT 4
#define  HSYNC_COUNT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_COUNT_CL_RESERVED1_MASK) >> HSYNC_COUNT_CL_RESERVED1_SHIFT)
#define  HSYNC_COUNT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNT_CL_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_COUNT_CL_RESERVED1_SHIFT))

#define  HSYNC_COUNT_CL_CLEAR_MASK 0xf
#define  HSYNC_COUNT_CL_CLEAR_SHIFT 0
#define  HSYNC_COUNT_CL_CLEAR_MASK_SHIFT(reg) (((reg) & HSYNC_COUNT_CL_CLEAR_MASK) >> HSYNC_COUNT_CL_CLEAR_SHIFT)
#define  HSYNC_COUNT_CL_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNT_CL_CLEAR_MASK) | (((uint32_t)val) << HSYNC_COUNT_CL_CLEAR_SHIFT))

//====================================================================
//Register: HSync Counter register (Instance 1 of 4) (counter1)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  HSYNC_COUNTER1_RESERVED1_MASK 0xffff0000
#define  HSYNC_COUNTER1_RESERVED1_SHIFT 16
#define  HSYNC_COUNTER1_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER1_RESERVED1_MASK) >> HSYNC_COUNTER1_RESERVED1_SHIFT)
#define  HSYNC_COUNTER1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER1_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_COUNTER1_RESERVED1_SHIFT))

#define  HSYNC_COUNTER1_COUNT_MASK 0xffff
#define  HSYNC_COUNTER1_COUNT_SHIFT 0
#define  HSYNC_COUNTER1_COUNT_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER1_COUNT_MASK) >> HSYNC_COUNTER1_COUNT_SHIFT)
#define  HSYNC_COUNTER1_COUNT_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER1_COUNT_MASK) | (((uint32_t)val) << HSYNC_COUNTER1_COUNT_SHIFT))

//====================================================================
//Register: HSync Counter register (Instance 2 of 4) (counter2)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  HSYNC_COUNTER2_RESERVED1_MASK 0xffff0000
#define  HSYNC_COUNTER2_RESERVED1_SHIFT 16
#define  HSYNC_COUNTER2_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER2_RESERVED1_MASK) >> HSYNC_COUNTER2_RESERVED1_SHIFT)
#define  HSYNC_COUNTER2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER2_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_COUNTER2_RESERVED1_SHIFT))

#define  HSYNC_COUNTER2_COUNT_MASK 0xffff
#define  HSYNC_COUNTER2_COUNT_SHIFT 0
#define  HSYNC_COUNTER2_COUNT_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER2_COUNT_MASK) >> HSYNC_COUNTER2_COUNT_SHIFT)
#define  HSYNC_COUNTER2_COUNT_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER2_COUNT_MASK) | (((uint32_t)val) << HSYNC_COUNTER2_COUNT_SHIFT))

//====================================================================
//Register: HSync Counter register (Instance 3 of 4) (counter3)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  HSYNC_COUNTER3_RESERVED1_MASK 0xffff0000
#define  HSYNC_COUNTER3_RESERVED1_SHIFT 16
#define  HSYNC_COUNTER3_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER3_RESERVED1_MASK) >> HSYNC_COUNTER3_RESERVED1_SHIFT)
#define  HSYNC_COUNTER3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER3_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_COUNTER3_RESERVED1_SHIFT))

#define  HSYNC_COUNTER3_COUNT_MASK 0xffff
#define  HSYNC_COUNTER3_COUNT_SHIFT 0
#define  HSYNC_COUNTER3_COUNT_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER3_COUNT_MASK) >> HSYNC_COUNTER3_COUNT_SHIFT)
#define  HSYNC_COUNTER3_COUNT_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER3_COUNT_MASK) | (((uint32_t)val) << HSYNC_COUNTER3_COUNT_SHIFT))

//====================================================================
//Register: HSync Counter register (Instance 4 of 4) (counter4)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  HSYNC_COUNTER4_RESERVED1_MASK 0xffff0000
#define  HSYNC_COUNTER4_RESERVED1_SHIFT 16
#define  HSYNC_COUNTER4_RESERVED1_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER4_RESERVED1_MASK) >> HSYNC_COUNTER4_RESERVED1_SHIFT)
#define  HSYNC_COUNTER4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER4_RESERVED1_MASK) | (((uint32_t)val) << HSYNC_COUNTER4_RESERVED1_SHIFT))

#define  HSYNC_COUNTER4_COUNT_MASK 0xffff
#define  HSYNC_COUNTER4_COUNT_SHIFT 0
#define  HSYNC_COUNTER4_COUNT_MASK_SHIFT(reg) (((reg) & HSYNC_COUNTER4_COUNT_MASK) >> HSYNC_COUNTER4_COUNT_SHIFT)
#define  HSYNC_COUNTER4_COUNT_REPLACE_VAL(reg,val) (((reg) & ~HSYNC_COUNTER4_COUNT_MASK) | (((uint32_t)val) << HSYNC_COUNTER4_COUNT_SHIFT))

//====================================================================
//
//Register File: Mirror Control (Instance 1 of 2) (mirror0)
//
//====================================================================

//====================================================================
//Register: Mirror Configuration 1 register (mcfg1)
//====================================================================

#define  MIRROR0_MCFG1_MODE_MASK 0xc0000000
#define  MIRROR0_MCFG1_MODE_SHIFT 30
#define  MIRROR0_MCFG1_MODE_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG1_MODE_MASK) >> MIRROR0_MCFG1_MODE_SHIFT)
#define  MIRROR0_MCFG1_MODE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG1_MODE_MASK) | (((uint32_t)val) << MIRROR0_MCFG1_MODE_SHIFT))

#define  MIRROR0_MCFG1_HSYNC_SEL_MASK 0x30000000
#define  MIRROR0_MCFG1_HSYNC_SEL_SHIFT 28
#define  MIRROR0_MCFG1_HSYNC_SEL_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG1_HSYNC_SEL_MASK) >> MIRROR0_MCFG1_HSYNC_SEL_SHIFT)
#define  MIRROR0_MCFG1_HSYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG1_HSYNC_SEL_MASK) | (((uint32_t)val) << MIRROR0_MCFG1_HSYNC_SEL_SHIFT))

#define  MIRROR0_MCFG1_FACETS_MASK 0xf000000
#define  MIRROR0_MCFG1_FACETS_SHIFT 24
#define  MIRROR0_MCFG1_FACETS_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG1_FACETS_MASK) >> MIRROR0_MCFG1_FACETS_SHIFT)
#define  MIRROR0_MCFG1_FACETS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG1_FACETS_MASK) | (((uint32_t)val) << MIRROR0_MCFG1_FACETS_SHIFT))

#define  MIRROR0_MCFG1_CONTROL_MASK 0xffffff
#define  MIRROR0_MCFG1_CONTROL_SHIFT 0
#define  MIRROR0_MCFG1_CONTROL_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG1_CONTROL_MASK) >> MIRROR0_MCFG1_CONTROL_SHIFT)
#define  MIRROR0_MCFG1_CONTROL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG1_CONTROL_MASK) | (((uint32_t)val) << MIRROR0_MCFG1_CONTROL_SHIFT))

//====================================================================
//Register: Mirror Configuration 2 register (mcfg2)
//====================================================================

#define  MIRROR0_MCFG2_GALVOR_DIS_MASK 0xc0000000
#define  MIRROR0_MCFG2_GALVOR_DIS_SHIFT 30
#define  MIRROR0_MCFG2_GALVOR_DIS_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_GALVOR_DIS_MASK) >> MIRROR0_MCFG2_GALVOR_DIS_SHIFT)
#define  MIRROR0_MCFG2_GALVOR_DIS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_GALVOR_DIS_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_GALVOR_DIS_SHIFT))

#define  MIRROR0_MCFG2_GALVON_DIS_MASK 0x20000000
#define  MIRROR0_MCFG2_GALVON_DIS_SHIFT 29
#define  MIRROR0_MCFG2_GALVON_DIS_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_GALVON_DIS_MASK) >> MIRROR0_MCFG2_GALVON_DIS_SHIFT)
#define  MIRROR0_MCFG2_GALVON_DIS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_GALVON_DIS_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_GALVON_DIS_SHIFT))

#define  MIRROR0_MCFG2_GALVOP_DIS_MASK 0x10000000
#define  MIRROR0_MCFG2_GALVOP_DIS_SHIFT 28
#define  MIRROR0_MCFG2_GALVOP_DIS_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_GALVOP_DIS_MASK) >> MIRROR0_MCFG2_GALVOP_DIS_SHIFT)
#define  MIRROR0_MCFG2_GALVOP_DIS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_GALVOP_DIS_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_GALVOP_DIS_SHIFT))

#define  MIRROR0_MCFG2_GALVOR_IDLE_MASK 0xc000000
#define  MIRROR0_MCFG2_GALVOR_IDLE_SHIFT 26
#define  MIRROR0_MCFG2_GALVOR_IDLE_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_GALVOR_IDLE_MASK) >> MIRROR0_MCFG2_GALVOR_IDLE_SHIFT)
#define  MIRROR0_MCFG2_GALVOR_IDLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_GALVOR_IDLE_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_GALVOR_IDLE_SHIFT))

#define  MIRROR0_MCFG2_GALVON_IDLE_MASK 0x2000000
#define  MIRROR0_MCFG2_GALVON_IDLE_SHIFT 25
#define  MIRROR0_MCFG2_GALVON_IDLE_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_GALVON_IDLE_MASK) >> MIRROR0_MCFG2_GALVON_IDLE_SHIFT)
#define  MIRROR0_MCFG2_GALVON_IDLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_GALVON_IDLE_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_GALVON_IDLE_SHIFT))

#define  MIRROR0_MCFG2_GALVOP_IDLE_MASK 0x1000000
#define  MIRROR0_MCFG2_GALVOP_IDLE_SHIFT 24
#define  MIRROR0_MCFG2_GALVOP_IDLE_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_GALVOP_IDLE_MASK) >> MIRROR0_MCFG2_GALVOP_IDLE_SHIFT)
#define  MIRROR0_MCFG2_GALVOP_IDLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_GALVOP_IDLE_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_GALVOP_IDLE_SHIFT))

#define  MIRROR0_MCFG2_RESERVED1_MASK 0x800000
#define  MIRROR0_MCFG2_RESERVED1_SHIFT 23
#define  MIRROR0_MCFG2_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_RESERVED1_MASK) >> MIRROR0_MCFG2_RESERVED1_SHIFT)
#define  MIRROR0_MCFG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_RESERVED1_SHIFT))

#define  MIRROR0_MCFG2_DEC_POL_MASK 0x400000
#define  MIRROR0_MCFG2_DEC_POL_SHIFT 22
#define  MIRROR0_MCFG2_DEC_POL_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_DEC_POL_MASK) >> MIRROR0_MCFG2_DEC_POL_SHIFT)
#define  MIRROR0_MCFG2_DEC_POL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_DEC_POL_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_DEC_POL_SHIFT))

#define  MIRROR0_MCFG2_ACC_POL_MASK 0x200000
#define  MIRROR0_MCFG2_ACC_POL_SHIFT 21
#define  MIRROR0_MCFG2_ACC_POL_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_ACC_POL_MASK) >> MIRROR0_MCFG2_ACC_POL_SHIFT)
#define  MIRROR0_MCFG2_ACC_POL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_ACC_POL_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_ACC_POL_SHIFT))

#define  MIRROR0_MCFG2_LOCK_POL_MASK 0x100000
#define  MIRROR0_MCFG2_LOCK_POL_SHIFT 20
#define  MIRROR0_MCFG2_LOCK_POL_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_LOCK_POL_MASK) >> MIRROR0_MCFG2_LOCK_POL_SHIFT)
#define  MIRROR0_MCFG2_LOCK_POL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_LOCK_POL_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_LOCK_POL_SHIFT))

#define  MIRROR0_MCFG2_RESERVED2_MASK 0xe0000
#define  MIRROR0_MCFG2_RESERVED2_SHIFT 17
#define  MIRROR0_MCFG2_RESERVED2_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_RESERVED2_MASK) >> MIRROR0_MCFG2_RESERVED2_SHIFT)
#define  MIRROR0_MCFG2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_RESERVED2_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_RESERVED2_SHIFT))

#define  MIRROR0_MCFG2_RECOV_MASK 0x10000
#define  MIRROR0_MCFG2_RECOV_SHIFT 16
#define  MIRROR0_MCFG2_RECOV_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_RECOV_MASK) >> MIRROR0_MCFG2_RECOV_SHIFT)
#define  MIRROR0_MCFG2_RECOV_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_RECOV_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_RECOV_SHIFT))

#define  MIRROR0_MCFG2_RESERVED3_MASK 0xf000
#define  MIRROR0_MCFG2_RESERVED3_SHIFT 12
#define  MIRROR0_MCFG2_RESERVED3_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_RESERVED3_MASK) >> MIRROR0_MCFG2_RESERVED3_SHIFT)
#define  MIRROR0_MCFG2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_RESERVED3_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_RESERVED3_SHIFT))

#define  MIRROR0_MCFG2_MAX_ADJ_MASK 0xf00
#define  MIRROR0_MCFG2_MAX_ADJ_SHIFT 8
#define  MIRROR0_MCFG2_MAX_ADJ_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_MAX_ADJ_MASK) >> MIRROR0_MCFG2_MAX_ADJ_SHIFT)
#define  MIRROR0_MCFG2_MAX_ADJ_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_MAX_ADJ_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_MAX_ADJ_SHIFT))

#define  MIRROR0_MCFG2_RESERVED4_MASK 0xf0
#define  MIRROR0_MCFG2_RESERVED4_SHIFT 4
#define  MIRROR0_MCFG2_RESERVED4_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_RESERVED4_MASK) >> MIRROR0_MCFG2_RESERVED4_SHIFT)
#define  MIRROR0_MCFG2_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_RESERVED4_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_RESERVED4_SHIFT))

#define  MIRROR0_MCFG2_MIN_ADJ_MASK 0xf
#define  MIRROR0_MCFG2_MIN_ADJ_SHIFT 0
#define  MIRROR0_MCFG2_MIN_ADJ_MASK_SHIFT(reg) (((reg) & MIRROR0_MCFG2_MIN_ADJ_MASK) >> MIRROR0_MCFG2_MIN_ADJ_SHIFT)
#define  MIRROR0_MCFG2_MIN_ADJ_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_MCFG2_MIN_ADJ_MASK) | (((uint32_t)val) << MIRROR0_MCFG2_MIN_ADJ_SHIFT))

//====================================================================
//Register: Mirror Status register (status)
//====================================================================

#define  MIRROR0_STATUS_RESERVED1_MASK 0xfc000000
#define  MIRROR0_STATUS_RESERVED1_SHIFT 26
#define  MIRROR0_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_STATUS_RESERVED1_MASK) >> MIRROR0_STATUS_RESERVED1_SHIFT)
#define  MIRROR0_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_STATUS_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_STATUS_RESERVED1_SHIFT))

#define  MIRROR0_STATUS_LOCK_DET_MASK 0x2000000
#define  MIRROR0_STATUS_LOCK_DET_SHIFT 25
#define  MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(reg) (((reg) & MIRROR0_STATUS_LOCK_DET_MASK) >> MIRROR0_STATUS_LOCK_DET_SHIFT)
#define  MIRROR0_STATUS_LOCK_DET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_STATUS_LOCK_DET_MASK) | (((uint32_t)val) << MIRROR0_STATUS_LOCK_DET_SHIFT))

#define  MIRROR0_STATUS_LOCKED_MASK 0x1000000
#define  MIRROR0_STATUS_LOCKED_SHIFT 24
#define  MIRROR0_STATUS_LOCKED_MASK_SHIFT(reg) (((reg) & MIRROR0_STATUS_LOCKED_MASK) >> MIRROR0_STATUS_LOCKED_SHIFT)
#define  MIRROR0_STATUS_LOCKED_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_STATUS_LOCKED_MASK) | (((uint32_t)val) << MIRROR0_STATUS_LOCKED_SHIFT))

#define  MIRROR0_STATUS_PERIOD_MASK 0xffffff
#define  MIRROR0_STATUS_PERIOD_SHIFT 0
#define  MIRROR0_STATUS_PERIOD_MASK_SHIFT(reg) (((reg) & MIRROR0_STATUS_PERIOD_MASK) >> MIRROR0_STATUS_PERIOD_SHIFT)
#define  MIRROR0_STATUS_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_STATUS_PERIOD_MASK) | (((uint32_t)val) << MIRROR0_STATUS_PERIOD_SHIFT))

//====================================================================
//Register: Mirror Lock Max register (lockmax)
//====================================================================

#define  MIRROR0_LOCKMAX_RESERVED1_MASK 0xff000000
#define  MIRROR0_LOCKMAX_RESERVED1_SHIFT 24
#define  MIRROR0_LOCKMAX_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_LOCKMAX_RESERVED1_MASK) >> MIRROR0_LOCKMAX_RESERVED1_SHIFT)
#define  MIRROR0_LOCKMAX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_LOCKMAX_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_LOCKMAX_RESERVED1_SHIFT))

#define  MIRROR0_LOCKMAX_LOCKMAX_MASK 0xffffff
#define  MIRROR0_LOCKMAX_LOCKMAX_SHIFT 0
#define  MIRROR0_LOCKMAX_LOCKMAX_MASK_SHIFT(reg) (((reg) & MIRROR0_LOCKMAX_LOCKMAX_MASK) >> MIRROR0_LOCKMAX_LOCKMAX_SHIFT)
#define  MIRROR0_LOCKMAX_LOCKMAX_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_LOCKMAX_LOCKMAX_MASK) | (((uint32_t)val) << MIRROR0_LOCKMAX_LOCKMAX_SHIFT))

//====================================================================
//Register: Mirror Lock Min register (lockmin)
//====================================================================

#define  MIRROR0_LOCKMIN_RESERVED1_MASK 0xff000000
#define  MIRROR0_LOCKMIN_RESERVED1_SHIFT 24
#define  MIRROR0_LOCKMIN_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_LOCKMIN_RESERVED1_MASK) >> MIRROR0_LOCKMIN_RESERVED1_SHIFT)
#define  MIRROR0_LOCKMIN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_LOCKMIN_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_LOCKMIN_RESERVED1_SHIFT))

#define  MIRROR0_LOCKMIN_LOCKMIN_MASK 0xffffff
#define  MIRROR0_LOCKMIN_LOCKMIN_SHIFT 0
#define  MIRROR0_LOCKMIN_LOCKMIN_MASK_SHIFT(reg) (((reg) & MIRROR0_LOCKMIN_LOCKMIN_MASK) >> MIRROR0_LOCKMIN_LOCKMIN_SHIFT)
#define  MIRROR0_LOCKMIN_LOCKMIN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_LOCKMIN_LOCKMIN_MASK) | (((uint32_t)val) << MIRROR0_LOCKMIN_LOCKMIN_SHIFT))

//====================================================================
//Register: HSync Interrupt Configuration register (hsync_int)
//====================================================================

#define  MIRROR0_HSYNC_INT_RESERVED1_MASK 0xffffff00
#define  MIRROR0_HSYNC_INT_RESERVED1_SHIFT 8
#define  MIRROR0_HSYNC_INT_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_INT_RESERVED1_MASK) >> MIRROR0_HSYNC_INT_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_INT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_INT_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_INT_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_INT_HSYNC_SET_MASK 0xff
#define  MIRROR0_HSYNC_INT_HSYNC_SET_SHIFT 0
#define  MIRROR0_HSYNC_INT_HSYNC_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_INT_HSYNC_SET_MASK) >> MIRROR0_HSYNC_INT_HSYNC_SET_SHIFT)
#define  MIRROR0_HSYNC_INT_HSYNC_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_INT_HSYNC_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_INT_HSYNC_SET_SHIFT))

//====================================================================
//Register: Interrupt Enable register (int_en)
//====================================================================

#define  MIRROR0_INT_EN_RESERVED1_MASK 0xffffffe0
#define  MIRROR0_INT_EN_RESERVED1_SHIFT 5
#define  MIRROR0_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_EN_RESERVED1_MASK) >> MIRROR0_INT_EN_RESERVED1_SHIFT)
#define  MIRROR0_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_INT_EN_RESERVED1_SHIFT))

#define  MIRROR0_INT_EN_HSYNC_INT_EN_MASK 0x10
#define  MIRROR0_INT_EN_HSYNC_INT_EN_SHIFT 4
#define  MIRROR0_INT_EN_HSYNC_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_EN_HSYNC_INT_EN_MASK) >> MIRROR0_INT_EN_HSYNC_INT_EN_SHIFT)
#define  MIRROR0_INT_EN_HSYNC_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_EN_HSYNC_INT_EN_MASK) | (((uint32_t)val) << MIRROR0_INT_EN_HSYNC_INT_EN_SHIFT))

#define  MIRROR0_INT_EN_MEASR_INT_EN_MASK 0x8
#define  MIRROR0_INT_EN_MEASR_INT_EN_SHIFT 3
#define  MIRROR0_INT_EN_MEASR_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_EN_MEASR_INT_EN_MASK) >> MIRROR0_INT_EN_MEASR_INT_EN_SHIFT)
#define  MIRROR0_INT_EN_MEASR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_EN_MEASR_INT_EN_MASK) | (((uint32_t)val) << MIRROR0_INT_EN_MEASR_INT_EN_SHIFT))

#define  MIRROR0_INT_EN_ERROR_INT_EN_MASK 0x4
#define  MIRROR0_INT_EN_ERROR_INT_EN_SHIFT 2
#define  MIRROR0_INT_EN_ERROR_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_EN_ERROR_INT_EN_MASK) >> MIRROR0_INT_EN_ERROR_INT_EN_SHIFT)
#define  MIRROR0_INT_EN_ERROR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_EN_ERROR_INT_EN_MASK) | (((uint32_t)val) << MIRROR0_INT_EN_ERROR_INT_EN_SHIFT))

#define  MIRROR0_INT_EN_LATE_INT_EN_MASK 0x2
#define  MIRROR0_INT_EN_LATE_INT_EN_SHIFT 1
#define  MIRROR0_INT_EN_LATE_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_EN_LATE_INT_EN_MASK) >> MIRROR0_INT_EN_LATE_INT_EN_SHIFT)
#define  MIRROR0_INT_EN_LATE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_EN_LATE_INT_EN_MASK) | (((uint32_t)val) << MIRROR0_INT_EN_LATE_INT_EN_SHIFT))

#define  MIRROR0_INT_EN_EARLY_INT_EN_MASK 0x1
#define  MIRROR0_INT_EN_EARLY_INT_EN_SHIFT 0
#define  MIRROR0_INT_EN_EARLY_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_EN_EARLY_INT_EN_MASK) >> MIRROR0_INT_EN_EARLY_INT_EN_SHIFT)
#define  MIRROR0_INT_EN_EARLY_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_EN_EARLY_INT_EN_MASK) | (((uint32_t)val) << MIRROR0_INT_EN_EARLY_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending register (int_st)
//====================================================================

#define  MIRROR0_INT_ST_RESERVED1_MASK 0xffffffe0
#define  MIRROR0_INT_ST_RESERVED1_SHIFT 5
#define  MIRROR0_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_ST_RESERVED1_MASK) >> MIRROR0_INT_ST_RESERVED1_SHIFT)
#define  MIRROR0_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_INT_ST_RESERVED1_SHIFT))

#define  MIRROR0_INT_ST_HSYNC_INT_ST_MASK 0x10
#define  MIRROR0_INT_ST_HSYNC_INT_ST_SHIFT 4
#define  MIRROR0_INT_ST_HSYNC_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_ST_HSYNC_INT_ST_MASK) >> MIRROR0_INT_ST_HSYNC_INT_ST_SHIFT)
#define  MIRROR0_INT_ST_HSYNC_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_ST_HSYNC_INT_ST_MASK) | (((uint32_t)val) << MIRROR0_INT_ST_HSYNC_INT_ST_SHIFT))

#define  MIRROR0_INT_ST_MEASR_INT_ST_MASK 0x8
#define  MIRROR0_INT_ST_MEASR_INT_ST_SHIFT 3
#define  MIRROR0_INT_ST_MEASR_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_ST_MEASR_INT_ST_MASK) >> MIRROR0_INT_ST_MEASR_INT_ST_SHIFT)
#define  MIRROR0_INT_ST_MEASR_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_ST_MEASR_INT_ST_MASK) | (((uint32_t)val) << MIRROR0_INT_ST_MEASR_INT_ST_SHIFT))

#define  MIRROR0_INT_ST_ERROR_INT_ST_MASK 0x4
#define  MIRROR0_INT_ST_ERROR_INT_ST_SHIFT 2
#define  MIRROR0_INT_ST_ERROR_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_ST_ERROR_INT_ST_MASK) >> MIRROR0_INT_ST_ERROR_INT_ST_SHIFT)
#define  MIRROR0_INT_ST_ERROR_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_ST_ERROR_INT_ST_MASK) | (((uint32_t)val) << MIRROR0_INT_ST_ERROR_INT_ST_SHIFT))

#define  MIRROR0_INT_ST_LATE_INT_ST_MASK 0x2
#define  MIRROR0_INT_ST_LATE_INT_ST_SHIFT 1
#define  MIRROR0_INT_ST_LATE_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_ST_LATE_INT_ST_MASK) >> MIRROR0_INT_ST_LATE_INT_ST_SHIFT)
#define  MIRROR0_INT_ST_LATE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_ST_LATE_INT_ST_MASK) | (((uint32_t)val) << MIRROR0_INT_ST_LATE_INT_ST_SHIFT))

#define  MIRROR0_INT_ST_EARLY_INT_ST_MASK 0x1
#define  MIRROR0_INT_ST_EARLY_INT_ST_SHIFT 0
#define  MIRROR0_INT_ST_EARLY_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_ST_EARLY_INT_ST_MASK) >> MIRROR0_INT_ST_EARLY_INT_ST_SHIFT)
#define  MIRROR0_INT_ST_EARLY_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_ST_EARLY_INT_ST_MASK) | (((uint32_t)val) << MIRROR0_INT_ST_EARLY_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge register (int_cl)
//====================================================================

#define  MIRROR0_INT_CL_RESERVED1_MASK 0xffffffe0
#define  MIRROR0_INT_CL_RESERVED1_SHIFT 5
#define  MIRROR0_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_CL_RESERVED1_MASK) >> MIRROR0_INT_CL_RESERVED1_SHIFT)
#define  MIRROR0_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_INT_CL_RESERVED1_SHIFT))

#define  MIRROR0_INT_CL_HSYNC_INT_CL_MASK 0x10
#define  MIRROR0_INT_CL_HSYNC_INT_CL_SHIFT 4
#define  MIRROR0_INT_CL_HSYNC_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_CL_HSYNC_INT_CL_MASK) >> MIRROR0_INT_CL_HSYNC_INT_CL_SHIFT)
#define  MIRROR0_INT_CL_HSYNC_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_CL_HSYNC_INT_CL_MASK) | (((uint32_t)val) << MIRROR0_INT_CL_HSYNC_INT_CL_SHIFT))

#define  MIRROR0_INT_CL_MEASR_INT_CL_MASK 0x8
#define  MIRROR0_INT_CL_MEASR_INT_CL_SHIFT 3
#define  MIRROR0_INT_CL_MEASR_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_CL_MEASR_INT_CL_MASK) >> MIRROR0_INT_CL_MEASR_INT_CL_SHIFT)
#define  MIRROR0_INT_CL_MEASR_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_CL_MEASR_INT_CL_MASK) | (((uint32_t)val) << MIRROR0_INT_CL_MEASR_INT_CL_SHIFT))

#define  MIRROR0_INT_CL_ERROR_INT_CL_MASK 0x4
#define  MIRROR0_INT_CL_ERROR_INT_CL_SHIFT 2
#define  MIRROR0_INT_CL_ERROR_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_CL_ERROR_INT_CL_MASK) >> MIRROR0_INT_CL_ERROR_INT_CL_SHIFT)
#define  MIRROR0_INT_CL_ERROR_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_CL_ERROR_INT_CL_MASK) | (((uint32_t)val) << MIRROR0_INT_CL_ERROR_INT_CL_SHIFT))

#define  MIRROR0_INT_CL_LATE_INT_CL_MASK 0x2
#define  MIRROR0_INT_CL_LATE_INT_CL_SHIFT 1
#define  MIRROR0_INT_CL_LATE_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_CL_LATE_INT_CL_MASK) >> MIRROR0_INT_CL_LATE_INT_CL_SHIFT)
#define  MIRROR0_INT_CL_LATE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_CL_LATE_INT_CL_MASK) | (((uint32_t)val) << MIRROR0_INT_CL_LATE_INT_CL_SHIFT))

#define  MIRROR0_INT_CL_EARLY_INT_CL_MASK 0x1
#define  MIRROR0_INT_CL_EARLY_INT_CL_SHIFT 0
#define  MIRROR0_INT_CL_EARLY_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_CL_EARLY_INT_CL_MASK) >> MIRROR0_INT_CL_EARLY_INT_CL_SHIFT)
#define  MIRROR0_INT_CL_EARLY_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_CL_EARLY_INT_CL_MASK) | (((uint32_t)val) << MIRROR0_INT_CL_EARLY_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force register (int_fo)
//====================================================================

#define  MIRROR0_INT_FO_RESERVED1_MASK 0xffffffe0
#define  MIRROR0_INT_FO_RESERVED1_SHIFT 5
#define  MIRROR0_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_FO_RESERVED1_MASK) >> MIRROR0_INT_FO_RESERVED1_SHIFT)
#define  MIRROR0_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_INT_FO_RESERVED1_SHIFT))

#define  MIRROR0_INT_FO_HSYNC_INT_FO_MASK 0x10
#define  MIRROR0_INT_FO_HSYNC_INT_FO_SHIFT 4
#define  MIRROR0_INT_FO_HSYNC_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_FO_HSYNC_INT_FO_MASK) >> MIRROR0_INT_FO_HSYNC_INT_FO_SHIFT)
#define  MIRROR0_INT_FO_HSYNC_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_FO_HSYNC_INT_FO_MASK) | (((uint32_t)val) << MIRROR0_INT_FO_HSYNC_INT_FO_SHIFT))

#define  MIRROR0_INT_FO_MEASR_INT_FO_MASK 0x8
#define  MIRROR0_INT_FO_MEASR_INT_FO_SHIFT 3
#define  MIRROR0_INT_FO_MEASR_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_FO_MEASR_INT_FO_MASK) >> MIRROR0_INT_FO_MEASR_INT_FO_SHIFT)
#define  MIRROR0_INT_FO_MEASR_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_FO_MEASR_INT_FO_MASK) | (((uint32_t)val) << MIRROR0_INT_FO_MEASR_INT_FO_SHIFT))

#define  MIRROR0_INT_FO_ERROR_INT_FO_MASK 0x4
#define  MIRROR0_INT_FO_ERROR_INT_FO_SHIFT 2
#define  MIRROR0_INT_FO_ERROR_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_FO_ERROR_INT_FO_MASK) >> MIRROR0_INT_FO_ERROR_INT_FO_SHIFT)
#define  MIRROR0_INT_FO_ERROR_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_FO_ERROR_INT_FO_MASK) | (((uint32_t)val) << MIRROR0_INT_FO_ERROR_INT_FO_SHIFT))

#define  MIRROR0_INT_FO_LATE_INT_FO_MASK 0x2
#define  MIRROR0_INT_FO_LATE_INT_FO_SHIFT 1
#define  MIRROR0_INT_FO_LATE_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_FO_LATE_INT_FO_MASK) >> MIRROR0_INT_FO_LATE_INT_FO_SHIFT)
#define  MIRROR0_INT_FO_LATE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_FO_LATE_INT_FO_MASK) | (((uint32_t)val) << MIRROR0_INT_FO_LATE_INT_FO_SHIFT))

#define  MIRROR0_INT_FO_EARLY_INT_FO_MASK 0x1
#define  MIRROR0_INT_FO_EARLY_INT_FO_SHIFT 0
#define  MIRROR0_INT_FO_EARLY_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR0_INT_FO_EARLY_INT_FO_MASK) >> MIRROR0_INT_FO_EARLY_INT_FO_SHIFT)
#define  MIRROR0_INT_FO_EARLY_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_INT_FO_EARLY_INT_FO_MASK) | (((uint32_t)val) << MIRROR0_INT_FO_EARLY_INT_FO_SHIFT))

//====================================================================
//Register: SyncGalvo register (Instance 1 of 2) (sync_galvo0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_SYNC_GALVO0_ENABLE_MASK 0x80000000
#define  MIRROR0_SYNC_GALVO0_ENABLE_SHIFT 31
#define  MIRROR0_SYNC_GALVO0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_GALVO0_ENABLE_MASK) >> MIRROR0_SYNC_GALVO0_ENABLE_SHIFT)
#define  MIRROR0_SYNC_GALVO0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_GALVO0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_SYNC_GALVO0_ENABLE_SHIFT))

#define  MIRROR0_SYNC_GALVO0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_SYNC_GALVO0_RESERVED1_SHIFT 24
#define  MIRROR0_SYNC_GALVO0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_GALVO0_RESERVED1_MASK) >> MIRROR0_SYNC_GALVO0_RESERVED1_SHIFT)
#define  MIRROR0_SYNC_GALVO0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_GALVO0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_SYNC_GALVO0_RESERVED1_SHIFT))

#define  MIRROR0_SYNC_GALVO0_SET_MASK 0xffffff
#define  MIRROR0_SYNC_GALVO0_SET_SHIFT 0
#define  MIRROR0_SYNC_GALVO0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_GALVO0_SET_MASK) >> MIRROR0_SYNC_GALVO0_SET_SHIFT)
#define  MIRROR0_SYNC_GALVO0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_GALVO0_SET_MASK) | (((uint32_t)val) << MIRROR0_SYNC_GALVO0_SET_SHIFT))

//====================================================================
//Register: SyncGalvo register (Instance 2 of 2) (sync_galvo1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_SYNC_GALVO1_ENABLE_MASK 0x80000000
#define  MIRROR0_SYNC_GALVO1_ENABLE_SHIFT 31
#define  MIRROR0_SYNC_GALVO1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_GALVO1_ENABLE_MASK) >> MIRROR0_SYNC_GALVO1_ENABLE_SHIFT)
#define  MIRROR0_SYNC_GALVO1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_GALVO1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_SYNC_GALVO1_ENABLE_SHIFT))

#define  MIRROR0_SYNC_GALVO1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_SYNC_GALVO1_RESERVED1_SHIFT 24
#define  MIRROR0_SYNC_GALVO1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_GALVO1_RESERVED1_MASK) >> MIRROR0_SYNC_GALVO1_RESERVED1_SHIFT)
#define  MIRROR0_SYNC_GALVO1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_GALVO1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_SYNC_GALVO1_RESERVED1_SHIFT))

#define  MIRROR0_SYNC_GALVO1_SET_MASK 0xffffff
#define  MIRROR0_SYNC_GALVO1_SET_SHIFT 0
#define  MIRROR0_SYNC_GALVO1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_GALVO1_SET_MASK) >> MIRROR0_SYNC_GALVO1_SET_SHIFT)
#define  MIRROR0_SYNC_GALVO1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_GALVO1_SET_MASK) | (((uint32_t)val) << MIRROR0_SYNC_GALVO1_SET_SHIFT))

//====================================================================
//Register: SyncWFG register (Instance 1 of 2) (sync_wfg0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_SYNC_WFG0_ENABLE_MASK 0x80000000
#define  MIRROR0_SYNC_WFG0_ENABLE_SHIFT 31
#define  MIRROR0_SYNC_WFG0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_WFG0_ENABLE_MASK) >> MIRROR0_SYNC_WFG0_ENABLE_SHIFT)
#define  MIRROR0_SYNC_WFG0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_WFG0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_SYNC_WFG0_ENABLE_SHIFT))

#define  MIRROR0_SYNC_WFG0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_SYNC_WFG0_RESERVED1_SHIFT 24
#define  MIRROR0_SYNC_WFG0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_WFG0_RESERVED1_MASK) >> MIRROR0_SYNC_WFG0_RESERVED1_SHIFT)
#define  MIRROR0_SYNC_WFG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_WFG0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_SYNC_WFG0_RESERVED1_SHIFT))

#define  MIRROR0_SYNC_WFG0_SET_MASK 0xffffff
#define  MIRROR0_SYNC_WFG0_SET_SHIFT 0
#define  MIRROR0_SYNC_WFG0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_WFG0_SET_MASK) >> MIRROR0_SYNC_WFG0_SET_SHIFT)
#define  MIRROR0_SYNC_WFG0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_WFG0_SET_MASK) | (((uint32_t)val) << MIRROR0_SYNC_WFG0_SET_SHIFT))

//====================================================================
//Register: SyncWFG register (Instance 2 of 2) (sync_wfg1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_SYNC_WFG1_ENABLE_MASK 0x80000000
#define  MIRROR0_SYNC_WFG1_ENABLE_SHIFT 31
#define  MIRROR0_SYNC_WFG1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_WFG1_ENABLE_MASK) >> MIRROR0_SYNC_WFG1_ENABLE_SHIFT)
#define  MIRROR0_SYNC_WFG1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_WFG1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_SYNC_WFG1_ENABLE_SHIFT))

#define  MIRROR0_SYNC_WFG1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_SYNC_WFG1_RESERVED1_SHIFT 24
#define  MIRROR0_SYNC_WFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_WFG1_RESERVED1_MASK) >> MIRROR0_SYNC_WFG1_RESERVED1_SHIFT)
#define  MIRROR0_SYNC_WFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_WFG1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_SYNC_WFG1_RESERVED1_SHIFT))

#define  MIRROR0_SYNC_WFG1_SET_MASK 0xffffff
#define  MIRROR0_SYNC_WFG1_SET_SHIFT 0
#define  MIRROR0_SYNC_WFG1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_SYNC_WFG1_SET_MASK) >> MIRROR0_SYNC_WFG1_SET_SHIFT)
#define  MIRROR0_SYNC_WFG1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_SYNC_WFG1_SET_MASK) | (((uint32_t)val) << MIRROR0_SYNC_WFG1_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Rising register (Instance 1 of 2) (galvo_rtol_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_GALVO_RTOL_RISE0_RESERVED1_MASK 0xff000000
#define  MIRROR0_GALVO_RTOL_RISE0_RESERVED1_SHIFT 24
#define  MIRROR0_GALVO_RTOL_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_RISE0_RESERVED1_MASK) >> MIRROR0_GALVO_RTOL_RISE0_RESERVED1_SHIFT)
#define  MIRROR0_GALVO_RTOL_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_RISE0_RESERVED1_SHIFT))

#define  MIRROR0_GALVO_RTOL_RISE0_SET_MASK 0xffffff
#define  MIRROR0_GALVO_RTOL_RISE0_SET_SHIFT 0
#define  MIRROR0_GALVO_RTOL_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_RISE0_SET_MASK) >> MIRROR0_GALVO_RTOL_RISE0_SET_SHIFT)
#define  MIRROR0_GALVO_RTOL_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_RISE0_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Rising register (Instance 2 of 2) (galvo_rtol_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_GALVO_RTOL_RISE1_RESERVED1_MASK 0xff000000
#define  MIRROR0_GALVO_RTOL_RISE1_RESERVED1_SHIFT 24
#define  MIRROR0_GALVO_RTOL_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_RISE1_RESERVED1_MASK) >> MIRROR0_GALVO_RTOL_RISE1_RESERVED1_SHIFT)
#define  MIRROR0_GALVO_RTOL_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_RISE1_RESERVED1_SHIFT))

#define  MIRROR0_GALVO_RTOL_RISE1_SET_MASK 0xffffff
#define  MIRROR0_GALVO_RTOL_RISE1_SET_SHIFT 0
#define  MIRROR0_GALVO_RTOL_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_RISE1_SET_MASK) >> MIRROR0_GALVO_RTOL_RISE1_SET_SHIFT)
#define  MIRROR0_GALVO_RTOL_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_RISE1_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Falling register (Instance 1 of 2) (galvo_rtol_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_GALVO_RTOL_FALL0_RESERVED1_MASK 0xff000000
#define  MIRROR0_GALVO_RTOL_FALL0_RESERVED1_SHIFT 24
#define  MIRROR0_GALVO_RTOL_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_FALL0_RESERVED1_MASK) >> MIRROR0_GALVO_RTOL_FALL0_RESERVED1_SHIFT)
#define  MIRROR0_GALVO_RTOL_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_FALL0_RESERVED1_SHIFT))

#define  MIRROR0_GALVO_RTOL_FALL0_SET_MASK 0xffffff
#define  MIRROR0_GALVO_RTOL_FALL0_SET_SHIFT 0
#define  MIRROR0_GALVO_RTOL_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_FALL0_SET_MASK) >> MIRROR0_GALVO_RTOL_FALL0_SET_SHIFT)
#define  MIRROR0_GALVO_RTOL_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_FALL0_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Falling register (Instance 2 of 2) (galvo_rtol_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_GALVO_RTOL_FALL1_RESERVED1_MASK 0xff000000
#define  MIRROR0_GALVO_RTOL_FALL1_RESERVED1_SHIFT 24
#define  MIRROR0_GALVO_RTOL_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_FALL1_RESERVED1_MASK) >> MIRROR0_GALVO_RTOL_FALL1_RESERVED1_SHIFT)
#define  MIRROR0_GALVO_RTOL_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_FALL1_RESERVED1_SHIFT))

#define  MIRROR0_GALVO_RTOL_FALL1_SET_MASK 0xffffff
#define  MIRROR0_GALVO_RTOL_FALL1_SET_SHIFT 0
#define  MIRROR0_GALVO_RTOL_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVO_RTOL_FALL1_SET_MASK) >> MIRROR0_GALVO_RTOL_FALL1_SET_SHIFT)
#define  MIRROR0_GALVO_RTOL_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVO_RTOL_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVO_RTOL_FALL1_SET_SHIFT))

//====================================================================
//Register: GalvoP Rising register (Instance 1 of 2) (galvop_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_GALVOP_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVOP_RISE0_ENABLE_SHIFT 31
#define  MIRROR0_GALVOP_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_RISE0_ENABLE_MASK) >> MIRROR0_GALVOP_RISE0_ENABLE_SHIFT)
#define  MIRROR0_GALVOP_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_RISE0_ENABLE_SHIFT))

#define  MIRROR0_GALVOP_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVOP_RISE0_RESERVED1_SHIFT 24
#define  MIRROR0_GALVOP_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_RISE0_RESERVED1_MASK) >> MIRROR0_GALVOP_RISE0_RESERVED1_SHIFT)
#define  MIRROR0_GALVOP_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_RISE0_RESERVED1_SHIFT))

#define  MIRROR0_GALVOP_RISE0_SET_MASK 0xffffff
#define  MIRROR0_GALVOP_RISE0_SET_SHIFT 0
#define  MIRROR0_GALVOP_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_RISE0_SET_MASK) >> MIRROR0_GALVOP_RISE0_SET_SHIFT)
#define  MIRROR0_GALVOP_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_RISE0_SET_SHIFT))

//====================================================================
//Register: GalvoP Rising register (Instance 2 of 2) (galvop_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_GALVOP_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVOP_RISE1_ENABLE_SHIFT 31
#define  MIRROR0_GALVOP_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_RISE1_ENABLE_MASK) >> MIRROR0_GALVOP_RISE1_ENABLE_SHIFT)
#define  MIRROR0_GALVOP_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_RISE1_ENABLE_SHIFT))

#define  MIRROR0_GALVOP_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVOP_RISE1_RESERVED1_SHIFT 24
#define  MIRROR0_GALVOP_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_RISE1_RESERVED1_MASK) >> MIRROR0_GALVOP_RISE1_RESERVED1_SHIFT)
#define  MIRROR0_GALVOP_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_RISE1_RESERVED1_SHIFT))

#define  MIRROR0_GALVOP_RISE1_SET_MASK 0xffffff
#define  MIRROR0_GALVOP_RISE1_SET_SHIFT 0
#define  MIRROR0_GALVOP_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_RISE1_SET_MASK) >> MIRROR0_GALVOP_RISE1_SET_SHIFT)
#define  MIRROR0_GALVOP_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_RISE1_SET_SHIFT))

//====================================================================
//Register: GalvoP Falling register (Instance 1 of 2) (galvop_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_GALVOP_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVOP_FALL0_ENABLE_SHIFT 31
#define  MIRROR0_GALVOP_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_FALL0_ENABLE_MASK) >> MIRROR0_GALVOP_FALL0_ENABLE_SHIFT)
#define  MIRROR0_GALVOP_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_FALL0_ENABLE_SHIFT))

#define  MIRROR0_GALVOP_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVOP_FALL0_RESERVED1_SHIFT 24
#define  MIRROR0_GALVOP_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_FALL0_RESERVED1_MASK) >> MIRROR0_GALVOP_FALL0_RESERVED1_SHIFT)
#define  MIRROR0_GALVOP_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_FALL0_RESERVED1_SHIFT))

#define  MIRROR0_GALVOP_FALL0_SET_MASK 0xffffff
#define  MIRROR0_GALVOP_FALL0_SET_SHIFT 0
#define  MIRROR0_GALVOP_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_FALL0_SET_MASK) >> MIRROR0_GALVOP_FALL0_SET_SHIFT)
#define  MIRROR0_GALVOP_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_FALL0_SET_SHIFT))

//====================================================================
//Register: GalvoP Falling register (Instance 2 of 2) (galvop_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_GALVOP_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVOP_FALL1_ENABLE_SHIFT 31
#define  MIRROR0_GALVOP_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_FALL1_ENABLE_MASK) >> MIRROR0_GALVOP_FALL1_ENABLE_SHIFT)
#define  MIRROR0_GALVOP_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_FALL1_ENABLE_SHIFT))

#define  MIRROR0_GALVOP_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVOP_FALL1_RESERVED1_SHIFT 24
#define  MIRROR0_GALVOP_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_FALL1_RESERVED1_MASK) >> MIRROR0_GALVOP_FALL1_RESERVED1_SHIFT)
#define  MIRROR0_GALVOP_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_FALL1_RESERVED1_SHIFT))

#define  MIRROR0_GALVOP_FALL1_SET_MASK 0xffffff
#define  MIRROR0_GALVOP_FALL1_SET_SHIFT 0
#define  MIRROR0_GALVOP_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVOP_FALL1_SET_MASK) >> MIRROR0_GALVOP_FALL1_SET_SHIFT)
#define  MIRROR0_GALVOP_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVOP_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVOP_FALL1_SET_SHIFT))

//====================================================================
//Register: GalvoN Rising register (Instance 1 of 2) (galvon_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_GALVON_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVON_RISE0_ENABLE_SHIFT 31
#define  MIRROR0_GALVON_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_RISE0_ENABLE_MASK) >> MIRROR0_GALVON_RISE0_ENABLE_SHIFT)
#define  MIRROR0_GALVON_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVON_RISE0_ENABLE_SHIFT))

#define  MIRROR0_GALVON_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVON_RISE0_RESERVED1_SHIFT 24
#define  MIRROR0_GALVON_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_RISE0_RESERVED1_MASK) >> MIRROR0_GALVON_RISE0_RESERVED1_SHIFT)
#define  MIRROR0_GALVON_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVON_RISE0_RESERVED1_SHIFT))

#define  MIRROR0_GALVON_RISE0_SET_MASK 0xffffff
#define  MIRROR0_GALVON_RISE0_SET_SHIFT 0
#define  MIRROR0_GALVON_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_RISE0_SET_MASK) >> MIRROR0_GALVON_RISE0_SET_SHIFT)
#define  MIRROR0_GALVON_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVON_RISE0_SET_SHIFT))

//====================================================================
//Register: GalvoN Rising register (Instance 2 of 2) (galvon_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_GALVON_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVON_RISE1_ENABLE_SHIFT 31
#define  MIRROR0_GALVON_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_RISE1_ENABLE_MASK) >> MIRROR0_GALVON_RISE1_ENABLE_SHIFT)
#define  MIRROR0_GALVON_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVON_RISE1_ENABLE_SHIFT))

#define  MIRROR0_GALVON_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVON_RISE1_RESERVED1_SHIFT 24
#define  MIRROR0_GALVON_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_RISE1_RESERVED1_MASK) >> MIRROR0_GALVON_RISE1_RESERVED1_SHIFT)
#define  MIRROR0_GALVON_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVON_RISE1_RESERVED1_SHIFT))

#define  MIRROR0_GALVON_RISE1_SET_MASK 0xffffff
#define  MIRROR0_GALVON_RISE1_SET_SHIFT 0
#define  MIRROR0_GALVON_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_RISE1_SET_MASK) >> MIRROR0_GALVON_RISE1_SET_SHIFT)
#define  MIRROR0_GALVON_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVON_RISE1_SET_SHIFT))

//====================================================================
//Register: GalvoN Falling register (Instance 1 of 2) (galvon_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_GALVON_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVON_FALL0_ENABLE_SHIFT 31
#define  MIRROR0_GALVON_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_FALL0_ENABLE_MASK) >> MIRROR0_GALVON_FALL0_ENABLE_SHIFT)
#define  MIRROR0_GALVON_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVON_FALL0_ENABLE_SHIFT))

#define  MIRROR0_GALVON_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVON_FALL0_RESERVED1_SHIFT 24
#define  MIRROR0_GALVON_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_FALL0_RESERVED1_MASK) >> MIRROR0_GALVON_FALL0_RESERVED1_SHIFT)
#define  MIRROR0_GALVON_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVON_FALL0_RESERVED1_SHIFT))

#define  MIRROR0_GALVON_FALL0_SET_MASK 0xffffff
#define  MIRROR0_GALVON_FALL0_SET_SHIFT 0
#define  MIRROR0_GALVON_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_FALL0_SET_MASK) >> MIRROR0_GALVON_FALL0_SET_SHIFT)
#define  MIRROR0_GALVON_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVON_FALL0_SET_SHIFT))

//====================================================================
//Register: GalvoN Falling register (Instance 2 of 2) (galvon_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_GALVON_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR0_GALVON_FALL1_ENABLE_SHIFT 31
#define  MIRROR0_GALVON_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_FALL1_ENABLE_MASK) >> MIRROR0_GALVON_FALL1_ENABLE_SHIFT)
#define  MIRROR0_GALVON_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_GALVON_FALL1_ENABLE_SHIFT))

#define  MIRROR0_GALVON_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_GALVON_FALL1_RESERVED1_SHIFT 24
#define  MIRROR0_GALVON_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_FALL1_RESERVED1_MASK) >> MIRROR0_GALVON_FALL1_RESERVED1_SHIFT)
#define  MIRROR0_GALVON_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_GALVON_FALL1_RESERVED1_SHIFT))

#define  MIRROR0_GALVON_FALL1_SET_MASK 0xffffff
#define  MIRROR0_GALVON_FALL1_SET_SHIFT 0
#define  MIRROR0_GALVON_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_GALVON_FALL1_SET_MASK) >> MIRROR0_GALVON_FALL1_SET_SHIFT)
#define  MIRROR0_GALVON_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_GALVON_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR0_GALVON_FALL1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Rising register (Instance 1 of 2) (hsync_en0_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN0_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN0_RISE0_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN0_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_RISE0_ENABLE_MASK) >> MIRROR0_HSYNC_EN0_RISE0_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN0_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_RISE0_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN0_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN0_RISE0_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN0_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_RISE0_RESERVED1_MASK) >> MIRROR0_HSYNC_EN0_RISE0_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN0_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_RISE0_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN0_RISE0_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN0_RISE0_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN0_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_RISE0_SET_MASK) >> MIRROR0_HSYNC_EN0_RISE0_SET_SHIFT)
#define  MIRROR0_HSYNC_EN0_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_RISE0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Rising register (Instance 2 of 2) (hsync_en0_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN0_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN0_RISE1_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN0_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_RISE1_ENABLE_MASK) >> MIRROR0_HSYNC_EN0_RISE1_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN0_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_RISE1_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN0_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN0_RISE1_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN0_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_RISE1_RESERVED1_MASK) >> MIRROR0_HSYNC_EN0_RISE1_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN0_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_RISE1_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN0_RISE1_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN0_RISE1_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN0_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_RISE1_SET_MASK) >> MIRROR0_HSYNC_EN0_RISE1_SET_SHIFT)
#define  MIRROR0_HSYNC_EN0_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_RISE1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Falling register (Instance 1 of 2) (hsync_en0_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN0_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN0_FALL0_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN0_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_FALL0_ENABLE_MASK) >> MIRROR0_HSYNC_EN0_FALL0_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN0_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_FALL0_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN0_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN0_FALL0_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN0_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_FALL0_RESERVED1_MASK) >> MIRROR0_HSYNC_EN0_FALL0_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN0_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_FALL0_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN0_FALL0_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN0_FALL0_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN0_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_FALL0_SET_MASK) >> MIRROR0_HSYNC_EN0_FALL0_SET_SHIFT)
#define  MIRROR0_HSYNC_EN0_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_FALL0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Falling register (Instance 2 of 2) (hsync_en0_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN0_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN0_FALL1_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN0_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_FALL1_ENABLE_MASK) >> MIRROR0_HSYNC_EN0_FALL1_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN0_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_FALL1_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN0_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN0_FALL1_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN0_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_FALL1_RESERVED1_MASK) >> MIRROR0_HSYNC_EN0_FALL1_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN0_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_FALL1_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN0_FALL1_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN0_FALL1_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN0_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN0_FALL1_SET_MASK) >> MIRROR0_HSYNC_EN0_FALL1_SET_SHIFT)
#define  MIRROR0_HSYNC_EN0_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN0_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN0_FALL1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Rising register (Instance 1 of 2) (hsync_en1_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN1_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN1_RISE0_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN1_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_RISE0_ENABLE_MASK) >> MIRROR0_HSYNC_EN1_RISE0_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN1_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_RISE0_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN1_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN1_RISE0_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN1_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_RISE0_RESERVED1_MASK) >> MIRROR0_HSYNC_EN1_RISE0_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN1_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_RISE0_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN1_RISE0_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN1_RISE0_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN1_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_RISE0_SET_MASK) >> MIRROR0_HSYNC_EN1_RISE0_SET_SHIFT)
#define  MIRROR0_HSYNC_EN1_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_RISE0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Rising register (Instance 2 of 2) (hsync_en1_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN1_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN1_RISE1_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN1_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_RISE1_ENABLE_MASK) >> MIRROR0_HSYNC_EN1_RISE1_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN1_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_RISE1_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN1_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN1_RISE1_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN1_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_RISE1_RESERVED1_MASK) >> MIRROR0_HSYNC_EN1_RISE1_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN1_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_RISE1_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN1_RISE1_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN1_RISE1_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN1_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_RISE1_SET_MASK) >> MIRROR0_HSYNC_EN1_RISE1_SET_SHIFT)
#define  MIRROR0_HSYNC_EN1_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_RISE1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Falling register (Instance 1 of 2) (hsync_en1_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN1_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN1_FALL0_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN1_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_FALL0_ENABLE_MASK) >> MIRROR0_HSYNC_EN1_FALL0_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN1_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_FALL0_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN1_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN1_FALL0_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN1_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_FALL0_RESERVED1_MASK) >> MIRROR0_HSYNC_EN1_FALL0_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN1_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_FALL0_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN1_FALL0_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN1_FALL0_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN1_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_FALL0_SET_MASK) >> MIRROR0_HSYNC_EN1_FALL0_SET_SHIFT)
#define  MIRROR0_HSYNC_EN1_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_FALL0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Falling register (Instance 2 of 2) (hsync_en1_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR0_HSYNC_EN1_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR0_HSYNC_EN1_FALL1_ENABLE_SHIFT 31
#define  MIRROR0_HSYNC_EN1_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_FALL1_ENABLE_MASK) >> MIRROR0_HSYNC_EN1_FALL1_ENABLE_SHIFT)
#define  MIRROR0_HSYNC_EN1_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_FALL1_ENABLE_SHIFT))

#define  MIRROR0_HSYNC_EN1_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR0_HSYNC_EN1_FALL1_RESERVED1_SHIFT 24
#define  MIRROR0_HSYNC_EN1_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_FALL1_RESERVED1_MASK) >> MIRROR0_HSYNC_EN1_FALL1_RESERVED1_SHIFT)
#define  MIRROR0_HSYNC_EN1_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_FALL1_RESERVED1_SHIFT))

#define  MIRROR0_HSYNC_EN1_FALL1_SET_MASK 0xffffff
#define  MIRROR0_HSYNC_EN1_FALL1_SET_SHIFT 0
#define  MIRROR0_HSYNC_EN1_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR0_HSYNC_EN1_FALL1_SET_MASK) >> MIRROR0_HSYNC_EN1_FALL1_SET_SHIFT)
#define  MIRROR0_HSYNC_EN1_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR0_HSYNC_EN1_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR0_HSYNC_EN1_FALL1_SET_SHIFT))

//====================================================================
//
//Register File: Mirror Control (Instance 2 of 2) (mirror1)
//
//====================================================================

//====================================================================
//Register: Mirror Configuration 1 register (mcfg1)
//====================================================================

#define  MIRROR1_MCFG1_MODE_MASK 0xc0000000
#define  MIRROR1_MCFG1_MODE_SHIFT 30
#define  MIRROR1_MCFG1_MODE_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG1_MODE_MASK) >> MIRROR1_MCFG1_MODE_SHIFT)
#define  MIRROR1_MCFG1_MODE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG1_MODE_MASK) | (((uint32_t)val) << MIRROR1_MCFG1_MODE_SHIFT))

#define  MIRROR1_MCFG1_HSYNC_SEL_MASK 0x30000000
#define  MIRROR1_MCFG1_HSYNC_SEL_SHIFT 28
#define  MIRROR1_MCFG1_HSYNC_SEL_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG1_HSYNC_SEL_MASK) >> MIRROR1_MCFG1_HSYNC_SEL_SHIFT)
#define  MIRROR1_MCFG1_HSYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG1_HSYNC_SEL_MASK) | (((uint32_t)val) << MIRROR1_MCFG1_HSYNC_SEL_SHIFT))

#define  MIRROR1_MCFG1_FACETS_MASK 0xf000000
#define  MIRROR1_MCFG1_FACETS_SHIFT 24
#define  MIRROR1_MCFG1_FACETS_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG1_FACETS_MASK) >> MIRROR1_MCFG1_FACETS_SHIFT)
#define  MIRROR1_MCFG1_FACETS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG1_FACETS_MASK) | (((uint32_t)val) << MIRROR1_MCFG1_FACETS_SHIFT))

#define  MIRROR1_MCFG1_CONTROL_MASK 0xffffff
#define  MIRROR1_MCFG1_CONTROL_SHIFT 0
#define  MIRROR1_MCFG1_CONTROL_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG1_CONTROL_MASK) >> MIRROR1_MCFG1_CONTROL_SHIFT)
#define  MIRROR1_MCFG1_CONTROL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG1_CONTROL_MASK) | (((uint32_t)val) << MIRROR1_MCFG1_CONTROL_SHIFT))

//====================================================================
//Register: Mirror Configuration 2 register (mcfg2)
//====================================================================

#define  MIRROR1_MCFG2_GALVOR_DIS_MASK 0xc0000000
#define  MIRROR1_MCFG2_GALVOR_DIS_SHIFT 30
#define  MIRROR1_MCFG2_GALVOR_DIS_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_GALVOR_DIS_MASK) >> MIRROR1_MCFG2_GALVOR_DIS_SHIFT)
#define  MIRROR1_MCFG2_GALVOR_DIS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_GALVOR_DIS_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_GALVOR_DIS_SHIFT))

#define  MIRROR1_MCFG2_GALVON_DIS_MASK 0x20000000
#define  MIRROR1_MCFG2_GALVON_DIS_SHIFT 29
#define  MIRROR1_MCFG2_GALVON_DIS_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_GALVON_DIS_MASK) >> MIRROR1_MCFG2_GALVON_DIS_SHIFT)
#define  MIRROR1_MCFG2_GALVON_DIS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_GALVON_DIS_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_GALVON_DIS_SHIFT))

#define  MIRROR1_MCFG2_GALVOP_DIS_MASK 0x10000000
#define  MIRROR1_MCFG2_GALVOP_DIS_SHIFT 28
#define  MIRROR1_MCFG2_GALVOP_DIS_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_GALVOP_DIS_MASK) >> MIRROR1_MCFG2_GALVOP_DIS_SHIFT)
#define  MIRROR1_MCFG2_GALVOP_DIS_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_GALVOP_DIS_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_GALVOP_DIS_SHIFT))

#define  MIRROR1_MCFG2_GALVOR_IDLE_MASK 0xc000000
#define  MIRROR1_MCFG2_GALVOR_IDLE_SHIFT 26
#define  MIRROR1_MCFG2_GALVOR_IDLE_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_GALVOR_IDLE_MASK) >> MIRROR1_MCFG2_GALVOR_IDLE_SHIFT)
#define  MIRROR1_MCFG2_GALVOR_IDLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_GALVOR_IDLE_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_GALVOR_IDLE_SHIFT))

#define  MIRROR1_MCFG2_GALVON_IDLE_MASK 0x2000000
#define  MIRROR1_MCFG2_GALVON_IDLE_SHIFT 25
#define  MIRROR1_MCFG2_GALVON_IDLE_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_GALVON_IDLE_MASK) >> MIRROR1_MCFG2_GALVON_IDLE_SHIFT)
#define  MIRROR1_MCFG2_GALVON_IDLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_GALVON_IDLE_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_GALVON_IDLE_SHIFT))

#define  MIRROR1_MCFG2_GALVOP_IDLE_MASK 0x1000000
#define  MIRROR1_MCFG2_GALVOP_IDLE_SHIFT 24
#define  MIRROR1_MCFG2_GALVOP_IDLE_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_GALVOP_IDLE_MASK) >> MIRROR1_MCFG2_GALVOP_IDLE_SHIFT)
#define  MIRROR1_MCFG2_GALVOP_IDLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_GALVOP_IDLE_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_GALVOP_IDLE_SHIFT))

#define  MIRROR1_MCFG2_RESERVED1_MASK 0x800000
#define  MIRROR1_MCFG2_RESERVED1_SHIFT 23
#define  MIRROR1_MCFG2_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_RESERVED1_MASK) >> MIRROR1_MCFG2_RESERVED1_SHIFT)
#define  MIRROR1_MCFG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_RESERVED1_SHIFT))

#define  MIRROR1_MCFG2_DEC_POL_MASK 0x400000
#define  MIRROR1_MCFG2_DEC_POL_SHIFT 22
#define  MIRROR1_MCFG2_DEC_POL_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_DEC_POL_MASK) >> MIRROR1_MCFG2_DEC_POL_SHIFT)
#define  MIRROR1_MCFG2_DEC_POL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_DEC_POL_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_DEC_POL_SHIFT))

#define  MIRROR1_MCFG2_ACC_POL_MASK 0x200000
#define  MIRROR1_MCFG2_ACC_POL_SHIFT 21
#define  MIRROR1_MCFG2_ACC_POL_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_ACC_POL_MASK) >> MIRROR1_MCFG2_ACC_POL_SHIFT)
#define  MIRROR1_MCFG2_ACC_POL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_ACC_POL_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_ACC_POL_SHIFT))

#define  MIRROR1_MCFG2_LOCK_POL_MASK 0x100000
#define  MIRROR1_MCFG2_LOCK_POL_SHIFT 20
#define  MIRROR1_MCFG2_LOCK_POL_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_LOCK_POL_MASK) >> MIRROR1_MCFG2_LOCK_POL_SHIFT)
#define  MIRROR1_MCFG2_LOCK_POL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_LOCK_POL_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_LOCK_POL_SHIFT))

#define  MIRROR1_MCFG2_RESERVED2_MASK 0xe0000
#define  MIRROR1_MCFG2_RESERVED2_SHIFT 17
#define  MIRROR1_MCFG2_RESERVED2_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_RESERVED2_MASK) >> MIRROR1_MCFG2_RESERVED2_SHIFT)
#define  MIRROR1_MCFG2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_RESERVED2_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_RESERVED2_SHIFT))

#define  MIRROR1_MCFG2_RECOV_MASK 0x10000
#define  MIRROR1_MCFG2_RECOV_SHIFT 16
#define  MIRROR1_MCFG2_RECOV_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_RECOV_MASK) >> MIRROR1_MCFG2_RECOV_SHIFT)
#define  MIRROR1_MCFG2_RECOV_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_RECOV_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_RECOV_SHIFT))

#define  MIRROR1_MCFG2_RESERVED3_MASK 0xf000
#define  MIRROR1_MCFG2_RESERVED3_SHIFT 12
#define  MIRROR1_MCFG2_RESERVED3_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_RESERVED3_MASK) >> MIRROR1_MCFG2_RESERVED3_SHIFT)
#define  MIRROR1_MCFG2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_RESERVED3_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_RESERVED3_SHIFT))

#define  MIRROR1_MCFG2_MAX_ADJ_MASK 0xf00
#define  MIRROR1_MCFG2_MAX_ADJ_SHIFT 8
#define  MIRROR1_MCFG2_MAX_ADJ_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_MAX_ADJ_MASK) >> MIRROR1_MCFG2_MAX_ADJ_SHIFT)
#define  MIRROR1_MCFG2_MAX_ADJ_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_MAX_ADJ_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_MAX_ADJ_SHIFT))

#define  MIRROR1_MCFG2_RESERVED4_MASK 0xf0
#define  MIRROR1_MCFG2_RESERVED4_SHIFT 4
#define  MIRROR1_MCFG2_RESERVED4_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_RESERVED4_MASK) >> MIRROR1_MCFG2_RESERVED4_SHIFT)
#define  MIRROR1_MCFG2_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_RESERVED4_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_RESERVED4_SHIFT))

#define  MIRROR1_MCFG2_MIN_ADJ_MASK 0xf
#define  MIRROR1_MCFG2_MIN_ADJ_SHIFT 0
#define  MIRROR1_MCFG2_MIN_ADJ_MASK_SHIFT(reg) (((reg) & MIRROR1_MCFG2_MIN_ADJ_MASK) >> MIRROR1_MCFG2_MIN_ADJ_SHIFT)
#define  MIRROR1_MCFG2_MIN_ADJ_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_MCFG2_MIN_ADJ_MASK) | (((uint32_t)val) << MIRROR1_MCFG2_MIN_ADJ_SHIFT))

//====================================================================
//Register: Mirror Status register (status)
//====================================================================

#define  MIRROR1_STATUS_RESERVED1_MASK 0xfc000000
#define  MIRROR1_STATUS_RESERVED1_SHIFT 26
#define  MIRROR1_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_STATUS_RESERVED1_MASK) >> MIRROR1_STATUS_RESERVED1_SHIFT)
#define  MIRROR1_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_STATUS_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_STATUS_RESERVED1_SHIFT))

#define  MIRROR1_STATUS_LOCK_DET_MASK 0x2000000
#define  MIRROR1_STATUS_LOCK_DET_SHIFT 25
#define  MIRROR1_STATUS_LOCK_DET_MASK_SHIFT(reg) (((reg) & MIRROR1_STATUS_LOCK_DET_MASK) >> MIRROR1_STATUS_LOCK_DET_SHIFT)
#define  MIRROR1_STATUS_LOCK_DET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_STATUS_LOCK_DET_MASK) | (((uint32_t)val) << MIRROR1_STATUS_LOCK_DET_SHIFT))

#define  MIRROR1_STATUS_LOCKED_MASK 0x1000000
#define  MIRROR1_STATUS_LOCKED_SHIFT 24
#define  MIRROR1_STATUS_LOCKED_MASK_SHIFT(reg) (((reg) & MIRROR1_STATUS_LOCKED_MASK) >> MIRROR1_STATUS_LOCKED_SHIFT)
#define  MIRROR1_STATUS_LOCKED_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_STATUS_LOCKED_MASK) | (((uint32_t)val) << MIRROR1_STATUS_LOCKED_SHIFT))

#define  MIRROR1_STATUS_PERIOD_MASK 0xffffff
#define  MIRROR1_STATUS_PERIOD_SHIFT 0
#define  MIRROR1_STATUS_PERIOD_MASK_SHIFT(reg) (((reg) & MIRROR1_STATUS_PERIOD_MASK) >> MIRROR1_STATUS_PERIOD_SHIFT)
#define  MIRROR1_STATUS_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_STATUS_PERIOD_MASK) | (((uint32_t)val) << MIRROR1_STATUS_PERIOD_SHIFT))

//====================================================================
//Register: Mirror Lock Max register (lockmax)
//====================================================================

#define  MIRROR1_LOCKMAX_RESERVED1_MASK 0xff000000
#define  MIRROR1_LOCKMAX_RESERVED1_SHIFT 24
#define  MIRROR1_LOCKMAX_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_LOCKMAX_RESERVED1_MASK) >> MIRROR1_LOCKMAX_RESERVED1_SHIFT)
#define  MIRROR1_LOCKMAX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_LOCKMAX_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_LOCKMAX_RESERVED1_SHIFT))

#define  MIRROR1_LOCKMAX_LOCKMAX_MASK 0xffffff
#define  MIRROR1_LOCKMAX_LOCKMAX_SHIFT 0
#define  MIRROR1_LOCKMAX_LOCKMAX_MASK_SHIFT(reg) (((reg) & MIRROR1_LOCKMAX_LOCKMAX_MASK) >> MIRROR1_LOCKMAX_LOCKMAX_SHIFT)
#define  MIRROR1_LOCKMAX_LOCKMAX_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_LOCKMAX_LOCKMAX_MASK) | (((uint32_t)val) << MIRROR1_LOCKMAX_LOCKMAX_SHIFT))

//====================================================================
//Register: Mirror Lock Min register (lockmin)
//====================================================================

#define  MIRROR1_LOCKMIN_RESERVED1_MASK 0xff000000
#define  MIRROR1_LOCKMIN_RESERVED1_SHIFT 24
#define  MIRROR1_LOCKMIN_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_LOCKMIN_RESERVED1_MASK) >> MIRROR1_LOCKMIN_RESERVED1_SHIFT)
#define  MIRROR1_LOCKMIN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_LOCKMIN_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_LOCKMIN_RESERVED1_SHIFT))

#define  MIRROR1_LOCKMIN_LOCKMIN_MASK 0xffffff
#define  MIRROR1_LOCKMIN_LOCKMIN_SHIFT 0
#define  MIRROR1_LOCKMIN_LOCKMIN_MASK_SHIFT(reg) (((reg) & MIRROR1_LOCKMIN_LOCKMIN_MASK) >> MIRROR1_LOCKMIN_LOCKMIN_SHIFT)
#define  MIRROR1_LOCKMIN_LOCKMIN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_LOCKMIN_LOCKMIN_MASK) | (((uint32_t)val) << MIRROR1_LOCKMIN_LOCKMIN_SHIFT))

//====================================================================
//Register: HSync Interrupt Configuration register (hsync_int)
//====================================================================

#define  MIRROR1_HSYNC_INT_RESERVED1_MASK 0xffffff00
#define  MIRROR1_HSYNC_INT_RESERVED1_SHIFT 8
#define  MIRROR1_HSYNC_INT_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_INT_RESERVED1_MASK) >> MIRROR1_HSYNC_INT_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_INT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_INT_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_INT_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_INT_HSYNC_SET_MASK 0xff
#define  MIRROR1_HSYNC_INT_HSYNC_SET_SHIFT 0
#define  MIRROR1_HSYNC_INT_HSYNC_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_INT_HSYNC_SET_MASK) >> MIRROR1_HSYNC_INT_HSYNC_SET_SHIFT)
#define  MIRROR1_HSYNC_INT_HSYNC_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_INT_HSYNC_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_INT_HSYNC_SET_SHIFT))

//====================================================================
//Register: Interrupt Enable register (int_en)
//====================================================================

#define  MIRROR1_INT_EN_RESERVED1_MASK 0xffffffe0
#define  MIRROR1_INT_EN_RESERVED1_SHIFT 5
#define  MIRROR1_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_EN_RESERVED1_MASK) >> MIRROR1_INT_EN_RESERVED1_SHIFT)
#define  MIRROR1_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_INT_EN_RESERVED1_SHIFT))

#define  MIRROR1_INT_EN_HSYNC_INT_EN_MASK 0x10
#define  MIRROR1_INT_EN_HSYNC_INT_EN_SHIFT 4
#define  MIRROR1_INT_EN_HSYNC_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_EN_HSYNC_INT_EN_MASK) >> MIRROR1_INT_EN_HSYNC_INT_EN_SHIFT)
#define  MIRROR1_INT_EN_HSYNC_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_EN_HSYNC_INT_EN_MASK) | (((uint32_t)val) << MIRROR1_INT_EN_HSYNC_INT_EN_SHIFT))

#define  MIRROR1_INT_EN_MEASR_INT_EN_MASK 0x8
#define  MIRROR1_INT_EN_MEASR_INT_EN_SHIFT 3
#define  MIRROR1_INT_EN_MEASR_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_EN_MEASR_INT_EN_MASK) >> MIRROR1_INT_EN_MEASR_INT_EN_SHIFT)
#define  MIRROR1_INT_EN_MEASR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_EN_MEASR_INT_EN_MASK) | (((uint32_t)val) << MIRROR1_INT_EN_MEASR_INT_EN_SHIFT))

#define  MIRROR1_INT_EN_ERROR_INT_EN_MASK 0x4
#define  MIRROR1_INT_EN_ERROR_INT_EN_SHIFT 2
#define  MIRROR1_INT_EN_ERROR_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_EN_ERROR_INT_EN_MASK) >> MIRROR1_INT_EN_ERROR_INT_EN_SHIFT)
#define  MIRROR1_INT_EN_ERROR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_EN_ERROR_INT_EN_MASK) | (((uint32_t)val) << MIRROR1_INT_EN_ERROR_INT_EN_SHIFT))

#define  MIRROR1_INT_EN_LATE_INT_EN_MASK 0x2
#define  MIRROR1_INT_EN_LATE_INT_EN_SHIFT 1
#define  MIRROR1_INT_EN_LATE_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_EN_LATE_INT_EN_MASK) >> MIRROR1_INT_EN_LATE_INT_EN_SHIFT)
#define  MIRROR1_INT_EN_LATE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_EN_LATE_INT_EN_MASK) | (((uint32_t)val) << MIRROR1_INT_EN_LATE_INT_EN_SHIFT))

#define  MIRROR1_INT_EN_EARLY_INT_EN_MASK 0x1
#define  MIRROR1_INT_EN_EARLY_INT_EN_SHIFT 0
#define  MIRROR1_INT_EN_EARLY_INT_EN_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_EN_EARLY_INT_EN_MASK) >> MIRROR1_INT_EN_EARLY_INT_EN_SHIFT)
#define  MIRROR1_INT_EN_EARLY_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_EN_EARLY_INT_EN_MASK) | (((uint32_t)val) << MIRROR1_INT_EN_EARLY_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending register (int_st)
//====================================================================

#define  MIRROR1_INT_ST_RESERVED1_MASK 0xffffffe0
#define  MIRROR1_INT_ST_RESERVED1_SHIFT 5
#define  MIRROR1_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_ST_RESERVED1_MASK) >> MIRROR1_INT_ST_RESERVED1_SHIFT)
#define  MIRROR1_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_INT_ST_RESERVED1_SHIFT))

#define  MIRROR1_INT_ST_HSYNC_INT_ST_MASK 0x10
#define  MIRROR1_INT_ST_HSYNC_INT_ST_SHIFT 4
#define  MIRROR1_INT_ST_HSYNC_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_ST_HSYNC_INT_ST_MASK) >> MIRROR1_INT_ST_HSYNC_INT_ST_SHIFT)
#define  MIRROR1_INT_ST_HSYNC_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_ST_HSYNC_INT_ST_MASK) | (((uint32_t)val) << MIRROR1_INT_ST_HSYNC_INT_ST_SHIFT))

#define  MIRROR1_INT_ST_MEASR_INT_ST_MASK 0x8
#define  MIRROR1_INT_ST_MEASR_INT_ST_SHIFT 3
#define  MIRROR1_INT_ST_MEASR_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_ST_MEASR_INT_ST_MASK) >> MIRROR1_INT_ST_MEASR_INT_ST_SHIFT)
#define  MIRROR1_INT_ST_MEASR_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_ST_MEASR_INT_ST_MASK) | (((uint32_t)val) << MIRROR1_INT_ST_MEASR_INT_ST_SHIFT))

#define  MIRROR1_INT_ST_ERROR_INT_ST_MASK 0x4
#define  MIRROR1_INT_ST_ERROR_INT_ST_SHIFT 2
#define  MIRROR1_INT_ST_ERROR_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_ST_ERROR_INT_ST_MASK) >> MIRROR1_INT_ST_ERROR_INT_ST_SHIFT)
#define  MIRROR1_INT_ST_ERROR_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_ST_ERROR_INT_ST_MASK) | (((uint32_t)val) << MIRROR1_INT_ST_ERROR_INT_ST_SHIFT))

#define  MIRROR1_INT_ST_LATE_INT_ST_MASK 0x2
#define  MIRROR1_INT_ST_LATE_INT_ST_SHIFT 1
#define  MIRROR1_INT_ST_LATE_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_ST_LATE_INT_ST_MASK) >> MIRROR1_INT_ST_LATE_INT_ST_SHIFT)
#define  MIRROR1_INT_ST_LATE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_ST_LATE_INT_ST_MASK) | (((uint32_t)val) << MIRROR1_INT_ST_LATE_INT_ST_SHIFT))

#define  MIRROR1_INT_ST_EARLY_INT_ST_MASK 0x1
#define  MIRROR1_INT_ST_EARLY_INT_ST_SHIFT 0
#define  MIRROR1_INT_ST_EARLY_INT_ST_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_ST_EARLY_INT_ST_MASK) >> MIRROR1_INT_ST_EARLY_INT_ST_SHIFT)
#define  MIRROR1_INT_ST_EARLY_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_ST_EARLY_INT_ST_MASK) | (((uint32_t)val) << MIRROR1_INT_ST_EARLY_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge register (int_cl)
//====================================================================

#define  MIRROR1_INT_CL_RESERVED1_MASK 0xffffffe0
#define  MIRROR1_INT_CL_RESERVED1_SHIFT 5
#define  MIRROR1_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_CL_RESERVED1_MASK) >> MIRROR1_INT_CL_RESERVED1_SHIFT)
#define  MIRROR1_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_INT_CL_RESERVED1_SHIFT))

#define  MIRROR1_INT_CL_HSYNC_INT_CL_MASK 0x10
#define  MIRROR1_INT_CL_HSYNC_INT_CL_SHIFT 4
#define  MIRROR1_INT_CL_HSYNC_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_CL_HSYNC_INT_CL_MASK) >> MIRROR1_INT_CL_HSYNC_INT_CL_SHIFT)
#define  MIRROR1_INT_CL_HSYNC_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_CL_HSYNC_INT_CL_MASK) | (((uint32_t)val) << MIRROR1_INT_CL_HSYNC_INT_CL_SHIFT))

#define  MIRROR1_INT_CL_MEASR_INT_CL_MASK 0x8
#define  MIRROR1_INT_CL_MEASR_INT_CL_SHIFT 3
#define  MIRROR1_INT_CL_MEASR_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_CL_MEASR_INT_CL_MASK) >> MIRROR1_INT_CL_MEASR_INT_CL_SHIFT)
#define  MIRROR1_INT_CL_MEASR_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_CL_MEASR_INT_CL_MASK) | (((uint32_t)val) << MIRROR1_INT_CL_MEASR_INT_CL_SHIFT))

#define  MIRROR1_INT_CL_ERROR_INT_CL_MASK 0x4
#define  MIRROR1_INT_CL_ERROR_INT_CL_SHIFT 2
#define  MIRROR1_INT_CL_ERROR_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_CL_ERROR_INT_CL_MASK) >> MIRROR1_INT_CL_ERROR_INT_CL_SHIFT)
#define  MIRROR1_INT_CL_ERROR_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_CL_ERROR_INT_CL_MASK) | (((uint32_t)val) << MIRROR1_INT_CL_ERROR_INT_CL_SHIFT))

#define  MIRROR1_INT_CL_LATE_INT_CL_MASK 0x2
#define  MIRROR1_INT_CL_LATE_INT_CL_SHIFT 1
#define  MIRROR1_INT_CL_LATE_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_CL_LATE_INT_CL_MASK) >> MIRROR1_INT_CL_LATE_INT_CL_SHIFT)
#define  MIRROR1_INT_CL_LATE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_CL_LATE_INT_CL_MASK) | (((uint32_t)val) << MIRROR1_INT_CL_LATE_INT_CL_SHIFT))

#define  MIRROR1_INT_CL_EARLY_INT_CL_MASK 0x1
#define  MIRROR1_INT_CL_EARLY_INT_CL_SHIFT 0
#define  MIRROR1_INT_CL_EARLY_INT_CL_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_CL_EARLY_INT_CL_MASK) >> MIRROR1_INT_CL_EARLY_INT_CL_SHIFT)
#define  MIRROR1_INT_CL_EARLY_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_CL_EARLY_INT_CL_MASK) | (((uint32_t)val) << MIRROR1_INT_CL_EARLY_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force register (int_fo)
//====================================================================

#define  MIRROR1_INT_FO_RESERVED1_MASK 0xffffffe0
#define  MIRROR1_INT_FO_RESERVED1_SHIFT 5
#define  MIRROR1_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_FO_RESERVED1_MASK) >> MIRROR1_INT_FO_RESERVED1_SHIFT)
#define  MIRROR1_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_INT_FO_RESERVED1_SHIFT))

#define  MIRROR1_INT_FO_HSYNC_INT_FO_MASK 0x10
#define  MIRROR1_INT_FO_HSYNC_INT_FO_SHIFT 4
#define  MIRROR1_INT_FO_HSYNC_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_FO_HSYNC_INT_FO_MASK) >> MIRROR1_INT_FO_HSYNC_INT_FO_SHIFT)
#define  MIRROR1_INT_FO_HSYNC_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_FO_HSYNC_INT_FO_MASK) | (((uint32_t)val) << MIRROR1_INT_FO_HSYNC_INT_FO_SHIFT))

#define  MIRROR1_INT_FO_MEASR_INT_FO_MASK 0x8
#define  MIRROR1_INT_FO_MEASR_INT_FO_SHIFT 3
#define  MIRROR1_INT_FO_MEASR_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_FO_MEASR_INT_FO_MASK) >> MIRROR1_INT_FO_MEASR_INT_FO_SHIFT)
#define  MIRROR1_INT_FO_MEASR_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_FO_MEASR_INT_FO_MASK) | (((uint32_t)val) << MIRROR1_INT_FO_MEASR_INT_FO_SHIFT))

#define  MIRROR1_INT_FO_ERROR_INT_FO_MASK 0x4
#define  MIRROR1_INT_FO_ERROR_INT_FO_SHIFT 2
#define  MIRROR1_INT_FO_ERROR_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_FO_ERROR_INT_FO_MASK) >> MIRROR1_INT_FO_ERROR_INT_FO_SHIFT)
#define  MIRROR1_INT_FO_ERROR_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_FO_ERROR_INT_FO_MASK) | (((uint32_t)val) << MIRROR1_INT_FO_ERROR_INT_FO_SHIFT))

#define  MIRROR1_INT_FO_LATE_INT_FO_MASK 0x2
#define  MIRROR1_INT_FO_LATE_INT_FO_SHIFT 1
#define  MIRROR1_INT_FO_LATE_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_FO_LATE_INT_FO_MASK) >> MIRROR1_INT_FO_LATE_INT_FO_SHIFT)
#define  MIRROR1_INT_FO_LATE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_FO_LATE_INT_FO_MASK) | (((uint32_t)val) << MIRROR1_INT_FO_LATE_INT_FO_SHIFT))

#define  MIRROR1_INT_FO_EARLY_INT_FO_MASK 0x1
#define  MIRROR1_INT_FO_EARLY_INT_FO_SHIFT 0
#define  MIRROR1_INT_FO_EARLY_INT_FO_MASK_SHIFT(reg) (((reg) & MIRROR1_INT_FO_EARLY_INT_FO_MASK) >> MIRROR1_INT_FO_EARLY_INT_FO_SHIFT)
#define  MIRROR1_INT_FO_EARLY_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_INT_FO_EARLY_INT_FO_MASK) | (((uint32_t)val) << MIRROR1_INT_FO_EARLY_INT_FO_SHIFT))

//====================================================================
//Register: SyncGalvo register (Instance 1 of 2) (sync_galvo0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_SYNC_GALVO0_ENABLE_MASK 0x80000000
#define  MIRROR1_SYNC_GALVO0_ENABLE_SHIFT 31
#define  MIRROR1_SYNC_GALVO0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_GALVO0_ENABLE_MASK) >> MIRROR1_SYNC_GALVO0_ENABLE_SHIFT)
#define  MIRROR1_SYNC_GALVO0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_GALVO0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_SYNC_GALVO0_ENABLE_SHIFT))

#define  MIRROR1_SYNC_GALVO0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_SYNC_GALVO0_RESERVED1_SHIFT 24
#define  MIRROR1_SYNC_GALVO0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_GALVO0_RESERVED1_MASK) >> MIRROR1_SYNC_GALVO0_RESERVED1_SHIFT)
#define  MIRROR1_SYNC_GALVO0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_GALVO0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_SYNC_GALVO0_RESERVED1_SHIFT))

#define  MIRROR1_SYNC_GALVO0_SET_MASK 0xffffff
#define  MIRROR1_SYNC_GALVO0_SET_SHIFT 0
#define  MIRROR1_SYNC_GALVO0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_GALVO0_SET_MASK) >> MIRROR1_SYNC_GALVO0_SET_SHIFT)
#define  MIRROR1_SYNC_GALVO0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_GALVO0_SET_MASK) | (((uint32_t)val) << MIRROR1_SYNC_GALVO0_SET_SHIFT))

//====================================================================
//Register: SyncGalvo register (Instance 2 of 2) (sync_galvo1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_SYNC_GALVO1_ENABLE_MASK 0x80000000
#define  MIRROR1_SYNC_GALVO1_ENABLE_SHIFT 31
#define  MIRROR1_SYNC_GALVO1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_GALVO1_ENABLE_MASK) >> MIRROR1_SYNC_GALVO1_ENABLE_SHIFT)
#define  MIRROR1_SYNC_GALVO1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_GALVO1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_SYNC_GALVO1_ENABLE_SHIFT))

#define  MIRROR1_SYNC_GALVO1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_SYNC_GALVO1_RESERVED1_SHIFT 24
#define  MIRROR1_SYNC_GALVO1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_GALVO1_RESERVED1_MASK) >> MIRROR1_SYNC_GALVO1_RESERVED1_SHIFT)
#define  MIRROR1_SYNC_GALVO1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_GALVO1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_SYNC_GALVO1_RESERVED1_SHIFT))

#define  MIRROR1_SYNC_GALVO1_SET_MASK 0xffffff
#define  MIRROR1_SYNC_GALVO1_SET_SHIFT 0
#define  MIRROR1_SYNC_GALVO1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_GALVO1_SET_MASK) >> MIRROR1_SYNC_GALVO1_SET_SHIFT)
#define  MIRROR1_SYNC_GALVO1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_GALVO1_SET_MASK) | (((uint32_t)val) << MIRROR1_SYNC_GALVO1_SET_SHIFT))

//====================================================================
//Register: SyncWFG register (Instance 1 of 2) (sync_wfg0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_SYNC_WFG0_ENABLE_MASK 0x80000000
#define  MIRROR1_SYNC_WFG0_ENABLE_SHIFT 31
#define  MIRROR1_SYNC_WFG0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_WFG0_ENABLE_MASK) >> MIRROR1_SYNC_WFG0_ENABLE_SHIFT)
#define  MIRROR1_SYNC_WFG0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_WFG0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_SYNC_WFG0_ENABLE_SHIFT))

#define  MIRROR1_SYNC_WFG0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_SYNC_WFG0_RESERVED1_SHIFT 24
#define  MIRROR1_SYNC_WFG0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_WFG0_RESERVED1_MASK) >> MIRROR1_SYNC_WFG0_RESERVED1_SHIFT)
#define  MIRROR1_SYNC_WFG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_WFG0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_SYNC_WFG0_RESERVED1_SHIFT))

#define  MIRROR1_SYNC_WFG0_SET_MASK 0xffffff
#define  MIRROR1_SYNC_WFG0_SET_SHIFT 0
#define  MIRROR1_SYNC_WFG0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_WFG0_SET_MASK) >> MIRROR1_SYNC_WFG0_SET_SHIFT)
#define  MIRROR1_SYNC_WFG0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_WFG0_SET_MASK) | (((uint32_t)val) << MIRROR1_SYNC_WFG0_SET_SHIFT))

//====================================================================
//Register: SyncWFG register (Instance 2 of 2) (sync_wfg1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_SYNC_WFG1_ENABLE_MASK 0x80000000
#define  MIRROR1_SYNC_WFG1_ENABLE_SHIFT 31
#define  MIRROR1_SYNC_WFG1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_WFG1_ENABLE_MASK) >> MIRROR1_SYNC_WFG1_ENABLE_SHIFT)
#define  MIRROR1_SYNC_WFG1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_WFG1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_SYNC_WFG1_ENABLE_SHIFT))

#define  MIRROR1_SYNC_WFG1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_SYNC_WFG1_RESERVED1_SHIFT 24
#define  MIRROR1_SYNC_WFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_WFG1_RESERVED1_MASK) >> MIRROR1_SYNC_WFG1_RESERVED1_SHIFT)
#define  MIRROR1_SYNC_WFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_WFG1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_SYNC_WFG1_RESERVED1_SHIFT))

#define  MIRROR1_SYNC_WFG1_SET_MASK 0xffffff
#define  MIRROR1_SYNC_WFG1_SET_SHIFT 0
#define  MIRROR1_SYNC_WFG1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_SYNC_WFG1_SET_MASK) >> MIRROR1_SYNC_WFG1_SET_SHIFT)
#define  MIRROR1_SYNC_WFG1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_SYNC_WFG1_SET_MASK) | (((uint32_t)val) << MIRROR1_SYNC_WFG1_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Rising register (Instance 1 of 2) (galvo_rtol_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_GALVO_RTOL_RISE0_RESERVED1_MASK 0xff000000
#define  MIRROR1_GALVO_RTOL_RISE0_RESERVED1_SHIFT 24
#define  MIRROR1_GALVO_RTOL_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_RISE0_RESERVED1_MASK) >> MIRROR1_GALVO_RTOL_RISE0_RESERVED1_SHIFT)
#define  MIRROR1_GALVO_RTOL_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_RISE0_RESERVED1_SHIFT))

#define  MIRROR1_GALVO_RTOL_RISE0_SET_MASK 0xffffff
#define  MIRROR1_GALVO_RTOL_RISE0_SET_SHIFT 0
#define  MIRROR1_GALVO_RTOL_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_RISE0_SET_MASK) >> MIRROR1_GALVO_RTOL_RISE0_SET_SHIFT)
#define  MIRROR1_GALVO_RTOL_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_RISE0_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Rising register (Instance 2 of 2) (galvo_rtol_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_GALVO_RTOL_RISE1_RESERVED1_MASK 0xff000000
#define  MIRROR1_GALVO_RTOL_RISE1_RESERVED1_SHIFT 24
#define  MIRROR1_GALVO_RTOL_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_RISE1_RESERVED1_MASK) >> MIRROR1_GALVO_RTOL_RISE1_RESERVED1_SHIFT)
#define  MIRROR1_GALVO_RTOL_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_RISE1_RESERVED1_SHIFT))

#define  MIRROR1_GALVO_RTOL_RISE1_SET_MASK 0xffffff
#define  MIRROR1_GALVO_RTOL_RISE1_SET_SHIFT 0
#define  MIRROR1_GALVO_RTOL_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_RISE1_SET_MASK) >> MIRROR1_GALVO_RTOL_RISE1_SET_SHIFT)
#define  MIRROR1_GALVO_RTOL_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_RISE1_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Falling register (Instance 1 of 2) (galvo_rtol_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_GALVO_RTOL_FALL0_RESERVED1_MASK 0xff000000
#define  MIRROR1_GALVO_RTOL_FALL0_RESERVED1_SHIFT 24
#define  MIRROR1_GALVO_RTOL_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_FALL0_RESERVED1_MASK) >> MIRROR1_GALVO_RTOL_FALL0_RESERVED1_SHIFT)
#define  MIRROR1_GALVO_RTOL_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_FALL0_RESERVED1_SHIFT))

#define  MIRROR1_GALVO_RTOL_FALL0_SET_MASK 0xffffff
#define  MIRROR1_GALVO_RTOL_FALL0_SET_SHIFT 0
#define  MIRROR1_GALVO_RTOL_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_FALL0_SET_MASK) >> MIRROR1_GALVO_RTOL_FALL0_SET_SHIFT)
#define  MIRROR1_GALVO_RTOL_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_FALL0_SET_SHIFT))

//====================================================================
//Register: GalvoRtoL Falling register (Instance 2 of 2) (galvo_rtol_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_GALVO_RTOL_FALL1_RESERVED1_MASK 0xff000000
#define  MIRROR1_GALVO_RTOL_FALL1_RESERVED1_SHIFT 24
#define  MIRROR1_GALVO_RTOL_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_FALL1_RESERVED1_MASK) >> MIRROR1_GALVO_RTOL_FALL1_RESERVED1_SHIFT)
#define  MIRROR1_GALVO_RTOL_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_FALL1_RESERVED1_SHIFT))

#define  MIRROR1_GALVO_RTOL_FALL1_SET_MASK 0xffffff
#define  MIRROR1_GALVO_RTOL_FALL1_SET_SHIFT 0
#define  MIRROR1_GALVO_RTOL_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVO_RTOL_FALL1_SET_MASK) >> MIRROR1_GALVO_RTOL_FALL1_SET_SHIFT)
#define  MIRROR1_GALVO_RTOL_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVO_RTOL_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVO_RTOL_FALL1_SET_SHIFT))

//====================================================================
//Register: GalvoP Rising register (Instance 1 of 2) (galvop_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_GALVOP_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVOP_RISE0_ENABLE_SHIFT 31
#define  MIRROR1_GALVOP_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_RISE0_ENABLE_MASK) >> MIRROR1_GALVOP_RISE0_ENABLE_SHIFT)
#define  MIRROR1_GALVOP_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_RISE0_ENABLE_SHIFT))

#define  MIRROR1_GALVOP_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVOP_RISE0_RESERVED1_SHIFT 24
#define  MIRROR1_GALVOP_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_RISE0_RESERVED1_MASK) >> MIRROR1_GALVOP_RISE0_RESERVED1_SHIFT)
#define  MIRROR1_GALVOP_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_RISE0_RESERVED1_SHIFT))

#define  MIRROR1_GALVOP_RISE0_SET_MASK 0xffffff
#define  MIRROR1_GALVOP_RISE0_SET_SHIFT 0
#define  MIRROR1_GALVOP_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_RISE0_SET_MASK) >> MIRROR1_GALVOP_RISE0_SET_SHIFT)
#define  MIRROR1_GALVOP_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_RISE0_SET_SHIFT))

//====================================================================
//Register: GalvoP Rising register (Instance 2 of 2) (galvop_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_GALVOP_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVOP_RISE1_ENABLE_SHIFT 31
#define  MIRROR1_GALVOP_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_RISE1_ENABLE_MASK) >> MIRROR1_GALVOP_RISE1_ENABLE_SHIFT)
#define  MIRROR1_GALVOP_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_RISE1_ENABLE_SHIFT))

#define  MIRROR1_GALVOP_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVOP_RISE1_RESERVED1_SHIFT 24
#define  MIRROR1_GALVOP_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_RISE1_RESERVED1_MASK) >> MIRROR1_GALVOP_RISE1_RESERVED1_SHIFT)
#define  MIRROR1_GALVOP_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_RISE1_RESERVED1_SHIFT))

#define  MIRROR1_GALVOP_RISE1_SET_MASK 0xffffff
#define  MIRROR1_GALVOP_RISE1_SET_SHIFT 0
#define  MIRROR1_GALVOP_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_RISE1_SET_MASK) >> MIRROR1_GALVOP_RISE1_SET_SHIFT)
#define  MIRROR1_GALVOP_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_RISE1_SET_SHIFT))

//====================================================================
//Register: GalvoP Falling register (Instance 1 of 2) (galvop_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_GALVOP_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVOP_FALL0_ENABLE_SHIFT 31
#define  MIRROR1_GALVOP_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_FALL0_ENABLE_MASK) >> MIRROR1_GALVOP_FALL0_ENABLE_SHIFT)
#define  MIRROR1_GALVOP_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_FALL0_ENABLE_SHIFT))

#define  MIRROR1_GALVOP_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVOP_FALL0_RESERVED1_SHIFT 24
#define  MIRROR1_GALVOP_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_FALL0_RESERVED1_MASK) >> MIRROR1_GALVOP_FALL0_RESERVED1_SHIFT)
#define  MIRROR1_GALVOP_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_FALL0_RESERVED1_SHIFT))

#define  MIRROR1_GALVOP_FALL0_SET_MASK 0xffffff
#define  MIRROR1_GALVOP_FALL0_SET_SHIFT 0
#define  MIRROR1_GALVOP_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_FALL0_SET_MASK) >> MIRROR1_GALVOP_FALL0_SET_SHIFT)
#define  MIRROR1_GALVOP_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_FALL0_SET_SHIFT))

//====================================================================
//Register: GalvoP Falling register (Instance 2 of 2) (galvop_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_GALVOP_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVOP_FALL1_ENABLE_SHIFT 31
#define  MIRROR1_GALVOP_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_FALL1_ENABLE_MASK) >> MIRROR1_GALVOP_FALL1_ENABLE_SHIFT)
#define  MIRROR1_GALVOP_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_FALL1_ENABLE_SHIFT))

#define  MIRROR1_GALVOP_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVOP_FALL1_RESERVED1_SHIFT 24
#define  MIRROR1_GALVOP_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_FALL1_RESERVED1_MASK) >> MIRROR1_GALVOP_FALL1_RESERVED1_SHIFT)
#define  MIRROR1_GALVOP_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_FALL1_RESERVED1_SHIFT))

#define  MIRROR1_GALVOP_FALL1_SET_MASK 0xffffff
#define  MIRROR1_GALVOP_FALL1_SET_SHIFT 0
#define  MIRROR1_GALVOP_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVOP_FALL1_SET_MASK) >> MIRROR1_GALVOP_FALL1_SET_SHIFT)
#define  MIRROR1_GALVOP_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVOP_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVOP_FALL1_SET_SHIFT))

//====================================================================
//Register: GalvoN Rising register (Instance 1 of 2) (galvon_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_GALVON_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVON_RISE0_ENABLE_SHIFT 31
#define  MIRROR1_GALVON_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_RISE0_ENABLE_MASK) >> MIRROR1_GALVON_RISE0_ENABLE_SHIFT)
#define  MIRROR1_GALVON_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVON_RISE0_ENABLE_SHIFT))

#define  MIRROR1_GALVON_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVON_RISE0_RESERVED1_SHIFT 24
#define  MIRROR1_GALVON_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_RISE0_RESERVED1_MASK) >> MIRROR1_GALVON_RISE0_RESERVED1_SHIFT)
#define  MIRROR1_GALVON_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVON_RISE0_RESERVED1_SHIFT))

#define  MIRROR1_GALVON_RISE0_SET_MASK 0xffffff
#define  MIRROR1_GALVON_RISE0_SET_SHIFT 0
#define  MIRROR1_GALVON_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_RISE0_SET_MASK) >> MIRROR1_GALVON_RISE0_SET_SHIFT)
#define  MIRROR1_GALVON_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVON_RISE0_SET_SHIFT))

//====================================================================
//Register: GalvoN Rising register (Instance 2 of 2) (galvon_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_GALVON_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVON_RISE1_ENABLE_SHIFT 31
#define  MIRROR1_GALVON_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_RISE1_ENABLE_MASK) >> MIRROR1_GALVON_RISE1_ENABLE_SHIFT)
#define  MIRROR1_GALVON_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVON_RISE1_ENABLE_SHIFT))

#define  MIRROR1_GALVON_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVON_RISE1_RESERVED1_SHIFT 24
#define  MIRROR1_GALVON_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_RISE1_RESERVED1_MASK) >> MIRROR1_GALVON_RISE1_RESERVED1_SHIFT)
#define  MIRROR1_GALVON_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVON_RISE1_RESERVED1_SHIFT))

#define  MIRROR1_GALVON_RISE1_SET_MASK 0xffffff
#define  MIRROR1_GALVON_RISE1_SET_SHIFT 0
#define  MIRROR1_GALVON_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_RISE1_SET_MASK) >> MIRROR1_GALVON_RISE1_SET_SHIFT)
#define  MIRROR1_GALVON_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVON_RISE1_SET_SHIFT))

//====================================================================
//Register: GalvoN Falling register (Instance 1 of 2) (galvon_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_GALVON_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVON_FALL0_ENABLE_SHIFT 31
#define  MIRROR1_GALVON_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_FALL0_ENABLE_MASK) >> MIRROR1_GALVON_FALL0_ENABLE_SHIFT)
#define  MIRROR1_GALVON_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVON_FALL0_ENABLE_SHIFT))

#define  MIRROR1_GALVON_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVON_FALL0_RESERVED1_SHIFT 24
#define  MIRROR1_GALVON_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_FALL0_RESERVED1_MASK) >> MIRROR1_GALVON_FALL0_RESERVED1_SHIFT)
#define  MIRROR1_GALVON_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVON_FALL0_RESERVED1_SHIFT))

#define  MIRROR1_GALVON_FALL0_SET_MASK 0xffffff
#define  MIRROR1_GALVON_FALL0_SET_SHIFT 0
#define  MIRROR1_GALVON_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_FALL0_SET_MASK) >> MIRROR1_GALVON_FALL0_SET_SHIFT)
#define  MIRROR1_GALVON_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVON_FALL0_SET_SHIFT))

//====================================================================
//Register: GalvoN Falling register (Instance 2 of 2) (galvon_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_GALVON_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR1_GALVON_FALL1_ENABLE_SHIFT 31
#define  MIRROR1_GALVON_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_FALL1_ENABLE_MASK) >> MIRROR1_GALVON_FALL1_ENABLE_SHIFT)
#define  MIRROR1_GALVON_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_GALVON_FALL1_ENABLE_SHIFT))

#define  MIRROR1_GALVON_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_GALVON_FALL1_RESERVED1_SHIFT 24
#define  MIRROR1_GALVON_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_FALL1_RESERVED1_MASK) >> MIRROR1_GALVON_FALL1_RESERVED1_SHIFT)
#define  MIRROR1_GALVON_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_GALVON_FALL1_RESERVED1_SHIFT))

#define  MIRROR1_GALVON_FALL1_SET_MASK 0xffffff
#define  MIRROR1_GALVON_FALL1_SET_SHIFT 0
#define  MIRROR1_GALVON_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_GALVON_FALL1_SET_MASK) >> MIRROR1_GALVON_FALL1_SET_SHIFT)
#define  MIRROR1_GALVON_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_GALVON_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR1_GALVON_FALL1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Rising register (Instance 1 of 2) (hsync_en0_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN0_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN0_RISE0_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN0_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_RISE0_ENABLE_MASK) >> MIRROR1_HSYNC_EN0_RISE0_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN0_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_RISE0_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN0_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN0_RISE0_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN0_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_RISE0_RESERVED1_MASK) >> MIRROR1_HSYNC_EN0_RISE0_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN0_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_RISE0_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN0_RISE0_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN0_RISE0_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN0_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_RISE0_SET_MASK) >> MIRROR1_HSYNC_EN0_RISE0_SET_SHIFT)
#define  MIRROR1_HSYNC_EN0_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_RISE0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Rising register (Instance 2 of 2) (hsync_en0_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN0_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN0_RISE1_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN0_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_RISE1_ENABLE_MASK) >> MIRROR1_HSYNC_EN0_RISE1_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN0_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_RISE1_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN0_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN0_RISE1_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN0_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_RISE1_RESERVED1_MASK) >> MIRROR1_HSYNC_EN0_RISE1_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN0_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_RISE1_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN0_RISE1_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN0_RISE1_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN0_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_RISE1_SET_MASK) >> MIRROR1_HSYNC_EN0_RISE1_SET_SHIFT)
#define  MIRROR1_HSYNC_EN0_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_RISE1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Falling register (Instance 1 of 2) (hsync_en0_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN0_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN0_FALL0_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN0_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_FALL0_ENABLE_MASK) >> MIRROR1_HSYNC_EN0_FALL0_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN0_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_FALL0_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN0_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN0_FALL0_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN0_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_FALL0_RESERVED1_MASK) >> MIRROR1_HSYNC_EN0_FALL0_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN0_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_FALL0_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN0_FALL0_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN0_FALL0_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN0_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_FALL0_SET_MASK) >> MIRROR1_HSYNC_EN0_FALL0_SET_SHIFT)
#define  MIRROR1_HSYNC_EN0_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_FALL0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable0 Falling register (Instance 2 of 2) (hsync_en0_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN0_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN0_FALL1_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN0_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_FALL1_ENABLE_MASK) >> MIRROR1_HSYNC_EN0_FALL1_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN0_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_FALL1_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN0_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN0_FALL1_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN0_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_FALL1_RESERVED1_MASK) >> MIRROR1_HSYNC_EN0_FALL1_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN0_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_FALL1_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN0_FALL1_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN0_FALL1_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN0_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN0_FALL1_SET_MASK) >> MIRROR1_HSYNC_EN0_FALL1_SET_SHIFT)
#define  MIRROR1_HSYNC_EN0_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN0_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN0_FALL1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Rising register (Instance 1 of 2) (hsync_en1_rise0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN1_RISE0_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN1_RISE0_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN1_RISE0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_RISE0_ENABLE_MASK) >> MIRROR1_HSYNC_EN1_RISE0_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN1_RISE0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_RISE0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_RISE0_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN1_RISE0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN1_RISE0_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN1_RISE0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_RISE0_RESERVED1_MASK) >> MIRROR1_HSYNC_EN1_RISE0_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN1_RISE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_RISE0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_RISE0_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN1_RISE0_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN1_RISE0_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN1_RISE0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_RISE0_SET_MASK) >> MIRROR1_HSYNC_EN1_RISE0_SET_SHIFT)
#define  MIRROR1_HSYNC_EN1_RISE0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_RISE0_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_RISE0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Rising register (Instance 2 of 2) (hsync_en1_rise1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN1_RISE1_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN1_RISE1_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN1_RISE1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_RISE1_ENABLE_MASK) >> MIRROR1_HSYNC_EN1_RISE1_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN1_RISE1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_RISE1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_RISE1_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN1_RISE1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN1_RISE1_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN1_RISE1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_RISE1_RESERVED1_MASK) >> MIRROR1_HSYNC_EN1_RISE1_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN1_RISE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_RISE1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_RISE1_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN1_RISE1_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN1_RISE1_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN1_RISE1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_RISE1_SET_MASK) >> MIRROR1_HSYNC_EN1_RISE1_SET_SHIFT)
#define  MIRROR1_HSYNC_EN1_RISE1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_RISE1_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_RISE1_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Falling register (Instance 1 of 2) (hsync_en1_fall0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN1_FALL0_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN1_FALL0_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN1_FALL0_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_FALL0_ENABLE_MASK) >> MIRROR1_HSYNC_EN1_FALL0_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN1_FALL0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_FALL0_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_FALL0_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN1_FALL0_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN1_FALL0_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN1_FALL0_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_FALL0_RESERVED1_MASK) >> MIRROR1_HSYNC_EN1_FALL0_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN1_FALL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_FALL0_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_FALL0_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN1_FALL0_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN1_FALL0_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN1_FALL0_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_FALL0_SET_MASK) >> MIRROR1_HSYNC_EN1_FALL0_SET_SHIFT)
#define  MIRROR1_HSYNC_EN1_FALL0_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_FALL0_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_FALL0_SET_SHIFT))

//====================================================================
//Register: HSyncEnable1 Falling register (Instance 2 of 2) (hsync_en1_fall1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  MIRROR1_HSYNC_EN1_FALL1_ENABLE_MASK 0x80000000
#define  MIRROR1_HSYNC_EN1_FALL1_ENABLE_SHIFT 31
#define  MIRROR1_HSYNC_EN1_FALL1_ENABLE_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_FALL1_ENABLE_MASK) >> MIRROR1_HSYNC_EN1_FALL1_ENABLE_SHIFT)
#define  MIRROR1_HSYNC_EN1_FALL1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_FALL1_ENABLE_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_FALL1_ENABLE_SHIFT))

#define  MIRROR1_HSYNC_EN1_FALL1_RESERVED1_MASK 0x7f000000
#define  MIRROR1_HSYNC_EN1_FALL1_RESERVED1_SHIFT 24
#define  MIRROR1_HSYNC_EN1_FALL1_RESERVED1_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_FALL1_RESERVED1_MASK) >> MIRROR1_HSYNC_EN1_FALL1_RESERVED1_SHIFT)
#define  MIRROR1_HSYNC_EN1_FALL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_FALL1_RESERVED1_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_FALL1_RESERVED1_SHIFT))

#define  MIRROR1_HSYNC_EN1_FALL1_SET_MASK 0xffffff
#define  MIRROR1_HSYNC_EN1_FALL1_SET_SHIFT 0
#define  MIRROR1_HSYNC_EN1_FALL1_SET_MASK_SHIFT(reg) (((reg) & MIRROR1_HSYNC_EN1_FALL1_SET_MASK) >> MIRROR1_HSYNC_EN1_FALL1_SET_SHIFT)
#define  MIRROR1_HSYNC_EN1_FALL1_SET_REPLACE_VAL(reg,val) (((reg) & ~MIRROR1_HSYNC_EN1_FALL1_SET_MASK) | (((uint32_t)val) << MIRROR1_HSYNC_EN1_FALL1_SET_SHIFT))

//====================================================================
//
//Register File: Waverform Generator Control (Instance 1 of 4) (wfg0)
//
//====================================================================

//====================================================================
//Register: Waveform Generator Configuration register (scfg)
//====================================================================

#define  WFG0_SCFG_MODE_MASK 0xc0000000
#define  WFG0_SCFG_MODE_SHIFT 30
#define  WFG0_SCFG_MODE_MASK_SHIFT(reg) (((reg) & WFG0_SCFG_MODE_MASK) >> WFG0_SCFG_MODE_SHIFT)
#define  WFG0_SCFG_MODE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_SCFG_MODE_MASK) | (((uint32_t)val) << WFG0_SCFG_MODE_SHIFT))

#define  WFG0_SCFG_RECOV_MASK 0x20000000
#define  WFG0_SCFG_RECOV_SHIFT 29
#define  WFG0_SCFG_RECOV_MASK_SHIFT(reg) (((reg) & WFG0_SCFG_RECOV_MASK) >> WFG0_SCFG_RECOV_SHIFT)
#define  WFG0_SCFG_RECOV_REPLACE_VAL(reg,val) (((reg) & ~WFG0_SCFG_RECOV_MASK) | (((uint32_t)val) << WFG0_SCFG_RECOV_SHIFT))

#define  WFG0_SCFG_SYNC_SEL_MASK 0x10000000
#define  WFG0_SCFG_SYNC_SEL_SHIFT 28
#define  WFG0_SCFG_SYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG0_SCFG_SYNC_SEL_MASK) >> WFG0_SCFG_SYNC_SEL_SHIFT)
#define  WFG0_SCFG_SYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG0_SCFG_SYNC_SEL_MASK) | (((uint32_t)val) << WFG0_SCFG_SYNC_SEL_SHIFT))

#define  WFG0_SCFG_GALVO_SEL_MASK 0xc000000
#define  WFG0_SCFG_GALVO_SEL_SHIFT 26
#define  WFG0_SCFG_GALVO_SEL_MASK_SHIFT(reg) (((reg) & WFG0_SCFG_GALVO_SEL_MASK) >> WFG0_SCFG_GALVO_SEL_SHIFT)
#define  WFG0_SCFG_GALVO_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG0_SCFG_GALVO_SEL_MASK) | (((uint32_t)val) << WFG0_SCFG_GALVO_SEL_SHIFT))

#define  WFG0_SCFG_HSYNC_SEL_MASK 0x3000000
#define  WFG0_SCFG_HSYNC_SEL_SHIFT 24
#define  WFG0_SCFG_HSYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG0_SCFG_HSYNC_SEL_MASK) >> WFG0_SCFG_HSYNC_SEL_SHIFT)
#define  WFG0_SCFG_HSYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG0_SCFG_HSYNC_SEL_MASK) | (((uint32_t)val) << WFG0_SCFG_HSYNC_SEL_SHIFT))

#define  WFG0_SCFG_COUNT_MASK 0xffffff
#define  WFG0_SCFG_COUNT_SHIFT 0
#define  WFG0_SCFG_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_SCFG_COUNT_MASK) >> WFG0_SCFG_COUNT_SHIFT)
#define  WFG0_SCFG_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_SCFG_COUNT_MASK) | (((uint32_t)val) << WFG0_SCFG_COUNT_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 1 of 10) (period0)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG0_PERIOD0_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD0_RESERVED1_SHIFT 17
#define  WFG0_PERIOD0_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD0_RESERVED1_MASK) >> WFG0_PERIOD0_RESERVED1_SHIFT)
#define  WFG0_PERIOD0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD0_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD0_RESERVED1_SHIFT))

#define  WFG0_PERIOD0_IDLE_MASK 0x10000
#define  WFG0_PERIOD0_IDLE_SHIFT 16
#define  WFG0_PERIOD0_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD0_IDLE_MASK) >> WFG0_PERIOD0_IDLE_SHIFT)
#define  WFG0_PERIOD0_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD0_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD0_IDLE_SHIFT))

#define  WFG0_PERIOD0_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD0_ACTIVE_SHIFT 8
#define  WFG0_PERIOD0_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD0_ACTIVE_MASK) >> WFG0_PERIOD0_ACTIVE_SHIFT)
#define  WFG0_PERIOD0_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD0_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD0_ACTIVE_SHIFT))

#define  WFG0_PERIOD0_PERIOD_MASK 0xff
#define  WFG0_PERIOD0_PERIOD_SHIFT 0
#define  WFG0_PERIOD0_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD0_PERIOD_MASK) >> WFG0_PERIOD0_PERIOD_SHIFT)
#define  WFG0_PERIOD0_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD0_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD0_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 2 of 10) (period1)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG0_PERIOD1_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD1_RESERVED1_SHIFT 17
#define  WFG0_PERIOD1_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD1_RESERVED1_MASK) >> WFG0_PERIOD1_RESERVED1_SHIFT)
#define  WFG0_PERIOD1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD1_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD1_RESERVED1_SHIFT))

#define  WFG0_PERIOD1_IDLE_MASK 0x10000
#define  WFG0_PERIOD1_IDLE_SHIFT 16
#define  WFG0_PERIOD1_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD1_IDLE_MASK) >> WFG0_PERIOD1_IDLE_SHIFT)
#define  WFG0_PERIOD1_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD1_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD1_IDLE_SHIFT))

#define  WFG0_PERIOD1_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD1_ACTIVE_SHIFT 8
#define  WFG0_PERIOD1_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD1_ACTIVE_MASK) >> WFG0_PERIOD1_ACTIVE_SHIFT)
#define  WFG0_PERIOD1_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD1_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD1_ACTIVE_SHIFT))

#define  WFG0_PERIOD1_PERIOD_MASK 0xff
#define  WFG0_PERIOD1_PERIOD_SHIFT 0
#define  WFG0_PERIOD1_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD1_PERIOD_MASK) >> WFG0_PERIOD1_PERIOD_SHIFT)
#define  WFG0_PERIOD1_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD1_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD1_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 3 of 10) (period2)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG0_PERIOD2_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD2_RESERVED1_SHIFT 17
#define  WFG0_PERIOD2_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD2_RESERVED1_MASK) >> WFG0_PERIOD2_RESERVED1_SHIFT)
#define  WFG0_PERIOD2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD2_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD2_RESERVED1_SHIFT))

#define  WFG0_PERIOD2_IDLE_MASK 0x10000
#define  WFG0_PERIOD2_IDLE_SHIFT 16
#define  WFG0_PERIOD2_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD2_IDLE_MASK) >> WFG0_PERIOD2_IDLE_SHIFT)
#define  WFG0_PERIOD2_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD2_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD2_IDLE_SHIFT))

#define  WFG0_PERIOD2_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD2_ACTIVE_SHIFT 8
#define  WFG0_PERIOD2_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD2_ACTIVE_MASK) >> WFG0_PERIOD2_ACTIVE_SHIFT)
#define  WFG0_PERIOD2_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD2_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD2_ACTIVE_SHIFT))

#define  WFG0_PERIOD2_PERIOD_MASK 0xff
#define  WFG0_PERIOD2_PERIOD_SHIFT 0
#define  WFG0_PERIOD2_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD2_PERIOD_MASK) >> WFG0_PERIOD2_PERIOD_SHIFT)
#define  WFG0_PERIOD2_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD2_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD2_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 4 of 10) (period3)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG0_PERIOD3_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD3_RESERVED1_SHIFT 17
#define  WFG0_PERIOD3_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD3_RESERVED1_MASK) >> WFG0_PERIOD3_RESERVED1_SHIFT)
#define  WFG0_PERIOD3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD3_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD3_RESERVED1_SHIFT))

#define  WFG0_PERIOD3_IDLE_MASK 0x10000
#define  WFG0_PERIOD3_IDLE_SHIFT 16
#define  WFG0_PERIOD3_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD3_IDLE_MASK) >> WFG0_PERIOD3_IDLE_SHIFT)
#define  WFG0_PERIOD3_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD3_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD3_IDLE_SHIFT))

#define  WFG0_PERIOD3_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD3_ACTIVE_SHIFT 8
#define  WFG0_PERIOD3_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD3_ACTIVE_MASK) >> WFG0_PERIOD3_ACTIVE_SHIFT)
#define  WFG0_PERIOD3_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD3_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD3_ACTIVE_SHIFT))

#define  WFG0_PERIOD3_PERIOD_MASK 0xff
#define  WFG0_PERIOD3_PERIOD_SHIFT 0
#define  WFG0_PERIOD3_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD3_PERIOD_MASK) >> WFG0_PERIOD3_PERIOD_SHIFT)
#define  WFG0_PERIOD3_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD3_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD3_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 5 of 10) (period4)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG0_PERIOD4_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD4_RESERVED1_SHIFT 17
#define  WFG0_PERIOD4_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD4_RESERVED1_MASK) >> WFG0_PERIOD4_RESERVED1_SHIFT)
#define  WFG0_PERIOD4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD4_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD4_RESERVED1_SHIFT))

#define  WFG0_PERIOD4_IDLE_MASK 0x10000
#define  WFG0_PERIOD4_IDLE_SHIFT 16
#define  WFG0_PERIOD4_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD4_IDLE_MASK) >> WFG0_PERIOD4_IDLE_SHIFT)
#define  WFG0_PERIOD4_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD4_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD4_IDLE_SHIFT))

#define  WFG0_PERIOD4_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD4_ACTIVE_SHIFT 8
#define  WFG0_PERIOD4_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD4_ACTIVE_MASK) >> WFG0_PERIOD4_ACTIVE_SHIFT)
#define  WFG0_PERIOD4_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD4_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD4_ACTIVE_SHIFT))

#define  WFG0_PERIOD4_PERIOD_MASK 0xff
#define  WFG0_PERIOD4_PERIOD_SHIFT 0
#define  WFG0_PERIOD4_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD4_PERIOD_MASK) >> WFG0_PERIOD4_PERIOD_SHIFT)
#define  WFG0_PERIOD4_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD4_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD4_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 6 of 10) (period5)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG0_PERIOD5_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD5_RESERVED1_SHIFT 17
#define  WFG0_PERIOD5_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD5_RESERVED1_MASK) >> WFG0_PERIOD5_RESERVED1_SHIFT)
#define  WFG0_PERIOD5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD5_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD5_RESERVED1_SHIFT))

#define  WFG0_PERIOD5_IDLE_MASK 0x10000
#define  WFG0_PERIOD5_IDLE_SHIFT 16
#define  WFG0_PERIOD5_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD5_IDLE_MASK) >> WFG0_PERIOD5_IDLE_SHIFT)
#define  WFG0_PERIOD5_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD5_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD5_IDLE_SHIFT))

#define  WFG0_PERIOD5_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD5_ACTIVE_SHIFT 8
#define  WFG0_PERIOD5_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD5_ACTIVE_MASK) >> WFG0_PERIOD5_ACTIVE_SHIFT)
#define  WFG0_PERIOD5_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD5_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD5_ACTIVE_SHIFT))

#define  WFG0_PERIOD5_PERIOD_MASK 0xff
#define  WFG0_PERIOD5_PERIOD_SHIFT 0
#define  WFG0_PERIOD5_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD5_PERIOD_MASK) >> WFG0_PERIOD5_PERIOD_SHIFT)
#define  WFG0_PERIOD5_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD5_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD5_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 7 of 10) (period6)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG0_PERIOD6_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD6_RESERVED1_SHIFT 17
#define  WFG0_PERIOD6_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD6_RESERVED1_MASK) >> WFG0_PERIOD6_RESERVED1_SHIFT)
#define  WFG0_PERIOD6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD6_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD6_RESERVED1_SHIFT))

#define  WFG0_PERIOD6_IDLE_MASK 0x10000
#define  WFG0_PERIOD6_IDLE_SHIFT 16
#define  WFG0_PERIOD6_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD6_IDLE_MASK) >> WFG0_PERIOD6_IDLE_SHIFT)
#define  WFG0_PERIOD6_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD6_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD6_IDLE_SHIFT))

#define  WFG0_PERIOD6_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD6_ACTIVE_SHIFT 8
#define  WFG0_PERIOD6_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD6_ACTIVE_MASK) >> WFG0_PERIOD6_ACTIVE_SHIFT)
#define  WFG0_PERIOD6_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD6_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD6_ACTIVE_SHIFT))

#define  WFG0_PERIOD6_PERIOD_MASK 0xff
#define  WFG0_PERIOD6_PERIOD_SHIFT 0
#define  WFG0_PERIOD6_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD6_PERIOD_MASK) >> WFG0_PERIOD6_PERIOD_SHIFT)
#define  WFG0_PERIOD6_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD6_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD6_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 8 of 10) (period7)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG0_PERIOD7_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD7_RESERVED1_SHIFT 17
#define  WFG0_PERIOD7_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD7_RESERVED1_MASK) >> WFG0_PERIOD7_RESERVED1_SHIFT)
#define  WFG0_PERIOD7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD7_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD7_RESERVED1_SHIFT))

#define  WFG0_PERIOD7_IDLE_MASK 0x10000
#define  WFG0_PERIOD7_IDLE_SHIFT 16
#define  WFG0_PERIOD7_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD7_IDLE_MASK) >> WFG0_PERIOD7_IDLE_SHIFT)
#define  WFG0_PERIOD7_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD7_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD7_IDLE_SHIFT))

#define  WFG0_PERIOD7_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD7_ACTIVE_SHIFT 8
#define  WFG0_PERIOD7_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD7_ACTIVE_MASK) >> WFG0_PERIOD7_ACTIVE_SHIFT)
#define  WFG0_PERIOD7_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD7_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD7_ACTIVE_SHIFT))

#define  WFG0_PERIOD7_PERIOD_MASK 0xff
#define  WFG0_PERIOD7_PERIOD_SHIFT 0
#define  WFG0_PERIOD7_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD7_PERIOD_MASK) >> WFG0_PERIOD7_PERIOD_SHIFT)
#define  WFG0_PERIOD7_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD7_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD7_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 9 of 10) (period8)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG0_PERIOD8_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD8_RESERVED1_SHIFT 17
#define  WFG0_PERIOD8_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD8_RESERVED1_MASK) >> WFG0_PERIOD8_RESERVED1_SHIFT)
#define  WFG0_PERIOD8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD8_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD8_RESERVED1_SHIFT))

#define  WFG0_PERIOD8_IDLE_MASK 0x10000
#define  WFG0_PERIOD8_IDLE_SHIFT 16
#define  WFG0_PERIOD8_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD8_IDLE_MASK) >> WFG0_PERIOD8_IDLE_SHIFT)
#define  WFG0_PERIOD8_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD8_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD8_IDLE_SHIFT))

#define  WFG0_PERIOD8_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD8_ACTIVE_SHIFT 8
#define  WFG0_PERIOD8_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD8_ACTIVE_MASK) >> WFG0_PERIOD8_ACTIVE_SHIFT)
#define  WFG0_PERIOD8_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD8_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD8_ACTIVE_SHIFT))

#define  WFG0_PERIOD8_PERIOD_MASK 0xff
#define  WFG0_PERIOD8_PERIOD_SHIFT 0
#define  WFG0_PERIOD8_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD8_PERIOD_MASK) >> WFG0_PERIOD8_PERIOD_SHIFT)
#define  WFG0_PERIOD8_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD8_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD8_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 10 of 10) (period9)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG0_PERIOD9_RESERVED1_MASK 0xfffe0000
#define  WFG0_PERIOD9_RESERVED1_SHIFT 17
#define  WFG0_PERIOD9_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD9_RESERVED1_MASK) >> WFG0_PERIOD9_RESERVED1_SHIFT)
#define  WFG0_PERIOD9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD9_RESERVED1_MASK) | (((uint32_t)val) << WFG0_PERIOD9_RESERVED1_SHIFT))

#define  WFG0_PERIOD9_IDLE_MASK 0x10000
#define  WFG0_PERIOD9_IDLE_SHIFT 16
#define  WFG0_PERIOD9_IDLE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD9_IDLE_MASK) >> WFG0_PERIOD9_IDLE_SHIFT)
#define  WFG0_PERIOD9_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD9_IDLE_MASK) | (((uint32_t)val) << WFG0_PERIOD9_IDLE_SHIFT))

#define  WFG0_PERIOD9_ACTIVE_MASK 0xff00
#define  WFG0_PERIOD9_ACTIVE_SHIFT 8
#define  WFG0_PERIOD9_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD9_ACTIVE_MASK) >> WFG0_PERIOD9_ACTIVE_SHIFT)
#define  WFG0_PERIOD9_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD9_ACTIVE_MASK) | (((uint32_t)val) << WFG0_PERIOD9_ACTIVE_SHIFT))

#define  WFG0_PERIOD9_PERIOD_MASK 0xff
#define  WFG0_PERIOD9_PERIOD_SHIFT 0
#define  WFG0_PERIOD9_PERIOD_MASK_SHIFT(reg) (((reg) & WFG0_PERIOD9_PERIOD_MASK) >> WFG0_PERIOD9_PERIOD_SHIFT)
#define  WFG0_PERIOD9_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG0_PERIOD9_PERIOD_MASK) | (((uint32_t)val) << WFG0_PERIOD9_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 1 of 10) (action10)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG0_ACTION10_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION10_RESERVED1_SHIFT 30
#define  WFG0_ACTION10_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION10_RESERVED1_MASK) >> WFG0_ACTION10_RESERVED1_SHIFT)
#define  WFG0_ACTION10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION10_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION10_RESERVED1_SHIFT))

#define  WFG0_ACTION10_ACTION_MASK 0x30000000
#define  WFG0_ACTION10_ACTION_SHIFT 28
#define  WFG0_ACTION10_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION10_ACTION_MASK) >> WFG0_ACTION10_ACTION_SHIFT)
#define  WFG0_ACTION10_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION10_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION10_ACTION_SHIFT))

#define  WFG0_ACTION10_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION10_RESERVED2_SHIFT 24
#define  WFG0_ACTION10_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION10_RESERVED2_MASK) >> WFG0_ACTION10_RESERVED2_SHIFT)
#define  WFG0_ACTION10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION10_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION10_RESERVED2_SHIFT))

#define  WFG0_ACTION10_COUNT_MASK 0xffffff
#define  WFG0_ACTION10_COUNT_SHIFT 0
#define  WFG0_ACTION10_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION10_COUNT_MASK) >> WFG0_ACTION10_COUNT_SHIFT)
#define  WFG0_ACTION10_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION10_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION10_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 2 of 10) (action11)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG0_ACTION11_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION11_RESERVED1_SHIFT 30
#define  WFG0_ACTION11_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION11_RESERVED1_MASK) >> WFG0_ACTION11_RESERVED1_SHIFT)
#define  WFG0_ACTION11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION11_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION11_RESERVED1_SHIFT))

#define  WFG0_ACTION11_ACTION_MASK 0x30000000
#define  WFG0_ACTION11_ACTION_SHIFT 28
#define  WFG0_ACTION11_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION11_ACTION_MASK) >> WFG0_ACTION11_ACTION_SHIFT)
#define  WFG0_ACTION11_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION11_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION11_ACTION_SHIFT))

#define  WFG0_ACTION11_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION11_RESERVED2_SHIFT 24
#define  WFG0_ACTION11_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION11_RESERVED2_MASK) >> WFG0_ACTION11_RESERVED2_SHIFT)
#define  WFG0_ACTION11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION11_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION11_RESERVED2_SHIFT))

#define  WFG0_ACTION11_COUNT_MASK 0xffffff
#define  WFG0_ACTION11_COUNT_SHIFT 0
#define  WFG0_ACTION11_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION11_COUNT_MASK) >> WFG0_ACTION11_COUNT_SHIFT)
#define  WFG0_ACTION11_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION11_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION11_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 3 of 10) (action12)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG0_ACTION12_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION12_RESERVED1_SHIFT 30
#define  WFG0_ACTION12_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION12_RESERVED1_MASK) >> WFG0_ACTION12_RESERVED1_SHIFT)
#define  WFG0_ACTION12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION12_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION12_RESERVED1_SHIFT))

#define  WFG0_ACTION12_ACTION_MASK 0x30000000
#define  WFG0_ACTION12_ACTION_SHIFT 28
#define  WFG0_ACTION12_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION12_ACTION_MASK) >> WFG0_ACTION12_ACTION_SHIFT)
#define  WFG0_ACTION12_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION12_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION12_ACTION_SHIFT))

#define  WFG0_ACTION12_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION12_RESERVED2_SHIFT 24
#define  WFG0_ACTION12_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION12_RESERVED2_MASK) >> WFG0_ACTION12_RESERVED2_SHIFT)
#define  WFG0_ACTION12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION12_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION12_RESERVED2_SHIFT))

#define  WFG0_ACTION12_COUNT_MASK 0xffffff
#define  WFG0_ACTION12_COUNT_SHIFT 0
#define  WFG0_ACTION12_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION12_COUNT_MASK) >> WFG0_ACTION12_COUNT_SHIFT)
#define  WFG0_ACTION12_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION12_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION12_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 4 of 10) (action13)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG0_ACTION13_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION13_RESERVED1_SHIFT 30
#define  WFG0_ACTION13_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION13_RESERVED1_MASK) >> WFG0_ACTION13_RESERVED1_SHIFT)
#define  WFG0_ACTION13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION13_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION13_RESERVED1_SHIFT))

#define  WFG0_ACTION13_ACTION_MASK 0x30000000
#define  WFG0_ACTION13_ACTION_SHIFT 28
#define  WFG0_ACTION13_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION13_ACTION_MASK) >> WFG0_ACTION13_ACTION_SHIFT)
#define  WFG0_ACTION13_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION13_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION13_ACTION_SHIFT))

#define  WFG0_ACTION13_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION13_RESERVED2_SHIFT 24
#define  WFG0_ACTION13_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION13_RESERVED2_MASK) >> WFG0_ACTION13_RESERVED2_SHIFT)
#define  WFG0_ACTION13_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION13_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION13_RESERVED2_SHIFT))

#define  WFG0_ACTION13_COUNT_MASK 0xffffff
#define  WFG0_ACTION13_COUNT_SHIFT 0
#define  WFG0_ACTION13_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION13_COUNT_MASK) >> WFG0_ACTION13_COUNT_SHIFT)
#define  WFG0_ACTION13_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION13_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION13_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 5 of 10) (action14)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG0_ACTION14_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION14_RESERVED1_SHIFT 30
#define  WFG0_ACTION14_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION14_RESERVED1_MASK) >> WFG0_ACTION14_RESERVED1_SHIFT)
#define  WFG0_ACTION14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION14_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION14_RESERVED1_SHIFT))

#define  WFG0_ACTION14_ACTION_MASK 0x30000000
#define  WFG0_ACTION14_ACTION_SHIFT 28
#define  WFG0_ACTION14_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION14_ACTION_MASK) >> WFG0_ACTION14_ACTION_SHIFT)
#define  WFG0_ACTION14_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION14_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION14_ACTION_SHIFT))

#define  WFG0_ACTION14_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION14_RESERVED2_SHIFT 24
#define  WFG0_ACTION14_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION14_RESERVED2_MASK) >> WFG0_ACTION14_RESERVED2_SHIFT)
#define  WFG0_ACTION14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION14_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION14_RESERVED2_SHIFT))

#define  WFG0_ACTION14_COUNT_MASK 0xffffff
#define  WFG0_ACTION14_COUNT_SHIFT 0
#define  WFG0_ACTION14_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION14_COUNT_MASK) >> WFG0_ACTION14_COUNT_SHIFT)
#define  WFG0_ACTION14_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION14_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION14_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 6 of 10) (action15)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG0_ACTION15_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION15_RESERVED1_SHIFT 30
#define  WFG0_ACTION15_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION15_RESERVED1_MASK) >> WFG0_ACTION15_RESERVED1_SHIFT)
#define  WFG0_ACTION15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION15_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION15_RESERVED1_SHIFT))

#define  WFG0_ACTION15_ACTION_MASK 0x30000000
#define  WFG0_ACTION15_ACTION_SHIFT 28
#define  WFG0_ACTION15_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION15_ACTION_MASK) >> WFG0_ACTION15_ACTION_SHIFT)
#define  WFG0_ACTION15_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION15_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION15_ACTION_SHIFT))

#define  WFG0_ACTION15_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION15_RESERVED2_SHIFT 24
#define  WFG0_ACTION15_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION15_RESERVED2_MASK) >> WFG0_ACTION15_RESERVED2_SHIFT)
#define  WFG0_ACTION15_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION15_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION15_RESERVED2_SHIFT))

#define  WFG0_ACTION15_COUNT_MASK 0xffffff
#define  WFG0_ACTION15_COUNT_SHIFT 0
#define  WFG0_ACTION15_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION15_COUNT_MASK) >> WFG0_ACTION15_COUNT_SHIFT)
#define  WFG0_ACTION15_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION15_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION15_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 7 of 10) (action16)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG0_ACTION16_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION16_RESERVED1_SHIFT 30
#define  WFG0_ACTION16_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION16_RESERVED1_MASK) >> WFG0_ACTION16_RESERVED1_SHIFT)
#define  WFG0_ACTION16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION16_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION16_RESERVED1_SHIFT))

#define  WFG0_ACTION16_ACTION_MASK 0x30000000
#define  WFG0_ACTION16_ACTION_SHIFT 28
#define  WFG0_ACTION16_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION16_ACTION_MASK) >> WFG0_ACTION16_ACTION_SHIFT)
#define  WFG0_ACTION16_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION16_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION16_ACTION_SHIFT))

#define  WFG0_ACTION16_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION16_RESERVED2_SHIFT 24
#define  WFG0_ACTION16_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION16_RESERVED2_MASK) >> WFG0_ACTION16_RESERVED2_SHIFT)
#define  WFG0_ACTION16_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION16_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION16_RESERVED2_SHIFT))

#define  WFG0_ACTION16_COUNT_MASK 0xffffff
#define  WFG0_ACTION16_COUNT_SHIFT 0
#define  WFG0_ACTION16_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION16_COUNT_MASK) >> WFG0_ACTION16_COUNT_SHIFT)
#define  WFG0_ACTION16_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION16_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION16_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 8 of 10) (action17)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG0_ACTION17_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION17_RESERVED1_SHIFT 30
#define  WFG0_ACTION17_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION17_RESERVED1_MASK) >> WFG0_ACTION17_RESERVED1_SHIFT)
#define  WFG0_ACTION17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION17_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION17_RESERVED1_SHIFT))

#define  WFG0_ACTION17_ACTION_MASK 0x30000000
#define  WFG0_ACTION17_ACTION_SHIFT 28
#define  WFG0_ACTION17_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION17_ACTION_MASK) >> WFG0_ACTION17_ACTION_SHIFT)
#define  WFG0_ACTION17_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION17_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION17_ACTION_SHIFT))

#define  WFG0_ACTION17_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION17_RESERVED2_SHIFT 24
#define  WFG0_ACTION17_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION17_RESERVED2_MASK) >> WFG0_ACTION17_RESERVED2_SHIFT)
#define  WFG0_ACTION17_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION17_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION17_RESERVED2_SHIFT))

#define  WFG0_ACTION17_COUNT_MASK 0xffffff
#define  WFG0_ACTION17_COUNT_SHIFT 0
#define  WFG0_ACTION17_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION17_COUNT_MASK) >> WFG0_ACTION17_COUNT_SHIFT)
#define  WFG0_ACTION17_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION17_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION17_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 9 of 10) (action18)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG0_ACTION18_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION18_RESERVED1_SHIFT 30
#define  WFG0_ACTION18_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION18_RESERVED1_MASK) >> WFG0_ACTION18_RESERVED1_SHIFT)
#define  WFG0_ACTION18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION18_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION18_RESERVED1_SHIFT))

#define  WFG0_ACTION18_ACTION_MASK 0x30000000
#define  WFG0_ACTION18_ACTION_SHIFT 28
#define  WFG0_ACTION18_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION18_ACTION_MASK) >> WFG0_ACTION18_ACTION_SHIFT)
#define  WFG0_ACTION18_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION18_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION18_ACTION_SHIFT))

#define  WFG0_ACTION18_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION18_RESERVED2_SHIFT 24
#define  WFG0_ACTION18_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION18_RESERVED2_MASK) >> WFG0_ACTION18_RESERVED2_SHIFT)
#define  WFG0_ACTION18_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION18_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION18_RESERVED2_SHIFT))

#define  WFG0_ACTION18_COUNT_MASK 0xffffff
#define  WFG0_ACTION18_COUNT_SHIFT 0
#define  WFG0_ACTION18_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION18_COUNT_MASK) >> WFG0_ACTION18_COUNT_SHIFT)
#define  WFG0_ACTION18_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION18_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION18_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 10 of 10) (action19)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG0_ACTION19_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION19_RESERVED1_SHIFT 30
#define  WFG0_ACTION19_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION19_RESERVED1_MASK) >> WFG0_ACTION19_RESERVED1_SHIFT)
#define  WFG0_ACTION19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION19_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION19_RESERVED1_SHIFT))

#define  WFG0_ACTION19_ACTION_MASK 0x30000000
#define  WFG0_ACTION19_ACTION_SHIFT 28
#define  WFG0_ACTION19_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION19_ACTION_MASK) >> WFG0_ACTION19_ACTION_SHIFT)
#define  WFG0_ACTION19_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION19_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION19_ACTION_SHIFT))

#define  WFG0_ACTION19_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION19_RESERVED2_SHIFT 24
#define  WFG0_ACTION19_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION19_RESERVED2_MASK) >> WFG0_ACTION19_RESERVED2_SHIFT)
#define  WFG0_ACTION19_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION19_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION19_RESERVED2_SHIFT))

#define  WFG0_ACTION19_COUNT_MASK 0xffffff
#define  WFG0_ACTION19_COUNT_SHIFT 0
#define  WFG0_ACTION19_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION19_COUNT_MASK) >> WFG0_ACTION19_COUNT_SHIFT)
#define  WFG0_ACTION19_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION19_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION19_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 1 of 10) (action20)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG0_ACTION20_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION20_RESERVED1_SHIFT 30
#define  WFG0_ACTION20_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION20_RESERVED1_MASK) >> WFG0_ACTION20_RESERVED1_SHIFT)
#define  WFG0_ACTION20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION20_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION20_RESERVED1_SHIFT))

#define  WFG0_ACTION20_ACTION_MASK 0x30000000
#define  WFG0_ACTION20_ACTION_SHIFT 28
#define  WFG0_ACTION20_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION20_ACTION_MASK) >> WFG0_ACTION20_ACTION_SHIFT)
#define  WFG0_ACTION20_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION20_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION20_ACTION_SHIFT))

#define  WFG0_ACTION20_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION20_RESERVED2_SHIFT 24
#define  WFG0_ACTION20_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION20_RESERVED2_MASK) >> WFG0_ACTION20_RESERVED2_SHIFT)
#define  WFG0_ACTION20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION20_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION20_RESERVED2_SHIFT))

#define  WFG0_ACTION20_COUNT_MASK 0xffffff
#define  WFG0_ACTION20_COUNT_SHIFT 0
#define  WFG0_ACTION20_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION20_COUNT_MASK) >> WFG0_ACTION20_COUNT_SHIFT)
#define  WFG0_ACTION20_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION20_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION20_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 2 of 10) (action21)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG0_ACTION21_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION21_RESERVED1_SHIFT 30
#define  WFG0_ACTION21_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION21_RESERVED1_MASK) >> WFG0_ACTION21_RESERVED1_SHIFT)
#define  WFG0_ACTION21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION21_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION21_RESERVED1_SHIFT))

#define  WFG0_ACTION21_ACTION_MASK 0x30000000
#define  WFG0_ACTION21_ACTION_SHIFT 28
#define  WFG0_ACTION21_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION21_ACTION_MASK) >> WFG0_ACTION21_ACTION_SHIFT)
#define  WFG0_ACTION21_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION21_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION21_ACTION_SHIFT))

#define  WFG0_ACTION21_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION21_RESERVED2_SHIFT 24
#define  WFG0_ACTION21_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION21_RESERVED2_MASK) >> WFG0_ACTION21_RESERVED2_SHIFT)
#define  WFG0_ACTION21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION21_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION21_RESERVED2_SHIFT))

#define  WFG0_ACTION21_COUNT_MASK 0xffffff
#define  WFG0_ACTION21_COUNT_SHIFT 0
#define  WFG0_ACTION21_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION21_COUNT_MASK) >> WFG0_ACTION21_COUNT_SHIFT)
#define  WFG0_ACTION21_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION21_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION21_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 3 of 10) (action22)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG0_ACTION22_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION22_RESERVED1_SHIFT 30
#define  WFG0_ACTION22_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION22_RESERVED1_MASK) >> WFG0_ACTION22_RESERVED1_SHIFT)
#define  WFG0_ACTION22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION22_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION22_RESERVED1_SHIFT))

#define  WFG0_ACTION22_ACTION_MASK 0x30000000
#define  WFG0_ACTION22_ACTION_SHIFT 28
#define  WFG0_ACTION22_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION22_ACTION_MASK) >> WFG0_ACTION22_ACTION_SHIFT)
#define  WFG0_ACTION22_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION22_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION22_ACTION_SHIFT))

#define  WFG0_ACTION22_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION22_RESERVED2_SHIFT 24
#define  WFG0_ACTION22_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION22_RESERVED2_MASK) >> WFG0_ACTION22_RESERVED2_SHIFT)
#define  WFG0_ACTION22_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION22_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION22_RESERVED2_SHIFT))

#define  WFG0_ACTION22_COUNT_MASK 0xffffff
#define  WFG0_ACTION22_COUNT_SHIFT 0
#define  WFG0_ACTION22_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION22_COUNT_MASK) >> WFG0_ACTION22_COUNT_SHIFT)
#define  WFG0_ACTION22_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION22_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION22_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 4 of 10) (action23)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG0_ACTION23_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION23_RESERVED1_SHIFT 30
#define  WFG0_ACTION23_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION23_RESERVED1_MASK) >> WFG0_ACTION23_RESERVED1_SHIFT)
#define  WFG0_ACTION23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION23_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION23_RESERVED1_SHIFT))

#define  WFG0_ACTION23_ACTION_MASK 0x30000000
#define  WFG0_ACTION23_ACTION_SHIFT 28
#define  WFG0_ACTION23_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION23_ACTION_MASK) >> WFG0_ACTION23_ACTION_SHIFT)
#define  WFG0_ACTION23_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION23_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION23_ACTION_SHIFT))

#define  WFG0_ACTION23_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION23_RESERVED2_SHIFT 24
#define  WFG0_ACTION23_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION23_RESERVED2_MASK) >> WFG0_ACTION23_RESERVED2_SHIFT)
#define  WFG0_ACTION23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION23_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION23_RESERVED2_SHIFT))

#define  WFG0_ACTION23_COUNT_MASK 0xffffff
#define  WFG0_ACTION23_COUNT_SHIFT 0
#define  WFG0_ACTION23_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION23_COUNT_MASK) >> WFG0_ACTION23_COUNT_SHIFT)
#define  WFG0_ACTION23_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION23_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION23_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 5 of 10) (action24)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG0_ACTION24_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION24_RESERVED1_SHIFT 30
#define  WFG0_ACTION24_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION24_RESERVED1_MASK) >> WFG0_ACTION24_RESERVED1_SHIFT)
#define  WFG0_ACTION24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION24_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION24_RESERVED1_SHIFT))

#define  WFG0_ACTION24_ACTION_MASK 0x30000000
#define  WFG0_ACTION24_ACTION_SHIFT 28
#define  WFG0_ACTION24_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION24_ACTION_MASK) >> WFG0_ACTION24_ACTION_SHIFT)
#define  WFG0_ACTION24_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION24_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION24_ACTION_SHIFT))

#define  WFG0_ACTION24_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION24_RESERVED2_SHIFT 24
#define  WFG0_ACTION24_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION24_RESERVED2_MASK) >> WFG0_ACTION24_RESERVED2_SHIFT)
#define  WFG0_ACTION24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION24_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION24_RESERVED2_SHIFT))

#define  WFG0_ACTION24_COUNT_MASK 0xffffff
#define  WFG0_ACTION24_COUNT_SHIFT 0
#define  WFG0_ACTION24_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION24_COUNT_MASK) >> WFG0_ACTION24_COUNT_SHIFT)
#define  WFG0_ACTION24_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION24_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION24_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 6 of 10) (action25)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG0_ACTION25_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION25_RESERVED1_SHIFT 30
#define  WFG0_ACTION25_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION25_RESERVED1_MASK) >> WFG0_ACTION25_RESERVED1_SHIFT)
#define  WFG0_ACTION25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION25_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION25_RESERVED1_SHIFT))

#define  WFG0_ACTION25_ACTION_MASK 0x30000000
#define  WFG0_ACTION25_ACTION_SHIFT 28
#define  WFG0_ACTION25_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION25_ACTION_MASK) >> WFG0_ACTION25_ACTION_SHIFT)
#define  WFG0_ACTION25_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION25_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION25_ACTION_SHIFT))

#define  WFG0_ACTION25_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION25_RESERVED2_SHIFT 24
#define  WFG0_ACTION25_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION25_RESERVED2_MASK) >> WFG0_ACTION25_RESERVED2_SHIFT)
#define  WFG0_ACTION25_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION25_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION25_RESERVED2_SHIFT))

#define  WFG0_ACTION25_COUNT_MASK 0xffffff
#define  WFG0_ACTION25_COUNT_SHIFT 0
#define  WFG0_ACTION25_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION25_COUNT_MASK) >> WFG0_ACTION25_COUNT_SHIFT)
#define  WFG0_ACTION25_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION25_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION25_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 7 of 10) (action26)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG0_ACTION26_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION26_RESERVED1_SHIFT 30
#define  WFG0_ACTION26_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION26_RESERVED1_MASK) >> WFG0_ACTION26_RESERVED1_SHIFT)
#define  WFG0_ACTION26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION26_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION26_RESERVED1_SHIFT))

#define  WFG0_ACTION26_ACTION_MASK 0x30000000
#define  WFG0_ACTION26_ACTION_SHIFT 28
#define  WFG0_ACTION26_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION26_ACTION_MASK) >> WFG0_ACTION26_ACTION_SHIFT)
#define  WFG0_ACTION26_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION26_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION26_ACTION_SHIFT))

#define  WFG0_ACTION26_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION26_RESERVED2_SHIFT 24
#define  WFG0_ACTION26_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION26_RESERVED2_MASK) >> WFG0_ACTION26_RESERVED2_SHIFT)
#define  WFG0_ACTION26_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION26_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION26_RESERVED2_SHIFT))

#define  WFG0_ACTION26_COUNT_MASK 0xffffff
#define  WFG0_ACTION26_COUNT_SHIFT 0
#define  WFG0_ACTION26_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION26_COUNT_MASK) >> WFG0_ACTION26_COUNT_SHIFT)
#define  WFG0_ACTION26_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION26_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION26_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 8 of 10) (action27)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG0_ACTION27_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION27_RESERVED1_SHIFT 30
#define  WFG0_ACTION27_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION27_RESERVED1_MASK) >> WFG0_ACTION27_RESERVED1_SHIFT)
#define  WFG0_ACTION27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION27_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION27_RESERVED1_SHIFT))

#define  WFG0_ACTION27_ACTION_MASK 0x30000000
#define  WFG0_ACTION27_ACTION_SHIFT 28
#define  WFG0_ACTION27_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION27_ACTION_MASK) >> WFG0_ACTION27_ACTION_SHIFT)
#define  WFG0_ACTION27_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION27_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION27_ACTION_SHIFT))

#define  WFG0_ACTION27_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION27_RESERVED2_SHIFT 24
#define  WFG0_ACTION27_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION27_RESERVED2_MASK) >> WFG0_ACTION27_RESERVED2_SHIFT)
#define  WFG0_ACTION27_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION27_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION27_RESERVED2_SHIFT))

#define  WFG0_ACTION27_COUNT_MASK 0xffffff
#define  WFG0_ACTION27_COUNT_SHIFT 0
#define  WFG0_ACTION27_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION27_COUNT_MASK) >> WFG0_ACTION27_COUNT_SHIFT)
#define  WFG0_ACTION27_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION27_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION27_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 9 of 10) (action28)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG0_ACTION28_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION28_RESERVED1_SHIFT 30
#define  WFG0_ACTION28_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION28_RESERVED1_MASK) >> WFG0_ACTION28_RESERVED1_SHIFT)
#define  WFG0_ACTION28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION28_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION28_RESERVED1_SHIFT))

#define  WFG0_ACTION28_ACTION_MASK 0x30000000
#define  WFG0_ACTION28_ACTION_SHIFT 28
#define  WFG0_ACTION28_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION28_ACTION_MASK) >> WFG0_ACTION28_ACTION_SHIFT)
#define  WFG0_ACTION28_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION28_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION28_ACTION_SHIFT))

#define  WFG0_ACTION28_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION28_RESERVED2_SHIFT 24
#define  WFG0_ACTION28_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION28_RESERVED2_MASK) >> WFG0_ACTION28_RESERVED2_SHIFT)
#define  WFG0_ACTION28_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION28_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION28_RESERVED2_SHIFT))

#define  WFG0_ACTION28_COUNT_MASK 0xffffff
#define  WFG0_ACTION28_COUNT_SHIFT 0
#define  WFG0_ACTION28_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION28_COUNT_MASK) >> WFG0_ACTION28_COUNT_SHIFT)
#define  WFG0_ACTION28_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION28_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION28_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 10 of 10) (action29)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG0_ACTION29_RESERVED1_MASK 0xc0000000
#define  WFG0_ACTION29_RESERVED1_SHIFT 30
#define  WFG0_ACTION29_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG0_ACTION29_RESERVED1_MASK) >> WFG0_ACTION29_RESERVED1_SHIFT)
#define  WFG0_ACTION29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION29_RESERVED1_MASK) | (((uint32_t)val) << WFG0_ACTION29_RESERVED1_SHIFT))

#define  WFG0_ACTION29_ACTION_MASK 0x30000000
#define  WFG0_ACTION29_ACTION_SHIFT 28
#define  WFG0_ACTION29_ACTION_MASK_SHIFT(reg) (((reg) & WFG0_ACTION29_ACTION_MASK) >> WFG0_ACTION29_ACTION_SHIFT)
#define  WFG0_ACTION29_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION29_ACTION_MASK) | (((uint32_t)val) << WFG0_ACTION29_ACTION_SHIFT))

#define  WFG0_ACTION29_RESERVED2_MASK 0xf000000
#define  WFG0_ACTION29_RESERVED2_SHIFT 24
#define  WFG0_ACTION29_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG0_ACTION29_RESERVED2_MASK) >> WFG0_ACTION29_RESERVED2_SHIFT)
#define  WFG0_ACTION29_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION29_RESERVED2_MASK) | (((uint32_t)val) << WFG0_ACTION29_RESERVED2_SHIFT))

#define  WFG0_ACTION29_COUNT_MASK 0xffffff
#define  WFG0_ACTION29_COUNT_SHIFT 0
#define  WFG0_ACTION29_COUNT_MASK_SHIFT(reg) (((reg) & WFG0_ACTION29_COUNT_MASK) >> WFG0_ACTION29_COUNT_SHIFT)
#define  WFG0_ACTION29_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG0_ACTION29_COUNT_MASK) | (((uint32_t)val) << WFG0_ACTION29_COUNT_SHIFT))

//====================================================================
//
//Register File: Waverform Generator Control (Instance 2 of 4) (wfg1)
//
//====================================================================

//====================================================================
//Register: Waveform Generator Configuration register (scfg)
//====================================================================

#define  WFG1_SCFG_MODE_MASK 0xc0000000
#define  WFG1_SCFG_MODE_SHIFT 30
#define  WFG1_SCFG_MODE_MASK_SHIFT(reg) (((reg) & WFG1_SCFG_MODE_MASK) >> WFG1_SCFG_MODE_SHIFT)
#define  WFG1_SCFG_MODE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_SCFG_MODE_MASK) | (((uint32_t)val) << WFG1_SCFG_MODE_SHIFT))

#define  WFG1_SCFG_RECOV_MASK 0x20000000
#define  WFG1_SCFG_RECOV_SHIFT 29
#define  WFG1_SCFG_RECOV_MASK_SHIFT(reg) (((reg) & WFG1_SCFG_RECOV_MASK) >> WFG1_SCFG_RECOV_SHIFT)
#define  WFG1_SCFG_RECOV_REPLACE_VAL(reg,val) (((reg) & ~WFG1_SCFG_RECOV_MASK) | (((uint32_t)val) << WFG1_SCFG_RECOV_SHIFT))

#define  WFG1_SCFG_SYNC_SEL_MASK 0x10000000
#define  WFG1_SCFG_SYNC_SEL_SHIFT 28
#define  WFG1_SCFG_SYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG1_SCFG_SYNC_SEL_MASK) >> WFG1_SCFG_SYNC_SEL_SHIFT)
#define  WFG1_SCFG_SYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG1_SCFG_SYNC_SEL_MASK) | (((uint32_t)val) << WFG1_SCFG_SYNC_SEL_SHIFT))

#define  WFG1_SCFG_GALVO_SEL_MASK 0xc000000
#define  WFG1_SCFG_GALVO_SEL_SHIFT 26
#define  WFG1_SCFG_GALVO_SEL_MASK_SHIFT(reg) (((reg) & WFG1_SCFG_GALVO_SEL_MASK) >> WFG1_SCFG_GALVO_SEL_SHIFT)
#define  WFG1_SCFG_GALVO_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG1_SCFG_GALVO_SEL_MASK) | (((uint32_t)val) << WFG1_SCFG_GALVO_SEL_SHIFT))

#define  WFG1_SCFG_HSYNC_SEL_MASK 0x3000000
#define  WFG1_SCFG_HSYNC_SEL_SHIFT 24
#define  WFG1_SCFG_HSYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG1_SCFG_HSYNC_SEL_MASK) >> WFG1_SCFG_HSYNC_SEL_SHIFT)
#define  WFG1_SCFG_HSYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG1_SCFG_HSYNC_SEL_MASK) | (((uint32_t)val) << WFG1_SCFG_HSYNC_SEL_SHIFT))

#define  WFG1_SCFG_COUNT_MASK 0xffffff
#define  WFG1_SCFG_COUNT_SHIFT 0
#define  WFG1_SCFG_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_SCFG_COUNT_MASK) >> WFG1_SCFG_COUNT_SHIFT)
#define  WFG1_SCFG_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_SCFG_COUNT_MASK) | (((uint32_t)val) << WFG1_SCFG_COUNT_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 1 of 10) (period0)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG1_PERIOD0_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD0_RESERVED1_SHIFT 17
#define  WFG1_PERIOD0_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD0_RESERVED1_MASK) >> WFG1_PERIOD0_RESERVED1_SHIFT)
#define  WFG1_PERIOD0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD0_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD0_RESERVED1_SHIFT))

#define  WFG1_PERIOD0_IDLE_MASK 0x10000
#define  WFG1_PERIOD0_IDLE_SHIFT 16
#define  WFG1_PERIOD0_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD0_IDLE_MASK) >> WFG1_PERIOD0_IDLE_SHIFT)
#define  WFG1_PERIOD0_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD0_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD0_IDLE_SHIFT))

#define  WFG1_PERIOD0_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD0_ACTIVE_SHIFT 8
#define  WFG1_PERIOD0_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD0_ACTIVE_MASK) >> WFG1_PERIOD0_ACTIVE_SHIFT)
#define  WFG1_PERIOD0_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD0_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD0_ACTIVE_SHIFT))

#define  WFG1_PERIOD0_PERIOD_MASK 0xff
#define  WFG1_PERIOD0_PERIOD_SHIFT 0
#define  WFG1_PERIOD0_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD0_PERIOD_MASK) >> WFG1_PERIOD0_PERIOD_SHIFT)
#define  WFG1_PERIOD0_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD0_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD0_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 2 of 10) (period1)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG1_PERIOD1_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD1_RESERVED1_SHIFT 17
#define  WFG1_PERIOD1_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD1_RESERVED1_MASK) >> WFG1_PERIOD1_RESERVED1_SHIFT)
#define  WFG1_PERIOD1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD1_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD1_RESERVED1_SHIFT))

#define  WFG1_PERIOD1_IDLE_MASK 0x10000
#define  WFG1_PERIOD1_IDLE_SHIFT 16
#define  WFG1_PERIOD1_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD1_IDLE_MASK) >> WFG1_PERIOD1_IDLE_SHIFT)
#define  WFG1_PERIOD1_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD1_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD1_IDLE_SHIFT))

#define  WFG1_PERIOD1_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD1_ACTIVE_SHIFT 8
#define  WFG1_PERIOD1_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD1_ACTIVE_MASK) >> WFG1_PERIOD1_ACTIVE_SHIFT)
#define  WFG1_PERIOD1_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD1_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD1_ACTIVE_SHIFT))

#define  WFG1_PERIOD1_PERIOD_MASK 0xff
#define  WFG1_PERIOD1_PERIOD_SHIFT 0
#define  WFG1_PERIOD1_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD1_PERIOD_MASK) >> WFG1_PERIOD1_PERIOD_SHIFT)
#define  WFG1_PERIOD1_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD1_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD1_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 3 of 10) (period2)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG1_PERIOD2_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD2_RESERVED1_SHIFT 17
#define  WFG1_PERIOD2_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD2_RESERVED1_MASK) >> WFG1_PERIOD2_RESERVED1_SHIFT)
#define  WFG1_PERIOD2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD2_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD2_RESERVED1_SHIFT))

#define  WFG1_PERIOD2_IDLE_MASK 0x10000
#define  WFG1_PERIOD2_IDLE_SHIFT 16
#define  WFG1_PERIOD2_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD2_IDLE_MASK) >> WFG1_PERIOD2_IDLE_SHIFT)
#define  WFG1_PERIOD2_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD2_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD2_IDLE_SHIFT))

#define  WFG1_PERIOD2_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD2_ACTIVE_SHIFT 8
#define  WFG1_PERIOD2_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD2_ACTIVE_MASK) >> WFG1_PERIOD2_ACTIVE_SHIFT)
#define  WFG1_PERIOD2_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD2_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD2_ACTIVE_SHIFT))

#define  WFG1_PERIOD2_PERIOD_MASK 0xff
#define  WFG1_PERIOD2_PERIOD_SHIFT 0
#define  WFG1_PERIOD2_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD2_PERIOD_MASK) >> WFG1_PERIOD2_PERIOD_SHIFT)
#define  WFG1_PERIOD2_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD2_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD2_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 4 of 10) (period3)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG1_PERIOD3_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD3_RESERVED1_SHIFT 17
#define  WFG1_PERIOD3_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD3_RESERVED1_MASK) >> WFG1_PERIOD3_RESERVED1_SHIFT)
#define  WFG1_PERIOD3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD3_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD3_RESERVED1_SHIFT))

#define  WFG1_PERIOD3_IDLE_MASK 0x10000
#define  WFG1_PERIOD3_IDLE_SHIFT 16
#define  WFG1_PERIOD3_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD3_IDLE_MASK) >> WFG1_PERIOD3_IDLE_SHIFT)
#define  WFG1_PERIOD3_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD3_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD3_IDLE_SHIFT))

#define  WFG1_PERIOD3_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD3_ACTIVE_SHIFT 8
#define  WFG1_PERIOD3_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD3_ACTIVE_MASK) >> WFG1_PERIOD3_ACTIVE_SHIFT)
#define  WFG1_PERIOD3_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD3_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD3_ACTIVE_SHIFT))

#define  WFG1_PERIOD3_PERIOD_MASK 0xff
#define  WFG1_PERIOD3_PERIOD_SHIFT 0
#define  WFG1_PERIOD3_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD3_PERIOD_MASK) >> WFG1_PERIOD3_PERIOD_SHIFT)
#define  WFG1_PERIOD3_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD3_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD3_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 5 of 10) (period4)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG1_PERIOD4_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD4_RESERVED1_SHIFT 17
#define  WFG1_PERIOD4_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD4_RESERVED1_MASK) >> WFG1_PERIOD4_RESERVED1_SHIFT)
#define  WFG1_PERIOD4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD4_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD4_RESERVED1_SHIFT))

#define  WFG1_PERIOD4_IDLE_MASK 0x10000
#define  WFG1_PERIOD4_IDLE_SHIFT 16
#define  WFG1_PERIOD4_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD4_IDLE_MASK) >> WFG1_PERIOD4_IDLE_SHIFT)
#define  WFG1_PERIOD4_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD4_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD4_IDLE_SHIFT))

#define  WFG1_PERIOD4_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD4_ACTIVE_SHIFT 8
#define  WFG1_PERIOD4_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD4_ACTIVE_MASK) >> WFG1_PERIOD4_ACTIVE_SHIFT)
#define  WFG1_PERIOD4_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD4_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD4_ACTIVE_SHIFT))

#define  WFG1_PERIOD4_PERIOD_MASK 0xff
#define  WFG1_PERIOD4_PERIOD_SHIFT 0
#define  WFG1_PERIOD4_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD4_PERIOD_MASK) >> WFG1_PERIOD4_PERIOD_SHIFT)
#define  WFG1_PERIOD4_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD4_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD4_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 6 of 10) (period5)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG1_PERIOD5_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD5_RESERVED1_SHIFT 17
#define  WFG1_PERIOD5_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD5_RESERVED1_MASK) >> WFG1_PERIOD5_RESERVED1_SHIFT)
#define  WFG1_PERIOD5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD5_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD5_RESERVED1_SHIFT))

#define  WFG1_PERIOD5_IDLE_MASK 0x10000
#define  WFG1_PERIOD5_IDLE_SHIFT 16
#define  WFG1_PERIOD5_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD5_IDLE_MASK) >> WFG1_PERIOD5_IDLE_SHIFT)
#define  WFG1_PERIOD5_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD5_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD5_IDLE_SHIFT))

#define  WFG1_PERIOD5_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD5_ACTIVE_SHIFT 8
#define  WFG1_PERIOD5_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD5_ACTIVE_MASK) >> WFG1_PERIOD5_ACTIVE_SHIFT)
#define  WFG1_PERIOD5_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD5_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD5_ACTIVE_SHIFT))

#define  WFG1_PERIOD5_PERIOD_MASK 0xff
#define  WFG1_PERIOD5_PERIOD_SHIFT 0
#define  WFG1_PERIOD5_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD5_PERIOD_MASK) >> WFG1_PERIOD5_PERIOD_SHIFT)
#define  WFG1_PERIOD5_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD5_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD5_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 7 of 10) (period6)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG1_PERIOD6_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD6_RESERVED1_SHIFT 17
#define  WFG1_PERIOD6_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD6_RESERVED1_MASK) >> WFG1_PERIOD6_RESERVED1_SHIFT)
#define  WFG1_PERIOD6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD6_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD6_RESERVED1_SHIFT))

#define  WFG1_PERIOD6_IDLE_MASK 0x10000
#define  WFG1_PERIOD6_IDLE_SHIFT 16
#define  WFG1_PERIOD6_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD6_IDLE_MASK) >> WFG1_PERIOD6_IDLE_SHIFT)
#define  WFG1_PERIOD6_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD6_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD6_IDLE_SHIFT))

#define  WFG1_PERIOD6_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD6_ACTIVE_SHIFT 8
#define  WFG1_PERIOD6_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD6_ACTIVE_MASK) >> WFG1_PERIOD6_ACTIVE_SHIFT)
#define  WFG1_PERIOD6_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD6_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD6_ACTIVE_SHIFT))

#define  WFG1_PERIOD6_PERIOD_MASK 0xff
#define  WFG1_PERIOD6_PERIOD_SHIFT 0
#define  WFG1_PERIOD6_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD6_PERIOD_MASK) >> WFG1_PERIOD6_PERIOD_SHIFT)
#define  WFG1_PERIOD6_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD6_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD6_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 8 of 10) (period7)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG1_PERIOD7_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD7_RESERVED1_SHIFT 17
#define  WFG1_PERIOD7_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD7_RESERVED1_MASK) >> WFG1_PERIOD7_RESERVED1_SHIFT)
#define  WFG1_PERIOD7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD7_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD7_RESERVED1_SHIFT))

#define  WFG1_PERIOD7_IDLE_MASK 0x10000
#define  WFG1_PERIOD7_IDLE_SHIFT 16
#define  WFG1_PERIOD7_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD7_IDLE_MASK) >> WFG1_PERIOD7_IDLE_SHIFT)
#define  WFG1_PERIOD7_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD7_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD7_IDLE_SHIFT))

#define  WFG1_PERIOD7_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD7_ACTIVE_SHIFT 8
#define  WFG1_PERIOD7_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD7_ACTIVE_MASK) >> WFG1_PERIOD7_ACTIVE_SHIFT)
#define  WFG1_PERIOD7_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD7_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD7_ACTIVE_SHIFT))

#define  WFG1_PERIOD7_PERIOD_MASK 0xff
#define  WFG1_PERIOD7_PERIOD_SHIFT 0
#define  WFG1_PERIOD7_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD7_PERIOD_MASK) >> WFG1_PERIOD7_PERIOD_SHIFT)
#define  WFG1_PERIOD7_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD7_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD7_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 9 of 10) (period8)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG1_PERIOD8_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD8_RESERVED1_SHIFT 17
#define  WFG1_PERIOD8_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD8_RESERVED1_MASK) >> WFG1_PERIOD8_RESERVED1_SHIFT)
#define  WFG1_PERIOD8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD8_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD8_RESERVED1_SHIFT))

#define  WFG1_PERIOD8_IDLE_MASK 0x10000
#define  WFG1_PERIOD8_IDLE_SHIFT 16
#define  WFG1_PERIOD8_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD8_IDLE_MASK) >> WFG1_PERIOD8_IDLE_SHIFT)
#define  WFG1_PERIOD8_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD8_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD8_IDLE_SHIFT))

#define  WFG1_PERIOD8_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD8_ACTIVE_SHIFT 8
#define  WFG1_PERIOD8_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD8_ACTIVE_MASK) >> WFG1_PERIOD8_ACTIVE_SHIFT)
#define  WFG1_PERIOD8_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD8_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD8_ACTIVE_SHIFT))

#define  WFG1_PERIOD8_PERIOD_MASK 0xff
#define  WFG1_PERIOD8_PERIOD_SHIFT 0
#define  WFG1_PERIOD8_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD8_PERIOD_MASK) >> WFG1_PERIOD8_PERIOD_SHIFT)
#define  WFG1_PERIOD8_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD8_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD8_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 10 of 10) (period9)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG1_PERIOD9_RESERVED1_MASK 0xfffe0000
#define  WFG1_PERIOD9_RESERVED1_SHIFT 17
#define  WFG1_PERIOD9_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD9_RESERVED1_MASK) >> WFG1_PERIOD9_RESERVED1_SHIFT)
#define  WFG1_PERIOD9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD9_RESERVED1_MASK) | (((uint32_t)val) << WFG1_PERIOD9_RESERVED1_SHIFT))

#define  WFG1_PERIOD9_IDLE_MASK 0x10000
#define  WFG1_PERIOD9_IDLE_SHIFT 16
#define  WFG1_PERIOD9_IDLE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD9_IDLE_MASK) >> WFG1_PERIOD9_IDLE_SHIFT)
#define  WFG1_PERIOD9_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD9_IDLE_MASK) | (((uint32_t)val) << WFG1_PERIOD9_IDLE_SHIFT))

#define  WFG1_PERIOD9_ACTIVE_MASK 0xff00
#define  WFG1_PERIOD9_ACTIVE_SHIFT 8
#define  WFG1_PERIOD9_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD9_ACTIVE_MASK) >> WFG1_PERIOD9_ACTIVE_SHIFT)
#define  WFG1_PERIOD9_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD9_ACTIVE_MASK) | (((uint32_t)val) << WFG1_PERIOD9_ACTIVE_SHIFT))

#define  WFG1_PERIOD9_PERIOD_MASK 0xff
#define  WFG1_PERIOD9_PERIOD_SHIFT 0
#define  WFG1_PERIOD9_PERIOD_MASK_SHIFT(reg) (((reg) & WFG1_PERIOD9_PERIOD_MASK) >> WFG1_PERIOD9_PERIOD_SHIFT)
#define  WFG1_PERIOD9_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG1_PERIOD9_PERIOD_MASK) | (((uint32_t)val) << WFG1_PERIOD9_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 1 of 10) (action10)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG1_ACTION10_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION10_RESERVED1_SHIFT 30
#define  WFG1_ACTION10_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION10_RESERVED1_MASK) >> WFG1_ACTION10_RESERVED1_SHIFT)
#define  WFG1_ACTION10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION10_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION10_RESERVED1_SHIFT))

#define  WFG1_ACTION10_ACTION_MASK 0x30000000
#define  WFG1_ACTION10_ACTION_SHIFT 28
#define  WFG1_ACTION10_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION10_ACTION_MASK) >> WFG1_ACTION10_ACTION_SHIFT)
#define  WFG1_ACTION10_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION10_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION10_ACTION_SHIFT))

#define  WFG1_ACTION10_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION10_RESERVED2_SHIFT 24
#define  WFG1_ACTION10_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION10_RESERVED2_MASK) >> WFG1_ACTION10_RESERVED2_SHIFT)
#define  WFG1_ACTION10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION10_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION10_RESERVED2_SHIFT))

#define  WFG1_ACTION10_COUNT_MASK 0xffffff
#define  WFG1_ACTION10_COUNT_SHIFT 0
#define  WFG1_ACTION10_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION10_COUNT_MASK) >> WFG1_ACTION10_COUNT_SHIFT)
#define  WFG1_ACTION10_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION10_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION10_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 2 of 10) (action11)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG1_ACTION11_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION11_RESERVED1_SHIFT 30
#define  WFG1_ACTION11_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION11_RESERVED1_MASK) >> WFG1_ACTION11_RESERVED1_SHIFT)
#define  WFG1_ACTION11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION11_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION11_RESERVED1_SHIFT))

#define  WFG1_ACTION11_ACTION_MASK 0x30000000
#define  WFG1_ACTION11_ACTION_SHIFT 28
#define  WFG1_ACTION11_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION11_ACTION_MASK) >> WFG1_ACTION11_ACTION_SHIFT)
#define  WFG1_ACTION11_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION11_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION11_ACTION_SHIFT))

#define  WFG1_ACTION11_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION11_RESERVED2_SHIFT 24
#define  WFG1_ACTION11_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION11_RESERVED2_MASK) >> WFG1_ACTION11_RESERVED2_SHIFT)
#define  WFG1_ACTION11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION11_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION11_RESERVED2_SHIFT))

#define  WFG1_ACTION11_COUNT_MASK 0xffffff
#define  WFG1_ACTION11_COUNT_SHIFT 0
#define  WFG1_ACTION11_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION11_COUNT_MASK) >> WFG1_ACTION11_COUNT_SHIFT)
#define  WFG1_ACTION11_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION11_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION11_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 3 of 10) (action12)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG1_ACTION12_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION12_RESERVED1_SHIFT 30
#define  WFG1_ACTION12_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION12_RESERVED1_MASK) >> WFG1_ACTION12_RESERVED1_SHIFT)
#define  WFG1_ACTION12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION12_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION12_RESERVED1_SHIFT))

#define  WFG1_ACTION12_ACTION_MASK 0x30000000
#define  WFG1_ACTION12_ACTION_SHIFT 28
#define  WFG1_ACTION12_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION12_ACTION_MASK) >> WFG1_ACTION12_ACTION_SHIFT)
#define  WFG1_ACTION12_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION12_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION12_ACTION_SHIFT))

#define  WFG1_ACTION12_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION12_RESERVED2_SHIFT 24
#define  WFG1_ACTION12_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION12_RESERVED2_MASK) >> WFG1_ACTION12_RESERVED2_SHIFT)
#define  WFG1_ACTION12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION12_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION12_RESERVED2_SHIFT))

#define  WFG1_ACTION12_COUNT_MASK 0xffffff
#define  WFG1_ACTION12_COUNT_SHIFT 0
#define  WFG1_ACTION12_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION12_COUNT_MASK) >> WFG1_ACTION12_COUNT_SHIFT)
#define  WFG1_ACTION12_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION12_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION12_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 4 of 10) (action13)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG1_ACTION13_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION13_RESERVED1_SHIFT 30
#define  WFG1_ACTION13_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION13_RESERVED1_MASK) >> WFG1_ACTION13_RESERVED1_SHIFT)
#define  WFG1_ACTION13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION13_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION13_RESERVED1_SHIFT))

#define  WFG1_ACTION13_ACTION_MASK 0x30000000
#define  WFG1_ACTION13_ACTION_SHIFT 28
#define  WFG1_ACTION13_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION13_ACTION_MASK) >> WFG1_ACTION13_ACTION_SHIFT)
#define  WFG1_ACTION13_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION13_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION13_ACTION_SHIFT))

#define  WFG1_ACTION13_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION13_RESERVED2_SHIFT 24
#define  WFG1_ACTION13_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION13_RESERVED2_MASK) >> WFG1_ACTION13_RESERVED2_SHIFT)
#define  WFG1_ACTION13_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION13_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION13_RESERVED2_SHIFT))

#define  WFG1_ACTION13_COUNT_MASK 0xffffff
#define  WFG1_ACTION13_COUNT_SHIFT 0
#define  WFG1_ACTION13_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION13_COUNT_MASK) >> WFG1_ACTION13_COUNT_SHIFT)
#define  WFG1_ACTION13_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION13_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION13_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 5 of 10) (action14)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG1_ACTION14_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION14_RESERVED1_SHIFT 30
#define  WFG1_ACTION14_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION14_RESERVED1_MASK) >> WFG1_ACTION14_RESERVED1_SHIFT)
#define  WFG1_ACTION14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION14_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION14_RESERVED1_SHIFT))

#define  WFG1_ACTION14_ACTION_MASK 0x30000000
#define  WFG1_ACTION14_ACTION_SHIFT 28
#define  WFG1_ACTION14_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION14_ACTION_MASK) >> WFG1_ACTION14_ACTION_SHIFT)
#define  WFG1_ACTION14_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION14_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION14_ACTION_SHIFT))

#define  WFG1_ACTION14_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION14_RESERVED2_SHIFT 24
#define  WFG1_ACTION14_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION14_RESERVED2_MASK) >> WFG1_ACTION14_RESERVED2_SHIFT)
#define  WFG1_ACTION14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION14_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION14_RESERVED2_SHIFT))

#define  WFG1_ACTION14_COUNT_MASK 0xffffff
#define  WFG1_ACTION14_COUNT_SHIFT 0
#define  WFG1_ACTION14_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION14_COUNT_MASK) >> WFG1_ACTION14_COUNT_SHIFT)
#define  WFG1_ACTION14_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION14_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION14_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 6 of 10) (action15)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG1_ACTION15_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION15_RESERVED1_SHIFT 30
#define  WFG1_ACTION15_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION15_RESERVED1_MASK) >> WFG1_ACTION15_RESERVED1_SHIFT)
#define  WFG1_ACTION15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION15_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION15_RESERVED1_SHIFT))

#define  WFG1_ACTION15_ACTION_MASK 0x30000000
#define  WFG1_ACTION15_ACTION_SHIFT 28
#define  WFG1_ACTION15_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION15_ACTION_MASK) >> WFG1_ACTION15_ACTION_SHIFT)
#define  WFG1_ACTION15_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION15_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION15_ACTION_SHIFT))

#define  WFG1_ACTION15_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION15_RESERVED2_SHIFT 24
#define  WFG1_ACTION15_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION15_RESERVED2_MASK) >> WFG1_ACTION15_RESERVED2_SHIFT)
#define  WFG1_ACTION15_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION15_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION15_RESERVED2_SHIFT))

#define  WFG1_ACTION15_COUNT_MASK 0xffffff
#define  WFG1_ACTION15_COUNT_SHIFT 0
#define  WFG1_ACTION15_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION15_COUNT_MASK) >> WFG1_ACTION15_COUNT_SHIFT)
#define  WFG1_ACTION15_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION15_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION15_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 7 of 10) (action16)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG1_ACTION16_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION16_RESERVED1_SHIFT 30
#define  WFG1_ACTION16_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION16_RESERVED1_MASK) >> WFG1_ACTION16_RESERVED1_SHIFT)
#define  WFG1_ACTION16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION16_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION16_RESERVED1_SHIFT))

#define  WFG1_ACTION16_ACTION_MASK 0x30000000
#define  WFG1_ACTION16_ACTION_SHIFT 28
#define  WFG1_ACTION16_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION16_ACTION_MASK) >> WFG1_ACTION16_ACTION_SHIFT)
#define  WFG1_ACTION16_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION16_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION16_ACTION_SHIFT))

#define  WFG1_ACTION16_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION16_RESERVED2_SHIFT 24
#define  WFG1_ACTION16_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION16_RESERVED2_MASK) >> WFG1_ACTION16_RESERVED2_SHIFT)
#define  WFG1_ACTION16_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION16_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION16_RESERVED2_SHIFT))

#define  WFG1_ACTION16_COUNT_MASK 0xffffff
#define  WFG1_ACTION16_COUNT_SHIFT 0
#define  WFG1_ACTION16_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION16_COUNT_MASK) >> WFG1_ACTION16_COUNT_SHIFT)
#define  WFG1_ACTION16_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION16_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION16_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 8 of 10) (action17)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG1_ACTION17_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION17_RESERVED1_SHIFT 30
#define  WFG1_ACTION17_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION17_RESERVED1_MASK) >> WFG1_ACTION17_RESERVED1_SHIFT)
#define  WFG1_ACTION17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION17_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION17_RESERVED1_SHIFT))

#define  WFG1_ACTION17_ACTION_MASK 0x30000000
#define  WFG1_ACTION17_ACTION_SHIFT 28
#define  WFG1_ACTION17_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION17_ACTION_MASK) >> WFG1_ACTION17_ACTION_SHIFT)
#define  WFG1_ACTION17_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION17_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION17_ACTION_SHIFT))

#define  WFG1_ACTION17_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION17_RESERVED2_SHIFT 24
#define  WFG1_ACTION17_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION17_RESERVED2_MASK) >> WFG1_ACTION17_RESERVED2_SHIFT)
#define  WFG1_ACTION17_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION17_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION17_RESERVED2_SHIFT))

#define  WFG1_ACTION17_COUNT_MASK 0xffffff
#define  WFG1_ACTION17_COUNT_SHIFT 0
#define  WFG1_ACTION17_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION17_COUNT_MASK) >> WFG1_ACTION17_COUNT_SHIFT)
#define  WFG1_ACTION17_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION17_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION17_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 9 of 10) (action18)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG1_ACTION18_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION18_RESERVED1_SHIFT 30
#define  WFG1_ACTION18_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION18_RESERVED1_MASK) >> WFG1_ACTION18_RESERVED1_SHIFT)
#define  WFG1_ACTION18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION18_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION18_RESERVED1_SHIFT))

#define  WFG1_ACTION18_ACTION_MASK 0x30000000
#define  WFG1_ACTION18_ACTION_SHIFT 28
#define  WFG1_ACTION18_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION18_ACTION_MASK) >> WFG1_ACTION18_ACTION_SHIFT)
#define  WFG1_ACTION18_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION18_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION18_ACTION_SHIFT))

#define  WFG1_ACTION18_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION18_RESERVED2_SHIFT 24
#define  WFG1_ACTION18_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION18_RESERVED2_MASK) >> WFG1_ACTION18_RESERVED2_SHIFT)
#define  WFG1_ACTION18_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION18_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION18_RESERVED2_SHIFT))

#define  WFG1_ACTION18_COUNT_MASK 0xffffff
#define  WFG1_ACTION18_COUNT_SHIFT 0
#define  WFG1_ACTION18_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION18_COUNT_MASK) >> WFG1_ACTION18_COUNT_SHIFT)
#define  WFG1_ACTION18_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION18_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION18_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 10 of 10) (action19)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG1_ACTION19_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION19_RESERVED1_SHIFT 30
#define  WFG1_ACTION19_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION19_RESERVED1_MASK) >> WFG1_ACTION19_RESERVED1_SHIFT)
#define  WFG1_ACTION19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION19_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION19_RESERVED1_SHIFT))

#define  WFG1_ACTION19_ACTION_MASK 0x30000000
#define  WFG1_ACTION19_ACTION_SHIFT 28
#define  WFG1_ACTION19_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION19_ACTION_MASK) >> WFG1_ACTION19_ACTION_SHIFT)
#define  WFG1_ACTION19_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION19_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION19_ACTION_SHIFT))

#define  WFG1_ACTION19_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION19_RESERVED2_SHIFT 24
#define  WFG1_ACTION19_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION19_RESERVED2_MASK) >> WFG1_ACTION19_RESERVED2_SHIFT)
#define  WFG1_ACTION19_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION19_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION19_RESERVED2_SHIFT))

#define  WFG1_ACTION19_COUNT_MASK 0xffffff
#define  WFG1_ACTION19_COUNT_SHIFT 0
#define  WFG1_ACTION19_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION19_COUNT_MASK) >> WFG1_ACTION19_COUNT_SHIFT)
#define  WFG1_ACTION19_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION19_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION19_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 1 of 10) (action20)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG1_ACTION20_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION20_RESERVED1_SHIFT 30
#define  WFG1_ACTION20_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION20_RESERVED1_MASK) >> WFG1_ACTION20_RESERVED1_SHIFT)
#define  WFG1_ACTION20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION20_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION20_RESERVED1_SHIFT))

#define  WFG1_ACTION20_ACTION_MASK 0x30000000
#define  WFG1_ACTION20_ACTION_SHIFT 28
#define  WFG1_ACTION20_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION20_ACTION_MASK) >> WFG1_ACTION20_ACTION_SHIFT)
#define  WFG1_ACTION20_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION20_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION20_ACTION_SHIFT))

#define  WFG1_ACTION20_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION20_RESERVED2_SHIFT 24
#define  WFG1_ACTION20_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION20_RESERVED2_MASK) >> WFG1_ACTION20_RESERVED2_SHIFT)
#define  WFG1_ACTION20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION20_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION20_RESERVED2_SHIFT))

#define  WFG1_ACTION20_COUNT_MASK 0xffffff
#define  WFG1_ACTION20_COUNT_SHIFT 0
#define  WFG1_ACTION20_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION20_COUNT_MASK) >> WFG1_ACTION20_COUNT_SHIFT)
#define  WFG1_ACTION20_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION20_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION20_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 2 of 10) (action21)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG1_ACTION21_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION21_RESERVED1_SHIFT 30
#define  WFG1_ACTION21_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION21_RESERVED1_MASK) >> WFG1_ACTION21_RESERVED1_SHIFT)
#define  WFG1_ACTION21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION21_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION21_RESERVED1_SHIFT))

#define  WFG1_ACTION21_ACTION_MASK 0x30000000
#define  WFG1_ACTION21_ACTION_SHIFT 28
#define  WFG1_ACTION21_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION21_ACTION_MASK) >> WFG1_ACTION21_ACTION_SHIFT)
#define  WFG1_ACTION21_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION21_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION21_ACTION_SHIFT))

#define  WFG1_ACTION21_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION21_RESERVED2_SHIFT 24
#define  WFG1_ACTION21_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION21_RESERVED2_MASK) >> WFG1_ACTION21_RESERVED2_SHIFT)
#define  WFG1_ACTION21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION21_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION21_RESERVED2_SHIFT))

#define  WFG1_ACTION21_COUNT_MASK 0xffffff
#define  WFG1_ACTION21_COUNT_SHIFT 0
#define  WFG1_ACTION21_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION21_COUNT_MASK) >> WFG1_ACTION21_COUNT_SHIFT)
#define  WFG1_ACTION21_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION21_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION21_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 3 of 10) (action22)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG1_ACTION22_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION22_RESERVED1_SHIFT 30
#define  WFG1_ACTION22_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION22_RESERVED1_MASK) >> WFG1_ACTION22_RESERVED1_SHIFT)
#define  WFG1_ACTION22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION22_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION22_RESERVED1_SHIFT))

#define  WFG1_ACTION22_ACTION_MASK 0x30000000
#define  WFG1_ACTION22_ACTION_SHIFT 28
#define  WFG1_ACTION22_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION22_ACTION_MASK) >> WFG1_ACTION22_ACTION_SHIFT)
#define  WFG1_ACTION22_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION22_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION22_ACTION_SHIFT))

#define  WFG1_ACTION22_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION22_RESERVED2_SHIFT 24
#define  WFG1_ACTION22_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION22_RESERVED2_MASK) >> WFG1_ACTION22_RESERVED2_SHIFT)
#define  WFG1_ACTION22_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION22_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION22_RESERVED2_SHIFT))

#define  WFG1_ACTION22_COUNT_MASK 0xffffff
#define  WFG1_ACTION22_COUNT_SHIFT 0
#define  WFG1_ACTION22_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION22_COUNT_MASK) >> WFG1_ACTION22_COUNT_SHIFT)
#define  WFG1_ACTION22_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION22_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION22_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 4 of 10) (action23)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG1_ACTION23_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION23_RESERVED1_SHIFT 30
#define  WFG1_ACTION23_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION23_RESERVED1_MASK) >> WFG1_ACTION23_RESERVED1_SHIFT)
#define  WFG1_ACTION23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION23_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION23_RESERVED1_SHIFT))

#define  WFG1_ACTION23_ACTION_MASK 0x30000000
#define  WFG1_ACTION23_ACTION_SHIFT 28
#define  WFG1_ACTION23_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION23_ACTION_MASK) >> WFG1_ACTION23_ACTION_SHIFT)
#define  WFG1_ACTION23_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION23_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION23_ACTION_SHIFT))

#define  WFG1_ACTION23_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION23_RESERVED2_SHIFT 24
#define  WFG1_ACTION23_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION23_RESERVED2_MASK) >> WFG1_ACTION23_RESERVED2_SHIFT)
#define  WFG1_ACTION23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION23_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION23_RESERVED2_SHIFT))

#define  WFG1_ACTION23_COUNT_MASK 0xffffff
#define  WFG1_ACTION23_COUNT_SHIFT 0
#define  WFG1_ACTION23_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION23_COUNT_MASK) >> WFG1_ACTION23_COUNT_SHIFT)
#define  WFG1_ACTION23_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION23_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION23_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 5 of 10) (action24)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG1_ACTION24_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION24_RESERVED1_SHIFT 30
#define  WFG1_ACTION24_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION24_RESERVED1_MASK) >> WFG1_ACTION24_RESERVED1_SHIFT)
#define  WFG1_ACTION24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION24_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION24_RESERVED1_SHIFT))

#define  WFG1_ACTION24_ACTION_MASK 0x30000000
#define  WFG1_ACTION24_ACTION_SHIFT 28
#define  WFG1_ACTION24_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION24_ACTION_MASK) >> WFG1_ACTION24_ACTION_SHIFT)
#define  WFG1_ACTION24_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION24_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION24_ACTION_SHIFT))

#define  WFG1_ACTION24_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION24_RESERVED2_SHIFT 24
#define  WFG1_ACTION24_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION24_RESERVED2_MASK) >> WFG1_ACTION24_RESERVED2_SHIFT)
#define  WFG1_ACTION24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION24_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION24_RESERVED2_SHIFT))

#define  WFG1_ACTION24_COUNT_MASK 0xffffff
#define  WFG1_ACTION24_COUNT_SHIFT 0
#define  WFG1_ACTION24_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION24_COUNT_MASK) >> WFG1_ACTION24_COUNT_SHIFT)
#define  WFG1_ACTION24_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION24_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION24_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 6 of 10) (action25)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG1_ACTION25_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION25_RESERVED1_SHIFT 30
#define  WFG1_ACTION25_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION25_RESERVED1_MASK) >> WFG1_ACTION25_RESERVED1_SHIFT)
#define  WFG1_ACTION25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION25_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION25_RESERVED1_SHIFT))

#define  WFG1_ACTION25_ACTION_MASK 0x30000000
#define  WFG1_ACTION25_ACTION_SHIFT 28
#define  WFG1_ACTION25_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION25_ACTION_MASK) >> WFG1_ACTION25_ACTION_SHIFT)
#define  WFG1_ACTION25_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION25_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION25_ACTION_SHIFT))

#define  WFG1_ACTION25_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION25_RESERVED2_SHIFT 24
#define  WFG1_ACTION25_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION25_RESERVED2_MASK) >> WFG1_ACTION25_RESERVED2_SHIFT)
#define  WFG1_ACTION25_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION25_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION25_RESERVED2_SHIFT))

#define  WFG1_ACTION25_COUNT_MASK 0xffffff
#define  WFG1_ACTION25_COUNT_SHIFT 0
#define  WFG1_ACTION25_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION25_COUNT_MASK) >> WFG1_ACTION25_COUNT_SHIFT)
#define  WFG1_ACTION25_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION25_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION25_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 7 of 10) (action26)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG1_ACTION26_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION26_RESERVED1_SHIFT 30
#define  WFG1_ACTION26_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION26_RESERVED1_MASK) >> WFG1_ACTION26_RESERVED1_SHIFT)
#define  WFG1_ACTION26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION26_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION26_RESERVED1_SHIFT))

#define  WFG1_ACTION26_ACTION_MASK 0x30000000
#define  WFG1_ACTION26_ACTION_SHIFT 28
#define  WFG1_ACTION26_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION26_ACTION_MASK) >> WFG1_ACTION26_ACTION_SHIFT)
#define  WFG1_ACTION26_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION26_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION26_ACTION_SHIFT))

#define  WFG1_ACTION26_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION26_RESERVED2_SHIFT 24
#define  WFG1_ACTION26_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION26_RESERVED2_MASK) >> WFG1_ACTION26_RESERVED2_SHIFT)
#define  WFG1_ACTION26_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION26_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION26_RESERVED2_SHIFT))

#define  WFG1_ACTION26_COUNT_MASK 0xffffff
#define  WFG1_ACTION26_COUNT_SHIFT 0
#define  WFG1_ACTION26_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION26_COUNT_MASK) >> WFG1_ACTION26_COUNT_SHIFT)
#define  WFG1_ACTION26_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION26_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION26_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 8 of 10) (action27)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG1_ACTION27_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION27_RESERVED1_SHIFT 30
#define  WFG1_ACTION27_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION27_RESERVED1_MASK) >> WFG1_ACTION27_RESERVED1_SHIFT)
#define  WFG1_ACTION27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION27_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION27_RESERVED1_SHIFT))

#define  WFG1_ACTION27_ACTION_MASK 0x30000000
#define  WFG1_ACTION27_ACTION_SHIFT 28
#define  WFG1_ACTION27_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION27_ACTION_MASK) >> WFG1_ACTION27_ACTION_SHIFT)
#define  WFG1_ACTION27_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION27_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION27_ACTION_SHIFT))

#define  WFG1_ACTION27_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION27_RESERVED2_SHIFT 24
#define  WFG1_ACTION27_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION27_RESERVED2_MASK) >> WFG1_ACTION27_RESERVED2_SHIFT)
#define  WFG1_ACTION27_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION27_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION27_RESERVED2_SHIFT))

#define  WFG1_ACTION27_COUNT_MASK 0xffffff
#define  WFG1_ACTION27_COUNT_SHIFT 0
#define  WFG1_ACTION27_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION27_COUNT_MASK) >> WFG1_ACTION27_COUNT_SHIFT)
#define  WFG1_ACTION27_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION27_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION27_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 9 of 10) (action28)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG1_ACTION28_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION28_RESERVED1_SHIFT 30
#define  WFG1_ACTION28_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION28_RESERVED1_MASK) >> WFG1_ACTION28_RESERVED1_SHIFT)
#define  WFG1_ACTION28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION28_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION28_RESERVED1_SHIFT))

#define  WFG1_ACTION28_ACTION_MASK 0x30000000
#define  WFG1_ACTION28_ACTION_SHIFT 28
#define  WFG1_ACTION28_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION28_ACTION_MASK) >> WFG1_ACTION28_ACTION_SHIFT)
#define  WFG1_ACTION28_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION28_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION28_ACTION_SHIFT))

#define  WFG1_ACTION28_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION28_RESERVED2_SHIFT 24
#define  WFG1_ACTION28_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION28_RESERVED2_MASK) >> WFG1_ACTION28_RESERVED2_SHIFT)
#define  WFG1_ACTION28_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION28_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION28_RESERVED2_SHIFT))

#define  WFG1_ACTION28_COUNT_MASK 0xffffff
#define  WFG1_ACTION28_COUNT_SHIFT 0
#define  WFG1_ACTION28_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION28_COUNT_MASK) >> WFG1_ACTION28_COUNT_SHIFT)
#define  WFG1_ACTION28_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION28_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION28_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 10 of 10) (action29)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG1_ACTION29_RESERVED1_MASK 0xc0000000
#define  WFG1_ACTION29_RESERVED1_SHIFT 30
#define  WFG1_ACTION29_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG1_ACTION29_RESERVED1_MASK) >> WFG1_ACTION29_RESERVED1_SHIFT)
#define  WFG1_ACTION29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION29_RESERVED1_MASK) | (((uint32_t)val) << WFG1_ACTION29_RESERVED1_SHIFT))

#define  WFG1_ACTION29_ACTION_MASK 0x30000000
#define  WFG1_ACTION29_ACTION_SHIFT 28
#define  WFG1_ACTION29_ACTION_MASK_SHIFT(reg) (((reg) & WFG1_ACTION29_ACTION_MASK) >> WFG1_ACTION29_ACTION_SHIFT)
#define  WFG1_ACTION29_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION29_ACTION_MASK) | (((uint32_t)val) << WFG1_ACTION29_ACTION_SHIFT))

#define  WFG1_ACTION29_RESERVED2_MASK 0xf000000
#define  WFG1_ACTION29_RESERVED2_SHIFT 24
#define  WFG1_ACTION29_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG1_ACTION29_RESERVED2_MASK) >> WFG1_ACTION29_RESERVED2_SHIFT)
#define  WFG1_ACTION29_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION29_RESERVED2_MASK) | (((uint32_t)val) << WFG1_ACTION29_RESERVED2_SHIFT))

#define  WFG1_ACTION29_COUNT_MASK 0xffffff
#define  WFG1_ACTION29_COUNT_SHIFT 0
#define  WFG1_ACTION29_COUNT_MASK_SHIFT(reg) (((reg) & WFG1_ACTION29_COUNT_MASK) >> WFG1_ACTION29_COUNT_SHIFT)
#define  WFG1_ACTION29_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG1_ACTION29_COUNT_MASK) | (((uint32_t)val) << WFG1_ACTION29_COUNT_SHIFT))

//====================================================================
//
//Register File: Waverform Generator Control (Instance 3 of 4) (wfg2)
//
//====================================================================

//====================================================================
//Register: Waveform Generator Configuration register (scfg)
//====================================================================

#define  WFG2_SCFG_MODE_MASK 0xc0000000
#define  WFG2_SCFG_MODE_SHIFT 30
#define  WFG2_SCFG_MODE_MASK_SHIFT(reg) (((reg) & WFG2_SCFG_MODE_MASK) >> WFG2_SCFG_MODE_SHIFT)
#define  WFG2_SCFG_MODE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_SCFG_MODE_MASK) | (((uint32_t)val) << WFG2_SCFG_MODE_SHIFT))

#define  WFG2_SCFG_RECOV_MASK 0x20000000
#define  WFG2_SCFG_RECOV_SHIFT 29
#define  WFG2_SCFG_RECOV_MASK_SHIFT(reg) (((reg) & WFG2_SCFG_RECOV_MASK) >> WFG2_SCFG_RECOV_SHIFT)
#define  WFG2_SCFG_RECOV_REPLACE_VAL(reg,val) (((reg) & ~WFG2_SCFG_RECOV_MASK) | (((uint32_t)val) << WFG2_SCFG_RECOV_SHIFT))

#define  WFG2_SCFG_SYNC_SEL_MASK 0x10000000
#define  WFG2_SCFG_SYNC_SEL_SHIFT 28
#define  WFG2_SCFG_SYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG2_SCFG_SYNC_SEL_MASK) >> WFG2_SCFG_SYNC_SEL_SHIFT)
#define  WFG2_SCFG_SYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG2_SCFG_SYNC_SEL_MASK) | (((uint32_t)val) << WFG2_SCFG_SYNC_SEL_SHIFT))

#define  WFG2_SCFG_GALVO_SEL_MASK 0xc000000
#define  WFG2_SCFG_GALVO_SEL_SHIFT 26
#define  WFG2_SCFG_GALVO_SEL_MASK_SHIFT(reg) (((reg) & WFG2_SCFG_GALVO_SEL_MASK) >> WFG2_SCFG_GALVO_SEL_SHIFT)
#define  WFG2_SCFG_GALVO_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG2_SCFG_GALVO_SEL_MASK) | (((uint32_t)val) << WFG2_SCFG_GALVO_SEL_SHIFT))

#define  WFG2_SCFG_HSYNC_SEL_MASK 0x3000000
#define  WFG2_SCFG_HSYNC_SEL_SHIFT 24
#define  WFG2_SCFG_HSYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG2_SCFG_HSYNC_SEL_MASK) >> WFG2_SCFG_HSYNC_SEL_SHIFT)
#define  WFG2_SCFG_HSYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG2_SCFG_HSYNC_SEL_MASK) | (((uint32_t)val) << WFG2_SCFG_HSYNC_SEL_SHIFT))

#define  WFG2_SCFG_COUNT_MASK 0xffffff
#define  WFG2_SCFG_COUNT_SHIFT 0
#define  WFG2_SCFG_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_SCFG_COUNT_MASK) >> WFG2_SCFG_COUNT_SHIFT)
#define  WFG2_SCFG_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_SCFG_COUNT_MASK) | (((uint32_t)val) << WFG2_SCFG_COUNT_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 1 of 10) (period0)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG2_PERIOD0_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD0_RESERVED1_SHIFT 17
#define  WFG2_PERIOD0_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD0_RESERVED1_MASK) >> WFG2_PERIOD0_RESERVED1_SHIFT)
#define  WFG2_PERIOD0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD0_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD0_RESERVED1_SHIFT))

#define  WFG2_PERIOD0_IDLE_MASK 0x10000
#define  WFG2_PERIOD0_IDLE_SHIFT 16
#define  WFG2_PERIOD0_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD0_IDLE_MASK) >> WFG2_PERIOD0_IDLE_SHIFT)
#define  WFG2_PERIOD0_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD0_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD0_IDLE_SHIFT))

#define  WFG2_PERIOD0_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD0_ACTIVE_SHIFT 8
#define  WFG2_PERIOD0_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD0_ACTIVE_MASK) >> WFG2_PERIOD0_ACTIVE_SHIFT)
#define  WFG2_PERIOD0_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD0_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD0_ACTIVE_SHIFT))

#define  WFG2_PERIOD0_PERIOD_MASK 0xff
#define  WFG2_PERIOD0_PERIOD_SHIFT 0
#define  WFG2_PERIOD0_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD0_PERIOD_MASK) >> WFG2_PERIOD0_PERIOD_SHIFT)
#define  WFG2_PERIOD0_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD0_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD0_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 2 of 10) (period1)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG2_PERIOD1_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD1_RESERVED1_SHIFT 17
#define  WFG2_PERIOD1_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD1_RESERVED1_MASK) >> WFG2_PERIOD1_RESERVED1_SHIFT)
#define  WFG2_PERIOD1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD1_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD1_RESERVED1_SHIFT))

#define  WFG2_PERIOD1_IDLE_MASK 0x10000
#define  WFG2_PERIOD1_IDLE_SHIFT 16
#define  WFG2_PERIOD1_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD1_IDLE_MASK) >> WFG2_PERIOD1_IDLE_SHIFT)
#define  WFG2_PERIOD1_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD1_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD1_IDLE_SHIFT))

#define  WFG2_PERIOD1_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD1_ACTIVE_SHIFT 8
#define  WFG2_PERIOD1_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD1_ACTIVE_MASK) >> WFG2_PERIOD1_ACTIVE_SHIFT)
#define  WFG2_PERIOD1_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD1_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD1_ACTIVE_SHIFT))

#define  WFG2_PERIOD1_PERIOD_MASK 0xff
#define  WFG2_PERIOD1_PERIOD_SHIFT 0
#define  WFG2_PERIOD1_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD1_PERIOD_MASK) >> WFG2_PERIOD1_PERIOD_SHIFT)
#define  WFG2_PERIOD1_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD1_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD1_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 3 of 10) (period2)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG2_PERIOD2_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD2_RESERVED1_SHIFT 17
#define  WFG2_PERIOD2_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD2_RESERVED1_MASK) >> WFG2_PERIOD2_RESERVED1_SHIFT)
#define  WFG2_PERIOD2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD2_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD2_RESERVED1_SHIFT))

#define  WFG2_PERIOD2_IDLE_MASK 0x10000
#define  WFG2_PERIOD2_IDLE_SHIFT 16
#define  WFG2_PERIOD2_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD2_IDLE_MASK) >> WFG2_PERIOD2_IDLE_SHIFT)
#define  WFG2_PERIOD2_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD2_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD2_IDLE_SHIFT))

#define  WFG2_PERIOD2_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD2_ACTIVE_SHIFT 8
#define  WFG2_PERIOD2_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD2_ACTIVE_MASK) >> WFG2_PERIOD2_ACTIVE_SHIFT)
#define  WFG2_PERIOD2_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD2_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD2_ACTIVE_SHIFT))

#define  WFG2_PERIOD2_PERIOD_MASK 0xff
#define  WFG2_PERIOD2_PERIOD_SHIFT 0
#define  WFG2_PERIOD2_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD2_PERIOD_MASK) >> WFG2_PERIOD2_PERIOD_SHIFT)
#define  WFG2_PERIOD2_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD2_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD2_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 4 of 10) (period3)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG2_PERIOD3_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD3_RESERVED1_SHIFT 17
#define  WFG2_PERIOD3_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD3_RESERVED1_MASK) >> WFG2_PERIOD3_RESERVED1_SHIFT)
#define  WFG2_PERIOD3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD3_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD3_RESERVED1_SHIFT))

#define  WFG2_PERIOD3_IDLE_MASK 0x10000
#define  WFG2_PERIOD3_IDLE_SHIFT 16
#define  WFG2_PERIOD3_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD3_IDLE_MASK) >> WFG2_PERIOD3_IDLE_SHIFT)
#define  WFG2_PERIOD3_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD3_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD3_IDLE_SHIFT))

#define  WFG2_PERIOD3_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD3_ACTIVE_SHIFT 8
#define  WFG2_PERIOD3_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD3_ACTIVE_MASK) >> WFG2_PERIOD3_ACTIVE_SHIFT)
#define  WFG2_PERIOD3_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD3_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD3_ACTIVE_SHIFT))

#define  WFG2_PERIOD3_PERIOD_MASK 0xff
#define  WFG2_PERIOD3_PERIOD_SHIFT 0
#define  WFG2_PERIOD3_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD3_PERIOD_MASK) >> WFG2_PERIOD3_PERIOD_SHIFT)
#define  WFG2_PERIOD3_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD3_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD3_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 5 of 10) (period4)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG2_PERIOD4_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD4_RESERVED1_SHIFT 17
#define  WFG2_PERIOD4_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD4_RESERVED1_MASK) >> WFG2_PERIOD4_RESERVED1_SHIFT)
#define  WFG2_PERIOD4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD4_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD4_RESERVED1_SHIFT))

#define  WFG2_PERIOD4_IDLE_MASK 0x10000
#define  WFG2_PERIOD4_IDLE_SHIFT 16
#define  WFG2_PERIOD4_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD4_IDLE_MASK) >> WFG2_PERIOD4_IDLE_SHIFT)
#define  WFG2_PERIOD4_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD4_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD4_IDLE_SHIFT))

#define  WFG2_PERIOD4_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD4_ACTIVE_SHIFT 8
#define  WFG2_PERIOD4_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD4_ACTIVE_MASK) >> WFG2_PERIOD4_ACTIVE_SHIFT)
#define  WFG2_PERIOD4_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD4_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD4_ACTIVE_SHIFT))

#define  WFG2_PERIOD4_PERIOD_MASK 0xff
#define  WFG2_PERIOD4_PERIOD_SHIFT 0
#define  WFG2_PERIOD4_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD4_PERIOD_MASK) >> WFG2_PERIOD4_PERIOD_SHIFT)
#define  WFG2_PERIOD4_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD4_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD4_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 6 of 10) (period5)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG2_PERIOD5_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD5_RESERVED1_SHIFT 17
#define  WFG2_PERIOD5_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD5_RESERVED1_MASK) >> WFG2_PERIOD5_RESERVED1_SHIFT)
#define  WFG2_PERIOD5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD5_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD5_RESERVED1_SHIFT))

#define  WFG2_PERIOD5_IDLE_MASK 0x10000
#define  WFG2_PERIOD5_IDLE_SHIFT 16
#define  WFG2_PERIOD5_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD5_IDLE_MASK) >> WFG2_PERIOD5_IDLE_SHIFT)
#define  WFG2_PERIOD5_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD5_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD5_IDLE_SHIFT))

#define  WFG2_PERIOD5_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD5_ACTIVE_SHIFT 8
#define  WFG2_PERIOD5_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD5_ACTIVE_MASK) >> WFG2_PERIOD5_ACTIVE_SHIFT)
#define  WFG2_PERIOD5_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD5_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD5_ACTIVE_SHIFT))

#define  WFG2_PERIOD5_PERIOD_MASK 0xff
#define  WFG2_PERIOD5_PERIOD_SHIFT 0
#define  WFG2_PERIOD5_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD5_PERIOD_MASK) >> WFG2_PERIOD5_PERIOD_SHIFT)
#define  WFG2_PERIOD5_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD5_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD5_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 7 of 10) (period6)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG2_PERIOD6_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD6_RESERVED1_SHIFT 17
#define  WFG2_PERIOD6_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD6_RESERVED1_MASK) >> WFG2_PERIOD6_RESERVED1_SHIFT)
#define  WFG2_PERIOD6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD6_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD6_RESERVED1_SHIFT))

#define  WFG2_PERIOD6_IDLE_MASK 0x10000
#define  WFG2_PERIOD6_IDLE_SHIFT 16
#define  WFG2_PERIOD6_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD6_IDLE_MASK) >> WFG2_PERIOD6_IDLE_SHIFT)
#define  WFG2_PERIOD6_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD6_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD6_IDLE_SHIFT))

#define  WFG2_PERIOD6_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD6_ACTIVE_SHIFT 8
#define  WFG2_PERIOD6_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD6_ACTIVE_MASK) >> WFG2_PERIOD6_ACTIVE_SHIFT)
#define  WFG2_PERIOD6_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD6_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD6_ACTIVE_SHIFT))

#define  WFG2_PERIOD6_PERIOD_MASK 0xff
#define  WFG2_PERIOD6_PERIOD_SHIFT 0
#define  WFG2_PERIOD6_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD6_PERIOD_MASK) >> WFG2_PERIOD6_PERIOD_SHIFT)
#define  WFG2_PERIOD6_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD6_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD6_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 8 of 10) (period7)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG2_PERIOD7_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD7_RESERVED1_SHIFT 17
#define  WFG2_PERIOD7_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD7_RESERVED1_MASK) >> WFG2_PERIOD7_RESERVED1_SHIFT)
#define  WFG2_PERIOD7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD7_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD7_RESERVED1_SHIFT))

#define  WFG2_PERIOD7_IDLE_MASK 0x10000
#define  WFG2_PERIOD7_IDLE_SHIFT 16
#define  WFG2_PERIOD7_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD7_IDLE_MASK) >> WFG2_PERIOD7_IDLE_SHIFT)
#define  WFG2_PERIOD7_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD7_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD7_IDLE_SHIFT))

#define  WFG2_PERIOD7_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD7_ACTIVE_SHIFT 8
#define  WFG2_PERIOD7_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD7_ACTIVE_MASK) >> WFG2_PERIOD7_ACTIVE_SHIFT)
#define  WFG2_PERIOD7_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD7_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD7_ACTIVE_SHIFT))

#define  WFG2_PERIOD7_PERIOD_MASK 0xff
#define  WFG2_PERIOD7_PERIOD_SHIFT 0
#define  WFG2_PERIOD7_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD7_PERIOD_MASK) >> WFG2_PERIOD7_PERIOD_SHIFT)
#define  WFG2_PERIOD7_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD7_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD7_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 9 of 10) (period8)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG2_PERIOD8_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD8_RESERVED1_SHIFT 17
#define  WFG2_PERIOD8_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD8_RESERVED1_MASK) >> WFG2_PERIOD8_RESERVED1_SHIFT)
#define  WFG2_PERIOD8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD8_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD8_RESERVED1_SHIFT))

#define  WFG2_PERIOD8_IDLE_MASK 0x10000
#define  WFG2_PERIOD8_IDLE_SHIFT 16
#define  WFG2_PERIOD8_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD8_IDLE_MASK) >> WFG2_PERIOD8_IDLE_SHIFT)
#define  WFG2_PERIOD8_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD8_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD8_IDLE_SHIFT))

#define  WFG2_PERIOD8_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD8_ACTIVE_SHIFT 8
#define  WFG2_PERIOD8_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD8_ACTIVE_MASK) >> WFG2_PERIOD8_ACTIVE_SHIFT)
#define  WFG2_PERIOD8_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD8_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD8_ACTIVE_SHIFT))

#define  WFG2_PERIOD8_PERIOD_MASK 0xff
#define  WFG2_PERIOD8_PERIOD_SHIFT 0
#define  WFG2_PERIOD8_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD8_PERIOD_MASK) >> WFG2_PERIOD8_PERIOD_SHIFT)
#define  WFG2_PERIOD8_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD8_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD8_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 10 of 10) (period9)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG2_PERIOD9_RESERVED1_MASK 0xfffe0000
#define  WFG2_PERIOD9_RESERVED1_SHIFT 17
#define  WFG2_PERIOD9_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD9_RESERVED1_MASK) >> WFG2_PERIOD9_RESERVED1_SHIFT)
#define  WFG2_PERIOD9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD9_RESERVED1_MASK) | (((uint32_t)val) << WFG2_PERIOD9_RESERVED1_SHIFT))

#define  WFG2_PERIOD9_IDLE_MASK 0x10000
#define  WFG2_PERIOD9_IDLE_SHIFT 16
#define  WFG2_PERIOD9_IDLE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD9_IDLE_MASK) >> WFG2_PERIOD9_IDLE_SHIFT)
#define  WFG2_PERIOD9_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD9_IDLE_MASK) | (((uint32_t)val) << WFG2_PERIOD9_IDLE_SHIFT))

#define  WFG2_PERIOD9_ACTIVE_MASK 0xff00
#define  WFG2_PERIOD9_ACTIVE_SHIFT 8
#define  WFG2_PERIOD9_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD9_ACTIVE_MASK) >> WFG2_PERIOD9_ACTIVE_SHIFT)
#define  WFG2_PERIOD9_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD9_ACTIVE_MASK) | (((uint32_t)val) << WFG2_PERIOD9_ACTIVE_SHIFT))

#define  WFG2_PERIOD9_PERIOD_MASK 0xff
#define  WFG2_PERIOD9_PERIOD_SHIFT 0
#define  WFG2_PERIOD9_PERIOD_MASK_SHIFT(reg) (((reg) & WFG2_PERIOD9_PERIOD_MASK) >> WFG2_PERIOD9_PERIOD_SHIFT)
#define  WFG2_PERIOD9_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG2_PERIOD9_PERIOD_MASK) | (((uint32_t)val) << WFG2_PERIOD9_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 1 of 10) (action10)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG2_ACTION10_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION10_RESERVED1_SHIFT 30
#define  WFG2_ACTION10_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION10_RESERVED1_MASK) >> WFG2_ACTION10_RESERVED1_SHIFT)
#define  WFG2_ACTION10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION10_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION10_RESERVED1_SHIFT))

#define  WFG2_ACTION10_ACTION_MASK 0x30000000
#define  WFG2_ACTION10_ACTION_SHIFT 28
#define  WFG2_ACTION10_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION10_ACTION_MASK) >> WFG2_ACTION10_ACTION_SHIFT)
#define  WFG2_ACTION10_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION10_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION10_ACTION_SHIFT))

#define  WFG2_ACTION10_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION10_RESERVED2_SHIFT 24
#define  WFG2_ACTION10_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION10_RESERVED2_MASK) >> WFG2_ACTION10_RESERVED2_SHIFT)
#define  WFG2_ACTION10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION10_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION10_RESERVED2_SHIFT))

#define  WFG2_ACTION10_COUNT_MASK 0xffffff
#define  WFG2_ACTION10_COUNT_SHIFT 0
#define  WFG2_ACTION10_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION10_COUNT_MASK) >> WFG2_ACTION10_COUNT_SHIFT)
#define  WFG2_ACTION10_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION10_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION10_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 2 of 10) (action11)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG2_ACTION11_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION11_RESERVED1_SHIFT 30
#define  WFG2_ACTION11_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION11_RESERVED1_MASK) >> WFG2_ACTION11_RESERVED1_SHIFT)
#define  WFG2_ACTION11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION11_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION11_RESERVED1_SHIFT))

#define  WFG2_ACTION11_ACTION_MASK 0x30000000
#define  WFG2_ACTION11_ACTION_SHIFT 28
#define  WFG2_ACTION11_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION11_ACTION_MASK) >> WFG2_ACTION11_ACTION_SHIFT)
#define  WFG2_ACTION11_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION11_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION11_ACTION_SHIFT))

#define  WFG2_ACTION11_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION11_RESERVED2_SHIFT 24
#define  WFG2_ACTION11_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION11_RESERVED2_MASK) >> WFG2_ACTION11_RESERVED2_SHIFT)
#define  WFG2_ACTION11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION11_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION11_RESERVED2_SHIFT))

#define  WFG2_ACTION11_COUNT_MASK 0xffffff
#define  WFG2_ACTION11_COUNT_SHIFT 0
#define  WFG2_ACTION11_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION11_COUNT_MASK) >> WFG2_ACTION11_COUNT_SHIFT)
#define  WFG2_ACTION11_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION11_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION11_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 3 of 10) (action12)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG2_ACTION12_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION12_RESERVED1_SHIFT 30
#define  WFG2_ACTION12_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION12_RESERVED1_MASK) >> WFG2_ACTION12_RESERVED1_SHIFT)
#define  WFG2_ACTION12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION12_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION12_RESERVED1_SHIFT))

#define  WFG2_ACTION12_ACTION_MASK 0x30000000
#define  WFG2_ACTION12_ACTION_SHIFT 28
#define  WFG2_ACTION12_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION12_ACTION_MASK) >> WFG2_ACTION12_ACTION_SHIFT)
#define  WFG2_ACTION12_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION12_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION12_ACTION_SHIFT))

#define  WFG2_ACTION12_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION12_RESERVED2_SHIFT 24
#define  WFG2_ACTION12_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION12_RESERVED2_MASK) >> WFG2_ACTION12_RESERVED2_SHIFT)
#define  WFG2_ACTION12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION12_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION12_RESERVED2_SHIFT))

#define  WFG2_ACTION12_COUNT_MASK 0xffffff
#define  WFG2_ACTION12_COUNT_SHIFT 0
#define  WFG2_ACTION12_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION12_COUNT_MASK) >> WFG2_ACTION12_COUNT_SHIFT)
#define  WFG2_ACTION12_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION12_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION12_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 4 of 10) (action13)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG2_ACTION13_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION13_RESERVED1_SHIFT 30
#define  WFG2_ACTION13_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION13_RESERVED1_MASK) >> WFG2_ACTION13_RESERVED1_SHIFT)
#define  WFG2_ACTION13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION13_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION13_RESERVED1_SHIFT))

#define  WFG2_ACTION13_ACTION_MASK 0x30000000
#define  WFG2_ACTION13_ACTION_SHIFT 28
#define  WFG2_ACTION13_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION13_ACTION_MASK) >> WFG2_ACTION13_ACTION_SHIFT)
#define  WFG2_ACTION13_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION13_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION13_ACTION_SHIFT))

#define  WFG2_ACTION13_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION13_RESERVED2_SHIFT 24
#define  WFG2_ACTION13_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION13_RESERVED2_MASK) >> WFG2_ACTION13_RESERVED2_SHIFT)
#define  WFG2_ACTION13_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION13_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION13_RESERVED2_SHIFT))

#define  WFG2_ACTION13_COUNT_MASK 0xffffff
#define  WFG2_ACTION13_COUNT_SHIFT 0
#define  WFG2_ACTION13_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION13_COUNT_MASK) >> WFG2_ACTION13_COUNT_SHIFT)
#define  WFG2_ACTION13_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION13_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION13_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 5 of 10) (action14)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG2_ACTION14_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION14_RESERVED1_SHIFT 30
#define  WFG2_ACTION14_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION14_RESERVED1_MASK) >> WFG2_ACTION14_RESERVED1_SHIFT)
#define  WFG2_ACTION14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION14_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION14_RESERVED1_SHIFT))

#define  WFG2_ACTION14_ACTION_MASK 0x30000000
#define  WFG2_ACTION14_ACTION_SHIFT 28
#define  WFG2_ACTION14_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION14_ACTION_MASK) >> WFG2_ACTION14_ACTION_SHIFT)
#define  WFG2_ACTION14_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION14_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION14_ACTION_SHIFT))

#define  WFG2_ACTION14_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION14_RESERVED2_SHIFT 24
#define  WFG2_ACTION14_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION14_RESERVED2_MASK) >> WFG2_ACTION14_RESERVED2_SHIFT)
#define  WFG2_ACTION14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION14_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION14_RESERVED2_SHIFT))

#define  WFG2_ACTION14_COUNT_MASK 0xffffff
#define  WFG2_ACTION14_COUNT_SHIFT 0
#define  WFG2_ACTION14_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION14_COUNT_MASK) >> WFG2_ACTION14_COUNT_SHIFT)
#define  WFG2_ACTION14_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION14_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION14_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 6 of 10) (action15)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG2_ACTION15_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION15_RESERVED1_SHIFT 30
#define  WFG2_ACTION15_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION15_RESERVED1_MASK) >> WFG2_ACTION15_RESERVED1_SHIFT)
#define  WFG2_ACTION15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION15_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION15_RESERVED1_SHIFT))

#define  WFG2_ACTION15_ACTION_MASK 0x30000000
#define  WFG2_ACTION15_ACTION_SHIFT 28
#define  WFG2_ACTION15_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION15_ACTION_MASK) >> WFG2_ACTION15_ACTION_SHIFT)
#define  WFG2_ACTION15_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION15_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION15_ACTION_SHIFT))

#define  WFG2_ACTION15_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION15_RESERVED2_SHIFT 24
#define  WFG2_ACTION15_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION15_RESERVED2_MASK) >> WFG2_ACTION15_RESERVED2_SHIFT)
#define  WFG2_ACTION15_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION15_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION15_RESERVED2_SHIFT))

#define  WFG2_ACTION15_COUNT_MASK 0xffffff
#define  WFG2_ACTION15_COUNT_SHIFT 0
#define  WFG2_ACTION15_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION15_COUNT_MASK) >> WFG2_ACTION15_COUNT_SHIFT)
#define  WFG2_ACTION15_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION15_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION15_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 7 of 10) (action16)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG2_ACTION16_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION16_RESERVED1_SHIFT 30
#define  WFG2_ACTION16_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION16_RESERVED1_MASK) >> WFG2_ACTION16_RESERVED1_SHIFT)
#define  WFG2_ACTION16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION16_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION16_RESERVED1_SHIFT))

#define  WFG2_ACTION16_ACTION_MASK 0x30000000
#define  WFG2_ACTION16_ACTION_SHIFT 28
#define  WFG2_ACTION16_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION16_ACTION_MASK) >> WFG2_ACTION16_ACTION_SHIFT)
#define  WFG2_ACTION16_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION16_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION16_ACTION_SHIFT))

#define  WFG2_ACTION16_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION16_RESERVED2_SHIFT 24
#define  WFG2_ACTION16_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION16_RESERVED2_MASK) >> WFG2_ACTION16_RESERVED2_SHIFT)
#define  WFG2_ACTION16_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION16_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION16_RESERVED2_SHIFT))

#define  WFG2_ACTION16_COUNT_MASK 0xffffff
#define  WFG2_ACTION16_COUNT_SHIFT 0
#define  WFG2_ACTION16_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION16_COUNT_MASK) >> WFG2_ACTION16_COUNT_SHIFT)
#define  WFG2_ACTION16_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION16_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION16_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 8 of 10) (action17)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG2_ACTION17_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION17_RESERVED1_SHIFT 30
#define  WFG2_ACTION17_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION17_RESERVED1_MASK) >> WFG2_ACTION17_RESERVED1_SHIFT)
#define  WFG2_ACTION17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION17_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION17_RESERVED1_SHIFT))

#define  WFG2_ACTION17_ACTION_MASK 0x30000000
#define  WFG2_ACTION17_ACTION_SHIFT 28
#define  WFG2_ACTION17_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION17_ACTION_MASK) >> WFG2_ACTION17_ACTION_SHIFT)
#define  WFG2_ACTION17_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION17_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION17_ACTION_SHIFT))

#define  WFG2_ACTION17_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION17_RESERVED2_SHIFT 24
#define  WFG2_ACTION17_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION17_RESERVED2_MASK) >> WFG2_ACTION17_RESERVED2_SHIFT)
#define  WFG2_ACTION17_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION17_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION17_RESERVED2_SHIFT))

#define  WFG2_ACTION17_COUNT_MASK 0xffffff
#define  WFG2_ACTION17_COUNT_SHIFT 0
#define  WFG2_ACTION17_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION17_COUNT_MASK) >> WFG2_ACTION17_COUNT_SHIFT)
#define  WFG2_ACTION17_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION17_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION17_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 9 of 10) (action18)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG2_ACTION18_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION18_RESERVED1_SHIFT 30
#define  WFG2_ACTION18_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION18_RESERVED1_MASK) >> WFG2_ACTION18_RESERVED1_SHIFT)
#define  WFG2_ACTION18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION18_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION18_RESERVED1_SHIFT))

#define  WFG2_ACTION18_ACTION_MASK 0x30000000
#define  WFG2_ACTION18_ACTION_SHIFT 28
#define  WFG2_ACTION18_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION18_ACTION_MASK) >> WFG2_ACTION18_ACTION_SHIFT)
#define  WFG2_ACTION18_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION18_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION18_ACTION_SHIFT))

#define  WFG2_ACTION18_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION18_RESERVED2_SHIFT 24
#define  WFG2_ACTION18_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION18_RESERVED2_MASK) >> WFG2_ACTION18_RESERVED2_SHIFT)
#define  WFG2_ACTION18_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION18_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION18_RESERVED2_SHIFT))

#define  WFG2_ACTION18_COUNT_MASK 0xffffff
#define  WFG2_ACTION18_COUNT_SHIFT 0
#define  WFG2_ACTION18_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION18_COUNT_MASK) >> WFG2_ACTION18_COUNT_SHIFT)
#define  WFG2_ACTION18_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION18_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION18_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 10 of 10) (action19)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG2_ACTION19_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION19_RESERVED1_SHIFT 30
#define  WFG2_ACTION19_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION19_RESERVED1_MASK) >> WFG2_ACTION19_RESERVED1_SHIFT)
#define  WFG2_ACTION19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION19_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION19_RESERVED1_SHIFT))

#define  WFG2_ACTION19_ACTION_MASK 0x30000000
#define  WFG2_ACTION19_ACTION_SHIFT 28
#define  WFG2_ACTION19_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION19_ACTION_MASK) >> WFG2_ACTION19_ACTION_SHIFT)
#define  WFG2_ACTION19_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION19_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION19_ACTION_SHIFT))

#define  WFG2_ACTION19_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION19_RESERVED2_SHIFT 24
#define  WFG2_ACTION19_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION19_RESERVED2_MASK) >> WFG2_ACTION19_RESERVED2_SHIFT)
#define  WFG2_ACTION19_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION19_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION19_RESERVED2_SHIFT))

#define  WFG2_ACTION19_COUNT_MASK 0xffffff
#define  WFG2_ACTION19_COUNT_SHIFT 0
#define  WFG2_ACTION19_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION19_COUNT_MASK) >> WFG2_ACTION19_COUNT_SHIFT)
#define  WFG2_ACTION19_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION19_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION19_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 1 of 10) (action20)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG2_ACTION20_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION20_RESERVED1_SHIFT 30
#define  WFG2_ACTION20_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION20_RESERVED1_MASK) >> WFG2_ACTION20_RESERVED1_SHIFT)
#define  WFG2_ACTION20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION20_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION20_RESERVED1_SHIFT))

#define  WFG2_ACTION20_ACTION_MASK 0x30000000
#define  WFG2_ACTION20_ACTION_SHIFT 28
#define  WFG2_ACTION20_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION20_ACTION_MASK) >> WFG2_ACTION20_ACTION_SHIFT)
#define  WFG2_ACTION20_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION20_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION20_ACTION_SHIFT))

#define  WFG2_ACTION20_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION20_RESERVED2_SHIFT 24
#define  WFG2_ACTION20_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION20_RESERVED2_MASK) >> WFG2_ACTION20_RESERVED2_SHIFT)
#define  WFG2_ACTION20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION20_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION20_RESERVED2_SHIFT))

#define  WFG2_ACTION20_COUNT_MASK 0xffffff
#define  WFG2_ACTION20_COUNT_SHIFT 0
#define  WFG2_ACTION20_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION20_COUNT_MASK) >> WFG2_ACTION20_COUNT_SHIFT)
#define  WFG2_ACTION20_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION20_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION20_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 2 of 10) (action21)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG2_ACTION21_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION21_RESERVED1_SHIFT 30
#define  WFG2_ACTION21_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION21_RESERVED1_MASK) >> WFG2_ACTION21_RESERVED1_SHIFT)
#define  WFG2_ACTION21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION21_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION21_RESERVED1_SHIFT))

#define  WFG2_ACTION21_ACTION_MASK 0x30000000
#define  WFG2_ACTION21_ACTION_SHIFT 28
#define  WFG2_ACTION21_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION21_ACTION_MASK) >> WFG2_ACTION21_ACTION_SHIFT)
#define  WFG2_ACTION21_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION21_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION21_ACTION_SHIFT))

#define  WFG2_ACTION21_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION21_RESERVED2_SHIFT 24
#define  WFG2_ACTION21_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION21_RESERVED2_MASK) >> WFG2_ACTION21_RESERVED2_SHIFT)
#define  WFG2_ACTION21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION21_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION21_RESERVED2_SHIFT))

#define  WFG2_ACTION21_COUNT_MASK 0xffffff
#define  WFG2_ACTION21_COUNT_SHIFT 0
#define  WFG2_ACTION21_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION21_COUNT_MASK) >> WFG2_ACTION21_COUNT_SHIFT)
#define  WFG2_ACTION21_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION21_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION21_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 3 of 10) (action22)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG2_ACTION22_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION22_RESERVED1_SHIFT 30
#define  WFG2_ACTION22_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION22_RESERVED1_MASK) >> WFG2_ACTION22_RESERVED1_SHIFT)
#define  WFG2_ACTION22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION22_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION22_RESERVED1_SHIFT))

#define  WFG2_ACTION22_ACTION_MASK 0x30000000
#define  WFG2_ACTION22_ACTION_SHIFT 28
#define  WFG2_ACTION22_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION22_ACTION_MASK) >> WFG2_ACTION22_ACTION_SHIFT)
#define  WFG2_ACTION22_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION22_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION22_ACTION_SHIFT))

#define  WFG2_ACTION22_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION22_RESERVED2_SHIFT 24
#define  WFG2_ACTION22_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION22_RESERVED2_MASK) >> WFG2_ACTION22_RESERVED2_SHIFT)
#define  WFG2_ACTION22_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION22_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION22_RESERVED2_SHIFT))

#define  WFG2_ACTION22_COUNT_MASK 0xffffff
#define  WFG2_ACTION22_COUNT_SHIFT 0
#define  WFG2_ACTION22_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION22_COUNT_MASK) >> WFG2_ACTION22_COUNT_SHIFT)
#define  WFG2_ACTION22_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION22_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION22_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 4 of 10) (action23)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG2_ACTION23_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION23_RESERVED1_SHIFT 30
#define  WFG2_ACTION23_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION23_RESERVED1_MASK) >> WFG2_ACTION23_RESERVED1_SHIFT)
#define  WFG2_ACTION23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION23_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION23_RESERVED1_SHIFT))

#define  WFG2_ACTION23_ACTION_MASK 0x30000000
#define  WFG2_ACTION23_ACTION_SHIFT 28
#define  WFG2_ACTION23_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION23_ACTION_MASK) >> WFG2_ACTION23_ACTION_SHIFT)
#define  WFG2_ACTION23_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION23_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION23_ACTION_SHIFT))

#define  WFG2_ACTION23_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION23_RESERVED2_SHIFT 24
#define  WFG2_ACTION23_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION23_RESERVED2_MASK) >> WFG2_ACTION23_RESERVED2_SHIFT)
#define  WFG2_ACTION23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION23_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION23_RESERVED2_SHIFT))

#define  WFG2_ACTION23_COUNT_MASK 0xffffff
#define  WFG2_ACTION23_COUNT_SHIFT 0
#define  WFG2_ACTION23_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION23_COUNT_MASK) >> WFG2_ACTION23_COUNT_SHIFT)
#define  WFG2_ACTION23_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION23_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION23_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 5 of 10) (action24)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG2_ACTION24_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION24_RESERVED1_SHIFT 30
#define  WFG2_ACTION24_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION24_RESERVED1_MASK) >> WFG2_ACTION24_RESERVED1_SHIFT)
#define  WFG2_ACTION24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION24_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION24_RESERVED1_SHIFT))

#define  WFG2_ACTION24_ACTION_MASK 0x30000000
#define  WFG2_ACTION24_ACTION_SHIFT 28
#define  WFG2_ACTION24_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION24_ACTION_MASK) >> WFG2_ACTION24_ACTION_SHIFT)
#define  WFG2_ACTION24_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION24_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION24_ACTION_SHIFT))

#define  WFG2_ACTION24_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION24_RESERVED2_SHIFT 24
#define  WFG2_ACTION24_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION24_RESERVED2_MASK) >> WFG2_ACTION24_RESERVED2_SHIFT)
#define  WFG2_ACTION24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION24_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION24_RESERVED2_SHIFT))

#define  WFG2_ACTION24_COUNT_MASK 0xffffff
#define  WFG2_ACTION24_COUNT_SHIFT 0
#define  WFG2_ACTION24_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION24_COUNT_MASK) >> WFG2_ACTION24_COUNT_SHIFT)
#define  WFG2_ACTION24_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION24_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION24_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 6 of 10) (action25)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG2_ACTION25_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION25_RESERVED1_SHIFT 30
#define  WFG2_ACTION25_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION25_RESERVED1_MASK) >> WFG2_ACTION25_RESERVED1_SHIFT)
#define  WFG2_ACTION25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION25_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION25_RESERVED1_SHIFT))

#define  WFG2_ACTION25_ACTION_MASK 0x30000000
#define  WFG2_ACTION25_ACTION_SHIFT 28
#define  WFG2_ACTION25_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION25_ACTION_MASK) >> WFG2_ACTION25_ACTION_SHIFT)
#define  WFG2_ACTION25_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION25_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION25_ACTION_SHIFT))

#define  WFG2_ACTION25_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION25_RESERVED2_SHIFT 24
#define  WFG2_ACTION25_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION25_RESERVED2_MASK) >> WFG2_ACTION25_RESERVED2_SHIFT)
#define  WFG2_ACTION25_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION25_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION25_RESERVED2_SHIFT))

#define  WFG2_ACTION25_COUNT_MASK 0xffffff
#define  WFG2_ACTION25_COUNT_SHIFT 0
#define  WFG2_ACTION25_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION25_COUNT_MASK) >> WFG2_ACTION25_COUNT_SHIFT)
#define  WFG2_ACTION25_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION25_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION25_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 7 of 10) (action26)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG2_ACTION26_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION26_RESERVED1_SHIFT 30
#define  WFG2_ACTION26_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION26_RESERVED1_MASK) >> WFG2_ACTION26_RESERVED1_SHIFT)
#define  WFG2_ACTION26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION26_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION26_RESERVED1_SHIFT))

#define  WFG2_ACTION26_ACTION_MASK 0x30000000
#define  WFG2_ACTION26_ACTION_SHIFT 28
#define  WFG2_ACTION26_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION26_ACTION_MASK) >> WFG2_ACTION26_ACTION_SHIFT)
#define  WFG2_ACTION26_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION26_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION26_ACTION_SHIFT))

#define  WFG2_ACTION26_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION26_RESERVED2_SHIFT 24
#define  WFG2_ACTION26_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION26_RESERVED2_MASK) >> WFG2_ACTION26_RESERVED2_SHIFT)
#define  WFG2_ACTION26_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION26_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION26_RESERVED2_SHIFT))

#define  WFG2_ACTION26_COUNT_MASK 0xffffff
#define  WFG2_ACTION26_COUNT_SHIFT 0
#define  WFG2_ACTION26_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION26_COUNT_MASK) >> WFG2_ACTION26_COUNT_SHIFT)
#define  WFG2_ACTION26_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION26_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION26_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 8 of 10) (action27)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG2_ACTION27_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION27_RESERVED1_SHIFT 30
#define  WFG2_ACTION27_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION27_RESERVED1_MASK) >> WFG2_ACTION27_RESERVED1_SHIFT)
#define  WFG2_ACTION27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION27_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION27_RESERVED1_SHIFT))

#define  WFG2_ACTION27_ACTION_MASK 0x30000000
#define  WFG2_ACTION27_ACTION_SHIFT 28
#define  WFG2_ACTION27_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION27_ACTION_MASK) >> WFG2_ACTION27_ACTION_SHIFT)
#define  WFG2_ACTION27_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION27_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION27_ACTION_SHIFT))

#define  WFG2_ACTION27_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION27_RESERVED2_SHIFT 24
#define  WFG2_ACTION27_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION27_RESERVED2_MASK) >> WFG2_ACTION27_RESERVED2_SHIFT)
#define  WFG2_ACTION27_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION27_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION27_RESERVED2_SHIFT))

#define  WFG2_ACTION27_COUNT_MASK 0xffffff
#define  WFG2_ACTION27_COUNT_SHIFT 0
#define  WFG2_ACTION27_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION27_COUNT_MASK) >> WFG2_ACTION27_COUNT_SHIFT)
#define  WFG2_ACTION27_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION27_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION27_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 9 of 10) (action28)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG2_ACTION28_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION28_RESERVED1_SHIFT 30
#define  WFG2_ACTION28_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION28_RESERVED1_MASK) >> WFG2_ACTION28_RESERVED1_SHIFT)
#define  WFG2_ACTION28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION28_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION28_RESERVED1_SHIFT))

#define  WFG2_ACTION28_ACTION_MASK 0x30000000
#define  WFG2_ACTION28_ACTION_SHIFT 28
#define  WFG2_ACTION28_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION28_ACTION_MASK) >> WFG2_ACTION28_ACTION_SHIFT)
#define  WFG2_ACTION28_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION28_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION28_ACTION_SHIFT))

#define  WFG2_ACTION28_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION28_RESERVED2_SHIFT 24
#define  WFG2_ACTION28_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION28_RESERVED2_MASK) >> WFG2_ACTION28_RESERVED2_SHIFT)
#define  WFG2_ACTION28_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION28_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION28_RESERVED2_SHIFT))

#define  WFG2_ACTION28_COUNT_MASK 0xffffff
#define  WFG2_ACTION28_COUNT_SHIFT 0
#define  WFG2_ACTION28_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION28_COUNT_MASK) >> WFG2_ACTION28_COUNT_SHIFT)
#define  WFG2_ACTION28_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION28_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION28_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 10 of 10) (action29)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG2_ACTION29_RESERVED1_MASK 0xc0000000
#define  WFG2_ACTION29_RESERVED1_SHIFT 30
#define  WFG2_ACTION29_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG2_ACTION29_RESERVED1_MASK) >> WFG2_ACTION29_RESERVED1_SHIFT)
#define  WFG2_ACTION29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION29_RESERVED1_MASK) | (((uint32_t)val) << WFG2_ACTION29_RESERVED1_SHIFT))

#define  WFG2_ACTION29_ACTION_MASK 0x30000000
#define  WFG2_ACTION29_ACTION_SHIFT 28
#define  WFG2_ACTION29_ACTION_MASK_SHIFT(reg) (((reg) & WFG2_ACTION29_ACTION_MASK) >> WFG2_ACTION29_ACTION_SHIFT)
#define  WFG2_ACTION29_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION29_ACTION_MASK) | (((uint32_t)val) << WFG2_ACTION29_ACTION_SHIFT))

#define  WFG2_ACTION29_RESERVED2_MASK 0xf000000
#define  WFG2_ACTION29_RESERVED2_SHIFT 24
#define  WFG2_ACTION29_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG2_ACTION29_RESERVED2_MASK) >> WFG2_ACTION29_RESERVED2_SHIFT)
#define  WFG2_ACTION29_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION29_RESERVED2_MASK) | (((uint32_t)val) << WFG2_ACTION29_RESERVED2_SHIFT))

#define  WFG2_ACTION29_COUNT_MASK 0xffffff
#define  WFG2_ACTION29_COUNT_SHIFT 0
#define  WFG2_ACTION29_COUNT_MASK_SHIFT(reg) (((reg) & WFG2_ACTION29_COUNT_MASK) >> WFG2_ACTION29_COUNT_SHIFT)
#define  WFG2_ACTION29_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG2_ACTION29_COUNT_MASK) | (((uint32_t)val) << WFG2_ACTION29_COUNT_SHIFT))

//====================================================================
//
//Register File: Waverform Generator Control (Instance 4 of 4) (wfg3)
//
//====================================================================

//====================================================================
//Register: Waveform Generator Configuration register (scfg)
//====================================================================

#define  WFG3_SCFG_MODE_MASK 0xc0000000
#define  WFG3_SCFG_MODE_SHIFT 30
#define  WFG3_SCFG_MODE_MASK_SHIFT(reg) (((reg) & WFG3_SCFG_MODE_MASK) >> WFG3_SCFG_MODE_SHIFT)
#define  WFG3_SCFG_MODE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_SCFG_MODE_MASK) | (((uint32_t)val) << WFG3_SCFG_MODE_SHIFT))

#define  WFG3_SCFG_RECOV_MASK 0x20000000
#define  WFG3_SCFG_RECOV_SHIFT 29
#define  WFG3_SCFG_RECOV_MASK_SHIFT(reg) (((reg) & WFG3_SCFG_RECOV_MASK) >> WFG3_SCFG_RECOV_SHIFT)
#define  WFG3_SCFG_RECOV_REPLACE_VAL(reg,val) (((reg) & ~WFG3_SCFG_RECOV_MASK) | (((uint32_t)val) << WFG3_SCFG_RECOV_SHIFT))

#define  WFG3_SCFG_SYNC_SEL_MASK 0x10000000
#define  WFG3_SCFG_SYNC_SEL_SHIFT 28
#define  WFG3_SCFG_SYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG3_SCFG_SYNC_SEL_MASK) >> WFG3_SCFG_SYNC_SEL_SHIFT)
#define  WFG3_SCFG_SYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG3_SCFG_SYNC_SEL_MASK) | (((uint32_t)val) << WFG3_SCFG_SYNC_SEL_SHIFT))

#define  WFG3_SCFG_GALVO_SEL_MASK 0xc000000
#define  WFG3_SCFG_GALVO_SEL_SHIFT 26
#define  WFG3_SCFG_GALVO_SEL_MASK_SHIFT(reg) (((reg) & WFG3_SCFG_GALVO_SEL_MASK) >> WFG3_SCFG_GALVO_SEL_SHIFT)
#define  WFG3_SCFG_GALVO_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG3_SCFG_GALVO_SEL_MASK) | (((uint32_t)val) << WFG3_SCFG_GALVO_SEL_SHIFT))

#define  WFG3_SCFG_HSYNC_SEL_MASK 0x3000000
#define  WFG3_SCFG_HSYNC_SEL_SHIFT 24
#define  WFG3_SCFG_HSYNC_SEL_MASK_SHIFT(reg) (((reg) & WFG3_SCFG_HSYNC_SEL_MASK) >> WFG3_SCFG_HSYNC_SEL_SHIFT)
#define  WFG3_SCFG_HSYNC_SEL_REPLACE_VAL(reg,val) (((reg) & ~WFG3_SCFG_HSYNC_SEL_MASK) | (((uint32_t)val) << WFG3_SCFG_HSYNC_SEL_SHIFT))

#define  WFG3_SCFG_COUNT_MASK 0xffffff
#define  WFG3_SCFG_COUNT_SHIFT 0
#define  WFG3_SCFG_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_SCFG_COUNT_MASK) >> WFG3_SCFG_COUNT_SHIFT)
#define  WFG3_SCFG_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_SCFG_COUNT_MASK) | (((uint32_t)val) << WFG3_SCFG_COUNT_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 1 of 10) (period0)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG3_PERIOD0_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD0_RESERVED1_SHIFT 17
#define  WFG3_PERIOD0_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD0_RESERVED1_MASK) >> WFG3_PERIOD0_RESERVED1_SHIFT)
#define  WFG3_PERIOD0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD0_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD0_RESERVED1_SHIFT))

#define  WFG3_PERIOD0_IDLE_MASK 0x10000
#define  WFG3_PERIOD0_IDLE_SHIFT 16
#define  WFG3_PERIOD0_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD0_IDLE_MASK) >> WFG3_PERIOD0_IDLE_SHIFT)
#define  WFG3_PERIOD0_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD0_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD0_IDLE_SHIFT))

#define  WFG3_PERIOD0_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD0_ACTIVE_SHIFT 8
#define  WFG3_PERIOD0_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD0_ACTIVE_MASK) >> WFG3_PERIOD0_ACTIVE_SHIFT)
#define  WFG3_PERIOD0_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD0_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD0_ACTIVE_SHIFT))

#define  WFG3_PERIOD0_PERIOD_MASK 0xff
#define  WFG3_PERIOD0_PERIOD_SHIFT 0
#define  WFG3_PERIOD0_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD0_PERIOD_MASK) >> WFG3_PERIOD0_PERIOD_SHIFT)
#define  WFG3_PERIOD0_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD0_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD0_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 2 of 10) (period1)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG3_PERIOD1_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD1_RESERVED1_SHIFT 17
#define  WFG3_PERIOD1_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD1_RESERVED1_MASK) >> WFG3_PERIOD1_RESERVED1_SHIFT)
#define  WFG3_PERIOD1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD1_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD1_RESERVED1_SHIFT))

#define  WFG3_PERIOD1_IDLE_MASK 0x10000
#define  WFG3_PERIOD1_IDLE_SHIFT 16
#define  WFG3_PERIOD1_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD1_IDLE_MASK) >> WFG3_PERIOD1_IDLE_SHIFT)
#define  WFG3_PERIOD1_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD1_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD1_IDLE_SHIFT))

#define  WFG3_PERIOD1_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD1_ACTIVE_SHIFT 8
#define  WFG3_PERIOD1_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD1_ACTIVE_MASK) >> WFG3_PERIOD1_ACTIVE_SHIFT)
#define  WFG3_PERIOD1_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD1_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD1_ACTIVE_SHIFT))

#define  WFG3_PERIOD1_PERIOD_MASK 0xff
#define  WFG3_PERIOD1_PERIOD_SHIFT 0
#define  WFG3_PERIOD1_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD1_PERIOD_MASK) >> WFG3_PERIOD1_PERIOD_SHIFT)
#define  WFG3_PERIOD1_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD1_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD1_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 3 of 10) (period2)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG3_PERIOD2_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD2_RESERVED1_SHIFT 17
#define  WFG3_PERIOD2_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD2_RESERVED1_MASK) >> WFG3_PERIOD2_RESERVED1_SHIFT)
#define  WFG3_PERIOD2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD2_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD2_RESERVED1_SHIFT))

#define  WFG3_PERIOD2_IDLE_MASK 0x10000
#define  WFG3_PERIOD2_IDLE_SHIFT 16
#define  WFG3_PERIOD2_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD2_IDLE_MASK) >> WFG3_PERIOD2_IDLE_SHIFT)
#define  WFG3_PERIOD2_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD2_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD2_IDLE_SHIFT))

#define  WFG3_PERIOD2_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD2_ACTIVE_SHIFT 8
#define  WFG3_PERIOD2_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD2_ACTIVE_MASK) >> WFG3_PERIOD2_ACTIVE_SHIFT)
#define  WFG3_PERIOD2_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD2_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD2_ACTIVE_SHIFT))

#define  WFG3_PERIOD2_PERIOD_MASK 0xff
#define  WFG3_PERIOD2_PERIOD_SHIFT 0
#define  WFG3_PERIOD2_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD2_PERIOD_MASK) >> WFG3_PERIOD2_PERIOD_SHIFT)
#define  WFG3_PERIOD2_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD2_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD2_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 4 of 10) (period3)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG3_PERIOD3_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD3_RESERVED1_SHIFT 17
#define  WFG3_PERIOD3_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD3_RESERVED1_MASK) >> WFG3_PERIOD3_RESERVED1_SHIFT)
#define  WFG3_PERIOD3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD3_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD3_RESERVED1_SHIFT))

#define  WFG3_PERIOD3_IDLE_MASK 0x10000
#define  WFG3_PERIOD3_IDLE_SHIFT 16
#define  WFG3_PERIOD3_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD3_IDLE_MASK) >> WFG3_PERIOD3_IDLE_SHIFT)
#define  WFG3_PERIOD3_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD3_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD3_IDLE_SHIFT))

#define  WFG3_PERIOD3_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD3_ACTIVE_SHIFT 8
#define  WFG3_PERIOD3_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD3_ACTIVE_MASK) >> WFG3_PERIOD3_ACTIVE_SHIFT)
#define  WFG3_PERIOD3_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD3_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD3_ACTIVE_SHIFT))

#define  WFG3_PERIOD3_PERIOD_MASK 0xff
#define  WFG3_PERIOD3_PERIOD_SHIFT 0
#define  WFG3_PERIOD3_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD3_PERIOD_MASK) >> WFG3_PERIOD3_PERIOD_SHIFT)
#define  WFG3_PERIOD3_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD3_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD3_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 5 of 10) (period4)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG3_PERIOD4_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD4_RESERVED1_SHIFT 17
#define  WFG3_PERIOD4_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD4_RESERVED1_MASK) >> WFG3_PERIOD4_RESERVED1_SHIFT)
#define  WFG3_PERIOD4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD4_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD4_RESERVED1_SHIFT))

#define  WFG3_PERIOD4_IDLE_MASK 0x10000
#define  WFG3_PERIOD4_IDLE_SHIFT 16
#define  WFG3_PERIOD4_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD4_IDLE_MASK) >> WFG3_PERIOD4_IDLE_SHIFT)
#define  WFG3_PERIOD4_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD4_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD4_IDLE_SHIFT))

#define  WFG3_PERIOD4_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD4_ACTIVE_SHIFT 8
#define  WFG3_PERIOD4_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD4_ACTIVE_MASK) >> WFG3_PERIOD4_ACTIVE_SHIFT)
#define  WFG3_PERIOD4_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD4_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD4_ACTIVE_SHIFT))

#define  WFG3_PERIOD4_PERIOD_MASK 0xff
#define  WFG3_PERIOD4_PERIOD_SHIFT 0
#define  WFG3_PERIOD4_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD4_PERIOD_MASK) >> WFG3_PERIOD4_PERIOD_SHIFT)
#define  WFG3_PERIOD4_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD4_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD4_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 6 of 10) (period5)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG3_PERIOD5_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD5_RESERVED1_SHIFT 17
#define  WFG3_PERIOD5_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD5_RESERVED1_MASK) >> WFG3_PERIOD5_RESERVED1_SHIFT)
#define  WFG3_PERIOD5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD5_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD5_RESERVED1_SHIFT))

#define  WFG3_PERIOD5_IDLE_MASK 0x10000
#define  WFG3_PERIOD5_IDLE_SHIFT 16
#define  WFG3_PERIOD5_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD5_IDLE_MASK) >> WFG3_PERIOD5_IDLE_SHIFT)
#define  WFG3_PERIOD5_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD5_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD5_IDLE_SHIFT))

#define  WFG3_PERIOD5_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD5_ACTIVE_SHIFT 8
#define  WFG3_PERIOD5_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD5_ACTIVE_MASK) >> WFG3_PERIOD5_ACTIVE_SHIFT)
#define  WFG3_PERIOD5_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD5_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD5_ACTIVE_SHIFT))

#define  WFG3_PERIOD5_PERIOD_MASK 0xff
#define  WFG3_PERIOD5_PERIOD_SHIFT 0
#define  WFG3_PERIOD5_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD5_PERIOD_MASK) >> WFG3_PERIOD5_PERIOD_SHIFT)
#define  WFG3_PERIOD5_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD5_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD5_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 7 of 10) (period6)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG3_PERIOD6_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD6_RESERVED1_SHIFT 17
#define  WFG3_PERIOD6_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD6_RESERVED1_MASK) >> WFG3_PERIOD6_RESERVED1_SHIFT)
#define  WFG3_PERIOD6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD6_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD6_RESERVED1_SHIFT))

#define  WFG3_PERIOD6_IDLE_MASK 0x10000
#define  WFG3_PERIOD6_IDLE_SHIFT 16
#define  WFG3_PERIOD6_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD6_IDLE_MASK) >> WFG3_PERIOD6_IDLE_SHIFT)
#define  WFG3_PERIOD6_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD6_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD6_IDLE_SHIFT))

#define  WFG3_PERIOD6_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD6_ACTIVE_SHIFT 8
#define  WFG3_PERIOD6_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD6_ACTIVE_MASK) >> WFG3_PERIOD6_ACTIVE_SHIFT)
#define  WFG3_PERIOD6_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD6_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD6_ACTIVE_SHIFT))

#define  WFG3_PERIOD6_PERIOD_MASK 0xff
#define  WFG3_PERIOD6_PERIOD_SHIFT 0
#define  WFG3_PERIOD6_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD6_PERIOD_MASK) >> WFG3_PERIOD6_PERIOD_SHIFT)
#define  WFG3_PERIOD6_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD6_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD6_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 8 of 10) (period7)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG3_PERIOD7_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD7_RESERVED1_SHIFT 17
#define  WFG3_PERIOD7_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD7_RESERVED1_MASK) >> WFG3_PERIOD7_RESERVED1_SHIFT)
#define  WFG3_PERIOD7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD7_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD7_RESERVED1_SHIFT))

#define  WFG3_PERIOD7_IDLE_MASK 0x10000
#define  WFG3_PERIOD7_IDLE_SHIFT 16
#define  WFG3_PERIOD7_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD7_IDLE_MASK) >> WFG3_PERIOD7_IDLE_SHIFT)
#define  WFG3_PERIOD7_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD7_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD7_IDLE_SHIFT))

#define  WFG3_PERIOD7_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD7_ACTIVE_SHIFT 8
#define  WFG3_PERIOD7_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD7_ACTIVE_MASK) >> WFG3_PERIOD7_ACTIVE_SHIFT)
#define  WFG3_PERIOD7_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD7_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD7_ACTIVE_SHIFT))

#define  WFG3_PERIOD7_PERIOD_MASK 0xff
#define  WFG3_PERIOD7_PERIOD_SHIFT 0
#define  WFG3_PERIOD7_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD7_PERIOD_MASK) >> WFG3_PERIOD7_PERIOD_SHIFT)
#define  WFG3_PERIOD7_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD7_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD7_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 9 of 10) (period8)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG3_PERIOD8_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD8_RESERVED1_SHIFT 17
#define  WFG3_PERIOD8_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD8_RESERVED1_MASK) >> WFG3_PERIOD8_RESERVED1_SHIFT)
#define  WFG3_PERIOD8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD8_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD8_RESERVED1_SHIFT))

#define  WFG3_PERIOD8_IDLE_MASK 0x10000
#define  WFG3_PERIOD8_IDLE_SHIFT 16
#define  WFG3_PERIOD8_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD8_IDLE_MASK) >> WFG3_PERIOD8_IDLE_SHIFT)
#define  WFG3_PERIOD8_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD8_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD8_IDLE_SHIFT))

#define  WFG3_PERIOD8_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD8_ACTIVE_SHIFT 8
#define  WFG3_PERIOD8_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD8_ACTIVE_MASK) >> WFG3_PERIOD8_ACTIVE_SHIFT)
#define  WFG3_PERIOD8_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD8_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD8_ACTIVE_SHIFT))

#define  WFG3_PERIOD8_PERIOD_MASK 0xff
#define  WFG3_PERIOD8_PERIOD_SHIFT 0
#define  WFG3_PERIOD8_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD8_PERIOD_MASK) >> WFG3_PERIOD8_PERIOD_SHIFT)
#define  WFG3_PERIOD8_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD8_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD8_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Control register (Instance 10 of 10) (period9)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG3_PERIOD9_RESERVED1_MASK 0xfffe0000
#define  WFG3_PERIOD9_RESERVED1_SHIFT 17
#define  WFG3_PERIOD9_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD9_RESERVED1_MASK) >> WFG3_PERIOD9_RESERVED1_SHIFT)
#define  WFG3_PERIOD9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD9_RESERVED1_MASK) | (((uint32_t)val) << WFG3_PERIOD9_RESERVED1_SHIFT))

#define  WFG3_PERIOD9_IDLE_MASK 0x10000
#define  WFG3_PERIOD9_IDLE_SHIFT 16
#define  WFG3_PERIOD9_IDLE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD9_IDLE_MASK) >> WFG3_PERIOD9_IDLE_SHIFT)
#define  WFG3_PERIOD9_IDLE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD9_IDLE_MASK) | (((uint32_t)val) << WFG3_PERIOD9_IDLE_SHIFT))

#define  WFG3_PERIOD9_ACTIVE_MASK 0xff00
#define  WFG3_PERIOD9_ACTIVE_SHIFT 8
#define  WFG3_PERIOD9_ACTIVE_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD9_ACTIVE_MASK) >> WFG3_PERIOD9_ACTIVE_SHIFT)
#define  WFG3_PERIOD9_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD9_ACTIVE_MASK) | (((uint32_t)val) << WFG3_PERIOD9_ACTIVE_SHIFT))

#define  WFG3_PERIOD9_PERIOD_MASK 0xff
#define  WFG3_PERIOD9_PERIOD_SHIFT 0
#define  WFG3_PERIOD9_PERIOD_MASK_SHIFT(reg) (((reg) & WFG3_PERIOD9_PERIOD_MASK) >> WFG3_PERIOD9_PERIOD_SHIFT)
#define  WFG3_PERIOD9_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~WFG3_PERIOD9_PERIOD_MASK) | (((uint32_t)val) << WFG3_PERIOD9_PERIOD_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 1 of 10) (action10)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG3_ACTION10_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION10_RESERVED1_SHIFT 30
#define  WFG3_ACTION10_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION10_RESERVED1_MASK) >> WFG3_ACTION10_RESERVED1_SHIFT)
#define  WFG3_ACTION10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION10_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION10_RESERVED1_SHIFT))

#define  WFG3_ACTION10_ACTION_MASK 0x30000000
#define  WFG3_ACTION10_ACTION_SHIFT 28
#define  WFG3_ACTION10_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION10_ACTION_MASK) >> WFG3_ACTION10_ACTION_SHIFT)
#define  WFG3_ACTION10_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION10_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION10_ACTION_SHIFT))

#define  WFG3_ACTION10_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION10_RESERVED2_SHIFT 24
#define  WFG3_ACTION10_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION10_RESERVED2_MASK) >> WFG3_ACTION10_RESERVED2_SHIFT)
#define  WFG3_ACTION10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION10_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION10_RESERVED2_SHIFT))

#define  WFG3_ACTION10_COUNT_MASK 0xffffff
#define  WFG3_ACTION10_COUNT_SHIFT 0
#define  WFG3_ACTION10_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION10_COUNT_MASK) >> WFG3_ACTION10_COUNT_SHIFT)
#define  WFG3_ACTION10_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION10_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION10_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 2 of 10) (action11)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG3_ACTION11_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION11_RESERVED1_SHIFT 30
#define  WFG3_ACTION11_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION11_RESERVED1_MASK) >> WFG3_ACTION11_RESERVED1_SHIFT)
#define  WFG3_ACTION11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION11_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION11_RESERVED1_SHIFT))

#define  WFG3_ACTION11_ACTION_MASK 0x30000000
#define  WFG3_ACTION11_ACTION_SHIFT 28
#define  WFG3_ACTION11_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION11_ACTION_MASK) >> WFG3_ACTION11_ACTION_SHIFT)
#define  WFG3_ACTION11_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION11_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION11_ACTION_SHIFT))

#define  WFG3_ACTION11_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION11_RESERVED2_SHIFT 24
#define  WFG3_ACTION11_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION11_RESERVED2_MASK) >> WFG3_ACTION11_RESERVED2_SHIFT)
#define  WFG3_ACTION11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION11_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION11_RESERVED2_SHIFT))

#define  WFG3_ACTION11_COUNT_MASK 0xffffff
#define  WFG3_ACTION11_COUNT_SHIFT 0
#define  WFG3_ACTION11_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION11_COUNT_MASK) >> WFG3_ACTION11_COUNT_SHIFT)
#define  WFG3_ACTION11_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION11_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION11_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 3 of 10) (action12)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG3_ACTION12_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION12_RESERVED1_SHIFT 30
#define  WFG3_ACTION12_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION12_RESERVED1_MASK) >> WFG3_ACTION12_RESERVED1_SHIFT)
#define  WFG3_ACTION12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION12_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION12_RESERVED1_SHIFT))

#define  WFG3_ACTION12_ACTION_MASK 0x30000000
#define  WFG3_ACTION12_ACTION_SHIFT 28
#define  WFG3_ACTION12_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION12_ACTION_MASK) >> WFG3_ACTION12_ACTION_SHIFT)
#define  WFG3_ACTION12_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION12_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION12_ACTION_SHIFT))

#define  WFG3_ACTION12_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION12_RESERVED2_SHIFT 24
#define  WFG3_ACTION12_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION12_RESERVED2_MASK) >> WFG3_ACTION12_RESERVED2_SHIFT)
#define  WFG3_ACTION12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION12_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION12_RESERVED2_SHIFT))

#define  WFG3_ACTION12_COUNT_MASK 0xffffff
#define  WFG3_ACTION12_COUNT_SHIFT 0
#define  WFG3_ACTION12_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION12_COUNT_MASK) >> WFG3_ACTION12_COUNT_SHIFT)
#define  WFG3_ACTION12_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION12_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION12_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 4 of 10) (action13)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG3_ACTION13_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION13_RESERVED1_SHIFT 30
#define  WFG3_ACTION13_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION13_RESERVED1_MASK) >> WFG3_ACTION13_RESERVED1_SHIFT)
#define  WFG3_ACTION13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION13_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION13_RESERVED1_SHIFT))

#define  WFG3_ACTION13_ACTION_MASK 0x30000000
#define  WFG3_ACTION13_ACTION_SHIFT 28
#define  WFG3_ACTION13_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION13_ACTION_MASK) >> WFG3_ACTION13_ACTION_SHIFT)
#define  WFG3_ACTION13_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION13_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION13_ACTION_SHIFT))

#define  WFG3_ACTION13_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION13_RESERVED2_SHIFT 24
#define  WFG3_ACTION13_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION13_RESERVED2_MASK) >> WFG3_ACTION13_RESERVED2_SHIFT)
#define  WFG3_ACTION13_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION13_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION13_RESERVED2_SHIFT))

#define  WFG3_ACTION13_COUNT_MASK 0xffffff
#define  WFG3_ACTION13_COUNT_SHIFT 0
#define  WFG3_ACTION13_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION13_COUNT_MASK) >> WFG3_ACTION13_COUNT_SHIFT)
#define  WFG3_ACTION13_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION13_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION13_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 5 of 10) (action14)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG3_ACTION14_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION14_RESERVED1_SHIFT 30
#define  WFG3_ACTION14_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION14_RESERVED1_MASK) >> WFG3_ACTION14_RESERVED1_SHIFT)
#define  WFG3_ACTION14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION14_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION14_RESERVED1_SHIFT))

#define  WFG3_ACTION14_ACTION_MASK 0x30000000
#define  WFG3_ACTION14_ACTION_SHIFT 28
#define  WFG3_ACTION14_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION14_ACTION_MASK) >> WFG3_ACTION14_ACTION_SHIFT)
#define  WFG3_ACTION14_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION14_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION14_ACTION_SHIFT))

#define  WFG3_ACTION14_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION14_RESERVED2_SHIFT 24
#define  WFG3_ACTION14_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION14_RESERVED2_MASK) >> WFG3_ACTION14_RESERVED2_SHIFT)
#define  WFG3_ACTION14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION14_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION14_RESERVED2_SHIFT))

#define  WFG3_ACTION14_COUNT_MASK 0xffffff
#define  WFG3_ACTION14_COUNT_SHIFT 0
#define  WFG3_ACTION14_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION14_COUNT_MASK) >> WFG3_ACTION14_COUNT_SHIFT)
#define  WFG3_ACTION14_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION14_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION14_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 6 of 10) (action15)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG3_ACTION15_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION15_RESERVED1_SHIFT 30
#define  WFG3_ACTION15_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION15_RESERVED1_MASK) >> WFG3_ACTION15_RESERVED1_SHIFT)
#define  WFG3_ACTION15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION15_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION15_RESERVED1_SHIFT))

#define  WFG3_ACTION15_ACTION_MASK 0x30000000
#define  WFG3_ACTION15_ACTION_SHIFT 28
#define  WFG3_ACTION15_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION15_ACTION_MASK) >> WFG3_ACTION15_ACTION_SHIFT)
#define  WFG3_ACTION15_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION15_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION15_ACTION_SHIFT))

#define  WFG3_ACTION15_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION15_RESERVED2_SHIFT 24
#define  WFG3_ACTION15_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION15_RESERVED2_MASK) >> WFG3_ACTION15_RESERVED2_SHIFT)
#define  WFG3_ACTION15_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION15_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION15_RESERVED2_SHIFT))

#define  WFG3_ACTION15_COUNT_MASK 0xffffff
#define  WFG3_ACTION15_COUNT_SHIFT 0
#define  WFG3_ACTION15_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION15_COUNT_MASK) >> WFG3_ACTION15_COUNT_SHIFT)
#define  WFG3_ACTION15_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION15_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION15_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 7 of 10) (action16)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG3_ACTION16_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION16_RESERVED1_SHIFT 30
#define  WFG3_ACTION16_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION16_RESERVED1_MASK) >> WFG3_ACTION16_RESERVED1_SHIFT)
#define  WFG3_ACTION16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION16_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION16_RESERVED1_SHIFT))

#define  WFG3_ACTION16_ACTION_MASK 0x30000000
#define  WFG3_ACTION16_ACTION_SHIFT 28
#define  WFG3_ACTION16_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION16_ACTION_MASK) >> WFG3_ACTION16_ACTION_SHIFT)
#define  WFG3_ACTION16_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION16_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION16_ACTION_SHIFT))

#define  WFG3_ACTION16_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION16_RESERVED2_SHIFT 24
#define  WFG3_ACTION16_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION16_RESERVED2_MASK) >> WFG3_ACTION16_RESERVED2_SHIFT)
#define  WFG3_ACTION16_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION16_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION16_RESERVED2_SHIFT))

#define  WFG3_ACTION16_COUNT_MASK 0xffffff
#define  WFG3_ACTION16_COUNT_SHIFT 0
#define  WFG3_ACTION16_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION16_COUNT_MASK) >> WFG3_ACTION16_COUNT_SHIFT)
#define  WFG3_ACTION16_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION16_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION16_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 8 of 10) (action17)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG3_ACTION17_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION17_RESERVED1_SHIFT 30
#define  WFG3_ACTION17_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION17_RESERVED1_MASK) >> WFG3_ACTION17_RESERVED1_SHIFT)
#define  WFG3_ACTION17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION17_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION17_RESERVED1_SHIFT))

#define  WFG3_ACTION17_ACTION_MASK 0x30000000
#define  WFG3_ACTION17_ACTION_SHIFT 28
#define  WFG3_ACTION17_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION17_ACTION_MASK) >> WFG3_ACTION17_ACTION_SHIFT)
#define  WFG3_ACTION17_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION17_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION17_ACTION_SHIFT))

#define  WFG3_ACTION17_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION17_RESERVED2_SHIFT 24
#define  WFG3_ACTION17_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION17_RESERVED2_MASK) >> WFG3_ACTION17_RESERVED2_SHIFT)
#define  WFG3_ACTION17_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION17_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION17_RESERVED2_SHIFT))

#define  WFG3_ACTION17_COUNT_MASK 0xffffff
#define  WFG3_ACTION17_COUNT_SHIFT 0
#define  WFG3_ACTION17_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION17_COUNT_MASK) >> WFG3_ACTION17_COUNT_SHIFT)
#define  WFG3_ACTION17_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION17_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION17_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 9 of 10) (action18)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG3_ACTION18_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION18_RESERVED1_SHIFT 30
#define  WFG3_ACTION18_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION18_RESERVED1_MASK) >> WFG3_ACTION18_RESERVED1_SHIFT)
#define  WFG3_ACTION18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION18_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION18_RESERVED1_SHIFT))

#define  WFG3_ACTION18_ACTION_MASK 0x30000000
#define  WFG3_ACTION18_ACTION_SHIFT 28
#define  WFG3_ACTION18_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION18_ACTION_MASK) >> WFG3_ACTION18_ACTION_SHIFT)
#define  WFG3_ACTION18_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION18_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION18_ACTION_SHIFT))

#define  WFG3_ACTION18_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION18_RESERVED2_SHIFT 24
#define  WFG3_ACTION18_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION18_RESERVED2_MASK) >> WFG3_ACTION18_RESERVED2_SHIFT)
#define  WFG3_ACTION18_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION18_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION18_RESERVED2_SHIFT))

#define  WFG3_ACTION18_COUNT_MASK 0xffffff
#define  WFG3_ACTION18_COUNT_SHIFT 0
#define  WFG3_ACTION18_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION18_COUNT_MASK) >> WFG3_ACTION18_COUNT_SHIFT)
#define  WFG3_ACTION18_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION18_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION18_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 1 register (Instance 10 of 10) (action19)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG3_ACTION19_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION19_RESERVED1_SHIFT 30
#define  WFG3_ACTION19_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION19_RESERVED1_MASK) >> WFG3_ACTION19_RESERVED1_SHIFT)
#define  WFG3_ACTION19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION19_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION19_RESERVED1_SHIFT))

#define  WFG3_ACTION19_ACTION_MASK 0x30000000
#define  WFG3_ACTION19_ACTION_SHIFT 28
#define  WFG3_ACTION19_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION19_ACTION_MASK) >> WFG3_ACTION19_ACTION_SHIFT)
#define  WFG3_ACTION19_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION19_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION19_ACTION_SHIFT))

#define  WFG3_ACTION19_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION19_RESERVED2_SHIFT 24
#define  WFG3_ACTION19_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION19_RESERVED2_MASK) >> WFG3_ACTION19_RESERVED2_SHIFT)
#define  WFG3_ACTION19_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION19_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION19_RESERVED2_SHIFT))

#define  WFG3_ACTION19_COUNT_MASK 0xffffff
#define  WFG3_ACTION19_COUNT_SHIFT 0
#define  WFG3_ACTION19_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION19_COUNT_MASK) >> WFG3_ACTION19_COUNT_SHIFT)
#define  WFG3_ACTION19_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION19_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION19_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 1 of 10) (action20)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  WFG3_ACTION20_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION20_RESERVED1_SHIFT 30
#define  WFG3_ACTION20_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION20_RESERVED1_MASK) >> WFG3_ACTION20_RESERVED1_SHIFT)
#define  WFG3_ACTION20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION20_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION20_RESERVED1_SHIFT))

#define  WFG3_ACTION20_ACTION_MASK 0x30000000
#define  WFG3_ACTION20_ACTION_SHIFT 28
#define  WFG3_ACTION20_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION20_ACTION_MASK) >> WFG3_ACTION20_ACTION_SHIFT)
#define  WFG3_ACTION20_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION20_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION20_ACTION_SHIFT))

#define  WFG3_ACTION20_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION20_RESERVED2_SHIFT 24
#define  WFG3_ACTION20_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION20_RESERVED2_MASK) >> WFG3_ACTION20_RESERVED2_SHIFT)
#define  WFG3_ACTION20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION20_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION20_RESERVED2_SHIFT))

#define  WFG3_ACTION20_COUNT_MASK 0xffffff
#define  WFG3_ACTION20_COUNT_SHIFT 0
#define  WFG3_ACTION20_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION20_COUNT_MASK) >> WFG3_ACTION20_COUNT_SHIFT)
#define  WFG3_ACTION20_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION20_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION20_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 2 of 10) (action21)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  WFG3_ACTION21_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION21_RESERVED1_SHIFT 30
#define  WFG3_ACTION21_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION21_RESERVED1_MASK) >> WFG3_ACTION21_RESERVED1_SHIFT)
#define  WFG3_ACTION21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION21_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION21_RESERVED1_SHIFT))

#define  WFG3_ACTION21_ACTION_MASK 0x30000000
#define  WFG3_ACTION21_ACTION_SHIFT 28
#define  WFG3_ACTION21_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION21_ACTION_MASK) >> WFG3_ACTION21_ACTION_SHIFT)
#define  WFG3_ACTION21_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION21_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION21_ACTION_SHIFT))

#define  WFG3_ACTION21_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION21_RESERVED2_SHIFT 24
#define  WFG3_ACTION21_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION21_RESERVED2_MASK) >> WFG3_ACTION21_RESERVED2_SHIFT)
#define  WFG3_ACTION21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION21_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION21_RESERVED2_SHIFT))

#define  WFG3_ACTION21_COUNT_MASK 0xffffff
#define  WFG3_ACTION21_COUNT_SHIFT 0
#define  WFG3_ACTION21_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION21_COUNT_MASK) >> WFG3_ACTION21_COUNT_SHIFT)
#define  WFG3_ACTION21_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION21_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION21_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 3 of 10) (action22)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  WFG3_ACTION22_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION22_RESERVED1_SHIFT 30
#define  WFG3_ACTION22_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION22_RESERVED1_MASK) >> WFG3_ACTION22_RESERVED1_SHIFT)
#define  WFG3_ACTION22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION22_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION22_RESERVED1_SHIFT))

#define  WFG3_ACTION22_ACTION_MASK 0x30000000
#define  WFG3_ACTION22_ACTION_SHIFT 28
#define  WFG3_ACTION22_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION22_ACTION_MASK) >> WFG3_ACTION22_ACTION_SHIFT)
#define  WFG3_ACTION22_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION22_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION22_ACTION_SHIFT))

#define  WFG3_ACTION22_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION22_RESERVED2_SHIFT 24
#define  WFG3_ACTION22_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION22_RESERVED2_MASK) >> WFG3_ACTION22_RESERVED2_SHIFT)
#define  WFG3_ACTION22_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION22_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION22_RESERVED2_SHIFT))

#define  WFG3_ACTION22_COUNT_MASK 0xffffff
#define  WFG3_ACTION22_COUNT_SHIFT 0
#define  WFG3_ACTION22_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION22_COUNT_MASK) >> WFG3_ACTION22_COUNT_SHIFT)
#define  WFG3_ACTION22_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION22_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION22_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 4 of 10) (action23)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  WFG3_ACTION23_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION23_RESERVED1_SHIFT 30
#define  WFG3_ACTION23_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION23_RESERVED1_MASK) >> WFG3_ACTION23_RESERVED1_SHIFT)
#define  WFG3_ACTION23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION23_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION23_RESERVED1_SHIFT))

#define  WFG3_ACTION23_ACTION_MASK 0x30000000
#define  WFG3_ACTION23_ACTION_SHIFT 28
#define  WFG3_ACTION23_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION23_ACTION_MASK) >> WFG3_ACTION23_ACTION_SHIFT)
#define  WFG3_ACTION23_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION23_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION23_ACTION_SHIFT))

#define  WFG3_ACTION23_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION23_RESERVED2_SHIFT 24
#define  WFG3_ACTION23_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION23_RESERVED2_MASK) >> WFG3_ACTION23_RESERVED2_SHIFT)
#define  WFG3_ACTION23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION23_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION23_RESERVED2_SHIFT))

#define  WFG3_ACTION23_COUNT_MASK 0xffffff
#define  WFG3_ACTION23_COUNT_SHIFT 0
#define  WFG3_ACTION23_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION23_COUNT_MASK) >> WFG3_ACTION23_COUNT_SHIFT)
#define  WFG3_ACTION23_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION23_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION23_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 5 of 10) (action24)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  WFG3_ACTION24_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION24_RESERVED1_SHIFT 30
#define  WFG3_ACTION24_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION24_RESERVED1_MASK) >> WFG3_ACTION24_RESERVED1_SHIFT)
#define  WFG3_ACTION24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION24_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION24_RESERVED1_SHIFT))

#define  WFG3_ACTION24_ACTION_MASK 0x30000000
#define  WFG3_ACTION24_ACTION_SHIFT 28
#define  WFG3_ACTION24_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION24_ACTION_MASK) >> WFG3_ACTION24_ACTION_SHIFT)
#define  WFG3_ACTION24_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION24_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION24_ACTION_SHIFT))

#define  WFG3_ACTION24_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION24_RESERVED2_SHIFT 24
#define  WFG3_ACTION24_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION24_RESERVED2_MASK) >> WFG3_ACTION24_RESERVED2_SHIFT)
#define  WFG3_ACTION24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION24_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION24_RESERVED2_SHIFT))

#define  WFG3_ACTION24_COUNT_MASK 0xffffff
#define  WFG3_ACTION24_COUNT_SHIFT 0
#define  WFG3_ACTION24_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION24_COUNT_MASK) >> WFG3_ACTION24_COUNT_SHIFT)
#define  WFG3_ACTION24_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION24_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION24_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 6 of 10) (action25)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  WFG3_ACTION25_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION25_RESERVED1_SHIFT 30
#define  WFG3_ACTION25_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION25_RESERVED1_MASK) >> WFG3_ACTION25_RESERVED1_SHIFT)
#define  WFG3_ACTION25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION25_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION25_RESERVED1_SHIFT))

#define  WFG3_ACTION25_ACTION_MASK 0x30000000
#define  WFG3_ACTION25_ACTION_SHIFT 28
#define  WFG3_ACTION25_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION25_ACTION_MASK) >> WFG3_ACTION25_ACTION_SHIFT)
#define  WFG3_ACTION25_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION25_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION25_ACTION_SHIFT))

#define  WFG3_ACTION25_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION25_RESERVED2_SHIFT 24
#define  WFG3_ACTION25_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION25_RESERVED2_MASK) >> WFG3_ACTION25_RESERVED2_SHIFT)
#define  WFG3_ACTION25_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION25_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION25_RESERVED2_SHIFT))

#define  WFG3_ACTION25_COUNT_MASK 0xffffff
#define  WFG3_ACTION25_COUNT_SHIFT 0
#define  WFG3_ACTION25_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION25_COUNT_MASK) >> WFG3_ACTION25_COUNT_SHIFT)
#define  WFG3_ACTION25_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION25_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION25_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 7 of 10) (action26)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  WFG3_ACTION26_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION26_RESERVED1_SHIFT 30
#define  WFG3_ACTION26_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION26_RESERVED1_MASK) >> WFG3_ACTION26_RESERVED1_SHIFT)
#define  WFG3_ACTION26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION26_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION26_RESERVED1_SHIFT))

#define  WFG3_ACTION26_ACTION_MASK 0x30000000
#define  WFG3_ACTION26_ACTION_SHIFT 28
#define  WFG3_ACTION26_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION26_ACTION_MASK) >> WFG3_ACTION26_ACTION_SHIFT)
#define  WFG3_ACTION26_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION26_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION26_ACTION_SHIFT))

#define  WFG3_ACTION26_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION26_RESERVED2_SHIFT 24
#define  WFG3_ACTION26_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION26_RESERVED2_MASK) >> WFG3_ACTION26_RESERVED2_SHIFT)
#define  WFG3_ACTION26_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION26_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION26_RESERVED2_SHIFT))

#define  WFG3_ACTION26_COUNT_MASK 0xffffff
#define  WFG3_ACTION26_COUNT_SHIFT 0
#define  WFG3_ACTION26_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION26_COUNT_MASK) >> WFG3_ACTION26_COUNT_SHIFT)
#define  WFG3_ACTION26_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION26_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION26_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 8 of 10) (action27)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  WFG3_ACTION27_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION27_RESERVED1_SHIFT 30
#define  WFG3_ACTION27_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION27_RESERVED1_MASK) >> WFG3_ACTION27_RESERVED1_SHIFT)
#define  WFG3_ACTION27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION27_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION27_RESERVED1_SHIFT))

#define  WFG3_ACTION27_ACTION_MASK 0x30000000
#define  WFG3_ACTION27_ACTION_SHIFT 28
#define  WFG3_ACTION27_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION27_ACTION_MASK) >> WFG3_ACTION27_ACTION_SHIFT)
#define  WFG3_ACTION27_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION27_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION27_ACTION_SHIFT))

#define  WFG3_ACTION27_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION27_RESERVED2_SHIFT 24
#define  WFG3_ACTION27_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION27_RESERVED2_MASK) >> WFG3_ACTION27_RESERVED2_SHIFT)
#define  WFG3_ACTION27_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION27_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION27_RESERVED2_SHIFT))

#define  WFG3_ACTION27_COUNT_MASK 0xffffff
#define  WFG3_ACTION27_COUNT_SHIFT 0
#define  WFG3_ACTION27_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION27_COUNT_MASK) >> WFG3_ACTION27_COUNT_SHIFT)
#define  WFG3_ACTION27_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION27_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION27_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 9 of 10) (action28)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  WFG3_ACTION28_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION28_RESERVED1_SHIFT 30
#define  WFG3_ACTION28_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION28_RESERVED1_MASK) >> WFG3_ACTION28_RESERVED1_SHIFT)
#define  WFG3_ACTION28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION28_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION28_RESERVED1_SHIFT))

#define  WFG3_ACTION28_ACTION_MASK 0x30000000
#define  WFG3_ACTION28_ACTION_SHIFT 28
#define  WFG3_ACTION28_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION28_ACTION_MASK) >> WFG3_ACTION28_ACTION_SHIFT)
#define  WFG3_ACTION28_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION28_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION28_ACTION_SHIFT))

#define  WFG3_ACTION28_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION28_RESERVED2_SHIFT 24
#define  WFG3_ACTION28_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION28_RESERVED2_MASK) >> WFG3_ACTION28_RESERVED2_SHIFT)
#define  WFG3_ACTION28_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION28_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION28_RESERVED2_SHIFT))

#define  WFG3_ACTION28_COUNT_MASK 0xffffff
#define  WFG3_ACTION28_COUNT_SHIFT 0
#define  WFG3_ACTION28_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION28_COUNT_MASK) >> WFG3_ACTION28_COUNT_SHIFT)
#define  WFG3_ACTION28_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION28_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION28_COUNT_SHIFT))

//====================================================================
//Register: Waveform Action 2 register (Instance 10 of 10) (action29)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  WFG3_ACTION29_RESERVED1_MASK 0xc0000000
#define  WFG3_ACTION29_RESERVED1_SHIFT 30
#define  WFG3_ACTION29_RESERVED1_MASK_SHIFT(reg) (((reg) & WFG3_ACTION29_RESERVED1_MASK) >> WFG3_ACTION29_RESERVED1_SHIFT)
#define  WFG3_ACTION29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION29_RESERVED1_MASK) | (((uint32_t)val) << WFG3_ACTION29_RESERVED1_SHIFT))

#define  WFG3_ACTION29_ACTION_MASK 0x30000000
#define  WFG3_ACTION29_ACTION_SHIFT 28
#define  WFG3_ACTION29_ACTION_MASK_SHIFT(reg) (((reg) & WFG3_ACTION29_ACTION_MASK) >> WFG3_ACTION29_ACTION_SHIFT)
#define  WFG3_ACTION29_ACTION_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION29_ACTION_MASK) | (((uint32_t)val) << WFG3_ACTION29_ACTION_SHIFT))

#define  WFG3_ACTION29_RESERVED2_MASK 0xf000000
#define  WFG3_ACTION29_RESERVED2_SHIFT 24
#define  WFG3_ACTION29_RESERVED2_MASK_SHIFT(reg) (((reg) & WFG3_ACTION29_RESERVED2_MASK) >> WFG3_ACTION29_RESERVED2_SHIFT)
#define  WFG3_ACTION29_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION29_RESERVED2_MASK) | (((uint32_t)val) << WFG3_ACTION29_RESERVED2_SHIFT))

#define  WFG3_ACTION29_COUNT_MASK 0xffffff
#define  WFG3_ACTION29_COUNT_SHIFT 0
#define  WFG3_ACTION29_COUNT_MASK_SHIFT(reg) (((reg) & WFG3_ACTION29_COUNT_MASK) >> WFG3_ACTION29_COUNT_SHIFT)
#define  WFG3_ACTION29_COUNT_REPLACE_VAL(reg,val) (((reg) & ~WFG3_ACTION29_COUNT_MASK) | (((uint32_t)val) << WFG3_ACTION29_COUNT_SHIFT))

//====================================================================
//
//Register File: Waverform Selector Control (wfs)
//
//====================================================================

//====================================================================
//Register: Waveform Output Select register (out_sel)
//====================================================================

#define  WFS_OUT_SEL_ENABLE_MASK 0xff000000
#define  WFS_OUT_SEL_ENABLE_SHIFT 24
#define  WFS_OUT_SEL_ENABLE_MASK_SHIFT(reg) (((reg) & WFS_OUT_SEL_ENABLE_MASK) >> WFS_OUT_SEL_ENABLE_SHIFT)
#define  WFS_OUT_SEL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~WFS_OUT_SEL_ENABLE_MASK) | (((uint32_t)val) << WFS_OUT_SEL_ENABLE_SHIFT))

#define  WFS_OUT_SEL_FORCE_ON_MASK 0xff0000
#define  WFS_OUT_SEL_FORCE_ON_SHIFT 16
#define  WFS_OUT_SEL_FORCE_ON_MASK_SHIFT(reg) (((reg) & WFS_OUT_SEL_FORCE_ON_MASK) >> WFS_OUT_SEL_FORCE_ON_SHIFT)
#define  WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(reg,val) (((reg) & ~WFS_OUT_SEL_FORCE_ON_MASK) | (((uint32_t)val) << WFS_OUT_SEL_FORCE_ON_SHIFT))

#define  WFS_OUT_SEL_RESERVED1_MASK 0xf000
#define  WFS_OUT_SEL_RESERVED1_SHIFT 12
#define  WFS_OUT_SEL_RESERVED1_MASK_SHIFT(reg) (((reg) & WFS_OUT_SEL_RESERVED1_MASK) >> WFS_OUT_SEL_RESERVED1_SHIFT)
#define  WFS_OUT_SEL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFS_OUT_SEL_RESERVED1_MASK) | (((uint32_t)val) << WFS_OUT_SEL_RESERVED1_SHIFT))

#define  WFS_OUT_SEL_LD_CTRL_MASK 0xfff
#define  WFS_OUT_SEL_LD_CTRL_SHIFT 0
#define  WFS_OUT_SEL_LD_CTRL_MASK_SHIFT(reg) (((reg) & WFS_OUT_SEL_LD_CTRL_MASK) >> WFS_OUT_SEL_LD_CTRL_SHIFT)
#define  WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(reg,val) (((reg) & ~WFS_OUT_SEL_LD_CTRL_MASK) | (((uint32_t)val) << WFS_OUT_SEL_LD_CTRL_SHIFT))

//====================================================================
//Register: Waveform Select Data register (data_sel)
//====================================================================

#define  WFS_DATA_SEL_ENABLE_MASK 0xff000000
#define  WFS_DATA_SEL_ENABLE_SHIFT 24
#define  WFS_DATA_SEL_ENABLE_MASK_SHIFT(reg) (((reg) & WFS_DATA_SEL_ENABLE_MASK) >> WFS_DATA_SEL_ENABLE_SHIFT)
#define  WFS_DATA_SEL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~WFS_DATA_SEL_ENABLE_MASK) | (((uint32_t)val) << WFS_DATA_SEL_ENABLE_SHIFT))

#define  WFS_DATA_SEL_FORCE_ON_MASK 0xff0000
#define  WFS_DATA_SEL_FORCE_ON_SHIFT 16
#define  WFS_DATA_SEL_FORCE_ON_MASK_SHIFT(reg) (((reg) & WFS_DATA_SEL_FORCE_ON_MASK) >> WFS_DATA_SEL_FORCE_ON_SHIFT)
#define  WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(reg,val) (((reg) & ~WFS_DATA_SEL_FORCE_ON_MASK) | (((uint32_t)val) << WFS_DATA_SEL_FORCE_ON_SHIFT))

#define  WFS_DATA_SEL_RESERVED1_MASK 0xf000
#define  WFS_DATA_SEL_RESERVED1_SHIFT 12
#define  WFS_DATA_SEL_RESERVED1_MASK_SHIFT(reg) (((reg) & WFS_DATA_SEL_RESERVED1_MASK) >> WFS_DATA_SEL_RESERVED1_SHIFT)
#define  WFS_DATA_SEL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFS_DATA_SEL_RESERVED1_MASK) | (((uint32_t)val) << WFS_DATA_SEL_RESERVED1_SHIFT))

#define  WFS_DATA_SEL_LD_CTRL_MASK 0xfff
#define  WFS_DATA_SEL_LD_CTRL_SHIFT 0
#define  WFS_DATA_SEL_LD_CTRL_MASK_SHIFT(reg) (((reg) & WFS_DATA_SEL_LD_CTRL_MASK) >> WFS_DATA_SEL_LD_CTRL_SHIFT)
#define  WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(reg,val) (((reg) & ~WFS_DATA_SEL_LD_CTRL_MASK) | (((uint32_t)val) << WFS_DATA_SEL_LD_CTRL_SHIFT))

//====================================================================
//Register: Waveform Select Polarity register (wave_pol)
//====================================================================

#define  WFS_WAVE_POL_ENABLE_INV_MASK 0xff000000
#define  WFS_WAVE_POL_ENABLE_INV_SHIFT 24
#define  WFS_WAVE_POL_ENABLE_INV_MASK_SHIFT(reg) (((reg) & WFS_WAVE_POL_ENABLE_INV_MASK) >> WFS_WAVE_POL_ENABLE_INV_SHIFT)
#define  WFS_WAVE_POL_ENABLE_INV_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_POL_ENABLE_INV_MASK) | (((uint32_t)val) << WFS_WAVE_POL_ENABLE_INV_SHIFT))

#define  WFS_WAVE_POL_FORCE_INV_MASK 0xff0000
#define  WFS_WAVE_POL_FORCE_INV_SHIFT 16
#define  WFS_WAVE_POL_FORCE_INV_MASK_SHIFT(reg) (((reg) & WFS_WAVE_POL_FORCE_INV_MASK) >> WFS_WAVE_POL_FORCE_INV_SHIFT)
#define  WFS_WAVE_POL_FORCE_INV_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_POL_FORCE_INV_MASK) | (((uint32_t)val) << WFS_WAVE_POL_FORCE_INV_SHIFT))

#define  WFS_WAVE_POL_RESERVED1_MASK 0xf000
#define  WFS_WAVE_POL_RESERVED1_SHIFT 12
#define  WFS_WAVE_POL_RESERVED1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_POL_RESERVED1_MASK) >> WFS_WAVE_POL_RESERVED1_SHIFT)
#define  WFS_WAVE_POL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_POL_RESERVED1_MASK) | (((uint32_t)val) << WFS_WAVE_POL_RESERVED1_SHIFT))

#define  WFS_WAVE_POL_LD_CTRL_INV_MASK 0xfff
#define  WFS_WAVE_POL_LD_CTRL_INV_SHIFT 0
#define  WFS_WAVE_POL_LD_CTRL_INV_MASK_SHIFT(reg) (((reg) & WFS_WAVE_POL_LD_CTRL_INV_MASK) >> WFS_WAVE_POL_LD_CTRL_INV_SHIFT)
#define  WFS_WAVE_POL_LD_CTRL_INV_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_POL_LD_CTRL_INV_MASK) | (((uint32_t)val) << WFS_WAVE_POL_LD_CTRL_INV_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 1 of 12) (wave_ld_sel0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL0_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL0_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL0_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL0_WFG3_MASK) >> WFS_WAVE_LD_SEL0_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL0_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL0_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL0_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL0_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL0_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL0_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL0_WFG2_MASK) >> WFS_WAVE_LD_SEL0_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL0_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL0_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL0_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL0_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL0_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL0_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL0_WFG1_MASK) >> WFS_WAVE_LD_SEL0_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL0_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL0_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL0_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL0_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL0_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL0_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL0_WFG0_MASK) >> WFS_WAVE_LD_SEL0_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL0_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL0_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL0_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 2 of 12) (wave_ld_sel1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL1_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL1_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL1_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL1_WFG3_MASK) >> WFS_WAVE_LD_SEL1_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL1_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL1_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL1_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL1_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL1_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL1_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL1_WFG2_MASK) >> WFS_WAVE_LD_SEL1_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL1_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL1_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL1_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL1_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL1_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL1_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL1_WFG1_MASK) >> WFS_WAVE_LD_SEL1_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL1_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL1_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL1_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL1_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL1_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL1_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL1_WFG0_MASK) >> WFS_WAVE_LD_SEL1_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL1_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL1_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL1_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 3 of 12) (wave_ld_sel2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL2_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL2_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL2_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL2_WFG3_MASK) >> WFS_WAVE_LD_SEL2_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL2_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL2_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL2_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL2_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL2_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL2_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL2_WFG2_MASK) >> WFS_WAVE_LD_SEL2_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL2_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL2_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL2_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL2_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL2_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL2_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL2_WFG1_MASK) >> WFS_WAVE_LD_SEL2_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL2_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL2_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL2_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL2_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL2_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL2_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL2_WFG0_MASK) >> WFS_WAVE_LD_SEL2_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL2_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL2_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL2_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 4 of 12) (wave_ld_sel3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL3_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL3_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL3_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL3_WFG3_MASK) >> WFS_WAVE_LD_SEL3_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL3_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL3_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL3_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL3_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL3_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL3_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL3_WFG2_MASK) >> WFS_WAVE_LD_SEL3_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL3_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL3_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL3_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL3_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL3_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL3_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL3_WFG1_MASK) >> WFS_WAVE_LD_SEL3_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL3_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL3_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL3_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL3_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL3_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL3_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL3_WFG0_MASK) >> WFS_WAVE_LD_SEL3_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL3_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL3_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL3_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 5 of 12) (wave_ld_sel4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL4_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL4_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL4_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL4_WFG3_MASK) >> WFS_WAVE_LD_SEL4_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL4_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL4_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL4_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL4_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL4_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL4_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL4_WFG2_MASK) >> WFS_WAVE_LD_SEL4_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL4_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL4_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL4_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL4_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL4_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL4_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL4_WFG1_MASK) >> WFS_WAVE_LD_SEL4_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL4_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL4_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL4_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL4_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL4_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL4_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL4_WFG0_MASK) >> WFS_WAVE_LD_SEL4_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL4_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL4_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL4_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 6 of 12) (wave_ld_sel5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL5_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL5_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL5_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL5_WFG3_MASK) >> WFS_WAVE_LD_SEL5_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL5_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL5_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL5_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL5_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL5_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL5_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL5_WFG2_MASK) >> WFS_WAVE_LD_SEL5_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL5_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL5_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL5_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL5_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL5_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL5_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL5_WFG1_MASK) >> WFS_WAVE_LD_SEL5_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL5_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL5_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL5_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL5_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL5_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL5_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL5_WFG0_MASK) >> WFS_WAVE_LD_SEL5_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL5_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL5_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL5_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 7 of 12) (wave_ld_sel6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL6_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL6_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL6_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL6_WFG3_MASK) >> WFS_WAVE_LD_SEL6_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL6_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL6_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL6_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL6_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL6_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL6_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL6_WFG2_MASK) >> WFS_WAVE_LD_SEL6_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL6_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL6_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL6_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL6_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL6_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL6_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL6_WFG1_MASK) >> WFS_WAVE_LD_SEL6_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL6_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL6_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL6_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL6_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL6_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL6_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL6_WFG0_MASK) >> WFS_WAVE_LD_SEL6_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL6_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL6_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL6_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 8 of 12) (wave_ld_sel7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL7_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL7_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL7_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL7_WFG3_MASK) >> WFS_WAVE_LD_SEL7_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL7_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL7_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL7_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL7_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL7_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL7_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL7_WFG2_MASK) >> WFS_WAVE_LD_SEL7_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL7_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL7_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL7_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL7_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL7_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL7_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL7_WFG1_MASK) >> WFS_WAVE_LD_SEL7_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL7_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL7_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL7_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL7_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL7_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL7_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL7_WFG0_MASK) >> WFS_WAVE_LD_SEL7_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL7_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL7_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL7_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 9 of 12) (wave_ld_sel8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL8_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL8_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL8_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL8_WFG3_MASK) >> WFS_WAVE_LD_SEL8_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL8_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL8_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL8_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL8_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL8_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL8_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL8_WFG2_MASK) >> WFS_WAVE_LD_SEL8_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL8_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL8_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL8_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL8_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL8_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL8_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL8_WFG1_MASK) >> WFS_WAVE_LD_SEL8_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL8_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL8_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL8_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL8_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL8_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL8_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL8_WFG0_MASK) >> WFS_WAVE_LD_SEL8_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL8_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL8_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL8_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 10 of 12) (wave_ld_sel9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL9_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL9_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL9_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL9_WFG3_MASK) >> WFS_WAVE_LD_SEL9_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL9_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL9_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL9_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL9_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL9_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL9_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL9_WFG2_MASK) >> WFS_WAVE_LD_SEL9_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL9_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL9_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL9_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL9_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL9_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL9_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL9_WFG1_MASK) >> WFS_WAVE_LD_SEL9_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL9_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL9_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL9_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL9_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL9_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL9_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL9_WFG0_MASK) >> WFS_WAVE_LD_SEL9_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL9_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL9_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL9_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 11 of 12) (wave_ld_sel10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL10_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL10_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL10_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL10_WFG3_MASK) >> WFS_WAVE_LD_SEL10_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL10_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL10_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL10_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL10_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL10_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL10_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL10_WFG2_MASK) >> WFS_WAVE_LD_SEL10_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL10_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL10_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL10_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL10_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL10_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL10_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL10_WFG1_MASK) >> WFS_WAVE_LD_SEL10_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL10_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL10_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL10_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL10_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL10_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL10_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL10_WFG0_MASK) >> WFS_WAVE_LD_SEL10_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL10_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL10_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL10_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select LaserDiode_CTRL register (Instance 12 of 12) (wave_ld_sel11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  WFS_WAVE_LD_SEL11_WFG3_MASK 0xff000000
#define  WFS_WAVE_LD_SEL11_WFG3_SHIFT 24
#define  WFS_WAVE_LD_SEL11_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL11_WFG3_MASK) >> WFS_WAVE_LD_SEL11_WFG3_SHIFT)
#define  WFS_WAVE_LD_SEL11_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL11_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL11_WFG3_SHIFT))

#define  WFS_WAVE_LD_SEL11_WFG2_MASK 0xff0000
#define  WFS_WAVE_LD_SEL11_WFG2_SHIFT 16
#define  WFS_WAVE_LD_SEL11_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL11_WFG2_MASK) >> WFS_WAVE_LD_SEL11_WFG2_SHIFT)
#define  WFS_WAVE_LD_SEL11_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL11_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL11_WFG2_SHIFT))

#define  WFS_WAVE_LD_SEL11_WFG1_MASK 0xff00
#define  WFS_WAVE_LD_SEL11_WFG1_SHIFT 8
#define  WFS_WAVE_LD_SEL11_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL11_WFG1_MASK) >> WFS_WAVE_LD_SEL11_WFG1_SHIFT)
#define  WFS_WAVE_LD_SEL11_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL11_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL11_WFG1_SHIFT))

#define  WFS_WAVE_LD_SEL11_WFG0_MASK 0xff
#define  WFS_WAVE_LD_SEL11_WFG0_SHIFT 0
#define  WFS_WAVE_LD_SEL11_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_LD_SEL11_WFG0_MASK) >> WFS_WAVE_LD_SEL11_WFG0_SHIFT)
#define  WFS_WAVE_LD_SEL11_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_LD_SEL11_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_LD_SEL11_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 1 of 8) (wave_frc_sel0)
/** \brief  (Instance 1 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL0_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL0_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL0_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL0_WFG3_MASK) >> WFS_WAVE_FRC_SEL0_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL0_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL0_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL0_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL0_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL0_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL0_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL0_WFG2_MASK) >> WFS_WAVE_FRC_SEL0_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL0_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL0_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL0_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL0_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL0_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL0_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL0_WFG1_MASK) >> WFS_WAVE_FRC_SEL0_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL0_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL0_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL0_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL0_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL0_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL0_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL0_WFG0_MASK) >> WFS_WAVE_FRC_SEL0_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL0_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL0_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL0_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 2 of 8) (wave_frc_sel1)
/** \brief  (Instance 2 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL1_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL1_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL1_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL1_WFG3_MASK) >> WFS_WAVE_FRC_SEL1_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL1_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL1_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL1_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL1_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL1_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL1_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL1_WFG2_MASK) >> WFS_WAVE_FRC_SEL1_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL1_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL1_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL1_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL1_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL1_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL1_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL1_WFG1_MASK) >> WFS_WAVE_FRC_SEL1_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL1_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL1_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL1_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL1_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL1_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL1_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL1_WFG0_MASK) >> WFS_WAVE_FRC_SEL1_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL1_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL1_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL1_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 3 of 8) (wave_frc_sel2)
/** \brief  (Instance 3 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL2_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL2_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL2_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL2_WFG3_MASK) >> WFS_WAVE_FRC_SEL2_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL2_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL2_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL2_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL2_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL2_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL2_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL2_WFG2_MASK) >> WFS_WAVE_FRC_SEL2_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL2_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL2_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL2_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL2_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL2_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL2_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL2_WFG1_MASK) >> WFS_WAVE_FRC_SEL2_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL2_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL2_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL2_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL2_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL2_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL2_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL2_WFG0_MASK) >> WFS_WAVE_FRC_SEL2_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL2_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL2_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL2_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 4 of 8) (wave_frc_sel3)
/** \brief  (Instance 4 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL3_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL3_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL3_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL3_WFG3_MASK) >> WFS_WAVE_FRC_SEL3_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL3_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL3_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL3_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL3_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL3_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL3_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL3_WFG2_MASK) >> WFS_WAVE_FRC_SEL3_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL3_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL3_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL3_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL3_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL3_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL3_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL3_WFG1_MASK) >> WFS_WAVE_FRC_SEL3_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL3_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL3_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL3_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL3_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL3_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL3_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL3_WFG0_MASK) >> WFS_WAVE_FRC_SEL3_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL3_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL3_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL3_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 5 of 8) (wave_frc_sel4)
/** \brief  (Instance 5 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL4_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL4_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL4_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL4_WFG3_MASK) >> WFS_WAVE_FRC_SEL4_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL4_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL4_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL4_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL4_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL4_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL4_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL4_WFG2_MASK) >> WFS_WAVE_FRC_SEL4_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL4_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL4_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL4_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL4_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL4_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL4_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL4_WFG1_MASK) >> WFS_WAVE_FRC_SEL4_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL4_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL4_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL4_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL4_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL4_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL4_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL4_WFG0_MASK) >> WFS_WAVE_FRC_SEL4_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL4_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL4_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL4_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 6 of 8) (wave_frc_sel5)
/** \brief  (Instance 6 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL5_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL5_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL5_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL5_WFG3_MASK) >> WFS_WAVE_FRC_SEL5_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL5_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL5_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL5_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL5_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL5_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL5_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL5_WFG2_MASK) >> WFS_WAVE_FRC_SEL5_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL5_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL5_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL5_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL5_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL5_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL5_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL5_WFG1_MASK) >> WFS_WAVE_FRC_SEL5_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL5_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL5_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL5_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL5_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL5_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL5_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL5_WFG0_MASK) >> WFS_WAVE_FRC_SEL5_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL5_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL5_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL5_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 7 of 8) (wave_frc_sel6)
/** \brief  (Instance 7 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL6_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL6_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL6_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL6_WFG3_MASK) >> WFS_WAVE_FRC_SEL6_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL6_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL6_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL6_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL6_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL6_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL6_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL6_WFG2_MASK) >> WFS_WAVE_FRC_SEL6_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL6_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL6_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL6_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL6_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL6_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL6_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL6_WFG1_MASK) >> WFS_WAVE_FRC_SEL6_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL6_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL6_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL6_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL6_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL6_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL6_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL6_WFG0_MASK) >> WFS_WAVE_FRC_SEL6_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL6_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL6_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL6_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select ForceLaserOn register (Instance 8 of 8) (wave_frc_sel7)
/** \brief  (Instance 8 of 8)*/
//====================================================================

#define  WFS_WAVE_FRC_SEL7_WFG3_MASK 0xff000000
#define  WFS_WAVE_FRC_SEL7_WFG3_SHIFT 24
#define  WFS_WAVE_FRC_SEL7_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL7_WFG3_MASK) >> WFS_WAVE_FRC_SEL7_WFG3_SHIFT)
#define  WFS_WAVE_FRC_SEL7_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL7_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL7_WFG3_SHIFT))

#define  WFS_WAVE_FRC_SEL7_WFG2_MASK 0xff0000
#define  WFS_WAVE_FRC_SEL7_WFG2_SHIFT 16
#define  WFS_WAVE_FRC_SEL7_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL7_WFG2_MASK) >> WFS_WAVE_FRC_SEL7_WFG2_SHIFT)
#define  WFS_WAVE_FRC_SEL7_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL7_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL7_WFG2_SHIFT))

#define  WFS_WAVE_FRC_SEL7_WFG1_MASK 0xff00
#define  WFS_WAVE_FRC_SEL7_WFG1_SHIFT 8
#define  WFS_WAVE_FRC_SEL7_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL7_WFG1_MASK) >> WFS_WAVE_FRC_SEL7_WFG1_SHIFT)
#define  WFS_WAVE_FRC_SEL7_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL7_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL7_WFG1_SHIFT))

#define  WFS_WAVE_FRC_SEL7_WFG0_MASK 0xff
#define  WFS_WAVE_FRC_SEL7_WFG0_SHIFT 0
#define  WFS_WAVE_FRC_SEL7_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_FRC_SEL7_WFG0_MASK) >> WFS_WAVE_FRC_SEL7_WFG0_SHIFT)
#define  WFS_WAVE_FRC_SEL7_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_FRC_SEL7_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_FRC_SEL7_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 1 of 8) (wave_en_sel0)
/** \brief  (Instance 1 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL0_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL0_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL0_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL0_WFG3_MASK) >> WFS_WAVE_EN_SEL0_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL0_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL0_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL0_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL0_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL0_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL0_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL0_WFG2_MASK) >> WFS_WAVE_EN_SEL0_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL0_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL0_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL0_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL0_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL0_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL0_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL0_WFG1_MASK) >> WFS_WAVE_EN_SEL0_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL0_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL0_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL0_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL0_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL0_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL0_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL0_WFG0_MASK) >> WFS_WAVE_EN_SEL0_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL0_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL0_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL0_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 2 of 8) (wave_en_sel1)
/** \brief  (Instance 2 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL1_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL1_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL1_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL1_WFG3_MASK) >> WFS_WAVE_EN_SEL1_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL1_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL1_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL1_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL1_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL1_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL1_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL1_WFG2_MASK) >> WFS_WAVE_EN_SEL1_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL1_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL1_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL1_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL1_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL1_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL1_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL1_WFG1_MASK) >> WFS_WAVE_EN_SEL1_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL1_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL1_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL1_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL1_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL1_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL1_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL1_WFG0_MASK) >> WFS_WAVE_EN_SEL1_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL1_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL1_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL1_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 3 of 8) (wave_en_sel2)
/** \brief  (Instance 3 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL2_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL2_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL2_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL2_WFG3_MASK) >> WFS_WAVE_EN_SEL2_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL2_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL2_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL2_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL2_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL2_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL2_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL2_WFG2_MASK) >> WFS_WAVE_EN_SEL2_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL2_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL2_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL2_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL2_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL2_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL2_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL2_WFG1_MASK) >> WFS_WAVE_EN_SEL2_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL2_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL2_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL2_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL2_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL2_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL2_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL2_WFG0_MASK) >> WFS_WAVE_EN_SEL2_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL2_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL2_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL2_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 4 of 8) (wave_en_sel3)
/** \brief  (Instance 4 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL3_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL3_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL3_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL3_WFG3_MASK) >> WFS_WAVE_EN_SEL3_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL3_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL3_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL3_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL3_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL3_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL3_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL3_WFG2_MASK) >> WFS_WAVE_EN_SEL3_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL3_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL3_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL3_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL3_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL3_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL3_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL3_WFG1_MASK) >> WFS_WAVE_EN_SEL3_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL3_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL3_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL3_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL3_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL3_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL3_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL3_WFG0_MASK) >> WFS_WAVE_EN_SEL3_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL3_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL3_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL3_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 5 of 8) (wave_en_sel4)
/** \brief  (Instance 5 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL4_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL4_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL4_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL4_WFG3_MASK) >> WFS_WAVE_EN_SEL4_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL4_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL4_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL4_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL4_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL4_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL4_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL4_WFG2_MASK) >> WFS_WAVE_EN_SEL4_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL4_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL4_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL4_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL4_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL4_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL4_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL4_WFG1_MASK) >> WFS_WAVE_EN_SEL4_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL4_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL4_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL4_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL4_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL4_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL4_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL4_WFG0_MASK) >> WFS_WAVE_EN_SEL4_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL4_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL4_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL4_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 6 of 8) (wave_en_sel5)
/** \brief  (Instance 6 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL5_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL5_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL5_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL5_WFG3_MASK) >> WFS_WAVE_EN_SEL5_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL5_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL5_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL5_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL5_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL5_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL5_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL5_WFG2_MASK) >> WFS_WAVE_EN_SEL5_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL5_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL5_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL5_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL5_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL5_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL5_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL5_WFG1_MASK) >> WFS_WAVE_EN_SEL5_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL5_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL5_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL5_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL5_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL5_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL5_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL5_WFG0_MASK) >> WFS_WAVE_EN_SEL5_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL5_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL5_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL5_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 7 of 8) (wave_en_sel6)
/** \brief  (Instance 7 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL6_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL6_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL6_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL6_WFG3_MASK) >> WFS_WAVE_EN_SEL6_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL6_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL6_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL6_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL6_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL6_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL6_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL6_WFG2_MASK) >> WFS_WAVE_EN_SEL6_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL6_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL6_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL6_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL6_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL6_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL6_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL6_WFG1_MASK) >> WFS_WAVE_EN_SEL6_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL6_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL6_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL6_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL6_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL6_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL6_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL6_WFG0_MASK) >> WFS_WAVE_EN_SEL6_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL6_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL6_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL6_WFG0_SHIFT))

//====================================================================
//Register: Waveform Select EnableVDO register (Instance 8 of 8) (wave_en_sel7)
/** \brief  (Instance 8 of 8)*/
//====================================================================

#define  WFS_WAVE_EN_SEL7_WFG3_MASK 0xff000000
#define  WFS_WAVE_EN_SEL7_WFG3_SHIFT 24
#define  WFS_WAVE_EN_SEL7_WFG3_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL7_WFG3_MASK) >> WFS_WAVE_EN_SEL7_WFG3_SHIFT)
#define  WFS_WAVE_EN_SEL7_WFG3_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL7_WFG3_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL7_WFG3_SHIFT))

#define  WFS_WAVE_EN_SEL7_WFG2_MASK 0xff0000
#define  WFS_WAVE_EN_SEL7_WFG2_SHIFT 16
#define  WFS_WAVE_EN_SEL7_WFG2_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL7_WFG2_MASK) >> WFS_WAVE_EN_SEL7_WFG2_SHIFT)
#define  WFS_WAVE_EN_SEL7_WFG2_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL7_WFG2_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL7_WFG2_SHIFT))

#define  WFS_WAVE_EN_SEL7_WFG1_MASK 0xff00
#define  WFS_WAVE_EN_SEL7_WFG1_SHIFT 8
#define  WFS_WAVE_EN_SEL7_WFG1_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL7_WFG1_MASK) >> WFS_WAVE_EN_SEL7_WFG1_SHIFT)
#define  WFS_WAVE_EN_SEL7_WFG1_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL7_WFG1_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL7_WFG1_SHIFT))

#define  WFS_WAVE_EN_SEL7_WFG0_MASK 0xff
#define  WFS_WAVE_EN_SEL7_WFG0_SHIFT 0
#define  WFS_WAVE_EN_SEL7_WFG0_MASK_SHIFT(reg) (((reg) & WFS_WAVE_EN_SEL7_WFG0_MASK) >> WFS_WAVE_EN_SEL7_WFG0_SHIFT)
#define  WFS_WAVE_EN_SEL7_WFG0_REPLACE_VAL(reg,val) (((reg) & ~WFS_WAVE_EN_SEL7_WFG0_MASK) | (((uint32_t)val) << WFS_WAVE_EN_SEL7_WFG0_SHIFT))

#endif // LSCAN
