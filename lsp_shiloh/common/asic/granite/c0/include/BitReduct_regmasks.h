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
 * \file BitReduct_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BITREDUCT_REGMASKS_H_
#define _BITREDUCT_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BitReduct (BitReduct)
/** \brief Register Descriptions for the PIC BitReduct block*/
//
//====================================================================

//====================================================================
//Register: BDR Configuration Register (BDRCR)
/** \brief General configuration register for BitReduct block.*/
//====================================================================

#define  BITREDUCT_BDRCR_RESERVED1_MASK 0xffffff80
#define  BITREDUCT_BDRCR_RESERVED1_SHIFT 7
#define  BITREDUCT_BDRCR_RESERVED1_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRCR_RESERVED1_MASK) >> BITREDUCT_BDRCR_RESERVED1_SHIFT)
#define  BITREDUCT_BDRCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRCR_RESERVED1_MASK) | (((uint32_t)val) << BITREDUCT_BDRCR_RESERVED1_SHIFT))

#define  BITREDUCT_BDRCR_NUMLUTS_MASK 0x40
#define  BITREDUCT_BDRCR_NUMLUTS_SHIFT 6
#define  BITREDUCT_BDRCR_NUMLUTS_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRCR_NUMLUTS_MASK) >> BITREDUCT_BDRCR_NUMLUTS_SHIFT)
#define  BITREDUCT_BDRCR_NUMLUTS_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRCR_NUMLUTS_MASK) | (((uint32_t)val) << BITREDUCT_BDRCR_NUMLUTS_SHIFT))

#define  BITREDUCT_BDRCR_LUTSEL_MASK 0x30
#define  BITREDUCT_BDRCR_LUTSEL_SHIFT 4
#define  BITREDUCT_BDRCR_LUTSEL_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRCR_LUTSEL_MASK) >> BITREDUCT_BDRCR_LUTSEL_SHIFT)
#define  BITREDUCT_BDRCR_LUTSEL_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRCR_LUTSEL_MASK) | (((uint32_t)val) << BITREDUCT_BDRCR_LUTSEL_SHIFT))

#define  BITREDUCT_BDRCR_RESERVED2_MASK 0xc
#define  BITREDUCT_BDRCR_RESERVED2_SHIFT 2
#define  BITREDUCT_BDRCR_RESERVED2_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRCR_RESERVED2_MASK) >> BITREDUCT_BDRCR_RESERVED2_SHIFT)
#define  BITREDUCT_BDRCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRCR_RESERVED2_MASK) | (((uint32_t)val) << BITREDUCT_BDRCR_RESERVED2_SHIFT))

#define  BITREDUCT_BDRCR_CPU_MASK 0x2
#define  BITREDUCT_BDRCR_CPU_SHIFT 1
#define  BITREDUCT_BDRCR_CPU_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRCR_CPU_MASK) >> BITREDUCT_BDRCR_CPU_SHIFT)
#define  BITREDUCT_BDRCR_CPU_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRCR_CPU_MASK) | (((uint32_t)val) << BITREDUCT_BDRCR_CPU_SHIFT))

#define  BITREDUCT_BDRCR_BYPASS_MASK 0x1
#define  BITREDUCT_BDRCR_BYPASS_SHIFT 0
#define  BITREDUCT_BDRCR_BYPASS_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRCR_BYPASS_MASK) >> BITREDUCT_BDRCR_BYPASS_SHIFT)
#define  BITREDUCT_BDRCR_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRCR_BYPASS_MASK) | (((uint32_t)val) << BITREDUCT_BDRCR_BYPASS_SHIFT))

//====================================================================
//Register: BDR Look Up Table (BDRLUT)
/** \brief The LUT, look up table, is used for the "LUT method" of bit depth reduction, and is implemented using a SRAM and a flop register as explained below. Firmware (processor) can read/write entries of the LUT SRAM/register only when the Enable CPU Access bit of the BDR Configuration Register (BDRCR.CPU bit) is set to 1. <br>
In bit depth reduction mode, hardware will access the LUT for two values: LUT(addr) and LUT(addr+1). The LUT size is 1025entries X 8bits, address input is 10bits, and data output is 8bits. The reason for 1025 entries is: 2^10=1024 entries for addr={0,1,...,1023}, and one additional entry for addr+1=1023+1=1024. The LUT implementation consists of <br>
"	SRAM: 1024entries X 8bits, dual-ported for concurrent lookup of LUT(addr) and LUT(addr+1) <br>
"	Flip-flop Register: 1entry x 8bits, specifically for lookup of (addr+1)=1024. <br>
Note that the processor view of the LUT is 1025 registers, word addressed. For example, the CPU address to LUT entry0 is Base+0x0000_0000, and the address to LUT entry1 is Base+0x0000_0004, and the address to LUT entry1024 is Base+0x0000_1000.*/
//====================================================================

#define  BITREDUCT_BDRLUT_RESERVED1_MASK 0xffffff00
#define  BITREDUCT_BDRLUT_RESERVED1_SHIFT 8
#define  BITREDUCT_BDRLUT_RESERVED1_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRLUT_RESERVED1_MASK) >> BITREDUCT_BDRLUT_RESERVED1_SHIFT)
#define  BITREDUCT_BDRLUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRLUT_RESERVED1_MASK) | (((uint32_t)val) << BITREDUCT_BDRLUT_RESERVED1_SHIFT))

#define  BITREDUCT_BDRLUT_VAL_MASK 0xff
#define  BITREDUCT_BDRLUT_VAL_SHIFT 0
#define  BITREDUCT_BDRLUT_VAL_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRLUT_VAL_MASK) >> BITREDUCT_BDRLUT_VAL_SHIFT)
#define  BITREDUCT_BDRLUT_VAL_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRLUT_VAL_MASK) | (((uint32_t)val) << BITREDUCT_BDRLUT_VAL_SHIFT))

//====================================================================
//Register: BDR Look Up Table Register (BDRLUTR)
/** \brief Flip-flop Register: 1entry x 8bits, specifically for lookup of (addr+1)=1024*/
//====================================================================

#define  BITREDUCT_BDRLUTR_RESERVED1_MASK 0xffffff00
#define  BITREDUCT_BDRLUTR_RESERVED1_SHIFT 8
#define  BITREDUCT_BDRLUTR_RESERVED1_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRLUTR_RESERVED1_MASK) >> BITREDUCT_BDRLUTR_RESERVED1_SHIFT)
#define  BITREDUCT_BDRLUTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRLUTR_RESERVED1_MASK) | (((uint32_t)val) << BITREDUCT_BDRLUTR_RESERVED1_SHIFT))

#define  BITREDUCT_BDRLUTR_VAL_MASK 0xff
#define  BITREDUCT_BDRLUTR_VAL_SHIFT 0
#define  BITREDUCT_BDRLUTR_VAL_MASK_SHIFT(reg) (((reg) & BITREDUCT_BDRLUTR_VAL_MASK) >> BITREDUCT_BDRLUTR_VAL_SHIFT)
#define  BITREDUCT_BDRLUTR_VAL_REPLACE_VAL(reg,val) (((reg) & ~BITREDUCT_BDRLUTR_VAL_MASK) | (((uint32_t)val) << BITREDUCT_BDRLUTR_VAL_SHIFT))

#endif // BITREDUCT
