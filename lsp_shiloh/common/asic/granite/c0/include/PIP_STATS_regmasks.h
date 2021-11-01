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
 * \file PIP_STATS_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIP_STATS_REGMASKS_H_
#define _PIP_STATS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIP_STATS (PIP_STATS)
/** \brief Register descriptions for the PIP_STATS block.*/
//
//====================================================================

//====================================================================
//Register: PIP_STATS Configuration Register (ACR)
/** \brief This register configures the general operation of the PIP_STATS block. If PIP_STATS is bypassed, the states of the internal state machines are reset, but register settings are preserved. The CPU access bit must be enabled to write the auto halftone LUT, and must be disabled before starting the block.*/
//====================================================================

#define  PIP_STATS_ACR_IMAGE_WIDTH_M1_MASK 0xffff0000
#define  PIP_STATS_ACR_IMAGE_WIDTH_M1_SHIFT 16
#define  PIP_STATS_ACR_IMAGE_WIDTH_M1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_IMAGE_WIDTH_M1_MASK) >> PIP_STATS_ACR_IMAGE_WIDTH_M1_SHIFT)
#define  PIP_STATS_ACR_IMAGE_WIDTH_M1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_IMAGE_WIDTH_M1_MASK) | (((uint32_t)val) << PIP_STATS_ACR_IMAGE_WIDTH_M1_SHIFT))

#define  PIP_STATS_ACR_RESERVED1_MASK 0xf800
#define  PIP_STATS_ACR_RESERVED1_SHIFT 11
#define  PIP_STATS_ACR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_RESERVED1_MASK) >> PIP_STATS_ACR_RESERVED1_SHIFT)
#define  PIP_STATS_ACR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_ACR_RESERVED1_SHIFT))

#define  PIP_STATS_ACR_CHAN_DISABLE_MASK 0x700
#define  PIP_STATS_ACR_CHAN_DISABLE_SHIFT 8
#define  PIP_STATS_ACR_CHAN_DISABLE_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_CHAN_DISABLE_MASK) >> PIP_STATS_ACR_CHAN_DISABLE_SHIFT)
#define  PIP_STATS_ACR_CHAN_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_CHAN_DISABLE_MASK) | (((uint32_t)val) << PIP_STATS_ACR_CHAN_DISABLE_SHIFT))

#define  PIP_STATS_ACR_RESERVED2_MASK 0xe0
#define  PIP_STATS_ACR_RESERVED2_SHIFT 5
#define  PIP_STATS_ACR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_RESERVED2_MASK) >> PIP_STATS_ACR_RESERVED2_SHIFT)
#define  PIP_STATS_ACR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_RESERVED2_MASK) | (((uint32_t)val) << PIP_STATS_ACR_RESERVED2_SHIFT))

#define  PIP_STATS_ACR_SAVE_RESTORE_MODE_MASK 0x10
#define  PIP_STATS_ACR_SAVE_RESTORE_MODE_SHIFT 4
#define  PIP_STATS_ACR_SAVE_RESTORE_MODE_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_SAVE_RESTORE_MODE_MASK) >> PIP_STATS_ACR_SAVE_RESTORE_MODE_SHIFT)
#define  PIP_STATS_ACR_SAVE_RESTORE_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_SAVE_RESTORE_MODE_MASK) | (((uint32_t)val) << PIP_STATS_ACR_SAVE_RESTORE_MODE_SHIFT))

#define  PIP_STATS_ACR_RESERVED3_MASK 0x8
#define  PIP_STATS_ACR_RESERVED3_SHIFT 3
#define  PIP_STATS_ACR_RESERVED3_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_RESERVED3_MASK) >> PIP_STATS_ACR_RESERVED3_SHIFT)
#define  PIP_STATS_ACR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_RESERVED3_MASK) | (((uint32_t)val) << PIP_STATS_ACR_RESERVED3_SHIFT))

#define  PIP_STATS_ACR_ACCUM_ENABLE_MASK 0x4
#define  PIP_STATS_ACR_ACCUM_ENABLE_SHIFT 2
#define  PIP_STATS_ACR_ACCUM_ENABLE_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_ACCUM_ENABLE_MASK) >> PIP_STATS_ACR_ACCUM_ENABLE_SHIFT)
#define  PIP_STATS_ACR_ACCUM_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_ACCUM_ENABLE_MASK) | (((uint32_t)val) << PIP_STATS_ACR_ACCUM_ENABLE_SHIFT))

#define  PIP_STATS_ACR_STATS_ENABLE_MASK 0x2
#define  PIP_STATS_ACR_STATS_ENABLE_SHIFT 1
#define  PIP_STATS_ACR_STATS_ENABLE_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_STATS_ENABLE_MASK) >> PIP_STATS_ACR_STATS_ENABLE_SHIFT)
#define  PIP_STATS_ACR_STATS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_STATS_ENABLE_MASK) | (((uint32_t)val) << PIP_STATS_ACR_STATS_ENABLE_SHIFT))

#define  PIP_STATS_ACR_BYPASS_ENABLE_MASK 0x1
#define  PIP_STATS_ACR_BYPASS_ENABLE_SHIFT 0
#define  PIP_STATS_ACR_BYPASS_ENABLE_MASK_SHIFT(reg) (((reg) & PIP_STATS_ACR_BYPASS_ENABLE_MASK) >> PIP_STATS_ACR_BYPASS_ENABLE_SHIFT)
#define  PIP_STATS_ACR_BYPASS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ACR_BYPASS_ENABLE_MASK) | (((uint32_t)val) << PIP_STATS_ACR_BYPASS_ENABLE_SHIFT))

