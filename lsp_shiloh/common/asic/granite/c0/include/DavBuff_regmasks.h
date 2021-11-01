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
 * \file DavBuff_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DAVBUFF_REGMASKS_H_
#define _DAVBUFF_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Davinci Buffer (DavBuff)
/** \brief Top-level register file for Davinci Buffer block*/
//
//====================================================================

//====================================================================
//Register: Configuration Register (CFG)
/** \brief The Configuration Register is used to configure the PIP Buffer by choosing the operating mode. The PIP Buffer can run in one of four modes, Normal, Store, Restore, Debug. In Normal mode, the Davinci block controls the the line buffer interface, performing reads and writes when it needs data. In Store mode, the internal SRAM is written to the memory backing store, and the internal write buffer is flushed. In Restore mode, the internal SRAM is filled from the memory backing store. Finally, in Debug mode, FW can use the Debug registers to access the line buffers.*/
//====================================================================

#define  DAVBUFF_CFG_RESERVED1_MASK 0xfffffffc
#define  DAVBUFF_CFG_RESERVED1_SHIFT 2
#define  DAVBUFF_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_CFG_RESERVED1_MASK) >> DAVBUFF_CFG_RESERVED1_SHIFT)
#define  DAVBUFF_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_CFG_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_CFG_RESERVED1_SHIFT))

#define  DAVBUFF_CFG_MODE_MASK 0x3
#define  DAVBUFF_CFG_MODE_SHIFT 0
#define  DAVBUFF_CFG_MODE_MASK_SHIFT(reg) (((reg) & DAVBUFF_CFG_MODE_MASK) >> DAVBUFF_CFG_MODE_SHIFT)
#define  DAVBUFF_CFG_MODE_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_CFG_MODE_MASK) | (((uint32_t)val) << DAVBUFF_CFG_MODE_SHIFT))

//====================================================================
//Register: DMA Start Address (DMA_START)
/** \brief The memory start address for DMA operations. The DMA Start Address indicates where the backing store begins in memory. The backing store is the location in memory where the line buffer is saved off to free up room in the SRAM. FW must malloc a backing store with enough space to fit one full line of pixels and then write the start address to the DMA_START register.*/
//====================================================================

#define  DAVBUFF_DMA_START_STARTADDR_MASK 0xffffffff
#define  DAVBUFF_DMA_START_STARTADDR_SHIFT 0
#define  DAVBUFF_DMA_START_STARTADDR_MASK_SHIFT(reg) (((reg) & DAVBUFF_DMA_START_STARTADDR_MASK) >> DAVBUFF_DMA_START_STARTADDR_SHIFT)
#define  DAVBUFF_DMA_START_STARTADDR_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DMA_START_STARTADDR_MASK) | (((uint32_t)val) << DAVBUFF_DMA_START_STARTADDR_SHIFT))

//====================================================================
//Register: DMA End Addr (DMA_END)
/** \brief The memory end address for DMA operations. The DMA End Address indicates where the backing store ends in memory. FW must malloc a backing store with enough space to fit one full line of pixels and then write the end address to the DMA_END register. The end address is only used for error checking, if the DMA attempts to burst beyond the end address, the SegFault flag will be set in the STAT_READ register.*/
//====================================================================

#define  DAVBUFF_DMA_END_ENDADDR_MASK 0xffffffff
#define  DAVBUFF_DMA_END_ENDADDR_SHIFT 0
#define  DAVBUFF_DMA_END_ENDADDR_MASK_SHIFT(reg) (((reg) & DAVBUFF_DMA_END_ENDADDR_MASK) >> DAVBUFF_DMA_END_ENDADDR_SHIFT)
#define  DAVBUFF_DMA_END_ENDADDR_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DMA_END_ENDADDR_MASK) | (((uint32_t)val) << DAVBUFF_DMA_END_ENDADDR_SHIFT))

