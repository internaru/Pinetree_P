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
 * \file PIC_IntCtrl_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIC_INTCTRL_REGMASKS_H_
#define _PIC_INTCTRL_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIC_IntCtrl (PIC_IntCtrl)
/** \brief Register Descriptions for the PIC PIC_IntCtrl block*/
//
//====================================================================

//====================================================================
//Register: Interrupt Enable Register (IENR)
/** \brief Interrupt Enable Register (IENR)*/
//====================================================================

#define  PIC_INTCTRL_IENR_RESERVED1_MASK 0xffe00000
#define  PIC_INTCTRL_IENR_RESERVED1_SHIFT 21
#define  PIC_INTCTRL_IENR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_RESERVED1_MASK) >> PIC_INTCTRL_IENR_RESERVED1_SHIFT)
#define  PIC_INTCTRL_IENR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_RESERVED1_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_RESERVED1_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA11_MASK 0x100000
#define  PIC_INTCTRL_IENR_WDMA11_SHIFT 20
#define  PIC_INTCTRL_IENR_WDMA11_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA11_MASK) >> PIC_INTCTRL_IENR_WDMA11_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA11_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA11_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA11_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA10_MASK 0x80000
#define  PIC_INTCTRL_IENR_WDMA10_SHIFT 19
#define  PIC_INTCTRL_IENR_WDMA10_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA10_MASK) >> PIC_INTCTRL_IENR_WDMA10_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA10_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA10_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA10_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA9_MASK 0x40000
#define  PIC_INTCTRL_IENR_WDMA9_SHIFT 18
#define  PIC_INTCTRL_IENR_WDMA9_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA9_MASK) >> PIC_INTCTRL_IENR_WDMA9_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA9_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA9_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA9_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA8_MASK 0x20000
#define  PIC_INTCTRL_IENR_WDMA8_SHIFT 17
#define  PIC_INTCTRL_IENR_WDMA8_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA8_MASK) >> PIC_INTCTRL_IENR_WDMA8_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA8_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA8_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA8_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA7_MASK 0x10000
#define  PIC_INTCTRL_IENR_WDMA7_SHIFT 16
#define  PIC_INTCTRL_IENR_WDMA7_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA7_MASK) >> PIC_INTCTRL_IENR_WDMA7_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA7_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA7_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA7_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA6_MASK 0x8000
#define  PIC_INTCTRL_IENR_WDMA6_SHIFT 15
#define  PIC_INTCTRL_IENR_WDMA6_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA6_MASK) >> PIC_INTCTRL_IENR_WDMA6_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA6_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA6_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA6_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA5_MASK 0x4000
#define  PIC_INTCTRL_IENR_WDMA5_SHIFT 14
#define  PIC_INTCTRL_IENR_WDMA5_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA5_MASK) >> PIC_INTCTRL_IENR_WDMA5_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA5_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA5_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA5_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA4_MASK 0x2000
#define  PIC_INTCTRL_IENR_WDMA4_SHIFT 13
#define  PIC_INTCTRL_IENR_WDMA4_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA4_MASK) >> PIC_INTCTRL_IENR_WDMA4_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA4_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA4_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA4_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA3_MASK 0x1000
#define  PIC_INTCTRL_IENR_WDMA3_SHIFT 12
#define  PIC_INTCTRL_IENR_WDMA3_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA3_MASK) >> PIC_INTCTRL_IENR_WDMA3_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA3_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA3_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA3_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA2_MASK 0x800
#define  PIC_INTCTRL_IENR_WDMA2_SHIFT 11
#define  PIC_INTCTRL_IENR_WDMA2_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA2_MASK) >> PIC_INTCTRL_IENR_WDMA2_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA2_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA2_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA2_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA1_MASK 0x400
#define  PIC_INTCTRL_IENR_WDMA1_SHIFT 10
#define  PIC_INTCTRL_IENR_WDMA1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA1_MASK) >> PIC_INTCTRL_IENR_WDMA1_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA1_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA1_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA0_MASK 0x200
#define  PIC_INTCTRL_IENR_WDMA0_SHIFT 9
#define  PIC_INTCTRL_IENR_WDMA0_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA0_MASK) >> PIC_INTCTRL_IENR_WDMA0_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA0_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA0_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA0_SHIFT))

