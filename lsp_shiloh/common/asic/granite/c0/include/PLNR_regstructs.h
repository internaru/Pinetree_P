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
 * \file PLNR_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PLNR_REGSTRUCTS_H_
#define _PLNR_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Planar Registers (PLNR)
//
//====================================================================

typedef struct PLNR_REGS_s
{
  volatile uint32_t PCR;  ///< 0x0 [R/W]: PLNR Configuration Register
  volatile uint32_t PIWR;  ///< 0x4 [R/W]: PLNR Image Width Register
  volatile uint32_t PIHR;  ///< 0x8 [R/W]: PLNR Image Height Register
  volatile uint32_t reserved0;
  volatile uint32_t PWR0;  ///< 0x10 [W]: PLNR Write Register (Instance 1 of 3)
  volatile uint32_t reserved1[3];
  volatile uint32_t PWR1;  ///< 0x20 [W]: PLNR Write Register (Instance 2 of 3)
  volatile uint32_t reserved2[3];
  volatile uint32_t PWR2;  ///< 0x30 [W]: PLNR Write Register (Instance 3 of 3)
} PLNR_REGS_t;

#endif // PLNR
