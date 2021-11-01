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
 * \file CSC_RGB2ES_3L_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSC_RGB2ES_3L_REGMASKS_H_
#define _CSC_RGB2ES_3L_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: RGB2ES Configuration with three LUTs (CSC_RGB2ES_3L)
/** \brief Register Descriptions for the RGB2ES 3 LUTs CSC block*/
//
//====================================================================

//====================================================================
//Register: CSC Configuration Register (CCR)
/** \brief The CSC Configuration Register (CCR) manages the configuration of the CSC block. For normal operation of the CSC block, all bits in this register should be set to '0'.
Note that firmware should ensure that the pipeline is idle before setting the Processor LUT Access bit and programming the LUT. Also note that when the Block Bypass bit is set, the state machine is reset.*/
//====================================================================

#define  CSC_RGB2ES_3L_CCR_RESERVED1_MASK 0xffffff00
#define  CSC_RGB2ES_3L_CCR_RESERVED1_SHIFT 8
#define  CSC_RGB2ES_3L_CCR_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_RESERVED1_MASK) >> CSC_RGB2ES_3L_CCR_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_CCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_CCR_RESERVED2_MASK 0x80
#define  CSC_RGB2ES_3L_CCR_RESERVED2_SHIFT 7
#define  CSC_RGB2ES_3L_CCR_RESERVED2_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_RESERVED2_MASK) >> CSC_RGB2ES_3L_CCR_RESERVED2_SHIFT)
#define  CSC_RGB2ES_3L_CCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_RESERVED2_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_RESERVED2_SHIFT))

#define  CSC_RGB2ES_3L_CCR_RESERVED3_MASK 0x40
#define  CSC_RGB2ES_3L_CCR_RESERVED3_SHIFT 6
#define  CSC_RGB2ES_3L_CCR_RESERVED3_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_RESERVED3_MASK) >> CSC_RGB2ES_3L_CCR_RESERVED3_SHIFT)
#define  CSC_RGB2ES_3L_CCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_RESERVED3_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_RESERVED3_SHIFT))

#define  CSC_RGB2ES_3L_CCR_RESERVED4_MASK 0x20
#define  CSC_RGB2ES_3L_CCR_RESERVED4_SHIFT 5
#define  CSC_RGB2ES_3L_CCR_RESERVED4_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_RESERVED4_MASK) >> CSC_RGB2ES_3L_CCR_RESERVED4_SHIFT)
#define  CSC_RGB2ES_3L_CCR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_RESERVED4_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_RESERVED4_SHIFT))

#define  CSC_RGB2ES_3L_CCR_CPUACCESS_MASK 0x10
#define  CSC_RGB2ES_3L_CCR_CPUACCESS_SHIFT 4
#define  CSC_RGB2ES_3L_CCR_CPUACCESS_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_CPUACCESS_MASK) >> CSC_RGB2ES_3L_CCR_CPUACCESS_SHIFT)
#define  CSC_RGB2ES_3L_CCR_CPUACCESS_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_CPUACCESS_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_CPUACCESS_SHIFT))

#define  CSC_RGB2ES_3L_CCR_OLUTBYPASS_MASK 0x8
#define  CSC_RGB2ES_3L_CCR_OLUTBYPASS_SHIFT 3
#define  CSC_RGB2ES_3L_CCR_OLUTBYPASS_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_OLUTBYPASS_MASK) >> CSC_RGB2ES_3L_CCR_OLUTBYPASS_SHIFT)
#define  CSC_RGB2ES_3L_CCR_OLUTBYPASS_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_OLUTBYPASS_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_OLUTBYPASS_SHIFT))

#define  CSC_RGB2ES_3L_CCR_RESERVED5_MASK 0x4
#define  CSC_RGB2ES_3L_CCR_RESERVED5_SHIFT 2
#define  CSC_RGB2ES_3L_CCR_RESERVED5_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_RESERVED5_MASK) >> CSC_RGB2ES_3L_CCR_RESERVED5_SHIFT)
#define  CSC_RGB2ES_3L_CCR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_RESERVED5_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_RESERVED5_SHIFT))