//====================================================================
//Register: Status Read Register (STAT_RD)
/** \brief Provides status information. The register is READ ONLY.*/
//====================================================================

#define  DAVBUFF_STAT_RD_RESERVED1_MASK 0xfffffffe
#define  DAVBUFF_STAT_RD_RESERVED1_SHIFT 1
#define  DAVBUFF_STAT_RD_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_STAT_RD_RESERVED1_MASK) >> DAVBUFF_STAT_RD_RESERVED1_SHIFT)
#define  DAVBUFF_STAT_RD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_STAT_RD_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_STAT_RD_RESERVED1_SHIFT))

#define  DAVBUFF_STAT_RD_SEGFAULT_MASK 0x1
#define  DAVBUFF_STAT_RD_SEGFAULT_SHIFT 0
#define  DAVBUFF_STAT_RD_SEGFAULT_MASK_SHIFT(reg) (((reg) & DAVBUFF_STAT_RD_SEGFAULT_MASK) >> DAVBUFF_STAT_RD_SEGFAULT_SHIFT)
#define  DAVBUFF_STAT_RD_SEGFAULT_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_STAT_RD_SEGFAULT_MASK) | (((uint32_t)val) << DAVBUFF_STAT_RD_SEGFAULT_SHIFT))

//====================================================================
//Register: Status Write Register (STAT_WR)
/** \brief Allows internal control signals to be changed. The register is WRITE ONLY.*/
//====================================================================

#define  DAVBUFF_STAT_WR_RESERVED1_MASK 0xfffffffe
#define  DAVBUFF_STAT_WR_RESERVED1_SHIFT 1
#define  DAVBUFF_STAT_WR_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_STAT_WR_RESERVED1_MASK) >> DAVBUFF_STAT_WR_RESERVED1_SHIFT)
#define  DAVBUFF_STAT_WR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_STAT_WR_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_STAT_WR_RESERVED1_SHIFT))

#define  DAVBUFF_STAT_WR_SEGFAULT_MASK 0x1
#define  DAVBUFF_STAT_WR_SEGFAULT_SHIFT 0
#define  DAVBUFF_STAT_WR_SEGFAULT_MASK_SHIFT(reg) (((reg) & DAVBUFF_STAT_WR_SEGFAULT_MASK) >> DAVBUFF_STAT_WR_SEGFAULT_SHIFT)
#define  DAVBUFF_STAT_WR_SEGFAULT_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_STAT_WR_SEGFAULT_MASK) | (((uint32_t)val) << DAVBUFF_STAT_WR_SEGFAULT_SHIFT))

//====================================================================
//Register: Store/Restore Control Register (SR_CTRL)
/** \brief Control register for the Store/Restore DMA operations. Provides a start bit which is used to kick of the store or restore operation. In addition, the Mode must be set to either Store or Restore using the CFG register. This register is WRITE ONLY.*/
//====================================================================

#define  DAVBUFF_SR_CTRL_RESERVED1_MASK 0xfffffffe
#define  DAVBUFF_SR_CTRL_RESERVED1_SHIFT 1
#define  DAVBUFF_SR_CTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_SR_CTRL_RESERVED1_MASK) >> DAVBUFF_SR_CTRL_RESERVED1_SHIFT)
#define  DAVBUFF_SR_CTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SR_CTRL_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_SR_CTRL_RESERVED1_SHIFT))

#define  DAVBUFF_SR_CTRL_SRSTART_MASK 0x1
#define  DAVBUFF_SR_CTRL_SRSTART_SHIFT 0
#define  DAVBUFF_SR_CTRL_SRSTART_MASK_SHIFT(reg) (((reg) & DAVBUFF_SR_CTRL_SRSTART_MASK) >> DAVBUFF_SR_CTRL_SRSTART_SHIFT)
#define  DAVBUFF_SR_CTRL_SRSTART_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SR_CTRL_SRSTART_MASK) | (((uint32_t)val) << DAVBUFF_SR_CTRL_SRSTART_SHIFT))

