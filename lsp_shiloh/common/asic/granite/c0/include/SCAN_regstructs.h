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
 * \file SCAN_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SCAN_REGSTRUCTS_H_
#define _SCAN_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SCAN (SCAN)
/** \brief Top-level register file for SCAN superblock*/
//
//====================================================================

typedef struct SCAN_REGS_s
{
  volatile uint32_t SCFG1;  ///< 0x0 [R/W]: Scan Config1
  volatile uint32_t SCFG2;  ///< 0x4 [R/W]: Scan Config2
  volatile uint32_t SCFG3;  ///< 0x8 [R/W]: Scan Config3
  volatile uint32_t SCTRL;  ///< 0xc [R/W]: Scan Control
  volatile uint32_t SCMD;  ///< 0x10 [R/W]: Scan Command
  volatile uint32_t SSTAT;  ///< 0x14 [R]: Scan Status
  volatile uint32_t SIACK;  ///< 0x18 [W]: Scan Interrupt Acknowledge
  volatile uint32_t SCANX;  ///< 0x1c [R/W]: Scan Pixels/Line
  volatile uint32_t SCYCLE;  ///< 0x20 [R/W]: Scan Cycle
  volatile uint32_t STCFG1;  ///< 0x24 [R/W]: Scan Start Config1
  volatile uint32_t STCFG2;  ///< 0x28 [R/W]: Scan Start Config2
  volatile uint32_t STCFG3;  ///< 0x2c [R/W]: Scan Start Config3
  volatile uint32_t STCFG4;  ///< 0x30 [R/W]: Scan Start Config4
  volatile uint32_t CCFG;  ///< 0x34 [R/W]: Clamp Config
  volatile uint32_t SCLK1;  ///< 0x38 [R/W]: Sensor Clocks 1
  volatile uint32_t SCLK2;  ///< 0x3c [R/W]: Sensor Clocks 2
  volatile uint32_t SCLK3;  ///< 0x40 [R/W]: Sensor Clocks 3
  volatile uint32_t SCLK4;  ///< 0x44 [R/W]: Sensor Clocks 4
  volatile uint32_t SCLK5;  ///< 0x48 [R/W]: Sensor Clocks 5
  volatile uint32_t SCLK6;  ///< 0x4c [R/W]: Sensor Clocks 6
  volatile uint32_t ACLK1;  ///< 0x50 [R/W]: AFE Clocks 1
  volatile uint32_t ACLK2;  ///< 0x54 [R/W]: AFE Clocks 2
  volatile uint32_t ACLK3;  ///< 0x58 [R/W]: AFE Clocks 3
  volatile uint32_t ACLK4;  ///< 0x5c [R/W]: AFE Clocks 4
  volatile uint32_t ACLK5;  ///< 0x60 [R/W]: AFE Clocks 5
  volatile uint32_t ACLK6;  ///< 0x64 [R/W]: AFE Clocks 6
  volatile uint32_t LDATA10;  ///< 0x68 [R/W]: AFE Load Data 1 (Instance 1 of 2)
  volatile uint32_t LDATA11;  ///< 0x6c [R/W]: AFE Load Data 1 (Instance 2 of 2)
  volatile uint32_t LDATA20;  ///< 0x70 [R/W]: AFE Load Data 2 (Instance 1 of 2)
  volatile uint32_t LDATA21;  ///< 0x74 [R/W]: AFE Load Data 2 (Instance 2 of 2)
  volatile uint32_t LDATA30;  ///< 0x78 [R/W]: AFE Load Data 3 (Instance 1 of 2)
  volatile uint32_t LDATA31;  ///< 0x7c [R/W]: AFE Load Data 3 (Instance 2 of 2)
  volatile uint32_t LDATA40;  ///< 0x80 [R/W]: AFE Load Data 4 (Instance 1 of 2)
  volatile uint32_t LDATA41;  ///< 0x84 [R/W]: AFE Load Data 4 (Instance 2 of 2)
  volatile uint32_t LDATA50;  ///< 0x88 [R/W]: AFE Load Data 5 (Instance 1 of 2)
  volatile uint32_t LDATA51;  ///< 0x8c [R/W]: AFE Load Data 5 (Instance 2 of 2)
  volatile uint32_t LDATA60;  ///< 0x90 [R/W]: AFE Load Data 6 (Instance 1 of 2)
  volatile uint32_t LDATA61;  ///< 0x94 [R/W]: AFE Load Data 6 (Instance 2 of 2)
  volatile uint32_t PSEQ1;  ///< 0x98 [R/W]: AFE Pixel Sequence 1
  volatile uint32_t PSEQ2;  ///< 0x9c [R/W]: AFE Pixel Sequence 2
  volatile uint32_t BPWM;  ///< 0xa0 [R/W]: Bulb PWM
  volatile uint32_t MCFG;  ///< 0xa4 [R/W]: Motor Config
  volatile uint32_t MotorSyncMin;  ///< 0xa8 [R/W]: Min scanclk cycles between Motor pulses in sync every mode
  volatile uint32_t AFEPC;  ///< 0xac [R/W]: AFE Program Control
  volatile uint32_t reserved0;
  volatile uint32_t CFGARB;  ///< 0xb4 [R/W]: CBI Arbitration Config
  volatile uint32_t LED00;  ///< 0xb8 [R/W]: LED0 Gate (Instance 1 of 2)
  volatile uint32_t LED01;  ///< 0xbc [R/W]: LED0 Gate (Instance 2 of 2)
  volatile uint32_t LED10;  ///< 0xc0 [R/W]: LED1 Gate (Instance 1 of 2)
  volatile uint32_t LED11;  ///< 0xc4 [R/W]: LED1 Gate (Instance 2 of 2)
  volatile uint32_t LED20;  ///< 0xc8 [R/W]: LED2 Gate (Instance 1 of 2)
  volatile uint32_t LED21;  ///< 0xcc [R/W]: LED2 Gate (Instance 2 of 2)
  volatile uint32_t LPWMConfig0;  ///< 0xd0 [R/W]: LED PWM Period Config (Instance 1 of 2)
  volatile uint32_t LPWMConfig1;  ///< 0xd4 [R/W]: LED PWM Period Config (Instance 2 of 2)
  volatile uint32_t LPWM00;  ///< 0xd8 [R/W]: LED0 PWM Pulse Config (Instance 1 of 2)
  volatile uint32_t LPWM01;  ///< 0xdc [R/W]: LED0 PWM Pulse Config (Instance 2 of 2)
  volatile uint32_t LPWM10;  ///< 0xe0 [R/W]: LED1 PWM Pulse Config (Instance 1 of 2)
  volatile uint32_t LPWM11;  ///< 0xe4 [R/W]: LED1 PWM Pulse Config (Instance 2 of 2)
  volatile uint32_t LPWM20;  ///< 0xe8 [R/W]: LED2 PWM Pulse Config (Instance 1 of 2)
  volatile uint32_t LPWM21;  ///< 0xec [R/W]: LED2 PWM Pulse Config (Instance 2 of 2)
} SCAN_REGS_t;

