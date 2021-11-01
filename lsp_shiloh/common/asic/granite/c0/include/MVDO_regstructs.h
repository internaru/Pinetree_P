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
 * \file MVDO_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _MVDO_REGSTRUCTS_H_
#define _MVDO_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: MVDO (MVDO)
/** \brief Register Descriptions for the MVDO superblock*/
//
//====================================================================

//====================================================================
//
//Register File: HiPS registers (HiPS)
/** \brief Top-level register file for HiPS block*/
//
//====================================================================

typedef struct HIPS_REGS_s
{
  volatile uint32_t Chan_Enable;  ///< 0x0 [R/W]: HiPS Channel Enable
  volatile uint32_t FWBIST_Control;  ///< 0x4 [R/W]: HiPS FWBIST Control
  volatile uint32_t FWBIST_Enable;  ///< 0x8 [R/W]: HiPS FWBIST Enable
  volatile uint32_t FWBIST_Error;  ///< 0xc [R]: HiPS FWBIST Error
  volatile uint32_t FWBIST_Done;  ///< 0x10 [R]: HiPS FWBIST Done
  volatile uint32_t FWBIST_PassnFail;  ///< 0x14 [R]: HiPS FWBIST PassnFail
  volatile uint32_t reserved0[2];
  volatile uint32_t FWBIST_Diagnostic_Table0;  ///< 0x20 [R]: HiPS FWBIST Diagnostic Values Table (Instance 1 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table1;  ///< 0x24 [R]: HiPS FWBIST Diagnostic Values Table (Instance 2 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table2;  ///< 0x28 [R]: HiPS FWBIST Diagnostic Values Table (Instance 3 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table3;  ///< 0x2c [R]: HiPS FWBIST Diagnostic Values Table (Instance 4 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table4;  ///< 0x30 [R]: HiPS FWBIST Diagnostic Values Table (Instance 5 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table5;  ///< 0x34 [R]: HiPS FWBIST Diagnostic Values Table (Instance 6 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table6;  ///< 0x38 [R]: HiPS FWBIST Diagnostic Values Table (Instance 7 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table7;  ///< 0x3c [R]: HiPS FWBIST Diagnostic Values Table (Instance 8 of 8)
} HIPS_REGS_t;

//====================================================================
//
//Register File: HVSync registers (HVSync)
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

//====================================================================
//
//Register File: MVDO Top registers (MVDO_top)
//
//====================================================================

typedef struct MVDO_TOP_REGS_s
{
  volatile uint32_t MVDOInt;  ///< 0x0 [R]: MVDO Interrupt Status Register
  volatile uint32_t MVDOPR;  ///< 0x4 [R]: MVDO Parameter Register
} MVDO_TOP_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 1 of 8) (SVDO0)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO0_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO0_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 2 of 8) (SVDO1)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO1_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO1_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 3 of 8) (SVDO2)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO2_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO2_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 4 of 8) (SVDO3)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO3_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO3_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 5 of 8) (SVDO4)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO4_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO4_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 6 of 8) (SVDO5)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO5_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO5_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 7 of 8) (SVDO6)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO6_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO6_REGS_t;