//====================================================================
//Register: Store/Restore Status Register (SR_STAT)
/** \brief Status register for the Store/Restore DMA operations. Provides a busy bit which will clear when the operation is complete. The register is READ ONLY.*/
//====================================================================

#define  DAVBUFF_SR_STAT_RESERVED1_MASK 0xfffffffe
#define  DAVBUFF_SR_STAT_RESERVED1_SHIFT 1
#define  DAVBUFF_SR_STAT_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_SR_STAT_RESERVED1_MASK) >> DAVBUFF_SR_STAT_RESERVED1_SHIFT)
#define  DAVBUFF_SR_STAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SR_STAT_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_SR_STAT_RESERVED1_SHIFT))

#define  DAVBUFF_SR_STAT_SRBUSY_MASK 0x1
#define  DAVBUFF_SR_STAT_SRBUSY_SHIFT 0
#define  DAVBUFF_SR_STAT_SRBUSY_MASK_SHIFT(reg) (((reg) & DAVBUFF_SR_STAT_SRBUSY_MASK) >> DAVBUFF_SR_STAT_SRBUSY_SHIFT)
#define  DAVBUFF_SR_STAT_SRBUSY_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SR_STAT_SRBUSY_MASK) | (((uint32_t)val) << DAVBUFF_SR_STAT_SRBUSY_SHIFT))

//====================================================================
//Register: Debug Address Register (DEBUG_PIX_ADDR)
/** \brief Allows the uP to access the pixel line buffer via the register interface. This register provides the pixel address which can fall within the SRAM or outside in the SRAM backing store. The register must be written before writing to the DEBUG_GO register which initiates the actual operation.*/
//====================================================================

#define  DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_MASK 0xffff0000
#define  DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_SHIFT 16
#define  DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_MASK) >> DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_SHIFT)
#define  DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_PIX_ADDR_RESERVED1_SHIFT))

#define  DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_MASK 0xffff
#define  DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_SHIFT 0
#define  DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_MASK) >> DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_SHIFT)
#define  DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_PIX_ADDR_DEBUGADDR_SHIFT))

//====================================================================
//Register: Debug Data Index Register (DEBUG_DATA_IDX)
/** \brief Allows the uP to access the Pixel Buffer via the register interface. This register provides the word index for reading or writing pixel data. For example, for an 84 bit pixel width, Index 0 corresponds to bits 31:0, Index 1 corresponds to bits 63:32, and Index 2 Corresponds to bits 83:64. The register needs to be set prior writing or reading from the DEBUG_DATA register to index into the pixel data.*/
//====================================================================

#define  DAVBUFF_DEBUG_DATA_IDX_RESERVED1_MASK 0xffffff00
#define  DAVBUFF_DEBUG_DATA_IDX_RESERVED1_SHIFT 8
#define  DAVBUFF_DEBUG_DATA_IDX_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_DATA_IDX_RESERVED1_MASK) >> DAVBUFF_DEBUG_DATA_IDX_RESERVED1_SHIFT)
#define  DAVBUFF_DEBUG_DATA_IDX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_DATA_IDX_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_DATA_IDX_RESERVED1_SHIFT))

#define  DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_MASK 0xff
#define  DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_SHIFT 0
#define  DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_MASK) >> DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_SHIFT)
#define  DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_DATA_IDX_DEBUGDATAIDX_SHIFT))

//====================================================================
//Register: Debug Data Write Register (DEBUG_DATA_WR)
/** \brief Allows the uP to access the Pixel Buffer via the register interface. This register provides the data interface in tandem with the DEBUG_DATA_IDX register. The DEBUG_DATA_IDX register is used to index into a pixel width width greater than 1 word. On a write, the data must be preloaded by writing to DEBUG_DATA (using DEBUG_DATA_IDX as an index), and the operation is then performed by writing to DEBUG_GO. On a read, DEBUG_GO must be written first, and then the pixel data can be read from DEBUG_DATA.*/
//====================================================================

