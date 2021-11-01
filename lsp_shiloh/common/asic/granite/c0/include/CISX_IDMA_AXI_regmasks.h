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
 * \file CISX_IDMA_AXI_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CISX_IDMA_REGMASKS_H_
#define _CISX_IDMA_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CISX_IDMA (CISX_IDMA)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  CISX_IDMA_CFG_RESERVED1_MASK 0xfe000000
#define  CISX_IDMA_CFG_RESERVED1_SHIFT 25
#define  CISX_IDMA_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_RESERVED1_MASK) >> CISX_IDMA_CFG_RESERVED1_SHIFT)
#define  CISX_IDMA_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_RESERVED1_SHIFT))

#define  CISX_IDMA_CFG_REPLICATE_MASK 0x1000000
#define  CISX_IDMA_CFG_REPLICATE_SHIFT 24
#define  CISX_IDMA_CFG_REPLICATE_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_REPLICATE_MASK) >> CISX_IDMA_CFG_REPLICATE_SHIFT)
#define  CISX_IDMA_CFG_REPLICATE_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_REPLICATE_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_REPLICATE_SHIFT))

#define  CISX_IDMA_CFG_RESERVED2_MASK 0xfe0000
#define  CISX_IDMA_CFG_RESERVED2_SHIFT 17
#define  CISX_IDMA_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_RESERVED2_MASK) >> CISX_IDMA_CFG_RESERVED2_SHIFT)
#define  CISX_IDMA_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_RESERVED2_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_RESERVED2_SHIFT))

#define  CISX_IDMA_CFG_ENABLE_MASK 0x10000
#define  CISX_IDMA_CFG_ENABLE_SHIFT 16
#define  CISX_IDMA_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_ENABLE_MASK) >> CISX_IDMA_CFG_ENABLE_SHIFT)
#define  CISX_IDMA_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_ENABLE_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_ENABLE_SHIFT))

#define  CISX_IDMA_CFG_RESERVED3_MASK 0xf800
#define  CISX_IDMA_CFG_RESERVED3_SHIFT 11
#define  CISX_IDMA_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_RESERVED3_MASK) >> CISX_IDMA_CFG_RESERVED3_SHIFT)
#define  CISX_IDMA_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_RESERVED3_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_RESERVED3_SHIFT))

#define  CISX_IDMA_CFG_OUT_WIDTH_MASK 0x700
#define  CISX_IDMA_CFG_OUT_WIDTH_SHIFT 8
#define  CISX_IDMA_CFG_OUT_WIDTH_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_OUT_WIDTH_MASK) >> CISX_IDMA_CFG_OUT_WIDTH_SHIFT)
#define  CISX_IDMA_CFG_OUT_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_OUT_WIDTH_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_OUT_WIDTH_SHIFT))

#define  CISX_IDMA_CFG_RESERVED4_MASK 0xfc
#define  CISX_IDMA_CFG_RESERVED4_SHIFT 2
#define  CISX_IDMA_CFG_RESERVED4_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_RESERVED4_MASK) >> CISX_IDMA_CFG_RESERVED4_SHIFT)
#define  CISX_IDMA_CFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_RESERVED4_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_RESERVED4_SHIFT))

#define  CISX_IDMA_CFG_BURST_LEN_MASK 0x3
#define  CISX_IDMA_CFG_BURST_LEN_SHIFT 0
#define  CISX_IDMA_CFG_BURST_LEN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CFG_BURST_LEN_MASK) >> CISX_IDMA_CFG_BURST_LEN_SHIFT)
#define  CISX_IDMA_CFG_BURST_LEN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CFG_BURST_LEN_MASK) | (((uint32_t)val) << CISX_IDMA_CFG_BURST_LEN_SHIFT))

//====================================================================
//Register: Status register (status)
//====================================================================

