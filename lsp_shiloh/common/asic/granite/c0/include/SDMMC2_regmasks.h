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
 * \file SDMMC2_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SDMMC2_REGMASKS_H_
#define _SDMMC2_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SDMMC2 Card Interface (SDMMC2)
/** \brief Top-level register file for SDMMC2 Host Interface Block*/
//
//====================================================================

//====================================================================
//Register: System Address Low Register (Instance 1 of 2) (SD_SYS_ADDR_LOW0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_MASK 0xffff
#define  SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_SHIFT 0
#define  SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_MASK) >> SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_SHIFT)
#define  SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_MASK) | (((uint16_t)val) << SDMMC2_SD_SYS_ADDR_LOW0_DMA_ADDR_L_SHIFT))

//====================================================================
//Register: System Address Low Register (Instance 2 of 2) (SD_SYS_ADDR_LOW1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_MASK 0xffff
#define  SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_SHIFT 0
#define  SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_MASK) >> SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_SHIFT)
#define  SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_MASK) | (((uint16_t)val) << SDMMC2_SD_SYS_ADDR_LOW1_DMA_ADDR_L_SHIFT))

//====================================================================
//Register: System Address High Register (Instance 1 of 2) (SD_SYS_ADDR_HIGH0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_MASK 0xffff
#define  SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_SHIFT 0
#define  SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_MASK) >> SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_SHIFT)
#define  SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_MASK) | (((uint16_t)val) << SDMMC2_SD_SYS_ADDR_HIGH0_DMA_ADDR_H_SHIFT))

//====================================================================
//Register: System Address High Register (Instance 2 of 2) (SD_SYS_ADDR_HIGH1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_MASK 0xffff
#define  SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_SHIFT 0
#define  SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_MASK) >> SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_SHIFT)
#define  SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_MASK) | (((uint16_t)val) << SDMMC2_SD_SYS_ADDR_HIGH1_DMA_ADDR_H_SHIFT))

//====================================================================
//Register: Block Size Register (Instance 1 of 2) (SD_BLOCK_SIZE0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_BLOCK_SIZE0_RESERVED2_MASK 0x8000
#define  SDMMC2_SD_BLOCK_SIZE0_RESERVED2_SHIFT 15
#define  SDMMC2_SD_BLOCK_SIZE0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_SIZE0_RESERVED2_MASK) >> SDMMC2_SD_BLOCK_SIZE0_RESERVED2_SHIFT)
#define  SDMMC2_SD_BLOCK_SIZE0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_SIZE0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_SIZE0_RESERVED2_SHIFT))

#define  SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_MASK 0x7000
#define  SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_SHIFT 12
#define  SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_MASK) >> SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_SHIFT)
#define  SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_SIZE0_HOST_DMA_BDRY_SHIFT))

#define  SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_MASK 0xfff
#define  SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_SHIFT 0
#define  SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_MASK) >> SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_SHIFT)
#define  SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_SIZE0_BLOCK_SIZE_SHIFT))

//====================================================================
//Register: Block Size Register (Instance 2 of 2) (SD_BLOCK_SIZE1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_BLOCK_SIZE1_RESERVED2_MASK 0x8000
#define  SDMMC2_SD_BLOCK_SIZE1_RESERVED2_SHIFT 15
#define  SDMMC2_SD_BLOCK_SIZE1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_SIZE1_RESERVED2_MASK) >> SDMMC2_SD_BLOCK_SIZE1_RESERVED2_SHIFT)
#define  SDMMC2_SD_BLOCK_SIZE1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_SIZE1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_SIZE1_RESERVED2_SHIFT))

#define  SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_MASK 0x7000
#define  SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_SHIFT 12
#define  SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_MASK) >> SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_SHIFT)
#define  SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_SIZE1_HOST_DMA_BDRY_SHIFT))

#define  SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_MASK 0xfff
#define  SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_SHIFT 0
#define  SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_MASK) >> SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_SHIFT)
#define  SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_SIZE1_BLOCK_SIZE_SHIFT))

//====================================================================
//Register: Block Count Register (Instance 1 of 2) (SD_BLOCK_COUNT0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_MASK 0xffff
#define  SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_SHIFT 0
#define  SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_MASK) >> SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_SHIFT)
#define  SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_COUNT0_BLOCK_COUNT_SHIFT))

//====================================================================
//Register: Block Count Register (Instance 2 of 2) (SD_BLOCK_COUNT1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_MASK 0xffff
#define  SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_SHIFT 0
#define  SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_MASK) >> SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_SHIFT)
#define  SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_COUNT1_BLOCK_COUNT_SHIFT))

//====================================================================
//Register: Argument Low Register (Instance 1 of 2) (SD_ARG_LOW0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ARG_LOW0_ARG_L_MASK 0xffff
#define  SDMMC2_SD_ARG_LOW0_ARG_L_SHIFT 0
#define  SDMMC2_SD_ARG_LOW0_ARG_L_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ARG_LOW0_ARG_L_MASK) >> SDMMC2_SD_ARG_LOW0_ARG_L_SHIFT)
#define  SDMMC2_SD_ARG_LOW0_ARG_L_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ARG_LOW0_ARG_L_MASK) | (((uint16_t)val) << SDMMC2_SD_ARG_LOW0_ARG_L_SHIFT))

//====================================================================
//Register: Argument Low Register (Instance 2 of 2) (SD_ARG_LOW1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ARG_LOW1_ARG_L_MASK 0xffff
#define  SDMMC2_SD_ARG_LOW1_ARG_L_SHIFT 0
#define  SDMMC2_SD_ARG_LOW1_ARG_L_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ARG_LOW1_ARG_L_MASK) >> SDMMC2_SD_ARG_LOW1_ARG_L_SHIFT)
#define  SDMMC2_SD_ARG_LOW1_ARG_L_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ARG_LOW1_ARG_L_MASK) | (((uint16_t)val) << SDMMC2_SD_ARG_LOW1_ARG_L_SHIFT))

//====================================================================
//Register: Argument High Register (Instance 1 of 2) (SD_ARG_HIGH0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ARG_HIGH0_ARG_H_MASK 0xffff
#define  SDMMC2_SD_ARG_HIGH0_ARG_H_SHIFT 0
#define  SDMMC2_SD_ARG_HIGH0_ARG_H_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ARG_HIGH0_ARG_H_MASK) >> SDMMC2_SD_ARG_HIGH0_ARG_H_SHIFT)
#define  SDMMC2_SD_ARG_HIGH0_ARG_H_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ARG_HIGH0_ARG_H_MASK) | (((uint16_t)val) << SDMMC2_SD_ARG_HIGH0_ARG_H_SHIFT))

//====================================================================
//Register: Argument High Register (Instance 2 of 2) (SD_ARG_HIGH1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ARG_HIGH1_ARG_H_MASK 0xffff
#define  SDMMC2_SD_ARG_HIGH1_ARG_H_SHIFT 0
#define  SDMMC2_SD_ARG_HIGH1_ARG_H_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ARG_HIGH1_ARG_H_MASK) >> SDMMC2_SD_ARG_HIGH1_ARG_H_SHIFT)
#define  SDMMC2_SD_ARG_HIGH1_ARG_H_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ARG_HIGH1_ARG_H_MASK) | (((uint16_t)val) << SDMMC2_SD_ARG_HIGH1_ARG_H_SHIFT))

//====================================================================
//Register: Transfer Mode Register (Instance 1 of 2) (SD_TRANSFER_MODE0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED2_MASK 0xffc0
#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED2_SHIFT 6
#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE0_RESERVED2_MASK) >> SDMMC2_SD_TRANSFER_MODE0_RESERVED2_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE0_RESERVED2_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_MASK 0x20
#define  SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_SHIFT 5
#define  SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_MASK) >> SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE0_MULTI_BLK_SEL_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_MASK 0x10
#define  SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_SHIFT 4
#define  SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_MASK) >> SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE0_TO_HOST_DIR_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED3_MASK 0x8
#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED3_SHIFT 3
#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE0_RESERVED3_MASK) >> SDMMC2_SD_TRANSFER_MODE0_RESERVED3_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE0_RESERVED3_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_MASK 0x4
#define  SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_SHIFT 2
#define  SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_MASK) >> SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE0_AUTO_CMD12_EN_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_MASK 0x2
#define  SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_SHIFT 1
#define  SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_MASK) >> SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE0_BLK_CNT_EN_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE0_DMA_EN_MASK 0x1
#define  SDMMC2_SD_TRANSFER_MODE0_DMA_EN_SHIFT 0
#define  SDMMC2_SD_TRANSFER_MODE0_DMA_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE0_DMA_EN_MASK) >> SDMMC2_SD_TRANSFER_MODE0_DMA_EN_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE0_DMA_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE0_DMA_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE0_DMA_EN_SHIFT))

//====================================================================
//Register: Transfer Mode Register (Instance 2 of 2) (SD_TRANSFER_MODE1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED2_MASK 0xffc0
#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED2_SHIFT 6
#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE1_RESERVED2_MASK) >> SDMMC2_SD_TRANSFER_MODE1_RESERVED2_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE1_RESERVED2_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_MASK 0x20
#define  SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_SHIFT 5
#define  SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_MASK) >> SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE1_MULTI_BLK_SEL_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_MASK 0x10
#define  SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_SHIFT 4
#define  SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_MASK) >> SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE1_TO_HOST_DIR_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED3_MASK 0x8
#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED3_SHIFT 3
#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE1_RESERVED3_MASK) >> SDMMC2_SD_TRANSFER_MODE1_RESERVED3_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE1_RESERVED3_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_MASK 0x4
#define  SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_SHIFT 2
#define  SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_MASK) >> SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE1_AUTO_CMD12_EN_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_MASK 0x2
#define  SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_SHIFT 1
#define  SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_MASK) >> SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE1_BLK_CNT_EN_SHIFT))

#define  SDMMC2_SD_TRANSFER_MODE1_DMA_EN_MASK 0x1
#define  SDMMC2_SD_TRANSFER_MODE1_DMA_EN_SHIFT 0
#define  SDMMC2_SD_TRANSFER_MODE1_DMA_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TRANSFER_MODE1_DMA_EN_MASK) >> SDMMC2_SD_TRANSFER_MODE1_DMA_EN_SHIFT)
#define  SDMMC2_SD_TRANSFER_MODE1_DMA_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TRANSFER_MODE1_DMA_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_TRANSFER_MODE1_DMA_EN_SHIFT))

//====================================================================
//Register: Command Register (Instance 1 of 2) (SD_CMD0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CMD0_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CMD0_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CMD0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_RESERVED2_MASK) >> SDMMC2_SD_CMD0_RESERVED2_SHIFT)
#define  SDMMC2_SD_CMD0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_RESERVED2_SHIFT))

#define  SDMMC2_SD_CMD0_CMD_INDEX_MASK 0x3f00
#define  SDMMC2_SD_CMD0_CMD_INDEX_SHIFT 8
#define  SDMMC2_SD_CMD0_CMD_INDEX_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_CMD_INDEX_MASK) >> SDMMC2_SD_CMD0_CMD_INDEX_SHIFT)
#define  SDMMC2_SD_CMD0_CMD_INDEX_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_CMD_INDEX_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_CMD_INDEX_SHIFT))

#define  SDMMC2_SD_CMD0_CMD_TYPE_MASK 0xc0
#define  SDMMC2_SD_CMD0_CMD_TYPE_SHIFT 6
#define  SDMMC2_SD_CMD0_CMD_TYPE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_CMD_TYPE_MASK) >> SDMMC2_SD_CMD0_CMD_TYPE_SHIFT)
#define  SDMMC2_SD_CMD0_CMD_TYPE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_CMD_TYPE_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_CMD_TYPE_SHIFT))

