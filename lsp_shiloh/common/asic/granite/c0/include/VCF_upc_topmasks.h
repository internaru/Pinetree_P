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
 * \file VCF_upc_topmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_UPC_TOP_REGMASKS_H_
#define _VCF_UPC_TOP_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF UPC Registers (VCF_UPC_TOP)
//
//====================================================================

//====================================================================
//
//Register File: UPC (UPC)
//
//====================================================================

//====================================================================
//Register: VCF UPC Config Register (VUCR)
//====================================================================

#define  UPC_VUCR_RESERVED1_MASK 0xfffffff0
#define  UPC_VUCR_RESERVED1_SHIFT 4
#define  UPC_VUCR_RESERVED1_MASK_SHIFT(reg) (((reg) & UPC_VUCR_RESERVED1_MASK) >> UPC_VUCR_RESERVED1_SHIFT)
#define  UPC_VUCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUCR_RESERVED1_MASK) | (((uint32_t)val) << UPC_VUCR_RESERVED1_SHIFT))

#define  UPC_VUCR_UPCBLOCKALL_MASK 0x8
#define  UPC_VUCR_UPCBLOCKALL_SHIFT 3
#define  UPC_VUCR_UPCBLOCKALL_MASK_SHIFT(reg) (((reg) & UPC_VUCR_UPCBLOCKALL_MASK) >> UPC_VUCR_UPCBLOCKALL_SHIFT)
#define  UPC_VUCR_UPCBLOCKALL_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUCR_UPCBLOCKALL_MASK) | (((uint32_t)val) << UPC_VUCR_UPCBLOCKALL_SHIFT))

#define  UPC_VUCR_LASERACCUMSEL_MASK 0x4
#define  UPC_VUCR_LASERACCUMSEL_SHIFT 2
#define  UPC_VUCR_LASERACCUMSEL_MASK_SHIFT(reg) (((reg) & UPC_VUCR_LASERACCUMSEL_MASK) >> UPC_VUCR_LASERACCUMSEL_SHIFT)
#define  UPC_VUCR_LASERACCUMSEL_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUCR_LASERACCUMSEL_MASK) | (((uint32_t)val) << UPC_VUCR_LASERACCUMSEL_SHIFT))

#define  UPC_VUCR_ALTINKMODE_MASK 0x2
#define  UPC_VUCR_ALTINKMODE_SHIFT 1
#define  UPC_VUCR_ALTINKMODE_MASK_SHIFT(reg) (((reg) & UPC_VUCR_ALTINKMODE_MASK) >> UPC_VUCR_ALTINKMODE_SHIFT)
#define  UPC_VUCR_ALTINKMODE_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUCR_ALTINKMODE_MASK) | (((uint32_t)val) << UPC_VUCR_ALTINKMODE_SHIFT))

#define  UPC_VUCR_PRINTACTIVITY_MASK 0x1
#define  UPC_VUCR_PRINTACTIVITY_SHIFT 0
#define  UPC_VUCR_PRINTACTIVITY_MASK_SHIFT(reg) (((reg) & UPC_VUCR_PRINTACTIVITY_MASK) >> UPC_VUCR_PRINTACTIVITY_SHIFT)
#define  UPC_VUCR_PRINTACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUCR_PRINTACTIVITY_MASK) | (((uint32_t)val) << UPC_VUCR_PRINTACTIVITY_SHIFT))

//====================================================================
//Register: VCF UPC Hash Register (Instance 1 of 2) (VUHR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  UPC_VUHR0_RESERVED1_MASK 0xfffffffe
#define  UPC_VUHR0_RESERVED1_SHIFT 1
#define  UPC_VUHR0_RESERVED1_MASK_SHIFT(reg) (((reg) & UPC_VUHR0_RESERVED1_MASK) >> UPC_VUHR0_RESERVED1_SHIFT)
#define  UPC_VUHR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUHR0_RESERVED1_MASK) | (((uint32_t)val) << UPC_VUHR0_RESERVED1_SHIFT))

