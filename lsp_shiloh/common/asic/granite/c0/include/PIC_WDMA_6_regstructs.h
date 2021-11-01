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
 * \file PIC_WDMA_6_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIC_WDMA_6_REGSTRUCTS_H_
#define _PIC_WDMA_6_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIC_WDMA (PIC_WDMA_6)
/** \brief Register Descriptions for the multi-channel PIC WDMA block*/
//
//====================================================================

typedef struct PIC_WDMA_6_REGS_s
{
  volatile uint32_t DWOCR;  ///< 0x0 [R/W]: PIC WDMA Configuration Register
  volatile uint32_t DWRTBL;  ///< 0x4 [R/W]: PIC WDMA Router Table Register
  volatile uint32_t DWCIS;  ///< 0x8 [R/W]: PIC Write DMA Common Interrupt Select
} PIC_WDMA_6_REGS_t;

//====================================================================
//
//Register File: DMA channel (Instance 1 of 6) (DMA0)
//
//====================================================================

typedef struct DMA0_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t status;  ///< 0x4 [R]: Status register
  volatile uint32_t line_size;  ///< 0x8 [R/W]: Line Size Register
  volatile uint32_t int_en;  ///< 0xc [R/W]: Interrupt Enable Register
  volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
  volatile uint32_t desc_write;  ///< 0x1c [W]: Descriptor Write Register
  volatile uint32_t desc_read;  ///< 0x20 [R]: Descriptor Read Register
  volatile uint32_t xfer_length;  ///< 0x24 [R]: Descriptor Transfer Length Register
  volatile uint32_t xfer_addr;  ///< 0x28 [R]: Descriptor Transfer Address Register
  volatile uint32_t xfer_burst;  ///< 0x2c [R]: Descriptor Transfer Burst Register
  volatile uint32_t data_val;  ///< 0x30 [R]: Data Valid Register
  volatile uint32_t reset;  ///< 0x34 [W]: Soft Reset register
} DMA0_REGS_t;

//====================================================================
//
//Register File: DMA channel (Instance 2 of 6) (DMA1)
//
//====================================================================

typedef struct DMA1_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t status;  ///< 0x4 [R]: Status register
  volatile uint32_t line_size;  ///< 0x8 [R/W]: Line Size Register
  volatile uint32_t int_en;  ///< 0xc [R/W]: Interrupt Enable Register
  volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
  volatile uint32_t desc_write;  ///< 0x1c [W]: Descriptor Write Register
  volatile uint32_t desc_read;  ///< 0x20 [R]: Descriptor Read Register
  volatile uint32_t xfer_length;  ///< 0x24 [R]: Descriptor Transfer Length Register
  volatile uint32_t xfer_addr;  ///< 0x28 [R]: Descriptor Transfer Address Register
  volatile uint32_t xfer_burst;  ///< 0x2c [R]: Descriptor Transfer Burst Register
  volatile uint32_t data_val;  ///< 0x30 [R]: Data Valid Register
  volatile uint32_t reset;  ///< 0x34 [W]: Soft Reset register
} DMA1_REGS_t;

//====================================================================
//
//Register File: DMA channel (Instance 3 of 6) (DMA2)
//
//====================================================================

typedef struct DMA2_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t status;  ///< 0x4 [R]: Status register
  volatile uint32_t line_size;  ///< 0x8 [R/W]: Line Size Register
  volatile uint32_t int_en;  ///< 0xc [R/W]: Interrupt Enable Register
  volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
  volatile uint32_t desc_write;  ///< 0x1c [W]: Descriptor Write Register
  volatile uint32_t desc_read;  ///< 0x20 [R]: Descriptor Read Register
  volatile uint32_t xfer_length;  ///< 0x24 [R]: Descriptor Transfer Length Register
  volatile uint32_t xfer_addr;  ///< 0x28 [R]: Descriptor Transfer Address Register
  volatile uint32_t xfer_burst;  ///< 0x2c [R]: Descriptor Transfer Burst Register
  volatile uint32_t data_val;  ///< 0x30 [R]: Data Valid Register
  volatile uint32_t reset;  ///< 0x34 [W]: Soft Reset register
} DMA2_REGS_t;

//====================================================================
//
//Register File: DMA channel (Instance 4 of 6) (DMA3)
//
//====================================================================

typedef struct DMA3_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t status;  ///< 0x4 [R]: Status register
  volatile uint32_t line_size;  ///< 0x8 [R/W]: Line Size Register
  volatile uint32_t int_en;  ///< 0xc [R/W]: Interrupt Enable Register
  volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
  volatile uint32_t desc_write;  ///< 0x1c [W]: Descriptor Write Register
  volatile uint32_t desc_read;  ///< 0x20 [R]: Descriptor Read Register
  volatile uint32_t xfer_length;  ///< 0x24 [R]: Descriptor Transfer Length Register
  volatile uint32_t xfer_addr;  ///< 0x28 [R]: Descriptor Transfer Address Register
  volatile uint32_t xfer_burst;  ///< 0x2c [R]: Descriptor Transfer Burst Register
  volatile uint32_t data_val;  ///< 0x30 [R]: Data Valid Register
  volatile uint32_t reset;  ///< 0x34 [W]: Soft Reset register
} DMA3_REGS_t;

//====================================================================
//
//Register File: DMA channel (Instance 5 of 6) (DMA4)
//
//====================================================================

typedef struct DMA4_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t status;  ///< 0x4 [R]: Status register
  volatile uint32_t line_size;  ///< 0x8 [R/W]: Line Size Register
  volatile uint32_t int_en;  ///< 0xc [R/W]: Interrupt Enable Register
  volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
  volatile uint32_t desc_write;  ///< 0x1c [W]: Descriptor Write Register
  volatile uint32_t desc_read;  ///< 0x20 [R]: Descriptor Read Register
  volatile uint32_t xfer_length;  ///< 0x24 [R]: Descriptor Transfer Length Register
  volatile uint32_t xfer_addr;  ///< 0x28 [R]: Descriptor Transfer Address Register
  volatile uint32_t xfer_burst;  ///< 0x2c [R]: Descriptor Transfer Burst Register
  volatile uint32_t data_val;  ///< 0x30 [R]: Data Valid Register
  volatile uint32_t reset;  ///< 0x34 [W]: Soft Reset register
} DMA4_REGS_t;

//====================================================================
//
//Register File: DMA channel (Instance 6 of 6) (DMA5)
//
//====================================================================

typedef struct DMA5_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t status;  ///< 0x4 [R]: Status register
  volatile uint32_t line_size;  ///< 0x8 [R/W]: Line Size Register
  volatile uint32_t int_en;  ///< 0xc [R/W]: Interrupt Enable Register
  volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
  volatile uint32_t desc_write;  ///< 0x1c [W]: Descriptor Write Register
  volatile uint32_t desc_read;  ///< 0x20 [R]: Descriptor Read Register
  volatile uint32_t xfer_length;  ///< 0x24 [R]: Descriptor Transfer Length Register
  volatile uint32_t xfer_addr;  ///< 0x28 [R]: Descriptor Transfer Address Register
  volatile uint32_t xfer_burst;  ///< 0x2c [R]: Descriptor Transfer Burst Register
  volatile uint32_t data_val;  ///< 0x30 [R]: Data Valid Register
  volatile uint32_t reset;  ///< 0x34 [W]: Soft Reset register
} DMA5_REGS_t;

#endif // PIC_WDMA_6
