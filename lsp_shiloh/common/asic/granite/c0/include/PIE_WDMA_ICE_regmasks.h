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
 * \file PIE_WDMA_ICE_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIE_WDMA_ICE_REGMASKS_H_
#define _PIE_WDMA_ICE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIE_WDMA (PIE_WDMA_ICE)
/** \brief Register Descriptions for the multi-channel PIE WDMA block*/
//
//====================================================================

//====================================================================
//Register: DMA General Control Register (DGCR)
/** \brief General configuration register for all DMAs in this block.
The values in this register should only be changed when none of the DMA channels are active. Changing these values during operation of the DMA will cause unexpected behavior.*/
//====================================================================

#define  PIE_WDMA_ICE_DGCR_RESERVED1_MASK 0xfffff800
#define  PIE_WDMA_ICE_DGCR_RESERVED1_SHIFT 11
#define  PIE_WDMA_ICE_DGCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_DGCR_RESERVED1_MASK) >> PIE_WDMA_ICE_DGCR_RESERVED1_SHIFT)
#define  PIE_WDMA_ICE_DGCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_DGCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_DGCR_RESERVED1_SHIFT))

#define  PIE_WDMA_ICE_DGCR_RGBMODE_MASK 0x600
#define  PIE_WDMA_ICE_DGCR_RGBMODE_SHIFT 9
#define  PIE_WDMA_ICE_DGCR_RGBMODE_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_DGCR_RGBMODE_MASK) >> PIE_WDMA_ICE_DGCR_RGBMODE_SHIFT)
#define  PIE_WDMA_ICE_DGCR_RGBMODE_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_DGCR_RGBMODE_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_DGCR_RGBMODE_SHIFT))

#define  PIE_WDMA_ICE_DGCR_M_MASK 0x100
#define  PIE_WDMA_ICE_DGCR_M_SHIFT 8
#define  PIE_WDMA_ICE_DGCR_M_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_DGCR_M_MASK) >> PIE_WDMA_ICE_DGCR_M_SHIFT)
#define  PIE_WDMA_ICE_DGCR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_DGCR_M_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_DGCR_M_SHIFT))

#define  PIE_WDMA_ICE_DGCR_D_MASK 0x80
#define  PIE_WDMA_ICE_DGCR_D_SHIFT 7
#define  PIE_WDMA_ICE_DGCR_D_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_DGCR_D_MASK) >> PIE_WDMA_ICE_DGCR_D_SHIFT)
#define  PIE_WDMA_ICE_DGCR_D_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_DGCR_D_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_DGCR_D_SHIFT))

#define  PIE_WDMA_ICE_DGCR_RESERVED2_MASK 0x40
#define  PIE_WDMA_ICE_DGCR_RESERVED2_SHIFT 6
#define  PIE_WDMA_ICE_DGCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_DGCR_RESERVED2_MASK) >> PIE_WDMA_ICE_DGCR_RESERVED2_SHIFT)
#define  PIE_WDMA_ICE_DGCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_DGCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_DGCR_RESERVED2_SHIFT))

#define  PIE_WDMA_ICE_DGCR_R_MASK 0x3f
#define  PIE_WDMA_ICE_DGCR_R_SHIFT 0
#define  PIE_WDMA_ICE_DGCR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_DGCR_R_MASK) >> PIE_WDMA_ICE_DGCR_R_SHIFT)
#define  PIE_WDMA_ICE_DGCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_DGCR_R_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_DGCR_R_SHIFT))

//====================================================================
//Register: Control Interrupt Register (CIR)
/** \brief DMA Channel Common Interrupt Select*/
//====================================================================

#define  PIE_WDMA_ICE_CIR_RESERVED1_MASK 0xfffffff8
#define  PIE_WDMA_ICE_CIR_RESERVED1_SHIFT 3
#define  PIE_WDMA_ICE_CIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_CIR_RESERVED1_MASK) >> PIE_WDMA_ICE_CIR_RESERVED1_SHIFT)
#define  PIE_WDMA_ICE_CIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_CIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_CIR_RESERVED1_SHIFT))

#define  PIE_WDMA_ICE_CIR_C_MASK 0x4
#define  PIE_WDMA_ICE_CIR_C_SHIFT 2
#define  PIE_WDMA_ICE_CIR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_CIR_C_MASK) >> PIE_WDMA_ICE_CIR_C_SHIFT)
#define  PIE_WDMA_ICE_CIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_CIR_C_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_CIR_C_SHIFT))

#define  PIE_WDMA_ICE_CIR_B_MASK 0x2
#define  PIE_WDMA_ICE_CIR_B_SHIFT 1
#define  PIE_WDMA_ICE_CIR_B_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_CIR_B_MASK) >> PIE_WDMA_ICE_CIR_B_SHIFT)
#define  PIE_WDMA_ICE_CIR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_CIR_B_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_CIR_B_SHIFT))

#define  PIE_WDMA_ICE_CIR_A_MASK 0x1
#define  PIE_WDMA_ICE_CIR_A_SHIFT 0
#define  PIE_WDMA_ICE_CIR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA_ICE_CIR_A_MASK) >> PIE_WDMA_ICE_CIR_A_SHIFT)
#define  PIE_WDMA_ICE_CIR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA_ICE_CIR_A_MASK) | (((uint32_t)val) << PIE_WDMA_ICE_CIR_A_SHIFT))

