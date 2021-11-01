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
 * \file I2C_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _I2C_REGSTRUCTS_H_
#define _I2C_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: I2C (I2C)
/** \brief Top-level register file for APB I2C Block*/
//
//====================================================================

typedef struct I2C_REGS_s
{
  volatile uint32_t IC_CON;  ///< 0x0 [R/W]: I2C Control Register
  volatile uint32_t IC_TAR;  ///< 0x4 [R/W]: Target Address Register
  volatile uint32_t IC_SAR;  ///< 0x8 [R/W]: Slave Address Register
  volatile uint32_t IC_HS_MADDR;  ///< 0xc [R/W]: High Speed Mode Code Address Register
  volatile uint32_t IC_DATA_CMD;  ///< 0x10 [R/W]: Tx/Rx DataBuffer and Command register
  volatile uint32_t IC_SS_SCL_HCNT;  ///< 0x14 [R/W]: Standard Speed SCL HIGH Count
  volatile uint32_t IC_SS_SCL_LCNT;  ///< 0x18 [R/W]: Standard Speed SCL Low Count
  volatile uint32_t IC_FS_SCL_HCNT;  ///< 0x1c [R/W]: Fast Speed SCL HIGH Count
  volatile uint32_t IC_FS_SCL_LCNT;  ///< 0x20 [R/W]: FAST Speed SCL Low Count
  volatile uint32_t IC_HS_SCL_HCNT;  ///< 0x24 [R/W]: HIGH Speed SCL HIGH Count
  volatile uint32_t IC_HS_SCL_LCNT;  ///< 0x28 [R/W]: High Speed SCL Low Count
  volatile uint32_t IC_INTR_STAT;  ///< 0x2c [R]: I2C Interrupt Status Register
  volatile uint32_t IC_INTR_MASK;  ///< 0x30 [R/W]: I2C Interrupt Mask Register
  volatile uint32_t IC_RAW_INTR_STAT;  ///< 0x34 [R]: I2C Raw Interrupt Status Register
  volatile uint32_t IC_RX_TL;  ///< 0x38 [R/W]: Rx FIFO Full Threshold
  volatile uint32_t IC_TX_TL;  ///< 0x3c [R/W]: Tx FIFO Empty Threshold
  volatile uint32_t IC_CLR_INTR;  ///< 0x40 [R]: Clear Combined or Individual IRQ Register
  volatile uint32_t IC_CLR_RX_UNDER;  ///< 0x44 [R]: Clear Rx Underrun IRQ Register
  volatile uint32_t IC_CLR_RX_OVER;  ///< 0x48 [R]: Clear Rx Overrun IRQ Register
  volatile uint32_t IC_CLR_TX_OVER;  ///< 0x4c [R]: Clear Tx Overrun IRQ Register
  volatile uint32_t IC_CLR_RD_REQ;  ///< 0x50 [R]: Clear Read Request IRQ Register
  volatile uint32_t IC_CLR_TX_ABRT;  ///< 0x54 [R]: Clear Tx Abort IRQ Register
  volatile uint32_t IC_CLR_RX_DONE;  ///< 0x58 [R]: Clear Rx Done IRQ Register
  volatile uint32_t IC_CLR_ACTIVITY;  ///< 0x5c [R]: Clear Activity IRQ Register
  volatile uint32_t IC_CLR_STOP_DET;  ///< 0x60 [R]: Clear STOP Detect IRQ Register
  volatile uint32_t IC_CLR_START_DET;  ///< 0x64 [R]: Clear START Detect IRQ Register
  volatile uint32_t IC_CLR_GEN_CALL;  ///< 0x68 [R]: Clear General Call IRQ Register
  volatile uint32_t IC_ENABLE;  ///< 0x6c [R/W]: I2C Enable Register
  volatile uint32_t IC_STATUS;  ///< 0x70 [R]: I2C Status Register
  volatile uint32_t IC_TXFLR;  ///< 0x74 [R]: Tx FIFO Level Register
  volatile uint32_t IC_RXFLR;  ///< 0x78 [R]: Rx FIFO Level Register
  volatile uint32_t reserved0;
  volatile uint32_t IC_TX_ABRT_SOURCE;  ///< 0x80 [R/W]: Tx Abort Source Register
  volatile uint32_t IC_SLV_DATA_NACK_ONLY;  ///< 0x84 [R/W]: Generate Slave Data NACK Register
  volatile uint32_t IC_DMA_CR;  ///< 0x88 [R/W]: DMA Control Register
  volatile uint32_t IC_DMA_TDLR;  ///< 0x8c [R/W]: DMA Transmit Data Level Register
  volatile uint32_t IC_DMA_RDLR;  ///< 0x90 [R/W]: DMA Receive Data Level Register
  volatile uint32_t IC_SDA_SETUP;  ///< 0x94 [R/W]: SDA Setup Timing Register
  volatile uint32_t IC_ACK_GENERAL_CALL;  ///< 0x98 [R/W]: General call Register
  volatile uint32_t IC_ENABLE_STATUS;  ///< 0x9c [R]: Enable Status Register
  volatile uint32_t reserved1[21];
  volatile uint32_t IC_COMP_PARAM_1;  ///< 0xf4 [R]: Component Parameter Register
  volatile uint32_t IC_COMP_VERSION;  ///< 0xf8 [R]: Component Version Register
  volatile uint32_t IC_COMP_TYPE;  ///< 0xfc [R]: Component Type Register
} I2C_REGS_t;

#endif // I2C