#define  DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_MASK 0xffffffff
#define  DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_SHIFT 0
#define  DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_MASK) >> DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_SHIFT)
#define  DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_DATA_WR_DEBUGDATA_SHIFT))

//====================================================================
//Register: Debug Data Read Register (DEBUG_DATA_RD)
/** \brief Allows the uP to access the Pixel Buffer via the register interface. This register provides the data interface in tandem with the DEBUG_DATA_IDX register. The DEBUG_DATA_IDX register is used to index into a pixel width width greater than 1 word. On a write, the data must be preloaded by writing to DEBUG_DATA (using DEBUG_DATA_IDX as an index), and the operation is then performed by writing to DEBUG_GO. On a read, DEBUG_GO must be written first, and then the pixel data can be read from DEBUG_DATA.*/
//====================================================================

#define  DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_MASK 0xffffffff
#define  DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_SHIFT 0
#define  DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_MASK) >> DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_SHIFT)
#define  DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_DATA_RD_DEBUGDATA_SHIFT))

//====================================================================
//Register: Debug Go Write Register (DEBUG_GO)
/** \brief Allows the uP to access the Pixel Buffer via the register interface. This register is used to kick off the actual internal operation. On a pixel write, the address should be preloaded with DEBUG_PIX_ADDR and the data should be preloaded with DEBUG_DATA and DEBUG_DATA_IDX. The operation is then performed by writing a '1' to DEBUG_GO, and finishes when the Busy bit of DEBUG_STAT clears. On a pixel read, the address should be preloaded with DEBUG_PIX_ADDR and the operation perfromed by writing '0' to DEBUG_GO. When the operation finishes (Busy bit of DEBUG_STAT clears), the data can be read using DEBUG_DATA_IDX and DEBUG_DATA registers.*/
//====================================================================

#define  DAVBUFF_DEBUG_GO_RESERVED1_MASK 0xfffffffe
#define  DAVBUFF_DEBUG_GO_RESERVED1_SHIFT 1
#define  DAVBUFF_DEBUG_GO_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_GO_RESERVED1_MASK) >> DAVBUFF_DEBUG_GO_RESERVED1_SHIFT)
#define  DAVBUFF_DEBUG_GO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_GO_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_GO_RESERVED1_SHIFT))

#define  DAVBUFF_DEBUG_GO_DEBUGWNR_MASK 0x1
#define  DAVBUFF_DEBUG_GO_DEBUGWNR_SHIFT 0
#define  DAVBUFF_DEBUG_GO_DEBUGWNR_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_GO_DEBUGWNR_MASK) >> DAVBUFF_DEBUG_GO_DEBUGWNR_SHIFT)
#define  DAVBUFF_DEBUG_GO_DEBUGWNR_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_GO_DEBUGWNR_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_GO_DEBUGWNR_SHIFT))

//====================================================================
//Register: Debug Status Register (DEBUG_STAT)
/** \brief Allows the uP to access the Pixel Buffer via the register interface. This register is used monitor the status of the internal operation (kicked off using DEBUG_GO)*/
//====================================================================

#define  DAVBUFF_DEBUG_STAT_RESERVED1_MASK 0xfffffffe
#define  DAVBUFF_DEBUG_STAT_RESERVED1_SHIFT 1
#define  DAVBUFF_DEBUG_STAT_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_STAT_RESERVED1_MASK) >> DAVBUFF_DEBUG_STAT_RESERVED1_SHIFT)
#define  DAVBUFF_DEBUG_STAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_STAT_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_STAT_RESERVED1_SHIFT))

