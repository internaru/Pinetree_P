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
 * \file HVSync_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HVSYNC_REGSTRUCTS_H_
#define _HVSYNC_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: HVSync (HVSync)
/** \brief Top-level register file for HVSync block*/
//
//====================================================================

typedef struct HVSYNC_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: HVSync Configuration Register
  volatile uint32_t MIR_CFG0;  ///< 0x4 [R/W]: Mirror Configuration Register (Instance 1 of 2)
  volatile uint32_t MIR_CFG1;  ///< 0x8 [R/W]: Mirror Configuration Register (Instance 2 of 2)
  volatile uint32_t MIR_STA0;  ///< 0xc [R]: Mirror Status Register (Instance 1 of 2)
  volatile uint32_t MIR_STA1;  ///< 0x10 [R]: Mirror Status Register (Instance 2 of 2)
  volatile uint32_t FT_RST0;  ///< 0x14 [W]: Facet Tracker Reset Register (Instance 1 of 2)
  volatile uint32_t FT_RST1;  ///< 0x18 [W]: Facet Tracker Reset Register (Instance 2 of 2)
  volatile uint32_t OUT_SEL0;  ///< 0x1c [R/W]: Output Select Register (Instance 1 of 8)
  volatile uint32_t OUT_SEL1;  ///< 0x20 [R/W]: Output Select Register (Instance 2 of 8)
  volatile uint32_t OUT_SEL2;  ///< 0x24 [R/W]: Output Select Register (Instance 3 of 8)
  volatile uint32_t OUT_SEL3;  ///< 0x28 [R/W]: Output Select Register (Instance 4 of 8)
  volatile uint32_t OUT_SEL4;  ///< 0x2c [R/W]: Output Select Register (Instance 5 of 8)
  volatile uint32_t OUT_SEL5;  ///< 0x30 [R/W]: Output Select Register (Instance 6 of 8)
  volatile uint32_t OUT_SEL6;  ///< 0x34 [R/W]: Output Select Register (Instance 7 of 8)
  volatile uint32_t OUT_SEL7;  ///< 0x38 [R/W]: Output Select Register (Instance 8 of 8)
  volatile uint32_t TQSTAT;  ///< 0x3c [R]: Top Queue Status Register
  volatile uint32_t TQSEL;  ///< 0x40 [R/W]: Top Queue HSYNC select Register
  volatile uint32_t HVSIER;  ///< 0x44 [R/W]: HVSync Interrupt Enable Register
  volatile uint32_t HVSIPR;  ///< 0x48 [R]: HVSync Interrupt Pending Register
  volatile uint32_t HVSIAR;  ///< 0x4c [W]: HVSync Interrupt Acknowledge Register
  volatile uint32_t HVSIFR;  ///< 0x50 [W]: HVSync Interrupt Force Register
  volatile uint32_t HVSTPMR0;  ///< 0x54 [R/W]: Top Margin Register (Instance 1 of 4)
  volatile uint32_t HVSTPMR1;  ///< 0x58 [R/W]: Top Margin Register (Instance 2 of 4)
  volatile uint32_t HVSTPMR2;  ///< 0x5c [R/W]: Top Margin Register (Instance 3 of 4)
  volatile uint32_t HVSTPMR3;  ///< 0x60 [R/W]: Top Margin Register (Instance 4 of 4)
  volatile uint32_t HFCR0;  ///< 0x64 [R/W]: HSYNC Filter Configuration Register (Instance 1 of 4)
  volatile uint32_t HFCR1;  ///< 0x68 [R/W]: HSYNC Filter Configuration Register (Instance 2 of 4)
  volatile uint32_t HFCR2;  ///< 0x6c [R/W]: HSYNC Filter Configuration Register (Instance 3 of 4)
  volatile uint32_t HFCR3;  ///< 0x70 [R/W]: HSYNC Filter Configuration Register (Instance 4 of 4)
  volatile uint32_t HFFCR0;  ///< 0x74 [R/W]: HSYNC FIR Configuration Register (Instance 1 of 4)
  volatile uint32_t HFFCR1;  ///< 0x78 [R/W]: HSYNC FIR Configuration Register (Instance 2 of 4)
  volatile uint32_t HFFCR2;  ///< 0x7c [R/W]: HSYNC FIR Configuration Register (Instance 3 of 4)
  volatile uint32_t HFFCR3;  ///< 0x80 [R/W]: HSYNC FIR Configuration Register (Instance 4 of 4)
  volatile uint32_t HLOR0;  ///< 0x84 [R/W]: HSYNC Lockout Register (Instance 1 of 4)
  volatile uint32_t HLOR1;  ///< 0x88 [R/W]: HSYNC Lockout Register (Instance 2 of 4)
  volatile uint32_t HLOR2;  ///< 0x8c [R/W]: HSYNC Lockout Register (Instance 3 of 4)
  volatile uint32_t HLOR3;  ///< 0x90 [R/W]: HSYNC Lockout Register (Instance 4 of 4)
  volatile uint32_t HCCR0;  ///< 0x94 [R/W]: HSYNC Capture Configuration Register (Instance 1 of 4)
  volatile uint32_t HCCR1;  ///< 0x98 [R/W]: HSYNC Capture Configuration Register (Instance 2 of 4)
  volatile uint32_t HCCR2;  ///< 0x9c [R/W]: HSYNC Capture Configuration Register (Instance 3 of 4)
  volatile uint32_t HCCR3;  ///< 0xa0 [R/W]: HSYNC Capture Configuration Register (Instance 4 of 4)
  volatile uint32_t HECR0;  ///< 0xa4 [R]: HSYNC Edge Capture Register (Instance 1 of 4)
  volatile uint32_t HECR1;  ///< 0xa8 [R]: HSYNC Edge Capture Register (Instance 2 of 4)
  volatile uint32_t HECR2;  ///< 0xac [R]: HSYNC Edge Capture Register (Instance 3 of 4)
  volatile uint32_t HECR3;  ///< 0xb0 [R]: HSYNC Edge Capture Register (Instance 4 of 4)
  volatile uint32_t HOCR0;  ///< 0xb4 [R]: HSYNC Offset Capture Register (Instance 1 of 4)
  volatile uint32_t HOCR1;  ///< 0xb8 [R]: HSYNC Offset Capture Register (Instance 2 of 4)
  volatile uint32_t HOCR2;  ///< 0xbc [R]: HSYNC Offset Capture Register (Instance 3 of 4)
  volatile uint32_t HOCR3;  ///< 0xc0 [R]: HSYNC Offset Capture Register (Instance 4 of 4)
  volatile uint32_t HFSR0;  ///< 0xc4 [R]: HSYNC Filter Status Register (Instance 1 of 4)
  volatile uint32_t HFSR1;  ///< 0xc8 [R]: HSYNC Filter Status Register (Instance 2 of 4)
  volatile uint32_t HFSR2;  ///< 0xcc [R]: HSYNC Filter Status Register (Instance 3 of 4)
  volatile uint32_t HFSR3;  ///< 0xd0 [R]: HSYNC Filter Status Register (Instance 4 of 4)
  volatile uint32_t HSET0;  ///< 0xd4 [R/W]: HSYNC SOS To EOS Time (Instance 1 of 4)
  volatile uint32_t HSET1;  ///< 0xd8 [R/W]: HSYNC SOS To EOS Time (Instance 2 of 4)
  volatile uint32_t HSET2;  ///< 0xdc [R/W]: HSYNC SOS To EOS Time (Instance 3 of 4)
  volatile uint32_t HSET3;  ///< 0xe0 [R/W]: HSYNC SOS To EOS Time (Instance 4 of 4)
  volatile uint32_t MASK_CFG0;  ///< 0xe4 [R/W]: HSYNC Masking Configuration Register (Instance 1 of 8)
  volatile uint32_t MASK_CFG1;  ///< 0xe8 [R/W]: HSYNC Masking Configuration Register (Instance 2 of 8)
  volatile uint32_t MASK_CFG2;  ///< 0xec [R/W]: HSYNC Masking Configuration Register (Instance 3 of 8)
  volatile uint32_t MASK_CFG3;  ///< 0xf0 [R/W]: HSYNC Masking Configuration Register (Instance 4 of 8)
  volatile uint32_t MASK_CFG4;  ///< 0xf4 [R/W]: HSYNC Masking Configuration Register (Instance 5 of 8)
  volatile uint32_t MASK_CFG5;  ///< 0xf8 [R/W]: HSYNC Masking Configuration Register (Instance 6 of 8)
  volatile uint32_t MASK_CFG6;  ///< 0xfc [R/W]: HSYNC Masking Configuration Register (Instance 7 of 8)
  volatile uint32_t MASK_CFG7;  ///< 0x100 [R/W]: HSYNC Masking Configuration Register (Instance 8 of 8)
  volatile uint32_t TOP_TM0;  ///< 0x104 [R]: Top Time Register (Instance 1 of 2)
  volatile uint32_t TOP_TM1;  ///< 0x108 [R]: Top Time Register (Instance 2 of 2)
  volatile uint32_t HVSTAT;  ///< 0x10c [R]: HVSync Status Register
  volatile uint32_t TM0_CNT0;  ///< 0x110 [R]: Facet Time Register (Instance 1 of 12)
  volatile uint32_t TM0_CNT1;  ///< 0x114 [R]: Facet Time Register (Instance 2 of 12)
  volatile uint32_t TM0_CNT2;  ///< 0x118 [R]: Facet Time Register (Instance 3 of 12)
  volatile uint32_t TM0_CNT3;  ///< 0x11c [R]: Facet Time Register (Instance 4 of 12)
  volatile uint32_t TM0_CNT4;  ///< 0x120 [R]: Facet Time Register (Instance 5 of 12)
  volatile uint32_t TM0_CNT5;  ///< 0x124 [R]: Facet Time Register (Instance 6 of 12)
  volatile uint32_t TM0_CNT6;  ///< 0x128 [R]: Facet Time Register (Instance 7 of 12)
  volatile uint32_t TM0_CNT7;  ///< 0x12c [R]: Facet Time Register (Instance 8 of 12)
  volatile uint32_t TM0_CNT8;  ///< 0x130 [R]: Facet Time Register (Instance 9 of 12)
  volatile uint32_t TM0_CNT9;  ///< 0x134 [R]: Facet Time Register (Instance 10 of 12)
  volatile uint32_t TM0_CNT10;  ///< 0x138 [R]: Facet Time Register (Instance 11 of 12)
  volatile uint32_t TM0_CNT11;  ///< 0x13c [R]: Facet Time Register (Instance 12 of 12)
  volatile uint32_t TM1_CNT0;  ///< 0x140 [R]: Facet Time Register (Instance 1 of 12)
  volatile uint32_t TM1_CNT1;  ///< 0x144 [R]: Facet Time Register (Instance 2 of 12)
  volatile uint32_t TM1_CNT2;  ///< 0x148 [R]: Facet Time Register (Instance 3 of 12)
  volatile uint32_t TM1_CNT3;  ///< 0x14c [R]: Facet Time Register (Instance 4 of 12)
  volatile uint32_t TM1_CNT4;  ///< 0x150 [R]: Facet Time Register (Instance 5 of 12)
  volatile uint32_t TM1_CNT5;  ///< 0x154 [R]: Facet Time Register (Instance 6 of 12)
  volatile uint32_t TM1_CNT6;  ///< 0x158 [R]: Facet Time Register (Instance 7 of 12)
  volatile uint32_t TM1_CNT7;  ///< 0x15c [R]: Facet Time Register (Instance 8 of 12)
  volatile uint32_t TM1_CNT8;  ///< 0x160 [R]: Facet Time Register (Instance 9 of 12)
  volatile uint32_t TM1_CNT9;  ///< 0x164 [R]: Facet Time Register (Instance 10 of 12)
  volatile uint32_t TM1_CNT10;  ///< 0x168 [R]: Facet Time Register (Instance 11 of 12)
  volatile uint32_t TM1_CNT11;  ///< 0x16c [R]: Facet Time Register (Instance 12 of 12)
  volatile uint32_t HDLR0;  ///< 0x170 [R/W]: HSYNC Delay Register (Instance 1 of 2)
  volatile uint32_t HDLR1;  ///< 0x174 [R/W]: HSYNC Delay Register (Instance 2 of 2)
  volatile uint32_t reserved0[2];
  volatile uint32_t ETDATA;  ///< 0x180 [R]: HVSync Edge Timer Data Register
  volatile uint32_t ETCFG;  ///< 0x184 [R/W]: HVSync Edge Timer Configuration Register
  volatile uint32_t ETSTAT;  ///< 0x188 [R]: HVSync Edge Timer FIFO Status Register
  volatile uint32_t HWCR0;  ///< 0x18c [R/W]: HSYNC Watchdog Configuration Register (Instance 1 of 4)
  volatile uint32_t HWCR1;  ///< 0x190 [R/W]: HSYNC Watchdog Configuration Register (Instance 2 of 4)
  volatile uint32_t HWCR2;  ///< 0x194 [R/W]: HSYNC Watchdog Configuration Register (Instance 3 of 4)
  volatile uint32_t HWCR3;  ///< 0x198 [R/W]: HSYNC Watchdog Configuration Register (Instance 4 of 4)
} HVSYNC_REGS_t;

#endif // HVSYNC
