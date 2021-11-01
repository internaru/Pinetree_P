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
 * \file NGAdjust_DAVINCI_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _NGADJUST_DAVINCI_REGSTRUCTS_H_
#define _NGADJUST_DAVINCI_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Neutral Gray Adjust (NGAdjust_DAVINCI)
/** \brief Register Descriptions for the PIP NGAdjust block*/
//
//====================================================================

typedef struct NGADJUST_DAVINCI_REGS_s
{
  volatile uint32_t Config;  ///< 0x0 [R/W]: NGA Configuration Register
  volatile uint32_t perPixEn;  ///< 0x4 [R/W]: Per Pixel Enable Register
  volatile uint32_t reserved0[254];
  volatile uint32_t LUTAccess[256];  ///< 0x400 [R/W]: Look Up Table Access
} NGADJUST_DAVINCI_REGS_t;

#endif // NGADJUST_DAVINCI