#define  PIC_INTCTRL_IENR_WDMA_COMMON_MASK 0x100
#define  PIC_INTCTRL_IENR_WDMA_COMMON_SHIFT 8
#define  PIC_INTCTRL_IENR_WDMA_COMMON_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_WDMA_COMMON_MASK) >> PIC_INTCTRL_IENR_WDMA_COMMON_SHIFT)
#define  PIC_INTCTRL_IENR_WDMA_COMMON_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_WDMA_COMMON_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_WDMA_COMMON_SHIFT))

#define  PIC_INTCTRL_IENR_RESERVED2_MASK 0xf8
#define  PIC_INTCTRL_IENR_RESERVED2_SHIFT 3
#define  PIC_INTCTRL_IENR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_RESERVED2_MASK) >> PIC_INTCTRL_IENR_RESERVED2_SHIFT)
#define  PIC_INTCTRL_IENR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_RESERVED2_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_RESERVED2_SHIFT))

#define  PIC_INTCTRL_IENR_IDMA_2D_CH1_MASK 0x4
#define  PIC_INTCTRL_IENR_IDMA_2D_CH1_SHIFT 2
#define  PIC_INTCTRL_IENR_IDMA_2D_CH1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_IDMA_2D_CH1_MASK) >> PIC_INTCTRL_IENR_IDMA_2D_CH1_SHIFT)
#define  PIC_INTCTRL_IENR_IDMA_2D_CH1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_IDMA_2D_CH1_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_IDMA_2D_CH1_SHIFT))

#define  PIC_INTCTRL_IENR_IDMA_2D_CH0_MASK 0x2
#define  PIC_INTCTRL_IENR_IDMA_2D_CH0_SHIFT 1
#define  PIC_INTCTRL_IENR_IDMA_2D_CH0_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_IDMA_2D_CH0_MASK) >> PIC_INTCTRL_IENR_IDMA_2D_CH0_SHIFT)
#define  PIC_INTCTRL_IENR_IDMA_2D_CH0_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_IDMA_2D_CH0_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_IDMA_2D_CH0_SHIFT))

#define  PIC_INTCTRL_IENR_OVERFLOW_MASK 0x1
#define  PIC_INTCTRL_IENR_OVERFLOW_SHIFT 0
#define  PIC_INTCTRL_IENR_OVERFLOW_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IENR_OVERFLOW_MASK) >> PIC_INTCTRL_IENR_OVERFLOW_SHIFT)
#define  PIC_INTCTRL_IENR_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IENR_OVERFLOW_MASK) | (((uint32_t)val) << PIC_INTCTRL_IENR_OVERFLOW_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (IPR) (IPR)
/** \brief Interrupt Pending Register (IPR)*/
//====================================================================

