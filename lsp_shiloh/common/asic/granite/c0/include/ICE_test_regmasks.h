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
 * \file ICE_test_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ICE_TEST_REGMASKS_H_
#define _ICE_TEST_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Register descriptions for ICE_test module (ICE_test)
/** \brief Register Descriptions for the ICE_test block*/
//
//====================================================================

//====================================================================
//Register: ICE Test Configuration Register (TestConfig)
/** \brief ICE Test Configuration Register*/
//====================================================================

#define  ICE_TEST_TESTCONFIG_RESERVED1_MASK 0xffff0000
#define  ICE_TEST_TESTCONFIG_RESERVED1_SHIFT 16
#define  ICE_TEST_TESTCONFIG_RESERVED1_MASK_SHIFT(reg) (((reg) & ICE_TEST_TESTCONFIG_RESERVED1_MASK) >> ICE_TEST_TESTCONFIG_RESERVED1_SHIFT)
#define  ICE_TEST_TESTCONFIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_TESTCONFIG_RESERVED1_MASK) | (((uint32_t)val) << ICE_TEST_TESTCONFIG_RESERVED1_SHIFT))

#define  ICE_TEST_TESTCONFIG_PICRATE_MASK 0xff00
#define  ICE_TEST_TESTCONFIG_PICRATE_SHIFT 8
#define  ICE_TEST_TESTCONFIG_PICRATE_MASK_SHIFT(reg) (((reg) & ICE_TEST_TESTCONFIG_PICRATE_MASK) >> ICE_TEST_TESTCONFIG_PICRATE_SHIFT)
#define  ICE_TEST_TESTCONFIG_PICRATE_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_TESTCONFIG_PICRATE_MASK) | (((uint32_t)val) << ICE_TEST_TESTCONFIG_PICRATE_SHIFT))

#define  ICE_TEST_TESTCONFIG_RESERVED2_MASK 0xf8
#define  ICE_TEST_TESTCONFIG_RESERVED2_SHIFT 3
#define  ICE_TEST_TESTCONFIG_RESERVED2_MASK_SHIFT(reg) (((reg) & ICE_TEST_TESTCONFIG_RESERVED2_MASK) >> ICE_TEST_TESTCONFIG_RESERVED2_SHIFT)
#define  ICE_TEST_TESTCONFIG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_TESTCONFIG_RESERVED2_MASK) | (((uint32_t)val) << ICE_TEST_TESTCONFIG_RESERVED2_SHIFT))

#define  ICE_TEST_TESTCONFIG_TESTSR_MASK 0x4
#define  ICE_TEST_TESTCONFIG_TESTSR_SHIFT 2
#define  ICE_TEST_TESTCONFIG_TESTSR_MASK_SHIFT(reg) (((reg) & ICE_TEST_TESTCONFIG_TESTSR_MASK) >> ICE_TEST_TESTCONFIG_TESTSR_SHIFT)
#define  ICE_TEST_TESTCONFIG_TESTSR_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_TESTCONFIG_TESTSR_MASK) | (((uint32_t)val) << ICE_TEST_TESTCONFIG_TESTSR_SHIFT))

#define  ICE_TEST_TESTCONFIG_TESTEN_MASK 0x2
#define  ICE_TEST_TESTCONFIG_TESTEN_SHIFT 1
#define  ICE_TEST_TESTCONFIG_TESTEN_MASK_SHIFT(reg) (((reg) & ICE_TEST_TESTCONFIG_TESTEN_MASK) >> ICE_TEST_TESTCONFIG_TESTEN_SHIFT)
#define  ICE_TEST_TESTCONFIG_TESTEN_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_TESTCONFIG_TESTEN_MASK) | (((uint32_t)val) << ICE_TEST_TESTCONFIG_TESTEN_SHIFT))

#define  ICE_TEST_TESTCONFIG_TESTSEL_MASK 0x1
#define  ICE_TEST_TESTCONFIG_TESTSEL_SHIFT 0
#define  ICE_TEST_TESTCONFIG_TESTSEL_MASK_SHIFT(reg) (((reg) & ICE_TEST_TESTCONFIG_TESTSEL_MASK) >> ICE_TEST_TESTCONFIG_TESTSEL_SHIFT)
#define  ICE_TEST_TESTCONFIG_TESTSEL_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_TESTCONFIG_TESTSEL_MASK) | (((uint32_t)val) << ICE_TEST_TESTCONFIG_TESTSEL_SHIFT))

//====================================================================
//Register: Interrupt Enable (IntEn)
/** \brief This register allows firmware to either enable or mask off the testmode underflow
        interrupt. The other interrupt source for ICE test is the IDMA block, and it's interrupt
        can be enabled or masked using the IDMA Interrupt Enable register.*/
//====================================================================

#define  ICE_TEST_INTEN_RESERVED1_MASK 0xfffffffe
#define  ICE_TEST_INTEN_RESERVED1_SHIFT 1
#define  ICE_TEST_INTEN_RESERVED1_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTEN_RESERVED1_MASK) >> ICE_TEST_INTEN_RESERVED1_SHIFT)
#define  ICE_TEST_INTEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTEN_RESERVED1_MASK) | (((uint32_t)val) << ICE_TEST_INTEN_RESERVED1_SHIFT))