//====================================================================
//Register: PIP_STATS Stats A Configuration Register (ASACR)
/** \brief This register configures the operation of stats_a by setting channel thresholds and the swath height.*/
//====================================================================

#define  PIP_STATS_ASACR_RESERVED1_MASK 0xf0000000
#define  PIP_STATS_ASACR_RESERVED1_SHIFT 28
#define  PIP_STATS_ASACR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASACR_RESERVED1_MASK) >> PIP_STATS_ASACR_RESERVED1_SHIFT)
#define  PIP_STATS_ASACR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASACR_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_ASACR_RESERVED1_SHIFT))

#define  PIP_STATS_ASACR_CH0_THRESH_MASK 0xf000000
#define  PIP_STATS_ASACR_CH0_THRESH_SHIFT 24
#define  PIP_STATS_ASACR_CH0_THRESH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASACR_CH0_THRESH_MASK) >> PIP_STATS_ASACR_CH0_THRESH_SHIFT)
#define  PIP_STATS_ASACR_CH0_THRESH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASACR_CH0_THRESH_MASK) | (((uint32_t)val) << PIP_STATS_ASACR_CH0_THRESH_SHIFT))

#define  PIP_STATS_ASACR_RESERVED2_MASK 0xf00000
#define  PIP_STATS_ASACR_RESERVED2_SHIFT 20
#define  PIP_STATS_ASACR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASACR_RESERVED2_MASK) >> PIP_STATS_ASACR_RESERVED2_SHIFT)
#define  PIP_STATS_ASACR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASACR_RESERVED2_MASK) | (((uint32_t)val) << PIP_STATS_ASACR_RESERVED2_SHIFT))

#define  PIP_STATS_ASACR_CH1_THRESH_MASK 0xf0000
#define  PIP_STATS_ASACR_CH1_THRESH_SHIFT 16
#define  PIP_STATS_ASACR_CH1_THRESH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASACR_CH1_THRESH_MASK) >> PIP_STATS_ASACR_CH1_THRESH_SHIFT)
#define  PIP_STATS_ASACR_CH1_THRESH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASACR_CH1_THRESH_MASK) | (((uint32_t)val) << PIP_STATS_ASACR_CH1_THRESH_SHIFT))

#define  PIP_STATS_ASACR_RESERVED3_MASK 0xf000
#define  PIP_STATS_ASACR_RESERVED3_SHIFT 12
#define  PIP_STATS_ASACR_RESERVED3_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASACR_RESERVED3_MASK) >> PIP_STATS_ASACR_RESERVED3_SHIFT)
#define  PIP_STATS_ASACR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASACR_RESERVED3_MASK) | (((uint32_t)val) << PIP_STATS_ASACR_RESERVED3_SHIFT))

#define  PIP_STATS_ASACR_CH4_THRESH_MASK 0xf00
#define  PIP_STATS_ASACR_CH4_THRESH_SHIFT 8
#define  PIP_STATS_ASACR_CH4_THRESH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASACR_CH4_THRESH_MASK) >> PIP_STATS_ASACR_CH4_THRESH_SHIFT)
#define  PIP_STATS_ASACR_CH4_THRESH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASACR_CH4_THRESH_MASK) | (((uint32_t)val) << PIP_STATS_ASACR_CH4_THRESH_SHIFT))

#define  PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_MASK 0xff
#define  PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_SHIFT 0
#define  PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_MASK) >> PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_SHIFT)
#define  PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_MASK) | (((uint32_t)val) << PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_SHIFT))

//====================================================================
//Register: PIP_STATS Stats B Configuration Register (ASBCR)
/** \brief This register configures the operation of stats_b by setting channel thresholds and the swath height.*/
//====================================================================

#define  PIP_STATS_ASBCR_RESERVED1_MASK 0xfff00000
#define  PIP_STATS_ASBCR_RESERVED1_SHIFT 20
#define  PIP_STATS_ASBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASBCR_RESERVED1_MASK) >> PIP_STATS_ASBCR_RESERVED1_SHIFT)
#define  PIP_STATS_ASBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASBCR_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_ASBCR_RESERVED1_SHIFT))

#define  PIP_STATS_ASBCR_CH2_THRESH_MASK 0xf0000
#define  PIP_STATS_ASBCR_CH2_THRESH_SHIFT 16
#define  PIP_STATS_ASBCR_CH2_THRESH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASBCR_CH2_THRESH_MASK) >> PIP_STATS_ASBCR_CH2_THRESH_SHIFT)
#define  PIP_STATS_ASBCR_CH2_THRESH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASBCR_CH2_THRESH_MASK) | (((uint32_t)val) << PIP_STATS_ASBCR_CH2_THRESH_SHIFT))

#define  PIP_STATS_ASBCR_RESERVED2_MASK 0xf000
#define  PIP_STATS_ASBCR_RESERVED2_SHIFT 12
#define  PIP_STATS_ASBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASBCR_RESERVED2_MASK) >> PIP_STATS_ASBCR_RESERVED2_SHIFT)
#define  PIP_STATS_ASBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASBCR_RESERVED2_MASK) | (((uint32_t)val) << PIP_STATS_ASBCR_RESERVED2_SHIFT))

