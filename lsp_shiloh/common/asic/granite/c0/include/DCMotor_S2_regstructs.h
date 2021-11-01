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
 * \file DCMotor_S2_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DCMOTOR_S2_REGSTRUCTS_H_
#define _DCMOTOR_S2_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: DCMotor (DCMotor_S2)
/** \brief Top-level register file for DCMotor*/
//
//====================================================================

typedef struct DCMOTOR_S2_REGS_s
{
  volatile uint32_t MCFG;  ///< 0x0 [R/W]: Motor Control Configuration
  volatile uint32_t IN_PIN_CFG;  ///< 0x4 [R/W]: Input Pin Configuration
  volatile uint32_t PDIV0;  ///< 0x8 [R/W]: PWM Frequency Divider 0
  volatile uint32_t PPER0;  ///< 0xc [R/W]: PWM Period 0
  volatile uint32_t PDTY0;  ///< 0x10 [R/W]: PWM Duty Cycle 0
  volatile uint32_t PDLY0;  ///< 0x14 [R/W]: PWM Delay 0
  volatile uint32_t PDIV1;  ///< 0x18 [R/W]: PWM Frequency Divider 1
  volatile uint32_t PPER1;  ///< 0x1c [R/W]: PWM Period 1
  volatile uint32_t PDTY1;  ///< 0x20 [R/W]: PWM Duty Cycle 1
  volatile uint32_t PDLY1;  ///< 0x24 [R/W]: PWM Delay 1
  volatile uint32_t PWATCH_CTRL;  ///< 0x28 [R/W]: PWM Watchdog Control
  volatile uint32_t PWATCH_STAT;  ///< 0x2c [R]: PWM Watchdog Status
  volatile uint32_t PWATCH_SERVICE;  ///< 0x30 [W]: PWM Watchdog Service
  volatile uint32_t PWATCH_CNT_WR;  ///< 0x34 [W]: PWM Watchdog Count Write
  volatile uint32_t ECFG;  ///< 0x38 [R/W]: Encoder Data Path Configuration
  volatile uint32_t SReset;  ///< 0x3c [R/W]: Soft Reset
  volatile uint32_t DB;  ///< 0x40 [R/W]: Debounce
  volatile uint32_t EPOS_RD;  ///< 0x44 [R]: Encoder Position Read
  volatile uint32_t EPOS_WR;  ///< 0x48 [W]: Encoder Position Write
  volatile uint32_t LPOS;  ///< 0x4c [R]: Encoder Latched Position
  volatile uint32_t APER;  ///< 0x50 [R]: ENCA Period
  volatile uint32_t BPER;  ///< 0x54 [R]: ENCB Period
  volatile uint32_t FILT_IN;  ///< 0x58 [R]: Filter Input Period
  volatile uint32_t TPER;  ///< 0x5c [R/W]: Test Period
  volatile uint32_t FILT_SAMPLE;  ///< 0x60 [R/W]: Filter Sample Rate
  volatile uint32_t FILK1;  ///< 0x64 [R/W]: Filter Input Gain K1
  volatile uint32_t reserved0[2];
  volatile uint32_t FILA2;  ///< 0x70 [R/W]: Filter Coefficient A2
  volatile uint32_t FILA3;  ///< 0x74 [R/W]: Filter Coefficient A3
  volatile uint32_t reserved1;
  volatile uint32_t FILSTAT;  ///< 0x7c [R]: Filter Status
  volatile uint32_t FILD1;  ///< 0x80 [R]: Filter Delay1
  volatile uint32_t FILD2;  ///< 0x84 [R]: Filter Delay2
  volatile uint32_t FILTOUT;  ///< 0x88 [R]: Filter Output
  volatile uint32_t RS_PM;  ///< 0x8c [R/W]: Row Sync Period Multiplier
  volatile uint32_t RS_DB;  ///< 0x90 [R/W]: Row Sync Input Debounce
  volatile uint32_t RS_DLY;  ///< 0x94 [R/W]: Row Sync Delay
  volatile uint32_t RS_OUT;  ///< 0x98 [R]: Row Sync Output
  volatile uint32_t RSTART;  ///< 0x9c [R/W]: Row Sync Start Position
  volatile uint32_t RSTOP;  ///< 0xa0 [R/W]: Row Sync Stop Position
  volatile uint32_t IEN;  ///< 0xa4 [R/W]: Interrupt Enable
  volatile uint32_t TS_IEN;  ///< 0xa8 [R/W]: Timestamp/EPOS Interrupt Enable
  volatile uint32_t IPEND;  ///< 0xac [R]: Interrupt Pending
  volatile uint32_t IACK;  ///< 0xb0 [W]: Interrupt Acknowledge
  volatile uint32_t IFORCE;  ///< 0xb4 [W]: Interrupt Force
  volatile uint32_t TS_CTRL;  ///< 0xb8 [R/W]: Timestamp Control
  volatile uint32_t TS_CNT;  ///< 0xbc [R]: Timestamp Cnt Read
  volatile uint32_t TS_ENC_0;  ///< 0xc0 [R]: Enc Timestamp 0
  volatile uint32_t TS_ENC_1;  ///< 0xc4 [R]: Enc Timestamp 1
  volatile uint32_t TS_ENC_2;  ///< 0xc8 [R]: Enc Timestamp 2
  volatile uint32_t TS_ENC_3;  ///< 0xcc [R]: Enc Timestamp 3
  volatile uint32_t TS_ENC_4;  ///< 0xd0 [R]: Enc Timestamp 4
  volatile uint32_t LATCH_EN;  ///< 0xd4 [W]: Latch Enable
  volatile uint32_t LATCH_EPOS;  ///< 0xd8 [R]: Latched Encoder Position
  volatile uint32_t LATCH_TS_CNT;  ///< 0xdc [R]: Latched Enc Timestamp Cnt
  volatile uint32_t LATCH_TS_ENC_0;  ///< 0xe0 [R]: Latched Enc Timestamp 0
  volatile uint32_t LATCH_TS_ENC_1;  ///< 0xe4 [R]: Latched Enc Timestamp 1
  volatile uint32_t LATCH_TS_ENC_2;  ///< 0xe8 [R]: Latched Enc Timestamp 2
  volatile uint32_t LATCH_TS_ENC_3;  ///< 0xec [R]: Latched Enc Timestamp 3
  volatile uint32_t LATCH_TS_ENC_4;  ///< 0xf0 [R]: Latched Enc Timestamp 4
  volatile uint32_t INT_ENC_TS;  ///< 0xf4 [R]: Interrupt Encoder Timestamp
  volatile uint32_t INT_TS;  ///< 0xf8 [R]: Interrupt Timestamp
  volatile uint32_t INT_EPOS;  ///< 0xfc [R]: Interrupt Encoder Position
  volatile uint32_t WATCH_CTRL;  ///< 0x100 [R/W]: Encoder Watch Control
  volatile uint32_t WATCH_VALUE;  ///< 0x104 [R/W]: Encoder Watch Value
  volatile uint32_t WATCH_CNT;  ///< 0x108 [R]: Encoder Watch Counter Read
  volatile uint32_t ENC_INT_CFG;  ///< 0x10c [R/W]: Encoder Edge Interrupt Configuration
  volatile uint32_t TRIG_EPOS;  ///< 0x110 [R/W]: Encoder Position Trigger
  volatile uint32_t TRIG_CTRL;  ///< 0x114 [R/W]: Trigger Control Register
  volatile uint32_t S2_CFG;  ///< 0x118 [R/W]: S2 Cfg Register
  volatile uint32_t S2_DB;  ///< 0x11c [R/W]: S2 Debounce
  volatile uint32_t S2_TSTAMP;  ///< 0x120 [R]: S2 Timestamp Read
  volatile uint32_t S2_ENCPOS_INT;  ///< 0x124 [R]: S2 Internal Encoder Position
  volatile uint32_t S2_ENC_TSTAMP_INT;  ///< 0x128 [R]: S2 Internal Enc Timestamp
  volatile uint32_t S2_ENCPOS_EXT;  ///< 0x12c [R]: S2 External Encoder Position
  volatile uint32_t S2_ENC_TSTAMP_EXT;  ///< 0x130 [R]: S2 External Enc Timestamp
} DCMOTOR_S2_REGS_t;

#endif // DCMOTOR_S2