#define  ICE_TEST_INTEN_UNDERINTEN_MASK 0x1
#define  ICE_TEST_INTEN_UNDERINTEN_SHIFT 0
#define  ICE_TEST_INTEN_UNDERINTEN_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTEN_UNDERINTEN_MASK) >> ICE_TEST_INTEN_UNDERINTEN_SHIFT)
#define  ICE_TEST_INTEN_UNDERINTEN_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTEN_UNDERINTEN_MASK) | (((uint32_t)val) << ICE_TEST_INTEN_UNDERINTEN_SHIFT))

//====================================================================
//Register: Interrupt Pending (IntPend)
/** \brief This register allows firmware to determine which of the two interrupt sources within
        the ICE test block needs servicing.*/
//====================================================================

#define  ICE_TEST_INTPEND_RESERVED1_MASK 0xfffffffc
#define  ICE_TEST_INTPEND_RESERVED1_SHIFT 2
#define  ICE_TEST_INTPEND_RESERVED1_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTPEND_RESERVED1_MASK) >> ICE_TEST_INTPEND_RESERVED1_SHIFT)
#define  ICE_TEST_INTPEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTPEND_RESERVED1_MASK) | (((uint32_t)val) << ICE_TEST_INTPEND_RESERVED1_SHIFT))

#define  ICE_TEST_INTPEND_DMAINTPEND_MASK 0x2
#define  ICE_TEST_INTPEND_DMAINTPEND_SHIFT 1
#define  ICE_TEST_INTPEND_DMAINTPEND_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTPEND_DMAINTPEND_MASK) >> ICE_TEST_INTPEND_DMAINTPEND_SHIFT)
#define  ICE_TEST_INTPEND_DMAINTPEND_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTPEND_DMAINTPEND_MASK) | (((uint32_t)val) << ICE_TEST_INTPEND_DMAINTPEND_SHIFT))

#define  ICE_TEST_INTPEND_UNDERINTPEND_MASK 0x1
#define  ICE_TEST_INTPEND_UNDERINTPEND_SHIFT 0
#define  ICE_TEST_INTPEND_UNDERINTPEND_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTPEND_UNDERINTPEND_MASK) >> ICE_TEST_INTPEND_UNDERINTPEND_SHIFT)
#define  ICE_TEST_INTPEND_UNDERINTPEND_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTPEND_UNDERINTPEND_MASK) | (((uint32_t)val) << ICE_TEST_INTPEND_UNDERINTPEND_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge (IntAck)
/** \brief This register allows firmware to acknowledge (clear) the ICE test underflow interrupt.
        Note that the IDMA interrupt is cleared using the IDMA interrupt acknowledge register.*/
//====================================================================

#define  ICE_TEST_INTACK_RESERVED1_MASK 0xfffffffe
#define  ICE_TEST_INTACK_RESERVED1_SHIFT 1
#define  ICE_TEST_INTACK_RESERVED1_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTACK_RESERVED1_MASK) >> ICE_TEST_INTACK_RESERVED1_SHIFT)
#define  ICE_TEST_INTACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTACK_RESERVED1_MASK) | (((uint32_t)val) << ICE_TEST_INTACK_RESERVED1_SHIFT))

#define  ICE_TEST_INTACK_UNDERINTACK_MASK 0x1
#define  ICE_TEST_INTACK_UNDERINTACK_SHIFT 0
#define  ICE_TEST_INTACK_UNDERINTACK_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTACK_UNDERINTACK_MASK) >> ICE_TEST_INTACK_UNDERINTACK_SHIFT)
#define  ICE_TEST_INTACK_UNDERINTACK_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTACK_UNDERINTACK_MASK) | (((uint32_t)val) << ICE_TEST_INTACK_UNDERINTACK_SHIFT))

//====================================================================
//Register: Interrupt Force (IntForce)
/** \brief This register allows firmware to set the ICE test underflow interrupt.
        Note that the IDMA interrupt sources can be forced using the IDMA interrupt force register.*/
//====================================================================

#define  ICE_TEST_INTFORCE_RESERVED1_MASK 0xfffffffe
#define  ICE_TEST_INTFORCE_RESERVED1_SHIFT 1
#define  ICE_TEST_INTFORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTFORCE_RESERVED1_MASK) >> ICE_TEST_INTFORCE_RESERVED1_SHIFT)
#define  ICE_TEST_INTFORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTFORCE_RESERVED1_MASK) | (((uint32_t)val) << ICE_TEST_INTFORCE_RESERVED1_SHIFT))

#define  ICE_TEST_INTFORCE_UNDERINTFORCE_MASK 0x1
#define  ICE_TEST_INTFORCE_UNDERINTFORCE_SHIFT 0
#define  ICE_TEST_INTFORCE_UNDERINTFORCE_MASK_SHIFT(reg) (((reg) & ICE_TEST_INTFORCE_UNDERINTFORCE_MASK) >> ICE_TEST_INTFORCE_UNDERINTFORCE_SHIFT)
#define  ICE_TEST_INTFORCE_UNDERINTFORCE_REPLACE_VAL(reg,val) (((reg) & ~ICE_TEST_INTFORCE_UNDERINTFORCE_MASK) | (((uint32_t)val) << ICE_TEST_INTFORCE_UNDERINTFORCE_SHIFT))