#define  PIP_STATS_ASBCR_CH3_THRESH_MASK 0xf00
#define  PIP_STATS_ASBCR_CH3_THRESH_SHIFT 8
#define  PIP_STATS_ASBCR_CH3_THRESH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASBCR_CH3_THRESH_MASK) >> PIP_STATS_ASBCR_CH3_THRESH_SHIFT)
#define  PIP_STATS_ASBCR_CH3_THRESH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASBCR_CH3_THRESH_MASK) | (((uint32_t)val) << PIP_STATS_ASBCR_CH3_THRESH_SHIFT))

#define  PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_MASK 0xff
#define  PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_SHIFT 0
#define  PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_MASK) >> PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_SHIFT)
#define  PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_MASK) | (((uint32_t)val) << PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_SHIFT))

//====================================================================
//Register: PIP_STATS Stats C Configuration Register (ASCCR)
/** \brief This register configures the operation of stats_c by setting channel thresholds and the swath height.*/
//====================================================================

#define  PIP_STATS_ASCCR_RESERVED1_MASK 0xfffff000
#define  PIP_STATS_ASCCR_RESERVED1_SHIFT 12
#define  PIP_STATS_ASCCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASCCR_RESERVED1_MASK) >> PIP_STATS_ASCCR_RESERVED1_SHIFT)
#define  PIP_STATS_ASCCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASCCR_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_ASCCR_RESERVED1_SHIFT))

#define  PIP_STATS_ASCCR_CH5_THRESH_MASK 0xf00
#define  PIP_STATS_ASCCR_CH5_THRESH_SHIFT 8
#define  PIP_STATS_ASCCR_CH5_THRESH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASCCR_CH5_THRESH_MASK) >> PIP_STATS_ASCCR_CH5_THRESH_SHIFT)
#define  PIP_STATS_ASCCR_CH5_THRESH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASCCR_CH5_THRESH_MASK) | (((uint32_t)val) << PIP_STATS_ASCCR_CH5_THRESH_SHIFT))

#define  PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_MASK 0xff
#define  PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_SHIFT 0
#define  PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_MASK) >> PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_SHIFT)
#define  PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_MASK) | (((uint32_t)val) << PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_SHIFT))

//====================================================================
//Register: PIP_STATS Maximum Minimum 0 Register (AMM0R)
/** \brief This register contains the maximum and minimum stat for the contone channel 0 for the current swath.*/
//====================================================================

#define  PIP_STATS_AMM0R_CH0_MAXIMUM_MASK 0xffff0000
#define  PIP_STATS_AMM0R_CH0_MAXIMUM_SHIFT 16
#define  PIP_STATS_AMM0R_CH0_MAXIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM0R_CH0_MAXIMUM_MASK) >> PIP_STATS_AMM0R_CH0_MAXIMUM_SHIFT)
#define  PIP_STATS_AMM0R_CH0_MAXIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM0R_CH0_MAXIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM0R_CH0_MAXIMUM_SHIFT))

#define  PIP_STATS_AMM0R_CH0_MINIMUM_MASK 0xffff
#define  PIP_STATS_AMM0R_CH0_MINIMUM_SHIFT 0
#define  PIP_STATS_AMM0R_CH0_MINIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM0R_CH0_MINIMUM_MASK) >> PIP_STATS_AMM0R_CH0_MINIMUM_SHIFT)
#define  PIP_STATS_AMM0R_CH0_MINIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM0R_CH0_MINIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM0R_CH0_MINIMUM_SHIFT))

//====================================================================
//Register: PIP_STATS Maximum Minimum 1 Register (AMM1R)
/** \brief This register contains the maximum and minimum stat for the contone channel 1 for the current swath.*/
//====================================================================

#define  PIP_STATS_AMM1R_CH1_MAXIMUM_MASK 0xffff0000
#define  PIP_STATS_AMM1R_CH1_MAXIMUM_SHIFT 16
#define  PIP_STATS_AMM1R_CH1_MAXIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM1R_CH1_MAXIMUM_MASK) >> PIP_STATS_AMM1R_CH1_MAXIMUM_SHIFT)
#define  PIP_STATS_AMM1R_CH1_MAXIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM1R_CH1_MAXIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM1R_CH1_MAXIMUM_SHIFT))

#define  PIP_STATS_AMM1R_CH1_MINIMUM_MASK 0xffff
#define  PIP_STATS_AMM1R_CH1_MINIMUM_SHIFT 0
#define  PIP_STATS_AMM1R_CH1_MINIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM1R_CH1_MINIMUM_MASK) >> PIP_STATS_AMM1R_CH1_MINIMUM_SHIFT)
#define  PIP_STATS_AMM1R_CH1_MINIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM1R_CH1_MINIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM1R_CH1_MINIMUM_SHIFT))

//====================================================================
//Register: PIP_STATS Maximum Minimum 2 Register (AMM2R)
/** \brief This register contains the maximum and minimum stat for the contone channel 2 for the current swath.*/
//====================================================================

