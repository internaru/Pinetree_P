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
 * \file UED_advanced_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _UED_ADVANCED_REGMASKS_H_
#define _UED_ADVANCED_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Universal Error Diffusion Halftone Registers (UED_advanced)
/** \brief Register descriptions for the Univeral Error Diffusion halfTone block (UED).*/
//
//====================================================================

//====================================================================
//Register: TDS LUT SRAM (Instance 1 of 6) (CLUT0)
/** \brief This LUT contains the scale values used for the tone-dependent threshold and bias (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_CLUT0_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_CLUT0_RESERVED1_SHIFT 12
#define  UED_ADVANCED_CLUT0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT0_RESERVED1_MASK) >> UED_ADVANCED_CLUT0_RESERVED1_SHIFT)
#define  UED_ADVANCED_CLUT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT0_RESERVED1_SHIFT))

#define  UED_ADVANCED_CLUT0_BIAS_MASK 0xc00
#define  UED_ADVANCED_CLUT0_BIAS_SHIFT 10
#define  UED_ADVANCED_CLUT0_BIAS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT0_BIAS_MASK) >> UED_ADVANCED_CLUT0_BIAS_SHIFT)
#define  UED_ADVANCED_CLUT0_BIAS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT0_BIAS_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT0_BIAS_SHIFT))

#define  UED_ADVANCED_CLUT0_THRESH_MASK 0x3ff
#define  UED_ADVANCED_CLUT0_THRESH_SHIFT 0
#define  UED_ADVANCED_CLUT0_THRESH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT0_THRESH_MASK) >> UED_ADVANCED_CLUT0_THRESH_SHIFT)
#define  UED_ADVANCED_CLUT0_THRESH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT0_THRESH_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT0_THRESH_SHIFT))

//====================================================================
//Register: TDS LUT SRAM (Instance 2 of 6) (CLUT1)
/** \brief This LUT contains the scale values used for the tone-dependent threshold and bias (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_CLUT1_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_CLUT1_RESERVED1_SHIFT 12
#define  UED_ADVANCED_CLUT1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT1_RESERVED1_MASK) >> UED_ADVANCED_CLUT1_RESERVED1_SHIFT)
#define  UED_ADVANCED_CLUT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT1_RESERVED1_SHIFT))

#define  UED_ADVANCED_CLUT1_BIAS_MASK 0xc00
#define  UED_ADVANCED_CLUT1_BIAS_SHIFT 10
#define  UED_ADVANCED_CLUT1_BIAS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT1_BIAS_MASK) >> UED_ADVANCED_CLUT1_BIAS_SHIFT)
#define  UED_ADVANCED_CLUT1_BIAS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT1_BIAS_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT1_BIAS_SHIFT))

#define  UED_ADVANCED_CLUT1_THRESH_MASK 0x3ff
#define  UED_ADVANCED_CLUT1_THRESH_SHIFT 0
#define  UED_ADVANCED_CLUT1_THRESH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT1_THRESH_MASK) >> UED_ADVANCED_CLUT1_THRESH_SHIFT)
#define  UED_ADVANCED_CLUT1_THRESH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT1_THRESH_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT1_THRESH_SHIFT))

//====================================================================
//Register: TDS LUT SRAM (Instance 3 of 6) (CLUT2)
/** \brief This LUT contains the scale values used for the tone-dependent threshold and bias (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_CLUT2_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_CLUT2_RESERVED1_SHIFT 12
#define  UED_ADVANCED_CLUT2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT2_RESERVED1_MASK) >> UED_ADVANCED_CLUT2_RESERVED1_SHIFT)
#define  UED_ADVANCED_CLUT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT2_RESERVED1_SHIFT))

#define  UED_ADVANCED_CLUT2_BIAS_MASK 0xc00
#define  UED_ADVANCED_CLUT2_BIAS_SHIFT 10
#define  UED_ADVANCED_CLUT2_BIAS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT2_BIAS_MASK) >> UED_ADVANCED_CLUT2_BIAS_SHIFT)
#define  UED_ADVANCED_CLUT2_BIAS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT2_BIAS_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT2_BIAS_SHIFT))

#define  UED_ADVANCED_CLUT2_THRESH_MASK 0x3ff
#define  UED_ADVANCED_CLUT2_THRESH_SHIFT 0
#define  UED_ADVANCED_CLUT2_THRESH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT2_THRESH_MASK) >> UED_ADVANCED_CLUT2_THRESH_SHIFT)
#define  UED_ADVANCED_CLUT2_THRESH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT2_THRESH_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT2_THRESH_SHIFT))

//====================================================================
//Register: TDS LUT SRAM (Instance 4 of 6) (CLUT3)
/** \brief This LUT contains the scale values used for the tone-dependent threshold and bias (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_CLUT3_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_CLUT3_RESERVED1_SHIFT 12
#define  UED_ADVANCED_CLUT3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT3_RESERVED1_MASK) >> UED_ADVANCED_CLUT3_RESERVED1_SHIFT)
#define  UED_ADVANCED_CLUT3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT3_RESERVED1_SHIFT))

#define  UED_ADVANCED_CLUT3_BIAS_MASK 0xc00
#define  UED_ADVANCED_CLUT3_BIAS_SHIFT 10
#define  UED_ADVANCED_CLUT3_BIAS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT3_BIAS_MASK) >> UED_ADVANCED_CLUT3_BIAS_SHIFT)
#define  UED_ADVANCED_CLUT3_BIAS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT3_BIAS_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT3_BIAS_SHIFT))

#define  UED_ADVANCED_CLUT3_THRESH_MASK 0x3ff
#define  UED_ADVANCED_CLUT3_THRESH_SHIFT 0
#define  UED_ADVANCED_CLUT3_THRESH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT3_THRESH_MASK) >> UED_ADVANCED_CLUT3_THRESH_SHIFT)
#define  UED_ADVANCED_CLUT3_THRESH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT3_THRESH_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT3_THRESH_SHIFT))

//====================================================================
//Register: TDS LUT SRAM (Instance 5 of 6) (CLUT4)
/** \brief This LUT contains the scale values used for the tone-dependent threshold and bias (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_CLUT4_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_CLUT4_RESERVED1_SHIFT 12
#define  UED_ADVANCED_CLUT4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT4_RESERVED1_MASK) >> UED_ADVANCED_CLUT4_RESERVED1_SHIFT)
#define  UED_ADVANCED_CLUT4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT4_RESERVED1_SHIFT))

#define  UED_ADVANCED_CLUT4_BIAS_MASK 0xc00
#define  UED_ADVANCED_CLUT4_BIAS_SHIFT 10
#define  UED_ADVANCED_CLUT4_BIAS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT4_BIAS_MASK) >> UED_ADVANCED_CLUT4_BIAS_SHIFT)
#define  UED_ADVANCED_CLUT4_BIAS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT4_BIAS_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT4_BIAS_SHIFT))

#define  UED_ADVANCED_CLUT4_THRESH_MASK 0x3ff
#define  UED_ADVANCED_CLUT4_THRESH_SHIFT 0
#define  UED_ADVANCED_CLUT4_THRESH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT4_THRESH_MASK) >> UED_ADVANCED_CLUT4_THRESH_SHIFT)
#define  UED_ADVANCED_CLUT4_THRESH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT4_THRESH_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT4_THRESH_SHIFT))

//====================================================================
//Register: TDS LUT SRAM (Instance 6 of 6) (CLUT5)
/** \brief This LUT contains the scale values used for the tone-dependent threshold and bias (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_CLUT5_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_CLUT5_RESERVED1_SHIFT 12
#define  UED_ADVANCED_CLUT5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT5_RESERVED1_MASK) >> UED_ADVANCED_CLUT5_RESERVED1_SHIFT)
#define  UED_ADVANCED_CLUT5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT5_RESERVED1_SHIFT))

#define  UED_ADVANCED_CLUT5_BIAS_MASK 0xc00
#define  UED_ADVANCED_CLUT5_BIAS_SHIFT 10
#define  UED_ADVANCED_CLUT5_BIAS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT5_BIAS_MASK) >> UED_ADVANCED_CLUT5_BIAS_SHIFT)
#define  UED_ADVANCED_CLUT5_BIAS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT5_BIAS_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT5_BIAS_SHIFT))

#define  UED_ADVANCED_CLUT5_THRESH_MASK 0x3ff
#define  UED_ADVANCED_CLUT5_THRESH_SHIFT 0
#define  UED_ADVANCED_CLUT5_THRESH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CLUT5_THRESH_MASK) >> UED_ADVANCED_CLUT5_THRESH_SHIFT)
#define  UED_ADVANCED_CLUT5_THRESH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CLUT5_THRESH_MASK) | (((uint32_t)val) << UED_ADVANCED_CLUT5_THRESH_SHIFT))

//====================================================================
//Register: TDW LUT SRAM (Instance 1 of 6) (WLUT0)
/** \brief This LUT contains the tone-dependent weights used in the error diffusion process. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUT0_W3_MASK 0xff000000
#define  UED_ADVANCED_WLUT0_W3_SHIFT 24
#define  UED_ADVANCED_WLUT0_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT0_W3_MASK) >> UED_ADVANCED_WLUT0_W3_SHIFT)
#define  UED_ADVANCED_WLUT0_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT0_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT0_W3_SHIFT))

#define  UED_ADVANCED_WLUT0_W2_MASK 0xff0000
#define  UED_ADVANCED_WLUT0_W2_SHIFT 16
#define  UED_ADVANCED_WLUT0_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT0_W2_MASK) >> UED_ADVANCED_WLUT0_W2_SHIFT)
#define  UED_ADVANCED_WLUT0_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT0_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT0_W2_SHIFT))

#define  UED_ADVANCED_WLUT0_W1_MASK 0xff00
#define  UED_ADVANCED_WLUT0_W1_SHIFT 8
#define  UED_ADVANCED_WLUT0_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT0_W1_MASK) >> UED_ADVANCED_WLUT0_W1_SHIFT)
#define  UED_ADVANCED_WLUT0_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT0_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT0_W1_SHIFT))

#define  UED_ADVANCED_WLUT0_W0_MASK 0xff
#define  UED_ADVANCED_WLUT0_W0_SHIFT 0
#define  UED_ADVANCED_WLUT0_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT0_W0_MASK) >> UED_ADVANCED_WLUT0_W0_SHIFT)
#define  UED_ADVANCED_WLUT0_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT0_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT0_W0_SHIFT))

//====================================================================
//Register: TDW LUT SRAM (Instance 2 of 6) (WLUT1)
/** \brief This LUT contains the tone-dependent weights used in the error diffusion process. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUT1_W3_MASK 0xff000000
#define  UED_ADVANCED_WLUT1_W3_SHIFT 24
#define  UED_ADVANCED_WLUT1_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT1_W3_MASK) >> UED_ADVANCED_WLUT1_W3_SHIFT)
#define  UED_ADVANCED_WLUT1_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT1_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT1_W3_SHIFT))

#define  UED_ADVANCED_WLUT1_W2_MASK 0xff0000
#define  UED_ADVANCED_WLUT1_W2_SHIFT 16
#define  UED_ADVANCED_WLUT1_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT1_W2_MASK) >> UED_ADVANCED_WLUT1_W2_SHIFT)
#define  UED_ADVANCED_WLUT1_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT1_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT1_W2_SHIFT))

#define  UED_ADVANCED_WLUT1_W1_MASK 0xff00
#define  UED_ADVANCED_WLUT1_W1_SHIFT 8
#define  UED_ADVANCED_WLUT1_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT1_W1_MASK) >> UED_ADVANCED_WLUT1_W1_SHIFT)
#define  UED_ADVANCED_WLUT1_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT1_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT1_W1_SHIFT))

#define  UED_ADVANCED_WLUT1_W0_MASK 0xff
#define  UED_ADVANCED_WLUT1_W0_SHIFT 0
#define  UED_ADVANCED_WLUT1_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT1_W0_MASK) >> UED_ADVANCED_WLUT1_W0_SHIFT)
#define  UED_ADVANCED_WLUT1_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT1_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT1_W0_SHIFT))

//====================================================================
//Register: TDW LUT SRAM (Instance 3 of 6) (WLUT2)
/** \brief This LUT contains the tone-dependent weights used in the error diffusion process. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUT2_W3_MASK 0xff000000
#define  UED_ADVANCED_WLUT2_W3_SHIFT 24
#define  UED_ADVANCED_WLUT2_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT2_W3_MASK) >> UED_ADVANCED_WLUT2_W3_SHIFT)
#define  UED_ADVANCED_WLUT2_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT2_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT2_W3_SHIFT))

#define  UED_ADVANCED_WLUT2_W2_MASK 0xff0000
#define  UED_ADVANCED_WLUT2_W2_SHIFT 16
#define  UED_ADVANCED_WLUT2_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT2_W2_MASK) >> UED_ADVANCED_WLUT2_W2_SHIFT)
#define  UED_ADVANCED_WLUT2_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT2_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT2_W2_SHIFT))

#define  UED_ADVANCED_WLUT2_W1_MASK 0xff00
#define  UED_ADVANCED_WLUT2_W1_SHIFT 8
#define  UED_ADVANCED_WLUT2_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT2_W1_MASK) >> UED_ADVANCED_WLUT2_W1_SHIFT)
#define  UED_ADVANCED_WLUT2_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT2_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT2_W1_SHIFT))

#define  UED_ADVANCED_WLUT2_W0_MASK 0xff
#define  UED_ADVANCED_WLUT2_W0_SHIFT 0
#define  UED_ADVANCED_WLUT2_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT2_W0_MASK) >> UED_ADVANCED_WLUT2_W0_SHIFT)
#define  UED_ADVANCED_WLUT2_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT2_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT2_W0_SHIFT))

//====================================================================
//Register: TDW LUT SRAM (Instance 4 of 6) (WLUT3)
/** \brief This LUT contains the tone-dependent weights used in the error diffusion process. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUT3_W3_MASK 0xff000000
#define  UED_ADVANCED_WLUT3_W3_SHIFT 24
#define  UED_ADVANCED_WLUT3_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT3_W3_MASK) >> UED_ADVANCED_WLUT3_W3_SHIFT)
#define  UED_ADVANCED_WLUT3_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT3_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT3_W3_SHIFT))

#define  UED_ADVANCED_WLUT3_W2_MASK 0xff0000
#define  UED_ADVANCED_WLUT3_W2_SHIFT 16
#define  UED_ADVANCED_WLUT3_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT3_W2_MASK) >> UED_ADVANCED_WLUT3_W2_SHIFT)
#define  UED_ADVANCED_WLUT3_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT3_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT3_W2_SHIFT))

#define  UED_ADVANCED_WLUT3_W1_MASK 0xff00
#define  UED_ADVANCED_WLUT3_W1_SHIFT 8
#define  UED_ADVANCED_WLUT3_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT3_W1_MASK) >> UED_ADVANCED_WLUT3_W1_SHIFT)
#define  UED_ADVANCED_WLUT3_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT3_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT3_W1_SHIFT))

#define  UED_ADVANCED_WLUT3_W0_MASK 0xff
#define  UED_ADVANCED_WLUT3_W0_SHIFT 0
#define  UED_ADVANCED_WLUT3_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT3_W0_MASK) >> UED_ADVANCED_WLUT3_W0_SHIFT)
#define  UED_ADVANCED_WLUT3_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT3_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT3_W0_SHIFT))

//====================================================================
//Register: TDW LUT SRAM (Instance 5 of 6) (WLUT4)
/** \brief This LUT contains the tone-dependent weights used in the error diffusion process. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUT4_W3_MASK 0xff000000
#define  UED_ADVANCED_WLUT4_W3_SHIFT 24
#define  UED_ADVANCED_WLUT4_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT4_W3_MASK) >> UED_ADVANCED_WLUT4_W3_SHIFT)
#define  UED_ADVANCED_WLUT4_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT4_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT4_W3_SHIFT))

#define  UED_ADVANCED_WLUT4_W2_MASK 0xff0000
#define  UED_ADVANCED_WLUT4_W2_SHIFT 16
#define  UED_ADVANCED_WLUT4_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT4_W2_MASK) >> UED_ADVANCED_WLUT4_W2_SHIFT)
#define  UED_ADVANCED_WLUT4_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT4_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT4_W2_SHIFT))

#define  UED_ADVANCED_WLUT4_W1_MASK 0xff00
#define  UED_ADVANCED_WLUT4_W1_SHIFT 8
#define  UED_ADVANCED_WLUT4_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT4_W1_MASK) >> UED_ADVANCED_WLUT4_W1_SHIFT)
#define  UED_ADVANCED_WLUT4_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT4_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT4_W1_SHIFT))

#define  UED_ADVANCED_WLUT4_W0_MASK 0xff
#define  UED_ADVANCED_WLUT4_W0_SHIFT 0
#define  UED_ADVANCED_WLUT4_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT4_W0_MASK) >> UED_ADVANCED_WLUT4_W0_SHIFT)
#define  UED_ADVANCED_WLUT4_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT4_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT4_W0_SHIFT))

//====================================================================
//Register: TDW LUT SRAM (Instance 6 of 6) (WLUT5)
/** \brief This LUT contains the tone-dependent weights used in the error diffusion process. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUT5_W3_MASK 0xff000000
#define  UED_ADVANCED_WLUT5_W3_SHIFT 24
#define  UED_ADVANCED_WLUT5_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT5_W3_MASK) >> UED_ADVANCED_WLUT5_W3_SHIFT)
#define  UED_ADVANCED_WLUT5_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT5_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT5_W3_SHIFT))

#define  UED_ADVANCED_WLUT5_W2_MASK 0xff0000
#define  UED_ADVANCED_WLUT5_W2_SHIFT 16
#define  UED_ADVANCED_WLUT5_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT5_W2_MASK) >> UED_ADVANCED_WLUT5_W2_SHIFT)
#define  UED_ADVANCED_WLUT5_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT5_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT5_W2_SHIFT))

#define  UED_ADVANCED_WLUT5_W1_MASK 0xff00
#define  UED_ADVANCED_WLUT5_W1_SHIFT 8
#define  UED_ADVANCED_WLUT5_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT5_W1_MASK) >> UED_ADVANCED_WLUT5_W1_SHIFT)
#define  UED_ADVANCED_WLUT5_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT5_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT5_W1_SHIFT))

#define  UED_ADVANCED_WLUT5_W0_MASK 0xff
#define  UED_ADVANCED_WLUT5_W0_SHIFT 0
#define  UED_ADVANCED_WLUT5_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUT5_W0_MASK) >> UED_ADVANCED_WLUT5_W0_SHIFT)
#define  UED_ADVANCED_WLUT5_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUT5_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUT5_W0_SHIFT))

//====================================================================
//Register: Bias LUT Access Command (Instance 1 of 6) (BLAC0)
/** \brief This register provides control for the BLUT indirect access means. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAC0_RESERVED1_MASK 0xfffffffc
#define  UED_ADVANCED_BLAC0_RESERVED1_SHIFT 2
#define  UED_ADVANCED_BLAC0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC0_RESERVED1_MASK) >> UED_ADVANCED_BLAC0_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAC0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC0_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAC0_INCA_MASK 0x2
#define  UED_ADVANCED_BLAC0_INCA_SHIFT 1
#define  UED_ADVANCED_BLAC0_INCA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC0_INCA_MASK) >> UED_ADVANCED_BLAC0_INCA_SHIFT)
#define  UED_ADVANCED_BLAC0_INCA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC0_INCA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC0_INCA_SHIFT))

#define  UED_ADVANCED_BLAC0_MODE_MASK 0x1
#define  UED_ADVANCED_BLAC0_MODE_SHIFT 0
#define  UED_ADVANCED_BLAC0_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC0_MODE_MASK) >> UED_ADVANCED_BLAC0_MODE_SHIFT)
#define  UED_ADVANCED_BLAC0_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC0_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC0_MODE_SHIFT))

//====================================================================
//Register: Bias LUT Access Command (Instance 2 of 6) (BLAC1)
/** \brief This register provides control for the BLUT indirect access means. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAC1_RESERVED1_MASK 0xfffffffc
#define  UED_ADVANCED_BLAC1_RESERVED1_SHIFT 2
#define  UED_ADVANCED_BLAC1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC1_RESERVED1_MASK) >> UED_ADVANCED_BLAC1_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAC1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC1_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAC1_INCA_MASK 0x2
#define  UED_ADVANCED_BLAC1_INCA_SHIFT 1
#define  UED_ADVANCED_BLAC1_INCA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC1_INCA_MASK) >> UED_ADVANCED_BLAC1_INCA_SHIFT)
#define  UED_ADVANCED_BLAC1_INCA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC1_INCA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC1_INCA_SHIFT))

#define  UED_ADVANCED_BLAC1_MODE_MASK 0x1
#define  UED_ADVANCED_BLAC1_MODE_SHIFT 0
#define  UED_ADVANCED_BLAC1_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC1_MODE_MASK) >> UED_ADVANCED_BLAC1_MODE_SHIFT)
#define  UED_ADVANCED_BLAC1_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC1_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC1_MODE_SHIFT))

//====================================================================
//Register: Bias LUT Access Command (Instance 3 of 6) (BLAC2)
/** \brief This register provides control for the BLUT indirect access means. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAC2_RESERVED1_MASK 0xfffffffc
#define  UED_ADVANCED_BLAC2_RESERVED1_SHIFT 2
#define  UED_ADVANCED_BLAC2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC2_RESERVED1_MASK) >> UED_ADVANCED_BLAC2_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAC2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC2_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAC2_INCA_MASK 0x2
#define  UED_ADVANCED_BLAC2_INCA_SHIFT 1
#define  UED_ADVANCED_BLAC2_INCA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC2_INCA_MASK) >> UED_ADVANCED_BLAC2_INCA_SHIFT)
#define  UED_ADVANCED_BLAC2_INCA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC2_INCA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC2_INCA_SHIFT))

#define  UED_ADVANCED_BLAC2_MODE_MASK 0x1
#define  UED_ADVANCED_BLAC2_MODE_SHIFT 0
#define  UED_ADVANCED_BLAC2_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC2_MODE_MASK) >> UED_ADVANCED_BLAC2_MODE_SHIFT)
#define  UED_ADVANCED_BLAC2_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC2_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC2_MODE_SHIFT))

//====================================================================
//Register: Bias LUT Access Command (Instance 4 of 6) (BLAC3)
/** \brief This register provides control for the BLUT indirect access means. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAC3_RESERVED1_MASK 0xfffffffc
#define  UED_ADVANCED_BLAC3_RESERVED1_SHIFT 2
#define  UED_ADVANCED_BLAC3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC3_RESERVED1_MASK) >> UED_ADVANCED_BLAC3_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAC3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC3_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAC3_INCA_MASK 0x2
#define  UED_ADVANCED_BLAC3_INCA_SHIFT 1
#define  UED_ADVANCED_BLAC3_INCA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC3_INCA_MASK) >> UED_ADVANCED_BLAC3_INCA_SHIFT)
#define  UED_ADVANCED_BLAC3_INCA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC3_INCA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC3_INCA_SHIFT))

#define  UED_ADVANCED_BLAC3_MODE_MASK 0x1
#define  UED_ADVANCED_BLAC3_MODE_SHIFT 0
#define  UED_ADVANCED_BLAC3_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC3_MODE_MASK) >> UED_ADVANCED_BLAC3_MODE_SHIFT)
#define  UED_ADVANCED_BLAC3_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC3_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC3_MODE_SHIFT))

//====================================================================
//Register: Bias LUT Access Command (Instance 5 of 6) (BLAC4)
/** \brief This register provides control for the BLUT indirect access means. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAC4_RESERVED1_MASK 0xfffffffc
#define  UED_ADVANCED_BLAC4_RESERVED1_SHIFT 2
#define  UED_ADVANCED_BLAC4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC4_RESERVED1_MASK) >> UED_ADVANCED_BLAC4_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAC4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC4_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAC4_INCA_MASK 0x2
#define  UED_ADVANCED_BLAC4_INCA_SHIFT 1
#define  UED_ADVANCED_BLAC4_INCA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC4_INCA_MASK) >> UED_ADVANCED_BLAC4_INCA_SHIFT)
#define  UED_ADVANCED_BLAC4_INCA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC4_INCA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC4_INCA_SHIFT))

#define  UED_ADVANCED_BLAC4_MODE_MASK 0x1
#define  UED_ADVANCED_BLAC4_MODE_SHIFT 0
#define  UED_ADVANCED_BLAC4_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC4_MODE_MASK) >> UED_ADVANCED_BLAC4_MODE_SHIFT)
#define  UED_ADVANCED_BLAC4_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC4_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC4_MODE_SHIFT))

//====================================================================
//Register: Bias LUT Access Command (Instance 6 of 6) (BLAC5)
/** \brief This register provides control for the BLUT indirect access means. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAC5_RESERVED1_MASK 0xfffffffc
#define  UED_ADVANCED_BLAC5_RESERVED1_SHIFT 2
#define  UED_ADVANCED_BLAC5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC5_RESERVED1_MASK) >> UED_ADVANCED_BLAC5_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAC5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC5_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAC5_INCA_MASK 0x2
#define  UED_ADVANCED_BLAC5_INCA_SHIFT 1
#define  UED_ADVANCED_BLAC5_INCA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC5_INCA_MASK) >> UED_ADVANCED_BLAC5_INCA_SHIFT)
#define  UED_ADVANCED_BLAC5_INCA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC5_INCA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC5_INCA_SHIFT))

#define  UED_ADVANCED_BLAC5_MODE_MASK 0x1
#define  UED_ADVANCED_BLAC5_MODE_SHIFT 0
#define  UED_ADVANCED_BLAC5_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAC5_MODE_MASK) >> UED_ADVANCED_BLAC5_MODE_SHIFT)
#define  UED_ADVANCED_BLAC5_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAC5_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAC5_MODE_SHIFT))

//====================================================================
//Register: Bias LUT Access Address (Instance 1 of 6) (BLAA0)
/** \brief This register provides the address for the BLUT indirect access means. The address is incremented per the incaddr setting in register BLAC. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAA0_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BLAA0_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BLAA0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA0_RESERVED1_MASK) >> UED_ADVANCED_BLAA0_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAA0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA0_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAA0_ADDR_MASK 0xfff
#define  UED_ADVANCED_BLAA0_ADDR_SHIFT 0
#define  UED_ADVANCED_BLAA0_ADDR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA0_ADDR_MASK) >> UED_ADVANCED_BLAA0_ADDR_SHIFT)
#define  UED_ADVANCED_BLAA0_ADDR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA0_ADDR_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA0_ADDR_SHIFT))

//====================================================================
//Register: Bias LUT Access Address (Instance 2 of 6) (BLAA1)
/** \brief This register provides the address for the BLUT indirect access means. The address is incremented per the incaddr setting in register BLAC. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAA1_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BLAA1_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BLAA1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA1_RESERVED1_MASK) >> UED_ADVANCED_BLAA1_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAA1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA1_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAA1_ADDR_MASK 0xfff
#define  UED_ADVANCED_BLAA1_ADDR_SHIFT 0
#define  UED_ADVANCED_BLAA1_ADDR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA1_ADDR_MASK) >> UED_ADVANCED_BLAA1_ADDR_SHIFT)
#define  UED_ADVANCED_BLAA1_ADDR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA1_ADDR_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA1_ADDR_SHIFT))

//====================================================================
//Register: Bias LUT Access Address (Instance 3 of 6) (BLAA2)
/** \brief This register provides the address for the BLUT indirect access means. The address is incremented per the incaddr setting in register BLAC. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAA2_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BLAA2_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BLAA2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA2_RESERVED1_MASK) >> UED_ADVANCED_BLAA2_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAA2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA2_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAA2_ADDR_MASK 0xfff
#define  UED_ADVANCED_BLAA2_ADDR_SHIFT 0
#define  UED_ADVANCED_BLAA2_ADDR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA2_ADDR_MASK) >> UED_ADVANCED_BLAA2_ADDR_SHIFT)
#define  UED_ADVANCED_BLAA2_ADDR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA2_ADDR_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA2_ADDR_SHIFT))

//====================================================================
//Register: Bias LUT Access Address (Instance 4 of 6) (BLAA3)
/** \brief This register provides the address for the BLUT indirect access means. The address is incremented per the incaddr setting in register BLAC. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAA3_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BLAA3_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BLAA3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA3_RESERVED1_MASK) >> UED_ADVANCED_BLAA3_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAA3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA3_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAA3_ADDR_MASK 0xfff
#define  UED_ADVANCED_BLAA3_ADDR_SHIFT 0
#define  UED_ADVANCED_BLAA3_ADDR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA3_ADDR_MASK) >> UED_ADVANCED_BLAA3_ADDR_SHIFT)
#define  UED_ADVANCED_BLAA3_ADDR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA3_ADDR_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA3_ADDR_SHIFT))

//====================================================================
//Register: Bias LUT Access Address (Instance 5 of 6) (BLAA4)
/** \brief This register provides the address for the BLUT indirect access means. The address is incremented per the incaddr setting in register BLAC. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAA4_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BLAA4_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BLAA4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA4_RESERVED1_MASK) >> UED_ADVANCED_BLAA4_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAA4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA4_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAA4_ADDR_MASK 0xfff
#define  UED_ADVANCED_BLAA4_ADDR_SHIFT 0
#define  UED_ADVANCED_BLAA4_ADDR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA4_ADDR_MASK) >> UED_ADVANCED_BLAA4_ADDR_SHIFT)
#define  UED_ADVANCED_BLAA4_ADDR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA4_ADDR_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA4_ADDR_SHIFT))

//====================================================================
//Register: Bias LUT Access Address (Instance 6 of 6) (BLAA5)
/** \brief This register provides the address for the BLUT indirect access means. The address is incremented per the incaddr setting in register BLAC. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAA5_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BLAA5_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BLAA5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA5_RESERVED1_MASK) >> UED_ADVANCED_BLAA5_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAA5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA5_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAA5_ADDR_MASK 0xfff
#define  UED_ADVANCED_BLAA5_ADDR_SHIFT 0
#define  UED_ADVANCED_BLAA5_ADDR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAA5_ADDR_MASK) >> UED_ADVANCED_BLAA5_ADDR_SHIFT)
#define  UED_ADVANCED_BLAA5_ADDR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAA5_ADDR_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAA5_ADDR_SHIFT))

//====================================================================
//Register: Bias LUT Access Data (Instance 1 of 6) (BLAD0)
/** \brief This register provides access to the data for the indirect access means (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAD0_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_BLAD0_RESERVED1_SHIFT 10
#define  UED_ADVANCED_BLAD0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD0_RESERVED1_MASK) >> UED_ADVANCED_BLAD0_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAD0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD0_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAD0_DATA_MASK 0x3ff
#define  UED_ADVANCED_BLAD0_DATA_SHIFT 0
#define  UED_ADVANCED_BLAD0_DATA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD0_DATA_MASK) >> UED_ADVANCED_BLAD0_DATA_SHIFT)
#define  UED_ADVANCED_BLAD0_DATA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD0_DATA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD0_DATA_SHIFT))

//====================================================================
//Register: Bias LUT Access Data (Instance 2 of 6) (BLAD1)
/** \brief This register provides access to the data for the indirect access means (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAD1_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_BLAD1_RESERVED1_SHIFT 10
#define  UED_ADVANCED_BLAD1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD1_RESERVED1_MASK) >> UED_ADVANCED_BLAD1_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAD1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD1_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAD1_DATA_MASK 0x3ff
#define  UED_ADVANCED_BLAD1_DATA_SHIFT 0
#define  UED_ADVANCED_BLAD1_DATA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD1_DATA_MASK) >> UED_ADVANCED_BLAD1_DATA_SHIFT)
#define  UED_ADVANCED_BLAD1_DATA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD1_DATA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD1_DATA_SHIFT))

//====================================================================
//Register: Bias LUT Access Data (Instance 3 of 6) (BLAD2)
/** \brief This register provides access to the data for the indirect access means (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAD2_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_BLAD2_RESERVED1_SHIFT 10
#define  UED_ADVANCED_BLAD2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD2_RESERVED1_MASK) >> UED_ADVANCED_BLAD2_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAD2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD2_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAD2_DATA_MASK 0x3ff
#define  UED_ADVANCED_BLAD2_DATA_SHIFT 0
#define  UED_ADVANCED_BLAD2_DATA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD2_DATA_MASK) >> UED_ADVANCED_BLAD2_DATA_SHIFT)
#define  UED_ADVANCED_BLAD2_DATA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD2_DATA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD2_DATA_SHIFT))

//====================================================================
//Register: Bias LUT Access Data (Instance 4 of 6) (BLAD3)
/** \brief This register provides access to the data for the indirect access means (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAD3_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_BLAD3_RESERVED1_SHIFT 10
#define  UED_ADVANCED_BLAD3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD3_RESERVED1_MASK) >> UED_ADVANCED_BLAD3_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAD3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD3_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAD3_DATA_MASK 0x3ff
#define  UED_ADVANCED_BLAD3_DATA_SHIFT 0
#define  UED_ADVANCED_BLAD3_DATA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD3_DATA_MASK) >> UED_ADVANCED_BLAD3_DATA_SHIFT)
#define  UED_ADVANCED_BLAD3_DATA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD3_DATA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD3_DATA_SHIFT))

//====================================================================
//Register: Bias LUT Access Data (Instance 5 of 6) (BLAD4)
/** \brief This register provides access to the data for the indirect access means (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAD4_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_BLAD4_RESERVED1_SHIFT 10
#define  UED_ADVANCED_BLAD4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD4_RESERVED1_MASK) >> UED_ADVANCED_BLAD4_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAD4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD4_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAD4_DATA_MASK 0x3ff
#define  UED_ADVANCED_BLAD4_DATA_SHIFT 0
#define  UED_ADVANCED_BLAD4_DATA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD4_DATA_MASK) >> UED_ADVANCED_BLAD4_DATA_SHIFT)
#define  UED_ADVANCED_BLAD4_DATA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD4_DATA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD4_DATA_SHIFT))

//====================================================================
//Register: Bias LUT Access Data (Instance 6 of 6) (BLAD5)
/** \brief This register provides access to the data for the indirect access means (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_BLAD5_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_BLAD5_RESERVED1_SHIFT 10
#define  UED_ADVANCED_BLAD5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD5_RESERVED1_MASK) >> UED_ADVANCED_BLAD5_RESERVED1_SHIFT)
#define  UED_ADVANCED_BLAD5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD5_RESERVED1_SHIFT))

#define  UED_ADVANCED_BLAD5_DATA_MASK 0x3ff
#define  UED_ADVANCED_BLAD5_DATA_SHIFT 0
#define  UED_ADVANCED_BLAD5_DATA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BLAD5_DATA_MASK) >> UED_ADVANCED_BLAD5_DATA_SHIFT)
#define  UED_ADVANCED_BLAD5_DATA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BLAD5_DATA_MASK) | (((uint32_t)val) << UED_ADVANCED_BLAD5_DATA_SHIFT))

//====================================================================
//Register: Configuration Register (CFGA)
/** \brief This register contains bits to control the global configuration*/
//====================================================================

