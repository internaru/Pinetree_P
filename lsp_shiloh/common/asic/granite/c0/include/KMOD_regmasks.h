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
 * \file KMOD_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _KMOD_REGMASKS_H_
#define _KMOD_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Black Modification Registers (KMOD)
/** \brief This file defines the registers for the Black Modification Module*/
//
//====================================================================

//====================================================================
//Register: KMOD Control 1 Register (KMDC1R)
//====================================================================

#define  KMOD_KMDC1R_RESERVED1_MASK 0xe0000000
#define  KMOD_KMDC1R_RESERVED1_SHIFT 29
#define  KMOD_KMDC1R_RESERVED1_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_RESERVED1_MASK) >> KMOD_KMDC1R_RESERVED1_SHIFT)
#define  KMOD_KMDC1R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_RESERVED1_MASK) | (((uint32_t)val) << KMOD_KMDC1R_RESERVED1_SHIFT))

#define  KMOD_KMDC1R_RESET_MASK 0x10000000
#define  KMOD_KMDC1R_RESET_SHIFT 28
#define  KMOD_KMDC1R_RESET_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_RESET_MASK) >> KMOD_KMDC1R_RESET_SHIFT)
#define  KMOD_KMDC1R_RESET_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_RESET_MASK) | (((uint32_t)val) << KMOD_KMDC1R_RESET_SHIFT))

#define  KMOD_KMDC1R_RESERVED2_MASK 0x8000000
#define  KMOD_KMDC1R_RESERVED2_SHIFT 27
#define  KMOD_KMDC1R_RESERVED2_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_RESERVED2_MASK) >> KMOD_KMDC1R_RESERVED2_SHIFT)
#define  KMOD_KMDC1R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_RESERVED2_MASK) | (((uint32_t)val) << KMOD_KMDC1R_RESERVED2_SHIFT))

#define  KMOD_KMDC1R_DMRS_MASK 0x7000000
#define  KMOD_KMDC1R_DMRS_SHIFT 24
#define  KMOD_KMDC1R_DMRS_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_DMRS_MASK) >> KMOD_KMDC1R_DMRS_SHIFT)
#define  KMOD_KMDC1R_DMRS_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_DMRS_MASK) | (((uint32_t)val) << KMOD_KMDC1R_DMRS_SHIFT))

#define  KMOD_KMDC1R_RESERVED3_MASK 0x800000
#define  KMOD_KMDC1R_RESERVED3_SHIFT 23
#define  KMOD_KMDC1R_RESERVED3_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_RESERVED3_MASK) >> KMOD_KMDC1R_RESERVED3_SHIFT)
#define  KMOD_KMDC1R_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_RESERVED3_MASK) | (((uint32_t)val) << KMOD_KMDC1R_RESERVED3_SHIFT))

#define  KMOD_KMDC1R_DMCS_MASK 0x700000
#define  KMOD_KMDC1R_DMCS_SHIFT 20
#define  KMOD_KMDC1R_DMCS_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_DMCS_MASK) >> KMOD_KMDC1R_DMCS_SHIFT)
#define  KMOD_KMDC1R_DMCS_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_DMCS_MASK) | (((uint32_t)val) << KMOD_KMDC1R_DMCS_SHIFT))

#define  KMOD_KMDC1R_TRAPEN_MASK 0x80000
#define  KMOD_KMDC1R_TRAPEN_SHIFT 19
#define  KMOD_KMDC1R_TRAPEN_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_TRAPEN_MASK) >> KMOD_KMDC1R_TRAPEN_SHIFT)
#define  KMOD_KMDC1R_TRAPEN_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_TRAPEN_MASK) | (((uint32_t)val) << KMOD_KMDC1R_TRAPEN_SHIFT))

#define  KMOD_KMDC1R_STARTDIR_MASK 0x40000
#define  KMOD_KMDC1R_STARTDIR_SHIFT 18
#define  KMOD_KMDC1R_STARTDIR_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_STARTDIR_MASK) >> KMOD_KMDC1R_STARTDIR_SHIFT)
#define  KMOD_KMDC1R_STARTDIR_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_STARTDIR_MASK) | (((uint32_t)val) << KMOD_KMDC1R_STARTDIR_SHIFT))

#define  KMOD_KMDC1R_SERP_MASK 0x20000
#define  KMOD_KMDC1R_SERP_SHIFT 17
#define  KMOD_KMDC1R_SERP_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_SERP_MASK) >> KMOD_KMDC1R_SERP_SHIFT)
#define  KMOD_KMDC1R_SERP_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_SERP_MASK) | (((uint32_t)val) << KMOD_KMDC1R_SERP_SHIFT))

