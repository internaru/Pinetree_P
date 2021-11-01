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
 * \file PIE_RDMA_ICE_regstructs.h
 * Generated using RegBuild 1.6.2
 *
 *************************************************************************
 */

#ifndef _PIE_RDMA_ICE_REGSTRUCTS_H_
#define _PIE_RDMA_ICE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PIE_RDMA (PIE_RDMA_ICE)
/** \brief Register Descriptions for the multi-channel PIE RDMA block*/
//
//====================================================================

typedef struct PIE_RDMA_ICE_REGS_s
{
  volatile uint32_t CCR;  ///< 0x0 [R/W]: Control Configuration Register
  volatile uint32_t CSR;  ///< 0x4 [R/W]: Control Start Register
  volatile uint32_t CIR;  ///< 0x8 [R/W]: Control Interrupt Register
} PIE_RDMA_ICE_REGS_t;

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 1 of 6) (PIE_RDMA0)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

typedef struct PIE_RDMA0_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: PIE Read DMA Configuration Register
  volatile uint32_t DAR1;  ///< 0x4 [R/W]: PIE Read DMA Address Register 1
  volatile uint32_t DAR2;  ///< 0x8 [R/W]: PIE Read DMA Address Register 2
  volatile uint32_t DRCR1;  ///< 0xc [R/W]: DMA Row Count Register1
  volatile uint32_t DRCR2;  ///< 0x10 [R/W]: DMA Row Count Register2
  volatile uint32_t DBCR;  ///< 0x14 [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x24 [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x34 [W]: DMA Interrupt Register
} PIE_RDMA0_REGS_t;

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 2 of 6) (PIE_RDMA1)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

typedef struct PIE_RDMA1_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: PIE Read DMA Configuration Register
  volatile uint32_t DAR1;  ///< 0x4 [R/W]: PIE Read DMA Address Register 1
  volatile uint32_t DAR2;  ///< 0x8 [R/W]: PIE Read DMA Address Register 2
  volatile uint32_t DRCR1;  ///< 0xc [R/W]: DMA Row Count Register1
  volatile uint32_t DRCR2;  ///< 0x10 [R/W]: DMA Row Count Register2
  volatile uint32_t DBCR;  ///< 0x14 [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x24 [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x34 [W]: DMA Interrupt Register
} PIE_RDMA1_REGS_t;

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 3 of 6) (PIE_RDMA2)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

typedef struct PIE_RDMA2_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: PIE Read DMA Configuration Register
  volatile uint32_t DAR1;  ///< 0x4 [R/W]: PIE Read DMA Address Register 1
  volatile uint32_t DAR2;  ///< 0x8 [R/W]: PIE Read DMA Address Register 2
  volatile uint32_t DRCR1;  ///< 0xc [R/W]: DMA Row Count Register1
  volatile uint32_t DRCR2;  ///< 0x10 [R/W]: DMA Row Count Register2
  volatile uint32_t DBCR;  ///< 0x14 [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x24 [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x34 [W]: DMA Interrupt Register
} PIE_RDMA2_REGS_t;

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 4 of 6) (PIE_RDMA3)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

typedef struct PIE_RDMA3_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: PIE Read DMA Configuration Register
  volatile uint32_t DAR1;  ///< 0x4 [R/W]: PIE Read DMA Address Register 1
  volatile uint32_t DAR2;  ///< 0x8 [R/W]: PIE Read DMA Address Register 2
  volatile uint32_t DRCR1;  ///< 0xc [R/W]: DMA Row Count Register1
  volatile uint32_t DRCR2;  ///< 0x10 [R/W]: DMA Row Count Register2
  volatile uint32_t DBCR;  ///< 0x14 [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x24 [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x34 [W]: DMA Interrupt Register
} PIE_RDMA3_REGS_t;

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 5 of 6) (PIE_RDMA4)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

typedef struct PIE_RDMA4_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: PIE Read DMA Configuration Register
  volatile uint32_t DAR1;  ///< 0x4 [R/W]: PIE Read DMA Address Register 1
  volatile uint32_t DAR2;  ///< 0x8 [R/W]: PIE Read DMA Address Register 2
  volatile uint32_t DRCR1;  ///< 0xc [R/W]: DMA Row Count Register1
  volatile uint32_t DRCR2;  ///< 0x10 [R/W]: DMA Row Count Register2
  volatile uint32_t DBCR;  ///< 0x14 [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x24 [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x34 [W]: DMA Interrupt Register
} PIE_RDMA4_REGS_t;

//====================================================================
//
//Register File: PIE_RDMA channel (Instance 6 of 6) (PIE_RDMA5)
/** \brief Register Descriptions for a single PIE RDMA channel*/
//
//====================================================================

typedef struct PIE_RDMA5_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: PIE Read DMA Configuration Register
  volatile uint32_t DAR1;  ///< 0x4 [R/W]: PIE Read DMA Address Register 1
  volatile uint32_t DAR2;  ///< 0x8 [R/W]: PIE Read DMA Address Register 2
  volatile uint32_t DRCR1;  ///< 0xc [R/W]: DMA Row Count Register1
  volatile uint32_t DRCR2;  ///< 0x10 [R/W]: DMA Row Count Register2
  volatile uint32_t DBCR;  ///< 0x14 [R/W]: DMA Byte Count Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: DMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: DMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: DMA Source Current Address
  volatile uint32_t DCSR;  ///< 0x24 [R]: DMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: DMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: DMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: DMA Results Register
  volatile uint32_t DIR;  ///< 0x34 [W]: DMA Interrupt Register
} PIE_RDMA5_REGS_t;

#endif // PIE_RDMA_ICE
