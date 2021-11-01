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
 * \file ODMA_AXI_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ODMA_REGMASKS_H_
#define _ODMA_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: ODMA (ODMA)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  ODMA_CFG_RESERVED1_MASK 0xff800000
#define  ODMA_CFG_RESERVED1_SHIFT 23
#define  ODMA_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_CFG_RESERVED1_MASK) >> ODMA_CFG_RESERVED1_SHIFT)
#define  ODMA_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_RESERVED1_MASK) | (((uint32_t)val) << ODMA_CFG_RESERVED1_SHIFT))

#define  ODMA_CFG_OWNWRITEDISABLE_MASK 0x400000
#define  ODMA_CFG_OWNWRITEDISABLE_SHIFT 22
#define  ODMA_CFG_OWNWRITEDISABLE_MASK_SHIFT(reg) (((reg) & ODMA_CFG_OWNWRITEDISABLE_MASK) >> ODMA_CFG_OWNWRITEDISABLE_SHIFT)
#define  ODMA_CFG_OWNWRITEDISABLE_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_OWNWRITEDISABLE_MASK) | (((uint32_t)val) << ODMA_CFG_OWNWRITEDISABLE_SHIFT))

#define  ODMA_CFG_SERPENTINE_MASK 0x200000
#define  ODMA_CFG_SERPENTINE_SHIFT 21
#define  ODMA_CFG_SERPENTINE_MASK_SHIFT(reg) (((reg) & ODMA_CFG_SERPENTINE_MASK) >> ODMA_CFG_SERPENTINE_SHIFT)
#define  ODMA_CFG_SERPENTINE_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_SERPENTINE_MASK) | (((uint32_t)val) << ODMA_CFG_SERPENTINE_SHIFT))

#define  ODMA_CFG_LINE_REV_MASK 0x100000
#define  ODMA_CFG_LINE_REV_SHIFT 20
#define  ODMA_CFG_LINE_REV_MASK_SHIFT(reg) (((reg) & ODMA_CFG_LINE_REV_MASK) >> ODMA_CFG_LINE_REV_SHIFT)
#define  ODMA_CFG_LINE_REV_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_LINE_REV_MASK) | (((uint32_t)val) << ODMA_CFG_LINE_REV_SHIFT))

#define  ODMA_CFG_TRAN_REV_MASK 0x80000
#define  ODMA_CFG_TRAN_REV_SHIFT 19
#define  ODMA_CFG_TRAN_REV_MASK_SHIFT(reg) (((reg) & ODMA_CFG_TRAN_REV_MASK) >> ODMA_CFG_TRAN_REV_SHIFT)
#define  ODMA_CFG_TRAN_REV_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_TRAN_REV_MASK) | (((uint32_t)val) << ODMA_CFG_TRAN_REV_SHIFT))

#define  ODMA_CFG_UPPER_HALF_MASK 0x40000
#define  ODMA_CFG_UPPER_HALF_SHIFT 18
#define  ODMA_CFG_UPPER_HALF_MASK_SHIFT(reg) (((reg) & ODMA_CFG_UPPER_HALF_MASK) >> ODMA_CFG_UPPER_HALF_SHIFT)
#define  ODMA_CFG_UPPER_HALF_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_UPPER_HALF_MASK) | (((uint32_t)val) << ODMA_CFG_UPPER_HALF_SHIFT))

#define  ODMA_CFG_MSB_IN_MASK 0x20000
#define  ODMA_CFG_MSB_IN_SHIFT 17
#define  ODMA_CFG_MSB_IN_MASK_SHIFT(reg) (((reg) & ODMA_CFG_MSB_IN_MASK) >> ODMA_CFG_MSB_IN_SHIFT)
#define  ODMA_CFG_MSB_IN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_MSB_IN_MASK) | (((uint32_t)val) << ODMA_CFG_MSB_IN_SHIFT))

#define  ODMA_CFG_ENABLE_MASK 0x10000
#define  ODMA_CFG_ENABLE_SHIFT 16
#define  ODMA_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & ODMA_CFG_ENABLE_MASK) >> ODMA_CFG_ENABLE_SHIFT)
#define  ODMA_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_ENABLE_MASK) | (((uint32_t)val) << ODMA_CFG_ENABLE_SHIFT))

#define  ODMA_CFG_RESERVED2_MASK 0xf800
#define  ODMA_CFG_RESERVED2_SHIFT 11
#define  ODMA_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & ODMA_CFG_RESERVED2_MASK) >> ODMA_CFG_RESERVED2_SHIFT)
#define  ODMA_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_RESERVED2_MASK) | (((uint32_t)val) << ODMA_CFG_RESERVED2_SHIFT))

#define  ODMA_CFG_IN_WIDTH_MASK 0x700
#define  ODMA_CFG_IN_WIDTH_SHIFT 8
#define  ODMA_CFG_IN_WIDTH_MASK_SHIFT(reg) (((reg) & ODMA_CFG_IN_WIDTH_MASK) >> ODMA_CFG_IN_WIDTH_SHIFT)
#define  ODMA_CFG_IN_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_IN_WIDTH_MASK) | (((uint32_t)val) << ODMA_CFG_IN_WIDTH_SHIFT))

