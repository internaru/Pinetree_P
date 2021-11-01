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
 * \file VCF_upcmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_UPC_REGMASKS_H_
#define _VCF_UPC_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF UPC Registers (VCF_UPC)
//
//====================================================================

//====================================================================
//Register: VCF UPC Config Register (VUCR)
//====================================================================

#define  VCF_UPC_VUCR_RESERVED1_MASK 0xfffffff0
#define  VCF_UPC_VUCR_RESERVED1_SHIFT 4
#define  VCF_UPC_VUCR_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUCR_RESERVED1_MASK) >> VCF_UPC_VUCR_RESERVED1_SHIFT)
#define  VCF_UPC_VUCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUCR_RESERVED1_MASK) | (((uint32_t)val) << VCF_UPC_VUCR_RESERVED1_SHIFT))

#define  VCF_UPC_VUCR_UPCBLOCKALL_MASK 0x8
#define  VCF_UPC_VUCR_UPCBLOCKALL_SHIFT 3
#define  VCF_UPC_VUCR_UPCBLOCKALL_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUCR_UPCBLOCKALL_MASK) >> VCF_UPC_VUCR_UPCBLOCKALL_SHIFT)
#define  VCF_UPC_VUCR_UPCBLOCKALL_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUCR_UPCBLOCKALL_MASK) | (((uint32_t)val) << VCF_UPC_VUCR_UPCBLOCKALL_SHIFT))

#define  VCF_UPC_VUCR_LASERACCUMSEL_MASK 0x4
#define  VCF_UPC_VUCR_LASERACCUMSEL_SHIFT 2
#define  VCF_UPC_VUCR_LASERACCUMSEL_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUCR_LASERACCUMSEL_MASK) >> VCF_UPC_VUCR_LASERACCUMSEL_SHIFT)
#define  VCF_UPC_VUCR_LASERACCUMSEL_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUCR_LASERACCUMSEL_MASK) | (((uint32_t)val) << VCF_UPC_VUCR_LASERACCUMSEL_SHIFT))

#define  VCF_UPC_VUCR_ALTINKMODE_MASK 0x2
#define  VCF_UPC_VUCR_ALTINKMODE_SHIFT 1
#define  VCF_UPC_VUCR_ALTINKMODE_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUCR_ALTINKMODE_MASK) >> VCF_UPC_VUCR_ALTINKMODE_SHIFT)
#define  VCF_UPC_VUCR_ALTINKMODE_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUCR_ALTINKMODE_MASK) | (((uint32_t)val) << VCF_UPC_VUCR_ALTINKMODE_SHIFT))

#define  VCF_UPC_VUCR_PRINTACTIVITY_MASK 0x1
#define  VCF_UPC_VUCR_PRINTACTIVITY_SHIFT 0
#define  VCF_UPC_VUCR_PRINTACTIVITY_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUCR_PRINTACTIVITY_MASK) >> VCF_UPC_VUCR_PRINTACTIVITY_SHIFT)
#define  VCF_UPC_VUCR_PRINTACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUCR_PRINTACTIVITY_MASK) | (((uint32_t)val) << VCF_UPC_VUCR_PRINTACTIVITY_SHIFT))

//====================================================================
//Register: VCF UPC Hash Register (Instance 1 of 2) (VUHR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  VCF_UPC_VUHR0_RESERVED1_MASK 0xfffffffe
#define  VCF_UPC_VUHR0_RESERVED1_SHIFT 1
#define  VCF_UPC_VUHR0_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUHR0_RESERVED1_MASK) >> VCF_UPC_VUHR0_RESERVED1_SHIFT)
#define  VCF_UPC_VUHR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUHR0_RESERVED1_MASK) | (((uint32_t)val) << VCF_UPC_VUHR0_RESERVED1_SHIFT))

