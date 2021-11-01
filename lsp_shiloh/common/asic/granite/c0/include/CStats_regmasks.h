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
 * \file CStats_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSTATS_REGMASKS_H_
#define _CSTATS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CStats (CStats)
//
//====================================================================

//====================================================================
//Register: Configure CStats Register (CFG)
//====================================================================

#define  CSTATS_CFG_RESERVED1_MASK 0xff000000
#define  CSTATS_CFG_RESERVED1_SHIFT 24
#define  CSTATS_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_RESERVED1_MASK) >> CSTATS_CFG_RESERVED1_SHIFT)
#define  CSTATS_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_CFG_RESERVED1_SHIFT))

#define  CSTATS_CFG_EOCT_MASK 0x800000
#define  CSTATS_CFG_EOCT_SHIFT 23
#define  CSTATS_CFG_EOCT_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_EOCT_MASK) >> CSTATS_CFG_EOCT_SHIFT)
#define  CSTATS_CFG_EOCT_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_EOCT_MASK) | (((uint32_t)val) << CSTATS_CFG_EOCT_SHIFT))

#define  CSTATS_CFG_MH_MASK 0x400000
#define  CSTATS_CFG_MH_SHIFT 22
#define  CSTATS_CFG_MH_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_MH_MASK) >> CSTATS_CFG_MH_SHIFT)
#define  CSTATS_CFG_MH_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_MH_MASK) | (((uint32_t)val) << CSTATS_CFG_MH_SHIFT))

#define  CSTATS_CFG_IM_MASK 0x300000
#define  CSTATS_CFG_IM_SHIFT 20
#define  CSTATS_CFG_IM_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_IM_MASK) >> CSTATS_CFG_IM_SHIFT)
#define  CSTATS_CFG_IM_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_IM_MASK) | (((uint32_t)val) << CSTATS_CFG_IM_SHIFT))

#define  CSTATS_CFG_SO_MASK 0xf0000
#define  CSTATS_CFG_SO_SHIFT 16
#define  CSTATS_CFG_SO_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_SO_MASK) >> CSTATS_CFG_SO_SHIFT)
#define  CSTATS_CFG_SO_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_SO_MASK) | (((uint32_t)val) << CSTATS_CFG_SO_SHIFT))

#define  CSTATS_CFG_MCC_MASK 0x8000
#define  CSTATS_CFG_MCC_SHIFT 15
#define  CSTATS_CFG_MCC_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_MCC_MASK) >> CSTATS_CFG_MCC_SHIFT)
#define  CSTATS_CFG_MCC_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_MCC_MASK) | (((uint32_t)val) << CSTATS_CFG_MCC_SHIFT))

#define  CSTATS_CFG_MSC_MASK 0x4000
#define  CSTATS_CFG_MSC_SHIFT 14
#define  CSTATS_CFG_MSC_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_MSC_MASK) >> CSTATS_CFG_MSC_SHIFT)
#define  CSTATS_CFG_MSC_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_MSC_MASK) | (((uint32_t)val) << CSTATS_CFG_MSC_SHIFT))

#define  CSTATS_CFG_SH_MASK 0x3f00
#define  CSTATS_CFG_SH_SHIFT 8
#define  CSTATS_CFG_SH_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_SH_MASK) >> CSTATS_CFG_SH_SHIFT)
#define  CSTATS_CFG_SH_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_SH_MASK) | (((uint32_t)val) << CSTATS_CFG_SH_SHIFT))

#define  CSTATS_CFG_RESERVED2_MASK 0x80
#define  CSTATS_CFG_RESERVED2_SHIFT 7
#define  CSTATS_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_RESERVED2_MASK) >> CSTATS_CFG_RESERVED2_SHIFT)
#define  CSTATS_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_RESERVED2_MASK) | (((uint32_t)val) << CSTATS_CFG_RESERVED2_SHIFT))

#define  CSTATS_CFG_ESC_MASK 0x40
#define  CSTATS_CFG_ESC_SHIFT 6
#define  CSTATS_CFG_ESC_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_ESC_MASK) >> CSTATS_CFG_ESC_SHIFT)
#define  CSTATS_CFG_ESC_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_ESC_MASK) | (((uint32_t)val) << CSTATS_CFG_ESC_SHIFT))