//====================================================================
//
//Register File: PIE_WDMA channel (Instance 1 of 3) (PIE_WDMA0)
/** \brief Register Descriptions for a single PIE WDMA channel*/
//
//====================================================================

//====================================================================
//Register: DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_WDMA0_DCR_C_MASK 0x80000000
#define  PIE_WDMA0_DCR_C_SHIFT 31
#define  PIE_WDMA0_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCR_C_MASK) >> PIE_WDMA0_DCR_C_SHIFT)
#define  PIE_WDMA0_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCR_C_MASK) | (((uint32_t)val) << PIE_WDMA0_DCR_C_SHIFT))

#define  PIE_WDMA0_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_WDMA0_DCR_RESERVED1_SHIFT 5
#define  PIE_WDMA0_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCR_RESERVED1_MASK) >> PIE_WDMA0_DCR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DCR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DCR_B_MASK 0x10
#define  PIE_WDMA0_DCR_B_SHIFT 4
#define  PIE_WDMA0_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCR_B_MASK) >> PIE_WDMA0_DCR_B_SHIFT)
#define  PIE_WDMA0_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCR_B_MASK) | (((uint32_t)val) << PIE_WDMA0_DCR_B_SHIFT))

#define  PIE_WDMA0_DCR_RESERVED2_MASK 0xc
#define  PIE_WDMA0_DCR_RESERVED2_SHIFT 2
#define  PIE_WDMA0_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCR_RESERVED2_MASK) >> PIE_WDMA0_DCR_RESERVED2_SHIFT)
#define  PIE_WDMA0_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA0_DCR_RESERVED2_SHIFT))

#define  PIE_WDMA0_DCR_R_MASK 0x2
#define  PIE_WDMA0_DCR_R_SHIFT 1
#define  PIE_WDMA0_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCR_R_MASK) >> PIE_WDMA0_DCR_R_SHIFT)
#define  PIE_WDMA0_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCR_R_MASK) | (((uint32_t)val) << PIE_WDMA0_DCR_R_SHIFT))

#define  PIE_WDMA0_DCR_E_MASK 0x1
#define  PIE_WDMA0_DCR_E_SHIFT 0
#define  PIE_WDMA0_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCR_E_MASK) >> PIE_WDMA0_DCR_E_SHIFT)
#define  PIE_WDMA0_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCR_E_MASK) | (((uint32_t)val) << PIE_WDMA0_DCR_E_SHIFT))

//====================================================================
//Register: PIE Write DMA Address Register (DAR)
/** \brief DMA Channel Starting Word Address Register*/
//====================================================================

#define  PIE_WDMA0_DAR_A_MASK 0xfffffff0
#define  PIE_WDMA0_DAR_A_SHIFT 4
#define  PIE_WDMA0_DAR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DAR_A_MASK) >> PIE_WDMA0_DAR_A_SHIFT)
#define  PIE_WDMA0_DAR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DAR_A_MASK) | (((uint32_t)val) << PIE_WDMA0_DAR_A_SHIFT))

#define  PIE_WDMA0_DAR_RESERVED1_MASK 0xf
#define  PIE_WDMA0_DAR_RESERVED1_SHIFT 0
#define  PIE_WDMA0_DAR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DAR_RESERVED1_MASK) >> PIE_WDMA0_DAR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DAR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DAR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register (DRCR)
/** \brief Row count in lines*/
//====================================================================

#define  PIE_WDMA0_DRCR_RESERVED1_MASK 0xffffffc0
#define  PIE_WDMA0_DRCR_RESERVED1_SHIFT 6
#define  PIE_WDMA0_DRCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRCR_RESERVED1_MASK) >> PIE_WDMA0_DRCR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DRCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DRCR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DRCR_R_MASK 0x3f
#define  PIE_WDMA0_DRCR_R_SHIFT 0
#define  PIE_WDMA0_DRCR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRCR_R_MASK) >> PIE_WDMA0_DRCR_R_SHIFT)
#define  PIE_WDMA0_DRCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRCR_R_MASK) | (((uint32_t)val) << PIE_WDMA0_DRCR_R_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_WDMA0_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_WDMA0_DBCR_RESERVED1_SHIFT 18
#define  PIE_WDMA0_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DBCR_RESERVED1_MASK) >> PIE_WDMA0_DBCR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DBCR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DBCR_W_MASK 0x3fff0
#define  PIE_WDMA0_DBCR_W_SHIFT 4
#define  PIE_WDMA0_DBCR_W_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DBCR_W_MASK) >> PIE_WDMA0_DBCR_W_SHIFT)
#define  PIE_WDMA0_DBCR_W_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DBCR_W_MASK) | (((uint32_t)val) << PIE_WDMA0_DBCR_W_SHIFT))

#define  PIE_WDMA0_DBCR_RESERVED2_MASK 0xf
#define  PIE_WDMA0_DBCR_RESERVED2_SHIFT 0
#define  PIE_WDMA0_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DBCR_RESERVED2_MASK) >> PIE_WDMA0_DBCR_RESERVED2_SHIFT)
#define  PIE_WDMA0_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA0_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_WDMA0_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_WDMA0_DTCR_RESERVED1_SHIFT 20
#define  PIE_WDMA0_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DTCR_RESERVED1_MASK) >> PIE_WDMA0_DTCR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DTCR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DTCR_T_MASK 0xffff0
#define  PIE_WDMA0_DTCR_T_SHIFT 4
#define  PIE_WDMA0_DTCR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DTCR_T_MASK) >> PIE_WDMA0_DTCR_T_SHIFT)
#define  PIE_WDMA0_DTCR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DTCR_T_MASK) | (((uint32_t)val) << PIE_WDMA0_DTCR_T_SHIFT))

