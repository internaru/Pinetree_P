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
 * \file PIE_RDMA_ICE_regmasks.h
 * Generated using RegBuild 1.6.2
 *
 *************************************************************************
 */

#ifndef _PIE_RDMA_ICE_REGMASKS_H_
#define _PIE_RDMA_ICE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIE_RDMA (PIE_RDMA_ICE)
/** \brief Register Descriptions for the multi-channel PIE RDMA block*/
//
//====================================================================

//====================================================================
//Register: Control Configuration Register (CCR)
/** \brief General configuration register for all DMAs in this block.*/
//====================================================================

#define  PIE_RDMA_ICE_CCR_RESERVED1_MASK 0xffffc000
#define  PIE_RDMA_ICE_CCR_RESERVED1_SHIFT 14
#define  PIE_RDMA_ICE_CCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CCR_RESERVED1_MASK) >> PIE_RDMA_ICE_CCR_RESERVED1_SHIFT)
#define  PIE_RDMA_ICE_CCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CCR_RESERVED1_SHIFT))

#define  PIE_RDMA_ICE_CCR_R_MASK 0x3f00
#define  PIE_RDMA_ICE_CCR_R_SHIFT 8
#define  PIE_RDMA_ICE_CCR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CCR_R_MASK) >> PIE_RDMA_ICE_CCR_R_SHIFT)
#define  PIE_RDMA_ICE_CCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CCR_R_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CCR_R_SHIFT))

#define  PIE_RDMA_ICE_CCR_RESERVED2_MASK 0xf8
#define  PIE_RDMA_ICE_CCR_RESERVED2_SHIFT 3
#define  PIE_RDMA_ICE_CCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CCR_RESERVED2_MASK) >> PIE_RDMA_ICE_CCR_RESERVED2_SHIFT)
#define  PIE_RDMA_ICE_CCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CCR_RESERVED2_SHIFT))

#define  PIE_RDMA_ICE_CCR_P_MASK 0x4
#define  PIE_RDMA_ICE_CCR_P_SHIFT 2
#define  PIE_RDMA_ICE_CCR_P_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CCR_P_MASK) >> PIE_RDMA_ICE_CCR_P_SHIFT)
#define  PIE_RDMA_ICE_CCR_P_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CCR_P_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CCR_P_SHIFT))

#define  PIE_RDMA_ICE_CCR_S_MASK 0x2
#define  PIE_RDMA_ICE_CCR_S_SHIFT 1
#define  PIE_RDMA_ICE_CCR_S_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CCR_S_MASK) >> PIE_RDMA_ICE_CCR_S_SHIFT)
#define  PIE_RDMA_ICE_CCR_S_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CCR_S_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CCR_S_SHIFT))

#define  PIE_RDMA_ICE_CCR_M_MASK 0x1
#define  PIE_RDMA_ICE_CCR_M_SHIFT 0
#define  PIE_RDMA_ICE_CCR_M_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CCR_M_MASK) >> PIE_RDMA_ICE_CCR_M_SHIFT)
#define  PIE_RDMA_ICE_CCR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CCR_M_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CCR_M_SHIFT))

//====================================================================
//Register: Control Start Register (CSR)
//====================================================================

#define  PIE_RDMA_ICE_CSR_RESERVED1_MASK 0xfffffffe
#define  PIE_RDMA_ICE_CSR_RESERVED1_SHIFT 1
#define  PIE_RDMA_ICE_CSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CSR_RESERVED1_MASK) >> PIE_RDMA_ICE_CSR_RESERVED1_SHIFT)
#define  PIE_RDMA_ICE_CSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CSR_RESERVED1_SHIFT))

#define  PIE_RDMA_ICE_CSR_S_MASK 0x1
#define  PIE_RDMA_ICE_CSR_S_SHIFT 0
#define  PIE_RDMA_ICE_CSR_S_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CSR_S_MASK) >> PIE_RDMA_ICE_CSR_S_SHIFT)
#define  PIE_RDMA_ICE_CSR_S_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CSR_S_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CSR_S_SHIFT))

//====================================================================
//Register: Control Interrupt Register (CIR)
/** \brief DMA Channel Common Interrupt Select*/
//====================================================================

#define  PIE_RDMA_ICE_CIR_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA_ICE_CIR_RESERVED1_SHIFT 6
#define  PIE_RDMA_ICE_CIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CIR_RESERVED1_MASK) >> PIE_RDMA_ICE_CIR_RESERVED1_SHIFT)
#define  PIE_RDMA_ICE_CIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CIR_RESERVED1_SHIFT))

#define  PIE_RDMA_ICE_CIR_F_MASK 0x20
#define  PIE_RDMA_ICE_CIR_F_SHIFT 5
#define  PIE_RDMA_ICE_CIR_F_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CIR_F_MASK) >> PIE_RDMA_ICE_CIR_F_SHIFT)
#define  PIE_RDMA_ICE_CIR_F_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CIR_F_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CIR_F_SHIFT))

#define  PIE_RDMA_ICE_CIR_E_MASK 0x10
#define  PIE_RDMA_ICE_CIR_E_SHIFT 4
#define  PIE_RDMA_ICE_CIR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CIR_E_MASK) >> PIE_RDMA_ICE_CIR_E_SHIFT)
#define  PIE_RDMA_ICE_CIR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CIR_E_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CIR_E_SHIFT))

#define  PIE_RDMA_ICE_CIR_D_MASK 0x8
#define  PIE_RDMA_ICE_CIR_D_SHIFT 3
#define  PIE_RDMA_ICE_CIR_D_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CIR_D_MASK) >> PIE_RDMA_ICE_CIR_D_SHIFT)
#define  PIE_RDMA_ICE_CIR_D_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CIR_D_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CIR_D_SHIFT))

#define  PIE_RDMA_ICE_CIR_C_MASK 0x4
#define  PIE_RDMA_ICE_CIR_C_SHIFT 2
#define  PIE_RDMA_ICE_CIR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CIR_C_MASK) >> PIE_RDMA_ICE_CIR_C_SHIFT)
#define  PIE_RDMA_ICE_CIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CIR_C_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CIR_C_SHIFT))

#define  PIE_RDMA_ICE_CIR_B_MASK 0x2
#define  PIE_RDMA_ICE_CIR_B_SHIFT 1
#define  PIE_RDMA_ICE_CIR_B_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CIR_B_MASK) >> PIE_RDMA_ICE_CIR_B_SHIFT)
#define  PIE_RDMA_ICE_CIR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CIR_B_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CIR_B_SHIFT))

#define  PIE_RDMA_ICE_CIR_A_MASK 0x1
#define  PIE_RDMA_ICE_CIR_A_SHIFT 0
#define  PIE_RDMA_ICE_CIR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA_ICE_CIR_A_MASK) >> PIE_RDMA_ICE_CIR_A_SHIFT)
#define  PIE_RDMA_ICE_CIR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA_ICE_CIR_A_MASK) | (((uint32_t)val) << PIE_RDMA_ICE_CIR_A_SHIFT))

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 1 of 6) (PIE_RDMA0)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

//====================================================================
//Register: PIE Read DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_RDMA0_DCR_C_MASK 0x80000000
#define  PIE_RDMA0_DCR_C_SHIFT 31
#define  PIE_RDMA0_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCR_C_MASK) >> PIE_RDMA0_DCR_C_SHIFT)
#define  PIE_RDMA0_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCR_C_MASK) | (((uint32_t)val) << PIE_RDMA0_DCR_C_SHIFT))

#define  PIE_RDMA0_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_RDMA0_DCR_RESERVED1_SHIFT 5
#define  PIE_RDMA0_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCR_RESERVED1_MASK) >> PIE_RDMA0_DCR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DCR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DCR_B_MASK 0x10
#define  PIE_RDMA0_DCR_B_SHIFT 4
#define  PIE_RDMA0_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCR_B_MASK) >> PIE_RDMA0_DCR_B_SHIFT)
#define  PIE_RDMA0_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCR_B_MASK) | (((uint32_t)val) << PIE_RDMA0_DCR_B_SHIFT))

#define  PIE_RDMA0_DCR_RESERVED2_MASK 0xc
#define  PIE_RDMA0_DCR_RESERVED2_SHIFT 2
#define  PIE_RDMA0_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCR_RESERVED2_MASK) >> PIE_RDMA0_DCR_RESERVED2_SHIFT)
#define  PIE_RDMA0_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA0_DCR_RESERVED2_SHIFT))

#define  PIE_RDMA0_DCR_R_MASK 0x2
#define  PIE_RDMA0_DCR_R_SHIFT 1
#define  PIE_RDMA0_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCR_R_MASK) >> PIE_RDMA0_DCR_R_SHIFT)
#define  PIE_RDMA0_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCR_R_MASK) | (((uint32_t)val) << PIE_RDMA0_DCR_R_SHIFT))

#define  PIE_RDMA0_DCR_E_MASK 0x1
#define  PIE_RDMA0_DCR_E_SHIFT 0
#define  PIE_RDMA0_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCR_E_MASK) >> PIE_RDMA0_DCR_E_SHIFT)
#define  PIE_RDMA0_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCR_E_MASK) | (((uint32_t)val) << PIE_RDMA0_DCR_E_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 1 (DAR1)
/** \brief DMA Channel Starting Word Address Register 1*/
//====================================================================

#define  PIE_RDMA0_DAR1_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA0_DAR1_ADDRESS_SHIFT 4
#define  PIE_RDMA0_DAR1_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DAR1_ADDRESS_MASK) >> PIE_RDMA0_DAR1_ADDRESS_SHIFT)
#define  PIE_RDMA0_DAR1_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DAR1_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA0_DAR1_ADDRESS_SHIFT))

#define  PIE_RDMA0_DAR1_RESERVED1_MASK 0xf
#define  PIE_RDMA0_DAR1_RESERVED1_SHIFT 0
#define  PIE_RDMA0_DAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DAR1_RESERVED1_MASK) >> PIE_RDMA0_DAR1_RESERVED1_SHIFT)
#define  PIE_RDMA0_DAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DAR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DAR1_RESERVED1_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 2 (DAR2)
/** \brief DMA Channel Starting Word Address Register 2*/
//====================================================================

#define  PIE_RDMA0_DAR2_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA0_DAR2_ADDRESS_SHIFT 4
#define  PIE_RDMA0_DAR2_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DAR2_ADDRESS_MASK) >> PIE_RDMA0_DAR2_ADDRESS_SHIFT)
#define  PIE_RDMA0_DAR2_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DAR2_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA0_DAR2_ADDRESS_SHIFT))

#define  PIE_RDMA0_DAR2_RESERVED1_MASK 0xf
#define  PIE_RDMA0_DAR2_RESERVED1_SHIFT 0
#define  PIE_RDMA0_DAR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DAR2_RESERVED1_MASK) >> PIE_RDMA0_DAR2_RESERVED1_SHIFT)
#define  PIE_RDMA0_DAR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DAR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DAR2_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register1 (DRCR1)
/** \brief Row count in lines for Address 1*/
//====================================================================

#define  PIE_RDMA0_DRCR1_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA0_DRCR1_RESERVED1_SHIFT 6
#define  PIE_RDMA0_DRCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCR1_RESERVED1_MASK) >> PIE_RDMA0_DRCR1_RESERVED1_SHIFT)
#define  PIE_RDMA0_DRCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCR1_RESERVED1_SHIFT))

#define  PIE_RDMA0_DRCR1_T_MASK 0x3f
#define  PIE_RDMA0_DRCR1_T_SHIFT 0
#define  PIE_RDMA0_DRCR1_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCR1_T_MASK) >> PIE_RDMA0_DRCR1_T_SHIFT)
#define  PIE_RDMA0_DRCR1_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCR1_T_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCR1_T_SHIFT))

//====================================================================
//Register: DMA Row Count Register2 (DRCR2)
/** \brief Row count in lines for Address 2*/
//====================================================================

#define  PIE_RDMA0_DRCR2_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA0_DRCR2_RESERVED1_SHIFT 6
#define  PIE_RDMA0_DRCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCR2_RESERVED1_MASK) >> PIE_RDMA0_DRCR2_RESERVED1_SHIFT)
#define  PIE_RDMA0_DRCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCR2_RESERVED1_SHIFT))

