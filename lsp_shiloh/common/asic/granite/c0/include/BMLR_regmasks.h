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
 * \file BMLR_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BMLR_REGMASKS_H_
#define _BMLR_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BulbMonLRMargin (BMLR)
/** \brief Register Descriptions for the PIC BulbMonLRMargin block*/
//
//====================================================================

//====================================================================
//Register: Bulb Monitor Configuration Register (BMCR)
/** \brief General configuration register for BulbMonLRMargin block.*/
//====================================================================

#define  BMLR_BMCR_BYPASS_MASK 0x80000000
#define  BMLR_BMCR_BYPASS_SHIFT 31
#define  BMLR_BMCR_BYPASS_MASK_SHIFT(reg) (((reg) & BMLR_BMCR_BYPASS_MASK) >> BMLR_BMCR_BYPASS_SHIFT)
#define  BMLR_BMCR_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMCR_BYPASS_MASK) | (((uint32_t)val) << BMLR_BMCR_BYPASS_SHIFT))

#define  BMLR_BMCR_COLORCOUNT_MASK 0x40000000
#define  BMLR_BMCR_COLORCOUNT_SHIFT 30
#define  BMLR_BMCR_COLORCOUNT_MASK_SHIFT(reg) (((reg) & BMLR_BMCR_COLORCOUNT_MASK) >> BMLR_BMCR_COLORCOUNT_SHIFT)
#define  BMLR_BMCR_COLORCOUNT_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMCR_COLORCOUNT_MASK) | (((uint32_t)val) << BMLR_BMCR_COLORCOUNT_SHIFT))

#define  BMLR_BMCR_RESERVED1_MASK 0x38000000
#define  BMLR_BMCR_RESERVED1_SHIFT 27
#define  BMLR_BMCR_RESERVED1_MASK_SHIFT(reg) (((reg) & BMLR_BMCR_RESERVED1_MASK) >> BMLR_BMCR_RESERVED1_SHIFT)
#define  BMLR_BMCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMCR_RESERVED1_MASK) | (((uint32_t)val) << BMLR_BMCR_RESERVED1_SHIFT))

#define  BMLR_BMCR_LINESTOAVE_MASK 0x7000000
#define  BMLR_BMCR_LINESTOAVE_SHIFT 24
#define  BMLR_BMCR_LINESTOAVE_MASK_SHIFT(reg) (((reg) & BMLR_BMCR_LINESTOAVE_MASK) >> BMLR_BMCR_LINESTOAVE_SHIFT)
#define  BMLR_BMCR_LINESTOAVE_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMCR_LINESTOAVE_MASK) | (((uint32_t)val) << BMLR_BMCR_LINESTOAVE_SHIFT))

#define  BMLR_BMCR_WINLEN_MASK 0xff0000
#define  BMLR_BMCR_WINLEN_SHIFT 16
#define  BMLR_BMCR_WINLEN_MASK_SHIFT(reg) (((reg) & BMLR_BMCR_WINLEN_MASK) >> BMLR_BMCR_WINLEN_SHIFT)
#define  BMLR_BMCR_WINLEN_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMCR_WINLEN_MASK) | (((uint32_t)val) << BMLR_BMCR_WINLEN_SHIFT))

#define  BMLR_BMCR_STARTPIX_MASK 0xffff
#define  BMLR_BMCR_STARTPIX_SHIFT 0
#define  BMLR_BMCR_STARTPIX_MASK_SHIFT(reg) (((reg) & BMLR_BMCR_STARTPIX_MASK) >> BMLR_BMCR_STARTPIX_SHIFT)
#define  BMLR_BMCR_STARTPIX_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMCR_STARTPIX_MASK) | (((uint32_t)val) << BMLR_BMCR_STARTPIX_SHIFT))

//====================================================================
//Register: Left/Right Margins Register (LRMR0)
//====================================================================

#define  BMLR_LRMR0_RIGHTMARGINC0_MASK 0xffff0000
#define  BMLR_LRMR0_RIGHTMARGINC0_SHIFT 16
#define  BMLR_LRMR0_RIGHTMARGINC0_MASK_SHIFT(reg) (((reg) & BMLR_LRMR0_RIGHTMARGINC0_MASK) >> BMLR_LRMR0_RIGHTMARGINC0_SHIFT)
#define  BMLR_LRMR0_RIGHTMARGINC0_REPLACE_VAL(reg,val) (((reg) & ~BMLR_LRMR0_RIGHTMARGINC0_MASK) | (((uint32_t)val) << BMLR_LRMR0_RIGHTMARGINC0_SHIFT))