#define  ODMA_CFG_RESERVED3_MASK 0xfc
#define  ODMA_CFG_RESERVED3_SHIFT 2
#define  ODMA_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & ODMA_CFG_RESERVED3_MASK) >> ODMA_CFG_RESERVED3_SHIFT)
#define  ODMA_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_RESERVED3_MASK) | (((uint32_t)val) << ODMA_CFG_RESERVED3_SHIFT))

#define  ODMA_CFG_BURST_LEN_MASK 0x3
#define  ODMA_CFG_BURST_LEN_SHIFT 0
#define  ODMA_CFG_BURST_LEN_MASK_SHIFT(reg) (((reg) & ODMA_CFG_BURST_LEN_MASK) >> ODMA_CFG_BURST_LEN_SHIFT)
#define  ODMA_CFG_BURST_LEN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_CFG_BURST_LEN_MASK) | (((uint32_t)val) << ODMA_CFG_BURST_LEN_SHIFT))

//====================================================================
//Register: Status register (status)
//====================================================================

#define  ODMA_STATUS_RESERVED1_MASK 0xfffff800
#define  ODMA_STATUS_RESERVED1_SHIFT 11
#define  ODMA_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_RESERVED1_MASK) >> ODMA_STATUS_RESERVED1_SHIFT)
#define  ODMA_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_RESERVED1_MASK) | (((uint32_t)val) << ODMA_STATUS_RESERVED1_SHIFT))

#define  ODMA_STATUS_DESC_OWN_MASK 0x400
#define  ODMA_STATUS_DESC_OWN_SHIFT 10
#define  ODMA_STATUS_DESC_OWN_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_DESC_OWN_MASK) >> ODMA_STATUS_DESC_OWN_SHIFT)
#define  ODMA_STATUS_DESC_OWN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_DESC_OWN_MASK) | (((uint32_t)val) << ODMA_STATUS_DESC_OWN_SHIFT))

#define  ODMA_STATUS_DESC_EOI_MASK 0x200
#define  ODMA_STATUS_DESC_EOI_SHIFT 9
#define  ODMA_STATUS_DESC_EOI_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_DESC_EOI_MASK) >> ODMA_STATUS_DESC_EOI_SHIFT)
#define  ODMA_STATUS_DESC_EOI_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_DESC_EOI_MASK) | (((uint32_t)val) << ODMA_STATUS_DESC_EOI_SHIFT))

#define  ODMA_STATUS_DESC_SOI_MASK 0x100
#define  ODMA_STATUS_DESC_SOI_SHIFT 8
#define  ODMA_STATUS_DESC_SOI_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_DESC_SOI_MASK) >> ODMA_STATUS_DESC_SOI_SHIFT)
#define  ODMA_STATUS_DESC_SOI_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_DESC_SOI_MASK) | (((uint32_t)val) << ODMA_STATUS_DESC_SOI_SHIFT))

#define  ODMA_STATUS_REVERSE_MASK 0x80
#define  ODMA_STATUS_REVERSE_SHIFT 7
#define  ODMA_STATUS_REVERSE_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_REVERSE_MASK) >> ODMA_STATUS_REVERSE_SHIFT)
#define  ODMA_STATUS_REVERSE_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_REVERSE_MASK) | (((uint32_t)val) << ODMA_STATUS_REVERSE_SHIFT))

#define  ODMA_STATUS_SOFTRESET_MASK 0x40
#define  ODMA_STATUS_SOFTRESET_SHIFT 6
#define  ODMA_STATUS_SOFTRESET_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_SOFTRESET_MASK) >> ODMA_STATUS_SOFTRESET_SHIFT)
#define  ODMA_STATUS_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_SOFTRESET_MASK) | (((uint32_t)val) << ODMA_STATUS_SOFTRESET_SHIFT))

#define  ODMA_STATUS_FULL_CBUF_MASK 0x20
#define  ODMA_STATUS_FULL_CBUF_SHIFT 5
#define  ODMA_STATUS_FULL_CBUF_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_FULL_CBUF_MASK) >> ODMA_STATUS_FULL_CBUF_SHIFT)
#define  ODMA_STATUS_FULL_CBUF_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_FULL_CBUF_MASK) | (((uint32_t)val) << ODMA_STATUS_FULL_CBUF_SHIFT))

#define  ODMA_STATUS_EMPTY_CBUF_MASK 0x10
#define  ODMA_STATUS_EMPTY_CBUF_SHIFT 4
#define  ODMA_STATUS_EMPTY_CBUF_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_EMPTY_CBUF_MASK) >> ODMA_STATUS_EMPTY_CBUF_SHIFT)
#define  ODMA_STATUS_EMPTY_CBUF_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_EMPTY_CBUF_MASK) | (((uint32_t)val) << ODMA_STATUS_EMPTY_CBUF_SHIFT))