#define  PIE_RDMA0_DRCR2_T_MASK 0x3f
#define  PIE_RDMA0_DRCR2_T_SHIFT 0
#define  PIE_RDMA0_DRCR2_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCR2_T_MASK) >> PIE_RDMA0_DRCR2_T_SHIFT)
#define  PIE_RDMA0_DRCR2_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCR2_T_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCR2_T_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_RDMA0_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_RDMA0_DBCR_RESERVED1_SHIFT 18
#define  PIE_RDMA0_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DBCR_RESERVED1_MASK) >> PIE_RDMA0_DBCR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DBCR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DBCR_L_MASK 0x3fff0
#define  PIE_RDMA0_DBCR_L_SHIFT 4
#define  PIE_RDMA0_DBCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DBCR_L_MASK) >> PIE_RDMA0_DBCR_L_SHIFT)
#define  PIE_RDMA0_DBCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DBCR_L_MASK) | (((uint32_t)val) << PIE_RDMA0_DBCR_L_SHIFT))

#define  PIE_RDMA0_DBCR_RESERVED2_MASK 0xf
#define  PIE_RDMA0_DBCR_RESERVED2_SHIFT 0
#define  PIE_RDMA0_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DBCR_RESERVED2_MASK) >> PIE_RDMA0_DBCR_RESERVED2_SHIFT)
#define  PIE_RDMA0_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA0_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_RDMA0_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA0_DTCR_RESERVED1_SHIFT 20
#define  PIE_RDMA0_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DTCR_RESERVED1_MASK) >> PIE_RDMA0_DTCR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DTCR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DTCR_L_MASK 0xffff0
#define  PIE_RDMA0_DTCR_L_SHIFT 4
#define  PIE_RDMA0_DTCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DTCR_L_MASK) >> PIE_RDMA0_DTCR_L_SHIFT)
#define  PIE_RDMA0_DTCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DTCR_L_MASK) | (((uint32_t)val) << PIE_RDMA0_DTCR_L_SHIFT))

#define  PIE_RDMA0_DTCR_RESERVED2_MASK 0xf
#define  PIE_RDMA0_DTCR_RESERVED2_SHIFT 0
#define  PIE_RDMA0_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DTCR_RESERVED2_MASK) >> PIE_RDMA0_DTCR_RESERVED2_SHIFT)
#define  PIE_RDMA0_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA0_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_RDMA0_DSTR_RESERVED1_MASK 0xffffffff
#define  PIE_RDMA0_DSTR_RESERVED1_SHIFT 0
#define  PIE_RDMA0_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DSTR_RESERVED1_MASK) >> PIE_RDMA0_DSTR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DSTR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DSTR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_RDMA0_DASR_A_MASK 0xfffffffc
#define  PIE_RDMA0_DASR_A_SHIFT 2
#define  PIE_RDMA0_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DASR_A_MASK) >> PIE_RDMA0_DASR_A_SHIFT)
#define  PIE_RDMA0_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DASR_A_MASK) | (((uint32_t)val) << PIE_RDMA0_DASR_A_SHIFT))

#define  PIE_RDMA0_DASR_RESERVED1_MASK 0x3
#define  PIE_RDMA0_DASR_RESERVED1_SHIFT 0
#define  PIE_RDMA0_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DASR_RESERVED1_MASK) >> PIE_RDMA0_DASR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_RDMA0_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA0_DCSR_RESERVED1_SHIFT 20
#define  PIE_RDMA0_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCSR_RESERVED1_MASK) >> PIE_RDMA0_DCSR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DCSR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DCSR_T_MASK 0xfffff
#define  PIE_RDMA0_DCSR_T_SHIFT 0
#define  PIE_RDMA0_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DCSR_T_MASK) >> PIE_RDMA0_DCSR_T_SHIFT)
#define  PIE_RDMA0_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DCSR_T_MASK) | (((uint32_t)val) << PIE_RDMA0_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_RDMA0_DRCSR_RESERVED1_MASK 0xffffc000
#define  PIE_RDMA0_DRCSR_RESERVED1_SHIFT 14
#define  PIE_RDMA0_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCSR_RESERVED1_MASK) >> PIE_RDMA0_DRCSR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCSR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DRCSR_M_MASK 0x3f00
#define  PIE_RDMA0_DRCSR_M_SHIFT 8
#define  PIE_RDMA0_DRCSR_M_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCSR_M_MASK) >> PIE_RDMA0_DRCSR_M_SHIFT)
#define  PIE_RDMA0_DRCSR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCSR_M_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCSR_M_SHIFT))

#define  PIE_RDMA0_DRCSR_RESERVED2_MASK 0xc0
#define  PIE_RDMA0_DRCSR_RESERVED2_SHIFT 6
#define  PIE_RDMA0_DRCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCSR_RESERVED2_MASK) >> PIE_RDMA0_DRCSR_RESERVED2_SHIFT)
#define  PIE_RDMA0_DRCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCSR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCSR_RESERVED2_SHIFT))

#define  PIE_RDMA0_DRCSR_L_MASK 0x3f
#define  PIE_RDMA0_DRCSR_L_SHIFT 0
#define  PIE_RDMA0_DRCSR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRCSR_L_MASK) >> PIE_RDMA0_DRCSR_L_SHIFT)
#define  PIE_RDMA0_DRCSR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRCSR_L_MASK) | (((uint32_t)val) << PIE_RDMA0_DRCSR_L_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_RDMA0_DSR_C_MASK 0x80000000
#define  PIE_RDMA0_DSR_C_SHIFT 31
#define  PIE_RDMA0_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DSR_C_MASK) >> PIE_RDMA0_DSR_C_SHIFT)
#define  PIE_RDMA0_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DSR_C_MASK) | (((uint32_t)val) << PIE_RDMA0_DSR_C_SHIFT))

#define  PIE_RDMA0_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_RDMA0_DSR_RESERVED1_SHIFT 2
#define  PIE_RDMA0_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DSR_RESERVED1_MASK) >> PIE_RDMA0_DSR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DSR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DSR_R_MASK 0x2
#define  PIE_RDMA0_DSR_R_SHIFT 1
#define  PIE_RDMA0_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DSR_R_MASK) >> PIE_RDMA0_DSR_R_SHIFT)
#define  PIE_RDMA0_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DSR_R_MASK) | (((uint32_t)val) << PIE_RDMA0_DSR_R_SHIFT))

#define  PIE_RDMA0_DSR_A_MASK 0x1
#define  PIE_RDMA0_DSR_A_SHIFT 0
#define  PIE_RDMA0_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DSR_A_MASK) >> PIE_RDMA0_DSR_A_SHIFT)
#define  PIE_RDMA0_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DSR_A_MASK) | (((uint32_t)val) << PIE_RDMA0_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_RDMA0_DRR_RESERVED1_MASK 0xff000000
#define  PIE_RDMA0_DRR_RESERVED1_SHIFT 24
#define  PIE_RDMA0_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRR_RESERVED1_MASK) >> PIE_RDMA0_DRR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DRR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DRR_T_MASK 0x800000
#define  PIE_RDMA0_DRR_T_SHIFT 23
#define  PIE_RDMA0_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRR_T_MASK) >> PIE_RDMA0_DRR_T_SHIFT)
#define  PIE_RDMA0_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRR_T_MASK) | (((uint32_t)val) << PIE_RDMA0_DRR_T_SHIFT))

#define  PIE_RDMA0_DRR_E_MASK 0x400000
#define  PIE_RDMA0_DRR_E_SHIFT 22
#define  PIE_RDMA0_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRR_E_MASK) >> PIE_RDMA0_DRR_E_SHIFT)
#define  PIE_RDMA0_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRR_E_MASK) | (((uint32_t)val) << PIE_RDMA0_DRR_E_SHIFT))

#define  PIE_RDMA0_DRR_RESERVED2_MASK 0x300000
#define  PIE_RDMA0_DRR_RESERVED2_SHIFT 20
#define  PIE_RDMA0_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRR_RESERVED2_MASK) >> PIE_RDMA0_DRR_RESERVED2_SHIFT)
#define  PIE_RDMA0_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA0_DRR_RESERVED2_SHIFT))

#define  PIE_RDMA0_DRR_N_MASK 0xfffff
#define  PIE_RDMA0_DRR_N_SHIFT 0
#define  PIE_RDMA0_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DRR_N_MASK) >> PIE_RDMA0_DRR_N_SHIFT)
#define  PIE_RDMA0_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DRR_N_MASK) | (((uint32_t)val) << PIE_RDMA0_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_RDMA0_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_RDMA0_DIR_RESERVED1_SHIFT 1
#define  PIE_RDMA0_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DIR_RESERVED1_MASK) >> PIE_RDMA0_DIR_RESERVED1_SHIFT)
#define  PIE_RDMA0_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA0_DIR_RESERVED1_SHIFT))

#define  PIE_RDMA0_DIR_C_MASK 0x1
#define  PIE_RDMA0_DIR_C_SHIFT 0
#define  PIE_RDMA0_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA0_DIR_C_MASK) >> PIE_RDMA0_DIR_C_SHIFT)
#define  PIE_RDMA0_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA0_DIR_C_MASK) | (((uint32_t)val) << PIE_RDMA0_DIR_C_SHIFT))

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 2 of 6) (PIE_RDMA1)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

//====================================================================
//Register: PIE Read DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_RDMA1_DCR_C_MASK 0x80000000
#define  PIE_RDMA1_DCR_C_SHIFT 31
#define  PIE_RDMA1_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCR_C_MASK) >> PIE_RDMA1_DCR_C_SHIFT)
#define  PIE_RDMA1_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCR_C_MASK) | (((uint32_t)val) << PIE_RDMA1_DCR_C_SHIFT))

#define  PIE_RDMA1_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_RDMA1_DCR_RESERVED1_SHIFT 5
#define  PIE_RDMA1_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCR_RESERVED1_MASK) >> PIE_RDMA1_DCR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DCR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DCR_B_MASK 0x10
#define  PIE_RDMA1_DCR_B_SHIFT 4
#define  PIE_RDMA1_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCR_B_MASK) >> PIE_RDMA1_DCR_B_SHIFT)
#define  PIE_RDMA1_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCR_B_MASK) | (((uint32_t)val) << PIE_RDMA1_DCR_B_SHIFT))

#define  PIE_RDMA1_DCR_RESERVED2_MASK 0xc
#define  PIE_RDMA1_DCR_RESERVED2_SHIFT 2
#define  PIE_RDMA1_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCR_RESERVED2_MASK) >> PIE_RDMA1_DCR_RESERVED2_SHIFT)
#define  PIE_RDMA1_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA1_DCR_RESERVED2_SHIFT))

#define  PIE_RDMA1_DCR_R_MASK 0x2
#define  PIE_RDMA1_DCR_R_SHIFT 1
#define  PIE_RDMA1_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCR_R_MASK) >> PIE_RDMA1_DCR_R_SHIFT)
#define  PIE_RDMA1_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCR_R_MASK) | (((uint32_t)val) << PIE_RDMA1_DCR_R_SHIFT))

#define  PIE_RDMA1_DCR_E_MASK 0x1
#define  PIE_RDMA1_DCR_E_SHIFT 0
#define  PIE_RDMA1_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCR_E_MASK) >> PIE_RDMA1_DCR_E_SHIFT)
#define  PIE_RDMA1_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCR_E_MASK) | (((uint32_t)val) << PIE_RDMA1_DCR_E_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 1 (DAR1)
/** \brief DMA Channel Starting Word Address Register 1*/
//====================================================================

#define  PIE_RDMA1_DAR1_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA1_DAR1_ADDRESS_SHIFT 4
#define  PIE_RDMA1_DAR1_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DAR1_ADDRESS_MASK) >> PIE_RDMA1_DAR1_ADDRESS_SHIFT)
#define  PIE_RDMA1_DAR1_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DAR1_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA1_DAR1_ADDRESS_SHIFT))