#define  PIE_WDMA0_DTCR_RESERVED2_MASK 0xf
#define  PIE_WDMA0_DTCR_RESERVED2_SHIFT 0
#define  PIE_WDMA0_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DTCR_RESERVED2_MASK) >> PIE_WDMA0_DTCR_RESERVED2_SHIFT)
#define  PIE_WDMA0_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA0_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_WDMA0_DSTR_START_MASK 0xffffffff
#define  PIE_WDMA0_DSTR_START_SHIFT 0
#define  PIE_WDMA0_DSTR_START_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DSTR_START_MASK) >> PIE_WDMA0_DSTR_START_SHIFT)
#define  PIE_WDMA0_DSTR_START_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DSTR_START_MASK) | (((uint32_t)val) << PIE_WDMA0_DSTR_START_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_WDMA0_DASR_A_MASK 0xfffffffc
#define  PIE_WDMA0_DASR_A_SHIFT 2
#define  PIE_WDMA0_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DASR_A_MASK) >> PIE_WDMA0_DASR_A_SHIFT)
#define  PIE_WDMA0_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DASR_A_MASK) | (((uint32_t)val) << PIE_WDMA0_DASR_A_SHIFT))

#define  PIE_WDMA0_DASR_RESERVED1_MASK 0x3
#define  PIE_WDMA0_DASR_RESERVED1_SHIFT 0
#define  PIE_WDMA0_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DASR_RESERVED1_MASK) >> PIE_WDMA0_DASR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_WDMA0_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_WDMA0_DCSR_RESERVED1_SHIFT 20
#define  PIE_WDMA0_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCSR_RESERVED1_MASK) >> PIE_WDMA0_DCSR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DCSR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DCSR_T_MASK 0xfffff
#define  PIE_WDMA0_DCSR_T_SHIFT 0
#define  PIE_WDMA0_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DCSR_T_MASK) >> PIE_WDMA0_DCSR_T_SHIFT)
#define  PIE_WDMA0_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DCSR_T_MASK) | (((uint32_t)val) << PIE_WDMA0_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_WDMA0_DRCSR_RESERVED1_MASK 0xffffffe0
#define  PIE_WDMA0_DRCSR_RESERVED1_SHIFT 5
#define  PIE_WDMA0_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRCSR_RESERVED1_MASK) >> PIE_WDMA0_DRCSR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DRCSR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DRCSR_R_MASK 0x1f
#define  PIE_WDMA0_DRCSR_R_SHIFT 0
#define  PIE_WDMA0_DRCSR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRCSR_R_MASK) >> PIE_WDMA0_DRCSR_R_SHIFT)
#define  PIE_WDMA0_DRCSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRCSR_R_MASK) | (((uint32_t)val) << PIE_WDMA0_DRCSR_R_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_WDMA0_DSR_C_MASK 0x80000000
#define  PIE_WDMA0_DSR_C_SHIFT 31
#define  PIE_WDMA0_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DSR_C_MASK) >> PIE_WDMA0_DSR_C_SHIFT)
#define  PIE_WDMA0_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DSR_C_MASK) | (((uint32_t)val) << PIE_WDMA0_DSR_C_SHIFT))

#define  PIE_WDMA0_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_WDMA0_DSR_RESERVED1_SHIFT 2
#define  PIE_WDMA0_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DSR_RESERVED1_MASK) >> PIE_WDMA0_DSR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DSR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DSR_R_MASK 0x2
#define  PIE_WDMA0_DSR_R_SHIFT 1
#define  PIE_WDMA0_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DSR_R_MASK) >> PIE_WDMA0_DSR_R_SHIFT)
#define  PIE_WDMA0_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DSR_R_MASK) | (((uint32_t)val) << PIE_WDMA0_DSR_R_SHIFT))

#define  PIE_WDMA0_DSR_A_MASK 0x1
#define  PIE_WDMA0_DSR_A_SHIFT 0
#define  PIE_WDMA0_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DSR_A_MASK) >> PIE_WDMA0_DSR_A_SHIFT)
#define  PIE_WDMA0_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DSR_A_MASK) | (((uint32_t)val) << PIE_WDMA0_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_WDMA0_DRR_RESERVED1_MASK 0xff000000
#define  PIE_WDMA0_DRR_RESERVED1_SHIFT 24
#define  PIE_WDMA0_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRR_RESERVED1_MASK) >> PIE_WDMA0_DRR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DRR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DRR_T_MASK 0x800000
#define  PIE_WDMA0_DRR_T_SHIFT 23
#define  PIE_WDMA0_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRR_T_MASK) >> PIE_WDMA0_DRR_T_SHIFT)
#define  PIE_WDMA0_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRR_T_MASK) | (((uint32_t)val) << PIE_WDMA0_DRR_T_SHIFT))

