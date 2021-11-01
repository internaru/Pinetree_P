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
 * \file BUS_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BUS_REGMASKS_H_
#define _BUS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BUS (BUS)
//
//====================================================================

//====================================================================
//
//Register File: ADR (ADR)
//
//====================================================================

//====================================================================
//Register: Remap (Remap)
//====================================================================

#define  ADR_REMAP_RESERVED1_MASK 0xffffff00
#define  ADR_REMAP_RESERVED1_SHIFT 8
#define  ADR_REMAP_RESERVED1_MASK_SHIFT(reg) (((reg) & ADR_REMAP_RESERVED1_MASK) >> ADR_REMAP_RESERVED1_SHIFT)
#define  ADR_REMAP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_RESERVED1_MASK) | (((uint32_t)val) << ADR_REMAP_RESERVED1_SHIFT))

#define  ADR_REMAP_NANDSD_MASK 0x80
#define  ADR_REMAP_NANDSD_SHIFT 7
#define  ADR_REMAP_NANDSD_MASK_SHIFT(reg) (((reg) & ADR_REMAP_NANDSD_MASK) >> ADR_REMAP_NANDSD_SHIFT)
#define  ADR_REMAP_NANDSD_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_NANDSD_MASK) | (((uint32_t)val) << ADR_REMAP_NANDSD_SHIFT))

#define  ADR_REMAP_LAN_MASK 0x40
#define  ADR_REMAP_LAN_SHIFT 6
#define  ADR_REMAP_LAN_MASK_SHIFT(reg) (((reg) & ADR_REMAP_LAN_MASK) >> ADR_REMAP_LAN_SHIFT)
#define  ADR_REMAP_LAN_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_LAN_MASK) | (((uint32_t)val) << ADR_REMAP_LAN_SHIFT))

#define  ADR_REMAP_LCD_MASK 0x20
#define  ADR_REMAP_LCD_SHIFT 5
#define  ADR_REMAP_LCD_MASK_SHIFT(reg) (((reg) & ADR_REMAP_LCD_MASK) >> ADR_REMAP_LCD_SHIFT)
#define  ADR_REMAP_LCD_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_LCD_MASK) | (((uint32_t)val) << ADR_REMAP_LCD_SHIFT))

#define  ADR_REMAP_MVDO_MASK 0x10
#define  ADR_REMAP_MVDO_SHIFT 4
#define  ADR_REMAP_MVDO_MASK_SHIFT(reg) (((reg) & ADR_REMAP_MVDO_MASK) >> ADR_REMAP_MVDO_SHIFT)
#define  ADR_REMAP_MVDO_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_MVDO_MASK) | (((uint32_t)val) << ADR_REMAP_MVDO_SHIFT))

#define  ADR_REMAP_PCIE_MASK 0x8
#define  ADR_REMAP_PCIE_SHIFT 3
#define  ADR_REMAP_PCIE_MASK_SHIFT(reg) (((reg) & ADR_REMAP_PCIE_MASK) >> ADR_REMAP_PCIE_SHIFT)
#define  ADR_REMAP_PCIE_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_PCIE_MASK) | (((uint32_t)val) << ADR_REMAP_PCIE_SHIFT))

#define  ADR_REMAP_UPC_MASK 0x4
#define  ADR_REMAP_UPC_SHIFT 2
#define  ADR_REMAP_UPC_MASK_SHIFT(reg) (((reg) & ADR_REMAP_UPC_MASK) >> ADR_REMAP_UPC_SHIFT)
#define  ADR_REMAP_UPC_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_UPC_MASK) | (((uint32_t)val) << ADR_REMAP_UPC_SHIFT))

#define  ADR_REMAP_IMAGING_MASK 0x2
#define  ADR_REMAP_IMAGING_SHIFT 1
#define  ADR_REMAP_IMAGING_MASK_SHIFT(reg) (((reg) & ADR_REMAP_IMAGING_MASK) >> ADR_REMAP_IMAGING_SHIFT)
#define  ADR_REMAP_IMAGING_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_IMAGING_MASK) | (((uint32_t)val) << ADR_REMAP_IMAGING_SHIFT))

#define  ADR_REMAP_LCM_MASK 0x1
#define  ADR_REMAP_LCM_SHIFT 0
#define  ADR_REMAP_LCM_MASK_SHIFT(reg) (((reg) & ADR_REMAP_LCM_MASK) >> ADR_REMAP_LCM_SHIFT)
#define  ADR_REMAP_LCM_REPLACE_VAL(reg,val) (((reg) & ~ADR_REMAP_LCM_MASK) | (((uint32_t)val) << ADR_REMAP_LCM_SHIFT))

//====================================================================
//
//Register File: MC0_M (MC0_M)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  MC0_M_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  MC0_M_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  MC0_M_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & MC0_M_FN_MOD_BM_ISS_RESERVED1_MASK) >> MC0_M_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  MC0_M_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MC0_M_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << MC0_M_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  MC0_M_FN_MOD_BM_ISS_WR_MASK 0x2
#define  MC0_M_FN_MOD_BM_ISS_WR_SHIFT 1
#define  MC0_M_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & MC0_M_FN_MOD_BM_ISS_WR_MASK) >> MC0_M_FN_MOD_BM_ISS_WR_SHIFT)
#define  MC0_M_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~MC0_M_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << MC0_M_FN_MOD_BM_ISS_WR_SHIFT))

#define  MC0_M_FN_MOD_BM_ISS_RD_MASK 0x1
#define  MC0_M_FN_MOD_BM_ISS_RD_SHIFT 0
#define  MC0_M_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & MC0_M_FN_MOD_BM_ISS_RD_MASK) >> MC0_M_FN_MOD_BM_ISS_RD_SHIFT)
#define  MC0_M_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~MC0_M_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << MC0_M_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//
//Register File: MC1_M (MC1_M)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  MC1_M_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  MC1_M_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  MC1_M_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & MC1_M_FN_MOD_BM_ISS_RESERVED1_MASK) >> MC1_M_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  MC1_M_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MC1_M_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << MC1_M_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  MC1_M_FN_MOD_BM_ISS_WR_MASK 0x2
#define  MC1_M_FN_MOD_BM_ISS_WR_SHIFT 1
#define  MC1_M_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & MC1_M_FN_MOD_BM_ISS_WR_MASK) >> MC1_M_FN_MOD_BM_ISS_WR_SHIFT)
#define  MC1_M_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~MC1_M_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << MC1_M_FN_MOD_BM_ISS_WR_SHIFT))

#define  MC1_M_FN_MOD_BM_ISS_RD_MASK 0x1
#define  MC1_M_FN_MOD_BM_ISS_RD_SHIFT 0
#define  MC1_M_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & MC1_M_FN_MOD_BM_ISS_RD_MASK) >> MC1_M_FN_MOD_BM_ISS_RD_SHIFT)
#define  MC1_M_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~MC1_M_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << MC1_M_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  MC1_M_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  MC1_M_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  MC1_M_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & MC1_M_FN_MOD_ISS_RESERVED1_MASK) >> MC1_M_FN_MOD_ISS_RESERVED1_SHIFT)
#define  MC1_M_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MC1_M_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << MC1_M_FN_MOD_ISS_RESERVED1_SHIFT))

#define  MC1_M_FN_MOD_ISS_WR_MASK 0x2
#define  MC1_M_FN_MOD_ISS_WR_SHIFT 1
#define  MC1_M_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & MC1_M_FN_MOD_ISS_WR_MASK) >> MC1_M_FN_MOD_ISS_WR_SHIFT)
#define  MC1_M_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~MC1_M_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << MC1_M_FN_MOD_ISS_WR_SHIFT))

#define  MC1_M_FN_MOD_ISS_RD_MASK 0x1
#define  MC1_M_FN_MOD_ISS_RD_SHIFT 0
#define  MC1_M_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & MC1_M_FN_MOD_ISS_RD_MASK) >> MC1_M_FN_MOD_ISS_RD_SHIFT)
#define  MC1_M_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~MC1_M_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << MC1_M_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: LCM_M (LCM_M)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  LCM_M_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  LCM_M_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  LCM_M_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & LCM_M_FN_MOD_BM_ISS_RESERVED1_MASK) >> LCM_M_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  LCM_M_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LCM_M_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << LCM_M_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  LCM_M_FN_MOD_BM_ISS_WR_MASK 0x2
#define  LCM_M_FN_MOD_BM_ISS_WR_SHIFT 1
#define  LCM_M_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & LCM_M_FN_MOD_BM_ISS_WR_MASK) >> LCM_M_FN_MOD_BM_ISS_WR_SHIFT)
#define  LCM_M_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~LCM_M_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << LCM_M_FN_MOD_BM_ISS_WR_SHIFT))

#define  LCM_M_FN_MOD_BM_ISS_RD_MASK 0x1
#define  LCM_M_FN_MOD_BM_ISS_RD_SHIFT 0
#define  LCM_M_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & LCM_M_FN_MOD_BM_ISS_RD_MASK) >> LCM_M_FN_MOD_BM_ISS_RD_SHIFT)
#define  LCM_M_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~LCM_M_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << LCM_M_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//
//Register File: PCIE_M (PCIE_M)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  PCIE_M_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  PCIE_M_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  PCIE_M_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_M_FN_MOD_BM_ISS_RESERVED1_MASK) >> PCIE_M_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  PCIE_M_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_M_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << PCIE_M_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  PCIE_M_FN_MOD_BM_ISS_WR_MASK 0x2
#define  PCIE_M_FN_MOD_BM_ISS_WR_SHIFT 1
#define  PCIE_M_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & PCIE_M_FN_MOD_BM_ISS_WR_MASK) >> PCIE_M_FN_MOD_BM_ISS_WR_SHIFT)
#define  PCIE_M_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_M_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << PCIE_M_FN_MOD_BM_ISS_WR_SHIFT))

