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
 * \file TIMEBASE_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _APB_TIMEBASE_REG_REGSTRUCTS_H_
#define _APB_TIMEBASE_REG_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Timebase (apb_timebase_reg)
/** \brief The timebase is used to generate timing periods that are useful to other blocks in the system, including the APB Timers/watchdog, Switcher, DRO monitor, and Temperature Sensor monitor. The timebase is programmable to generate correct period timing over a range of input system clock frequencies. By using a common configurable timebase, each block is not burdened with programmable dividers.<BR>
   The timebase provides the following output frequencies (single clock pulse):<BR>
 1Î¼s, 10Î¼s, 100Î¼s, 1ms, 10ms, 100ms*/
//
//====================================================================

typedef struct APB_TIMEBASE_REG_REGS_s
{
  volatile uint32_t TCR;  ///< 0x0 [R/W]: Timebase Configuration Register
  volatile uint32_t TTR;  ///< 0x4 [R/W]: Timebase Testing Register
} APB_TIMEBASE_REG_REGS_t;

#endif // APB_TIMEBASE_REG