#define  CISX_IDMA_STATUS_RESERVED1_MASK 0xfffffe00
#define  CISX_IDMA_STATUS_RESERVED1_SHIFT 9
#define  CISX_IDMA_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_RESERVED1_MASK) >> CISX_IDMA_STATUS_RESERVED1_SHIFT)
#define  CISX_IDMA_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_RESERVED1_SHIFT))

#define  CISX_IDMA_STATUS_SOI_MASK 0x100
#define  CISX_IDMA_STATUS_SOI_SHIFT 8
#define  CISX_IDMA_STATUS_SOI_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_SOI_MASK) >> CISX_IDMA_STATUS_SOI_SHIFT)
#define  CISX_IDMA_STATUS_SOI_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_SOI_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_SOI_SHIFT))

#define  CISX_IDMA_STATUS_EOI_MASK 0x80
#define  CISX_IDMA_STATUS_EOI_SHIFT 7
#define  CISX_IDMA_STATUS_EOI_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_EOI_MASK) >> CISX_IDMA_STATUS_EOI_SHIFT)
#define  CISX_IDMA_STATUS_EOI_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_EOI_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_EOI_SHIFT))

#define  CISX_IDMA_STATUS_SOFTRESET_MASK 0x40
#define  CISX_IDMA_STATUS_SOFTRESET_SHIFT 6
#define  CISX_IDMA_STATUS_SOFTRESET_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_SOFTRESET_MASK) >> CISX_IDMA_STATUS_SOFTRESET_SHIFT)
#define  CISX_IDMA_STATUS_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_SOFTRESET_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_SOFTRESET_SHIFT))

#define  CISX_IDMA_STATUS_FULL_CBUF_MASK 0x20
#define  CISX_IDMA_STATUS_FULL_CBUF_SHIFT 5
#define  CISX_IDMA_STATUS_FULL_CBUF_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_FULL_CBUF_MASK) >> CISX_IDMA_STATUS_FULL_CBUF_SHIFT)
#define  CISX_IDMA_STATUS_FULL_CBUF_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_FULL_CBUF_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_FULL_CBUF_SHIFT))

#define  CISX_IDMA_STATUS_EMPTY_CBUF_MASK 0x10
#define  CISX_IDMA_STATUS_EMPTY_CBUF_SHIFT 4
#define  CISX_IDMA_STATUS_EMPTY_CBUF_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_EMPTY_CBUF_MASK) >> CISX_IDMA_STATUS_EMPTY_CBUF_SHIFT)
#define  CISX_IDMA_STATUS_EMPTY_CBUF_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_EMPTY_CBUF_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_EMPTY_CBUF_SHIFT))

#define  CISX_IDMA_STATUS_FULL_DBUF_MASK 0x8
#define  CISX_IDMA_STATUS_FULL_DBUF_SHIFT 3
#define  CISX_IDMA_STATUS_FULL_DBUF_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_FULL_DBUF_MASK) >> CISX_IDMA_STATUS_FULL_DBUF_SHIFT)
#define  CISX_IDMA_STATUS_FULL_DBUF_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_FULL_DBUF_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_FULL_DBUF_SHIFT))

#define  CISX_IDMA_STATUS_EMPTY_DBUF_MASK 0x4
#define  CISX_IDMA_STATUS_EMPTY_DBUF_SHIFT 2
#define  CISX_IDMA_STATUS_EMPTY_DBUF_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_EMPTY_DBUF_MASK) >> CISX_IDMA_STATUS_EMPTY_DBUF_SHIFT)
#define  CISX_IDMA_STATUS_EMPTY_DBUF_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_EMPTY_DBUF_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_EMPTY_DBUF_SHIFT))

#define  CISX_IDMA_STATUS_PACKER_EMPTY_MASK 0x2
#define  CISX_IDMA_STATUS_PACKER_EMPTY_SHIFT 1
#define  CISX_IDMA_STATUS_PACKER_EMPTY_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_PACKER_EMPTY_MASK) >> CISX_IDMA_STATUS_PACKER_EMPTY_SHIFT)
#define  CISX_IDMA_STATUS_PACKER_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_PACKER_EMPTY_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_PACKER_EMPTY_SHIFT))