#define  PIP_STATS_AMM2R_CH2_MAXIMUM_MASK 0xffff0000
#define  PIP_STATS_AMM2R_CH2_MAXIMUM_SHIFT 16
#define  PIP_STATS_AMM2R_CH2_MAXIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM2R_CH2_MAXIMUM_MASK) >> PIP_STATS_AMM2R_CH2_MAXIMUM_SHIFT)
#define  PIP_STATS_AMM2R_CH2_MAXIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM2R_CH2_MAXIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM2R_CH2_MAXIMUM_SHIFT))

#define  PIP_STATS_AMM2R_CH2_MINIMUM_MASK 0xffff
#define  PIP_STATS_AMM2R_CH2_MINIMUM_SHIFT 0
#define  PIP_STATS_AMM2R_CH2_MINIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM2R_CH2_MINIMUM_MASK) >> PIP_STATS_AMM2R_CH2_MINIMUM_SHIFT)
#define  PIP_STATS_AMM2R_CH2_MINIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM2R_CH2_MINIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM2R_CH2_MINIMUM_SHIFT))

//====================================================================
//Register: PIP_STATS Maximum Minimum 3 Register (AMM3R)
/** \brief This register contains the maximum and minimum stat for the contone channel 3 for the current swath.*/
//====================================================================

#define  PIP_STATS_AMM3R_CH3_MAXIMUM_MASK 0xffff0000
#define  PIP_STATS_AMM3R_CH3_MAXIMUM_SHIFT 16
#define  PIP_STATS_AMM3R_CH3_MAXIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM3R_CH3_MAXIMUM_MASK) >> PIP_STATS_AMM3R_CH3_MAXIMUM_SHIFT)
#define  PIP_STATS_AMM3R_CH3_MAXIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM3R_CH3_MAXIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM3R_CH3_MAXIMUM_SHIFT))

#define  PIP_STATS_AMM3R_CH3_MINIMUM_MASK 0xffff
#define  PIP_STATS_AMM3R_CH3_MINIMUM_SHIFT 0
#define  PIP_STATS_AMM3R_CH3_MINIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM3R_CH3_MINIMUM_MASK) >> PIP_STATS_AMM3R_CH3_MINIMUM_SHIFT)
#define  PIP_STATS_AMM3R_CH3_MINIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM3R_CH3_MINIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM3R_CH3_MINIMUM_SHIFT))

//====================================================================
//Register: PIP_STATS Maximum Minimum 4 Register (AMM4R)
/** \brief This register contains the maximum and minimum stat for the contone channel 4 for the current swath.*/
//====================================================================

#define  PIP_STATS_AMM4R_CH4_MAXIMUM_MASK 0xffff0000
#define  PIP_STATS_AMM4R_CH4_MAXIMUM_SHIFT 16
#define  PIP_STATS_AMM4R_CH4_MAXIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM4R_CH4_MAXIMUM_MASK) >> PIP_STATS_AMM4R_CH4_MAXIMUM_SHIFT)
#define  PIP_STATS_AMM4R_CH4_MAXIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM4R_CH4_MAXIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM4R_CH4_MAXIMUM_SHIFT))

#define  PIP_STATS_AMM4R_CH4_MINIMUM_MASK 0xffff
#define  PIP_STATS_AMM4R_CH4_MINIMUM_SHIFT 0
#define  PIP_STATS_AMM4R_CH4_MINIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM4R_CH4_MINIMUM_MASK) >> PIP_STATS_AMM4R_CH4_MINIMUM_SHIFT)
#define  PIP_STATS_AMM4R_CH4_MINIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM4R_CH4_MINIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM4R_CH4_MINIMUM_SHIFT))

//====================================================================
//Register: PIP_STATS Maximum Minimum 5 Register (AMM5R)
/** \brief This register contains the maximum and minimum stat for the contone channel 5 for the current swath.*/
//====================================================================

#define  PIP_STATS_AMM5R_CH5_MAXIMUM_MASK 0xffff0000
#define  PIP_STATS_AMM5R_CH5_MAXIMUM_SHIFT 16
#define  PIP_STATS_AMM5R_CH5_MAXIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM5R_CH5_MAXIMUM_MASK) >> PIP_STATS_AMM5R_CH5_MAXIMUM_SHIFT)
#define  PIP_STATS_AMM5R_CH5_MAXIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM5R_CH5_MAXIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM5R_CH5_MAXIMUM_SHIFT))

#define  PIP_STATS_AMM5R_CH5_MINIMUM_MASK 0xffff
#define  PIP_STATS_AMM5R_CH5_MINIMUM_SHIFT 0
#define  PIP_STATS_AMM5R_CH5_MINIMUM_MASK_SHIFT(reg) (((reg) & PIP_STATS_AMM5R_CH5_MINIMUM_MASK) >> PIP_STATS_AMM5R_CH5_MINIMUM_SHIFT)
#define  PIP_STATS_AMM5R_CH5_MINIMUM_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AMM5R_CH5_MINIMUM_MASK) | (((uint32_t)val) << PIP_STATS_AMM5R_CH5_MINIMUM_SHIFT))

//====================================================================
//Register: PIP_STATS Accumulator 0 Register (AA0R)
/** \brief This is a saturating counter which accumulates the pixel values for the
      current swath.*/
//====================================================================

#define  PIP_STATS_AA0R_RESERVED1_MASK 0xff000000
#define  PIP_STATS_AA0R_RESERVED1_SHIFT 24
#define  PIP_STATS_AA0R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA0R_RESERVED1_MASK) >> PIP_STATS_AA0R_RESERVED1_SHIFT)
#define  PIP_STATS_AA0R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA0R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AA0R_RESERVED1_SHIFT))

