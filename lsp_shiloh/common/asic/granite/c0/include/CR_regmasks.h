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
 * \file CR_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CR_REGMASKS_H_
#define _CR_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Color Replace (CR)
/** \brief Top-level register file for Color Replace block*/
//
//====================================================================

//====================================================================
//Register: Configuration Register (CFG)
/** \brief Register used to configure the Color Replace block. Contains the block bypass bit and data merge configuration bits for controlling the outgoing 8 bit sideband bus.*/
//====================================================================

#define  CR_CFG_RESERVED1_MASK 0xf0000000
#define  CR_CFG_RESERVED1_SHIFT 28
#define  CR_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_CFG_RESERVED1_MASK) >> CR_CFG_RESERVED1_SHIFT)
#define  CR_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_RESERVED1_MASK) | (((uint32_t)val) << CR_CFG_RESERVED1_SHIFT))

#define  CR_CFG_LUTMUXSEL3_MASK 0xf000000
#define  CR_CFG_LUTMUXSEL3_SHIFT 24
#define  CR_CFG_LUTMUXSEL3_MASK_SHIFT(reg) (((reg) & CR_CFG_LUTMUXSEL3_MASK) >> CR_CFG_LUTMUXSEL3_SHIFT)
#define  CR_CFG_LUTMUXSEL3_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_LUTMUXSEL3_MASK) | (((uint32_t)val) << CR_CFG_LUTMUXSEL3_SHIFT))

#define  CR_CFG_LUTMUXSEL2_MASK 0xf00000
#define  CR_CFG_LUTMUXSEL2_SHIFT 20
#define  CR_CFG_LUTMUXSEL2_MASK_SHIFT(reg) (((reg) & CR_CFG_LUTMUXSEL2_MASK) >> CR_CFG_LUTMUXSEL2_SHIFT)
#define  CR_CFG_LUTMUXSEL2_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_LUTMUXSEL2_MASK) | (((uint32_t)val) << CR_CFG_LUTMUXSEL2_SHIFT))

#define  CR_CFG_LUTMUXSEL1_MASK 0xf0000
#define  CR_CFG_LUTMUXSEL1_SHIFT 16
#define  CR_CFG_LUTMUXSEL1_MASK_SHIFT(reg) (((reg) & CR_CFG_LUTMUXSEL1_MASK) >> CR_CFG_LUTMUXSEL1_SHIFT)
#define  CR_CFG_LUTMUXSEL1_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_LUTMUXSEL1_MASK) | (((uint32_t)val) << CR_CFG_LUTMUXSEL1_SHIFT))

#define  CR_CFG_LUTMUXSEL0_MASK 0xf000
#define  CR_CFG_LUTMUXSEL0_SHIFT 12
#define  CR_CFG_LUTMUXSEL0_MASK_SHIFT(reg) (((reg) & CR_CFG_LUTMUXSEL0_MASK) >> CR_CFG_LUTMUXSEL0_SHIFT)
#define  CR_CFG_LUTMUXSEL0_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_LUTMUXSEL0_MASK) | (((uint32_t)val) << CR_CFG_LUTMUXSEL0_SHIFT))

#define  CR_CFG_RESERVED2_MASK 0xc00
#define  CR_CFG_RESERVED2_SHIFT 10
#define  CR_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & CR_CFG_RESERVED2_MASK) >> CR_CFG_RESERVED2_SHIFT)
#define  CR_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_RESERVED2_MASK) | (((uint32_t)val) << CR_CFG_RESERVED2_SHIFT))

#define  CR_CFG_CONTONEEN_MASK 0x200
#define  CR_CFG_CONTONEEN_SHIFT 9
#define  CR_CFG_CONTONEEN_MASK_SHIFT(reg) (((reg) & CR_CFG_CONTONEEN_MASK) >> CR_CFG_CONTONEEN_SHIFT)
#define  CR_CFG_CONTONEEN_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_CONTONEEN_MASK) | (((uint32_t)val) << CR_CFG_CONTONEEN_SHIFT))

#define  CR_CFG_OBJECTEN_MASK 0x100
#define  CR_CFG_OBJECTEN_SHIFT 8
#define  CR_CFG_OBJECTEN_MASK_SHIFT(reg) (((reg) & CR_CFG_OBJECTEN_MASK) >> CR_CFG_OBJECTEN_SHIFT)
#define  CR_CFG_OBJECTEN_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_OBJECTEN_MASK) | (((uint32_t)val) << CR_CFG_OBJECTEN_SHIFT))

