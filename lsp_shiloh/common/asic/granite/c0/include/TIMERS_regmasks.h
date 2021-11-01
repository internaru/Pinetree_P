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
 * \file TIMERS_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TIMERS_REGMASKS_H_
#define _TIMERS_REGMASKS_H_

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

//====================================================================
//Register: Timer0 Watchdog Register (TWR0)
/** \brief When Watchdog Mode is enabled, this register must be serviced properly or the system will reset. This is useful to automatically reboot a malfunctioning system. This register must be correctly serviced before timer0 reaches the terminal event (see Control register bit[2]), which can be a terminal count (see Terminal Count register) or pulse. Also, if improperly serviced, the system will also immediately reset (if Watchdog Mode is enabled). In addition, when you write this register to disable the watchdog (Enable bit zero), it must be correctly serviced or a system reset will be generated (this is to prevent a malfunctioning system from accidentally disabling the watchdog).<BR>
            The correct method of servicing the watchdog is to read the register value, then write back the value with the lower four bits inverted (XOR with 0x0000000f). The register read value cycles in a pseudo-random pattern.<BR>
            Attention: the Timer0 Control Register must be placed in Continuous mode (TCR0.Mode=1) for the watchdog timeout and reset to properly function. When Timer0 is placed in non-continuous mode, the watchdog reset function is de-activated even if the watchdog mode is enabled (TWR0.Enable=1).*/
//====================================================================

#define  TIMERS_TWR0_ENABLE_MASK 0x80000000
#define  TIMERS_TWR0_ENABLE_SHIFT 31
#define  TIMERS_TWR0_ENABLE_MASK_SHIFT(reg) (((reg) & TIMERS_TWR0_ENABLE_MASK) >> TIMERS_TWR0_ENABLE_SHIFT)
#define  TIMERS_TWR0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TWR0_ENABLE_MASK) | (((uint32_t)val) << TIMERS_TWR0_ENABLE_SHIFT))

#define  TIMERS_TWR0_RESERVED1_MASK 0x7ffffff0
#define  TIMERS_TWR0_RESERVED1_SHIFT 4
#define  TIMERS_TWR0_RESERVED1_MASK_SHIFT(reg) (((reg) & TIMERS_TWR0_RESERVED1_MASK) >> TIMERS_TWR0_RESERVED1_SHIFT)
#define  TIMERS_TWR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TWR0_RESERVED1_MASK) | (((uint32_t)val) << TIMERS_TWR0_RESERVED1_SHIFT))

#define  TIMERS_TWR0_SERVICE_MASK 0xf
#define  TIMERS_TWR0_SERVICE_SHIFT 0
#define  TIMERS_TWR0_SERVICE_MASK_SHIFT(reg) (((reg) & TIMERS_TWR0_SERVICE_MASK) >> TIMERS_TWR0_SERVICE_SHIFT)
#define  TIMERS_TWR0_SERVICE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TWR0_SERVICE_MASK) | (((uint32_t)val) << TIMERS_TWR0_SERVICE_SHIFT))

//====================================================================
//Register: Timers Interrupt Status Register (TISR)
/** \brief This register provides the raw interrupt status. The TimerIRQ[3:0] interrupt request lines are passed to an Interrupt Controller. The TimerIRQ lines are individually maskable using the Interrupt Controller's Interrupt Enable Register.*/
//====================================================================

#define  TIMERS_TISR_RESERVED1_MASK 0xfffffff0
#define  TIMERS_TISR_RESERVED1_SHIFT 4
#define  TIMERS_TISR_RESERVED1_MASK_SHIFT(reg) (((reg) & TIMERS_TISR_RESERVED1_MASK) >> TIMERS_TISR_RESERVED1_SHIFT)
#define  TIMERS_TISR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TISR_RESERVED1_MASK) | (((uint32_t)val) << TIMERS_TISR_RESERVED1_SHIFT))

#define  TIMERS_TISR_STATUS3_MASK 0x8
#define  TIMERS_TISR_STATUS3_SHIFT 3
#define  TIMERS_TISR_STATUS3_MASK_SHIFT(reg) (((reg) & TIMERS_TISR_STATUS3_MASK) >> TIMERS_TISR_STATUS3_SHIFT)
#define  TIMERS_TISR_STATUS3_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TISR_STATUS3_MASK) | (((uint32_t)val) << TIMERS_TISR_STATUS3_SHIFT))