//====================================================================
//
//Register File: ICE Test DMA register set (TestIDMA)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  TESTIDMA_CFG_RESERVED1_MASK 0xfe000000
#define  TESTIDMA_CFG_RESERVED1_SHIFT 25
#define  TESTIDMA_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_RESERVED1_MASK) >> TESTIDMA_CFG_RESERVED1_SHIFT)
#define  TESTIDMA_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_CFG_RESERVED1_SHIFT))

#define  TESTIDMA_CFG_REPLICATE_MASK 0x1000000
#define  TESTIDMA_CFG_REPLICATE_SHIFT 24
#define  TESTIDMA_CFG_REPLICATE_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_REPLICATE_MASK) >> TESTIDMA_CFG_REPLICATE_SHIFT)
#define  TESTIDMA_CFG_REPLICATE_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_REPLICATE_MASK) | (((uint32_t)val) << TESTIDMA_CFG_REPLICATE_SHIFT))

#define  TESTIDMA_CFG_RESERVED2_MASK 0xfe0000
#define  TESTIDMA_CFG_RESERVED2_SHIFT 17
#define  TESTIDMA_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_RESERVED2_MASK) >> TESTIDMA_CFG_RESERVED2_SHIFT)
#define  TESTIDMA_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_RESERVED2_MASK) | (((uint32_t)val) << TESTIDMA_CFG_RESERVED2_SHIFT))

#define  TESTIDMA_CFG_ENABLE_MASK 0x10000
#define  TESTIDMA_CFG_ENABLE_SHIFT 16
#define  TESTIDMA_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_ENABLE_MASK) >> TESTIDMA_CFG_ENABLE_SHIFT)
#define  TESTIDMA_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_ENABLE_MASK) | (((uint32_t)val) << TESTIDMA_CFG_ENABLE_SHIFT))

#define  TESTIDMA_CFG_RESERVED3_MASK 0xf800
#define  TESTIDMA_CFG_RESERVED3_SHIFT 11
#define  TESTIDMA_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_RESERVED3_MASK) >> TESTIDMA_CFG_RESERVED3_SHIFT)
#define  TESTIDMA_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_RESERVED3_MASK) | (((uint32_t)val) << TESTIDMA_CFG_RESERVED3_SHIFT))

#define  TESTIDMA_CFG_OUT_WIDTH_MASK 0x700
#define  TESTIDMA_CFG_OUT_WIDTH_SHIFT 8
#define  TESTIDMA_CFG_OUT_WIDTH_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_OUT_WIDTH_MASK) >> TESTIDMA_CFG_OUT_WIDTH_SHIFT)
#define  TESTIDMA_CFG_OUT_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_OUT_WIDTH_MASK) | (((uint32_t)val) << TESTIDMA_CFG_OUT_WIDTH_SHIFT))

#define  TESTIDMA_CFG_RESERVED4_MASK 0xfc
#define  TESTIDMA_CFG_RESERVED4_SHIFT 2
#define  TESTIDMA_CFG_RESERVED4_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_RESERVED4_MASK) >> TESTIDMA_CFG_RESERVED4_SHIFT)
#define  TESTIDMA_CFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_RESERVED4_MASK) | (((uint32_t)val) << TESTIDMA_CFG_RESERVED4_SHIFT))

#define  TESTIDMA_CFG_BURST_LEN_MASK 0x3
#define  TESTIDMA_CFG_BURST_LEN_SHIFT 0
#define  TESTIDMA_CFG_BURST_LEN_MASK_SHIFT(reg) (((reg) & TESTIDMA_CFG_BURST_LEN_MASK) >> TESTIDMA_CFG_BURST_LEN_SHIFT)
#define  TESTIDMA_CFG_BURST_LEN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CFG_BURST_LEN_MASK) | (((uint32_t)val) << TESTIDMA_CFG_BURST_LEN_SHIFT))

//====================================================================
//Register: Status register (status)
//====================================================================

#define  TESTIDMA_STATUS_RESERVED1_MASK 0xfffffe00
#define  TESTIDMA_STATUS_RESERVED1_SHIFT 9
#define  TESTIDMA_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_RESERVED1_MASK) >> TESTIDMA_STATUS_RESERVED1_SHIFT)
#define  TESTIDMA_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_RESERVED1_SHIFT))

#define  TESTIDMA_STATUS_SOI_MASK 0x100
#define  TESTIDMA_STATUS_SOI_SHIFT 8
#define  TESTIDMA_STATUS_SOI_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_SOI_MASK) >> TESTIDMA_STATUS_SOI_SHIFT)
#define  TESTIDMA_STATUS_SOI_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_SOI_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_SOI_SHIFT))

#define  TESTIDMA_STATUS_EOI_MASK 0x80
#define  TESTIDMA_STATUS_EOI_SHIFT 7
#define  TESTIDMA_STATUS_EOI_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_EOI_MASK) >> TESTIDMA_STATUS_EOI_SHIFT)
#define  TESTIDMA_STATUS_EOI_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_EOI_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_EOI_SHIFT))