#define  PIC_INTCTRL_IPR_RESERVED1_MASK 0xffe00000
#define  PIC_INTCTRL_IPR_RESERVED1_SHIFT 21
#define  PIC_INTCTRL_IPR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_RESERVED1_MASK) >> PIC_INTCTRL_IPR_RESERVED1_SHIFT)
#define  PIC_INTCTRL_IPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_RESERVED1_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_RESERVED1_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA11_MASK 0x100000
#define  PIC_INTCTRL_IPR_WDMA11_SHIFT 20
#define  PIC_INTCTRL_IPR_WDMA11_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA11_MASK) >> PIC_INTCTRL_IPR_WDMA11_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA11_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA11_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA11_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA10_MASK 0x80000
#define  PIC_INTCTRL_IPR_WDMA10_SHIFT 19
#define  PIC_INTCTRL_IPR_WDMA10_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA10_MASK) >> PIC_INTCTRL_IPR_WDMA10_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA10_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA10_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA10_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA9_MASK 0x40000
#define  PIC_INTCTRL_IPR_WDMA9_SHIFT 18
#define  PIC_INTCTRL_IPR_WDMA9_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA9_MASK) >> PIC_INTCTRL_IPR_WDMA9_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA9_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA9_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA9_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA8_MASK 0x20000
#define  PIC_INTCTRL_IPR_WDMA8_SHIFT 17
#define  PIC_INTCTRL_IPR_WDMA8_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA8_MASK) >> PIC_INTCTRL_IPR_WDMA8_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA8_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA8_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA8_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA7_MASK 0x10000
#define  PIC_INTCTRL_IPR_WDMA7_SHIFT 16
#define  PIC_INTCTRL_IPR_WDMA7_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA7_MASK) >> PIC_INTCTRL_IPR_WDMA7_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA7_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA7_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA7_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA6_MASK 0x8000
#define  PIC_INTCTRL_IPR_WDMA6_SHIFT 15
#define  PIC_INTCTRL_IPR_WDMA6_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA6_MASK) >> PIC_INTCTRL_IPR_WDMA6_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA6_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA6_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA6_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA5_MASK 0x4000
#define  PIC_INTCTRL_IPR_WDMA5_SHIFT 14
#define  PIC_INTCTRL_IPR_WDMA5_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA5_MASK) >> PIC_INTCTRL_IPR_WDMA5_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA5_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA5_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA5_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA4_MASK 0x2000
#define  PIC_INTCTRL_IPR_WDMA4_SHIFT 13
#define  PIC_INTCTRL_IPR_WDMA4_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA4_MASK) >> PIC_INTCTRL_IPR_WDMA4_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA4_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA4_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA4_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA3_MASK 0x1000
#define  PIC_INTCTRL_IPR_WDMA3_SHIFT 12
#define  PIC_INTCTRL_IPR_WDMA3_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA3_MASK) >> PIC_INTCTRL_IPR_WDMA3_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA3_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA3_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA3_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA2_MASK 0x800
#define  PIC_INTCTRL_IPR_WDMA2_SHIFT 11
#define  PIC_INTCTRL_IPR_WDMA2_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA2_MASK) >> PIC_INTCTRL_IPR_WDMA2_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA2_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA2_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA2_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA1_MASK 0x400
#define  PIC_INTCTRL_IPR_WDMA1_SHIFT 10
#define  PIC_INTCTRL_IPR_WDMA1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA1_MASK) >> PIC_INTCTRL_IPR_WDMA1_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA1_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA1_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA0_MASK 0x200
#define  PIC_INTCTRL_IPR_WDMA0_SHIFT 9
#define  PIC_INTCTRL_IPR_WDMA0_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA0_MASK) >> PIC_INTCTRL_IPR_WDMA0_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA0_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA0_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA0_SHIFT))

#define  PIC_INTCTRL_IPR_WDMA_COMMON_MASK 0x100
#define  PIC_INTCTRL_IPR_WDMA_COMMON_SHIFT 8
#define  PIC_INTCTRL_IPR_WDMA_COMMON_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_WDMA_COMMON_MASK) >> PIC_INTCTRL_IPR_WDMA_COMMON_SHIFT)
#define  PIC_INTCTRL_IPR_WDMA_COMMON_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_WDMA_COMMON_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_WDMA_COMMON_SHIFT))

#define  PIC_INTCTRL_IPR_RESERVED2_MASK 0xf8
#define  PIC_INTCTRL_IPR_RESERVED2_SHIFT 3
#define  PIC_INTCTRL_IPR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_RESERVED2_MASK) >> PIC_INTCTRL_IPR_RESERVED2_SHIFT)
#define  PIC_INTCTRL_IPR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_RESERVED2_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_RESERVED2_SHIFT))

#define  PIC_INTCTRL_IPR_IDMA_2D_CH1_MASK 0x4
#define  PIC_INTCTRL_IPR_IDMA_2D_CH1_SHIFT 2
#define  PIC_INTCTRL_IPR_IDMA_2D_CH1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_IDMA_2D_CH1_MASK) >> PIC_INTCTRL_IPR_IDMA_2D_CH1_SHIFT)
#define  PIC_INTCTRL_IPR_IDMA_2D_CH1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_IDMA_2D_CH1_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_IDMA_2D_CH1_SHIFT))

#define  PIC_INTCTRL_IPR_IDMA_2D_CH0_MASK 0x2
#define  PIC_INTCTRL_IPR_IDMA_2D_CH0_SHIFT 1
#define  PIC_INTCTRL_IPR_IDMA_2D_CH0_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_IDMA_2D_CH0_MASK) >> PIC_INTCTRL_IPR_IDMA_2D_CH0_SHIFT)
#define  PIC_INTCTRL_IPR_IDMA_2D_CH0_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_IDMA_2D_CH0_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_IDMA_2D_CH0_SHIFT))

