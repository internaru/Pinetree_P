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
 * \file NGAdjust_PIE_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _NGADJUST_PIE_REGMASKS_H_
#define _NGADJUST_PIE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Neutral Gray Adjust registers (NGAdjust_PIE)
/** \brief Register Descriptions for the PIE NGAdjust block*/
//
//====================================================================

//====================================================================
//Register: NGA Configuration Register (Config)
/** \brief General configuration register for NGAdjust block.*/
//====================================================================

#define  NGADJUST_PIE_CONFIG_RESERVED1_MASK 0xfffffff8
#define  NGADJUST_PIE_CONFIG_RESERVED1_SHIFT 3
#define  NGADJUST_PIE_CONFIG_RESERVED1_MASK_SHIFT(reg) (((reg) & NGADJUST_PIE_CONFIG_RESERVED1_MASK) >> NGADJUST_PIE_CONFIG_RESERVED1_SHIFT)
#define  NGADJUST_PIE_CONFIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~NGADJUST_PIE_CONFIG_RESERVED1_MASK) | (((uint32_t)val) << NGADJUST_PIE_CONFIG_RESERVED1_SHIFT))

#define  NGADJUST_PIE_CONFIG_METHOD_MASK 0x4
#define  NGADJUST_PIE_CONFIG_METHOD_SHIFT 2
#define  NGADJUST_PIE_CONFIG_METHOD_MASK_SHIFT(reg) (((reg) & NGADJUST_PIE_CONFIG_METHOD_MASK) >> NGADJUST_PIE_CONFIG_METHOD_SHIFT)
#define  NGADJUST_PIE_CONFIG_METHOD_REPLACE_VAL(reg,val) (((reg) & ~NGADJUST_PIE_CONFIG_METHOD_MASK) | (((uint32_t)val) << NGADJUST_PIE_CONFIG_METHOD_SHIFT))

#define  NGADJUST_PIE_CONFIG_CPU_ACCESS_MASK 0x2
#define  NGADJUST_PIE_CONFIG_CPU_ACCESS_SHIFT 1
#define  NGADJUST_PIE_CONFIG_CPU_ACCESS_MASK_SHIFT(reg) (((reg) & NGADJUST_PIE_CONFIG_CPU_ACCESS_MASK) >> NGADJUST_PIE_CONFIG_CPU_ACCESS_SHIFT)
#define  NGADJUST_PIE_CONFIG_CPU_ACCESS_REPLACE_VAL(reg,val) (((reg) & ~NGADJUST_PIE_CONFIG_CPU_ACCESS_MASK) | (((uint32_t)val) << NGADJUST_PIE_CONFIG_CPU_ACCESS_SHIFT))

#define  NGADJUST_PIE_CONFIG_BYPASS_MASK 0x1
#define  NGADJUST_PIE_CONFIG_BYPASS_SHIFT 0
#define  NGADJUST_PIE_CONFIG_BYPASS_MASK_SHIFT(reg) (((reg) & NGADJUST_PIE_CONFIG_BYPASS_MASK) >> NGADJUST_PIE_CONFIG_BYPASS_SHIFT)
#define  NGADJUST_PIE_CONFIG_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~NGADJUST_PIE_CONFIG_BYPASS_MASK) | (((uint32_t)val) << NGADJUST_PIE_CONFIG_BYPASS_SHIFT))

//====================================================================
//Register: Look Up Table Access (LUTAccess)
/** \brief The LUT, look up table, is used for Neutral Gray Adjust. The processor requires Read/Write access to the LUT to set up the values needed. The CPU Access bit must be set in the NGA Configuration register in order to access the LUTs. The LUT values have a smaller width than the data path, since they are intended to shift the incoming pixels somewhat and not over their entire value range.
Note that the processor view of the LUT is 256 registers (1024 for 10-bit data paths), word addressed.*/
//====================================================================

#define  NGADJUST_PIE_LUTACCESS_RESERVED1_MASK 0xffff0000
#define  NGADJUST_PIE_LUTACCESS_RESERVED1_SHIFT 16
#define  NGADJUST_PIE_LUTACCESS_RESERVED1_MASK_SHIFT(reg) (((reg) & NGADJUST_PIE_LUTACCESS_RESERVED1_MASK) >> NGADJUST_PIE_LUTACCESS_RESERVED1_SHIFT)
#define  NGADJUST_PIE_LUTACCESS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~NGADJUST_PIE_LUTACCESS_RESERVED1_MASK) | (((uint32_t)val) << NGADJUST_PIE_LUTACCESS_RESERVED1_SHIFT))

#define  NGADJUST_PIE_LUTACCESS_C1_MASK 0xff00
#define  NGADJUST_PIE_LUTACCESS_C1_SHIFT 8
#define  NGADJUST_PIE_LUTACCESS_C1_MASK_SHIFT(reg) (((reg) & NGADJUST_PIE_LUTACCESS_C1_MASK) >> NGADJUST_PIE_LUTACCESS_C1_SHIFT)
#define  NGADJUST_PIE_LUTACCESS_C1_REPLACE_VAL(reg,val) (((reg) & ~NGADJUST_PIE_LUTACCESS_C1_MASK) | (((uint32_t)val) << NGADJUST_PIE_LUTACCESS_C1_SHIFT))

#define  NGADJUST_PIE_LUTACCESS_C2_MASK 0xff
#define  NGADJUST_PIE_LUTACCESS_C2_SHIFT 0
#define  NGADJUST_PIE_LUTACCESS_C2_MASK_SHIFT(reg) (((reg) & NGADJUST_PIE_LUTACCESS_C2_MASK) >> NGADJUST_PIE_LUTACCESS_C2_SHIFT)
#define  NGADJUST_PIE_LUTACCESS_C2_REPLACE_VAL(reg,val) (((reg) & ~NGADJUST_PIE_LUTACCESS_C2_MASK) | (((uint32_t)val) << NGADJUST_PIE_LUTACCESS_C2_SHIFT))

#endif // NGADJUST_PIE
