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
 * \file PIP_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIP_REGMASKS_H_
#define _PIP_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIP Top Level (PIP)
/** \brief Top-level register file for Print Image Pipeline*/
//
//====================================================================

//====================================================================
//Register: PIP Configuration Register (CFG)
/** \brief Register used to configure various aspects of the PIP block.*/
//====================================================================

#define  PIP_CFG_RESERVED1_MASK 0x80000000
#define  PIP_CFG_RESERVED1_SHIFT 31
#define  PIP_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_CFG_RESERVED1_MASK) >> PIP_CFG_RESERVED1_SHIFT)
#define  PIP_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_RESERVED1_MASK) | (((uint32_t)val) << PIP_CFG_RESERVED1_SHIFT))

#define  PIP_CFG_RM_MASK 0x70000000
#define  PIP_CFG_RM_SHIFT 28
#define  PIP_CFG_RM_MASK_SHIFT(reg) (((reg) & PIP_CFG_RM_MASK) >> PIP_CFG_RM_SHIFT)
#define  PIP_CFG_RM_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_RM_MASK) | (((uint32_t)val) << PIP_CFG_RM_SHIFT))

#define  PIP_CFG_RESERVED2_MASK 0xe000000
#define  PIP_CFG_RESERVED2_SHIFT 25
#define  PIP_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & PIP_CFG_RESERVED2_MASK) >> PIP_CFG_RESERVED2_SHIFT)
#define  PIP_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_RESERVED2_MASK) | (((uint32_t)val) << PIP_CFG_RESERVED2_SHIFT))

#define  PIP_CFG_IME_MASK 0x1ffff00
#define  PIP_CFG_IME_SHIFT 8
#define  PIP_CFG_IME_MASK_SHIFT(reg) (((reg) & PIP_CFG_IME_MASK) >> PIP_CFG_IME_SHIFT)
#define  PIP_CFG_IME_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_IME_MASK) | (((uint32_t)val) << PIP_CFG_IME_SHIFT))

#define  PIP_CFG_RESERVED3_MASK 0x80
#define  PIP_CFG_RESERVED3_SHIFT 7
#define  PIP_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & PIP_CFG_RESERVED3_MASK) >> PIP_CFG_RESERVED3_SHIFT)
#define  PIP_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_RESERVED3_MASK) | (((uint32_t)val) << PIP_CFG_RESERVED3_SHIFT))

#define  PIP_CFG_PNZ_MASK 0x40
#define  PIP_CFG_PNZ_SHIFT 6
#define  PIP_CFG_PNZ_MASK_SHIFT(reg) (((reg) & PIP_CFG_PNZ_MASK) >> PIP_CFG_PNZ_SHIFT)
#define  PIP_CFG_PNZ_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_PNZ_MASK) | (((uint32_t)val) << PIP_CFG_PNZ_SHIFT))

#define  PIP_CFG_LPCSB_MASK 0x20
#define  PIP_CFG_LPCSB_SHIFT 5
#define  PIP_CFG_LPCSB_MASK_SHIFT(reg) (((reg) & PIP_CFG_LPCSB_MASK) >> PIP_CFG_LPCSB_SHIFT)
#define  PIP_CFG_LPCSB_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_LPCSB_MASK) | (((uint32_t)val) << PIP_CFG_LPCSB_SHIFT))

#define  PIP_CFG_CRES_MASK 0x10
#define  PIP_CFG_CRES_SHIFT 4
#define  PIP_CFG_CRES_MASK_SHIFT(reg) (((reg) & PIP_CFG_CRES_MASK) >> PIP_CFG_CRES_SHIFT)
#define  PIP_CFG_CRES_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_CRES_MASK) | (((uint32_t)val) << PIP_CFG_CRES_SHIFT))

#define  PIP_CFG_MS_MASK 0xc
#define  PIP_CFG_MS_SHIFT 2
#define  PIP_CFG_MS_MASK_SHIFT(reg) (((reg) & PIP_CFG_MS_MASK) >> PIP_CFG_MS_SHIFT)
#define  PIP_CFG_MS_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_MS_MASK) | (((uint32_t)val) << PIP_CFG_MS_SHIFT))

