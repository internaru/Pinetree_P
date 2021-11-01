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
 * \file TP_CONTONE_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TP_CONTONE_REGSTRUCTS_H_
#define _TP_CONTONE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Contone Pad (TP_CONTONE)
/** \brief Top-level register file for the Text Pad block*/
//
//====================================================================

typedef struct TP_CONTONE_REGS_s
{
  volatile uint32_t Cfg;  ///< 0x0 [R/W]: Text Pad Configuration Register
  volatile uint32_t LPad;  ///< 0x4 [R/W]: Left Side Padding Register
  volatile uint32_t LVal;  ///< 0x8 [R/W]: Left Side Padding Value Register
  volatile uint32_t RPad;  ///< 0xc [R/W]: Right Side Padding Register
  volatile uint32_t RVal;  ///< 0x10 [R/W]: Right Side Padding Value Register
  volatile uint32_t TPPR;  ///< 0x14 [R]: Text Pad Parameter Register
} TP_CONTONE_REGS_t;

#endif // TP_CONTONE
