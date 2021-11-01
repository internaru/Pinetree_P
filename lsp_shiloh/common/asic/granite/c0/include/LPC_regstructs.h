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
 * \file LPC_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _LPC_REGSTRUCTS_H_
#define _LPC_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Linear Precision Control (LPC) (LPC)
/** \brief Top-level register file for LPC block*/
//
//====================================================================

typedef struct LPC_REGS_s
{
  volatile uint32_t LCR;  ///< 0x0 [R/W]: LPC Configuration Register
  volatile uint32_t LICR;  ///< 0x4 [R/W]: LPC Image Configuration Register
  volatile uint32_t FPCR;  ///< 0x8 [R/W]: LPC Fractional Path Configuration Register
  volatile uint32_t FPCR2;  ///< 0xc [R/W]: LPC Fractional Path Configuration Register 2
  volatile uint32_t LWMSR;  ///< 0x10 [R]: LPC Window Manager Status Register
  volatile uint32_t LFPSR;  ///< 0x14 [R]: LPC Fractional Path Status Register
  volatile uint32_t LBAR;  ///< 0x18 [R/W]: LPC LineBuffer Access Control Register
  volatile uint32_t LBAWR;  ///< 0x1c [W]: LPC LineBuffer Address Write Register
  volatile uint32_t LBSAR;  ///< 0x20 [R]: LPC LineBuffer Address Status Register
  volatile uint32_t LBDWR;  ///< 0x24 [W]: LPC LineBuffer Data Write Register
  volatile uint32_t LBDRR;  ///< 0x28 [R]: LPC LineBuffer Data Read Register
  volatile uint32_t LLIR;  ///< 0x2c [R/W]: LPC LFSR Initialization Register
  volatile uint32_t LLCVR;  ///< 0x30 [R]: LPC LFSR Current Value Register
  volatile uint32_t FRIR;  ///< 0x34 [R/W]: LPC FPT R2L Index Register
  volatile uint32_t LSRA;  ///< 0x38 [R/W]: LPC Save Restore Address
  volatile uint32_t LSRR;  ///< 0x3c [R]: LPC Save Restore Read
  volatile uint32_t LSRW;  ///< 0x40 [W]: LPC Save Restore Write
  volatile uint32_t reserved0[239];
  volatile uint32_t FPT[256];  ///< 0x400 [R/W]: Fractional Path Table
  volatile uint32_t MixLUT[256];  ///< 0x800 [R/W]: Two Pixel Mixing LUT
} LPC_REGS_t;

#endif // LPC
