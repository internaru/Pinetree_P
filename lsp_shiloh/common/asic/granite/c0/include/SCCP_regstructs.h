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
 * \file SCCP_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SCCP_REG_REGSTRUCTS_H_
#define _SCCP_REG_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Serial Control Channel Processor (SCCP_reg)
/** \brief Top-level register file for SCCP (Serial Control Channel Processor) block*/
//
//====================================================================

typedef struct SCCP_REG_REGS_s
{
  volatile uint32_t IPTR;  ///< 0x0 [R/W]: Instruction Pointer Register
  volatile uint32_t CINST;  ///< 0x4 [R]: Current Instruction Register
  volatile uint32_t STACK;  ///< 0x8 [R/W]: Stack Pointer
  volatile uint32_t MODE;  ///< 0xc [R/W]: Mode Register
  volatile uint32_t CTRL0;  ///< 0x10 [R/W]: Control 0 Register
  volatile uint32_t CTRL1;  ///< 0x14 [R/W]: Control 1 Register
  volatile uint32_t DATA0;  ///< 0x18 [R/W]: Data 0 Register
  volatile uint32_t DATA1;  ///< 0x1c [R/W]: Data 1 Register
  volatile uint32_t CBPTR0;  ///< 0x20 [R/W]: Control Bit Pointer 0 Register
  volatile uint32_t CBPTR1;  ///< 0x24 [R/W]: Control Bit Pointer 1 Register
  volatile uint32_t DBPTR0;  ///< 0x28 [R/W]: Data Bit Pointer 0 Register
  volatile uint32_t DBPTR1;  ///< 0x2c [R/W]: Data Bit Pointer 1 Register
  volatile uint32_t CPAR0;  ///< 0x30 [R/W]: Control Parity 0 Register
  volatile uint32_t CPAR1;  ///< 0x34 [R/W]: Control Parity 1 Register
  volatile uint32_t DPAR0;  ///< 0x38 [R/W]: Data Parity 0 Register
  volatile uint32_t DPAR1;  ///< 0x3c [R/W]: Data Parity 1 Register
  volatile uint32_t GPIO_OE;  ///< 0x40 [R/W]: GPIO Output Enable Register
  volatile uint32_t GPIO_O;  ///< 0x44 [R/W]: GPIO Output Value Register
  volatile uint32_t GPIO_I;  ///< 0x48 [R]: GPIO Input Value Register
  volatile uint32_t TRIG;  ///< 0x4c [R/W]: External Triggers Register
  volatile uint32_t IADDR;  ///< 0x50 [R/W]: Indirect Address Register
  volatile uint32_t INT;  ///< 0x54 [R/W]: Interrupts Register
  volatile uint32_t GEN0;  ///< 0x58 [R/W]: General Purpose Register 0
  volatile uint32_t GEN1;  ///< 0x5c [R/W]: General Purpose Register 1
  volatile uint32_t GEN2;  ///< 0x60 [R/W]: General Purpose Register 2
  volatile uint32_t GEN3;  ///< 0x64 [R/W]: General Purpose Register 3
  volatile uint32_t GEN4;  ///< 0x68 [R/W]: General Purpose Register 4
  volatile uint32_t GEN5;  ///< 0x6c [R/W]: General Purpose Register 5
  volatile uint32_t GEN6;  ///< 0x70 [R/W]: General Purpose Register 6
  volatile uint32_t GEN7;  ///< 0x74 [R/W]: General Purpose Register 7
  volatile uint32_t CPU_CTRL;  ///< 0x78 [R/W]: CPU Control Register
  volatile uint32_t DEBUG_IADDR;  ///< 0x7c [R]: Debug Instruction Address
  volatile uint32_t TRIG_CFG0;  ///< 0x80 [R/W]: Trigger Configuration 0
  volatile uint32_t TRIG_CFG1;  ///< 0x84 [R/W]: Trigger Configuration 1
  volatile uint32_t GPIO_MUX;  ///< 0x88 [R/W]: GPIO Input Muxing
  volatile uint32_t reserved0[2013];
  volatile uint32_t SRAM[2048];  ///< 0x2000 [R/W]: Microcode SRAM
} SCCP_REG_REGS_t;

#endif // SCCP_REG
