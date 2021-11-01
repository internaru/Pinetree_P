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
 * \file SCCP_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SCCP_REG_REGMASKS_H_
#define _SCCP_REG_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Serial Control Channel Processor (SCCP_reg)
/** \brief Top-level register file for SCCP (Serial Control Channel Processor) block*/
//
//====================================================================

//====================================================================
//Register: Instruction Pointer Register (IPTR)
/** \brief Instruction Pointer is a 18 bit word offset into the SCCM SRAM space viewed as bits 19:2 by the main CPU. The instruction pointer word offset is incremented by 1 in the SCCP which is seen as increments of 4 by the main CPU.*/
//====================================================================

#define  SCCP_REG_IPTR_RESERVED1_MASK 0xfff00000
#define  SCCP_REG_IPTR_RESERVED1_SHIFT 20
#define  SCCP_REG_IPTR_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_IPTR_RESERVED1_MASK) >> SCCP_REG_IPTR_RESERVED1_SHIFT)
#define  SCCP_REG_IPTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_IPTR_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_IPTR_RESERVED1_SHIFT))

#define  SCCP_REG_IPTR_INSTRPTR_MASK 0xffffc
#define  SCCP_REG_IPTR_INSTRPTR_SHIFT 2
#define  SCCP_REG_IPTR_INSTRPTR_MASK_SHIFT(reg) (((reg) & SCCP_REG_IPTR_INSTRPTR_MASK) >> SCCP_REG_IPTR_INSTRPTR_SHIFT)
#define  SCCP_REG_IPTR_INSTRPTR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_IPTR_INSTRPTR_MASK) | (((uint32_t)val) << SCCP_REG_IPTR_INSTRPTR_SHIFT))

#define  SCCP_REG_IPTR_RESERVED2_MASK 0x3
#define  SCCP_REG_IPTR_RESERVED2_SHIFT 0
#define  SCCP_REG_IPTR_RESERVED2_MASK_SHIFT(reg) (((reg) & SCCP_REG_IPTR_RESERVED2_MASK) >> SCCP_REG_IPTR_RESERVED2_SHIFT)
#define  SCCP_REG_IPTR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_IPTR_RESERVED2_MASK) | (((uint32_t)val) << SCCP_REG_IPTR_RESERVED2_SHIFT))

//====================================================================
//Register: Current Instruction Register (CINST)
/** \brief Current Instruction is a READ-ONLY 32-bit register that contains the current instruction being executed. Current Instruction is made accessible to the main CPU as a debugging aid.*/
//====================================================================

#define  SCCP_REG_CINST_CURINST_MASK 0xffffffff
#define  SCCP_REG_CINST_CURINST_SHIFT 0
#define  SCCP_REG_CINST_CURINST_MASK_SHIFT(reg) (((reg) & SCCP_REG_CINST_CURINST_MASK) >> SCCP_REG_CINST_CURINST_SHIFT)
#define  SCCP_REG_CINST_CURINST_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CINST_CURINST_MASK) | (((uint32_t)val) << SCCP_REG_CINST_CURINST_SHIFT))

//====================================================================
//Register: Stack Pointer (STACK)
/** \brief Stack Pointer is 18 bit word offset into the SCCP SRAM space which points to the current location in the stack. The stack pointer is incremented/decremented by 1 in the SCCP whenever a stack related operation occurs (push, pop, jsr, return). The stack grows upward, so jsr and push will increment the Stack Pointer while return and pop will decrement the Stack Pointer.*/
//====================================================================

#define  SCCP_REG_STACK_RESERVED1_MASK 0xfff00000
#define  SCCP_REG_STACK_RESERVED1_SHIFT 20
#define  SCCP_REG_STACK_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_STACK_RESERVED1_MASK) >> SCCP_REG_STACK_RESERVED1_SHIFT)
#define  SCCP_REG_STACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_STACK_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_STACK_RESERVED1_SHIFT))

#define  SCCP_REG_STACK_STACKPTR_MASK 0xffffc
#define  SCCP_REG_STACK_STACKPTR_SHIFT 2
#define  SCCP_REG_STACK_STACKPTR_MASK_SHIFT(reg) (((reg) & SCCP_REG_STACK_STACKPTR_MASK) >> SCCP_REG_STACK_STACKPTR_SHIFT)
#define  SCCP_REG_STACK_STACKPTR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_STACK_STACKPTR_MASK) | (((uint32_t)val) << SCCP_REG_STACK_STACKPTR_SHIFT))

#define  SCCP_REG_STACK_RESERVED2_MASK 0x3
#define  SCCP_REG_STACK_RESERVED2_SHIFT 0
#define  SCCP_REG_STACK_RESERVED2_MASK_SHIFT(reg) (((reg) & SCCP_REG_STACK_RESERVED2_MASK) >> SCCP_REG_STACK_RESERVED2_SHIFT)
#define  SCCP_REG_STACK_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_STACK_RESERVED2_MASK) | (((uint32_t)val) << SCCP_REG_STACK_RESERVED2_SHIFT))

//====================================================================
//Register: Mode Register (MODE)
/** \brief The Mode Register is used to control and configure the overall operation of SCCP.*/
//====================================================================

#define  SCCP_REG_MODE_RESERVED1_MASK 0xffffe000
#define  SCCP_REG_MODE_RESERVED1_SHIFT 13
#define  SCCP_REG_MODE_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_RESERVED1_MASK) >> SCCP_REG_MODE_RESERVED1_SHIFT)
#define  SCCP_REG_MODE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_MODE_RESERVED1_SHIFT))

#define  SCCP_REG_MODE_STACKERR_MASK 0x1000
#define  SCCP_REG_MODE_STACKERR_SHIFT 12
#define  SCCP_REG_MODE_STACKERR_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_STACKERR_MASK) >> SCCP_REG_MODE_STACKERR_SHIFT)
#define  SCCP_REG_MODE_STACKERR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_STACKERR_MASK) | (((uint32_t)val) << SCCP_REG_MODE_STACKERR_SHIFT))

