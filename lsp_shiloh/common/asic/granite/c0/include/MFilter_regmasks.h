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
 * \file MFilter_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _MFILTER_REGMASKS_H_
#define _MFILTER_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: MFilter (MFilter)
/** \brief Register Descriptions for the PIE MFilter block*/
//
//====================================================================

//====================================================================
//
//Register File: MFilter POGO registers (POGO_Aligner)
/** \brief Register Descriptions for the PIE POGO block*/
//
//====================================================================

//====================================================================
//Register: Pogo Aligner Configuration Register (PACR)
/** \brief General configuration register for POGO block.*/
//====================================================================

#define  POGO_ALIGNER_PACR_RESERVED1_MASK 0xfc000000
#define  POGO_ALIGNER_PACR_RESERVED1_SHIFT 26
#define  POGO_ALIGNER_PACR_RESERVED1_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PACR_RESERVED1_MASK) >> POGO_ALIGNER_PACR_RESERVED1_SHIFT)
#define  POGO_ALIGNER_PACR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PACR_RESERVED1_MASK) | (((uint32_t)val) << POGO_ALIGNER_PACR_RESERVED1_SHIFT))

#define  POGO_ALIGNER_PACR_P_MASK 0x3ffff00
#define  POGO_ALIGNER_PACR_P_SHIFT 8
#define  POGO_ALIGNER_PACR_P_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PACR_P_MASK) >> POGO_ALIGNER_PACR_P_SHIFT)
#define  POGO_ALIGNER_PACR_P_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PACR_P_MASK) | (((uint32_t)val) << POGO_ALIGNER_PACR_P_SHIFT))

#define  POGO_ALIGNER_PACR_RESERVED2_MASK 0xc0
#define  POGO_ALIGNER_PACR_RESERVED2_SHIFT 6
#define  POGO_ALIGNER_PACR_RESERVED2_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PACR_RESERVED2_MASK) >> POGO_ALIGNER_PACR_RESERVED2_SHIFT)
#define  POGO_ALIGNER_PACR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PACR_RESERVED2_MASK) | (((uint32_t)val) << POGO_ALIGNER_PACR_RESERVED2_SHIFT))

#define  POGO_ALIGNER_PACR_N_MASK 0x3f
#define  POGO_ALIGNER_PACR_N_SHIFT 0
#define  POGO_ALIGNER_PACR_N_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PACR_N_MASK) >> POGO_ALIGNER_PACR_N_SHIFT)
#define  POGO_ALIGNER_PACR_N_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PACR_N_MASK) | (((uint32_t)val) << POGO_ALIGNER_PACR_N_SHIFT))

//====================================================================
//Register: Pogo Aligner Status Register (PASTR)
/** \brief General status register for POGO block.*/
//====================================================================

#define  POGO_ALIGNER_PASTR_P_MASK 0xffffc000
#define  POGO_ALIGNER_PASTR_P_SHIFT 14
#define  POGO_ALIGNER_PASTR_P_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PASTR_P_MASK) >> POGO_ALIGNER_PASTR_P_SHIFT)
#define  POGO_ALIGNER_PASTR_P_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PASTR_P_MASK) | (((uint32_t)val) << POGO_ALIGNER_PASTR_P_SHIFT))

#define  POGO_ALIGNER_PASTR_N_MASK 0x3f00
#define  POGO_ALIGNER_PASTR_N_SHIFT 8
#define  POGO_ALIGNER_PASTR_N_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PASTR_N_MASK) >> POGO_ALIGNER_PASTR_N_SHIFT)
#define  POGO_ALIGNER_PASTR_N_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PASTR_N_MASK) | (((uint32_t)val) << POGO_ALIGNER_PASTR_N_SHIFT))

#define  POGO_ALIGNER_PASTR_RESERVED1_MASK 0xfe
#define  POGO_ALIGNER_PASTR_RESERVED1_SHIFT 1
#define  POGO_ALIGNER_PASTR_RESERVED1_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PASTR_RESERVED1_MASK) >> POGO_ALIGNER_PASTR_RESERVED1_SHIFT)
#define  POGO_ALIGNER_PASTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PASTR_RESERVED1_MASK) | (((uint32_t)val) << POGO_ALIGNER_PASTR_RESERVED1_SHIFT))

#define  POGO_ALIGNER_PASTR_A_MASK 0x1
#define  POGO_ALIGNER_PASTR_A_SHIFT 0
#define  POGO_ALIGNER_PASTR_A_MASK_SHIFT(reg) (((reg) & POGO_ALIGNER_PASTR_A_MASK) >> POGO_ALIGNER_PASTR_A_SHIFT)
#define  POGO_ALIGNER_PASTR_A_REPLACE_VAL(reg,val) (((reg) & ~POGO_ALIGNER_PASTR_A_MASK) | (((uint32_t)val) << POGO_ALIGNER_PASTR_A_SHIFT))

//====================================================================
//
//Register File: MFilter Sculptor registers (Sculptor)
/** \brief Register Descriptions for the PIE Sculptor block*/
//
//====================================================================

//====================================================================
//Register: Activity Index LUT (AILR)
//====================================================================

#define  SCULPTOR_AILR_RESERVED1_MASK 0xffff0000
#define  SCULPTOR_AILR_RESERVED1_SHIFT 16
#define  SCULPTOR_AILR_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_AILR_RESERVED1_MASK) >> SCULPTOR_AILR_RESERVED1_SHIFT)
#define  SCULPTOR_AILR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_AILR_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_AILR_RESERVED1_SHIFT))

#define  SCULPTOR_AILR_Y_MASK 0xff00
#define  SCULPTOR_AILR_Y_SHIFT 8
#define  SCULPTOR_AILR_Y_MASK_SHIFT(reg) (((reg) & SCULPTOR_AILR_Y_MASK) >> SCULPTOR_AILR_Y_SHIFT)
#define  SCULPTOR_AILR_Y_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_AILR_Y_MASK) | (((uint32_t)val) << SCULPTOR_AILR_Y_SHIFT))

#define  SCULPTOR_AILR_C_MASK 0xff
#define  SCULPTOR_AILR_C_SHIFT 0
#define  SCULPTOR_AILR_C_MASK_SHIFT(reg) (((reg) & SCULPTOR_AILR_C_MASK) >> SCULPTOR_AILR_C_SHIFT)
#define  SCULPTOR_AILR_C_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_AILR_C_MASK) | (((uint32_t)val) << SCULPTOR_AILR_C_SHIFT))

//====================================================================
//Register: Sculptor Configuration 1 Register (SCR1)
//====================================================================

#define  SCULPTOR_SCR1_T_MASK 0xff800000
#define  SCULPTOR_SCR1_T_SHIFT 23
#define  SCULPTOR_SCR1_T_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_T_MASK) >> SCULPTOR_SCR1_T_SHIFT)
#define  SCULPTOR_SCR1_T_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_T_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_T_SHIFT))

#define  SCULPTOR_SCR1_RESERVED1_MASK 0x400000
#define  SCULPTOR_SCR1_RESERVED1_SHIFT 22
#define  SCULPTOR_SCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_RESERVED1_MASK) >> SCULPTOR_SCR1_RESERVED1_SHIFT)
#define  SCULPTOR_SCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_RESERVED1_SHIFT))

#define  SCULPTOR_SCR1_N_MASK 0x300000
#define  SCULPTOR_SCR1_N_SHIFT 20
#define  SCULPTOR_SCR1_N_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_N_MASK) >> SCULPTOR_SCR1_N_SHIFT)
#define  SCULPTOR_SCR1_N_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_N_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_N_SHIFT))

