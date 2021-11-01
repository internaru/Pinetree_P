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
 * \file PLNR_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PLNR_REGMASKS_H_
#define _PLNR_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Planar Registers (PLNR)
//
//====================================================================

//====================================================================
//Register: PLNR Configuration Register (PCR)
//====================================================================

#define  PLNR_PCR_RESERVED1_MASK 0xfffffff8
#define  PLNR_PCR_RESERVED1_SHIFT 3
#define  PLNR_PCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PLNR_PCR_RESERVED1_MASK) >> PLNR_PCR_RESERVED1_SHIFT)
#define  PLNR_PCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PCR_RESERVED1_MASK) | (((uint32_t)val) << PLNR_PCR_RESERVED1_SHIFT))

#define  PLNR_PCR_NOEOI_MASK 0x4
#define  PLNR_PCR_NOEOI_SHIFT 2
#define  PLNR_PCR_NOEOI_MASK_SHIFT(reg) (((reg) & PLNR_PCR_NOEOI_MASK) >> PLNR_PCR_NOEOI_SHIFT)
#define  PLNR_PCR_NOEOI_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PCR_NOEOI_MASK) | (((uint32_t)val) << PLNR_PCR_NOEOI_SHIFT))

#define  PLNR_PCR_NOSOI_MASK 0x2
#define  PLNR_PCR_NOSOI_SHIFT 1
#define  PLNR_PCR_NOSOI_MASK_SHIFT(reg) (((reg) & PLNR_PCR_NOSOI_MASK) >> PLNR_PCR_NOSOI_SHIFT)
#define  PLNR_PCR_NOSOI_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PCR_NOSOI_MASK) | (((uint32_t)val) << PLNR_PCR_NOSOI_SHIFT))

#define  PLNR_PCR_ENABLE_MASK 0x1
#define  PLNR_PCR_ENABLE_SHIFT 0
#define  PLNR_PCR_ENABLE_MASK_SHIFT(reg) (((reg) & PLNR_PCR_ENABLE_MASK) >> PLNR_PCR_ENABLE_SHIFT)
#define  PLNR_PCR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PCR_ENABLE_MASK) | (((uint32_t)val) << PLNR_PCR_ENABLE_SHIFT))

//====================================================================
//Register: PLNR Image Width Register (PIWR)
//====================================================================

#define  PLNR_PIWR_RESERVED1_MASK 0xffff0000
#define  PLNR_PIWR_RESERVED1_SHIFT 16
#define  PLNR_PIWR_RESERVED1_MASK_SHIFT(reg) (((reg) & PLNR_PIWR_RESERVED1_MASK) >> PLNR_PIWR_RESERVED1_SHIFT)
#define  PLNR_PIWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PIWR_RESERVED1_MASK) | (((uint32_t)val) << PLNR_PIWR_RESERVED1_SHIFT))

#define  PLNR_PIWR_IMAGEWIDTH_MASK 0xffff
#define  PLNR_PIWR_IMAGEWIDTH_SHIFT 0
#define  PLNR_PIWR_IMAGEWIDTH_MASK_SHIFT(reg) (((reg) & PLNR_PIWR_IMAGEWIDTH_MASK) >> PLNR_PIWR_IMAGEWIDTH_SHIFT)
#define  PLNR_PIWR_IMAGEWIDTH_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PIWR_IMAGEWIDTH_MASK) | (((uint32_t)val) << PLNR_PIWR_IMAGEWIDTH_SHIFT))

//====================================================================
//Register: PLNR Image Height Register (PIHR)
//====================================================================

#define  PLNR_PIHR_RESERVED1_MASK 0xffff0000
#define  PLNR_PIHR_RESERVED1_SHIFT 16
#define  PLNR_PIHR_RESERVED1_MASK_SHIFT(reg) (((reg) & PLNR_PIHR_RESERVED1_MASK) >> PLNR_PIHR_RESERVED1_SHIFT)
#define  PLNR_PIHR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PIHR_RESERVED1_MASK) | (((uint32_t)val) << PLNR_PIHR_RESERVED1_SHIFT))

#define  PLNR_PIHR_IMAGEHEIGHT_MASK 0xffff
#define  PLNR_PIHR_IMAGEHEIGHT_SHIFT 0
#define  PLNR_PIHR_IMAGEHEIGHT_MASK_SHIFT(reg) (((reg) & PLNR_PIHR_IMAGEHEIGHT_MASK) >> PLNR_PIHR_IMAGEHEIGHT_SHIFT)
#define  PLNR_PIHR_IMAGEHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PIHR_IMAGEHEIGHT_MASK) | (((uint32_t)val) << PLNR_PIHR_IMAGEHEIGHT_SHIFT))

//====================================================================
//Register: PLNR Write Register (Instance 1 of 3) (PWR0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  PLNR_PWR0_DATA_MASK 0xffffffff
#define  PLNR_PWR0_DATA_SHIFT 0
#define  PLNR_PWR0_DATA_MASK_SHIFT(reg) (((reg) & PLNR_PWR0_DATA_MASK) >> PLNR_PWR0_DATA_SHIFT)
#define  PLNR_PWR0_DATA_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PWR0_DATA_MASK) | (((uint32_t)val) << PLNR_PWR0_DATA_SHIFT))

//====================================================================
//Register: PLNR Write Register (Instance 2 of 3) (PWR1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  PLNR_PWR1_DATA_MASK 0xffffffff
#define  PLNR_PWR1_DATA_SHIFT 0
#define  PLNR_PWR1_DATA_MASK_SHIFT(reg) (((reg) & PLNR_PWR1_DATA_MASK) >> PLNR_PWR1_DATA_SHIFT)
#define  PLNR_PWR1_DATA_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PWR1_DATA_MASK) | (((uint32_t)val) << PLNR_PWR1_DATA_SHIFT))

//====================================================================
//Register: PLNR Write Register (Instance 3 of 3) (PWR2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  PLNR_PWR2_DATA_MASK 0xffffffff
#define  PLNR_PWR2_DATA_SHIFT 0
#define  PLNR_PWR2_DATA_MASK_SHIFT(reg) (((reg) & PLNR_PWR2_DATA_MASK) >> PLNR_PWR2_DATA_SHIFT)
#define  PLNR_PWR2_DATA_REPLACE_VAL(reg,val) (((reg) & ~PLNR_PWR2_DATA_MASK) | (((uint32_t)val) << PLNR_PWR2_DATA_SHIFT))

#endif // PLNR