#define  ODMA_STATUS_FULL_DBUF_MASK 0x8
#define  ODMA_STATUS_FULL_DBUF_SHIFT 3
#define  ODMA_STATUS_FULL_DBUF_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_FULL_DBUF_MASK) >> ODMA_STATUS_FULL_DBUF_SHIFT)
#define  ODMA_STATUS_FULL_DBUF_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_FULL_DBUF_MASK) | (((uint32_t)val) << ODMA_STATUS_FULL_DBUF_SHIFT))

#define  ODMA_STATUS_EMPTY_DBUF_MASK 0x4
#define  ODMA_STATUS_EMPTY_DBUF_SHIFT 2
#define  ODMA_STATUS_EMPTY_DBUF_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_EMPTY_DBUF_MASK) >> ODMA_STATUS_EMPTY_DBUF_SHIFT)
#define  ODMA_STATUS_EMPTY_DBUF_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_EMPTY_DBUF_MASK) | (((uint32_t)val) << ODMA_STATUS_EMPTY_DBUF_SHIFT))

#define  ODMA_STATUS_PACKER_EMPTY_MASK 0x2
#define  ODMA_STATUS_PACKER_EMPTY_SHIFT 1
#define  ODMA_STATUS_PACKER_EMPTY_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_PACKER_EMPTY_MASK) >> ODMA_STATUS_PACKER_EMPTY_SHIFT)
#define  ODMA_STATUS_PACKER_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_PACKER_EMPTY_MASK) | (((uint32_t)val) << ODMA_STATUS_PACKER_EMPTY_SHIFT))

#define  ODMA_STATUS_DMA_BUSY_MASK 0x1
#define  ODMA_STATUS_DMA_BUSY_SHIFT 0
#define  ODMA_STATUS_DMA_BUSY_MASK_SHIFT(reg) (((reg) & ODMA_STATUS_DMA_BUSY_MASK) >> ODMA_STATUS_DMA_BUSY_SHIFT)
#define  ODMA_STATUS_DMA_BUSY_REPLACE_VAL(reg,val) (((reg) & ~ODMA_STATUS_DMA_BUSY_MASK) | (((uint32_t)val) << ODMA_STATUS_DMA_BUSY_SHIFT))

//====================================================================
//Register: Line Size Register (line_size)
//====================================================================

#define  ODMA_LINE_SIZE_RESERVED1_MASK 0xfffc0000
#define  ODMA_LINE_SIZE_RESERVED1_SHIFT 18
#define  ODMA_LINE_SIZE_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_LINE_SIZE_RESERVED1_MASK) >> ODMA_LINE_SIZE_RESERVED1_SHIFT)
#define  ODMA_LINE_SIZE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_LINE_SIZE_RESERVED1_MASK) | (((uint32_t)val) << ODMA_LINE_SIZE_RESERVED1_SHIFT))

#define  ODMA_LINE_SIZE_LINE_SIZE_MASK 0x3ffff
#define  ODMA_LINE_SIZE_LINE_SIZE_SHIFT 0
#define  ODMA_LINE_SIZE_LINE_SIZE_MASK_SHIFT(reg) (((reg) & ODMA_LINE_SIZE_LINE_SIZE_MASK) >> ODMA_LINE_SIZE_LINE_SIZE_SHIFT)
#define  ODMA_LINE_SIZE_LINE_SIZE_REPLACE_VAL(reg,val) (((reg) & ~ODMA_LINE_SIZE_LINE_SIZE_MASK) | (((uint32_t)val) << ODMA_LINE_SIZE_LINE_SIZE_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  ODMA_INT_EN_RESERVED1_MASK 0xfffff800
#define  ODMA_INT_EN_RESERVED1_SHIFT 11
#define  ODMA_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_RESERVED1_MASK) >> ODMA_INT_EN_RESERVED1_SHIFT)
#define  ODMA_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << ODMA_INT_EN_RESERVED1_SHIFT))

#define  ODMA_INT_EN_RRESP_INT_EN_MASK 0x400
#define  ODMA_INT_EN_RRESP_INT_EN_SHIFT 10
#define  ODMA_INT_EN_RRESP_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_RRESP_INT_EN_MASK) >> ODMA_INT_EN_RRESP_INT_EN_SHIFT)
#define  ODMA_INT_EN_RRESP_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_RRESP_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_RRESP_INT_EN_SHIFT))

#define  ODMA_INT_EN_BRESP_INT_EN_MASK 0x200
#define  ODMA_INT_EN_BRESP_INT_EN_SHIFT 9
#define  ODMA_INT_EN_BRESP_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_BRESP_INT_EN_MASK) >> ODMA_INT_EN_BRESP_INT_EN_SHIFT)
#define  ODMA_INT_EN_BRESP_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_BRESP_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_BRESP_INT_EN_SHIFT))

