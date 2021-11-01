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
 * \file PIP_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIP_REGSTRUCTS_H_
#define _PIP_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIP Top Level (PIP)
/** \brief Top-level register file for Print Image Pipeline*/
//
//====================================================================

typedef struct PIP_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: PIP Configuration Register
  volatile uint32_t DMA_CFG;  ///< 0x4 [R/W]: Output DMA Muxing Configuration Register
  volatile uint32_t IP;  ///< 0x8 [R]: PIP Interrupt Pending Register
  volatile uint32_t STAT;  ///< 0xc [R]: PIP Status Register
  volatile uint32_t DBG;  ///< 0x10 [R/W]: PIP Debug Register
} PIP_REGS_t;

#endif // PIP