#define  TIMERS_TISR_STATUS2_MASK 0x4
#define  TIMERS_TISR_STATUS2_SHIFT 2
#define  TIMERS_TISR_STATUS2_MASK_SHIFT(reg) (((reg) & TIMERS_TISR_STATUS2_MASK) >> TIMERS_TISR_STATUS2_SHIFT)
#define  TIMERS_TISR_STATUS2_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TISR_STATUS2_MASK) | (((uint32_t)val) << TIMERS_TISR_STATUS2_SHIFT))

#define  TIMERS_TISR_STATUS1_MASK 0x2
#define  TIMERS_TISR_STATUS1_SHIFT 1
#define  TIMERS_TISR_STATUS1_MASK_SHIFT(reg) (((reg) & TIMERS_TISR_STATUS1_MASK) >> TIMERS_TISR_STATUS1_SHIFT)
#define  TIMERS_TISR_STATUS1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TISR_STATUS1_MASK) | (((uint32_t)val) << TIMERS_TISR_STATUS1_SHIFT))

#define  TIMERS_TISR_STATUS0_MASK 0x1
#define  TIMERS_TISR_STATUS0_SHIFT 0
#define  TIMERS_TISR_STATUS0_MASK_SHIFT(reg) (((reg) & TIMERS_TISR_STATUS0_MASK) >> TIMERS_TISR_STATUS0_SHIFT)
#define  TIMERS_TISR_STATUS0_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TISR_STATUS0_MASK) | (((uint32_t)val) << TIMERS_TISR_STATUS0_SHIFT))

//====================================================================
//Register: Timers Interrupt Acknowledge Register (TIAR)
/** \brief This register is written to acknowledge and clear the TimerIRQs.*/
//====================================================================

#define  TIMERS_TIAR_RESERVED1_MASK 0xfffffff0
#define  TIMERS_TIAR_RESERVED1_SHIFT 4
#define  TIMERS_TIAR_RESERVED1_MASK_SHIFT(reg) (((reg) & TIMERS_TIAR_RESERVED1_MASK) >> TIMERS_TIAR_RESERVED1_SHIFT)
#define  TIMERS_TIAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TIAR_RESERVED1_MASK) | (((uint32_t)val) << TIMERS_TIAR_RESERVED1_SHIFT))

#define  TIMERS_TIAR_ACK3_MASK 0x8
#define  TIMERS_TIAR_ACK3_SHIFT 3
#define  TIMERS_TIAR_ACK3_MASK_SHIFT(reg) (((reg) & TIMERS_TIAR_ACK3_MASK) >> TIMERS_TIAR_ACK3_SHIFT)
#define  TIMERS_TIAR_ACK3_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TIAR_ACK3_MASK) | (((uint32_t)val) << TIMERS_TIAR_ACK3_SHIFT))

#define  TIMERS_TIAR_ACK2_MASK 0x4
#define  TIMERS_TIAR_ACK2_SHIFT 2
#define  TIMERS_TIAR_ACK2_MASK_SHIFT(reg) (((reg) & TIMERS_TIAR_ACK2_MASK) >> TIMERS_TIAR_ACK2_SHIFT)
#define  TIMERS_TIAR_ACK2_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TIAR_ACK2_MASK) | (((uint32_t)val) << TIMERS_TIAR_ACK2_SHIFT))

#define  TIMERS_TIAR_ACK1_MASK 0x2
#define  TIMERS_TIAR_ACK1_SHIFT 1
#define  TIMERS_TIAR_ACK1_MASK_SHIFT(reg) (((reg) & TIMERS_TIAR_ACK1_MASK) >> TIMERS_TIAR_ACK1_SHIFT)
#define  TIMERS_TIAR_ACK1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TIAR_ACK1_MASK) | (((uint32_t)val) << TIMERS_TIAR_ACK1_SHIFT))