#define  ODMA_INT_EN_RST_INT_EN_MASK 0x100
#define  ODMA_INT_EN_RST_INT_EN_SHIFT 8
#define  ODMA_INT_EN_RST_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_RST_INT_EN_MASK) >> ODMA_INT_EN_RST_INT_EN_SHIFT)
#define  ODMA_INT_EN_RST_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_RST_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_RST_INT_EN_SHIFT))

#define  ODMA_INT_EN_EOI_INT_EN_MASK 0x80
#define  ODMA_INT_EN_EOI_INT_EN_SHIFT 7
#define  ODMA_INT_EN_EOI_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_EOI_INT_EN_MASK) >> ODMA_INT_EN_EOI_INT_EN_SHIFT)
#define  ODMA_INT_EN_EOI_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_EOI_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_EOI_INT_EN_SHIFT))

#define  ODMA_INT_EN_DIR_INT_EN_MASK 0x40
#define  ODMA_INT_EN_DIR_INT_EN_SHIFT 6
#define  ODMA_INT_EN_DIR_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_DIR_INT_EN_MASK) >> ODMA_INT_EN_DIR_INT_EN_SHIFT)
#define  ODMA_INT_EN_DIR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_DIR_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_DIR_INT_EN_SHIFT))

#define  ODMA_INT_EN_CL_ALI_INT_EN_MASK 0x20
#define  ODMA_INT_EN_CL_ALI_INT_EN_SHIFT 5
#define  ODMA_INT_EN_CL_ALI_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_CL_ALI_INT_EN_MASK) >> ODMA_INT_EN_CL_ALI_INT_EN_SHIFT)
#define  ODMA_INT_EN_CL_ALI_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_CL_ALI_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_CL_ALI_INT_EN_SHIFT))

#define  ODMA_INT_EN_EOL_ALI_INT_EN_MASK 0x10
#define  ODMA_INT_EN_EOL_ALI_INT_EN_SHIFT 4
#define  ODMA_INT_EN_EOL_ALI_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_EOL_ALI_INT_EN_MASK) >> ODMA_INT_EN_EOL_ALI_INT_EN_SHIFT)
#define  ODMA_INT_EN_EOL_ALI_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_EOL_ALI_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_EOL_ALI_INT_EN_SHIFT))

#define  ODMA_INT_EN_EOI_ALI_INT_EN_MASK 0x8
#define  ODMA_INT_EN_EOI_ALI_INT_EN_SHIFT 3
#define  ODMA_INT_EN_EOI_ALI_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_EOI_ALI_INT_EN_MASK) >> ODMA_INT_EN_EOI_ALI_INT_EN_SHIFT)
#define  ODMA_INT_EN_EOI_ALI_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_EOI_ALI_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_EOI_ALI_INT_EN_SHIFT))

#define  ODMA_INT_EN_EOI_ERR_INT_EN_MASK 0x4
#define  ODMA_INT_EN_EOI_ERR_INT_EN_SHIFT 2
#define  ODMA_INT_EN_EOI_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_EOI_ERR_INT_EN_MASK) >> ODMA_INT_EN_EOI_ERR_INT_EN_SHIFT)
#define  ODMA_INT_EN_EOI_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_EOI_ERR_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_EOI_ERR_INT_EN_SHIFT))

#define  ODMA_INT_EN_FIN_INT_EN_MASK 0x2
#define  ODMA_INT_EN_FIN_INT_EN_SHIFT 1
#define  ODMA_INT_EN_FIN_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_FIN_INT_EN_MASK) >> ODMA_INT_EN_FIN_INT_EN_SHIFT)
#define  ODMA_INT_EN_FIN_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_FIN_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_FIN_INT_EN_SHIFT))

#define  ODMA_INT_EN_WHO_INT_EN_MASK 0x1
#define  ODMA_INT_EN_WHO_INT_EN_SHIFT 0
#define  ODMA_INT_EN_WHO_INT_EN_MASK_SHIFT(reg) (((reg) & ODMA_INT_EN_WHO_INT_EN_MASK) >> ODMA_INT_EN_WHO_INT_EN_SHIFT)
#define  ODMA_INT_EN_WHO_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_EN_WHO_INT_EN_MASK) | (((uint32_t)val) << ODMA_INT_EN_WHO_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_pend)
//====================================================================

#define  ODMA_INT_PEND_RESERVED1_MASK 0xfffff800
#define  ODMA_INT_PEND_RESERVED1_SHIFT 11
#define  ODMA_INT_PEND_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_RESERVED1_MASK) >> ODMA_INT_PEND_RESERVED1_SHIFT)
#define  ODMA_INT_PEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_RESERVED1_MASK) | (((uint32_t)val) << ODMA_INT_PEND_RESERVED1_SHIFT))

