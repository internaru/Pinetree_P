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
 * \file VCF_irqmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_IRQ_REGMASKS_H_
#define _VCF_IRQ_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF IRQ Registers (VCF_IRQ)
//
//====================================================================

//====================================================================
//Register: VCF Interrupt Enable Register (VIER)
//====================================================================

#define  VCF_IRQ_VIER_RESERVED1_MASK 0xffffff00
#define  VCF_IRQ_VIER_RESERVED1_SHIFT 8
#define  VCF_IRQ_VIER_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIER_RESERVED1_MASK) >> VCF_IRQ_VIER_RESERVED1_SHIFT)
#define  VCF_IRQ_VIER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIER_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VIER_RESERVED1_SHIFT))

#define  VCF_IRQ_VIER_ENABLERE_MASK 0xf0
#define  VCF_IRQ_VIER_ENABLERE_SHIFT 4
#define  VCF_IRQ_VIER_ENABLERE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIER_ENABLERE_MASK) >> VCF_IRQ_VIER_ENABLERE_SHIFT)
#define  VCF_IRQ_VIER_ENABLERE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIER_ENABLERE_MASK) | (((uint32_t)val) << VCF_IRQ_VIER_ENABLERE_SHIFT))

#define  VCF_IRQ_VIER_ENABLEFE_MASK 0xf
#define  VCF_IRQ_VIER_ENABLEFE_SHIFT 0
#define  VCF_IRQ_VIER_ENABLEFE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIER_ENABLEFE_MASK) >> VCF_IRQ_VIER_ENABLEFE_SHIFT)
#define  VCF_IRQ_VIER_ENABLEFE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIER_ENABLEFE_MASK) | (((uint32_t)val) << VCF_IRQ_VIER_ENABLEFE_SHIFT))

//====================================================================
//Register: VCF Interrupt Pending Register (VIPR)
//====================================================================

#define  VCF_IRQ_VIPR_RESERVED1_MASK 0xffffff00
#define  VCF_IRQ_VIPR_RESERVED1_SHIFT 8
#define  VCF_IRQ_VIPR_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIPR_RESERVED1_MASK) >> VCF_IRQ_VIPR_RESERVED1_SHIFT)
#define  VCF_IRQ_VIPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIPR_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VIPR_RESERVED1_SHIFT))

#define  VCF_IRQ_VIPR_PENDINGRE_MASK 0xf0
#define  VCF_IRQ_VIPR_PENDINGRE_SHIFT 4
#define  VCF_IRQ_VIPR_PENDINGRE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIPR_PENDINGRE_MASK) >> VCF_IRQ_VIPR_PENDINGRE_SHIFT)
#define  VCF_IRQ_VIPR_PENDINGRE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIPR_PENDINGRE_MASK) | (((uint32_t)val) << VCF_IRQ_VIPR_PENDINGRE_SHIFT))

#define  VCF_IRQ_VIPR_PENDINGFE_MASK 0xf
#define  VCF_IRQ_VIPR_PENDINGFE_SHIFT 0
#define  VCF_IRQ_VIPR_PENDINGFE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIPR_PENDINGFE_MASK) >> VCF_IRQ_VIPR_PENDINGFE_SHIFT)
#define  VCF_IRQ_VIPR_PENDINGFE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIPR_PENDINGFE_MASK) | (((uint32_t)val) << VCF_IRQ_VIPR_PENDINGFE_SHIFT))

//====================================================================
//Register: VCF Interrupt Clear Register (VICR)
//====================================================================

#define  VCF_IRQ_VICR_RESERVED1_MASK 0xffffff00
#define  VCF_IRQ_VICR_RESERVED1_SHIFT 8
#define  VCF_IRQ_VICR_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VICR_RESERVED1_MASK) >> VCF_IRQ_VICR_RESERVED1_SHIFT)
#define  VCF_IRQ_VICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VICR_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VICR_RESERVED1_SHIFT))