#define  PIP_CFG_IE_MASK 0x2
#define  PIP_CFG_IE_SHIFT 1
#define  PIP_CFG_IE_MASK_SHIFT(reg) (((reg) & PIP_CFG_IE_MASK) >> PIP_CFG_IE_SHIFT)
#define  PIP_CFG_IE_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_IE_MASK) | (((uint32_t)val) << PIP_CFG_IE_SHIFT))

#define  PIP_CFG_SOFTRESET_MASK 0x1
#define  PIP_CFG_SOFTRESET_SHIFT 0
#define  PIP_CFG_SOFTRESET_MASK_SHIFT(reg) (((reg) & PIP_CFG_SOFTRESET_MASK) >> PIP_CFG_SOFTRESET_SHIFT)
#define  PIP_CFG_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~PIP_CFG_SOFTRESET_MASK) | (((uint32_t)val) << PIP_CFG_SOFTRESET_SHIFT))

//====================================================================
//Register: Output DMA Muxing Configuration Register (DMA_CFG)
//====================================================================

#define  PIP_DMA_CFG_DMA7_MODE_MASK 0xc0000000
#define  PIP_DMA_CFG_DMA7_MODE_SHIFT 30
#define  PIP_DMA_CFG_DMA7_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA7_MODE_MASK) >> PIP_DMA_CFG_DMA7_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA7_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA7_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA7_MODE_SHIFT))

#define  PIP_DMA_CFG_DMA6_MODE_MASK 0x30000000
#define  PIP_DMA_CFG_DMA6_MODE_SHIFT 28
#define  PIP_DMA_CFG_DMA6_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA6_MODE_MASK) >> PIP_DMA_CFG_DMA6_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA6_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA6_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA6_MODE_SHIFT))

#define  PIP_DMA_CFG_DMA5_MODE_MASK 0xc000000
#define  PIP_DMA_CFG_DMA5_MODE_SHIFT 26
#define  PIP_DMA_CFG_DMA5_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA5_MODE_MASK) >> PIP_DMA_CFG_DMA5_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA5_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA5_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA5_MODE_SHIFT))

#define  PIP_DMA_CFG_DMA4_MODE_MASK 0x3000000
#define  PIP_DMA_CFG_DMA4_MODE_SHIFT 24
#define  PIP_DMA_CFG_DMA4_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA4_MODE_MASK) >> PIP_DMA_CFG_DMA4_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA4_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA4_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA4_MODE_SHIFT))

#define  PIP_DMA_CFG_DMA3_MODE_MASK 0xc00000
#define  PIP_DMA_CFG_DMA3_MODE_SHIFT 22
#define  PIP_DMA_CFG_DMA3_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA3_MODE_MASK) >> PIP_DMA_CFG_DMA3_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA3_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA3_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA3_MODE_SHIFT))

#define  PIP_DMA_CFG_DMA2_MODE_MASK 0x300000
#define  PIP_DMA_CFG_DMA2_MODE_SHIFT 20
#define  PIP_DMA_CFG_DMA2_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA2_MODE_MASK) >> PIP_DMA_CFG_DMA2_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA2_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA2_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA2_MODE_SHIFT))

#define  PIP_DMA_CFG_DMA1_MODE_MASK 0xc0000
#define  PIP_DMA_CFG_DMA1_MODE_SHIFT 18
#define  PIP_DMA_CFG_DMA1_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA1_MODE_MASK) >> PIP_DMA_CFG_DMA1_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA1_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA1_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA1_MODE_SHIFT))

#define  PIP_DMA_CFG_DMA0_MODE_MASK 0x30000
#define  PIP_DMA_CFG_DMA0_MODE_SHIFT 16
#define  PIP_DMA_CFG_DMA0_MODE_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_DMA0_MODE_MASK) >> PIP_DMA_CFG_DMA0_MODE_SHIFT)
#define  PIP_DMA_CFG_DMA0_MODE_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_DMA0_MODE_MASK) | (((uint32_t)val) << PIP_DMA_CFG_DMA0_MODE_SHIFT))

#define  PIP_DMA_CFG_RESERVED1_MASK 0xfc00
#define  PIP_DMA_CFG_RESERVED1_SHIFT 10
#define  PIP_DMA_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_RESERVED1_MASK) >> PIP_DMA_CFG_RESERVED1_SHIFT)
#define  PIP_DMA_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_RESERVED1_MASK) | (((uint32_t)val) << PIP_DMA_CFG_RESERVED1_SHIFT))

