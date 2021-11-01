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
 * \file IDMA_2D_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _IDMA_2D_REGMASKS_H_
#define _IDMA_2D_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: IDMA_2D (IDMA_2D)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  IDMA_2D_CFG_RESERVED1_MASK 0xfffe0000
#define  IDMA_2D_CFG_RESERVED1_SHIFT 17
#define  IDMA_2D_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_CFG_RESERVED1_MASK) >> IDMA_2D_CFG_RESERVED1_SHIFT)
#define  IDMA_2D_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_CFG_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_CFG_RESERVED1_SHIFT))

#define  IDMA_2D_CFG_ENABLE_MASK 0x10000
#define  IDMA_2D_CFG_ENABLE_SHIFT 16
#define  IDMA_2D_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & IDMA_2D_CFG_ENABLE_MASK) >> IDMA_2D_CFG_ENABLE_SHIFT)
#define  IDMA_2D_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_CFG_ENABLE_MASK) | (((uint32_t)val) << IDMA_2D_CFG_ENABLE_SHIFT))

#define  IDMA_2D_CFG_RESERVED2_MASK 0xfffc
#define  IDMA_2D_CFG_RESERVED2_SHIFT 2
#define  IDMA_2D_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & IDMA_2D_CFG_RESERVED2_MASK) >> IDMA_2D_CFG_RESERVED2_SHIFT)
#define  IDMA_2D_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_CFG_RESERVED2_MASK) | (((uint32_t)val) << IDMA_2D_CFG_RESERVED2_SHIFT))

#define  IDMA_2D_CFG_BURST_LEN_MASK 0x3
#define  IDMA_2D_CFG_BURST_LEN_SHIFT 0
#define  IDMA_2D_CFG_BURST_LEN_MASK_SHIFT(reg) (((reg) & IDMA_2D_CFG_BURST_LEN_MASK) >> IDMA_2D_CFG_BURST_LEN_SHIFT)
#define  IDMA_2D_CFG_BURST_LEN_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_CFG_BURST_LEN_MASK) | (((uint32_t)val) << IDMA_2D_CFG_BURST_LEN_SHIFT))

//====================================================================
//Register: Status register (status)
//====================================================================

#define  IDMA_2D_STATUS_RESERVED1_MASK 0xfffffe00
#define  IDMA_2D_STATUS_RESERVED1_SHIFT 9
#define  IDMA_2D_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_RESERVED1_MASK) >> IDMA_2D_STATUS_RESERVED1_SHIFT)
#define  IDMA_2D_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_RESERVED1_SHIFT))

#define  IDMA_2D_STATUS_SOI_MASK 0x100
#define  IDMA_2D_STATUS_SOI_SHIFT 8
#define  IDMA_2D_STATUS_SOI_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_SOI_MASK) >> IDMA_2D_STATUS_SOI_SHIFT)
#define  IDMA_2D_STATUS_SOI_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_SOI_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_SOI_SHIFT))

#define  IDMA_2D_STATUS_EOI_MASK 0x80
#define  IDMA_2D_STATUS_EOI_SHIFT 7
#define  IDMA_2D_STATUS_EOI_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_EOI_MASK) >> IDMA_2D_STATUS_EOI_SHIFT)
#define  IDMA_2D_STATUS_EOI_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_EOI_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_EOI_SHIFT))

#define  IDMA_2D_STATUS_SOFTRESET_MASK 0x40
#define  IDMA_2D_STATUS_SOFTRESET_SHIFT 6
#define  IDMA_2D_STATUS_SOFTRESET_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_SOFTRESET_MASK) >> IDMA_2D_STATUS_SOFTRESET_SHIFT)
#define  IDMA_2D_STATUS_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_SOFTRESET_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_SOFTRESET_SHIFT))

#define  IDMA_2D_STATUS_FULL_CBUF_MASK 0x20
#define  IDMA_2D_STATUS_FULL_CBUF_SHIFT 5
#define  IDMA_2D_STATUS_FULL_CBUF_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_FULL_CBUF_MASK) >> IDMA_2D_STATUS_FULL_CBUF_SHIFT)
#define  IDMA_2D_STATUS_FULL_CBUF_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_FULL_CBUF_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_FULL_CBUF_SHIFT))

#define  IDMA_2D_STATUS_EMPTY_CBUF_MASK 0x10
#define  IDMA_2D_STATUS_EMPTY_CBUF_SHIFT 4
#define  IDMA_2D_STATUS_EMPTY_CBUF_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_EMPTY_CBUF_MASK) >> IDMA_2D_STATUS_EMPTY_CBUF_SHIFT)
#define  IDMA_2D_STATUS_EMPTY_CBUF_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_EMPTY_CBUF_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_EMPTY_CBUF_SHIFT))

