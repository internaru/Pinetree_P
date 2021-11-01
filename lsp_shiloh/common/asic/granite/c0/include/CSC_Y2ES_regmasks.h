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
 * \file CSC_Y2ES_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSC_Y2ES_REGMASKS_H_
#define _CSC_Y2ES_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: YCC2esRGB Configuration (CSC_Y2ES)
/** \brief Register Descriptions for the Y2ES CSC block*/
//
//====================================================================

//====================================================================
//Register: CSC Configuration Register (CCR)
/** \brief The CSC Configuration Register (CCR) manages the configuration of the CSC block. For normal operation of the CSC block, all bits in this register should be set to '0'.*/
//====================================================================

#define  CSC_Y2ES_CCR_RESERVED1_MASK 0xffffff00
#define  CSC_Y2ES_CCR_RESERVED1_SHIFT 8
#define  CSC_Y2ES_CCR_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_RESERVED1_MASK) >> CSC_Y2ES_CCR_RESERVED1_SHIFT)
#define  CSC_Y2ES_CCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_RESERVED1_SHIFT))

#define  CSC_Y2ES_CCR_RESERVED2_MASK 0x80
#define  CSC_Y2ES_CCR_RESERVED2_SHIFT 7
#define  CSC_Y2ES_CCR_RESERVED2_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_RESERVED2_MASK) >> CSC_Y2ES_CCR_RESERVED2_SHIFT)
#define  CSC_Y2ES_CCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_RESERVED2_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_RESERVED2_SHIFT))

#define  CSC_Y2ES_CCR_RESERVED3_MASK 0x40
#define  CSC_Y2ES_CCR_RESERVED3_SHIFT 6
#define  CSC_Y2ES_CCR_RESERVED3_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_RESERVED3_MASK) >> CSC_Y2ES_CCR_RESERVED3_SHIFT)
#define  CSC_Y2ES_CCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_RESERVED3_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_RESERVED3_SHIFT))

#define  CSC_Y2ES_CCR_PREOFFBYPASS_MASK 0x20
#define  CSC_Y2ES_CCR_PREOFFBYPASS_SHIFT 5
#define  CSC_Y2ES_CCR_PREOFFBYPASS_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_PREOFFBYPASS_MASK) >> CSC_Y2ES_CCR_PREOFFBYPASS_SHIFT)
#define  CSC_Y2ES_CCR_PREOFFBYPASS_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_PREOFFBYPASS_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_PREOFFBYPASS_SHIFT))

#define  CSC_Y2ES_CCR_RESERVED4_MASK 0x10
#define  CSC_Y2ES_CCR_RESERVED4_SHIFT 4
#define  CSC_Y2ES_CCR_RESERVED4_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_RESERVED4_MASK) >> CSC_Y2ES_CCR_RESERVED4_SHIFT)
#define  CSC_Y2ES_CCR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_RESERVED4_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_RESERVED4_SHIFT))

#define  CSC_Y2ES_CCR_RESERVED5_MASK 0x8
#define  CSC_Y2ES_CCR_RESERVED5_SHIFT 3
#define  CSC_Y2ES_CCR_RESERVED5_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_RESERVED5_MASK) >> CSC_Y2ES_CCR_RESERVED5_SHIFT)
#define  CSC_Y2ES_CCR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_RESERVED5_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_RESERVED5_SHIFT))

#define  CSC_Y2ES_CCR_RESERVED6_MASK 0x4
#define  CSC_Y2ES_CCR_RESERVED6_SHIFT 2
#define  CSC_Y2ES_CCR_RESERVED6_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_RESERVED6_MASK) >> CSC_Y2ES_CCR_RESERVED6_SHIFT)
#define  CSC_Y2ES_CCR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_RESERVED6_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_RESERVED6_SHIFT))

#define  CSC_Y2ES_CCR_OFFSETBYPASS_MASK 0x2
#define  CSC_Y2ES_CCR_OFFSETBYPASS_SHIFT 1
#define  CSC_Y2ES_CCR_OFFSETBYPASS_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_OFFSETBYPASS_MASK) >> CSC_Y2ES_CCR_OFFSETBYPASS_SHIFT)
#define  CSC_Y2ES_CCR_OFFSETBYPASS_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_OFFSETBYPASS_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_OFFSETBYPASS_SHIFT))

