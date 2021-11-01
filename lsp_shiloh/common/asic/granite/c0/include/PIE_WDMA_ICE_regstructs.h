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
 * \file PIE_WDMA_ICE_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PIE_WDMA_ICE_REGSTRUCTS_H_
#define _PIE_WDMA_ICE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIE_WDMA (PIE_WDMA_ICE)
/** \brief Register Descriptions for the multi-channel PIE WDMA block*/
//
//====================================================================

typedef struct PIE_WDMA_ICE_REGS_s
{
  volatile uint32_t reserved0[48];
  volatile uint32_t DGCR;  ///< 0xc0 [R/W]: DMA General Control Register
  volatile uint32_t CIR;  ///< 0xc4 [R/W]: Control Interrupt Register
} PIE_WDMA_ICE_REGS_t;

//====================================================================
//
//Register File: PIE_WDMA channel (Instance 1 of 3) (PIE_WDMA0)
/** \brief Register Descriptions for a single PIE WDMA channel*/
//
//====================================================================

typedef struct PIE_WDMA0_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t DAR;  ///< 0x4 [R/W]: PIE Write DMA Address Register
  volatile uint32_t DRCR;  ///< 0x8 [R/W]: DMA Row Count Register
  volatile uint32_t DBCR;  ///< 0xc [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x10 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x14 [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x18 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x1c [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x20 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x24 [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x28 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x2c [W]: DMA Interrupt Register
} PIE_WDMA0_REGS_t;

//====================================================================
//
//Register File: PIE_WDMA channel (Instance 2 of 3) (PIE_WDMA1)
/** \brief Register Descriptions for a single PIE WDMA channel*/
//
//====================================================================

typedef struct PIE_WDMA1_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t DAR;  ///< 0x4 [R/W]: PIE Write DMA Address Register
  volatile uint32_t DRCR;  ///< 0x8 [R/W]: DMA Row Count Register
  volatile uint32_t DBCR;  ///< 0xc [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x10 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x14 [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x18 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x1c [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x20 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x24 [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x28 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x2c [W]: DMA Interrupt Register
} PIE_WDMA1_REGS_t;

//====================================================================
//
//Register File: PIE_WDMA channel (Instance 3 of 3) (PIE_WDMA2)
/** \brief Register Descriptions for a single PIE WDMA channel*/
//
//====================================================================

typedef struct PIE_WDMA2_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t DAR;  ///< 0x4 [R/W]: PIE Write DMA Address Register
  volatile uint32_t DRCR;  ///< 0x8 [R/W]: DMA Row Count Register
  volatile uint32_t DBCR;  ///< 0xc [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x10 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x14 [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x18 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x1c [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x20 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x24 [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x28 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x2c [W]: DMA Interrupt Register
} PIE_WDMA2_REGS_t;

#endif // PIE_WDMA_ICE
