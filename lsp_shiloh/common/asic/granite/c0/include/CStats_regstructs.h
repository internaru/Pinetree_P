 /*
 **************************************************************************************
 *
 * Copyright (c) 2011 Marvell International, Ltd.
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
 * \file CStats_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSTATS_REGSTRUCTS_H_
#define _CSTATS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CStats (CStats)
//
//====================================================================

typedef struct CSTATS_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: Configure CStats Register
  volatile uint32_t LM;  ///< 0x4 [R/W]: Left Margin Register
  volatile uint32_t RM;  ///< 0x8 [R/W]: Right Margin Register
  volatile uint32_t SW;  ///< 0xc [R/W]: Section Width
  volatile uint32_t STW;  ///< 0x10 [R/W]: Strip Width
  volatile uint32_t TR1;  ///< 0x14 [R/W]: Threshold Register 1
  volatile uint32_t TR2;  ///< 0x18 [R/W]: Threshold Register 2
  volatile uint32_t SBLK;  ///< 0x1c [R/W]: Strip Black
  volatile uint32_t SGRY;  ///< 0x20 [R/W]: Strip Grey
  volatile uint32_t SC;  ///< 0x24 [R/W]: Strip Color
  volatile uint32_t CFGB;  ///< 0x28 [R/W]: Configure Register B
  volatile uint32_t reserved0[117];
  volatile uint32_t RAM0[128];  ///< 0x200 [R/W]: Ram 0
  volatile uint32_t RAM1[128];  ///< 0x400 [R/W]: Ram 1
  volatile uint32_t RAM2[128];  ///< 0x600 [R/W]: Ram 2
  volatile uint32_t RAM3[32];  ///< 0x800 [R/W]: Ram 3
} CSTATS_REGS_t;

#endif // CSTATS