#define  BMLR_LRMR0_LEFTMARGINC0_MASK 0xffff
#define  BMLR_LRMR0_LEFTMARGINC0_SHIFT 0
#define  BMLR_LRMR0_LEFTMARGINC0_MASK_SHIFT(reg) (((reg) & BMLR_LRMR0_LEFTMARGINC0_MASK) >> BMLR_LRMR0_LEFTMARGINC0_SHIFT)
#define  BMLR_LRMR0_LEFTMARGINC0_REPLACE_VAL(reg,val) (((reg) & ~BMLR_LRMR0_LEFTMARGINC0_MASK) | (((uint32_t)val) << BMLR_LRMR0_LEFTMARGINC0_SHIFT))

//====================================================================
//Register: Left/Right Margins Register (LRMR1)
//====================================================================

#define  BMLR_LRMR1_RIGHTMARGINC1_MASK 0xffff0000
#define  BMLR_LRMR1_RIGHTMARGINC1_SHIFT 16
#define  BMLR_LRMR1_RIGHTMARGINC1_MASK_SHIFT(reg) (((reg) & BMLR_LRMR1_RIGHTMARGINC1_MASK) >> BMLR_LRMR1_RIGHTMARGINC1_SHIFT)
#define  BMLR_LRMR1_RIGHTMARGINC1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_LRMR1_RIGHTMARGINC1_MASK) | (((uint32_t)val) << BMLR_LRMR1_RIGHTMARGINC1_SHIFT))

#define  BMLR_LRMR1_LEFTMARGINC1_MASK 0xffff
#define  BMLR_LRMR1_LEFTMARGINC1_SHIFT 0
#define  BMLR_LRMR1_LEFTMARGINC1_MASK_SHIFT(reg) (((reg) & BMLR_LRMR1_LEFTMARGINC1_MASK) >> BMLR_LRMR1_LEFTMARGINC1_SHIFT)
#define  BMLR_LRMR1_LEFTMARGINC1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_LRMR1_LEFTMARGINC1_MASK) | (((uint32_t)val) << BMLR_LRMR1_LEFTMARGINC1_SHIFT))

//====================================================================
//Register: Left/Right Margins Register (LRMR2)
//====================================================================

#define  BMLR_LRMR2_RIGHTMARGINC2_MASK 0xffff0000
#define  BMLR_LRMR2_RIGHTMARGINC2_SHIFT 16
#define  BMLR_LRMR2_RIGHTMARGINC2_MASK_SHIFT(reg) (((reg) & BMLR_LRMR2_RIGHTMARGINC2_MASK) >> BMLR_LRMR2_RIGHTMARGINC2_SHIFT)
#define  BMLR_LRMR2_RIGHTMARGINC2_REPLACE_VAL(reg,val) (((reg) & ~BMLR_LRMR2_RIGHTMARGINC2_MASK) | (((uint32_t)val) << BMLR_LRMR2_RIGHTMARGINC2_SHIFT))

#define  BMLR_LRMR2_LEFTMARGINC2_MASK 0xffff
#define  BMLR_LRMR2_LEFTMARGINC2_SHIFT 0
#define  BMLR_LRMR2_LEFTMARGINC2_MASK_SHIFT(reg) (((reg) & BMLR_LRMR2_LEFTMARGINC2_MASK) >> BMLR_LRMR2_LEFTMARGINC2_SHIFT)
#define  BMLR_LRMR2_LEFTMARGINC2_REPLACE_VAL(reg,val) (((reg) & ~BMLR_LRMR2_LEFTMARGINC2_MASK) | (((uint32_t)val) << BMLR_LRMR2_LEFTMARGINC2_SHIFT))

//====================================================================
//Register: Bulb Monitor Color0 Register (BMR0)
/** \brief BMR0 is cleared by BypassALL, SoftReset and Hard Reset (nReset)*/
//====================================================================

#define  BMLR_BMR0_RESERVED1_MASK 0xff000000
#define  BMLR_BMR0_RESERVED1_SHIFT 24
#define  BMLR_BMR0_RESERVED1_MASK_SHIFT(reg) (((reg) & BMLR_BMR0_RESERVED1_MASK) >> BMLR_BMR0_RESERVED1_SHIFT)
#define  BMLR_BMR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR0_RESERVED1_MASK) | (((uint32_t)val) << BMLR_BMR0_RESERVED1_SHIFT))