#define  SCCP_REG_MODE_FLAGS_MASK 0xc00
#define  SCCP_REG_MODE_FLAGS_SHIFT 10
#define  SCCP_REG_MODE_FLAGS_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_FLAGS_MASK) >> SCCP_REG_MODE_FLAGS_SHIFT)
#define  SCCP_REG_MODE_FLAGS_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_FLAGS_MASK) | (((uint32_t)val) << SCCP_REG_MODE_FLAGS_SHIFT))

#define  SCCP_REG_MODE_DMATOMEM_MASK 0x200
#define  SCCP_REG_MODE_DMATOMEM_SHIFT 9
#define  SCCP_REG_MODE_DMATOMEM_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_DMATOMEM_MASK) >> SCCP_REG_MODE_DMATOMEM_SHIFT)
#define  SCCP_REG_MODE_DMATOMEM_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_DMATOMEM_MASK) | (((uint32_t)val) << SCCP_REG_MODE_DMATOMEM_SHIFT))

#define  SCCP_REG_MODE_DMAFROMMEM_MASK 0x100
#define  SCCP_REG_MODE_DMAFROMMEM_SHIFT 8
#define  SCCP_REG_MODE_DMAFROMMEM_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_DMAFROMMEM_MASK) >> SCCP_REG_MODE_DMAFROMMEM_SHIFT)
#define  SCCP_REG_MODE_DMAFROMMEM_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_DMAFROMMEM_MASK) | (((uint32_t)val) << SCCP_REG_MODE_DMAFROMMEM_SHIFT))

#define  SCCP_REG_MODE_RESERVED2_MASK 0xc0
#define  SCCP_REG_MODE_RESERVED2_SHIFT 6
#define  SCCP_REG_MODE_RESERVED2_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_RESERVED2_MASK) >> SCCP_REG_MODE_RESERVED2_SHIFT)
#define  SCCP_REG_MODE_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_RESERVED2_MASK) | (((uint32_t)val) << SCCP_REG_MODE_RESERVED2_SHIFT))

#define  SCCP_REG_MODE_ISIZE_MASK 0x30
#define  SCCP_REG_MODE_ISIZE_SHIFT 4
#define  SCCP_REG_MODE_ISIZE_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_ISIZE_MASK) >> SCCP_REG_MODE_ISIZE_SHIFT)
#define  SCCP_REG_MODE_ISIZE_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_ISIZE_MASK) | (((uint32_t)val) << SCCP_REG_MODE_ISIZE_SHIFT))

#define  SCCP_REG_MODE_CTRL1MSB_MASK 0x8
#define  SCCP_REG_MODE_CTRL1MSB_SHIFT 3
#define  SCCP_REG_MODE_CTRL1MSB_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_CTRL1MSB_MASK) >> SCCP_REG_MODE_CTRL1MSB_SHIFT)
#define  SCCP_REG_MODE_CTRL1MSB_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_CTRL1MSB_MASK) | (((uint32_t)val) << SCCP_REG_MODE_CTRL1MSB_SHIFT))

#define  SCCP_REG_MODE_CTRL0MSB_MASK 0x4
#define  SCCP_REG_MODE_CTRL0MSB_SHIFT 2
#define  SCCP_REG_MODE_CTRL0MSB_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_CTRL0MSB_MASK) >> SCCP_REG_MODE_CTRL0MSB_SHIFT)
#define  SCCP_REG_MODE_CTRL0MSB_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_CTRL0MSB_MASK) | (((uint32_t)val) << SCCP_REG_MODE_CTRL0MSB_SHIFT))

#define  SCCP_REG_MODE_DATA1MSB_MASK 0x2
#define  SCCP_REG_MODE_DATA1MSB_SHIFT 1
#define  SCCP_REG_MODE_DATA1MSB_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_DATA1MSB_MASK) >> SCCP_REG_MODE_DATA1MSB_SHIFT)
#define  SCCP_REG_MODE_DATA1MSB_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_DATA1MSB_MASK) | (((uint32_t)val) << SCCP_REG_MODE_DATA1MSB_SHIFT))

#define  SCCP_REG_MODE_DATA0MSB_MASK 0x1
#define  SCCP_REG_MODE_DATA0MSB_SHIFT 0
#define  SCCP_REG_MODE_DATA0MSB_MASK_SHIFT(reg) (((reg) & SCCP_REG_MODE_DATA0MSB_MASK) >> SCCP_REG_MODE_DATA0MSB_SHIFT)
#define  SCCP_REG_MODE_DATA0MSB_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_MODE_DATA0MSB_MASK) | (((uint32_t)val) << SCCP_REG_MODE_DATA0MSB_SHIFT))

//====================================================================
//Register: Control 0 Register (CTRL0)
/** \brief Contains up to a 32-bit control sequence to be sent with SDAT instruction. The Control Bit Pointer 0 (CBPTR0) register selects which bit will be used by the SDAT instruction.*/
//====================================================================

#define  SCCP_REG_CTRL0_VALUE_MASK 0xffffffff
#define  SCCP_REG_CTRL0_VALUE_SHIFT 0
#define  SCCP_REG_CTRL0_VALUE_MASK_SHIFT(reg) (((reg) & SCCP_REG_CTRL0_VALUE_MASK) >> SCCP_REG_CTRL0_VALUE_SHIFT)
#define  SCCP_REG_CTRL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CTRL0_VALUE_MASK) | (((uint32_t)val) << SCCP_REG_CTRL0_VALUE_SHIFT))

//====================================================================
//Register: Control 1 Register (CTRL1)
/** \brief Contains up to a 32-bit control sequence to be sent with SDAT instruction. The Control Bit Pointer 1 (CBPTR1) register selects which bit will be used by the SDAT instruction.*/
//====================================================================