#define  PIE_RDMA1_DAR1_RESERVED1_MASK 0xf
#define  PIE_RDMA1_DAR1_RESERVED1_SHIFT 0
#define  PIE_RDMA1_DAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DAR1_RESERVED1_MASK) >> PIE_RDMA1_DAR1_RESERVED1_SHIFT)
#define  PIE_RDMA1_DAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DAR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DAR1_RESERVED1_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 2 (DAR2)
/** \brief DMA Channel Starting Word Address Register 2*/
//====================================================================

#define  PIE_RDMA1_DAR2_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA1_DAR2_ADDRESS_SHIFT 4
#define  PIE_RDMA1_DAR2_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DAR2_ADDRESS_MASK) >> PIE_RDMA1_DAR2_ADDRESS_SHIFT)
#define  PIE_RDMA1_DAR2_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DAR2_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA1_DAR2_ADDRESS_SHIFT))

#define  PIE_RDMA1_DAR2_RESERVED1_MASK 0xf
#define  PIE_RDMA1_DAR2_RESERVED1_SHIFT 0
#define  PIE_RDMA1_DAR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DAR2_RESERVED1_MASK) >> PIE_RDMA1_DAR2_RESERVED1_SHIFT)
#define  PIE_RDMA1_DAR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DAR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DAR2_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register1 (DRCR1)
/** \brief Row count in lines for Address 1*/
//====================================================================

#define  PIE_RDMA1_DRCR1_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA1_DRCR1_RESERVED1_SHIFT 6
#define  PIE_RDMA1_DRCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCR1_RESERVED1_MASK) >> PIE_RDMA1_DRCR1_RESERVED1_SHIFT)
#define  PIE_RDMA1_DRCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCR1_RESERVED1_SHIFT))

#define  PIE_RDMA1_DRCR1_T_MASK 0x3f
#define  PIE_RDMA1_DRCR1_T_SHIFT 0
#define  PIE_RDMA1_DRCR1_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCR1_T_MASK) >> PIE_RDMA1_DRCR1_T_SHIFT)
#define  PIE_RDMA1_DRCR1_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCR1_T_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCR1_T_SHIFT))

//====================================================================
//Register: DMA Row Count Register2 (DRCR2)
/** \brief Row count in lines for Address 2*/
//====================================================================

#define  PIE_RDMA1_DRCR2_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA1_DRCR2_RESERVED1_SHIFT 6
#define  PIE_RDMA1_DRCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCR2_RESERVED1_MASK) >> PIE_RDMA1_DRCR2_RESERVED1_SHIFT)
#define  PIE_RDMA1_DRCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCR2_RESERVED1_SHIFT))

#define  PIE_RDMA1_DRCR2_T_MASK 0x3f
#define  PIE_RDMA1_DRCR2_T_SHIFT 0
#define  PIE_RDMA1_DRCR2_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCR2_T_MASK) >> PIE_RDMA1_DRCR2_T_SHIFT)
#define  PIE_RDMA1_DRCR2_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCR2_T_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCR2_T_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_RDMA1_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_RDMA1_DBCR_RESERVED1_SHIFT 18
#define  PIE_RDMA1_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DBCR_RESERVED1_MASK) >> PIE_RDMA1_DBCR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DBCR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DBCR_L_MASK 0x3fff0
#define  PIE_RDMA1_DBCR_L_SHIFT 4
#define  PIE_RDMA1_DBCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DBCR_L_MASK) >> PIE_RDMA1_DBCR_L_SHIFT)
#define  PIE_RDMA1_DBCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DBCR_L_MASK) | (((uint32_t)val) << PIE_RDMA1_DBCR_L_SHIFT))

#define  PIE_RDMA1_DBCR_RESERVED2_MASK 0xf
#define  PIE_RDMA1_DBCR_RESERVED2_SHIFT 0
#define  PIE_RDMA1_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DBCR_RESERVED2_MASK) >> PIE_RDMA1_DBCR_RESERVED2_SHIFT)
#define  PIE_RDMA1_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA1_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_RDMA1_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA1_DTCR_RESERVED1_SHIFT 20
#define  PIE_RDMA1_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DTCR_RESERVED1_MASK) >> PIE_RDMA1_DTCR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DTCR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DTCR_L_MASK 0xffff0
#define  PIE_RDMA1_DTCR_L_SHIFT 4
#define  PIE_RDMA1_DTCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DTCR_L_MASK) >> PIE_RDMA1_DTCR_L_SHIFT)
#define  PIE_RDMA1_DTCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DTCR_L_MASK) | (((uint32_t)val) << PIE_RDMA1_DTCR_L_SHIFT))

#define  PIE_RDMA1_DTCR_RESERVED2_MASK 0xf
#define  PIE_RDMA1_DTCR_RESERVED2_SHIFT 0
#define  PIE_RDMA1_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DTCR_RESERVED2_MASK) >> PIE_RDMA1_DTCR_RESERVED2_SHIFT)
#define  PIE_RDMA1_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA1_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_RDMA1_DSTR_RESERVED1_MASK 0xffffffff
#define  PIE_RDMA1_DSTR_RESERVED1_SHIFT 0
#define  PIE_RDMA1_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DSTR_RESERVED1_MASK) >> PIE_RDMA1_DSTR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DSTR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DSTR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_RDMA1_DASR_A_MASK 0xfffffffc
#define  PIE_RDMA1_DASR_A_SHIFT 2
#define  PIE_RDMA1_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DASR_A_MASK) >> PIE_RDMA1_DASR_A_SHIFT)
#define  PIE_RDMA1_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DASR_A_MASK) | (((uint32_t)val) << PIE_RDMA1_DASR_A_SHIFT))

#define  PIE_RDMA1_DASR_RESERVED1_MASK 0x3
#define  PIE_RDMA1_DASR_RESERVED1_SHIFT 0
#define  PIE_RDMA1_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DASR_RESERVED1_MASK) >> PIE_RDMA1_DASR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_RDMA1_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA1_DCSR_RESERVED1_SHIFT 20
#define  PIE_RDMA1_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCSR_RESERVED1_MASK) >> PIE_RDMA1_DCSR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DCSR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DCSR_T_MASK 0xfffff
#define  PIE_RDMA1_DCSR_T_SHIFT 0
#define  PIE_RDMA1_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DCSR_T_MASK) >> PIE_RDMA1_DCSR_T_SHIFT)
#define  PIE_RDMA1_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DCSR_T_MASK) | (((uint32_t)val) << PIE_RDMA1_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_RDMA1_DRCSR_RESERVED1_MASK 0xffffc000
#define  PIE_RDMA1_DRCSR_RESERVED1_SHIFT 14
#define  PIE_RDMA1_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCSR_RESERVED1_MASK) >> PIE_RDMA1_DRCSR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCSR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DRCSR_M_MASK 0x3f00
#define  PIE_RDMA1_DRCSR_M_SHIFT 8
#define  PIE_RDMA1_DRCSR_M_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCSR_M_MASK) >> PIE_RDMA1_DRCSR_M_SHIFT)
#define  PIE_RDMA1_DRCSR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCSR_M_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCSR_M_SHIFT))

#define  PIE_RDMA1_DRCSR_RESERVED2_MASK 0xc0
#define  PIE_RDMA1_DRCSR_RESERVED2_SHIFT 6
#define  PIE_RDMA1_DRCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCSR_RESERVED2_MASK) >> PIE_RDMA1_DRCSR_RESERVED2_SHIFT)
#define  PIE_RDMA1_DRCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCSR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCSR_RESERVED2_SHIFT))

#define  PIE_RDMA1_DRCSR_L_MASK 0x3f
#define  PIE_RDMA1_DRCSR_L_SHIFT 0
#define  PIE_RDMA1_DRCSR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRCSR_L_MASK) >> PIE_RDMA1_DRCSR_L_SHIFT)
#define  PIE_RDMA1_DRCSR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRCSR_L_MASK) | (((uint32_t)val) << PIE_RDMA1_DRCSR_L_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_RDMA1_DSR_C_MASK 0x80000000
#define  PIE_RDMA1_DSR_C_SHIFT 31
#define  PIE_RDMA1_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DSR_C_MASK) >> PIE_RDMA1_DSR_C_SHIFT)
#define  PIE_RDMA1_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DSR_C_MASK) | (((uint32_t)val) << PIE_RDMA1_DSR_C_SHIFT))

#define  PIE_RDMA1_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_RDMA1_DSR_RESERVED1_SHIFT 2
#define  PIE_RDMA1_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DSR_RESERVED1_MASK) >> PIE_RDMA1_DSR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DSR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DSR_R_MASK 0x2
#define  PIE_RDMA1_DSR_R_SHIFT 1
#define  PIE_RDMA1_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DSR_R_MASK) >> PIE_RDMA1_DSR_R_SHIFT)
#define  PIE_RDMA1_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DSR_R_MASK) | (((uint32_t)val) << PIE_RDMA1_DSR_R_SHIFT))

#define  PIE_RDMA1_DSR_A_MASK 0x1
#define  PIE_RDMA1_DSR_A_SHIFT 0
#define  PIE_RDMA1_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DSR_A_MASK) >> PIE_RDMA1_DSR_A_SHIFT)
#define  PIE_RDMA1_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DSR_A_MASK) | (((uint32_t)val) << PIE_RDMA1_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_RDMA1_DRR_RESERVED1_MASK 0xff000000
#define  PIE_RDMA1_DRR_RESERVED1_SHIFT 24
#define  PIE_RDMA1_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRR_RESERVED1_MASK) >> PIE_RDMA1_DRR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DRR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DRR_T_MASK 0x800000
#define  PIE_RDMA1_DRR_T_SHIFT 23
#define  PIE_RDMA1_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRR_T_MASK) >> PIE_RDMA1_DRR_T_SHIFT)
#define  PIE_RDMA1_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRR_T_MASK) | (((uint32_t)val) << PIE_RDMA1_DRR_T_SHIFT))

#define  PIE_RDMA1_DRR_E_MASK 0x400000
#define  PIE_RDMA1_DRR_E_SHIFT 22
#define  PIE_RDMA1_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRR_E_MASK) >> PIE_RDMA1_DRR_E_SHIFT)
#define  PIE_RDMA1_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRR_E_MASK) | (((uint32_t)val) << PIE_RDMA1_DRR_E_SHIFT))

#define  PIE_RDMA1_DRR_RESERVED2_MASK 0x300000
#define  PIE_RDMA1_DRR_RESERVED2_SHIFT 20
#define  PIE_RDMA1_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRR_RESERVED2_MASK) >> PIE_RDMA1_DRR_RESERVED2_SHIFT)
#define  PIE_RDMA1_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA1_DRR_RESERVED2_SHIFT))

#define  PIE_RDMA1_DRR_N_MASK 0xfffff
#define  PIE_RDMA1_DRR_N_SHIFT 0
#define  PIE_RDMA1_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DRR_N_MASK) >> PIE_RDMA1_DRR_N_SHIFT)
#define  PIE_RDMA1_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DRR_N_MASK) | (((uint32_t)val) << PIE_RDMA1_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_RDMA1_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_RDMA1_DIR_RESERVED1_SHIFT 1
#define  PIE_RDMA1_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DIR_RESERVED1_MASK) >> PIE_RDMA1_DIR_RESERVED1_SHIFT)
#define  PIE_RDMA1_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA1_DIR_RESERVED1_SHIFT))

#define  PIE_RDMA1_DIR_C_MASK 0x1
#define  PIE_RDMA1_DIR_C_SHIFT 0
#define  PIE_RDMA1_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA1_DIR_C_MASK) >> PIE_RDMA1_DIR_C_SHIFT)
#define  PIE_RDMA1_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA1_DIR_C_MASK) | (((uint32_t)val) << PIE_RDMA1_DIR_C_SHIFT))

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 3 of 6) (PIE_RDMA2)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

//====================================================================
//Register: PIE Read DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_RDMA2_DCR_C_MASK 0x80000000
#define  PIE_RDMA2_DCR_C_SHIFT 31
#define  PIE_RDMA2_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCR_C_MASK) >> PIE_RDMA2_DCR_C_SHIFT)
#define  PIE_RDMA2_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCR_C_MASK) | (((uint32_t)val) << PIE_RDMA2_DCR_C_SHIFT))