#define  SDMMC2_SD_CMD0_DATA_PRESENT_MASK 0x20
#define  SDMMC2_SD_CMD0_DATA_PRESENT_SHIFT 5
#define  SDMMC2_SD_CMD0_DATA_PRESENT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_DATA_PRESENT_MASK) >> SDMMC2_SD_CMD0_DATA_PRESENT_SHIFT)
#define  SDMMC2_SD_CMD0_DATA_PRESENT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_DATA_PRESENT_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_DATA_PRESENT_SHIFT))

#define  SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_MASK 0x10
#define  SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_SHIFT 4
#define  SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_MASK) >> SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_SHIFT)
#define  SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_CMD_INDEX_CHK_EN_SHIFT))

#define  SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_MASK 0x8
#define  SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_SHIFT 3
#define  SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_MASK) >> SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_SHIFT)
#define  SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_CMD_CRC_CHK_EN_SHIFT))

#define  SDMMC2_SD_CMD0_RESERVED3_MASK 0x4
#define  SDMMC2_SD_CMD0_RESERVED3_SHIFT 2
#define  SDMMC2_SD_CMD0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_RESERVED3_MASK) >> SDMMC2_SD_CMD0_RESERVED3_SHIFT)
#define  SDMMC2_SD_CMD0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_RESERVED3_SHIFT))

#define  SDMMC2_SD_CMD0_RESP_TYPE_MASK 0x3
#define  SDMMC2_SD_CMD0_RESP_TYPE_SHIFT 0
#define  SDMMC2_SD_CMD0_RESP_TYPE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD0_RESP_TYPE_MASK) >> SDMMC2_SD_CMD0_RESP_TYPE_SHIFT)
#define  SDMMC2_SD_CMD0_RESP_TYPE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD0_RESP_TYPE_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD0_RESP_TYPE_SHIFT))

//====================================================================
//Register: Command Register (Instance 2 of 2) (SD_CMD1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CMD1_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CMD1_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CMD1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_RESERVED2_MASK) >> SDMMC2_SD_CMD1_RESERVED2_SHIFT)
#define  SDMMC2_SD_CMD1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_RESERVED2_SHIFT))

#define  SDMMC2_SD_CMD1_CMD_INDEX_MASK 0x3f00
#define  SDMMC2_SD_CMD1_CMD_INDEX_SHIFT 8
#define  SDMMC2_SD_CMD1_CMD_INDEX_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_CMD_INDEX_MASK) >> SDMMC2_SD_CMD1_CMD_INDEX_SHIFT)
#define  SDMMC2_SD_CMD1_CMD_INDEX_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_CMD_INDEX_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_CMD_INDEX_SHIFT))

#define  SDMMC2_SD_CMD1_CMD_TYPE_MASK 0xc0
#define  SDMMC2_SD_CMD1_CMD_TYPE_SHIFT 6
#define  SDMMC2_SD_CMD1_CMD_TYPE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_CMD_TYPE_MASK) >> SDMMC2_SD_CMD1_CMD_TYPE_SHIFT)
#define  SDMMC2_SD_CMD1_CMD_TYPE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_CMD_TYPE_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_CMD_TYPE_SHIFT))

#define  SDMMC2_SD_CMD1_DATA_PRESENT_MASK 0x20
#define  SDMMC2_SD_CMD1_DATA_PRESENT_SHIFT 5
#define  SDMMC2_SD_CMD1_DATA_PRESENT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_DATA_PRESENT_MASK) >> SDMMC2_SD_CMD1_DATA_PRESENT_SHIFT)
#define  SDMMC2_SD_CMD1_DATA_PRESENT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_DATA_PRESENT_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_DATA_PRESENT_SHIFT))

#define  SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_MASK 0x10
#define  SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_SHIFT 4
#define  SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_MASK) >> SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_SHIFT)
#define  SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_CMD_INDEX_CHK_EN_SHIFT))

#define  SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_MASK 0x8
#define  SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_SHIFT 3
#define  SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_MASK) >> SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_SHIFT)
#define  SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_CMD_CRC_CHK_EN_SHIFT))

#define  SDMMC2_SD_CMD1_RESERVED3_MASK 0x4
#define  SDMMC2_SD_CMD1_RESERVED3_SHIFT 2
#define  SDMMC2_SD_CMD1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_RESERVED3_MASK) >> SDMMC2_SD_CMD1_RESERVED3_SHIFT)
#define  SDMMC2_SD_CMD1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_RESERVED3_SHIFT))

#define  SDMMC2_SD_CMD1_RESP_TYPE_MASK 0x3
#define  SDMMC2_SD_CMD1_RESP_TYPE_SHIFT 0
#define  SDMMC2_SD_CMD1_RESP_TYPE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CMD1_RESP_TYPE_MASK) >> SDMMC2_SD_CMD1_RESP_TYPE_SHIFT)
#define  SDMMC2_SD_CMD1_RESP_TYPE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CMD1_RESP_TYPE_MASK) | (((uint16_t)val) << SDMMC2_SD_CMD1_RESP_TYPE_SHIFT))

//====================================================================
//Register: Response Register 0 (Instance 1 of 2) (SD_RESP_00)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_00_RESP0_MASK 0xffff
#define  SDMMC2_SD_RESP_00_RESP0_SHIFT 0
#define  SDMMC2_SD_RESP_00_RESP0_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_00_RESP0_MASK) >> SDMMC2_SD_RESP_00_RESP0_SHIFT)
#define  SDMMC2_SD_RESP_00_RESP0_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_00_RESP0_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_00_RESP0_SHIFT))

//====================================================================
//Register: Response Register 0 (Instance 2 of 2) (SD_RESP_01)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_01_RESP0_MASK 0xffff
#define  SDMMC2_SD_RESP_01_RESP0_SHIFT 0
#define  SDMMC2_SD_RESP_01_RESP0_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_01_RESP0_MASK) >> SDMMC2_SD_RESP_01_RESP0_SHIFT)
#define  SDMMC2_SD_RESP_01_RESP0_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_01_RESP0_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_01_RESP0_SHIFT))

//====================================================================
//Register: Response Register 1 (Instance 1 of 2) (SD_RESP_10)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_10_RESP1_MASK 0xffff
#define  SDMMC2_SD_RESP_10_RESP1_SHIFT 0
#define  SDMMC2_SD_RESP_10_RESP1_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_10_RESP1_MASK) >> SDMMC2_SD_RESP_10_RESP1_SHIFT)
#define  SDMMC2_SD_RESP_10_RESP1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_10_RESP1_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_10_RESP1_SHIFT))

//====================================================================
//Register: Response Register 1 (Instance 2 of 2) (SD_RESP_11)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_11_RESP1_MASK 0xffff
#define  SDMMC2_SD_RESP_11_RESP1_SHIFT 0
#define  SDMMC2_SD_RESP_11_RESP1_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_11_RESP1_MASK) >> SDMMC2_SD_RESP_11_RESP1_SHIFT)
#define  SDMMC2_SD_RESP_11_RESP1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_11_RESP1_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_11_RESP1_SHIFT))

//====================================================================
//Register: Response Register 2 (Instance 1 of 2) (SD_RESP_20)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_20_RESP2_MASK 0xffff
#define  SDMMC2_SD_RESP_20_RESP2_SHIFT 0
#define  SDMMC2_SD_RESP_20_RESP2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_20_RESP2_MASK) >> SDMMC2_SD_RESP_20_RESP2_SHIFT)
#define  SDMMC2_SD_RESP_20_RESP2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_20_RESP2_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_20_RESP2_SHIFT))

//====================================================================
//Register: Response Register 2 (Instance 2 of 2) (SD_RESP_21)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_21_RESP2_MASK 0xffff
#define  SDMMC2_SD_RESP_21_RESP2_SHIFT 0
#define  SDMMC2_SD_RESP_21_RESP2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_21_RESP2_MASK) >> SDMMC2_SD_RESP_21_RESP2_SHIFT)
#define  SDMMC2_SD_RESP_21_RESP2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_21_RESP2_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_21_RESP2_SHIFT))

//====================================================================
//Register: Response Register 3 (Instance 1 of 2) (SD_RESP_30)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_30_RESP3_MASK 0xffff
#define  SDMMC2_SD_RESP_30_RESP3_SHIFT 0
#define  SDMMC2_SD_RESP_30_RESP3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_30_RESP3_MASK) >> SDMMC2_SD_RESP_30_RESP3_SHIFT)
#define  SDMMC2_SD_RESP_30_RESP3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_30_RESP3_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_30_RESP3_SHIFT))

//====================================================================
//Register: Response Register 3 (Instance 2 of 2) (SD_RESP_31)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_31_RESP3_MASK 0xffff
#define  SDMMC2_SD_RESP_31_RESP3_SHIFT 0
#define  SDMMC2_SD_RESP_31_RESP3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_31_RESP3_MASK) >> SDMMC2_SD_RESP_31_RESP3_SHIFT)
#define  SDMMC2_SD_RESP_31_RESP3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_31_RESP3_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_31_RESP3_SHIFT))

//====================================================================
//Register: Response Register 4 (Instance 1 of 2) (SD_RESP_40)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_40_RESP4_MASK 0xffff
#define  SDMMC2_SD_RESP_40_RESP4_SHIFT 0
#define  SDMMC2_SD_RESP_40_RESP4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_40_RESP4_MASK) >> SDMMC2_SD_RESP_40_RESP4_SHIFT)
#define  SDMMC2_SD_RESP_40_RESP4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_40_RESP4_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_40_RESP4_SHIFT))

//====================================================================
//Register: Response Register 4 (Instance 2 of 2) (SD_RESP_41)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_41_RESP4_MASK 0xffff
#define  SDMMC2_SD_RESP_41_RESP4_SHIFT 0
#define  SDMMC2_SD_RESP_41_RESP4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_41_RESP4_MASK) >> SDMMC2_SD_RESP_41_RESP4_SHIFT)
#define  SDMMC2_SD_RESP_41_RESP4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_41_RESP4_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_41_RESP4_SHIFT))

//====================================================================
//Register: Response Register 5 (Instance 1 of 2) (SD_RESP_50)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_50_RESP5_MASK 0xffff
#define  SDMMC2_SD_RESP_50_RESP5_SHIFT 0
#define  SDMMC2_SD_RESP_50_RESP5_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_50_RESP5_MASK) >> SDMMC2_SD_RESP_50_RESP5_SHIFT)
#define  SDMMC2_SD_RESP_50_RESP5_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_50_RESP5_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_50_RESP5_SHIFT))

//====================================================================
//Register: Response Register 5 (Instance 2 of 2) (SD_RESP_51)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_51_RESP5_MASK 0xffff
#define  SDMMC2_SD_RESP_51_RESP5_SHIFT 0
#define  SDMMC2_SD_RESP_51_RESP5_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_51_RESP5_MASK) >> SDMMC2_SD_RESP_51_RESP5_SHIFT)
#define  SDMMC2_SD_RESP_51_RESP5_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_51_RESP5_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_51_RESP5_SHIFT))

//====================================================================
//Register: Response Register 6 (Instance 1 of 2) (SD_RESP_60)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_60_RESP6_MASK 0xffff
#define  SDMMC2_SD_RESP_60_RESP6_SHIFT 0
#define  SDMMC2_SD_RESP_60_RESP6_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_60_RESP6_MASK) >> SDMMC2_SD_RESP_60_RESP6_SHIFT)
#define  SDMMC2_SD_RESP_60_RESP6_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_60_RESP6_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_60_RESP6_SHIFT))

//====================================================================
//Register: Response Register 6 (Instance 2 of 2) (SD_RESP_61)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_61_RESP6_MASK 0xffff
#define  SDMMC2_SD_RESP_61_RESP6_SHIFT 0
#define  SDMMC2_SD_RESP_61_RESP6_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_61_RESP6_MASK) >> SDMMC2_SD_RESP_61_RESP6_SHIFT)
#define  SDMMC2_SD_RESP_61_RESP6_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_61_RESP6_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_61_RESP6_SHIFT))

