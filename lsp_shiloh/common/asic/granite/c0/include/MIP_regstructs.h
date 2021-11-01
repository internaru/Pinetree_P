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
 * \file MIP_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _MIP_REGSET_REGSTRUCTS_H_
#define _MIP_REGSET_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Mono Image Processing (MIP) (MIP_RegSet)
/** \brief Top-level register file for MIP Block*/
//
//====================================================================

typedef struct MIP_REGSET_REGS_s
{
  volatile uint32_t MCR;  ///< 0x0 [R/W]: MIP Configuration Register
  volatile uint32_t MHMR;  ///< 0x4 [R/W]: MIP Horizontal Move Register
  volatile uint32_t MVMR;  ///< 0x8 [R/W]: MIP Vertical Move Register
  volatile uint32_t MHCR;  ///< 0xc [R/W]: MIP Horizontal Count Register
  volatile uint32_t MVCR;  ///< 0x10 [R/W]: MIP Vertical Count Register
  volatile uint32_t MWSR;  ///< 0x14 [R/W]: MIP Window Stability Register
  volatile uint32_t MMR0;  ///< 0x18 [R/W]: MIP Mux Register (Instance 1 of 19)
  volatile uint32_t MMR1;  ///< 0x1c [R/W]: MIP Mux Register (Instance 2 of 19)
  volatile uint32_t MMR2;  ///< 0x20 [R/W]: MIP Mux Register (Instance 3 of 19)
  volatile uint32_t MMR3;  ///< 0x24 [R/W]: MIP Mux Register (Instance 4 of 19)
  volatile uint32_t MMR4;  ///< 0x28 [R/W]: MIP Mux Register (Instance 5 of 19)
  volatile uint32_t MMR5;  ///< 0x2c [R/W]: MIP Mux Register (Instance 6 of 19)
  volatile uint32_t MMR6;  ///< 0x30 [R/W]: MIP Mux Register (Instance 7 of 19)
  volatile uint32_t MMR7;  ///< 0x34 [R/W]: MIP Mux Register (Instance 8 of 19)
  volatile uint32_t MMR8;  ///< 0x38 [R/W]: MIP Mux Register (Instance 9 of 19)
  volatile uint32_t MMR9;  ///< 0x3c [R/W]: MIP Mux Register (Instance 10 of 19)
  volatile uint32_t MMR10;  ///< 0x40 [R/W]: MIP Mux Register (Instance 11 of 19)
  volatile uint32_t MMR11;  ///< 0x44 [R/W]: MIP Mux Register (Instance 12 of 19)
  volatile uint32_t MMR12;  ///< 0x48 [R/W]: MIP Mux Register (Instance 13 of 19)
  volatile uint32_t MMR13;  ///< 0x4c [R/W]: MIP Mux Register (Instance 14 of 19)
  volatile uint32_t MMR14;  ///< 0x50 [R/W]: MIP Mux Register (Instance 15 of 19)
  volatile uint32_t MMR15;  ///< 0x54 [R/W]: MIP Mux Register (Instance 16 of 19)
  volatile uint32_t MMR16;  ///< 0x58 [R/W]: MIP Mux Register (Instance 17 of 19)
  volatile uint32_t MMR17;  ///< 0x5c [R/W]: MIP Mux Register (Instance 18 of 19)
  volatile uint32_t MMR18;  ///< 0x60 [R/W]: MIP Mux Register (Instance 19 of 19)
  volatile uint32_t MTER;  ///< 0x64 [R]: MIP Toner Estimation Register
  volatile uint32_t MTLR0;  ///< 0x68 [R/W]: MIP Toner LUT Register (Instance 1 of 32)
  volatile uint32_t MTLR1;  ///< 0x6c [R/W]: MIP Toner LUT Register (Instance 2 of 32)
  volatile uint32_t MTLR2;  ///< 0x70 [R/W]: MIP Toner LUT Register (Instance 3 of 32)
  volatile uint32_t MTLR3;  ///< 0x74 [R/W]: MIP Toner LUT Register (Instance 4 of 32)
  volatile uint32_t MTLR4;  ///< 0x78 [R/W]: MIP Toner LUT Register (Instance 5 of 32)
  volatile uint32_t MTLR5;  ///< 0x7c [R/W]: MIP Toner LUT Register (Instance 6 of 32)
  volatile uint32_t MTLR6;  ///< 0x80 [R/W]: MIP Toner LUT Register (Instance 7 of 32)
  volatile uint32_t MTLR7;  ///< 0x84 [R/W]: MIP Toner LUT Register (Instance 8 of 32)
  volatile uint32_t MTLR8;  ///< 0x88 [R/W]: MIP Toner LUT Register (Instance 9 of 32)
  volatile uint32_t MTLR9;  ///< 0x8c [R/W]: MIP Toner LUT Register (Instance 10 of 32)
  volatile uint32_t MTLR10;  ///< 0x90 [R/W]: MIP Toner LUT Register (Instance 11 of 32)
  volatile uint32_t MTLR11;  ///< 0x94 [R/W]: MIP Toner LUT Register (Instance 12 of 32)
  volatile uint32_t MTLR12;  ///< 0x98 [R/W]: MIP Toner LUT Register (Instance 13 of 32)
  volatile uint32_t MTLR13;  ///< 0x9c [R/W]: MIP Toner LUT Register (Instance 14 of 32)
  volatile uint32_t MTLR14;  ///< 0xa0 [R/W]: MIP Toner LUT Register (Instance 15 of 32)
  volatile uint32_t MTLR15;  ///< 0xa4 [R/W]: MIP Toner LUT Register (Instance 16 of 32)
  volatile uint32_t MTLR16;  ///< 0xa8 [R/W]: MIP Toner LUT Register (Instance 17 of 32)
  volatile uint32_t MTLR17;  ///< 0xac [R/W]: MIP Toner LUT Register (Instance 18 of 32)
  volatile uint32_t MTLR18;  ///< 0xb0 [R/W]: MIP Toner LUT Register (Instance 19 of 32)
  volatile uint32_t MTLR19;  ///< 0xb4 [R/W]: MIP Toner LUT Register (Instance 20 of 32)
  volatile uint32_t MTLR20;  ///< 0xb8 [R/W]: MIP Toner LUT Register (Instance 21 of 32)
  volatile uint32_t MTLR21;  ///< 0xbc [R/W]: MIP Toner LUT Register (Instance 22 of 32)
  volatile uint32_t MTLR22;  ///< 0xc0 [R/W]: MIP Toner LUT Register (Instance 23 of 32)
  volatile uint32_t MTLR23;  ///< 0xc4 [R/W]: MIP Toner LUT Register (Instance 24 of 32)
  volatile uint32_t MTLR24;  ///< 0xc8 [R/W]: MIP Toner LUT Register (Instance 25 of 32)
  volatile uint32_t MTLR25;  ///< 0xcc [R/W]: MIP Toner LUT Register (Instance 26 of 32)
  volatile uint32_t MTLR26;  ///< 0xd0 [R/W]: MIP Toner LUT Register (Instance 27 of 32)
  volatile uint32_t MTLR27;  ///< 0xd4 [R/W]: MIP Toner LUT Register (Instance 28 of 32)
  volatile uint32_t MTLR28;  ///< 0xd8 [R/W]: MIP Toner LUT Register (Instance 29 of 32)
  volatile uint32_t MTLR29;  ///< 0xdc [R/W]: MIP Toner LUT Register (Instance 30 of 32)
  volatile uint32_t MTLR30;  ///< 0xe0 [R/W]: MIP Toner LUT Register (Instance 31 of 32)
  volatile uint32_t MTLR31;  ///< 0xe4 [R/W]: MIP Toner LUT Register (Instance 32 of 32)
  volatile uint32_t LB0AR;  ///< 0xe8 [R/W]: MIP LineBuffer0 Address Register
  volatile uint32_t LB0DWR0;  ///< 0xec [W]: MIP LineBuffer0 Data Write Register 0
  volatile uint32_t LB0DWR1;  ///< 0xf0 [W]: MIP LineBuffer0 Data Write Register 1
  volatile uint32_t LB0DWR2;  ///< 0xf4 [W]: MIP LineBuffer0 Data Write Register 2
  volatile uint32_t LB0DRR0;  ///< 0xf8 [R]: MIP LineBuffer0 Data Read Register 0
  volatile uint32_t LB0DRR1;  ///< 0xfc [R]: MIP LineBuffer0 Data Read Register 1
  volatile uint32_t LB0DRR2;  ///< 0x100 [R]: MIP LineBuffer0 Data Read Register 2
  volatile uint32_t TIEAR;  ///< 0x104 [R/W]: TIE LUT Address Register
  volatile uint32_t TIEWR0;  ///< 0x108 [W]: MIP TIE Data Write Register 0
  volatile uint32_t TIEWR1;  ///< 0x10c [W]: MIP TIE Data Write Register 1
  volatile uint32_t reserved0;
  volatile uint32_t TIEDRR0;  ///< 0x114 [R]: MIP TIE Data Read Register 0
  volatile uint32_t TIEDRR1;  ///< 0x118 [R]: MIP TIE Data Read Register 1
  volatile uint32_t reserved1;
  volatile uint32_t MGSR;  ///< 0x120 [R/W]: MIP Galvo Setup Register
  volatile uint32_t SR0;  ///< 0x124 [R/W]: MIP Save-Restore Register0
  volatile uint32_t SR1;  ///< 0x128 [R/W]: MIP Save-Restore Register1
  volatile uint32_t SR2;  ///< 0x12c [R/W]: MIP Save-Restore Register2
  volatile uint32_t SR3;  ///< 0x130 [R/W]: MIP Save-Restore Register3
  volatile uint32_t SR4;  ///< 0x134 [R/W]: MIP Save-Restore Register4
  volatile uint32_t SR5;  ///< 0x138 [R/W]: MIP Save-Restore Register5
  volatile uint32_t SR6;  ///< 0x13c [R/W]: MIP Save-Restore Register6
  volatile uint32_t SR7;  ///< 0x140 [R/W]: MIP Save-Restore Register7
  volatile uint32_t SR8;  ///< 0x144 [R/W]: MIP Save-Restore Register8
  volatile uint32_t SR9;  ///< 0x148 [R/W]: MIP Save-Restore Register9
  volatile uint32_t SR10;  ///< 0x14c [R/W]: MIP Save-Restore Register10
  volatile uint32_t SR11;  ///< 0x150 [R/W]: MIP Save-Restore Register11
  volatile uint32_t SR12;  ///< 0x154 [R/W]: MIP Save-Restore Register12
  volatile uint32_t SR13;  ///< 0x158 [R/W]: MIP Save-Restore Register13
  volatile uint32_t SR14;  ///< 0x15c [R/W]: MIP Save-Restore Register14
  volatile uint32_t SR15;  ///< 0x160 [R/W]: MIP Save-Restore Register15
  volatile uint32_t SR16;  ///< 0x164 [R/W]: MIP Save-Restore Register16
  volatile uint32_t SR17;  ///< 0x168 [R/W]: MIP Save-Restore Register17
  volatile uint32_t SR18;  ///< 0x16c [R/W]: MIP Save-Restore Register18
  volatile uint32_t SR19;  ///< 0x170 [R/W]: MIP Save-Restore Register19
  volatile uint32_t SR20;  ///< 0x174 [R/W]: MIP Save-Restore Register20
  volatile uint32_t SR21;  ///< 0x178 [R/W]: MIP Save-Restore Register21
  volatile uint32_t SR22;  ///< 0x17c [R/W]: MIP Save-Restore Register22
  volatile uint32_t SR23;  ///< 0x180 [R/W]: MIP Save-Restore Register23
  volatile uint32_t SR24;  ///< 0x184 [R/W]: MIP Save-Restore Register24
  volatile uint32_t SR25;  ///< 0x188 [R/W]: MIP Save-Restore Register25
  volatile uint32_t SR26;  ///< 0x18c [R/W]: MIP Save-Restore Register26
  volatile uint32_t SR27;  ///< 0x190 [R/W]: MIP Save-Restore Register27
  volatile uint32_t SR28;  ///< 0x194 [R/W]: MIP Save-Restore Register28
  volatile uint32_t SR29;  ///< 0x198 [R/W]: MIP Save-Restore Register29
  volatile uint32_t SR30;  ///< 0x19c [R/W]: MIP Save-Restore Register30
  volatile uint32_t SR31;  ///< 0x1a0 [R/W]: MIP Save-Restore Register31
  volatile uint32_t SR32;  ///< 0x1a4 [R/W]: MIP Save-Restore Register32
  volatile uint32_t SR33;  ///< 0x1a8 [R/W]: MIP Save-Restore Register33
  volatile uint32_t SR34;  ///< 0x1ac [R/W]: MIP Save-Restore Register34
  volatile uint32_t SR35;  ///< 0x1b0 [R/W]: MIP Save-Restore Register35
  volatile uint32_t SR36;  ///< 0x1b4 [R/W]: MIP Save-Restore Register36
  volatile uint32_t SR37;  ///< 0x1b8 [R/W]: MIP Save-Restore Register37
} MIP_REGSET_REGS_t;

#endif // MIP_REGSET
