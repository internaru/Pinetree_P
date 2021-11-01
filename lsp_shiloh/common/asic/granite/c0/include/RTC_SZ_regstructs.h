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
 * \file RTC_SZ_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _RTC_SZ_REGSTRUCTS_H_
#define _RTC_SZ_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Register descriptions for Real-Time Clock module (RTC_SZ)
/** \brief Register Descriptions for the Swiss RTC.
    Since the interface to the RTC hard macro is only 8-bits wide,
    firmware MUST read each register twice. The first read updates a
    32-bit shadow in the RTC wrapper and the second read reads
    the 32-bit shadow.*/
//
//====================================================================

typedef struct RTC_SZ_REGS_s
{
  volatile uint32_t Status;  ///< 0x0 [R/W]: RTC Status Register
  volatile uint32_t Interrupt1;  ///< 0x4 [R/W]: RTC Interrupt 1 Register
  volatile uint32_t Interrupt2;  ///< 0x8 [R/W]: RTC Interrupt 2 Register
  volatile uint32_t Time;  ///< 0xc [R/W]: RTC Time Register
  volatile uint32_t Alarm1;  ///< 0x10 [R/W]: RTC Alarm 1 Register
  volatile uint32_t Alarm2;  ///< 0x14 [R/W]: RTC Alarm 2 Register
  volatile uint32_t ClockCorrect;  ///< 0x18 [R/W]: Clock Correction Register
  volatile uint32_t TestConfig;  ///< 0x1c [R/W]: RTC Test Configuration Register
  volatile uint32_t Timing;  ///< 0x20 [R/W]: RTC Timing Register
} RTC_SZ_REGS_t;

#endif // RTC_SZ