#define  UPC_VUHR0_IHASHPASS_MASK 0x1
#define  UPC_VUHR0_IHASHPASS_SHIFT 0
#define  UPC_VUHR0_IHASHPASS_MASK_SHIFT(reg) (((reg) & UPC_VUHR0_IHASHPASS_MASK) >> UPC_VUHR0_IHASHPASS_SHIFT)
#define  UPC_VUHR0_IHASHPASS_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUHR0_IHASHPASS_MASK) | (((uint32_t)val) << UPC_VUHR0_IHASHPASS_SHIFT))

//====================================================================
//Register: VCF UPC Hash Register (Instance 2 of 2) (VUHR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  UPC_VUHR1_RESERVED1_MASK 0xfffffffe
#define  UPC_VUHR1_RESERVED1_SHIFT 1
#define  UPC_VUHR1_RESERVED1_MASK_SHIFT(reg) (((reg) & UPC_VUHR1_RESERVED1_MASK) >> UPC_VUHR1_RESERVED1_SHIFT)
#define  UPC_VUHR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUHR1_RESERVED1_MASK) | (((uint32_t)val) << UPC_VUHR1_RESERVED1_SHIFT))

#define  UPC_VUHR1_IHASHPASS_MASK 0x1
#define  UPC_VUHR1_IHASHPASS_SHIFT 0
#define  UPC_VUHR1_IHASHPASS_MASK_SHIFT(reg) (((reg) & UPC_VUHR1_IHASHPASS_MASK) >> UPC_VUHR1_IHASHPASS_SHIFT)
#define  UPC_VUHR1_IHASHPASS_REPLACE_VAL(reg,val) (((reg) & ~UPC_VUHR1_IHASHPASS_MASK) | (((uint32_t)val) << UPC_VUHR1_IHASHPASS_SHIFT))

//====================================================================
//Register: VCF UPC Laser Register (VULR)
//====================================================================

#define  UPC_VULR_RESERVED1_MASK 0x80000000
#define  UPC_VULR_RESERVED1_SHIFT 31
#define  UPC_VULR_RESERVED1_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED1_MASK) >> UPC_VULR_RESERVED1_SHIFT)
#define  UPC_VULR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED1_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED1_SHIFT))

#define  UPC_VULR_CHAN7_MASK 0x70000000
#define  UPC_VULR_CHAN7_SHIFT 28
#define  UPC_VULR_CHAN7_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN7_MASK) >> UPC_VULR_CHAN7_SHIFT)
#define  UPC_VULR_CHAN7_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN7_MASK) | (((uint32_t)val) << UPC_VULR_CHAN7_SHIFT))

#define  UPC_VULR_RESERVED2_MASK 0x8000000
#define  UPC_VULR_RESERVED2_SHIFT 27
#define  UPC_VULR_RESERVED2_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED2_MASK) >> UPC_VULR_RESERVED2_SHIFT)
#define  UPC_VULR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED2_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED2_SHIFT))

#define  UPC_VULR_CHAN6_MASK 0x7000000
#define  UPC_VULR_CHAN6_SHIFT 24
#define  UPC_VULR_CHAN6_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN6_MASK) >> UPC_VULR_CHAN6_SHIFT)
#define  UPC_VULR_CHAN6_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN6_MASK) | (((uint32_t)val) << UPC_VULR_CHAN6_SHIFT))

#define  UPC_VULR_RESERVED3_MASK 0x800000
#define  UPC_VULR_RESERVED3_SHIFT 23
#define  UPC_VULR_RESERVED3_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED3_MASK) >> UPC_VULR_RESERVED3_SHIFT)
#define  UPC_VULR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED3_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED3_SHIFT))

#define  UPC_VULR_CHAN5_MASK 0x700000
#define  UPC_VULR_CHAN5_SHIFT 20
#define  UPC_VULR_CHAN5_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN5_MASK) >> UPC_VULR_CHAN5_SHIFT)
#define  UPC_VULR_CHAN5_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN5_MASK) | (((uint32_t)val) << UPC_VULR_CHAN5_SHIFT))