#define  CISX_IDMA_STATUS_DMA_BUSY_MASK 0x1
#define  CISX_IDMA_STATUS_DMA_BUSY_SHIFT 0
#define  CISX_IDMA_STATUS_DMA_BUSY_MASK_SHIFT(reg) (((reg) & CISX_IDMA_STATUS_DMA_BUSY_MASK) >> CISX_IDMA_STATUS_DMA_BUSY_SHIFT)
#define  CISX_IDMA_STATUS_DMA_BUSY_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_STATUS_DMA_BUSY_MASK) | (((uint32_t)val) << CISX_IDMA_STATUS_DMA_BUSY_SHIFT))

//====================================================================
//Register: Line Width register (line_width)
//====================================================================

#define  CISX_IDMA_LINE_WIDTH_RESERVED1_MASK 0xffff0000
#define  CISX_IDMA_LINE_WIDTH_RESERVED1_SHIFT 16
#define  CISX_IDMA_LINE_WIDTH_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_LINE_WIDTH_RESERVED1_MASK) >> CISX_IDMA_LINE_WIDTH_RESERVED1_SHIFT)
#define  CISX_IDMA_LINE_WIDTH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_LINE_WIDTH_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_LINE_WIDTH_RESERVED1_SHIFT))

#define  CISX_IDMA_LINE_WIDTH_LINE_WIDTH_MASK 0xffff
#define  CISX_IDMA_LINE_WIDTH_LINE_WIDTH_SHIFT 0
#define  CISX_IDMA_LINE_WIDTH_LINE_WIDTH_MASK_SHIFT(reg) (((reg) & CISX_IDMA_LINE_WIDTH_LINE_WIDTH_MASK) >> CISX_IDMA_LINE_WIDTH_LINE_WIDTH_SHIFT)
#define  CISX_IDMA_LINE_WIDTH_LINE_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_LINE_WIDTH_LINE_WIDTH_MASK) | (((uint32_t)val) << CISX_IDMA_LINE_WIDTH_LINE_WIDTH_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  CISX_IDMA_INT_EN_RESERVED1_MASK 0xffffffc0
#define  CISX_IDMA_INT_EN_RESERVED1_SHIFT 6
#define  CISX_IDMA_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_EN_RESERVED1_MASK) >> CISX_IDMA_INT_EN_RESERVED1_SHIFT)
#define  CISX_IDMA_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_INT_EN_RESERVED1_SHIFT))

#define  CISX_IDMA_INT_EN_RRESP_INT_EN_MASK 0x20
#define  CISX_IDMA_INT_EN_RRESP_INT_EN_SHIFT 5
#define  CISX_IDMA_INT_EN_RRESP_INT_EN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_EN_RRESP_INT_EN_MASK) >> CISX_IDMA_INT_EN_RRESP_INT_EN_SHIFT)
#define  CISX_IDMA_INT_EN_RRESP_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_EN_RRESP_INT_EN_MASK) | (((uint32_t)val) << CISX_IDMA_INT_EN_RRESP_INT_EN_SHIFT))

#define  CISX_IDMA_INT_EN_BRESP_INT_EN_MASK 0x10
#define  CISX_IDMA_INT_EN_BRESP_INT_EN_SHIFT 4
#define  CISX_IDMA_INT_EN_BRESP_INT_EN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_EN_BRESP_INT_EN_MASK) >> CISX_IDMA_INT_EN_BRESP_INT_EN_SHIFT)
#define  CISX_IDMA_INT_EN_BRESP_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_EN_BRESP_INT_EN_MASK) | (((uint32_t)val) << CISX_IDMA_INT_EN_BRESP_INT_EN_SHIFT))