#define  SCCP_REG_CTRL1_VALUE_MASK 0xffffffff
#define  SCCP_REG_CTRL1_VALUE_SHIFT 0
#define  SCCP_REG_CTRL1_VALUE_MASK_SHIFT(reg) (((reg) & SCCP_REG_CTRL1_VALUE_MASK) >> SCCP_REG_CTRL1_VALUE_SHIFT)
#define  SCCP_REG_CTRL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CTRL1_VALUE_MASK) | (((uint32_t)val) << SCCP_REG_CTRL1_VALUE_SHIFT))

//====================================================================
//Register: Data 0 Register (DATA0)
/** \brief Contains up to a 32-bit control sequence to be sent with SDAT or loaded with RDAT instructions. The Control Bit Pointer 0 (DBPTR0) register content selects which bit will be used by the SDAT or RDAT instructions.*/
//====================================================================

#define  SCCP_REG_DATA0_VALUE_MASK 0xffffffff
#define  SCCP_REG_DATA0_VALUE_SHIFT 0
#define  SCCP_REG_DATA0_VALUE_MASK_SHIFT(reg) (((reg) & SCCP_REG_DATA0_VALUE_MASK) >> SCCP_REG_DATA0_VALUE_SHIFT)
#define  SCCP_REG_DATA0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DATA0_VALUE_MASK) | (((uint32_t)val) << SCCP_REG_DATA0_VALUE_SHIFT))

//====================================================================
//Register: Data 1 Register (DATA1)
/** \brief Contains up to a 32-bit control sequence to be sent with SDAT or loaded with RDAT instructions. The Control Bit Pointer 1 (DBPTR1) register content selects which bit will be used by the SDAT or RDAT instructions.*/
//====================================================================

#define  SCCP_REG_DATA1_VALUE_MASK 0xffffffff
#define  SCCP_REG_DATA1_VALUE_SHIFT 0
#define  SCCP_REG_DATA1_VALUE_MASK_SHIFT(reg) (((reg) & SCCP_REG_DATA1_VALUE_MASK) >> SCCP_REG_DATA1_VALUE_SHIFT)
#define  SCCP_REG_DATA1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DATA1_VALUE_MASK) | (((uint32_t)val) << SCCP_REG_DATA1_VALUE_SHIFT))

//====================================================================
//Register: Control Bit Pointer 0 Register (CBPTR0)
/** \brief Control Bit Pointer 0 is a 5 bit pointer that selects which bit of the CTRL0 register is accessed by the SDAT instruction. Control Bit Pointer 0 is post-incremented or post-decremented by the SDAT instructions depending on the Ctrl0MSB field in MODE register.*/
//====================================================================

#define  SCCP_REG_CBPTR0_RESERVED1_MASK 0xffffffe0
#define  SCCP_REG_CBPTR0_RESERVED1_SHIFT 5
#define  SCCP_REG_CBPTR0_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_CBPTR0_RESERVED1_MASK) >> SCCP_REG_CBPTR0_RESERVED1_SHIFT)
#define  SCCP_REG_CBPTR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CBPTR0_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_CBPTR0_RESERVED1_SHIFT))

#define  SCCP_REG_CBPTR0_BITPTR_MASK 0x1f
#define  SCCP_REG_CBPTR0_BITPTR_SHIFT 0
#define  SCCP_REG_CBPTR0_BITPTR_MASK_SHIFT(reg) (((reg) & SCCP_REG_CBPTR0_BITPTR_MASK) >> SCCP_REG_CBPTR0_BITPTR_SHIFT)
#define  SCCP_REG_CBPTR0_BITPTR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CBPTR0_BITPTR_MASK) | (((uint32_t)val) << SCCP_REG_CBPTR0_BITPTR_SHIFT))

//====================================================================
//Register: Control Bit Pointer 1 Register (CBPTR1)
/** \brief Control Bit Pointer 1 is a 5 bit pointer that selects which bit of the CTRL1 register is accessed by the SDAT instruction. Control Bit Pointer 1 is post-incremented or post-decremented by the SDAT instructions depending on the Ctrl1MSB field in MODE register.*/
//====================================================================

#define  SCCP_REG_CBPTR1_RESERVED1_MASK 0xffffffe0
#define  SCCP_REG_CBPTR1_RESERVED1_SHIFT 5
#define  SCCP_REG_CBPTR1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_CBPTR1_RESERVED1_MASK) >> SCCP_REG_CBPTR1_RESERVED1_SHIFT)
#define  SCCP_REG_CBPTR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CBPTR1_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_CBPTR1_RESERVED1_SHIFT))

#define  SCCP_REG_CBPTR1_BITPTR_MASK 0x1f
#define  SCCP_REG_CBPTR1_BITPTR_SHIFT 0
#define  SCCP_REG_CBPTR1_BITPTR_MASK_SHIFT(reg) (((reg) & SCCP_REG_CBPTR1_BITPTR_MASK) >> SCCP_REG_CBPTR1_BITPTR_SHIFT)
#define  SCCP_REG_CBPTR1_BITPTR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CBPTR1_BITPTR_MASK) | (((uint32_t)val) << SCCP_REG_CBPTR1_BITPTR_SHIFT))

//====================================================================
//Register: Data Bit Pointer 0 Register (DBPTR0)
/** \brief Data Bit Pointer 0 is a 5 bit pointer that selects which bit of the DATA0 register is accessed by the SDAT or RDAT instructions. Data Bit Pointer 0 is post-incremented or post-decremented by the SDAT or RDAT instructions depending on the Data0MSB field in MODE register.*/
//====================================================================

#define  SCCP_REG_DBPTR0_RESERVED1_MASK 0xffffffe0
#define  SCCP_REG_DBPTR0_RESERVED1_SHIFT 5
#define  SCCP_REG_DBPTR0_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_DBPTR0_RESERVED1_MASK) >> SCCP_REG_DBPTR0_RESERVED1_SHIFT)
#define  SCCP_REG_DBPTR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DBPTR0_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_DBPTR0_RESERVED1_SHIFT))

