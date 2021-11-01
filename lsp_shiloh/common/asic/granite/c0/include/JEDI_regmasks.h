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
 * \file JEDI_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _JEDI_REGMASKS_H_
#define _JEDI_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: JEDI Register Set (JEDI)
/** \brief Top-level register file for JEDI block*/
//
//====================================================================

//====================================================================
//
//Register File: RDMA Registers (RDMA)
/** \brief This file defines the registers for the Read DMA for the JPEG Super Block*/
//
//====================================================================

//====================================================================
//Register: JPEG RDMA Configuration Register (DCR)
//====================================================================

#define  RDMA_DCR_INT_EN_MASK 0x80000000
#define  RDMA_DCR_INT_EN_SHIFT 31
#define  RDMA_DCR_INT_EN_MASK_SHIFT(reg) (((reg) & RDMA_DCR_INT_EN_MASK) >> RDMA_DCR_INT_EN_SHIFT)
#define  RDMA_DCR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_INT_EN_MASK) | (((uint32_t)val) << RDMA_DCR_INT_EN_SHIFT))

#define  RDMA_DCR_LE_BYTE_PACK_MASK 0x40000000
#define  RDMA_DCR_LE_BYTE_PACK_SHIFT 30
#define  RDMA_DCR_LE_BYTE_PACK_MASK_SHIFT(reg) (((reg) & RDMA_DCR_LE_BYTE_PACK_MASK) >> RDMA_DCR_LE_BYTE_PACK_SHIFT)
#define  RDMA_DCR_LE_BYTE_PACK_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_LE_BYTE_PACK_MASK) | (((uint32_t)val) << RDMA_DCR_LE_BYTE_PACK_SHIFT))

#define  RDMA_DCR_RESERVED1_MASK 0x3ffffc00
#define  RDMA_DCR_RESERVED1_SHIFT 10
#define  RDMA_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DCR_RESERVED1_MASK) >> RDMA_DCR_RESERVED1_SHIFT)
#define  RDMA_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DCR_RESERVED1_SHIFT))

#define  RDMA_DCR_PXL_MODE_MASK 0x300
#define  RDMA_DCR_PXL_MODE_SHIFT 8
#define  RDMA_DCR_PXL_MODE_MASK_SHIFT(reg) (((reg) & RDMA_DCR_PXL_MODE_MASK) >> RDMA_DCR_PXL_MODE_SHIFT)
#define  RDMA_DCR_PXL_MODE_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_PXL_MODE_MASK) | (((uint32_t)val) << RDMA_DCR_PXL_MODE_SHIFT))

#define  RDMA_DCR_RESERVED2_MASK 0xc0
#define  RDMA_DCR_RESERVED2_SHIFT 6
#define  RDMA_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & RDMA_DCR_RESERVED2_MASK) >> RDMA_DCR_RESERVED2_SHIFT)
#define  RDMA_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_RESERVED2_MASK) | (((uint32_t)val) << RDMA_DCR_RESERVED2_SHIFT))

#define  RDMA_DCR_PATH_MASK 0x30
#define  RDMA_DCR_PATH_SHIFT 4
#define  RDMA_DCR_PATH_MASK_SHIFT(reg) (((reg) & RDMA_DCR_PATH_MASK) >> RDMA_DCR_PATH_SHIFT)
#define  RDMA_DCR_PATH_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_PATH_MASK) | (((uint32_t)val) << RDMA_DCR_PATH_SHIFT))

#define  RDMA_DCR_RESERVED3_MASK 0xc
#define  RDMA_DCR_RESERVED3_SHIFT 2
#define  RDMA_DCR_RESERVED3_MASK_SHIFT(reg) (((reg) & RDMA_DCR_RESERVED3_MASK) >> RDMA_DCR_RESERVED3_SHIFT)
#define  RDMA_DCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_RESERVED3_MASK) | (((uint32_t)val) << RDMA_DCR_RESERVED3_SHIFT))

#define  RDMA_DCR_RESET_MASK 0x2
#define  RDMA_DCR_RESET_SHIFT 1
#define  RDMA_DCR_RESET_MASK_SHIFT(reg) (((reg) & RDMA_DCR_RESET_MASK) >> RDMA_DCR_RESET_SHIFT)
#define  RDMA_DCR_RESET_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_RESET_MASK) | (((uint32_t)val) << RDMA_DCR_RESET_SHIFT))

#define  RDMA_DCR_ENABLE_MASK 0x1
#define  RDMA_DCR_ENABLE_SHIFT 0
#define  RDMA_DCR_ENABLE_MASK_SHIFT(reg) (((reg) & RDMA_DCR_ENABLE_MASK) >> RDMA_DCR_ENABLE_SHIFT)
#define  RDMA_DCR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DCR_ENABLE_MASK) | (((uint32_t)val) << RDMA_DCR_ENABLE_SHIFT))

//====================================================================
//Register: JPEG RDMA Address 1 Register (DAR1)
//====================================================================

#define  RDMA_DAR1_ADDR_MASK 0xffffffff
#define  RDMA_DAR1_ADDR_SHIFT 0
#define  RDMA_DAR1_ADDR_MASK_SHIFT(reg) (((reg) & RDMA_DAR1_ADDR_MASK) >> RDMA_DAR1_ADDR_SHIFT)
#define  RDMA_DAR1_ADDR_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DAR1_ADDR_MASK) | (((uint32_t)val) << RDMA_DAR1_ADDR_SHIFT))

//====================================================================
//Register: JPEG RDMA Address 2 Register (DAR2)
//====================================================================

#define  RDMA_DAR2_ADDR_MASK 0xffffffff
#define  RDMA_DAR2_ADDR_SHIFT 0
#define  RDMA_DAR2_ADDR_MASK_SHIFT(reg) (((reg) & RDMA_DAR2_ADDR_MASK) >> RDMA_DAR2_ADDR_SHIFT)
#define  RDMA_DAR2_ADDR_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DAR2_ADDR_MASK) | (((uint32_t)val) << RDMA_DAR2_ADDR_SHIFT))

//====================================================================
//Register: JPEG RDMA Row Count 1 Register (DRCR1)
/** \brief Main configuration register for the NAND Flash Contorller*/
//====================================================================

#define  RDMA_DRCR1_RESERVED1_MASK 0xffff0000
#define  RDMA_DRCR1_RESERVED1_SHIFT 16
#define  RDMA_DRCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DRCR1_RESERVED1_MASK) >> RDMA_DRCR1_RESERVED1_SHIFT)
#define  RDMA_DRCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRCR1_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DRCR1_RESERVED1_SHIFT))

#define  RDMA_DRCR1_ROWCNT_MASK 0xffff
#define  RDMA_DRCR1_ROWCNT_SHIFT 0
#define  RDMA_DRCR1_ROWCNT_MASK_SHIFT(reg) (((reg) & RDMA_DRCR1_ROWCNT_MASK) >> RDMA_DRCR1_ROWCNT_SHIFT)
#define  RDMA_DRCR1_ROWCNT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRCR1_ROWCNT_MASK) | (((uint32_t)val) << RDMA_DRCR1_ROWCNT_SHIFT))

//====================================================================
//Register: JPEG RDMA Row Count 2 Register (DRCR2)
//====================================================================

#define  RDMA_DRCR2_RESERVED1_MASK 0xffff0000
#define  RDMA_DRCR2_RESERVED1_SHIFT 16
#define  RDMA_DRCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DRCR2_RESERVED1_MASK) >> RDMA_DRCR2_RESERVED1_SHIFT)
#define  RDMA_DRCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRCR2_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DRCR2_RESERVED1_SHIFT))

#define  RDMA_DRCR2_ROWCNT_MASK 0xffff
#define  RDMA_DRCR2_ROWCNT_SHIFT 0
#define  RDMA_DRCR2_ROWCNT_MASK_SHIFT(reg) (((reg) & RDMA_DRCR2_ROWCNT_MASK) >> RDMA_DRCR2_ROWCNT_SHIFT)
#define  RDMA_DRCR2_ROWCNT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRCR2_ROWCNT_MASK) | (((uint32_t)val) << RDMA_DRCR2_ROWCNT_SHIFT))

//====================================================================
//Register: JPEG RDMA Line Width Register (DLWR)
//====================================================================

#define  RDMA_DLWR_RESERVED1_MASK 0xff000000
#define  RDMA_DLWR_RESERVED1_SHIFT 24
#define  RDMA_DLWR_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DLWR_RESERVED1_MASK) >> RDMA_DLWR_RESERVED1_SHIFT)
#define  RDMA_DLWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DLWR_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DLWR_RESERVED1_SHIFT))

#define  RDMA_DLWR_LWIDTH_MASK 0xffffff
#define  RDMA_DLWR_LWIDTH_SHIFT 0
#define  RDMA_DLWR_LWIDTH_MASK_SHIFT(reg) (((reg) & RDMA_DLWR_LWIDTH_MASK) >> RDMA_DLWR_LWIDTH_SHIFT)
#define  RDMA_DLWR_LWIDTH_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DLWR_LWIDTH_MASK) | (((uint32_t)val) << RDMA_DLWR_LWIDTH_SHIFT))

//====================================================================
//Register: JPEG RDMA Transfer Count Register (DTCR)
//====================================================================

#define  RDMA_DTCR_XFERCNT_MASK 0xffffffff
#define  RDMA_DTCR_XFERCNT_SHIFT 0
#define  RDMA_DTCR_XFERCNT_MASK_SHIFT(reg) (((reg) & RDMA_DTCR_XFERCNT_MASK) >> RDMA_DTCR_XFERCNT_SHIFT)
#define  RDMA_DTCR_XFERCNT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DTCR_XFERCNT_MASK) | (((uint32_t)val) << RDMA_DTCR_XFERCNT_SHIFT))

//====================================================================
//Register: JPEG RDMA Start Register (DSTR)
/** \brief Writing any value to this register will start a DMA transaction.
        Since it is Write Only, reading this register returns 32'hDEADDEAD*/
//====================================================================

#define  RDMA_DSTR_RESERVED1_MASK 0xfffffffe
#define  RDMA_DSTR_RESERVED1_SHIFT 1
#define  RDMA_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DSTR_RESERVED1_MASK) >> RDMA_DSTR_RESERVED1_SHIFT)
#define  RDMA_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DSTR_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DSTR_RESERVED1_SHIFT))

#define  RDMA_DSTR_START_MASK 0x1
#define  RDMA_DSTR_START_SHIFT 0
#define  RDMA_DSTR_START_MASK_SHIFT(reg) (((reg) & RDMA_DSTR_START_MASK) >> RDMA_DSTR_START_SHIFT)
#define  RDMA_DSTR_START_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DSTR_START_MASK) | (((uint32_t)val) << RDMA_DSTR_START_SHIFT))