#define  TESTIDMA_STATUS_SOFTRESET_MASK 0x40
#define  TESTIDMA_STATUS_SOFTRESET_SHIFT 6
#define  TESTIDMA_STATUS_SOFTRESET_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_SOFTRESET_MASK) >> TESTIDMA_STATUS_SOFTRESET_SHIFT)
#define  TESTIDMA_STATUS_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_SOFTRESET_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_SOFTRESET_SHIFT))

#define  TESTIDMA_STATUS_FULL_CBUF_MASK 0x20
#define  TESTIDMA_STATUS_FULL_CBUF_SHIFT 5
#define  TESTIDMA_STATUS_FULL_CBUF_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_FULL_CBUF_MASK) >> TESTIDMA_STATUS_FULL_CBUF_SHIFT)
#define  TESTIDMA_STATUS_FULL_CBUF_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_FULL_CBUF_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_FULL_CBUF_SHIFT))

#define  TESTIDMA_STATUS_EMPTY_CBUF_MASK 0x10
#define  TESTIDMA_STATUS_EMPTY_CBUF_SHIFT 4
#define  TESTIDMA_STATUS_EMPTY_CBUF_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_EMPTY_CBUF_MASK) >> TESTIDMA_STATUS_EMPTY_CBUF_SHIFT)
#define  TESTIDMA_STATUS_EMPTY_CBUF_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_EMPTY_CBUF_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_EMPTY_CBUF_SHIFT))

#define  TESTIDMA_STATUS_FULL_DBUF_MASK 0x8
#define  TESTIDMA_STATUS_FULL_DBUF_SHIFT 3
#define  TESTIDMA_STATUS_FULL_DBUF_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_FULL_DBUF_MASK) >> TESTIDMA_STATUS_FULL_DBUF_SHIFT)
#define  TESTIDMA_STATUS_FULL_DBUF_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_FULL_DBUF_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_FULL_DBUF_SHIFT))

#define  TESTIDMA_STATUS_EMPTY_DBUF_MASK 0x4
#define  TESTIDMA_STATUS_EMPTY_DBUF_SHIFT 2
#define  TESTIDMA_STATUS_EMPTY_DBUF_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_EMPTY_DBUF_MASK) >> TESTIDMA_STATUS_EMPTY_DBUF_SHIFT)
#define  TESTIDMA_STATUS_EMPTY_DBUF_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_EMPTY_DBUF_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_EMPTY_DBUF_SHIFT))

#define  TESTIDMA_STATUS_PACKER_EMPTY_MASK 0x2
#define  TESTIDMA_STATUS_PACKER_EMPTY_SHIFT 1
#define  TESTIDMA_STATUS_PACKER_EMPTY_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_PACKER_EMPTY_MASK) >> TESTIDMA_STATUS_PACKER_EMPTY_SHIFT)
#define  TESTIDMA_STATUS_PACKER_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_PACKER_EMPTY_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_PACKER_EMPTY_SHIFT))

#define  TESTIDMA_STATUS_DMA_BUSY_MASK 0x1
#define  TESTIDMA_STATUS_DMA_BUSY_SHIFT 0
#define  TESTIDMA_STATUS_DMA_BUSY_MASK_SHIFT(reg) (((reg) & TESTIDMA_STATUS_DMA_BUSY_MASK) >> TESTIDMA_STATUS_DMA_BUSY_SHIFT)
#define  TESTIDMA_STATUS_DMA_BUSY_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_STATUS_DMA_BUSY_MASK) | (((uint32_t)val) << TESTIDMA_STATUS_DMA_BUSY_SHIFT))

//====================================================================
//Register: Line Width register (line_width)
//====================================================================

#define  TESTIDMA_LINE_WIDTH_RESERVED1_MASK 0xffff0000
#define  TESTIDMA_LINE_WIDTH_RESERVED1_SHIFT 16
#define  TESTIDMA_LINE_WIDTH_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_LINE_WIDTH_RESERVED1_MASK) >> TESTIDMA_LINE_WIDTH_RESERVED1_SHIFT)
#define  TESTIDMA_LINE_WIDTH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_LINE_WIDTH_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_LINE_WIDTH_RESERVED1_SHIFT))

#define  TESTIDMA_LINE_WIDTH_LINE_WIDTH_MASK 0xffff
#define  TESTIDMA_LINE_WIDTH_LINE_WIDTH_SHIFT 0
#define  TESTIDMA_LINE_WIDTH_LINE_WIDTH_MASK_SHIFT(reg) (((reg) & TESTIDMA_LINE_WIDTH_LINE_WIDTH_MASK) >> TESTIDMA_LINE_WIDTH_LINE_WIDTH_SHIFT)
#define  TESTIDMA_LINE_WIDTH_LINE_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_LINE_WIDTH_LINE_WIDTH_MASK) | (((uint32_t)val) << TESTIDMA_LINE_WIDTH_LINE_WIDTH_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  TESTIDMA_INT_EN_RESERVED1_MASK 0xffffffc0
#define  TESTIDMA_INT_EN_RESERVED1_SHIFT 6
#define  TESTIDMA_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_EN_RESERVED1_MASK) >> TESTIDMA_INT_EN_RESERVED1_SHIFT)
#define  TESTIDMA_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_INT_EN_RESERVED1_SHIFT))

