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
 * \file LSCAN_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _LSCAN_REGSTRUCTS_H_
#define _LSCAN_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: LSCAN (LSCAN)
//
//====================================================================

//====================================================================
//
//Register File: HSync Control (hsync)
//
//====================================================================

typedef struct HSYNC_REGS_s
{
  volatile uint32_t hcfg;  ///< 0x0 [R/W]: HSync Configuration register
  volatile uint32_t combine;  ///< 0x4 [R/W]: HSync Combination register
  volatile uint32_t hfilt;  ///< 0x8 [R/W]: HSync Filter Configuration register
  volatile uint32_t tcfg1;  ///< 0xc [R/W]: HSync Timers Configuration 1 register
  volatile uint32_t tcfg2;  ///< 0x10 [R/W]: HSync Timers Configuration 2 register
  volatile uint32_t reserved0[3];
  volatile uint32_t timer1;  ///< 0x20 [R]: HSync Timer register (Instance 1 of 16)
  volatile uint32_t timer2;  ///< 0x24 [R]: HSync Timer register (Instance 2 of 16)
  volatile uint32_t timer3;  ///< 0x28 [R]: HSync Timer register (Instance 3 of 16)
  volatile uint32_t timer4;  ///< 0x2c [R]: HSync Timer register (Instance 4 of 16)
  volatile uint32_t timer5;  ///< 0x30 [R]: HSync Timer register (Instance 5 of 16)
  volatile uint32_t timer6;  ///< 0x34 [R]: HSync Timer register (Instance 6 of 16)
  volatile uint32_t timer7;  ///< 0x38 [R]: HSync Timer register (Instance 7 of 16)
  volatile uint32_t timer8;  ///< 0x3c [R]: HSync Timer register (Instance 8 of 16)
  volatile uint32_t timer9;  ///< 0x40 [R]: HSync Timer register (Instance 9 of 16)
  volatile uint32_t timer10;  ///< 0x44 [R]: HSync Timer register (Instance 10 of 16)
  volatile uint32_t timer11;  ///< 0x48 [R]: HSync Timer register (Instance 11 of 16)
  volatile uint32_t timer12;  ///< 0x4c [R]: HSync Timer register (Instance 12 of 16)
  volatile uint32_t timer13;  ///< 0x50 [R]: HSync Timer register (Instance 13 of 16)
  volatile uint32_t timer14;  ///< 0x54 [R]: HSync Timer register (Instance 14 of 16)
  volatile uint32_t timer15;  ///< 0x58 [R]: HSync Timer register (Instance 15 of 16)
  volatile uint32_t timer16;  ///< 0x5c [R]: HSync Timer register (Instance 16 of 16)
  volatile uint32_t count_en;  ///< 0x60 [R/W]: HSync Counter Enable register
  volatile uint32_t count_cl;  ///< 0x64 [W]: HSync Counter Clear register
  volatile uint32_t counter1;  ///< 0x68 [R]: HSync Counter register (Instance 1 of 4)
  volatile uint32_t counter2;  ///< 0x6c [R]: HSync Counter register (Instance 2 of 4)
  volatile uint32_t counter3;  ///< 0x70 [R]: HSync Counter register (Instance 3 of 4)
  volatile uint32_t counter4;  ///< 0x74 [R]: HSync Counter register (Instance 4 of 4)
} HSYNC_REGS_t;

//====================================================================
//
//Register File: Mirror Control (Instance 1 of 2) (mirror0)
//
//====================================================================