#define  IDMA_2D_STATUS_FULL_DBUF_MASK 0x8
#define  IDMA_2D_STATUS_FULL_DBUF_SHIFT 3
#define  IDMA_2D_STATUS_FULL_DBUF_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_FULL_DBUF_MASK) >> IDMA_2D_STATUS_FULL_DBUF_SHIFT)
#define  IDMA_2D_STATUS_FULL_DBUF_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_FULL_DBUF_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_FULL_DBUF_SHIFT))

#define  IDMA_2D_STATUS_EMPTY_DBUF_MASK 0x4
#define  IDMA_2D_STATUS_EMPTY_DBUF_SHIFT 2
#define  IDMA_2D_STATUS_EMPTY_DBUF_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_EMPTY_DBUF_MASK) >> IDMA_2D_STATUS_EMPTY_DBUF_SHIFT)
#define  IDMA_2D_STATUS_EMPTY_DBUF_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_EMPTY_DBUF_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_EMPTY_DBUF_SHIFT))

#define  IDMA_2D_STATUS_PACKER_EMPTY_MASK 0x2
#define  IDMA_2D_STATUS_PACKER_EMPTY_SHIFT 1
#define  IDMA_2D_STATUS_PACKER_EMPTY_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_PACKER_EMPTY_MASK) >> IDMA_2D_STATUS_PACKER_EMPTY_SHIFT)
#define  IDMA_2D_STATUS_PACKER_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_PACKER_EMPTY_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_PACKER_EMPTY_SHIFT))

#define  IDMA_2D_STATUS_DMA_BUSY_MASK 0x1
#define  IDMA_2D_STATUS_DMA_BUSY_SHIFT 0
#define  IDMA_2D_STATUS_DMA_BUSY_MASK_SHIFT(reg) (((reg) & IDMA_2D_STATUS_DMA_BUSY_MASK) >> IDMA_2D_STATUS_DMA_BUSY_SHIFT)
#define  IDMA_2D_STATUS_DMA_BUSY_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_STATUS_DMA_BUSY_MASK) | (((uint32_t)val) << IDMA_2D_STATUS_DMA_BUSY_SHIFT))

//====================================================================
//Register: Line Width register (line_width)
//====================================================================

#define  IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_MASK 0x80000000
#define  IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_SHIFT 31
#define  IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_MASK_SHIFT(reg) (((reg) & IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_MASK) >> IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_SHIFT)
#define  IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_MASK) | (((uint32_t)val) << IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_SHIFT))

#define  IDMA_2D_LINE_WIDTH_RESERVED1_MASK 0x70000000
#define  IDMA_2D_LINE_WIDTH_RESERVED1_SHIFT 28
#define  IDMA_2D_LINE_WIDTH_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_LINE_WIDTH_RESERVED1_MASK) >> IDMA_2D_LINE_WIDTH_RESERVED1_SHIFT)
#define  IDMA_2D_LINE_WIDTH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_LINE_WIDTH_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_LINE_WIDTH_RESERVED1_SHIFT))

#define  IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_MASK 0xff00000
#define  IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_SHIFT 20
#define  IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_MASK_SHIFT(reg) (((reg) & IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_MASK) >> IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_SHIFT)
#define  IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_MASK) | (((uint32_t)val) << IDMA_2D_LINE_WIDTH_REPEAT_LINE_CNT_SHIFT))

#define  IDMA_2D_LINE_WIDTH_LINE_WIDTH_MASK 0xfffff
#define  IDMA_2D_LINE_WIDTH_LINE_WIDTH_SHIFT 0
#define  IDMA_2D_LINE_WIDTH_LINE_WIDTH_MASK_SHIFT(reg) (((reg) & IDMA_2D_LINE_WIDTH_LINE_WIDTH_MASK) >> IDMA_2D_LINE_WIDTH_LINE_WIDTH_SHIFT)
#define  IDMA_2D_LINE_WIDTH_LINE_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_LINE_WIDTH_LINE_WIDTH_MASK) | (((uint32_t)val) << IDMA_2D_LINE_WIDTH_LINE_WIDTH_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  IDMA_2D_INT_EN_RESERVED1_MASK 0xffffffe0
#define  IDMA_2D_INT_EN_RESERVED1_SHIFT 5
#define  IDMA_2D_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_EN_RESERVED1_MASK) >> IDMA_2D_INT_EN_RESERVED1_SHIFT)
#define  IDMA_2D_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_INT_EN_RESERVED1_SHIFT))

