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
 * \file VCF_rommasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_ROM_REGMASKS_H_
#define _VCF_ROM_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF ROM (VCF_ROM)
//
//====================================================================

//====================================================================
//Register: ROM (ROM)
//====================================================================

#define  VCF_ROM_ROM_VALUE_MASK 0xffffffff
#define  VCF_ROM_ROM_VALUE_SHIFT 0
#define  VCF_ROM_ROM_VALUE_MASK_SHIFT(reg) (((reg) & VCF_ROM_ROM_VALUE_MASK) >> VCF_ROM_ROM_VALUE_SHIFT)
#define  VCF_ROM_ROM_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_ROM_ROM_VALUE_MASK) | (((uint32_t)val) << VCF_ROM_ROM_VALUE_SHIFT))

#endif // VCF_ROM