#define  PIE_WDMA0_DRR_E_MASK 0x400000
#define  PIE_WDMA0_DRR_E_SHIFT 22
#define  PIE_WDMA0_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRR_E_MASK) >> PIE_WDMA0_DRR_E_SHIFT)
#define  PIE_WDMA0_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRR_E_MASK) | (((uint32_t)val) << PIE_WDMA0_DRR_E_SHIFT))

#define  PIE_WDMA0_DRR_RESERVED2_MASK 0x300000
#define  PIE_WDMA0_DRR_RESERVED2_SHIFT 20
#define  PIE_WDMA0_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRR_RESERVED2_MASK) >> PIE_WDMA0_DRR_RESERVED2_SHIFT)
#define  PIE_WDMA0_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA0_DRR_RESERVED2_SHIFT))

#define  PIE_WDMA0_DRR_N_MASK 0xfffff
#define  PIE_WDMA0_DRR_N_SHIFT 0
#define  PIE_WDMA0_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DRR_N_MASK) >> PIE_WDMA0_DRR_N_SHIFT)
#define  PIE_WDMA0_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DRR_N_MASK) | (((uint32_t)val) << PIE_WDMA0_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_WDMA0_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_WDMA0_DIR_RESERVED1_SHIFT 1
#define  PIE_WDMA0_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DIR_RESERVED1_MASK) >> PIE_WDMA0_DIR_RESERVED1_SHIFT)
#define  PIE_WDMA0_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA0_DIR_RESERVED1_SHIFT))

#define  PIE_WDMA0_DIR_C_MASK 0x1
#define  PIE_WDMA0_DIR_C_SHIFT 0
#define  PIE_WDMA0_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA0_DIR_C_MASK) >> PIE_WDMA0_DIR_C_SHIFT)
#define  PIE_WDMA0_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA0_DIR_C_MASK) | (((uint32_t)val) << PIE_WDMA0_DIR_C_SHIFT))

//====================================================================
//
//Register File: PIE_WDMA channel (Instance 2 of 3) (PIE_WDMA1)
/** \brief Register Descriptions for a single PIE WDMA channel*/
//
//====================================================================

//====================================================================
//Register: DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_WDMA1_DCR_C_MASK 0x80000000
#define  PIE_WDMA1_DCR_C_SHIFT 31
#define  PIE_WDMA1_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCR_C_MASK) >> PIE_WDMA1_DCR_C_SHIFT)
#define  PIE_WDMA1_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCR_C_MASK) | (((uint32_t)val) << PIE_WDMA1_DCR_C_SHIFT))

#define  PIE_WDMA1_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_WDMA1_DCR_RESERVED1_SHIFT 5
#define  PIE_WDMA1_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCR_RESERVED1_MASK) >> PIE_WDMA1_DCR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DCR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DCR_B_MASK 0x10
#define  PIE_WDMA1_DCR_B_SHIFT 4
#define  PIE_WDMA1_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCR_B_MASK) >> PIE_WDMA1_DCR_B_SHIFT)
#define  PIE_WDMA1_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCR_B_MASK) | (((uint32_t)val) << PIE_WDMA1_DCR_B_SHIFT))

#define  PIE_WDMA1_DCR_RESERVED2_MASK 0xc
#define  PIE_WDMA1_DCR_RESERVED2_SHIFT 2
#define  PIE_WDMA1_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCR_RESERVED2_MASK) >> PIE_WDMA1_DCR_RESERVED2_SHIFT)
#define  PIE_WDMA1_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA1_DCR_RESERVED2_SHIFT))

#define  PIE_WDMA1_DCR_R_MASK 0x2
#define  PIE_WDMA1_DCR_R_SHIFT 1
#define  PIE_WDMA1_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCR_R_MASK) >> PIE_WDMA1_DCR_R_SHIFT)
#define  PIE_WDMA1_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCR_R_MASK) | (((uint32_t)val) << PIE_WDMA1_DCR_R_SHIFT))

#define  PIE_WDMA1_DCR_E_MASK 0x1
#define  PIE_WDMA1_DCR_E_SHIFT 0
#define  PIE_WDMA1_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCR_E_MASK) >> PIE_WDMA1_DCR_E_SHIFT)
#define  PIE_WDMA1_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCR_E_MASK) | (((uint32_t)val) << PIE_WDMA1_DCR_E_SHIFT))

//====================================================================
//Register: PIE Write DMA Address Register (DAR)
/** \brief DMA Channel Starting Word Address Register*/
//====================================================================

#define  PIE_WDMA1_DAR_A_MASK 0xfffffff0
#define  PIE_WDMA1_DAR_A_SHIFT 4
#define  PIE_WDMA1_DAR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DAR_A_MASK) >> PIE_WDMA1_DAR_A_SHIFT)
#define  PIE_WDMA1_DAR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DAR_A_MASK) | (((uint32_t)val) << PIE_WDMA1_DAR_A_SHIFT))

#define  PIE_WDMA1_DAR_RESERVED1_MASK 0xf
#define  PIE_WDMA1_DAR_RESERVED1_SHIFT 0
#define  PIE_WDMA1_DAR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DAR_RESERVED1_MASK) >> PIE_WDMA1_DAR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DAR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DAR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register (DRCR)
/** \brief Row count in lines*/
//====================================================================