#define  SCULPTOR_SCR1_RESERVED2_MASK 0xc0000
#define  SCULPTOR_SCR1_RESERVED2_SHIFT 18
#define  SCULPTOR_SCR1_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_RESERVED2_MASK) >> SCULPTOR_SCR1_RESERVED2_SHIFT)
#define  SCULPTOR_SCR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_RESERVED2_SHIFT))

#define  SCULPTOR_SCR1_P_MASK 0x30000
#define  SCULPTOR_SCR1_P_SHIFT 16
#define  SCULPTOR_SCR1_P_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_P_MASK) >> SCULPTOR_SCR1_P_SHIFT)
#define  SCULPTOR_SCR1_P_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_P_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_P_SHIFT))

#define  SCULPTOR_SCR1_RESERVED3_MASK 0xc000
#define  SCULPTOR_SCR1_RESERVED3_SHIFT 14
#define  SCULPTOR_SCR1_RESERVED3_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_RESERVED3_MASK) >> SCULPTOR_SCR1_RESERVED3_SHIFT)
#define  SCULPTOR_SCR1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_RESERVED3_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_RESERVED3_SHIFT))

#define  SCULPTOR_SCR1_M_MASK 0x3000
#define  SCULPTOR_SCR1_M_SHIFT 12
#define  SCULPTOR_SCR1_M_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_M_MASK) >> SCULPTOR_SCR1_M_SHIFT)
#define  SCULPTOR_SCR1_M_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_M_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_M_SHIFT))

#define  SCULPTOR_SCR1_RESERVED4_MASK 0xc00
#define  SCULPTOR_SCR1_RESERVED4_SHIFT 10
#define  SCULPTOR_SCR1_RESERVED4_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_RESERVED4_MASK) >> SCULPTOR_SCR1_RESERVED4_SHIFT)
#define  SCULPTOR_SCR1_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_RESERVED4_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_RESERVED4_SHIFT))

#define  SCULPTOR_SCR1_S_MASK 0x300
#define  SCULPTOR_SCR1_S_SHIFT 8
#define  SCULPTOR_SCR1_S_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_S_MASK) >> SCULPTOR_SCR1_S_SHIFT)
#define  SCULPTOR_SCR1_S_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_S_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_S_SHIFT))

#define  SCULPTOR_SCR1_RESERVED5_MASK 0xc0
#define  SCULPTOR_SCR1_RESERVED5_SHIFT 6
#define  SCULPTOR_SCR1_RESERVED5_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_RESERVED5_MASK) >> SCULPTOR_SCR1_RESERVED5_SHIFT)
#define  SCULPTOR_SCR1_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_RESERVED5_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_RESERVED5_SHIFT))

#define  SCULPTOR_SCR1_CRCBR_MASK 0x30
#define  SCULPTOR_SCR1_CRCBR_SHIFT 4
#define  SCULPTOR_SCR1_CRCBR_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_CRCBR_MASK) >> SCULPTOR_SCR1_CRCBR_SHIFT)
#define  SCULPTOR_SCR1_CRCBR_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_CRCBR_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_CRCBR_SHIFT))

#define  SCULPTOR_SCR1_RESERVED6_MASK 0xc
#define  SCULPTOR_SCR1_RESERVED6_SHIFT 2
#define  SCULPTOR_SCR1_RESERVED6_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_RESERVED6_MASK) >> SCULPTOR_SCR1_RESERVED6_SHIFT)
#define  SCULPTOR_SCR1_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_RESERVED6_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_RESERVED6_SHIFT))

#define  SCULPTOR_SCR1_Y_MASK 0x3
#define  SCULPTOR_SCR1_Y_SHIFT 0
#define  SCULPTOR_SCR1_Y_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR1_Y_MASK) >> SCULPTOR_SCR1_Y_SHIFT)
#define  SCULPTOR_SCR1_Y_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR1_Y_MASK) | (((uint32_t)val) << SCULPTOR_SCR1_Y_SHIFT))

//====================================================================
//Register: Sculptor Configuration 2 Register (SCR2)
//====================================================================

#define  SCULPTOR_SCR2_RESERVED1_MASK 0xe0000000
#define  SCULPTOR_SCR2_RESERVED1_SHIFT 29
#define  SCULPTOR_SCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED1_MASK) >> SCULPTOR_SCR2_RESERVED1_SHIFT)
#define  SCULPTOR_SCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED1_SHIFT))

#define  SCULPTOR_SCR2_R_MASK 0x10000000
#define  SCULPTOR_SCR2_R_SHIFT 28
#define  SCULPTOR_SCR2_R_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_R_MASK) >> SCULPTOR_SCR2_R_SHIFT)
#define  SCULPTOR_SCR2_R_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_R_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_R_SHIFT))

#define  SCULPTOR_SCR2_RESERVED2_MASK 0xe000000
#define  SCULPTOR_SCR2_RESERVED2_SHIFT 25
#define  SCULPTOR_SCR2_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED2_MASK) >> SCULPTOR_SCR2_RESERVED2_SHIFT)
#define  SCULPTOR_SCR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED2_SHIFT))

#define  SCULPTOR_SCR2_D_MASK 0x1000000
#define  SCULPTOR_SCR2_D_SHIFT 24
#define  SCULPTOR_SCR2_D_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_D_MASK) >> SCULPTOR_SCR2_D_SHIFT)
#define  SCULPTOR_SCR2_D_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_D_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_D_SHIFT))

#define  SCULPTOR_SCR2_RESERVED3_MASK 0xe00000
#define  SCULPTOR_SCR2_RESERVED3_SHIFT 21
#define  SCULPTOR_SCR2_RESERVED3_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED3_MASK) >> SCULPTOR_SCR2_RESERVED3_SHIFT)
#define  SCULPTOR_SCR2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED3_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED3_SHIFT))

#define  SCULPTOR_SCR2_V_MASK 0x100000
#define  SCULPTOR_SCR2_V_SHIFT 20
#define  SCULPTOR_SCR2_V_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_V_MASK) >> SCULPTOR_SCR2_V_SHIFT)
#define  SCULPTOR_SCR2_V_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_V_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_V_SHIFT))

#define  SCULPTOR_SCR2_RESERVED4_MASK 0xe0000
#define  SCULPTOR_SCR2_RESERVED4_SHIFT 17
#define  SCULPTOR_SCR2_RESERVED4_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED4_MASK) >> SCULPTOR_SCR2_RESERVED4_SHIFT)
#define  SCULPTOR_SCR2_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED4_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED4_SHIFT))

#define  SCULPTOR_SCR2_C_MASK 0x10000
#define  SCULPTOR_SCR2_C_SHIFT 16
#define  SCULPTOR_SCR2_C_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_C_MASK) >> SCULPTOR_SCR2_C_SHIFT)
#define  SCULPTOR_SCR2_C_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_C_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_C_SHIFT))

#define  SCULPTOR_SCR2_RESERVED5_MASK 0xe000
#define  SCULPTOR_SCR2_RESERVED5_SHIFT 13
#define  SCULPTOR_SCR2_RESERVED5_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED5_MASK) >> SCULPTOR_SCR2_RESERVED5_SHIFT)
#define  SCULPTOR_SCR2_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED5_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED5_SHIFT))