#define  DAVBUFF_DEBUG_STAT_DEBUGBUSY_MASK 0x1
#define  DAVBUFF_DEBUG_STAT_DEBUGBUSY_SHIFT 0
#define  DAVBUFF_DEBUG_STAT_DEBUGBUSY_MASK_SHIFT(reg) (((reg) & DAVBUFF_DEBUG_STAT_DEBUGBUSY_MASK) >> DAVBUFF_DEBUG_STAT_DEBUGBUSY_SHIFT)
#define  DAVBUFF_DEBUG_STAT_DEBUGBUSY_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_DEBUG_STAT_DEBUGBUSY_MASK) | (((uint32_t)val) << DAVBUFF_DEBUG_STAT_DEBUGBUSY_SHIFT))

//====================================================================
//Register: SRAM Start Address (SRAM_START)
/** \brief Allows the SRAM start address to be specified indicating where in the line buffer the SRAM storage space maps. In other words, any address between SRAM Start Address and SRAM End Address will be mapped to the SRAM and any address outside the range will exist in the memory backing store. The register is useful for staggering the Davinci buffers so that all buffers aren't attempting to DMA at the same time.  By default the SRAM range is 0x0 to SRAM_DEPTH-1.*/
//====================================================================

#define  DAVBUFF_SRAM_START_RESERVED1_MASK 0xffff0000
#define  DAVBUFF_SRAM_START_RESERVED1_SHIFT 16
#define  DAVBUFF_SRAM_START_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_SRAM_START_RESERVED1_MASK) >> DAVBUFF_SRAM_START_RESERVED1_SHIFT)
#define  DAVBUFF_SRAM_START_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SRAM_START_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_SRAM_START_RESERVED1_SHIFT))

#define  DAVBUFF_SRAM_START_SRAMSTART_MASK 0xffff
#define  DAVBUFF_SRAM_START_SRAMSTART_SHIFT 0
#define  DAVBUFF_SRAM_START_SRAMSTART_MASK_SHIFT(reg) (((reg) & DAVBUFF_SRAM_START_SRAMSTART_MASK) >> DAVBUFF_SRAM_START_SRAMSTART_SHIFT)
#define  DAVBUFF_SRAM_START_SRAMSTART_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SRAM_START_SRAMSTART_MASK) | (((uint32_t)val) << DAVBUFF_SRAM_START_SRAMSTART_SHIFT))

//====================================================================
//Register: SRAM End Address (SRAM_END)
/** \brief Allows the SRAM end address to be specified indicating where in the line buffer the SRAM storage space maps. In other words, any address between SRAM Start Address and SRAM End Address will be mapped to the SRAM and any address outside the range will exist in the memory backing store. The register is useful for staggering the Davinci buffers so that all buffers aren't attempting to DMA at the same time.  By default the SRAM range is 0x0 to SRAM_DEPTH-1.*/
//====================================================================

#define  DAVBUFF_SRAM_END_RESERVED1_MASK 0xffff0000
#define  DAVBUFF_SRAM_END_RESERVED1_SHIFT 16
#define  DAVBUFF_SRAM_END_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_SRAM_END_RESERVED1_MASK) >> DAVBUFF_SRAM_END_RESERVED1_SHIFT)
#define  DAVBUFF_SRAM_END_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SRAM_END_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_SRAM_END_RESERVED1_SHIFT))

#define  DAVBUFF_SRAM_END_SRAMEND_MASK 0xffff
#define  DAVBUFF_SRAM_END_SRAMEND_SHIFT 0
#define  DAVBUFF_SRAM_END_SRAMEND_MASK_SHIFT(reg) (((reg) & DAVBUFF_SRAM_END_SRAMEND_MASK) >> DAVBUFF_SRAM_END_SRAMEND_SHIFT)
#define  DAVBUFF_SRAM_END_SRAMEND_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_SRAM_END_SRAMEND_MASK) | (((uint32_t)val) << DAVBUFF_SRAM_END_SRAMEND_SHIFT))

//====================================================================
//Register: Interrupt Enable (INT_EN)
/** \brief Used to enable interrupt sources. Writing a '1' will enable the specific interrupt*/
//====================================================================