#define  TIMERS_TIAR_ACK0_MASK 0x1
#define  TIMERS_TIAR_ACK0_SHIFT 0
#define  TIMERS_TIAR_ACK0_MASK_SHIFT(reg) (((reg) & TIMERS_TIAR_ACK0_MASK) >> TIMERS_TIAR_ACK0_SHIFT)
#define  TIMERS_TIAR_ACK0_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TIAR_ACK0_MASK) | (((uint32_t)val) << TIMERS_TIAR_ACK0_SHIFT))

//====================================================================
//Register: Timer0 Terminal Count Register (TTCR0)
/** \brief This register sets the final count where the interrupt is generated. For Continuous mode the counter will reset to zero on the next timebase period; otherwise, the counter will stop at the terminal count (programmed in this register) in terminal count mode (Control register bit[2]=0) or at the current count in terminal pulse mode (Control bit[2]=1).*/
//====================================================================

#define  TIMERS_TTCR0_TERMINALCOUNT_MASK 0xffffffff
#define  TIMERS_TTCR0_TERMINALCOUNT_SHIFT 0
#define  TIMERS_TTCR0_TERMINALCOUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TTCR0_TERMINALCOUNT_MASK) >> TIMERS_TTCR0_TERMINALCOUNT_SHIFT)
#define  TIMERS_TTCR0_TERMINALCOUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TTCR0_TERMINALCOUNT_MASK) | (((uint32_t)val) << TIMERS_TTCR0_TERMINALCOUNT_SHIFT))

//====================================================================
//Register: Timer0 Control Register (TCR0)
/** \brief The control register is used to configure the operation of the timer. The timer count is reset when the counter is enabled (Enable bit changes from 0 to 1). When in continuous mode (see bit[1]), the counter will reset and continue after reaching the terminal event, otherwise the timer stops when reaching the terminal event. The Terminal event (see bit[2]) can be a pulse on the external input signal or a pre-programmed count. <BR>
            The timer counter may be disabled (write Enable bit to 0 after it was originally enabled) to lock the count value. The resulting count value can be read from the Timer Status Register.*/
//====================================================================

#define  TIMERS_TCR0_RESERVED1_MASK 0xffffff80
#define  TIMERS_TCR0_RESERVED1_SHIFT 7
#define  TIMERS_TCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & TIMERS_TCR0_RESERVED1_MASK) >> TIMERS_TCR0_RESERVED1_SHIFT)
#define  TIMERS_TCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR0_RESERVED1_MASK) | (((uint32_t)val) << TIMERS_TCR0_RESERVED1_SHIFT))

#define  TIMERS_TCR0_TIMEBASESEL_MASK 0x70
#define  TIMERS_TCR0_TIMEBASESEL_SHIFT 4
#define  TIMERS_TCR0_TIMEBASESEL_MASK_SHIFT(reg) (((reg) & TIMERS_TCR0_TIMEBASESEL_MASK) >> TIMERS_TCR0_TIMEBASESEL_SHIFT)
#define  TIMERS_TCR0_TIMEBASESEL_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR0_TIMEBASESEL_MASK) | (((uint32_t)val) << TIMERS_TCR0_TIMEBASESEL_SHIFT))

#define  TIMERS_TCR0_POLARITY_MASK 0x8
#define  TIMERS_TCR0_POLARITY_SHIFT 3
#define  TIMERS_TCR0_POLARITY_MASK_SHIFT(reg) (((reg) & TIMERS_TCR0_POLARITY_MASK) >> TIMERS_TCR0_POLARITY_SHIFT)
#define  TIMERS_TCR0_POLARITY_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR0_POLARITY_MASK) | (((uint32_t)val) << TIMERS_TCR0_POLARITY_SHIFT))

#define  TIMERS_TCR0_TERMINALMODE_MASK 0x4
#define  TIMERS_TCR0_TERMINALMODE_SHIFT 2
#define  TIMERS_TCR0_TERMINALMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR0_TERMINALMODE_MASK) >> TIMERS_TCR0_TERMINALMODE_SHIFT)
#define  TIMERS_TCR0_TERMINALMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR0_TERMINALMODE_MASK) | (((uint32_t)val) << TIMERS_TCR0_TERMINALMODE_SHIFT))

