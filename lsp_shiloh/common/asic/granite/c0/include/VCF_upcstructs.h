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
 * \file VCF_upcstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_UPC_REGSTRUCTS_H_
#define _VCF_UPC_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF UPC Registers (VCF_UPC)
//
//====================================================================

typedef struct VCF_UPC_REGS_s
{
  volatile uint32_t VUCR;  ///< 0x0 [R/W]: VCF UPC Config Register
  volatile uint32_t VUHR0;  ///< 0x4 [W]: VCF UPC Hash Register (Instance 1 of 2)
  volatile uint32_t VUHR1;  ///< 0x8 [W]: VCF UPC Hash Register (Instance 2 of 2)
  volatile uint32_t VULR;  ///< 0xc [R/W]: VCF UPC Laser Register
} VCF_UPC_REGS_t;

#endif // VCF_UPC