#define  TESTIDMA_INT_EN_RRESP_INT_EN_MASK 0x20
#define  TESTIDMA_INT_EN_RRESP_INT_EN_SHIFT 5
#define  TESTIDMA_INT_EN_RRESP_INT_EN_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_EN_RRESP_INT_EN_MASK) >> TESTIDMA_INT_EN_RRESP_INT_EN_SHIFT)
#define  TESTIDMA_INT_EN_RRESP_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_EN_RRESP_INT_EN_MASK) | (((uint32_t)val) << TESTIDMA_INT_EN_RRESP_INT_EN_SHIFT))

#define  TESTIDMA_INT_EN_BRESP_INT_EN_MASK 0x10
#define  TESTIDMA_INT_EN_BRESP_INT_EN_SHIFT 4
#define  TESTIDMA_INT_EN_BRESP_INT_EN_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_EN_BRESP_INT_EN_MASK) >> TESTIDMA_INT_EN_BRESP_INT_EN_SHIFT)
#define  TESTIDMA_INT_EN_BRESP_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_EN_BRESP_INT_EN_MASK) | (((uint32_t)val) << TESTIDMA_INT_EN_BRESP_INT_EN_SHIFT))

#define  TESTIDMA_INT_EN_RST_INT_EN_MASK 0x8
#define  TESTIDMA_INT_EN_RST_INT_EN_SHIFT 3
#define  TESTIDMA_INT_EN_RST_INT_EN_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_EN_RST_INT_EN_MASK) >> TESTIDMA_INT_EN_RST_INT_EN_SHIFT)
#define  TESTIDMA_INT_EN_RST_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_EN_RST_INT_EN_MASK) | (((uint32_t)val) << TESTIDMA_INT_EN_RST_INT_EN_SHIFT))

#define  TESTIDMA_INT_EN_EOI_INT_EN_MASK 0x4
#define  TESTIDMA_INT_EN_EOI_INT_EN_SHIFT 2
#define  TESTIDMA_INT_EN_EOI_INT_EN_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_EN_EOI_INT_EN_MASK) >> TESTIDMA_INT_EN_EOI_INT_EN_SHIFT)
#define  TESTIDMA_INT_EN_EOI_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_EN_EOI_INT_EN_MASK) | (((uint32_t)val) << TESTIDMA_INT_EN_EOI_INT_EN_SHIFT))

#define  TESTIDMA_INT_EN_FIN_INT_EN_MASK 0x2
#define  TESTIDMA_INT_EN_FIN_INT_EN_SHIFT 1
#define  TESTIDMA_INT_EN_FIN_INT_EN_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_EN_FIN_INT_EN_MASK) >> TESTIDMA_INT_EN_FIN_INT_EN_SHIFT)
#define  TESTIDMA_INT_EN_FIN_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_EN_FIN_INT_EN_MASK) | (((uint32_t)val) << TESTIDMA_INT_EN_FIN_INT_EN_SHIFT))

#define  TESTIDMA_INT_EN_WHO_INT_EN_MASK 0x1
#define  TESTIDMA_INT_EN_WHO_INT_EN_SHIFT 0
#define  TESTIDMA_INT_EN_WHO_INT_EN_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_EN_WHO_INT_EN_MASK) >> TESTIDMA_INT_EN_WHO_INT_EN_SHIFT)
#define  TESTIDMA_INT_EN_WHO_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_EN_WHO_INT_EN_MASK) | (((uint32_t)val) << TESTIDMA_INT_EN_WHO_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  TESTIDMA_INT_ST_RESERVED1_MASK 0xffffffc0
#define  TESTIDMA_INT_ST_RESERVED1_SHIFT 6
#define  TESTIDMA_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_ST_RESERVED1_MASK) >> TESTIDMA_INT_ST_RESERVED1_SHIFT)
#define  TESTIDMA_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_INT_ST_RESERVED1_SHIFT))

#define  TESTIDMA_INT_ST_RRESP_INT_ST_MASK 0x20
#define  TESTIDMA_INT_ST_RRESP_INT_ST_SHIFT 5
#define  TESTIDMA_INT_ST_RRESP_INT_ST_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_ST_RRESP_INT_ST_MASK) >> TESTIDMA_INT_ST_RRESP_INT_ST_SHIFT)
#define  TESTIDMA_INT_ST_RRESP_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_ST_RRESP_INT_ST_MASK) | (((uint32_t)val) << TESTIDMA_INT_ST_RRESP_INT_ST_SHIFT))

#define  TESTIDMA_INT_ST_BRESP_INT_ST_MASK 0x10
#define  TESTIDMA_INT_ST_BRESP_INT_ST_SHIFT 4
#define  TESTIDMA_INT_ST_BRESP_INT_ST_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_ST_BRESP_INT_ST_MASK) >> TESTIDMA_INT_ST_BRESP_INT_ST_SHIFT)
#define  TESTIDMA_INT_ST_BRESP_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_ST_BRESP_INT_ST_MASK) | (((uint32_t)val) << TESTIDMA_INT_ST_BRESP_INT_ST_SHIFT))