//====================================================================
//Register: JPEG RDMA Address Status Register (DASR)
//====================================================================

#define  RDMA_DASR_ADDR_MASK 0xffffffff
#define  RDMA_DASR_ADDR_SHIFT 0
#define  RDMA_DASR_ADDR_MASK_SHIFT(reg) (((reg) & RDMA_DASR_ADDR_MASK) >> RDMA_DASR_ADDR_SHIFT)
#define  RDMA_DASR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DASR_ADDR_MASK) | (((uint32_t)val) << RDMA_DASR_ADDR_SHIFT))

//====================================================================
//Register: JPEG RDMA Transfer Count Status Register (DTCSR)
//====================================================================

#define  RDMA_DTCSR_XFERCNT_MASK 0xffffffff
#define  RDMA_DTCSR_XFERCNT_SHIFT 0
#define  RDMA_DTCSR_XFERCNT_MASK_SHIFT(reg) (((reg) & RDMA_DTCSR_XFERCNT_MASK) >> RDMA_DTCSR_XFERCNT_SHIFT)
#define  RDMA_DTCSR_XFERCNT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DTCSR_XFERCNT_MASK) | (((uint32_t)val) << RDMA_DTCSR_XFERCNT_SHIFT))

//====================================================================
//Register: JPEG RDMA Row Count Status Register (DRCSR)
//====================================================================

#define  RDMA_DRCSR_RESERVED1_MASK 0xffff0000
#define  RDMA_DRCSR_RESERVED1_SHIFT 16
#define  RDMA_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DRCSR_RESERVED1_MASK) >> RDMA_DRCSR_RESERVED1_SHIFT)
#define  RDMA_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DRCSR_RESERVED1_SHIFT))

#define  RDMA_DRCSR_ROWCNT_MASK 0xffff
#define  RDMA_DRCSR_ROWCNT_SHIFT 0
#define  RDMA_DRCSR_ROWCNT_MASK_SHIFT(reg) (((reg) & RDMA_DRCSR_ROWCNT_MASK) >> RDMA_DRCSR_ROWCNT_SHIFT)
#define  RDMA_DRCSR_ROWCNT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRCSR_ROWCNT_MASK) | (((uint32_t)val) << RDMA_DRCSR_ROWCNT_SHIFT))

//====================================================================
//Register: JPEG RDMA Status Register (DSR)
//====================================================================

#define  RDMA_DSR_COMP_INT_MASK 0x80000000
#define  RDMA_DSR_COMP_INT_SHIFT 31
#define  RDMA_DSR_COMP_INT_MASK_SHIFT(reg) (((reg) & RDMA_DSR_COMP_INT_MASK) >> RDMA_DSR_COMP_INT_SHIFT)
#define  RDMA_DSR_COMP_INT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DSR_COMP_INT_MASK) | (((uint32_t)val) << RDMA_DSR_COMP_INT_SHIFT))

#define  RDMA_DSR_RESERVED1_MASK 0x7ffffffc
#define  RDMA_DSR_RESERVED1_SHIFT 2
#define  RDMA_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DSR_RESERVED1_MASK) >> RDMA_DSR_RESERVED1_SHIFT)
#define  RDMA_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DSR_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DSR_RESERVED1_SHIFT))

#define  RDMA_DSR_READY_MASK 0x2
#define  RDMA_DSR_READY_SHIFT 1
#define  RDMA_DSR_READY_MASK_SHIFT(reg) (((reg) & RDMA_DSR_READY_MASK) >> RDMA_DSR_READY_SHIFT)
#define  RDMA_DSR_READY_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DSR_READY_MASK) | (((uint32_t)val) << RDMA_DSR_READY_SHIFT))

#define  RDMA_DSR_ACTIVE_MASK 0x1
#define  RDMA_DSR_ACTIVE_SHIFT 0
#define  RDMA_DSR_ACTIVE_MASK_SHIFT(reg) (((reg) & RDMA_DSR_ACTIVE_MASK) >> RDMA_DSR_ACTIVE_SHIFT)
#define  RDMA_DSR_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DSR_ACTIVE_MASK) | (((uint32_t)val) << RDMA_DSR_ACTIVE_SHIFT))

//====================================================================
//Register: JPEG RDMA Result Register (DRR)
/** \brief The contents of this register are valid only when the "DMA Complete" bit is set in the DMA Status Register (not valid during firmware interrupt testing). After the results has been read, the "DMA Complete" bit should be cleared to allow the DMA to continue.*/
//====================================================================

#define  RDMA_DRR_XFERCNT_MASK 0xfffffffc
#define  RDMA_DRR_XFERCNT_SHIFT 2
#define  RDMA_DRR_XFERCNT_MASK_SHIFT(reg) (((reg) & RDMA_DRR_XFERCNT_MASK) >> RDMA_DRR_XFERCNT_SHIFT)
#define  RDMA_DRR_XFERCNT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRR_XFERCNT_MASK) | (((uint32_t)val) << RDMA_DRR_XFERCNT_SHIFT))

#define  RDMA_DRR_TERM_MASK 0x2
#define  RDMA_DRR_TERM_SHIFT 1
#define  RDMA_DRR_TERM_MASK_SHIFT(reg) (((reg) & RDMA_DRR_TERM_MASK) >> RDMA_DRR_TERM_SHIFT)
#define  RDMA_DRR_TERM_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRR_TERM_MASK) | (((uint32_t)val) << RDMA_DRR_TERM_SHIFT))

#define  RDMA_DRR_ERROR_MASK 0x1
#define  RDMA_DRR_ERROR_SHIFT 0
#define  RDMA_DRR_ERROR_MASK_SHIFT(reg) (((reg) & RDMA_DRR_ERROR_MASK) >> RDMA_DRR_ERROR_SHIFT)
#define  RDMA_DRR_ERROR_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DRR_ERROR_MASK) | (((uint32_t)val) << RDMA_DRR_ERROR_SHIFT))

//====================================================================
//Register: JPEG RDMA Interrupt Register (DIR)
//====================================================================

#define  RDMA_DIR_RESERVED1_MASK 0xfffffffe
#define  RDMA_DIR_RESERVED1_SHIFT 1
#define  RDMA_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & RDMA_DIR_RESERVED1_MASK) >> RDMA_DIR_RESERVED1_SHIFT)
#define  RDMA_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DIR_RESERVED1_MASK) | (((uint32_t)val) << RDMA_DIR_RESERVED1_SHIFT))

#define  RDMA_DIR_CLR_INT_MASK 0x1
#define  RDMA_DIR_CLR_INT_SHIFT 0
#define  RDMA_DIR_CLR_INT_MASK_SHIFT(reg) (((reg) & RDMA_DIR_CLR_INT_MASK) >> RDMA_DIR_CLR_INT_SHIFT)
#define  RDMA_DIR_CLR_INT_REPLACE_VAL(reg,val) (((reg) & ~RDMA_DIR_CLR_INT_MASK) | (((uint32_t)val) << RDMA_DIR_CLR_INT_SHIFT))

//====================================================================
//
//Register File: WDMA Registers (WDMA)
/** \brief This file defines the registers for the Write DMA for the JPEG Super Block*/
//
//====================================================================

//====================================================================
//Register: JPEG WDMA Configuration Register (DCR)
//====================================================================

#define  WDMA_DCR_INT_EN_MASK 0x80000000
#define  WDMA_DCR_INT_EN_SHIFT 31
#define  WDMA_DCR_INT_EN_MASK_SHIFT(reg) (((reg) & WDMA_DCR_INT_EN_MASK) >> WDMA_DCR_INT_EN_SHIFT)
#define  WDMA_DCR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_INT_EN_MASK) | (((uint32_t)val) << WDMA_DCR_INT_EN_SHIFT))

#define  WDMA_DCR_LE_BYTE_PACK_MASK 0x40000000
#define  WDMA_DCR_LE_BYTE_PACK_SHIFT 30
#define  WDMA_DCR_LE_BYTE_PACK_MASK_SHIFT(reg) (((reg) & WDMA_DCR_LE_BYTE_PACK_MASK) >> WDMA_DCR_LE_BYTE_PACK_SHIFT)
#define  WDMA_DCR_LE_BYTE_PACK_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_LE_BYTE_PACK_MASK) | (((uint32_t)val) << WDMA_DCR_LE_BYTE_PACK_SHIFT))

#define  WDMA_DCR_TWIDTH_MASK 0x38000000
#define  WDMA_DCR_TWIDTH_SHIFT 27
#define  WDMA_DCR_TWIDTH_MASK_SHIFT(reg) (((reg) & WDMA_DCR_TWIDTH_MASK) >> WDMA_DCR_TWIDTH_SHIFT)
#define  WDMA_DCR_TWIDTH_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_TWIDTH_MASK) | (((uint32_t)val) << WDMA_DCR_TWIDTH_SHIFT))

#define  WDMA_DCR_THEIGHT_MASK 0x7000000
#define  WDMA_DCR_THEIGHT_SHIFT 24
#define  WDMA_DCR_THEIGHT_MASK_SHIFT(reg) (((reg) & WDMA_DCR_THEIGHT_MASK) >> WDMA_DCR_THEIGHT_SHIFT)
#define  WDMA_DCR_THEIGHT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_THEIGHT_MASK) | (((uint32_t)val) << WDMA_DCR_THEIGHT_SHIFT))

#define  WDMA_DCR_RESERVED1_MASK 0xff8000
#define  WDMA_DCR_RESERVED1_SHIFT 15
#define  WDMA_DCR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DCR_RESERVED1_MASK) >> WDMA_DCR_RESERVED1_SHIFT)
#define  WDMA_DCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DCR_RESERVED1_SHIFT))

#define  WDMA_DCR_ROTCONFIG_MASK 0x7000
#define  WDMA_DCR_ROTCONFIG_SHIFT 12
#define  WDMA_DCR_ROTCONFIG_MASK_SHIFT(reg) (((reg) & WDMA_DCR_ROTCONFIG_MASK) >> WDMA_DCR_ROTCONFIG_SHIFT)
#define  WDMA_DCR_ROTCONFIG_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_ROTCONFIG_MASK) | (((uint32_t)val) << WDMA_DCR_ROTCONFIG_SHIFT))

#define  WDMA_DCR_RESERVED2_MASK 0xc00
#define  WDMA_DCR_RESERVED2_SHIFT 10
#define  WDMA_DCR_RESERVED2_MASK_SHIFT(reg) (((reg) & WDMA_DCR_RESERVED2_MASK) >> WDMA_DCR_RESERVED2_SHIFT)
#define  WDMA_DCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_RESERVED2_MASK) | (((uint32_t)val) << WDMA_DCR_RESERVED2_SHIFT))