#define  PIP_STATS_AA0R_ACCUM_0_MASK 0xffffff
#define  PIP_STATS_AA0R_ACCUM_0_SHIFT 0
#define  PIP_STATS_AA0R_ACCUM_0_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA0R_ACCUM_0_MASK) >> PIP_STATS_AA0R_ACCUM_0_SHIFT)
#define  PIP_STATS_AA0R_ACCUM_0_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA0R_ACCUM_0_MASK) | (((uint32_t)val) << PIP_STATS_AA0R_ACCUM_0_SHIFT))

//====================================================================
//Register: PIP_STATS Accumulator 1 Register (AA1R)
/** \brief This is a saturating counter which accumulates the pixel values for the
      current swath.*/
//====================================================================

#define  PIP_STATS_AA1R_RESERVED1_MASK 0xff000000
#define  PIP_STATS_AA1R_RESERVED1_SHIFT 24
#define  PIP_STATS_AA1R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA1R_RESERVED1_MASK) >> PIP_STATS_AA1R_RESERVED1_SHIFT)
#define  PIP_STATS_AA1R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA1R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AA1R_RESERVED1_SHIFT))

#define  PIP_STATS_AA1R_ACCUM_1_MASK 0xffffff
#define  PIP_STATS_AA1R_ACCUM_1_SHIFT 0
#define  PIP_STATS_AA1R_ACCUM_1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA1R_ACCUM_1_MASK) >> PIP_STATS_AA1R_ACCUM_1_SHIFT)
#define  PIP_STATS_AA1R_ACCUM_1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA1R_ACCUM_1_MASK) | (((uint32_t)val) << PIP_STATS_AA1R_ACCUM_1_SHIFT))

//====================================================================
//Register: PIP_STATS Accumulator 2 Register (AA2R)
/** \brief This is a saturating counter which accumulates the pixel values for the
      current swath.*/
//====================================================================

#define  PIP_STATS_AA2R_RESERVED1_MASK 0xff000000
#define  PIP_STATS_AA2R_RESERVED1_SHIFT 24
#define  PIP_STATS_AA2R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA2R_RESERVED1_MASK) >> PIP_STATS_AA2R_RESERVED1_SHIFT)
#define  PIP_STATS_AA2R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA2R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AA2R_RESERVED1_SHIFT))

#define  PIP_STATS_AA2R_ACCUM_2_MASK 0xffffff
#define  PIP_STATS_AA2R_ACCUM_2_SHIFT 0
#define  PIP_STATS_AA2R_ACCUM_2_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA2R_ACCUM_2_MASK) >> PIP_STATS_AA2R_ACCUM_2_SHIFT)
#define  PIP_STATS_AA2R_ACCUM_2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA2R_ACCUM_2_MASK) | (((uint32_t)val) << PIP_STATS_AA2R_ACCUM_2_SHIFT))

//====================================================================
//Register: PIP_STATS Accumulator 3 Register (AA3R)
/** \brief This is a saturating counter which accumulates the pixel values for the
      current swath.*/
//====================================================================

#define  PIP_STATS_AA3R_RESERVED1_MASK 0xff000000
#define  PIP_STATS_AA3R_RESERVED1_SHIFT 24
#define  PIP_STATS_AA3R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA3R_RESERVED1_MASK) >> PIP_STATS_AA3R_RESERVED1_SHIFT)
#define  PIP_STATS_AA3R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA3R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AA3R_RESERVED1_SHIFT))

#define  PIP_STATS_AA3R_ACCUM_3_MASK 0xffffff
#define  PIP_STATS_AA3R_ACCUM_3_SHIFT 0
#define  PIP_STATS_AA3R_ACCUM_3_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA3R_ACCUM_3_MASK) >> PIP_STATS_AA3R_ACCUM_3_SHIFT)
#define  PIP_STATS_AA3R_ACCUM_3_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA3R_ACCUM_3_MASK) | (((uint32_t)val) << PIP_STATS_AA3R_ACCUM_3_SHIFT))

//====================================================================
//Register: PIP_STATS Accumulator 4 Register (AA4R)
/** \brief This is a saturating counter which accumulates the pixel values for the
      current swath.*/
//====================================================================

#define  PIP_STATS_AA4R_RESERVED1_MASK 0xff000000
#define  PIP_STATS_AA4R_RESERVED1_SHIFT 24
#define  PIP_STATS_AA4R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA4R_RESERVED1_MASK) >> PIP_STATS_AA4R_RESERVED1_SHIFT)
#define  PIP_STATS_AA4R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA4R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AA4R_RESERVED1_SHIFT))

#define  PIP_STATS_AA4R_ACCUM_4_MASK 0xffffff
#define  PIP_STATS_AA4R_ACCUM_4_SHIFT 0
#define  PIP_STATS_AA4R_ACCUM_4_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA4R_ACCUM_4_MASK) >> PIP_STATS_AA4R_ACCUM_4_SHIFT)
#define  PIP_STATS_AA4R_ACCUM_4_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA4R_ACCUM_4_MASK) | (((uint32_t)val) << PIP_STATS_AA4R_ACCUM_4_SHIFT))