#define  DAVBUFF_INT_EN_RESERVED1_MASK 0xfffffff0
#define  DAVBUFF_INT_EN_RESERVED1_SHIFT 4
#define  DAVBUFF_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_EN_RESERVED1_MASK) >> DAVBUFF_INT_EN_RESERVED1_SHIFT)
#define  DAVBUFF_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_INT_EN_RESERVED1_SHIFT))

#define  DAVBUFF_INT_EN_DEBUG_MASK 0x8
#define  DAVBUFF_INT_EN_DEBUG_SHIFT 3
#define  DAVBUFF_INT_EN_DEBUG_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_EN_DEBUG_MASK) >> DAVBUFF_INT_EN_DEBUG_SHIFT)
#define  DAVBUFF_INT_EN_DEBUG_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_EN_DEBUG_MASK) | (((uint32_t)val) << DAVBUFF_INT_EN_DEBUG_SHIFT))

#define  DAVBUFF_INT_EN_SAVERESTORE_MASK 0x4
#define  DAVBUFF_INT_EN_SAVERESTORE_SHIFT 2
#define  DAVBUFF_INT_EN_SAVERESTORE_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_EN_SAVERESTORE_MASK) >> DAVBUFF_INT_EN_SAVERESTORE_SHIFT)
#define  DAVBUFF_INT_EN_SAVERESTORE_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_EN_SAVERESTORE_MASK) | (((uint32_t)val) << DAVBUFF_INT_EN_SAVERESTORE_SHIFT))

#define  DAVBUFF_INT_EN_SOFTRESET_MASK 0x2
#define  DAVBUFF_INT_EN_SOFTRESET_SHIFT 1
#define  DAVBUFF_INT_EN_SOFTRESET_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_EN_SOFTRESET_MASK) >> DAVBUFF_INT_EN_SOFTRESET_SHIFT)
#define  DAVBUFF_INT_EN_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_EN_SOFTRESET_MASK) | (((uint32_t)val) << DAVBUFF_INT_EN_SOFTRESET_SHIFT))

#define  DAVBUFF_INT_EN_SEGFAULT_MASK 0x1
#define  DAVBUFF_INT_EN_SEGFAULT_SHIFT 0
#define  DAVBUFF_INT_EN_SEGFAULT_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_EN_SEGFAULT_MASK) >> DAVBUFF_INT_EN_SEGFAULT_SHIFT)
#define  DAVBUFF_INT_EN_SEGFAULT_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_EN_SEGFAULT_MASK) | (((uint32_t)val) << DAVBUFF_INT_EN_SEGFAULT_SHIFT))

//====================================================================
//Register: Interrupt Pending (INT_PEND)
/** \brief Indicates which interrupts are currently pending. A value of '1' indicates that a specific interrupt is pending.*/
//====================================================================

#define  DAVBUFF_INT_PEND_RESERVED1_MASK 0xfffffff0
#define  DAVBUFF_INT_PEND_RESERVED1_SHIFT 4
#define  DAVBUFF_INT_PEND_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_PEND_RESERVED1_MASK) >> DAVBUFF_INT_PEND_RESERVED1_SHIFT)
#define  DAVBUFF_INT_PEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_PEND_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_INT_PEND_RESERVED1_SHIFT))

#define  DAVBUFF_INT_PEND_DEBUG_MASK 0x8
#define  DAVBUFF_INT_PEND_DEBUG_SHIFT 3
#define  DAVBUFF_INT_PEND_DEBUG_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_PEND_DEBUG_MASK) >> DAVBUFF_INT_PEND_DEBUG_SHIFT)
#define  DAVBUFF_INT_PEND_DEBUG_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_PEND_DEBUG_MASK) | (((uint32_t)val) << DAVBUFF_INT_PEND_DEBUG_SHIFT))

