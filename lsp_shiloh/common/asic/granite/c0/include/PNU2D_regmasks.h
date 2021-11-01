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
 * \file PNU2D_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PNU2D_REGMASKS_H_
#define _PNU2D_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PNU2D: Camera PRNU/DSNU Support (PNU2D)
/** \brief Top-level register file for the Camera PRNU/DSNU Support block*/
//
//====================================================================

//====================================================================
//Register: Configuration Register (PNU2DCFG)
/** \brief This register should be programmed based on the size of the 2D image and the number of correction
coefficients stored in memory.*/
//====================================================================

#define  PNU2D_PNU2DCFG_RESERVED1_MASK 0xc0000000
#define  PNU2D_PNU2DCFG_RESERVED1_SHIFT 30
#define  PNU2D_PNU2DCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_RESERVED1_MASK) >> PNU2D_PNU2DCFG_RESERVED1_SHIFT)
#define  PNU2D_PNU2DCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_RESERVED1_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_RESERVED1_SHIFT))

#define  PNU2D_PNU2DCFG_LINEWIDTH_MASK 0x3ffff000
#define  PNU2D_PNU2DCFG_LINEWIDTH_SHIFT 12
#define  PNU2D_PNU2DCFG_LINEWIDTH_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_LINEWIDTH_MASK) >> PNU2D_PNU2DCFG_LINEWIDTH_SHIFT)
#define  PNU2D_PNU2DCFG_LINEWIDTH_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_LINEWIDTH_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_LINEWIDTH_SHIFT))

#define  PNU2D_PNU2DCFG_RESERVED2_MASK 0x800
#define  PNU2D_PNU2DCFG_RESERVED2_SHIFT 11
#define  PNU2D_PNU2DCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_RESERVED2_MASK) >> PNU2D_PNU2DCFG_RESERVED2_SHIFT)
#define  PNU2D_PNU2DCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_RESERVED2_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_RESERVED2_SHIFT))

#define  PNU2D_PNU2DCFG_XSTEP_MASK 0x700
#define  PNU2D_PNU2DCFG_XSTEP_SHIFT 8
#define  PNU2D_PNU2DCFG_XSTEP_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_XSTEP_MASK) >> PNU2D_PNU2DCFG_XSTEP_SHIFT)
#define  PNU2D_PNU2DCFG_XSTEP_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_XSTEP_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_XSTEP_SHIFT))

#define  PNU2D_PNU2DCFG_RESERVED3_MASK 0x80
#define  PNU2D_PNU2DCFG_RESERVED3_SHIFT 7
#define  PNU2D_PNU2DCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_RESERVED3_MASK) >> PNU2D_PNU2DCFG_RESERVED3_SHIFT)
#define  PNU2D_PNU2DCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_RESERVED3_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_RESERVED3_SHIFT))

#define  PNU2D_PNU2DCFG_YSTEP_MASK 0x70
#define  PNU2D_PNU2DCFG_YSTEP_SHIFT 4
#define  PNU2D_PNU2DCFG_YSTEP_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_YSTEP_MASK) >> PNU2D_PNU2DCFG_YSTEP_SHIFT)
#define  PNU2D_PNU2DCFG_YSTEP_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_YSTEP_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_YSTEP_SHIFT))

#define  PNU2D_PNU2DCFG_RESERVED4_MASK 0xe
#define  PNU2D_PNU2DCFG_RESERVED4_SHIFT 1
#define  PNU2D_PNU2DCFG_RESERVED4_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_RESERVED4_MASK) >> PNU2D_PNU2DCFG_RESERVED4_SHIFT)
#define  PNU2D_PNU2DCFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_RESERVED4_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_RESERVED4_SHIFT))

#define  PNU2D_PNU2DCFG_BYPASS_MASK 0x1
#define  PNU2D_PNU2DCFG_BYPASS_SHIFT 0
#define  PNU2D_PNU2DCFG_BYPASS_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DCFG_BYPASS_MASK) >> PNU2D_PNU2DCFG_BYPASS_SHIFT)
#define  PNU2D_PNU2DCFG_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DCFG_BYPASS_MASK) | (((uint32_t)val) << PNU2D_PNU2DCFG_BYPASS_SHIFT))

//====================================================================
//Register: PNU2D Bayer Configuration Register (PNU2DBCFG)
/** \brief Bayer configuration register.*/
//====================================================================

#define  PNU2D_PNU2DBCFG_RESERVED1_MASK 0xfffffff8
#define  PNU2D_PNU2DBCFG_RESERVED1_SHIFT 3
#define  PNU2D_PNU2DBCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBCFG_RESERVED1_MASK) >> PNU2D_PNU2DBCFG_RESERVED1_SHIFT)
#define  PNU2D_PNU2DBCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBCFG_RESERVED1_MASK) | (((uint32_t)val) << PNU2D_PNU2DBCFG_RESERVED1_SHIFT))