#define  SCULPTOR_SCR2_M_MASK 0x1000
#define  SCULPTOR_SCR2_M_SHIFT 12
#define  SCULPTOR_SCR2_M_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_M_MASK) >> SCULPTOR_SCR2_M_SHIFT)
#define  SCULPTOR_SCR2_M_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_M_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_M_SHIFT))

#define  SCULPTOR_SCR2_RESERVED6_MASK 0xe00
#define  SCULPTOR_SCR2_RESERVED6_SHIFT 9
#define  SCULPTOR_SCR2_RESERVED6_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED6_MASK) >> SCULPTOR_SCR2_RESERVED6_SHIFT)
#define  SCULPTOR_SCR2_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED6_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED6_SHIFT))

#define  SCULPTOR_SCR2_S_MASK 0x100
#define  SCULPTOR_SCR2_S_SHIFT 8
#define  SCULPTOR_SCR2_S_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_S_MASK) >> SCULPTOR_SCR2_S_SHIFT)
#define  SCULPTOR_SCR2_S_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_S_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_S_SHIFT))

#define  SCULPTOR_SCR2_RESERVED7_MASK 0xe0
#define  SCULPTOR_SCR2_RESERVED7_SHIFT 5
#define  SCULPTOR_SCR2_RESERVED7_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED7_MASK) >> SCULPTOR_SCR2_RESERVED7_SHIFT)
#define  SCULPTOR_SCR2_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED7_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED7_SHIFT))

#define  SCULPTOR_SCR2_E_MASK 0x10
#define  SCULPTOR_SCR2_E_SHIFT 4
#define  SCULPTOR_SCR2_E_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_E_MASK) >> SCULPTOR_SCR2_E_SHIFT)
#define  SCULPTOR_SCR2_E_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_E_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_E_SHIFT))

#define  SCULPTOR_SCR2_RESERVED8_MASK 0xe
#define  SCULPTOR_SCR2_RESERVED8_SHIFT 1
#define  SCULPTOR_SCR2_RESERVED8_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_RESERVED8_MASK) >> SCULPTOR_SCR2_RESERVED8_SHIFT)
#define  SCULPTOR_SCR2_RESERVED8_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_RESERVED8_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_RESERVED8_SHIFT))

#define  SCULPTOR_SCR2_U_MASK 0x1
#define  SCULPTOR_SCR2_U_SHIFT 0
#define  SCULPTOR_SCR2_U_MASK_SHIFT(reg) (((reg) & SCULPTOR_SCR2_U_MASK) >> SCULPTOR_SCR2_U_SHIFT)
#define  SCULPTOR_SCR2_U_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SCR2_U_MASK) | (((uint32_t)val) << SCULPTOR_SCR2_U_SHIFT))

//====================================================================
//Register: Text Enhance Limit 1 Register (TEL1R)
//====================================================================

#define  SCULPTOR_TEL1R_RESERVED1_MASK 0x80000000
#define  SCULPTOR_TEL1R_RESERVED1_SHIFT 31
#define  SCULPTOR_TEL1R_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_RESERVED1_MASK) >> SCULPTOR_TEL1R_RESERVED1_SHIFT)
#define  SCULPTOR_TEL1R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_RESERVED1_SHIFT))

#define  SCULPTOR_TEL1R_D_MASK 0x7f000000
#define  SCULPTOR_TEL1R_D_SHIFT 24
#define  SCULPTOR_TEL1R_D_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_D_MASK) >> SCULPTOR_TEL1R_D_SHIFT)
#define  SCULPTOR_TEL1R_D_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_D_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_D_SHIFT))

#define  SCULPTOR_TEL1R_RESERVED2_MASK 0xc00000
#define  SCULPTOR_TEL1R_RESERVED2_SHIFT 22
#define  SCULPTOR_TEL1R_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_RESERVED2_MASK) >> SCULPTOR_TEL1R_RESERVED2_SHIFT)
#define  SCULPTOR_TEL1R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_RESERVED2_SHIFT))

#define  SCULPTOR_TEL1R_C_MASK 0x3f0000
#define  SCULPTOR_TEL1R_C_SHIFT 16
#define  SCULPTOR_TEL1R_C_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_C_MASK) >> SCULPTOR_TEL1R_C_SHIFT)
#define  SCULPTOR_TEL1R_C_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_C_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_C_SHIFT))

#define  SCULPTOR_TEL1R_RESERVED3_MASK 0xe000
#define  SCULPTOR_TEL1R_RESERVED3_SHIFT 13
#define  SCULPTOR_TEL1R_RESERVED3_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_RESERVED3_MASK) >> SCULPTOR_TEL1R_RESERVED3_SHIFT)
#define  SCULPTOR_TEL1R_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_RESERVED3_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_RESERVED3_SHIFT))

#define  SCULPTOR_TEL1R_B_MASK 0x1f00
#define  SCULPTOR_TEL1R_B_SHIFT 8
#define  SCULPTOR_TEL1R_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_B_MASK) >> SCULPTOR_TEL1R_B_SHIFT)
#define  SCULPTOR_TEL1R_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_B_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_B_SHIFT))

#define  SCULPTOR_TEL1R_RESERVED4_MASK 0xe0
#define  SCULPTOR_TEL1R_RESERVED4_SHIFT 5
#define  SCULPTOR_TEL1R_RESERVED4_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_RESERVED4_MASK) >> SCULPTOR_TEL1R_RESERVED4_SHIFT)
#define  SCULPTOR_TEL1R_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_RESERVED4_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_RESERVED4_SHIFT))

#define  SCULPTOR_TEL1R_A_MASK 0x1f
#define  SCULPTOR_TEL1R_A_SHIFT 0
#define  SCULPTOR_TEL1R_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL1R_A_MASK) >> SCULPTOR_TEL1R_A_SHIFT)
#define  SCULPTOR_TEL1R_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL1R_A_MASK) | (((uint32_t)val) << SCULPTOR_TEL1R_A_SHIFT))

//====================================================================
//Register: Text Enhance Limit 2 Register (TEL2R)
//====================================================================

#define  SCULPTOR_TEL2R_RESERVED1_MASK 0xffffff80
#define  SCULPTOR_TEL2R_RESERVED1_SHIFT 7
#define  SCULPTOR_TEL2R_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL2R_RESERVED1_MASK) >> SCULPTOR_TEL2R_RESERVED1_SHIFT)
#define  SCULPTOR_TEL2R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL2R_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_TEL2R_RESERVED1_SHIFT))

#define  SCULPTOR_TEL2R_A_MASK 0x7f
#define  SCULPTOR_TEL2R_A_SHIFT 0
#define  SCULPTOR_TEL2R_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL2R_A_MASK) >> SCULPTOR_TEL2R_A_SHIFT)
#define  SCULPTOR_TEL2R_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL2R_A_MASK) | (((uint32_t)val) << SCULPTOR_TEL2R_A_SHIFT))

//====================================================================
//Register: Text Enhance Limit 3 Register (TEL3R)
//====================================================================

#define  SCULPTOR_TEL3R_RESERVED1_MASK 0x80000000
#define  SCULPTOR_TEL3R_RESERVED1_SHIFT 31
#define  SCULPTOR_TEL3R_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_RESERVED1_MASK) >> SCULPTOR_TEL3R_RESERVED1_SHIFT)
#define  SCULPTOR_TEL3R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_RESERVED1_SHIFT))

#define  SCULPTOR_TEL3R_D_MASK 0x7f000000
#define  SCULPTOR_TEL3R_D_SHIFT 24
#define  SCULPTOR_TEL3R_D_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_D_MASK) >> SCULPTOR_TEL3R_D_SHIFT)
#define  SCULPTOR_TEL3R_D_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_D_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_D_SHIFT))