#define  PIE_RDMA2_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_RDMA2_DCR_RESERVED1_SHIFT 5
#define  PIE_RDMA2_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCR_RESERVED1_MASK) >> PIE_RDMA2_DCR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DCR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DCR_B_MASK 0x10
#define  PIE_RDMA2_DCR_B_SHIFT 4
#define  PIE_RDMA2_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCR_B_MASK) >> PIE_RDMA2_DCR_B_SHIFT)
#define  PIE_RDMA2_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCR_B_MASK) | (((uint32_t)val) << PIE_RDMA2_DCR_B_SHIFT))

#define  PIE_RDMA2_DCR_RESERVED2_MASK 0xc
#define  PIE_RDMA2_DCR_RESERVED2_SHIFT 2
#define  PIE_RDMA2_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCR_RESERVED2_MASK) >> PIE_RDMA2_DCR_RESERVED2_SHIFT)
#define  PIE_RDMA2_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA2_DCR_RESERVED2_SHIFT))

#define  PIE_RDMA2_DCR_R_MASK 0x2
#define  PIE_RDMA2_DCR_R_SHIFT 1
#define  PIE_RDMA2_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCR_R_MASK) >> PIE_RDMA2_DCR_R_SHIFT)
#define  PIE_RDMA2_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCR_R_MASK) | (((uint32_t)val) << PIE_RDMA2_DCR_R_SHIFT))

#define  PIE_RDMA2_DCR_E_MASK 0x1
#define  PIE_RDMA2_DCR_E_SHIFT 0
#define  PIE_RDMA2_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCR_E_MASK) >> PIE_RDMA2_DCR_E_SHIFT)
#define  PIE_RDMA2_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCR_E_MASK) | (((uint32_t)val) << PIE_RDMA2_DCR_E_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 1 (DAR1)
/** \brief DMA Channel Starting Word Address Register 1*/
//====================================================================

#define  PIE_RDMA2_DAR1_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA2_DAR1_ADDRESS_SHIFT 4
#define  PIE_RDMA2_DAR1_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DAR1_ADDRESS_MASK) >> PIE_RDMA2_DAR1_ADDRESS_SHIFT)
#define  PIE_RDMA2_DAR1_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DAR1_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA2_DAR1_ADDRESS_SHIFT))

#define  PIE_RDMA2_DAR1_RESERVED1_MASK 0xf
#define  PIE_RDMA2_DAR1_RESERVED1_SHIFT 0
#define  PIE_RDMA2_DAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DAR1_RESERVED1_MASK) >> PIE_RDMA2_DAR1_RESERVED1_SHIFT)
#define  PIE_RDMA2_DAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DAR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DAR1_RESERVED1_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 2 (DAR2)
/** \brief DMA Channel Starting Word Address Register 2*/
//====================================================================

#define  PIE_RDMA2_DAR2_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA2_DAR2_ADDRESS_SHIFT 4
#define  PIE_RDMA2_DAR2_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DAR2_ADDRESS_MASK) >> PIE_RDMA2_DAR2_ADDRESS_SHIFT)
#define  PIE_RDMA2_DAR2_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DAR2_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA2_DAR2_ADDRESS_SHIFT))

#define  PIE_RDMA2_DAR2_RESERVED1_MASK 0xf
#define  PIE_RDMA2_DAR2_RESERVED1_SHIFT 0
#define  PIE_RDMA2_DAR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DAR2_RESERVED1_MASK) >> PIE_RDMA2_DAR2_RESERVED1_SHIFT)
#define  PIE_RDMA2_DAR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DAR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DAR2_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register1 (DRCR1)
/** \brief Row count in lines for Address 1*/
//====================================================================

#define  PIE_RDMA2_DRCR1_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA2_DRCR1_RESERVED1_SHIFT 6
#define  PIE_RDMA2_DRCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCR1_RESERVED1_MASK) >> PIE_RDMA2_DRCR1_RESERVED1_SHIFT)
#define  PIE_RDMA2_DRCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCR1_RESERVED1_SHIFT))

#define  PIE_RDMA2_DRCR1_T_MASK 0x3f
#define  PIE_RDMA2_DRCR1_T_SHIFT 0
#define  PIE_RDMA2_DRCR1_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCR1_T_MASK) >> PIE_RDMA2_DRCR1_T_SHIFT)
#define  PIE_RDMA2_DRCR1_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCR1_T_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCR1_T_SHIFT))

//====================================================================
//Register: DMA Row Count Register2 (DRCR2)
/** \brief Row count in lines for Address 2*/
//====================================================================

#define  PIE_RDMA2_DRCR2_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA2_DRCR2_RESERVED1_SHIFT 6
#define  PIE_RDMA2_DRCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCR2_RESERVED1_MASK) >> PIE_RDMA2_DRCR2_RESERVED1_SHIFT)
#define  PIE_RDMA2_DRCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCR2_RESERVED1_SHIFT))

#define  PIE_RDMA2_DRCR2_T_MASK 0x3f
#define  PIE_RDMA2_DRCR2_T_SHIFT 0
#define  PIE_RDMA2_DRCR2_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCR2_T_MASK) >> PIE_RDMA2_DRCR2_T_SHIFT)
#define  PIE_RDMA2_DRCR2_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCR2_T_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCR2_T_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_RDMA2_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_RDMA2_DBCR_RESERVED1_SHIFT 18
#define  PIE_RDMA2_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DBCR_RESERVED1_MASK) >> PIE_RDMA2_DBCR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DBCR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DBCR_L_MASK 0x3fff0
#define  PIE_RDMA2_DBCR_L_SHIFT 4
#define  PIE_RDMA2_DBCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DBCR_L_MASK) >> PIE_RDMA2_DBCR_L_SHIFT)
#define  PIE_RDMA2_DBCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DBCR_L_MASK) | (((uint32_t)val) << PIE_RDMA2_DBCR_L_SHIFT))

#define  PIE_RDMA2_DBCR_RESERVED2_MASK 0xf
#define  PIE_RDMA2_DBCR_RESERVED2_SHIFT 0
#define  PIE_RDMA2_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DBCR_RESERVED2_MASK) >> PIE_RDMA2_DBCR_RESERVED2_SHIFT)
#define  PIE_RDMA2_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA2_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_RDMA2_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA2_DTCR_RESERVED1_SHIFT 20
#define  PIE_RDMA2_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DTCR_RESERVED1_MASK) >> PIE_RDMA2_DTCR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DTCR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DTCR_L_MASK 0xffff0
#define  PIE_RDMA2_DTCR_L_SHIFT 4
#define  PIE_RDMA2_DTCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DTCR_L_MASK) >> PIE_RDMA2_DTCR_L_SHIFT)
#define  PIE_RDMA2_DTCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DTCR_L_MASK) | (((uint32_t)val) << PIE_RDMA2_DTCR_L_SHIFT))

#define  PIE_RDMA2_DTCR_RESERVED2_MASK 0xf
#define  PIE_RDMA2_DTCR_RESERVED2_SHIFT 0
#define  PIE_RDMA2_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DTCR_RESERVED2_MASK) >> PIE_RDMA2_DTCR_RESERVED2_SHIFT)
#define  PIE_RDMA2_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA2_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_RDMA2_DSTR_RESERVED1_MASK 0xffffffff
#define  PIE_RDMA2_DSTR_RESERVED1_SHIFT 0
#define  PIE_RDMA2_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DSTR_RESERVED1_MASK) >> PIE_RDMA2_DSTR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DSTR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DSTR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_RDMA2_DASR_A_MASK 0xfffffffc
#define  PIE_RDMA2_DASR_A_SHIFT 2
#define  PIE_RDMA2_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DASR_A_MASK) >> PIE_RDMA2_DASR_A_SHIFT)
#define  PIE_RDMA2_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DASR_A_MASK) | (((uint32_t)val) << PIE_RDMA2_DASR_A_SHIFT))

#define  PIE_RDMA2_DASR_RESERVED1_MASK 0x3
#define  PIE_RDMA2_DASR_RESERVED1_SHIFT 0
#define  PIE_RDMA2_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DASR_RESERVED1_MASK) >> PIE_RDMA2_DASR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_RDMA2_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA2_DCSR_RESERVED1_SHIFT 20
#define  PIE_RDMA2_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCSR_RESERVED1_MASK) >> PIE_RDMA2_DCSR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DCSR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DCSR_T_MASK 0xfffff
#define  PIE_RDMA2_DCSR_T_SHIFT 0
#define  PIE_RDMA2_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DCSR_T_MASK) >> PIE_RDMA2_DCSR_T_SHIFT)
#define  PIE_RDMA2_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DCSR_T_MASK) | (((uint32_t)val) << PIE_RDMA2_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_RDMA2_DRCSR_RESERVED1_MASK 0xffffc000
#define  PIE_RDMA2_DRCSR_RESERVED1_SHIFT 14
#define  PIE_RDMA2_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCSR_RESERVED1_MASK) >> PIE_RDMA2_DRCSR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCSR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DRCSR_M_MASK 0x3f00
#define  PIE_RDMA2_DRCSR_M_SHIFT 8
#define  PIE_RDMA2_DRCSR_M_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCSR_M_MASK) >> PIE_RDMA2_DRCSR_M_SHIFT)
#define  PIE_RDMA2_DRCSR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCSR_M_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCSR_M_SHIFT))

#define  PIE_RDMA2_DRCSR_RESERVED2_MASK 0xc0
#define  PIE_RDMA2_DRCSR_RESERVED2_SHIFT 6
#define  PIE_RDMA2_DRCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCSR_RESERVED2_MASK) >> PIE_RDMA2_DRCSR_RESERVED2_SHIFT)
#define  PIE_RDMA2_DRCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCSR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCSR_RESERVED2_SHIFT))

#define  PIE_RDMA2_DRCSR_L_MASK 0x3f
#define  PIE_RDMA2_DRCSR_L_SHIFT 0
#define  PIE_RDMA2_DRCSR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRCSR_L_MASK) >> PIE_RDMA2_DRCSR_L_SHIFT)
#define  PIE_RDMA2_DRCSR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRCSR_L_MASK) | (((uint32_t)val) << PIE_RDMA2_DRCSR_L_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_RDMA2_DSR_C_MASK 0x80000000
#define  PIE_RDMA2_DSR_C_SHIFT 31
#define  PIE_RDMA2_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DSR_C_MASK) >> PIE_RDMA2_DSR_C_SHIFT)
#define  PIE_RDMA2_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DSR_C_MASK) | (((uint32_t)val) << PIE_RDMA2_DSR_C_SHIFT))

#define  PIE_RDMA2_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_RDMA2_DSR_RESERVED1_SHIFT 2
#define  PIE_RDMA2_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DSR_RESERVED1_MASK) >> PIE_RDMA2_DSR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DSR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DSR_R_MASK 0x2
#define  PIE_RDMA2_DSR_R_SHIFT 1
#define  PIE_RDMA2_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DSR_R_MASK) >> PIE_RDMA2_DSR_R_SHIFT)
#define  PIE_RDMA2_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DSR_R_MASK) | (((uint32_t)val) << PIE_RDMA2_DSR_R_SHIFT))

#define  PIE_RDMA2_DSR_A_MASK 0x1
#define  PIE_RDMA2_DSR_A_SHIFT 0
#define  PIE_RDMA2_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DSR_A_MASK) >> PIE_RDMA2_DSR_A_SHIFT)
#define  PIE_RDMA2_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DSR_A_MASK) | (((uint32_t)val) << PIE_RDMA2_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_RDMA2_DRR_RESERVED1_MASK 0xff000000
#define  PIE_RDMA2_DRR_RESERVED1_SHIFT 24
#define  PIE_RDMA2_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRR_RESERVED1_MASK) >> PIE_RDMA2_DRR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DRR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DRR_T_MASK 0x800000
#define  PIE_RDMA2_DRR_T_SHIFT 23
#define  PIE_RDMA2_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRR_T_MASK) >> PIE_RDMA2_DRR_T_SHIFT)
#define  PIE_RDMA2_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRR_T_MASK) | (((uint32_t)val) << PIE_RDMA2_DRR_T_SHIFT))