#define  CR_CFG_LUTOVR_MASK 0x80
#define  CR_CFG_LUTOVR_SHIFT 7
#define  CR_CFG_LUTOVR_MASK_SHIFT(reg) (((reg) & CR_CFG_LUTOVR_MASK) >> CR_CFG_LUTOVR_SHIFT)
#define  CR_CFG_LUTOVR_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_LUTOVR_MASK) | (((uint32_t)val) << CR_CFG_LUTOVR_SHIFT))

#define  CR_CFG_RESERVED3_MASK 0x40
#define  CR_CFG_RESERVED3_SHIFT 6
#define  CR_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & CR_CFG_RESERVED3_MASK) >> CR_CFG_RESERVED3_SHIFT)
#define  CR_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_RESERVED3_MASK) | (((uint32_t)val) << CR_CFG_RESERVED3_SHIFT))

#define  CR_CFG_SBANDMAP_MASK 0x30
#define  CR_CFG_SBANDMAP_SHIFT 4
#define  CR_CFG_SBANDMAP_MASK_SHIFT(reg) (((reg) & CR_CFG_SBANDMAP_MASK) >> CR_CFG_SBANDMAP_SHIFT)
#define  CR_CFG_SBANDMAP_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_SBANDMAP_MASK) | (((uint32_t)val) << CR_CFG_SBANDMAP_SHIFT))

#define  CR_CFG_RESERVED4_MASK 0xe
#define  CR_CFG_RESERVED4_SHIFT 1
#define  CR_CFG_RESERVED4_MASK_SHIFT(reg) (((reg) & CR_CFG_RESERVED4_MASK) >> CR_CFG_RESERVED4_SHIFT)
#define  CR_CFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_RESERVED4_MASK) | (((uint32_t)val) << CR_CFG_RESERVED4_SHIFT))

#define  CR_CFG_BYPASS_MASK 0x1
#define  CR_CFG_BYPASS_SHIFT 0
#define  CR_CFG_BYPASS_MASK_SHIFT(reg) (((reg) & CR_CFG_BYPASS_MASK) >> CR_CFG_BYPASS_SHIFT)
#define  CR_CFG_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~CR_CFG_BYPASS_MASK) | (((uint32_t)val) << CR_CFG_BYPASS_SHIFT))

//====================================================================
//Register: Upper Threshold (Instance 1 of 2) (UPPER0)
/** \brief Register contains the upper threshold for the colors to be replaced. The 0th element has highest priority of the replacements in the case that more than one replacement threshold matches. (Instance 1 of 2)*/
//====================================================================

#define  CR_UPPER0_RESERVED1_MASK 0xff000000
#define  CR_UPPER0_RESERVED1_SHIFT 24
#define  CR_UPPER0_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_UPPER0_RESERVED1_MASK) >> CR_UPPER0_RESERVED1_SHIFT)
#define  CR_UPPER0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER0_RESERVED1_MASK) | (((uint32_t)val) << CR_UPPER0_RESERVED1_SHIFT))

#define  CR_UPPER0_CH0_MASK 0xff0000
#define  CR_UPPER0_CH0_SHIFT 16
#define  CR_UPPER0_CH0_MASK_SHIFT(reg) (((reg) & CR_UPPER0_CH0_MASK) >> CR_UPPER0_CH0_SHIFT)
#define  CR_UPPER0_CH0_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER0_CH0_MASK) | (((uint32_t)val) << CR_UPPER0_CH0_SHIFT))

#define  CR_UPPER0_CH1_MASK 0xff00
#define  CR_UPPER0_CH1_SHIFT 8
#define  CR_UPPER0_CH1_MASK_SHIFT(reg) (((reg) & CR_UPPER0_CH1_MASK) >> CR_UPPER0_CH1_SHIFT)
#define  CR_UPPER0_CH1_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER0_CH1_MASK) | (((uint32_t)val) << CR_UPPER0_CH1_SHIFT))

#define  CR_UPPER0_CH2_MASK 0xff
#define  CR_UPPER0_CH2_SHIFT 0
#define  CR_UPPER0_CH2_MASK_SHIFT(reg) (((reg) & CR_UPPER0_CH2_MASK) >> CR_UPPER0_CH2_SHIFT)
#define  CR_UPPER0_CH2_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER0_CH2_MASK) | (((uint32_t)val) << CR_UPPER0_CH2_SHIFT))