#define  SCCP_REG_DBPTR0_BITPTR_MASK 0x1f
#define  SCCP_REG_DBPTR0_BITPTR_SHIFT 0
#define  SCCP_REG_DBPTR0_BITPTR_MASK_SHIFT(reg) (((reg) & SCCP_REG_DBPTR0_BITPTR_MASK) >> SCCP_REG_DBPTR0_BITPTR_SHIFT)
#define  SCCP_REG_DBPTR0_BITPTR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DBPTR0_BITPTR_MASK) | (((uint32_t)val) << SCCP_REG_DBPTR0_BITPTR_SHIFT))

//====================================================================
//Register: Data Bit Pointer 1 Register (DBPTR1)
/** \brief Data Bit Pointer 1 is a 5 bit pointer that selects which bit of the DATA1 register is accessed by the SDAT or RDAT instructions. Data Bit Pointer 1 is post-incremented or post-decremented by the SDAT or RDAT instructions depending on the Data1MSB field in MODE register.*/
//====================================================================

#define  SCCP_REG_DBPTR1_RESERVED1_MASK 0xffffffe0
#define  SCCP_REG_DBPTR1_RESERVED1_SHIFT 5
#define  SCCP_REG_DBPTR1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_DBPTR1_RESERVED1_MASK) >> SCCP_REG_DBPTR1_RESERVED1_SHIFT)
#define  SCCP_REG_DBPTR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DBPTR1_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_DBPTR1_RESERVED1_SHIFT))

#define  SCCP_REG_DBPTR1_BITPTR_MASK 0x1f
#define  SCCP_REG_DBPTR1_BITPTR_SHIFT 0
#define  SCCP_REG_DBPTR1_BITPTR_MASK_SHIFT(reg) (((reg) & SCCP_REG_DBPTR1_BITPTR_MASK) >> SCCP_REG_DBPTR1_BITPTR_SHIFT)
#define  SCCP_REG_DBPTR1_BITPTR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DBPTR1_BITPTR_MASK) | (((uint32_t)val) << SCCP_REG_DBPTR1_BITPTR_SHIFT))

//====================================================================
//Register: Control Parity 0 Register (CPAR0)
/** \brief Control Parity 0 is a two bit register with odd and even parity bits calculated by SDAT instructions.*/
//====================================================================

#define  SCCP_REG_CPAR0_RESERVED1_MASK 0xfffffffc
#define  SCCP_REG_CPAR0_RESERVED1_SHIFT 2
#define  SCCP_REG_CPAR0_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPAR0_RESERVED1_MASK) >> SCCP_REG_CPAR0_RESERVED1_SHIFT)
#define  SCCP_REG_CPAR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPAR0_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_CPAR0_RESERVED1_SHIFT))

#define  SCCP_REG_CPAR0_ODDPAR_MASK 0x2
#define  SCCP_REG_CPAR0_ODDPAR_SHIFT 1
#define  SCCP_REG_CPAR0_ODDPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPAR0_ODDPAR_MASK) >> SCCP_REG_CPAR0_ODDPAR_SHIFT)
#define  SCCP_REG_CPAR0_ODDPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPAR0_ODDPAR_MASK) | (((uint32_t)val) << SCCP_REG_CPAR0_ODDPAR_SHIFT))

#define  SCCP_REG_CPAR0_EVENPAR_MASK 0x1
#define  SCCP_REG_CPAR0_EVENPAR_SHIFT 0
#define  SCCP_REG_CPAR0_EVENPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPAR0_EVENPAR_MASK) >> SCCP_REG_CPAR0_EVENPAR_SHIFT)
#define  SCCP_REG_CPAR0_EVENPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPAR0_EVENPAR_MASK) | (((uint32_t)val) << SCCP_REG_CPAR0_EVENPAR_SHIFT))

//====================================================================
//Register: Control Parity 1 Register (CPAR1)
/** \brief Control Parity 1 is a two bit register with odd and even parity bits calculated by SDAT instructions.*/
//====================================================================

#define  SCCP_REG_CPAR1_RESERVED1_MASK 0xfffffffc
#define  SCCP_REG_CPAR1_RESERVED1_SHIFT 2
#define  SCCP_REG_CPAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPAR1_RESERVED1_MASK) >> SCCP_REG_CPAR1_RESERVED1_SHIFT)
#define  SCCP_REG_CPAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPAR1_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_CPAR1_RESERVED1_SHIFT))

#define  SCCP_REG_CPAR1_ODDPAR_MASK 0x2
#define  SCCP_REG_CPAR1_ODDPAR_SHIFT 1
#define  SCCP_REG_CPAR1_ODDPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPAR1_ODDPAR_MASK) >> SCCP_REG_CPAR1_ODDPAR_SHIFT)
#define  SCCP_REG_CPAR1_ODDPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPAR1_ODDPAR_MASK) | (((uint32_t)val) << SCCP_REG_CPAR1_ODDPAR_SHIFT))

#define  SCCP_REG_CPAR1_EVENPAR_MASK 0x1
#define  SCCP_REG_CPAR1_EVENPAR_SHIFT 0
#define  SCCP_REG_CPAR1_EVENPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPAR1_EVENPAR_MASK) >> SCCP_REG_CPAR1_EVENPAR_SHIFT)
#define  SCCP_REG_CPAR1_EVENPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPAR1_EVENPAR_MASK) | (((uint32_t)val) << SCCP_REG_CPAR1_EVENPAR_SHIFT))

//====================================================================
//Register: Data Parity 0 Register (DPAR0)
/** \brief Data Parity 0 is a two bit register with odd and even parity bits calculated by SDAT/RDAT instructions.*/
//====================================================================

#define  SCCP_REG_DPAR0_RESERVED1_MASK 0xfffffffc
#define  SCCP_REG_DPAR0_RESERVED1_SHIFT 2
#define  SCCP_REG_DPAR0_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_DPAR0_RESERVED1_MASK) >> SCCP_REG_DPAR0_RESERVED1_SHIFT)
#define  SCCP_REG_DPAR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DPAR0_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_DPAR0_RESERVED1_SHIFT))

