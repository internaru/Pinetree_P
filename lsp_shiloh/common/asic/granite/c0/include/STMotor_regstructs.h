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
 * \file STMotor_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _STMOTOR_REG_REGSTRUCTS_H_
#define _STMOTOR_REG_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: STMotor (STMotor_reg)
/** \brief Top-level register file for Stepper Motor Controller block*/
//
//====================================================================

typedef struct STMOTOR_REG_REGS_s
{
  volatile uint32_t SMC_CFG;  ///< 0x0 [R/W]: Block Enable
  volatile uint32_t SMC_CTRL;  ///< 0x4 [W]: Block Control Register
  volatile uint32_t LS_INCR;  ///< 0x8 [R/W]: Line Start Increment
  volatile uint32_t REG_01;  ///< 0xc [R/W]: Register Output Bits
  volatile uint32_t NUM_USTEPS;  ///< 0x10 [R/W]: Number of Micro-steps
  volatile uint32_t STEP_EN_IDLE;  ///< 0x14 [R/W]: Idle State Outputs
  volatile uint32_t TVT_01;  ///< 0x18 [R/W]: Torque Vector Table 0/1
  volatile uint32_t TVT_23;  ///< 0x1c [R/W]: Torque Vector Table 2/3
  volatile uint32_t TVT_45;  ///< 0x20 [R/W]: Torque Vector Table 4/5
  volatile uint32_t TVT_67;  ///< 0x24 [R/W]: Torque Vector Table 6/7
  volatile uint32_t TVT_8;  ///< 0x28 [R/W]: Torque Vector Table 8
  volatile uint32_t SEQ_0;  ///< 0x2c [R/W]: Sequence 0
  volatile uint32_t SEQ_1;  ///< 0x30 [R/W]: Sequence 1
  volatile uint32_t SEQ_2;  ///< 0x34 [R/W]: Sequence 2
  volatile uint32_t SEQ_3;  ///< 0x38 [R/W]: Sequence 3
  volatile uint32_t SEQ_4;  ///< 0x3c [R/W]: Sequence 4
  volatile uint32_t SEQ_5;  ///< 0x40 [R/W]: Sequence 5
  volatile uint32_t OUT_MODE;  ///< 0x44 [R/W]: Output Mode
  volatile uint32_t STEP_INT;  ///< 0x48 [R/W]: Step Interrupt Amount
  volatile uint32_t PWM_T;  ///< 0x4c [R/W]: PWM_T Register
  volatile uint32_t PWM_P;  ///< 0x50 [R/W]: PWM_P Register
  volatile uint32_t ACC_INCR;  ///< 0x54 [R/W]: Acceleration Increment
  volatile uint32_t PWM_M;  ///< 0x58 [R/W]: PWM_M Register
  volatile uint32_t CMD;  ///< 0x5c [R/W]: Command Register
  volatile uint32_t CMD_ISSUE;  ///< 0x60 [W]: Command Issue Register
  volatile uint32_t COM_STAT;  ///< 0x64 [R]: Status Register
  volatile uint32_t COM_PWM_P;  ///< 0x68 [R]: Current PWM_P Value
  volatile uint32_t COM_ACC_INCR;  ///< 0x6c [R]: Current Acceleration Increment
  volatile uint32_t COM_PWM_M;  ///< 0x70 [R]: Current PWM_M Value
  volatile uint32_t COM_CMD;  ///< 0x74 [R]: Current Command Value
  volatile uint32_t COM_STP;  ///< 0x78 [R]: Step Count
  volatile uint32_t COM_TVT;  ///< 0x7c [R]: TVT Index
  volatile uint32_t COM_SEQ;  ///< 0x80 [R]: Sequence Index
  volatile uint32_t I_EN;  ///< 0x84 [R/W]: Interrupt Enable Register
  volatile uint32_t I_PEND;  ///< 0x88 [R]: Interrupt Pending Register
  volatile uint32_t I_ACK;  ///< 0x8c [W]: Interrupt Acknowledge Register
  volatile uint32_t I_FORCE;  ///< 0x90 [W]: Interrupt Force Register
  volatile uint32_t SMC_TRIG;  ///< 0x94 [R/W]: Trigger Configuration
} STMOTOR_REG_REGS_t;

#endif // STMOTOR_REG
