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
 * \file VCF_wrapperstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_WRAPPER_REGSTRUCTS_H_
#define _VCF_WRAPPER_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF Wrapper Registers (VCF_Wrapper)
//
//====================================================================

typedef struct VCF_WRAPPER_REGS_s
{
  volatile uint32_t VALR0;  ///< 0x0 [R]: VCF Accumulator Low Register (Instance 1 of 12)
  volatile uint32_t VAHR0;  ///< 0x4 [R]: VCF Accumulator High Register (Instance 1 of 12)
  volatile uint32_t VALR1;  ///< 0x8 [R]: VCF Accumulator Low Register (Instance 2 of 12)
  volatile uint32_t VAHR1;  ///< 0xc [R]: VCF Accumulator High Register (Instance 2 of 12)
  volatile uint32_t VALR2;  ///< 0x10 [R]: VCF Accumulator Low Register (Instance 3 of 12)
  volatile uint32_t VAHR2;  ///< 0x14 [R]: VCF Accumulator High Register (Instance 3 of 12)
  volatile uint32_t VALR3;  ///< 0x18 [R]: VCF Accumulator Low Register (Instance 4 of 12)
  volatile uint32_t VAHR3;  ///< 0x1c [R]: VCF Accumulator High Register (Instance 4 of 12)
  volatile uint32_t VALR4;  ///< 0x20 [R]: VCF Accumulator Low Register (Instance 5 of 12)
  volatile uint32_t VAHR4;  ///< 0x24 [R]: VCF Accumulator High Register (Instance 5 of 12)
  volatile uint32_t VALR5;  ///< 0x28 [R]: VCF Accumulator Low Register (Instance 6 of 12)
  volatile uint32_t VAHR5;  ///< 0x2c [R]: VCF Accumulator High Register (Instance 6 of 12)
  volatile uint32_t VALR6;  ///< 0x30 [R]: VCF Accumulator Low Register (Instance 7 of 12)
  volatile uint32_t VAHR6;  ///< 0x34 [R]: VCF Accumulator High Register (Instance 7 of 12)
  volatile uint32_t VALR7;  ///< 0x38 [R]: VCF Accumulator Low Register (Instance 8 of 12)
  volatile uint32_t VAHR7;  ///< 0x3c [R]: VCF Accumulator High Register (Instance 8 of 12)
  volatile uint32_t VALR8;  ///< 0x40 [R]: VCF Accumulator Low Register (Instance 9 of 12)
  volatile uint32_t VAHR8;  ///< 0x44 [R]: VCF Accumulator High Register (Instance 9 of 12)
  volatile uint32_t VALR9;  ///< 0x48 [R]: VCF Accumulator Low Register (Instance 10 of 12)
  volatile uint32_t VAHR9;  ///< 0x4c [R]: VCF Accumulator High Register (Instance 10 of 12)
  volatile uint32_t VALR10;  ///< 0x50 [R]: VCF Accumulator Low Register (Instance 11 of 12)
  volatile uint32_t VAHR10;  ///< 0x54 [R]: VCF Accumulator High Register (Instance 11 of 12)
  volatile uint32_t VALR11;  ///< 0x58 [R]: VCF Accumulator Low Register (Instance 12 of 12)
  volatile uint32_t VAHR11;  ///< 0x5c [R]: VCF Accumulator High Register (Instance 12 of 12)
  volatile uint32_t reserved0[8];
  volatile uint32_t VAIR0;  ///< 0x80 [W]: VCF Accumulator Increment Register (Instance 1 of 12)
  volatile uint32_t VAIR1;  ///< 0x84 [W]: VCF Accumulator Increment Register (Instance 2 of 12)
  volatile uint32_t VAIR2;  ///< 0x88 [W]: VCF Accumulator Increment Register (Instance 3 of 12)
  volatile uint32_t VAIR3;  ///< 0x8c [W]: VCF Accumulator Increment Register (Instance 4 of 12)
  volatile uint32_t VAIR4;  ///< 0x90 [W]: VCF Accumulator Increment Register (Instance 5 of 12)
  volatile uint32_t VAIR5;  ///< 0x94 [W]: VCF Accumulator Increment Register (Instance 6 of 12)
  volatile uint32_t VAIR6;  ///< 0x98 [W]: VCF Accumulator Increment Register (Instance 7 of 12)
  volatile uint32_t VAIR7;  ///< 0x9c [W]: VCF Accumulator Increment Register (Instance 8 of 12)
  volatile uint32_t VAIR8;  ///< 0xa0 [W]: VCF Accumulator Increment Register (Instance 9 of 12)
  volatile uint32_t VAIR9;  ///< 0xa4 [W]: VCF Accumulator Increment Register (Instance 10 of 12)
  volatile uint32_t VAIR10;  ///< 0xa8 [W]: VCF Accumulator Increment Register (Instance 11 of 12)
  volatile uint32_t VAIR11;  ///< 0xac [W]: VCF Accumulator Increment Register (Instance 12 of 12)
  volatile uint32_t reserved1[4];
  volatile uint32_t VMHR0;  ///< 0xc0 [R]: VCF MCPU Hash Register (Instance 1 of 6)
  volatile uint32_t VMHR1;  ///< 0xc4 [R]: VCF MCPU Hash Register (Instance 2 of 6)
  volatile uint32_t VMHR2;  ///< 0xc8 [R]: VCF MCPU Hash Register (Instance 3 of 6)
  volatile uint32_t VMHR3;  ///< 0xcc [R]: VCF MCPU Hash Register (Instance 4 of 6)
  volatile uint32_t VMHR4;  ///< 0xd0 [R]: VCF MCPU Hash Register (Instance 5 of 6)
  volatile uint32_t VMHR5;  ///< 0xd4 [R]: VCF MCPU Hash Register (Instance 6 of 6)
  volatile uint32_t reserved2[2];
  volatile uint32_t VIHR00;  ///< 0xe0 [R]: VCF ICPU0 Hash Register (Instance 1 of 6)
  volatile uint32_t VIHR01;  ///< 0xe4 [R]: VCF ICPU0 Hash Register (Instance 2 of 6)
  volatile uint32_t VIHR02;  ///< 0xe8 [R]: VCF ICPU0 Hash Register (Instance 3 of 6)
  volatile uint32_t VIHR03;  ///< 0xec [R]: VCF ICPU0 Hash Register (Instance 4 of 6)
  volatile uint32_t VIHR04;  ///< 0xf0 [R]: VCF ICPU0 Hash Register (Instance 5 of 6)
  volatile uint32_t VIHR05;  ///< 0xf4 [R]: VCF ICPU0 Hash Register (Instance 6 of 6)
  volatile uint32_t reserved3[2];
  volatile uint32_t VIHR10;  ///< 0x100 [R]: VCF ICPU1 Hash Register (Instance 1 of 6)
  volatile uint32_t VIHR11;  ///< 0x104 [R]: VCF ICPU1 Hash Register (Instance 2 of 6)
  volatile uint32_t VIHR12;  ///< 0x108 [R]: VCF ICPU1 Hash Register (Instance 3 of 6)
  volatile uint32_t VIHR13;  ///< 0x10c [R]: VCF ICPU1 Hash Register (Instance 4 of 6)
  volatile uint32_t VIHR14;  ///< 0x110 [R]: VCF ICPU1 Hash Register (Instance 5 of 6)
  volatile uint32_t VIHR15;  ///< 0x114 [R]: VCF ICPU1 Hash Register (Instance 6 of 6)
  volatile uint32_t reserved4[2];
  volatile uint32_t VWSR;  ///< 0x120 [R]: VCF Wrapper Status Register
  volatile uint32_t VMFR;  ///< 0x124 [W]: VCF Minimum Function Register
  volatile uint32_t VLMR;  ///< 0x128 [R/W]: VCF Laser Map Register
  volatile uint32_t VLER;  ///< 0x12c [R]: VCF Laser Enforce Register
  volatile uint32_t VIER0;  ///< 0x130 [R]: VCF Ink Enforce Register 0
  volatile uint32_t VIER1;  ///< 0x134 [R]: VCF Ink Enforce Register 1
} VCF_WRAPPER_REGS_t;

#endif // VCF_WRAPPER