#define  TESTIDMA_INT_ST_RST_INT_ST_MASK 0x8
#define  TESTIDMA_INT_ST_RST_INT_ST_SHIFT 3
#define  TESTIDMA_INT_ST_RST_INT_ST_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_ST_RST_INT_ST_MASK) >> TESTIDMA_INT_ST_RST_INT_ST_SHIFT)
#define  TESTIDMA_INT_ST_RST_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_ST_RST_INT_ST_MASK) | (((uint32_t)val) << TESTIDMA_INT_ST_RST_INT_ST_SHIFT))

#define  TESTIDMA_INT_ST_EOI_INT_ST_MASK 0x4
#define  TESTIDMA_INT_ST_EOI_INT_ST_SHIFT 2
#define  TESTIDMA_INT_ST_EOI_INT_ST_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_ST_EOI_INT_ST_MASK) >> TESTIDMA_INT_ST_EOI_INT_ST_SHIFT)
#define  TESTIDMA_INT_ST_EOI_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_ST_EOI_INT_ST_MASK) | (((uint32_t)val) << TESTIDMA_INT_ST_EOI_INT_ST_SHIFT))

#define  TESTIDMA_INT_ST_FIN_INT_ST_MASK 0x2
#define  TESTIDMA_INT_ST_FIN_INT_ST_SHIFT 1
#define  TESTIDMA_INT_ST_FIN_INT_ST_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_ST_FIN_INT_ST_MASK) >> TESTIDMA_INT_ST_FIN_INT_ST_SHIFT)
#define  TESTIDMA_INT_ST_FIN_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_ST_FIN_INT_ST_MASK) | (((uint32_t)val) << TESTIDMA_INT_ST_FIN_INT_ST_SHIFT))

#define  TESTIDMA_INT_ST_WHO_INT_ST_MASK 0x1
#define  TESTIDMA_INT_ST_WHO_INT_ST_SHIFT 0
#define  TESTIDMA_INT_ST_WHO_INT_ST_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_ST_WHO_INT_ST_MASK) >> TESTIDMA_INT_ST_WHO_INT_ST_SHIFT)
#define  TESTIDMA_INT_ST_WHO_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_ST_WHO_INT_ST_MASK) | (((uint32_t)val) << TESTIDMA_INT_ST_WHO_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Clear Register (int_cl)
//====================================================================

#define  TESTIDMA_INT_CL_RESERVED1_MASK 0xffffffc0
#define  TESTIDMA_INT_CL_RESERVED1_SHIFT 6
#define  TESTIDMA_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_CL_RESERVED1_MASK) >> TESTIDMA_INT_CL_RESERVED1_SHIFT)
#define  TESTIDMA_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_INT_CL_RESERVED1_SHIFT))

#define  TESTIDMA_INT_CL_RRESP_INT_CL_MASK 0x20
#define  TESTIDMA_INT_CL_RRESP_INT_CL_SHIFT 5
#define  TESTIDMA_INT_CL_RRESP_INT_CL_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_CL_RRESP_INT_CL_MASK) >> TESTIDMA_INT_CL_RRESP_INT_CL_SHIFT)
#define  TESTIDMA_INT_CL_RRESP_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_CL_RRESP_INT_CL_MASK) | (((uint32_t)val) << TESTIDMA_INT_CL_RRESP_INT_CL_SHIFT))

#define  TESTIDMA_INT_CL_BRESP_INT_CL_MASK 0x10
#define  TESTIDMA_INT_CL_BRESP_INT_CL_SHIFT 4
#define  TESTIDMA_INT_CL_BRESP_INT_CL_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_CL_BRESP_INT_CL_MASK) >> TESTIDMA_INT_CL_BRESP_INT_CL_SHIFT)
#define  TESTIDMA_INT_CL_BRESP_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_CL_BRESP_INT_CL_MASK) | (((uint32_t)val) << TESTIDMA_INT_CL_BRESP_INT_CL_SHIFT))

#define  TESTIDMA_INT_CL_RST_INT_CL_MASK 0x8
#define  TESTIDMA_INT_CL_RST_INT_CL_SHIFT 3
#define  TESTIDMA_INT_CL_RST_INT_CL_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_CL_RST_INT_CL_MASK) >> TESTIDMA_INT_CL_RST_INT_CL_SHIFT)
#define  TESTIDMA_INT_CL_RST_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_CL_RST_INT_CL_MASK) | (((uint32_t)val) << TESTIDMA_INT_CL_RST_INT_CL_SHIFT))

#define  TESTIDMA_INT_CL_EOI_INT_CL_MASK 0x4
#define  TESTIDMA_INT_CL_EOI_INT_CL_SHIFT 2
#define  TESTIDMA_INT_CL_EOI_INT_CL_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_CL_EOI_INT_CL_MASK) >> TESTIDMA_INT_CL_EOI_INT_CL_SHIFT)
#define  TESTIDMA_INT_CL_EOI_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_CL_EOI_INT_CL_MASK) | (((uint32_t)val) << TESTIDMA_INT_CL_EOI_INT_CL_SHIFT))

