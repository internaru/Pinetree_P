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
 * \file STMotor_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _STMOTOR_REG_REGMASKS_H_
#define _STMOTOR_REG_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: STMotor (STMotor_reg)
/** \brief Top-level register file for Stepper Motor Controller block*/
//
//====================================================================

//====================================================================
//Register: Block Enable (SMC_CFG)
/** \brief This register is used to enable and configure the Stepper Motor Controller.*/
//====================================================================

#define  STMOTOR_REG_SMC_CFG_RESERVED1_MASK 0xfffffffc
#define  STMOTOR_REG_SMC_CFG_RESERVED1_SHIFT 2
#define  STMOTOR_REG_SMC_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CFG_RESERVED1_MASK) >> STMOTOR_REG_SMC_CFG_RESERVED1_SHIFT)
#define  STMOTOR_REG_SMC_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CFG_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CFG_RESERVED1_SHIFT))

#define  STMOTOR_REG_SMC_CFG_CDMA_MODE_MASK 0x2
#define  STMOTOR_REG_SMC_CFG_CDMA_MODE_SHIFT 1
#define  STMOTOR_REG_SMC_CFG_CDMA_MODE_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CFG_CDMA_MODE_MASK) >> STMOTOR_REG_SMC_CFG_CDMA_MODE_SHIFT)
#define  STMOTOR_REG_SMC_CFG_CDMA_MODE_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CFG_CDMA_MODE_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CFG_CDMA_MODE_SHIFT))

#define  STMOTOR_REG_SMC_CFG_STEP_EN_MASK 0x1
#define  STMOTOR_REG_SMC_CFG_STEP_EN_SHIFT 0
#define  STMOTOR_REG_SMC_CFG_STEP_EN_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CFG_STEP_EN_MASK) >> STMOTOR_REG_SMC_CFG_STEP_EN_SHIFT)
#define  STMOTOR_REG_SMC_CFG_STEP_EN_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CFG_STEP_EN_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CFG_STEP_EN_SHIFT))

//====================================================================
//Register: Block Control Register (SMC_CTRL)
/** \brief This write only register is used to control the inner workings of the block.*/
//====================================================================

#define  STMOTOR_REG_SMC_CTRL_RESERVED1_MASK 0xfffffe00
#define  STMOTOR_REG_SMC_CTRL_RESERVED1_SHIFT 9
#define  STMOTOR_REG_SMC_CTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CTRL_RESERVED1_MASK) >> STMOTOR_REG_SMC_CTRL_RESERVED1_SHIFT)
#define  STMOTOR_REG_SMC_CTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CTRL_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CTRL_RESERVED1_SHIFT))

#define  STMOTOR_REG_SMC_CTRL_SRESET_MASK 0x100
#define  STMOTOR_REG_SMC_CTRL_SRESET_SHIFT 8
#define  STMOTOR_REG_SMC_CTRL_SRESET_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CTRL_SRESET_MASK) >> STMOTOR_REG_SMC_CTRL_SRESET_SHIFT)
#define  STMOTOR_REG_SMC_CTRL_SRESET_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CTRL_SRESET_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CTRL_SRESET_SHIFT))

#define  STMOTOR_REG_SMC_CTRL_RESERVED2_MASK 0xe0
#define  STMOTOR_REG_SMC_CTRL_RESERVED2_SHIFT 5
#define  STMOTOR_REG_SMC_CTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CTRL_RESERVED2_MASK) >> STMOTOR_REG_SMC_CTRL_RESERVED2_SHIFT)
#define  STMOTOR_REG_SMC_CTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CTRL_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CTRL_RESERVED2_SHIFT))

#define  STMOTOR_REG_SMC_CTRL_STCNT_RST_MASK 0x10
#define  STMOTOR_REG_SMC_CTRL_STCNT_RST_SHIFT 4
#define  STMOTOR_REG_SMC_CTRL_STCNT_RST_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CTRL_STCNT_RST_MASK) >> STMOTOR_REG_SMC_CTRL_STCNT_RST_SHIFT)
#define  STMOTOR_REG_SMC_CTRL_STCNT_RST_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CTRL_STCNT_RST_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CTRL_STCNT_RST_SHIFT))

#define  STMOTOR_REG_SMC_CTRL_RESERVED3_MASK 0xe
#define  STMOTOR_REG_SMC_CTRL_RESERVED3_SHIFT 1
#define  STMOTOR_REG_SMC_CTRL_RESERVED3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CTRL_RESERVED3_MASK) >> STMOTOR_REG_SMC_CTRL_RESERVED3_SHIFT)
#define  STMOTOR_REG_SMC_CTRL_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CTRL_RESERVED3_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CTRL_RESERVED3_SHIFT))

#define  STMOTOR_REG_SMC_CTRL_CMD_ABRT_MASK 0x1
#define  STMOTOR_REG_SMC_CTRL_CMD_ABRT_SHIFT 0
#define  STMOTOR_REG_SMC_CTRL_CMD_ABRT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_CTRL_CMD_ABRT_MASK) >> STMOTOR_REG_SMC_CTRL_CMD_ABRT_SHIFT)
#define  STMOTOR_REG_SMC_CTRL_CMD_ABRT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_CTRL_CMD_ABRT_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_CTRL_CMD_ABRT_SHIFT))

//====================================================================
//Register: Line Start Increment (LS_INCR)
/** \brief Line Start Increment.*/
//====================================================================

#define  STMOTOR_REG_LS_INCR_RESERVED1_MASK 0xffe00000
#define  STMOTOR_REG_LS_INCR_RESERVED1_SHIFT 21
#define  STMOTOR_REG_LS_INCR_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_LS_INCR_RESERVED1_MASK) >> STMOTOR_REG_LS_INCR_RESERVED1_SHIFT)
#define  STMOTOR_REG_LS_INCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_LS_INCR_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_LS_INCR_RESERVED1_SHIFT))

#define  STMOTOR_REG_LS_INCR_LS_SRC_MASK 0x100000
#define  STMOTOR_REG_LS_INCR_LS_SRC_SHIFT 20
#define  STMOTOR_REG_LS_INCR_LS_SRC_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_LS_INCR_LS_SRC_MASK) >> STMOTOR_REG_LS_INCR_LS_SRC_SHIFT)
#define  STMOTOR_REG_LS_INCR_LS_SRC_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_LS_INCR_LS_SRC_MASK) | (((uint32_t)val) << STMOTOR_REG_LS_INCR_LS_SRC_SHIFT))

#define  STMOTOR_REG_LS_INCR_RESERVED2_MASK 0xf0000
#define  STMOTOR_REG_LS_INCR_RESERVED2_SHIFT 16
#define  STMOTOR_REG_LS_INCR_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_LS_INCR_RESERVED2_MASK) >> STMOTOR_REG_LS_INCR_RESERVED2_SHIFT)
#define  STMOTOR_REG_LS_INCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_LS_INCR_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_LS_INCR_RESERVED2_SHIFT))

#define  STMOTOR_REG_LS_INCR_LS_INCR_MASK 0xffff
#define  STMOTOR_REG_LS_INCR_LS_INCR_SHIFT 0
#define  STMOTOR_REG_LS_INCR_LS_INCR_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_LS_INCR_LS_INCR_MASK) >> STMOTOR_REG_LS_INCR_LS_INCR_SHIFT)
#define  STMOTOR_REG_LS_INCR_LS_INCR_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_LS_INCR_LS_INCR_MASK) | (((uint32_t)val) << STMOTOR_REG_LS_INCR_LS_INCR_SHIFT))

//====================================================================
//Register: Register Output Bits (REG_01)
/** \brief Registered output bits 0 and 1.*/
//====================================================================

#define  STMOTOR_REG_REG_01_RESERVED1_MASK 0xffffffe0
#define  STMOTOR_REG_REG_01_RESERVED1_SHIFT 5
#define  STMOTOR_REG_REG_01_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_REG_01_RESERVED1_MASK) >> STMOTOR_REG_REG_01_RESERVED1_SHIFT)
#define  STMOTOR_REG_REG_01_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_REG_01_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_REG_01_RESERVED1_SHIFT))

#define  STMOTOR_REG_REG_01_REG_BIT1_MASK 0x10
#define  STMOTOR_REG_REG_01_REG_BIT1_SHIFT 4
#define  STMOTOR_REG_REG_01_REG_BIT1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_REG_01_REG_BIT1_MASK) >> STMOTOR_REG_REG_01_REG_BIT1_SHIFT)
#define  STMOTOR_REG_REG_01_REG_BIT1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_REG_01_REG_BIT1_MASK) | (((uint32_t)val) << STMOTOR_REG_REG_01_REG_BIT1_SHIFT))

#define  STMOTOR_REG_REG_01_RESERVED2_MASK 0xe
#define  STMOTOR_REG_REG_01_RESERVED2_SHIFT 1
#define  STMOTOR_REG_REG_01_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_REG_01_RESERVED2_MASK) >> STMOTOR_REG_REG_01_RESERVED2_SHIFT)
#define  STMOTOR_REG_REG_01_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_REG_01_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_REG_01_RESERVED2_SHIFT))

#define  STMOTOR_REG_REG_01_REG_BIT0_MASK 0x1
#define  STMOTOR_REG_REG_01_REG_BIT0_SHIFT 0
#define  STMOTOR_REG_REG_01_REG_BIT0_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_REG_01_REG_BIT0_MASK) >> STMOTOR_REG_REG_01_REG_BIT0_SHIFT)
#define  STMOTOR_REG_REG_01_REG_BIT0_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_REG_01_REG_BIT0_MASK) | (((uint32_t)val) << STMOTOR_REG_REG_01_REG_BIT0_SHIFT))