//====================================================================
//Register: PIP_STATS Accumulator 5 Register (AA5R)
/** \brief This is a saturating counter which accumulates the pixel values for the
      current swath.*/
//====================================================================

#define  PIP_STATS_AA5R_RESERVED1_MASK 0xff000000
#define  PIP_STATS_AA5R_RESERVED1_SHIFT 24
#define  PIP_STATS_AA5R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA5R_RESERVED1_MASK) >> PIP_STATS_AA5R_RESERVED1_SHIFT)
#define  PIP_STATS_AA5R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA5R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AA5R_RESERVED1_SHIFT))

#define  PIP_STATS_AA5R_ACCUM_5_MASK 0xffffff
#define  PIP_STATS_AA5R_ACCUM_5_SHIFT 0
#define  PIP_STATS_AA5R_ACCUM_5_MASK_SHIFT(reg) (((reg) & PIP_STATS_AA5R_ACCUM_5_MASK) >> PIP_STATS_AA5R_ACCUM_5_SHIFT)
#define  PIP_STATS_AA5R_ACCUM_5_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AA5R_ACCUM_5_MASK) | (((uint32_t)val) << PIP_STATS_AA5R_ACCUM_5_SHIFT))

//====================================================================
//Register: PIP_STATS Status Register (ASR)
/** \brief This register contains status flags. If any of the flags are set, it means some type of error has occurred. All flags will be reset when a start-of-image (SOI) data type is detected.*/
//====================================================================

#define  PIP_STATS_ASR_RESERVED1_MASK 0xfffffe00
#define  PIP_STATS_ASR_RESERVED1_SHIFT 9
#define  PIP_STATS_ASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASR_RESERVED1_MASK) >> PIP_STATS_ASR_RESERVED1_SHIFT)
#define  PIP_STATS_ASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASR_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_ASR_RESERVED1_SHIFT))

#define  PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_MASK 0x100
#define  PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_SHIFT 8
#define  PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_MASK) >> PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_SHIFT)
#define  PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_MASK) | (((uint32_t)val) << PIP_STATS_ASR_LINE_C_WIDTH_MISMATCH_SHIFT))

#define  PIP_STATS_ASR_RESERVED2_MASK 0xe0
#define  PIP_STATS_ASR_RESERVED2_SHIFT 5
#define  PIP_STATS_ASR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASR_RESERVED2_MASK) >> PIP_STATS_ASR_RESERVED2_SHIFT)
#define  PIP_STATS_ASR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASR_RESERVED2_MASK) | (((uint32_t)val) << PIP_STATS_ASR_RESERVED2_SHIFT))

#define  PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_MASK 0x10
#define  PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_SHIFT 4
#define  PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_MASK) >> PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_SHIFT)
#define  PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_MASK) | (((uint32_t)val) << PIP_STATS_ASR_LINE_B_WIDTH_MISMATCH_SHIFT))

#define  PIP_STATS_ASR_RESERVED3_MASK 0xe
#define  PIP_STATS_ASR_RESERVED3_SHIFT 1
#define  PIP_STATS_ASR_RESERVED3_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASR_RESERVED3_MASK) >> PIP_STATS_ASR_RESERVED3_SHIFT)
#define  PIP_STATS_ASR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASR_RESERVED3_MASK) | (((uint32_t)val) << PIP_STATS_ASR_RESERVED3_SHIFT))

#define  PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_MASK 0x1
#define  PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_SHIFT 0
#define  PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_MASK_SHIFT(reg) (((reg) & PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_MASK) >> PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_SHIFT)
#define  PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_MASK) | (((uint32_t)val) << PIP_STATS_ASR_LINE_A_WIDTH_MISMATCH_SHIFT))

//====================================================================
//Register: PIP_STATS Debug 1 Register (AD1R)
/** \brief This register exposes internal states that are used in the operation of the stats_a instance of PIP_STATS_stats.*/
//====================================================================

#define  PIP_STATS_AD1R_RESERVED1_MASK 0x80000000
#define  PIP_STATS_AD1R_RESERVED1_SHIFT 31
#define  PIP_STATS_AD1R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD1R_RESERVED1_MASK) >> PIP_STATS_AD1R_RESERVED1_SHIFT)
#define  PIP_STATS_AD1R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD1R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AD1R_RESERVED1_SHIFT))

#define  PIP_STATS_AD1R_STATS_A_Q_STATS_MASK 0x70000000
#define  PIP_STATS_AD1R_STATS_A_Q_STATS_SHIFT 28
#define  PIP_STATS_AD1R_STATS_A_Q_STATS_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD1R_STATS_A_Q_STATS_MASK) >> PIP_STATS_AD1R_STATS_A_Q_STATS_SHIFT)
#define  PIP_STATS_AD1R_STATS_A_Q_STATS_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD1R_STATS_A_Q_STATS_MASK) | (((uint32_t)val) << PIP_STATS_AD1R_STATS_A_Q_STATS_SHIFT))

#define  PIP_STATS_AD1R_RESERVED2_MASK 0xf000000
#define  PIP_STATS_AD1R_RESERVED2_SHIFT 24
#define  PIP_STATS_AD1R_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD1R_RESERVED2_MASK) >> PIP_STATS_AD1R_RESERVED2_SHIFT)
#define  PIP_STATS_AD1R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD1R_RESERVED2_MASK) | (((uint32_t)val) << PIP_STATS_AD1R_RESERVED2_SHIFT))