#define  VCF_IRQ_VICR_CLEARRE_MASK 0xf0
#define  VCF_IRQ_VICR_CLEARRE_SHIFT 4
#define  VCF_IRQ_VICR_CLEARRE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VICR_CLEARRE_MASK) >> VCF_IRQ_VICR_CLEARRE_SHIFT)
#define  VCF_IRQ_VICR_CLEARRE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VICR_CLEARRE_MASK) | (((uint32_t)val) << VCF_IRQ_VICR_CLEARRE_SHIFT))

#define  VCF_IRQ_VICR_CLEARFE_MASK 0xf
#define  VCF_IRQ_VICR_CLEARFE_SHIFT 0
#define  VCF_IRQ_VICR_CLEARFE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VICR_CLEARFE_MASK) >> VCF_IRQ_VICR_CLEARFE_SHIFT)
#define  VCF_IRQ_VICR_CLEARFE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VICR_CLEARFE_MASK) | (((uint32_t)val) << VCF_IRQ_VICR_CLEARFE_SHIFT))

//====================================================================
//Register: VCF Interrupt Force Register (VIFR)
//====================================================================

#define  VCF_IRQ_VIFR_RESERVED1_MASK 0xffffff00
#define  VCF_IRQ_VIFR_RESERVED1_SHIFT 8
#define  VCF_IRQ_VIFR_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIFR_RESERVED1_MASK) >> VCF_IRQ_VIFR_RESERVED1_SHIFT)
#define  VCF_IRQ_VIFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIFR_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VIFR_RESERVED1_SHIFT))

#define  VCF_IRQ_VIFR_FORCERE_MASK 0xf0
#define  VCF_IRQ_VIFR_FORCERE_SHIFT 4
#define  VCF_IRQ_VIFR_FORCERE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIFR_FORCERE_MASK) >> VCF_IRQ_VIFR_FORCERE_SHIFT)
#define  VCF_IRQ_VIFR_FORCERE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIFR_FORCERE_MASK) | (((uint32_t)val) << VCF_IRQ_VIFR_FORCERE_SHIFT))

#define  VCF_IRQ_VIFR_FORCEFE_MASK 0xf
#define  VCF_IRQ_VIFR_FORCEFE_SHIFT 0
#define  VCF_IRQ_VIFR_FORCEFE_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIFR_FORCEFE_MASK) >> VCF_IRQ_VIFR_FORCEFE_SHIFT)
#define  VCF_IRQ_VIFR_FORCEFE_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIFR_FORCEFE_MASK) | (((uint32_t)val) << VCF_IRQ_VIFR_FORCEFE_SHIFT))

//====================================================================
//Register: VCF IRQ Input Register (Instance 1 of 2) (VIIR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  VCF_IRQ_VIIR0_UPCIRQIN_MASK 0xffffffff
#define  VCF_IRQ_VIIR0_UPCIRQIN_SHIFT 0
#define  VCF_IRQ_VIIR0_UPCIRQIN_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIIR0_UPCIRQIN_MASK) >> VCF_IRQ_VIIR0_UPCIRQIN_SHIFT)
#define  VCF_IRQ_VIIR0_UPCIRQIN_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIIR0_UPCIRQIN_MASK) | (((uint32_t)val) << VCF_IRQ_VIIR0_UPCIRQIN_SHIFT))

//====================================================================
//Register: VCF IRQ Input Register (Instance 2 of 2) (VIIR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  VCF_IRQ_VIIR1_UPCIRQIN_MASK 0xffffffff
#define  VCF_IRQ_VIIR1_UPCIRQIN_SHIFT 0
#define  VCF_IRQ_VIIR1_UPCIRQIN_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIIR1_UPCIRQIN_MASK) >> VCF_IRQ_VIIR1_UPCIRQIN_SHIFT)
#define  VCF_IRQ_VIIR1_UPCIRQIN_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIIR1_UPCIRQIN_MASK) | (((uint32_t)val) << VCF_IRQ_VIIR1_UPCIRQIN_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 1 of 4) (VIMR0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_IRQ_VIMR0_RESERVED1_MASK 0xffffffc0
#define  VCF_IRQ_VIMR0_RESERVED1_SHIFT 6
#define  VCF_IRQ_VIMR0_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR0_RESERVED1_MASK) >> VCF_IRQ_VIMR0_RESERVED1_SHIFT)
#define  VCF_IRQ_VIMR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR0_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR0_RESERVED1_SHIFT))