#define  PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_MASK 0x7
#define  PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_SHIFT 0
#define  PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_MASK) >> PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_SHIFT)
#define  PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_MASK) | (((uint32_t)val) << PNU2D_PNU2DBCFG_PRNU_BAYER_SHIFT_SHIFT))

//====================================================================
//Register: PNU2D PRNU Bayer Gain Register (PNU2DBGN)
/** \brief Register for setting optional sub-global gains corresponding to pixel location within a Bayer matrix.
There are four positions (0,0; 0,1; 1,0; 1,1) within this matrix. The PRNU values associated with the normalised
location (0,0) will not be adjusted The normalised channel should have the lowest sensitivity (highest gain).
The PRNU values corresponding to the other three Bayer locations will be gained down based on the values set
in this register.*/
//====================================================================

#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_MASK 0xff000000
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_SHIFT 24
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_MASK) >> PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_SHIFT)
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_MASK) | (((uint32_t)val) << PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_11_SHIFT))

#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_MASK 0xff0000
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_SHIFT 16
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_MASK) >> PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_SHIFT)
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_MASK) | (((uint32_t)val) << PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_10_SHIFT))

#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_MASK 0xff00
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_SHIFT 8
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_MASK) >> PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_SHIFT)
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_MASK) | (((uint32_t)val) << PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_01_SHIFT))

#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_MASK 0xff
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_SHIFT 0
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_MASK) >> PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_SHIFT)
#define  PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_MASK) | (((uint32_t)val) << PNU2D_PNU2DBGN_PRNU_BAYER_GAIN_00_SHIFT))

//====================================================================
//Register: PNU2D PRNU Bayer Offset Register 1 (PNU2DBOS1)
/** \brief Register for setting optional sub-global offsets corresponding to pixel location within a Bayer matrix.
There are four positions (0,0; 0,1; 1,0; 1,1) within this matrix. The PRNU values associated with the normalised
location (0,0) will not be adjusted. The normalised channel should have the lowest sensitivity (highest gain).
The PRNU values corresponding to Bayer locations (0,1) and (1,0) will be adjusted based on the values set
in this register. These are 15 bit signed numbers.*/
//====================================================================

#define  PNU2D_PNU2DBOS1_RESERVED1_MASK 0x80000000
#define  PNU2D_PNU2DBOS1_RESERVED1_SHIFT 31
#define  PNU2D_PNU2DBOS1_RESERVED1_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS1_RESERVED1_MASK) >> PNU2D_PNU2DBOS1_RESERVED1_SHIFT)
#define  PNU2D_PNU2DBOS1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS1_RESERVED1_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS1_RESERVED1_SHIFT))

#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_MASK 0x7fff0000
#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_SHIFT 16
#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_MASK) >> PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_SHIFT)
#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_01_SHIFT))

#define  PNU2D_PNU2DBOS1_RESERVED2_MASK 0x8000
#define  PNU2D_PNU2DBOS1_RESERVED2_SHIFT 15
#define  PNU2D_PNU2DBOS1_RESERVED2_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS1_RESERVED2_MASK) >> PNU2D_PNU2DBOS1_RESERVED2_SHIFT)
#define  PNU2D_PNU2DBOS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS1_RESERVED2_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS1_RESERVED2_SHIFT))

#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_MASK 0x7fff
#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_SHIFT 0
#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_MASK) >> PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_SHIFT)
#define  PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS1_PRNU_BAYER_OFFSET_00_SHIFT))

//====================================================================
//Register: PNU2D PRNU Bayer Offset Register 2 (PNU2DBOS2)
/** \brief Register for setting optional sub-global offsets corresponding to pixel location within a Bayer matrix.
There are four positions (0,0; 0,1; 1,0; 1,1) within this matrix. The PRNU values associated with the normalised
location (0,0) will not be adjusted. The normalised channel should have the lowest sensitivity (highest gain).
The PRNU value corresponding to Bayer location (1,1) will be adjusted based on the values set in this register.*/
//====================================================================

#define  PNU2D_PNU2DBOS2_RESERVED1_MASK 0x80000000
#define  PNU2D_PNU2DBOS2_RESERVED1_SHIFT 31
#define  PNU2D_PNU2DBOS2_RESERVED1_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS2_RESERVED1_MASK) >> PNU2D_PNU2DBOS2_RESERVED1_SHIFT)
#define  PNU2D_PNU2DBOS2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS2_RESERVED1_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS2_RESERVED1_SHIFT))

#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_MASK 0x7fff0000
#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_SHIFT 16
#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_MASK) >> PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_SHIFT)
#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_11_SHIFT))

