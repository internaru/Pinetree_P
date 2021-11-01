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
 * \file KMOD_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _KMOD_REGSTRUCTS_H_
#define _KMOD_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Black Modification Registers (KMOD)
/** \brief This file defines the registers for the Black Modification Module*/
//
//====================================================================

typedef struct KMOD_REGS_s
{
  volatile uint32_t KMDC1R;  ///< 0x0 [R/W]: KMOD Control 1 Register
  volatile uint32_t reserved0;
  volatile uint32_t KMDC2R;  ///< 0x8 [R/W]: KMOD Control 2 Register
  volatile uint32_t KMDLWR;  ///< 0xc [R/W]: KMOD Line Width Register
  volatile uint32_t KMDSR;  ///< 0x10 [R]: KMOD Status Register
  volatile uint32_t reserved1[59];
  volatile uint32_t KMODPMM[55];  ///< 0x100 [R/W]: KMOD Pattern Matrix Memory
  volatile uint32_t reserved2[9];
  volatile uint32_t KMODDMM[64];  ///< 0x200 [R/W]: KMOD Dither Matrix Memory
} KMOD_REGS_t;

#endif // KMOD