//====================================================================
//Register: Number of Micro-steps (NUM_USTEPS)
/** \brief Number of micro-steps per step.*/
//====================================================================

#define  STMOTOR_REG_NUM_USTEPS_RESERVED1_MASK 0xfffffff0
#define  STMOTOR_REG_NUM_USTEPS_RESERVED1_SHIFT 4
#define  STMOTOR_REG_NUM_USTEPS_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_NUM_USTEPS_RESERVED1_MASK) >> STMOTOR_REG_NUM_USTEPS_RESERVED1_SHIFT)
#define  STMOTOR_REG_NUM_USTEPS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_NUM_USTEPS_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_NUM_USTEPS_RESERVED1_SHIFT))

#define  STMOTOR_REG_NUM_USTEPS_NUM_MASK 0xf
#define  STMOTOR_REG_NUM_USTEPS_NUM_SHIFT 0
#define  STMOTOR_REG_NUM_USTEPS_NUM_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_NUM_USTEPS_NUM_MASK) >> STMOTOR_REG_NUM_USTEPS_NUM_SHIFT)
#define  STMOTOR_REG_NUM_USTEPS_NUM_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_NUM_USTEPS_NUM_MASK) | (((uint32_t)val) << STMOTOR_REG_NUM_USTEPS_NUM_SHIFT))

//====================================================================
//Register: Idle State Outputs (STEP_EN_IDLE)
/** \brief Current command.*/
//====================================================================

#define  STMOTOR_REG_STEP_EN_IDLE_RESERVED1_MASK 0xffffffc0
#define  STMOTOR_REG_STEP_EN_IDLE_RESERVED1_SHIFT 6
#define  STMOTOR_REG_STEP_EN_IDLE_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_STEP_EN_IDLE_RESERVED1_MASK) >> STMOTOR_REG_STEP_EN_IDLE_RESERVED1_SHIFT)
#define  STMOTOR_REG_STEP_EN_IDLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_STEP_EN_IDLE_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_STEP_EN_IDLE_RESERVED1_SHIFT))

#define  STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_MASK 0x3f
#define  STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_SHIFT 0
#define  STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_MASK) >> STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_SHIFT)
#define  STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_MASK) | (((uint32_t)val) << STMOTOR_REG_STEP_EN_IDLE_STEP_EN_IDLE_SMC_SHIFT))

//====================================================================
//Register: Torque Vector Table 0/1 (TVT_01)
/** \brief Torque Vector Table Values 0,1.*/
//====================================================================

#define  STMOTOR_REG_TVT_01_TVT_1_MASK 0xffff0000
#define  STMOTOR_REG_TVT_01_TVT_1_SHIFT 16
#define  STMOTOR_REG_TVT_01_TVT_1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_01_TVT_1_MASK) >> STMOTOR_REG_TVT_01_TVT_1_SHIFT)
#define  STMOTOR_REG_TVT_01_TVT_1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_01_TVT_1_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_01_TVT_1_SHIFT))

#define  STMOTOR_REG_TVT_01_TVT_0_MASK 0xffff
#define  STMOTOR_REG_TVT_01_TVT_0_SHIFT 0
#define  STMOTOR_REG_TVT_01_TVT_0_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_01_TVT_0_MASK) >> STMOTOR_REG_TVT_01_TVT_0_SHIFT)
#define  STMOTOR_REG_TVT_01_TVT_0_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_01_TVT_0_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_01_TVT_0_SHIFT))

//====================================================================
//Register: Torque Vector Table 2/3 (TVT_23)
/** \brief Torque Vector Table Values 2,3.*/
//====================================================================

#define  STMOTOR_REG_TVT_23_TVT_3_MASK 0xffff0000
#define  STMOTOR_REG_TVT_23_TVT_3_SHIFT 16
#define  STMOTOR_REG_TVT_23_TVT_3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_23_TVT_3_MASK) >> STMOTOR_REG_TVT_23_TVT_3_SHIFT)
#define  STMOTOR_REG_TVT_23_TVT_3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_23_TVT_3_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_23_TVT_3_SHIFT))

#define  STMOTOR_REG_TVT_23_TVT_2_MASK 0xffff
#define  STMOTOR_REG_TVT_23_TVT_2_SHIFT 0
#define  STMOTOR_REG_TVT_23_TVT_2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_23_TVT_2_MASK) >> STMOTOR_REG_TVT_23_TVT_2_SHIFT)
#define  STMOTOR_REG_TVT_23_TVT_2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_23_TVT_2_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_23_TVT_2_SHIFT))

//====================================================================
//Register: Torque Vector Table 4/5 (TVT_45)
/** \brief Torque Vector Table Values 4,5.*/
//====================================================================

#define  STMOTOR_REG_TVT_45_TVT_5_MASK 0xffff0000
#define  STMOTOR_REG_TVT_45_TVT_5_SHIFT 16
#define  STMOTOR_REG_TVT_45_TVT_5_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_45_TVT_5_MASK) >> STMOTOR_REG_TVT_45_TVT_5_SHIFT)
#define  STMOTOR_REG_TVT_45_TVT_5_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_45_TVT_5_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_45_TVT_5_SHIFT))

#define  STMOTOR_REG_TVT_45_TVT_4_MASK 0xffff
#define  STMOTOR_REG_TVT_45_TVT_4_SHIFT 0
#define  STMOTOR_REG_TVT_45_TVT_4_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_45_TVT_4_MASK) >> STMOTOR_REG_TVT_45_TVT_4_SHIFT)
#define  STMOTOR_REG_TVT_45_TVT_4_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_45_TVT_4_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_45_TVT_4_SHIFT))

//====================================================================
//Register: Torque Vector Table 6/7 (TVT_67)
/** \brief Torque Vector Table Values 6,7.*/
//====================================================================

#define  STMOTOR_REG_TVT_67_TVT_7_MASK 0xffff0000
#define  STMOTOR_REG_TVT_67_TVT_7_SHIFT 16
#define  STMOTOR_REG_TVT_67_TVT_7_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_67_TVT_7_MASK) >> STMOTOR_REG_TVT_67_TVT_7_SHIFT)
#define  STMOTOR_REG_TVT_67_TVT_7_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_67_TVT_7_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_67_TVT_7_SHIFT))

#define  STMOTOR_REG_TVT_67_TVT_6_MASK 0xffff
#define  STMOTOR_REG_TVT_67_TVT_6_SHIFT 0
#define  STMOTOR_REG_TVT_67_TVT_6_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_67_TVT_6_MASK) >> STMOTOR_REG_TVT_67_TVT_6_SHIFT)
#define  STMOTOR_REG_TVT_67_TVT_6_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_67_TVT_6_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_67_TVT_6_SHIFT))

//====================================================================
//Register: Torque Vector Table 8 (TVT_8)
/** \brief Torque Vector Table Values 8.*/
//====================================================================

#define  STMOTOR_REG_TVT_8_RESERVED1_MASK 0xffff0000
#define  STMOTOR_REG_TVT_8_RESERVED1_SHIFT 16
#define  STMOTOR_REG_TVT_8_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_8_RESERVED1_MASK) >> STMOTOR_REG_TVT_8_RESERVED1_SHIFT)
#define  STMOTOR_REG_TVT_8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_8_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_8_RESERVED1_SHIFT))

#define  STMOTOR_REG_TVT_8_TVT_8_MASK 0xffff
#define  STMOTOR_REG_TVT_8_TVT_8_SHIFT 0
#define  STMOTOR_REG_TVT_8_TVT_8_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_TVT_8_TVT_8_MASK) >> STMOTOR_REG_TVT_8_TVT_8_SHIFT)
#define  STMOTOR_REG_TVT_8_TVT_8_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_TVT_8_TVT_8_MASK) | (((uint32_t)val) << STMOTOR_REG_TVT_8_TVT_8_SHIFT))

//====================================================================
//Register: Sequence 0 (SEQ_0)
/** \brief Sequence 0 definition.*/
//====================================================================

#define  STMOTOR_REG_SEQ_0_SEQ_0_MASK 0xffffffff
#define  STMOTOR_REG_SEQ_0_SEQ_0_SHIFT 0
#define  STMOTOR_REG_SEQ_0_SEQ_0_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SEQ_0_SEQ_0_MASK) >> STMOTOR_REG_SEQ_0_SEQ_0_SHIFT)
#define  STMOTOR_REG_SEQ_0_SEQ_0_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SEQ_0_SEQ_0_MASK) | (((uint32_t)val) << STMOTOR_REG_SEQ_0_SEQ_0_SHIFT))

//====================================================================
//Register: Sequence 1 (SEQ_1)
/** \brief Sequence 1 definition.*/
//====================================================================

#define  STMOTOR_REG_SEQ_1_SEQ_1_MASK 0xffffffff
#define  STMOTOR_REG_SEQ_1_SEQ_1_SHIFT 0
#define  STMOTOR_REG_SEQ_1_SEQ_1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SEQ_1_SEQ_1_MASK) >> STMOTOR_REG_SEQ_1_SEQ_1_SHIFT)
#define  STMOTOR_REG_SEQ_1_SEQ_1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SEQ_1_SEQ_1_MASK) | (((uint32_t)val) << STMOTOR_REG_SEQ_1_SEQ_1_SHIFT))

//====================================================================
//Register: Sequence 2 (SEQ_2)
/** \brief Sequence 2 definition.*/
//====================================================================

#define  STMOTOR_REG_SEQ_2_SEQ_2_MASK 0xffffffff
#define  STMOTOR_REG_SEQ_2_SEQ_2_SHIFT 0
#define  STMOTOR_REG_SEQ_2_SEQ_2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SEQ_2_SEQ_2_MASK) >> STMOTOR_REG_SEQ_2_SEQ_2_SHIFT)
#define  STMOTOR_REG_SEQ_2_SEQ_2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SEQ_2_SEQ_2_MASK) | (((uint32_t)val) << STMOTOR_REG_SEQ_2_SEQ_2_SHIFT))