#define  IDMA_2D_INT_EN_EOL_INT_EN_MASK 0x10
#define  IDMA_2D_INT_EN_EOL_INT_EN_SHIFT 4
#define  IDMA_2D_INT_EN_EOL_INT_EN_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_EN_EOL_INT_EN_MASK) >> IDMA_2D_INT_EN_EOL_INT_EN_SHIFT)
#define  IDMA_2D_INT_EN_EOL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_EN_EOL_INT_EN_MASK) | (((uint32_t)val) << IDMA_2D_INT_EN_EOL_INT_EN_SHIFT))

#define  IDMA_2D_INT_EN_RST_INT_EN_MASK 0x8
#define  IDMA_2D_INT_EN_RST_INT_EN_SHIFT 3
#define  IDMA_2D_INT_EN_RST_INT_EN_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_EN_RST_INT_EN_MASK) >> IDMA_2D_INT_EN_RST_INT_EN_SHIFT)
#define  IDMA_2D_INT_EN_RST_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_EN_RST_INT_EN_MASK) | (((uint32_t)val) << IDMA_2D_INT_EN_RST_INT_EN_SHIFT))

#define  IDMA_2D_INT_EN_EOI_INT_EN_MASK 0x4
#define  IDMA_2D_INT_EN_EOI_INT_EN_SHIFT 2
#define  IDMA_2D_INT_EN_EOI_INT_EN_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_EN_EOI_INT_EN_MASK) >> IDMA_2D_INT_EN_EOI_INT_EN_SHIFT)
#define  IDMA_2D_INT_EN_EOI_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_EN_EOI_INT_EN_MASK) | (((uint32_t)val) << IDMA_2D_INT_EN_EOI_INT_EN_SHIFT))

#define  IDMA_2D_INT_EN_FIN_INT_EN_MASK 0x2
#define  IDMA_2D_INT_EN_FIN_INT_EN_SHIFT 1
#define  IDMA_2D_INT_EN_FIN_INT_EN_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_EN_FIN_INT_EN_MASK) >> IDMA_2D_INT_EN_FIN_INT_EN_SHIFT)
#define  IDMA_2D_INT_EN_FIN_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_EN_FIN_INT_EN_MASK) | (((uint32_t)val) << IDMA_2D_INT_EN_FIN_INT_EN_SHIFT))

#define  IDMA_2D_INT_EN_WHO_INT_EN_MASK 0x1
#define  IDMA_2D_INT_EN_WHO_INT_EN_SHIFT 0
#define  IDMA_2D_INT_EN_WHO_INT_EN_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_EN_WHO_INT_EN_MASK) >> IDMA_2D_INT_EN_WHO_INT_EN_SHIFT)
#define  IDMA_2D_INT_EN_WHO_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_EN_WHO_INT_EN_MASK) | (((uint32_t)val) << IDMA_2D_INT_EN_WHO_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  IDMA_2D_INT_ST_RESERVED1_MASK 0xffffffe0
#define  IDMA_2D_INT_ST_RESERVED1_SHIFT 5
#define  IDMA_2D_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_ST_RESERVED1_MASK) >> IDMA_2D_INT_ST_RESERVED1_SHIFT)
#define  IDMA_2D_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_INT_ST_RESERVED1_SHIFT))

#define  IDMA_2D_INT_ST_EOL_INT_ST_MASK 0x10
#define  IDMA_2D_INT_ST_EOL_INT_ST_SHIFT 4
#define  IDMA_2D_INT_ST_EOL_INT_ST_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_ST_EOL_INT_ST_MASK) >> IDMA_2D_INT_ST_EOL_INT_ST_SHIFT)
#define  IDMA_2D_INT_ST_EOL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_ST_EOL_INT_ST_MASK) | (((uint32_t)val) << IDMA_2D_INT_ST_EOL_INT_ST_SHIFT))

#define  IDMA_2D_INT_ST_RST_INT_ST_MASK 0x8
#define  IDMA_2D_INT_ST_RST_INT_ST_SHIFT 3
#define  IDMA_2D_INT_ST_RST_INT_ST_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_ST_RST_INT_ST_MASK) >> IDMA_2D_INT_ST_RST_INT_ST_SHIFT)
#define  IDMA_2D_INT_ST_RST_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_ST_RST_INT_ST_MASK) | (((uint32_t)val) << IDMA_2D_INT_ST_RST_INT_ST_SHIFT))