#define  WDMA_DCR_PXL_MODE_MASK 0x300
#define  WDMA_DCR_PXL_MODE_SHIFT 8
#define  WDMA_DCR_PXL_MODE_MASK_SHIFT(reg) (((reg) & WDMA_DCR_PXL_MODE_MASK) >> WDMA_DCR_PXL_MODE_SHIFT)
#define  WDMA_DCR_PXL_MODE_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_PXL_MODE_MASK) | (((uint32_t)val) << WDMA_DCR_PXL_MODE_SHIFT))

#define  WDMA_DCR_RESERVED3_MASK 0xc0
#define  WDMA_DCR_RESERVED3_SHIFT 6
#define  WDMA_DCR_RESERVED3_MASK_SHIFT(reg) (((reg) & WDMA_DCR_RESERVED3_MASK) >> WDMA_DCR_RESERVED3_SHIFT)
#define  WDMA_DCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_RESERVED3_MASK) | (((uint32_t)val) << WDMA_DCR_RESERVED3_SHIFT))

#define  WDMA_DCR_PATH_MASK 0x30
#define  WDMA_DCR_PATH_SHIFT 4
#define  WDMA_DCR_PATH_MASK_SHIFT(reg) (((reg) & WDMA_DCR_PATH_MASK) >> WDMA_DCR_PATH_SHIFT)
#define  WDMA_DCR_PATH_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_PATH_MASK) | (((uint32_t)val) << WDMA_DCR_PATH_SHIFT))

#define  WDMA_DCR_RESERVED4_MASK 0xc
#define  WDMA_DCR_RESERVED4_SHIFT 2
#define  WDMA_DCR_RESERVED4_MASK_SHIFT(reg) (((reg) & WDMA_DCR_RESERVED4_MASK) >> WDMA_DCR_RESERVED4_SHIFT)
#define  WDMA_DCR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_RESERVED4_MASK) | (((uint32_t)val) << WDMA_DCR_RESERVED4_SHIFT))

#define  WDMA_DCR_RESET_MASK 0x2
#define  WDMA_DCR_RESET_SHIFT 1
#define  WDMA_DCR_RESET_MASK_SHIFT(reg) (((reg) & WDMA_DCR_RESET_MASK) >> WDMA_DCR_RESET_SHIFT)
#define  WDMA_DCR_RESET_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_RESET_MASK) | (((uint32_t)val) << WDMA_DCR_RESET_SHIFT))

#define  WDMA_DCR_ENABLE_MASK 0x1
#define  WDMA_DCR_ENABLE_SHIFT 0
#define  WDMA_DCR_ENABLE_MASK_SHIFT(reg) (((reg) & WDMA_DCR_ENABLE_MASK) >> WDMA_DCR_ENABLE_SHIFT)
#define  WDMA_DCR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DCR_ENABLE_MASK) | (((uint32_t)val) << WDMA_DCR_ENABLE_SHIFT))

//====================================================================
//Register: JPEG WDMA Address Register (DAR)
//====================================================================

#define  WDMA_DAR_ADDR_MASK 0xffffffff
#define  WDMA_DAR_ADDR_SHIFT 0
#define  WDMA_DAR_ADDR_MASK_SHIFT(reg) (((reg) & WDMA_DAR_ADDR_MASK) >> WDMA_DAR_ADDR_SHIFT)
#define  WDMA_DAR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DAR_ADDR_MASK) | (((uint32_t)val) << WDMA_DAR_ADDR_SHIFT))

//====================================================================
//Register: JPEG WDMA Row Count Register (DRCR)
//====================================================================

#define  WDMA_DRCR_RESERVED1_MASK 0xffff0000
#define  WDMA_DRCR_RESERVED1_SHIFT 16
#define  WDMA_DRCR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DRCR_RESERVED1_MASK) >> WDMA_DRCR_RESERVED1_SHIFT)
#define  WDMA_DRCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DRCR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DRCR_RESERVED1_SHIFT))

#define  WDMA_DRCR_ROWCNT_MASK 0xffff
#define  WDMA_DRCR_ROWCNT_SHIFT 0
#define  WDMA_DRCR_ROWCNT_MASK_SHIFT(reg) (((reg) & WDMA_DRCR_ROWCNT_MASK) >> WDMA_DRCR_ROWCNT_SHIFT)
#define  WDMA_DRCR_ROWCNT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DRCR_ROWCNT_MASK) | (((uint32_t)val) << WDMA_DRCR_ROWCNT_SHIFT))

//====================================================================
//Register: JPEG WDMA Image Height Register (DIHR)
//====================================================================

#define  WDMA_DIHR_RESERVED1_MASK 0xff000000
#define  WDMA_DIHR_RESERVED1_SHIFT 24
#define  WDMA_DIHR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DIHR_RESERVED1_MASK) >> WDMA_DIHR_RESERVED1_SHIFT)
#define  WDMA_DIHR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DIHR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DIHR_RESERVED1_SHIFT))

#define  WDMA_DIHR_IMAGEHEIGHT_MASK 0xffffff
#define  WDMA_DIHR_IMAGEHEIGHT_SHIFT 0
#define  WDMA_DIHR_IMAGEHEIGHT_MASK_SHIFT(reg) (((reg) & WDMA_DIHR_IMAGEHEIGHT_MASK) >> WDMA_DIHR_IMAGEHEIGHT_SHIFT)
#define  WDMA_DIHR_IMAGEHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DIHR_IMAGEHEIGHT_MASK) | (((uint32_t)val) << WDMA_DIHR_IMAGEHEIGHT_SHIFT))

//====================================================================
//Register: JPEG WDMA Line Width Register (DLWR)
//====================================================================

#define  WDMA_DLWR_RESERVED1_MASK 0xff000000
#define  WDMA_DLWR_RESERVED1_SHIFT 24
#define  WDMA_DLWR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DLWR_RESERVED1_MASK) >> WDMA_DLWR_RESERVED1_SHIFT)
#define  WDMA_DLWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DLWR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DLWR_RESERVED1_SHIFT))

#define  WDMA_DLWR_LWIDTH_MASK 0xffffff
#define  WDMA_DLWR_LWIDTH_SHIFT 0
#define  WDMA_DLWR_LWIDTH_MASK_SHIFT(reg) (((reg) & WDMA_DLWR_LWIDTH_MASK) >> WDMA_DLWR_LWIDTH_SHIFT)
#define  WDMA_DLWR_LWIDTH_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DLWR_LWIDTH_MASK) | (((uint32_t)val) << WDMA_DLWR_LWIDTH_SHIFT))

//====================================================================
//Register: JPEG WDMA Transfer Count Register (DTCR)
//====================================================================

#define  WDMA_DTCR_XFERCNT_MASK 0xffffffff
#define  WDMA_DTCR_XFERCNT_SHIFT 0
#define  WDMA_DTCR_XFERCNT_MASK_SHIFT(reg) (((reg) & WDMA_DTCR_XFERCNT_MASK) >> WDMA_DTCR_XFERCNT_SHIFT)
#define  WDMA_DTCR_XFERCNT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DTCR_XFERCNT_MASK) | (((uint32_t)val) << WDMA_DTCR_XFERCNT_SHIFT))

//====================================================================
//Register: JPEG WDMA Start Register (DSTR)
/** \brief Writing any value to this register will start a DMA transaction.
        Since it is Write Only, reading this register returns 32'hDEADDEAD*/
//====================================================================

#define  WDMA_DSTR_RESERVED1_MASK 0xfffffffe
#define  WDMA_DSTR_RESERVED1_SHIFT 1
#define  WDMA_DSTR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DSTR_RESERVED1_MASK) >> WDMA_DSTR_RESERVED1_SHIFT)
#define  WDMA_DSTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DSTR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DSTR_RESERVED1_SHIFT))

#define  WDMA_DSTR_START_MASK 0x1
#define  WDMA_DSTR_START_SHIFT 0
#define  WDMA_DSTR_START_MASK_SHIFT(reg) (((reg) & WDMA_DSTR_START_MASK) >> WDMA_DSTR_START_SHIFT)
#define  WDMA_DSTR_START_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DSTR_START_MASK) | (((uint32_t)val) << WDMA_DSTR_START_SHIFT))

//====================================================================
//Register: JPEG WDMA Address Status Register (DASR)
//====================================================================

#define  WDMA_DASR_ADDR_MASK 0xffffffff
#define  WDMA_DASR_ADDR_SHIFT 0
#define  WDMA_DASR_ADDR_MASK_SHIFT(reg) (((reg) & WDMA_DASR_ADDR_MASK) >> WDMA_DASR_ADDR_SHIFT)
#define  WDMA_DASR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DASR_ADDR_MASK) | (((uint32_t)val) << WDMA_DASR_ADDR_SHIFT))

//====================================================================
//Register: JPEG WDMA Transfer Count Status Register (DTCSR)
//====================================================================

#define  WDMA_DTCSR_XFERCNT_MASK 0xffffffff
#define  WDMA_DTCSR_XFERCNT_SHIFT 0
#define  WDMA_DTCSR_XFERCNT_MASK_SHIFT(reg) (((reg) & WDMA_DTCSR_XFERCNT_MASK) >> WDMA_DTCSR_XFERCNT_SHIFT)
#define  WDMA_DTCSR_XFERCNT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DTCSR_XFERCNT_MASK) | (((uint32_t)val) << WDMA_DTCSR_XFERCNT_SHIFT))

//====================================================================
//Register: JPEG WDMA Row Count Status Register (DRCSR)
//====================================================================

#define  WDMA_DRCSR_RESERVED1_MASK 0xffff0000
#define  WDMA_DRCSR_RESERVED1_SHIFT 16
#define  WDMA_DRCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DRCSR_RESERVED1_MASK) >> WDMA_DRCSR_RESERVED1_SHIFT)
#define  WDMA_DRCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DRCSR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DRCSR_RESERVED1_SHIFT))

#define  WDMA_DRCSR_ROWCNT_MASK 0xffff
#define  WDMA_DRCSR_ROWCNT_SHIFT 0
#define  WDMA_DRCSR_ROWCNT_MASK_SHIFT(reg) (((reg) & WDMA_DRCSR_ROWCNT_MASK) >> WDMA_DRCSR_ROWCNT_SHIFT)
#define  WDMA_DRCSR_ROWCNT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DRCSR_ROWCNT_MASK) | (((uint32_t)val) << WDMA_DRCSR_ROWCNT_SHIFT))