#define  VCF_UPC_VUHR0_IHASHPASS_MASK 0x1
#define  VCF_UPC_VUHR0_IHASHPASS_SHIFT 0
#define  VCF_UPC_VUHR0_IHASHPASS_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUHR0_IHASHPASS_MASK) >> VCF_UPC_VUHR0_IHASHPASS_SHIFT)
#define  VCF_UPC_VUHR0_IHASHPASS_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUHR0_IHASHPASS_MASK) | (((uint32_t)val) << VCF_UPC_VUHR0_IHASHPASS_SHIFT))

//====================================================================
//Register: VCF UPC Hash Register (Instance 2 of 2) (VUHR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  VCF_UPC_VUHR1_RESERVED1_MASK 0xfffffffe
#define  VCF_UPC_VUHR1_RESERVED1_SHIFT 1
#define  VCF_UPC_VUHR1_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUHR1_RESERVED1_MASK) >> VCF_UPC_VUHR1_RESERVED1_SHIFT)
#define  VCF_UPC_VUHR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUHR1_RESERVED1_MASK) | (((uint32_t)val) << VCF_UPC_VUHR1_RESERVED1_SHIFT))

#define  VCF_UPC_VUHR1_IHASHPASS_MASK 0x1
#define  VCF_UPC_VUHR1_IHASHPASS_SHIFT 0
#define  VCF_UPC_VUHR1_IHASHPASS_MASK_SHIFT(reg) (((reg) & VCF_UPC_VUHR1_IHASHPASS_MASK) >> VCF_UPC_VUHR1_IHASHPASS_SHIFT)
#define  VCF_UPC_VUHR1_IHASHPASS_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VUHR1_IHASHPASS_MASK) | (((uint32_t)val) << VCF_UPC_VUHR1_IHASHPASS_SHIFT))

//====================================================================
//Register: VCF UPC Laser Register (VULR)
//====================================================================

#define  VCF_UPC_VULR_RESERVED1_MASK 0x80000000
#define  VCF_UPC_VULR_RESERVED1_SHIFT 31
#define  VCF_UPC_VULR_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED1_MASK) >> VCF_UPC_VULR_RESERVED1_SHIFT)
#define  VCF_UPC_VULR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED1_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED1_SHIFT))

#define  VCF_UPC_VULR_CHAN7_MASK 0x70000000
#define  VCF_UPC_VULR_CHAN7_SHIFT 28
#define  VCF_UPC_VULR_CHAN7_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN7_MASK) >> VCF_UPC_VULR_CHAN7_SHIFT)
#define  VCF_UPC_VULR_CHAN7_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN7_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN7_SHIFT))

#define  VCF_UPC_VULR_RESERVED2_MASK 0x8000000
#define  VCF_UPC_VULR_RESERVED2_SHIFT 27
#define  VCF_UPC_VULR_RESERVED2_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED2_MASK) >> VCF_UPC_VULR_RESERVED2_SHIFT)
#define  VCF_UPC_VULR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED2_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED2_SHIFT))

#define  VCF_UPC_VULR_CHAN6_MASK 0x7000000
#define  VCF_UPC_VULR_CHAN6_SHIFT 24
#define  VCF_UPC_VULR_CHAN6_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN6_MASK) >> VCF_UPC_VULR_CHAN6_SHIFT)
#define  VCF_UPC_VULR_CHAN6_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN6_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN6_SHIFT))

#define  VCF_UPC_VULR_RESERVED3_MASK 0x800000
#define  VCF_UPC_VULR_RESERVED3_SHIFT 23
#define  VCF_UPC_VULR_RESERVED3_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED3_MASK) >> VCF_UPC_VULR_RESERVED3_SHIFT)
#define  VCF_UPC_VULR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED3_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED3_SHIFT))

#define  VCF_UPC_VULR_CHAN5_MASK 0x700000
#define  VCF_UPC_VULR_CHAN5_SHIFT 20
#define  VCF_UPC_VULR_CHAN5_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN5_MASK) >> VCF_UPC_VULR_CHAN5_SHIFT)
#define  VCF_UPC_VULR_CHAN5_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN5_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN5_SHIFT))

