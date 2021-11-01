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
 * \file TIMERS_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TIMERS_REGSTRUCTS_H_
#define _TIMERS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: TIMERS (TIMERS)
/** \brief The APB TIMERS block provides programmable interval timers and watchdog. <BR>
The interval timers can be used to generate a repetitive period timer (for the RTO/S periodic tick interrupt), to time intervals (for switch presses, LED flashing, or fan speed), or for a watchdog reset. <BR>
Four identical programmable timers (Timer0, Timer1, Timer2, Timer3) are provided in the APB subsystem. Each timer generates a level interrupt when it reaches the final event, which can be programmed to be either a terminal count or an input pulse (on an external signal). When the final event is reached, the counter can restart at zero (continuous mode) or halt, depending on how it is configured. <BR>
Timer0 provides the watchdog function, and when enabled, the system will be reset if the watchdog timer reaches the final event. <BR>
The timer count is incremented on a selected input timebase. There are 8 choices of input timebases to select from: 1us, 10us, 100us, 1ms, 10ms and 100ms, the bus clock, and external input signal pulses. <BR>
To facilitate interval timing, the counter may be disabled to lock the count value that can then be read via the Timer Status Register. The counter is reset to zero and begins to increment when the Timer is re-enabled. <BR>
To assist system level functions, each timer can generate a signal that toggles when the final count is reached. The toggling signal is a periodic waveform that can be used within the SoC or be connected to the pins for further use by the system. <BR>*/
//
//====================================================================

typedef struct TIMERS_REGS_s
{
  volatile uint32_t TWR0;  ///< 0x0 [R/W]: Timer0 Watchdog Register
  volatile uint32_t TTCR0;  ///< 0x4 [R/W]: Timer0 Terminal Count Register
  volatile uint32_t TCR0;  ///< 0x8 [R/W]: Timer0 Control Register
  volatile uint32_t TSR0;  ///< 0xc [R]: Timer0 Status Register
  volatile uint32_t TISR;  ///< 0x10 [R]: Timers Interrupt Status Register
  volatile uint32_t TTCR1;  ///< 0x14 [R/W]: Timer1 Terminal Count Register
  volatile uint32_t TCR1;  ///< 0x18 [R/W]: Timer1 Control Register
  volatile uint32_t TSR1;  ///< 0x1c [R]: Timer1 Status Register
  volatile uint32_t TIAR;  ///< 0x20 [W]: Timers Interrupt Acknowledge Register
  volatile uint32_t TTCR2;  ///< 0x24 [R/W]: Timer2 Terminal Count Register
  volatile uint32_t TCR2;  ///< 0x28 [R/W]: Timer2 Control Register
  volatile uint32_t TSR2;  ///< 0x2c [R]: Timer2 Status Register
  volatile uint32_t reserved0;
  volatile uint32_t TTCR3;  ///< 0x34 [R/W]: Timer3 Terminal Count Register
  volatile uint32_t TCR3;  ///< 0x38 [R/W]: Timer3 Control Register
  volatile uint32_t TSR3;  ///< 0x3c [R]: Timer3 Status Register
} TIMERS_REGS_t;

#endif // TIMERS