#define  KMOD_KMDC1R_CPU_MASK 0x10000
#define  KMOD_KMDC1R_CPU_SHIFT 16
#define  KMOD_KMDC1R_CPU_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_CPU_MASK) >> KMOD_KMDC1R_CPU_SHIFT)
#define  KMOD_KMDC1R_CPU_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_CPU_MASK) | (((uint32_t)val) << KMOD_KMDC1R_CPU_SHIFT))

#define  KMOD_KMDC1R_PAD_MASK 0xf000
#define  KMOD_KMDC1R_PAD_SHIFT 12
#define  KMOD_KMDC1R_PAD_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_PAD_MASK) >> KMOD_KMDC1R_PAD_SHIFT)
#define  KMOD_KMDC1R_PAD_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_PAD_MASK) | (((uint32_t)val) << KMOD_KMDC1R_PAD_SHIFT))

#define  KMOD_KMDC1R_CTHRESH_MASK 0xc00
#define  KMOD_KMDC1R_CTHRESH_SHIFT 10
#define  KMOD_KMDC1R_CTHRESH_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_CTHRESH_MASK) >> KMOD_KMDC1R_CTHRESH_SHIFT)
#define  KMOD_KMDC1R_CTHRESH_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_CTHRESH_MASK) | (((uint32_t)val) << KMOD_KMDC1R_CTHRESH_SHIFT))

#define  KMOD_KMDC1R_MTHRESH_MASK 0x300
#define  KMOD_KMDC1R_MTHRESH_SHIFT 8
#define  KMOD_KMDC1R_MTHRESH_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_MTHRESH_MASK) >> KMOD_KMDC1R_MTHRESH_SHIFT)
#define  KMOD_KMDC1R_MTHRESH_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_MTHRESH_MASK) | (((uint32_t)val) << KMOD_KMDC1R_MTHRESH_SHIFT))

#define  KMOD_KMDC1R_KTHRESH_MASK 0xf0
#define  KMOD_KMDC1R_KTHRESH_SHIFT 4
#define  KMOD_KMDC1R_KTHRESH_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_KTHRESH_MASK) >> KMOD_KMDC1R_KTHRESH_SHIFT)
#define  KMOD_KMDC1R_KTHRESH_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_KTHRESH_MASK) | (((uint32_t)val) << KMOD_KMDC1R_KTHRESH_SHIFT))

#define  KMOD_KMDC1R_WINSIZE_MASK 0xc
#define  KMOD_KMDC1R_WINSIZE_SHIFT 2
#define  KMOD_KMDC1R_WINSIZE_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_WINSIZE_MASK) >> KMOD_KMDC1R_WINSIZE_SHIFT)
#define  KMOD_KMDC1R_WINSIZE_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_WINSIZE_MASK) | (((uint32_t)val) << KMOD_KMDC1R_WINSIZE_SHIFT))

#define  KMOD_KMDC1R_BYPASS_MASK 0x2
#define  KMOD_KMDC1R_BYPASS_SHIFT 1
#define  KMOD_KMDC1R_BYPASS_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_BYPASS_MASK) >> KMOD_KMDC1R_BYPASS_SHIFT)
#define  KMOD_KMDC1R_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_BYPASS_MASK) | (((uint32_t)val) << KMOD_KMDC1R_BYPASS_SHIFT))

#define  KMOD_KMDC1R_ENABLE_MASK 0x1
#define  KMOD_KMDC1R_ENABLE_SHIFT 0
#define  KMOD_KMDC1R_ENABLE_MASK_SHIFT(reg) (((reg) & KMOD_KMDC1R_ENABLE_MASK) >> KMOD_KMDC1R_ENABLE_SHIFT)
#define  KMOD_KMDC1R_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC1R_ENABLE_MASK) | (((uint32_t)val) << KMOD_KMDC1R_ENABLE_SHIFT))

//====================================================================
//Register: KMOD Control 2 Register (KMDC2R)
//====================================================================

#define  KMOD_KMDC2R_RESERVED1_MASK 0xfffc0000
#define  KMOD_KMDC2R_RESERVED1_SHIFT 18
#define  KMOD_KMDC2R_RESERVED1_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_RESERVED1_MASK) >> KMOD_KMDC2R_RESERVED1_SHIFT)
#define  KMOD_KMDC2R_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_RESERVED1_MASK) | (((uint32_t)val) << KMOD_KMDC2R_RESERVED1_SHIFT))