#define  SCULPTOR_TEL3R_RESERVED2_MASK 0x800000
#define  SCULPTOR_TEL3R_RESERVED2_SHIFT 23
#define  SCULPTOR_TEL3R_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_RESERVED2_MASK) >> SCULPTOR_TEL3R_RESERVED2_SHIFT)
#define  SCULPTOR_TEL3R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_RESERVED2_SHIFT))

#define  SCULPTOR_TEL3R_C_MASK 0x7f0000
#define  SCULPTOR_TEL3R_C_SHIFT 16
#define  SCULPTOR_TEL3R_C_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_C_MASK) >> SCULPTOR_TEL3R_C_SHIFT)
#define  SCULPTOR_TEL3R_C_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_C_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_C_SHIFT))

#define  SCULPTOR_TEL3R_RESERVED3_MASK 0x8000
#define  SCULPTOR_TEL3R_RESERVED3_SHIFT 15
#define  SCULPTOR_TEL3R_RESERVED3_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_RESERVED3_MASK) >> SCULPTOR_TEL3R_RESERVED3_SHIFT)
#define  SCULPTOR_TEL3R_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_RESERVED3_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_RESERVED3_SHIFT))

#define  SCULPTOR_TEL3R_B_MASK 0x7f00
#define  SCULPTOR_TEL3R_B_SHIFT 8
#define  SCULPTOR_TEL3R_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_B_MASK) >> SCULPTOR_TEL3R_B_SHIFT)
#define  SCULPTOR_TEL3R_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_B_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_B_SHIFT))

#define  SCULPTOR_TEL3R_RESERVED4_MASK 0x80
#define  SCULPTOR_TEL3R_RESERVED4_SHIFT 7
#define  SCULPTOR_TEL3R_RESERVED4_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_RESERVED4_MASK) >> SCULPTOR_TEL3R_RESERVED4_SHIFT)
#define  SCULPTOR_TEL3R_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_RESERVED4_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_RESERVED4_SHIFT))

#define  SCULPTOR_TEL3R_A_MASK 0x7f
#define  SCULPTOR_TEL3R_A_SHIFT 0
#define  SCULPTOR_TEL3R_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_TEL3R_A_MASK) >> SCULPTOR_TEL3R_A_SHIFT)
#define  SCULPTOR_TEL3R_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_TEL3R_A_MASK) | (((uint32_t)val) << SCULPTOR_TEL3R_A_SHIFT))

//====================================================================
//Register: Edge Count (EC)
//====================================================================

#define  SCULPTOR_EC_C_MASK 0xff000000
#define  SCULPTOR_EC_C_SHIFT 24
#define  SCULPTOR_EC_C_MASK_SHIFT(reg) (((reg) & SCULPTOR_EC_C_MASK) >> SCULPTOR_EC_C_SHIFT)
#define  SCULPTOR_EC_C_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_EC_C_MASK) | (((uint32_t)val) << SCULPTOR_EC_C_SHIFT))

#define  SCULPTOR_EC_E_MASK 0xffffff
#define  SCULPTOR_EC_E_SHIFT 0
#define  SCULPTOR_EC_E_MASK_SHIFT(reg) (((reg) & SCULPTOR_EC_E_MASK) >> SCULPTOR_EC_E_SHIFT)
#define  SCULPTOR_EC_E_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_EC_E_MASK) | (((uint32_t)val) << SCULPTOR_EC_E_SHIFT))

//====================================================================
//Register: Edge Count Indexes (ECI)
//====================================================================

#define  SCULPTOR_ECI_RESERVED1_MASK 0xffe00000
#define  SCULPTOR_ECI_RESERVED1_SHIFT 21
#define  SCULPTOR_ECI_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_ECI_RESERVED1_MASK) >> SCULPTOR_ECI_RESERVED1_SHIFT)
#define  SCULPTOR_ECI_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_ECI_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_ECI_RESERVED1_SHIFT))

#define  SCULPTOR_ECI_R_MASK 0x1f0000
#define  SCULPTOR_ECI_R_SHIFT 16
#define  SCULPTOR_ECI_R_MASK_SHIFT(reg) (((reg) & SCULPTOR_ECI_R_MASK) >> SCULPTOR_ECI_R_SHIFT)
#define  SCULPTOR_ECI_R_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_ECI_R_MASK) | (((uint32_t)val) << SCULPTOR_ECI_R_SHIFT))

#define  SCULPTOR_ECI_RESERVED2_MASK 0xe000
#define  SCULPTOR_ECI_RESERVED2_SHIFT 13
#define  SCULPTOR_ECI_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_ECI_RESERVED2_MASK) >> SCULPTOR_ECI_RESERVED2_SHIFT)
#define  SCULPTOR_ECI_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_ECI_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_ECI_RESERVED2_SHIFT))

#define  SCULPTOR_ECI_E_MASK 0x1f00
#define  SCULPTOR_ECI_E_SHIFT 8
#define  SCULPTOR_ECI_E_MASK_SHIFT(reg) (((reg) & SCULPTOR_ECI_E_MASK) >> SCULPTOR_ECI_E_SHIFT)
#define  SCULPTOR_ECI_E_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_ECI_E_MASK) | (((uint32_t)val) << SCULPTOR_ECI_E_SHIFT))

#define  SCULPTOR_ECI_RESERVED3_MASK 0xe0
#define  SCULPTOR_ECI_RESERVED3_SHIFT 5
#define  SCULPTOR_ECI_RESERVED3_MASK_SHIFT(reg) (((reg) & SCULPTOR_ECI_RESERVED3_MASK) >> SCULPTOR_ECI_RESERVED3_SHIFT)
#define  SCULPTOR_ECI_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_ECI_RESERVED3_MASK) | (((uint32_t)val) << SCULPTOR_ECI_RESERVED3_SHIFT))

#define  SCULPTOR_ECI_S_MASK 0x1f
#define  SCULPTOR_ECI_S_SHIFT 0
#define  SCULPTOR_ECI_S_MASK_SHIFT(reg) (((reg) & SCULPTOR_ECI_S_MASK) >> SCULPTOR_ECI_S_SHIFT)
#define  SCULPTOR_ECI_S_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_ECI_S_MASK) | (((uint32_t)val) << SCULPTOR_ECI_S_SHIFT))

//====================================================================
//Register: Y Variable Coefficient00_01 (YVC00_01)
/** \brief The following table is for reference to understand the names of the
coefficients with their position in a 5x9 pixel window.
<table>
<tr><td>Coef00</td><td>Coef01</td></td><td>Coef02</td><td>Coef03</td><td>Coef04</td><td>Coef03</td><td>Coef02</td><td>Coef01</td><td>Coef00</td></td></td></tr>
<tr><td>Coef10</td><td>Coef11</td></td><td>Coef12</td><td>Coef13</td><td>Coef14</td><td>Coef13</td><td>Coef12</td><td>Coef11</td><td>Coef10</td></td></td></tr>
<tr><td>Coef20</td><td>Coef21</td></td><td>Coef22</td><td>Coef23</td><td>Coef24</td><td>Coef23</td><td>Coef22</td><td>Coef21</td><td>Coef20</td></td></td></tr>
<tr><td>Coef10</td><td>Coef11</td></td><td>Coef12</td><td>Coef13</td><td>Coef14</td><td>Coef13</td><td>Coef11</td><td>Coef11</td><td>Coef10</td></td></td></tr>
<tr><td>Coef00</td><td>Coef01</td></td><td>Coef02</td><td>Coef03</td><td>Coef04</td><td>Coef03</td><td>Coef02</td><td>Coef01</td><td>Coef00</td></td></td></tr>
</table>*/
//====================================================================