#define  IDMA_2D_INT_ST_EOI_INT_ST_MASK 0x4
#define  IDMA_2D_INT_ST_EOI_INT_ST_SHIFT 2
#define  IDMA_2D_INT_ST_EOI_INT_ST_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_ST_EOI_INT_ST_MASK) >> IDMA_2D_INT_ST_EOI_INT_ST_SHIFT)
#define  IDMA_2D_INT_ST_EOI_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_ST_EOI_INT_ST_MASK) | (((uint32_t)val) << IDMA_2D_INT_ST_EOI_INT_ST_SHIFT))

#define  IDMA_2D_INT_ST_FIN_INT_ST_MASK 0x2
#define  IDMA_2D_INT_ST_FIN_INT_ST_SHIFT 1
#define  IDMA_2D_INT_ST_FIN_INT_ST_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_ST_FIN_INT_ST_MASK) >> IDMA_2D_INT_ST_FIN_INT_ST_SHIFT)
#define  IDMA_2D_INT_ST_FIN_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_ST_FIN_INT_ST_MASK) | (((uint32_t)val) << IDMA_2D_INT_ST_FIN_INT_ST_SHIFT))

#define  IDMA_2D_INT_ST_WHO_INT_ST_MASK 0x1
#define  IDMA_2D_INT_ST_WHO_INT_ST_SHIFT 0
#define  IDMA_2D_INT_ST_WHO_INT_ST_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_ST_WHO_INT_ST_MASK) >> IDMA_2D_INT_ST_WHO_INT_ST_SHIFT)
#define  IDMA_2D_INT_ST_WHO_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_ST_WHO_INT_ST_MASK) | (((uint32_t)val) << IDMA_2D_INT_ST_WHO_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Clear Register (int_cl)
//====================================================================

#define  IDMA_2D_INT_CL_RESERVED1_MASK 0xffffffe0
#define  IDMA_2D_INT_CL_RESERVED1_SHIFT 5
#define  IDMA_2D_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_CL_RESERVED1_MASK) >> IDMA_2D_INT_CL_RESERVED1_SHIFT)
#define  IDMA_2D_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_INT_CL_RESERVED1_SHIFT))

#define  IDMA_2D_INT_CL_EOL_INT_CL_MASK 0x10
#define  IDMA_2D_INT_CL_EOL_INT_CL_SHIFT 4
#define  IDMA_2D_INT_CL_EOL_INT_CL_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_CL_EOL_INT_CL_MASK) >> IDMA_2D_INT_CL_EOL_INT_CL_SHIFT)
#define  IDMA_2D_INT_CL_EOL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_CL_EOL_INT_CL_MASK) | (((uint32_t)val) << IDMA_2D_INT_CL_EOL_INT_CL_SHIFT))

#define  IDMA_2D_INT_CL_RST_INT_CL_MASK 0x8
#define  IDMA_2D_INT_CL_RST_INT_CL_SHIFT 3
#define  IDMA_2D_INT_CL_RST_INT_CL_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_CL_RST_INT_CL_MASK) >> IDMA_2D_INT_CL_RST_INT_CL_SHIFT)
#define  IDMA_2D_INT_CL_RST_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_CL_RST_INT_CL_MASK) | (((uint32_t)val) << IDMA_2D_INT_CL_RST_INT_CL_SHIFT))

#define  IDMA_2D_INT_CL_EOI_INT_CL_MASK 0x4
#define  IDMA_2D_INT_CL_EOI_INT_CL_SHIFT 2
#define  IDMA_2D_INT_CL_EOI_INT_CL_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_CL_EOI_INT_CL_MASK) >> IDMA_2D_INT_CL_EOI_INT_CL_SHIFT)
#define  IDMA_2D_INT_CL_EOI_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_CL_EOI_INT_CL_MASK) | (((uint32_t)val) << IDMA_2D_INT_CL_EOI_INT_CL_SHIFT))

#define  IDMA_2D_INT_CL_FIN_INT_CL_MASK 0x2
#define  IDMA_2D_INT_CL_FIN_INT_CL_SHIFT 1
#define  IDMA_2D_INT_CL_FIN_INT_CL_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_CL_FIN_INT_CL_MASK) >> IDMA_2D_INT_CL_FIN_INT_CL_SHIFT)
#define  IDMA_2D_INT_CL_FIN_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_CL_FIN_INT_CL_MASK) | (((uint32_t)val) << IDMA_2D_INT_CL_FIN_INT_CL_SHIFT))

