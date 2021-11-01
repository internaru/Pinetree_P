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
 * \file PIP_STATS_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIP_STATS_REGSTRUCTS_H_
#define _PIP_STATS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIP_STATS (PIP_STATS)
/** \brief Register descriptions for the PIP_STATS block.*/
//
//====================================================================

typedef struct PIP_STATS_REGS_s
{
  volatile uint32_t ACR;  ///< 0x0 [R/W]: PIP_STATS Configuration Register
  volatile uint32_t reserved0;
  volatile uint32_t ASACR;  ///< 0x8 [R/W]: PIP_STATS Stats A Configuration Register
  volatile uint32_t ASBCR;  ///< 0xc [R/W]: PIP_STATS Stats B Configuration Register
  volatile uint32_t ASCCR;  ///< 0x10 [R/W]: PIP_STATS Stats C Configuration Register
  volatile uint32_t reserved1[3];
  volatile uint32_t AMM0R;  ///< 0x20 [R]: PIP_STATS Maximum Minimum 0 Register
  volatile uint32_t AMM1R;  ///< 0x24 [R]: PIP_STATS Maximum Minimum 1 Register
  volatile uint32_t AMM2R;  ///< 0x28 [R]: PIP_STATS Maximum Minimum 2 Register
  volatile uint32_t AMM3R;  ///< 0x2c [R]: PIP_STATS Maximum Minimum 3 Register
  volatile uint32_t AMM4R;  ///< 0x30 [R]: PIP_STATS Maximum Minimum 4 Register
  volatile uint32_t AMM5R;  ///< 0x34 [R]: PIP_STATS Maximum Minimum 5 Register
  volatile uint32_t reserved2[2];
  volatile uint32_t AA0R;  ///< 0x40 [R]: PIP_STATS Accumulator 0 Register
  volatile uint32_t AA1R;  ///< 0x44 [R]: PIP_STATS Accumulator 1 Register
  volatile uint32_t AA2R;  ///< 0x48 [R]: PIP_STATS Accumulator 2 Register
  volatile uint32_t AA3R;  ///< 0x4c [R]: PIP_STATS Accumulator 3 Register
  volatile uint32_t AA4R;  ///< 0x50 [R]: PIP_STATS Accumulator 4 Register
  volatile uint32_t AA5R;  ///< 0x54 [R]: PIP_STATS Accumulator 5 Register
  volatile uint32_t reserved3[2];
  volatile uint32_t ASR;  ///< 0x60 [R]: PIP_STATS Status Register
  volatile uint32_t AD1R;  ///< 0x64 [R]: PIP_STATS Debug 1 Register
  volatile uint32_t AD2R;  ///< 0x68 [R]: PIP_STATS Debug 2 Register
  volatile uint32_t AD3R;  ///< 0x6c [R]: PIP_STATS Debug 3 Register
  volatile uint32_t SRA;  ///< 0x70 [R/W]: Save Restore Address
  volatile uint32_t SRR;  ///< 0x74 [R]: Save Restore Read
  volatile uint32_t SRW;  ///< 0x78 [W]: Save Restore Write
} PIP_STATS_REGS_t;

#endif // PIP_STATS
