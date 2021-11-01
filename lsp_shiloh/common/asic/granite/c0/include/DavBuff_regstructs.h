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
 * \file DavBuff_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DAVBUFF_REGSTRUCTS_H_
#define _DAVBUFF_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Davinci Buffer (DavBuff)
/** \brief Top-level register file for Davinci Buffer block*/
//
//====================================================================

typedef struct DAVBUFF_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: Configuration Register
  volatile uint32_t DMA_START;  ///< 0x4 [R/W]: DMA Start Address
  volatile uint32_t DMA_END;  ///< 0x8 [R/W]: DMA End Addr
  volatile uint32_t STAT_RD;  ///< 0xc [R]: Status Read Register
  volatile uint32_t STAT_WR;  ///< 0x10 [W]: Status Write Register
  volatile uint32_t SR_CTRL;  ///< 0x14 [W]: Store/Restore Control Register
  volatile uint32_t SR_STAT;  ///< 0x18 [R]: Store/Restore Status Register
  volatile uint32_t DEBUG_PIX_ADDR;  ///< 0x1c [R/W]: Debug Address Register
  volatile uint32_t DEBUG_DATA_IDX;  ///< 0x20 [R/W]: Debug Data Index Register
  volatile uint32_t DEBUG_DATA_WR;  ///< 0x24 [W]: Debug Data Write Register
  volatile uint32_t DEBUG_DATA_RD;  ///< 0x28 [R]: Debug Data Read Register
  volatile uint32_t DEBUG_GO;  ///< 0x2c [W]: Debug Go Write Register
  volatile uint32_t DEBUG_STAT;  ///< 0x30 [R]: Debug Status Register
  volatile uint32_t SRAM_START;  ///< 0x34 [R/W]: SRAM Start Address
  volatile uint32_t SRAM_END;  ///< 0x38 [R/W]: SRAM End Address
  volatile uint32_t INT_EN;  ///< 0x3c [R/W]: Interrupt Enable
  volatile uint32_t INT_PEND;  ///< 0x40 [R]: Interrupt Pending
  volatile uint32_t INT_ACK;  ///< 0x44 [W]: Interrupt Acknowledge
  volatile uint32_t INT_FORCE;  ///< 0x48 [W]: Interrupt Force
} DAVBUFF_REGS_t;

#endif // DAVBUFF