#define  KMOD_KMDC2R_KMODE_MASK 0x30000
#define  KMOD_KMDC2R_KMODE_SHIFT 16
#define  KMOD_KMDC2R_KMODE_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_KMODE_MASK) >> KMOD_KMDC2R_KMODE_SHIFT)
#define  KMOD_KMDC2R_KMODE_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_KMODE_MASK) | (((uint32_t)val) << KMOD_KMDC2R_KMODE_SHIFT))

#define  KMOD_KMDC2R_RESERVED2_MASK 0xc000
#define  KMOD_KMDC2R_RESERVED2_SHIFT 14
#define  KMOD_KMDC2R_RESERVED2_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_RESERVED2_MASK) >> KMOD_KMDC2R_RESERVED2_SHIFT)
#define  KMOD_KMDC2R_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_RESERVED2_MASK) | (((uint32_t)val) << KMOD_KMDC2R_RESERVED2_SHIFT))

#define  KMOD_KMDC2R_MMODE_MASK 0x3000
#define  KMOD_KMDC2R_MMODE_SHIFT 12
#define  KMOD_KMDC2R_MMODE_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_MMODE_MASK) >> KMOD_KMDC2R_MMODE_SHIFT)
#define  KMOD_KMDC2R_MMODE_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_MMODE_MASK) | (((uint32_t)val) << KMOD_KMDC2R_MMODE_SHIFT))

#define  KMOD_KMDC2R_RESERVED3_MASK 0xc00
#define  KMOD_KMDC2R_RESERVED3_SHIFT 10
#define  KMOD_KMDC2R_RESERVED3_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_RESERVED3_MASK) >> KMOD_KMDC2R_RESERVED3_SHIFT)
#define  KMOD_KMDC2R_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_RESERVED3_MASK) | (((uint32_t)val) << KMOD_KMDC2R_RESERVED3_SHIFT))

#define  KMOD_KMDC2R_CMODE_MASK 0x300
#define  KMOD_KMDC2R_CMODE_SHIFT 8
#define  KMOD_KMDC2R_CMODE_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_CMODE_MASK) >> KMOD_KMDC2R_CMODE_SHIFT)
#define  KMOD_KMDC2R_CMODE_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_CMODE_MASK) | (((uint32_t)val) << KMOD_KMDC2R_CMODE_SHIFT))

#define  KMOD_KMDC2R_RESERVED4_MASK 0xe0
#define  KMOD_KMDC2R_RESERVED4_SHIFT 5
#define  KMOD_KMDC2R_RESERVED4_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_RESERVED4_MASK) >> KMOD_KMDC2R_RESERVED4_SHIFT)
#define  KMOD_KMDC2R_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_RESERVED4_MASK) | (((uint32_t)val) << KMOD_KMDC2R_RESERVED4_SHIFT))

#define  KMOD_KMDC2R_KDBYPASS_MASK 0x10
#define  KMOD_KMDC2R_KDBYPASS_SHIFT 4
#define  KMOD_KMDC2R_KDBYPASS_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_KDBYPASS_MASK) >> KMOD_KMDC2R_KDBYPASS_SHIFT)
#define  KMOD_KMDC2R_KDBYPASS_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_KDBYPASS_MASK) | (((uint32_t)val) << KMOD_KMDC2R_KDBYPASS_SHIFT))

#define  KMOD_KMDC2R_RESERVED5_MASK 0xe
#define  KMOD_KMDC2R_RESERVED5_SHIFT 1
#define  KMOD_KMDC2R_RESERVED5_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_RESERVED5_MASK) >> KMOD_KMDC2R_RESERVED5_SHIFT)
#define  KMOD_KMDC2R_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_RESERVED5_MASK) | (((uint32_t)val) << KMOD_KMDC2R_RESERVED5_SHIFT))

#define  KMOD_KMDC2R_KFBYPASS_MASK 0x1
#define  KMOD_KMDC2R_KFBYPASS_SHIFT 0
#define  KMOD_KMDC2R_KFBYPASS_MASK_SHIFT(reg) (((reg) & KMOD_KMDC2R_KFBYPASS_MASK) >> KMOD_KMDC2R_KFBYPASS_SHIFT)
#define  KMOD_KMDC2R_KFBYPASS_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDC2R_KFBYPASS_MASK) | (((uint32_t)val) << KMOD_KMDC2R_KFBYPASS_SHIFT))

//====================================================================
//Register: KMOD Line Width Register (KMDLWR)
//====================================================================