typedef struct MIRROR0_REGS_s
{
  volatile uint32_t mcfg1;  ///< 0x0 [R/W]: Mirror Configuration 1 register
  volatile uint32_t mcfg2;  ///< 0x4 [R/W]: Mirror Configuration 2 register
  volatile uint32_t status;  ///< 0x8 [R]: Mirror Status register
  volatile uint32_t lockmax;  ///< 0xc [R/W]: Mirror Lock Max register
  volatile uint32_t lockmin;  ///< 0x10 [R/W]: Mirror Lock Min register
  volatile uint32_t hsync_int;  ///< 0x14 [R/W]: HSync Interrupt Configuration register
  volatile uint32_t int_en;  ///< 0x18 [R/W]: Interrupt Enable register
  volatile uint32_t int_st;  ///< 0x1c [R]: Interrupt Pending register
  volatile uint32_t int_cl;  ///< 0x20 [W]: Interrupt Acknowledge register
  volatile uint32_t int_fo;  ///< 0x24 [W]: Interrupt Force register
  volatile uint32_t reserved0[2];
  volatile uint32_t sync_galvo0;  ///< 0x30 [R/W]: SyncGalvo register (Instance 1 of 2)
  volatile uint32_t sync_galvo1;  ///< 0x34 [R/W]: SyncGalvo register (Instance 2 of 2)
  volatile uint32_t sync_wfg0;  ///< 0x38 [R/W]: SyncWFG register (Instance 1 of 2)
  volatile uint32_t sync_wfg1;  ///< 0x3c [R/W]: SyncWFG register (Instance 2 of 2)
  volatile uint32_t galvo_rtol_rise0;  ///< 0x40 [R/W]: GalvoRtoL Rising register (Instance 1 of 2)
  volatile uint32_t galvo_rtol_rise1;  ///< 0x44 [R/W]: GalvoRtoL Rising register (Instance 2 of 2)
  volatile uint32_t galvo_rtol_fall0;  ///< 0x48 [R/W]: GalvoRtoL Falling register (Instance 1 of 2)
  volatile uint32_t galvo_rtol_fall1;  ///< 0x4c [R/W]: GalvoRtoL Falling register (Instance 2 of 2)
  volatile uint32_t galvop_rise0;  ///< 0x50 [R/W]: GalvoP Rising register (Instance 1 of 2)
  volatile uint32_t galvop_rise1;  ///< 0x54 [R/W]: GalvoP Rising register (Instance 2 of 2)
  volatile uint32_t galvop_fall0;  ///< 0x58 [R/W]: GalvoP Falling register (Instance 1 of 2)
  volatile uint32_t galvop_fall1;  ///< 0x5c [R/W]: GalvoP Falling register (Instance 2 of 2)
  volatile uint32_t galvon_rise0;  ///< 0x60 [R/W]: GalvoN Rising register (Instance 1 of 2)
  volatile uint32_t galvon_rise1;  ///< 0x64 [R/W]: GalvoN Rising register (Instance 2 of 2)
  volatile uint32_t galvon_fall0;  ///< 0x68 [R/W]: GalvoN Falling register (Instance 1 of 2)
  volatile uint32_t galvon_fall1;  ///< 0x6c [R/W]: GalvoN Falling register (Instance 2 of 2)
  volatile uint32_t hsync_en0_rise0;  ///< 0x70 [R/W]: HSyncEnable0 Rising register (Instance 1 of 2)
  volatile uint32_t hsync_en0_rise1;  ///< 0x74 [R/W]: HSyncEnable0 Rising register (Instance 2 of 2)
  volatile uint32_t hsync_en0_fall0;  ///< 0x78 [R/W]: HSyncEnable0 Falling register (Instance 1 of 2)
  volatile uint32_t hsync_en0_fall1;  ///< 0x7c [R/W]: HSyncEnable0 Falling register (Instance 2 of 2)
  volatile uint32_t hsync_en1_rise0;  ///< 0x80 [R/W]: HSyncEnable1 Rising register (Instance 1 of 2)
  volatile uint32_t hsync_en1_rise1;  ///< 0x84 [R/W]: HSyncEnable1 Rising register (Instance 2 of 2)
  volatile uint32_t hsync_en1_fall0;  ///< 0x88 [R/W]: HSyncEnable1 Falling register (Instance 1 of 2)
  volatile uint32_t hsync_en1_fall1;  ///< 0x8c [R/W]: HSyncEnable1 Falling register (Instance 2 of 2)
} MIRROR0_REGS_t;

//====================================================================
//
//Register File: Mirror Control (Instance 2 of 2) (mirror1)
//
//====================================================================