#define  PIE_WDMA1_DRCR_RESERVED1_MASK 0xffffffc0
#define  PIE_WDMA1_DRCR_RESERVED1_SHIFT 6
#define  PIE_WDMA1_DRCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRCR_RESERVED1_MASK) >> PIE_WDMA1_DRCR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DRCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DRCR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DRCR_R_MASK 0x3f
#define  PIE_WDMA1_DRCR_R_SHIFT 0
#define  PIE_WDMA1_DRCR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRCR_R_MASK) >> PIE_WDMA1_DRCR_R_SHIFT)
#define  PIE_WDMA1_DRCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRCR_R_MASK) | (((uint32_t)val) << PIE_WDMA1_DRCR_R_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_WDMA1_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_WDMA1_DBCR_RESERVED1_SHIFT 18
#define  PIE_WDMA1_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DBCR_RESERVED1_MASK) >> PIE_WDMA1_DBCR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DBCR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DBCR_W_MASK 0x3fff0
#define  PIE_WDMA1_DBCR_W_SHIFT 4
#define  PIE_WDMA1_DBCR_W_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DBCR_W_MASK) >> PIE_WDMA1_DBCR_W_SHIFT)
#define  PIE_WDMA1_DBCR_W_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DBCR_W_MASK) | (((uint32_t)val) << PIE_WDMA1_DBCR_W_SHIFT))

#define  PIE_WDMA1_DBCR_RESERVED2_MASK 0xf
#define  PIE_WDMA1_DBCR_RESERVED2_SHIFT 0
#define  PIE_WDMA1_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DBCR_RESERVED2_MASK) >> PIE_WDMA1_DBCR_RESERVED2_SHIFT)
#define  PIE_WDMA1_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA1_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_WDMA1_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_WDMA1_DTCR_RESERVED1_SHIFT 20
#define  PIE_WDMA1_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DTCR_RESERVED1_MASK) >> PIE_WDMA1_DTCR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DTCR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DTCR_T_MASK 0xffff0
#define  PIE_WDMA1_DTCR_T_SHIFT 4
#define  PIE_WDMA1_DTCR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DTCR_T_MASK) >> PIE_WDMA1_DTCR_T_SHIFT)
#define  PIE_WDMA1_DTCR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DTCR_T_MASK) | (((uint32_t)val) << PIE_WDMA1_DTCR_T_SHIFT))

#define  PIE_WDMA1_DTCR_RESERVED2_MASK 0xf
#define  PIE_WDMA1_DTCR_RESERVED2_SHIFT 0
#define  PIE_WDMA1_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DTCR_RESERVED2_MASK) >> PIE_WDMA1_DTCR_RESERVED2_SHIFT)
#define  PIE_WDMA1_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA1_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_WDMA1_DSTR_START_MASK 0xffffffff
#define  PIE_WDMA1_DSTR_START_SHIFT 0
#define  PIE_WDMA1_DSTR_START_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DSTR_START_MASK) >> PIE_WDMA1_DSTR_START_SHIFT)
#define  PIE_WDMA1_DSTR_START_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DSTR_START_MASK) | (((uint32_t)val) << PIE_WDMA1_DSTR_START_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_WDMA1_DASR_A_MASK 0xfffffffc
#define  PIE_WDMA1_DASR_A_SHIFT 2
#define  PIE_WDMA1_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DASR_A_MASK) >> PIE_WDMA1_DASR_A_SHIFT)
#define  PIE_WDMA1_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DASR_A_MASK) | (((uint32_t)val) << PIE_WDMA1_DASR_A_SHIFT))

#define  PIE_WDMA1_DASR_RESERVED1_MASK 0x3
#define  PIE_WDMA1_DASR_RESERVED1_SHIFT 0
#define  PIE_WDMA1_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DASR_RESERVED1_MASK) >> PIE_WDMA1_DASR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_WDMA1_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_WDMA1_DCSR_RESERVED1_SHIFT 20
#define  PIE_WDMA1_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCSR_RESERVED1_MASK) >> PIE_WDMA1_DCSR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DCSR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DCSR_T_MASK 0xfffff
#define  PIE_WDMA1_DCSR_T_SHIFT 0
#define  PIE_WDMA1_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DCSR_T_MASK) >> PIE_WDMA1_DCSR_T_SHIFT)
#define  PIE_WDMA1_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DCSR_T_MASK) | (((uint32_t)val) << PIE_WDMA1_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_WDMA1_DRCSR_RESERVED1_MASK 0xffffffe0
#define  PIE_WDMA1_DRCSR_RESERVED1_SHIFT 5
#define  PIE_WDMA1_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRCSR_RESERVED1_MASK) >> PIE_WDMA1_DRCSR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DRCSR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DRCSR_R_MASK 0x1f
#define  PIE_WDMA1_DRCSR_R_SHIFT 0
#define  PIE_WDMA1_DRCSR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRCSR_R_MASK) >> PIE_WDMA1_DRCSR_R_SHIFT)
#define  PIE_WDMA1_DRCSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRCSR_R_MASK) | (((uint32_t)val) << PIE_WDMA1_DRCSR_R_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_WDMA1_DSR_C_MASK 0x80000000
#define  PIE_WDMA1_DSR_C_SHIFT 31
#define  PIE_WDMA1_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DSR_C_MASK) >> PIE_WDMA1_DSR_C_SHIFT)
#define  PIE_WDMA1_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DSR_C_MASK) | (((uint32_t)val) << PIE_WDMA1_DSR_C_SHIFT))

