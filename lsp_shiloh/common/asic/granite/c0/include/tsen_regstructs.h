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
 * \file tsen_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TSEN_REGS_REGSTRUCTS_H_
#define _TSEN_REGS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: TSEN Temperature Sensor Registers (tsen_regs)
/** \brief Registers for firmware control/configuration/status to TSEN hardmacro.*/
//
//====================================================================

typedef struct TSEN_REGS_REGS_s
{
  volatile uint32_t SoftReset;  ///< 0x0 [R/W]: Soft Reset register
  volatile uint32_t StartCal;  ///< 0x4 [R/W]: Start Calibration register
  volatile uint32_t PWControl;  ///< 0x8 [R/W]: Power Control register
  volatile uint32_t ConfigSensor;  ///< 0xc [R/W]: Sensor Configuration register
  volatile uint32_t ConfigCalib;  ///< 0x10 [R/W]: Calibration Configuration register
  volatile uint32_t Status1;  ///< 0x14 [R]: Internal Status register
  volatile uint32_t Status2;  ///< 0x18 [R]: Temperature Status register
} TSEN_REGS_REGS_t;

#endif // TSEN_REGS