#define  PCIE_M_FN_MOD_BM_ISS_RD_MASK 0x1
#define  PCIE_M_FN_MOD_BM_ISS_RD_SHIFT 0
#define  PCIE_M_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & PCIE_M_FN_MOD_BM_ISS_RD_MASK) >> PCIE_M_FN_MOD_BM_ISS_RD_SHIFT)
#define  PCIE_M_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~PCIE_M_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << PCIE_M_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//
//Register File: NAND_M (NAND_M)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  NAND_M_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  NAND_M_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  NAND_M_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & NAND_M_FN_MOD_BM_ISS_RESERVED1_MASK) >> NAND_M_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  NAND_M_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~NAND_M_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << NAND_M_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  NAND_M_FN_MOD_BM_ISS_WR_MASK 0x2
#define  NAND_M_FN_MOD_BM_ISS_WR_SHIFT 1
#define  NAND_M_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & NAND_M_FN_MOD_BM_ISS_WR_MASK) >> NAND_M_FN_MOD_BM_ISS_WR_SHIFT)
#define  NAND_M_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~NAND_M_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << NAND_M_FN_MOD_BM_ISS_WR_SHIFT))

#define  NAND_M_FN_MOD_BM_ISS_RD_MASK 0x1
#define  NAND_M_FN_MOD_BM_ISS_RD_SHIFT 0
#define  NAND_M_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & NAND_M_FN_MOD_BM_ISS_RD_MASK) >> NAND_M_FN_MOD_BM_ISS_RD_SHIFT)
#define  NAND_M_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~NAND_M_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << NAND_M_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  NAND_M_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  NAND_M_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  NAND_M_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & NAND_M_FN_MOD_ISS_RESERVED1_MASK) >> NAND_M_FN_MOD_ISS_RESERVED1_SHIFT)
#define  NAND_M_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~NAND_M_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << NAND_M_FN_MOD_ISS_RESERVED1_SHIFT))

#define  NAND_M_FN_MOD_ISS_WR_MASK 0x2
#define  NAND_M_FN_MOD_ISS_WR_SHIFT 1
#define  NAND_M_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & NAND_M_FN_MOD_ISS_WR_MASK) >> NAND_M_FN_MOD_ISS_WR_SHIFT)
#define  NAND_M_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~NAND_M_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << NAND_M_FN_MOD_ISS_WR_SHIFT))

#define  NAND_M_FN_MOD_ISS_RD_MASK 0x1
#define  NAND_M_FN_MOD_ISS_RD_SHIFT 0
#define  NAND_M_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & NAND_M_FN_MOD_ISS_RD_MASK) >> NAND_M_FN_MOD_ISS_RD_SHIFT)
#define  NAND_M_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~NAND_M_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << NAND_M_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: BOOTSPI_M (BOOTSPI_M)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_MASK) >> BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << BOOTSPI_M_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  BOOTSPI_M_FN_MOD_BM_ISS_WR_MASK 0x2
#define  BOOTSPI_M_FN_MOD_BM_ISS_WR_SHIFT 1
#define  BOOTSPI_M_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & BOOTSPI_M_FN_MOD_BM_ISS_WR_MASK) >> BOOTSPI_M_FN_MOD_BM_ISS_WR_SHIFT)
#define  BOOTSPI_M_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~BOOTSPI_M_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << BOOTSPI_M_FN_MOD_BM_ISS_WR_SHIFT))

#define  BOOTSPI_M_FN_MOD_BM_ISS_RD_MASK 0x1
#define  BOOTSPI_M_FN_MOD_BM_ISS_RD_SHIFT 0
#define  BOOTSPI_M_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & BOOTSPI_M_FN_MOD_BM_ISS_RD_MASK) >> BOOTSPI_M_FN_MOD_BM_ISS_RD_SHIFT)
#define  BOOTSPI_M_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~BOOTSPI_M_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << BOOTSPI_M_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  BOOTSPI_M_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  BOOTSPI_M_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  BOOTSPI_M_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & BOOTSPI_M_FN_MOD_ISS_RESERVED1_MASK) >> BOOTSPI_M_FN_MOD_ISS_RESERVED1_SHIFT)
#define  BOOTSPI_M_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BOOTSPI_M_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << BOOTSPI_M_FN_MOD_ISS_RESERVED1_SHIFT))

#define  BOOTSPI_M_FN_MOD_ISS_WR_MASK 0x2
#define  BOOTSPI_M_FN_MOD_ISS_WR_SHIFT 1
#define  BOOTSPI_M_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & BOOTSPI_M_FN_MOD_ISS_WR_MASK) >> BOOTSPI_M_FN_MOD_ISS_WR_SHIFT)
#define  BOOTSPI_M_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~BOOTSPI_M_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << BOOTSPI_M_FN_MOD_ISS_WR_SHIFT))

#define  BOOTSPI_M_FN_MOD_ISS_RD_MASK 0x1
#define  BOOTSPI_M_FN_MOD_ISS_RD_SHIFT 0
#define  BOOTSPI_M_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & BOOTSPI_M_FN_MOD_ISS_RD_MASK) >> BOOTSPI_M_FN_MOD_ISS_RD_SHIFT)
#define  BOOTSPI_M_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~BOOTSPI_M_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << BOOTSPI_M_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: REG_M (REG_M)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  REG_M_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  REG_M_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  REG_M_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & REG_M_FN_MOD_BM_ISS_RESERVED1_MASK) >> REG_M_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  REG_M_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~REG_M_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << REG_M_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  REG_M_FN_MOD_BM_ISS_WR_MASK 0x2
#define  REG_M_FN_MOD_BM_ISS_WR_SHIFT 1
#define  REG_M_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & REG_M_FN_MOD_BM_ISS_WR_MASK) >> REG_M_FN_MOD_BM_ISS_WR_SHIFT)
#define  REG_M_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~REG_M_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << REG_M_FN_MOD_BM_ISS_WR_SHIFT))

#define  REG_M_FN_MOD_BM_ISS_RD_MASK 0x1
#define  REG_M_FN_MOD_BM_ISS_RD_SHIFT 0
#define  REG_M_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & REG_M_FN_MOD_BM_ISS_RD_MASK) >> REG_M_FN_MOD_BM_ISS_RD_SHIFT)
#define  REG_M_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~REG_M_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << REG_M_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  REG_M_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  REG_M_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  REG_M_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & REG_M_FN_MOD_ISS_RESERVED1_MASK) >> REG_M_FN_MOD_ISS_RESERVED1_SHIFT)
#define  REG_M_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~REG_M_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << REG_M_FN_MOD_ISS_RESERVED1_SHIFT))

#define  REG_M_FN_MOD_ISS_WR_MASK 0x2
#define  REG_M_FN_MOD_ISS_WR_SHIFT 1
#define  REG_M_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & REG_M_FN_MOD_ISS_WR_MASK) >> REG_M_FN_MOD_ISS_WR_SHIFT)
#define  REG_M_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~REG_M_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << REG_M_FN_MOD_ISS_WR_SHIFT))

#define  REG_M_FN_MOD_ISS_RD_MASK 0x1
#define  REG_M_FN_MOD_ISS_RD_SHIFT 0
#define  REG_M_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & REG_M_FN_MOD_ISS_RD_MASK) >> REG_M_FN_MOD_ISS_RD_SHIFT)
#define  REG_M_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~REG_M_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << REG_M_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: CPU_S (CPU_S)
//
//====================================================================

//====================================================================
//Register: sync_mode (sync_mode)
//====================================================================

#define  CPU_S_SYNC_MODE_RESERVED1_MASK 0xfffffff8
#define  CPU_S_SYNC_MODE_RESERVED1_SHIFT 3
#define  CPU_S_SYNC_MODE_RESERVED1_MASK_SHIFT(reg) (((reg) & CPU_S_SYNC_MODE_RESERVED1_MASK) >> CPU_S_SYNC_MODE_RESERVED1_SHIFT)
#define  CPU_S_SYNC_MODE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_SYNC_MODE_RESERVED1_MASK) | (((uint32_t)val) << CPU_S_SYNC_MODE_RESERVED1_SHIFT))

#define  CPU_S_SYNC_MODE_VALUE_MASK 0x7
#define  CPU_S_SYNC_MODE_VALUE_SHIFT 0
#define  CPU_S_SYNC_MODE_VALUE_MASK_SHIFT(reg) (((reg) & CPU_S_SYNC_MODE_VALUE_MASK) >> CPU_S_SYNC_MODE_VALUE_SHIFT)
#define  CPU_S_SYNC_MODE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_SYNC_MODE_VALUE_MASK) | (((uint32_t)val) << CPU_S_SYNC_MODE_VALUE_SHIFT))

//====================================================================
//Register: wr_tidemark (wr_tidemark)
//====================================================================

#define  CPU_S_WR_TIDEMARK_RESERVED1_MASK 0xfffffff0
#define  CPU_S_WR_TIDEMARK_RESERVED1_SHIFT 4
#define  CPU_S_WR_TIDEMARK_RESERVED1_MASK_SHIFT(reg) (((reg) & CPU_S_WR_TIDEMARK_RESERVED1_MASK) >> CPU_S_WR_TIDEMARK_RESERVED1_SHIFT)
#define  CPU_S_WR_TIDEMARK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_WR_TIDEMARK_RESERVED1_MASK) | (((uint32_t)val) << CPU_S_WR_TIDEMARK_RESERVED1_SHIFT))

