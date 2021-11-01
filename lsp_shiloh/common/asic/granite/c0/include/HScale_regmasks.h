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
 * \file HScale_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HSCALE_REGMASKS_H_
#define _HSCALE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: HScale (HScale)
/** \brief Register Descriptions for the PIC HScale block*/
//
//====================================================================

//====================================================================
//Register: Horizontal Scaler Configuration Register (HSCR)
/** \brief General configuration register for HScale block.*/
//====================================================================

#define  HSCALE_HSCR_RESERVED1_MASK 0xfffffffc
#define  HSCALE_HSCR_RESERVED1_SHIFT 2
#define  HSCALE_HSCR_RESERVED1_MASK_SHIFT(reg) (((reg) & HSCALE_HSCR_RESERVED1_MASK) >> HSCALE_HSCR_RESERVED1_SHIFT)
#define  HSCALE_HSCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSCALE_HSCR_RESERVED1_MASK) | (((uint32_t)val) << HSCALE_HSCR_RESERVED1_SHIFT))

#define  HSCALE_HSCR_BYPASS1_MASK 0x2
#define  HSCALE_HSCR_BYPASS1_SHIFT 1
#define  HSCALE_HSCR_BYPASS1_MASK_SHIFT(reg) (((reg) & HSCALE_HSCR_BYPASS1_MASK) >> HSCALE_HSCR_BYPASS1_SHIFT)
#define  HSCALE_HSCR_BYPASS1_REPLACE_VAL(reg,val) (((reg) & ~HSCALE_HSCR_BYPASS1_MASK) | (((uint32_t)val) << HSCALE_HSCR_BYPASS1_SHIFT))

#define  HSCALE_HSCR_BYPASS0_MASK 0x1
#define  HSCALE_HSCR_BYPASS0_SHIFT 0
#define  HSCALE_HSCR_BYPASS0_MASK_SHIFT(reg) (((reg) & HSCALE_HSCR_BYPASS0_MASK) >> HSCALE_HSCR_BYPASS0_SHIFT)
#define  HSCALE_HSCR_BYPASS0_REPLACE_VAL(reg,val) (((reg) & ~HSCALE_HSCR_BYPASS0_MASK) | (((uint32_t)val) << HSCALE_HSCR_BYPASS0_SHIFT))

//====================================================================
//Register: Horizontal Scaler Scale Ch0 Register (HSSR0)
/** \brief Horizontal Scaler Scale Factor*/
//====================================================================

#define  HSCALE_HSSR0_RESERVED1_MASK 0xfffe0000
#define  HSCALE_HSSR0_RESERVED1_SHIFT 17
#define  HSCALE_HSSR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HSCALE_HSSR0_RESERVED1_MASK) >> HSCALE_HSSR0_RESERVED1_SHIFT)
#define  HSCALE_HSSR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSCALE_HSSR0_RESERVED1_MASK) | (((uint32_t)val) << HSCALE_HSSR0_RESERVED1_SHIFT))

#define  HSCALE_HSSR0_SCALE_MASK 0x1ffff
#define  HSCALE_HSSR0_SCALE_SHIFT 0
#define  HSCALE_HSSR0_SCALE_MASK_SHIFT(reg) (((reg) & HSCALE_HSSR0_SCALE_MASK) >> HSCALE_HSSR0_SCALE_SHIFT)
#define  HSCALE_HSSR0_SCALE_REPLACE_VAL(reg,val) (((reg) & ~HSCALE_HSSR0_SCALE_MASK) | (((uint32_t)val) << HSCALE_HSSR0_SCALE_SHIFT))

//====================================================================
//Register: Horizontal Scaler Scale Ch1 Register (HSSR1)
/** \brief Horizontal Scaler Scale Factor*/
//====================================================================

#define  HSCALE_HSSR1_RESERVED1_MASK 0xfffe0000
#define  HSCALE_HSSR1_RESERVED1_SHIFT 17
#define  HSCALE_HSSR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HSCALE_HSSR1_RESERVED1_MASK) >> HSCALE_HSSR1_RESERVED1_SHIFT)
#define  HSCALE_HSSR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HSCALE_HSSR1_RESERVED1_MASK) | (((uint32_t)val) << HSCALE_HSSR1_RESERVED1_SHIFT))

#define  HSCALE_HSSR1_SCALE_MASK 0x1ffff
#define  HSCALE_HSSR1_SCALE_SHIFT 0
#define  HSCALE_HSSR1_SCALE_MASK_SHIFT(reg) (((reg) & HSCALE_HSSR1_SCALE_MASK) >> HSCALE_HSSR1_SCALE_SHIFT)
#define  HSCALE_HSSR1_SCALE_REPLACE_VAL(reg,val) (((reg) & ~HSCALE_HSSR1_SCALE_MASK) | (((uint32_t)val) << HSCALE_HSSR1_SCALE_SHIFT))

#endif // HSCALE
