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
 * \file SILABS_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SILABS_REGSTRUCTS_H_
#define _SILABS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SILABS (SILABS)
/** \brief Top-level register file for APB SILABS block*/
//
//====================================================================

typedef struct SILABS_REGS_s
{
  volatile uint32_t CTL;  ///< 0x0 [R/W]: Control Register
  volatile uint32_t SECTX;  ///< 0x4 [W]: Secondary Communications Register for DAA Channel
  volatile uint32_t STS;  ///< 0x8 [R]: Status Register for DAA Channel
  volatile uint32_t RXD;  ///< 0xc [R]: Receive Register for DAA Channel
  volatile uint32_t TXD;  ///< 0x10 [W]: Transmit Register for DAA Channel
  volatile uint32_t reserved0[3];
  volatile uint32_t SECTX2;  ///< 0x20 [W]: Secondary Communications Register for CODEC Channel
  volatile uint32_t STS2;  ///< 0x24 [R]: Status Register for CODEC Channel
  volatile uint32_t RXD2;  ///< 0x28 [R]: Receive Register for CODEC Channel
  volatile uint32_t TXD2;  ///< 0x2c [W]: Transmit Register for CODEC Channel
} SILABS_REGS_t;

#endif // SILABS
