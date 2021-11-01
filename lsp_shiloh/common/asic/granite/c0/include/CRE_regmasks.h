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
 * \file CRE_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CRE_REGMASKS_H_
#define _CRE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Color Registration Enhancment (CRE)
/** \brief Top-level register file for CRE*/
//
//====================================================================

//====================================================================
//Register: Status Register (STATUS)
/** \brief CRE Status*/
//====================================================================

#define  CRE_STATUS_RESERVED1_MASK 0xffffff00
#define  CRE_STATUS_RESERVED1_SHIFT 8
#define  CRE_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_STATUS_RESERVED1_MASK) >> CRE_STATUS_RESERVED1_SHIFT)
#define  CRE_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_RESERVED1_MASK) | (((uint32_t)val) << CRE_STATUS_RESERVED1_SHIFT))

#define  CRE_STATUS_IDT_MASK 0x80
#define  CRE_STATUS_IDT_SHIFT 7
#define  CRE_STATUS_IDT_MASK_SHIFT(reg) (((reg) & CRE_STATUS_IDT_MASK) >> CRE_STATUS_IDT_SHIFT)
#define  CRE_STATUS_IDT_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_IDT_MASK) | (((uint32_t)val) << CRE_STATUS_IDT_SHIFT))

#define  CRE_STATUS_SR_MASK 0x40
#define  CRE_STATUS_SR_SHIFT 6
#define  CRE_STATUS_SR_MASK_SHIFT(reg) (((reg) & CRE_STATUS_SR_MASK) >> CRE_STATUS_SR_SHIFT)
#define  CRE_STATUS_SR_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_SR_MASK) | (((uint32_t)val) << CRE_STATUS_SR_SHIFT))

#define  CRE_STATUS_ITS_MASK 0x20
#define  CRE_STATUS_ITS_SHIFT 5
#define  CRE_STATUS_ITS_MASK_SHIFT(reg) (((reg) & CRE_STATUS_ITS_MASK) >> CRE_STATUS_ITS_SHIFT)
#define  CRE_STATUS_ITS_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_ITS_MASK) | (((uint32_t)val) << CRE_STATUS_ITS_SHIFT))

#define  CRE_STATUS_PRS_MASK 0x10
#define  CRE_STATUS_PRS_SHIFT 4
#define  CRE_STATUS_PRS_MASK_SHIFT(reg) (((reg) & CRE_STATUS_PRS_MASK) >> CRE_STATUS_PRS_SHIFT)
#define  CRE_STATUS_PRS_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_PRS_MASK) | (((uint32_t)val) << CRE_STATUS_PRS_SHIFT))

#define  CRE_STATUS_MSR_MASK 0x8
#define  CRE_STATUS_MSR_SHIFT 3
#define  CRE_STATUS_MSR_MASK_SHIFT(reg) (((reg) & CRE_STATUS_MSR_MASK) >> CRE_STATUS_MSR_SHIFT)
#define  CRE_STATUS_MSR_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_MSR_MASK) | (((uint32_t)val) << CRE_STATUS_MSR_SHIFT))

#define  CRE_STATUS_LLV_MASK 0x4
#define  CRE_STATUS_LLV_SHIFT 2
#define  CRE_STATUS_LLV_MASK_SHIFT(reg) (((reg) & CRE_STATUS_LLV_MASK) >> CRE_STATUS_LLV_SHIFT)
#define  CRE_STATUS_LLV_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_LLV_MASK) | (((uint32_t)val) << CRE_STATUS_LLV_SHIFT))

#define  CRE_STATUS_OLL_MASK 0x2
#define  CRE_STATUS_OLL_SHIFT 1
#define  CRE_STATUS_OLL_MASK_SHIFT(reg) (((reg) & CRE_STATUS_OLL_MASK) >> CRE_STATUS_OLL_SHIFT)
#define  CRE_STATUS_OLL_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_OLL_MASK) | (((uint32_t)val) << CRE_STATUS_OLL_SHIFT))

#define  CRE_STATUS_LLE_MASK 0x1
#define  CRE_STATUS_LLE_SHIFT 0
#define  CRE_STATUS_LLE_MASK_SHIFT(reg) (((reg) & CRE_STATUS_LLE_MASK) >> CRE_STATUS_LLE_SHIFT)
#define  CRE_STATUS_LLE_REPLACE_VAL(reg,val) (((reg) & ~CRE_STATUS_LLE_MASK) | (((uint32_t)val) << CRE_STATUS_LLE_SHIFT))

//====================================================================
//Register: CLEAR STATUS REG (CLR_STATUS)
/** \brief This is a write only register used to clear latched status bits (error signals).
Note:
These bits are also cleared when a softReset is received, or the block is placed in Bypass mode.*/
//====================================================================

#define  CRE_CLR_STATUS_RESERVED1_MASK 0xfffffffe
#define  CRE_CLR_STATUS_RESERVED1_SHIFT 1
#define  CRE_CLR_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CLR_STATUS_RESERVED1_MASK) >> CRE_CLR_STATUS_RESERVED1_SHIFT)
#define  CRE_CLR_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CLR_STATUS_RESERVED1_MASK) | (((uint32_t)val) << CRE_CLR_STATUS_RESERVED1_SHIFT))

#define  CRE_CLR_STATUS_CLR_ERR_MASK 0x1
#define  CRE_CLR_STATUS_CLR_ERR_SHIFT 0
#define  CRE_CLR_STATUS_CLR_ERR_MASK_SHIFT(reg) (((reg) & CRE_CLR_STATUS_CLR_ERR_MASK) >> CRE_CLR_STATUS_CLR_ERR_SHIFT)
#define  CRE_CLR_STATUS_CLR_ERR_REPLACE_VAL(reg,val) (((reg) & ~CRE_CLR_STATUS_CLR_ERR_MASK) | (((uint32_t)val) << CRE_CLR_STATUS_CLR_ERR_SHIFT))

//====================================================================
//Register: CRE CONTROL REG (CNTRL)
/** \brief Top Level C.R.E. Control Register*/
//====================================================================

#define  CRE_CNTRL_BYPASSALL_MASK 0x80000000
#define  CRE_CNTRL_BYPASSALL_SHIFT 31
#define  CRE_CNTRL_BYPASSALL_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_BYPASSALL_MASK) >> CRE_CNTRL_BYPASSALL_SHIFT)
#define  CRE_CNTRL_BYPASSALL_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_BYPASSALL_MASK) | (((uint32_t)val) << CRE_CNTRL_BYPASSALL_SHIFT))

#define  CRE_CNTRL_RESERVED1_MASK 0x7ffffe00
#define  CRE_CNTRL_RESERVED1_SHIFT 9
#define  CRE_CNTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_RESERVED1_MASK) >> CRE_CNTRL_RESERVED1_SHIFT)
#define  CRE_CNTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_RESERVED1_MASK) | (((uint32_t)val) << CRE_CNTRL_RESERVED1_SHIFT))

#define  CRE_CNTRL_CPUACCESS_MASK 0x100
#define  CRE_CNTRL_CPUACCESS_SHIFT 8
#define  CRE_CNTRL_CPUACCESS_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_CPUACCESS_MASK) >> CRE_CNTRL_CPUACCESS_SHIFT)
#define  CRE_CNTRL_CPUACCESS_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_CPUACCESS_MASK) | (((uint32_t)val) << CRE_CNTRL_CPUACCESS_SHIFT))

#define  CRE_CNTRL_HX2_MASK 0x80
#define  CRE_CNTRL_HX2_SHIFT 7
#define  CRE_CNTRL_HX2_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_HX2_MASK) >> CRE_CNTRL_HX2_SHIFT)
#define  CRE_CNTRL_HX2_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_HX2_MASK) | (((uint32_t)val) << CRE_CNTRL_HX2_SHIFT))

#define  CRE_CNTRL_VX2_MASK 0x40
#define  CRE_CNTRL_VX2_SHIFT 6
#define  CRE_CNTRL_VX2_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_VX2_MASK) >> CRE_CNTRL_VX2_SHIFT)
#define  CRE_CNTRL_VX2_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_VX2_MASK) | (((uint32_t)val) << CRE_CNTRL_VX2_SHIFT))

#define  CRE_CNTRL_RESERVED2_MASK 0x30
#define  CRE_CNTRL_RESERVED2_SHIFT 4
#define  CRE_CNTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_RESERVED2_MASK) >> CRE_CNTRL_RESERVED2_SHIFT)
#define  CRE_CNTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_RESERVED2_MASK) | (((uint32_t)val) << CRE_CNTRL_RESERVED2_SHIFT))

#define  CRE_CNTRL_COE_MASK 0x8
#define  CRE_CNTRL_COE_SHIFT 3
#define  CRE_CNTRL_COE_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_COE_MASK) >> CRE_CNTRL_COE_SHIFT)
#define  CRE_CNTRL_COE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_COE_MASK) | (((uint32_t)val) << CRE_CNTRL_COE_SHIFT))

#define  CRE_CNTRL_MOE_MASK 0x4
#define  CRE_CNTRL_MOE_SHIFT 2
#define  CRE_CNTRL_MOE_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_MOE_MASK) >> CRE_CNTRL_MOE_SHIFT)
#define  CRE_CNTRL_MOE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_MOE_MASK) | (((uint32_t)val) << CRE_CNTRL_MOE_SHIFT))

#define  CRE_CNTRL_YOE_MASK 0x2
#define  CRE_CNTRL_YOE_SHIFT 1
#define  CRE_CNTRL_YOE_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_YOE_MASK) >> CRE_CNTRL_YOE_SHIFT)
#define  CRE_CNTRL_YOE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_YOE_MASK) | (((uint32_t)val) << CRE_CNTRL_YOE_SHIFT))

#define  CRE_CNTRL_KOE_MASK 0x1
#define  CRE_CNTRL_KOE_SHIFT 0
#define  CRE_CNTRL_KOE_MASK_SHIFT(reg) (((reg) & CRE_CNTRL_KOE_MASK) >> CRE_CNTRL_KOE_SHIFT)
#define  CRE_CNTRL_KOE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CNTRL_KOE_MASK) | (((uint32_t)val) << CRE_CNTRL_KOE_SHIFT))

//====================================================================
//Register: TRAPPING CONTROL REG (TRAP_CNTRL)
/** \brief Control for trapping functions*/
//====================================================================

#define  CRE_TRAP_CNTRL_ENABLE_MASK 0x80000000
#define  CRE_TRAP_CNTRL_ENABLE_SHIFT 31
#define  CRE_TRAP_CNTRL_ENABLE_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_ENABLE_MASK) >> CRE_TRAP_CNTRL_ENABLE_SHIFT)
#define  CRE_TRAP_CNTRL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_ENABLE_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_ENABLE_SHIFT))

#define  CRE_TRAP_CNTRL_RESERVED1_MASK 0x7ffffc00
#define  CRE_TRAP_CNTRL_RESERVED1_SHIFT 10
#define  CRE_TRAP_CNTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_RESERVED1_MASK) >> CRE_TRAP_CNTRL_RESERVED1_SHIFT)
#define  CRE_TRAP_CNTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_RESERVED1_SHIFT))

#define  CRE_TRAP_CNTRL_OTMODE_MASK 0x300
#define  CRE_TRAP_CNTRL_OTMODE_SHIFT 8
#define  CRE_TRAP_CNTRL_OTMODE_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_OTMODE_MASK) >> CRE_TRAP_CNTRL_OTMODE_SHIFT)
#define  CRE_TRAP_CNTRL_OTMODE_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_OTMODE_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_OTMODE_SHIFT))

#define  CRE_TRAP_CNTRL_RESERVED2_MASK 0x80
#define  CRE_TRAP_CNTRL_RESERVED2_SHIFT 7
#define  CRE_TRAP_CNTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_RESERVED2_MASK) >> CRE_TRAP_CNTRL_RESERVED2_SHIFT)
#define  CRE_TRAP_CNTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_RESERVED2_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_RESERVED2_SHIFT))

#define  CRE_TRAP_CNTRL_ENPIXSIDE_MASK 0x40
#define  CRE_TRAP_CNTRL_ENPIXSIDE_SHIFT 6
#define  CRE_TRAP_CNTRL_ENPIXSIDE_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_ENPIXSIDE_MASK) >> CRE_TRAP_CNTRL_ENPIXSIDE_SHIFT)
#define  CRE_TRAP_CNTRL_ENPIXSIDE_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_ENPIXSIDE_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_ENPIXSIDE_SHIFT))

#define  CRE_TRAP_CNTRL_ENKLINE_MASK 0x20
#define  CRE_TRAP_CNTRL_ENKLINE_SHIFT 5
#define  CRE_TRAP_CNTRL_ENKLINE_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_ENKLINE_MASK) >> CRE_TRAP_CNTRL_ENKLINE_SHIFT)
#define  CRE_TRAP_CNTRL_ENKLINE_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_ENKLINE_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_ENKLINE_SHIFT))

#define  CRE_TRAP_CNTRL_EN1PIXHV_MASK 0x10
#define  CRE_TRAP_CNTRL_EN1PIXHV_SHIFT 4
#define  CRE_TRAP_CNTRL_EN1PIXHV_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_EN1PIXHV_MASK) >> CRE_TRAP_CNTRL_EN1PIXHV_SHIFT)
#define  CRE_TRAP_CNTRL_EN1PIXHV_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_EN1PIXHV_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_EN1PIXHV_SHIFT))

#define  CRE_TRAP_CNTRL_EN2PIXHV_MASK 0x8
#define  CRE_TRAP_CNTRL_EN2PIXHV_SHIFT 3
#define  CRE_TRAP_CNTRL_EN2PIXHV_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_EN2PIXHV_MASK) >> CRE_TRAP_CNTRL_EN2PIXHV_SHIFT)
#define  CRE_TRAP_CNTRL_EN2PIXHV_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_EN2PIXHV_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_EN2PIXHV_SHIFT))