typedef struct MIRROR1_REGS_s
{
  volatile uint32_t mcfg1;  ///< 0x0 [R/W]: Mirror Configuration 1 register
  volatile uint32_t mcfg2;  ///< 0x4 [R/W]: Mirror Configuration 2 register
  volatile uint32_t status;  ///< 0x8 [R]: Mirror Status register
  volatile uint32_t lockmax;  ///< 0xc [R/W]: Mirror Lock Max register
  volatile uint32_t lockmin;  ///< 0x10 [R/W]: Mirror Lock Min register
  volatile uint32_t hsync_int;  ///< 0x14 [R/W]: HSync Interrupt Configuration register
  volatile uint32_t int_en;  ///< 0x18 [R/W]: Interrupt Enable register
  volatile uint32_t int_st;  ///< 0x1c [R]: Interrupt Pending register
  volatile uint32_t int_cl;  ///< 0x20 [W]: Interrupt Acknowledge register
  volatile uint32_t int_fo;  ///< 0x24 [W]: Interrupt Force register
  volatile uint32_t reserved0[2];
  volatile uint32_t sync_galvo0;  ///< 0x30 [R/W]: SyncGalvo register (Instance 1 of 2)
  volatile uint32_t sync_galvo1;  ///< 0x34 [R/W]: SyncGalvo register (Instance 2 of 2)
  volatile uint32_t sync_wfg0;  ///< 0x38 [R/W]: SyncWFG register (Instance 1 of 2)
  volatile uint32_t sync_wfg1;  ///< 0x3c [R/W]: SyncWFG register (Instance 2 of 2)
  volatile uint32_t galvo_rtol_rise0;  ///< 0x40 [R/W]: GalvoRtoL Rising register (Instance 1 of 2)
  volatile uint32_t galvo_rtol_rise1;  ///< 0x44 [R/W]: GalvoRtoL Rising register (Instance 2 of 2)
  volatile uint32_t galvo_rtol_fall0;  ///< 0x48 [R/W]: GalvoRtoL Falling register (Instance 1 of 2)
  volatile uint32_t galvo_rtol_fall1;  ///< 0x4c [R/W]: GalvoRtoL Falling register (Instance 2 of 2)
  volatile uint32_t galvop_rise0;  ///< 0x50 [R/W]: GalvoP Rising register (Instance 1 of 2)
  volatile uint32_t galvop_rise1;  ///< 0x54 [R/W]: GalvoP Rising register (Instance 2 of 2)
  volatile uint32_t galvop_fall0;  ///< 0x58 [R/W]: GalvoP Falling register (Instance 1 of 2)
  volatile uint32_t galvop_fall1;  ///< 0x5c [R/W]: GalvoP Falling register (Instance 2 of 2)
  volatile uint32_t galvon_rise0;  ///< 0x60 [R/W]: GalvoN Rising register (Instance 1 of 2)
  volatile uint32_t galvon_rise1;  ///< 0x64 [R/W]: GalvoN Rising register (Instance 2 of 2)
  volatile uint32_t galvon_fall0;  ///< 0x68 [R/W]: GalvoN Falling register (Instance 1 of 2)
  volatile uint32_t galvon_fall1;  ///< 0x6c [R/W]: GalvoN Falling register (Instance 2 of 2)
  volatile uint32_t hsync_en0_rise0;  ///< 0x70 [R/W]: HSyncEnable0 Rising register (Instance 1 of 2)
  volatile uint32_t hsync_en0_rise1;  ///< 0x74 [R/W]: HSyncEnable0 Rising register (Instance 2 of 2)
  volatile uint32_t hsync_en0_fall0;  ///< 0x78 [R/W]: HSyncEnable0 Falling register (Instance 1 of 2)
  volatile uint32_t hsync_en0_fall1;  ///< 0x7c [R/W]: HSyncEnable0 Falling register (Instance 2 of 2)
  volatile uint32_t hsync_en1_rise0;  ///< 0x80 [R/W]: HSyncEnable1 Rising register (Instance 1 of 2)
  volatile uint32_t hsync_en1_rise1;  ///< 0x84 [R/W]: HSyncEnable1 Rising register (Instance 2 of 2)
  volatile uint32_t hsync_en1_fall0;  ///< 0x88 [R/W]: HSyncEnable1 Falling register (Instance 1 of 2)
  volatile uint32_t hsync_en1_fall1;  ///< 0x8c [R/W]: HSyncEnable1 Falling register (Instance 2 of 2)
} MIRROR1_REGS_t;

//====================================================================
//
//Register File: Waverform Generator Control (Instance 1 of 4) (wfg0)
//
//====================================================================