#define  ODMA_INT_PEND_RRESP_INT_PEND_MASK 0x400
#define  ODMA_INT_PEND_RRESP_INT_PEND_SHIFT 10
#define  ODMA_INT_PEND_RRESP_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_RRESP_INT_PEND_MASK) >> ODMA_INT_PEND_RRESP_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_RRESP_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_RRESP_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_RRESP_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_BRESP_INT_PEND_MASK 0x200
#define  ODMA_INT_PEND_BRESP_INT_PEND_SHIFT 9
#define  ODMA_INT_PEND_BRESP_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_BRESP_INT_PEND_MASK) >> ODMA_INT_PEND_BRESP_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_BRESP_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_BRESP_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_BRESP_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_RST_INT_PEND_MASK 0x100
#define  ODMA_INT_PEND_RST_INT_PEND_SHIFT 8
#define  ODMA_INT_PEND_RST_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_RST_INT_PEND_MASK) >> ODMA_INT_PEND_RST_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_RST_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_RST_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_RST_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_EOI_INT_PEND_MASK 0x80
#define  ODMA_INT_PEND_EOI_INT_PEND_SHIFT 7
#define  ODMA_INT_PEND_EOI_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_EOI_INT_PEND_MASK) >> ODMA_INT_PEND_EOI_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_EOI_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_EOI_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_EOI_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_DIR_INT_PEND_MASK 0x40
#define  ODMA_INT_PEND_DIR_INT_PEND_SHIFT 6
#define  ODMA_INT_PEND_DIR_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_DIR_INT_PEND_MASK) >> ODMA_INT_PEND_DIR_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_DIR_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_DIR_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_DIR_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_CL_ALI_INT_PEND_MASK 0x20
#define  ODMA_INT_PEND_CL_ALI_INT_PEND_SHIFT 5
#define  ODMA_INT_PEND_CL_ALI_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_CL_ALI_INT_PEND_MASK) >> ODMA_INT_PEND_CL_ALI_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_CL_ALI_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_CL_ALI_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_CL_ALI_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_EOL_ALI_INT_PEND_MASK 0x10
#define  ODMA_INT_PEND_EOL_ALI_INT_PEND_SHIFT 4
#define  ODMA_INT_PEND_EOL_ALI_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_EOL_ALI_INT_PEND_MASK) >> ODMA_INT_PEND_EOL_ALI_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_EOL_ALI_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_EOL_ALI_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_EOL_ALI_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_EOI_ALI_INT_PEND_MASK 0x8
#define  ODMA_INT_PEND_EOI_ALI_INT_PEND_SHIFT 3
#define  ODMA_INT_PEND_EOI_ALI_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_EOI_ALI_INT_PEND_MASK) >> ODMA_INT_PEND_EOI_ALI_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_EOI_ALI_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_EOI_ALI_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_EOI_ALI_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_EOI_ERR_INT_PEND_MASK 0x4
#define  ODMA_INT_PEND_EOI_ERR_INT_PEND_SHIFT 2
#define  ODMA_INT_PEND_EOI_ERR_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_EOI_ERR_INT_PEND_MASK) >> ODMA_INT_PEND_EOI_ERR_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_EOI_ERR_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_EOI_ERR_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_EOI_ERR_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_FIN_INT_PEND_MASK 0x2
#define  ODMA_INT_PEND_FIN_INT_PEND_SHIFT 1
#define  ODMA_INT_PEND_FIN_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_FIN_INT_PEND_MASK) >> ODMA_INT_PEND_FIN_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_FIN_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_FIN_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_FIN_INT_PEND_SHIFT))

#define  ODMA_INT_PEND_WHO_INT_PEND_MASK 0x1
#define  ODMA_INT_PEND_WHO_INT_PEND_SHIFT 0
#define  ODMA_INT_PEND_WHO_INT_PEND_MASK_SHIFT(reg) (((reg) & ODMA_INT_PEND_WHO_INT_PEND_MASK) >> ODMA_INT_PEND_WHO_INT_PEND_SHIFT)
#define  ODMA_INT_PEND_WHO_INT_PEND_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_PEND_WHO_INT_PEND_MASK) | (((uint32_t)val) << ODMA_INT_PEND_WHO_INT_PEND_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_ack)
//====================================================================

#define  ODMA_INT_ACK_RESERVED1_MASK 0xfffff800
#define  ODMA_INT_ACK_RESERVED1_SHIFT 11
#define  ODMA_INT_ACK_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_RESERVED1_MASK) >> ODMA_INT_ACK_RESERVED1_SHIFT)
#define  ODMA_INT_ACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_RESERVED1_MASK) | (((uint32_t)val) << ODMA_INT_ACK_RESERVED1_SHIFT))