//====================================================================
//Register: JPEG WDMA Status Register (DSR)
//====================================================================

#define  WDMA_DSR_COMP_INT_MASK 0x80000000
#define  WDMA_DSR_COMP_INT_SHIFT 31
#define  WDMA_DSR_COMP_INT_MASK_SHIFT(reg) (((reg) & WDMA_DSR_COMP_INT_MASK) >> WDMA_DSR_COMP_INT_SHIFT)
#define  WDMA_DSR_COMP_INT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DSR_COMP_INT_MASK) | (((uint32_t)val) << WDMA_DSR_COMP_INT_SHIFT))

#define  WDMA_DSR_RESERVED1_MASK 0x7ffffffc
#define  WDMA_DSR_RESERVED1_SHIFT 2
#define  WDMA_DSR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DSR_RESERVED1_MASK) >> WDMA_DSR_RESERVED1_SHIFT)
#define  WDMA_DSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DSR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DSR_RESERVED1_SHIFT))

#define  WDMA_DSR_READY_MASK 0x2
#define  WDMA_DSR_READY_SHIFT 1
#define  WDMA_DSR_READY_MASK_SHIFT(reg) (((reg) & WDMA_DSR_READY_MASK) >> WDMA_DSR_READY_SHIFT)
#define  WDMA_DSR_READY_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DSR_READY_MASK) | (((uint32_t)val) << WDMA_DSR_READY_SHIFT))

#define  WDMA_DSR_ACTIVE_MASK 0x1
#define  WDMA_DSR_ACTIVE_SHIFT 0
#define  WDMA_DSR_ACTIVE_MASK_SHIFT(reg) (((reg) & WDMA_DSR_ACTIVE_MASK) >> WDMA_DSR_ACTIVE_SHIFT)
#define  WDMA_DSR_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DSR_ACTIVE_MASK) | (((uint32_t)val) << WDMA_DSR_ACTIVE_SHIFT))

//====================================================================
//Register: JPEG WDMA Result Register (DRR)
//====================================================================

#define  WDMA_DRR_XFERCNT_MASK 0xfffffffc
#define  WDMA_DRR_XFERCNT_SHIFT 2
#define  WDMA_DRR_XFERCNT_MASK_SHIFT(reg) (((reg) & WDMA_DRR_XFERCNT_MASK) >> WDMA_DRR_XFERCNT_SHIFT)
#define  WDMA_DRR_XFERCNT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DRR_XFERCNT_MASK) | (((uint32_t)val) << WDMA_DRR_XFERCNT_SHIFT))

#define  WDMA_DRR_TERM_MASK 0x2
#define  WDMA_DRR_TERM_SHIFT 1
#define  WDMA_DRR_TERM_MASK_SHIFT(reg) (((reg) & WDMA_DRR_TERM_MASK) >> WDMA_DRR_TERM_SHIFT)
#define  WDMA_DRR_TERM_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DRR_TERM_MASK) | (((uint32_t)val) << WDMA_DRR_TERM_SHIFT))

#define  WDMA_DRR_ERROR_MASK 0x1
#define  WDMA_DRR_ERROR_SHIFT 0
#define  WDMA_DRR_ERROR_MASK_SHIFT(reg) (((reg) & WDMA_DRR_ERROR_MASK) >> WDMA_DRR_ERROR_SHIFT)
#define  WDMA_DRR_ERROR_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DRR_ERROR_MASK) | (((uint32_t)val) << WDMA_DRR_ERROR_SHIFT))

//====================================================================
//Register: JPEG WDMA Interrupt Register (DIR)
//====================================================================

#define  WDMA_DIR_RESERVED1_MASK 0xfffffffe
#define  WDMA_DIR_RESERVED1_SHIFT 1
#define  WDMA_DIR_RESERVED1_MASK_SHIFT(reg) (((reg) & WDMA_DIR_RESERVED1_MASK) >> WDMA_DIR_RESERVED1_SHIFT)
#define  WDMA_DIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DIR_RESERVED1_MASK) | (((uint32_t)val) << WDMA_DIR_RESERVED1_SHIFT))

#define  WDMA_DIR_CLR_INT_MASK 0x1
#define  WDMA_DIR_CLR_INT_SHIFT 0
#define  WDMA_DIR_CLR_INT_MASK_SHIFT(reg) (((reg) & WDMA_DIR_CLR_INT_MASK) >> WDMA_DIR_CLR_INT_SHIFT)
#define  WDMA_DIR_CLR_INT_REPLACE_VAL(reg,val) (((reg) & ~WDMA_DIR_CLR_INT_MASK) | (((uint32_t)val) << WDMA_DIR_CLR_INT_SHIFT))

//====================================================================
//
//Register File: Color Space Conversion Registers (CSC)
/** \brief Register Descriptions for the 1 CSC block*/
//
//====================================================================

//====================================================================
//Register: CSC Configuration Register (CCR)
/** \brief The CSC Configuration Register (CCR) manages the configuration of the CSC block. For normal operation of the CSC block, all bits in this register should be set to '0'.*/
//====================================================================

#define  CSC_CCR_RESERVED1_MASK 0xffffff00
#define  CSC_CCR_RESERVED1_SHIFT 8
#define  CSC_CCR_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_CCR_RESERVED1_MASK) >> CSC_CCR_RESERVED1_SHIFT)
#define  CSC_CCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_RESERVED1_MASK) | (((uint32_t)val) << CSC_CCR_RESERVED1_SHIFT))

#define  CSC_CCR_RESERVED2_MASK 0x80
#define  CSC_CCR_RESERVED2_SHIFT 7
#define  CSC_CCR_RESERVED2_MASK_SHIFT(reg) (((reg) & CSC_CCR_RESERVED2_MASK) >> CSC_CCR_RESERVED2_SHIFT)
#define  CSC_CCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_RESERVED2_MASK) | (((uint32_t)val) << CSC_CCR_RESERVED2_SHIFT))

#define  CSC_CCR_POSTOFFBYPASS_MASK 0x40
#define  CSC_CCR_POSTOFFBYPASS_SHIFT 6
#define  CSC_CCR_POSTOFFBYPASS_MASK_SHIFT(reg) (((reg) & CSC_CCR_POSTOFFBYPASS_MASK) >> CSC_CCR_POSTOFFBYPASS_SHIFT)
#define  CSC_CCR_POSTOFFBYPASS_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_POSTOFFBYPASS_MASK) | (((uint32_t)val) << CSC_CCR_POSTOFFBYPASS_SHIFT))

#define  CSC_CCR_PREOFFBYPASS_MASK 0x20
#define  CSC_CCR_PREOFFBYPASS_SHIFT 5
#define  CSC_CCR_PREOFFBYPASS_MASK_SHIFT(reg) (((reg) & CSC_CCR_PREOFFBYPASS_MASK) >> CSC_CCR_PREOFFBYPASS_SHIFT)
#define  CSC_CCR_PREOFFBYPASS_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_PREOFFBYPASS_MASK) | (((uint32_t)val) << CSC_CCR_PREOFFBYPASS_SHIFT))

#define  CSC_CCR_RESERVED3_MASK 0x10
#define  CSC_CCR_RESERVED3_SHIFT 4
#define  CSC_CCR_RESERVED3_MASK_SHIFT(reg) (((reg) & CSC_CCR_RESERVED3_MASK) >> CSC_CCR_RESERVED3_SHIFT)
#define  CSC_CCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_RESERVED3_MASK) | (((uint32_t)val) << CSC_CCR_RESERVED3_SHIFT))

#define  CSC_CCR_RESERVED4_MASK 0x8
#define  CSC_CCR_RESERVED4_SHIFT 3
#define  CSC_CCR_RESERVED4_MASK_SHIFT(reg) (((reg) & CSC_CCR_RESERVED4_MASK) >> CSC_CCR_RESERVED4_SHIFT)
#define  CSC_CCR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_RESERVED4_MASK) | (((uint32_t)val) << CSC_CCR_RESERVED4_SHIFT))

#define  CSC_CCR_RESERVED5_MASK 0x4
#define  CSC_CCR_RESERVED5_SHIFT 2
#define  CSC_CCR_RESERVED5_MASK_SHIFT(reg) (((reg) & CSC_CCR_RESERVED5_MASK) >> CSC_CCR_RESERVED5_SHIFT)
#define  CSC_CCR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_RESERVED5_MASK) | (((uint32_t)val) << CSC_CCR_RESERVED5_SHIFT))

#define  CSC_CCR_OFFSETBYPASS_MASK 0x2
#define  CSC_CCR_OFFSETBYPASS_SHIFT 1
#define  CSC_CCR_OFFSETBYPASS_MASK_SHIFT(reg) (((reg) & CSC_CCR_OFFSETBYPASS_MASK) >> CSC_CCR_OFFSETBYPASS_SHIFT)
#define  CSC_CCR_OFFSETBYPASS_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_OFFSETBYPASS_MASK) | (((uint32_t)val) << CSC_CCR_OFFSETBYPASS_SHIFT))

#define  CSC_CCR_BYPASSALL_MASK 0x1
#define  CSC_CCR_BYPASSALL_SHIFT 0
#define  CSC_CCR_BYPASSALL_MASK_SHIFT(reg) (((reg) & CSC_CCR_BYPASSALL_MASK) >> CSC_CCR_BYPASSALL_SHIFT)
#define  CSC_CCR_BYPASSALL_REPLACE_VAL(reg,val) (((reg) & ~CSC_CCR_BYPASSALL_MASK) | (((uint32_t)val) << CSC_CCR_BYPASSALL_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 1 of 9) (MCR0)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 1 of 9)*/
//====================================================================

#define  CSC_MCR0_RESERVED1_MASK 0xffffc000
#define  CSC_MCR0_RESERVED1_SHIFT 14
#define  CSC_MCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR0_RESERVED1_MASK) >> CSC_MCR0_RESERVED1_SHIFT)
#define  CSC_MCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR0_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR0_RESERVED1_SHIFT))

#define  CSC_MCR0_COEFF_MASK 0x3fff
#define  CSC_MCR0_COEFF_SHIFT 0
#define  CSC_MCR0_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR0_COEFF_MASK) >> CSC_MCR0_COEFF_SHIFT)
#define  CSC_MCR0_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR0_COEFF_MASK) | (((uint32_t)val) << CSC_MCR0_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 2 of 9) (MCR1)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 2 of 9)*/
//====================================================================