//====================================================================
//Register: Upper Threshold (Instance 2 of 2) (UPPER1)
/** \brief Register contains the upper threshold for the colors to be replaced. The 0th element has highest priority of the replacements in the case that more than one replacement threshold matches. (Instance 2 of 2)*/
//====================================================================

#define  CR_UPPER1_RESERVED1_MASK 0xff000000
#define  CR_UPPER1_RESERVED1_SHIFT 24
#define  CR_UPPER1_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_UPPER1_RESERVED1_MASK) >> CR_UPPER1_RESERVED1_SHIFT)
#define  CR_UPPER1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER1_RESERVED1_MASK) | (((uint32_t)val) << CR_UPPER1_RESERVED1_SHIFT))

#define  CR_UPPER1_CH0_MASK 0xff0000
#define  CR_UPPER1_CH0_SHIFT 16
#define  CR_UPPER1_CH0_MASK_SHIFT(reg) (((reg) & CR_UPPER1_CH0_MASK) >> CR_UPPER1_CH0_SHIFT)
#define  CR_UPPER1_CH0_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER1_CH0_MASK) | (((uint32_t)val) << CR_UPPER1_CH0_SHIFT))

#define  CR_UPPER1_CH1_MASK 0xff00
#define  CR_UPPER1_CH1_SHIFT 8
#define  CR_UPPER1_CH1_MASK_SHIFT(reg) (((reg) & CR_UPPER1_CH1_MASK) >> CR_UPPER1_CH1_SHIFT)
#define  CR_UPPER1_CH1_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER1_CH1_MASK) | (((uint32_t)val) << CR_UPPER1_CH1_SHIFT))

#define  CR_UPPER1_CH2_MASK 0xff
#define  CR_UPPER1_CH2_SHIFT 0
#define  CR_UPPER1_CH2_MASK_SHIFT(reg) (((reg) & CR_UPPER1_CH2_MASK) >> CR_UPPER1_CH2_SHIFT)
#define  CR_UPPER1_CH2_REPLACE_VAL(reg,val) (((reg) & ~CR_UPPER1_CH2_MASK) | (((uint32_t)val) << CR_UPPER1_CH2_SHIFT))

//====================================================================
//Register: Lower Threshold (Instance 1 of 2) (LOWER0)
/** \brief Register contains the lower threshold for the colors to be replaced. The 0th element has highest priority of the replacements in the case that more than one replacement threshold matches. (Instance 1 of 2)*/
//====================================================================

#define  CR_LOWER0_RESERVED1_MASK 0xff000000
#define  CR_LOWER0_RESERVED1_SHIFT 24
#define  CR_LOWER0_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_LOWER0_RESERVED1_MASK) >> CR_LOWER0_RESERVED1_SHIFT)
#define  CR_LOWER0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER0_RESERVED1_MASK) | (((uint32_t)val) << CR_LOWER0_RESERVED1_SHIFT))

#define  CR_LOWER0_CH0_MASK 0xff0000
#define  CR_LOWER0_CH0_SHIFT 16
#define  CR_LOWER0_CH0_MASK_SHIFT(reg) (((reg) & CR_LOWER0_CH0_MASK) >> CR_LOWER0_CH0_SHIFT)
#define  CR_LOWER0_CH0_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER0_CH0_MASK) | (((uint32_t)val) << CR_LOWER0_CH0_SHIFT))

#define  CR_LOWER0_CH1_MASK 0xff00
#define  CR_LOWER0_CH1_SHIFT 8
#define  CR_LOWER0_CH1_MASK_SHIFT(reg) (((reg) & CR_LOWER0_CH1_MASK) >> CR_LOWER0_CH1_SHIFT)
#define  CR_LOWER0_CH1_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER0_CH1_MASK) | (((uint32_t)val) << CR_LOWER0_CH1_SHIFT))

#define  CR_LOWER0_CH2_MASK 0xff
#define  CR_LOWER0_CH2_SHIFT 0
#define  CR_LOWER0_CH2_MASK_SHIFT(reg) (((reg) & CR_LOWER0_CH2_MASK) >> CR_LOWER0_CH2_SHIFT)
#define  CR_LOWER0_CH2_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER0_CH2_MASK) | (((uint32_t)val) << CR_LOWER0_CH2_SHIFT))