#define  KMOD_KMDLWR_RESERVED1_MASK 0xff000000
#define  KMOD_KMDLWR_RESERVED1_SHIFT 24
#define  KMOD_KMDLWR_RESERVED1_MASK_SHIFT(reg) (((reg) & KMOD_KMDLWR_RESERVED1_MASK) >> KMOD_KMDLWR_RESERVED1_SHIFT)
#define  KMOD_KMDLWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDLWR_RESERVED1_MASK) | (((uint32_t)val) << KMOD_KMDLWR_RESERVED1_SHIFT))

#define  KMOD_KMDLWR_LW_MASK 0xffffff
#define  KMOD_KMDLWR_LW_SHIFT 0
#define  KMOD_KMDLWR_LW_MASK_SHIFT(reg) (((reg) & KMOD_KMDLWR_LW_MASK) >> KMOD_KMDLWR_LW_SHIFT)
#define  KMOD_KMDLWR_LW_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDLWR_LW_MASK) | (((uint32_t)val) << KMOD_KMDLWR_LW_SHIFT))

//====================================================================
//Register: KMOD Status Register (KMDSR)
/** \brief This status register contains information needed for saving and restoring the context of the black modification block.*/
//====================================================================

#define  KMOD_KMDSR_RESERVED1_MASK 0xffffff80
#define  KMOD_KMDSR_RESERVED1_SHIFT 7
#define  KMOD_KMDSR_RESERVED1_MASK_SHIFT(reg) (((reg) & KMOD_KMDSR_RESERVED1_MASK) >> KMOD_KMDSR_RESERVED1_SHIFT)
#define  KMOD_KMDSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDSR_RESERVED1_MASK) | (((uint32_t)val) << KMOD_KMDSR_RESERVED1_SHIFT))

#define  KMOD_KMDSR_DMCS_MASK 0x70
#define  KMOD_KMDSR_DMCS_SHIFT 4
#define  KMOD_KMDSR_DMCS_MASK_SHIFT(reg) (((reg) & KMOD_KMDSR_DMCS_MASK) >> KMOD_KMDSR_DMCS_SHIFT)
#define  KMOD_KMDSR_DMCS_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDSR_DMCS_MASK) | (((uint32_t)val) << KMOD_KMDSR_DMCS_SHIFT))

#define  KMOD_KMDSR_RESERVED2_MASK 0x8
#define  KMOD_KMDSR_RESERVED2_SHIFT 3
#define  KMOD_KMDSR_RESERVED2_MASK_SHIFT(reg) (((reg) & KMOD_KMDSR_RESERVED2_MASK) >> KMOD_KMDSR_RESERVED2_SHIFT)
#define  KMOD_KMDSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDSR_RESERVED2_MASK) | (((uint32_t)val) << KMOD_KMDSR_RESERVED2_SHIFT))

#define  KMOD_KMDSR_DMRS_MASK 0x7
#define  KMOD_KMDSR_DMRS_SHIFT 0
#define  KMOD_KMDSR_DMRS_MASK_SHIFT(reg) (((reg) & KMOD_KMDSR_DMRS_MASK) >> KMOD_KMDSR_DMRS_SHIFT)
#define  KMOD_KMDSR_DMRS_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMDSR_DMRS_MASK) | (((uint32_t)val) << KMOD_KMDSR_DMRS_SHIFT))

//====================================================================
//Register: KMOD Pattern Matrix Memory (KMODPMM)
/** \brief This is the Pattern Matrix Memory Access. There are 4 pattern matrices, each is 1 bit wide and 55 entries deep.
         This field is 4 bits wide with each bit assigned to one of the different pattern matrices. Address 0 represents
         the lower right corner of the matrix. The address increments in row order. See the section on pattern matching
         for more details.*/
//====================================================================

#define  KMOD_KMODPMM_RESERVED1_MASK 0xfffffff0
#define  KMOD_KMODPMM_RESERVED1_SHIFT 4
#define  KMOD_KMODPMM_RESERVED1_MASK_SHIFT(reg) (((reg) & KMOD_KMODPMM_RESERVED1_MASK) >> KMOD_KMODPMM_RESERVED1_SHIFT)
#define  KMOD_KMODPMM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODPMM_RESERVED1_MASK) | (((uint32_t)val) << KMOD_KMODPMM_RESERVED1_SHIFT))