#define  PIE_RDMA2_DRR_E_MASK 0x400000
#define  PIE_RDMA2_DRR_E_SHIFT 22
#define  PIE_RDMA2_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRR_E_MASK) >> PIE_RDMA2_DRR_E_SHIFT)
#define  PIE_RDMA2_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRR_E_MASK) | (((uint32_t)val) << PIE_RDMA2_DRR_E_SHIFT))

#define  PIE_RDMA2_DRR_RESERVED2_MASK 0x300000
#define  PIE_RDMA2_DRR_RESERVED2_SHIFT 20
#define  PIE_RDMA2_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRR_RESERVED2_MASK) >> PIE_RDMA2_DRR_RESERVED2_SHIFT)
#define  PIE_RDMA2_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA2_DRR_RESERVED2_SHIFT))

#define  PIE_RDMA2_DRR_N_MASK 0xfffff
#define  PIE_RDMA2_DRR_N_SHIFT 0
#define  PIE_RDMA2_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DRR_N_MASK) >> PIE_RDMA2_DRR_N_SHIFT)
#define  PIE_RDMA2_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DRR_N_MASK) | (((uint32_t)val) << PIE_RDMA2_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_RDMA2_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_RDMA2_DIR_RESERVED1_SHIFT 1
#define  PIE_RDMA2_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DIR_RESERVED1_MASK) >> PIE_RDMA2_DIR_RESERVED1_SHIFT)
#define  PIE_RDMA2_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA2_DIR_RESERVED1_SHIFT))

#define  PIE_RDMA2_DIR_C_MASK 0x1
#define  PIE_RDMA2_DIR_C_SHIFT 0
#define  PIE_RDMA2_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA2_DIR_C_MASK) >> PIE_RDMA2_DIR_C_SHIFT)
#define  PIE_RDMA2_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA2_DIR_C_MASK) | (((uint32_t)val) << PIE_RDMA2_DIR_C_SHIFT))

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 4 of 6) (PIE_RDMA3)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

//====================================================================
//Register: PIE Read DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_RDMA3_DCR_C_MASK 0x80000000
#define  PIE_RDMA3_DCR_C_SHIFT 31
#define  PIE_RDMA3_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCR_C_MASK) >> PIE_RDMA3_DCR_C_SHIFT)
#define  PIE_RDMA3_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCR_C_MASK) | (((uint32_t)val) << PIE_RDMA3_DCR_C_SHIFT))

#define  PIE_RDMA3_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_RDMA3_DCR_RESERVED1_SHIFT 5
#define  PIE_RDMA3_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCR_RESERVED1_MASK) >> PIE_RDMA3_DCR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DCR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DCR_B_MASK 0x10
#define  PIE_RDMA3_DCR_B_SHIFT 4
#define  PIE_RDMA3_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCR_B_MASK) >> PIE_RDMA3_DCR_B_SHIFT)
#define  PIE_RDMA3_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCR_B_MASK) | (((uint32_t)val) << PIE_RDMA3_DCR_B_SHIFT))

#define  PIE_RDMA3_DCR_RESERVED2_MASK 0xc
#define  PIE_RDMA3_DCR_RESERVED2_SHIFT 2
#define  PIE_RDMA3_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCR_RESERVED2_MASK) >> PIE_RDMA3_DCR_RESERVED2_SHIFT)
#define  PIE_RDMA3_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA3_DCR_RESERVED2_SHIFT))

#define  PIE_RDMA3_DCR_R_MASK 0x2
#define  PIE_RDMA3_DCR_R_SHIFT 1
#define  PIE_RDMA3_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCR_R_MASK) >> PIE_RDMA3_DCR_R_SHIFT)
#define  PIE_RDMA3_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCR_R_MASK) | (((uint32_t)val) << PIE_RDMA3_DCR_R_SHIFT))

#define  PIE_RDMA3_DCR_E_MASK 0x1
#define  PIE_RDMA3_DCR_E_SHIFT 0
#define  PIE_RDMA3_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCR_E_MASK) >> PIE_RDMA3_DCR_E_SHIFT)
#define  PIE_RDMA3_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCR_E_MASK) | (((uint32_t)val) << PIE_RDMA3_DCR_E_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 1 (DAR1)
/** \brief DMA Channel Starting Word Address Register 1*/
//====================================================================

#define  PIE_RDMA3_DAR1_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA3_DAR1_ADDRESS_SHIFT 4
#define  PIE_RDMA3_DAR1_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DAR1_ADDRESS_MASK) >> PIE_RDMA3_DAR1_ADDRESS_SHIFT)
#define  PIE_RDMA3_DAR1_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DAR1_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA3_DAR1_ADDRESS_SHIFT))

#define  PIE_RDMA3_DAR1_RESERVED1_MASK 0xf
#define  PIE_RDMA3_DAR1_RESERVED1_SHIFT 0
#define  PIE_RDMA3_DAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DAR1_RESERVED1_MASK) >> PIE_RDMA3_DAR1_RESERVED1_SHIFT)
#define  PIE_RDMA3_DAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DAR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DAR1_RESERVED1_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 2 (DAR2)
/** \brief DMA Channel Starting Word Address Register 2*/
//====================================================================

#define  PIE_RDMA3_DAR2_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA3_DAR2_ADDRESS_SHIFT 4
#define  PIE_RDMA3_DAR2_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DAR2_ADDRESS_MASK) >> PIE_RDMA3_DAR2_ADDRESS_SHIFT)
#define  PIE_RDMA3_DAR2_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DAR2_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA3_DAR2_ADDRESS_SHIFT))

#define  PIE_RDMA3_DAR2_RESERVED1_MASK 0xf
#define  PIE_RDMA3_DAR2_RESERVED1_SHIFT 0
#define  PIE_RDMA3_DAR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DAR2_RESERVED1_MASK) >> PIE_RDMA3_DAR2_RESERVED1_SHIFT)
#define  PIE_RDMA3_DAR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DAR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DAR2_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register1 (DRCR1)
/** \brief Row count in lines for Address 1*/
//====================================================================

#define  PIE_RDMA3_DRCR1_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA3_DRCR1_RESERVED1_SHIFT 6
#define  PIE_RDMA3_DRCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCR1_RESERVED1_MASK) >> PIE_RDMA3_DRCR1_RESERVED1_SHIFT)
#define  PIE_RDMA3_DRCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCR1_RESERVED1_SHIFT))

#define  PIE_RDMA3_DRCR1_T_MASK 0x3f
#define  PIE_RDMA3_DRCR1_T_SHIFT 0
#define  PIE_RDMA3_DRCR1_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCR1_T_MASK) >> PIE_RDMA3_DRCR1_T_SHIFT)
#define  PIE_RDMA3_DRCR1_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCR1_T_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCR1_T_SHIFT))

//====================================================================
//Register: DMA Row Count Register2 (DRCR2)
/** \brief Row count in lines for Address 2*/
//====================================================================

#define  PIE_RDMA3_DRCR2_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA3_DRCR2_RESERVED1_SHIFT 6
#define  PIE_RDMA3_DRCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCR2_RESERVED1_MASK) >> PIE_RDMA3_DRCR2_RESERVED1_SHIFT)
#define  PIE_RDMA3_DRCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCR2_RESERVED1_SHIFT))

#define  PIE_RDMA3_DRCR2_T_MASK 0x3f
#define  PIE_RDMA3_DRCR2_T_SHIFT 0
#define  PIE_RDMA3_DRCR2_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCR2_T_MASK) >> PIE_RDMA3_DRCR2_T_SHIFT)
#define  PIE_RDMA3_DRCR2_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCR2_T_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCR2_T_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_RDMA3_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_RDMA3_DBCR_RESERVED1_SHIFT 18
#define  PIE_RDMA3_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DBCR_RESERVED1_MASK) >> PIE_RDMA3_DBCR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DBCR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DBCR_L_MASK 0x3fff0
#define  PIE_RDMA3_DBCR_L_SHIFT 4
#define  PIE_RDMA3_DBCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DBCR_L_MASK) >> PIE_RDMA3_DBCR_L_SHIFT)
#define  PIE_RDMA3_DBCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DBCR_L_MASK) | (((uint32_t)val) << PIE_RDMA3_DBCR_L_SHIFT))

#define  PIE_RDMA3_DBCR_RESERVED2_MASK 0xf
#define  PIE_RDMA3_DBCR_RESERVED2_SHIFT 0
#define  PIE_RDMA3_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DBCR_RESERVED2_MASK) >> PIE_RDMA3_DBCR_RESERVED2_SHIFT)
#define  PIE_RDMA3_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA3_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_RDMA3_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA3_DTCR_RESERVED1_SHIFT 20
#define  PIE_RDMA3_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DTCR_RESERVED1_MASK) >> PIE_RDMA3_DTCR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DTCR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DTCR_L_MASK 0xffff0
#define  PIE_RDMA3_DTCR_L_SHIFT 4
#define  PIE_RDMA3_DTCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DTCR_L_MASK) >> PIE_RDMA3_DTCR_L_SHIFT)
#define  PIE_RDMA3_DTCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DTCR_L_MASK) | (((uint32_t)val) << PIE_RDMA3_DTCR_L_SHIFT))

#define  PIE_RDMA3_DTCR_RESERVED2_MASK 0xf
#define  PIE_RDMA3_DTCR_RESERVED2_SHIFT 0
#define  PIE_RDMA3_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DTCR_RESERVED2_MASK) >> PIE_RDMA3_DTCR_RESERVED2_SHIFT)
#define  PIE_RDMA3_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA3_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_RDMA3_DSTR_RESERVED1_MASK 0xffffffff
#define  PIE_RDMA3_DSTR_RESERVED1_SHIFT 0
#define  PIE_RDMA3_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DSTR_RESERVED1_MASK) >> PIE_RDMA3_DSTR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DSTR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DSTR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_RDMA3_DASR_A_MASK 0xfffffffc
#define  PIE_RDMA3_DASR_A_SHIFT 2
#define  PIE_RDMA3_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DASR_A_MASK) >> PIE_RDMA3_DASR_A_SHIFT)
#define  PIE_RDMA3_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DASR_A_MASK) | (((uint32_t)val) << PIE_RDMA3_DASR_A_SHIFT))

#define  PIE_RDMA3_DASR_RESERVED1_MASK 0x3
#define  PIE_RDMA3_DASR_RESERVED1_SHIFT 0
#define  PIE_RDMA3_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DASR_RESERVED1_MASK) >> PIE_RDMA3_DASR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_RDMA3_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA3_DCSR_RESERVED1_SHIFT 20
#define  PIE_RDMA3_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCSR_RESERVED1_MASK) >> PIE_RDMA3_DCSR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DCSR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DCSR_T_MASK 0xfffff
#define  PIE_RDMA3_DCSR_T_SHIFT 0
#define  PIE_RDMA3_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DCSR_T_MASK) >> PIE_RDMA3_DCSR_T_SHIFT)
#define  PIE_RDMA3_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DCSR_T_MASK) | (((uint32_t)val) << PIE_RDMA3_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_RDMA3_DRCSR_RESERVED1_MASK 0xffffc000
#define  PIE_RDMA3_DRCSR_RESERVED1_SHIFT 14
#define  PIE_RDMA3_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCSR_RESERVED1_MASK) >> PIE_RDMA3_DRCSR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCSR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DRCSR_M_MASK 0x3f00
#define  PIE_RDMA3_DRCSR_M_SHIFT 8
#define  PIE_RDMA3_DRCSR_M_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCSR_M_MASK) >> PIE_RDMA3_DRCSR_M_SHIFT)
#define  PIE_RDMA3_DRCSR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCSR_M_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCSR_M_SHIFT))

#define  PIE_RDMA3_DRCSR_RESERVED2_MASK 0xc0
#define  PIE_RDMA3_DRCSR_RESERVED2_SHIFT 6
#define  PIE_RDMA3_DRCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCSR_RESERVED2_MASK) >> PIE_RDMA3_DRCSR_RESERVED2_SHIFT)
#define  PIE_RDMA3_DRCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCSR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCSR_RESERVED2_SHIFT))