//====================================================================
//Register: Lower Threshold (Instance 2 of 2) (LOWER1)
/** \brief Register contains the lower threshold for the colors to be replaced. The 0th element has highest priority of the replacements in the case that more than one replacement threshold matches. (Instance 2 of 2)*/
//====================================================================

#define  CR_LOWER1_RESERVED1_MASK 0xff000000
#define  CR_LOWER1_RESERVED1_SHIFT 24
#define  CR_LOWER1_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_LOWER1_RESERVED1_MASK) >> CR_LOWER1_RESERVED1_SHIFT)
#define  CR_LOWER1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER1_RESERVED1_MASK) | (((uint32_t)val) << CR_LOWER1_RESERVED1_SHIFT))

#define  CR_LOWER1_CH0_MASK 0xff0000
#define  CR_LOWER1_CH0_SHIFT 16
#define  CR_LOWER1_CH0_MASK_SHIFT(reg) (((reg) & CR_LOWER1_CH0_MASK) >> CR_LOWER1_CH0_SHIFT)
#define  CR_LOWER1_CH0_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER1_CH0_MASK) | (((uint32_t)val) << CR_LOWER1_CH0_SHIFT))

#define  CR_LOWER1_CH1_MASK 0xff00
#define  CR_LOWER1_CH1_SHIFT 8
#define  CR_LOWER1_CH1_MASK_SHIFT(reg) (((reg) & CR_LOWER1_CH1_MASK) >> CR_LOWER1_CH1_SHIFT)
#define  CR_LOWER1_CH1_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER1_CH1_MASK) | (((uint32_t)val) << CR_LOWER1_CH1_SHIFT))

#define  CR_LOWER1_CH2_MASK 0xff
#define  CR_LOWER1_CH2_SHIFT 0
#define  CR_LOWER1_CH2_MASK_SHIFT(reg) (((reg) & CR_LOWER1_CH2_MASK) >> CR_LOWER1_CH2_SHIFT)
#define  CR_LOWER1_CH2_REPLACE_VAL(reg,val) (((reg) & ~CR_LOWER1_CH2_MASK) | (((uint32_t)val) << CR_LOWER1_CH2_SHIFT))

//====================================================================
//Register: Replacement Value (Instance 1 of 2) (REP0)
/** \brief Register contains the replacement color values. The 0th element has highest priority of the replacements in the case that more than one replacement threshold matches. (Instance 1 of 2)*/
//====================================================================

#define  CR_REP0_RESERVED1_MASK 0xff000000
#define  CR_REP0_RESERVED1_SHIFT 24
#define  CR_REP0_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_REP0_RESERVED1_MASK) >> CR_REP0_RESERVED1_SHIFT)
#define  CR_REP0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_REP0_RESERVED1_MASK) | (((uint32_t)val) << CR_REP0_RESERVED1_SHIFT))

#define  CR_REP0_CH0_MASK 0xff0000
#define  CR_REP0_CH0_SHIFT 16
#define  CR_REP0_CH0_MASK_SHIFT(reg) (((reg) & CR_REP0_CH0_MASK) >> CR_REP0_CH0_SHIFT)
#define  CR_REP0_CH0_REPLACE_VAL(reg,val) (((reg) & ~CR_REP0_CH0_MASK) | (((uint32_t)val) << CR_REP0_CH0_SHIFT))

#define  CR_REP0_CH1_MASK 0xff00
#define  CR_REP0_CH1_SHIFT 8
#define  CR_REP0_CH1_MASK_SHIFT(reg) (((reg) & CR_REP0_CH1_MASK) >> CR_REP0_CH1_SHIFT)
#define  CR_REP0_CH1_REPLACE_VAL(reg,val) (((reg) & ~CR_REP0_CH1_MASK) | (((uint32_t)val) << CR_REP0_CH1_SHIFT))

#define  CR_REP0_CH2_MASK 0xff
#define  CR_REP0_CH2_SHIFT 0
#define  CR_REP0_CH2_MASK_SHIFT(reg) (((reg) & CR_REP0_CH2_MASK) >> CR_REP0_CH2_SHIFT)
#define  CR_REP0_CH2_REPLACE_VAL(reg,val) (((reg) & ~CR_REP0_CH2_MASK) | (((uint32_t)val) << CR_REP0_CH2_SHIFT))

//====================================================================
//Register: Replacement Value (Instance 2 of 2) (REP1)
/** \brief Register contains the replacement color values. The 0th element has highest priority of the replacements in the case that more than one replacement threshold matches. (Instance 2 of 2)*/
//====================================================================

