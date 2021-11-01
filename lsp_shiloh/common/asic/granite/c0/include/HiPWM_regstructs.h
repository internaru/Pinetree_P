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
 * \file HiPWM_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HIPWM_REGSTRUCTS_H_
#define _HIPWM_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: HiPWM (HiPWM)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM_REGS_s
{
  volatile uint32_t HCR;  ///< 0x0 [R/W]: HiPWM Configuration Register
  volatile uint32_t HSMR;  ///< 0x4 [R/W]: HiPWM Side Margins Register
  volatile uint32_t HTSR;  ///< 0x8 [R/W]: HiPWM Top Skew Register
  volatile uint32_t HSR;  ///< 0xc [R]: HiPWM Status Register
  volatile uint32_t HSPWR;  ///< 0x10 [R/W]: HiPWM Synth HSYNC Pulse Width Register
  volatile uint32_t HSQR;  ///< 0x14 [R/W]: HiPWM Synth HSYNC Quiescent Register
  volatile uint32_t PRRA;  ///< 0x18 [R]: HiPWM PEC Results Register
  volatile uint32_t HPPVR;  ///< 0x1c [R/W]: HiPWM PIG Edge Pixel Value Register
  volatile uint32_t HPSR;  ///< 0x20 [R/W]: HiPWM PIG Size Register
  volatile uint32_t HPCR;  ///< 0x24 [R/W]: HiPWM PIG Pixel Count Register
  volatile uint32_t HPPSR;  ///< 0x28 [R/W]: HiPWM PIG Pixel Spacing Register
  volatile uint32_t HPLSR;  ///< 0x2c [R/W]: HiPWM PIG Line Spacing Register
  volatile uint32_t HPPOER;  ///< 0x30 [R/W]: HiPWM PIG Pixel Offset Even Register
  volatile uint32_t HPPOOR;  ///< 0x34 [R/W]: HiPWM PIG Pixel Offset Odd Register
  volatile uint32_t HPPLOR;  ///< 0x38 [R/W]: HiPWM PIG Line Offset Register
  volatile uint32_t HCRC;  ///< 0x3c [R]: HiPWM CRC Register
  volatile uint32_t HBCR;  ///< 0x40 [R/W]: HiPWM BENDER Configuration Register
  volatile uint32_t HLBDR;  ///< 0x44 [R/W]: HiPWM Left Side BENDER Default Register
  volatile uint32_t HRBDR;  ///< 0x48 [R/W]: HiPWM Right Side BENDER Default Register
  volatile uint32_t HBNCR;  ///< 0x4c [R/W]: HiPWM BENDER Normalization Configuration Register
  volatile uint32_t HBNINIT;  ///< 0x50 [R/W]: HiPWM BENDER LFSR Initialization Register
  volatile uint32_t HBNCV;  ///< 0x54 [R]: HiPWM BENDER LFSR Current Value Register
  volatile uint32_t HBAT0;  ///< 0x58 [R/W]: HiPWM BENDER Acceleration Table (Instance 1 of 64)
  volatile uint32_t HBAT1;  ///< 0x5c [R/W]: HiPWM BENDER Acceleration Table (Instance 2 of 64)
  volatile uint32_t HBAT2;  ///< 0x60 [R/W]: HiPWM BENDER Acceleration Table (Instance 3 of 64)
  volatile uint32_t HBAT3;  ///< 0x64 [R/W]: HiPWM BENDER Acceleration Table (Instance 4 of 64)
  volatile uint32_t HBAT4;  ///< 0x68 [R/W]: HiPWM BENDER Acceleration Table (Instance 5 of 64)
  volatile uint32_t HBAT5;  ///< 0x6c [R/W]: HiPWM BENDER Acceleration Table (Instance 6 of 64)
  volatile uint32_t HBAT6;  ///< 0x70 [R/W]: HiPWM BENDER Acceleration Table (Instance 7 of 64)
  volatile uint32_t HBAT7;  ///< 0x74 [R/W]: HiPWM BENDER Acceleration Table (Instance 8 of 64)
  volatile uint32_t HBAT8;  ///< 0x78 [R/W]: HiPWM BENDER Acceleration Table (Instance 9 of 64)
  volatile uint32_t HBAT9;  ///< 0x7c [R/W]: HiPWM BENDER Acceleration Table (Instance 10 of 64)
  volatile uint32_t HBAT10;  ///< 0x80 [R/W]: HiPWM BENDER Acceleration Table (Instance 11 of 64)
  volatile uint32_t HBAT11;  ///< 0x84 [R/W]: HiPWM BENDER Acceleration Table (Instance 12 of 64)
  volatile uint32_t HBAT12;  ///< 0x88 [R/W]: HiPWM BENDER Acceleration Table (Instance 13 of 64)
  volatile uint32_t HBAT13;  ///< 0x8c [R/W]: HiPWM BENDER Acceleration Table (Instance 14 of 64)
  volatile uint32_t HBAT14;  ///< 0x90 [R/W]: HiPWM BENDER Acceleration Table (Instance 15 of 64)
  volatile uint32_t HBAT15;  ///< 0x94 [R/W]: HiPWM BENDER Acceleration Table (Instance 16 of 64)
  volatile uint32_t HBAT16;  ///< 0x98 [R/W]: HiPWM BENDER Acceleration Table (Instance 17 of 64)
  volatile uint32_t HBAT17;  ///< 0x9c [R/W]: HiPWM BENDER Acceleration Table (Instance 18 of 64)
  volatile uint32_t HBAT18;  ///< 0xa0 [R/W]: HiPWM BENDER Acceleration Table (Instance 19 of 64)
  volatile uint32_t HBAT19;  ///< 0xa4 [R/W]: HiPWM BENDER Acceleration Table (Instance 20 of 64)
  volatile uint32_t HBAT20;  ///< 0xa8 [R/W]: HiPWM BENDER Acceleration Table (Instance 21 of 64)
  volatile uint32_t HBAT21;  ///< 0xac [R/W]: HiPWM BENDER Acceleration Table (Instance 22 of 64)
  volatile uint32_t HBAT22;  ///< 0xb0 [R/W]: HiPWM BENDER Acceleration Table (Instance 23 of 64)
  volatile uint32_t HBAT23;  ///< 0xb4 [R/W]: HiPWM BENDER Acceleration Table (Instance 24 of 64)
  volatile uint32_t HBAT24;  ///< 0xb8 [R/W]: HiPWM BENDER Acceleration Table (Instance 25 of 64)
  volatile uint32_t HBAT25;  ///< 0xbc [R/W]: HiPWM BENDER Acceleration Table (Instance 26 of 64)
  volatile uint32_t HBAT26;  ///< 0xc0 [R/W]: HiPWM BENDER Acceleration Table (Instance 27 of 64)
  volatile uint32_t HBAT27;  ///< 0xc4 [R/W]: HiPWM BENDER Acceleration Table (Instance 28 of 64)
  volatile uint32_t HBAT28;  ///< 0xc8 [R/W]: HiPWM BENDER Acceleration Table (Instance 29 of 64)
  volatile uint32_t HBAT29;  ///< 0xcc [R/W]: HiPWM BENDER Acceleration Table (Instance 30 of 64)
  volatile uint32_t HBAT30;  ///< 0xd0 [R/W]: HiPWM BENDER Acceleration Table (Instance 31 of 64)
  volatile uint32_t HBAT31;  ///< 0xd4 [R/W]: HiPWM BENDER Acceleration Table (Instance 32 of 64)
  volatile uint32_t HBAT32;  ///< 0xd8 [R/W]: HiPWM BENDER Acceleration Table (Instance 33 of 64)
  volatile uint32_t HBAT33;  ///< 0xdc [R/W]: HiPWM BENDER Acceleration Table (Instance 34 of 64)
  volatile uint32_t HBAT34;  ///< 0xe0 [R/W]: HiPWM BENDER Acceleration Table (Instance 35 of 64)
  volatile uint32_t HBAT35;  ///< 0xe4 [R/W]: HiPWM BENDER Acceleration Table (Instance 36 of 64)
  volatile uint32_t HBAT36;  ///< 0xe8 [R/W]: HiPWM BENDER Acceleration Table (Instance 37 of 64)
  volatile uint32_t HBAT37;  ///< 0xec [R/W]: HiPWM BENDER Acceleration Table (Instance 38 of 64)
  volatile uint32_t HBAT38;  ///< 0xf0 [R/W]: HiPWM BENDER Acceleration Table (Instance 39 of 64)
  volatile uint32_t HBAT39;  ///< 0xf4 [R/W]: HiPWM BENDER Acceleration Table (Instance 40 of 64)
  volatile uint32_t HBAT40;  ///< 0xf8 [R/W]: HiPWM BENDER Acceleration Table (Instance 41 of 64)
  volatile uint32_t HBAT41;  ///< 0xfc [R/W]: HiPWM BENDER Acceleration Table (Instance 42 of 64)
  volatile uint32_t HBAT42;  ///< 0x100 [R/W]: HiPWM BENDER Acceleration Table (Instance 43 of 64)
  volatile uint32_t HBAT43;  ///< 0x104 [R/W]: HiPWM BENDER Acceleration Table (Instance 44 of 64)
  volatile uint32_t HBAT44;  ///< 0x108 [R/W]: HiPWM BENDER Acceleration Table (Instance 45 of 64)
  volatile uint32_t HBAT45;  ///< 0x10c [R/W]: HiPWM BENDER Acceleration Table (Instance 46 of 64)
  volatile uint32_t HBAT46;  ///< 0x110 [R/W]: HiPWM BENDER Acceleration Table (Instance 47 of 64)
  volatile uint32_t HBAT47;  ///< 0x114 [R/W]: HiPWM BENDER Acceleration Table (Instance 48 of 64)
  volatile uint32_t HBAT48;  ///< 0x118 [R/W]: HiPWM BENDER Acceleration Table (Instance 49 of 64)
  volatile uint32_t HBAT49;  ///< 0x11c [R/W]: HiPWM BENDER Acceleration Table (Instance 50 of 64)
  volatile uint32_t HBAT50;  ///< 0x120 [R/W]: HiPWM BENDER Acceleration Table (Instance 51 of 64)
  volatile uint32_t HBAT51;  ///< 0x124 [R/W]: HiPWM BENDER Acceleration Table (Instance 52 of 64)
  volatile uint32_t HBAT52;  ///< 0x128 [R/W]: HiPWM BENDER Acceleration Table (Instance 53 of 64)
  volatile uint32_t HBAT53;  ///< 0x12c [R/W]: HiPWM BENDER Acceleration Table (Instance 54 of 64)
  volatile uint32_t HBAT54;  ///< 0x130 [R/W]: HiPWM BENDER Acceleration Table (Instance 55 of 64)
  volatile uint32_t HBAT55;  ///< 0x134 [R/W]: HiPWM BENDER Acceleration Table (Instance 56 of 64)
  volatile uint32_t HBAT56;  ///< 0x138 [R/W]: HiPWM BENDER Acceleration Table (Instance 57 of 64)
  volatile uint32_t HBAT57;  ///< 0x13c [R/W]: HiPWM BENDER Acceleration Table (Instance 58 of 64)
  volatile uint32_t HBAT58;  ///< 0x140 [R/W]: HiPWM BENDER Acceleration Table (Instance 59 of 64)
  volatile uint32_t HBAT59;  ///< 0x144 [R/W]: HiPWM BENDER Acceleration Table (Instance 60 of 64)
  volatile uint32_t HBAT60;  ///< 0x148 [R/W]: HiPWM BENDER Acceleration Table (Instance 61 of 64)
  volatile uint32_t HBAT61;  ///< 0x14c [R/W]: HiPWM BENDER Acceleration Table (Instance 62 of 64)
  volatile uint32_t HBAT62;  ///< 0x150 [R/W]: HiPWM BENDER Acceleration Table (Instance 63 of 64)
  volatile uint32_t HBAT63;  ///< 0x154 [R/W]: HiPWM BENDER Acceleration Table (Instance 64 of 64)
  volatile uint32_t HMIC0;  ///< 0x158 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 1 of 12)
  volatile uint32_t HMIC1;  ///< 0x15c [R/W]: HiPWM Mirror Correction Margin Registers (Instance 2 of 12)
  volatile uint32_t HMIC2;  ///< 0x160 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 3 of 12)
  volatile uint32_t HMIC3;  ///< 0x164 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 4 of 12)
  volatile uint32_t HMIC4;  ///< 0x168 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 5 of 12)
  volatile uint32_t HMIC5;  ///< 0x16c [R/W]: HiPWM Mirror Correction Margin Registers (Instance 6 of 12)
  volatile uint32_t HMIC6;  ///< 0x170 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 7 of 12)
  volatile uint32_t HMIC7;  ///< 0x174 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 8 of 12)
  volatile uint32_t HMIC8;  ///< 0x178 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 9 of 12)
  volatile uint32_t HMIC9;  ///< 0x17c [R/W]: HiPWM Mirror Correction Margin Registers (Instance 10 of 12)
  volatile uint32_t HMIC10;  ///< 0x180 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 11 of 12)
  volatile uint32_t HMIC11;  ///< 0x184 [R/W]: HiPWM Mirror Correction Margin Registers (Instance 12 of 12)
} HIPWM_REGS_t;

#endif // HIPWM