#define  PIE_RDMA3_DRCSR_L_MASK 0x3f
#define  PIE_RDMA3_DRCSR_L_SHIFT 0
#define  PIE_RDMA3_DRCSR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRCSR_L_MASK) >> PIE_RDMA3_DRCSR_L_SHIFT)
#define  PIE_RDMA3_DRCSR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRCSR_L_MASK) | (((uint32_t)val) << PIE_RDMA3_DRCSR_L_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_RDMA3_DSR_C_MASK 0x80000000
#define  PIE_RDMA3_DSR_C_SHIFT 31
#define  PIE_RDMA3_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DSR_C_MASK) >> PIE_RDMA3_DSR_C_SHIFT)
#define  PIE_RDMA3_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DSR_C_MASK) | (((uint32_t)val) << PIE_RDMA3_DSR_C_SHIFT))

#define  PIE_RDMA3_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_RDMA3_DSR_RESERVED1_SHIFT 2
#define  PIE_RDMA3_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DSR_RESERVED1_MASK) >> PIE_RDMA3_DSR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DSR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DSR_R_MASK 0x2
#define  PIE_RDMA3_DSR_R_SHIFT 1
#define  PIE_RDMA3_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DSR_R_MASK) >> PIE_RDMA3_DSR_R_SHIFT)
#define  PIE_RDMA3_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DSR_R_MASK) | (((uint32_t)val) << PIE_RDMA3_DSR_R_SHIFT))

#define  PIE_RDMA3_DSR_A_MASK 0x1
#define  PIE_RDMA3_DSR_A_SHIFT 0
#define  PIE_RDMA3_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DSR_A_MASK) >> PIE_RDMA3_DSR_A_SHIFT)
#define  PIE_RDMA3_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DSR_A_MASK) | (((uint32_t)val) << PIE_RDMA3_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_RDMA3_DRR_RESERVED1_MASK 0xff000000
#define  PIE_RDMA3_DRR_RESERVED1_SHIFT 24
#define  PIE_RDMA3_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRR_RESERVED1_MASK) >> PIE_RDMA3_DRR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DRR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DRR_T_MASK 0x800000
#define  PIE_RDMA3_DRR_T_SHIFT 23
#define  PIE_RDMA3_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRR_T_MASK) >> PIE_RDMA3_DRR_T_SHIFT)
#define  PIE_RDMA3_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRR_T_MASK) | (((uint32_t)val) << PIE_RDMA3_DRR_T_SHIFT))

#define  PIE_RDMA3_DRR_E_MASK 0x400000
#define  PIE_RDMA3_DRR_E_SHIFT 22
#define  PIE_RDMA3_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRR_E_MASK) >> PIE_RDMA3_DRR_E_SHIFT)
#define  PIE_RDMA3_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRR_E_MASK) | (((uint32_t)val) << PIE_RDMA3_DRR_E_SHIFT))

#define  PIE_RDMA3_DRR_RESERVED2_MASK 0x300000
#define  PIE_RDMA3_DRR_RESERVED2_SHIFT 20
#define  PIE_RDMA3_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRR_RESERVED2_MASK) >> PIE_RDMA3_DRR_RESERVED2_SHIFT)
#define  PIE_RDMA3_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA3_DRR_RESERVED2_SHIFT))

#define  PIE_RDMA3_DRR_N_MASK 0xfffff
#define  PIE_RDMA3_DRR_N_SHIFT 0
#define  PIE_RDMA3_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DRR_N_MASK) >> PIE_RDMA3_DRR_N_SHIFT)
#define  PIE_RDMA3_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DRR_N_MASK) | (((uint32_t)val) << PIE_RDMA3_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_RDMA3_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_RDMA3_DIR_RESERVED1_SHIFT 1
#define  PIE_RDMA3_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DIR_RESERVED1_MASK) >> PIE_RDMA3_DIR_RESERVED1_SHIFT)
#define  PIE_RDMA3_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA3_DIR_RESERVED1_SHIFT))

#define  PIE_RDMA3_DIR_C_MASK 0x1
#define  PIE_RDMA3_DIR_C_SHIFT 0
#define  PIE_RDMA3_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA3_DIR_C_MASK) >> PIE_RDMA3_DIR_C_SHIFT)
#define  PIE_RDMA3_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA3_DIR_C_MASK) | (((uint32_t)val) << PIE_RDMA3_DIR_C_SHIFT))

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 5 of 6) (PIE_RDMA4)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

//====================================================================
//Register: PIE Read DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_RDMA4_DCR_C_MASK 0x80000000
#define  PIE_RDMA4_DCR_C_SHIFT 31
#define  PIE_RDMA4_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCR_C_MASK) >> PIE_RDMA4_DCR_C_SHIFT)
#define  PIE_RDMA4_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCR_C_MASK) | (((uint32_t)val) << PIE_RDMA4_DCR_C_SHIFT))

#define  PIE_RDMA4_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_RDMA4_DCR_RESERVED1_SHIFT 5
#define  PIE_RDMA4_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCR_RESERVED1_MASK) >> PIE_RDMA4_DCR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DCR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DCR_B_MASK 0x10
#define  PIE_RDMA4_DCR_B_SHIFT 4
#define  PIE_RDMA4_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCR_B_MASK) >> PIE_RDMA4_DCR_B_SHIFT)
#define  PIE_RDMA4_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCR_B_MASK) | (((uint32_t)val) << PIE_RDMA4_DCR_B_SHIFT))

#define  PIE_RDMA4_DCR_RESERVED2_MASK 0xc
#define  PIE_RDMA4_DCR_RESERVED2_SHIFT 2
#define  PIE_RDMA4_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCR_RESERVED2_MASK) >> PIE_RDMA4_DCR_RESERVED2_SHIFT)
#define  PIE_RDMA4_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA4_DCR_RESERVED2_SHIFT))

#define  PIE_RDMA4_DCR_R_MASK 0x2
#define  PIE_RDMA4_DCR_R_SHIFT 1
#define  PIE_RDMA4_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCR_R_MASK) >> PIE_RDMA4_DCR_R_SHIFT)
#define  PIE_RDMA4_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCR_R_MASK) | (((uint32_t)val) << PIE_RDMA4_DCR_R_SHIFT))

#define  PIE_RDMA4_DCR_E_MASK 0x1
#define  PIE_RDMA4_DCR_E_SHIFT 0
#define  PIE_RDMA4_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCR_E_MASK) >> PIE_RDMA4_DCR_E_SHIFT)
#define  PIE_RDMA4_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCR_E_MASK) | (((uint32_t)val) << PIE_RDMA4_DCR_E_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 1 (DAR1)
/** \brief DMA Channel Starting Word Address Register 1*/
//====================================================================

#define  PIE_RDMA4_DAR1_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA4_DAR1_ADDRESS_SHIFT 4
#define  PIE_RDMA4_DAR1_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DAR1_ADDRESS_MASK) >> PIE_RDMA4_DAR1_ADDRESS_SHIFT)
#define  PIE_RDMA4_DAR1_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DAR1_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA4_DAR1_ADDRESS_SHIFT))

#define  PIE_RDMA4_DAR1_RESERVED1_MASK 0xf
#define  PIE_RDMA4_DAR1_RESERVED1_SHIFT 0
#define  PIE_RDMA4_DAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DAR1_RESERVED1_MASK) >> PIE_RDMA4_DAR1_RESERVED1_SHIFT)
#define  PIE_RDMA4_DAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DAR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DAR1_RESERVED1_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 2 (DAR2)
/** \brief DMA Channel Starting Word Address Register 2*/
//====================================================================

#define  PIE_RDMA4_DAR2_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA4_DAR2_ADDRESS_SHIFT 4
#define  PIE_RDMA4_DAR2_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DAR2_ADDRESS_MASK) >> PIE_RDMA4_DAR2_ADDRESS_SHIFT)
#define  PIE_RDMA4_DAR2_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DAR2_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA4_DAR2_ADDRESS_SHIFT))

#define  PIE_RDMA4_DAR2_RESERVED1_MASK 0xf
#define  PIE_RDMA4_DAR2_RESERVED1_SHIFT 0
#define  PIE_RDMA4_DAR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DAR2_RESERVED1_MASK) >> PIE_RDMA4_DAR2_RESERVED1_SHIFT)
#define  PIE_RDMA4_DAR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DAR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DAR2_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register1 (DRCR1)
/** \brief Row count in lines for Address 1*/
//====================================================================

#define  PIE_RDMA4_DRCR1_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA4_DRCR1_RESERVED1_SHIFT 6
#define  PIE_RDMA4_DRCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCR1_RESERVED1_MASK) >> PIE_RDMA4_DRCR1_RESERVED1_SHIFT)
#define  PIE_RDMA4_DRCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCR1_RESERVED1_SHIFT))

#define  PIE_RDMA4_DRCR1_T_MASK 0x3f
#define  PIE_RDMA4_DRCR1_T_SHIFT 0
#define  PIE_RDMA4_DRCR1_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCR1_T_MASK) >> PIE_RDMA4_DRCR1_T_SHIFT)
#define  PIE_RDMA4_DRCR1_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCR1_T_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCR1_T_SHIFT))

//====================================================================
//Register: DMA Row Count Register2 (DRCR2)
/** \brief Row count in lines for Address 2*/
//====================================================================

#define  PIE_RDMA4_DRCR2_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA4_DRCR2_RESERVED1_SHIFT 6
#define  PIE_RDMA4_DRCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCR2_RESERVED1_MASK) >> PIE_RDMA4_DRCR2_RESERVED1_SHIFT)
#define  PIE_RDMA4_DRCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCR2_RESERVED1_SHIFT))

#define  PIE_RDMA4_DRCR2_T_MASK 0x3f
#define  PIE_RDMA4_DRCR2_T_SHIFT 0
#define  PIE_RDMA4_DRCR2_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCR2_T_MASK) >> PIE_RDMA4_DRCR2_T_SHIFT)
#define  PIE_RDMA4_DRCR2_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCR2_T_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCR2_T_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_RDMA4_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_RDMA4_DBCR_RESERVED1_SHIFT 18
#define  PIE_RDMA4_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DBCR_RESERVED1_MASK) >> PIE_RDMA4_DBCR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DBCR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DBCR_L_MASK 0x3fff0
#define  PIE_RDMA4_DBCR_L_SHIFT 4
#define  PIE_RDMA4_DBCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DBCR_L_MASK) >> PIE_RDMA4_DBCR_L_SHIFT)
#define  PIE_RDMA4_DBCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DBCR_L_MASK) | (((uint32_t)val) << PIE_RDMA4_DBCR_L_SHIFT))

#define  PIE_RDMA4_DBCR_RESERVED2_MASK 0xf
#define  PIE_RDMA4_DBCR_RESERVED2_SHIFT 0
#define  PIE_RDMA4_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DBCR_RESERVED2_MASK) >> PIE_RDMA4_DBCR_RESERVED2_SHIFT)
#define  PIE_RDMA4_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA4_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_RDMA4_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA4_DTCR_RESERVED1_SHIFT 20
#define  PIE_RDMA4_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DTCR_RESERVED1_MASK) >> PIE_RDMA4_DTCR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DTCR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DTCR_L_MASK 0xffff0
#define  PIE_RDMA4_DTCR_L_SHIFT 4
#define  PIE_RDMA4_DTCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DTCR_L_MASK) >> PIE_RDMA4_DTCR_L_SHIFT)
#define  PIE_RDMA4_DTCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DTCR_L_MASK) | (((uint32_t)val) << PIE_RDMA4_DTCR_L_SHIFT))

#define  PIE_RDMA4_DTCR_RESERVED2_MASK 0xf
#define  PIE_RDMA4_DTCR_RESERVED2_SHIFT 0
#define  PIE_RDMA4_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DTCR_RESERVED2_MASK) >> PIE_RDMA4_DTCR_RESERVED2_SHIFT)
#define  PIE_RDMA4_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA4_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_RDMA4_DSTR_RESERVED1_MASK 0xffffffff
#define  PIE_RDMA4_DSTR_RESERVED1_SHIFT 0
#define  PIE_RDMA4_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DSTR_RESERVED1_MASK) >> PIE_RDMA4_DSTR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DSTR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DSTR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_RDMA4_DASR_A_MASK 0xfffffffc
#define  PIE_RDMA4_DASR_A_SHIFT 2
#define  PIE_RDMA4_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DASR_A_MASK) >> PIE_RDMA4_DASR_A_SHIFT)
#define  PIE_RDMA4_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DASR_A_MASK) | (((uint32_t)val) << PIE_RDMA4_DASR_A_SHIFT))

