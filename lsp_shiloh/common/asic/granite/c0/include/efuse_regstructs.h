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
 * \file efuse_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _EFUSE_REGSTRUCTS_H_
#define _EFUSE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: APB EFUSE registers (EFUSE)
/** \brief Registers for EFUSE Programming block which provides hardware assist
    for porogramming and reading the Id_efuse macro.*/
//
//====================================================================

typedef struct EFUSE_REGS_s
{
  volatile uint32_t TOP_Control;  ///< 0x0 [R/W]: Efuse Top-level Control Register
  volatile uint32_t FW_Control;  ///< 0x4 [R/W]: Firmware Control Register
  volatile uint32_t AUTO_Control;  ///< 0x8 [R/W]: Automatic Mode Control Register
  volatile uint32_t reserved0;
  volatile uint32_t ProgData0;  ///< 0x10 [R/W]: Program Data0 Register
  volatile uint32_t ProgData1;  ///< 0x14 [R/W]: Program Data1 Register
  volatile uint32_t ProgData2;  ///< 0x18 [R/W]: Program Data2 Register
  volatile uint32_t reserved1;
  volatile uint32_t TOP_Status;  ///< 0x20 [R]: Efuse Top-level Status Register
  volatile uint32_t reserved2[3];
  volatile uint32_t EfuseStatus31to0_Bank0;  ///< 0x30 [R]: Efuse bits[31:0] Status Register (Instance 1 of 4)
  volatile uint32_t EfuseStatus63to32_Bank0;  ///< 0x34 [R]: Efuse bits[63:32] Status Register (Instance 1 of 4)
  volatile uint32_t EfuseStatus95to64_Bank0;  ///< 0x38 [R]: Efuse bits[95:64] Status Register (Instance 1 of 4)
  volatile uint32_t EfuseStatus96_Bank0;  ///< 0x3c [R]: Efuse bit[96] Status Register (Instance 1 of 4)
  volatile uint32_t EfuseStatus31to0_Bank1;  ///< 0x40 [R]: Efuse bits[31:0] Status Register (Instance 2 of 4)
  volatile uint32_t EfuseStatus63to32_Bank1;  ///< 0x44 [R]: Efuse bits[63:32] Status Register (Instance 2 of 4)
  volatile uint32_t EfuseStatus95to64_Bank1;  ///< 0x48 [R]: Efuse bits[95:64] Status Register (Instance 2 of 4)
  volatile uint32_t EfuseStatus96_Bank1;  ///< 0x4c [R]: Efuse bit[96] Status Register (Instance 2 of 4)
  volatile uint32_t EfuseStatus31to0_Bank2;  ///< 0x50 [R]: Efuse bits[31:0] Status Register (Instance 3 of 4)
  volatile uint32_t EfuseStatus63to32_Bank2;  ///< 0x54 [R]: Efuse bits[63:32] Status Register (Instance 3 of 4)
  volatile uint32_t EfuseStatus95to64_Bank2;  ///< 0x58 [R]: Efuse bits[95:64] Status Register (Instance 3 of 4)
  volatile uint32_t EfuseStatus96_Bank2;  ///< 0x5c [R]: Efuse bit[96] Status Register (Instance 3 of 4)
  volatile uint32_t EfuseStatus31to0_Bank3;  ///< 0x60 [R]: Efuse bits[31:0] Status Register (Instance 4 of 4)
  volatile uint32_t EfuseStatus63to32_Bank3;  ///< 0x64 [R]: Efuse bits[63:32] Status Register (Instance 4 of 4)
  volatile uint32_t EfuseStatus95to64_Bank3;  ///< 0x68 [R]: Efuse bits[95:64] Status Register (Instance 4 of 4)
  volatile uint32_t EfuseStatus96_Bank3;  ///< 0x6c [R]: Efuse bit[96] Status Register (Instance 4 of 4)
} EFUSE_REGS_t;

#endif // EFUSE
