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
 * \file XYCScale_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _XYCSCALE_REGSTRUCTS_H_
#define _XYCSCALE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: XYCScale (XYCScale)
//
//====================================================================

typedef struct XYCSCALE_REGS_s
{
  volatile uint32_t CSR;  ///< 0x0 [R/W]: Configure X-Y Scale Register
  volatile uint32_t CLCR;  ///< 0x4 [R/W]: Input Strip Column and Line Count Register 
  volatile uint32_t CCR;  ///< 0x8 [R/W]: Output Strip Column Count Register
  volatile uint32_t CFR1;  ///< 0xc [R/W]: Color 2 and Color 1 Fill Data Register
  volatile uint32_t CFR2;  ///< 0x10 [R/W]: Color 0 Fill Data Register
  volatile uint32_t HSR;  ///< 0x14 [R/W]: Horizontal Scale Register
  volatile uint32_t VSR;  ///< 0x18 [R/W]: Vertical Scale Register
  volatile uint32_t reserved0[2];
  volatile uint32_t XTR;  ///< 0x24 [W]: X-Transpose Configuration Register
  volatile uint32_t YTR;  ///< 0x28 [W]: Y-Transpose Configuration Register
  volatile uint32_t CXR;  ///< 0x2c [R]: Current X-Start Register
  volatile uint32_t CYR;  ///< 0x30 [R]: Current Y-Start Register
  volatile uint32_t reserved1[3];
  volatile uint32_t XOR;  ///< 0x40 [R]: Observation Register
} XYCSCALE_REGS_t;

#endif // XYCSCALE
