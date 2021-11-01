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
 * \file BDEnhance_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BDENHANCE_REGMASKS_H_
#define _BDENHANCE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BDEnhance (BDEnhance)
/** \brief Register Descriptions for the PIE BDEnhance block*/
//
//====================================================================

//====================================================================
//Register: BDE Configuration Register (BDECR)
/** \brief General configuration register for BDEnhance block.*/
//====================================================================

#define  BDENHANCE_BDECR_RESERVED1_MASK 0xfffffff8
#define  BDENHANCE_BDECR_RESERVED1_SHIFT 3
#define  BDENHANCE_BDECR_RESERVED1_MASK_SHIFT(reg) (((reg) & BDENHANCE_BDECR_RESERVED1_MASK) >> BDENHANCE_BDECR_RESERVED1_SHIFT)
#define  BDENHANCE_BDECR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BDENHANCE_BDECR_RESERVED1_MASK) | (((uint32_t)val) << BDENHANCE_BDECR_RESERVED1_SHIFT))

#define  BDENHANCE_BDECR_M_MASK 0x4
#define  BDENHANCE_BDECR_M_SHIFT 2
#define  BDENHANCE_BDECR_M_MASK_SHIFT(reg) (((reg) & BDENHANCE_BDECR_M_MASK) >> BDENHANCE_BDECR_M_SHIFT)
#define  BDENHANCE_BDECR_M_REPLACE_VAL(reg,val) (((reg) & ~BDENHANCE_BDECR_M_MASK) | (((uint32_t)val) << BDENHANCE_BDECR_M_SHIFT))

#define  BDENHANCE_BDECR_C_MASK 0x2
#define  BDENHANCE_BDECR_C_SHIFT 1
#define  BDENHANCE_BDECR_C_MASK_SHIFT(reg) (((reg) & BDENHANCE_BDECR_C_MASK) >> BDENHANCE_BDECR_C_SHIFT)
#define  BDENHANCE_BDECR_C_REPLACE_VAL(reg,val) (((reg) & ~BDENHANCE_BDECR_C_MASK) | (((uint32_t)val) << BDENHANCE_BDECR_C_SHIFT))

#define  BDENHANCE_BDECR_B_MASK 0x1
#define  BDENHANCE_BDECR_B_SHIFT 0
#define  BDENHANCE_BDECR_B_MASK_SHIFT(reg) (((reg) & BDENHANCE_BDECR_B_MASK) >> BDENHANCE_BDECR_B_SHIFT)
#define  BDENHANCE_BDECR_B_REPLACE_VAL(reg,val) (((reg) & ~BDENHANCE_BDECR_B_MASK) | (((uint32_t)val) << BDENHANCE_BDECR_B_SHIFT))

//====================================================================
//Register: BDE Look Up Table (BDELUT)
/** \brief The LUT (look up table) is used for Bit Depth Enhance. The processor requires Read/Write access to the LUT to set up the values needed. The CPU Access bit must be set in the BDE Configuration register in order to access the LUT.
Note that the processor view of the LUT is 256 registers, word addressed.*/
//====================================================================

#define  BDENHANCE_BDELUT_RESERVED1_MASK 0xffffc000
#define  BDENHANCE_BDELUT_RESERVED1_SHIFT 14
#define  BDENHANCE_BDELUT_RESERVED1_MASK_SHIFT(reg) (((reg) & BDENHANCE_BDELUT_RESERVED1_MASK) >> BDENHANCE_BDELUT_RESERVED1_SHIFT)
#define  BDENHANCE_BDELUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BDENHANCE_BDELUT_RESERVED1_MASK) | (((uint32_t)val) << BDENHANCE_BDELUT_RESERVED1_SHIFT))

#define  BDENHANCE_BDELUT_L_MASK 0x3fff
#define  BDENHANCE_BDELUT_L_SHIFT 0
#define  BDENHANCE_BDELUT_L_MASK_SHIFT(reg) (((reg) & BDENHANCE_BDELUT_L_MASK) >> BDENHANCE_BDELUT_L_SHIFT)
#define  BDENHANCE_BDELUT_L_REPLACE_VAL(reg,val) (((reg) & ~BDENHANCE_BDELUT_L_MASK) | (((uint32_t)val) << BDENHANCE_BDELUT_L_SHIFT))

#endif // BDENHANCE
