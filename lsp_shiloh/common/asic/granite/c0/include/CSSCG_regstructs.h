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
 * \file CSSCG_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSSCG_REGSTRUCTS_H_
#define _CSSCG_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CSSCG Configuration Registers (CSSCG)
/** \brief Top-level register file for the CSSCG block*/
//
//====================================================================

typedef struct CSSCG_REGS_s
{
  volatile uint32_t MODE_CONFIG;  ///< 0x0 [R/W]: CSSCG Mode Configuration Register
  volatile uint32_t HALT_CONFIG;  ///< 0x4 [R/W]: CSSCG Halt Configuration Register
  volatile uint32_t SB_OVER;  ///< 0x8 [R/W]: CSSCG Sign Bit Override Register
  volatile uint32_t CONTROL;  ///< 0xc [R/W]: CSSCG Control Register
  volatile uint32_t STATUS;  ///< 0x10 [R]: CSSCG Status Register
  volatile uint32_t reserved0[251];
  volatile uint32_t CSSCG_RAM[32];  ///< 0x400 [R/W]: CSSCG RAM Access Register
} CSSCG_REGS_t;

#endif // CSSCG