#define  UED_ADVANCED_CFGA_CH_TDT_EN_MASK 0xfc000000
#define  UED_ADVANCED_CFGA_CH_TDT_EN_SHIFT 26
#define  UED_ADVANCED_CFGA_CH_TDT_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_CH_TDT_EN_MASK) >> UED_ADVANCED_CFGA_CH_TDT_EN_SHIFT)
#define  UED_ADVANCED_CFGA_CH_TDT_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_CH_TDT_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_CH_TDT_EN_SHIFT))

#define  UED_ADVANCED_CFGA_TH_PER_CH_MASK 0x2000000
#define  UED_ADVANCED_CFGA_TH_PER_CH_SHIFT 25
#define  UED_ADVANCED_CFGA_TH_PER_CH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_TH_PER_CH_MASK) >> UED_ADVANCED_CFGA_TH_PER_CH_SHIFT)
#define  UED_ADVANCED_CFGA_TH_PER_CH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_TH_PER_CH_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_TH_PER_CH_SHIFT))

#define  UED_ADVANCED_CFGA_CH_MIN_EN_MASK 0x1f80000
#define  UED_ADVANCED_CFGA_CH_MIN_EN_SHIFT 19
#define  UED_ADVANCED_CFGA_CH_MIN_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_CH_MIN_EN_MASK) >> UED_ADVANCED_CFGA_CH_MIN_EN_SHIFT)
#define  UED_ADVANCED_CFGA_CH_MIN_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_CH_MIN_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_CH_MIN_EN_SHIFT))

#define  UED_ADVANCED_CFGA_CH_MAX_EN_MASK 0x7e000
#define  UED_ADVANCED_CFGA_CH_MAX_EN_SHIFT 13
#define  UED_ADVANCED_CFGA_CH_MAX_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_CH_MAX_EN_MASK) >> UED_ADVANCED_CFGA_CH_MAX_EN_SHIFT)
#define  UED_ADVANCED_CFGA_CH_MAX_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_CH_MAX_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_CH_MAX_EN_SHIFT))

#define  UED_ADVANCED_CFGA_CH_EN_MASK 0x1f80
#define  UED_ADVANCED_CFGA_CH_EN_SHIFT 7
#define  UED_ADVANCED_CFGA_CH_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_CH_EN_MASK) >> UED_ADVANCED_CFGA_CH_EN_SHIFT)
#define  UED_ADVANCED_CFGA_CH_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_CH_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_CH_EN_SHIFT))

#define  UED_ADVANCED_CFGA_BLINE_MASK 0x40
#define  UED_ADVANCED_CFGA_BLINE_SHIFT 6
#define  UED_ADVANCED_CFGA_BLINE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_BLINE_MASK) >> UED_ADVANCED_CFGA_BLINE_SHIFT)
#define  UED_ADVANCED_CFGA_BLINE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_BLINE_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_BLINE_SHIFT))

#define  UED_ADVANCED_CFGA_S1_MASK 0x20
#define  UED_ADVANCED_CFGA_S1_SHIFT 5
#define  UED_ADVANCED_CFGA_S1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_S1_MASK) >> UED_ADVANCED_CFGA_S1_SHIFT)
#define  UED_ADVANCED_CFGA_S1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_S1_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_S1_SHIFT))

#define  UED_ADVANCED_CFGA_SERP_WRAP_MASK 0x10
#define  UED_ADVANCED_CFGA_SERP_WRAP_SHIFT 4
#define  UED_ADVANCED_CFGA_SERP_WRAP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_SERP_WRAP_MASK) >> UED_ADVANCED_CFGA_SERP_WRAP_SHIFT)
#define  UED_ADVANCED_CFGA_SERP_WRAP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_SERP_WRAP_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_SERP_WRAP_SHIFT))

#define  UED_ADVANCED_CFGA_BYPASS_MASK 0x8
#define  UED_ADVANCED_CFGA_BYPASS_SHIFT 3
#define  UED_ADVANCED_CFGA_BYPASS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_BYPASS_MASK) >> UED_ADVANCED_CFGA_BYPASS_SHIFT)
#define  UED_ADVANCED_CFGA_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_BYPASS_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_BYPASS_SHIFT))

#define  UED_ADVANCED_CFGA_CPU_STATE_ACCESS_MASK 0x4
#define  UED_ADVANCED_CFGA_CPU_STATE_ACCESS_SHIFT 2
#define  UED_ADVANCED_CFGA_CPU_STATE_ACCESS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_CPU_STATE_ACCESS_MASK) >> UED_ADVANCED_CFGA_CPU_STATE_ACCESS_SHIFT)
#define  UED_ADVANCED_CFGA_CPU_STATE_ACCESS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_CPU_STATE_ACCESS_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_CPU_STATE_ACCESS_SHIFT))

#define  UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_MASK 0x2
#define  UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_SHIFT 1
#define  UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_MASK) >> UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_SHIFT)
#define  UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_CPU_SRAM_ACCESS_SHIFT))

#define  UED_ADVANCED_CFGA_MODE_MASK 0x1
#define  UED_ADVANCED_CFGA_MODE_SHIFT 0
#define  UED_ADVANCED_CFGA_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGA_MODE_MASK) >> UED_ADVANCED_CFGA_MODE_SHIFT)
#define  UED_ADVANCED_CFGA_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGA_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGA_MODE_SHIFT))

//====================================================================
//Register: Configuration Register for ED PAIR mode (CFGB)
/** \brief This register contains bits to control the global configuration*/
//====================================================================

#define  UED_ADVANCED_CFGB_RESERVED1_MASK 0xfffc0000
#define  UED_ADVANCED_CFGB_RESERVED1_SHIFT 18
#define  UED_ADVANCED_CFGB_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_RESERVED1_MASK) >> UED_ADVANCED_CFGB_RESERVED1_SHIFT)
#define  UED_ADVANCED_CFGB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_RESERVED1_SHIFT))

#define  UED_ADVANCED_CFGB_SV5_MASK 0x30000
#define  UED_ADVANCED_CFGB_SV5_SHIFT 16
#define  UED_ADVANCED_CFGB_SV5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_SV5_MASK) >> UED_ADVANCED_CFGB_SV5_SHIFT)
#define  UED_ADVANCED_CFGB_SV5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_SV5_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_SV5_SHIFT))

#define  UED_ADVANCED_CFGB_SV4_MASK 0xc000
#define  UED_ADVANCED_CFGB_SV4_SHIFT 14
#define  UED_ADVANCED_CFGB_SV4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_SV4_MASK) >> UED_ADVANCED_CFGB_SV4_SHIFT)
#define  UED_ADVANCED_CFGB_SV4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_SV4_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_SV4_SHIFT))

#define  UED_ADVANCED_CFGB_SV3_MASK 0x3000
#define  UED_ADVANCED_CFGB_SV3_SHIFT 12
#define  UED_ADVANCED_CFGB_SV3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_SV3_MASK) >> UED_ADVANCED_CFGB_SV3_SHIFT)
#define  UED_ADVANCED_CFGB_SV3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_SV3_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_SV3_SHIFT))

#define  UED_ADVANCED_CFGB_SV2_MASK 0xc00
#define  UED_ADVANCED_CFGB_SV2_SHIFT 10
#define  UED_ADVANCED_CFGB_SV2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_SV2_MASK) >> UED_ADVANCED_CFGB_SV2_SHIFT)
#define  UED_ADVANCED_CFGB_SV2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_SV2_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_SV2_SHIFT))

#define  UED_ADVANCED_CFGB_SV1_MASK 0x300
#define  UED_ADVANCED_CFGB_SV1_SHIFT 8
#define  UED_ADVANCED_CFGB_SV1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_SV1_MASK) >> UED_ADVANCED_CFGB_SV1_SHIFT)
#define  UED_ADVANCED_CFGB_SV1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_SV1_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_SV1_SHIFT))

#define  UED_ADVANCED_CFGB_SV0_MASK 0xc0
#define  UED_ADVANCED_CFGB_SV0_SHIFT 6
#define  UED_ADVANCED_CFGB_SV0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_SV0_MASK) >> UED_ADVANCED_CFGB_SV0_SHIFT)
#define  UED_ADVANCED_CFGB_SV0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_SV0_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_SV0_SHIFT))

#define  UED_ADVANCED_CFGB_BD_MASK 0x30
#define  UED_ADVANCED_CFGB_BD_SHIFT 4
#define  UED_ADVANCED_CFGB_BD_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_BD_MASK) >> UED_ADVANCED_CFGB_BD_SHIFT)
#define  UED_ADVANCED_CFGB_BD_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_BD_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_BD_SHIFT))

#define  UED_ADVANCED_CFGB_H2X_MASK 0x8
#define  UED_ADVANCED_CFGB_H2X_SHIFT 3
#define  UED_ADVANCED_CFGB_H2X_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_H2X_MASK) >> UED_ADVANCED_CFGB_H2X_SHIFT)
#define  UED_ADVANCED_CFGB_H2X_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_H2X_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_H2X_SHIFT))

#define  UED_ADVANCED_CFGB_S4_MASK 0x4
#define  UED_ADVANCED_CFGB_S4_SHIFT 2
#define  UED_ADVANCED_CFGB_S4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_S4_MASK) >> UED_ADVANCED_CFGB_S4_SHIFT)
#define  UED_ADVANCED_CFGB_S4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_S4_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_S4_SHIFT))

#define  UED_ADVANCED_CFGB_S3_MASK 0x2
#define  UED_ADVANCED_CFGB_S3_SHIFT 1
#define  UED_ADVANCED_CFGB_S3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_S3_MASK) >> UED_ADVANCED_CFGB_S3_SHIFT)
#define  UED_ADVANCED_CFGB_S3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_S3_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_S3_SHIFT))

#define  UED_ADVANCED_CFGB_S0_MASK 0x1
#define  UED_ADVANCED_CFGB_S0_SHIFT 0
#define  UED_ADVANCED_CFGB_S0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_CFGB_S0_MASK) >> UED_ADVANCED_CFGB_S0_SHIFT)
#define  UED_ADVANCED_CFGB_S0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_CFGB_S0_MASK) | (((uint32_t)val) << UED_ADVANCED_CFGB_S0_SHIFT))

//====================================================================
//Register: Image Line Width (LW)
/** \brief This register must be set with the width (pixels) of the image to be halftoned. Must be set before halftoning commences.*/
//====================================================================

