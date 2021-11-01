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
 * \file YIPI_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _YIPI_REGMASKS_H_
#define _YIPI_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Yellow Insertion Pixel Identification (YIPI)
/** \brief Register Descriptions for the ColorPipe YIPI block*/
//
//====================================================================

//====================================================================
//Register: Yellow Insertion Pixel Identification control and configuration register (YCCR)
/** \brief Control and Configuration register for YIPI block.*/
//====================================================================

#define  YIPI_YCCR_B_MASK 0x80000000
#define  YIPI_YCCR_B_SHIFT 31
#define  YIPI_YCCR_B_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_B_MASK) >> YIPI_YCCR_B_SHIFT)
#define  YIPI_YCCR_B_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_B_MASK) | (((uint32_t)val) << YIPI_YCCR_B_SHIFT))

#define  YIPI_YCCR_CH_MASK 0x40000000
#define  YIPI_YCCR_CH_SHIFT 30
#define  YIPI_YCCR_CH_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_CH_MASK) >> YIPI_YCCR_CH_SHIFT)
#define  YIPI_YCCR_CH_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_CH_MASK) | (((uint32_t)val) << YIPI_YCCR_CH_SHIFT))

#define  YIPI_YCCR_SS_MASK 0x20000000
#define  YIPI_YCCR_SS_SHIFT 29
#define  YIPI_YCCR_SS_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_SS_MASK) >> YIPI_YCCR_SS_SHIFT)
#define  YIPI_YCCR_SS_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_SS_MASK) | (((uint32_t)val) << YIPI_YCCR_SS_SHIFT))

#define  YIPI_YCCR_SD_MASK 0x10000000
#define  YIPI_YCCR_SD_SHIFT 28
#define  YIPI_YCCR_SD_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_SD_MASK) >> YIPI_YCCR_SD_SHIFT)
#define  YIPI_YCCR_SD_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_SD_MASK) | (((uint32_t)val) << YIPI_YCCR_SD_SHIFT))

#define  YIPI_YCCR_RESERVED1_MASK 0xc000000
#define  YIPI_YCCR_RESERVED1_SHIFT 26
#define  YIPI_YCCR_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_RESERVED1_MASK) >> YIPI_YCCR_RESERVED1_SHIFT)
#define  YIPI_YCCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YCCR_RESERVED1_SHIFT))

#define  YIPI_YCCR_D_MASK 0x3000000
#define  YIPI_YCCR_D_SHIFT 24
#define  YIPI_YCCR_D_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_D_MASK) >> YIPI_YCCR_D_SHIFT)
#define  YIPI_YCCR_D_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_D_MASK) | (((uint32_t)val) << YIPI_YCCR_D_SHIFT))

#define  YIPI_YCCR_RESERVED2_MASK 0xc00000
#define  YIPI_YCCR_RESERVED2_SHIFT 22
#define  YIPI_YCCR_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_RESERVED2_MASK) >> YIPI_YCCR_RESERVED2_SHIFT)
#define  YIPI_YCCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YCCR_RESERVED2_SHIFT))

#define  YIPI_YCCR_V_MASK 0x3ff000
#define  YIPI_YCCR_V_SHIFT 12
#define  YIPI_YCCR_V_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_V_MASK) >> YIPI_YCCR_V_SHIFT)
#define  YIPI_YCCR_V_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_V_MASK) | (((uint32_t)val) << YIPI_YCCR_V_SHIFT))

#define  YIPI_YCCR_RESERVED3_MASK 0xc00
#define  YIPI_YCCR_RESERVED3_SHIFT 10
#define  YIPI_YCCR_RESERVED3_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_RESERVED3_MASK) >> YIPI_YCCR_RESERVED3_SHIFT)
#define  YIPI_YCCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_RESERVED3_MASK) | (((uint32_t)val) << YIPI_YCCR_RESERVED3_SHIFT))

#define  YIPI_YCCR_H_MASK 0x3ff
#define  YIPI_YCCR_H_SHIFT 0
#define  YIPI_YCCR_H_MASK_SHIFT(reg) (((reg) & YIPI_YCCR_H_MASK) >> YIPI_YCCR_H_SHIFT)
#define  YIPI_YCCR_H_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCCR_H_MASK) | (((uint32_t)val) << YIPI_YCCR_H_SHIFT))