#define  PIP_DMA_CFG_LPC3_EN_MASK 0x200
#define  PIP_DMA_CFG_LPC3_EN_SHIFT 9
#define  PIP_DMA_CFG_LPC3_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_LPC3_EN_MASK) >> PIP_DMA_CFG_LPC3_EN_SHIFT)
#define  PIP_DMA_CFG_LPC3_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_LPC3_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_LPC3_EN_SHIFT))

#define  PIP_DMA_CFG_LPC2_EN_MASK 0x100
#define  PIP_DMA_CFG_LPC2_EN_SHIFT 8
#define  PIP_DMA_CFG_LPC2_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_LPC2_EN_MASK) >> PIP_DMA_CFG_LPC2_EN_SHIFT)
#define  PIP_DMA_CFG_LPC2_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_LPC2_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_LPC2_EN_SHIFT))

#define  PIP_DMA_CFG_LPC1_EN_MASK 0x80
#define  PIP_DMA_CFG_LPC1_EN_SHIFT 7
#define  PIP_DMA_CFG_LPC1_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_LPC1_EN_MASK) >> PIP_DMA_CFG_LPC1_EN_SHIFT)
#define  PIP_DMA_CFG_LPC1_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_LPC1_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_LPC1_EN_SHIFT))

#define  PIP_DMA_CFG_LPC0_EN_MASK 0x40
#define  PIP_DMA_CFG_LPC0_EN_SHIFT 6
#define  PIP_DMA_CFG_LPC0_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_LPC0_EN_MASK) >> PIP_DMA_CFG_LPC0_EN_SHIFT)
#define  PIP_DMA_CFG_LPC0_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_LPC0_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_LPC0_EN_SHIFT))

#define  PIP_DMA_CFG_STAT5_EN_MASK 0x20
#define  PIP_DMA_CFG_STAT5_EN_SHIFT 5
#define  PIP_DMA_CFG_STAT5_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_STAT5_EN_MASK) >> PIP_DMA_CFG_STAT5_EN_SHIFT)
#define  PIP_DMA_CFG_STAT5_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_STAT5_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_STAT5_EN_SHIFT))

#define  PIP_DMA_CFG_STAT4_EN_MASK 0x10
#define  PIP_DMA_CFG_STAT4_EN_SHIFT 4
#define  PIP_DMA_CFG_STAT4_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_STAT4_EN_MASK) >> PIP_DMA_CFG_STAT4_EN_SHIFT)
#define  PIP_DMA_CFG_STAT4_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_STAT4_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_STAT4_EN_SHIFT))

#define  PIP_DMA_CFG_STAT3_EN_MASK 0x8
#define  PIP_DMA_CFG_STAT3_EN_SHIFT 3
#define  PIP_DMA_CFG_STAT3_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_STAT3_EN_MASK) >> PIP_DMA_CFG_STAT3_EN_SHIFT)
#define  PIP_DMA_CFG_STAT3_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_STAT3_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_STAT3_EN_SHIFT))

#define  PIP_DMA_CFG_STAT2_EN_MASK 0x4
#define  PIP_DMA_CFG_STAT2_EN_SHIFT 2
#define  PIP_DMA_CFG_STAT2_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_STAT2_EN_MASK) >> PIP_DMA_CFG_STAT2_EN_SHIFT)
#define  PIP_DMA_CFG_STAT2_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_STAT2_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_STAT2_EN_SHIFT))

#define  PIP_DMA_CFG_STAT1_EN_MASK 0x2
#define  PIP_DMA_CFG_STAT1_EN_SHIFT 1
#define  PIP_DMA_CFG_STAT1_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_STAT1_EN_MASK) >> PIP_DMA_CFG_STAT1_EN_SHIFT)
#define  PIP_DMA_CFG_STAT1_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_STAT1_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_STAT1_EN_SHIFT))

#define  PIP_DMA_CFG_STAT0_EN_MASK 0x1
#define  PIP_DMA_CFG_STAT0_EN_SHIFT 0
#define  PIP_DMA_CFG_STAT0_EN_MASK_SHIFT(reg) (((reg) & PIP_DMA_CFG_STAT0_EN_MASK) >> PIP_DMA_CFG_STAT0_EN_SHIFT)
#define  PIP_DMA_CFG_STAT0_EN_REPLACE_VAL(reg,val) (((reg) & ~PIP_DMA_CFG_STAT0_EN_MASK) | (((uint32_t)val) << PIP_DMA_CFG_STAT0_EN_SHIFT))