#define  UED_ADVANCED_LW_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LW_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LW_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LW_RESERVED1_MASK) >> UED_ADVANCED_LW_RESERVED1_SHIFT)
#define  UED_ADVANCED_LW_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LW_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LW_RESERVED1_SHIFT))

#define  UED_ADVANCED_LW_WIDTH_MASK 0xffff
#define  UED_ADVANCED_LW_WIDTH_SHIFT 0
#define  UED_ADVANCED_LW_WIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LW_WIDTH_MASK) >> UED_ADVANCED_LW_WIDTH_SHIFT)
#define  UED_ADVANCED_LW_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LW_WIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_LW_WIDTH_SHIFT))

//====================================================================
//Register: First Line Error Control (L1)
/** \brief This register controls how error terms added into the Floyd-Steinberg algorithm are handled for the first line of the image.*/
//====================================================================

#define  UED_ADVANCED_L1_SCALE2_MASK 0xff000000
#define  UED_ADVANCED_L1_SCALE2_SHIFT 24
#define  UED_ADVANCED_L1_SCALE2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_L1_SCALE2_MASK) >> UED_ADVANCED_L1_SCALE2_SHIFT)
#define  UED_ADVANCED_L1_SCALE2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_L1_SCALE2_MASK) | (((uint32_t)val) << UED_ADVANCED_L1_SCALE2_SHIFT))

#define  UED_ADVANCED_L1_SCALE1_MASK 0xff0000
#define  UED_ADVANCED_L1_SCALE1_SHIFT 16
#define  UED_ADVANCED_L1_SCALE1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_L1_SCALE1_MASK) >> UED_ADVANCED_L1_SCALE1_SHIFT)
#define  UED_ADVANCED_L1_SCALE1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_L1_SCALE1_MASK) | (((uint32_t)val) << UED_ADVANCED_L1_SCALE1_SHIFT))

#define  UED_ADVANCED_L1_RESERVED1_MASK 0xf000
#define  UED_ADVANCED_L1_RESERVED1_SHIFT 12
#define  UED_ADVANCED_L1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_L1_RESERVED1_MASK) >> UED_ADVANCED_L1_RESERVED1_SHIFT)
#define  UED_ADVANCED_L1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_L1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_L1_RESERVED1_SHIFT))

#define  UED_ADVANCED_L1_OFFSET_MASK 0xffc
#define  UED_ADVANCED_L1_OFFSET_SHIFT 2
#define  UED_ADVANCED_L1_OFFSET_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_L1_OFFSET_MASK) >> UED_ADVANCED_L1_OFFSET_SHIFT)
#define  UED_ADVANCED_L1_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_L1_OFFSET_MASK) | (((uint32_t)val) << UED_ADVANCED_L1_OFFSET_SHIFT))

#define  UED_ADVANCED_L1_MODE_MASK 0x3
#define  UED_ADVANCED_L1_MODE_SHIFT 0
#define  UED_ADVANCED_L1_MODE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_L1_MODE_MASK) >> UED_ADVANCED_L1_MODE_SHIFT)
#define  UED_ADVANCED_L1_MODE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_L1_MODE_MASK) | (((uint32_t)val) << UED_ADVANCED_L1_MODE_SHIFT))

//====================================================================
//Register: Pixel Stacking Avoidance Control (PSA)
/** \brief This register controls how the pixel stacking avoidance procedure operates. Pixel avoidance attempts to adjust the location of the various channel output that make up a pixel to avoid being placed one on top of another. The 1 feature is implemented to correlate three planes. There are two independent units. The first set, called set0, correlates channels 0, 1, and (3 or 4). The second set, called set1, correlates channels 2, (3 or 4), and 5. One of the channel inputs to each 1 set includes a 2-1 mux to allow selection between two input channels (3 or 4). These are uniquely assigned, meaning there are only two possibilities for their assignment. The first is set0:ch3 set1:ch4, and the second is set0:ch4 set1:ch3. This selection is determined by a bit in this register. Each channel may be independently enabled. Each set may choose the bias input from one of its active, participating channels.*/
//====================================================================

#define  UED_ADVANCED_PSA_RESERVED1_MASK 0xfff00000
#define  UED_ADVANCED_PSA_RESERVED1_SHIFT 20
#define  UED_ADVANCED_PSA_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_RESERVED1_MASK) >> UED_ADVANCED_PSA_RESERVED1_SHIFT)
#define  UED_ADVANCED_PSA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_RESERVED1_SHIFT))

#define  UED_ADVANCED_PSA_BLK_PRIOR_MASK 0xf0000
#define  UED_ADVANCED_PSA_BLK_PRIOR_SHIFT 16
#define  UED_ADVANCED_PSA_BLK_PRIOR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_BLK_PRIOR_MASK) >> UED_ADVANCED_PSA_BLK_PRIOR_SHIFT)
#define  UED_ADVANCED_PSA_BLK_PRIOR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_BLK_PRIOR_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_BLK_PRIOR_SHIFT))

#define  UED_ADVANCED_PSA_FILTER_EN_MASK 0x8000
#define  UED_ADVANCED_PSA_FILTER_EN_SHIFT 15
#define  UED_ADVANCED_PSA_FILTER_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_FILTER_EN_MASK) >> UED_ADVANCED_PSA_FILTER_EN_SHIFT)
#define  UED_ADVANCED_PSA_FILTER_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_FILTER_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_FILTER_EN_SHIFT))

#define  UED_ADVANCED_PSA_MAX_LEVEL_MASK 0x6000
#define  UED_ADVANCED_PSA_MAX_LEVEL_SHIFT 13
#define  UED_ADVANCED_PSA_MAX_LEVEL_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_MAX_LEVEL_MASK) >> UED_ADVANCED_PSA_MAX_LEVEL_SHIFT)
#define  UED_ADVANCED_PSA_MAX_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_MAX_LEVEL_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_MAX_LEVEL_SHIFT))

#define  UED_ADVANCED_PSA_CH34_SEL_MASK 0x1000
#define  UED_ADVANCED_PSA_CH34_SEL_SHIFT 12
#define  UED_ADVANCED_PSA_CH34_SEL_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_CH34_SEL_MASK) >> UED_ADVANCED_PSA_CH34_SEL_SHIFT)
#define  UED_ADVANCED_PSA_CH34_SEL_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_CH34_SEL_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_CH34_SEL_SHIFT))

#define  UED_ADVANCED_PSA_BSEL1_MASK 0xe00
#define  UED_ADVANCED_PSA_BSEL1_SHIFT 9
#define  UED_ADVANCED_PSA_BSEL1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_BSEL1_MASK) >> UED_ADVANCED_PSA_BSEL1_SHIFT)
#define  UED_ADVANCED_PSA_BSEL1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_BSEL1_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_BSEL1_SHIFT))

#define  UED_ADVANCED_PSA_BSEL0_MASK 0x1c0
#define  UED_ADVANCED_PSA_BSEL0_SHIFT 6
#define  UED_ADVANCED_PSA_BSEL0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_BSEL0_MASK) >> UED_ADVANCED_PSA_BSEL0_SHIFT)
#define  UED_ADVANCED_PSA_BSEL0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_BSEL0_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_BSEL0_SHIFT))

#define  UED_ADVANCED_PSA_EN5_MASK 0x20
#define  UED_ADVANCED_PSA_EN5_SHIFT 5
#define  UED_ADVANCED_PSA_EN5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_EN5_MASK) >> UED_ADVANCED_PSA_EN5_SHIFT)
#define  UED_ADVANCED_PSA_EN5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_EN5_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_EN5_SHIFT))

#define  UED_ADVANCED_PSA_EN4_MASK 0x10
#define  UED_ADVANCED_PSA_EN4_SHIFT 4
#define  UED_ADVANCED_PSA_EN4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_EN4_MASK) >> UED_ADVANCED_PSA_EN4_SHIFT)
#define  UED_ADVANCED_PSA_EN4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_EN4_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_EN4_SHIFT))

#define  UED_ADVANCED_PSA_EN3_MASK 0x8
#define  UED_ADVANCED_PSA_EN3_SHIFT 3
#define  UED_ADVANCED_PSA_EN3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_EN3_MASK) >> UED_ADVANCED_PSA_EN3_SHIFT)
#define  UED_ADVANCED_PSA_EN3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_EN3_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_EN3_SHIFT))

#define  UED_ADVANCED_PSA_EN2_MASK 0x4
#define  UED_ADVANCED_PSA_EN2_SHIFT 2
#define  UED_ADVANCED_PSA_EN2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_EN2_MASK) >> UED_ADVANCED_PSA_EN2_SHIFT)
#define  UED_ADVANCED_PSA_EN2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_EN2_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_EN2_SHIFT))

#define  UED_ADVANCED_PSA_EN1_MASK 0x2
#define  UED_ADVANCED_PSA_EN1_SHIFT 1
#define  UED_ADVANCED_PSA_EN1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_EN1_MASK) >> UED_ADVANCED_PSA_EN1_SHIFT)
#define  UED_ADVANCED_PSA_EN1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_EN1_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_EN1_SHIFT))

#define  UED_ADVANCED_PSA_EN0_MASK 0x1
#define  UED_ADVANCED_PSA_EN0_SHIFT 0
#define  UED_ADVANCED_PSA_EN0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_PSA_EN0_MASK) >> UED_ADVANCED_PSA_EN0_SHIFT)
#define  UED_ADVANCED_PSA_EN0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_PSA_EN0_MASK) | (((uint32_t)val) << UED_ADVANCED_PSA_EN0_SHIFT))

//====================================================================
//Register: Tone Dependent Error Weight Control (EWC)
/** \brief This register provides control for the tone-dependent error weight feature.*/
//====================================================================

#define  UED_ADVANCED_EWC_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_EWC_RESERVED1_SHIFT 10
#define  UED_ADVANCED_EWC_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWC_RESERVED1_MASK) >> UED_ADVANCED_EWC_RESERVED1_SHIFT)
#define  UED_ADVANCED_EWC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWC_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_EWC_RESERVED1_SHIFT))

#define  UED_ADVANCED_EWC_ED_DISABLE_MASK 0x200
#define  UED_ADVANCED_EWC_ED_DISABLE_SHIFT 9
#define  UED_ADVANCED_EWC_ED_DISABLE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWC_ED_DISABLE_MASK) >> UED_ADVANCED_EWC_ED_DISABLE_SHIFT)
#define  UED_ADVANCED_EWC_ED_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWC_ED_DISABLE_MASK) | (((uint32_t)val) << UED_ADVANCED_EWC_ED_DISABLE_SHIFT))

#define  UED_ADVANCED_EWC_RAND_CRUMBS_MASK 0x100
#define  UED_ADVANCED_EWC_RAND_CRUMBS_SHIFT 8
#define  UED_ADVANCED_EWC_RAND_CRUMBS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWC_RAND_CRUMBS_MASK) >> UED_ADVANCED_EWC_RAND_CRUMBS_SHIFT)
#define  UED_ADVANCED_EWC_RAND_CRUMBS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWC_RAND_CRUMBS_MASK) | (((uint32_t)val) << UED_ADVANCED_EWC_RAND_CRUMBS_SHIFT))

#define  UED_ADVANCED_EWC_ERR_CRUMBS_MASK 0x80
#define  UED_ADVANCED_EWC_ERR_CRUMBS_SHIFT 7
#define  UED_ADVANCED_EWC_ERR_CRUMBS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWC_ERR_CRUMBS_MASK) >> UED_ADVANCED_EWC_ERR_CRUMBS_SHIFT)
#define  UED_ADVANCED_EWC_ERR_CRUMBS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWC_ERR_CRUMBS_MASK) | (((uint32_t)val) << UED_ADVANCED_EWC_ERR_CRUMBS_SHIFT))

#define  UED_ADVANCED_EWC_WEIGHT8_MASK 0x40
#define  UED_ADVANCED_EWC_WEIGHT8_SHIFT 6
#define  UED_ADVANCED_EWC_WEIGHT8_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWC_WEIGHT8_MASK) >> UED_ADVANCED_EWC_WEIGHT8_SHIFT)
#define  UED_ADVANCED_EWC_WEIGHT8_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWC_WEIGHT8_MASK) | (((uint32_t)val) << UED_ADVANCED_EWC_WEIGHT8_SHIFT))

#define  UED_ADVANCED_EWC_CH_TDW_EN_MASK 0x3f
#define  UED_ADVANCED_EWC_CH_TDW_EN_SHIFT 0
#define  UED_ADVANCED_EWC_CH_TDW_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWC_CH_TDW_EN_MASK) >> UED_ADVANCED_EWC_CH_TDW_EN_SHIFT)
#define  UED_ADVANCED_EWC_CH_TDW_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWC_CH_TDW_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_EWC_CH_TDW_EN_SHIFT))

//====================================================================
//Register: Error Decay Control (EDC)
/** \brief This register provides control for the error diffusion error decay feature.*/
//====================================================================

#define  UED_ADVANCED_EDC_RESERVED1_MASK 0xfffffffc
#define  UED_ADVANCED_EDC_RESERVED1_SHIFT 2
#define  UED_ADVANCED_EDC_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDC_RESERVED1_MASK) >> UED_ADVANCED_EDC_RESERVED1_SHIFT)
#define  UED_ADVANCED_EDC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDC_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_EDC_RESERVED1_SHIFT))

#define  UED_ADVANCED_EDC_BK_EN_MASK 0x2
#define  UED_ADVANCED_EDC_BK_EN_SHIFT 1
#define  UED_ADVANCED_EDC_BK_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDC_BK_EN_MASK) >> UED_ADVANCED_EDC_BK_EN_SHIFT)
#define  UED_ADVANCED_EDC_BK_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDC_BK_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_EDC_BK_EN_SHIFT))

#define  UED_ADVANCED_EDC_WH_EN_MASK 0x1
#define  UED_ADVANCED_EDC_WH_EN_SHIFT 0
#define  UED_ADVANCED_EDC_WH_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDC_WH_EN_MASK) >> UED_ADVANCED_EDC_WH_EN_SHIFT)
#define  UED_ADVANCED_EDC_WH_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDC_WH_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_EDC_WH_EN_SHIFT))

//====================================================================
//Register: Decay Error Weights (EDD1)
/** \brief When a white (0x000) or black (0x3ff) pixel is detected and the associated decay enable bit is set, these weights will be used in the error diffusion. This replaces both the default weights and the tone-dependent weights.*/
//====================================================================

#define  UED_ADVANCED_EDD1_W3_MASK 0xff000000
#define  UED_ADVANCED_EDD1_W3_SHIFT 24
#define  UED_ADVANCED_EDD1_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD1_W3_MASK) >> UED_ADVANCED_EDD1_W3_SHIFT)
#define  UED_ADVANCED_EDD1_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD1_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD1_W3_SHIFT))

#define  UED_ADVANCED_EDD1_W2_MASK 0xff0000
#define  UED_ADVANCED_EDD1_W2_SHIFT 16
#define  UED_ADVANCED_EDD1_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD1_W2_MASK) >> UED_ADVANCED_EDD1_W2_SHIFT)
#define  UED_ADVANCED_EDD1_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD1_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD1_W2_SHIFT))

#define  UED_ADVANCED_EDD1_W1_MASK 0xff00
#define  UED_ADVANCED_EDD1_W1_SHIFT 8
#define  UED_ADVANCED_EDD1_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD1_W1_MASK) >> UED_ADVANCED_EDD1_W1_SHIFT)
#define  UED_ADVANCED_EDD1_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD1_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD1_W1_SHIFT))

#define  UED_ADVANCED_EDD1_W0_MASK 0xff
#define  UED_ADVANCED_EDD1_W0_SHIFT 0
#define  UED_ADVANCED_EDD1_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD1_W0_MASK) >> UED_ADVANCED_EDD1_W0_SHIFT)
#define  UED_ADVANCED_EDD1_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD1_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD1_W0_SHIFT))

//====================================================================
//Register: Decay Error Weights (EDD2)
/** \brief When a white (0x000) or black (0x3ff) pixel is detected and the associated decay enable bit is set, these weights will be used in the error diffusion. This replaces both the default weights and the tone-dependent weights.*/
//====================================================================

#define  UED_ADVANCED_EDD2_W7_MASK 0xff000000
#define  UED_ADVANCED_EDD2_W7_SHIFT 24
#define  UED_ADVANCED_EDD2_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD2_W7_MASK) >> UED_ADVANCED_EDD2_W7_SHIFT)
#define  UED_ADVANCED_EDD2_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD2_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD2_W7_SHIFT))

#define  UED_ADVANCED_EDD2_W6_MASK 0xff0000
#define  UED_ADVANCED_EDD2_W6_SHIFT 16
#define  UED_ADVANCED_EDD2_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD2_W6_MASK) >> UED_ADVANCED_EDD2_W6_SHIFT)
#define  UED_ADVANCED_EDD2_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD2_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD2_W6_SHIFT))

#define  UED_ADVANCED_EDD2_W5_MASK 0xff00
#define  UED_ADVANCED_EDD2_W5_SHIFT 8
#define  UED_ADVANCED_EDD2_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD2_W5_MASK) >> UED_ADVANCED_EDD2_W5_SHIFT)
#define  UED_ADVANCED_EDD2_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD2_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD2_W5_SHIFT))

#define  UED_ADVANCED_EDD2_W4_MASK 0xff
#define  UED_ADVANCED_EDD2_W4_SHIFT 0
#define  UED_ADVANCED_EDD2_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EDD2_W4_MASK) >> UED_ADVANCED_EDD2_W4_SHIFT)
#define  UED_ADVANCED_EDD2_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EDD2_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_EDD2_W4_SHIFT))

//====================================================================
//Register: Default Error Weights (EWD1)
/** \brief The TDEW feature can be disabled on a per-channel basis in the previous register. For disabled channels, this register provides the default error weights. Since the TDEW values are based on 8-bit values, so must these. For example, instead of using the standard 1/16, 3/16, 5/16, 7/16 weights, these values would be used: 16/256, 48/256, 80/256, 112/256. Only the numerator values are stored in the register. The /256 is implied.*/
//====================================================================

#define  UED_ADVANCED_EWD1_W3_MASK 0xff000000
#define  UED_ADVANCED_EWD1_W3_SHIFT 24
#define  UED_ADVANCED_EWD1_W3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD1_W3_MASK) >> UED_ADVANCED_EWD1_W3_SHIFT)
#define  UED_ADVANCED_EWD1_W3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD1_W3_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD1_W3_SHIFT))

#define  UED_ADVANCED_EWD1_W2_MASK 0xff0000
#define  UED_ADVANCED_EWD1_W2_SHIFT 16
#define  UED_ADVANCED_EWD1_W2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD1_W2_MASK) >> UED_ADVANCED_EWD1_W2_SHIFT)
#define  UED_ADVANCED_EWD1_W2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD1_W2_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD1_W2_SHIFT))

#define  UED_ADVANCED_EWD1_W1_MASK 0xff00
#define  UED_ADVANCED_EWD1_W1_SHIFT 8
#define  UED_ADVANCED_EWD1_W1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD1_W1_MASK) >> UED_ADVANCED_EWD1_W1_SHIFT)
#define  UED_ADVANCED_EWD1_W1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD1_W1_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD1_W1_SHIFT))

#define  UED_ADVANCED_EWD1_W0_MASK 0xff
#define  UED_ADVANCED_EWD1_W0_SHIFT 0
#define  UED_ADVANCED_EWD1_W0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD1_W0_MASK) >> UED_ADVANCED_EWD1_W0_SHIFT)
#define  UED_ADVANCED_EWD1_W0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD1_W0_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD1_W0_SHIFT))

//====================================================================
//Register: Default Error Weights (EWD2)
/** \brief The TDEW feature can be disabled on a per-channel basis in the previous register. For disabled channels, this register provides additional default error weights.*/
//====================================================================

#define  UED_ADVANCED_EWD2_W7_MASK 0xff000000
#define  UED_ADVANCED_EWD2_W7_SHIFT 24
#define  UED_ADVANCED_EWD2_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD2_W7_MASK) >> UED_ADVANCED_EWD2_W7_SHIFT)
#define  UED_ADVANCED_EWD2_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD2_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD2_W7_SHIFT))

#define  UED_ADVANCED_EWD2_W6_MASK 0xff0000
#define  UED_ADVANCED_EWD2_W6_SHIFT 16
#define  UED_ADVANCED_EWD2_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD2_W6_MASK) >> UED_ADVANCED_EWD2_W6_SHIFT)
#define  UED_ADVANCED_EWD2_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD2_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD2_W6_SHIFT))

#define  UED_ADVANCED_EWD2_W5_MASK 0xff00
#define  UED_ADVANCED_EWD2_W5_SHIFT 8
#define  UED_ADVANCED_EWD2_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD2_W5_MASK) >> UED_ADVANCED_EWD2_W5_SHIFT)
#define  UED_ADVANCED_EWD2_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD2_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD2_W5_SHIFT))

#define  UED_ADVANCED_EWD2_W4_MASK 0xff
#define  UED_ADVANCED_EWD2_W4_SHIFT 0
#define  UED_ADVANCED_EWD2_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_EWD2_W4_MASK) >> UED_ADVANCED_EWD2_W4_SHIFT)
#define  UED_ADVANCED_EWD2_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_EWD2_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_EWD2_W4_SHIFT))

//====================================================================
//Register: Bias Control 1 (B1)
/** \brief This register contains control bits for the signed bias section of UED*/
//====================================================================

#define  UED_ADVANCED_B1_RESERVED1_MASK 0xffe00000
#define  UED_ADVANCED_B1_RESERVED1_SHIFT 21
#define  UED_ADVANCED_B1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B1_RESERVED1_MASK) >> UED_ADVANCED_B1_RESERVED1_SHIFT)
#define  UED_ADVANCED_B1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_B1_RESERVED1_SHIFT))

#define  UED_ADVANCED_B1_CH_TDS_EN_MASK 0x1f8000
#define  UED_ADVANCED_B1_CH_TDS_EN_SHIFT 15
#define  UED_ADVANCED_B1_CH_TDS_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B1_CH_TDS_EN_MASK) >> UED_ADVANCED_B1_CH_TDS_EN_SHIFT)
#define  UED_ADVANCED_B1_CH_TDS_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B1_CH_TDS_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_B1_CH_TDS_EN_SHIFT))

#define  UED_ADVANCED_B1_CH_TDB_EN_MASK 0x7e00
#define  UED_ADVANCED_B1_CH_TDB_EN_SHIFT 9
#define  UED_ADVANCED_B1_CH_TDB_EN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B1_CH_TDB_EN_MASK) >> UED_ADVANCED_B1_CH_TDB_EN_SHIFT)
#define  UED_ADVANCED_B1_CH_TDB_EN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B1_CH_TDB_EN_MASK) | (((uint32_t)val) << UED_ADVANCED_B1_CH_TDB_EN_SHIFT))