#define  CPU_S_WR_TIDEMARK_VALUE_MASK 0xf
#define  CPU_S_WR_TIDEMARK_VALUE_SHIFT 0
#define  CPU_S_WR_TIDEMARK_VALUE_MASK_SHIFT(reg) (((reg) & CPU_S_WR_TIDEMARK_VALUE_MASK) >> CPU_S_WR_TIDEMARK_VALUE_SHIFT)
#define  CPU_S_WR_TIDEMARK_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_WR_TIDEMARK_VALUE_MASK) | (((uint32_t)val) << CPU_S_WR_TIDEMARK_VALUE_SHIFT))

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  CPU_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  CPU_S_READ_QOS_RESERVED1_SHIFT 4
#define  CPU_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CPU_S_READ_QOS_RESERVED1_MASK) >> CPU_S_READ_QOS_RESERVED1_SHIFT)
#define  CPU_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << CPU_S_READ_QOS_RESERVED1_SHIFT))

#define  CPU_S_READ_QOS_VALUE_MASK 0xf
#define  CPU_S_READ_QOS_VALUE_SHIFT 0
#define  CPU_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CPU_S_READ_QOS_VALUE_MASK) >> CPU_S_READ_QOS_VALUE_SHIFT)
#define  CPU_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << CPU_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  CPU_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  CPU_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  CPU_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CPU_S_WRITE_QOS_RESERVED1_MASK) >> CPU_S_WRITE_QOS_RESERVED1_SHIFT)
#define  CPU_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << CPU_S_WRITE_QOS_RESERVED1_SHIFT))

#define  CPU_S_WRITE_QOS_VALUE_MASK 0xf
#define  CPU_S_WRITE_QOS_VALUE_SHIFT 0
#define  CPU_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CPU_S_WRITE_QOS_VALUE_MASK) >> CPU_S_WRITE_QOS_VALUE_SHIFT)
#define  CPU_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << CPU_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  CPU_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  CPU_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  CPU_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & CPU_S_FN_MOD_ISS_RESERVED1_MASK) >> CPU_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  CPU_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << CPU_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  CPU_S_FN_MOD_ISS_WR_MASK 0x2
#define  CPU_S_FN_MOD_ISS_WR_SHIFT 1
#define  CPU_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & CPU_S_FN_MOD_ISS_WR_MASK) >> CPU_S_FN_MOD_ISS_WR_SHIFT)
#define  CPU_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << CPU_S_FN_MOD_ISS_WR_SHIFT))

#define  CPU_S_FN_MOD_ISS_RD_MASK 0x1
#define  CPU_S_FN_MOD_ISS_RD_SHIFT 0
#define  CPU_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & CPU_S_FN_MOD_ISS_RD_MASK) >> CPU_S_FN_MOD_ISS_RD_SHIFT)
#define  CPU_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~CPU_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << CPU_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: PCIE_S (PCIE_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  PCIE_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  PCIE_S_READ_QOS_RESERVED1_SHIFT 4
#define  PCIE_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_S_READ_QOS_RESERVED1_MASK) >> PCIE_S_READ_QOS_RESERVED1_SHIFT)
#define  PCIE_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << PCIE_S_READ_QOS_RESERVED1_SHIFT))

#define  PCIE_S_READ_QOS_VALUE_MASK 0xf
#define  PCIE_S_READ_QOS_VALUE_SHIFT 0
#define  PCIE_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PCIE_S_READ_QOS_VALUE_MASK) >> PCIE_S_READ_QOS_VALUE_SHIFT)
#define  PCIE_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << PCIE_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  PCIE_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  PCIE_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  PCIE_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_S_WRITE_QOS_RESERVED1_MASK) >> PCIE_S_WRITE_QOS_RESERVED1_SHIFT)
#define  PCIE_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << PCIE_S_WRITE_QOS_RESERVED1_SHIFT))

#define  PCIE_S_WRITE_QOS_VALUE_MASK 0xf
#define  PCIE_S_WRITE_QOS_VALUE_SHIFT 0
#define  PCIE_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PCIE_S_WRITE_QOS_VALUE_MASK) >> PCIE_S_WRITE_QOS_VALUE_SHIFT)
#define  PCIE_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << PCIE_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  PCIE_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  PCIE_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  PCIE_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_S_FN_MOD_ISS_RESERVED1_MASK) >> PCIE_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  PCIE_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << PCIE_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  PCIE_S_FN_MOD_ISS_WR_MASK 0x2
#define  PCIE_S_FN_MOD_ISS_WR_SHIFT 1
#define  PCIE_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & PCIE_S_FN_MOD_ISS_WR_MASK) >> PCIE_S_FN_MOD_ISS_WR_SHIFT)
#define  PCIE_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << PCIE_S_FN_MOD_ISS_WR_SHIFT))

#define  PCIE_S_FN_MOD_ISS_RD_MASK 0x1
#define  PCIE_S_FN_MOD_ISS_RD_SHIFT 0
#define  PCIE_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & PCIE_S_FN_MOD_ISS_RD_MASK) >> PCIE_S_FN_MOD_ISS_RD_SHIFT)
#define  PCIE_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~PCIE_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << PCIE_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: M2M_S (M2M_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  M2M_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  M2M_S_READ_QOS_RESERVED1_SHIFT 4
#define  M2M_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M_S_READ_QOS_RESERVED1_MASK) >> M2M_S_READ_QOS_RESERVED1_SHIFT)
#define  M2M_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << M2M_S_READ_QOS_RESERVED1_SHIFT))

#define  M2M_S_READ_QOS_VALUE_MASK 0xf
#define  M2M_S_READ_QOS_VALUE_SHIFT 0
#define  M2M_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & M2M_S_READ_QOS_VALUE_MASK) >> M2M_S_READ_QOS_VALUE_SHIFT)
#define  M2M_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~M2M_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << M2M_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  M2M_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  M2M_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  M2M_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M_S_WRITE_QOS_RESERVED1_MASK) >> M2M_S_WRITE_QOS_RESERVED1_SHIFT)
#define  M2M_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << M2M_S_WRITE_QOS_RESERVED1_SHIFT))

#define  M2M_S_WRITE_QOS_VALUE_MASK 0xf
#define  M2M_S_WRITE_QOS_VALUE_SHIFT 0
#define  M2M_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & M2M_S_WRITE_QOS_VALUE_MASK) >> M2M_S_WRITE_QOS_VALUE_SHIFT)
#define  M2M_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~M2M_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << M2M_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  M2M_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  M2M_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  M2M_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M_S_FN_MOD_ISS_RESERVED1_MASK) >> M2M_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  M2M_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << M2M_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  M2M_S_FN_MOD_ISS_WR_MASK 0x2
#define  M2M_S_FN_MOD_ISS_WR_SHIFT 1
#define  M2M_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & M2M_S_FN_MOD_ISS_WR_MASK) >> M2M_S_FN_MOD_ISS_WR_SHIFT)
#define  M2M_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~M2M_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << M2M_S_FN_MOD_ISS_WR_SHIFT))

#define  M2M_S_FN_MOD_ISS_RD_MASK 0x1
#define  M2M_S_FN_MOD_ISS_RD_SHIFT 0
#define  M2M_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & M2M_S_FN_MOD_ISS_RD_MASK) >> M2M_S_FN_MOD_ISS_RD_SHIFT)
#define  M2M_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~M2M_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << M2M_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: GLAN_S (GLAN_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  GLAN_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  GLAN_S_READ_QOS_RESERVED1_SHIFT 4
#define  GLAN_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & GLAN_S_READ_QOS_RESERVED1_MASK) >> GLAN_S_READ_QOS_RESERVED1_SHIFT)
#define  GLAN_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GLAN_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << GLAN_S_READ_QOS_RESERVED1_SHIFT))

#define  GLAN_S_READ_QOS_VALUE_MASK 0xf
#define  GLAN_S_READ_QOS_VALUE_SHIFT 0
#define  GLAN_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & GLAN_S_READ_QOS_VALUE_MASK) >> GLAN_S_READ_QOS_VALUE_SHIFT)
#define  GLAN_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~GLAN_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << GLAN_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  GLAN_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  GLAN_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  GLAN_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & GLAN_S_WRITE_QOS_RESERVED1_MASK) >> GLAN_S_WRITE_QOS_RESERVED1_SHIFT)
#define  GLAN_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GLAN_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << GLAN_S_WRITE_QOS_RESERVED1_SHIFT))

#define  GLAN_S_WRITE_QOS_VALUE_MASK 0xf
#define  GLAN_S_WRITE_QOS_VALUE_SHIFT 0
#define  GLAN_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & GLAN_S_WRITE_QOS_VALUE_MASK) >> GLAN_S_WRITE_QOS_VALUE_SHIFT)
#define  GLAN_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~GLAN_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << GLAN_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  GLAN_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  GLAN_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  GLAN_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & GLAN_S_FN_MOD_ISS_RESERVED1_MASK) >> GLAN_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  GLAN_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GLAN_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << GLAN_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  GLAN_S_FN_MOD_ISS_WR_MASK 0x2
#define  GLAN_S_FN_MOD_ISS_WR_SHIFT 1
#define  GLAN_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & GLAN_S_FN_MOD_ISS_WR_MASK) >> GLAN_S_FN_MOD_ISS_WR_SHIFT)
#define  GLAN_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~GLAN_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << GLAN_S_FN_MOD_ISS_WR_SHIFT))