//====================================================================
//Register: PIP Interrupt Pending Register (IP)
//====================================================================

#define  PIP_IP_RESERVED1_MASK 0xfffe0000
#define  PIP_IP_RESERVED1_SHIFT 17
#define  PIP_IP_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_IP_RESERVED1_MASK) >> PIP_IP_RESERVED1_SHIFT)
#define  PIP_IP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_RESERVED1_MASK) | (((uint32_t)val) << PIP_IP_RESERVED1_SHIFT))

#define  PIP_IP_LPC3_IRQ_MASK 0x10000
#define  PIP_IP_LPC3_IRQ_SHIFT 16
#define  PIP_IP_LPC3_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_LPC3_IRQ_MASK) >> PIP_IP_LPC3_IRQ_SHIFT)
#define  PIP_IP_LPC3_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_LPC3_IRQ_MASK) | (((uint32_t)val) << PIP_IP_LPC3_IRQ_SHIFT))

#define  PIP_IP_LPC2_IRQ_MASK 0x8000
#define  PIP_IP_LPC2_IRQ_SHIFT 15
#define  PIP_IP_LPC2_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_LPC2_IRQ_MASK) >> PIP_IP_LPC2_IRQ_SHIFT)
#define  PIP_IP_LPC2_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_LPC2_IRQ_MASK) | (((uint32_t)val) << PIP_IP_LPC2_IRQ_SHIFT))

#define  PIP_IP_LPC1_IRQ_MASK 0x4000
#define  PIP_IP_LPC1_IRQ_SHIFT 14
#define  PIP_IP_LPC1_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_LPC1_IRQ_MASK) >> PIP_IP_LPC1_IRQ_SHIFT)
#define  PIP_IP_LPC1_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_LPC1_IRQ_MASK) | (((uint32_t)val) << PIP_IP_LPC1_IRQ_SHIFT))

#define  PIP_IP_LPC0_IRQ_MASK 0x2000
#define  PIP_IP_LPC0_IRQ_SHIFT 13
#define  PIP_IP_LPC0_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_LPC0_IRQ_MASK) >> PIP_IP_LPC0_IRQ_SHIFT)
#define  PIP_IP_LPC0_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_LPC0_IRQ_MASK) | (((uint32_t)val) << PIP_IP_LPC0_IRQ_SHIFT))

#define  PIP_IP_DAVBUFF_KMOD_IRQ_MASK 0x1000
#define  PIP_IP_DAVBUFF_KMOD_IRQ_SHIFT 12
#define  PIP_IP_DAVBUFF_KMOD_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_DAVBUFF_KMOD_IRQ_MASK) >> PIP_IP_DAVBUFF_KMOD_IRQ_SHIFT)
#define  PIP_IP_DAVBUFF_KMOD_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_DAVBUFF_KMOD_IRQ_MASK) | (((uint32_t)val) << PIP_IP_DAVBUFF_KMOD_IRQ_SHIFT))

#define  PIP_IP_DAVBUFF_SERP_IRQ_MASK 0x800
#define  PIP_IP_DAVBUFF_SERP_IRQ_SHIFT 11
#define  PIP_IP_DAVBUFF_SERP_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_DAVBUFF_SERP_IRQ_MASK) >> PIP_IP_DAVBUFF_SERP_IRQ_SHIFT)
#define  PIP_IP_DAVBUFF_SERP_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_DAVBUFF_SERP_IRQ_MASK) | (((uint32_t)val) << PIP_IP_DAVBUFF_SERP_IRQ_SHIFT))

#define  PIP_IP_DAVBUFF_UED_IRQ_MASK 0x400
#define  PIP_IP_DAVBUFF_UED_IRQ_SHIFT 10
#define  PIP_IP_DAVBUFF_UED_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_DAVBUFF_UED_IRQ_MASK) >> PIP_IP_DAVBUFF_UED_IRQ_SHIFT)
#define  PIP_IP_DAVBUFF_UED_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_DAVBUFF_UED_IRQ_MASK) | (((uint32_t)val) << PIP_IP_DAVBUFF_UED_IRQ_SHIFT))