#define  UED_ADVANCED_B1_SBSELECT_MASK 0x1c0
#define  UED_ADVANCED_B1_SBSELECT_SHIFT 6
#define  UED_ADVANCED_B1_SBSELECT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B1_SBSELECT_MASK) >> UED_ADVANCED_B1_SBSELECT_SHIFT)
#define  UED_ADVANCED_B1_SBSELECT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B1_SBSELECT_MASK) | (((uint32_t)val) << UED_ADVANCED_B1_SBSELECT_SHIFT))

#define  UED_ADVANCED_B1_SBRGEN_MASK 0x3c
#define  UED_ADVANCED_B1_SBRGEN_SHIFT 2
#define  UED_ADVANCED_B1_SBRGEN_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B1_SBRGEN_MASK) >> UED_ADVANCED_B1_SBRGEN_SHIFT)
#define  UED_ADVANCED_B1_SBRGEN_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B1_SBRGEN_MASK) | (((uint32_t)val) << UED_ADVANCED_B1_SBRGEN_SHIFT))

#define  UED_ADVANCED_B1_SBMODIFY_MASK 0x3
#define  UED_ADVANCED_B1_SBMODIFY_SHIFT 0
#define  UED_ADVANCED_B1_SBMODIFY_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B1_SBMODIFY_MASK) >> UED_ADVANCED_B1_SBMODIFY_SHIFT)
#define  UED_ADVANCED_B1_SBMODIFY_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B1_SBMODIFY_MASK) | (((uint32_t)val) << UED_ADVANCED_B1_SBMODIFY_SHIFT))

//====================================================================
//Register: Bias Control 2 (B2)
/** \brief This register contains control bits for the signed bias section of UED*/
//====================================================================

#define  UED_ADVANCED_B2_RESERVED1_MASK 0xc0000000
#define  UED_ADVANCED_B2_RESERVED1_SHIFT 30
#define  UED_ADVANCED_B2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B2_RESERVED1_MASK) >> UED_ADVANCED_B2_RESERVED1_SHIFT)
#define  UED_ADVANCED_B2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_B2_RESERVED1_SHIFT))

#define  UED_ADVANCED_B2_CLIFF1_MASK 0x3ff00000
#define  UED_ADVANCED_B2_CLIFF1_SHIFT 20
#define  UED_ADVANCED_B2_CLIFF1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B2_CLIFF1_MASK) >> UED_ADVANCED_B2_CLIFF1_SHIFT)
#define  UED_ADVANCED_B2_CLIFF1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B2_CLIFF1_MASK) | (((uint32_t)val) << UED_ADVANCED_B2_CLIFF1_SHIFT))

#define  UED_ADVANCED_B2_RANGE_MASK 0xf0000
#define  UED_ADVANCED_B2_RANGE_SHIFT 16
#define  UED_ADVANCED_B2_RANGE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B2_RANGE_MASK) >> UED_ADVANCED_B2_RANGE_SHIFT)
#define  UED_ADVANCED_B2_RANGE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B2_RANGE_MASK) | (((uint32_t)val) << UED_ADVANCED_B2_RANGE_SHIFT))

#define  UED_ADVANCED_B2_RESERVED2_MASK 0xc000
#define  UED_ADVANCED_B2_RESERVED2_SHIFT 14
#define  UED_ADVANCED_B2_RESERVED2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B2_RESERVED2_MASK) >> UED_ADVANCED_B2_RESERVED2_SHIFT)
#define  UED_ADVANCED_B2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B2_RESERVED2_MASK) | (((uint32_t)val) << UED_ADVANCED_B2_RESERVED2_SHIFT))

#define  UED_ADVANCED_B2_LAST_LEVEL_MASK 0x3ff0
#define  UED_ADVANCED_B2_LAST_LEVEL_SHIFT 4
#define  UED_ADVANCED_B2_LAST_LEVEL_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B2_LAST_LEVEL_MASK) >> UED_ADVANCED_B2_LAST_LEVEL_SHIFT)
#define  UED_ADVANCED_B2_LAST_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B2_LAST_LEVEL_MASK) | (((uint32_t)val) << UED_ADVANCED_B2_LAST_LEVEL_SHIFT))

#define  UED_ADVANCED_B2_LEVELS_MASK 0xf
#define  UED_ADVANCED_B2_LEVELS_SHIFT 0
#define  UED_ADVANCED_B2_LEVELS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B2_LEVELS_MASK) >> UED_ADVANCED_B2_LEVELS_SHIFT)
#define  UED_ADVANCED_B2_LEVELS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B2_LEVELS_MASK) | (((uint32_t)val) << UED_ADVANCED_B2_LEVELS_SHIFT))

//====================================================================
//Register: Bias Control 3 (B3)
/** \brief This register contains constants used in the tone-dependent signed bias algorithm.*/
//====================================================================

#define  UED_ADVANCED_B3_RESERVED1_MASK 0xfc000000
#define  UED_ADVANCED_B3_RESERVED1_SHIFT 26
#define  UED_ADVANCED_B3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B3_RESERVED1_MASK) >> UED_ADVANCED_B3_RESERVED1_SHIFT)
#define  UED_ADVANCED_B3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_B3_RESERVED1_SHIFT))

#define  UED_ADVANCED_B3_SBIAS2_MASK 0x3ff0000
#define  UED_ADVANCED_B3_SBIAS2_SHIFT 16
#define  UED_ADVANCED_B3_SBIAS2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B3_SBIAS2_MASK) >> UED_ADVANCED_B3_SBIAS2_SHIFT)
#define  UED_ADVANCED_B3_SBIAS2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B3_SBIAS2_MASK) | (((uint32_t)val) << UED_ADVANCED_B3_SBIAS2_SHIFT))

#define  UED_ADVANCED_B3_RESERVED2_MASK 0xfc00
#define  UED_ADVANCED_B3_RESERVED2_SHIFT 10
#define  UED_ADVANCED_B3_RESERVED2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B3_RESERVED2_MASK) >> UED_ADVANCED_B3_RESERVED2_SHIFT)
#define  UED_ADVANCED_B3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B3_RESERVED2_MASK) | (((uint32_t)val) << UED_ADVANCED_B3_RESERVED2_SHIFT))

#define  UED_ADVANCED_B3_SBIAS1_MASK 0x3ff
#define  UED_ADVANCED_B3_SBIAS1_SHIFT 0
#define  UED_ADVANCED_B3_SBIAS1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_B3_SBIAS1_MASK) >> UED_ADVANCED_B3_SBIAS1_SHIFT)
#define  UED_ADVANCED_B3_SBIAS1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_B3_SBIAS1_MASK) | (((uint32_t)val) << UED_ADVANCED_B3_SBIAS1_SHIFT))

//====================================================================
//Register: UED Build Spec (UBS)
/** \brief This register provides information regarding which optional hardware features were included or how they were configured for the build.*/
//====================================================================

#define  UED_ADVANCED_UBS_RESERVED1_MASK 0xfff00000
#define  UED_ADVANCED_UBS_RESERVED1_SHIFT 20
#define  UED_ADVANCED_UBS_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_RESERVED1_MASK) >> UED_ADVANCED_UBS_RESERVED1_SHIFT)
#define  UED_ADVANCED_UBS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_RESERVED1_SHIFT))

#define  UED_ADVANCED_UBS_THPER_MASK 0x80000
#define  UED_ADVANCED_UBS_THPER_SHIFT 19
#define  UED_ADVANCED_UBS_THPER_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_THPER_MASK) >> UED_ADVANCED_UBS_THPER_SHIFT)
#define  UED_ADVANCED_UBS_THPER_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_THPER_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_THPER_SHIFT))

#define  UED_ADVANCED_UBS_PSA_MASK 0x40000
#define  UED_ADVANCED_UBS_PSA_SHIFT 18
#define  UED_ADVANCED_UBS_PSA_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_PSA_MASK) >> UED_ADVANCED_UBS_PSA_SHIFT)
#define  UED_ADVANCED_UBS_PSA_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_PSA_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_PSA_SHIFT))

#define  UED_ADVANCED_UBS_NUMCHANS_MASK 0x38000
#define  UED_ADVANCED_UBS_NUMCHANS_SHIFT 15
#define  UED_ADVANCED_UBS_NUMCHANS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_NUMCHANS_MASK) >> UED_ADVANCED_UBS_NUMCHANS_SHIFT)
#define  UED_ADVANCED_UBS_NUMCHANS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_NUMCHANS_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_NUMCHANS_SHIFT))

#define  UED_ADVANCED_UBS_ERRCOMP_MASK 0x4000
#define  UED_ADVANCED_UBS_ERRCOMP_SHIFT 14
#define  UED_ADVANCED_UBS_ERRCOMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_ERRCOMP_MASK) >> UED_ADVANCED_UBS_ERRCOMP_SHIFT)
#define  UED_ADVANCED_UBS_ERRCOMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_ERRCOMP_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_ERRCOMP_SHIFT))

#define  UED_ADVANCED_UBS_TDSCALE_MASK 0x2000
#define  UED_ADVANCED_UBS_TDSCALE_SHIFT 13
#define  UED_ADVANCED_UBS_TDSCALE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_TDSCALE_MASK) >> UED_ADVANCED_UBS_TDSCALE_SHIFT)
#define  UED_ADVANCED_UBS_TDSCALE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_TDSCALE_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_TDSCALE_SHIFT))

#define  UED_ADVANCED_UBS_WLUTS_MASK 0x1c00
#define  UED_ADVANCED_UBS_WLUTS_SHIFT 10
#define  UED_ADVANCED_UBS_WLUTS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_WLUTS_MASK) >> UED_ADVANCED_UBS_WLUTS_SHIFT)
#define  UED_ADVANCED_UBS_WLUTS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_WLUTS_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_WLUTS_SHIFT))

#define  UED_ADVANCED_UBS_PIXWIDTH_MASK 0x3c0
#define  UED_ADVANCED_UBS_PIXWIDTH_SHIFT 6
#define  UED_ADVANCED_UBS_PIXWIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_PIXWIDTH_MASK) >> UED_ADVANCED_UBS_PIXWIDTH_SHIFT)
#define  UED_ADVANCED_UBS_PIXWIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_PIXWIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_PIXWIDTH_SHIFT))

#define  UED_ADVANCED_UBS_LASER_MASK 0x3f
#define  UED_ADVANCED_UBS_LASER_SHIFT 0
#define  UED_ADVANCED_UBS_LASER_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_UBS_LASER_MASK) >> UED_ADVANCED_UBS_LASER_SHIFT)
#define  UED_ADVANCED_UBS_LASER_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_UBS_LASER_MASK) | (((uint32_t)val) << UED_ADVANCED_UBS_LASER_SHIFT))

//====================================================================
//Register: Bias Window Shape (Instance 1 of 6) (BWS0)
/** \brief This register defines the shape of the bias perturbation window per channel. It allows implementing different screen angles for each channel. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWS0_RESERVED1_MASK 0xfc000000
#define  UED_ADVANCED_BWS0_RESERVED1_SHIFT 26
#define  UED_ADVANCED_BWS0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS0_RESERVED1_MASK) >> UED_ADVANCED_BWS0_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWS0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS0_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWS0_HEIGHT_MASK 0x3ffe000
#define  UED_ADVANCED_BWS0_HEIGHT_SHIFT 13
#define  UED_ADVANCED_BWS0_HEIGHT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS0_HEIGHT_MASK) >> UED_ADVANCED_BWS0_HEIGHT_SHIFT)
#define  UED_ADVANCED_BWS0_HEIGHT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS0_HEIGHT_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS0_HEIGHT_SHIFT))

#define  UED_ADVANCED_BWS0_WIDTH_MASK 0x1fff
#define  UED_ADVANCED_BWS0_WIDTH_SHIFT 0
#define  UED_ADVANCED_BWS0_WIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS0_WIDTH_MASK) >> UED_ADVANCED_BWS0_WIDTH_SHIFT)
#define  UED_ADVANCED_BWS0_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS0_WIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS0_WIDTH_SHIFT))

//====================================================================
//Register: Bias Window Shape (Instance 2 of 6) (BWS1)
/** \brief This register defines the shape of the bias perturbation window per channel. It allows implementing different screen angles for each channel. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWS1_RESERVED1_MASK 0xfc000000
#define  UED_ADVANCED_BWS1_RESERVED1_SHIFT 26
#define  UED_ADVANCED_BWS1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS1_RESERVED1_MASK) >> UED_ADVANCED_BWS1_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWS1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS1_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWS1_HEIGHT_MASK 0x3ffe000
#define  UED_ADVANCED_BWS1_HEIGHT_SHIFT 13
#define  UED_ADVANCED_BWS1_HEIGHT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS1_HEIGHT_MASK) >> UED_ADVANCED_BWS1_HEIGHT_SHIFT)
#define  UED_ADVANCED_BWS1_HEIGHT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS1_HEIGHT_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS1_HEIGHT_SHIFT))

#define  UED_ADVANCED_BWS1_WIDTH_MASK 0x1fff
#define  UED_ADVANCED_BWS1_WIDTH_SHIFT 0
#define  UED_ADVANCED_BWS1_WIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS1_WIDTH_MASK) >> UED_ADVANCED_BWS1_WIDTH_SHIFT)
#define  UED_ADVANCED_BWS1_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS1_WIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS1_WIDTH_SHIFT))

//====================================================================
//Register: Bias Window Shape (Instance 3 of 6) (BWS2)
/** \brief This register defines the shape of the bias perturbation window per channel. It allows implementing different screen angles for each channel. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWS2_RESERVED1_MASK 0xfc000000
#define  UED_ADVANCED_BWS2_RESERVED1_SHIFT 26
#define  UED_ADVANCED_BWS2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS2_RESERVED1_MASK) >> UED_ADVANCED_BWS2_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWS2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS2_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWS2_HEIGHT_MASK 0x3ffe000
#define  UED_ADVANCED_BWS2_HEIGHT_SHIFT 13
#define  UED_ADVANCED_BWS2_HEIGHT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS2_HEIGHT_MASK) >> UED_ADVANCED_BWS2_HEIGHT_SHIFT)
#define  UED_ADVANCED_BWS2_HEIGHT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS2_HEIGHT_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS2_HEIGHT_SHIFT))

#define  UED_ADVANCED_BWS2_WIDTH_MASK 0x1fff
#define  UED_ADVANCED_BWS2_WIDTH_SHIFT 0
#define  UED_ADVANCED_BWS2_WIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS2_WIDTH_MASK) >> UED_ADVANCED_BWS2_WIDTH_SHIFT)
#define  UED_ADVANCED_BWS2_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS2_WIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS2_WIDTH_SHIFT))

//====================================================================
//Register: Bias Window Shape (Instance 4 of 6) (BWS3)
/** \brief This register defines the shape of the bias perturbation window per channel. It allows implementing different screen angles for each channel. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWS3_RESERVED1_MASK 0xfc000000
#define  UED_ADVANCED_BWS3_RESERVED1_SHIFT 26
#define  UED_ADVANCED_BWS3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS3_RESERVED1_MASK) >> UED_ADVANCED_BWS3_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWS3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS3_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWS3_HEIGHT_MASK 0x3ffe000
#define  UED_ADVANCED_BWS3_HEIGHT_SHIFT 13
#define  UED_ADVANCED_BWS3_HEIGHT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS3_HEIGHT_MASK) >> UED_ADVANCED_BWS3_HEIGHT_SHIFT)
#define  UED_ADVANCED_BWS3_HEIGHT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS3_HEIGHT_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS3_HEIGHT_SHIFT))

#define  UED_ADVANCED_BWS3_WIDTH_MASK 0x1fff
#define  UED_ADVANCED_BWS3_WIDTH_SHIFT 0
#define  UED_ADVANCED_BWS3_WIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS3_WIDTH_MASK) >> UED_ADVANCED_BWS3_WIDTH_SHIFT)
#define  UED_ADVANCED_BWS3_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS3_WIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS3_WIDTH_SHIFT))

//====================================================================
//Register: Bias Window Shape (Instance 5 of 6) (BWS4)
/** \brief This register defines the shape of the bias perturbation window per channel. It allows implementing different screen angles for each channel. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWS4_RESERVED1_MASK 0xfc000000
#define  UED_ADVANCED_BWS4_RESERVED1_SHIFT 26
#define  UED_ADVANCED_BWS4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS4_RESERVED1_MASK) >> UED_ADVANCED_BWS4_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWS4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS4_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWS4_HEIGHT_MASK 0x3ffe000
#define  UED_ADVANCED_BWS4_HEIGHT_SHIFT 13
#define  UED_ADVANCED_BWS4_HEIGHT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS4_HEIGHT_MASK) >> UED_ADVANCED_BWS4_HEIGHT_SHIFT)
#define  UED_ADVANCED_BWS4_HEIGHT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS4_HEIGHT_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS4_HEIGHT_SHIFT))

#define  UED_ADVANCED_BWS4_WIDTH_MASK 0x1fff
#define  UED_ADVANCED_BWS4_WIDTH_SHIFT 0
#define  UED_ADVANCED_BWS4_WIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS4_WIDTH_MASK) >> UED_ADVANCED_BWS4_WIDTH_SHIFT)
#define  UED_ADVANCED_BWS4_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS4_WIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS4_WIDTH_SHIFT))

//====================================================================
//Register: Bias Window Shape (Instance 6 of 6) (BWS5)
/** \brief This register defines the shape of the bias perturbation window per channel. It allows implementing different screen angles for each channel. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWS5_RESERVED1_MASK 0xfc000000
#define  UED_ADVANCED_BWS5_RESERVED1_SHIFT 26
#define  UED_ADVANCED_BWS5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS5_RESERVED1_MASK) >> UED_ADVANCED_BWS5_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWS5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS5_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWS5_HEIGHT_MASK 0x3ffe000
#define  UED_ADVANCED_BWS5_HEIGHT_SHIFT 13
#define  UED_ADVANCED_BWS5_HEIGHT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS5_HEIGHT_MASK) >> UED_ADVANCED_BWS5_HEIGHT_SHIFT)
#define  UED_ADVANCED_BWS5_HEIGHT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS5_HEIGHT_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS5_HEIGHT_SHIFT))

#define  UED_ADVANCED_BWS5_WIDTH_MASK 0x1fff
#define  UED_ADVANCED_BWS5_WIDTH_SHIFT 0
#define  UED_ADVANCED_BWS5_WIDTH_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWS5_WIDTH_MASK) >> UED_ADVANCED_BWS5_WIDTH_SHIFT)
#define  UED_ADVANCED_BWS5_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWS5_WIDTH_MASK) | (((uint32_t)val) << UED_ADVANCED_BWS5_WIDTH_SHIFT))

//====================================================================
//Register: Image Row Odd Even (IR)
/** \brief This register provides context switch support for the present row value for the image being processed. Only the LSB of the row value is needed to indicate whether it is an odd or even row. Firmware reads this value as part of saving context for the current image being processed, and writes the value for the image being restored.*/
//====================================================================

#define  UED_ADVANCED_IR_RESERVED1_MASK 0xfffffffe
#define  UED_ADVANCED_IR_RESERVED1_SHIFT 1
#define  UED_ADVANCED_IR_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_IR_RESERVED1_MASK) >> UED_ADVANCED_IR_RESERVED1_SHIFT)
#define  UED_ADVANCED_IR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_IR_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_IR_RESERVED1_SHIFT))

#define  UED_ADVANCED_IR_ODD_MASK 0x1
#define  UED_ADVANCED_IR_ODD_SHIFT 0
#define  UED_ADVANCED_IR_ODD_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_IR_ODD_MASK) >> UED_ADVANCED_IR_ODD_SHIFT)
#define  UED_ADVANCED_IR_ODD_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_IR_ODD_MASK) | (((uint32_t)val) << UED_ADVANCED_IR_ODD_SHIFT))

