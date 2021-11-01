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
 * \file COLOR_SHIFT_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _COLOR_SHIFT_REGMASKS_H_
#define _COLOR_SHIFT_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: COLOR_SHIFT (COLOR_SHIFT)
/** \brief Register Descriptions for the CIS Color Shift Block*/
//
//====================================================================

//====================================================================
//Register: CIS Color Shift Configuration Register (CCSCR)
/** \brief This register controls the configuration of the CIS Color Shift Block*/
//====================================================================

#define  COLOR_SHIFT_CCSCR_RESERVED1_MASK 0xfffffe00
#define  COLOR_SHIFT_CCSCR_RESERVED1_SHIFT 9
#define  COLOR_SHIFT_CCSCR_RESERVED1_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSCR_RESERVED1_MASK) >> COLOR_SHIFT_CCSCR_RESERVED1_SHIFT)
#define  COLOR_SHIFT_CCSCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSCR_RESERVED1_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSCR_RESERVED1_SHIFT))

#define  COLOR_SHIFT_CCSCR_COLHEIGHT_MASK 0x1f0
#define  COLOR_SHIFT_CCSCR_COLHEIGHT_SHIFT 4
#define  COLOR_SHIFT_CCSCR_COLHEIGHT_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSCR_COLHEIGHT_MASK) >> COLOR_SHIFT_CCSCR_COLHEIGHT_SHIFT)
#define  COLOR_SHIFT_CCSCR_COLHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSCR_COLHEIGHT_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSCR_COLHEIGHT_SHIFT))

#define  COLOR_SHIFT_CCSCR_ROUND_MASK 0xc
#define  COLOR_SHIFT_CCSCR_ROUND_SHIFT 2
#define  COLOR_SHIFT_CCSCR_ROUND_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSCR_ROUND_MASK) >> COLOR_SHIFT_CCSCR_ROUND_SHIFT)
#define  COLOR_SHIFT_CCSCR_ROUND_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSCR_ROUND_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSCR_ROUND_SHIFT))

#define  COLOR_SHIFT_CCSCR_TWOLINE_MASK 0x2
#define  COLOR_SHIFT_CCSCR_TWOLINE_SHIFT 1
#define  COLOR_SHIFT_CCSCR_TWOLINE_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSCR_TWOLINE_MASK) >> COLOR_SHIFT_CCSCR_TWOLINE_SHIFT)
#define  COLOR_SHIFT_CCSCR_TWOLINE_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSCR_TWOLINE_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSCR_TWOLINE_SHIFT))

#define  COLOR_SHIFT_CCSCR_BYPASS_MASK 0x1
#define  COLOR_SHIFT_CCSCR_BYPASS_SHIFT 0
#define  COLOR_SHIFT_CCSCR_BYPASS_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSCR_BYPASS_MASK) >> COLOR_SHIFT_CCSCR_BYPASS_SHIFT)
#define  COLOR_SHIFT_CCSCR_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSCR_BYPASS_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSCR_BYPASS_SHIFT))

//====================================================================
//Register: CIS Color Shift Multiplier Mode Register (CCSMMR)
/** \brief This register controls the multiplier modes of the CIS Color Shift Block*/
//====================================================================

#define  COLOR_SHIFT_CCSMMR_RESERVED1_MASK 0xfffffc00
#define  COLOR_SHIFT_CCSMMR_RESERVED1_SHIFT 10
#define  COLOR_SHIFT_CCSMMR_RESERVED1_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMMR_RESERVED1_MASK) >> COLOR_SHIFT_CCSMMR_RESERVED1_SHIFT)
#define  COLOR_SHIFT_CCSMMR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMMR_RESERVED1_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMMR_RESERVED1_SHIFT))

#define  COLOR_SHIFT_CCSMMR_BLUE_MASK 0x300
#define  COLOR_SHIFT_CCSMMR_BLUE_SHIFT 8
#define  COLOR_SHIFT_CCSMMR_BLUE_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMMR_BLUE_MASK) >> COLOR_SHIFT_CCSMMR_BLUE_SHIFT)
#define  COLOR_SHIFT_CCSMMR_BLUE_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMMR_BLUE_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMMR_BLUE_SHIFT))