#define  CSC_MCR1_RESERVED1_MASK 0xffffc000
#define  CSC_MCR1_RESERVED1_SHIFT 14
#define  CSC_MCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR1_RESERVED1_MASK) >> CSC_MCR1_RESERVED1_SHIFT)
#define  CSC_MCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR1_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR1_RESERVED1_SHIFT))

#define  CSC_MCR1_COEFF_MASK 0x3fff
#define  CSC_MCR1_COEFF_SHIFT 0
#define  CSC_MCR1_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR1_COEFF_MASK) >> CSC_MCR1_COEFF_SHIFT)
#define  CSC_MCR1_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR1_COEFF_MASK) | (((uint32_t)val) << CSC_MCR1_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 3 of 9) (MCR2)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 3 of 9)*/
//====================================================================

#define  CSC_MCR2_RESERVED1_MASK 0xffffc000
#define  CSC_MCR2_RESERVED1_SHIFT 14
#define  CSC_MCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR2_RESERVED1_MASK) >> CSC_MCR2_RESERVED1_SHIFT)
#define  CSC_MCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR2_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR2_RESERVED1_SHIFT))

#define  CSC_MCR2_COEFF_MASK 0x3fff
#define  CSC_MCR2_COEFF_SHIFT 0
#define  CSC_MCR2_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR2_COEFF_MASK) >> CSC_MCR2_COEFF_SHIFT)
#define  CSC_MCR2_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR2_COEFF_MASK) | (((uint32_t)val) << CSC_MCR2_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 4 of 9) (MCR3)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 4 of 9)*/
//====================================================================

#define  CSC_MCR3_RESERVED1_MASK 0xffffc000
#define  CSC_MCR3_RESERVED1_SHIFT 14
#define  CSC_MCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR3_RESERVED1_MASK) >> CSC_MCR3_RESERVED1_SHIFT)
#define  CSC_MCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR3_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR3_RESERVED1_SHIFT))

#define  CSC_MCR3_COEFF_MASK 0x3fff
#define  CSC_MCR3_COEFF_SHIFT 0
#define  CSC_MCR3_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR3_COEFF_MASK) >> CSC_MCR3_COEFF_SHIFT)
#define  CSC_MCR3_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR3_COEFF_MASK) | (((uint32_t)val) << CSC_MCR3_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 5 of 9) (MCR4)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 5 of 9)*/
//====================================================================

#define  CSC_MCR4_RESERVED1_MASK 0xffffc000
#define  CSC_MCR4_RESERVED1_SHIFT 14
#define  CSC_MCR4_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR4_RESERVED1_MASK) >> CSC_MCR4_RESERVED1_SHIFT)
#define  CSC_MCR4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR4_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR4_RESERVED1_SHIFT))

#define  CSC_MCR4_COEFF_MASK 0x3fff
#define  CSC_MCR4_COEFF_SHIFT 0
#define  CSC_MCR4_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR4_COEFF_MASK) >> CSC_MCR4_COEFF_SHIFT)
#define  CSC_MCR4_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR4_COEFF_MASK) | (((uint32_t)val) << CSC_MCR4_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 6 of 9) (MCR5)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 6 of 9)*/
//====================================================================

#define  CSC_MCR5_RESERVED1_MASK 0xffffc000
#define  CSC_MCR5_RESERVED1_SHIFT 14
#define  CSC_MCR5_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR5_RESERVED1_MASK) >> CSC_MCR5_RESERVED1_SHIFT)
#define  CSC_MCR5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR5_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR5_RESERVED1_SHIFT))

#define  CSC_MCR5_COEFF_MASK 0x3fff
#define  CSC_MCR5_COEFF_SHIFT 0
#define  CSC_MCR5_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR5_COEFF_MASK) >> CSC_MCR5_COEFF_SHIFT)
#define  CSC_MCR5_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR5_COEFF_MASK) | (((uint32_t)val) << CSC_MCR5_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 7 of 9) (MCR6)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 7 of 9)*/
//====================================================================

#define  CSC_MCR6_RESERVED1_MASK 0xffffc000
#define  CSC_MCR6_RESERVED1_SHIFT 14
#define  CSC_MCR6_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR6_RESERVED1_MASK) >> CSC_MCR6_RESERVED1_SHIFT)
#define  CSC_MCR6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR6_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR6_RESERVED1_SHIFT))

#define  CSC_MCR6_COEFF_MASK 0x3fff
#define  CSC_MCR6_COEFF_SHIFT 0
#define  CSC_MCR6_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR6_COEFF_MASK) >> CSC_MCR6_COEFF_SHIFT)
#define  CSC_MCR6_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR6_COEFF_MASK) | (((uint32_t)val) << CSC_MCR6_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 8 of 9) (MCR7)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 8 of 9)*/
//====================================================================

#define  CSC_MCR7_RESERVED1_MASK 0xffffc000
#define  CSC_MCR7_RESERVED1_SHIFT 14
#define  CSC_MCR7_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR7_RESERVED1_MASK) >> CSC_MCR7_RESERVED1_SHIFT)
#define  CSC_MCR7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR7_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR7_RESERVED1_SHIFT))

#define  CSC_MCR7_COEFF_MASK 0x3fff
#define  CSC_MCR7_COEFF_SHIFT 0
#define  CSC_MCR7_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR7_COEFF_MASK) >> CSC_MCR7_COEFF_SHIFT)
#define  CSC_MCR7_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR7_COEFF_MASK) | (((uint32_t)val) << CSC_MCR7_COEFF_SHIFT))

//====================================================================
//Register: Multiply Coefficient Register (Instance 9 of 9) (MCR8)
/** \brief <p class=Body>
      There are nine Multiply Coefficient Registers (MCRn), each of which stores one
      coefficient needed for the 3x3 multiply. MCR0 stores the coefficient for row 1, column 1
      of the coefficient matrix; MCR1 stores the coefficient for row 1, column 2, etc. In other
      words, the 'n' in the address is equal to [(row-1)*3]+(column-1).
      </p>
      <p class=Body>
      The coefficients are stored as signed 2's compliment values. Each coefficient represents 
      a number in the range (-2,2). This range is obtained by normalizing the value in the
      register by 4096 . For positive numbers (MSB = 0), the second MSB will be interpreted
      as the integer portion of the number, and the remaining bits will represent the fractional
      portion. </p> (Instance 9 of 9)*/
//====================================================================

#define  CSC_MCR8_RESERVED1_MASK 0xffffc000
#define  CSC_MCR8_RESERVED1_SHIFT 14
#define  CSC_MCR8_RESERVED1_MASK_SHIFT(reg) (((reg) & CSC_MCR8_RESERVED1_MASK) >> CSC_MCR8_RESERVED1_SHIFT)
#define  CSC_MCR8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR8_RESERVED1_MASK) | (((uint32_t)val) << CSC_MCR8_RESERVED1_SHIFT))

#define  CSC_MCR8_COEFF_MASK 0x3fff
#define  CSC_MCR8_COEFF_SHIFT 0
#define  CSC_MCR8_COEFF_MASK_SHIFT(reg) (((reg) & CSC_MCR8_COEFF_MASK) >> CSC_MCR8_COEFF_SHIFT)
#define  CSC_MCR8_COEFF_REPLACE_VAL(reg,val) (((reg) & ~CSC_MCR8_COEFF_MASK) | (((uint32_t)val) << CSC_MCR8_COEFF_SHIFT))

//====================================================================
//
//Register File: MCU_JPEG Registers (MCU_JPEG)
/** \brief Top-level register file for MCU JPEG block*/
//
//====================================================================

//====================================================================
//Register: MCU_JPEG Enable (MJEN)
/** \brief The McuJpeg Enable Register is used to start the MCU_JPEG operation by writing 1 to it.*/
//====================================================================

#define  MCU_JPEG_MJEN_RESERVED1_MASK 0xfffffffe
#define  MCU_JPEG_MJEN_RESERVED1_SHIFT 1
#define  MCU_JPEG_MJEN_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJEN_RESERVED1_MASK) >> MCU_JPEG_MJEN_RESERVED1_SHIFT)
#define  MCU_JPEG_MJEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJEN_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_MJEN_RESERVED1_SHIFT))

#define  MCU_JPEG_MJEN_ENABLE_MASK 0x1
#define  MCU_JPEG_MJEN_ENABLE_SHIFT 0
#define  MCU_JPEG_MJEN_ENABLE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJEN_ENABLE_MASK) >> MCU_JPEG_MJEN_ENABLE_SHIFT)
#define  MCU_JPEG_MJEN_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJEN_ENABLE_MASK) | (((uint32_t)val) << MCU_JPEG_MJEN_ENABLE_SHIFT))

//====================================================================
//Register: MCU_JPEG Control (MJCTL)
/** \brief The McuJpeg Control Register is used to control the configuration of the core and the MCU generation blocks.*/
//====================================================================

#define  MCU_JPEG_MJCTL_RESERVED1_MASK 0xfffff800
#define  MCU_JPEG_MJCTL_RESERVED1_SHIFT 11
#define  MCU_JPEG_MJCTL_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_RESERVED1_MASK) >> MCU_JPEG_MJCTL_RESERVED1_SHIFT)
#define  MCU_JPEG_MJCTL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_RESERVED1_SHIFT))

#define  MCU_JPEG_MJCTL_STANDALONEROT_MASK 0x400
#define  MCU_JPEG_MJCTL_STANDALONEROT_SHIFT 10
#define  MCU_JPEG_MJCTL_STANDALONEROT_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_STANDALONEROT_MASK) >> MCU_JPEG_MJCTL_STANDALONEROT_SHIFT)
#define  MCU_JPEG_MJCTL_STANDALONEROT_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_STANDALONEROT_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_STANDALONEROT_SHIFT))

#define  MCU_JPEG_MJCTL_MCUROTCONFIG_MASK 0x380
#define  MCU_JPEG_MJCTL_MCUROTCONFIG_SHIFT 7
#define  MCU_JPEG_MJCTL_MCUROTCONFIG_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_MCUROTCONFIG_MASK) >> MCU_JPEG_MJCTL_MCUROTCONFIG_SHIFT)
#define  MCU_JPEG_MJCTL_MCUROTCONFIG_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_MCUROTCONFIG_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_MCUROTCONFIG_SHIFT))

#define  MCU_JPEG_MJCTL_MCUBYPASS_MASK 0x40
#define  MCU_JPEG_MJCTL_MCUBYPASS_SHIFT 6
#define  MCU_JPEG_MJCTL_MCUBYPASS_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_MCUBYPASS_MASK) >> MCU_JPEG_MJCTL_MCUBYPASS_SHIFT)
#define  MCU_JPEG_MJCTL_MCUBYPASS_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_MCUBYPASS_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_MCUBYPASS_SHIFT))