//====================================================================
//Register: Bias Window Row Access (Instance 1 of 6) (BWR0)
/** \brief This register provides context switch support for the bias window row value. The bias window row value is different than the row of the image being processed. It indicates the row within the bias window which can be of varying heights (see register BWS) and is on a per channel basis. Firmware reads this value as part of saving context for the current image being processed, and writes the value for the image being restored. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWR0_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BWR0_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BWR0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR0_RESERVED1_MASK) >> UED_ADVANCED_BWR0_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR0_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWR0_ROW_MASK 0xfff
#define  UED_ADVANCED_BWR0_ROW_SHIFT 0
#define  UED_ADVANCED_BWR0_ROW_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR0_ROW_MASK) >> UED_ADVANCED_BWR0_ROW_SHIFT)
#define  UED_ADVANCED_BWR0_ROW_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR0_ROW_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR0_ROW_SHIFT))

//====================================================================
//Register: Bias Window Row Access (Instance 2 of 6) (BWR1)
/** \brief This register provides context switch support for the bias window row value. The bias window row value is different than the row of the image being processed. It indicates the row within the bias window which can be of varying heights (see register BWS) and is on a per channel basis. Firmware reads this value as part of saving context for the current image being processed, and writes the value for the image being restored. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWR1_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BWR1_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BWR1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR1_RESERVED1_MASK) >> UED_ADVANCED_BWR1_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR1_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWR1_ROW_MASK 0xfff
#define  UED_ADVANCED_BWR1_ROW_SHIFT 0
#define  UED_ADVANCED_BWR1_ROW_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR1_ROW_MASK) >> UED_ADVANCED_BWR1_ROW_SHIFT)
#define  UED_ADVANCED_BWR1_ROW_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR1_ROW_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR1_ROW_SHIFT))

//====================================================================
//Register: Bias Window Row Access (Instance 3 of 6) (BWR2)
/** \brief This register provides context switch support for the bias window row value. The bias window row value is different than the row of the image being processed. It indicates the row within the bias window which can be of varying heights (see register BWS) and is on a per channel basis. Firmware reads this value as part of saving context for the current image being processed, and writes the value for the image being restored. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWR2_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BWR2_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BWR2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR2_RESERVED1_MASK) >> UED_ADVANCED_BWR2_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR2_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWR2_ROW_MASK 0xfff
#define  UED_ADVANCED_BWR2_ROW_SHIFT 0
#define  UED_ADVANCED_BWR2_ROW_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR2_ROW_MASK) >> UED_ADVANCED_BWR2_ROW_SHIFT)
#define  UED_ADVANCED_BWR2_ROW_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR2_ROW_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR2_ROW_SHIFT))

//====================================================================
//Register: Bias Window Row Access (Instance 4 of 6) (BWR3)
/** \brief This register provides context switch support for the bias window row value. The bias window row value is different than the row of the image being processed. It indicates the row within the bias window which can be of varying heights (see register BWS) and is on a per channel basis. Firmware reads this value as part of saving context for the current image being processed, and writes the value for the image being restored. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWR3_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BWR3_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BWR3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR3_RESERVED1_MASK) >> UED_ADVANCED_BWR3_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR3_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWR3_ROW_MASK 0xfff
#define  UED_ADVANCED_BWR3_ROW_SHIFT 0
#define  UED_ADVANCED_BWR3_ROW_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR3_ROW_MASK) >> UED_ADVANCED_BWR3_ROW_SHIFT)
#define  UED_ADVANCED_BWR3_ROW_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR3_ROW_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR3_ROW_SHIFT))

//====================================================================
//Register: Bias Window Row Access (Instance 5 of 6) (BWR4)
/** \brief This register provides context switch support for the bias window row value. The bias window row value is different than the row of the image being processed. It indicates the row within the bias window which can be of varying heights (see register BWS) and is on a per channel basis. Firmware reads this value as part of saving context for the current image being processed, and writes the value for the image being restored. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWR4_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BWR4_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BWR4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR4_RESERVED1_MASK) >> UED_ADVANCED_BWR4_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWR4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR4_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWR4_ROW_MASK 0xfff
#define  UED_ADVANCED_BWR4_ROW_SHIFT 0
#define  UED_ADVANCED_BWR4_ROW_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR4_ROW_MASK) >> UED_ADVANCED_BWR4_ROW_SHIFT)
#define  UED_ADVANCED_BWR4_ROW_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR4_ROW_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR4_ROW_SHIFT))

//====================================================================
//Register: Bias Window Row Access (Instance 6 of 6) (BWR5)
/** \brief This register provides context switch support for the bias window row value. The bias window row value is different than the row of the image being processed. It indicates the row within the bias window which can be of varying heights (see register BWS) and is on a per channel basis. Firmware reads this value as part of saving context for the current image being processed, and writes the value for the image being restored. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_BWR5_RESERVED1_MASK 0xfffff000
#define  UED_ADVANCED_BWR5_RESERVED1_SHIFT 12
#define  UED_ADVANCED_BWR5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR5_RESERVED1_MASK) >> UED_ADVANCED_BWR5_RESERVED1_SHIFT)
#define  UED_ADVANCED_BWR5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR5_RESERVED1_SHIFT))

#define  UED_ADVANCED_BWR5_ROW_MASK 0xfff
#define  UED_ADVANCED_BWR5_ROW_SHIFT 0
#define  UED_ADVANCED_BWR5_ROW_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_BWR5_ROW_MASK) >> UED_ADVANCED_BWR5_ROW_SHIFT)
#define  UED_ADVANCED_BWR5_ROW_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_BWR5_ROW_MASK) | (((uint32_t)val) << UED_ADVANCED_BWR5_ROW_SHIFT))

//====================================================================
//Register: LFSR Context Switch (Instance 1 of 6) (LFSRX0)
/** \brief This register accesses a 16-bit Galois LFSR (x^16 + x^10 + x^9 + x^6 + 1) random number generator for use for save and restore operation.
      The value from this LFSR is used to load the per-pixel LFSR at that start of each line.
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      For context switching, this register needs read (if L1 modes 2 or 3 are active), as part of the context switch algorithm (save), and written (restore). (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRX0_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRX0_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRX0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX0_RESERVED1_MASK) >> UED_ADVANCED_LFSRX0_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRX0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX0_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRX0_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRX0_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRX0_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX0_RNUM_MASK) >> UED_ADVANCED_LFSRX0_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRX0_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX0_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX0_RNUM_SHIFT))

//====================================================================
//Register: LFSR Context Switch (Instance 2 of 6) (LFSRX1)
/** \brief This register accesses a 16-bit Galois LFSR (x^16 + x^10 + x^9 + x^6 + 1) random number generator for use for save and restore operation.
      The value from this LFSR is used to load the per-pixel LFSR at that start of each line.
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      For context switching, this register needs read (if L1 modes 2 or 3 are active), as part of the context switch algorithm (save), and written (restore). (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRX1_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRX1_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRX1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX1_RESERVED1_MASK) >> UED_ADVANCED_LFSRX1_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRX1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX1_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRX1_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRX1_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRX1_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX1_RNUM_MASK) >> UED_ADVANCED_LFSRX1_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRX1_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX1_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX1_RNUM_SHIFT))

//====================================================================
//Register: LFSR Context Switch (Instance 3 of 6) (LFSRX2)
/** \brief This register accesses a 16-bit Galois LFSR (x^16 + x^10 + x^9 + x^6 + 1) random number generator for use for save and restore operation.
      The value from this LFSR is used to load the per-pixel LFSR at that start of each line.
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      For context switching, this register needs read (if L1 modes 2 or 3 are active), as part of the context switch algorithm (save), and written (restore). (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRX2_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRX2_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRX2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX2_RESERVED1_MASK) >> UED_ADVANCED_LFSRX2_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRX2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX2_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRX2_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRX2_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRX2_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX2_RNUM_MASK) >> UED_ADVANCED_LFSRX2_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRX2_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX2_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX2_RNUM_SHIFT))

//====================================================================
//Register: LFSR Context Switch (Instance 4 of 6) (LFSRX3)
/** \brief This register accesses a 16-bit Galois LFSR (x^16 + x^10 + x^9 + x^6 + 1) random number generator for use for save and restore operation.
      The value from this LFSR is used to load the per-pixel LFSR at that start of each line.
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      For context switching, this register needs read (if L1 modes 2 or 3 are active), as part of the context switch algorithm (save), and written (restore). (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRX3_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRX3_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRX3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX3_RESERVED1_MASK) >> UED_ADVANCED_LFSRX3_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRX3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX3_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRX3_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRX3_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRX3_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX3_RNUM_MASK) >> UED_ADVANCED_LFSRX3_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRX3_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX3_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX3_RNUM_SHIFT))

//====================================================================
//Register: LFSR Context Switch (Instance 5 of 6) (LFSRX4)
/** \brief This register accesses a 16-bit Galois LFSR (x^16 + x^10 + x^9 + x^6 + 1) random number generator for use for save and restore operation.
      The value from this LFSR is used to load the per-pixel LFSR at that start of each line.
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      For context switching, this register needs read (if L1 modes 2 or 3 are active), as part of the context switch algorithm (save), and written (restore). (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRX4_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRX4_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRX4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX4_RESERVED1_MASK) >> UED_ADVANCED_LFSRX4_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRX4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX4_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRX4_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRX4_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRX4_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX4_RNUM_MASK) >> UED_ADVANCED_LFSRX4_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRX4_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX4_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX4_RNUM_SHIFT))

//====================================================================
//Register: LFSR Context Switch (Instance 6 of 6) (LFSRX5)
/** \brief This register accesses a 16-bit Galois LFSR (x^16 + x^10 + x^9 + x^6 + 1) random number generator for use for save and restore operation.
      The value from this LFSR is used to load the per-pixel LFSR at that start of each line.
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      For context switching, this register needs read (if L1 modes 2 or 3 are active), as part of the context switch algorithm (save), and written (restore). (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRX5_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRX5_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRX5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX5_RESERVED1_MASK) >> UED_ADVANCED_LFSRX5_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRX5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX5_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRX5_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRX5_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRX5_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRX5_RNUM_MASK) >> UED_ADVANCED_LFSRX5_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRX5_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRX5_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRX5_RNUM_SHIFT))

//====================================================================
//Register: LFSR Start Value (Instance 1 of 6) (LFSRS0)
/** \brief This register provides a start value for two 16-bit Galois LFSR generators.
      The first (x^16 + x^14 + x^13 + x^11 + 1) is clocked and used on a per pixel basis.
      The second (x^16 + x^10 + x^9 + x^6 + 1) is clocked and used on a per line basis to load the first LFSR at the start of every line (except the start of image line).
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      The value from this register preloads them both at the start of image. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRS0_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRS0_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRS0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS0_RESERVED1_MASK) >> UED_ADVANCED_LFSRS0_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRS0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS0_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRS0_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRS0_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRS0_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS0_RNUM_MASK) >> UED_ADVANCED_LFSRS0_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRS0_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS0_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS0_RNUM_SHIFT))

//====================================================================
//Register: LFSR Start Value (Instance 2 of 6) (LFSRS1)
/** \brief This register provides a start value for two 16-bit Galois LFSR generators.
      The first (x^16 + x^14 + x^13 + x^11 + 1) is clocked and used on a per pixel basis.
      The second (x^16 + x^10 + x^9 + x^6 + 1) is clocked and used on a per line basis to load the first LFSR at the start of every line (except the start of image line).
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      The value from this register preloads them both at the start of image. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRS1_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRS1_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRS1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS1_RESERVED1_MASK) >> UED_ADVANCED_LFSRS1_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRS1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS1_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRS1_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRS1_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRS1_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS1_RNUM_MASK) >> UED_ADVANCED_LFSRS1_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRS1_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS1_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS1_RNUM_SHIFT))

//====================================================================
//Register: LFSR Start Value (Instance 3 of 6) (LFSRS2)
/** \brief This register provides a start value for two 16-bit Galois LFSR generators.
      The first (x^16 + x^14 + x^13 + x^11 + 1) is clocked and used on a per pixel basis.
      The second (x^16 + x^10 + x^9 + x^6 + 1) is clocked and used on a per line basis to load the first LFSR at the start of every line (except the start of image line).
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      The value from this register preloads them both at the start of image. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRS2_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRS2_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRS2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS2_RESERVED1_MASK) >> UED_ADVANCED_LFSRS2_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRS2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS2_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRS2_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRS2_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRS2_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS2_RNUM_MASK) >> UED_ADVANCED_LFSRS2_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRS2_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS2_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS2_RNUM_SHIFT))

//====================================================================
//Register: LFSR Start Value (Instance 4 of 6) (LFSRS3)
/** \brief This register provides a start value for two 16-bit Galois LFSR generators.
      The first (x^16 + x^14 + x^13 + x^11 + 1) is clocked and used on a per pixel basis.
      The second (x^16 + x^10 + x^9 + x^6 + 1) is clocked and used on a per line basis to load the first LFSR at the start of every line (except the start of image line).
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      The value from this register preloads them both at the start of image. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRS3_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRS3_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRS3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS3_RESERVED1_MASK) >> UED_ADVANCED_LFSRS3_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRS3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS3_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRS3_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRS3_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRS3_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS3_RNUM_MASK) >> UED_ADVANCED_LFSRS3_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRS3_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS3_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS3_RNUM_SHIFT))

//====================================================================
//Register: LFSR Start Value (Instance 5 of 6) (LFSRS4)
/** \brief This register provides a start value for two 16-bit Galois LFSR generators.
      The first (x^16 + x^14 + x^13 + x^11 + 1) is clocked and used on a per pixel basis.
      The second (x^16 + x^10 + x^9 + x^6 + 1) is clocked and used on a per line basis to load the first LFSR at the start of every line (except the start of image line).
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      The value from this register preloads them both at the start of image. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRS4_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRS4_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRS4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS4_RESERVED1_MASK) >> UED_ADVANCED_LFSRS4_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRS4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS4_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRS4_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRS4_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRS4_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS4_RNUM_MASK) >> UED_ADVANCED_LFSRS4_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRS4_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS4_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS4_RNUM_SHIFT))

//====================================================================
//Register: LFSR Start Value (Instance 6 of 6) (LFSRS5)
/** \brief This register provides a start value for two 16-bit Galois LFSR generators.
      The first (x^16 + x^14 + x^13 + x^11 + 1) is clocked and used on a per pixel basis.
      The second (x^16 + x^10 + x^9 + x^6 + 1) is clocked and used on a per line basis to load the first LFSR at the start of every line (except the start of image line).
      The random number is used in one of the First Line Error Term modes and a certain Bias mode.
      The value from this register preloads them both at the start of image. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_LFSRS5_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_LFSRS5_RESERVED1_SHIFT 16
#define  UED_ADVANCED_LFSRS5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS5_RESERVED1_MASK) >> UED_ADVANCED_LFSRS5_RESERVED1_SHIFT)
#define  UED_ADVANCED_LFSRS5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS5_RESERVED1_SHIFT))

#define  UED_ADVANCED_LFSRS5_RNUM_MASK 0xffff
#define  UED_ADVANCED_LFSRS5_RNUM_SHIFT 0
#define  UED_ADVANCED_LFSRS5_RNUM_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_LFSRS5_RNUM_MASK) >> UED_ADVANCED_LFSRS5_RNUM_SHIFT)
#define  UED_ADVANCED_LFSRS5_RNUM_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_LFSRS5_RNUM_MASK) | (((uint32_t)val) << UED_ADVANCED_LFSRS5_RNUM_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 1 of 30) (THA0)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 1 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA0_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA0_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA0_RESERVED1_MASK) >> UED_ADVANCED_THA0_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA0_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA0_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA0_DENS_SHIFT 15
#define  UED_ADVANCED_THA0_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA0_DENS_MASK) >> UED_ADVANCED_THA0_DENS_SHIFT)
#define  UED_ADVANCED_THA0_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA0_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA0_DENS_SHIFT))

#define  UED_ADVANCED_THA0_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA0_SIZE_SHIFT 10
#define  UED_ADVANCED_THA0_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA0_SIZE_MASK) >> UED_ADVANCED_THA0_SIZE_SHIFT)
#define  UED_ADVANCED_THA0_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA0_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA0_SIZE_SHIFT))

#define  UED_ADVANCED_THA0_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA0_COMP_SHIFT 0
#define  UED_ADVANCED_THA0_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA0_COMP_MASK) >> UED_ADVANCED_THA0_COMP_SHIFT)
#define  UED_ADVANCED_THA0_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA0_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA0_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 2 of 30) (THA1)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 2 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA1_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA1_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA1_RESERVED1_MASK) >> UED_ADVANCED_THA1_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA1_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA1_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA1_DENS_SHIFT 15
#define  UED_ADVANCED_THA1_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA1_DENS_MASK) >> UED_ADVANCED_THA1_DENS_SHIFT)
#define  UED_ADVANCED_THA1_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA1_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA1_DENS_SHIFT))

#define  UED_ADVANCED_THA1_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA1_SIZE_SHIFT 10
#define  UED_ADVANCED_THA1_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA1_SIZE_MASK) >> UED_ADVANCED_THA1_SIZE_SHIFT)
#define  UED_ADVANCED_THA1_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA1_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA1_SIZE_SHIFT))

#define  UED_ADVANCED_THA1_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA1_COMP_SHIFT 0
#define  UED_ADVANCED_THA1_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA1_COMP_MASK) >> UED_ADVANCED_THA1_COMP_SHIFT)
#define  UED_ADVANCED_THA1_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA1_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA1_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 3 of 30) (THA2)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 3 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA2_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA2_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA2_RESERVED1_MASK) >> UED_ADVANCED_THA2_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA2_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA2_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA2_DENS_SHIFT 15
#define  UED_ADVANCED_THA2_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA2_DENS_MASK) >> UED_ADVANCED_THA2_DENS_SHIFT)
#define  UED_ADVANCED_THA2_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA2_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA2_DENS_SHIFT))

#define  UED_ADVANCED_THA2_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA2_SIZE_SHIFT 10
#define  UED_ADVANCED_THA2_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA2_SIZE_MASK) >> UED_ADVANCED_THA2_SIZE_SHIFT)
#define  UED_ADVANCED_THA2_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA2_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA2_SIZE_SHIFT))

#define  UED_ADVANCED_THA2_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA2_COMP_SHIFT 0
#define  UED_ADVANCED_THA2_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA2_COMP_MASK) >> UED_ADVANCED_THA2_COMP_SHIFT)
#define  UED_ADVANCED_THA2_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA2_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA2_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 4 of 30) (THA3)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 4 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA3_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA3_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA3_RESERVED1_MASK) >> UED_ADVANCED_THA3_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA3_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA3_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA3_DENS_SHIFT 15
#define  UED_ADVANCED_THA3_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA3_DENS_MASK) >> UED_ADVANCED_THA3_DENS_SHIFT)
#define  UED_ADVANCED_THA3_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA3_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA3_DENS_SHIFT))

#define  UED_ADVANCED_THA3_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA3_SIZE_SHIFT 10
#define  UED_ADVANCED_THA3_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA3_SIZE_MASK) >> UED_ADVANCED_THA3_SIZE_SHIFT)
#define  UED_ADVANCED_THA3_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA3_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA3_SIZE_SHIFT))

#define  UED_ADVANCED_THA3_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA3_COMP_SHIFT 0
#define  UED_ADVANCED_THA3_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA3_COMP_MASK) >> UED_ADVANCED_THA3_COMP_SHIFT)
#define  UED_ADVANCED_THA3_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA3_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA3_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 5 of 30) (THA4)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 5 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA4_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA4_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA4_RESERVED1_MASK) >> UED_ADVANCED_THA4_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA4_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA4_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA4_DENS_SHIFT 15
#define  UED_ADVANCED_THA4_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA4_DENS_MASK) >> UED_ADVANCED_THA4_DENS_SHIFT)
#define  UED_ADVANCED_THA4_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA4_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA4_DENS_SHIFT))

#define  UED_ADVANCED_THA4_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA4_SIZE_SHIFT 10
#define  UED_ADVANCED_THA4_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA4_SIZE_MASK) >> UED_ADVANCED_THA4_SIZE_SHIFT)
#define  UED_ADVANCED_THA4_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA4_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA4_SIZE_SHIFT))

#define  UED_ADVANCED_THA4_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA4_COMP_SHIFT 0
#define  UED_ADVANCED_THA4_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA4_COMP_MASK) >> UED_ADVANCED_THA4_COMP_SHIFT)
#define  UED_ADVANCED_THA4_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA4_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA4_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 6 of 30) (THA5)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 6 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA5_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA5_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA5_RESERVED1_MASK) >> UED_ADVANCED_THA5_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA5_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA5_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA5_DENS_SHIFT 15
#define  UED_ADVANCED_THA5_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA5_DENS_MASK) >> UED_ADVANCED_THA5_DENS_SHIFT)
#define  UED_ADVANCED_THA5_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA5_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA5_DENS_SHIFT))

#define  UED_ADVANCED_THA5_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA5_SIZE_SHIFT 10
#define  UED_ADVANCED_THA5_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA5_SIZE_MASK) >> UED_ADVANCED_THA5_SIZE_SHIFT)
#define  UED_ADVANCED_THA5_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA5_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA5_SIZE_SHIFT))

#define  UED_ADVANCED_THA5_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA5_COMP_SHIFT 0
#define  UED_ADVANCED_THA5_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA5_COMP_MASK) >> UED_ADVANCED_THA5_COMP_SHIFT)
#define  UED_ADVANCED_THA5_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA5_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA5_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 7 of 30) (THA6)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 7 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA6_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA6_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA6_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA6_RESERVED1_MASK) >> UED_ADVANCED_THA6_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA6_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA6_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA6_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA6_DENS_SHIFT 15
#define  UED_ADVANCED_THA6_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA6_DENS_MASK) >> UED_ADVANCED_THA6_DENS_SHIFT)
#define  UED_ADVANCED_THA6_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA6_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA6_DENS_SHIFT))

#define  UED_ADVANCED_THA6_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA6_SIZE_SHIFT 10
#define  UED_ADVANCED_THA6_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA6_SIZE_MASK) >> UED_ADVANCED_THA6_SIZE_SHIFT)
#define  UED_ADVANCED_THA6_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA6_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA6_SIZE_SHIFT))

#define  UED_ADVANCED_THA6_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA6_COMP_SHIFT 0
#define  UED_ADVANCED_THA6_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA6_COMP_MASK) >> UED_ADVANCED_THA6_COMP_SHIFT)
#define  UED_ADVANCED_THA6_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA6_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA6_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 8 of 30) (THA7)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 8 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA7_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA7_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA7_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA7_RESERVED1_MASK) >> UED_ADVANCED_THA7_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA7_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA7_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA7_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA7_DENS_SHIFT 15
#define  UED_ADVANCED_THA7_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA7_DENS_MASK) >> UED_ADVANCED_THA7_DENS_SHIFT)
#define  UED_ADVANCED_THA7_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA7_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA7_DENS_SHIFT))

#define  UED_ADVANCED_THA7_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA7_SIZE_SHIFT 10
#define  UED_ADVANCED_THA7_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA7_SIZE_MASK) >> UED_ADVANCED_THA7_SIZE_SHIFT)
#define  UED_ADVANCED_THA7_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA7_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA7_SIZE_SHIFT))

#define  UED_ADVANCED_THA7_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA7_COMP_SHIFT 0
#define  UED_ADVANCED_THA7_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA7_COMP_MASK) >> UED_ADVANCED_THA7_COMP_SHIFT)
#define  UED_ADVANCED_THA7_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA7_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA7_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 9 of 30) (THA8)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 9 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA8_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA8_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA8_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA8_RESERVED1_MASK) >> UED_ADVANCED_THA8_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA8_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA8_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA8_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA8_DENS_SHIFT 15
#define  UED_ADVANCED_THA8_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA8_DENS_MASK) >> UED_ADVANCED_THA8_DENS_SHIFT)
#define  UED_ADVANCED_THA8_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA8_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA8_DENS_SHIFT))

#define  UED_ADVANCED_THA8_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA8_SIZE_SHIFT 10
#define  UED_ADVANCED_THA8_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA8_SIZE_MASK) >> UED_ADVANCED_THA8_SIZE_SHIFT)
#define  UED_ADVANCED_THA8_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA8_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA8_SIZE_SHIFT))

#define  UED_ADVANCED_THA8_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA8_COMP_SHIFT 0
#define  UED_ADVANCED_THA8_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA8_COMP_MASK) >> UED_ADVANCED_THA8_COMP_SHIFT)
#define  UED_ADVANCED_THA8_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA8_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA8_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 10 of 30) (THA9)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 10 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA9_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA9_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA9_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA9_RESERVED1_MASK) >> UED_ADVANCED_THA9_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA9_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA9_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA9_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA9_DENS_SHIFT 15
#define  UED_ADVANCED_THA9_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA9_DENS_MASK) >> UED_ADVANCED_THA9_DENS_SHIFT)
#define  UED_ADVANCED_THA9_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA9_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA9_DENS_SHIFT))

#define  UED_ADVANCED_THA9_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA9_SIZE_SHIFT 10
#define  UED_ADVANCED_THA9_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA9_SIZE_MASK) >> UED_ADVANCED_THA9_SIZE_SHIFT)
#define  UED_ADVANCED_THA9_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA9_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA9_SIZE_SHIFT))

#define  UED_ADVANCED_THA9_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA9_COMP_SHIFT 0
#define  UED_ADVANCED_THA9_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA9_COMP_MASK) >> UED_ADVANCED_THA9_COMP_SHIFT)
#define  UED_ADVANCED_THA9_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA9_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA9_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 11 of 30) (THA10)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 11 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA10_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA10_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA10_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA10_RESERVED1_MASK) >> UED_ADVANCED_THA10_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA10_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA10_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA10_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA10_DENS_SHIFT 15
#define  UED_ADVANCED_THA10_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA10_DENS_MASK) >> UED_ADVANCED_THA10_DENS_SHIFT)
#define  UED_ADVANCED_THA10_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA10_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA10_DENS_SHIFT))

#define  UED_ADVANCED_THA10_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA10_SIZE_SHIFT 10
#define  UED_ADVANCED_THA10_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA10_SIZE_MASK) >> UED_ADVANCED_THA10_SIZE_SHIFT)
#define  UED_ADVANCED_THA10_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA10_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA10_SIZE_SHIFT))

#define  UED_ADVANCED_THA10_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA10_COMP_SHIFT 0
#define  UED_ADVANCED_THA10_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA10_COMP_MASK) >> UED_ADVANCED_THA10_COMP_SHIFT)
#define  UED_ADVANCED_THA10_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA10_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA10_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 12 of 30) (THA11)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 12 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA11_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA11_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA11_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA11_RESERVED1_MASK) >> UED_ADVANCED_THA11_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA11_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA11_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA11_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA11_DENS_SHIFT 15
#define  UED_ADVANCED_THA11_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA11_DENS_MASK) >> UED_ADVANCED_THA11_DENS_SHIFT)
#define  UED_ADVANCED_THA11_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA11_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA11_DENS_SHIFT))

#define  UED_ADVANCED_THA11_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA11_SIZE_SHIFT 10
#define  UED_ADVANCED_THA11_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA11_SIZE_MASK) >> UED_ADVANCED_THA11_SIZE_SHIFT)
#define  UED_ADVANCED_THA11_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA11_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA11_SIZE_SHIFT))

#define  UED_ADVANCED_THA11_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA11_COMP_SHIFT 0
#define  UED_ADVANCED_THA11_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA11_COMP_MASK) >> UED_ADVANCED_THA11_COMP_SHIFT)
#define  UED_ADVANCED_THA11_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA11_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA11_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 13 of 30) (THA12)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 13 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA12_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA12_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA12_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA12_RESERVED1_MASK) >> UED_ADVANCED_THA12_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA12_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA12_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA12_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA12_DENS_SHIFT 15
#define  UED_ADVANCED_THA12_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA12_DENS_MASK) >> UED_ADVANCED_THA12_DENS_SHIFT)
#define  UED_ADVANCED_THA12_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA12_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA12_DENS_SHIFT))

#define  UED_ADVANCED_THA12_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA12_SIZE_SHIFT 10
#define  UED_ADVANCED_THA12_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA12_SIZE_MASK) >> UED_ADVANCED_THA12_SIZE_SHIFT)
#define  UED_ADVANCED_THA12_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA12_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA12_SIZE_SHIFT))

#define  UED_ADVANCED_THA12_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA12_COMP_SHIFT 0
#define  UED_ADVANCED_THA12_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA12_COMP_MASK) >> UED_ADVANCED_THA12_COMP_SHIFT)
#define  UED_ADVANCED_THA12_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA12_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA12_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 14 of 30) (THA13)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 14 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA13_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA13_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA13_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA13_RESERVED1_MASK) >> UED_ADVANCED_THA13_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA13_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA13_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA13_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA13_DENS_SHIFT 15
#define  UED_ADVANCED_THA13_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA13_DENS_MASK) >> UED_ADVANCED_THA13_DENS_SHIFT)
#define  UED_ADVANCED_THA13_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA13_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA13_DENS_SHIFT))

#define  UED_ADVANCED_THA13_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA13_SIZE_SHIFT 10
#define  UED_ADVANCED_THA13_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA13_SIZE_MASK) >> UED_ADVANCED_THA13_SIZE_SHIFT)
#define  UED_ADVANCED_THA13_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA13_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA13_SIZE_SHIFT))

#define  UED_ADVANCED_THA13_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA13_COMP_SHIFT 0
#define  UED_ADVANCED_THA13_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA13_COMP_MASK) >> UED_ADVANCED_THA13_COMP_SHIFT)
#define  UED_ADVANCED_THA13_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA13_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA13_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 15 of 30) (THA14)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 15 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA14_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA14_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA14_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA14_RESERVED1_MASK) >> UED_ADVANCED_THA14_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA14_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA14_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA14_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA14_DENS_SHIFT 15
#define  UED_ADVANCED_THA14_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA14_DENS_MASK) >> UED_ADVANCED_THA14_DENS_SHIFT)
#define  UED_ADVANCED_THA14_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA14_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA14_DENS_SHIFT))

#define  UED_ADVANCED_THA14_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA14_SIZE_SHIFT 10
#define  UED_ADVANCED_THA14_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA14_SIZE_MASK) >> UED_ADVANCED_THA14_SIZE_SHIFT)
#define  UED_ADVANCED_THA14_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA14_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA14_SIZE_SHIFT))

#define  UED_ADVANCED_THA14_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA14_COMP_SHIFT 0
#define  UED_ADVANCED_THA14_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA14_COMP_MASK) >> UED_ADVANCED_THA14_COMP_SHIFT)
#define  UED_ADVANCED_THA14_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA14_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA14_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 16 of 30) (THA15)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 16 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA15_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA15_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA15_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA15_RESERVED1_MASK) >> UED_ADVANCED_THA15_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA15_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA15_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA15_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA15_DENS_SHIFT 15
#define  UED_ADVANCED_THA15_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA15_DENS_MASK) >> UED_ADVANCED_THA15_DENS_SHIFT)
#define  UED_ADVANCED_THA15_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA15_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA15_DENS_SHIFT))

#define  UED_ADVANCED_THA15_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA15_SIZE_SHIFT 10
#define  UED_ADVANCED_THA15_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA15_SIZE_MASK) >> UED_ADVANCED_THA15_SIZE_SHIFT)
#define  UED_ADVANCED_THA15_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA15_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA15_SIZE_SHIFT))

#define  UED_ADVANCED_THA15_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA15_COMP_SHIFT 0
#define  UED_ADVANCED_THA15_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA15_COMP_MASK) >> UED_ADVANCED_THA15_COMP_SHIFT)
#define  UED_ADVANCED_THA15_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA15_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA15_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 17 of 30) (THA16)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 17 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA16_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA16_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA16_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA16_RESERVED1_MASK) >> UED_ADVANCED_THA16_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA16_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA16_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA16_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA16_DENS_SHIFT 15
#define  UED_ADVANCED_THA16_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA16_DENS_MASK) >> UED_ADVANCED_THA16_DENS_SHIFT)
#define  UED_ADVANCED_THA16_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA16_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA16_DENS_SHIFT))

#define  UED_ADVANCED_THA16_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA16_SIZE_SHIFT 10
#define  UED_ADVANCED_THA16_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA16_SIZE_MASK) >> UED_ADVANCED_THA16_SIZE_SHIFT)
#define  UED_ADVANCED_THA16_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA16_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA16_SIZE_SHIFT))

#define  UED_ADVANCED_THA16_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA16_COMP_SHIFT 0
#define  UED_ADVANCED_THA16_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA16_COMP_MASK) >> UED_ADVANCED_THA16_COMP_SHIFT)
#define  UED_ADVANCED_THA16_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA16_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA16_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 18 of 30) (THA17)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 18 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA17_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA17_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA17_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA17_RESERVED1_MASK) >> UED_ADVANCED_THA17_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA17_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA17_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA17_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA17_DENS_SHIFT 15
#define  UED_ADVANCED_THA17_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA17_DENS_MASK) >> UED_ADVANCED_THA17_DENS_SHIFT)
#define  UED_ADVANCED_THA17_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA17_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA17_DENS_SHIFT))

#define  UED_ADVANCED_THA17_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA17_SIZE_SHIFT 10
#define  UED_ADVANCED_THA17_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA17_SIZE_MASK) >> UED_ADVANCED_THA17_SIZE_SHIFT)
#define  UED_ADVANCED_THA17_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA17_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA17_SIZE_SHIFT))

#define  UED_ADVANCED_THA17_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA17_COMP_SHIFT 0
#define  UED_ADVANCED_THA17_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA17_COMP_MASK) >> UED_ADVANCED_THA17_COMP_SHIFT)
#define  UED_ADVANCED_THA17_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA17_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA17_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 19 of 30) (THA18)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 19 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA18_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA18_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA18_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA18_RESERVED1_MASK) >> UED_ADVANCED_THA18_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA18_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA18_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA18_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA18_DENS_SHIFT 15
#define  UED_ADVANCED_THA18_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA18_DENS_MASK) >> UED_ADVANCED_THA18_DENS_SHIFT)
#define  UED_ADVANCED_THA18_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA18_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA18_DENS_SHIFT))

#define  UED_ADVANCED_THA18_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA18_SIZE_SHIFT 10
#define  UED_ADVANCED_THA18_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA18_SIZE_MASK) >> UED_ADVANCED_THA18_SIZE_SHIFT)
#define  UED_ADVANCED_THA18_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA18_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA18_SIZE_SHIFT))

#define  UED_ADVANCED_THA18_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA18_COMP_SHIFT 0
#define  UED_ADVANCED_THA18_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA18_COMP_MASK) >> UED_ADVANCED_THA18_COMP_SHIFT)
#define  UED_ADVANCED_THA18_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA18_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA18_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 20 of 30) (THA19)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 20 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA19_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA19_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA19_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA19_RESERVED1_MASK) >> UED_ADVANCED_THA19_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA19_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA19_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA19_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA19_DENS_SHIFT 15
#define  UED_ADVANCED_THA19_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA19_DENS_MASK) >> UED_ADVANCED_THA19_DENS_SHIFT)
#define  UED_ADVANCED_THA19_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA19_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA19_DENS_SHIFT))

#define  UED_ADVANCED_THA19_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA19_SIZE_SHIFT 10
#define  UED_ADVANCED_THA19_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA19_SIZE_MASK) >> UED_ADVANCED_THA19_SIZE_SHIFT)
#define  UED_ADVANCED_THA19_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA19_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA19_SIZE_SHIFT))

#define  UED_ADVANCED_THA19_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA19_COMP_SHIFT 0
#define  UED_ADVANCED_THA19_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA19_COMP_MASK) >> UED_ADVANCED_THA19_COMP_SHIFT)
#define  UED_ADVANCED_THA19_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA19_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA19_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 21 of 30) (THA20)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 21 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA20_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA20_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA20_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA20_RESERVED1_MASK) >> UED_ADVANCED_THA20_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA20_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA20_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA20_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA20_DENS_SHIFT 15
#define  UED_ADVANCED_THA20_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA20_DENS_MASK) >> UED_ADVANCED_THA20_DENS_SHIFT)
#define  UED_ADVANCED_THA20_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA20_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA20_DENS_SHIFT))

#define  UED_ADVANCED_THA20_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA20_SIZE_SHIFT 10
#define  UED_ADVANCED_THA20_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA20_SIZE_MASK) >> UED_ADVANCED_THA20_SIZE_SHIFT)
#define  UED_ADVANCED_THA20_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA20_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA20_SIZE_SHIFT))

#define  UED_ADVANCED_THA20_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA20_COMP_SHIFT 0
#define  UED_ADVANCED_THA20_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA20_COMP_MASK) >> UED_ADVANCED_THA20_COMP_SHIFT)
#define  UED_ADVANCED_THA20_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA20_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA20_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 22 of 30) (THA21)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 22 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA21_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA21_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA21_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA21_RESERVED1_MASK) >> UED_ADVANCED_THA21_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA21_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA21_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA21_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA21_DENS_SHIFT 15
#define  UED_ADVANCED_THA21_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA21_DENS_MASK) >> UED_ADVANCED_THA21_DENS_SHIFT)
#define  UED_ADVANCED_THA21_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA21_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA21_DENS_SHIFT))

#define  UED_ADVANCED_THA21_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA21_SIZE_SHIFT 10
#define  UED_ADVANCED_THA21_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA21_SIZE_MASK) >> UED_ADVANCED_THA21_SIZE_SHIFT)
#define  UED_ADVANCED_THA21_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA21_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA21_SIZE_SHIFT))

#define  UED_ADVANCED_THA21_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA21_COMP_SHIFT 0
#define  UED_ADVANCED_THA21_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA21_COMP_MASK) >> UED_ADVANCED_THA21_COMP_SHIFT)
#define  UED_ADVANCED_THA21_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA21_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA21_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 23 of 30) (THA22)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 23 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA22_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA22_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA22_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA22_RESERVED1_MASK) >> UED_ADVANCED_THA22_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA22_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA22_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA22_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA22_DENS_SHIFT 15
#define  UED_ADVANCED_THA22_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA22_DENS_MASK) >> UED_ADVANCED_THA22_DENS_SHIFT)
#define  UED_ADVANCED_THA22_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA22_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA22_DENS_SHIFT))

#define  UED_ADVANCED_THA22_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA22_SIZE_SHIFT 10
#define  UED_ADVANCED_THA22_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA22_SIZE_MASK) >> UED_ADVANCED_THA22_SIZE_SHIFT)
#define  UED_ADVANCED_THA22_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA22_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA22_SIZE_SHIFT))

#define  UED_ADVANCED_THA22_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA22_COMP_SHIFT 0
#define  UED_ADVANCED_THA22_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA22_COMP_MASK) >> UED_ADVANCED_THA22_COMP_SHIFT)
#define  UED_ADVANCED_THA22_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA22_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA22_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 24 of 30) (THA23)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 24 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA23_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA23_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA23_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA23_RESERVED1_MASK) >> UED_ADVANCED_THA23_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA23_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA23_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA23_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA23_DENS_SHIFT 15
#define  UED_ADVANCED_THA23_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA23_DENS_MASK) >> UED_ADVANCED_THA23_DENS_SHIFT)
#define  UED_ADVANCED_THA23_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA23_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA23_DENS_SHIFT))

#define  UED_ADVANCED_THA23_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA23_SIZE_SHIFT 10
#define  UED_ADVANCED_THA23_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA23_SIZE_MASK) >> UED_ADVANCED_THA23_SIZE_SHIFT)
#define  UED_ADVANCED_THA23_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA23_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA23_SIZE_SHIFT))

#define  UED_ADVANCED_THA23_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA23_COMP_SHIFT 0
#define  UED_ADVANCED_THA23_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA23_COMP_MASK) >> UED_ADVANCED_THA23_COMP_SHIFT)
#define  UED_ADVANCED_THA23_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA23_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA23_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 25 of 30) (THA24)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 25 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA24_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA24_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA24_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA24_RESERVED1_MASK) >> UED_ADVANCED_THA24_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA24_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA24_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA24_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA24_DENS_SHIFT 15
#define  UED_ADVANCED_THA24_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA24_DENS_MASK) >> UED_ADVANCED_THA24_DENS_SHIFT)
#define  UED_ADVANCED_THA24_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA24_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA24_DENS_SHIFT))

#define  UED_ADVANCED_THA24_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA24_SIZE_SHIFT 10
#define  UED_ADVANCED_THA24_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA24_SIZE_MASK) >> UED_ADVANCED_THA24_SIZE_SHIFT)
#define  UED_ADVANCED_THA24_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA24_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA24_SIZE_SHIFT))

#define  UED_ADVANCED_THA24_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA24_COMP_SHIFT 0
#define  UED_ADVANCED_THA24_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA24_COMP_MASK) >> UED_ADVANCED_THA24_COMP_SHIFT)
#define  UED_ADVANCED_THA24_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA24_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA24_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 26 of 30) (THA25)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 26 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA25_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA25_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA25_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA25_RESERVED1_MASK) >> UED_ADVANCED_THA25_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA25_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA25_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA25_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA25_DENS_SHIFT 15
#define  UED_ADVANCED_THA25_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA25_DENS_MASK) >> UED_ADVANCED_THA25_DENS_SHIFT)
#define  UED_ADVANCED_THA25_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA25_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA25_DENS_SHIFT))

#define  UED_ADVANCED_THA25_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA25_SIZE_SHIFT 10
#define  UED_ADVANCED_THA25_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA25_SIZE_MASK) >> UED_ADVANCED_THA25_SIZE_SHIFT)
#define  UED_ADVANCED_THA25_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA25_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA25_SIZE_SHIFT))

#define  UED_ADVANCED_THA25_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA25_COMP_SHIFT 0
#define  UED_ADVANCED_THA25_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA25_COMP_MASK) >> UED_ADVANCED_THA25_COMP_SHIFT)
#define  UED_ADVANCED_THA25_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA25_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA25_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 27 of 30) (THA26)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 27 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA26_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA26_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA26_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA26_RESERVED1_MASK) >> UED_ADVANCED_THA26_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA26_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA26_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA26_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA26_DENS_SHIFT 15
#define  UED_ADVANCED_THA26_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA26_DENS_MASK) >> UED_ADVANCED_THA26_DENS_SHIFT)
#define  UED_ADVANCED_THA26_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA26_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA26_DENS_SHIFT))

#define  UED_ADVANCED_THA26_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA26_SIZE_SHIFT 10
#define  UED_ADVANCED_THA26_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA26_SIZE_MASK) >> UED_ADVANCED_THA26_SIZE_SHIFT)
#define  UED_ADVANCED_THA26_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA26_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA26_SIZE_SHIFT))

#define  UED_ADVANCED_THA26_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA26_COMP_SHIFT 0
#define  UED_ADVANCED_THA26_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA26_COMP_MASK) >> UED_ADVANCED_THA26_COMP_SHIFT)
#define  UED_ADVANCED_THA26_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA26_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA26_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 28 of 30) (THA27)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 28 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA27_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA27_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA27_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA27_RESERVED1_MASK) >> UED_ADVANCED_THA27_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA27_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA27_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA27_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA27_DENS_SHIFT 15
#define  UED_ADVANCED_THA27_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA27_DENS_MASK) >> UED_ADVANCED_THA27_DENS_SHIFT)
#define  UED_ADVANCED_THA27_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA27_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA27_DENS_SHIFT))

#define  UED_ADVANCED_THA27_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA27_SIZE_SHIFT 10
#define  UED_ADVANCED_THA27_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA27_SIZE_MASK) >> UED_ADVANCED_THA27_SIZE_SHIFT)
#define  UED_ADVANCED_THA27_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA27_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA27_SIZE_SHIFT))

#define  UED_ADVANCED_THA27_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA27_COMP_SHIFT 0
#define  UED_ADVANCED_THA27_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA27_COMP_MASK) >> UED_ADVANCED_THA27_COMP_SHIFT)
#define  UED_ADVANCED_THA27_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA27_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA27_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 29 of 30) (THA28)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 29 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA28_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA28_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA28_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA28_RESERVED1_MASK) >> UED_ADVANCED_THA28_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA28_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA28_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA28_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA28_DENS_SHIFT 15
#define  UED_ADVANCED_THA28_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA28_DENS_MASK) >> UED_ADVANCED_THA28_DENS_SHIFT)
#define  UED_ADVANCED_THA28_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA28_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA28_DENS_SHIFT))

#define  UED_ADVANCED_THA28_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA28_SIZE_SHIFT 10
#define  UED_ADVANCED_THA28_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA28_SIZE_MASK) >> UED_ADVANCED_THA28_SIZE_SHIFT)
#define  UED_ADVANCED_THA28_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA28_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA28_SIZE_SHIFT))

#define  UED_ADVANCED_THA28_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA28_COMP_SHIFT 0
#define  UED_ADVANCED_THA28_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA28_COMP_MASK) >> UED_ADVANCED_THA28_COMP_SHIFT)
#define  UED_ADVANCED_THA28_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA28_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA28_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers (Instance 30 of 30) (THA29)
/** \brief This set of registers mark the thresholding boundaries. This function has typically been done in past designs by the Size/Density LUT. But the 1 algorithm requires simultaneous access to multiple entry points of that LUT, so a set of registers is used instead. (Instance 30 of 30)*/
//====================================================================