#define  GLAN_S_FN_MOD_ISS_RD_MASK 0x1
#define  GLAN_S_FN_MOD_ISS_RD_SHIFT 0
#define  GLAN_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & GLAN_S_FN_MOD_ISS_RD_MASK) >> GLAN_S_FN_MOD_ISS_RD_SHIFT)
#define  GLAN_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~GLAN_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << GLAN_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: LCD_S (LCD_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  LCD_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  LCD_S_READ_QOS_RESERVED1_SHIFT 4
#define  LCD_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & LCD_S_READ_QOS_RESERVED1_MASK) >> LCD_S_READ_QOS_RESERVED1_SHIFT)
#define  LCD_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LCD_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << LCD_S_READ_QOS_RESERVED1_SHIFT))

#define  LCD_S_READ_QOS_VALUE_MASK 0xf
#define  LCD_S_READ_QOS_VALUE_SHIFT 0
#define  LCD_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & LCD_S_READ_QOS_VALUE_MASK) >> LCD_S_READ_QOS_VALUE_SHIFT)
#define  LCD_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~LCD_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << LCD_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  LCD_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  LCD_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  LCD_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & LCD_S_WRITE_QOS_RESERVED1_MASK) >> LCD_S_WRITE_QOS_RESERVED1_SHIFT)
#define  LCD_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LCD_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << LCD_S_WRITE_QOS_RESERVED1_SHIFT))

#define  LCD_S_WRITE_QOS_VALUE_MASK 0xf
#define  LCD_S_WRITE_QOS_VALUE_SHIFT 0
#define  LCD_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & LCD_S_WRITE_QOS_VALUE_MASK) >> LCD_S_WRITE_QOS_VALUE_SHIFT)
#define  LCD_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~LCD_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << LCD_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  LCD_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  LCD_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  LCD_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & LCD_S_FN_MOD_ISS_RESERVED1_MASK) >> LCD_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  LCD_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LCD_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << LCD_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  LCD_S_FN_MOD_ISS_WR_MASK 0x2
#define  LCD_S_FN_MOD_ISS_WR_SHIFT 1
#define  LCD_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & LCD_S_FN_MOD_ISS_WR_MASK) >> LCD_S_FN_MOD_ISS_WR_SHIFT)
#define  LCD_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~LCD_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << LCD_S_FN_MOD_ISS_WR_SHIFT))

#define  LCD_S_FN_MOD_ISS_RD_MASK 0x1
#define  LCD_S_FN_MOD_ISS_RD_SHIFT 0
#define  LCD_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & LCD_S_FN_MOD_ISS_RD_MASK) >> LCD_S_FN_MOD_ISS_RD_SHIFT)
#define  LCD_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~LCD_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << LCD_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: ASELA_S (ASELA_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  ASELA_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  ASELA_S_READ_QOS_RESERVED1_SHIFT 4
#define  ASELA_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & ASELA_S_READ_QOS_RESERVED1_MASK) >> ASELA_S_READ_QOS_RESERVED1_SHIFT)
#define  ASELA_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ASELA_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << ASELA_S_READ_QOS_RESERVED1_SHIFT))

#define  ASELA_S_READ_QOS_VALUE_MASK 0xf
#define  ASELA_S_READ_QOS_VALUE_SHIFT 0
#define  ASELA_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & ASELA_S_READ_QOS_VALUE_MASK) >> ASELA_S_READ_QOS_VALUE_SHIFT)
#define  ASELA_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~ASELA_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << ASELA_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  ASELA_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  ASELA_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  ASELA_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & ASELA_S_WRITE_QOS_RESERVED1_MASK) >> ASELA_S_WRITE_QOS_RESERVED1_SHIFT)
#define  ASELA_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ASELA_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << ASELA_S_WRITE_QOS_RESERVED1_SHIFT))

#define  ASELA_S_WRITE_QOS_VALUE_MASK 0xf
#define  ASELA_S_WRITE_QOS_VALUE_SHIFT 0
#define  ASELA_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & ASELA_S_WRITE_QOS_VALUE_MASK) >> ASELA_S_WRITE_QOS_VALUE_SHIFT)
#define  ASELA_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~ASELA_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << ASELA_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  ASELA_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  ASELA_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  ASELA_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & ASELA_S_FN_MOD_ISS_RESERVED1_MASK) >> ASELA_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  ASELA_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ASELA_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << ASELA_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  ASELA_S_FN_MOD_ISS_WR_MASK 0x2
#define  ASELA_S_FN_MOD_ISS_WR_SHIFT 1
#define  ASELA_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & ASELA_S_FN_MOD_ISS_WR_MASK) >> ASELA_S_FN_MOD_ISS_WR_SHIFT)
#define  ASELA_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~ASELA_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << ASELA_S_FN_MOD_ISS_WR_SHIFT))

#define  ASELA_S_FN_MOD_ISS_RD_MASK 0x1
#define  ASELA_S_FN_MOD_ISS_RD_SHIFT 0
#define  ASELA_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & ASELA_S_FN_MOD_ISS_RD_MASK) >> ASELA_S_FN_MOD_ISS_RD_SHIFT)
#define  ASELA_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~ASELA_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << ASELA_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: PIP_S (PIP_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  PIP_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  PIP_S_READ_QOS_RESERVED1_SHIFT 4
#define  PIP_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_S_READ_QOS_RESERVED1_MASK) >> PIP_S_READ_QOS_RESERVED1_SHIFT)
#define  PIP_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << PIP_S_READ_QOS_RESERVED1_SHIFT))

#define  PIP_S_READ_QOS_VALUE_MASK 0xf
#define  PIP_S_READ_QOS_VALUE_SHIFT 0
#define  PIP_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PIP_S_READ_QOS_VALUE_MASK) >> PIP_S_READ_QOS_VALUE_SHIFT)
#define  PIP_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PIP_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << PIP_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  PIP_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  PIP_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  PIP_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_S_WRITE_QOS_RESERVED1_MASK) >> PIP_S_WRITE_QOS_RESERVED1_SHIFT)
#define  PIP_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << PIP_S_WRITE_QOS_RESERVED1_SHIFT))

#define  PIP_S_WRITE_QOS_VALUE_MASK 0xf
#define  PIP_S_WRITE_QOS_VALUE_SHIFT 0
#define  PIP_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PIP_S_WRITE_QOS_VALUE_MASK) >> PIP_S_WRITE_QOS_VALUE_SHIFT)
#define  PIP_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PIP_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << PIP_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  PIP_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  PIP_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  PIP_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIP_S_FN_MOD_ISS_RESERVED1_MASK) >> PIP_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  PIP_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIP_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << PIP_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  PIP_S_FN_MOD_ISS_WR_MASK 0x2
#define  PIP_S_FN_MOD_ISS_WR_SHIFT 1
#define  PIP_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & PIP_S_FN_MOD_ISS_WR_MASK) >> PIP_S_FN_MOD_ISS_WR_SHIFT)
#define  PIP_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~PIP_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << PIP_S_FN_MOD_ISS_WR_SHIFT))

#define  PIP_S_FN_MOD_ISS_RD_MASK 0x1
#define  PIP_S_FN_MOD_ISS_RD_SHIFT 0
#define  PIP_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & PIP_S_FN_MOD_ISS_RD_MASK) >> PIP_S_FN_MOD_ISS_RD_SHIFT)
#define  PIP_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~PIP_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << PIP_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: PIE_S (PIE_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  PIE_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  PIE_S_READ_QOS_RESERVED1_SHIFT 4
#define  PIE_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_S_READ_QOS_RESERVED1_MASK) >> PIE_S_READ_QOS_RESERVED1_SHIFT)
#define  PIE_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << PIE_S_READ_QOS_RESERVED1_SHIFT))

#define  PIE_S_READ_QOS_VALUE_MASK 0xf
#define  PIE_S_READ_QOS_VALUE_SHIFT 0
#define  PIE_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PIE_S_READ_QOS_VALUE_MASK) >> PIE_S_READ_QOS_VALUE_SHIFT)
#define  PIE_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PIE_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << PIE_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  PIE_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  PIE_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  PIE_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_S_WRITE_QOS_RESERVED1_MASK) >> PIE_S_WRITE_QOS_RESERVED1_SHIFT)
#define  PIE_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << PIE_S_WRITE_QOS_RESERVED1_SHIFT))

#define  PIE_S_WRITE_QOS_VALUE_MASK 0xf
#define  PIE_S_WRITE_QOS_VALUE_SHIFT 0
#define  PIE_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PIE_S_WRITE_QOS_VALUE_MASK) >> PIE_S_WRITE_QOS_VALUE_SHIFT)
#define  PIE_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PIE_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << PIE_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  PIE_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  PIE_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  PIE_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_S_FN_MOD_ISS_RESERVED1_MASK) >> PIE_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  PIE_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << PIE_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  PIE_S_FN_MOD_ISS_WR_MASK 0x2
#define  PIE_S_FN_MOD_ISS_WR_SHIFT 1
#define  PIE_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & PIE_S_FN_MOD_ISS_WR_MASK) >> PIE_S_FN_MOD_ISS_WR_SHIFT)
#define  PIE_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~PIE_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << PIE_S_FN_MOD_ISS_WR_SHIFT))

#define  PIE_S_FN_MOD_ISS_RD_MASK 0x1
#define  PIE_S_FN_MOD_ISS_RD_SHIFT 0
#define  PIE_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & PIE_S_FN_MOD_ISS_RD_MASK) >> PIE_S_FN_MOD_ISS_RD_SHIFT)
#define  PIE_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~PIE_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << PIE_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: ANTIC_S (ANTIC_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  ANTIC_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  ANTIC_S_READ_QOS_RESERVED1_SHIFT 4
#define  ANTIC_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & ANTIC_S_READ_QOS_RESERVED1_MASK) >> ANTIC_S_READ_QOS_RESERVED1_SHIFT)
#define  ANTIC_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ANTIC_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << ANTIC_S_READ_QOS_RESERVED1_SHIFT))