#define  CSC_Y2ES_CCR_BYPASSALL_MASK 0x1
#define  CSC_Y2ES_CCR_BYPASSALL_SHIFT 0
#define  CSC_Y2ES_CCR_BYPASSALL_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_CCR_BYPASSALL_MASK) >> CSC_Y2ES_CCR_BYPASSALL_SHIFT)
#define  CSC_Y2ES_CCR_BYPASSALL_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_CCR_BYPASSALL_MASK) | (((uint32_t)val) << CSC_Y2ES_CCR_BYPASSALL_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 1 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR0_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR0_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR0_RESERVED1_MASK) >> CSC_Y2ES_MCR0_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR0_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR0_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR0_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR0_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR0_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR0_COEFF_MASK) >> CSC_Y2ES_MCR0_COEFF_SHIFT)
#define  CSC_Y2ES_MCR0_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR0_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR0_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 2 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR1_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR1_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR1_RESERVED1_MASK) >> CSC_Y2ES_MCR1_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR1_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR1_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR1_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR1_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR1_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR1_COEFF_MASK) >> CSC_Y2ES_MCR1_COEFF_SHIFT)
#define  CSC_Y2ES_MCR1_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR1_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR1_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 3 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR2_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR2_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR2_RESERVED1_MASK) >> CSC_Y2ES_MCR2_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR2_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR2_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR2_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR2_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR2_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR2_COEFF_MASK) >> CSC_Y2ES_MCR2_COEFF_SHIFT)
#define  CSC_Y2ES_MCR2_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR2_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR2_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 4 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR3_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR3_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR3_RESERVED1_MASK) >> CSC_Y2ES_MCR3_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR3_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR3_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR3_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR3_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR3_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR3_COEFF_MASK) >> CSC_Y2ES_MCR3_COEFF_SHIFT)
#define  CSC_Y2ES_MCR3_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR3_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR3_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 5 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR4_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR4_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR4_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR4_RESERVED1_MASK) >> CSC_Y2ES_MCR4_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR4_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR4_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR4_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR4_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR4_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR4_COEFF_MASK) >> CSC_Y2ES_MCR4_COEFF_SHIFT)
#define  CSC_Y2ES_MCR4_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR4_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR4_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 6 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR5_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR5_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR5_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR5_RESERVED1_MASK) >> CSC_Y2ES_MCR5_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR5_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR5_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR5_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR5_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR5_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR5_COEFF_MASK) >> CSC_Y2ES_MCR5_COEFF_SHIFT)
#define  CSC_Y2ES_MCR5_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR5_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR5_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 7 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR6_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR6_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR6_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR6_RESERVED1_MASK) >> CSC_Y2ES_MCR6_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR6_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR6_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR6_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR6_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR6_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR6_COEFF_MASK) >> CSC_Y2ES_MCR6_COEFF_SHIFT)
#define  CSC_Y2ES_MCR6_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR6_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR6_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 8 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR7_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR7_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR7_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR7_RESERVED1_MASK) >> CSC_Y2ES_MCR7_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR7_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR7_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR7_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR7_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR7_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR7_COEFF_MASK) >> CSC_Y2ES_MCR7_COEFF_SHIFT)
#define  CSC_Y2ES_MCR7_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR7_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR7_COEFF_SHIFT))

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
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 9 of 9)*/
//====================================================================

#define  CSC_Y2ES_MCR8_RESERVED1_MASK 0xffffc000
#define  CSC_Y2ES_MCR8_RESERVED1_SHIFT 14
#define  CSC_Y2ES_MCR8_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR8_RESERVED1_MASK) >> CSC_Y2ES_MCR8_RESERVED1_SHIFT)
#define  CSC_Y2ES_MCR8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR8_RESERVED1_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR8_RESERVED1_SHIFT))

#define  CSC_Y2ES_MCR8_COEFF_MASK 0x3fff
#define  CSC_Y2ES_MCR8_COEFF_SHIFT 0
#define  CSC_Y2ES_MCR8_COEFF_MASK_SHIFT(reg) (((reg) & CSC_Y2ES_MCR8_COEFF_MASK) >> CSC_Y2ES_MCR8_COEFF_SHIFT)
#define  CSC_Y2ES_MCR8_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_Y2ES_MCR8_COEFF_MASK) | (((uint32_t)val) << CSC_Y2ES_MCR8_COEFF_SHIFT))

#endif // CSC_Y2ES
