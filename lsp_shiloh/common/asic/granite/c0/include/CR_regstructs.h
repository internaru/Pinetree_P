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
 * \file CR_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CR_REGSTRUCTS_H_
#define _CR_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Color Replace (CR)
/** \brief Top-level register file for Color Replace block*/
//
//====================================================================

typedef struct CR_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: Configuration Register
  volatile uint32_t UPPER0;  ///< 0x4 [R/W]: Upper Threshold (Instance 1 of 2)
  volatile uint32_t UPPER1;  ///< 0x8 [R/W]: Upper Threshold (Instance 2 of 2)
  volatile uint32_t LOWER0;  ///< 0xc [R/W]: Lower Threshold (Instance 1 of 2)
  volatile uint32_t LOWER1;  ///< 0x10 [R/W]: Lower Threshold (Instance 2 of 2)
  volatile uint32_t REP0;  ///< 0x14 [R/W]: Replacement Value (Instance 1 of 2)
  volatile uint32_t REP1;  ///< 0x18 [R/W]: Replacement Value (Instance 2 of 2)
  volatile uint32_t MLUT;  ///< 0x1c [R/W]: Merge LUT
  volatile uint32_t RLUT;  ///< 0x20 [R/W]: Replace LUT
  volatile uint32_t STAT_RD;  ///< 0x24 [R]: Status Read Register
  volatile uint32_t STAT_WR;  ///< 0x28 [W]: Status Write Register
} CR_REGS_t;

#endif // CR