#define  COLOR_SHIFT_CCSMMR_RESERVED2_MASK 0xc0
#define  COLOR_SHIFT_CCSMMR_RESERVED2_SHIFT 6
#define  COLOR_SHIFT_CCSMMR_RESERVED2_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMMR_RESERVED2_MASK) >> COLOR_SHIFT_CCSMMR_RESERVED2_SHIFT)
#define  COLOR_SHIFT_CCSMMR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMMR_RESERVED2_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMMR_RESERVED2_SHIFT))

#define  COLOR_SHIFT_CCSMMR_GREEN_MASK 0x30
#define  COLOR_SHIFT_CCSMMR_GREEN_SHIFT 4
#define  COLOR_SHIFT_CCSMMR_GREEN_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMMR_GREEN_MASK) >> COLOR_SHIFT_CCSMMR_GREEN_SHIFT)
#define  COLOR_SHIFT_CCSMMR_GREEN_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMMR_GREEN_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMMR_GREEN_SHIFT))

#define  COLOR_SHIFT_CCSMMR_RESERVED3_MASK 0xc
#define  COLOR_SHIFT_CCSMMR_RESERVED3_SHIFT 2
#define  COLOR_SHIFT_CCSMMR_RESERVED3_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMMR_RESERVED3_MASK) >> COLOR_SHIFT_CCSMMR_RESERVED3_SHIFT)
#define  COLOR_SHIFT_CCSMMR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMMR_RESERVED3_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMMR_RESERVED3_SHIFT))

#define  COLOR_SHIFT_CCSMMR_RED_MASK 0x3
#define  COLOR_SHIFT_CCSMMR_RED_SHIFT 0
#define  COLOR_SHIFT_CCSMMR_RED_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMMR_RED_MASK) >> COLOR_SHIFT_CCSMMR_RED_SHIFT)
#define  COLOR_SHIFT_CCSMMR_RED_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMMR_RED_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMMR_RED_SHIFT))

//====================================================================
//Register: CIS Color Shift Multiplier Weights Register (CCSMWR)
/** \brief This register controls the multiplier weights of the CIS Color Shift Block*/
//====================================================================

#define  COLOR_SHIFT_CCSMWR_RESERVED1_MASK 0xffe00000
#define  COLOR_SHIFT_CCSMWR_RESERVED1_SHIFT 21
#define  COLOR_SHIFT_CCSMWR_RESERVED1_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMWR_RESERVED1_MASK) >> COLOR_SHIFT_CCSMWR_RESERVED1_SHIFT)
#define  COLOR_SHIFT_CCSMWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMWR_RESERVED1_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMWR_RESERVED1_SHIFT))

#define  COLOR_SHIFT_CCSMWR_BLUE_MASK 0x1f0000
#define  COLOR_SHIFT_CCSMWR_BLUE_SHIFT 16
#define  COLOR_SHIFT_CCSMWR_BLUE_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMWR_BLUE_MASK) >> COLOR_SHIFT_CCSMWR_BLUE_SHIFT)
#define  COLOR_SHIFT_CCSMWR_BLUE_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMWR_BLUE_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMWR_BLUE_SHIFT))

#define  COLOR_SHIFT_CCSMWR_RESERVED2_MASK 0xe000
#define  COLOR_SHIFT_CCSMWR_RESERVED2_SHIFT 13
#define  COLOR_SHIFT_CCSMWR_RESERVED2_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMWR_RESERVED2_MASK) >> COLOR_SHIFT_CCSMWR_RESERVED2_SHIFT)
#define  COLOR_SHIFT_CCSMWR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMWR_RESERVED2_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMWR_RESERVED2_SHIFT))

#define  COLOR_SHIFT_CCSMWR_GREEN_MASK 0x1f00
#define  COLOR_SHIFT_CCSMWR_GREEN_SHIFT 8
#define  COLOR_SHIFT_CCSMWR_GREEN_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMWR_GREEN_MASK) >> COLOR_SHIFT_CCSMWR_GREEN_SHIFT)
#define  COLOR_SHIFT_CCSMWR_GREEN_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMWR_GREEN_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMWR_GREEN_SHIFT))

#define  COLOR_SHIFT_CCSMWR_RESERVED3_MASK 0xe0
#define  COLOR_SHIFT_CCSMWR_RESERVED3_SHIFT 5
#define  COLOR_SHIFT_CCSMWR_RESERVED3_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMWR_RESERVED3_MASK) >> COLOR_SHIFT_CCSMWR_RESERVED3_SHIFT)
#define  COLOR_SHIFT_CCSMWR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMWR_RESERVED3_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMWR_RESERVED3_SHIFT))