#define  CRE_TRAP_CNTRL_EN2PIX_MASK 0x4
#define  CRE_TRAP_CNTRL_EN2PIX_SHIFT 2
#define  CRE_TRAP_CNTRL_EN2PIX_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_EN2PIX_MASK) >> CRE_TRAP_CNTRL_EN2PIX_SHIFT)
#define  CRE_TRAP_CNTRL_EN2PIX_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_EN2PIX_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_EN2PIX_SHIFT))

#define  CRE_TRAP_CNTRL_EN1PIX_MASK 0x2
#define  CRE_TRAP_CNTRL_EN1PIX_SHIFT 1
#define  CRE_TRAP_CNTRL_EN1PIX_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_EN1PIX_MASK) >> CRE_TRAP_CNTRL_EN1PIX_SHIFT)
#define  CRE_TRAP_CNTRL_EN1PIX_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_EN1PIX_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_EN1PIX_SHIFT))

#define  CRE_TRAP_CNTRL_KONLY_MASK 0x1
#define  CRE_TRAP_CNTRL_KONLY_SHIFT 0
#define  CRE_TRAP_CNTRL_KONLY_MASK_SHIFT(reg) (((reg) & CRE_TRAP_CNTRL_KONLY_MASK) >> CRE_TRAP_CNTRL_KONLY_SHIFT)
#define  CRE_TRAP_CNTRL_KONLY_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_CNTRL_KONLY_MASK) | (((uint32_t)val) << CRE_TRAP_CNTRL_KONLY_SHIFT))

//====================================================================
//Register: TRAP CYAN THRESHOLD (TRAP_T_C)
/** \brief Defines the cyan threshold for a group of pixels to be considered similar (no edge).*/
//====================================================================

#define  CRE_TRAP_T_C_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_T_C_RESERVED1_SHIFT 8
#define  CRE_TRAP_T_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_C_RESERVED1_MASK) >> CRE_TRAP_T_C_RESERVED1_SHIFT)
#define  CRE_TRAP_T_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_T_C_RESERVED1_SHIFT))

#define  CRE_TRAP_T_C_THRESHOLD_MASK 0xff
#define  CRE_TRAP_T_C_THRESHOLD_SHIFT 0
#define  CRE_TRAP_T_C_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_C_THRESHOLD_MASK) >> CRE_TRAP_T_C_THRESHOLD_SHIFT)
#define  CRE_TRAP_T_C_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_C_THRESHOLD_MASK) | (((uint32_t)val) << CRE_TRAP_T_C_THRESHOLD_SHIFT))

//====================================================================
//Register: TRAP MAGENTA THRESHOLD (TRAP_T_M)
/** \brief Defines the magenta threshold for a group of pixels to be considered similar (no edge).*/
//====================================================================

#define  CRE_TRAP_T_M_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_T_M_RESERVED1_SHIFT 8
#define  CRE_TRAP_T_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_M_RESERVED1_MASK) >> CRE_TRAP_T_M_RESERVED1_SHIFT)
#define  CRE_TRAP_T_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_T_M_RESERVED1_SHIFT))

#define  CRE_TRAP_T_M_THRESHOLD_MASK 0xff
#define  CRE_TRAP_T_M_THRESHOLD_SHIFT 0
#define  CRE_TRAP_T_M_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_M_THRESHOLD_MASK) >> CRE_TRAP_T_M_THRESHOLD_SHIFT)
#define  CRE_TRAP_T_M_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_M_THRESHOLD_MASK) | (((uint32_t)val) << CRE_TRAP_T_M_THRESHOLD_SHIFT))

//====================================================================
//Register: TRAP YELLOW THRESHOLD (TRAP_T_Y)
/** \brief Defines the Yellow Threshold for a group of pixels to be considered similar (no edge).*/
//====================================================================

#define  CRE_TRAP_T_Y_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_T_Y_RESERVED1_SHIFT 8
#define  CRE_TRAP_T_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_Y_RESERVED1_MASK) >> CRE_TRAP_T_Y_RESERVED1_SHIFT)
#define  CRE_TRAP_T_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_T_Y_RESERVED1_SHIFT))

#define  CRE_TRAP_T_Y_THRESHOLD_MASK 0xff
#define  CRE_TRAP_T_Y_THRESHOLD_SHIFT 0
#define  CRE_TRAP_T_Y_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_Y_THRESHOLD_MASK) >> CRE_TRAP_T_Y_THRESHOLD_SHIFT)
#define  CRE_TRAP_T_Y_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_Y_THRESHOLD_MASK) | (((uint32_t)val) << CRE_TRAP_T_Y_THRESHOLD_SHIFT))

//====================================================================
//Register: TRAP BLACK TRESHOLD (TRAP_T_K)
/** \brief Defines the Black Threshold for a group of pixels to be considered similar (no edge).*/
//====================================================================

#define  CRE_TRAP_T_K_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_T_K_RESERVED1_SHIFT 8
#define  CRE_TRAP_T_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_K_RESERVED1_MASK) >> CRE_TRAP_T_K_RESERVED1_SHIFT)
#define  CRE_TRAP_T_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_T_K_RESERVED1_SHIFT))

#define  CRE_TRAP_T_K_THRESHOLD_MASK 0xff
#define  CRE_TRAP_T_K_THRESHOLD_SHIFT 0
#define  CRE_TRAP_T_K_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_TRAP_T_K_THRESHOLD_MASK) >> CRE_TRAP_T_K_THRESHOLD_SHIFT)
#define  CRE_TRAP_T_K_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_T_K_THRESHOLD_MASK) | (((uint32_t)val) << CRE_TRAP_T_K_THRESHOLD_SHIFT))

//====================================================================
//Register: TRAP INTENSITY PERCENTAGE LIMIT (TRAP_IPLIM)
/** \brief Sets maximum percentage of trapped color intensity increase allowed compared to original intensity.*/
//====================================================================

#define  CRE_TRAP_IPLIM_RESERVED1_MASK 0xffffffe0
#define  CRE_TRAP_IPLIM_RESERVED1_SHIFT 5
#define  CRE_TRAP_IPLIM_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_IPLIM_RESERVED1_MASK) >> CRE_TRAP_IPLIM_RESERVED1_SHIFT)
#define  CRE_TRAP_IPLIM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_IPLIM_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_IPLIM_RESERVED1_SHIFT))

#define  CRE_TRAP_IPLIM_LIMIT_MASK 0x1f
#define  CRE_TRAP_IPLIM_LIMIT_SHIFT 0
#define  CRE_TRAP_IPLIM_LIMIT_MASK_SHIFT(reg) (((reg) & CRE_TRAP_IPLIM_LIMIT_MASK) >> CRE_TRAP_IPLIM_LIMIT_SHIFT)
#define  CRE_TRAP_IPLIM_LIMIT_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_IPLIM_LIMIT_MASK) | (((uint32_t)val) << CRE_TRAP_IPLIM_LIMIT_SHIFT))

//====================================================================
//Register: TRAP INTENSITY THRESHOLD (TRAP_I_T)
/** \brief Threshold at which two pixels intensities are considered equal for trapping.*/
//====================================================================

#define  CRE_TRAP_I_T_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_I_T_RESERVED1_SHIFT 8
#define  CRE_TRAP_I_T_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_I_T_RESERVED1_MASK) >> CRE_TRAP_I_T_RESERVED1_SHIFT)
#define  CRE_TRAP_I_T_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_I_T_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_I_T_RESERVED1_SHIFT))

#define  CRE_TRAP_I_T_THRESHOLD_MASK 0xff
#define  CRE_TRAP_I_T_THRESHOLD_SHIFT 0
#define  CRE_TRAP_I_T_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_TRAP_I_T_THRESHOLD_MASK) >> CRE_TRAP_I_T_THRESHOLD_SHIFT)
#define  CRE_TRAP_I_T_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_I_T_THRESHOLD_MASK) | (((uint32_t)val) << CRE_TRAP_I_T_THRESHOLD_SHIFT))

//====================================================================
//Register: TRAP BLACK LINE THRESHOLD (TRAP_KLINE_T)
/** \brief Minimum value for the K plane for a pixel to be considered "Black".*/
//====================================================================

#define  CRE_TRAP_KLINE_T_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_KLINE_T_RESERVED1_SHIFT 8
#define  CRE_TRAP_KLINE_T_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_KLINE_T_RESERVED1_MASK) >> CRE_TRAP_KLINE_T_RESERVED1_SHIFT)
#define  CRE_TRAP_KLINE_T_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_KLINE_T_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_KLINE_T_RESERVED1_SHIFT))

#define  CRE_TRAP_KLINE_T_THRESHOLD_MASK 0xff
#define  CRE_TRAP_KLINE_T_THRESHOLD_SHIFT 0
#define  CRE_TRAP_KLINE_T_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_TRAP_KLINE_T_THRESHOLD_MASK) >> CRE_TRAP_KLINE_T_THRESHOLD_SHIFT)
#define  CRE_TRAP_KLINE_T_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_KLINE_T_THRESHOLD_MASK) | (((uint32_t)val) << CRE_TRAP_KLINE_T_THRESHOLD_SHIFT))

//====================================================================
//Register: ADJACENT BLACK PIXELS PER LINE (TRAP_KPLINE)
/** \brief Minimum number of Adjacent Black pixels required for a pixel to be considered as
part of a NON-Horizontal/Vertical black line.*/
//====================================================================

#define  CRE_TRAP_KPLINE_RESERVED1_MASK 0xfffffff0
#define  CRE_TRAP_KPLINE_RESERVED1_SHIFT 4
#define  CRE_TRAP_KPLINE_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_KPLINE_RESERVED1_MASK) >> CRE_TRAP_KPLINE_RESERVED1_SHIFT)
#define  CRE_TRAP_KPLINE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_KPLINE_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_KPLINE_RESERVED1_SHIFT))

#define  CRE_TRAP_KPLINE_THRESHOLD_MASK 0xf
#define  CRE_TRAP_KPLINE_THRESHOLD_SHIFT 0
#define  CRE_TRAP_KPLINE_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_TRAP_KPLINE_THRESHOLD_MASK) >> CRE_TRAP_KPLINE_THRESHOLD_SHIFT)
#define  CRE_TRAP_KPLINE_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_KPLINE_THRESHOLD_MASK) | (((uint32_t)val) << CRE_TRAP_KPLINE_THRESHOLD_SHIFT))

//====================================================================
//Register: TRAP MAXIMUM (TRAP_MAX)
/** \brief Absolute Maximum amount of trapping allowed (C+M+Y+K)*/
//====================================================================

#define  CRE_TRAP_MAX_RESERVED1_MASK 0xfffffc00
#define  CRE_TRAP_MAX_RESERVED1_SHIFT 10
#define  CRE_TRAP_MAX_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_MAX_RESERVED1_MASK) >> CRE_TRAP_MAX_RESERVED1_SHIFT)
#define  CRE_TRAP_MAX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_MAX_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_MAX_RESERVED1_SHIFT))

#define  CRE_TRAP_MAX_MAX_MASK 0x3ff
#define  CRE_TRAP_MAX_MAX_SHIFT 0
#define  CRE_TRAP_MAX_MAX_MASK_SHIFT(reg) (((reg) & CRE_TRAP_MAX_MAX_MASK) >> CRE_TRAP_MAX_MAX_SHIFT)
#define  CRE_TRAP_MAX_MAX_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_MAX_MAX_MASK) | (((uint32_t)val) << CRE_TRAP_MAX_MAX_SHIFT))

//====================================================================
//Register: TRAP CYAN INTENSITY LUT (TRAP_LUTCI)
/** \brief 16x8 Cyan Intensity LUT Indexed by cyan pixel value, returns Intensity.
             For CPU to access this LUT the CNTRL.CPUAccess bit must be set.*/
//====================================================================

#define  CRE_TRAP_LUTCI_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_LUTCI_RESERVED1_SHIFT 8
#define  CRE_TRAP_LUTCI_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTCI_RESERVED1_MASK) >> CRE_TRAP_LUTCI_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTCI_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTCI_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTCI_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTCI_TRAP_LUTCI_MASK 0xff
#define  CRE_TRAP_LUTCI_TRAP_LUTCI_SHIFT 0
#define  CRE_TRAP_LUTCI_TRAP_LUTCI_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTCI_TRAP_LUTCI_MASK) >> CRE_TRAP_LUTCI_TRAP_LUTCI_SHIFT)
#define  CRE_TRAP_LUTCI_TRAP_LUTCI_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTCI_TRAP_LUTCI_MASK) | (((uint32_t)val) << CRE_TRAP_LUTCI_TRAP_LUTCI_SHIFT))

//====================================================================
//Register: TRAP MAGENTA INTENSITY LUT (TRAP_LUTMI)
/** \brief 16x8 Magenta Intensity LUT Indexed by magenta pixel value, returns Intensity
             For CPU to access this LUT the CNTRL.CPUAccess bit must be set.*/
//====================================================================

#define  CRE_TRAP_LUTMI_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_LUTMI_RESERVED1_SHIFT 8
#define  CRE_TRAP_LUTMI_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTMI_RESERVED1_MASK) >> CRE_TRAP_LUTMI_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTMI_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTMI_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTMI_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTMI_TRAP_LUTMI_MASK 0xff
#define  CRE_TRAP_LUTMI_TRAP_LUTMI_SHIFT 0
#define  CRE_TRAP_LUTMI_TRAP_LUTMI_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTMI_TRAP_LUTMI_MASK) >> CRE_TRAP_LUTMI_TRAP_LUTMI_SHIFT)
#define  CRE_TRAP_LUTMI_TRAP_LUTMI_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTMI_TRAP_LUTMI_MASK) | (((uint32_t)val) << CRE_TRAP_LUTMI_TRAP_LUTMI_SHIFT))

//====================================================================
//Register: TRAP YELLOW INTENSITY LUT (TRAP_LUTYI)
/** \brief 8x8 Yellow Intensity LUT Indexed by yellow pixel value, returns Intensity.
             For CPU to access this LUT the CNTRL.CPUAccess bit must be set.*/
//====================================================================