#define  PIP_IP_FHS_IRQ_MASK 0x200
#define  PIP_IP_FHS_IRQ_SHIFT 9
#define  PIP_IP_FHS_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_FHS_IRQ_MASK) >> PIP_IP_FHS_IRQ_SHIFT)
#define  PIP_IP_FHS_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_FHS_IRQ_MASK) | (((uint32_t)val) << PIP_IP_FHS_IRQ_SHIFT))

#define  PIP_IP_PA_IRQ_MASK 0x100
#define  PIP_IP_PA_IRQ_SHIFT 8
#define  PIP_IP_PA_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_PA_IRQ_MASK) >> PIP_IP_PA_IRQ_SHIFT)
#define  PIP_IP_PA_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_PA_IRQ_MASK) | (((uint32_t)val) << PIP_IP_PA_IRQ_SHIFT))

#define  PIP_IP_ODMA7_IRQ_MASK 0x80
#define  PIP_IP_ODMA7_IRQ_SHIFT 7
#define  PIP_IP_ODMA7_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA7_IRQ_MASK) >> PIP_IP_ODMA7_IRQ_SHIFT)
#define  PIP_IP_ODMA7_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA7_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA7_IRQ_SHIFT))

#define  PIP_IP_ODMA6_IRQ_MASK 0x40
#define  PIP_IP_ODMA6_IRQ_SHIFT 6
#define  PIP_IP_ODMA6_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA6_IRQ_MASK) >> PIP_IP_ODMA6_IRQ_SHIFT)
#define  PIP_IP_ODMA6_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA6_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA6_IRQ_SHIFT))

#define  PIP_IP_ODMA5_IRQ_MASK 0x20
#define  PIP_IP_ODMA5_IRQ_SHIFT 5
#define  PIP_IP_ODMA5_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA5_IRQ_MASK) >> PIP_IP_ODMA5_IRQ_SHIFT)
#define  PIP_IP_ODMA5_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA5_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA5_IRQ_SHIFT))

#define  PIP_IP_ODMA4_IRQ_MASK 0x10
#define  PIP_IP_ODMA4_IRQ_SHIFT 4
#define  PIP_IP_ODMA4_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA4_IRQ_MASK) >> PIP_IP_ODMA4_IRQ_SHIFT)
#define  PIP_IP_ODMA4_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA4_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA4_IRQ_SHIFT))

#define  PIP_IP_ODMA3_IRQ_MASK 0x8
#define  PIP_IP_ODMA3_IRQ_SHIFT 3
#define  PIP_IP_ODMA3_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA3_IRQ_MASK) >> PIP_IP_ODMA3_IRQ_SHIFT)
#define  PIP_IP_ODMA3_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA3_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA3_IRQ_SHIFT))

#define  PIP_IP_ODMA2_IRQ_MASK 0x4
#define  PIP_IP_ODMA2_IRQ_SHIFT 2
#define  PIP_IP_ODMA2_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA2_IRQ_MASK) >> PIP_IP_ODMA2_IRQ_SHIFT)
#define  PIP_IP_ODMA2_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA2_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA2_IRQ_SHIFT))

#define  PIP_IP_ODMA1_IRQ_MASK 0x2
#define  PIP_IP_ODMA1_IRQ_SHIFT 1
#define  PIP_IP_ODMA1_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA1_IRQ_MASK) >> PIP_IP_ODMA1_IRQ_SHIFT)
#define  PIP_IP_ODMA1_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA1_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA1_IRQ_SHIFT))

#define  PIP_IP_ODMA0_IRQ_MASK 0x1
#define  PIP_IP_ODMA0_IRQ_SHIFT 0
#define  PIP_IP_ODMA0_IRQ_MASK_SHIFT(reg) (((reg) & PIP_IP_ODMA0_IRQ_MASK) >> PIP_IP_ODMA0_IRQ_SHIFT)
#define  PIP_IP_ODMA0_IRQ_REPLACE_VAL(reg,val) (((reg) & ~PIP_IP_ODMA0_IRQ_MASK) | (((uint32_t)val) << PIP_IP_ODMA0_IRQ_SHIFT))

//====================================================================
//Register: PIP Status Register (STAT)
//====================================================================

#define  PIP_STAT_RESERVED1_MASK 0xfffffff0
#define  PIP_STAT_RESERVED1_SHIFT 4
#define  PIP_STAT_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_STAT_RESERVED1_MASK) >> PIP_STAT_RESERVED1_SHIFT)
#define  PIP_STAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_STAT_RESERVED1_MASK) | (((uint32_t)val) << PIP_STAT_RESERVED1_SHIFT))