#define  IDMA_2D_INT_CL_WHO_INT_CL_MASK 0x1
#define  IDMA_2D_INT_CL_WHO_INT_CL_SHIFT 0
#define  IDMA_2D_INT_CL_WHO_INT_CL_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_CL_WHO_INT_CL_MASK) >> IDMA_2D_INT_CL_WHO_INT_CL_SHIFT)
#define  IDMA_2D_INT_CL_WHO_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_CL_WHO_INT_CL_MASK) | (((uint32_t)val) << IDMA_2D_INT_CL_WHO_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  IDMA_2D_INT_FO_RESERVED1_MASK 0xffffffe0
#define  IDMA_2D_INT_FO_RESERVED1_SHIFT 5
#define  IDMA_2D_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_FO_RESERVED1_MASK) >> IDMA_2D_INT_FO_RESERVED1_SHIFT)
#define  IDMA_2D_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_INT_FO_RESERVED1_SHIFT))

#define  IDMA_2D_INT_FO_EOL_INT_FO_MASK 0x10
#define  IDMA_2D_INT_FO_EOL_INT_FO_SHIFT 4
#define  IDMA_2D_INT_FO_EOL_INT_FO_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_FO_EOL_INT_FO_MASK) >> IDMA_2D_INT_FO_EOL_INT_FO_SHIFT)
#define  IDMA_2D_INT_FO_EOL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_FO_EOL_INT_FO_MASK) | (((uint32_t)val) << IDMA_2D_INT_FO_EOL_INT_FO_SHIFT))

#define  IDMA_2D_INT_FO_RST_INT_FO_MASK 0x8
#define  IDMA_2D_INT_FO_RST_INT_FO_SHIFT 3
#define  IDMA_2D_INT_FO_RST_INT_FO_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_FO_RST_INT_FO_MASK) >> IDMA_2D_INT_FO_RST_INT_FO_SHIFT)
#define  IDMA_2D_INT_FO_RST_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_FO_RST_INT_FO_MASK) | (((uint32_t)val) << IDMA_2D_INT_FO_RST_INT_FO_SHIFT))

#define  IDMA_2D_INT_FO_EOI_INT_FO_MASK 0x4
#define  IDMA_2D_INT_FO_EOI_INT_FO_SHIFT 2
#define  IDMA_2D_INT_FO_EOI_INT_FO_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_FO_EOI_INT_FO_MASK) >> IDMA_2D_INT_FO_EOI_INT_FO_SHIFT)
#define  IDMA_2D_INT_FO_EOI_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_FO_EOI_INT_FO_MASK) | (((uint32_t)val) << IDMA_2D_INT_FO_EOI_INT_FO_SHIFT))

#define  IDMA_2D_INT_FO_FIN_INT_FO_MASK 0x2
#define  IDMA_2D_INT_FO_FIN_INT_FO_SHIFT 1
#define  IDMA_2D_INT_FO_FIN_INT_FO_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_FO_FIN_INT_FO_MASK) >> IDMA_2D_INT_FO_FIN_INT_FO_SHIFT)
#define  IDMA_2D_INT_FO_FIN_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_FO_FIN_INT_FO_MASK) | (((uint32_t)val) << IDMA_2D_INT_FO_FIN_INT_FO_SHIFT))

#define  IDMA_2D_INT_FO_WHO_INT_FO_MASK 0x1
#define  IDMA_2D_INT_FO_WHO_INT_FO_SHIFT 0
#define  IDMA_2D_INT_FO_WHO_INT_FO_MASK_SHIFT(reg) (((reg) & IDMA_2D_INT_FO_WHO_INT_FO_MASK) >> IDMA_2D_INT_FO_WHO_INT_FO_SHIFT)
#define  IDMA_2D_INT_FO_WHO_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_INT_FO_WHO_INT_FO_MASK) | (((uint32_t)val) << IDMA_2D_INT_FO_WHO_INT_FO_SHIFT))

//====================================================================
//Register: Descriptor Write Register (desc_write)
//====================================================================

#define  IDMA_2D_DESC_WRITE_DESC_MASK 0xffffffff
#define  IDMA_2D_DESC_WRITE_DESC_SHIFT 0
#define  IDMA_2D_DESC_WRITE_DESC_MASK_SHIFT(reg) (((reg) & IDMA_2D_DESC_WRITE_DESC_MASK) >> IDMA_2D_DESC_WRITE_DESC_SHIFT)
#define  IDMA_2D_DESC_WRITE_DESC_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_DESC_WRITE_DESC_MASK) | (((uint32_t)val) << IDMA_2D_DESC_WRITE_DESC_SHIFT))