#define  SCCP_REG_DPAR0_ODDPAR_MASK 0x2
#define  SCCP_REG_DPAR0_ODDPAR_SHIFT 1
#define  SCCP_REG_DPAR0_ODDPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_DPAR0_ODDPAR_MASK) >> SCCP_REG_DPAR0_ODDPAR_SHIFT)
#define  SCCP_REG_DPAR0_ODDPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DPAR0_ODDPAR_MASK) | (((uint32_t)val) << SCCP_REG_DPAR0_ODDPAR_SHIFT))

#define  SCCP_REG_DPAR0_EVENPAR_MASK 0x1
#define  SCCP_REG_DPAR0_EVENPAR_SHIFT 0
#define  SCCP_REG_DPAR0_EVENPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_DPAR0_EVENPAR_MASK) >> SCCP_REG_DPAR0_EVENPAR_SHIFT)
#define  SCCP_REG_DPAR0_EVENPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DPAR0_EVENPAR_MASK) | (((uint32_t)val) << SCCP_REG_DPAR0_EVENPAR_SHIFT))

//====================================================================
//Register: Data Parity 1 Register (DPAR1)
/** \brief Data Parity 1 is a two bit register with odd and even parity bits calculated by SDAT/RDAT instructions.*/
//====================================================================

#define  SCCP_REG_DPAR1_RESERVED1_MASK 0xfffffffc
#define  SCCP_REG_DPAR1_RESERVED1_SHIFT 2
#define  SCCP_REG_DPAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_DPAR1_RESERVED1_MASK) >> SCCP_REG_DPAR1_RESERVED1_SHIFT)
#define  SCCP_REG_DPAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DPAR1_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_DPAR1_RESERVED1_SHIFT))

#define  SCCP_REG_DPAR1_ODDPAR_MASK 0x2
#define  SCCP_REG_DPAR1_ODDPAR_SHIFT 1
#define  SCCP_REG_DPAR1_ODDPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_DPAR1_ODDPAR_MASK) >> SCCP_REG_DPAR1_ODDPAR_SHIFT)
#define  SCCP_REG_DPAR1_ODDPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DPAR1_ODDPAR_MASK) | (((uint32_t)val) << SCCP_REG_DPAR1_ODDPAR_SHIFT))

#define  SCCP_REG_DPAR1_EVENPAR_MASK 0x1
#define  SCCP_REG_DPAR1_EVENPAR_SHIFT 0
#define  SCCP_REG_DPAR1_EVENPAR_MASK_SHIFT(reg) (((reg) & SCCP_REG_DPAR1_EVENPAR_MASK) >> SCCP_REG_DPAR1_EVENPAR_SHIFT)
#define  SCCP_REG_DPAR1_EVENPAR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DPAR1_EVENPAR_MASK) | (((uint32_t)val) << SCCP_REG_DPAR1_EVENPAR_SHIFT))

//====================================================================
//Register: GPIO Output Enable Register (GPIO_OE)
/** \brief GPIO Output Enable is a 32 bit register to hold output enables for up to 32 GPIO pins. GPIO Output Enable can be modified directly or as a side effect of the SGPIO instruction.*/
//====================================================================

#define  SCCP_REG_GPIO_OE_GPIOOE_MASK 0xffffffff
#define  SCCP_REG_GPIO_OE_GPIOOE_SHIFT 0
#define  SCCP_REG_GPIO_OE_GPIOOE_MASK_SHIFT(reg) (((reg) & SCCP_REG_GPIO_OE_GPIOOE_MASK) >> SCCP_REG_GPIO_OE_GPIOOE_SHIFT)
#define  SCCP_REG_GPIO_OE_GPIOOE_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GPIO_OE_GPIOOE_MASK) | (((uint32_t)val) << SCCP_REG_GPIO_OE_GPIOOE_SHIFT))

//====================================================================
//Register: GPIO Output Value Register (GPIO_O)
/** \brief GPIO Output Value is a 32 bit register to hold output value for up to 32 GPIO pins. GPIO Output Value can be modified directly or as a side effect of the SGPIO instruction.*/
//====================================================================

#define  SCCP_REG_GPIO_O_GPIOOUT_MASK 0xffffffff
#define  SCCP_REG_GPIO_O_GPIOOUT_SHIFT 0
#define  SCCP_REG_GPIO_O_GPIOOUT_MASK_SHIFT(reg) (((reg) & SCCP_REG_GPIO_O_GPIOOUT_MASK) >> SCCP_REG_GPIO_O_GPIOOUT_SHIFT)
#define  SCCP_REG_GPIO_O_GPIOOUT_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GPIO_O_GPIOOUT_MASK) | (((uint32_t)val) << SCCP_REG_GPIO_O_GPIOOUT_SHIFT))

//====================================================================
//Register: GPIO Input Value Register (GPIO_I)
/** \brief GPIO Input Value is a READ-ONLY 32 bit register that reflects input values for up to 32 GPIO pins. GPIO Input Value can be modified directly or as a side effect of the SGPIO instruction.*/
//====================================================================

#define  SCCP_REG_GPIO_I_GPIOIN_MASK 0xffffffff
#define  SCCP_REG_GPIO_I_GPIOIN_SHIFT 0
#define  SCCP_REG_GPIO_I_GPIOIN_MASK_SHIFT(reg) (((reg) & SCCP_REG_GPIO_I_GPIOIN_MASK) >> SCCP_REG_GPIO_I_GPIOIN_SHIFT)
#define  SCCP_REG_GPIO_I_GPIOIN_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GPIO_I_GPIOIN_MASK) | (((uint32_t)val) << SCCP_REG_GPIO_I_GPIOIN_SHIFT))

//====================================================================
//Register: External Triggers Register (TRIG)
/** \brief External Triggers is a 18 bit register that can be set by external events and cleared by SCCP code or the main CPU. External Triggers can be modified directly or as a side effect if the CLT instruction.*/
//====================================================================