#define  TIMERS_TCR0_CONTMODE_MASK 0x2
#define  TIMERS_TCR0_CONTMODE_SHIFT 1
#define  TIMERS_TCR0_CONTMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR0_CONTMODE_MASK) >> TIMERS_TCR0_CONTMODE_SHIFT)
#define  TIMERS_TCR0_CONTMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR0_CONTMODE_MASK) | (((uint32_t)val) << TIMERS_TCR0_CONTMODE_SHIFT))

#define  TIMERS_TCR0_ENABLE_MASK 0x1
#define  TIMERS_TCR0_ENABLE_SHIFT 0
#define  TIMERS_TCR0_ENABLE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR0_ENABLE_MASK) >> TIMERS_TCR0_ENABLE_SHIFT)
#define  TIMERS_TCR0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR0_ENABLE_MASK) | (((uint32_t)val) << TIMERS_TCR0_ENABLE_SHIFT))

//====================================================================
//Register: Timer0 Status Register (TSR0)
/** \brief The Status register returns the current timer count value, which updates on each timebase tick (selected by Control Timebase Input Select bits), as long the timer is enabled (Control.Enable=1).<BR>
            When the timer has been enabled and is counting, the timer counter may be disabled (write Control.Enable bit to 0) and the counter locks the current value, so that firmware can read the current count. NOTE - when the timer is re-enabled (write Enable bit to 1), the count value is reset to zero.<BR>
            If firmware wishes to obtain a reliable count value without having to disable the timer, firmware can rely on the TimerIRQ to indicate that the final count is reached. Alternatively, firmware can do two consecutive reads of the Status register, and two identical consecutive readings means that the count value is valid because it has not updated since the last timebase tick. For these two methods to work, be sure to set the Control.Enable bit to 1, and the Control.ContinuousMode bit to 0 (stop when reach terminal event).<BR>
            When the timer is configured to stop counting when reaching a terminal pulse event (Control.ContMode bit is 0, and Control.TerminalMode bit is 1), this registers returns the elapsed time between two consecutive pulses. The TimerIRQ interrupt indicates when the final count is ready to read from this register. This functional mode is useful, for example, for measuring the external fan speed. The count is directly related to the fan rpm's.*/
//====================================================================

#define  TIMERS_TSR0_COUNT_MASK 0xffffffff
#define  TIMERS_TSR0_COUNT_SHIFT 0
#define  TIMERS_TSR0_COUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TSR0_COUNT_MASK) >> TIMERS_TSR0_COUNT_SHIFT)
#define  TIMERS_TSR0_COUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TSR0_COUNT_MASK) | (((uint32_t)val) << TIMERS_TSR0_COUNT_SHIFT))

//====================================================================
//Register: Timer1 Terminal Count Register (TTCR1)
/** \brief See description for Timer0 Terminal Count Register.*/
//====================================================================

#define  TIMERS_TTCR1_TERMINALCOUNT_MASK 0xffffffff
#define  TIMERS_TTCR1_TERMINALCOUNT_SHIFT 0
#define  TIMERS_TTCR1_TERMINALCOUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TTCR1_TERMINALCOUNT_MASK) >> TIMERS_TTCR1_TERMINALCOUNT_SHIFT)
#define  TIMERS_TTCR1_TERMINALCOUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TTCR1_TERMINALCOUNT_MASK) | (((uint32_t)val) << TIMERS_TTCR1_TERMINALCOUNT_SHIFT))

//====================================================================
//Register: Timer1 Control Register (TCR1)
/** \brief See description for Timer0 Control Register.*/
//====================================================================

#define  TIMERS_TCR1_RESERVED1_MASK 0xffffff80
#define  TIMERS_TCR1_RESERVED1_SHIFT 7
#define  TIMERS_TCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & TIMERS_TCR1_RESERVED1_MASK) >> TIMERS_TCR1_RESERVED1_SHIFT)
#define  TIMERS_TCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR1_RESERVED1_MASK) | (((uint32_t)val) << TIMERS_TCR1_RESERVED1_SHIFT))