//====================================================================
//Register: Sequence 3 (SEQ_3)
/** \brief Sequence 1 definition.*/
//====================================================================

#define  STMOTOR_REG_SEQ_3_SEQ_3_MASK 0xffffffff
#define  STMOTOR_REG_SEQ_3_SEQ_3_SHIFT 0
#define  STMOTOR_REG_SEQ_3_SEQ_3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SEQ_3_SEQ_3_MASK) >> STMOTOR_REG_SEQ_3_SEQ_3_SHIFT)
#define  STMOTOR_REG_SEQ_3_SEQ_3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SEQ_3_SEQ_3_MASK) | (((uint32_t)val) << STMOTOR_REG_SEQ_3_SEQ_3_SHIFT))

//====================================================================
//Register: Sequence 4 (SEQ_4)
/** \brief Sequence 4 definition.*/
//====================================================================

#define  STMOTOR_REG_SEQ_4_SEQ_4_MASK 0xffffffff
#define  STMOTOR_REG_SEQ_4_SEQ_4_SHIFT 0
#define  STMOTOR_REG_SEQ_4_SEQ_4_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SEQ_4_SEQ_4_MASK) >> STMOTOR_REG_SEQ_4_SEQ_4_SHIFT)
#define  STMOTOR_REG_SEQ_4_SEQ_4_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SEQ_4_SEQ_4_MASK) | (((uint32_t)val) << STMOTOR_REG_SEQ_4_SEQ_4_SHIFT))

//====================================================================
//Register: Sequence 5 (SEQ_5)
/** \brief Sequence 5 definition.*/
//====================================================================

#define  STMOTOR_REG_SEQ_5_SEQ_5_MASK 0xffffffff
#define  STMOTOR_REG_SEQ_5_SEQ_5_SHIFT 0
#define  STMOTOR_REG_SEQ_5_SEQ_5_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SEQ_5_SEQ_5_MASK) >> STMOTOR_REG_SEQ_5_SEQ_5_SHIFT)
#define  STMOTOR_REG_SEQ_5_SEQ_5_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SEQ_5_SEQ_5_MASK) | (((uint32_t)val) << STMOTOR_REG_SEQ_5_SEQ_5_SHIFT))

//====================================================================
//Register: Output Mode (OUT_MODE)
/** \brief Output mode selection for the SMC output lines. The following table describes the encoding:
<table align="center"> <tr><th>o_mode[2:0]</th> <th>SMC Output</th></tr> <tr><td>'000'</td><td>PWM1</td></tr> <tr><td>'001'</td><td>PWM2</td></tr> <tr><td>'010'</td><td>REG0</td></tr> <tr><td>'011'</td><td>REG1</td></tr> <tr><td>'100'</td><td>SEQ[i]</td></tr> <tr><td>'101'</td><td>LineStart (for debug)</td></tr></table>*/
//====================================================================

#define  STMOTOR_REG_OUT_MODE_RESERVED1_MASK 0xff800000
#define  STMOTOR_REG_OUT_MODE_RESERVED1_SHIFT 23
#define  STMOTOR_REG_OUT_MODE_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_RESERVED1_MASK) >> STMOTOR_REG_OUT_MODE_RESERVED1_SHIFT)
#define  STMOTOR_REG_OUT_MODE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_RESERVED1_SHIFT))

#define  STMOTOR_REG_OUT_MODE_O_MODE5_MASK 0x700000
#define  STMOTOR_REG_OUT_MODE_O_MODE5_SHIFT 20
#define  STMOTOR_REG_OUT_MODE_O_MODE5_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_O_MODE5_MASK) >> STMOTOR_REG_OUT_MODE_O_MODE5_SHIFT)
#define  STMOTOR_REG_OUT_MODE_O_MODE5_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_O_MODE5_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_O_MODE5_SHIFT))

#define  STMOTOR_REG_OUT_MODE_RESERVED2_MASK 0x80000
#define  STMOTOR_REG_OUT_MODE_RESERVED2_SHIFT 19
#define  STMOTOR_REG_OUT_MODE_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_RESERVED2_MASK) >> STMOTOR_REG_OUT_MODE_RESERVED2_SHIFT)
#define  STMOTOR_REG_OUT_MODE_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_RESERVED2_SHIFT))

#define  STMOTOR_REG_OUT_MODE_O_MODE4_MASK 0x70000
#define  STMOTOR_REG_OUT_MODE_O_MODE4_SHIFT 16
#define  STMOTOR_REG_OUT_MODE_O_MODE4_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_O_MODE4_MASK) >> STMOTOR_REG_OUT_MODE_O_MODE4_SHIFT)
#define  STMOTOR_REG_OUT_MODE_O_MODE4_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_O_MODE4_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_O_MODE4_SHIFT))

#define  STMOTOR_REG_OUT_MODE_RESERVED3_MASK 0x8000
#define  STMOTOR_REG_OUT_MODE_RESERVED3_SHIFT 15
#define  STMOTOR_REG_OUT_MODE_RESERVED3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_RESERVED3_MASK) >> STMOTOR_REG_OUT_MODE_RESERVED3_SHIFT)
#define  STMOTOR_REG_OUT_MODE_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_RESERVED3_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_RESERVED3_SHIFT))

#define  STMOTOR_REG_OUT_MODE_O_MODE3_MASK 0x7000
#define  STMOTOR_REG_OUT_MODE_O_MODE3_SHIFT 12
#define  STMOTOR_REG_OUT_MODE_O_MODE3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_O_MODE3_MASK) >> STMOTOR_REG_OUT_MODE_O_MODE3_SHIFT)
#define  STMOTOR_REG_OUT_MODE_O_MODE3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_O_MODE3_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_O_MODE3_SHIFT))

#define  STMOTOR_REG_OUT_MODE_RESERVED4_MASK 0x800
#define  STMOTOR_REG_OUT_MODE_RESERVED4_SHIFT 11
#define  STMOTOR_REG_OUT_MODE_RESERVED4_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_RESERVED4_MASK) >> STMOTOR_REG_OUT_MODE_RESERVED4_SHIFT)
#define  STMOTOR_REG_OUT_MODE_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_RESERVED4_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_RESERVED4_SHIFT))

#define  STMOTOR_REG_OUT_MODE_O_MODE2_MASK 0x700
#define  STMOTOR_REG_OUT_MODE_O_MODE2_SHIFT 8
#define  STMOTOR_REG_OUT_MODE_O_MODE2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_O_MODE2_MASK) >> STMOTOR_REG_OUT_MODE_O_MODE2_SHIFT)
#define  STMOTOR_REG_OUT_MODE_O_MODE2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_O_MODE2_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_O_MODE2_SHIFT))

#define  STMOTOR_REG_OUT_MODE_RESERVED5_MASK 0x80
#define  STMOTOR_REG_OUT_MODE_RESERVED5_SHIFT 7
#define  STMOTOR_REG_OUT_MODE_RESERVED5_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_RESERVED5_MASK) >> STMOTOR_REG_OUT_MODE_RESERVED5_SHIFT)
#define  STMOTOR_REG_OUT_MODE_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_RESERVED5_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_RESERVED5_SHIFT))

#define  STMOTOR_REG_OUT_MODE_O_MODE1_MASK 0x70
#define  STMOTOR_REG_OUT_MODE_O_MODE1_SHIFT 4
#define  STMOTOR_REG_OUT_MODE_O_MODE1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_O_MODE1_MASK) >> STMOTOR_REG_OUT_MODE_O_MODE1_SHIFT)
#define  STMOTOR_REG_OUT_MODE_O_MODE1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_O_MODE1_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_O_MODE1_SHIFT))

#define  STMOTOR_REG_OUT_MODE_RESERVED6_MASK 0x8
#define  STMOTOR_REG_OUT_MODE_RESERVED6_SHIFT 3
#define  STMOTOR_REG_OUT_MODE_RESERVED6_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_RESERVED6_MASK) >> STMOTOR_REG_OUT_MODE_RESERVED6_SHIFT)
#define  STMOTOR_REG_OUT_MODE_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_RESERVED6_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_RESERVED6_SHIFT))

#define  STMOTOR_REG_OUT_MODE_O_MODE0_MASK 0x7
#define  STMOTOR_REG_OUT_MODE_O_MODE0_SHIFT 0
#define  STMOTOR_REG_OUT_MODE_O_MODE0_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_OUT_MODE_O_MODE0_MASK) >> STMOTOR_REG_OUT_MODE_O_MODE0_SHIFT)
#define  STMOTOR_REG_OUT_MODE_O_MODE0_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_OUT_MODE_O_MODE0_MASK) | (((uint32_t)val) << STMOTOR_REG_OUT_MODE_O_MODE0_SHIFT))

//====================================================================
//Register: Step Interrupt Amount (STEP_INT)
/** \brief Sets the absolute step count that will trigger the step interrupt.*/
//====================================================================

#define  STMOTOR_REG_STEP_INT_RESERVED1_MASK 0xff000000
#define  STMOTOR_REG_STEP_INT_RESERVED1_SHIFT 24
#define  STMOTOR_REG_STEP_INT_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_STEP_INT_RESERVED1_MASK) >> STMOTOR_REG_STEP_INT_RESERVED1_SHIFT)
#define  STMOTOR_REG_STEP_INT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_STEP_INT_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_STEP_INT_RESERVED1_SHIFT))