//====================================================================
//Register: YIPI inserted code and function select register (YCFR)
/** \brief Values for inserted yellow pixels and function select for those pixels*/
//====================================================================

#define  YIPI_YCFR_C1_MASK 0xf0000000
#define  YIPI_YCFR_C1_SHIFT 28
#define  YIPI_YCFR_C1_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_C1_MASK) >> YIPI_YCFR_C1_SHIFT)
#define  YIPI_YCFR_C1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_C1_MASK) | (((uint32_t)val) << YIPI_YCFR_C1_SHIFT))

#define  YIPI_YCFR_C2_MASK 0xf000000
#define  YIPI_YCFR_C2_SHIFT 24
#define  YIPI_YCFR_C2_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_C2_MASK) >> YIPI_YCFR_C2_SHIFT)
#define  YIPI_YCFR_C2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_C2_MASK) | (((uint32_t)val) << YIPI_YCFR_C2_SHIFT))

#define  YIPI_YCFR_C3_MASK 0xf00000
#define  YIPI_YCFR_C3_SHIFT 20
#define  YIPI_YCFR_C3_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_C3_MASK) >> YIPI_YCFR_C3_SHIFT)
#define  YIPI_YCFR_C3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_C3_MASK) | (((uint32_t)val) << YIPI_YCFR_C3_SHIFT))

#define  YIPI_YCFR_C4_MASK 0xf0000
#define  YIPI_YCFR_C4_SHIFT 16
#define  YIPI_YCFR_C4_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_C4_MASK) >> YIPI_YCFR_C4_SHIFT)
#define  YIPI_YCFR_C4_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_C4_MASK) | (((uint32_t)val) << YIPI_YCFR_C4_SHIFT))

#define  YIPI_YCFR_RESERVED1_MASK 0xc000
#define  YIPI_YCFR_RESERVED1_SHIFT 14
#define  YIPI_YCFR_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_RESERVED1_MASK) >> YIPI_YCFR_RESERVED1_SHIFT)
#define  YIPI_YCFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YCFR_RESERVED1_SHIFT))

#define  YIPI_YCFR_F1_MASK 0x3000
#define  YIPI_YCFR_F1_SHIFT 12
#define  YIPI_YCFR_F1_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_F1_MASK) >> YIPI_YCFR_F1_SHIFT)
#define  YIPI_YCFR_F1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_F1_MASK) | (((uint32_t)val) << YIPI_YCFR_F1_SHIFT))

#define  YIPI_YCFR_RESERVED2_MASK 0xc00
#define  YIPI_YCFR_RESERVED2_SHIFT 10
#define  YIPI_YCFR_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_RESERVED2_MASK) >> YIPI_YCFR_RESERVED2_SHIFT)
#define  YIPI_YCFR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YCFR_RESERVED2_SHIFT))

#define  YIPI_YCFR_F2_MASK 0x300
#define  YIPI_YCFR_F2_SHIFT 8
#define  YIPI_YCFR_F2_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_F2_MASK) >> YIPI_YCFR_F2_SHIFT)
#define  YIPI_YCFR_F2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_F2_MASK) | (((uint32_t)val) << YIPI_YCFR_F2_SHIFT))

#define  YIPI_YCFR_RESERVED3_MASK 0xc0
#define  YIPI_YCFR_RESERVED3_SHIFT 6
#define  YIPI_YCFR_RESERVED3_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_RESERVED3_MASK) >> YIPI_YCFR_RESERVED3_SHIFT)
#define  YIPI_YCFR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_RESERVED3_MASK) | (((uint32_t)val) << YIPI_YCFR_RESERVED3_SHIFT))

#define  YIPI_YCFR_F3_MASK 0x30
#define  YIPI_YCFR_F3_SHIFT 4
#define  YIPI_YCFR_F3_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_F3_MASK) >> YIPI_YCFR_F3_SHIFT)
#define  YIPI_YCFR_F3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_F3_MASK) | (((uint32_t)val) << YIPI_YCFR_F3_SHIFT))

#define  YIPI_YCFR_RESERVED4_MASK 0xc
#define  YIPI_YCFR_RESERVED4_SHIFT 2
#define  YIPI_YCFR_RESERVED4_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_RESERVED4_MASK) >> YIPI_YCFR_RESERVED4_SHIFT)
#define  YIPI_YCFR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_RESERVED4_MASK) | (((uint32_t)val) << YIPI_YCFR_RESERVED4_SHIFT))

