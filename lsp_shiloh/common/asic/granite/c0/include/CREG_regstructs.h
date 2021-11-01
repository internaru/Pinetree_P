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
 * \file CREG_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CREG_REG_REGSTRUCTS_H_
#define _CREG_REG_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CREG Register Set (CREG_reg)
/** \brief The following registers are used to configure the Color Registration (CREG) block.*/
//
//====================================================================

typedef struct CREG_REG_REGS_s
{
  volatile uint32_t CRCR;  ///< 0x0 [R/W]: Color Registration Configuration Register
  volatile uint32_t CFCR;  ///< 0x4 [R/W]: CREG Filter Configuration Register
  volatile uint32_t reserved0[2];
  volatile uint32_t CAP0R;  ///< 0x10 [R]: Color Registration Capture 0 Register
  volatile uint32_t reserved1[3];
  volatile uint32_t CAP1R;  ///< 0x20 [R]: Color Registration Capture 1 Register
  volatile uint32_t reserved2[3];
  volatile uint32_t CAP2R;  ///< 0x30 [R]: Color Registration Capture 2 Register
  volatile uint32_t reserved3[3];
  volatile uint32_t CRSR;  ///< 0x40 [R]: Color Registration Status Register
  volatile uint32_t reserved4[3];
  volatile uint32_t CRTSR;  ///< 0x50 [W]: Color Registration Time Synchronization Register
  volatile uint32_t CRFPR;  ///< 0x54 [W]: Color Registration FIFO Pop Register
  volatile uint32_t reserved5[2];
  volatile uint32_t CRIER;  ///< 0x60 [R/W]: Color Registration Interrupt Enable Register
  volatile uint32_t CRIPR;  ///< 0x64 [R]: Color Registration Interrupt Pending Register
  volatile uint32_t CRICR;  ///< 0x68 [W]: Color Registration Interrupt Clear Register
  volatile uint32_t CRIFR;  ///< 0x6c [W]: Color Registration Interrupt Force Register
} CREG_REG_REGS_t;

#endif // CREG_REG