#define  TIMERS_TCR1_TIMEBASESEL_MASK 0x70
#define  TIMERS_TCR1_TIMEBASESEL_SHIFT 4
#define  TIMERS_TCR1_TIMEBASESEL_MASK_SHIFT(reg) (((reg) & TIMERS_TCR1_TIMEBASESEL_MASK) >> TIMERS_TCR1_TIMEBASESEL_SHIFT)
#define  TIMERS_TCR1_TIMEBASESEL_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR1_TIMEBASESEL_MASK) | (((uint32_t)val) << TIMERS_TCR1_TIMEBASESEL_SHIFT))

#define  TIMERS_TCR1_POLARITY_MASK 0x8
#define  TIMERS_TCR1_POLARITY_SHIFT 3
#define  TIMERS_TCR1_POLARITY_MASK_SHIFT(reg) (((reg) & TIMERS_TCR1_POLARITY_MASK) >> TIMERS_TCR1_POLARITY_SHIFT)
#define  TIMERS_TCR1_POLARITY_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR1_POLARITY_MASK) | (((uint32_t)val) << TIMERS_TCR1_POLARITY_SHIFT))

#define  TIMERS_TCR1_TERMINALMODE_MASK 0x4
#define  TIMERS_TCR1_TERMINALMODE_SHIFT 2
#define  TIMERS_TCR1_TERMINALMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR1_TERMINALMODE_MASK) >> TIMERS_TCR1_TERMINALMODE_SHIFT)
#define  TIMERS_TCR1_TERMINALMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR1_TERMINALMODE_MASK) | (((uint32_t)val) << TIMERS_TCR1_TERMINALMODE_SHIFT))

#define  TIMERS_TCR1_CONTMODE_MASK 0x2
#define  TIMERS_TCR1_CONTMODE_SHIFT 1
#define  TIMERS_TCR1_CONTMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR1_CONTMODE_MASK) >> TIMERS_TCR1_CONTMODE_SHIFT)
#define  TIMERS_TCR1_CONTMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR1_CONTMODE_MASK) | (((uint32_t)val) << TIMERS_TCR1_CONTMODE_SHIFT))

#define  TIMERS_TCR1_ENABLE_MASK 0x1
#define  TIMERS_TCR1_ENABLE_SHIFT 0
#define  TIMERS_TCR1_ENABLE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR1_ENABLE_MASK) >> TIMERS_TCR1_ENABLE_SHIFT)
#define  TIMERS_TCR1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR1_ENABLE_MASK) | (((uint32_t)val) << TIMERS_TCR1_ENABLE_SHIFT))

//====================================================================
//Register: Timer1 Status Register (TSR1)
/** \brief See description for Timer0 Status Register.*/
//====================================================================

#define  TIMERS_TSR1_COUNT_MASK 0xffffffff
#define  TIMERS_TSR1_COUNT_SHIFT 0
#define  TIMERS_TSR1_COUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TSR1_COUNT_MASK) >> TIMERS_TSR1_COUNT_SHIFT)
#define  TIMERS_TSR1_COUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TSR1_COUNT_MASK) | (((uint32_t)val) << TIMERS_TSR1_COUNT_SHIFT))

//====================================================================
//Register: Timer2 Terminal Count Register (TTCR2)
/** \brief See description for Timer0 Terminal Count Register.*/
//====================================================================

#define  TIMERS_TTCR2_TERMINALCOUNT_MASK 0xffffffff
#define  TIMERS_TTCR2_TERMINALCOUNT_SHIFT 0
#define  TIMERS_TTCR2_TERMINALCOUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TTCR2_TERMINALCOUNT_MASK) >> TIMERS_TTCR2_TERMINALCOUNT_SHIFT)
#define  TIMERS_TTCR2_TERMINALCOUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TTCR2_TERMINALCOUNT_MASK) | (((uint32_t)val) << TIMERS_TTCR2_TERMINALCOUNT_SHIFT))

//====================================================================
//Register: Timer2 Control Register (TCR2)
/** \brief See description for Timer0 Control Register.*/
//====================================================================