#define  SCCP_REG_TRIG_RESERVED1_MASK 0xfffc0000
#define  SCCP_REG_TRIG_RESERVED1_SHIFT 18
#define  SCCP_REG_TRIG_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_RESERVED1_MASK) >> SCCP_REG_TRIG_RESERVED1_SHIFT)
#define  SCCP_REG_TRIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_RESERVED1_SHIFT))

#define  SCCP_REG_TRIG_TRIG_MASK 0x3ffff
#define  SCCP_REG_TRIG_TRIG_SHIFT 0
#define  SCCP_REG_TRIG_TRIG_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_TRIG_MASK) >> SCCP_REG_TRIG_TRIG_SHIFT)
#define  SCCP_REG_TRIG_TRIG_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_TRIG_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_TRIG_SHIFT))

//====================================================================
//Register: Indirect Address Register (IADDR)
/** \brief Indirect Address is a 20 bit SRAM address for LDRI/STRI indirect loads and stores. The lower two bits are used to support byte addressing if configured by the ISize field in the MODE register.*/
//====================================================================

#define  SCCP_REG_IADDR_RESERVED1_MASK 0xfff00000
#define  SCCP_REG_IADDR_RESERVED1_SHIFT 20
#define  SCCP_REG_IADDR_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_IADDR_RESERVED1_MASK) >> SCCP_REG_IADDR_RESERVED1_SHIFT)
#define  SCCP_REG_IADDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_IADDR_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_IADDR_RESERVED1_SHIFT))

#define  SCCP_REG_IADDR_ADDR_MASK 0xfffff
#define  SCCP_REG_IADDR_ADDR_SHIFT 0
#define  SCCP_REG_IADDR_ADDR_MASK_SHIFT(reg) (((reg) & SCCP_REG_IADDR_ADDR_MASK) >> SCCP_REG_IADDR_ADDR_SHIFT)
#define  SCCP_REG_IADDR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_IADDR_ADDR_MASK) | (((uint32_t)val) << SCCP_REG_IADDR_ADDR_SHIFT))

//====================================================================
//Register: Interrupts Register (INT)
/** \brief Interrupts is a 32 bit register that can be set/cleared by SCCP code or by the main CPU. Interrupts can be modified directly or as a side effect if the SINT instruction.*/
//====================================================================

#define  SCCP_REG_INT_INT_MASK 0xffffffff
#define  SCCP_REG_INT_INT_SHIFT 0
#define  SCCP_REG_INT_INT_MASK_SHIFT(reg) (((reg) & SCCP_REG_INT_INT_MASK) >> SCCP_REG_INT_INT_SHIFT)
#define  SCCP_REG_INT_INT_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_INT_INT_MASK) | (((uint32_t)val) << SCCP_REG_INT_INT_SHIFT))

//====================================================================
//Register: General Purpose Register 0 (GEN0)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN0_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN0_DATA_SHIFT 0
#define  SCCP_REG_GEN0_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN0_DATA_MASK) >> SCCP_REG_GEN0_DATA_SHIFT)
#define  SCCP_REG_GEN0_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN0_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN0_DATA_SHIFT))

//====================================================================
//Register: General Purpose Register 1 (GEN1)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN1_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN1_DATA_SHIFT 0
#define  SCCP_REG_GEN1_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN1_DATA_MASK) >> SCCP_REG_GEN1_DATA_SHIFT)
#define  SCCP_REG_GEN1_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN1_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN1_DATA_SHIFT))

//====================================================================
//Register: General Purpose Register 2 (GEN2)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN2_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN2_DATA_SHIFT 0
#define  SCCP_REG_GEN2_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN2_DATA_MASK) >> SCCP_REG_GEN2_DATA_SHIFT)
#define  SCCP_REG_GEN2_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN2_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN2_DATA_SHIFT))

//====================================================================
//Register: General Purpose Register 3 (GEN3)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN3_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN3_DATA_SHIFT 0
#define  SCCP_REG_GEN3_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN3_DATA_MASK) >> SCCP_REG_GEN3_DATA_SHIFT)
#define  SCCP_REG_GEN3_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN3_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN3_DATA_SHIFT))

//====================================================================
//Register: General Purpose Register 4 (GEN4)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN4_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN4_DATA_SHIFT 0
#define  SCCP_REG_GEN4_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN4_DATA_MASK) >> SCCP_REG_GEN4_DATA_SHIFT)
#define  SCCP_REG_GEN4_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN4_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN4_DATA_SHIFT))

//====================================================================
//Register: General Purpose Register 5 (GEN5)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN5_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN5_DATA_SHIFT 0
#define  SCCP_REG_GEN5_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN5_DATA_MASK) >> SCCP_REG_GEN5_DATA_SHIFT)
#define  SCCP_REG_GEN5_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN5_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN5_DATA_SHIFT))

//====================================================================
//Register: General Purpose Register 6 (GEN6)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN6_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN6_DATA_SHIFT 0
#define  SCCP_REG_GEN6_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN6_DATA_MASK) >> SCCP_REG_GEN6_DATA_SHIFT)
#define  SCCP_REG_GEN6_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN6_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN6_DATA_SHIFT))

//====================================================================
//Register: General Purpose Register 7 (GEN7)
/** \brief 32 Bit general purpose data register*/
//====================================================================

#define  SCCP_REG_GEN7_DATA_MASK 0xffffffff
#define  SCCP_REG_GEN7_DATA_SHIFT 0
#define  SCCP_REG_GEN7_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_GEN7_DATA_MASK) >> SCCP_REG_GEN7_DATA_SHIFT)
#define  SCCP_REG_GEN7_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GEN7_DATA_MASK) | (((uint32_t)val) << SCCP_REG_GEN7_DATA_SHIFT))

//====================================================================
//Register: CPU Control Register (CPU_CTRL)
/** \brief This register is only accessible by the main CPU, and is used to control the uCtrl.*/
//====================================================================