#define  PIE_WDMA1_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_WDMA1_DSR_RESERVED1_SHIFT 2
#define  PIE_WDMA1_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DSR_RESERVED1_MASK) >> PIE_WDMA1_DSR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DSR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DSR_R_MASK 0x2
#define  PIE_WDMA1_DSR_R_SHIFT 1
#define  PIE_WDMA1_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DSR_R_MASK) >> PIE_WDMA1_DSR_R_SHIFT)
#define  PIE_WDMA1_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DSR_R_MASK) | (((uint32_t)val) << PIE_WDMA1_DSR_R_SHIFT))

#define  PIE_WDMA1_DSR_A_MASK 0x1
#define  PIE_WDMA1_DSR_A_SHIFT 0
#define  PIE_WDMA1_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DSR_A_MASK) >> PIE_WDMA1_DSR_A_SHIFT)
#define  PIE_WDMA1_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DSR_A_MASK) | (((uint32_t)val) << PIE_WDMA1_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_WDMA1_DRR_RESERVED1_MASK 0xff000000
#define  PIE_WDMA1_DRR_RESERVED1_SHIFT 24
#define  PIE_WDMA1_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRR_RESERVED1_MASK) >> PIE_WDMA1_DRR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DRR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DRR_T_MASK 0x800000
#define  PIE_WDMA1_DRR_T_SHIFT 23
#define  PIE_WDMA1_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRR_T_MASK) >> PIE_WDMA1_DRR_T_SHIFT)
#define  PIE_WDMA1_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRR_T_MASK) | (((uint32_t)val) << PIE_WDMA1_DRR_T_SHIFT))

#define  PIE_WDMA1_DRR_E_MASK 0x400000
#define  PIE_WDMA1_DRR_E_SHIFT 22
#define  PIE_WDMA1_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRR_E_MASK) >> PIE_WDMA1_DRR_E_SHIFT)
#define  PIE_WDMA1_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRR_E_MASK) | (((uint32_t)val) << PIE_WDMA1_DRR_E_SHIFT))

#define  PIE_WDMA1_DRR_RESERVED2_MASK 0x300000
#define  PIE_WDMA1_DRR_RESERVED2_SHIFT 20
#define  PIE_WDMA1_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRR_RESERVED2_MASK) >> PIE_WDMA1_DRR_RESERVED2_SHIFT)
#define  PIE_WDMA1_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA1_DRR_RESERVED2_SHIFT))

#define  PIE_WDMA1_DRR_N_MASK 0xfffff
#define  PIE_WDMA1_DRR_N_SHIFT 0
#define  PIE_WDMA1_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DRR_N_MASK) >> PIE_WDMA1_DRR_N_SHIFT)
#define  PIE_WDMA1_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DRR_N_MASK) | (((uint32_t)val) << PIE_WDMA1_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_WDMA1_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_WDMA1_DIR_RESERVED1_SHIFT 1
#define  PIE_WDMA1_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DIR_RESERVED1_MASK) >> PIE_WDMA1_DIR_RESERVED1_SHIFT)
#define  PIE_WDMA1_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA1_DIR_RESERVED1_SHIFT))

#define  PIE_WDMA1_DIR_C_MASK 0x1
#define  PIE_WDMA1_DIR_C_SHIFT 0
#define  PIE_WDMA1_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA1_DIR_C_MASK) >> PIE_WDMA1_DIR_C_SHIFT)
#define  PIE_WDMA1_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA1_DIR_C_MASK) | (((uint32_t)val) << PIE_WDMA1_DIR_C_SHIFT))

//====================================================================
//
//Register File: PIE_WDMA channel (Instance 3 of 3) (PIE_WDMA2)
/** \brief Register Descriptions for a single PIE WDMA channel*/
//
//====================================================================

//====================================================================
//Register: DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_WDMA2_DCR_C_MASK 0x80000000
#define  PIE_WDMA2_DCR_C_SHIFT 31
#define  PIE_WDMA2_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCR_C_MASK) >> PIE_WDMA2_DCR_C_SHIFT)
#define  PIE_WDMA2_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCR_C_MASK) | (((uint32_t)val) << PIE_WDMA2_DCR_C_SHIFT))

#define  PIE_WDMA2_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_WDMA2_DCR_RESERVED1_SHIFT 5
#define  PIE_WDMA2_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCR_RESERVED1_MASK) >> PIE_WDMA2_DCR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DCR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DCR_B_MASK 0x10
#define  PIE_WDMA2_DCR_B_SHIFT 4
#define  PIE_WDMA2_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCR_B_MASK) >> PIE_WDMA2_DCR_B_SHIFT)
#define  PIE_WDMA2_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCR_B_MASK) | (((uint32_t)val) << PIE_WDMA2_DCR_B_SHIFT))

#define  PIE_WDMA2_DCR_RESERVED2_MASK 0xc
#define  PIE_WDMA2_DCR_RESERVED2_SHIFT 2
#define  PIE_WDMA2_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCR_RESERVED2_MASK) >> PIE_WDMA2_DCR_RESERVED2_SHIFT)
#define  PIE_WDMA2_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA2_DCR_RESERVED2_SHIFT))

#define  PIE_WDMA2_DCR_R_MASK 0x2
#define  PIE_WDMA2_DCR_R_SHIFT 1
#define  PIE_WDMA2_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCR_R_MASK) >> PIE_WDMA2_DCR_R_SHIFT)
#define  PIE_WDMA2_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCR_R_MASK) | (((uint32_t)val) << PIE_WDMA2_DCR_R_SHIFT))