#define  CSC_RGB2ES_3L_CCR_RESERVED6_MASK 0x2
#define  CSC_RGB2ES_3L_CCR_RESERVED6_SHIFT 1
#define  CSC_RGB2ES_3L_CCR_RESERVED6_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_RESERVED6_MASK) >> CSC_RGB2ES_3L_CCR_RESERVED6_SHIFT)
#define  CSC_RGB2ES_3L_CCR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_RESERVED6_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_RESERVED6_SHIFT))

#define  CSC_RGB2ES_3L_CCR_BYPASSALL_MASK 0x1
#define  CSC_RGB2ES_3L_CCR_BYPASSALL_SHIFT 0
#define  CSC_RGB2ES_3L_CCR_BYPASSALL_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_CCR_BYPASSALL_MASK) >> CSC_RGB2ES_3L_CCR_BYPASSALL_SHIFT)
#define  CSC_RGB2ES_3L_CCR_BYPASSALL_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_CCR_BYPASSALL_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_CCR_BYPASSALL_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 1 of 9) (MCR0)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 1 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR0_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR0_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR0_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR0_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR0_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR0_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR0_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR0_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR0_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR0_COEFF_MASK) >> CSC_RGB2ES_3L_MCR0_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR0_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR0_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR0_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 2 of 9) (MCR1)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 2 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR1_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR1_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR1_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR1_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR1_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR1_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR1_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR1_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR1_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR1_COEFF_MASK) >> CSC_RGB2ES_3L_MCR1_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR1_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR1_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR1_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 3 of 9) (MCR2)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 3 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR2_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR2_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR2_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR2_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR2_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR2_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR2_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR2_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR2_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR2_COEFF_MASK) >> CSC_RGB2ES_3L_MCR2_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR2_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR2_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR2_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 4 of 9) (MCR3)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 4 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR3_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR3_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR3_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR3_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR3_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR3_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR3_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR3_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR3_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR3_COEFF_MASK) >> CSC_RGB2ES_3L_MCR3_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR3_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR3_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR3_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 5 of 9) (MCR4)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 5 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR4_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR4_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR4_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR4_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR4_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR4_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR4_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR4_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR4_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR4_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR4_COEFF_MASK) >> CSC_RGB2ES_3L_MCR4_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR4_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR4_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR4_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 6 of 9) (MCR5)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 6 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR5_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR5_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR5_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR5_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR5_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR5_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR5_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR5_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR5_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR5_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR5_COEFF_MASK) >> CSC_RGB2ES_3L_MCR5_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR5_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR5_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR5_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 7 of 9) (MCR6)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 7 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR6_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR6_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR6_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR6_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR6_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR6_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR6_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR6_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR6_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR6_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR6_COEFF_MASK) >> CSC_RGB2ES_3L_MCR6_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR6_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR6_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR6_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 8 of 9) (MCR7)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 8 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR7_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR7_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR7_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR7_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR7_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR7_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR7_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR7_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR7_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR7_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR7_COEFF_MASK) >> CSC_RGB2ES_3L_MCR7_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR7_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR7_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR7_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 9 of 9) (MCR8)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 16384 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 9 of 9)*/
//====================================================================

#define  CSC_RGB2ES_3L_MCR8_RESERVED1_MASK 0xffff0000
#define  CSC_RGB2ES_3L_MCR8_RESERVED1_SHIFT 16
#define  CSC_RGB2ES_3L_MCR8_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR8_RESERVED1_MASK) >> CSC_RGB2ES_3L_MCR8_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_MCR8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR8_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR8_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_MCR8_COEFF_MASK 0xffff
#define  CSC_RGB2ES_3L_MCR8_COEFF_SHIFT 0
#define  CSC_RGB2ES_3L_MCR8_COEFF_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_MCR8_COEFF_MASK) >> CSC_RGB2ES_3L_MCR8_COEFF_SHIFT)
#define  CSC_RGB2ES_3L_MCR8_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_MCR8_COEFF_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_MCR8_COEFF_SHIFT))