#define  SCCP_REG_CPU_CTRL_SOFTRESET_MASK 0x80000000
#define  SCCP_REG_CPU_CTRL_SOFTRESET_SHIFT 31
#define  SCCP_REG_CPU_CTRL_SOFTRESET_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPU_CTRL_SOFTRESET_MASK) >> SCCP_REG_CPU_CTRL_SOFTRESET_SHIFT)
#define  SCCP_REG_CPU_CTRL_SOFTRESET_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPU_CTRL_SOFTRESET_MASK) | (((uint32_t)val) << SCCP_REG_CPU_CTRL_SOFTRESET_SHIFT))

#define  SCCP_REG_CPU_CTRL_RESERVED1_MASK 0x7ffffff8
#define  SCCP_REG_CPU_CTRL_RESERVED1_SHIFT 3
#define  SCCP_REG_CPU_CTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPU_CTRL_RESERVED1_MASK) >> SCCP_REG_CPU_CTRL_RESERVED1_SHIFT)
#define  SCCP_REG_CPU_CTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPU_CTRL_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_CPU_CTRL_RESERVED1_SHIFT))

#define  SCCP_REG_CPU_CTRL_SRAMPRIORITY_MASK 0x4
#define  SCCP_REG_CPU_CTRL_SRAMPRIORITY_SHIFT 2
#define  SCCP_REG_CPU_CTRL_SRAMPRIORITY_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPU_CTRL_SRAMPRIORITY_MASK) >> SCCP_REG_CPU_CTRL_SRAMPRIORITY_SHIFT)
#define  SCCP_REG_CPU_CTRL_SRAMPRIORITY_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPU_CTRL_SRAMPRIORITY_MASK) | (((uint32_t)val) << SCCP_REG_CPU_CTRL_SRAMPRIORITY_SHIFT))

#define  SCCP_REG_CPU_CTRL_SS_MASK 0x2
#define  SCCP_REG_CPU_CTRL_SS_SHIFT 1
#define  SCCP_REG_CPU_CTRL_SS_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPU_CTRL_SS_MASK) >> SCCP_REG_CPU_CTRL_SS_SHIFT)
#define  SCCP_REG_CPU_CTRL_SS_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPU_CTRL_SS_MASK) | (((uint32_t)val) << SCCP_REG_CPU_CTRL_SS_SHIFT))

#define  SCCP_REG_CPU_CTRL_RUN_MASK 0x1
#define  SCCP_REG_CPU_CTRL_RUN_SHIFT 0
#define  SCCP_REG_CPU_CTRL_RUN_MASK_SHIFT(reg) (((reg) & SCCP_REG_CPU_CTRL_RUN_MASK) >> SCCP_REG_CPU_CTRL_RUN_SHIFT)
#define  SCCP_REG_CPU_CTRL_RUN_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_CPU_CTRL_RUN_MASK) | (((uint32_t)val) << SCCP_REG_CPU_CTRL_RUN_SHIFT))

//====================================================================
//Register: Debug Instruction Address (DEBUG_IADDR)
/** \brief Debug Instruction Address is a 18 bit word offset into the SCCM SRAM space. It represents the address of the current instruction in the execute stage (ie. the instruction that will be executed next). Useful for debugging SCCP assembly in single step mode.*/
//====================================================================

#define  SCCP_REG_DEBUG_IADDR_RESERVED1_MASK 0xfffc0000
#define  SCCP_REG_DEBUG_IADDR_RESERVED1_SHIFT 18
#define  SCCP_REG_DEBUG_IADDR_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_DEBUG_IADDR_RESERVED1_MASK) >> SCCP_REG_DEBUG_IADDR_RESERVED1_SHIFT)
#define  SCCP_REG_DEBUG_IADDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DEBUG_IADDR_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_DEBUG_IADDR_RESERVED1_SHIFT))

#define  SCCP_REG_DEBUG_IADDR_DEBUGIADDR_MASK 0x3ffff
#define  SCCP_REG_DEBUG_IADDR_DEBUGIADDR_SHIFT 0
#define  SCCP_REG_DEBUG_IADDR_DEBUGIADDR_MASK_SHIFT(reg) (((reg) & SCCP_REG_DEBUG_IADDR_DEBUGIADDR_MASK) >> SCCP_REG_DEBUG_IADDR_DEBUGIADDR_SHIFT)
#define  SCCP_REG_DEBUG_IADDR_DEBUGIADDR_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_DEBUG_IADDR_DEBUGIADDR_MASK) | (((uint32_t)val) << SCCP_REG_DEBUG_IADDR_DEBUGIADDR_SHIFT))

//====================================================================
//Register: Trigger Configuration 0 (TRIG_CFG0)
/** \brief This register is only accessible by the main CPU, and is used to configure the external triggers. There is a two bit edge select field for each of the 18 external triggers. The table below describes the edge select encoding.
<table align="center"> <tr><th>edge_sel[1:0]</th> <th>Trigger Type</th></tr> <tr><td>'00'</td><td>Rising Edge</td></tr> <tr><td>'01'</td><td>Falling Edge</td></tr> <tr><td>'10'</td><td>Both Edges</td></tr> <tr><td>'11'</td><td>Disabled</td></tr></table>*/
//====================================================================

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL15_MASK 0xc0000000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL15_SHIFT 30
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL15_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL15_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL15_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL15_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL15_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL15_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL14_MASK 0x30000000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL14_SHIFT 28
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL14_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL14_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL14_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL14_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL14_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL14_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL13_MASK 0xc000000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL13_SHIFT 26
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL13_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL13_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL13_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL13_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL13_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL13_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL12_MASK 0x3000000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL12_SHIFT 24
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL12_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL12_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL12_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL12_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL12_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL12_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL11_MASK 0xc00000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL11_SHIFT 22
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL11_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL11_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL11_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL11_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL11_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL11_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL10_MASK 0x300000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL10_SHIFT 20
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL10_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL10_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL10_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL10_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL10_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL10_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL9_MASK 0xc0000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL9_SHIFT 18
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL9_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL9_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL9_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL9_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL9_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL9_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL8_MASK 0x30000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL8_SHIFT 16
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL8_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL8_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL8_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL8_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL8_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL8_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL7_MASK 0xc000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL7_SHIFT 14
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL7_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL7_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL7_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL7_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL7_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL7_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL6_MASK 0x3000
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL6_SHIFT 12
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL6_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL6_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL6_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL6_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL6_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL6_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL5_MASK 0xc00
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL5_SHIFT 10
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL5_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL5_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL5_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL5_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL5_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL5_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL4_MASK 0x300
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL4_SHIFT 8
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL4_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL4_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL4_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL4_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL4_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL4_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL3_MASK 0xc0
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL3_SHIFT 6
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL3_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL3_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL3_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL3_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL3_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL3_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL2_MASK 0x30
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL2_SHIFT 4
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL2_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL2_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL2_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL2_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL2_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL2_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL1_MASK 0xc
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL1_SHIFT 2
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL1_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL1_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL1_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL1_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL1_SHIFT))