//====================================================================
//Register: Response Register 7 (Instance 1 of 2) (SD_RESP_70)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_70_RESP7_MASK 0xffff
#define  SDMMC2_SD_RESP_70_RESP7_SHIFT 0
#define  SDMMC2_SD_RESP_70_RESP7_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_70_RESP7_MASK) >> SDMMC2_SD_RESP_70_RESP7_SHIFT)
#define  SDMMC2_SD_RESP_70_RESP7_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_70_RESP7_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_70_RESP7_SHIFT))

//====================================================================
//Register: Response Register 7 (Instance 2 of 2) (SD_RESP_71)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_RESP_71_RESP7_MASK 0xffff
#define  SDMMC2_SD_RESP_71_RESP7_SHIFT 0
#define  SDMMC2_SD_RESP_71_RESP7_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_RESP_71_RESP7_MASK) >> SDMMC2_SD_RESP_71_RESP7_SHIFT)
#define  SDMMC2_SD_RESP_71_RESP7_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_RESP_71_RESP7_MASK) | (((uint16_t)val) << SDMMC2_SD_RESP_71_RESP7_SHIFT))

//====================================================================
//Register: Buffer Data Port 0 Register (Instance 1 of 2) (SD_BUFFER_DATA_PORT_00)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_MASK 0xffff
#define  SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_SHIFT 0
#define  SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_MASK) >> SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_SHIFT)
#define  SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_MASK) | (((uint16_t)val) << SDMMC2_SD_BUFFER_DATA_PORT_00_CPU_DATA0_SHIFT))

//====================================================================
//Register: Buffer Data Port 0 Register (Instance 2 of 2) (SD_BUFFER_DATA_PORT_01)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_MASK 0xffff
#define  SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_SHIFT 0
#define  SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_MASK) >> SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_SHIFT)
#define  SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_MASK) | (((uint16_t)val) << SDMMC2_SD_BUFFER_DATA_PORT_01_CPU_DATA0_SHIFT))

//====================================================================
//Register: Buffer Data Port 1 Register (Instance 1 of 2) (SD_BUFFER_DATA_PORT_10)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_MASK 0xffff
#define  SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_SHIFT 0
#define  SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_MASK) >> SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_SHIFT)
#define  SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_MASK) | (((uint16_t)val) << SDMMC2_SD_BUFFER_DATA_PORT_10_CPU_DATA1_SHIFT))

//====================================================================
//Register: Buffer Data Port 1 Register (Instance 2 of 2) (SD_BUFFER_DATA_PORT_11)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_MASK 0xffff
#define  SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_SHIFT 0
#define  SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_MASK) >> SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_SHIFT)
#define  SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_MASK) | (((uint16_t)val) << SDMMC2_SD_BUFFER_DATA_PORT_11_CPU_DATA1_SHIFT))

//====================================================================
//Register: Present State Register 0 (Instance 1 of 2) (SD_PRESENT_STATE_00)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED2_MASK 0xf000
#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED2_SHIFT 12
#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_RESERVED2_MASK) >> SDMMC2_SD_PRESENT_STATE_00_RESERVED2_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_RESERVED2_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_MASK 0x800
#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_SHIFT 11
#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_MASK) >> SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_BUFFER_RD_EN_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_MASK 0x400
#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_SHIFT 10
#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_MASK) >> SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_BUFFER_WR_EN_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_MASK 0x200
#define  SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_SHIFT 9
#define  SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_MASK) >> SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_RX_ACTIVE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_MASK 0x100
#define  SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_SHIFT 8
#define  SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_MASK) >> SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_TX_ACTIVE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED3_MASK 0xf8
#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED3_SHIFT 3
#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_RESERVED3_MASK) >> SDMMC2_SD_PRESENT_STATE_00_RESERVED3_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_RESERVED3_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_MASK 0x4
#define  SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_SHIFT 2
#define  SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_MASK) >> SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_DAT_ACTIVE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_MASK 0x2
#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_SHIFT 1
#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_MASK) >> SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_DAT_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_MASK 0x1
#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_SHIFT 0
#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_MASK) >> SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_00_CMD_INHIBIT_CMD_SHIFT))

//====================================================================
//Register: Present State Register 0 (Instance 2 of 2) (SD_PRESENT_STATE_01)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED2_MASK 0xf000
#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED2_SHIFT 12
#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_RESERVED2_MASK) >> SDMMC2_SD_PRESENT_STATE_01_RESERVED2_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_RESERVED2_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_MASK 0x800
#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_SHIFT 11
#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_MASK) >> SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_BUFFER_RD_EN_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_MASK 0x400
#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_SHIFT 10
#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_MASK) >> SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_BUFFER_WR_EN_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_MASK 0x200
#define  SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_SHIFT 9
#define  SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_MASK) >> SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_RX_ACTIVE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_MASK 0x100
#define  SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_SHIFT 8
#define  SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_MASK) >> SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_TX_ACTIVE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED3_MASK 0xf8
#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED3_SHIFT 3
#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_RESERVED3_MASK) >> SDMMC2_SD_PRESENT_STATE_01_RESERVED3_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_RESERVED3_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_MASK 0x4
#define  SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_SHIFT 2
#define  SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_MASK) >> SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_DAT_ACTIVE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_MASK 0x2
#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_SHIFT 1
#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_MASK) >> SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_DAT_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_MASK 0x1
#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_SHIFT 0
#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_MASK) >> SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_01_CMD_INHIBIT_CMD_SHIFT))

//====================================================================
//Register: Present State Register 1 (Instance 1 of 2) (SD_PRESENT_STATE_10)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_PRESENT_STATE_10_RESERVED2_MASK 0xfe00
#define  SDMMC2_SD_PRESENT_STATE_10_RESERVED2_SHIFT 9
#define  SDMMC2_SD_PRESENT_STATE_10_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_10_RESERVED2_MASK) >> SDMMC2_SD_PRESENT_STATE_10_RESERVED2_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_10_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_10_RESERVED2_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_MASK 0x100
#define  SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_SHIFT 8
#define  SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_MASK) >> SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_10_CMD_LEVEL_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_MASK 0xf0
#define  SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_SHIFT 4
#define  SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_MASK) >> SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_10_DAT_LEVEL_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_MASK 0x8
#define  SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_SHIFT 3
#define  SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_MASK) >> SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_10_WRITE_PROT_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_10_CARD_DET_MASK 0x4
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_DET_SHIFT 2
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_DET_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_10_CARD_DET_MASK) >> SDMMC2_SD_PRESENT_STATE_10_CARD_DET_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_DET_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_10_CARD_DET_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_10_CARD_DET_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_MASK 0x2
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_SHIFT 1
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_MASK) >> SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_10_CARD_STABLE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_MASK 0x1
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_SHIFT 0
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_MASK) >> SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_10_CARD_INSERTED_SHIFT))

//====================================================================
//Register: Present State Register 1 (Instance 2 of 2) (SD_PRESENT_STATE_11)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_PRESENT_STATE_11_RESERVED2_MASK 0xfe00
#define  SDMMC2_SD_PRESENT_STATE_11_RESERVED2_SHIFT 9
#define  SDMMC2_SD_PRESENT_STATE_11_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_11_RESERVED2_MASK) >> SDMMC2_SD_PRESENT_STATE_11_RESERVED2_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_11_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_11_RESERVED2_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_MASK 0x100
#define  SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_SHIFT 8
#define  SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_MASK) >> SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_11_CMD_LEVEL_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_MASK 0xf0
#define  SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_SHIFT 4
#define  SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_MASK) >> SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_11_DAT_LEVEL_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_MASK 0x8
#define  SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_SHIFT 3
#define  SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_MASK) >> SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_11_WRITE_PROT_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_11_CARD_DET_MASK 0x4
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_DET_SHIFT 2
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_DET_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_11_CARD_DET_MASK) >> SDMMC2_SD_PRESENT_STATE_11_CARD_DET_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_DET_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_11_CARD_DET_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_11_CARD_DET_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_MASK 0x2
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_SHIFT 1
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_MASK) >> SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_11_CARD_STABLE_SHIFT))

#define  SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_MASK 0x1
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_SHIFT 0
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_MASK) >> SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_SHIFT)
#define  SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_MASK) | (((uint16_t)val) << SDMMC2_SD_PRESENT_STATE_11_CARD_INSERTED_SHIFT))

//====================================================================
//Register: Host Control Register (Instance 1 of 2) (SD_HOST_CTRL0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_HOST_CTRL0_RESERVED2_MASK 0xf000
#define  SDMMC2_SD_HOST_CTRL0_RESERVED2_SHIFT 12
#define  SDMMC2_SD_HOST_CTRL0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_RESERVED2_MASK) >> SDMMC2_SD_HOST_CTRL0_RESERVED2_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_RESERVED2_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_MASK 0xe00
#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_SHIFT 9
#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_MASK) >> SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_SD_BUS_VLT_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_MASK 0x100
#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_SHIFT 8
#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_MASK) >> SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_SD_BUS_POWER_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_S_MASK 0x80
#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_S_SHIFT 7
#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_S_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_CARD_DET_S_MASK) >> SDMMC2_SD_HOST_CTRL0_CARD_DET_S_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_S_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_CARD_DET_S_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_CARD_DET_S_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_L_MASK 0x40
#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_L_SHIFT 6
#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_L_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_CARD_DET_L_MASK) >> SDMMC2_SD_HOST_CTRL0_CARD_DET_L_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_CARD_DET_L_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_CARD_DET_L_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_CARD_DET_L_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_RESERVED3_MASK 0x20
#define  SDMMC2_SD_HOST_CTRL0_RESERVED3_SHIFT 5
#define  SDMMC2_SD_HOST_CTRL0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_RESERVED3_MASK) >> SDMMC2_SD_HOST_CTRL0_RESERVED3_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_RESERVED3_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_DMA_SEL_MASK 0x18
#define  SDMMC2_SD_HOST_CTRL0_DMA_SEL_SHIFT 3
#define  SDMMC2_SD_HOST_CTRL0_DMA_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_DMA_SEL_MASK) >> SDMMC2_SD_HOST_CTRL0_DMA_SEL_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_DMA_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_DMA_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_DMA_SEL_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_RESERVED4_MASK 0x4
#define  SDMMC2_SD_HOST_CTRL0_RESERVED4_SHIFT 2
#define  SDMMC2_SD_HOST_CTRL0_RESERVED4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_RESERVED4_MASK) >> SDMMC2_SD_HOST_CTRL0_RESERVED4_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_RESERVED4_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_RESERVED4_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_MASK 0x2
#define  SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_SHIFT 1
#define  SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_MASK) >> SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_DATA_WIDTH_SHIFT))

#define  SDMMC2_SD_HOST_CTRL0_LED_CTRL_MASK 0x1
#define  SDMMC2_SD_HOST_CTRL0_LED_CTRL_SHIFT 0
#define  SDMMC2_SD_HOST_CTRL0_LED_CTRL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL0_LED_CTRL_MASK) >> SDMMC2_SD_HOST_CTRL0_LED_CTRL_SHIFT)
#define  SDMMC2_SD_HOST_CTRL0_LED_CTRL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL0_LED_CTRL_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL0_LED_CTRL_SHIFT))