typedef struct WFG0_REGS_s
{
  volatile uint32_t scfg;  ///< 0x0 [R/W]: Waveform Generator Configuration register
  volatile uint32_t reserved0[3];
  volatile uint32_t period0;  ///< 0x10 [R/W]: Waveform Control register (Instance 1 of 10)
  volatile uint32_t period1;  ///< 0x14 [R/W]: Waveform Control register (Instance 2 of 10)
  volatile uint32_t period2;  ///< 0x18 [R/W]: Waveform Control register (Instance 3 of 10)
  volatile uint32_t period3;  ///< 0x1c [R/W]: Waveform Control register (Instance 4 of 10)
  volatile uint32_t period4;  ///< 0x20 [R/W]: Waveform Control register (Instance 5 of 10)
  volatile uint32_t period5;  ///< 0x24 [R/W]: Waveform Control register (Instance 6 of 10)
  volatile uint32_t period6;  ///< 0x28 [R/W]: Waveform Control register (Instance 7 of 10)
  volatile uint32_t period7;  ///< 0x2c [R/W]: Waveform Control register (Instance 8 of 10)
  volatile uint32_t period8;  ///< 0x30 [R/W]: Waveform Control register (Instance 9 of 10)
  volatile uint32_t period9;  ///< 0x34 [R/W]: Waveform Control register (Instance 10 of 10)
  volatile uint32_t reserved1[2];
  volatile uint32_t action10;  ///< 0x40 [R/W]: Waveform Action 1 register (Instance 1 of 10)
  volatile uint32_t action11;  ///< 0x44 [R/W]: Waveform Action 1 register (Instance 2 of 10)
  volatile uint32_t action12;  ///< 0x48 [R/W]: Waveform Action 1 register (Instance 3 of 10)
  volatile uint32_t action13;  ///< 0x4c [R/W]: Waveform Action 1 register (Instance 4 of 10)
  volatile uint32_t action14;  ///< 0x50 [R/W]: Waveform Action 1 register (Instance 5 of 10)
  volatile uint32_t action15;  ///< 0x54 [R/W]: Waveform Action 1 register (Instance 6 of 10)
  volatile uint32_t action16;  ///< 0x58 [R/W]: Waveform Action 1 register (Instance 7 of 10)
  volatile uint32_t action17;  ///< 0x5c [R/W]: Waveform Action 1 register (Instance 8 of 10)
  volatile uint32_t action18;  ///< 0x60 [R/W]: Waveform Action 1 register (Instance 9 of 10)
  volatile uint32_t action19;  ///< 0x64 [R/W]: Waveform Action 1 register (Instance 10 of 10)
  volatile uint32_t reserved2[2];
  volatile uint32_t action20;  ///< 0x70 [R/W]: Waveform Action 2 register (Instance 1 of 10)
  volatile uint32_t action21;  ///< 0x74 [R/W]: Waveform Action 2 register (Instance 2 of 10)
  volatile uint32_t action22;  ///< 0x78 [R/W]: Waveform Action 2 register (Instance 3 of 10)
  volatile uint32_t action23;  ///< 0x7c [R/W]: Waveform Action 2 register (Instance 4 of 10)
  volatile uint32_t action24;  ///< 0x80 [R/W]: Waveform Action 2 register (Instance 5 of 10)
  volatile uint32_t action25;  ///< 0x84 [R/W]: Waveform Action 2 register (Instance 6 of 10)
  volatile uint32_t action26;  ///< 0x88 [R/W]: Waveform Action 2 register (Instance 7 of 10)
  volatile uint32_t action27;  ///< 0x8c [R/W]: Waveform Action 2 register (Instance 8 of 10)
  volatile uint32_t action28;  ///< 0x90 [R/W]: Waveform Action 2 register (Instance 9 of 10)
  volatile uint32_t action29;  ///< 0x94 [R/W]: Waveform Action 2 register (Instance 10 of 10)
} WFG0_REGS_t;

//====================================================================
//
//Register File: Waverform Generator Control (Instance 2 of 4) (wfg1)
//
//====================================================================