//====================================================================
//
//Register File: SVDO (Instance 8 of 8) (SVDO7)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO7_REGS_s
{
  volatile uint32_t SCFG;  ///< 0x0 [R/W]: Video Configuration Register
  volatile uint32_t STAT;  ///< 0x4 [R]: Video Status Register
  volatile uint32_t VDWR;  ///< 0x8 [W]: Video DMA Descriptor Write Address Register
  volatile uint32_t VDAR;  ///< 0xc [R]: Video DMA Current Descriptor Address Register
  volatile uint32_t VDBR;  ///< 0x10 [R]: Video DMA Bytes Left to Transfer Register
  volatile uint32_t VDLR;  ///< 0x14 [R]: Video DMA Lines Left to Transfer Register
  volatile uint32_t VDNR;  ///< 0x18 [R]: Video DMA Next Descriptor Address Register
  volatile uint32_t VIER;  ///< 0x1c [R/W]: Video Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x20 [R]: Video Interrupt Pending Register
  volatile uint32_t VIAR;  ///< 0x24 [W]: Video Interrupt Acknowledge Register
  volatile uint32_t VIFR;  ///< 0x28 [W]: Video Interrupt Force Register
  volatile uint32_t VLWR;  ///< 0x2c [R/W]: Video Line Width Register
  volatile uint32_t VTCA;  ///< 0x30 [R]: Video Toner Count Accumulation Register
  volatile uint32_t VTCP;  ///< 0x34 [R]: Video Toner Count Non-Blank Pixels Register
  volatile uint32_t VSAR;  ///< 0x38 [R]: Video DMA Source Address Register
  volatile uint32_t VPKR;  ///< 0x3c [R/W]: Video Pixel Skip Register
  volatile uint32_t JUST0;  ///< 0x40 [R/W]: Justification LUT (Instance 1 of 16)
  volatile uint32_t JUST1;  ///< 0x44 [R/W]: Justification LUT (Instance 2 of 16)
  volatile uint32_t JUST2;  ///< 0x48 [R/W]: Justification LUT (Instance 3 of 16)
  volatile uint32_t JUST3;  ///< 0x4c [R/W]: Justification LUT (Instance 4 of 16)
  volatile uint32_t JUST4;  ///< 0x50 [R/W]: Justification LUT (Instance 5 of 16)
  volatile uint32_t JUST5;  ///< 0x54 [R/W]: Justification LUT (Instance 6 of 16)
  volatile uint32_t JUST6;  ///< 0x58 [R/W]: Justification LUT (Instance 7 of 16)
  volatile uint32_t JUST7;  ///< 0x5c [R/W]: Justification LUT (Instance 8 of 16)
  volatile uint32_t JUST8;  ///< 0x60 [R/W]: Justification LUT (Instance 9 of 16)
  volatile uint32_t JUST9;  ///< 0x64 [R/W]: Justification LUT (Instance 10 of 16)
  volatile uint32_t JUST10;  ///< 0x68 [R/W]: Justification LUT (Instance 11 of 16)
  volatile uint32_t JUST11;  ///< 0x6c [R/W]: Justification LUT (Instance 12 of 16)
  volatile uint32_t JUST12;  ///< 0x70 [R/W]: Justification LUT (Instance 13 of 16)
  volatile uint32_t JUST13;  ///< 0x74 [R/W]: Justification LUT (Instance 14 of 16)
  volatile uint32_t JUST14;  ///< 0x78 [R/W]: Justification LUT (Instance 15 of 16)
  volatile uint32_t JUST15;  ///< 0x7c [R/W]: Justification LUT (Instance 16 of 16)
  volatile uint32_t PCODE0;  ///< 0x80 [R/W]: PCODE LUT (Instance 1 of 16)
  volatile uint32_t PCODE1;  ///< 0x84 [R/W]: PCODE LUT (Instance 2 of 16)
  volatile uint32_t PCODE2;  ///< 0x88 [R/W]: PCODE LUT (Instance 3 of 16)
  volatile uint32_t PCODE3;  ///< 0x8c [R/W]: PCODE LUT (Instance 4 of 16)
  volatile uint32_t PCODE4;  ///< 0x90 [R/W]: PCODE LUT (Instance 5 of 16)
  volatile uint32_t PCODE5;  ///< 0x94 [R/W]: PCODE LUT (Instance 6 of 16)
  volatile uint32_t PCODE6;  ///< 0x98 [R/W]: PCODE LUT (Instance 7 of 16)
  volatile uint32_t PCODE7;  ///< 0x9c [R/W]: PCODE LUT (Instance 8 of 16)
  volatile uint32_t PCODE8;  ///< 0xa0 [R/W]: PCODE LUT (Instance 9 of 16)
  volatile uint32_t PCODE9;  ///< 0xa4 [R/W]: PCODE LUT (Instance 10 of 16)
  volatile uint32_t PCODE10;  ///< 0xa8 [R/W]: PCODE LUT (Instance 11 of 16)
  volatile uint32_t PCODE11;  ///< 0xac [R/W]: PCODE LUT (Instance 12 of 16)
  volatile uint32_t PCODE12;  ///< 0xb0 [R/W]: PCODE LUT (Instance 13 of 16)
  volatile uint32_t PCODE13;  ///< 0xb4 [R/W]: PCODE LUT (Instance 14 of 16)
  volatile uint32_t PCODE14;  ///< 0xb8 [R/W]: PCODE LUT (Instance 15 of 16)
  volatile uint32_t PCODE15;  ///< 0xbc [R/W]: PCODE LUT (Instance 16 of 16)
} SVDO7_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 1 of 8) (HiPWM0)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM0_REGS_s
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
} HIPWM0_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 2 of 8) (HiPWM1)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM1_REGS_s
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
} HIPWM1_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 3 of 8) (HiPWM2)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM2_REGS_s
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
} HIPWM2_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 4 of 8) (HiPWM3)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM3_REGS_s
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
} HIPWM3_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 5 of 8) (HiPWM4)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM4_REGS_s
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
} HIPWM4_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 6 of 8) (HiPWM5)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM5_REGS_s
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
} HIPWM5_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 7 of 8) (HiPWM6)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM6_REGS_s
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
} HIPWM6_REGS_t;

//====================================================================
//
//Register File: HiPWM (Instance 8 of 8) (HiPWM7)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

typedef struct HIPWM7_REGS_s
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
} HIPWM7_REGS_t;

#endif // MVDO