#define  YIPI_YCFR_F4_MASK 0x3
#define  YIPI_YCFR_F4_SHIFT 0
#define  YIPI_YCFR_F4_MASK_SHIFT(reg) (((reg) & YIPI_YCFR_F4_MASK) >> YIPI_YCFR_F4_SHIFT)
#define  YIPI_YCFR_F4_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YCFR_F4_MASK) | (((uint32_t)val) << YIPI_YCFR_F4_SHIFT))

//====================================================================
//Register: YIPI inserted array information lines 1 and 2 (YAL12)
/** \brief Lines 1 and 2 of 8X16 (128) bit array of dot information that FW will load for a particular page or job*/
//====================================================================

#define  YIPI_YAL12_A1_MASK 0xffff0000
#define  YIPI_YAL12_A1_SHIFT 16
#define  YIPI_YAL12_A1_MASK_SHIFT(reg) (((reg) & YIPI_YAL12_A1_MASK) >> YIPI_YAL12_A1_SHIFT)
#define  YIPI_YAL12_A1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL12_A1_MASK) | (((uint32_t)val) << YIPI_YAL12_A1_SHIFT))

#define  YIPI_YAL12_A2_MASK 0xffff
#define  YIPI_YAL12_A2_SHIFT 0
#define  YIPI_YAL12_A2_MASK_SHIFT(reg) (((reg) & YIPI_YAL12_A2_MASK) >> YIPI_YAL12_A2_SHIFT)
#define  YIPI_YAL12_A2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL12_A2_MASK) | (((uint32_t)val) << YIPI_YAL12_A2_SHIFT))

//====================================================================
//Register: YIPI inserted array information lines 3 and 4 (YAL34)
/** \brief Lines 3 and 4 of 8X16 (128) bit array of dot information that FW will load for a particular page or job*/
//====================================================================

#define  YIPI_YAL34_A3_MASK 0xffff0000
#define  YIPI_YAL34_A3_SHIFT 16
#define  YIPI_YAL34_A3_MASK_SHIFT(reg) (((reg) & YIPI_YAL34_A3_MASK) >> YIPI_YAL34_A3_SHIFT)
#define  YIPI_YAL34_A3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL34_A3_MASK) | (((uint32_t)val) << YIPI_YAL34_A3_SHIFT))

#define  YIPI_YAL34_A4_MASK 0xffff
#define  YIPI_YAL34_A4_SHIFT 0
#define  YIPI_YAL34_A4_MASK_SHIFT(reg) (((reg) & YIPI_YAL34_A4_MASK) >> YIPI_YAL34_A4_SHIFT)
#define  YIPI_YAL34_A4_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL34_A4_MASK) | (((uint32_t)val) << YIPI_YAL34_A4_SHIFT))

//====================================================================
//Register: YIPI inserted array information lines 5 and 6 (YAL56)
/** \brief Lines 5 and 6 of 8X16 (128) bit array of dot information that FW will load for a particular page or job*/
//====================================================================

#define  YIPI_YAL56_A5_MASK 0xffff0000
#define  YIPI_YAL56_A5_SHIFT 16
#define  YIPI_YAL56_A5_MASK_SHIFT(reg) (((reg) & YIPI_YAL56_A5_MASK) >> YIPI_YAL56_A5_SHIFT)
#define  YIPI_YAL56_A5_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL56_A5_MASK) | (((uint32_t)val) << YIPI_YAL56_A5_SHIFT))

#define  YIPI_YAL56_A6_MASK 0xffff
#define  YIPI_YAL56_A6_SHIFT 0
#define  YIPI_YAL56_A6_MASK_SHIFT(reg) (((reg) & YIPI_YAL56_A6_MASK) >> YIPI_YAL56_A6_SHIFT)
#define  YIPI_YAL56_A6_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL56_A6_MASK) | (((uint32_t)val) << YIPI_YAL56_A6_SHIFT))

//====================================================================
//Register: YIPI inserted array information lines 7 and 8 (YAL78)
/** \brief Lines 7 and 8 of 8X16 (128) bit array of dot information that FW will load for a particular page or job*/
//====================================================================