typedef struct WFG1_REGS_s
{
  volatile uint32_t scfg;  ///< 0x0 [R/W]: Waveform Generator Configuration register
  volatile uint32_t reserved0[3];
  volatile uint32_t period0;  ///< 0x10 [R/W]: Waveform Control register (Instance 1 of 10)
  volatile uint32_t period1;  ///< 0x14 [R/W]: Waveform Control register (Instance 2 of 10)
  volatile uint32_t period2;  ///< 0x18 [R/W]: Waveform Control register (Instance 3 of 10)
  volatile uint32_t period3;  ///< 0x1c [R/W]: Waveform Control register (Instance 4 of 10)
  volatile uint32_t period4;  ///< 0x20 [R/W]: Waveform Control register (Instance 5 of 10)
  volatile uint32_t period5;  ///< 0x24 [R/W]: Waveform Control register (Instance 6 of 10)
  volatile uint32_t period6;  ///< 0x28 [R/W]: Waveform Control register (Instance 7 of 10)
  volatile uint32_t period7;  ///< 0x2c [R/W]: Waveform Control register (Instance 8 of 10)
  volatile uint32_t period8;  ///< 0x30 [R/W]: Waveform Control register (Instance 9 of 10)
  volatile uint32_t period9;  ///< 0x34 [R/W]: Waveform Control register (Instance 10 of 10)
  volatile uint32_t reserved1[2];
  volatile uint32_t action10;  ///< 0x40 [R/W]: Waveform Action 1 register (Instance 1 of 10)
  volatile uint32_t action11;  ///< 0x44 [R/W]: Waveform Action 1 register (Instance 2 of 10)
  volatile uint32_t action12;  ///< 0x48 [R/W]: Waveform Action 1 register (Instance 3 of 10)
  volatile uint32_t action13;  ///< 0x4c [R/W]: Waveform Action 1 register (Instance 4 of 10)
  volatile uint32_t action14;  ///< 0x50 [R/W]: Waveform Action 1 register (Instance 5 of 10)
  volatile uint32_t action15;  ///< 0x54 [R/W]: Waveform Action 1 register (Instance 6 of 10)
  volatile uint32_t action16;  ///< 0x58 [R/W]: Waveform Action 1 register (Instance 7 of 10)
  volatile uint32_t action17;  ///< 0x5c [R/W]: Waveform Action 1 register (Instance 8 of 10)
  volatile uint32_t action18;  ///< 0x60 [R/W]: Waveform Action 1 register (Instance 9 of 10)
  volatile uint32_t action19;  ///< 0x64 [R/W]: Waveform Action 1 register (Instance 10 of 10)
  volatile uint32_t reserved2[2];
  volatile uint32_t action20;  ///< 0x70 [R/W]: Waveform Action 2 register (Instance 1 of 10)
  volatile uint32_t action21;  ///< 0x74 [R/W]: Waveform Action 2 register (Instance 2 of 10)
  volatile uint32_t action22;  ///< 0x78 [R/W]: Waveform Action 2 register (Instance 3 of 10)
  volatile uint32_t action23;  ///< 0x7c [R/W]: Waveform Action 2 register (Instance 4 of 10)
  volatile uint32_t action24;  ///< 0x80 [R/W]: Waveform Action 2 register (Instance 5 of 10)
  volatile uint32_t action25;  ///< 0x84 [R/W]: Waveform Action 2 register (Instance 6 of 10)
  volatile uint32_t action26;  ///< 0x88 [R/W]: Waveform Action 2 register (Instance 7 of 10)
  volatile uint32_t action27;  ///< 0x8c [R/W]: Waveform Action 2 register (Instance 8 of 10)
  volatile uint32_t action28;  ///< 0x90 [R/W]: Waveform Action 2 register (Instance 9 of 10)
  volatile uint32_t action29;  ///< 0x94 [R/W]: Waveform Action 2 register (Instance 10 of 10)
} WFG1_REGS_t;

//====================================================================
//
//Register File: Waverform Generator Control (Instance 3 of 4) (wfg2)
//
//====================================================================

