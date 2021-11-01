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
 * \file UART_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _UART_REGSTRUCTS_H_
#define _UART_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: UART Registers (UART)
/** \brief Register Description for UART MODULE*/
//
//====================================================================

typedef struct UART_REGS_s
{
 union {
  volatile uint32_t RBR;  ///< 0x0 [R]: Receive Buffer Register
  volatile uint32_t THR;  ///< 0x0 [W]: Transmit Holding Register
  volatile uint32_t DLL;  ///< 0x0 [R/W]: Divisor Latch Low
 } union0;
 union {
  volatile uint32_t DLH;  ///< 0x4 [R/W]: Divisor Latch High
  volatile uint32_t IER;  ///< 0x4 [R/W]: Interrupt Enable Register
 } union1;
 union {
  volatile uint32_t IIR;  ///< 0x8 [R]: Interrupt Identity Register
  volatile uint32_t FCR;  ///< 0x8 [W]: FIFO Control Register
 } union2;
  volatile uint32_t LCR;  ///< 0xc [R/W]: Line Control Register
  volatile uint32_t MCR;  ///< 0x10 [R/W]: Modem Control Register
  volatile uint32_t LSR;  ///< 0x14 [R]: Line Status Register
  volatile uint32_t MSR;  ///< 0x18 [R]: Modem Status Register
  volatile uint32_t SCR;  ///< 0x1c [R/W]: Scratchpad Register
  volatile uint32_t LPDLL;  ///< 0x20 [R]: Low Power Divisor Latch Low Register
  volatile uint32_t LPDLH;  ///< 0x24 [R]: Low Power Divisor Latch High Register
  volatile uint32_t reserved0[2];
 union {
  volatile uint32_t SRBRL;  ///< 0x30 [R]: Shadow Receive Buffer Low Register
  volatile uint32_t STHRL;  ///< 0x30 [W]: Shadow Transmit Holding Low Register
 } union3;
  volatile uint32_t reserved1[15];
  volatile uint32_t FAR;  ///< 0x70 [R/W]: FIFO Access Register
  volatile uint32_t TFR;  ///< 0x74 [R]: Transmit FIFO Read Register
  volatile uint32_t RFW;  ///< 0x78 [W]: Receive FIFO Write Register
  volatile uint32_t USR;  ///< 0x7c [R]: UART Status Register
  volatile uint32_t TFL;  ///< 0x80 [R]: Transmit FIFO Level
  volatile uint32_t RFL;  ///< 0x84 [R]: Receive FIFO Level
  volatile uint32_t SRR;  ///< 0x88 [W]: Software Reset Register
  volatile uint32_t SRTS;  ///< 0x8c [R]: Shadow Request to Send
  volatile uint32_t SBCR;  ///< 0x90 [R]: Shadow Break Control Register
  volatile uint32_t SDMAM;  ///< 0x94 [R]: Shadow DMA Mode Register
  volatile uint32_t SFE;  ///< 0x98 [R]: Shadow FIFO Enable Register
  volatile uint32_t SRT;  ///< 0x9c [R]: Shadow RCVR Trigger Register
  volatile uint32_t STET;  ///< 0xa0 [R]: Shadow TX Empty Trigger Register
  volatile uint32_t HTX;  ///< 0xa4 [R/W]: Halt TX Register
  volatile uint32_t DMASA;  ///< 0xa8 [R/W]: DMA Software Acknowledge Register
  volatile uint32_t NBRBR;  ///< 0xac [R]: Nine-Bit Receive Buffer Register
  volatile uint32_t reserved2[17];
  volatile uint32_t CPR;  ///< 0xf4 [R]: Component Parameter Register
  volatile uint32_t UCV;  ///< 0xf8 [R]: UART Component Version Register
  volatile uint32_t CTR;  ///< 0xfc [R]: Component Type Register
} UART_REGS_t;

#endif // UART