#define  PIC_INTCTRL_IPR_OVERFLOW_MASK 0x1
#define  PIC_INTCTRL_IPR_OVERFLOW_SHIFT 0
#define  PIC_INTCTRL_IPR_OVERFLOW_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_IPR_OVERFLOW_MASK) >> PIC_INTCTRL_IPR_OVERFLOW_SHIFT)
#define  PIC_INTCTRL_IPR_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_IPR_OVERFLOW_MASK) | (((uint32_t)val) << PIC_INTCTRL_IPR_OVERFLOW_SHIFT))

//====================================================================
//Register: Interrupt Test Register (ITR)
/** \brief Interrupt Test Register (IENR)*/
//====================================================================

#define  PIC_INTCTRL_ITR_RESERVED1_MASK 0xffe00000
#define  PIC_INTCTRL_ITR_RESERVED1_SHIFT 21
#define  PIC_INTCTRL_ITR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_RESERVED1_MASK) >> PIC_INTCTRL_ITR_RESERVED1_SHIFT)
#define  PIC_INTCTRL_ITR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_RESERVED1_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_RESERVED1_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA11_MASK 0x100000
#define  PIC_INTCTRL_ITR_WDMA11_SHIFT 20
#define  PIC_INTCTRL_ITR_WDMA11_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA11_MASK) >> PIC_INTCTRL_ITR_WDMA11_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA11_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA11_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA11_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA10_MASK 0x80000
#define  PIC_INTCTRL_ITR_WDMA10_SHIFT 19
#define  PIC_INTCTRL_ITR_WDMA10_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA10_MASK) >> PIC_INTCTRL_ITR_WDMA10_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA10_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA10_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA10_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA9_MASK 0x40000
#define  PIC_INTCTRL_ITR_WDMA9_SHIFT 18
#define  PIC_INTCTRL_ITR_WDMA9_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA9_MASK) >> PIC_INTCTRL_ITR_WDMA9_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA9_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA9_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA9_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA8_MASK 0x20000
#define  PIC_INTCTRL_ITR_WDMA8_SHIFT 17
#define  PIC_INTCTRL_ITR_WDMA8_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA8_MASK) >> PIC_INTCTRL_ITR_WDMA8_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA8_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA8_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA8_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA7_MASK 0x10000
#define  PIC_INTCTRL_ITR_WDMA7_SHIFT 16
#define  PIC_INTCTRL_ITR_WDMA7_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA7_MASK) >> PIC_INTCTRL_ITR_WDMA7_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA7_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA7_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA7_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA6_MASK 0x8000
#define  PIC_INTCTRL_ITR_WDMA6_SHIFT 15
#define  PIC_INTCTRL_ITR_WDMA6_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA6_MASK) >> PIC_INTCTRL_ITR_WDMA6_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA6_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA6_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA6_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA5_MASK 0x4000
#define  PIC_INTCTRL_ITR_WDMA5_SHIFT 14
#define  PIC_INTCTRL_ITR_WDMA5_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA5_MASK) >> PIC_INTCTRL_ITR_WDMA5_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA5_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA5_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA5_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA4_MASK 0x2000
#define  PIC_INTCTRL_ITR_WDMA4_SHIFT 13
#define  PIC_INTCTRL_ITR_WDMA4_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA4_MASK) >> PIC_INTCTRL_ITR_WDMA4_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA4_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA4_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA4_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA3_MASK 0x1000
#define  PIC_INTCTRL_ITR_WDMA3_SHIFT 12
#define  PIC_INTCTRL_ITR_WDMA3_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA3_MASK) >> PIC_INTCTRL_ITR_WDMA3_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA3_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA3_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA3_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA2_MASK 0x800
#define  PIC_INTCTRL_ITR_WDMA2_SHIFT 11
#define  PIC_INTCTRL_ITR_WDMA2_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA2_MASK) >> PIC_INTCTRL_ITR_WDMA2_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA2_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA2_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA2_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA1_MASK 0x400
#define  PIC_INTCTRL_ITR_WDMA1_SHIFT 10
#define  PIC_INTCTRL_ITR_WDMA1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA1_MASK) >> PIC_INTCTRL_ITR_WDMA1_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA1_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA1_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA0_MASK 0x200
#define  PIC_INTCTRL_ITR_WDMA0_SHIFT 9
#define  PIC_INTCTRL_ITR_WDMA0_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA0_MASK) >> PIC_INTCTRL_ITR_WDMA0_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA0_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA0_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA0_SHIFT))