//====================================================================
//
//Register File: LVDS AFE Interface (LVDSAFE)
/** \brief Top-level register file for the LVDS AFE Control block*/
//
//====================================================================

typedef struct LVDSAFE_REGS_s
{
  volatile uint32_t rb4LVDSAFEControl;  ///< 0x0 [R/W]: LVDS AFE Control Register
  volatile uint32_t rb4LVDSAFEControl2;  ///< 0x4 [R/W]: LVDS AFE Control Register 2
  volatile uint32_t rb4LVDSAFEControl3;  ///< 0x8 [R/W]: LVDS AFE Control Register 3
  volatile uint32_t rb4LVDSAFEAltPxl;  ///< 0xc [R/W]: LVDS AFE Alternate Pixel Encodings Register
  volatile uint32_t rb4LVDSAFEDataLineSync;  ///< 0x10 [R/W]: LVDS AFE Data Line Sync Register
  volatile uint32_t rb4LVDSAFEClk;  ///< 0x14 [R/W]: LVDS AFE Clock Register
  volatile uint32_t rb4LVDSAFECh0LineCount;  ///< 0x18 [W]: LVDS AFE Channel 0 Line Count Register
  volatile uint32_t rb4LVDSAFECh0LineCountObs;  ///< 0x1c [R]: LVDS AFE Channel 0 Line Count Observation Register
  volatile uint32_t rb4LVDSAFECh1LineCount;  ///< 0x20 [W]: LVDS AFE Channel 1 Line Count Register
  volatile uint32_t rb4LVDSAFECh1LineCountObs;  ///< 0x24 [R]: LVDS AFE Channel 1 Line Count Observation Register
  volatile uint32_t rb4LVDSAFECh2LineCount;  ///< 0x28 [W]: LVDS AFE Channel 2 Line Count Register
  volatile uint32_t rb4LVDSAFECh2LineCountObs;  ///< 0x2c [R]: LVDS AFE Channel 2 Line Count Observation Register
  volatile uint32_t rb4LVDSAFEIrqEnb;  ///< 0x30 [R/W]: LVDS AFE IRQ Enable Register
  volatile uint32_t rb4LVDSAFEIrqPnd;  ///< 0x34 [R]: LVDS AFE IRQ Pending Register
  volatile uint32_t rb4LVDSAFEIrqAck;  ///< 0x38 [W]: LVDS AFE IRQ Acknowledge Register
  volatile uint32_t rb4LVDSAFEIrqForce;  ///< 0x3c [W]: LVDS AFE IRQ Force Register
} LVDSAFE_REGS_t;

//====================================================================
//
//Register File: Camera Capture Interface (VCAP)
/** \brief Top-level register file for the VCAP (Camera Capture) block*/
//
//====================================================================

typedef struct VCAP_REGS_s
{
  volatile uint32_t rb4VCapControl;  ///< 0x0 [R/W]: VCap Control Register
  volatile uint32_t rb4VCapCounter;  ///< 0x4 [W]: VCap Counter Register
  volatile uint32_t rb4VCapCounterObs;  ///< 0x8 [R]: VCap Counter Observation Register
  volatile uint32_t rb4VCapPixelObs;  ///< 0xc [R]: VCap Pixel Observation Register
  volatile uint32_t rb4VCapLineCount;  ///< 0x10 [W]: VCap Line Count Register
  volatile uint32_t rb4VCapLineCountObs;  ///< 0x14 [R]: VCap Line Count Observation Register
  volatile uint32_t rb4VCapIrqEnb;  ///< 0x18 [R/W]: VCap IRQ Enable Register
  volatile uint32_t rb4VCapIrqPnd;  ///< 0x1c [R]: VCap IRQ Pending Register
  volatile uint32_t rb4VCapIrqAck;  ///< 0x20 [W]: VCap IRQ Acknowledge Register
  volatile uint32_t rb4VCapIrqForce;  ///< 0x24 [W]: VCap IRQ Force Register
  volatile uint32_t rb4VCapClk;  ///< 0x28 [R/W]: VCAP Clock Register
} VCAP_REGS_t;

#endif // SCAN