#define  TIMERS_TCR2_RESERVED1_MASK 0xffffff80
#define  TIMERS_TCR2_RESERVED1_SHIFT 7
#define  TIMERS_TCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & TIMERS_TCR2_RESERVED1_MASK) >> TIMERS_TCR2_RESERVED1_SHIFT)
#define  TIMERS_TCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR2_RESERVED1_MASK) | (((uint32_t)val) << TIMERS_TCR2_RESERVED1_SHIFT))

#define  TIMERS_TCR2_TIMEBASESEL_MASK 0x70
#define  TIMERS_TCR2_TIMEBASESEL_SHIFT 4
#define  TIMERS_TCR2_TIMEBASESEL_MASK_SHIFT(reg) (((reg) & TIMERS_TCR2_TIMEBASESEL_MASK) >> TIMERS_TCR2_TIMEBASESEL_SHIFT)
#define  TIMERS_TCR2_TIMEBASESEL_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR2_TIMEBASESEL_MASK) | (((uint32_t)val) << TIMERS_TCR2_TIMEBASESEL_SHIFT))

#define  TIMERS_TCR2_POLARITY_MASK 0x8
#define  TIMERS_TCR2_POLARITY_SHIFT 3
#define  TIMERS_TCR2_POLARITY_MASK_SHIFT(reg) (((reg) & TIMERS_TCR2_POLARITY_MASK) >> TIMERS_TCR2_POLARITY_SHIFT)
#define  TIMERS_TCR2_POLARITY_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR2_POLARITY_MASK) | (((uint32_t)val) << TIMERS_TCR2_POLARITY_SHIFT))

#define  TIMERS_TCR2_TERMINALMODE_MASK 0x4
#define  TIMERS_TCR2_TERMINALMODE_SHIFT 2
#define  TIMERS_TCR2_TERMINALMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR2_TERMINALMODE_MASK) >> TIMERS_TCR2_TERMINALMODE_SHIFT)
#define  TIMERS_TCR2_TERMINALMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR2_TERMINALMODE_MASK) | (((uint32_t)val) << TIMERS_TCR2_TERMINALMODE_SHIFT))

#define  TIMERS_TCR2_CONTMODE_MASK 0x2
#define  TIMERS_TCR2_CONTMODE_SHIFT 1
#define  TIMERS_TCR2_CONTMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR2_CONTMODE_MASK) >> TIMERS_TCR2_CONTMODE_SHIFT)
#define  TIMERS_TCR2_CONTMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR2_CONTMODE_MASK) | (((uint32_t)val) << TIMERS_TCR2_CONTMODE_SHIFT))

#define  TIMERS_TCR2_ENABLE_MASK 0x1
#define  TIMERS_TCR2_ENABLE_SHIFT 0
#define  TIMERS_TCR2_ENABLE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR2_ENABLE_MASK) >> TIMERS_TCR2_ENABLE_SHIFT)
#define  TIMERS_TCR2_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR2_ENABLE_MASK) | (((uint32_t)val) << TIMERS_TCR2_ENABLE_SHIFT))

//====================================================================
//Register: Timer2 Status Register (TSR2)
/** \brief See description for Timer0 Status Register.*/
//====================================================================

#define  TIMERS_TSR2_COUNT_MASK 0xffffffff
#define  TIMERS_TSR2_COUNT_SHIFT 0
#define  TIMERS_TSR2_COUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TSR2_COUNT_MASK) >> TIMERS_TSR2_COUNT_SHIFT)
#define  TIMERS_TSR2_COUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TSR2_COUNT_MASK) | (((uint32_t)val) << TIMERS_TSR2_COUNT_SHIFT))

//====================================================================
//Register: Timer3 Terminal Count Register (TTCR3)
/** \brief See description for Timer0 Terminal Count Register.*/
//====================================================================

#define  TIMERS_TTCR3_TERMINALCOUNT_MASK 0xffffffff
#define  TIMERS_TTCR3_TERMINALCOUNT_SHIFT 0
#define  TIMERS_TTCR3_TERMINALCOUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TTCR3_TERMINALCOUNT_MASK) >> TIMERS_TTCR3_TERMINALCOUNT_SHIFT)
#define  TIMERS_TTCR3_TERMINALCOUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TTCR3_TERMINALCOUNT_MASK) | (((uint32_t)val) << TIMERS_TTCR3_TERMINALCOUNT_SHIFT))