typedef struct WFG2_REGS_s
{
  volatile uint32_t scfg;  ///< 0x0 [R/W]: Waveform Generator Configuration register
  volatile uint32_t reserved0[3];
  volatile uint32_t period0;  ///< 0x10 [R/W]: Waveform Control register (Instance 1 of 10)
  volatile uint32_t period1;  ///< 0x14 [R/W]: Waveform Control register (Instance 2 of 10)
  volatile uint32_t period2;  ///< 0x18 [R/W]: Waveform Control register (Instance 3 of 10)
  volatile uint32_t period3;  ///< 0x1c [R/W]: Waveform Control register (Instance 4 of 10)
  volatile uint32_t period4;  ///< 0x20 [R/W]: Waveform Control register (Instance 5 of 10)
  volatile uint32_t period5;  ///< 0x24 [R/W]: Waveform Control register (Instance 6 of 10)
  volatile uint32_t period6;  ///< 0x28 [R/W]: Waveform Control register (Instance 7 of 10)
  volatile uint32_t period7;  ///< 0x2c [R/W]: Waveform Control register (Instance 8 of 10)
  volatile uint32_t period8;  ///< 0x30 [R/W]: Waveform Control register (Instance 9 of 10)
  volatile uint32_t period9;  ///< 0x34 [R/W]: Waveform Control register (Instance 10 of 10)
  volatile uint32_t reserved1[2];
  volatile uint32_t action10;  ///< 0x40 [R/W]: Waveform Action 1 register (Instance 1 of 10)
  volatile uint32_t action11;  ///< 0x44 [R/W]: Waveform Action 1 register (Instance 2 of 10)
  volatile uint32_t action12;  ///< 0x48 [R/W]: Waveform Action 1 register (Instance 3 of 10)
  volatile uint32_t action13;  ///< 0x4c [R/W]: Waveform Action 1 register (Instance 4 of 10)
  volatile uint32_t action14;  ///< 0x50 [R/W]: Waveform Action 1 register (Instance 5 of 10)
  volatile uint32_t action15;  ///< 0x54 [R/W]: Waveform Action 1 register (Instance 6 of 10)
  volatile uint32_t action16;  ///< 0x58 [R/W]: Waveform Action 1 register (Instance 7 of 10)
  volatile uint32_t action17;  ///< 0x5c [R/W]: Waveform Action 1 register (Instance 8 of 10)
  volatile uint32_t action18;  ///< 0x60 [R/W]: Waveform Action 1 register (Instance 9 of 10)
  volatile uint32_t action19;  ///< 0x64 [R/W]: Waveform Action 1 register (Instance 10 of 10)
  volatile uint32_t reserved2[2];
  volatile uint32_t action20;  ///< 0x70 [R/W]: Waveform Action 2 register (Instance 1 of 10)
  volatile uint32_t action21;  ///< 0x74 [R/W]: Waveform Action 2 register (Instance 2 of 10)
  volatile uint32_t action22;  ///< 0x78 [R/W]: Waveform Action 2 register (Instance 3 of 10)
  volatile uint32_t action23;  ///< 0x7c [R/W]: Waveform Action 2 register (Instance 4 of 10)
  volatile uint32_t action24;  ///< 0x80 [R/W]: Waveform Action 2 register (Instance 5 of 10)
  volatile uint32_t action25;  ///< 0x84 [R/W]: Waveform Action 2 register (Instance 6 of 10)
  volatile uint32_t action26;  ///< 0x88 [R/W]: Waveform Action 2 register (Instance 7 of 10)
  volatile uint32_t action27;  ///< 0x8c [R/W]: Waveform Action 2 register (Instance 8 of 10)
  volatile uint32_t action28;  ///< 0x90 [R/W]: Waveform Action 2 register (Instance 9 of 10)
  volatile uint32_t action29;  ///< 0x94 [R/W]: Waveform Action 2 register (Instance 10 of 10)
} WFG2_REGS_t;

//====================================================================
//
//Register File: Waverform Generator Control (Instance 4 of 4) (wfg3)
//
//====================================================================