#define  UED_ADVANCED_THA29_RESERVED1_MASK 0xfe000000
#define  UED_ADVANCED_THA29_RESERVED1_SHIFT 25
#define  UED_ADVANCED_THA29_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA29_RESERVED1_MASK) >> UED_ADVANCED_THA29_RESERVED1_SHIFT)
#define  UED_ADVANCED_THA29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA29_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THA29_RESERVED1_SHIFT))

#define  UED_ADVANCED_THA29_DENS_MASK 0x1ff8000
#define  UED_ADVANCED_THA29_DENS_SHIFT 15
#define  UED_ADVANCED_THA29_DENS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA29_DENS_MASK) >> UED_ADVANCED_THA29_DENS_SHIFT)
#define  UED_ADVANCED_THA29_DENS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA29_DENS_MASK) | (((uint32_t)val) << UED_ADVANCED_THA29_DENS_SHIFT))

#define  UED_ADVANCED_THA29_SIZE_MASK 0x7c00
#define  UED_ADVANCED_THA29_SIZE_SHIFT 10
#define  UED_ADVANCED_THA29_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA29_SIZE_MASK) >> UED_ADVANCED_THA29_SIZE_SHIFT)
#define  UED_ADVANCED_THA29_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA29_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THA29_SIZE_SHIFT))