//====================================================================
//Register: CSC Output Look Up Table (OLIW) (OLIW)
/** \brief <p class=Body>
      The output LUT Interface Window (OLIW) allows the processor access to the output LUT. The LUT
      is only accessible through this window if the Processor LUT Access bit of the CCR is set.
      If the Processor LUT Access bit is not set, writes to this address range will have no affect,
      and reads will always return all zeros. Firmware should ensure the pipe is idle before
      setting the Processor LUT Access bit or attempting to load the LUT through the LIW.
      </p><p class=Body>
      When the Processor LUT Access bit is set, the LUT can be written to and read from using
      this address range. The processor address bits from SRAM_ADDRESS_WIDTH+1 down to 2 are
      used to address each word of the SRAM. Note that the LUT is only accessible at 32 bit
      word addresses in the processor address space, even though fewer than 32 bits are available
      at each address location. The upper bits of each 32 bit "word" will always read back as
      zeros. </p>
      <p class=Body>
      With NUM_OLUTS = 3, the physical SRAM size will not be a power of two and SRAM_ADDRESS_WIDTH
      will need to be sized to allow for the actual size of the SRAM, however the SRAM will not
      use the entire address space that is thus reserved. For example, a 256 location LUT with
      NUM_OLUTS = 3 would be addressed as follows (adding the base address of the OLIW register
      to the values shown): </p>
      <p class=Body>
      SRAM_ADDRESS_WIDTH = 10 (need 1024 locations to hold the 768 actual locations) <br />
      Color1 offsets: 0x000-0x3FC <br />
      Color2 offsets: 0x400-0x7FC <br />
      Color3 offsets: 0x800-0xBFF <br />
      Unused address space: 0xC00-FFF <br /> </p>
      <p class=Body>
      In implementations with the NUM_OLUTS parameter set to 3 the physical LUT will be logically
      divided into three even blocks and each block will have its own LUT Overflow Register.
      This means that when NUM_OLUTS = 3 the overflow registers for the Color1 and Color2 tables
      will not be contiguous with the main LUT data, but will follow the Color3 overflow data in
      the address map. </p>
      <p class=Body>
      The width of the LUT Value is set by the value given to the OUTPUT_WIDTH parameter.
      </p>*/
//====================================================================

#define  CSC_RGB2ES_3L_OLIW_RESERVED1_MASK 0xfffffc00
#define  CSC_RGB2ES_3L_OLIW_RESERVED1_SHIFT 10
#define  CSC_RGB2ES_3L_OLIW_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLIW_RESERVED1_MASK) >> CSC_RGB2ES_3L_OLIW_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_OLIW_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLIW_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLIW_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_OLIW_LUTVAL_MASK 0x3ff
#define  CSC_RGB2ES_3L_OLIW_LUTVAL_SHIFT 0
#define  CSC_RGB2ES_3L_OLIW_LUTVAL_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLIW_LUTVAL_MASK) >> CSC_RGB2ES_3L_OLIW_LUTVAL_SHIFT)
#define  CSC_RGB2ES_3L_OLIW_LUTVAL_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLIW_LUTVAL_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLIW_LUTVAL_SHIFT))

//====================================================================
//Register: Output LUT Overflow Register (OLOR) (Instance 1 of 3) (OLOR0)
/** \brief The output LUT Overflow Register(s) (OLORx) protect against the address overflow
      possibility that is inherent in linear interpolation addressing schemes. When the LUT
      address is at its maximum value, the required "address + 1" entry is not present in the LUT.
      Instead, it exists in the LOR1. The Processor LUT Access bit in the CCR must be set in
      order to write to this register. (Instance 1 of 3)*/
//====================================================================