//====================================================================
//Register: Host Control Register (Instance 2 of 2) (SD_HOST_CTRL1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_HOST_CTRL1_RESERVED2_MASK 0xf000
#define  SDMMC2_SD_HOST_CTRL1_RESERVED2_SHIFT 12
#define  SDMMC2_SD_HOST_CTRL1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_RESERVED2_MASK) >> SDMMC2_SD_HOST_CTRL1_RESERVED2_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_RESERVED2_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_MASK 0xe00
#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_SHIFT 9
#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_MASK) >> SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_SD_BUS_VLT_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_MASK 0x100
#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_SHIFT 8
#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_MASK) >> SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_SD_BUS_POWER_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_S_MASK 0x80
#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_S_SHIFT 7
#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_S_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_CARD_DET_S_MASK) >> SDMMC2_SD_HOST_CTRL1_CARD_DET_S_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_S_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_CARD_DET_S_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_CARD_DET_S_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_L_MASK 0x40
#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_L_SHIFT 6
#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_L_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_CARD_DET_L_MASK) >> SDMMC2_SD_HOST_CTRL1_CARD_DET_L_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_CARD_DET_L_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_CARD_DET_L_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_CARD_DET_L_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_RESERVED3_MASK 0x20
#define  SDMMC2_SD_HOST_CTRL1_RESERVED3_SHIFT 5
#define  SDMMC2_SD_HOST_CTRL1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_RESERVED3_MASK) >> SDMMC2_SD_HOST_CTRL1_RESERVED3_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_RESERVED3_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_DMA_SEL_MASK 0x18
#define  SDMMC2_SD_HOST_CTRL1_DMA_SEL_SHIFT 3
#define  SDMMC2_SD_HOST_CTRL1_DMA_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_DMA_SEL_MASK) >> SDMMC2_SD_HOST_CTRL1_DMA_SEL_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_DMA_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_DMA_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_DMA_SEL_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_RESERVED4_MASK 0x4
#define  SDMMC2_SD_HOST_CTRL1_RESERVED4_SHIFT 2
#define  SDMMC2_SD_HOST_CTRL1_RESERVED4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_RESERVED4_MASK) >> SDMMC2_SD_HOST_CTRL1_RESERVED4_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_RESERVED4_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_RESERVED4_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_MASK 0x2
#define  SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_SHIFT 1
#define  SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_MASK) >> SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_DATA_WIDTH_SHIFT))

#define  SDMMC2_SD_HOST_CTRL1_LED_CTRL_MASK 0x1
#define  SDMMC2_SD_HOST_CTRL1_LED_CTRL_SHIFT 0
#define  SDMMC2_SD_HOST_CTRL1_LED_CTRL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL1_LED_CTRL_MASK) >> SDMMC2_SD_HOST_CTRL1_LED_CTRL_SHIFT)
#define  SDMMC2_SD_HOST_CTRL1_LED_CTRL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL1_LED_CTRL_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL1_LED_CTRL_SHIFT))

//====================================================================
//Register: Block Gap Control Register (Instance 1 of 2) (SD_BLOCK_GAP_CTRL0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_SHIFT 11
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED2_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_MASK 0x400
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_SHIFT 10
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_W_REMOVAL_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_MASK 0x200
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_SHIFT 9
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_W_INSERTION_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_MASK 0x100
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_SHIFT 8
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_W_CARD_INT_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_MASK 0xf0
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_SHIFT 4
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_RESERVED3_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_MASK 0x8
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_SHIFT 3
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_INT_BLK_GAP_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_MASK 0x4
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_SHIFT 2
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_RD_WAIT_CTL_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_MASK 0x2
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_SHIFT 1
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_CONT_REQ_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_MASK 0x1
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_SHIFT 0
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL0_STOP_AT_BLOCK_GAP_REQ_SHIFT))

//====================================================================
//Register: Block Gap Control Register (Instance 2 of 2) (SD_BLOCK_GAP_CTRL1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_SHIFT 11
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED2_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_MASK 0x400
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_SHIFT 10
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_W_REMOVAL_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_MASK 0x200
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_SHIFT 9
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_W_INSERTION_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_MASK 0x100
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_SHIFT 8
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_W_CARD_INT_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_MASK 0xf0
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_SHIFT 4
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_RESERVED3_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_MASK 0x8
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_SHIFT 3
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_INT_BLK_GAP_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_MASK 0x4
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_SHIFT 2
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_RD_WAIT_CTL_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_MASK 0x2
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_SHIFT 1
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_CONT_REQ_SHIFT))

#define  SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_MASK 0x1
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_SHIFT 0
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_MASK) >> SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_SHIFT)
#define  SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_MASK) | (((uint16_t)val) << SDMMC2_SD_BLOCK_GAP_CTRL1_STOP_AT_BLOCK_GAP_REQ_SHIFT))

//====================================================================
//Register: Clock Control Register (Instance 1 of 2) (SD_CLOCK_CTRL0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_MASK 0xff00
#define  SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_SHIFT 8
#define  SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_MASK) >> SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL0_SD_FREQ_SEL_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL0_RESERVED2_MASK 0xf8
#define  SDMMC2_SD_CLOCK_CTRL0_RESERVED2_SHIFT 3
#define  SDMMC2_SD_CLOCK_CTRL0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL0_RESERVED2_MASK) >> SDMMC2_SD_CLOCK_CTRL0_RESERVED2_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL0_RESERVED2_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_MASK 0x4
#define  SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_SHIFT 2
#define  SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_MASK) >> SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL0_SD_CLK_EN_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_MASK 0x2
#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_SHIFT 1
#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_MASK) >> SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL0_INT_CLK_STABLE_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_MASK 0x1
#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_SHIFT 0
#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_MASK) >> SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL0_INT_CLK_EN_SHIFT))

//====================================================================
//Register: Clock Control Register (Instance 2 of 2) (SD_CLOCK_CTRL1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_MASK 0xff00
#define  SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_SHIFT 8
#define  SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_MASK) >> SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL1_SD_FREQ_SEL_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL1_RESERVED2_MASK 0xf8
#define  SDMMC2_SD_CLOCK_CTRL1_RESERVED2_SHIFT 3
#define  SDMMC2_SD_CLOCK_CTRL1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL1_RESERVED2_MASK) >> SDMMC2_SD_CLOCK_CTRL1_RESERVED2_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL1_RESERVED2_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_MASK 0x4
#define  SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_SHIFT 2
#define  SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_MASK) >> SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL1_SD_CLK_EN_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_MASK 0x2
#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_SHIFT 1
#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_MASK) >> SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL1_INT_CLK_STABLE_SHIFT))

#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_MASK 0x1
#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_SHIFT 0
#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_MASK) >> SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_SHIFT)
#define  SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_CTRL1_INT_CLK_EN_SHIFT))

//====================================================================
//Register: Timeout Control/Software Reset Register (Instance 1 of 2) (SD_TIMEOUT_CTRL_SW_RESET0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_SHIFT 11
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED2_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_MASK 0x400
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_SHIFT 10
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_DAT_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_MASK 0x200
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_SHIFT 9
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_CMD_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_MASK 0x100
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_SHIFT 8
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_SW_RST_ALL_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_MASK 0xf0
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_SHIFT 4
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_RESERVED3_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_MASK 0xf
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_SHIFT 0
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET0_TIMEOUT_VALUE_SHIFT))

//====================================================================
//Register: Timeout Control/Software Reset Register (Instance 2 of 2) (SD_TIMEOUT_CTRL_SW_RESET1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_SHIFT 11
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED2_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_MASK 0x400
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_SHIFT 10
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_DAT_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_MASK 0x200
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_SHIFT 9
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_CMD_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_MASK 0x100
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_SHIFT 8
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_SW_RST_ALL_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_MASK 0xf0
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_SHIFT 4
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_RESERVED3_SHIFT))

#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_MASK 0xf
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_SHIFT 0
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_MASK) >> SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_SHIFT)
#define  SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_MASK) | (((uint16_t)val) << SDMMC2_SD_TIMEOUT_CTRL_SW_RESET1_TIMEOUT_VALUE_SHIFT))

//====================================================================
//Register: Normal Interrupt Status Register (Instance 1 of 2) (SD_NORMAL_INT_STATUS0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_MASK 0x8000
#define  SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_SHIFT 15
#define  SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_ERR_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_MASK 0x7e00
#define  SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_SHIFT 9
#define  SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_RESERVED2_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_MASK 0x100
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_SHIFT 8
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_MASK 0x80
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_SHIFT 7
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_CARD_REM_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_MASK 0x40
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_SHIFT 6
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_CARD_INS_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_MASK 0x20
#define  SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_SHIFT 5
#define  SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_RX_RDY_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_MASK 0x10
#define  SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_SHIFT 4
#define  SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_TX_RDY_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_MASK 0x8
#define  SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_SHIFT 3
#define  SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_DMA_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_MASK 0x4
#define  SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_SHIFT 2
#define  SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_BLOCK_GAP_EVT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_MASK 0x2
#define  SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_SHIFT 1
#define  SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_XFER_COMPLETE_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_MASK 0x1
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_SHIFT 0
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS0_CMD_COMPLETE_SHIFT))

//====================================================================
//Register: Normal Interrupt Status Register (Instance 2 of 2) (SD_NORMAL_INT_STATUS1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_MASK 0x8000
#define  SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_SHIFT 15
#define  SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_ERR_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_MASK 0x7e00
#define  SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_SHIFT 9
#define  SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_RESERVED2_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_MASK 0x100
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_SHIFT 8
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_MASK 0x80
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_SHIFT 7
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_CARD_REM_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_MASK 0x40
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_SHIFT 6
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_CARD_INS_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_MASK 0x20
#define  SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_SHIFT 5
#define  SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_RX_RDY_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_MASK 0x10
#define  SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_SHIFT 4
#define  SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_TX_RDY_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_MASK 0x8
#define  SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_SHIFT 3
#define  SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_DMA_INT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_MASK 0x4
#define  SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_SHIFT 2
#define  SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_BLOCK_GAP_EVT_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_MASK 0x2
#define  SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_SHIFT 1
#define  SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_XFER_COMPLETE_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_MASK 0x1
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_SHIFT 0
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS1_CMD_COMPLETE_SHIFT))

//====================================================================
//Register: Error Interrupt Status Register (Instance 1 of 2) (SD_ERROR_INT_STATUS0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_MASK 0x8000
#define  SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_SHIFT 15
#define  SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_CRC_STATUS_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_MASK 0x4000
#define  SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_SHIFT 14
#define  SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_CPL_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_MASK 0x2000
#define  SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_SHIFT 13
#define  SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_AXI_RESP_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_MASK 0x1000
#define  SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_SHIFT 12
#define  SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_SPI_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_SHIFT 10
#define  SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_RESERVED2_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_MASK 0x200
#define  SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_SHIFT 9
#define  SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_ADMA_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_MASK 0x100
#define  SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_SHIFT 8
#define  SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_AUTO_CMD12_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_MASK 0x80
#define  SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_SHIFT 7
#define  SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_CUR_LIMIT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_MASK 0x40
#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_SHIFT 6
#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_MASK 0x20
#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_SHIFT 5
#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_RD_DATA_CRC_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_MASK 0x10
#define  SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_SHIFT 4
#define  SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_DATA_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_MASK 0x8
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_SHIFT 3
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_CMD_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_MASK 0x4
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_SHIFT 2
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_CMD_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_MASK 0x2
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_SHIFT 1
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_CMD_CRC_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_MASK 0x1
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_SHIFT 0
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS0_CMD_TIMEOUT_ERR_SHIFT))