#define  UED_ADVANCED_THA29_COMP_MASK 0x3ff
#define  UED_ADVANCED_THA29_COMP_SHIFT 0
#define  UED_ADVANCED_THA29_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THA29_COMP_MASK) >> UED_ADVANCED_THA29_COMP_SHIFT)
#define  UED_ADVANCED_THA29_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THA29_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THA29_COMP_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 1 of 30) (THB0)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 1 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB0_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB0_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB0_RESERVED1_MASK) >> UED_ADVANCED_THB0_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB0_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB0_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB0_DIFF_SHIFT 0
#define  UED_ADVANCED_THB0_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB0_DIFF_MASK) >> UED_ADVANCED_THB0_DIFF_SHIFT)
#define  UED_ADVANCED_THB0_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB0_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB0_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 2 of 30) (THB1)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 2 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB1_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB1_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB1_RESERVED1_MASK) >> UED_ADVANCED_THB1_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB1_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB1_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB1_DIFF_SHIFT 0
#define  UED_ADVANCED_THB1_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB1_DIFF_MASK) >> UED_ADVANCED_THB1_DIFF_SHIFT)
#define  UED_ADVANCED_THB1_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB1_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB1_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 3 of 30) (THB2)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 3 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB2_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB2_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB2_RESERVED1_MASK) >> UED_ADVANCED_THB2_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB2_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB2_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB2_DIFF_SHIFT 0
#define  UED_ADVANCED_THB2_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB2_DIFF_MASK) >> UED_ADVANCED_THB2_DIFF_SHIFT)
#define  UED_ADVANCED_THB2_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB2_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB2_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 4 of 30) (THB3)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 4 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB3_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB3_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB3_RESERVED1_MASK) >> UED_ADVANCED_THB3_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB3_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB3_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB3_DIFF_SHIFT 0
#define  UED_ADVANCED_THB3_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB3_DIFF_MASK) >> UED_ADVANCED_THB3_DIFF_SHIFT)
#define  UED_ADVANCED_THB3_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB3_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB3_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 5 of 30) (THB4)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 5 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB4_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB4_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB4_RESERVED1_MASK) >> UED_ADVANCED_THB4_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB4_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB4_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB4_DIFF_SHIFT 0
#define  UED_ADVANCED_THB4_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB4_DIFF_MASK) >> UED_ADVANCED_THB4_DIFF_SHIFT)
#define  UED_ADVANCED_THB4_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB4_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB4_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 6 of 30) (THB5)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 6 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB5_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB5_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB5_RESERVED1_MASK) >> UED_ADVANCED_THB5_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB5_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB5_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB5_DIFF_SHIFT 0
#define  UED_ADVANCED_THB5_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB5_DIFF_MASK) >> UED_ADVANCED_THB5_DIFF_SHIFT)
#define  UED_ADVANCED_THB5_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB5_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB5_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 7 of 30) (THB6)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 7 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB6_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB6_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB6_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB6_RESERVED1_MASK) >> UED_ADVANCED_THB6_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB6_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB6_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB6_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB6_DIFF_SHIFT 0
#define  UED_ADVANCED_THB6_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB6_DIFF_MASK) >> UED_ADVANCED_THB6_DIFF_SHIFT)
#define  UED_ADVANCED_THB6_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB6_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB6_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 8 of 30) (THB7)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 8 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB7_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB7_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB7_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB7_RESERVED1_MASK) >> UED_ADVANCED_THB7_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB7_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB7_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB7_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB7_DIFF_SHIFT 0
#define  UED_ADVANCED_THB7_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB7_DIFF_MASK) >> UED_ADVANCED_THB7_DIFF_SHIFT)
#define  UED_ADVANCED_THB7_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB7_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB7_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 9 of 30) (THB8)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 9 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB8_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB8_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB8_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB8_RESERVED1_MASK) >> UED_ADVANCED_THB8_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB8_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB8_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB8_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB8_DIFF_SHIFT 0
#define  UED_ADVANCED_THB8_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB8_DIFF_MASK) >> UED_ADVANCED_THB8_DIFF_SHIFT)
#define  UED_ADVANCED_THB8_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB8_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB8_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 10 of 30) (THB9)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 10 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB9_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB9_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB9_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB9_RESERVED1_MASK) >> UED_ADVANCED_THB9_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB9_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB9_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB9_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB9_DIFF_SHIFT 0
#define  UED_ADVANCED_THB9_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB9_DIFF_MASK) >> UED_ADVANCED_THB9_DIFF_SHIFT)
#define  UED_ADVANCED_THB9_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB9_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB9_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 11 of 30) (THB10)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 11 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB10_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB10_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB10_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB10_RESERVED1_MASK) >> UED_ADVANCED_THB10_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB10_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB10_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB10_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB10_DIFF_SHIFT 0
#define  UED_ADVANCED_THB10_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB10_DIFF_MASK) >> UED_ADVANCED_THB10_DIFF_SHIFT)
#define  UED_ADVANCED_THB10_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB10_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB10_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 12 of 30) (THB11)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 12 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB11_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB11_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB11_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB11_RESERVED1_MASK) >> UED_ADVANCED_THB11_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB11_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB11_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB11_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB11_DIFF_SHIFT 0
#define  UED_ADVANCED_THB11_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB11_DIFF_MASK) >> UED_ADVANCED_THB11_DIFF_SHIFT)
#define  UED_ADVANCED_THB11_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB11_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB11_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 13 of 30) (THB12)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 13 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB12_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB12_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB12_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB12_RESERVED1_MASK) >> UED_ADVANCED_THB12_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB12_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB12_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB12_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB12_DIFF_SHIFT 0
#define  UED_ADVANCED_THB12_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB12_DIFF_MASK) >> UED_ADVANCED_THB12_DIFF_SHIFT)
#define  UED_ADVANCED_THB12_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB12_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB12_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 14 of 30) (THB13)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 14 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB13_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB13_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB13_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB13_RESERVED1_MASK) >> UED_ADVANCED_THB13_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB13_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB13_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB13_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB13_DIFF_SHIFT 0
#define  UED_ADVANCED_THB13_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB13_DIFF_MASK) >> UED_ADVANCED_THB13_DIFF_SHIFT)
#define  UED_ADVANCED_THB13_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB13_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB13_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 15 of 30) (THB14)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 15 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB14_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB14_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB14_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB14_RESERVED1_MASK) >> UED_ADVANCED_THB14_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB14_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB14_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB14_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB14_DIFF_SHIFT 0
#define  UED_ADVANCED_THB14_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB14_DIFF_MASK) >> UED_ADVANCED_THB14_DIFF_SHIFT)
#define  UED_ADVANCED_THB14_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB14_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB14_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 16 of 30) (THB15)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 16 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB15_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB15_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB15_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB15_RESERVED1_MASK) >> UED_ADVANCED_THB15_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB15_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB15_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB15_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB15_DIFF_SHIFT 0
#define  UED_ADVANCED_THB15_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB15_DIFF_MASK) >> UED_ADVANCED_THB15_DIFF_SHIFT)
#define  UED_ADVANCED_THB15_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB15_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB15_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 17 of 30) (THB16)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 17 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB16_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB16_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB16_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB16_RESERVED1_MASK) >> UED_ADVANCED_THB16_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB16_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB16_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB16_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB16_DIFF_SHIFT 0
#define  UED_ADVANCED_THB16_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB16_DIFF_MASK) >> UED_ADVANCED_THB16_DIFF_SHIFT)
#define  UED_ADVANCED_THB16_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB16_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB16_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 18 of 30) (THB17)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 18 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB17_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB17_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB17_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB17_RESERVED1_MASK) >> UED_ADVANCED_THB17_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB17_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB17_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB17_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB17_DIFF_SHIFT 0
#define  UED_ADVANCED_THB17_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB17_DIFF_MASK) >> UED_ADVANCED_THB17_DIFF_SHIFT)
#define  UED_ADVANCED_THB17_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB17_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB17_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 19 of 30) (THB18)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 19 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB18_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB18_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB18_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB18_RESERVED1_MASK) >> UED_ADVANCED_THB18_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB18_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB18_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB18_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB18_DIFF_SHIFT 0
#define  UED_ADVANCED_THB18_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB18_DIFF_MASK) >> UED_ADVANCED_THB18_DIFF_SHIFT)
#define  UED_ADVANCED_THB18_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB18_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB18_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 20 of 30) (THB19)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 20 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB19_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB19_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB19_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB19_RESERVED1_MASK) >> UED_ADVANCED_THB19_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB19_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB19_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB19_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB19_DIFF_SHIFT 0
#define  UED_ADVANCED_THB19_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB19_DIFF_MASK) >> UED_ADVANCED_THB19_DIFF_SHIFT)
#define  UED_ADVANCED_THB19_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB19_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB19_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 21 of 30) (THB20)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 21 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB20_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB20_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB20_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB20_RESERVED1_MASK) >> UED_ADVANCED_THB20_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB20_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB20_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB20_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB20_DIFF_SHIFT 0
#define  UED_ADVANCED_THB20_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB20_DIFF_MASK) >> UED_ADVANCED_THB20_DIFF_SHIFT)
#define  UED_ADVANCED_THB20_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB20_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB20_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 22 of 30) (THB21)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 22 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB21_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB21_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB21_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB21_RESERVED1_MASK) >> UED_ADVANCED_THB21_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB21_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB21_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB21_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB21_DIFF_SHIFT 0
#define  UED_ADVANCED_THB21_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB21_DIFF_MASK) >> UED_ADVANCED_THB21_DIFF_SHIFT)
#define  UED_ADVANCED_THB21_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB21_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB21_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 23 of 30) (THB22)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 23 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB22_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB22_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB22_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB22_RESERVED1_MASK) >> UED_ADVANCED_THB22_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB22_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB22_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB22_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB22_DIFF_SHIFT 0
#define  UED_ADVANCED_THB22_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB22_DIFF_MASK) >> UED_ADVANCED_THB22_DIFF_SHIFT)
#define  UED_ADVANCED_THB22_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB22_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB22_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 24 of 30) (THB23)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 24 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB23_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB23_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB23_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB23_RESERVED1_MASK) >> UED_ADVANCED_THB23_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB23_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB23_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB23_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB23_DIFF_SHIFT 0
#define  UED_ADVANCED_THB23_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB23_DIFF_MASK) >> UED_ADVANCED_THB23_DIFF_SHIFT)
#define  UED_ADVANCED_THB23_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB23_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB23_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 25 of 30) (THB24)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 25 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB24_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB24_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB24_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB24_RESERVED1_MASK) >> UED_ADVANCED_THB24_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB24_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB24_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB24_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB24_DIFF_SHIFT 0
#define  UED_ADVANCED_THB24_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB24_DIFF_MASK) >> UED_ADVANCED_THB24_DIFF_SHIFT)
#define  UED_ADVANCED_THB24_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB24_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB24_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 26 of 30) (THB25)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 26 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB25_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB25_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB25_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB25_RESERVED1_MASK) >> UED_ADVANCED_THB25_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB25_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB25_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB25_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB25_DIFF_SHIFT 0
#define  UED_ADVANCED_THB25_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB25_DIFF_MASK) >> UED_ADVANCED_THB25_DIFF_SHIFT)
#define  UED_ADVANCED_THB25_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB25_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB25_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 27 of 30) (THB26)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 27 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB26_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB26_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB26_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB26_RESERVED1_MASK) >> UED_ADVANCED_THB26_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB26_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB26_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB26_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB26_DIFF_SHIFT 0
#define  UED_ADVANCED_THB26_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB26_DIFF_MASK) >> UED_ADVANCED_THB26_DIFF_SHIFT)
#define  UED_ADVANCED_THB26_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB26_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB26_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 28 of 30) (THB27)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 28 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB27_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB27_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB27_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB27_RESERVED1_MASK) >> UED_ADVANCED_THB27_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB27_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB27_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB27_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB27_DIFF_SHIFT 0
#define  UED_ADVANCED_THB27_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB27_DIFF_MASK) >> UED_ADVANCED_THB27_DIFF_SHIFT)
#define  UED_ADVANCED_THB27_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB27_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB27_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 29 of 30) (THB28)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 29 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB28_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB28_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB28_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB28_RESERVED1_MASK) >> UED_ADVANCED_THB28_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB28_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB28_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB28_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB28_DIFF_SHIFT 0
#define  UED_ADVANCED_THB28_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB28_DIFF_MASK) >> UED_ADVANCED_THB28_DIFF_SHIFT)
#define  UED_ADVANCED_THB28_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB28_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB28_DIFF_SHIFT))

//====================================================================
//Register: Thresholding Registers for ED Pair (Instance 30 of 30) (THB29)
/** \brief When running in ED PAIR mode, an additional value in required for the threshold compare registers. This is the diffusion value. (Instance 30 of 30)*/
//====================================================================

#define  UED_ADVANCED_THB29_RESERVED1_MASK 0xfffffc00
#define  UED_ADVANCED_THB29_RESERVED1_SHIFT 10
#define  UED_ADVANCED_THB29_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB29_RESERVED1_MASK) >> UED_ADVANCED_THB29_RESERVED1_SHIFT)
#define  UED_ADVANCED_THB29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB29_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THB29_RESERVED1_SHIFT))

#define  UED_ADVANCED_THB29_DIFF_MASK 0x3ff
#define  UED_ADVANCED_THB29_DIFF_SHIFT 0
#define  UED_ADVANCED_THB29_DIFF_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THB29_DIFF_MASK) >> UED_ADVANCED_THB29_DIFF_SHIFT)
#define  UED_ADVANCED_THB29_DIFF_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THB29_DIFF_MASK) | (((uint32_t)val) << UED_ADVANCED_THB29_DIFF_SHIFT))

//====================================================================
//Register: Write Window for Weight SRAM (upper 32 bits [63:32]) (WLupW)
/** \brief To write the 64-bit Weight SRAM, first write this upper register, and then write the lower 32-bits to the LUT register. The two register values will be concatenated together and written to the LUT. When using UED in four-weight mode (see register CFGA field "weight8"), this register will be used to load the weights for the odd numbered tones. Note that the LUT is 128 x 64. This provides 7 address bits when in eight-weight mode and 8 logical address bits when in four-weight mode. The lsb's of the incoming tone are dropped correspondingly to form the address.*/
//====================================================================

#define  UED_ADVANCED_WLUPW_W7_MASK 0xff000000
#define  UED_ADVANCED_WLUPW_W7_SHIFT 24
#define  UED_ADVANCED_WLUPW_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPW_W7_MASK) >> UED_ADVANCED_WLUPW_W7_SHIFT)
#define  UED_ADVANCED_WLUPW_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPW_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPW_W7_SHIFT))

#define  UED_ADVANCED_WLUPW_W6_MASK 0xff0000
#define  UED_ADVANCED_WLUPW_W6_SHIFT 16
#define  UED_ADVANCED_WLUPW_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPW_W6_MASK) >> UED_ADVANCED_WLUPW_W6_SHIFT)
#define  UED_ADVANCED_WLUPW_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPW_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPW_W6_SHIFT))

#define  UED_ADVANCED_WLUPW_W5_MASK 0xff00
#define  UED_ADVANCED_WLUPW_W5_SHIFT 8
#define  UED_ADVANCED_WLUPW_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPW_W5_MASK) >> UED_ADVANCED_WLUPW_W5_SHIFT)
#define  UED_ADVANCED_WLUPW_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPW_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPW_W5_SHIFT))

#define  UED_ADVANCED_WLUPW_W4_MASK 0xff
#define  UED_ADVANCED_WLUPW_W4_SHIFT 0
#define  UED_ADVANCED_WLUPW_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPW_W4_MASK) >> UED_ADVANCED_WLUPW_W4_SHIFT)
#define  UED_ADVANCED_WLUPW_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPW_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPW_W4_SHIFT))

//====================================================================
//Register: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 1 of 6) (WLupR0)
/** \brief To read the 64-bit BIAS SRAM, first read the LUT to receive the lower 32-bits, then read this upper register and concatenate them together yourself in your code. When in four-weight mode the returned longword will provide two sets of weights. The lower four bytes are for the even tones, and the upper four bytes are for the odd numbered tones. In this case, the W4 field will hold the W0 value, and so forth. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUPR0_W7_MASK 0xff000000
#define  UED_ADVANCED_WLUPR0_W7_SHIFT 24
#define  UED_ADVANCED_WLUPR0_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR0_W7_MASK) >> UED_ADVANCED_WLUPR0_W7_SHIFT)
#define  UED_ADVANCED_WLUPR0_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR0_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR0_W7_SHIFT))

#define  UED_ADVANCED_WLUPR0_W6_MASK 0xff0000
#define  UED_ADVANCED_WLUPR0_W6_SHIFT 16
#define  UED_ADVANCED_WLUPR0_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR0_W6_MASK) >> UED_ADVANCED_WLUPR0_W6_SHIFT)
#define  UED_ADVANCED_WLUPR0_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR0_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR0_W6_SHIFT))

#define  UED_ADVANCED_WLUPR0_W5_MASK 0xff00
#define  UED_ADVANCED_WLUPR0_W5_SHIFT 8
#define  UED_ADVANCED_WLUPR0_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR0_W5_MASK) >> UED_ADVANCED_WLUPR0_W5_SHIFT)
#define  UED_ADVANCED_WLUPR0_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR0_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR0_W5_SHIFT))

#define  UED_ADVANCED_WLUPR0_W4_MASK 0xff
#define  UED_ADVANCED_WLUPR0_W4_SHIFT 0
#define  UED_ADVANCED_WLUPR0_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR0_W4_MASK) >> UED_ADVANCED_WLUPR0_W4_SHIFT)
#define  UED_ADVANCED_WLUPR0_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR0_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR0_W4_SHIFT))

//====================================================================
//Register: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 2 of 6) (WLupR1)
/** \brief To read the 64-bit BIAS SRAM, first read the LUT to receive the lower 32-bits, then read this upper register and concatenate them together yourself in your code. When in four-weight mode the returned longword will provide two sets of weights. The lower four bytes are for the even tones, and the upper four bytes are for the odd numbered tones. In this case, the W4 field will hold the W0 value, and so forth. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUPR1_W7_MASK 0xff000000
#define  UED_ADVANCED_WLUPR1_W7_SHIFT 24
#define  UED_ADVANCED_WLUPR1_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR1_W7_MASK) >> UED_ADVANCED_WLUPR1_W7_SHIFT)
#define  UED_ADVANCED_WLUPR1_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR1_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR1_W7_SHIFT))

#define  UED_ADVANCED_WLUPR1_W6_MASK 0xff0000
#define  UED_ADVANCED_WLUPR1_W6_SHIFT 16
#define  UED_ADVANCED_WLUPR1_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR1_W6_MASK) >> UED_ADVANCED_WLUPR1_W6_SHIFT)
#define  UED_ADVANCED_WLUPR1_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR1_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR1_W6_SHIFT))

#define  UED_ADVANCED_WLUPR1_W5_MASK 0xff00
#define  UED_ADVANCED_WLUPR1_W5_SHIFT 8
#define  UED_ADVANCED_WLUPR1_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR1_W5_MASK) >> UED_ADVANCED_WLUPR1_W5_SHIFT)
#define  UED_ADVANCED_WLUPR1_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR1_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR1_W5_SHIFT))

#define  UED_ADVANCED_WLUPR1_W4_MASK 0xff
#define  UED_ADVANCED_WLUPR1_W4_SHIFT 0
#define  UED_ADVANCED_WLUPR1_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR1_W4_MASK) >> UED_ADVANCED_WLUPR1_W4_SHIFT)
#define  UED_ADVANCED_WLUPR1_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR1_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR1_W4_SHIFT))

//====================================================================
//Register: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 3 of 6) (WLupR2)
/** \brief To read the 64-bit BIAS SRAM, first read the LUT to receive the lower 32-bits, then read this upper register and concatenate them together yourself in your code. When in four-weight mode the returned longword will provide two sets of weights. The lower four bytes are for the even tones, and the upper four bytes are for the odd numbered tones. In this case, the W4 field will hold the W0 value, and so forth. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUPR2_W7_MASK 0xff000000
#define  UED_ADVANCED_WLUPR2_W7_SHIFT 24
#define  UED_ADVANCED_WLUPR2_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR2_W7_MASK) >> UED_ADVANCED_WLUPR2_W7_SHIFT)
#define  UED_ADVANCED_WLUPR2_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR2_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR2_W7_SHIFT))

#define  UED_ADVANCED_WLUPR2_W6_MASK 0xff0000
#define  UED_ADVANCED_WLUPR2_W6_SHIFT 16
#define  UED_ADVANCED_WLUPR2_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR2_W6_MASK) >> UED_ADVANCED_WLUPR2_W6_SHIFT)
#define  UED_ADVANCED_WLUPR2_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR2_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR2_W6_SHIFT))

#define  UED_ADVANCED_WLUPR2_W5_MASK 0xff00
#define  UED_ADVANCED_WLUPR2_W5_SHIFT 8
#define  UED_ADVANCED_WLUPR2_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR2_W5_MASK) >> UED_ADVANCED_WLUPR2_W5_SHIFT)
#define  UED_ADVANCED_WLUPR2_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR2_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR2_W5_SHIFT))

#define  UED_ADVANCED_WLUPR2_W4_MASK 0xff
#define  UED_ADVANCED_WLUPR2_W4_SHIFT 0
#define  UED_ADVANCED_WLUPR2_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR2_W4_MASK) >> UED_ADVANCED_WLUPR2_W4_SHIFT)
#define  UED_ADVANCED_WLUPR2_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR2_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR2_W4_SHIFT))

//====================================================================
//Register: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 4 of 6) (WLupR3)
/** \brief To read the 64-bit BIAS SRAM, first read the LUT to receive the lower 32-bits, then read this upper register and concatenate them together yourself in your code. When in four-weight mode the returned longword will provide two sets of weights. The lower four bytes are for the even tones, and the upper four bytes are for the odd numbered tones. In this case, the W4 field will hold the W0 value, and so forth. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUPR3_W7_MASK 0xff000000
#define  UED_ADVANCED_WLUPR3_W7_SHIFT 24
#define  UED_ADVANCED_WLUPR3_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR3_W7_MASK) >> UED_ADVANCED_WLUPR3_W7_SHIFT)
#define  UED_ADVANCED_WLUPR3_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR3_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR3_W7_SHIFT))

#define  UED_ADVANCED_WLUPR3_W6_MASK 0xff0000
#define  UED_ADVANCED_WLUPR3_W6_SHIFT 16
#define  UED_ADVANCED_WLUPR3_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR3_W6_MASK) >> UED_ADVANCED_WLUPR3_W6_SHIFT)
#define  UED_ADVANCED_WLUPR3_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR3_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR3_W6_SHIFT))

#define  UED_ADVANCED_WLUPR3_W5_MASK 0xff00
#define  UED_ADVANCED_WLUPR3_W5_SHIFT 8
#define  UED_ADVANCED_WLUPR3_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR3_W5_MASK) >> UED_ADVANCED_WLUPR3_W5_SHIFT)
#define  UED_ADVANCED_WLUPR3_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR3_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR3_W5_SHIFT))

#define  UED_ADVANCED_WLUPR3_W4_MASK 0xff
#define  UED_ADVANCED_WLUPR3_W4_SHIFT 0
#define  UED_ADVANCED_WLUPR3_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR3_W4_MASK) >> UED_ADVANCED_WLUPR3_W4_SHIFT)
#define  UED_ADVANCED_WLUPR3_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR3_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR3_W4_SHIFT))

//====================================================================
//Register: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 5 of 6) (WLupR4)
/** \brief To read the 64-bit BIAS SRAM, first read the LUT to receive the lower 32-bits, then read this upper register and concatenate them together yourself in your code. When in four-weight mode the returned longword will provide two sets of weights. The lower four bytes are for the even tones, and the upper four bytes are for the odd numbered tones. In this case, the W4 field will hold the W0 value, and so forth. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUPR4_W7_MASK 0xff000000
#define  UED_ADVANCED_WLUPR4_W7_SHIFT 24
#define  UED_ADVANCED_WLUPR4_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR4_W7_MASK) >> UED_ADVANCED_WLUPR4_W7_SHIFT)
#define  UED_ADVANCED_WLUPR4_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR4_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR4_W7_SHIFT))

#define  UED_ADVANCED_WLUPR4_W6_MASK 0xff0000
#define  UED_ADVANCED_WLUPR4_W6_SHIFT 16
#define  UED_ADVANCED_WLUPR4_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR4_W6_MASK) >> UED_ADVANCED_WLUPR4_W6_SHIFT)
#define  UED_ADVANCED_WLUPR4_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR4_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR4_W6_SHIFT))

#define  UED_ADVANCED_WLUPR4_W5_MASK 0xff00
#define  UED_ADVANCED_WLUPR4_W5_SHIFT 8
#define  UED_ADVANCED_WLUPR4_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR4_W5_MASK) >> UED_ADVANCED_WLUPR4_W5_SHIFT)
#define  UED_ADVANCED_WLUPR4_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR4_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR4_W5_SHIFT))

#define  UED_ADVANCED_WLUPR4_W4_MASK 0xff
#define  UED_ADVANCED_WLUPR4_W4_SHIFT 0
#define  UED_ADVANCED_WLUPR4_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR4_W4_MASK) >> UED_ADVANCED_WLUPR4_W4_SHIFT)
#define  UED_ADVANCED_WLUPR4_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR4_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR4_W4_SHIFT))

//====================================================================
//Register: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 6 of 6) (WLupR5)
/** \brief To read the 64-bit BIAS SRAM, first read the LUT to receive the lower 32-bits, then read this upper register and concatenate them together yourself in your code. When in four-weight mode the returned longword will provide two sets of weights. The lower four bytes are for the even tones, and the upper four bytes are for the odd numbered tones. In this case, the W4 field will hold the W0 value, and so forth. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_WLUPR5_W7_MASK 0xff000000
#define  UED_ADVANCED_WLUPR5_W7_SHIFT 24
#define  UED_ADVANCED_WLUPR5_W7_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR5_W7_MASK) >> UED_ADVANCED_WLUPR5_W7_SHIFT)
#define  UED_ADVANCED_WLUPR5_W7_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR5_W7_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR5_W7_SHIFT))

#define  UED_ADVANCED_WLUPR5_W6_MASK 0xff0000
#define  UED_ADVANCED_WLUPR5_W6_SHIFT 16
#define  UED_ADVANCED_WLUPR5_W6_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR5_W6_MASK) >> UED_ADVANCED_WLUPR5_W6_SHIFT)
#define  UED_ADVANCED_WLUPR5_W6_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR5_W6_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR5_W6_SHIFT))

#define  UED_ADVANCED_WLUPR5_W5_MASK 0xff00
#define  UED_ADVANCED_WLUPR5_W5_SHIFT 8
#define  UED_ADVANCED_WLUPR5_W5_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR5_W5_MASK) >> UED_ADVANCED_WLUPR5_W5_SHIFT)
#define  UED_ADVANCED_WLUPR5_W5_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR5_W5_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR5_W5_SHIFT))

#define  UED_ADVANCED_WLUPR5_W4_MASK 0xff
#define  UED_ADVANCED_WLUPR5_W4_SHIFT 0
#define  UED_ADVANCED_WLUPR5_W4_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_WLUPR5_W4_MASK) >> UED_ADVANCED_WLUPR5_W4_SHIFT)
#define  UED_ADVANCED_WLUPR5_W4_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_WLUPR5_W4_MASK) | (((uint32_t)val) << UED_ADVANCED_WLUPR5_W4_SHIFT))

//====================================================================
//Register: Error Compression Control (ECMP1)
/** \brief This register contains the control bit enabling/bypassing error compression.*/
//====================================================================