#define  MCU_JPEG_MJCTL_CSCONLY_MASK 0x20
#define  MCU_JPEG_MJCTL_CSCONLY_SHIFT 5
#define  MCU_JPEG_MJCTL_CSCONLY_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_CSCONLY_MASK) >> MCU_JPEG_MJCTL_CSCONLY_SHIFT)
#define  MCU_JPEG_MJCTL_CSCONLY_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_CSCONLY_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_CSCONLY_SHIFT))

#define  MCU_JPEG_MJCTL_AVERAGING_MASK 0x10
#define  MCU_JPEG_MJCTL_AVERAGING_SHIFT 4
#define  MCU_JPEG_MJCTL_AVERAGING_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_AVERAGING_MASK) >> MCU_JPEG_MJCTL_AVERAGING_SHIFT)
#define  MCU_JPEG_MJCTL_AVERAGING_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_AVERAGING_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_AVERAGING_SHIFT))

#define  MCU_JPEG_MJCTL_SUBSAMPLE_MASK 0xe
#define  MCU_JPEG_MJCTL_SUBSAMPLE_SHIFT 1
#define  MCU_JPEG_MJCTL_SUBSAMPLE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_SUBSAMPLE_MASK) >> MCU_JPEG_MJCTL_SUBSAMPLE_SHIFT)
#define  MCU_JPEG_MJCTL_SUBSAMPLE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_SUBSAMPLE_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_SUBSAMPLE_SHIFT))

#define  MCU_JPEG_MJCTL_DECODE_MASK 0x1
#define  MCU_JPEG_MJCTL_DECODE_SHIFT 0
#define  MCU_JPEG_MJCTL_DECODE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJCTL_DECODE_MASK) >> MCU_JPEG_MJCTL_DECODE_SHIFT)
#define  MCU_JPEG_MJCTL_DECODE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJCTL_DECODE_MASK) | (((uint32_t)val) << MCU_JPEG_MJCTL_DECODE_SHIFT))

//====================================================================
//Register: MCU_JPEG Scaler Control (MJSCL)
/** \brief The McuJpeg Scaler Control defines the type of scaling if enabled.*/
//====================================================================

#define  MCU_JPEG_MJSCL_RESERVED1_MASK 0xf0000000
#define  MCU_JPEG_MJSCL_RESERVED1_SHIFT 28
#define  MCU_JPEG_MJSCL_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJSCL_RESERVED1_MASK) >> MCU_JPEG_MJSCL_RESERVED1_SHIFT)
#define  MCU_JPEG_MJSCL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJSCL_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_MJSCL_RESERVED1_SHIFT))

#define  MCU_JPEG_MJSCL_HSCALING_MASK 0xf000000
#define  MCU_JPEG_MJSCL_HSCALING_SHIFT 24
#define  MCU_JPEG_MJSCL_HSCALING_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJSCL_HSCALING_MASK) >> MCU_JPEG_MJSCL_HSCALING_SHIFT)
#define  MCU_JPEG_MJSCL_HSCALING_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJSCL_HSCALING_MASK) | (((uint32_t)val) << MCU_JPEG_MJSCL_HSCALING_SHIFT))

#define  MCU_JPEG_MJSCL_RESERVED2_MASK 0xf00000
#define  MCU_JPEG_MJSCL_RESERVED2_SHIFT 20
#define  MCU_JPEG_MJSCL_RESERVED2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJSCL_RESERVED2_MASK) >> MCU_JPEG_MJSCL_RESERVED2_SHIFT)
#define  MCU_JPEG_MJSCL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJSCL_RESERVED2_MASK) | (((uint32_t)val) << MCU_JPEG_MJSCL_RESERVED2_SHIFT))

#define  MCU_JPEG_MJSCL_VSCALING_MASK 0xf0000
#define  MCU_JPEG_MJSCL_VSCALING_SHIFT 16
#define  MCU_JPEG_MJSCL_VSCALING_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJSCL_VSCALING_MASK) >> MCU_JPEG_MJSCL_VSCALING_SHIFT)
#define  MCU_JPEG_MJSCL_VSCALING_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJSCL_VSCALING_MASK) | (((uint32_t)val) << MCU_JPEG_MJSCL_VSCALING_SHIFT))

#define  MCU_JPEG_MJSCL_RESERVED3_MASK 0xfffe
#define  MCU_JPEG_MJSCL_RESERVED3_SHIFT 1
#define  MCU_JPEG_MJSCL_RESERVED3_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJSCL_RESERVED3_MASK) >> MCU_JPEG_MJSCL_RESERVED3_SHIFT)
#define  MCU_JPEG_MJSCL_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJSCL_RESERVED3_MASK) | (((uint32_t)val) << MCU_JPEG_MJSCL_RESERVED3_SHIFT))

#define  MCU_JPEG_MJSCL_SCALEENABLE_MASK 0x1
#define  MCU_JPEG_MJSCL_SCALEENABLE_SHIFT 0
#define  MCU_JPEG_MJSCL_SCALEENABLE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJSCL_SCALEENABLE_MASK) >> MCU_JPEG_MJSCL_SCALEENABLE_SHIFT)
#define  MCU_JPEG_MJSCL_SCALEENABLE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJSCL_SCALEENABLE_MASK) | (((uint32_t)val) << MCU_JPEG_MJSCL_SCALEENABLE_SHIFT))

//====================================================================
//Register: JPEG Memory Access Control (JMACTL)
/** \brief Ths register specifies the Memory type to be accessed and the address along with whether its a read/write operation. Write to this only if JMOCS[0] is 1.*/
//====================================================================

#define  MCU_JPEG_JMACTL_RESERVED1_MASK 0xfffe0000
#define  MCU_JPEG_JMACTL_RESERVED1_SHIFT 17
#define  MCU_JPEG_JMACTL_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMACTL_RESERVED1_MASK) >> MCU_JPEG_JMACTL_RESERVED1_SHIFT)
#define  MCU_JPEG_JMACTL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMACTL_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JMACTL_RESERVED1_SHIFT))

#define  MCU_JPEG_JMACTL_LOCATION_MASK 0x1ff00
#define  MCU_JPEG_JMACTL_LOCATION_SHIFT 8
#define  MCU_JPEG_JMACTL_LOCATION_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMACTL_LOCATION_MASK) >> MCU_JPEG_JMACTL_LOCATION_SHIFT)
#define  MCU_JPEG_JMACTL_LOCATION_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMACTL_LOCATION_MASK) | (((uint32_t)val) << MCU_JPEG_JMACTL_LOCATION_SHIFT))

#define  MCU_JPEG_JMACTL_RESERVED2_MASK 0xf8
#define  MCU_JPEG_JMACTL_RESERVED2_SHIFT 3
#define  MCU_JPEG_JMACTL_RESERVED2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMACTL_RESERVED2_MASK) >> MCU_JPEG_JMACTL_RESERVED2_SHIFT)
#define  MCU_JPEG_JMACTL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMACTL_RESERVED2_MASK) | (((uint32_t)val) << MCU_JPEG_JMACTL_RESERVED2_SHIFT))

#define  MCU_JPEG_JMACTL_WRITE_MASK 0x4
#define  MCU_JPEG_JMACTL_WRITE_SHIFT 2
#define  MCU_JPEG_JMACTL_WRITE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMACTL_WRITE_MASK) >> MCU_JPEG_JMACTL_WRITE_SHIFT)
#define  MCU_JPEG_JMACTL_WRITE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMACTL_WRITE_MASK) | (((uint32_t)val) << MCU_JPEG_JMACTL_WRITE_SHIFT))

#define  MCU_JPEG_JMACTL_TYPE_MASK 0x3
#define  MCU_JPEG_JMACTL_TYPE_SHIFT 0
#define  MCU_JPEG_JMACTL_TYPE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMACTL_TYPE_MASK) >> MCU_JPEG_JMACTL_TYPE_SHIFT)
#define  MCU_JPEG_JMACTL_TYPE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMACTL_TYPE_MASK) | (((uint32_t)val) << MCU_JPEG_JMACTL_TYPE_SHIFT))

//====================================================================
//Register: JPEG Memory Data (JMDATA)
/** \brief The Jpeg memory Data register stores the data to be written/read from the memory. Write to this only if JMOCS[0] is 1.*/
//====================================================================

#define  MCU_JPEG_JMDATA_RESERVED1_MASK 0xfffff000
#define  MCU_JPEG_JMDATA_RESERVED1_SHIFT 12
#define  MCU_JPEG_JMDATA_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMDATA_RESERVED1_MASK) >> MCU_JPEG_JMDATA_RESERVED1_SHIFT)
#define  MCU_JPEG_JMDATA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMDATA_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JMDATA_RESERVED1_SHIFT))

#define  MCU_JPEG_JMDATA_DATA_MASK 0xfff
#define  MCU_JPEG_JMDATA_DATA_SHIFT 0
#define  MCU_JPEG_JMDATA_DATA_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMDATA_DATA_MASK) >> MCU_JPEG_JMDATA_DATA_SHIFT)
#define  MCU_JPEG_JMDATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMDATA_DATA_MASK) | (((uint32_t)val) << MCU_JPEG_JMDATA_DATA_SHIFT))

//====================================================================
//Register: JPEG Memory Operation Control/Status (JMOCS)
/** \brief The Jpeg memory operation control register which also reflects the status of the operation. Always set by hardware. 1 reflects command executed and ready for next command.*/
//====================================================================

#define  MCU_JPEG_JMOCS_RESERVED1_MASK 0xfffffffe
#define  MCU_JPEG_JMOCS_RESERVED1_SHIFT 1
#define  MCU_JPEG_JMOCS_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMOCS_RESERVED1_MASK) >> MCU_JPEG_JMOCS_RESERVED1_SHIFT)
#define  MCU_JPEG_JMOCS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMOCS_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JMOCS_RESERVED1_SHIFT))

#define  MCU_JPEG_JMOCS_STATUS_MASK 0x1
#define  MCU_JPEG_JMOCS_STATUS_SHIFT 0
#define  MCU_JPEG_JMOCS_STATUS_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JMOCS_STATUS_MASK) >> MCU_JPEG_JMOCS_STATUS_SHIFT)
#define  MCU_JPEG_JMOCS_STATUS_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JMOCS_STATUS_MASK) | (((uint32_t)val) << MCU_JPEG_JMOCS_STATUS_SHIFT))

//====================================================================
//Register: MCU_JPEG Over (MJINT)
/** \brief The register which reflects the interrupt status of McuJpeg.*/
//====================================================================