#define  PIP_STATS_AD1R_STATS_A_ROW_COUNT_MASK 0xff0000
#define  PIP_STATS_AD1R_STATS_A_ROW_COUNT_SHIFT 16
#define  PIP_STATS_AD1R_STATS_A_ROW_COUNT_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD1R_STATS_A_ROW_COUNT_MASK) >> PIP_STATS_AD1R_STATS_A_ROW_COUNT_SHIFT)
#define  PIP_STATS_AD1R_STATS_A_ROW_COUNT_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD1R_STATS_A_ROW_COUNT_MASK) | (((uint32_t)val) << PIP_STATS_AD1R_STATS_A_ROW_COUNT_SHIFT))

#define  PIP_STATS_AD1R_STATS_A_INDEX_MASK 0xffff
#define  PIP_STATS_AD1R_STATS_A_INDEX_SHIFT 0
#define  PIP_STATS_AD1R_STATS_A_INDEX_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD1R_STATS_A_INDEX_MASK) >> PIP_STATS_AD1R_STATS_A_INDEX_SHIFT)
#define  PIP_STATS_AD1R_STATS_A_INDEX_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD1R_STATS_A_INDEX_MASK) | (((uint32_t)val) << PIP_STATS_AD1R_STATS_A_INDEX_SHIFT))

//====================================================================
//Register: PIP_STATS Debug 2 Register (AD2R)
/** \brief This register exposes internal states that are used in the operation of the stats_b instance of PIP_STATS_stats.*/
//====================================================================

#define  PIP_STATS_AD2R_RESERVED1_MASK 0x80000000
#define  PIP_STATS_AD2R_RESERVED1_SHIFT 31
#define  PIP_STATS_AD2R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD2R_RESERVED1_MASK) >> PIP_STATS_AD2R_RESERVED1_SHIFT)
#define  PIP_STATS_AD2R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD2R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AD2R_RESERVED1_SHIFT))

#define  PIP_STATS_AD2R_STATS_B_Q_STATS_MASK 0x70000000
#define  PIP_STATS_AD2R_STATS_B_Q_STATS_SHIFT 28
#define  PIP_STATS_AD2R_STATS_B_Q_STATS_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD2R_STATS_B_Q_STATS_MASK) >> PIP_STATS_AD2R_STATS_B_Q_STATS_SHIFT)
#define  PIP_STATS_AD2R_STATS_B_Q_STATS_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD2R_STATS_B_Q_STATS_MASK) | (((uint32_t)val) << PIP_STATS_AD2R_STATS_B_Q_STATS_SHIFT))

#define  PIP_STATS_AD2R_RESERVED2_MASK 0xf000000
#define  PIP_STATS_AD2R_RESERVED2_SHIFT 24
#define  PIP_STATS_AD2R_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD2R_RESERVED2_MASK) >> PIP_STATS_AD2R_RESERVED2_SHIFT)
#define  PIP_STATS_AD2R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD2R_RESERVED2_MASK) | (((uint32_t)val) << PIP_STATS_AD2R_RESERVED2_SHIFT))

#define  PIP_STATS_AD2R_STATS_B_ROW_COUNT_MASK 0xff0000
#define  PIP_STATS_AD2R_STATS_B_ROW_COUNT_SHIFT 16
#define  PIP_STATS_AD2R_STATS_B_ROW_COUNT_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD2R_STATS_B_ROW_COUNT_MASK) >> PIP_STATS_AD2R_STATS_B_ROW_COUNT_SHIFT)
#define  PIP_STATS_AD2R_STATS_B_ROW_COUNT_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD2R_STATS_B_ROW_COUNT_MASK) | (((uint32_t)val) << PIP_STATS_AD2R_STATS_B_ROW_COUNT_SHIFT))

#define  PIP_STATS_AD2R_STATS_B_INDEX_MASK 0xffff
#define  PIP_STATS_AD2R_STATS_B_INDEX_SHIFT 0
#define  PIP_STATS_AD2R_STATS_B_INDEX_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD2R_STATS_B_INDEX_MASK) >> PIP_STATS_AD2R_STATS_B_INDEX_SHIFT)
#define  PIP_STATS_AD2R_STATS_B_INDEX_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD2R_STATS_B_INDEX_MASK) | (((uint32_t)val) << PIP_STATS_AD2R_STATS_B_INDEX_SHIFT))

//====================================================================
//Register: PIP_STATS Debug 3 Register (AD3R)
/** \brief This register exposes internal states that are used in the operation of the stats_c instance of PIP_STATS_stats.*/
//====================================================================

#define  PIP_STATS_AD3R_RESERVED1_MASK 0xc0000000
#define  PIP_STATS_AD3R_RESERVED1_SHIFT 30
#define  PIP_STATS_AD3R_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD3R_RESERVED1_MASK) >> PIP_STATS_AD3R_RESERVED1_SHIFT)
#define  PIP_STATS_AD3R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD3R_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_AD3R_RESERVED1_SHIFT))

#define  PIP_STATS_AD3R_STATS_C_Q_STATS_MASK 0x30000000
#define  PIP_STATS_AD3R_STATS_C_Q_STATS_SHIFT 28
#define  PIP_STATS_AD3R_STATS_C_Q_STATS_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD3R_STATS_C_Q_STATS_MASK) >> PIP_STATS_AD3R_STATS_C_Q_STATS_SHIFT)
#define  PIP_STATS_AD3R_STATS_C_Q_STATS_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD3R_STATS_C_Q_STATS_MASK) | (((uint32_t)val) << PIP_STATS_AD3R_STATS_C_Q_STATS_SHIFT))

