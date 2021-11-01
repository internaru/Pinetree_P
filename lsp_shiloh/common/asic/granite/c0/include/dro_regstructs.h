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
 * \file dro_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DRO_REGSTRUCTS_H_
#define _DRO_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: DRO Monitor (DRO)
/** \brief The ASIC provides multiple instances of the digital ring oscillator (DRO) block for production verification of the silicon, and for active on-chip thermal management. The DRO can be accessed via ASIC pins in analog test mode, or it can be accessed via registers in normal operation. The APB block apb_dro provides the firmware accessible DRO Monitor control and status registers. <BR>
The DRO source is a ring oscillator that outputs a clock up to 1GHz that is proportional to process-voltage-temperature. Measurement logic is provided by the DRO count-down timer and counter which are external to APB. The DRO can take ~700ns (worst case) to get a valid DRO count. That count may be read from the DRO Monitor Status register. The count is continuously updated into the Status register every 2ms when measurement is enabled (see Control register EnableTimer and EnableRO bits).*/
//
//====================================================================

typedef struct DRO_REGS_s
{
  volatile uint32_t CR;  ///< 0x0 [R/W]: DRO Control Register
  volatile uint32_t SR;  ///< 0x4 [R]: DRO Status Register
} DRO_REGS_t;

#endif // DRO