#define  MCU_JPEG_MJINT_RESERVED1_MASK 0xfffffffe
#define  MCU_JPEG_MJINT_RESERVED1_SHIFT 1
#define  MCU_JPEG_MJINT_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJINT_RESERVED1_MASK) >> MCU_JPEG_MJINT_RESERVED1_SHIFT)
#define  MCU_JPEG_MJINT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJINT_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_MJINT_RESERVED1_SHIFT))

#define  MCU_JPEG_MJINT_INT_MASK 0x1
#define  MCU_JPEG_MJINT_INT_SHIFT 0
#define  MCU_JPEG_MJINT_INT_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJINT_INT_MASK) >> MCU_JPEG_MJINT_INT_SHIFT)
#define  MCU_JPEG_MJINT_INT_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJINT_INT_MASK) | (((uint32_t)val) << MCU_JPEG_MJINT_INT_SHIFT))

//====================================================================
//Register: Soft Reset (MJRST)
/** \brief Soft Reset for MCU ordering JPEG core and CSC blocks.*/
//====================================================================

#define  MCU_JPEG_MJRST_RESERVED1_MASK 0xfffffffe
#define  MCU_JPEG_MJRST_RESERVED1_SHIFT 1
#define  MCU_JPEG_MJRST_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJRST_RESERVED1_MASK) >> MCU_JPEG_MJRST_RESERVED1_SHIFT)
#define  MCU_JPEG_MJRST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJRST_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_MJRST_RESERVED1_SHIFT))

#define  MCU_JPEG_MJRST_RESET_MASK 0x1
#define  MCU_JPEG_MJRST_RESET_SHIFT 0
#define  MCU_JPEG_MJRST_RESET_MASK_SHIFT(reg) (((reg) & MCU_JPEG_MJRST_RESET_MASK) >> MCU_JPEG_MJRST_RESET_SHIFT)
#define  MCU_JPEG_MJRST_RESET_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_MJRST_RESET_MASK) | (((uint32_t)val) << MCU_JPEG_MJRST_RESET_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 0 (JREG0)
/** \brief The register to start and stop the encoding/decoding process.*/
//====================================================================

#define  MCU_JPEG_JREG0_RESERVED1_MASK 0xfffffffe
#define  MCU_JPEG_JREG0_RESERVED1_SHIFT 1
#define  MCU_JPEG_JREG0_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG0_RESERVED1_MASK) >> MCU_JPEG_JREG0_RESERVED1_SHIFT)
#define  MCU_JPEG_JREG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG0_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG0_RESERVED1_SHIFT))

#define  MCU_JPEG_JREG0_START_MASK 0x1
#define  MCU_JPEG_JREG0_START_SHIFT 0
#define  MCU_JPEG_JREG0_START_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG0_START_MASK) >> MCU_JPEG_JREG0_START_SHIFT)
#define  MCU_JPEG_JREG0_START_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG0_START_MASK) | (((uint32_t)val) << MCU_JPEG_JREG0_START_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 1 (JREG1)
//====================================================================

#define  MCU_JPEG_JREG1_YSIZ_MASK 0xffff0000
#define  MCU_JPEG_JREG1_YSIZ_SHIFT 16
#define  MCU_JPEG_JREG1_YSIZ_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_YSIZ_MASK) >> MCU_JPEG_JREG1_YSIZ_SHIFT)
#define  MCU_JPEG_JREG1_YSIZ_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_YSIZ_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_YSIZ_SHIFT))

#define  MCU_JPEG_JREG1_RESERVED1_MASK 0xfe00
#define  MCU_JPEG_JREG1_RESERVED1_SHIFT 9
#define  MCU_JPEG_JREG1_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_RESERVED1_MASK) >> MCU_JPEG_JREG1_RESERVED1_SHIFT)
#define  MCU_JPEG_JREG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_RESERVED1_SHIFT))

#define  MCU_JPEG_JREG1_HDR_MASK 0x100
#define  MCU_JPEG_JREG1_HDR_SHIFT 8
#define  MCU_JPEG_JREG1_HDR_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_HDR_MASK) >> MCU_JPEG_JREG1_HDR_SHIFT)
#define  MCU_JPEG_JREG1_HDR_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_HDR_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_HDR_SHIFT))

#define  MCU_JPEG_JREG1_NS_MASK 0xc0
#define  MCU_JPEG_JREG1_NS_SHIFT 6
#define  MCU_JPEG_JREG1_NS_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_NS_MASK) >> MCU_JPEG_JREG1_NS_SHIFT)
#define  MCU_JPEG_JREG1_NS_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_NS_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_NS_SHIFT))

#define  MCU_JPEG_JREG1_COLSPCTYPE_MASK 0x30
#define  MCU_JPEG_JREG1_COLSPCTYPE_SHIFT 4
#define  MCU_JPEG_JREG1_COLSPCTYPE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_COLSPCTYPE_MASK) >> MCU_JPEG_JREG1_COLSPCTYPE_SHIFT)
#define  MCU_JPEG_JREG1_COLSPCTYPE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_COLSPCTYPE_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_COLSPCTYPE_SHIFT))

#define  MCU_JPEG_JREG1_DE_MASK 0x8
#define  MCU_JPEG_JREG1_DE_SHIFT 3
#define  MCU_JPEG_JREG1_DE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_DE_MASK) >> MCU_JPEG_JREG1_DE_SHIFT)
#define  MCU_JPEG_JREG1_DE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_DE_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_DE_SHIFT))

#define  MCU_JPEG_JREG1_RE_MASK 0x4
#define  MCU_JPEG_JREG1_RE_SHIFT 2
#define  MCU_JPEG_JREG1_RE_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_RE_MASK) >> MCU_JPEG_JREG1_RE_SHIFT)
#define  MCU_JPEG_JREG1_RE_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_RE_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_RE_SHIFT))

#define  MCU_JPEG_JREG1_NF_MASK 0x3
#define  MCU_JPEG_JREG1_NF_SHIFT 0
#define  MCU_JPEG_JREG1_NF_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG1_NF_MASK) >> MCU_JPEG_JREG1_NF_SHIFT)
#define  MCU_JPEG_JREG1_NF_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG1_NF_MASK) | (((uint32_t)val) << MCU_JPEG_JREG1_NF_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 2 (JREG2)
/** \brief Number of MCU*/
//====================================================================

#define  MCU_JPEG_JREG2_RESERVED1_MASK 0xfc000000
#define  MCU_JPEG_JREG2_RESERVED1_SHIFT 26
#define  MCU_JPEG_JREG2_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG2_RESERVED1_MASK) >> MCU_JPEG_JREG2_RESERVED1_SHIFT)
#define  MCU_JPEG_JREG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG2_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG2_RESERVED1_SHIFT))

#define  MCU_JPEG_JREG2_NMCU_MASK 0x3ffffff
#define  MCU_JPEG_JREG2_NMCU_SHIFT 0
#define  MCU_JPEG_JREG2_NMCU_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG2_NMCU_MASK) >> MCU_JPEG_JREG2_NMCU_SHIFT)
#define  MCU_JPEG_JREG2_NMCU_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG2_NMCU_MASK) | (((uint32_t)val) << MCU_JPEG_JREG2_NMCU_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 3 (JREG3)
//====================================================================

#define  MCU_JPEG_JREG3_XSIZ_MASK 0xffff0000
#define  MCU_JPEG_JREG3_XSIZ_SHIFT 16
#define  MCU_JPEG_JREG3_XSIZ_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG3_XSIZ_MASK) >> MCU_JPEG_JREG3_XSIZ_SHIFT)
#define  MCU_JPEG_JREG3_XSIZ_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG3_XSIZ_MASK) | (((uint32_t)val) << MCU_JPEG_JREG3_XSIZ_SHIFT))

#define  MCU_JPEG_JREG3_NRST_MASK 0xffff
#define  MCU_JPEG_JREG3_NRST_SHIFT 0
#define  MCU_JPEG_JREG3_NRST_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG3_NRST_MASK) >> MCU_JPEG_JREG3_NRST_SHIFT)
#define  MCU_JPEG_JREG3_NRST_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG3_NRST_MASK) | (((uint32_t)val) << MCU_JPEG_JREG3_NRST_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 4 (JREG4)
/** \brief Register for color component 1*/
//====================================================================

#define  MCU_JPEG_JREG4_RESERVED1_MASK 0xffff0000
#define  MCU_JPEG_JREG4_RESERVED1_SHIFT 16
#define  MCU_JPEG_JREG4_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG4_RESERVED1_MASK) >> MCU_JPEG_JREG4_RESERVED1_SHIFT)
#define  MCU_JPEG_JREG4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG4_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG4_RESERVED1_SHIFT))

#define  MCU_JPEG_JREG4_H1_MASK 0xf000
#define  MCU_JPEG_JREG4_H1_SHIFT 12
#define  MCU_JPEG_JREG4_H1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG4_H1_MASK) >> MCU_JPEG_JREG4_H1_SHIFT)
#define  MCU_JPEG_JREG4_H1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG4_H1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG4_H1_SHIFT))

#define  MCU_JPEG_JREG4_V1_MASK 0xf00
#define  MCU_JPEG_JREG4_V1_SHIFT 8
#define  MCU_JPEG_JREG4_V1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG4_V1_MASK) >> MCU_JPEG_JREG4_V1_SHIFT)
#define  MCU_JPEG_JREG4_V1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG4_V1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG4_V1_SHIFT))

#define  MCU_JPEG_JREG4_NBLOCK1_MASK 0xf0
#define  MCU_JPEG_JREG4_NBLOCK1_SHIFT 4
#define  MCU_JPEG_JREG4_NBLOCK1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG4_NBLOCK1_MASK) >> MCU_JPEG_JREG4_NBLOCK1_SHIFT)
#define  MCU_JPEG_JREG4_NBLOCK1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG4_NBLOCK1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG4_NBLOCK1_SHIFT))

#define  MCU_JPEG_JREG4_QT1_MASK 0xc
#define  MCU_JPEG_JREG4_QT1_SHIFT 2
#define  MCU_JPEG_JREG4_QT1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG4_QT1_MASK) >> MCU_JPEG_JREG4_QT1_SHIFT)
#define  MCU_JPEG_JREG4_QT1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG4_QT1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG4_QT1_SHIFT))

#define  MCU_JPEG_JREG4_HA1_MASK 0x2
#define  MCU_JPEG_JREG4_HA1_SHIFT 1
#define  MCU_JPEG_JREG4_HA1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG4_HA1_MASK) >> MCU_JPEG_JREG4_HA1_SHIFT)
#define  MCU_JPEG_JREG4_HA1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG4_HA1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG4_HA1_SHIFT))