#define  PIP_STATS_AD3R_RESERVED2_MASK 0xf000000
#define  PIP_STATS_AD3R_RESERVED2_SHIFT 24
#define  PIP_STATS_AD3R_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD3R_RESERVED2_MASK) >> PIP_STATS_AD3R_RESERVED2_SHIFT)
#define  PIP_STATS_AD3R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD3R_RESERVED2_MASK) | (((uint32_t)val) << PIP_STATS_AD3R_RESERVED2_SHIFT))

#define  PIP_STATS_AD3R_STATS_C_ROW_COUNT_MASK 0xff0000
#define  PIP_STATS_AD3R_STATS_C_ROW_COUNT_SHIFT 16
#define  PIP_STATS_AD3R_STATS_C_ROW_COUNT_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD3R_STATS_C_ROW_COUNT_MASK) >> PIP_STATS_AD3R_STATS_C_ROW_COUNT_SHIFT)
#define  PIP_STATS_AD3R_STATS_C_ROW_COUNT_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD3R_STATS_C_ROW_COUNT_MASK) | (((uint32_t)val) << PIP_STATS_AD3R_STATS_C_ROW_COUNT_SHIFT))

#define  PIP_STATS_AD3R_STATS_C_INDEX_MASK 0xffff
#define  PIP_STATS_AD3R_STATS_C_INDEX_SHIFT 0
#define  PIP_STATS_AD3R_STATS_C_INDEX_MASK_SHIFT(reg) (((reg) & PIP_STATS_AD3R_STATS_C_INDEX_MASK) >> PIP_STATS_AD3R_STATS_C_INDEX_SHIFT)
#define  PIP_STATS_AD3R_STATS_C_INDEX_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_AD3R_STATS_C_INDEX_MASK) | (((uint32_t)val) << PIP_STATS_AD3R_STATS_C_INDEX_SHIFT))

//====================================================================
//Register: Save Restore Address (SRA)
/** \brief This register is provided for the save portion of save/restore functionality. This register is used to set the address of the save/restore register to access. There are currently 15 32-bit registers that must be saved and restored, therefore valid addresses are 0x0 to 0xE.*/
//====================================================================

#define  PIP_STATS_SRA_RESERVED1_MASK 0xffffffe0
#define  PIP_STATS_SRA_RESERVED1_SHIFT 5
#define  PIP_STATS_SRA_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STATS_SRA_RESERVED1_MASK) >> PIP_STATS_SRA_RESERVED1_SHIFT)
#define  PIP_STATS_SRA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_SRA_RESERVED1_MASK) | (((uint32_t)val) << PIP_STATS_SRA_RESERVED1_SHIFT))

#define  PIP_STATS_SRA_SR_ADDR_MASK 0x1f
#define  PIP_STATS_SRA_SR_ADDR_SHIFT 0
#define  PIP_STATS_SRA_SR_ADDR_MASK_SHIFT(reg) (((reg) & PIP_STATS_SRA_SR_ADDR_MASK) >> PIP_STATS_SRA_SR_ADDR_SHIFT)
#define  PIP_STATS_SRA_SR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_SRA_SR_ADDR_MASK) | (((uint32_t)val) << PIP_STATS_SRA_SR_ADDR_SHIFT))

//====================================================================
//Register: Save Restore Read (SRR)
/** \brief This read-only register is provided for the save portion of save/restore functionality. The "save_restore_mode" bit of the configuration register must be set to a 1 to access this register.*/
//====================================================================

#define  PIP_STATS_SRR_SAVE_VAL_MASK 0xffffffff
#define  PIP_STATS_SRR_SAVE_VAL_SHIFT 0
#define  PIP_STATS_SRR_SAVE_VAL_MASK_SHIFT(reg) (((reg) & PIP_STATS_SRR_SAVE_VAL_MASK) >> PIP_STATS_SRR_SAVE_VAL_SHIFT)
#define  PIP_STATS_SRR_SAVE_VAL_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_SRR_SAVE_VAL_MASK) | (((uint32_t)val) << PIP_STATS_SRR_SAVE_VAL_SHIFT))

//====================================================================
//Register: Save Restore Write (SRW)
/** \brief This write-only register is provided for the restore portion of save/restore functionality.
      The "save_restore_mode" bit of the configuration register must be set to a 1 to access this register.*/
//====================================================================

#define  PIP_STATS_SRW_RESTORE_VAL_MASK 0xffffffff
#define  PIP_STATS_SRW_RESTORE_VAL_SHIFT 0
#define  PIP_STATS_SRW_RESTORE_VAL_MASK_SHIFT(reg) (((reg) & PIP_STATS_SRW_RESTORE_VAL_MASK) >> PIP_STATS_SRW_RESTORE_VAL_SHIFT)
#define  PIP_STATS_SRW_RESTORE_VAL_REPLACE_VAL(reg,val) (((reg) & ~PIP_STATS_SRW_RESTORE_VAL_MASK) | (((uint32_t)val) << PIP_STATS_SRW_RESTORE_VAL_SHIFT))

#endif // PIP_STATS