#define  DAVBUFF_INT_PEND_SAVERESTORE_MASK 0x4
#define  DAVBUFF_INT_PEND_SAVERESTORE_SHIFT 2
#define  DAVBUFF_INT_PEND_SAVERESTORE_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_PEND_SAVERESTORE_MASK) >> DAVBUFF_INT_PEND_SAVERESTORE_SHIFT)
#define  DAVBUFF_INT_PEND_SAVERESTORE_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_PEND_SAVERESTORE_MASK) | (((uint32_t)val) << DAVBUFF_INT_PEND_SAVERESTORE_SHIFT))

#define  DAVBUFF_INT_PEND_SOFTRESET_MASK 0x2
#define  DAVBUFF_INT_PEND_SOFTRESET_SHIFT 1
#define  DAVBUFF_INT_PEND_SOFTRESET_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_PEND_SOFTRESET_MASK) >> DAVBUFF_INT_PEND_SOFTRESET_SHIFT)
#define  DAVBUFF_INT_PEND_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_PEND_SOFTRESET_MASK) | (((uint32_t)val) << DAVBUFF_INT_PEND_SOFTRESET_SHIFT))

#define  DAVBUFF_INT_PEND_SEGFAULT_MASK 0x1
#define  DAVBUFF_INT_PEND_SEGFAULT_SHIFT 0
#define  DAVBUFF_INT_PEND_SEGFAULT_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_PEND_SEGFAULT_MASK) >> DAVBUFF_INT_PEND_SEGFAULT_SHIFT)
#define  DAVBUFF_INT_PEND_SEGFAULT_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_PEND_SEGFAULT_MASK) | (((uint32_t)val) << DAVBUFF_INT_PEND_SEGFAULT_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge (INT_ACK)
/** \brief Used to acknowledge interrupt sources. Writing a '1' will clear the specific interrupt.*/
//====================================================================

#define  DAVBUFF_INT_ACK_RESERVED1_MASK 0xfffffff0
#define  DAVBUFF_INT_ACK_RESERVED1_SHIFT 4
#define  DAVBUFF_INT_ACK_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_ACK_RESERVED1_MASK) >> DAVBUFF_INT_ACK_RESERVED1_SHIFT)
#define  DAVBUFF_INT_ACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_ACK_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_INT_ACK_RESERVED1_SHIFT))

#define  DAVBUFF_INT_ACK_DEBUG_MASK 0x8
#define  DAVBUFF_INT_ACK_DEBUG_SHIFT 3
#define  DAVBUFF_INT_ACK_DEBUG_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_ACK_DEBUG_MASK) >> DAVBUFF_INT_ACK_DEBUG_SHIFT)
#define  DAVBUFF_INT_ACK_DEBUG_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_ACK_DEBUG_MASK) | (((uint32_t)val) << DAVBUFF_INT_ACK_DEBUG_SHIFT))

#define  DAVBUFF_INT_ACK_SAVERESTORE_MASK 0x4
#define  DAVBUFF_INT_ACK_SAVERESTORE_SHIFT 2
#define  DAVBUFF_INT_ACK_SAVERESTORE_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_ACK_SAVERESTORE_MASK) >> DAVBUFF_INT_ACK_SAVERESTORE_SHIFT)
#define  DAVBUFF_INT_ACK_SAVERESTORE_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_ACK_SAVERESTORE_MASK) | (((uint32_t)val) << DAVBUFF_INT_ACK_SAVERESTORE_SHIFT))

#define  DAVBUFF_INT_ACK_SOFTRESET_MASK 0x2
#define  DAVBUFF_INT_ACK_SOFTRESET_SHIFT 1
#define  DAVBUFF_INT_ACK_SOFTRESET_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_ACK_SOFTRESET_MASK) >> DAVBUFF_INT_ACK_SOFTRESET_SHIFT)
#define  DAVBUFF_INT_ACK_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_ACK_SOFTRESET_MASK) | (((uint32_t)val) << DAVBUFF_INT_ACK_SOFTRESET_SHIFT))