#define  YIPI_YAL78_A7_MASK 0xffff0000
#define  YIPI_YAL78_A7_SHIFT 16
#define  YIPI_YAL78_A7_MASK_SHIFT(reg) (((reg) & YIPI_YAL78_A7_MASK) >> YIPI_YAL78_A7_SHIFT)
#define  YIPI_YAL78_A7_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL78_A7_MASK) | (((uint32_t)val) << YIPI_YAL78_A7_SHIFT))

#define  YIPI_YAL78_A8_MASK 0xffff
#define  YIPI_YAL78_A8_SHIFT 0
#define  YIPI_YAL78_A8_MASK_SHIFT(reg) (((reg) & YIPI_YAL78_A8_MASK) >> YIPI_YAL78_A8_SHIFT)
#define  YIPI_YAL78_A8_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YAL78_A8_MASK) | (((uint32_t)val) << YIPI_YAL78_A8_SHIFT))

//====================================================================
//Register: YIPI image width register (YIWR)
/** \brief This register is programmed with the expected (width minus one) of the image.*/
//====================================================================

#define  YIPI_YIWR_RESERVED1_MASK 0xffff0000
#define  YIPI_YIWR_RESERVED1_SHIFT 16
#define  YIPI_YIWR_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YIWR_RESERVED1_MASK) >> YIPI_YIWR_RESERVED1_SHIFT)
#define  YIPI_YIWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YIWR_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YIWR_RESERVED1_SHIFT))

#define  YIPI_YIWR_W_MASK 0xffff
#define  YIPI_YIWR_W_SHIFT 0
#define  YIPI_YIWR_W_MASK_SHIFT(reg) (((reg) & YIPI_YIWR_W_MASK) >> YIPI_YIWR_W_SHIFT)
#define  YIPI_YIWR_W_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YIWR_W_MASK) | (((uint32_t)val) << YIPI_YIWR_W_SHIFT))

//====================================================================
//Register: YIPI Serpentine End-of-Line Configuration (YELC)
/** \brief This register needs to be programmed with two values. These values are used initialize counters when operating on a serpentined (R2L) line.*/
//====================================================================

#define  YIPI_YELC_RESERVED1_MASK 0xfff00000
#define  YIPI_YELC_RESERVED1_SHIFT 20
#define  YIPI_YELC_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YELC_RESERVED1_MASK) >> YIPI_YELC_RESERVED1_SHIFT)
#define  YIPI_YELC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YELC_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YELC_RESERVED1_SHIFT))

#define  YIPI_YELC_AI_MASK 0xf0000
#define  YIPI_YELC_AI_SHIFT 16
#define  YIPI_YELC_AI_MASK_SHIFT(reg) (((reg) & YIPI_YELC_AI_MASK) >> YIPI_YELC_AI_SHIFT)
#define  YIPI_YELC_AI_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YELC_AI_MASK) | (((uint32_t)val) << YIPI_YELC_AI_SHIFT))

#define  YIPI_YELC_RESERVED2_MASK 0xfc00
#define  YIPI_YELC_RESERVED2_SHIFT 10
#define  YIPI_YELC_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YELC_RESERVED2_MASK) >> YIPI_YELC_RESERVED2_SHIFT)
#define  YIPI_YELC_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YELC_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YELC_RESERVED2_SHIFT))

#define  YIPI_YELC_SI_MASK 0x3ff
#define  YIPI_YELC_SI_SHIFT 0
#define  YIPI_YELC_SI_MASK_SHIFT(reg) (((reg) & YIPI_YELC_SI_MASK) >> YIPI_YELC_SI_SHIFT)
#define  YIPI_YELC_SI_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YELC_SI_MASK) | (((uint32_t)val) << YIPI_YELC_SI_SHIFT))

//====================================================================
//Register: YIPI Status Register (YSR)
/** \brief This register contains status flags. If any of the flags is set, it means some type of error has occurred. All flags will be reset when a start-of-image (SOI) data type is detected.*/
//====================================================================

#define  YIPI_YSR_RESERVED1_MASK 0xffffe000
#define  YIPI_YSR_RESERVED1_SHIFT 13
#define  YIPI_YSR_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YSR_RESERVED1_MASK) >> YIPI_YSR_RESERVED1_SHIFT)
#define  YIPI_YSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YSR_RESERVED1_SHIFT))

#define  YIPI_YSR_S3_MASK 0x1000
#define  YIPI_YSR_S3_SHIFT 12
#define  YIPI_YSR_S3_MASK_SHIFT(reg) (((reg) & YIPI_YSR_S3_MASK) >> YIPI_YSR_S3_SHIFT)
#define  YIPI_YSR_S3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_S3_MASK) | (((uint32_t)val) << YIPI_YSR_S3_SHIFT))