#define  TESTIDMA_INT_CL_FIN_INT_CL_MASK 0x2
#define  TESTIDMA_INT_CL_FIN_INT_CL_SHIFT 1
#define  TESTIDMA_INT_CL_FIN_INT_CL_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_CL_FIN_INT_CL_MASK) >> TESTIDMA_INT_CL_FIN_INT_CL_SHIFT)
#define  TESTIDMA_INT_CL_FIN_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_CL_FIN_INT_CL_MASK) | (((uint32_t)val) << TESTIDMA_INT_CL_FIN_INT_CL_SHIFT))

#define  TESTIDMA_INT_CL_WHO_INT_CL_MASK 0x1
#define  TESTIDMA_INT_CL_WHO_INT_CL_SHIFT 0
#define  TESTIDMA_INT_CL_WHO_INT_CL_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_CL_WHO_INT_CL_MASK) >> TESTIDMA_INT_CL_WHO_INT_CL_SHIFT)
#define  TESTIDMA_INT_CL_WHO_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_CL_WHO_INT_CL_MASK) | (((uint32_t)val) << TESTIDMA_INT_CL_WHO_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  TESTIDMA_INT_FO_RESERVED1_MASK 0xffffffc0
#define  TESTIDMA_INT_FO_RESERVED1_SHIFT 6
#define  TESTIDMA_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_FO_RESERVED1_MASK) >> TESTIDMA_INT_FO_RESERVED1_SHIFT)
#define  TESTIDMA_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_INT_FO_RESERVED1_SHIFT))

#define  TESTIDMA_INT_FO_RRESP_INT_FO_MASK 0x20
#define  TESTIDMA_INT_FO_RRESP_INT_FO_SHIFT 5
#define  TESTIDMA_INT_FO_RRESP_INT_FO_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_FO_RRESP_INT_FO_MASK) >> TESTIDMA_INT_FO_RRESP_INT_FO_SHIFT)
#define  TESTIDMA_INT_FO_RRESP_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_FO_RRESP_INT_FO_MASK) | (((uint32_t)val) << TESTIDMA_INT_FO_RRESP_INT_FO_SHIFT))

#define  TESTIDMA_INT_FO_BRESP_INT_FO_MASK 0x10
#define  TESTIDMA_INT_FO_BRESP_INT_FO_SHIFT 4
#define  TESTIDMA_INT_FO_BRESP_INT_FO_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_FO_BRESP_INT_FO_MASK) >> TESTIDMA_INT_FO_BRESP_INT_FO_SHIFT)
#define  TESTIDMA_INT_FO_BRESP_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_FO_BRESP_INT_FO_MASK) | (((uint32_t)val) << TESTIDMA_INT_FO_BRESP_INT_FO_SHIFT))

#define  TESTIDMA_INT_FO_RST_INT_FO_MASK 0x8
#define  TESTIDMA_INT_FO_RST_INT_FO_SHIFT 3
#define  TESTIDMA_INT_FO_RST_INT_FO_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_FO_RST_INT_FO_MASK) >> TESTIDMA_INT_FO_RST_INT_FO_SHIFT)
#define  TESTIDMA_INT_FO_RST_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_FO_RST_INT_FO_MASK) | (((uint32_t)val) << TESTIDMA_INT_FO_RST_INT_FO_SHIFT))

#define  TESTIDMA_INT_FO_EOI_INT_FO_MASK 0x4
#define  TESTIDMA_INT_FO_EOI_INT_FO_SHIFT 2
#define  TESTIDMA_INT_FO_EOI_INT_FO_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_FO_EOI_INT_FO_MASK) >> TESTIDMA_INT_FO_EOI_INT_FO_SHIFT)
#define  TESTIDMA_INT_FO_EOI_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_FO_EOI_INT_FO_MASK) | (((uint32_t)val) << TESTIDMA_INT_FO_EOI_INT_FO_SHIFT))

#define  TESTIDMA_INT_FO_FIN_INT_FO_MASK 0x2
#define  TESTIDMA_INT_FO_FIN_INT_FO_SHIFT 1
#define  TESTIDMA_INT_FO_FIN_INT_FO_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_FO_FIN_INT_FO_MASK) >> TESTIDMA_INT_FO_FIN_INT_FO_SHIFT)
#define  TESTIDMA_INT_FO_FIN_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_FO_FIN_INT_FO_MASK) | (((uint32_t)val) << TESTIDMA_INT_FO_FIN_INT_FO_SHIFT))

#define  TESTIDMA_INT_FO_WHO_INT_FO_MASK 0x1
#define  TESTIDMA_INT_FO_WHO_INT_FO_SHIFT 0
#define  TESTIDMA_INT_FO_WHO_INT_FO_MASK_SHIFT(reg) (((reg) & TESTIDMA_INT_FO_WHO_INT_FO_MASK) >> TESTIDMA_INT_FO_WHO_INT_FO_SHIFT)
#define  TESTIDMA_INT_FO_WHO_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_INT_FO_WHO_INT_FO_MASK) | (((uint32_t)val) << TESTIDMA_INT_FO_WHO_INT_FO_SHIFT))