#define  SCULPTOR_YVC00_01_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_YVC00_01_RESERVED1_SHIFT 25
#define  SCULPTOR_YVC00_01_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC00_01_RESERVED1_MASK) >> SCULPTOR_YVC00_01_RESERVED1_SHIFT)
#define  SCULPTOR_YVC00_01_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC00_01_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC00_01_RESERVED1_SHIFT))

#define  SCULPTOR_YVC00_01_B_MASK 0x1ff0000
#define  SCULPTOR_YVC00_01_B_SHIFT 16
#define  SCULPTOR_YVC00_01_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC00_01_B_MASK) >> SCULPTOR_YVC00_01_B_SHIFT)
#define  SCULPTOR_YVC00_01_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC00_01_B_MASK) | (((uint32_t)val) << SCULPTOR_YVC00_01_B_SHIFT))

#define  SCULPTOR_YVC00_01_RESERVED2_MASK 0xfe00
#define  SCULPTOR_YVC00_01_RESERVED2_SHIFT 9
#define  SCULPTOR_YVC00_01_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC00_01_RESERVED2_MASK) >> SCULPTOR_YVC00_01_RESERVED2_SHIFT)
#define  SCULPTOR_YVC00_01_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC00_01_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC00_01_RESERVED2_SHIFT))

#define  SCULPTOR_YVC00_01_A_MASK 0x1ff
#define  SCULPTOR_YVC00_01_A_SHIFT 0
#define  SCULPTOR_YVC00_01_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC00_01_A_MASK) >> SCULPTOR_YVC00_01_A_SHIFT)
#define  SCULPTOR_YVC00_01_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC00_01_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC00_01_A_SHIFT))

//====================================================================
//Register: Y Variable Coefficient02_03 (YVC02_03)
//====================================================================

#define  SCULPTOR_YVC02_03_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_YVC02_03_RESERVED1_SHIFT 25
#define  SCULPTOR_YVC02_03_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC02_03_RESERVED1_MASK) >> SCULPTOR_YVC02_03_RESERVED1_SHIFT)
#define  SCULPTOR_YVC02_03_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC02_03_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC02_03_RESERVED1_SHIFT))

#define  SCULPTOR_YVC02_03_B_MASK 0x1ff0000
#define  SCULPTOR_YVC02_03_B_SHIFT 16
#define  SCULPTOR_YVC02_03_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC02_03_B_MASK) >> SCULPTOR_YVC02_03_B_SHIFT)
#define  SCULPTOR_YVC02_03_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC02_03_B_MASK) | (((uint32_t)val) << SCULPTOR_YVC02_03_B_SHIFT))

#define  SCULPTOR_YVC02_03_RESERVED2_MASK 0xfe00
#define  SCULPTOR_YVC02_03_RESERVED2_SHIFT 9
#define  SCULPTOR_YVC02_03_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC02_03_RESERVED2_MASK) >> SCULPTOR_YVC02_03_RESERVED2_SHIFT)
#define  SCULPTOR_YVC02_03_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC02_03_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC02_03_RESERVED2_SHIFT))

#define  SCULPTOR_YVC02_03_A_MASK 0x1ff
#define  SCULPTOR_YVC02_03_A_SHIFT 0
#define  SCULPTOR_YVC02_03_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC02_03_A_MASK) >> SCULPTOR_YVC02_03_A_SHIFT)
#define  SCULPTOR_YVC02_03_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC02_03_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC02_03_A_SHIFT))

//====================================================================
//Register: Y Variable Coefficient04_10 (YVC04_10)
//====================================================================

#define  SCULPTOR_YVC04_10_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_YVC04_10_RESERVED1_SHIFT 25
#define  SCULPTOR_YVC04_10_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC04_10_RESERVED1_MASK) >> SCULPTOR_YVC04_10_RESERVED1_SHIFT)
#define  SCULPTOR_YVC04_10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC04_10_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC04_10_RESERVED1_SHIFT))

#define  SCULPTOR_YVC04_10_B_MASK 0x1ff0000
#define  SCULPTOR_YVC04_10_B_SHIFT 16
#define  SCULPTOR_YVC04_10_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC04_10_B_MASK) >> SCULPTOR_YVC04_10_B_SHIFT)
#define  SCULPTOR_YVC04_10_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC04_10_B_MASK) | (((uint32_t)val) << SCULPTOR_YVC04_10_B_SHIFT))

#define  SCULPTOR_YVC04_10_RESERVED2_MASK 0xfe00
#define  SCULPTOR_YVC04_10_RESERVED2_SHIFT 9
#define  SCULPTOR_YVC04_10_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC04_10_RESERVED2_MASK) >> SCULPTOR_YVC04_10_RESERVED2_SHIFT)
#define  SCULPTOR_YVC04_10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC04_10_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC04_10_RESERVED2_SHIFT))

#define  SCULPTOR_YVC04_10_A_MASK 0x1ff
#define  SCULPTOR_YVC04_10_A_SHIFT 0
#define  SCULPTOR_YVC04_10_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC04_10_A_MASK) >> SCULPTOR_YVC04_10_A_SHIFT)
#define  SCULPTOR_YVC04_10_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC04_10_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC04_10_A_SHIFT))

//====================================================================
//Register: Y Variable Coefficient11_12 (YVC11_12)
//====================================================================

#define  SCULPTOR_YVC11_12_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_YVC11_12_RESERVED1_SHIFT 25
#define  SCULPTOR_YVC11_12_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC11_12_RESERVED1_MASK) >> SCULPTOR_YVC11_12_RESERVED1_SHIFT)
#define  SCULPTOR_YVC11_12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC11_12_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC11_12_RESERVED1_SHIFT))

#define  SCULPTOR_YVC11_12_B_MASK 0x1ff0000
#define  SCULPTOR_YVC11_12_B_SHIFT 16
#define  SCULPTOR_YVC11_12_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC11_12_B_MASK) >> SCULPTOR_YVC11_12_B_SHIFT)
#define  SCULPTOR_YVC11_12_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC11_12_B_MASK) | (((uint32_t)val) << SCULPTOR_YVC11_12_B_SHIFT))

#define  SCULPTOR_YVC11_12_RESERVED2_MASK 0xfe00
#define  SCULPTOR_YVC11_12_RESERVED2_SHIFT 9
#define  SCULPTOR_YVC11_12_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC11_12_RESERVED2_MASK) >> SCULPTOR_YVC11_12_RESERVED2_SHIFT)
#define  SCULPTOR_YVC11_12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC11_12_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC11_12_RESERVED2_SHIFT))

#define  SCULPTOR_YVC11_12_A_MASK 0x1ff
#define  SCULPTOR_YVC11_12_A_SHIFT 0
#define  SCULPTOR_YVC11_12_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC11_12_A_MASK) >> SCULPTOR_YVC11_12_A_SHIFT)
#define  SCULPTOR_YVC11_12_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC11_12_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC11_12_A_SHIFT))

//====================================================================
//Register: Y Variable Coefficient13_14 (YVC13_14)
//====================================================================

#define  SCULPTOR_YVC13_14_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_YVC13_14_RESERVED1_SHIFT 25
#define  SCULPTOR_YVC13_14_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC13_14_RESERVED1_MASK) >> SCULPTOR_YVC13_14_RESERVED1_SHIFT)
#define  SCULPTOR_YVC13_14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC13_14_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC13_14_RESERVED1_SHIFT))

