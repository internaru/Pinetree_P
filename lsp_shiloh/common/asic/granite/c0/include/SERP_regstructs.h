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
 * \file SERP_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SERP_REGSTRUCTS_H_
#define _SERP_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SERP (SERP)
/** \brief Register descriptions for the SERP block.*/
//
//====================================================================

typedef struct SERP_REGS_s
{
  volatile uint32_t SCR;  ///< 0x0 [R/W]: SERP Configuration Register
  volatile uint32_t reserved0;
  volatile uint32_t SHRR;  ///< 0x8 [R/W]: SERP Halftone Replace Register
  volatile uint32_t reserved1;
  volatile uint32_t SBTAR;  ///< 0x10 [R/W]: SERP Blank Threshold A Register
  volatile uint32_t SBTBR;  ///< 0x14 [R/W]: SERP Blank Threshold B Register
  volatile uint32_t SBTCR;  ///< 0x18 [R/W]: SERP Blank Threshold C Register
  volatile uint32_t reserved2;
  volatile uint32_t SSR;  ///< 0x20 [R/W]: SERP Scale Register
  volatile uint32_t reserved3[3];
  volatile uint32_t STMCR;  ///< 0x30 [R/W]: SERP Text Merge Register
  volatile uint32_t STMAR;  ///< 0x34 [R/W]: SERP Text Mapping A Register
  volatile uint32_t STMBR;  ///< 0x38 [R/W]: SERP Text Mapping B Register
  volatile uint32_t reserved4;
  volatile uint32_t SBCR;  ///< 0x40 [R/W]: SERP Black Core Register
  volatile uint32_t SECA;  ///< 0x44 [R/W]: SERP Edge Strength Configuration A
  volatile uint32_t SECB;  ///< 0x48 [R/W]: SERP Edge Strength Configuration B
  volatile uint32_t reserved5[5];
  volatile uint32_t STLBC;  ///< 0x60 [R/W]: Text Line Buffer Access Control Register
  volatile uint32_t STLBA;  ///< 0x64 [W]: Text Line Buffer Address Register
  volatile uint32_t STLBAS;  ///< 0x68 [R]: Text Line Buffer Address Status Register
  volatile uint32_t STLBWD;  ///< 0x6c [W]: Text Line Buffer Write Data
  volatile uint32_t STLBRD;  ///< 0x70 [R]: Text Line Buffer Read Data
  volatile uint32_t SSRA;  ///< 0x74 [R/W]: SERP Save Restore Address
  volatile uint32_t SSRR;  ///< 0x78 [R]: SERP Save Restore Read
  volatile uint32_t SSRW;  ///< 0x7c [W]: SERP Save Restore Write
  volatile uint32_t SEI;  ///< 0x80 [R]: SERP Edge Idle
} SERP_REGS_t;

#endif // SERP