#define  PIE_RDMA4_DASR_RESERVED1_MASK 0x3
#define  PIE_RDMA4_DASR_RESERVED1_SHIFT 0
#define  PIE_RDMA4_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DASR_RESERVED1_MASK) >> PIE_RDMA4_DASR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_RDMA4_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA4_DCSR_RESERVED1_SHIFT 20
#define  PIE_RDMA4_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCSR_RESERVED1_MASK) >> PIE_RDMA4_DCSR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DCSR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DCSR_T_MASK 0xfffff
#define  PIE_RDMA4_DCSR_T_SHIFT 0
#define  PIE_RDMA4_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DCSR_T_MASK) >> PIE_RDMA4_DCSR_T_SHIFT)
#define  PIE_RDMA4_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DCSR_T_MASK) | (((uint32_t)val) << PIE_RDMA4_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_RDMA4_DRCSR_RESERVED1_MASK 0xffffc000
#define  PIE_RDMA4_DRCSR_RESERVED1_SHIFT 14
#define  PIE_RDMA4_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCSR_RESERVED1_MASK) >> PIE_RDMA4_DRCSR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCSR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DRCSR_M_MASK 0x3f00
#define  PIE_RDMA4_DRCSR_M_SHIFT 8
#define  PIE_RDMA4_DRCSR_M_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCSR_M_MASK) >> PIE_RDMA4_DRCSR_M_SHIFT)
#define  PIE_RDMA4_DRCSR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCSR_M_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCSR_M_SHIFT))

#define  PIE_RDMA4_DRCSR_RESERVED2_MASK 0xc0
#define  PIE_RDMA4_DRCSR_RESERVED2_SHIFT 6
#define  PIE_RDMA4_DRCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCSR_RESERVED2_MASK) >> PIE_RDMA4_DRCSR_RESERVED2_SHIFT)
#define  PIE_RDMA4_DRCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCSR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCSR_RESERVED2_SHIFT))

#define  PIE_RDMA4_DRCSR_L_MASK 0x3f
#define  PIE_RDMA4_DRCSR_L_SHIFT 0
#define  PIE_RDMA4_DRCSR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRCSR_L_MASK) >> PIE_RDMA4_DRCSR_L_SHIFT)
#define  PIE_RDMA4_DRCSR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRCSR_L_MASK) | (((uint32_t)val) << PIE_RDMA4_DRCSR_L_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_RDMA4_DSR_C_MASK 0x80000000
#define  PIE_RDMA4_DSR_C_SHIFT 31
#define  PIE_RDMA4_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DSR_C_MASK) >> PIE_RDMA4_DSR_C_SHIFT)
#define  PIE_RDMA4_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DSR_C_MASK) | (((uint32_t)val) << PIE_RDMA4_DSR_C_SHIFT))

#define  PIE_RDMA4_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_RDMA4_DSR_RESERVED1_SHIFT 2
#define  PIE_RDMA4_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DSR_RESERVED1_MASK) >> PIE_RDMA4_DSR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DSR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DSR_R_MASK 0x2
#define  PIE_RDMA4_DSR_R_SHIFT 1
#define  PIE_RDMA4_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DSR_R_MASK) >> PIE_RDMA4_DSR_R_SHIFT)
#define  PIE_RDMA4_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DSR_R_MASK) | (((uint32_t)val) << PIE_RDMA4_DSR_R_SHIFT))

#define  PIE_RDMA4_DSR_A_MASK 0x1
#define  PIE_RDMA4_DSR_A_SHIFT 0
#define  PIE_RDMA4_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DSR_A_MASK) >> PIE_RDMA4_DSR_A_SHIFT)
#define  PIE_RDMA4_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DSR_A_MASK) | (((uint32_t)val) << PIE_RDMA4_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_RDMA4_DRR_RESERVED1_MASK 0xff000000
#define  PIE_RDMA4_DRR_RESERVED1_SHIFT 24
#define  PIE_RDMA4_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRR_RESERVED1_MASK) >> PIE_RDMA4_DRR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DRR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DRR_T_MASK 0x800000
#define  PIE_RDMA4_DRR_T_SHIFT 23
#define  PIE_RDMA4_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRR_T_MASK) >> PIE_RDMA4_DRR_T_SHIFT)
#define  PIE_RDMA4_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRR_T_MASK) | (((uint32_t)val) << PIE_RDMA4_DRR_T_SHIFT))

#define  PIE_RDMA4_DRR_E_MASK 0x400000
#define  PIE_RDMA4_DRR_E_SHIFT 22
#define  PIE_RDMA4_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRR_E_MASK) >> PIE_RDMA4_DRR_E_SHIFT)
#define  PIE_RDMA4_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRR_E_MASK) | (((uint32_t)val) << PIE_RDMA4_DRR_E_SHIFT))

#define  PIE_RDMA4_DRR_RESERVED2_MASK 0x300000
#define  PIE_RDMA4_DRR_RESERVED2_SHIFT 20
#define  PIE_RDMA4_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRR_RESERVED2_MASK) >> PIE_RDMA4_DRR_RESERVED2_SHIFT)
#define  PIE_RDMA4_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA4_DRR_RESERVED2_SHIFT))

#define  PIE_RDMA4_DRR_N_MASK 0xfffff
#define  PIE_RDMA4_DRR_N_SHIFT 0
#define  PIE_RDMA4_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DRR_N_MASK) >> PIE_RDMA4_DRR_N_SHIFT)
#define  PIE_RDMA4_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DRR_N_MASK) | (((uint32_t)val) << PIE_RDMA4_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_RDMA4_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_RDMA4_DIR_RESERVED1_SHIFT 1
#define  PIE_RDMA4_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DIR_RESERVED1_MASK) >> PIE_RDMA4_DIR_RESERVED1_SHIFT)
#define  PIE_RDMA4_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA4_DIR_RESERVED1_SHIFT))

#define  PIE_RDMA4_DIR_C_MASK 0x1
#define  PIE_RDMA4_DIR_C_SHIFT 0
#define  PIE_RDMA4_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA4_DIR_C_MASK) >> PIE_RDMA4_DIR_C_SHIFT)
#define  PIE_RDMA4_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA4_DIR_C_MASK) | (((uint32_t)val) << PIE_RDMA4_DIR_C_SHIFT))

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 6 of 6) (PIE_RDMA5)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

//====================================================================
//Register: PIE Read DMA Configuration Register (DCR)
/** \brief DMA Channel Configuration Register*/
//====================================================================

#define  PIE_RDMA5_DCR_C_MASK 0x80000000
#define  PIE_RDMA5_DCR_C_SHIFT 31
#define  PIE_RDMA5_DCR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCR_C_MASK) >> PIE_RDMA5_DCR_C_SHIFT)
#define  PIE_RDMA5_DCR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCR_C_MASK) | (((uint32_t)val) << PIE_RDMA5_DCR_C_SHIFT))

#define  PIE_RDMA5_DCR_RESERVED1_MASK 0x7fffffe0
#define  PIE_RDMA5_DCR_RESERVED1_SHIFT 5
#define  PIE_RDMA5_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCR_RESERVED1_MASK) >> PIE_RDMA5_DCR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DCR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DCR_B_MASK 0x10
#define  PIE_RDMA5_DCR_B_SHIFT 4
#define  PIE_RDMA5_DCR_B_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCR_B_MASK) >> PIE_RDMA5_DCR_B_SHIFT)
#define  PIE_RDMA5_DCR_B_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCR_B_MASK) | (((uint32_t)val) << PIE_RDMA5_DCR_B_SHIFT))

#define  PIE_RDMA5_DCR_RESERVED2_MASK 0xc
#define  PIE_RDMA5_DCR_RESERVED2_SHIFT 2
#define  PIE_RDMA5_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCR_RESERVED2_MASK) >> PIE_RDMA5_DCR_RESERVED2_SHIFT)
#define  PIE_RDMA5_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA5_DCR_RESERVED2_SHIFT))

#define  PIE_RDMA5_DCR_R_MASK 0x2
#define  PIE_RDMA5_DCR_R_SHIFT 1
#define  PIE_RDMA5_DCR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCR_R_MASK) >> PIE_RDMA5_DCR_R_SHIFT)
#define  PIE_RDMA5_DCR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCR_R_MASK) | (((uint32_t)val) << PIE_RDMA5_DCR_R_SHIFT))

#define  PIE_RDMA5_DCR_E_MASK 0x1
#define  PIE_RDMA5_DCR_E_SHIFT 0
#define  PIE_RDMA5_DCR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCR_E_MASK) >> PIE_RDMA5_DCR_E_SHIFT)
#define  PIE_RDMA5_DCR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCR_E_MASK) | (((uint32_t)val) << PIE_RDMA5_DCR_E_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 1 (DAR1)
/** \brief DMA Channel Starting Word Address Register 1*/
//====================================================================

#define  PIE_RDMA5_DAR1_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA5_DAR1_ADDRESS_SHIFT 4
#define  PIE_RDMA5_DAR1_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DAR1_ADDRESS_MASK) >> PIE_RDMA5_DAR1_ADDRESS_SHIFT)
#define  PIE_RDMA5_DAR1_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DAR1_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA5_DAR1_ADDRESS_SHIFT))

#define  PIE_RDMA5_DAR1_RESERVED1_MASK 0xf
#define  PIE_RDMA5_DAR1_RESERVED1_SHIFT 0
#define  PIE_RDMA5_DAR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DAR1_RESERVED1_MASK) >> PIE_RDMA5_DAR1_RESERVED1_SHIFT)
#define  PIE_RDMA5_DAR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DAR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DAR1_RESERVED1_SHIFT))

//====================================================================
//Register: PIE Read DMA Address Register 2 (DAR2)
/** \brief DMA Channel Starting Word Address Register 2*/
//====================================================================

#define  PIE_RDMA5_DAR2_ADDRESS_MASK 0xfffffff0
#define  PIE_RDMA5_DAR2_ADDRESS_SHIFT 4
#define  PIE_RDMA5_DAR2_ADDRESS_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DAR2_ADDRESS_MASK) >> PIE_RDMA5_DAR2_ADDRESS_SHIFT)
#define  PIE_RDMA5_DAR2_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DAR2_ADDRESS_MASK) | (((uint32_t)val) << PIE_RDMA5_DAR2_ADDRESS_SHIFT))

#define  PIE_RDMA5_DAR2_RESERVED1_MASK 0xf
#define  PIE_RDMA5_DAR2_RESERVED1_SHIFT 0
#define  PIE_RDMA5_DAR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DAR2_RESERVED1_MASK) >> PIE_RDMA5_DAR2_RESERVED1_SHIFT)
#define  PIE_RDMA5_DAR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DAR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DAR2_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Row Count Register1 (DRCR1)
/** \brief Row count in lines for Address 1*/
//====================================================================

#define  PIE_RDMA5_DRCR1_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA5_DRCR1_RESERVED1_SHIFT 6
#define  PIE_RDMA5_DRCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCR1_RESERVED1_MASK) >> PIE_RDMA5_DRCR1_RESERVED1_SHIFT)
#define  PIE_RDMA5_DRCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCR1_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCR1_RESERVED1_SHIFT))

#define  PIE_RDMA5_DRCR1_T_MASK 0x3f
#define  PIE_RDMA5_DRCR1_T_SHIFT 0
#define  PIE_RDMA5_DRCR1_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCR1_T_MASK) >> PIE_RDMA5_DRCR1_T_SHIFT)
#define  PIE_RDMA5_DRCR1_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCR1_T_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCR1_T_SHIFT))

//====================================================================
//Register: DMA Row Count Register2 (DRCR2)
/** \brief Row count in lines for Address 2*/
//====================================================================