//====================================================================
//Register: Descriptor Write Register (desc_write)
//====================================================================

#define  TESTIDMA_DESC_WRITE_DESC_MASK 0xffffffff
#define  TESTIDMA_DESC_WRITE_DESC_SHIFT 0
#define  TESTIDMA_DESC_WRITE_DESC_MASK_SHIFT(reg) (((reg) & TESTIDMA_DESC_WRITE_DESC_MASK) >> TESTIDMA_DESC_WRITE_DESC_SHIFT)
#define  TESTIDMA_DESC_WRITE_DESC_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_DESC_WRITE_DESC_MASK) | (((uint32_t)val) << TESTIDMA_DESC_WRITE_DESC_SHIFT))

//====================================================================
//Register: Descriptor Read Register (desc_read)
//====================================================================

#define  TESTIDMA_DESC_READ_DESC_MASK 0xffffffff
#define  TESTIDMA_DESC_READ_DESC_SHIFT 0
#define  TESTIDMA_DESC_READ_DESC_MASK_SHIFT(reg) (((reg) & TESTIDMA_DESC_READ_DESC_MASK) >> TESTIDMA_DESC_READ_DESC_SHIFT)
#define  TESTIDMA_DESC_READ_DESC_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_DESC_READ_DESC_MASK) | (((uint32_t)val) << TESTIDMA_DESC_READ_DESC_SHIFT))

//====================================================================
//Register: Descriptor Transfer Length Register (xfer_length)
//====================================================================

#define  TESTIDMA_XFER_LENGTH_LEN_MASK 0xffffffff
#define  TESTIDMA_XFER_LENGTH_LEN_SHIFT 0
#define  TESTIDMA_XFER_LENGTH_LEN_MASK_SHIFT(reg) (((reg) & TESTIDMA_XFER_LENGTH_LEN_MASK) >> TESTIDMA_XFER_LENGTH_LEN_SHIFT)
#define  TESTIDMA_XFER_LENGTH_LEN_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_XFER_LENGTH_LEN_MASK) | (((uint32_t)val) << TESTIDMA_XFER_LENGTH_LEN_SHIFT))

//====================================================================
//Register: Descriptor Transfer Address Register (xfer_addr)
//====================================================================

#define  TESTIDMA_XFER_ADDR_ADDR_MASK 0xffffffff
#define  TESTIDMA_XFER_ADDR_ADDR_SHIFT 0
#define  TESTIDMA_XFER_ADDR_ADDR_MASK_SHIFT(reg) (((reg) & TESTIDMA_XFER_ADDR_ADDR_MASK) >> TESTIDMA_XFER_ADDR_ADDR_SHIFT)
#define  TESTIDMA_XFER_ADDR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_XFER_ADDR_ADDR_MASK) | (((uint32_t)val) << TESTIDMA_XFER_ADDR_ADDR_SHIFT))

//====================================================================
//Register: Descriptor Control Word Register (ctrl_word)
//====================================================================

#define  TESTIDMA_CTRL_WORD_CTRL_WORD_MASK 0xffffffff
#define  TESTIDMA_CTRL_WORD_CTRL_WORD_SHIFT 0
#define  TESTIDMA_CTRL_WORD_CTRL_WORD_MASK_SHIFT(reg) (((reg) & TESTIDMA_CTRL_WORD_CTRL_WORD_MASK) >> TESTIDMA_CTRL_WORD_CTRL_WORD_SHIFT)
#define  TESTIDMA_CTRL_WORD_CTRL_WORD_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_CTRL_WORD_CTRL_WORD_MASK) | (((uint32_t)val) << TESTIDMA_CTRL_WORD_CTRL_WORD_SHIFT))

//====================================================================
//Register: Soft Reset register (reset)
//====================================================================

#define  TESTIDMA_RESET_RESERVED1_MASK 0xfffffffe
#define  TESTIDMA_RESET_RESERVED1_SHIFT 1
#define  TESTIDMA_RESET_RESERVED1_MASK_SHIFT(reg) (((reg) & TESTIDMA_RESET_RESERVED1_MASK) >> TESTIDMA_RESET_RESERVED1_SHIFT)
#define  TESTIDMA_RESET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_RESET_RESERVED1_MASK) | (((uint32_t)val) << TESTIDMA_RESET_RESERVED1_SHIFT))

#define  TESTIDMA_RESET_SOFT_RESET_MASK 0x1
#define  TESTIDMA_RESET_SOFT_RESET_SHIFT 0
#define  TESTIDMA_RESET_SOFT_RESET_MASK_SHIFT(reg) (((reg) & TESTIDMA_RESET_SOFT_RESET_MASK) >> TESTIDMA_RESET_SOFT_RESET_SHIFT)
#define  TESTIDMA_RESET_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~TESTIDMA_RESET_SOFT_RESET_MASK) | (((uint32_t)val) << TESTIDMA_RESET_SOFT_RESET_SHIFT))

#endif // ICE_TEST