#define  UPC_VULR_RESERVED4_MASK 0x80000
#define  UPC_VULR_RESERVED4_SHIFT 19
#define  UPC_VULR_RESERVED4_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED4_MASK) >> UPC_VULR_RESERVED4_SHIFT)
#define  UPC_VULR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED4_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED4_SHIFT))

#define  UPC_VULR_CHAN4_MASK 0x70000
#define  UPC_VULR_CHAN4_SHIFT 16
#define  UPC_VULR_CHAN4_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN4_MASK) >> UPC_VULR_CHAN4_SHIFT)
#define  UPC_VULR_CHAN4_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN4_MASK) | (((uint32_t)val) << UPC_VULR_CHAN4_SHIFT))

#define  UPC_VULR_RESERVED5_MASK 0x8000
#define  UPC_VULR_RESERVED5_SHIFT 15
#define  UPC_VULR_RESERVED5_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED5_MASK) >> UPC_VULR_RESERVED5_SHIFT)
#define  UPC_VULR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED5_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED5_SHIFT))

#define  UPC_VULR_CHAN3_MASK 0x7000
#define  UPC_VULR_CHAN3_SHIFT 12
#define  UPC_VULR_CHAN3_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN3_MASK) >> UPC_VULR_CHAN3_SHIFT)
#define  UPC_VULR_CHAN3_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN3_MASK) | (((uint32_t)val) << UPC_VULR_CHAN3_SHIFT))

#define  UPC_VULR_RESERVED6_MASK 0x800
#define  UPC_VULR_RESERVED6_SHIFT 11
#define  UPC_VULR_RESERVED6_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED6_MASK) >> UPC_VULR_RESERVED6_SHIFT)
#define  UPC_VULR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED6_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED6_SHIFT))

#define  UPC_VULR_CHAN2_MASK 0x700
#define  UPC_VULR_CHAN2_SHIFT 8
#define  UPC_VULR_CHAN2_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN2_MASK) >> UPC_VULR_CHAN2_SHIFT)
#define  UPC_VULR_CHAN2_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN2_MASK) | (((uint32_t)val) << UPC_VULR_CHAN2_SHIFT))

#define  UPC_VULR_RESERVED7_MASK 0x80
#define  UPC_VULR_RESERVED7_SHIFT 7
#define  UPC_VULR_RESERVED7_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED7_MASK) >> UPC_VULR_RESERVED7_SHIFT)
#define  UPC_VULR_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED7_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED7_SHIFT))

#define  UPC_VULR_CHAN1_MASK 0x70
#define  UPC_VULR_CHAN1_SHIFT 4
#define  UPC_VULR_CHAN1_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN1_MASK) >> UPC_VULR_CHAN1_SHIFT)
#define  UPC_VULR_CHAN1_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN1_MASK) | (((uint32_t)val) << UPC_VULR_CHAN1_SHIFT))

#define  UPC_VULR_RESERVED8_MASK 0x8
#define  UPC_VULR_RESERVED8_SHIFT 3
#define  UPC_VULR_RESERVED8_MASK_SHIFT(reg) (((reg) & UPC_VULR_RESERVED8_MASK) >> UPC_VULR_RESERVED8_SHIFT)
#define  UPC_VULR_RESERVED8_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_RESERVED8_MASK) | (((uint32_t)val) << UPC_VULR_RESERVED8_SHIFT))

#define  UPC_VULR_CHAN0_MASK 0x7
#define  UPC_VULR_CHAN0_SHIFT 0
#define  UPC_VULR_CHAN0_MASK_SHIFT(reg) (((reg) & UPC_VULR_CHAN0_MASK) >> UPC_VULR_CHAN0_SHIFT)
#define  UPC_VULR_CHAN0_REPLACE_VAL(reg,val) (((reg) & ~UPC_VULR_CHAN0_MASK) | (((uint32_t)val) << UPC_VULR_CHAN0_SHIFT))