#define  CISX_IDMA_INT_EN_RST_INT_EN_MASK 0x8
#define  CISX_IDMA_INT_EN_RST_INT_EN_SHIFT 3
#define  CISX_IDMA_INT_EN_RST_INT_EN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_EN_RST_INT_EN_MASK) >> CISX_IDMA_INT_EN_RST_INT_EN_SHIFT)
#define  CISX_IDMA_INT_EN_RST_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_EN_RST_INT_EN_MASK) | (((uint32_t)val) << CISX_IDMA_INT_EN_RST_INT_EN_SHIFT))

#define  CISX_IDMA_INT_EN_EOI_INT_EN_MASK 0x4
#define  CISX_IDMA_INT_EN_EOI_INT_EN_SHIFT 2
#define  CISX_IDMA_INT_EN_EOI_INT_EN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_EN_EOI_INT_EN_MASK) >> CISX_IDMA_INT_EN_EOI_INT_EN_SHIFT)
#define  CISX_IDMA_INT_EN_EOI_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_EN_EOI_INT_EN_MASK) | (((uint32_t)val) << CISX_IDMA_INT_EN_EOI_INT_EN_SHIFT))

#define  CISX_IDMA_INT_EN_FIN_INT_EN_MASK 0x2
#define  CISX_IDMA_INT_EN_FIN_INT_EN_SHIFT 1
#define  CISX_IDMA_INT_EN_FIN_INT_EN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_EN_FIN_INT_EN_MASK) >> CISX_IDMA_INT_EN_FIN_INT_EN_SHIFT)
#define  CISX_IDMA_INT_EN_FIN_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_EN_FIN_INT_EN_MASK) | (((uint32_t)val) << CISX_IDMA_INT_EN_FIN_INT_EN_SHIFT))

#define  CISX_IDMA_INT_EN_WHO_INT_EN_MASK 0x1
#define  CISX_IDMA_INT_EN_WHO_INT_EN_SHIFT 0
#define  CISX_IDMA_INT_EN_WHO_INT_EN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_EN_WHO_INT_EN_MASK) >> CISX_IDMA_INT_EN_WHO_INT_EN_SHIFT)
#define  CISX_IDMA_INT_EN_WHO_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_EN_WHO_INT_EN_MASK) | (((uint32_t)val) << CISX_IDMA_INT_EN_WHO_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  CISX_IDMA_INT_ST_RESERVED1_MASK 0xffffffc0
#define  CISX_IDMA_INT_ST_RESERVED1_SHIFT 6
#define  CISX_IDMA_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_ST_RESERVED1_MASK) >> CISX_IDMA_INT_ST_RESERVED1_SHIFT)
#define  CISX_IDMA_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_INT_ST_RESERVED1_SHIFT))

#define  CISX_IDMA_INT_ST_RRESP_INT_ST_MASK 0x20
#define  CISX_IDMA_INT_ST_RRESP_INT_ST_SHIFT 5
#define  CISX_IDMA_INT_ST_RRESP_INT_ST_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_ST_RRESP_INT_ST_MASK) >> CISX_IDMA_INT_ST_RRESP_INT_ST_SHIFT)
#define  CISX_IDMA_INT_ST_RRESP_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_ST_RRESP_INT_ST_MASK) | (((uint32_t)val) << CISX_IDMA_INT_ST_RRESP_INT_ST_SHIFT))

#define  CISX_IDMA_INT_ST_BRESP_INT_ST_MASK 0x10
#define  CISX_IDMA_INT_ST_BRESP_INT_ST_SHIFT 4
#define  CISX_IDMA_INT_ST_BRESP_INT_ST_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_ST_BRESP_INT_ST_MASK) >> CISX_IDMA_INT_ST_BRESP_INT_ST_SHIFT)
#define  CISX_IDMA_INT_ST_BRESP_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_ST_BRESP_INT_ST_MASK) | (((uint32_t)val) << CISX_IDMA_INT_ST_BRESP_INT_ST_SHIFT))

