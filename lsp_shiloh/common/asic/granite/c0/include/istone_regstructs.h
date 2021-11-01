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
 * \file istone_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ISTONE_REGSTRUCTS_H_
#define _ISTONE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: IsTone Calculations (istone)
/** \brief Top-level register file for the IsTone block*/
//
//====================================================================

typedef struct ISTONE_REGS_s
{
  volatile uint32_t IsCtrl;  ///< 0x0 [R/W]: IsTone Control Register
  volatile uint32_t IsLumThresh;  ///< 0x4 [R/W]: Luminance Thresholds Register
  volatile uint32_t IsChromThresh;  ///< 0x8 [R/W]: Chrominance Thresholds Register
  volatile uint32_t IsRotCoeff;  ///< 0xc [R/W]: Rotation Matrix Coefficient Register
  volatile uint32_t IsRotOffst;  ///< 0x10 [R/W]: Rotation Offset Register
  volatile uint32_t IsPxlOffst;  ///< 0x14 [R/W]: Chrominance Pixel Offset Register
  volatile uint32_t IsLumShift;  ///< 0x18 [R/W]: Luminance Shift Register
  volatile uint32_t IsLumComp;  ///< 0x1c [R/W]: Luminance Compensation Register
  volatile uint32_t IsMultDiv;  ///< 0x20 [R/W]: Chrominance Multiply / Divide Register
  volatile uint32_t IsThresh;  ///< 0x24 [R/W]: Test Value Threshold Register
} ISTONE_REGS_t;

#endif // ISTONE