//====================================================================
//
//Register File: IRQ (IRQ)
//
//====================================================================

//====================================================================
//Register: VCF Interrupt Enable Register (VIER)
//====================================================================

#define  IRQ_VIER_RESERVED1_MASK 0xffffff00
#define  IRQ_VIER_RESERVED1_SHIFT 8
#define  IRQ_VIER_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VIER_RESERVED1_MASK) >> IRQ_VIER_RESERVED1_SHIFT)
#define  IRQ_VIER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIER_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VIER_RESERVED1_SHIFT))

#define  IRQ_VIER_ENABLERE_MASK 0xf0
#define  IRQ_VIER_ENABLERE_SHIFT 4
#define  IRQ_VIER_ENABLERE_MASK_SHIFT(reg) (((reg) & IRQ_VIER_ENABLERE_MASK) >> IRQ_VIER_ENABLERE_SHIFT)
#define  IRQ_VIER_ENABLERE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIER_ENABLERE_MASK) | (((uint32_t)val) << IRQ_VIER_ENABLERE_SHIFT))

#define  IRQ_VIER_ENABLEFE_MASK 0xf
#define  IRQ_VIER_ENABLEFE_SHIFT 0
#define  IRQ_VIER_ENABLEFE_MASK_SHIFT(reg) (((reg) & IRQ_VIER_ENABLEFE_MASK) >> IRQ_VIER_ENABLEFE_SHIFT)
#define  IRQ_VIER_ENABLEFE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIER_ENABLEFE_MASK) | (((uint32_t)val) << IRQ_VIER_ENABLEFE_SHIFT))

//====================================================================
//Register: VCF Interrupt Pending Register (VIPR)
//====================================================================

#define  IRQ_VIPR_RESERVED1_MASK 0xffffff00
#define  IRQ_VIPR_RESERVED1_SHIFT 8
#define  IRQ_VIPR_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VIPR_RESERVED1_MASK) >> IRQ_VIPR_RESERVED1_SHIFT)
#define  IRQ_VIPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIPR_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VIPR_RESERVED1_SHIFT))

#define  IRQ_VIPR_PENDINGRE_MASK 0xf0
#define  IRQ_VIPR_PENDINGRE_SHIFT 4
#define  IRQ_VIPR_PENDINGRE_MASK_SHIFT(reg) (((reg) & IRQ_VIPR_PENDINGRE_MASK) >> IRQ_VIPR_PENDINGRE_SHIFT)
#define  IRQ_VIPR_PENDINGRE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIPR_PENDINGRE_MASK) | (((uint32_t)val) << IRQ_VIPR_PENDINGRE_SHIFT))

#define  IRQ_VIPR_PENDINGFE_MASK 0xf
#define  IRQ_VIPR_PENDINGFE_SHIFT 0
#define  IRQ_VIPR_PENDINGFE_MASK_SHIFT(reg) (((reg) & IRQ_VIPR_PENDINGFE_MASK) >> IRQ_VIPR_PENDINGFE_SHIFT)
#define  IRQ_VIPR_PENDINGFE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIPR_PENDINGFE_MASK) | (((uint32_t)val) << IRQ_VIPR_PENDINGFE_SHIFT))

//====================================================================
//Register: VCF Interrupt Clear Register (VICR)
//====================================================================

#define  IRQ_VICR_RESERVED1_MASK 0xffffff00
#define  IRQ_VICR_RESERVED1_SHIFT 8
#define  IRQ_VICR_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VICR_RESERVED1_MASK) >> IRQ_VICR_RESERVED1_SHIFT)
#define  IRQ_VICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VICR_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VICR_RESERVED1_SHIFT))