#define  PIE_RDMA5_DRCR2_RESERVED1_MASK 0xffffffc0
#define  PIE_RDMA5_DRCR2_RESERVED1_SHIFT 6
#define  PIE_RDMA5_DRCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCR2_RESERVED1_MASK) >> PIE_RDMA5_DRCR2_RESERVED1_SHIFT)
#define  PIE_RDMA5_DRCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCR2_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCR2_RESERVED1_SHIFT))

#define  PIE_RDMA5_DRCR2_T_MASK 0x3f
#define  PIE_RDMA5_DRCR2_T_SHIFT 0
#define  PIE_RDMA5_DRCR2_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCR2_T_MASK) >> PIE_RDMA5_DRCR2_T_SHIFT)
#define  PIE_RDMA5_DRCR2_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCR2_T_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCR2_T_SHIFT))

//====================================================================
//Register: DMA Byte Count Register (DBCR)
/** \brief Byte count per line of scan data*/
//====================================================================

#define  PIE_RDMA5_DBCR_RESERVED1_MASK 0xfffc0000
#define  PIE_RDMA5_DBCR_RESERVED1_SHIFT 18
#define  PIE_RDMA5_DBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DBCR_RESERVED1_MASK) >> PIE_RDMA5_DBCR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DBCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DBCR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DBCR_L_MASK 0x3fff0
#define  PIE_RDMA5_DBCR_L_SHIFT 4
#define  PIE_RDMA5_DBCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DBCR_L_MASK) >> PIE_RDMA5_DBCR_L_SHIFT)
#define  PIE_RDMA5_DBCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DBCR_L_MASK) | (((uint32_t)val) << PIE_RDMA5_DBCR_L_SHIFT))

#define  PIE_RDMA5_DBCR_RESERVED2_MASK 0xf
#define  PIE_RDMA5_DBCR_RESERVED2_SHIFT 0
#define  PIE_RDMA5_DBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DBCR_RESERVED2_MASK) >> PIE_RDMA5_DBCR_RESERVED2_SHIFT)
#define  PIE_RDMA5_DBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DBCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA5_DBCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Transfer Count Register (DTCR)
/** \brief Total transfer Count in bytes.*/
//====================================================================

#define  PIE_RDMA5_DTCR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA5_DTCR_RESERVED1_SHIFT 20
#define  PIE_RDMA5_DTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DTCR_RESERVED1_MASK) >> PIE_RDMA5_DTCR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DTCR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DTCR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DTCR_L_MASK 0xffff0
#define  PIE_RDMA5_DTCR_L_SHIFT 4
#define  PIE_RDMA5_DTCR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DTCR_L_MASK) >> PIE_RDMA5_DTCR_L_SHIFT)
#define  PIE_RDMA5_DTCR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DTCR_L_MASK) | (((uint32_t)val) << PIE_RDMA5_DTCR_L_SHIFT))

#define  PIE_RDMA5_DTCR_RESERVED2_MASK 0xf
#define  PIE_RDMA5_DTCR_RESERVED2_SHIFT 0
#define  PIE_RDMA5_DTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DTCR_RESERVED2_MASK) >> PIE_RDMA5_DTCR_RESERVED2_SHIFT)
#define  PIE_RDMA5_DTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DTCR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA5_DTCR_RESERVED2_SHIFT))

//====================================================================
//Register: DMA Start Register (DSTR)
/** \brief DMA Channel Start*/
//====================================================================

#define  PIE_RDMA5_DSTR_RESERVED1_MASK 0xffffffff
#define  PIE_RDMA5_DSTR_RESERVED1_SHIFT 0
#define  PIE_RDMA5_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DSTR_RESERVED1_MASK) >> PIE_RDMA5_DSTR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DSTR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DSTR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Source Current Address (DASR)
/** \brief DMA Channel Source Current Address*/
//====================================================================

#define  PIE_RDMA5_DASR_A_MASK 0xfffffffc
#define  PIE_RDMA5_DASR_A_SHIFT 2
#define  PIE_RDMA5_DASR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DASR_A_MASK) >> PIE_RDMA5_DASR_A_SHIFT)
#define  PIE_RDMA5_DASR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DASR_A_MASK) | (((uint32_t)val) << PIE_RDMA5_DASR_A_SHIFT))

#define  PIE_RDMA5_DASR_RESERVED1_MASK 0x3
#define  PIE_RDMA5_DASR_RESERVED1_SHIFT 0
#define  PIE_RDMA5_DASR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DASR_RESERVED1_MASK) >> PIE_RDMA5_DASR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DASR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DASR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DASR_RESERVED1_SHIFT))

//====================================================================
//Register: DMA Transfer Count Status Register (DCSR)
/** \brief Current transfer count*/
//====================================================================

#define  PIE_RDMA5_DCSR_RESERVED1_MASK 0xfff00000
#define  PIE_RDMA5_DCSR_RESERVED1_SHIFT 20
#define  PIE_RDMA5_DCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCSR_RESERVED1_MASK) >> PIE_RDMA5_DCSR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DCSR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DCSR_T_MASK 0xfffff
#define  PIE_RDMA5_DCSR_T_SHIFT 0
#define  PIE_RDMA5_DCSR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DCSR_T_MASK) >> PIE_RDMA5_DCSR_T_SHIFT)
#define  PIE_RDMA5_DCSR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DCSR_T_MASK) | (((uint32_t)val) << PIE_RDMA5_DCSR_T_SHIFT))

//====================================================================
//Register: DMA Row Count Status Register (DRCSR)
/** \brief DMA Channel Row Count Status Register*/
//====================================================================

#define  PIE_RDMA5_DRCSR_RESERVED1_MASK 0xffffc000
#define  PIE_RDMA5_DRCSR_RESERVED1_SHIFT 14
#define  PIE_RDMA5_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCSR_RESERVED1_MASK) >> PIE_RDMA5_DRCSR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCSR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DRCSR_M_MASK 0x3f00
#define  PIE_RDMA5_DRCSR_M_SHIFT 8
#define  PIE_RDMA5_DRCSR_M_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCSR_M_MASK) >> PIE_RDMA5_DRCSR_M_SHIFT)
#define  PIE_RDMA5_DRCSR_M_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCSR_M_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCSR_M_SHIFT))

#define  PIE_RDMA5_DRCSR_RESERVED2_MASK 0xc0
#define  PIE_RDMA5_DRCSR_RESERVED2_SHIFT 6
#define  PIE_RDMA5_DRCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCSR_RESERVED2_MASK) >> PIE_RDMA5_DRCSR_RESERVED2_SHIFT)
#define  PIE_RDMA5_DRCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCSR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCSR_RESERVED2_SHIFT))

#define  PIE_RDMA5_DRCSR_L_MASK 0x3f
#define  PIE_RDMA5_DRCSR_L_SHIFT 0
#define  PIE_RDMA5_DRCSR_L_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRCSR_L_MASK) >> PIE_RDMA5_DRCSR_L_SHIFT)
#define  PIE_RDMA5_DRCSR_L_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRCSR_L_MASK) | (((uint32_t)val) << PIE_RDMA5_DRCSR_L_SHIFT))

//====================================================================
//Register: DMA Status Register (DSR)
/** \brief DMA Status Register*/
//====================================================================

#define  PIE_RDMA5_DSR_C_MASK 0x80000000
#define  PIE_RDMA5_DSR_C_SHIFT 31
#define  PIE_RDMA5_DSR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DSR_C_MASK) >> PIE_RDMA5_DSR_C_SHIFT)
#define  PIE_RDMA5_DSR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DSR_C_MASK) | (((uint32_t)val) << PIE_RDMA5_DSR_C_SHIFT))

#define  PIE_RDMA5_DSR_RESERVED1_MASK 0x7ffffffc
#define  PIE_RDMA5_DSR_RESERVED1_SHIFT 2
#define  PIE_RDMA5_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DSR_RESERVED1_MASK) >> PIE_RDMA5_DSR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DSR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DSR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DSR_R_MASK 0x2
#define  PIE_RDMA5_DSR_R_SHIFT 1
#define  PIE_RDMA5_DSR_R_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DSR_R_MASK) >> PIE_RDMA5_DSR_R_SHIFT)
#define  PIE_RDMA5_DSR_R_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DSR_R_MASK) | (((uint32_t)val) << PIE_RDMA5_DSR_R_SHIFT))

#define  PIE_RDMA5_DSR_A_MASK 0x1
#define  PIE_RDMA5_DSR_A_SHIFT 0
#define  PIE_RDMA5_DSR_A_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DSR_A_MASK) >> PIE_RDMA5_DSR_A_SHIFT)
#define  PIE_RDMA5_DSR_A_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DSR_A_MASK) | (((uint32_t)val) << PIE_RDMA5_DSR_A_SHIFT))

//====================================================================
//Register: DMA Results Register (DRR)
/** \brief DMA Results Register*/
//====================================================================

#define  PIE_RDMA5_DRR_RESERVED1_MASK 0xff000000
#define  PIE_RDMA5_DRR_RESERVED1_SHIFT 24
#define  PIE_RDMA5_DRR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRR_RESERVED1_MASK) >> PIE_RDMA5_DRR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DRR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DRR_T_MASK 0x800000
#define  PIE_RDMA5_DRR_T_SHIFT 23
#define  PIE_RDMA5_DRR_T_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRR_T_MASK) >> PIE_RDMA5_DRR_T_SHIFT)
#define  PIE_RDMA5_DRR_T_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRR_T_MASK) | (((uint32_t)val) << PIE_RDMA5_DRR_T_SHIFT))

#define  PIE_RDMA5_DRR_E_MASK 0x400000
#define  PIE_RDMA5_DRR_E_SHIFT 22
#define  PIE_RDMA5_DRR_E_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRR_E_MASK) >> PIE_RDMA5_DRR_E_SHIFT)
#define  PIE_RDMA5_DRR_E_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRR_E_MASK) | (((uint32_t)val) << PIE_RDMA5_DRR_E_SHIFT))

#define  PIE_RDMA5_DRR_RESERVED2_MASK 0x300000
#define  PIE_RDMA5_DRR_RESERVED2_SHIFT 20
#define  PIE_RDMA5_DRR_RESERVED2_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRR_RESERVED2_MASK) >> PIE_RDMA5_DRR_RESERVED2_SHIFT)
#define  PIE_RDMA5_DRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRR_RESERVED2_MASK) | (((uint32_t)val) << PIE_RDMA5_DRR_RESERVED2_SHIFT))

#define  PIE_RDMA5_DRR_N_MASK 0xfffff
#define  PIE_RDMA5_DRR_N_SHIFT 0
#define  PIE_RDMA5_DRR_N_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DRR_N_MASK) >> PIE_RDMA5_DRR_N_SHIFT)
#define  PIE_RDMA5_DRR_N_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DRR_N_MASK) | (((uint32_t)val) << PIE_RDMA5_DRR_N_SHIFT))

//====================================================================
//Register: DMA Interrupt Register (DIR)
/** \brief DMA Results Register
This register is write only. Reading this register will return a value of 32'hDEADDEAD.*/
//====================================================================

#define  PIE_RDMA5_DIR_RESERVED1_MASK 0xfffffffe
#define  PIE_RDMA5_DIR_RESERVED1_SHIFT 1
#define  PIE_RDMA5_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DIR_RESERVED1_MASK) >> PIE_RDMA5_DIR_RESERVED1_SHIFT)
#define  PIE_RDMA5_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DIR_RESERVED1_MASK) | (((uint32_t)val) << PIE_RDMA5_DIR_RESERVED1_SHIFT))

#define  PIE_RDMA5_DIR_C_MASK 0x1
#define  PIE_RDMA5_DIR_C_SHIFT 0
#define  PIE_RDMA5_DIR_C_MASK_SHIFT(reg) (((reg) & PIE_RDMA5_DIR_C_MASK) >> PIE_RDMA5_DIR_C_SHIFT)
#define  PIE_RDMA5_DIR_C_REPLACE_VAL(reg,val) (((reg) & ~PIE_RDMA5_DIR_C_MASK) | (((uint32_t)val) << PIE_RDMA5_DIR_C_SHIFT))

#endif // PIE_RDMA_ICE