#define  SCULPTOR_YVC13_14_B_MASK 0x1ff0000
#define  SCULPTOR_YVC13_14_B_SHIFT 16
#define  SCULPTOR_YVC13_14_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC13_14_B_MASK) >> SCULPTOR_YVC13_14_B_SHIFT)
#define  SCULPTOR_YVC13_14_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC13_14_B_MASK) | (((uint32_t)val) << SCULPTOR_YVC13_14_B_SHIFT))

#define  SCULPTOR_YVC13_14_RESERVED2_MASK 0xfe00
#define  SCULPTOR_YVC13_14_RESERVED2_SHIFT 9
#define  SCULPTOR_YVC13_14_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC13_14_RESERVED2_MASK) >> SCULPTOR_YVC13_14_RESERVED2_SHIFT)
#define  SCULPTOR_YVC13_14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC13_14_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC13_14_RESERVED2_SHIFT))

#define  SCULPTOR_YVC13_14_A_MASK 0x1ff
#define  SCULPTOR_YVC13_14_A_SHIFT 0
#define  SCULPTOR_YVC13_14_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC13_14_A_MASK) >> SCULPTOR_YVC13_14_A_SHIFT)
#define  SCULPTOR_YVC13_14_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC13_14_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC13_14_A_SHIFT))

//====================================================================
//Register: Y Variable Coefficient20_21 (YVC20_21)
//====================================================================

#define  SCULPTOR_YVC20_21_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_YVC20_21_RESERVED1_SHIFT 25
#define  SCULPTOR_YVC20_21_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC20_21_RESERVED1_MASK) >> SCULPTOR_YVC20_21_RESERVED1_SHIFT)
#define  SCULPTOR_YVC20_21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC20_21_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC20_21_RESERVED1_SHIFT))

#define  SCULPTOR_YVC20_21_B_MASK 0x1ff0000
#define  SCULPTOR_YVC20_21_B_SHIFT 16
#define  SCULPTOR_YVC20_21_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC20_21_B_MASK) >> SCULPTOR_YVC20_21_B_SHIFT)
#define  SCULPTOR_YVC20_21_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC20_21_B_MASK) | (((uint32_t)val) << SCULPTOR_YVC20_21_B_SHIFT))

#define  SCULPTOR_YVC20_21_RESERVED2_MASK 0xfe00
#define  SCULPTOR_YVC20_21_RESERVED2_SHIFT 9
#define  SCULPTOR_YVC20_21_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC20_21_RESERVED2_MASK) >> SCULPTOR_YVC20_21_RESERVED2_SHIFT)
#define  SCULPTOR_YVC20_21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC20_21_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC20_21_RESERVED2_SHIFT))

#define  SCULPTOR_YVC20_21_A_MASK 0x1ff
#define  SCULPTOR_YVC20_21_A_SHIFT 0
#define  SCULPTOR_YVC20_21_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC20_21_A_MASK) >> SCULPTOR_YVC20_21_A_SHIFT)
#define  SCULPTOR_YVC20_21_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC20_21_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC20_21_A_SHIFT))

//====================================================================
//Register: Y Variable Coefficient22_23 (YVC22_23)
//====================================================================

#define  SCULPTOR_YVC22_23_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_YVC22_23_RESERVED1_SHIFT 25
#define  SCULPTOR_YVC22_23_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC22_23_RESERVED1_MASK) >> SCULPTOR_YVC22_23_RESERVED1_SHIFT)
#define  SCULPTOR_YVC22_23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC22_23_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC22_23_RESERVED1_SHIFT))

#define  SCULPTOR_YVC22_23_B_MASK 0x1ff0000
#define  SCULPTOR_YVC22_23_B_SHIFT 16
#define  SCULPTOR_YVC22_23_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC22_23_B_MASK) >> SCULPTOR_YVC22_23_B_SHIFT)
#define  SCULPTOR_YVC22_23_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC22_23_B_MASK) | (((uint32_t)val) << SCULPTOR_YVC22_23_B_SHIFT))

#define  SCULPTOR_YVC22_23_RESERVED2_MASK 0xfe00
#define  SCULPTOR_YVC22_23_RESERVED2_SHIFT 9
#define  SCULPTOR_YVC22_23_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC22_23_RESERVED2_MASK) >> SCULPTOR_YVC22_23_RESERVED2_SHIFT)
#define  SCULPTOR_YVC22_23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC22_23_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC22_23_RESERVED2_SHIFT))

#define  SCULPTOR_YVC22_23_A_MASK 0x1ff
#define  SCULPTOR_YVC22_23_A_SHIFT 0
#define  SCULPTOR_YVC22_23_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC22_23_A_MASK) >> SCULPTOR_YVC22_23_A_SHIFT)
#define  SCULPTOR_YVC22_23_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC22_23_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC22_23_A_SHIFT))

//====================================================================
//Register: Y Variable Coefficient24 and Shift (YVC24)
//====================================================================

#define  SCULPTOR_YVC24_RESERVED1_MASK 0xfff00000
#define  SCULPTOR_YVC24_RESERVED1_SHIFT 20
#define  SCULPTOR_YVC24_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC24_RESERVED1_MASK) >> SCULPTOR_YVC24_RESERVED1_SHIFT)
#define  SCULPTOR_YVC24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC24_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_YVC24_RESERVED1_SHIFT))

#define  SCULPTOR_YVC24_S_MASK 0xf0000
#define  SCULPTOR_YVC24_S_SHIFT 16
#define  SCULPTOR_YVC24_S_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC24_S_MASK) >> SCULPTOR_YVC24_S_SHIFT)
#define  SCULPTOR_YVC24_S_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC24_S_MASK) | (((uint32_t)val) << SCULPTOR_YVC24_S_SHIFT))

#define  SCULPTOR_YVC24_RESERVED2_MASK 0xf800
#define  SCULPTOR_YVC24_RESERVED2_SHIFT 11
#define  SCULPTOR_YVC24_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC24_RESERVED2_MASK) >> SCULPTOR_YVC24_RESERVED2_SHIFT)
#define  SCULPTOR_YVC24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC24_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_YVC24_RESERVED2_SHIFT))

#define  SCULPTOR_YVC24_A_MASK 0x7ff
#define  SCULPTOR_YVC24_A_SHIFT 0
#define  SCULPTOR_YVC24_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_YVC24_A_MASK) >> SCULPTOR_YVC24_A_SHIFT)
#define  SCULPTOR_YVC24_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_YVC24_A_MASK) | (((uint32_t)val) << SCULPTOR_YVC24_A_SHIFT))

//====================================================================
//Register: Shift Factor (SF)
//====================================================================

#define  SCULPTOR_SF_RESERVED1_MASK 0xf0000000
#define  SCULPTOR_SF_RESERVED1_SHIFT 28
#define  SCULPTOR_SF_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_SF_RESERVED1_MASK) >> SCULPTOR_SF_RESERVED1_SHIFT)
#define  SCULPTOR_SF_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SF_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_SF_RESERVED1_SHIFT))

#define  SCULPTOR_SF_C_MASK 0xfff0000
#define  SCULPTOR_SF_C_SHIFT 16
#define  SCULPTOR_SF_C_MASK_SHIFT(reg) (((reg) & SCULPTOR_SF_C_MASK) >> SCULPTOR_SF_C_SHIFT)
#define  SCULPTOR_SF_C_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SF_C_MASK) | (((uint32_t)val) << SCULPTOR_SF_C_SHIFT))