#define  PIE_WDMA2_DCR_E_MASK 0x1
#define  PIE_WDMA2_DCR_E_SHIFT 0
#define  PIE_WDMA2_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCR_E_MASK) >> PIE_WDMA2_DCR_E_SHIFT)
#define  PIE_WDMA2_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCR_E_MASK) | (((uint32_t)val) << PIE_WDMA2_DCR_E_SHIFT))

//====================================================================
//Register: PIE Write DMA Address Register (DAR)
/** \brief DMA Channel Starting Word Address Register*/
//====================================================================

#define  PIE_WDMA2_DAR_A_MASK 0xfffffff0
#define  PIE_WDMA2_DAR_A_SHIFT 4
#define  PIE_WDMA2_DAR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DAR_A_MASK) >> PIE_WDMA2_DAR_A_SHIFT)
#define  PIE_WDMA2_DAR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DAR_A_MASK) | (((uint32_t)val) << PIE_WDMA2_DAR_A_SHIFT))

#define  PIE_WDMA2_DAR_RESERVED1_MASK 0xf
#define  PIE_WDMA2_DAR_RESERVED1_SHIFT 0
#define  PIE_WDMA2_DAR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DAR_RESERVED1_MASK) >> PIE_WDMA2_DAR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DAR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DAR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register (DRCR)
/** \brief Row count in lines*/
//====================================================================

#define  PIE_WDMA2_DRCR_RESERVED1_MASK 0xffffffc0
#define  PIE_WDMA2_DRCR_RESERVED1_SHIFT 6
#define  PIE_WDMA2_DRCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRCR_RESERVED1_MASK) >> PIE_WDMA2_DRCR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DRCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DRCR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DRCR_R_MASK 0x3f
#define  PIE_WDMA2_DRCR_R_SHIFT 0
#define  PIE_WDMA2_DRCR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRCR_R_MASK) >> PIE_WDMA2_DRCR_R_SHIFT)
#define  PIE_WDMA2_DRCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRCR_R_MASK) | (((uint32_t)val) << PIE_WDMA2_DRCR_R_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_WDMA2_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_WDMA2_DBCR_RESERVED1_SHIFT 18
#define  PIE_WDMA2_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DBCR_RESERVED1_MASK) >> PIE_WDMA2_DBCR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DBCR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DBCR_W_MASK 0x3fff0
#define  PIE_WDMA2_DBCR_W_SHIFT 4
#define  PIE_WDMA2_DBCR_W_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DBCR_W_MASK) >> PIE_WDMA2_DBCR_W_SHIFT)
#define  PIE_WDMA2_DBCR_W_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DBCR_W_MASK) | (((uint32_t)val) << PIE_WDMA2_DBCR_W_SHIFT))

#define  PIE_WDMA2_DBCR_RESERVED2_MASK 0xf
#define  PIE_WDMA2_DBCR_RESERVED2_SHIFT 0
#define  PIE_WDMA2_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DBCR_RESERVED2_MASK) >> PIE_WDMA2_DBCR_RESERVED2_SHIFT)
#define  PIE_WDMA2_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA2_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_WDMA2_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_WDMA2_DTCR_RESERVED1_SHIFT 20
#define  PIE_WDMA2_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DTCR_RESERVED1_MASK) >> PIE_WDMA2_DTCR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DTCR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DTCR_T_MASK 0xffff0
#define  PIE_WDMA2_DTCR_T_SHIFT 4
#define  PIE_WDMA2_DTCR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DTCR_T_MASK) >> PIE_WDMA2_DTCR_T_SHIFT)
#define  PIE_WDMA2_DTCR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DTCR_T_MASK) | (((uint32_t)val) << PIE_WDMA2_DTCR_T_SHIFT))

#define  PIE_WDMA2_DTCR_RESERVED2_MASK 0xf
#define  PIE_WDMA2_DTCR_RESERVED2_SHIFT 0
#define  PIE_WDMA2_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DTCR_RESERVED2_MASK) >> PIE_WDMA2_DTCR_RESERVED2_SHIFT)
#define  PIE_WDMA2_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA2_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_WDMA2_DSTR_START_MASK 0xffffffff
#define  PIE_WDMA2_DSTR_START_SHIFT 0
#define  PIE_WDMA2_DSTR_START_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DSTR_START_MASK) >> PIE_WDMA2_DSTR_START_SHIFT)
#define  PIE_WDMA2_DSTR_START_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DSTR_START_MASK) | (((uint32_t)val) << PIE_WDMA2_DSTR_START_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_WDMA2_DASR_A_MASK 0xfffffffc
#define  PIE_WDMA2_DASR_A_SHIFT 2
#define  PIE_WDMA2_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DASR_A_MASK) >> PIE_WDMA2_DASR_A_SHIFT)
#define  PIE_WDMA2_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DASR_A_MASK) | (((uint32_t)val) << PIE_WDMA2_DASR_A_SHIFT))