#define  CR_REP1_RESERVED1_MASK 0xff000000
#define  CR_REP1_RESERVED1_SHIFT 24
#define  CR_REP1_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_REP1_RESERVED1_MASK) >> CR_REP1_RESERVED1_SHIFT)
#define  CR_REP1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_REP1_RESERVED1_MASK) | (((uint32_t)val) << CR_REP1_RESERVED1_SHIFT))

#define  CR_REP1_CH0_MASK 0xff0000
#define  CR_REP1_CH0_SHIFT 16
#define  CR_REP1_CH0_MASK_SHIFT(reg) (((reg) & CR_REP1_CH0_MASK) >> CR_REP1_CH0_SHIFT)
#define  CR_REP1_CH0_REPLACE_VAL(reg,val) (((reg) & ~CR_REP1_CH0_MASK) | (((uint32_t)val) << CR_REP1_CH0_SHIFT))

#define  CR_REP1_CH1_MASK 0xff00
#define  CR_REP1_CH1_SHIFT 8
#define  CR_REP1_CH1_MASK_SHIFT(reg) (((reg) & CR_REP1_CH1_MASK) >> CR_REP1_CH1_SHIFT)
#define  CR_REP1_CH1_REPLACE_VAL(reg,val) (((reg) & ~CR_REP1_CH1_MASK) | (((uint32_t)val) << CR_REP1_CH1_SHIFT))

#define  CR_REP1_CH2_MASK 0xff
#define  CR_REP1_CH2_SHIFT 0
#define  CR_REP1_CH2_MASK_SHIFT(reg) (((reg) & CR_REP1_CH2_MASK) >> CR_REP1_CH2_SHIFT)
#define  CR_REP1_CH2_REPLACE_VAL(reg,val) (((reg) & ~CR_REP1_CH2_MASK) | (((uint32_t)val) << CR_REP1_CH2_SHIFT))

//====================================================================
//Register: Merge LUT (MLUT)
/** \brief FIXME*/
//====================================================================

#define  CR_MLUT_VALUE15_MASK 0xc0000000
#define  CR_MLUT_VALUE15_SHIFT 30
#define  CR_MLUT_VALUE15_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE15_MASK) >> CR_MLUT_VALUE15_SHIFT)
#define  CR_MLUT_VALUE15_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE15_MASK) | (((uint32_t)val) << CR_MLUT_VALUE15_SHIFT))

#define  CR_MLUT_VALUE14_MASK 0x30000000
#define  CR_MLUT_VALUE14_SHIFT 28
#define  CR_MLUT_VALUE14_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE14_MASK) >> CR_MLUT_VALUE14_SHIFT)
#define  CR_MLUT_VALUE14_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE14_MASK) | (((uint32_t)val) << CR_MLUT_VALUE14_SHIFT))

#define  CR_MLUT_VALUE13_MASK 0xc000000
#define  CR_MLUT_VALUE13_SHIFT 26
#define  CR_MLUT_VALUE13_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE13_MASK) >> CR_MLUT_VALUE13_SHIFT)
#define  CR_MLUT_VALUE13_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE13_MASK) | (((uint32_t)val) << CR_MLUT_VALUE13_SHIFT))

#define  CR_MLUT_VALUE12_MASK 0x3000000
#define  CR_MLUT_VALUE12_SHIFT 24
#define  CR_MLUT_VALUE12_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE12_MASK) >> CR_MLUT_VALUE12_SHIFT)
#define  CR_MLUT_VALUE12_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE12_MASK) | (((uint32_t)val) << CR_MLUT_VALUE12_SHIFT))

#define  CR_MLUT_VALUE11_MASK 0xc00000
#define  CR_MLUT_VALUE11_SHIFT 22
#define  CR_MLUT_VALUE11_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE11_MASK) >> CR_MLUT_VALUE11_SHIFT)
#define  CR_MLUT_VALUE11_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE11_MASK) | (((uint32_t)val) << CR_MLUT_VALUE11_SHIFT))

#define  CR_MLUT_VALUE10_MASK 0x300000
#define  CR_MLUT_VALUE10_SHIFT 20
#define  CR_MLUT_VALUE10_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE10_MASK) >> CR_MLUT_VALUE10_SHIFT)
#define  CR_MLUT_VALUE10_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE10_MASK) | (((uint32_t)val) << CR_MLUT_VALUE10_SHIFT))

