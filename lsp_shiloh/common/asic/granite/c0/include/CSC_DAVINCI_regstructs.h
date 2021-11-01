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
 * \file CSC_DAVINCI_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSC_DAVINCI_REGSTRUCTS_H_
#define _CSC_DAVINCI_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIP configuration (CSC_DAVINCI)
/** \brief Register Descriptions for the PIP CSC block*/
//
//====================================================================

typedef struct CSC_DAVINCI_REGS_s
{
  volatile uint32_t CCR;  ///< 0x0 [R/W]: CSC Configuration Register
  volatile uint32_t reserved0[63];
  volatile uint32_t MCR0;  ///< 0x100 [R/W]: Multiply Coefficient Register (Instance 1 of 9)
  volatile uint32_t MCR1;  ///< 0x104 [R/W]: Multiply Coefficient Register (Instance 2 of 9)
  volatile uint32_t MCR2;  ///< 0x108 [R/W]: Multiply Coefficient Register (Instance 3 of 9)
  volatile uint32_t MCR3;  ///< 0x10c [R/W]: Multiply Coefficient Register (Instance 4 of 9)
  volatile uint32_t MCR4;  ///< 0x110 [R/W]: Multiply Coefficient Register (Instance 5 of 9)
  volatile uint32_t MCR5;  ///< 0x114 [R/W]: Multiply Coefficient Register (Instance 6 of 9)
  volatile uint32_t MCR6;  ///< 0x118 [R/W]: Multiply Coefficient Register (Instance 7 of 9)
  volatile uint32_t MCR7;  ///< 0x11c [R/W]: Multiply Coefficient Register (Instance 8 of 9)
  volatile uint32_t MCR8;  ///< 0x120 [R/W]: Multiply Coefficient Register (Instance 9 of 9)
  volatile uint32_t reserved1[55];
  volatile uint32_t ILOR0;  ///< 0x200 [R/W]: Input LUT Overflow Register (ILOR) (Instance 1 of 3)
  volatile uint32_t ILOR1;  ///< 0x204 [R/W]: Input LUT Overflow Register (ILOR) (Instance 2 of 3)
  volatile uint32_t ILOR2;  ///< 0x208 [R/W]: Input LUT Overflow Register (ILOR) (Instance 3 of 3)
  volatile uint32_t reserved2[125];
  volatile uint32_t OLOR0;  ///< 0x400 [R/W]: Output LUT Overflow Register (OLOR) (Instance 1 of 3)
  volatile uint32_t OLOR1;  ///< 0x404 [R/W]: Output LUT Overflow Register (OLOR) (Instance 2 of 3)
  volatile uint32_t OLOR2;  ///< 0x408 [R/W]: Output LUT Overflow Register (OLOR) (Instance 3 of 3)
  volatile uint32_t reserved3[765];
  volatile uint32_t ILIW[768];  ///< 0x1000 [R/W]: CSC Input Look Up Table (ILIW)
  volatile uint32_t reserved4[256];
  volatile uint32_t OLIW[768];  ///< 0x2000 [R/W]: CSC Output Look Up Table (OLIW)
} CSC_DAVINCI_REGS_t;

#endif // CSC_DAVINCI