#define  STMOTOR_REG_STEP_INT_AMT_MASK 0xffffff
#define  STMOTOR_REG_STEP_INT_AMT_SHIFT 0
#define  STMOTOR_REG_STEP_INT_AMT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_STEP_INT_AMT_MASK) >> STMOTOR_REG_STEP_INT_AMT_SHIFT)
#define  STMOTOR_REG_STEP_INT_AMT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_STEP_INT_AMT_MASK) | (((uint32_t)val) << STMOTOR_REG_STEP_INT_AMT_SHIFT))

//====================================================================
//Register: PWM_T Register (PWM_T)
/** \brief PWM T value.*/
//====================================================================

#define  STMOTOR_REG_PWM_T_RESERVED1_MASK 0xffff0000
#define  STMOTOR_REG_PWM_T_RESERVED1_SHIFT 16
#define  STMOTOR_REG_PWM_T_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_PWM_T_RESERVED1_MASK) >> STMOTOR_REG_PWM_T_RESERVED1_SHIFT)
#define  STMOTOR_REG_PWM_T_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_PWM_T_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_PWM_T_RESERVED1_SHIFT))

#define  STMOTOR_REG_PWM_T_PWM_T_MASK 0xffff
#define  STMOTOR_REG_PWM_T_PWM_T_SHIFT 0
#define  STMOTOR_REG_PWM_T_PWM_T_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_PWM_T_PWM_T_MASK) >> STMOTOR_REG_PWM_T_PWM_T_SHIFT)
#define  STMOTOR_REG_PWM_T_PWM_T_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_PWM_T_PWM_T_MASK) | (((uint32_t)val) << STMOTOR_REG_PWM_T_PWM_T_SHIFT))

//====================================================================
//Register: PWM_P Register (PWM_P)
/** \brief PWM P value.*/
//====================================================================

#define  STMOTOR_REG_PWM_P_RESERVED1_MASK 0xff000000
#define  STMOTOR_REG_PWM_P_RESERVED1_SHIFT 24
#define  STMOTOR_REG_PWM_P_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_PWM_P_RESERVED1_MASK) >> STMOTOR_REG_PWM_P_RESERVED1_SHIFT)
#define  STMOTOR_REG_PWM_P_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_PWM_P_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_PWM_P_RESERVED1_SHIFT))

#define  STMOTOR_REG_PWM_P_PWM_P_MASK 0xffffff
#define  STMOTOR_REG_PWM_P_PWM_P_SHIFT 0
#define  STMOTOR_REG_PWM_P_PWM_P_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_PWM_P_PWM_P_MASK) >> STMOTOR_REG_PWM_P_PWM_P_SHIFT)
#define  STMOTOR_REG_PWM_P_PWM_P_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_PWM_P_PWM_P_MASK) | (((uint32_t)val) << STMOTOR_REG_PWM_P_PWM_P_SHIFT))

//====================================================================
//Register: Acceleration Increment (ACC_INCR)
/** \brief Acceleration Increment.*/
//====================================================================

#define  STMOTOR_REG_ACC_INCR_RESERVED1_MASK 0xffffff00
#define  STMOTOR_REG_ACC_INCR_RESERVED1_SHIFT 8
#define  STMOTOR_REG_ACC_INCR_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_ACC_INCR_RESERVED1_MASK) >> STMOTOR_REG_ACC_INCR_RESERVED1_SHIFT)
#define  STMOTOR_REG_ACC_INCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_ACC_INCR_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_ACC_INCR_RESERVED1_SHIFT))

#define  STMOTOR_REG_ACC_INCR_ACC_INCR_MASK 0xff
#define  STMOTOR_REG_ACC_INCR_ACC_INCR_SHIFT 0
#define  STMOTOR_REG_ACC_INCR_ACC_INCR_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_ACC_INCR_ACC_INCR_MASK) >> STMOTOR_REG_ACC_INCR_ACC_INCR_SHIFT)
#define  STMOTOR_REG_ACC_INCR_ACC_INCR_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_ACC_INCR_ACC_INCR_MASK) | (((uint32_t)val) << STMOTOR_REG_ACC_INCR_ACC_INCR_SHIFT))

//====================================================================
//Register: PWM_M Register (PWM_M)
/** \brief PWM M value*/
//====================================================================

#define  STMOTOR_REG_PWM_M_RESERVED1_MASK 0xff000000
#define  STMOTOR_REG_PWM_M_RESERVED1_SHIFT 24
#define  STMOTOR_REG_PWM_M_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_PWM_M_RESERVED1_MASK) >> STMOTOR_REG_PWM_M_RESERVED1_SHIFT)
#define  STMOTOR_REG_PWM_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_PWM_M_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_PWM_M_RESERVED1_SHIFT))

#define  STMOTOR_REG_PWM_M_PWM_M_MASK 0xffffff
#define  STMOTOR_REG_PWM_M_PWM_M_SHIFT 0
#define  STMOTOR_REG_PWM_M_PWM_M_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_PWM_M_PWM_M_MASK) >> STMOTOR_REG_PWM_M_PWM_M_SHIFT)
#define  STMOTOR_REG_PWM_M_PWM_M_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_PWM_M_PWM_M_MASK) | (((uint32_t)val) << STMOTOR_REG_PWM_M_PWM_M_SHIFT))

//====================================================================
//Register: Command Register (CMD)
/** \brief Command to be issued.*/
//====================================================================

#define  STMOTOR_REG_CMD_ST_TGT_MASK 0xffff0000
#define  STMOTOR_REG_CMD_ST_TGT_SHIFT 16
#define  STMOTOR_REG_CMD_ST_TGT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_ST_TGT_MASK) >> STMOTOR_REG_CMD_ST_TGT_SHIFT)
#define  STMOTOR_REG_CMD_ST_TGT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_ST_TGT_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_ST_TGT_SHIFT))

#define  STMOTOR_REG_CMD_INF_MASK 0x8000
#define  STMOTOR_REG_CMD_INF_SHIFT 15
#define  STMOTOR_REG_CMD_INF_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_INF_MASK) >> STMOTOR_REG_CMD_INF_SHIFT)
#define  STMOTOR_REG_CMD_INF_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_INF_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_INF_SHIFT))

#define  STMOTOR_REG_CMD_RESERVED1_MASK 0x6000
#define  STMOTOR_REG_CMD_RESERVED1_SHIFT 13
#define  STMOTOR_REG_CMD_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_RESERVED1_MASK) >> STMOTOR_REG_CMD_RESERVED1_SHIFT)
#define  STMOTOR_REG_CMD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_RESERVED1_SHIFT))

#define  STMOTOR_REG_CMD_LS_EN_MASK 0x1000
#define  STMOTOR_REG_CMD_LS_EN_SHIFT 12
#define  STMOTOR_REG_CMD_LS_EN_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_LS_EN_MASK) >> STMOTOR_REG_CMD_LS_EN_SHIFT)
#define  STMOTOR_REG_CMD_LS_EN_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_LS_EN_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_LS_EN_SHIFT))

#define  STMOTOR_REG_CMD_RESERVED2_MASK 0xfc0
#define  STMOTOR_REG_CMD_RESERVED2_SHIFT 6
#define  STMOTOR_REG_CMD_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_RESERVED2_MASK) >> STMOTOR_REG_CMD_RESERVED2_SHIFT)
#define  STMOTOR_REG_CMD_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_RESERVED2_SHIFT))

#define  STMOTOR_REG_CMD_DIR_MASK 0x20
#define  STMOTOR_REG_CMD_DIR_SHIFT 5
#define  STMOTOR_REG_CMD_DIR_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_DIR_MASK) >> STMOTOR_REG_CMD_DIR_SHIFT)
#define  STMOTOR_REG_CMD_DIR_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_DIR_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_DIR_SHIFT))

#define  STMOTOR_REG_CMD_ACC_DIR_MASK 0x10
#define  STMOTOR_REG_CMD_ACC_DIR_SHIFT 4
#define  STMOTOR_REG_CMD_ACC_DIR_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_ACC_DIR_MASK) >> STMOTOR_REG_CMD_ACC_DIR_SHIFT)
#define  STMOTOR_REG_CMD_ACC_DIR_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_ACC_DIR_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_ACC_DIR_SHIFT))

#define  STMOTOR_REG_CMD_RESERVED3_MASK 0x8
#define  STMOTOR_REG_CMD_RESERVED3_SHIFT 3
#define  STMOTOR_REG_CMD_RESERVED3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_RESERVED3_MASK) >> STMOTOR_REG_CMD_RESERVED3_SHIFT)
#define  STMOTOR_REG_CMD_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_RESERVED3_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_RESERVED3_SHIFT))

#define  STMOTOR_REG_CMD_CMD_MASK 0x7
#define  STMOTOR_REG_CMD_CMD_SHIFT 0
#define  STMOTOR_REG_CMD_CMD_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_CMD_MASK) >> STMOTOR_REG_CMD_CMD_SHIFT)
#define  STMOTOR_REG_CMD_CMD_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_CMD_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_CMD_SHIFT))

//====================================================================
//Register: Command Issue Register (CMD_ISSUE)
/** \brief Writing the register causes a command to be issued to the execution FIFO*/
//====================================================================

#define  STMOTOR_REG_CMD_ISSUE_RESERVED1_MASK 0xfffffffe
#define  STMOTOR_REG_CMD_ISSUE_RESERVED1_SHIFT 1
#define  STMOTOR_REG_CMD_ISSUE_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_ISSUE_RESERVED1_MASK) >> STMOTOR_REG_CMD_ISSUE_RESERVED1_SHIFT)
#define  STMOTOR_REG_CMD_ISSUE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_ISSUE_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_ISSUE_RESERVED1_SHIFT))

