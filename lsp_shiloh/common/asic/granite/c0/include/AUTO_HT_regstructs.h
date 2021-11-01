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
 * \file AUTO_HT_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _AUTO_HT_REGSTRUCTS_H_
#define _AUTO_HT_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: AUTO_HT (AUTO_HT)
/** \brief Register descriptions for the AUTO_HT block.*/
//
//====================================================================

typedef struct AUTO_HT_REGS_s
{
  volatile uint32_t ACR;  ///< 0x0 [R/W]: AUTO_HT Configuration Register
  volatile uint32_t AAHCR;  ///< 0x4 [R/W]: AH Configuration Register
  volatile uint32_t reserved0[2];
  volatile uint32_t AD0R;  ///< 0x10 [R]: AUTO_HT Debug 0 Register
  volatile uint32_t reserved1[59];
  volatile uint32_t LUT0[512];  ///< 0x100 [R/W]: AUTO_HT LUT 0
  volatile uint32_t LUT1[512];  ///< 0x900 [R/W]: AUTO_HT LUT 1
} AUTO_HT_REGS_t;

#endif // AUTO_HT
