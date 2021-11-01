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
 * \file PIPE_ACCESS_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIPE_ACCESS_REGSTRUCTS_H_
#define _PIPE_ACCESS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIPE_ACCESS (PIPE_ACCESS)
//
//====================================================================

typedef struct PIPE_ACCESS_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t irq_en;  ///< 0x4 [R/W]: Interrupt Enable Register
  volatile uint32_t irq_st;  ///< 0x8 [R]: Interrupt Pending Register
  volatile uint32_t irq_ack;  ///< 0xc [W]: Interrupt Acknowledge Register
  volatile uint32_t irq_force;  ///< 0x10 [W]: Interrupt Force Register
  volatile uint32_t pack_cfg;  ///< 0x14 [R/W]: Packing Configuration
  volatile uint32_t unpack_cfg;  ///< 0x18 [R/W]: Unpacking Configuration
  volatile uint32_t lut_input_sel;  ///< 0x1c [R/W]: 1D LUT 3,4,5 Input Selection
  volatile uint32_t per_pixel_en;  ///< 0x20 [R/W]: 1D LUT Per Pixel Enable Register
  volatile uint32_t stats_cfg0;  ///< 0x24 [R/W]: Stats Configuration Register 0
  volatile uint32_t stats_cfg1;  ///< 0x28 [R/W]: Stats Configuration Register 1
  volatile uint32_t stats_cfg2;  ///< 0x2c [R/W]: Stats Configuration Register 2
  volatile uint32_t stats_cfg3;  ///< 0x30 [R/W]: Stats Configuration Register 3
  volatile uint32_t reserved0[1011];
  volatile uint32_t cpu_lut[1024];  ///< 0x1000 [R/W]: CPU LUT Data
} PIPE_ACCESS_REGS_t;

#endif // PIPE_ACCESS