#define  KMOD_KMODPMM_PATTERN3_MASK 0x8
#define  KMOD_KMODPMM_PATTERN3_SHIFT 3
#define  KMOD_KMODPMM_PATTERN3_MASK_SHIFT(reg) (((reg) & KMOD_KMODPMM_PATTERN3_MASK) >> KMOD_KMODPMM_PATTERN3_SHIFT)
#define  KMOD_KMODPMM_PATTERN3_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODPMM_PATTERN3_MASK) | (((uint32_t)val) << KMOD_KMODPMM_PATTERN3_SHIFT))

#define  KMOD_KMODPMM_PATTERN2_MASK 0x4
#define  KMOD_KMODPMM_PATTERN2_SHIFT 2
#define  KMOD_KMODPMM_PATTERN2_MASK_SHIFT(reg) (((reg) & KMOD_KMODPMM_PATTERN2_MASK) >> KMOD_KMODPMM_PATTERN2_SHIFT)
#define  KMOD_KMODPMM_PATTERN2_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODPMM_PATTERN2_MASK) | (((uint32_t)val) << KMOD_KMODPMM_PATTERN2_SHIFT))

#define  KMOD_KMODPMM_PATTERN1_MASK 0x2
#define  KMOD_KMODPMM_PATTERN1_SHIFT 1
#define  KMOD_KMODPMM_PATTERN1_MASK_SHIFT(reg) (((reg) & KMOD_KMODPMM_PATTERN1_MASK) >> KMOD_KMODPMM_PATTERN1_SHIFT)
#define  KMOD_KMODPMM_PATTERN1_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODPMM_PATTERN1_MASK) | (((uint32_t)val) << KMOD_KMODPMM_PATTERN1_SHIFT))

#define  KMOD_KMODPMM_PATTERN0_MASK 0x1
#define  KMOD_KMODPMM_PATTERN0_SHIFT 0
#define  KMOD_KMODPMM_PATTERN0_MASK_SHIFT(reg) (((reg) & KMOD_KMODPMM_PATTERN0_MASK) >> KMOD_KMODPMM_PATTERN0_SHIFT)
#define  KMOD_KMODPMM_PATTERN0_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODPMM_PATTERN0_MASK) | (((uint32_t)val) << KMOD_KMODPMM_PATTERN0_SHIFT))

//====================================================================
//Register: KMOD Dither Matrix Memory (KMODDMM)
/** \brief This is the Dither Matrix Memory Access. There are 3 dither matrices, each is 4 bits wide and 64 entries deep.
         This field is 12 bits wide with each nibble assigned to one of the different dither matrices.*/
//====================================================================

#define  KMOD_KMODDMM_RESERVED1_MASK 0xfffff000
#define  KMOD_KMODDMM_RESERVED1_SHIFT 12
#define  KMOD_KMODDMM_RESERVED1_MASK_SHIFT(reg) (((reg) & KMOD_KMODDMM_RESERVED1_MASK) >> KMOD_KMODDMM_RESERVED1_SHIFT)
#define  KMOD_KMODDMM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODDMM_RESERVED1_MASK) | (((uint32_t)val) << KMOD_KMODDMM_RESERVED1_SHIFT))

#define  KMOD_KMODDMM_K_MASK 0xf00
#define  KMOD_KMODDMM_K_SHIFT 8
#define  KMOD_KMODDMM_K_MASK_SHIFT(reg) (((reg) & KMOD_KMODDMM_K_MASK) >> KMOD_KMODDMM_K_SHIFT)
#define  KMOD_KMODDMM_K_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODDMM_K_MASK) | (((uint32_t)val) << KMOD_KMODDMM_K_SHIFT))

#define  KMOD_KMODDMM_C_MASK 0xf0
#define  KMOD_KMODDMM_C_SHIFT 4
#define  KMOD_KMODDMM_C_MASK_SHIFT(reg) (((reg) & KMOD_KMODDMM_C_MASK) >> KMOD_KMODDMM_C_SHIFT)
#define  KMOD_KMODDMM_C_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODDMM_C_MASK) | (((uint32_t)val) << KMOD_KMODDMM_C_SHIFT))

#define  KMOD_KMODDMM_M_MASK 0xf
#define  KMOD_KMODDMM_M_SHIFT 0
#define  KMOD_KMODDMM_M_MASK_SHIFT(reg) (((reg) & KMOD_KMODDMM_M_MASK) >> KMOD_KMODDMM_M_SHIFT)
#define  KMOD_KMODDMM_M_REPLACE_VAL(reg,val) (((reg) & ~KMOD_KMODDMM_M_MASK) | (((uint32_t)val) << KMOD_KMODDMM_M_SHIFT))

#endif // KMOD
