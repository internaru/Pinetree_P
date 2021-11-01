 /*
 **************************************************************************************
 *
 * Copyright (c) 2011 Marvell International, Ltd.
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
 * \file TCNSense_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TCNSENSE_REGMASKS_H_
#define _TCNSENSE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: TCNSense (TCNSense)
/** \brief Register Descriptions for the PIE TCNSense block*/
//
//====================================================================

//====================================================================
//Register: TCNS Configuration Register (TCNSCR)
/** \brief General configuration register for TCNSense block.*/
//====================================================================

#define  TCNSENSE_TCNSCR_RESERVED1_MASK 0xfffffffc
#define  TCNSENSE_TCNSCR_RESERVED1_SHIFT 2
#define  TCNSENSE_TCNSCR_RESERVED1_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSCR_RESERVED1_MASK) >> TCNSENSE_TCNSCR_RESERVED1_SHIFT)
#define  TCNSENSE_TCNSCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSCR_RESERVED1_MASK) | (((uint32_t)val) << TCNSENSE_TCNSCR_RESERVED1_SHIFT))

#define  TCNSENSE_TCNSCR_C_MASK 0x2
#define  TCNSENSE_TCNSCR_C_SHIFT 1
#define  TCNSENSE_TCNSCR_C_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSCR_C_MASK) >> TCNSENSE_TCNSCR_C_SHIFT)
#define  TCNSENSE_TCNSCR_C_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSCR_C_MASK) | (((uint32_t)val) << TCNSENSE_TCNSCR_C_SHIFT))

#define  TCNSENSE_TCNSCR_B_MASK 0x1
#define  TCNSENSE_TCNSCR_B_SHIFT 0
#define  TCNSENSE_TCNSCR_B_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSCR_B_MASK) >> TCNSENSE_TCNSCR_B_SHIFT)
#define  TCNSENSE_TCNSCR_B_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSCR_B_MASK) | (((uint32_t)val) << TCNSENSE_TCNSCR_B_SHIFT))

//====================================================================
//Register: TCNS Look Up Table (TCNSLUT)
/** \brief The LUT, look up table, is used for Tone Contrast Neutral Sensitivity. The processor requires Read/Write access to the LUT to set up the values needed. Access to the three LUTs is consolidated into a single register.*/
//====================================================================

#define  TCNSENSE_TCNSLUT_RESERVED1_MASK 0xc0000000
#define  TCNSENSE_TCNSLUT_RESERVED1_SHIFT 30
#define  TCNSENSE_TCNSLUT_RESERVED1_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSLUT_RESERVED1_MASK) >> TCNSENSE_TCNSLUT_RESERVED1_SHIFT)
#define  TCNSENSE_TCNSLUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSLUT_RESERVED1_MASK) | (((uint32_t)val) << TCNSENSE_TCNSLUT_RESERVED1_SHIFT))

#define  TCNSENSE_TCNSLUT_Y_MASK 0x3fc00000
#define  TCNSENSE_TCNSLUT_Y_SHIFT 22
#define  TCNSENSE_TCNSLUT_Y_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSLUT_Y_MASK) >> TCNSENSE_TCNSLUT_Y_SHIFT)
#define  TCNSENSE_TCNSLUT_Y_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSLUT_Y_MASK) | (((uint32_t)val) << TCNSENSE_TCNSLUT_Y_SHIFT))

#define  TCNSENSE_TCNSLUT_RESERVED2_MASK 0x380000
#define  TCNSENSE_TCNSLUT_RESERVED2_SHIFT 19
#define  TCNSENSE_TCNSLUT_RESERVED2_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSLUT_RESERVED2_MASK) >> TCNSENSE_TCNSLUT_RESERVED2_SHIFT)
#define  TCNSENSE_TCNSLUT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSLUT_RESERVED2_MASK) | (((uint32_t)val) << TCNSENSE_TCNSLUT_RESERVED2_SHIFT))

#define  TCNSENSE_TCNSLUT_CR_MASK 0x7f800
#define  TCNSENSE_TCNSLUT_CR_SHIFT 11
#define  TCNSENSE_TCNSLUT_CR_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSLUT_CR_MASK) >> TCNSENSE_TCNSLUT_CR_SHIFT)
#define  TCNSENSE_TCNSLUT_CR_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSLUT_CR_MASK) | (((uint32_t)val) << TCNSENSE_TCNSLUT_CR_SHIFT))

#define  TCNSENSE_TCNSLUT_RESERVED3_MASK 0x700
#define  TCNSENSE_TCNSLUT_RESERVED3_SHIFT 8
#define  TCNSENSE_TCNSLUT_RESERVED3_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSLUT_RESERVED3_MASK) >> TCNSENSE_TCNSLUT_RESERVED3_SHIFT)
#define  TCNSENSE_TCNSLUT_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSLUT_RESERVED3_MASK) | (((uint32_t)val) << TCNSENSE_TCNSLUT_RESERVED3_SHIFT))

#define  TCNSENSE_TCNSLUT_CB_MASK 0xff
#define  TCNSENSE_TCNSLUT_CB_SHIFT 0
#define  TCNSENSE_TCNSLUT_CB_MASK_SHIFT(reg) (((reg) & TCNSENSE_TCNSLUT_CB_MASK) >> TCNSENSE_TCNSLUT_CB_SHIFT)
#define  TCNSENSE_TCNSLUT_CB_REPLACE_VAL(reg,val) (((reg) & ~TCNSENSE_TCNSLUT_CB_MASK) | (((uint32_t)val) << TCNSENSE_TCNSLUT_CB_SHIFT))

#endif // TCNSENSE
