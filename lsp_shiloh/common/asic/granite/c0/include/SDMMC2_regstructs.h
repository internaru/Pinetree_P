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
 * \file SDMMC2_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SDMMC2_REGSTRUCTS_H_
#define _SDMMC2_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SDMMC2 Card Interface (SDMMC2)
/** \brief Top-level register file for SDMMC2 Host Interface Block*/
//
//====================================================================

typedef struct SDMMC2_REGS_s
{
  volatile uint16_t SD_SYS_ADDR_LOW0;  ///< 0x0 [R/W]: System Address Low Register (Instance 1 of 2)
  volatile uint16_t SD_SYS_ADDR_HIGH0;  ///< 0x2 [R/W]: System Address High Register (Instance 1 of 2)
  volatile uint16_t SD_BLOCK_SIZE0;  ///< 0x4 [R/W]: Block Size Register (Instance 1 of 2)
  volatile uint16_t SD_BLOCK_COUNT0;  ///< 0x6 [R/W]: Block Count Register (Instance 1 of 2)
  volatile uint16_t SD_ARG_LOW0;  ///< 0x8 [R/W]: Argument Low Register (Instance 1 of 2)
  volatile uint16_t SD_ARG_HIGH0;  ///< 0xA [R/W]: Argument High Register (Instance 1 of 2)
  volatile uint16_t SD_TRANSFER_MODE0;  ///< 0xC [R/W]: Transfer Mode Register (Instance 1 of 2)
  volatile uint16_t SD_CMD0;  ///< 0xE [R/W]: Command Register (Instance 1 of 2)
  volatile uint16_t SD_RESP_00;  ///< 0x10 [R]: Response Register 0 (Instance 1 of 2)
  volatile uint16_t SD_RESP_10;  ///< 0x12 [R]: Response Register 1 (Instance 1 of 2)
  volatile uint16_t SD_RESP_20;  ///< 0x14 [R]: Response Register 2 (Instance 1 of 2)
  volatile uint16_t SD_RESP_30;  ///< 0x16 [R/W]: Response Register 3 (Instance 1 of 2)
  volatile uint16_t SD_RESP_40;  ///< 0x18 [R]: Response Register 4 (Instance 1 of 2)
  volatile uint16_t SD_RESP_50;  ///< 0x1A [R]: Response Register 5 (Instance 1 of 2)
  volatile uint16_t SD_RESP_60;  ///< 0x1C [R]: Response Register 6 (Instance 1 of 2)
  volatile uint16_t SD_RESP_70;  ///< 0x1E [R]: Response Register 7 (Instance 1 of 2)
  volatile uint16_t SD_BUFFER_DATA_PORT_00;  ///< 0x20 [R/W]: Buffer Data Port 0 Register (Instance 1 of 2)
  volatile uint16_t SD_BUFFER_DATA_PORT_10;  ///< 0x22 [R/W]: Buffer Data Port 1 Register (Instance 1 of 2)
  volatile uint16_t SD_PRESENT_STATE_00;  ///< 0x24 [R/W]: Present State Register 0 (Instance 1 of 2)
  volatile uint16_t SD_PRESENT_STATE_10;  ///< 0x26 [R]: Present State Register 1 (Instance 1 of 2)
  volatile uint16_t SD_HOST_CTRL0;  ///< 0x28 [R/W]: Host Control Register (Instance 1 of 2)
  volatile uint16_t SD_BLOCK_GAP_CTRL0;  ///< 0x2A [R/W]: Block Gap Control Register (Instance 1 of 2)
  volatile uint16_t SD_CLOCK_CTRL0;  ///< 0x2C [R/W]: Clock Control Register (Instance 1 of 2)
  volatile uint16_t SD_TIMEOUT_CTRL_SW_RESET0;  ///< 0x2E [R/W]: Timeout Control/Software Reset Register (Instance 1 of 2)
  volatile uint16_t SD_NORMAL_INT_STATUS0;  ///< 0x30 [R/W]: Normal Interrupt Status Register (Instance 1 of 2)
  volatile uint16_t SD_ERROR_INT_STATUS0;  ///< 0x32 [R/W]: Error Interrupt Status Register (Instance 1 of 2)
  volatile uint16_t SD_NORMAL_INT_STATUS_EN0;  ///< 0x34 [R/W]: Normal Interrupt Status Enable Register (Instance 1 of 2)
  volatile uint16_t SD_ERROR_INT_STATUS_EN0;  ///< 0x36 [R/W]: Error Interrupt Status Enable Register (Instance 1 of 2)
  volatile uint16_t SD_NORMAL_INT_STATUS_INT_EN0;  ///< 0x38 [R/W]: Normal Interrupt Status Interrupt Enable Register (Instance 1 of 2)
  volatile uint16_t SD_ERROR_INT_STATUS_INT_EN0;  ///< 0x3A [R/W]: Error Interrupt Status Interrupt Enable Register (Instance 1 of 2)
  volatile uint16_t SD_AUTO_CMD12_ERROR_STATUS0;  ///< 0x3C [R/W]: Auto CMD12 Error Status Register (Instance 1 of 2)
  volatile uint16_t reserved0;
  volatile uint16_t SD_CAPABILITIES_00;  ///< 0x40 [R]: Capabilities Register 0 (Instance 1 of 2)
  volatile uint16_t SD_CAPABILITIES_10;  ///< 0x42 [R]: Capabilities Register 1 (Instance 1 of 2)
  volatile uint16_t SD_CAPABILITIES_20;  ///< 0x44 [R]: Capabilities Register 2 (Instance 1 of 2)
  volatile uint16_t SD_CAPABILITIES_30;  ///< 0x46 [R]: Capabilities Register 3 (Instance 1 of 2)
  volatile uint16_t SD_MAX_CURRENT_00;  ///< 0x48 [R]: Maximum Current Register 0 (Instance 1 of 2)
  volatile uint16_t SD_MAX_CURRENT_10;  ///< 0x4A [R]: Maximum Current Register 1 (Instance 1 of 2)
  volatile uint16_t SD_MAX_CURRENT_20;  ///< 0x4C [R]: Maximum Current Register 2 (Instance 1 of 2)
  volatile uint16_t SD_MAX_CURRENT_30;  ///< 0x4E [R]: Maximum Current Register 3 (Instance 1 of 2)
  volatile uint16_t SD_FORCE_EVENT_AUTO_CMD12_ERROR0;  ///< 0x50 [W]: Force Event Auto cmd12 Error Register (Instance 1 of 2)
  volatile uint16_t SD_FORCE_EVENT_FOR_ERROR_STATUS0;  ///< 0x52 [W]: Force Event for Error Status Register (Instance 1 of 2)
  volatile uint16_t SD_ADMA_ERROR_STATUS0;  ///< 0x54 [R/W]: ADMA Error Status Register (Instance 1 of 2)
  volatile uint16_t reserved1;
  volatile uint16_t SD_ADMA_SYS_ADDR_00;  ///< 0x58 [R/W]: ADMA System Address Register 0 (Instance 1 of 2)
  volatile uint16_t SD_ADMA_SYS_ADDR_10;  ///< 0x5A [R/W]: ADMA System Address Register 1 (Instance 1 of 2)
  volatile uint16_t SD_ADMA_SYS_ADDR_20;  ///< 0x5C [R/W]: ADMA System Address Register 2 (Instance 1 of 2)
  volatile uint16_t SD_ADMA_SYS_ADDR_30;  ///< 0x5E [R/W]: ADMA System Address Register 3 (Instance 1 of 2)
  volatile uint16_t reserved2[64];
  volatile uint16_t SD_FIFO_PARAM0;  ///< 0xE0 [R/W]: FIFO Parameters Register (Instance 1 of 2)
  volatile uint16_t reserved3;
  volatile uint16_t SD_SPI_MODE0;  ///< 0xE4 [R/W]: SPI Mode Register (Instance 1 of 2)
  volatile uint16_t SD_CLOCK_AND_BURST_SIZE_SETUP0;  ///< 0xE6 [R/W]: Clock and Burst Size Setup Register (Instance 1 of 2)
  volatile uint16_t SD_CE_ATA_00;  ///< 0xE8 [R/W]: CE-ATA Register 0 (Instance 1 of 2)
  volatile uint16_t SD_CE_ATA_10;  ///< 0xEA [R/W]: CE-ATA Register 1 (Instance 1 of 2)
  volatile uint16_t reserved4[8];
  volatile uint16_t SD_SLOT_INT_STATUS0;  ///< 0xFC [R]: Slot Interrupt Status Register (Instance 1 of 2)
  volatile uint16_t SD_HOST_CTRL_VER0;  ///< 0xFE [R]: Host Control Version Register (Instance 1 of 2)
  volatile uint16_t reserved5[128];
  volatile uint16_t SD_SYS_ADDR_LOW1;  ///< 0x200 [R/W]: System Address Low Register (Instance 2 of 2)
  volatile uint16_t SD_SYS_ADDR_HIGH1;  ///< 0x202 [R/W]: System Address High Register (Instance 2 of 2)
  volatile uint16_t SD_BLOCK_SIZE1;  ///< 0x204 [R/W]: Block Size Register (Instance 2 of 2)
  volatile uint16_t SD_BLOCK_COUNT1;  ///< 0x206 [R/W]: Block Count Register (Instance 2 of 2)
  volatile uint16_t SD_ARG_LOW1;  ///< 0x208 [R/W]: Argument Low Register (Instance 2 of 2)
  volatile uint16_t SD_ARG_HIGH1;  ///< 0x20A [R/W]: Argument High Register (Instance 2 of 2)
  volatile uint16_t SD_TRANSFER_MODE1;  ///< 0x20C [R/W]: Transfer Mode Register (Instance 2 of 2)
  volatile uint16_t SD_CMD1;  ///< 0x20E [R/W]: Command Register (Instance 2 of 2)
  volatile uint16_t SD_RESP_01;  ///< 0x210 [R]: Response Register 0 (Instance 2 of 2)
  volatile uint16_t SD_RESP_11;  ///< 0x212 [R]: Response Register 1 (Instance 2 of 2)
  volatile uint16_t SD_RESP_21;  ///< 0x214 [R]: Response Register 2 (Instance 2 of 2)
  volatile uint16_t SD_RESP_31;  ///< 0x216 [R/W]: Response Register 3 (Instance 2 of 2)
  volatile uint16_t SD_RESP_41;  ///< 0x218 [R]: Response Register 4 (Instance 2 of 2)
  volatile uint16_t SD_RESP_51;  ///< 0x21A [R]: Response Register 5 (Instance 2 of 2)
  volatile uint16_t SD_RESP_61;  ///< 0x21C [R]: Response Register 6 (Instance 2 of 2)
  volatile uint16_t SD_RESP_71;  ///< 0x21E [R]: Response Register 7 (Instance 2 of 2)
  volatile uint16_t SD_BUFFER_DATA_PORT_01;  ///< 0x220 [R/W]: Buffer Data Port 0 Register (Instance 2 of 2)
  volatile uint16_t SD_BUFFER_DATA_PORT_11;  ///< 0x222 [R/W]: Buffer Data Port 1 Register (Instance 2 of 2)
  volatile uint16_t SD_PRESENT_STATE_01;  ///< 0x224 [R/W]: Present State Register 0 (Instance 2 of 2)
  volatile uint16_t SD_PRESENT_STATE_11;  ///< 0x226 [R]: Present State Register 1 (Instance 2 of 2)
  volatile uint16_t SD_HOST_CTRL1;  ///< 0x228 [R/W]: Host Control Register (Instance 2 of 2)
  volatile uint16_t SD_BLOCK_GAP_CTRL1;  ///< 0x22A [R/W]: Block Gap Control Register (Instance 2 of 2)
  volatile uint16_t SD_CLOCK_CTRL1;  ///< 0x22C [R/W]: Clock Control Register (Instance 2 of 2)
  volatile uint16_t SD_TIMEOUT_CTRL_SW_RESET1;  ///< 0x22E [R/W]: Timeout Control/Software Reset Register (Instance 2 of 2)
  volatile uint16_t SD_NORMAL_INT_STATUS1;  ///< 0x230 [R/W]: Normal Interrupt Status Register (Instance 2 of 2)
  volatile uint16_t SD_ERROR_INT_STATUS1;  ///< 0x232 [R/W]: Error Interrupt Status Register (Instance 2 of 2)
  volatile uint16_t SD_NORMAL_INT_STATUS_EN1;  ///< 0x234 [R/W]: Normal Interrupt Status Enable Register (Instance 2 of 2)
  volatile uint16_t SD_ERROR_INT_STATUS_EN1;  ///< 0x236 [R/W]: Error Interrupt Status Enable Register (Instance 2 of 2)
  volatile uint16_t SD_NORMAL_INT_STATUS_INT_EN1;  ///< 0x238 [R/W]: Normal Interrupt Status Interrupt Enable Register (Instance 2 of 2)
  volatile uint16_t SD_ERROR_INT_STATUS_INT_EN1;  ///< 0x23A [R/W]: Error Interrupt Status Interrupt Enable Register (Instance 2 of 2)
  volatile uint16_t SD_AUTO_CMD12_ERROR_STATUS1;  ///< 0x23C [R/W]: Auto CMD12 Error Status Register (Instance 2 of 2)
  volatile uint16_t reserved6;
  volatile uint16_t SD_CAPABILITIES_01;  ///< 0x240 [R]: Capabilities Register 0 (Instance 2 of 2)
  volatile uint16_t SD_CAPABILITIES_11;  ///< 0x242 [R]: Capabilities Register 1 (Instance 2 of 2)
  volatile uint16_t SD_CAPABILITIES_21;  ///< 0x244 [R]: Capabilities Register 2 (Instance 2 of 2)
  volatile uint16_t SD_CAPABILITIES_31;  ///< 0x246 [R]: Capabilities Register 3 (Instance 2 of 2)
  volatile uint16_t SD_MAX_CURRENT_01;  ///< 0x248 [R]: Maximum Current Register 0 (Instance 2 of 2)
  volatile uint16_t SD_MAX_CURRENT_11;  ///< 0x24A [R]: Maximum Current Register 1 (Instance 2 of 2)
  volatile uint16_t SD_MAX_CURRENT_21;  ///< 0x24C [R]: Maximum Current Register 2 (Instance 2 of 2)
  volatile uint16_t SD_MAX_CURRENT_31;  ///< 0x24E [R]: Maximum Current Register 3 (Instance 2 of 2)
  volatile uint16_t SD_FORCE_EVENT_AUTO_CMD12_ERROR1;  ///< 0x250 [W]: Force Event Auto cmd12 Error Register (Instance 2 of 2)
  volatile uint16_t SD_FORCE_EVENT_FOR_ERROR_STATUS1;  ///< 0x252 [W]: Force Event for Error Status Register (Instance 2 of 2)
  volatile uint16_t SD_ADMA_ERROR_STATUS1;  ///< 0x254 [R/W]: ADMA Error Status Register (Instance 2 of 2)
  volatile uint16_t reserved7;
  volatile uint16_t SD_ADMA_SYS_ADDR_01;  ///< 0x258 [R/W]: ADMA System Address Register 0 (Instance 2 of 2)
  volatile uint16_t SD_ADMA_SYS_ADDR_11;  ///< 0x25A [R/W]: ADMA System Address Register 1 (Instance 2 of 2)
  volatile uint16_t SD_ADMA_SYS_ADDR_21;  ///< 0x25C [R/W]: ADMA System Address Register 2 (Instance 2 of 2)
  volatile uint16_t SD_ADMA_SYS_ADDR_31;  ///< 0x25E [R/W]: ADMA System Address Register 3 (Instance 2 of 2)
  volatile uint16_t reserved8[64];
  volatile uint16_t SD_FIFO_PARAM1;  ///< 0x2E0 [R/W]: FIFO Parameters Register (Instance 2 of 2)
  volatile uint16_t reserved9;
  volatile uint16_t SD_SPI_MODE1;  ///< 0x2E4 [R/W]: SPI Mode Register (Instance 2 of 2)
  volatile uint16_t SD_CLOCK_AND_BURST_SIZE_SETUP1;  ///< 0x2E6 [R/W]: Clock and Burst Size Setup Register (Instance 2 of 2)
  volatile uint16_t SD_CE_ATA_01;  ///< 0x2E8 [R/W]: CE-ATA Register 0 (Instance 2 of 2)
  volatile uint16_t SD_CE_ATA_11;  ///< 0x2EA [R/W]: CE-ATA Register 1 (Instance 2 of 2)
  volatile uint16_t reserved10[8];
  volatile uint16_t SD_SLOT_INT_STATUS1;  ///< 0x2FC [R]: Slot Interrupt Status Register (Instance 2 of 2)
  volatile uint16_t SD_HOST_CTRL_VER1;  ///< 0x2FE [R]: Host Control Version Register (Instance 2 of 2)
} SDMMC2_REGS_t;

#endif // SDMMC2