#define  ODMA_INT_ACK_RRESP_INT_ACK_MASK 0x400
#define  ODMA_INT_ACK_RRESP_INT_ACK_SHIFT 10
#define  ODMA_INT_ACK_RRESP_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_RRESP_INT_ACK_MASK) >> ODMA_INT_ACK_RRESP_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_RRESP_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_RRESP_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_RRESP_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_BRESP_INT_ACK_MASK 0x200
#define  ODMA_INT_ACK_BRESP_INT_ACK_SHIFT 9
#define  ODMA_INT_ACK_BRESP_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_BRESP_INT_ACK_MASK) >> ODMA_INT_ACK_BRESP_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_BRESP_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_BRESP_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_BRESP_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_RST_INT_ACK_MASK 0x100
#define  ODMA_INT_ACK_RST_INT_ACK_SHIFT 8
#define  ODMA_INT_ACK_RST_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_RST_INT_ACK_MASK) >> ODMA_INT_ACK_RST_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_RST_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_RST_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_RST_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_EOI_INT_ACK_MASK 0x80
#define  ODMA_INT_ACK_EOI_INT_ACK_SHIFT 7
#define  ODMA_INT_ACK_EOI_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_EOI_INT_ACK_MASK) >> ODMA_INT_ACK_EOI_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_EOI_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_EOI_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_EOI_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_DIR_INT_ACK_MASK 0x40
#define  ODMA_INT_ACK_DIR_INT_ACK_SHIFT 6
#define  ODMA_INT_ACK_DIR_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_DIR_INT_ACK_MASK) >> ODMA_INT_ACK_DIR_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_DIR_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_DIR_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_DIR_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_CL_ALI_INT_ACK_MASK 0x20
#define  ODMA_INT_ACK_CL_ALI_INT_ACK_SHIFT 5
#define  ODMA_INT_ACK_CL_ALI_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_CL_ALI_INT_ACK_MASK) >> ODMA_INT_ACK_CL_ALI_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_CL_ALI_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_CL_ALI_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_CL_ALI_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_EOL_ALI_INT_ACK_MASK 0x10
#define  ODMA_INT_ACK_EOL_ALI_INT_ACK_SHIFT 4
#define  ODMA_INT_ACK_EOL_ALI_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_EOL_ALI_INT_ACK_MASK) >> ODMA_INT_ACK_EOL_ALI_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_EOL_ALI_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_EOL_ALI_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_EOL_ALI_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_EOI_ALI_INT_ACK_MASK 0x8
#define  ODMA_INT_ACK_EOI_ALI_INT_ACK_SHIFT 3
#define  ODMA_INT_ACK_EOI_ALI_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_EOI_ALI_INT_ACK_MASK) >> ODMA_INT_ACK_EOI_ALI_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_EOI_ALI_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_EOI_ALI_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_EOI_ALI_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_EOI_ERR_INT_ACK_MASK 0x4
#define  ODMA_INT_ACK_EOI_ERR_INT_ACK_SHIFT 2
#define  ODMA_INT_ACK_EOI_ERR_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_EOI_ERR_INT_ACK_MASK) >> ODMA_INT_ACK_EOI_ERR_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_EOI_ERR_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_EOI_ERR_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_EOI_ERR_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_FIN_INT_ACK_MASK 0x2
#define  ODMA_INT_ACK_FIN_INT_ACK_SHIFT 1
#define  ODMA_INT_ACK_FIN_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_FIN_INT_ACK_MASK) >> ODMA_INT_ACK_FIN_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_FIN_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_FIN_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_FIN_INT_ACK_SHIFT))

#define  ODMA_INT_ACK_WHO_INT_ACK_MASK 0x1
#define  ODMA_INT_ACK_WHO_INT_ACK_SHIFT 0
#define  ODMA_INT_ACK_WHO_INT_ACK_MASK_SHIFT(reg) (((reg) & ODMA_INT_ACK_WHO_INT_ACK_MASK) >> ODMA_INT_ACK_WHO_INT_ACK_SHIFT)
#define  ODMA_INT_ACK_WHO_INT_ACK_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_ACK_WHO_INT_ACK_MASK) | (((uint32_t)val) << ODMA_INT_ACK_WHO_INT_ACK_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  ODMA_INT_FO_RESERVED1_MASK 0xfffff800
#define  ODMA_INT_FO_RESERVED1_SHIFT 11
#define  ODMA_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_RESERVED1_MASK) >> ODMA_INT_FO_RESERVED1_SHIFT)
#define  ODMA_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << ODMA_INT_FO_RESERVED1_SHIFT))

#define  ODMA_INT_FO_RRESP_INT_FO_MASK 0x400
#define  ODMA_INT_FO_RRESP_INT_FO_SHIFT 10
#define  ODMA_INT_FO_RRESP_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_RRESP_INT_FO_MASK) >> ODMA_INT_FO_RRESP_INT_FO_SHIFT)
#define  ODMA_INT_FO_RRESP_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_RRESP_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_RRESP_INT_FO_SHIFT))

#define  ODMA_INT_FO_BRESP_INT_FO_MASK 0x200
#define  ODMA_INT_FO_BRESP_INT_FO_SHIFT 9
#define  ODMA_INT_FO_BRESP_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_BRESP_INT_FO_MASK) >> ODMA_INT_FO_BRESP_INT_FO_SHIFT)
#define  ODMA_INT_FO_BRESP_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_BRESP_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_BRESP_INT_FO_SHIFT))