#define  COLOR_SHIFT_CCSMWR_RED_MASK 0x1f
#define  COLOR_SHIFT_CCSMWR_RED_SHIFT 0
#define  COLOR_SHIFT_CCSMWR_RED_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSMWR_RED_MASK) >> COLOR_SHIFT_CCSMWR_RED_SHIFT)
#define  COLOR_SHIFT_CCSMWR_RED_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSMWR_RED_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSMWR_RED_SHIFT))

//====================================================================
//Register: CIS Color Shift LFSR Starting Seed Register (CCSLSSR)
/** \brief This register controls the LFSR starting seed value of the CIS Color Shift Block*/
//====================================================================

#define  COLOR_SHIFT_CCSLSSR_RESERVED1_MASK 0xfffff000
#define  COLOR_SHIFT_CCSLSSR_RESERVED1_SHIFT 12
#define  COLOR_SHIFT_CCSLSSR_RESERVED1_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSSR_RESERVED1_MASK) >> COLOR_SHIFT_CCSLSSR_RESERVED1_SHIFT)
#define  COLOR_SHIFT_CCSLSSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSSR_RESERVED1_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSSR_RESERVED1_SHIFT))

#define  COLOR_SHIFT_CCSLSSR_SEED_MASK 0xfff
#define  COLOR_SHIFT_CCSLSSR_SEED_SHIFT 0
#define  COLOR_SHIFT_CCSLSSR_SEED_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSSR_SEED_MASK) >> COLOR_SHIFT_CCSLSSR_SEED_SHIFT)
#define  COLOR_SHIFT_CCSLSSR_SEED_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSSR_SEED_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSSR_SEED_SHIFT))

//====================================================================
//Register: CIS Color Shift LFSR Status Register (CCSLSR)
/** \brief This register provides LFSR status information of the CIS Color Shift Block*/
//====================================================================

#define  COLOR_SHIFT_CCSLSR_RESERVED1_MASK 0xffe00000
#define  COLOR_SHIFT_CCSLSR_RESERVED1_SHIFT 21
#define  COLOR_SHIFT_CCSLSR_RESERVED1_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSR_RESERVED1_MASK) >> COLOR_SHIFT_CCSLSR_RESERVED1_SHIFT)
#define  COLOR_SHIFT_CCSLSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSR_RESERVED1_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSR_RESERVED1_SHIFT))

#define  COLOR_SHIFT_CCSLSR_BLUELFSR_MASK 0x1f0000
#define  COLOR_SHIFT_CCSLSR_BLUELFSR_SHIFT 16
#define  COLOR_SHIFT_CCSLSR_BLUELFSR_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSR_BLUELFSR_MASK) >> COLOR_SHIFT_CCSLSR_BLUELFSR_SHIFT)
#define  COLOR_SHIFT_CCSLSR_BLUELFSR_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSR_BLUELFSR_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSR_BLUELFSR_SHIFT))

#define  COLOR_SHIFT_CCSLSR_RESERVED2_MASK 0xe000
#define  COLOR_SHIFT_CCSLSR_RESERVED2_SHIFT 13
#define  COLOR_SHIFT_CCSLSR_RESERVED2_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSR_RESERVED2_MASK) >> COLOR_SHIFT_CCSLSR_RESERVED2_SHIFT)
#define  COLOR_SHIFT_CCSLSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSR_RESERVED2_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSR_RESERVED2_SHIFT))

#define  COLOR_SHIFT_CCSLSR_GREENLFSR_MASK 0x1f00
#define  COLOR_SHIFT_CCSLSR_GREENLFSR_SHIFT 8
#define  COLOR_SHIFT_CCSLSR_GREENLFSR_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSR_GREENLFSR_MASK) >> COLOR_SHIFT_CCSLSR_GREENLFSR_SHIFT)
#define  COLOR_SHIFT_CCSLSR_GREENLFSR_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSR_GREENLFSR_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSR_GREENLFSR_SHIFT))