#define  UED_ADVANCED_ECMP1_RESERVED1_MASK 0xfffffff8
#define  UED_ADVANCED_ECMP1_RESERVED1_SHIFT 3
#define  UED_ADVANCED_ECMP1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP1_RESERVED1_MASK) >> UED_ADVANCED_ECMP1_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECMP1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP1_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECMP1_SIZE_OPT_MASK 0x4
#define  UED_ADVANCED_ECMP1_SIZE_OPT_SHIFT 2
#define  UED_ADVANCED_ECMP1_SIZE_OPT_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP1_SIZE_OPT_MASK) >> UED_ADVANCED_ECMP1_SIZE_OPT_SHIFT)
#define  UED_ADVANCED_ECMP1_SIZE_OPT_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP1_SIZE_OPT_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP1_SIZE_OPT_SHIFT))

#define  UED_ADVANCED_ECMP1_CRUMBS_MASK 0x2
#define  UED_ADVANCED_ECMP1_CRUMBS_SHIFT 1
#define  UED_ADVANCED_ECMP1_CRUMBS_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP1_CRUMBS_MASK) >> UED_ADVANCED_ECMP1_CRUMBS_SHIFT)
#define  UED_ADVANCED_ECMP1_CRUMBS_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP1_CRUMBS_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP1_CRUMBS_SHIFT))

#define  UED_ADVANCED_ECMP1_ENABLE_MASK 0x1
#define  UED_ADVANCED_ECMP1_ENABLE_SHIFT 0
#define  UED_ADVANCED_ECMP1_ENABLE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP1_ENABLE_MASK) >> UED_ADVANCED_ECMP1_ENABLE_SHIFT)
#define  UED_ADVANCED_ECMP1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP1_ENABLE_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP1_ENABLE_SHIFT))

//====================================================================
//Register: Error Decompression Shift Settings (ECMP3)
/** \brief This register contains the shift decompression mapping for incoming error.
      Each four bits represent a shift for a given decode.
      Decodes can range from 0-3, creating the need for 4 values, each being 4 bits.
      Bits 3:0 - Shift applied for a decode of 0, bits 7:4 - Shift applied for a decode of 1, bits 11:8 - Shift applied for a decode of 2, bits 15:12 - Shift applied for a decode of 3.*/
//====================================================================

#define  UED_ADVANCED_ECMP3_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_ECMP3_RESERVED1_SHIFT 16
#define  UED_ADVANCED_ECMP3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP3_RESERVED1_MASK) >> UED_ADVANCED_ECMP3_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECMP3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP3_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECMP3_SH3_MASK 0xf000
#define  UED_ADVANCED_ECMP3_SH3_SHIFT 12
#define  UED_ADVANCED_ECMP3_SH3_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP3_SH3_MASK) >> UED_ADVANCED_ECMP3_SH3_SHIFT)
#define  UED_ADVANCED_ECMP3_SH3_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP3_SH3_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP3_SH3_SHIFT))

#define  UED_ADVANCED_ECMP3_SH2_MASK 0xf00
#define  UED_ADVANCED_ECMP3_SH2_SHIFT 8
#define  UED_ADVANCED_ECMP3_SH2_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP3_SH2_MASK) >> UED_ADVANCED_ECMP3_SH2_SHIFT)
#define  UED_ADVANCED_ECMP3_SH2_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP3_SH2_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP3_SH2_SHIFT))

#define  UED_ADVANCED_ECMP3_SH1_MASK 0xf0
#define  UED_ADVANCED_ECMP3_SH1_SHIFT 4
#define  UED_ADVANCED_ECMP3_SH1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP3_SH1_MASK) >> UED_ADVANCED_ECMP3_SH1_SHIFT)
#define  UED_ADVANCED_ECMP3_SH1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP3_SH1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP3_SH1_SHIFT))

#define  UED_ADVANCED_ECMP3_SH0_MASK 0xf
#define  UED_ADVANCED_ECMP3_SH0_SHIFT 0
#define  UED_ADVANCED_ECMP3_SH0_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECMP3_SH0_MASK) >> UED_ADVANCED_ECMP3_SH0_SHIFT)
#define  UED_ADVANCED_ECMP3_SH0_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECMP3_SH0_MASK) | (((uint32_t)val) << UED_ADVANCED_ECMP3_SH0_SHIFT))

//====================================================================
//Register: Error Current Access (Instance 1 of 6) (ECA0)
/** \brief This register provides context switch support for the ErrorCurrent value in the 1 block. If ALUT error compression is enabled, this value must be read from each channel and restored as part of the context switch. (Instance 1 of 6)*/
//====================================================================

#define  UED_ADVANCED_ECA0_RESERVED1_MASK 0xffffe000
#define  UED_ADVANCED_ECA0_RESERVED1_SHIFT 13
#define  UED_ADVANCED_ECA0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA0_RESERVED1_MASK) >> UED_ADVANCED_ECA0_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECA0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA0_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECA0_ERRCURR_MASK 0x1fff
#define  UED_ADVANCED_ECA0_ERRCURR_SHIFT 0
#define  UED_ADVANCED_ECA0_ERRCURR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA0_ERRCURR_MASK) >> UED_ADVANCED_ECA0_ERRCURR_SHIFT)
#define  UED_ADVANCED_ECA0_ERRCURR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA0_ERRCURR_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA0_ERRCURR_SHIFT))

//====================================================================
//Register: Error Current Access (Instance 2 of 6) (ECA1)
/** \brief This register provides context switch support for the ErrorCurrent value in the 1 block. If ALUT error compression is enabled, this value must be read from each channel and restored as part of the context switch. (Instance 2 of 6)*/
//====================================================================

#define  UED_ADVANCED_ECA1_RESERVED1_MASK 0xffffe000
#define  UED_ADVANCED_ECA1_RESERVED1_SHIFT 13
#define  UED_ADVANCED_ECA1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA1_RESERVED1_MASK) >> UED_ADVANCED_ECA1_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECA1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA1_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECA1_ERRCURR_MASK 0x1fff
#define  UED_ADVANCED_ECA1_ERRCURR_SHIFT 0
#define  UED_ADVANCED_ECA1_ERRCURR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA1_ERRCURR_MASK) >> UED_ADVANCED_ECA1_ERRCURR_SHIFT)
#define  UED_ADVANCED_ECA1_ERRCURR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA1_ERRCURR_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA1_ERRCURR_SHIFT))

//====================================================================
//Register: Error Current Access (Instance 3 of 6) (ECA2)
/** \brief This register provides context switch support for the ErrorCurrent value in the 1 block. If ALUT error compression is enabled, this value must be read from each channel and restored as part of the context switch. (Instance 3 of 6)*/
//====================================================================

#define  UED_ADVANCED_ECA2_RESERVED1_MASK 0xffffe000
#define  UED_ADVANCED_ECA2_RESERVED1_SHIFT 13
#define  UED_ADVANCED_ECA2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA2_RESERVED1_MASK) >> UED_ADVANCED_ECA2_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECA2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA2_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECA2_ERRCURR_MASK 0x1fff
#define  UED_ADVANCED_ECA2_ERRCURR_SHIFT 0
#define  UED_ADVANCED_ECA2_ERRCURR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA2_ERRCURR_MASK) >> UED_ADVANCED_ECA2_ERRCURR_SHIFT)
#define  UED_ADVANCED_ECA2_ERRCURR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA2_ERRCURR_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA2_ERRCURR_SHIFT))

//====================================================================
//Register: Error Current Access (Instance 4 of 6) (ECA3)
/** \brief This register provides context switch support for the ErrorCurrent value in the 1 block. If ALUT error compression is enabled, this value must be read from each channel and restored as part of the context switch. (Instance 4 of 6)*/
//====================================================================

#define  UED_ADVANCED_ECA3_RESERVED1_MASK 0xffffe000
#define  UED_ADVANCED_ECA3_RESERVED1_SHIFT 13
#define  UED_ADVANCED_ECA3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA3_RESERVED1_MASK) >> UED_ADVANCED_ECA3_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECA3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA3_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECA3_ERRCURR_MASK 0x1fff
#define  UED_ADVANCED_ECA3_ERRCURR_SHIFT 0
#define  UED_ADVANCED_ECA3_ERRCURR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA3_ERRCURR_MASK) >> UED_ADVANCED_ECA3_ERRCURR_SHIFT)
#define  UED_ADVANCED_ECA3_ERRCURR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA3_ERRCURR_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA3_ERRCURR_SHIFT))

//====================================================================
//Register: Error Current Access (Instance 5 of 6) (ECA4)
/** \brief This register provides context switch support for the ErrorCurrent value in the 1 block. If ALUT error compression is enabled, this value must be read from each channel and restored as part of the context switch. (Instance 5 of 6)*/
//====================================================================

#define  UED_ADVANCED_ECA4_RESERVED1_MASK 0xffffe000
#define  UED_ADVANCED_ECA4_RESERVED1_SHIFT 13
#define  UED_ADVANCED_ECA4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA4_RESERVED1_MASK) >> UED_ADVANCED_ECA4_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECA4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA4_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECA4_ERRCURR_MASK 0x1fff
#define  UED_ADVANCED_ECA4_ERRCURR_SHIFT 0
#define  UED_ADVANCED_ECA4_ERRCURR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA4_ERRCURR_MASK) >> UED_ADVANCED_ECA4_ERRCURR_SHIFT)
#define  UED_ADVANCED_ECA4_ERRCURR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA4_ERRCURR_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA4_ERRCURR_SHIFT))

//====================================================================
//Register: Error Current Access (Instance 6 of 6) (ECA5)
/** \brief This register provides context switch support for the ErrorCurrent value in the 1 block. If ALUT error compression is enabled, this value must be read from each channel and restored as part of the context switch. (Instance 6 of 6)*/
//====================================================================

#define  UED_ADVANCED_ECA5_RESERVED1_MASK 0xffffe000
#define  UED_ADVANCED_ECA5_RESERVED1_SHIFT 13
#define  UED_ADVANCED_ECA5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA5_RESERVED1_MASK) >> UED_ADVANCED_ECA5_RESERVED1_SHIFT)
#define  UED_ADVANCED_ECA5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA5_RESERVED1_SHIFT))

#define  UED_ADVANCED_ECA5_ERRCURR_MASK 0x1fff
#define  UED_ADVANCED_ECA5_ERRCURR_SHIFT 0
#define  UED_ADVANCED_ECA5_ERRCURR_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_ECA5_ERRCURR_MASK) >> UED_ADVANCED_ECA5_ERRCURR_SHIFT)
#define  UED_ADVANCED_ECA5_ERRCURR_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_ECA5_ERRCURR_MASK) | (((uint32_t)val) << UED_ADVANCED_ECA5_ERRCURR_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 1 of 9) (THP0)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 1 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP0_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP0_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP0_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP0_RESERVED1_MASK) >> UED_ADVANCED_THP0_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP0_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP0_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP0_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP0_SIZE_SHIFT 12
#define  UED_ADVANCED_THP0_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP0_SIZE_MASK) >> UED_ADVANCED_THP0_SIZE_SHIFT)
#define  UED_ADVANCED_THP0_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP0_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP0_SIZE_SHIFT))

#define  UED_ADVANCED_THP0_COMP_MASK 0xfff
#define  UED_ADVANCED_THP0_COMP_SHIFT 0
#define  UED_ADVANCED_THP0_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP0_COMP_MASK) >> UED_ADVANCED_THP0_COMP_SHIFT)
#define  UED_ADVANCED_THP0_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP0_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP0_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 2 of 9) (THP1)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 2 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP1_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP1_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP1_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP1_RESERVED1_MASK) >> UED_ADVANCED_THP1_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP1_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP1_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP1_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP1_SIZE_SHIFT 12
#define  UED_ADVANCED_THP1_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP1_SIZE_MASK) >> UED_ADVANCED_THP1_SIZE_SHIFT)
#define  UED_ADVANCED_THP1_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP1_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP1_SIZE_SHIFT))

#define  UED_ADVANCED_THP1_COMP_MASK 0xfff
#define  UED_ADVANCED_THP1_COMP_SHIFT 0
#define  UED_ADVANCED_THP1_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP1_COMP_MASK) >> UED_ADVANCED_THP1_COMP_SHIFT)
#define  UED_ADVANCED_THP1_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP1_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP1_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 3 of 9) (THP2)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 3 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP2_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP2_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP2_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP2_RESERVED1_MASK) >> UED_ADVANCED_THP2_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP2_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP2_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP2_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP2_SIZE_SHIFT 12
#define  UED_ADVANCED_THP2_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP2_SIZE_MASK) >> UED_ADVANCED_THP2_SIZE_SHIFT)
#define  UED_ADVANCED_THP2_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP2_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP2_SIZE_SHIFT))

#define  UED_ADVANCED_THP2_COMP_MASK 0xfff
#define  UED_ADVANCED_THP2_COMP_SHIFT 0
#define  UED_ADVANCED_THP2_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP2_COMP_MASK) >> UED_ADVANCED_THP2_COMP_SHIFT)
#define  UED_ADVANCED_THP2_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP2_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP2_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 4 of 9) (THP3)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 4 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP3_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP3_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP3_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP3_RESERVED1_MASK) >> UED_ADVANCED_THP3_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP3_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP3_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP3_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP3_SIZE_SHIFT 12
#define  UED_ADVANCED_THP3_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP3_SIZE_MASK) >> UED_ADVANCED_THP3_SIZE_SHIFT)
#define  UED_ADVANCED_THP3_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP3_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP3_SIZE_SHIFT))

#define  UED_ADVANCED_THP3_COMP_MASK 0xfff
#define  UED_ADVANCED_THP3_COMP_SHIFT 0
#define  UED_ADVANCED_THP3_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP3_COMP_MASK) >> UED_ADVANCED_THP3_COMP_SHIFT)
#define  UED_ADVANCED_THP3_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP3_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP3_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 5 of 9) (THP4)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 5 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP4_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP4_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP4_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP4_RESERVED1_MASK) >> UED_ADVANCED_THP4_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP4_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP4_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP4_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP4_SIZE_SHIFT 12
#define  UED_ADVANCED_THP4_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP4_SIZE_MASK) >> UED_ADVANCED_THP4_SIZE_SHIFT)
#define  UED_ADVANCED_THP4_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP4_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP4_SIZE_SHIFT))

#define  UED_ADVANCED_THP4_COMP_MASK 0xfff
#define  UED_ADVANCED_THP4_COMP_SHIFT 0
#define  UED_ADVANCED_THP4_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP4_COMP_MASK) >> UED_ADVANCED_THP4_COMP_SHIFT)
#define  UED_ADVANCED_THP4_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP4_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP4_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 6 of 9) (THP5)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 6 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP5_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP5_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP5_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP5_RESERVED1_MASK) >> UED_ADVANCED_THP5_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP5_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP5_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP5_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP5_SIZE_SHIFT 12
#define  UED_ADVANCED_THP5_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP5_SIZE_MASK) >> UED_ADVANCED_THP5_SIZE_SHIFT)
#define  UED_ADVANCED_THP5_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP5_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP5_SIZE_SHIFT))

#define  UED_ADVANCED_THP5_COMP_MASK 0xfff
#define  UED_ADVANCED_THP5_COMP_SHIFT 0
#define  UED_ADVANCED_THP5_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP5_COMP_MASK) >> UED_ADVANCED_THP5_COMP_SHIFT)
#define  UED_ADVANCED_THP5_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP5_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP5_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 7 of 9) (THP6)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 7 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP6_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP6_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP6_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP6_RESERVED1_MASK) >> UED_ADVANCED_THP6_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP6_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP6_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP6_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP6_SIZE_SHIFT 12
#define  UED_ADVANCED_THP6_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP6_SIZE_MASK) >> UED_ADVANCED_THP6_SIZE_SHIFT)
#define  UED_ADVANCED_THP6_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP6_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP6_SIZE_SHIFT))

#define  UED_ADVANCED_THP6_COMP_MASK 0xfff
#define  UED_ADVANCED_THP6_COMP_SHIFT 0
#define  UED_ADVANCED_THP6_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP6_COMP_MASK) >> UED_ADVANCED_THP6_COMP_SHIFT)
#define  UED_ADVANCED_THP6_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP6_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP6_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 8 of 9) (THP7)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 8 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP7_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP7_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP7_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP7_RESERVED1_MASK) >> UED_ADVANCED_THP7_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP7_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP7_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP7_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP7_SIZE_SHIFT 12
#define  UED_ADVANCED_THP7_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP7_SIZE_MASK) >> UED_ADVANCED_THP7_SIZE_SHIFT)
#define  UED_ADVANCED_THP7_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP7_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP7_SIZE_SHIFT))

#define  UED_ADVANCED_THP7_COMP_MASK 0xfff
#define  UED_ADVANCED_THP7_COMP_SHIFT 0
#define  UED_ADVANCED_THP7_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP7_COMP_MASK) >> UED_ADVANCED_THP7_COMP_SHIFT)
#define  UED_ADVANCED_THP7_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP7_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP7_COMP_SHIFT))

//====================================================================
//Register: PSA Thresholding Registers (Instance 9 of 9) (THP8)
/** \brief This set of registers mark the thresholding boundaries for the combined C, M, K psuedo-channel used in the new 1 algorithm. Only the "comp" field is required as the individual, real C, M, K channels still perform the actual halftoning. (Instance 9 of 9)*/
//====================================================================

#define  UED_ADVANCED_THP8_RESERVED1_MASK 0xffff0000
#define  UED_ADVANCED_THP8_RESERVED1_SHIFT 16
#define  UED_ADVANCED_THP8_RESERVED1_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP8_RESERVED1_MASK) >> UED_ADVANCED_THP8_RESERVED1_SHIFT)
#define  UED_ADVANCED_THP8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP8_RESERVED1_MASK) | (((uint32_t)val) << UED_ADVANCED_THP8_RESERVED1_SHIFT))

#define  UED_ADVANCED_THP8_SIZE_MASK 0xf000
#define  UED_ADVANCED_THP8_SIZE_SHIFT 12
#define  UED_ADVANCED_THP8_SIZE_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP8_SIZE_MASK) >> UED_ADVANCED_THP8_SIZE_SHIFT)
#define  UED_ADVANCED_THP8_SIZE_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP8_SIZE_MASK) | (((uint32_t)val) << UED_ADVANCED_THP8_SIZE_SHIFT))

#define  UED_ADVANCED_THP8_COMP_MASK 0xfff
#define  UED_ADVANCED_THP8_COMP_SHIFT 0
#define  UED_ADVANCED_THP8_COMP_MASK_SHIFT(reg) (((reg) & UED_ADVANCED_THP8_COMP_MASK) >> UED_ADVANCED_THP8_COMP_SHIFT)
#define  UED_ADVANCED_THP8_COMP_REPLACE_VAL(reg,val) (((reg) & ~UED_ADVANCED_THP8_COMP_MASK) | (((uint32_t)val) << UED_ADVANCED_THP8_COMP_SHIFT))

#endif // UED_ADVANCED
