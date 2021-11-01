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
 * \file SPI_MASTER_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SPI_MASTER_REGSTRUCTS_H_
#define _SPI_MASTER_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SPI registers (SPI_MASTER)
/** \brief Registers for the SPI Master module*/
//
//====================================================================

typedef struct SPI_MASTER_REGS_s
{
  volatile uint32_t CTRLR0;  ///< 0x0 [R/W]: Control Register 0
  volatile uint32_t CTRLR1;  ///< 0x4 [R/W]: Control Register 1
  volatile uint32_t SSIENR;  ///< 0x8 [R/W]: SSI Enable Register
  volatile uint32_t reserved0;
  volatile uint32_t SER;  ///< 0x10 [R/W]: Slave Enable Register
  volatile uint32_t BAUDR;  ///< 0x14 [R/W]: Baud Rate Select
  volatile uint32_t TXFTLR;  ///< 0x18 [R/W]: Transmit FIFO Threshold Level
  volatile uint32_t RXFTLR;  ///< 0x1c [R/W]: Receive FIFO Threshold Level
  volatile uint32_t TXFLR;  ///< 0x20 [R]: Transmit FIFO Level Register
  volatile uint32_t RXFLR;  ///< 0x24 [R]: Receive FIFO Level Register
  volatile uint32_t SR;  ///< 0x28 [R]: Status Register
  volatile uint32_t IMR;  ///< 0x2c [R/W]: Interrupt Mask Register
  volatile uint32_t ISR;  ///< 0x30 [R]: Interrupt Status Register
  volatile uint32_t RISR;  ///< 0x34 [R]: Raw Interrupt Status Register
  volatile uint32_t TXOICR;  ///< 0x38 [R]: Transmit FIFO Overflow Interrupt Clear Register
  volatile uint32_t RXOICR;  ///< 0x3c [R/W]: Receive FIFO Overflow Interrupt Clear Register
  volatile uint32_t RXUICR;  ///< 0x40 [R]: Receive FIFO Underflow Interrupt Clear Register
  volatile uint32_t MSTICR;  ///< 0x44 [R]: Multi-Master Interrupt Clear Register
  volatile uint32_t ICR;  ///< 0x48 [R]: Interrupt Clear Register
  volatile uint32_t DMACR;  ///< 0x4c [R/W]: DMA Control Register
  volatile uint32_t DMATDLR;  ///< 0x50 [R/W]: DMA Transmit Data Level
  volatile uint32_t DMARDLR;  ///< 0x54 [R/W]: DMA Receive Data Level
  volatile uint32_t IDR;  ///< 0x58 [R]: Identification Register
  volatile uint32_t SSI_COMP_VERSION;  ///< 0x5c [R]: coreKit version ID register
  volatile uint32_t DR_LOW;  ///< 0x60 [R/W]: Data Register (Low)
  volatile uint32_t reserved1[14];
  volatile uint32_t DR_HIGH;  ///< 0x9c [R/W]: Data Register (High)
} SPI_MASTER_REGS_t;

#endif // SPI_MASTER
