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
 * \file Xena_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _XENA_REGMASKS_H_
#define _XENA_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Register descriptions for UPC module (Xena)
/** \brief Register Descriptions for the UPC block. Registers from offset starting at
0x0 are "Public", and registers with offsets starting at 0x1000 are "Private".*/
//
//====================================================================

//====================================================================
//Register: UPC Prefetch Reset Register (Private) (XRR)
/** \brief UPC Prefetch Reset Register*/
//====================================================================

#define  XENA_XRR_RESERVED1_MASK 0xfffffffc
#define  XENA_XRR_RESERVED1_SHIFT 2
#define  XENA_XRR_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XRR_RESERVED1_MASK) >> XENA_XRR_RESERVED1_SHIFT)
#define  XENA_XRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XRR_RESERVED1_MASK) | (((uint32_t)val) << XENA_XRR_RESERVED1_SHIFT))

#define  XENA_XRR_ROTATEBYPASS_MASK 0x2
#define  XENA_XRR_ROTATEBYPASS_SHIFT 1
#define  XENA_XRR_ROTATEBYPASS_MASK_SHIFT(reg) (((reg) & XENA_XRR_ROTATEBYPASS_MASK) >> XENA_XRR_ROTATEBYPASS_SHIFT)
#define  XENA_XRR_ROTATEBYPASS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XRR_ROTATEBYPASS_MASK) | (((uint32_t)val) << XENA_XRR_ROTATEBYPASS_SHIFT))

#define  XENA_XRR_S_MASK 0x1
#define  XENA_XRR_S_SHIFT 0
#define  XENA_XRR_S_MASK_SHIFT(reg) (((reg) & XENA_XRR_S_MASK) >> XENA_XRR_S_SHIFT)
#define  XENA_XRR_S_REPLACE_VAL(reg,val) (((reg) & ~XENA_XRR_S_MASK) | (((uint32_t)val) << XENA_XRR_S_SHIFT))

//====================================================================
//Register: UPC Prefetch Start Register (Private) (XPS)
/** \brief UPC Prefetch Start Register*/
//====================================================================

#define  XENA_XPS_RESERVED1_MASK 0xfffffffe
#define  XENA_XPS_RESERVED1_SHIFT 1
#define  XENA_XPS_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XPS_RESERVED1_MASK) >> XENA_XPS_RESERVED1_SHIFT)
#define  XENA_XPS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XPS_RESERVED1_MASK) | (((uint32_t)val) << XENA_XPS_RESERVED1_SHIFT))

#define  XENA_XPS_START_MASK 0x1
#define  XENA_XPS_START_SHIFT 0
#define  XENA_XPS_START_MASK_SHIFT(reg) (((reg) & XENA_XPS_START_MASK) >> XENA_XPS_START_SHIFT)
#define  XENA_XPS_START_REPLACE_VAL(reg,val) (((reg) & ~XENA_XPS_START_MASK) | (((uint32_t)val) << XENA_XPS_START_SHIFT))

//====================================================================
//Register: UPC Prefetch Configuration Register (Private) (XCR)
/** \brief UPC Prefetch Configuration Register*/
//====================================================================

#define  XENA_XCR_BYPASS_MASK 0x80000000
#define  XENA_XCR_BYPASS_SHIFT 31
#define  XENA_XCR_BYPASS_MASK_SHIFT(reg) (((reg) & XENA_XCR_BYPASS_MASK) >> XENA_XCR_BYPASS_SHIFT)
#define  XENA_XCR_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_BYPASS_MASK) | (((uint32_t)val) << XENA_XCR_BYPASS_SHIFT))

#define  XENA_XCR_TILE_MASK 0x40000000
#define  XENA_XCR_TILE_SHIFT 30
#define  XENA_XCR_TILE_MASK_SHIFT(reg) (((reg) & XENA_XCR_TILE_MASK) >> XENA_XCR_TILE_SHIFT)
#define  XENA_XCR_TILE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_TILE_MASK) | (((uint32_t)val) << XENA_XCR_TILE_SHIFT))

#define  XENA_XCR_REPLICATE_MASK 0x30000000
#define  XENA_XCR_REPLICATE_SHIFT 28
#define  XENA_XCR_REPLICATE_MASK_SHIFT(reg) (((reg) & XENA_XCR_REPLICATE_MASK) >> XENA_XCR_REPLICATE_SHIFT)
#define  XENA_XCR_REPLICATE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_REPLICATE_MASK) | (((uint32_t)val) << XENA_XCR_REPLICATE_SHIFT))

#define  XENA_XCR_LINEINTERLEAVE_MASK 0xc000000
#define  XENA_XCR_LINEINTERLEAVE_SHIFT 26
#define  XENA_XCR_LINEINTERLEAVE_MASK_SHIFT(reg) (((reg) & XENA_XCR_LINEINTERLEAVE_MASK) >> XENA_XCR_LINEINTERLEAVE_SHIFT)
#define  XENA_XCR_LINEINTERLEAVE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_LINEINTERLEAVE_MASK) | (((uint32_t)val) << XENA_XCR_LINEINTERLEAVE_SHIFT))

#define  XENA_XCR_DESTSWIZZLE_MASK 0x2000000
#define  XENA_XCR_DESTSWIZZLE_SHIFT 25
#define  XENA_XCR_DESTSWIZZLE_MASK_SHIFT(reg) (((reg) & XENA_XCR_DESTSWIZZLE_MASK) >> XENA_XCR_DESTSWIZZLE_SHIFT)
#define  XENA_XCR_DESTSWIZZLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_DESTSWIZZLE_MASK) | (((uint32_t)val) << XENA_XCR_DESTSWIZZLE_SHIFT))

#define  XENA_XCR_SRCSWIZZLE_MASK 0x1000000
#define  XENA_XCR_SRCSWIZZLE_SHIFT 24
#define  XENA_XCR_SRCSWIZZLE_MASK_SHIFT(reg) (((reg) & XENA_XCR_SRCSWIZZLE_MASK) >> XENA_XCR_SRCSWIZZLE_SHIFT)
#define  XENA_XCR_SRCSWIZZLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_SRCSWIZZLE_MASK) | (((uint32_t)val) << XENA_XCR_SRCSWIZZLE_SHIFT))

#define  XENA_XCR_BURST_MASK 0xf00000
#define  XENA_XCR_BURST_SHIFT 20
#define  XENA_XCR_BURST_MASK_SHIFT(reg) (((reg) & XENA_XCR_BURST_MASK) >> XENA_XCR_BURST_SHIFT)
#define  XENA_XCR_BURST_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_BURST_MASK) | (((uint32_t)val) << XENA_XCR_BURST_SHIFT))

#define  XENA_XCR_MASKBURST_MASK 0xf0000
#define  XENA_XCR_MASKBURST_SHIFT 16
#define  XENA_XCR_MASKBURST_MASK_SHIFT(reg) (((reg) & XENA_XCR_MASKBURST_MASK) >> XENA_XCR_MASKBURST_SHIFT)
#define  XENA_XCR_MASKBURST_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_MASKBURST_MASK) | (((uint32_t)val) << XENA_XCR_MASKBURST_SHIFT))

#define  XENA_XCR_DIRECTION_MASK 0xc000
#define  XENA_XCR_DIRECTION_SHIFT 14
#define  XENA_XCR_DIRECTION_MASK_SHIFT(reg) (((reg) & XENA_XCR_DIRECTION_MASK) >> XENA_XCR_DIRECTION_SHIFT)
#define  XENA_XCR_DIRECTION_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_DIRECTION_MASK) | (((uint32_t)val) << XENA_XCR_DIRECTION_SHIFT))

#define  XENA_XCR_MASKBURSTS_MASK 0x3e00
#define  XENA_XCR_MASKBURSTS_SHIFT 9
#define  XENA_XCR_MASKBURSTS_MASK_SHIFT(reg) (((reg) & XENA_XCR_MASKBURSTS_MASK) >> XENA_XCR_MASKBURSTS_SHIFT)
#define  XENA_XCR_MASKBURSTS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_MASKBURSTS_MASK) | (((uint32_t)val) << XENA_XCR_MASKBURSTS_SHIFT))

#define  XENA_XCR_DATABURSTS_MASK 0x1f0
#define  XENA_XCR_DATABURSTS_SHIFT 4
#define  XENA_XCR_DATABURSTS_MASK_SHIFT(reg) (((reg) & XENA_XCR_DATABURSTS_MASK) >> XENA_XCR_DATABURSTS_SHIFT)
#define  XENA_XCR_DATABURSTS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_DATABURSTS_MASK) | (((uint32_t)val) << XENA_XCR_DATABURSTS_SHIFT))

#define  XENA_XCR_BITSPERPIXEL_MASK 0xf
#define  XENA_XCR_BITSPERPIXEL_SHIFT 0
#define  XENA_XCR_BITSPERPIXEL_MASK_SHIFT(reg) (((reg) & XENA_XCR_BITSPERPIXEL_MASK) >> XENA_XCR_BITSPERPIXEL_SHIFT)
#define  XENA_XCR_BITSPERPIXEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCR_BITSPERPIXEL_MASK) | (((uint32_t)val) << XENA_XCR_BITSPERPIXEL_SHIFT))

//====================================================================
//Register: UPC Prefetch Mask Configuration Register (Private) (XMC)
/** \brief UPC Prefetch Mask Configuration Register*/
//====================================================================

#define  XENA_XMC_MASKEN_MASK 0x80000000
#define  XENA_XMC_MASKEN_SHIFT 31
#define  XENA_XMC_MASKEN_MASK_SHIFT(reg) (((reg) & XENA_XMC_MASKEN_MASK) >> XENA_XMC_MASKEN_SHIFT)
#define  XENA_XMC_MASKEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XMC_MASKEN_MASK) | (((uint32_t)val) << XENA_XMC_MASKEN_SHIFT))

#define  XENA_XMC_RESERVED1_MASK 0x7ffffff8
#define  XENA_XMC_RESERVED1_SHIFT 3
#define  XENA_XMC_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XMC_RESERVED1_MASK) >> XENA_XMC_RESERVED1_SHIFT)
#define  XENA_XMC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XMC_RESERVED1_MASK) | (((uint32_t)val) << XENA_XMC_RESERVED1_SHIFT))

#define  XENA_XMC_MASKTYPE_MASK 0x7
#define  XENA_XMC_MASKTYPE_SHIFT 0
#define  XENA_XMC_MASKTYPE_MASK_SHIFT(reg) (((reg) & XENA_XMC_MASKTYPE_MASK) >> XENA_XMC_MASKTYPE_SHIFT)
#define  XENA_XMC_MASKTYPE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XMC_MASKTYPE_MASK) | (((uint32_t)val) << XENA_XMC_MASKTYPE_SHIFT))

//====================================================================
//Register: UPC Prefetch Mask Address Register (Private) (XMA)
/** \brief UPC Prefetch Mask Address Register*/
//====================================================================

#define  XENA_XMA_MASK_ADDRESS_MASK 0xffffffff
#define  XENA_XMA_MASK_ADDRESS_SHIFT 0
#define  XENA_XMA_MASK_ADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XMA_MASK_ADDRESS_MASK) >> XENA_XMA_MASK_ADDRESS_SHIFT)
#define  XENA_XMA_MASK_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XMA_MASK_ADDRESS_MASK) | (((uint32_t)val) << XENA_XMA_MASK_ADDRESS_SHIFT))

//====================================================================
//Register: UPC Prefetch Mask Jump Register (Private) (XMJ)
/** \brief UPC Prefetch Mask Jump Register*/
//====================================================================

#define  XENA_XMJ_MASK_JUMP_MASK 0xffffffff
#define  XENA_XMJ_MASK_JUMP_SHIFT 0
#define  XENA_XMJ_MASK_JUMP_MASK_SHIFT(reg) (((reg) & XENA_XMJ_MASK_JUMP_MASK) >> XENA_XMJ_MASK_JUMP_SHIFT)
#define  XENA_XMJ_MASK_JUMP_REPLACE_VAL(reg,val) (((reg) & ~XENA_XMJ_MASK_JUMP_MASK) | (((uint32_t)val) << XENA_XMJ_MASK_JUMP_SHIFT))

//====================================================================
//Register: UPC Prefetch Mask Address2 Register (Private) (XMA2)
/** \brief UPC Prefetch Mask Address2 Register*/
//====================================================================

#define  XENA_XMA2_MASK_ADDRESS2_MASK 0xffffffff
#define  XENA_XMA2_MASK_ADDRESS2_SHIFT 0
#define  XENA_XMA2_MASK_ADDRESS2_MASK_SHIFT(reg) (((reg) & XENA_XMA2_MASK_ADDRESS2_MASK) >> XENA_XMA2_MASK_ADDRESS2_SHIFT)
#define  XENA_XMA2_MASK_ADDRESS2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XMA2_MASK_ADDRESS2_MASK) | (((uint32_t)val) << XENA_XMA2_MASK_ADDRESS2_SHIFT))

//====================================================================
//Register: UPC Prefetch Mask Lines Register (Private) (XML)
/** \brief UPC Prefetch Mask Lines Register*/
//====================================================================

#define  XENA_XML_RESERVED1_MASK 0xffffffe0
#define  XENA_XML_RESERVED1_SHIFT 5
#define  XENA_XML_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XML_RESERVED1_MASK) >> XENA_XML_RESERVED1_SHIFT)
#define  XENA_XML_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XML_RESERVED1_MASK) | (((uint32_t)val) << XENA_XML_RESERVED1_SHIFT))

#define  XENA_XML_MASK_LINES_MASK 0x1f
#define  XENA_XML_MASK_LINES_SHIFT 0
#define  XENA_XML_MASK_LINES_MASK_SHIFT(reg) (((reg) & XENA_XML_MASK_LINES_MASK) >> XENA_XML_MASK_LINES_SHIFT)
#define  XENA_XML_MASK_LINES_REPLACE_VAL(reg,val) (((reg) & ~XENA_XML_MASK_LINES_MASK) | (((uint32_t)val) << XENA_XML_MASK_LINES_SHIFT))

//====================================================================
//Register: UPC Prefetch Data Address Register (Private) (XDA)
/** \brief UPC Prefetch Data Address Register*/
//====================================================================

#define  XENA_XDA_DATA_ADDRESS_MASK 0xffffffff
#define  XENA_XDA_DATA_ADDRESS_SHIFT 0
#define  XENA_XDA_DATA_ADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XDA_DATA_ADDRESS_MASK) >> XENA_XDA_DATA_ADDRESS_SHIFT)
#define  XENA_XDA_DATA_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDA_DATA_ADDRESS_MASK) | (((uint32_t)val) << XENA_XDA_DATA_ADDRESS_SHIFT))

//====================================================================
//Register: UPC Prefetch Data Jump Register (Private) (XDJ)
/** \brief UPC Prefetch Data Jump Register*/
//====================================================================

#define  XENA_XDJ_DATA_JUMP_MASK 0xffffffff
#define  XENA_XDJ_DATA_JUMP_SHIFT 0
#define  XENA_XDJ_DATA_JUMP_MASK_SHIFT(reg) (((reg) & XENA_XDJ_DATA_JUMP_MASK) >> XENA_XDJ_DATA_JUMP_SHIFT)
#define  XENA_XDJ_DATA_JUMP_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDJ_DATA_JUMP_MASK) | (((uint32_t)val) << XENA_XDJ_DATA_JUMP_SHIFT))

//====================================================================
//Register: UPC Prefetch Data Address2 Register (Private) (XDA2)
/** \brief UPC Prefetch Data Address2 Register*/
//====================================================================

#define  XENA_XDA2_ADDRESS2_MASK 0xffffffff
#define  XENA_XDA2_ADDRESS2_SHIFT 0
#define  XENA_XDA2_ADDRESS2_MASK_SHIFT(reg) (((reg) & XENA_XDA2_ADDRESS2_MASK) >> XENA_XDA2_ADDRESS2_SHIFT)
#define  XENA_XDA2_ADDRESS2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDA2_ADDRESS2_MASK) | (((uint32_t)val) << XENA_XDA2_ADDRESS2_SHIFT))

//====================================================================
//Register: UPC Prefetch Data Lines Register (Private) (XDL)
/** \brief UPC Prefetch Data Lines Register*/
//====================================================================

#define  XENA_XDL_RESERVED1_MASK 0xffffffe0
#define  XENA_XDL_RESERVED1_SHIFT 5
#define  XENA_XDL_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XDL_RESERVED1_MASK) >> XENA_XDL_RESERVED1_SHIFT)
#define  XENA_XDL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDL_RESERVED1_MASK) | (((uint32_t)val) << XENA_XDL_RESERVED1_SHIFT))

#define  XENA_XDL_LINES_MASK 0x1f
#define  XENA_XDL_LINES_SHIFT 0
#define  XENA_XDL_LINES_MASK_SHIFT(reg) (((reg) & XENA_XDL_LINES_MASK) >> XENA_XDL_LINES_SHIFT)
#define  XENA_XDL_LINES_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDL_LINES_MASK) | (((uint32_t)val) << XENA_XDL_LINES_SHIFT))

//====================================================================
//Register: UPC Prefetch Mask Burst Tiling setup Register (Private) (MaskTileBurst)
/** \brief The values in this register are used to control the tiling feature of the Mask prefetch block.*/
//====================================================================

#define  XENA_MASKTILEBURST_RESERVED1_MASK 0xe0000000
#define  XENA_MASKTILEBURST_RESERVED1_SHIFT 29
#define  XENA_MASKTILEBURST_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_RESERVED1_MASK) >> XENA_MASKTILEBURST_RESERVED1_SHIFT)
#define  XENA_MASKTILEBURST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_RESERVED1_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_RESERVED1_SHIFT))

#define  XENA_MASKTILEBURST_MINBURSTINDEX_MASK 0x1f000000
#define  XENA_MASKTILEBURST_MINBURSTINDEX_SHIFT 24
#define  XENA_MASKTILEBURST_MINBURSTINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_MINBURSTINDEX_MASK) >> XENA_MASKTILEBURST_MINBURSTINDEX_SHIFT)
#define  XENA_MASKTILEBURST_MINBURSTINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_MINBURSTINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_MINBURSTINDEX_SHIFT))

#define  XENA_MASKTILEBURST_RESERVED2_MASK 0xe00000
#define  XENA_MASKTILEBURST_RESERVED2_SHIFT 21
#define  XENA_MASKTILEBURST_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_RESERVED2_MASK) >> XENA_MASKTILEBURST_RESERVED2_SHIFT)
#define  XENA_MASKTILEBURST_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_RESERVED2_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_RESERVED2_SHIFT))

#define  XENA_MASKTILEBURST_MAXBURSTINDEX_MASK 0x1f0000
#define  XENA_MASKTILEBURST_MAXBURSTINDEX_SHIFT 16
#define  XENA_MASKTILEBURST_MAXBURSTINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_MAXBURSTINDEX_MASK) >> XENA_MASKTILEBURST_MAXBURSTINDEX_SHIFT)
#define  XENA_MASKTILEBURST_MAXBURSTINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_MAXBURSTINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_MAXBURSTINDEX_SHIFT))

#define  XENA_MASKTILEBURST_RESERVED3_MASK 0xe000
#define  XENA_MASKTILEBURST_RESERVED3_SHIFT 13
#define  XENA_MASKTILEBURST_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_RESERVED3_MASK) >> XENA_MASKTILEBURST_RESERVED3_SHIFT)
#define  XENA_MASKTILEBURST_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_RESERVED3_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_RESERVED3_SHIFT))

#define  XENA_MASKTILEBURST_STOPBURSTINDEX_MASK 0x1f00
#define  XENA_MASKTILEBURST_STOPBURSTINDEX_SHIFT 8
#define  XENA_MASKTILEBURST_STOPBURSTINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_STOPBURSTINDEX_MASK) >> XENA_MASKTILEBURST_STOPBURSTINDEX_SHIFT)
#define  XENA_MASKTILEBURST_STOPBURSTINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_STOPBURSTINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_STOPBURSTINDEX_SHIFT))

#define  XENA_MASKTILEBURST_RESERVED4_MASK 0xe0
#define  XENA_MASKTILEBURST_RESERVED4_SHIFT 5
#define  XENA_MASKTILEBURST_RESERVED4_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_RESERVED4_MASK) >> XENA_MASKTILEBURST_RESERVED4_SHIFT)
#define  XENA_MASKTILEBURST_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_RESERVED4_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_RESERVED4_SHIFT))

#define  XENA_MASKTILEBURST_STARTBURSTINDEX_MASK 0x1f
#define  XENA_MASKTILEBURST_STARTBURSTINDEX_SHIFT 0
#define  XENA_MASKTILEBURST_STARTBURSTINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEBURST_STARTBURSTINDEX_MASK) >> XENA_MASKTILEBURST_STARTBURSTINDEX_SHIFT)
#define  XENA_MASKTILEBURST_STARTBURSTINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEBURST_STARTBURSTINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEBURST_STARTBURSTINDEX_SHIFT))

//====================================================================
//Register: UPC Prefetch Mask Word Tiling setup Register (Private) (MaskTileWord)
/** \brief The values in this register are used to control the tiling feature of the Mask prefetch block.*/
//====================================================================

#define  XENA_MASKTILEWORD_RESERVED1_MASK 0xf0000000
#define  XENA_MASKTILEWORD_RESERVED1_SHIFT 28
#define  XENA_MASKTILEWORD_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_RESERVED1_MASK) >> XENA_MASKTILEWORD_RESERVED1_SHIFT)
#define  XENA_MASKTILEWORD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_RESERVED1_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_RESERVED1_SHIFT))

#define  XENA_MASKTILEWORD_MINWORDINDEX_MASK 0xf000000
#define  XENA_MASKTILEWORD_MINWORDINDEX_SHIFT 24
#define  XENA_MASKTILEWORD_MINWORDINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_MINWORDINDEX_MASK) >> XENA_MASKTILEWORD_MINWORDINDEX_SHIFT)
#define  XENA_MASKTILEWORD_MINWORDINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_MINWORDINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_MINWORDINDEX_SHIFT))

#define  XENA_MASKTILEWORD_RESERVED2_MASK 0xf00000
#define  XENA_MASKTILEWORD_RESERVED2_SHIFT 20
#define  XENA_MASKTILEWORD_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_RESERVED2_MASK) >> XENA_MASKTILEWORD_RESERVED2_SHIFT)
#define  XENA_MASKTILEWORD_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_RESERVED2_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_RESERVED2_SHIFT))

#define  XENA_MASKTILEWORD_MAXWORDINDEX_MASK 0xf0000
#define  XENA_MASKTILEWORD_MAXWORDINDEX_SHIFT 16
#define  XENA_MASKTILEWORD_MAXWORDINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_MAXWORDINDEX_MASK) >> XENA_MASKTILEWORD_MAXWORDINDEX_SHIFT)
#define  XENA_MASKTILEWORD_MAXWORDINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_MAXWORDINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_MAXWORDINDEX_SHIFT))

#define  XENA_MASKTILEWORD_RESERVED3_MASK 0xf000
#define  XENA_MASKTILEWORD_RESERVED3_SHIFT 12
#define  XENA_MASKTILEWORD_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_RESERVED3_MASK) >> XENA_MASKTILEWORD_RESERVED3_SHIFT)
#define  XENA_MASKTILEWORD_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_RESERVED3_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_RESERVED3_SHIFT))

#define  XENA_MASKTILEWORD_STOPWORDINDEX_MASK 0xf00
#define  XENA_MASKTILEWORD_STOPWORDINDEX_SHIFT 8
#define  XENA_MASKTILEWORD_STOPWORDINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_STOPWORDINDEX_MASK) >> XENA_MASKTILEWORD_STOPWORDINDEX_SHIFT)
#define  XENA_MASKTILEWORD_STOPWORDINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_STOPWORDINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_STOPWORDINDEX_SHIFT))

#define  XENA_MASKTILEWORD_RESERVED4_MASK 0xf0
#define  XENA_MASKTILEWORD_RESERVED4_SHIFT 4
#define  XENA_MASKTILEWORD_RESERVED4_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_RESERVED4_MASK) >> XENA_MASKTILEWORD_RESERVED4_SHIFT)
#define  XENA_MASKTILEWORD_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_RESERVED4_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_RESERVED4_SHIFT))

#define  XENA_MASKTILEWORD_STARTWORDINDEX_MASK 0xf
#define  XENA_MASKTILEWORD_STARTWORDINDEX_SHIFT 0
#define  XENA_MASKTILEWORD_STARTWORDINDEX_MASK_SHIFT(reg) (((reg) & XENA_MASKTILEWORD_STARTWORDINDEX_MASK) >> XENA_MASKTILEWORD_STARTWORDINDEX_SHIFT)
#define  XENA_MASKTILEWORD_STARTWORDINDEX_REPLACE_VAL(reg,val) (((reg) & ~XENA_MASKTILEWORD_STARTWORDINDEX_MASK) | (((uint32_t)val) << XENA_MASKTILEWORD_STARTWORDINDEX_SHIFT))

//====================================================================
//Register: UPC Prefetch Destination Address Register (Private) (XCA)
/** \brief UPC Prefetch Destination Address Register*/
//====================================================================

#define  XENA_XCA_DEST_ADDRESS_MASK 0xffffffff
#define  XENA_XCA_DEST_ADDRESS_SHIFT 0
#define  XENA_XCA_DEST_ADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XCA_DEST_ADDRESS_MASK) >> XENA_XCA_DEST_ADDRESS_SHIFT)
#define  XENA_XCA_DEST_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCA_DEST_ADDRESS_MASK) | (((uint32_t)val) << XENA_XCA_DEST_ADDRESS_SHIFT))

//====================================================================
//Register: UPC Prefetch Destination Jump Register (Private) (XCJ)
/** \brief UPC Prefetch Destination Jump Register*/
//====================================================================

#define  XENA_XCJ_DEST_JUMP_MASK 0xffffffff
#define  XENA_XCJ_DEST_JUMP_SHIFT 0
#define  XENA_XCJ_DEST_JUMP_MASK_SHIFT(reg) (((reg) & XENA_XCJ_DEST_JUMP_MASK) >> XENA_XCJ_DEST_JUMP_SHIFT)
#define  XENA_XCJ_DEST_JUMP_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCJ_DEST_JUMP_MASK) | (((uint32_t)val) << XENA_XCJ_DEST_JUMP_SHIFT))

//====================================================================
//Register: UPC Prefetch Bypass NumWords Register (Private) (XNW)
/** \brief UPC Prefetch Bypass NumWords Register*/
//====================================================================

#define  XENA_XNW_NUMWORDS_MASK 0xffffffff
#define  XENA_XNW_NUMWORDS_SHIFT 0
#define  XENA_XNW_NUMWORDS_MASK_SHIFT(reg) (((reg) & XENA_XNW_NUMWORDS_MASK) >> XENA_XNW_NUMWORDS_SHIFT)
#define  XENA_XNW_NUMWORDS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XNW_NUMWORDS_MASK) | (((uint32_t)val) << XENA_XNW_NUMWORDS_SHIFT))

//====================================================================
//Register: UPC Prefetch Status Register (Private) (XST)
/** \brief UPC Prefetch Status Register*/
//====================================================================

#define  XENA_XST_DONE_MASK 0x80000000
#define  XENA_XST_DONE_SHIFT 31
#define  XENA_XST_DONE_MASK_SHIFT(reg) (((reg) & XENA_XST_DONE_MASK) >> XENA_XST_DONE_SHIFT)
#define  XENA_XST_DONE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XST_DONE_MASK) | (((uint32_t)val) << XENA_XST_DONE_SHIFT))

#define  XENA_XST_RESERVED1_MASK 0x7fffffff
#define  XENA_XST_RESERVED1_SHIFT 0
#define  XENA_XST_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XST_RESERVED1_MASK) >> XENA_XST_RESERVED1_SHIFT)
#define  XENA_XST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XST_RESERVED1_MASK) | (((uint32_t)val) << XENA_XST_RESERVED1_SHIFT))

//====================================================================
//Register: UPC Prefetch Destination Address Register2 (Private) (XCA2)
/** \brief UPC Prefetch Destination Address Register2*/
//====================================================================

#define  XENA_XCA2_DEST_ADDRESS2_MASK 0xffffffff
#define  XENA_XCA2_DEST_ADDRESS2_SHIFT 0
#define  XENA_XCA2_DEST_ADDRESS2_MASK_SHIFT(reg) (((reg) & XENA_XCA2_DEST_ADDRESS2_MASK) >> XENA_XCA2_DEST_ADDRESS2_SHIFT)
#define  XENA_XCA2_DEST_ADDRESS2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCA2_DEST_ADDRESS2_MASK) | (((uint32_t)val) << XENA_XCA2_DEST_ADDRESS2_SHIFT))

//====================================================================
//Register: UPC Prefetch Destination Config Register (Private) (XDest)
/** \brief UPC Prefetch Destination Config Register*/
//====================================================================

#define  XENA_XDEST_DEST_USEADD2_MASK 0x80000000
#define  XENA_XDEST_DEST_USEADD2_SHIFT 31
#define  XENA_XDEST_DEST_USEADD2_MASK_SHIFT(reg) (((reg) & XENA_XDEST_DEST_USEADD2_MASK) >> XENA_XDEST_DEST_USEADD2_SHIFT)
#define  XENA_XDEST_DEST_USEADD2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDEST_DEST_USEADD2_MASK) | (((uint32_t)val) << XENA_XDEST_DEST_USEADD2_SHIFT))

#define  XENA_XDEST_RESERVED1_MASK 0x7ffffc00
#define  XENA_XDEST_RESERVED1_SHIFT 10
#define  XENA_XDEST_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XDEST_RESERVED1_MASK) >> XENA_XDEST_RESERVED1_SHIFT)
#define  XENA_XDEST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDEST_RESERVED1_MASK) | (((uint32_t)val) << XENA_XDEST_RESERVED1_SHIFT))

#define  XENA_XDEST_DEST_NUMWRITES_MASK 0x3ff
#define  XENA_XDEST_DEST_NUMWRITES_SHIFT 0
#define  XENA_XDEST_DEST_NUMWRITES_MASK_SHIFT(reg) (((reg) & XENA_XDEST_DEST_NUMWRITES_MASK) >> XENA_XDEST_DEST_NUMWRITES_SHIFT)
#define  XENA_XDEST_DEST_NUMWRITES_REPLACE_VAL(reg,val) (((reg) & ~XENA_XDEST_DEST_NUMWRITES_MASK) | (((uint32_t)val) << XENA_XDEST_DEST_NUMWRITES_SHIFT))

//====================================================================
//Register: UPC Encoder Control Register (Private) (EncControl)
/** \brief The UPC Encoder Control Register provides firmware access to
      control various aspects of processing for the quadrature encoder
      signals.*/
//====================================================================

#define  XENA_ENCCONTROL_RESERVED1_MASK 0xfffc0000
#define  XENA_ENCCONTROL_RESERVED1_SHIFT 18
#define  XENA_ENCCONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_RESERVED1_MASK) >> XENA_ENCCONTROL_RESERVED1_SHIFT)
#define  XENA_ENCCONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_RESERVED1_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_RESERVED1_SHIFT))

#define  XENA_ENCCONTROL_SMARTEN_MASK 0x20000
#define  XENA_ENCCONTROL_SMARTEN_SHIFT 17
#define  XENA_ENCCONTROL_SMARTEN_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_SMARTEN_MASK) >> XENA_ENCCONTROL_SMARTEN_SHIFT)
#define  XENA_ENCCONTROL_SMARTEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_SMARTEN_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_SMARTEN_SHIFT))

#define  XENA_ENCCONTROL_DIRLOCK_MASK 0x10000
#define  XENA_ENCCONTROL_DIRLOCK_SHIFT 16
#define  XENA_ENCCONTROL_DIRLOCK_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_DIRLOCK_MASK) >> XENA_ENCCONTROL_DIRLOCK_SHIFT)
#define  XENA_ENCCONTROL_DIRLOCK_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_DIRLOCK_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_DIRLOCK_SHIFT))

#define  XENA_ENCCONTROL_MAXCHANGE_MASK 0xf000
#define  XENA_ENCCONTROL_MAXCHANGE_SHIFT 12
#define  XENA_ENCCONTROL_MAXCHANGE_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_MAXCHANGE_MASK) >> XENA_ENCCONTROL_MAXCHANGE_SHIFT)
#define  XENA_ENCCONTROL_MAXCHANGE_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_MAXCHANGE_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_MAXCHANGE_SHIFT))

#define  XENA_ENCCONTROL_RESERVED2_MASK 0xc00
#define  XENA_ENCCONTROL_RESERVED2_SHIFT 10
#define  XENA_ENCCONTROL_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_RESERVED2_MASK) >> XENA_ENCCONTROL_RESERVED2_SHIFT)
#define  XENA_ENCCONTROL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_RESERVED2_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_RESERVED2_SHIFT))

#define  XENA_ENCCONTROL_ENCSWAP_MASK 0x200
#define  XENA_ENCCONTROL_ENCSWAP_SHIFT 9
#define  XENA_ENCCONTROL_ENCSWAP_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_ENCSWAP_MASK) >> XENA_ENCCONTROL_ENCSWAP_SHIFT)
#define  XENA_ENCCONTROL_ENCSWAP_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_ENCSWAP_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_ENCSWAP_SHIFT))

#define  XENA_ENCCONTROL_ENCINV_MASK 0x100
#define  XENA_ENCCONTROL_ENCINV_SHIFT 8
#define  XENA_ENCCONTROL_ENCINV_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_ENCINV_MASK) >> XENA_ENCCONTROL_ENCINV_SHIFT)
#define  XENA_ENCCONTROL_ENCINV_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_ENCINV_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_ENCINV_SHIFT))

#define  XENA_ENCCONTROL_RESERVED3_MASK 0xe0
#define  XENA_ENCCONTROL_RESERVED3_SHIFT 5
#define  XENA_ENCCONTROL_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_RESERVED3_MASK) >> XENA_ENCCONTROL_RESERVED3_SHIFT)
#define  XENA_ENCCONTROL_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_RESERVED3_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_RESERVED3_SHIFT))

#define  XENA_ENCCONTROL_ZEROPOS_MASK 0x10
#define  XENA_ENCCONTROL_ZEROPOS_SHIFT 4
#define  XENA_ENCCONTROL_ZEROPOS_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_ZEROPOS_MASK) >> XENA_ENCCONTROL_ZEROPOS_SHIFT)
#define  XENA_ENCCONTROL_ZEROPOS_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_ZEROPOS_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_ZEROPOS_SHIFT))

#define  XENA_ENCCONTROL_RESERVED4_MASK 0xe
#define  XENA_ENCCONTROL_RESERVED4_SHIFT 1
#define  XENA_ENCCONTROL_RESERVED4_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_RESERVED4_MASK) >> XENA_ENCCONTROL_RESERVED4_SHIFT)
#define  XENA_ENCCONTROL_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_RESERVED4_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_RESERVED4_SHIFT))

#define  XENA_ENCCONTROL_ENABLE_MASK 0x1
#define  XENA_ENCCONTROL_ENABLE_SHIFT 0
#define  XENA_ENCCONTROL_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_ENCCONTROL_ENABLE_MASK) >> XENA_ENCCONTROL_ENABLE_SHIFT)
#define  XENA_ENCCONTROL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCCONTROL_ENABLE_MASK) | (((uint32_t)val) << XENA_ENCCONTROL_ENABLE_SHIFT))

//====================================================================
//Register: UPC Encoder Position Register (EncPos)
/** \brief The UPC Encoder Position Register provides firmware access to
      the current position of the encoder in units of the encoder's
      native resolution. Writing this register sets the current position
      to the value written. This new position is also pushed into the
      fire generators, so the resolution must be set correctly
      in all used fire generators BEFORE the position is written.*/
//====================================================================

#define  XENA_ENCPOS_RESERVED1_MASK 0xfff80000
#define  XENA_ENCPOS_RESERVED1_SHIFT 19
#define  XENA_ENCPOS_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_ENCPOS_RESERVED1_MASK) >> XENA_ENCPOS_RESERVED1_SHIFT)
#define  XENA_ENCPOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCPOS_RESERVED1_MASK) | (((uint32_t)val) << XENA_ENCPOS_RESERVED1_SHIFT))

#define  XENA_ENCPOS_POS_MASK 0x7ffff
#define  XENA_ENCPOS_POS_SHIFT 0
#define  XENA_ENCPOS_POS_MASK_SHIFT(reg) (((reg) & XENA_ENCPOS_POS_MASK) >> XENA_ENCPOS_POS_SHIFT)
#define  XENA_ENCPOS_POS_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCPOS_POS_MASK) | (((uint32_t)val) << XENA_ENCPOS_POS_SHIFT))

//====================================================================
//Register: UPC Encoder Filter Register (Private) (EncFilt)
/** \brief The UPC Encoder Filter Register provides firmware access to
      control various aspects of filter of the raw encoder signals.
      After being synchronized to the internal clock, the encoder
      signals are filtered to remove signal quality issues.*/
//====================================================================

#define  XENA_ENCFILT_LOCK_MASK 0xffff0000
#define  XENA_ENCFILT_LOCK_SHIFT 16
#define  XENA_ENCFILT_LOCK_MASK_SHIFT(reg) (((reg) & XENA_ENCFILT_LOCK_MASK) >> XENA_ENCFILT_LOCK_SHIFT)
#define  XENA_ENCFILT_LOCK_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCFILT_LOCK_MASK) | (((uint32_t)val) << XENA_ENCFILT_LOCK_SHIFT))

#define  XENA_ENCFILT_FILT_MASK 0xffff
#define  XENA_ENCFILT_FILT_SHIFT 0
#define  XENA_ENCFILT_FILT_MASK_SHIFT(reg) (((reg) & XENA_ENCFILT_FILT_MASK) >> XENA_ENCFILT_FILT_SHIFT)
#define  XENA_ENCFILT_FILT_REPLACE_VAL(reg,val) (((reg) & ~XENA_ENCFILT_FILT_MASK) | (((uint32_t)val) << XENA_ENCFILT_FILT_SHIFT))

//====================================================================
//Register: UPC Encoder Period Prediction Coefficient Register (Private) (Coeff)
/** \brief The UPC Encoder Period Prediction Coefficient Register defines
      what the programmable coefficients are used during phase period
      prediction. The equation to do phase period prediction is
      PP(n+1) = A*PT(n) + B*PT(n-1) + C*PT(n-2) + D*PT(n-3)
      where pp is the predicted period of the next encoder phase, and pt
      are the measured phase periods. The constants A - D are scalars
      used to weight the average: Constants A - D should nominally sum to
      1.0 and weight the previous four phase period measurements.
      Constants A to D are 8-bit signed, fixed-point numbers in sign-magnitude
      format. Bit 7 is the sign bit and bits 6-0 are the magnitude.
      The radix point is between bits 4 and 5. The effective range of these
      coefficients is -3.96875 - 3.96875.*/
//====================================================================

#define  XENA_COEFF_D_MASK 0xff000000
#define  XENA_COEFF_D_SHIFT 24
#define  XENA_COEFF_D_MASK_SHIFT(reg) (((reg) & XENA_COEFF_D_MASK) >> XENA_COEFF_D_SHIFT)
#define  XENA_COEFF_D_REPLACE_VAL(reg,val) (((reg) & ~XENA_COEFF_D_MASK) | (((uint32_t)val) << XENA_COEFF_D_SHIFT))

#define  XENA_COEFF_C_MASK 0xff0000
#define  XENA_COEFF_C_SHIFT 16
#define  XENA_COEFF_C_MASK_SHIFT(reg) (((reg) & XENA_COEFF_C_MASK) >> XENA_COEFF_C_SHIFT)
#define  XENA_COEFF_C_REPLACE_VAL(reg,val) (((reg) & ~XENA_COEFF_C_MASK) | (((uint32_t)val) << XENA_COEFF_C_SHIFT))

#define  XENA_COEFF_B_MASK 0xff00
#define  XENA_COEFF_B_SHIFT 8
#define  XENA_COEFF_B_MASK_SHIFT(reg) (((reg) & XENA_COEFF_B_MASK) >> XENA_COEFF_B_SHIFT)
#define  XENA_COEFF_B_REPLACE_VAL(reg,val) (((reg) & ~XENA_COEFF_B_MASK) | (((uint32_t)val) << XENA_COEFF_B_SHIFT))

#define  XENA_COEFF_A_MASK 0xff
#define  XENA_COEFF_A_SHIFT 0
#define  XENA_COEFF_A_MASK_SHIFT(reg) (((reg) & XENA_COEFF_A_MASK) >> XENA_COEFF_A_SHIFT)
#define  XENA_COEFF_A_REPLACE_VAL(reg,val) (((reg) & ~XENA_COEFF_A_MASK) | (((uint32_t)val) << XENA_COEFF_A_SHIFT))

//====================================================================
//Register: UPC Firmware Predicted Period Register (Private) (FWPredPeriod)
/** \brief This register allows firmware to bypass the period prediction hardware
      and program its own extrapolated value.*/
//====================================================================

#define  XENA_FWPREDPERIOD_RESERVED1_MASK 0xff000000
#define  XENA_FWPREDPERIOD_RESERVED1_SHIFT 24
#define  XENA_FWPREDPERIOD_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_FWPREDPERIOD_RESERVED1_MASK) >> XENA_FWPREDPERIOD_RESERVED1_SHIFT)
#define  XENA_FWPREDPERIOD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_FWPREDPERIOD_RESERVED1_MASK) | (((uint32_t)val) << XENA_FWPREDPERIOD_RESERVED1_SHIFT))

#define  XENA_FWPREDPERIOD_PREDPERIOD_MASK 0xffffff
#define  XENA_FWPREDPERIOD_PREDPERIOD_SHIFT 0
#define  XENA_FWPREDPERIOD_PREDPERIOD_MASK_SHIFT(reg) (((reg) & XENA_FWPREDPERIOD_PREDPERIOD_MASK) >> XENA_FWPREDPERIOD_PREDPERIOD_SHIFT)
#define  XENA_FWPREDPERIOD_PREDPERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_FWPREDPERIOD_PREDPERIOD_MASK) | (((uint32_t)val) << XENA_FWPREDPERIOD_PREDPERIOD_SHIFT))

//====================================================================
//Register: UPC Encoder Phase 0 Register (Phase0)
/** \brief The UPC Encoder Phase 0 Register provides firmware access to
      the latest phase period for state 0-to-state 0 transitions of the
      encoder.*/
//====================================================================

#define  XENA_PHASE0_RESERVED1_MASK 0xe0000000
#define  XENA_PHASE0_RESERVED1_SHIFT 29
#define  XENA_PHASE0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PHASE0_RESERVED1_MASK) >> XENA_PHASE0_RESERVED1_SHIFT)
#define  XENA_PHASE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE0_RESERVED1_MASK) | (((uint32_t)val) << XENA_PHASE0_RESERVED1_SHIFT))

#define  XENA_PHASE0_DIRVALID_MASK 0x10000000
#define  XENA_PHASE0_DIRVALID_SHIFT 28
#define  XENA_PHASE0_DIRVALID_MASK_SHIFT(reg) (((reg) & XENA_PHASE0_DIRVALID_MASK) >> XENA_PHASE0_DIRVALID_SHIFT)
#define  XENA_PHASE0_DIRVALID_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE0_DIRVALID_MASK) | (((uint32_t)val) << XENA_PHASE0_DIRVALID_SHIFT))

#define  XENA_PHASE0_RESERVED2_MASK 0xe000000
#define  XENA_PHASE0_RESERVED2_SHIFT 25
#define  XENA_PHASE0_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_PHASE0_RESERVED2_MASK) >> XENA_PHASE0_RESERVED2_SHIFT)
#define  XENA_PHASE0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE0_RESERVED2_MASK) | (((uint32_t)val) << XENA_PHASE0_RESERVED2_SHIFT))

#define  XENA_PHASE0_DIR_MASK 0x1000000
#define  XENA_PHASE0_DIR_SHIFT 24
#define  XENA_PHASE0_DIR_MASK_SHIFT(reg) (((reg) & XENA_PHASE0_DIR_MASK) >> XENA_PHASE0_DIR_SHIFT)
#define  XENA_PHASE0_DIR_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE0_DIR_MASK) | (((uint32_t)val) << XENA_PHASE0_DIR_SHIFT))

#define  XENA_PHASE0_PHASE_MASK 0xffffff
#define  XENA_PHASE0_PHASE_SHIFT 0
#define  XENA_PHASE0_PHASE_MASK_SHIFT(reg) (((reg) & XENA_PHASE0_PHASE_MASK) >> XENA_PHASE0_PHASE_SHIFT)
#define  XENA_PHASE0_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE0_PHASE_MASK) | (((uint32_t)val) << XENA_PHASE0_PHASE_SHIFT))

//====================================================================
//Register: UPC Encoder Phase 1 Register (Phase1)
/** \brief The UPC Encoder Phase 1 Register provides firmware access to
      the latest phase period for state 1-to-state 1 transitions of the
      encoder.*/
//====================================================================

#define  XENA_PHASE1_RESERVED1_MASK 0xe0000000
#define  XENA_PHASE1_RESERVED1_SHIFT 29
#define  XENA_PHASE1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PHASE1_RESERVED1_MASK) >> XENA_PHASE1_RESERVED1_SHIFT)
#define  XENA_PHASE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE1_RESERVED1_MASK) | (((uint32_t)val) << XENA_PHASE1_RESERVED1_SHIFT))

#define  XENA_PHASE1_DIRVALID_MASK 0x10000000
#define  XENA_PHASE1_DIRVALID_SHIFT 28
#define  XENA_PHASE1_DIRVALID_MASK_SHIFT(reg) (((reg) & XENA_PHASE1_DIRVALID_MASK) >> XENA_PHASE1_DIRVALID_SHIFT)
#define  XENA_PHASE1_DIRVALID_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE1_DIRVALID_MASK) | (((uint32_t)val) << XENA_PHASE1_DIRVALID_SHIFT))

#define  XENA_PHASE1_RESERVED2_MASK 0xe000000
#define  XENA_PHASE1_RESERVED2_SHIFT 25
#define  XENA_PHASE1_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_PHASE1_RESERVED2_MASK) >> XENA_PHASE1_RESERVED2_SHIFT)
#define  XENA_PHASE1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE1_RESERVED2_MASK) | (((uint32_t)val) << XENA_PHASE1_RESERVED2_SHIFT))

#define  XENA_PHASE1_DIR_MASK 0x1000000
#define  XENA_PHASE1_DIR_SHIFT 24
#define  XENA_PHASE1_DIR_MASK_SHIFT(reg) (((reg) & XENA_PHASE1_DIR_MASK) >> XENA_PHASE1_DIR_SHIFT)
#define  XENA_PHASE1_DIR_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE1_DIR_MASK) | (((uint32_t)val) << XENA_PHASE1_DIR_SHIFT))

#define  XENA_PHASE1_PHASE_MASK 0xffffff
#define  XENA_PHASE1_PHASE_SHIFT 0
#define  XENA_PHASE1_PHASE_MASK_SHIFT(reg) (((reg) & XENA_PHASE1_PHASE_MASK) >> XENA_PHASE1_PHASE_SHIFT)
#define  XENA_PHASE1_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE1_PHASE_MASK) | (((uint32_t)val) << XENA_PHASE1_PHASE_SHIFT))

//====================================================================
//Register: UPC Encoder Phase 2 Register (Phase2)
/** \brief The UPC Encoder Phase 2 Register provides firmware access to
      the latest phase period for state 2-to-state 2 transitions of the
      encoder.*/
//====================================================================

#define  XENA_PHASE2_RESERVED1_MASK 0xe0000000
#define  XENA_PHASE2_RESERVED1_SHIFT 29
#define  XENA_PHASE2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PHASE2_RESERVED1_MASK) >> XENA_PHASE2_RESERVED1_SHIFT)
#define  XENA_PHASE2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE2_RESERVED1_MASK) | (((uint32_t)val) << XENA_PHASE2_RESERVED1_SHIFT))

#define  XENA_PHASE2_DIRVALID_MASK 0x10000000
#define  XENA_PHASE2_DIRVALID_SHIFT 28
#define  XENA_PHASE2_DIRVALID_MASK_SHIFT(reg) (((reg) & XENA_PHASE2_DIRVALID_MASK) >> XENA_PHASE2_DIRVALID_SHIFT)
#define  XENA_PHASE2_DIRVALID_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE2_DIRVALID_MASK) | (((uint32_t)val) << XENA_PHASE2_DIRVALID_SHIFT))

#define  XENA_PHASE2_RESERVED2_MASK 0xe000000
#define  XENA_PHASE2_RESERVED2_SHIFT 25
#define  XENA_PHASE2_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_PHASE2_RESERVED2_MASK) >> XENA_PHASE2_RESERVED2_SHIFT)
#define  XENA_PHASE2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE2_RESERVED2_MASK) | (((uint32_t)val) << XENA_PHASE2_RESERVED2_SHIFT))

#define  XENA_PHASE2_DIR_MASK 0x1000000
#define  XENA_PHASE2_DIR_SHIFT 24
#define  XENA_PHASE2_DIR_MASK_SHIFT(reg) (((reg) & XENA_PHASE2_DIR_MASK) >> XENA_PHASE2_DIR_SHIFT)
#define  XENA_PHASE2_DIR_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE2_DIR_MASK) | (((uint32_t)val) << XENA_PHASE2_DIR_SHIFT))

#define  XENA_PHASE2_PHASE_MASK 0xffffff
#define  XENA_PHASE2_PHASE_SHIFT 0
#define  XENA_PHASE2_PHASE_MASK_SHIFT(reg) (((reg) & XENA_PHASE2_PHASE_MASK) >> XENA_PHASE2_PHASE_SHIFT)
#define  XENA_PHASE2_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE2_PHASE_MASK) | (((uint32_t)val) << XENA_PHASE2_PHASE_SHIFT))

//====================================================================
//Register: UPC Encoder Phase 3 Register (Phase3)
/** \brief The UPC Encoder Phase 3 Register provides firmware access to
      the latest phase period for state 3-to-state 3 transitions of the
      encoder.*/
//====================================================================

#define  XENA_PHASE3_RESERVED1_MASK 0xe0000000
#define  XENA_PHASE3_RESERVED1_SHIFT 29
#define  XENA_PHASE3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PHASE3_RESERVED1_MASK) >> XENA_PHASE3_RESERVED1_SHIFT)
#define  XENA_PHASE3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE3_RESERVED1_MASK) | (((uint32_t)val) << XENA_PHASE3_RESERVED1_SHIFT))

#define  XENA_PHASE3_DIRVALID_MASK 0x10000000
#define  XENA_PHASE3_DIRVALID_SHIFT 28
#define  XENA_PHASE3_DIRVALID_MASK_SHIFT(reg) (((reg) & XENA_PHASE3_DIRVALID_MASK) >> XENA_PHASE3_DIRVALID_SHIFT)
#define  XENA_PHASE3_DIRVALID_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE3_DIRVALID_MASK) | (((uint32_t)val) << XENA_PHASE3_DIRVALID_SHIFT))

#define  XENA_PHASE3_RESERVED2_MASK 0xe000000
#define  XENA_PHASE3_RESERVED2_SHIFT 25
#define  XENA_PHASE3_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_PHASE3_RESERVED2_MASK) >> XENA_PHASE3_RESERVED2_SHIFT)
#define  XENA_PHASE3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE3_RESERVED2_MASK) | (((uint32_t)val) << XENA_PHASE3_RESERVED2_SHIFT))

#define  XENA_PHASE3_DIR_MASK 0x1000000
#define  XENA_PHASE3_DIR_SHIFT 24
#define  XENA_PHASE3_DIR_MASK_SHIFT(reg) (((reg) & XENA_PHASE3_DIR_MASK) >> XENA_PHASE3_DIR_SHIFT)
#define  XENA_PHASE3_DIR_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE3_DIR_MASK) | (((uint32_t)val) << XENA_PHASE3_DIR_SHIFT))

#define  XENA_PHASE3_PHASE_MASK 0xffffff
#define  XENA_PHASE3_PHASE_SHIFT 0
#define  XENA_PHASE3_PHASE_MASK_SHIFT(reg) (((reg) & XENA_PHASE3_PHASE_MASK) >> XENA_PHASE3_PHASE_SHIFT)
#define  XENA_PHASE3_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASE3_PHASE_MASK) | (((uint32_t)val) << XENA_PHASE3_PHASE_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 1 of 8) (LeftMargin0)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 1 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN0_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN0_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN0_RESERVED1_MASK) >> XENA_LEFTMARGIN0_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN0_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN0_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN0_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN0_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN0_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN0_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN0_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN0_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN0_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN0_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN0_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN0_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN0_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN0_RESERVED2_MASK) >> XENA_LEFTMARGIN0_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN0_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN0_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN0_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN0_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN0_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN0_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN0_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN0_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN0_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN0_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 2 of 8) (LeftMargin1)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 2 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN1_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN1_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN1_RESERVED1_MASK) >> XENA_LEFTMARGIN1_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN1_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN1_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN1_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN1_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN1_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN1_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN1_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN1_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN1_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN1_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN1_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN1_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN1_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN1_RESERVED2_MASK) >> XENA_LEFTMARGIN1_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN1_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN1_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN1_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN1_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN1_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN1_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN1_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN1_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN1_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN1_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 3 of 8) (LeftMargin2)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 3 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN2_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN2_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN2_RESERVED1_MASK) >> XENA_LEFTMARGIN2_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN2_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN2_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN2_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN2_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN2_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN2_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN2_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN2_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN2_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN2_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN2_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN2_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN2_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN2_RESERVED2_MASK) >> XENA_LEFTMARGIN2_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN2_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN2_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN2_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN2_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN2_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN2_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN2_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN2_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN2_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN2_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 4 of 8) (LeftMargin3)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 4 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN3_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN3_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN3_RESERVED1_MASK) >> XENA_LEFTMARGIN3_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN3_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN3_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN3_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN3_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN3_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN3_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN3_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN3_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN3_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN3_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN3_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN3_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN3_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN3_RESERVED2_MASK) >> XENA_LEFTMARGIN3_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN3_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN3_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN3_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN3_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN3_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN3_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN3_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN3_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN3_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN3_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 5 of 8) (LeftMargin4)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 5 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN4_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN4_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN4_RESERVED1_MASK) >> XENA_LEFTMARGIN4_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN4_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN4_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN4_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN4_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN4_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN4_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN4_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN4_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN4_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN4_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN4_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN4_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN4_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN4_RESERVED2_MASK) >> XENA_LEFTMARGIN4_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN4_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN4_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN4_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN4_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN4_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN4_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN4_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN4_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN4_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN4_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN4_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 6 of 8) (LeftMargin5)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 6 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN5_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN5_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN5_RESERVED1_MASK) >> XENA_LEFTMARGIN5_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN5_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN5_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN5_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN5_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN5_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN5_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN5_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN5_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN5_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN5_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN5_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN5_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN5_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN5_RESERVED2_MASK) >> XENA_LEFTMARGIN5_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN5_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN5_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN5_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN5_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN5_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN5_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN5_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN5_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN5_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN5_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN5_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 7 of 8) (LeftMargin6)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 7 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN6_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN6_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN6_RESERVED1_MASK) >> XENA_LEFTMARGIN6_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN6_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN6_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN6_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN6_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN6_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN6_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN6_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN6_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN6_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN6_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN6_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN6_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN6_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN6_RESERVED2_MASK) >> XENA_LEFTMARGIN6_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN6_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN6_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN6_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN6_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN6_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN6_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN6_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN6_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN6_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN6_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Left Margin Register (Private) (Instance 8 of 8) (LeftMargin7)
/** \brief The UPC Encoder Left Margin Register sets the left margin
      for the fire generator. (Instance 8 of 8)*/
//====================================================================

#define  XENA_LEFTMARGIN7_RESERVED1_MASK 0xff800000
#define  XENA_LEFTMARGIN7_RESERVED1_SHIFT 23
#define  XENA_LEFTMARGIN7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN7_RESERVED1_MASK) >> XENA_LEFTMARGIN7_RESERVED1_SHIFT)
#define  XENA_LEFTMARGIN7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN7_RESERVED1_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN7_RESERVED1_SHIFT))

#define  XENA_LEFTMARGIN7_LEFTMARGINFRAC_MASK 0x700000
#define  XENA_LEFTMARGIN7_LEFTMARGINFRAC_SHIFT 20
#define  XENA_LEFTMARGIN7_LEFTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN7_LEFTMARGINFRAC_MASK) >> XENA_LEFTMARGIN7_LEFTMARGINFRAC_SHIFT)
#define  XENA_LEFTMARGIN7_LEFTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN7_LEFTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN7_LEFTMARGINFRAC_SHIFT))

#define  XENA_LEFTMARGIN7_RESERVED2_MASK 0x80000
#define  XENA_LEFTMARGIN7_RESERVED2_SHIFT 19
#define  XENA_LEFTMARGIN7_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN7_RESERVED2_MASK) >> XENA_LEFTMARGIN7_RESERVED2_SHIFT)
#define  XENA_LEFTMARGIN7_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN7_RESERVED2_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN7_RESERVED2_SHIFT))

#define  XENA_LEFTMARGIN7_LEFTMARGIN_MASK 0x7ffff
#define  XENA_LEFTMARGIN7_LEFTMARGIN_SHIFT 0
#define  XENA_LEFTMARGIN7_LEFTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_LEFTMARGIN7_LEFTMARGIN_MASK) >> XENA_LEFTMARGIN7_LEFTMARGIN_SHIFT)
#define  XENA_LEFTMARGIN7_LEFTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_LEFTMARGIN7_LEFTMARGIN_MASK) | (((uint32_t)val) << XENA_LEFTMARGIN7_LEFTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 1 of 8) (RightMargin0)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 1 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN0_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN0_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN0_RESERVED1_MASK) >> XENA_RIGHTMARGIN0_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN0_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN0_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN0_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN0_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN0_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN0_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN0_RESERVED2_MASK) >> XENA_RIGHTMARGIN0_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN0_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN0_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN0_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN0_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN0_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN0_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN0_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN0_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN0_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN0_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 2 of 8) (RightMargin1)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 2 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN1_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN1_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN1_RESERVED1_MASK) >> XENA_RIGHTMARGIN1_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN1_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN1_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN1_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN1_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN1_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN1_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN1_RESERVED2_MASK) >> XENA_RIGHTMARGIN1_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN1_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN1_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN1_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN1_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN1_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN1_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN1_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN1_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN1_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN1_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 3 of 8) (RightMargin2)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 3 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN2_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN2_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN2_RESERVED1_MASK) >> XENA_RIGHTMARGIN2_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN2_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN2_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN2_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN2_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN2_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN2_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN2_RESERVED2_MASK) >> XENA_RIGHTMARGIN2_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN2_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN2_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN2_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN2_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN2_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN2_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN2_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN2_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN2_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN2_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 4 of 8) (RightMargin3)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 4 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN3_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN3_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN3_RESERVED1_MASK) >> XENA_RIGHTMARGIN3_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN3_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN3_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN3_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN3_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN3_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN3_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN3_RESERVED2_MASK) >> XENA_RIGHTMARGIN3_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN3_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN3_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN3_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN3_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN3_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN3_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN3_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN3_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN3_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN3_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 5 of 8) (RightMargin4)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 5 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN4_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN4_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN4_RESERVED1_MASK) >> XENA_RIGHTMARGIN4_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN4_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN4_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN4_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN4_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN4_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN4_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN4_RESERVED2_MASK) >> XENA_RIGHTMARGIN4_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN4_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN4_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN4_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN4_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN4_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN4_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN4_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN4_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN4_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN4_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN4_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 6 of 8) (RightMargin5)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 6 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN5_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN5_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN5_RESERVED1_MASK) >> XENA_RIGHTMARGIN5_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN5_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN5_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN5_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN5_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN5_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN5_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN5_RESERVED2_MASK) >> XENA_RIGHTMARGIN5_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN5_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN5_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN5_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN5_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN5_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN5_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN5_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN5_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN5_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN5_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN5_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 7 of 8) (RightMargin6)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 7 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN6_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN6_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN6_RESERVED1_MASK) >> XENA_RIGHTMARGIN6_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN6_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN6_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN6_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN6_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN6_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN6_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN6_RESERVED2_MASK) >> XENA_RIGHTMARGIN6_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN6_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN6_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN6_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN6_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN6_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN6_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN6_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN6_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN6_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN6_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Right Margin Register (Private) (Instance 8 of 8) (RightMargin7)
/** \brief The UPC Encoder Right Margin Register sets the right margin
      for the fire generator. (Instance 8 of 8)*/
//====================================================================

#define  XENA_RIGHTMARGIN7_RESERVED1_MASK 0xff800000
#define  XENA_RIGHTMARGIN7_RESERVED1_SHIFT 23
#define  XENA_RIGHTMARGIN7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN7_RESERVED1_MASK) >> XENA_RIGHTMARGIN7_RESERVED1_SHIFT)
#define  XENA_RIGHTMARGIN7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN7_RESERVED1_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN7_RESERVED1_SHIFT))

#define  XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_MASK 0x700000
#define  XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_SHIFT 20
#define  XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_MASK) >> XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_SHIFT)
#define  XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN7_RIGHTMARGINFRAC_SHIFT))

#define  XENA_RIGHTMARGIN7_RESERVED2_MASK 0x80000
#define  XENA_RIGHTMARGIN7_RESERVED2_SHIFT 19
#define  XENA_RIGHTMARGIN7_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN7_RESERVED2_MASK) >> XENA_RIGHTMARGIN7_RESERVED2_SHIFT)
#define  XENA_RIGHTMARGIN7_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN7_RESERVED2_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN7_RESERVED2_SHIFT))

#define  XENA_RIGHTMARGIN7_RIGHTMARGIN_MASK 0x7ffff
#define  XENA_RIGHTMARGIN7_RIGHTMARGIN_SHIFT 0
#define  XENA_RIGHTMARGIN7_RIGHTMARGIN_MASK_SHIFT(reg) (((reg) & XENA_RIGHTMARGIN7_RIGHTMARGIN_MASK) >> XENA_RIGHTMARGIN7_RIGHTMARGIN_SHIFT)
#define  XENA_RIGHTMARGIN7_RIGHTMARGIN_REPLACE_VAL(reg,val) (((reg) & ~XENA_RIGHTMARGIN7_RIGHTMARGIN_MASK) | (((uint32_t)val) << XENA_RIGHTMARGIN7_RIGHTMARGIN_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 1 of 8) (Res0)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 1 of 8)*/
//====================================================================

#define  XENA_RES0_RESERVED1_MASK 0xc0000000
#define  XENA_RES0_RESERVED1_SHIFT 30
#define  XENA_RES0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES0_RESERVED1_MASK) >> XENA_RES0_RESERVED1_SHIFT)
#define  XENA_RES0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES0_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES0_RESERVED1_SHIFT))

#define  XENA_RES0_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES0_FIRESYNCDIV_SHIFT 24
#define  XENA_RES0_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES0_FIRESYNCDIV_MASK) >> XENA_RES0_FIRESYNCDIV_SHIFT)
#define  XENA_RES0_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES0_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES0_FIRESYNCDIV_SHIFT))

#define  XENA_RES0_RESERVED2_MASK 0xc00000
#define  XENA_RES0_RESERVED2_SHIFT 22
#define  XENA_RES0_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES0_RESERVED2_MASK) >> XENA_RES0_RESERVED2_SHIFT)
#define  XENA_RES0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES0_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES0_RESERVED2_SHIFT))

#define  XENA_RES0_FIRESYNC_MASK 0x3f0000
#define  XENA_RES0_FIRESYNC_SHIFT 16
#define  XENA_RES0_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES0_FIRESYNC_MASK) >> XENA_RES0_FIRESYNC_SHIFT)
#define  XENA_RES0_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES0_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES0_FIRESYNC_SHIFT))

#define  XENA_RES0_RESERVED3_MASK 0xc000
#define  XENA_RES0_RESERVED3_SHIFT 14
#define  XENA_RES0_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES0_RESERVED3_MASK) >> XENA_RES0_RESERVED3_SHIFT)
#define  XENA_RES0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES0_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES0_RESERVED3_SHIFT))

#define  XENA_RES0_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES0_SUBDIVSCALE_SHIFT 8
#define  XENA_RES0_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES0_SUBDIVSCALE_MASK) >> XENA_RES0_SUBDIVSCALE_SHIFT)
#define  XENA_RES0_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES0_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES0_SUBDIVSCALE_SHIFT))

#define  XENA_RES0_SUBDIV_MASK 0xff
#define  XENA_RES0_SUBDIV_SHIFT 0
#define  XENA_RES0_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES0_SUBDIV_MASK) >> XENA_RES0_SUBDIV_SHIFT)
#define  XENA_RES0_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES0_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES0_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 2 of 8) (Res1)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 2 of 8)*/
//====================================================================

#define  XENA_RES1_RESERVED1_MASK 0xc0000000
#define  XENA_RES1_RESERVED1_SHIFT 30
#define  XENA_RES1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES1_RESERVED1_MASK) >> XENA_RES1_RESERVED1_SHIFT)
#define  XENA_RES1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES1_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES1_RESERVED1_SHIFT))

#define  XENA_RES1_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES1_FIRESYNCDIV_SHIFT 24
#define  XENA_RES1_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES1_FIRESYNCDIV_MASK) >> XENA_RES1_FIRESYNCDIV_SHIFT)
#define  XENA_RES1_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES1_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES1_FIRESYNCDIV_SHIFT))

#define  XENA_RES1_RESERVED2_MASK 0xc00000
#define  XENA_RES1_RESERVED2_SHIFT 22
#define  XENA_RES1_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES1_RESERVED2_MASK) >> XENA_RES1_RESERVED2_SHIFT)
#define  XENA_RES1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES1_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES1_RESERVED2_SHIFT))

#define  XENA_RES1_FIRESYNC_MASK 0x3f0000
#define  XENA_RES1_FIRESYNC_SHIFT 16
#define  XENA_RES1_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES1_FIRESYNC_MASK) >> XENA_RES1_FIRESYNC_SHIFT)
#define  XENA_RES1_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES1_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES1_FIRESYNC_SHIFT))

#define  XENA_RES1_RESERVED3_MASK 0xc000
#define  XENA_RES1_RESERVED3_SHIFT 14
#define  XENA_RES1_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES1_RESERVED3_MASK) >> XENA_RES1_RESERVED3_SHIFT)
#define  XENA_RES1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES1_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES1_RESERVED3_SHIFT))

#define  XENA_RES1_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES1_SUBDIVSCALE_SHIFT 8
#define  XENA_RES1_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES1_SUBDIVSCALE_MASK) >> XENA_RES1_SUBDIVSCALE_SHIFT)
#define  XENA_RES1_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES1_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES1_SUBDIVSCALE_SHIFT))

#define  XENA_RES1_SUBDIV_MASK 0xff
#define  XENA_RES1_SUBDIV_SHIFT 0
#define  XENA_RES1_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES1_SUBDIV_MASK) >> XENA_RES1_SUBDIV_SHIFT)
#define  XENA_RES1_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES1_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES1_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 3 of 8) (Res2)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 3 of 8)*/
//====================================================================

#define  XENA_RES2_RESERVED1_MASK 0xc0000000
#define  XENA_RES2_RESERVED1_SHIFT 30
#define  XENA_RES2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES2_RESERVED1_MASK) >> XENA_RES2_RESERVED1_SHIFT)
#define  XENA_RES2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES2_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES2_RESERVED1_SHIFT))

#define  XENA_RES2_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES2_FIRESYNCDIV_SHIFT 24
#define  XENA_RES2_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES2_FIRESYNCDIV_MASK) >> XENA_RES2_FIRESYNCDIV_SHIFT)
#define  XENA_RES2_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES2_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES2_FIRESYNCDIV_SHIFT))

#define  XENA_RES2_RESERVED2_MASK 0xc00000
#define  XENA_RES2_RESERVED2_SHIFT 22
#define  XENA_RES2_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES2_RESERVED2_MASK) >> XENA_RES2_RESERVED2_SHIFT)
#define  XENA_RES2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES2_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES2_RESERVED2_SHIFT))

#define  XENA_RES2_FIRESYNC_MASK 0x3f0000
#define  XENA_RES2_FIRESYNC_SHIFT 16
#define  XENA_RES2_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES2_FIRESYNC_MASK) >> XENA_RES2_FIRESYNC_SHIFT)
#define  XENA_RES2_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES2_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES2_FIRESYNC_SHIFT))

#define  XENA_RES2_RESERVED3_MASK 0xc000
#define  XENA_RES2_RESERVED3_SHIFT 14
#define  XENA_RES2_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES2_RESERVED3_MASK) >> XENA_RES2_RESERVED3_SHIFT)
#define  XENA_RES2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES2_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES2_RESERVED3_SHIFT))

#define  XENA_RES2_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES2_SUBDIVSCALE_SHIFT 8
#define  XENA_RES2_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES2_SUBDIVSCALE_MASK) >> XENA_RES2_SUBDIVSCALE_SHIFT)
#define  XENA_RES2_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES2_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES2_SUBDIVSCALE_SHIFT))

#define  XENA_RES2_SUBDIV_MASK 0xff
#define  XENA_RES2_SUBDIV_SHIFT 0
#define  XENA_RES2_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES2_SUBDIV_MASK) >> XENA_RES2_SUBDIV_SHIFT)
#define  XENA_RES2_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES2_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES2_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 4 of 8) (Res3)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 4 of 8)*/
//====================================================================

#define  XENA_RES3_RESERVED1_MASK 0xc0000000
#define  XENA_RES3_RESERVED1_SHIFT 30
#define  XENA_RES3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES3_RESERVED1_MASK) >> XENA_RES3_RESERVED1_SHIFT)
#define  XENA_RES3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES3_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES3_RESERVED1_SHIFT))

#define  XENA_RES3_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES3_FIRESYNCDIV_SHIFT 24
#define  XENA_RES3_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES3_FIRESYNCDIV_MASK) >> XENA_RES3_FIRESYNCDIV_SHIFT)
#define  XENA_RES3_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES3_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES3_FIRESYNCDIV_SHIFT))

#define  XENA_RES3_RESERVED2_MASK 0xc00000
#define  XENA_RES3_RESERVED2_SHIFT 22
#define  XENA_RES3_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES3_RESERVED2_MASK) >> XENA_RES3_RESERVED2_SHIFT)
#define  XENA_RES3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES3_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES3_RESERVED2_SHIFT))

#define  XENA_RES3_FIRESYNC_MASK 0x3f0000
#define  XENA_RES3_FIRESYNC_SHIFT 16
#define  XENA_RES3_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES3_FIRESYNC_MASK) >> XENA_RES3_FIRESYNC_SHIFT)
#define  XENA_RES3_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES3_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES3_FIRESYNC_SHIFT))

#define  XENA_RES3_RESERVED3_MASK 0xc000
#define  XENA_RES3_RESERVED3_SHIFT 14
#define  XENA_RES3_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES3_RESERVED3_MASK) >> XENA_RES3_RESERVED3_SHIFT)
#define  XENA_RES3_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES3_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES3_RESERVED3_SHIFT))

#define  XENA_RES3_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES3_SUBDIVSCALE_SHIFT 8
#define  XENA_RES3_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES3_SUBDIVSCALE_MASK) >> XENA_RES3_SUBDIVSCALE_SHIFT)
#define  XENA_RES3_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES3_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES3_SUBDIVSCALE_SHIFT))

#define  XENA_RES3_SUBDIV_MASK 0xff
#define  XENA_RES3_SUBDIV_SHIFT 0
#define  XENA_RES3_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES3_SUBDIV_MASK) >> XENA_RES3_SUBDIV_SHIFT)
#define  XENA_RES3_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES3_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES3_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 5 of 8) (Res4)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 5 of 8)*/
//====================================================================

#define  XENA_RES4_RESERVED1_MASK 0xc0000000
#define  XENA_RES4_RESERVED1_SHIFT 30
#define  XENA_RES4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES4_RESERVED1_MASK) >> XENA_RES4_RESERVED1_SHIFT)
#define  XENA_RES4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES4_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES4_RESERVED1_SHIFT))

#define  XENA_RES4_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES4_FIRESYNCDIV_SHIFT 24
#define  XENA_RES4_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES4_FIRESYNCDIV_MASK) >> XENA_RES4_FIRESYNCDIV_SHIFT)
#define  XENA_RES4_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES4_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES4_FIRESYNCDIV_SHIFT))

#define  XENA_RES4_RESERVED2_MASK 0xc00000
#define  XENA_RES4_RESERVED2_SHIFT 22
#define  XENA_RES4_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES4_RESERVED2_MASK) >> XENA_RES4_RESERVED2_SHIFT)
#define  XENA_RES4_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES4_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES4_RESERVED2_SHIFT))

#define  XENA_RES4_FIRESYNC_MASK 0x3f0000
#define  XENA_RES4_FIRESYNC_SHIFT 16
#define  XENA_RES4_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES4_FIRESYNC_MASK) >> XENA_RES4_FIRESYNC_SHIFT)
#define  XENA_RES4_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES4_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES4_FIRESYNC_SHIFT))

#define  XENA_RES4_RESERVED3_MASK 0xc000
#define  XENA_RES4_RESERVED3_SHIFT 14
#define  XENA_RES4_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES4_RESERVED3_MASK) >> XENA_RES4_RESERVED3_SHIFT)
#define  XENA_RES4_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES4_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES4_RESERVED3_SHIFT))

#define  XENA_RES4_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES4_SUBDIVSCALE_SHIFT 8
#define  XENA_RES4_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES4_SUBDIVSCALE_MASK) >> XENA_RES4_SUBDIVSCALE_SHIFT)
#define  XENA_RES4_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES4_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES4_SUBDIVSCALE_SHIFT))

#define  XENA_RES4_SUBDIV_MASK 0xff
#define  XENA_RES4_SUBDIV_SHIFT 0
#define  XENA_RES4_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES4_SUBDIV_MASK) >> XENA_RES4_SUBDIV_SHIFT)
#define  XENA_RES4_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES4_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES4_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 6 of 8) (Res5)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 6 of 8)*/
//====================================================================

#define  XENA_RES5_RESERVED1_MASK 0xc0000000
#define  XENA_RES5_RESERVED1_SHIFT 30
#define  XENA_RES5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES5_RESERVED1_MASK) >> XENA_RES5_RESERVED1_SHIFT)
#define  XENA_RES5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES5_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES5_RESERVED1_SHIFT))

#define  XENA_RES5_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES5_FIRESYNCDIV_SHIFT 24
#define  XENA_RES5_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES5_FIRESYNCDIV_MASK) >> XENA_RES5_FIRESYNCDIV_SHIFT)
#define  XENA_RES5_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES5_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES5_FIRESYNCDIV_SHIFT))

#define  XENA_RES5_RESERVED2_MASK 0xc00000
#define  XENA_RES5_RESERVED2_SHIFT 22
#define  XENA_RES5_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES5_RESERVED2_MASK) >> XENA_RES5_RESERVED2_SHIFT)
#define  XENA_RES5_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES5_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES5_RESERVED2_SHIFT))

#define  XENA_RES5_FIRESYNC_MASK 0x3f0000
#define  XENA_RES5_FIRESYNC_SHIFT 16
#define  XENA_RES5_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES5_FIRESYNC_MASK) >> XENA_RES5_FIRESYNC_SHIFT)
#define  XENA_RES5_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES5_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES5_FIRESYNC_SHIFT))

#define  XENA_RES5_RESERVED3_MASK 0xc000
#define  XENA_RES5_RESERVED3_SHIFT 14
#define  XENA_RES5_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES5_RESERVED3_MASK) >> XENA_RES5_RESERVED3_SHIFT)
#define  XENA_RES5_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES5_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES5_RESERVED3_SHIFT))

#define  XENA_RES5_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES5_SUBDIVSCALE_SHIFT 8
#define  XENA_RES5_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES5_SUBDIVSCALE_MASK) >> XENA_RES5_SUBDIVSCALE_SHIFT)
#define  XENA_RES5_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES5_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES5_SUBDIVSCALE_SHIFT))

#define  XENA_RES5_SUBDIV_MASK 0xff
#define  XENA_RES5_SUBDIV_SHIFT 0
#define  XENA_RES5_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES5_SUBDIV_MASK) >> XENA_RES5_SUBDIV_SHIFT)
#define  XENA_RES5_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES5_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES5_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 7 of 8) (Res6)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 7 of 8)*/
//====================================================================

#define  XENA_RES6_RESERVED1_MASK 0xc0000000
#define  XENA_RES6_RESERVED1_SHIFT 30
#define  XENA_RES6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES6_RESERVED1_MASK) >> XENA_RES6_RESERVED1_SHIFT)
#define  XENA_RES6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES6_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES6_RESERVED1_SHIFT))

#define  XENA_RES6_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES6_FIRESYNCDIV_SHIFT 24
#define  XENA_RES6_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES6_FIRESYNCDIV_MASK) >> XENA_RES6_FIRESYNCDIV_SHIFT)
#define  XENA_RES6_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES6_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES6_FIRESYNCDIV_SHIFT))

#define  XENA_RES6_RESERVED2_MASK 0xc00000
#define  XENA_RES6_RESERVED2_SHIFT 22
#define  XENA_RES6_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES6_RESERVED2_MASK) >> XENA_RES6_RESERVED2_SHIFT)
#define  XENA_RES6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES6_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES6_RESERVED2_SHIFT))

#define  XENA_RES6_FIRESYNC_MASK 0x3f0000
#define  XENA_RES6_FIRESYNC_SHIFT 16
#define  XENA_RES6_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES6_FIRESYNC_MASK) >> XENA_RES6_FIRESYNC_SHIFT)
#define  XENA_RES6_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES6_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES6_FIRESYNC_SHIFT))

#define  XENA_RES6_RESERVED3_MASK 0xc000
#define  XENA_RES6_RESERVED3_SHIFT 14
#define  XENA_RES6_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES6_RESERVED3_MASK) >> XENA_RES6_RESERVED3_SHIFT)
#define  XENA_RES6_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES6_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES6_RESERVED3_SHIFT))

#define  XENA_RES6_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES6_SUBDIVSCALE_SHIFT 8
#define  XENA_RES6_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES6_SUBDIVSCALE_MASK) >> XENA_RES6_SUBDIVSCALE_SHIFT)
#define  XENA_RES6_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES6_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES6_SUBDIVSCALE_SHIFT))

#define  XENA_RES6_SUBDIV_MASK 0xff
#define  XENA_RES6_SUBDIV_SHIFT 0
#define  XENA_RES6_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES6_SUBDIV_MASK) >> XENA_RES6_SUBDIV_SHIFT)
#define  XENA_RES6_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES6_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES6_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Resolution Register (Private) (Instance 8 of 8) (Res7)
/** \brief The UPC Encoder Resolution Register sets the resolution of fire
      sync pulses for the fire generator. (Instance 8 of 8)*/
//====================================================================

#define  XENA_RES7_RESERVED1_MASK 0xc0000000
#define  XENA_RES7_RESERVED1_SHIFT 30
#define  XENA_RES7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_RES7_RESERVED1_MASK) >> XENA_RES7_RESERVED1_SHIFT)
#define  XENA_RES7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES7_RESERVED1_MASK) | (((uint32_t)val) << XENA_RES7_RESERVED1_SHIFT))

#define  XENA_RES7_FIRESYNCDIV_MASK 0x3f000000
#define  XENA_RES7_FIRESYNCDIV_SHIFT 24
#define  XENA_RES7_FIRESYNCDIV_MASK_SHIFT(reg) (((reg) & XENA_RES7_FIRESYNCDIV_MASK) >> XENA_RES7_FIRESYNCDIV_SHIFT)
#define  XENA_RES7_FIRESYNCDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES7_FIRESYNCDIV_MASK) | (((uint32_t)val) << XENA_RES7_FIRESYNCDIV_SHIFT))

#define  XENA_RES7_RESERVED2_MASK 0xc00000
#define  XENA_RES7_RESERVED2_SHIFT 22
#define  XENA_RES7_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_RES7_RESERVED2_MASK) >> XENA_RES7_RESERVED2_SHIFT)
#define  XENA_RES7_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES7_RESERVED2_MASK) | (((uint32_t)val) << XENA_RES7_RESERVED2_SHIFT))

#define  XENA_RES7_FIRESYNC_MASK 0x3f0000
#define  XENA_RES7_FIRESYNC_SHIFT 16
#define  XENA_RES7_FIRESYNC_MASK_SHIFT(reg) (((reg) & XENA_RES7_FIRESYNC_MASK) >> XENA_RES7_FIRESYNC_SHIFT)
#define  XENA_RES7_FIRESYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES7_FIRESYNC_MASK) | (((uint32_t)val) << XENA_RES7_FIRESYNC_SHIFT))

#define  XENA_RES7_RESERVED3_MASK 0xc000
#define  XENA_RES7_RESERVED3_SHIFT 14
#define  XENA_RES7_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_RES7_RESERVED3_MASK) >> XENA_RES7_RESERVED3_SHIFT)
#define  XENA_RES7_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES7_RESERVED3_MASK) | (((uint32_t)val) << XENA_RES7_RESERVED3_SHIFT))

#define  XENA_RES7_SUBDIVSCALE_MASK 0x3f00
#define  XENA_RES7_SUBDIVSCALE_SHIFT 8
#define  XENA_RES7_SUBDIVSCALE_MASK_SHIFT(reg) (((reg) & XENA_RES7_SUBDIVSCALE_MASK) >> XENA_RES7_SUBDIVSCALE_SHIFT)
#define  XENA_RES7_SUBDIVSCALE_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES7_SUBDIVSCALE_MASK) | (((uint32_t)val) << XENA_RES7_SUBDIVSCALE_SHIFT))

#define  XENA_RES7_SUBDIV_MASK 0xff
#define  XENA_RES7_SUBDIV_SHIFT 0
#define  XENA_RES7_SUBDIV_MASK_SHIFT(reg) (((reg) & XENA_RES7_SUBDIV_MASK) >> XENA_RES7_SUBDIV_SHIFT)
#define  XENA_RES7_SUBDIV_REPLACE_VAL(reg,val) (((reg) & ~XENA_RES7_SUBDIV_MASK) | (((uint32_t)val) << XENA_RES7_SUBDIV_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 1 of 8) (TOF0)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 1 of 8)*/
//====================================================================

#define  XENA_TOF0_RESERVED1_MASK 0xff000000
#define  XENA_TOF0_RESERVED1_SHIFT 24
#define  XENA_TOF0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF0_RESERVED1_MASK) >> XENA_TOF0_RESERVED1_SHIFT)
#define  XENA_TOF0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF0_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF0_RESERVED1_SHIFT))

#define  XENA_TOF0_TOF_MASK 0xffffff
#define  XENA_TOF0_TOF_SHIFT 0
#define  XENA_TOF0_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF0_TOF_MASK) >> XENA_TOF0_TOF_SHIFT)
#define  XENA_TOF0_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF0_TOF_MASK) | (((uint32_t)val) << XENA_TOF0_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 2 of 8) (TOF1)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 2 of 8)*/
//====================================================================

#define  XENA_TOF1_RESERVED1_MASK 0xff000000
#define  XENA_TOF1_RESERVED1_SHIFT 24
#define  XENA_TOF1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF1_RESERVED1_MASK) >> XENA_TOF1_RESERVED1_SHIFT)
#define  XENA_TOF1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF1_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF1_RESERVED1_SHIFT))

#define  XENA_TOF1_TOF_MASK 0xffffff
#define  XENA_TOF1_TOF_SHIFT 0
#define  XENA_TOF1_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF1_TOF_MASK) >> XENA_TOF1_TOF_SHIFT)
#define  XENA_TOF1_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF1_TOF_MASK) | (((uint32_t)val) << XENA_TOF1_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 3 of 8) (TOF2)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 3 of 8)*/
//====================================================================

#define  XENA_TOF2_RESERVED1_MASK 0xff000000
#define  XENA_TOF2_RESERVED1_SHIFT 24
#define  XENA_TOF2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF2_RESERVED1_MASK) >> XENA_TOF2_RESERVED1_SHIFT)
#define  XENA_TOF2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF2_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF2_RESERVED1_SHIFT))

#define  XENA_TOF2_TOF_MASK 0xffffff
#define  XENA_TOF2_TOF_SHIFT 0
#define  XENA_TOF2_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF2_TOF_MASK) >> XENA_TOF2_TOF_SHIFT)
#define  XENA_TOF2_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF2_TOF_MASK) | (((uint32_t)val) << XENA_TOF2_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 4 of 8) (TOF3)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 4 of 8)*/
//====================================================================

#define  XENA_TOF3_RESERVED1_MASK 0xff000000
#define  XENA_TOF3_RESERVED1_SHIFT 24
#define  XENA_TOF3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF3_RESERVED1_MASK) >> XENA_TOF3_RESERVED1_SHIFT)
#define  XENA_TOF3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF3_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF3_RESERVED1_SHIFT))

#define  XENA_TOF3_TOF_MASK 0xffffff
#define  XENA_TOF3_TOF_SHIFT 0
#define  XENA_TOF3_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF3_TOF_MASK) >> XENA_TOF3_TOF_SHIFT)
#define  XENA_TOF3_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF3_TOF_MASK) | (((uint32_t)val) << XENA_TOF3_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 5 of 8) (TOF4)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 5 of 8)*/
//====================================================================

#define  XENA_TOF4_RESERVED1_MASK 0xff000000
#define  XENA_TOF4_RESERVED1_SHIFT 24
#define  XENA_TOF4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF4_RESERVED1_MASK) >> XENA_TOF4_RESERVED1_SHIFT)
#define  XENA_TOF4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF4_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF4_RESERVED1_SHIFT))

#define  XENA_TOF4_TOF_MASK 0xffffff
#define  XENA_TOF4_TOF_SHIFT 0
#define  XENA_TOF4_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF4_TOF_MASK) >> XENA_TOF4_TOF_SHIFT)
#define  XENA_TOF4_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF4_TOF_MASK) | (((uint32_t)val) << XENA_TOF4_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 6 of 8) (TOF5)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 6 of 8)*/
//====================================================================

#define  XENA_TOF5_RESERVED1_MASK 0xff000000
#define  XENA_TOF5_RESERVED1_SHIFT 24
#define  XENA_TOF5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF5_RESERVED1_MASK) >> XENA_TOF5_RESERVED1_SHIFT)
#define  XENA_TOF5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF5_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF5_RESERVED1_SHIFT))

#define  XENA_TOF5_TOF_MASK 0xffffff
#define  XENA_TOF5_TOF_SHIFT 0
#define  XENA_TOF5_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF5_TOF_MASK) >> XENA_TOF5_TOF_SHIFT)
#define  XENA_TOF5_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF5_TOF_MASK) | (((uint32_t)val) << XENA_TOF5_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 7 of 8) (TOF6)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 7 of 8)*/
//====================================================================

#define  XENA_TOF6_RESERVED1_MASK 0xff000000
#define  XENA_TOF6_RESERVED1_SHIFT 24
#define  XENA_TOF6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF6_RESERVED1_MASK) >> XENA_TOF6_RESERVED1_SHIFT)
#define  XENA_TOF6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF6_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF6_RESERVED1_SHIFT))

#define  XENA_TOF6_TOF_MASK 0xffffff
#define  XENA_TOF6_TOF_SHIFT 0
#define  XENA_TOF6_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF6_TOF_MASK) >> XENA_TOF6_TOF_SHIFT)
#define  XENA_TOF6_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF6_TOF_MASK) | (((uint32_t)val) << XENA_TOF6_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Time-of-Flight Register (Private) (Instance 8 of 8) (TOF7)
/** \brief The UPC Encoder Time-of-Flight Register sets the amount of time
      (in CPU clocks) that fire sync pulses are advanced in order to
      compensate for the ink drop's time-of-flight. (Instance 8 of 8)*/
//====================================================================

#define  XENA_TOF7_RESERVED1_MASK 0xff000000
#define  XENA_TOF7_RESERVED1_SHIFT 24
#define  XENA_TOF7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_TOF7_RESERVED1_MASK) >> XENA_TOF7_RESERVED1_SHIFT)
#define  XENA_TOF7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF7_RESERVED1_MASK) | (((uint32_t)val) << XENA_TOF7_RESERVED1_SHIFT))

#define  XENA_TOF7_TOF_MASK 0xffffff
#define  XENA_TOF7_TOF_SHIFT 0
#define  XENA_TOF7_TOF_MASK_SHIFT(reg) (((reg) & XENA_TOF7_TOF_MASK) >> XENA_TOF7_TOF_SHIFT)
#define  XENA_TOF7_TOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_TOF7_TOF_MASK) | (((uint32_t)val) << XENA_TOF7_TOF_SHIFT))

//====================================================================
//Register: UPC Encoder Status Register (Private) (Status)
/** \brief This register contains status information on the Encoder.*/
//====================================================================

#define  XENA_STATUS_RESERVED1_MASK 0xfff80000
#define  XENA_STATUS_RESERVED1_SHIFT 19
#define  XENA_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_STATUS_RESERVED1_MASK) >> XENA_STATUS_RESERVED1_SHIFT)
#define  XENA_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_RESERVED1_MASK) | (((uint32_t)val) << XENA_STATUS_RESERVED1_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED7_MASK 0x40000
#define  XENA_STATUS_FIRESYNCMISSED7_SHIFT 18
#define  XENA_STATUS_FIRESYNCMISSED7_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED7_MASK) >> XENA_STATUS_FIRESYNCMISSED7_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED7_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED7_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED7_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW7_MASK 0x20000
#define  XENA_STATUS_TOFOVERFLOW7_SHIFT 17
#define  XENA_STATUS_TOFOVERFLOW7_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW7_MASK) >> XENA_STATUS_TOFOVERFLOW7_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW7_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW7_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW7_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED6_MASK 0x10000
#define  XENA_STATUS_FIRESYNCMISSED6_SHIFT 16
#define  XENA_STATUS_FIRESYNCMISSED6_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED6_MASK) >> XENA_STATUS_FIRESYNCMISSED6_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED6_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED6_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED6_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW6_MASK 0x8000
#define  XENA_STATUS_TOFOVERFLOW6_SHIFT 15
#define  XENA_STATUS_TOFOVERFLOW6_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW6_MASK) >> XENA_STATUS_TOFOVERFLOW6_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW6_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW6_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW6_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED5_MASK 0x4000
#define  XENA_STATUS_FIRESYNCMISSED5_SHIFT 14
#define  XENA_STATUS_FIRESYNCMISSED5_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED5_MASK) >> XENA_STATUS_FIRESYNCMISSED5_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED5_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED5_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED5_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW5_MASK 0x2000
#define  XENA_STATUS_TOFOVERFLOW5_SHIFT 13
#define  XENA_STATUS_TOFOVERFLOW5_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW5_MASK) >> XENA_STATUS_TOFOVERFLOW5_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW5_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW5_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW5_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED4_MASK 0x1000
#define  XENA_STATUS_FIRESYNCMISSED4_SHIFT 12
#define  XENA_STATUS_FIRESYNCMISSED4_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED4_MASK) >> XENA_STATUS_FIRESYNCMISSED4_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED4_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED4_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED4_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW4_MASK 0x800
#define  XENA_STATUS_TOFOVERFLOW4_SHIFT 11
#define  XENA_STATUS_TOFOVERFLOW4_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW4_MASK) >> XENA_STATUS_TOFOVERFLOW4_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW4_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW4_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW4_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED3_MASK 0x400
#define  XENA_STATUS_FIRESYNCMISSED3_SHIFT 10
#define  XENA_STATUS_FIRESYNCMISSED3_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED3_MASK) >> XENA_STATUS_FIRESYNCMISSED3_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED3_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED3_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW3_MASK 0x200
#define  XENA_STATUS_TOFOVERFLOW3_SHIFT 9
#define  XENA_STATUS_TOFOVERFLOW3_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW3_MASK) >> XENA_STATUS_TOFOVERFLOW3_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW3_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW3_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW3_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED2_MASK 0x100
#define  XENA_STATUS_FIRESYNCMISSED2_SHIFT 8
#define  XENA_STATUS_FIRESYNCMISSED2_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED2_MASK) >> XENA_STATUS_FIRESYNCMISSED2_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED2_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED2_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW2_MASK 0x80
#define  XENA_STATUS_TOFOVERFLOW2_SHIFT 7
#define  XENA_STATUS_TOFOVERFLOW2_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW2_MASK) >> XENA_STATUS_TOFOVERFLOW2_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW2_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW2_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW2_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED1_MASK 0x40
#define  XENA_STATUS_FIRESYNCMISSED1_SHIFT 6
#define  XENA_STATUS_FIRESYNCMISSED1_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED1_MASK) >> XENA_STATUS_FIRESYNCMISSED1_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED1_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED1_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW1_MASK 0x20
#define  XENA_STATUS_TOFOVERFLOW1_SHIFT 5
#define  XENA_STATUS_TOFOVERFLOW1_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW1_MASK) >> XENA_STATUS_TOFOVERFLOW1_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW1_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW1_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW1_SHIFT))

#define  XENA_STATUS_FIRESYNCMISSED0_MASK 0x10
#define  XENA_STATUS_FIRESYNCMISSED0_SHIFT 4
#define  XENA_STATUS_FIRESYNCMISSED0_MASK_SHIFT(reg) (((reg) & XENA_STATUS_FIRESYNCMISSED0_MASK) >> XENA_STATUS_FIRESYNCMISSED0_SHIFT)
#define  XENA_STATUS_FIRESYNCMISSED0_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_FIRESYNCMISSED0_MASK) | (((uint32_t)val) << XENA_STATUS_FIRESYNCMISSED0_SHIFT))

#define  XENA_STATUS_TOFOVERFLOW0_MASK 0x8
#define  XENA_STATUS_TOFOVERFLOW0_SHIFT 3
#define  XENA_STATUS_TOFOVERFLOW0_MASK_SHIFT(reg) (((reg) & XENA_STATUS_TOFOVERFLOW0_MASK) >> XENA_STATUS_TOFOVERFLOW0_SHIFT)
#define  XENA_STATUS_TOFOVERFLOW0_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_TOFOVERFLOW0_MASK) | (((uint32_t)val) << XENA_STATUS_TOFOVERFLOW0_SHIFT))

#define  XENA_STATUS_DIRVALID_MASK 0x4
#define  XENA_STATUS_DIRVALID_SHIFT 2
#define  XENA_STATUS_DIRVALID_MASK_SHIFT(reg) (((reg) & XENA_STATUS_DIRVALID_MASK) >> XENA_STATUS_DIRVALID_SHIFT)
#define  XENA_STATUS_DIRVALID_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_DIRVALID_MASK) | (((uint32_t)val) << XENA_STATUS_DIRVALID_SHIFT))

#define  XENA_STATUS_MAXCHANGE_MASK 0x2
#define  XENA_STATUS_MAXCHANGE_SHIFT 1
#define  XENA_STATUS_MAXCHANGE_MASK_SHIFT(reg) (((reg) & XENA_STATUS_MAXCHANGE_MASK) >> XENA_STATUS_MAXCHANGE_SHIFT)
#define  XENA_STATUS_MAXCHANGE_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_MAXCHANGE_MASK) | (((uint32_t)val) << XENA_STATUS_MAXCHANGE_SHIFT))

#define  XENA_STATUS_MINCHANGE_MASK 0x1
#define  XENA_STATUS_MINCHANGE_SHIFT 0
#define  XENA_STATUS_MINCHANGE_MASK_SHIFT(reg) (((reg) & XENA_STATUS_MINCHANGE_MASK) >> XENA_STATUS_MINCHANGE_SHIFT)
#define  XENA_STATUS_MINCHANGE_REPLACE_VAL(reg,val) (((reg) & ~XENA_STATUS_MINCHANGE_MASK) | (((uint32_t)val) << XENA_STATUS_MINCHANGE_SHIFT))

//====================================================================
//Register: UPC Pseudo Encoder Register (Private) (PseudoEncoder)
/** \brief The UPC Pseudo Encoder Register controls the function of the internal
      generation of synthetic quadrature encoder signals. When enabled,
      the pseudo encoder synchronized to the current state of the encoder
      inputs and then begins "motion" according the period and direction
      values programmed here. When disabled, the pseudo encoder continues
      "moving" until it reaches the same state as the encoder inputs then
      stops. Note: The pseudo encoder signals are substituted after
      the Encoder Unit's filter, so the Encoder Unit's inverting
      and swapping does not apply.*/
//====================================================================

#define  XENA_PSEUDOENCODER_RESERVED1_MASK 0xfe000000
#define  XENA_PSEUDOENCODER_RESERVED1_SHIFT 25
#define  XENA_PSEUDOENCODER_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PSEUDOENCODER_RESERVED1_MASK) >> XENA_PSEUDOENCODER_RESERVED1_SHIFT)
#define  XENA_PSEUDOENCODER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PSEUDOENCODER_RESERVED1_MASK) | (((uint32_t)val) << XENA_PSEUDOENCODER_RESERVED1_SHIFT))

#define  XENA_PSEUDOENCODER_DIR_MASK 0x1000000
#define  XENA_PSEUDOENCODER_DIR_SHIFT 24
#define  XENA_PSEUDOENCODER_DIR_MASK_SHIFT(reg) (((reg) & XENA_PSEUDOENCODER_DIR_MASK) >> XENA_PSEUDOENCODER_DIR_SHIFT)
#define  XENA_PSEUDOENCODER_DIR_REPLACE_VAL(reg,val) (((reg) & ~XENA_PSEUDOENCODER_DIR_MASK) | (((uint32_t)val) << XENA_PSEUDOENCODER_DIR_SHIFT))

#define  XENA_PSEUDOENCODER_PHASE_MASK 0xffffff
#define  XENA_PSEUDOENCODER_PHASE_SHIFT 0
#define  XENA_PSEUDOENCODER_PHASE_MASK_SHIFT(reg) (((reg) & XENA_PSEUDOENCODER_PHASE_MASK) >> XENA_PSEUDOENCODER_PHASE_SHIFT)
#define  XENA_PSEUDOENCODER_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PSEUDOENCODER_PHASE_MASK) | (((uint32_t)val) << XENA_PSEUDOENCODER_PHASE_SHIFT))

//====================================================================
//Register: UPCIO IO LFSR (CRC) Register (Private) (LFSR)
/** \brief Each IO Cell contains an LFSR that provides a CRC signature of all bits
      output by that IO Cell. The LFSR is advanced and the current output
      of the IO added when the respective data source's clock enable asserts.
      This prevents a single datum from being "counted" twice in the
      signature. This works for all IO Cell data sources except for
      register "bit-bang" mode.
      Each cell is addressed from this one register depending on the
      address used when accessing this register. E.g. LFSR[0] accesses
      IO Cell 0's LFSR; LFSR[1] accesses IO Cell 1's LFSR.*/
//====================================================================

#define  XENA_LFSR_RESERVED1_MASK 0xffff0000
#define  XENA_LFSR_RESERVED1_SHIFT 16
#define  XENA_LFSR_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_LFSR_RESERVED1_MASK) >> XENA_LFSR_RESERVED1_SHIFT)
#define  XENA_LFSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_LFSR_RESERVED1_MASK) | (((uint32_t)val) << XENA_LFSR_RESERVED1_SHIFT))

#define  XENA_LFSR_SIG_MASK 0xffff
#define  XENA_LFSR_SIG_SHIFT 0
#define  XENA_LFSR_SIG_MASK_SHIFT(reg) (((reg) & XENA_LFSR_SIG_MASK) >> XENA_LFSR_SIG_SHIFT)
#define  XENA_LFSR_SIG_REPLACE_VAL(reg,val) (((reg) & ~XENA_LFSR_SIG_MASK) | (((uint32_t)val) << XENA_LFSR_SIG_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Regular Interrupt Enable (Private) (Instance 1 of 4) (IRQEnableFM0)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_IRQENABLEFM0_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEFM0_ENABLE_SHIFT 0
#define  XENA_IRQENABLEFM0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEFM0_ENABLE_MASK) >> XENA_IRQENABLEFM0_ENABLE_SHIFT)
#define  XENA_IRQENABLEFM0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEFM0_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEFM0_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Regular Interrupt Enable (Private) (Instance 2 of 4) (IRQEnableFM1)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_IRQENABLEFM1_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEFM1_ENABLE_SHIFT 0
#define  XENA_IRQENABLEFM1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEFM1_ENABLE_MASK) >> XENA_IRQENABLEFM1_ENABLE_SHIFT)
#define  XENA_IRQENABLEFM1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEFM1_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEFM1_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Regular Interrupt Enable (Private) (Instance 3 of 4) (IRQEnableFM2)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_IRQENABLEFM2_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEFM2_ENABLE_SHIFT 0
#define  XENA_IRQENABLEFM2_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEFM2_ENABLE_MASK) >> XENA_IRQENABLEFM2_ENABLE_SHIFT)
#define  XENA_IRQENABLEFM2_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEFM2_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEFM2_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Regular Interrupt Enable (Private) (Instance 4 of 4) (IRQEnableFM3)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_IRQENABLEFM3_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEFM3_ENABLE_SHIFT 0
#define  XENA_IRQENABLEFM3_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEFM3_ENABLE_MASK) >> XENA_IRQENABLEFM3_ENABLE_SHIFT)
#define  XENA_IRQENABLEFM3_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEFM3_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEFM3_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Fast Interrupt Enable (Private) (Instance 1 of 4) (FIQEnableFM0)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_FIQENABLEFM0_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEFM0_ENABLE_SHIFT 0
#define  XENA_FIQENABLEFM0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEFM0_ENABLE_MASK) >> XENA_FIQENABLEFM0_ENABLE_SHIFT)
#define  XENA_FIQENABLEFM0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEFM0_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEFM0_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Fast Interrupt Enable (Private) (Instance 2 of 4) (FIQEnableFM1)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_FIQENABLEFM1_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEFM1_ENABLE_SHIFT 0
#define  XENA_FIQENABLEFM1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEFM1_ENABLE_MASK) >> XENA_FIQENABLEFM1_ENABLE_SHIFT)
#define  XENA_FIQENABLEFM1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEFM1_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEFM1_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Fast Interrupt Enable (Private) (Instance 3 of 4) (FIQEnableFM2)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_FIQENABLEFM2_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEFM2_ENABLE_SHIFT 0
#define  XENA_FIQENABLEFM2_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEFM2_ENABLE_MASK) >> XENA_FIQENABLEFM2_ENABLE_SHIFT)
#define  XENA_FIQENABLEFM2_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEFM2_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEFM2_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Fast Interrupt Enable (Private) (Instance 4 of 4) (FIQEnableFM3)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the Formatter CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_FIQENABLEFM3_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEFM3_ENABLE_SHIFT 0
#define  XENA_FIQENABLEFM3_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEFM3_ENABLE_MASK) >> XENA_FIQENABLEFM3_ENABLE_SHIFT)
#define  XENA_FIQENABLEFM3_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEFM3_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEFM3_ENABLE_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Posted (Private) (Instance 1 of 4) (IntPostFM0)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_INTPOSTFM0_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTFM0_POSTED_SHIFT 0
#define  XENA_INTPOSTFM0_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTFM0_POSTED_MASK) >> XENA_INTPOSTFM0_POSTED_SHIFT)
#define  XENA_INTPOSTFM0_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTFM0_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTFM0_POSTED_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Posted (Private) (Instance 2 of 4) (IntPostFM1)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_INTPOSTFM1_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTFM1_POSTED_SHIFT 0
#define  XENA_INTPOSTFM1_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTFM1_POSTED_MASK) >> XENA_INTPOSTFM1_POSTED_SHIFT)
#define  XENA_INTPOSTFM1_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTFM1_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTFM1_POSTED_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Posted (Private) (Instance 3 of 4) (IntPostFM2)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_INTPOSTFM2_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTFM2_POSTED_SHIFT 0
#define  XENA_INTPOSTFM2_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTFM2_POSTED_MASK) >> XENA_INTPOSTFM2_POSTED_SHIFT)
#define  XENA_INTPOSTFM2_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTFM2_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTFM2_POSTED_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Posted (Private) (Instance 4 of 4) (IntPostFM3)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_INTPOSTFM3_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTFM3_POSTED_SHIFT 0
#define  XENA_INTPOSTFM3_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTFM3_POSTED_MASK) >> XENA_INTPOSTFM3_POSTED_SHIFT)
#define  XENA_INTPOSTFM3_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTFM3_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTFM3_POSTED_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Clear (Private) (Instance 1 of 4) (IntClearFM0)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_INTCLEARFM0_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARFM0_CLEAR_SHIFT 0
#define  XENA_INTCLEARFM0_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARFM0_CLEAR_MASK) >> XENA_INTCLEARFM0_CLEAR_SHIFT)
#define  XENA_INTCLEARFM0_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARFM0_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARFM0_CLEAR_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Clear (Private) (Instance 2 of 4) (IntClearFM1)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_INTCLEARFM1_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARFM1_CLEAR_SHIFT 0
#define  XENA_INTCLEARFM1_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARFM1_CLEAR_MASK) >> XENA_INTCLEARFM1_CLEAR_SHIFT)
#define  XENA_INTCLEARFM1_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARFM1_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARFM1_CLEAR_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Clear (Private) (Instance 3 of 4) (IntClearFM2)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_INTCLEARFM2_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARFM2_CLEAR_SHIFT 0
#define  XENA_INTCLEARFM2_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARFM2_CLEAR_MASK) >> XENA_INTCLEARFM2_CLEAR_SHIFT)
#define  XENA_INTCLEARFM2_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARFM2_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARFM2_CLEAR_SHIFT))

//====================================================================
//Register: UPC Formatter CPU Interrupt Clear (Private) (Instance 4 of 4) (IntClearFM3)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_INTCLEARFM3_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARFM3_CLEAR_SHIFT 0
#define  XENA_INTCLEARFM3_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARFM3_CLEAR_MASK) >> XENA_INTCLEARFM3_CLEAR_SHIFT)
#define  XENA_INTCLEARFM3_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARFM3_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARFM3_CLEAR_SHIFT))

//====================================================================
//Register: UPC I/O CPU Regular Interrupt Enable (Private) (Instance 1 of 4) (IRQEnableIO0)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_IRQENABLEIO0_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEIO0_ENABLE_SHIFT 0
#define  XENA_IRQENABLEIO0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEIO0_ENABLE_MASK) >> XENA_IRQENABLEIO0_ENABLE_SHIFT)
#define  XENA_IRQENABLEIO0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEIO0_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEIO0_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Regular Interrupt Enable (Private) (Instance 2 of 4) (IRQEnableIO1)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_IRQENABLEIO1_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEIO1_ENABLE_SHIFT 0
#define  XENA_IRQENABLEIO1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEIO1_ENABLE_MASK) >> XENA_IRQENABLEIO1_ENABLE_SHIFT)
#define  XENA_IRQENABLEIO1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEIO1_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEIO1_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Regular Interrupt Enable (Private) (Instance 3 of 4) (IRQEnableIO2)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_IRQENABLEIO2_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEIO2_ENABLE_SHIFT 0
#define  XENA_IRQENABLEIO2_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEIO2_ENABLE_MASK) >> XENA_IRQENABLEIO2_ENABLE_SHIFT)
#define  XENA_IRQENABLEIO2_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEIO2_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEIO2_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Regular Interrupt Enable (Private) (Instance 4 of 4) (IRQEnableIO3)
/** \brief The Regular Interrupt Enable register enables an interrupt source
      to post a regular (IRQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_IRQENABLEIO3_ENABLE_MASK 0xffffffff
#define  XENA_IRQENABLEIO3_ENABLE_SHIFT 0
#define  XENA_IRQENABLEIO3_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_IRQENABLEIO3_ENABLE_MASK) >> XENA_IRQENABLEIO3_ENABLE_SHIFT)
#define  XENA_IRQENABLEIO3_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_IRQENABLEIO3_ENABLE_MASK) | (((uint32_t)val) << XENA_IRQENABLEIO3_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Fast Interrupt Enable (Private) (Instance 1 of 4) (FIQEnableIO0)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_FIQENABLEIO0_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEIO0_ENABLE_SHIFT 0
#define  XENA_FIQENABLEIO0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEIO0_ENABLE_MASK) >> XENA_FIQENABLEIO0_ENABLE_SHIFT)
#define  XENA_FIQENABLEIO0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEIO0_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEIO0_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Fast Interrupt Enable (Private) (Instance 2 of 4) (FIQEnableIO1)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_FIQENABLEIO1_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEIO1_ENABLE_SHIFT 0
#define  XENA_FIQENABLEIO1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEIO1_ENABLE_MASK) >> XENA_FIQENABLEIO1_ENABLE_SHIFT)
#define  XENA_FIQENABLEIO1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEIO1_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEIO1_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Fast Interrupt Enable (Private) (Instance 3 of 4) (FIQEnableIO2)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_FIQENABLEIO2_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEIO2_ENABLE_SHIFT 0
#define  XENA_FIQENABLEIO2_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEIO2_ENABLE_MASK) >> XENA_FIQENABLEIO2_ENABLE_SHIFT)
#define  XENA_FIQENABLEIO2_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEIO2_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEIO2_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Fast Interrupt Enable (Private) (Instance 4 of 4) (FIQEnableIO3)
/** \brief The Fast Interrupt Enable register enables an interrupt source
      to post a fast (FIQ) interrupt to the I/O CPU.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_FIQENABLEIO3_ENABLE_MASK 0xffffffff
#define  XENA_FIQENABLEIO3_ENABLE_SHIFT 0
#define  XENA_FIQENABLEIO3_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_FIQENABLEIO3_ENABLE_MASK) >> XENA_FIQENABLEIO3_ENABLE_SHIFT)
#define  XENA_FIQENABLEIO3_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_FIQENABLEIO3_ENABLE_MASK) | (((uint32_t)val) << XENA_FIQENABLEIO3_ENABLE_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Posted (Private) (Instance 1 of 4) (IntPostIO0)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_INTPOSTIO0_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTIO0_POSTED_SHIFT 0
#define  XENA_INTPOSTIO0_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTIO0_POSTED_MASK) >> XENA_INTPOSTIO0_POSTED_SHIFT)
#define  XENA_INTPOSTIO0_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTIO0_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTIO0_POSTED_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Posted (Private) (Instance 2 of 4) (IntPostIO1)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_INTPOSTIO1_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTIO1_POSTED_SHIFT 0
#define  XENA_INTPOSTIO1_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTIO1_POSTED_MASK) >> XENA_INTPOSTIO1_POSTED_SHIFT)
#define  XENA_INTPOSTIO1_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTIO1_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTIO1_POSTED_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Posted (Private) (Instance 3 of 4) (IntPostIO2)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_INTPOSTIO2_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTIO2_POSTED_SHIFT 0
#define  XENA_INTPOSTIO2_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTIO2_POSTED_MASK) >> XENA_INTPOSTIO2_POSTED_SHIFT)
#define  XENA_INTPOSTIO2_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTIO2_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTIO2_POSTED_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Posted (Private) (Instance 4 of 4) (IntPostIO3)
/** \brief The Posted register is used by firmware to determine the source of an
      interrupt exception.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_INTPOSTIO3_POSTED_MASK 0xffffffff
#define  XENA_INTPOSTIO3_POSTED_SHIFT 0
#define  XENA_INTPOSTIO3_POSTED_MASK_SHIFT(reg) (((reg) & XENA_INTPOSTIO3_POSTED_MASK) >> XENA_INTPOSTIO3_POSTED_SHIFT)
#define  XENA_INTPOSTIO3_POSTED_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTPOSTIO3_POSTED_MASK) | (((uint32_t)val) << XENA_INTPOSTIO3_POSTED_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Clear (Private) (Instance 1 of 4) (IntClearIO0)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 1 of 4)*/
//====================================================================

#define  XENA_INTCLEARIO0_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARIO0_CLEAR_SHIFT 0
#define  XENA_INTCLEARIO0_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARIO0_CLEAR_MASK) >> XENA_INTCLEARIO0_CLEAR_SHIFT)
#define  XENA_INTCLEARIO0_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARIO0_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARIO0_CLEAR_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Clear (Private) (Instance 2 of 4) (IntClearIO1)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 2 of 4)*/
//====================================================================

#define  XENA_INTCLEARIO1_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARIO1_CLEAR_SHIFT 0
#define  XENA_INTCLEARIO1_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARIO1_CLEAR_MASK) >> XENA_INTCLEARIO1_CLEAR_SHIFT)
#define  XENA_INTCLEARIO1_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARIO1_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARIO1_CLEAR_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Clear (Private) (Instance 3 of 4) (IntClearIO2)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 3 of 4)*/
//====================================================================

#define  XENA_INTCLEARIO2_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARIO2_CLEAR_SHIFT 0
#define  XENA_INTCLEARIO2_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARIO2_CLEAR_MASK) >> XENA_INTCLEARIO2_CLEAR_SHIFT)
#define  XENA_INTCLEARIO2_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARIO2_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARIO2_CLEAR_SHIFT))

//====================================================================
//Register: UPC I/O CPU Interrupt Clear (Private) (Instance 4 of 4) (IntClearIO3)
/** \brief The Clear register is used by firmware to clear a posted interrupt.
      Note that Instance 1 refers to interrupt sources 0-31; Instance 2
      refers to interrupt sources 32-63, and so forth. Refer to the UPC
      Interrupt Sources table for a mapping of interrupt sources. (Instance 4 of 4)*/
//====================================================================

#define  XENA_INTCLEARIO3_CLEAR_MASK 0xffffffff
#define  XENA_INTCLEARIO3_CLEAR_SHIFT 0
#define  XENA_INTCLEARIO3_CLEAR_MASK_SHIFT(reg) (((reg) & XENA_INTCLEARIO3_CLEAR_MASK) >> XENA_INTCLEARIO3_CLEAR_SHIFT)
#define  XENA_INTCLEARIO3_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~XENA_INTCLEARIO3_CLEAR_MASK) | (((uint32_t)val) << XENA_INTCLEARIO3_CLEAR_SHIFT))

//====================================================================
//Register: UPC Mailbox 0 Register (Mailbox0)
/** \brief The Mailbox 0 register is a general purpose mailbox.*/
//====================================================================

#define  XENA_MAILBOX0_DATA_MASK 0xffffffff
#define  XENA_MAILBOX0_DATA_SHIFT 0
#define  XENA_MAILBOX0_DATA_MASK_SHIFT(reg) (((reg) & XENA_MAILBOX0_DATA_MASK) >> XENA_MAILBOX0_DATA_SHIFT)
#define  XENA_MAILBOX0_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_MAILBOX0_DATA_MASK) | (((uint32_t)val) << XENA_MAILBOX0_DATA_SHIFT))

//====================================================================
//Register: UPC Mailbox 1 Register (Mailbox1)
/** \brief The Mailbox 1 register is a general purpose mailbox.*/
//====================================================================

#define  XENA_MAILBOX1_DATA_MASK 0xffffffff
#define  XENA_MAILBOX1_DATA_SHIFT 0
#define  XENA_MAILBOX1_DATA_MASK_SHIFT(reg) (((reg) & XENA_MAILBOX1_DATA_MASK) >> XENA_MAILBOX1_DATA_SHIFT)
#define  XENA_MAILBOX1_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_MAILBOX1_DATA_MASK) | (((uint32_t)val) << XENA_MAILBOX1_DATA_SHIFT))

//====================================================================
//Register: UPC Mailbox 2 Register (Private) (Mailbox2)
/** \brief The Mailbox 2 register is a general purpose mailbox.*/
//====================================================================

#define  XENA_MAILBOX2_DATA_MASK 0xffffffff
#define  XENA_MAILBOX2_DATA_SHIFT 0
#define  XENA_MAILBOX2_DATA_MASK_SHIFT(reg) (((reg) & XENA_MAILBOX2_DATA_MASK) >> XENA_MAILBOX2_DATA_SHIFT)
#define  XENA_MAILBOX2_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_MAILBOX2_DATA_MASK) | (((uint32_t)val) << XENA_MAILBOX2_DATA_SHIFT))

//====================================================================
//Register: UPC Mailbox 3 Register (Private) (Mailbox3)
/** \brief The Mailbox 3 register is a general purpose mailbox.*/
//====================================================================

#define  XENA_MAILBOX3_DATA_MASK 0xffffffff
#define  XENA_MAILBOX3_DATA_SHIFT 0
#define  XENA_MAILBOX3_DATA_MASK_SHIFT(reg) (((reg) & XENA_MAILBOX3_DATA_MASK) >> XENA_MAILBOX3_DATA_SHIFT)
#define  XENA_MAILBOX3_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_MAILBOX3_DATA_MASK) | (((uint32_t)val) << XENA_MAILBOX3_DATA_SHIFT))

//====================================================================
//Register: UPC Mailbox 4 Register (Private) (Mailbox4)
/** \brief The Mailbox 4 register is a general purpose mailbox.*/
//====================================================================

#define  XENA_MAILBOX4_DATA_MASK 0xffffffff
#define  XENA_MAILBOX4_DATA_SHIFT 0
#define  XENA_MAILBOX4_DATA_MASK_SHIFT(reg) (((reg) & XENA_MAILBOX4_DATA_MASK) >> XENA_MAILBOX4_DATA_SHIFT)
#define  XENA_MAILBOX4_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_MAILBOX4_DATA_MASK) | (((uint32_t)val) << XENA_MAILBOX4_DATA_SHIFT))

//====================================================================
//Register: UPC Mailbox 5 Register (Private) (Mailbox5)
/** \brief The Mailbox 5 register is a general purpose mailbox.*/
//====================================================================

#define  XENA_MAILBOX5_DATA_MASK 0xffffffff
#define  XENA_MAILBOX5_DATA_SHIFT 0
#define  XENA_MAILBOX5_DATA_MASK_SHIFT(reg) (((reg) & XENA_MAILBOX5_DATA_MASK) >> XENA_MAILBOX5_DATA_SHIFT)
#define  XENA_MAILBOX5_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_MAILBOX5_DATA_MASK) | (((uint32_t)val) << XENA_MAILBOX5_DATA_SHIFT))

//====================================================================
//Register: UPC Timer 0 Register (Private) (Timer0)
/** \brief Timer 0 is a general purpose timer.*/
//====================================================================

#define  XENA_TIMER0_DATA_MASK 0xffffffff
#define  XENA_TIMER0_DATA_SHIFT 0
#define  XENA_TIMER0_DATA_MASK_SHIFT(reg) (((reg) & XENA_TIMER0_DATA_MASK) >> XENA_TIMER0_DATA_SHIFT)
#define  XENA_TIMER0_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_TIMER0_DATA_MASK) | (((uint32_t)val) << XENA_TIMER0_DATA_SHIFT))

//====================================================================
//Register: UPC Timer 1 Register (Private) (Timer1)
/** \brief Timer 1 is a general purpose timer.*/
//====================================================================

#define  XENA_TIMER1_DATA_MASK 0xffffffff
#define  XENA_TIMER1_DATA_SHIFT 0
#define  XENA_TIMER1_DATA_MASK_SHIFT(reg) (((reg) & XENA_TIMER1_DATA_MASK) >> XENA_TIMER1_DATA_SHIFT)
#define  XENA_TIMER1_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_TIMER1_DATA_MASK) | (((uint32_t)val) << XENA_TIMER1_DATA_SHIFT))

//====================================================================
//Register: UPC Timer 2 Register (Private) (Timer2)
/** \brief Timer 2 is a general purpose timer.*/
//====================================================================

#define  XENA_TIMER2_DATA_MASK 0xffffffff
#define  XENA_TIMER2_DATA_SHIFT 0
#define  XENA_TIMER2_DATA_MASK_SHIFT(reg) (((reg) & XENA_TIMER2_DATA_MASK) >> XENA_TIMER2_DATA_SHIFT)
#define  XENA_TIMER2_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_TIMER2_DATA_MASK) | (((uint32_t)val) << XENA_TIMER2_DATA_SHIFT))

//====================================================================
//Register: UPC Timer 3 Register (Private) (Timer3)
/** \brief Timer 3 is a general purpose timer.*/
//====================================================================

#define  XENA_TIMER3_DATA_MASK 0xffffffff
#define  XENA_TIMER3_DATA_SHIFT 0
#define  XENA_TIMER3_DATA_MASK_SHIFT(reg) (((reg) & XENA_TIMER3_DATA_MASK) >> XENA_TIMER3_DATA_SHIFT)
#define  XENA_TIMER3_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_TIMER3_DATA_MASK) | (((uint32_t)val) << XENA_TIMER3_DATA_SHIFT))

//====================================================================
//Register: UPC General Purpose Status Register (GPStatus)
/** \brief The General Purpose Status Register is used by UPC to communicate its
      status back to the system. The contents of this register are
      application specific according to the application's needs.  The
      contents of this register are sent to the system, and it is expected
      that at least some of the bits of this register are routed to the
      system-level interrupt controller to provide UPC with a means for
      interrupting the system-level CPU.*/
//====================================================================

#define  XENA_GPSTATUS_DATA_MASK 0xffffffff
#define  XENA_GPSTATUS_DATA_SHIFT 0
#define  XENA_GPSTATUS_DATA_MASK_SHIFT(reg) (((reg) & XENA_GPSTATUS_DATA_MASK) >> XENA_GPSTATUS_DATA_SHIFT)
#define  XENA_GPSTATUS_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_GPSTATUS_DATA_MASK) | (((uint32_t)val) << XENA_GPSTATUS_DATA_SHIFT))

//====================================================================
//Register: UPC CPU Soft Reset Register (CPUSoftReset)
/** \brief The CPU Soft Reset Register is used by the System CPU to
      reset the UPC CPUs.*/
//====================================================================

#define  XENA_CPUSOFTRESET_BURDEBBYPASS_MASK 0x80000000
#define  XENA_CPUSOFTRESET_BURDEBBYPASS_SHIFT 31
#define  XENA_CPUSOFTRESET_BURDEBBYPASS_MASK_SHIFT(reg) (((reg) & XENA_CPUSOFTRESET_BURDEBBYPASS_MASK) >> XENA_CPUSOFTRESET_BURDEBBYPASS_SHIFT)
#define  XENA_CPUSOFTRESET_BURDEBBYPASS_REPLACE_VAL(reg,val) (((reg) & ~XENA_CPUSOFTRESET_BURDEBBYPASS_MASK) | (((uint32_t)val) << XENA_CPUSOFTRESET_BURDEBBYPASS_SHIFT))

#define  XENA_CPUSOFTRESET_RESERVED1_MASK 0x7ffffffe
#define  XENA_CPUSOFTRESET_RESERVED1_SHIFT 1
#define  XENA_CPUSOFTRESET_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_CPUSOFTRESET_RESERVED1_MASK) >> XENA_CPUSOFTRESET_RESERVED1_SHIFT)
#define  XENA_CPUSOFTRESET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_CPUSOFTRESET_RESERVED1_MASK) | (((uint32_t)val) << XENA_CPUSOFTRESET_RESERVED1_SHIFT))

#define  XENA_CPUSOFTRESET_NRESET_MASK 0x1
#define  XENA_CPUSOFTRESET_NRESET_SHIFT 0
#define  XENA_CPUSOFTRESET_NRESET_MASK_SHIFT(reg) (((reg) & XENA_CPUSOFTRESET_NRESET_MASK) >> XENA_CPUSOFTRESET_NRESET_SHIFT)
#define  XENA_CPUSOFTRESET_NRESET_REPLACE_VAL(reg,val) (((reg) & ~XENA_CPUSOFTRESET_NRESET_MASK) | (((uint32_t)val) << XENA_CPUSOFTRESET_NRESET_SHIFT))

//====================================================================
//Register: UPC Encoder Phase 0 Reference Register (PhaseRef)
/** \brief The UPC Encoder Phase 0 Reference Register provides firmware access to
      the latest position for phase 0 at the time that the latest update to the Phase[3:0] registers.*/
//====================================================================

#define  XENA_PHASEREF_RESERVED1_MASK 0xff000000
#define  XENA_PHASEREF_RESERVED1_SHIFT 24
#define  XENA_PHASEREF_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PHASEREF_RESERVED1_MASK) >> XENA_PHASEREF_RESERVED1_SHIFT)
#define  XENA_PHASEREF_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASEREF_RESERVED1_MASK) | (((uint32_t)val) << XENA_PHASEREF_RESERVED1_SHIFT))

#define  XENA_PHASEREF_PHASE123_MASK 0xffffff
#define  XENA_PHASEREF_PHASE123_SHIFT 0
#define  XENA_PHASEREF_PHASE123_MASK_SHIFT(reg) (((reg) & XENA_PHASEREF_PHASE123_MASK) >> XENA_PHASEREF_PHASE123_SHIFT)
#define  XENA_PHASEREF_PHASE123_REPLACE_VAL(reg,val) (((reg) & ~XENA_PHASEREF_PHASE123_MASK) | (((uint32_t)val) << XENA_PHASEREF_PHASE123_SHIFT))

//====================================================================
//Register: UPCIO Reset Register (Private) (XCT)
/** \brief UPCIO Reset Register*/
//====================================================================

#define  XENA_XCT_RESERVED1_MASK 0xfffffe00
#define  XENA_XCT_RESERVED1_SHIFT 9
#define  XENA_XCT_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XCT_RESERVED1_MASK) >> XENA_XCT_RESERVED1_SHIFT)
#define  XENA_XCT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCT_RESERVED1_MASK) | (((uint32_t)val) << XENA_XCT_RESERVED1_SHIFT))

#define  XENA_XCT_S_MASK 0x100
#define  XENA_XCT_S_SHIFT 8
#define  XENA_XCT_S_MASK_SHIFT(reg) (((reg) & XENA_XCT_S_MASK) >> XENA_XCT_S_SHIFT)
#define  XENA_XCT_S_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCT_S_MASK) | (((uint32_t)val) << XENA_XCT_S_SHIFT))

#define  XENA_XCT_RESERVED2_MASK 0xfe
#define  XENA_XCT_RESERVED2_SHIFT 1
#define  XENA_XCT_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XCT_RESERVED2_MASK) >> XENA_XCT_RESERVED2_SHIFT)
#define  XENA_XCT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCT_RESERVED2_MASK) | (((uint32_t)val) << XENA_XCT_RESERVED2_SHIFT))

#define  XENA_XCT_STARTCLK_MASK 0x1
#define  XENA_XCT_STARTCLK_SHIFT 0
#define  XENA_XCT_STARTCLK_MASK_SHIFT(reg) (((reg) & XENA_XCT_STARTCLK_MASK) >> XENA_XCT_STARTCLK_SHIFT)
#define  XENA_XCT_STARTCLK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XCT_STARTCLK_MASK) | (((uint32_t)val) << XENA_XCT_STARTCLK_SHIFT))

//====================================================================
//Register: UPCIO Clock Enable Register (Private) (XIE0)
/** \brief UPCIO clock generator Enable Register*/
//====================================================================

#define  XENA_XIE0_CLKENABLE_MASK 0xff000000
#define  XENA_XIE0_CLKENABLE_SHIFT 24
#define  XENA_XIE0_CLKENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIE0_CLKENABLE_MASK) >> XENA_XIE0_CLKENABLE_SHIFT)
#define  XENA_XIE0_CLKENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIE0_CLKENABLE_MASK) | (((uint32_t)val) << XENA_XIE0_CLKENABLE_SHIFT))

#define  XENA_XIE0_FIREENABLE_MASK 0xff0000
#define  XENA_XIE0_FIREENABLE_SHIFT 16
#define  XENA_XIE0_FIREENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIE0_FIREENABLE_MASK) >> XENA_XIE0_FIREENABLE_SHIFT)
#define  XENA_XIE0_FIREENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIE0_FIREENABLE_MASK) | (((uint32_t)val) << XENA_XIE0_FIREENABLE_SHIFT))

#define  XENA_XIE0_SYNCENABLE_MASK 0xff00
#define  XENA_XIE0_SYNCENABLE_SHIFT 8
#define  XENA_XIE0_SYNCENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIE0_SYNCENABLE_MASK) >> XENA_XIE0_SYNCENABLE_SHIFT)
#define  XENA_XIE0_SYNCENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIE0_SYNCENABLE_MASK) | (((uint32_t)val) << XENA_XIE0_SYNCENABLE_SHIFT))

#define  XENA_XIE0_REPEATENABLE_MASK 0xff
#define  XENA_XIE0_REPEATENABLE_SHIFT 0
#define  XENA_XIE0_REPEATENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIE0_REPEATENABLE_MASK) >> XENA_XIE0_REPEATENABLE_SHIFT)
#define  XENA_XIE0_REPEATENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIE0_REPEATENABLE_MASK) | (((uint32_t)val) << XENA_XIE0_REPEATENABLE_SHIFT))

//====================================================================
//Register: UPCIO Fire pulse selection (Private) (XIFire)
/** \brief UPCIO clock generator Fire Pulse Selection Register*/
//====================================================================

#define  XENA_XIFIRE_RESERVED1_MASK 0xff000000
#define  XENA_XIFIRE_RESERVED1_SHIFT 24
#define  XENA_XIFIRE_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_RESERVED1_MASK) >> XENA_XIFIRE_RESERVED1_SHIFT)
#define  XENA_XIFIRE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFIRE_RESERVED1_SHIFT))

#define  XENA_XIFIRE_CLKENCEN7_MASK 0xe00000
#define  XENA_XIFIRE_CLKENCEN7_SHIFT 21
#define  XENA_XIFIRE_CLKENCEN7_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN7_MASK) >> XENA_XIFIRE_CLKENCEN7_SHIFT)
#define  XENA_XIFIRE_CLKENCEN7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN7_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN7_SHIFT))

#define  XENA_XIFIRE_CLKENCEN6_MASK 0x1c0000
#define  XENA_XIFIRE_CLKENCEN6_SHIFT 18
#define  XENA_XIFIRE_CLKENCEN6_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN6_MASK) >> XENA_XIFIRE_CLKENCEN6_SHIFT)
#define  XENA_XIFIRE_CLKENCEN6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN6_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN6_SHIFT))

#define  XENA_XIFIRE_CLKENCEN5_MASK 0x38000
#define  XENA_XIFIRE_CLKENCEN5_SHIFT 15
#define  XENA_XIFIRE_CLKENCEN5_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN5_MASK) >> XENA_XIFIRE_CLKENCEN5_SHIFT)
#define  XENA_XIFIRE_CLKENCEN5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN5_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN5_SHIFT))

#define  XENA_XIFIRE_CLKENCEN4_MASK 0x7000
#define  XENA_XIFIRE_CLKENCEN4_SHIFT 12
#define  XENA_XIFIRE_CLKENCEN4_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN4_MASK) >> XENA_XIFIRE_CLKENCEN4_SHIFT)
#define  XENA_XIFIRE_CLKENCEN4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN4_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN4_SHIFT))

#define  XENA_XIFIRE_CLKENCEN3_MASK 0xe00
#define  XENA_XIFIRE_CLKENCEN3_SHIFT 9
#define  XENA_XIFIRE_CLKENCEN3_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN3_MASK) >> XENA_XIFIRE_CLKENCEN3_SHIFT)
#define  XENA_XIFIRE_CLKENCEN3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN3_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN3_SHIFT))

#define  XENA_XIFIRE_CLKENCEN2_MASK 0x1c0
#define  XENA_XIFIRE_CLKENCEN2_SHIFT 6
#define  XENA_XIFIRE_CLKENCEN2_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN2_MASK) >> XENA_XIFIRE_CLKENCEN2_SHIFT)
#define  XENA_XIFIRE_CLKENCEN2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN2_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN2_SHIFT))

#define  XENA_XIFIRE_CLKENCEN1_MASK 0x38
#define  XENA_XIFIRE_CLKENCEN1_SHIFT 3
#define  XENA_XIFIRE_CLKENCEN1_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN1_MASK) >> XENA_XIFIRE_CLKENCEN1_SHIFT)
#define  XENA_XIFIRE_CLKENCEN1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN1_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN1_SHIFT))

#define  XENA_XIFIRE_CLKENCEN0_MASK 0x7
#define  XENA_XIFIRE_CLKENCEN0_SHIFT 0
#define  XENA_XIFIRE_CLKENCEN0_MASK_SHIFT(reg) (((reg) & XENA_XIFIRE_CLKENCEN0_MASK) >> XENA_XIFIRE_CLKENCEN0_SHIFT)
#define  XENA_XIFIRE_CLKENCEN0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFIRE_CLKENCEN0_MASK) | (((uint32_t)val) << XENA_XIFIRE_CLKENCEN0_SHIFT))

//====================================================================
//Register: UPCIO Sync pulse selection (Private) (XISync)
/** \brief UPCIO clock generator Sync Pulse Selection Register*/
//====================================================================

#define  XENA_XISYNC_CLKQUEUEEN_MASK 0xff000000
#define  XENA_XISYNC_CLKQUEUEEN_SHIFT 24
#define  XENA_XISYNC_CLKQUEUEEN_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKQUEUEEN_MASK) >> XENA_XISYNC_CLKQUEUEEN_SHIFT)
#define  XENA_XISYNC_CLKQUEUEEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKQUEUEEN_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKQUEUEEN_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN7_MASK 0xe00000
#define  XENA_XISYNC_CLKSYNCEN7_SHIFT 21
#define  XENA_XISYNC_CLKSYNCEN7_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN7_MASK) >> XENA_XISYNC_CLKSYNCEN7_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN7_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN7_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN6_MASK 0x1c0000
#define  XENA_XISYNC_CLKSYNCEN6_SHIFT 18
#define  XENA_XISYNC_CLKSYNCEN6_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN6_MASK) >> XENA_XISYNC_CLKSYNCEN6_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN6_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN6_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN5_MASK 0x38000
#define  XENA_XISYNC_CLKSYNCEN5_SHIFT 15
#define  XENA_XISYNC_CLKSYNCEN5_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN5_MASK) >> XENA_XISYNC_CLKSYNCEN5_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN5_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN5_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN4_MASK 0x7000
#define  XENA_XISYNC_CLKSYNCEN4_SHIFT 12
#define  XENA_XISYNC_CLKSYNCEN4_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN4_MASK) >> XENA_XISYNC_CLKSYNCEN4_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN4_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN4_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN3_MASK 0xe00
#define  XENA_XISYNC_CLKSYNCEN3_SHIFT 9
#define  XENA_XISYNC_CLKSYNCEN3_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN3_MASK) >> XENA_XISYNC_CLKSYNCEN3_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN3_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN3_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN2_MASK 0x1c0
#define  XENA_XISYNC_CLKSYNCEN2_SHIFT 6
#define  XENA_XISYNC_CLKSYNCEN2_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN2_MASK) >> XENA_XISYNC_CLKSYNCEN2_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN2_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN2_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN1_MASK 0x38
#define  XENA_XISYNC_CLKSYNCEN1_SHIFT 3
#define  XENA_XISYNC_CLKSYNCEN1_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN1_MASK) >> XENA_XISYNC_CLKSYNCEN1_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN1_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN1_SHIFT))

#define  XENA_XISYNC_CLKSYNCEN0_MASK 0x7
#define  XENA_XISYNC_CLKSYNCEN0_SHIFT 0
#define  XENA_XISYNC_CLKSYNCEN0_MASK_SHIFT(reg) (((reg) & XENA_XISYNC_CLKSYNCEN0_MASK) >> XENA_XISYNC_CLKSYNCEN0_SHIFT)
#define  XENA_XISYNC_CLKSYNCEN0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNC_CLKSYNCEN0_MASK) | (((uint32_t)val) << XENA_XISYNC_CLKSYNCEN0_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 1 of 8) (XIC0)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 1 of 8)*/
//====================================================================

#define  XENA_XIC0_COUNT_MASK 0xffff0000
#define  XENA_XIC0_COUNT_SHIFT 16
#define  XENA_XIC0_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC0_COUNT_MASK) >> XENA_XIC0_COUNT_SHIFT)
#define  XENA_XIC0_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC0_COUNT_MASK) | (((uint32_t)val) << XENA_XIC0_COUNT_SHIFT))

#define  XENA_XIC0_DELAY_MASK 0xff00
#define  XENA_XIC0_DELAY_SHIFT 8
#define  XENA_XIC0_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC0_DELAY_MASK) >> XENA_XIC0_DELAY_SHIFT)
#define  XENA_XIC0_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC0_DELAY_MASK) | (((uint32_t)val) << XENA_XIC0_DELAY_SHIFT))

#define  XENA_XIC0_DIVIDE_MASK 0xff
#define  XENA_XIC0_DIVIDE_SHIFT 0
#define  XENA_XIC0_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC0_DIVIDE_MASK) >> XENA_XIC0_DIVIDE_SHIFT)
#define  XENA_XIC0_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC0_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC0_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 2 of 8) (XIC1)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 2 of 8)*/
//====================================================================

#define  XENA_XIC1_COUNT_MASK 0xffff0000
#define  XENA_XIC1_COUNT_SHIFT 16
#define  XENA_XIC1_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC1_COUNT_MASK) >> XENA_XIC1_COUNT_SHIFT)
#define  XENA_XIC1_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC1_COUNT_MASK) | (((uint32_t)val) << XENA_XIC1_COUNT_SHIFT))

#define  XENA_XIC1_DELAY_MASK 0xff00
#define  XENA_XIC1_DELAY_SHIFT 8
#define  XENA_XIC1_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC1_DELAY_MASK) >> XENA_XIC1_DELAY_SHIFT)
#define  XENA_XIC1_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC1_DELAY_MASK) | (((uint32_t)val) << XENA_XIC1_DELAY_SHIFT))

#define  XENA_XIC1_DIVIDE_MASK 0xff
#define  XENA_XIC1_DIVIDE_SHIFT 0
#define  XENA_XIC1_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC1_DIVIDE_MASK) >> XENA_XIC1_DIVIDE_SHIFT)
#define  XENA_XIC1_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC1_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC1_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 3 of 8) (XIC2)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 3 of 8)*/
//====================================================================

#define  XENA_XIC2_COUNT_MASK 0xffff0000
#define  XENA_XIC2_COUNT_SHIFT 16
#define  XENA_XIC2_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC2_COUNT_MASK) >> XENA_XIC2_COUNT_SHIFT)
#define  XENA_XIC2_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC2_COUNT_MASK) | (((uint32_t)val) << XENA_XIC2_COUNT_SHIFT))

#define  XENA_XIC2_DELAY_MASK 0xff00
#define  XENA_XIC2_DELAY_SHIFT 8
#define  XENA_XIC2_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC2_DELAY_MASK) >> XENA_XIC2_DELAY_SHIFT)
#define  XENA_XIC2_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC2_DELAY_MASK) | (((uint32_t)val) << XENA_XIC2_DELAY_SHIFT))

#define  XENA_XIC2_DIVIDE_MASK 0xff
#define  XENA_XIC2_DIVIDE_SHIFT 0
#define  XENA_XIC2_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC2_DIVIDE_MASK) >> XENA_XIC2_DIVIDE_SHIFT)
#define  XENA_XIC2_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC2_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC2_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 4 of 8) (XIC3)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 4 of 8)*/
//====================================================================

#define  XENA_XIC3_COUNT_MASK 0xffff0000
#define  XENA_XIC3_COUNT_SHIFT 16
#define  XENA_XIC3_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC3_COUNT_MASK) >> XENA_XIC3_COUNT_SHIFT)
#define  XENA_XIC3_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC3_COUNT_MASK) | (((uint32_t)val) << XENA_XIC3_COUNT_SHIFT))

#define  XENA_XIC3_DELAY_MASK 0xff00
#define  XENA_XIC3_DELAY_SHIFT 8
#define  XENA_XIC3_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC3_DELAY_MASK) >> XENA_XIC3_DELAY_SHIFT)
#define  XENA_XIC3_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC3_DELAY_MASK) | (((uint32_t)val) << XENA_XIC3_DELAY_SHIFT))

#define  XENA_XIC3_DIVIDE_MASK 0xff
#define  XENA_XIC3_DIVIDE_SHIFT 0
#define  XENA_XIC3_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC3_DIVIDE_MASK) >> XENA_XIC3_DIVIDE_SHIFT)
#define  XENA_XIC3_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC3_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC3_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 5 of 8) (XIC4)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 5 of 8)*/
//====================================================================

#define  XENA_XIC4_COUNT_MASK 0xffff0000
#define  XENA_XIC4_COUNT_SHIFT 16
#define  XENA_XIC4_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC4_COUNT_MASK) >> XENA_XIC4_COUNT_SHIFT)
#define  XENA_XIC4_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC4_COUNT_MASK) | (((uint32_t)val) << XENA_XIC4_COUNT_SHIFT))

#define  XENA_XIC4_DELAY_MASK 0xff00
#define  XENA_XIC4_DELAY_SHIFT 8
#define  XENA_XIC4_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC4_DELAY_MASK) >> XENA_XIC4_DELAY_SHIFT)
#define  XENA_XIC4_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC4_DELAY_MASK) | (((uint32_t)val) << XENA_XIC4_DELAY_SHIFT))

#define  XENA_XIC4_DIVIDE_MASK 0xff
#define  XENA_XIC4_DIVIDE_SHIFT 0
#define  XENA_XIC4_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC4_DIVIDE_MASK) >> XENA_XIC4_DIVIDE_SHIFT)
#define  XENA_XIC4_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC4_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC4_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 6 of 8) (XIC5)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 6 of 8)*/
//====================================================================

#define  XENA_XIC5_COUNT_MASK 0xffff0000
#define  XENA_XIC5_COUNT_SHIFT 16
#define  XENA_XIC5_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC5_COUNT_MASK) >> XENA_XIC5_COUNT_SHIFT)
#define  XENA_XIC5_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC5_COUNT_MASK) | (((uint32_t)val) << XENA_XIC5_COUNT_SHIFT))

#define  XENA_XIC5_DELAY_MASK 0xff00
#define  XENA_XIC5_DELAY_SHIFT 8
#define  XENA_XIC5_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC5_DELAY_MASK) >> XENA_XIC5_DELAY_SHIFT)
#define  XENA_XIC5_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC5_DELAY_MASK) | (((uint32_t)val) << XENA_XIC5_DELAY_SHIFT))

#define  XENA_XIC5_DIVIDE_MASK 0xff
#define  XENA_XIC5_DIVIDE_SHIFT 0
#define  XENA_XIC5_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC5_DIVIDE_MASK) >> XENA_XIC5_DIVIDE_SHIFT)
#define  XENA_XIC5_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC5_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC5_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 7 of 8) (XIC6)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 7 of 8)*/
//====================================================================

#define  XENA_XIC6_COUNT_MASK 0xffff0000
#define  XENA_XIC6_COUNT_SHIFT 16
#define  XENA_XIC6_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC6_COUNT_MASK) >> XENA_XIC6_COUNT_SHIFT)
#define  XENA_XIC6_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC6_COUNT_MASK) | (((uint32_t)val) << XENA_XIC6_COUNT_SHIFT))

#define  XENA_XIC6_DELAY_MASK 0xff00
#define  XENA_XIC6_DELAY_SHIFT 8
#define  XENA_XIC6_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC6_DELAY_MASK) >> XENA_XIC6_DELAY_SHIFT)
#define  XENA_XIC6_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC6_DELAY_MASK) | (((uint32_t)val) << XENA_XIC6_DELAY_SHIFT))

#define  XENA_XIC6_DIVIDE_MASK 0xff
#define  XENA_XIC6_DIVIDE_SHIFT 0
#define  XENA_XIC6_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC6_DIVIDE_MASK) >> XENA_XIC6_DIVIDE_SHIFT)
#define  XENA_XIC6_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC6_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC6_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Configuration Register (Private) (Instance 8 of 8) (XIC7)
/** \brief UPCIO ClkGen Configuration RegisterA (Instance 8 of 8)*/
//====================================================================

#define  XENA_XIC7_COUNT_MASK 0xffff0000
#define  XENA_XIC7_COUNT_SHIFT 16
#define  XENA_XIC7_COUNT_MASK_SHIFT(reg) (((reg) & XENA_XIC7_COUNT_MASK) >> XENA_XIC7_COUNT_SHIFT)
#define  XENA_XIC7_COUNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC7_COUNT_MASK) | (((uint32_t)val) << XENA_XIC7_COUNT_SHIFT))

#define  XENA_XIC7_DELAY_MASK 0xff00
#define  XENA_XIC7_DELAY_SHIFT 8
#define  XENA_XIC7_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIC7_DELAY_MASK) >> XENA_XIC7_DELAY_SHIFT)
#define  XENA_XIC7_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC7_DELAY_MASK) | (((uint32_t)val) << XENA_XIC7_DELAY_SHIFT))

#define  XENA_XIC7_DIVIDE_MASK 0xff
#define  XENA_XIC7_DIVIDE_SHIFT 0
#define  XENA_XIC7_DIVIDE_MASK_SHIFT(reg) (((reg) & XENA_XIC7_DIVIDE_MASK) >> XENA_XIC7_DIVIDE_SHIFT)
#define  XENA_XIC7_DIVIDE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIC7_DIVIDE_MASK) | (((uint32_t)val) << XENA_XIC7_DIVIDE_SHIFT))

//====================================================================
//Register: UPCIO Status Register0 (Private) (XICST0)
/** \brief UPCIO ClkGen Status Register*/
//====================================================================

#define  XENA_XICST0_FIFO7LEVEL_MASK 0xe0000000
#define  XENA_XICST0_FIFO7LEVEL_SHIFT 29
#define  XENA_XICST0_FIFO7LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO7LEVEL_MASK) >> XENA_XICST0_FIFO7LEVEL_SHIFT)
#define  XENA_XICST0_FIFO7LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO7LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO7LEVEL_SHIFT))

#define  XENA_XICST0_FIFO6LEVEL_MASK 0x1c000000
#define  XENA_XICST0_FIFO6LEVEL_SHIFT 26
#define  XENA_XICST0_FIFO6LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO6LEVEL_MASK) >> XENA_XICST0_FIFO6LEVEL_SHIFT)
#define  XENA_XICST0_FIFO6LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO6LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO6LEVEL_SHIFT))

#define  XENA_XICST0_FIFO5LEVEL_MASK 0x3800000
#define  XENA_XICST0_FIFO5LEVEL_SHIFT 23
#define  XENA_XICST0_FIFO5LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO5LEVEL_MASK) >> XENA_XICST0_FIFO5LEVEL_SHIFT)
#define  XENA_XICST0_FIFO5LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO5LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO5LEVEL_SHIFT))

#define  XENA_XICST0_FIFO4LEVEL_MASK 0x700000
#define  XENA_XICST0_FIFO4LEVEL_SHIFT 20
#define  XENA_XICST0_FIFO4LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO4LEVEL_MASK) >> XENA_XICST0_FIFO4LEVEL_SHIFT)
#define  XENA_XICST0_FIFO4LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO4LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO4LEVEL_SHIFT))

#define  XENA_XICST0_FIFO3LEVEL_MASK 0xe0000
#define  XENA_XICST0_FIFO3LEVEL_SHIFT 17
#define  XENA_XICST0_FIFO3LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO3LEVEL_MASK) >> XENA_XICST0_FIFO3LEVEL_SHIFT)
#define  XENA_XICST0_FIFO3LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO3LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO3LEVEL_SHIFT))

#define  XENA_XICST0_FIFO2LEVEL_MASK 0x1c000
#define  XENA_XICST0_FIFO2LEVEL_SHIFT 14
#define  XENA_XICST0_FIFO2LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO2LEVEL_MASK) >> XENA_XICST0_FIFO2LEVEL_SHIFT)
#define  XENA_XICST0_FIFO2LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO2LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO2LEVEL_SHIFT))

#define  XENA_XICST0_FIFO1LEVEL_MASK 0x3800
#define  XENA_XICST0_FIFO1LEVEL_SHIFT 11
#define  XENA_XICST0_FIFO1LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO1LEVEL_MASK) >> XENA_XICST0_FIFO1LEVEL_SHIFT)
#define  XENA_XICST0_FIFO1LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO1LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO1LEVEL_SHIFT))

#define  XENA_XICST0_FIFO0LEVEL_MASK 0x700
#define  XENA_XICST0_FIFO0LEVEL_SHIFT 8
#define  XENA_XICST0_FIFO0LEVEL_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFO0LEVEL_MASK) >> XENA_XICST0_FIFO0LEVEL_SHIFT)
#define  XENA_XICST0_FIFO0LEVEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFO0LEVEL_MASK) | (((uint32_t)val) << XENA_XICST0_FIFO0LEVEL_SHIFT))

#define  XENA_XICST0_RESERVED1_MASK 0x80
#define  XENA_XICST0_RESERVED1_SHIFT 7
#define  XENA_XICST0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XICST0_RESERVED1_MASK) >> XENA_XICST0_RESERVED1_SHIFT)
#define  XENA_XICST0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XICST0_RESERVED1_SHIFT))

#define  XENA_XICST0_FIFOUF_MASK 0x40
#define  XENA_XICST0_FIFOUF_SHIFT 6
#define  XENA_XICST0_FIFOUF_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFOUF_MASK) >> XENA_XICST0_FIFOUF_SHIFT)
#define  XENA_XICST0_FIFOUF_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFOUF_MASK) | (((uint32_t)val) << XENA_XICST0_FIFOUF_SHIFT))

#define  XENA_XICST0_RESERVED2_MASK 0x20
#define  XENA_XICST0_RESERVED2_SHIFT 5
#define  XENA_XICST0_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XICST0_RESERVED2_MASK) >> XENA_XICST0_RESERVED2_SHIFT)
#define  XENA_XICST0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_RESERVED2_MASK) | (((uint32_t)val) << XENA_XICST0_RESERVED2_SHIFT))

#define  XENA_XICST0_FIFOOF_MASK 0x10
#define  XENA_XICST0_FIFOOF_SHIFT 4
#define  XENA_XICST0_FIFOOF_MASK_SHIFT(reg) (((reg) & XENA_XICST0_FIFOOF_MASK) >> XENA_XICST0_FIFOOF_SHIFT)
#define  XENA_XICST0_FIFOOF_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_FIFOOF_MASK) | (((uint32_t)val) << XENA_XICST0_FIFOOF_SHIFT))

#define  XENA_XICST0_RESERVED3_MASK 0xf
#define  XENA_XICST0_RESERVED3_SHIFT 0
#define  XENA_XICST0_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XICST0_RESERVED3_MASK) >> XENA_XICST0_RESERVED3_SHIFT)
#define  XENA_XICST0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST0_RESERVED3_MASK) | (((uint32_t)val) << XENA_XICST0_RESERVED3_SHIFT))

//====================================================================
//Register: UPCIO Status Register (Private) (XICST)
/** \brief UPCIO ClkGen Status Register*/
//====================================================================

#define  XENA_XICST_RESERVED1_MASK 0xffffff00
#define  XENA_XICST_RESERVED1_SHIFT 8
#define  XENA_XICST_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XICST_RESERVED1_MASK) >> XENA_XICST_RESERVED1_SHIFT)
#define  XENA_XICST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST_RESERVED1_MASK) | (((uint32_t)val) << XENA_XICST_RESERVED1_SHIFT))

#define  XENA_XICST_CLKDONE_MASK 0xff
#define  XENA_XICST_CLKDONE_SHIFT 0
#define  XENA_XICST_CLKDONE_MASK_SHIFT(reg) (((reg) & XENA_XICST_CLKDONE_MASK) >> XENA_XICST_CLKDONE_SHIFT)
#define  XENA_XICST_CLKDONE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XICST_CLKDONE_MASK) | (((uint32_t)val) << XENA_XICST_CLKDONE_SHIFT))

//====================================================================
//Register: UPCIO FIFO Configuration Register (Private) (XIFC)
/** \brief UPCIO FIFO Configuration Register*/
//====================================================================

#define  XENA_XIFC_RESERVED1_MASK 0xff000000
#define  XENA_XIFC_RESERVED1_SHIFT 24
#define  XENA_XIFC_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFC_RESERVED1_MASK) >> XENA_XIFC_RESERVED1_SHIFT)
#define  XENA_XIFC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFC_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFC_RESERVED1_SHIFT))

#define  XENA_XIFC_SERIALIZERREVERSE_MASK 0xff0000
#define  XENA_XIFC_SERIALIZERREVERSE_SHIFT 16
#define  XENA_XIFC_SERIALIZERREVERSE_MASK_SHIFT(reg) (((reg) & XENA_XIFC_SERIALIZERREVERSE_MASK) >> XENA_XIFC_SERIALIZERREVERSE_SHIFT)
#define  XENA_XIFC_SERIALIZERREVERSE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFC_SERIALIZERREVERSE_MASK) | (((uint32_t)val) << XENA_XIFC_SERIALIZERREVERSE_SHIFT))

#define  XENA_XIFC_AUTOFIFOEN_MASK 0xff00
#define  XENA_XIFC_AUTOFIFOEN_SHIFT 8
#define  XENA_XIFC_AUTOFIFOEN_MASK_SHIFT(reg) (((reg) & XENA_XIFC_AUTOFIFOEN_MASK) >> XENA_XIFC_AUTOFIFOEN_SHIFT)
#define  XENA_XIFC_AUTOFIFOEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFC_AUTOFIFOEN_MASK) | (((uint32_t)val) << XENA_XIFC_AUTOFIFOEN_SHIFT))

#define  XENA_XIFC_RESERVED2_MASK 0xfc
#define  XENA_XIFC_RESERVED2_SHIFT 2
#define  XENA_XIFC_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIFC_RESERVED2_MASK) >> XENA_XIFC_RESERVED2_SHIFT)
#define  XENA_XIFC_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFC_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIFC_RESERVED2_SHIFT))

#define  XENA_XIFC_PARALLELMODE_MASK 0x3
#define  XENA_XIFC_PARALLELMODE_SHIFT 0
#define  XENA_XIFC_PARALLELMODE_MASK_SHIFT(reg) (((reg) & XENA_XIFC_PARALLELMODE_MASK) >> XENA_XIFC_PARALLELMODE_SHIFT)
#define  XENA_XIFC_PARALLELMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFC_PARALLELMODE_MASK) | (((uint32_t)val) << XENA_XIFC_PARALLELMODE_SHIFT))

//====================================================================
//Register: UPCIO FIFO Interrupt Level Register (Private) (XIFI)
/** \brief UPCIO FIFO Interrupt Level Register*/
//====================================================================

#define  XENA_XIFI_AUTOFIFOLEVEL7_MASK 0xf0000000
#define  XENA_XIFI_AUTOFIFOLEVEL7_SHIFT 28
#define  XENA_XIFI_AUTOFIFOLEVEL7_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL7_MASK) >> XENA_XIFI_AUTOFIFOLEVEL7_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL7_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL7_SHIFT))

#define  XENA_XIFI_AUTOFIFOLEVEL6_MASK 0xf000000
#define  XENA_XIFI_AUTOFIFOLEVEL6_SHIFT 24
#define  XENA_XIFI_AUTOFIFOLEVEL6_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL6_MASK) >> XENA_XIFI_AUTOFIFOLEVEL6_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL6_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL6_SHIFT))

#define  XENA_XIFI_AUTOFIFOLEVEL5_MASK 0xf00000
#define  XENA_XIFI_AUTOFIFOLEVEL5_SHIFT 20
#define  XENA_XIFI_AUTOFIFOLEVEL5_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL5_MASK) >> XENA_XIFI_AUTOFIFOLEVEL5_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL5_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL5_SHIFT))

#define  XENA_XIFI_AUTOFIFOLEVEL4_MASK 0xf0000
#define  XENA_XIFI_AUTOFIFOLEVEL4_SHIFT 16
#define  XENA_XIFI_AUTOFIFOLEVEL4_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL4_MASK) >> XENA_XIFI_AUTOFIFOLEVEL4_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL4_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL4_SHIFT))

#define  XENA_XIFI_AUTOFIFOLEVEL3_MASK 0xf000
#define  XENA_XIFI_AUTOFIFOLEVEL3_SHIFT 12
#define  XENA_XIFI_AUTOFIFOLEVEL3_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL3_MASK) >> XENA_XIFI_AUTOFIFOLEVEL3_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL3_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL3_SHIFT))

#define  XENA_XIFI_AUTOFIFOLEVEL2_MASK 0xf00
#define  XENA_XIFI_AUTOFIFOLEVEL2_SHIFT 8
#define  XENA_XIFI_AUTOFIFOLEVEL2_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL2_MASK) >> XENA_XIFI_AUTOFIFOLEVEL2_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL2_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL2_SHIFT))

#define  XENA_XIFI_AUTOFIFOLEVEL1_MASK 0xf0
#define  XENA_XIFI_AUTOFIFOLEVEL1_SHIFT 4
#define  XENA_XIFI_AUTOFIFOLEVEL1_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL1_MASK) >> XENA_XIFI_AUTOFIFOLEVEL1_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL1_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL1_SHIFT))

#define  XENA_XIFI_AUTOFIFOLEVEL0_MASK 0xf
#define  XENA_XIFI_AUTOFIFOLEVEL0_SHIFT 0
#define  XENA_XIFI_AUTOFIFOLEVEL0_MASK_SHIFT(reg) (((reg) & XENA_XIFI_AUTOFIFOLEVEL0_MASK) >> XENA_XIFI_AUTOFIFOLEVEL0_SHIFT)
#define  XENA_XIFI_AUTOFIFOLEVEL0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFI_AUTOFIFOLEVEL0_MASK) | (((uint32_t)val) << XENA_XIFI_AUTOFIFOLEVEL0_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 1 of 8) (XIFD0)
/** \brief UPCIO FIFO Configuration Register (Instance 1 of 8)*/
//====================================================================

#define  XENA_XIFD0_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD0_FIFODATA_SHIFT 0
#define  XENA_XIFD0_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD0_FIFODATA_MASK) >> XENA_XIFD0_FIFODATA_SHIFT)
#define  XENA_XIFD0_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD0_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD0_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 2 of 8) (XIFD1)
/** \brief UPCIO FIFO Configuration Register (Instance 2 of 8)*/
//====================================================================

#define  XENA_XIFD1_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD1_FIFODATA_SHIFT 0
#define  XENA_XIFD1_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD1_FIFODATA_MASK) >> XENA_XIFD1_FIFODATA_SHIFT)
#define  XENA_XIFD1_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD1_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD1_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 3 of 8) (XIFD2)
/** \brief UPCIO FIFO Configuration Register (Instance 3 of 8)*/
//====================================================================

#define  XENA_XIFD2_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD2_FIFODATA_SHIFT 0
#define  XENA_XIFD2_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD2_FIFODATA_MASK) >> XENA_XIFD2_FIFODATA_SHIFT)
#define  XENA_XIFD2_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD2_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD2_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 4 of 8) (XIFD3)
/** \brief UPCIO FIFO Configuration Register (Instance 4 of 8)*/
//====================================================================

#define  XENA_XIFD3_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD3_FIFODATA_SHIFT 0
#define  XENA_XIFD3_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD3_FIFODATA_MASK) >> XENA_XIFD3_FIFODATA_SHIFT)
#define  XENA_XIFD3_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD3_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD3_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 5 of 8) (XIFD4)
/** \brief UPCIO FIFO Configuration Register (Instance 5 of 8)*/
//====================================================================

#define  XENA_XIFD4_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD4_FIFODATA_SHIFT 0
#define  XENA_XIFD4_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD4_FIFODATA_MASK) >> XENA_XIFD4_FIFODATA_SHIFT)
#define  XENA_XIFD4_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD4_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD4_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 6 of 8) (XIFD5)
/** \brief UPCIO FIFO Configuration Register (Instance 6 of 8)*/
//====================================================================

#define  XENA_XIFD5_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD5_FIFODATA_SHIFT 0
#define  XENA_XIFD5_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD5_FIFODATA_MASK) >> XENA_XIFD5_FIFODATA_SHIFT)
#define  XENA_XIFD5_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD5_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD5_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 7 of 8) (XIFD6)
/** \brief UPCIO FIFO Configuration Register (Instance 7 of 8)*/
//====================================================================

#define  XENA_XIFD6_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD6_FIFODATA_SHIFT 0
#define  XENA_XIFD6_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD6_FIFODATA_MASK) >> XENA_XIFD6_FIFODATA_SHIFT)
#define  XENA_XIFD6_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD6_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD6_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Data Register (Private) (Instance 8 of 8) (XIFD7)
/** \brief UPCIO FIFO Configuration Register (Instance 8 of 8)*/
//====================================================================

#define  XENA_XIFD7_FIFODATA_MASK 0xffffffff
#define  XENA_XIFD7_FIFODATA_SHIFT 0
#define  XENA_XIFD7_FIFODATA_MASK_SHIFT(reg) (((reg) & XENA_XIFD7_FIFODATA_MASK) >> XENA_XIFD7_FIFODATA_SHIFT)
#define  XENA_XIFD7_FIFODATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFD7_FIFODATA_MASK) | (((uint32_t)val) << XENA_XIFD7_FIFODATA_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 1 of 8) (XIFSt0)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 1 of 8)*/
//====================================================================

#define  XENA_XIFST0_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST0_RESERVED1_SHIFT 26
#define  XENA_XIFST0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST0_RESERVED1_MASK) >> XENA_XIFST0_RESERVED1_SHIFT)
#define  XENA_XIFST0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST0_RESERVED1_SHIFT))

#define  XENA_XIFST0_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST0_STARTADDRESS_SHIFT 0
#define  XENA_XIFST0_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST0_STARTADDRESS_MASK) >> XENA_XIFST0_STARTADDRESS_SHIFT)
#define  XENA_XIFST0_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST0_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST0_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 2 of 8) (XIFSt1)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 2 of 8)*/
//====================================================================

#define  XENA_XIFST1_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST1_RESERVED1_SHIFT 26
#define  XENA_XIFST1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST1_RESERVED1_MASK) >> XENA_XIFST1_RESERVED1_SHIFT)
#define  XENA_XIFST1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST1_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST1_RESERVED1_SHIFT))

#define  XENA_XIFST1_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST1_STARTADDRESS_SHIFT 0
#define  XENA_XIFST1_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST1_STARTADDRESS_MASK) >> XENA_XIFST1_STARTADDRESS_SHIFT)
#define  XENA_XIFST1_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST1_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST1_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 3 of 8) (XIFSt2)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 3 of 8)*/
//====================================================================

#define  XENA_XIFST2_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST2_RESERVED1_SHIFT 26
#define  XENA_XIFST2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST2_RESERVED1_MASK) >> XENA_XIFST2_RESERVED1_SHIFT)
#define  XENA_XIFST2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST2_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST2_RESERVED1_SHIFT))

#define  XENA_XIFST2_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST2_STARTADDRESS_SHIFT 0
#define  XENA_XIFST2_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST2_STARTADDRESS_MASK) >> XENA_XIFST2_STARTADDRESS_SHIFT)
#define  XENA_XIFST2_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST2_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST2_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 4 of 8) (XIFSt3)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 4 of 8)*/
//====================================================================

#define  XENA_XIFST3_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST3_RESERVED1_SHIFT 26
#define  XENA_XIFST3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST3_RESERVED1_MASK) >> XENA_XIFST3_RESERVED1_SHIFT)
#define  XENA_XIFST3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST3_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST3_RESERVED1_SHIFT))

#define  XENA_XIFST3_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST3_STARTADDRESS_SHIFT 0
#define  XENA_XIFST3_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST3_STARTADDRESS_MASK) >> XENA_XIFST3_STARTADDRESS_SHIFT)
#define  XENA_XIFST3_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST3_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST3_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 5 of 8) (XIFSt4)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 5 of 8)*/
//====================================================================

#define  XENA_XIFST4_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST4_RESERVED1_SHIFT 26
#define  XENA_XIFST4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST4_RESERVED1_MASK) >> XENA_XIFST4_RESERVED1_SHIFT)
#define  XENA_XIFST4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST4_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST4_RESERVED1_SHIFT))

#define  XENA_XIFST4_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST4_STARTADDRESS_SHIFT 0
#define  XENA_XIFST4_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST4_STARTADDRESS_MASK) >> XENA_XIFST4_STARTADDRESS_SHIFT)
#define  XENA_XIFST4_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST4_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST4_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 6 of 8) (XIFSt5)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 6 of 8)*/
//====================================================================

#define  XENA_XIFST5_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST5_RESERVED1_SHIFT 26
#define  XENA_XIFST5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST5_RESERVED1_MASK) >> XENA_XIFST5_RESERVED1_SHIFT)
#define  XENA_XIFST5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST5_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST5_RESERVED1_SHIFT))

#define  XENA_XIFST5_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST5_STARTADDRESS_SHIFT 0
#define  XENA_XIFST5_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST5_STARTADDRESS_MASK) >> XENA_XIFST5_STARTADDRESS_SHIFT)
#define  XENA_XIFST5_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST5_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST5_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 7 of 8) (XIFSt6)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 7 of 8)*/
//====================================================================

#define  XENA_XIFST6_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST6_RESERVED1_SHIFT 26
#define  XENA_XIFST6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST6_RESERVED1_MASK) >> XENA_XIFST6_RESERVED1_SHIFT)
#define  XENA_XIFST6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST6_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST6_RESERVED1_SHIFT))

#define  XENA_XIFST6_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST6_STARTADDRESS_SHIFT 0
#define  XENA_XIFST6_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST6_STARTADDRESS_MASK) >> XENA_XIFST6_STARTADDRESS_SHIFT)
#define  XENA_XIFST6_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST6_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST6_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 8 of 8) (XIFSt7)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 8 of 8)*/
//====================================================================

#define  XENA_XIFST7_RESERVED1_MASK 0xfc000000
#define  XENA_XIFST7_RESERVED1_SHIFT 26
#define  XENA_XIFST7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFST7_RESERVED1_MASK) >> XENA_XIFST7_RESERVED1_SHIFT)
#define  XENA_XIFST7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST7_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFST7_RESERVED1_SHIFT))

#define  XENA_XIFST7_STARTADDRESS_MASK 0x3ffffff
#define  XENA_XIFST7_STARTADDRESS_SHIFT 0
#define  XENA_XIFST7_STARTADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFST7_STARTADDRESS_MASK) >> XENA_XIFST7_STARTADDRESS_SHIFT)
#define  XENA_XIFST7_STARTADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFST7_STARTADDRESS_MASK) | (((uint32_t)val) << XENA_XIFST7_STARTADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 1 of 8) (XIFSp0)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 1 of 8)*/
//====================================================================

#define  XENA_XIFSP0_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP0_RESERVED1_SHIFT 26
#define  XENA_XIFSP0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP0_RESERVED1_MASK) >> XENA_XIFSP0_RESERVED1_SHIFT)
#define  XENA_XIFSP0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP0_RESERVED1_SHIFT))

#define  XENA_XIFSP0_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP0_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP0_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP0_STOPADDRESS_MASK) >> XENA_XIFSP0_STOPADDRESS_SHIFT)
#define  XENA_XIFSP0_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP0_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP0_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 2 of 8) (XIFSp1)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 2 of 8)*/
//====================================================================

#define  XENA_XIFSP1_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP1_RESERVED1_SHIFT 26
#define  XENA_XIFSP1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP1_RESERVED1_MASK) >> XENA_XIFSP1_RESERVED1_SHIFT)
#define  XENA_XIFSP1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP1_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP1_RESERVED1_SHIFT))

#define  XENA_XIFSP1_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP1_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP1_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP1_STOPADDRESS_MASK) >> XENA_XIFSP1_STOPADDRESS_SHIFT)
#define  XENA_XIFSP1_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP1_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP1_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 3 of 8) (XIFSp2)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 3 of 8)*/
//====================================================================

#define  XENA_XIFSP2_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP2_RESERVED1_SHIFT 26
#define  XENA_XIFSP2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP2_RESERVED1_MASK) >> XENA_XIFSP2_RESERVED1_SHIFT)
#define  XENA_XIFSP2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP2_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP2_RESERVED1_SHIFT))

#define  XENA_XIFSP2_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP2_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP2_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP2_STOPADDRESS_MASK) >> XENA_XIFSP2_STOPADDRESS_SHIFT)
#define  XENA_XIFSP2_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP2_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP2_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 4 of 8) (XIFSp3)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 4 of 8)*/
//====================================================================

#define  XENA_XIFSP3_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP3_RESERVED1_SHIFT 26
#define  XENA_XIFSP3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP3_RESERVED1_MASK) >> XENA_XIFSP3_RESERVED1_SHIFT)
#define  XENA_XIFSP3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP3_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP3_RESERVED1_SHIFT))

#define  XENA_XIFSP3_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP3_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP3_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP3_STOPADDRESS_MASK) >> XENA_XIFSP3_STOPADDRESS_SHIFT)
#define  XENA_XIFSP3_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP3_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP3_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 5 of 8) (XIFSp4)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 5 of 8)*/
//====================================================================

#define  XENA_XIFSP4_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP4_RESERVED1_SHIFT 26
#define  XENA_XIFSP4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP4_RESERVED1_MASK) >> XENA_XIFSP4_RESERVED1_SHIFT)
#define  XENA_XIFSP4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP4_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP4_RESERVED1_SHIFT))

#define  XENA_XIFSP4_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP4_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP4_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP4_STOPADDRESS_MASK) >> XENA_XIFSP4_STOPADDRESS_SHIFT)
#define  XENA_XIFSP4_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP4_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP4_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 6 of 8) (XIFSp5)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 6 of 8)*/
//====================================================================

#define  XENA_XIFSP5_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP5_RESERVED1_SHIFT 26
#define  XENA_XIFSP5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP5_RESERVED1_MASK) >> XENA_XIFSP5_RESERVED1_SHIFT)
#define  XENA_XIFSP5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP5_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP5_RESERVED1_SHIFT))

#define  XENA_XIFSP5_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP5_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP5_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP5_STOPADDRESS_MASK) >> XENA_XIFSP5_STOPADDRESS_SHIFT)
#define  XENA_XIFSP5_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP5_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP5_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 7 of 8) (XIFSp6)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 7 of 8)*/
//====================================================================

#define  XENA_XIFSP6_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP6_RESERVED1_SHIFT 26
#define  XENA_XIFSP6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP6_RESERVED1_MASK) >> XENA_XIFSP6_RESERVED1_SHIFT)
#define  XENA_XIFSP6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP6_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP6_RESERVED1_SHIFT))

#define  XENA_XIFSP6_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP6_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP6_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP6_STOPADDRESS_MASK) >> XENA_XIFSP6_STOPADDRESS_SHIFT)
#define  XENA_XIFSP6_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP6_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP6_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 8 of 8) (XIFSp7)
/** \brief UPCIO FIFO Ring Buffer Start address Register (Instance 8 of 8)*/
//====================================================================

#define  XENA_XIFSP7_RESERVED1_MASK 0xfc000000
#define  XENA_XIFSP7_RESERVED1_SHIFT 26
#define  XENA_XIFSP7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFSP7_RESERVED1_MASK) >> XENA_XIFSP7_RESERVED1_SHIFT)
#define  XENA_XIFSP7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP7_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFSP7_RESERVED1_SHIFT))

#define  XENA_XIFSP7_STOPADDRESS_MASK 0x3ffffff
#define  XENA_XIFSP7_STOPADDRESS_SHIFT 0
#define  XENA_XIFSP7_STOPADDRESS_MASK_SHIFT(reg) (((reg) & XENA_XIFSP7_STOPADDRESS_MASK) >> XENA_XIFSP7_STOPADDRESS_SHIFT)
#define  XENA_XIFSP7_STOPADDRESS_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFSP7_STOPADDRESS_MASK) | (((uint32_t)val) << XENA_XIFSP7_STOPADDRESS_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 1 of 8) (XIFBF0)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 1 of 8)*/
//====================================================================

#define  XENA_XIFBF0_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF0_RESERVED1_SHIFT 26
#define  XENA_XIFBF0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF0_RESERVED1_MASK) >> XENA_XIFBF0_RESERVED1_SHIFT)
#define  XENA_XIFBF0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF0_RESERVED1_SHIFT))

#define  XENA_XIFBF0_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF0_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF0_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF0_FIFOCNTINC_MASK) >> XENA_XIFBF0_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF0_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF0_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF0_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 2 of 8) (XIFBF1)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 2 of 8)*/
//====================================================================

#define  XENA_XIFBF1_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF1_RESERVED1_SHIFT 26
#define  XENA_XIFBF1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF1_RESERVED1_MASK) >> XENA_XIFBF1_RESERVED1_SHIFT)
#define  XENA_XIFBF1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF1_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF1_RESERVED1_SHIFT))

#define  XENA_XIFBF1_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF1_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF1_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF1_FIFOCNTINC_MASK) >> XENA_XIFBF1_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF1_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF1_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF1_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 3 of 8) (XIFBF2)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 3 of 8)*/
//====================================================================

#define  XENA_XIFBF2_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF2_RESERVED1_SHIFT 26
#define  XENA_XIFBF2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF2_RESERVED1_MASK) >> XENA_XIFBF2_RESERVED1_SHIFT)
#define  XENA_XIFBF2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF2_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF2_RESERVED1_SHIFT))

#define  XENA_XIFBF2_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF2_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF2_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF2_FIFOCNTINC_MASK) >> XENA_XIFBF2_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF2_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF2_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF2_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 4 of 8) (XIFBF3)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 4 of 8)*/
//====================================================================

#define  XENA_XIFBF3_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF3_RESERVED1_SHIFT 26
#define  XENA_XIFBF3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF3_RESERVED1_MASK) >> XENA_XIFBF3_RESERVED1_SHIFT)
#define  XENA_XIFBF3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF3_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF3_RESERVED1_SHIFT))

#define  XENA_XIFBF3_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF3_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF3_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF3_FIFOCNTINC_MASK) >> XENA_XIFBF3_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF3_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF3_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF3_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 5 of 8) (XIFBF4)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 5 of 8)*/
//====================================================================

#define  XENA_XIFBF4_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF4_RESERVED1_SHIFT 26
#define  XENA_XIFBF4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF4_RESERVED1_MASK) >> XENA_XIFBF4_RESERVED1_SHIFT)
#define  XENA_XIFBF4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF4_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF4_RESERVED1_SHIFT))

#define  XENA_XIFBF4_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF4_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF4_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF4_FIFOCNTINC_MASK) >> XENA_XIFBF4_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF4_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF4_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF4_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 6 of 8) (XIFBF5)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 6 of 8)*/
//====================================================================

#define  XENA_XIFBF5_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF5_RESERVED1_SHIFT 26
#define  XENA_XIFBF5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF5_RESERVED1_MASK) >> XENA_XIFBF5_RESERVED1_SHIFT)
#define  XENA_XIFBF5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF5_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF5_RESERVED1_SHIFT))

#define  XENA_XIFBF5_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF5_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF5_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF5_FIFOCNTINC_MASK) >> XENA_XIFBF5_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF5_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF5_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF5_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 7 of 8) (XIFBF6)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 7 of 8)*/
//====================================================================

#define  XENA_XIFBF6_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF6_RESERVED1_SHIFT 26
#define  XENA_XIFBF6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF6_RESERVED1_MASK) >> XENA_XIFBF6_RESERVED1_SHIFT)
#define  XENA_XIFBF6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF6_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF6_RESERVED1_SHIFT))

#define  XENA_XIFBF6_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF6_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF6_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF6_FIFOCNTINC_MASK) >> XENA_XIFBF6_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF6_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF6_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF6_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 8 of 8) (XIFBF7)
/** \brief UPCIO FIFO Ring Buffer Count Increment Register (Instance 8 of 8)*/
//====================================================================

#define  XENA_XIFBF7_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBF7_RESERVED1_SHIFT 26
#define  XENA_XIFBF7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBF7_RESERVED1_MASK) >> XENA_XIFBF7_RESERVED1_SHIFT)
#define  XENA_XIFBF7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF7_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBF7_RESERVED1_SHIFT))

#define  XENA_XIFBF7_FIFOCNTINC_MASK 0x3ffffff
#define  XENA_XIFBF7_FIFOCNTINC_SHIFT 0
#define  XENA_XIFBF7_FIFOCNTINC_MASK_SHIFT(reg) (((reg) & XENA_XIFBF7_FIFOCNTINC_MASK) >> XENA_XIFBF7_FIFOCNTINC_SHIFT)
#define  XENA_XIFBF7_FIFOCNTINC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBF7_FIFOCNTINC_MASK) | (((uint32_t)val) << XENA_XIFBF7_FIFOCNTINC_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 1 of 8) (XIFBC0)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 1 of 8)*/
//====================================================================

#define  XENA_XIFBC0_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC0_RESERVED1_SHIFT 26
#define  XENA_XIFBC0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC0_RESERVED1_MASK) >> XENA_XIFBC0_RESERVED1_SHIFT)
#define  XENA_XIFBC0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC0_RESERVED1_SHIFT))

#define  XENA_XIFBC0_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC0_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC0_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC0_FIFOCURCNT_MASK) >> XENA_XIFBC0_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC0_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC0_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC0_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 2 of 8) (XIFBC1)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 2 of 8)*/
//====================================================================

#define  XENA_XIFBC1_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC1_RESERVED1_SHIFT 26
#define  XENA_XIFBC1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC1_RESERVED1_MASK) >> XENA_XIFBC1_RESERVED1_SHIFT)
#define  XENA_XIFBC1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC1_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC1_RESERVED1_SHIFT))

#define  XENA_XIFBC1_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC1_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC1_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC1_FIFOCURCNT_MASK) >> XENA_XIFBC1_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC1_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC1_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC1_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 3 of 8) (XIFBC2)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 3 of 8)*/
//====================================================================

#define  XENA_XIFBC2_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC2_RESERVED1_SHIFT 26
#define  XENA_XIFBC2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC2_RESERVED1_MASK) >> XENA_XIFBC2_RESERVED1_SHIFT)
#define  XENA_XIFBC2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC2_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC2_RESERVED1_SHIFT))

#define  XENA_XIFBC2_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC2_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC2_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC2_FIFOCURCNT_MASK) >> XENA_XIFBC2_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC2_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC2_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC2_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 4 of 8) (XIFBC3)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 4 of 8)*/
//====================================================================

#define  XENA_XIFBC3_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC3_RESERVED1_SHIFT 26
#define  XENA_XIFBC3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC3_RESERVED1_MASK) >> XENA_XIFBC3_RESERVED1_SHIFT)
#define  XENA_XIFBC3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC3_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC3_RESERVED1_SHIFT))

#define  XENA_XIFBC3_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC3_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC3_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC3_FIFOCURCNT_MASK) >> XENA_XIFBC3_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC3_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC3_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC3_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 5 of 8) (XIFBC4)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 5 of 8)*/
//====================================================================

#define  XENA_XIFBC4_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC4_RESERVED1_SHIFT 26
#define  XENA_XIFBC4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC4_RESERVED1_MASK) >> XENA_XIFBC4_RESERVED1_SHIFT)
#define  XENA_XIFBC4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC4_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC4_RESERVED1_SHIFT))

#define  XENA_XIFBC4_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC4_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC4_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC4_FIFOCURCNT_MASK) >> XENA_XIFBC4_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC4_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC4_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC4_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 6 of 8) (XIFBC5)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 6 of 8)*/
//====================================================================

#define  XENA_XIFBC5_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC5_RESERVED1_SHIFT 26
#define  XENA_XIFBC5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC5_RESERVED1_MASK) >> XENA_XIFBC5_RESERVED1_SHIFT)
#define  XENA_XIFBC5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC5_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC5_RESERVED1_SHIFT))

#define  XENA_XIFBC5_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC5_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC5_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC5_FIFOCURCNT_MASK) >> XENA_XIFBC5_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC5_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC5_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC5_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 7 of 8) (XIFBC6)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 7 of 8)*/
//====================================================================

#define  XENA_XIFBC6_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC6_RESERVED1_SHIFT 26
#define  XENA_XIFBC6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC6_RESERVED1_MASK) >> XENA_XIFBC6_RESERVED1_SHIFT)
#define  XENA_XIFBC6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC6_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC6_RESERVED1_SHIFT))

#define  XENA_XIFBC6_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC6_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC6_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC6_FIFOCURCNT_MASK) >> XENA_XIFBC6_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC6_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC6_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC6_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 8 of 8) (XIFBC7)
/** \brief UPCIO FIFO Ring Buffer Current Count Register (Instance 8 of 8)*/
//====================================================================

#define  XENA_XIFBC7_RESERVED1_MASK 0xfc000000
#define  XENA_XIFBC7_RESERVED1_SHIFT 26
#define  XENA_XIFBC7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIFBC7_RESERVED1_MASK) >> XENA_XIFBC7_RESERVED1_SHIFT)
#define  XENA_XIFBC7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC7_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIFBC7_RESERVED1_SHIFT))

#define  XENA_XIFBC7_FIFOCURCNT_MASK 0x3ffffff
#define  XENA_XIFBC7_FIFOCURCNT_SHIFT 0
#define  XENA_XIFBC7_FIFOCURCNT_MASK_SHIFT(reg) (((reg) & XENA_XIFBC7_FIFOCURCNT_MASK) >> XENA_XIFBC7_FIFOCURCNT_SHIFT)
#define  XENA_XIFBC7_FIFOCURCNT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIFBC7_FIFOCURCNT_MASK) | (((uint32_t)val) << XENA_XIFBC7_FIFOCURCNT_SHIFT))

//====================================================================
//Register: UPCIO Serializer Interleave Configuration Register (Private) (XISI)
/** \brief The UPCIO Serializer Interleave Configuration Register is used to set up the Interleave modes for the 8 Serializers in the
UPCIO block.*/
//====================================================================

#define  XENA_XISI_SERCLKSEL7_MASK 0xe0000000
#define  XENA_XISI_SERCLKSEL7_SHIFT 29
#define  XENA_XISI_SERCLKSEL7_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL7_MASK) >> XENA_XISI_SERCLKSEL7_SHIFT)
#define  XENA_XISI_SERCLKSEL7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL7_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL7_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE7_MASK 0x10000000
#define  XENA_XISI_INTERLEAVEMODE7_SHIFT 28
#define  XENA_XISI_INTERLEAVEMODE7_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE7_MASK) >> XENA_XISI_INTERLEAVEMODE7_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE7_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE7_SHIFT))

#define  XENA_XISI_SERCLKSEL6_MASK 0xe000000
#define  XENA_XISI_SERCLKSEL6_SHIFT 25
#define  XENA_XISI_SERCLKSEL6_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL6_MASK) >> XENA_XISI_SERCLKSEL6_SHIFT)
#define  XENA_XISI_SERCLKSEL6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL6_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL6_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE6_MASK 0x1000000
#define  XENA_XISI_INTERLEAVEMODE6_SHIFT 24
#define  XENA_XISI_INTERLEAVEMODE6_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE6_MASK) >> XENA_XISI_INTERLEAVEMODE6_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE6_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE6_SHIFT))

#define  XENA_XISI_SERCLKSEL5_MASK 0xe00000
#define  XENA_XISI_SERCLKSEL5_SHIFT 21
#define  XENA_XISI_SERCLKSEL5_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL5_MASK) >> XENA_XISI_SERCLKSEL5_SHIFT)
#define  XENA_XISI_SERCLKSEL5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL5_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL5_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE5_MASK 0x100000
#define  XENA_XISI_INTERLEAVEMODE5_SHIFT 20
#define  XENA_XISI_INTERLEAVEMODE5_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE5_MASK) >> XENA_XISI_INTERLEAVEMODE5_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE5_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE5_SHIFT))

#define  XENA_XISI_SERCLKSEL4_MASK 0xe0000
#define  XENA_XISI_SERCLKSEL4_SHIFT 17
#define  XENA_XISI_SERCLKSEL4_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL4_MASK) >> XENA_XISI_SERCLKSEL4_SHIFT)
#define  XENA_XISI_SERCLKSEL4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL4_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL4_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE4_MASK 0x10000
#define  XENA_XISI_INTERLEAVEMODE4_SHIFT 16
#define  XENA_XISI_INTERLEAVEMODE4_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE4_MASK) >> XENA_XISI_INTERLEAVEMODE4_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE4_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE4_SHIFT))

#define  XENA_XISI_SERCLKSEL3_MASK 0xe000
#define  XENA_XISI_SERCLKSEL3_SHIFT 13
#define  XENA_XISI_SERCLKSEL3_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL3_MASK) >> XENA_XISI_SERCLKSEL3_SHIFT)
#define  XENA_XISI_SERCLKSEL3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL3_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL3_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE3_MASK 0x1000
#define  XENA_XISI_INTERLEAVEMODE3_SHIFT 12
#define  XENA_XISI_INTERLEAVEMODE3_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE3_MASK) >> XENA_XISI_INTERLEAVEMODE3_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE3_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE3_SHIFT))

#define  XENA_XISI_SERCLKSEL2_MASK 0xe00
#define  XENA_XISI_SERCLKSEL2_SHIFT 9
#define  XENA_XISI_SERCLKSEL2_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL2_MASK) >> XENA_XISI_SERCLKSEL2_SHIFT)
#define  XENA_XISI_SERCLKSEL2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL2_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL2_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE2_MASK 0x100
#define  XENA_XISI_INTERLEAVEMODE2_SHIFT 8
#define  XENA_XISI_INTERLEAVEMODE2_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE2_MASK) >> XENA_XISI_INTERLEAVEMODE2_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE2_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE2_SHIFT))

#define  XENA_XISI_SERCLKSEL1_MASK 0xe0
#define  XENA_XISI_SERCLKSEL1_SHIFT 5
#define  XENA_XISI_SERCLKSEL1_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL1_MASK) >> XENA_XISI_SERCLKSEL1_SHIFT)
#define  XENA_XISI_SERCLKSEL1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL1_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL1_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE1_MASK 0x10
#define  XENA_XISI_INTERLEAVEMODE1_SHIFT 4
#define  XENA_XISI_INTERLEAVEMODE1_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE1_MASK) >> XENA_XISI_INTERLEAVEMODE1_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE1_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE1_SHIFT))

#define  XENA_XISI_SERCLKSEL0_MASK 0xe
#define  XENA_XISI_SERCLKSEL0_SHIFT 1
#define  XENA_XISI_SERCLKSEL0_MASK_SHIFT(reg) (((reg) & XENA_XISI_SERCLKSEL0_MASK) >> XENA_XISI_SERCLKSEL0_SHIFT)
#define  XENA_XISI_SERCLKSEL0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_SERCLKSEL0_MASK) | (((uint32_t)val) << XENA_XISI_SERCLKSEL0_SHIFT))

#define  XENA_XISI_INTERLEAVEMODE0_MASK 0x1
#define  XENA_XISI_INTERLEAVEMODE0_SHIFT 0
#define  XENA_XISI_INTERLEAVEMODE0_MASK_SHIFT(reg) (((reg) & XENA_XISI_INTERLEAVEMODE0_MASK) >> XENA_XISI_INTERLEAVEMODE0_SHIFT)
#define  XENA_XISI_INTERLEAVEMODE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISI_INTERLEAVEMODE0_MASK) | (((uint32_t)val) << XENA_XISI_INTERLEAVEMODE0_SHIFT))

//====================================================================
//Register: UPCIO Serializer Configuration Register (Private) (XISC)
/** \brief UPCIO Serializer Configuration Register*/
//====================================================================

#define  XENA_XISC_SERCHAN7_MASK 0xf0000000
#define  XENA_XISC_SERCHAN7_SHIFT 28
#define  XENA_XISC_SERCHAN7_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN7_MASK) >> XENA_XISC_SERCHAN7_SHIFT)
#define  XENA_XISC_SERCHAN7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN7_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN7_SHIFT))

#define  XENA_XISC_SERCHAN6_MASK 0xf000000
#define  XENA_XISC_SERCHAN6_SHIFT 24
#define  XENA_XISC_SERCHAN6_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN6_MASK) >> XENA_XISC_SERCHAN6_SHIFT)
#define  XENA_XISC_SERCHAN6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN6_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN6_SHIFT))

#define  XENA_XISC_SERCHAN5_MASK 0xf00000
#define  XENA_XISC_SERCHAN5_SHIFT 20
#define  XENA_XISC_SERCHAN5_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN5_MASK) >> XENA_XISC_SERCHAN5_SHIFT)
#define  XENA_XISC_SERCHAN5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN5_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN5_SHIFT))

#define  XENA_XISC_SERCHAN4_MASK 0xf0000
#define  XENA_XISC_SERCHAN4_SHIFT 16
#define  XENA_XISC_SERCHAN4_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN4_MASK) >> XENA_XISC_SERCHAN4_SHIFT)
#define  XENA_XISC_SERCHAN4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN4_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN4_SHIFT))

#define  XENA_XISC_SERCHAN3_MASK 0xf000
#define  XENA_XISC_SERCHAN3_SHIFT 12
#define  XENA_XISC_SERCHAN3_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN3_MASK) >> XENA_XISC_SERCHAN3_SHIFT)
#define  XENA_XISC_SERCHAN3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN3_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN3_SHIFT))

#define  XENA_XISC_SERCHAN2_MASK 0xf00
#define  XENA_XISC_SERCHAN2_SHIFT 8
#define  XENA_XISC_SERCHAN2_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN2_MASK) >> XENA_XISC_SERCHAN2_SHIFT)
#define  XENA_XISC_SERCHAN2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN2_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN2_SHIFT))

#define  XENA_XISC_SERCHAN1_MASK 0xf0
#define  XENA_XISC_SERCHAN1_SHIFT 4
#define  XENA_XISC_SERCHAN1_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN1_MASK) >> XENA_XISC_SERCHAN1_SHIFT)
#define  XENA_XISC_SERCHAN1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN1_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN1_SHIFT))

#define  XENA_XISC_SERCHAN0_MASK 0xf
#define  XENA_XISC_SERCHAN0_SHIFT 0
#define  XENA_XISC_SERCHAN0_MASK_SHIFT(reg) (((reg) & XENA_XISC_SERCHAN0_MASK) >> XENA_XISC_SERCHAN0_SHIFT)
#define  XENA_XISC_SERCHAN0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISC_SERCHAN0_MASK) | (((uint32_t)val) << XENA_XISC_SERCHAN0_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 1 of 52) (XIIC0)
/** \brief UPCIO IO Configuration Register (Instance 1 of 52)*/
//====================================================================

#define  XENA_XIIC0_INVERT_MASK 0x80000000
#define  XENA_XIIC0_INVERT_SHIFT 31
#define  XENA_XIIC0_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_INVERT_MASK) >> XENA_XIIC0_INVERT_SHIFT)
#define  XENA_XIIC0_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC0_INVERT_SHIFT))

#define  XENA_XIIC0_ENABLE_MASK 0x40000000
#define  XENA_XIIC0_ENABLE_SHIFT 30
#define  XENA_XIIC0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_ENABLE_MASK) >> XENA_XIIC0_ENABLE_SHIFT)
#define  XENA_XIIC0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC0_ENABLE_SHIFT))

#define  XENA_XIIC0_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC0_SAFETYEN_SHIFT 29
#define  XENA_XIIC0_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_SAFETYEN_MASK) >> XENA_XIIC0_SAFETYEN_SHIFT)
#define  XENA_XIIC0_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC0_SAFETYEN_SHIFT))

#define  XENA_XIIC0_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC0_RESERVED1_SHIFT 24
#define  XENA_XIIC0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_RESERVED1_MASK) >> XENA_XIIC0_RESERVED1_SHIFT)
#define  XENA_XIIC0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC0_RESERVED1_SHIFT))

#define  XENA_XIIC0_DELAY_MASK 0xff0000
#define  XENA_XIIC0_DELAY_SHIFT 16
#define  XENA_XIIC0_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_DELAY_MASK) >> XENA_XIIC0_DELAY_SHIFT)
#define  XENA_XIIC0_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC0_DELAY_SHIFT))

#define  XENA_XIIC0_RESERVED2_MASK 0xfe00
#define  XENA_XIIC0_RESERVED2_SHIFT 9
#define  XENA_XIIC0_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_RESERVED2_MASK) >> XENA_XIIC0_RESERVED2_SHIFT)
#define  XENA_XIIC0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC0_RESERVED2_SHIFT))

#define  XENA_XIIC0_BITSELECT_MASK 0x1f0
#define  XENA_XIIC0_BITSELECT_SHIFT 4
#define  XENA_XIIC0_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_BITSELECT_MASK) >> XENA_XIIC0_BITSELECT_SHIFT)
#define  XENA_XIIC0_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC0_BITSELECT_SHIFT))

#define  XENA_XIIC0_SELECT_MASK 0xf
#define  XENA_XIIC0_SELECT_SHIFT 0
#define  XENA_XIIC0_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC0_SELECT_MASK) >> XENA_XIIC0_SELECT_SHIFT)
#define  XENA_XIIC0_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC0_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC0_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 2 of 52) (XIIC1)
/** \brief UPCIO IO Configuration Register (Instance 2 of 52)*/
//====================================================================

#define  XENA_XIIC1_INVERT_MASK 0x80000000
#define  XENA_XIIC1_INVERT_SHIFT 31
#define  XENA_XIIC1_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_INVERT_MASK) >> XENA_XIIC1_INVERT_SHIFT)
#define  XENA_XIIC1_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC1_INVERT_SHIFT))

#define  XENA_XIIC1_ENABLE_MASK 0x40000000
#define  XENA_XIIC1_ENABLE_SHIFT 30
#define  XENA_XIIC1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_ENABLE_MASK) >> XENA_XIIC1_ENABLE_SHIFT)
#define  XENA_XIIC1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC1_ENABLE_SHIFT))

#define  XENA_XIIC1_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC1_SAFETYEN_SHIFT 29
#define  XENA_XIIC1_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_SAFETYEN_MASK) >> XENA_XIIC1_SAFETYEN_SHIFT)
#define  XENA_XIIC1_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC1_SAFETYEN_SHIFT))

#define  XENA_XIIC1_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC1_RESERVED1_SHIFT 24
#define  XENA_XIIC1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_RESERVED1_MASK) >> XENA_XIIC1_RESERVED1_SHIFT)
#define  XENA_XIIC1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC1_RESERVED1_SHIFT))

#define  XENA_XIIC1_DELAY_MASK 0xff0000
#define  XENA_XIIC1_DELAY_SHIFT 16
#define  XENA_XIIC1_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_DELAY_MASK) >> XENA_XIIC1_DELAY_SHIFT)
#define  XENA_XIIC1_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC1_DELAY_SHIFT))

#define  XENA_XIIC1_RESERVED2_MASK 0xfe00
#define  XENA_XIIC1_RESERVED2_SHIFT 9
#define  XENA_XIIC1_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_RESERVED2_MASK) >> XENA_XIIC1_RESERVED2_SHIFT)
#define  XENA_XIIC1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC1_RESERVED2_SHIFT))

#define  XENA_XIIC1_BITSELECT_MASK 0x1f0
#define  XENA_XIIC1_BITSELECT_SHIFT 4
#define  XENA_XIIC1_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_BITSELECT_MASK) >> XENA_XIIC1_BITSELECT_SHIFT)
#define  XENA_XIIC1_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC1_BITSELECT_SHIFT))

#define  XENA_XIIC1_SELECT_MASK 0xf
#define  XENA_XIIC1_SELECT_SHIFT 0
#define  XENA_XIIC1_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC1_SELECT_MASK) >> XENA_XIIC1_SELECT_SHIFT)
#define  XENA_XIIC1_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC1_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC1_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 3 of 52) (XIIC2)
/** \brief UPCIO IO Configuration Register (Instance 3 of 52)*/
//====================================================================

#define  XENA_XIIC2_INVERT_MASK 0x80000000
#define  XENA_XIIC2_INVERT_SHIFT 31
#define  XENA_XIIC2_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_INVERT_MASK) >> XENA_XIIC2_INVERT_SHIFT)
#define  XENA_XIIC2_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC2_INVERT_SHIFT))

#define  XENA_XIIC2_ENABLE_MASK 0x40000000
#define  XENA_XIIC2_ENABLE_SHIFT 30
#define  XENA_XIIC2_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_ENABLE_MASK) >> XENA_XIIC2_ENABLE_SHIFT)
#define  XENA_XIIC2_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC2_ENABLE_SHIFT))

#define  XENA_XIIC2_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC2_SAFETYEN_SHIFT 29
#define  XENA_XIIC2_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_SAFETYEN_MASK) >> XENA_XIIC2_SAFETYEN_SHIFT)
#define  XENA_XIIC2_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC2_SAFETYEN_SHIFT))

#define  XENA_XIIC2_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC2_RESERVED1_SHIFT 24
#define  XENA_XIIC2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_RESERVED1_MASK) >> XENA_XIIC2_RESERVED1_SHIFT)
#define  XENA_XIIC2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC2_RESERVED1_SHIFT))

#define  XENA_XIIC2_DELAY_MASK 0xff0000
#define  XENA_XIIC2_DELAY_SHIFT 16
#define  XENA_XIIC2_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_DELAY_MASK) >> XENA_XIIC2_DELAY_SHIFT)
#define  XENA_XIIC2_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC2_DELAY_SHIFT))

#define  XENA_XIIC2_RESERVED2_MASK 0xfe00
#define  XENA_XIIC2_RESERVED2_SHIFT 9
#define  XENA_XIIC2_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_RESERVED2_MASK) >> XENA_XIIC2_RESERVED2_SHIFT)
#define  XENA_XIIC2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC2_RESERVED2_SHIFT))

#define  XENA_XIIC2_BITSELECT_MASK 0x1f0
#define  XENA_XIIC2_BITSELECT_SHIFT 4
#define  XENA_XIIC2_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_BITSELECT_MASK) >> XENA_XIIC2_BITSELECT_SHIFT)
#define  XENA_XIIC2_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC2_BITSELECT_SHIFT))

#define  XENA_XIIC2_SELECT_MASK 0xf
#define  XENA_XIIC2_SELECT_SHIFT 0
#define  XENA_XIIC2_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC2_SELECT_MASK) >> XENA_XIIC2_SELECT_SHIFT)
#define  XENA_XIIC2_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC2_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC2_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 4 of 52) (XIIC3)
/** \brief UPCIO IO Configuration Register (Instance 4 of 52)*/
//====================================================================

#define  XENA_XIIC3_INVERT_MASK 0x80000000
#define  XENA_XIIC3_INVERT_SHIFT 31
#define  XENA_XIIC3_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_INVERT_MASK) >> XENA_XIIC3_INVERT_SHIFT)
#define  XENA_XIIC3_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC3_INVERT_SHIFT))

#define  XENA_XIIC3_ENABLE_MASK 0x40000000
#define  XENA_XIIC3_ENABLE_SHIFT 30
#define  XENA_XIIC3_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_ENABLE_MASK) >> XENA_XIIC3_ENABLE_SHIFT)
#define  XENA_XIIC3_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC3_ENABLE_SHIFT))

#define  XENA_XIIC3_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC3_SAFETYEN_SHIFT 29
#define  XENA_XIIC3_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_SAFETYEN_MASK) >> XENA_XIIC3_SAFETYEN_SHIFT)
#define  XENA_XIIC3_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC3_SAFETYEN_SHIFT))

#define  XENA_XIIC3_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC3_RESERVED1_SHIFT 24
#define  XENA_XIIC3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_RESERVED1_MASK) >> XENA_XIIC3_RESERVED1_SHIFT)
#define  XENA_XIIC3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC3_RESERVED1_SHIFT))

#define  XENA_XIIC3_DELAY_MASK 0xff0000
#define  XENA_XIIC3_DELAY_SHIFT 16
#define  XENA_XIIC3_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_DELAY_MASK) >> XENA_XIIC3_DELAY_SHIFT)
#define  XENA_XIIC3_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC3_DELAY_SHIFT))

#define  XENA_XIIC3_RESERVED2_MASK 0xfe00
#define  XENA_XIIC3_RESERVED2_SHIFT 9
#define  XENA_XIIC3_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_RESERVED2_MASK) >> XENA_XIIC3_RESERVED2_SHIFT)
#define  XENA_XIIC3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC3_RESERVED2_SHIFT))

#define  XENA_XIIC3_BITSELECT_MASK 0x1f0
#define  XENA_XIIC3_BITSELECT_SHIFT 4
#define  XENA_XIIC3_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_BITSELECT_MASK) >> XENA_XIIC3_BITSELECT_SHIFT)
#define  XENA_XIIC3_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC3_BITSELECT_SHIFT))

#define  XENA_XIIC3_SELECT_MASK 0xf
#define  XENA_XIIC3_SELECT_SHIFT 0
#define  XENA_XIIC3_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC3_SELECT_MASK) >> XENA_XIIC3_SELECT_SHIFT)
#define  XENA_XIIC3_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC3_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC3_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 5 of 52) (XIIC4)
/** \brief UPCIO IO Configuration Register (Instance 5 of 52)*/
//====================================================================

#define  XENA_XIIC4_INVERT_MASK 0x80000000
#define  XENA_XIIC4_INVERT_SHIFT 31
#define  XENA_XIIC4_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_INVERT_MASK) >> XENA_XIIC4_INVERT_SHIFT)
#define  XENA_XIIC4_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC4_INVERT_SHIFT))

#define  XENA_XIIC4_ENABLE_MASK 0x40000000
#define  XENA_XIIC4_ENABLE_SHIFT 30
#define  XENA_XIIC4_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_ENABLE_MASK) >> XENA_XIIC4_ENABLE_SHIFT)
#define  XENA_XIIC4_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC4_ENABLE_SHIFT))

#define  XENA_XIIC4_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC4_SAFETYEN_SHIFT 29
#define  XENA_XIIC4_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_SAFETYEN_MASK) >> XENA_XIIC4_SAFETYEN_SHIFT)
#define  XENA_XIIC4_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC4_SAFETYEN_SHIFT))

#define  XENA_XIIC4_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC4_RESERVED1_SHIFT 24
#define  XENA_XIIC4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_RESERVED1_MASK) >> XENA_XIIC4_RESERVED1_SHIFT)
#define  XENA_XIIC4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC4_RESERVED1_SHIFT))

#define  XENA_XIIC4_DELAY_MASK 0xff0000
#define  XENA_XIIC4_DELAY_SHIFT 16
#define  XENA_XIIC4_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_DELAY_MASK) >> XENA_XIIC4_DELAY_SHIFT)
#define  XENA_XIIC4_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC4_DELAY_SHIFT))

#define  XENA_XIIC4_RESERVED2_MASK 0xfe00
#define  XENA_XIIC4_RESERVED2_SHIFT 9
#define  XENA_XIIC4_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_RESERVED2_MASK) >> XENA_XIIC4_RESERVED2_SHIFT)
#define  XENA_XIIC4_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC4_RESERVED2_SHIFT))

#define  XENA_XIIC4_BITSELECT_MASK 0x1f0
#define  XENA_XIIC4_BITSELECT_SHIFT 4
#define  XENA_XIIC4_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_BITSELECT_MASK) >> XENA_XIIC4_BITSELECT_SHIFT)
#define  XENA_XIIC4_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC4_BITSELECT_SHIFT))

#define  XENA_XIIC4_SELECT_MASK 0xf
#define  XENA_XIIC4_SELECT_SHIFT 0
#define  XENA_XIIC4_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC4_SELECT_MASK) >> XENA_XIIC4_SELECT_SHIFT)
#define  XENA_XIIC4_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC4_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC4_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 6 of 52) (XIIC5)
/** \brief UPCIO IO Configuration Register (Instance 6 of 52)*/
//====================================================================

#define  XENA_XIIC5_INVERT_MASK 0x80000000
#define  XENA_XIIC5_INVERT_SHIFT 31
#define  XENA_XIIC5_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_INVERT_MASK) >> XENA_XIIC5_INVERT_SHIFT)
#define  XENA_XIIC5_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC5_INVERT_SHIFT))

#define  XENA_XIIC5_ENABLE_MASK 0x40000000
#define  XENA_XIIC5_ENABLE_SHIFT 30
#define  XENA_XIIC5_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_ENABLE_MASK) >> XENA_XIIC5_ENABLE_SHIFT)
#define  XENA_XIIC5_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC5_ENABLE_SHIFT))

#define  XENA_XIIC5_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC5_SAFETYEN_SHIFT 29
#define  XENA_XIIC5_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_SAFETYEN_MASK) >> XENA_XIIC5_SAFETYEN_SHIFT)
#define  XENA_XIIC5_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC5_SAFETYEN_SHIFT))

#define  XENA_XIIC5_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC5_RESERVED1_SHIFT 24
#define  XENA_XIIC5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_RESERVED1_MASK) >> XENA_XIIC5_RESERVED1_SHIFT)
#define  XENA_XIIC5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC5_RESERVED1_SHIFT))

#define  XENA_XIIC5_DELAY_MASK 0xff0000
#define  XENA_XIIC5_DELAY_SHIFT 16
#define  XENA_XIIC5_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_DELAY_MASK) >> XENA_XIIC5_DELAY_SHIFT)
#define  XENA_XIIC5_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC5_DELAY_SHIFT))

#define  XENA_XIIC5_RESERVED2_MASK 0xfe00
#define  XENA_XIIC5_RESERVED2_SHIFT 9
#define  XENA_XIIC5_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_RESERVED2_MASK) >> XENA_XIIC5_RESERVED2_SHIFT)
#define  XENA_XIIC5_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC5_RESERVED2_SHIFT))

#define  XENA_XIIC5_BITSELECT_MASK 0x1f0
#define  XENA_XIIC5_BITSELECT_SHIFT 4
#define  XENA_XIIC5_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_BITSELECT_MASK) >> XENA_XIIC5_BITSELECT_SHIFT)
#define  XENA_XIIC5_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC5_BITSELECT_SHIFT))

#define  XENA_XIIC5_SELECT_MASK 0xf
#define  XENA_XIIC5_SELECT_SHIFT 0
#define  XENA_XIIC5_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC5_SELECT_MASK) >> XENA_XIIC5_SELECT_SHIFT)
#define  XENA_XIIC5_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC5_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC5_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 7 of 52) (XIIC6)
/** \brief UPCIO IO Configuration Register (Instance 7 of 52)*/
//====================================================================

#define  XENA_XIIC6_INVERT_MASK 0x80000000
#define  XENA_XIIC6_INVERT_SHIFT 31
#define  XENA_XIIC6_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_INVERT_MASK) >> XENA_XIIC6_INVERT_SHIFT)
#define  XENA_XIIC6_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC6_INVERT_SHIFT))

#define  XENA_XIIC6_ENABLE_MASK 0x40000000
#define  XENA_XIIC6_ENABLE_SHIFT 30
#define  XENA_XIIC6_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_ENABLE_MASK) >> XENA_XIIC6_ENABLE_SHIFT)
#define  XENA_XIIC6_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC6_ENABLE_SHIFT))

#define  XENA_XIIC6_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC6_SAFETYEN_SHIFT 29
#define  XENA_XIIC6_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_SAFETYEN_MASK) >> XENA_XIIC6_SAFETYEN_SHIFT)
#define  XENA_XIIC6_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC6_SAFETYEN_SHIFT))

#define  XENA_XIIC6_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC6_RESERVED1_SHIFT 24
#define  XENA_XIIC6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_RESERVED1_MASK) >> XENA_XIIC6_RESERVED1_SHIFT)
#define  XENA_XIIC6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC6_RESERVED1_SHIFT))

#define  XENA_XIIC6_DELAY_MASK 0xff0000
#define  XENA_XIIC6_DELAY_SHIFT 16
#define  XENA_XIIC6_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_DELAY_MASK) >> XENA_XIIC6_DELAY_SHIFT)
#define  XENA_XIIC6_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC6_DELAY_SHIFT))

#define  XENA_XIIC6_RESERVED2_MASK 0xfe00
#define  XENA_XIIC6_RESERVED2_SHIFT 9
#define  XENA_XIIC6_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_RESERVED2_MASK) >> XENA_XIIC6_RESERVED2_SHIFT)
#define  XENA_XIIC6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC6_RESERVED2_SHIFT))

#define  XENA_XIIC6_BITSELECT_MASK 0x1f0
#define  XENA_XIIC6_BITSELECT_SHIFT 4
#define  XENA_XIIC6_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_BITSELECT_MASK) >> XENA_XIIC6_BITSELECT_SHIFT)
#define  XENA_XIIC6_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC6_BITSELECT_SHIFT))

#define  XENA_XIIC6_SELECT_MASK 0xf
#define  XENA_XIIC6_SELECT_SHIFT 0
#define  XENA_XIIC6_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC6_SELECT_MASK) >> XENA_XIIC6_SELECT_SHIFT)
#define  XENA_XIIC6_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC6_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC6_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 8 of 52) (XIIC7)
/** \brief UPCIO IO Configuration Register (Instance 8 of 52)*/
//====================================================================

#define  XENA_XIIC7_INVERT_MASK 0x80000000
#define  XENA_XIIC7_INVERT_SHIFT 31
#define  XENA_XIIC7_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_INVERT_MASK) >> XENA_XIIC7_INVERT_SHIFT)
#define  XENA_XIIC7_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC7_INVERT_SHIFT))

#define  XENA_XIIC7_ENABLE_MASK 0x40000000
#define  XENA_XIIC7_ENABLE_SHIFT 30
#define  XENA_XIIC7_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_ENABLE_MASK) >> XENA_XIIC7_ENABLE_SHIFT)
#define  XENA_XIIC7_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC7_ENABLE_SHIFT))

#define  XENA_XIIC7_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC7_SAFETYEN_SHIFT 29
#define  XENA_XIIC7_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_SAFETYEN_MASK) >> XENA_XIIC7_SAFETYEN_SHIFT)
#define  XENA_XIIC7_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC7_SAFETYEN_SHIFT))

#define  XENA_XIIC7_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC7_RESERVED1_SHIFT 24
#define  XENA_XIIC7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_RESERVED1_MASK) >> XENA_XIIC7_RESERVED1_SHIFT)
#define  XENA_XIIC7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC7_RESERVED1_SHIFT))

#define  XENA_XIIC7_DELAY_MASK 0xff0000
#define  XENA_XIIC7_DELAY_SHIFT 16
#define  XENA_XIIC7_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_DELAY_MASK) >> XENA_XIIC7_DELAY_SHIFT)
#define  XENA_XIIC7_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC7_DELAY_SHIFT))

#define  XENA_XIIC7_RESERVED2_MASK 0xfe00
#define  XENA_XIIC7_RESERVED2_SHIFT 9
#define  XENA_XIIC7_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_RESERVED2_MASK) >> XENA_XIIC7_RESERVED2_SHIFT)
#define  XENA_XIIC7_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC7_RESERVED2_SHIFT))

#define  XENA_XIIC7_BITSELECT_MASK 0x1f0
#define  XENA_XIIC7_BITSELECT_SHIFT 4
#define  XENA_XIIC7_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_BITSELECT_MASK) >> XENA_XIIC7_BITSELECT_SHIFT)
#define  XENA_XIIC7_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC7_BITSELECT_SHIFT))

#define  XENA_XIIC7_SELECT_MASK 0xf
#define  XENA_XIIC7_SELECT_SHIFT 0
#define  XENA_XIIC7_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC7_SELECT_MASK) >> XENA_XIIC7_SELECT_SHIFT)
#define  XENA_XIIC7_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC7_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC7_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 9 of 52) (XIIC8)
/** \brief UPCIO IO Configuration Register (Instance 9 of 52)*/
//====================================================================

#define  XENA_XIIC8_INVERT_MASK 0x80000000
#define  XENA_XIIC8_INVERT_SHIFT 31
#define  XENA_XIIC8_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_INVERT_MASK) >> XENA_XIIC8_INVERT_SHIFT)
#define  XENA_XIIC8_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC8_INVERT_SHIFT))

#define  XENA_XIIC8_ENABLE_MASK 0x40000000
#define  XENA_XIIC8_ENABLE_SHIFT 30
#define  XENA_XIIC8_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_ENABLE_MASK) >> XENA_XIIC8_ENABLE_SHIFT)
#define  XENA_XIIC8_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC8_ENABLE_SHIFT))

#define  XENA_XIIC8_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC8_SAFETYEN_SHIFT 29
#define  XENA_XIIC8_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_SAFETYEN_MASK) >> XENA_XIIC8_SAFETYEN_SHIFT)
#define  XENA_XIIC8_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC8_SAFETYEN_SHIFT))

#define  XENA_XIIC8_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC8_RESERVED1_SHIFT 24
#define  XENA_XIIC8_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_RESERVED1_MASK) >> XENA_XIIC8_RESERVED1_SHIFT)
#define  XENA_XIIC8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC8_RESERVED1_SHIFT))

#define  XENA_XIIC8_DELAY_MASK 0xff0000
#define  XENA_XIIC8_DELAY_SHIFT 16
#define  XENA_XIIC8_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_DELAY_MASK) >> XENA_XIIC8_DELAY_SHIFT)
#define  XENA_XIIC8_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC8_DELAY_SHIFT))

#define  XENA_XIIC8_RESERVED2_MASK 0xfe00
#define  XENA_XIIC8_RESERVED2_SHIFT 9
#define  XENA_XIIC8_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_RESERVED2_MASK) >> XENA_XIIC8_RESERVED2_SHIFT)
#define  XENA_XIIC8_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC8_RESERVED2_SHIFT))

#define  XENA_XIIC8_BITSELECT_MASK 0x1f0
#define  XENA_XIIC8_BITSELECT_SHIFT 4
#define  XENA_XIIC8_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_BITSELECT_MASK) >> XENA_XIIC8_BITSELECT_SHIFT)
#define  XENA_XIIC8_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC8_BITSELECT_SHIFT))

#define  XENA_XIIC8_SELECT_MASK 0xf
#define  XENA_XIIC8_SELECT_SHIFT 0
#define  XENA_XIIC8_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC8_SELECT_MASK) >> XENA_XIIC8_SELECT_SHIFT)
#define  XENA_XIIC8_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC8_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC8_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 10 of 52) (XIIC9)
/** \brief UPCIO IO Configuration Register (Instance 10 of 52)*/
//====================================================================

#define  XENA_XIIC9_INVERT_MASK 0x80000000
#define  XENA_XIIC9_INVERT_SHIFT 31
#define  XENA_XIIC9_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_INVERT_MASK) >> XENA_XIIC9_INVERT_SHIFT)
#define  XENA_XIIC9_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC9_INVERT_SHIFT))

#define  XENA_XIIC9_ENABLE_MASK 0x40000000
#define  XENA_XIIC9_ENABLE_SHIFT 30
#define  XENA_XIIC9_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_ENABLE_MASK) >> XENA_XIIC9_ENABLE_SHIFT)
#define  XENA_XIIC9_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC9_ENABLE_SHIFT))

#define  XENA_XIIC9_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC9_SAFETYEN_SHIFT 29
#define  XENA_XIIC9_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_SAFETYEN_MASK) >> XENA_XIIC9_SAFETYEN_SHIFT)
#define  XENA_XIIC9_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC9_SAFETYEN_SHIFT))

#define  XENA_XIIC9_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC9_RESERVED1_SHIFT 24
#define  XENA_XIIC9_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_RESERVED1_MASK) >> XENA_XIIC9_RESERVED1_SHIFT)
#define  XENA_XIIC9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC9_RESERVED1_SHIFT))

#define  XENA_XIIC9_DELAY_MASK 0xff0000
#define  XENA_XIIC9_DELAY_SHIFT 16
#define  XENA_XIIC9_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_DELAY_MASK) >> XENA_XIIC9_DELAY_SHIFT)
#define  XENA_XIIC9_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC9_DELAY_SHIFT))

#define  XENA_XIIC9_RESERVED2_MASK 0xfe00
#define  XENA_XIIC9_RESERVED2_SHIFT 9
#define  XENA_XIIC9_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_RESERVED2_MASK) >> XENA_XIIC9_RESERVED2_SHIFT)
#define  XENA_XIIC9_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC9_RESERVED2_SHIFT))

#define  XENA_XIIC9_BITSELECT_MASK 0x1f0
#define  XENA_XIIC9_BITSELECT_SHIFT 4
#define  XENA_XIIC9_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_BITSELECT_MASK) >> XENA_XIIC9_BITSELECT_SHIFT)
#define  XENA_XIIC9_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC9_BITSELECT_SHIFT))

#define  XENA_XIIC9_SELECT_MASK 0xf
#define  XENA_XIIC9_SELECT_SHIFT 0
#define  XENA_XIIC9_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC9_SELECT_MASK) >> XENA_XIIC9_SELECT_SHIFT)
#define  XENA_XIIC9_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC9_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC9_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 11 of 52) (XIIC10)
/** \brief UPCIO IO Configuration Register (Instance 11 of 52)*/
//====================================================================

#define  XENA_XIIC10_INVERT_MASK 0x80000000
#define  XENA_XIIC10_INVERT_SHIFT 31
#define  XENA_XIIC10_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_INVERT_MASK) >> XENA_XIIC10_INVERT_SHIFT)
#define  XENA_XIIC10_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC10_INVERT_SHIFT))

#define  XENA_XIIC10_ENABLE_MASK 0x40000000
#define  XENA_XIIC10_ENABLE_SHIFT 30
#define  XENA_XIIC10_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_ENABLE_MASK) >> XENA_XIIC10_ENABLE_SHIFT)
#define  XENA_XIIC10_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC10_ENABLE_SHIFT))

#define  XENA_XIIC10_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC10_SAFETYEN_SHIFT 29
#define  XENA_XIIC10_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_SAFETYEN_MASK) >> XENA_XIIC10_SAFETYEN_SHIFT)
#define  XENA_XIIC10_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC10_SAFETYEN_SHIFT))

#define  XENA_XIIC10_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC10_RESERVED1_SHIFT 24
#define  XENA_XIIC10_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_RESERVED1_MASK) >> XENA_XIIC10_RESERVED1_SHIFT)
#define  XENA_XIIC10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC10_RESERVED1_SHIFT))

#define  XENA_XIIC10_DELAY_MASK 0xff0000
#define  XENA_XIIC10_DELAY_SHIFT 16
#define  XENA_XIIC10_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_DELAY_MASK) >> XENA_XIIC10_DELAY_SHIFT)
#define  XENA_XIIC10_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC10_DELAY_SHIFT))

#define  XENA_XIIC10_RESERVED2_MASK 0xfe00
#define  XENA_XIIC10_RESERVED2_SHIFT 9
#define  XENA_XIIC10_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_RESERVED2_MASK) >> XENA_XIIC10_RESERVED2_SHIFT)
#define  XENA_XIIC10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC10_RESERVED2_SHIFT))

#define  XENA_XIIC10_BITSELECT_MASK 0x1f0
#define  XENA_XIIC10_BITSELECT_SHIFT 4
#define  XENA_XIIC10_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_BITSELECT_MASK) >> XENA_XIIC10_BITSELECT_SHIFT)
#define  XENA_XIIC10_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC10_BITSELECT_SHIFT))

#define  XENA_XIIC10_SELECT_MASK 0xf
#define  XENA_XIIC10_SELECT_SHIFT 0
#define  XENA_XIIC10_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC10_SELECT_MASK) >> XENA_XIIC10_SELECT_SHIFT)
#define  XENA_XIIC10_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC10_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC10_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 12 of 52) (XIIC11)
/** \brief UPCIO IO Configuration Register (Instance 12 of 52)*/
//====================================================================

#define  XENA_XIIC11_INVERT_MASK 0x80000000
#define  XENA_XIIC11_INVERT_SHIFT 31
#define  XENA_XIIC11_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_INVERT_MASK) >> XENA_XIIC11_INVERT_SHIFT)
#define  XENA_XIIC11_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC11_INVERT_SHIFT))

#define  XENA_XIIC11_ENABLE_MASK 0x40000000
#define  XENA_XIIC11_ENABLE_SHIFT 30
#define  XENA_XIIC11_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_ENABLE_MASK) >> XENA_XIIC11_ENABLE_SHIFT)
#define  XENA_XIIC11_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC11_ENABLE_SHIFT))

#define  XENA_XIIC11_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC11_SAFETYEN_SHIFT 29
#define  XENA_XIIC11_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_SAFETYEN_MASK) >> XENA_XIIC11_SAFETYEN_SHIFT)
#define  XENA_XIIC11_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC11_SAFETYEN_SHIFT))

#define  XENA_XIIC11_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC11_RESERVED1_SHIFT 24
#define  XENA_XIIC11_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_RESERVED1_MASK) >> XENA_XIIC11_RESERVED1_SHIFT)
#define  XENA_XIIC11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC11_RESERVED1_SHIFT))

#define  XENA_XIIC11_DELAY_MASK 0xff0000
#define  XENA_XIIC11_DELAY_SHIFT 16
#define  XENA_XIIC11_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_DELAY_MASK) >> XENA_XIIC11_DELAY_SHIFT)
#define  XENA_XIIC11_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC11_DELAY_SHIFT))

#define  XENA_XIIC11_RESERVED2_MASK 0xfe00
#define  XENA_XIIC11_RESERVED2_SHIFT 9
#define  XENA_XIIC11_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_RESERVED2_MASK) >> XENA_XIIC11_RESERVED2_SHIFT)
#define  XENA_XIIC11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC11_RESERVED2_SHIFT))

#define  XENA_XIIC11_BITSELECT_MASK 0x1f0
#define  XENA_XIIC11_BITSELECT_SHIFT 4
#define  XENA_XIIC11_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_BITSELECT_MASK) >> XENA_XIIC11_BITSELECT_SHIFT)
#define  XENA_XIIC11_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC11_BITSELECT_SHIFT))

#define  XENA_XIIC11_SELECT_MASK 0xf
#define  XENA_XIIC11_SELECT_SHIFT 0
#define  XENA_XIIC11_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC11_SELECT_MASK) >> XENA_XIIC11_SELECT_SHIFT)
#define  XENA_XIIC11_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC11_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC11_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 13 of 52) (XIIC12)
/** \brief UPCIO IO Configuration Register (Instance 13 of 52)*/
//====================================================================

#define  XENA_XIIC12_INVERT_MASK 0x80000000
#define  XENA_XIIC12_INVERT_SHIFT 31
#define  XENA_XIIC12_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_INVERT_MASK) >> XENA_XIIC12_INVERT_SHIFT)
#define  XENA_XIIC12_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC12_INVERT_SHIFT))

#define  XENA_XIIC12_ENABLE_MASK 0x40000000
#define  XENA_XIIC12_ENABLE_SHIFT 30
#define  XENA_XIIC12_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_ENABLE_MASK) >> XENA_XIIC12_ENABLE_SHIFT)
#define  XENA_XIIC12_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC12_ENABLE_SHIFT))

#define  XENA_XIIC12_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC12_SAFETYEN_SHIFT 29
#define  XENA_XIIC12_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_SAFETYEN_MASK) >> XENA_XIIC12_SAFETYEN_SHIFT)
#define  XENA_XIIC12_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC12_SAFETYEN_SHIFT))

#define  XENA_XIIC12_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC12_RESERVED1_SHIFT 24
#define  XENA_XIIC12_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_RESERVED1_MASK) >> XENA_XIIC12_RESERVED1_SHIFT)
#define  XENA_XIIC12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC12_RESERVED1_SHIFT))

#define  XENA_XIIC12_DELAY_MASK 0xff0000
#define  XENA_XIIC12_DELAY_SHIFT 16
#define  XENA_XIIC12_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_DELAY_MASK) >> XENA_XIIC12_DELAY_SHIFT)
#define  XENA_XIIC12_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC12_DELAY_SHIFT))

#define  XENA_XIIC12_RESERVED2_MASK 0xfe00
#define  XENA_XIIC12_RESERVED2_SHIFT 9
#define  XENA_XIIC12_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_RESERVED2_MASK) >> XENA_XIIC12_RESERVED2_SHIFT)
#define  XENA_XIIC12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC12_RESERVED2_SHIFT))

#define  XENA_XIIC12_BITSELECT_MASK 0x1f0
#define  XENA_XIIC12_BITSELECT_SHIFT 4
#define  XENA_XIIC12_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_BITSELECT_MASK) >> XENA_XIIC12_BITSELECT_SHIFT)
#define  XENA_XIIC12_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC12_BITSELECT_SHIFT))

#define  XENA_XIIC12_SELECT_MASK 0xf
#define  XENA_XIIC12_SELECT_SHIFT 0
#define  XENA_XIIC12_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC12_SELECT_MASK) >> XENA_XIIC12_SELECT_SHIFT)
#define  XENA_XIIC12_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC12_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC12_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 14 of 52) (XIIC13)
/** \brief UPCIO IO Configuration Register (Instance 14 of 52)*/
//====================================================================

#define  XENA_XIIC13_INVERT_MASK 0x80000000
#define  XENA_XIIC13_INVERT_SHIFT 31
#define  XENA_XIIC13_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_INVERT_MASK) >> XENA_XIIC13_INVERT_SHIFT)
#define  XENA_XIIC13_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC13_INVERT_SHIFT))

#define  XENA_XIIC13_ENABLE_MASK 0x40000000
#define  XENA_XIIC13_ENABLE_SHIFT 30
#define  XENA_XIIC13_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_ENABLE_MASK) >> XENA_XIIC13_ENABLE_SHIFT)
#define  XENA_XIIC13_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC13_ENABLE_SHIFT))

#define  XENA_XIIC13_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC13_SAFETYEN_SHIFT 29
#define  XENA_XIIC13_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_SAFETYEN_MASK) >> XENA_XIIC13_SAFETYEN_SHIFT)
#define  XENA_XIIC13_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC13_SAFETYEN_SHIFT))

#define  XENA_XIIC13_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC13_RESERVED1_SHIFT 24
#define  XENA_XIIC13_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_RESERVED1_MASK) >> XENA_XIIC13_RESERVED1_SHIFT)
#define  XENA_XIIC13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC13_RESERVED1_SHIFT))

#define  XENA_XIIC13_DELAY_MASK 0xff0000
#define  XENA_XIIC13_DELAY_SHIFT 16
#define  XENA_XIIC13_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_DELAY_MASK) >> XENA_XIIC13_DELAY_SHIFT)
#define  XENA_XIIC13_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC13_DELAY_SHIFT))

#define  XENA_XIIC13_RESERVED2_MASK 0xfe00
#define  XENA_XIIC13_RESERVED2_SHIFT 9
#define  XENA_XIIC13_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_RESERVED2_MASK) >> XENA_XIIC13_RESERVED2_SHIFT)
#define  XENA_XIIC13_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC13_RESERVED2_SHIFT))

#define  XENA_XIIC13_BITSELECT_MASK 0x1f0
#define  XENA_XIIC13_BITSELECT_SHIFT 4
#define  XENA_XIIC13_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_BITSELECT_MASK) >> XENA_XIIC13_BITSELECT_SHIFT)
#define  XENA_XIIC13_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC13_BITSELECT_SHIFT))

#define  XENA_XIIC13_SELECT_MASK 0xf
#define  XENA_XIIC13_SELECT_SHIFT 0
#define  XENA_XIIC13_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC13_SELECT_MASK) >> XENA_XIIC13_SELECT_SHIFT)
#define  XENA_XIIC13_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC13_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC13_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 15 of 52) (XIIC14)
/** \brief UPCIO IO Configuration Register (Instance 15 of 52)*/
//====================================================================

#define  XENA_XIIC14_INVERT_MASK 0x80000000
#define  XENA_XIIC14_INVERT_SHIFT 31
#define  XENA_XIIC14_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_INVERT_MASK) >> XENA_XIIC14_INVERT_SHIFT)
#define  XENA_XIIC14_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC14_INVERT_SHIFT))

#define  XENA_XIIC14_ENABLE_MASK 0x40000000
#define  XENA_XIIC14_ENABLE_SHIFT 30
#define  XENA_XIIC14_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_ENABLE_MASK) >> XENA_XIIC14_ENABLE_SHIFT)
#define  XENA_XIIC14_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC14_ENABLE_SHIFT))

#define  XENA_XIIC14_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC14_SAFETYEN_SHIFT 29
#define  XENA_XIIC14_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_SAFETYEN_MASK) >> XENA_XIIC14_SAFETYEN_SHIFT)
#define  XENA_XIIC14_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC14_SAFETYEN_SHIFT))

#define  XENA_XIIC14_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC14_RESERVED1_SHIFT 24
#define  XENA_XIIC14_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_RESERVED1_MASK) >> XENA_XIIC14_RESERVED1_SHIFT)
#define  XENA_XIIC14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC14_RESERVED1_SHIFT))

#define  XENA_XIIC14_DELAY_MASK 0xff0000
#define  XENA_XIIC14_DELAY_SHIFT 16
#define  XENA_XIIC14_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_DELAY_MASK) >> XENA_XIIC14_DELAY_SHIFT)
#define  XENA_XIIC14_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC14_DELAY_SHIFT))

#define  XENA_XIIC14_RESERVED2_MASK 0xfe00
#define  XENA_XIIC14_RESERVED2_SHIFT 9
#define  XENA_XIIC14_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_RESERVED2_MASK) >> XENA_XIIC14_RESERVED2_SHIFT)
#define  XENA_XIIC14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC14_RESERVED2_SHIFT))

#define  XENA_XIIC14_BITSELECT_MASK 0x1f0
#define  XENA_XIIC14_BITSELECT_SHIFT 4
#define  XENA_XIIC14_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_BITSELECT_MASK) >> XENA_XIIC14_BITSELECT_SHIFT)
#define  XENA_XIIC14_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC14_BITSELECT_SHIFT))

#define  XENA_XIIC14_SELECT_MASK 0xf
#define  XENA_XIIC14_SELECT_SHIFT 0
#define  XENA_XIIC14_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC14_SELECT_MASK) >> XENA_XIIC14_SELECT_SHIFT)
#define  XENA_XIIC14_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC14_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC14_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 16 of 52) (XIIC15)
/** \brief UPCIO IO Configuration Register (Instance 16 of 52)*/
//====================================================================

#define  XENA_XIIC15_INVERT_MASK 0x80000000
#define  XENA_XIIC15_INVERT_SHIFT 31
#define  XENA_XIIC15_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_INVERT_MASK) >> XENA_XIIC15_INVERT_SHIFT)
#define  XENA_XIIC15_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC15_INVERT_SHIFT))

#define  XENA_XIIC15_ENABLE_MASK 0x40000000
#define  XENA_XIIC15_ENABLE_SHIFT 30
#define  XENA_XIIC15_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_ENABLE_MASK) >> XENA_XIIC15_ENABLE_SHIFT)
#define  XENA_XIIC15_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC15_ENABLE_SHIFT))

#define  XENA_XIIC15_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC15_SAFETYEN_SHIFT 29
#define  XENA_XIIC15_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_SAFETYEN_MASK) >> XENA_XIIC15_SAFETYEN_SHIFT)
#define  XENA_XIIC15_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC15_SAFETYEN_SHIFT))

#define  XENA_XIIC15_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC15_RESERVED1_SHIFT 24
#define  XENA_XIIC15_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_RESERVED1_MASK) >> XENA_XIIC15_RESERVED1_SHIFT)
#define  XENA_XIIC15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC15_RESERVED1_SHIFT))

#define  XENA_XIIC15_DELAY_MASK 0xff0000
#define  XENA_XIIC15_DELAY_SHIFT 16
#define  XENA_XIIC15_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_DELAY_MASK) >> XENA_XIIC15_DELAY_SHIFT)
#define  XENA_XIIC15_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC15_DELAY_SHIFT))

#define  XENA_XIIC15_RESERVED2_MASK 0xfe00
#define  XENA_XIIC15_RESERVED2_SHIFT 9
#define  XENA_XIIC15_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_RESERVED2_MASK) >> XENA_XIIC15_RESERVED2_SHIFT)
#define  XENA_XIIC15_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC15_RESERVED2_SHIFT))

#define  XENA_XIIC15_BITSELECT_MASK 0x1f0
#define  XENA_XIIC15_BITSELECT_SHIFT 4
#define  XENA_XIIC15_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_BITSELECT_MASK) >> XENA_XIIC15_BITSELECT_SHIFT)
#define  XENA_XIIC15_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC15_BITSELECT_SHIFT))

#define  XENA_XIIC15_SELECT_MASK 0xf
#define  XENA_XIIC15_SELECT_SHIFT 0
#define  XENA_XIIC15_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC15_SELECT_MASK) >> XENA_XIIC15_SELECT_SHIFT)
#define  XENA_XIIC15_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC15_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC15_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 17 of 52) (XIIC16)
/** \brief UPCIO IO Configuration Register (Instance 17 of 52)*/
//====================================================================

#define  XENA_XIIC16_INVERT_MASK 0x80000000
#define  XENA_XIIC16_INVERT_SHIFT 31
#define  XENA_XIIC16_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_INVERT_MASK) >> XENA_XIIC16_INVERT_SHIFT)
#define  XENA_XIIC16_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC16_INVERT_SHIFT))

#define  XENA_XIIC16_ENABLE_MASK 0x40000000
#define  XENA_XIIC16_ENABLE_SHIFT 30
#define  XENA_XIIC16_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_ENABLE_MASK) >> XENA_XIIC16_ENABLE_SHIFT)
#define  XENA_XIIC16_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC16_ENABLE_SHIFT))

#define  XENA_XIIC16_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC16_SAFETYEN_SHIFT 29
#define  XENA_XIIC16_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_SAFETYEN_MASK) >> XENA_XIIC16_SAFETYEN_SHIFT)
#define  XENA_XIIC16_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC16_SAFETYEN_SHIFT))

#define  XENA_XIIC16_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC16_RESERVED1_SHIFT 24
#define  XENA_XIIC16_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_RESERVED1_MASK) >> XENA_XIIC16_RESERVED1_SHIFT)
#define  XENA_XIIC16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC16_RESERVED1_SHIFT))

#define  XENA_XIIC16_DELAY_MASK 0xff0000
#define  XENA_XIIC16_DELAY_SHIFT 16
#define  XENA_XIIC16_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_DELAY_MASK) >> XENA_XIIC16_DELAY_SHIFT)
#define  XENA_XIIC16_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC16_DELAY_SHIFT))

#define  XENA_XIIC16_RESERVED2_MASK 0xfe00
#define  XENA_XIIC16_RESERVED2_SHIFT 9
#define  XENA_XIIC16_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_RESERVED2_MASK) >> XENA_XIIC16_RESERVED2_SHIFT)
#define  XENA_XIIC16_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC16_RESERVED2_SHIFT))

#define  XENA_XIIC16_BITSELECT_MASK 0x1f0
#define  XENA_XIIC16_BITSELECT_SHIFT 4
#define  XENA_XIIC16_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_BITSELECT_MASK) >> XENA_XIIC16_BITSELECT_SHIFT)
#define  XENA_XIIC16_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC16_BITSELECT_SHIFT))

#define  XENA_XIIC16_SELECT_MASK 0xf
#define  XENA_XIIC16_SELECT_SHIFT 0
#define  XENA_XIIC16_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC16_SELECT_MASK) >> XENA_XIIC16_SELECT_SHIFT)
#define  XENA_XIIC16_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC16_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC16_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 18 of 52) (XIIC17)
/** \brief UPCIO IO Configuration Register (Instance 18 of 52)*/
//====================================================================

#define  XENA_XIIC17_INVERT_MASK 0x80000000
#define  XENA_XIIC17_INVERT_SHIFT 31
#define  XENA_XIIC17_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_INVERT_MASK) >> XENA_XIIC17_INVERT_SHIFT)
#define  XENA_XIIC17_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC17_INVERT_SHIFT))

#define  XENA_XIIC17_ENABLE_MASK 0x40000000
#define  XENA_XIIC17_ENABLE_SHIFT 30
#define  XENA_XIIC17_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_ENABLE_MASK) >> XENA_XIIC17_ENABLE_SHIFT)
#define  XENA_XIIC17_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC17_ENABLE_SHIFT))

#define  XENA_XIIC17_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC17_SAFETYEN_SHIFT 29
#define  XENA_XIIC17_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_SAFETYEN_MASK) >> XENA_XIIC17_SAFETYEN_SHIFT)
#define  XENA_XIIC17_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC17_SAFETYEN_SHIFT))

#define  XENA_XIIC17_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC17_RESERVED1_SHIFT 24
#define  XENA_XIIC17_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_RESERVED1_MASK) >> XENA_XIIC17_RESERVED1_SHIFT)
#define  XENA_XIIC17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC17_RESERVED1_SHIFT))

#define  XENA_XIIC17_DELAY_MASK 0xff0000
#define  XENA_XIIC17_DELAY_SHIFT 16
#define  XENA_XIIC17_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_DELAY_MASK) >> XENA_XIIC17_DELAY_SHIFT)
#define  XENA_XIIC17_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC17_DELAY_SHIFT))

#define  XENA_XIIC17_RESERVED2_MASK 0xfe00
#define  XENA_XIIC17_RESERVED2_SHIFT 9
#define  XENA_XIIC17_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_RESERVED2_MASK) >> XENA_XIIC17_RESERVED2_SHIFT)
#define  XENA_XIIC17_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC17_RESERVED2_SHIFT))

#define  XENA_XIIC17_BITSELECT_MASK 0x1f0
#define  XENA_XIIC17_BITSELECT_SHIFT 4
#define  XENA_XIIC17_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_BITSELECT_MASK) >> XENA_XIIC17_BITSELECT_SHIFT)
#define  XENA_XIIC17_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC17_BITSELECT_SHIFT))

#define  XENA_XIIC17_SELECT_MASK 0xf
#define  XENA_XIIC17_SELECT_SHIFT 0
#define  XENA_XIIC17_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC17_SELECT_MASK) >> XENA_XIIC17_SELECT_SHIFT)
#define  XENA_XIIC17_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC17_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC17_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 19 of 52) (XIIC18)
/** \brief UPCIO IO Configuration Register (Instance 19 of 52)*/
//====================================================================

#define  XENA_XIIC18_INVERT_MASK 0x80000000
#define  XENA_XIIC18_INVERT_SHIFT 31
#define  XENA_XIIC18_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_INVERT_MASK) >> XENA_XIIC18_INVERT_SHIFT)
#define  XENA_XIIC18_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC18_INVERT_SHIFT))

#define  XENA_XIIC18_ENABLE_MASK 0x40000000
#define  XENA_XIIC18_ENABLE_SHIFT 30
#define  XENA_XIIC18_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_ENABLE_MASK) >> XENA_XIIC18_ENABLE_SHIFT)
#define  XENA_XIIC18_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC18_ENABLE_SHIFT))

#define  XENA_XIIC18_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC18_SAFETYEN_SHIFT 29
#define  XENA_XIIC18_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_SAFETYEN_MASK) >> XENA_XIIC18_SAFETYEN_SHIFT)
#define  XENA_XIIC18_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC18_SAFETYEN_SHIFT))

#define  XENA_XIIC18_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC18_RESERVED1_SHIFT 24
#define  XENA_XIIC18_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_RESERVED1_MASK) >> XENA_XIIC18_RESERVED1_SHIFT)
#define  XENA_XIIC18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC18_RESERVED1_SHIFT))

#define  XENA_XIIC18_DELAY_MASK 0xff0000
#define  XENA_XIIC18_DELAY_SHIFT 16
#define  XENA_XIIC18_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_DELAY_MASK) >> XENA_XIIC18_DELAY_SHIFT)
#define  XENA_XIIC18_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC18_DELAY_SHIFT))

#define  XENA_XIIC18_RESERVED2_MASK 0xfe00
#define  XENA_XIIC18_RESERVED2_SHIFT 9
#define  XENA_XIIC18_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_RESERVED2_MASK) >> XENA_XIIC18_RESERVED2_SHIFT)
#define  XENA_XIIC18_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC18_RESERVED2_SHIFT))

#define  XENA_XIIC18_BITSELECT_MASK 0x1f0
#define  XENA_XIIC18_BITSELECT_SHIFT 4
#define  XENA_XIIC18_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_BITSELECT_MASK) >> XENA_XIIC18_BITSELECT_SHIFT)
#define  XENA_XIIC18_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC18_BITSELECT_SHIFT))

#define  XENA_XIIC18_SELECT_MASK 0xf
#define  XENA_XIIC18_SELECT_SHIFT 0
#define  XENA_XIIC18_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC18_SELECT_MASK) >> XENA_XIIC18_SELECT_SHIFT)
#define  XENA_XIIC18_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC18_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC18_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 20 of 52) (XIIC19)
/** \brief UPCIO IO Configuration Register (Instance 20 of 52)*/
//====================================================================

#define  XENA_XIIC19_INVERT_MASK 0x80000000
#define  XENA_XIIC19_INVERT_SHIFT 31
#define  XENA_XIIC19_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_INVERT_MASK) >> XENA_XIIC19_INVERT_SHIFT)
#define  XENA_XIIC19_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC19_INVERT_SHIFT))

#define  XENA_XIIC19_ENABLE_MASK 0x40000000
#define  XENA_XIIC19_ENABLE_SHIFT 30
#define  XENA_XIIC19_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_ENABLE_MASK) >> XENA_XIIC19_ENABLE_SHIFT)
#define  XENA_XIIC19_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC19_ENABLE_SHIFT))

#define  XENA_XIIC19_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC19_SAFETYEN_SHIFT 29
#define  XENA_XIIC19_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_SAFETYEN_MASK) >> XENA_XIIC19_SAFETYEN_SHIFT)
#define  XENA_XIIC19_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC19_SAFETYEN_SHIFT))

#define  XENA_XIIC19_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC19_RESERVED1_SHIFT 24
#define  XENA_XIIC19_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_RESERVED1_MASK) >> XENA_XIIC19_RESERVED1_SHIFT)
#define  XENA_XIIC19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC19_RESERVED1_SHIFT))

#define  XENA_XIIC19_DELAY_MASK 0xff0000
#define  XENA_XIIC19_DELAY_SHIFT 16
#define  XENA_XIIC19_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_DELAY_MASK) >> XENA_XIIC19_DELAY_SHIFT)
#define  XENA_XIIC19_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC19_DELAY_SHIFT))

#define  XENA_XIIC19_RESERVED2_MASK 0xfe00
#define  XENA_XIIC19_RESERVED2_SHIFT 9
#define  XENA_XIIC19_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_RESERVED2_MASK) >> XENA_XIIC19_RESERVED2_SHIFT)
#define  XENA_XIIC19_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC19_RESERVED2_SHIFT))

#define  XENA_XIIC19_BITSELECT_MASK 0x1f0
#define  XENA_XIIC19_BITSELECT_SHIFT 4
#define  XENA_XIIC19_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_BITSELECT_MASK) >> XENA_XIIC19_BITSELECT_SHIFT)
#define  XENA_XIIC19_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC19_BITSELECT_SHIFT))

#define  XENA_XIIC19_SELECT_MASK 0xf
#define  XENA_XIIC19_SELECT_SHIFT 0
#define  XENA_XIIC19_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC19_SELECT_MASK) >> XENA_XIIC19_SELECT_SHIFT)
#define  XENA_XIIC19_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC19_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC19_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 21 of 52) (XIIC20)
/** \brief UPCIO IO Configuration Register (Instance 21 of 52)*/
//====================================================================

#define  XENA_XIIC20_INVERT_MASK 0x80000000
#define  XENA_XIIC20_INVERT_SHIFT 31
#define  XENA_XIIC20_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_INVERT_MASK) >> XENA_XIIC20_INVERT_SHIFT)
#define  XENA_XIIC20_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC20_INVERT_SHIFT))

#define  XENA_XIIC20_ENABLE_MASK 0x40000000
#define  XENA_XIIC20_ENABLE_SHIFT 30
#define  XENA_XIIC20_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_ENABLE_MASK) >> XENA_XIIC20_ENABLE_SHIFT)
#define  XENA_XIIC20_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC20_ENABLE_SHIFT))

#define  XENA_XIIC20_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC20_SAFETYEN_SHIFT 29
#define  XENA_XIIC20_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_SAFETYEN_MASK) >> XENA_XIIC20_SAFETYEN_SHIFT)
#define  XENA_XIIC20_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC20_SAFETYEN_SHIFT))

#define  XENA_XIIC20_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC20_RESERVED1_SHIFT 24
#define  XENA_XIIC20_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_RESERVED1_MASK) >> XENA_XIIC20_RESERVED1_SHIFT)
#define  XENA_XIIC20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC20_RESERVED1_SHIFT))

#define  XENA_XIIC20_DELAY_MASK 0xff0000
#define  XENA_XIIC20_DELAY_SHIFT 16
#define  XENA_XIIC20_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_DELAY_MASK) >> XENA_XIIC20_DELAY_SHIFT)
#define  XENA_XIIC20_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC20_DELAY_SHIFT))

#define  XENA_XIIC20_RESERVED2_MASK 0xfe00
#define  XENA_XIIC20_RESERVED2_SHIFT 9
#define  XENA_XIIC20_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_RESERVED2_MASK) >> XENA_XIIC20_RESERVED2_SHIFT)
#define  XENA_XIIC20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC20_RESERVED2_SHIFT))

#define  XENA_XIIC20_BITSELECT_MASK 0x1f0
#define  XENA_XIIC20_BITSELECT_SHIFT 4
#define  XENA_XIIC20_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_BITSELECT_MASK) >> XENA_XIIC20_BITSELECT_SHIFT)
#define  XENA_XIIC20_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC20_BITSELECT_SHIFT))

#define  XENA_XIIC20_SELECT_MASK 0xf
#define  XENA_XIIC20_SELECT_SHIFT 0
#define  XENA_XIIC20_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC20_SELECT_MASK) >> XENA_XIIC20_SELECT_SHIFT)
#define  XENA_XIIC20_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC20_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC20_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 22 of 52) (XIIC21)
/** \brief UPCIO IO Configuration Register (Instance 22 of 52)*/
//====================================================================

#define  XENA_XIIC21_INVERT_MASK 0x80000000
#define  XENA_XIIC21_INVERT_SHIFT 31
#define  XENA_XIIC21_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_INVERT_MASK) >> XENA_XIIC21_INVERT_SHIFT)
#define  XENA_XIIC21_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC21_INVERT_SHIFT))

#define  XENA_XIIC21_ENABLE_MASK 0x40000000
#define  XENA_XIIC21_ENABLE_SHIFT 30
#define  XENA_XIIC21_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_ENABLE_MASK) >> XENA_XIIC21_ENABLE_SHIFT)
#define  XENA_XIIC21_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC21_ENABLE_SHIFT))

#define  XENA_XIIC21_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC21_SAFETYEN_SHIFT 29
#define  XENA_XIIC21_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_SAFETYEN_MASK) >> XENA_XIIC21_SAFETYEN_SHIFT)
#define  XENA_XIIC21_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC21_SAFETYEN_SHIFT))

#define  XENA_XIIC21_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC21_RESERVED1_SHIFT 24
#define  XENA_XIIC21_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_RESERVED1_MASK) >> XENA_XIIC21_RESERVED1_SHIFT)
#define  XENA_XIIC21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC21_RESERVED1_SHIFT))

#define  XENA_XIIC21_DELAY_MASK 0xff0000
#define  XENA_XIIC21_DELAY_SHIFT 16
#define  XENA_XIIC21_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_DELAY_MASK) >> XENA_XIIC21_DELAY_SHIFT)
#define  XENA_XIIC21_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC21_DELAY_SHIFT))

#define  XENA_XIIC21_RESERVED2_MASK 0xfe00
#define  XENA_XIIC21_RESERVED2_SHIFT 9
#define  XENA_XIIC21_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_RESERVED2_MASK) >> XENA_XIIC21_RESERVED2_SHIFT)
#define  XENA_XIIC21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC21_RESERVED2_SHIFT))

#define  XENA_XIIC21_BITSELECT_MASK 0x1f0
#define  XENA_XIIC21_BITSELECT_SHIFT 4
#define  XENA_XIIC21_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_BITSELECT_MASK) >> XENA_XIIC21_BITSELECT_SHIFT)
#define  XENA_XIIC21_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC21_BITSELECT_SHIFT))

#define  XENA_XIIC21_SELECT_MASK 0xf
#define  XENA_XIIC21_SELECT_SHIFT 0
#define  XENA_XIIC21_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC21_SELECT_MASK) >> XENA_XIIC21_SELECT_SHIFT)
#define  XENA_XIIC21_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC21_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC21_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 23 of 52) (XIIC22)
/** \brief UPCIO IO Configuration Register (Instance 23 of 52)*/
//====================================================================

#define  XENA_XIIC22_INVERT_MASK 0x80000000
#define  XENA_XIIC22_INVERT_SHIFT 31
#define  XENA_XIIC22_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_INVERT_MASK) >> XENA_XIIC22_INVERT_SHIFT)
#define  XENA_XIIC22_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC22_INVERT_SHIFT))

#define  XENA_XIIC22_ENABLE_MASK 0x40000000
#define  XENA_XIIC22_ENABLE_SHIFT 30
#define  XENA_XIIC22_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_ENABLE_MASK) >> XENA_XIIC22_ENABLE_SHIFT)
#define  XENA_XIIC22_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC22_ENABLE_SHIFT))

#define  XENA_XIIC22_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC22_SAFETYEN_SHIFT 29
#define  XENA_XIIC22_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_SAFETYEN_MASK) >> XENA_XIIC22_SAFETYEN_SHIFT)
#define  XENA_XIIC22_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC22_SAFETYEN_SHIFT))

#define  XENA_XIIC22_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC22_RESERVED1_SHIFT 24
#define  XENA_XIIC22_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_RESERVED1_MASK) >> XENA_XIIC22_RESERVED1_SHIFT)
#define  XENA_XIIC22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC22_RESERVED1_SHIFT))

#define  XENA_XIIC22_DELAY_MASK 0xff0000
#define  XENA_XIIC22_DELAY_SHIFT 16
#define  XENA_XIIC22_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_DELAY_MASK) >> XENA_XIIC22_DELAY_SHIFT)
#define  XENA_XIIC22_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC22_DELAY_SHIFT))

#define  XENA_XIIC22_RESERVED2_MASK 0xfe00
#define  XENA_XIIC22_RESERVED2_SHIFT 9
#define  XENA_XIIC22_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_RESERVED2_MASK) >> XENA_XIIC22_RESERVED2_SHIFT)
#define  XENA_XIIC22_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC22_RESERVED2_SHIFT))

#define  XENA_XIIC22_BITSELECT_MASK 0x1f0
#define  XENA_XIIC22_BITSELECT_SHIFT 4
#define  XENA_XIIC22_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_BITSELECT_MASK) >> XENA_XIIC22_BITSELECT_SHIFT)
#define  XENA_XIIC22_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC22_BITSELECT_SHIFT))

#define  XENA_XIIC22_SELECT_MASK 0xf
#define  XENA_XIIC22_SELECT_SHIFT 0
#define  XENA_XIIC22_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC22_SELECT_MASK) >> XENA_XIIC22_SELECT_SHIFT)
#define  XENA_XIIC22_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC22_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC22_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 24 of 52) (XIIC23)
/** \brief UPCIO IO Configuration Register (Instance 24 of 52)*/
//====================================================================

#define  XENA_XIIC23_INVERT_MASK 0x80000000
#define  XENA_XIIC23_INVERT_SHIFT 31
#define  XENA_XIIC23_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_INVERT_MASK) >> XENA_XIIC23_INVERT_SHIFT)
#define  XENA_XIIC23_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC23_INVERT_SHIFT))

#define  XENA_XIIC23_ENABLE_MASK 0x40000000
#define  XENA_XIIC23_ENABLE_SHIFT 30
#define  XENA_XIIC23_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_ENABLE_MASK) >> XENA_XIIC23_ENABLE_SHIFT)
#define  XENA_XIIC23_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC23_ENABLE_SHIFT))

#define  XENA_XIIC23_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC23_SAFETYEN_SHIFT 29
#define  XENA_XIIC23_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_SAFETYEN_MASK) >> XENA_XIIC23_SAFETYEN_SHIFT)
#define  XENA_XIIC23_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC23_SAFETYEN_SHIFT))

#define  XENA_XIIC23_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC23_RESERVED1_SHIFT 24
#define  XENA_XIIC23_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_RESERVED1_MASK) >> XENA_XIIC23_RESERVED1_SHIFT)
#define  XENA_XIIC23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC23_RESERVED1_SHIFT))

#define  XENA_XIIC23_DELAY_MASK 0xff0000
#define  XENA_XIIC23_DELAY_SHIFT 16
#define  XENA_XIIC23_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_DELAY_MASK) >> XENA_XIIC23_DELAY_SHIFT)
#define  XENA_XIIC23_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC23_DELAY_SHIFT))

#define  XENA_XIIC23_RESERVED2_MASK 0xfe00
#define  XENA_XIIC23_RESERVED2_SHIFT 9
#define  XENA_XIIC23_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_RESERVED2_MASK) >> XENA_XIIC23_RESERVED2_SHIFT)
#define  XENA_XIIC23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC23_RESERVED2_SHIFT))

#define  XENA_XIIC23_BITSELECT_MASK 0x1f0
#define  XENA_XIIC23_BITSELECT_SHIFT 4
#define  XENA_XIIC23_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_BITSELECT_MASK) >> XENA_XIIC23_BITSELECT_SHIFT)
#define  XENA_XIIC23_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC23_BITSELECT_SHIFT))

#define  XENA_XIIC23_SELECT_MASK 0xf
#define  XENA_XIIC23_SELECT_SHIFT 0
#define  XENA_XIIC23_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC23_SELECT_MASK) >> XENA_XIIC23_SELECT_SHIFT)
#define  XENA_XIIC23_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC23_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC23_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 25 of 52) (XIIC24)
/** \brief UPCIO IO Configuration Register (Instance 25 of 52)*/
//====================================================================

#define  XENA_XIIC24_INVERT_MASK 0x80000000
#define  XENA_XIIC24_INVERT_SHIFT 31
#define  XENA_XIIC24_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_INVERT_MASK) >> XENA_XIIC24_INVERT_SHIFT)
#define  XENA_XIIC24_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC24_INVERT_SHIFT))

#define  XENA_XIIC24_ENABLE_MASK 0x40000000
#define  XENA_XIIC24_ENABLE_SHIFT 30
#define  XENA_XIIC24_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_ENABLE_MASK) >> XENA_XIIC24_ENABLE_SHIFT)
#define  XENA_XIIC24_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC24_ENABLE_SHIFT))

#define  XENA_XIIC24_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC24_SAFETYEN_SHIFT 29
#define  XENA_XIIC24_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_SAFETYEN_MASK) >> XENA_XIIC24_SAFETYEN_SHIFT)
#define  XENA_XIIC24_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC24_SAFETYEN_SHIFT))

#define  XENA_XIIC24_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC24_RESERVED1_SHIFT 24
#define  XENA_XIIC24_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_RESERVED1_MASK) >> XENA_XIIC24_RESERVED1_SHIFT)
#define  XENA_XIIC24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC24_RESERVED1_SHIFT))

#define  XENA_XIIC24_DELAY_MASK 0xff0000
#define  XENA_XIIC24_DELAY_SHIFT 16
#define  XENA_XIIC24_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_DELAY_MASK) >> XENA_XIIC24_DELAY_SHIFT)
#define  XENA_XIIC24_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC24_DELAY_SHIFT))

#define  XENA_XIIC24_RESERVED2_MASK 0xfe00
#define  XENA_XIIC24_RESERVED2_SHIFT 9
#define  XENA_XIIC24_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_RESERVED2_MASK) >> XENA_XIIC24_RESERVED2_SHIFT)
#define  XENA_XIIC24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC24_RESERVED2_SHIFT))

#define  XENA_XIIC24_BITSELECT_MASK 0x1f0
#define  XENA_XIIC24_BITSELECT_SHIFT 4
#define  XENA_XIIC24_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_BITSELECT_MASK) >> XENA_XIIC24_BITSELECT_SHIFT)
#define  XENA_XIIC24_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC24_BITSELECT_SHIFT))

#define  XENA_XIIC24_SELECT_MASK 0xf
#define  XENA_XIIC24_SELECT_SHIFT 0
#define  XENA_XIIC24_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC24_SELECT_MASK) >> XENA_XIIC24_SELECT_SHIFT)
#define  XENA_XIIC24_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC24_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC24_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 26 of 52) (XIIC25)
/** \brief UPCIO IO Configuration Register (Instance 26 of 52)*/
//====================================================================

#define  XENA_XIIC25_INVERT_MASK 0x80000000
#define  XENA_XIIC25_INVERT_SHIFT 31
#define  XENA_XIIC25_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_INVERT_MASK) >> XENA_XIIC25_INVERT_SHIFT)
#define  XENA_XIIC25_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC25_INVERT_SHIFT))

#define  XENA_XIIC25_ENABLE_MASK 0x40000000
#define  XENA_XIIC25_ENABLE_SHIFT 30
#define  XENA_XIIC25_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_ENABLE_MASK) >> XENA_XIIC25_ENABLE_SHIFT)
#define  XENA_XIIC25_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC25_ENABLE_SHIFT))

#define  XENA_XIIC25_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC25_SAFETYEN_SHIFT 29
#define  XENA_XIIC25_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_SAFETYEN_MASK) >> XENA_XIIC25_SAFETYEN_SHIFT)
#define  XENA_XIIC25_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC25_SAFETYEN_SHIFT))

#define  XENA_XIIC25_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC25_RESERVED1_SHIFT 24
#define  XENA_XIIC25_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_RESERVED1_MASK) >> XENA_XIIC25_RESERVED1_SHIFT)
#define  XENA_XIIC25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC25_RESERVED1_SHIFT))

#define  XENA_XIIC25_DELAY_MASK 0xff0000
#define  XENA_XIIC25_DELAY_SHIFT 16
#define  XENA_XIIC25_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_DELAY_MASK) >> XENA_XIIC25_DELAY_SHIFT)
#define  XENA_XIIC25_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC25_DELAY_SHIFT))

#define  XENA_XIIC25_RESERVED2_MASK 0xfe00
#define  XENA_XIIC25_RESERVED2_SHIFT 9
#define  XENA_XIIC25_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_RESERVED2_MASK) >> XENA_XIIC25_RESERVED2_SHIFT)
#define  XENA_XIIC25_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC25_RESERVED2_SHIFT))

#define  XENA_XIIC25_BITSELECT_MASK 0x1f0
#define  XENA_XIIC25_BITSELECT_SHIFT 4
#define  XENA_XIIC25_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_BITSELECT_MASK) >> XENA_XIIC25_BITSELECT_SHIFT)
#define  XENA_XIIC25_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC25_BITSELECT_SHIFT))

#define  XENA_XIIC25_SELECT_MASK 0xf
#define  XENA_XIIC25_SELECT_SHIFT 0
#define  XENA_XIIC25_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC25_SELECT_MASK) >> XENA_XIIC25_SELECT_SHIFT)
#define  XENA_XIIC25_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC25_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC25_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 27 of 52) (XIIC26)
/** \brief UPCIO IO Configuration Register (Instance 27 of 52)*/
//====================================================================

#define  XENA_XIIC26_INVERT_MASK 0x80000000
#define  XENA_XIIC26_INVERT_SHIFT 31
#define  XENA_XIIC26_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_INVERT_MASK) >> XENA_XIIC26_INVERT_SHIFT)
#define  XENA_XIIC26_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC26_INVERT_SHIFT))

#define  XENA_XIIC26_ENABLE_MASK 0x40000000
#define  XENA_XIIC26_ENABLE_SHIFT 30
#define  XENA_XIIC26_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_ENABLE_MASK) >> XENA_XIIC26_ENABLE_SHIFT)
#define  XENA_XIIC26_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC26_ENABLE_SHIFT))

#define  XENA_XIIC26_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC26_SAFETYEN_SHIFT 29
#define  XENA_XIIC26_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_SAFETYEN_MASK) >> XENA_XIIC26_SAFETYEN_SHIFT)
#define  XENA_XIIC26_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC26_SAFETYEN_SHIFT))

#define  XENA_XIIC26_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC26_RESERVED1_SHIFT 24
#define  XENA_XIIC26_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_RESERVED1_MASK) >> XENA_XIIC26_RESERVED1_SHIFT)
#define  XENA_XIIC26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC26_RESERVED1_SHIFT))

#define  XENA_XIIC26_DELAY_MASK 0xff0000
#define  XENA_XIIC26_DELAY_SHIFT 16
#define  XENA_XIIC26_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_DELAY_MASK) >> XENA_XIIC26_DELAY_SHIFT)
#define  XENA_XIIC26_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC26_DELAY_SHIFT))

#define  XENA_XIIC26_RESERVED2_MASK 0xfe00
#define  XENA_XIIC26_RESERVED2_SHIFT 9
#define  XENA_XIIC26_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_RESERVED2_MASK) >> XENA_XIIC26_RESERVED2_SHIFT)
#define  XENA_XIIC26_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC26_RESERVED2_SHIFT))

#define  XENA_XIIC26_BITSELECT_MASK 0x1f0
#define  XENA_XIIC26_BITSELECT_SHIFT 4
#define  XENA_XIIC26_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_BITSELECT_MASK) >> XENA_XIIC26_BITSELECT_SHIFT)
#define  XENA_XIIC26_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC26_BITSELECT_SHIFT))

#define  XENA_XIIC26_SELECT_MASK 0xf
#define  XENA_XIIC26_SELECT_SHIFT 0
#define  XENA_XIIC26_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC26_SELECT_MASK) >> XENA_XIIC26_SELECT_SHIFT)
#define  XENA_XIIC26_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC26_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC26_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 28 of 52) (XIIC27)
/** \brief UPCIO IO Configuration Register (Instance 28 of 52)*/
//====================================================================

#define  XENA_XIIC27_INVERT_MASK 0x80000000
#define  XENA_XIIC27_INVERT_SHIFT 31
#define  XENA_XIIC27_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_INVERT_MASK) >> XENA_XIIC27_INVERT_SHIFT)
#define  XENA_XIIC27_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC27_INVERT_SHIFT))

#define  XENA_XIIC27_ENABLE_MASK 0x40000000
#define  XENA_XIIC27_ENABLE_SHIFT 30
#define  XENA_XIIC27_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_ENABLE_MASK) >> XENA_XIIC27_ENABLE_SHIFT)
#define  XENA_XIIC27_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC27_ENABLE_SHIFT))

#define  XENA_XIIC27_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC27_SAFETYEN_SHIFT 29
#define  XENA_XIIC27_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_SAFETYEN_MASK) >> XENA_XIIC27_SAFETYEN_SHIFT)
#define  XENA_XIIC27_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC27_SAFETYEN_SHIFT))

#define  XENA_XIIC27_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC27_RESERVED1_SHIFT 24
#define  XENA_XIIC27_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_RESERVED1_MASK) >> XENA_XIIC27_RESERVED1_SHIFT)
#define  XENA_XIIC27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC27_RESERVED1_SHIFT))

#define  XENA_XIIC27_DELAY_MASK 0xff0000
#define  XENA_XIIC27_DELAY_SHIFT 16
#define  XENA_XIIC27_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_DELAY_MASK) >> XENA_XIIC27_DELAY_SHIFT)
#define  XENA_XIIC27_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC27_DELAY_SHIFT))

#define  XENA_XIIC27_RESERVED2_MASK 0xfe00
#define  XENA_XIIC27_RESERVED2_SHIFT 9
#define  XENA_XIIC27_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_RESERVED2_MASK) >> XENA_XIIC27_RESERVED2_SHIFT)
#define  XENA_XIIC27_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC27_RESERVED2_SHIFT))

#define  XENA_XIIC27_BITSELECT_MASK 0x1f0
#define  XENA_XIIC27_BITSELECT_SHIFT 4
#define  XENA_XIIC27_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_BITSELECT_MASK) >> XENA_XIIC27_BITSELECT_SHIFT)
#define  XENA_XIIC27_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC27_BITSELECT_SHIFT))

#define  XENA_XIIC27_SELECT_MASK 0xf
#define  XENA_XIIC27_SELECT_SHIFT 0
#define  XENA_XIIC27_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC27_SELECT_MASK) >> XENA_XIIC27_SELECT_SHIFT)
#define  XENA_XIIC27_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC27_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC27_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 29 of 52) (XIIC28)
/** \brief UPCIO IO Configuration Register (Instance 29 of 52)*/
//====================================================================

#define  XENA_XIIC28_INVERT_MASK 0x80000000
#define  XENA_XIIC28_INVERT_SHIFT 31
#define  XENA_XIIC28_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_INVERT_MASK) >> XENA_XIIC28_INVERT_SHIFT)
#define  XENA_XIIC28_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC28_INVERT_SHIFT))

#define  XENA_XIIC28_ENABLE_MASK 0x40000000
#define  XENA_XIIC28_ENABLE_SHIFT 30
#define  XENA_XIIC28_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_ENABLE_MASK) >> XENA_XIIC28_ENABLE_SHIFT)
#define  XENA_XIIC28_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC28_ENABLE_SHIFT))

#define  XENA_XIIC28_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC28_SAFETYEN_SHIFT 29
#define  XENA_XIIC28_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_SAFETYEN_MASK) >> XENA_XIIC28_SAFETYEN_SHIFT)
#define  XENA_XIIC28_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC28_SAFETYEN_SHIFT))

#define  XENA_XIIC28_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC28_RESERVED1_SHIFT 24
#define  XENA_XIIC28_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_RESERVED1_MASK) >> XENA_XIIC28_RESERVED1_SHIFT)
#define  XENA_XIIC28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC28_RESERVED1_SHIFT))

#define  XENA_XIIC28_DELAY_MASK 0xff0000
#define  XENA_XIIC28_DELAY_SHIFT 16
#define  XENA_XIIC28_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_DELAY_MASK) >> XENA_XIIC28_DELAY_SHIFT)
#define  XENA_XIIC28_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC28_DELAY_SHIFT))

#define  XENA_XIIC28_RESERVED2_MASK 0xfe00
#define  XENA_XIIC28_RESERVED2_SHIFT 9
#define  XENA_XIIC28_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_RESERVED2_MASK) >> XENA_XIIC28_RESERVED2_SHIFT)
#define  XENA_XIIC28_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC28_RESERVED2_SHIFT))

#define  XENA_XIIC28_BITSELECT_MASK 0x1f0
#define  XENA_XIIC28_BITSELECT_SHIFT 4
#define  XENA_XIIC28_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_BITSELECT_MASK) >> XENA_XIIC28_BITSELECT_SHIFT)
#define  XENA_XIIC28_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC28_BITSELECT_SHIFT))

#define  XENA_XIIC28_SELECT_MASK 0xf
#define  XENA_XIIC28_SELECT_SHIFT 0
#define  XENA_XIIC28_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC28_SELECT_MASK) >> XENA_XIIC28_SELECT_SHIFT)
#define  XENA_XIIC28_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC28_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC28_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 30 of 52) (XIIC29)
/** \brief UPCIO IO Configuration Register (Instance 30 of 52)*/
//====================================================================

#define  XENA_XIIC29_INVERT_MASK 0x80000000
#define  XENA_XIIC29_INVERT_SHIFT 31
#define  XENA_XIIC29_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_INVERT_MASK) >> XENA_XIIC29_INVERT_SHIFT)
#define  XENA_XIIC29_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC29_INVERT_SHIFT))

#define  XENA_XIIC29_ENABLE_MASK 0x40000000
#define  XENA_XIIC29_ENABLE_SHIFT 30
#define  XENA_XIIC29_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_ENABLE_MASK) >> XENA_XIIC29_ENABLE_SHIFT)
#define  XENA_XIIC29_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC29_ENABLE_SHIFT))

#define  XENA_XIIC29_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC29_SAFETYEN_SHIFT 29
#define  XENA_XIIC29_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_SAFETYEN_MASK) >> XENA_XIIC29_SAFETYEN_SHIFT)
#define  XENA_XIIC29_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC29_SAFETYEN_SHIFT))

#define  XENA_XIIC29_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC29_RESERVED1_SHIFT 24
#define  XENA_XIIC29_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_RESERVED1_MASK) >> XENA_XIIC29_RESERVED1_SHIFT)
#define  XENA_XIIC29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC29_RESERVED1_SHIFT))

#define  XENA_XIIC29_DELAY_MASK 0xff0000
#define  XENA_XIIC29_DELAY_SHIFT 16
#define  XENA_XIIC29_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_DELAY_MASK) >> XENA_XIIC29_DELAY_SHIFT)
#define  XENA_XIIC29_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC29_DELAY_SHIFT))

#define  XENA_XIIC29_RESERVED2_MASK 0xfe00
#define  XENA_XIIC29_RESERVED2_SHIFT 9
#define  XENA_XIIC29_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_RESERVED2_MASK) >> XENA_XIIC29_RESERVED2_SHIFT)
#define  XENA_XIIC29_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC29_RESERVED2_SHIFT))

#define  XENA_XIIC29_BITSELECT_MASK 0x1f0
#define  XENA_XIIC29_BITSELECT_SHIFT 4
#define  XENA_XIIC29_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_BITSELECT_MASK) >> XENA_XIIC29_BITSELECT_SHIFT)
#define  XENA_XIIC29_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC29_BITSELECT_SHIFT))

#define  XENA_XIIC29_SELECT_MASK 0xf
#define  XENA_XIIC29_SELECT_SHIFT 0
#define  XENA_XIIC29_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC29_SELECT_MASK) >> XENA_XIIC29_SELECT_SHIFT)
#define  XENA_XIIC29_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC29_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC29_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 31 of 52) (XIIC30)
/** \brief UPCIO IO Configuration Register (Instance 31 of 52)*/
//====================================================================

#define  XENA_XIIC30_INVERT_MASK 0x80000000
#define  XENA_XIIC30_INVERT_SHIFT 31
#define  XENA_XIIC30_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_INVERT_MASK) >> XENA_XIIC30_INVERT_SHIFT)
#define  XENA_XIIC30_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC30_INVERT_SHIFT))

#define  XENA_XIIC30_ENABLE_MASK 0x40000000
#define  XENA_XIIC30_ENABLE_SHIFT 30
#define  XENA_XIIC30_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_ENABLE_MASK) >> XENA_XIIC30_ENABLE_SHIFT)
#define  XENA_XIIC30_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC30_ENABLE_SHIFT))

#define  XENA_XIIC30_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC30_SAFETYEN_SHIFT 29
#define  XENA_XIIC30_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_SAFETYEN_MASK) >> XENA_XIIC30_SAFETYEN_SHIFT)
#define  XENA_XIIC30_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC30_SAFETYEN_SHIFT))

#define  XENA_XIIC30_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC30_RESERVED1_SHIFT 24
#define  XENA_XIIC30_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_RESERVED1_MASK) >> XENA_XIIC30_RESERVED1_SHIFT)
#define  XENA_XIIC30_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC30_RESERVED1_SHIFT))

#define  XENA_XIIC30_DELAY_MASK 0xff0000
#define  XENA_XIIC30_DELAY_SHIFT 16
#define  XENA_XIIC30_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_DELAY_MASK) >> XENA_XIIC30_DELAY_SHIFT)
#define  XENA_XIIC30_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC30_DELAY_SHIFT))

#define  XENA_XIIC30_RESERVED2_MASK 0xfe00
#define  XENA_XIIC30_RESERVED2_SHIFT 9
#define  XENA_XIIC30_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_RESERVED2_MASK) >> XENA_XIIC30_RESERVED2_SHIFT)
#define  XENA_XIIC30_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC30_RESERVED2_SHIFT))

#define  XENA_XIIC30_BITSELECT_MASK 0x1f0
#define  XENA_XIIC30_BITSELECT_SHIFT 4
#define  XENA_XIIC30_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_BITSELECT_MASK) >> XENA_XIIC30_BITSELECT_SHIFT)
#define  XENA_XIIC30_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC30_BITSELECT_SHIFT))

#define  XENA_XIIC30_SELECT_MASK 0xf
#define  XENA_XIIC30_SELECT_SHIFT 0
#define  XENA_XIIC30_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC30_SELECT_MASK) >> XENA_XIIC30_SELECT_SHIFT)
#define  XENA_XIIC30_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC30_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC30_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 32 of 52) (XIIC31)
/** \brief UPCIO IO Configuration Register (Instance 32 of 52)*/
//====================================================================

#define  XENA_XIIC31_INVERT_MASK 0x80000000
#define  XENA_XIIC31_INVERT_SHIFT 31
#define  XENA_XIIC31_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_INVERT_MASK) >> XENA_XIIC31_INVERT_SHIFT)
#define  XENA_XIIC31_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC31_INVERT_SHIFT))

#define  XENA_XIIC31_ENABLE_MASK 0x40000000
#define  XENA_XIIC31_ENABLE_SHIFT 30
#define  XENA_XIIC31_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_ENABLE_MASK) >> XENA_XIIC31_ENABLE_SHIFT)
#define  XENA_XIIC31_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC31_ENABLE_SHIFT))

#define  XENA_XIIC31_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC31_SAFETYEN_SHIFT 29
#define  XENA_XIIC31_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_SAFETYEN_MASK) >> XENA_XIIC31_SAFETYEN_SHIFT)
#define  XENA_XIIC31_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC31_SAFETYEN_SHIFT))

#define  XENA_XIIC31_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC31_RESERVED1_SHIFT 24
#define  XENA_XIIC31_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_RESERVED1_MASK) >> XENA_XIIC31_RESERVED1_SHIFT)
#define  XENA_XIIC31_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC31_RESERVED1_SHIFT))

#define  XENA_XIIC31_DELAY_MASK 0xff0000
#define  XENA_XIIC31_DELAY_SHIFT 16
#define  XENA_XIIC31_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_DELAY_MASK) >> XENA_XIIC31_DELAY_SHIFT)
#define  XENA_XIIC31_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC31_DELAY_SHIFT))

#define  XENA_XIIC31_RESERVED2_MASK 0xfe00
#define  XENA_XIIC31_RESERVED2_SHIFT 9
#define  XENA_XIIC31_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_RESERVED2_MASK) >> XENA_XIIC31_RESERVED2_SHIFT)
#define  XENA_XIIC31_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC31_RESERVED2_SHIFT))

#define  XENA_XIIC31_BITSELECT_MASK 0x1f0
#define  XENA_XIIC31_BITSELECT_SHIFT 4
#define  XENA_XIIC31_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_BITSELECT_MASK) >> XENA_XIIC31_BITSELECT_SHIFT)
#define  XENA_XIIC31_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC31_BITSELECT_SHIFT))

#define  XENA_XIIC31_SELECT_MASK 0xf
#define  XENA_XIIC31_SELECT_SHIFT 0
#define  XENA_XIIC31_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC31_SELECT_MASK) >> XENA_XIIC31_SELECT_SHIFT)
#define  XENA_XIIC31_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC31_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC31_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 33 of 52) (XIIC32)
/** \brief UPCIO IO Configuration Register (Instance 33 of 52)*/
//====================================================================

#define  XENA_XIIC32_INVERT_MASK 0x80000000
#define  XENA_XIIC32_INVERT_SHIFT 31
#define  XENA_XIIC32_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_INVERT_MASK) >> XENA_XIIC32_INVERT_SHIFT)
#define  XENA_XIIC32_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC32_INVERT_SHIFT))

#define  XENA_XIIC32_ENABLE_MASK 0x40000000
#define  XENA_XIIC32_ENABLE_SHIFT 30
#define  XENA_XIIC32_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_ENABLE_MASK) >> XENA_XIIC32_ENABLE_SHIFT)
#define  XENA_XIIC32_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC32_ENABLE_SHIFT))

#define  XENA_XIIC32_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC32_SAFETYEN_SHIFT 29
#define  XENA_XIIC32_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_SAFETYEN_MASK) >> XENA_XIIC32_SAFETYEN_SHIFT)
#define  XENA_XIIC32_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC32_SAFETYEN_SHIFT))

#define  XENA_XIIC32_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC32_RESERVED1_SHIFT 24
#define  XENA_XIIC32_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_RESERVED1_MASK) >> XENA_XIIC32_RESERVED1_SHIFT)
#define  XENA_XIIC32_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC32_RESERVED1_SHIFT))

#define  XENA_XIIC32_DELAY_MASK 0xff0000
#define  XENA_XIIC32_DELAY_SHIFT 16
#define  XENA_XIIC32_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_DELAY_MASK) >> XENA_XIIC32_DELAY_SHIFT)
#define  XENA_XIIC32_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC32_DELAY_SHIFT))

#define  XENA_XIIC32_RESERVED2_MASK 0xfe00
#define  XENA_XIIC32_RESERVED2_SHIFT 9
#define  XENA_XIIC32_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_RESERVED2_MASK) >> XENA_XIIC32_RESERVED2_SHIFT)
#define  XENA_XIIC32_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC32_RESERVED2_SHIFT))

#define  XENA_XIIC32_BITSELECT_MASK 0x1f0
#define  XENA_XIIC32_BITSELECT_SHIFT 4
#define  XENA_XIIC32_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_BITSELECT_MASK) >> XENA_XIIC32_BITSELECT_SHIFT)
#define  XENA_XIIC32_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC32_BITSELECT_SHIFT))

#define  XENA_XIIC32_SELECT_MASK 0xf
#define  XENA_XIIC32_SELECT_SHIFT 0
#define  XENA_XIIC32_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC32_SELECT_MASK) >> XENA_XIIC32_SELECT_SHIFT)
#define  XENA_XIIC32_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC32_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC32_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 34 of 52) (XIIC33)
/** \brief UPCIO IO Configuration Register (Instance 34 of 52)*/
//====================================================================

#define  XENA_XIIC33_INVERT_MASK 0x80000000
#define  XENA_XIIC33_INVERT_SHIFT 31
#define  XENA_XIIC33_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_INVERT_MASK) >> XENA_XIIC33_INVERT_SHIFT)
#define  XENA_XIIC33_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC33_INVERT_SHIFT))

#define  XENA_XIIC33_ENABLE_MASK 0x40000000
#define  XENA_XIIC33_ENABLE_SHIFT 30
#define  XENA_XIIC33_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_ENABLE_MASK) >> XENA_XIIC33_ENABLE_SHIFT)
#define  XENA_XIIC33_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC33_ENABLE_SHIFT))

#define  XENA_XIIC33_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC33_SAFETYEN_SHIFT 29
#define  XENA_XIIC33_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_SAFETYEN_MASK) >> XENA_XIIC33_SAFETYEN_SHIFT)
#define  XENA_XIIC33_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC33_SAFETYEN_SHIFT))

#define  XENA_XIIC33_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC33_RESERVED1_SHIFT 24
#define  XENA_XIIC33_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_RESERVED1_MASK) >> XENA_XIIC33_RESERVED1_SHIFT)
#define  XENA_XIIC33_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC33_RESERVED1_SHIFT))

#define  XENA_XIIC33_DELAY_MASK 0xff0000
#define  XENA_XIIC33_DELAY_SHIFT 16
#define  XENA_XIIC33_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_DELAY_MASK) >> XENA_XIIC33_DELAY_SHIFT)
#define  XENA_XIIC33_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC33_DELAY_SHIFT))

#define  XENA_XIIC33_RESERVED2_MASK 0xfe00
#define  XENA_XIIC33_RESERVED2_SHIFT 9
#define  XENA_XIIC33_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_RESERVED2_MASK) >> XENA_XIIC33_RESERVED2_SHIFT)
#define  XENA_XIIC33_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC33_RESERVED2_SHIFT))

#define  XENA_XIIC33_BITSELECT_MASK 0x1f0
#define  XENA_XIIC33_BITSELECT_SHIFT 4
#define  XENA_XIIC33_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_BITSELECT_MASK) >> XENA_XIIC33_BITSELECT_SHIFT)
#define  XENA_XIIC33_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC33_BITSELECT_SHIFT))

#define  XENA_XIIC33_SELECT_MASK 0xf
#define  XENA_XIIC33_SELECT_SHIFT 0
#define  XENA_XIIC33_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC33_SELECT_MASK) >> XENA_XIIC33_SELECT_SHIFT)
#define  XENA_XIIC33_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC33_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC33_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 35 of 52) (XIIC34)
/** \brief UPCIO IO Configuration Register (Instance 35 of 52)*/
//====================================================================

#define  XENA_XIIC34_INVERT_MASK 0x80000000
#define  XENA_XIIC34_INVERT_SHIFT 31
#define  XENA_XIIC34_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_INVERT_MASK) >> XENA_XIIC34_INVERT_SHIFT)
#define  XENA_XIIC34_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC34_INVERT_SHIFT))

#define  XENA_XIIC34_ENABLE_MASK 0x40000000
#define  XENA_XIIC34_ENABLE_SHIFT 30
#define  XENA_XIIC34_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_ENABLE_MASK) >> XENA_XIIC34_ENABLE_SHIFT)
#define  XENA_XIIC34_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC34_ENABLE_SHIFT))

#define  XENA_XIIC34_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC34_SAFETYEN_SHIFT 29
#define  XENA_XIIC34_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_SAFETYEN_MASK) >> XENA_XIIC34_SAFETYEN_SHIFT)
#define  XENA_XIIC34_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC34_SAFETYEN_SHIFT))

#define  XENA_XIIC34_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC34_RESERVED1_SHIFT 24
#define  XENA_XIIC34_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_RESERVED1_MASK) >> XENA_XIIC34_RESERVED1_SHIFT)
#define  XENA_XIIC34_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC34_RESERVED1_SHIFT))

#define  XENA_XIIC34_DELAY_MASK 0xff0000
#define  XENA_XIIC34_DELAY_SHIFT 16
#define  XENA_XIIC34_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_DELAY_MASK) >> XENA_XIIC34_DELAY_SHIFT)
#define  XENA_XIIC34_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC34_DELAY_SHIFT))

#define  XENA_XIIC34_RESERVED2_MASK 0xfe00
#define  XENA_XIIC34_RESERVED2_SHIFT 9
#define  XENA_XIIC34_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_RESERVED2_MASK) >> XENA_XIIC34_RESERVED2_SHIFT)
#define  XENA_XIIC34_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC34_RESERVED2_SHIFT))

#define  XENA_XIIC34_BITSELECT_MASK 0x1f0
#define  XENA_XIIC34_BITSELECT_SHIFT 4
#define  XENA_XIIC34_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_BITSELECT_MASK) >> XENA_XIIC34_BITSELECT_SHIFT)
#define  XENA_XIIC34_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC34_BITSELECT_SHIFT))

#define  XENA_XIIC34_SELECT_MASK 0xf
#define  XENA_XIIC34_SELECT_SHIFT 0
#define  XENA_XIIC34_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC34_SELECT_MASK) >> XENA_XIIC34_SELECT_SHIFT)
#define  XENA_XIIC34_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC34_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC34_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 36 of 52) (XIIC35)
/** \brief UPCIO IO Configuration Register (Instance 36 of 52)*/
//====================================================================

#define  XENA_XIIC35_INVERT_MASK 0x80000000
#define  XENA_XIIC35_INVERT_SHIFT 31
#define  XENA_XIIC35_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_INVERT_MASK) >> XENA_XIIC35_INVERT_SHIFT)
#define  XENA_XIIC35_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC35_INVERT_SHIFT))

#define  XENA_XIIC35_ENABLE_MASK 0x40000000
#define  XENA_XIIC35_ENABLE_SHIFT 30
#define  XENA_XIIC35_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_ENABLE_MASK) >> XENA_XIIC35_ENABLE_SHIFT)
#define  XENA_XIIC35_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC35_ENABLE_SHIFT))

#define  XENA_XIIC35_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC35_SAFETYEN_SHIFT 29
#define  XENA_XIIC35_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_SAFETYEN_MASK) >> XENA_XIIC35_SAFETYEN_SHIFT)
#define  XENA_XIIC35_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC35_SAFETYEN_SHIFT))

#define  XENA_XIIC35_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC35_RESERVED1_SHIFT 24
#define  XENA_XIIC35_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_RESERVED1_MASK) >> XENA_XIIC35_RESERVED1_SHIFT)
#define  XENA_XIIC35_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC35_RESERVED1_SHIFT))

#define  XENA_XIIC35_DELAY_MASK 0xff0000
#define  XENA_XIIC35_DELAY_SHIFT 16
#define  XENA_XIIC35_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_DELAY_MASK) >> XENA_XIIC35_DELAY_SHIFT)
#define  XENA_XIIC35_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC35_DELAY_SHIFT))

#define  XENA_XIIC35_RESERVED2_MASK 0xfe00
#define  XENA_XIIC35_RESERVED2_SHIFT 9
#define  XENA_XIIC35_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_RESERVED2_MASK) >> XENA_XIIC35_RESERVED2_SHIFT)
#define  XENA_XIIC35_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC35_RESERVED2_SHIFT))

#define  XENA_XIIC35_BITSELECT_MASK 0x1f0
#define  XENA_XIIC35_BITSELECT_SHIFT 4
#define  XENA_XIIC35_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_BITSELECT_MASK) >> XENA_XIIC35_BITSELECT_SHIFT)
#define  XENA_XIIC35_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC35_BITSELECT_SHIFT))

#define  XENA_XIIC35_SELECT_MASK 0xf
#define  XENA_XIIC35_SELECT_SHIFT 0
#define  XENA_XIIC35_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC35_SELECT_MASK) >> XENA_XIIC35_SELECT_SHIFT)
#define  XENA_XIIC35_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC35_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC35_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 37 of 52) (XIIC36)
/** \brief UPCIO IO Configuration Register (Instance 37 of 52)*/
//====================================================================

#define  XENA_XIIC36_INVERT_MASK 0x80000000
#define  XENA_XIIC36_INVERT_SHIFT 31
#define  XENA_XIIC36_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_INVERT_MASK) >> XENA_XIIC36_INVERT_SHIFT)
#define  XENA_XIIC36_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC36_INVERT_SHIFT))

#define  XENA_XIIC36_ENABLE_MASK 0x40000000
#define  XENA_XIIC36_ENABLE_SHIFT 30
#define  XENA_XIIC36_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_ENABLE_MASK) >> XENA_XIIC36_ENABLE_SHIFT)
#define  XENA_XIIC36_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC36_ENABLE_SHIFT))

#define  XENA_XIIC36_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC36_SAFETYEN_SHIFT 29
#define  XENA_XIIC36_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_SAFETYEN_MASK) >> XENA_XIIC36_SAFETYEN_SHIFT)
#define  XENA_XIIC36_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC36_SAFETYEN_SHIFT))

#define  XENA_XIIC36_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC36_RESERVED1_SHIFT 24
#define  XENA_XIIC36_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_RESERVED1_MASK) >> XENA_XIIC36_RESERVED1_SHIFT)
#define  XENA_XIIC36_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC36_RESERVED1_SHIFT))

#define  XENA_XIIC36_DELAY_MASK 0xff0000
#define  XENA_XIIC36_DELAY_SHIFT 16
#define  XENA_XIIC36_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_DELAY_MASK) >> XENA_XIIC36_DELAY_SHIFT)
#define  XENA_XIIC36_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC36_DELAY_SHIFT))

#define  XENA_XIIC36_RESERVED2_MASK 0xfe00
#define  XENA_XIIC36_RESERVED2_SHIFT 9
#define  XENA_XIIC36_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_RESERVED2_MASK) >> XENA_XIIC36_RESERVED2_SHIFT)
#define  XENA_XIIC36_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC36_RESERVED2_SHIFT))

#define  XENA_XIIC36_BITSELECT_MASK 0x1f0
#define  XENA_XIIC36_BITSELECT_SHIFT 4
#define  XENA_XIIC36_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_BITSELECT_MASK) >> XENA_XIIC36_BITSELECT_SHIFT)
#define  XENA_XIIC36_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC36_BITSELECT_SHIFT))

#define  XENA_XIIC36_SELECT_MASK 0xf
#define  XENA_XIIC36_SELECT_SHIFT 0
#define  XENA_XIIC36_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC36_SELECT_MASK) >> XENA_XIIC36_SELECT_SHIFT)
#define  XENA_XIIC36_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC36_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC36_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 38 of 52) (XIIC37)
/** \brief UPCIO IO Configuration Register (Instance 38 of 52)*/
//====================================================================

#define  XENA_XIIC37_INVERT_MASK 0x80000000
#define  XENA_XIIC37_INVERT_SHIFT 31
#define  XENA_XIIC37_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_INVERT_MASK) >> XENA_XIIC37_INVERT_SHIFT)
#define  XENA_XIIC37_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC37_INVERT_SHIFT))

#define  XENA_XIIC37_ENABLE_MASK 0x40000000
#define  XENA_XIIC37_ENABLE_SHIFT 30
#define  XENA_XIIC37_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_ENABLE_MASK) >> XENA_XIIC37_ENABLE_SHIFT)
#define  XENA_XIIC37_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC37_ENABLE_SHIFT))

#define  XENA_XIIC37_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC37_SAFETYEN_SHIFT 29
#define  XENA_XIIC37_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_SAFETYEN_MASK) >> XENA_XIIC37_SAFETYEN_SHIFT)
#define  XENA_XIIC37_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC37_SAFETYEN_SHIFT))

#define  XENA_XIIC37_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC37_RESERVED1_SHIFT 24
#define  XENA_XIIC37_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_RESERVED1_MASK) >> XENA_XIIC37_RESERVED1_SHIFT)
#define  XENA_XIIC37_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC37_RESERVED1_SHIFT))

#define  XENA_XIIC37_DELAY_MASK 0xff0000
#define  XENA_XIIC37_DELAY_SHIFT 16
#define  XENA_XIIC37_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_DELAY_MASK) >> XENA_XIIC37_DELAY_SHIFT)
#define  XENA_XIIC37_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC37_DELAY_SHIFT))

#define  XENA_XIIC37_RESERVED2_MASK 0xfe00
#define  XENA_XIIC37_RESERVED2_SHIFT 9
#define  XENA_XIIC37_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_RESERVED2_MASK) >> XENA_XIIC37_RESERVED2_SHIFT)
#define  XENA_XIIC37_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC37_RESERVED2_SHIFT))

#define  XENA_XIIC37_BITSELECT_MASK 0x1f0
#define  XENA_XIIC37_BITSELECT_SHIFT 4
#define  XENA_XIIC37_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_BITSELECT_MASK) >> XENA_XIIC37_BITSELECT_SHIFT)
#define  XENA_XIIC37_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC37_BITSELECT_SHIFT))

#define  XENA_XIIC37_SELECT_MASK 0xf
#define  XENA_XIIC37_SELECT_SHIFT 0
#define  XENA_XIIC37_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC37_SELECT_MASK) >> XENA_XIIC37_SELECT_SHIFT)
#define  XENA_XIIC37_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC37_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC37_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 39 of 52) (XIIC38)
/** \brief UPCIO IO Configuration Register (Instance 39 of 52)*/
//====================================================================

#define  XENA_XIIC38_INVERT_MASK 0x80000000
#define  XENA_XIIC38_INVERT_SHIFT 31
#define  XENA_XIIC38_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_INVERT_MASK) >> XENA_XIIC38_INVERT_SHIFT)
#define  XENA_XIIC38_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC38_INVERT_SHIFT))

#define  XENA_XIIC38_ENABLE_MASK 0x40000000
#define  XENA_XIIC38_ENABLE_SHIFT 30
#define  XENA_XIIC38_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_ENABLE_MASK) >> XENA_XIIC38_ENABLE_SHIFT)
#define  XENA_XIIC38_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC38_ENABLE_SHIFT))

#define  XENA_XIIC38_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC38_SAFETYEN_SHIFT 29
#define  XENA_XIIC38_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_SAFETYEN_MASK) >> XENA_XIIC38_SAFETYEN_SHIFT)
#define  XENA_XIIC38_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC38_SAFETYEN_SHIFT))

#define  XENA_XIIC38_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC38_RESERVED1_SHIFT 24
#define  XENA_XIIC38_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_RESERVED1_MASK) >> XENA_XIIC38_RESERVED1_SHIFT)
#define  XENA_XIIC38_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC38_RESERVED1_SHIFT))

#define  XENA_XIIC38_DELAY_MASK 0xff0000
#define  XENA_XIIC38_DELAY_SHIFT 16
#define  XENA_XIIC38_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_DELAY_MASK) >> XENA_XIIC38_DELAY_SHIFT)
#define  XENA_XIIC38_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC38_DELAY_SHIFT))

#define  XENA_XIIC38_RESERVED2_MASK 0xfe00
#define  XENA_XIIC38_RESERVED2_SHIFT 9
#define  XENA_XIIC38_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_RESERVED2_MASK) >> XENA_XIIC38_RESERVED2_SHIFT)
#define  XENA_XIIC38_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC38_RESERVED2_SHIFT))

#define  XENA_XIIC38_BITSELECT_MASK 0x1f0
#define  XENA_XIIC38_BITSELECT_SHIFT 4
#define  XENA_XIIC38_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_BITSELECT_MASK) >> XENA_XIIC38_BITSELECT_SHIFT)
#define  XENA_XIIC38_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC38_BITSELECT_SHIFT))

#define  XENA_XIIC38_SELECT_MASK 0xf
#define  XENA_XIIC38_SELECT_SHIFT 0
#define  XENA_XIIC38_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC38_SELECT_MASK) >> XENA_XIIC38_SELECT_SHIFT)
#define  XENA_XIIC38_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC38_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC38_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 40 of 52) (XIIC39)
/** \brief UPCIO IO Configuration Register (Instance 40 of 52)*/
//====================================================================

#define  XENA_XIIC39_INVERT_MASK 0x80000000
#define  XENA_XIIC39_INVERT_SHIFT 31
#define  XENA_XIIC39_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_INVERT_MASK) >> XENA_XIIC39_INVERT_SHIFT)
#define  XENA_XIIC39_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC39_INVERT_SHIFT))

#define  XENA_XIIC39_ENABLE_MASK 0x40000000
#define  XENA_XIIC39_ENABLE_SHIFT 30
#define  XENA_XIIC39_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_ENABLE_MASK) >> XENA_XIIC39_ENABLE_SHIFT)
#define  XENA_XIIC39_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC39_ENABLE_SHIFT))

#define  XENA_XIIC39_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC39_SAFETYEN_SHIFT 29
#define  XENA_XIIC39_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_SAFETYEN_MASK) >> XENA_XIIC39_SAFETYEN_SHIFT)
#define  XENA_XIIC39_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC39_SAFETYEN_SHIFT))

#define  XENA_XIIC39_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC39_RESERVED1_SHIFT 24
#define  XENA_XIIC39_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_RESERVED1_MASK) >> XENA_XIIC39_RESERVED1_SHIFT)
#define  XENA_XIIC39_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC39_RESERVED1_SHIFT))

#define  XENA_XIIC39_DELAY_MASK 0xff0000
#define  XENA_XIIC39_DELAY_SHIFT 16
#define  XENA_XIIC39_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_DELAY_MASK) >> XENA_XIIC39_DELAY_SHIFT)
#define  XENA_XIIC39_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC39_DELAY_SHIFT))

#define  XENA_XIIC39_RESERVED2_MASK 0xfe00
#define  XENA_XIIC39_RESERVED2_SHIFT 9
#define  XENA_XIIC39_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_RESERVED2_MASK) >> XENA_XIIC39_RESERVED2_SHIFT)
#define  XENA_XIIC39_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC39_RESERVED2_SHIFT))

#define  XENA_XIIC39_BITSELECT_MASK 0x1f0
#define  XENA_XIIC39_BITSELECT_SHIFT 4
#define  XENA_XIIC39_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_BITSELECT_MASK) >> XENA_XIIC39_BITSELECT_SHIFT)
#define  XENA_XIIC39_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC39_BITSELECT_SHIFT))

#define  XENA_XIIC39_SELECT_MASK 0xf
#define  XENA_XIIC39_SELECT_SHIFT 0
#define  XENA_XIIC39_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC39_SELECT_MASK) >> XENA_XIIC39_SELECT_SHIFT)
#define  XENA_XIIC39_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC39_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC39_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 41 of 52) (XIIC40)
/** \brief UPCIO IO Configuration Register (Instance 41 of 52)*/
//====================================================================

#define  XENA_XIIC40_INVERT_MASK 0x80000000
#define  XENA_XIIC40_INVERT_SHIFT 31
#define  XENA_XIIC40_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_INVERT_MASK) >> XENA_XIIC40_INVERT_SHIFT)
#define  XENA_XIIC40_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC40_INVERT_SHIFT))

#define  XENA_XIIC40_ENABLE_MASK 0x40000000
#define  XENA_XIIC40_ENABLE_SHIFT 30
#define  XENA_XIIC40_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_ENABLE_MASK) >> XENA_XIIC40_ENABLE_SHIFT)
#define  XENA_XIIC40_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC40_ENABLE_SHIFT))

#define  XENA_XIIC40_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC40_SAFETYEN_SHIFT 29
#define  XENA_XIIC40_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_SAFETYEN_MASK) >> XENA_XIIC40_SAFETYEN_SHIFT)
#define  XENA_XIIC40_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC40_SAFETYEN_SHIFT))

#define  XENA_XIIC40_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC40_RESERVED1_SHIFT 24
#define  XENA_XIIC40_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_RESERVED1_MASK) >> XENA_XIIC40_RESERVED1_SHIFT)
#define  XENA_XIIC40_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC40_RESERVED1_SHIFT))

#define  XENA_XIIC40_DELAY_MASK 0xff0000
#define  XENA_XIIC40_DELAY_SHIFT 16
#define  XENA_XIIC40_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_DELAY_MASK) >> XENA_XIIC40_DELAY_SHIFT)
#define  XENA_XIIC40_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC40_DELAY_SHIFT))

#define  XENA_XIIC40_RESERVED2_MASK 0xfe00
#define  XENA_XIIC40_RESERVED2_SHIFT 9
#define  XENA_XIIC40_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_RESERVED2_MASK) >> XENA_XIIC40_RESERVED2_SHIFT)
#define  XENA_XIIC40_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC40_RESERVED2_SHIFT))

#define  XENA_XIIC40_BITSELECT_MASK 0x1f0
#define  XENA_XIIC40_BITSELECT_SHIFT 4
#define  XENA_XIIC40_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_BITSELECT_MASK) >> XENA_XIIC40_BITSELECT_SHIFT)
#define  XENA_XIIC40_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC40_BITSELECT_SHIFT))

#define  XENA_XIIC40_SELECT_MASK 0xf
#define  XENA_XIIC40_SELECT_SHIFT 0
#define  XENA_XIIC40_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC40_SELECT_MASK) >> XENA_XIIC40_SELECT_SHIFT)
#define  XENA_XIIC40_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC40_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC40_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 42 of 52) (XIIC41)
/** \brief UPCIO IO Configuration Register (Instance 42 of 52)*/
//====================================================================

#define  XENA_XIIC41_INVERT_MASK 0x80000000
#define  XENA_XIIC41_INVERT_SHIFT 31
#define  XENA_XIIC41_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_INVERT_MASK) >> XENA_XIIC41_INVERT_SHIFT)
#define  XENA_XIIC41_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC41_INVERT_SHIFT))

#define  XENA_XIIC41_ENABLE_MASK 0x40000000
#define  XENA_XIIC41_ENABLE_SHIFT 30
#define  XENA_XIIC41_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_ENABLE_MASK) >> XENA_XIIC41_ENABLE_SHIFT)
#define  XENA_XIIC41_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC41_ENABLE_SHIFT))

#define  XENA_XIIC41_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC41_SAFETYEN_SHIFT 29
#define  XENA_XIIC41_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_SAFETYEN_MASK) >> XENA_XIIC41_SAFETYEN_SHIFT)
#define  XENA_XIIC41_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC41_SAFETYEN_SHIFT))

#define  XENA_XIIC41_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC41_RESERVED1_SHIFT 24
#define  XENA_XIIC41_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_RESERVED1_MASK) >> XENA_XIIC41_RESERVED1_SHIFT)
#define  XENA_XIIC41_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC41_RESERVED1_SHIFT))

#define  XENA_XIIC41_DELAY_MASK 0xff0000
#define  XENA_XIIC41_DELAY_SHIFT 16
#define  XENA_XIIC41_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_DELAY_MASK) >> XENA_XIIC41_DELAY_SHIFT)
#define  XENA_XIIC41_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC41_DELAY_SHIFT))

#define  XENA_XIIC41_RESERVED2_MASK 0xfe00
#define  XENA_XIIC41_RESERVED2_SHIFT 9
#define  XENA_XIIC41_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_RESERVED2_MASK) >> XENA_XIIC41_RESERVED2_SHIFT)
#define  XENA_XIIC41_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC41_RESERVED2_SHIFT))

#define  XENA_XIIC41_BITSELECT_MASK 0x1f0
#define  XENA_XIIC41_BITSELECT_SHIFT 4
#define  XENA_XIIC41_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_BITSELECT_MASK) >> XENA_XIIC41_BITSELECT_SHIFT)
#define  XENA_XIIC41_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC41_BITSELECT_SHIFT))

#define  XENA_XIIC41_SELECT_MASK 0xf
#define  XENA_XIIC41_SELECT_SHIFT 0
#define  XENA_XIIC41_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC41_SELECT_MASK) >> XENA_XIIC41_SELECT_SHIFT)
#define  XENA_XIIC41_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC41_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC41_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 43 of 52) (XIIC42)
/** \brief UPCIO IO Configuration Register (Instance 43 of 52)*/
//====================================================================

#define  XENA_XIIC42_INVERT_MASK 0x80000000
#define  XENA_XIIC42_INVERT_SHIFT 31
#define  XENA_XIIC42_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_INVERT_MASK) >> XENA_XIIC42_INVERT_SHIFT)
#define  XENA_XIIC42_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC42_INVERT_SHIFT))

#define  XENA_XIIC42_ENABLE_MASK 0x40000000
#define  XENA_XIIC42_ENABLE_SHIFT 30
#define  XENA_XIIC42_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_ENABLE_MASK) >> XENA_XIIC42_ENABLE_SHIFT)
#define  XENA_XIIC42_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC42_ENABLE_SHIFT))

#define  XENA_XIIC42_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC42_SAFETYEN_SHIFT 29
#define  XENA_XIIC42_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_SAFETYEN_MASK) >> XENA_XIIC42_SAFETYEN_SHIFT)
#define  XENA_XIIC42_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC42_SAFETYEN_SHIFT))

#define  XENA_XIIC42_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC42_RESERVED1_SHIFT 24
#define  XENA_XIIC42_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_RESERVED1_MASK) >> XENA_XIIC42_RESERVED1_SHIFT)
#define  XENA_XIIC42_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC42_RESERVED1_SHIFT))

#define  XENA_XIIC42_DELAY_MASK 0xff0000
#define  XENA_XIIC42_DELAY_SHIFT 16
#define  XENA_XIIC42_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_DELAY_MASK) >> XENA_XIIC42_DELAY_SHIFT)
#define  XENA_XIIC42_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC42_DELAY_SHIFT))

#define  XENA_XIIC42_RESERVED2_MASK 0xfe00
#define  XENA_XIIC42_RESERVED2_SHIFT 9
#define  XENA_XIIC42_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_RESERVED2_MASK) >> XENA_XIIC42_RESERVED2_SHIFT)
#define  XENA_XIIC42_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC42_RESERVED2_SHIFT))

#define  XENA_XIIC42_BITSELECT_MASK 0x1f0
#define  XENA_XIIC42_BITSELECT_SHIFT 4
#define  XENA_XIIC42_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_BITSELECT_MASK) >> XENA_XIIC42_BITSELECT_SHIFT)
#define  XENA_XIIC42_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC42_BITSELECT_SHIFT))

#define  XENA_XIIC42_SELECT_MASK 0xf
#define  XENA_XIIC42_SELECT_SHIFT 0
#define  XENA_XIIC42_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC42_SELECT_MASK) >> XENA_XIIC42_SELECT_SHIFT)
#define  XENA_XIIC42_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC42_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC42_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 44 of 52) (XIIC43)
/** \brief UPCIO IO Configuration Register (Instance 44 of 52)*/
//====================================================================

#define  XENA_XIIC43_INVERT_MASK 0x80000000
#define  XENA_XIIC43_INVERT_SHIFT 31
#define  XENA_XIIC43_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_INVERT_MASK) >> XENA_XIIC43_INVERT_SHIFT)
#define  XENA_XIIC43_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC43_INVERT_SHIFT))

#define  XENA_XIIC43_ENABLE_MASK 0x40000000
#define  XENA_XIIC43_ENABLE_SHIFT 30
#define  XENA_XIIC43_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_ENABLE_MASK) >> XENA_XIIC43_ENABLE_SHIFT)
#define  XENA_XIIC43_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC43_ENABLE_SHIFT))

#define  XENA_XIIC43_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC43_SAFETYEN_SHIFT 29
#define  XENA_XIIC43_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_SAFETYEN_MASK) >> XENA_XIIC43_SAFETYEN_SHIFT)
#define  XENA_XIIC43_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC43_SAFETYEN_SHIFT))

#define  XENA_XIIC43_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC43_RESERVED1_SHIFT 24
#define  XENA_XIIC43_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_RESERVED1_MASK) >> XENA_XIIC43_RESERVED1_SHIFT)
#define  XENA_XIIC43_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC43_RESERVED1_SHIFT))

#define  XENA_XIIC43_DELAY_MASK 0xff0000
#define  XENA_XIIC43_DELAY_SHIFT 16
#define  XENA_XIIC43_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_DELAY_MASK) >> XENA_XIIC43_DELAY_SHIFT)
#define  XENA_XIIC43_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC43_DELAY_SHIFT))

#define  XENA_XIIC43_RESERVED2_MASK 0xfe00
#define  XENA_XIIC43_RESERVED2_SHIFT 9
#define  XENA_XIIC43_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_RESERVED2_MASK) >> XENA_XIIC43_RESERVED2_SHIFT)
#define  XENA_XIIC43_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC43_RESERVED2_SHIFT))

#define  XENA_XIIC43_BITSELECT_MASK 0x1f0
#define  XENA_XIIC43_BITSELECT_SHIFT 4
#define  XENA_XIIC43_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_BITSELECT_MASK) >> XENA_XIIC43_BITSELECT_SHIFT)
#define  XENA_XIIC43_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC43_BITSELECT_SHIFT))

#define  XENA_XIIC43_SELECT_MASK 0xf
#define  XENA_XIIC43_SELECT_SHIFT 0
#define  XENA_XIIC43_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC43_SELECT_MASK) >> XENA_XIIC43_SELECT_SHIFT)
#define  XENA_XIIC43_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC43_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC43_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 45 of 52) (XIIC44)
/** \brief UPCIO IO Configuration Register (Instance 45 of 52)*/
//====================================================================

#define  XENA_XIIC44_INVERT_MASK 0x80000000
#define  XENA_XIIC44_INVERT_SHIFT 31
#define  XENA_XIIC44_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_INVERT_MASK) >> XENA_XIIC44_INVERT_SHIFT)
#define  XENA_XIIC44_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC44_INVERT_SHIFT))

#define  XENA_XIIC44_ENABLE_MASK 0x40000000
#define  XENA_XIIC44_ENABLE_SHIFT 30
#define  XENA_XIIC44_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_ENABLE_MASK) >> XENA_XIIC44_ENABLE_SHIFT)
#define  XENA_XIIC44_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC44_ENABLE_SHIFT))

#define  XENA_XIIC44_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC44_SAFETYEN_SHIFT 29
#define  XENA_XIIC44_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_SAFETYEN_MASK) >> XENA_XIIC44_SAFETYEN_SHIFT)
#define  XENA_XIIC44_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC44_SAFETYEN_SHIFT))

#define  XENA_XIIC44_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC44_RESERVED1_SHIFT 24
#define  XENA_XIIC44_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_RESERVED1_MASK) >> XENA_XIIC44_RESERVED1_SHIFT)
#define  XENA_XIIC44_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC44_RESERVED1_SHIFT))

#define  XENA_XIIC44_DELAY_MASK 0xff0000
#define  XENA_XIIC44_DELAY_SHIFT 16
#define  XENA_XIIC44_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_DELAY_MASK) >> XENA_XIIC44_DELAY_SHIFT)
#define  XENA_XIIC44_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC44_DELAY_SHIFT))

#define  XENA_XIIC44_RESERVED2_MASK 0xfe00
#define  XENA_XIIC44_RESERVED2_SHIFT 9
#define  XENA_XIIC44_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_RESERVED2_MASK) >> XENA_XIIC44_RESERVED2_SHIFT)
#define  XENA_XIIC44_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC44_RESERVED2_SHIFT))

#define  XENA_XIIC44_BITSELECT_MASK 0x1f0
#define  XENA_XIIC44_BITSELECT_SHIFT 4
#define  XENA_XIIC44_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_BITSELECT_MASK) >> XENA_XIIC44_BITSELECT_SHIFT)
#define  XENA_XIIC44_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC44_BITSELECT_SHIFT))

#define  XENA_XIIC44_SELECT_MASK 0xf
#define  XENA_XIIC44_SELECT_SHIFT 0
#define  XENA_XIIC44_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC44_SELECT_MASK) >> XENA_XIIC44_SELECT_SHIFT)
#define  XENA_XIIC44_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC44_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC44_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 46 of 52) (XIIC45)
/** \brief UPCIO IO Configuration Register (Instance 46 of 52)*/
//====================================================================

#define  XENA_XIIC45_INVERT_MASK 0x80000000
#define  XENA_XIIC45_INVERT_SHIFT 31
#define  XENA_XIIC45_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_INVERT_MASK) >> XENA_XIIC45_INVERT_SHIFT)
#define  XENA_XIIC45_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC45_INVERT_SHIFT))

#define  XENA_XIIC45_ENABLE_MASK 0x40000000
#define  XENA_XIIC45_ENABLE_SHIFT 30
#define  XENA_XIIC45_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_ENABLE_MASK) >> XENA_XIIC45_ENABLE_SHIFT)
#define  XENA_XIIC45_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC45_ENABLE_SHIFT))

#define  XENA_XIIC45_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC45_SAFETYEN_SHIFT 29
#define  XENA_XIIC45_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_SAFETYEN_MASK) >> XENA_XIIC45_SAFETYEN_SHIFT)
#define  XENA_XIIC45_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC45_SAFETYEN_SHIFT))

#define  XENA_XIIC45_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC45_RESERVED1_SHIFT 24
#define  XENA_XIIC45_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_RESERVED1_MASK) >> XENA_XIIC45_RESERVED1_SHIFT)
#define  XENA_XIIC45_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC45_RESERVED1_SHIFT))

#define  XENA_XIIC45_DELAY_MASK 0xff0000
#define  XENA_XIIC45_DELAY_SHIFT 16
#define  XENA_XIIC45_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_DELAY_MASK) >> XENA_XIIC45_DELAY_SHIFT)
#define  XENA_XIIC45_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC45_DELAY_SHIFT))

#define  XENA_XIIC45_RESERVED2_MASK 0xfe00
#define  XENA_XIIC45_RESERVED2_SHIFT 9
#define  XENA_XIIC45_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_RESERVED2_MASK) >> XENA_XIIC45_RESERVED2_SHIFT)
#define  XENA_XIIC45_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC45_RESERVED2_SHIFT))

#define  XENA_XIIC45_BITSELECT_MASK 0x1f0
#define  XENA_XIIC45_BITSELECT_SHIFT 4
#define  XENA_XIIC45_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_BITSELECT_MASK) >> XENA_XIIC45_BITSELECT_SHIFT)
#define  XENA_XIIC45_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC45_BITSELECT_SHIFT))

#define  XENA_XIIC45_SELECT_MASK 0xf
#define  XENA_XIIC45_SELECT_SHIFT 0
#define  XENA_XIIC45_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC45_SELECT_MASK) >> XENA_XIIC45_SELECT_SHIFT)
#define  XENA_XIIC45_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC45_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC45_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 47 of 52) (XIIC46)
/** \brief UPCIO IO Configuration Register (Instance 47 of 52)*/
//====================================================================

#define  XENA_XIIC46_INVERT_MASK 0x80000000
#define  XENA_XIIC46_INVERT_SHIFT 31
#define  XENA_XIIC46_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_INVERT_MASK) >> XENA_XIIC46_INVERT_SHIFT)
#define  XENA_XIIC46_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC46_INVERT_SHIFT))

#define  XENA_XIIC46_ENABLE_MASK 0x40000000
#define  XENA_XIIC46_ENABLE_SHIFT 30
#define  XENA_XIIC46_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_ENABLE_MASK) >> XENA_XIIC46_ENABLE_SHIFT)
#define  XENA_XIIC46_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC46_ENABLE_SHIFT))

#define  XENA_XIIC46_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC46_SAFETYEN_SHIFT 29
#define  XENA_XIIC46_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_SAFETYEN_MASK) >> XENA_XIIC46_SAFETYEN_SHIFT)
#define  XENA_XIIC46_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC46_SAFETYEN_SHIFT))

#define  XENA_XIIC46_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC46_RESERVED1_SHIFT 24
#define  XENA_XIIC46_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_RESERVED1_MASK) >> XENA_XIIC46_RESERVED1_SHIFT)
#define  XENA_XIIC46_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC46_RESERVED1_SHIFT))

#define  XENA_XIIC46_DELAY_MASK 0xff0000
#define  XENA_XIIC46_DELAY_SHIFT 16
#define  XENA_XIIC46_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_DELAY_MASK) >> XENA_XIIC46_DELAY_SHIFT)
#define  XENA_XIIC46_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC46_DELAY_SHIFT))

#define  XENA_XIIC46_RESERVED2_MASK 0xfe00
#define  XENA_XIIC46_RESERVED2_SHIFT 9
#define  XENA_XIIC46_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_RESERVED2_MASK) >> XENA_XIIC46_RESERVED2_SHIFT)
#define  XENA_XIIC46_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC46_RESERVED2_SHIFT))

#define  XENA_XIIC46_BITSELECT_MASK 0x1f0
#define  XENA_XIIC46_BITSELECT_SHIFT 4
#define  XENA_XIIC46_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_BITSELECT_MASK) >> XENA_XIIC46_BITSELECT_SHIFT)
#define  XENA_XIIC46_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC46_BITSELECT_SHIFT))

#define  XENA_XIIC46_SELECT_MASK 0xf
#define  XENA_XIIC46_SELECT_SHIFT 0
#define  XENA_XIIC46_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC46_SELECT_MASK) >> XENA_XIIC46_SELECT_SHIFT)
#define  XENA_XIIC46_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC46_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC46_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 48 of 52) (XIIC47)
/** \brief UPCIO IO Configuration Register (Instance 48 of 52)*/
//====================================================================

#define  XENA_XIIC47_INVERT_MASK 0x80000000
#define  XENA_XIIC47_INVERT_SHIFT 31
#define  XENA_XIIC47_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_INVERT_MASK) >> XENA_XIIC47_INVERT_SHIFT)
#define  XENA_XIIC47_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC47_INVERT_SHIFT))

#define  XENA_XIIC47_ENABLE_MASK 0x40000000
#define  XENA_XIIC47_ENABLE_SHIFT 30
#define  XENA_XIIC47_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_ENABLE_MASK) >> XENA_XIIC47_ENABLE_SHIFT)
#define  XENA_XIIC47_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC47_ENABLE_SHIFT))

#define  XENA_XIIC47_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC47_SAFETYEN_SHIFT 29
#define  XENA_XIIC47_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_SAFETYEN_MASK) >> XENA_XIIC47_SAFETYEN_SHIFT)
#define  XENA_XIIC47_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC47_SAFETYEN_SHIFT))

#define  XENA_XIIC47_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC47_RESERVED1_SHIFT 24
#define  XENA_XIIC47_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_RESERVED1_MASK) >> XENA_XIIC47_RESERVED1_SHIFT)
#define  XENA_XIIC47_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC47_RESERVED1_SHIFT))

#define  XENA_XIIC47_DELAY_MASK 0xff0000
#define  XENA_XIIC47_DELAY_SHIFT 16
#define  XENA_XIIC47_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_DELAY_MASK) >> XENA_XIIC47_DELAY_SHIFT)
#define  XENA_XIIC47_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC47_DELAY_SHIFT))

#define  XENA_XIIC47_RESERVED2_MASK 0xfe00
#define  XENA_XIIC47_RESERVED2_SHIFT 9
#define  XENA_XIIC47_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_RESERVED2_MASK) >> XENA_XIIC47_RESERVED2_SHIFT)
#define  XENA_XIIC47_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC47_RESERVED2_SHIFT))

#define  XENA_XIIC47_BITSELECT_MASK 0x1f0
#define  XENA_XIIC47_BITSELECT_SHIFT 4
#define  XENA_XIIC47_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_BITSELECT_MASK) >> XENA_XIIC47_BITSELECT_SHIFT)
#define  XENA_XIIC47_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC47_BITSELECT_SHIFT))

#define  XENA_XIIC47_SELECT_MASK 0xf
#define  XENA_XIIC47_SELECT_SHIFT 0
#define  XENA_XIIC47_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC47_SELECT_MASK) >> XENA_XIIC47_SELECT_SHIFT)
#define  XENA_XIIC47_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC47_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC47_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 49 of 52) (XIIC48)
/** \brief UPCIO IO Configuration Register (Instance 49 of 52)*/
//====================================================================

#define  XENA_XIIC48_INVERT_MASK 0x80000000
#define  XENA_XIIC48_INVERT_SHIFT 31
#define  XENA_XIIC48_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_INVERT_MASK) >> XENA_XIIC48_INVERT_SHIFT)
#define  XENA_XIIC48_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC48_INVERT_SHIFT))

#define  XENA_XIIC48_ENABLE_MASK 0x40000000
#define  XENA_XIIC48_ENABLE_SHIFT 30
#define  XENA_XIIC48_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_ENABLE_MASK) >> XENA_XIIC48_ENABLE_SHIFT)
#define  XENA_XIIC48_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC48_ENABLE_SHIFT))

#define  XENA_XIIC48_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC48_SAFETYEN_SHIFT 29
#define  XENA_XIIC48_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_SAFETYEN_MASK) >> XENA_XIIC48_SAFETYEN_SHIFT)
#define  XENA_XIIC48_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC48_SAFETYEN_SHIFT))

#define  XENA_XIIC48_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC48_RESERVED1_SHIFT 24
#define  XENA_XIIC48_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_RESERVED1_MASK) >> XENA_XIIC48_RESERVED1_SHIFT)
#define  XENA_XIIC48_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC48_RESERVED1_SHIFT))

#define  XENA_XIIC48_DELAY_MASK 0xff0000
#define  XENA_XIIC48_DELAY_SHIFT 16
#define  XENA_XIIC48_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_DELAY_MASK) >> XENA_XIIC48_DELAY_SHIFT)
#define  XENA_XIIC48_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC48_DELAY_SHIFT))

#define  XENA_XIIC48_RESERVED2_MASK 0xfe00
#define  XENA_XIIC48_RESERVED2_SHIFT 9
#define  XENA_XIIC48_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_RESERVED2_MASK) >> XENA_XIIC48_RESERVED2_SHIFT)
#define  XENA_XIIC48_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC48_RESERVED2_SHIFT))

#define  XENA_XIIC48_BITSELECT_MASK 0x1f0
#define  XENA_XIIC48_BITSELECT_SHIFT 4
#define  XENA_XIIC48_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_BITSELECT_MASK) >> XENA_XIIC48_BITSELECT_SHIFT)
#define  XENA_XIIC48_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC48_BITSELECT_SHIFT))

#define  XENA_XIIC48_SELECT_MASK 0xf
#define  XENA_XIIC48_SELECT_SHIFT 0
#define  XENA_XIIC48_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC48_SELECT_MASK) >> XENA_XIIC48_SELECT_SHIFT)
#define  XENA_XIIC48_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC48_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC48_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 50 of 52) (XIIC49)
/** \brief UPCIO IO Configuration Register (Instance 50 of 52)*/
//====================================================================

#define  XENA_XIIC49_INVERT_MASK 0x80000000
#define  XENA_XIIC49_INVERT_SHIFT 31
#define  XENA_XIIC49_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_INVERT_MASK) >> XENA_XIIC49_INVERT_SHIFT)
#define  XENA_XIIC49_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC49_INVERT_SHIFT))

#define  XENA_XIIC49_ENABLE_MASK 0x40000000
#define  XENA_XIIC49_ENABLE_SHIFT 30
#define  XENA_XIIC49_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_ENABLE_MASK) >> XENA_XIIC49_ENABLE_SHIFT)
#define  XENA_XIIC49_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC49_ENABLE_SHIFT))

#define  XENA_XIIC49_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC49_SAFETYEN_SHIFT 29
#define  XENA_XIIC49_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_SAFETYEN_MASK) >> XENA_XIIC49_SAFETYEN_SHIFT)
#define  XENA_XIIC49_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC49_SAFETYEN_SHIFT))

#define  XENA_XIIC49_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC49_RESERVED1_SHIFT 24
#define  XENA_XIIC49_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_RESERVED1_MASK) >> XENA_XIIC49_RESERVED1_SHIFT)
#define  XENA_XIIC49_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC49_RESERVED1_SHIFT))

#define  XENA_XIIC49_DELAY_MASK 0xff0000
#define  XENA_XIIC49_DELAY_SHIFT 16
#define  XENA_XIIC49_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_DELAY_MASK) >> XENA_XIIC49_DELAY_SHIFT)
#define  XENA_XIIC49_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC49_DELAY_SHIFT))

#define  XENA_XIIC49_RESERVED2_MASK 0xfe00
#define  XENA_XIIC49_RESERVED2_SHIFT 9
#define  XENA_XIIC49_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_RESERVED2_MASK) >> XENA_XIIC49_RESERVED2_SHIFT)
#define  XENA_XIIC49_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC49_RESERVED2_SHIFT))

#define  XENA_XIIC49_BITSELECT_MASK 0x1f0
#define  XENA_XIIC49_BITSELECT_SHIFT 4
#define  XENA_XIIC49_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_BITSELECT_MASK) >> XENA_XIIC49_BITSELECT_SHIFT)
#define  XENA_XIIC49_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC49_BITSELECT_SHIFT))

#define  XENA_XIIC49_SELECT_MASK 0xf
#define  XENA_XIIC49_SELECT_SHIFT 0
#define  XENA_XIIC49_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC49_SELECT_MASK) >> XENA_XIIC49_SELECT_SHIFT)
#define  XENA_XIIC49_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC49_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC49_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 51 of 52) (XIIC50)
/** \brief UPCIO IO Configuration Register (Instance 51 of 52)*/
//====================================================================

#define  XENA_XIIC50_INVERT_MASK 0x80000000
#define  XENA_XIIC50_INVERT_SHIFT 31
#define  XENA_XIIC50_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_INVERT_MASK) >> XENA_XIIC50_INVERT_SHIFT)
#define  XENA_XIIC50_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC50_INVERT_SHIFT))

#define  XENA_XIIC50_ENABLE_MASK 0x40000000
#define  XENA_XIIC50_ENABLE_SHIFT 30
#define  XENA_XIIC50_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_ENABLE_MASK) >> XENA_XIIC50_ENABLE_SHIFT)
#define  XENA_XIIC50_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC50_ENABLE_SHIFT))

#define  XENA_XIIC50_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC50_SAFETYEN_SHIFT 29
#define  XENA_XIIC50_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_SAFETYEN_MASK) >> XENA_XIIC50_SAFETYEN_SHIFT)
#define  XENA_XIIC50_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC50_SAFETYEN_SHIFT))

#define  XENA_XIIC50_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC50_RESERVED1_SHIFT 24
#define  XENA_XIIC50_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_RESERVED1_MASK) >> XENA_XIIC50_RESERVED1_SHIFT)
#define  XENA_XIIC50_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC50_RESERVED1_SHIFT))

#define  XENA_XIIC50_DELAY_MASK 0xff0000
#define  XENA_XIIC50_DELAY_SHIFT 16
#define  XENA_XIIC50_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_DELAY_MASK) >> XENA_XIIC50_DELAY_SHIFT)
#define  XENA_XIIC50_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC50_DELAY_SHIFT))

#define  XENA_XIIC50_RESERVED2_MASK 0xfe00
#define  XENA_XIIC50_RESERVED2_SHIFT 9
#define  XENA_XIIC50_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_RESERVED2_MASK) >> XENA_XIIC50_RESERVED2_SHIFT)
#define  XENA_XIIC50_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC50_RESERVED2_SHIFT))

#define  XENA_XIIC50_BITSELECT_MASK 0x1f0
#define  XENA_XIIC50_BITSELECT_SHIFT 4
#define  XENA_XIIC50_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_BITSELECT_MASK) >> XENA_XIIC50_BITSELECT_SHIFT)
#define  XENA_XIIC50_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC50_BITSELECT_SHIFT))

#define  XENA_XIIC50_SELECT_MASK 0xf
#define  XENA_XIIC50_SELECT_SHIFT 0
#define  XENA_XIIC50_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC50_SELECT_MASK) >> XENA_XIIC50_SELECT_SHIFT)
#define  XENA_XIIC50_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC50_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC50_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO Configuration Register (Private) (Instance 52 of 52) (XIIC51)
/** \brief UPCIO IO Configuration Register (Instance 52 of 52)*/
//====================================================================

#define  XENA_XIIC51_INVERT_MASK 0x80000000
#define  XENA_XIIC51_INVERT_SHIFT 31
#define  XENA_XIIC51_INVERT_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_INVERT_MASK) >> XENA_XIIC51_INVERT_SHIFT)
#define  XENA_XIIC51_INVERT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_INVERT_MASK) | (((uint32_t)val) << XENA_XIIC51_INVERT_SHIFT))

#define  XENA_XIIC51_ENABLE_MASK 0x40000000
#define  XENA_XIIC51_ENABLE_SHIFT 30
#define  XENA_XIIC51_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_ENABLE_MASK) >> XENA_XIIC51_ENABLE_SHIFT)
#define  XENA_XIIC51_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_ENABLE_MASK) | (((uint32_t)val) << XENA_XIIC51_ENABLE_SHIFT))

#define  XENA_XIIC51_SAFETYEN_MASK 0x20000000
#define  XENA_XIIC51_SAFETYEN_SHIFT 29
#define  XENA_XIIC51_SAFETYEN_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_SAFETYEN_MASK) >> XENA_XIIC51_SAFETYEN_SHIFT)
#define  XENA_XIIC51_SAFETYEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_SAFETYEN_MASK) | (((uint32_t)val) << XENA_XIIC51_SAFETYEN_SHIFT))

#define  XENA_XIIC51_RESERVED1_MASK 0x1f000000
#define  XENA_XIIC51_RESERVED1_SHIFT 24
#define  XENA_XIIC51_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_RESERVED1_MASK) >> XENA_XIIC51_RESERVED1_SHIFT)
#define  XENA_XIIC51_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIIC51_RESERVED1_SHIFT))

#define  XENA_XIIC51_DELAY_MASK 0xff0000
#define  XENA_XIIC51_DELAY_SHIFT 16
#define  XENA_XIIC51_DELAY_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_DELAY_MASK) >> XENA_XIIC51_DELAY_SHIFT)
#define  XENA_XIIC51_DELAY_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_DELAY_MASK) | (((uint32_t)val) << XENA_XIIC51_DELAY_SHIFT))

#define  XENA_XIIC51_RESERVED2_MASK 0xfe00
#define  XENA_XIIC51_RESERVED2_SHIFT 9
#define  XENA_XIIC51_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_RESERVED2_MASK) >> XENA_XIIC51_RESERVED2_SHIFT)
#define  XENA_XIIC51_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIIC51_RESERVED2_SHIFT))

#define  XENA_XIIC51_BITSELECT_MASK 0x1f0
#define  XENA_XIIC51_BITSELECT_SHIFT 4
#define  XENA_XIIC51_BITSELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_BITSELECT_MASK) >> XENA_XIIC51_BITSELECT_SHIFT)
#define  XENA_XIIC51_BITSELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_BITSELECT_MASK) | (((uint32_t)val) << XENA_XIIC51_BITSELECT_SHIFT))

#define  XENA_XIIC51_SELECT_MASK 0xf
#define  XENA_XIIC51_SELECT_SHIFT 0
#define  XENA_XIIC51_SELECT_MASK_SHIFT(reg) (((reg) & XENA_XIIC51_SELECT_MASK) >> XENA_XIIC51_SELECT_SHIFT)
#define  XENA_XIIC51_SELECT_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIIC51_SELECT_MASK) | (((uint32_t)val) << XENA_XIIC51_SELECT_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 1 of 52) (XIP00)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 1 of 52)*/
//====================================================================

#define  XENA_XIP00_PHASE3_MASK 0xff000000
#define  XENA_XIP00_PHASE3_SHIFT 24
#define  XENA_XIP00_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP00_PHASE3_MASK) >> XENA_XIP00_PHASE3_SHIFT)
#define  XENA_XIP00_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP00_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP00_PHASE3_SHIFT))

#define  XENA_XIP00_PHASE2_MASK 0xff0000
#define  XENA_XIP00_PHASE2_SHIFT 16
#define  XENA_XIP00_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP00_PHASE2_MASK) >> XENA_XIP00_PHASE2_SHIFT)
#define  XENA_XIP00_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP00_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP00_PHASE2_SHIFT))

#define  XENA_XIP00_PHASE1_MASK 0xff00
#define  XENA_XIP00_PHASE1_SHIFT 8
#define  XENA_XIP00_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP00_PHASE1_MASK) >> XENA_XIP00_PHASE1_SHIFT)
#define  XENA_XIP00_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP00_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP00_PHASE1_SHIFT))

#define  XENA_XIP00_PHASE0_MASK 0xff
#define  XENA_XIP00_PHASE0_SHIFT 0
#define  XENA_XIP00_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP00_PHASE0_MASK) >> XENA_XIP00_PHASE0_SHIFT)
#define  XENA_XIP00_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP00_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP00_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 2 of 52) (XIP01)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 2 of 52)*/
//====================================================================

#define  XENA_XIP01_PHASE3_MASK 0xff000000
#define  XENA_XIP01_PHASE3_SHIFT 24
#define  XENA_XIP01_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP01_PHASE3_MASK) >> XENA_XIP01_PHASE3_SHIFT)
#define  XENA_XIP01_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP01_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP01_PHASE3_SHIFT))

#define  XENA_XIP01_PHASE2_MASK 0xff0000
#define  XENA_XIP01_PHASE2_SHIFT 16
#define  XENA_XIP01_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP01_PHASE2_MASK) >> XENA_XIP01_PHASE2_SHIFT)
#define  XENA_XIP01_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP01_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP01_PHASE2_SHIFT))

#define  XENA_XIP01_PHASE1_MASK 0xff00
#define  XENA_XIP01_PHASE1_SHIFT 8
#define  XENA_XIP01_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP01_PHASE1_MASK) >> XENA_XIP01_PHASE1_SHIFT)
#define  XENA_XIP01_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP01_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP01_PHASE1_SHIFT))

#define  XENA_XIP01_PHASE0_MASK 0xff
#define  XENA_XIP01_PHASE0_SHIFT 0
#define  XENA_XIP01_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP01_PHASE0_MASK) >> XENA_XIP01_PHASE0_SHIFT)
#define  XENA_XIP01_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP01_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP01_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 3 of 52) (XIP02)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 3 of 52)*/
//====================================================================

#define  XENA_XIP02_PHASE3_MASK 0xff000000
#define  XENA_XIP02_PHASE3_SHIFT 24
#define  XENA_XIP02_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP02_PHASE3_MASK) >> XENA_XIP02_PHASE3_SHIFT)
#define  XENA_XIP02_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP02_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP02_PHASE3_SHIFT))

#define  XENA_XIP02_PHASE2_MASK 0xff0000
#define  XENA_XIP02_PHASE2_SHIFT 16
#define  XENA_XIP02_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP02_PHASE2_MASK) >> XENA_XIP02_PHASE2_SHIFT)
#define  XENA_XIP02_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP02_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP02_PHASE2_SHIFT))

#define  XENA_XIP02_PHASE1_MASK 0xff00
#define  XENA_XIP02_PHASE1_SHIFT 8
#define  XENA_XIP02_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP02_PHASE1_MASK) >> XENA_XIP02_PHASE1_SHIFT)
#define  XENA_XIP02_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP02_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP02_PHASE1_SHIFT))

#define  XENA_XIP02_PHASE0_MASK 0xff
#define  XENA_XIP02_PHASE0_SHIFT 0
#define  XENA_XIP02_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP02_PHASE0_MASK) >> XENA_XIP02_PHASE0_SHIFT)
#define  XENA_XIP02_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP02_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP02_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 4 of 52) (XIP03)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 4 of 52)*/
//====================================================================

#define  XENA_XIP03_PHASE3_MASK 0xff000000
#define  XENA_XIP03_PHASE3_SHIFT 24
#define  XENA_XIP03_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP03_PHASE3_MASK) >> XENA_XIP03_PHASE3_SHIFT)
#define  XENA_XIP03_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP03_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP03_PHASE3_SHIFT))

#define  XENA_XIP03_PHASE2_MASK 0xff0000
#define  XENA_XIP03_PHASE2_SHIFT 16
#define  XENA_XIP03_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP03_PHASE2_MASK) >> XENA_XIP03_PHASE2_SHIFT)
#define  XENA_XIP03_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP03_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP03_PHASE2_SHIFT))

#define  XENA_XIP03_PHASE1_MASK 0xff00
#define  XENA_XIP03_PHASE1_SHIFT 8
#define  XENA_XIP03_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP03_PHASE1_MASK) >> XENA_XIP03_PHASE1_SHIFT)
#define  XENA_XIP03_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP03_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP03_PHASE1_SHIFT))

#define  XENA_XIP03_PHASE0_MASK 0xff
#define  XENA_XIP03_PHASE0_SHIFT 0
#define  XENA_XIP03_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP03_PHASE0_MASK) >> XENA_XIP03_PHASE0_SHIFT)
#define  XENA_XIP03_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP03_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP03_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 5 of 52) (XIP04)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 5 of 52)*/
//====================================================================

#define  XENA_XIP04_PHASE3_MASK 0xff000000
#define  XENA_XIP04_PHASE3_SHIFT 24
#define  XENA_XIP04_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP04_PHASE3_MASK) >> XENA_XIP04_PHASE3_SHIFT)
#define  XENA_XIP04_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP04_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP04_PHASE3_SHIFT))

#define  XENA_XIP04_PHASE2_MASK 0xff0000
#define  XENA_XIP04_PHASE2_SHIFT 16
#define  XENA_XIP04_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP04_PHASE2_MASK) >> XENA_XIP04_PHASE2_SHIFT)
#define  XENA_XIP04_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP04_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP04_PHASE2_SHIFT))

#define  XENA_XIP04_PHASE1_MASK 0xff00
#define  XENA_XIP04_PHASE1_SHIFT 8
#define  XENA_XIP04_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP04_PHASE1_MASK) >> XENA_XIP04_PHASE1_SHIFT)
#define  XENA_XIP04_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP04_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP04_PHASE1_SHIFT))

#define  XENA_XIP04_PHASE0_MASK 0xff
#define  XENA_XIP04_PHASE0_SHIFT 0
#define  XENA_XIP04_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP04_PHASE0_MASK) >> XENA_XIP04_PHASE0_SHIFT)
#define  XENA_XIP04_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP04_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP04_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 6 of 52) (XIP05)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 6 of 52)*/
//====================================================================

#define  XENA_XIP05_PHASE3_MASK 0xff000000
#define  XENA_XIP05_PHASE3_SHIFT 24
#define  XENA_XIP05_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP05_PHASE3_MASK) >> XENA_XIP05_PHASE3_SHIFT)
#define  XENA_XIP05_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP05_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP05_PHASE3_SHIFT))

#define  XENA_XIP05_PHASE2_MASK 0xff0000
#define  XENA_XIP05_PHASE2_SHIFT 16
#define  XENA_XIP05_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP05_PHASE2_MASK) >> XENA_XIP05_PHASE2_SHIFT)
#define  XENA_XIP05_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP05_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP05_PHASE2_SHIFT))

#define  XENA_XIP05_PHASE1_MASK 0xff00
#define  XENA_XIP05_PHASE1_SHIFT 8
#define  XENA_XIP05_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP05_PHASE1_MASK) >> XENA_XIP05_PHASE1_SHIFT)
#define  XENA_XIP05_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP05_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP05_PHASE1_SHIFT))

#define  XENA_XIP05_PHASE0_MASK 0xff
#define  XENA_XIP05_PHASE0_SHIFT 0
#define  XENA_XIP05_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP05_PHASE0_MASK) >> XENA_XIP05_PHASE0_SHIFT)
#define  XENA_XIP05_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP05_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP05_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 7 of 52) (XIP06)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 7 of 52)*/
//====================================================================

#define  XENA_XIP06_PHASE3_MASK 0xff000000
#define  XENA_XIP06_PHASE3_SHIFT 24
#define  XENA_XIP06_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP06_PHASE3_MASK) >> XENA_XIP06_PHASE3_SHIFT)
#define  XENA_XIP06_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP06_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP06_PHASE3_SHIFT))

#define  XENA_XIP06_PHASE2_MASK 0xff0000
#define  XENA_XIP06_PHASE2_SHIFT 16
#define  XENA_XIP06_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP06_PHASE2_MASK) >> XENA_XIP06_PHASE2_SHIFT)
#define  XENA_XIP06_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP06_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP06_PHASE2_SHIFT))

#define  XENA_XIP06_PHASE1_MASK 0xff00
#define  XENA_XIP06_PHASE1_SHIFT 8
#define  XENA_XIP06_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP06_PHASE1_MASK) >> XENA_XIP06_PHASE1_SHIFT)
#define  XENA_XIP06_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP06_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP06_PHASE1_SHIFT))

#define  XENA_XIP06_PHASE0_MASK 0xff
#define  XENA_XIP06_PHASE0_SHIFT 0
#define  XENA_XIP06_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP06_PHASE0_MASK) >> XENA_XIP06_PHASE0_SHIFT)
#define  XENA_XIP06_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP06_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP06_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 8 of 52) (XIP07)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 8 of 52)*/
//====================================================================

#define  XENA_XIP07_PHASE3_MASK 0xff000000
#define  XENA_XIP07_PHASE3_SHIFT 24
#define  XENA_XIP07_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP07_PHASE3_MASK) >> XENA_XIP07_PHASE3_SHIFT)
#define  XENA_XIP07_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP07_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP07_PHASE3_SHIFT))

#define  XENA_XIP07_PHASE2_MASK 0xff0000
#define  XENA_XIP07_PHASE2_SHIFT 16
#define  XENA_XIP07_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP07_PHASE2_MASK) >> XENA_XIP07_PHASE2_SHIFT)
#define  XENA_XIP07_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP07_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP07_PHASE2_SHIFT))

#define  XENA_XIP07_PHASE1_MASK 0xff00
#define  XENA_XIP07_PHASE1_SHIFT 8
#define  XENA_XIP07_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP07_PHASE1_MASK) >> XENA_XIP07_PHASE1_SHIFT)
#define  XENA_XIP07_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP07_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP07_PHASE1_SHIFT))

#define  XENA_XIP07_PHASE0_MASK 0xff
#define  XENA_XIP07_PHASE0_SHIFT 0
#define  XENA_XIP07_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP07_PHASE0_MASK) >> XENA_XIP07_PHASE0_SHIFT)
#define  XENA_XIP07_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP07_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP07_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 9 of 52) (XIP08)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 9 of 52)*/
//====================================================================

#define  XENA_XIP08_PHASE3_MASK 0xff000000
#define  XENA_XIP08_PHASE3_SHIFT 24
#define  XENA_XIP08_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP08_PHASE3_MASK) >> XENA_XIP08_PHASE3_SHIFT)
#define  XENA_XIP08_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP08_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP08_PHASE3_SHIFT))

#define  XENA_XIP08_PHASE2_MASK 0xff0000
#define  XENA_XIP08_PHASE2_SHIFT 16
#define  XENA_XIP08_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP08_PHASE2_MASK) >> XENA_XIP08_PHASE2_SHIFT)
#define  XENA_XIP08_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP08_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP08_PHASE2_SHIFT))

#define  XENA_XIP08_PHASE1_MASK 0xff00
#define  XENA_XIP08_PHASE1_SHIFT 8
#define  XENA_XIP08_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP08_PHASE1_MASK) >> XENA_XIP08_PHASE1_SHIFT)
#define  XENA_XIP08_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP08_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP08_PHASE1_SHIFT))

#define  XENA_XIP08_PHASE0_MASK 0xff
#define  XENA_XIP08_PHASE0_SHIFT 0
#define  XENA_XIP08_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP08_PHASE0_MASK) >> XENA_XIP08_PHASE0_SHIFT)
#define  XENA_XIP08_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP08_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP08_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 10 of 52) (XIP09)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 10 of 52)*/
//====================================================================

#define  XENA_XIP09_PHASE3_MASK 0xff000000
#define  XENA_XIP09_PHASE3_SHIFT 24
#define  XENA_XIP09_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP09_PHASE3_MASK) >> XENA_XIP09_PHASE3_SHIFT)
#define  XENA_XIP09_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP09_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP09_PHASE3_SHIFT))

#define  XENA_XIP09_PHASE2_MASK 0xff0000
#define  XENA_XIP09_PHASE2_SHIFT 16
#define  XENA_XIP09_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP09_PHASE2_MASK) >> XENA_XIP09_PHASE2_SHIFT)
#define  XENA_XIP09_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP09_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP09_PHASE2_SHIFT))

#define  XENA_XIP09_PHASE1_MASK 0xff00
#define  XENA_XIP09_PHASE1_SHIFT 8
#define  XENA_XIP09_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP09_PHASE1_MASK) >> XENA_XIP09_PHASE1_SHIFT)
#define  XENA_XIP09_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP09_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP09_PHASE1_SHIFT))

#define  XENA_XIP09_PHASE0_MASK 0xff
#define  XENA_XIP09_PHASE0_SHIFT 0
#define  XENA_XIP09_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP09_PHASE0_MASK) >> XENA_XIP09_PHASE0_SHIFT)
#define  XENA_XIP09_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP09_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP09_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 11 of 52) (XIP010)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 11 of 52)*/
//====================================================================

#define  XENA_XIP010_PHASE3_MASK 0xff000000
#define  XENA_XIP010_PHASE3_SHIFT 24
#define  XENA_XIP010_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP010_PHASE3_MASK) >> XENA_XIP010_PHASE3_SHIFT)
#define  XENA_XIP010_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP010_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP010_PHASE3_SHIFT))

#define  XENA_XIP010_PHASE2_MASK 0xff0000
#define  XENA_XIP010_PHASE2_SHIFT 16
#define  XENA_XIP010_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP010_PHASE2_MASK) >> XENA_XIP010_PHASE2_SHIFT)
#define  XENA_XIP010_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP010_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP010_PHASE2_SHIFT))

#define  XENA_XIP010_PHASE1_MASK 0xff00
#define  XENA_XIP010_PHASE1_SHIFT 8
#define  XENA_XIP010_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP010_PHASE1_MASK) >> XENA_XIP010_PHASE1_SHIFT)
#define  XENA_XIP010_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP010_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP010_PHASE1_SHIFT))

#define  XENA_XIP010_PHASE0_MASK 0xff
#define  XENA_XIP010_PHASE0_SHIFT 0
#define  XENA_XIP010_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP010_PHASE0_MASK) >> XENA_XIP010_PHASE0_SHIFT)
#define  XENA_XIP010_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP010_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP010_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 12 of 52) (XIP011)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 12 of 52)*/
//====================================================================

#define  XENA_XIP011_PHASE3_MASK 0xff000000
#define  XENA_XIP011_PHASE3_SHIFT 24
#define  XENA_XIP011_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP011_PHASE3_MASK) >> XENA_XIP011_PHASE3_SHIFT)
#define  XENA_XIP011_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP011_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP011_PHASE3_SHIFT))

#define  XENA_XIP011_PHASE2_MASK 0xff0000
#define  XENA_XIP011_PHASE2_SHIFT 16
#define  XENA_XIP011_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP011_PHASE2_MASK) >> XENA_XIP011_PHASE2_SHIFT)
#define  XENA_XIP011_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP011_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP011_PHASE2_SHIFT))

#define  XENA_XIP011_PHASE1_MASK 0xff00
#define  XENA_XIP011_PHASE1_SHIFT 8
#define  XENA_XIP011_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP011_PHASE1_MASK) >> XENA_XIP011_PHASE1_SHIFT)
#define  XENA_XIP011_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP011_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP011_PHASE1_SHIFT))

#define  XENA_XIP011_PHASE0_MASK 0xff
#define  XENA_XIP011_PHASE0_SHIFT 0
#define  XENA_XIP011_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP011_PHASE0_MASK) >> XENA_XIP011_PHASE0_SHIFT)
#define  XENA_XIP011_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP011_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP011_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 13 of 52) (XIP012)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 13 of 52)*/
//====================================================================

#define  XENA_XIP012_PHASE3_MASK 0xff000000
#define  XENA_XIP012_PHASE3_SHIFT 24
#define  XENA_XIP012_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP012_PHASE3_MASK) >> XENA_XIP012_PHASE3_SHIFT)
#define  XENA_XIP012_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP012_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP012_PHASE3_SHIFT))

#define  XENA_XIP012_PHASE2_MASK 0xff0000
#define  XENA_XIP012_PHASE2_SHIFT 16
#define  XENA_XIP012_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP012_PHASE2_MASK) >> XENA_XIP012_PHASE2_SHIFT)
#define  XENA_XIP012_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP012_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP012_PHASE2_SHIFT))

#define  XENA_XIP012_PHASE1_MASK 0xff00
#define  XENA_XIP012_PHASE1_SHIFT 8
#define  XENA_XIP012_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP012_PHASE1_MASK) >> XENA_XIP012_PHASE1_SHIFT)
#define  XENA_XIP012_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP012_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP012_PHASE1_SHIFT))

#define  XENA_XIP012_PHASE0_MASK 0xff
#define  XENA_XIP012_PHASE0_SHIFT 0
#define  XENA_XIP012_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP012_PHASE0_MASK) >> XENA_XIP012_PHASE0_SHIFT)
#define  XENA_XIP012_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP012_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP012_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 14 of 52) (XIP013)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 14 of 52)*/
//====================================================================

#define  XENA_XIP013_PHASE3_MASK 0xff000000
#define  XENA_XIP013_PHASE3_SHIFT 24
#define  XENA_XIP013_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP013_PHASE3_MASK) >> XENA_XIP013_PHASE3_SHIFT)
#define  XENA_XIP013_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP013_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP013_PHASE3_SHIFT))

#define  XENA_XIP013_PHASE2_MASK 0xff0000
#define  XENA_XIP013_PHASE2_SHIFT 16
#define  XENA_XIP013_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP013_PHASE2_MASK) >> XENA_XIP013_PHASE2_SHIFT)
#define  XENA_XIP013_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP013_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP013_PHASE2_SHIFT))

#define  XENA_XIP013_PHASE1_MASK 0xff00
#define  XENA_XIP013_PHASE1_SHIFT 8
#define  XENA_XIP013_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP013_PHASE1_MASK) >> XENA_XIP013_PHASE1_SHIFT)
#define  XENA_XIP013_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP013_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP013_PHASE1_SHIFT))

#define  XENA_XIP013_PHASE0_MASK 0xff
#define  XENA_XIP013_PHASE0_SHIFT 0
#define  XENA_XIP013_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP013_PHASE0_MASK) >> XENA_XIP013_PHASE0_SHIFT)
#define  XENA_XIP013_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP013_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP013_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 15 of 52) (XIP014)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 15 of 52)*/
//====================================================================

#define  XENA_XIP014_PHASE3_MASK 0xff000000
#define  XENA_XIP014_PHASE3_SHIFT 24
#define  XENA_XIP014_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP014_PHASE3_MASK) >> XENA_XIP014_PHASE3_SHIFT)
#define  XENA_XIP014_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP014_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP014_PHASE3_SHIFT))

#define  XENA_XIP014_PHASE2_MASK 0xff0000
#define  XENA_XIP014_PHASE2_SHIFT 16
#define  XENA_XIP014_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP014_PHASE2_MASK) >> XENA_XIP014_PHASE2_SHIFT)
#define  XENA_XIP014_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP014_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP014_PHASE2_SHIFT))

#define  XENA_XIP014_PHASE1_MASK 0xff00
#define  XENA_XIP014_PHASE1_SHIFT 8
#define  XENA_XIP014_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP014_PHASE1_MASK) >> XENA_XIP014_PHASE1_SHIFT)
#define  XENA_XIP014_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP014_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP014_PHASE1_SHIFT))

#define  XENA_XIP014_PHASE0_MASK 0xff
#define  XENA_XIP014_PHASE0_SHIFT 0
#define  XENA_XIP014_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP014_PHASE0_MASK) >> XENA_XIP014_PHASE0_SHIFT)
#define  XENA_XIP014_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP014_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP014_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 16 of 52) (XIP015)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 16 of 52)*/
//====================================================================

#define  XENA_XIP015_PHASE3_MASK 0xff000000
#define  XENA_XIP015_PHASE3_SHIFT 24
#define  XENA_XIP015_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP015_PHASE3_MASK) >> XENA_XIP015_PHASE3_SHIFT)
#define  XENA_XIP015_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP015_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP015_PHASE3_SHIFT))

#define  XENA_XIP015_PHASE2_MASK 0xff0000
#define  XENA_XIP015_PHASE2_SHIFT 16
#define  XENA_XIP015_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP015_PHASE2_MASK) >> XENA_XIP015_PHASE2_SHIFT)
#define  XENA_XIP015_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP015_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP015_PHASE2_SHIFT))

#define  XENA_XIP015_PHASE1_MASK 0xff00
#define  XENA_XIP015_PHASE1_SHIFT 8
#define  XENA_XIP015_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP015_PHASE1_MASK) >> XENA_XIP015_PHASE1_SHIFT)
#define  XENA_XIP015_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP015_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP015_PHASE1_SHIFT))

#define  XENA_XIP015_PHASE0_MASK 0xff
#define  XENA_XIP015_PHASE0_SHIFT 0
#define  XENA_XIP015_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP015_PHASE0_MASK) >> XENA_XIP015_PHASE0_SHIFT)
#define  XENA_XIP015_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP015_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP015_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 17 of 52) (XIP016)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 17 of 52)*/
//====================================================================

#define  XENA_XIP016_PHASE3_MASK 0xff000000
#define  XENA_XIP016_PHASE3_SHIFT 24
#define  XENA_XIP016_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP016_PHASE3_MASK) >> XENA_XIP016_PHASE3_SHIFT)
#define  XENA_XIP016_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP016_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP016_PHASE3_SHIFT))

#define  XENA_XIP016_PHASE2_MASK 0xff0000
#define  XENA_XIP016_PHASE2_SHIFT 16
#define  XENA_XIP016_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP016_PHASE2_MASK) >> XENA_XIP016_PHASE2_SHIFT)
#define  XENA_XIP016_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP016_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP016_PHASE2_SHIFT))

#define  XENA_XIP016_PHASE1_MASK 0xff00
#define  XENA_XIP016_PHASE1_SHIFT 8
#define  XENA_XIP016_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP016_PHASE1_MASK) >> XENA_XIP016_PHASE1_SHIFT)
#define  XENA_XIP016_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP016_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP016_PHASE1_SHIFT))

#define  XENA_XIP016_PHASE0_MASK 0xff
#define  XENA_XIP016_PHASE0_SHIFT 0
#define  XENA_XIP016_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP016_PHASE0_MASK) >> XENA_XIP016_PHASE0_SHIFT)
#define  XENA_XIP016_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP016_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP016_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 18 of 52) (XIP017)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 18 of 52)*/
//====================================================================

#define  XENA_XIP017_PHASE3_MASK 0xff000000
#define  XENA_XIP017_PHASE3_SHIFT 24
#define  XENA_XIP017_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP017_PHASE3_MASK) >> XENA_XIP017_PHASE3_SHIFT)
#define  XENA_XIP017_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP017_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP017_PHASE3_SHIFT))

#define  XENA_XIP017_PHASE2_MASK 0xff0000
#define  XENA_XIP017_PHASE2_SHIFT 16
#define  XENA_XIP017_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP017_PHASE2_MASK) >> XENA_XIP017_PHASE2_SHIFT)
#define  XENA_XIP017_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP017_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP017_PHASE2_SHIFT))

#define  XENA_XIP017_PHASE1_MASK 0xff00
#define  XENA_XIP017_PHASE1_SHIFT 8
#define  XENA_XIP017_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP017_PHASE1_MASK) >> XENA_XIP017_PHASE1_SHIFT)
#define  XENA_XIP017_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP017_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP017_PHASE1_SHIFT))

#define  XENA_XIP017_PHASE0_MASK 0xff
#define  XENA_XIP017_PHASE0_SHIFT 0
#define  XENA_XIP017_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP017_PHASE0_MASK) >> XENA_XIP017_PHASE0_SHIFT)
#define  XENA_XIP017_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP017_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP017_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 19 of 52) (XIP018)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 19 of 52)*/
//====================================================================

#define  XENA_XIP018_PHASE3_MASK 0xff000000
#define  XENA_XIP018_PHASE3_SHIFT 24
#define  XENA_XIP018_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP018_PHASE3_MASK) >> XENA_XIP018_PHASE3_SHIFT)
#define  XENA_XIP018_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP018_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP018_PHASE3_SHIFT))

#define  XENA_XIP018_PHASE2_MASK 0xff0000
#define  XENA_XIP018_PHASE2_SHIFT 16
#define  XENA_XIP018_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP018_PHASE2_MASK) >> XENA_XIP018_PHASE2_SHIFT)
#define  XENA_XIP018_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP018_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP018_PHASE2_SHIFT))

#define  XENA_XIP018_PHASE1_MASK 0xff00
#define  XENA_XIP018_PHASE1_SHIFT 8
#define  XENA_XIP018_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP018_PHASE1_MASK) >> XENA_XIP018_PHASE1_SHIFT)
#define  XENA_XIP018_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP018_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP018_PHASE1_SHIFT))

#define  XENA_XIP018_PHASE0_MASK 0xff
#define  XENA_XIP018_PHASE0_SHIFT 0
#define  XENA_XIP018_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP018_PHASE0_MASK) >> XENA_XIP018_PHASE0_SHIFT)
#define  XENA_XIP018_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP018_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP018_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 20 of 52) (XIP019)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 20 of 52)*/
//====================================================================

#define  XENA_XIP019_PHASE3_MASK 0xff000000
#define  XENA_XIP019_PHASE3_SHIFT 24
#define  XENA_XIP019_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP019_PHASE3_MASK) >> XENA_XIP019_PHASE3_SHIFT)
#define  XENA_XIP019_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP019_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP019_PHASE3_SHIFT))

#define  XENA_XIP019_PHASE2_MASK 0xff0000
#define  XENA_XIP019_PHASE2_SHIFT 16
#define  XENA_XIP019_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP019_PHASE2_MASK) >> XENA_XIP019_PHASE2_SHIFT)
#define  XENA_XIP019_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP019_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP019_PHASE2_SHIFT))

#define  XENA_XIP019_PHASE1_MASK 0xff00
#define  XENA_XIP019_PHASE1_SHIFT 8
#define  XENA_XIP019_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP019_PHASE1_MASK) >> XENA_XIP019_PHASE1_SHIFT)
#define  XENA_XIP019_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP019_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP019_PHASE1_SHIFT))

#define  XENA_XIP019_PHASE0_MASK 0xff
#define  XENA_XIP019_PHASE0_SHIFT 0
#define  XENA_XIP019_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP019_PHASE0_MASK) >> XENA_XIP019_PHASE0_SHIFT)
#define  XENA_XIP019_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP019_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP019_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 21 of 52) (XIP020)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 21 of 52)*/
//====================================================================

#define  XENA_XIP020_PHASE3_MASK 0xff000000
#define  XENA_XIP020_PHASE3_SHIFT 24
#define  XENA_XIP020_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP020_PHASE3_MASK) >> XENA_XIP020_PHASE3_SHIFT)
#define  XENA_XIP020_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP020_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP020_PHASE3_SHIFT))

#define  XENA_XIP020_PHASE2_MASK 0xff0000
#define  XENA_XIP020_PHASE2_SHIFT 16
#define  XENA_XIP020_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP020_PHASE2_MASK) >> XENA_XIP020_PHASE2_SHIFT)
#define  XENA_XIP020_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP020_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP020_PHASE2_SHIFT))

#define  XENA_XIP020_PHASE1_MASK 0xff00
#define  XENA_XIP020_PHASE1_SHIFT 8
#define  XENA_XIP020_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP020_PHASE1_MASK) >> XENA_XIP020_PHASE1_SHIFT)
#define  XENA_XIP020_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP020_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP020_PHASE1_SHIFT))

#define  XENA_XIP020_PHASE0_MASK 0xff
#define  XENA_XIP020_PHASE0_SHIFT 0
#define  XENA_XIP020_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP020_PHASE0_MASK) >> XENA_XIP020_PHASE0_SHIFT)
#define  XENA_XIP020_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP020_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP020_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 22 of 52) (XIP021)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 22 of 52)*/
//====================================================================

#define  XENA_XIP021_PHASE3_MASK 0xff000000
#define  XENA_XIP021_PHASE3_SHIFT 24
#define  XENA_XIP021_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP021_PHASE3_MASK) >> XENA_XIP021_PHASE3_SHIFT)
#define  XENA_XIP021_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP021_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP021_PHASE3_SHIFT))

#define  XENA_XIP021_PHASE2_MASK 0xff0000
#define  XENA_XIP021_PHASE2_SHIFT 16
#define  XENA_XIP021_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP021_PHASE2_MASK) >> XENA_XIP021_PHASE2_SHIFT)
#define  XENA_XIP021_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP021_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP021_PHASE2_SHIFT))

#define  XENA_XIP021_PHASE1_MASK 0xff00
#define  XENA_XIP021_PHASE1_SHIFT 8
#define  XENA_XIP021_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP021_PHASE1_MASK) >> XENA_XIP021_PHASE1_SHIFT)
#define  XENA_XIP021_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP021_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP021_PHASE1_SHIFT))

#define  XENA_XIP021_PHASE0_MASK 0xff
#define  XENA_XIP021_PHASE0_SHIFT 0
#define  XENA_XIP021_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP021_PHASE0_MASK) >> XENA_XIP021_PHASE0_SHIFT)
#define  XENA_XIP021_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP021_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP021_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 23 of 52) (XIP022)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 23 of 52)*/
//====================================================================

#define  XENA_XIP022_PHASE3_MASK 0xff000000
#define  XENA_XIP022_PHASE3_SHIFT 24
#define  XENA_XIP022_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP022_PHASE3_MASK) >> XENA_XIP022_PHASE3_SHIFT)
#define  XENA_XIP022_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP022_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP022_PHASE3_SHIFT))

#define  XENA_XIP022_PHASE2_MASK 0xff0000
#define  XENA_XIP022_PHASE2_SHIFT 16
#define  XENA_XIP022_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP022_PHASE2_MASK) >> XENA_XIP022_PHASE2_SHIFT)
#define  XENA_XIP022_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP022_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP022_PHASE2_SHIFT))

#define  XENA_XIP022_PHASE1_MASK 0xff00
#define  XENA_XIP022_PHASE1_SHIFT 8
#define  XENA_XIP022_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP022_PHASE1_MASK) >> XENA_XIP022_PHASE1_SHIFT)
#define  XENA_XIP022_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP022_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP022_PHASE1_SHIFT))

#define  XENA_XIP022_PHASE0_MASK 0xff
#define  XENA_XIP022_PHASE0_SHIFT 0
#define  XENA_XIP022_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP022_PHASE0_MASK) >> XENA_XIP022_PHASE0_SHIFT)
#define  XENA_XIP022_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP022_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP022_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 24 of 52) (XIP023)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 24 of 52)*/
//====================================================================

#define  XENA_XIP023_PHASE3_MASK 0xff000000
#define  XENA_XIP023_PHASE3_SHIFT 24
#define  XENA_XIP023_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP023_PHASE3_MASK) >> XENA_XIP023_PHASE3_SHIFT)
#define  XENA_XIP023_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP023_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP023_PHASE3_SHIFT))

#define  XENA_XIP023_PHASE2_MASK 0xff0000
#define  XENA_XIP023_PHASE2_SHIFT 16
#define  XENA_XIP023_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP023_PHASE2_MASK) >> XENA_XIP023_PHASE2_SHIFT)
#define  XENA_XIP023_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP023_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP023_PHASE2_SHIFT))

#define  XENA_XIP023_PHASE1_MASK 0xff00
#define  XENA_XIP023_PHASE1_SHIFT 8
#define  XENA_XIP023_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP023_PHASE1_MASK) >> XENA_XIP023_PHASE1_SHIFT)
#define  XENA_XIP023_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP023_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP023_PHASE1_SHIFT))

#define  XENA_XIP023_PHASE0_MASK 0xff
#define  XENA_XIP023_PHASE0_SHIFT 0
#define  XENA_XIP023_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP023_PHASE0_MASK) >> XENA_XIP023_PHASE0_SHIFT)
#define  XENA_XIP023_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP023_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP023_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 25 of 52) (XIP024)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 25 of 52)*/
//====================================================================

#define  XENA_XIP024_PHASE3_MASK 0xff000000
#define  XENA_XIP024_PHASE3_SHIFT 24
#define  XENA_XIP024_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP024_PHASE3_MASK) >> XENA_XIP024_PHASE3_SHIFT)
#define  XENA_XIP024_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP024_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP024_PHASE3_SHIFT))

#define  XENA_XIP024_PHASE2_MASK 0xff0000
#define  XENA_XIP024_PHASE2_SHIFT 16
#define  XENA_XIP024_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP024_PHASE2_MASK) >> XENA_XIP024_PHASE2_SHIFT)
#define  XENA_XIP024_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP024_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP024_PHASE2_SHIFT))

#define  XENA_XIP024_PHASE1_MASK 0xff00
#define  XENA_XIP024_PHASE1_SHIFT 8
#define  XENA_XIP024_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP024_PHASE1_MASK) >> XENA_XIP024_PHASE1_SHIFT)
#define  XENA_XIP024_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP024_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP024_PHASE1_SHIFT))

#define  XENA_XIP024_PHASE0_MASK 0xff
#define  XENA_XIP024_PHASE0_SHIFT 0
#define  XENA_XIP024_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP024_PHASE0_MASK) >> XENA_XIP024_PHASE0_SHIFT)
#define  XENA_XIP024_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP024_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP024_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 26 of 52) (XIP025)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 26 of 52)*/
//====================================================================

#define  XENA_XIP025_PHASE3_MASK 0xff000000
#define  XENA_XIP025_PHASE3_SHIFT 24
#define  XENA_XIP025_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP025_PHASE3_MASK) >> XENA_XIP025_PHASE3_SHIFT)
#define  XENA_XIP025_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP025_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP025_PHASE3_SHIFT))

#define  XENA_XIP025_PHASE2_MASK 0xff0000
#define  XENA_XIP025_PHASE2_SHIFT 16
#define  XENA_XIP025_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP025_PHASE2_MASK) >> XENA_XIP025_PHASE2_SHIFT)
#define  XENA_XIP025_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP025_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP025_PHASE2_SHIFT))

#define  XENA_XIP025_PHASE1_MASK 0xff00
#define  XENA_XIP025_PHASE1_SHIFT 8
#define  XENA_XIP025_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP025_PHASE1_MASK) >> XENA_XIP025_PHASE1_SHIFT)
#define  XENA_XIP025_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP025_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP025_PHASE1_SHIFT))

#define  XENA_XIP025_PHASE0_MASK 0xff
#define  XENA_XIP025_PHASE0_SHIFT 0
#define  XENA_XIP025_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP025_PHASE0_MASK) >> XENA_XIP025_PHASE0_SHIFT)
#define  XENA_XIP025_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP025_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP025_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 27 of 52) (XIP026)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 27 of 52)*/
//====================================================================

#define  XENA_XIP026_PHASE3_MASK 0xff000000
#define  XENA_XIP026_PHASE3_SHIFT 24
#define  XENA_XIP026_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP026_PHASE3_MASK) >> XENA_XIP026_PHASE3_SHIFT)
#define  XENA_XIP026_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP026_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP026_PHASE3_SHIFT))

#define  XENA_XIP026_PHASE2_MASK 0xff0000
#define  XENA_XIP026_PHASE2_SHIFT 16
#define  XENA_XIP026_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP026_PHASE2_MASK) >> XENA_XIP026_PHASE2_SHIFT)
#define  XENA_XIP026_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP026_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP026_PHASE2_SHIFT))

#define  XENA_XIP026_PHASE1_MASK 0xff00
#define  XENA_XIP026_PHASE1_SHIFT 8
#define  XENA_XIP026_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP026_PHASE1_MASK) >> XENA_XIP026_PHASE1_SHIFT)
#define  XENA_XIP026_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP026_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP026_PHASE1_SHIFT))

#define  XENA_XIP026_PHASE0_MASK 0xff
#define  XENA_XIP026_PHASE0_SHIFT 0
#define  XENA_XIP026_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP026_PHASE0_MASK) >> XENA_XIP026_PHASE0_SHIFT)
#define  XENA_XIP026_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP026_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP026_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 28 of 52) (XIP027)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 28 of 52)*/
//====================================================================

#define  XENA_XIP027_PHASE3_MASK 0xff000000
#define  XENA_XIP027_PHASE3_SHIFT 24
#define  XENA_XIP027_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP027_PHASE3_MASK) >> XENA_XIP027_PHASE3_SHIFT)
#define  XENA_XIP027_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP027_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP027_PHASE3_SHIFT))

#define  XENA_XIP027_PHASE2_MASK 0xff0000
#define  XENA_XIP027_PHASE2_SHIFT 16
#define  XENA_XIP027_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP027_PHASE2_MASK) >> XENA_XIP027_PHASE2_SHIFT)
#define  XENA_XIP027_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP027_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP027_PHASE2_SHIFT))

#define  XENA_XIP027_PHASE1_MASK 0xff00
#define  XENA_XIP027_PHASE1_SHIFT 8
#define  XENA_XIP027_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP027_PHASE1_MASK) >> XENA_XIP027_PHASE1_SHIFT)
#define  XENA_XIP027_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP027_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP027_PHASE1_SHIFT))

#define  XENA_XIP027_PHASE0_MASK 0xff
#define  XENA_XIP027_PHASE0_SHIFT 0
#define  XENA_XIP027_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP027_PHASE0_MASK) >> XENA_XIP027_PHASE0_SHIFT)
#define  XENA_XIP027_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP027_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP027_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 29 of 52) (XIP028)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 29 of 52)*/
//====================================================================

#define  XENA_XIP028_PHASE3_MASK 0xff000000
#define  XENA_XIP028_PHASE3_SHIFT 24
#define  XENA_XIP028_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP028_PHASE3_MASK) >> XENA_XIP028_PHASE3_SHIFT)
#define  XENA_XIP028_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP028_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP028_PHASE3_SHIFT))

#define  XENA_XIP028_PHASE2_MASK 0xff0000
#define  XENA_XIP028_PHASE2_SHIFT 16
#define  XENA_XIP028_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP028_PHASE2_MASK) >> XENA_XIP028_PHASE2_SHIFT)
#define  XENA_XIP028_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP028_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP028_PHASE2_SHIFT))

#define  XENA_XIP028_PHASE1_MASK 0xff00
#define  XENA_XIP028_PHASE1_SHIFT 8
#define  XENA_XIP028_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP028_PHASE1_MASK) >> XENA_XIP028_PHASE1_SHIFT)
#define  XENA_XIP028_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP028_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP028_PHASE1_SHIFT))

#define  XENA_XIP028_PHASE0_MASK 0xff
#define  XENA_XIP028_PHASE0_SHIFT 0
#define  XENA_XIP028_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP028_PHASE0_MASK) >> XENA_XIP028_PHASE0_SHIFT)
#define  XENA_XIP028_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP028_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP028_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 30 of 52) (XIP029)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 30 of 52)*/
//====================================================================

#define  XENA_XIP029_PHASE3_MASK 0xff000000
#define  XENA_XIP029_PHASE3_SHIFT 24
#define  XENA_XIP029_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP029_PHASE3_MASK) >> XENA_XIP029_PHASE3_SHIFT)
#define  XENA_XIP029_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP029_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP029_PHASE3_SHIFT))

#define  XENA_XIP029_PHASE2_MASK 0xff0000
#define  XENA_XIP029_PHASE2_SHIFT 16
#define  XENA_XIP029_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP029_PHASE2_MASK) >> XENA_XIP029_PHASE2_SHIFT)
#define  XENA_XIP029_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP029_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP029_PHASE2_SHIFT))

#define  XENA_XIP029_PHASE1_MASK 0xff00
#define  XENA_XIP029_PHASE1_SHIFT 8
#define  XENA_XIP029_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP029_PHASE1_MASK) >> XENA_XIP029_PHASE1_SHIFT)
#define  XENA_XIP029_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP029_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP029_PHASE1_SHIFT))

#define  XENA_XIP029_PHASE0_MASK 0xff
#define  XENA_XIP029_PHASE0_SHIFT 0
#define  XENA_XIP029_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP029_PHASE0_MASK) >> XENA_XIP029_PHASE0_SHIFT)
#define  XENA_XIP029_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP029_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP029_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 31 of 52) (XIP030)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 31 of 52)*/
//====================================================================

#define  XENA_XIP030_PHASE3_MASK 0xff000000
#define  XENA_XIP030_PHASE3_SHIFT 24
#define  XENA_XIP030_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP030_PHASE3_MASK) >> XENA_XIP030_PHASE3_SHIFT)
#define  XENA_XIP030_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP030_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP030_PHASE3_SHIFT))

#define  XENA_XIP030_PHASE2_MASK 0xff0000
#define  XENA_XIP030_PHASE2_SHIFT 16
#define  XENA_XIP030_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP030_PHASE2_MASK) >> XENA_XIP030_PHASE2_SHIFT)
#define  XENA_XIP030_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP030_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP030_PHASE2_SHIFT))

#define  XENA_XIP030_PHASE1_MASK 0xff00
#define  XENA_XIP030_PHASE1_SHIFT 8
#define  XENA_XIP030_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP030_PHASE1_MASK) >> XENA_XIP030_PHASE1_SHIFT)
#define  XENA_XIP030_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP030_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP030_PHASE1_SHIFT))

#define  XENA_XIP030_PHASE0_MASK 0xff
#define  XENA_XIP030_PHASE0_SHIFT 0
#define  XENA_XIP030_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP030_PHASE0_MASK) >> XENA_XIP030_PHASE0_SHIFT)
#define  XENA_XIP030_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP030_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP030_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 32 of 52) (XIP031)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 32 of 52)*/
//====================================================================

#define  XENA_XIP031_PHASE3_MASK 0xff000000
#define  XENA_XIP031_PHASE3_SHIFT 24
#define  XENA_XIP031_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP031_PHASE3_MASK) >> XENA_XIP031_PHASE3_SHIFT)
#define  XENA_XIP031_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP031_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP031_PHASE3_SHIFT))

#define  XENA_XIP031_PHASE2_MASK 0xff0000
#define  XENA_XIP031_PHASE2_SHIFT 16
#define  XENA_XIP031_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP031_PHASE2_MASK) >> XENA_XIP031_PHASE2_SHIFT)
#define  XENA_XIP031_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP031_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP031_PHASE2_SHIFT))

#define  XENA_XIP031_PHASE1_MASK 0xff00
#define  XENA_XIP031_PHASE1_SHIFT 8
#define  XENA_XIP031_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP031_PHASE1_MASK) >> XENA_XIP031_PHASE1_SHIFT)
#define  XENA_XIP031_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP031_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP031_PHASE1_SHIFT))

#define  XENA_XIP031_PHASE0_MASK 0xff
#define  XENA_XIP031_PHASE0_SHIFT 0
#define  XENA_XIP031_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP031_PHASE0_MASK) >> XENA_XIP031_PHASE0_SHIFT)
#define  XENA_XIP031_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP031_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP031_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 33 of 52) (XIP032)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 33 of 52)*/
//====================================================================

#define  XENA_XIP032_PHASE3_MASK 0xff000000
#define  XENA_XIP032_PHASE3_SHIFT 24
#define  XENA_XIP032_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP032_PHASE3_MASK) >> XENA_XIP032_PHASE3_SHIFT)
#define  XENA_XIP032_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP032_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP032_PHASE3_SHIFT))

#define  XENA_XIP032_PHASE2_MASK 0xff0000
#define  XENA_XIP032_PHASE2_SHIFT 16
#define  XENA_XIP032_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP032_PHASE2_MASK) >> XENA_XIP032_PHASE2_SHIFT)
#define  XENA_XIP032_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP032_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP032_PHASE2_SHIFT))

#define  XENA_XIP032_PHASE1_MASK 0xff00
#define  XENA_XIP032_PHASE1_SHIFT 8
#define  XENA_XIP032_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP032_PHASE1_MASK) >> XENA_XIP032_PHASE1_SHIFT)
#define  XENA_XIP032_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP032_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP032_PHASE1_SHIFT))

#define  XENA_XIP032_PHASE0_MASK 0xff
#define  XENA_XIP032_PHASE0_SHIFT 0
#define  XENA_XIP032_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP032_PHASE0_MASK) >> XENA_XIP032_PHASE0_SHIFT)
#define  XENA_XIP032_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP032_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP032_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 34 of 52) (XIP033)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 34 of 52)*/
//====================================================================

#define  XENA_XIP033_PHASE3_MASK 0xff000000
#define  XENA_XIP033_PHASE3_SHIFT 24
#define  XENA_XIP033_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP033_PHASE3_MASK) >> XENA_XIP033_PHASE3_SHIFT)
#define  XENA_XIP033_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP033_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP033_PHASE3_SHIFT))

#define  XENA_XIP033_PHASE2_MASK 0xff0000
#define  XENA_XIP033_PHASE2_SHIFT 16
#define  XENA_XIP033_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP033_PHASE2_MASK) >> XENA_XIP033_PHASE2_SHIFT)
#define  XENA_XIP033_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP033_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP033_PHASE2_SHIFT))

#define  XENA_XIP033_PHASE1_MASK 0xff00
#define  XENA_XIP033_PHASE1_SHIFT 8
#define  XENA_XIP033_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP033_PHASE1_MASK) >> XENA_XIP033_PHASE1_SHIFT)
#define  XENA_XIP033_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP033_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP033_PHASE1_SHIFT))

#define  XENA_XIP033_PHASE0_MASK 0xff
#define  XENA_XIP033_PHASE0_SHIFT 0
#define  XENA_XIP033_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP033_PHASE0_MASK) >> XENA_XIP033_PHASE0_SHIFT)
#define  XENA_XIP033_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP033_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP033_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 35 of 52) (XIP034)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 35 of 52)*/
//====================================================================

#define  XENA_XIP034_PHASE3_MASK 0xff000000
#define  XENA_XIP034_PHASE3_SHIFT 24
#define  XENA_XIP034_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP034_PHASE3_MASK) >> XENA_XIP034_PHASE3_SHIFT)
#define  XENA_XIP034_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP034_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP034_PHASE3_SHIFT))

#define  XENA_XIP034_PHASE2_MASK 0xff0000
#define  XENA_XIP034_PHASE2_SHIFT 16
#define  XENA_XIP034_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP034_PHASE2_MASK) >> XENA_XIP034_PHASE2_SHIFT)
#define  XENA_XIP034_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP034_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP034_PHASE2_SHIFT))

#define  XENA_XIP034_PHASE1_MASK 0xff00
#define  XENA_XIP034_PHASE1_SHIFT 8
#define  XENA_XIP034_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP034_PHASE1_MASK) >> XENA_XIP034_PHASE1_SHIFT)
#define  XENA_XIP034_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP034_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP034_PHASE1_SHIFT))

#define  XENA_XIP034_PHASE0_MASK 0xff
#define  XENA_XIP034_PHASE0_SHIFT 0
#define  XENA_XIP034_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP034_PHASE0_MASK) >> XENA_XIP034_PHASE0_SHIFT)
#define  XENA_XIP034_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP034_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP034_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 36 of 52) (XIP035)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 36 of 52)*/
//====================================================================

#define  XENA_XIP035_PHASE3_MASK 0xff000000
#define  XENA_XIP035_PHASE3_SHIFT 24
#define  XENA_XIP035_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP035_PHASE3_MASK) >> XENA_XIP035_PHASE3_SHIFT)
#define  XENA_XIP035_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP035_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP035_PHASE3_SHIFT))

#define  XENA_XIP035_PHASE2_MASK 0xff0000
#define  XENA_XIP035_PHASE2_SHIFT 16
#define  XENA_XIP035_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP035_PHASE2_MASK) >> XENA_XIP035_PHASE2_SHIFT)
#define  XENA_XIP035_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP035_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP035_PHASE2_SHIFT))

#define  XENA_XIP035_PHASE1_MASK 0xff00
#define  XENA_XIP035_PHASE1_SHIFT 8
#define  XENA_XIP035_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP035_PHASE1_MASK) >> XENA_XIP035_PHASE1_SHIFT)
#define  XENA_XIP035_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP035_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP035_PHASE1_SHIFT))

#define  XENA_XIP035_PHASE0_MASK 0xff
#define  XENA_XIP035_PHASE0_SHIFT 0
#define  XENA_XIP035_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP035_PHASE0_MASK) >> XENA_XIP035_PHASE0_SHIFT)
#define  XENA_XIP035_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP035_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP035_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 37 of 52) (XIP036)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 37 of 52)*/
//====================================================================

#define  XENA_XIP036_PHASE3_MASK 0xff000000
#define  XENA_XIP036_PHASE3_SHIFT 24
#define  XENA_XIP036_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP036_PHASE3_MASK) >> XENA_XIP036_PHASE3_SHIFT)
#define  XENA_XIP036_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP036_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP036_PHASE3_SHIFT))

#define  XENA_XIP036_PHASE2_MASK 0xff0000
#define  XENA_XIP036_PHASE2_SHIFT 16
#define  XENA_XIP036_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP036_PHASE2_MASK) >> XENA_XIP036_PHASE2_SHIFT)
#define  XENA_XIP036_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP036_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP036_PHASE2_SHIFT))

#define  XENA_XIP036_PHASE1_MASK 0xff00
#define  XENA_XIP036_PHASE1_SHIFT 8
#define  XENA_XIP036_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP036_PHASE1_MASK) >> XENA_XIP036_PHASE1_SHIFT)
#define  XENA_XIP036_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP036_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP036_PHASE1_SHIFT))

#define  XENA_XIP036_PHASE0_MASK 0xff
#define  XENA_XIP036_PHASE0_SHIFT 0
#define  XENA_XIP036_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP036_PHASE0_MASK) >> XENA_XIP036_PHASE0_SHIFT)
#define  XENA_XIP036_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP036_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP036_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 38 of 52) (XIP037)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 38 of 52)*/
//====================================================================

#define  XENA_XIP037_PHASE3_MASK 0xff000000
#define  XENA_XIP037_PHASE3_SHIFT 24
#define  XENA_XIP037_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP037_PHASE3_MASK) >> XENA_XIP037_PHASE3_SHIFT)
#define  XENA_XIP037_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP037_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP037_PHASE3_SHIFT))

#define  XENA_XIP037_PHASE2_MASK 0xff0000
#define  XENA_XIP037_PHASE2_SHIFT 16
#define  XENA_XIP037_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP037_PHASE2_MASK) >> XENA_XIP037_PHASE2_SHIFT)
#define  XENA_XIP037_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP037_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP037_PHASE2_SHIFT))

#define  XENA_XIP037_PHASE1_MASK 0xff00
#define  XENA_XIP037_PHASE1_SHIFT 8
#define  XENA_XIP037_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP037_PHASE1_MASK) >> XENA_XIP037_PHASE1_SHIFT)
#define  XENA_XIP037_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP037_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP037_PHASE1_SHIFT))

#define  XENA_XIP037_PHASE0_MASK 0xff
#define  XENA_XIP037_PHASE0_SHIFT 0
#define  XENA_XIP037_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP037_PHASE0_MASK) >> XENA_XIP037_PHASE0_SHIFT)
#define  XENA_XIP037_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP037_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP037_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 39 of 52) (XIP038)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 39 of 52)*/
//====================================================================

#define  XENA_XIP038_PHASE3_MASK 0xff000000
#define  XENA_XIP038_PHASE3_SHIFT 24
#define  XENA_XIP038_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP038_PHASE3_MASK) >> XENA_XIP038_PHASE3_SHIFT)
#define  XENA_XIP038_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP038_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP038_PHASE3_SHIFT))

#define  XENA_XIP038_PHASE2_MASK 0xff0000
#define  XENA_XIP038_PHASE2_SHIFT 16
#define  XENA_XIP038_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP038_PHASE2_MASK) >> XENA_XIP038_PHASE2_SHIFT)
#define  XENA_XIP038_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP038_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP038_PHASE2_SHIFT))

#define  XENA_XIP038_PHASE1_MASK 0xff00
#define  XENA_XIP038_PHASE1_SHIFT 8
#define  XENA_XIP038_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP038_PHASE1_MASK) >> XENA_XIP038_PHASE1_SHIFT)
#define  XENA_XIP038_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP038_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP038_PHASE1_SHIFT))

#define  XENA_XIP038_PHASE0_MASK 0xff
#define  XENA_XIP038_PHASE0_SHIFT 0
#define  XENA_XIP038_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP038_PHASE0_MASK) >> XENA_XIP038_PHASE0_SHIFT)
#define  XENA_XIP038_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP038_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP038_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 40 of 52) (XIP039)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 40 of 52)*/
//====================================================================

#define  XENA_XIP039_PHASE3_MASK 0xff000000
#define  XENA_XIP039_PHASE3_SHIFT 24
#define  XENA_XIP039_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP039_PHASE3_MASK) >> XENA_XIP039_PHASE3_SHIFT)
#define  XENA_XIP039_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP039_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP039_PHASE3_SHIFT))

#define  XENA_XIP039_PHASE2_MASK 0xff0000
#define  XENA_XIP039_PHASE2_SHIFT 16
#define  XENA_XIP039_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP039_PHASE2_MASK) >> XENA_XIP039_PHASE2_SHIFT)
#define  XENA_XIP039_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP039_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP039_PHASE2_SHIFT))

#define  XENA_XIP039_PHASE1_MASK 0xff00
#define  XENA_XIP039_PHASE1_SHIFT 8
#define  XENA_XIP039_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP039_PHASE1_MASK) >> XENA_XIP039_PHASE1_SHIFT)
#define  XENA_XIP039_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP039_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP039_PHASE1_SHIFT))

#define  XENA_XIP039_PHASE0_MASK 0xff
#define  XENA_XIP039_PHASE0_SHIFT 0
#define  XENA_XIP039_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP039_PHASE0_MASK) >> XENA_XIP039_PHASE0_SHIFT)
#define  XENA_XIP039_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP039_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP039_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 41 of 52) (XIP040)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 41 of 52)*/
//====================================================================

#define  XENA_XIP040_PHASE3_MASK 0xff000000
#define  XENA_XIP040_PHASE3_SHIFT 24
#define  XENA_XIP040_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP040_PHASE3_MASK) >> XENA_XIP040_PHASE3_SHIFT)
#define  XENA_XIP040_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP040_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP040_PHASE3_SHIFT))

#define  XENA_XIP040_PHASE2_MASK 0xff0000
#define  XENA_XIP040_PHASE2_SHIFT 16
#define  XENA_XIP040_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP040_PHASE2_MASK) >> XENA_XIP040_PHASE2_SHIFT)
#define  XENA_XIP040_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP040_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP040_PHASE2_SHIFT))

#define  XENA_XIP040_PHASE1_MASK 0xff00
#define  XENA_XIP040_PHASE1_SHIFT 8
#define  XENA_XIP040_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP040_PHASE1_MASK) >> XENA_XIP040_PHASE1_SHIFT)
#define  XENA_XIP040_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP040_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP040_PHASE1_SHIFT))

#define  XENA_XIP040_PHASE0_MASK 0xff
#define  XENA_XIP040_PHASE0_SHIFT 0
#define  XENA_XIP040_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP040_PHASE0_MASK) >> XENA_XIP040_PHASE0_SHIFT)
#define  XENA_XIP040_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP040_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP040_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 42 of 52) (XIP041)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 42 of 52)*/
//====================================================================

#define  XENA_XIP041_PHASE3_MASK 0xff000000
#define  XENA_XIP041_PHASE3_SHIFT 24
#define  XENA_XIP041_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP041_PHASE3_MASK) >> XENA_XIP041_PHASE3_SHIFT)
#define  XENA_XIP041_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP041_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP041_PHASE3_SHIFT))

#define  XENA_XIP041_PHASE2_MASK 0xff0000
#define  XENA_XIP041_PHASE2_SHIFT 16
#define  XENA_XIP041_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP041_PHASE2_MASK) >> XENA_XIP041_PHASE2_SHIFT)
#define  XENA_XIP041_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP041_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP041_PHASE2_SHIFT))

#define  XENA_XIP041_PHASE1_MASK 0xff00
#define  XENA_XIP041_PHASE1_SHIFT 8
#define  XENA_XIP041_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP041_PHASE1_MASK) >> XENA_XIP041_PHASE1_SHIFT)
#define  XENA_XIP041_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP041_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP041_PHASE1_SHIFT))

#define  XENA_XIP041_PHASE0_MASK 0xff
#define  XENA_XIP041_PHASE0_SHIFT 0
#define  XENA_XIP041_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP041_PHASE0_MASK) >> XENA_XIP041_PHASE0_SHIFT)
#define  XENA_XIP041_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP041_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP041_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 43 of 52) (XIP042)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 43 of 52)*/
//====================================================================

#define  XENA_XIP042_PHASE3_MASK 0xff000000
#define  XENA_XIP042_PHASE3_SHIFT 24
#define  XENA_XIP042_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP042_PHASE3_MASK) >> XENA_XIP042_PHASE3_SHIFT)
#define  XENA_XIP042_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP042_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP042_PHASE3_SHIFT))

#define  XENA_XIP042_PHASE2_MASK 0xff0000
#define  XENA_XIP042_PHASE2_SHIFT 16
#define  XENA_XIP042_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP042_PHASE2_MASK) >> XENA_XIP042_PHASE2_SHIFT)
#define  XENA_XIP042_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP042_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP042_PHASE2_SHIFT))

#define  XENA_XIP042_PHASE1_MASK 0xff00
#define  XENA_XIP042_PHASE1_SHIFT 8
#define  XENA_XIP042_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP042_PHASE1_MASK) >> XENA_XIP042_PHASE1_SHIFT)
#define  XENA_XIP042_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP042_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP042_PHASE1_SHIFT))

#define  XENA_XIP042_PHASE0_MASK 0xff
#define  XENA_XIP042_PHASE0_SHIFT 0
#define  XENA_XIP042_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP042_PHASE0_MASK) >> XENA_XIP042_PHASE0_SHIFT)
#define  XENA_XIP042_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP042_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP042_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 44 of 52) (XIP043)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 44 of 52)*/
//====================================================================

#define  XENA_XIP043_PHASE3_MASK 0xff000000
#define  XENA_XIP043_PHASE3_SHIFT 24
#define  XENA_XIP043_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP043_PHASE3_MASK) >> XENA_XIP043_PHASE3_SHIFT)
#define  XENA_XIP043_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP043_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP043_PHASE3_SHIFT))

#define  XENA_XIP043_PHASE2_MASK 0xff0000
#define  XENA_XIP043_PHASE2_SHIFT 16
#define  XENA_XIP043_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP043_PHASE2_MASK) >> XENA_XIP043_PHASE2_SHIFT)
#define  XENA_XIP043_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP043_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP043_PHASE2_SHIFT))

#define  XENA_XIP043_PHASE1_MASK 0xff00
#define  XENA_XIP043_PHASE1_SHIFT 8
#define  XENA_XIP043_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP043_PHASE1_MASK) >> XENA_XIP043_PHASE1_SHIFT)
#define  XENA_XIP043_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP043_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP043_PHASE1_SHIFT))

#define  XENA_XIP043_PHASE0_MASK 0xff
#define  XENA_XIP043_PHASE0_SHIFT 0
#define  XENA_XIP043_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP043_PHASE0_MASK) >> XENA_XIP043_PHASE0_SHIFT)
#define  XENA_XIP043_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP043_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP043_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 45 of 52) (XIP044)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 45 of 52)*/
//====================================================================

#define  XENA_XIP044_PHASE3_MASK 0xff000000
#define  XENA_XIP044_PHASE3_SHIFT 24
#define  XENA_XIP044_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP044_PHASE3_MASK) >> XENA_XIP044_PHASE3_SHIFT)
#define  XENA_XIP044_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP044_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP044_PHASE3_SHIFT))

#define  XENA_XIP044_PHASE2_MASK 0xff0000
#define  XENA_XIP044_PHASE2_SHIFT 16
#define  XENA_XIP044_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP044_PHASE2_MASK) >> XENA_XIP044_PHASE2_SHIFT)
#define  XENA_XIP044_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP044_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP044_PHASE2_SHIFT))

#define  XENA_XIP044_PHASE1_MASK 0xff00
#define  XENA_XIP044_PHASE1_SHIFT 8
#define  XENA_XIP044_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP044_PHASE1_MASK) >> XENA_XIP044_PHASE1_SHIFT)
#define  XENA_XIP044_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP044_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP044_PHASE1_SHIFT))

#define  XENA_XIP044_PHASE0_MASK 0xff
#define  XENA_XIP044_PHASE0_SHIFT 0
#define  XENA_XIP044_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP044_PHASE0_MASK) >> XENA_XIP044_PHASE0_SHIFT)
#define  XENA_XIP044_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP044_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP044_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 46 of 52) (XIP045)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 46 of 52)*/
//====================================================================

#define  XENA_XIP045_PHASE3_MASK 0xff000000
#define  XENA_XIP045_PHASE3_SHIFT 24
#define  XENA_XIP045_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP045_PHASE3_MASK) >> XENA_XIP045_PHASE3_SHIFT)
#define  XENA_XIP045_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP045_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP045_PHASE3_SHIFT))

#define  XENA_XIP045_PHASE2_MASK 0xff0000
#define  XENA_XIP045_PHASE2_SHIFT 16
#define  XENA_XIP045_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP045_PHASE2_MASK) >> XENA_XIP045_PHASE2_SHIFT)
#define  XENA_XIP045_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP045_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP045_PHASE2_SHIFT))

#define  XENA_XIP045_PHASE1_MASK 0xff00
#define  XENA_XIP045_PHASE1_SHIFT 8
#define  XENA_XIP045_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP045_PHASE1_MASK) >> XENA_XIP045_PHASE1_SHIFT)
#define  XENA_XIP045_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP045_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP045_PHASE1_SHIFT))

#define  XENA_XIP045_PHASE0_MASK 0xff
#define  XENA_XIP045_PHASE0_SHIFT 0
#define  XENA_XIP045_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP045_PHASE0_MASK) >> XENA_XIP045_PHASE0_SHIFT)
#define  XENA_XIP045_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP045_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP045_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 47 of 52) (XIP046)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 47 of 52)*/
//====================================================================

#define  XENA_XIP046_PHASE3_MASK 0xff000000
#define  XENA_XIP046_PHASE3_SHIFT 24
#define  XENA_XIP046_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP046_PHASE3_MASK) >> XENA_XIP046_PHASE3_SHIFT)
#define  XENA_XIP046_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP046_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP046_PHASE3_SHIFT))

#define  XENA_XIP046_PHASE2_MASK 0xff0000
#define  XENA_XIP046_PHASE2_SHIFT 16
#define  XENA_XIP046_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP046_PHASE2_MASK) >> XENA_XIP046_PHASE2_SHIFT)
#define  XENA_XIP046_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP046_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP046_PHASE2_SHIFT))

#define  XENA_XIP046_PHASE1_MASK 0xff00
#define  XENA_XIP046_PHASE1_SHIFT 8
#define  XENA_XIP046_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP046_PHASE1_MASK) >> XENA_XIP046_PHASE1_SHIFT)
#define  XENA_XIP046_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP046_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP046_PHASE1_SHIFT))

#define  XENA_XIP046_PHASE0_MASK 0xff
#define  XENA_XIP046_PHASE0_SHIFT 0
#define  XENA_XIP046_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP046_PHASE0_MASK) >> XENA_XIP046_PHASE0_SHIFT)
#define  XENA_XIP046_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP046_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP046_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 48 of 52) (XIP047)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 48 of 52)*/
//====================================================================

#define  XENA_XIP047_PHASE3_MASK 0xff000000
#define  XENA_XIP047_PHASE3_SHIFT 24
#define  XENA_XIP047_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP047_PHASE3_MASK) >> XENA_XIP047_PHASE3_SHIFT)
#define  XENA_XIP047_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP047_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP047_PHASE3_SHIFT))

#define  XENA_XIP047_PHASE2_MASK 0xff0000
#define  XENA_XIP047_PHASE2_SHIFT 16
#define  XENA_XIP047_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP047_PHASE2_MASK) >> XENA_XIP047_PHASE2_SHIFT)
#define  XENA_XIP047_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP047_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP047_PHASE2_SHIFT))

#define  XENA_XIP047_PHASE1_MASK 0xff00
#define  XENA_XIP047_PHASE1_SHIFT 8
#define  XENA_XIP047_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP047_PHASE1_MASK) >> XENA_XIP047_PHASE1_SHIFT)
#define  XENA_XIP047_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP047_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP047_PHASE1_SHIFT))

#define  XENA_XIP047_PHASE0_MASK 0xff
#define  XENA_XIP047_PHASE0_SHIFT 0
#define  XENA_XIP047_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP047_PHASE0_MASK) >> XENA_XIP047_PHASE0_SHIFT)
#define  XENA_XIP047_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP047_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP047_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 49 of 52) (XIP048)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 49 of 52)*/
//====================================================================

#define  XENA_XIP048_PHASE3_MASK 0xff000000
#define  XENA_XIP048_PHASE3_SHIFT 24
#define  XENA_XIP048_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP048_PHASE3_MASK) >> XENA_XIP048_PHASE3_SHIFT)
#define  XENA_XIP048_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP048_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP048_PHASE3_SHIFT))

#define  XENA_XIP048_PHASE2_MASK 0xff0000
#define  XENA_XIP048_PHASE2_SHIFT 16
#define  XENA_XIP048_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP048_PHASE2_MASK) >> XENA_XIP048_PHASE2_SHIFT)
#define  XENA_XIP048_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP048_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP048_PHASE2_SHIFT))

#define  XENA_XIP048_PHASE1_MASK 0xff00
#define  XENA_XIP048_PHASE1_SHIFT 8
#define  XENA_XIP048_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP048_PHASE1_MASK) >> XENA_XIP048_PHASE1_SHIFT)
#define  XENA_XIP048_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP048_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP048_PHASE1_SHIFT))

#define  XENA_XIP048_PHASE0_MASK 0xff
#define  XENA_XIP048_PHASE0_SHIFT 0
#define  XENA_XIP048_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP048_PHASE0_MASK) >> XENA_XIP048_PHASE0_SHIFT)
#define  XENA_XIP048_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP048_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP048_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 50 of 52) (XIP049)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 50 of 52)*/
//====================================================================

#define  XENA_XIP049_PHASE3_MASK 0xff000000
#define  XENA_XIP049_PHASE3_SHIFT 24
#define  XENA_XIP049_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP049_PHASE3_MASK) >> XENA_XIP049_PHASE3_SHIFT)
#define  XENA_XIP049_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP049_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP049_PHASE3_SHIFT))

#define  XENA_XIP049_PHASE2_MASK 0xff0000
#define  XENA_XIP049_PHASE2_SHIFT 16
#define  XENA_XIP049_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP049_PHASE2_MASK) >> XENA_XIP049_PHASE2_SHIFT)
#define  XENA_XIP049_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP049_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP049_PHASE2_SHIFT))

#define  XENA_XIP049_PHASE1_MASK 0xff00
#define  XENA_XIP049_PHASE1_SHIFT 8
#define  XENA_XIP049_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP049_PHASE1_MASK) >> XENA_XIP049_PHASE1_SHIFT)
#define  XENA_XIP049_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP049_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP049_PHASE1_SHIFT))

#define  XENA_XIP049_PHASE0_MASK 0xff
#define  XENA_XIP049_PHASE0_SHIFT 0
#define  XENA_XIP049_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP049_PHASE0_MASK) >> XENA_XIP049_PHASE0_SHIFT)
#define  XENA_XIP049_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP049_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP049_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 51 of 52) (XIP050)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 51 of 52)*/
//====================================================================

#define  XENA_XIP050_PHASE3_MASK 0xff000000
#define  XENA_XIP050_PHASE3_SHIFT 24
#define  XENA_XIP050_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP050_PHASE3_MASK) >> XENA_XIP050_PHASE3_SHIFT)
#define  XENA_XIP050_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP050_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP050_PHASE3_SHIFT))

#define  XENA_XIP050_PHASE2_MASK 0xff0000
#define  XENA_XIP050_PHASE2_SHIFT 16
#define  XENA_XIP050_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP050_PHASE2_MASK) >> XENA_XIP050_PHASE2_SHIFT)
#define  XENA_XIP050_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP050_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP050_PHASE2_SHIFT))

#define  XENA_XIP050_PHASE1_MASK 0xff00
#define  XENA_XIP050_PHASE1_SHIFT 8
#define  XENA_XIP050_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP050_PHASE1_MASK) >> XENA_XIP050_PHASE1_SHIFT)
#define  XENA_XIP050_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP050_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP050_PHASE1_SHIFT))

#define  XENA_XIP050_PHASE0_MASK 0xff
#define  XENA_XIP050_PHASE0_SHIFT 0
#define  XENA_XIP050_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP050_PHASE0_MASK) >> XENA_XIP050_PHASE0_SHIFT)
#define  XENA_XIP050_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP050_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP050_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register0 (Private) (Instance 52 of 52) (XIP051)
/** \brief The UPCIO IO PWM Config Register0 contains configuration information for the first four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 52 of 52)*/
//====================================================================

#define  XENA_XIP051_PHASE3_MASK 0xff000000
#define  XENA_XIP051_PHASE3_SHIFT 24
#define  XENA_XIP051_PHASE3_MASK_SHIFT(reg) (((reg) & XENA_XIP051_PHASE3_MASK) >> XENA_XIP051_PHASE3_SHIFT)
#define  XENA_XIP051_PHASE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP051_PHASE3_MASK) | (((uint32_t)val) << XENA_XIP051_PHASE3_SHIFT))

#define  XENA_XIP051_PHASE2_MASK 0xff0000
#define  XENA_XIP051_PHASE2_SHIFT 16
#define  XENA_XIP051_PHASE2_MASK_SHIFT(reg) (((reg) & XENA_XIP051_PHASE2_MASK) >> XENA_XIP051_PHASE2_SHIFT)
#define  XENA_XIP051_PHASE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP051_PHASE2_MASK) | (((uint32_t)val) << XENA_XIP051_PHASE2_SHIFT))

#define  XENA_XIP051_PHASE1_MASK 0xff00
#define  XENA_XIP051_PHASE1_SHIFT 8
#define  XENA_XIP051_PHASE1_MASK_SHIFT(reg) (((reg) & XENA_XIP051_PHASE1_MASK) >> XENA_XIP051_PHASE1_SHIFT)
#define  XENA_XIP051_PHASE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP051_PHASE1_MASK) | (((uint32_t)val) << XENA_XIP051_PHASE1_SHIFT))

#define  XENA_XIP051_PHASE0_MASK 0xff
#define  XENA_XIP051_PHASE0_SHIFT 0
#define  XENA_XIP051_PHASE0_MASK_SHIFT(reg) (((reg) & XENA_XIP051_PHASE0_MASK) >> XENA_XIP051_PHASE0_SHIFT)
#define  XENA_XIP051_PHASE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP051_PHASE0_MASK) | (((uint32_t)val) << XENA_XIP051_PHASE0_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 1 of 52) (XIP10)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 1 of 52)*/
//====================================================================

#define  XENA_XIP10_PHASE7_MASK 0xff000000
#define  XENA_XIP10_PHASE7_SHIFT 24
#define  XENA_XIP10_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP10_PHASE7_MASK) >> XENA_XIP10_PHASE7_SHIFT)
#define  XENA_XIP10_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP10_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP10_PHASE7_SHIFT))

#define  XENA_XIP10_PHASE6_MASK 0xff0000
#define  XENA_XIP10_PHASE6_SHIFT 16
#define  XENA_XIP10_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP10_PHASE6_MASK) >> XENA_XIP10_PHASE6_SHIFT)
#define  XENA_XIP10_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP10_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP10_PHASE6_SHIFT))

#define  XENA_XIP10_PHASE5_MASK 0xff00
#define  XENA_XIP10_PHASE5_SHIFT 8
#define  XENA_XIP10_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP10_PHASE5_MASK) >> XENA_XIP10_PHASE5_SHIFT)
#define  XENA_XIP10_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP10_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP10_PHASE5_SHIFT))

#define  XENA_XIP10_PHASE4_MASK 0xff
#define  XENA_XIP10_PHASE4_SHIFT 0
#define  XENA_XIP10_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP10_PHASE4_MASK) >> XENA_XIP10_PHASE4_SHIFT)
#define  XENA_XIP10_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP10_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP10_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 2 of 52) (XIP11)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 2 of 52)*/
//====================================================================

#define  XENA_XIP11_PHASE7_MASK 0xff000000
#define  XENA_XIP11_PHASE7_SHIFT 24
#define  XENA_XIP11_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP11_PHASE7_MASK) >> XENA_XIP11_PHASE7_SHIFT)
#define  XENA_XIP11_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP11_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP11_PHASE7_SHIFT))

#define  XENA_XIP11_PHASE6_MASK 0xff0000
#define  XENA_XIP11_PHASE6_SHIFT 16
#define  XENA_XIP11_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP11_PHASE6_MASK) >> XENA_XIP11_PHASE6_SHIFT)
#define  XENA_XIP11_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP11_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP11_PHASE6_SHIFT))

#define  XENA_XIP11_PHASE5_MASK 0xff00
#define  XENA_XIP11_PHASE5_SHIFT 8
#define  XENA_XIP11_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP11_PHASE5_MASK) >> XENA_XIP11_PHASE5_SHIFT)
#define  XENA_XIP11_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP11_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP11_PHASE5_SHIFT))

#define  XENA_XIP11_PHASE4_MASK 0xff
#define  XENA_XIP11_PHASE4_SHIFT 0
#define  XENA_XIP11_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP11_PHASE4_MASK) >> XENA_XIP11_PHASE4_SHIFT)
#define  XENA_XIP11_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP11_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP11_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 3 of 52) (XIP12)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 3 of 52)*/
//====================================================================

#define  XENA_XIP12_PHASE7_MASK 0xff000000
#define  XENA_XIP12_PHASE7_SHIFT 24
#define  XENA_XIP12_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP12_PHASE7_MASK) >> XENA_XIP12_PHASE7_SHIFT)
#define  XENA_XIP12_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP12_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP12_PHASE7_SHIFT))

#define  XENA_XIP12_PHASE6_MASK 0xff0000
#define  XENA_XIP12_PHASE6_SHIFT 16
#define  XENA_XIP12_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP12_PHASE6_MASK) >> XENA_XIP12_PHASE6_SHIFT)
#define  XENA_XIP12_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP12_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP12_PHASE6_SHIFT))

#define  XENA_XIP12_PHASE5_MASK 0xff00
#define  XENA_XIP12_PHASE5_SHIFT 8
#define  XENA_XIP12_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP12_PHASE5_MASK) >> XENA_XIP12_PHASE5_SHIFT)
#define  XENA_XIP12_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP12_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP12_PHASE5_SHIFT))

#define  XENA_XIP12_PHASE4_MASK 0xff
#define  XENA_XIP12_PHASE4_SHIFT 0
#define  XENA_XIP12_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP12_PHASE4_MASK) >> XENA_XIP12_PHASE4_SHIFT)
#define  XENA_XIP12_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP12_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP12_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 4 of 52) (XIP13)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 4 of 52)*/
//====================================================================

#define  XENA_XIP13_PHASE7_MASK 0xff000000
#define  XENA_XIP13_PHASE7_SHIFT 24
#define  XENA_XIP13_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP13_PHASE7_MASK) >> XENA_XIP13_PHASE7_SHIFT)
#define  XENA_XIP13_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP13_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP13_PHASE7_SHIFT))

#define  XENA_XIP13_PHASE6_MASK 0xff0000
#define  XENA_XIP13_PHASE6_SHIFT 16
#define  XENA_XIP13_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP13_PHASE6_MASK) >> XENA_XIP13_PHASE6_SHIFT)
#define  XENA_XIP13_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP13_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP13_PHASE6_SHIFT))

#define  XENA_XIP13_PHASE5_MASK 0xff00
#define  XENA_XIP13_PHASE5_SHIFT 8
#define  XENA_XIP13_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP13_PHASE5_MASK) >> XENA_XIP13_PHASE5_SHIFT)
#define  XENA_XIP13_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP13_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP13_PHASE5_SHIFT))

#define  XENA_XIP13_PHASE4_MASK 0xff
#define  XENA_XIP13_PHASE4_SHIFT 0
#define  XENA_XIP13_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP13_PHASE4_MASK) >> XENA_XIP13_PHASE4_SHIFT)
#define  XENA_XIP13_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP13_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP13_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 5 of 52) (XIP14)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 5 of 52)*/
//====================================================================

#define  XENA_XIP14_PHASE7_MASK 0xff000000
#define  XENA_XIP14_PHASE7_SHIFT 24
#define  XENA_XIP14_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP14_PHASE7_MASK) >> XENA_XIP14_PHASE7_SHIFT)
#define  XENA_XIP14_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP14_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP14_PHASE7_SHIFT))

#define  XENA_XIP14_PHASE6_MASK 0xff0000
#define  XENA_XIP14_PHASE6_SHIFT 16
#define  XENA_XIP14_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP14_PHASE6_MASK) >> XENA_XIP14_PHASE6_SHIFT)
#define  XENA_XIP14_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP14_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP14_PHASE6_SHIFT))

#define  XENA_XIP14_PHASE5_MASK 0xff00
#define  XENA_XIP14_PHASE5_SHIFT 8
#define  XENA_XIP14_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP14_PHASE5_MASK) >> XENA_XIP14_PHASE5_SHIFT)
#define  XENA_XIP14_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP14_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP14_PHASE5_SHIFT))

#define  XENA_XIP14_PHASE4_MASK 0xff
#define  XENA_XIP14_PHASE4_SHIFT 0
#define  XENA_XIP14_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP14_PHASE4_MASK) >> XENA_XIP14_PHASE4_SHIFT)
#define  XENA_XIP14_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP14_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP14_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 6 of 52) (XIP15)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 6 of 52)*/
//====================================================================

#define  XENA_XIP15_PHASE7_MASK 0xff000000
#define  XENA_XIP15_PHASE7_SHIFT 24
#define  XENA_XIP15_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP15_PHASE7_MASK) >> XENA_XIP15_PHASE7_SHIFT)
#define  XENA_XIP15_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP15_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP15_PHASE7_SHIFT))

#define  XENA_XIP15_PHASE6_MASK 0xff0000
#define  XENA_XIP15_PHASE6_SHIFT 16
#define  XENA_XIP15_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP15_PHASE6_MASK) >> XENA_XIP15_PHASE6_SHIFT)
#define  XENA_XIP15_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP15_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP15_PHASE6_SHIFT))

#define  XENA_XIP15_PHASE5_MASK 0xff00
#define  XENA_XIP15_PHASE5_SHIFT 8
#define  XENA_XIP15_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP15_PHASE5_MASK) >> XENA_XIP15_PHASE5_SHIFT)
#define  XENA_XIP15_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP15_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP15_PHASE5_SHIFT))

#define  XENA_XIP15_PHASE4_MASK 0xff
#define  XENA_XIP15_PHASE4_SHIFT 0
#define  XENA_XIP15_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP15_PHASE4_MASK) >> XENA_XIP15_PHASE4_SHIFT)
#define  XENA_XIP15_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP15_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP15_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 7 of 52) (XIP16)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 7 of 52)*/
//====================================================================

#define  XENA_XIP16_PHASE7_MASK 0xff000000
#define  XENA_XIP16_PHASE7_SHIFT 24
#define  XENA_XIP16_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP16_PHASE7_MASK) >> XENA_XIP16_PHASE7_SHIFT)
#define  XENA_XIP16_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP16_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP16_PHASE7_SHIFT))

#define  XENA_XIP16_PHASE6_MASK 0xff0000
#define  XENA_XIP16_PHASE6_SHIFT 16
#define  XENA_XIP16_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP16_PHASE6_MASK) >> XENA_XIP16_PHASE6_SHIFT)
#define  XENA_XIP16_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP16_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP16_PHASE6_SHIFT))

#define  XENA_XIP16_PHASE5_MASK 0xff00
#define  XENA_XIP16_PHASE5_SHIFT 8
#define  XENA_XIP16_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP16_PHASE5_MASK) >> XENA_XIP16_PHASE5_SHIFT)
#define  XENA_XIP16_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP16_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP16_PHASE5_SHIFT))

#define  XENA_XIP16_PHASE4_MASK 0xff
#define  XENA_XIP16_PHASE4_SHIFT 0
#define  XENA_XIP16_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP16_PHASE4_MASK) >> XENA_XIP16_PHASE4_SHIFT)
#define  XENA_XIP16_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP16_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP16_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 8 of 52) (XIP17)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 8 of 52)*/
//====================================================================

#define  XENA_XIP17_PHASE7_MASK 0xff000000
#define  XENA_XIP17_PHASE7_SHIFT 24
#define  XENA_XIP17_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP17_PHASE7_MASK) >> XENA_XIP17_PHASE7_SHIFT)
#define  XENA_XIP17_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP17_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP17_PHASE7_SHIFT))

#define  XENA_XIP17_PHASE6_MASK 0xff0000
#define  XENA_XIP17_PHASE6_SHIFT 16
#define  XENA_XIP17_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP17_PHASE6_MASK) >> XENA_XIP17_PHASE6_SHIFT)
#define  XENA_XIP17_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP17_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP17_PHASE6_SHIFT))

#define  XENA_XIP17_PHASE5_MASK 0xff00
#define  XENA_XIP17_PHASE5_SHIFT 8
#define  XENA_XIP17_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP17_PHASE5_MASK) >> XENA_XIP17_PHASE5_SHIFT)
#define  XENA_XIP17_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP17_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP17_PHASE5_SHIFT))

#define  XENA_XIP17_PHASE4_MASK 0xff
#define  XENA_XIP17_PHASE4_SHIFT 0
#define  XENA_XIP17_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP17_PHASE4_MASK) >> XENA_XIP17_PHASE4_SHIFT)
#define  XENA_XIP17_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP17_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP17_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 9 of 52) (XIP18)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 9 of 52)*/
//====================================================================

#define  XENA_XIP18_PHASE7_MASK 0xff000000
#define  XENA_XIP18_PHASE7_SHIFT 24
#define  XENA_XIP18_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP18_PHASE7_MASK) >> XENA_XIP18_PHASE7_SHIFT)
#define  XENA_XIP18_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP18_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP18_PHASE7_SHIFT))

#define  XENA_XIP18_PHASE6_MASK 0xff0000
#define  XENA_XIP18_PHASE6_SHIFT 16
#define  XENA_XIP18_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP18_PHASE6_MASK) >> XENA_XIP18_PHASE6_SHIFT)
#define  XENA_XIP18_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP18_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP18_PHASE6_SHIFT))

#define  XENA_XIP18_PHASE5_MASK 0xff00
#define  XENA_XIP18_PHASE5_SHIFT 8
#define  XENA_XIP18_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP18_PHASE5_MASK) >> XENA_XIP18_PHASE5_SHIFT)
#define  XENA_XIP18_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP18_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP18_PHASE5_SHIFT))

#define  XENA_XIP18_PHASE4_MASK 0xff
#define  XENA_XIP18_PHASE4_SHIFT 0
#define  XENA_XIP18_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP18_PHASE4_MASK) >> XENA_XIP18_PHASE4_SHIFT)
#define  XENA_XIP18_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP18_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP18_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 10 of 52) (XIP19)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 10 of 52)*/
//====================================================================

#define  XENA_XIP19_PHASE7_MASK 0xff000000
#define  XENA_XIP19_PHASE7_SHIFT 24
#define  XENA_XIP19_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP19_PHASE7_MASK) >> XENA_XIP19_PHASE7_SHIFT)
#define  XENA_XIP19_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP19_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP19_PHASE7_SHIFT))

#define  XENA_XIP19_PHASE6_MASK 0xff0000
#define  XENA_XIP19_PHASE6_SHIFT 16
#define  XENA_XIP19_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP19_PHASE6_MASK) >> XENA_XIP19_PHASE6_SHIFT)
#define  XENA_XIP19_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP19_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP19_PHASE6_SHIFT))

#define  XENA_XIP19_PHASE5_MASK 0xff00
#define  XENA_XIP19_PHASE5_SHIFT 8
#define  XENA_XIP19_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP19_PHASE5_MASK) >> XENA_XIP19_PHASE5_SHIFT)
#define  XENA_XIP19_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP19_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP19_PHASE5_SHIFT))

#define  XENA_XIP19_PHASE4_MASK 0xff
#define  XENA_XIP19_PHASE4_SHIFT 0
#define  XENA_XIP19_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP19_PHASE4_MASK) >> XENA_XIP19_PHASE4_SHIFT)
#define  XENA_XIP19_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP19_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP19_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 11 of 52) (XIP110)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 11 of 52)*/
//====================================================================

#define  XENA_XIP110_PHASE7_MASK 0xff000000
#define  XENA_XIP110_PHASE7_SHIFT 24
#define  XENA_XIP110_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP110_PHASE7_MASK) >> XENA_XIP110_PHASE7_SHIFT)
#define  XENA_XIP110_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP110_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP110_PHASE7_SHIFT))

#define  XENA_XIP110_PHASE6_MASK 0xff0000
#define  XENA_XIP110_PHASE6_SHIFT 16
#define  XENA_XIP110_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP110_PHASE6_MASK) >> XENA_XIP110_PHASE6_SHIFT)
#define  XENA_XIP110_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP110_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP110_PHASE6_SHIFT))

#define  XENA_XIP110_PHASE5_MASK 0xff00
#define  XENA_XIP110_PHASE5_SHIFT 8
#define  XENA_XIP110_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP110_PHASE5_MASK) >> XENA_XIP110_PHASE5_SHIFT)
#define  XENA_XIP110_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP110_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP110_PHASE5_SHIFT))

#define  XENA_XIP110_PHASE4_MASK 0xff
#define  XENA_XIP110_PHASE4_SHIFT 0
#define  XENA_XIP110_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP110_PHASE4_MASK) >> XENA_XIP110_PHASE4_SHIFT)
#define  XENA_XIP110_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP110_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP110_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 12 of 52) (XIP111)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 12 of 52)*/
//====================================================================

#define  XENA_XIP111_PHASE7_MASK 0xff000000
#define  XENA_XIP111_PHASE7_SHIFT 24
#define  XENA_XIP111_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP111_PHASE7_MASK) >> XENA_XIP111_PHASE7_SHIFT)
#define  XENA_XIP111_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP111_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP111_PHASE7_SHIFT))

#define  XENA_XIP111_PHASE6_MASK 0xff0000
#define  XENA_XIP111_PHASE6_SHIFT 16
#define  XENA_XIP111_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP111_PHASE6_MASK) >> XENA_XIP111_PHASE6_SHIFT)
#define  XENA_XIP111_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP111_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP111_PHASE6_SHIFT))

#define  XENA_XIP111_PHASE5_MASK 0xff00
#define  XENA_XIP111_PHASE5_SHIFT 8
#define  XENA_XIP111_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP111_PHASE5_MASK) >> XENA_XIP111_PHASE5_SHIFT)
#define  XENA_XIP111_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP111_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP111_PHASE5_SHIFT))

#define  XENA_XIP111_PHASE4_MASK 0xff
#define  XENA_XIP111_PHASE4_SHIFT 0
#define  XENA_XIP111_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP111_PHASE4_MASK) >> XENA_XIP111_PHASE4_SHIFT)
#define  XENA_XIP111_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP111_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP111_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 13 of 52) (XIP112)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 13 of 52)*/
//====================================================================

#define  XENA_XIP112_PHASE7_MASK 0xff000000
#define  XENA_XIP112_PHASE7_SHIFT 24
#define  XENA_XIP112_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP112_PHASE7_MASK) >> XENA_XIP112_PHASE7_SHIFT)
#define  XENA_XIP112_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP112_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP112_PHASE7_SHIFT))

#define  XENA_XIP112_PHASE6_MASK 0xff0000
#define  XENA_XIP112_PHASE6_SHIFT 16
#define  XENA_XIP112_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP112_PHASE6_MASK) >> XENA_XIP112_PHASE6_SHIFT)
#define  XENA_XIP112_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP112_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP112_PHASE6_SHIFT))

#define  XENA_XIP112_PHASE5_MASK 0xff00
#define  XENA_XIP112_PHASE5_SHIFT 8
#define  XENA_XIP112_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP112_PHASE5_MASK) >> XENA_XIP112_PHASE5_SHIFT)
#define  XENA_XIP112_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP112_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP112_PHASE5_SHIFT))

#define  XENA_XIP112_PHASE4_MASK 0xff
#define  XENA_XIP112_PHASE4_SHIFT 0
#define  XENA_XIP112_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP112_PHASE4_MASK) >> XENA_XIP112_PHASE4_SHIFT)
#define  XENA_XIP112_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP112_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP112_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 14 of 52) (XIP113)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 14 of 52)*/
//====================================================================

#define  XENA_XIP113_PHASE7_MASK 0xff000000
#define  XENA_XIP113_PHASE7_SHIFT 24
#define  XENA_XIP113_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP113_PHASE7_MASK) >> XENA_XIP113_PHASE7_SHIFT)
#define  XENA_XIP113_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP113_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP113_PHASE7_SHIFT))

#define  XENA_XIP113_PHASE6_MASK 0xff0000
#define  XENA_XIP113_PHASE6_SHIFT 16
#define  XENA_XIP113_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP113_PHASE6_MASK) >> XENA_XIP113_PHASE6_SHIFT)
#define  XENA_XIP113_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP113_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP113_PHASE6_SHIFT))

#define  XENA_XIP113_PHASE5_MASK 0xff00
#define  XENA_XIP113_PHASE5_SHIFT 8
#define  XENA_XIP113_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP113_PHASE5_MASK) >> XENA_XIP113_PHASE5_SHIFT)
#define  XENA_XIP113_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP113_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP113_PHASE5_SHIFT))

#define  XENA_XIP113_PHASE4_MASK 0xff
#define  XENA_XIP113_PHASE4_SHIFT 0
#define  XENA_XIP113_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP113_PHASE4_MASK) >> XENA_XIP113_PHASE4_SHIFT)
#define  XENA_XIP113_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP113_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP113_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 15 of 52) (XIP114)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 15 of 52)*/
//====================================================================

#define  XENA_XIP114_PHASE7_MASK 0xff000000
#define  XENA_XIP114_PHASE7_SHIFT 24
#define  XENA_XIP114_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP114_PHASE7_MASK) >> XENA_XIP114_PHASE7_SHIFT)
#define  XENA_XIP114_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP114_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP114_PHASE7_SHIFT))

#define  XENA_XIP114_PHASE6_MASK 0xff0000
#define  XENA_XIP114_PHASE6_SHIFT 16
#define  XENA_XIP114_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP114_PHASE6_MASK) >> XENA_XIP114_PHASE6_SHIFT)
#define  XENA_XIP114_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP114_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP114_PHASE6_SHIFT))

#define  XENA_XIP114_PHASE5_MASK 0xff00
#define  XENA_XIP114_PHASE5_SHIFT 8
#define  XENA_XIP114_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP114_PHASE5_MASK) >> XENA_XIP114_PHASE5_SHIFT)
#define  XENA_XIP114_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP114_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP114_PHASE5_SHIFT))

#define  XENA_XIP114_PHASE4_MASK 0xff
#define  XENA_XIP114_PHASE4_SHIFT 0
#define  XENA_XIP114_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP114_PHASE4_MASK) >> XENA_XIP114_PHASE4_SHIFT)
#define  XENA_XIP114_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP114_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP114_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 16 of 52) (XIP115)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 16 of 52)*/
//====================================================================

#define  XENA_XIP115_PHASE7_MASK 0xff000000
#define  XENA_XIP115_PHASE7_SHIFT 24
#define  XENA_XIP115_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP115_PHASE7_MASK) >> XENA_XIP115_PHASE7_SHIFT)
#define  XENA_XIP115_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP115_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP115_PHASE7_SHIFT))

#define  XENA_XIP115_PHASE6_MASK 0xff0000
#define  XENA_XIP115_PHASE6_SHIFT 16
#define  XENA_XIP115_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP115_PHASE6_MASK) >> XENA_XIP115_PHASE6_SHIFT)
#define  XENA_XIP115_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP115_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP115_PHASE6_SHIFT))

#define  XENA_XIP115_PHASE5_MASK 0xff00
#define  XENA_XIP115_PHASE5_SHIFT 8
#define  XENA_XIP115_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP115_PHASE5_MASK) >> XENA_XIP115_PHASE5_SHIFT)
#define  XENA_XIP115_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP115_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP115_PHASE5_SHIFT))

#define  XENA_XIP115_PHASE4_MASK 0xff
#define  XENA_XIP115_PHASE4_SHIFT 0
#define  XENA_XIP115_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP115_PHASE4_MASK) >> XENA_XIP115_PHASE4_SHIFT)
#define  XENA_XIP115_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP115_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP115_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 17 of 52) (XIP116)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 17 of 52)*/
//====================================================================

#define  XENA_XIP116_PHASE7_MASK 0xff000000
#define  XENA_XIP116_PHASE7_SHIFT 24
#define  XENA_XIP116_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP116_PHASE7_MASK) >> XENA_XIP116_PHASE7_SHIFT)
#define  XENA_XIP116_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP116_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP116_PHASE7_SHIFT))

#define  XENA_XIP116_PHASE6_MASK 0xff0000
#define  XENA_XIP116_PHASE6_SHIFT 16
#define  XENA_XIP116_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP116_PHASE6_MASK) >> XENA_XIP116_PHASE6_SHIFT)
#define  XENA_XIP116_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP116_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP116_PHASE6_SHIFT))

#define  XENA_XIP116_PHASE5_MASK 0xff00
#define  XENA_XIP116_PHASE5_SHIFT 8
#define  XENA_XIP116_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP116_PHASE5_MASK) >> XENA_XIP116_PHASE5_SHIFT)
#define  XENA_XIP116_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP116_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP116_PHASE5_SHIFT))

#define  XENA_XIP116_PHASE4_MASK 0xff
#define  XENA_XIP116_PHASE4_SHIFT 0
#define  XENA_XIP116_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP116_PHASE4_MASK) >> XENA_XIP116_PHASE4_SHIFT)
#define  XENA_XIP116_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP116_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP116_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 18 of 52) (XIP117)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 18 of 52)*/
//====================================================================

#define  XENA_XIP117_PHASE7_MASK 0xff000000
#define  XENA_XIP117_PHASE7_SHIFT 24
#define  XENA_XIP117_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP117_PHASE7_MASK) >> XENA_XIP117_PHASE7_SHIFT)
#define  XENA_XIP117_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP117_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP117_PHASE7_SHIFT))

#define  XENA_XIP117_PHASE6_MASK 0xff0000
#define  XENA_XIP117_PHASE6_SHIFT 16
#define  XENA_XIP117_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP117_PHASE6_MASK) >> XENA_XIP117_PHASE6_SHIFT)
#define  XENA_XIP117_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP117_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP117_PHASE6_SHIFT))

#define  XENA_XIP117_PHASE5_MASK 0xff00
#define  XENA_XIP117_PHASE5_SHIFT 8
#define  XENA_XIP117_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP117_PHASE5_MASK) >> XENA_XIP117_PHASE5_SHIFT)
#define  XENA_XIP117_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP117_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP117_PHASE5_SHIFT))

#define  XENA_XIP117_PHASE4_MASK 0xff
#define  XENA_XIP117_PHASE4_SHIFT 0
#define  XENA_XIP117_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP117_PHASE4_MASK) >> XENA_XIP117_PHASE4_SHIFT)
#define  XENA_XIP117_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP117_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP117_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 19 of 52) (XIP118)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 19 of 52)*/
//====================================================================

#define  XENA_XIP118_PHASE7_MASK 0xff000000
#define  XENA_XIP118_PHASE7_SHIFT 24
#define  XENA_XIP118_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP118_PHASE7_MASK) >> XENA_XIP118_PHASE7_SHIFT)
#define  XENA_XIP118_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP118_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP118_PHASE7_SHIFT))

#define  XENA_XIP118_PHASE6_MASK 0xff0000
#define  XENA_XIP118_PHASE6_SHIFT 16
#define  XENA_XIP118_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP118_PHASE6_MASK) >> XENA_XIP118_PHASE6_SHIFT)
#define  XENA_XIP118_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP118_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP118_PHASE6_SHIFT))

#define  XENA_XIP118_PHASE5_MASK 0xff00
#define  XENA_XIP118_PHASE5_SHIFT 8
#define  XENA_XIP118_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP118_PHASE5_MASK) >> XENA_XIP118_PHASE5_SHIFT)
#define  XENA_XIP118_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP118_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP118_PHASE5_SHIFT))

#define  XENA_XIP118_PHASE4_MASK 0xff
#define  XENA_XIP118_PHASE4_SHIFT 0
#define  XENA_XIP118_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP118_PHASE4_MASK) >> XENA_XIP118_PHASE4_SHIFT)
#define  XENA_XIP118_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP118_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP118_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 20 of 52) (XIP119)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 20 of 52)*/
//====================================================================

#define  XENA_XIP119_PHASE7_MASK 0xff000000
#define  XENA_XIP119_PHASE7_SHIFT 24
#define  XENA_XIP119_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP119_PHASE7_MASK) >> XENA_XIP119_PHASE7_SHIFT)
#define  XENA_XIP119_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP119_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP119_PHASE7_SHIFT))

#define  XENA_XIP119_PHASE6_MASK 0xff0000
#define  XENA_XIP119_PHASE6_SHIFT 16
#define  XENA_XIP119_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP119_PHASE6_MASK) >> XENA_XIP119_PHASE6_SHIFT)
#define  XENA_XIP119_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP119_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP119_PHASE6_SHIFT))

#define  XENA_XIP119_PHASE5_MASK 0xff00
#define  XENA_XIP119_PHASE5_SHIFT 8
#define  XENA_XIP119_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP119_PHASE5_MASK) >> XENA_XIP119_PHASE5_SHIFT)
#define  XENA_XIP119_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP119_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP119_PHASE5_SHIFT))

#define  XENA_XIP119_PHASE4_MASK 0xff
#define  XENA_XIP119_PHASE4_SHIFT 0
#define  XENA_XIP119_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP119_PHASE4_MASK) >> XENA_XIP119_PHASE4_SHIFT)
#define  XENA_XIP119_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP119_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP119_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 21 of 52) (XIP120)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 21 of 52)*/
//====================================================================

#define  XENA_XIP120_PHASE7_MASK 0xff000000
#define  XENA_XIP120_PHASE7_SHIFT 24
#define  XENA_XIP120_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP120_PHASE7_MASK) >> XENA_XIP120_PHASE7_SHIFT)
#define  XENA_XIP120_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP120_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP120_PHASE7_SHIFT))

#define  XENA_XIP120_PHASE6_MASK 0xff0000
#define  XENA_XIP120_PHASE6_SHIFT 16
#define  XENA_XIP120_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP120_PHASE6_MASK) >> XENA_XIP120_PHASE6_SHIFT)
#define  XENA_XIP120_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP120_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP120_PHASE6_SHIFT))

#define  XENA_XIP120_PHASE5_MASK 0xff00
#define  XENA_XIP120_PHASE5_SHIFT 8
#define  XENA_XIP120_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP120_PHASE5_MASK) >> XENA_XIP120_PHASE5_SHIFT)
#define  XENA_XIP120_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP120_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP120_PHASE5_SHIFT))

#define  XENA_XIP120_PHASE4_MASK 0xff
#define  XENA_XIP120_PHASE4_SHIFT 0
#define  XENA_XIP120_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP120_PHASE4_MASK) >> XENA_XIP120_PHASE4_SHIFT)
#define  XENA_XIP120_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP120_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP120_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 22 of 52) (XIP121)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 22 of 52)*/
//====================================================================

#define  XENA_XIP121_PHASE7_MASK 0xff000000
#define  XENA_XIP121_PHASE7_SHIFT 24
#define  XENA_XIP121_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP121_PHASE7_MASK) >> XENA_XIP121_PHASE7_SHIFT)
#define  XENA_XIP121_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP121_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP121_PHASE7_SHIFT))

#define  XENA_XIP121_PHASE6_MASK 0xff0000
#define  XENA_XIP121_PHASE6_SHIFT 16
#define  XENA_XIP121_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP121_PHASE6_MASK) >> XENA_XIP121_PHASE6_SHIFT)
#define  XENA_XIP121_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP121_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP121_PHASE6_SHIFT))

#define  XENA_XIP121_PHASE5_MASK 0xff00
#define  XENA_XIP121_PHASE5_SHIFT 8
#define  XENA_XIP121_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP121_PHASE5_MASK) >> XENA_XIP121_PHASE5_SHIFT)
#define  XENA_XIP121_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP121_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP121_PHASE5_SHIFT))

#define  XENA_XIP121_PHASE4_MASK 0xff
#define  XENA_XIP121_PHASE4_SHIFT 0
#define  XENA_XIP121_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP121_PHASE4_MASK) >> XENA_XIP121_PHASE4_SHIFT)
#define  XENA_XIP121_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP121_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP121_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 23 of 52) (XIP122)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 23 of 52)*/
//====================================================================

#define  XENA_XIP122_PHASE7_MASK 0xff000000
#define  XENA_XIP122_PHASE7_SHIFT 24
#define  XENA_XIP122_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP122_PHASE7_MASK) >> XENA_XIP122_PHASE7_SHIFT)
#define  XENA_XIP122_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP122_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP122_PHASE7_SHIFT))

#define  XENA_XIP122_PHASE6_MASK 0xff0000
#define  XENA_XIP122_PHASE6_SHIFT 16
#define  XENA_XIP122_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP122_PHASE6_MASK) >> XENA_XIP122_PHASE6_SHIFT)
#define  XENA_XIP122_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP122_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP122_PHASE6_SHIFT))

#define  XENA_XIP122_PHASE5_MASK 0xff00
#define  XENA_XIP122_PHASE5_SHIFT 8
#define  XENA_XIP122_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP122_PHASE5_MASK) >> XENA_XIP122_PHASE5_SHIFT)
#define  XENA_XIP122_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP122_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP122_PHASE5_SHIFT))

#define  XENA_XIP122_PHASE4_MASK 0xff
#define  XENA_XIP122_PHASE4_SHIFT 0
#define  XENA_XIP122_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP122_PHASE4_MASK) >> XENA_XIP122_PHASE4_SHIFT)
#define  XENA_XIP122_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP122_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP122_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 24 of 52) (XIP123)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 24 of 52)*/
//====================================================================

#define  XENA_XIP123_PHASE7_MASK 0xff000000
#define  XENA_XIP123_PHASE7_SHIFT 24
#define  XENA_XIP123_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP123_PHASE7_MASK) >> XENA_XIP123_PHASE7_SHIFT)
#define  XENA_XIP123_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP123_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP123_PHASE7_SHIFT))

#define  XENA_XIP123_PHASE6_MASK 0xff0000
#define  XENA_XIP123_PHASE6_SHIFT 16
#define  XENA_XIP123_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP123_PHASE6_MASK) >> XENA_XIP123_PHASE6_SHIFT)
#define  XENA_XIP123_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP123_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP123_PHASE6_SHIFT))

#define  XENA_XIP123_PHASE5_MASK 0xff00
#define  XENA_XIP123_PHASE5_SHIFT 8
#define  XENA_XIP123_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP123_PHASE5_MASK) >> XENA_XIP123_PHASE5_SHIFT)
#define  XENA_XIP123_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP123_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP123_PHASE5_SHIFT))

#define  XENA_XIP123_PHASE4_MASK 0xff
#define  XENA_XIP123_PHASE4_SHIFT 0
#define  XENA_XIP123_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP123_PHASE4_MASK) >> XENA_XIP123_PHASE4_SHIFT)
#define  XENA_XIP123_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP123_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP123_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 25 of 52) (XIP124)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 25 of 52)*/
//====================================================================

#define  XENA_XIP124_PHASE7_MASK 0xff000000
#define  XENA_XIP124_PHASE7_SHIFT 24
#define  XENA_XIP124_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP124_PHASE7_MASK) >> XENA_XIP124_PHASE7_SHIFT)
#define  XENA_XIP124_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP124_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP124_PHASE7_SHIFT))

#define  XENA_XIP124_PHASE6_MASK 0xff0000
#define  XENA_XIP124_PHASE6_SHIFT 16
#define  XENA_XIP124_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP124_PHASE6_MASK) >> XENA_XIP124_PHASE6_SHIFT)
#define  XENA_XIP124_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP124_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP124_PHASE6_SHIFT))

#define  XENA_XIP124_PHASE5_MASK 0xff00
#define  XENA_XIP124_PHASE5_SHIFT 8
#define  XENA_XIP124_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP124_PHASE5_MASK) >> XENA_XIP124_PHASE5_SHIFT)
#define  XENA_XIP124_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP124_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP124_PHASE5_SHIFT))

#define  XENA_XIP124_PHASE4_MASK 0xff
#define  XENA_XIP124_PHASE4_SHIFT 0
#define  XENA_XIP124_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP124_PHASE4_MASK) >> XENA_XIP124_PHASE4_SHIFT)
#define  XENA_XIP124_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP124_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP124_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 26 of 52) (XIP125)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 26 of 52)*/
//====================================================================

#define  XENA_XIP125_PHASE7_MASK 0xff000000
#define  XENA_XIP125_PHASE7_SHIFT 24
#define  XENA_XIP125_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP125_PHASE7_MASK) >> XENA_XIP125_PHASE7_SHIFT)
#define  XENA_XIP125_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP125_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP125_PHASE7_SHIFT))

#define  XENA_XIP125_PHASE6_MASK 0xff0000
#define  XENA_XIP125_PHASE6_SHIFT 16
#define  XENA_XIP125_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP125_PHASE6_MASK) >> XENA_XIP125_PHASE6_SHIFT)
#define  XENA_XIP125_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP125_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP125_PHASE6_SHIFT))

#define  XENA_XIP125_PHASE5_MASK 0xff00
#define  XENA_XIP125_PHASE5_SHIFT 8
#define  XENA_XIP125_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP125_PHASE5_MASK) >> XENA_XIP125_PHASE5_SHIFT)
#define  XENA_XIP125_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP125_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP125_PHASE5_SHIFT))

#define  XENA_XIP125_PHASE4_MASK 0xff
#define  XENA_XIP125_PHASE4_SHIFT 0
#define  XENA_XIP125_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP125_PHASE4_MASK) >> XENA_XIP125_PHASE4_SHIFT)
#define  XENA_XIP125_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP125_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP125_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 27 of 52) (XIP126)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 27 of 52)*/
//====================================================================

#define  XENA_XIP126_PHASE7_MASK 0xff000000
#define  XENA_XIP126_PHASE7_SHIFT 24
#define  XENA_XIP126_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP126_PHASE7_MASK) >> XENA_XIP126_PHASE7_SHIFT)
#define  XENA_XIP126_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP126_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP126_PHASE7_SHIFT))

#define  XENA_XIP126_PHASE6_MASK 0xff0000
#define  XENA_XIP126_PHASE6_SHIFT 16
#define  XENA_XIP126_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP126_PHASE6_MASK) >> XENA_XIP126_PHASE6_SHIFT)
#define  XENA_XIP126_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP126_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP126_PHASE6_SHIFT))

#define  XENA_XIP126_PHASE5_MASK 0xff00
#define  XENA_XIP126_PHASE5_SHIFT 8
#define  XENA_XIP126_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP126_PHASE5_MASK) >> XENA_XIP126_PHASE5_SHIFT)
#define  XENA_XIP126_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP126_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP126_PHASE5_SHIFT))

#define  XENA_XIP126_PHASE4_MASK 0xff
#define  XENA_XIP126_PHASE4_SHIFT 0
#define  XENA_XIP126_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP126_PHASE4_MASK) >> XENA_XIP126_PHASE4_SHIFT)
#define  XENA_XIP126_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP126_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP126_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 28 of 52) (XIP127)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 28 of 52)*/
//====================================================================

#define  XENA_XIP127_PHASE7_MASK 0xff000000
#define  XENA_XIP127_PHASE7_SHIFT 24
#define  XENA_XIP127_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP127_PHASE7_MASK) >> XENA_XIP127_PHASE7_SHIFT)
#define  XENA_XIP127_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP127_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP127_PHASE7_SHIFT))

#define  XENA_XIP127_PHASE6_MASK 0xff0000
#define  XENA_XIP127_PHASE6_SHIFT 16
#define  XENA_XIP127_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP127_PHASE6_MASK) >> XENA_XIP127_PHASE6_SHIFT)
#define  XENA_XIP127_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP127_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP127_PHASE6_SHIFT))

#define  XENA_XIP127_PHASE5_MASK 0xff00
#define  XENA_XIP127_PHASE5_SHIFT 8
#define  XENA_XIP127_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP127_PHASE5_MASK) >> XENA_XIP127_PHASE5_SHIFT)
#define  XENA_XIP127_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP127_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP127_PHASE5_SHIFT))

#define  XENA_XIP127_PHASE4_MASK 0xff
#define  XENA_XIP127_PHASE4_SHIFT 0
#define  XENA_XIP127_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP127_PHASE4_MASK) >> XENA_XIP127_PHASE4_SHIFT)
#define  XENA_XIP127_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP127_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP127_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 29 of 52) (XIP128)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 29 of 52)*/
//====================================================================

#define  XENA_XIP128_PHASE7_MASK 0xff000000
#define  XENA_XIP128_PHASE7_SHIFT 24
#define  XENA_XIP128_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP128_PHASE7_MASK) >> XENA_XIP128_PHASE7_SHIFT)
#define  XENA_XIP128_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP128_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP128_PHASE7_SHIFT))

#define  XENA_XIP128_PHASE6_MASK 0xff0000
#define  XENA_XIP128_PHASE6_SHIFT 16
#define  XENA_XIP128_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP128_PHASE6_MASK) >> XENA_XIP128_PHASE6_SHIFT)
#define  XENA_XIP128_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP128_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP128_PHASE6_SHIFT))

#define  XENA_XIP128_PHASE5_MASK 0xff00
#define  XENA_XIP128_PHASE5_SHIFT 8
#define  XENA_XIP128_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP128_PHASE5_MASK) >> XENA_XIP128_PHASE5_SHIFT)
#define  XENA_XIP128_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP128_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP128_PHASE5_SHIFT))

#define  XENA_XIP128_PHASE4_MASK 0xff
#define  XENA_XIP128_PHASE4_SHIFT 0
#define  XENA_XIP128_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP128_PHASE4_MASK) >> XENA_XIP128_PHASE4_SHIFT)
#define  XENA_XIP128_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP128_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP128_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 30 of 52) (XIP129)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 30 of 52)*/
//====================================================================

#define  XENA_XIP129_PHASE7_MASK 0xff000000
#define  XENA_XIP129_PHASE7_SHIFT 24
#define  XENA_XIP129_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP129_PHASE7_MASK) >> XENA_XIP129_PHASE7_SHIFT)
#define  XENA_XIP129_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP129_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP129_PHASE7_SHIFT))

#define  XENA_XIP129_PHASE6_MASK 0xff0000
#define  XENA_XIP129_PHASE6_SHIFT 16
#define  XENA_XIP129_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP129_PHASE6_MASK) >> XENA_XIP129_PHASE6_SHIFT)
#define  XENA_XIP129_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP129_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP129_PHASE6_SHIFT))

#define  XENA_XIP129_PHASE5_MASK 0xff00
#define  XENA_XIP129_PHASE5_SHIFT 8
#define  XENA_XIP129_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP129_PHASE5_MASK) >> XENA_XIP129_PHASE5_SHIFT)
#define  XENA_XIP129_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP129_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP129_PHASE5_SHIFT))

#define  XENA_XIP129_PHASE4_MASK 0xff
#define  XENA_XIP129_PHASE4_SHIFT 0
#define  XENA_XIP129_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP129_PHASE4_MASK) >> XENA_XIP129_PHASE4_SHIFT)
#define  XENA_XIP129_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP129_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP129_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 31 of 52) (XIP130)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 31 of 52)*/
//====================================================================

#define  XENA_XIP130_PHASE7_MASK 0xff000000
#define  XENA_XIP130_PHASE7_SHIFT 24
#define  XENA_XIP130_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP130_PHASE7_MASK) >> XENA_XIP130_PHASE7_SHIFT)
#define  XENA_XIP130_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP130_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP130_PHASE7_SHIFT))

#define  XENA_XIP130_PHASE6_MASK 0xff0000
#define  XENA_XIP130_PHASE6_SHIFT 16
#define  XENA_XIP130_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP130_PHASE6_MASK) >> XENA_XIP130_PHASE6_SHIFT)
#define  XENA_XIP130_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP130_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP130_PHASE6_SHIFT))

#define  XENA_XIP130_PHASE5_MASK 0xff00
#define  XENA_XIP130_PHASE5_SHIFT 8
#define  XENA_XIP130_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP130_PHASE5_MASK) >> XENA_XIP130_PHASE5_SHIFT)
#define  XENA_XIP130_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP130_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP130_PHASE5_SHIFT))

#define  XENA_XIP130_PHASE4_MASK 0xff
#define  XENA_XIP130_PHASE4_SHIFT 0
#define  XENA_XIP130_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP130_PHASE4_MASK) >> XENA_XIP130_PHASE4_SHIFT)
#define  XENA_XIP130_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP130_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP130_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 32 of 52) (XIP131)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 32 of 52)*/
//====================================================================

#define  XENA_XIP131_PHASE7_MASK 0xff000000
#define  XENA_XIP131_PHASE7_SHIFT 24
#define  XENA_XIP131_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP131_PHASE7_MASK) >> XENA_XIP131_PHASE7_SHIFT)
#define  XENA_XIP131_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP131_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP131_PHASE7_SHIFT))

#define  XENA_XIP131_PHASE6_MASK 0xff0000
#define  XENA_XIP131_PHASE6_SHIFT 16
#define  XENA_XIP131_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP131_PHASE6_MASK) >> XENA_XIP131_PHASE6_SHIFT)
#define  XENA_XIP131_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP131_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP131_PHASE6_SHIFT))

#define  XENA_XIP131_PHASE5_MASK 0xff00
#define  XENA_XIP131_PHASE5_SHIFT 8
#define  XENA_XIP131_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP131_PHASE5_MASK) >> XENA_XIP131_PHASE5_SHIFT)
#define  XENA_XIP131_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP131_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP131_PHASE5_SHIFT))

#define  XENA_XIP131_PHASE4_MASK 0xff
#define  XENA_XIP131_PHASE4_SHIFT 0
#define  XENA_XIP131_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP131_PHASE4_MASK) >> XENA_XIP131_PHASE4_SHIFT)
#define  XENA_XIP131_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP131_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP131_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 33 of 52) (XIP132)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 33 of 52)*/
//====================================================================

#define  XENA_XIP132_PHASE7_MASK 0xff000000
#define  XENA_XIP132_PHASE7_SHIFT 24
#define  XENA_XIP132_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP132_PHASE7_MASK) >> XENA_XIP132_PHASE7_SHIFT)
#define  XENA_XIP132_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP132_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP132_PHASE7_SHIFT))

#define  XENA_XIP132_PHASE6_MASK 0xff0000
#define  XENA_XIP132_PHASE6_SHIFT 16
#define  XENA_XIP132_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP132_PHASE6_MASK) >> XENA_XIP132_PHASE6_SHIFT)
#define  XENA_XIP132_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP132_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP132_PHASE6_SHIFT))

#define  XENA_XIP132_PHASE5_MASK 0xff00
#define  XENA_XIP132_PHASE5_SHIFT 8
#define  XENA_XIP132_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP132_PHASE5_MASK) >> XENA_XIP132_PHASE5_SHIFT)
#define  XENA_XIP132_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP132_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP132_PHASE5_SHIFT))

#define  XENA_XIP132_PHASE4_MASK 0xff
#define  XENA_XIP132_PHASE4_SHIFT 0
#define  XENA_XIP132_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP132_PHASE4_MASK) >> XENA_XIP132_PHASE4_SHIFT)
#define  XENA_XIP132_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP132_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP132_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 34 of 52) (XIP133)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 34 of 52)*/
//====================================================================

#define  XENA_XIP133_PHASE7_MASK 0xff000000
#define  XENA_XIP133_PHASE7_SHIFT 24
#define  XENA_XIP133_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP133_PHASE7_MASK) >> XENA_XIP133_PHASE7_SHIFT)
#define  XENA_XIP133_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP133_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP133_PHASE7_SHIFT))

#define  XENA_XIP133_PHASE6_MASK 0xff0000
#define  XENA_XIP133_PHASE6_SHIFT 16
#define  XENA_XIP133_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP133_PHASE6_MASK) >> XENA_XIP133_PHASE6_SHIFT)
#define  XENA_XIP133_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP133_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP133_PHASE6_SHIFT))

#define  XENA_XIP133_PHASE5_MASK 0xff00
#define  XENA_XIP133_PHASE5_SHIFT 8
#define  XENA_XIP133_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP133_PHASE5_MASK) >> XENA_XIP133_PHASE5_SHIFT)
#define  XENA_XIP133_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP133_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP133_PHASE5_SHIFT))

#define  XENA_XIP133_PHASE4_MASK 0xff
#define  XENA_XIP133_PHASE4_SHIFT 0
#define  XENA_XIP133_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP133_PHASE4_MASK) >> XENA_XIP133_PHASE4_SHIFT)
#define  XENA_XIP133_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP133_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP133_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 35 of 52) (XIP134)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 35 of 52)*/
//====================================================================

#define  XENA_XIP134_PHASE7_MASK 0xff000000
#define  XENA_XIP134_PHASE7_SHIFT 24
#define  XENA_XIP134_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP134_PHASE7_MASK) >> XENA_XIP134_PHASE7_SHIFT)
#define  XENA_XIP134_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP134_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP134_PHASE7_SHIFT))

#define  XENA_XIP134_PHASE6_MASK 0xff0000
#define  XENA_XIP134_PHASE6_SHIFT 16
#define  XENA_XIP134_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP134_PHASE6_MASK) >> XENA_XIP134_PHASE6_SHIFT)
#define  XENA_XIP134_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP134_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP134_PHASE6_SHIFT))

#define  XENA_XIP134_PHASE5_MASK 0xff00
#define  XENA_XIP134_PHASE5_SHIFT 8
#define  XENA_XIP134_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP134_PHASE5_MASK) >> XENA_XIP134_PHASE5_SHIFT)
#define  XENA_XIP134_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP134_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP134_PHASE5_SHIFT))

#define  XENA_XIP134_PHASE4_MASK 0xff
#define  XENA_XIP134_PHASE4_SHIFT 0
#define  XENA_XIP134_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP134_PHASE4_MASK) >> XENA_XIP134_PHASE4_SHIFT)
#define  XENA_XIP134_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP134_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP134_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 36 of 52) (XIP135)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 36 of 52)*/
//====================================================================

#define  XENA_XIP135_PHASE7_MASK 0xff000000
#define  XENA_XIP135_PHASE7_SHIFT 24
#define  XENA_XIP135_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP135_PHASE7_MASK) >> XENA_XIP135_PHASE7_SHIFT)
#define  XENA_XIP135_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP135_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP135_PHASE7_SHIFT))

#define  XENA_XIP135_PHASE6_MASK 0xff0000
#define  XENA_XIP135_PHASE6_SHIFT 16
#define  XENA_XIP135_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP135_PHASE6_MASK) >> XENA_XIP135_PHASE6_SHIFT)
#define  XENA_XIP135_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP135_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP135_PHASE6_SHIFT))

#define  XENA_XIP135_PHASE5_MASK 0xff00
#define  XENA_XIP135_PHASE5_SHIFT 8
#define  XENA_XIP135_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP135_PHASE5_MASK) >> XENA_XIP135_PHASE5_SHIFT)
#define  XENA_XIP135_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP135_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP135_PHASE5_SHIFT))

#define  XENA_XIP135_PHASE4_MASK 0xff
#define  XENA_XIP135_PHASE4_SHIFT 0
#define  XENA_XIP135_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP135_PHASE4_MASK) >> XENA_XIP135_PHASE4_SHIFT)
#define  XENA_XIP135_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP135_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP135_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 37 of 52) (XIP136)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 37 of 52)*/
//====================================================================

#define  XENA_XIP136_PHASE7_MASK 0xff000000
#define  XENA_XIP136_PHASE7_SHIFT 24
#define  XENA_XIP136_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP136_PHASE7_MASK) >> XENA_XIP136_PHASE7_SHIFT)
#define  XENA_XIP136_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP136_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP136_PHASE7_SHIFT))

#define  XENA_XIP136_PHASE6_MASK 0xff0000
#define  XENA_XIP136_PHASE6_SHIFT 16
#define  XENA_XIP136_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP136_PHASE6_MASK) >> XENA_XIP136_PHASE6_SHIFT)
#define  XENA_XIP136_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP136_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP136_PHASE6_SHIFT))

#define  XENA_XIP136_PHASE5_MASK 0xff00
#define  XENA_XIP136_PHASE5_SHIFT 8
#define  XENA_XIP136_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP136_PHASE5_MASK) >> XENA_XIP136_PHASE5_SHIFT)
#define  XENA_XIP136_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP136_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP136_PHASE5_SHIFT))

#define  XENA_XIP136_PHASE4_MASK 0xff
#define  XENA_XIP136_PHASE4_SHIFT 0
#define  XENA_XIP136_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP136_PHASE4_MASK) >> XENA_XIP136_PHASE4_SHIFT)
#define  XENA_XIP136_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP136_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP136_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 38 of 52) (XIP137)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 38 of 52)*/
//====================================================================

#define  XENA_XIP137_PHASE7_MASK 0xff000000
#define  XENA_XIP137_PHASE7_SHIFT 24
#define  XENA_XIP137_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP137_PHASE7_MASK) >> XENA_XIP137_PHASE7_SHIFT)
#define  XENA_XIP137_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP137_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP137_PHASE7_SHIFT))

#define  XENA_XIP137_PHASE6_MASK 0xff0000
#define  XENA_XIP137_PHASE6_SHIFT 16
#define  XENA_XIP137_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP137_PHASE6_MASK) >> XENA_XIP137_PHASE6_SHIFT)
#define  XENA_XIP137_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP137_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP137_PHASE6_SHIFT))

#define  XENA_XIP137_PHASE5_MASK 0xff00
#define  XENA_XIP137_PHASE5_SHIFT 8
#define  XENA_XIP137_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP137_PHASE5_MASK) >> XENA_XIP137_PHASE5_SHIFT)
#define  XENA_XIP137_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP137_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP137_PHASE5_SHIFT))

#define  XENA_XIP137_PHASE4_MASK 0xff
#define  XENA_XIP137_PHASE4_SHIFT 0
#define  XENA_XIP137_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP137_PHASE4_MASK) >> XENA_XIP137_PHASE4_SHIFT)
#define  XENA_XIP137_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP137_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP137_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 39 of 52) (XIP138)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 39 of 52)*/
//====================================================================

#define  XENA_XIP138_PHASE7_MASK 0xff000000
#define  XENA_XIP138_PHASE7_SHIFT 24
#define  XENA_XIP138_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP138_PHASE7_MASK) >> XENA_XIP138_PHASE7_SHIFT)
#define  XENA_XIP138_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP138_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP138_PHASE7_SHIFT))

#define  XENA_XIP138_PHASE6_MASK 0xff0000
#define  XENA_XIP138_PHASE6_SHIFT 16
#define  XENA_XIP138_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP138_PHASE6_MASK) >> XENA_XIP138_PHASE6_SHIFT)
#define  XENA_XIP138_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP138_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP138_PHASE6_SHIFT))

#define  XENA_XIP138_PHASE5_MASK 0xff00
#define  XENA_XIP138_PHASE5_SHIFT 8
#define  XENA_XIP138_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP138_PHASE5_MASK) >> XENA_XIP138_PHASE5_SHIFT)
#define  XENA_XIP138_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP138_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP138_PHASE5_SHIFT))

#define  XENA_XIP138_PHASE4_MASK 0xff
#define  XENA_XIP138_PHASE4_SHIFT 0
#define  XENA_XIP138_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP138_PHASE4_MASK) >> XENA_XIP138_PHASE4_SHIFT)
#define  XENA_XIP138_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP138_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP138_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 40 of 52) (XIP139)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 40 of 52)*/
//====================================================================

#define  XENA_XIP139_PHASE7_MASK 0xff000000
#define  XENA_XIP139_PHASE7_SHIFT 24
#define  XENA_XIP139_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP139_PHASE7_MASK) >> XENA_XIP139_PHASE7_SHIFT)
#define  XENA_XIP139_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP139_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP139_PHASE7_SHIFT))

#define  XENA_XIP139_PHASE6_MASK 0xff0000
#define  XENA_XIP139_PHASE6_SHIFT 16
#define  XENA_XIP139_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP139_PHASE6_MASK) >> XENA_XIP139_PHASE6_SHIFT)
#define  XENA_XIP139_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP139_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP139_PHASE6_SHIFT))

#define  XENA_XIP139_PHASE5_MASK 0xff00
#define  XENA_XIP139_PHASE5_SHIFT 8
#define  XENA_XIP139_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP139_PHASE5_MASK) >> XENA_XIP139_PHASE5_SHIFT)
#define  XENA_XIP139_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP139_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP139_PHASE5_SHIFT))

#define  XENA_XIP139_PHASE4_MASK 0xff
#define  XENA_XIP139_PHASE4_SHIFT 0
#define  XENA_XIP139_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP139_PHASE4_MASK) >> XENA_XIP139_PHASE4_SHIFT)
#define  XENA_XIP139_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP139_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP139_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 41 of 52) (XIP140)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 41 of 52)*/
//====================================================================

#define  XENA_XIP140_PHASE7_MASK 0xff000000
#define  XENA_XIP140_PHASE7_SHIFT 24
#define  XENA_XIP140_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP140_PHASE7_MASK) >> XENA_XIP140_PHASE7_SHIFT)
#define  XENA_XIP140_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP140_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP140_PHASE7_SHIFT))

#define  XENA_XIP140_PHASE6_MASK 0xff0000
#define  XENA_XIP140_PHASE6_SHIFT 16
#define  XENA_XIP140_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP140_PHASE6_MASK) >> XENA_XIP140_PHASE6_SHIFT)
#define  XENA_XIP140_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP140_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP140_PHASE6_SHIFT))

#define  XENA_XIP140_PHASE5_MASK 0xff00
#define  XENA_XIP140_PHASE5_SHIFT 8
#define  XENA_XIP140_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP140_PHASE5_MASK) >> XENA_XIP140_PHASE5_SHIFT)
#define  XENA_XIP140_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP140_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP140_PHASE5_SHIFT))

#define  XENA_XIP140_PHASE4_MASK 0xff
#define  XENA_XIP140_PHASE4_SHIFT 0
#define  XENA_XIP140_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP140_PHASE4_MASK) >> XENA_XIP140_PHASE4_SHIFT)
#define  XENA_XIP140_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP140_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP140_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 42 of 52) (XIP141)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 42 of 52)*/
//====================================================================

#define  XENA_XIP141_PHASE7_MASK 0xff000000
#define  XENA_XIP141_PHASE7_SHIFT 24
#define  XENA_XIP141_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP141_PHASE7_MASK) >> XENA_XIP141_PHASE7_SHIFT)
#define  XENA_XIP141_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP141_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP141_PHASE7_SHIFT))

#define  XENA_XIP141_PHASE6_MASK 0xff0000
#define  XENA_XIP141_PHASE6_SHIFT 16
#define  XENA_XIP141_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP141_PHASE6_MASK) >> XENA_XIP141_PHASE6_SHIFT)
#define  XENA_XIP141_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP141_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP141_PHASE6_SHIFT))

#define  XENA_XIP141_PHASE5_MASK 0xff00
#define  XENA_XIP141_PHASE5_SHIFT 8
#define  XENA_XIP141_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP141_PHASE5_MASK) >> XENA_XIP141_PHASE5_SHIFT)
#define  XENA_XIP141_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP141_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP141_PHASE5_SHIFT))

#define  XENA_XIP141_PHASE4_MASK 0xff
#define  XENA_XIP141_PHASE4_SHIFT 0
#define  XENA_XIP141_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP141_PHASE4_MASK) >> XENA_XIP141_PHASE4_SHIFT)
#define  XENA_XIP141_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP141_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP141_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 43 of 52) (XIP142)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 43 of 52)*/
//====================================================================

#define  XENA_XIP142_PHASE7_MASK 0xff000000
#define  XENA_XIP142_PHASE7_SHIFT 24
#define  XENA_XIP142_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP142_PHASE7_MASK) >> XENA_XIP142_PHASE7_SHIFT)
#define  XENA_XIP142_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP142_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP142_PHASE7_SHIFT))

#define  XENA_XIP142_PHASE6_MASK 0xff0000
#define  XENA_XIP142_PHASE6_SHIFT 16
#define  XENA_XIP142_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP142_PHASE6_MASK) >> XENA_XIP142_PHASE6_SHIFT)
#define  XENA_XIP142_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP142_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP142_PHASE6_SHIFT))

#define  XENA_XIP142_PHASE5_MASK 0xff00
#define  XENA_XIP142_PHASE5_SHIFT 8
#define  XENA_XIP142_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP142_PHASE5_MASK) >> XENA_XIP142_PHASE5_SHIFT)
#define  XENA_XIP142_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP142_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP142_PHASE5_SHIFT))

#define  XENA_XIP142_PHASE4_MASK 0xff
#define  XENA_XIP142_PHASE4_SHIFT 0
#define  XENA_XIP142_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP142_PHASE4_MASK) >> XENA_XIP142_PHASE4_SHIFT)
#define  XENA_XIP142_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP142_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP142_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 44 of 52) (XIP143)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 44 of 52)*/
//====================================================================

#define  XENA_XIP143_PHASE7_MASK 0xff000000
#define  XENA_XIP143_PHASE7_SHIFT 24
#define  XENA_XIP143_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP143_PHASE7_MASK) >> XENA_XIP143_PHASE7_SHIFT)
#define  XENA_XIP143_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP143_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP143_PHASE7_SHIFT))

#define  XENA_XIP143_PHASE6_MASK 0xff0000
#define  XENA_XIP143_PHASE6_SHIFT 16
#define  XENA_XIP143_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP143_PHASE6_MASK) >> XENA_XIP143_PHASE6_SHIFT)
#define  XENA_XIP143_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP143_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP143_PHASE6_SHIFT))

#define  XENA_XIP143_PHASE5_MASK 0xff00
#define  XENA_XIP143_PHASE5_SHIFT 8
#define  XENA_XIP143_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP143_PHASE5_MASK) >> XENA_XIP143_PHASE5_SHIFT)
#define  XENA_XIP143_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP143_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP143_PHASE5_SHIFT))

#define  XENA_XIP143_PHASE4_MASK 0xff
#define  XENA_XIP143_PHASE4_SHIFT 0
#define  XENA_XIP143_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP143_PHASE4_MASK) >> XENA_XIP143_PHASE4_SHIFT)
#define  XENA_XIP143_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP143_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP143_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 45 of 52) (XIP144)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 45 of 52)*/
//====================================================================

#define  XENA_XIP144_PHASE7_MASK 0xff000000
#define  XENA_XIP144_PHASE7_SHIFT 24
#define  XENA_XIP144_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP144_PHASE7_MASK) >> XENA_XIP144_PHASE7_SHIFT)
#define  XENA_XIP144_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP144_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP144_PHASE7_SHIFT))

#define  XENA_XIP144_PHASE6_MASK 0xff0000
#define  XENA_XIP144_PHASE6_SHIFT 16
#define  XENA_XIP144_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP144_PHASE6_MASK) >> XENA_XIP144_PHASE6_SHIFT)
#define  XENA_XIP144_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP144_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP144_PHASE6_SHIFT))

#define  XENA_XIP144_PHASE5_MASK 0xff00
#define  XENA_XIP144_PHASE5_SHIFT 8
#define  XENA_XIP144_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP144_PHASE5_MASK) >> XENA_XIP144_PHASE5_SHIFT)
#define  XENA_XIP144_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP144_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP144_PHASE5_SHIFT))

#define  XENA_XIP144_PHASE4_MASK 0xff
#define  XENA_XIP144_PHASE4_SHIFT 0
#define  XENA_XIP144_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP144_PHASE4_MASK) >> XENA_XIP144_PHASE4_SHIFT)
#define  XENA_XIP144_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP144_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP144_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 46 of 52) (XIP145)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 46 of 52)*/
//====================================================================

#define  XENA_XIP145_PHASE7_MASK 0xff000000
#define  XENA_XIP145_PHASE7_SHIFT 24
#define  XENA_XIP145_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP145_PHASE7_MASK) >> XENA_XIP145_PHASE7_SHIFT)
#define  XENA_XIP145_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP145_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP145_PHASE7_SHIFT))

#define  XENA_XIP145_PHASE6_MASK 0xff0000
#define  XENA_XIP145_PHASE6_SHIFT 16
#define  XENA_XIP145_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP145_PHASE6_MASK) >> XENA_XIP145_PHASE6_SHIFT)
#define  XENA_XIP145_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP145_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP145_PHASE6_SHIFT))

#define  XENA_XIP145_PHASE5_MASK 0xff00
#define  XENA_XIP145_PHASE5_SHIFT 8
#define  XENA_XIP145_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP145_PHASE5_MASK) >> XENA_XIP145_PHASE5_SHIFT)
#define  XENA_XIP145_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP145_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP145_PHASE5_SHIFT))

#define  XENA_XIP145_PHASE4_MASK 0xff
#define  XENA_XIP145_PHASE4_SHIFT 0
#define  XENA_XIP145_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP145_PHASE4_MASK) >> XENA_XIP145_PHASE4_SHIFT)
#define  XENA_XIP145_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP145_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP145_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 47 of 52) (XIP146)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 47 of 52)*/
//====================================================================

#define  XENA_XIP146_PHASE7_MASK 0xff000000
#define  XENA_XIP146_PHASE7_SHIFT 24
#define  XENA_XIP146_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP146_PHASE7_MASK) >> XENA_XIP146_PHASE7_SHIFT)
#define  XENA_XIP146_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP146_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP146_PHASE7_SHIFT))

#define  XENA_XIP146_PHASE6_MASK 0xff0000
#define  XENA_XIP146_PHASE6_SHIFT 16
#define  XENA_XIP146_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP146_PHASE6_MASK) >> XENA_XIP146_PHASE6_SHIFT)
#define  XENA_XIP146_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP146_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP146_PHASE6_SHIFT))

#define  XENA_XIP146_PHASE5_MASK 0xff00
#define  XENA_XIP146_PHASE5_SHIFT 8
#define  XENA_XIP146_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP146_PHASE5_MASK) >> XENA_XIP146_PHASE5_SHIFT)
#define  XENA_XIP146_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP146_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP146_PHASE5_SHIFT))

#define  XENA_XIP146_PHASE4_MASK 0xff
#define  XENA_XIP146_PHASE4_SHIFT 0
#define  XENA_XIP146_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP146_PHASE4_MASK) >> XENA_XIP146_PHASE4_SHIFT)
#define  XENA_XIP146_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP146_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP146_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 48 of 52) (XIP147)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 48 of 52)*/
//====================================================================

#define  XENA_XIP147_PHASE7_MASK 0xff000000
#define  XENA_XIP147_PHASE7_SHIFT 24
#define  XENA_XIP147_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP147_PHASE7_MASK) >> XENA_XIP147_PHASE7_SHIFT)
#define  XENA_XIP147_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP147_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP147_PHASE7_SHIFT))

#define  XENA_XIP147_PHASE6_MASK 0xff0000
#define  XENA_XIP147_PHASE6_SHIFT 16
#define  XENA_XIP147_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP147_PHASE6_MASK) >> XENA_XIP147_PHASE6_SHIFT)
#define  XENA_XIP147_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP147_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP147_PHASE6_SHIFT))

#define  XENA_XIP147_PHASE5_MASK 0xff00
#define  XENA_XIP147_PHASE5_SHIFT 8
#define  XENA_XIP147_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP147_PHASE5_MASK) >> XENA_XIP147_PHASE5_SHIFT)
#define  XENA_XIP147_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP147_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP147_PHASE5_SHIFT))

#define  XENA_XIP147_PHASE4_MASK 0xff
#define  XENA_XIP147_PHASE4_SHIFT 0
#define  XENA_XIP147_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP147_PHASE4_MASK) >> XENA_XIP147_PHASE4_SHIFT)
#define  XENA_XIP147_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP147_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP147_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 49 of 52) (XIP148)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 49 of 52)*/
//====================================================================

#define  XENA_XIP148_PHASE7_MASK 0xff000000
#define  XENA_XIP148_PHASE7_SHIFT 24
#define  XENA_XIP148_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP148_PHASE7_MASK) >> XENA_XIP148_PHASE7_SHIFT)
#define  XENA_XIP148_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP148_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP148_PHASE7_SHIFT))

#define  XENA_XIP148_PHASE6_MASK 0xff0000
#define  XENA_XIP148_PHASE6_SHIFT 16
#define  XENA_XIP148_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP148_PHASE6_MASK) >> XENA_XIP148_PHASE6_SHIFT)
#define  XENA_XIP148_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP148_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP148_PHASE6_SHIFT))

#define  XENA_XIP148_PHASE5_MASK 0xff00
#define  XENA_XIP148_PHASE5_SHIFT 8
#define  XENA_XIP148_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP148_PHASE5_MASK) >> XENA_XIP148_PHASE5_SHIFT)
#define  XENA_XIP148_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP148_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP148_PHASE5_SHIFT))

#define  XENA_XIP148_PHASE4_MASK 0xff
#define  XENA_XIP148_PHASE4_SHIFT 0
#define  XENA_XIP148_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP148_PHASE4_MASK) >> XENA_XIP148_PHASE4_SHIFT)
#define  XENA_XIP148_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP148_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP148_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 50 of 52) (XIP149)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 50 of 52)*/
//====================================================================

#define  XENA_XIP149_PHASE7_MASK 0xff000000
#define  XENA_XIP149_PHASE7_SHIFT 24
#define  XENA_XIP149_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP149_PHASE7_MASK) >> XENA_XIP149_PHASE7_SHIFT)
#define  XENA_XIP149_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP149_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP149_PHASE7_SHIFT))

#define  XENA_XIP149_PHASE6_MASK 0xff0000
#define  XENA_XIP149_PHASE6_SHIFT 16
#define  XENA_XIP149_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP149_PHASE6_MASK) >> XENA_XIP149_PHASE6_SHIFT)
#define  XENA_XIP149_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP149_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP149_PHASE6_SHIFT))

#define  XENA_XIP149_PHASE5_MASK 0xff00
#define  XENA_XIP149_PHASE5_SHIFT 8
#define  XENA_XIP149_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP149_PHASE5_MASK) >> XENA_XIP149_PHASE5_SHIFT)
#define  XENA_XIP149_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP149_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP149_PHASE5_SHIFT))

#define  XENA_XIP149_PHASE4_MASK 0xff
#define  XENA_XIP149_PHASE4_SHIFT 0
#define  XENA_XIP149_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP149_PHASE4_MASK) >> XENA_XIP149_PHASE4_SHIFT)
#define  XENA_XIP149_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP149_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP149_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 51 of 52) (XIP150)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 51 of 52)*/
//====================================================================

#define  XENA_XIP150_PHASE7_MASK 0xff000000
#define  XENA_XIP150_PHASE7_SHIFT 24
#define  XENA_XIP150_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP150_PHASE7_MASK) >> XENA_XIP150_PHASE7_SHIFT)
#define  XENA_XIP150_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP150_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP150_PHASE7_SHIFT))

#define  XENA_XIP150_PHASE6_MASK 0xff0000
#define  XENA_XIP150_PHASE6_SHIFT 16
#define  XENA_XIP150_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP150_PHASE6_MASK) >> XENA_XIP150_PHASE6_SHIFT)
#define  XENA_XIP150_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP150_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP150_PHASE6_SHIFT))

#define  XENA_XIP150_PHASE5_MASK 0xff00
#define  XENA_XIP150_PHASE5_SHIFT 8
#define  XENA_XIP150_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP150_PHASE5_MASK) >> XENA_XIP150_PHASE5_SHIFT)
#define  XENA_XIP150_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP150_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP150_PHASE5_SHIFT))

#define  XENA_XIP150_PHASE4_MASK 0xff
#define  XENA_XIP150_PHASE4_SHIFT 0
#define  XENA_XIP150_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP150_PHASE4_MASK) >> XENA_XIP150_PHASE4_SHIFT)
#define  XENA_XIP150_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP150_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP150_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register1 (Private) (Instance 52 of 52) (XIP151)
/** \brief The UPCIO IO PWM Config Register1 contains configuration information for the last four phases for the I/O PWMs. The programmed values are clkEN[x] cycles, where x is set by the value in XIIC.PWMSel. (Instance 52 of 52)*/
//====================================================================

#define  XENA_XIP151_PHASE7_MASK 0xff000000
#define  XENA_XIP151_PHASE7_SHIFT 24
#define  XENA_XIP151_PHASE7_MASK_SHIFT(reg) (((reg) & XENA_XIP151_PHASE7_MASK) >> XENA_XIP151_PHASE7_SHIFT)
#define  XENA_XIP151_PHASE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP151_PHASE7_MASK) | (((uint32_t)val) << XENA_XIP151_PHASE7_SHIFT))

#define  XENA_XIP151_PHASE6_MASK 0xff0000
#define  XENA_XIP151_PHASE6_SHIFT 16
#define  XENA_XIP151_PHASE6_MASK_SHIFT(reg) (((reg) & XENA_XIP151_PHASE6_MASK) >> XENA_XIP151_PHASE6_SHIFT)
#define  XENA_XIP151_PHASE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP151_PHASE6_MASK) | (((uint32_t)val) << XENA_XIP151_PHASE6_SHIFT))

#define  XENA_XIP151_PHASE5_MASK 0xff00
#define  XENA_XIP151_PHASE5_SHIFT 8
#define  XENA_XIP151_PHASE5_MASK_SHIFT(reg) (((reg) & XENA_XIP151_PHASE5_MASK) >> XENA_XIP151_PHASE5_SHIFT)
#define  XENA_XIP151_PHASE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP151_PHASE5_MASK) | (((uint32_t)val) << XENA_XIP151_PHASE5_SHIFT))

#define  XENA_XIP151_PHASE4_MASK 0xff
#define  XENA_XIP151_PHASE4_SHIFT 0
#define  XENA_XIP151_PHASE4_MASK_SHIFT(reg) (((reg) & XENA_XIP151_PHASE4_MASK) >> XENA_XIP151_PHASE4_SHIFT)
#define  XENA_XIP151_PHASE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP151_PHASE4_MASK) | (((uint32_t)val) << XENA_XIP151_PHASE4_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 1 of 52) (XIP20)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 1 of 52)*/
//====================================================================

#define  XENA_XIP20_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP20_RESERVED1_SHIFT 17
#define  XENA_XIP20_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP20_RESERVED1_MASK) >> XENA_XIP20_RESERVED1_SHIFT)
#define  XENA_XIP20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP20_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP20_RESERVED1_SHIFT))

#define  XENA_XIP20_PWMMODE_MASK 0x10000
#define  XENA_XIP20_PWMMODE_SHIFT 16
#define  XENA_XIP20_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP20_PWMMODE_MASK) >> XENA_XIP20_PWMMODE_SHIFT)
#define  XENA_XIP20_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP20_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP20_PWMMODE_SHIFT))

#define  XENA_XIP20_RESERVED2_MASK 0xff00
#define  XENA_XIP20_RESERVED2_SHIFT 8
#define  XENA_XIP20_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP20_RESERVED2_MASK) >> XENA_XIP20_RESERVED2_SHIFT)
#define  XENA_XIP20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP20_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP20_RESERVED2_SHIFT))

#define  XENA_XIP20_PWMEN_MASK 0x80
#define  XENA_XIP20_PWMEN_SHIFT 7
#define  XENA_XIP20_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP20_PWMEN_MASK) >> XENA_XIP20_PWMEN_SHIFT)
#define  XENA_XIP20_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP20_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP20_PWMEN_SHIFT))

#define  XENA_XIP20_RESERVED3_MASK 0x70
#define  XENA_XIP20_RESERVED3_SHIFT 4
#define  XENA_XIP20_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP20_RESERVED3_MASK) >> XENA_XIP20_RESERVED3_SHIFT)
#define  XENA_XIP20_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP20_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP20_RESERVED3_SHIFT))

#define  XENA_XIP20_PWMSEL_MASK 0xf
#define  XENA_XIP20_PWMSEL_SHIFT 0
#define  XENA_XIP20_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP20_PWMSEL_MASK) >> XENA_XIP20_PWMSEL_SHIFT)
#define  XENA_XIP20_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP20_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP20_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 2 of 52) (XIP21)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 2 of 52)*/
//====================================================================

#define  XENA_XIP21_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP21_RESERVED1_SHIFT 17
#define  XENA_XIP21_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP21_RESERVED1_MASK) >> XENA_XIP21_RESERVED1_SHIFT)
#define  XENA_XIP21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP21_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP21_RESERVED1_SHIFT))

#define  XENA_XIP21_PWMMODE_MASK 0x10000
#define  XENA_XIP21_PWMMODE_SHIFT 16
#define  XENA_XIP21_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP21_PWMMODE_MASK) >> XENA_XIP21_PWMMODE_SHIFT)
#define  XENA_XIP21_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP21_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP21_PWMMODE_SHIFT))

#define  XENA_XIP21_RESERVED2_MASK 0xff00
#define  XENA_XIP21_RESERVED2_SHIFT 8
#define  XENA_XIP21_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP21_RESERVED2_MASK) >> XENA_XIP21_RESERVED2_SHIFT)
#define  XENA_XIP21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP21_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP21_RESERVED2_SHIFT))

#define  XENA_XIP21_PWMEN_MASK 0x80
#define  XENA_XIP21_PWMEN_SHIFT 7
#define  XENA_XIP21_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP21_PWMEN_MASK) >> XENA_XIP21_PWMEN_SHIFT)
#define  XENA_XIP21_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP21_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP21_PWMEN_SHIFT))

#define  XENA_XIP21_RESERVED3_MASK 0x70
#define  XENA_XIP21_RESERVED3_SHIFT 4
#define  XENA_XIP21_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP21_RESERVED3_MASK) >> XENA_XIP21_RESERVED3_SHIFT)
#define  XENA_XIP21_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP21_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP21_RESERVED3_SHIFT))

#define  XENA_XIP21_PWMSEL_MASK 0xf
#define  XENA_XIP21_PWMSEL_SHIFT 0
#define  XENA_XIP21_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP21_PWMSEL_MASK) >> XENA_XIP21_PWMSEL_SHIFT)
#define  XENA_XIP21_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP21_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP21_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 3 of 52) (XIP22)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 3 of 52)*/
//====================================================================

#define  XENA_XIP22_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP22_RESERVED1_SHIFT 17
#define  XENA_XIP22_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP22_RESERVED1_MASK) >> XENA_XIP22_RESERVED1_SHIFT)
#define  XENA_XIP22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP22_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP22_RESERVED1_SHIFT))

#define  XENA_XIP22_PWMMODE_MASK 0x10000
#define  XENA_XIP22_PWMMODE_SHIFT 16
#define  XENA_XIP22_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP22_PWMMODE_MASK) >> XENA_XIP22_PWMMODE_SHIFT)
#define  XENA_XIP22_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP22_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP22_PWMMODE_SHIFT))

#define  XENA_XIP22_RESERVED2_MASK 0xff00
#define  XENA_XIP22_RESERVED2_SHIFT 8
#define  XENA_XIP22_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP22_RESERVED2_MASK) >> XENA_XIP22_RESERVED2_SHIFT)
#define  XENA_XIP22_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP22_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP22_RESERVED2_SHIFT))

#define  XENA_XIP22_PWMEN_MASK 0x80
#define  XENA_XIP22_PWMEN_SHIFT 7
#define  XENA_XIP22_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP22_PWMEN_MASK) >> XENA_XIP22_PWMEN_SHIFT)
#define  XENA_XIP22_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP22_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP22_PWMEN_SHIFT))

#define  XENA_XIP22_RESERVED3_MASK 0x70
#define  XENA_XIP22_RESERVED3_SHIFT 4
#define  XENA_XIP22_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP22_RESERVED3_MASK) >> XENA_XIP22_RESERVED3_SHIFT)
#define  XENA_XIP22_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP22_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP22_RESERVED3_SHIFT))

#define  XENA_XIP22_PWMSEL_MASK 0xf
#define  XENA_XIP22_PWMSEL_SHIFT 0
#define  XENA_XIP22_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP22_PWMSEL_MASK) >> XENA_XIP22_PWMSEL_SHIFT)
#define  XENA_XIP22_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP22_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP22_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 4 of 52) (XIP23)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 4 of 52)*/
//====================================================================

#define  XENA_XIP23_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP23_RESERVED1_SHIFT 17
#define  XENA_XIP23_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP23_RESERVED1_MASK) >> XENA_XIP23_RESERVED1_SHIFT)
#define  XENA_XIP23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP23_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP23_RESERVED1_SHIFT))

#define  XENA_XIP23_PWMMODE_MASK 0x10000
#define  XENA_XIP23_PWMMODE_SHIFT 16
#define  XENA_XIP23_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP23_PWMMODE_MASK) >> XENA_XIP23_PWMMODE_SHIFT)
#define  XENA_XIP23_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP23_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP23_PWMMODE_SHIFT))

#define  XENA_XIP23_RESERVED2_MASK 0xff00
#define  XENA_XIP23_RESERVED2_SHIFT 8
#define  XENA_XIP23_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP23_RESERVED2_MASK) >> XENA_XIP23_RESERVED2_SHIFT)
#define  XENA_XIP23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP23_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP23_RESERVED2_SHIFT))

#define  XENA_XIP23_PWMEN_MASK 0x80
#define  XENA_XIP23_PWMEN_SHIFT 7
#define  XENA_XIP23_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP23_PWMEN_MASK) >> XENA_XIP23_PWMEN_SHIFT)
#define  XENA_XIP23_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP23_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP23_PWMEN_SHIFT))

#define  XENA_XIP23_RESERVED3_MASK 0x70
#define  XENA_XIP23_RESERVED3_SHIFT 4
#define  XENA_XIP23_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP23_RESERVED3_MASK) >> XENA_XIP23_RESERVED3_SHIFT)
#define  XENA_XIP23_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP23_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP23_RESERVED3_SHIFT))

#define  XENA_XIP23_PWMSEL_MASK 0xf
#define  XENA_XIP23_PWMSEL_SHIFT 0
#define  XENA_XIP23_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP23_PWMSEL_MASK) >> XENA_XIP23_PWMSEL_SHIFT)
#define  XENA_XIP23_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP23_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP23_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 5 of 52) (XIP24)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 5 of 52)*/
//====================================================================

#define  XENA_XIP24_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP24_RESERVED1_SHIFT 17
#define  XENA_XIP24_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP24_RESERVED1_MASK) >> XENA_XIP24_RESERVED1_SHIFT)
#define  XENA_XIP24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP24_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP24_RESERVED1_SHIFT))

#define  XENA_XIP24_PWMMODE_MASK 0x10000
#define  XENA_XIP24_PWMMODE_SHIFT 16
#define  XENA_XIP24_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP24_PWMMODE_MASK) >> XENA_XIP24_PWMMODE_SHIFT)
#define  XENA_XIP24_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP24_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP24_PWMMODE_SHIFT))

#define  XENA_XIP24_RESERVED2_MASK 0xff00
#define  XENA_XIP24_RESERVED2_SHIFT 8
#define  XENA_XIP24_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP24_RESERVED2_MASK) >> XENA_XIP24_RESERVED2_SHIFT)
#define  XENA_XIP24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP24_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP24_RESERVED2_SHIFT))

#define  XENA_XIP24_PWMEN_MASK 0x80
#define  XENA_XIP24_PWMEN_SHIFT 7
#define  XENA_XIP24_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP24_PWMEN_MASK) >> XENA_XIP24_PWMEN_SHIFT)
#define  XENA_XIP24_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP24_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP24_PWMEN_SHIFT))

#define  XENA_XIP24_RESERVED3_MASK 0x70
#define  XENA_XIP24_RESERVED3_SHIFT 4
#define  XENA_XIP24_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP24_RESERVED3_MASK) >> XENA_XIP24_RESERVED3_SHIFT)
#define  XENA_XIP24_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP24_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP24_RESERVED3_SHIFT))

#define  XENA_XIP24_PWMSEL_MASK 0xf
#define  XENA_XIP24_PWMSEL_SHIFT 0
#define  XENA_XIP24_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP24_PWMSEL_MASK) >> XENA_XIP24_PWMSEL_SHIFT)
#define  XENA_XIP24_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP24_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP24_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 6 of 52) (XIP25)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 6 of 52)*/
//====================================================================

#define  XENA_XIP25_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP25_RESERVED1_SHIFT 17
#define  XENA_XIP25_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP25_RESERVED1_MASK) >> XENA_XIP25_RESERVED1_SHIFT)
#define  XENA_XIP25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP25_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP25_RESERVED1_SHIFT))

#define  XENA_XIP25_PWMMODE_MASK 0x10000
#define  XENA_XIP25_PWMMODE_SHIFT 16
#define  XENA_XIP25_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP25_PWMMODE_MASK) >> XENA_XIP25_PWMMODE_SHIFT)
#define  XENA_XIP25_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP25_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP25_PWMMODE_SHIFT))

#define  XENA_XIP25_RESERVED2_MASK 0xff00
#define  XENA_XIP25_RESERVED2_SHIFT 8
#define  XENA_XIP25_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP25_RESERVED2_MASK) >> XENA_XIP25_RESERVED2_SHIFT)
#define  XENA_XIP25_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP25_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP25_RESERVED2_SHIFT))

#define  XENA_XIP25_PWMEN_MASK 0x80
#define  XENA_XIP25_PWMEN_SHIFT 7
#define  XENA_XIP25_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP25_PWMEN_MASK) >> XENA_XIP25_PWMEN_SHIFT)
#define  XENA_XIP25_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP25_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP25_PWMEN_SHIFT))

#define  XENA_XIP25_RESERVED3_MASK 0x70
#define  XENA_XIP25_RESERVED3_SHIFT 4
#define  XENA_XIP25_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP25_RESERVED3_MASK) >> XENA_XIP25_RESERVED3_SHIFT)
#define  XENA_XIP25_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP25_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP25_RESERVED3_SHIFT))

#define  XENA_XIP25_PWMSEL_MASK 0xf
#define  XENA_XIP25_PWMSEL_SHIFT 0
#define  XENA_XIP25_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP25_PWMSEL_MASK) >> XENA_XIP25_PWMSEL_SHIFT)
#define  XENA_XIP25_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP25_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP25_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 7 of 52) (XIP26)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 7 of 52)*/
//====================================================================

#define  XENA_XIP26_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP26_RESERVED1_SHIFT 17
#define  XENA_XIP26_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP26_RESERVED1_MASK) >> XENA_XIP26_RESERVED1_SHIFT)
#define  XENA_XIP26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP26_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP26_RESERVED1_SHIFT))

#define  XENA_XIP26_PWMMODE_MASK 0x10000
#define  XENA_XIP26_PWMMODE_SHIFT 16
#define  XENA_XIP26_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP26_PWMMODE_MASK) >> XENA_XIP26_PWMMODE_SHIFT)
#define  XENA_XIP26_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP26_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP26_PWMMODE_SHIFT))

#define  XENA_XIP26_RESERVED2_MASK 0xff00
#define  XENA_XIP26_RESERVED2_SHIFT 8
#define  XENA_XIP26_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP26_RESERVED2_MASK) >> XENA_XIP26_RESERVED2_SHIFT)
#define  XENA_XIP26_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP26_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP26_RESERVED2_SHIFT))

#define  XENA_XIP26_PWMEN_MASK 0x80
#define  XENA_XIP26_PWMEN_SHIFT 7
#define  XENA_XIP26_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP26_PWMEN_MASK) >> XENA_XIP26_PWMEN_SHIFT)
#define  XENA_XIP26_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP26_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP26_PWMEN_SHIFT))

#define  XENA_XIP26_RESERVED3_MASK 0x70
#define  XENA_XIP26_RESERVED3_SHIFT 4
#define  XENA_XIP26_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP26_RESERVED3_MASK) >> XENA_XIP26_RESERVED3_SHIFT)
#define  XENA_XIP26_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP26_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP26_RESERVED3_SHIFT))

#define  XENA_XIP26_PWMSEL_MASK 0xf
#define  XENA_XIP26_PWMSEL_SHIFT 0
#define  XENA_XIP26_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP26_PWMSEL_MASK) >> XENA_XIP26_PWMSEL_SHIFT)
#define  XENA_XIP26_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP26_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP26_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 8 of 52) (XIP27)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 8 of 52)*/
//====================================================================

#define  XENA_XIP27_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP27_RESERVED1_SHIFT 17
#define  XENA_XIP27_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP27_RESERVED1_MASK) >> XENA_XIP27_RESERVED1_SHIFT)
#define  XENA_XIP27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP27_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP27_RESERVED1_SHIFT))

#define  XENA_XIP27_PWMMODE_MASK 0x10000
#define  XENA_XIP27_PWMMODE_SHIFT 16
#define  XENA_XIP27_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP27_PWMMODE_MASK) >> XENA_XIP27_PWMMODE_SHIFT)
#define  XENA_XIP27_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP27_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP27_PWMMODE_SHIFT))

#define  XENA_XIP27_RESERVED2_MASK 0xff00
#define  XENA_XIP27_RESERVED2_SHIFT 8
#define  XENA_XIP27_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP27_RESERVED2_MASK) >> XENA_XIP27_RESERVED2_SHIFT)
#define  XENA_XIP27_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP27_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP27_RESERVED2_SHIFT))

#define  XENA_XIP27_PWMEN_MASK 0x80
#define  XENA_XIP27_PWMEN_SHIFT 7
#define  XENA_XIP27_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP27_PWMEN_MASK) >> XENA_XIP27_PWMEN_SHIFT)
#define  XENA_XIP27_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP27_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP27_PWMEN_SHIFT))

#define  XENA_XIP27_RESERVED3_MASK 0x70
#define  XENA_XIP27_RESERVED3_SHIFT 4
#define  XENA_XIP27_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP27_RESERVED3_MASK) >> XENA_XIP27_RESERVED3_SHIFT)
#define  XENA_XIP27_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP27_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP27_RESERVED3_SHIFT))

#define  XENA_XIP27_PWMSEL_MASK 0xf
#define  XENA_XIP27_PWMSEL_SHIFT 0
#define  XENA_XIP27_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP27_PWMSEL_MASK) >> XENA_XIP27_PWMSEL_SHIFT)
#define  XENA_XIP27_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP27_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP27_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 9 of 52) (XIP28)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 9 of 52)*/
//====================================================================

#define  XENA_XIP28_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP28_RESERVED1_SHIFT 17
#define  XENA_XIP28_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP28_RESERVED1_MASK) >> XENA_XIP28_RESERVED1_SHIFT)
#define  XENA_XIP28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP28_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP28_RESERVED1_SHIFT))

#define  XENA_XIP28_PWMMODE_MASK 0x10000
#define  XENA_XIP28_PWMMODE_SHIFT 16
#define  XENA_XIP28_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP28_PWMMODE_MASK) >> XENA_XIP28_PWMMODE_SHIFT)
#define  XENA_XIP28_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP28_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP28_PWMMODE_SHIFT))

#define  XENA_XIP28_RESERVED2_MASK 0xff00
#define  XENA_XIP28_RESERVED2_SHIFT 8
#define  XENA_XIP28_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP28_RESERVED2_MASK) >> XENA_XIP28_RESERVED2_SHIFT)
#define  XENA_XIP28_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP28_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP28_RESERVED2_SHIFT))

#define  XENA_XIP28_PWMEN_MASK 0x80
#define  XENA_XIP28_PWMEN_SHIFT 7
#define  XENA_XIP28_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP28_PWMEN_MASK) >> XENA_XIP28_PWMEN_SHIFT)
#define  XENA_XIP28_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP28_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP28_PWMEN_SHIFT))

#define  XENA_XIP28_RESERVED3_MASK 0x70
#define  XENA_XIP28_RESERVED3_SHIFT 4
#define  XENA_XIP28_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP28_RESERVED3_MASK) >> XENA_XIP28_RESERVED3_SHIFT)
#define  XENA_XIP28_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP28_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP28_RESERVED3_SHIFT))

#define  XENA_XIP28_PWMSEL_MASK 0xf
#define  XENA_XIP28_PWMSEL_SHIFT 0
#define  XENA_XIP28_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP28_PWMSEL_MASK) >> XENA_XIP28_PWMSEL_SHIFT)
#define  XENA_XIP28_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP28_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP28_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 10 of 52) (XIP29)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 10 of 52)*/
//====================================================================

#define  XENA_XIP29_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP29_RESERVED1_SHIFT 17
#define  XENA_XIP29_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP29_RESERVED1_MASK) >> XENA_XIP29_RESERVED1_SHIFT)
#define  XENA_XIP29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP29_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP29_RESERVED1_SHIFT))

#define  XENA_XIP29_PWMMODE_MASK 0x10000
#define  XENA_XIP29_PWMMODE_SHIFT 16
#define  XENA_XIP29_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP29_PWMMODE_MASK) >> XENA_XIP29_PWMMODE_SHIFT)
#define  XENA_XIP29_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP29_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP29_PWMMODE_SHIFT))

#define  XENA_XIP29_RESERVED2_MASK 0xff00
#define  XENA_XIP29_RESERVED2_SHIFT 8
#define  XENA_XIP29_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP29_RESERVED2_MASK) >> XENA_XIP29_RESERVED2_SHIFT)
#define  XENA_XIP29_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP29_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP29_RESERVED2_SHIFT))

#define  XENA_XIP29_PWMEN_MASK 0x80
#define  XENA_XIP29_PWMEN_SHIFT 7
#define  XENA_XIP29_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP29_PWMEN_MASK) >> XENA_XIP29_PWMEN_SHIFT)
#define  XENA_XIP29_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP29_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP29_PWMEN_SHIFT))

#define  XENA_XIP29_RESERVED3_MASK 0x70
#define  XENA_XIP29_RESERVED3_SHIFT 4
#define  XENA_XIP29_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP29_RESERVED3_MASK) >> XENA_XIP29_RESERVED3_SHIFT)
#define  XENA_XIP29_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP29_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP29_RESERVED3_SHIFT))

#define  XENA_XIP29_PWMSEL_MASK 0xf
#define  XENA_XIP29_PWMSEL_SHIFT 0
#define  XENA_XIP29_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP29_PWMSEL_MASK) >> XENA_XIP29_PWMSEL_SHIFT)
#define  XENA_XIP29_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP29_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP29_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 11 of 52) (XIP210)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 11 of 52)*/
//====================================================================

#define  XENA_XIP210_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP210_RESERVED1_SHIFT 17
#define  XENA_XIP210_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP210_RESERVED1_MASK) >> XENA_XIP210_RESERVED1_SHIFT)
#define  XENA_XIP210_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP210_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP210_RESERVED1_SHIFT))

#define  XENA_XIP210_PWMMODE_MASK 0x10000
#define  XENA_XIP210_PWMMODE_SHIFT 16
#define  XENA_XIP210_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP210_PWMMODE_MASK) >> XENA_XIP210_PWMMODE_SHIFT)
#define  XENA_XIP210_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP210_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP210_PWMMODE_SHIFT))

#define  XENA_XIP210_RESERVED2_MASK 0xff00
#define  XENA_XIP210_RESERVED2_SHIFT 8
#define  XENA_XIP210_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP210_RESERVED2_MASK) >> XENA_XIP210_RESERVED2_SHIFT)
#define  XENA_XIP210_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP210_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP210_RESERVED2_SHIFT))

#define  XENA_XIP210_PWMEN_MASK 0x80
#define  XENA_XIP210_PWMEN_SHIFT 7
#define  XENA_XIP210_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP210_PWMEN_MASK) >> XENA_XIP210_PWMEN_SHIFT)
#define  XENA_XIP210_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP210_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP210_PWMEN_SHIFT))

#define  XENA_XIP210_RESERVED3_MASK 0x70
#define  XENA_XIP210_RESERVED3_SHIFT 4
#define  XENA_XIP210_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP210_RESERVED3_MASK) >> XENA_XIP210_RESERVED3_SHIFT)
#define  XENA_XIP210_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP210_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP210_RESERVED3_SHIFT))

#define  XENA_XIP210_PWMSEL_MASK 0xf
#define  XENA_XIP210_PWMSEL_SHIFT 0
#define  XENA_XIP210_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP210_PWMSEL_MASK) >> XENA_XIP210_PWMSEL_SHIFT)
#define  XENA_XIP210_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP210_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP210_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 12 of 52) (XIP211)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 12 of 52)*/
//====================================================================

#define  XENA_XIP211_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP211_RESERVED1_SHIFT 17
#define  XENA_XIP211_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP211_RESERVED1_MASK) >> XENA_XIP211_RESERVED1_SHIFT)
#define  XENA_XIP211_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP211_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP211_RESERVED1_SHIFT))

#define  XENA_XIP211_PWMMODE_MASK 0x10000
#define  XENA_XIP211_PWMMODE_SHIFT 16
#define  XENA_XIP211_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP211_PWMMODE_MASK) >> XENA_XIP211_PWMMODE_SHIFT)
#define  XENA_XIP211_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP211_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP211_PWMMODE_SHIFT))

#define  XENA_XIP211_RESERVED2_MASK 0xff00
#define  XENA_XIP211_RESERVED2_SHIFT 8
#define  XENA_XIP211_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP211_RESERVED2_MASK) >> XENA_XIP211_RESERVED2_SHIFT)
#define  XENA_XIP211_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP211_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP211_RESERVED2_SHIFT))

#define  XENA_XIP211_PWMEN_MASK 0x80
#define  XENA_XIP211_PWMEN_SHIFT 7
#define  XENA_XIP211_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP211_PWMEN_MASK) >> XENA_XIP211_PWMEN_SHIFT)
#define  XENA_XIP211_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP211_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP211_PWMEN_SHIFT))

#define  XENA_XIP211_RESERVED3_MASK 0x70
#define  XENA_XIP211_RESERVED3_SHIFT 4
#define  XENA_XIP211_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP211_RESERVED3_MASK) >> XENA_XIP211_RESERVED3_SHIFT)
#define  XENA_XIP211_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP211_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP211_RESERVED3_SHIFT))

#define  XENA_XIP211_PWMSEL_MASK 0xf
#define  XENA_XIP211_PWMSEL_SHIFT 0
#define  XENA_XIP211_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP211_PWMSEL_MASK) >> XENA_XIP211_PWMSEL_SHIFT)
#define  XENA_XIP211_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP211_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP211_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 13 of 52) (XIP212)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 13 of 52)*/
//====================================================================

#define  XENA_XIP212_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP212_RESERVED1_SHIFT 17
#define  XENA_XIP212_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP212_RESERVED1_MASK) >> XENA_XIP212_RESERVED1_SHIFT)
#define  XENA_XIP212_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP212_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP212_RESERVED1_SHIFT))

#define  XENA_XIP212_PWMMODE_MASK 0x10000
#define  XENA_XIP212_PWMMODE_SHIFT 16
#define  XENA_XIP212_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP212_PWMMODE_MASK) >> XENA_XIP212_PWMMODE_SHIFT)
#define  XENA_XIP212_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP212_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP212_PWMMODE_SHIFT))

#define  XENA_XIP212_RESERVED2_MASK 0xff00
#define  XENA_XIP212_RESERVED2_SHIFT 8
#define  XENA_XIP212_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP212_RESERVED2_MASK) >> XENA_XIP212_RESERVED2_SHIFT)
#define  XENA_XIP212_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP212_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP212_RESERVED2_SHIFT))

#define  XENA_XIP212_PWMEN_MASK 0x80
#define  XENA_XIP212_PWMEN_SHIFT 7
#define  XENA_XIP212_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP212_PWMEN_MASK) >> XENA_XIP212_PWMEN_SHIFT)
#define  XENA_XIP212_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP212_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP212_PWMEN_SHIFT))

#define  XENA_XIP212_RESERVED3_MASK 0x70
#define  XENA_XIP212_RESERVED3_SHIFT 4
#define  XENA_XIP212_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP212_RESERVED3_MASK) >> XENA_XIP212_RESERVED3_SHIFT)
#define  XENA_XIP212_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP212_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP212_RESERVED3_SHIFT))

#define  XENA_XIP212_PWMSEL_MASK 0xf
#define  XENA_XIP212_PWMSEL_SHIFT 0
#define  XENA_XIP212_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP212_PWMSEL_MASK) >> XENA_XIP212_PWMSEL_SHIFT)
#define  XENA_XIP212_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP212_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP212_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 14 of 52) (XIP213)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 14 of 52)*/
//====================================================================

#define  XENA_XIP213_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP213_RESERVED1_SHIFT 17
#define  XENA_XIP213_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP213_RESERVED1_MASK) >> XENA_XIP213_RESERVED1_SHIFT)
#define  XENA_XIP213_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP213_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP213_RESERVED1_SHIFT))

#define  XENA_XIP213_PWMMODE_MASK 0x10000
#define  XENA_XIP213_PWMMODE_SHIFT 16
#define  XENA_XIP213_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP213_PWMMODE_MASK) >> XENA_XIP213_PWMMODE_SHIFT)
#define  XENA_XIP213_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP213_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP213_PWMMODE_SHIFT))

#define  XENA_XIP213_RESERVED2_MASK 0xff00
#define  XENA_XIP213_RESERVED2_SHIFT 8
#define  XENA_XIP213_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP213_RESERVED2_MASK) >> XENA_XIP213_RESERVED2_SHIFT)
#define  XENA_XIP213_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP213_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP213_RESERVED2_SHIFT))

#define  XENA_XIP213_PWMEN_MASK 0x80
#define  XENA_XIP213_PWMEN_SHIFT 7
#define  XENA_XIP213_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP213_PWMEN_MASK) >> XENA_XIP213_PWMEN_SHIFT)
#define  XENA_XIP213_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP213_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP213_PWMEN_SHIFT))

#define  XENA_XIP213_RESERVED3_MASK 0x70
#define  XENA_XIP213_RESERVED3_SHIFT 4
#define  XENA_XIP213_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP213_RESERVED3_MASK) >> XENA_XIP213_RESERVED3_SHIFT)
#define  XENA_XIP213_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP213_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP213_RESERVED3_SHIFT))

#define  XENA_XIP213_PWMSEL_MASK 0xf
#define  XENA_XIP213_PWMSEL_SHIFT 0
#define  XENA_XIP213_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP213_PWMSEL_MASK) >> XENA_XIP213_PWMSEL_SHIFT)
#define  XENA_XIP213_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP213_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP213_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 15 of 52) (XIP214)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 15 of 52)*/
//====================================================================

#define  XENA_XIP214_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP214_RESERVED1_SHIFT 17
#define  XENA_XIP214_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP214_RESERVED1_MASK) >> XENA_XIP214_RESERVED1_SHIFT)
#define  XENA_XIP214_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP214_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP214_RESERVED1_SHIFT))

#define  XENA_XIP214_PWMMODE_MASK 0x10000
#define  XENA_XIP214_PWMMODE_SHIFT 16
#define  XENA_XIP214_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP214_PWMMODE_MASK) >> XENA_XIP214_PWMMODE_SHIFT)
#define  XENA_XIP214_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP214_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP214_PWMMODE_SHIFT))

#define  XENA_XIP214_RESERVED2_MASK 0xff00
#define  XENA_XIP214_RESERVED2_SHIFT 8
#define  XENA_XIP214_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP214_RESERVED2_MASK) >> XENA_XIP214_RESERVED2_SHIFT)
#define  XENA_XIP214_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP214_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP214_RESERVED2_SHIFT))

#define  XENA_XIP214_PWMEN_MASK 0x80
#define  XENA_XIP214_PWMEN_SHIFT 7
#define  XENA_XIP214_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP214_PWMEN_MASK) >> XENA_XIP214_PWMEN_SHIFT)
#define  XENA_XIP214_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP214_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP214_PWMEN_SHIFT))

#define  XENA_XIP214_RESERVED3_MASK 0x70
#define  XENA_XIP214_RESERVED3_SHIFT 4
#define  XENA_XIP214_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP214_RESERVED3_MASK) >> XENA_XIP214_RESERVED3_SHIFT)
#define  XENA_XIP214_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP214_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP214_RESERVED3_SHIFT))

#define  XENA_XIP214_PWMSEL_MASK 0xf
#define  XENA_XIP214_PWMSEL_SHIFT 0
#define  XENA_XIP214_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP214_PWMSEL_MASK) >> XENA_XIP214_PWMSEL_SHIFT)
#define  XENA_XIP214_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP214_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP214_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 16 of 52) (XIP215)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 16 of 52)*/
//====================================================================

#define  XENA_XIP215_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP215_RESERVED1_SHIFT 17
#define  XENA_XIP215_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP215_RESERVED1_MASK) >> XENA_XIP215_RESERVED1_SHIFT)
#define  XENA_XIP215_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP215_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP215_RESERVED1_SHIFT))

#define  XENA_XIP215_PWMMODE_MASK 0x10000
#define  XENA_XIP215_PWMMODE_SHIFT 16
#define  XENA_XIP215_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP215_PWMMODE_MASK) >> XENA_XIP215_PWMMODE_SHIFT)
#define  XENA_XIP215_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP215_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP215_PWMMODE_SHIFT))

#define  XENA_XIP215_RESERVED2_MASK 0xff00
#define  XENA_XIP215_RESERVED2_SHIFT 8
#define  XENA_XIP215_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP215_RESERVED2_MASK) >> XENA_XIP215_RESERVED2_SHIFT)
#define  XENA_XIP215_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP215_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP215_RESERVED2_SHIFT))

#define  XENA_XIP215_PWMEN_MASK 0x80
#define  XENA_XIP215_PWMEN_SHIFT 7
#define  XENA_XIP215_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP215_PWMEN_MASK) >> XENA_XIP215_PWMEN_SHIFT)
#define  XENA_XIP215_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP215_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP215_PWMEN_SHIFT))

#define  XENA_XIP215_RESERVED3_MASK 0x70
#define  XENA_XIP215_RESERVED3_SHIFT 4
#define  XENA_XIP215_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP215_RESERVED3_MASK) >> XENA_XIP215_RESERVED3_SHIFT)
#define  XENA_XIP215_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP215_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP215_RESERVED3_SHIFT))

#define  XENA_XIP215_PWMSEL_MASK 0xf
#define  XENA_XIP215_PWMSEL_SHIFT 0
#define  XENA_XIP215_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP215_PWMSEL_MASK) >> XENA_XIP215_PWMSEL_SHIFT)
#define  XENA_XIP215_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP215_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP215_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 17 of 52) (XIP216)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 17 of 52)*/
//====================================================================

#define  XENA_XIP216_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP216_RESERVED1_SHIFT 17
#define  XENA_XIP216_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP216_RESERVED1_MASK) >> XENA_XIP216_RESERVED1_SHIFT)
#define  XENA_XIP216_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP216_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP216_RESERVED1_SHIFT))

#define  XENA_XIP216_PWMMODE_MASK 0x10000
#define  XENA_XIP216_PWMMODE_SHIFT 16
#define  XENA_XIP216_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP216_PWMMODE_MASK) >> XENA_XIP216_PWMMODE_SHIFT)
#define  XENA_XIP216_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP216_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP216_PWMMODE_SHIFT))

#define  XENA_XIP216_RESERVED2_MASK 0xff00
#define  XENA_XIP216_RESERVED2_SHIFT 8
#define  XENA_XIP216_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP216_RESERVED2_MASK) >> XENA_XIP216_RESERVED2_SHIFT)
#define  XENA_XIP216_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP216_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP216_RESERVED2_SHIFT))

#define  XENA_XIP216_PWMEN_MASK 0x80
#define  XENA_XIP216_PWMEN_SHIFT 7
#define  XENA_XIP216_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP216_PWMEN_MASK) >> XENA_XIP216_PWMEN_SHIFT)
#define  XENA_XIP216_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP216_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP216_PWMEN_SHIFT))

#define  XENA_XIP216_RESERVED3_MASK 0x70
#define  XENA_XIP216_RESERVED3_SHIFT 4
#define  XENA_XIP216_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP216_RESERVED3_MASK) >> XENA_XIP216_RESERVED3_SHIFT)
#define  XENA_XIP216_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP216_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP216_RESERVED3_SHIFT))

#define  XENA_XIP216_PWMSEL_MASK 0xf
#define  XENA_XIP216_PWMSEL_SHIFT 0
#define  XENA_XIP216_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP216_PWMSEL_MASK) >> XENA_XIP216_PWMSEL_SHIFT)
#define  XENA_XIP216_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP216_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP216_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 18 of 52) (XIP217)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 18 of 52)*/
//====================================================================

#define  XENA_XIP217_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP217_RESERVED1_SHIFT 17
#define  XENA_XIP217_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP217_RESERVED1_MASK) >> XENA_XIP217_RESERVED1_SHIFT)
#define  XENA_XIP217_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP217_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP217_RESERVED1_SHIFT))

#define  XENA_XIP217_PWMMODE_MASK 0x10000
#define  XENA_XIP217_PWMMODE_SHIFT 16
#define  XENA_XIP217_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP217_PWMMODE_MASK) >> XENA_XIP217_PWMMODE_SHIFT)
#define  XENA_XIP217_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP217_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP217_PWMMODE_SHIFT))

#define  XENA_XIP217_RESERVED2_MASK 0xff00
#define  XENA_XIP217_RESERVED2_SHIFT 8
#define  XENA_XIP217_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP217_RESERVED2_MASK) >> XENA_XIP217_RESERVED2_SHIFT)
#define  XENA_XIP217_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP217_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP217_RESERVED2_SHIFT))

#define  XENA_XIP217_PWMEN_MASK 0x80
#define  XENA_XIP217_PWMEN_SHIFT 7
#define  XENA_XIP217_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP217_PWMEN_MASK) >> XENA_XIP217_PWMEN_SHIFT)
#define  XENA_XIP217_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP217_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP217_PWMEN_SHIFT))

#define  XENA_XIP217_RESERVED3_MASK 0x70
#define  XENA_XIP217_RESERVED3_SHIFT 4
#define  XENA_XIP217_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP217_RESERVED3_MASK) >> XENA_XIP217_RESERVED3_SHIFT)
#define  XENA_XIP217_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP217_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP217_RESERVED3_SHIFT))

#define  XENA_XIP217_PWMSEL_MASK 0xf
#define  XENA_XIP217_PWMSEL_SHIFT 0
#define  XENA_XIP217_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP217_PWMSEL_MASK) >> XENA_XIP217_PWMSEL_SHIFT)
#define  XENA_XIP217_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP217_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP217_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 19 of 52) (XIP218)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 19 of 52)*/
//====================================================================

#define  XENA_XIP218_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP218_RESERVED1_SHIFT 17
#define  XENA_XIP218_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP218_RESERVED1_MASK) >> XENA_XIP218_RESERVED1_SHIFT)
#define  XENA_XIP218_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP218_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP218_RESERVED1_SHIFT))

#define  XENA_XIP218_PWMMODE_MASK 0x10000
#define  XENA_XIP218_PWMMODE_SHIFT 16
#define  XENA_XIP218_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP218_PWMMODE_MASK) >> XENA_XIP218_PWMMODE_SHIFT)
#define  XENA_XIP218_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP218_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP218_PWMMODE_SHIFT))

#define  XENA_XIP218_RESERVED2_MASK 0xff00
#define  XENA_XIP218_RESERVED2_SHIFT 8
#define  XENA_XIP218_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP218_RESERVED2_MASK) >> XENA_XIP218_RESERVED2_SHIFT)
#define  XENA_XIP218_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP218_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP218_RESERVED2_SHIFT))

#define  XENA_XIP218_PWMEN_MASK 0x80
#define  XENA_XIP218_PWMEN_SHIFT 7
#define  XENA_XIP218_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP218_PWMEN_MASK) >> XENA_XIP218_PWMEN_SHIFT)
#define  XENA_XIP218_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP218_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP218_PWMEN_SHIFT))

#define  XENA_XIP218_RESERVED3_MASK 0x70
#define  XENA_XIP218_RESERVED3_SHIFT 4
#define  XENA_XIP218_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP218_RESERVED3_MASK) >> XENA_XIP218_RESERVED3_SHIFT)
#define  XENA_XIP218_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP218_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP218_RESERVED3_SHIFT))

#define  XENA_XIP218_PWMSEL_MASK 0xf
#define  XENA_XIP218_PWMSEL_SHIFT 0
#define  XENA_XIP218_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP218_PWMSEL_MASK) >> XENA_XIP218_PWMSEL_SHIFT)
#define  XENA_XIP218_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP218_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP218_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 20 of 52) (XIP219)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 20 of 52)*/
//====================================================================

#define  XENA_XIP219_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP219_RESERVED1_SHIFT 17
#define  XENA_XIP219_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP219_RESERVED1_MASK) >> XENA_XIP219_RESERVED1_SHIFT)
#define  XENA_XIP219_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP219_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP219_RESERVED1_SHIFT))

#define  XENA_XIP219_PWMMODE_MASK 0x10000
#define  XENA_XIP219_PWMMODE_SHIFT 16
#define  XENA_XIP219_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP219_PWMMODE_MASK) >> XENA_XIP219_PWMMODE_SHIFT)
#define  XENA_XIP219_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP219_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP219_PWMMODE_SHIFT))

#define  XENA_XIP219_RESERVED2_MASK 0xff00
#define  XENA_XIP219_RESERVED2_SHIFT 8
#define  XENA_XIP219_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP219_RESERVED2_MASK) >> XENA_XIP219_RESERVED2_SHIFT)
#define  XENA_XIP219_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP219_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP219_RESERVED2_SHIFT))

#define  XENA_XIP219_PWMEN_MASK 0x80
#define  XENA_XIP219_PWMEN_SHIFT 7
#define  XENA_XIP219_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP219_PWMEN_MASK) >> XENA_XIP219_PWMEN_SHIFT)
#define  XENA_XIP219_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP219_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP219_PWMEN_SHIFT))

#define  XENA_XIP219_RESERVED3_MASK 0x70
#define  XENA_XIP219_RESERVED3_SHIFT 4
#define  XENA_XIP219_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP219_RESERVED3_MASK) >> XENA_XIP219_RESERVED3_SHIFT)
#define  XENA_XIP219_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP219_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP219_RESERVED3_SHIFT))

#define  XENA_XIP219_PWMSEL_MASK 0xf
#define  XENA_XIP219_PWMSEL_SHIFT 0
#define  XENA_XIP219_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP219_PWMSEL_MASK) >> XENA_XIP219_PWMSEL_SHIFT)
#define  XENA_XIP219_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP219_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP219_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 21 of 52) (XIP220)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 21 of 52)*/
//====================================================================

#define  XENA_XIP220_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP220_RESERVED1_SHIFT 17
#define  XENA_XIP220_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP220_RESERVED1_MASK) >> XENA_XIP220_RESERVED1_SHIFT)
#define  XENA_XIP220_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP220_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP220_RESERVED1_SHIFT))

#define  XENA_XIP220_PWMMODE_MASK 0x10000
#define  XENA_XIP220_PWMMODE_SHIFT 16
#define  XENA_XIP220_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP220_PWMMODE_MASK) >> XENA_XIP220_PWMMODE_SHIFT)
#define  XENA_XIP220_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP220_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP220_PWMMODE_SHIFT))

#define  XENA_XIP220_RESERVED2_MASK 0xff00
#define  XENA_XIP220_RESERVED2_SHIFT 8
#define  XENA_XIP220_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP220_RESERVED2_MASK) >> XENA_XIP220_RESERVED2_SHIFT)
#define  XENA_XIP220_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP220_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP220_RESERVED2_SHIFT))

#define  XENA_XIP220_PWMEN_MASK 0x80
#define  XENA_XIP220_PWMEN_SHIFT 7
#define  XENA_XIP220_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP220_PWMEN_MASK) >> XENA_XIP220_PWMEN_SHIFT)
#define  XENA_XIP220_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP220_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP220_PWMEN_SHIFT))

#define  XENA_XIP220_RESERVED3_MASK 0x70
#define  XENA_XIP220_RESERVED3_SHIFT 4
#define  XENA_XIP220_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP220_RESERVED3_MASK) >> XENA_XIP220_RESERVED3_SHIFT)
#define  XENA_XIP220_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP220_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP220_RESERVED3_SHIFT))

#define  XENA_XIP220_PWMSEL_MASK 0xf
#define  XENA_XIP220_PWMSEL_SHIFT 0
#define  XENA_XIP220_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP220_PWMSEL_MASK) >> XENA_XIP220_PWMSEL_SHIFT)
#define  XENA_XIP220_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP220_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP220_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 22 of 52) (XIP221)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 22 of 52)*/
//====================================================================

#define  XENA_XIP221_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP221_RESERVED1_SHIFT 17
#define  XENA_XIP221_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP221_RESERVED1_MASK) >> XENA_XIP221_RESERVED1_SHIFT)
#define  XENA_XIP221_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP221_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP221_RESERVED1_SHIFT))

#define  XENA_XIP221_PWMMODE_MASK 0x10000
#define  XENA_XIP221_PWMMODE_SHIFT 16
#define  XENA_XIP221_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP221_PWMMODE_MASK) >> XENA_XIP221_PWMMODE_SHIFT)
#define  XENA_XIP221_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP221_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP221_PWMMODE_SHIFT))

#define  XENA_XIP221_RESERVED2_MASK 0xff00
#define  XENA_XIP221_RESERVED2_SHIFT 8
#define  XENA_XIP221_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP221_RESERVED2_MASK) >> XENA_XIP221_RESERVED2_SHIFT)
#define  XENA_XIP221_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP221_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP221_RESERVED2_SHIFT))

#define  XENA_XIP221_PWMEN_MASK 0x80
#define  XENA_XIP221_PWMEN_SHIFT 7
#define  XENA_XIP221_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP221_PWMEN_MASK) >> XENA_XIP221_PWMEN_SHIFT)
#define  XENA_XIP221_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP221_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP221_PWMEN_SHIFT))

#define  XENA_XIP221_RESERVED3_MASK 0x70
#define  XENA_XIP221_RESERVED3_SHIFT 4
#define  XENA_XIP221_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP221_RESERVED3_MASK) >> XENA_XIP221_RESERVED3_SHIFT)
#define  XENA_XIP221_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP221_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP221_RESERVED3_SHIFT))

#define  XENA_XIP221_PWMSEL_MASK 0xf
#define  XENA_XIP221_PWMSEL_SHIFT 0
#define  XENA_XIP221_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP221_PWMSEL_MASK) >> XENA_XIP221_PWMSEL_SHIFT)
#define  XENA_XIP221_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP221_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP221_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 23 of 52) (XIP222)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 23 of 52)*/
//====================================================================

#define  XENA_XIP222_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP222_RESERVED1_SHIFT 17
#define  XENA_XIP222_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP222_RESERVED1_MASK) >> XENA_XIP222_RESERVED1_SHIFT)
#define  XENA_XIP222_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP222_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP222_RESERVED1_SHIFT))

#define  XENA_XIP222_PWMMODE_MASK 0x10000
#define  XENA_XIP222_PWMMODE_SHIFT 16
#define  XENA_XIP222_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP222_PWMMODE_MASK) >> XENA_XIP222_PWMMODE_SHIFT)
#define  XENA_XIP222_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP222_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP222_PWMMODE_SHIFT))

#define  XENA_XIP222_RESERVED2_MASK 0xff00
#define  XENA_XIP222_RESERVED2_SHIFT 8
#define  XENA_XIP222_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP222_RESERVED2_MASK) >> XENA_XIP222_RESERVED2_SHIFT)
#define  XENA_XIP222_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP222_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP222_RESERVED2_SHIFT))

#define  XENA_XIP222_PWMEN_MASK 0x80
#define  XENA_XIP222_PWMEN_SHIFT 7
#define  XENA_XIP222_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP222_PWMEN_MASK) >> XENA_XIP222_PWMEN_SHIFT)
#define  XENA_XIP222_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP222_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP222_PWMEN_SHIFT))

#define  XENA_XIP222_RESERVED3_MASK 0x70
#define  XENA_XIP222_RESERVED3_SHIFT 4
#define  XENA_XIP222_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP222_RESERVED3_MASK) >> XENA_XIP222_RESERVED3_SHIFT)
#define  XENA_XIP222_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP222_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP222_RESERVED3_SHIFT))

#define  XENA_XIP222_PWMSEL_MASK 0xf
#define  XENA_XIP222_PWMSEL_SHIFT 0
#define  XENA_XIP222_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP222_PWMSEL_MASK) >> XENA_XIP222_PWMSEL_SHIFT)
#define  XENA_XIP222_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP222_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP222_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 24 of 52) (XIP223)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 24 of 52)*/
//====================================================================

#define  XENA_XIP223_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP223_RESERVED1_SHIFT 17
#define  XENA_XIP223_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP223_RESERVED1_MASK) >> XENA_XIP223_RESERVED1_SHIFT)
#define  XENA_XIP223_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP223_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP223_RESERVED1_SHIFT))

#define  XENA_XIP223_PWMMODE_MASK 0x10000
#define  XENA_XIP223_PWMMODE_SHIFT 16
#define  XENA_XIP223_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP223_PWMMODE_MASK) >> XENA_XIP223_PWMMODE_SHIFT)
#define  XENA_XIP223_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP223_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP223_PWMMODE_SHIFT))

#define  XENA_XIP223_RESERVED2_MASK 0xff00
#define  XENA_XIP223_RESERVED2_SHIFT 8
#define  XENA_XIP223_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP223_RESERVED2_MASK) >> XENA_XIP223_RESERVED2_SHIFT)
#define  XENA_XIP223_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP223_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP223_RESERVED2_SHIFT))

#define  XENA_XIP223_PWMEN_MASK 0x80
#define  XENA_XIP223_PWMEN_SHIFT 7
#define  XENA_XIP223_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP223_PWMEN_MASK) >> XENA_XIP223_PWMEN_SHIFT)
#define  XENA_XIP223_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP223_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP223_PWMEN_SHIFT))

#define  XENA_XIP223_RESERVED3_MASK 0x70
#define  XENA_XIP223_RESERVED3_SHIFT 4
#define  XENA_XIP223_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP223_RESERVED3_MASK) >> XENA_XIP223_RESERVED3_SHIFT)
#define  XENA_XIP223_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP223_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP223_RESERVED3_SHIFT))

#define  XENA_XIP223_PWMSEL_MASK 0xf
#define  XENA_XIP223_PWMSEL_SHIFT 0
#define  XENA_XIP223_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP223_PWMSEL_MASK) >> XENA_XIP223_PWMSEL_SHIFT)
#define  XENA_XIP223_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP223_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP223_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 25 of 52) (XIP224)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 25 of 52)*/
//====================================================================

#define  XENA_XIP224_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP224_RESERVED1_SHIFT 17
#define  XENA_XIP224_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP224_RESERVED1_MASK) >> XENA_XIP224_RESERVED1_SHIFT)
#define  XENA_XIP224_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP224_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP224_RESERVED1_SHIFT))

#define  XENA_XIP224_PWMMODE_MASK 0x10000
#define  XENA_XIP224_PWMMODE_SHIFT 16
#define  XENA_XIP224_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP224_PWMMODE_MASK) >> XENA_XIP224_PWMMODE_SHIFT)
#define  XENA_XIP224_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP224_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP224_PWMMODE_SHIFT))

#define  XENA_XIP224_RESERVED2_MASK 0xff00
#define  XENA_XIP224_RESERVED2_SHIFT 8
#define  XENA_XIP224_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP224_RESERVED2_MASK) >> XENA_XIP224_RESERVED2_SHIFT)
#define  XENA_XIP224_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP224_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP224_RESERVED2_SHIFT))

#define  XENA_XIP224_PWMEN_MASK 0x80
#define  XENA_XIP224_PWMEN_SHIFT 7
#define  XENA_XIP224_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP224_PWMEN_MASK) >> XENA_XIP224_PWMEN_SHIFT)
#define  XENA_XIP224_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP224_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP224_PWMEN_SHIFT))

#define  XENA_XIP224_RESERVED3_MASK 0x70
#define  XENA_XIP224_RESERVED3_SHIFT 4
#define  XENA_XIP224_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP224_RESERVED3_MASK) >> XENA_XIP224_RESERVED3_SHIFT)
#define  XENA_XIP224_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP224_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP224_RESERVED3_SHIFT))

#define  XENA_XIP224_PWMSEL_MASK 0xf
#define  XENA_XIP224_PWMSEL_SHIFT 0
#define  XENA_XIP224_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP224_PWMSEL_MASK) >> XENA_XIP224_PWMSEL_SHIFT)
#define  XENA_XIP224_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP224_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP224_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 26 of 52) (XIP225)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 26 of 52)*/
//====================================================================

#define  XENA_XIP225_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP225_RESERVED1_SHIFT 17
#define  XENA_XIP225_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP225_RESERVED1_MASK) >> XENA_XIP225_RESERVED1_SHIFT)
#define  XENA_XIP225_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP225_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP225_RESERVED1_SHIFT))

#define  XENA_XIP225_PWMMODE_MASK 0x10000
#define  XENA_XIP225_PWMMODE_SHIFT 16
#define  XENA_XIP225_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP225_PWMMODE_MASK) >> XENA_XIP225_PWMMODE_SHIFT)
#define  XENA_XIP225_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP225_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP225_PWMMODE_SHIFT))

#define  XENA_XIP225_RESERVED2_MASK 0xff00
#define  XENA_XIP225_RESERVED2_SHIFT 8
#define  XENA_XIP225_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP225_RESERVED2_MASK) >> XENA_XIP225_RESERVED2_SHIFT)
#define  XENA_XIP225_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP225_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP225_RESERVED2_SHIFT))

#define  XENA_XIP225_PWMEN_MASK 0x80
#define  XENA_XIP225_PWMEN_SHIFT 7
#define  XENA_XIP225_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP225_PWMEN_MASK) >> XENA_XIP225_PWMEN_SHIFT)
#define  XENA_XIP225_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP225_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP225_PWMEN_SHIFT))

#define  XENA_XIP225_RESERVED3_MASK 0x70
#define  XENA_XIP225_RESERVED3_SHIFT 4
#define  XENA_XIP225_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP225_RESERVED3_MASK) >> XENA_XIP225_RESERVED3_SHIFT)
#define  XENA_XIP225_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP225_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP225_RESERVED3_SHIFT))

#define  XENA_XIP225_PWMSEL_MASK 0xf
#define  XENA_XIP225_PWMSEL_SHIFT 0
#define  XENA_XIP225_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP225_PWMSEL_MASK) >> XENA_XIP225_PWMSEL_SHIFT)
#define  XENA_XIP225_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP225_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP225_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 27 of 52) (XIP226)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 27 of 52)*/
//====================================================================

#define  XENA_XIP226_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP226_RESERVED1_SHIFT 17
#define  XENA_XIP226_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP226_RESERVED1_MASK) >> XENA_XIP226_RESERVED1_SHIFT)
#define  XENA_XIP226_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP226_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP226_RESERVED1_SHIFT))

#define  XENA_XIP226_PWMMODE_MASK 0x10000
#define  XENA_XIP226_PWMMODE_SHIFT 16
#define  XENA_XIP226_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP226_PWMMODE_MASK) >> XENA_XIP226_PWMMODE_SHIFT)
#define  XENA_XIP226_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP226_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP226_PWMMODE_SHIFT))

#define  XENA_XIP226_RESERVED2_MASK 0xff00
#define  XENA_XIP226_RESERVED2_SHIFT 8
#define  XENA_XIP226_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP226_RESERVED2_MASK) >> XENA_XIP226_RESERVED2_SHIFT)
#define  XENA_XIP226_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP226_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP226_RESERVED2_SHIFT))

#define  XENA_XIP226_PWMEN_MASK 0x80
#define  XENA_XIP226_PWMEN_SHIFT 7
#define  XENA_XIP226_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP226_PWMEN_MASK) >> XENA_XIP226_PWMEN_SHIFT)
#define  XENA_XIP226_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP226_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP226_PWMEN_SHIFT))

#define  XENA_XIP226_RESERVED3_MASK 0x70
#define  XENA_XIP226_RESERVED3_SHIFT 4
#define  XENA_XIP226_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP226_RESERVED3_MASK) >> XENA_XIP226_RESERVED3_SHIFT)
#define  XENA_XIP226_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP226_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP226_RESERVED3_SHIFT))

#define  XENA_XIP226_PWMSEL_MASK 0xf
#define  XENA_XIP226_PWMSEL_SHIFT 0
#define  XENA_XIP226_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP226_PWMSEL_MASK) >> XENA_XIP226_PWMSEL_SHIFT)
#define  XENA_XIP226_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP226_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP226_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 28 of 52) (XIP227)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 28 of 52)*/
//====================================================================

#define  XENA_XIP227_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP227_RESERVED1_SHIFT 17
#define  XENA_XIP227_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP227_RESERVED1_MASK) >> XENA_XIP227_RESERVED1_SHIFT)
#define  XENA_XIP227_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP227_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP227_RESERVED1_SHIFT))

#define  XENA_XIP227_PWMMODE_MASK 0x10000
#define  XENA_XIP227_PWMMODE_SHIFT 16
#define  XENA_XIP227_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP227_PWMMODE_MASK) >> XENA_XIP227_PWMMODE_SHIFT)
#define  XENA_XIP227_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP227_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP227_PWMMODE_SHIFT))

#define  XENA_XIP227_RESERVED2_MASK 0xff00
#define  XENA_XIP227_RESERVED2_SHIFT 8
#define  XENA_XIP227_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP227_RESERVED2_MASK) >> XENA_XIP227_RESERVED2_SHIFT)
#define  XENA_XIP227_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP227_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP227_RESERVED2_SHIFT))

#define  XENA_XIP227_PWMEN_MASK 0x80
#define  XENA_XIP227_PWMEN_SHIFT 7
#define  XENA_XIP227_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP227_PWMEN_MASK) >> XENA_XIP227_PWMEN_SHIFT)
#define  XENA_XIP227_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP227_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP227_PWMEN_SHIFT))

#define  XENA_XIP227_RESERVED3_MASK 0x70
#define  XENA_XIP227_RESERVED3_SHIFT 4
#define  XENA_XIP227_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP227_RESERVED3_MASK) >> XENA_XIP227_RESERVED3_SHIFT)
#define  XENA_XIP227_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP227_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP227_RESERVED3_SHIFT))

#define  XENA_XIP227_PWMSEL_MASK 0xf
#define  XENA_XIP227_PWMSEL_SHIFT 0
#define  XENA_XIP227_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP227_PWMSEL_MASK) >> XENA_XIP227_PWMSEL_SHIFT)
#define  XENA_XIP227_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP227_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP227_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 29 of 52) (XIP228)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 29 of 52)*/
//====================================================================

#define  XENA_XIP228_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP228_RESERVED1_SHIFT 17
#define  XENA_XIP228_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP228_RESERVED1_MASK) >> XENA_XIP228_RESERVED1_SHIFT)
#define  XENA_XIP228_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP228_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP228_RESERVED1_SHIFT))

#define  XENA_XIP228_PWMMODE_MASK 0x10000
#define  XENA_XIP228_PWMMODE_SHIFT 16
#define  XENA_XIP228_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP228_PWMMODE_MASK) >> XENA_XIP228_PWMMODE_SHIFT)
#define  XENA_XIP228_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP228_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP228_PWMMODE_SHIFT))

#define  XENA_XIP228_RESERVED2_MASK 0xff00
#define  XENA_XIP228_RESERVED2_SHIFT 8
#define  XENA_XIP228_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP228_RESERVED2_MASK) >> XENA_XIP228_RESERVED2_SHIFT)
#define  XENA_XIP228_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP228_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP228_RESERVED2_SHIFT))

#define  XENA_XIP228_PWMEN_MASK 0x80
#define  XENA_XIP228_PWMEN_SHIFT 7
#define  XENA_XIP228_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP228_PWMEN_MASK) >> XENA_XIP228_PWMEN_SHIFT)
#define  XENA_XIP228_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP228_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP228_PWMEN_SHIFT))

#define  XENA_XIP228_RESERVED3_MASK 0x70
#define  XENA_XIP228_RESERVED3_SHIFT 4
#define  XENA_XIP228_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP228_RESERVED3_MASK) >> XENA_XIP228_RESERVED3_SHIFT)
#define  XENA_XIP228_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP228_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP228_RESERVED3_SHIFT))

#define  XENA_XIP228_PWMSEL_MASK 0xf
#define  XENA_XIP228_PWMSEL_SHIFT 0
#define  XENA_XIP228_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP228_PWMSEL_MASK) >> XENA_XIP228_PWMSEL_SHIFT)
#define  XENA_XIP228_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP228_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP228_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 30 of 52) (XIP229)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 30 of 52)*/
//====================================================================

#define  XENA_XIP229_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP229_RESERVED1_SHIFT 17
#define  XENA_XIP229_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP229_RESERVED1_MASK) >> XENA_XIP229_RESERVED1_SHIFT)
#define  XENA_XIP229_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP229_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP229_RESERVED1_SHIFT))

#define  XENA_XIP229_PWMMODE_MASK 0x10000
#define  XENA_XIP229_PWMMODE_SHIFT 16
#define  XENA_XIP229_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP229_PWMMODE_MASK) >> XENA_XIP229_PWMMODE_SHIFT)
#define  XENA_XIP229_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP229_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP229_PWMMODE_SHIFT))

#define  XENA_XIP229_RESERVED2_MASK 0xff00
#define  XENA_XIP229_RESERVED2_SHIFT 8
#define  XENA_XIP229_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP229_RESERVED2_MASK) >> XENA_XIP229_RESERVED2_SHIFT)
#define  XENA_XIP229_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP229_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP229_RESERVED2_SHIFT))

#define  XENA_XIP229_PWMEN_MASK 0x80
#define  XENA_XIP229_PWMEN_SHIFT 7
#define  XENA_XIP229_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP229_PWMEN_MASK) >> XENA_XIP229_PWMEN_SHIFT)
#define  XENA_XIP229_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP229_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP229_PWMEN_SHIFT))

#define  XENA_XIP229_RESERVED3_MASK 0x70
#define  XENA_XIP229_RESERVED3_SHIFT 4
#define  XENA_XIP229_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP229_RESERVED3_MASK) >> XENA_XIP229_RESERVED3_SHIFT)
#define  XENA_XIP229_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP229_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP229_RESERVED3_SHIFT))

#define  XENA_XIP229_PWMSEL_MASK 0xf
#define  XENA_XIP229_PWMSEL_SHIFT 0
#define  XENA_XIP229_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP229_PWMSEL_MASK) >> XENA_XIP229_PWMSEL_SHIFT)
#define  XENA_XIP229_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP229_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP229_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 31 of 52) (XIP230)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 31 of 52)*/
//====================================================================

#define  XENA_XIP230_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP230_RESERVED1_SHIFT 17
#define  XENA_XIP230_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP230_RESERVED1_MASK) >> XENA_XIP230_RESERVED1_SHIFT)
#define  XENA_XIP230_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP230_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP230_RESERVED1_SHIFT))

#define  XENA_XIP230_PWMMODE_MASK 0x10000
#define  XENA_XIP230_PWMMODE_SHIFT 16
#define  XENA_XIP230_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP230_PWMMODE_MASK) >> XENA_XIP230_PWMMODE_SHIFT)
#define  XENA_XIP230_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP230_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP230_PWMMODE_SHIFT))

#define  XENA_XIP230_RESERVED2_MASK 0xff00
#define  XENA_XIP230_RESERVED2_SHIFT 8
#define  XENA_XIP230_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP230_RESERVED2_MASK) >> XENA_XIP230_RESERVED2_SHIFT)
#define  XENA_XIP230_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP230_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP230_RESERVED2_SHIFT))

#define  XENA_XIP230_PWMEN_MASK 0x80
#define  XENA_XIP230_PWMEN_SHIFT 7
#define  XENA_XIP230_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP230_PWMEN_MASK) >> XENA_XIP230_PWMEN_SHIFT)
#define  XENA_XIP230_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP230_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP230_PWMEN_SHIFT))

#define  XENA_XIP230_RESERVED3_MASK 0x70
#define  XENA_XIP230_RESERVED3_SHIFT 4
#define  XENA_XIP230_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP230_RESERVED3_MASK) >> XENA_XIP230_RESERVED3_SHIFT)
#define  XENA_XIP230_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP230_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP230_RESERVED3_SHIFT))

#define  XENA_XIP230_PWMSEL_MASK 0xf
#define  XENA_XIP230_PWMSEL_SHIFT 0
#define  XENA_XIP230_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP230_PWMSEL_MASK) >> XENA_XIP230_PWMSEL_SHIFT)
#define  XENA_XIP230_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP230_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP230_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 32 of 52) (XIP231)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 32 of 52)*/
//====================================================================

#define  XENA_XIP231_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP231_RESERVED1_SHIFT 17
#define  XENA_XIP231_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP231_RESERVED1_MASK) >> XENA_XIP231_RESERVED1_SHIFT)
#define  XENA_XIP231_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP231_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP231_RESERVED1_SHIFT))

#define  XENA_XIP231_PWMMODE_MASK 0x10000
#define  XENA_XIP231_PWMMODE_SHIFT 16
#define  XENA_XIP231_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP231_PWMMODE_MASK) >> XENA_XIP231_PWMMODE_SHIFT)
#define  XENA_XIP231_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP231_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP231_PWMMODE_SHIFT))

#define  XENA_XIP231_RESERVED2_MASK 0xff00
#define  XENA_XIP231_RESERVED2_SHIFT 8
#define  XENA_XIP231_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP231_RESERVED2_MASK) >> XENA_XIP231_RESERVED2_SHIFT)
#define  XENA_XIP231_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP231_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP231_RESERVED2_SHIFT))

#define  XENA_XIP231_PWMEN_MASK 0x80
#define  XENA_XIP231_PWMEN_SHIFT 7
#define  XENA_XIP231_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP231_PWMEN_MASK) >> XENA_XIP231_PWMEN_SHIFT)
#define  XENA_XIP231_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP231_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP231_PWMEN_SHIFT))

#define  XENA_XIP231_RESERVED3_MASK 0x70
#define  XENA_XIP231_RESERVED3_SHIFT 4
#define  XENA_XIP231_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP231_RESERVED3_MASK) >> XENA_XIP231_RESERVED3_SHIFT)
#define  XENA_XIP231_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP231_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP231_RESERVED3_SHIFT))

#define  XENA_XIP231_PWMSEL_MASK 0xf
#define  XENA_XIP231_PWMSEL_SHIFT 0
#define  XENA_XIP231_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP231_PWMSEL_MASK) >> XENA_XIP231_PWMSEL_SHIFT)
#define  XENA_XIP231_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP231_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP231_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 33 of 52) (XIP232)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 33 of 52)*/
//====================================================================

#define  XENA_XIP232_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP232_RESERVED1_SHIFT 17
#define  XENA_XIP232_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP232_RESERVED1_MASK) >> XENA_XIP232_RESERVED1_SHIFT)
#define  XENA_XIP232_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP232_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP232_RESERVED1_SHIFT))

#define  XENA_XIP232_PWMMODE_MASK 0x10000
#define  XENA_XIP232_PWMMODE_SHIFT 16
#define  XENA_XIP232_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP232_PWMMODE_MASK) >> XENA_XIP232_PWMMODE_SHIFT)
#define  XENA_XIP232_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP232_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP232_PWMMODE_SHIFT))

#define  XENA_XIP232_RESERVED2_MASK 0xff00
#define  XENA_XIP232_RESERVED2_SHIFT 8
#define  XENA_XIP232_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP232_RESERVED2_MASK) >> XENA_XIP232_RESERVED2_SHIFT)
#define  XENA_XIP232_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP232_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP232_RESERVED2_SHIFT))

#define  XENA_XIP232_PWMEN_MASK 0x80
#define  XENA_XIP232_PWMEN_SHIFT 7
#define  XENA_XIP232_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP232_PWMEN_MASK) >> XENA_XIP232_PWMEN_SHIFT)
#define  XENA_XIP232_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP232_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP232_PWMEN_SHIFT))

#define  XENA_XIP232_RESERVED3_MASK 0x70
#define  XENA_XIP232_RESERVED3_SHIFT 4
#define  XENA_XIP232_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP232_RESERVED3_MASK) >> XENA_XIP232_RESERVED3_SHIFT)
#define  XENA_XIP232_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP232_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP232_RESERVED3_SHIFT))

#define  XENA_XIP232_PWMSEL_MASK 0xf
#define  XENA_XIP232_PWMSEL_SHIFT 0
#define  XENA_XIP232_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP232_PWMSEL_MASK) >> XENA_XIP232_PWMSEL_SHIFT)
#define  XENA_XIP232_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP232_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP232_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 34 of 52) (XIP233)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 34 of 52)*/
//====================================================================

#define  XENA_XIP233_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP233_RESERVED1_SHIFT 17
#define  XENA_XIP233_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP233_RESERVED1_MASK) >> XENA_XIP233_RESERVED1_SHIFT)
#define  XENA_XIP233_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP233_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP233_RESERVED1_SHIFT))

#define  XENA_XIP233_PWMMODE_MASK 0x10000
#define  XENA_XIP233_PWMMODE_SHIFT 16
#define  XENA_XIP233_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP233_PWMMODE_MASK) >> XENA_XIP233_PWMMODE_SHIFT)
#define  XENA_XIP233_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP233_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP233_PWMMODE_SHIFT))

#define  XENA_XIP233_RESERVED2_MASK 0xff00
#define  XENA_XIP233_RESERVED2_SHIFT 8
#define  XENA_XIP233_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP233_RESERVED2_MASK) >> XENA_XIP233_RESERVED2_SHIFT)
#define  XENA_XIP233_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP233_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP233_RESERVED2_SHIFT))

#define  XENA_XIP233_PWMEN_MASK 0x80
#define  XENA_XIP233_PWMEN_SHIFT 7
#define  XENA_XIP233_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP233_PWMEN_MASK) >> XENA_XIP233_PWMEN_SHIFT)
#define  XENA_XIP233_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP233_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP233_PWMEN_SHIFT))

#define  XENA_XIP233_RESERVED3_MASK 0x70
#define  XENA_XIP233_RESERVED3_SHIFT 4
#define  XENA_XIP233_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP233_RESERVED3_MASK) >> XENA_XIP233_RESERVED3_SHIFT)
#define  XENA_XIP233_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP233_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP233_RESERVED3_SHIFT))

#define  XENA_XIP233_PWMSEL_MASK 0xf
#define  XENA_XIP233_PWMSEL_SHIFT 0
#define  XENA_XIP233_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP233_PWMSEL_MASK) >> XENA_XIP233_PWMSEL_SHIFT)
#define  XENA_XIP233_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP233_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP233_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 35 of 52) (XIP234)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 35 of 52)*/
//====================================================================

#define  XENA_XIP234_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP234_RESERVED1_SHIFT 17
#define  XENA_XIP234_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP234_RESERVED1_MASK) >> XENA_XIP234_RESERVED1_SHIFT)
#define  XENA_XIP234_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP234_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP234_RESERVED1_SHIFT))

#define  XENA_XIP234_PWMMODE_MASK 0x10000
#define  XENA_XIP234_PWMMODE_SHIFT 16
#define  XENA_XIP234_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP234_PWMMODE_MASK) >> XENA_XIP234_PWMMODE_SHIFT)
#define  XENA_XIP234_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP234_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP234_PWMMODE_SHIFT))

#define  XENA_XIP234_RESERVED2_MASK 0xff00
#define  XENA_XIP234_RESERVED2_SHIFT 8
#define  XENA_XIP234_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP234_RESERVED2_MASK) >> XENA_XIP234_RESERVED2_SHIFT)
#define  XENA_XIP234_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP234_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP234_RESERVED2_SHIFT))

#define  XENA_XIP234_PWMEN_MASK 0x80
#define  XENA_XIP234_PWMEN_SHIFT 7
#define  XENA_XIP234_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP234_PWMEN_MASK) >> XENA_XIP234_PWMEN_SHIFT)
#define  XENA_XIP234_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP234_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP234_PWMEN_SHIFT))

#define  XENA_XIP234_RESERVED3_MASK 0x70
#define  XENA_XIP234_RESERVED3_SHIFT 4
#define  XENA_XIP234_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP234_RESERVED3_MASK) >> XENA_XIP234_RESERVED3_SHIFT)
#define  XENA_XIP234_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP234_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP234_RESERVED3_SHIFT))

#define  XENA_XIP234_PWMSEL_MASK 0xf
#define  XENA_XIP234_PWMSEL_SHIFT 0
#define  XENA_XIP234_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP234_PWMSEL_MASK) >> XENA_XIP234_PWMSEL_SHIFT)
#define  XENA_XIP234_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP234_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP234_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 36 of 52) (XIP235)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 36 of 52)*/
//====================================================================

#define  XENA_XIP235_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP235_RESERVED1_SHIFT 17
#define  XENA_XIP235_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP235_RESERVED1_MASK) >> XENA_XIP235_RESERVED1_SHIFT)
#define  XENA_XIP235_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP235_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP235_RESERVED1_SHIFT))

#define  XENA_XIP235_PWMMODE_MASK 0x10000
#define  XENA_XIP235_PWMMODE_SHIFT 16
#define  XENA_XIP235_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP235_PWMMODE_MASK) >> XENA_XIP235_PWMMODE_SHIFT)
#define  XENA_XIP235_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP235_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP235_PWMMODE_SHIFT))

#define  XENA_XIP235_RESERVED2_MASK 0xff00
#define  XENA_XIP235_RESERVED2_SHIFT 8
#define  XENA_XIP235_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP235_RESERVED2_MASK) >> XENA_XIP235_RESERVED2_SHIFT)
#define  XENA_XIP235_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP235_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP235_RESERVED2_SHIFT))

#define  XENA_XIP235_PWMEN_MASK 0x80
#define  XENA_XIP235_PWMEN_SHIFT 7
#define  XENA_XIP235_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP235_PWMEN_MASK) >> XENA_XIP235_PWMEN_SHIFT)
#define  XENA_XIP235_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP235_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP235_PWMEN_SHIFT))

#define  XENA_XIP235_RESERVED3_MASK 0x70
#define  XENA_XIP235_RESERVED3_SHIFT 4
#define  XENA_XIP235_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP235_RESERVED3_MASK) >> XENA_XIP235_RESERVED3_SHIFT)
#define  XENA_XIP235_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP235_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP235_RESERVED3_SHIFT))

#define  XENA_XIP235_PWMSEL_MASK 0xf
#define  XENA_XIP235_PWMSEL_SHIFT 0
#define  XENA_XIP235_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP235_PWMSEL_MASK) >> XENA_XIP235_PWMSEL_SHIFT)
#define  XENA_XIP235_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP235_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP235_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 37 of 52) (XIP236)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 37 of 52)*/
//====================================================================

#define  XENA_XIP236_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP236_RESERVED1_SHIFT 17
#define  XENA_XIP236_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP236_RESERVED1_MASK) >> XENA_XIP236_RESERVED1_SHIFT)
#define  XENA_XIP236_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP236_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP236_RESERVED1_SHIFT))

#define  XENA_XIP236_PWMMODE_MASK 0x10000
#define  XENA_XIP236_PWMMODE_SHIFT 16
#define  XENA_XIP236_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP236_PWMMODE_MASK) >> XENA_XIP236_PWMMODE_SHIFT)
#define  XENA_XIP236_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP236_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP236_PWMMODE_SHIFT))

#define  XENA_XIP236_RESERVED2_MASK 0xff00
#define  XENA_XIP236_RESERVED2_SHIFT 8
#define  XENA_XIP236_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP236_RESERVED2_MASK) >> XENA_XIP236_RESERVED2_SHIFT)
#define  XENA_XIP236_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP236_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP236_RESERVED2_SHIFT))

#define  XENA_XIP236_PWMEN_MASK 0x80
#define  XENA_XIP236_PWMEN_SHIFT 7
#define  XENA_XIP236_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP236_PWMEN_MASK) >> XENA_XIP236_PWMEN_SHIFT)
#define  XENA_XIP236_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP236_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP236_PWMEN_SHIFT))

#define  XENA_XIP236_RESERVED3_MASK 0x70
#define  XENA_XIP236_RESERVED3_SHIFT 4
#define  XENA_XIP236_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP236_RESERVED3_MASK) >> XENA_XIP236_RESERVED3_SHIFT)
#define  XENA_XIP236_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP236_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP236_RESERVED3_SHIFT))

#define  XENA_XIP236_PWMSEL_MASK 0xf
#define  XENA_XIP236_PWMSEL_SHIFT 0
#define  XENA_XIP236_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP236_PWMSEL_MASK) >> XENA_XIP236_PWMSEL_SHIFT)
#define  XENA_XIP236_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP236_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP236_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 38 of 52) (XIP237)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 38 of 52)*/
//====================================================================

#define  XENA_XIP237_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP237_RESERVED1_SHIFT 17
#define  XENA_XIP237_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP237_RESERVED1_MASK) >> XENA_XIP237_RESERVED1_SHIFT)
#define  XENA_XIP237_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP237_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP237_RESERVED1_SHIFT))

#define  XENA_XIP237_PWMMODE_MASK 0x10000
#define  XENA_XIP237_PWMMODE_SHIFT 16
#define  XENA_XIP237_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP237_PWMMODE_MASK) >> XENA_XIP237_PWMMODE_SHIFT)
#define  XENA_XIP237_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP237_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP237_PWMMODE_SHIFT))

#define  XENA_XIP237_RESERVED2_MASK 0xff00
#define  XENA_XIP237_RESERVED2_SHIFT 8
#define  XENA_XIP237_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP237_RESERVED2_MASK) >> XENA_XIP237_RESERVED2_SHIFT)
#define  XENA_XIP237_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP237_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP237_RESERVED2_SHIFT))

#define  XENA_XIP237_PWMEN_MASK 0x80
#define  XENA_XIP237_PWMEN_SHIFT 7
#define  XENA_XIP237_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP237_PWMEN_MASK) >> XENA_XIP237_PWMEN_SHIFT)
#define  XENA_XIP237_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP237_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP237_PWMEN_SHIFT))

#define  XENA_XIP237_RESERVED3_MASK 0x70
#define  XENA_XIP237_RESERVED3_SHIFT 4
#define  XENA_XIP237_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP237_RESERVED3_MASK) >> XENA_XIP237_RESERVED3_SHIFT)
#define  XENA_XIP237_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP237_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP237_RESERVED3_SHIFT))

#define  XENA_XIP237_PWMSEL_MASK 0xf
#define  XENA_XIP237_PWMSEL_SHIFT 0
#define  XENA_XIP237_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP237_PWMSEL_MASK) >> XENA_XIP237_PWMSEL_SHIFT)
#define  XENA_XIP237_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP237_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP237_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 39 of 52) (XIP238)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 39 of 52)*/
//====================================================================

#define  XENA_XIP238_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP238_RESERVED1_SHIFT 17
#define  XENA_XIP238_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP238_RESERVED1_MASK) >> XENA_XIP238_RESERVED1_SHIFT)
#define  XENA_XIP238_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP238_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP238_RESERVED1_SHIFT))

#define  XENA_XIP238_PWMMODE_MASK 0x10000
#define  XENA_XIP238_PWMMODE_SHIFT 16
#define  XENA_XIP238_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP238_PWMMODE_MASK) >> XENA_XIP238_PWMMODE_SHIFT)
#define  XENA_XIP238_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP238_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP238_PWMMODE_SHIFT))

#define  XENA_XIP238_RESERVED2_MASK 0xff00
#define  XENA_XIP238_RESERVED2_SHIFT 8
#define  XENA_XIP238_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP238_RESERVED2_MASK) >> XENA_XIP238_RESERVED2_SHIFT)
#define  XENA_XIP238_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP238_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP238_RESERVED2_SHIFT))

#define  XENA_XIP238_PWMEN_MASK 0x80
#define  XENA_XIP238_PWMEN_SHIFT 7
#define  XENA_XIP238_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP238_PWMEN_MASK) >> XENA_XIP238_PWMEN_SHIFT)
#define  XENA_XIP238_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP238_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP238_PWMEN_SHIFT))

#define  XENA_XIP238_RESERVED3_MASK 0x70
#define  XENA_XIP238_RESERVED3_SHIFT 4
#define  XENA_XIP238_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP238_RESERVED3_MASK) >> XENA_XIP238_RESERVED3_SHIFT)
#define  XENA_XIP238_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP238_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP238_RESERVED3_SHIFT))

#define  XENA_XIP238_PWMSEL_MASK 0xf
#define  XENA_XIP238_PWMSEL_SHIFT 0
#define  XENA_XIP238_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP238_PWMSEL_MASK) >> XENA_XIP238_PWMSEL_SHIFT)
#define  XENA_XIP238_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP238_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP238_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 40 of 52) (XIP239)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 40 of 52)*/
//====================================================================

#define  XENA_XIP239_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP239_RESERVED1_SHIFT 17
#define  XENA_XIP239_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP239_RESERVED1_MASK) >> XENA_XIP239_RESERVED1_SHIFT)
#define  XENA_XIP239_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP239_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP239_RESERVED1_SHIFT))

#define  XENA_XIP239_PWMMODE_MASK 0x10000
#define  XENA_XIP239_PWMMODE_SHIFT 16
#define  XENA_XIP239_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP239_PWMMODE_MASK) >> XENA_XIP239_PWMMODE_SHIFT)
#define  XENA_XIP239_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP239_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP239_PWMMODE_SHIFT))

#define  XENA_XIP239_RESERVED2_MASK 0xff00
#define  XENA_XIP239_RESERVED2_SHIFT 8
#define  XENA_XIP239_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP239_RESERVED2_MASK) >> XENA_XIP239_RESERVED2_SHIFT)
#define  XENA_XIP239_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP239_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP239_RESERVED2_SHIFT))

#define  XENA_XIP239_PWMEN_MASK 0x80
#define  XENA_XIP239_PWMEN_SHIFT 7
#define  XENA_XIP239_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP239_PWMEN_MASK) >> XENA_XIP239_PWMEN_SHIFT)
#define  XENA_XIP239_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP239_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP239_PWMEN_SHIFT))

#define  XENA_XIP239_RESERVED3_MASK 0x70
#define  XENA_XIP239_RESERVED3_SHIFT 4
#define  XENA_XIP239_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP239_RESERVED3_MASK) >> XENA_XIP239_RESERVED3_SHIFT)
#define  XENA_XIP239_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP239_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP239_RESERVED3_SHIFT))

#define  XENA_XIP239_PWMSEL_MASK 0xf
#define  XENA_XIP239_PWMSEL_SHIFT 0
#define  XENA_XIP239_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP239_PWMSEL_MASK) >> XENA_XIP239_PWMSEL_SHIFT)
#define  XENA_XIP239_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP239_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP239_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 41 of 52) (XIP240)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 41 of 52)*/
//====================================================================

#define  XENA_XIP240_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP240_RESERVED1_SHIFT 17
#define  XENA_XIP240_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP240_RESERVED1_MASK) >> XENA_XIP240_RESERVED1_SHIFT)
#define  XENA_XIP240_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP240_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP240_RESERVED1_SHIFT))

#define  XENA_XIP240_PWMMODE_MASK 0x10000
#define  XENA_XIP240_PWMMODE_SHIFT 16
#define  XENA_XIP240_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP240_PWMMODE_MASK) >> XENA_XIP240_PWMMODE_SHIFT)
#define  XENA_XIP240_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP240_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP240_PWMMODE_SHIFT))

#define  XENA_XIP240_RESERVED2_MASK 0xff00
#define  XENA_XIP240_RESERVED2_SHIFT 8
#define  XENA_XIP240_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP240_RESERVED2_MASK) >> XENA_XIP240_RESERVED2_SHIFT)
#define  XENA_XIP240_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP240_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP240_RESERVED2_SHIFT))

#define  XENA_XIP240_PWMEN_MASK 0x80
#define  XENA_XIP240_PWMEN_SHIFT 7
#define  XENA_XIP240_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP240_PWMEN_MASK) >> XENA_XIP240_PWMEN_SHIFT)
#define  XENA_XIP240_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP240_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP240_PWMEN_SHIFT))

#define  XENA_XIP240_RESERVED3_MASK 0x70
#define  XENA_XIP240_RESERVED3_SHIFT 4
#define  XENA_XIP240_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP240_RESERVED3_MASK) >> XENA_XIP240_RESERVED3_SHIFT)
#define  XENA_XIP240_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP240_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP240_RESERVED3_SHIFT))

#define  XENA_XIP240_PWMSEL_MASK 0xf
#define  XENA_XIP240_PWMSEL_SHIFT 0
#define  XENA_XIP240_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP240_PWMSEL_MASK) >> XENA_XIP240_PWMSEL_SHIFT)
#define  XENA_XIP240_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP240_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP240_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 42 of 52) (XIP241)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 42 of 52)*/
//====================================================================

#define  XENA_XIP241_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP241_RESERVED1_SHIFT 17
#define  XENA_XIP241_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP241_RESERVED1_MASK) >> XENA_XIP241_RESERVED1_SHIFT)
#define  XENA_XIP241_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP241_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP241_RESERVED1_SHIFT))

#define  XENA_XIP241_PWMMODE_MASK 0x10000
#define  XENA_XIP241_PWMMODE_SHIFT 16
#define  XENA_XIP241_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP241_PWMMODE_MASK) >> XENA_XIP241_PWMMODE_SHIFT)
#define  XENA_XIP241_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP241_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP241_PWMMODE_SHIFT))

#define  XENA_XIP241_RESERVED2_MASK 0xff00
#define  XENA_XIP241_RESERVED2_SHIFT 8
#define  XENA_XIP241_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP241_RESERVED2_MASK) >> XENA_XIP241_RESERVED2_SHIFT)
#define  XENA_XIP241_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP241_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP241_RESERVED2_SHIFT))

#define  XENA_XIP241_PWMEN_MASK 0x80
#define  XENA_XIP241_PWMEN_SHIFT 7
#define  XENA_XIP241_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP241_PWMEN_MASK) >> XENA_XIP241_PWMEN_SHIFT)
#define  XENA_XIP241_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP241_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP241_PWMEN_SHIFT))

#define  XENA_XIP241_RESERVED3_MASK 0x70
#define  XENA_XIP241_RESERVED3_SHIFT 4
#define  XENA_XIP241_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP241_RESERVED3_MASK) >> XENA_XIP241_RESERVED3_SHIFT)
#define  XENA_XIP241_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP241_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP241_RESERVED3_SHIFT))

#define  XENA_XIP241_PWMSEL_MASK 0xf
#define  XENA_XIP241_PWMSEL_SHIFT 0
#define  XENA_XIP241_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP241_PWMSEL_MASK) >> XENA_XIP241_PWMSEL_SHIFT)
#define  XENA_XIP241_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP241_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP241_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 43 of 52) (XIP242)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 43 of 52)*/
//====================================================================

#define  XENA_XIP242_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP242_RESERVED1_SHIFT 17
#define  XENA_XIP242_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP242_RESERVED1_MASK) >> XENA_XIP242_RESERVED1_SHIFT)
#define  XENA_XIP242_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP242_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP242_RESERVED1_SHIFT))

#define  XENA_XIP242_PWMMODE_MASK 0x10000
#define  XENA_XIP242_PWMMODE_SHIFT 16
#define  XENA_XIP242_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP242_PWMMODE_MASK) >> XENA_XIP242_PWMMODE_SHIFT)
#define  XENA_XIP242_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP242_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP242_PWMMODE_SHIFT))

#define  XENA_XIP242_RESERVED2_MASK 0xff00
#define  XENA_XIP242_RESERVED2_SHIFT 8
#define  XENA_XIP242_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP242_RESERVED2_MASK) >> XENA_XIP242_RESERVED2_SHIFT)
#define  XENA_XIP242_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP242_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP242_RESERVED2_SHIFT))

#define  XENA_XIP242_PWMEN_MASK 0x80
#define  XENA_XIP242_PWMEN_SHIFT 7
#define  XENA_XIP242_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP242_PWMEN_MASK) >> XENA_XIP242_PWMEN_SHIFT)
#define  XENA_XIP242_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP242_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP242_PWMEN_SHIFT))

#define  XENA_XIP242_RESERVED3_MASK 0x70
#define  XENA_XIP242_RESERVED3_SHIFT 4
#define  XENA_XIP242_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP242_RESERVED3_MASK) >> XENA_XIP242_RESERVED3_SHIFT)
#define  XENA_XIP242_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP242_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP242_RESERVED3_SHIFT))

#define  XENA_XIP242_PWMSEL_MASK 0xf
#define  XENA_XIP242_PWMSEL_SHIFT 0
#define  XENA_XIP242_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP242_PWMSEL_MASK) >> XENA_XIP242_PWMSEL_SHIFT)
#define  XENA_XIP242_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP242_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP242_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 44 of 52) (XIP243)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 44 of 52)*/
//====================================================================

#define  XENA_XIP243_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP243_RESERVED1_SHIFT 17
#define  XENA_XIP243_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP243_RESERVED1_MASK) >> XENA_XIP243_RESERVED1_SHIFT)
#define  XENA_XIP243_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP243_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP243_RESERVED1_SHIFT))

#define  XENA_XIP243_PWMMODE_MASK 0x10000
#define  XENA_XIP243_PWMMODE_SHIFT 16
#define  XENA_XIP243_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP243_PWMMODE_MASK) >> XENA_XIP243_PWMMODE_SHIFT)
#define  XENA_XIP243_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP243_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP243_PWMMODE_SHIFT))

#define  XENA_XIP243_RESERVED2_MASK 0xff00
#define  XENA_XIP243_RESERVED2_SHIFT 8
#define  XENA_XIP243_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP243_RESERVED2_MASK) >> XENA_XIP243_RESERVED2_SHIFT)
#define  XENA_XIP243_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP243_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP243_RESERVED2_SHIFT))

#define  XENA_XIP243_PWMEN_MASK 0x80
#define  XENA_XIP243_PWMEN_SHIFT 7
#define  XENA_XIP243_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP243_PWMEN_MASK) >> XENA_XIP243_PWMEN_SHIFT)
#define  XENA_XIP243_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP243_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP243_PWMEN_SHIFT))

#define  XENA_XIP243_RESERVED3_MASK 0x70
#define  XENA_XIP243_RESERVED3_SHIFT 4
#define  XENA_XIP243_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP243_RESERVED3_MASK) >> XENA_XIP243_RESERVED3_SHIFT)
#define  XENA_XIP243_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP243_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP243_RESERVED3_SHIFT))

#define  XENA_XIP243_PWMSEL_MASK 0xf
#define  XENA_XIP243_PWMSEL_SHIFT 0
#define  XENA_XIP243_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP243_PWMSEL_MASK) >> XENA_XIP243_PWMSEL_SHIFT)
#define  XENA_XIP243_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP243_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP243_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 45 of 52) (XIP244)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 45 of 52)*/
//====================================================================

#define  XENA_XIP244_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP244_RESERVED1_SHIFT 17
#define  XENA_XIP244_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP244_RESERVED1_MASK) >> XENA_XIP244_RESERVED1_SHIFT)
#define  XENA_XIP244_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP244_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP244_RESERVED1_SHIFT))

#define  XENA_XIP244_PWMMODE_MASK 0x10000
#define  XENA_XIP244_PWMMODE_SHIFT 16
#define  XENA_XIP244_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP244_PWMMODE_MASK) >> XENA_XIP244_PWMMODE_SHIFT)
#define  XENA_XIP244_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP244_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP244_PWMMODE_SHIFT))

#define  XENA_XIP244_RESERVED2_MASK 0xff00
#define  XENA_XIP244_RESERVED2_SHIFT 8
#define  XENA_XIP244_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP244_RESERVED2_MASK) >> XENA_XIP244_RESERVED2_SHIFT)
#define  XENA_XIP244_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP244_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP244_RESERVED2_SHIFT))

#define  XENA_XIP244_PWMEN_MASK 0x80
#define  XENA_XIP244_PWMEN_SHIFT 7
#define  XENA_XIP244_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP244_PWMEN_MASK) >> XENA_XIP244_PWMEN_SHIFT)
#define  XENA_XIP244_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP244_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP244_PWMEN_SHIFT))

#define  XENA_XIP244_RESERVED3_MASK 0x70
#define  XENA_XIP244_RESERVED3_SHIFT 4
#define  XENA_XIP244_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP244_RESERVED3_MASK) >> XENA_XIP244_RESERVED3_SHIFT)
#define  XENA_XIP244_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP244_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP244_RESERVED3_SHIFT))

#define  XENA_XIP244_PWMSEL_MASK 0xf
#define  XENA_XIP244_PWMSEL_SHIFT 0
#define  XENA_XIP244_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP244_PWMSEL_MASK) >> XENA_XIP244_PWMSEL_SHIFT)
#define  XENA_XIP244_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP244_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP244_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 46 of 52) (XIP245)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 46 of 52)*/
//====================================================================

#define  XENA_XIP245_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP245_RESERVED1_SHIFT 17
#define  XENA_XIP245_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP245_RESERVED1_MASK) >> XENA_XIP245_RESERVED1_SHIFT)
#define  XENA_XIP245_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP245_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP245_RESERVED1_SHIFT))

#define  XENA_XIP245_PWMMODE_MASK 0x10000
#define  XENA_XIP245_PWMMODE_SHIFT 16
#define  XENA_XIP245_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP245_PWMMODE_MASK) >> XENA_XIP245_PWMMODE_SHIFT)
#define  XENA_XIP245_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP245_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP245_PWMMODE_SHIFT))

#define  XENA_XIP245_RESERVED2_MASK 0xff00
#define  XENA_XIP245_RESERVED2_SHIFT 8
#define  XENA_XIP245_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP245_RESERVED2_MASK) >> XENA_XIP245_RESERVED2_SHIFT)
#define  XENA_XIP245_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP245_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP245_RESERVED2_SHIFT))

#define  XENA_XIP245_PWMEN_MASK 0x80
#define  XENA_XIP245_PWMEN_SHIFT 7
#define  XENA_XIP245_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP245_PWMEN_MASK) >> XENA_XIP245_PWMEN_SHIFT)
#define  XENA_XIP245_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP245_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP245_PWMEN_SHIFT))

#define  XENA_XIP245_RESERVED3_MASK 0x70
#define  XENA_XIP245_RESERVED3_SHIFT 4
#define  XENA_XIP245_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP245_RESERVED3_MASK) >> XENA_XIP245_RESERVED3_SHIFT)
#define  XENA_XIP245_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP245_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP245_RESERVED3_SHIFT))

#define  XENA_XIP245_PWMSEL_MASK 0xf
#define  XENA_XIP245_PWMSEL_SHIFT 0
#define  XENA_XIP245_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP245_PWMSEL_MASK) >> XENA_XIP245_PWMSEL_SHIFT)
#define  XENA_XIP245_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP245_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP245_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 47 of 52) (XIP246)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 47 of 52)*/
//====================================================================

#define  XENA_XIP246_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP246_RESERVED1_SHIFT 17
#define  XENA_XIP246_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP246_RESERVED1_MASK) >> XENA_XIP246_RESERVED1_SHIFT)
#define  XENA_XIP246_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP246_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP246_RESERVED1_SHIFT))

#define  XENA_XIP246_PWMMODE_MASK 0x10000
#define  XENA_XIP246_PWMMODE_SHIFT 16
#define  XENA_XIP246_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP246_PWMMODE_MASK) >> XENA_XIP246_PWMMODE_SHIFT)
#define  XENA_XIP246_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP246_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP246_PWMMODE_SHIFT))

#define  XENA_XIP246_RESERVED2_MASK 0xff00
#define  XENA_XIP246_RESERVED2_SHIFT 8
#define  XENA_XIP246_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP246_RESERVED2_MASK) >> XENA_XIP246_RESERVED2_SHIFT)
#define  XENA_XIP246_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP246_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP246_RESERVED2_SHIFT))

#define  XENA_XIP246_PWMEN_MASK 0x80
#define  XENA_XIP246_PWMEN_SHIFT 7
#define  XENA_XIP246_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP246_PWMEN_MASK) >> XENA_XIP246_PWMEN_SHIFT)
#define  XENA_XIP246_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP246_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP246_PWMEN_SHIFT))

#define  XENA_XIP246_RESERVED3_MASK 0x70
#define  XENA_XIP246_RESERVED3_SHIFT 4
#define  XENA_XIP246_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP246_RESERVED3_MASK) >> XENA_XIP246_RESERVED3_SHIFT)
#define  XENA_XIP246_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP246_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP246_RESERVED3_SHIFT))

#define  XENA_XIP246_PWMSEL_MASK 0xf
#define  XENA_XIP246_PWMSEL_SHIFT 0
#define  XENA_XIP246_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP246_PWMSEL_MASK) >> XENA_XIP246_PWMSEL_SHIFT)
#define  XENA_XIP246_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP246_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP246_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 48 of 52) (XIP247)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 48 of 52)*/
//====================================================================

#define  XENA_XIP247_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP247_RESERVED1_SHIFT 17
#define  XENA_XIP247_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP247_RESERVED1_MASK) >> XENA_XIP247_RESERVED1_SHIFT)
#define  XENA_XIP247_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP247_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP247_RESERVED1_SHIFT))

#define  XENA_XIP247_PWMMODE_MASK 0x10000
#define  XENA_XIP247_PWMMODE_SHIFT 16
#define  XENA_XIP247_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP247_PWMMODE_MASK) >> XENA_XIP247_PWMMODE_SHIFT)
#define  XENA_XIP247_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP247_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP247_PWMMODE_SHIFT))

#define  XENA_XIP247_RESERVED2_MASK 0xff00
#define  XENA_XIP247_RESERVED2_SHIFT 8
#define  XENA_XIP247_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP247_RESERVED2_MASK) >> XENA_XIP247_RESERVED2_SHIFT)
#define  XENA_XIP247_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP247_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP247_RESERVED2_SHIFT))

#define  XENA_XIP247_PWMEN_MASK 0x80
#define  XENA_XIP247_PWMEN_SHIFT 7
#define  XENA_XIP247_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP247_PWMEN_MASK) >> XENA_XIP247_PWMEN_SHIFT)
#define  XENA_XIP247_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP247_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP247_PWMEN_SHIFT))

#define  XENA_XIP247_RESERVED3_MASK 0x70
#define  XENA_XIP247_RESERVED3_SHIFT 4
#define  XENA_XIP247_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP247_RESERVED3_MASK) >> XENA_XIP247_RESERVED3_SHIFT)
#define  XENA_XIP247_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP247_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP247_RESERVED3_SHIFT))

#define  XENA_XIP247_PWMSEL_MASK 0xf
#define  XENA_XIP247_PWMSEL_SHIFT 0
#define  XENA_XIP247_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP247_PWMSEL_MASK) >> XENA_XIP247_PWMSEL_SHIFT)
#define  XENA_XIP247_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP247_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP247_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 49 of 52) (XIP248)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 49 of 52)*/
//====================================================================

#define  XENA_XIP248_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP248_RESERVED1_SHIFT 17
#define  XENA_XIP248_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP248_RESERVED1_MASK) >> XENA_XIP248_RESERVED1_SHIFT)
#define  XENA_XIP248_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP248_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP248_RESERVED1_SHIFT))

#define  XENA_XIP248_PWMMODE_MASK 0x10000
#define  XENA_XIP248_PWMMODE_SHIFT 16
#define  XENA_XIP248_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP248_PWMMODE_MASK) >> XENA_XIP248_PWMMODE_SHIFT)
#define  XENA_XIP248_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP248_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP248_PWMMODE_SHIFT))

#define  XENA_XIP248_RESERVED2_MASK 0xff00
#define  XENA_XIP248_RESERVED2_SHIFT 8
#define  XENA_XIP248_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP248_RESERVED2_MASK) >> XENA_XIP248_RESERVED2_SHIFT)
#define  XENA_XIP248_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP248_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP248_RESERVED2_SHIFT))

#define  XENA_XIP248_PWMEN_MASK 0x80
#define  XENA_XIP248_PWMEN_SHIFT 7
#define  XENA_XIP248_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP248_PWMEN_MASK) >> XENA_XIP248_PWMEN_SHIFT)
#define  XENA_XIP248_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP248_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP248_PWMEN_SHIFT))

#define  XENA_XIP248_RESERVED3_MASK 0x70
#define  XENA_XIP248_RESERVED3_SHIFT 4
#define  XENA_XIP248_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP248_RESERVED3_MASK) >> XENA_XIP248_RESERVED3_SHIFT)
#define  XENA_XIP248_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP248_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP248_RESERVED3_SHIFT))

#define  XENA_XIP248_PWMSEL_MASK 0xf
#define  XENA_XIP248_PWMSEL_SHIFT 0
#define  XENA_XIP248_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP248_PWMSEL_MASK) >> XENA_XIP248_PWMSEL_SHIFT)
#define  XENA_XIP248_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP248_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP248_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 50 of 52) (XIP249)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 50 of 52)*/
//====================================================================

#define  XENA_XIP249_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP249_RESERVED1_SHIFT 17
#define  XENA_XIP249_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP249_RESERVED1_MASK) >> XENA_XIP249_RESERVED1_SHIFT)
#define  XENA_XIP249_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP249_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP249_RESERVED1_SHIFT))

#define  XENA_XIP249_PWMMODE_MASK 0x10000
#define  XENA_XIP249_PWMMODE_SHIFT 16
#define  XENA_XIP249_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP249_PWMMODE_MASK) >> XENA_XIP249_PWMMODE_SHIFT)
#define  XENA_XIP249_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP249_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP249_PWMMODE_SHIFT))

#define  XENA_XIP249_RESERVED2_MASK 0xff00
#define  XENA_XIP249_RESERVED2_SHIFT 8
#define  XENA_XIP249_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP249_RESERVED2_MASK) >> XENA_XIP249_RESERVED2_SHIFT)
#define  XENA_XIP249_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP249_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP249_RESERVED2_SHIFT))

#define  XENA_XIP249_PWMEN_MASK 0x80
#define  XENA_XIP249_PWMEN_SHIFT 7
#define  XENA_XIP249_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP249_PWMEN_MASK) >> XENA_XIP249_PWMEN_SHIFT)
#define  XENA_XIP249_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP249_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP249_PWMEN_SHIFT))

#define  XENA_XIP249_RESERVED3_MASK 0x70
#define  XENA_XIP249_RESERVED3_SHIFT 4
#define  XENA_XIP249_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP249_RESERVED3_MASK) >> XENA_XIP249_RESERVED3_SHIFT)
#define  XENA_XIP249_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP249_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP249_RESERVED3_SHIFT))

#define  XENA_XIP249_PWMSEL_MASK 0xf
#define  XENA_XIP249_PWMSEL_SHIFT 0
#define  XENA_XIP249_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP249_PWMSEL_MASK) >> XENA_XIP249_PWMSEL_SHIFT)
#define  XENA_XIP249_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP249_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP249_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 51 of 52) (XIP250)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 51 of 52)*/
//====================================================================

#define  XENA_XIP250_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP250_RESERVED1_SHIFT 17
#define  XENA_XIP250_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP250_RESERVED1_MASK) >> XENA_XIP250_RESERVED1_SHIFT)
#define  XENA_XIP250_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP250_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP250_RESERVED1_SHIFT))

#define  XENA_XIP250_PWMMODE_MASK 0x10000
#define  XENA_XIP250_PWMMODE_SHIFT 16
#define  XENA_XIP250_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP250_PWMMODE_MASK) >> XENA_XIP250_PWMMODE_SHIFT)
#define  XENA_XIP250_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP250_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP250_PWMMODE_SHIFT))

#define  XENA_XIP250_RESERVED2_MASK 0xff00
#define  XENA_XIP250_RESERVED2_SHIFT 8
#define  XENA_XIP250_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP250_RESERVED2_MASK) >> XENA_XIP250_RESERVED2_SHIFT)
#define  XENA_XIP250_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP250_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP250_RESERVED2_SHIFT))

#define  XENA_XIP250_PWMEN_MASK 0x80
#define  XENA_XIP250_PWMEN_SHIFT 7
#define  XENA_XIP250_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP250_PWMEN_MASK) >> XENA_XIP250_PWMEN_SHIFT)
#define  XENA_XIP250_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP250_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP250_PWMEN_SHIFT))

#define  XENA_XIP250_RESERVED3_MASK 0x70
#define  XENA_XIP250_RESERVED3_SHIFT 4
#define  XENA_XIP250_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP250_RESERVED3_MASK) >> XENA_XIP250_RESERVED3_SHIFT)
#define  XENA_XIP250_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP250_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP250_RESERVED3_SHIFT))

#define  XENA_XIP250_PWMSEL_MASK 0xf
#define  XENA_XIP250_PWMSEL_SHIFT 0
#define  XENA_XIP250_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP250_PWMSEL_MASK) >> XENA_XIP250_PWMSEL_SHIFT)
#define  XENA_XIP250_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP250_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP250_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO IO PWM Config Register2 (Private) (Instance 52 of 52) (XIP251)
/** \brief The UPCIO IO PWM Config Register2 contains the select field to control which ClkGen block is driving the respective PWM, an enable bit to globally control the start and stop of the PWM and the mode bit which selects whether the PWM is operating in 8 or 16 phase mode. (Instance 52 of 52)*/
//====================================================================

#define  XENA_XIP251_RESERVED1_MASK 0xfffe0000
#define  XENA_XIP251_RESERVED1_SHIFT 17
#define  XENA_XIP251_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIP251_RESERVED1_MASK) >> XENA_XIP251_RESERVED1_SHIFT)
#define  XENA_XIP251_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP251_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIP251_RESERVED1_SHIFT))

#define  XENA_XIP251_PWMMODE_MASK 0x10000
#define  XENA_XIP251_PWMMODE_SHIFT 16
#define  XENA_XIP251_PWMMODE_MASK_SHIFT(reg) (((reg) & XENA_XIP251_PWMMODE_MASK) >> XENA_XIP251_PWMMODE_SHIFT)
#define  XENA_XIP251_PWMMODE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP251_PWMMODE_MASK) | (((uint32_t)val) << XENA_XIP251_PWMMODE_SHIFT))

#define  XENA_XIP251_RESERVED2_MASK 0xff00
#define  XENA_XIP251_RESERVED2_SHIFT 8
#define  XENA_XIP251_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIP251_RESERVED2_MASK) >> XENA_XIP251_RESERVED2_SHIFT)
#define  XENA_XIP251_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP251_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIP251_RESERVED2_SHIFT))

#define  XENA_XIP251_PWMEN_MASK 0x80
#define  XENA_XIP251_PWMEN_SHIFT 7
#define  XENA_XIP251_PWMEN_MASK_SHIFT(reg) (((reg) & XENA_XIP251_PWMEN_MASK) >> XENA_XIP251_PWMEN_SHIFT)
#define  XENA_XIP251_PWMEN_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP251_PWMEN_MASK) | (((uint32_t)val) << XENA_XIP251_PWMEN_SHIFT))

#define  XENA_XIP251_RESERVED3_MASK 0x70
#define  XENA_XIP251_RESERVED3_SHIFT 4
#define  XENA_XIP251_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIP251_RESERVED3_MASK) >> XENA_XIP251_RESERVED3_SHIFT)
#define  XENA_XIP251_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP251_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIP251_RESERVED3_SHIFT))

#define  XENA_XIP251_PWMSEL_MASK 0xf
#define  XENA_XIP251_PWMSEL_SHIFT 0
#define  XENA_XIP251_PWMSEL_MASK_SHIFT(reg) (((reg) & XENA_XIP251_PWMSEL_MASK) >> XENA_XIP251_PWMSEL_SHIFT)
#define  XENA_XIP251_PWMSEL_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIP251_PWMSEL_MASK) | (((uint32_t)val) << XENA_XIP251_PWMSEL_SHIFT))

//====================================================================
//Register: UPCIO Bit-Bang Write Data Register (Private) (XIRW)
/** \brief UPCIO Bit-Bang Data Register*/
//====================================================================

#define  XENA_XIRW_REGWRDATA_MASK 0xffffffff
#define  XENA_XIRW_REGWRDATA_SHIFT 0
#define  XENA_XIRW_REGWRDATA_MASK_SHIFT(reg) (((reg) & XENA_XIRW_REGWRDATA_MASK) >> XENA_XIRW_REGWRDATA_SHIFT)
#define  XENA_XIRW_REGWRDATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIRW_REGWRDATA_MASK) | (((uint32_t)val) << XENA_XIRW_REGWRDATA_SHIFT))

//====================================================================
//Register: UPCIO Bit-Bang Read Data Register (Private) (XIRD)
/** \brief UPCIO Bit-Bang Read Data Register*/
//====================================================================

#define  XENA_XIRD_REGRDDATA_MASK 0xffffffff
#define  XENA_XIRD_REGRDDATA_SHIFT 0
#define  XENA_XIRD_REGRDDATA_MASK_SHIFT(reg) (((reg) & XENA_XIRD_REGRDDATA_MASK) >> XENA_XIRD_REGRDDATA_SHIFT)
#define  XENA_XIRD_REGRDDATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIRD_REGRDDATA_MASK) | (((uint32_t)val) << XENA_XIRD_REGRDDATA_SHIFT))

//====================================================================
//Register: UPCIO Bit-Bang Read Data Register 1 (Private) (XIRD1)
/** \brief UPCIO Bit-Bang Read Data Register 1*/
//====================================================================

#define  XENA_XIRD1_REGRDDATA_MASK 0xffffffff
#define  XENA_XIRD1_REGRDDATA_SHIFT 0
#define  XENA_XIRD1_REGRDDATA_MASK_SHIFT(reg) (((reg) & XENA_XIRD1_REGRDDATA_MASK) >> XENA_XIRD1_REGRDDATA_SHIFT)
#define  XENA_XIRD1_REGRDDATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIRD1_REGRDDATA_MASK) | (((uint32_t)val) << XENA_XIRD1_REGRDDATA_SHIFT))

//====================================================================
//Register: UPC I/O Encoder Source Selection Register 0 (Private) (XIEnSel0)
/** \brief The I/O Encoder Source Selection Register allows the input from an I/O Cell
      to be used in place of fire sync pulses generated by the Encoder Unit. This
      register covers fire sync pulses 0 - 3.*/
//====================================================================

#define  XENA_XIENSEL0_IOENABLE3_MASK 0x80000000
#define  XENA_XIENSEL0_IOENABLE3_SHIFT 31
#define  XENA_XIENSEL0_IOENABLE3_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOENABLE3_MASK) >> XENA_XIENSEL0_IOENABLE3_SHIFT)
#define  XENA_XIENSEL0_IOENABLE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOENABLE3_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOENABLE3_SHIFT))

#define  XENA_XIENSEL0_RESERVED1_MASK 0x40000000
#define  XENA_XIENSEL0_RESERVED1_SHIFT 30
#define  XENA_XIENSEL0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_RESERVED1_MASK) >> XENA_XIENSEL0_RESERVED1_SHIFT)
#define  XENA_XIENSEL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIENSEL0_RESERVED1_SHIFT))

#define  XENA_XIENSEL0_IOSEL3_MASK 0x3f000000
#define  XENA_XIENSEL0_IOSEL3_SHIFT 24
#define  XENA_XIENSEL0_IOSEL3_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOSEL3_MASK) >> XENA_XIENSEL0_IOSEL3_SHIFT)
#define  XENA_XIENSEL0_IOSEL3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOSEL3_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOSEL3_SHIFT))

#define  XENA_XIENSEL0_IOENABLE2_MASK 0x800000
#define  XENA_XIENSEL0_IOENABLE2_SHIFT 23
#define  XENA_XIENSEL0_IOENABLE2_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOENABLE2_MASK) >> XENA_XIENSEL0_IOENABLE2_SHIFT)
#define  XENA_XIENSEL0_IOENABLE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOENABLE2_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOENABLE2_SHIFT))

#define  XENA_XIENSEL0_RESERVED2_MASK 0x400000
#define  XENA_XIENSEL0_RESERVED2_SHIFT 22
#define  XENA_XIENSEL0_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_RESERVED2_MASK) >> XENA_XIENSEL0_RESERVED2_SHIFT)
#define  XENA_XIENSEL0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIENSEL0_RESERVED2_SHIFT))

#define  XENA_XIENSEL0_IOSEL2_MASK 0x3f0000
#define  XENA_XIENSEL0_IOSEL2_SHIFT 16
#define  XENA_XIENSEL0_IOSEL2_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOSEL2_MASK) >> XENA_XIENSEL0_IOSEL2_SHIFT)
#define  XENA_XIENSEL0_IOSEL2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOSEL2_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOSEL2_SHIFT))

#define  XENA_XIENSEL0_IOENABLE1_MASK 0x8000
#define  XENA_XIENSEL0_IOENABLE1_SHIFT 15
#define  XENA_XIENSEL0_IOENABLE1_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOENABLE1_MASK) >> XENA_XIENSEL0_IOENABLE1_SHIFT)
#define  XENA_XIENSEL0_IOENABLE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOENABLE1_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOENABLE1_SHIFT))

#define  XENA_XIENSEL0_RESERVED3_MASK 0x4000
#define  XENA_XIENSEL0_RESERVED3_SHIFT 14
#define  XENA_XIENSEL0_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_RESERVED3_MASK) >> XENA_XIENSEL0_RESERVED3_SHIFT)
#define  XENA_XIENSEL0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIENSEL0_RESERVED3_SHIFT))

#define  XENA_XIENSEL0_IOSEL1_MASK 0x3f00
#define  XENA_XIENSEL0_IOSEL1_SHIFT 8
#define  XENA_XIENSEL0_IOSEL1_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOSEL1_MASK) >> XENA_XIENSEL0_IOSEL1_SHIFT)
#define  XENA_XIENSEL0_IOSEL1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOSEL1_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOSEL1_SHIFT))

#define  XENA_XIENSEL0_IOENABLE0_MASK 0x80
#define  XENA_XIENSEL0_IOENABLE0_SHIFT 7
#define  XENA_XIENSEL0_IOENABLE0_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOENABLE0_MASK) >> XENA_XIENSEL0_IOENABLE0_SHIFT)
#define  XENA_XIENSEL0_IOENABLE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOENABLE0_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOENABLE0_SHIFT))

#define  XENA_XIENSEL0_RESERVED4_MASK 0x40
#define  XENA_XIENSEL0_RESERVED4_SHIFT 6
#define  XENA_XIENSEL0_RESERVED4_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_RESERVED4_MASK) >> XENA_XIENSEL0_RESERVED4_SHIFT)
#define  XENA_XIENSEL0_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_RESERVED4_MASK) | (((uint32_t)val) << XENA_XIENSEL0_RESERVED4_SHIFT))

#define  XENA_XIENSEL0_IOSEL0_MASK 0x3f
#define  XENA_XIENSEL0_IOSEL0_SHIFT 0
#define  XENA_XIENSEL0_IOSEL0_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL0_IOSEL0_MASK) >> XENA_XIENSEL0_IOSEL0_SHIFT)
#define  XENA_XIENSEL0_IOSEL0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL0_IOSEL0_MASK) | (((uint32_t)val) << XENA_XIENSEL0_IOSEL0_SHIFT))

//====================================================================
//Register: UPC I/O Encoder Source Selection Register 1 (Private) (XIEnSel1)
/** \brief The I/O Encoder Source Selection Register allows the input from an I/O Cell
      to be used in place of fire sync pulses generated by the Encoder Unit. This
      register covers fire sync pulses 4 - 7.*/
//====================================================================

#define  XENA_XIENSEL1_IOENABLE7_MASK 0x80000000
#define  XENA_XIENSEL1_IOENABLE7_SHIFT 31
#define  XENA_XIENSEL1_IOENABLE7_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOENABLE7_MASK) >> XENA_XIENSEL1_IOENABLE7_SHIFT)
#define  XENA_XIENSEL1_IOENABLE7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOENABLE7_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOENABLE7_SHIFT))

#define  XENA_XIENSEL1_RESERVED1_MASK 0x40000000
#define  XENA_XIENSEL1_RESERVED1_SHIFT 30
#define  XENA_XIENSEL1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_RESERVED1_MASK) >> XENA_XIENSEL1_RESERVED1_SHIFT)
#define  XENA_XIENSEL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIENSEL1_RESERVED1_SHIFT))

#define  XENA_XIENSEL1_IOSEL7_MASK 0x3f000000
#define  XENA_XIENSEL1_IOSEL7_SHIFT 24
#define  XENA_XIENSEL1_IOSEL7_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOSEL7_MASK) >> XENA_XIENSEL1_IOSEL7_SHIFT)
#define  XENA_XIENSEL1_IOSEL7_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOSEL7_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOSEL7_SHIFT))

#define  XENA_XIENSEL1_IOENABLE6_MASK 0x800000
#define  XENA_XIENSEL1_IOENABLE6_SHIFT 23
#define  XENA_XIENSEL1_IOENABLE6_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOENABLE6_MASK) >> XENA_XIENSEL1_IOENABLE6_SHIFT)
#define  XENA_XIENSEL1_IOENABLE6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOENABLE6_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOENABLE6_SHIFT))

#define  XENA_XIENSEL1_RESERVED2_MASK 0x400000
#define  XENA_XIENSEL1_RESERVED2_SHIFT 22
#define  XENA_XIENSEL1_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_RESERVED2_MASK) >> XENA_XIENSEL1_RESERVED2_SHIFT)
#define  XENA_XIENSEL1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIENSEL1_RESERVED2_SHIFT))

#define  XENA_XIENSEL1_IOSEL6_MASK 0x3f0000
#define  XENA_XIENSEL1_IOSEL6_SHIFT 16
#define  XENA_XIENSEL1_IOSEL6_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOSEL6_MASK) >> XENA_XIENSEL1_IOSEL6_SHIFT)
#define  XENA_XIENSEL1_IOSEL6_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOSEL6_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOSEL6_SHIFT))

#define  XENA_XIENSEL1_IOENABLE5_MASK 0x8000
#define  XENA_XIENSEL1_IOENABLE5_SHIFT 15
#define  XENA_XIENSEL1_IOENABLE5_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOENABLE5_MASK) >> XENA_XIENSEL1_IOENABLE5_SHIFT)
#define  XENA_XIENSEL1_IOENABLE5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOENABLE5_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOENABLE5_SHIFT))

#define  XENA_XIENSEL1_RESERVED3_MASK 0x4000
#define  XENA_XIENSEL1_RESERVED3_SHIFT 14
#define  XENA_XIENSEL1_RESERVED3_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_RESERVED3_MASK) >> XENA_XIENSEL1_RESERVED3_SHIFT)
#define  XENA_XIENSEL1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_RESERVED3_MASK) | (((uint32_t)val) << XENA_XIENSEL1_RESERVED3_SHIFT))

#define  XENA_XIENSEL1_IOSEL5_MASK 0x3f00
#define  XENA_XIENSEL1_IOSEL5_SHIFT 8
#define  XENA_XIENSEL1_IOSEL5_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOSEL5_MASK) >> XENA_XIENSEL1_IOSEL5_SHIFT)
#define  XENA_XIENSEL1_IOSEL5_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOSEL5_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOSEL5_SHIFT))

#define  XENA_XIENSEL1_IOENABLE4_MASK 0x80
#define  XENA_XIENSEL1_IOENABLE4_SHIFT 7
#define  XENA_XIENSEL1_IOENABLE4_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOENABLE4_MASK) >> XENA_XIENSEL1_IOENABLE4_SHIFT)
#define  XENA_XIENSEL1_IOENABLE4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOENABLE4_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOENABLE4_SHIFT))

#define  XENA_XIENSEL1_RESERVED4_MASK 0x40
#define  XENA_XIENSEL1_RESERVED4_SHIFT 6
#define  XENA_XIENSEL1_RESERVED4_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_RESERVED4_MASK) >> XENA_XIENSEL1_RESERVED4_SHIFT)
#define  XENA_XIENSEL1_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_RESERVED4_MASK) | (((uint32_t)val) << XENA_XIENSEL1_RESERVED4_SHIFT))

#define  XENA_XIENSEL1_IOSEL4_MASK 0x3f
#define  XENA_XIENSEL1_IOSEL4_SHIFT 0
#define  XENA_XIENSEL1_IOSEL4_MASK_SHIFT(reg) (((reg) & XENA_XIENSEL1_IOSEL4_MASK) >> XENA_XIENSEL1_IOSEL4_SHIFT)
#define  XENA_XIENSEL1_IOSEL4_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIENSEL1_IOSEL4_MASK) | (((uint32_t)val) << XENA_XIENSEL1_IOSEL4_SHIFT))

//====================================================================
//Register: UPCIO IO Read Sync select Register (Private) (XIRS)
/** \brief UPCIO IO Read Sync select Register*/
//====================================================================

#define  XENA_XIRS_RESERVED1_MASK 0xffffffe0
#define  XENA_XIRS_RESERVED1_SHIFT 5
#define  XENA_XIRS_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XIRS_RESERVED1_MASK) >> XENA_XIRS_RESERVED1_SHIFT)
#define  XENA_XIRS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIRS_RESERVED1_MASK) | (((uint32_t)val) << XENA_XIRS_RESERVED1_SHIFT))

#define  XENA_XIRS_REGSYNC_MASK 0x10
#define  XENA_XIRS_REGSYNC_SHIFT 4
#define  XENA_XIRS_REGSYNC_MASK_SHIFT(reg) (((reg) & XENA_XIRS_REGSYNC_MASK) >> XENA_XIRS_REGSYNC_SHIFT)
#define  XENA_XIRS_REGSYNC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIRS_REGSYNC_MASK) | (((uint32_t)val) << XENA_XIRS_REGSYNC_SHIFT))

#define  XENA_XIRS_RESERVED2_MASK 0x8
#define  XENA_XIRS_RESERVED2_SHIFT 3
#define  XENA_XIRS_RESERVED2_MASK_SHIFT(reg) (((reg) & XENA_XIRS_RESERVED2_MASK) >> XENA_XIRS_RESERVED2_SHIFT)
#define  XENA_XIRS_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIRS_RESERVED2_MASK) | (((uint32_t)val) << XENA_XIRS_RESERVED2_SHIFT))

#define  XENA_XIRS_REGSYNCSRC_MASK 0x7
#define  XENA_XIRS_REGSYNCSRC_SHIFT 0
#define  XENA_XIRS_REGSYNCSRC_MASK_SHIFT(reg) (((reg) & XENA_XIRS_REGSYNCSRC_MASK) >> XENA_XIRS_REGSYNCSRC_SHIFT)
#define  XENA_XIRS_REGSYNCSRC_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIRS_REGSYNCSRC_MASK) | (((uint32_t)val) << XENA_XIRS_REGSYNCSRC_SHIFT))

//====================================================================
//Register: UPCIO IO Interrupt Config Register (Private) (XIII)
/** \brief The UPCIO IO Interrupt Config Register controls which input can generate one of the four I/O interrupts. IRQ0 is controlled by bits 0-7, IRQ1 is controlled by bits 8-15, and so forth.*/
//====================================================================

#define  XENA_XIII_NEGEDGE3_MASK 0x80000000
#define  XENA_XIII_NEGEDGE3_SHIFT 31
#define  XENA_XIII_NEGEDGE3_MASK_SHIFT(reg) (((reg) & XENA_XIII_NEGEDGE3_MASK) >> XENA_XIII_NEGEDGE3_SHIFT)
#define  XENA_XIII_NEGEDGE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_NEGEDGE3_MASK) | (((uint32_t)val) << XENA_XIII_NEGEDGE3_SHIFT))

#define  XENA_XIII_POSEDGE3_MASK 0x40000000
#define  XENA_XIII_POSEDGE3_SHIFT 30
#define  XENA_XIII_POSEDGE3_MASK_SHIFT(reg) (((reg) & XENA_XIII_POSEDGE3_MASK) >> XENA_XIII_POSEDGE3_SHIFT)
#define  XENA_XIII_POSEDGE3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_POSEDGE3_MASK) | (((uint32_t)val) << XENA_XIII_POSEDGE3_SHIFT))

#define  XENA_XIII_INDEX3_MASK 0x3f000000
#define  XENA_XIII_INDEX3_SHIFT 24
#define  XENA_XIII_INDEX3_MASK_SHIFT(reg) (((reg) & XENA_XIII_INDEX3_MASK) >> XENA_XIII_INDEX3_SHIFT)
#define  XENA_XIII_INDEX3_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_INDEX3_MASK) | (((uint32_t)val) << XENA_XIII_INDEX3_SHIFT))

#define  XENA_XIII_NEGEDGE2_MASK 0x800000
#define  XENA_XIII_NEGEDGE2_SHIFT 23
#define  XENA_XIII_NEGEDGE2_MASK_SHIFT(reg) (((reg) & XENA_XIII_NEGEDGE2_MASK) >> XENA_XIII_NEGEDGE2_SHIFT)
#define  XENA_XIII_NEGEDGE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_NEGEDGE2_MASK) | (((uint32_t)val) << XENA_XIII_NEGEDGE2_SHIFT))

#define  XENA_XIII_POSEDGE2_MASK 0x400000
#define  XENA_XIII_POSEDGE2_SHIFT 22
#define  XENA_XIII_POSEDGE2_MASK_SHIFT(reg) (((reg) & XENA_XIII_POSEDGE2_MASK) >> XENA_XIII_POSEDGE2_SHIFT)
#define  XENA_XIII_POSEDGE2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_POSEDGE2_MASK) | (((uint32_t)val) << XENA_XIII_POSEDGE2_SHIFT))

#define  XENA_XIII_INDEX2_MASK 0x3f0000
#define  XENA_XIII_INDEX2_SHIFT 16
#define  XENA_XIII_INDEX2_MASK_SHIFT(reg) (((reg) & XENA_XIII_INDEX2_MASK) >> XENA_XIII_INDEX2_SHIFT)
#define  XENA_XIII_INDEX2_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_INDEX2_MASK) | (((uint32_t)val) << XENA_XIII_INDEX2_SHIFT))

#define  XENA_XIII_NEGEDGE1_MASK 0x8000
#define  XENA_XIII_NEGEDGE1_SHIFT 15
#define  XENA_XIII_NEGEDGE1_MASK_SHIFT(reg) (((reg) & XENA_XIII_NEGEDGE1_MASK) >> XENA_XIII_NEGEDGE1_SHIFT)
#define  XENA_XIII_NEGEDGE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_NEGEDGE1_MASK) | (((uint32_t)val) << XENA_XIII_NEGEDGE1_SHIFT))

#define  XENA_XIII_POSEDGE1_MASK 0x4000
#define  XENA_XIII_POSEDGE1_SHIFT 14
#define  XENA_XIII_POSEDGE1_MASK_SHIFT(reg) (((reg) & XENA_XIII_POSEDGE1_MASK) >> XENA_XIII_POSEDGE1_SHIFT)
#define  XENA_XIII_POSEDGE1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_POSEDGE1_MASK) | (((uint32_t)val) << XENA_XIII_POSEDGE1_SHIFT))

#define  XENA_XIII_INDEX1_MASK 0x3f00
#define  XENA_XIII_INDEX1_SHIFT 8
#define  XENA_XIII_INDEX1_MASK_SHIFT(reg) (((reg) & XENA_XIII_INDEX1_MASK) >> XENA_XIII_INDEX1_SHIFT)
#define  XENA_XIII_INDEX1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_INDEX1_MASK) | (((uint32_t)val) << XENA_XIII_INDEX1_SHIFT))

#define  XENA_XIII_NEGEDGE0_MASK 0x80
#define  XENA_XIII_NEGEDGE0_SHIFT 7
#define  XENA_XIII_NEGEDGE0_MASK_SHIFT(reg) (((reg) & XENA_XIII_NEGEDGE0_MASK) >> XENA_XIII_NEGEDGE0_SHIFT)
#define  XENA_XIII_NEGEDGE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_NEGEDGE0_MASK) | (((uint32_t)val) << XENA_XIII_NEGEDGE0_SHIFT))

#define  XENA_XIII_POSEDGE0_MASK 0x40
#define  XENA_XIII_POSEDGE0_SHIFT 6
#define  XENA_XIII_POSEDGE0_MASK_SHIFT(reg) (((reg) & XENA_XIII_POSEDGE0_MASK) >> XENA_XIII_POSEDGE0_SHIFT)
#define  XENA_XIII_POSEDGE0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_POSEDGE0_MASK) | (((uint32_t)val) << XENA_XIII_POSEDGE0_SHIFT))

#define  XENA_XIII_INDEX0_MASK 0x3f
#define  XENA_XIII_INDEX0_SHIFT 0
#define  XENA_XIII_INDEX0_MASK_SHIFT(reg) (((reg) & XENA_XIII_INDEX0_MASK) >> XENA_XIII_INDEX0_SHIFT)
#define  XENA_XIII_INDEX0_REPLACE_VAL(reg,val) (((reg) & ~XENA_XIII_INDEX0_MASK) | (((uint32_t)val) << XENA_XIII_INDEX0_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 1 of 8) (XISyncConfig0)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 1 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG0_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG0_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG0_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG0_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG0_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG0_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG0_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG0_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG0_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG0_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG0_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG0_PHASE_MASK) >> XENA_XISYNCCONFIG0_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG0_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG0_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG0_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG0_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG0_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG0_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG0_RESERVED1_MASK) >> XENA_XISYNCCONFIG0_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG0_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG0_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG0_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG0_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG0_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG0_PERIOD_MASK) >> XENA_XISYNCCONFIG0_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG0_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG0_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG0_PERIOD_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 2 of 8) (XISyncConfig1)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 2 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG1_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG1_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG1_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG1_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG1_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG1_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG1_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG1_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG1_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG1_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG1_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG1_PHASE_MASK) >> XENA_XISYNCCONFIG1_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG1_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG1_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG1_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG1_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG1_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG1_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG1_RESERVED1_MASK) >> XENA_XISYNCCONFIG1_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG1_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG1_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG1_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG1_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG1_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG1_PERIOD_MASK) >> XENA_XISYNCCONFIG1_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG1_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG1_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG1_PERIOD_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 3 of 8) (XISyncConfig2)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 3 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG2_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG2_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG2_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG2_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG2_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG2_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG2_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG2_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG2_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG2_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG2_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG2_PHASE_MASK) >> XENA_XISYNCCONFIG2_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG2_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG2_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG2_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG2_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG2_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG2_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG2_RESERVED1_MASK) >> XENA_XISYNCCONFIG2_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG2_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG2_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG2_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG2_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG2_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG2_PERIOD_MASK) >> XENA_XISYNCCONFIG2_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG2_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG2_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG2_PERIOD_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 4 of 8) (XISyncConfig3)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 4 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG3_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG3_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG3_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG3_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG3_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG3_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG3_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG3_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG3_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG3_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG3_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG3_PHASE_MASK) >> XENA_XISYNCCONFIG3_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG3_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG3_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG3_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG3_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG3_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG3_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG3_RESERVED1_MASK) >> XENA_XISYNCCONFIG3_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG3_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG3_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG3_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG3_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG3_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG3_PERIOD_MASK) >> XENA_XISYNCCONFIG3_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG3_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG3_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG3_PERIOD_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 5 of 8) (XISyncConfig4)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 5 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG4_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG4_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG4_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG4_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG4_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG4_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG4_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG4_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG4_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG4_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG4_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG4_PHASE_MASK) >> XENA_XISYNCCONFIG4_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG4_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG4_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG4_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG4_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG4_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG4_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG4_RESERVED1_MASK) >> XENA_XISYNCCONFIG4_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG4_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG4_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG4_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG4_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG4_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG4_PERIOD_MASK) >> XENA_XISYNCCONFIG4_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG4_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG4_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG4_PERIOD_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 6 of 8) (XISyncConfig5)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 6 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG5_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG5_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG5_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG5_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG5_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG5_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG5_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG5_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG5_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG5_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG5_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG5_PHASE_MASK) >> XENA_XISYNCCONFIG5_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG5_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG5_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG5_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG5_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG5_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG5_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG5_RESERVED1_MASK) >> XENA_XISYNCCONFIG5_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG5_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG5_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG5_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG5_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG5_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG5_PERIOD_MASK) >> XENA_XISYNCCONFIG5_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG5_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG5_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG5_PERIOD_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 7 of 8) (XISyncConfig6)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 7 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG6_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG6_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG6_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG6_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG6_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG6_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG6_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG6_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG6_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG6_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG6_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG6_PHASE_MASK) >> XENA_XISYNCCONFIG6_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG6_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG6_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG6_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG6_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG6_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG6_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG6_RESERVED1_MASK) >> XENA_XISYNCCONFIG6_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG6_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG6_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG6_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG6_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG6_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG6_PERIOD_MASK) >> XENA_XISYNCCONFIG6_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG6_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG6_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG6_PERIOD_SHIFT))

//====================================================================
//Register: UPCIO ClkGen Sync Configuration Register (Private) (Instance 8 of 8) (XISyncConfig7)
/** \brief UPCIO ClkGen Sync Configuration Register
<br/>
For example, if Period was 9 and Phase was 3
<br/>CG0 Output
<br/>_________|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_
<br/>CG0 Sync
<br/>_________________|__________________________|____________________
<br/>CG1 Output
<br/>_____________________________________________|__|__|__|__|__|__|_
<br/>CG1 Start
<br/>____________________________^____________________________________ (Instance 8 of 8)*/
//====================================================================

#define  XENA_XISYNCCONFIG7_CLKDONEMASK_MASK 0xff000000
#define  XENA_XISYNCCONFIG7_CLKDONEMASK_SHIFT 24
#define  XENA_XISYNCCONFIG7_CLKDONEMASK_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG7_CLKDONEMASK_MASK) >> XENA_XISYNCCONFIG7_CLKDONEMASK_SHIFT)
#define  XENA_XISYNCCONFIG7_CLKDONEMASK_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG7_CLKDONEMASK_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG7_CLKDONEMASK_SHIFT))

#define  XENA_XISYNCCONFIG7_PHASE_MASK 0xff0000
#define  XENA_XISYNCCONFIG7_PHASE_SHIFT 16
#define  XENA_XISYNCCONFIG7_PHASE_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG7_PHASE_MASK) >> XENA_XISYNCCONFIG7_PHASE_SHIFT)
#define  XENA_XISYNCCONFIG7_PHASE_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG7_PHASE_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG7_PHASE_SHIFT))

#define  XENA_XISYNCCONFIG7_RESERVED1_MASK 0xff00
#define  XENA_XISYNCCONFIG7_RESERVED1_SHIFT 8
#define  XENA_XISYNCCONFIG7_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG7_RESERVED1_MASK) >> XENA_XISYNCCONFIG7_RESERVED1_SHIFT)
#define  XENA_XISYNCCONFIG7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG7_RESERVED1_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG7_RESERVED1_SHIFT))

#define  XENA_XISYNCCONFIG7_PERIOD_MASK 0xff
#define  XENA_XISYNCCONFIG7_PERIOD_SHIFT 0
#define  XENA_XISYNCCONFIG7_PERIOD_MASK_SHIFT(reg) (((reg) & XENA_XISYNCCONFIG7_PERIOD_MASK) >> XENA_XISYNCCONFIG7_PERIOD_SHIFT)
#define  XENA_XISYNCCONFIG7_PERIOD_REPLACE_VAL(reg,val) (((reg) & ~XENA_XISYNCCONFIG7_PERIOD_MASK) | (((uint32_t)val) << XENA_XISYNCCONFIG7_PERIOD_SHIFT))

//====================================================================
//Register: UPC Safety Watchdog Safe Value (Private) (Instance 1 of 2) (SafetyData0)
/** \brief This value is forced onto UPC outputs if the Safety Watchdog
      Timer expires. The Safety Watchdog feature must be enabled in each
      IO Cell (via the respective XIIC register) for this to have effect. (Instance 1 of 2)*/
//====================================================================

#define  XENA_SAFETYDATA0_DATA_MASK 0xffffffff
#define  XENA_SAFETYDATA0_DATA_SHIFT 0
#define  XENA_SAFETYDATA0_DATA_MASK_SHIFT(reg) (((reg) & XENA_SAFETYDATA0_DATA_MASK) >> XENA_SAFETYDATA0_DATA_SHIFT)
#define  XENA_SAFETYDATA0_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_SAFETYDATA0_DATA_MASK) | (((uint32_t)val) << XENA_SAFETYDATA0_DATA_SHIFT))

//====================================================================
//Register: UPC Safety Watchdog Safe Value (Private) (Instance 2 of 2) (SafetyData1)
/** \brief This value is forced onto UPC outputs if the Safety Watchdog
      Timer expires. The Safety Watchdog feature must be enabled in each
      IO Cell (via the respective XIIC register) for this to have effect. (Instance 2 of 2)*/
//====================================================================

#define  XENA_SAFETYDATA1_DATA_MASK 0xffffffff
#define  XENA_SAFETYDATA1_DATA_SHIFT 0
#define  XENA_SAFETYDATA1_DATA_MASK_SHIFT(reg) (((reg) & XENA_SAFETYDATA1_DATA_MASK) >> XENA_SAFETYDATA1_DATA_SHIFT)
#define  XENA_SAFETYDATA1_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_SAFETYDATA1_DATA_MASK) | (((uint32_t)val) << XENA_SAFETYDATA1_DATA_SHIFT))

//====================================================================
//Register: UPC Safety Watchdog Safe Output Enable (Private) (Instance 1 of 2) (SafetyOE0)
/** \brief This value is forced onto UPC output enables if the Safety Watchdog
      Timer expires. The Safety Watchdog feature must be enabled in each
      IO Cell (via the respective XIIC register) for this to have effect. (Instance 1 of 2)*/
//====================================================================

#define  XENA_SAFETYOE0_DATA_MASK 0xffffffff
#define  XENA_SAFETYOE0_DATA_SHIFT 0
#define  XENA_SAFETYOE0_DATA_MASK_SHIFT(reg) (((reg) & XENA_SAFETYOE0_DATA_MASK) >> XENA_SAFETYOE0_DATA_SHIFT)
#define  XENA_SAFETYOE0_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_SAFETYOE0_DATA_MASK) | (((uint32_t)val) << XENA_SAFETYOE0_DATA_SHIFT))

//====================================================================
//Register: UPC Safety Watchdog Safe Output Enable (Private) (Instance 2 of 2) (SafetyOE1)
/** \brief This value is forced onto UPC output enables if the Safety Watchdog
      Timer expires. The Safety Watchdog feature must be enabled in each
      IO Cell (via the respective XIIC register) for this to have effect. (Instance 2 of 2)*/
//====================================================================

#define  XENA_SAFETYOE1_DATA_MASK 0xffffffff
#define  XENA_SAFETYOE1_DATA_SHIFT 0
#define  XENA_SAFETYOE1_DATA_MASK_SHIFT(reg) (((reg) & XENA_SAFETYOE1_DATA_MASK) >> XENA_SAFETYOE1_DATA_SHIFT)
#define  XENA_SAFETYOE1_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_SAFETYOE1_DATA_MASK) | (((uint32_t)val) << XENA_SAFETYOE1_DATA_SHIFT))

//====================================================================
//Register: UPC Safety Watchdog Timer (Private) (SafetyWatchdog)
/** \brief This timer counts down from the value written (at the rate of one
      per every UPC CPU clock) until the value hits zero. When the value hits
      zero, counting stops and "Safe" mode is entered. Safe mode forces
      the values in the SafetyData and SafetyOE registers
      onto UPC outputs for which safety has been enabled
      via their respective XIIC register.*/
//====================================================================

#define  XENA_SAFETYWATCHDOG_TIME_MASK 0xffffffff
#define  XENA_SAFETYWATCHDOG_TIME_SHIFT 0
#define  XENA_SAFETYWATCHDOG_TIME_MASK_SHIFT(reg) (((reg) & XENA_SAFETYWATCHDOG_TIME_MASK) >> XENA_SAFETYWATCHDOG_TIME_SHIFT)
#define  XENA_SAFETYWATCHDOG_TIME_REPLACE_VAL(reg,val) (((reg) & ~XENA_SAFETYWATCHDOG_TIME_MASK) | (((uint32_t)val) << XENA_SAFETYWATCHDOG_TIME_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 0 Firmware Event Register (PerfMon0FWEvent)
/** \brief This register allows firmware to cause a Performance Monitor
      "Event" to occur.*/
//====================================================================

#define  XENA_PERFMON0FWEVENT_RESERVED1_MASK 0xfffffffe
#define  XENA_PERFMON0FWEVENT_RESERVED1_SHIFT 1
#define  XENA_PERFMON0FWEVENT_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0FWEVENT_RESERVED1_MASK) >> XENA_PERFMON0FWEVENT_RESERVED1_SHIFT)
#define  XENA_PERFMON0FWEVENT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0FWEVENT_RESERVED1_MASK) | (((uint32_t)val) << XENA_PERFMON0FWEVENT_RESERVED1_SHIFT))

#define  XENA_PERFMON0FWEVENT_EVENT_MASK 0x1
#define  XENA_PERFMON0FWEVENT_EVENT_SHIFT 0
#define  XENA_PERFMON0FWEVENT_EVENT_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0FWEVENT_EVENT_MASK) >> XENA_PERFMON0FWEVENT_EVENT_SHIFT)
#define  XENA_PERFMON0FWEVENT_EVENT_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0FWEVENT_EVENT_MASK) | (((uint32_t)val) << XENA_PERFMON0FWEVENT_EVENT_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 0 Timer Register (PerfMon0Timer)
/** \brief This register accesses the Performance Monitor's timer.*/
//====================================================================

#define  XENA_PERFMON0TIMER_RESERVED1_MASK 0xff000000
#define  XENA_PERFMON0TIMER_RESERVED1_SHIFT 24
#define  XENA_PERFMON0TIMER_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0TIMER_RESERVED1_MASK) >> XENA_PERFMON0TIMER_RESERVED1_SHIFT)
#define  XENA_PERFMON0TIMER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0TIMER_RESERVED1_MASK) | (((uint32_t)val) << XENA_PERFMON0TIMER_RESERVED1_SHIFT))

#define  XENA_PERFMON0TIMER_TIMER_MASK 0xffffff
#define  XENA_PERFMON0TIMER_TIMER_SHIFT 0
#define  XENA_PERFMON0TIMER_TIMER_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0TIMER_TIMER_MASK) >> XENA_PERFMON0TIMER_TIMER_SHIFT)
#define  XENA_PERFMON0TIMER_TIMER_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0TIMER_TIMER_MASK) | (((uint32_t)val) << XENA_PERFMON0TIMER_TIMER_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 0 Start Enable (Instance 1 of 2) (PerfMon0StartEn0)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to start the Performance Monitor's timer. (Instance 1 of 2)*/
//====================================================================

#define  XENA_PERFMON0STARTEN0_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON0STARTEN0_ENABLE_SHIFT 0
#define  XENA_PERFMON0STARTEN0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0STARTEN0_ENABLE_MASK) >> XENA_PERFMON0STARTEN0_ENABLE_SHIFT)
#define  XENA_PERFMON0STARTEN0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0STARTEN0_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON0STARTEN0_ENABLE_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 0 Start Enable (Instance 2 of 2) (PerfMon0StartEn1)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to start the Performance Monitor's timer. (Instance 2 of 2)*/
//====================================================================

#define  XENA_PERFMON0STARTEN1_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON0STARTEN1_ENABLE_SHIFT 0
#define  XENA_PERFMON0STARTEN1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0STARTEN1_ENABLE_MASK) >> XENA_PERFMON0STARTEN1_ENABLE_SHIFT)
#define  XENA_PERFMON0STARTEN1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0STARTEN1_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON0STARTEN1_ENABLE_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 0 Stop Enable (Instance 1 of 2) (PerfMon0StopEn0)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to stop the Performance Monitor's timer. (Instance 1 of 2)*/
//====================================================================

#define  XENA_PERFMON0STOPEN0_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON0STOPEN0_ENABLE_SHIFT 0
#define  XENA_PERFMON0STOPEN0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0STOPEN0_ENABLE_MASK) >> XENA_PERFMON0STOPEN0_ENABLE_SHIFT)
#define  XENA_PERFMON0STOPEN0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0STOPEN0_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON0STOPEN0_ENABLE_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 0 Stop Enable (Instance 2 of 2) (PerfMon0StopEn1)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to stop the Performance Monitor's timer. (Instance 2 of 2)*/
//====================================================================

#define  XENA_PERFMON0STOPEN1_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON0STOPEN1_ENABLE_SHIFT 0
#define  XENA_PERFMON0STOPEN1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON0STOPEN1_ENABLE_MASK) >> XENA_PERFMON0STOPEN1_ENABLE_SHIFT)
#define  XENA_PERFMON0STOPEN1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON0STOPEN1_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON0STOPEN1_ENABLE_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 1 Firmware Event Register (PerfMon1FWEvent)
/** \brief This register allows firmware to cause a Performance Monitor
      "Event" to occur.*/
//====================================================================

#define  XENA_PERFMON1FWEVENT_RESERVED1_MASK 0xfffffffe
#define  XENA_PERFMON1FWEVENT_RESERVED1_SHIFT 1
#define  XENA_PERFMON1FWEVENT_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1FWEVENT_RESERVED1_MASK) >> XENA_PERFMON1FWEVENT_RESERVED1_SHIFT)
#define  XENA_PERFMON1FWEVENT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1FWEVENT_RESERVED1_MASK) | (((uint32_t)val) << XENA_PERFMON1FWEVENT_RESERVED1_SHIFT))

#define  XENA_PERFMON1FWEVENT_EVENT_MASK 0x1
#define  XENA_PERFMON1FWEVENT_EVENT_SHIFT 0
#define  XENA_PERFMON1FWEVENT_EVENT_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1FWEVENT_EVENT_MASK) >> XENA_PERFMON1FWEVENT_EVENT_SHIFT)
#define  XENA_PERFMON1FWEVENT_EVENT_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1FWEVENT_EVENT_MASK) | (((uint32_t)val) << XENA_PERFMON1FWEVENT_EVENT_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 1 Timer Register (PerfMon1Timer)
/** \brief This register accesses the Performance Monitor's timer.*/
//====================================================================

#define  XENA_PERFMON1TIMER_RESERVED1_MASK 0xff000000
#define  XENA_PERFMON1TIMER_RESERVED1_SHIFT 24
#define  XENA_PERFMON1TIMER_RESERVED1_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1TIMER_RESERVED1_MASK) >> XENA_PERFMON1TIMER_RESERVED1_SHIFT)
#define  XENA_PERFMON1TIMER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1TIMER_RESERVED1_MASK) | (((uint32_t)val) << XENA_PERFMON1TIMER_RESERVED1_SHIFT))

#define  XENA_PERFMON1TIMER_TIMER_MASK 0xffffff
#define  XENA_PERFMON1TIMER_TIMER_SHIFT 0
#define  XENA_PERFMON1TIMER_TIMER_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1TIMER_TIMER_MASK) >> XENA_PERFMON1TIMER_TIMER_SHIFT)
#define  XENA_PERFMON1TIMER_TIMER_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1TIMER_TIMER_MASK) | (((uint32_t)val) << XENA_PERFMON1TIMER_TIMER_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 1 Start Enable (Instance 1 of 2) (PerfMon1StartEn0)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to start the Performance Monitor's timer. (Instance 1 of 2)*/
//====================================================================

#define  XENA_PERFMON1STARTEN0_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON1STARTEN0_ENABLE_SHIFT 0
#define  XENA_PERFMON1STARTEN0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1STARTEN0_ENABLE_MASK) >> XENA_PERFMON1STARTEN0_ENABLE_SHIFT)
#define  XENA_PERFMON1STARTEN0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1STARTEN0_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON1STARTEN0_ENABLE_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 1 Start Enable (Instance 2 of 2) (PerfMon1StartEn1)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to start the Performance Monitor's timer. (Instance 2 of 2)*/
//====================================================================

#define  XENA_PERFMON1STARTEN1_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON1STARTEN1_ENABLE_SHIFT 0
#define  XENA_PERFMON1STARTEN1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1STARTEN1_ENABLE_MASK) >> XENA_PERFMON1STARTEN1_ENABLE_SHIFT)
#define  XENA_PERFMON1STARTEN1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1STARTEN1_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON1STARTEN1_ENABLE_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 1 Stop Enable (Instance 1 of 2) (PerfMon1StopEn0)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to stop the Performance Monitor's timer. (Instance 1 of 2)*/
//====================================================================

#define  XENA_PERFMON1STOPEN0_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON1STOPEN0_ENABLE_SHIFT 0
#define  XENA_PERFMON1STOPEN0_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1STOPEN0_ENABLE_MASK) >> XENA_PERFMON1STOPEN0_ENABLE_SHIFT)
#define  XENA_PERFMON1STOPEN0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1STOPEN0_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON1STOPEN0_ENABLE_SHIFT))

//====================================================================
//Register: UPC Performance Monitor 1 Stop Enable (Instance 2 of 2) (PerfMon1StopEn1)
/** \brief This register enables any of the UPC's internal (non-system) interrupt
      sources to stop the Performance Monitor's timer. (Instance 2 of 2)*/
//====================================================================

#define  XENA_PERFMON1STOPEN1_ENABLE_MASK 0xffffffff
#define  XENA_PERFMON1STOPEN1_ENABLE_SHIFT 0
#define  XENA_PERFMON1STOPEN1_ENABLE_MASK_SHIFT(reg) (((reg) & XENA_PERFMON1STOPEN1_ENABLE_MASK) >> XENA_PERFMON1STOPEN1_ENABLE_SHIFT)
#define  XENA_PERFMON1STOPEN1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~XENA_PERFMON1STOPEN1_ENABLE_MASK) | (((uint32_t)val) << XENA_PERFMON1STOPEN1_ENABLE_SHIFT))

//====================================================================
//Register: APB Register (Private) (APB)
/** \brief This register accesses the UPC's APB master port.
      The address range of this port is 4KB.*/
//====================================================================

#define  XENA_APB_DATA_MASK 0xffffffff
#define  XENA_APB_DATA_SHIFT 0
#define  XENA_APB_DATA_MASK_SHIFT(reg) (((reg) & XENA_APB_DATA_MASK) >> XENA_APB_DATA_SHIFT)
#define  XENA_APB_DATA_REPLACE_VAL(reg,val) (((reg) & ~XENA_APB_DATA_MASK) | (((uint32_t)val) << XENA_APB_DATA_SHIFT))

#endif // XENA