#define  YIPI_YSR_RESERVED2_MASK 0xe00
#define  YIPI_YSR_RESERVED2_SHIFT 9
#define  YIPI_YSR_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YSR_RESERVED2_MASK) >> YIPI_YSR_RESERVED2_SHIFT)
#define  YIPI_YSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YSR_RESERVED2_SHIFT))

#define  YIPI_YSR_S2_MASK 0x100
#define  YIPI_YSR_S2_SHIFT 8
#define  YIPI_YSR_S2_MASK_SHIFT(reg) (((reg) & YIPI_YSR_S2_MASK) >> YIPI_YSR_S2_SHIFT)
#define  YIPI_YSR_S2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_S2_MASK) | (((uint32_t)val) << YIPI_YSR_S2_SHIFT))

#define  YIPI_YSR_RESERVED3_MASK 0xe0
#define  YIPI_YSR_RESERVED3_SHIFT 5
#define  YIPI_YSR_RESERVED3_MASK_SHIFT(reg) (((reg) & YIPI_YSR_RESERVED3_MASK) >> YIPI_YSR_RESERVED3_SHIFT)
#define  YIPI_YSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_RESERVED3_MASK) | (((uint32_t)val) << YIPI_YSR_RESERVED3_SHIFT))

#define  YIPI_YSR_S1_MASK 0x10
#define  YIPI_YSR_S1_SHIFT 4
#define  YIPI_YSR_S1_MASK_SHIFT(reg) (((reg) & YIPI_YSR_S1_MASK) >> YIPI_YSR_S1_SHIFT)
#define  YIPI_YSR_S1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_S1_MASK) | (((uint32_t)val) << YIPI_YSR_S1_SHIFT))

#define  YIPI_YSR_RESERVED4_MASK 0xe
#define  YIPI_YSR_RESERVED4_SHIFT 1
#define  YIPI_YSR_RESERVED4_MASK_SHIFT(reg) (((reg) & YIPI_YSR_RESERVED4_MASK) >> YIPI_YSR_RESERVED4_SHIFT)
#define  YIPI_YSR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_RESERVED4_MASK) | (((uint32_t)val) << YIPI_YSR_RESERVED4_SHIFT))

#define  YIPI_YSR_S0_MASK 0x1
#define  YIPI_YSR_S0_SHIFT 0
#define  YIPI_YSR_S0_MASK_SHIFT(reg) (((reg) & YIPI_YSR_S0_MASK) >> YIPI_YSR_S0_SHIFT)
#define  YIPI_YSR_S0_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YSR_S0_MASK) | (((uint32_t)val) << YIPI_YSR_S0_SHIFT))

//====================================================================
//Register: YIPI Debug 0 Register (YD0R)
/** \brief This register is used for debug purposes and save and restore. It exposes the internal counts and other flops used by YIPI.*/
//====================================================================

#define  YIPI_YD0R_RESERVED1_MASK 0xfe000000
#define  YIPI_YD0R_RESERVED1_SHIFT 25
#define  YIPI_YD0R_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_RESERVED1_MASK) >> YIPI_YD0R_RESERVED1_SHIFT)
#define  YIPI_YD0R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YD0R_RESERVED1_SHIFT))

#define  YIPI_YD0R_STATE_MASK 0x1000000
#define  YIPI_YD0R_STATE_SHIFT 24
#define  YIPI_YD0R_STATE_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_STATE_MASK) >> YIPI_YD0R_STATE_SHIFT)
#define  YIPI_YD0R_STATE_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_STATE_MASK) | (((uint32_t)val) << YIPI_YD0R_STATE_SHIFT))

#define  YIPI_YD0R_RESERVED2_MASK 0xe00000
#define  YIPI_YD0R_RESERVED2_SHIFT 21
#define  YIPI_YD0R_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_RESERVED2_MASK) >> YIPI_YD0R_RESERVED2_SHIFT)
#define  YIPI_YD0R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YD0R_RESERVED2_SHIFT))

