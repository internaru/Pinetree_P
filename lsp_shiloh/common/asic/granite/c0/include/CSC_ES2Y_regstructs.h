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
 * \file CSC_ES2Y_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSC_ES2Y_REGSTRUCTS_H_
#define _CSC_ES2Y_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: esRGB2YCC Configuration (CSC_ES2Y)
/** \brief Register Descriptions for the ES2Y CSC block*/
//
//====================================================================

typedef struct CSC_ES2Y_REGS_s
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
} CSC_ES2Y_REGS_t;

#endif // CSC_ES2Y