//====================================================================
//Register: Error Interrupt Status Register (Instance 2 of 2) (SD_ERROR_INT_STATUS1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_MASK 0x8000
#define  SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_SHIFT 15
#define  SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_CRC_STATUS_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_MASK 0x4000
#define  SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_SHIFT 14
#define  SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_CPL_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_MASK 0x2000
#define  SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_SHIFT 13
#define  SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_AXI_RESP_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_MASK 0x1000
#define  SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_SHIFT 12
#define  SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_SPI_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_SHIFT 10
#define  SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_RESERVED2_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_MASK 0x200
#define  SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_SHIFT 9
#define  SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_ADMA_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_MASK 0x100
#define  SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_SHIFT 8
#define  SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_AUTO_CMD12_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_MASK 0x80
#define  SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_SHIFT 7
#define  SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_CUR_LIMIT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_MASK 0x40
#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_SHIFT 6
#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_MASK 0x20
#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_SHIFT 5
#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_RD_DATA_CRC_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_MASK 0x10
#define  SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_SHIFT 4
#define  SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_DATA_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_MASK 0x8
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_SHIFT 3
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_CMD_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_MASK 0x4
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_SHIFT 2
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_CMD_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_MASK 0x2
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_SHIFT 1
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_CMD_CRC_ERR_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_MASK 0x1
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_SHIFT 0
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_MASK) >> SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS1_CMD_TIMEOUT_ERR_SHIFT))

//====================================================================
//Register: Normal Interrupt Status Enable Register (Instance 1 of 2) (SD_NORMAL_INT_STATUS_EN0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_MASK 0xfe00
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_SHIFT 9
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_RESERVED2_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_MASK 0x100
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_SHIFT 8
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_MASK 0x80
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_SHIFT 7
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_REM_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_MASK 0x40
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_SHIFT 6
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_CARD_INS_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_MASK 0x20
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_SHIFT 5
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_RD_RDY_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_MASK 0x10
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_SHIFT 4
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_TX_RDY_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_MASK 0x8
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_SHIFT 3
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_DMA_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_MASK 0x4
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_SHIFT 2
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_BLOCK_GAP_EVT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_MASK 0x2
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_SHIFT 1
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_XFER_COMPLETE_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_MASK 0x1
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_SHIFT 0
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN0_CMD_COMPLETE_EN_SHIFT))

//====================================================================
//Register: Normal Interrupt Status Enable Register (Instance 2 of 2) (SD_NORMAL_INT_STATUS_EN1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_MASK 0xfe00
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_SHIFT 9
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_RESERVED2_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_MASK 0x100
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_SHIFT 8
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_MASK 0x80
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_SHIFT 7
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_REM_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_MASK 0x40
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_SHIFT 6
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_CARD_INS_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_MASK 0x20
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_SHIFT 5
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_RD_RDY_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_MASK 0x10
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_SHIFT 4
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_TX_RDY_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_MASK 0x8
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_SHIFT 3
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_DMA_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_MASK 0x4
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_SHIFT 2
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_BLOCK_GAP_EVT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_MASK 0x2
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_SHIFT 1
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_XFER_COMPLETE_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_MASK 0x1
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_SHIFT 0
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_EN1_CMD_COMPLETE_EN_SHIFT))

//====================================================================
//Register: Error Interrupt Status Enable Register (Instance 1 of 2) (SD_ERROR_INT_STATUS_EN0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_MASK 0x8000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_SHIFT 15
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_CRC_STATUS_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_MASK 0x4000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_SHIFT 14
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_CPL_TIMEOUT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_MASK 0x2000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_SHIFT 13
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_AXI_RESP_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_MASK 0x1000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_SHIFT 12
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_SPI_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_SHIFT 10
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_RESERVED2_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_MASK 0x200
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_SHIFT 9
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_ADMA_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_MASK 0x100
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_SHIFT 8
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_AUTO_CMD12_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_MASK 0x80
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_SHIFT 7
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_CUR_LIM_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_MASK 0x40
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_SHIFT 6
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_END_BIT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_MASK 0x20
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_SHIFT 5
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_RD_DATA_CRC_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_MASK 0x10
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_SHIFT 4
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_DATA_TIMEOUT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_MASK 0x8
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_SHIFT 3
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_INDEX_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_MASK 0x4
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_SHIFT 2
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_END_BIT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_MASK 0x2
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_SHIFT 1
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_CRC_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_MASK 0x1
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_SHIFT 0
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN0_CMD_TIMEOUT_ERR_EN_SHIFT))

//====================================================================
//Register: Error Interrupt Status Enable Register (Instance 2 of 2) (SD_ERROR_INT_STATUS_EN1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_MASK 0x8000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_SHIFT 15
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_CRC_STATUS_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_MASK 0x4000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_SHIFT 14
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_CPL_TIMEOUT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_MASK 0x2000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_SHIFT 13
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_AXI_RESP_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_MASK 0x1000
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_SHIFT 12
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_SPI_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_SHIFT 10
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_RESERVED2_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_MASK 0x200
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_SHIFT 9
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_ADMA_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_MASK 0x100
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_SHIFT 8
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_AUTO_CMD12_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_MASK 0x80
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_SHIFT 7
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_CUR_LIM_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_MASK 0x40
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_SHIFT 6
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_END_BIT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_MASK 0x20
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_SHIFT 5
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_RD_DATA_CRC_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_MASK 0x10
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_SHIFT 4
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_DATA_TIMEOUT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_MASK 0x8
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_SHIFT 3
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_INDEX_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_MASK 0x4
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_SHIFT 2
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_END_BIT_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_MASK 0x2
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_SHIFT 1
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_CRC_ERR_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_MASK 0x1
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_SHIFT 0
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_EN1_CMD_TIMEOUT_ERR_EN_SHIFT))

//====================================================================
//Register: Normal Interrupt Status Interrupt Enable Register (Instance 1 of 2) (SD_NORMAL_INT_STATUS_INT_EN0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_MASK 0xfe00
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_SHIFT 9
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RESERVED2_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_MASK 0x100
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_SHIFT 8
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INT_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_MASK 0x80
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_SHIFT 7
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_REM_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_MASK 0x40
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_SHIFT 6
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CARD_INS_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_MASK 0x20
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_SHIFT 5
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_RX_RDY_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_MASK 0x10
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_SHIFT 4
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_TX_RDY_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_MASK 0x8
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_SHIFT 3
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_DMA_INT_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_MASK 0x4
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_SHIFT 2
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_BLOCK_GAP_EVT_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_MASK 0x2
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_SHIFT 1
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_XFER_COMPLETE_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_MASK 0x1
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_SHIFT 0
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN0_CMD_COMPLETE_INT_EN_SHIFT))

//====================================================================
//Register: Normal Interrupt Status Interrupt Enable Register (Instance 2 of 2) (SD_NORMAL_INT_STATUS_INT_EN1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_MASK 0xfe00
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_SHIFT 9
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RESERVED2_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_MASK 0x100
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_SHIFT 8
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INT_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_MASK 0x80
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_SHIFT 7
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_REM_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_MASK 0x40
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_SHIFT 6
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CARD_INS_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_MASK 0x20
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_SHIFT 5
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_RX_RDY_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_MASK 0x10
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_SHIFT 4
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_TX_RDY_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_MASK 0x8
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_SHIFT 3
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_DMA_INT_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_MASK 0x4
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_SHIFT 2
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_BLOCK_GAP_EVT_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_MASK 0x2
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_SHIFT 1
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_XFER_COMPLETE_INT_EN_SHIFT))

#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_MASK 0x1
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_SHIFT 0
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_MASK) >> SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_SHIFT)
#define  SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_NORMAL_INT_STATUS_INT_EN1_CMD_COMPLETE_INT_EN_SHIFT))

//====================================================================
//Register: Error Interrupt Status Interrupt Enable Register (Instance 1 of 2) (SD_ERROR_INT_STATUS_INT_EN0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_MASK 0x8000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_SHIFT 15
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CRC_STATUS_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_MASK 0x4000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_SHIFT 14
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CPL_TIMEOUT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_MASK 0x2000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_SHIFT 13
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AXI_RESP_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_MASK 0x1000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_SHIFT 12
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_SPI_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_SHIFT 10
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RESERVED2_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_MASK 0x200
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_SHIFT 9
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_ADMA_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_MASK 0x100
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_SHIFT 8
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_AUTO_CMD12_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_MASK 0x80
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_SHIFT 7
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CUR_LIM_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_MASK 0x40
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_SHIFT 6
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_END_BIT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_MASK 0x20
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_SHIFT 5
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_RD_DATA_CRC_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_MASK 0x10
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_SHIFT 4
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_DATA_TIMEOUT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_MASK 0x8
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_SHIFT 3
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_INDEX_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_MASK 0x4
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_SHIFT 2
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_END_BIT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_MASK 0x2
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_SHIFT 1
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_CRC_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_MASK 0x1
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_SHIFT 0
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN0_CMD_TIMEOUT_ERR_INT_EN_SHIFT))

//====================================================================
//Register: Error Interrupt Status Interrupt Enable Register (Instance 2 of 2) (SD_ERROR_INT_STATUS_INT_EN1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_MASK 0x8000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_SHIFT 15
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CRC_STATUS_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_MASK 0x4000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_SHIFT 14
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CPL_TIMEOUT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_MASK 0x2000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_SHIFT 13
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AXI_RESP_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_MASK 0x1000
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_SHIFT 12
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_SPI_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_SHIFT 10
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RESERVED2_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_MASK 0x200
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_SHIFT 9
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_ADMA_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_MASK 0x100
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_SHIFT 8
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_AUTO_CMD12_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_MASK 0x80
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_SHIFT 7
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CUR_LIM_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_MASK 0x40
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_SHIFT 6
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_END_BIT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_MASK 0x20
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_SHIFT 5
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_RD_DATA_CRC_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_MASK 0x10
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_SHIFT 4
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_DATA_TIMEOUT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_MASK 0x8
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_SHIFT 3
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_INDEX_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_MASK 0x4
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_SHIFT 2
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_END_BIT_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_MASK 0x2
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_SHIFT 1
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_CRC_ERR_INT_EN_SHIFT))

#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_MASK 0x1
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_SHIFT 0
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_MASK) >> SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_SHIFT)
#define  SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_ERROR_INT_STATUS_INT_EN1_CMD_TIMEOUT_ERR_INT_EN_SHIFT))

//====================================================================
//Register: Auto CMD12 Error Status Register (Instance 1 of 2) (SD_AUTO_CMD12_ERROR_STATUS0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_MASK 0xff00
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_SHIFT 8
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED2_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_MASK 0x80
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_SHIFT 7
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_CMD_NOT_ISSUED_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_MASK 0x60
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_SHIFT 5
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_RESERVED3_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_MASK 0x10
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_SHIFT 4
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_MASK 0x8
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_SHIFT 3
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_MASK 0x4
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_SHIFT 2
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_CRC_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_MASK 0x2
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_SHIFT 1
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_MASK 0x1
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_SHIFT 0
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS0_AUTO_CMD12_NOT_EXE_SHIFT))

//====================================================================
//Register: Auto CMD12 Error Status Register (Instance 2 of 2) (SD_AUTO_CMD12_ERROR_STATUS1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_MASK 0xff00
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_SHIFT 8
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED2_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_MASK 0x80
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_SHIFT 7
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_CMD_NOT_ISSUED_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_MASK 0x60
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_SHIFT 5
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_RESERVED3_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_MASK 0x10
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_SHIFT 4
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_MASK 0x8
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_SHIFT 3
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_MASK 0x4
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_SHIFT 2
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_CRC_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_MASK 0x2
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_SHIFT 1
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_MASK 0x1
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_SHIFT 0
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_MASK) >> SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_SHIFT)
#define  SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_MASK) | (((uint16_t)val) << SDMMC2_SD_AUTO_CMD12_ERROR_STATUS1_AUTO_CMD12_NOT_EXE_SHIFT))