#define  ANTIC_S_READ_QOS_VALUE_MASK 0xf
#define  ANTIC_S_READ_QOS_VALUE_SHIFT 0
#define  ANTIC_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & ANTIC_S_READ_QOS_VALUE_MASK) >> ANTIC_S_READ_QOS_VALUE_SHIFT)
#define  ANTIC_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~ANTIC_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << ANTIC_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  ANTIC_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  ANTIC_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  ANTIC_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & ANTIC_S_WRITE_QOS_RESERVED1_MASK) >> ANTIC_S_WRITE_QOS_RESERVED1_SHIFT)
#define  ANTIC_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ANTIC_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << ANTIC_S_WRITE_QOS_RESERVED1_SHIFT))

#define  ANTIC_S_WRITE_QOS_VALUE_MASK 0xf
#define  ANTIC_S_WRITE_QOS_VALUE_SHIFT 0
#define  ANTIC_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & ANTIC_S_WRITE_QOS_VALUE_MASK) >> ANTIC_S_WRITE_QOS_VALUE_SHIFT)
#define  ANTIC_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~ANTIC_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << ANTIC_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  ANTIC_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  ANTIC_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  ANTIC_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & ANTIC_S_FN_MOD_ISS_RESERVED1_MASK) >> ANTIC_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  ANTIC_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ANTIC_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << ANTIC_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  ANTIC_S_FN_MOD_ISS_WR_MASK 0x2
#define  ANTIC_S_FN_MOD_ISS_WR_SHIFT 1
#define  ANTIC_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & ANTIC_S_FN_MOD_ISS_WR_MASK) >> ANTIC_S_FN_MOD_ISS_WR_SHIFT)
#define  ANTIC_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~ANTIC_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << ANTIC_S_FN_MOD_ISS_WR_SHIFT))

#define  ANTIC_S_FN_MOD_ISS_RD_MASK 0x1
#define  ANTIC_S_FN_MOD_ISS_RD_SHIFT 0
#define  ANTIC_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & ANTIC_S_FN_MOD_ISS_RD_MASK) >> ANTIC_S_FN_MOD_ISS_RD_SHIFT)
#define  ANTIC_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~ANTIC_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << ANTIC_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: CSAHB_S (CSAHB_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  CSAHB_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  CSAHB_S_READ_QOS_RESERVED1_SHIFT 4
#define  CSAHB_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CSAHB_S_READ_QOS_RESERVED1_MASK) >> CSAHB_S_READ_QOS_RESERVED1_SHIFT)
#define  CSAHB_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSAHB_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << CSAHB_S_READ_QOS_RESERVED1_SHIFT))

#define  CSAHB_S_READ_QOS_VALUE_MASK 0xf
#define  CSAHB_S_READ_QOS_VALUE_SHIFT 0
#define  CSAHB_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CSAHB_S_READ_QOS_VALUE_MASK) >> CSAHB_S_READ_QOS_VALUE_SHIFT)
#define  CSAHB_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CSAHB_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << CSAHB_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  CSAHB_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  CSAHB_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  CSAHB_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CSAHB_S_WRITE_QOS_RESERVED1_MASK) >> CSAHB_S_WRITE_QOS_RESERVED1_SHIFT)
#define  CSAHB_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSAHB_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << CSAHB_S_WRITE_QOS_RESERVED1_SHIFT))

#define  CSAHB_S_WRITE_QOS_VALUE_MASK 0xf
#define  CSAHB_S_WRITE_QOS_VALUE_SHIFT 0
#define  CSAHB_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CSAHB_S_WRITE_QOS_VALUE_MASK) >> CSAHB_S_WRITE_QOS_VALUE_SHIFT)
#define  CSAHB_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CSAHB_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << CSAHB_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  CSAHB_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  CSAHB_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  CSAHB_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & CSAHB_S_FN_MOD_ISS_RESERVED1_MASK) >> CSAHB_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  CSAHB_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSAHB_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << CSAHB_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  CSAHB_S_FN_MOD_ISS_WR_MASK 0x2
#define  CSAHB_S_FN_MOD_ISS_WR_SHIFT 1
#define  CSAHB_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & CSAHB_S_FN_MOD_ISS_WR_MASK) >> CSAHB_S_FN_MOD_ISS_WR_SHIFT)
#define  CSAHB_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~CSAHB_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << CSAHB_S_FN_MOD_ISS_WR_SHIFT))

#define  CSAHB_S_FN_MOD_ISS_RD_MASK 0x1
#define  CSAHB_S_FN_MOD_ISS_RD_SHIFT 0
#define  CSAHB_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & CSAHB_S_FN_MOD_ISS_RD_MASK) >> CSAHB_S_FN_MOD_ISS_RD_SHIFT)
#define  CSAHB_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~CSAHB_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << CSAHB_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: TIC_S (TIC_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  TIC_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  TIC_S_READ_QOS_RESERVED1_SHIFT 4
#define  TIC_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & TIC_S_READ_QOS_RESERVED1_MASK) >> TIC_S_READ_QOS_RESERVED1_SHIFT)
#define  TIC_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIC_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << TIC_S_READ_QOS_RESERVED1_SHIFT))

#define  TIC_S_READ_QOS_VALUE_MASK 0xf
#define  TIC_S_READ_QOS_VALUE_SHIFT 0
#define  TIC_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & TIC_S_READ_QOS_VALUE_MASK) >> TIC_S_READ_QOS_VALUE_SHIFT)
#define  TIC_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~TIC_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << TIC_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  TIC_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  TIC_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  TIC_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & TIC_S_WRITE_QOS_RESERVED1_MASK) >> TIC_S_WRITE_QOS_RESERVED1_SHIFT)
#define  TIC_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIC_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << TIC_S_WRITE_QOS_RESERVED1_SHIFT))

#define  TIC_S_WRITE_QOS_VALUE_MASK 0xf
#define  TIC_S_WRITE_QOS_VALUE_SHIFT 0
#define  TIC_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & TIC_S_WRITE_QOS_VALUE_MASK) >> TIC_S_WRITE_QOS_VALUE_SHIFT)
#define  TIC_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~TIC_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << TIC_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  TIC_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  TIC_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  TIC_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & TIC_S_FN_MOD_ISS_RESERVED1_MASK) >> TIC_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  TIC_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TIC_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << TIC_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  TIC_S_FN_MOD_ISS_WR_MASK 0x2
#define  TIC_S_FN_MOD_ISS_WR_SHIFT 1
#define  TIC_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & TIC_S_FN_MOD_ISS_WR_MASK) >> TIC_S_FN_MOD_ISS_WR_SHIFT)
#define  TIC_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~TIC_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << TIC_S_FN_MOD_ISS_WR_SHIFT))

#define  TIC_S_FN_MOD_ISS_RD_MASK 0x1
#define  TIC_S_FN_MOD_ISS_RD_SHIFT 0
#define  TIC_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & TIC_S_FN_MOD_ISS_RD_MASK) >> TIC_S_FN_MOD_ISS_RD_SHIFT)
#define  TIC_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~TIC_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << TIC_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: JPEGR_S (JPEGR_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  JPEGR_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  JPEGR_S_READ_QOS_RESERVED1_SHIFT 4
#define  JPEGR_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & JPEGR_S_READ_QOS_RESERVED1_MASK) >> JPEGR_S_READ_QOS_RESERVED1_SHIFT)
#define  JPEGR_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JPEGR_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << JPEGR_S_READ_QOS_RESERVED1_SHIFT))

#define  JPEGR_S_READ_QOS_VALUE_MASK 0xf
#define  JPEGR_S_READ_QOS_VALUE_SHIFT 0
#define  JPEGR_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & JPEGR_S_READ_QOS_VALUE_MASK) >> JPEGR_S_READ_QOS_VALUE_SHIFT)
#define  JPEGR_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~JPEGR_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << JPEGR_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  JPEGR_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  JPEGR_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  JPEGR_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & JPEGR_S_WRITE_QOS_RESERVED1_MASK) >> JPEGR_S_WRITE_QOS_RESERVED1_SHIFT)
#define  JPEGR_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JPEGR_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << JPEGR_S_WRITE_QOS_RESERVED1_SHIFT))

#define  JPEGR_S_WRITE_QOS_VALUE_MASK 0xf
#define  JPEGR_S_WRITE_QOS_VALUE_SHIFT 0
#define  JPEGR_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & JPEGR_S_WRITE_QOS_VALUE_MASK) >> JPEGR_S_WRITE_QOS_VALUE_SHIFT)
#define  JPEGR_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~JPEGR_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << JPEGR_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  JPEGR_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  JPEGR_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  JPEGR_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & JPEGR_S_FN_MOD_ISS_RESERVED1_MASK) >> JPEGR_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  JPEGR_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JPEGR_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << JPEGR_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  JPEGR_S_FN_MOD_ISS_WR_MASK 0x2
#define  JPEGR_S_FN_MOD_ISS_WR_SHIFT 1
#define  JPEGR_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & JPEGR_S_FN_MOD_ISS_WR_MASK) >> JPEGR_S_FN_MOD_ISS_WR_SHIFT)
#define  JPEGR_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~JPEGR_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << JPEGR_S_FN_MOD_ISS_WR_SHIFT))

