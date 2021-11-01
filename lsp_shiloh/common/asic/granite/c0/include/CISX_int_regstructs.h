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
 * \file CISX_int_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CISX_INT_REGSTRUCTS_H_
#define _CISX_INT_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CISX_int (CISX_int)
//
//====================================================================

typedef struct CISX_INT_REGS_s
{
  volatile uint32_t CisxCfg;  ///< 0x0 [R/W]: CISX Configuration Register
  volatile uint32_t CisxStat;  ///< 0x4 [R]: Status register
  volatile uint32_t Chan0Pix;  ///< 0x8 [R/W]: Channel 0 pixels
  volatile uint32_t Chan1Pix;  ///< 0xc [R/W]: Channel 1 pixels
  volatile uint32_t Chan2Pix;  ///< 0x10 [R/W]: Channel 2 pixels
  volatile uint32_t Chan0Shuffle;  ///< 0x14 [R/W]: Channel 0 Shuffle Separation Pixels
  volatile uint32_t Chan1Shuffle;  ///< 0x18 [R/W]: Channel 1 Shuffle Separation Pixels
  volatile uint32_t Chan2Shuffle;  ///< 0x1c [R/W]: Channel 2 Shuffle Separation Pixels
  volatile uint32_t IntEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t IntPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t IntAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t IntForce;  ///< 0x2c [W]: Interrupt Force Register
} CISX_INT_REGS_t;

#endif // CISX_INT