#define  VCF_IRQ_VIMR0_UPCIRQCONFIG_MASK 0x3f
#define  VCF_IRQ_VIMR0_UPCIRQCONFIG_SHIFT 0
#define  VCF_IRQ_VIMR0_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR0_UPCIRQCONFIG_MASK) >> VCF_IRQ_VIMR0_UPCIRQCONFIG_SHIFT)
#define  VCF_IRQ_VIMR0_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR0_UPCIRQCONFIG_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR0_UPCIRQCONFIG_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 2 of 4) (VIMR1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_IRQ_VIMR1_RESERVED1_MASK 0xffffffc0
#define  VCF_IRQ_VIMR1_RESERVED1_SHIFT 6
#define  VCF_IRQ_VIMR1_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR1_RESERVED1_MASK) >> VCF_IRQ_VIMR1_RESERVED1_SHIFT)
#define  VCF_IRQ_VIMR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR1_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR1_RESERVED1_SHIFT))

#define  VCF_IRQ_VIMR1_UPCIRQCONFIG_MASK 0x3f
#define  VCF_IRQ_VIMR1_UPCIRQCONFIG_SHIFT 0
#define  VCF_IRQ_VIMR1_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR1_UPCIRQCONFIG_MASK) >> VCF_IRQ_VIMR1_UPCIRQCONFIG_SHIFT)
#define  VCF_IRQ_VIMR1_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR1_UPCIRQCONFIG_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR1_UPCIRQCONFIG_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 3 of 4) (VIMR2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_IRQ_VIMR2_RESERVED1_MASK 0xffffffc0
#define  VCF_IRQ_VIMR2_RESERVED1_SHIFT 6
#define  VCF_IRQ_VIMR2_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR2_RESERVED1_MASK) >> VCF_IRQ_VIMR2_RESERVED1_SHIFT)
#define  VCF_IRQ_VIMR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR2_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR2_RESERVED1_SHIFT))

#define  VCF_IRQ_VIMR2_UPCIRQCONFIG_MASK 0x3f
#define  VCF_IRQ_VIMR2_UPCIRQCONFIG_SHIFT 0
#define  VCF_IRQ_VIMR2_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR2_UPCIRQCONFIG_MASK) >> VCF_IRQ_VIMR2_UPCIRQCONFIG_SHIFT)
#define  VCF_IRQ_VIMR2_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR2_UPCIRQCONFIG_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR2_UPCIRQCONFIG_SHIFT))

//====================================================================
//Register: VCF IRQ Mux Register (Instance 4 of 4) (VIMR3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_IRQ_VIMR3_RESERVED1_MASK 0xffffffc0
#define  VCF_IRQ_VIMR3_RESERVED1_SHIFT 6
#define  VCF_IRQ_VIMR3_RESERVED1_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR3_RESERVED1_MASK) >> VCF_IRQ_VIMR3_RESERVED1_SHIFT)
#define  VCF_IRQ_VIMR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR3_RESERVED1_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR3_RESERVED1_SHIFT))

#define  VCF_IRQ_VIMR3_UPCIRQCONFIG_MASK 0x3f
#define  VCF_IRQ_VIMR3_UPCIRQCONFIG_SHIFT 0
#define  VCF_IRQ_VIMR3_UPCIRQCONFIG_MASK_SHIFT(reg) (((reg) & VCF_IRQ_VIMR3_UPCIRQCONFIG_MASK) >> VCF_IRQ_VIMR3_UPCIRQCONFIG_SHIFT)
#define  VCF_IRQ_VIMR3_UPCIRQCONFIG_REPLACE_VAL(reg,val) (((reg) & ~VCF_IRQ_VIMR3_UPCIRQCONFIG_MASK) | (((uint32_t)val) << VCF_IRQ_VIMR3_UPCIRQCONFIG_SHIFT))

#endif // VCF_IRQ