#define  STMOTOR_REG_CMD_ISSUE_ISSUE_MASK 0x1
#define  STMOTOR_REG_CMD_ISSUE_ISSUE_SHIFT 0
#define  STMOTOR_REG_CMD_ISSUE_ISSUE_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_CMD_ISSUE_ISSUE_MASK) >> STMOTOR_REG_CMD_ISSUE_ISSUE_SHIFT)
#define  STMOTOR_REG_CMD_ISSUE_ISSUE_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_CMD_ISSUE_ISSUE_MASK) | (((uint32_t)val) << STMOTOR_REG_CMD_ISSUE_ISSUE_SHIFT))

//====================================================================
//Register: Status Register (COM_STAT)
/** \brief Status register*/
//====================================================================

#define  STMOTOR_REG_COM_STAT_RESERVED1_MASK 0xfffffe00
#define  STMOTOR_REG_COM_STAT_RESERVED1_SHIFT 9
#define  STMOTOR_REG_COM_STAT_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STAT_RESERVED1_MASK) >> STMOTOR_REG_COM_STAT_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_STAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STAT_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STAT_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_STAT_EXEC_MASK 0x100
#define  STMOTOR_REG_COM_STAT_EXEC_SHIFT 8
#define  STMOTOR_REG_COM_STAT_EXEC_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STAT_EXEC_MASK) >> STMOTOR_REG_COM_STAT_EXEC_SHIFT)
#define  STMOTOR_REG_COM_STAT_EXEC_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STAT_EXEC_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STAT_EXEC_SHIFT))

#define  STMOTOR_REG_COM_STAT_RESERVED2_MASK 0xf0
#define  STMOTOR_REG_COM_STAT_RESERVED2_SHIFT 4
#define  STMOTOR_REG_COM_STAT_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STAT_RESERVED2_MASK) >> STMOTOR_REG_COM_STAT_RESERVED2_SHIFT)
#define  STMOTOR_REG_COM_STAT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STAT_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STAT_RESERVED2_SHIFT))

#define  STMOTOR_REG_COM_STAT_FULL_MASK 0x8
#define  STMOTOR_REG_COM_STAT_FULL_SHIFT 3
#define  STMOTOR_REG_COM_STAT_FULL_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STAT_FULL_MASK) >> STMOTOR_REG_COM_STAT_FULL_SHIFT)
#define  STMOTOR_REG_COM_STAT_FULL_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STAT_FULL_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STAT_FULL_SHIFT))

#define  STMOTOR_REG_COM_STAT_ALMOST_FULL_MASK 0x4
#define  STMOTOR_REG_COM_STAT_ALMOST_FULL_SHIFT 2
#define  STMOTOR_REG_COM_STAT_ALMOST_FULL_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STAT_ALMOST_FULL_MASK) >> STMOTOR_REG_COM_STAT_ALMOST_FULL_SHIFT)
#define  STMOTOR_REG_COM_STAT_ALMOST_FULL_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STAT_ALMOST_FULL_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STAT_ALMOST_FULL_SHIFT))

#define  STMOTOR_REG_COM_STAT_ALMOST_EMPTY_MASK 0x2
#define  STMOTOR_REG_COM_STAT_ALMOST_EMPTY_SHIFT 1
#define  STMOTOR_REG_COM_STAT_ALMOST_EMPTY_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STAT_ALMOST_EMPTY_MASK) >> STMOTOR_REG_COM_STAT_ALMOST_EMPTY_SHIFT)
#define  STMOTOR_REG_COM_STAT_ALMOST_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STAT_ALMOST_EMPTY_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STAT_ALMOST_EMPTY_SHIFT))

#define  STMOTOR_REG_COM_STAT_EMPTY_MASK 0x1
#define  STMOTOR_REG_COM_STAT_EMPTY_SHIFT 0
#define  STMOTOR_REG_COM_STAT_EMPTY_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STAT_EMPTY_MASK) >> STMOTOR_REG_COM_STAT_EMPTY_SHIFT)
#define  STMOTOR_REG_COM_STAT_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STAT_EMPTY_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STAT_EMPTY_SHIFT))

//====================================================================
//Register: Current PWM_P Value (COM_PWM_P)
/** \brief Current PWM P value.*/
//====================================================================

#define  STMOTOR_REG_COM_PWM_P_RESERVED1_MASK 0xff000000
#define  STMOTOR_REG_COM_PWM_P_RESERVED1_SHIFT 24
#define  STMOTOR_REG_COM_PWM_P_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_PWM_P_RESERVED1_MASK) >> STMOTOR_REG_COM_PWM_P_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_PWM_P_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_PWM_P_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_PWM_P_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_PWM_P_PWM_P_MASK 0xffffff
#define  STMOTOR_REG_COM_PWM_P_PWM_P_SHIFT 0
#define  STMOTOR_REG_COM_PWM_P_PWM_P_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_PWM_P_PWM_P_MASK) >> STMOTOR_REG_COM_PWM_P_PWM_P_SHIFT)
#define  STMOTOR_REG_COM_PWM_P_PWM_P_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_PWM_P_PWM_P_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_PWM_P_PWM_P_SHIFT))

//====================================================================
//Register: Current Acceleration Increment (COM_ACC_INCR)
/** \brief Acceleration Increment.*/
//====================================================================

#define  STMOTOR_REG_COM_ACC_INCR_RESERVED1_MASK 0xffffff00
#define  STMOTOR_REG_COM_ACC_INCR_RESERVED1_SHIFT 8
#define  STMOTOR_REG_COM_ACC_INCR_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_ACC_INCR_RESERVED1_MASK) >> STMOTOR_REG_COM_ACC_INCR_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_ACC_INCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_ACC_INCR_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_ACC_INCR_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_ACC_INCR_ACC_INCR_MASK 0xff
#define  STMOTOR_REG_COM_ACC_INCR_ACC_INCR_SHIFT 0
#define  STMOTOR_REG_COM_ACC_INCR_ACC_INCR_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_ACC_INCR_ACC_INCR_MASK) >> STMOTOR_REG_COM_ACC_INCR_ACC_INCR_SHIFT)
#define  STMOTOR_REG_COM_ACC_INCR_ACC_INCR_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_ACC_INCR_ACC_INCR_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_ACC_INCR_ACC_INCR_SHIFT))

//====================================================================
//Register: Current PWM_M Value (COM_PWM_M)
/** \brief PWM M value*/
//====================================================================

#define  STMOTOR_REG_COM_PWM_M_RESERVED1_MASK 0xff000000
#define  STMOTOR_REG_COM_PWM_M_RESERVED1_SHIFT 24
#define  STMOTOR_REG_COM_PWM_M_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_PWM_M_RESERVED1_MASK) >> STMOTOR_REG_COM_PWM_M_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_PWM_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_PWM_M_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_PWM_M_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_PWM_M_PWM_M_MASK 0xffffff
#define  STMOTOR_REG_COM_PWM_M_PWM_M_SHIFT 0
#define  STMOTOR_REG_COM_PWM_M_PWM_M_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_PWM_M_PWM_M_MASK) >> STMOTOR_REG_COM_PWM_M_PWM_M_SHIFT)
#define  STMOTOR_REG_COM_PWM_M_PWM_M_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_PWM_M_PWM_M_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_PWM_M_PWM_M_SHIFT))

//====================================================================
//Register: Current Command Value (COM_CMD)
/** \brief Current command being run.*/
//====================================================================

#define  STMOTOR_REG_COM_CMD_ST_TGT_MASK 0xffff0000
#define  STMOTOR_REG_COM_CMD_ST_TGT_SHIFT 16
#define  STMOTOR_REG_COM_CMD_ST_TGT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_ST_TGT_MASK) >> STMOTOR_REG_COM_CMD_ST_TGT_SHIFT)
#define  STMOTOR_REG_COM_CMD_ST_TGT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_ST_TGT_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_ST_TGT_SHIFT))

#define  STMOTOR_REG_COM_CMD_INF_MASK 0x8000
#define  STMOTOR_REG_COM_CMD_INF_SHIFT 15
#define  STMOTOR_REG_COM_CMD_INF_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_INF_MASK) >> STMOTOR_REG_COM_CMD_INF_SHIFT)
#define  STMOTOR_REG_COM_CMD_INF_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_INF_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_INF_SHIFT))

#define  STMOTOR_REG_COM_CMD_RESERVED1_MASK 0x6000
#define  STMOTOR_REG_COM_CMD_RESERVED1_SHIFT 13
#define  STMOTOR_REG_COM_CMD_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_RESERVED1_MASK) >> STMOTOR_REG_COM_CMD_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_CMD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_CMD_LS_EN_MASK 0x1000
#define  STMOTOR_REG_COM_CMD_LS_EN_SHIFT 12
#define  STMOTOR_REG_COM_CMD_LS_EN_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_LS_EN_MASK) >> STMOTOR_REG_COM_CMD_LS_EN_SHIFT)
#define  STMOTOR_REG_COM_CMD_LS_EN_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_LS_EN_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_LS_EN_SHIFT))

#define  STMOTOR_REG_COM_CMD_RESERVED2_MASK 0xfc0
#define  STMOTOR_REG_COM_CMD_RESERVED2_SHIFT 6
#define  STMOTOR_REG_COM_CMD_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_RESERVED2_MASK) >> STMOTOR_REG_COM_CMD_RESERVED2_SHIFT)
#define  STMOTOR_REG_COM_CMD_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_RESERVED2_SHIFT))

#define  STMOTOR_REG_COM_CMD_DIR_MASK 0x20
#define  STMOTOR_REG_COM_CMD_DIR_SHIFT 5
#define  STMOTOR_REG_COM_CMD_DIR_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_DIR_MASK) >> STMOTOR_REG_COM_CMD_DIR_SHIFT)
#define  STMOTOR_REG_COM_CMD_DIR_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_DIR_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_DIR_SHIFT))