#define  CRE_TRAP_LUTYI_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_LUTYI_RESERVED1_SHIFT 8
#define  CRE_TRAP_LUTYI_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTYI_RESERVED1_MASK) >> CRE_TRAP_LUTYI_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTYI_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTYI_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTYI_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTYI_TRAP_LUTYI_MASK 0xff
#define  CRE_TRAP_LUTYI_TRAP_LUTYI_SHIFT 0
#define  CRE_TRAP_LUTYI_TRAP_LUTYI_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTYI_TRAP_LUTYI_MASK) >> CRE_TRAP_LUTYI_TRAP_LUTYI_SHIFT)
#define  CRE_TRAP_LUTYI_TRAP_LUTYI_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTYI_TRAP_LUTYI_MASK) | (((uint32_t)val) << CRE_TRAP_LUTYI_TRAP_LUTYI_SHIFT))

//====================================================================
//Register: TRAP BLACK INTENSITY LUT (TRAP_LUTKI)
/** \brief 32x8 Black Intensity LUT Indexed by black pixel value, returns Intensity.
             For CPU to access this LUT the CNTRL.CPUAccess bit must be set*/
//====================================================================

#define  CRE_TRAP_LUTKI_RESERVED1_MASK 0xffffff00
#define  CRE_TRAP_LUTKI_RESERVED1_SHIFT 8
#define  CRE_TRAP_LUTKI_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTKI_RESERVED1_MASK) >> CRE_TRAP_LUTKI_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTKI_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTKI_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTKI_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTKI_TRAP_LUTKI_MASK 0xff
#define  CRE_TRAP_LUTKI_TRAP_LUTKI_SHIFT 0
#define  CRE_TRAP_LUTKI_TRAP_LUTKI_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTKI_TRAP_LUTKI_MASK) >> CRE_TRAP_LUTKI_TRAP_LUTKI_SHIFT)
#define  CRE_TRAP_LUTKI_TRAP_LUTKI_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTKI_TRAP_LUTKI_MASK) | (((uint32_t)val) << CRE_TRAP_LUTKI_TRAP_LUTKI_SHIFT))

//====================================================================
//Register: TRAP CYAN FADE LUT (TRAP_LUTCF)
/** \brief 16x4 Cyan Fade LUT Indexed by trap cyan value, returns percentage.
             For CPU to access this LUT the CNTRL.cpuAccess bit must be set*/
//====================================================================

#define  CRE_TRAP_LUTCF_RESERVED1_MASK 0xfffffff0
#define  CRE_TRAP_LUTCF_RESERVED1_SHIFT 4
#define  CRE_TRAP_LUTCF_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTCF_RESERVED1_MASK) >> CRE_TRAP_LUTCF_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTCF_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTCF_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTCF_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTCF_TRAP_LUTCF_MASK 0xf
#define  CRE_TRAP_LUTCF_TRAP_LUTCF_SHIFT 0
#define  CRE_TRAP_LUTCF_TRAP_LUTCF_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTCF_TRAP_LUTCF_MASK) >> CRE_TRAP_LUTCF_TRAP_LUTCF_SHIFT)
#define  CRE_TRAP_LUTCF_TRAP_LUTCF_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTCF_TRAP_LUTCF_MASK) | (((uint32_t)val) << CRE_TRAP_LUTCF_TRAP_LUTCF_SHIFT))

//====================================================================
//Register: TRAP MAGENTA FADE LUT (TRAP_LUTMF)
/** \brief 16x4 Magenta Fade LUT Indexed by trap magenta value, returns percentage.
             For CPU to access this LUT the CNTRL.cpuAccess bit must be set*/
//====================================================================

#define  CRE_TRAP_LUTMF_RESERVED1_MASK 0xfffffff0
#define  CRE_TRAP_LUTMF_RESERVED1_SHIFT 4
#define  CRE_TRAP_LUTMF_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTMF_RESERVED1_MASK) >> CRE_TRAP_LUTMF_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTMF_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTMF_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTMF_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTMF_TRAP_LUTMF_MASK 0xf
#define  CRE_TRAP_LUTMF_TRAP_LUTMF_SHIFT 0
#define  CRE_TRAP_LUTMF_TRAP_LUTMF_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTMF_TRAP_LUTMF_MASK) >> CRE_TRAP_LUTMF_TRAP_LUTMF_SHIFT)
#define  CRE_TRAP_LUTMF_TRAP_LUTMF_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTMF_TRAP_LUTMF_MASK) | (((uint32_t)val) << CRE_TRAP_LUTMF_TRAP_LUTMF_SHIFT))

//====================================================================
//Register: TRAP YELLOW FADE LUT (TRAP_LUTYF)
/** \brief 8x3 Yellow Fade LUT Indexed by trap yellow value, returns percentage.
             For CPU to access this LUT the CNTRL.cpuAccess bit must be set*/
//====================================================================

#define  CRE_TRAP_LUTYF_RESERVED1_MASK 0xfffffff8
#define  CRE_TRAP_LUTYF_RESERVED1_SHIFT 3
#define  CRE_TRAP_LUTYF_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTYF_RESERVED1_MASK) >> CRE_TRAP_LUTYF_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTYF_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTYF_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTYF_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTYF_TRAP_LUTYF_MASK 0x7
#define  CRE_TRAP_LUTYF_TRAP_LUTYF_SHIFT 0
#define  CRE_TRAP_LUTYF_TRAP_LUTYF_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTYF_TRAP_LUTYF_MASK) >> CRE_TRAP_LUTYF_TRAP_LUTYF_SHIFT)
#define  CRE_TRAP_LUTYF_TRAP_LUTYF_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTYF_TRAP_LUTYF_MASK) | (((uint32_t)val) << CRE_TRAP_LUTYF_TRAP_LUTYF_SHIFT))

//====================================================================
//Register: TRAP BLACK FADE LUT (TRAP_LUTKF)
/** \brief 32x5 Black Fade LUT Indexed by trap black value, returns percentage.
             For CPU to access this LUT the CNTRL.cpuAccess bit must be set*/
//====================================================================

#define  CRE_TRAP_LUTKF_RESERVED1_MASK 0xffffffe0
#define  CRE_TRAP_LUTKF_RESERVED1_SHIFT 5
#define  CRE_TRAP_LUTKF_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTKF_RESERVED1_MASK) >> CRE_TRAP_LUTKF_RESERVED1_SHIFT)
#define  CRE_TRAP_LUTKF_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTKF_RESERVED1_MASK) | (((uint32_t)val) << CRE_TRAP_LUTKF_RESERVED1_SHIFT))

#define  CRE_TRAP_LUTKF_TRAP_LUTKF_MASK 0x1f
#define  CRE_TRAP_LUTKF_TRAP_LUTKF_SHIFT 0
#define  CRE_TRAP_LUTKF_TRAP_LUTKF_MASK_SHIFT(reg) (((reg) & CRE_TRAP_LUTKF_TRAP_LUTKF_MASK) >> CRE_TRAP_LUTKF_TRAP_LUTKF_SHIFT)
#define  CRE_TRAP_LUTKF_TRAP_LUTKF_REPLACE_VAL(reg,val) (((reg) & ~CRE_TRAP_LUTKF_TRAP_LUTKF_MASK) | (((uint32_t)val) << CRE_TRAP_LUTKF_TRAP_LUTKF_SHIFT))

//====================================================================
//Register: SHARPEN CONTROL REG (SHARP_CNTRL)
/** \brief Control for SHARPEN functions*/
//====================================================================

#define  CRE_SHARP_CNTRL_ENABLE_MASK 0x80000000
#define  CRE_SHARP_CNTRL_ENABLE_SHIFT 31
#define  CRE_SHARP_CNTRL_ENABLE_MASK_SHIFT(reg) (((reg) & CRE_SHARP_CNTRL_ENABLE_MASK) >> CRE_SHARP_CNTRL_ENABLE_SHIFT)
#define  CRE_SHARP_CNTRL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_CNTRL_ENABLE_MASK) | (((uint32_t)val) << CRE_SHARP_CNTRL_ENABLE_SHIFT))

#define  CRE_SHARP_CNTRL_RESERVED1_MASK 0x7ffffc00
#define  CRE_SHARP_CNTRL_RESERVED1_SHIFT 10
#define  CRE_SHARP_CNTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_CNTRL_RESERVED1_MASK) >> CRE_SHARP_CNTRL_RESERVED1_SHIFT)
#define  CRE_SHARP_CNTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_CNTRL_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_CNTRL_RESERVED1_SHIFT))

#define  CRE_SHARP_CNTRL_OTMODE_MASK 0x300
#define  CRE_SHARP_CNTRL_OTMODE_SHIFT 8
#define  CRE_SHARP_CNTRL_OTMODE_MASK_SHIFT(reg) (((reg) & CRE_SHARP_CNTRL_OTMODE_MASK) >> CRE_SHARP_CNTRL_OTMODE_SHIFT)
#define  CRE_SHARP_CNTRL_OTMODE_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_CNTRL_OTMODE_MASK) | (((uint32_t)val) << CRE_SHARP_CNTRL_OTMODE_SHIFT))

#define  CRE_SHARP_CNTRL_RESERVED2_MASK 0xff
#define  CRE_SHARP_CNTRL_RESERVED2_SHIFT 0
#define  CRE_SHARP_CNTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & CRE_SHARP_CNTRL_RESERVED2_MASK) >> CRE_SHARP_CNTRL_RESERVED2_SHIFT)
#define  CRE_SHARP_CNTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_CNTRL_RESERVED2_MASK) | (((uint32_t)val) << CRE_SHARP_CNTRL_RESERVED2_SHIFT))

//====================================================================
//Register: SHARPEN SCALE CYAN (SHARP_SCALE_C)
/** \brief Scale factor for increasing offset differences in Magenta plane*/
//====================================================================

#define  CRE_SHARP_SCALE_C_RESERVED1_MASK 0xffffffc0
#define  CRE_SHARP_SCALE_C_RESERVED1_SHIFT 6
#define  CRE_SHARP_SCALE_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_C_RESERVED1_MASK) >> CRE_SHARP_SCALE_C_RESERVED1_SHIFT)
#define  CRE_SHARP_SCALE_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_C_RESERVED1_SHIFT))

#define  CRE_SHARP_SCALE_C_SCALE_MASK 0x3f
#define  CRE_SHARP_SCALE_C_SCALE_SHIFT 0
#define  CRE_SHARP_SCALE_C_SCALE_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_C_SCALE_MASK) >> CRE_SHARP_SCALE_C_SCALE_SHIFT)
#define  CRE_SHARP_SCALE_C_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_C_SCALE_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_C_SCALE_SHIFT))

//====================================================================
//Register: SHARPEN SCALE MAGENTA (SHARP_SCALE_M)
/** \brief Scale factor for increasing offset differences in Magenta plane*/
//====================================================================

#define  CRE_SHARP_SCALE_M_RESERVED1_MASK 0xffffffc0
#define  CRE_SHARP_SCALE_M_RESERVED1_SHIFT 6
#define  CRE_SHARP_SCALE_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_M_RESERVED1_MASK) >> CRE_SHARP_SCALE_M_RESERVED1_SHIFT)
#define  CRE_SHARP_SCALE_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_M_RESERVED1_SHIFT))

#define  CRE_SHARP_SCALE_M_SCALE_MASK 0x3f
#define  CRE_SHARP_SCALE_M_SCALE_SHIFT 0
#define  CRE_SHARP_SCALE_M_SCALE_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_M_SCALE_MASK) >> CRE_SHARP_SCALE_M_SCALE_SHIFT)
#define  CRE_SHARP_SCALE_M_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_M_SCALE_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_M_SCALE_SHIFT))

//====================================================================
//Register: SHARPEN SCALE YELLOW (SHARP_SCALE_Y)
/** \brief Scale factor for increasing offset differences in Yellow plane*/
//====================================================================

#define  CRE_SHARP_SCALE_Y_RESERVED1_MASK 0xffffffc0
#define  CRE_SHARP_SCALE_Y_RESERVED1_SHIFT 6
#define  CRE_SHARP_SCALE_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_Y_RESERVED1_MASK) >> CRE_SHARP_SCALE_Y_RESERVED1_SHIFT)
#define  CRE_SHARP_SCALE_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_Y_RESERVED1_SHIFT))

#define  CRE_SHARP_SCALE_Y_SCALE_MASK 0x3f
#define  CRE_SHARP_SCALE_Y_SCALE_SHIFT 0
#define  CRE_SHARP_SCALE_Y_SCALE_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_Y_SCALE_MASK) >> CRE_SHARP_SCALE_Y_SCALE_SHIFT)
#define  CRE_SHARP_SCALE_Y_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_Y_SCALE_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_Y_SCALE_SHIFT))

//====================================================================
//Register: SHARPEN SCALE BLACK (SHARP_SCALE_K)
/** \brief Scale factor for increasing offset differences in Black plane*/
//====================================================================

#define  CRE_SHARP_SCALE_K_RESERVED1_MASK 0xffffffc0
#define  CRE_SHARP_SCALE_K_RESERVED1_SHIFT 6
#define  CRE_SHARP_SCALE_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_K_RESERVED1_MASK) >> CRE_SHARP_SCALE_K_RESERVED1_SHIFT)
#define  CRE_SHARP_SCALE_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_K_RESERVED1_SHIFT))

#define  CRE_SHARP_SCALE_K_SCALE_MASK 0x3f
#define  CRE_SHARP_SCALE_K_SCALE_SHIFT 0
#define  CRE_SHARP_SCALE_K_SCALE_MASK_SHIFT(reg) (((reg) & CRE_SHARP_SCALE_K_SCALE_MASK) >> CRE_SHARP_SCALE_K_SCALE_SHIFT)
#define  CRE_SHARP_SCALE_K_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_SCALE_K_SCALE_MASK) | (((uint32_t)val) << CRE_SHARP_SCALE_K_SCALE_SHIFT))