#define  VCF_UPC_VULR_RESERVED4_MASK 0x80000
#define  VCF_UPC_VULR_RESERVED4_SHIFT 19
#define  VCF_UPC_VULR_RESERVED4_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED4_MASK) >> VCF_UPC_VULR_RESERVED4_SHIFT)
#define  VCF_UPC_VULR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED4_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED4_SHIFT))

#define  VCF_UPC_VULR_CHAN4_MASK 0x70000
#define  VCF_UPC_VULR_CHAN4_SHIFT 16
#define  VCF_UPC_VULR_CHAN4_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN4_MASK) >> VCF_UPC_VULR_CHAN4_SHIFT)
#define  VCF_UPC_VULR_CHAN4_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN4_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN4_SHIFT))

#define  VCF_UPC_VULR_RESERVED5_MASK 0x8000
#define  VCF_UPC_VULR_RESERVED5_SHIFT 15
#define  VCF_UPC_VULR_RESERVED5_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED5_MASK) >> VCF_UPC_VULR_RESERVED5_SHIFT)
#define  VCF_UPC_VULR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED5_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED5_SHIFT))

#define  VCF_UPC_VULR_CHAN3_MASK 0x7000
#define  VCF_UPC_VULR_CHAN3_SHIFT 12
#define  VCF_UPC_VULR_CHAN3_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN3_MASK) >> VCF_UPC_VULR_CHAN3_SHIFT)
#define  VCF_UPC_VULR_CHAN3_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN3_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN3_SHIFT))

#define  VCF_UPC_VULR_RESERVED6_MASK 0x800
#define  VCF_UPC_VULR_RESERVED6_SHIFT 11
#define  VCF_UPC_VULR_RESERVED6_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED6_MASK) >> VCF_UPC_VULR_RESERVED6_SHIFT)
#define  VCF_UPC_VULR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED6_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED6_SHIFT))

#define  VCF_UPC_VULR_CHAN2_MASK 0x700
#define  VCF_UPC_VULR_CHAN2_SHIFT 8
#define  VCF_UPC_VULR_CHAN2_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN2_MASK) >> VCF_UPC_VULR_CHAN2_SHIFT)
#define  VCF_UPC_VULR_CHAN2_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN2_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN2_SHIFT))

#define  VCF_UPC_VULR_RESERVED7_MASK 0x80
#define  VCF_UPC_VULR_RESERVED7_SHIFT 7
#define  VCF_UPC_VULR_RESERVED7_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED7_MASK) >> VCF_UPC_VULR_RESERVED7_SHIFT)
#define  VCF_UPC_VULR_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED7_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED7_SHIFT))

#define  VCF_UPC_VULR_CHAN1_MASK 0x70
#define  VCF_UPC_VULR_CHAN1_SHIFT 4
#define  VCF_UPC_VULR_CHAN1_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN1_MASK) >> VCF_UPC_VULR_CHAN1_SHIFT)
#define  VCF_UPC_VULR_CHAN1_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN1_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN1_SHIFT))

#define  VCF_UPC_VULR_RESERVED8_MASK 0x8
#define  VCF_UPC_VULR_RESERVED8_SHIFT 3
#define  VCF_UPC_VULR_RESERVED8_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_RESERVED8_MASK) >> VCF_UPC_VULR_RESERVED8_SHIFT)
#define  VCF_UPC_VULR_RESERVED8_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_RESERVED8_MASK) | (((uint32_t)val) << VCF_UPC_VULR_RESERVED8_SHIFT))

#define  VCF_UPC_VULR_CHAN0_MASK 0x7
#define  VCF_UPC_VULR_CHAN0_SHIFT 0
#define  VCF_UPC_VULR_CHAN0_MASK_SHIFT(reg) (((reg) & VCF_UPC_VULR_CHAN0_MASK) >> VCF_UPC_VULR_CHAN0_SHIFT)
#define  VCF_UPC_VULR_CHAN0_REPLACE_VAL(reg,val) (((reg) & ~VCF_UPC_VULR_CHAN0_MASK) | (((uint32_t)val) << VCF_UPC_VULR_CHAN0_SHIFT))

#endif // VCF_UPC