#define  JPEGR_S_FN_MOD_ISS_RD_MASK 0x1
#define  JPEGR_S_FN_MOD_ISS_RD_SHIFT 0
#define  JPEGR_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & JPEGR_S_FN_MOD_ISS_RD_MASK) >> JPEGR_S_FN_MOD_ISS_RD_SHIFT)
#define  JPEGR_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~JPEGR_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << JPEGR_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: JPEGW_S (JPEGW_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  JPEGW_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  JPEGW_S_READ_QOS_RESERVED1_SHIFT 4
#define  JPEGW_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & JPEGW_S_READ_QOS_RESERVED1_MASK) >> JPEGW_S_READ_QOS_RESERVED1_SHIFT)
#define  JPEGW_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JPEGW_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << JPEGW_S_READ_QOS_RESERVED1_SHIFT))

#define  JPEGW_S_READ_QOS_VALUE_MASK 0xf
#define  JPEGW_S_READ_QOS_VALUE_SHIFT 0
#define  JPEGW_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & JPEGW_S_READ_QOS_VALUE_MASK) >> JPEGW_S_READ_QOS_VALUE_SHIFT)
#define  JPEGW_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~JPEGW_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << JPEGW_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  JPEGW_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  JPEGW_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  JPEGW_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & JPEGW_S_WRITE_QOS_RESERVED1_MASK) >> JPEGW_S_WRITE_QOS_RESERVED1_SHIFT)
#define  JPEGW_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JPEGW_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << JPEGW_S_WRITE_QOS_RESERVED1_SHIFT))

#define  JPEGW_S_WRITE_QOS_VALUE_MASK 0xf
#define  JPEGW_S_WRITE_QOS_VALUE_SHIFT 0
#define  JPEGW_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & JPEGW_S_WRITE_QOS_VALUE_MASK) >> JPEGW_S_WRITE_QOS_VALUE_SHIFT)
#define  JPEGW_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~JPEGW_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << JPEGW_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  JPEGW_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  JPEGW_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  JPEGW_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & JPEGW_S_FN_MOD_ISS_RESERVED1_MASK) >> JPEGW_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  JPEGW_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JPEGW_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << JPEGW_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  JPEGW_S_FN_MOD_ISS_WR_MASK 0x2
#define  JPEGW_S_FN_MOD_ISS_WR_SHIFT 1
#define  JPEGW_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & JPEGW_S_FN_MOD_ISS_WR_MASK) >> JPEGW_S_FN_MOD_ISS_WR_SHIFT)
#define  JPEGW_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~JPEGW_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << JPEGW_S_FN_MOD_ISS_WR_SHIFT))

#define  JPEGW_S_FN_MOD_ISS_RD_MASK 0x1
#define  JPEGW_S_FN_MOD_ISS_RD_SHIFT 0
#define  JPEGW_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & JPEGW_S_FN_MOD_ISS_RD_MASK) >> JPEGW_S_FN_MOD_ISS_RD_SHIFT)
#define  JPEGW_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~JPEGW_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << JPEGW_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: JBIG_S (JBIG_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  JBIG_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  JBIG_S_READ_QOS_RESERVED1_SHIFT 4
#define  JBIG_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & JBIG_S_READ_QOS_RESERVED1_MASK) >> JBIG_S_READ_QOS_RESERVED1_SHIFT)
#define  JBIG_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JBIG_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << JBIG_S_READ_QOS_RESERVED1_SHIFT))

#define  JBIG_S_READ_QOS_VALUE_MASK 0xf
#define  JBIG_S_READ_QOS_VALUE_SHIFT 0
#define  JBIG_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & JBIG_S_READ_QOS_VALUE_MASK) >> JBIG_S_READ_QOS_VALUE_SHIFT)
#define  JBIG_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~JBIG_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << JBIG_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  JBIG_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  JBIG_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  JBIG_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & JBIG_S_WRITE_QOS_RESERVED1_MASK) >> JBIG_S_WRITE_QOS_RESERVED1_SHIFT)
#define  JBIG_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JBIG_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << JBIG_S_WRITE_QOS_RESERVED1_SHIFT))

#define  JBIG_S_WRITE_QOS_VALUE_MASK 0xf
#define  JBIG_S_WRITE_QOS_VALUE_SHIFT 0
#define  JBIG_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & JBIG_S_WRITE_QOS_VALUE_MASK) >> JBIG_S_WRITE_QOS_VALUE_SHIFT)
#define  JBIG_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~JBIG_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << JBIG_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  JBIG_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  JBIG_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  JBIG_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & JBIG_S_FN_MOD_ISS_RESERVED1_MASK) >> JBIG_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  JBIG_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~JBIG_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << JBIG_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  JBIG_S_FN_MOD_ISS_WR_MASK 0x2
#define  JBIG_S_FN_MOD_ISS_WR_SHIFT 1
#define  JBIG_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & JBIG_S_FN_MOD_ISS_WR_MASK) >> JBIG_S_FN_MOD_ISS_WR_SHIFT)
#define  JBIG_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~JBIG_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << JBIG_S_FN_MOD_ISS_WR_SHIFT))

#define  JBIG_S_FN_MOD_ISS_RD_MASK 0x1
#define  JBIG_S_FN_MOD_ISS_RD_SHIFT 0
#define  JBIG_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & JBIG_S_FN_MOD_ISS_RD_MASK) >> JBIG_S_FN_MOD_ISS_RD_SHIFT)
#define  JBIG_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~JBIG_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << JBIG_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: MVDO_S (MVDO_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  MVDO_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  MVDO_S_READ_QOS_RESERVED1_SHIFT 4
#define  MVDO_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & MVDO_S_READ_QOS_RESERVED1_MASK) >> MVDO_S_READ_QOS_RESERVED1_SHIFT)
#define  MVDO_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MVDO_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << MVDO_S_READ_QOS_RESERVED1_SHIFT))

#define  MVDO_S_READ_QOS_VALUE_MASK 0xf
#define  MVDO_S_READ_QOS_VALUE_SHIFT 0
#define  MVDO_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & MVDO_S_READ_QOS_VALUE_MASK) >> MVDO_S_READ_QOS_VALUE_SHIFT)
#define  MVDO_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~MVDO_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << MVDO_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  MVDO_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  MVDO_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  MVDO_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & MVDO_S_WRITE_QOS_RESERVED1_MASK) >> MVDO_S_WRITE_QOS_RESERVED1_SHIFT)
#define  MVDO_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MVDO_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << MVDO_S_WRITE_QOS_RESERVED1_SHIFT))

#define  MVDO_S_WRITE_QOS_VALUE_MASK 0xf
#define  MVDO_S_WRITE_QOS_VALUE_SHIFT 0
#define  MVDO_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & MVDO_S_WRITE_QOS_VALUE_MASK) >> MVDO_S_WRITE_QOS_VALUE_SHIFT)
#define  MVDO_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~MVDO_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << MVDO_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  MVDO_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  MVDO_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  MVDO_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & MVDO_S_FN_MOD_ISS_RESERVED1_MASK) >> MVDO_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  MVDO_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MVDO_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << MVDO_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  MVDO_S_FN_MOD_ISS_WR_MASK 0x2
#define  MVDO_S_FN_MOD_ISS_WR_SHIFT 1
#define  MVDO_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & MVDO_S_FN_MOD_ISS_WR_MASK) >> MVDO_S_FN_MOD_ISS_WR_SHIFT)
#define  MVDO_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~MVDO_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << MVDO_S_FN_MOD_ISS_WR_SHIFT))

#define  MVDO_S_FN_MOD_ISS_RD_MASK 0x1
#define  MVDO_S_FN_MOD_ISS_RD_SHIFT 0
#define  MVDO_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & MVDO_S_FN_MOD_ISS_RD_MASK) >> MVDO_S_FN_MOD_ISS_RD_SHIFT)
#define  MVDO_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~MVDO_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << MVDO_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: UPC_S (UPC_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  UPC_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  UPC_S_READ_QOS_RESERVED1_SHIFT 4
#define  UPC_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & UPC_S_READ_QOS_RESERVED1_MASK) >> UPC_S_READ_QOS_RESERVED1_SHIFT)
#define  UPC_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UPC_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << UPC_S_READ_QOS_RESERVED1_SHIFT))

#define  UPC_S_READ_QOS_VALUE_MASK 0xf
#define  UPC_S_READ_QOS_VALUE_SHIFT 0
#define  UPC_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & UPC_S_READ_QOS_VALUE_MASK) >> UPC_S_READ_QOS_VALUE_SHIFT)
#define  UPC_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~UPC_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << UPC_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  UPC_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  UPC_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  UPC_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & UPC_S_WRITE_QOS_RESERVED1_MASK) >> UPC_S_WRITE_QOS_RESERVED1_SHIFT)
#define  UPC_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UPC_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << UPC_S_WRITE_QOS_RESERVED1_SHIFT))

#define  UPC_S_WRITE_QOS_VALUE_MASK 0xf
#define  UPC_S_WRITE_QOS_VALUE_SHIFT 0
#define  UPC_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & UPC_S_WRITE_QOS_VALUE_MASK) >> UPC_S_WRITE_QOS_VALUE_SHIFT)
#define  UPC_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~UPC_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << UPC_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  UPC_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  UPC_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  UPC_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & UPC_S_FN_MOD_ISS_RESERVED1_MASK) >> UPC_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  UPC_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UPC_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << UPC_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  UPC_S_FN_MOD_ISS_WR_MASK 0x2
#define  UPC_S_FN_MOD_ISS_WR_SHIFT 1
#define  UPC_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & UPC_S_FN_MOD_ISS_WR_MASK) >> UPC_S_FN_MOD_ISS_WR_SHIFT)
#define  UPC_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~UPC_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << UPC_S_FN_MOD_ISS_WR_SHIFT))