//====================================================================
//Register: SHARPEN OFFSET CYAN (SHARP_OFFSET_C)
/** \brief Controls how strong a cyan edge must be before it is sharpened*/
//====================================================================

#define  CRE_SHARP_OFFSET_C_RESERVED1_MASK 0xffffff00
#define  CRE_SHARP_OFFSET_C_RESERVED1_SHIFT 8
#define  CRE_SHARP_OFFSET_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_C_RESERVED1_MASK) >> CRE_SHARP_OFFSET_C_RESERVED1_SHIFT)
#define  CRE_SHARP_OFFSET_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_C_RESERVED1_SHIFT))

#define  CRE_SHARP_OFFSET_C_OFFSET_MASK 0xff
#define  CRE_SHARP_OFFSET_C_OFFSET_SHIFT 0
#define  CRE_SHARP_OFFSET_C_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_C_OFFSET_MASK) >> CRE_SHARP_OFFSET_C_OFFSET_SHIFT)
#define  CRE_SHARP_OFFSET_C_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_C_OFFSET_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_C_OFFSET_SHIFT))

//====================================================================
//Register: SHARPEN OFFSET MAGENTA (SHARP_OFFSET_M)
/** \brief Controls how strong a magenta edge must be before it is sharpened*/
//====================================================================

#define  CRE_SHARP_OFFSET_M_RESERVED1_MASK 0xffffff00
#define  CRE_SHARP_OFFSET_M_RESERVED1_SHIFT 8
#define  CRE_SHARP_OFFSET_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_M_RESERVED1_MASK) >> CRE_SHARP_OFFSET_M_RESERVED1_SHIFT)
#define  CRE_SHARP_OFFSET_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_M_RESERVED1_SHIFT))

#define  CRE_SHARP_OFFSET_M_OFFSET_MASK 0xff
#define  CRE_SHARP_OFFSET_M_OFFSET_SHIFT 0
#define  CRE_SHARP_OFFSET_M_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_M_OFFSET_MASK) >> CRE_SHARP_OFFSET_M_OFFSET_SHIFT)
#define  CRE_SHARP_OFFSET_M_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_M_OFFSET_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_M_OFFSET_SHIFT))

//====================================================================
//Register: SHARPEN OFFSET YELLOW (SHARP_OFFSET_Y)
/** \brief Controls how strong a yellow edge must be before it is sharpened*/
//====================================================================

#define  CRE_SHARP_OFFSET_Y_RESERVED1_MASK 0xffffff00
#define  CRE_SHARP_OFFSET_Y_RESERVED1_SHIFT 8
#define  CRE_SHARP_OFFSET_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_Y_RESERVED1_MASK) >> CRE_SHARP_OFFSET_Y_RESERVED1_SHIFT)
#define  CRE_SHARP_OFFSET_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_Y_RESERVED1_SHIFT))

#define  CRE_SHARP_OFFSET_Y_OFFSET_MASK 0xff
#define  CRE_SHARP_OFFSET_Y_OFFSET_SHIFT 0
#define  CRE_SHARP_OFFSET_Y_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_Y_OFFSET_MASK) >> CRE_SHARP_OFFSET_Y_OFFSET_SHIFT)
#define  CRE_SHARP_OFFSET_Y_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_Y_OFFSET_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_Y_OFFSET_SHIFT))

//====================================================================
//Register: SHARPEN OFFSET BLACK (SHARP_OFFSET_K)
/** \brief Controls how strong a black edge must be before it is sharpened*/
//====================================================================

#define  CRE_SHARP_OFFSET_K_RESERVED1_MASK 0xffffff00
#define  CRE_SHARP_OFFSET_K_RESERVED1_SHIFT 8
#define  CRE_SHARP_OFFSET_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_K_RESERVED1_MASK) >> CRE_SHARP_OFFSET_K_RESERVED1_SHIFT)
#define  CRE_SHARP_OFFSET_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_K_RESERVED1_SHIFT))

#define  CRE_SHARP_OFFSET_K_OFFSET_MASK 0xff
#define  CRE_SHARP_OFFSET_K_OFFSET_SHIFT 0
#define  CRE_SHARP_OFFSET_K_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_SHARP_OFFSET_K_OFFSET_MASK) >> CRE_SHARP_OFFSET_K_OFFSET_SHIFT)
#define  CRE_SHARP_OFFSET_K_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_SHARP_OFFSET_K_OFFSET_MASK) | (((uint32_t)val) << CRE_SHARP_OFFSET_K_OFFSET_SHIFT))

//====================================================================
//Register: EDGE STRENGTH CONTROL REG (EDGE_CNTRL)
/** \brief Control for Edge Strength functions*/
//====================================================================

#define  CRE_EDGE_CNTRL_ENABLE_MASK 0x80000000
#define  CRE_EDGE_CNTRL_ENABLE_SHIFT 31
#define  CRE_EDGE_CNTRL_ENABLE_MASK_SHIFT(reg) (((reg) & CRE_EDGE_CNTRL_ENABLE_MASK) >> CRE_EDGE_CNTRL_ENABLE_SHIFT)
#define  CRE_EDGE_CNTRL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_CNTRL_ENABLE_MASK) | (((uint32_t)val) << CRE_EDGE_CNTRL_ENABLE_SHIFT))

#define  CRE_EDGE_CNTRL_RESERVED1_MASK 0x7fffffff
#define  CRE_EDGE_CNTRL_RESERVED1_SHIFT 0
#define  CRE_EDGE_CNTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_CNTRL_RESERVED1_MASK) >> CRE_EDGE_CNTRL_RESERVED1_SHIFT)
#define  CRE_EDGE_CNTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_CNTRL_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_CNTRL_RESERVED1_SHIFT))

//====================================================================
//Register: CYAN EDGE STRENGTH THRESHOLD (EDGE_T_C)
/** \brief Threshold for magnitude of cyan edge that results in output OT bit getting set.*/
//====================================================================

#define  CRE_EDGE_T_C_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_T_C_RESERVED1_SHIFT 12
#define  CRE_EDGE_T_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_C_RESERVED1_MASK) >> CRE_EDGE_T_C_RESERVED1_SHIFT)
#define  CRE_EDGE_T_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_T_C_RESERVED1_SHIFT))

#define  CRE_EDGE_T_C_THRESHOLD_MASK 0xfff
#define  CRE_EDGE_T_C_THRESHOLD_SHIFT 0
#define  CRE_EDGE_T_C_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_C_THRESHOLD_MASK) >> CRE_EDGE_T_C_THRESHOLD_SHIFT)
#define  CRE_EDGE_T_C_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_C_THRESHOLD_MASK) | (((uint32_t)val) << CRE_EDGE_T_C_THRESHOLD_SHIFT))

//====================================================================
//Register: MAGENTA EDGE STRENGTH THRESHOLD (EDGE_T_M)
/** \brief Threshold for magnitude of magenta edge that results in output OT bit getting set.*/
//====================================================================

#define  CRE_EDGE_T_M_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_T_M_RESERVED1_SHIFT 12
#define  CRE_EDGE_T_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_M_RESERVED1_MASK) >> CRE_EDGE_T_M_RESERVED1_SHIFT)
#define  CRE_EDGE_T_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_T_M_RESERVED1_SHIFT))

#define  CRE_EDGE_T_M_THRESHOLD_MASK 0xfff
#define  CRE_EDGE_T_M_THRESHOLD_SHIFT 0
#define  CRE_EDGE_T_M_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_M_THRESHOLD_MASK) >> CRE_EDGE_T_M_THRESHOLD_SHIFT)
#define  CRE_EDGE_T_M_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_M_THRESHOLD_MASK) | (((uint32_t)val) << CRE_EDGE_T_M_THRESHOLD_SHIFT))

//====================================================================
//Register: YELLOW EDGE STRENGTH THRESHOLD (EDGE_T_Y)
/** \brief Threshold for magnitude of yellow edge that results in output OT bit getting set.*/
//====================================================================

#define  CRE_EDGE_T_Y_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_T_Y_RESERVED1_SHIFT 12
#define  CRE_EDGE_T_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_Y_RESERVED1_MASK) >> CRE_EDGE_T_Y_RESERVED1_SHIFT)
#define  CRE_EDGE_T_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_T_Y_RESERVED1_SHIFT))

#define  CRE_EDGE_T_Y_THRESHOLD_MASK 0xfff
#define  CRE_EDGE_T_Y_THRESHOLD_SHIFT 0
#define  CRE_EDGE_T_Y_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_Y_THRESHOLD_MASK) >> CRE_EDGE_T_Y_THRESHOLD_SHIFT)
#define  CRE_EDGE_T_Y_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_Y_THRESHOLD_MASK) | (((uint32_t)val) << CRE_EDGE_T_Y_THRESHOLD_SHIFT))

//====================================================================
//Register: BLACK EDGE STRENGTH THRESHOLD (EDGE_T_K)
/** \brief Threshold for magnitude of black edge that results in output OT bit getting set.*/
//====================================================================

#define  CRE_EDGE_T_K_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_T_K_RESERVED1_SHIFT 12
#define  CRE_EDGE_T_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_K_RESERVED1_MASK) >> CRE_EDGE_T_K_RESERVED1_SHIFT)
#define  CRE_EDGE_T_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_T_K_RESERVED1_SHIFT))

#define  CRE_EDGE_T_K_THRESHOLD_MASK 0xfff
#define  CRE_EDGE_T_K_THRESHOLD_SHIFT 0
#define  CRE_EDGE_T_K_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_EDGE_T_K_THRESHOLD_MASK) >> CRE_EDGE_T_K_THRESHOLD_SHIFT)
#define  CRE_EDGE_T_K_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_T_K_THRESHOLD_MASK) | (((uint32_t)val) << CRE_EDGE_T_K_THRESHOLD_SHIFT))

//====================================================================
//Register: CYAN EDGE STRENGTH SCALE FACTOR (EDGE_SCALE_C)
/** \brief Scale Factor for CYAN edge strength*/
//====================================================================

#define  CRE_EDGE_SCALE_C_RESERVED1_MASK 0xffffff00
#define  CRE_EDGE_SCALE_C_RESERVED1_SHIFT 8
#define  CRE_EDGE_SCALE_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_C_RESERVED1_MASK) >> CRE_EDGE_SCALE_C_RESERVED1_SHIFT)
#define  CRE_EDGE_SCALE_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_C_RESERVED1_SHIFT))

#define  CRE_EDGE_SCALE_C_SCALE_MASK 0xff
#define  CRE_EDGE_SCALE_C_SCALE_SHIFT 0
#define  CRE_EDGE_SCALE_C_SCALE_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_C_SCALE_MASK) >> CRE_EDGE_SCALE_C_SCALE_SHIFT)
#define  CRE_EDGE_SCALE_C_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_C_SCALE_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_C_SCALE_SHIFT))

//====================================================================
//Register: MAGENTA EDGE STRENGTH SCALE FACTOR (EDGE_SCALE_M)
/** \brief Scale Factor for MAGENTA edge strength*/
//====================================================================

#define  CRE_EDGE_SCALE_M_RESERVED1_MASK 0xffffff00
#define  CRE_EDGE_SCALE_M_RESERVED1_SHIFT 8
#define  CRE_EDGE_SCALE_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_M_RESERVED1_MASK) >> CRE_EDGE_SCALE_M_RESERVED1_SHIFT)
#define  CRE_EDGE_SCALE_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_M_RESERVED1_SHIFT))

#define  CRE_EDGE_SCALE_M_SCALE_MASK 0xff
#define  CRE_EDGE_SCALE_M_SCALE_SHIFT 0
#define  CRE_EDGE_SCALE_M_SCALE_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_M_SCALE_MASK) >> CRE_EDGE_SCALE_M_SCALE_SHIFT)
#define  CRE_EDGE_SCALE_M_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_M_SCALE_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_M_SCALE_SHIFT))

//====================================================================
//Register: YELLOW EDGE STRENGTH SCALE FACTOR (EDGE_SCALE_Y)
/** \brief Scale Factor for YELLOW edge strength*/
//====================================================================

#define  CRE_EDGE_SCALE_Y_RESERVED1_MASK 0xffffff00
#define  CRE_EDGE_SCALE_Y_RESERVED1_SHIFT 8
#define  CRE_EDGE_SCALE_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_Y_RESERVED1_MASK) >> CRE_EDGE_SCALE_Y_RESERVED1_SHIFT)
#define  CRE_EDGE_SCALE_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_Y_RESERVED1_SHIFT))

#define  CRE_EDGE_SCALE_Y_SCALE_MASK 0xff
#define  CRE_EDGE_SCALE_Y_SCALE_SHIFT 0
#define  CRE_EDGE_SCALE_Y_SCALE_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_Y_SCALE_MASK) >> CRE_EDGE_SCALE_Y_SCALE_SHIFT)
#define  CRE_EDGE_SCALE_Y_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_Y_SCALE_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_Y_SCALE_SHIFT))

//====================================================================
//Register: BLACK EDGE STRENGTH SCALE FACTOR (EDGE_SCALE_K)
/** \brief Scale Factor for BLACK edge strength*/
//====================================================================

#define  CRE_EDGE_SCALE_K_RESERVED1_MASK 0xffffff00
#define  CRE_EDGE_SCALE_K_RESERVED1_SHIFT 8
#define  CRE_EDGE_SCALE_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_K_RESERVED1_MASK) >> CRE_EDGE_SCALE_K_RESERVED1_SHIFT)
#define  CRE_EDGE_SCALE_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_K_RESERVED1_SHIFT))

#define  CRE_EDGE_SCALE_K_SCALE_MASK 0xff
#define  CRE_EDGE_SCALE_K_SCALE_SHIFT 0
#define  CRE_EDGE_SCALE_K_SCALE_MASK_SHIFT(reg) (((reg) & CRE_EDGE_SCALE_K_SCALE_MASK) >> CRE_EDGE_SCALE_K_SCALE_SHIFT)
#define  CRE_EDGE_SCALE_K_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_SCALE_K_SCALE_MASK) | (((uint32_t)val) << CRE_EDGE_SCALE_K_SCALE_SHIFT))