#define  STMOTOR_REG_COM_CMD_ACC_DIR_MASK 0x10
#define  STMOTOR_REG_COM_CMD_ACC_DIR_SHIFT 4
#define  STMOTOR_REG_COM_CMD_ACC_DIR_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_ACC_DIR_MASK) >> STMOTOR_REG_COM_CMD_ACC_DIR_SHIFT)
#define  STMOTOR_REG_COM_CMD_ACC_DIR_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_ACC_DIR_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_ACC_DIR_SHIFT))

#define  STMOTOR_REG_COM_CMD_RESERVED3_MASK 0x8
#define  STMOTOR_REG_COM_CMD_RESERVED3_SHIFT 3
#define  STMOTOR_REG_COM_CMD_RESERVED3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_RESERVED3_MASK) >> STMOTOR_REG_COM_CMD_RESERVED3_SHIFT)
#define  STMOTOR_REG_COM_CMD_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_RESERVED3_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_RESERVED3_SHIFT))

#define  STMOTOR_REG_COM_CMD_CMD_MASK 0x7
#define  STMOTOR_REG_COM_CMD_CMD_SHIFT 0
#define  STMOTOR_REG_COM_CMD_CMD_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_CMD_CMD_MASK) >> STMOTOR_REG_COM_CMD_CMD_SHIFT)
#define  STMOTOR_REG_COM_CMD_CMD_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_CMD_CMD_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_CMD_CMD_SHIFT))

//====================================================================
//Register: Step Count (COM_STP)
/** \brief Current command.*/
//====================================================================

#define  STMOTOR_REG_COM_STP_RESERVED1_MASK 0xff000000
#define  STMOTOR_REG_COM_STP_RESERVED1_SHIFT 24
#define  STMOTOR_REG_COM_STP_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STP_RESERVED1_MASK) >> STMOTOR_REG_COM_STP_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_STP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STP_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STP_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_STP_STEP_CNT_MASK 0xffffff
#define  STMOTOR_REG_COM_STP_STEP_CNT_SHIFT 0
#define  STMOTOR_REG_COM_STP_STEP_CNT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_STP_STEP_CNT_MASK) >> STMOTOR_REG_COM_STP_STEP_CNT_SHIFT)
#define  STMOTOR_REG_COM_STP_STEP_CNT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_STP_STEP_CNT_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_STP_STEP_CNT_SHIFT))

//====================================================================
//Register: TVT Index (COM_TVT)
/** \brief Current TVT Index pointers.*/
//====================================================================

#define  STMOTOR_REG_COM_TVT_RESERVED1_MASK 0xfffff000
#define  STMOTOR_REG_COM_TVT_RESERVED1_SHIFT 12
#define  STMOTOR_REG_COM_TVT_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_TVT_RESERVED1_MASK) >> STMOTOR_REG_COM_TVT_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_TVT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_TVT_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_TVT_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_TVT_PTR_A_MASK 0xf00
#define  STMOTOR_REG_COM_TVT_PTR_A_SHIFT 8
#define  STMOTOR_REG_COM_TVT_PTR_A_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_TVT_PTR_A_MASK) >> STMOTOR_REG_COM_TVT_PTR_A_SHIFT)
#define  STMOTOR_REG_COM_TVT_PTR_A_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_TVT_PTR_A_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_TVT_PTR_A_SHIFT))

#define  STMOTOR_REG_COM_TVT_RESERVED2_MASK 0xf0
#define  STMOTOR_REG_COM_TVT_RESERVED2_SHIFT 4
#define  STMOTOR_REG_COM_TVT_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_TVT_RESERVED2_MASK) >> STMOTOR_REG_COM_TVT_RESERVED2_SHIFT)
#define  STMOTOR_REG_COM_TVT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_TVT_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_TVT_RESERVED2_SHIFT))

#define  STMOTOR_REG_COM_TVT_PTR_B_MASK 0xf
#define  STMOTOR_REG_COM_TVT_PTR_B_SHIFT 0
#define  STMOTOR_REG_COM_TVT_PTR_B_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_TVT_PTR_B_MASK) >> STMOTOR_REG_COM_TVT_PTR_B_SHIFT)
#define  STMOTOR_REG_COM_TVT_PTR_B_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_TVT_PTR_B_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_TVT_PTR_B_SHIFT))

//====================================================================
//Register: Sequence Index (COM_SEQ)
/** \brief Current Sequence pointer.*/
//====================================================================

#define  STMOTOR_REG_COM_SEQ_RESERVED1_MASK 0xffffffe0
#define  STMOTOR_REG_COM_SEQ_RESERVED1_SHIFT 5
#define  STMOTOR_REG_COM_SEQ_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_SEQ_RESERVED1_MASK) >> STMOTOR_REG_COM_SEQ_RESERVED1_SHIFT)
#define  STMOTOR_REG_COM_SEQ_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_SEQ_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_SEQ_RESERVED1_SHIFT))

#define  STMOTOR_REG_COM_SEQ_PTR_SEQ_MASK 0x1f
#define  STMOTOR_REG_COM_SEQ_PTR_SEQ_SHIFT 0
#define  STMOTOR_REG_COM_SEQ_PTR_SEQ_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_COM_SEQ_PTR_SEQ_MASK) >> STMOTOR_REG_COM_SEQ_PTR_SEQ_SHIFT)
#define  STMOTOR_REG_COM_SEQ_PTR_SEQ_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_COM_SEQ_PTR_SEQ_MASK) | (((uint32_t)val) << STMOTOR_REG_COM_SEQ_PTR_SEQ_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (I_EN)
/** \brief Interrupt Enable.*/
//====================================================================

#define  STMOTOR_REG_I_EN_RESERVED1_MASK 0xfffffe00
#define  STMOTOR_REG_I_EN_RESERVED1_SHIFT 9
#define  STMOTOR_REG_I_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_RESERVED1_MASK) >> STMOTOR_REG_I_EN_RESERVED1_SHIFT)
#define  STMOTOR_REG_I_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_RESERVED1_SHIFT))

#define  STMOTOR_REG_I_EN_CMD_MASK 0x100
#define  STMOTOR_REG_I_EN_CMD_SHIFT 8
#define  STMOTOR_REG_I_EN_CMD_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_CMD_MASK) >> STMOTOR_REG_I_EN_CMD_SHIFT)
#define  STMOTOR_REG_I_EN_CMD_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_CMD_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_CMD_SHIFT))

#define  STMOTOR_REG_I_EN_STEP_INT_MASK 0x80
#define  STMOTOR_REG_I_EN_STEP_INT_SHIFT 7
#define  STMOTOR_REG_I_EN_STEP_INT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_STEP_INT_MASK) >> STMOTOR_REG_I_EN_STEP_INT_SHIFT)
#define  STMOTOR_REG_I_EN_STEP_INT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_STEP_INT_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_STEP_INT_SHIFT))

#define  STMOTOR_REG_I_EN_FIFO_OVERFLOW_MASK 0x40
#define  STMOTOR_REG_I_EN_FIFO_OVERFLOW_SHIFT 6
#define  STMOTOR_REG_I_EN_FIFO_OVERFLOW_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_FIFO_OVERFLOW_MASK) >> STMOTOR_REG_I_EN_FIFO_OVERFLOW_SHIFT)
#define  STMOTOR_REG_I_EN_FIFO_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_FIFO_OVERFLOW_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_FIFO_OVERFLOW_SHIFT))

#define  STMOTOR_REG_I_EN_I_OMODE_MASK 0x20
#define  STMOTOR_REG_I_EN_I_OMODE_SHIFT 5
#define  STMOTOR_REG_I_EN_I_OMODE_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_I_OMODE_MASK) >> STMOTOR_REG_I_EN_I_OMODE_SHIFT)
#define  STMOTOR_REG_I_EN_I_OMODE_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_I_OMODE_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_I_OMODE_SHIFT))

#define  STMOTOR_REG_I_EN_I_USTEPS_MASK 0x10
#define  STMOTOR_REG_I_EN_I_USTEPS_SHIFT 4
#define  STMOTOR_REG_I_EN_I_USTEPS_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_I_USTEPS_MASK) >> STMOTOR_REG_I_EN_I_USTEPS_SHIFT)
#define  STMOTOR_REG_I_EN_I_USTEPS_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_I_USTEPS_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_I_USTEPS_SHIFT))

#define  STMOTOR_REG_I_EN_STCNT_UO_MASK 0x8
#define  STMOTOR_REG_I_EN_STCNT_UO_SHIFT 3
#define  STMOTOR_REG_I_EN_STCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_STCNT_UO_MASK) >> STMOTOR_REG_I_EN_STCNT_UO_SHIFT)
#define  STMOTOR_REG_I_EN_STCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_STCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_STCNT_UO_SHIFT))

#define  STMOTOR_REG_I_EN_I_INST_MASK 0x4
#define  STMOTOR_REG_I_EN_I_INST_SHIFT 2
#define  STMOTOR_REG_I_EN_I_INST_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_I_INST_MASK) >> STMOTOR_REG_I_EN_I_INST_SHIFT)
#define  STMOTOR_REG_I_EN_I_INST_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_I_INST_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_I_INST_SHIFT))

#define  STMOTOR_REG_I_EN_PCNT_UO_MASK 0x2
#define  STMOTOR_REG_I_EN_PCNT_UO_SHIFT 1
#define  STMOTOR_REG_I_EN_PCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_PCNT_UO_MASK) >> STMOTOR_REG_I_EN_PCNT_UO_SHIFT)
#define  STMOTOR_REG_I_EN_PCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_PCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_PCNT_UO_SHIFT))