#define  UPC_S_FN_MOD_ISS_RD_MASK 0x1
#define  UPC_S_FN_MOD_ISS_RD_SHIFT 0
#define  UPC_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & UPC_S_FN_MOD_ISS_RD_MASK) >> UPC_S_FN_MOD_ISS_RD_SHIFT)
#define  UPC_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~UPC_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << UPC_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: CDMA_S (CDMA_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  CDMA_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  CDMA_S_READ_QOS_RESERVED1_SHIFT 4
#define  CDMA_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_S_READ_QOS_RESERVED1_MASK) >> CDMA_S_READ_QOS_RESERVED1_SHIFT)
#define  CDMA_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << CDMA_S_READ_QOS_RESERVED1_SHIFT))

#define  CDMA_S_READ_QOS_VALUE_MASK 0xf
#define  CDMA_S_READ_QOS_VALUE_SHIFT 0
#define  CDMA_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CDMA_S_READ_QOS_VALUE_MASK) >> CDMA_S_READ_QOS_VALUE_SHIFT)
#define  CDMA_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << CDMA_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  CDMA_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  CDMA_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  CDMA_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_S_WRITE_QOS_RESERVED1_MASK) >> CDMA_S_WRITE_QOS_RESERVED1_SHIFT)
#define  CDMA_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << CDMA_S_WRITE_QOS_RESERVED1_SHIFT))

#define  CDMA_S_WRITE_QOS_VALUE_MASK 0xf
#define  CDMA_S_WRITE_QOS_VALUE_SHIFT 0
#define  CDMA_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CDMA_S_WRITE_QOS_VALUE_MASK) >> CDMA_S_WRITE_QOS_VALUE_SHIFT)
#define  CDMA_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << CDMA_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  CDMA_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  CDMA_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  CDMA_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_S_FN_MOD_ISS_RESERVED1_MASK) >> CDMA_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  CDMA_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << CDMA_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  CDMA_S_FN_MOD_ISS_WR_MASK 0x2
#define  CDMA_S_FN_MOD_ISS_WR_SHIFT 1
#define  CDMA_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & CDMA_S_FN_MOD_ISS_WR_MASK) >> CDMA_S_FN_MOD_ISS_WR_SHIFT)
#define  CDMA_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << CDMA_S_FN_MOD_ISS_WR_SHIFT))

#define  CDMA_S_FN_MOD_ISS_RD_MASK 0x1
#define  CDMA_S_FN_MOD_ISS_RD_SHIFT 0
#define  CDMA_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & CDMA_S_FN_MOD_ISS_RD_MASK) >> CDMA_S_FN_MOD_ISS_RD_SHIFT)
#define  CDMA_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~CDMA_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << CDMA_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: USB_S (USB_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  USB_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  USB_S_READ_QOS_RESERVED1_SHIFT 4
#define  USB_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & USB_S_READ_QOS_RESERVED1_MASK) >> USB_S_READ_QOS_RESERVED1_SHIFT)
#define  USB_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~USB_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << USB_S_READ_QOS_RESERVED1_SHIFT))

#define  USB_S_READ_QOS_VALUE_MASK 0xf
#define  USB_S_READ_QOS_VALUE_SHIFT 0
#define  USB_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & USB_S_READ_QOS_VALUE_MASK) >> USB_S_READ_QOS_VALUE_SHIFT)
#define  USB_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~USB_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << USB_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  USB_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  USB_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  USB_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & USB_S_WRITE_QOS_RESERVED1_MASK) >> USB_S_WRITE_QOS_RESERVED1_SHIFT)
#define  USB_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~USB_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << USB_S_WRITE_QOS_RESERVED1_SHIFT))

#define  USB_S_WRITE_QOS_VALUE_MASK 0xf
#define  USB_S_WRITE_QOS_VALUE_SHIFT 0
#define  USB_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & USB_S_WRITE_QOS_VALUE_MASK) >> USB_S_WRITE_QOS_VALUE_SHIFT)
#define  USB_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~USB_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << USB_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  USB_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  USB_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  USB_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & USB_S_FN_MOD_ISS_RESERVED1_MASK) >> USB_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  USB_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~USB_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << USB_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  USB_S_FN_MOD_ISS_WR_MASK 0x2
#define  USB_S_FN_MOD_ISS_WR_SHIFT 1
#define  USB_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & USB_S_FN_MOD_ISS_WR_MASK) >> USB_S_FN_MOD_ISS_WR_SHIFT)
#define  USB_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~USB_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << USB_S_FN_MOD_ISS_WR_SHIFT))

#define  USB_S_FN_MOD_ISS_RD_MASK 0x1
#define  USB_S_FN_MOD_ISS_RD_SHIFT 0
#define  USB_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & USB_S_FN_MOD_ISS_RD_MASK) >> USB_S_FN_MOD_ISS_RD_SHIFT)
#define  USB_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~USB_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << USB_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: SDMMC_S (SDMMC_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  SDMMC_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  SDMMC_S_READ_QOS_RESERVED1_SHIFT 4
#define  SDMMC_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & SDMMC_S_READ_QOS_RESERVED1_MASK) >> SDMMC_S_READ_QOS_RESERVED1_SHIFT)
#define  SDMMC_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << SDMMC_S_READ_QOS_RESERVED1_SHIFT))

#define  SDMMC_S_READ_QOS_VALUE_MASK 0xf
#define  SDMMC_S_READ_QOS_VALUE_SHIFT 0
#define  SDMMC_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & SDMMC_S_READ_QOS_VALUE_MASK) >> SDMMC_S_READ_QOS_VALUE_SHIFT)
#define  SDMMC_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << SDMMC_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  SDMMC_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  SDMMC_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  SDMMC_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & SDMMC_S_WRITE_QOS_RESERVED1_MASK) >> SDMMC_S_WRITE_QOS_RESERVED1_SHIFT)
#define  SDMMC_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << SDMMC_S_WRITE_QOS_RESERVED1_SHIFT))

#define  SDMMC_S_WRITE_QOS_VALUE_MASK 0xf
#define  SDMMC_S_WRITE_QOS_VALUE_SHIFT 0
#define  SDMMC_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & SDMMC_S_WRITE_QOS_VALUE_MASK) >> SDMMC_S_WRITE_QOS_VALUE_SHIFT)
#define  SDMMC_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << SDMMC_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  SDMMC_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  SDMMC_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  SDMMC_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & SDMMC_S_FN_MOD_ISS_RESERVED1_MASK) >> SDMMC_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  SDMMC_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << SDMMC_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  SDMMC_S_FN_MOD_ISS_WR_MASK 0x2
#define  SDMMC_S_FN_MOD_ISS_WR_SHIFT 1
#define  SDMMC_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & SDMMC_S_FN_MOD_ISS_WR_MASK) >> SDMMC_S_FN_MOD_ISS_WR_SHIFT)
#define  SDMMC_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << SDMMC_S_FN_MOD_ISS_WR_SHIFT))

#define  SDMMC_S_FN_MOD_ISS_RD_MASK 0x1
#define  SDMMC_S_FN_MOD_ISS_RD_SHIFT 0
#define  SDMMC_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & SDMMC_S_FN_MOD_ISS_RD_MASK) >> SDMMC_S_FN_MOD_ISS_RD_SHIFT)
#define  SDMMC_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << SDMMC_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: Zx_S (Zx_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  ZX_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  ZX_S_READ_QOS_RESERVED1_SHIFT 4
#define  ZX_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_S_READ_QOS_RESERVED1_MASK) >> ZX_S_READ_QOS_RESERVED1_SHIFT)
#define  ZX_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << ZX_S_READ_QOS_RESERVED1_SHIFT))

#define  ZX_S_READ_QOS_VALUE_MASK 0xf
#define  ZX_S_READ_QOS_VALUE_SHIFT 0
#define  ZX_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & ZX_S_READ_QOS_VALUE_MASK) >> ZX_S_READ_QOS_VALUE_SHIFT)
#define  ZX_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~ZX_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << ZX_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  ZX_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  ZX_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  ZX_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_S_WRITE_QOS_RESERVED1_MASK) >> ZX_S_WRITE_QOS_RESERVED1_SHIFT)
#define  ZX_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << ZX_S_WRITE_QOS_RESERVED1_SHIFT))

#define  ZX_S_WRITE_QOS_VALUE_MASK 0xf
#define  ZX_S_WRITE_QOS_VALUE_SHIFT 0
#define  ZX_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & ZX_S_WRITE_QOS_VALUE_MASK) >> ZX_S_WRITE_QOS_VALUE_SHIFT)
#define  ZX_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~ZX_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << ZX_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  ZX_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  ZX_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  ZX_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_S_FN_MOD_ISS_RESERVED1_MASK) >> ZX_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  ZX_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << ZX_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  ZX_S_FN_MOD_ISS_WR_MASK 0x2
#define  ZX_S_FN_MOD_ISS_WR_SHIFT 1
#define  ZX_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & ZX_S_FN_MOD_ISS_WR_MASK) >> ZX_S_FN_MOD_ISS_WR_SHIFT)
#define  ZX_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~ZX_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << ZX_S_FN_MOD_ISS_WR_SHIFT))

#define  ZX_S_FN_MOD_ISS_RD_MASK 0x1
#define  ZX_S_FN_MOD_ISS_RD_SHIFT 0
#define  ZX_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & ZX_S_FN_MOD_ISS_RD_MASK) >> ZX_S_FN_MOD_ISS_RD_SHIFT)
#define  ZX_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~ZX_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << ZX_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: PIC_S (PIC_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  PIC_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  PIC_S_READ_QOS_RESERVED1_SHIFT 4
#define  PIC_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIC_S_READ_QOS_RESERVED1_MASK) >> PIC_S_READ_QOS_RESERVED1_SHIFT)
#define  PIC_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIC_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << PIC_S_READ_QOS_RESERVED1_SHIFT))