#define  CR_MLUT_VALUE9_MASK 0xc0000
#define  CR_MLUT_VALUE9_SHIFT 18
#define  CR_MLUT_VALUE9_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE9_MASK) >> CR_MLUT_VALUE9_SHIFT)
#define  CR_MLUT_VALUE9_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE9_MASK) | (((uint32_t)val) << CR_MLUT_VALUE9_SHIFT))

#define  CR_MLUT_VALUE8_MASK 0x30000
#define  CR_MLUT_VALUE8_SHIFT 16
#define  CR_MLUT_VALUE8_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE8_MASK) >> CR_MLUT_VALUE8_SHIFT)
#define  CR_MLUT_VALUE8_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE8_MASK) | (((uint32_t)val) << CR_MLUT_VALUE8_SHIFT))

#define  CR_MLUT_VALUE7_MASK 0xc000
#define  CR_MLUT_VALUE7_SHIFT 14
#define  CR_MLUT_VALUE7_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE7_MASK) >> CR_MLUT_VALUE7_SHIFT)
#define  CR_MLUT_VALUE7_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE7_MASK) | (((uint32_t)val) << CR_MLUT_VALUE7_SHIFT))

#define  CR_MLUT_VALUE6_MASK 0x3000
#define  CR_MLUT_VALUE6_SHIFT 12
#define  CR_MLUT_VALUE6_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE6_MASK) >> CR_MLUT_VALUE6_SHIFT)
#define  CR_MLUT_VALUE6_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE6_MASK) | (((uint32_t)val) << CR_MLUT_VALUE6_SHIFT))

#define  CR_MLUT_VALUE5_MASK 0xc00
#define  CR_MLUT_VALUE5_SHIFT 10
#define  CR_MLUT_VALUE5_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE5_MASK) >> CR_MLUT_VALUE5_SHIFT)
#define  CR_MLUT_VALUE5_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE5_MASK) | (((uint32_t)val) << CR_MLUT_VALUE5_SHIFT))

#define  CR_MLUT_VALUE4_MASK 0x300
#define  CR_MLUT_VALUE4_SHIFT 8
#define  CR_MLUT_VALUE4_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE4_MASK) >> CR_MLUT_VALUE4_SHIFT)
#define  CR_MLUT_VALUE4_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE4_MASK) | (((uint32_t)val) << CR_MLUT_VALUE4_SHIFT))

#define  CR_MLUT_VALUE3_MASK 0xc0
#define  CR_MLUT_VALUE3_SHIFT 6
#define  CR_MLUT_VALUE3_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE3_MASK) >> CR_MLUT_VALUE3_SHIFT)
#define  CR_MLUT_VALUE3_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE3_MASK) | (((uint32_t)val) << CR_MLUT_VALUE3_SHIFT))

#define  CR_MLUT_VALUE2_MASK 0x30
#define  CR_MLUT_VALUE2_SHIFT 4
#define  CR_MLUT_VALUE2_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE2_MASK) >> CR_MLUT_VALUE2_SHIFT)
#define  CR_MLUT_VALUE2_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE2_MASK) | (((uint32_t)val) << CR_MLUT_VALUE2_SHIFT))

#define  CR_MLUT_VALUE1_MASK 0xc
#define  CR_MLUT_VALUE1_SHIFT 2
#define  CR_MLUT_VALUE1_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE1_MASK) >> CR_MLUT_VALUE1_SHIFT)
#define  CR_MLUT_VALUE1_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE1_MASK) | (((uint32_t)val) << CR_MLUT_VALUE1_SHIFT))

#define  CR_MLUT_VALUE0_MASK 0x3
#define  CR_MLUT_VALUE0_SHIFT 0
#define  CR_MLUT_VALUE0_MASK_SHIFT(reg) (((reg) & CR_MLUT_VALUE0_MASK) >> CR_MLUT_VALUE0_SHIFT)
#define  CR_MLUT_VALUE0_REPLACE_VAL(reg,val) (((reg) & ~CR_MLUT_VALUE0_MASK) | (((uint32_t)val) << CR_MLUT_VALUE0_SHIFT))

//====================================================================
//Register: Replace LUT (RLUT)
/** \brief FIXME*/
//====================================================================

#define  CR_RLUT_VALUE15_MASK 0xc0000000
#define  CR_RLUT_VALUE15_SHIFT 30
#define  CR_RLUT_VALUE15_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE15_MASK) >> CR_RLUT_VALUE15_SHIFT)
#define  CR_RLUT_VALUE15_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE15_MASK) | (((uint32_t)val) << CR_RLUT_VALUE15_SHIFT))