#define  CSTATS_CFG_CS_MASK 0x20
#define  CSTATS_CFG_CS_SHIFT 5
#define  CSTATS_CFG_CS_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_CS_MASK) >> CSTATS_CFG_CS_SHIFT)
#define  CSTATS_CFG_CS_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_CS_MASK) | (((uint32_t)val) << CSTATS_CFG_CS_SHIFT))

#define  CSTATS_CFG_MONO_MASK 0x10
#define  CSTATS_CFG_MONO_SHIFT 4
#define  CSTATS_CFG_MONO_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_MONO_MASK) >> CSTATS_CFG_MONO_SHIFT)
#define  CSTATS_CFG_MONO_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_MONO_MASK) | (((uint32_t)val) << CSTATS_CFG_MONO_SHIFT))

#define  CSTATS_CFG_EH_MASK 0xc
#define  CSTATS_CFG_EH_SHIFT 2
#define  CSTATS_CFG_EH_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_EH_MASK) >> CSTATS_CFG_EH_SHIFT)
#define  CSTATS_CFG_EH_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_EH_MASK) | (((uint32_t)val) << CSTATS_CFG_EH_SHIFT))

#define  CSTATS_CFG_ECC_MASK 0x2
#define  CSTATS_CFG_ECC_SHIFT 1
#define  CSTATS_CFG_ECC_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_ECC_MASK) >> CSTATS_CFG_ECC_SHIFT)
#define  CSTATS_CFG_ECC_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_ECC_MASK) | (((uint32_t)val) << CSTATS_CFG_ECC_SHIFT))

#define  CSTATS_CFG_E_MASK 0x1
#define  CSTATS_CFG_E_SHIFT 0
#define  CSTATS_CFG_E_MASK_SHIFT(reg) (((reg) & CSTATS_CFG_E_MASK) >> CSTATS_CFG_E_SHIFT)
#define  CSTATS_CFG_E_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFG_E_MASK) | (((uint32_t)val) << CSTATS_CFG_E_SHIFT))

//====================================================================
//Register: Left Margin Register (LM)
//====================================================================

#define  CSTATS_LM_RESERVED1_MASK 0xfffc0000
#define  CSTATS_LM_RESERVED1_SHIFT 18
#define  CSTATS_LM_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_LM_RESERVED1_MASK) >> CSTATS_LM_RESERVED1_SHIFT)
#define  CSTATS_LM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_LM_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_LM_RESERVED1_SHIFT))

#define  CSTATS_LM_LM_MASK 0x3ffff
#define  CSTATS_LM_LM_SHIFT 0
#define  CSTATS_LM_LM_MASK_SHIFT(reg) (((reg) & CSTATS_LM_LM_MASK) >> CSTATS_LM_LM_SHIFT)
#define  CSTATS_LM_LM_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_LM_LM_MASK) | (((uint32_t)val) << CSTATS_LM_LM_SHIFT))

//====================================================================
//Register: Right Margin Register (RM)
//====================================================================

#define  CSTATS_RM_RESERVED1_MASK 0xfffc0000
#define  CSTATS_RM_RESERVED1_SHIFT 18
#define  CSTATS_RM_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_RM_RESERVED1_MASK) >> CSTATS_RM_RESERVED1_SHIFT)
#define  CSTATS_RM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RM_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_RM_RESERVED1_SHIFT))

#define  CSTATS_RM_RM_MASK 0x3ffff
#define  CSTATS_RM_RM_SHIFT 0
#define  CSTATS_RM_RM_MASK_SHIFT(reg) (((reg) & CSTATS_RM_RM_MASK) >> CSTATS_RM_RM_SHIFT)
#define  CSTATS_RM_RM_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RM_RM_MASK) | (((uint32_t)val) << CSTATS_RM_RM_SHIFT))

//====================================================================
//Register: Section Width (SW)
//====================================================================

#define  CSTATS_SW_RESERVED1_MASK 0xffffe000
#define  CSTATS_SW_RESERVED1_SHIFT 13
#define  CSTATS_SW_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_SW_RESERVED1_MASK) >> CSTATS_SW_RESERVED1_SHIFT)
#define  CSTATS_SW_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SW_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_SW_RESERVED1_SHIFT))