#define  CISX_IDMA_INT_ST_RST_INT_ST_MASK 0x8
#define  CISX_IDMA_INT_ST_RST_INT_ST_SHIFT 3
#define  CISX_IDMA_INT_ST_RST_INT_ST_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_ST_RST_INT_ST_MASK) >> CISX_IDMA_INT_ST_RST_INT_ST_SHIFT)
#define  CISX_IDMA_INT_ST_RST_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_ST_RST_INT_ST_MASK) | (((uint32_t)val) << CISX_IDMA_INT_ST_RST_INT_ST_SHIFT))

#define  CISX_IDMA_INT_ST_EOI_INT_ST_MASK 0x4
#define  CISX_IDMA_INT_ST_EOI_INT_ST_SHIFT 2
#define  CISX_IDMA_INT_ST_EOI_INT_ST_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_ST_EOI_INT_ST_MASK) >> CISX_IDMA_INT_ST_EOI_INT_ST_SHIFT)
#define  CISX_IDMA_INT_ST_EOI_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_ST_EOI_INT_ST_MASK) | (((uint32_t)val) << CISX_IDMA_INT_ST_EOI_INT_ST_SHIFT))

#define  CISX_IDMA_INT_ST_FIN_INT_ST_MASK 0x2
#define  CISX_IDMA_INT_ST_FIN_INT_ST_SHIFT 1
#define  CISX_IDMA_INT_ST_FIN_INT_ST_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_ST_FIN_INT_ST_MASK) >> CISX_IDMA_INT_ST_FIN_INT_ST_SHIFT)
#define  CISX_IDMA_INT_ST_FIN_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_ST_FIN_INT_ST_MASK) | (((uint32_t)val) << CISX_IDMA_INT_ST_FIN_INT_ST_SHIFT))

#define  CISX_IDMA_INT_ST_WHO_INT_ST_MASK 0x1
#define  CISX_IDMA_INT_ST_WHO_INT_ST_SHIFT 0
#define  CISX_IDMA_INT_ST_WHO_INT_ST_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_ST_WHO_INT_ST_MASK) >> CISX_IDMA_INT_ST_WHO_INT_ST_SHIFT)
#define  CISX_IDMA_INT_ST_WHO_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_ST_WHO_INT_ST_MASK) | (((uint32_t)val) << CISX_IDMA_INT_ST_WHO_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Clear Register (int_cl)
//====================================================================

#define  CISX_IDMA_INT_CL_RESERVED1_MASK 0xffffffc0
#define  CISX_IDMA_INT_CL_RESERVED1_SHIFT 6
#define  CISX_IDMA_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_CL_RESERVED1_MASK) >> CISX_IDMA_INT_CL_RESERVED1_SHIFT)
#define  CISX_IDMA_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_INT_CL_RESERVED1_SHIFT))

#define  CISX_IDMA_INT_CL_RRESP_INT_CL_MASK 0x20
#define  CISX_IDMA_INT_CL_RRESP_INT_CL_SHIFT 5
#define  CISX_IDMA_INT_CL_RRESP_INT_CL_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_CL_RRESP_INT_CL_MASK) >> CISX_IDMA_INT_CL_RRESP_INT_CL_SHIFT)
#define  CISX_IDMA_INT_CL_RRESP_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_CL_RRESP_INT_CL_MASK) | (((uint32_t)val) << CISX_IDMA_INT_CL_RRESP_INT_CL_SHIFT))

#define  CISX_IDMA_INT_CL_BRESP_INT_CL_MASK 0x10
#define  CISX_IDMA_INT_CL_BRESP_INT_CL_SHIFT 4
#define  CISX_IDMA_INT_CL_BRESP_INT_CL_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_CL_BRESP_INT_CL_MASK) >> CISX_IDMA_INT_CL_BRESP_INT_CL_SHIFT)
#define  CISX_IDMA_INT_CL_BRESP_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_CL_BRESP_INT_CL_MASK) | (((uint32_t)val) << CISX_IDMA_INT_CL_BRESP_INT_CL_SHIFT))