#define  ODMA_INT_FO_RST_INT_FO_MASK 0x100
#define  ODMA_INT_FO_RST_INT_FO_SHIFT 8
#define  ODMA_INT_FO_RST_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_RST_INT_FO_MASK) >> ODMA_INT_FO_RST_INT_FO_SHIFT)
#define  ODMA_INT_FO_RST_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_RST_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_RST_INT_FO_SHIFT))

#define  ODMA_INT_FO_EOI_INT_FO_MASK 0x80
#define  ODMA_INT_FO_EOI_INT_FO_SHIFT 7
#define  ODMA_INT_FO_EOI_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_EOI_INT_FO_MASK) >> ODMA_INT_FO_EOI_INT_FO_SHIFT)
#define  ODMA_INT_FO_EOI_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_EOI_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_EOI_INT_FO_SHIFT))

#define  ODMA_INT_FO_DIR_INT_FO_MASK 0x40
#define  ODMA_INT_FO_DIR_INT_FO_SHIFT 6
#define  ODMA_INT_FO_DIR_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_DIR_INT_FO_MASK) >> ODMA_INT_FO_DIR_INT_FO_SHIFT)
#define  ODMA_INT_FO_DIR_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_DIR_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_DIR_INT_FO_SHIFT))

#define  ODMA_INT_FO_CL_ALI_INT_FO_MASK 0x20
#define  ODMA_INT_FO_CL_ALI_INT_FO_SHIFT 5
#define  ODMA_INT_FO_CL_ALI_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_CL_ALI_INT_FO_MASK) >> ODMA_INT_FO_CL_ALI_INT_FO_SHIFT)
#define  ODMA_INT_FO_CL_ALI_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_CL_ALI_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_CL_ALI_INT_FO_SHIFT))

#define  ODMA_INT_FO_EOL_ALI_INT_FO_MASK 0x10
#define  ODMA_INT_FO_EOL_ALI_INT_FO_SHIFT 4
#define  ODMA_INT_FO_EOL_ALI_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_EOL_ALI_INT_FO_MASK) >> ODMA_INT_FO_EOL_ALI_INT_FO_SHIFT)
#define  ODMA_INT_FO_EOL_ALI_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_EOL_ALI_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_EOL_ALI_INT_FO_SHIFT))

#define  ODMA_INT_FO_EOI_ALI_INT_FO_MASK 0x8
#define  ODMA_INT_FO_EOI_ALI_INT_FO_SHIFT 3
#define  ODMA_INT_FO_EOI_ALI_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_EOI_ALI_INT_FO_MASK) >> ODMA_INT_FO_EOI_ALI_INT_FO_SHIFT)
#define  ODMA_INT_FO_EOI_ALI_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_EOI_ALI_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_EOI_ALI_INT_FO_SHIFT))

#define  ODMA_INT_FO_EOI_ERR_INT_FO_MASK 0x4
#define  ODMA_INT_FO_EOI_ERR_INT_FO_SHIFT 2
#define  ODMA_INT_FO_EOI_ERR_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_EOI_ERR_INT_FO_MASK) >> ODMA_INT_FO_EOI_ERR_INT_FO_SHIFT)
#define  ODMA_INT_FO_EOI_ERR_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_EOI_ERR_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_EOI_ERR_INT_FO_SHIFT))

#define  ODMA_INT_FO_FIN_INT_FO_MASK 0x2
#define  ODMA_INT_FO_FIN_INT_FO_SHIFT 1
#define  ODMA_INT_FO_FIN_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_FIN_INT_FO_MASK) >> ODMA_INT_FO_FIN_INT_FO_SHIFT)
#define  ODMA_INT_FO_FIN_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_FIN_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_FIN_INT_FO_SHIFT))

#define  ODMA_INT_FO_WHO_INT_FO_MASK 0x1
#define  ODMA_INT_FO_WHO_INT_FO_SHIFT 0
#define  ODMA_INT_FO_WHO_INT_FO_MASK_SHIFT(reg) (((reg) & ODMA_INT_FO_WHO_INT_FO_MASK) >> ODMA_INT_FO_WHO_INT_FO_SHIFT)
#define  ODMA_INT_FO_WHO_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~ODMA_INT_FO_WHO_INT_FO_MASK) | (((uint32_t)val) << ODMA_INT_FO_WHO_INT_FO_SHIFT))

//====================================================================
//Register: Descriptor Write Register (desc_write)
//====================================================================

#define  ODMA_DESC_WRITE_DESC_MASK 0xfffffffc
#define  ODMA_DESC_WRITE_DESC_SHIFT 2
#define  ODMA_DESC_WRITE_DESC_MASK_SHIFT(reg) (((reg) & ODMA_DESC_WRITE_DESC_MASK) >> ODMA_DESC_WRITE_DESC_SHIFT)
#define  ODMA_DESC_WRITE_DESC_REPLACE_VAL(reg,val) (((reg) & ~ODMA_DESC_WRITE_DESC_MASK) | (((uint32_t)val) << ODMA_DESC_WRITE_DESC_SHIFT))