#define  YIPI_YD0R_S_DIR_MASK 0x100000
#define  YIPI_YD0R_S_DIR_SHIFT 20
#define  YIPI_YD0R_S_DIR_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_S_DIR_MASK) >> YIPI_YD0R_S_DIR_SHIFT)
#define  YIPI_YD0R_S_DIR_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_S_DIR_MASK) | (((uint32_t)val) << YIPI_YD0R_S_DIR_SHIFT))

#define  YIPI_YD0R_RESERVED3_MASK 0x80000
#define  YIPI_YD0R_RESERVED3_SHIFT 19
#define  YIPI_YD0R_RESERVED3_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_RESERVED3_MASK) >> YIPI_YD0R_RESERVED3_SHIFT)
#define  YIPI_YD0R_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_RESERVED3_MASK) | (((uint32_t)val) << YIPI_YD0R_RESERVED3_SHIFT))

#define  YIPI_YD0R_ARRAY_SELECT_MASK 0x70000
#define  YIPI_YD0R_ARRAY_SELECT_SHIFT 16
#define  YIPI_YD0R_ARRAY_SELECT_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_ARRAY_SELECT_MASK) >> YIPI_YD0R_ARRAY_SELECT_SHIFT)
#define  YIPI_YD0R_ARRAY_SELECT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_ARRAY_SELECT_MASK) | (((uint32_t)val) << YIPI_YD0R_ARRAY_SELECT_SHIFT))

#define  YIPI_YD0R_ARRAY_INDEX_MASK 0xf000
#define  YIPI_YD0R_ARRAY_INDEX_SHIFT 12
#define  YIPI_YD0R_ARRAY_INDEX_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_ARRAY_INDEX_MASK) >> YIPI_YD0R_ARRAY_INDEX_SHIFT)
#define  YIPI_YD0R_ARRAY_INDEX_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_ARRAY_INDEX_MASK) | (((uint32_t)val) << YIPI_YD0R_ARRAY_INDEX_SHIFT))

#define  YIPI_YD0R_RESERVED4_MASK 0xc00
#define  YIPI_YD0R_RESERVED4_SHIFT 10
#define  YIPI_YD0R_RESERVED4_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_RESERVED4_MASK) >> YIPI_YD0R_RESERVED4_SHIFT)
#define  YIPI_YD0R_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_RESERVED4_MASK) | (((uint32_t)val) << YIPI_YD0R_RESERVED4_SHIFT))

#define  YIPI_YD0R_ARRAY_SUB_INDEX_MASK 0x3ff
#define  YIPI_YD0R_ARRAY_SUB_INDEX_SHIFT 0
#define  YIPI_YD0R_ARRAY_SUB_INDEX_MASK_SHIFT(reg) (((reg) & YIPI_YD0R_ARRAY_SUB_INDEX_MASK) >> YIPI_YD0R_ARRAY_SUB_INDEX_SHIFT)
#define  YIPI_YD0R_ARRAY_SUB_INDEX_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD0R_ARRAY_SUB_INDEX_MASK) | (((uint32_t)val) << YIPI_YD0R_ARRAY_SUB_INDEX_SHIFT))

//====================================================================
//Register: YIPI Debug 1 Register (YD1R)
/** \brief This register is used for debug purposes and save and restore. It exposes the internal counts and other flops used by YIPI.*/
//====================================================================

#define  YIPI_YD1R_RESERVED1_MASK 0xc0000000
#define  YIPI_YD1R_RESERVED1_SHIFT 30
#define  YIPI_YD1R_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YD1R_RESERVED1_MASK) >> YIPI_YD1R_RESERVED1_SHIFT)
#define  YIPI_YD1R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD1R_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YD1R_RESERVED1_SHIFT))

#define  YIPI_YD1R_DCOUNT_MASK 0x30000000
#define  YIPI_YD1R_DCOUNT_SHIFT 28
#define  YIPI_YD1R_DCOUNT_MASK_SHIFT(reg) (((reg) & YIPI_YD1R_DCOUNT_MASK) >> YIPI_YD1R_DCOUNT_SHIFT)
#define  YIPI_YD1R_DCOUNT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD1R_DCOUNT_MASK) | (((uint32_t)val) << YIPI_YD1R_DCOUNT_SHIFT))

#define  YIPI_YD1R_RESERVED2_MASK 0xc000000
#define  YIPI_YD1R_RESERVED2_SHIFT 26
#define  YIPI_YD1R_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YD1R_RESERVED2_MASK) >> YIPI_YD1R_RESERVED2_SHIFT)
#define  YIPI_YD1R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD1R_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YD1R_RESERVED2_SHIFT))