//====================================================================
//Register: Timer3 Control Register (TCR3)
/** \brief See description for Timer0 Control Register.*/
//====================================================================

#define  TIMERS_TCR3_RESERVED1_MASK 0xffffff80
#define  TIMERS_TCR3_RESERVED1_SHIFT 7
#define  TIMERS_TCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & TIMERS_TCR3_RESERVED1_MASK) >> TIMERS_TCR3_RESERVED1_SHIFT)
#define  TIMERS_TCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR3_RESERVED1_MASK) | (((uint32_t)val) << TIMERS_TCR3_RESERVED1_SHIFT))

#define  TIMERS_TCR3_TIMEBASESEL_MASK 0x70
#define  TIMERS_TCR3_TIMEBASESEL_SHIFT 4
#define  TIMERS_TCR3_TIMEBASESEL_MASK_SHIFT(reg) (((reg) & TIMERS_TCR3_TIMEBASESEL_MASK) >> TIMERS_TCR3_TIMEBASESEL_SHIFT)
#define  TIMERS_TCR3_TIMEBASESEL_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR3_TIMEBASESEL_MASK) | (((uint32_t)val) << TIMERS_TCR3_TIMEBASESEL_SHIFT))

#define  TIMERS_TCR3_POLARITY_MASK 0x8
#define  TIMERS_TCR3_POLARITY_SHIFT 3
#define  TIMERS_TCR3_POLARITY_MASK_SHIFT(reg) (((reg) & TIMERS_TCR3_POLARITY_MASK) >> TIMERS_TCR3_POLARITY_SHIFT)
#define  TIMERS_TCR3_POLARITY_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR3_POLARITY_MASK) | (((uint32_t)val) << TIMERS_TCR3_POLARITY_SHIFT))

#define  TIMERS_TCR3_TERMINALMODE_MASK 0x4
#define  TIMERS_TCR3_TERMINALMODE_SHIFT 2
#define  TIMERS_TCR3_TERMINALMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR3_TERMINALMODE_MASK) >> TIMERS_TCR3_TERMINALMODE_SHIFT)
#define  TIMERS_TCR3_TERMINALMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR3_TERMINALMODE_MASK) | (((uint32_t)val) << TIMERS_TCR3_TERMINALMODE_SHIFT))

#define  TIMERS_TCR3_CONTMODE_MASK 0x2
#define  TIMERS_TCR3_CONTMODE_SHIFT 1
#define  TIMERS_TCR3_CONTMODE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR3_CONTMODE_MASK) >> TIMERS_TCR3_CONTMODE_SHIFT)
#define  TIMERS_TCR3_CONTMODE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR3_CONTMODE_MASK) | (((uint32_t)val) << TIMERS_TCR3_CONTMODE_SHIFT))

#define  TIMERS_TCR3_ENABLE_MASK 0x1
#define  TIMERS_TCR3_ENABLE_SHIFT 0
#define  TIMERS_TCR3_ENABLE_MASK_SHIFT(reg) (((reg) & TIMERS_TCR3_ENABLE_MASK) >> TIMERS_TCR3_ENABLE_SHIFT)
#define  TIMERS_TCR3_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TCR3_ENABLE_MASK) | (((uint32_t)val) << TIMERS_TCR3_ENABLE_SHIFT))

//====================================================================
//Register: Timer3 Status Register (TSR3)
/** \brief See description for Timer0 Status Register.*/
//====================================================================

#define  TIMERS_TSR3_COUNT_MASK 0xffffffff
#define  TIMERS_TSR3_COUNT_SHIFT 0
#define  TIMERS_TSR3_COUNT_MASK_SHIFT(reg) (((reg) & TIMERS_TSR3_COUNT_MASK) >> TIMERS_TSR3_COUNT_SHIFT)
#define  TIMERS_TSR3_COUNT_REPLACE_VAL(reg,val) (((reg) & ~TIMERS_TSR3_COUNT_MASK) | (((uint32_t)val) << TIMERS_TSR3_COUNT_SHIFT))

#endif // TIMERS