#define  IRQ_VICR_CLEARRE_MASK 0xf0
#define  IRQ_VICR_CLEARRE_SHIFT 4
#define  IRQ_VICR_CLEARRE_MASK_SHIFT(reg) (((reg) & IRQ_VICR_CLEARRE_MASK) >> IRQ_VICR_CLEARRE_SHIFT)
#define  IRQ_VICR_CLEARRE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VICR_CLEARRE_MASK) | (((uint32_t)val) << IRQ_VICR_CLEARRE_SHIFT))

#define  IRQ_VICR_CLEARFE_MASK 0xf
#define  IRQ_VICR_CLEARFE_SHIFT 0
#define  IRQ_VICR_CLEARFE_MASK_SHIFT(reg) (((reg) & IRQ_VICR_CLEARFE_MASK) >> IRQ_VICR_CLEARFE_SHIFT)
#define  IRQ_VICR_CLEARFE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VICR_CLEARFE_MASK) | (((uint32_t)val) << IRQ_VICR_CLEARFE_SHIFT))

//====================================================================
//Register: VCF Interrupt Force Register (VIFR)
//====================================================================

#define  IRQ_VIFR_RESERVED1_MASK 0xffffff00
#define  IRQ_VIFR_RESERVED1_SHIFT 8
#define  IRQ_VIFR_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VIFR_RESERVED1_MASK) >> IRQ_VIFR_RESERVED1_SHIFT)
#define  IRQ_VIFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIFR_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VIFR_RESERVED1_SHIFT))

#define  IRQ_VIFR_FORCERE_MASK 0xf0
#define  IRQ_VIFR_FORCERE_SHIFT 4
#define  IRQ_VIFR_FORCERE_MASK_SHIFT(reg) (((reg) & IRQ_VIFR_FORCERE_MASK) >> IRQ_VIFR_FORCERE_SHIFT)
#define  IRQ_VIFR_FORCERE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIFR_FORCERE_MASK) | (((uint32_t)val) << IRQ_VIFR_FORCERE_SHIFT))

#define  IRQ_VIFR_FORCEFE_MASK 0xf
#define  IRQ_VIFR_FORCEFE_SHIFT 0
#define  IRQ_VIFR_FORCEFE_MASK_SHIFT(reg) (((reg) & IRQ_VIFR_FORCEFE_MASK) >> IRQ_VIFR_FORCEFE_SHIFT)
#define  IRQ_VIFR_FORCEFE_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIFR_FORCEFE_MASK) | (((uint32_t)val) << IRQ_VIFR_FORCEFE_SHIFT))

//====================================================================
//Register: VCF IRQ Input Register (Instance 1 of 2) (VIIR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  IRQ_VIIR0_UPCIRQIN_MASK 0xffffffff
#define  IRQ_VIIR0_UPCIRQIN_SHIFT 0
#define  IRQ_VIIR0_UPCIRQIN_MASK_SHIFT(reg) (((reg) & IRQ_VIIR0_UPCIRQIN_MASK) >> IRQ_VIIR0_UPCIRQIN_SHIFT)
#define  IRQ_VIIR0_UPCIRQIN_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIIR0_UPCIRQIN_MASK) | (((uint32_t)val) << IRQ_VIIR0_UPCIRQIN_SHIFT))

//====================================================================
//Register: VCF IRQ Input Register (Instance 2 of 2) (VIIR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  IRQ_VIIR1_UPCIRQIN_MASK 0xffffffff
#define  IRQ_VIIR1_UPCIRQIN_SHIFT 0
#define  IRQ_VIIR1_UPCIRQIN_MASK_SHIFT(reg) (((reg) & IRQ_VIIR1_UPCIRQIN_MASK) >> IRQ_VIIR1_UPCIRQIN_SHIFT)
#define  IRQ_VIIR1_UPCIRQIN_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIIR1_UPCIRQIN_MASK) | (((uint32_t)val) << IRQ_VIIR1_UPCIRQIN_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 1 of 4) (VIMR0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  IRQ_VIMR0_RESERVED1_MASK 0xffffffc0
#define  IRQ_VIMR0_RESERVED1_SHIFT 6
#define  IRQ_VIMR0_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VIMR0_RESERVED1_MASK) >> IRQ_VIMR0_RESERVED1_SHIFT)
#define  IRQ_VIMR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR0_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VIMR0_RESERVED1_SHIFT))