typedef struct WFG3_REGS_s
{
  volatile uint32_t scfg;  ///< 0x0 [R/W]: Waveform Generator Configuration register
  volatile uint32_t reserved0[3];
  volatile uint32_t period0;  ///< 0x10 [R/W]: Waveform Control register (Instance 1 of 10)
  volatile uint32_t period1;  ///< 0x14 [R/W]: Waveform Control register (Instance 2 of 10)
  volatile uint32_t period2;  ///< 0x18 [R/W]: Waveform Control register (Instance 3 of 10)
  volatile uint32_t period3;  ///< 0x1c [R/W]: Waveform Control register (Instance 4 of 10)
  volatile uint32_t period4;  ///< 0x20 [R/W]: Waveform Control register (Instance 5 of 10)
  volatile uint32_t period5;  ///< 0x24 [R/W]: Waveform Control register (Instance 6 of 10)
  volatile uint32_t period6;  ///< 0x28 [R/W]: Waveform Control register (Instance 7 of 10)
  volatile uint32_t period7;  ///< 0x2c [R/W]: Waveform Control register (Instance 8 of 10)
  volatile uint32_t period8;  ///< 0x30 [R/W]: Waveform Control register (Instance 9 of 10)
  volatile uint32_t period9;  ///< 0x34 [R/W]: Waveform Control register (Instance 10 of 10)
  volatile uint32_t reserved1[2];
  volatile uint32_t action10;  ///< 0x40 [R/W]: Waveform Action 1 register (Instance 1 of 10)
  volatile uint32_t action11;  ///< 0x44 [R/W]: Waveform Action 1 register (Instance 2 of 10)
  volatile uint32_t action12;  ///< 0x48 [R/W]: Waveform Action 1 register (Instance 3 of 10)
  volatile uint32_t action13;  ///< 0x4c [R/W]: Waveform Action 1 register (Instance 4 of 10)
  volatile uint32_t action14;  ///< 0x50 [R/W]: Waveform Action 1 register (Instance 5 of 10)
  volatile uint32_t action15;  ///< 0x54 [R/W]: Waveform Action 1 register (Instance 6 of 10)
  volatile uint32_t action16;  ///< 0x58 [R/W]: Waveform Action 1 register (Instance 7 of 10)
  volatile uint32_t action17;  ///< 0x5c [R/W]: Waveform Action 1 register (Instance 8 of 10)
  volatile uint32_t action18;  ///< 0x60 [R/W]: Waveform Action 1 register (Instance 9 of 10)
  volatile uint32_t action19;  ///< 0x64 [R/W]: Waveform Action 1 register (Instance 10 of 10)
  volatile uint32_t reserved2[2];
  volatile uint32_t action20;  ///< 0x70 [R/W]: Waveform Action 2 register (Instance 1 of 10)
  volatile uint32_t action21;  ///< 0x74 [R/W]: Waveform Action 2 register (Instance 2 of 10)
  volatile uint32_t action22;  ///< 0x78 [R/W]: Waveform Action 2 register (Instance 3 of 10)
  volatile uint32_t action23;  ///< 0x7c [R/W]: Waveform Action 2 register (Instance 4 of 10)
  volatile uint32_t action24;  ///< 0x80 [R/W]: Waveform Action 2 register (Instance 5 of 10)
  volatile uint32_t action25;  ///< 0x84 [R/W]: Waveform Action 2 register (Instance 6 of 10)
  volatile uint32_t action26;  ///< 0x88 [R/W]: Waveform Action 2 register (Instance 7 of 10)
  volatile uint32_t action27;  ///< 0x8c [R/W]: Waveform Action 2 register (Instance 8 of 10)
  volatile uint32_t action28;  ///< 0x90 [R/W]: Waveform Action 2 register (Instance 9 of 10)
  volatile uint32_t action29;  ///< 0x94 [R/W]: Waveform Action 2 register (Instance 10 of 10)
} WFG3_REGS_t;

//====================================================================
//
//Register File: Waverform Selector Control (wfs)
//
//====================================================================