#define  CSTATS_SW_SW_MASK 0x1fff
#define  CSTATS_SW_SW_SHIFT 0
#define  CSTATS_SW_SW_MASK_SHIFT(reg) (((reg) & CSTATS_SW_SW_MASK) >> CSTATS_SW_SW_SHIFT)
#define  CSTATS_SW_SW_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SW_SW_MASK) | (((uint32_t)val) << CSTATS_SW_SW_SHIFT))

//====================================================================
//Register: Strip Width (STW)
//====================================================================

#define  CSTATS_STW_RESERVED1_MASK 0xfffc0000
#define  CSTATS_STW_RESERVED1_SHIFT 18
#define  CSTATS_STW_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_STW_RESERVED1_MASK) >> CSTATS_STW_RESERVED1_SHIFT)
#define  CSTATS_STW_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_STW_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_STW_RESERVED1_SHIFT))

#define  CSTATS_STW_SW_MASK 0x3ffff
#define  CSTATS_STW_SW_SHIFT 0
#define  CSTATS_STW_SW_MASK_SHIFT(reg) (((reg) & CSTATS_STW_SW_MASK) >> CSTATS_STW_SW_SHIFT)
#define  CSTATS_STW_SW_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_STW_SW_MASK) | (((uint32_t)val) << CSTATS_STW_SW_SHIFT))

//====================================================================
//Register: Threshold Register 1 (TR1)
//====================================================================

#define  CSTATS_TR1_CBH_MASK 0xff000000
#define  CSTATS_TR1_CBH_SHIFT 24
#define  CSTATS_TR1_CBH_MASK_SHIFT(reg) (((reg) & CSTATS_TR1_CBH_MASK) >> CSTATS_TR1_CBH_SHIFT)
#define  CSTATS_TR1_CBH_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR1_CBH_MASK) | (((uint32_t)val) << CSTATS_TR1_CBH_SHIFT))

#define  CSTATS_TR1_CBL_MASK 0xff0000
#define  CSTATS_TR1_CBL_SHIFT 16
#define  CSTATS_TR1_CBL_MASK_SHIFT(reg) (((reg) & CSTATS_TR1_CBL_MASK) >> CSTATS_TR1_CBL_SHIFT)
#define  CSTATS_TR1_CBL_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR1_CBL_MASK) | (((uint32_t)val) << CSTATS_TR1_CBL_SHIFT))

#define  CSTATS_TR1_CRH_MASK 0xff00
#define  CSTATS_TR1_CRH_SHIFT 8
#define  CSTATS_TR1_CRH_MASK_SHIFT(reg) (((reg) & CSTATS_TR1_CRH_MASK) >> CSTATS_TR1_CRH_SHIFT)
#define  CSTATS_TR1_CRH_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR1_CRH_MASK) | (((uint32_t)val) << CSTATS_TR1_CRH_SHIFT))

#define  CSTATS_TR1_CRL_MASK 0xff
#define  CSTATS_TR1_CRL_SHIFT 0
#define  CSTATS_TR1_CRL_MASK_SHIFT(reg) (((reg) & CSTATS_TR1_CRL_MASK) >> CSTATS_TR1_CRL_SHIFT)
#define  CSTATS_TR1_CRL_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR1_CRL_MASK) | (((uint32_t)val) << CSTATS_TR1_CRL_SHIFT))

//====================================================================
//Register: Threshold Register 2 (TR2)
//====================================================================

#define  CSTATS_TR2_RESERVED1_MASK 0xff000000
#define  CSTATS_TR2_RESERVED1_SHIFT 24
#define  CSTATS_TR2_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_TR2_RESERVED1_MASK) >> CSTATS_TR2_RESERVED1_SHIFT)
#define  CSTATS_TR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR2_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_TR2_RESERVED1_SHIFT))

#define  CSTATS_TR2_RGBT_MASK 0xff0000
#define  CSTATS_TR2_RGBT_SHIFT 16
#define  CSTATS_TR2_RGBT_MASK_SHIFT(reg) (((reg) & CSTATS_TR2_RGBT_MASK) >> CSTATS_TR2_RGBT_SHIFT)
#define  CSTATS_TR2_RGBT_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR2_RGBT_MASK) | (((uint32_t)val) << CSTATS_TR2_RGBT_SHIFT))

