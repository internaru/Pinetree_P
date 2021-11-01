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
 * \file adc_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ADC_REGS_REGSTRUCTS_H_
#define _ADC_REGS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: ADC registers (adc_regs)
/** \brief Registers for ADC Control which provides for programmable ADC communications and assist functions.*/
//
//====================================================================

typedef struct ADC_REGS_REGS_s
{
  volatile uint32_t SARADC_Control;  ///< 0x0 [R/W]: SARADC Control Register
  volatile uint32_t ADC_Control;  ///< 0x4 [R/W]: ADC Control Register
  volatile uint32_t ContTimebaseConfig;  ///< 0x8 [R/W]: Continuous-mode Timebase Configuration Register
  volatile uint32_t reserved0;
  volatile uint32_t GpTimestampConfig0;  ///< 0x10 [R/W]: General-purpose Timestamp Configuration Register (Instance 1 of 2)
  volatile uint32_t GpTimestampStatus0;  ///< 0x14 [R]: General-purpose Timestamp Status Register (Instance 1 of 2)
  volatile uint32_t GpTimestampConfig1;  ///< 0x18 [R/W]: General-purpose Timestamp Configuration Register (Instance 2 of 2)
  volatile uint32_t GpTimestampStatus1;  ///< 0x1c [R]: General-purpose Timestamp Status Register (Instance 2 of 2)
  volatile uint32_t TriggerConfig0;  ///< 0x20 [R/W]: Trigger Configuration Register (Instance 1 of 15)
  volatile uint32_t TrTimestampStatus0;  ///< 0x24 [R]: Trigger Timestamp Status Register (Instance 1 of 15)
  volatile uint32_t TriggerConfig1;  ///< 0x28 [R/W]: Trigger Configuration Register (Instance 2 of 15)
  volatile uint32_t TrTimestampStatus1;  ///< 0x2c [R]: Trigger Timestamp Status Register (Instance 2 of 15)
  volatile uint32_t TriggerConfig2;  ///< 0x30 [R/W]: Trigger Configuration Register (Instance 3 of 15)
  volatile uint32_t TrTimestampStatus2;  ///< 0x34 [R]: Trigger Timestamp Status Register (Instance 3 of 15)
  volatile uint32_t TriggerConfig3;  ///< 0x38 [R/W]: Trigger Configuration Register (Instance 4 of 15)
  volatile uint32_t TrTimestampStatus3;  ///< 0x3c [R]: Trigger Timestamp Status Register (Instance 4 of 15)
  volatile uint32_t TriggerConfig4;  ///< 0x40 [R/W]: Trigger Configuration Register (Instance 5 of 15)
  volatile uint32_t TrTimestampStatus4;  ///< 0x44 [R]: Trigger Timestamp Status Register (Instance 5 of 15)
  volatile uint32_t TriggerConfig5;  ///< 0x48 [R/W]: Trigger Configuration Register (Instance 6 of 15)
  volatile uint32_t TrTimestampStatus5;  ///< 0x4c [R]: Trigger Timestamp Status Register (Instance 6 of 15)
  volatile uint32_t TriggerConfig6;  ///< 0x50 [R/W]: Trigger Configuration Register (Instance 7 of 15)
  volatile uint32_t TrTimestampStatus6;  ///< 0x54 [R]: Trigger Timestamp Status Register (Instance 7 of 15)
  volatile uint32_t TriggerConfig7;  ///< 0x58 [R/W]: Trigger Configuration Register (Instance 8 of 15)
  volatile uint32_t TrTimestampStatus7;  ///< 0x5c [R]: Trigger Timestamp Status Register (Instance 8 of 15)
  volatile uint32_t TriggerConfig8;  ///< 0x60 [R/W]: Trigger Configuration Register (Instance 9 of 15)
  volatile uint32_t TrTimestampStatus8;  ///< 0x64 [R]: Trigger Timestamp Status Register (Instance 9 of 15)
  volatile uint32_t TriggerConfig9;  ///< 0x68 [R/W]: Trigger Configuration Register (Instance 10 of 15)
  volatile uint32_t TrTimestampStatus9;  ///< 0x6c [R]: Trigger Timestamp Status Register (Instance 10 of 15)
  volatile uint32_t TriggerConfig10;  ///< 0x70 [R/W]: Trigger Configuration Register (Instance 11 of 15)
  volatile uint32_t TrTimestampStatus10;  ///< 0x74 [R]: Trigger Timestamp Status Register (Instance 11 of 15)
  volatile uint32_t TriggerConfig11;  ///< 0x78 [R/W]: Trigger Configuration Register (Instance 12 of 15)
  volatile uint32_t TrTimestampStatus11;  ///< 0x7c [R]: Trigger Timestamp Status Register (Instance 12 of 15)
  volatile uint32_t TriggerConfig12;  ///< 0x80 [R/W]: Trigger Configuration Register (Instance 13 of 15)
  volatile uint32_t TrTimestampStatus12;  ///< 0x84 [R]: Trigger Timestamp Status Register (Instance 13 of 15)
  volatile uint32_t TriggerConfig13;  ///< 0x88 [R/W]: Trigger Configuration Register (Instance 14 of 15)
  volatile uint32_t TrTimestampStatus13;  ///< 0x8c [R]: Trigger Timestamp Status Register (Instance 14 of 15)
  volatile uint32_t TriggerConfig14;  ///< 0x90 [R/W]: Trigger Configuration Register (Instance 15 of 15)
  volatile uint32_t TrTimestampStatus14;  ///< 0x94 [R]: Trigger Timestamp Status Register (Instance 15 of 15)
  volatile uint32_t reserved1[2];
  volatile uint32_t Threshold1;  ///< 0xa0 [R/W]: Threshold1 Register
  volatile uint32_t Threshold2;  ///< 0xa4 [R/W]: Threshold2 Register
  volatile uint32_t Threshold3;  ///< 0xa8 [R/W]: Threshold3 Register
  volatile uint32_t Threshold4;  ///< 0xac [R/W]: Threshold4 Register
  volatile uint32_t Threshold5;  ///< 0xb0 [R/W]: Threshold5 Register
  volatile uint32_t Threshold6;  ///< 0xb4 [R/W]: Threshold6 Register
  volatile uint32_t Threshold7;  ///< 0xb8 [R/W]: Threshold7 Register
  volatile uint32_t reserved2;
  volatile uint32_t IntEna;  ///< 0xc0 [R/W]: Interrupt Enable Register
  volatile uint32_t IntRaw;  ///< 0xc4 [R]: Interrupt Raw Register
  volatile uint32_t IntActive;  ///< 0xc8 [R]: Interrupt Active Register
  volatile uint32_t IntAck;  ///< 0xcc [W]: Interrupt Acknowledge Register
  volatile uint32_t IntForce;  ///< 0xd0 [W]: Interrupt Force Register
  volatile uint32_t Int2Ena;  ///< 0xd4 [R/W]: Interrupt2 Enable Register
  volatile uint32_t Int2Raw;  ///< 0xd8 [R]: Interrupt2 Raw Register
  volatile uint32_t Int2Active;  ///< 0xdc [R]: Interrupt2 Active Register
  volatile uint32_t Int2Ack;  ///< 0xe0 [W]: Interrupt2 Acknowledge Register
  volatile uint32_t Int2Force;  ///< 0xe4 [W]: Interrupt2 Force Register
  volatile uint32_t FWRequest;  ///< 0xe8 [R/W]: Firmware Request Register
  volatile uint32_t reserved3;
  volatile uint32_t BypControl;  ///< 0xf0 [R/W]: Bypass Control Register
  volatile uint32_t BypStatus;  ///< 0xf4 [R]: Bypass Status Register
  volatile uint32_t reserved4[2];
  volatile uint32_t RXFIFO1;  ///< 0x100 [R]: RXFIFO1 Register
  volatile uint32_t RXFIFO2;  ///< 0x104 [R]: RXFIFO2 Register
  volatile uint32_t reserved5[30];
  volatile uint32_t RXData0;  ///< 0x180 [R]: RXData Result Register (Instance 1 of 16)
  volatile uint32_t RXData1;  ///< 0x184 [R]: RXData Result Register (Instance 2 of 16)
  volatile uint32_t RXData2;  ///< 0x188 [R]: RXData Result Register (Instance 3 of 16)
  volatile uint32_t RXData3;  ///< 0x18c [R]: RXData Result Register (Instance 4 of 16)
  volatile uint32_t RXData4;  ///< 0x190 [R]: RXData Result Register (Instance 5 of 16)
  volatile uint32_t RXData5;  ///< 0x194 [R]: RXData Result Register (Instance 6 of 16)
  volatile uint32_t RXData6;  ///< 0x198 [R]: RXData Result Register (Instance 7 of 16)
  volatile uint32_t RXData7;  ///< 0x19c [R]: RXData Result Register (Instance 8 of 16)
  volatile uint32_t RXData8;  ///< 0x1a0 [R]: RXData Result Register (Instance 9 of 16)
  volatile uint32_t RXData9;  ///< 0x1a4 [R]: RXData Result Register (Instance 10 of 16)
  volatile uint32_t RXData10;  ///< 0x1a8 [R]: RXData Result Register (Instance 11 of 16)
  volatile uint32_t RXData11;  ///< 0x1ac [R]: RXData Result Register (Instance 12 of 16)
  volatile uint32_t RXData12;  ///< 0x1b0 [R]: RXData Result Register (Instance 13 of 16)
  volatile uint32_t RXData13;  ///< 0x1b4 [R]: RXData Result Register (Instance 14 of 16)
  volatile uint32_t RXData14;  ///< 0x1b8 [R]: RXData Result Register (Instance 15 of 16)
  volatile uint32_t RXData15;  ///< 0x1bc [R]: RXData Result Register (Instance 16 of 16)
  volatile uint32_t reserved6[16];
  volatile uint32_t Instruction0;  ///< 0x200 [R/W]: Instruction Configuration Table Register (Instance 1 of 16)
  volatile uint32_t Instruction1;  ///< 0x204 [R/W]: Instruction Configuration Table Register (Instance 2 of 16)
  volatile uint32_t Instruction2;  ///< 0x208 [R/W]: Instruction Configuration Table Register (Instance 3 of 16)
  volatile uint32_t Instruction3;  ///< 0x20c [R/W]: Instruction Configuration Table Register (Instance 4 of 16)
  volatile uint32_t Instruction4;  ///< 0x210 [R/W]: Instruction Configuration Table Register (Instance 5 of 16)
  volatile uint32_t Instruction5;  ///< 0x214 [R/W]: Instruction Configuration Table Register (Instance 6 of 16)
  volatile uint32_t Instruction6;  ///< 0x218 [R/W]: Instruction Configuration Table Register (Instance 7 of 16)
  volatile uint32_t Instruction7;  ///< 0x21c [R/W]: Instruction Configuration Table Register (Instance 8 of 16)
  volatile uint32_t Instruction8;  ///< 0x220 [R/W]: Instruction Configuration Table Register (Instance 9 of 16)
  volatile uint32_t Instruction9;  ///< 0x224 [R/W]: Instruction Configuration Table Register (Instance 10 of 16)
  volatile uint32_t Instruction10;  ///< 0x228 [R/W]: Instruction Configuration Table Register (Instance 11 of 16)
  volatile uint32_t Instruction11;  ///< 0x22c [R/W]: Instruction Configuration Table Register (Instance 12 of 16)
  volatile uint32_t Instruction12;  ///< 0x230 [R/W]: Instruction Configuration Table Register (Instance 13 of 16)
  volatile uint32_t Instruction13;  ///< 0x234 [R/W]: Instruction Configuration Table Register (Instance 14 of 16)
  volatile uint32_t Instruction14;  ///< 0x238 [R/W]: Instruction Configuration Table Register (Instance 15 of 16)
  volatile uint32_t Instruction15;  ///< 0x23c [R/W]: Instruction Configuration Table Register (Instance 16 of 16)
  volatile uint32_t reserved7[16];
  volatile uint32_t Priority0;  ///< 0x280 [R/W]: Priority Control Table Register (Instance 1 of 15)
  volatile uint32_t Priority1;  ///< 0x284 [R/W]: Priority Control Table Register (Instance 2 of 15)
  volatile uint32_t Priority2;  ///< 0x288 [R/W]: Priority Control Table Register (Instance 3 of 15)
  volatile uint32_t Priority3;  ///< 0x28c [R/W]: Priority Control Table Register (Instance 4 of 15)
  volatile uint32_t Priority4;  ///< 0x290 [R/W]: Priority Control Table Register (Instance 5 of 15)
  volatile uint32_t Priority5;  ///< 0x294 [R/W]: Priority Control Table Register (Instance 6 of 15)
  volatile uint32_t Priority6;  ///< 0x298 [R/W]: Priority Control Table Register (Instance 7 of 15)
  volatile uint32_t Priority7;  ///< 0x29c [R/W]: Priority Control Table Register (Instance 8 of 15)
  volatile uint32_t Priority8;  ///< 0x2a0 [R/W]: Priority Control Table Register (Instance 9 of 15)
  volatile uint32_t Priority9;  ///< 0x2a4 [R/W]: Priority Control Table Register (Instance 10 of 15)
  volatile uint32_t Priority10;  ///< 0x2a8 [R/W]: Priority Control Table Register (Instance 11 of 15)
  volatile uint32_t Priority11;  ///< 0x2ac [R/W]: Priority Control Table Register (Instance 12 of 15)
  volatile uint32_t Priority12;  ///< 0x2b0 [R/W]: Priority Control Table Register (Instance 13 of 15)
  volatile uint32_t Priority13;  ///< 0x2b4 [R/W]: Priority Control Table Register (Instance 14 of 15)
  volatile uint32_t Priority14;  ///< 0x2b8 [R/W]: Priority Control Table Register (Instance 15 of 15)
  volatile uint32_t Priority_15;  ///< 0x2bc [R/W]: Priority Control Table Register for Continuous mode
  volatile uint32_t reserved8[16];
  volatile uint32_t TriggerMuxControl0;  ///< 0x300 [R/W]: HWTrigger Mux Control0
  volatile uint32_t TriggerMuxControl1;  ///< 0x304 [R/W]: HWTrigger Mux Control1
  volatile uint32_t DatastampMuxControl;  ///< 0x308 [R/W]: Datastamp Mux Control
} ADC_REGS_REGS_t;

#endif // ADC_REGS