#define  STMOTOR_REG_I_EN_MCNT_UO_MASK 0x1
#define  STMOTOR_REG_I_EN_MCNT_UO_SHIFT 0
#define  STMOTOR_REG_I_EN_MCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_EN_MCNT_UO_MASK) >> STMOTOR_REG_I_EN_MCNT_UO_SHIFT)
#define  STMOTOR_REG_I_EN_MCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_EN_MCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_EN_MCNT_UO_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (I_PEND)
/** \brief Interrupt Pending*/
//====================================================================

#define  STMOTOR_REG_I_PEND_RESERVED1_MASK 0xfffffe00
#define  STMOTOR_REG_I_PEND_RESERVED1_SHIFT 9
#define  STMOTOR_REG_I_PEND_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_RESERVED1_MASK) >> STMOTOR_REG_I_PEND_RESERVED1_SHIFT)
#define  STMOTOR_REG_I_PEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_RESERVED1_SHIFT))

#define  STMOTOR_REG_I_PEND_CMD_MASK 0x100
#define  STMOTOR_REG_I_PEND_CMD_SHIFT 8
#define  STMOTOR_REG_I_PEND_CMD_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_CMD_MASK) >> STMOTOR_REG_I_PEND_CMD_SHIFT)
#define  STMOTOR_REG_I_PEND_CMD_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_CMD_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_CMD_SHIFT))

#define  STMOTOR_REG_I_PEND_STEP_INT_MASK 0x80
#define  STMOTOR_REG_I_PEND_STEP_INT_SHIFT 7
#define  STMOTOR_REG_I_PEND_STEP_INT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_STEP_INT_MASK) >> STMOTOR_REG_I_PEND_STEP_INT_SHIFT)
#define  STMOTOR_REG_I_PEND_STEP_INT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_STEP_INT_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_STEP_INT_SHIFT))

#define  STMOTOR_REG_I_PEND_FIFO_OVERFLOW_MASK 0x40
#define  STMOTOR_REG_I_PEND_FIFO_OVERFLOW_SHIFT 6
#define  STMOTOR_REG_I_PEND_FIFO_OVERFLOW_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_FIFO_OVERFLOW_MASK) >> STMOTOR_REG_I_PEND_FIFO_OVERFLOW_SHIFT)
#define  STMOTOR_REG_I_PEND_FIFO_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_FIFO_OVERFLOW_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_FIFO_OVERFLOW_SHIFT))

#define  STMOTOR_REG_I_PEND_I_OMODE_MASK 0x20
#define  STMOTOR_REG_I_PEND_I_OMODE_SHIFT 5
#define  STMOTOR_REG_I_PEND_I_OMODE_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_I_OMODE_MASK) >> STMOTOR_REG_I_PEND_I_OMODE_SHIFT)
#define  STMOTOR_REG_I_PEND_I_OMODE_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_I_OMODE_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_I_OMODE_SHIFT))

#define  STMOTOR_REG_I_PEND_I_USTEPS_MASK 0x10
#define  STMOTOR_REG_I_PEND_I_USTEPS_SHIFT 4
#define  STMOTOR_REG_I_PEND_I_USTEPS_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_I_USTEPS_MASK) >> STMOTOR_REG_I_PEND_I_USTEPS_SHIFT)
#define  STMOTOR_REG_I_PEND_I_USTEPS_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_I_USTEPS_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_I_USTEPS_SHIFT))

#define  STMOTOR_REG_I_PEND_STCNT_UO_MASK 0x8
#define  STMOTOR_REG_I_PEND_STCNT_UO_SHIFT 3
#define  STMOTOR_REG_I_PEND_STCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_STCNT_UO_MASK) >> STMOTOR_REG_I_PEND_STCNT_UO_SHIFT)
#define  STMOTOR_REG_I_PEND_STCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_STCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_STCNT_UO_SHIFT))

#define  STMOTOR_REG_I_PEND_I_INST_MASK 0x4
#define  STMOTOR_REG_I_PEND_I_INST_SHIFT 2
#define  STMOTOR_REG_I_PEND_I_INST_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_I_INST_MASK) >> STMOTOR_REG_I_PEND_I_INST_SHIFT)
#define  STMOTOR_REG_I_PEND_I_INST_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_I_INST_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_I_INST_SHIFT))

#define  STMOTOR_REG_I_PEND_PCNT_UO_MASK 0x2
#define  STMOTOR_REG_I_PEND_PCNT_UO_SHIFT 1
#define  STMOTOR_REG_I_PEND_PCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_PCNT_UO_MASK) >> STMOTOR_REG_I_PEND_PCNT_UO_SHIFT)
#define  STMOTOR_REG_I_PEND_PCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_PCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_PCNT_UO_SHIFT))

#define  STMOTOR_REG_I_PEND_MCNT_UO_MASK 0x1
#define  STMOTOR_REG_I_PEND_MCNT_UO_SHIFT 0
#define  STMOTOR_REG_I_PEND_MCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_PEND_MCNT_UO_MASK) >> STMOTOR_REG_I_PEND_MCNT_UO_SHIFT)
#define  STMOTOR_REG_I_PEND_MCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_PEND_MCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_PEND_MCNT_UO_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (I_ACK)
/** \brief Interrupt Acknowledge.*/
//====================================================================

#define  STMOTOR_REG_I_ACK_RESERVED1_MASK 0xfffffe00
#define  STMOTOR_REG_I_ACK_RESERVED1_SHIFT 9
#define  STMOTOR_REG_I_ACK_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_RESERVED1_MASK) >> STMOTOR_REG_I_ACK_RESERVED1_SHIFT)
#define  STMOTOR_REG_I_ACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_RESERVED1_SHIFT))

#define  STMOTOR_REG_I_ACK_CMD_MASK 0x100
#define  STMOTOR_REG_I_ACK_CMD_SHIFT 8
#define  STMOTOR_REG_I_ACK_CMD_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_CMD_MASK) >> STMOTOR_REG_I_ACK_CMD_SHIFT)
#define  STMOTOR_REG_I_ACK_CMD_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_CMD_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_CMD_SHIFT))

#define  STMOTOR_REG_I_ACK_STEP_INT_MASK 0x80
#define  STMOTOR_REG_I_ACK_STEP_INT_SHIFT 7
#define  STMOTOR_REG_I_ACK_STEP_INT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_STEP_INT_MASK) >> STMOTOR_REG_I_ACK_STEP_INT_SHIFT)
#define  STMOTOR_REG_I_ACK_STEP_INT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_STEP_INT_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_STEP_INT_SHIFT))

#define  STMOTOR_REG_I_ACK_FIFO_OVERFLOW_MASK 0x40
#define  STMOTOR_REG_I_ACK_FIFO_OVERFLOW_SHIFT 6
#define  STMOTOR_REG_I_ACK_FIFO_OVERFLOW_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_FIFO_OVERFLOW_MASK) >> STMOTOR_REG_I_ACK_FIFO_OVERFLOW_SHIFT)
#define  STMOTOR_REG_I_ACK_FIFO_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_FIFO_OVERFLOW_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_FIFO_OVERFLOW_SHIFT))

#define  STMOTOR_REG_I_ACK_I_OMODE_MASK 0x20
#define  STMOTOR_REG_I_ACK_I_OMODE_SHIFT 5
#define  STMOTOR_REG_I_ACK_I_OMODE_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_I_OMODE_MASK) >> STMOTOR_REG_I_ACK_I_OMODE_SHIFT)
#define  STMOTOR_REG_I_ACK_I_OMODE_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_I_OMODE_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_I_OMODE_SHIFT))

#define  STMOTOR_REG_I_ACK_I_USTEPS_MASK 0x10
#define  STMOTOR_REG_I_ACK_I_USTEPS_SHIFT 4
#define  STMOTOR_REG_I_ACK_I_USTEPS_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_I_USTEPS_MASK) >> STMOTOR_REG_I_ACK_I_USTEPS_SHIFT)
#define  STMOTOR_REG_I_ACK_I_USTEPS_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_I_USTEPS_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_I_USTEPS_SHIFT))

#define  STMOTOR_REG_I_ACK_STCNT_UO_MASK 0x8
#define  STMOTOR_REG_I_ACK_STCNT_UO_SHIFT 3
#define  STMOTOR_REG_I_ACK_STCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_STCNT_UO_MASK) >> STMOTOR_REG_I_ACK_STCNT_UO_SHIFT)
#define  STMOTOR_REG_I_ACK_STCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_STCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_STCNT_UO_SHIFT))

#define  STMOTOR_REG_I_ACK_I_INST_MASK 0x4
#define  STMOTOR_REG_I_ACK_I_INST_SHIFT 2
#define  STMOTOR_REG_I_ACK_I_INST_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_I_INST_MASK) >> STMOTOR_REG_I_ACK_I_INST_SHIFT)
#define  STMOTOR_REG_I_ACK_I_INST_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_I_INST_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_I_INST_SHIFT))

#define  STMOTOR_REG_I_ACK_PCNT_UO_MASK 0x2
#define  STMOTOR_REG_I_ACK_PCNT_UO_SHIFT 1
#define  STMOTOR_REG_I_ACK_PCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_PCNT_UO_MASK) >> STMOTOR_REG_I_ACK_PCNT_UO_SHIFT)
#define  STMOTOR_REG_I_ACK_PCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_PCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_PCNT_UO_SHIFT))

#define  STMOTOR_REG_I_ACK_MCNT_UO_MASK 0x1
#define  STMOTOR_REG_I_ACK_MCNT_UO_SHIFT 0
#define  STMOTOR_REG_I_ACK_MCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_ACK_MCNT_UO_MASK) >> STMOTOR_REG_I_ACK_MCNT_UO_SHIFT)
#define  STMOTOR_REG_I_ACK_MCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_ACK_MCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_ACK_MCNT_UO_SHIFT))

//====================================================================
//Register: Interrupt Force Register (I_FORCE)
/** \brief Interrupt Force.*/
//====================================================================