//====================================================================
//Register: CYAN EDGE STRENGTH OFFSET (EDGE_OFFSET_C)
/** \brief Offset for CYAN edge strength calculation*/
//====================================================================

#define  CRE_EDGE_OFFSET_C_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_OFFSET_C_RESERVED1_SHIFT 12
#define  CRE_EDGE_OFFSET_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_C_RESERVED1_MASK) >> CRE_EDGE_OFFSET_C_RESERVED1_SHIFT)
#define  CRE_EDGE_OFFSET_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_C_RESERVED1_SHIFT))

#define  CRE_EDGE_OFFSET_C_OFFSET_MASK 0xfff
#define  CRE_EDGE_OFFSET_C_OFFSET_SHIFT 0
#define  CRE_EDGE_OFFSET_C_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_C_OFFSET_MASK) >> CRE_EDGE_OFFSET_C_OFFSET_SHIFT)
#define  CRE_EDGE_OFFSET_C_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_C_OFFSET_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_C_OFFSET_SHIFT))

//====================================================================
//Register: MAGENTA EDGE STRENGTH OFFSET (EDGE_OFFSET_M)
/** \brief Offset for MAGENTA edge strength calculation*/
//====================================================================

#define  CRE_EDGE_OFFSET_M_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_OFFSET_M_RESERVED1_SHIFT 12
#define  CRE_EDGE_OFFSET_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_M_RESERVED1_MASK) >> CRE_EDGE_OFFSET_M_RESERVED1_SHIFT)
#define  CRE_EDGE_OFFSET_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_M_RESERVED1_SHIFT))

#define  CRE_EDGE_OFFSET_M_OFFSET_MASK 0xfff
#define  CRE_EDGE_OFFSET_M_OFFSET_SHIFT 0
#define  CRE_EDGE_OFFSET_M_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_M_OFFSET_MASK) >> CRE_EDGE_OFFSET_M_OFFSET_SHIFT)
#define  CRE_EDGE_OFFSET_M_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_M_OFFSET_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_M_OFFSET_SHIFT))

//====================================================================
//Register: YELLOW EDGE STRENGTH OFFSET (EDGE_OFFSET_Y)
/** \brief Offset for YELLOW edge strength calculation*/
//====================================================================

#define  CRE_EDGE_OFFSET_Y_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_OFFSET_Y_RESERVED1_SHIFT 12
#define  CRE_EDGE_OFFSET_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_Y_RESERVED1_MASK) >> CRE_EDGE_OFFSET_Y_RESERVED1_SHIFT)
#define  CRE_EDGE_OFFSET_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_Y_RESERVED1_SHIFT))

#define  CRE_EDGE_OFFSET_Y_OFFSET_MASK 0xfff
#define  CRE_EDGE_OFFSET_Y_OFFSET_SHIFT 0
#define  CRE_EDGE_OFFSET_Y_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_Y_OFFSET_MASK) >> CRE_EDGE_OFFSET_Y_OFFSET_SHIFT)
#define  CRE_EDGE_OFFSET_Y_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_Y_OFFSET_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_Y_OFFSET_SHIFT))

//====================================================================
//Register: BLACK EDGE STRENGTH OFFSET (EDGE_OFFSET_K)
/** \brief Offset for BLACK edge strength calculation*/
//====================================================================

#define  CRE_EDGE_OFFSET_K_RESERVED1_MASK 0xfffff000
#define  CRE_EDGE_OFFSET_K_RESERVED1_SHIFT 12
#define  CRE_EDGE_OFFSET_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_K_RESERVED1_MASK) >> CRE_EDGE_OFFSET_K_RESERVED1_SHIFT)
#define  CRE_EDGE_OFFSET_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_K_RESERVED1_SHIFT))

#define  CRE_EDGE_OFFSET_K_OFFSET_MASK 0xfff
#define  CRE_EDGE_OFFSET_K_OFFSET_SHIFT 0
#define  CRE_EDGE_OFFSET_K_OFFSET_MASK_SHIFT(reg) (((reg) & CRE_EDGE_OFFSET_K_OFFSET_MASK) >> CRE_EDGE_OFFSET_K_OFFSET_SHIFT)
#define  CRE_EDGE_OFFSET_K_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~CRE_EDGE_OFFSET_K_OFFSET_MASK) | (((uint32_t)val) << CRE_EDGE_OFFSET_K_OFFSET_SHIFT))

//====================================================================
//Register: CORING CONTROL REG (CORE_CNTRL)
/** \brief Control for Coring functions*/
//====================================================================

#define  CRE_CORE_CNTRL_ENABLE_MASK 0x80000000
#define  CRE_CORE_CNTRL_ENABLE_SHIFT 31
#define  CRE_CORE_CNTRL_ENABLE_MASK_SHIFT(reg) (((reg) & CRE_CORE_CNTRL_ENABLE_MASK) >> CRE_CORE_CNTRL_ENABLE_SHIFT)
#define  CRE_CORE_CNTRL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_CNTRL_ENABLE_MASK) | (((uint32_t)val) << CRE_CORE_CNTRL_ENABLE_SHIFT))

#define  CRE_CORE_CNTRL_RESERVED1_MASK 0x7ffffc00
#define  CRE_CORE_CNTRL_RESERVED1_SHIFT 10
#define  CRE_CORE_CNTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_CNTRL_RESERVED1_MASK) >> CRE_CORE_CNTRL_RESERVED1_SHIFT)
#define  CRE_CORE_CNTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_CNTRL_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_CNTRL_RESERVED1_SHIFT))

#define  CRE_CORE_CNTRL_OTMODE_MASK 0x300
#define  CRE_CORE_CNTRL_OTMODE_SHIFT 8
#define  CRE_CORE_CNTRL_OTMODE_MASK_SHIFT(reg) (((reg) & CRE_CORE_CNTRL_OTMODE_MASK) >> CRE_CORE_CNTRL_OTMODE_SHIFT)
#define  CRE_CORE_CNTRL_OTMODE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_CNTRL_OTMODE_MASK) | (((uint32_t)val) << CRE_CORE_CNTRL_OTMODE_SHIFT))

#define  CRE_CORE_CNTRL_RESERVED2_MASK 0xfc
#define  CRE_CORE_CNTRL_RESERVED2_SHIFT 2
#define  CRE_CORE_CNTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & CRE_CORE_CNTRL_RESERVED2_MASK) >> CRE_CORE_CNTRL_RESERVED2_SHIFT)
#define  CRE_CORE_CNTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_CNTRL_RESERVED2_MASK) | (((uint32_t)val) << CRE_CORE_CNTRL_RESERVED2_SHIFT))

#define  CRE_CORE_CNTRL_ENCHKBRD_MASK 0x2
#define  CRE_CORE_CNTRL_ENCHKBRD_SHIFT 1
#define  CRE_CORE_CNTRL_ENCHKBRD_MASK_SHIFT(reg) (((reg) & CRE_CORE_CNTRL_ENCHKBRD_MASK) >> CRE_CORE_CNTRL_ENCHKBRD_SHIFT)
#define  CRE_CORE_CNTRL_ENCHKBRD_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_CNTRL_ENCHKBRD_MASK) | (((uint32_t)val) << CRE_CORE_CNTRL_ENCHKBRD_SHIFT))

#define  CRE_CORE_CNTRL_MODE_MASK 0x1
#define  CRE_CORE_CNTRL_MODE_SHIFT 0
#define  CRE_CORE_CNTRL_MODE_MASK_SHIFT(reg) (((reg) & CRE_CORE_CNTRL_MODE_MASK) >> CRE_CORE_CNTRL_MODE_SHIFT)
#define  CRE_CORE_CNTRL_MODE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_CNTRL_MODE_MASK) | (((uint32_t)val) << CRE_CORE_CNTRL_MODE_SHIFT))

//====================================================================
//Register: CORE CYAN THRESHOLD (CORE_T_C)
/** \brief Threshold for Cyan plane at which constant areas are cored*/
//====================================================================

#define  CRE_CORE_T_C_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_T_C_RESERVED1_SHIFT 8
#define  CRE_CORE_T_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_C_RESERVED1_MASK) >> CRE_CORE_T_C_RESERVED1_SHIFT)
#define  CRE_CORE_T_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_T_C_RESERVED1_SHIFT))

#define  CRE_CORE_T_C_THRESHOLD_MASK 0xff
#define  CRE_CORE_T_C_THRESHOLD_SHIFT 0
#define  CRE_CORE_T_C_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_C_THRESHOLD_MASK) >> CRE_CORE_T_C_THRESHOLD_SHIFT)
#define  CRE_CORE_T_C_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_C_THRESHOLD_MASK) | (((uint32_t)val) << CRE_CORE_T_C_THRESHOLD_SHIFT))

//====================================================================
//Register: CORE MAGENTA THRESHOLD (CORE_T_M)
/** \brief Threshold for Magenta plane at which constant areas are cored*/
//====================================================================

#define  CRE_CORE_T_M_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_T_M_RESERVED1_SHIFT 8
#define  CRE_CORE_T_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_M_RESERVED1_MASK) >> CRE_CORE_T_M_RESERVED1_SHIFT)
#define  CRE_CORE_T_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_T_M_RESERVED1_SHIFT))

#define  CRE_CORE_T_M_THRESHOLD_MASK 0xff
#define  CRE_CORE_T_M_THRESHOLD_SHIFT 0
#define  CRE_CORE_T_M_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_M_THRESHOLD_MASK) >> CRE_CORE_T_M_THRESHOLD_SHIFT)
#define  CRE_CORE_T_M_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_M_THRESHOLD_MASK) | (((uint32_t)val) << CRE_CORE_T_M_THRESHOLD_SHIFT))

//====================================================================
//Register: CORE YELLOW THRESHOLD (CORE_T_Y)
/** \brief Threshold for Yellow plane at which constant areas are cored*/
//====================================================================

#define  CRE_CORE_T_Y_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_T_Y_RESERVED1_SHIFT 8
#define  CRE_CORE_T_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_Y_RESERVED1_MASK) >> CRE_CORE_T_Y_RESERVED1_SHIFT)
#define  CRE_CORE_T_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_T_Y_RESERVED1_SHIFT))

#define  CRE_CORE_T_Y_THRESHOLD_MASK 0xff
#define  CRE_CORE_T_Y_THRESHOLD_SHIFT 0
#define  CRE_CORE_T_Y_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_Y_THRESHOLD_MASK) >> CRE_CORE_T_Y_THRESHOLD_SHIFT)
#define  CRE_CORE_T_Y_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_Y_THRESHOLD_MASK) | (((uint32_t)val) << CRE_CORE_T_Y_THRESHOLD_SHIFT))

//====================================================================
//Register: CORE BLACK THRESHOLD (CORE_T_K)
/** \brief Threshold for Black plane at which constant areas are cored*/
//====================================================================

#define  CRE_CORE_T_K_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_T_K_RESERVED1_SHIFT 8
#define  CRE_CORE_T_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_K_RESERVED1_MASK) >> CRE_CORE_T_K_RESERVED1_SHIFT)
#define  CRE_CORE_T_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_T_K_RESERVED1_SHIFT))

#define  CRE_CORE_T_K_THRESHOLD_MASK 0xff
#define  CRE_CORE_T_K_THRESHOLD_SHIFT 0
#define  CRE_CORE_T_K_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_K_THRESHOLD_MASK) >> CRE_CORE_T_K_THRESHOLD_SHIFT)
#define  CRE_CORE_T_K_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_K_THRESHOLD_MASK) | (((uint32_t)val) << CRE_CORE_T_K_THRESHOLD_SHIFT))

//====================================================================
//Register: CORING CMYK THRESHOLD (CORE_T_CMYK)
/** \brief Sets the total toner threshold for the combined CMYK planes*/
//====================================================================

#define  CRE_CORE_T_CMYK_RESERVED1_MASK 0xfffffc00
#define  CRE_CORE_T_CMYK_RESERVED1_SHIFT 10
#define  CRE_CORE_T_CMYK_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_CMYK_RESERVED1_MASK) >> CRE_CORE_T_CMYK_RESERVED1_SHIFT)
#define  CRE_CORE_T_CMYK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_CMYK_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_T_CMYK_RESERVED1_SHIFT))

#define  CRE_CORE_T_CMYK_THRESHOLD_MASK 0x3ff
#define  CRE_CORE_T_CMYK_THRESHOLD_SHIFT 0
#define  CRE_CORE_T_CMYK_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_CORE_T_CMYK_THRESHOLD_MASK) >> CRE_CORE_T_CMYK_THRESHOLD_SHIFT)
#define  CRE_CORE_T_CMYK_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_T_CMYK_THRESHOLD_MASK) | (((uint32_t)val) << CRE_CORE_T_CMYK_THRESHOLD_SHIFT))

//====================================================================
//Register: CORE CYAN SCALE FACTOR (CORE_SCALE_C)
/** \brief Scale factor for coring of Cyan*/
//====================================================================

#define  CRE_CORE_SCALE_C_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_SCALE_C_RESERVED1_SHIFT 8
#define  CRE_CORE_SCALE_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_C_RESERVED1_MASK) >> CRE_CORE_SCALE_C_RESERVED1_SHIFT)
#define  CRE_CORE_SCALE_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_C_RESERVED1_SHIFT))

#define  CRE_CORE_SCALE_C_SCALE_MASK 0xff
#define  CRE_CORE_SCALE_C_SCALE_SHIFT 0
#define  CRE_CORE_SCALE_C_SCALE_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_C_SCALE_MASK) >> CRE_CORE_SCALE_C_SCALE_SHIFT)
#define  CRE_CORE_SCALE_C_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_C_SCALE_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_C_SCALE_SHIFT))

//====================================================================
//Register: CORE MAGENTA SCALE FACTOR (CORE_SCALE_M)
/** \brief Scale factor for coring of Magenta*/
//====================================================================