#define  COLOR_SHIFT_CCSLSR_RESERVED3_MASK 0xe0
#define  COLOR_SHIFT_CCSLSR_RESERVED3_SHIFT 5
#define  COLOR_SHIFT_CCSLSR_RESERVED3_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSR_RESERVED3_MASK) >> COLOR_SHIFT_CCSLSR_RESERVED3_SHIFT)
#define  COLOR_SHIFT_CCSLSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSR_RESERVED3_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSR_RESERVED3_SHIFT))

#define  COLOR_SHIFT_CCSLSR_REDLFSR_MASK 0x1f
#define  COLOR_SHIFT_CCSLSR_REDLFSR_SHIFT 0
#define  COLOR_SHIFT_CCSLSR_REDLFSR_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSLSR_REDLFSR_MASK) >> COLOR_SHIFT_CCSLSR_REDLFSR_SHIFT)
#define  COLOR_SHIFT_CCSLSR_REDLFSR_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSLSR_REDLFSR_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSLSR_REDLFSR_SHIFT))

//====================================================================
//Register: CIS Color Shift Row Status Register (CCSRSR)
/** \brief This register provides Row status information of the CIS Color Shift Block*/
//====================================================================

#define  COLOR_SHIFT_CCSRSR_RESERVED1_MASK 0xffe00000
#define  COLOR_SHIFT_CCSRSR_RESERVED1_SHIFT 21
#define  COLOR_SHIFT_CCSRSR_RESERVED1_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSRSR_RESERVED1_MASK) >> COLOR_SHIFT_CCSRSR_RESERVED1_SHIFT)
#define  COLOR_SHIFT_CCSRSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSRSR_RESERVED1_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSRSR_RESERVED1_SHIFT))

#define  COLOR_SHIFT_CCSRSR_BLUEROW_MASK 0x1f0000
#define  COLOR_SHIFT_CCSRSR_BLUEROW_SHIFT 16
#define  COLOR_SHIFT_CCSRSR_BLUEROW_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSRSR_BLUEROW_MASK) >> COLOR_SHIFT_CCSRSR_BLUEROW_SHIFT)
#define  COLOR_SHIFT_CCSRSR_BLUEROW_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSRSR_BLUEROW_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSRSR_BLUEROW_SHIFT))

#define  COLOR_SHIFT_CCSRSR_RESERVED2_MASK 0xe000
#define  COLOR_SHIFT_CCSRSR_RESERVED2_SHIFT 13
#define  COLOR_SHIFT_CCSRSR_RESERVED2_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSRSR_RESERVED2_MASK) >> COLOR_SHIFT_CCSRSR_RESERVED2_SHIFT)
#define  COLOR_SHIFT_CCSRSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSRSR_RESERVED2_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSRSR_RESERVED2_SHIFT))

#define  COLOR_SHIFT_CCSRSR_GREENROW_MASK 0x1f00
#define  COLOR_SHIFT_CCSRSR_GREENROW_SHIFT 8
#define  COLOR_SHIFT_CCSRSR_GREENROW_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSRSR_GREENROW_MASK) >> COLOR_SHIFT_CCSRSR_GREENROW_SHIFT)
#define  COLOR_SHIFT_CCSRSR_GREENROW_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSRSR_GREENROW_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSRSR_GREENROW_SHIFT))

#define  COLOR_SHIFT_CCSRSR_RESERVED3_MASK 0xe0
#define  COLOR_SHIFT_CCSRSR_RESERVED3_SHIFT 5
#define  COLOR_SHIFT_CCSRSR_RESERVED3_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSRSR_RESERVED3_MASK) >> COLOR_SHIFT_CCSRSR_RESERVED3_SHIFT)
#define  COLOR_SHIFT_CCSRSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSRSR_RESERVED3_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSRSR_RESERVED3_SHIFT))

#define  COLOR_SHIFT_CCSRSR_REDROW_MASK 0x1f
#define  COLOR_SHIFT_CCSRSR_REDROW_SHIFT 0
#define  COLOR_SHIFT_CCSRSR_REDROW_MASK_SHIFT(reg) (((reg) & COLOR_SHIFT_CCSRSR_REDROW_MASK) >> COLOR_SHIFT_CCSRSR_REDROW_SHIFT)
#define  COLOR_SHIFT_CCSRSR_REDROW_REPLACE_VAL(reg,val) (((reg) & ~COLOR_SHIFT_CCSRSR_REDROW_MASK) | (((uint32_t)val) << COLOR_SHIFT_CCSRSR_REDROW_SHIFT))

#endif // COLOR_SHIFT