typedef struct WFS_REGS_s
{
  volatile uint32_t out_sel;  ///< 0x0 [R/W]: Waveform Output Select register
  volatile uint32_t reserved0[3];
  volatile uint32_t data_sel;  ///< 0x10 [R/W]: Waveform Select Data register
  volatile uint32_t reserved1[11];
  volatile uint32_t wave_pol;  ///< 0x40 [R/W]: Waveform Select Polarity register
  volatile uint32_t reserved2[47];
  volatile uint32_t wave_ld_sel0;  ///< 0x100 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 1 of 12)
  volatile uint32_t reserved3[3];
  volatile uint32_t wave_ld_sel1;  ///< 0x110 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 2 of 12)
  volatile uint32_t reserved4[3];
  volatile uint32_t wave_ld_sel2;  ///< 0x120 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 3 of 12)
  volatile uint32_t reserved5[3];
  volatile uint32_t wave_ld_sel3;  ///< 0x130 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 4 of 12)
  volatile uint32_t reserved6[3];
  volatile uint32_t wave_ld_sel4;  ///< 0x140 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 5 of 12)
  volatile uint32_t reserved7[3];
  volatile uint32_t wave_ld_sel5;  ///< 0x150 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 6 of 12)
  volatile uint32_t reserved8[3];
  volatile uint32_t wave_ld_sel6;  ///< 0x160 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 7 of 12)
  volatile uint32_t reserved9[3];
  volatile uint32_t wave_ld_sel7;  ///< 0x170 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 8 of 12)
  volatile uint32_t reserved10[3];
  volatile uint32_t wave_ld_sel8;  ///< 0x180 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 9 of 12)
  volatile uint32_t reserved11[3];
  volatile uint32_t wave_ld_sel9;  ///< 0x190 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 10 of 12)
  volatile uint32_t reserved12[3];
  volatile uint32_t wave_ld_sel10;  ///< 0x1a0 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 11 of 12)
  volatile uint32_t reserved13[3];
  volatile uint32_t wave_ld_sel11;  ///< 0x1b0 [R/W]: Waveform Select LaserDiode_CTRL register (Instance 12 of 12)
  volatile uint32_t reserved14[19];
  volatile uint32_t wave_frc_sel0;  ///< 0x200 [R/W]: Waveform Select ForceLaserOn register (Instance 1 of 8)
  volatile uint32_t reserved15[3];
  volatile uint32_t wave_frc_sel1;  ///< 0x210 [R/W]: Waveform Select ForceLaserOn register (Instance 2 of 8)
  volatile uint32_t reserved16[3];
  volatile uint32_t wave_frc_sel2;  ///< 0x220 [R/W]: Waveform Select ForceLaserOn register (Instance 3 of 8)
  volatile uint32_t reserved17[3];
  volatile uint32_t wave_frc_sel3;  ///< 0x230 [R/W]: Waveform Select ForceLaserOn register (Instance 4 of 8)
  volatile uint32_t reserved18[3];
  volatile uint32_t wave_frc_sel4;  ///< 0x240 [R/W]: Waveform Select ForceLaserOn register (Instance 5 of 8)
  volatile uint32_t reserved19[3];
  volatile uint32_t wave_frc_sel5;  ///< 0x250 [R/W]: Waveform Select ForceLaserOn register (Instance 6 of 8)
  volatile uint32_t reserved20[3];
  volatile uint32_t wave_frc_sel6;  ///< 0x260 [R/W]: Waveform Select ForceLaserOn register (Instance 7 of 8)
  volatile uint32_t reserved21[3];
  volatile uint32_t wave_frc_sel7;  ///< 0x270 [R/W]: Waveform Select ForceLaserOn register (Instance 8 of 8)
  volatile uint32_t reserved22[35];
  volatile uint32_t wave_en_sel0;  ///< 0x300 [R/W]: Waveform Select EnableVDO register (Instance 1 of 8)
  volatile uint32_t reserved23[3];
  volatile uint32_t wave_en_sel1;  ///< 0x310 [R/W]: Waveform Select EnableVDO register (Instance 2 of 8)
  volatile uint32_t reserved24[3];
  volatile uint32_t wave_en_sel2;  ///< 0x320 [R/W]: Waveform Select EnableVDO register (Instance 3 of 8)
  volatile uint32_t reserved25[3];
  volatile uint32_t wave_en_sel3;  ///< 0x330 [R/W]: Waveform Select EnableVDO register (Instance 4 of 8)
  volatile uint32_t reserved26[3];
  volatile uint32_t wave_en_sel4;  ///< 0x340 [R/W]: Waveform Select EnableVDO register (Instance 5 of 8)
  volatile uint32_t reserved27[3];
  volatile uint32_t wave_en_sel5;  ///< 0x350 [R/W]: Waveform Select EnableVDO register (Instance 6 of 8)
  volatile uint32_t reserved28[3];
  volatile uint32_t wave_en_sel6;  ///< 0x360 [R/W]: Waveform Select EnableVDO register (Instance 7 of 8)
  volatile uint32_t reserved29[3];
  volatile uint32_t wave_en_sel7;  ///< 0x370 [R/W]: Waveform Select EnableVDO register (Instance 8 of 8)
} WFS_REGS_t;

#endif // LSCAN