//====================================================================
//Register: Capabilities Register 0 (Instance 1 of 2) (SD_CAPABILITIES_00)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_00_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CAPABILITIES_00_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CAPABILITIES_00_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_00_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_00_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_00_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_00_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_00_RESERVED2_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_MASK 0x3f00
#define  SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_SHIFT 8
#define  SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_MASK) >> SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_00_BASE_FREQ_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_MASK 0x80
#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_SHIFT 7
#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_MASK) >> SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_00_TIMEOUT_UNIT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_00_RESERVED3_MASK 0x40
#define  SDMMC2_SD_CAPABILITIES_00_RESERVED3_SHIFT 6
#define  SDMMC2_SD_CAPABILITIES_00_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_00_RESERVED3_MASK) >> SDMMC2_SD_CAPABILITIES_00_RESERVED3_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_00_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_00_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_00_RESERVED3_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_MASK 0x3f
#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_MASK) >> SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_00_TIMEOUT_FREQ_SHIFT))

//====================================================================
//Register: Capabilities Register 0 (Instance 2 of 2) (SD_CAPABILITIES_01)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_01_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CAPABILITIES_01_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CAPABILITIES_01_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_01_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_01_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_01_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_01_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_01_RESERVED2_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_MASK 0x3f00
#define  SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_SHIFT 8
#define  SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_MASK) >> SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_01_BASE_FREQ_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_MASK 0x80
#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_SHIFT 7
#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_MASK) >> SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_01_TIMEOUT_UNIT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_01_RESERVED3_MASK 0x40
#define  SDMMC2_SD_CAPABILITIES_01_RESERVED3_SHIFT 6
#define  SDMMC2_SD_CAPABILITIES_01_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_01_RESERVED3_MASK) >> SDMMC2_SD_CAPABILITIES_01_RESERVED3_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_01_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_01_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_01_RESERVED3_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_MASK 0x3f
#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_MASK) >> SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_01_TIMEOUT_FREQ_SHIFT))

//====================================================================
//Register: Capabilities Register 1 (Instance 1 of 2) (SD_CAPABILITIES_10)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_10_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED2_SHIFT 11
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_10_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_RESERVED2_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_MASK 0x400
#define  SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_SHIFT 10
#define  SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_VLT_18_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_MASK 0x200
#define  SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_SHIFT 9
#define  SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_VLT_30_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_MASK 0x100
#define  SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_SHIFT 8
#define  SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_VLT_33_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_MASK 0x80
#define  SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_SHIFT 7
#define  SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_SUSPEND_RESUME_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_MASK 0x40
#define  SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_SHIFT 6
#define  SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_MASK) >> SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_DMA_SUPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_MASK 0x20
#define  SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_SHIFT 5
#define  SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_HI_SPEED_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_RESERVED3_MASK 0x10
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED3_SHIFT 4
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_RESERVED3_MASK) >> SDMMC2_SD_CAPABILITIES_10_RESERVED3_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_RESERVED3_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_MASK 0x8
#define  SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_SHIFT 3
#define  SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_ADMA2_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_RESERVED4_MASK 0x4
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED4_SHIFT 2
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_RESERVED4_MASK) >> SDMMC2_SD_CAPABILITIES_10_RESERVED4_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_RESERVED4_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_RESERVED4_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_MASK 0x3
#define  SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_MASK) >> SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_10_MAX_BLK_LEN_SHIFT))

//====================================================================
//Register: Capabilities Register 1 (Instance 2 of 2) (SD_CAPABILITIES_11)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_11_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED2_SHIFT 11
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_11_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_RESERVED2_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_MASK 0x400
#define  SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_SHIFT 10
#define  SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_VLT_18_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_MASK 0x200
#define  SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_SHIFT 9
#define  SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_VLT_30_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_MASK 0x100
#define  SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_SHIFT 8
#define  SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_VLT_33_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_MASK 0x80
#define  SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_SHIFT 7
#define  SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_SUSPEND_RESUME_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_MASK 0x40
#define  SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_SHIFT 6
#define  SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_MASK) >> SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_DMA_SUPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_MASK 0x20
#define  SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_SHIFT 5
#define  SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_HI_SPEED_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_RESERVED3_MASK 0x10
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED3_SHIFT 4
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_RESERVED3_MASK) >> SDMMC2_SD_CAPABILITIES_11_RESERVED3_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_RESERVED3_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_MASK 0x8
#define  SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_SHIFT 3
#define  SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_MASK) >> SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_ADMA2_SUPPORT_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_RESERVED4_MASK 0x4
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED4_SHIFT 2
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_RESERVED4_MASK) >> SDMMC2_SD_CAPABILITIES_11_RESERVED4_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_RESERVED4_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_RESERVED4_SHIFT))

#define  SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_MASK 0x3
#define  SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_MASK) >> SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_11_MAX_BLK_LEN_SHIFT))

//====================================================================
//Register: Capabilities Register 2 (Instance 1 of 2) (SD_CAPABILITIES_20)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_20_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_CAPABILITIES_20_RESERVED2_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_20_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_20_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_20_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_20_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_20_RESERVED2_SHIFT))

//====================================================================
//Register: Capabilities Register 2 (Instance 2 of 2) (SD_CAPABILITIES_21)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_21_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_CAPABILITIES_21_RESERVED2_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_21_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_21_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_21_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_21_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_21_RESERVED2_SHIFT))

//====================================================================
//Register: Capabilities Register 3 (Instance 1 of 2) (SD_CAPABILITIES_30)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_30_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_CAPABILITIES_30_RESERVED2_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_30_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_30_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_30_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_30_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_30_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_30_RESERVED2_SHIFT))

//====================================================================
//Register: Capabilities Register 3 (Instance 2 of 2) (SD_CAPABILITIES_31)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CAPABILITIES_31_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_CAPABILITIES_31_RESERVED2_SHIFT 0
#define  SDMMC2_SD_CAPABILITIES_31_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CAPABILITIES_31_RESERVED2_MASK) >> SDMMC2_SD_CAPABILITIES_31_RESERVED2_SHIFT)
#define  SDMMC2_SD_CAPABILITIES_31_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CAPABILITIES_31_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CAPABILITIES_31_RESERVED2_SHIFT))

//====================================================================
//Register: Maximum Current Register 0 (Instance 1 of 2) (SD_MAX_CURRENT_00)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_MASK 0xff00
#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_SHIFT 8
#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_MASK) >> SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_30_SHIFT))

#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_MASK 0xff
#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_MASK) >> SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_00_MAX_CUR_33_SHIFT))

//====================================================================
//Register: Maximum Current Register 0 (Instance 2 of 2) (SD_MAX_CURRENT_01)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_MASK 0xff00
#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_SHIFT 8
#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_MASK) >> SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_30_SHIFT))

#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_MASK 0xff
#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_MASK) >> SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_01_MAX_CUR_33_SHIFT))

//====================================================================
//Register: Maximum Current Register 1 (Instance 1 of 2) (SD_MAX_CURRENT_10)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_10_RESERVED2_MASK 0xff00
#define  SDMMC2_SD_MAX_CURRENT_10_RESERVED2_SHIFT 8
#define  SDMMC2_SD_MAX_CURRENT_10_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_10_RESERVED2_MASK) >> SDMMC2_SD_MAX_CURRENT_10_RESERVED2_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_10_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_10_RESERVED2_SHIFT))

#define  SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_MASK 0xff
#define  SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_MASK) >> SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_10_MAX_CUR_18_SHIFT))

//====================================================================
//Register: Maximum Current Register 1 (Instance 2 of 2) (SD_MAX_CURRENT_11)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_11_RESERVED2_MASK 0xff00
#define  SDMMC2_SD_MAX_CURRENT_11_RESERVED2_SHIFT 8
#define  SDMMC2_SD_MAX_CURRENT_11_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_11_RESERVED2_MASK) >> SDMMC2_SD_MAX_CURRENT_11_RESERVED2_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_11_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_11_RESERVED2_SHIFT))

#define  SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_MASK 0xff
#define  SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_MASK) >> SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_11_MAX_CUR_18_SHIFT))

//====================================================================
//Register: Maximum Current Register 2 (Instance 1 of 2) (SD_MAX_CURRENT_20)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_20_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_MAX_CURRENT_20_RESERVED2_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_20_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_20_RESERVED2_MASK) >> SDMMC2_SD_MAX_CURRENT_20_RESERVED2_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_20_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_20_RESERVED2_SHIFT))

//====================================================================
//Register: Maximum Current Register 2 (Instance 2 of 2) (SD_MAX_CURRENT_21)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_21_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_MAX_CURRENT_21_RESERVED2_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_21_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_21_RESERVED2_MASK) >> SDMMC2_SD_MAX_CURRENT_21_RESERVED2_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_21_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_21_RESERVED2_SHIFT))

//====================================================================
//Register: Maximum Current Register 3 (Instance 1 of 2) (SD_MAX_CURRENT_30)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_30_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_MAX_CURRENT_30_RESERVED2_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_30_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_30_RESERVED2_MASK) >> SDMMC2_SD_MAX_CURRENT_30_RESERVED2_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_30_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_30_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_30_RESERVED2_SHIFT))

//====================================================================
//Register: Maximum Current Register 3 (Instance 2 of 2) (SD_MAX_CURRENT_31)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_MAX_CURRENT_31_RESERVED2_MASK 0xffff
#define  SDMMC2_SD_MAX_CURRENT_31_RESERVED2_SHIFT 0
#define  SDMMC2_SD_MAX_CURRENT_31_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_MAX_CURRENT_31_RESERVED2_MASK) >> SDMMC2_SD_MAX_CURRENT_31_RESERVED2_SHIFT)
#define  SDMMC2_SD_MAX_CURRENT_31_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_MAX_CURRENT_31_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_MAX_CURRENT_31_RESERVED2_SHIFT))

//====================================================================
//Register: Force Event Auto cmd12 Error Register (Instance 1 of 2) (SD_FORCE_EVENT_AUTO_CMD12_ERROR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_MASK 0xff00
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_SHIFT 8
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED2_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_MASK 0x80
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_SHIFT 7
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_ISSUE_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_MASK 0x60
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_SHIFT 5
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_RESERVED3_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_MASK 0x10
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_SHIFT 4
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_MASK 0x8
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_SHIFT 3
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_EBIT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_MASK 0x4
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_SHIFT 2
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_CRC_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_MASK 0x2
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_SHIFT 1
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_TO_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_MASK 0x1
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_SHIFT 0
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR0_F_ACMD12_NEXE_ERR_SHIFT))

//====================================================================
//Register: Force Event Auto cmd12 Error Register (Instance 2 of 2) (SD_FORCE_EVENT_AUTO_CMD12_ERROR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_MASK 0xff00
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_SHIFT 8
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED2_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_MASK 0x80
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_SHIFT 7
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_ISSUE_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_MASK 0x60
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_SHIFT 5
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_RESERVED3_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_MASK 0x10
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_SHIFT 4
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_MASK 0x8
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_SHIFT 3
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_EBIT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_MASK 0x4
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_SHIFT 2
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_CRC_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_MASK 0x2
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_SHIFT 1
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_TO_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_MASK 0x1
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_SHIFT 0
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_AUTO_CMD12_ERROR1_F_ACMD12_NEXE_ERR_SHIFT))

//====================================================================
//Register: Force Event for Error Status Register (Instance 1 of 2) (SD_FORCE_EVENT_FOR_ERROR_STATUS0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_MASK 0x8000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_SHIFT 15
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CRC_STATUS_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_MASK 0x4000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_SHIFT 14
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CPL_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_MASK 0x2000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_SHIFT 13
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_AXI_RESP_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_MASK 0x1000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_SHIFT 12
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_SPI_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_SHIFT 10
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_RESERVED2_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_MASK 0x200
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_SHIFT 9
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ADMA_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_MASK 0x100
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_SHIFT 8
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_ACMD12_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_MASK 0x80
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_SHIFT 7
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CURRENT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_MASK 0x40
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_SHIFT 6
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_MASK 0x20
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_SHIFT 5
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_CRC_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_MASK 0x10
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_SHIFT 4
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_DAT_TO_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_MASK 0x8
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_SHIFT 3
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_MASK 0x4
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_SHIFT 2
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_MASK 0x2
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_SHIFT 1
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_CRC_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_MASK 0x1
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_SHIFT 0
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS0_F_CMD_TO_ERR_SHIFT))

