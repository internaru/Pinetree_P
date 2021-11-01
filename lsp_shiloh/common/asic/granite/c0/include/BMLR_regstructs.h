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
 * \file BMLR_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BMLR_REGSTRUCTS_H_
#define _BMLR_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BulbMonLRMargin (BMLR)
/** \brief Register Descriptions for the PIC BulbMonLRMargin block*/
//
//====================================================================

typedef struct BMLR_REGS_s
{
  volatile uint32_t BMCR;  ///< 0x0 [R/W]: Bulb Monitor Configuration Register
  volatile uint32_t LRMR0;  ///< 0x4 [R/W]: Left/Right Margins Register
  volatile uint32_t LRMR1;  ///< 0x8 [R/W]: Left/Right Margins Register
  volatile uint32_t LRMR2;  ///< 0xc [R/W]: Left/Right Margins Register
  volatile uint32_t BMR0;  ///< 0x10 [R]: Bulb Monitor Color0 Register
  volatile uint32_t BMR1;  ///< 0x14 [R]: Bulb Monitor Color1 Register
  volatile uint32_t BMR2;  ///< 0x18 [R]: Bulb Monitor Color2 Register
  volatile uint32_t BMR3;  ///< 0x1c [R]: Bulb Monitor Exposure Register
} BMLR_REGS_t;

#endif // BMLR