#define  CISX_IDMA_INT_CL_RST_INT_CL_MASK 0x8
#define  CISX_IDMA_INT_CL_RST_INT_CL_SHIFT 3
#define  CISX_IDMA_INT_CL_RST_INT_CL_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_CL_RST_INT_CL_MASK) >> CISX_IDMA_INT_CL_RST_INT_CL_SHIFT)
#define  CISX_IDMA_INT_CL_RST_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_CL_RST_INT_CL_MASK) | (((uint32_t)val) << CISX_IDMA_INT_CL_RST_INT_CL_SHIFT))

#define  CISX_IDMA_INT_CL_EOI_INT_CL_MASK 0x4
#define  CISX_IDMA_INT_CL_EOI_INT_CL_SHIFT 2
#define  CISX_IDMA_INT_CL_EOI_INT_CL_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_CL_EOI_INT_CL_MASK) >> CISX_IDMA_INT_CL_EOI_INT_CL_SHIFT)
#define  CISX_IDMA_INT_CL_EOI_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_CL_EOI_INT_CL_MASK) | (((uint32_t)val) << CISX_IDMA_INT_CL_EOI_INT_CL_SHIFT))

#define  CISX_IDMA_INT_CL_FIN_INT_CL_MASK 0x2
#define  CISX_IDMA_INT_CL_FIN_INT_CL_SHIFT 1
#define  CISX_IDMA_INT_CL_FIN_INT_CL_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_CL_FIN_INT_CL_MASK) >> CISX_IDMA_INT_CL_FIN_INT_CL_SHIFT)
#define  CISX_IDMA_INT_CL_FIN_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_CL_FIN_INT_CL_MASK) | (((uint32_t)val) << CISX_IDMA_INT_CL_FIN_INT_CL_SHIFT))

#define  CISX_IDMA_INT_CL_WHO_INT_CL_MASK 0x1
#define  CISX_IDMA_INT_CL_WHO_INT_CL_SHIFT 0
#define  CISX_IDMA_INT_CL_WHO_INT_CL_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_CL_WHO_INT_CL_MASK) >> CISX_IDMA_INT_CL_WHO_INT_CL_SHIFT)
#define  CISX_IDMA_INT_CL_WHO_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_CL_WHO_INT_CL_MASK) | (((uint32_t)val) << CISX_IDMA_INT_CL_WHO_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  CISX_IDMA_INT_FO_RESERVED1_MASK 0xffffffc0
#define  CISX_IDMA_INT_FO_RESERVED1_SHIFT 6
#define  CISX_IDMA_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_FO_RESERVED1_MASK) >> CISX_IDMA_INT_FO_RESERVED1_SHIFT)
#define  CISX_IDMA_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_INT_FO_RESERVED1_SHIFT))

#define  CISX_IDMA_INT_FO_RRESP_INT_FO_MASK 0x20
#define  CISX_IDMA_INT_FO_RRESP_INT_FO_SHIFT 5
#define  CISX_IDMA_INT_FO_RRESP_INT_FO_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_FO_RRESP_INT_FO_MASK) >> CISX_IDMA_INT_FO_RRESP_INT_FO_SHIFT)
#define  CISX_IDMA_INT_FO_RRESP_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_FO_RRESP_INT_FO_MASK) | (((uint32_t)val) << CISX_IDMA_INT_FO_RRESP_INT_FO_SHIFT))

#define  CISX_IDMA_INT_FO_BRESP_INT_FO_MASK 0x10
#define  CISX_IDMA_INT_FO_BRESP_INT_FO_SHIFT 4
#define  CISX_IDMA_INT_FO_BRESP_INT_FO_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_FO_BRESP_INT_FO_MASK) >> CISX_IDMA_INT_FO_BRESP_INT_FO_SHIFT)
#define  CISX_IDMA_INT_FO_BRESP_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_FO_BRESP_INT_FO_MASK) | (((uint32_t)val) << CISX_IDMA_INT_FO_BRESP_INT_FO_SHIFT))