#define  CR_RLUT_VALUE14_MASK 0x30000000
#define  CR_RLUT_VALUE14_SHIFT 28
#define  CR_RLUT_VALUE14_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE14_MASK) >> CR_RLUT_VALUE14_SHIFT)
#define  CR_RLUT_VALUE14_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE14_MASK) | (((uint32_t)val) << CR_RLUT_VALUE14_SHIFT))

#define  CR_RLUT_VALUE13_MASK 0xc000000
#define  CR_RLUT_VALUE13_SHIFT 26
#define  CR_RLUT_VALUE13_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE13_MASK) >> CR_RLUT_VALUE13_SHIFT)
#define  CR_RLUT_VALUE13_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE13_MASK) | (((uint32_t)val) << CR_RLUT_VALUE13_SHIFT))

#define  CR_RLUT_VALUE12_MASK 0x3000000
#define  CR_RLUT_VALUE12_SHIFT 24
#define  CR_RLUT_VALUE12_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE12_MASK) >> CR_RLUT_VALUE12_SHIFT)
#define  CR_RLUT_VALUE12_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE12_MASK) | (((uint32_t)val) << CR_RLUT_VALUE12_SHIFT))

#define  CR_RLUT_VALUE11_MASK 0xc00000
#define  CR_RLUT_VALUE11_SHIFT 22
#define  CR_RLUT_VALUE11_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE11_MASK) >> CR_RLUT_VALUE11_SHIFT)
#define  CR_RLUT_VALUE11_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE11_MASK) | (((uint32_t)val) << CR_RLUT_VALUE11_SHIFT))

#define  CR_RLUT_VALUE10_MASK 0x300000
#define  CR_RLUT_VALUE10_SHIFT 20
#define  CR_RLUT_VALUE10_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE10_MASK) >> CR_RLUT_VALUE10_SHIFT)
#define  CR_RLUT_VALUE10_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE10_MASK) | (((uint32_t)val) << CR_RLUT_VALUE10_SHIFT))

#define  CR_RLUT_VALUE9_MASK 0xc0000
#define  CR_RLUT_VALUE9_SHIFT 18
#define  CR_RLUT_VALUE9_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE9_MASK) >> CR_RLUT_VALUE9_SHIFT)
#define  CR_RLUT_VALUE9_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE9_MASK) | (((uint32_t)val) << CR_RLUT_VALUE9_SHIFT))

#define  CR_RLUT_VALUE8_MASK 0x30000
#define  CR_RLUT_VALUE8_SHIFT 16
#define  CR_RLUT_VALUE8_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE8_MASK) >> CR_RLUT_VALUE8_SHIFT)
#define  CR_RLUT_VALUE8_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE8_MASK) | (((uint32_t)val) << CR_RLUT_VALUE8_SHIFT))

#define  CR_RLUT_VALUE7_MASK 0xc000
#define  CR_RLUT_VALUE7_SHIFT 14
#define  CR_RLUT_VALUE7_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE7_MASK) >> CR_RLUT_VALUE7_SHIFT)
#define  CR_RLUT_VALUE7_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE7_MASK) | (((uint32_t)val) << CR_RLUT_VALUE7_SHIFT))

#define  CR_RLUT_VALUE6_MASK 0x3000
#define  CR_RLUT_VALUE6_SHIFT 12
#define  CR_RLUT_VALUE6_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE6_MASK) >> CR_RLUT_VALUE6_SHIFT)
#define  CR_RLUT_VALUE6_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE6_MASK) | (((uint32_t)val) << CR_RLUT_VALUE6_SHIFT))

#define  CR_RLUT_VALUE5_MASK 0xc00
#define  CR_RLUT_VALUE5_SHIFT 10
#define  CR_RLUT_VALUE5_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE5_MASK) >> CR_RLUT_VALUE5_SHIFT)
#define  CR_RLUT_VALUE5_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE5_MASK) | (((uint32_t)val) << CR_RLUT_VALUE5_SHIFT))

#define  CR_RLUT_VALUE4_MASK 0x300
#define  CR_RLUT_VALUE4_SHIFT 8
#define  CR_RLUT_VALUE4_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE4_MASK) >> CR_RLUT_VALUE4_SHIFT)
#define  CR_RLUT_VALUE4_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE4_MASK) | (((uint32_t)val) << CR_RLUT_VALUE4_SHIFT))