#define  DAVBUFF_INT_ACK_SEGFAULT_MASK 0x1
#define  DAVBUFF_INT_ACK_SEGFAULT_SHIFT 0
#define  DAVBUFF_INT_ACK_SEGFAULT_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_ACK_SEGFAULT_MASK) >> DAVBUFF_INT_ACK_SEGFAULT_SHIFT)
#define  DAVBUFF_INT_ACK_SEGFAULT_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_ACK_SEGFAULT_MASK) | (((uint32_t)val) << DAVBUFF_INT_ACK_SEGFAULT_SHIFT))

//====================================================================
//Register: Interrupt Force (INT_FORCE)
/** \brief Used to force an interrupt. Writing a '1' will set the specific interrupt.*/
//====================================================================

#define  DAVBUFF_INT_FORCE_RESERVED1_MASK 0xfffffff0
#define  DAVBUFF_INT_FORCE_RESERVED1_SHIFT 4
#define  DAVBUFF_INT_FORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_FORCE_RESERVED1_MASK) >> DAVBUFF_INT_FORCE_RESERVED1_SHIFT)
#define  DAVBUFF_INT_FORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_FORCE_RESERVED1_MASK) | (((uint32_t)val) << DAVBUFF_INT_FORCE_RESERVED1_SHIFT))

#define  DAVBUFF_INT_FORCE_DEBUG_MASK 0x8
#define  DAVBUFF_INT_FORCE_DEBUG_SHIFT 3
#define  DAVBUFF_INT_FORCE_DEBUG_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_FORCE_DEBUG_MASK) >> DAVBUFF_INT_FORCE_DEBUG_SHIFT)
#define  DAVBUFF_INT_FORCE_DEBUG_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_FORCE_DEBUG_MASK) | (((uint32_t)val) << DAVBUFF_INT_FORCE_DEBUG_SHIFT))

#define  DAVBUFF_INT_FORCE_SAVERESTORE_MASK 0x4
#define  DAVBUFF_INT_FORCE_SAVERESTORE_SHIFT 2
#define  DAVBUFF_INT_FORCE_SAVERESTORE_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_FORCE_SAVERESTORE_MASK) >> DAVBUFF_INT_FORCE_SAVERESTORE_SHIFT)
#define  DAVBUFF_INT_FORCE_SAVERESTORE_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_FORCE_SAVERESTORE_MASK) | (((uint32_t)val) << DAVBUFF_INT_FORCE_SAVERESTORE_SHIFT))

#define  DAVBUFF_INT_FORCE_SOFTRESET_MASK 0x2
#define  DAVBUFF_INT_FORCE_SOFTRESET_SHIFT 1
#define  DAVBUFF_INT_FORCE_SOFTRESET_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_FORCE_SOFTRESET_MASK) >> DAVBUFF_INT_FORCE_SOFTRESET_SHIFT)
#define  DAVBUFF_INT_FORCE_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_FORCE_SOFTRESET_MASK) | (((uint32_t)val) << DAVBUFF_INT_FORCE_SOFTRESET_SHIFT))

#define  DAVBUFF_INT_FORCE_SEGFAULT_MASK 0x1
#define  DAVBUFF_INT_FORCE_SEGFAULT_SHIFT 0
#define  DAVBUFF_INT_FORCE_SEGFAULT_MASK_SHIFT(reg) (((reg) & DAVBUFF_INT_FORCE_SEGFAULT_MASK) >> DAVBUFF_INT_FORCE_SEGFAULT_SHIFT)
#define  DAVBUFF_INT_FORCE_SEGFAULT_REPLACE_VAL(reg,val) (((reg) & ~DAVBUFF_INT_FORCE_SEGFAULT_MASK) | (((uint32_t)val) << DAVBUFF_INT_FORCE_SEGFAULT_SHIFT))

#endif // DAVBUFF