#define  PIP_STAT_DB_KMOD_SRDONE_MASK 0x8
#define  PIP_STAT_DB_KMOD_SRDONE_SHIFT 3
#define  PIP_STAT_DB_KMOD_SRDONE_MASK_SHIFT(reg) (((reg) & PIP_STAT_DB_KMOD_SRDONE_MASK) >> PIP_STAT_DB_KMOD_SRDONE_SHIFT)
#define  PIP_STAT_DB_KMOD_SRDONE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STAT_DB_KMOD_SRDONE_MASK) | (((uint32_t)val) << PIP_STAT_DB_KMOD_SRDONE_SHIFT))

#define  PIP_STAT_DB_SERP_SRDONE_MASK 0x4
#define  PIP_STAT_DB_SERP_SRDONE_SHIFT 2
#define  PIP_STAT_DB_SERP_SRDONE_MASK_SHIFT(reg) (((reg) & PIP_STAT_DB_SERP_SRDONE_MASK) >> PIP_STAT_DB_SERP_SRDONE_SHIFT)
#define  PIP_STAT_DB_SERP_SRDONE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STAT_DB_SERP_SRDONE_MASK) | (((uint32_t)val) << PIP_STAT_DB_SERP_SRDONE_SHIFT))

#define  PIP_STAT_DB_UED_SRDONE_MASK 0x2
#define  PIP_STAT_DB_UED_SRDONE_SHIFT 1
#define  PIP_STAT_DB_UED_SRDONE_MASK_SHIFT(reg) (((reg) & PIP_STAT_DB_UED_SRDONE_MASK) >> PIP_STAT_DB_UED_SRDONE_SHIFT)
#define  PIP_STAT_DB_UED_SRDONE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STAT_DB_UED_SRDONE_MASK) | (((uint32_t)val) << PIP_STAT_DB_UED_SRDONE_SHIFT))

#define  PIP_STAT_SCCSC_SRDONE_MASK 0x1
#define  PIP_STAT_SCCSC_SRDONE_SHIFT 0
#define  PIP_STAT_SCCSC_SRDONE_MASK_SHIFT(reg) (((reg) & PIP_STAT_SCCSC_SRDONE_MASK) >> PIP_STAT_SCCSC_SRDONE_SHIFT)
#define  PIP_STAT_SCCSC_SRDONE_REPLACE_VAL(reg,val) (((reg) & ~PIP_STAT_SCCSC_SRDONE_MASK) | (((uint32_t)val) << PIP_STAT_SCCSC_SRDONE_SHIFT))

//====================================================================
//Register: PIP Debug Register (DEBUG)
//====================================================================

#define  PIP_DEBUG_RESERVED1_MASK 0xfffffe00
#define  PIP_DEBUG_RESERVED1_SHIFT 9
#define  PIP_DEBUG_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_DEBUG_RESERVED1_MASK) >> PIP_DEBUG_RESERVED1_SHIFT)
#define  PIP_DEBUG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_DEBUG_RESERVED1_MASK) | (((uint32_t)val) << PIP_DEBUG_RESERVED1_SHIFT))

#define  PIP_DEBUG_BIG_END_MASK 0x100
#define  PIP_DEBUG_BIG_END_SHIFT 8
#define  PIP_DEBUG_BIG_END_MASK_SHIFT(reg) (((reg) & PIP_DEBUG_BIG_END_MASK) >> PIP_DEBUG_BIG_END_SHIFT)
#define  PIP_DEBUG_BIG_END_REPLACE_VAL(reg,val) (((reg) & ~PIP_DEBUG_BIG_END_MASK) | (((uint32_t)val) << PIP_DEBUG_BIG_END_SHIFT))

#define  PIP_DEBUG_CS_MASK 0xff
#define  PIP_DEBUG_CS_SHIFT 0
#define  PIP_DEBUG_CS_MASK_SHIFT(reg) (((reg) & PIP_DEBUG_CS_MASK) >> PIP_DEBUG_CS_SHIFT)
#define  PIP_DEBUG_CS_REPLACE_VAL(reg,val) (((reg) & ~PIP_DEBUG_CS_MASK) | (((uint32_t)val) << PIP_DEBUG_CS_SHIFT))

#endif // PIP