#define  CR_RLUT_VALUE3_MASK 0xc0
#define  CR_RLUT_VALUE3_SHIFT 6
#define  CR_RLUT_VALUE3_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE3_MASK) >> CR_RLUT_VALUE3_SHIFT)
#define  CR_RLUT_VALUE3_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE3_MASK) | (((uint32_t)val) << CR_RLUT_VALUE3_SHIFT))

#define  CR_RLUT_VALUE2_MASK 0x30
#define  CR_RLUT_VALUE2_SHIFT 4
#define  CR_RLUT_VALUE2_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE2_MASK) >> CR_RLUT_VALUE2_SHIFT)
#define  CR_RLUT_VALUE2_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE2_MASK) | (((uint32_t)val) << CR_RLUT_VALUE2_SHIFT))

#define  CR_RLUT_VALUE1_MASK 0xc
#define  CR_RLUT_VALUE1_SHIFT 2
#define  CR_RLUT_VALUE1_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE1_MASK) >> CR_RLUT_VALUE1_SHIFT)
#define  CR_RLUT_VALUE1_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE1_MASK) | (((uint32_t)val) << CR_RLUT_VALUE1_SHIFT))

#define  CR_RLUT_VALUE0_MASK 0x3
#define  CR_RLUT_VALUE0_SHIFT 0
#define  CR_RLUT_VALUE0_MASK_SHIFT(reg) (((reg) & CR_RLUT_VALUE0_MASK) >> CR_RLUT_VALUE0_SHIFT)
#define  CR_RLUT_VALUE0_REPLACE_VAL(reg,val) (((reg) & ~CR_RLUT_VALUE0_MASK) | (((uint32_t)val) << CR_RLUT_VALUE0_SHIFT))

//====================================================================
//Register: Status Read Register (STAT_RD)
/** \brief Provides status information. The register is READ ONLY.*/
//====================================================================

#define  CR_STAT_RD_RESERVED1_MASK 0xfffffffe
#define  CR_STAT_RD_RESERVED1_SHIFT 1
#define  CR_STAT_RD_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_STAT_RD_RESERVED1_MASK) >> CR_STAT_RD_RESERVED1_SHIFT)
#define  CR_STAT_RD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_STAT_RD_RESERVED1_MASK) | (((uint32_t)val) << CR_STAT_RD_RESERVED1_SHIFT))

#define  CR_STAT_RD_DT_FAULT_MASK 0x1
#define  CR_STAT_RD_DT_FAULT_SHIFT 0
#define  CR_STAT_RD_DT_FAULT_MASK_SHIFT(reg) (((reg) & CR_STAT_RD_DT_FAULT_MASK) >> CR_STAT_RD_DT_FAULT_SHIFT)
#define  CR_STAT_RD_DT_FAULT_REPLACE_VAL(reg,val) (((reg) & ~CR_STAT_RD_DT_FAULT_MASK) | (((uint32_t)val) << CR_STAT_RD_DT_FAULT_SHIFT))

//====================================================================
//Register: Status Write Register (STAT_WR)
/** \brief Allows the status flags to be cleared. The register is WRITE ONLY.*/
//====================================================================

#define  CR_STAT_WR_RESERVED1_MASK 0xfffffffe
#define  CR_STAT_WR_RESERVED1_SHIFT 1
#define  CR_STAT_WR_RESERVED1_MASK_SHIFT(reg) (((reg) & CR_STAT_WR_RESERVED1_MASK) >> CR_STAT_WR_RESERVED1_SHIFT)
#define  CR_STAT_WR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CR_STAT_WR_RESERVED1_MASK) | (((uint32_t)val) << CR_STAT_WR_RESERVED1_SHIFT))

#define  CR_STAT_WR_DT_FAULT_MASK 0x1
#define  CR_STAT_WR_DT_FAULT_SHIFT 0
#define  CR_STAT_WR_DT_FAULT_MASK_SHIFT(reg) (((reg) & CR_STAT_WR_DT_FAULT_MASK) >> CR_STAT_WR_DT_FAULT_SHIFT)
#define  CR_STAT_WR_DT_FAULT_REPLACE_VAL(reg,val) (((reg) & ~CR_STAT_WR_DT_FAULT_MASK) | (((uint32_t)val) << CR_STAT_WR_DT_FAULT_SHIFT))

#endif // CR