#define  PNU2D_PNU2DBOS2_RESERVED2_MASK 0x8000
#define  PNU2D_PNU2DBOS2_RESERVED2_SHIFT 15
#define  PNU2D_PNU2DBOS2_RESERVED2_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS2_RESERVED2_MASK) >> PNU2D_PNU2DBOS2_RESERVED2_SHIFT)
#define  PNU2D_PNU2DBOS2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS2_RESERVED2_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS2_RESERVED2_SHIFT))

#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_MASK 0x7fff
#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_SHIFT 0
#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_MASK) >> PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_SHIFT)
#define  PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_MASK) | (((uint32_t)val) << PNU2D_PNU2DBOS2_PRNU_BAYER_OFFSET_10_SHIFT))

//====================================================================
//Register: PNU2D Status Register (PNU2DST)
/** \brief Gives the status of the interpolation machine.*/
//====================================================================

#define  PNU2D_PNU2DST_RESERVED1_MASK 0xfc000000
#define  PNU2D_PNU2DST_RESERVED1_SHIFT 26
#define  PNU2D_PNU2DST_RESERVED1_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_RESERVED1_MASK) >> PNU2D_PNU2DST_RESERVED1_SHIFT)
#define  PNU2D_PNU2DST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_RESERVED1_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_RESERVED1_SHIFT))

#define  PNU2D_PNU2DST_BAYER_POS_MASK 0x3000000
#define  PNU2D_PNU2DST_BAYER_POS_SHIFT 24
#define  PNU2D_PNU2DST_BAYER_POS_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_BAYER_POS_MASK) >> PNU2D_PNU2DST_BAYER_POS_SHIFT)
#define  PNU2D_PNU2DST_BAYER_POS_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_BAYER_POS_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_BAYER_POS_SHIFT))

#define  PNU2D_PNU2DST_RESERVED2_MASK 0xfe0000
#define  PNU2D_PNU2DST_RESERVED2_SHIFT 17
#define  PNU2D_PNU2DST_RESERVED2_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_RESERVED2_MASK) >> PNU2D_PNU2DST_RESERVED2_SHIFT)
#define  PNU2D_PNU2DST_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_RESERVED2_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_RESERVED2_SHIFT))

#define  PNU2D_PNU2DST_PRNU_OVERFLOW_MASK 0x10000
#define  PNU2D_PNU2DST_PRNU_OVERFLOW_SHIFT 16
#define  PNU2D_PNU2DST_PRNU_OVERFLOW_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_PRNU_OVERFLOW_MASK) >> PNU2D_PNU2DST_PRNU_OVERFLOW_SHIFT)
#define  PNU2D_PNU2DST_PRNU_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_PRNU_OVERFLOW_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_PRNU_OVERFLOW_SHIFT))

#define  PNU2D_PNU2DST_RESERVED3_MASK 0xf800
#define  PNU2D_PNU2DST_RESERVED3_SHIFT 11
#define  PNU2D_PNU2DST_RESERVED3_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_RESERVED3_MASK) >> PNU2D_PNU2DST_RESERVED3_SHIFT)
#define  PNU2D_PNU2DST_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_RESERVED3_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_RESERVED3_SHIFT))

#define  PNU2D_PNU2DST_DEBUG_STATE_MASK 0x700
#define  PNU2D_PNU2DST_DEBUG_STATE_SHIFT 8
#define  PNU2D_PNU2DST_DEBUG_STATE_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_DEBUG_STATE_MASK) >> PNU2D_PNU2DST_DEBUG_STATE_SHIFT)
#define  PNU2D_PNU2DST_DEBUG_STATE_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_DEBUG_STATE_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_DEBUG_STATE_SHIFT))

#define  PNU2D_PNU2DST_RESERVED4_MASK 0xfe
#define  PNU2D_PNU2DST_RESERVED4_SHIFT 1
#define  PNU2D_PNU2DST_RESERVED4_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_RESERVED4_MASK) >> PNU2D_PNU2DST_RESERVED4_SHIFT)
#define  PNU2D_PNU2DST_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_RESERVED4_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_RESERVED4_SHIFT))

#define  PNU2D_PNU2DST_STATUS_MASK 0x1
#define  PNU2D_PNU2DST_STATUS_SHIFT 0
#define  PNU2D_PNU2DST_STATUS_MASK_SHIFT(reg) (((reg) & PNU2D_PNU2DST_STATUS_MASK) >> PNU2D_PNU2DST_STATUS_SHIFT)
#define  PNU2D_PNU2DST_STATUS_REPLACE_VAL(reg,val) (((reg) & ~PNU2D_PNU2DST_STATUS_MASK) | (((uint32_t)val) << PNU2D_PNU2DST_STATUS_SHIFT))

#endif // PNU2D