#define  CSC_RGB2ES_3L_OLOR0_RESERVED1_MASK 0xfffffc00
#define  CSC_RGB2ES_3L_OLOR0_RESERVED1_SHIFT 10
#define  CSC_RGB2ES_3L_OLOR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLOR0_RESERVED1_MASK) >> CSC_RGB2ES_3L_OLOR0_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_OLOR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLOR0_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLOR0_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_OLOR0_LUTVAL_MASK 0x3ff
#define  CSC_RGB2ES_3L_OLOR0_LUTVAL_SHIFT 0
#define  CSC_RGB2ES_3L_OLOR0_LUTVAL_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLOR0_LUTVAL_MASK) >> CSC_RGB2ES_3L_OLOR0_LUTVAL_SHIFT)
#define  CSC_RGB2ES_3L_OLOR0_LUTVAL_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLOR0_LUTVAL_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLOR0_LUTVAL_SHIFT))

//====================================================================
//Register: Output LUT Overflow Register (OLOR) (Instance 2 of 3) (OLOR1)
/** \brief The output LUT Overflow Register(s) (OLORx) protect against the address overflow
      possibility that is inherent in linear interpolation addressing schemes. When the LUT
      address is at its maximum value, the required "address + 1" entry is not present in the LUT.
      Instead, it exists in the LOR1. The Processor LUT Access bit in the CCR must be set in
      order to write to this register. (Instance 2 of 3)*/
//====================================================================

#define  CSC_RGB2ES_3L_OLOR1_RESERVED1_MASK 0xfffffc00
#define  CSC_RGB2ES_3L_OLOR1_RESERVED1_SHIFT 10
#define  CSC_RGB2ES_3L_OLOR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLOR1_RESERVED1_MASK) >> CSC_RGB2ES_3L_OLOR1_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_OLOR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLOR1_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLOR1_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_OLOR1_LUTVAL_MASK 0x3ff
#define  CSC_RGB2ES_3L_OLOR1_LUTVAL_SHIFT 0
#define  CSC_RGB2ES_3L_OLOR1_LUTVAL_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLOR1_LUTVAL_MASK) >> CSC_RGB2ES_3L_OLOR1_LUTVAL_SHIFT)
#define  CSC_RGB2ES_3L_OLOR1_LUTVAL_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLOR1_LUTVAL_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLOR1_LUTVAL_SHIFT))

//====================================================================
//Register: Output LUT Overflow Register (OLOR) (Instance 3 of 3) (OLOR2)
/** \brief The output LUT Overflow Register(s) (OLORx) protect against the address overflow
      possibility that is inherent in linear interpolation addressing schemes. When the LUT
      address is at its maximum value, the required "address + 1" entry is not present in the LUT.
      Instead, it exists in the LOR1. The Processor LUT Access bit in the CCR must be set in
      order to write to this register. (Instance 3 of 3)*/
//====================================================================

#define  CSC_RGB2ES_3L_OLOR2_RESERVED1_MASK 0xfffffc00
#define  CSC_RGB2ES_3L_OLOR2_RESERVED1_SHIFT 10
#define  CSC_RGB2ES_3L_OLOR2_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLOR2_RESERVED1_MASK) >> CSC_RGB2ES_3L_OLOR2_RESERVED1_SHIFT)
#define  CSC_RGB2ES_3L_OLOR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLOR2_RESERVED1_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLOR2_RESERVED1_SHIFT))

#define  CSC_RGB2ES_3L_OLOR2_LUTVAL_MASK 0x3ff
#define  CSC_RGB2ES_3L_OLOR2_LUTVAL_SHIFT 0
#define  CSC_RGB2ES_3L_OLOR2_LUTVAL_MASK_SHIFT(reg) (((reg) & CSC_RGB2ES_3L_OLOR2_LUTVAL_MASK) >> CSC_RGB2ES_3L_OLOR2_LUTVAL_SHIFT)
#define  CSC_RGB2ES_3L_OLOR2_LUTVAL_REPLACE_VAL(reg,val) (((reg) & ~CSC_RGB2ES_3L_OLOR2_LUTVAL_MASK) | (((uint32_t)val) << CSC_RGB2ES_3L_OLOR2_LUTVAL_SHIFT))

#endif // CSC_RGB2ES_3L
