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
 * \file YIPI_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _YIPI_REGSTRUCTS_H_
#define _YIPI_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Yellow Insertion Pixel Identification (YIPI)
/** \brief Register Descriptions for the ColorPipe YIPI block*/
//
//====================================================================

typedef struct YIPI_REGS_s
{
  volatile uint32_t YCCR;  ///< 0x0 [R/W]: Yellow Insertion Pixel Identification control and configuration register
  volatile uint32_t YCFR;  ///< 0x4 [R/W]: YIPI inserted code and function select register
  volatile uint32_t YAL12;  ///< 0x8 [R/W]: YIPI inserted array information lines 1 and 2
  volatile uint32_t YAL34;  ///< 0xc [R/W]: YIPI inserted array information lines 3 and 4
  volatile uint32_t YAL56;  ///< 0x10 [R/W]: YIPI inserted array information lines 5 and 6
  volatile uint32_t YAL78;  ///< 0x14 [R/W]: YIPI inserted array information lines 7 and 8
  volatile uint32_t YIWR;  ///< 0x18 [R/W]: YIPI image width register
  volatile uint32_t YELC;  ///< 0x1c [R/W]: YIPI Serpentine End-of-Line Configuration
  volatile uint32_t YSR;  ///< 0x20 [R]: YIPI Status Register
  volatile uint32_t YD0R;  ///< 0x24 [R]: YIPI Debug 0 Register
  volatile uint32_t YD1R;  ///< 0x28 [R]: YIPI Debug 1 Register
  volatile uint32_t YR0;  ///< 0x2c [W]: YIPI Restore 0 Register
  volatile uint32_t YR1;  ///< 0x30 [W]: YIPI Restore 1 Register
} YIPI_REGS_t;

#endif // YIPI
