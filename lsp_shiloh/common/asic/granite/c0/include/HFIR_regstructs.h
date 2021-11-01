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
 * \file HFIR_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HFIR_REGSTRUCTS_H_
#define _HFIR_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Horizontal FIR Filter (HFIR)
/** \brief Register file for the HFIR block*/
//
//====================================================================

typedef struct HFIR_REGS_s
{
  volatile uint32_t Cfg;  ///< 0x0 [R/W]: HFIR Configuration Register
  volatile uint32_t coeff0_1;  ///< 0x4 [R/W]: HFIR Coefficient set 0 and 1
  volatile uint32_t coeff2_3;  ///< 0x8 [R/W]: HFIR Coefficient set 2 and 3
  volatile uint32_t coeff4_5;  ///< 0xc [R/W]: HFIR Coefficient set 4 and 5
  volatile uint32_t coeff6_7;  ///< 0x10 [R/W]: HFIR Coefficient set 6 and 7
  volatile uint32_t coeff8_9;  ///< 0x14 [R/W]: HFIR Coefficient set 8 and 9
  volatile uint32_t coeff10_11;  ///< 0x18 [R/W]: HFIR Coefficient set 10 and 11
  volatile uint32_t coeff12_13;  ///< 0x1c [R/W]: HFIR Coefficient set 12 and 13
  volatile uint32_t coeff14_15;  ///< 0x20 [R/W]: HFIR Coefficient set 14 and 15
  volatile uint32_t CSum;  ///< 0x24 [R]: Check Sum
  volatile uint32_t CHColorDecode;  ///< 0x28 [R/W]: Channel Color Decode
} HFIR_REGS_t;

#endif // HFIR