#define  YIPI_YD1R_VCOUNT_MASK 0x3ff0000
#define  YIPI_YD1R_VCOUNT_SHIFT 16
#define  YIPI_YD1R_VCOUNT_MASK_SHIFT(reg) (((reg) & YIPI_YD1R_VCOUNT_MASK) >> YIPI_YD1R_VCOUNT_SHIFT)
#define  YIPI_YD1R_VCOUNT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD1R_VCOUNT_MASK) | (((uint32_t)val) << YIPI_YD1R_VCOUNT_SHIFT))

#define  YIPI_YD1R_HCOUNT_MASK 0xffff
#define  YIPI_YD1R_HCOUNT_SHIFT 0
#define  YIPI_YD1R_HCOUNT_MASK_SHIFT(reg) (((reg) & YIPI_YD1R_HCOUNT_MASK) >> YIPI_YD1R_HCOUNT_SHIFT)
#define  YIPI_YD1R_HCOUNT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YD1R_HCOUNT_MASK) | (((uint32_t)val) << YIPI_YD1R_HCOUNT_SHIFT))

//====================================================================
//Register: YIPI Restore 0 Register (YR0)
/** \brief This register is used for restore the state of the YIPI block when performing a save/restore.*/
//====================================================================

#define  YIPI_YR0_RESERVED1_MASK 0xfe000000
#define  YIPI_YR0_RESERVED1_SHIFT 25
#define  YIPI_YR0_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YR0_RESERVED1_MASK) >> YIPI_YR0_RESERVED1_SHIFT)
#define  YIPI_YR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YR0_RESERVED1_SHIFT))

#define  YIPI_YR0_STATE_MASK 0x1000000
#define  YIPI_YR0_STATE_SHIFT 24
#define  YIPI_YR0_STATE_MASK_SHIFT(reg) (((reg) & YIPI_YR0_STATE_MASK) >> YIPI_YR0_STATE_SHIFT)
#define  YIPI_YR0_STATE_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_STATE_MASK) | (((uint32_t)val) << YIPI_YR0_STATE_SHIFT))

#define  YIPI_YR0_RESERVED2_MASK 0xe00000
#define  YIPI_YR0_RESERVED2_SHIFT 21
#define  YIPI_YR0_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YR0_RESERVED2_MASK) >> YIPI_YR0_RESERVED2_SHIFT)
#define  YIPI_YR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YR0_RESERVED2_SHIFT))

#define  YIPI_YR0_S_DIR_MASK 0x100000
#define  YIPI_YR0_S_DIR_SHIFT 20
#define  YIPI_YR0_S_DIR_MASK_SHIFT(reg) (((reg) & YIPI_YR0_S_DIR_MASK) >> YIPI_YR0_S_DIR_SHIFT)
#define  YIPI_YR0_S_DIR_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_S_DIR_MASK) | (((uint32_t)val) << YIPI_YR0_S_DIR_SHIFT))

#define  YIPI_YR0_RESERVED3_MASK 0x80000
#define  YIPI_YR0_RESERVED3_SHIFT 19
#define  YIPI_YR0_RESERVED3_MASK_SHIFT(reg) (((reg) & YIPI_YR0_RESERVED3_MASK) >> YIPI_YR0_RESERVED3_SHIFT)
#define  YIPI_YR0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_RESERVED3_MASK) | (((uint32_t)val) << YIPI_YR0_RESERVED3_SHIFT))

#define  YIPI_YR0_ARRAY_SELECT_MASK 0x70000
#define  YIPI_YR0_ARRAY_SELECT_SHIFT 16
#define  YIPI_YR0_ARRAY_SELECT_MASK_SHIFT(reg) (((reg) & YIPI_YR0_ARRAY_SELECT_MASK) >> YIPI_YR0_ARRAY_SELECT_SHIFT)
#define  YIPI_YR0_ARRAY_SELECT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_ARRAY_SELECT_MASK) | (((uint32_t)val) << YIPI_YR0_ARRAY_SELECT_SHIFT))