#define  CRE_CORE_SCALE_M_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_SCALE_M_RESERVED1_SHIFT 8
#define  CRE_CORE_SCALE_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_M_RESERVED1_MASK) >> CRE_CORE_SCALE_M_RESERVED1_SHIFT)
#define  CRE_CORE_SCALE_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_M_RESERVED1_SHIFT))

#define  CRE_CORE_SCALE_M_SCALE_MASK 0xff
#define  CRE_CORE_SCALE_M_SCALE_SHIFT 0
#define  CRE_CORE_SCALE_M_SCALE_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_M_SCALE_MASK) >> CRE_CORE_SCALE_M_SCALE_SHIFT)
#define  CRE_CORE_SCALE_M_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_M_SCALE_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_M_SCALE_SHIFT))

//====================================================================
//Register: CORE YELLOW SCALE FACTOR (CORE_SCALE_Y)
/** \brief Scale factor for coring of Yellow*/
//====================================================================

#define  CRE_CORE_SCALE_Y_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_SCALE_Y_RESERVED1_SHIFT 8
#define  CRE_CORE_SCALE_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_Y_RESERVED1_MASK) >> CRE_CORE_SCALE_Y_RESERVED1_SHIFT)
#define  CRE_CORE_SCALE_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_Y_RESERVED1_SHIFT))

#define  CRE_CORE_SCALE_Y_SCALE_MASK 0xff
#define  CRE_CORE_SCALE_Y_SCALE_SHIFT 0
#define  CRE_CORE_SCALE_Y_SCALE_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_Y_SCALE_MASK) >> CRE_CORE_SCALE_Y_SCALE_SHIFT)
#define  CRE_CORE_SCALE_Y_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_Y_SCALE_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_Y_SCALE_SHIFT))

//====================================================================
//Register: CORE BLACK SCALE FACTOR (CORE_SCALE_K)
/** \brief Scale factor for coring of Black*/
//====================================================================

#define  CRE_CORE_SCALE_K_RESERVED1_MASK 0xffffff00
#define  CRE_CORE_SCALE_K_RESERVED1_SHIFT 8
#define  CRE_CORE_SCALE_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_K_RESERVED1_MASK) >> CRE_CORE_SCALE_K_RESERVED1_SHIFT)
#define  CRE_CORE_SCALE_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_K_RESERVED1_SHIFT))

#define  CRE_CORE_SCALE_K_SCALE_MASK 0xff
#define  CRE_CORE_SCALE_K_SCALE_SHIFT 0
#define  CRE_CORE_SCALE_K_SCALE_MASK_SHIFT(reg) (((reg) & CRE_CORE_SCALE_K_SCALE_MASK) >> CRE_CORE_SCALE_K_SCALE_SHIFT)
#define  CRE_CORE_SCALE_K_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_CORE_SCALE_K_SCALE_MASK) | (((uint32_t)val) << CRE_CORE_SCALE_K_SCALE_SHIFT))

//====================================================================
//Register: PILING CONTROL REG (PILE_CNTRL)
/** \brief Control for Piling functions*/
//====================================================================

#define  CRE_PILE_CNTRL_ENABLE_MASK 0x80000000
#define  CRE_PILE_CNTRL_ENABLE_SHIFT 31
#define  CRE_PILE_CNTRL_ENABLE_MASK_SHIFT(reg) (((reg) & CRE_PILE_CNTRL_ENABLE_MASK) >> CRE_PILE_CNTRL_ENABLE_SHIFT)
#define  CRE_PILE_CNTRL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_CNTRL_ENABLE_MASK) | (((uint32_t)val) << CRE_PILE_CNTRL_ENABLE_SHIFT))

#define  CRE_PILE_CNTRL_RESERVED1_MASK 0x7ffffc00
#define  CRE_PILE_CNTRL_RESERVED1_SHIFT 10
#define  CRE_PILE_CNTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_CNTRL_RESERVED1_MASK) >> CRE_PILE_CNTRL_RESERVED1_SHIFT)
#define  CRE_PILE_CNTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_CNTRL_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_CNTRL_RESERVED1_SHIFT))

#define  CRE_PILE_CNTRL_OTMODE_MASK 0x300
#define  CRE_PILE_CNTRL_OTMODE_SHIFT 8
#define  CRE_PILE_CNTRL_OTMODE_MASK_SHIFT(reg) (((reg) & CRE_PILE_CNTRL_OTMODE_MASK) >> CRE_PILE_CNTRL_OTMODE_SHIFT)
#define  CRE_PILE_CNTRL_OTMODE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_CNTRL_OTMODE_MASK) | (((uint32_t)val) << CRE_PILE_CNTRL_OTMODE_SHIFT))

#define  CRE_PILE_CNTRL_RESERVED2_MASK 0xfc
#define  CRE_PILE_CNTRL_RESERVED2_SHIFT 2
#define  CRE_PILE_CNTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & CRE_PILE_CNTRL_RESERVED2_MASK) >> CRE_PILE_CNTRL_RESERVED2_SHIFT)
#define  CRE_PILE_CNTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_CNTRL_RESERVED2_MASK) | (((uint32_t)val) << CRE_PILE_CNTRL_RESERVED2_SHIFT))

#define  CRE_PILE_CNTRL_TEMODE_MASK 0x2
#define  CRE_PILE_CNTRL_TEMODE_SHIFT 1
#define  CRE_PILE_CNTRL_TEMODE_MASK_SHIFT(reg) (((reg) & CRE_PILE_CNTRL_TEMODE_MASK) >> CRE_PILE_CNTRL_TEMODE_SHIFT)
#define  CRE_PILE_CNTRL_TEMODE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_CNTRL_TEMODE_MASK) | (((uint32_t)val) << CRE_PILE_CNTRL_TEMODE_SHIFT))

#define  CRE_PILE_CNTRL_LEMODE_MASK 0x1
#define  CRE_PILE_CNTRL_LEMODE_SHIFT 0
#define  CRE_PILE_CNTRL_LEMODE_MASK_SHIFT(reg) (((reg) & CRE_PILE_CNTRL_LEMODE_MASK) >> CRE_PILE_CNTRL_LEMODE_SHIFT)
#define  CRE_PILE_CNTRL_LEMODE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_CNTRL_LEMODE_MASK) | (((uint32_t)val) << CRE_PILE_CNTRL_LEMODE_SHIFT))

//====================================================================
//Register: PILE EDGE THRESHOLD (PILE_ET)
/** \brief Threshold for total color in trailing edge*/
//====================================================================

#define  CRE_PILE_ET_RESERVED1_MASK 0xfffffc00
#define  CRE_PILE_ET_RESERVED1_SHIFT 10
#define  CRE_PILE_ET_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_ET_RESERVED1_MASK) >> CRE_PILE_ET_RESERVED1_SHIFT)
#define  CRE_PILE_ET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_ET_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_ET_RESERVED1_SHIFT))

#define  CRE_PILE_ET_THRESHOLD_MASK 0x3ff
#define  CRE_PILE_ET_THRESHOLD_SHIFT 0
#define  CRE_PILE_ET_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_PILE_ET_THRESHOLD_MASK) >> CRE_PILE_ET_THRESHOLD_SHIFT)
#define  CRE_PILE_ET_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_ET_THRESHOLD_MASK) | (((uint32_t)val) << CRE_PILE_ET_THRESHOLD_SHIFT))

//====================================================================
//Register: PILE CYAN THRESHOLD (PILE_T_C)
/** \brief Threshold for Cyan plane Piling*/
//====================================================================

#define  CRE_PILE_T_C_RESERVED1_MASK 0xffffff00
#define  CRE_PILE_T_C_RESERVED1_SHIFT 8
#define  CRE_PILE_T_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_C_RESERVED1_MASK) >> CRE_PILE_T_C_RESERVED1_SHIFT)
#define  CRE_PILE_T_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_T_C_RESERVED1_SHIFT))

#define  CRE_PILE_T_C_THRESHOLD_MASK 0xff
#define  CRE_PILE_T_C_THRESHOLD_SHIFT 0
#define  CRE_PILE_T_C_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_C_THRESHOLD_MASK) >> CRE_PILE_T_C_THRESHOLD_SHIFT)
#define  CRE_PILE_T_C_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_C_THRESHOLD_MASK) | (((uint32_t)val) << CRE_PILE_T_C_THRESHOLD_SHIFT))

//====================================================================
//Register: PILE MAGENTA THRESHOLD (PILE_T_M)
/** \brief Threshold for Magenta plane Piling*/
//====================================================================

#define  CRE_PILE_T_M_RESERVED1_MASK 0xffffff00
#define  CRE_PILE_T_M_RESERVED1_SHIFT 8
#define  CRE_PILE_T_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_M_RESERVED1_MASK) >> CRE_PILE_T_M_RESERVED1_SHIFT)
#define  CRE_PILE_T_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_T_M_RESERVED1_SHIFT))

#define  CRE_PILE_T_M_THRESHOLD_MASK 0xff
#define  CRE_PILE_T_M_THRESHOLD_SHIFT 0
#define  CRE_PILE_T_M_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_M_THRESHOLD_MASK) >> CRE_PILE_T_M_THRESHOLD_SHIFT)
#define  CRE_PILE_T_M_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_M_THRESHOLD_MASK) | (((uint32_t)val) << CRE_PILE_T_M_THRESHOLD_SHIFT))

//====================================================================
//Register: PILE YELLOW THRESHOLD (PILE_T_Y)
/** \brief Threshold for Yellow plane Piling*/
//====================================================================

#define  CRE_PILE_T_Y_RESERVED1_MASK 0xffffff00
#define  CRE_PILE_T_Y_RESERVED1_SHIFT 8
#define  CRE_PILE_T_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_Y_RESERVED1_MASK) >> CRE_PILE_T_Y_RESERVED1_SHIFT)
#define  CRE_PILE_T_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_T_Y_RESERVED1_SHIFT))

#define  CRE_PILE_T_Y_THRESHOLD_MASK 0xff
#define  CRE_PILE_T_Y_THRESHOLD_SHIFT 0
#define  CRE_PILE_T_Y_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_Y_THRESHOLD_MASK) >> CRE_PILE_T_Y_THRESHOLD_SHIFT)
#define  CRE_PILE_T_Y_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_Y_THRESHOLD_MASK) | (((uint32_t)val) << CRE_PILE_T_Y_THRESHOLD_SHIFT))

//====================================================================
//Register: PILE BLACK THRESHOLD (PILE_T_K)
/** \brief Threshold for Black plane Piling*/
//====================================================================

#define  CRE_PILE_T_K_RESERVED1_MASK 0xffffff00
#define  CRE_PILE_T_K_RESERVED1_SHIFT 8
#define  CRE_PILE_T_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_K_RESERVED1_MASK) >> CRE_PILE_T_K_RESERVED1_SHIFT)
#define  CRE_PILE_T_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_T_K_RESERVED1_SHIFT))

#define  CRE_PILE_T_K_THRESHOLD_MASK 0xff
#define  CRE_PILE_T_K_THRESHOLD_SHIFT 0
#define  CRE_PILE_T_K_THRESHOLD_MASK_SHIFT(reg) (((reg) & CRE_PILE_T_K_THRESHOLD_MASK) >> CRE_PILE_T_K_THRESHOLD_SHIFT)
#define  CRE_PILE_T_K_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_T_K_THRESHOLD_MASK) | (((uint32_t)val) << CRE_PILE_T_K_THRESHOLD_SHIFT))

//====================================================================
//Register: CYAN LEADING EDGE PILE SCALE (PILE_SCALE_LE_C)
/** \brief Scale factor for Cyan Leading Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_LE_C_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_LE_C_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_LE_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_C_RESERVED1_MASK) >> CRE_PILE_SCALE_LE_C_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_LE_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_C_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_LE_C_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_LE_C_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_LE_C_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_C_SCALE_MASK) >> CRE_PILE_SCALE_LE_C_SCALE_SHIFT)
#define  CRE_PILE_SCALE_LE_C_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_C_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_C_SCALE_SHIFT))

//====================================================================
//Register: MAGENTA LEADING EDGE PILE SCALE  (PILE_SCALE_LE_M)
/** \brief Scale factor for Magenta Leading Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_LE_M_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_LE_M_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_LE_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_M_RESERVED1_MASK) >> CRE_PILE_SCALE_LE_M_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_LE_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_M_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_LE_M_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_LE_M_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_LE_M_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_M_SCALE_MASK) >> CRE_PILE_SCALE_LE_M_SCALE_SHIFT)
#define  CRE_PILE_SCALE_LE_M_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_M_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_M_SCALE_SHIFT))

//====================================================================
//Register: YELLOW LEADING EDGE PILE SCALE (PILE_SCALE_LE_Y)
/** \brief Scale factor for Yellow leading Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_LE_Y_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_LE_Y_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_LE_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_Y_RESERVED1_MASK) >> CRE_PILE_SCALE_LE_Y_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_LE_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_Y_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_LE_Y_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_LE_Y_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_LE_Y_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_Y_SCALE_MASK) >> CRE_PILE_SCALE_LE_Y_SCALE_SHIFT)
#define  CRE_PILE_SCALE_LE_Y_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_Y_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_Y_SCALE_SHIFT))

//====================================================================
//Register: BLACK LEADING EDGE PILE SCALE (PILE_SCALE_LE_K)
/** \brief Scale factor for Black leading Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_LE_K_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_LE_K_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_LE_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_K_RESERVED1_MASK) >> CRE_PILE_SCALE_LE_K_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_LE_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_K_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_LE_K_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_LE_K_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_LE_K_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_LE_K_SCALE_MASK) >> CRE_PILE_SCALE_LE_K_SCALE_SHIFT)
#define  CRE_PILE_SCALE_LE_K_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_LE_K_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_LE_K_SCALE_SHIFT))

//====================================================================
//Register: CYAN TRAILING EDGE PILE SCALE (PILE_SCALE_TE_C)
/** \brief Scale factor for Cyan trailing Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_TE_C_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_TE_C_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_TE_C_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_C_RESERVED1_MASK) >> CRE_PILE_SCALE_TE_C_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_TE_C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_C_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_C_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_TE_C_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_TE_C_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_TE_C_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_C_SCALE_MASK) >> CRE_PILE_SCALE_TE_C_SCALE_SHIFT)
#define  CRE_PILE_SCALE_TE_C_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_C_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_C_SCALE_SHIFT))

//====================================================================
//Register: MAGENTA TRAILING EDGE PILE SCALE  (PILE_SCALE_TE_M)
/** \brief Scale factor for Magenta trailing Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_TE_M_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_TE_M_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_TE_M_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_M_RESERVED1_MASK) >> CRE_PILE_SCALE_TE_M_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_TE_M_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_M_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_M_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_TE_M_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_TE_M_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_TE_M_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_M_SCALE_MASK) >> CRE_PILE_SCALE_TE_M_SCALE_SHIFT)
#define  CRE_PILE_SCALE_TE_M_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_M_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_M_SCALE_SHIFT))

//====================================================================
//Register: YELLOW TRAILING EDGE PILE SCALE (PILE_SCALE_TE_Y)
/** \brief Scale factor for Yellow trailing Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_TE_Y_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_TE_Y_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_TE_Y_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_Y_RESERVED1_MASK) >> CRE_PILE_SCALE_TE_Y_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_TE_Y_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_Y_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_Y_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_TE_Y_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_TE_Y_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_TE_Y_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_Y_SCALE_MASK) >> CRE_PILE_SCALE_TE_Y_SCALE_SHIFT)
#define  CRE_PILE_SCALE_TE_Y_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_Y_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_Y_SCALE_SHIFT))

//====================================================================
//Register: BLACK TRAILING EDGE PILE SCALE (PILE_SCALE_TE_K)
/** \brief Scale factor for Black trailing Edge Pile Reduction*/
//====================================================================