#define  CSTATS_TR2_WT_MASK 0xff00
#define  CSTATS_TR2_WT_SHIFT 8
#define  CSTATS_TR2_WT_MASK_SHIFT(reg) (((reg) & CSTATS_TR2_WT_MASK) >> CSTATS_TR2_WT_SHIFT)
#define  CSTATS_TR2_WT_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR2_WT_MASK) | (((uint32_t)val) << CSTATS_TR2_WT_SHIFT))

#define  CSTATS_TR2_BT_MASK 0xff
#define  CSTATS_TR2_BT_SHIFT 0
#define  CSTATS_TR2_BT_MASK_SHIFT(reg) (((reg) & CSTATS_TR2_BT_MASK) >> CSTATS_TR2_BT_SHIFT)
#define  CSTATS_TR2_BT_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_TR2_BT_MASK) | (((uint32_t)val) << CSTATS_TR2_BT_SHIFT))

//====================================================================
//Register: Strip Black (SBLK)
//====================================================================

#define  CSTATS_SBLK_RESERVED1_MASK 0xff000000
#define  CSTATS_SBLK_RESERVED1_SHIFT 24
#define  CSTATS_SBLK_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_SBLK_RESERVED1_MASK) >> CSTATS_SBLK_RESERVED1_SHIFT)
#define  CSTATS_SBLK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SBLK_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_SBLK_RESERVED1_SHIFT))

#define  CSTATS_SBLK_SBLK_MASK 0xffffff
#define  CSTATS_SBLK_SBLK_SHIFT 0
#define  CSTATS_SBLK_SBLK_MASK_SHIFT(reg) (((reg) & CSTATS_SBLK_SBLK_MASK) >> CSTATS_SBLK_SBLK_SHIFT)
#define  CSTATS_SBLK_SBLK_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SBLK_SBLK_MASK) | (((uint32_t)val) << CSTATS_SBLK_SBLK_SHIFT))

//====================================================================
//Register: Strip Grey (SGRY)
//====================================================================

#define  CSTATS_SGRY_RESERVED1_MASK 0xff000000
#define  CSTATS_SGRY_RESERVED1_SHIFT 24
#define  CSTATS_SGRY_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_SGRY_RESERVED1_MASK) >> CSTATS_SGRY_RESERVED1_SHIFT)
#define  CSTATS_SGRY_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SGRY_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_SGRY_RESERVED1_SHIFT))

#define  CSTATS_SGRY_SGRY_MASK 0xffffff
#define  CSTATS_SGRY_SGRY_SHIFT 0
#define  CSTATS_SGRY_SGRY_MASK_SHIFT(reg) (((reg) & CSTATS_SGRY_SGRY_MASK) >> CSTATS_SGRY_SGRY_SHIFT)
#define  CSTATS_SGRY_SGRY_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SGRY_SGRY_MASK) | (((uint32_t)val) << CSTATS_SGRY_SGRY_SHIFT))

//====================================================================
//Register: Strip Color (SC)
//====================================================================

#define  CSTATS_SC_RESERVED1_MASK 0xff000000
#define  CSTATS_SC_RESERVED1_SHIFT 24
#define  CSTATS_SC_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_SC_RESERVED1_MASK) >> CSTATS_SC_RESERVED1_SHIFT)
#define  CSTATS_SC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SC_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_SC_RESERVED1_SHIFT))

#define  CSTATS_SC_SC_MASK 0xffffff
#define  CSTATS_SC_SC_SHIFT 0
#define  CSTATS_SC_SC_MASK_SHIFT(reg) (((reg) & CSTATS_SC_SC_MASK) >> CSTATS_SC_SC_SHIFT)
#define  CSTATS_SC_SC_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_SC_SC_MASK) | (((uint32_t)val) << CSTATS_SC_SC_SHIFT))

//====================================================================
//Register: Configure Register B (CFGB)
//====================================================================

#define  CSTATS_CFGB_RESERVED1_MASK 0xfffffffe
#define  CSTATS_CFGB_RESERVED1_SHIFT 1
#define  CSTATS_CFGB_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_CFGB_RESERVED1_MASK) >> CSTATS_CFGB_RESERVED1_SHIFT)
#define  CSTATS_CFGB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFGB_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_CFGB_RESERVED1_SHIFT))