//====================================================================
//Register: Force Event for Error Status Register (Instance 2 of 2) (SD_FORCE_EVENT_FOR_ERROR_STATUS1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_MASK 0x8000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_SHIFT 15
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CRC_STATUS_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_MASK 0x4000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_SHIFT 14
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CPL_TIMEOUT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_MASK 0x2000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_SHIFT 13
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_AXI_RESP_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_MASK 0x1000
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_SHIFT 12
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_SPI_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_MASK 0xc00
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_SHIFT 10
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_RESERVED2_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_MASK 0x200
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_SHIFT 9
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ADMA_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_MASK 0x100
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_SHIFT 8
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_ACMD12_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_MASK 0x80
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_SHIFT 7
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CURRENT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_MASK 0x40
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_SHIFT 6
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_MASK 0x20
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_SHIFT 5
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_CRC_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_MASK 0x10
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_SHIFT 4
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_DAT_TO_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_MASK 0x8
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_SHIFT 3
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_INDEX_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_MASK 0x4
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_SHIFT 2
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_END_BIT_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_MASK 0x2
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_SHIFT 1
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_CRC_ERR_SHIFT))

#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_MASK 0x1
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_SHIFT 0
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_MASK) >> SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_SHIFT)
#define  SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_FORCE_EVENT_FOR_ERROR_STATUS1_F_CMD_TO_ERR_SHIFT))

//====================================================================
//Register: ADMA Error Status Register (Instance 1 of 2) (SD_ADMA_ERROR_STATUS0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_MASK 0xfff8
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_SHIFT 3
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_MASK) >> SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_SHIFT)
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_ERROR_STATUS0_RESERVED2_SHIFT))

#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_MASK 0x4
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_SHIFT 2
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_MASK) >> SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_SHIFT)
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_LEN_ERR_SHIFT))

#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_MASK 0x3
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_SHIFT 0
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_MASK) >> SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_SHIFT)
#define  SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_ERROR_STATUS0_ADMA_STATE_SHIFT))

//====================================================================
//Register: ADMA Error Status Register (Instance 2 of 2) (SD_ADMA_ERROR_STATUS1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_MASK 0xfff8
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_SHIFT 3
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_MASK) >> SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_SHIFT)
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_ERROR_STATUS1_RESERVED2_SHIFT))

#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_MASK 0x4
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_SHIFT 2
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_MASK) >> SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_SHIFT)
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_LEN_ERR_SHIFT))

#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_MASK 0x3
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_SHIFT 0
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_MASK) >> SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_SHIFT)
#define  SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_ERROR_STATUS1_ADMA_STATE_SHIFT))

//====================================================================
//Register: ADMA System Address Register 0 (Instance 1 of 2) (SD_ADMA_SYS_ADDR_00)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_00_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: ADMA System Address Register 0 (Instance 2 of 2) (SD_ADMA_SYS_ADDR_01)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_01_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: ADMA System Address Register 1 (Instance 1 of 2) (SD_ADMA_SYS_ADDR_10)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_10_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: ADMA System Address Register 1 (Instance 2 of 2) (SD_ADMA_SYS_ADDR_11)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_11_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: ADMA System Address Register 2 (Instance 1 of 2) (SD_ADMA_SYS_ADDR_20)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_20_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: ADMA System Address Register 2 (Instance 2 of 2) (SD_ADMA_SYS_ADDR_21)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_21_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: ADMA System Address Register 3 (Instance 1 of 2) (SD_ADMA_SYS_ADDR_30)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_30_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: ADMA System Address Register 3 (Instance 2 of 2) (SD_ADMA_SYS_ADDR_31)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_MASK 0xffff
#define  SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_SHIFT 0
#define  SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_MASK) >> SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_SHIFT)
#define  SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_MASK) | (((uint16_t)val) << SDMMC2_SD_ADMA_SYS_ADDR_31_ADMA_SYS_ADDR_SHIFT))

//====================================================================
//Register: FIFO Parameters Register (Instance 1 of 2) (SD_FIFO_PARAM0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_FIFO_PARAM0_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_FIFO_PARAM0_RESERVED2_SHIFT 11
#define  SDMMC2_SD_FIFO_PARAM0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM0_RESERVED2_MASK) >> SDMMC2_SD_FIFO_PARAM0_RESERVED2_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM0_RESERVED2_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_MASK 0x400
#define  SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_SHIFT 10
#define  SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_MASK) >> SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM0_DIS_PAD_SD_CLK_GATE_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_MASK 0x200
#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_SHIFT 9
#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_MASK) >> SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM0_CLK_GATE_ON_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_MASK 0x100
#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_SHIFT 8
#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_MASK) >> SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM0_CLK_GATE_CTL_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM0_RESERVED3_MASK 0xff
#define  SDMMC2_SD_FIFO_PARAM0_RESERVED3_SHIFT 0
#define  SDMMC2_SD_FIFO_PARAM0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM0_RESERVED3_MASK) >> SDMMC2_SD_FIFO_PARAM0_RESERVED3_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM0_RESERVED3_SHIFT))

//====================================================================
//Register: FIFO Parameters Register (Instance 2 of 2) (SD_FIFO_PARAM1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_FIFO_PARAM1_RESERVED2_MASK 0xf800
#define  SDMMC2_SD_FIFO_PARAM1_RESERVED2_SHIFT 11
#define  SDMMC2_SD_FIFO_PARAM1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM1_RESERVED2_MASK) >> SDMMC2_SD_FIFO_PARAM1_RESERVED2_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM1_RESERVED2_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_MASK 0x400
#define  SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_SHIFT 10
#define  SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_MASK) >> SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM1_DIS_PAD_SD_CLK_GATE_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_MASK 0x200
#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_SHIFT 9
#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_MASK) >> SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM1_CLK_GATE_ON_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_MASK 0x100
#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_SHIFT 8
#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_MASK) >> SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM1_CLK_GATE_CTL_SHIFT))

#define  SDMMC2_SD_FIFO_PARAM1_RESERVED3_MASK 0xff
#define  SDMMC2_SD_FIFO_PARAM1_RESERVED3_SHIFT 0
#define  SDMMC2_SD_FIFO_PARAM1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_FIFO_PARAM1_RESERVED3_MASK) >> SDMMC2_SD_FIFO_PARAM1_RESERVED3_SHIFT)
#define  SDMMC2_SD_FIFO_PARAM1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_FIFO_PARAM1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_FIFO_PARAM1_RESERVED3_SHIFT))

//====================================================================
//Register: SPI Mode Register (Instance 1 of 2) (SD_SPI_MODE0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_SPI_MODE0_RESERVED2_MASK 0xe000
#define  SDMMC2_SD_SPI_MODE0_RESERVED2_SHIFT 13
#define  SDMMC2_SD_SPI_MODE0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE0_RESERVED2_MASK) >> SDMMC2_SD_SPI_MODE0_RESERVED2_SHIFT)
#define  SDMMC2_SD_SPI_MODE0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE0_RESERVED2_SHIFT))

#define  SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_MASK 0x1f00
#define  SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_SHIFT 8
#define  SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_MASK) >> SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_SHIFT)
#define  SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE0_SPI_ERR_TOKEN_SHIFT))

#define  SDMMC2_SD_SPI_MODE0_RESERVED3_MASK 0xfe
#define  SDMMC2_SD_SPI_MODE0_RESERVED3_SHIFT 1
#define  SDMMC2_SD_SPI_MODE0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE0_RESERVED3_MASK) >> SDMMC2_SD_SPI_MODE0_RESERVED3_SHIFT)
#define  SDMMC2_SD_SPI_MODE0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE0_RESERVED3_SHIFT))

#define  SDMMC2_SD_SPI_MODE0_SPI_EN_MASK 0x1
#define  SDMMC2_SD_SPI_MODE0_SPI_EN_SHIFT 0
#define  SDMMC2_SD_SPI_MODE0_SPI_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE0_SPI_EN_MASK) >> SDMMC2_SD_SPI_MODE0_SPI_EN_SHIFT)
#define  SDMMC2_SD_SPI_MODE0_SPI_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE0_SPI_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE0_SPI_EN_SHIFT))

//====================================================================
//Register: SPI Mode Register (Instance 2 of 2) (SD_SPI_MODE1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_SPI_MODE1_RESERVED2_MASK 0xe000
#define  SDMMC2_SD_SPI_MODE1_RESERVED2_SHIFT 13
#define  SDMMC2_SD_SPI_MODE1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE1_RESERVED2_MASK) >> SDMMC2_SD_SPI_MODE1_RESERVED2_SHIFT)
#define  SDMMC2_SD_SPI_MODE1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE1_RESERVED2_SHIFT))

#define  SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_MASK 0x1f00
#define  SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_SHIFT 8
#define  SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_MASK) >> SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_SHIFT)
#define  SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE1_SPI_ERR_TOKEN_SHIFT))

#define  SDMMC2_SD_SPI_MODE1_RESERVED3_MASK 0xfe
#define  SDMMC2_SD_SPI_MODE1_RESERVED3_SHIFT 1
#define  SDMMC2_SD_SPI_MODE1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE1_RESERVED3_MASK) >> SDMMC2_SD_SPI_MODE1_RESERVED3_SHIFT)
#define  SDMMC2_SD_SPI_MODE1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE1_RESERVED3_SHIFT))

#define  SDMMC2_SD_SPI_MODE1_SPI_EN_MASK 0x1
#define  SDMMC2_SD_SPI_MODE1_SPI_EN_SHIFT 0
#define  SDMMC2_SD_SPI_MODE1_SPI_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SPI_MODE1_SPI_EN_MASK) >> SDMMC2_SD_SPI_MODE1_SPI_EN_SHIFT)
#define  SDMMC2_SD_SPI_MODE1_SPI_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SPI_MODE1_SPI_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_SPI_MODE1_SPI_EN_SHIFT))

//====================================================================
//Register: Clock and Burst Size Setup Register (Instance 1 of 2) (SD_CLOCK_AND_BURST_SIZE_SETUP0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED2_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_MASK 0x3c00
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_SHIFT 10
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_DELAY_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_MASK 0x300
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_SHIFT 8
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_SDCLK_SEL_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_MASK 0xfe
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_SHIFT 1
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_RESERVED3_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_MASK 0x1
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_SHIFT 0
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP0_BRST_SIZE_SHIFT))

//====================================================================
//Register: Clock and Burst Size Setup Register (Instance 2 of 2) (SD_CLOCK_AND_BURST_SIZE_SETUP1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED2_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_MASK 0x3c00
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_SHIFT 10
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_DELAY_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_MASK 0x300
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_SHIFT 8
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_SDCLK_SEL_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_MASK 0xfe
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_SHIFT 1
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_RESERVED3_SHIFT))

#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_MASK 0x1
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_SHIFT 0
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_MASK) >> SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_SHIFT)
#define  SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_MASK) | (((uint16_t)val) << SDMMC2_SD_CLOCK_AND_BURST_SIZE_SETUP1_BRST_SIZE_SHIFT))

//====================================================================
//Register: CE-ATA Register 0 (Instance 1 of 2) (SD_CE_ATA_00)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CE_ATA_00_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CE_ATA_00_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CE_ATA_00_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_00_RESERVED2_MASK) >> SDMMC2_SD_CE_ATA_00_RESERVED2_SHIFT)
#define  SDMMC2_SD_CE_ATA_00_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_00_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_00_RESERVED2_SHIFT))