#define  CRE_PILE_SCALE_TE_K_RESERVED1_MASK 0xfffffff0
#define  CRE_PILE_SCALE_TE_K_RESERVED1_SHIFT 4
#define  CRE_PILE_SCALE_TE_K_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_K_RESERVED1_MASK) >> CRE_PILE_SCALE_TE_K_RESERVED1_SHIFT)
#define  CRE_PILE_SCALE_TE_K_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_K_RESERVED1_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_K_RESERVED1_SHIFT))

#define  CRE_PILE_SCALE_TE_K_SCALE_MASK 0xf
#define  CRE_PILE_SCALE_TE_K_SCALE_SHIFT 0
#define  CRE_PILE_SCALE_TE_K_SCALE_MASK_SHIFT(reg) (((reg) & CRE_PILE_SCALE_TE_K_SCALE_MASK) >> CRE_PILE_SCALE_TE_K_SCALE_SHIFT)
#define  CRE_PILE_SCALE_TE_K_SCALE_REPLACE_VAL(reg,val) (((reg) & ~CRE_PILE_SCALE_TE_K_SCALE_MASK) | (((uint32_t)val) << CRE_PILE_SCALE_TE_K_SCALE_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 1 of 40) (Window0)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 1 of 40)*/
//====================================================================

#define  CRE_WINDOW0_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW0_PIXVAL_SHIFT 0
#define  CRE_WINDOW0_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW0_PIXVAL_MASK) >> CRE_WINDOW0_PIXVAL_SHIFT)
#define  CRE_WINDOW0_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW0_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW0_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 2 of 40) (Window1)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 2 of 40)*/
//====================================================================

#define  CRE_WINDOW1_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW1_PIXVAL_SHIFT 0
#define  CRE_WINDOW1_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW1_PIXVAL_MASK) >> CRE_WINDOW1_PIXVAL_SHIFT)
#define  CRE_WINDOW1_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW1_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW1_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 3 of 40) (Window2)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 3 of 40)*/
//====================================================================

#define  CRE_WINDOW2_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW2_PIXVAL_SHIFT 0
#define  CRE_WINDOW2_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW2_PIXVAL_MASK) >> CRE_WINDOW2_PIXVAL_SHIFT)
#define  CRE_WINDOW2_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW2_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW2_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 4 of 40) (Window3)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 4 of 40)*/
//====================================================================

#define  CRE_WINDOW3_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW3_PIXVAL_SHIFT 0
#define  CRE_WINDOW3_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW3_PIXVAL_MASK) >> CRE_WINDOW3_PIXVAL_SHIFT)
#define  CRE_WINDOW3_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW3_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW3_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 5 of 40) (Window4)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 5 of 40)*/
//====================================================================

#define  CRE_WINDOW4_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW4_PIXVAL_SHIFT 0
#define  CRE_WINDOW4_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW4_PIXVAL_MASK) >> CRE_WINDOW4_PIXVAL_SHIFT)
#define  CRE_WINDOW4_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW4_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW4_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 6 of 40) (Window5)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 6 of 40)*/
//====================================================================

#define  CRE_WINDOW5_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW5_PIXVAL_SHIFT 0
#define  CRE_WINDOW5_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW5_PIXVAL_MASK) >> CRE_WINDOW5_PIXVAL_SHIFT)
#define  CRE_WINDOW5_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW5_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW5_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 7 of 40) (Window6)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 7 of 40)*/
//====================================================================

#define  CRE_WINDOW6_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW6_PIXVAL_SHIFT 0
#define  CRE_WINDOW6_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW6_PIXVAL_MASK) >> CRE_WINDOW6_PIXVAL_SHIFT)
#define  CRE_WINDOW6_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW6_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW6_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 8 of 40) (Window7)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 8 of 40)*/
//====================================================================

#define  CRE_WINDOW7_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW7_PIXVAL_SHIFT 0
#define  CRE_WINDOW7_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW7_PIXVAL_MASK) >> CRE_WINDOW7_PIXVAL_SHIFT)
#define  CRE_WINDOW7_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW7_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW7_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 9 of 40) (Window8)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 9 of 40)*/
//====================================================================

#define  CRE_WINDOW8_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW8_PIXVAL_SHIFT 0
#define  CRE_WINDOW8_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW8_PIXVAL_MASK) >> CRE_WINDOW8_PIXVAL_SHIFT)
#define  CRE_WINDOW8_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW8_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW8_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 10 of 40) (Window9)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 10 of 40)*/
//====================================================================

#define  CRE_WINDOW9_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW9_PIXVAL_SHIFT 0
#define  CRE_WINDOW9_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW9_PIXVAL_MASK) >> CRE_WINDOW9_PIXVAL_SHIFT)
#define  CRE_WINDOW9_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW9_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW9_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 11 of 40) (Window10)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 11 of 40)*/
//====================================================================

#define  CRE_WINDOW10_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW10_PIXVAL_SHIFT 0
#define  CRE_WINDOW10_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW10_PIXVAL_MASK) >> CRE_WINDOW10_PIXVAL_SHIFT)
#define  CRE_WINDOW10_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW10_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW10_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 12 of 40) (Window11)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 12 of 40)*/
//====================================================================

#define  CRE_WINDOW11_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW11_PIXVAL_SHIFT 0
#define  CRE_WINDOW11_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW11_PIXVAL_MASK) >> CRE_WINDOW11_PIXVAL_SHIFT)
#define  CRE_WINDOW11_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW11_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW11_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 13 of 40) (Window12)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 13 of 40)*/
//====================================================================

#define  CRE_WINDOW12_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW12_PIXVAL_SHIFT 0
#define  CRE_WINDOW12_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW12_PIXVAL_MASK) >> CRE_WINDOW12_PIXVAL_SHIFT)
#define  CRE_WINDOW12_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW12_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW12_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 14 of 40) (Window13)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 14 of 40)*/
//====================================================================

#define  CRE_WINDOW13_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW13_PIXVAL_SHIFT 0
#define  CRE_WINDOW13_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW13_PIXVAL_MASK) >> CRE_WINDOW13_PIXVAL_SHIFT)
#define  CRE_WINDOW13_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW13_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW13_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 15 of 40) (Window14)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 15 of 40)*/
//====================================================================

#define  CRE_WINDOW14_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW14_PIXVAL_SHIFT 0
#define  CRE_WINDOW14_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW14_PIXVAL_MASK) >> CRE_WINDOW14_PIXVAL_SHIFT)
#define  CRE_WINDOW14_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW14_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW14_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 16 of 40) (Window15)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 16 of 40)*/
//====================================================================

#define  CRE_WINDOW15_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW15_PIXVAL_SHIFT 0
#define  CRE_WINDOW15_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW15_PIXVAL_MASK) >> CRE_WINDOW15_PIXVAL_SHIFT)
#define  CRE_WINDOW15_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW15_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW15_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 17 of 40) (Window16)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 17 of 40)*/
//====================================================================

#define  CRE_WINDOW16_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW16_PIXVAL_SHIFT 0
#define  CRE_WINDOW16_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW16_PIXVAL_MASK) >> CRE_WINDOW16_PIXVAL_SHIFT)
#define  CRE_WINDOW16_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW16_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW16_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 18 of 40) (Window17)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 18 of 40)*/
//====================================================================

#define  CRE_WINDOW17_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW17_PIXVAL_SHIFT 0
#define  CRE_WINDOW17_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW17_PIXVAL_MASK) >> CRE_WINDOW17_PIXVAL_SHIFT)
#define  CRE_WINDOW17_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW17_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW17_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 19 of 40) (Window18)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 19 of 40)*/
//====================================================================

#define  CRE_WINDOW18_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW18_PIXVAL_SHIFT 0
#define  CRE_WINDOW18_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW18_PIXVAL_MASK) >> CRE_WINDOW18_PIXVAL_SHIFT)
#define  CRE_WINDOW18_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW18_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW18_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 20 of 40) (Window19)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 20 of 40)*/
//====================================================================

#define  CRE_WINDOW19_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW19_PIXVAL_SHIFT 0
#define  CRE_WINDOW19_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW19_PIXVAL_MASK) >> CRE_WINDOW19_PIXVAL_SHIFT)
#define  CRE_WINDOW19_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW19_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW19_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 21 of 40) (Window20)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 21 of 40)*/
//====================================================================

#define  CRE_WINDOW20_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW20_PIXVAL_SHIFT 0
#define  CRE_WINDOW20_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW20_PIXVAL_MASK) >> CRE_WINDOW20_PIXVAL_SHIFT)
#define  CRE_WINDOW20_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW20_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW20_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 22 of 40) (Window21)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 22 of 40)*/
//====================================================================

#define  CRE_WINDOW21_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW21_PIXVAL_SHIFT 0
#define  CRE_WINDOW21_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW21_PIXVAL_MASK) >> CRE_WINDOW21_PIXVAL_SHIFT)
#define  CRE_WINDOW21_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW21_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW21_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 23 of 40) (Window22)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 23 of 40)*/
//====================================================================

#define  CRE_WINDOW22_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW22_PIXVAL_SHIFT 0
#define  CRE_WINDOW22_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW22_PIXVAL_MASK) >> CRE_WINDOW22_PIXVAL_SHIFT)
#define  CRE_WINDOW22_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW22_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW22_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 24 of 40) (Window23)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 24 of 40)*/
//====================================================================

#define  CRE_WINDOW23_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW23_PIXVAL_SHIFT 0
#define  CRE_WINDOW23_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW23_PIXVAL_MASK) >> CRE_WINDOW23_PIXVAL_SHIFT)
#define  CRE_WINDOW23_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW23_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW23_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 25 of 40) (Window24)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 25 of 40)*/
//====================================================================

#define  CRE_WINDOW24_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW24_PIXVAL_SHIFT 0
#define  CRE_WINDOW24_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW24_PIXVAL_MASK) >> CRE_WINDOW24_PIXVAL_SHIFT)
#define  CRE_WINDOW24_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW24_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW24_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 26 of 40) (Window25)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 26 of 40)*/
//====================================================================

#define  CRE_WINDOW25_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW25_PIXVAL_SHIFT 0
#define  CRE_WINDOW25_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW25_PIXVAL_MASK) >> CRE_WINDOW25_PIXVAL_SHIFT)
#define  CRE_WINDOW25_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW25_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW25_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 27 of 40) (Window26)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 27 of 40)*/
//====================================================================

#define  CRE_WINDOW26_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW26_PIXVAL_SHIFT 0
#define  CRE_WINDOW26_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW26_PIXVAL_MASK) >> CRE_WINDOW26_PIXVAL_SHIFT)
#define  CRE_WINDOW26_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW26_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW26_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 28 of 40) (Window27)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 28 of 40)*/
//====================================================================

#define  CRE_WINDOW27_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW27_PIXVAL_SHIFT 0
#define  CRE_WINDOW27_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW27_PIXVAL_MASK) >> CRE_WINDOW27_PIXVAL_SHIFT)
#define  CRE_WINDOW27_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW27_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW27_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 29 of 40) (Window28)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 29 of 40)*/
//====================================================================

#define  CRE_WINDOW28_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW28_PIXVAL_SHIFT 0
#define  CRE_WINDOW28_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW28_PIXVAL_MASK) >> CRE_WINDOW28_PIXVAL_SHIFT)
#define  CRE_WINDOW28_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW28_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW28_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 30 of 40) (Window29)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 30 of 40)*/
//====================================================================

#define  CRE_WINDOW29_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW29_PIXVAL_SHIFT 0
#define  CRE_WINDOW29_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW29_PIXVAL_MASK) >> CRE_WINDOW29_PIXVAL_SHIFT)
#define  CRE_WINDOW29_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW29_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW29_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 31 of 40) (Window30)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 31 of 40)*/
//====================================================================

#define  CRE_WINDOW30_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW30_PIXVAL_SHIFT 0
#define  CRE_WINDOW30_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW30_PIXVAL_MASK) >> CRE_WINDOW30_PIXVAL_SHIFT)
#define  CRE_WINDOW30_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW30_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW30_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 32 of 40) (Window31)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 32 of 40)*/
//====================================================================