#define  ODMA_DESC_WRITE_RESERVED1_MASK 0x3
#define  ODMA_DESC_WRITE_RESERVED1_SHIFT 0
#define  ODMA_DESC_WRITE_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_DESC_WRITE_RESERVED1_MASK) >> ODMA_DESC_WRITE_RESERVED1_SHIFT)
#define  ODMA_DESC_WRITE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_DESC_WRITE_RESERVED1_MASK) | (((uint32_t)val) << ODMA_DESC_WRITE_RESERVED1_SHIFT))

//====================================================================
//Register: Descriptor Read Register (desc_read)
//====================================================================

#define  ODMA_DESC_READ_DESC_MASK 0xffffffff
#define  ODMA_DESC_READ_DESC_SHIFT 0
#define  ODMA_DESC_READ_DESC_MASK_SHIFT(reg) (((reg) & ODMA_DESC_READ_DESC_MASK) >> ODMA_DESC_READ_DESC_SHIFT)
#define  ODMA_DESC_READ_DESC_REPLACE_VAL(reg,val) (((reg) & ~ODMA_DESC_READ_DESC_MASK) | (((uint32_t)val) << ODMA_DESC_READ_DESC_SHIFT))

//====================================================================
//Register: Descriptor Transfer Length Register (xfer_length)
//====================================================================

#define  ODMA_XFER_LENGTH_LEN_MASK 0xffffffff
#define  ODMA_XFER_LENGTH_LEN_SHIFT 0
#define  ODMA_XFER_LENGTH_LEN_MASK_SHIFT(reg) (((reg) & ODMA_XFER_LENGTH_LEN_MASK) >> ODMA_XFER_LENGTH_LEN_SHIFT)
#define  ODMA_XFER_LENGTH_LEN_REPLACE_VAL(reg,val) (((reg) & ~ODMA_XFER_LENGTH_LEN_MASK) | (((uint32_t)val) << ODMA_XFER_LENGTH_LEN_SHIFT))

//====================================================================
//Register: Descriptor Transfer Address Register (xfer_addr)
//====================================================================

#define  ODMA_XFER_ADDR_ADDR_MASK 0xffffffff
#define  ODMA_XFER_ADDR_ADDR_SHIFT 0
#define  ODMA_XFER_ADDR_ADDR_MASK_SHIFT(reg) (((reg) & ODMA_XFER_ADDR_ADDR_MASK) >> ODMA_XFER_ADDR_ADDR_SHIFT)
#define  ODMA_XFER_ADDR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~ODMA_XFER_ADDR_ADDR_MASK) | (((uint32_t)val) << ODMA_XFER_ADDR_ADDR_SHIFT))

//====================================================================
//Register: Descriptor Transfer Burst Register (xfer_burst)
//====================================================================

#define  ODMA_XFER_BURST_RESERVED1_MASK 0xffffffe0
#define  ODMA_XFER_BURST_RESERVED1_SHIFT 5
#define  ODMA_XFER_BURST_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_XFER_BURST_RESERVED1_MASK) >> ODMA_XFER_BURST_RESERVED1_SHIFT)
#define  ODMA_XFER_BURST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_XFER_BURST_RESERVED1_MASK) | (((uint32_t)val) << ODMA_XFER_BURST_RESERVED1_SHIFT))

#define  ODMA_XFER_BURST_BURST_MASK 0x1f
#define  ODMA_XFER_BURST_BURST_SHIFT 0
#define  ODMA_XFER_BURST_BURST_MASK_SHIFT(reg) (((reg) & ODMA_XFER_BURST_BURST_MASK) >> ODMA_XFER_BURST_BURST_SHIFT)
#define  ODMA_XFER_BURST_BURST_REPLACE_VAL(reg,val) (((reg) & ~ODMA_XFER_BURST_BURST_MASK) | (((uint32_t)val) << ODMA_XFER_BURST_BURST_SHIFT))

//====================================================================
//Register: Soft Reset register (reset)
//====================================================================

#define  ODMA_RESET_RESERVED1_MASK 0xfffffffe
#define  ODMA_RESET_RESERVED1_SHIFT 1
#define  ODMA_RESET_RESERVED1_MASK_SHIFT(reg) (((reg) & ODMA_RESET_RESERVED1_MASK) >> ODMA_RESET_RESERVED1_SHIFT)
#define  ODMA_RESET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ODMA_RESET_RESERVED1_MASK) | (((uint32_t)val) << ODMA_RESET_RESERVED1_SHIFT))

#define  ODMA_RESET_SOFT_RESET_MASK 0x1
#define  ODMA_RESET_SOFT_RESET_SHIFT 0
#define  ODMA_RESET_SOFT_RESET_MASK_SHIFT(reg) (((reg) & ODMA_RESET_SOFT_RESET_MASK) >> ODMA_RESET_SOFT_RESET_SHIFT)
#define  ODMA_RESET_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~ODMA_RESET_SOFT_RESET_MASK) | (((uint32_t)val) << ODMA_RESET_SOFT_RESET_SHIFT))

#endif // ODMA
