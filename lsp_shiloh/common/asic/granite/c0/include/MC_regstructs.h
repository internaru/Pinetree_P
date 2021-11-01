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
 * \file MC_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _MC_REGSTRUCTS_H_
#define _MC_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: MC (MC)
/** \brief This defines the registers for the Memory Controller (MC).*/
//
//====================================================================

typedef struct MC_REGS_s
{
  volatile uint32_t ID;  ///< 0x0 [R]: CPU ID and Revision
  volatile uint32_t reserved0[3];
  volatile uint32_t RegDecodeAddr;  ///< 0x10 [R/W]: Configuration Register Decode Address
  volatile uint32_t reserved1[3];
  volatile uint32_t SDRAMConfig0;  ///< 0x20 [R/W]: SDRAM Configuration Register 0
  volatile uint32_t reserved2[3];
  volatile uint32_t SDRAMConfig1;  ///< 0x30 [R/W]: SDRAM Configuration Register 1
  volatile uint32_t reserved3[3];
  volatile uint32_t SDRAMConfig2;  ///< 0x40 [R/W]: SDRAM Configuration Register 2
  volatile uint32_t reserved4[3];
  volatile uint32_t SDRAMTiming1;  ///< 0x50 [R/W]: SDRAM Timing Register I
  volatile uint32_t reserved5[3];
  volatile uint32_t SDRAMTiming2;  ///< 0x60 [R/W]: SDRAM Timing Register II
  volatile uint32_t reserved6[3];
  volatile uint32_t FTDLLConfig;  ///< 0x70 [R/W]: FTDLL Configuration
  volatile uint32_t reserved7[3];
  volatile uint32_t SDRAMControl1;  ///< 0x80 [R/W]: SDRAM Control Register I
  volatile uint32_t reserved8[3];
  volatile uint32_t SDRAMControl2;  ///< 0x90 [R/W]: SDRAM Control Register II
  volatile uint32_t reserved9[3];
  volatile uint32_t PadCal;  ///< 0xa0 [R/W]: SDRAM Pad Calibration
  volatile uint32_t reserved10[3];
  volatile uint32_t PHYControl1;  ///< 0xb0 [R/W]: PHY Control I
  volatile uint32_t reserved11[3];
  volatile uint32_t PHYControl2;  ///< 0xc0 [R/W]: PHY Control II
  volatile uint32_t reserved12[3];
  volatile uint32_t MstrOutOfRange;  ///< 0xd0 [R/W]: Master Out of Range
  volatile uint32_t reserved13[7];
  volatile uint32_t SDRAMControl3;  ///< 0xf0 [R/W]: SDRAM Control Register III
  volatile uint32_t reserved14[3];
  volatile uint32_t MemAddrMap0;  ///< 0x100 [R/W]: Memory Address Map Register 0
  volatile uint32_t reserved15[3];
  volatile uint32_t MemAddrMap1;  ///< 0x110 [R/W]: Memory Address Map Register 1
  volatile uint32_t reserved16[3];
  volatile uint32_t UserCommand;  ///< 0x120 [W]: User Initiated Command Register
  volatile uint32_t reserved17[3];
  volatile uint32_t MemAddrMap2;  ///< 0x130 [R/W]: Memory Address Map Register 2
  volatile uint32_t reserved18[3];
  volatile uint32_t PHYControl3;  ///< 0x140 [R/W]: PHY Control III
  volatile uint32_t reserved19[3];
  volatile uint32_t PHYControl4;  ///< 0x150 [R]: PHY Control IV
  volatile uint32_t reserved20[3];
  volatile uint32_t PHYControl5;  ///< 0x160 [R]: PHY Control V
  volatile uint32_t reserved21[3];
  volatile uint32_t PHYControl6;  ///< 0x170 [R]: PHY Control VI
  volatile uint32_t reserved22[7];
  volatile uint32_t SDRAMTiming3;  ///< 0x190 [R/W]: SDRAM Timing Register III
  volatile uint32_t reserved23[3];
  volatile uint32_t SDRAMControl4;  ///< 0x1a0 [R/W]: SDRAM Control Register IV
  volatile uint32_t reserved24[3];
  volatile uint32_t DRAMStatus;  ///< 0x1b0 [R]: DRAM Status Register
  volatile uint32_t reserved25[3];
  volatile uint32_t SDRAMTiming4;  ///< 0x1c0 [R/W]: SDRAM Timing Register IV
  volatile uint32_t reserved26[3];
  volatile uint32_t PHYControl7;  ///< 0x1d0 [R/W]: PHY Control VII (PHY65_SC)
  volatile uint32_t reserved27[3];
  volatile uint32_t PHYControl8;  ///< 0x1e0 [R/W]: PHY Control VIII (PHY65_SC)
  volatile uint32_t reserved28[3];
  volatile uint32_t PHYControl9;  ///< 0x1f0 [R/W]: PHY Control IX (PHY65_SC)
  volatile uint32_t reserved29[3];
  volatile uint32_t PHYControl10;  ///< 0x200 [R/W]: PHY Control X (PHY65_SC)
  volatile uint32_t reserved30[3];
  volatile uint32_t PHYControl11;  ///< 0x210 [R/W]: PHY Control XI (PHY65_SC)
  volatile uint32_t reserved31[3];
  volatile uint32_t PHYControl12;  ///< 0x220 [R/W]: PHY Control XII (PHY65_SC)
  volatile uint32_t reserved32[3];
  volatile uint32_t PHYControl13;  ///< 0x230 [R/W]: PHY Control XIII (PHY65_SC)
  volatile uint32_t reserved33[3];
  volatile uint32_t PHYControl14;  ///< 0x240 [R/W]: PHY Control XIV (PHY_SC)
  volatile uint32_t reserved34[3];
  volatile uint32_t PHYControl15;  ///< 0x250 [R/W]: PHY Control XV (PHY65_SC)
  volatile uint32_t reserved35[3];
  volatile uint32_t PHYControl16;  ///< 0x260 [R/W]: PHY Control XVI (PHY_SC)
  volatile uint32_t reserved36[7];
  volatile uint32_t SDRAMControl5;  ///< 0x280 [R/W]: SDRAM Control Register V
  volatile uint32_t reserved37[3];
  volatile uint32_t PHYControl19;  ///< 0x290 [R/W]: PHY Control XIX (PHY_SC)
  volatile uint32_t reserved38[3];
  volatile uint32_t PHYControl20;  ///< 0x2a0 [R/W]: PHY Control XX (PHY_SC)
  volatile uint32_t reserved39[3];
  volatile uint32_t PHYControl21;  ///< 0x2b0 [R/W]: PHY Control XXI (PHY_SC)
  volatile uint32_t reserved40[3];
  volatile uint32_t PHYControl22;  ///< 0x2c0 [R/W]: PHY Control XXII (PHY_SC)
  volatile uint32_t reserved41[15];
  volatile uint32_t ShdwRpr0;  ///< 0x300 [R]: Shadow Repair Register 0
  volatile uint32_t reserved42[3];
  volatile uint32_t ShdwRpr1;  ///< 0x310 [R/W]: Shadow Repair Register 1
  volatile uint32_t reserved43[3];
  volatile uint32_t ShdwRpr2;  ///< 0x320 [R/W]: Shadow Repair Register 2
  volatile uint32_t reserved44[103];
  volatile uint32_t TestMode0;  ///< 0x4c0 [W]: Test Mode 0 Register
  volatile uint32_t reserved45[3];
  volatile uint32_t TestMode1;  ///< 0x4d0 [R/W]: Test Mode 1 Register
  volatile uint32_t reserved46[15];
  volatile uint32_t MCBControl1;  ///< 0x510 [R/W]: MCB2DRAM Control Register 1
  volatile uint32_t reserved47[3];
  volatile uint32_t MCBControl2;  ///< 0x520 [R/W]: MCB2DRAM Control Register 2
  volatile uint32_t reserved48[3];
  volatile uint32_t MCBControl3;  ///< 0x530 [R/W]: MCB2DRAM Control Register 3
  volatile uint32_t reserved49[3];
  volatile uint32_t MCBControl4;  ///< 0x540 [R/W]: MCB Read/Write Flow Control
  volatile uint32_t reserved50[67];
  volatile uint32_t SDRAMTiming5;  ///< 0x650 [R/W]: SDRAM Timing Register V
  volatile uint32_t reserved51[3];
  volatile uint32_t SDRAMTiming6;  ///< 0x660 [R/W]: SDRAM Timing Register VI
  volatile uint32_t reserved52[63];
  volatile uint32_t SDRAMControl6;  ///< 0x760 [R/W]: SDRAM Control VI
  volatile uint32_t reserved53[3];
  volatile uint32_t SDRAMControl7;  ///< 0x770 [R/W]: SDRAM Control VII
  volatile uint32_t reserved54[23];
  volatile uint32_t SDRAMControl13;  ///< 0x7d0 [R/W]: SDRAM Control Register 13
  volatile uint32_t reserved55[3];
  volatile uint32_t SDRAMControl14;  ///< 0x7e0 [R/W]: SDRAM Control XIV
  volatile uint32_t reserved56[147];
  volatile uint32_t MemAddrMap3;  ///< 0xa30 [R/W]: Memory Address Map Register 3
  volatile uint32_t reserved57[63];
  volatile uint32_t SDRAMConfig3;  ///< 0xb30 [R/W]: SDRAM Configuration Register 3
  volatile uint32_t reserved58[179];
  volatile uint32_t PHYDLLSelect;  ///< 0xe00 [R/W]: PHY DLL Select (PHY65_SC)
  volatile uint32_t reserved59[3];
  volatile uint32_t PHYDLLControl;  ///< 0xe10 [R/W]: PHY DLL Control (PHY65_SC)
  volatile uint32_t reserved60[3];
  volatile uint32_t PHYDataByteControl;  ///< 0xe20 [R/W]: PHY Data Byte Control (PHY65_SC)
  volatile uint32_t reserved61[7];
  volatile uint32_t PHYWriteLevelSelect;  ///< 0xe40 [R/W]: PHY Write Level Select (PHY_SC)
  volatile uint32_t reserved62[3];
  volatile uint32_t PHYWLControl;  ///< 0xe50 [R/W]: PHY Control Register WL Control (PHY65_SC)
  volatile uint32_t reserved63[3];
  volatile uint32_t PHYWriteLevelClock0;  ///< 0xe60 [R/W]: PHY Write Level Clock 0 (PHY_SC)
  volatile uint32_t reserved64[3];
  volatile uint32_t PHYWriteLevelClock1;  ///< 0xe70 [R/W]: PHY Write Level Clock 1 (PHY_SC)
  volatile uint32_t reserved65[3];
  volatile uint32_t PHYWriteLevelTest;  ///< 0xe80 [R/W]: PHY Write Level Test Mode (PHY_SC)
  volatile uint32_t reserved66[3];
  volatile uint32_t PHYDQSGate;  ///< 0xe90 [R/W]: PHY DQS Gate Control (PHY_SC)
} MC_REGS_t;

#endif // MC