#define  PIC_S_READ_QOS_VALUE_MASK 0xf
#define  PIC_S_READ_QOS_VALUE_SHIFT 0
#define  PIC_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PIC_S_READ_QOS_VALUE_MASK) >> PIC_S_READ_QOS_VALUE_SHIFT)
#define  PIC_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PIC_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << PIC_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  PIC_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  PIC_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  PIC_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIC_S_WRITE_QOS_RESERVED1_MASK) >> PIC_S_WRITE_QOS_RESERVED1_SHIFT)
#define  PIC_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIC_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << PIC_S_WRITE_QOS_RESERVED1_SHIFT))

#define  PIC_S_WRITE_QOS_VALUE_MASK 0xf
#define  PIC_S_WRITE_QOS_VALUE_SHIFT 0
#define  PIC_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & PIC_S_WRITE_QOS_VALUE_MASK) >> PIC_S_WRITE_QOS_VALUE_SHIFT)
#define  PIC_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~PIC_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << PIC_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  PIC_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  PIC_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  PIC_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & PIC_S_FN_MOD_ISS_RESERVED1_MASK) >> PIC_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  PIC_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIC_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << PIC_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  PIC_S_FN_MOD_ISS_WR_MASK 0x2
#define  PIC_S_FN_MOD_ISS_WR_SHIFT 1
#define  PIC_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & PIC_S_FN_MOD_ISS_WR_MASK) >> PIC_S_FN_MOD_ISS_WR_SHIFT)
#define  PIC_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~PIC_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << PIC_S_FN_MOD_ISS_WR_SHIFT))

#define  PIC_S_FN_MOD_ISS_RD_MASK 0x1
#define  PIC_S_FN_MOD_ISS_RD_SHIFT 0
#define  PIC_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & PIC_S_FN_MOD_ISS_RD_MASK) >> PIC_S_FN_MOD_ISS_RD_SHIFT)
#define  PIC_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~PIC_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << PIC_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: CISX_S (CISX_S)
//
//====================================================================

//====================================================================
//Register: read_qos (read_qos)
//====================================================================

#define  CISX_S_READ_QOS_RESERVED1_MASK 0xfffffff0
#define  CISX_S_READ_QOS_RESERVED1_SHIFT 4
#define  CISX_S_READ_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_S_READ_QOS_RESERVED1_MASK) >> CISX_S_READ_QOS_RESERVED1_SHIFT)
#define  CISX_S_READ_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_S_READ_QOS_RESERVED1_MASK) | (((uint32_t)val) << CISX_S_READ_QOS_RESERVED1_SHIFT))

#define  CISX_S_READ_QOS_VALUE_MASK 0xf
#define  CISX_S_READ_QOS_VALUE_SHIFT 0
#define  CISX_S_READ_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CISX_S_READ_QOS_VALUE_MASK) >> CISX_S_READ_QOS_VALUE_SHIFT)
#define  CISX_S_READ_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CISX_S_READ_QOS_VALUE_MASK) | (((uint32_t)val) << CISX_S_READ_QOS_VALUE_SHIFT))

//====================================================================
//Register: write_qos (write_qos)
//====================================================================

#define  CISX_S_WRITE_QOS_RESERVED1_MASK 0xfffffff0
#define  CISX_S_WRITE_QOS_RESERVED1_SHIFT 4
#define  CISX_S_WRITE_QOS_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_S_WRITE_QOS_RESERVED1_MASK) >> CISX_S_WRITE_QOS_RESERVED1_SHIFT)
#define  CISX_S_WRITE_QOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_S_WRITE_QOS_RESERVED1_MASK) | (((uint32_t)val) << CISX_S_WRITE_QOS_RESERVED1_SHIFT))

#define  CISX_S_WRITE_QOS_VALUE_MASK 0xf
#define  CISX_S_WRITE_QOS_VALUE_SHIFT 0
#define  CISX_S_WRITE_QOS_VALUE_MASK_SHIFT(reg) (((reg) & CISX_S_WRITE_QOS_VALUE_MASK) >> CISX_S_WRITE_QOS_VALUE_SHIFT)
#define  CISX_S_WRITE_QOS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CISX_S_WRITE_QOS_VALUE_MASK) | (((uint32_t)val) << CISX_S_WRITE_QOS_VALUE_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  CISX_S_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  CISX_S_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  CISX_S_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_S_FN_MOD_ISS_RESERVED1_MASK) >> CISX_S_FN_MOD_ISS_RESERVED1_SHIFT)
#define  CISX_S_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_S_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << CISX_S_FN_MOD_ISS_RESERVED1_SHIFT))

#define  CISX_S_FN_MOD_ISS_WR_MASK 0x2
#define  CISX_S_FN_MOD_ISS_WR_SHIFT 1
#define  CISX_S_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & CISX_S_FN_MOD_ISS_WR_MASK) >> CISX_S_FN_MOD_ISS_WR_SHIFT)
#define  CISX_S_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~CISX_S_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << CISX_S_FN_MOD_ISS_WR_SHIFT))

#define  CISX_S_FN_MOD_ISS_RD_MASK 0x1
#define  CISX_S_FN_MOD_ISS_RD_SHIFT 0
#define  CISX_S_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & CISX_S_FN_MOD_ISS_RD_MASK) >> CISX_S_FN_MOD_ISS_RD_SHIFT)
#define  CISX_S_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~CISX_S_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << CISX_S_FN_MOD_ISS_RD_SHIFT))

//====================================================================
//
//Register File: IB1 (IB1)
//
//====================================================================

//====================================================================
//Register: fn_mod_bm_iss (fn_mod_bm_iss)
//====================================================================

#define  IB1_FN_MOD_BM_ISS_RESERVED1_MASK 0xfffffffc
#define  IB1_FN_MOD_BM_ISS_RESERVED1_SHIFT 2
#define  IB1_FN_MOD_BM_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & IB1_FN_MOD_BM_ISS_RESERVED1_MASK) >> IB1_FN_MOD_BM_ISS_RESERVED1_SHIFT)
#define  IB1_FN_MOD_BM_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IB1_FN_MOD_BM_ISS_RESERVED1_MASK) | (((uint32_t)val) << IB1_FN_MOD_BM_ISS_RESERVED1_SHIFT))

#define  IB1_FN_MOD_BM_ISS_WR_MASK 0x2
#define  IB1_FN_MOD_BM_ISS_WR_SHIFT 1
#define  IB1_FN_MOD_BM_ISS_WR_MASK_SHIFT(reg) (((reg) & IB1_FN_MOD_BM_ISS_WR_MASK) >> IB1_FN_MOD_BM_ISS_WR_SHIFT)
#define  IB1_FN_MOD_BM_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~IB1_FN_MOD_BM_ISS_WR_MASK) | (((uint32_t)val) << IB1_FN_MOD_BM_ISS_WR_SHIFT))

#define  IB1_FN_MOD_BM_ISS_RD_MASK 0x1
#define  IB1_FN_MOD_BM_ISS_RD_SHIFT 0
#define  IB1_FN_MOD_BM_ISS_RD_MASK_SHIFT(reg) (((reg) & IB1_FN_MOD_BM_ISS_RD_MASK) >> IB1_FN_MOD_BM_ISS_RD_SHIFT)
#define  IB1_FN_MOD_BM_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~IB1_FN_MOD_BM_ISS_RD_MASK) | (((uint32_t)val) << IB1_FN_MOD_BM_ISS_RD_SHIFT))

//====================================================================
//Register: fn_mod_iss (fn_mod_iss)
//====================================================================

#define  IB1_FN_MOD_ISS_RESERVED1_MASK 0xfffffffc
#define  IB1_FN_MOD_ISS_RESERVED1_SHIFT 2
#define  IB1_FN_MOD_ISS_RESERVED1_MASK_SHIFT(reg) (((reg) & IB1_FN_MOD_ISS_RESERVED1_MASK) >> IB1_FN_MOD_ISS_RESERVED1_SHIFT)
#define  IB1_FN_MOD_ISS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~IB1_FN_MOD_ISS_RESERVED1_MASK) | (((uint32_t)val) << IB1_FN_MOD_ISS_RESERVED1_SHIFT))

#define  IB1_FN_MOD_ISS_WR_MASK 0x2
#define  IB1_FN_MOD_ISS_WR_SHIFT 1
#define  IB1_FN_MOD_ISS_WR_MASK_SHIFT(reg) (((reg) & IB1_FN_MOD_ISS_WR_MASK) >> IB1_FN_MOD_ISS_WR_SHIFT)
#define  IB1_FN_MOD_ISS_WR_REPLACE_VAL(reg,val) (((reg) & ~IB1_FN_MOD_ISS_WR_MASK) | (((uint32_t)val) << IB1_FN_MOD_ISS_WR_SHIFT))

#define  IB1_FN_MOD_ISS_RD_MASK 0x1
#define  IB1_FN_MOD_ISS_RD_SHIFT 0
#define  IB1_FN_MOD_ISS_RD_MASK_SHIFT(reg) (((reg) & IB1_FN_MOD_ISS_RD_MASK) >> IB1_FN_MOD_ISS_RD_SHIFT)
#define  IB1_FN_MOD_ISS_RD_REPLACE_VAL(reg,val) (((reg) & ~IB1_FN_MOD_ISS_RD_MASK) | (((uint32_t)val) << IB1_FN_MOD_ISS_RD_SHIFT))

#endif // BUS