#define  SCULPTOR_SF_RESERVED2_MASK 0xf000
#define  SCULPTOR_SF_RESERVED2_SHIFT 12
#define  SCULPTOR_SF_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_SF_RESERVED2_MASK) >> SCULPTOR_SF_RESERVED2_SHIFT)
#define  SCULPTOR_SF_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SF_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_SF_RESERVED2_SHIFT))

#define  SCULPTOR_SF_Y_MASK 0xfff
#define  SCULPTOR_SF_Y_SHIFT 0
#define  SCULPTOR_SF_Y_MASK_SHIFT(reg) (((reg) & SCULPTOR_SF_Y_MASK) >> SCULPTOR_SF_Y_SHIFT)
#define  SCULPTOR_SF_Y_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_SF_Y_MASK) | (((uint32_t)val) << SCULPTOR_SF_Y_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient00_01 (CVC00_01)
//====================================================================

#define  SCULPTOR_CVC00_01_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_CVC00_01_RESERVED1_SHIFT 25
#define  SCULPTOR_CVC00_01_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC00_01_RESERVED1_MASK) >> SCULPTOR_CVC00_01_RESERVED1_SHIFT)
#define  SCULPTOR_CVC00_01_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC00_01_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC00_01_RESERVED1_SHIFT))

#define  SCULPTOR_CVC00_01_B_MASK 0x1ff0000
#define  SCULPTOR_CVC00_01_B_SHIFT 16
#define  SCULPTOR_CVC00_01_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC00_01_B_MASK) >> SCULPTOR_CVC00_01_B_SHIFT)
#define  SCULPTOR_CVC00_01_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC00_01_B_MASK) | (((uint32_t)val) << SCULPTOR_CVC00_01_B_SHIFT))

#define  SCULPTOR_CVC00_01_RESERVED2_MASK 0xfe00
#define  SCULPTOR_CVC00_01_RESERVED2_SHIFT 9
#define  SCULPTOR_CVC00_01_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC00_01_RESERVED2_MASK) >> SCULPTOR_CVC00_01_RESERVED2_SHIFT)
#define  SCULPTOR_CVC00_01_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC00_01_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC00_01_RESERVED2_SHIFT))

#define  SCULPTOR_CVC00_01_A_MASK 0x1ff
#define  SCULPTOR_CVC00_01_A_SHIFT 0
#define  SCULPTOR_CVC00_01_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC00_01_A_MASK) >> SCULPTOR_CVC00_01_A_SHIFT)
#define  SCULPTOR_CVC00_01_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC00_01_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC00_01_A_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient02_03 (CVC02_03)
//====================================================================

#define  SCULPTOR_CVC02_03_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_CVC02_03_RESERVED1_SHIFT 25
#define  SCULPTOR_CVC02_03_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC02_03_RESERVED1_MASK) >> SCULPTOR_CVC02_03_RESERVED1_SHIFT)
#define  SCULPTOR_CVC02_03_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC02_03_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC02_03_RESERVED1_SHIFT))

#define  SCULPTOR_CVC02_03_B_MASK 0x1ff0000
#define  SCULPTOR_CVC02_03_B_SHIFT 16
#define  SCULPTOR_CVC02_03_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC02_03_B_MASK) >> SCULPTOR_CVC02_03_B_SHIFT)
#define  SCULPTOR_CVC02_03_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC02_03_B_MASK) | (((uint32_t)val) << SCULPTOR_CVC02_03_B_SHIFT))

#define  SCULPTOR_CVC02_03_RESERVED2_MASK 0xfe00
#define  SCULPTOR_CVC02_03_RESERVED2_SHIFT 9
#define  SCULPTOR_CVC02_03_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC02_03_RESERVED2_MASK) >> SCULPTOR_CVC02_03_RESERVED2_SHIFT)
#define  SCULPTOR_CVC02_03_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC02_03_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC02_03_RESERVED2_SHIFT))

#define  SCULPTOR_CVC02_03_A_MASK 0x1ff
#define  SCULPTOR_CVC02_03_A_SHIFT 0
#define  SCULPTOR_CVC02_03_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC02_03_A_MASK) >> SCULPTOR_CVC02_03_A_SHIFT)
#define  SCULPTOR_CVC02_03_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC02_03_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC02_03_A_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient04_10 (CVC04_10)
//====================================================================

#define  SCULPTOR_CVC04_10_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_CVC04_10_RESERVED1_SHIFT 25
#define  SCULPTOR_CVC04_10_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC04_10_RESERVED1_MASK) >> SCULPTOR_CVC04_10_RESERVED1_SHIFT)
#define  SCULPTOR_CVC04_10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC04_10_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC04_10_RESERVED1_SHIFT))

#define  SCULPTOR_CVC04_10_B_MASK 0x1ff0000
#define  SCULPTOR_CVC04_10_B_SHIFT 16
#define  SCULPTOR_CVC04_10_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC04_10_B_MASK) >> SCULPTOR_CVC04_10_B_SHIFT)
#define  SCULPTOR_CVC04_10_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC04_10_B_MASK) | (((uint32_t)val) << SCULPTOR_CVC04_10_B_SHIFT))

#define  SCULPTOR_CVC04_10_RESERVED2_MASK 0xfe00
#define  SCULPTOR_CVC04_10_RESERVED2_SHIFT 9
#define  SCULPTOR_CVC04_10_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC04_10_RESERVED2_MASK) >> SCULPTOR_CVC04_10_RESERVED2_SHIFT)
#define  SCULPTOR_CVC04_10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC04_10_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC04_10_RESERVED2_SHIFT))

#define  SCULPTOR_CVC04_10_A_MASK 0x1ff
#define  SCULPTOR_CVC04_10_A_SHIFT 0
#define  SCULPTOR_CVC04_10_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC04_10_A_MASK) >> SCULPTOR_CVC04_10_A_SHIFT)
#define  SCULPTOR_CVC04_10_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC04_10_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC04_10_A_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient11_12 (CVC11_12)
//====================================================================

#define  SCULPTOR_CVC11_12_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_CVC11_12_RESERVED1_SHIFT 25
#define  SCULPTOR_CVC11_12_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC11_12_RESERVED1_MASK) >> SCULPTOR_CVC11_12_RESERVED1_SHIFT)
#define  SCULPTOR_CVC11_12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC11_12_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC11_12_RESERVED1_SHIFT))

#define  SCULPTOR_CVC11_12_B_MASK 0x1ff0000
#define  SCULPTOR_CVC11_12_B_SHIFT 16
#define  SCULPTOR_CVC11_12_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC11_12_B_MASK) >> SCULPTOR_CVC11_12_B_SHIFT)
#define  SCULPTOR_CVC11_12_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC11_12_B_MASK) | (((uint32_t)val) << SCULPTOR_CVC11_12_B_SHIFT))

#define  SCULPTOR_CVC11_12_RESERVED2_MASK 0xfe00
#define  SCULPTOR_CVC11_12_RESERVED2_SHIFT 9
#define  SCULPTOR_CVC11_12_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC11_12_RESERVED2_MASK) >> SCULPTOR_CVC11_12_RESERVED2_SHIFT)
#define  SCULPTOR_CVC11_12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC11_12_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC11_12_RESERVED2_SHIFT))

