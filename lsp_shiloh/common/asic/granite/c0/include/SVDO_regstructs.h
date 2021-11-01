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
 * \file SVDO_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SVDO_REGSTRUCTS_H_
#define _SVDO_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SVDO (SVDO)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

typedef struct SVDO_REGS_s
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
} SVDO_REGS_t;

#endif // SVDO