#define  PIC_INTCTRL_ITR_WDMA_COMMON_MASK 0x100
#define  PIC_INTCTRL_ITR_WDMA_COMMON_SHIFT 8
#define  PIC_INTCTRL_ITR_WDMA_COMMON_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_WDMA_COMMON_MASK) >> PIC_INTCTRL_ITR_WDMA_COMMON_SHIFT)
#define  PIC_INTCTRL_ITR_WDMA_COMMON_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_WDMA_COMMON_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_WDMA_COMMON_SHIFT))

#define  PIC_INTCTRL_ITR_RESERVED2_MASK 0xf8
#define  PIC_INTCTRL_ITR_RESERVED2_SHIFT 3
#define  PIC_INTCTRL_ITR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_RESERVED2_MASK) >> PIC_INTCTRL_ITR_RESERVED2_SHIFT)
#define  PIC_INTCTRL_ITR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_RESERVED2_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_RESERVED2_SHIFT))

#define  PIC_INTCTRL_ITR_IDMA_2D_CH1_MASK 0x4
#define  PIC_INTCTRL_ITR_IDMA_2D_CH1_SHIFT 2
#define  PIC_INTCTRL_ITR_IDMA_2D_CH1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_IDMA_2D_CH1_MASK) >> PIC_INTCTRL_ITR_IDMA_2D_CH1_SHIFT)
#define  PIC_INTCTRL_ITR_IDMA_2D_CH1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_IDMA_2D_CH1_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_IDMA_2D_CH1_SHIFT))

#define  PIC_INTCTRL_ITR_IDMA_2D_CH0_MASK 0x2
#define  PIC_INTCTRL_ITR_IDMA_2D_CH0_SHIFT 1
#define  PIC_INTCTRL_ITR_IDMA_2D_CH0_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_IDMA_2D_CH0_MASK) >> PIC_INTCTRL_ITR_IDMA_2D_CH0_SHIFT)
#define  PIC_INTCTRL_ITR_IDMA_2D_CH0_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_IDMA_2D_CH0_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_IDMA_2D_CH0_SHIFT))

#define  PIC_INTCTRL_ITR_OVERFLOW_MASK 0x1
#define  PIC_INTCTRL_ITR_OVERFLOW_SHIFT 0
#define  PIC_INTCTRL_ITR_OVERFLOW_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_ITR_OVERFLOW_MASK) >> PIC_INTCTRL_ITR_OVERFLOW_SHIFT)
#define  PIC_INTCTRL_ITR_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_ITR_OVERFLOW_MASK) | (((uint32_t)val) << PIC_INTCTRL_ITR_OVERFLOW_SHIFT))

//====================================================================
//Register: PIC Control Register (PCR)
/** \brief PIC Control Register*/
//====================================================================

#define  PIC_INTCTRL_PCR_RESERVED1_MASK 0xfffffffc
#define  PIC_INTCTRL_PCR_RESERVED1_SHIFT 2
#define  PIC_INTCTRL_PCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_PCR_RESERVED1_MASK) >> PIC_INTCTRL_PCR_RESERVED1_SHIFT)
#define  PIC_INTCTRL_PCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_PCR_RESERVED1_MASK) | (((uint32_t)val) << PIC_INTCTRL_PCR_RESERVED1_SHIFT))

#define  PIC_INTCTRL_PCR_DUALCH_MASK 0x2
#define  PIC_INTCTRL_PCR_DUALCH_SHIFT 1
#define  PIC_INTCTRL_PCR_DUALCH_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_PCR_DUALCH_MASK) >> PIC_INTCTRL_PCR_DUALCH_SHIFT)
#define  PIC_INTCTRL_PCR_DUALCH_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_PCR_DUALCH_MASK) | (((uint32_t)val) << PIC_INTCTRL_PCR_DUALCH_SHIFT))

#define  PIC_INTCTRL_PCR_SOFTRESET_MASK 0x1
#define  PIC_INTCTRL_PCR_SOFTRESET_SHIFT 0
#define  PIC_INTCTRL_PCR_SOFTRESET_MASK_SHIFT(reg) (((reg) & PIC_INTCTRL_PCR_SOFTRESET_MASK) >> PIC_INTCTRL_PCR_SOFTRESET_SHIFT)
#define  PIC_INTCTRL_PCR_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~PIC_INTCTRL_PCR_SOFTRESET_MASK) | (((uint32_t)val) << PIC_INTCTRL_PCR_SOFTRESET_SHIFT))

#endif // PIC_INTCTRL