#define  PIE_WDMA2_DASR_RESERVED1_MASK 0x3
#define  PIE_WDMA2_DASR_RESERVED1_SHIFT 0
#define  PIE_WDMA2_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DASR_RESERVED1_MASK) >> PIE_WDMA2_DASR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_WDMA2_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_WDMA2_DCSR_RESERVED1_SHIFT 20
#define  PIE_WDMA2_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCSR_RESERVED1_MASK) >> PIE_WDMA2_DCSR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DCSR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DCSR_T_MASK 0xfffff
#define  PIE_WDMA2_DCSR_T_SHIFT 0
#define  PIE_WDMA2_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DCSR_T_MASK) >> PIE_WDMA2_DCSR_T_SHIFT)
#define  PIE_WDMA2_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DCSR_T_MASK) | (((uint32_t)val) << PIE_WDMA2_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_WDMA2_DRCSR_RESERVED1_MASK 0xffffffe0
#define  PIE_WDMA2_DRCSR_RESERVED1_SHIFT 5
#define  PIE_WDMA2_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRCSR_RESERVED1_MASK) >> PIE_WDMA2_DRCSR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DRCSR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DRCSR_R_MASK 0x1f
#define  PIE_WDMA2_DRCSR_R_SHIFT 0
#define  PIE_WDMA2_DRCSR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRCSR_R_MASK) >> PIE_WDMA2_DRCSR_R_SHIFT)
#define  PIE_WDMA2_DRCSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRCSR_R_MASK) | (((uint32_t)val) << PIE_WDMA2_DRCSR_R_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_WDMA2_DSR_C_MASK 0x80000000
#define  PIE_WDMA2_DSR_C_SHIFT 31
#define  PIE_WDMA2_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DSR_C_MASK) >> PIE_WDMA2_DSR_C_SHIFT)
#define  PIE_WDMA2_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DSR_C_MASK) | (((uint32_t)val) << PIE_WDMA2_DSR_C_SHIFT))

#define  PIE_WDMA2_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_WDMA2_DSR_RESERVED1_SHIFT 2
#define  PIE_WDMA2_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DSR_RESERVED1_MASK) >> PIE_WDMA2_DSR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DSR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DSR_R_MASK 0x2
#define  PIE_WDMA2_DSR_R_SHIFT 1
#define  PIE_WDMA2_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DSR_R_MASK) >> PIE_WDMA2_DSR_R_SHIFT)
#define  PIE_WDMA2_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DSR_R_MASK) | (((uint32_t)val) << PIE_WDMA2_DSR_R_SHIFT))

#define  PIE_WDMA2_DSR_A_MASK 0x1
#define  PIE_WDMA2_DSR_A_SHIFT 0
#define  PIE_WDMA2_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DSR_A_MASK) >> PIE_WDMA2_DSR_A_SHIFT)
#define  PIE_WDMA2_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DSR_A_MASK) | (((uint32_t)val) << PIE_WDMA2_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_WDMA2_DRR_RESERVED1_MASK 0xff000000
#define  PIE_WDMA2_DRR_RESERVED1_SHIFT 24
#define  PIE_WDMA2_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRR_RESERVED1_MASK) >> PIE_WDMA2_DRR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DRR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DRR_T_MASK 0x800000
#define  PIE_WDMA2_DRR_T_SHIFT 23
#define  PIE_WDMA2_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRR_T_MASK) >> PIE_WDMA2_DRR_T_SHIFT)
#define  PIE_WDMA2_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRR_T_MASK) | (((uint32_t)val) << PIE_WDMA2_DRR_T_SHIFT))

#define  PIE_WDMA2_DRR_E_MASK 0x400000
#define  PIE_WDMA2_DRR_E_SHIFT 22
#define  PIE_WDMA2_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRR_E_MASK) >> PIE_WDMA2_DRR_E_SHIFT)
#define  PIE_WDMA2_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRR_E_MASK) | (((uint32_t)val) << PIE_WDMA2_DRR_E_SHIFT))

#define  PIE_WDMA2_DRR_RESERVED2_MASK 0x300000
#define  PIE_WDMA2_DRR_RESERVED2_SHIFT 20
#define  PIE_WDMA2_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRR_RESERVED2_MASK) >> PIE_WDMA2_DRR_RESERVED2_SHIFT)
#define  PIE_WDMA2_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_WDMA2_DRR_RESERVED2_SHIFT))

#define  PIE_WDMA2_DRR_N_MASK 0xfffff
#define  PIE_WDMA2_DRR_N_SHIFT 0
#define  PIE_WDMA2_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DRR_N_MASK) >> PIE_WDMA2_DRR_N_SHIFT)
#define  PIE_WDMA2_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DRR_N_MASK) | (((uint32_t)val) << PIE_WDMA2_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_WDMA2_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_WDMA2_DIR_RESERVED1_SHIFT 1
#define  PIE_WDMA2_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DIR_RESERVED1_MASK) >> PIE_WDMA2_DIR_RESERVED1_SHIFT)
#define  PIE_WDMA2_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_WDMA2_DIR_RESERVED1_SHIFT))

#define  PIE_WDMA2_DIR_C_MASK 0x1
#define  PIE_WDMA2_DIR_C_SHIFT 0
#define  PIE_WDMA2_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_WDMA2_DIR_C_MASK) >> PIE_WDMA2_DIR_C_SHIFT)
#define  PIE_WDMA2_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_WDMA2_DIR_C_MASK) | (((uint32_t)val) << PIE_WDMA2_DIR_C_SHIFT))

#endif // PIE_WDMA_ICE