#define  MCU_JPEG_JREG4_HD1_MASK 0x1
#define  MCU_JPEG_JREG4_HD1_SHIFT 0
#define  MCU_JPEG_JREG4_HD1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG4_HD1_MASK) >> MCU_JPEG_JREG4_HD1_SHIFT)
#define  MCU_JPEG_JREG4_HD1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG4_HD1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG4_HD1_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 5 (JREG5)
/** \brief Register for color component 2*/
//====================================================================

#define  MCU_JPEG_JREG5_RESERVED1_MASK 0xffff0000
#define  MCU_JPEG_JREG5_RESERVED1_SHIFT 16
#define  MCU_JPEG_JREG5_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG5_RESERVED1_MASK) >> MCU_JPEG_JREG5_RESERVED1_SHIFT)
#define  MCU_JPEG_JREG5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG5_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG5_RESERVED1_SHIFT))

#define  MCU_JPEG_JREG5_H2_MASK 0xf000
#define  MCU_JPEG_JREG5_H2_SHIFT 12
#define  MCU_JPEG_JREG5_H2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG5_H2_MASK) >> MCU_JPEG_JREG5_H2_SHIFT)
#define  MCU_JPEG_JREG5_H2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG5_H2_MASK) | (((uint32_t)val) << MCU_JPEG_JREG5_H2_SHIFT))

#define  MCU_JPEG_JREG5_V2_MASK 0xf00
#define  MCU_JPEG_JREG5_V2_SHIFT 8
#define  MCU_JPEG_JREG5_V2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG5_V2_MASK) >> MCU_JPEG_JREG5_V2_SHIFT)
#define  MCU_JPEG_JREG5_V2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG5_V2_MASK) | (((uint32_t)val) << MCU_JPEG_JREG5_V2_SHIFT))

#define  MCU_JPEG_JREG5_NBLOCK2_MASK 0xf0
#define  MCU_JPEG_JREG5_NBLOCK2_SHIFT 4
#define  MCU_JPEG_JREG5_NBLOCK2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG5_NBLOCK2_MASK) >> MCU_JPEG_JREG5_NBLOCK2_SHIFT)
#define  MCU_JPEG_JREG5_NBLOCK2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG5_NBLOCK2_MASK) | (((uint32_t)val) << MCU_JPEG_JREG5_NBLOCK2_SHIFT))

#define  MCU_JPEG_JREG5_QT2_MASK 0xc
#define  MCU_JPEG_JREG5_QT2_SHIFT 2
#define  MCU_JPEG_JREG5_QT2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG5_QT2_MASK) >> MCU_JPEG_JREG5_QT2_SHIFT)
#define  MCU_JPEG_JREG5_QT2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG5_QT2_MASK) | (((uint32_t)val) << MCU_JPEG_JREG5_QT2_SHIFT))

#define  MCU_JPEG_JREG5_HA2_MASK 0x2
#define  MCU_JPEG_JREG5_HA2_SHIFT 1
#define  MCU_JPEG_JREG5_HA2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG5_HA2_MASK) >> MCU_JPEG_JREG5_HA2_SHIFT)
#define  MCU_JPEG_JREG5_HA2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG5_HA2_MASK) | (((uint32_t)val) << MCU_JPEG_JREG5_HA2_SHIFT))

#define  MCU_JPEG_JREG5_HD2_MASK 0x1
#define  MCU_JPEG_JREG5_HD2_SHIFT 0
#define  MCU_JPEG_JREG5_HD2_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG5_HD2_MASK) >> MCU_JPEG_JREG5_HD2_SHIFT)
#define  MCU_JPEG_JREG5_HD2_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG5_HD2_MASK) | (((uint32_t)val) << MCU_JPEG_JREG5_HD2_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 6 (JREG6)
/** \brief Register for color component 3*/
//====================================================================

#define  MCU_JPEG_JREG6_RESERVED1_MASK 0xffff0000
#define  MCU_JPEG_JREG6_RESERVED1_SHIFT 16
#define  MCU_JPEG_JREG6_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG6_RESERVED1_MASK) >> MCU_JPEG_JREG6_RESERVED1_SHIFT)
#define  MCU_JPEG_JREG6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG6_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG6_RESERVED1_SHIFT))

#define  MCU_JPEG_JREG6_H3_MASK 0xf000
#define  MCU_JPEG_JREG6_H3_SHIFT 12
#define  MCU_JPEG_JREG6_H3_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG6_H3_MASK) >> MCU_JPEG_JREG6_H3_SHIFT)
#define  MCU_JPEG_JREG6_H3_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG6_H3_MASK) | (((uint32_t)val) << MCU_JPEG_JREG6_H3_SHIFT))

#define  MCU_JPEG_JREG6_V3_MASK 0xf00
#define  MCU_JPEG_JREG6_V3_SHIFT 8
#define  MCU_JPEG_JREG6_V3_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG6_V3_MASK) >> MCU_JPEG_JREG6_V3_SHIFT)
#define  MCU_JPEG_JREG6_V3_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG6_V3_MASK) | (((uint32_t)val) << MCU_JPEG_JREG6_V3_SHIFT))

#define  MCU_JPEG_JREG6_NBLOCK3_MASK 0xf0
#define  MCU_JPEG_JREG6_NBLOCK3_SHIFT 4
#define  MCU_JPEG_JREG6_NBLOCK3_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG6_NBLOCK3_MASK) >> MCU_JPEG_JREG6_NBLOCK3_SHIFT)
#define  MCU_JPEG_JREG6_NBLOCK3_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG6_NBLOCK3_MASK) | (((uint32_t)val) << MCU_JPEG_JREG6_NBLOCK3_SHIFT))

#define  MCU_JPEG_JREG6_QT3_MASK 0xc
#define  MCU_JPEG_JREG6_QT3_SHIFT 2
#define  MCU_JPEG_JREG6_QT3_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG6_QT3_MASK) >> MCU_JPEG_JREG6_QT3_SHIFT)
#define  MCU_JPEG_JREG6_QT3_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG6_QT3_MASK) | (((uint32_t)val) << MCU_JPEG_JREG6_QT3_SHIFT))

#define  MCU_JPEG_JREG6_HA3_MASK 0x2
#define  MCU_JPEG_JREG6_HA3_SHIFT 1
#define  MCU_JPEG_JREG6_HA3_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG6_HA3_MASK) >> MCU_JPEG_JREG6_HA3_SHIFT)
#define  MCU_JPEG_JREG6_HA3_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG6_HA3_MASK) | (((uint32_t)val) << MCU_JPEG_JREG6_HA3_SHIFT))

#define  MCU_JPEG_JREG6_HD3_MASK 0x1
#define  MCU_JPEG_JREG6_HD3_SHIFT 0
#define  MCU_JPEG_JREG6_HD3_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG6_HD3_MASK) >> MCU_JPEG_JREG6_HD3_SHIFT)
#define  MCU_JPEG_JREG6_HD3_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG6_HD3_MASK) | (((uint32_t)val) << MCU_JPEG_JREG6_HD3_SHIFT))

//====================================================================
//Register: JPEG Core Internal Register 7 (JREG7)
/** \brief Register for color component 4*/
//====================================================================

#define  MCU_JPEG_JREG7_RESERVED1_MASK 0xffff0000
#define  MCU_JPEG_JREG7_RESERVED1_SHIFT 16
#define  MCU_JPEG_JREG7_RESERVED1_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG7_RESERVED1_MASK) >> MCU_JPEG_JREG7_RESERVED1_SHIFT)
#define  MCU_JPEG_JREG7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG7_RESERVED1_MASK) | (((uint32_t)val) << MCU_JPEG_JREG7_RESERVED1_SHIFT))

#define  MCU_JPEG_JREG7_H4_MASK 0xf000
#define  MCU_JPEG_JREG7_H4_SHIFT 12
#define  MCU_JPEG_JREG7_H4_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG7_H4_MASK) >> MCU_JPEG_JREG7_H4_SHIFT)
#define  MCU_JPEG_JREG7_H4_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG7_H4_MASK) | (((uint32_t)val) << MCU_JPEG_JREG7_H4_SHIFT))

#define  MCU_JPEG_JREG7_V4_MASK 0xf00
#define  MCU_JPEG_JREG7_V4_SHIFT 8
#define  MCU_JPEG_JREG7_V4_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG7_V4_MASK) >> MCU_JPEG_JREG7_V4_SHIFT)
#define  MCU_JPEG_JREG7_V4_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG7_V4_MASK) | (((uint32_t)val) << MCU_JPEG_JREG7_V4_SHIFT))

#define  MCU_JPEG_JREG7_NBLOCK4_MASK 0xf0
#define  MCU_JPEG_JREG7_NBLOCK4_SHIFT 4
#define  MCU_JPEG_JREG7_NBLOCK4_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG7_NBLOCK4_MASK) >> MCU_JPEG_JREG7_NBLOCK4_SHIFT)
#define  MCU_JPEG_JREG7_NBLOCK4_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG7_NBLOCK4_MASK) | (((uint32_t)val) << MCU_JPEG_JREG7_NBLOCK4_SHIFT))

#define  MCU_JPEG_JREG7_QT4_MASK 0xc
#define  MCU_JPEG_JREG7_QT4_SHIFT 2
#define  MCU_JPEG_JREG7_QT4_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG7_QT4_MASK) >> MCU_JPEG_JREG7_QT4_SHIFT)
#define  MCU_JPEG_JREG7_QT4_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG7_QT4_MASK) | (((uint32_t)val) << MCU_JPEG_JREG7_QT4_SHIFT))

#define  MCU_JPEG_JREG7_HA4_MASK 0x2
#define  MCU_JPEG_JREG7_HA4_SHIFT 1
#define  MCU_JPEG_JREG7_HA4_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG7_HA4_MASK) >> MCU_JPEG_JREG7_HA4_SHIFT)
#define  MCU_JPEG_JREG7_HA4_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG7_HA4_MASK) | (((uint32_t)val) << MCU_JPEG_JREG7_HA4_SHIFT))

#define  MCU_JPEG_JREG7_HD4_MASK 0x1
#define  MCU_JPEG_JREG7_HD4_SHIFT 0
#define  MCU_JPEG_JREG7_HD4_MASK_SHIFT(reg) (((reg) & MCU_JPEG_JREG7_HD4_MASK) >> MCU_JPEG_JREG7_HD4_SHIFT)
#define  MCU_JPEG_JREG7_HD4_REPLACE_VAL(reg,val) (((reg) & ~MCU_JPEG_JREG7_HD4_MASK) | (((uint32_t)val) << MCU_JPEG_JREG7_HD4_SHIFT))

#endif // JEDI
