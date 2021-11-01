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
 * \file DNAND_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DNAND_REGSTRUCTS_H_
#define _DNAND_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Direct NAND Registers (DNAND)
/** \brief This register set includes the registers from NAND Core and Direct NAND Config*/
//
//====================================================================

//====================================================================
//
//Register File: NAND Core Registers (NAND)
/** \brief This file defines the registers for the CHG NAND flash controller*/
//
//====================================================================

typedef struct NAND_REGS_s
{
  volatile uint32_t NDCR;  ///< 0x0 [R/W]: NAND Control Register
  volatile uint32_t NDTR0CS0;  ///< 0x4 [R/W]: NAND Timing Register 0
  volatile uint32_t reserved0;
  volatile uint32_t NDTR1CS0;  ///< 0xc [R/W]: NAND Timing Register 1
  volatile uint32_t reserved1;
  volatile uint32_t NDSR;  ///< 0x14 [R/W]: NAND Status Register
  volatile uint32_t NDPCR;  ///< 0x18 [R]: NAND Page Count Register
  volatile uint32_t NDBBR0;  ///< 0x1c [R]: NAND Bad Block Register 0
  volatile uint32_t NDBBR1;  ///< 0x20 [R]: NAND Bad Block Register 1
  volatile uint32_t NDREDEL;  ///< 0x24 [R/W]: NAND Read Enable Return Delay Register
  volatile uint32_t NDECCCTRL;  ///< 0x28 [R/W]: NAND ECC Control Register
  volatile uint32_t NDBZCNT;  ///< 0x2c [R]: NAND Busy Length Count Register
  volatile uint32_t NDMUTEX;  ///< 0x30 [R/W]: NAND Mutex Lock Register
  volatile uint32_t NDCMDMAT1;  ///< 0x34 [R/W]: NAND Partition Command Match 1
  volatile uint32_t NDCMDMAT2;  ///< 0x38 [R/W]: NAND Partition Command Match 2
  volatile uint32_t NDCMDMAT3;  ///< 0x3c [R/W]: NAND Partition Command Match 3
  volatile uint32_t NDDB;  ///< 0x40 [R/W]: NAND Data Buffer
  volatile uint32_t reserved2;
  volatile uint32_t NDCB0;  ///< 0x48 [R/W]: NAND Command Buffer 0
  volatile uint32_t NDCB1;  ///< 0x4c [R]: NAND Command Buffer 1
  volatile uint32_t NDCB2;  ///< 0x50 [R]: NAND Command Buffer 2
  volatile uint32_t NDCB3;  ///< 0x54 [R]: NAND Command Buffer 3
  volatile uint32_t reserved3;
  volatile uint32_t NDARBCR;  ///< 0x5c [R/W]: NAND Arbitration Control
  volatile uint32_t NDPT0CS0;  ///< 0x60 [R/W]: NAND Partition Table 1 CS0
  volatile uint32_t NDPT1CS0;  ///< 0x64 [R/W]: NAND Partition Table 2 CS0
  volatile uint32_t NDPT2CS0;  ///< 0x68 [R/W]: NAND Partition Table 3 CS0
  volatile uint32_t NDPT3CS0;  ///< 0x6c [R/W]: NAND Partition Table 4 CS0
  volatile uint32_t NDPT0CS1;  ///< 0x70 [R/W]: NAND Partition Table 1 CS1
  volatile uint32_t NDPT1CS1;  ///< 0x74 [R/W]: NAND Partition Table 2 CS1
  volatile uint32_t NDPT2CS1;  ///< 0x78 [R/W]: NAND Partition Table 3 CS1
  volatile uint32_t NDPT3CS1;  ///< 0x7c [R/W]: NAND Partition Table 4 CS1
} NAND_REGS_t;

//====================================================================
//
//Register File: Direct NAND Configuration Registers (CONF)
/** \brief This file defines the registers for the Wrapper for the CHG NAND flash controller*/
//
//====================================================================

typedef struct CONF_REGS_s
{
  volatile uint32_t NDCRP;  ///< 0x0 [R/W]: NAND Control Register
  volatile uint32_t CFG;  ///< 0x4 [R/W]: NAND_PLUS Config Register
  volatile uint32_t WCFG;  ///< 0x8 [R/W]: NAND_PLUS Write Config Register
  volatile uint32_t RCFG;  ///< 0xc [R/W]: NAND_PLUS Read Config Register
  volatile uint32_t ECFG;  ///< 0x10 [R/W]: NAND_PLUS Erase Config Register
  volatile uint32_t STATUS;  ///< 0x14 [R]: NAND_PLUS Status Register
  volatile uint32_t ERR;  ///< 0x18 [W]: NAND_PLUS Error Clear Register
} CONF_REGS_t;

#endif // DNAND