#define  IRQ_VIMR0_UPCIRQCONFIG_MASK 0x3f
#define  IRQ_VIMR0_UPCIRQCONFIG_SHIFT 0
#define  IRQ_VIMR0_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & IRQ_VIMR0_UPCIRQCONFIG_MASK) >> IRQ_VIMR0_UPCIRQCONFIG_SHIFT)
#define  IRQ_VIMR0_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR0_UPCIRQCONFIG_MASK) | (((uint32_t)val) << IRQ_VIMR0_UPCIRQCONFIG_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 2 of 4) (VIMR1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  IRQ_VIMR1_RESERVED1_MASK 0xffffffc0
#define  IRQ_VIMR1_RESERVED1_SHIFT 6
#define  IRQ_VIMR1_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VIMR1_RESERVED1_MASK) >> IRQ_VIMR1_RESERVED1_SHIFT)
#define  IRQ_VIMR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR1_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VIMR1_RESERVED1_SHIFT))

#define  IRQ_VIMR1_UPCIRQCONFIG_MASK 0x3f
#define  IRQ_VIMR1_UPCIRQCONFIG_SHIFT 0
#define  IRQ_VIMR1_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & IRQ_VIMR1_UPCIRQCONFIG_MASK) >> IRQ_VIMR1_UPCIRQCONFIG_SHIFT)
#define  IRQ_VIMR1_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR1_UPCIRQCONFIG_MASK) | (((uint32_t)val) << IRQ_VIMR1_UPCIRQCONFIG_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 3 of 4) (VIMR2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  IRQ_VIMR2_RESERVED1_MASK 0xffffffc0
#define  IRQ_VIMR2_RESERVED1_SHIFT 6
#define  IRQ_VIMR2_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VIMR2_RESERVED1_MASK) >> IRQ_VIMR2_RESERVED1_SHIFT)
#define  IRQ_VIMR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR2_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VIMR2_RESERVED1_SHIFT))

#define  IRQ_VIMR2_UPCIRQCONFIG_MASK 0x3f
#define  IRQ_VIMR2_UPCIRQCONFIG_SHIFT 0
#define  IRQ_VIMR2_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & IRQ_VIMR2_UPCIRQCONFIG_MASK) >> IRQ_VIMR2_UPCIRQCONFIG_SHIFT)
#define  IRQ_VIMR2_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR2_UPCIRQCONFIG_MASK) | (((uint32_t)val) << IRQ_VIMR2_UPCIRQCONFIG_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 4 of 4) (VIMR3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  IRQ_VIMR3_RESERVED1_MASK 0xffffffc0
#define  IRQ_VIMR3_RESERVED1_SHIFT 6
#define  IRQ_VIMR3_RESERVED1_MASK_SHIFT(reg) (((reg) & IRQ_VIMR3_RESERVED1_MASK) >> IRQ_VIMR3_RESERVED1_SHIFT)
#define  IRQ_VIMR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR3_RESERVED1_MASK) | (((uint32_t)val) << IRQ_VIMR3_RESERVED1_SHIFT))

#define  IRQ_VIMR3_UPCIRQCONFIG_MASK 0x3f
#define  IRQ_VIMR3_UPCIRQCONFIG_SHIFT 0
#define  IRQ_VIMR3_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & IRQ_VIMR3_UPCIRQCONFIG_MASK) >> IRQ_VIMR3_UPCIRQCONFIG_SHIFT)
#define  IRQ_VIMR3_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~IRQ_VIMR3_UPCIRQCONFIG_MASK) | (((uint32_t)val) << IRQ_VIMR3_UPCIRQCONFIG_SHIFT))

#endif // VCF_UPC_TOP