#define  BMLR_BMR0_AVERAGEC0_MASK 0xffffff
#define  BMLR_BMR0_AVERAGEC0_SHIFT 0
#define  BMLR_BMR0_AVERAGEC0_MASK_SHIFT(reg) (((reg) & BMLR_BMR0_AVERAGEC0_MASK) >> BMLR_BMR0_AVERAGEC0_SHIFT)
#define  BMLR_BMR0_AVERAGEC0_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR0_AVERAGEC0_MASK) | (((uint32_t)val) << BMLR_BMR0_AVERAGEC0_SHIFT))

//====================================================================
//Register: Bulb Monitor Color1 Register (BMR1)
/** \brief BMR1 is cleared by BypassALL, SoftReset and Hard Reset (nReset)*/
//====================================================================

#define  BMLR_BMR1_RESERVED1_MASK 0xff000000
#define  BMLR_BMR1_RESERVED1_SHIFT 24
#define  BMLR_BMR1_RESERVED1_MASK_SHIFT(reg) (((reg) & BMLR_BMR1_RESERVED1_MASK) >> BMLR_BMR1_RESERVED1_SHIFT)
#define  BMLR_BMR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR1_RESERVED1_MASK) | (((uint32_t)val) << BMLR_BMR1_RESERVED1_SHIFT))

#define  BMLR_BMR1_AVERAGEC1_MASK 0xffffff
#define  BMLR_BMR1_AVERAGEC1_SHIFT 0
#define  BMLR_BMR1_AVERAGEC1_MASK_SHIFT(reg) (((reg) & BMLR_BMR1_AVERAGEC1_MASK) >> BMLR_BMR1_AVERAGEC1_SHIFT)
#define  BMLR_BMR1_AVERAGEC1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR1_AVERAGEC1_MASK) | (((uint32_t)val) << BMLR_BMR1_AVERAGEC1_SHIFT))

//====================================================================
//Register: Bulb Monitor Color2 Register (BMR2)
/** \brief BMR2 is cleared by BypassALL, SoftReset and Hard Reset (nReset)*/
//====================================================================

#define  BMLR_BMR2_RESERVED1_MASK 0xff000000
#define  BMLR_BMR2_RESERVED1_SHIFT 24
#define  BMLR_BMR2_RESERVED1_MASK_SHIFT(reg) (((reg) & BMLR_BMR2_RESERVED1_MASK) >> BMLR_BMR2_RESERVED1_SHIFT)
#define  BMLR_BMR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR2_RESERVED1_MASK) | (((uint32_t)val) << BMLR_BMR2_RESERVED1_SHIFT))

#define  BMLR_BMR2_AVERAGEC2_MASK 0xffffff
#define  BMLR_BMR2_AVERAGEC2_SHIFT 0
#define  BMLR_BMR2_AVERAGEC2_MASK_SHIFT(reg) (((reg) & BMLR_BMR2_AVERAGEC2_MASK) >> BMLR_BMR2_AVERAGEC2_SHIFT)
#define  BMLR_BMR2_AVERAGEC2_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR2_AVERAGEC2_MASK) | (((uint32_t)val) << BMLR_BMR2_AVERAGEC2_SHIFT))

//====================================================================
//Register: Bulb Monitor Exposure Register (BMR3)
/** \brief BMR2 is cleared by BypassALL, SoftReset and Hard Reset (nReset)*/
//====================================================================

#define  BMLR_BMR3_RESERVED1_MASK 0xfff00000
#define  BMLR_BMR3_RESERVED1_SHIFT 20
#define  BMLR_BMR3_RESERVED1_MASK_SHIFT(reg) (((reg) & BMLR_BMR3_RESERVED1_MASK) >> BMLR_BMR3_RESERVED1_SHIFT)
#define  BMLR_BMR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR3_RESERVED1_MASK) | (((uint32_t)val) << BMLR_BMR3_RESERVED1_SHIFT))

#define  BMLR_BMR3_ACCEXP_MASK 0xfffff
#define  BMLR_BMR3_ACCEXP_SHIFT 0
#define  BMLR_BMR3_ACCEXP_MASK_SHIFT(reg) (((reg) & BMLR_BMR3_ACCEXP_MASK) >> BMLR_BMR3_ACCEXP_SHIFT)
#define  BMLR_BMR3_ACCEXP_REPLACE_VAL(reg,val) (((reg) & ~BMLR_BMR3_ACCEXP_MASK) | (((uint32_t)val) << BMLR_BMR3_ACCEXP_SHIFT))

#endif // BMLR
