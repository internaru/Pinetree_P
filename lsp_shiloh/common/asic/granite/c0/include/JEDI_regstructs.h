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
 * \file JEDI_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _JEDI_REGSTRUCTS_H_
#define _JEDI_REGSTRUCTS_H_

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

typedef struct RDMA_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: JPEG RDMA Configuration Register
  volatile uint32_t DAR1;  ///< 0x4 [R/W]: JPEG RDMA Address 1 Register
  volatile uint32_t DAR2;  ///< 0x8 [R/W]: JPEG RDMA Address 2 Register
  volatile uint32_t DRCR1;  ///< 0xc [R/W]: JPEG RDMA Row Count 1 Register
  volatile uint32_t DRCR2;  ///< 0x10 [R/W]: JPEG RDMA Row Count 2 Register
  volatile uint32_t DLWR;  ///< 0x14 [R/W]: JPEG RDMA Line Width Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: JPEG RDMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: JPEG RDMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: JPEG RDMA Address Status Register
  volatile uint32_t DTCSR;  ///< 0x24 [R]: JPEG RDMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: JPEG RDMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: JPEG RDMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: JPEG RDMA Result Register
  volatile uint32_t DIR;  ///< 0x34 [W]: JPEG RDMA Interrupt Register
} RDMA_REGS_t;

//====================================================================
//
//Register File: WDMA Registers (WDMA)
/** \brief This file defines the registers for the Write DMA for the JPEG Super Block*/
//
//====================================================================

typedef struct WDMA_REGS_s
{
  volatile uint32_t DCR;  ///< 0x0 [R/W]: JPEG WDMA Configuration Register
  volatile uint32_t DAR;  ///< 0x4 [R/W]: JPEG WDMA Address Register
  volatile uint32_t reserved0;
  volatile uint32_t DRCR;  ///< 0xc [R/W]: JPEG WDMA Row Count Register
  volatile uint32_t DIHR;  ///< 0x10 [R/W]: JPEG WDMA Image Height Register
  volatile uint32_t DLWR;  ///< 0x14 [R/W]: JPEG WDMA Line Width Register
  volatile uint32_t DTCR;  ///< 0x18 [R/W]: JPEG WDMA Transfer Count Register
  volatile uint32_t DSTR;  ///< 0x1c [W]: JPEG WDMA Start Register
  volatile uint32_t DASR;  ///< 0x20 [R]: JPEG WDMA Address Status Register
  volatile uint32_t DTCSR;  ///< 0x24 [R]: JPEG WDMA Transfer Count Status Register
  volatile uint32_t DRCSR;  ///< 0x28 [R]: JPEG WDMA Row Count Status Register
  volatile uint32_t DSR;  ///< 0x2c [R]: JPEG WDMA Status Register
  volatile uint32_t DRR;  ///< 0x30 [R]: JPEG WDMA Result Register
  volatile uint32_t DIR;  ///< 0x34 [W]: JPEG WDMA Interrupt Register
} WDMA_REGS_t;

//====================================================================
//
//Register File: Color Space Conversion Registers (CSC)
/** \brief Register Descriptions for the 1 CSC block*/
//
//====================================================================

typedef struct CSC_REGS_s
{
  volatile uint32_t CCR;  ///< 0x0 [R/W]: CSC Configuration Register
  volatile uint32_t reserved0[63];
  volatile uint32_t MCR0;  ///< 0x100 [R/W]: Multiply Coefficient Register (Instance 1 of 9)
  volatile uint32_t MCR1;  ///< 0x104 [R/W]: Multiply Coefficient Register (Instance 2 of 9)
  volatile uint32_t MCR2;  ///< 0x108 [R/W]: Multiply Coefficient Register (Instance 3 of 9)
  volatile uint32_t MCR3;  ///< 0x10c [R/W]: Multiply Coefficient Register (Instance 4 of 9)
  volatile uint32_t MCR4;  ///< 0x110 [R/W]: Multiply Coefficient Register (Instance 5 of 9)
  volatile uint32_t MCR5;  ///< 0x114 [R/W]: Multiply Coefficient Register (Instance 6 of 9)
  volatile uint32_t MCR6;  ///< 0x118 [R/W]: Multiply Coefficient Register (Instance 7 of 9)
  volatile uint32_t MCR7;  ///< 0x11c [R/W]: Multiply Coefficient Register (Instance 8 of 9)
  volatile uint32_t MCR8;  ///< 0x120 [R/W]: Multiply Coefficient Register (Instance 9 of 9)
} CSC_REGS_t;

//====================================================================
//
//Register File: MCU_JPEG Registers (MCU_JPEG)
/** \brief Top-level register file for MCU JPEG block*/
//
//====================================================================

typedef struct MCU_JPEG_REGS_s
{
  volatile uint32_t MJEN;  ///< 0x0 [W]: MCU_JPEG Enable
  volatile uint32_t MJCTL;  ///< 0x4 [R/W]: MCU_JPEG Control
  volatile uint32_t MJSCL;  ///< 0x8 [R/W]: MCU_JPEG Scaler Control
  volatile uint32_t reserved0;
  volatile uint32_t JMACTL;  ///< 0x10 [R/W]: JPEG Memory Access Control
  volatile uint32_t JMDATA;  ///< 0x14 [R/W]: JPEG Memory Data
  volatile uint32_t JMOCS;  ///< 0x18 [R/W]: JPEG Memory Operation Control/Status
  volatile uint32_t MJINT;  ///< 0x1c [R/W]: MCU_JPEG Over
  volatile uint32_t MJRST;  ///< 0x20 [R/W]: Soft Reset
  volatile uint32_t JREG0;  ///< 0x24 [W]: JPEG Core Internal Register 0
  volatile uint32_t JREG1;  ///< 0x28 [R/W]: JPEG Core Internal Register 1
  volatile uint32_t JREG2;  ///< 0x2c [R/W]: JPEG Core Internal Register 2
  volatile uint32_t JREG3;  ///< 0x30 [R/W]: JPEG Core Internal Register 3
  volatile uint32_t JREG4;  ///< 0x34 [R/W]: JPEG Core Internal Register 4
  volatile uint32_t JREG5;  ///< 0x38 [R/W]: JPEG Core Internal Register 5
  volatile uint32_t JREG6;  ///< 0x3c [R/W]: JPEG Core Internal Register 6
  volatile uint32_t JREG7;  ///< 0x40 [R/W]: JPEG Core Internal Register 7
} MCU_JPEG_REGS_t;

#endif // JEDI