#define  YIPI_YR0_ARRAY_INDEX_MASK 0xf000
#define  YIPI_YR0_ARRAY_INDEX_SHIFT 12
#define  YIPI_YR0_ARRAY_INDEX_MASK_SHIFT(reg) (((reg) & YIPI_YR0_ARRAY_INDEX_MASK) >> YIPI_YR0_ARRAY_INDEX_SHIFT)
#define  YIPI_YR0_ARRAY_INDEX_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_ARRAY_INDEX_MASK) | (((uint32_t)val) << YIPI_YR0_ARRAY_INDEX_SHIFT))

#define  YIPI_YR0_RESERVED4_MASK 0xc00
#define  YIPI_YR0_RESERVED4_SHIFT 10
#define  YIPI_YR0_RESERVED4_MASK_SHIFT(reg) (((reg) & YIPI_YR0_RESERVED4_MASK) >> YIPI_YR0_RESERVED4_SHIFT)
#define  YIPI_YR0_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_RESERVED4_MASK) | (((uint32_t)val) << YIPI_YR0_RESERVED4_SHIFT))

#define  YIPI_YR0_ARRAY_SUB_INDEX_MASK 0x3ff
#define  YIPI_YR0_ARRAY_SUB_INDEX_SHIFT 0
#define  YIPI_YR0_ARRAY_SUB_INDEX_MASK_SHIFT(reg) (((reg) & YIPI_YR0_ARRAY_SUB_INDEX_MASK) >> YIPI_YR0_ARRAY_SUB_INDEX_SHIFT)
#define  YIPI_YR0_ARRAY_SUB_INDEX_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR0_ARRAY_SUB_INDEX_MASK) | (((uint32_t)val) << YIPI_YR0_ARRAY_SUB_INDEX_SHIFT))

//====================================================================
//Register: YIPI Restore 1 Register (YR1)
/** \brief This register is used to restore the state of the YIPI block when performing a save/restore.*/
//====================================================================

#define  YIPI_YR1_RESERVED1_MASK 0xc0000000
#define  YIPI_YR1_RESERVED1_SHIFT 30
#define  YIPI_YR1_RESERVED1_MASK_SHIFT(reg) (((reg) & YIPI_YR1_RESERVED1_MASK) >> YIPI_YR1_RESERVED1_SHIFT)
#define  YIPI_YR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR1_RESERVED1_MASK) | (((uint32_t)val) << YIPI_YR1_RESERVED1_SHIFT))

#define  YIPI_YR1_DCOUNT_MASK 0x30000000
#define  YIPI_YR1_DCOUNT_SHIFT 28
#define  YIPI_YR1_DCOUNT_MASK_SHIFT(reg) (((reg) & YIPI_YR1_DCOUNT_MASK) >> YIPI_YR1_DCOUNT_SHIFT)
#define  YIPI_YR1_DCOUNT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR1_DCOUNT_MASK) | (((uint32_t)val) << YIPI_YR1_DCOUNT_SHIFT))

#define  YIPI_YR1_RESERVED2_MASK 0xc000000
#define  YIPI_YR1_RESERVED2_SHIFT 26
#define  YIPI_YR1_RESERVED2_MASK_SHIFT(reg) (((reg) & YIPI_YR1_RESERVED2_MASK) >> YIPI_YR1_RESERVED2_SHIFT)
#define  YIPI_YR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR1_RESERVED2_MASK) | (((uint32_t)val) << YIPI_YR1_RESERVED2_SHIFT))

#define  YIPI_YR1_VCOUNT_MASK 0x3ff0000
#define  YIPI_YR1_VCOUNT_SHIFT 16
#define  YIPI_YR1_VCOUNT_MASK_SHIFT(reg) (((reg) & YIPI_YR1_VCOUNT_MASK) >> YIPI_YR1_VCOUNT_SHIFT)
#define  YIPI_YR1_VCOUNT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR1_VCOUNT_MASK) | (((uint32_t)val) << YIPI_YR1_VCOUNT_SHIFT))

#define  YIPI_YR1_HCOUNT_MASK 0xffff
#define  YIPI_YR1_HCOUNT_SHIFT 0
#define  YIPI_YR1_HCOUNT_MASK_SHIFT(reg) (((reg) & YIPI_YR1_HCOUNT_MASK) >> YIPI_YR1_HCOUNT_SHIFT)
#define  YIPI_YR1_HCOUNT_REPLACE_VAL(reg,val) (((reg) & ~YIPI_YR1_HCOUNT_MASK) | (((uint32_t)val) << YIPI_YR1_HCOUNT_SHIFT))

#endif // YIPI
