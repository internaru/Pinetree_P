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
 * \file CHG_OWI_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CHG_OWI_REGSTRUCTS_H_
#define _CHG_OWI_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CHG OWI (CHG_OWI)
/** \brief Registers for the 1-Wire Bus Master block*/
//
//====================================================================

typedef struct CHG_OWI_REGS_s
{
  volatile uint32_t W1CMDR;  ///< 0x0 [R/W]: OneWire Command Register
  volatile uint32_t W1TRR;  ///< 0x4 [R/W]: OneWire Transmit/Receive Buffer
  volatile uint32_t W1INTR;  ///< 0x8 [R]: OneWire Interrupt Register
  volatile uint32_t W1IER;  ///< 0xc [R/W]: OneWire Interrupt Enable Register
  volatile uint32_t W1CDR;  ///< 0x10 [R/W]: OneWire Clock Divisor Register
} CHG_OWI_REGS_t;

#endif // CHG_OWI