#define  CRE_WINDOW31_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW31_PIXVAL_SHIFT 0
#define  CRE_WINDOW31_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW31_PIXVAL_MASK) >> CRE_WINDOW31_PIXVAL_SHIFT)
#define  CRE_WINDOW31_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW31_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW31_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 33 of 40) (Window32)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 33 of 40)*/
//====================================================================

#define  CRE_WINDOW32_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW32_PIXVAL_SHIFT 0
#define  CRE_WINDOW32_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW32_PIXVAL_MASK) >> CRE_WINDOW32_PIXVAL_SHIFT)
#define  CRE_WINDOW32_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW32_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW32_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 34 of 40) (Window33)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 34 of 40)*/
//====================================================================

#define  CRE_WINDOW33_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW33_PIXVAL_SHIFT 0
#define  CRE_WINDOW33_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW33_PIXVAL_MASK) >> CRE_WINDOW33_PIXVAL_SHIFT)
#define  CRE_WINDOW33_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW33_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW33_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 35 of 40) (Window34)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 35 of 40)*/
//====================================================================

#define  CRE_WINDOW34_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW34_PIXVAL_SHIFT 0
#define  CRE_WINDOW34_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW34_PIXVAL_MASK) >> CRE_WINDOW34_PIXVAL_SHIFT)
#define  CRE_WINDOW34_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW34_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW34_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 36 of 40) (Window35)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 36 of 40)*/
//====================================================================

#define  CRE_WINDOW35_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW35_PIXVAL_SHIFT 0
#define  CRE_WINDOW35_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW35_PIXVAL_MASK) >> CRE_WINDOW35_PIXVAL_SHIFT)
#define  CRE_WINDOW35_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW35_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW35_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 37 of 40) (Window36)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 37 of 40)*/
//====================================================================

#define  CRE_WINDOW36_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW36_PIXVAL_SHIFT 0
#define  CRE_WINDOW36_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW36_PIXVAL_MASK) >> CRE_WINDOW36_PIXVAL_SHIFT)
#define  CRE_WINDOW36_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW36_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW36_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 38 of 40) (Window37)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 38 of 40)*/
//====================================================================

#define  CRE_WINDOW37_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW37_PIXVAL_SHIFT 0
#define  CRE_WINDOW37_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW37_PIXVAL_MASK) >> CRE_WINDOW37_PIXVAL_SHIFT)
#define  CRE_WINDOW37_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW37_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW37_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 39 of 40) (Window38)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 39 of 40)*/
//====================================================================

#define  CRE_WINDOW38_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW38_PIXVAL_SHIFT 0
#define  CRE_WINDOW38_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW38_PIXVAL_MASK) >> CRE_WINDOW38_PIXVAL_SHIFT)
#define  CRE_WINDOW38_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW38_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW38_PIXVAL_SHIFT))

//====================================================================
//Register: 5x5 window pixels (Instance 40 of 40) (Window39)
/** \brief This group of register is used for saving and restoring the 5x5 convolution window. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 40 of 40)*/
//====================================================================

#define  CRE_WINDOW39_PIXVAL_MASK 0xffffffff
#define  CRE_WINDOW39_PIXVAL_SHIFT 0
#define  CRE_WINDOW39_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINDOW39_PIXVAL_MASK) >> CRE_WINDOW39_PIXVAL_SHIFT)
#define  CRE_WINDOW39_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINDOW39_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINDOW39_PIXVAL_SHIFT))

//====================================================================
//Register: window control storage (Instance 1 of 4) (WinCntrl0)
/** \brief This group of register is used for saving and restoring the window control state machine and status. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 1 of 4)*/
//====================================================================

#define  CRE_WINCNTRL0_PIXVAL_MASK 0xffffffff
#define  CRE_WINCNTRL0_PIXVAL_SHIFT 0
#define  CRE_WINCNTRL0_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINCNTRL0_PIXVAL_MASK) >> CRE_WINCNTRL0_PIXVAL_SHIFT)
#define  CRE_WINCNTRL0_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINCNTRL0_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINCNTRL0_PIXVAL_SHIFT))

//====================================================================
//Register: window control storage (Instance 2 of 4) (WinCntrl1)
/** \brief This group of register is used for saving and restoring the window control state machine and status. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 2 of 4)*/
//====================================================================

#define  CRE_WINCNTRL1_PIXVAL_MASK 0xffffffff
#define  CRE_WINCNTRL1_PIXVAL_SHIFT 0
#define  CRE_WINCNTRL1_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINCNTRL1_PIXVAL_MASK) >> CRE_WINCNTRL1_PIXVAL_SHIFT)
#define  CRE_WINCNTRL1_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINCNTRL1_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINCNTRL1_PIXVAL_SHIFT))

//====================================================================
//Register: window control storage (Instance 3 of 4) (WinCntrl2)
/** \brief This group of register is used for saving and restoring the window control state machine and status. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 3 of 4)*/
//====================================================================

#define  CRE_WINCNTRL2_PIXVAL_MASK 0xffffffff
#define  CRE_WINCNTRL2_PIXVAL_SHIFT 0
#define  CRE_WINCNTRL2_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINCNTRL2_PIXVAL_MASK) >> CRE_WINCNTRL2_PIXVAL_SHIFT)
#define  CRE_WINCNTRL2_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINCNTRL2_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINCNTRL2_PIXVAL_SHIFT))

//====================================================================
//Register: window control storage (Instance 4 of 4) (WinCntrl3)
/** \brief This group of register is used for saving and restoring the window control state machine and status. These registers are update with the window values when cpuAccess is asserted, and written to the window when cpuAccess is deasserted. (Instance 4 of 4)*/
//====================================================================

#define  CRE_WINCNTRL3_PIXVAL_MASK 0xffffffff
#define  CRE_WINCNTRL3_PIXVAL_SHIFT 0
#define  CRE_WINCNTRL3_PIXVAL_MASK_SHIFT(reg) (((reg) & CRE_WINCNTRL3_PIXVAL_MASK) >> CRE_WINCNTRL3_PIXVAL_SHIFT)
#define  CRE_WINCNTRL3_PIXVAL_REPLACE_VAL(reg,val) (((reg) & ~CRE_WINCNTRL3_PIXVAL_MASK) | (((uint32_t)val) << CRE_WINCNTRL3_PIXVAL_SHIFT))

//====================================================================
//Register: Line buffer SRAM Address and control (LBAR)
/** \brief Write Only register used to select address and Read/Write control of Line buffers. The LineBuffer SRAM is currently two 2560x98 SRAMS. The SRAMs will be interleaved here so odd addresses access the ODD port ram and Even address access the even port ram.
  NOTE: Access to LineBuffer SRAMs via the LBAR, LBDW, and LBDR registers is only valid if the cpuAccess bit of the CNTRL register is asserted. Register is ignored unless cpuAccess bit of the CNTRL register is asserted*/
//====================================================================

#define  CRE_LBAR_AUTOINC_MASK 0x80000000
#define  CRE_LBAR_AUTOINC_SHIFT 31
#define  CRE_LBAR_AUTOINC_MASK_SHIFT(reg) (((reg) & CRE_LBAR_AUTOINC_MASK) >> CRE_LBAR_AUTOINC_SHIFT)
#define  CRE_LBAR_AUTOINC_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBAR_AUTOINC_MASK) | (((uint32_t)val) << CRE_LBAR_AUTOINC_SHIFT))

#define  CRE_LBAR_RESERVED1_MASK 0x7fff8000
#define  CRE_LBAR_RESERVED1_SHIFT 15
#define  CRE_LBAR_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_LBAR_RESERVED1_MASK) >> CRE_LBAR_RESERVED1_SHIFT)
#define  CRE_LBAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBAR_RESERVED1_MASK) | (((uint32_t)val) << CRE_LBAR_RESERVED1_SHIFT))

#define  CRE_LBAR_RNW_MASK 0x4000
#define  CRE_LBAR_RNW_SHIFT 14
#define  CRE_LBAR_RNW_MASK_SHIFT(reg) (((reg) & CRE_LBAR_RNW_MASK) >> CRE_LBAR_RNW_SHIFT)
#define  CRE_LBAR_RNW_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBAR_RNW_MASK) | (((uint32_t)val) << CRE_LBAR_RNW_SHIFT))

#define  CRE_LBAR_ADDR_MASK 0x3fff
#define  CRE_LBAR_ADDR_SHIFT 0
#define  CRE_LBAR_ADDR_MASK_SHIFT(reg) (((reg) & CRE_LBAR_ADDR_MASK) >> CRE_LBAR_ADDR_SHIFT)
#define  CRE_LBAR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBAR_ADDR_MASK) | (((uint32_t)val) << CRE_LBAR_ADDR_SHIFT))

//====================================================================
//Register: Line buffer Data Write 3 (LBDW3)
/** \brief Write data for LineBuffer SRAMs.*/
//====================================================================

#define  CRE_LBDW3_RESERVED1_MASK 0xfffffffc
#define  CRE_LBDW3_RESERVED1_SHIFT 2
#define  CRE_LBDW3_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_LBDW3_RESERVED1_MASK) >> CRE_LBDW3_RESERVED1_SHIFT)
#define  CRE_LBDW3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDW3_RESERVED1_MASK) | (((uint32_t)val) << CRE_LBDW3_RESERVED1_SHIFT))

#define  CRE_LBDW3_DATA_MASK 0x3
#define  CRE_LBDW3_DATA_SHIFT 0
#define  CRE_LBDW3_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDW3_DATA_MASK) >> CRE_LBDW3_DATA_SHIFT)
#define  CRE_LBDW3_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDW3_DATA_MASK) | (((uint32_t)val) << CRE_LBDW3_DATA_SHIFT))

//====================================================================
//Register: Write Data (LBDW2)
/** \brief Write data for the LineBuffer SRAMs*/
//====================================================================

#define  CRE_LBDW2_DATA_MASK 0xffffffff
#define  CRE_LBDW2_DATA_SHIFT 0
#define  CRE_LBDW2_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDW2_DATA_MASK) >> CRE_LBDW2_DATA_SHIFT)
#define  CRE_LBDW2_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDW2_DATA_MASK) | (((uint32_t)val) << CRE_LBDW2_DATA_SHIFT))

//====================================================================
//Register: Write Data (LBDW1)
/** \brief Write data for the LineBuffer SRAMs*/
//====================================================================

#define  CRE_LBDW1_DATA_MASK 0xffffffff
#define  CRE_LBDW1_DATA_SHIFT 0
#define  CRE_LBDW1_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDW1_DATA_MASK) >> CRE_LBDW1_DATA_SHIFT)
#define  CRE_LBDW1_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDW1_DATA_MASK) | (((uint32_t)val) << CRE_LBDW1_DATA_SHIFT))

//====================================================================
//Register: Write Data (LBDW0)
/** \brief Write data for the LineBuffer SRAMs*/
//====================================================================

#define  CRE_LBDW0_DATA_MASK 0xffffffff
#define  CRE_LBDW0_DATA_SHIFT 0
#define  CRE_LBDW0_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDW0_DATA_MASK) >> CRE_LBDW0_DATA_SHIFT)
#define  CRE_LBDW0_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDW0_DATA_MASK) | (((uint32_t)val) << CRE_LBDW0_DATA_SHIFT))

//====================================================================
//Register: Line Buffer Data Read 3 (LBDR3)
/** \brief fourth word of read data from LineBuffer SRAM.*/
//====================================================================

#define  CRE_LBDR3_RESERVED1_MASK 0xfffffffc
#define  CRE_LBDR3_RESERVED1_SHIFT 2
#define  CRE_LBDR3_RESERVED1_MASK_SHIFT(reg) (((reg) & CRE_LBDR3_RESERVED1_MASK) >> CRE_LBDR3_RESERVED1_SHIFT)
#define  CRE_LBDR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDR3_RESERVED1_MASK) | (((uint32_t)val) << CRE_LBDR3_RESERVED1_SHIFT))

#define  CRE_LBDR3_DATA_MASK 0x3
#define  CRE_LBDR3_DATA_SHIFT 0
#define  CRE_LBDR3_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDR3_DATA_MASK) >> CRE_LBDR3_DATA_SHIFT)
#define  CRE_LBDR3_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDR3_DATA_MASK) | (((uint32_t)val) << CRE_LBDR3_DATA_SHIFT))

//====================================================================
//Register: Line Buffer Data Read 2 (LBDR2)
/** \brief third word of read data from LineBuffer SRAMs*/
//====================================================================

#define  CRE_LBDR2_DATA_MASK 0xffffffff
#define  CRE_LBDR2_DATA_SHIFT 0
#define  CRE_LBDR2_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDR2_DATA_MASK) >> CRE_LBDR2_DATA_SHIFT)
#define  CRE_LBDR2_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDR2_DATA_MASK) | (((uint32_t)val) << CRE_LBDR2_DATA_SHIFT))

//====================================================================
//Register: Line Buffer Data Read 1 (LBDR1)
/** \brief second word of read data from LineBuffer SRAMs*/
//====================================================================

#define  CRE_LBDR1_DATA_MASK 0xffffffff
#define  CRE_LBDR1_DATA_SHIFT 0
#define  CRE_LBDR1_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDR1_DATA_MASK) >> CRE_LBDR1_DATA_SHIFT)
#define  CRE_LBDR1_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDR1_DATA_MASK) | (((uint32_t)val) << CRE_LBDR1_DATA_SHIFT))

//====================================================================
//Register: Line Buffer Data Read 0 (LBDR0)
/** \brief first word of read data from LineBuffer SRAMs*/
//====================================================================

#define  CRE_LBDR0_DATA_MASK 0xffffffff
#define  CRE_LBDR0_DATA_SHIFT 0
#define  CRE_LBDR0_DATA_MASK_SHIFT(reg) (((reg) & CRE_LBDR0_DATA_MASK) >> CRE_LBDR0_DATA_SHIFT)
#define  CRE_LBDR0_DATA_REPLACE_VAL(reg,val) (((reg) & ~CRE_LBDR0_DATA_MASK) | (((uint32_t)val) << CRE_LBDR0_DATA_SHIFT))

#endif // CRE