#define  CISX_IDMA_INT_FO_RST_INT_FO_MASK 0x8
#define  CISX_IDMA_INT_FO_RST_INT_FO_SHIFT 3
#define  CISX_IDMA_INT_FO_RST_INT_FO_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_FO_RST_INT_FO_MASK) >> CISX_IDMA_INT_FO_RST_INT_FO_SHIFT)
#define  CISX_IDMA_INT_FO_RST_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_FO_RST_INT_FO_MASK) | (((uint32_t)val) << CISX_IDMA_INT_FO_RST_INT_FO_SHIFT))

#define  CISX_IDMA_INT_FO_EOI_INT_FO_MASK 0x4
#define  CISX_IDMA_INT_FO_EOI_INT_FO_SHIFT 2
#define  CISX_IDMA_INT_FO_EOI_INT_FO_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_FO_EOI_INT_FO_MASK) >> CISX_IDMA_INT_FO_EOI_INT_FO_SHIFT)
#define  CISX_IDMA_INT_FO_EOI_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_FO_EOI_INT_FO_MASK) | (((uint32_t)val) << CISX_IDMA_INT_FO_EOI_INT_FO_SHIFT))

#define  CISX_IDMA_INT_FO_FIN_INT_FO_MASK 0x2
#define  CISX_IDMA_INT_FO_FIN_INT_FO_SHIFT 1
#define  CISX_IDMA_INT_FO_FIN_INT_FO_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_FO_FIN_INT_FO_MASK) >> CISX_IDMA_INT_FO_FIN_INT_FO_SHIFT)
#define  CISX_IDMA_INT_FO_FIN_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_FO_FIN_INT_FO_MASK) | (((uint32_t)val) << CISX_IDMA_INT_FO_FIN_INT_FO_SHIFT))

#define  CISX_IDMA_INT_FO_WHO_INT_FO_MASK 0x1
#define  CISX_IDMA_INT_FO_WHO_INT_FO_SHIFT 0
#define  CISX_IDMA_INT_FO_WHO_INT_FO_MASK_SHIFT(reg) (((reg) & CISX_IDMA_INT_FO_WHO_INT_FO_MASK) >> CISX_IDMA_INT_FO_WHO_INT_FO_SHIFT)
#define  CISX_IDMA_INT_FO_WHO_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_INT_FO_WHO_INT_FO_MASK) | (((uint32_t)val) << CISX_IDMA_INT_FO_WHO_INT_FO_SHIFT))

//====================================================================
//Register: Descriptor Write Register (desc_write)
//====================================================================

#define  CISX_IDMA_DESC_WRITE_DESC_MASK 0xffffffff
#define  CISX_IDMA_DESC_WRITE_DESC_SHIFT 0
#define  CISX_IDMA_DESC_WRITE_DESC_MASK_SHIFT(reg) (((reg) & CISX_IDMA_DESC_WRITE_DESC_MASK) >> CISX_IDMA_DESC_WRITE_DESC_SHIFT)
#define  CISX_IDMA_DESC_WRITE_DESC_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_DESC_WRITE_DESC_MASK) | (((uint32_t)val) << CISX_IDMA_DESC_WRITE_DESC_SHIFT))

//====================================================================
//Register: Descriptor Read Register (desc_read)
//====================================================================

#define  CISX_IDMA_DESC_READ_DESC_MASK 0xffffffff
#define  CISX_IDMA_DESC_READ_DESC_SHIFT 0
#define  CISX_IDMA_DESC_READ_DESC_MASK_SHIFT(reg) (((reg) & CISX_IDMA_DESC_READ_DESC_MASK) >> CISX_IDMA_DESC_READ_DESC_SHIFT)
#define  CISX_IDMA_DESC_READ_DESC_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_DESC_READ_DESC_MASK) | (((uint32_t)val) << CISX_IDMA_DESC_READ_DESC_SHIFT))