#define  SCCP_REG_TRIG_CFG0_EDGE_SEL0_MASK 0x3
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL0_SHIFT 0
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL0_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG0_EDGE_SEL0_MASK) >> SCCP_REG_TRIG_CFG0_EDGE_SEL0_SHIFT)
#define  SCCP_REG_TRIG_CFG0_EDGE_SEL0_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG0_EDGE_SEL0_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG0_EDGE_SEL0_SHIFT))

//====================================================================
//Register: Trigger Configuration 1 (TRIG_CFG1)
/** \brief This register is only accessible by the main CPU, and is used to configure the external triggers. There is a two bit edge select field for each of the 18 external triggers. The table below describes the edge select encoding.
<table align="center"> <tr><th>edge_sel[1:0]</th> <th>Trigger Type</th></tr> <tr><td>'00'</td><td>Rising Edge</td></tr> <tr><td>'01'</td><td>Falling Edge</td></tr> <tr><td>'10'</td><td>Both Edges</td></tr> <tr><td>'11'</td><td>Disabled</td></tr></table>*/
//====================================================================

#define  SCCP_REG_TRIG_CFG1_RESERVED1_MASK 0xfffffff0
#define  SCCP_REG_TRIG_CFG1_RESERVED1_SHIFT 4
#define  SCCP_REG_TRIG_CFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG1_RESERVED1_MASK) >> SCCP_REG_TRIG_CFG1_RESERVED1_SHIFT)
#define  SCCP_REG_TRIG_CFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG1_RESERVED1_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG1_RESERVED1_SHIFT))

#define  SCCP_REG_TRIG_CFG1_EDGE_SEL17_MASK 0xc
#define  SCCP_REG_TRIG_CFG1_EDGE_SEL17_SHIFT 2
#define  SCCP_REG_TRIG_CFG1_EDGE_SEL17_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG1_EDGE_SEL17_MASK) >> SCCP_REG_TRIG_CFG1_EDGE_SEL17_SHIFT)
#define  SCCP_REG_TRIG_CFG1_EDGE_SEL17_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG1_EDGE_SEL17_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG1_EDGE_SEL17_SHIFT))

#define  SCCP_REG_TRIG_CFG1_EDGE_SEL16_MASK 0x3
#define  SCCP_REG_TRIG_CFG1_EDGE_SEL16_SHIFT 0
#define  SCCP_REG_TRIG_CFG1_EDGE_SEL16_MASK_SHIFT(reg) (((reg) & SCCP_REG_TRIG_CFG1_EDGE_SEL16_MASK) >> SCCP_REG_TRIG_CFG1_EDGE_SEL16_SHIFT)
#define  SCCP_REG_TRIG_CFG1_EDGE_SEL16_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_TRIG_CFG1_EDGE_SEL16_MASK) | (((uint32_t)val) << SCCP_REG_TRIG_CFG1_EDGE_SEL16_SHIFT))

//====================================================================
//Register: GPIO Input Muxing (GPIO_MUX)
/** \brief This register is only accessible by the main CPU, and is used to configure the 32bit GPIO_in SCCP inputs. The GPIO_in inputs can either be mapped to the external pins (default) or else to internal signals (see APB document for the list of internal signal connections). There is one select bit for each of the GPIOs. The following table describes the muxing.
<table align="center"> <tr><th>sel[i]</th> <th>GPIO_in Muxing[i]</th></tr> <tr><td>'0'</td><td>External Pin</td></tr> <tr><td>'1'</td><td>Internal Signal</td></tr></table>*/
//====================================================================

#define  SCCP_REG_GPIO_MUX_MUX_CTRL_MASK 0xffffffff
#define  SCCP_REG_GPIO_MUX_MUX_CTRL_SHIFT 0
#define  SCCP_REG_GPIO_MUX_MUX_CTRL_MASK_SHIFT(reg) (((reg) & SCCP_REG_GPIO_MUX_MUX_CTRL_MASK) >> SCCP_REG_GPIO_MUX_MUX_CTRL_SHIFT)
#define  SCCP_REG_GPIO_MUX_MUX_CTRL_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_GPIO_MUX_MUX_CTRL_MASK) | (((uint32_t)val) << SCCP_REG_GPIO_MUX_MUX_CTRL_SHIFT))

//====================================================================
//Register: Microcode SRAM (SRAM)
/** \brief The Microcode SRAM holds the microcode executed by the SCCP embedded micro-controller. There are 1k address x 32 bits.*/
//====================================================================

#define  SCCP_REG_SRAM_DATA_MASK 0xffffffff
#define  SCCP_REG_SRAM_DATA_SHIFT 0
#define  SCCP_REG_SRAM_DATA_MASK_SHIFT(reg) (((reg) & SCCP_REG_SRAM_DATA_MASK) >> SCCP_REG_SRAM_DATA_SHIFT)
#define  SCCP_REG_SRAM_DATA_REPLACE_VAL(reg,val) (((reg) & ~SCCP_REG_SRAM_DATA_MASK) | (((uint32_t)val) << SCCP_REG_SRAM_DATA_SHIFT))

#endif // SCCP_REG