#define  CSTATS_CFGB_HCLR_MASK 0x1
#define  CSTATS_CFGB_HCLR_SHIFT 0
#define  CSTATS_CFGB_HCLR_MASK_SHIFT(reg) (((reg) & CSTATS_CFGB_HCLR_MASK) >> CSTATS_CFGB_HCLR_SHIFT)
#define  CSTATS_CFGB_HCLR_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_CFGB_HCLR_MASK) | (((uint32_t)val) << CSTATS_CFGB_HCLR_SHIFT))

//====================================================================
//Register: Ram 0 (RAM0)
//====================================================================

#define  CSTATS_RAM0_RESERVED1_MASK 0xff000000
#define  CSTATS_RAM0_RESERVED1_SHIFT 24
#define  CSTATS_RAM0_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_RAM0_RESERVED1_MASK) >> CSTATS_RAM0_RESERVED1_SHIFT)
#define  CSTATS_RAM0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM0_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_RAM0_RESERVED1_SHIFT))

#define  CSTATS_RAM0_D_MASK 0xffffff
#define  CSTATS_RAM0_D_SHIFT 0
#define  CSTATS_RAM0_D_MASK_SHIFT(reg) (((reg) & CSTATS_RAM0_D_MASK) >> CSTATS_RAM0_D_SHIFT)
#define  CSTATS_RAM0_D_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM0_D_MASK) | (((uint32_t)val) << CSTATS_RAM0_D_SHIFT))

//====================================================================
//Register: Ram 1 (RAM1)
//====================================================================

#define  CSTATS_RAM1_RESERVED1_MASK 0xff000000
#define  CSTATS_RAM1_RESERVED1_SHIFT 24
#define  CSTATS_RAM1_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_RAM1_RESERVED1_MASK) >> CSTATS_RAM1_RESERVED1_SHIFT)
#define  CSTATS_RAM1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM1_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_RAM1_RESERVED1_SHIFT))

#define  CSTATS_RAM1_D_MASK 0xffffff
#define  CSTATS_RAM1_D_SHIFT 0
#define  CSTATS_RAM1_D_MASK_SHIFT(reg) (((reg) & CSTATS_RAM1_D_MASK) >> CSTATS_RAM1_D_SHIFT)
#define  CSTATS_RAM1_D_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM1_D_MASK) | (((uint32_t)val) << CSTATS_RAM1_D_SHIFT))

//====================================================================
//Register: Ram 2 (RAM2)
//====================================================================

#define  CSTATS_RAM2_RESERVED1_MASK 0xff000000
#define  CSTATS_RAM2_RESERVED1_SHIFT 24
#define  CSTATS_RAM2_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_RAM2_RESERVED1_MASK) >> CSTATS_RAM2_RESERVED1_SHIFT)
#define  CSTATS_RAM2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM2_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_RAM2_RESERVED1_SHIFT))

#define  CSTATS_RAM2_D_MASK 0xffffff
#define  CSTATS_RAM2_D_SHIFT 0
#define  CSTATS_RAM2_D_MASK_SHIFT(reg) (((reg) & CSTATS_RAM2_D_MASK) >> CSTATS_RAM2_D_SHIFT)
#define  CSTATS_RAM2_D_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM2_D_MASK) | (((uint32_t)val) << CSTATS_RAM2_D_SHIFT))

//====================================================================
//Register: Ram 3 (RAM3)
//====================================================================

#define  CSTATS_RAM3_RESERVED1_MASK 0xfffff000
#define  CSTATS_RAM3_RESERVED1_SHIFT 12
#define  CSTATS_RAM3_RESERVED1_MASK_SHIFT(reg) (((reg) & CSTATS_RAM3_RESERVED1_MASK) >> CSTATS_RAM3_RESERVED1_SHIFT)
#define  CSTATS_RAM3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM3_RESERVED1_MASK) | (((uint32_t)val) << CSTATS_RAM3_RESERVED1_SHIFT))

#define  CSTATS_RAM3_D_MASK 0xfff
#define  CSTATS_RAM3_D_SHIFT 0
#define  CSTATS_RAM3_D_MASK_SHIFT(reg) (((reg) & CSTATS_RAM3_D_MASK) >> CSTATS_RAM3_D_SHIFT)
#define  CSTATS_RAM3_D_REPLACE_VAL(reg,val) (((reg) & ~CSTATS_RAM3_D_MASK) | (((uint32_t)val) << CSTATS_RAM3_D_SHIFT))

#endif // CSTATS