#define  SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_MASK 0x3fff
#define  SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_SHIFT 0
#define  SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_MASK) >> SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_SHIFT)
#define  SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_00_CPL_TIMEOUT_SHIFT))

//====================================================================
//Register: CE-ATA Register 0 (Instance 2 of 2) (SD_CE_ATA_01)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CE_ATA_01_RESERVED2_MASK 0xc000
#define  SDMMC2_SD_CE_ATA_01_RESERVED2_SHIFT 14
#define  SDMMC2_SD_CE_ATA_01_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_01_RESERVED2_MASK) >> SDMMC2_SD_CE_ATA_01_RESERVED2_SHIFT)
#define  SDMMC2_SD_CE_ATA_01_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_01_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_01_RESERVED2_SHIFT))

#define  SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_MASK 0x3fff
#define  SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_SHIFT 0
#define  SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_MASK) >> SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_SHIFT)
#define  SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_01_CPL_TIMEOUT_SHIFT))

//====================================================================
//Register: CE-ATA Register 1 (Instance 1 of 2) (SD_CE_ATA_10)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_CE_ATA_10_CHK_CPL_MASK 0x8000
#define  SDMMC2_SD_CE_ATA_10_CHK_CPL_SHIFT 15
#define  SDMMC2_SD_CE_ATA_10_CHK_CPL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_CHK_CPL_MASK) >> SDMMC2_SD_CE_ATA_10_CHK_CPL_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_CHK_CPL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_CHK_CPL_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_CHK_CPL_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_SND_CPL_MASK 0x4000
#define  SDMMC2_SD_CE_ATA_10_SND_CPL_SHIFT 14
#define  SDMMC2_SD_CE_ATA_10_SND_CPL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_SND_CPL_MASK) >> SDMMC2_SD_CE_ATA_10_SND_CPL_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_SND_CPL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_SND_CPL_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_SND_CPL_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_CEATA_CARD_MASK 0x2000
#define  SDMMC2_SD_CE_ATA_10_CEATA_CARD_SHIFT 13
#define  SDMMC2_SD_CE_ATA_10_CEATA_CARD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_CEATA_CARD_MASK) >> SDMMC2_SD_CE_ATA_10_CEATA_CARD_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_CEATA_CARD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_CEATA_CARD_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_CEATA_CARD_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_MMC_CARD_MASK 0x1000
#define  SDMMC2_SD_CE_ATA_10_MMC_CARD_SHIFT 12
#define  SDMMC2_SD_CE_ATA_10_MMC_CARD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_MMC_CARD_MASK) >> SDMMC2_SD_CE_ATA_10_MMC_CARD_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_MMC_CARD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_MMC_CARD_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_MMC_CARD_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_RESERVED2_MASK 0xe00
#define  SDMMC2_SD_CE_ATA_10_RESERVED2_SHIFT 9
#define  SDMMC2_SD_CE_ATA_10_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_RESERVED2_MASK) >> SDMMC2_SD_CE_ATA_10_RESERVED2_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_RESERVED2_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_MMC_WIDTH_MASK 0x100
#define  SDMMC2_SD_CE_ATA_10_MMC_WIDTH_SHIFT 8
#define  SDMMC2_SD_CE_ATA_10_MMC_WIDTH_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_MMC_WIDTH_MASK) >> SDMMC2_SD_CE_ATA_10_MMC_WIDTH_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_MMC_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_MMC_WIDTH_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_MMC_WIDTH_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_RESERVED3_MASK 0x80
#define  SDMMC2_SD_CE_ATA_10_RESERVED3_SHIFT 7
#define  SDMMC2_SD_CE_ATA_10_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_RESERVED3_MASK) >> SDMMC2_SD_CE_ATA_10_RESERVED3_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_RESERVED3_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_MASK 0x40
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_SHIFT 6
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_MASK) >> SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_MASK 0x20
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_SHIFT 5
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_MASK) >> SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_EN_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_MASK 0x10
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_SHIFT 4
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_MASK) >> SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_CPL_COMPLETE_INT_EN_SHIFT))

#define  SDMMC2_SD_CE_ATA_10_RESERVED4_MASK 0xf
#define  SDMMC2_SD_CE_ATA_10_RESERVED4_SHIFT 0
#define  SDMMC2_SD_CE_ATA_10_RESERVED4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_10_RESERVED4_MASK) >> SDMMC2_SD_CE_ATA_10_RESERVED4_SHIFT)
#define  SDMMC2_SD_CE_ATA_10_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_10_RESERVED4_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_10_RESERVED4_SHIFT))

//====================================================================
//Register: CE-ATA Register 1 (Instance 2 of 2) (SD_CE_ATA_11)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_CE_ATA_11_CHK_CPL_MASK 0x8000
#define  SDMMC2_SD_CE_ATA_11_CHK_CPL_SHIFT 15
#define  SDMMC2_SD_CE_ATA_11_CHK_CPL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_CHK_CPL_MASK) >> SDMMC2_SD_CE_ATA_11_CHK_CPL_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_CHK_CPL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_CHK_CPL_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_CHK_CPL_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_SND_CPL_MASK 0x4000
#define  SDMMC2_SD_CE_ATA_11_SND_CPL_SHIFT 14
#define  SDMMC2_SD_CE_ATA_11_SND_CPL_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_SND_CPL_MASK) >> SDMMC2_SD_CE_ATA_11_SND_CPL_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_SND_CPL_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_SND_CPL_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_SND_CPL_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_CEATA_CARD_MASK 0x2000
#define  SDMMC2_SD_CE_ATA_11_CEATA_CARD_SHIFT 13
#define  SDMMC2_SD_CE_ATA_11_CEATA_CARD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_CEATA_CARD_MASK) >> SDMMC2_SD_CE_ATA_11_CEATA_CARD_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_CEATA_CARD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_CEATA_CARD_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_CEATA_CARD_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_MMC_CARD_MASK 0x1000
#define  SDMMC2_SD_CE_ATA_11_MMC_CARD_SHIFT 12
#define  SDMMC2_SD_CE_ATA_11_MMC_CARD_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_MMC_CARD_MASK) >> SDMMC2_SD_CE_ATA_11_MMC_CARD_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_MMC_CARD_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_MMC_CARD_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_MMC_CARD_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_RESERVED2_MASK 0xe00
#define  SDMMC2_SD_CE_ATA_11_RESERVED2_SHIFT 9
#define  SDMMC2_SD_CE_ATA_11_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_RESERVED2_MASK) >> SDMMC2_SD_CE_ATA_11_RESERVED2_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_RESERVED2_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_MMC_WIDTH_MASK 0x100
#define  SDMMC2_SD_CE_ATA_11_MMC_WIDTH_SHIFT 8
#define  SDMMC2_SD_CE_ATA_11_MMC_WIDTH_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_MMC_WIDTH_MASK) >> SDMMC2_SD_CE_ATA_11_MMC_WIDTH_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_MMC_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_MMC_WIDTH_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_MMC_WIDTH_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_RESERVED3_MASK 0x80
#define  SDMMC2_SD_CE_ATA_11_RESERVED3_SHIFT 7
#define  SDMMC2_SD_CE_ATA_11_RESERVED3_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_RESERVED3_MASK) >> SDMMC2_SD_CE_ATA_11_RESERVED3_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_RESERVED3_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_RESERVED3_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_MASK 0x40
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_SHIFT 6
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_MASK) >> SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_MASK 0x20
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_SHIFT 5
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_MASK) >> SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_EN_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_MASK 0x10
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_SHIFT 4
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_MASK) >> SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_CPL_COMPLETE_INT_EN_SHIFT))

#define  SDMMC2_SD_CE_ATA_11_RESERVED4_MASK 0xf
#define  SDMMC2_SD_CE_ATA_11_RESERVED4_SHIFT 0
#define  SDMMC2_SD_CE_ATA_11_RESERVED4_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_CE_ATA_11_RESERVED4_MASK) >> SDMMC2_SD_CE_ATA_11_RESERVED4_SHIFT)
#define  SDMMC2_SD_CE_ATA_11_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_CE_ATA_11_RESERVED4_MASK) | (((uint16_t)val) << SDMMC2_SD_CE_ATA_11_RESERVED4_SHIFT))

//====================================================================
//Register: Slot Interrupt Status Register (Instance 1 of 2) (SD_SLOT_INT_STATUS0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_MASK 0xfffc
#define  SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_SHIFT 2
#define  SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_MASK) >> SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_SHIFT)
#define  SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_SLOT_INT_STATUS0_RESERVED2_SHIFT))

#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_MASK 0x2
#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_SHIFT 1
#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_MASK) >> SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_SHIFT)
#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_MASK) | (((uint16_t)val) << SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT1_SHIFT))

#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_MASK 0x1
#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_SHIFT 0
#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_MASK) >> SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_SHIFT)
#define  SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_MASK) | (((uint16_t)val) << SDMMC2_SD_SLOT_INT_STATUS0_SLOT_INT0_SHIFT))

//====================================================================
//Register: Slot Interrupt Status Register (Instance 2 of 2) (SD_SLOT_INT_STATUS1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_MASK 0xfffc
#define  SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_SHIFT 2
#define  SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_MASK) >> SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_SHIFT)
#define  SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_MASK) | (((uint16_t)val) << SDMMC2_SD_SLOT_INT_STATUS1_RESERVED2_SHIFT))

#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_MASK 0x2
#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_SHIFT 1
#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_MASK) >> SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_SHIFT)
#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_MASK) | (((uint16_t)val) << SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT1_SHIFT))

#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_MASK 0x1
#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_SHIFT 0
#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_MASK) >> SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_SHIFT)
#define  SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_MASK) | (((uint16_t)val) << SDMMC2_SD_SLOT_INT_STATUS1_SLOT_INT0_SHIFT))

//====================================================================
//Register: Host Control Version Register (Instance 1 of 2) (SD_HOST_CTRL_VER0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_MASK 0xff00
#define  SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_SHIFT 8
#define  SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_MASK) >> SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_SHIFT)
#define  SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL_VER0_VENDOR_VER_SHIFT))

#define  SDMMC2_SD_HOST_CTRL_VER0_SD_VER_MASK 0xff
#define  SDMMC2_SD_HOST_CTRL_VER0_SD_VER_SHIFT 0
#define  SDMMC2_SD_HOST_CTRL_VER0_SD_VER_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL_VER0_SD_VER_MASK) >> SDMMC2_SD_HOST_CTRL_VER0_SD_VER_SHIFT)
#define  SDMMC2_SD_HOST_CTRL_VER0_SD_VER_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL_VER0_SD_VER_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL_VER0_SD_VER_SHIFT))

//====================================================================
//Register: Host Control Version Register (Instance 2 of 2) (SD_HOST_CTRL_VER1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_MASK 0xff00
#define  SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_SHIFT 8
#define  SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_MASK) >> SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_SHIFT)
#define  SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL_VER1_VENDOR_VER_SHIFT))

#define  SDMMC2_SD_HOST_CTRL_VER1_SD_VER_MASK 0xff
#define  SDMMC2_SD_HOST_CTRL_VER1_SD_VER_SHIFT 0
#define  SDMMC2_SD_HOST_CTRL_VER1_SD_VER_MASK_SHIFT(reg) (((reg) & SDMMC2_SD_HOST_CTRL_VER1_SD_VER_MASK) >> SDMMC2_SD_HOST_CTRL_VER1_SD_VER_SHIFT)
#define  SDMMC2_SD_HOST_CTRL_VER1_SD_VER_REPLACE_VAL(reg,val) (((reg) & ~SDMMC2_SD_HOST_CTRL_VER1_SD_VER_MASK) | (((uint16_t)val) << SDMMC2_SD_HOST_CTRL_VER1_SD_VER_SHIFT))

#endif // SDMMC2