#define  STMOTOR_REG_I_FORCE_RESERVED1_MASK 0xfffffe00
#define  STMOTOR_REG_I_FORCE_RESERVED1_SHIFT 9
#define  STMOTOR_REG_I_FORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_RESERVED1_MASK) >> STMOTOR_REG_I_FORCE_RESERVED1_SHIFT)
#define  STMOTOR_REG_I_FORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_RESERVED1_SHIFT))

#define  STMOTOR_REG_I_FORCE_CMD_MASK 0x100
#define  STMOTOR_REG_I_FORCE_CMD_SHIFT 8
#define  STMOTOR_REG_I_FORCE_CMD_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_CMD_MASK) >> STMOTOR_REG_I_FORCE_CMD_SHIFT)
#define  STMOTOR_REG_I_FORCE_CMD_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_CMD_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_CMD_SHIFT))

#define  STMOTOR_REG_I_FORCE_STEP_INT_MASK 0x80
#define  STMOTOR_REG_I_FORCE_STEP_INT_SHIFT 7
#define  STMOTOR_REG_I_FORCE_STEP_INT_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_STEP_INT_MASK) >> STMOTOR_REG_I_FORCE_STEP_INT_SHIFT)
#define  STMOTOR_REG_I_FORCE_STEP_INT_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_STEP_INT_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_STEP_INT_SHIFT))

#define  STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_MASK 0x40
#define  STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_SHIFT 6
#define  STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_MASK) >> STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_SHIFT)
#define  STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_FIFO_OVERFLOW_SHIFT))

#define  STMOTOR_REG_I_FORCE_I_OMODE_MASK 0x20
#define  STMOTOR_REG_I_FORCE_I_OMODE_SHIFT 5
#define  STMOTOR_REG_I_FORCE_I_OMODE_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_I_OMODE_MASK) >> STMOTOR_REG_I_FORCE_I_OMODE_SHIFT)
#define  STMOTOR_REG_I_FORCE_I_OMODE_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_I_OMODE_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_I_OMODE_SHIFT))

#define  STMOTOR_REG_I_FORCE_I_USTEPS_MASK 0x10
#define  STMOTOR_REG_I_FORCE_I_USTEPS_SHIFT 4
#define  STMOTOR_REG_I_FORCE_I_USTEPS_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_I_USTEPS_MASK) >> STMOTOR_REG_I_FORCE_I_USTEPS_SHIFT)
#define  STMOTOR_REG_I_FORCE_I_USTEPS_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_I_USTEPS_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_I_USTEPS_SHIFT))

#define  STMOTOR_REG_I_FORCE_STCNT_UO_MASK 0x8
#define  STMOTOR_REG_I_FORCE_STCNT_UO_SHIFT 3
#define  STMOTOR_REG_I_FORCE_STCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_STCNT_UO_MASK) >> STMOTOR_REG_I_FORCE_STCNT_UO_SHIFT)
#define  STMOTOR_REG_I_FORCE_STCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_STCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_STCNT_UO_SHIFT))

#define  STMOTOR_REG_I_FORCE_I_INST_MASK 0x4
#define  STMOTOR_REG_I_FORCE_I_INST_SHIFT 2
#define  STMOTOR_REG_I_FORCE_I_INST_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_I_INST_MASK) >> STMOTOR_REG_I_FORCE_I_INST_SHIFT)
#define  STMOTOR_REG_I_FORCE_I_INST_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_I_INST_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_I_INST_SHIFT))

#define  STMOTOR_REG_I_FORCE_PCNT_UO_MASK 0x2
#define  STMOTOR_REG_I_FORCE_PCNT_UO_SHIFT 1
#define  STMOTOR_REG_I_FORCE_PCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_PCNT_UO_MASK) >> STMOTOR_REG_I_FORCE_PCNT_UO_SHIFT)
#define  STMOTOR_REG_I_FORCE_PCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_PCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_PCNT_UO_SHIFT))

#define  STMOTOR_REG_I_FORCE_MCNT_UO_MASK 0x1
#define  STMOTOR_REG_I_FORCE_MCNT_UO_SHIFT 0
#define  STMOTOR_REG_I_FORCE_MCNT_UO_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_I_FORCE_MCNT_UO_MASK) >> STMOTOR_REG_I_FORCE_MCNT_UO_SHIFT)
#define  STMOTOR_REG_I_FORCE_MCNT_UO_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_I_FORCE_MCNT_UO_MASK) | (((uint32_t)val) << STMOTOR_REG_I_FORCE_MCNT_UO_SHIFT))

//====================================================================
//Register: Trigger Configuration (SMC_TRIG)
/** \brief This register is used to configure the output trigger; there is a configuration field for each of the SMC outputs. The following table describes the bit encoding:
<table align="center"> <tr><th>cfg[1:0]</th> <th>Trigger Type</th></tr> <tr><td>'00'</td><td>Positive Edge</td></tr> <tr><td>'01'</td><td>Negative Edge</td></tr> <tr><td>'10'</td><td>Both Edges</td></tr> <tr><td>'11'</td><td>Disabled</td></tr></table>*/
//====================================================================

#define  STMOTOR_REG_SMC_TRIG_RESERVED1_MASK 0xffc00000
#define  STMOTOR_REG_SMC_TRIG_RESERVED1_SHIFT 22
#define  STMOTOR_REG_SMC_TRIG_RESERVED1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_RESERVED1_MASK) >> STMOTOR_REG_SMC_TRIG_RESERVED1_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_RESERVED1_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_RESERVED1_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_SMC5_MASK 0x300000
#define  STMOTOR_REG_SMC_TRIG_SMC5_SHIFT 20
#define  STMOTOR_REG_SMC_TRIG_SMC5_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_SMC5_MASK) >> STMOTOR_REG_SMC_TRIG_SMC5_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_SMC5_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_SMC5_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_SMC5_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_RESERVED2_MASK 0xc0000
#define  STMOTOR_REG_SMC_TRIG_RESERVED2_SHIFT 18
#define  STMOTOR_REG_SMC_TRIG_RESERVED2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_RESERVED2_MASK) >> STMOTOR_REG_SMC_TRIG_RESERVED2_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_RESERVED2_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_RESERVED2_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_SMC4_MASK 0x30000
#define  STMOTOR_REG_SMC_TRIG_SMC4_SHIFT 16
#define  STMOTOR_REG_SMC_TRIG_SMC4_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_SMC4_MASK) >> STMOTOR_REG_SMC_TRIG_SMC4_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_SMC4_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_SMC4_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_SMC4_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_RESERVED3_MASK 0xc000
#define  STMOTOR_REG_SMC_TRIG_RESERVED3_SHIFT 14
#define  STMOTOR_REG_SMC_TRIG_RESERVED3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_RESERVED3_MASK) >> STMOTOR_REG_SMC_TRIG_RESERVED3_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_RESERVED3_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_RESERVED3_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_SMC3_MASK 0x3000
#define  STMOTOR_REG_SMC_TRIG_SMC3_SHIFT 12
#define  STMOTOR_REG_SMC_TRIG_SMC3_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_SMC3_MASK) >> STMOTOR_REG_SMC_TRIG_SMC3_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_SMC3_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_SMC3_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_SMC3_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_RESERVED4_MASK 0xc00
#define  STMOTOR_REG_SMC_TRIG_RESERVED4_SHIFT 10
#define  STMOTOR_REG_SMC_TRIG_RESERVED4_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_RESERVED4_MASK) >> STMOTOR_REG_SMC_TRIG_RESERVED4_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_RESERVED4_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_RESERVED4_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_SMC2_MASK 0x300
#define  STMOTOR_REG_SMC_TRIG_SMC2_SHIFT 8
#define  STMOTOR_REG_SMC_TRIG_SMC2_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_SMC2_MASK) >> STMOTOR_REG_SMC_TRIG_SMC2_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_SMC2_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_SMC2_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_SMC2_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_RESERVED5_MASK 0xc0
#define  STMOTOR_REG_SMC_TRIG_RESERVED5_SHIFT 6
#define  STMOTOR_REG_SMC_TRIG_RESERVED5_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_RESERVED5_MASK) >> STMOTOR_REG_SMC_TRIG_RESERVED5_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_RESERVED5_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_RESERVED5_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_SMC1_MASK 0x30
#define  STMOTOR_REG_SMC_TRIG_SMC1_SHIFT 4
#define  STMOTOR_REG_SMC_TRIG_SMC1_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_SMC1_MASK) >> STMOTOR_REG_SMC_TRIG_SMC1_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_SMC1_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_SMC1_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_SMC1_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_RESERVED6_MASK 0xc
#define  STMOTOR_REG_SMC_TRIG_RESERVED6_SHIFT 2
#define  STMOTOR_REG_SMC_TRIG_RESERVED6_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_RESERVED6_MASK) >> STMOTOR_REG_SMC_TRIG_RESERVED6_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_RESERVED6_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_RESERVED6_SHIFT))

#define  STMOTOR_REG_SMC_TRIG_SMC0_MASK 0x3
#define  STMOTOR_REG_SMC_TRIG_SMC0_SHIFT 0
#define  STMOTOR_REG_SMC_TRIG_SMC0_MASK_SHIFT(reg) (((reg) & STMOTOR_REG_SMC_TRIG_SMC0_MASK) >> STMOTOR_REG_SMC_TRIG_SMC0_SHIFT)
#define  STMOTOR_REG_SMC_TRIG_SMC0_REPLACE_VAL(reg,val) (((reg) & ~STMOTOR_REG_SMC_TRIG_SMC0_MASK) | (((uint32_t)val) << STMOTOR_REG_SMC_TRIG_SMC0_SHIFT))

#endif // STMOTOR_REG