//====================================================================
//Register: Descriptor Transfer Length Register (xfer_length)
//====================================================================

#define  CISX_IDMA_XFER_LENGTH_LEN_MASK 0xffffffff
#define  CISX_IDMA_XFER_LENGTH_LEN_SHIFT 0
#define  CISX_IDMA_XFER_LENGTH_LEN_MASK_SHIFT(reg) (((reg) & CISX_IDMA_XFER_LENGTH_LEN_MASK) >> CISX_IDMA_XFER_LENGTH_LEN_SHIFT)
#define  CISX_IDMA_XFER_LENGTH_LEN_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_XFER_LENGTH_LEN_MASK) | (((uint32_t)val) << CISX_IDMA_XFER_LENGTH_LEN_SHIFT))

//====================================================================
//Register: Descriptor Transfer Address Register (xfer_addr)
//====================================================================

#define  CISX_IDMA_XFER_ADDR_ADDR_MASK 0xffffffff
#define  CISX_IDMA_XFER_ADDR_ADDR_SHIFT 0
#define  CISX_IDMA_XFER_ADDR_ADDR_MASK_SHIFT(reg) (((reg) & CISX_IDMA_XFER_ADDR_ADDR_MASK) >> CISX_IDMA_XFER_ADDR_ADDR_SHIFT)
#define  CISX_IDMA_XFER_ADDR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_XFER_ADDR_ADDR_MASK) | (((uint32_t)val) << CISX_IDMA_XFER_ADDR_ADDR_SHIFT))

//====================================================================
//Register: Descriptor Control Word Register (ctrl_word)
//====================================================================

#define  CISX_IDMA_CTRL_WORD_CTRL_WORD_MASK 0xffffffff
#define  CISX_IDMA_CTRL_WORD_CTRL_WORD_SHIFT 0
#define  CISX_IDMA_CTRL_WORD_CTRL_WORD_MASK_SHIFT(reg) (((reg) & CISX_IDMA_CTRL_WORD_CTRL_WORD_MASK) >> CISX_IDMA_CTRL_WORD_CTRL_WORD_SHIFT)
#define  CISX_IDMA_CTRL_WORD_CTRL_WORD_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_CTRL_WORD_CTRL_WORD_MASK) | (((uint32_t)val) << CISX_IDMA_CTRL_WORD_CTRL_WORD_SHIFT))

//====================================================================
//Register: Soft Reset register (reset)
//====================================================================

#define  CISX_IDMA_RESET_RESERVED1_MASK 0xfffffffe
#define  CISX_IDMA_RESET_RESERVED1_SHIFT 1
#define  CISX_IDMA_RESET_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_IDMA_RESET_RESERVED1_MASK) >> CISX_IDMA_RESET_RESERVED1_SHIFT)
#define  CISX_IDMA_RESET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_RESET_RESERVED1_MASK) | (((uint32_t)val) << CISX_IDMA_RESET_RESERVED1_SHIFT))

#define  CISX_IDMA_RESET_SOFT_RESET_MASK 0x1
#define  CISX_IDMA_RESET_SOFT_RESET_SHIFT 0
#define  CISX_IDMA_RESET_SOFT_RESET_MASK_SHIFT(reg) (((reg) & CISX_IDMA_RESET_SOFT_RESET_MASK) >> CISX_IDMA_RESET_SOFT_RESET_SHIFT)
#define  CISX_IDMA_RESET_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~CISX_IDMA_RESET_SOFT_RESET_MASK) | (((uint32_t)val) << CISX_IDMA_RESET_SOFT_RESET_SHIFT))

#endif // CISX_IDMA