#define  SCULPTOR_CVC11_12_A_MASK 0x1ff
#define  SCULPTOR_CVC11_12_A_SHIFT 0
#define  SCULPTOR_CVC11_12_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC11_12_A_MASK) >> SCULPTOR_CVC11_12_A_SHIFT)
#define  SCULPTOR_CVC11_12_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC11_12_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC11_12_A_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient13_14 (CVC13_14)
//====================================================================

#define  SCULPTOR_CVC13_14_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_CVC13_14_RESERVED1_SHIFT 25
#define  SCULPTOR_CVC13_14_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC13_14_RESERVED1_MASK) >> SCULPTOR_CVC13_14_RESERVED1_SHIFT)
#define  SCULPTOR_CVC13_14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC13_14_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC13_14_RESERVED1_SHIFT))

#define  SCULPTOR_CVC13_14_B_MASK 0x1ff0000
#define  SCULPTOR_CVC13_14_B_SHIFT 16
#define  SCULPTOR_CVC13_14_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC13_14_B_MASK) >> SCULPTOR_CVC13_14_B_SHIFT)
#define  SCULPTOR_CVC13_14_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC13_14_B_MASK) | (((uint32_t)val) << SCULPTOR_CVC13_14_B_SHIFT))

#define  SCULPTOR_CVC13_14_RESERVED2_MASK 0xfe00
#define  SCULPTOR_CVC13_14_RESERVED2_SHIFT 9
#define  SCULPTOR_CVC13_14_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC13_14_RESERVED2_MASK) >> SCULPTOR_CVC13_14_RESERVED2_SHIFT)
#define  SCULPTOR_CVC13_14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC13_14_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC13_14_RESERVED2_SHIFT))

#define  SCULPTOR_CVC13_14_A_MASK 0x1ff
#define  SCULPTOR_CVC13_14_A_SHIFT 0
#define  SCULPTOR_CVC13_14_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC13_14_A_MASK) >> SCULPTOR_CVC13_14_A_SHIFT)
#define  SCULPTOR_CVC13_14_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC13_14_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC13_14_A_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient20_21 (CVC20_21)
//====================================================================

#define  SCULPTOR_CVC20_21_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_CVC20_21_RESERVED1_SHIFT 25
#define  SCULPTOR_CVC20_21_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC20_21_RESERVED1_MASK) >> SCULPTOR_CVC20_21_RESERVED1_SHIFT)
#define  SCULPTOR_CVC20_21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC20_21_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC20_21_RESERVED1_SHIFT))

#define  SCULPTOR_CVC20_21_B_MASK 0x1ff0000
#define  SCULPTOR_CVC20_21_B_SHIFT 16
#define  SCULPTOR_CVC20_21_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC20_21_B_MASK) >> SCULPTOR_CVC20_21_B_SHIFT)
#define  SCULPTOR_CVC20_21_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC20_21_B_MASK) | (((uint32_t)val) << SCULPTOR_CVC20_21_B_SHIFT))

#define  SCULPTOR_CVC20_21_RESERVED2_MASK 0xfe00
#define  SCULPTOR_CVC20_21_RESERVED2_SHIFT 9
#define  SCULPTOR_CVC20_21_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC20_21_RESERVED2_MASK) >> SCULPTOR_CVC20_21_RESERVED2_SHIFT)
#define  SCULPTOR_CVC20_21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC20_21_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC20_21_RESERVED2_SHIFT))

#define  SCULPTOR_CVC20_21_A_MASK 0x1ff
#define  SCULPTOR_CVC20_21_A_SHIFT 0
#define  SCULPTOR_CVC20_21_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC20_21_A_MASK) >> SCULPTOR_CVC20_21_A_SHIFT)
#define  SCULPTOR_CVC20_21_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC20_21_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC20_21_A_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient22_23 (CVC22_23)
//====================================================================

#define  SCULPTOR_CVC22_23_RESERVED1_MASK 0xfe000000
#define  SCULPTOR_CVC22_23_RESERVED1_SHIFT 25
#define  SCULPTOR_CVC22_23_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC22_23_RESERVED1_MASK) >> SCULPTOR_CVC22_23_RESERVED1_SHIFT)
#define  SCULPTOR_CVC22_23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC22_23_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC22_23_RESERVED1_SHIFT))

#define  SCULPTOR_CVC22_23_B_MASK 0x1ff0000
#define  SCULPTOR_CVC22_23_B_SHIFT 16
#define  SCULPTOR_CVC22_23_B_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC22_23_B_MASK) >> SCULPTOR_CVC22_23_B_SHIFT)
#define  SCULPTOR_CVC22_23_B_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC22_23_B_MASK) | (((uint32_t)val) << SCULPTOR_CVC22_23_B_SHIFT))

#define  SCULPTOR_CVC22_23_RESERVED2_MASK 0xfe00
#define  SCULPTOR_CVC22_23_RESERVED2_SHIFT 9
#define  SCULPTOR_CVC22_23_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC22_23_RESERVED2_MASK) >> SCULPTOR_CVC22_23_RESERVED2_SHIFT)
#define  SCULPTOR_CVC22_23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC22_23_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC22_23_RESERVED2_SHIFT))

#define  SCULPTOR_CVC22_23_A_MASK 0x1ff
#define  SCULPTOR_CVC22_23_A_SHIFT 0
#define  SCULPTOR_CVC22_23_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC22_23_A_MASK) >> SCULPTOR_CVC22_23_A_SHIFT)
#define  SCULPTOR_CVC22_23_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC22_23_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC22_23_A_SHIFT))

//====================================================================
//Register: Chroma Variable Coefficient24 and Shift (CVC24)
//====================================================================

#define  SCULPTOR_CVC24_RESERVED1_MASK 0xfff00000
#define  SCULPTOR_CVC24_RESERVED1_SHIFT 20
#define  SCULPTOR_CVC24_RESERVED1_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC24_RESERVED1_MASK) >> SCULPTOR_CVC24_RESERVED1_SHIFT)
#define  SCULPTOR_CVC24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC24_RESERVED1_MASK) | (((uint32_t)val) << SCULPTOR_CVC24_RESERVED1_SHIFT))

#define  SCULPTOR_CVC24_S_MASK 0xf0000
#define  SCULPTOR_CVC24_S_SHIFT 16
#define  SCULPTOR_CVC24_S_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC24_S_MASK) >> SCULPTOR_CVC24_S_SHIFT)
#define  SCULPTOR_CVC24_S_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC24_S_MASK) | (((uint32_t)val) << SCULPTOR_CVC24_S_SHIFT))

#define  SCULPTOR_CVC24_RESERVED2_MASK 0xf800
#define  SCULPTOR_CVC24_RESERVED2_SHIFT 11
#define  SCULPTOR_CVC24_RESERVED2_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC24_RESERVED2_MASK) >> SCULPTOR_CVC24_RESERVED2_SHIFT)
#define  SCULPTOR_CVC24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC24_RESERVED2_MASK) | (((uint32_t)val) << SCULPTOR_CVC24_RESERVED2_SHIFT))

#define  SCULPTOR_CVC24_A_MASK 0x7ff
#define  SCULPTOR_CVC24_A_SHIFT 0
#define  SCULPTOR_CVC24_A_MASK_SHIFT(reg) (((reg) & SCULPTOR_CVC24_A_MASK) >> SCULPTOR_CVC24_A_SHIFT)
#define  SCULPTOR_CVC24_A_REPLACE_VAL(reg,val) (((reg) & ~SCULPTOR_CVC24_A_MASK) | (((uint32_t)val) << SCULPTOR_CVC24_A_SHIFT))

#endif // MFILTER