//====================================================================
//Register: Descriptor Read Register (desc_read)
//====================================================================

#define  IDMA_2D_DESC_READ_DESC_MASK 0xffffffff
#define  IDMA_2D_DESC_READ_DESC_SHIFT 0
#define  IDMA_2D_DESC_READ_DESC_MASK_SHIFT(reg) (((reg) & IDMA_2D_DESC_READ_DESC_MASK) >> IDMA_2D_DESC_READ_DESC_SHIFT)
#define  IDMA_2D_DESC_READ_DESC_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_DESC_READ_DESC_MASK) | (((uint32_t)val) << IDMA_2D_DESC_READ_DESC_SHIFT))

//====================================================================
//Register: Descriptor Transfer Length Register (xfer_length)
//====================================================================

#define  IDMA_2D_XFER_LENGTH_LEN_MASK 0xffffffff
#define  IDMA_2D_XFER_LENGTH_LEN_SHIFT 0
#define  IDMA_2D_XFER_LENGTH_LEN_MASK_SHIFT(reg) (((reg) & IDMA_2D_XFER_LENGTH_LEN_MASK) >> IDMA_2D_XFER_LENGTH_LEN_SHIFT)
#define  IDMA_2D_XFER_LENGTH_LEN_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_XFER_LENGTH_LEN_MASK) | (((uint32_t)val) << IDMA_2D_XFER_LENGTH_LEN_SHIFT))

//====================================================================
//Register: Descriptor Transfer Address Register (xfer_addr)
//====================================================================

#define  IDMA_2D_XFER_ADDR_ADDR_MASK 0xffffffff
#define  IDMA_2D_XFER_ADDR_ADDR_SHIFT 0
#define  IDMA_2D_XFER_ADDR_ADDR_MASK_SHIFT(reg) (((reg) & IDMA_2D_XFER_ADDR_ADDR_MASK) >> IDMA_2D_XFER_ADDR_ADDR_SHIFT)
#define  IDMA_2D_XFER_ADDR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_XFER_ADDR_ADDR_MASK) | (((uint32_t)val) << IDMA_2D_XFER_ADDR_ADDR_SHIFT))

//====================================================================
//Register: Descriptor Control Word Register (ctrl_word)
//====================================================================

#define  IDMA_2D_CTRL_WORD_CTRL_WORD_MASK 0xffffffff
#define  IDMA_2D_CTRL_WORD_CTRL_WORD_SHIFT 0
#define  IDMA_2D_CTRL_WORD_CTRL_WORD_MASK_SHIFT(reg) (((reg) & IDMA_2D_CTRL_WORD_CTRL_WORD_MASK) >> IDMA_2D_CTRL_WORD_CTRL_WORD_SHIFT)
#define  IDMA_2D_CTRL_WORD_CTRL_WORD_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_CTRL_WORD_CTRL_WORD_MASK) | (((uint32_t)val) << IDMA_2D_CTRL_WORD_CTRL_WORD_SHIFT))

//====================================================================
//Register: Soft Reset register (reset)
//====================================================================

#define  IDMA_2D_RESET_RESERVED1_MASK 0xfffffffe
#define  IDMA_2D_RESET_RESERVED1_SHIFT 1
#define  IDMA_2D_RESET_RESERVED1_MASK_SHIFT(reg) (((reg) & IDMA_2D_RESET_RESERVED1_MASK) >> IDMA_2D_RESET_RESERVED1_SHIFT)
#define  IDMA_2D_RESET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_RESET_RESERVED1_MASK) | (((uint32_t)val) << IDMA_2D_RESET_RESERVED1_SHIFT))

#define  IDMA_2D_RESET_SOFT_RESET_MASK 0x1
#define  IDMA_2D_RESET_SOFT_RESET_SHIFT 0
#define  IDMA_2D_RESET_SOFT_RESET_MASK_SHIFT(reg) (((reg) & IDMA_2D_RESET_SOFT_RESET_MASK) >> IDMA_2D_RESET_SOFT_RESET_SHIFT)
#define  IDMA_2D_RESET_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~IDMA_2D_RESET_SOFT_RESET_MASK) | (((uint32_t)val) << IDMA_2D_RESET_SOFT_RESET_SHIFT))

#endif // IDMA_2D
